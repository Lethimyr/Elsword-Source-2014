#include ".\reglogger.h"
#include <boost/bind.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>

#define CLASS_TYPE KRegLogger

static KRegLogger* g_kRegLogger = NULL;

KRegLogger::KRegLogger(void) : m_dwCategoryCount( 0 ), m_bIsRegisterAction( true )
{
    std::ostringstream stm;

    stm << "시스템 이벤트로그에 로그를 기록할 실행파일을 레지스트리에 등록 / 해제 합니다." << std::endl 
        << std::endl
        << "\t Ex) " << m_strAppname << " /name GameServer /cc 5    -- 등록" << std::endl
        << "\t     " << m_strAppname << " /unreg /name GameServer   -- 등록 해제" << std::endl;

    m_strComment = stm.str();

    char szbuff[MAX_PATH] = {0};
    ::GetCurrentDirectory( MAX_PATH, szbuff );

    m_strPath = szbuff;

    g_kRegLogger = this;

    stm.str("");

    stm << "등록한다. (기본 모드이므로 생략 가능)";
    ADD_CMD( reg, false );

    stm << "등록 해제한다.";
    ADD_CMD( unreg, false );

    stm << "파일 이름을 입력한다.";
    ADD_CMD( name, true );

    stm << "Category Count를 입력한다.";
    ADD_CMD( cc, true );

    stm << "등록할 실행파일의 전체경로를 입력받는다. (지원예정)";
    ADD_CMD( path, true );

}

KRegLogger::~KRegLogger(void)
{
}

IMPL_ON_FUNC( reg )
{
    g_kRegLogger->m_bIsRegisterAction = true;

    std::cout << szParam_ << " - 새로운 로그 생성자를 등록합니다." << std::endl;
    return true;
}

IMPL_ON_FUNC( unreg )
{
    g_kRegLogger->m_bIsRegisterAction = false;

    std::cout << szParam_ << " - 등록된 로그 생성자를 삭제합니다." << std::endl;
    return true;
}

IMPL_ON_FUNC( name )
{
    g_kRegLogger->m_strName = szParam_;
    return true;
}

IMPL_ON_FUNC( cc )
{
    g_kRegLogger->m_dwCategoryCount = ::atoi( szParam_ );
    return true;
}

IMPL_ON_FUNC( path )
{
    std::cout << szParam_ << " - 현재 지원하지 않는 옵션이므로 무시됩니다." << std::endl;
    return true;
}

#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

bool KRegLogger::Init()
{
    if( m_strName.empty() )
    {
        std::cout << "오류. 올바른 이름을 입력해 주세요." << std::endl;
        return false;
    }

    m_strPath += "\\";
    m_strPath += m_strName;
    m_strPath += ".exe";

    std::cout << std::endl 
        << std::setiosflags( std::ios::left ) << std::setw( 15 ) << "대상 파일"
        << " : " << m_strPath << std::endl;

    CFindFile cFile;
    if( ! cFile.FindFile( m_strPath.c_str() ) )
    {
        std::cout << "대상 파일이 존재하지 않습니다. path : " << m_strPath << std::endl;
        return false;
    }

    return true;
}

bool KRegLogger::Run()
{
    if( m_bIsRegisterAction ) 
        AddEventSource();
    else
        RemoveEventSource();

    return true;
}

#include <tchar.h>

void KRegLogger::AddEventSource()
{
    HKEY	hRegKey = NULL; 
    DWORD	dwError = 0;
    TCHAR	szPath[ MAX_PATH ];

    _stprintf( szPath, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"), m_strName.c_str() );

    // Create the event source registry key
    dwError = RegCreateKey( HKEY_LOCAL_MACHINE, szPath, &hRegKey );

    // Register EventMessageFile
    dwError = RegSetValueEx( hRegKey, _T("EventMessageFile"), 0, REG_EXPAND_SZ, 
        (PBYTE) m_strPath.c_str(), ((DWORD)m_strPath.size() + 1) * sizeof TCHAR ); 

    // Register supported event types
    DWORD dwTypes = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE; 
    dwError = RegSetValueEx( hRegKey, _T("TypesSupported"),	0, REG_DWORD, 
        (LPBYTE) &dwTypes, sizeof dwTypes );

    // If we want to support event categories, we have also to register	the CategoryMessageFile.
    // and set CategoryCount. Note that categories need to have the message ids 1 to CategoryCount!

    if( m_dwCategoryCount > 0 ) {

        dwError = RegSetValueEx( hRegKey, _T("CategoryMessageFile"), 0, REG_EXPAND_SZ,
            (PBYTE) m_strPath.c_str(), ((DWORD)m_strPath.size() + 1) * sizeof TCHAR ); 

        dwError = RegSetValueEx( hRegKey, _T("CategoryCount"), 0, REG_DWORD, 
            (PBYTE) &m_dwCategoryCount, sizeof m_dwCategoryCount );
    }
    RegCloseKey( hRegKey );

    char WSAMsg[256] = {0};
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        WSAMsg, MAX_PATH, NULL );

    std::cout << std::setiosflags( std::ios::left ) << std::setw( 15 ) << "생성 레지스트리"
              << " : " << szPath << std::endl
              << std::setiosflags( std::ios::left ) << std::setw( 15 ) << "Result"
              << " : " << WSAMsg;

}

void KRegLogger::RemoveEventSource()
{
    DWORD dwError = 0;
    TCHAR szPath[ MAX_PATH ];

    _stprintf( szPath, _T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s"), m_strName.c_str() );
    dwError = RegDeleteKey( HKEY_LOCAL_MACHINE, szPath );

    char WSAMsg[256] = {0};
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        WSAMsg, MAX_PATH, NULL );

    std::cout << std::setiosflags( std::ios::left ) << std::setw( 15 ) << "제거 대상 키"
              << " : " << szPath << std::endl
              << std::setiosflags( std::ios::left ) << std::setw( 15 ) << "Result"
              << " : " << WSAMsg;
}