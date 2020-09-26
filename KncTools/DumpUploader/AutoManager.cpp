#include "StdAfx.h"
#include ".\automanager.h"
#include "IniReader.h"
#include "Zipper.h"
#include "MailSender.h"
#include "MinFTP.h"
#include "Logger.h"
extern CLogger* g_Logger;

CAutoManager::CAutoManager(void)
{
    m_strINIFile    = ".\\DumpUploader.ini";
    m_vecFileElement.reserve(3);
    m_vecMailElement.reserve(3);
    
}

CAutoManager::~CAutoManager(void)
{
    ITVECELEMENT it;
    ITVECELEMENT itEnd;

    it = m_vecFileElement.begin();
    itEnd = m_vecFileElement.end();
    while( it != itEnd )
    {
        delete (*it);
        ++it;
    }
    m_vecFileElement.clear();

    
    
    it = m_vecMailElement.begin();
    itEnd = m_vecMailElement.end();
    while( it != itEnd )
    {
        delete (*it);
        ++it;
    }
    m_vecMailElement.clear();

    if( g_Logger )
    {
        delete g_Logger;
        g_Logger = NULL;
    }//end if
}


bool CAutoManager::Process( int argc, char* argv[] )
{
    if( argc != ARGC_COUNT )
        return false;

    if( g_Logger == NULL)
    {
        g_Logger = new CLogger;
    }//end if

    if(!_LoadParam( argc, argv ) )
    {
        if(g_Logger)
            g_Logger->AddLog( "입력 파라미터 문제\n" );

        _CleanUp();
        return false;
    }

    if( !_ValidChk() )
    {
        if(g_Logger)
            g_Logger->AddLog( "파라미터 가 유효하지 않음\n" );

        _CleanUp();
        return false;
    }

    if( !_WaitFileCompete( 50 ) )
    {
        if(g_Logger)
            g_Logger->AddLog( "생성되는 덤프파일에 문제 있음\n" );

        _CleanUp();
        return false;
    }

    if( !_Zipping() )
    {
        if(g_Logger)
            g_Logger->AddLog( "압축 실패\n" );

        _CleanUp();
        return false;
    }

    if( !_UploadToFTP() )
    {
        if(g_Logger)
            g_Logger->AddLog( "FTP업로드 실패\n" );

        _CleanUp();
        return false;
    }

    _CleanUp();
    
    return true;

}

bool CAutoManager::_LoadParam( int argc, char* argv[] )
{
    int i=0;
    int nMailCount=0;
    char* pChar;
    CStringEx strTmp;
//
    for( i = 0 ; i < argc ; ++i )
    {
        if( g_Logger )
        {
            CStringEx strLog;
            strLog.Format("argc = %d, str = %s\n", i, argv[i] );
            g_Logger->AddLog(strLog.c_str());
        }//end if
    }//end for
//
    for( i = 1 ; i < argc ; i+=2 )
    {
        if( (strcmp( argv[i], "/f") == 0)  || (strcmp( argv[i], "/F") == 0) )
        {
            IElement* pElement = ElementFactory::GetElement( IElement::ELEMENTTYPE::ELEMENTTYPE_FILEINFO );
            if( pElement == NULL )
            {
                if( g_Logger )
                    g_Logger->AddLog("입력 파라미터 처리 엘레먼트 생성 실패");
                return false;
            }
            ((CElementFileInfo*)pElement)->SetFileName( argv[i+1]);
            m_vecFileElement.push_back( pElement );
        }
        else if( (strcmp( argv[i], "/s") == 0)  || (strcmp( argv[i], "/s") == 0))
        {
            m_elServerName.SetCrashInfo( argv[i+1] );
        }
        else
        {
            if( g_Logger )
                g_Logger->AddLog("잘못된 입펵 파라미터 발견\n");
        }//end if... else if ... else
    }//end for

    //INIFile로 부터 읽어오기


    CIniReader iniReader( m_strINIFile.c_str() );
    
    pChar = iniReader.ReadString( "FTPINFO","FTPAddress", "" );
    m_elFTPElement.SetFTPAddress( pChar );
    delete[] pChar;

    pChar = iniReader.ReadString( "FTPINFO","FTPPort", "" );
    m_elFTPElement.SetFTPPort( pChar );
    delete[] pChar;

    pChar = iniReader.ReadString( "FTPINFO","UserID", "" );
    m_elFTPElement.SetUserID( pChar );
    delete[] pChar;

    pChar = iniReader.ReadString( "FTPINFO","UserPW", "" );
    m_elFTPElement.SetUserPW( pChar );
    delete[] pChar;


    nMailCount = iniReader.ReadInteger( "MAILINFO", "num", 0 );
    for( i = 0 ; i < nMailCount ; ++i )
    {        
        CElementMailInfo* pMailElement = (CElementMailInfo*)ElementFactory::GetElement( IElement::ELEMENTTYPE::ELEMENTTYPE_MAILINFO );
        strTmp.Format( "MailAddress%d",i );
        pChar = iniReader.ReadString( "MAILINFO",strTmp.c_str(), "" );
        pMailElement->SetMailAddress( pChar );
        delete[] pChar;


        strTmp.Format( "STMPAddress%d",i );
        pChar = iniReader.ReadString( "MAILINFO",strTmp.c_str(), "" );
        pMailElement->SetSMTPAddress( pChar );
        delete[] pChar;

        m_vecMailElement.push_back( pMailElement );

    }//end for

    return true;
}

