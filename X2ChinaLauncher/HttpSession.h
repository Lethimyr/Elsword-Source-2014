#pragma once

#include "define.h"

#include <windows.h>
#include <wininet.h>
#include <string>
#include <algorithm>
#include <stdio.h>

#define HTTP_BUFFERSIZE 2048

#ifdef GATHER_PATCH_INFO
typedef bool (*TOTAL_PROGRESS_CALLBACK)(float) ;
typedef bool (*FILE_PROGRESS_CALLBACK)(float, float) ;
#else GATHER_PATCH_INFO
typedef bool (*TOTAL_PROGRESS_CALLBACK)(float) ;
typedef bool (*FILE_PROGRESS_CALLBACK)(float) ;
#endif GATHER_PATCH_INFO

class KHttpSession
{
public:
	KHttpSession();
	virtual ~KHttpSession(void);
	
	bool OpenSession(std::string sessionname);
	void CloseSession();


	bool Connect(std::string servername, unsigned int port=INTERNET_DEFAULT_HTTP_PORT);
	void Disconnect();
	bool Open( std::string url,int offset=-1,int length=-1 ,int* query=NULL);
	
	void Close();
	unsigned int ReadFile(LPVOID buffer,int len)
	{
		DWORD readlength;
		InternetReadFile(m_hRequest, buffer , len , &readlength);
		return readlength;
	}

	bool Write(std::ostream &stream,int totallen,FILE_PROGRESS_CALLBACK progress);
	bool Write(std::ostream &stream,FILE_PROGRESS_CALLBACK progress = NULL);
	static bool SplitURL(std::string url,std::string &server,std::string &path);

private:	
	HINTERNET m_hSession;
	HINTERNET m_hConnection;
	HINTERNET m_hRequest;

	char * buffer;

	std::string sessionname;
};
