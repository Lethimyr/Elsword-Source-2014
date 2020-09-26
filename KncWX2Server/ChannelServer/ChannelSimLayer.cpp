#include "ChannelSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "ActorManager.h"
#include "DBLayer.h"
#include "ChannelServer.h"
#include "KAutoPath.h"

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#include "ProxyManager.h"
#include "ChannelLoginProxy.h"
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_GLOBAL_AUTH
#if defined SERV_COUNTRY_EU
#include "../Common/OnlyGlobal/AuthAndBilling/EU/GameForgeAuthManager.h"
#elif defined SERV_COUNTRY_US
#include "../Common/OnlyGlobal/AuthAndBilling/US/K3RComboAuthManager.h"
#elif defined SERV_COUNTRY_TWHK
#include "../Common/OnlyGlobal/AuthAndBilling/TW/GASHAuthManager.h"
#elif defined SERV_COUNTRY_JP
#include "../Common/OnlyGlobal/AuthAndBilling/JP/PURPLEAuthManager.h"
#elif defined SERV_COUNTRY_BR
#include "../Common/OnlyGlobal/AuthAndBilling/BR/LevelUpAuthManager.h"
#elif defined SERV_COUNTRY_IN
#include "../Common/OnlyGlobal/AuthAndBilling/IN/FunizenAuthManager.h"
#endif // SERV_COUNTRY
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "ChannelUser.h"

#include "DBLayer.h"
#include "NetError.h"

//#include "GASHAuthManager.h"

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	#include "HackingProtectionManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}
//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "WaitQueueManager.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	#include "DDOSGuardSimulator.h"
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
//}}

//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

#include <lua_tinker.h>
using namespace lua_tinker;

#include <boost/bind.hpp>

NiImplementRTTI( KChannelSimLayer, KSimLayer );

KChannelSimLayer::KChannelSimLayer(void) 
{
	//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
	m_eCheckIPMode = CIM_SERVICE_MODE;
#else	
	m_eCheckIPType = CIT_ALL_IP_ALLOW;	
#endif SERV_CHANNEL_SERV_CHECK_MODE
	//}}
	//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	m_bCheckServerSN		= false;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}
}

KChannelSimLayer::~KChannelSimLayer(void)
{
}

ImplToStringW( KChannelSimLayer )
{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	START_TOSTRING_PARENTW( KSimLayer );
	SiKProxyManager()->ToString( stm_ );
	return stm_;
#else
    return START_TOSTRING_PARENTW( KSimLayer );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
}

ImplOstmOperatorW2A( KChannelSimLayer );

