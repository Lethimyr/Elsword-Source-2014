#include <WinSock2.h>
#include "BaseServer.h"
#include "dbg/dbg.hpp"
#include "NetError.h"
#include "ThreadManager.h"
#include "socket/NetCommon.h"
#include "NetLayer.h"
#include "SimLayer.h"
#include "DBLayer.h"
#include "KncSend.h"
#include "ActorManager.h"
//{{ 2010. 02. 12  최육사	서버 로그 매니저
#ifdef SERV_LOG_MANAGER
	#include "ServerLogManager.h"
#endif SERV_LOG_MANAGER
//}}

//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

#include <ImportKncLua.h>
#include <ImportKncSerializer.h>
#include <ImportKncSecurity.h>
#include <ImportKncDebug.h>

#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "odbc32" )

#ifdef NDEBUG
#   include "CrashRpt/KncCrashRpt.h"
#   pragma comment(lib, "../Common/Crashrpt/CrashRpt.lib")

// 크래쉬 리포트 처리 함수
void CrashReportProcess( LPVOID lpvState )
{
	std::wostringstream stm;

	stm << L"Local IP : " << NetCommon::GetLocalIPW() << std::endl
		<< L"GetTickCount : " << ::GetTickCount() << std::endl;

	AppendToMailContent( lpvState, stm.str().c_str() );

	// get post-crash execute file's full path.
	wchar_t szBuff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, szBuff );
#ifdef _CONVERT_VS_2010
	wcscat_s( szBuff, MAX_PATH, L"\\DumpUploader.exe" );
#else
	::wcscat( szBuff, L"\\DumpUploader.exe" );
#endif _CONVERT_VS_2010

	stm.str( L"" );  // 초기화 - 버퍼의 내용을 인자로 받은 문자열로 대치.

	std::wstring wstrLogFileName = KncUtil::toWideString( dbg::logfile::GetLogFileName() );

	// build command line parameter
	stm << L"/s " << NetCommon::GetLocalIPW()			// 서버 이름 (아이피)
		<< L" /f " << wstrLogFileName					// 로그파일 이름
		<< L" /f " << GetFullDumpFileName( lpvState );	// 덤프파일 이름

	ShellExecuteW( NULL, L"open", szBuff, stm.str().c_str(), NULL, SW_SHOWNORMAL );
}

unsigned int CALLBACK CrashCallback_StackOverFlow( LPVOID lpvState )
{
	// stack over flow인 경우 별도의 스레드에서 크래쉬 리포트 처리한다!
    CrashReportProcess( lpvState );
	return 0;
}

BOOL CALLBACK CrashCallback( LPVOID lpvState )
{
	//{{ 2010. 04. 13  최육사	크래쉬 리포트 개선
	if( GetExcetionType( lpvState ) != EXCEPTION_STACK_OVERFLOW )
	{
		// 스택 오버플로우가 아니라면 기존대로 처리한다!
		CrashReportProcess( lpvState );
	}
	else
	{
		// stack over flow인 경우 별도의 스레드 생성하여 처리!
		HANDLE hThread = (HANDLE)_beginthreadex( 0, (1024 * 1024 * 2), CrashCallback_StackOverFlow, lpvState, 0, NULL );

		// 생성 쓰레드가 덤프 끝날 때까지 기다린다
		WaitForSingleObject( hThread, INFINITE );

		CloseHandle( hThread );
	}
	//}}

    return true;
}
#endif


NiImplementRTTI( KBaseServer, KPerformer );
ImplInstanceKeeping( KBaseServer );

lua_State* g_pLua = NULL;

#define SERVER_INFO_UPDATE_GAP 60 * 1000
DWORD KBaseServer::ms_dwServerInfoUpdateGap( SERVER_INFO_UPDATE_GAP );

KBaseServer::KBaseServer(void) 
:
    m_iServerGroupID( 0 ),
    m_strTagVersion( L"Tag version undefined." ),
    m_dwShutDownWait( 1000 * 60 * 2 ),
    m_hWnd( HWND_DESKTOP ),
	//{{ 2012. 12. 24	최육사	게임 서버 버전 세분화
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	m_eUseVersion( VE_INTERNAL )
#else
	m_bUseSecondVer( false )
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
{
    srand( (unsigned) time( NULL ) );
    m_dwServerInfoUpdateTick = ::GetTickCount();
    m_spEventAck.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
	m_bServerReady = false;
#ifdef SERV_MORNITORING
	m_bConnMornitoringEnable = false;
#endif SERV_MORNITORING
	//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
	m_bScriptParsingErrReport = false;
#endif SERV_SCRIPT_PARSING_ERR_REPORT
	//}}
	//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
	m_cServerRollType = KServerInfo::SRT_NONE;
#endif SERV_SERVER_ROLL_TYPE
	//}}
}

