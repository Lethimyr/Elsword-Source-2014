#include "KLogReporter.h"

//#include "KMsgBox.h"

//---------------------------------------------------------------------------
KLogReporter::KLogReporter()
{
	m_strUserID = L"anonymous";
	m_strUserPassword = L"";

	m_strIP = L"127.0.0.1";
	m_iPort = -1;

	m_strUploadPath = L"Root";
}
//---------------------------------------------------------------------------
void KLogReporter::SetConnectInfo(std::wstring strIP, int iPort)
{
	m_strIP = strIP;
	m_iPort = iPort;
}
//---------------------------------------------------------------------------
void KLogReporter::SetAccount(std::wstring strUserID, std::wstring strUserPassword)
{
	m_strUserID = strUserID;
	m_strUserPassword = strUserPassword;
}
//---------------------------------------------------------------------------
void KLogReporter::SetUploadPath(std::wstring strPath)
{
	m_strUploadPath = strPath;
}
//---------------------------------------------------------------------------
void KLogReporter::InsertFile(std::wstring strLocalFilename, std::wstring strRemoteFilename, KFtpWrapper::EFileType eFileType)
{
	KFileInfo kFileInfo;
	kFileInfo.m_strLocalFileName = strLocalFilename;
	kFileInfo.m_strRemoteFileName = strRemoteFilename;
	kFileInfo.m_eFileType = eFileType;

	m_vecFileList.push_back(kFileInfo);
}
//---------------------------------------------------------------------------
KLogReporter::ELRError KLogReporter::Send()
{
	KFtpWrapper kFtpSession;
	BOOL bRes = TRUE;

	if (m_iPort == -1)
	{
		bRes = kFtpSession.Connect(m_strIP, m_strUserID, m_strUserPassword);
	}
	else
	{
		bRes = kFtpSession.Connect(m_strIP, m_strUserID, m_strUserPassword, m_iPort);
	}

	if (!bRes)
	{
		return eLRError_ConnectFailed;
	}

	if (m_strUploadPath != L"Root")
	{
		bRes = kFtpSession.SetCurrentDirectory(m_strUploadPath);
		if (!bRes)
		{
			return eLRError_ChangePathFailed;
		}
	}

	KLogReporter::ELRError eErr = eLRError_OK;

	unsigned int iF;
	for (iF = 0; iF < m_vecFileList.size(); ++iF)
	{
		KFileInfo& kFileInfo = m_vecFileList[iF];
		bRes = kFtpSession.PutFile(kFileInfo.m_strLocalFileName, 
			kFileInfo.m_strRemoteFileName, kFileInfo.m_eFileType);

		if (!bRes)
		{
			eErr = eLRError_WriteFileFailed;
		}
	}

	return eLRError_OK;
}
//---------------------------------------------------------------------------