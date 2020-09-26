#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include <boost/bind.hpp>
#include "LoginServer.h"
#include "ActorManager.h"
#include "LoginSimLayer.h"
#include "LoginNetLayer.h"
#include "socket/NetCommon.h"
//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	#include "LoginRoomManager.h"
#endif SERV_PSHOP_AGENCY
//}}
#include "DBLayer.h"
#include "LoginAccountDBThread.h"
//{{ 2009. 11. 9  최육사	로그인서버GameDB
#include "LoginGameDBThread.h"
//}}
#include "LoginLogDBThread.h"
//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	#include "LoginSMSDBThread.h"
#endif SERV_MORNITORING
//}}
//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	#include "LoginNXWebDBThread.h"
#endif SERV_WEB_POINT_EVENT
//}}
#ifdef SERV_GLOBAL_BILLING
#include "LoginBillingDBThread.h"
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_COUNTRY_CN
#include <boost/foreach.hpp>
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantRoleRegManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantInfoManager.h"
#endif SERV_COUNTRY_CN

#include "defaultFSM.h"
#include "LoginUser.h"
//{{ 2009. 7. 6  최육사		랭킹개편
#include "RankingManager.h"
//}}
//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_TEST
	#include "GuildManager.h"
#endif GUILD_TEST
//}}
//{{ 2010. 01. 15  최육사	길드게시판
#ifdef SERV_GUILD_AD
	#include "GuildBoardManager.h"
#endif SERV_GUILD_AD
//}}
//{{ 2010. 06. 01  최육사	동접 처리 개편
#ifdef SERV_CCU_NEW
	#include "ServerCCUManager.h"
#endif SERV_CCU_NEW
//}}
//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
	#include "Nexon/AuthCheck.h"
#endif SERV_OTP_AUTH
//}}

//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
	#include "AccountBlockManager.h"
#endif SERV_ACCOUNT_BLOCK
//}}
//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
	#include "LoginScriptRefreshManager.h"
#endif SERV_REALTIME_SCRIPT
//}}

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
	#include "DLLManager.h"
#endif SERV_DLL_MANAGER
//}}
//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	#include "RandomItemEventManager.h"
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}
//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}

#include "X2Data/XSLGuildManager.h"
//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	#include "X2Data/XSLTitleManager.h"
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

//{{ 2012. 10. 9	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}
//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}
//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	#include "LoginLocalRankingManager.h"
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHallManager.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
#include "X2Data/XSLSkillTree.h"
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#include "NetError.h"

//{{ 최육사 : [2012/10/9] //	태국 OTP 인증 통신 모듈
#ifdef SERV_AUTH_TCP_THREAD_MANAGER
#include "../Common/OnlyGlobal/AuthAndBilling/TH/LoginOtpAuthTcpThread.h"
#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}
#ifdef SERV_COUNTRY_JP
#include "../Common/OnlyGlobal/AuthAndBilling/JP/HanNetCafeForSvr.h"
#endif //SERV_COUNTRY_JP

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2011. 03. 29	최육사	TBB 메모리 관리자
//#ifdef SERV_TBB_MALLOC_PROXY_TEST
//	#include <tbb/tbbmalloc_proxy.h>
//#endif SERV_TBB_MALLOC_PROXY_TEST
//}}

//#include "vld.h"

#ifdef SERV_COUNTRY_PH
#include "../Common/OnlyGlobal/AuthAndBilling/PH/GarenaBillingServer.h"
#endif //SERV_COUNTRY_PH

NiImplementRTTI( KLoginServer, KBaseServer );
ImplPfID( KLoginServer, PI_LOGIN_SERVER );

#define CLASS_TYPE KLoginServer

ImplToStringW( KLoginServer )
{
    return START_TOSTRING_PARENTW( KBaseServer );
}

KLoginServer::KLoginServer(void)
{
}

KLoginServer::~KLoginServer(void)
{
}

#include <lua_tinker.h>

void KLoginServer::RegToLua()
{
    KBaseServer::RegToLua();

    //lua_tinker::class_<KLoginServer>( "KLoginServer" )
    //    .inh<KBaseServer>()
    //    .def( "dump", KLoginServer::Dump );

    //lua_tinker::decl( "Server", this );
	lua_tinker::class_add<KLoginServer>( g_pLua, "KLoginServer" );
	lua_tinker::class_inh<KLoginServer, KBaseServer>( g_pLua );
	//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
	lua_tinker::class_def<KLoginServer>( g_pLua, "SetPassportLevel",	&KLoginServer::SetPassportLevel_LUA );
#endif SERV_OTP_AUTH
	//}}
	lua_tinker::class_def<KLoginServer>( g_pLua, "dump",				&KLoginServer::Dump );
	//{{ 2010. 10. 19	최육사	SMS테스트
#ifdef SERV_SMS_TEST
	lua_tinker::class_def<KLoginServer>( g_pLua, "SMSTest",				&KLoginServer::SMSTest_LUA );
#endif SERV_SMS_TEST
	//}}

#ifdef SERV_COUNTRY_PH
	SiKGarenaBillingServer()->RegToLua();
#endif //SERV_COUNTRY_PH

	lua_tinker::decl( g_pLua, "Server", this );
}

const wchar_t*  g_szConfigFile = L"config_lg";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KLoginServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
    switch( iDBConnectionInfo )
    {
    case KDBLayer::DC_ACCOUNT:
        return new KLoginAccountDBThread( szDSN, bDBConnStr );
		//{{ 2009. 11. 9  최육사	로그인서버GameDB
	case KDBLayer::DC_GAME:
		return new KLoginGameDBThread( szDSN, bDBConnStr );
		//}}
    case KDBLayer::DC_LOG:
        return new KLoginLogDBThread( szDSN, bDBConnStr );
		//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	case KDBLayer::DC_SMS:
		return new KLoginSMSDBThread( szDSN, bDBConnStr );
#endif SERV_MORNITORING
		//}}
		//{{ 2010. 02. 25  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	case KDBLayer::DC_NX_WEB:
		return new KLoginNXWebDBThread( szDSN, bDBConnStr );
#endif SERV_WEB_POINT_EVENT
		//}}
#ifdef SERV_GLOBAL_BILLING
	case KDBLayer::DC_KOG_BILLING:
		return new KLoginBillingDBThread( szDSN, bDBConnStr );
#endif // SERV_GLOBAL_BILLING
    default:
        START_LOG( cerr, L"접속하려는 DB 종류가 이상함." )
            << BUILD_LOG( iDBConnectionInfo )
            << END_LOG;
        return new KLoginAccountDBThread( szDSN, bDBConnStr );
    }
}

//{{ 최육사 : [2012/10/9] //	태국 OTP 인증 통신 모듈
#ifdef SERV_AUTH_TCP_THREAD_MANAGER
KThread*        CreateTCPThread( const std::wstring& wstrIP, const short sPort )
{
	return new KLoginOtpAuthTcpThread( wstrIP, sPort );
}
#endif SERV_AUTH_TCP_THREAD_MANAGER
//}}

