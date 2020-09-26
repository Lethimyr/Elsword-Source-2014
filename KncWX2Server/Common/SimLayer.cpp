#pragma warning( disable : 4800 )
#include <Winsock2.h>   
#include "SimLayer.h"
#include "NetLayer.h"
#include "BaseServer.h"
#include "ActorManager.h"
#include <KncLua.h>
#include "Thread/Locker.h"
#include <sstream>  // std::ostringstream
#include <boost/bind.hpp>
#include "KAutoPath.h"
//{{ 2013. 02. 15	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

ImplInstanceKeeping( KSimLayer );
NiImplementRootRTTI( KSimLayer );
#ifdef _CONVERT_VS_2010
#else
ImplementException( KSimLayer );
#endif _CONVERT_VS_2010

KSimLayer::KSimLayer( ) 
:
//{{ 2012. 05. 10	최육사	서버 버전
//#ifdef SERV_VERSION_FLAG
	m_dwVersionFlag( VF_NONE ),
//#endif SERV_VERSION_FLAG
//}}
    m_dwCommonFlag( CF_NONE ),
    m_dwAuthFlag( AF_INTERNAL ),
    m_dwBillingFlag( BF_NONE )
#ifdef SERV_USE_NATION_FLAG
	,m_dwNationFlag( NF_NONE )
#endif SERV_USE_NATION_FLAG
#ifdef SERV_KOG_OTP_VERIFY
	,m_bUseKogOTP( false )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_COUNTRY_TH
	,m_wstrAuthID( L"" )
	,m_wstrAuthPassword( L"" )
	,m_wstrDomain( L"" )
#endif // SERV_COUNTRY_
#ifdef SERV_GLOBAL_BILLING
	,m_dwPublisherBillingConnectType( 0 )
	,m_bCheckCouponByPublisher( false )
#endif // SERV_GLOBAL_BILLING
{	
    m_mapAuthTypeStrings[ AF_NEXON_KOREA ] = L"AF_NEXON_KOREA";
    //m_mapAuthTypeStrings[ AF_GAMANIA_TAIWAN ] = L"AF_GAMANIA_TAIWAN"; // 해외팀 제거
}

KSimLayer::~KSimLayer()
{
	//{{ 2010. 8. 30	최육사	서버 정상 종료 처리	
	// 명시적으로 shutdown호출하고 있는데 여기서 또 호출하고 있네 -ㅅ - ; 주석처리함.
    //std::cout << "~KSimLayer" << std::endl;
    //ShutDown();
	//}}
}

void KSimLayer::Init()
{
	//{{ 2013. 02. 15	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	{
		// 모든 서버가 공통으로 스크립트 파싱을 해야 하므로 여기에다가 넣었음.
		KLogManager::RegScriptName( "LogConfig.lua" );
		OPEN_SCRIPT_FILE( KLogManager );
	}
#endif SERV_LOG_SYSTEM_NEW
	//}}

    // run thread manager.
    std::for_each( m_vecpThreadMgr.begin(), m_vecpThreadMgr.end(), boost::bind( &KThreadManager::BeginThread, _1 ) );
}

ImplToStringW( KSimLayer )
{
	//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	KActorManager::GetKObj()->ToString( stm_ );
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}

	//{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	return START_TOSTRINGW
		<< TOSTRINGW( KActorManager::GetKObj()->GetCount() )
		<< TOSTRINGW( GetAuthFlag() )
		<< TOSTRINGW( GetBillingFlag() )
		<< TOSTRINGW( GetSimLayerMaxTick() )
		<< TOSTRINGW( GetSimLayerMaxTickRegDate() );
#else
	return START_TOSTRINGW
		<< TOSTRINGW( KActorManager::GetKObj()->GetCount() )
		<< TOSTRINGW( GetAuthFlag() )
		<< TOSTRINGW( GetBillingFlag() );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}    
}

void KSimLayer::Run()
{
    START_LOG( clog, L"Enter Run()" );

    DWORD ret;

    while( true )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, 1 );

        if( ret == WAIT_OBJECT_0 ) break;

        if( ret == WAIT_TIMEOUT ) Tick();

        else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
    }
}

void KSimLayer::Tick()
{
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

    KActorManager::GetKObj()->Tick();

    KBaseServer::GetKObj()->Tick();

	//{{ 2013. 02. 	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	m_kTickChecker.CheckSimLayerTick();
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}

	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}

