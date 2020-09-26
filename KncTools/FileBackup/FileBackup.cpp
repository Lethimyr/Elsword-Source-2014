#include ".\filebackup.h"
#include <atltime.h>    // CTime
#include <direct.h>     // _mkdir

#include <windows.h>    // GetModuleFileName, GetCurrentProcess


KFileBackup::KFileBackup(void)
{
    // default skip file
    // 기본적으로 아래의 파일을 skip한다. 이후 호출되는 ParseCmdParam 에서 변경될 수 있다.
    // 기본 skip 되는 파일을 Init에서 설정하면 안된다. 
    // calling order : constructor -> ParseCmdParam -> Init.
    SetSkipExt( "dll" );
    SetSkipExt( "lnk" );
    //SetSkipExt( "log" );

}

KFileBackup::~KFileBackup(void)
{
}

bool KFileBackup::Init()
{
    // get file name (itself)
    char pathbuff[MAX_PATH]     = {0};
    char namebuff[_MAX_FNAME]   = {0};
    char extbuff[_MAX_EXT]      = {0};
    ::GetModuleFileName( NULL, pathbuff, MAX_PATH );            // get file name (full path)
    ::_splitpath( pathbuff, NULL, NULL, namebuff, extbuff );    // split full path

    m_strMyName = namebuff;
    m_strMyName += extbuff;

    KLOG(( 2, "my file name : %s", m_strMyName.c_str() ));

    // get source dirctory
    ::GetCurrentDirectory( MAX_PATH, pathbuff );
    m_strSourceDir = pathbuff;

    KLOG(( 2, "source directory : %s", m_strSourceDir.c_str() ));

    // make destination direcotry
    ::strcat( pathbuff, "\\Backup" );
    MakeFolder( pathbuff );

    // 시간을 이용해 백업 폴더 이름을 정한다.
    CTime cTime = CTime::GetCurrentTime();
    m_strDestDir = pathbuff;
    m_strDestDir += cTime.Format("\\%Y.%m.%d_%H.%M.%S");

    MakeFolder( m_strDestDir.c_str() );

    KLOG(( 2, "destination directory : %s", m_strDestDir.c_str() ));

    // set log file path
    sprintf( pathbuff, "%s\\%s", 
        m_strDestDir.c_str(), 
        static_cast<LPCTSTR>( cTime.Format("_log_%Y.%m.%d_%H.%M.%S.log") ) );
    pSKLogManager->BeginFileOutput( pathbuff );

    return true;
}

bool KFileBackup::SetSkipExt( IN const char* szExt )
{
    CString cstrTemp( szExt );
    cstrTemp.MakeLower();               // 대문자를 모두 소문자로 변환
    cstrTemp.TrimLeft( '.' );           // 왼쪽의 '.' character를 모두 없앰.
    std::string strExt( cstrTemp );

    if( m_setSkipExt.find( strExt ) != m_setSkipExt.end() )
    {
        KLOGL(( 0, "%s(), 이미 추가된 확장자 : %s", __FUNCTION__, strExt.c_str() ));
        return false;
    }

    m_setSkipExt.insert( strExt );
    return true;
}

bool KFileBackup::ParseCmdParam( const int argc, const char** argv )
{
    if( argc > 2 )
    {
        KLOG(( 2, "too many command parameters." ));
        PrintHelp();
        return false;
    }

    CString cstrParam( argv[1] );
    cstrParam.TrimLeft( '/' );
    cstrParam.MakeLower();

    if( cstrParam == "?" )  // print help
    {
        PrintHelp();
        return false;
    }

    if( cstrParam == "noskip" ) // copy all extension files
    {
        ClearSkipExt();
        KLOG(( 2, "No Skip - copy all extension files." ));
        return true;
    }

    if( cstrParam[0] == '.' ) // custom extension filter
    {
        KLOG(( 2, "Custom file extension filter setting." ));
        cstrParam.TrimLeft( '.' ); // 제일 처음 있는 . 을 제거한다.
        char* torken = ::strtok( cstrParam.GetBuffer(), "." );
        while( torken != NULL )
        {
            SetSkipExt( torken );
            KLOG(( 2, "custom filtering extension : %s ", torken ));
            torken = ::strtok( NULL, "." );
        }
    }
    return true;
}

