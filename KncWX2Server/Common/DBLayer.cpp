#include "DBLayer.h"
#include "odbc/Odbc.h"
#include "KncSend.h"
#include "BaseServer.h"
//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
	#include "DBConnectionManager.h"
#endif SERV_DB_CONNECTION_SECURITY
//}}

#undef _ENUM
#define _ENUM( id ) #id,
const char*                 KDBLayer::ms_szDBConnection[] =
{
#include "DBConnectionInfo_def.h"
};

ImplToStringW( KDBLayer )
{
    std::map< int, KDBThreadManagerPtr >::const_iterator mit;
    for( mit = m_mapDBAgent.begin(); mit != m_mapDBAgent.end(); ++mit )
    {
		stm_ << L"---------------------------------------" << std::endl;
        stm_ << *( mit->second );
    }
    KODBC::DumpMaxTime( stm_ );

    return stm_;
}

ImplementSingleton( KDBLayer );

KDBLayer::KDBLayer()
:
m_bSMSEnable(true)
{
	//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
	m_spEventAck.reset( ::CreateEvent( NULL, false, false, NULL ), ::CloseHandle );
#endif SERV_DB_CONNECTION_SECURITY
	//}}
}

#include <KncLua.h>
#include <lua_tinker.h>

void KDBLayer::RegToLua()
{
	lua_tinker::def( g_pLua, "ResetMaxTimeQuery", &KODBC::ResetMaxTime );

	lua_tinker::class_add<KDBLayer>( g_pLua, "KDBLayer" );
	lua_tinker::class_def<KDBLayer>( g_pLua, "dump",			&KDBLayer::Dump );
	lua_tinker::class_def<KDBLayer>( g_pLua, "AddDB",			&KDBLayer::AddDBConnectionInfo_LUA );
	//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
	lua_tinker::class_def<KDBLayer>( g_pLua, "SetManagerDB",	&KDBLayer::SetManagerDBConnectionInfo_LUA );
#endif SERV_DB_CONNECTION_SECURITY
	//}}
	lua_tinker::class_def<KDBLayer>( g_pLua, "SetSMSEnable",	&KDBLayer::SetSMSEnable_LUA );

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KDBLayer::##id );
#   include "DBConnectionInfo_def.h"

	lua_tinker::decl( g_pLua, "DBLayer", this );
}

bool KDBLayer::AddDBConnectionInfo_LUA( int iDBConnection, const char* szDSN, int nThread, bool bDBConnStr )
{
    if( !szDSN )
    {
        START_LOG( cerr, L"DSN 문자열 포인터가 NULL임." )
            << BUILD_LOG( iDBConnection )
            << BUILD_LOG( nThread )
            << END_LOG;

        return false;
    }

    std::map< int, KDBThreadManagerPtr >::iterator mit = m_mapDBAgent.find( iDBConnection );
    if( mit != m_mapDBAgent.end() )
    {
        START_LOG( cerr, L"이미 존재하는 DB임." )
            << BUILD_LOG( iDBConnection )
            << BUILD_LOG( szDSN )
            << BUILD_LOG( nThread )
            << END_LOG;

        return false;
    }

    KDBThreadManagerPtr spDBThreadManager = KDBThreadManagerPtr( new KDBThreadManager );
    spDBThreadManager->SetDBConnectionID( iDBConnection );
	//{{ 2009. 6. 17  최육사	DBConnStr
	if( bDBConnStr )
	{
		std::vector< KClearString< std::wstring > > vecCmdArgs;
		KBaseServer::GetKObj()->GetCmdArgs( vecCmdArgs );
		if( vecCmdArgs.size() < 2 )
		{
			START_LOG( cerr, L"DB Connection String으로 세팅되어있는데 DBConn ID와 PW가 입력되지 않았습니다." )
				<< BUILD_LOG( iDBConnection )
				<< BUILD_LOG( szDSN )
				<< BUILD_LOG( nThread )
				<< BUILD_LOG( vecCmdArgs.size() )
				<< END_LOG;

			return false;
		}

		// DB Connection String
		const std::wstring wstrDSN = KncUtil::toWideString( szDSN );
		spDBThreadManager->SetDBConnString( wstrDSN, vecCmdArgs[0], vecCmdArgs[1] );
	}
	else
	{
		spDBThreadManager->SetFileDSN( szDSN );
	}
	//}}
    spDBThreadManager->Init( nThread );

    m_mapDBAgent.insert( std::make_pair( iDBConnection, spDBThreadManager ) );

    return true;
}