KBaseServer::~KBaseServer(void)
{
	//{{ 2010. 8. 30	최육사	서버 정상 종료 처리	
	if( g_pLua != NULL )
	{
		lua_close( g_pLua );
		g_pLua = NULL;
	}
	//}}

    //dbg::profiler::kill_profiler( "profile_info.htm" );
}

ImplToStringW( KBaseServer )
{
    size_t uiServerList = 0;
    {
        KLocker lock( m_csServerList );
        std::map< int, std::vector< KServerInfo > >::const_iterator mit;
        for( mit = m_mapServerList.begin(); mit != m_mapServerList.end(); ++mit )
        {
            uiServerList += mit->second.size();
        }
    }
    
    START_TOSTRING_PARENTW( KPerformer )
        << TOSTRINGW( m_strMailingList )
        << TOSTRINGW( m_strTagVersion )
        << TOSTRINGW( uiServerList )        
		//{{ 2012. 12. 24	최육사	게임 서버 버전 세분화
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
		<< TOSTRINGW( m_eUseVersion )
		<< TOSTRINGW( m_strMainVer[VE_INTERNAL] )
		<< TOSTRINGW( m_strMainVer[VE_OPEN_TEST] )
		<< TOSTRINGW( m_strMainVer[VE_SERVICE] )
#ifdef SERV_KR_OPEN_TEST2_VERSION// 작업날짜: 2013-05-24	// 박세훈
		<< TOSTRINGW( m_strMainVer[VE_OPEN_TEST2] )
#endif // SERV_KR_OPEN_TEST2_VERSION
#else
		<< TOSTRINGWb( m_bUseSecondVer )
		<< TOSTRINGW( m_strMainVer[0] )
		<< TOSTRINGW( m_strMainVer[1] )
#endif SERV_GAME_SERVER_VERSION_DETAIL
		//}}        
        << TOSTRINGW( m_dwShutDownWait );
 
//     m_stringTable.Dump( stm_ );

     return stm_;
}

#include <lua_tinker.h>

void KBaseServer::RegToLua()
{
	// global function set
	lua_tinker::def( g_pLua, "log",			&dbg::logfile::SetLogLevel );
	lua_tinker::def( g_pLua, "changelog",		&dbg::logfile::CloseFile );
	lua_tinker::def( g_pLua, "GetLocalIP",	&NetCommon::GetLocalIP );

 	lua_tinker::class_add<KThreadManager>( g_pLua, "KThreadManager" );
	lua_tinker::class_def<KThreadManager>( g_pLua, "Init",						&KThreadManager::Init );
	lua_tinker::class_def<KThreadManager>( g_pLua, "ResetMaxProcessingInfo",	&KThreadManager::ResetMaxProcessingInfo );
	lua_tinker::class_def<KThreadManager>( g_pLua, "SetThreadNum",				&KThreadManager::SetThreadNum );

 	lua_tinker::class_add<KBaseServer>( g_pLua, "KBaseServer" );
	//{{ 2012. 12. 24	최육사	게임 서버 버전 세분화
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetUseVersion",					&KBaseServer::SetUseVersion );
#else
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetMainVersion",					&KBaseServer::SetMainVersion );
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetSecondVersion",					&KBaseServer::SetSecondVersion );
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetShutDownWait",					&KBaseServer::SetShutDownWait );
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetMailingList",					&KBaseServer::SetMailingList );
	//{{ 2009. 12. 17  최육사	서버군확장
	lua_tinker::class_def<KBaseServer>( g_pLua, "GetServerGroupID",					&KBaseServer::GetServerGroupID );
	//}}
	//{{ 2010. 11. 12	최육사	서버간 접속 모니터링
#ifdef SERV_MORNITORING
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetConnectionMornitoringEnable",	&KBaseServer::SetConnectionMornitoringEnable_LUA );
#endif SERV_MORNITORING
	//}}
	//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetScriptParsingErrReport",		&KBaseServer::SetScriptParsingErrReport_LUA );
	lua_tinker::class_def<KBaseServer>( g_pLua, "AddScriptParsingErrReportMail",	&KBaseServer::AddScriptParsingErrReportMail_LUA );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
	//}}

	//{{ 2011.2.11  조효진  모니터링툴 서버 포트 체크 시 에러로그 남는거 수정
#ifdef SERV_PERMIT_PORT_CHECK
	lua_tinker::class_def<KBaseServer>( g_pLua, "AddPortCheckMoritoringServer", &KBaseServer::AddPortCheckMoritoringServer_LUA );
#endif SERV_PERMIT_PORT_CHECK
	//}}
	//{{ 2011. 04. 13  우편 및 거래 감시 실시간 SMS 전송
#ifdef SERV_MONITORING_LETTER_AND_TRADE_SMS
	lua_tinker::class_def<KBaseServer>( g_pLua, "SetMonitoringLetterAndTradeSMS",		&KBaseServer::SetMonitoringLetterAndTradeSMS_LUA );
#endif SERV_MONITORING_LETTER_AND_TRADE_SMS
	//}} 2011. 04. 13 우편 및 거래 감시 실시간 SMS 전송

	//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
	lua_tinker::decl( g_pLua, "ESC_CENTER",			KBaseServer::ESC_CENTER );
	lua_tinker::decl( g_pLua, "ESC_LOGIN",			KBaseServer::ESC_LOGIN );
	lua_tinker::decl( g_pLua, "ESC_GAME",			KBaseServer::ESC_GAME );
	lua_tinker::decl( g_pLua, "ESC_CHANNEL",		KBaseServer::ESC_CHANNEL );
	lua_tinker::decl( g_pLua, "ESC_SIMULATION",		KBaseServer::ESC_SIMULATION );
	lua_tinker::decl( g_pLua, "ESC_GLOBAL",			KBaseServer::ESC_GLOBAL );
#endif SERV_DB_CONNECTION_SECURITY
	//}}

	//{{ 2012. 12. 24	최육사	게임 서버 버전 세분화
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
	lua_tinker::decl( g_pLua, "VE_INTERNAL",		KBaseServer::VE_INTERNAL );
	lua_tinker::decl( g_pLua, "VE_OPEN_TEST",		KBaseServer::VE_OPEN_TEST );
	lua_tinker::decl( g_pLua, "VE_SERVICE",			KBaseServer::VE_SERVICE );
#ifdef SERV_KR_OPEN_TEST2_VERSION// 작업날짜: 2013-05-24	// 박세훈
	lua_tinker::decl( g_pLua, "VE_OPEN_TEST2",		KBaseServer::VE_OPEN_TEST2 );
#endif // SERV_KR_OPEN_TEST2_VERSION
#endif SERV_GAME_SERVER_VERSION_DETAIL
	//}}
}

