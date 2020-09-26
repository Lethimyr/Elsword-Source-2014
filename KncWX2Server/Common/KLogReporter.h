#pragma once

#include <string>
#include <vector>

#include "KFtpWrapper.h"

class KLogReporter
{

public:
	enum ELRError
	{
		eLRError_OK,
		eLRError_ConnectFailed,
		eLRError_ChangePathFailed,
		eLRError_WriteFileFailed,
		eLRError_Max
	};

public:
	KLogReporter();

	void SetConnectInfo(std::wstring strIP, int iPort = -1);
	void SetAccount(std::wstring strUserID, std::wstring strUserPassword);
	void SetUploadPath(std::wstring strPath);

	void InsertFile(std::wstring strLocalFilename, std::wstring strRemoteFilename,
		KFtpWrapper::EFileType eFileType = KFtpWrapper::eFileTypeBinary);

	KLogReporter::ELRError Send();

protected:
	std::wstring m_strIP;
	int m_iPort;
	
	std::wstring m_strUserID;
	std::wstring m_strUserPassword;

	std::wstring m_strUploadPath;


public:
	struct KFileInfo
	{
		std::wstring m_strLocalFileName;
		std::wstring m_strRemoteFileName;
		KFtpWrapper::EFileType m_eFileType;

		KFileInfo(){};
		KFileInfo( const KFileInfo& t ) { *this = t; }    // copy constructor
		KFileInfo& operator=( const KFileInfo& t )       // assign operator
		{
			m_strLocalFileName = t.m_strLocalFileName;
			m_strRemoteFileName = t.m_strRemoteFileName;
			m_eFileType = t.m_eFileType;

			return *this;
		}
	};

	std::vector<KFileInfo> m_vecFileList;

};