//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
bool KDBLayer::AddDBConnectionByString( IN const int iDBType, IN const std::wstring& wstrDBConnectionString, IN const int iThreadCount )
{
	if( wstrDBConnectionString.empty() )
	{
		START_LOG( cerr, L"DSN 문자열 포인터가 NULL임." )
			<< BUILD_LOG( iDBType )
			<< BUILD_LOG( iThreadCount )
			<< END_LOG;
		return false;
	}

	std::map< int, KDBThreadManagerPtr >::iterator mit = m_mapDBAgent.find( iDBType );
	if( mit != m_mapDBAgent.end() )
	{
		START_LOG( cerr, L"이미 존재하는 DB임." )
			<< BUILD_LOG( iDBType )
			<< BUILD_LOG( iThreadCount )
			<< END_LOG;

		return false;
	}

	KDBThreadManagerPtr spDBThreadManager = KDBThreadManagerPtr( new KDBThreadManager );
	spDBThreadManager->SetDBConnectionID( iDBType );
	spDBThreadManager->SetDBConnString( wstrDBConnectionString, KClearString< std::wstring >(), KClearString< std::wstring >() );
	spDBThreadManager->Init( iThreadCount );
	m_mapDBAgent.insert( std::make_pair( iDBType, spDBThreadManager ) );
	return true;
}

bool KDBLayer::SetManagerDBConnectionInfo_LUA( int iServerClass, const char* szDSN, bool bDBConnStr )
{
	if( !szDSN )
	{
		START_LOG( cerr, L"DSN 문자열 포인터가 NULL임." )
			<< END_LOG;
		return false;
	}

	KClearString< std::wstring > wstrDSN = KncUtil::toWideString( szDSN );
	if( bDBConnStr )
	{
		std::vector< KClearString< std::wstring > > vecCmdArgs;
		KBaseServer::GetKObj()->GetCmdArgs( vecCmdArgs );
		if( vecCmdArgs.size() < 2 )
		{
			START_LOG( cerr, L"DB Connection String으로 세팅되어있는데 DBConn ID와 PW가 입력되지 않았습니다." )
				<< BUILD_LOG( szDSN )
				<< BUILD_LOG( vecCmdArgs.size() )
				<< END_LOG;
			return false;
		}
		
		// UID와 PWD를 조합한 DB Connection String을 만들어낸다! - 절대로 외부 노출되면 안됨!
		wstrDSN += L";UID=";
		wstrDSN += vecCmdArgs[0];
		wstrDSN += L";PWD=";
		wstrDSN += vecCmdArgs[1];
	}

	// 본섭DB 접속 정보를 가져오자!
	KDBConnectionManager kDBConnectionManager;
	std::vector< KDBConnectionInfo > vecDBConnectionInfo;
	if( kDBConnectionManager.GetDBConnectionInfoFromManagerDB( iServerClass, wstrDSN.c_str(), bDBConnStr, vecDBConnectionInfo ) == false )
	{
		START_LOG( cerr, L"DB접속정보를 가져오지 못하였습니다!" )
			<< BUILD_LOG( iServerClass )
			<< BUILD_LOG( szDSN )
			<< BUILD_LOG( bDBConnStr )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KDBConnectionInfo&, kInfo, vecDBConnectionInfo )
	{
		if( AddDBConnectionByString( kInfo.m_iDBType, kInfo.m_wstrConnectionString, kInfo.m_iThreadCount ) == false )
		{
			START_LOG( cerr, L"DB 접속 정보 추가 실패!" )
				<< BUILD_LOG( kInfo.m_iDBType )
				<< BUILD_LOG( kInfo.m_iThreadCount )
				<< END_LOG;
			continue;
		}
	}
	
	return true;
}
#endif SERV_DB_CONNECTION_SECURITY
//}}

void KDBLayer::SetSMSEnable_LUA( bool bEnable )
{
	START_LOG( cout2, L"SMS DB ON / OFF" )
		<< BUILD_LOG( (int)bEnable );

	m_bSMSEnable = bEnable;
}

void KDBLayer::Init()
{
    std::map< int, KDBThreadManagerPtr >::iterator mit;
    for( mit = m_mapDBAgent.begin(); mit != m_mapDBAgent.end(); ++mit )
    {
        mit->second->BeginThread();
    }
}

void KDBLayer::ShutDown( DWORD dwWaitTime )
{
    std::map< int, KDBThreadManagerPtr >::iterator mit;
    for( mit = m_mapDBAgent.begin(); mit != m_mapDBAgent.end(); ++mit )
    {
        mit->second->EndThread( dwWaitTime );
    }

    int iRemainThreadNum = GetTotalThreadNum();

    LOG_SUCCESS( iRemainThreadNum == 0 )
        << BUILD_LOG( iRemainThreadNum );
}

