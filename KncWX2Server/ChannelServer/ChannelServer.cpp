#include <WinSock2.h>
#include <sstream>
#include <iomanip>
#include <boost/bind.hpp>
#include "ChannelServer.h"
#include "ActorManager.h"
#include "ChannelSimLayer.h"
#include "ChannelNetLayer.h"
#include "socket/NetCommon.h"
#include "DBLayer.h"
#include "ChannelAccountDBThread.h"
#include "ChannelLogDBThread.h"
//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "CHFSM.h"
#else
	#include "defaultFSM.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
#include "ChannelUser.h"

#include "NetError.h"
//{{ 2009. 12. 21  최육사	서버군 확장
#include "Enum/Enum.h"
//}}

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	#include "HackingProtectionManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#include "ProxyManager.h"
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_COUNTRY_JP
#include "../Common/OnlyGlobal/AuthAndBilling/JP/HanAuthForSvr.h"
#endif //SERV_COUNTRY_JP

#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_IN
#include "..\Common\OnlyGlobal\AuthAndBilling\IN\ChannelFunizenAuthDBThread.h"
#endif SERV_COUNTRY_IN
#endif SERV_GLOBAL_AUTH

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2011. 03. 29	최육사	TBB 메모리 관리자
//#ifdef SERV_TBB_MALLOC_PROXY_TEST
//	#include <tbb/tbbmalloc_proxy.h>
//#endif SERV_TBB_MALLOC_PROXY_TEST
//}}

//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#include "MachineBlockManager.h"
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2012. 03. 13	김민성		실시간 동접 하락 체크 sms
#ifdef SERV_CHECK_DROP_CCU
	#include "ChannelSMSDBThread.h"
#endif SERV_CHECK_DROP_CCU
//}}
//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	#include "UdpRelayChecker/UdpRelayChecker.h"
#endif SERV_UDP_RELAY_CHECKER
//}}
//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "WaitQueueManager.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2012. 10. 15	박세훈	채널 서버 실시간 스크립트 갱신 기능
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	#include "ChannelScriptRefreshManager.h"
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}
//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

//#include "vld.h"

NiImplementRTTI( KChannelServer, KBaseServer );
ImplPfID( KChannelServer, PI_CHANNEL_SERVER );

#define CLASS_TYPE KChannelServer

ImplToStringW( KChannelServer )
{
	START_TOSTRING_PARENTW( KBaseServer );

	stm_ << L"Channel List Size : " << m_mapChannelList.size() << std::endl;

    return stm_;
}

KChannelServer::KChannelServer(void) :
m_bIsFirstUpdate( false )
#ifdef SERVER_GROUP_UI_ADVANCED
,m_bIsFirstUpdateServerGroup( false )
#endif SERVER_GROUP_UI_ADVANCED
{
}

KChannelServer::~KChannelServer(void)
{
}

#include <lua_tinker.h>

void KChannelServer::RegToLua()
{
    KBaseServer::RegToLua();

	lua_tinker::class_add<KChannelServer>( g_pLua, "KChannelServer" );
	lua_tinker::class_inh<KChannelServer, KBaseServer>( g_pLua );
	lua_tinker::class_def<KChannelServer>( g_pLua, "dump", &KChannelServer::Dump );

	lua_tinker::decl( g_pLua, "Server", this );
}

const wchar_t*  g_szConfigFile = L"config_ch";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KChannelServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
    switch( iDBConnectionInfo )
    {
    case KDBLayer::DC_ACCOUNT:
        return new KChannelAccountDBThread( szDSN, bDBConnStr );
    case KDBLayer::DC_LOG:
        return new KChannelLogDBThread( szDSN, bDBConnStr );
		//{{ 2012. 03. 13	김민성		실시간 동접 하락 체크 sms
#ifdef SERV_CHECK_DROP_CCU
	case KDBLayer::DC_SMS:
		return new KChannelSMSDBThread( szDSN, bDBConnStr );
#endif SERV_CHECK_DROP_CCU
		//}}
#ifdef SERV_GLOBAL_AUTH
#ifdef SERV_COUNTRY_IN
	case KDBLayer::DC_PUBLISHER_AUTH:
		return new KChannelFunizenAuthDBThread( szDSN, bDBConnStr );
#endif SERV_COUNTRY_IN
#endif SERV_GLOBAL_AUTH
    default:
        START_LOG( cerr, L"접속하려는 DB 종류가 이상함." )
            << BUILD_LOG( iDBConnectionInfo )
            << END_LOG;
        return new KChannelLogDBThread( szDSN, bDBConnStr );
    }
}