void KChannelSimLayer::RegToLua()
{
    KSimLayer::RegToLua();
    
	lua_tinker::class_add<KChannelSimLayer>( g_pLua, "KChannelSimLayer" );
	lua_tinker::class_inh<KChannelSimLayer, KSimLayer>( g_pLua );
	lua_tinker::class_def<KChannelSimLayer>( g_pLua, "dump",					&KChannelSimLayer::Dump );	
	lua_tinker::class_def<KChannelSimLayer>( g_pLua, "AddIP",					&KChannelSimLayer::AddIP );
	lua_tinker::class_def<KChannelSimLayer>( g_pLua, "SetCheckIPType",			&KChannelSimLayer::SetCheckIPType );
	//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
	lua_tinker::class_def<KChannelSimLayer>( g_pLua, "AddCheckModeIP",			&KChannelSimLayer::AddCheckModeIP );
	lua_tinker::class_def<KChannelSimLayer>( g_pLua, "SetCheckModeCheckIPType",	&KChannelSimLayer::SetCheckModeCheckIPType );
	lua_tinker::class_def<KChannelSimLayer>( g_pLua, "SetCheckIPMode",			&KChannelSimLayer::SetCheckIPMode_LUA );
#endif SERV_CHANNEL_SERV_CHECK_MODE
	//}}

	lua_tinker::decl( g_pLua, "SimLayer", this );
	lua_tinker::decl( g_pLua, "CIT_ALL_IP_ALLOW",		KChannelSimLayer::CIT_ALL_IP_ALLOW );
	lua_tinker::decl( g_pLua, "CIT_CHECK_IP_ALLOW",		KChannelSimLayer::CIT_CHECK_IP_ALLOW );
	lua_tinker::decl( g_pLua, "CIT_CHECK_IP_BLOCK",		KChannelSimLayer::CIT_CHECK_IP_BLOCK );

	//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
	lua_tinker::decl( g_pLua, "CIM_SERVICE_MODE",		KChannelSimLayer::CIM_SERVICE_MODE );
	lua_tinker::decl( g_pLua, "CIM_CHECK_MODE",			KChannelSimLayer::CIM_CHECK_MODE );
#endif SERV_CHANNEL_SERV_CHECK_MODE
	//}}

	//{{ 2013.03.21 조효진 국가별 AuthManager 다름
#ifdef SERV_GLOBAL_AUTH
#if defined SERV_COUNTRY_EU
	SiKGameForgeAuthManager()->RegToLua();
#elif defined SERV_COUNTRY_US
	SiKK3RComboAuthManager()->RegToLua();
#elif defined SERV_COUNTRY_TWHK
	SiKGASHAuthManager()->RegToLua();
#elif defined SERV_COUNTRY_JP
	SiKPurpleAuthManager()->RegToLua();
#elif defined SERV_COUNTRY_BR
	SiKLevelUpAuthManager()->RegToLua();
#elif defined SERV_COUNTRY_IN
	//SiKFunizenAuthManager()->RegToLua(); 나중에 쓰일 수 있어서 주석처리만 해놓음
#endif // SERV_COUNTRY
#endif // SERV_GLOBAL_AUTH
	//}}
}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
KActorProxy* CreateProxy( int iProxyType )
{
	if( iProxyType < 0 || iProxyType >= KProxyManager::PT_NUM )
	{
		START_LOG( cerr, L"프록시 타입이 이상함." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;

		return NULL;
	}

	KActorProxy* pkProxy;
	switch( iProxyType )
	{
	case KProxyManager::PT_LOGIN:
		pkProxy = new KChannelLoginProxy;
		pkProxy->SetSendQueueSize( 512 * 1024 );
		return pkProxy;
	default:
		START_LOG( cerr, L"프록시 타입이 이상함." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;
		return NULL;
	}
}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

void KChannelSimLayer::Init()
{
	//{{ 2013.03.21 조효진 국가별 AuthManager 다름
#ifdef SERV_GLOBAL_AUTH
#if defined SERV_COUNTRY_EU
	m_vecpThreadMgr.push_back( SiKGameForgeAuthManager()->GetInstance() );
#elif defined SERV_COUNTRY_US
	m_vecpThreadMgr.push_back( SiKK3RComboAuthManager()->GetInstance() );
#elif defined SERV_COUNTRY_TWHK
	m_vecpThreadMgr.push_back( SiKGASHAuthManager()->GetInstance() );
#elif defined SERV_COUNTRY_JP
	m_vecpThreadMgr.push_back( SiKPurpleAuthManager()->GetInstance() );
#elif defined SERV_COUNTRY_BR
	m_vecpThreadMgr.push_back( SiKLevelUpAuthManager()->GetInstance() );
#elif defined SERV_COUNTRY_IN
	//m_vecpThreadMgr.push_back( SiKFunizenAuthManager()->GetInstance() ); 나중에 쓰일 수 있어서 주석처리만 해놓음
#endif // SERV_COUNTRY
#endif // SERV_GLOBAL_AUTH
	//}}

	//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	KAutoPath kAutoPath;
	std::string	strFile;
	bool bRVal = true;

	{
		LoadingTimer lt( L"Enum.lua" );

		//추후 ↓에서 이루어 지는 데이터 로딩이 실패할경우 서버를 종료 시켜야한다.
		strFile = "Enum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"Enum 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			KBaseServer::GetKObj()->AddFailScriptFileName( L"Enum.lua" );
		}
		else
		{
			START_LOG( cout, L"Enum 정보 로드 성공.!" );
		}
	}

	{
		LoadingTimer lt( L"DungeonEnum.lua" );

		//추후 ↓에서 이루어 지는 데이터 로딩이 실패할경우 서버를 종료 시켜야한다.
		strFile = "DungeonEnum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"DungeonEnum 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			KBaseServer::GetKObj()->AddFailScriptFileName( L"DungeonEnum.lua" );
		}
		else
		{
			START_LOG( cout, L"DungeonEnum 정보 로드 성공.!" );
		}
	}

	{
		KGameSysVal::RegScriptName( "GameSysValTable.lua" );
		OPEN_SCRIPT_FILE( KGameSysVal );
	}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}

	//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	{
		KWaitQueueManager::RegScriptName( "WaitQueueData.lua" );
		OPEN_SCRIPT_FILE( KWaitQueueManager );
	}
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	{
		KSMSPhoneNumberManager::RegScriptName( "SMSPhoneNumberList.lua" );
		OPEN_SCRIPT_FILE( KSMSPhoneNumberManager );
	}