extern const wchar_t* g_szConfigFile;
#if defined( SERV_CHANGE_SERVER_CONFIG_FORDER ) && !defined( SERV_INT_INTERNAL )
	#if defined( SERV_COUNTRY_TWHK )
		const wchar_t* g_szConfigForderName = L"Config\\TW\\";
	#elif defined( SERV_COUNTRY_JP )
		const wchar_t* g_szConfigForderName = L"Config\\JP\\";
	#elif defined( SERV_COUNTRY_EU )
		const wchar_t* g_szConfigForderName = L"Config\\EU\\";
	#elif defined( SERV_COUNTRY_US )
		const wchar_t* g_szConfigForderName = L"Config\\US\\";
	#elif defined( SERV_COUNTRY_CN )
		const wchar_t* g_szConfigForderName = L"Config\\CN\\";
	#elif defined( SERV_COUNTRY_TH )
		const wchar_t* g_szConfigForderName = L"Config\\TH\\";
	#elif defined( SERV_COUNTRY_ID )
		const wchar_t* g_szConfigForderName = L"Config\\ID\\";
	#elif defined( SERV_COUNTRY_BR )
		const wchar_t* g_szConfigForderName = L"Config\\BR\\";
	#elif defined( SERV_COUNTRY_PH )
		const wchar_t* g_szConfigForderName = L"Config\\PH\\";
	#elif defined( SERV_COUNTRY_IN )
		const wchar_t* g_szConfigForderName = L"Config\\IN\\";
	#else
		const wchar_t* g_szConfigForderName = L"Config\\INT\\";
	#endif
#else	
		const wchar_t* g_szConfigForderName = L"Config\\INT\\";
#endif //SERV_CHANGE_SERVER_CONFIG_FORDER

#ifndef _DEBUG	// 디버그 안되는 오류 해외팀 수정
#include "Lottery.h"
#endif // _DEBUG