bool KLoginServer::Init()
{
    KSimLayer::KeepObject<KLoginSimLayer>();
    KNetLayer::KeepObject<KLoginNetLayer>();
    KActorManager::KeepObject<KActorManager>();
	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	KRoomManager::KeepObject<KLoginRoomManager>();
#endif SERV_PSHOP_AGENCY
	//}}

    KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KLoginUser, KDefaultFSM, 512*1024> ) );
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_SERVER );
#endif SERV_KEVENT_FROM
	//}}

    _JIF( KBaseServer::Init(), return false );

    //WriteServerInfoToDB();

#ifdef SERV_COUNTRY_JP
	// 2010. 05. 27  김정협 넷카페 모듈 초기화 추가
	int ret = -1;
	ret = HanNetCafe_Init();
	if(ret == HAN_NETCAFE_OK)
	{
		START_LOG( clog, L"넷카페 모듈 초기화 완료" )				
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"넷카페 모듈 초기화 에러" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}
#endif //SERV_COUNTRY_JP

	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	_JIF( GetKLoginRoomManager()->Init(), return false );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 최육사 : [2012/10/9] //	태국 OTP 인증 통신 모듈
#ifdef SERV_AUTH_TCP_THREAD_MANAGER
	//m_kTHOTPThread.SetTCPConnectionInfo( L"5465165165165", 4654 );	//	나중에 스크립트로 뺄것!
	m_kTHOTPThread.Init( 50 ); // 나중에 스크립트로 뺄것!
	m_kTHOTPThread.BeginThread();
#endif SERV_AUTH_TCP_THREAD_MANAGER
	//}}

#ifdef SERV_COUNTRY_PH
	_JIF( SiKGarenaBillingServer()->Init(), return false );
#endif //SERV_COUNTRY_PH

	SetServerReady( true );

	//{{ 2009. 8. 27  최육사	server ready
	OnServerReadyComplete();
	//}}
	
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    return true;
}

void KLoginServer::OnServerReadyComplete()
{
	KBaseServer::OnServerReadyComplete();

	//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
	// Passport Level
	AuthCheck_SetPassportLevel( m_uiPassportLevel );

	START_LOG( cout2, L"Nexon AuthCheck SetPassportLevel : " << m_uiPassportLevel );
	
	// Locale
	AuthCheck_SetLocale( kLocaleID_KR );

	START_LOG( cout2, L"Nexon AuthCheck SetLocale : kLocaleID_KR" );
#endif SERV_OTP_AUTH
	//}}

	//{{ 2009. 7. 6  최육사		랭킹 개편
	SiKRankingManager()->Init();
	//}}

	//{{ 2010. 01. 15  최육사	길드게시판
#ifdef SERV_GUILD_AD
	SiKGuildBoardManager()->Init();
#endif SERV_GUILD_AD
	//}}

	//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	SiKRandomItemEventManager()->Init();
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}

	//{{ 2010. 06. 01  최육사	동접 처리 개편
#ifdef SERV_CCU_NEW
	// 서버 켜자마자 현시간부 동접 정보 업데이트
	SiKServerCCUManager()->DBUpdateCCUInfo();
#endif SERV_CCU_NEW
	//}}

	//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
	SiKAccountBlockManager()->Init();
#endif SERV_ACCOUNT_BLOCK
	//}}

	//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
	SiKLoginScriptRefreshManager()->Init();	
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
	SiKDLLManager()->OnServerReadyComplete();
#endif SERV_DLL_MANAGER
	//}}

	//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	SendToGameDB( DBE_LOAD_PSHOP_AGENCY_REQ, char() );
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKLoginLocalRankingManager()->Begin();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}
	
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\LoginServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\LoginServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS
	
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	SiKWeddingHallManager()->Init();
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	// 서버군 이름 표시
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );
}

void KLoginServer::ShutDown()
{
#ifdef SERV_COUNTRY_JP
	// 2010. 05. 27  김정협 넷카페 모듈 초기화 추가
	int ret = -1;
	ret = HanNetCafe_Cleanup();
	if(ret == HAN_NETCAFE_OK)
	{
		START_LOG( clog, L"넷카페 모듈 릴리즈 완료" )				
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"넷카페 모듈 릴리즈 에러" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}	
#endif //SERV_COUNTRY_JP

	//{{ 2013. 3. 15	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKLoginLocalRankingManager()->ShutDown();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	//{{ 2009. 7. 6  최육사		랭킹 개편 -	DBLayer가 죽기전에 랭킹정보를 업데이트 해야한다.
    SiKRankingManager()->ShutDown();
	//}}

	//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
	SiKDLLManager()->Shutdown();
#endif SERV_DLL_MANAGER
	//}}

	//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT	
	SiKLoginScriptRefreshManager()->EndThread();
	KLoginScriptRefreshManager::ReleaseInstance();
#endif SERV_REALTIME_SCRIPT
	//}}

#ifdef SERV_COUNTRY_PH
	SiKGarenaBillingServer()->ShutDown();
	KGarenaBillingServer::ReleaseInstance();
#endif SERV_COUNTRY_PH

    // 아직 아무 종료처리도 진행되기 전.
    KBaseServer::ShutDown();        // 이곳에서 모든 종료가 끝난다. 각각의 Layer들도 모두 소멸됨.

	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	GetKLoginRoomManager()->ShutDown();
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	SiKWeddingHallManager()->ShutDown();
#endif SERV_RELATIONSHIP_SYSTEM
	//}
}

bool KLoginServer::DestroyAllSession()
{
    // NetLayer의 Accepter만 닫힌 시점. 아직 모든 Layer들이 살아있다.
 
    KActorManager* pkActorManager = KActorManager::GetKObj();
   
    START_LOG( cout, L"현재 유저 : " << pkActorManager->GetCount() << L" 명" );

	//{{ 2009. 1. 30  최육사	서버 셧다운 종료 사유 설정
	int iIndex = KStatistics::eSIColDR_Server_Shutdown;

	pkActorManager->ForEach( boost::bind( &KActor::SetDisconnectReason, _1, iIndex ) );
	//}}

    pkActorManager->ForEach( boost::bind( &KActor::ReserveDestroy, _1 ) );

    DWORD dwBeginTick = ::GetTickCount();

    // 안정적인 종료 조건 :
    // 1. 모든 유저 객체가 소멸되어야 하고 
    // 2. 모든 db event가 처리 fetch 되어야 한다. (처리중일수 있음)
    bool bSuccess = pkActorManager->GetCount() == 0 &&
        SiKDBLayer()->GetTotalEventNum() == 0;

    while( !bSuccess && ::GetTickCount() - dwBeginTick < m_dwShutDownWait )   // 하지만 일정 시간이 지나면 그냥 통과.
    {
        ::Sleep( 500 );

        bSuccess = pkActorManager->GetCount() == 0 &&
            SiKDBLayer()->GetTotalEventNum() == 0;
    }

    LOG_CONDITION( bSuccess, cout, cerr )
        << BUILD_LOG( m_dwShutDownWait )
        << BUILD_LOG( pkActorManager->GetCount() )
        << BUILD_LOG( SiKDBLayer()->GetTotalEventNum() )
        << BUILD_LOG( ::GetTickCount() - dwBeginTick );

    return bSuccess;
}