#endif SERV_SMS_TOTAL_MANAGER
	//}}

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager();
#endif SERV_HACKING_TOOL_LIST
	//}}

    KSimLayer::Init();

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	SiKProxyManager()->SetProxyBuilder( CreateProxy );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_COUNTRY_PH
	m_bOpensslRSAInit = false;
	if ( m_OpensslRSAChannel.Init() == false) // 2013.07.31 lygan_조성욱 // 동남아시아 인증 암호화된걸 복호화 하기 위해
	{
		START_LOG( cerr, L"동남아시아 RSA 인증 초기화 실패" );
	}
	else
	{
		m_bOpensslRSAInit = true;
	}
#endif //SERV_COUNTRY_PH
}

void KChannelSimLayer::Tick()
{
    KSimLayer::Tick();

	//{{ 2013.03.21 조효진 국가별 AuthManager 다름
#ifdef SERV_GLOBAL_AUTH
#if defined SERV_COUNTRY_TWHK
	if (GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE)
		SiKGASHAuthManager()->Tick();
#endif // SERV_COUNTRY_TWXX
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	SiKProxyManager()->Tick();
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager()->Tick();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	SiKWaitQueueManager()->Tick();
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	SiKDDOSGuardSimulator()->Tick();
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//}}
}

void KChannelSimLayer::ShutDown()
{
    KSimLayer::ShutDown();

	//{{ 2013.03.21 조효진 국가별 AuthManager 다름
#ifdef SERV_GLOBAL_AUTH
#if defined SERV_COUNTRY_EU
	SiKGameForgeAuthManager()->ReleaseInstance();
#elif defined SERV_COUNTRY_US
	SiKK3RComboAuthManager()->ReleaseInstance();
#elif defined SERV_COUNTRY_TWHK
	SiKGASHAuthManager()->ReleaseInstance();
#elif defined SERV_COUNTRY_JP
	SiKPurpleAuthManager()->ReleaseInstance();
#elif defined SERV_COUNTRY_BR
	SiKLevelUpAuthManager()->ReleaseInstance();
#endif // SERV_COUNTRY
#endif // SERV_GLOBAL_AUTH
	//}}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	KProxyManager::ReleaseInstance();
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	KHackingProtectionManager::ReleaseInstance();
#endif SERV_HACKING_TOOL_LIST
	//}}
	//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	KWaitQueueManager::ReleaseInstance();
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}
	//{{ 2012. 11. 22	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	KSMSPhoneNumberManager::ReleaseInstance();
#endif SERV_SMS_TOTAL_MANAGER
	//}}
}

//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
void KChannelSimLayer::SetCheckIPMode_LUA( int iVal )
{
	m_eCheckIPMode = static_cast<CHECK_IP_MODE>(iVal);

	switch( m_eCheckIPMode )
	{
	case CIM_SERVICE_MODE:
		{
			START_LOG( cout, L"------------ 채널 서버 일반 모드! ------------" );
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
			SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"채널 서버 일반 모드!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
		}
		break;

	case CIM_CHECK_MODE:
		{
			START_LOG( cout, L"------------ 채널 서버 점검 모드! ------------" );
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
			SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"채널 서버 점검 모드!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
		}
		break;

	default:
		START_LOG( cerr, L"잘못된 채널 서버 모드 세팅입니다!" )
			<< BUILD_LOG( iVal )
			<< END_LOG;

		// 디폴트로 서비스 모드 적용
		m_eCheckIPMode = CIM_SERVICE_MODE;
		break;
	}
}
#endif SERV_CHANNEL_SERV_CHECK_MODE
//}}

void KChannelSimLayer::AddIP( const char* szAddIP )
{
	if( szAddIP == NULL )
	{
		START_LOG( cerr, L"IP 스트링 이상." )
			<< END_LOG;

		return;
	}

	std::string strIP = szAddIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"추가하려는 IP 이상." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return;
	}

	START_LOG( cout, L"=== ADD IP ===" )
		<< BUILD_LOG( strIP.c_str() )
		<< BUILD_LOG( strIP.size() )
		<< L"================"
		<< dbg::endl;

	//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
	m_kConnectIpCheck[CIM_SERVICE_MODE].m_vecIP.push_back( strIP );