bool KBaseServer::Init()
{
#ifndef _DEBUG	// 디버그 안되는 오류 해외팀 수정
	KLottery::Seed( static_cast<int>(std::time(0)) );
	KLottery kLotTest;
	kLotTest.Decision();
#endif // _DEBUG

    dbg::logfile::SetFileNameAtTime( true );            // init log file
    
    _JIF( NetCommon::InitWinSock(), return false );     // socket init

    g_pLua = lua_open();
        
    _JIF( g_pLua != NULL, return false );
    
    //luaopen_base( g_pLua );  // 루아함수 tonumber를 사용해야 한다.
    //luaopen_table( g_pLua );
    //luaopen_io( g_pLua );
    //luaopen_string( g_pLua );
    //luaopen_math( g_pLua );

    luaL_openlibs( g_pLua );

    lua_settop( g_pLua, 0 );

    RegToLua();
	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
	m_kTStatisticsManager.RegToLua();
#else
	m_kStatisticsManager.RegToLua();
#endif SERV_STATISTICS_THREAD
	//}}

    SiKDBLayer()->RegToLua();
    KSimLayer::GetKObj()->RegToLua();
    KNetLayer::GetKObj()->RegToLua();

    LoadINIFile( g_szConfigFile );

    // 050317. config 파일이 lua, ini 두가지로 늘어남. 확장자는 별도로 붙인다.
	std::string strLuaFileName = KncUtil::toNarrowString( g_szConfigFile );
		
	if( m_vecCmdArgs.size() >= 3 ) // 전달인자 갯수가 2이상이면 버전태그가 전달인자로 들어온건지 확인함!
	{
		std::string strVersionTag = KncUtil::toNarrowString( m_vecCmdArgs[1].GetString() );
		std::string strServerGroupID = KncUtil::toNarrowString( m_vecCmdArgs[2].GetString() );
		
		// 읽을 config파일 이름 설정
		strLuaFileName += "_";
		strLuaFileName += strVersionTag;

		// 서버군ID 저장
        SetServerGroupID( static_cast<int>( atoi( strServerGroupID.c_str() ) ) );

		// cmdArgs에서 DB Conn Str를 제외하고 삭제한다
		while( m_vecCmdArgs.size() > 2 )
		{
			m_vecCmdArgs.erase( m_vecCmdArgs.begin() );
		}
	}
	else
	{
		START_LOG( cerr, L"서버 켤때 필요한 전달인자가 부족합니다!" )
			<< BUILD_LOG( m_vecCmdArgs.size() )
			<< END_LOG;
        return false;
	}

	strLuaFileName += ".lua";
#ifdef SERV_CHANGE_SERVER_CONFIG_FORDER
	// 사내에서만 폴더를 나눈다
	if( (int)(strLuaFileName.find( "INTERNAL" )) != -1 ) // 찾았다
	{
		strLuaFileName = KncUtil::toNarrowString(g_szConfigForderName) + strLuaFileName;
	}
#endif //SERV_CHANGE_SERVER_CONFIG_FORDER

	START_LOG( cout, L"Config File Name : " << strLuaFileName );

    _JIF( 0 == LUA_DOFILE( g_pLua, strLuaFileName.c_str() ), return false );
    
    if( NetError::GetLastError() != NetError::NET_OK )
        return false;

	//{{ 2010. 02. 12  최육사	서버 로그 매니저
#ifdef SERV_LOG_MANAGER
	KServerLogManager::GetInstance().Begin();
#endif SERV_LOG_MANAGER
	//}}

    SiKDBLayer()->Init();
    KSimLayer::GetKObj()->Init();
	//{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	KSimLayer::GetKObj()->TickDumpReset();
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}
    KSimLayer::GetKObj()->Begin();

    _JIF( Authenticate( 10000 ), return false );

    // 서버 자체 인증을 거친 뒤에 Actor에서 접속을 받는다.
    KNetLayer::GetKObj()->Init();

    //{{ 050810. jseop. tag 버전 설정.
    SetTagVersion();    // 실행 파일 생성 시각 얻기.
    //}} 050810. jseop. tag 버전 설정.
	
	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
	m_kTStatisticsManager.Init();
#else
	m_kStatisticsManager.InitLocalLogKeys();
#endif SERV_STATISTICS_THREAD	
	//}}
	//{{ 2010. 12. 7	최육사	메일 전송 시스템
#ifdef SERV_MAIL_SYSTEM
	m_kMailManager.Init();
#endif SERV_MAIL_SYSTEM
	//}}
	//{{ 2007. 12. 24  최육사  사내서버라면 로그를 전송하지 않는다.
	//if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
	//	m_kStatisticsManager.SetSendToFTP( true );
	//}}

	// cmd args는 이제 필요없으므로 삭제
	m_vecCmdArgs.clear();

    return true;
}

void KBaseServer::Run( bool bRunInterpreter_ )
{
    if( bRunInterpreter_ )  lua_interpreter( g_pLua );
}

