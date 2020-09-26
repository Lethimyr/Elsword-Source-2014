#include "KFtpWrapper.h"

//---------------------------------------------------------------------------
KFtpWrapper::KFtpWrapper()
{
	m_hInternet = NULL;
	m_hServer = NULL;
}
//---------------------------------------------------------------------------
KFtpWrapper::~KFtpWrapper()
{
	Close();
}
//---------------------------------------------------------------------------
BOOL KFtpWrapper::Connect(std::wstring strAddress, std::wstring strUserID, std::wstring strPassword, int iPort)
{
	m_hInternet = ::InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!m_hInternet) return false;

	m_hServer = ::InternetConnect(m_hInternet, strAddress.c_str(), (INTERNET_PORT) iPort,
		strUserID.c_str(), strPassword.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	if (!m_hServer) return false;

	return true;
}
//---------------------------------------------------------------------------
void KFtpWrapper::Close()
{
	if (m_hServer != NULL)
	{
		::InternetCloseHandle(m_hServer);
		m_hServer = NULL;
	}

	if (m_hInternet != NULL)
	{
		::InternetCloseHandle(m_hInternet);
		m_hInternet = NULL;
	}
}
//---------------------------------------------------------------------------
BOOL KFtpWrapper::SetCurrentDirectory(std::wstring strPath)
{
	return ::FtpSetCurrentDirectory(m_hServer, strPath.c_str());
}
//---------------------------------------------------------------------------
BOOL KFtpWrapper::GetFile(std::wstring strLocalFile, std::wstring strRemoteFile, EFileType eFileType)
{
	DWORD dwFlag = GetTransferType(eFileType);

	return ::FtpGetFile(m_hServer, strRemoteFile.c_str(), 
		strLocalFile.c_str(), false, FILE_ATTRIBUTE_NORMAL, dwFlag, 0);
}
//---------------------------------------------------------------------------
BOOL KFtpWrapper::PutFile(std::wstring strLocalFile, std::wstring strRemoteFile, EFileType eFileType)
{
	DWORD dwFlag = GetTransferType(eFileType);

	return ::FtpPutFile(m_hServer, strLocalFile.c_str(), strRemoteFile.c_str(), dwFlag, 0);
}
//---------------------------------------------------------------------------
DWORD KFtpWrapper::GetTransferType(EFileType eFileType)
{
	switch (eFileType)
	{
	case eFileTypeBinary:
		return FTP_TRANSFER_TYPE_BINARY;
	case eFileTypeAscii:
		return FTP_TRANSFER_TYPE_ASCII;
	}

	return 0;
}
//---------------------------------------------------------------------------