bool KFileBackup::Backup()
{
    // copy all files
    WIN32_FIND_DATA     wfd;
    HANDLE              hSrch;
    BOOL                bResult = TRUE;
    std::string         strWildCard( m_strSourceDir );
    int                 nCount = 0;

    strWildCard += "\\*.*";

    hSrch = ::FindFirstFile( strWildCard.c_str(), &wfd );
    if( hSrch == INVALID_HANDLE_VALUE ) // 찾고자 하는 확장자의 파일이 하나도 없는 경우
    {
        KLOGL(( 0, "%s(), no files in directory (%s)", __FUNCTION__, m_strSourceDir.c_str() ));
        return false;
    }

    TCHAR szSrcFile[MAX_PATH] = {0};
    TCHAR szDestFile[MAX_PATH] = {0};
    while( bResult )
    {
        if( !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )    // 디렉토리가 아닌 파일에 대해서
        {
            if( ShouldSkip( m_strSourceDir.c_str(), wfd.cFileName ) )
            {
                KLOG(( 2, "Copy %s ... Skipped", wfd.cFileName ));
                bResult = ::FindNextFile( hSrch, &wfd );
                continue;
            }

            _tcscpy( szSrcFile, m_strSourceDir.c_str() );
            _tcscat( szSrcFile, "\\" );
            _tcscat( szSrcFile, wfd.cFileName );
            _tcscpy( szDestFile, m_strDestDir.c_str() );
            _tcscat( szDestFile, "\\" );
            _tcscat( szDestFile, wfd.cFileName );

            if( 0 == ::CopyFile( szSrcFile, szDestFile, FALSE ) )
            {
                KLOGL(( 0, "BackupFiles(), 파일 복사 실패( %s -> %s )", szSrcFile, szDestFile ));
                return false;
            }

            KLOG(( 2, "Copy %s ... OK", wfd.cFileName ));
            nCount++;
        } // if
        else
        {
            KLOG(( 2, "Copy %s ... Skipped (Directory)", wfd.cFileName ));
        }

        bResult = ::FindNextFile( hSrch, &wfd );
    } // while

    KLOG(( 2, "\n * Total %d files copied. \n source : %s \n destination : %s",
                nCount, 
                m_strSourceDir.c_str(),
                m_strDestDir.c_str() ));

    return true;

}

bool KFileBackup::MakeFolder( const char* szPath )
{
    if( 0 == ::_mkdir( szPath ) )
    {
        KLOG(( 2, "directory (%s) is created.", szPath ));
        return true;
    }

    switch( errno )
    {
    case EEXIST:
        KLOG(( 2, "directory (%s) already exist.", szPath ));
        return true;
        break;
    default:
        KLOGL(( 0, "unknown error" ));
        break;
    }

    return false;
}

bool KFileBackup::ShouldSkip( const char* szPath, const char* szFileName )
{
    char szFullPath[MAX_PATH]   = {0};
    char szExt[_MAX_EXT]        = {0};

    if( m_strMyName.compare( szFileName ) == 0 )
        return true; // itself

    // make full file path
    sprintf( szFullPath, "%s\\%s", szPath, szFileName );

    // split again to get file extension
    _splitpath( szFullPath, NULL, NULL, NULL, szExt );

    // all string compared as lower case.
    CString cstrExt( szExt );
    cstrExt.MakeLower();
    cstrExt.TrimLeft( '.' );

    if( m_setSkipExt.find( std::string( cstrExt ) ) != m_setSkipExt.end() )
        return true; // skip

    return false;
}

#include <iostream>

void KFileBackup::PrintHelp()
{
    std::cout << std::endl 
              << "해당 작업 폴더에 있는 파일을 백업합니다." << std::endl << std::endl
              << "옵션 : /?, /noskip, /.ext0.ext1.ext2...." << std::endl
              << "  /?                     도움말 출력" << std::endl
              << "  /noskip                모든 확장자의 파일을 백업" << std::endl
              << "  /.ext0.ext1.ext2....   skip할 확장자를 직접 설정" << std::endl
              << "                              예) /.bat.jpg.mp3 " << std::endl << std::endl;

    std::cout << "기본적으로 skip하는 파일 확장자는 다음과 같습니다." << std::endl;

    std::set<std::string>::iterator sit;
    for(sit = m_setSkipExt.begin(); sit != m_setSkipExt.end(); sit++ )
    {
        std::cout << "   " << *sit;
    }
    std::cout << " (총 " << m_setSkipExt.size() << " 개 확장자)" << std::endl << std::endl;

    std::cout << "백업되는 파일은 \"원본폴더\\Backup\\작업시간\\\" 의 경로에 복사됩니다." << std::endl
              << "해당 폴더 내에는 백업 과정을 기록한 로그파일이 생성됩니다." << std::endl;
}