void KBaseServer::ShutDown()
{
	//{{ 2009. 12. 15  최육사	서버관리
	START_LOG( cout, L"---------------------------------------- 서버 종료 덤프 ----------------------------------------" );
	//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	DumpToLogFile();
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}
	KSimLayer::GetKObj()->DumpToLogFile();
	KNetLayer::GetKObj()->DumpToLogFile();
	SiKDBLayer()->DumpToLogFile();
	START_LOG( cout, L"---------------------------------------- 서버 종료 덤프 ----------------------------------------" );
	//}}

    KNetLayer::GetKObj()->CloseAccepter();  // 가장먼저 새로운 접속을 막는다.

    LIF( DestroyAllSession() );             // 접속중인 세션의 종료는 child에서 한다.
	
	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
	m_kTStatisticsManager.Shutdown();
#else
	m_kStatisticsManager.FlushData();
	m_kStatisticsManager.OnFlushLocalLog();
#endif SERV_STATISTICS_THREAD
	//}}
	//{{ 2010. 12. 7	최육사	메일 전송 시스템
#ifdef SERV_MAIL_SYSTEM
	m_kMailManager.Shutdown();
#endif SERV_MAIL_SYSTEM
	//}}

	SendToLogDB( DBE_SERVER_OFF_NOT );

    SiKDBLayer()->ShutDown( 60000 );        // 60초 동안 DB 이벤트 처리를 기다린다.
    KSimLayer::GetKObj()->ShutDown();
    KNetLayer::GetKObj()->ShutDown();

	::WSACleanup();

    SiKDBLayer()->ReleaseInstance();
    KSimLayer::ReleaseKObj();
    KNetLayer::ReleaseKObj();
}

//void KBaseServer::WriteServerInfoToDB()
//{
//    KServerInfo kPacket;
//    GetServerInfo( kPacket );
//    SendToLogDB( DBE_UPDATE_SERVER_INFO_REQ, kPacket );
//
//    m_dwServerInfoUpdateTick = ::GetTickCount();
//}

void KBaseServer::Tick()
{
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

    KPerformer::Tick();

	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
#else
	m_kStatisticsManager.Tick();
#endif SERV_STATISTICS_THREAD
	//}}

	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	SiKLogManager()->Tick();
#endif SERV_LOG_SYSTEM_NEW
	//}}

	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}

void KBaseServer::OnServerReadyComplete()
{
	// 스크립트 파싱 에러 리포트
	if( m_bScriptParsingErrReport == false )
		return;

    if( IsScriptParsingSuccess() == true )
		return;

	std::wstring wstrLogFileName = KncUtil::toWideString( dbg::logfile::GetLogFileName() );

	// 메일로 서버 로그를 전송하자!
	KEMAIL_SEND_EMAIL_NOT kPacket;
	kPacket.m_vecRecvMailAddrList = m_vecScriptParsingErrMailList;

	// 제목
	kPacket.m_wstrTitle = L"스크립트 파싱 에러 리포트! : ";
	kPacket.m_wstrTitle += GetName();

	// 본문
	kPacket.m_wstrDesc = GetName();
	kPacket.m_wstrDesc += L" 서버를 켜는 도중에 스크립트 파싱이 실패하였습니다! 서버로그 확인 바랍니다!\n\n";

	BOOST_TEST_FOREACH( const std::wstring&, wstrScriptFileName, m_vecFailScriptFileList )
	{
		kPacket.m_wstrDesc += wstrScriptFileName;
		kPacket.m_wstrDesc += L"\n";
	}

	// 서버 로그 파일 첨부
	kPacket.m_vecAttachFileFullPathList.push_back( wstrLogFileName );
	QueueingEventToMailManager( EMAIL_SEND_EMAIL_NOT, kPacket );

	START_LOG( cout, L"--- 스크립트 파싱 에러 리포트 ---" )
		<< BUILD_LOG( m_bScriptParsingErrReport )
		<< BUILD_LOG( IsScriptParsingSuccess() )
		<< BUILD_LOG( wstrLogFileName );
}

void KBaseServer::SetTagVersion()
{
    KncUtil::GetTimeStampByNameW( m_strTagVersion );

	//{{ 2010. 04. 13  최육사	크래쉬 리포트	
    // 050716. crash report 설정 부분을 main.cpp에서 simlayer로 옮김 - 태그 제목을 스크립트로 수정하기 위함.
#ifdef NDEBUG
	// 디버그 빌드가 아닐때만 install
    LPVOID lpvState = Install( CrashCallback, m_strMailingList.c_str(), m_strTagVersion.c_str() );
#endif
	//}}

    // 제목은 처음 초기화때 출력하므로, 아직 아무 로그도 출력하지 않은 상태여야 한다. 
    dbg::logfile::SetLogFileTitle( m_strTagVersion.c_str() ); 
}