bool KChannelServer::Init()
{
    KSimLayer::KeepObject<KChannelSimLayer>();
    KNetLayer::KeepObject<KChannelNetLayer>();
    KActorManager::KeepObject<KActorManager>();
	//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::KeepObject<KUdpRelayChecker>();
#endif SERV_UDP_RELAY_CHECKER
	//}}

	//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KChannelUser, KCHFSM, 32*1024> ) );
#else
	KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KChannelUser, KDefaultFSM, 32*1024> ) );
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}    
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_CLIENT );
#endif SERV_KEVENT_FROM
	//}}

#ifdef SERV_LOGIN_RESULT_INFO
	InitLoginResultInfo();
#endif SERV_LOGIN_RESULT_INFO

    _JIF( KBaseServer::Init(), return false );

    //WriteServerInfoToDB();

#ifdef SERV_COUNTRY_JP
	int ret = -1;
	if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_ALPHA )
	{
		ret = HanAuthInit("J_ES_PRE");

		if(ret == 0)
		{
			START_LOG( clog, L"알파버전 인증 초기화 완료" )				
				<< END_LOG;
		}

	}
	else if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_REAL )
	{
		ret = HanAuthInit("J_ES");

		if(ret == 0)
		{
			START_LOG( clog, L"리얼버전 인증 초기화 완료" )				
				<< END_LOG;
		}
	}

	//{{ 2009. 11. 27  김정협 일본인증
	if(ret != 0)
	{
		START_LOG( cerr, L"인증초기화 에러" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}
#endif //SERV_COUNTRY_JP

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	SiKProxyManager()->ConnectAll();
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	SetServerReady( true );

	//{{ 2009. 8. 27  최육사	server ready
	OnServerReadyComplete();
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    return true;
}

void KChannelServer::OnServerReadyComplete()
{
	KBaseServer::OnServerReadyComplete();

#ifdef SERVER_GROUP_UI_ADVANCED
	RefreshServerGroupList();
#endif SERVER_GROUP_UI_ADVANCED

	//{{ 2009. 8. 13  최육사	채널리스트
	RefreshChannelList();
	//}}

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager()->Init();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2012. 10. 15	박세훈	채널 서버 실시간 스크립트 갱신 기능
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	SiKChannelScriptRefreshManager()->Init();
	SiKChannelScriptRefreshManager()->RegToLua( g_pLua );
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
	//}}

	//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	SiKMachineBlockManager()->Init();
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}

	//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->Init();
#endif SERV_UDP_RELAY_CHECKER
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\ChannelServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\ChannelServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	// 서버군 이름 표시
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );
}

void KChannelServer::ShutDown()
{
#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

    // 아직 아무 종료처리도 진행되기 전.
    KBaseServer::ShutDown();        // 이곳에서 모든 종료가 끝난다. 각각의 Layer들도 모두 소멸됨.

	//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->ShutDown();
	KUdpRelayChecker::ReleaseKObj();
#endif SERV_UDP_RELAY_CHECKER
	//}}
}

bool KChannelServer::DestroyAllSession()
{
    // NetLayer의 Accepter만 닫힌 시점. 아직 모든 Layer들이 살아있다.
 
    KActorManager* pkActorManager = KActorManager::GetKObj();
   
    START_LOG( cout, L"현재 유저 : " << pkActorManager->GetCount() << L" 명" );

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

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	// proxy manager의 종료 처리는 일단 유저 객체의 소멸처리를 끝낸 다음에 별도 진행한다.
	bool bProxyShutdown = SiKProxyManager()->Shutdown( 60.0f );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

    LOG_CONDITION( bSuccess, cout, cerr )
        << BUILD_LOG( m_dwShutDownWait )
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		<< BUILD_LOG( bProxyShutdown )
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
        << BUILD_LOG( pkActorManager->GetCount() )
        << BUILD_LOG( SiKDBLayer()->GetTotalEventNum() )
        << BUILD_LOG( ::GetTickCount() - dwBeginTick );

    return bSuccess;
}