void KLoginServer::Tick()
{
    KBaseServer::Tick();

	//{{ 2008. 5. 27  최육사  체험ID 동접
	//{{ 2010. 06. 01  최육사	동접 처리 개편
//#ifdef SERV_CCU_NEW
//	if( ::GetTickCount() - m_dwServerInfoUpdateTick > 20 * 1000 )
//#else
//	if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwServerInfoUpdateGap )
//#endif SERV_CCU_NEW
//	//}}    
//    {
//        if( GetServerReady() )
//        {
//            WriteServerInfoToDB();
//        }
//    }
	//}}

	//{{ 2010. 06. 01  최육사	동접 처리 개편
#ifdef SERV_CCU_NEW
	if( GetServerReady() )
	{
		SiKServerCCUManager()->Tick();
	}
#endif SERV_CCU_NEW
	//}}

	//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
	SiKAccountBlockManager()->Tick();
#endif SERV_ACCOUNT_BLOCK
	//}}

#ifdef SERV_COUNTRY_PH
	SiKGarenaBillingServer()->Tick();
#endif SERV_COUNTRY_PH

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\LoginServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\LoginServerWrite"));
			SiKProcessCommunicationManager()->Begin();
			m_bServerRunningProcessCommunicationOnOff = false;


		}
		else if (SiKGameSysVal()->GetProcessCommunication() == false && m_bServerRunningProcessCommunicationOnOff == false)
		{
			SiKProcessCommunicationManager()->ShutDown();

			m_bServerRunningProcessCommunicationOnOff = true;

		}
		m_tTimeProcessCommunicationONOFF.restart();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

//{{ 2008. 5. 27  최육사  체험ID 동접
void KLoginServer::WriteServerInfoToDB()
{
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
	{
		//{{ 2010. 06. 01  최육사	동접 처리 개편
#ifdef SERV_CCU_NEW
		KUpdateCCUInfo kInfo;
		kInfo.m_iServerUID			= GetUID();
		kInfo.m_iConcurrentUser		= GetKLoginSimLayer()->GetGuestConcurrentUserCount(); // 체험ID 동접
		kInfo.m_iCurPartyCount		= 0;
		kInfo.m_iPlayGamePartyCount = 0;
		SiKServerCCUManager()->UpdateCCUInfo( kInfo );
#else
		CTime tCurrentTime = CTime::GetCurrentTime();
		KDBE_UPDATE_SERVER_INFO_REQ kPacket;
		kPacket.m_wstrRegDate		  = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		kPacket.m_iServerUID		  = GetUID();
		kPacket.m_iConcurrentUser	  = GetKLoginSimLayer()->GetGuestConcurrentUserCount(); // 체험ID 동접
		kPacket.m_iCurPartyCount	  = 0;
		kPacket.m_iPlayGamePartyCount = 0;
		SendToLogDB( DBE_UPDATE_SERVER_INFO_REQ, kPacket );
#endif SERV_CCU_NEW
		//}}		
	}

	m_dwServerInfoUpdateTick = ::GetTickCount();
}
//}}

//{{ 2010. 10. 19	최육사	SMS테스트
#ifdef SERV_SMS_TEST
void KLoginServer::SMSTest_LUA()
{
	// 문자 전송!
	KDBE_SEND_PHONE_MSG_NOT kPacketNot;
	//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_TEST, kPacketNot.m_vecPhoneNum );
#else
	kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8421-1075" ) );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	kPacketNot.m_wstrSMSMessage = L"SMS 테스트 문자 전송!";
	SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );

	START_LOG( cout, L"SMS 테스트 문자 전송!" );
}
#endif SERV_SMS_TEST
//}}

void KLoginServer::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_ACK, KDBE_LOGIN_SERVER_ON_ACK );
		CASE_NOPARAM( DBE_UPDATE_SERVER_INFO_ACK );
#ifdef SERV_GLOBAL_AUTH 
		CASE( EPUBLISHER_REG_USER_ACK );
		CASE( EPUBLISHER_UNREG_USER_ACK );
#ifdef SERV_COUNTRY_CN
		CASE( EGIANT_INFO_GET_CCU_REQ );
#endif SERV_COUNTRY_CN
#endif // SERV_GLOBAL_AUTH

// 현재 사용하지 않는 코드로 보임
// #ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
// 		CASE( DBE_GET_GIANT_DELETED_UNIT_LIST_ACK );
// #endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
		CASE( ENX_AUTH_LOGIN_ACK );
		CASE( ENX_AUTH_TERMINATE_NOT );
		CASE( ENX_AUTH_MESSAGE_NOT );
		CASE( ENX_AUTH_SYNCHRONIZE_REQ );
#else
		CASE( ENX_COMMON_ACK_NOT );
#endif SERV_PCBANG_AUTH_NEW
		//}}
		//{{ 2009. 7. 6  최육사		랭킹 개편
		CASE( DBE_GET_HENIR_RANKING_INFO_ACK );
	   _CASE( ELG_CLEAR_RANKING_NOT, bool );
		CASE_NOPARAM( DMP_DUMP_RANKING_MANAGER_NOT );
		CASE( ELG_ADMIN_NEW_RECORD_TEST_NOT );
		CASE( ELG_ADMIN_NEW_RECORD_NOT );
		CASE( ELG_ADMIN_DELETE_RECORD_NOT );
		//}}
		//{{ 2009. 11. 9  최육사	로그인서버GameDB
		CASE( DBE_GET_WEB_RANKING_INFO_ACK );
		//}}

		//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_TEST
		CASE( DBE_GUILD_LEVEL_UP_ACK );
#endif GUILD_TEST
		//}}

		//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		CASE( DBE_INSERT_GUILD_CASH_SKILL_POINT_ACK );
		CASE( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK );

	   _CASE( DBE_ADMIN_GET_GUILD_SKILL_POINT_ACK, KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK );
#endif GUILD_SKILL_TEST	
		//}}

		//{{ 2010. 01. 15  최육사	길드게시판
#ifdef SERV_GUILD_AD
		CASE( DBE_GET_GUILD_BOARD_INFO_ACK );
		CASE( DBE_ACCEPT_JOIN_GUILD_ACK );
#endif SERV_GUILD_AD
		//}}

		//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT	
		CASE( DBE_GET_WEB_POINT_REWARD_ACK );
	   _CASE( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_ACK, KDBE_GET_WEB_POINT_REWARD_ACK );
#endif SERV_WEB_POINT_EVENT
		//}}

		//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
		CASE( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK );
#endif SERV_ACCOUNT_BLOCK
		//}}

		//{{ 2010. 07. 19  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
	   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_REALTIME_SCRIPT
		//}}
		//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
		CASE( DBE_GET_MODULE_INFO_LIST_ACK );
	   _CASE( DBE_UPDATE_MODULE_INFO_LIST_ACK, KDBE_GET_MODULE_INFO_LIST_ACK );