bool CAutoManager::_ValidChk()
{
    /*VECELEMENT          m_vecFileElement;
    VECELEMENT          m_vecMailElement;

    CElementFTPInfo     m_elFTPElement;
    CElementCrashInfo   m_elServerName; */

    ITVECELEMENT it;
    ITVECELEMENT itEnd;
    bool bRet = false;

//파일은 하나라도 건지자.
    it = m_vecFileElement.begin();
    itEnd = m_vecFileElement.end();
    while( it != itEnd )
    {
        (*it)->ValidChk();
        if( (*it)->IsValid() )
        {
            bRet = true;
        }//end if
        ++it;
    }//end while

    if( !bRet )
    {
        //err 
        //ExitCode
    }//end if

//FTP중요하다.
    m_elFTPElement.ValidChk();
    if( !m_elFTPElement.IsValid() )
    {
        bRet = false;
        //err
        //exitCode
    }//end if

//메일은 중요하지 않다.
    it = m_vecMailElement.begin();
    itEnd = m_vecMailElement.end();
    while( it != itEnd )
    {
        (*it)->ValidChk();
        ++it;
    }//end while

    m_elServerName.ValidChk();

    return bRet;
}

bool CAutoManager::_WaitFileCompete( int nTimeOut )
{
    nTimeOut = max( 0, nTimeOut );
    nTimeOut = min( 59, nTimeOut );

    CTime destTime = CTime::GetCurrentTime(); 
    destTime += CTimeSpan( 0, 0, 0, nTimeOut ); // 목표 시간
    CTime currentTime = CTime::GetCurrentTime(); // 현재 시간


    ITVECELEMENT it;
    ITVECELEMENT itEnd;
    CElementFileInfo* pFileInfo = NULL; 
    bool bDone = true;
    while( destTime > currentTime )
    {
        it = m_vecFileElement.begin();
        itEnd = m_vecFileElement.end();

        while( it != itEnd )
        {
            bDone = true; // 모든 놈들이 다 완료냐?
            pFileInfo = (CElementFileInfo*)(*it);
            if( !pFileInfo->GetUserble() )
            {
                if( !pFileInfo->ChkUserble() )
                    bDone = false;

            }
            ++it;
        }//end while
        if( bDone )
            return true;

        currentTime = CTime::GetCurrentTime();
    }//end while

    return false;
}


bool CAutoManager::_Zipping()
{
    CZipper zipper;
    CTime destTime = CTime::GetCurrentTime();
    
    CStringEx strTime = destTime.Format("%y%m%d_%H%M%S");
    CStringEx strFileName;
    strFileName = m_elServerName.GetCrashInfo();
    strFileName += strTime;
    strFileName +=".zip";
    TRACE( "%s\n",strFileName.c_str() );

    if(g_Logger)
    {
        CStringEx strLog;
        strLog.Format( "압축파일명 : %s\n",strFileName.c_str());
        g_Logger->AddLog( strLog );
    }

    if( !zipper.CreateZipFile( strFileName ) )
    {
        return false;
    }
    ITVECELEMENT it = m_vecFileElement.begin();
    ITVECELEMENT itEnd = m_vecFileElement.end();
    CElementFileInfo* pFileInfo = NULL;
    while( it != itEnd )
    {
        pFileInfo = (CElementFileInfo*)(*it);
        if( pFileInfo->GetUserble() )
        {
            if( !zipper.AddFile( pFileInfo ) )
            {
                
                if( g_Logger )
                {
                    CStringEx strLog;
                    strLog.Format( "압축 실패  : &s\n",pFileInfo->GetFileName() );
                    g_Logger->AddLog( strLog.c_str());
                }
            }//end if
        }//end if
        ++it;
    }//end while

    zipper.Close();
    m_strZipedFileName = zipper.GetZipFileName();
    return true;
}

bool CAutoManager::_UploadToFTP()
{
    CMinFTP minFTP;
    if( !minFTP.SetFTPInfo( &m_elFTPElement ) )
    {
        if( g_Logger )
        {
            CStringEx strLog;
            strLog.Format( "FTP INFO ERR\nAddr : %s \nID : %s\nPW : %s\n",
                           m_elFTPElement.GetFTPAddress(),
                           m_elFTPElement.GetUserID(),
                           m_elFTPElement.GetUserPW() );
            g_Logger->AddLog( strLog.c_str() );
        }//end if
        return false;
    }//end if

    if(!minFTP.FTPConnection())
    {
        if( g_Logger )
        {
            g_Logger->AddLog( "Connection ERR \n" );
        }//end if
        return false;
    }//end if
    

    if( !minFTP.UpLoadFile( m_strZipedFileName, m_elServerName.GetCrashInfo() ) )
    {
        if( g_Logger )
        {
            CStringEx strLog;
            strLog.Format( "Zip File : %s \n CrashServerName : %s",m_strZipedFileName.c_str(),
                            m_elServerName.GetCrashInfo() );
            g_Logger->AddLog( strLog.c_str() );
        }//end if
    }//end if

    if( g_Logger )
    {
        g_Logger->AddLog( "업로드 완료\n" );
    }//end if
    return true;
}


void CAutoManager::_SendMail()
{
    CMailSender mailSender;

    ITVECELEMENT it = m_vecMailElement.begin();
    ITVECELEMENT itEnd = m_vecMailElement.end();
    while( it != itEnd )
    {
        CElementMailInfo* pEl = (CElementMailInfo*)(*it);
        mailSender.SendMail( pEl ,m_elServerName.GetCrashInfo() );
        ++it;
    }//end while
}

void CAutoManager::_CleanUp()
{
    _SendMail();
    if( !m_strZipedFileName.IsEmpty() )
    {
        ::DeleteFileA( m_strZipedFileName.c_str() );
    }//end if;
}