//{{ 2012. 12. 24	최육사	게임 서버 버전 세분화
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GAME_SERVER_VERSION_DETAIL
//////////////////////////////////////////////////////////////////////////

namespace ConfigINI 
{
    /*//// ...in ini file... ////////////////////////////////////////////////////

    [Version]
    Protocol Version 1 = XXX
    Protocol Version 2 = YYY
    
    [String]
    aaa = AAA
    bbb = BBB
    ...

    /////////////////////////////////////////////////////////////////////////*/

    const wchar_t* szSectionVersion = L"Version";
    const wchar_t* szSectionString  = L"String";
	const wchar_t* szKeyVersion1    = L"Internal Version";
	const wchar_t* szKeyVersion2    = L"OpenTest Version";
	const wchar_t* szKeyVersion3    = L"Service Version";
#ifdef SERV_KR_OPEN_TEST2_VERSION// 작업날짜: 2013-05-24	// 박세훈
	const wchar_t* szKeyVersion4    = L"OpenTest Version2";
#endif // SERV_KR_OPEN_TEST2_VERSION
}

void KBaseServer::LoadINIFile( const wchar_t* szFileName_ )
{
	// 050318. florist. ini 파일은 전체 경로(혹은 상대경로)를 적어서 사용해야 한다. 
	// 그렇지 않으면 무조건 윈도우 폴더 안에서만 찾는다. 이왕 해야 할거면 확실하게 절대경로를 쓰자.
	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	m_strIniFilePath = buff;
	m_strIniFilePath += L"\\";
#ifdef SERV_CHANGE_SERVER_CONFIG_FORDER
	// 사내에서만 폴더 구분 한다
	if( m_vecCmdArgs.size() >= 3 )
	{
		std::string strVersionTag = KncUtil::toNarrowString( m_vecCmdArgs[1].GetString() );
		if( (int)(strVersionTag.find( "INTERNAL" )) != -1 ) // 찾았다
		{
			m_strIniFilePath += g_szConfigForderName;
		}
	}
#endif //SERV_CHANGE_SERVER_CONFIG_FORDER
	m_strIniFilePath += szFileName_;
	m_strIniFilePath += L".ini";
	
	// 050317. version 정보 얻어오기.
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_INTERNAL] = buff;
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion2, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_OPEN_TEST] = buff;
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion3, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_SERVICE] = buff;
#ifdef SERV_KR_OPEN_TEST2_VERSION// 작업날짜: 2013-05-24	// 박세훈
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion4, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[VE_OPEN_TEST2] = buff;
#endif // SERV_KR_OPEN_TEST2_VERSION

	LIF( !m_strMainVer[VE_INTERNAL].empty() );
	LIF( !m_strMainVer[VE_OPEN_TEST].empty() );
	LIF( !m_strMainVer[VE_SERVICE].empty() );
#ifdef SERV_KR_OPEN_TEST2_VERSION// 작업날짜: 2013-05-24	// 박세훈
	LIF( !m_strMainVer[VE_OPEN_TEST2].empty() );
#endif // SERV_KR_OPEN_TEST2_VERSION
	
	if( !m_stringTable.LoadIni( m_strIniFilePath.c_str() ) )
	{
		START_LOG( cerr, L"INI 파일을 읽어 오지 못했습니다." )
			<< BUILD_LOG( m_strIniFilePath )
			<< END_LOG;
	}

	m_stringTable.SetCurrentSection( ConfigINI::szSectionString );
}