void KChannelServer::Tick()
{
    KBaseServer::Tick();

#ifdef SERVER_GROUP_UI_ADVANCED
	if( m_kServerGroupListRefreshTimer.elapsed() > 60.f * 60 )
	{
		RefreshServerGroupList();

		m_kServerGroupListRefreshTimer.restart();
	}
#endif SERVER_GROUP_UI_ADVANCED

	//{{ 2009. 1. 16  최육사	채널 리스트 업데이트
	if( m_kChannelListRefreshTimer.elapsed() > 60.f )
	{
		RefreshChannelList();

		m_kChannelListRefreshTimer.restart();
	}
	//}}

	//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	SiKMachineBlockManager()->Tick();
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}

#ifdef SERV_LOGIN_RESULT_INFO
	if( m_kLoginResultInfoTimer.elapsed() > 60.f )
	{
		WriteLoginResultInfoToDB();
		m_kLoginResultInfoTimer.restart();

		InitLoginResultInfo();
	}
#endif SERV_LOGIN_RESULT_INFO

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\ChannelServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\ChannelServerWrite"));
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

#ifdef SERVER_GROUP_UI_ADVANCED
void KChannelServer::RefreshServerGroupList()
{
	SendToLogDB( DBE_SERVERGROUP_LIST_REQ );
}

void KChannelServer::UpdateServerGroupList( const std::map< int, KServerGroupInfo >& mapServerGroupList )
{
	m_mapServerGroupList.clear();
	m_mapServerGroupList = mapServerGroupList;

	// 처음 업데이트 한거라면 리스트를 로그로 출력한다!
	if( !m_bIsFirstUpdateServerGroup )
	{
		std::map< int, KServerGroupInfo >::iterator mit;
		for( mit = m_mapServerGroupList.begin(); mit != m_mapServerGroupList.end(); ++mit )
		{
			START_LOG( cout, L"서버군 :" )
				<< BUILD_LOG( mit->second.m_iOrder )
				<< BUILD_LOG( mit->second.m_iServerGroupUID )
				<< BUILD_LOG( mit->second.m_wstrServerGroupName )
				<< BUILD_LOG( mit->second.m_kServerIP )
				<< BUILD_LOG( mit->second.m_usMasterPort )
				<< BUILD_LOG( mit->second.m_usNCUDPPort )
				<< BUILD_LOG( mit->second.m_bEnable );
		}

		m_bIsFirstUpdateServerGroup = true;
	}
}
#endif SERVER_GROUP_UI_ADVANCED

void KChannelServer::RefreshChannelList()
{
	KDBE_CHANNEL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iServerGroupID = GetServerGroupID();
	SendToLogDB( DBE_CHANNEL_LIST_REQ, kPacketToDB );
}