#endif SERV_DLL_MANAGER
		//}}
	   //{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	   _CASE( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
	   //}}
	   //{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	   CASE( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	   //}}
	   //{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	   CASE( DBE_INSERT_REWARD_TO_POST_ACK );
	   CASE( DBE_LOAD_PSHOP_AGENCY_ACK );
	  _CASE( DBE_BREAK_PSHOP_AGENCY_ACK, KERM_BREAK_PSHOP_AGENCY_NOT );
	  _CASE( ERM_PSHOP_AGENCY_MESSAGE_NOT, KEGS_PSHOP_AGENCY_MESSAGE_NOT );
#endif SERV_PSHOP_AGENCY
	   //}}
		//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
		CASE( ELG_RANKING_TITLE_REWARD_NOT );
		CASE( DBE_INSERT_TITLE_ACK );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
		//}}
		//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
		CASE( DBE_GET_HACKING_MODULE_LIST_ACK );
#endif SERV_HACKING_DLL_CHECK
		//}}
		//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK, SiKLoginLocalRankingManager() );
		CASE( DBE_LOCAL_RANKING_RESET_ACK );
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_SYSTEM_INFO_CHECK_NOT, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( DBE_LOCAL_RANKING_RESET_CHECK_ACK, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_RESET_NOT, SiKLoginLocalRankingManager() );
		CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT, SiKLoginLocalRankingManager() );
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}

		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		CASE( DBE_LOAD_WEDDING_HALL_INFO_ACK );
		_CASE( ELG_ADD_WEDDING_HALL_INFO_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS// 작업날짜: 2013-10-28	// 박세훈
		CASE( E_DISCONNECT_SERVER_REPORT_NOT );
#endif // SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }
}

_IMPL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_LOGIN_SERVER_ON_ACK )
{
	//스타트에 실패했으면 프로그램을 끈다
	if( kPacket_.m_iOK != NetError::NET_OK)
	{
		::exit( 0 );
		return;
	}

	SetServerInfo( kPacket_.m_kServerInfo );

    SetEvent( m_spEventAck.get() );
}

IMPL_ON_FUNC_NOPARAM( DBE_UPDATE_SERVER_INFO_ACK )
{
	// 기능 없음.
}

//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW

IMPL_ON_FUNC( ENX_AUTH_LOGIN_ACK )
{
	START_LOG( cerr, L"넥슨 PC방 인증 서버 : 해외는 여기 로그인 타면 안됨!!!!!!" )
		<< BUILD_LOG( kPacket_.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_iSessionNo )
		<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
		<< BUILD_LOG( kPacket_.m_mapProperty.size() );

	// 인증 서버로부터 받은 SessionNo를 유저정보에 업데이트	
// 	if( GetKLoginSimLayer()->UpdateSessionNoByUserID( kPacket_.m_wstrUserID, kPacket_.m_iSessionNo ) == false )
// 	{
// 		START_LOG( cerr, L"SessionNo 업데이트 실패." )
// 			<< BUILD_LOG( kPacket_.m_wstrUserID )
// 			<< BUILD_LOG( kPacket_.m_bytePacketType )
// 			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
// 			<< BUILD_LOG( kPacket_.m_iSessionNo )
// 			<< END_LOG;
// 		return;
// 	}
// 
// 	KUserList::KGSUserInfo kInfo;
// 	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) == false )
// 	{
// 		START_LOG( cerr, L"유저 게임 서버 검색 실패." )
// 			<< BUILD_LOG( kPacket_.m_wstrUserID )
// 			<< BUILD_LOG( kPacket_.m_bytePacketType )
// 			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
// 			<< END_LOG;
// 		return;
// 	}
// 
// 	//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
// #ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
// 	GetKLoginSimLayer()->UnRegAuthWaitUser( kInfo.m_nUserUID );
// #endif SERV_DUPLICATE_CONNECT_BUG_FIX
// 	//}}
// 	
// 	switch( kPacket_.m_byteAuthorizeResult )
// 	{
// 	case 0: // Forbidden - 비정상 접속으로 게임 실행을 차단한다.
// 	case 1: // Allowed - 프리미엄을 적용하고 게임을 실행한다.
// 	case 2: // Trial - 프리미엄 없이 게임을 실행한다.
// 	case 3: // Terminate - 게임 실행을 차단하고 인증서버로 로그아웃 패킷을 전송한다.
// 		break;
// 
// 	default:
// 		START_LOG( cerr, L"넥슨 인증 서버 로그인 결과값이 이상하다." )
// 			<< BUILD_LOG( kPacket_.m_wstrUserID )
// 			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
// 			<< END_LOG;
// 		return;
// 	}
// 
// 	//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편
// 	if( kInfo.m_bChannelChanged )
// 	{
// 		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
// 		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_CHANNEL_CHANGE_USER_ACK, kPacket_ );
// 	}
// 	else
// 	{
// 		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
// 		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_NEXON_USER_ACK, kPacket_ );
// 	}
// 	//}}

	// 2008.12.23.hoons.
	// 피씨방 오류로 인한 임시 로그
	//if( kPacket_.m_wstrUserID.compare(L"nateawooda") == 0 || kPacket_.m_wstrUserID.compare(L"demiahun") == 0 )
	//{
	//	SOCKADDR_IN sAddr;
	//	sAddr.sin_addr.S_un.S_addr = kInfo.m_uiIP;

	//	START_LOG( cout2, L"피씨방 임시로그.!(확인필요)" )
	//		<< BUILD_LOG( kPacket_.m_wstrUserID )
	//		<< BUILD_LOG( static_cast<int>(kPacket_.m_byteIsCharged) )
	//		<< BUILD_LOG( ::inet_ntoa(sAddr.sin_addr) )
	//		<< END_LOG;
	//}
}