const std::wstring& KBaseServer::GetMainVersion() const
{
	if( m_eUseVersion < 0  ||  m_eUseVersion >= VE_MAX )
	{
		START_LOG( cerr, L"잘못된 버전 인덱스 입니다!" )
			<< BUILD_LOG( m_eUseVersion )
			<< END_LOG;

		return m_strMainVer[VE_INTERNAL];
	}

	return m_strMainVer[m_eUseVersion];
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

namespace ConfigINI 
{
    /*//// ...in ini file... ////////////////////////////////////////////////////

    [Version]
    Protocol Version 1 = XXX
    Protocol Version 2 = YYY
    
    [String]
    aaa = AAA
    bbb = BBB
    ...

    /////////////////////////////////////////////////////////////////////////*/

    const wchar_t* szSectionVersion = L"Version";
    const wchar_t* szSectionString  = L"String";
	const wchar_t* szKeyVersion1    = L"Main Version";
	const wchar_t* szKeyVersion2    = L"Service Version";
}

void KBaseServer::LoadINIFile( const wchar_t* szFileName_ )
{
	// 050318. florist. ini 파일은 전체 경로(혹은 상대경로)를 적어서 사용해야 한다. 
	// 그렇지 않으면 무조건 윈도우 폴더 안에서만 찾는다. 이왕 해야 할거면 확실하게 절대경로를 쓰자.
	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	m_strIniFilePath = buff;
	m_strIniFilePath += L"\\";
	m_strIniFilePath += szFileName_;
	m_strIniFilePath += L".ini";
	
	// 050317. version 정보 얻어오기.
	//m_nProtocolVer  = ::GetPrivateProfileIntW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, m_strIniFilePath.c_str() );
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion1, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[0] = buff;
	::GetPrivateProfileStringW( ConfigINI::szSectionVersion, ConfigINI::szKeyVersion2, 0, buff, MAX_PATH, m_strIniFilePath.c_str() );
	m_strMainVer[1] = buff;

	LIF( !m_strMainVer[0].empty() );
	LIF( !m_strMainVer[1].empty() );

	//{{ 060201. kkurrung. 문자열 테이블
	if( !m_stringTable.LoadIni( m_strIniFilePath.c_str() ) )
	{
		START_LOG( cerr, L"INI 파일을 읽어 오지 못했습니다." )
			<< BUILD_LOG( m_strIniFilePath )
			<< END_LOG;
	}

	m_stringTable.SetCurrentSection( ConfigINI::szSectionString );

	//}} kkurrung.
}

void KBaseServer::SetMainVersion( int nIndex_, const char* szVersion_ )
{
	std::wstring* pstrVersion = &m_strMainVer[ nIndex_ ];
	const wchar_t* szVersionName = nIndex_ == 0 ? ConfigINI::szKeyVersion1 : ConfigINI::szKeyVersion2;

	START_LOG( cout, L"Change Main Version." )
		<< BUILD_LOG( nIndex_ )
		<< BUILD_LOG( szVersionName )
		<< BUILD_LOG( *pstrVersion )
		<< BUILD_LOG( szVersion_ );

	*pstrVersion = KncUtil::toWideString( szVersion_ );

	LIF( ::WritePrivateProfileStringW( ConfigINI::szSectionVersion, szVersionName,
		pstrVersion->c_str(), m_strIniFilePath.c_str() ) );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_GAME_SERVER_VERSION_DETAIL
//////////////////////////////////////////////////////////////////////////
//}}


void KBaseServer::SetServerList( const std::map< int, std::vector< KServerInfo > >& mapServerList )
{
    KLocker lock( m_csServerList );
    m_mapServerList = mapServerList;
}

void KBaseServer::GetServerList( std::map< int, std::vector< KServerInfo > >& mapServerList )
{
    KLocker lock( m_csServerList );
    mapServerList = m_mapServerList;
}

//void KBaseServer::GetServerInfo( KServerInfo& kServerInfo )
//{
//    kServerInfo.m_iUID = ( int )GetUID();
//    kServerInfo.m_wstrName = GetName();
//    kServerInfo.m_iServerGroupID = GetServerGroupID();
//    kServerInfo.m_iServerClass = GetServerClass();
//	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
//#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
//	kServerInfo.m_wstrIP = NetCommon::GetLocalIPW();
//#else
//	kServerInfo.m_wstrIP = NetCommon::GetLocalIPW();
//#endif SERV_PRIVATE_IP_SERVER_NETWORKING
//	//}}
//    kServerInfo.m_usMasterPort = KNetLayer::GetKObj()->m_usPortMaster;
//    kServerInfo.m_usNCUDPPort = KNetLayer::GetKObj()->GetNCUDPPort();
//	//{{ 2011. 12. 16	최육사	동접 최대 인원 실시간 적용
//#ifdef SERV_REALTIME_MAX_USER
//	kServerInfo.m_nMaxUser = GetDefaultMaxUser();
//#else
//	kServerInfo.m_nMaxUser = GetMaxUser();
//#endif SERV_REALTIME_MAX_USER
//	//}}    
//    kServerInfo.m_nCurrentUser = KActorManager::GetKObj()->GetCount();
//    kServerInfo.m_bOn = true;
//}

void KBaseServer::SetServerInfo( const KServerInfo& kServerInfo )
{
	if( GetServerGroupID() != kServerInfo.m_iServerGroupID )
	{
		START_LOG( cerr, L"명령인자로 받은 ServerGroupID와 DB로부터 서버인증받은 ServerGroupID가 다릅니다!" )
			<< BUILD_LOG( GetServerGroupID() )
			<< BUILD_LOG( kServerInfo.m_iServerGroupID )
			<< END_LOG;
	}

    SetUID( kServerInfo.m_iUID );
    SetName( kServerInfo.m_wstrName );
    SetServerGroupID( kServerInfo.m_iServerGroupID );
    SetServerClass( kServerInfo.m_iServerClass );
	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	SetPublicIP( kServerInfo.m_wstrPublicIP );
	SetPrivateIP( kServerInfo.m_wstrPrivateIP );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
	KNetLayer::GetKObj()->SetPort( kServerInfo.m_usMasterPort );
    KNetLayer::GetKObj()->InitNCUDP( kServerInfo.m_usNCUDPPort );
	//{{ 2011. 12. 16	최육사	동접 최대 인원 실시간 적용
#ifdef SERV_REALTIME_MAX_USER
	SetDefaultMaxUser( kServerInfo.m_nMaxUser );
#else
	SetMaxUser( kServerInfo.m_nMaxUser );
#endif SERV_REALTIME_MAX_USER
	//}}    
	//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
	m_cServerRollType = kServerInfo.m_cServerRollType;
	if( m_cServerRollType != KServerInfo::SRT_NONE )
	{
		START_LOG( cout, L"--- 서버별 역할 부여 ---" )
			<< BUILD_LOGc( GetServerRollType() );
	}
#endif SERV_SERVER_ROLL_TYPE
	//}}
}

bool KBaseServer::Authenticate( DWORD dwTimeOut_ )
{
    KNetAddress kPacket;
    kPacket.m_wstrIP    = NetCommon::GetLocalIPW();
    kPacket.m_usPort    = KNetLayer::GetKObj()->m_usPortMaster;

    SendToLogDB( DBE_SERVER_ON_REQ, kPacket );

    switch( ::WaitForSingleObject( m_spEventAck.get(), dwTimeOut_ ) )
    {
    case WAIT_OBJECT_0: // ack를 제대로 받은 경우.
        return true;
    case WAIT_TIMEOUT: // 시간 초과
        START_LOG( cerr, L"verify account timeout." );
        return false;
    default:
        START_LOG( cerr, L"invalide event object." );
        return false;
    }

    return false; // never get here.
}

//{{ 2008. 10. 10  최육사	
UidType KBaseServer::GetFirstActorKey()
{
	return KActorManager::GetKObj()->GetFirstActorKey();
}
//}}

//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
void KBaseServer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
#endif SERV_MAX_QUEUE_SIZE_DUMP
//}}

//{{ 2010. 11. 12	최육사	서버간 접속 모니터링
#ifdef SERV_MORNITORING
void KBaseServer::SetConnectionMornitoringEnable_LUA( bool bVal )
{
	m_bConnMornitoringEnable = bVal;

	START_LOG( cout, L"서버간 접속 모니터링 동작 여부 : " << m_bConnMornitoringEnable );
}
#endif SERV_MORNITORING
//}}

//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
void KBaseServer::SetScriptParsingErrReport_LUA( bool bVal )
{
	m_bScriptParsingErrReport = bVal;

	START_LOG( cout, L"스크립트 파싱 오류 리포트 기능 동작 여부 : " << m_bScriptParsingErrReport );
}

void KBaseServer::AddScriptParsingErrReportMail_LUA( const char* pszMail )
{
	std::wstring wstrMail = KncUtil::toWideString( pszMail );
    m_vecScriptParsingErrMailList.push_back( wstrMail );

	START_LOG( cout, L"-- 스크립트 파싱 오류 리포트 대상 메일 주소 등록 --" )
		<< BUILD_LOG( wstrMail );
}

#endif SERV_SCRIPT_PARSING_ERR_REPORT
//}}

void KBaseServer::SendToAccountDB( unsigned short usEventID )
{
    SendToAccountDB( usEventID, char() );
}

void KBaseServer::SendToLogDB( unsigned short usEventID )
{
    SendToLogDB( usEventID, char() );
}

#ifdef SERV_ADD_EVENT_DB
void KBaseServer::SendToEventDB( unsigned short usEventID )
{
	SendToEventDB( usEventID, char() );
}
#endif //SERV_ADD_EVENT_DB

//{{ 2011.2.11  조효진  모니터링툴 서버 포트 체크 시 에러로그 남는거 수정
#ifdef SERV_PERMIT_PORT_CHECK
void KBaseServer::AddPortCheckMoritoringServer_LUA( const char* pStrIP )
{
	m_PortCheckMoritoringServerIPList.insert( pStrIP );
}

bool KBaseServer::IsMonitoringServerForPortCheck( std::string strIP )
{
	if( m_PortCheckMoritoringServerIPList.find(strIP) == m_PortCheckMoritoringServerIPList.end() )
		return false;
	else
		return true;
}
#endif SERV_PERMIT_PORT_CHECK
//}}
