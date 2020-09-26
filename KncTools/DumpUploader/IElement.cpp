#include "StdAfx.h"
#include ".\ielement.h"

IElement::IElement(void)
{
    _SetType( ELEMENTTYPE_NONE );
    m_bValid = false;
}

IElement::~IElement(void)
{
}

IElement::ELEMENTTYPE IElement::GetType()
{
    return m_eType;
}

void IElement::_SetValid( bool bValid )
{
    m_bValid = bValid;
}
bool IElement::IsValid()
{
    return m_bValid;
}

void IElement::_SetType( IElement::ELEMENTTYPE eType )
{
    m_eType = eType;
}


//------------------------------------------------------------------------------------------------//

CElementFileInfo::CElementFileInfo()
{
   _SetType( ELEMENTTYPE_FILEINFO );
   m_bUsable = false;
}

CElementFileInfo::~CElementFileInfo()
{
}

void CElementFileInfo::ValidChk()
{
    _SetValid( !m_strFileName.IsEmpty() );
}
void CElementFileInfo::SetFileName( char* strFileName )
{
    m_strFileName = strFileName;
}
char* CElementFileInfo::GetFileName()
{
    return m_strFileName.c_str();
}

bool CElementFileInfo::GetUserble()
{
    return m_bUsable;
}

bool CElementFileInfo::ChkUserble()
{
    CFile file;
    bool bRet = false;
    if( file.Open( m_strFileName.c_str(), CFile::modeRead | CFile::shareExclusive ) )
    {
        bRet = true;
        file.Close();
    }//end if
    m_bUsable = bRet;
    return bRet;
}
//------------------------------------------------------------------------------------------------//

CElementMailInfo::CElementMailInfo()
{
    _SetType( ELEMENTTYPE_MAILINFO );
}

CElementMailInfo::~CElementMailInfo()
{
}

char* CElementMailInfo::GetMailAddress()
{
    return m_strMailAddress.c_str();
}

char* CElementMailInfo::GetSMTPAddress()
{
    return m_strSTMPAddress.c_str();
}

void CElementMailInfo::SetMailAddress( char* strMailAddress )
{
    m_strMailAddress = strMailAddress;
}

void CElementMailInfo::SetSMTPAddress( char* strSMTPAddress )
{
    m_strSTMPAddress = strSMTPAddress;
}

void CElementMailInfo::ValidChk()
{
    _SetValid( !m_strMailAddress.IsEmpty() &&  !m_strSTMPAddress.IsEmpty() );

}
//------------------------------------------------------------------------------------------------//

CElementFTPInfo::CElementFTPInfo()
{
    _SetType( ELEMENTTYPE_FTPINFO );
}
CElementFTPInfo::~CElementFTPInfo()
{
}

char* CElementFTPInfo::GetFTPAddress()
{
    return m_strFTPAddress;
}
char* CElementFTPInfo::GetFTPPort()
{

    return m_strFTPPort;
}
char* CElementFTPInfo::GetUserID()
{
    return m_strUserID;
    
}
char* CElementFTPInfo::GetUserPW()
{
    return m_strUserPW;
}

void CElementFTPInfo::SetFTPAddress( char* strFTPAddress )
{
    m_strFTPAddress = strFTPAddress;
    

}
void CElementFTPInfo::SetFTPPort( char* strFTPPort )
{
    m_strFTPPort    = strFTPPort;
    
}
void CElementFTPInfo::SetUserID( char* strUserID )
{
    m_strUserID     = strUserID;

}
void CElementFTPInfo::SetUserPW( char* strUserPW )
{
    m_strUserPW     = strUserPW;
}

void CElementFTPInfo::ValidChk()
{
    _SetValid( !m_strFTPAddress.IsEmpty() &&
               !m_strFTPPort.IsEmpty() &&
               !m_strUserID.IsEmpty() &&
               !m_strUserPW.IsEmpty() );
}

//------------------------------------------------------------------------------------------------//


CElementCrashInfo::CElementCrashInfo()
{
    _SetType( ELEMENTTYPE_CRASH_SERVERINFO );
}
CElementCrashInfo::~CElementCrashInfo()
{
}
char* CElementCrashInfo::GetCrashInfo()
{
    return m_strCrashServer;
}
void CElementCrashInfo::SetCrashInfo( char* strCrashServer )
{
    m_strCrashServer = strCrashServer;
}

void CElementCrashInfo::ValidChk()
{
    _SetValid( !m_strCrashServer.IsEmpty() );
}
//------------------------------------------------------------------------------------------------//

IElement* ElementFactory::GetElement( IElement::ELEMENTTYPE eType )
{
    IElement* pElement = NULL;
    switch( eType )
    {
    case IElement::ELEMENTTYPE::ELEMENTTYPE_FILEINFO:
        {
            pElement = new CElementFileInfo;
        }
        break;
    case IElement::ELEMENTTYPE::ELEMENTTYPE_MAILINFO:
        {
            pElement = new CElementMailInfo;
        }
        break;
    case IElement::ELEMENTTYPE::ELEMENTTYPE_FTPINFO:
        {
            pElement = new CElementFTPInfo;
        }
        break;
    case IElement::ELEMENTTYPE::ELEMENTTYPE_CRASH_SERVERINFO:
        {
            pElement = new CElementCrashInfo;
        }
        break;
    default:
        
        break;
    }//end switch

    //Log

    return pElement;
}