IMPL_ON_FUNC( ENX_AUTH_TERMINATE_NOT )
{
	// 과금이 종료되어서 해당 유저를 접속 종료시키자!
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) == false )
	{
		START_LOG( cerr, L"유저 게임 서버 검색 실패." )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_bytePacketType )
			<< BUILD_LOG( kPacket_.m_byteTerminateType )
			<< END_LOG;
		return;
	}

	UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ENX_AUTH_TERMINATE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ENX_AUTH_MESSAGE_NOT )
{
    // 해당 유저에게 메시지 전송!
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) == false )
	{
		START_LOG( cerr, L"유저 게임 서버 검색 실패." )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_bytePacketType )
			<< BUILD_LOG( kPacket_.m_byteMessageType )
			<< END_LOG;
		return;
	}

	UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ENX_AUTH_MESSAGE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ENX_AUTH_SYNCHRONIZE_REQ )
{
	START_LOG( cout, L"넥슨 PC방 인증 서버 : 동기화 체크 패킷 도착!" )
		<< BUILD_LOG( kPacket_.m_byteIsMonitoring )
		<< BUILD_LOG( kPacket_.m_mapSynchronizeReq.size() );

	GetKLoginSimLayer()->CheckAuthSynchronize( kPacket_ );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( ENX_COMMON_ACK_NOT )
{
	KUserList::KGSUserInfo kInfo;

	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUserID, kInfo ) )
	{
		unsigned short usEventID;
		switch( kPacket_.m_byteResult )
		{
		case 0:
		case 1:
		case 2:
			usEventID = ELG_REGISTER_NEXON_USER_ACK;
			break;
		case 3:
			usEventID = ENX_USER_TERMINATE_NOT;
			break;
		case 4:
			usEventID = ENX_MESSAGE_NOT;
			break;
		default:
			START_LOG( cerr, L"넥슨 빌링 인증 응답 패킷 이상." )
				<< BUILD_LOG( kPacket_.m_wstrUserID )
				<< BUILD_LOG( kPacket_.m_bytePacketType )
				<< BUILD_LOG( kPacket_.m_byteResult )
				<< END_LOG;
			return;
		}

		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, usEventID, kPacket_ );

		// 2008.12.23.hoons.
		// 피씨방 오류로 인한 임시 로그
		if( kPacket_.m_wstrUserID.compare(L"nateawooda") == 0 || kPacket_.m_wstrUserID.compare(L"demiahun") == 0 )
		{
			SOCKADDR_IN sAddr;
			sAddr.sin_addr.S_un.S_addr = kInfo.m_uiIP;

			START_LOG( cout2, L"피씨방 임시로그.!(확인필요)" )
				<< BUILD_LOG( kPacket_.m_wstrUserID )
				<< BUILD_LOG( static_cast<int>(kPacket_.m_byteIsGameBangBillType) )
				<< BUILD_LOG( ::inet_ntoa(sAddr.sin_addr) )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"유저 게임 서버 검색 실패." )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_bytePacketType )
			<< BUILD_LOG( kPacket_.m_byteResult )
			<< END_LOG;
	}
}

#endif SERV_PCBANG_AUTH_NEW
//}}

//{{ 2009. 7. 6  최육사		랭킹 개편
IMPL_ON_FUNC( DBE_GET_HENIR_RANKING_INFO_ACK )
{
	// 헤니르의 시공 랭킹 업데이트
	SiKRankingManager()->InitHenirRanking( kPacket_.m_mapHenirRanking );
}

_IMPL_ON_FUNC( ELG_CLEAR_RANKING_NOT, bool )
{
	// 랭킹 초기화
	SiKRankingManager()->CheckRankingClearAndReward( true );

	START_LOG( cout, L"랭킹 수동 초기화 및 보상처리 완료!" );
}

IMPL_ON_FUNC_NOPARAM( DMP_DUMP_RANKING_MANAGER_NOT )
{
    SiKRankingManager()->DumpRankingInfo();
}

IMPL_ON_FUNC( ELG_ADMIN_NEW_RECORD_TEST_NOT )
{
	SiKRankingManager()->NewRecordForTest( kPacket_.m_vecNewRecordTest );
}

IMPL_ON_FUNC( ELG_ADMIN_NEW_RECORD_NOT )
{
	if( SiKRankingManager()->NewRecordHenirRanking( kPacket_.m_kNewRecordRanking, kPacket_.m_iRankingType ) )
	{
		START_LOG( cout, L"랭킹 기록 추가 성공!" );
	}
	else
	{
		START_LOG( cout, L"랭킹 기록 추가 실패!" );
	}
};