void KChannelServer::UpdateChannelList( const std::map< int, KChannelInfo >& mapChannelList )
{
	//{{ 2012. 03. 13	김민성		실시간 동접 하락 체크 sms
#ifdef SERV_CHECK_DROP_CCU
	if( SiKGameSysVal()->GetCheckDropCCU() == true )
	{
		std::map< int, KChannelInfo >			mapBeforChannelList = m_mapChannelList;
		
		std::map< int, KChannelInfo >::const_iterator mitNew = mapChannelList.begin();
		std::map< int, KChannelInfo >::iterator mitOld;

        for(  ; mitNew != mapChannelList.end() ; ++mitNew )
		{
			mitOld = mapBeforChannelList.find( mitNew->first );
			if( mitOld != mapBeforChannelList.end() )
			{
				if( mitOld->second.m_iCurrentUser < 500 )
				{
					continue;
				}

				int iGap = mitOld->second.m_iCurrentUser - mitNew->second.m_iCurrentUser;
				if( iGap > 0 )  // 동접이 하락하였다
				{
					// 설정한 동접 하락 수치 보다 높다면
					float fPercent = SiKGameSysVal()->GetDropCCUPercent();
					if( iGap > static_cast<int>(mitOld->second.m_iCurrentUser * fPercent) )
					{
						// sms 발송
						KDBE_SEND_PHONE_MSG_NOT kPacketNot;
						//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
						SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_CHECK_DROP_CCU, kPacketNot.m_vecPhoneNum );
#else
						//{{ 2012. 08. 11	박세훈	SMS 문자 발송 스크립트
#ifdef SERV_RUNTIME_SCRIPT_FOR_SMS
						SiKGameSysVal()->GetRecipientSMS( KGameSysVal::SMS_SURVEILLANCE_THE_CCU_DROP, kPacketNot.m_vecPhoneNum );
#else
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4118-3867" ) );		// 김창호 팀장님
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-7466-7958" ) );		// 우원식
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8421-1075" ) );		// 최육사
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-5598-4163" ) );		// 김민성
						//kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-9490-8761" ) );		// 황원준
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8982-3382" ) );		// 박창용
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-2906-5792" ) );		// 안위수
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8771-2480" ) );		// 유영식

						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4072-1031" ) ); // 김결
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-3898-7886" ) ); // 김강민 
						kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-5671-0502" ) ); // 이우진
#endif SERV_RUNTIME_SCRIPT_FOR_SMS
						//}}
#endif SERV_SMS_TOTAL_MANAGER
						//}}
						
						
						kPacketNot.m_wstrSMSMessage = L"!동접 급락 발견! IP : ";
						kPacketNot.m_wstrSMSMessage += mitNew->second.m_wstrIP;
						kPacketNot.m_wstrSMSMessage += L" ";
						kPacketNot.m_wstrSMSMessage += mitNew->second.m_wstrChannelName;
						kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L" %d " ) % mitOld->second.m_iCurrentUser );
						kPacketNot.m_wstrSMSMessage += L"->";
						kPacketNot.m_wstrSMSMessage += boost::str( boost::wformat( L" %d" ) % mitNew->second.m_iCurrentUser );
						kPacketNot.m_wstrSMSMessage += L"로 하락!";
						SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
					}
				}
			}
		}
	}
#endif SERV_CHECK_DROP_CCU
	//}}

	m_mapChannelList.clear();
	m_mapChannelList = mapChannelList;

	std::map< int, KChannelInfo >::iterator mit;
	for( mit = m_mapChannelList.begin(); mit != m_mapChannelList.end(); ++mit )
	{
		mit->second.m_iCurPartyCount = 0;
		mit->second.m_iPlayGamePartyCount = 0;
	}

	// 처음 업데이트 한거라면 리스트를 로그로 출력한다!
	if( !m_bIsFirstUpdate )
	{
		std::map< int, KChannelInfo >::iterator mit;
		for( mit = m_mapChannelList.begin(); mit != m_mapChannelList.end(); ++mit )
		{
			START_LOG( cout, L"채널 이름 : " << mit->second.m_wstrChannelName << L",  IP : " << mit->second.m_wstrIP << L",  Port : " << mit->second.m_usMasterPort );
		}

		m_bIsFirstUpdate = true;
	}
}

//{{ 2012. 06. 29	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
void KChannelServer::UpdateChannelBonusList( const std::map< int, KChannelBonusInfo >& mapChannelBonusList )
{
	m_mapChannelBonusList.clear();

	m_mapChannelBonusList = mapChannelBonusList;

}
#endif SERV_CHANNEL_LIST_RENEWAL
//}}

void KChannelServer::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_ACK, KDBE_CHANNEL_SERVER_ON_ACK );
       _CASE( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
		CASE( DBE_CHANNEL_LIST_ACK );

		//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
		CASE( DBE_CHECK_HACKING_TOOL_LIST_ACK );
#endif SERV_HACKING_TOOL_LIST
		//}}
		//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
		CASE( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK );
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
		//}}
		//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
		CASE( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK );
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}

		//{{ 2012. 10. 15	박세훈	채널 서버 실시간 스크립트 갱신 기능
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
		_CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
		//}}
#ifdef SERVER_GROUP_UI_ADVANCED
		CASE( DBE_SERVERGROUP_LIST_ACK );
