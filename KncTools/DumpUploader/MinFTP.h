#pragma once
#include <afxinet.h>
#include "IElement.h"
class CMinFTP
{
public:
    CMinFTP(void);
    ~CMinFTP(void);
    bool SetFTPInfo( IElement* pElement );
    bool FTPConnection();
    bool UpLoadFile( char* strFileName, char* strFolderName );
    

private:
    bool CloseFTP();
    CElementFTPInfo*    m_pFTPServerInfo;
    CStringEx           m_strFileName;

    CInternetSession    m_Session;
    CFtpConnection*     m_pFTPConnection;

};