void KDBLayer::QueueingEvent( const KEventPtr& spEvent_ )
{
    std::map< int, KDBThreadManagerPtr >::iterator mit;

    switch( GetPerformerClass( spEvent_->m_kDestPerformer.m_dwPerformerID ) )
    {
    case PC_ACCOUNT_DB:
        mit = m_mapDBAgent.find( DC_ACCOUNT );
        if( mit == m_mapDBAgent.end() )
        {
            START_LOG( cerr, L"Account DB Agent 가 없음." )
                << END_LOG;
            return;
        }
        break;
    case PC_GAME_DB:
        mit = m_mapDBAgent.find( DC_GAME );
        if( mit == m_mapDBAgent.end() )
        {
            START_LOG( cerr, L"Game DB Agent 가 없음." )
                << END_LOG;
            return;
        }
        break;
    case PC_LOG_DB:
        mit = m_mapDBAgent.find( DC_LOG );
        if( mit == m_mapDBAgent.end() )
        {
            START_LOG( cerr, L"Log DB Agent 가 없음." )
                << END_LOG;
            return;
        }
        break;
		//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	case PC_NX_WEB_DB:
		mit = m_mapDBAgent.find( DC_NX_WEB );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"Nx Web DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif SERV_WEB_POINT_EVENT
		//}}    
	case PC_SMS_DB:
		if( m_bSMSEnable == false )
			return;
		mit = m_mapDBAgent.find( DC_SMS );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"SMS DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
    case PC_GAME_DB_2ND:
        mit = m_mapDBAgent.find( DC_GAME_2ND );
        if( mit == m_mapDBAgent.end() )
        {
            START_LOG( cerr, L"GAME 2nd DB Agent 가 없음." )
                << END_LOG;
            return;
        }
        break;
    case PC_LOG_DB_2ND:
        mit = m_mapDBAgent.find( DC_LOG_2ND );
        if( mit == m_mapDBAgent.end() )
        {
            START_LOG( cerr, L"LOG 2nd DB Agent 가 없음." )
                << END_LOG;
            return;
        }
        break;
#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_IN
	case PC_PUBLISHER_AUTH_DB:
		mit = m_mapDBAgent.find( DC_PUBLISHER_AUTH );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"Publisher Auth DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif SERV_COUNTRY_IN
#endif SERV_GLOBAL_AUTH
#ifdef SERV_GLOBAL_BILLING
	case PC_KOG_BILLING_DB:
		mit = m_mapDBAgent.find( DC_KOG_BILLING );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"KOG Billing DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
	case PC_PUBLISHER_BILLING_DB:
		mit = m_mapDBAgent.find( DC_PUBLISHER_BILLING );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"Publisher Billing DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif // SERV_GLOBAL_BILLING

		//{{ 2010. 11. 11  조효진	GameForge 채팅 로그 관련 처리
#ifdef SERV_RECORD_CHAT
	case PC_CHAT_LOG_DB:
		mit = m_mapDBAgent.find( DC_CHAT_LOG );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"Chat Log DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif SERV_RECORD_CHAT
		//}}
#ifdef SERV_ID_NETMARBLE_PCBANG
	case PC_ID_PCBANG_AUTH_DB:
		mit = m_mapDBAgent.find( DC_ID_PCBANG_AUTH );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"인도네시아 PC방 DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif //SERV_ID_NETMARBLE_PCBANG
		//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
	case PC_JP_RELAY_DB:
		mit = m_mapDBAgent.find( DC_JP_RELAY_DB );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"일본 중계DB Agent 가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif SERV_RELAY_DB_CONNECTION
		//}}
#ifdef SERV_ADD_EVENT_DB
	case PC_EVENT_DB:
		mit = m_mapDBAgent.find( DC_EVENT );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"게임 이벤트 DB Agent가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif //SERV_ADD_EVENT_DB
#ifdef SERV_ADD_SCRIPT_DB
	case PC_SCRIPT_DB:
		mit = m_mapDBAgent.find( DC_SCRIPT );
		if( mit == m_mapDBAgent.end() )
		{
			START_LOG( cerr, L"스크립트 DB Agent가 없음." )
				<< END_LOG;
			return;
		}
		break;
#endif //SERV_ADD_SCRIPT_DB

    default:
        START_LOG( cerr, L"무슨 DB를 찾는고?" )
            << BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << END_LOG;
        return;
    }

    mit->second->QueueingEvent( spEvent_ );
}

int KDBLayer::GetTotalThreadNum()
{
    int nRet = 0;

    std::map<int, KDBThreadManagerPtr>::iterator mit;
    for( mit = m_mapDBAgent.begin(); mit != m_mapDBAgent.end(); ++mit )
    {
        nRet += mit->second->GetThreadNum();
    }

    return nRet;
}

int KDBLayer::GetTotalEventNum()
{
    int nRet = 0;

    std::map<int, KDBThreadManagerPtr>::iterator mit;
    for( mit = m_mapDBAgent.begin(); mit != m_mapDBAgent.end(); ++mit )
    {
        nRet += mit->second->GetQueueSize();
    }

    return nRet;
}

//{{ 2009. 12. 15  최육사	서버관리
void KDBLayer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
//}}