#else
	m_vecIP.push_back( strIP );
#endif SERV_CHANNEL_SERV_CHECK_MODE
	//}}	
}

void KChannelSimLayer::SetCheckIPType( int iVal )
{
	switch( iVal )
	{
	case CIT_ALL_IP_ALLOW:
		START_LOG( cout, L"모든 IP 허용!" );
		break;

	case CIT_CHECK_IP_ALLOW:
		START_LOG( cout, L"특정 IP만 허용!" );
		break;

	case CIT_CHECK_IP_BLOCK:
		START_LOG( cout, L"특정 IP만 차단!" );
		break;

	default:
		START_LOG( cerr, L"정의 되지 않은 IP허용 타입 입니다!" )
			<< BUILD_LOG( iVal )
			<< END_LOG;
		return;
	}

	//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
	m_kConnectIpCheck[CIM_SERVICE_MODE].m_eCheckIPType = static_cast<CHECK_IP_TYPE>(iVal);
#else
	m_eCheckIPType = static_cast<CHECK_IP_TYPE>(iVal);
#endif SERV_CHANNEL_SERV_CHECK_MODE
	//}}	
}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 08. 24	최육사	채널 서버 점검 모드
#ifdef SERV_CHANNEL_SERV_CHECK_MODE
void KChannelSimLayer::AddCheckModeIP( const char* szAddIP )
{
	if( szAddIP == NULL )
	{
		START_LOG( cerr, L"[점검 모드] IP 스트링 이상." )
			<< END_LOG;

		return;
	}

	std::string strIP = szAddIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"[점검 모드] 추가하려는 IP 이상." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return;
	}

	START_LOG( cout, L"=== [점검 모드] ADD IP ===" )
		<< BUILD_LOG( strIP.c_str() )
		<< BUILD_LOG( strIP.size() )
		<< L"================"
		<< dbg::endl;

	m_kConnectIpCheck[CIM_CHECK_MODE].m_vecIP.push_back( strIP );
}

void KChannelSimLayer::SetCheckModeCheckIPType( int iVal )
{
	switch( iVal )
	{
	case CIT_ALL_IP_ALLOW:
		START_LOG( cout, L"[점검 모드] 모든 IP 허용!" );
		break;

	case CIT_CHECK_IP_ALLOW:
		START_LOG( cout, L"[점검 모드] 특정 IP만 허용!" );
		break;

	case CIT_CHECK_IP_BLOCK:
		START_LOG( cout, L"[점검 모드] 특정 IP만 차단!" );
		break;

	default:
		START_LOG( cerr, L"[점검 모드] 정의 되지 않은 IP허용 타입 입니다!" )
			<< BUILD_LOG( iVal )
			<< END_LOG;
		return;
	}

	m_kConnectIpCheck[CIM_CHECK_MODE].m_eCheckIPType = static_cast<CHECK_IP_TYPE>(iVal);
}

bool KChannelSimLayer::CheckIP( const char* szIP )
{
    return CheckIP( m_eCheckIPMode, szIP );
}