IMPL_ON_FUNC( ELG_ADMIN_DELETE_RECORD_NOT )
{	
	if( SiKRankingManager()->DeleteRecord( kPacket_.m_iRankingType, kPacket_.m_wstrNickName ) )
	{
        START_LOG( cout, L"랭킹 기록 삭제 성공!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_wstrNickName );
	}
	else
	{
		START_LOG( cout, L"랭킹 기록 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_wstrNickName );
	}
}
//}}

IMPL_ON_FUNC( DBE_GET_WEB_RANKING_INFO_ACK )
{
	// 던전, 대전 랭킹 업데이트
	SiKRankingManager()->UpdateDungeonRanking( kPacket_.m_vecDungeonRanking );
	SiKRankingManager()->UpdatePvpRanking( kPacket_.m_vecPvpRanking );

	// 게임서버로 브로드 캐스팅
	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_VECTOR;
		SiKRankingManager()->GetDungeonRankingVector( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );        
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_MAP;
		SiKRankingManager()->GetDungeonRankingMap( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_VECTOR;
		SiKRankingManager()->GetPvpRankingVector( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true;
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_MAP;
		SiKRankingManager()->GetPvpRankingMap( 0, kPacket );
		BroadCastAllGS( ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}
}

//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_TEST

IMPL_ON_FUNC( DBE_GUILD_LEVEL_UP_ACK )
{
	// 길드 레벨업 업데이트 결과 처리
	if( SiKGuildManager()->UpdateGuildLevelResult( kPacket_.m_iGuildUID, kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 레벨업 업데이트 결과 처리 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOGc( kPacket_.m_ucGuildLevel )
			<< END_LOG;

		return;
	}

	//{{ 2010. 01. 25  최육사	길드 게시판
#ifdef SERV_GUILD_AD

	// 길드 광고가 있는지 찾아보고 있다면 길드정보를 갱신하자!
	SiKGuildBoardManager()->UpdateGuildLevelResult( kPacket_.m_iGuildUID, kPacket_.m_ucGuildLevel );

#endif SERV_GUILD_AD
	//}}

	// 길드 레벨 5일때, 10일때, 15일때 전체 공지 보내기
	switch( kPacket_.m_ucGuildLevel )
	{
	case 5:
	case 10:
	case 15:
		{
			KEGS_NOTIFY_MSG_NOT	kPacketNot;
			kPacketNot.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_GUILD_LEVEL_UP;
			kPacketNot.m_Count = 1;

			CStringW cwstrMessage;
			cwstrMessage.Format( L"%s;%d", kPacket_.m_wstrGuildName.c_str(), static_cast<int>(kPacket_.m_ucGuildLevel) );
			kPacketNot.m_wstrMSG = static_cast<LPCTSTR>(cwstrMessage);

			SendToGameServer( ELG_GUILD_LEVEL_UP_NOTIFY_NOT, kPacketNot );
		}
		break;
	}
}

#endif GUILD_TEST
//}}

//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( DBE_INSERT_GUILD_CASH_SKILL_POINT_ACK )
{
	// 길드 매니저에 결과 업데이트
	if( SiKGuildManager()->InsertGuildCashSkillPointResult( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 캐쉬 스킬 포인트 업데이트 결과 처리 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK )
{
	// 길드 매니저에 결과 업데이트
	if( SiKGuildManager()->ExpireGuildCashSkillPointResult( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 캐쉬 스킬 포인트 기간종료 결과 처리 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( DBE_ADMIN_GET_GUILD_SKILL_POINT_ACK, KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK )
{
	// 치트 결과 업데이트
    if( SiKGuildManager()->AdminInsertGuildSkillPointResult( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 스킬 포인트 치트 결과 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
}

#endif GUILD_SKILL_TEST	
//}}

//{{ 2010. 01. 15  최육사	길드게시판
#ifdef SERV_GUILD_AD

IMPL_ON_FUNC( DBE_GET_GUILD_BOARD_INFO_ACK )
{
	// DB로부터 받은 길드 게시판 정보 업데이트
	SiKGuildBoardManager()->UpdateGuildBoardInfo( kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCEPT_JOIN_GUILD_ACK )
{
	// 예약된 정보를 지운다
	if( SiKGuildManager()->RemoveReserve( kPacket_.m_iGuildUID, kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"길드 가입 예약 정보 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID )
			<< END_LOG;
	}

	// 길드 가입 성공이 아니면 종료처리
	if( kPacket_.m_iOK != NetError::NET_OK )
		return;

	// 길드 가입 처리!
	if( SiKGuildManager()->JoinGuildMember( kPacket_.m_iGuildUID, kPacket_.m_kJoinGuildMemberInfo, false ) == false )
	{
		START_LOG( cerr, L"길드 멤버 로그인 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID )
			<< END_LOG;
	}

	// 길드 정보 얻기
	KEGS_GUILD_INFO_NOT kNot;
	SiKGuildManager()->GetGuildInfo( kPacket_.m_iGuildUID, kNot );

	// 현재 등록한 길드 광고가 있는지 확인
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		// 현재 인원 정보 업데이트
		SiKGuildBoardManager()->UpdateGuildCurMemberCount( kPacket_.m_iGuildUID, static_cast<u_short>(kNot.m_vecMemberUnitUID.size()) );
	}

	// 로그오프한 유저를 길드 가입하는것이므로 편지로 가입 사실을 알려주자!	
	kPacket_.m_wstrGuildName = kNot.m_kGuildInfo.m_wstrGuildName;
	SendToGameServer( ELG_SEND_LETTER_JOIN_GUILD_NOT, kPacket_ );
}

#endif SERV_GUILD_AD
//}}

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT	

IMPL_ON_FUNC( DBE_GET_WEB_POINT_REWARD_ACK )
{
	if( kPacket_.m_vecWebPointReward.empty() )
		return;

	SendToGameDB( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_ACK, KDBE_GET_WEB_POINT_REWARD_ACK )
{
	if( kPacket_.m_vecWebPointReward.empty() )
		return;

	// 게임서버로 보상처리 하러가기
	UidType iGameServerUID = KBaseServer::GetKObj()->GetFirstActorKey();
	if( iGameServerUID <= 0 )
	{
		START_LOG( clog, L"웹 포인트 보상을 게임서버로 전송하려는데 ActorManager에 연결된 게임서버가 하나도 없군.." )
			<< BUILD_LOG( iGameServerUID )
			<< END_LOG;

		return;
	}

	// 게임서버로 랭킹 보상 보내기
	UidType anTrace[2] = { iGameServerUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, ELG_WEB_POINT_REWARD_INSERT_TO_POST_NOT, kPacket_ );	

	// 넥슨 웹DB에 결과 업데이트
	SendToNXWebDB( DBE_RESULT_WEB_POINT_REWARD_NOT, kPacket_ );
}

#endif SERV_WEB_POINT_EVENT
//}}

//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK

IMPL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK )
{
	std::map< int, std::vector< UidType > > mapBlockUserList;
	std::map< int, std::vector< UidType > > mapUnblockUserList;
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	std::vector< std::string > vecBlockMachineIDList;
	SiKAccountBlockManager()->UpdateAccountBlockList( kPacket_, mapBlockUserList, mapUnblockUserList, vecBlockMachineIDList );
#else
	SiKAccountBlockManager()->UpdateAccountBlockList( kPacket_, mapBlockUserList, mapUnblockUserList );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}    

	// 새로 추가된 차단해제 유저는 접속 여부를 확인하여 접속종료시킨다!
	{
		std::map< int, std::vector< UidType > >::const_iterator mit;
		for( mit = mapBlockUserList.begin(); mit != mapBlockUserList.end(); ++mit )
		{
			const int iBlockType = mit->first;
			const std::vector< UidType >& vecBlockList = mit->second;

			if( iBlockType < 0  ||  iBlockType >= KAccountBlockManager::BT_MAX )
				continue;

			switch( iBlockType )
			{
			case KAccountBlockManager::BT_CONNECT_BLOCK:
				{
					BOOST_TEST_FOREACH( const UidType, iUserUID, vecBlockList )
					{
						KUserList::KGSUserInfo kInfo;
						if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
							continue;

						int iOK = NetError::ERR_ADMIN_COMMAND_07;
						SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, E_KICK_USER_NOT, iOK );

						START_LOG( cout, L"접속 차단 대상자을 실시간으로 KICK합니다!" )
							<< BUILD_LOG( kInfo.m_nGSUID )
							<< BUILD_LOG( kInfo.m_nUserUID )
							<< BUILD_LOG( iUserUID );
					}
				}
				break;

			case KAccountBlockManager::BT_TRADE_BLOCK:
				{
					BOOST_TEST_FOREACH( const UidType, iUserUID, vecBlockList )
					{
						KUserList::KGSUserInfo kInfo;
						if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
							continue;

						KELG_TRADE_BLOCK_NOT kNot;
						kNot.m_bIsTradeBlock = true;
						SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ELG_TRADE_BLOCK_NOT, kNot );

						START_LOG( cout, L"거래 차단 대상자을 실시간으로 적용합니다!" )
							<< BUILD_LOG( kInfo.m_nGSUID )
							<< BUILD_LOG( kInfo.m_nUserUID )
							<< BUILD_LOG( iUserUID );
					}
				}
				break;

			default:
				break;
			}
		}
	}	

	// 삭제된 차단해제 유저는 접속 여부를 확인하여 실시간 해제 시킨다!
	{
		std::map< int, std::vector< UidType > >::const_iterator mit;
		for( mit = mapUnblockUserList.begin(); mit != mapUnblockUserList.end(); ++mit )
		{
			const int iBlockType = mit->first;
			const std::vector< UidType >& vecBlockList = mit->second;

			if( iBlockType < 0  ||  iBlockType >= KAccountBlockManager::BT_MAX )
				continue;

			switch( iBlockType )
			{
			case KAccountBlockManager::BT_CONNECT_BLOCK:
				break;

			case KAccountBlockManager::BT_TRADE_BLOCK:
				{
					BOOST_TEST_FOREACH( const UidType, iUserUID, vecBlockList )
					{
						KUserList::KGSUserInfo kInfo;
						if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( iUserUID, kInfo ) == false )
							continue;

						KELG_TRADE_BLOCK_NOT kNot;
						kNot.m_bIsTradeBlock = false;
						SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ELG_TRADE_BLOCK_NOT, kNot );

						START_LOG( cout, L"거래 차단 대상자을 실시간으로 해제합니다!" )
							<< BUILD_LOG( kInfo.m_nGSUID )
							<< BUILD_LOG( kInfo.m_nUserUID )
							<< BUILD_LOG( iUserUID );
					}
				}
				break;

			default:
				break;
			}
		}
	}	

	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	{
		// 접속중인 유저 접속 해제 시키기!
		BOOST_TEST_FOREACH( const std::string&, strMachineID, vecBlockMachineIDList )
		{
			KUserList::KGSUserInfo kInfo;
			if( GetKLoginSimLayer()->GetGSUserInfoByMachineID( strMachineID, kInfo ) == false )
				continue;

			int iOK = NetError::ERR_ADMIN_COMMAND_07;
			SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, E_KICK_USER_NOT, iOK );

			START_LOG( cout, L"머신ID 차단 대상자을 실시간으로 차단합니다!" )
				<< BUILD_LOG( kInfo.m_nGSUID )
				<< BUILD_LOG( kInfo.m_nUserUID )
				<< BUILD_LOG( strMachineID );
		}
	}
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
}

#endif SERV_ACCOUNT_BLOCK
//}}

//{{ 2010. 07. 19  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	// 로그인서버로 가는 명령인지 확인!
	if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SENTINEL < kPacket_.m_iOrderType  && 
		KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SENTINEL > kPacket_.m_iOrderType )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		SiKLoginScriptRefreshManager()->QueueingEvent( spEvent );
	}
	else
	{
		START_LOG( cerr, L"이쪽으로 오면 안되는 갱신 요청 타입입니다!" )
			<< BUILD_LOG( kPacket_.m_iOrderType )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_GUILD_MANAGER, CXSLGuildManager );
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_MORNITORING_MANAGER, KMornitoringManager );
#endif SERV_MORNITORING
	//}}
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_GAME_SYS_VAL, KGameSysVal );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
	//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_TITLE_MANAGER, CXSLTitleManager );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}

	//{{ 2012. 10. 9	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_LG_SKILL_TREE, CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

	default:
		{
			START_LOG( cerr, L"이쪽으로 오면 안되는 타입인데?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"LoginServer 스크립트 실시간 패치 완료"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

#endif SERV_REALTIME_SCRIPT
//}}

//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
IMPL_ON_FUNC( DBE_GET_MODULE_INFO_LIST_ACK )
{
	//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
	SiKDLLManager()->Init( kPacket_ );
#else
	SiKDLLManager()->Init( kPacket_.m_vecModuleList );
#endif SERV_HACKING_DLL_CHECK
	//}}

	START_LOG( cout, L"DLL 모듈 리스트 업데이트 완료!" )
		<< BUILD_LOG( kPacket_.m_vecModuleList.size() );
}

_IMPL_ON_FUNC( DBE_UPDATE_MODULE_INFO_LIST_ACK, KDBE_GET_MODULE_INFO_LIST_ACK )
{
	SiKDLLManager()->UpdateModuleList( kPacket_.m_vecModuleList );
}
#endif SERV_DLL_MANAGER
//}}

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
_IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT )
{
	SiKMornitoringManager()->ReserveSMSMessage( kPacket_ );
}
#endif SERV_MORNITORING
//}}

//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
IMPL_ON_FUNC( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK )
{
    SiKRandomItemEventManager()->UpdateEventInfo( kPacket_.m_vecTimeOpenRandomItemInfo, kPacket_.m_setGetRewardUserUIDList );
}
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}

//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
IMPL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_ACK )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iRewardLetter.m_iToUnitUID, kUserInfo ) == false )
		return;

	KELG_FIND_USER_SEND_LETTER_NOT kPacket;
	kPacket.m_kLetter = kPacket_.m_iRewardLetter;
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_FIND_USER_SEND_LETTER_NOT, kPacket );
}

