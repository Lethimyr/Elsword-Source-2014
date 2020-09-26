#pragma once

#include <Windows.h>
#include <WinInet.h>
#include <string>

#pragma comment (lib, "wininet")

class KFtpWrapper
{

public:
	enum EFileType
	{
		eFileTypeBinary,
		eFileTypeAscii,
		eFileTypeMax
	};

public:
	KFtpWrapper();
	~KFtpWrapper();

	BOOL Connect(std::wstring strAddress, std::wstring strUserID, 
		std::wstring strPassword, int iPort = INTERNET_DEFAULT_FTP_PORT);
	void Close();

	BOOL SetCurrentDirectory(std::wstring strPath);

	BOOL GetFile(std::wstring strLocalFile, std::wstring strRemoteFile, EFileType eFileType);
	BOOL PutFile(std::wstring strLocalFile, std::wstring strRemoteFile, EFileType eFileType);

	DWORD GetTransferType(EFileType eFileType);

protected:
	HINTERNET m_hInternet;
	HINTERNET m_hServer;

};