bool KChannelSimLayer::CheckIP( IN const CHECK_IP_MODE eCheckIpMode, IN const char* szIP )
{
	switch( eCheckIpMode )
	{
	case CIM_SERVICE_MODE:
	case CIM_CHECK_MODE:
		break;

	default:
		START_LOG( cerr, L"이상한 체크 모드입니다!" )
			<< BUILD_LOG( eCheckIpMode )
			<< END_LOG;

		// 일단은 통과 시키자!
		return true;
	}

	// 모든 IP허용이면 통과!
	if( m_kConnectIpCheck[eCheckIpMode].m_eCheckIPType == CIT_ALL_IP_ALLOW )
		return true;

	// 체크 IP리스트가 비어있으면 통과!
	if( m_kConnectIpCheck[eCheckIpMode].m_vecIP.empty() )
		return true;

	if( szIP == NULL )
	{
		START_LOG( cerr, L"비교하려는 IP 이상." )
			<< END_LOG;

		return false;
	}

	std::string strIP = szIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"비교하려는 IP 길이 이상." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return false;
	}

	// 특정 IP허용이라면..?
	if( m_kConnectIpCheck[eCheckIpMode].m_eCheckIPType == CIT_CHECK_IP_ALLOW )
	{
		std::vector< std::string >::iterator vit;
		for( vit = m_kConnectIpCheck[eCheckIpMode].m_vecIP.begin(); vit != m_kConnectIpCheck[eCheckIpMode].m_vecIP.end(); ++vit )
		{
			if( vit->size() < 1 || vit->size() > 15 )
			{
				START_LOG( cerr, L"비교하려는 IP 이상." )
					<< BUILD_LOG( vit->size() )
					<< END_LOG;

				return false;
			}

			// 해당 IP 대역이라면 허용!
			if( vit->compare( 0, vit->size(), strIP, 0, vit->size() ) == 0 )
			{
				return true;
			}
		}

		return false;
	}
	// 특정 IP차단이라면..?
	else if( m_kConnectIpCheck[eCheckIpMode].m_eCheckIPType == CIT_CHECK_IP_BLOCK )
	{
		std::vector< std::string >::iterator vit;
		for( vit = m_kConnectIpCheck[eCheckIpMode].m_vecIP.begin(); vit != m_kConnectIpCheck[eCheckIpMode].m_vecIP.end(); ++vit )
		{
			if( vit->size() < 1 || vit->size() > 15 )
			{
				START_LOG( cerr, L"비교하려는 IP 이상." )
					<< BUILD_LOG( vit->size() )
					<< END_LOG;

				return false;
			}

			// 해당 IP 대역이라면 차단!
			if( vit->compare( 0, vit->size(), strIP, 0, vit->size() ) == 0 )
			{
				return false;
			}
		}

		return true;
	}

	START_LOG( cerr, L"잘못된 IP 체크 타입 입니다." )
		<< BUILD_LOG( m_kConnectIpCheck[eCheckIpMode].m_eCheckIPType )
		<< END_LOG;

	return false;
}
#else
bool KChannelSimLayer::CheckIP( const char* szIP )
{
	// 모든 IP허용이면 통과!
	if( m_eCheckIPType == CIT_ALL_IP_ALLOW )
		return true;

	// 체크 IP리스트가 비어있으면 통과!
	if( m_vecIP.empty() )
		return true;

	if( szIP == NULL )
	{
		START_LOG( cerr, L"비교하려는 IP 이상." )
			<< END_LOG;

		return false;
	}

	std::string strIP = szIP;
	if( strIP.size() < 1 || strIP.size() > 15 )
	{
		START_LOG( cerr, L"비교하려는 IP 길이 이상." )
			<< BUILD_LOG( strIP.size() )
			<< END_LOG;

		return false;
	}

	// 특정 IP허용이라면..?
	if( m_eCheckIPType == CIT_CHECK_IP_ALLOW )
	{
		std::vector< std::string >::iterator vit;
		for( vit = m_vecIP.begin(); vit != m_vecIP.end(); ++vit )
		{
			if( vit->size() < 1 || vit->size() > 15 )
			{
				START_LOG( cerr, L"비교하려는 IP 이상." )
					<< BUILD_LOG( vit->size() )
					<< END_LOG;

				return false;
			}

			// 해당 IP 대역이라면 허용!
			if( vit->compare( 0, vit->size(), strIP, 0, vit->size() ) == 0 )
			{
				return true;
			}
		}

		return false;
	}
	// 특정 IP차단이라면..?
	else if( m_eCheckIPType == CIT_CHECK_IP_BLOCK )
	{
		std::vector< std::string >::iterator vit;
		for( vit = m_vecIP.begin(); vit != m_vecIP.end(); ++vit )
		{
			if( vit->size() < 1 || vit->size() > 15 )
			{
				START_LOG( cerr, L"비교하려는 IP 이상." )
					<< BUILD_LOG( vit->size() )
					<< END_LOG;

				return false;
			}

			// 해당 IP 대역이라면 차단!
			if( vit->compare( 0, vit->size(), strIP, 0, vit->size() ) == 0 )
			{
				return false;
			}
		}

		return true;
	}

	START_LOG( cerr, L"잘못된 IP 체크 타입 입니다." )
		<< BUILD_LOG( m_eCheckIPType )
		<< END_LOG;

	return false;
}
#endif SERV_CHANNEL_SERV_CHECK_MODE
//}}