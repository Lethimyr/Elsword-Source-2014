#pragma once 


#include "stdafx.h"
#include "../Kom2/kom.h"
#define HTTP_BUFFERSIZE 2048


struct SRecieveInfo
{
	SRecieveInfo(std::string u,std::ostream &strm,int off,int len,NETPROGRESS_CALLBACK prog,SProgressInfo *proginfo)
		:url(u),stream(strm),offset(off),length(len),readlength(0),progress(prog),progressinfo(proginfo)
	{
		std::transform(url.begin(),url.end(),url.begin(),tolower);
	}

	NETPROGRESS_CALLBACK progress;
	SProgressInfo	* progressinfo;
	std::string url;
	std::ostream & stream;
	int	offset;
	int length;
	int readlength;
};

class KCurlSession : public KWriterSession 
{
public:
	

	static void GlobalInit(){ curl_global_init( CURL_GLOBAL_WIN32 );};
	static void GlobalUninit(){ curl_global_cleanup();};
	static bool SplitURL(std::string url,std::string &server,std::string &path);

	
	KCurlSession();
	KCurlSession(std::string id,std::string pwd);
	virtual ~KCurlSession();

	bool WriteData(std::string url,std::ostream &stream,int off = 0,int length=-1,NETPROGRESS_CALLBACK progress=NULL,SProgressInfo * progressinfo =NULL,NETERROR_CALLBACK = NULL);
	bool DownloadFile(std::string url,std::string filename,int off = 0,int length=-1,NETPROGRESS_CALLBACK progress=NULL,SProgressInfo * progressinfo =NULL,NETERROR_CALLBACK = NULL);
private:	


	CURL * m_ctx;
};