IMPL_ON_FUNC( DBE_LOAD_PSHOP_AGENCY_ACK )
{
	if( kPacket_.m_vecOpenPShopAgency.empty() )
	{
		START_LOG( cout, L"개설된 대리 상인이 없습니다!" )
			<< BUILD_LOG( kPacket_.m_vecOpenPShopAgency.size() );
		return;
	}

	int iOpenPShopAgencyCount = 0;

	BOOST_TEST_FOREACH( const KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT&, kPacket, kPacket_.m_vecOpenPShopAgency )
	{
		if( kPacket.m_vecSellItemInfo.empty() )
		{
			START_LOG( clog, L"판매 아이템이 비어있습니다! 자동으로 상점개설을 하지 않습니다!" )
				<< BUILD_LOG( kPacket.m_iUnitUID );
			continue;
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_SERVER, NULL, ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, kPacket );
		//LIF( spEvent->m_kDestPerformer.AddUID( mit->first ) ); -- RoomUID는 이제 필요 없다!
		GetKLoginRoomManager()->QueueingEventToRoom( spEvent );

		++iOpenPShopAgencyCount;
	}

	START_LOG( cout, L"DB에 저장된 대리상인 자동 개설 완료!" )
		<< BUILD_LOG( iOpenPShopAgencyCount );
}