#endif SERVER_GROUP_UI_ADVANCED
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

_IMPL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_CHANNEL_SERVER_ON_ACK )
{
	//스타트에 실패했으면 프로그램을 끈다
	if( kPacket_.m_iOK != NetError::NET_OK)
	{
		::exit( 0 );
		return;
	}

	SetServerInfo( kPacket_.m_kServerInfo );

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	std::map< int, KNetAddress >::iterator mit;
	for( mit = kPacket_.m_mapLoginList.begin(); mit != kPacket_.m_mapLoginList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_LOGIN, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

    SetEvent( m_spEventAck.get() );
}

_IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList )
{
    SetServerList( kPacket_ );
}

IMPL_ON_FUNC( DBE_CHANNEL_LIST_ACK )
{
    UpdateChannelList( kPacket_.m_mapChannelList );

	//{{ 2012. 06. 29	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	UpdateChannelBonusList(kPacket_.m_mapChannelBonusList);
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
}

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST

IMPL_ON_FUNC( DBE_CHECK_HACKING_TOOL_LIST_ACK )
{
	SiKHackingProtectionManager()->UpdateHackingToolList( kPacket_ );
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
IMPL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK )
{
	std::vector< std::string > vecBlockMachineIDList;
	SiKMachineBlockManager()->UpdateMachineBlockList( kPacket_, vecBlockMachineIDList );

	START_LOG( cout2, L"MachinBlock ID 목록 갱싱 성공!" );
}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
IMPL_ON_FUNC( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK )
{
	SiKWaitQueueManager()->SetTotalCCU( kPacket_.m_iTotalCCU );
}
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2012. 10. 15	박세훈	채널 서버 실시간 스크립트 갱신 기능
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT
_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
		//{{ 2012. 10. 9	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CH_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_CH_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
		//}}

	default:
		{
			START_LOG( cerr, L"이쪽으로 오면 안되는 타입인데?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}

#ifdef SERVER_GROUP_UI_ADVANCED
IMPL_ON_FUNC( DBE_SERVERGROUP_LIST_ACK )
{
	UpdateServerGroupList( kPacket_.m_mapServerGroupList );
}
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_LOGIN_RESULT_INFO
void KChannelServer::InitLoginResultInfo()
{
	m_kLoginResultInfo.m_vecResultCount.clear();

	for(int i = 0; i < KLoginResultInfo::LRTE_SENTINEL; i++)
	{
		m_kLoginResultInfo.m_vecResultCount.push_back(0);
	}
}

void KChannelServer::WriteLoginResultInfoToDB()
{
	// InitLoginResultInfo()이 호출될 때는 ServerUID 값을 가져오기 전이라 여기서 처리함.
	m_kLoginResultInfo.m_iServerGroupID = GetServerGroupID();
	m_kLoginResultInfo.m_uiServerUID = GetUID();

	KDBE_INSERT_LOGIN_RESULT_INFO_REQ kPacket;
	kPacket.m_iServerGroupID = m_kLoginResultInfo.m_iServerGroupID;
	kPacket.m_uiServerUID = m_kLoginResultInfo.m_uiServerUID;
	kPacket.m_vecResultCount = m_kLoginResultInfo.m_vecResultCount;

	SendToLogDB( DBE_INSERT_LOGIN_RESULT_INFO_REQ, kPacket );
}

void KChannelServer::PlusLoginResultCount(KLoginResultInfo::LOGIN_RESULT_TYPE_ENUM eResultType)
{
	if( eResultType < KLoginResultInfo::LRTE_SUCCESS || KLoginResultInfo::LRTE_SENTINEL < eResultType )
	{
		START_LOG( cerr, L"유효하지않은 로그인 결과 사유입니다!" )
			<< BUILD_LOG( eResultType )
			<< END_LOG;
		return ;
	}

	m_kLoginResultInfo.m_vecResultCount[eResultType]++;

	START_LOG( clog, L"로그인 결과 사유 카운트 증가!!" )
		<< BUILD_LOG( eResultType )
		<< BUILD_LOG( m_kLoginResultInfo.m_vecResultCount[eResultType] )
		<< END_LOG;
}
#endif SERV_LOGIN_RESULT_INFO