void KSimLayer::ShutDown()
{
    End();
	
	// 스레드 매니저 종료 처리
    std::for_each( m_vecpThreadMgr.begin(), m_vecpThreadMgr.end(), boost::bind( &KThreadManager::EndThread, _1, 10000 ) );	

	// SimLayer가 소멸할때 멤버로 가지고 있던 ThreadManager들도 죽이자!
	m_vecpThreadMgr.clear();
	//}}

    KActorManager::ReleaseKObj();
}

//{{ 2009. 7. 13  최육사	하드코딩 제거
void KSimLayer::AddPath( const char* pStrPath )
{
	KAutoPath::AddPath( std::string( pStrPath ) );
}
//}}

//{{ 2009. 12. 15  최육사	서버관리
void KSimLayer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
//}}

#include <lua_tinker.h>

void KSimLayer::RegToLua()
{
	lua_tinker::class_add<KSimLayer>( g_pLua, "KSimLayer" );
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	lua_tinker::class_def<KSimLayer>( g_pLua, "GetVersionFlag",		&KSimLayer::GetVersionFlag );
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetVersionFlag",		&KSimLayer::SetVersionFlag );
	//#endif SERV_VERSION_FLAG
	//}}
	lua_tinker::class_def<KSimLayer>( g_pLua, "AddCommonFlag",		&KSimLayer::AddCommonFlag );
	lua_tinker::class_def<KSimLayer>( g_pLua, "DeleteCommonFlag",	&KSimLayer::DeleteCommonFlag );
	lua_tinker::class_def<KSimLayer>( g_pLua, "CheckCommonFlag",	&KSimLayer::CheckCommonFlag );
	lua_tinker::class_def<KSimLayer>( g_pLua, "GetAuthFlag",		&KSimLayer::GetAuthFlag );
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetAuthFlag",		&KSimLayer::SetAuthFlag );
#ifdef SERV_COUNTRY_TH
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetAuthAccount",		&KSimLayer::SetAuthAccount );
#endif // SERV_COUNTRY_TH
	lua_tinker::class_def<KSimLayer>( g_pLua, "GetBillingFlag",		&KSimLayer::GetBillingFlag );
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetBillingFlag",		&KSimLayer::SetBillingFlag );
#ifdef SERV_COUNTRY_JP
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetHanInitFlag",		&KSimLayer::SetHanInitFlag );
#endif //SERV_COUNTRY_JP
	//{{ 2009. 7. 13  최육사	하드코딩 제거
	lua_tinker::class_def<KSimLayer>( g_pLua, "AddPath",			&KSimLayer::AddPath );
	//}}	
	//{{ 2011.11.17 임홍락 유럽 서버 단일화
#ifdef SERV_USE_NATION_FLAG
	lua_tinker::class_def<KSimLayer>( g_pLua, "GetNationFlag",		&KSimLayer::GetDwNationFlag );
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetNationFlag",		&KSimLayer::SetDwNationFlag );

	lua_tinker::decl( g_pLua, "NF_NONE",				KSimLayer::NF_NONE );
	lua_tinker::decl( g_pLua, "NF_DE",					KSimLayer::NF_DE );
	lua_tinker::decl( g_pLua, "NF_FR",					KSimLayer::NF_FR );
	lua_tinker::decl( g_pLua, "NF_IT",					KSimLayer::NF_IT );
	lua_tinker::decl( g_pLua, "NF_PL",					KSimLayer::NF_PL );
	lua_tinker::decl( g_pLua, "NF_ES",					KSimLayer::NF_ES );
	lua_tinker::decl( g_pLua, "NF_TR",					KSimLayer::NF_TR );
	lua_tinker::decl( g_pLua, "NF_EN",					KSimLayer::NF_EN );

	lua_tinker::decl( g_pLua, "NF_KR",					KSimLayer::NF_KR );
	lua_tinker::decl( g_pLua, "NF_JP",					KSimLayer::NF_JP );
	lua_tinker::decl( g_pLua, "NF_TW",					KSimLayer::NF_TW );
	lua_tinker::decl( g_pLua, "NF_HK",					KSimLayer::NF_HK );
	lua_tinker::decl( g_pLua, "NF_US",					KSimLayer::NF_US );
	lua_tinker::decl( g_pLua, "NF_CN",					KSimLayer::NF_CN );
	lua_tinker::decl( g_pLua, "NF_VN",					KSimLayer::NF_VN );
	lua_tinker::decl( g_pLua, "NF_TH",					KSimLayer::NF_TH );
	lua_tinker::decl( g_pLua, "NF_ID",					KSimLayer::NF_ID );
	lua_tinker::decl( g_pLua, "NF_BR",					KSimLayer::NF_BR );
	lua_tinker::decl( g_pLua, "NF_PH",					KSimLayer::NF_PH );
	lua_tinker::decl( g_pLua, "NF_IN",					KSimLayer::NF_IN );
#endif SERV_USE_NATION_FLAG
	//}} 2011.11.17 임홍락 유럽 서버 단일화

#ifdef SERV_KOG_OTP_VERIFY
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetUseKogOTP",		&KSimLayer::SetUseKogOTP );
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_GLOBAL_BILLING
	lua_tinker::class_def<KSimLayer>( g_pLua, "SetPublisherBillingConnectType",		&KSimLayer::SetPublisherBillingConnectType );

	lua_tinker::decl( g_pLua, "PBCT_NONE",				KSimLayer::PBCT_NONE );
	lua_tinker::decl( g_pLua, "PBCT_NORMAL",			KSimLayer::PBCT_NORMAL );
	lua_tinker::decl( g_pLua, "PBCT_DB",				KSimLayer::PBCT_DB );
	lua_tinker::decl( g_pLua, "PBCT_UNUSUAL",			KSimLayer::PBCT_UNUSUAL );

	lua_tinker::class_def<KSimLayer>( g_pLua, "SetCheckCouponByPublisher",		&KSimLayer::SetCheckCouponByPublisher );
#endif // SERV_GLOBAL_BILLING

	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	lua_tinker::decl( g_pLua, "VF_INTERNAL",			KSimLayer::VF_INTERNAL );
	lua_tinker::decl( g_pLua, "VF_OPEN_TEST",			KSimLayer::VF_OPEN_TEST );
	lua_tinker::decl( g_pLua, "VF_SERVICE",				KSimLayer::VF_SERVICE );
	//#endif SERV_VERSION_FLAG
	//}}
	lua_tinker::decl( g_pLua, "CF_NPGG",				KSimLayer::CF_NPGG );
	lua_tinker::decl( g_pLua, "CF_CHECK_IP",			KSimLayer::CF_CHECK_IP );
	//{{ 2009. 8. 18  최육사	hack shield
	lua_tinker::decl( g_pLua, "CF_HSHIELD",				KSimLayer::CF_HSHIELD );
	//}}
#ifdef SERV_USE_XTRAP
	lua_tinker::decl( g_pLua, "CF_XTRAP",				KSimLayer::CF_XTRAP );
#endif SERV_USE_XTRAP

	lua_tinker::decl( g_pLua, "AF_INTERNAL",			KSimLayer::AF_INTERNAL );
	lua_tinker::decl( g_pLua, "AF_NEXON_KOREA",			KSimLayer::AF_NEXON_KOREA );
    lua_tinker::decl( g_pLua, "AF_GLOBAL_SERVICE",      KSimLayer::AF_GLOBAL_SERVICE );
	lua_tinker::decl( g_pLua, "BF_INTERNAL",			KSimLayer::BF_INTERNAL );
	lua_tinker::decl( g_pLua, "BF_NEXON_KOREA_TEST",	KSimLayer::BF_NEXON_KOREA_TEST );
	lua_tinker::decl( g_pLua, "BF_NEXON_KOREA",			KSimLayer::BF_NEXON_KOREA );
	lua_tinker::decl( g_pLua, "BF_GLOBAL_SERVICE",		KSimLayer::BF_GLOBAL_SERVICE );
	
#ifdef SERV_COUNTRY_JP
	lua_tinker::decl( g_pLua, "HIF_ALPHA",				KSimLayer::HIF_ALPHA );
	lua_tinker::decl( g_pLua, "HIF_REAL",				KSimLayer::HIF_REAL );
#endif //SERV_COUNTRY_JP
}
void KSimLayer::AddCommonFlag( DWORD dwFlag )
{ 
	KLocker lock( m_csCommonFlag ); 

	m_dwCommonFlag |= dwFlag; 

	START_LOG( cout, L"Add Common Flag Success : " << dwFlag );
}
void KSimLayer::DeleteCommonFlag( DWORD dwFlag )
{
	KLocker lock( m_csCommonFlag );
	
	m_dwCommonFlag &= ~dwFlag;

	START_LOG( cout, L"Delete Common Flag Success : " << dwFlag );
}

const std::wstring KSimLayer::GetAuthTypeStr( DWORD dwFlag_ ) const
{
    std::map< DWORD, std::wstring >::const_iterator mit;
    std::wstringstream stm;
    //stm << L"-- Auth Type --" << std::endl;
    for( mit = m_mapAuthTypeStrings.begin() ; mit != m_mapAuthTypeStrings.end() ; ++mit )
    {
        if( mit->first == dwFlag_ )
        {
            stm << boost::wformat( L"%s" ) % mit->second;
        }

    }
    return stm.str();
}