_IMPL_ON_FUNC( DBE_BREAK_PSHOP_AGENCY_ACK, KERM_BREAK_PSHOP_AGENCY_NOT )
{
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUID, kInfo ) == false )
		return;
	
	SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ERM_BREAK_PSHOP_AGENCY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_PSHOP_AGENCY_MESSAGE_NOT, KEGS_PSHOP_AGENCY_MESSAGE_NOT )
{
	KUserList::KGSUserInfo kInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUnitUID, kInfo ) == false )	
		return;

	SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ELG_PSHOP_AGENCY_MESSAGE_NOT, kPacket_ );
}
#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
IMPL_ON_FUNC( ELG_RANKING_TITLE_REWARD_NOT )
{
	// 특정 서버만 업데이트 하도록 할것!
	START_LOG( cout, L"랭킹 매니저의 요청으로 랭킹 칭호 보상 지급을 시작합니다!" )
		<< BUILD_LOG( kPacket_.m_mapTitleReward.size() );
	
	// GameDB에 타이틀을 insert해준다.
	std::map< UidType, KRankingTitleReward >::const_iterator mit;
	for( mit = kPacket_.m_mapTitleReward.begin(); mit != kPacket_.m_mapTitleReward.end(); ++mit )
	{
		const KRankingTitleReward& kTitleReward = mit->second;
		std::wstring wstrMessage;
	
		BOOST_TEST_FOREACH( const KTitleReward&, kRewardInfo, kTitleReward.m_vecReward )
		{
			// 타이틀 보상 주기
			KDBE_INSERT_TITLE_REQ kPacket;
			kPacket.m_iUnitUID		   = kTitleReward.m_iUnitUID;
			kPacket.m_iTitleID		   = kRewardInfo.m_iTitleID;
			kPacket.m_sPeriod		   = kRewardInfo.m_sPeriod;
			SendToGameDB( DBE_INSERT_TITLE_REQ, kPacket );
	
			const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( kRewardInfo.m_iTitleID );
			if( pTitleInfo == NULL )
			{
				START_LOG( cerr, L"존재하지 않는 칭호입니다!" )
					<< BUILD_LOG( kRewardInfo.m_iTitleID )
					<< END_LOG;
	
				continue;
			}
			//{{ 2011.11.20 임홍락	서버 단일화 // SERV_USE_NATION_FLAG 필수 포함임
#ifdef SERV_UNITED_SERVER_EU
			wstrMessage += boost::str( boost::wformat( L"%d" ) % pTitleInfo->m_iTitleID );
			//wstrMessage += L",\n\n";					// EU Mail 개행 문제 수정
#else SERV_UNITED_SERVER_EU
			wstrMessage += pTitleInfo->m_wstrTitleName;
			wstrMessage += L",\n\n";
#endif SERV_UNITED_SERVER_EU
			//}}
		}
	
		// 타이틀 보상에 대해 편지로 알리기
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = kTitleReward.m_iUnitUID;
		kPacketToDB.m_iToUnitUID   = kTitleReward.m_iUnitUID;
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_RANKING_TITLE;
		kPacketToDB.m_iRewardID	   = 0; // 보상없음
		kPacketToDB.m_wstrMessage  = wstrMessage;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
}

IMPL_ON_FUNC( DBE_INSERT_TITLE_ACK )
{
	KUserList::KGSUserInfo kUserInfo;
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kUserInfo ) )
		return;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INSERT_TITLE_NOT, kPacket_ );
}
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
IMPL_ON_FUNC( DBE_GET_HACKING_MODULE_LIST_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK && kPacket_.m_bRequest == true )
	{
		SiKDLLManager()->InitHackingModuleList( kPacket_.m_vecModuleList, kPacket_.m_iReleaseTick );

		START_LOG( cout, L"Module List를 다시 받았습니다." )
			<< END_LOG;
	}
}
#endif SERV_HACKING_DLL_CHECK
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
IMPL_ON_FUNC( DBE_LOCAL_RANKING_RESET_ACK )
{
	KEventPtr	spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_SERVER, NULL, DBE_LOCAL_RANKING_RESET_ACK, kPacket_ );
	SiKLoginLocalRankingManager()->QueueingEvent( spEvent );
	SiKGuildManager()->ResetLocalRankingInfo();
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( DBE_LOAD_WEDDING_HALL_INFO_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB에 있는 결혼식 정보를 받아오기 실패!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;
		return;
	}

	SiKWeddingHallManager()->SetWeddingInfoDB( kPacket_.m_mapWeddingInfo );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		BroadCastAllGS( ELG_WEDDING_HALL_INFO_UPDATE_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( ELG_ADD_WEDDING_HALL_INFO_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK )
{
	SiKWeddingHallManager()->SetWeddingInfoDB( kPacket_.m_mapWeddingInfo );
	BroadCastAllGS( ELG_WEDDING_HALL_INFO_UPDATE_NOT, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS// 작업날짜: 2013-10-28	// 박세훈
IMPL_ON_FUNC( E_DISCONNECT_SERVER_REPORT_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH );
		}
		break;

	default:
		START_LOG( cerr, L"잘못된 타입입니다!" )
			<< BUILD_LOGc( kPacket_.m_cType )
			<< END_LOG;
		break;
	}
}
#endif // SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS

#ifdef SERV_GLOBAL_AUTH 
IMPL_ON_FUNC( EPUBLISHER_REG_USER_ACK )
{
	// 국가별로 따로 RegUserAck 처리 하는게 없어서 함수로 따로 만들지는 않음 //
	KUserList::KGSUserInfo kInfo;
	
	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrServiceAccountID, kInfo ) )
	{
		//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
		GetKLoginSimLayer()->UnRegAuthWaitUser( kInfo.m_nUserUID );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
		//}}
		KELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrServiceAccountID = kPacket_.m_wstrServiceAccountID;
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		kPacketAck.m_bIsPcBang = kPacket_.m_bIsPcBang;
		kPacketAck.m_iPCBangType = kPacket_.m_iPCBangType;
		kPacketAck.m_iCheckState = kPacket_.m_iCheckState;

		START_LOG( clog, L"PC방 유저 체크." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOGc( kPacket_.m_bIsPcBang )
			<< BUILD_LOG( kPacket_.m_iPCBangType )
			<< BUILD_LOG( kPacket_.m_iCheckState )
			<< END_LOG;
#else //SERV_CHECK_PCBANG_BY_PUBLISHER
		START_LOG( clog, L"PC방 유저 체크." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< END_LOG;
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_COUNTRY_TH
		kPacketAck.m_wstrSockID = kPacket_.m_wstrSockID;
#endif //SERV_COUNTRY_TH

		UidType anTrace[2] = { kInfo.m_nGSUID, -1 };
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_USER, kInfo.m_nUserUID, anTrace, ELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK, kPacketAck );
	}
	else
	{
		START_LOG( cerr, L"유저 게임 서버 검색 실패." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EPUBLISHER_UNREG_USER_ACK )
{
	// 국가별로 따로 UnRegUserAck 처리 하는게 없어서 함수로 따로 만들지는 않음 //
}

#ifdef SERV_COUNTRY_CN
IMPL_ON_FUNC( EGIANT_INFO_GET_CCU_REQ )
{
	typedef std::map< UidType, KUpdateCCUInfo > CCUInfoMap;

	KEGIANT_INFO_GET_TOTAL_CCU_ACK kTotalCCUAck;
	kTotalCCUAck.m_TimeStamp = kPacket_.m_TimeStamp;
	kTotalCCUAck.m_usServerID = static_cast<unsigned short>(GetUID());
	kTotalCCUAck.m_usServerType = GetServerClass();
	kTotalCCUAck.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kTotalCCUAck.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
	kTotalCCUAck.m_strZoneName = SiKGiantInfoManager()->GetServerInfo().m_strZoneName;
	kTotalCCUAck.m_uiCCU = 0;
	BOOST_FOREACH( const CCUInfoMap::value_type& value, SiKServerCCUManager()->GetLastCCUInfo() )
	{
		kTotalCCUAck.m_uiCCU += value.second.m_iConcurrentUser;
	}

	KEventPtr spTotalEvent( new KEvent );
	spTotalEvent->SetData( PI_GS_SERVER, NULL, EGIANT_INFO_GET_TOTAL_CCU_ACK, kTotalCCUAck );
	SiKGiantInfoManager()->QueueingEvent( spTotalEvent );


	KEGIANT_INFO_GET_CHANNEL_CCU_ACK kChannelCCUAck;
	kChannelCCUAck.m_TimeStamp = kPacket_.m_TimeStamp;
	kChannelCCUAck.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kChannelCCUAck.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
	BOOST_FOREACH( const CCUInfoMap::value_type& value, SiKServerCCUManager()->GetLastCCUInfo() )
	{
		kChannelCCUAck.m_mapChannelCCU.insert(std::make_pair(value.second.m_iServerUID, value.second.m_iConcurrentUser));
	}

	KEventPtr spChannelEvent( new KEvent );
	spChannelEvent->SetData( PI_GS_SERVER, NULL, EGIANT_INFO_GET_CHANNEL_CCU_ACK, kChannelCCUAck );
	SiKGiantInfoManager()->QueueingEvent( spChannelEvent );
}
#endif SERV_COUNTRY_CN

#endif // SERV_GLOBAL_AUTH