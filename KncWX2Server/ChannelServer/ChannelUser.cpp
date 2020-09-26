#include "ChannelUser.h"
#include <dbg/dbg.hpp>
//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "CHFSM.h"
#else
	#include "defaultFSM.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

#include "DBLayer.h"
#include "NetError.h"
#include "ChannelSimLayer.h"
#include "ChannelServer.h"
//#include "GashAuthManager.h"
//{{ 2009. 12. 16  최육사	동접툴
#include "CCUToolManager.h"
//}}

//{{ 2011. 01. 20 김민성	동접 모니터링 툴               디비커리인젝션
#ifdef SERV_CCU_MONITORING_TOOL
#include "odbc/Odbc.h"
#endif SERV_CCU_MONITORING_TOOL
//}}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
#include "ProxyManager.h"
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#include <boost/format.hpp>

//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	#include "MachineBlockManager.h"
	#include "HackingProtectionManager.h"
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	#include "ServerSNMaker.h"
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}
//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "WaitQueueManager.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}
//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	#include "DDOSGuardSimulator.h"
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
//}}

NiImplementRTTI( KChannelUser, KActor );
ImplPfID( KChannelUser, PI_CHANNEL_USER );

#define CLASS_TYPE  KChannelUser

KChannelUser::KChannelUser(void) 
{
	//{{ 2009. 10. 14  최육사	필터	
	InitRepeatEventFilter();
	//}}

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	m_bVerifyAccount = false;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	m_bPassedEnterChannel = false;
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}
	//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	m_bPassedDDOSGuardAuth = false;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//}}
#ifdef SERV_GLOBAL_AUTH
	m_wstrID = L"";
#ifdef SERV_PUBLISHER_MATRIXKEY
	m_uiMatrixKey = 0;
#endif //SERV_PUBLISHER_MATRIXKEY
#ifdef SERV_COUNTRY_TH
	m_wstrMasterID = L"";
	m_wstrSocketID = L"";
#endif //SERV_COUNTRY_TH
#endif //SERV_GLOBAL_AUTH
}

KChannelUser::~KChannelUser(void)
{
}

ImplToStringW( KChannelUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KChannelUser::ProcessEvent( const KEventPtr& spEvent_ )
{
	//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//static std::string strCheck = "192.168.66.17";
	//std::string strIP = GetIPStr();
	//if( strIP == strCheck )
	//{
	//	if( m_bPassedDDOSGuardAuth == false )
	//	{
	//		m_bPassedDDOSGuardAuth = SiKDDOSGuardSimulator()->IsRegUserSession( GetIPStr() );
	//		if( m_bPassedDDOSGuardAuth == false )
	//		{
	//			START_LOG( cout, L"[테스트로그] DDOS 가드 시뮬레이터 동작!! 접속을 차단합니다!" )
	//				<< BUILD_LOG( GetUID() )
	//				<< BUILD_LOG( GetIPStr() );

	//			// 유저 세션 등록!
	//			SiKDDOSGuardSimulator()->RegUserSession( GetIPStr() );

	//			// 종료 예약 처리!
	//			m_bDestroyReserved = true;
	//			return;
	//		}
	//	}
	//}	
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	//}}

    if( RoutePacket( spEvent_.get() ) )
        return;

	//{{ 2008. 8. 28  최육사	이벤트 필터
	if( !CheckReqEvent( spEvent_->m_usEventID ) )
		return;

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
    CASE( ECH_VERIFY_ACCOUNT_REQ );
#ifndef SERV_GLOBAL_AUTH 
    CASE( ECH_VERIFY_ACCOUNT_ACK );	// 해외에서 사용안함
#endif // SERV_GLOBAL_AUTH
    //CASE( DBE_GASH_USER_LOGIN_ACK );// 해외에서 이제 사용안함
	CASE_NOPARAM( ECH_GET_CHANNEL_LIST_REQ );
	CASE_NOPARAM( ECH_DISCONNECT_REQ );

	//{{ 2009. 12. 16  최육사	동접툴
	CASE( E_TOOL_GET_CCU_INFO_REQ );
	CASE( DBE_GET_CONCURRENT_USER_INFO_ACK );
	//}}

	CASE_NOPARAM( E_RESERVE_DESTROY );

	//{{ 2011. 01. 20 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
	CASE( E_TOOL_CHECK_LOGIN_REQ );
	CASE_NOPARAM( E_TOOL_SERVER_LIST_REQ );
	CASE( DBE_GET_CONCURRENT_USER_LOGIN_ACK );
	CASE( DBE_GET_GAME_SERVER_LIST_ACK );
#endif SERV_CCU_MONITORING_TOOL
	//}}

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	CASE( ECH_HACKING_TOOL_LIST_ACK );
	CASE( DBE_UPDATE_CHANNEL_RANDOMKEY_ACK );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

	//{{  2011.11.08     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	CASE( DBE_CHECK_SERVER_SN_ACK );
	CASE( DBE_CREATE_SERVER_SN_ACK );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}
#ifdef SERVER_GROUP_UI_ADVANCED
	CASE_NOPARAM( ECH_GET_SERVERGROUP_LIST_REQ );
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_GLOBAL_AUTH
	CASE( EPUBLISHER_AUTHENTICATION_ACK );
	CASE( DBE_CH_USER_GENERAL_LOGIN_ACK );
	CASE( DBE_CH_USER_KOGOTP_LOGIN_ACK );

	CASE( ECH_SECURITY_AUTH_REQ );
	CASE( EPUBLISHER_SECURITY_AUTH_ACK );

	CASE( EPUBLISHER_SECURITY_INFO_NOT );
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_SERVER_TIME_GET
	CASE_NOPARAM( ECH_GET_SERVER_TIME_REQ );
#endif SERV_SERVER_TIME_GET

#ifdef SERV_ID_NETMARBLE_PCBANG
	CASE( ECH_PCBANG_IP_AND_MAC_INFO_NOT );
#endif //SERV_ID_NETMARBLE_PCBANG
    default:
        dbg::cerr << L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr();
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

void KChannelUser::Tick()
{
    KActor::Tick();

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	CheckHackingToolList();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	CheckVerifyAccountRecv();
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

    switch( GetStateID() ) 
	{
	case KCHFSM::S_LOGINED:
		{
			// 접속가능 IP 인증을 통과해야만 채널리스트를 주도록함.
			if( m_kTimer[TM_CHANNEL_LIST].elapsed() < 60.f )
				break;

			m_kTimer[TM_CHANNEL_LIST].restart();

#ifdef SERVER_GROUP_UI_ADVANCED
			KECH_GET_SERVERGROUP_LIST_NOT kPacketNot;
			kPacketNot.m_mapServerGroupList = GetKChannelServer()->GetServerGroupList();
			SendPacket( ECH_GET_SERVERGROUP_LIST_NOT, kPacketNot );
#endif SERVER_GROUP_UI_ADVANCED

			//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
			if( m_bPassedEnterChannel == true )
			{
				SendChannelList();
			}
#else
			KECH_GET_CHANNEL_LIST_NOT kPacket;
			kPacket.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
			kPacket.m_mapChannelList = GetKChannelServer()->GetChannelList();
			//{{ 2012. 07. 2	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
			kPacket.m_mapChannelBonusList = GetKChannelServer()->GetChannelBonusList();
#endif SERV_CHANNEL_LIST_RENEWAL
			//}}
			SendPacket( ECH_GET_CHANNEL_LIST_NOT, kPacket );
#endif SERV_WAIT_QUEUE_SYSTEM
			//}}
		}
		break;

		//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	case KCHFSM::S_WAIT_QUEUE:
		{
			// 만약 인원제한모드가 아니면 체크조차 하지 않는다.
			switch( SiKWaitQueueManager()->GetMode() )
			{
			case KWaitQueueManager::WQ_NONE:
				{
					// 인증 패스!
					m_bPassedEnterChannel = true;
					SiKWaitQueueManager()->RegPassedUser( GetUID() );

					// 상태 이동!
					StateTransition( KCHFSM::I_TO_AUTHENTICATED );

					// 해당 유저에게 팝업을 띄우기 위해 패킷 전송
					KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
					kPacketNot.m_bPassed = true;
					SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

					// 채널 리스트 전송!
					SendChannelList();
				}				
				break;

			case KWaitQueueManager::WQ_CCU_LIMIT:
				{
					// 대기큐를 체크하자! 자기 차례가 오면 채널리스트를 클라이언트한테 보낸다!
					if( SiKWaitQueueManager()->IsExist( GetUID() ) == true )
					{
						if( SiKWaitQueueManager()->IsNextEnterUser( GetUID() ) == true )
						{
							LIF( SiKWaitQueueManager()->UnRegWaitUser( GetUID() ) );

							// 인증 패스!
							m_bPassedEnterChannel = true;
                            SiKWaitQueueManager()->RegPassedUser( GetUID() );

							// 상태 이동!
							StateTransition( KCHFSM::I_TO_AUTHENTICATED );

							// 해당 유저에게 팝업을 띄우기 위해 패킷 전송
							KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
							kPacketNot.m_bPassed = true;
							SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

							// 채널 리스트 전송!
							SendChannelList();
						}
						else
						{
							if( m_kTimer[TM_WAIT_QUEUE].elapsed() > 5.0 )
							{
								m_kTimer[TM_WAIT_QUEUE].restart();

								// 해당 유저에게 팝업을 띄우기 위해 패킷 전송
								KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
								LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
								SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );
							}
						}
					}
				}
				break;

			case KWaitQueueManager::WQ_ENTRY_BAN:
				{
					// 무조건 메시지만 보냅니다.
					if( m_kTimer[TM_WAIT_QUEUE].elapsed() > 5.0 )
					{
						m_kTimer[TM_WAIT_QUEUE].restart();

						// 해당 유저에게 팝업을 띄우기 위해 패킷 전송
						KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
						LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
						SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );
					}
				}
				break;

			default:
				// 그외에 것들은 아무런 동작도 하지 않습니다.
				break;
			}
		}
		break;
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}

    case KCHFSM::S_EXIT:
        KActorManager::GetKObj()->ReserveDelete( m_nUID );    // 자기 자신을 스스로 삭제할 수 없음.
        break;
    }
}

void KChannelUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() )
	{
    case KCHFSM::S_INIT:
    case KCHFSM::S_LOGINED:
		//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	case KCHFSM::S_WAIT_QUEUE:
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}
		{
			//{{ 2012. 07. 10	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
			SiKWaitQueueManager()->UnRegWaitUser( GetUID() );
			SiKWaitQueueManager()->UnRegPassedUser( GetUID() );
#endif SERV_WAIT_QUEUE_SYSTEM
			//}}

			StateTransition( KCHFSM::I_TO_EXIT );    // OnDestroy()가 불린 현재 KSession::Tick 처리 중이며,
			// KLoginUser::Tick 처리 단계에서 종료 대기명단에 등록된다.
		}
        break;
    }
}

bool KChannelUser::RoutePacket( const KEvent* pkEvent_ )
{
    if( pkEvent_->m_kDestPerformer.m_dwPerformerID != PI_CHANNEL_USER )
    {
        int iEval = CompareServerLevel( pkEvent_->m_kDestPerformer.m_dwPerformerID, PI_CHANNEL_USER );
        if( iEval < 0 )
        {
            _JIF( !pkEvent_->IsEmptyTrace(), return true );
            KEvent kEvent( *pkEvent_ );
            kEvent.PopTrace();
            LIF( KSession::SendPacket( kEvent ) );
            return true;
        }
        else if( iEval > 0 )
        {
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
			KEvent kEvent( *pkEvent_ );
			kEvent.PushTrace( GetUID() );
			SiKProxyManager()->SendPacket( GetUID(), kEvent );

			return true;
#else
            START_LOG( cerr, L"현재는 채널 서버가 최상위 서버이다." )
                << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( pkEvent_->m_usEventID )
                << BUILD_LOG( pkEvent_->GetIDStr() )
                << END_LOG;

            return true;
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
        }
        else
        {
            KEventPtr spEvent( pkEvent_->Clone() );
            spEvent->PushTrace( GetUID() );

            switch( GetPerformerClass( pkEvent_->m_kDestPerformer.m_dwPerformerID ) )
            {
            case PC_SERVER:
                KBaseServer::GetKObj()->QueueingEvent( spEvent );
                return true;
            case PC_LOG_DB:
                SiKDBLayer()->QueueingEvent( spEvent );
                return true;
            default:
                START_LOG( cerr, L"도착지가 이상한 패킷." )
                    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                    << END_LOG;
                return true;
            }
        }
    }

    return false;
}

bool KChannelUser::SendID( unsigned short usEventID )
{
	return KSession::SendID( PI_GS_PROXY, GetUID(), NULL, usEventID );
}

void KChannelUser::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

//{{ 2008. 8. 28  최육사	이벤트 필터
void KChannelUser::InitRepeatEventFilter()
{
	m_kRepeatEventFilter.RegRepeatEvent( ECH_VERIFY_ACCOUNT_REQ, ECH_VERIFY_ACCOUNT_ACK );
	m_kRepeatEventFilter.RegRepeatEvent( ECH_GET_CHANNEL_LIST_REQ, ECH_GET_CHANNEL_LIST_ACK );

	// 등록 카운트 검사
	if( m_kRepeatEventFilter.GetRegCount() != m_kRepeatEventFilter.GetReqEventCount()  ||  
		m_kRepeatEventFilter.GetRegCount() != m_kRepeatEventFilter.GetAckEventCount() )
	{
		START_LOG( cerr, L"등록한 이벤트 필터의 갯수가 틀립니다. 등록실패한 이벤트가 존재!" )
			<< BUILD_LOG( m_kRepeatEventFilter.GetRegCount() )
			<< BUILD_LOG( m_kRepeatEventFilter.GetReqEventCount() )
			<< BUILD_LOG( m_kRepeatEventFilter.GetAckEventCount() )
			<< END_LOG;
	}
}

bool KChannelUser::CheckReqEvent( unsigned short usEventID )
{
	switch( usEventID )
	{
	case ECH_VERIFY_ACCOUNT_REQ:
	case ECH_GET_CHANNEL_LIST_REQ:
#ifdef SERV_PUBLISHER_MATRIXKEY
	case ECH_SECURITY_AUTH_REQ:
#endif SERV_PUBLISHER_MATRIXKEY
		{
			return m_kRepeatEventFilter.CheckReqEvent( usEventID );
		}
		break;

	default:
		break;
	}

	return true;
}

void KChannelUser::CheckAckEvent( unsigned short usEventID )
{
	switch( usEventID )
	{
	case ECH_VERIFY_ACCOUNT_ACK:
	case ECH_GET_CHANNEL_LIST_ACK:
		{
			m_kRepeatEventFilter.CheckAckEvent( usEventID );
		}
		break;

	default:
		break;
	}
}

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST

void KChannelUser::CheckHackingToolList()
{
	if( m_kTimer[TM_HACKING_TOOL_LIST].elapsed() < m_kUserHackingManager.GetHackingToolCheckTime() )
		return;

	m_kTimer[TM_HACKING_TOOL_LIST].restart();

	KEGS_HACKING_TOOL_LIST_NOT kNot;
	m_kUserHackingManager.GetHackingToolList( kNot.m_iRandomKey, kNot.m_vecHackingToolList );
	SendPacket( EGS_HACKING_TOOL_LIST_NOT, kNot );
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2013.03.19 조효진 해외 공용 인증 시스템 구성 (해외는 인증 패킷 처리 따로함)
#ifdef SERV_GLOBAL_AUTH
IMPL_ON_FUNC( ECH_VERIFY_ACCOUNT_REQ )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    START_LOG( clog, L"인증 요청." );

    KECH_VERIFY_ACCOUNT_ACK kPacket;

    //{{ 2009. 6. 10  최육사	접속IP제한
    if( GetKChannelSimLayer()->CheckIP( GetIPStr() ) == false )
    {
		if( GetKChannelSimLayer()->GetCheckIPMode() == KChannelSimLayer::CIM_SERVICE_MODE )
		{
			START_LOG( cout, L"[서비스 모드] 접속 차단 대상 IP의 유저가 접속시도를 하였습니다 : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}
		else
		{
            START_LOG( cout, L"[점검 모드] 점검 중에 일반 유저 접속 : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}

        kPacket.m_iOK = NetError::ERR_CONNECT_05;
        SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

		// 종료 예약
		ReserveDestroy();
		return;
	}
	//}}

	//{{ 2011. 1. 24 조효진		*중요* 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrID ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}
	//}}

#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrPassword ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrent = CTime::GetCurrentTime();
	kPacket.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
	if(KSimLayer::AF_INTERNAL == dwAuthFlag)
	{
		if( KSimLayer::GetKObj()->GetUseKogOTP() )
		{
			m_wstrID = kPacket_.m_wstrID;

			if (!kPacket_.m_bIDVerified)	// 퍼블리셔 인증 받은 후인지를 체크
			{
				// 원래는 퍼블인증 요청해야지만 사내 테스트용으로 우회함
				// 그럼에도 KOG OTP 사용 시 OTP 숫자 갱신을 위해 
				// Publisher 인증 제대로 받았을 때 요청하는 DBE_CH_USER_GENERAL_LOGIN_REQ 요청해야 함

				KDBE_CH_USER_GENERAL_LOGIN_REQ kPacketReq;
				kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
				kPacketReq.m_wstrIP = KncUtil::toWideString(KSession::GetIPStr());
				kPacketReq.m_iChannelingCode = kPacket_.m_iChannelingCode;
#ifdef SERV_PURCHASE_TOKEN
				kPacketReq.m_wstrPurchaseTok = L"";
#endif SERV_PURCHASE_TOKEN
				kPacketReq.m_bServerUseKogOTP = KSimLayer::GetKObj()->GetUseKogOTP();

#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
				kPacketReq.m_wstrPassword = kPacket_.m_wstrPassword;
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD

				SendToAccountDB( DBE_CH_USER_GENERAL_LOGIN_REQ, kPacketReq );
			}
			else
			{
				// 퍼블인증 통과했으면 KOG OTP 요청
				RequestKOGOTPLogin(kPacket_);
			}
		}
		else
		{
			StateTransition( KCHFSM::I_TO_AUTHENTICATED );
			kPacket.m_iOK = NetError::NET_OK;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			
		}

		return;
	}

	if( KSimLayer::GetKObj()->GetUseKogOTP() )
	{
		// - KOG OTP 사용 국가의 경우
		// KOG OTP 사용 국가의 경우 퍼블리셔 인증 -> KOG OTP 인증 2가지 스탭으로 나눠진다. 

		if (!kPacket_.m_bIDVerified)
		{
			m_wstrID = kPacket_.m_wstrID;	// 다른 인증 프로세스 같이 쓰는 경우 임시로 ID 저장해야 되는 경우가 있음

			// 퍼블인증 요청
			RequestPublisherLogin(kPacket_);
		}
		else
		{
			// 퍼블인증 통과했으면 KOG OTP 요청
			RequestKOGOTPLogin(kPacket_);
		}
	}
	else
	{
		m_wstrID = kPacket_.m_wstrID;	// 다른 인증 프로세스 같이 쓰는 경우 임시로 ID 저장해야 되는 경우가 있음

		// - KOG OTP 미사용 국가의 경우
		// 퍼블리셔 인증만 받으면 됨
		RequestPublisherLogin(kPacket_);
	}
}

#else // SERV_GLOBAL_AUTH
// 국내 인증 처리 부분
IMPL_ON_FUNC( ECH_VERIFY_ACCOUNT_REQ )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    START_LOG( clog, L"인증 요청." );

    KECH_VERIFY_ACCOUNT_ACK kPacket;

    //{{ 2009. 6. 10  최육사	접속IP제한
    if( GetKChannelSimLayer()->CheckIP( GetIPStr() ) == false )
    {
#ifndef SERV_PRIVACY_AGREEMENT
		if( GetKChannelSimLayer()->GetCheckIPMode() == KChannelSimLayer::CIM_SERVICE_MODE )
		{		
			START_LOG( cout, L"[서비스 모드] 접속 차단 대상 IP의 유저가 접속시도를 하였습니다 : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}
		else
		{
            START_LOG( cout, L"[점검 모드] 점검 중에 일반 유저 접속 : " << GetIPStr() << L" ,  UserID : " << kPacket_.m_wstrID );
		}
#endif SERV_PRIVACY_AGREEMENT	

        kPacket.m_iOK = NetError::ERR_CONNECT_05;
        SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

        // 종료 예약
        ReserveDestroy();
        return;
    }
    //}}
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrent = CTime::GetCurrentTime();
	kPacket.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

    DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
    switch( dwAuthFlag )
    {
    case KSimLayer::AF_INTERNAL:
		{
			StateTransition( KCHFSM::I_TO_AUTHENTICATED );
	     
		//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
			if( kPacket_.m_bUseVerify == true )
			{
				SetUserID( kPacket_.m_wstrID );

				//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				if( SetMachineID( kPacket_.m_strMachineID ) == true )
				{
					if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// 오픈 테섭에서는 검사 제외
					{
						if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
						{
							// 정상적인 MachineID 이므로 MachineID Block 검사를 한다.
							if( SiKMachineBlockManager()->IsBlockedMachineID( kPacket_.m_strMachineID ) == true )
							{
								// 블럭된 Machine ID 입니다.
								kPacket.m_iOK = NetError::ERR_VERIFY_17;
								SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

								// 종료 예약
								ReserveDestroy();
								return;
							}
						}
					}
				}
				else
				{
					// 비정상적인 MachineID 이므로 실패 처리한다.
					kPacket.m_iOK = NetError::ERR_VERIFY_17;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
					// 종료 예약
					ReserveDestroy();
					return;
				}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				//}}
				//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				if( SiKGameSysVal()->GetCheckServerSN() == true )
				{
					// 클라가 전달해준 sn 이 있다
					if( KServerSNMaker::IsEmptyServerSN( kPacket_.m_charServerSN ) == false )
					{
						// 검사해 볼까?
						// 데이터 유효성 검사
						__int64 iServerSN = 0;
						if( KServerSNMaker::CheckServerSN( kPacket_.m_charServerSN, iServerSN ) == false )
						{
							START_LOG( cerr, L"유효하지 않은 ServerSN을 클라가 줬어!" )
								<< BUILD_LOG( kPacket_.m_wstrID )
								<< BUILD_LOG( iServerSN );

							// 클라가 파일에서 읽은 12바이트 정보가 변조 되었다.
							//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
							KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							//}}
							
							KDBE_CREATE_SERVER_SN_REQ kCreateSN;
							kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
							SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
							return;
						}

						// DB에 존제하는 ServerSN 인지 확인하자
						KDBE_CHECK_SERVER_SN_REQ kCheckSN;
						kCheckSN.m_iServerSN = iServerSN;
						kCheckSN.m_strMachineID = kPacket_.m_strMachineID;
						kCheckSN.m_wstrID = kPacket_.m_wstrID;
						SendToAccountDB( DBE_CHECK_SERVER_SN_REQ, kCheckSN );

						START_LOG( clog, L"ServerSN 번호 확인용" )
							<< BUILD_LOG( kPacket_.m_wstrID )
							<< BUILD_LOG( iServerSN );

						return;
						
					}
					else // 클라가 전달해준 sn 이 없다
					{
						// 새로 sn을 생성 하자
						KDBE_CREATE_SERVER_SN_REQ kCreateSN;
						kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
						SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
						return;
					}
				}
				else
				{
					KECH_HACKING_TOOL_LIST_REQ kReq;
					SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
					SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

					m_bVerifyAccount = true;
					m_tVerifyAccount = CTime::GetCurrentTime();

					START_LOG( clog, L"해킹툴 리스트 전송 완료!!" )
						<< BUILD_LOG( kReq.m_iRandomKey )
						<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
						<< END_LOG;
				}
#else
				KECH_HACKING_TOOL_LIST_REQ kReq;
				SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
				SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

				m_bVerifyAccount = true;
				m_tVerifyAccount = CTime::GetCurrentTime();

				START_LOG( clog, L"해킹툴 리스트 전송 완료!!" )
					<< BUILD_LOG( kReq.m_iRandomKey )
					<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
					<< END_LOG;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				//}}
			}
			else
			{
				if( m_bVerifyAccount )
				{
					kPacket.m_iOK = NetError::NET_OK;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
				}
				else
				{
					kPacket.m_iOK = NetError::NET_OK;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
				}
			}
			return;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
			kPacket.m_iOK = NetError::NET_OK;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
    case KSimLayer::AF_NEXON_KOREA:
		{
			StateTransition( KCHFSM::I_TO_AUTHENTICATED );

		//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
			if( kPacket_.m_bUseVerify == true )
			{
				SetUserID( kPacket_.m_wstrID );

				//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				if( SetMachineID( kPacket_.m_strMachineID ) == true )
				{
					if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// 오픈 테섭에서는 검사 제외
					{
						if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
						{
							// 정상적인 MachineID 이므로 Account Block 검사를 한다.
							if( SiKMachineBlockManager()->IsBlockedMachineID( kPacket_.m_strMachineID ) == true )
							{
								// 블럭된 Machine ID 입니다.
								kPacket.m_iOK = NetError::ERR_VERIFY_17;
								SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

								// 종료 예약
								ReserveDestroy();
								return;
							}
						}
					}
				}
				else
				{
					// 비정상적인 MachineID 이므로 실패 처리한다.
					kPacket.m_iOK = NetError::ERR_VERIFY_17;
					SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
					// 종료 예약
					ReserveDestroy();
					return;
				}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
				//}}
				//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				if( SiKGameSysVal()->GetCheckServerSN() == true )
				{
					// 클라가 전달해준 sn 이 있다
					if( KServerSNMaker::IsEmptyServerSN( kPacket_.m_charServerSN ) == false )
					{
						// 검사해 볼까?
						// 데이터 유효성 검사
						__int64 iServerSN = 0;
						if( KServerSNMaker::CheckServerSN( kPacket_.m_charServerSN, iServerSN ) == false )
						{
							START_LOG( cerr, L"유효하지 않은 ServerSN을 클라가 줬어!" )
								<< BUILD_LOG( kPacket_.m_wstrID )
								<< BUILD_LOG( iServerSN );

							// 클라가 파일에서 읽은 12바이트 정보가 변조 되었다.
							//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
							KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
							kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_CHECK_SUM_SEND_MAIL;
							SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
							//}}
							
							KDBE_CREATE_SERVER_SN_REQ kCreateSN;
							kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
							SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
							return;
						}

						// DB에 존제하는 ServerSN 인지 확인하자
						KDBE_CHECK_SERVER_SN_REQ kCheckSN;
						kCheckSN.m_iServerSN = iServerSN;
						kCheckSN.m_strMachineID = kPacket_.m_strMachineID;
						kCheckSN.m_wstrID = kPacket_.m_wstrID;
						SendToAccountDB( DBE_CHECK_SERVER_SN_REQ, kCheckSN );
						return;

					}
					else // 클라가 전달해준 sn 이 없다
					{
						// 새로 sn을 생성 하자
						KDBE_CREATE_SERVER_SN_REQ kCreateSN;
						kCreateSN.m_strMachineID = kPacket_.m_strMachineID;
						SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
						return;
					}
				}
				else
				{
					KECH_HACKING_TOOL_LIST_REQ kReq;
					SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
					SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

					m_bVerifyAccount = true;
					m_tVerifyAccount = CTime::GetCurrentTime();

					START_LOG( clog, L"해킹툴 리스트 전송 완료!!" )
						<< BUILD_LOG( kReq.m_iRandomKey )
						<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
						<< END_LOG;
				}
#else
				KECH_HACKING_TOOL_LIST_REQ kReq;
				SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
				SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

				m_bVerifyAccount = true;
				m_tVerifyAccount = CTime::GetCurrentTime();

				START_LOG( clog, L"해킹툴 리스트 전송 완료!!" )
					<< BUILD_LOG( kReq.m_iRandomKey )
					<< BUILD_LOG( kReq.m_vecHackingToolList.size() )
					<< END_LOG;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
				//}}
			}
			else
			{
				kPacket.m_iOK = NetError::NET_OK;
				SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			}
			return;
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
			kPacket.m_iOK = NetError::NET_OK;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
    //case KSimLayer::AF_GAMANIA_TAIWAN:
    //    if( kPacket_.m_bLogin )
    //    {
    //        // GASH 인증
    //        KEGASH_AUTHORIZATION_REQ kPacketReq;
    //        kPacketReq.m_iActorUID = GetUID();
    //        kPacketReq.m_strServiceCode = SiKGASHAuthManager()->GetServiceCode();
    //        kPacketReq.m_strServiceRegion = SiKGASHAuthManager()->GetServiceRegion();
    //        kPacketReq.m_strServiceAccountID = KncUtil::toNarrowString( kPacket_.m_wstrID );
    //        kPacketReq.m_strServicePassword = KncUtil::toNarrowString( kPacket_.m_wstrPassword );
    //        kPacketReq.m_strAuthenticationFlag = "1";
    //        kPacketReq.m_strType = "I";
    //        kPacketReq.m_strDuplicateCheck = "0";
    //        kPacketReq.m_strIPAddress = KSession::GetIPStr();
    //        kPacketReq.m_strPacketID = KncUtil::toNarrowString( kPacket_.m_wstrID );

    //        KEventPtr spEvent( new KEvent );
    //        spEvent->SetData(PI_NULL, NULL, EGASH_AUTHORIZATION_REQ, kPacketReq );
    //        SiKGASHAuthManager()->QueueingEvent( spEvent );
    //    }
    //    else
    //    {
    //        // 자체 passport 인증
    //        KDBE_GASH_USER_LOGIN_REQ kPacketReq;
    //        kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
    //        kPacketReq.m_wstrOTP = kPacket_.m_wstrPassword;

    //        SendToAccountDB( DBE_GASH_USER_LOGIN_REQ, kPacketReq );
    //    }
    //    break;
    default:
		{
			START_LOG( cerr, L"인증 플래그 이상." )
				<< BUILD_LOG( dwAuthFlag )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
    }
}
#endif // SERV_GLOBAL_AUTH
//}}

#ifndef SERV_GLOBAL_AUTH 
IMPL_ON_FUNC( ECH_VERIFY_ACCOUNT_ACK )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket_ );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        StateTransition( KCHFSM::I_TO_AUTHENTICATED );
    }
    else
    {
        ReserveDestroy();
    }
}
#endif // SERV_GLOBAL_AUTH

/*
IMPL_ON_FUNC( DBE_GASH_USER_LOGIN_ACK )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

    KECH_VERIFY_ACCOUNT_ACK kPacket;
    kPacket.m_iOK = kPacket_.m_iOK;
    kPacket.m_iUserUID = kPacket_.m_iUserUID;
    kPacket.m_wstrPassport = kPacket_.m_wstrOTP;

    SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        StateTransition( KCHFSM::I_TO_AUTHENTICATED );
    }
    else
    {
        ReserveDestroy();
    }

}
//*/

//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( ECH_GET_CHANNEL_LIST_REQ )
{
	switch( GetStateID() )
	{
	case KCHFSM::S_LOGINED:
		break;

	case KCHFSM::S_WAIT_QUEUE:
		{
			KECH_GET_CHANNEL_LIST_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			SendPacket( ECH_GET_CHANNEL_LIST_ACK, kPacketAck );
		}
		break;

	default:
		{
			START_LOG( cerr, L"상태 오류. Name : " << m_strName )
				<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
				<< L"    허용 상태 : KCHFSM::S_LOGINED" << END_LOG;
			return;
		}
		break;
	}

	KECH_GET_CHANNEL_LIST_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( ECH_GET_CHANNEL_LIST_ACK, kPacketAck );

	// 현재 모드에 따른 동작
	switch( SiKWaitQueueManager()->GetMode() )
	{
	case KWaitQueueManager::WQ_NONE:
		{
			if( m_bPassedEnterChannel == false )
			{
				// 그냥 바로 등록!
				m_bPassedEnterChannel = true;
				SiKWaitQueueManager()->RegPassedUser( GetUID() );
			}
		}
		break;

	case KWaitQueueManager::WQ_CCU_LIMIT:
		{
			// 채널 리스트를 받을수 있도록 인증을 받았는가?
			if( m_bPassedEnterChannel == false )
			{
				// 일단 현재 동접이 얼만큼인지 채널이 입장할만큼 인원이 차있는지 본다!	
				m_bPassedEnterChannel = SiKWaitQueueManager()->IsCanEnterChannel();
				if( m_bPassedEnterChannel == false )
				{
					// 해당 유저를 대기열에 등록한다!
					LIF( SiKWaitQueueManager()->RegWaitUser( GetUID() ) );

					// 해당 유저에게 팝업을 띄우기 위해 패킷 전송
					KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
					LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
					SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

					// 상태 이동
					StateTransition( KCHFSM::I_TO_WAIT_QUEUE );
					return;
				}
				else
				{
					// 통과한 유저 등록!
					SiKWaitQueueManager()->RegPassedUser( GetUID() );
				}
			}
		}
		break;

	case KWaitQueueManager::WQ_ENTRY_BAN:
		{
			// 해당 유저를 대기열에 등록한다!
			LIF( SiKWaitQueueManager()->RegWaitUser( GetUID() ) );

			// 해당 유저에게 팝업을 띄우기 위해 패킷 전송
			KECH_WAIT_QUEUE_MESSAGE_NOT kPacketNot;
			LIF( SiKWaitQueueManager()->GetWaitUserCount( GetUID(), kPacketNot.m_iWaitUserCount ) );
			SendPacket( ECH_WAIT_QUEUE_MESSAGE_NOT, kPacketNot );

			// 상태 이동
			StateTransition( KCHFSM::I_TO_WAIT_QUEUE );
			return;
		}
		break;

	default:
		{
			START_LOG( cerr, L"존재 하지 않는 모드입니다!" )
				<< BUILD_LOG( SiKWaitQueueManager()->GetMode() )
				<< END_LOG;
		}
		return;
	}
	
	// 채널 리스트 전송
	SendChannelList();
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( ECH_GET_CHANNEL_LIST_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_LOGINED ) );

	KECH_GET_CHANNEL_LIST_ACK kPacketAck;	
	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( ECH_GET_CHANNEL_LIST_ACK, kPacketAck );

	KECH_GET_CHANNEL_LIST_NOT kPacketNot;
	kPacketNot.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketNot.m_mapChannelList = GetKChannelServer()->GetChannelList();
	//{{ 2012. 07. 2	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	kPacketNot.m_mapChannelBonusList = GetKChannelServer()->GetChannelBonusList();
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
	SendPacket( ECH_GET_CHANNEL_LIST_NOT, kPacketNot );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

IMPL_ON_FUNC_NOPARAM( ECH_DISCONNECT_REQ )
{
	//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	VERIFY_STATE( ( 3, KCHFSM::S_INIT, KCHFSM::S_LOGINED, KCHFSM::S_WAIT_QUEUE ) );
#else
	VERIFY_STATE( ( 2, KCHFSM::S_INIT, KCHFSM::S_LOGINED ) );
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	if( GetStateID() == KCHFSM::S_WAIT_QUEUE )
	{
		if( SiKWaitQueueManager()->IsExist( GetUID() ) == true )
		{
			LIF( SiKWaitQueueManager()->UnRegWaitUser( GetUID() ) );
		}
	}
#endif SERV_WAIT_QUEUE_SYSTEM
	//}}

	SendID( ECH_DISCONNECT_ACK );

	ReserveDestroy();
}

//{{ 2009. 12. 16  최육사	동접툴
IMPL_ON_FUNC( E_TOOL_GET_CCU_INFO_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	// 캐싱데이터 확인
	KE_TOOL_GET_CCU_INFO_ACK kPacketAck;
	if( SiKCCUToolManager()->GetExistCCU( kPacket_, kPacketAck ) == true )	// 요청한 날짜의 CCU 데이타가 있다
	{
		kPacketAck.m_iOK = NetError::NET_OK;
		SendPacket( E_TOOL_GET_CCU_INFO_ACK, kPacketAck );
		return;
	}

	KDBE_GET_CONCURRENT_USER_INFO_REQ kPacketToDB;
	kPacketToDB.m_cCCUType = kPacket_.m_cCCUType;
	kPacketToDB.m_iServerUID = kPacket_.m_iServerUID;
	kPacketToDB.m_wstrDate = kPacket_.m_wstrDate;
	SendToLogDB( DBE_GET_CONCURRENT_USER_INFO_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_GET_CONCURRENT_USER_INFO_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	KE_TOOL_GET_CCU_INFO_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	kAck.m_cCCUType = kPacket_.m_cCCUType;
	kAck.m_iServerUID = kPacket_.m_iServerUID;
	kAck.m_wstrDate = kPacket_.m_wstrDate;
	kAck.m_vecCCUInfo = kPacket_.m_vecCCUInfo;

	CTime tCCUDate;

	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrDate, tCCUDate ) == false )
	{
		START_LOG( cerr, L"날짜 변경 실패!" )
			<< BUILD_LOG( kPacket_.m_iServerUID )
			<< BUILD_LOG( kPacket_.m_cCCUType )
			<< BUILD_LOG( kPacket_.m_wstrDate )
			<< END_LOG;
		return;
	}

	const KCCUToolManager::CCUDateData* mapCCUData = SiKCCUToolManager()->GetCCUData( tCCUDate );
	if( mapCCUData == NULL ) 
	{
		if( SiKCCUToolManager()->NewCCUData( kPacket_.m_iServerUID, tCCUDate.GetTime(), kPacket_.m_vecCCUInfo ) == false )
		{
			//[동접툴] 로그 삭제
			START_LOG( cerr, L"동접 정보 추가 실패! 기존 정보 있는듯?" )
				<< BUILD_LOG( kPacket_.m_iServerUID )
				<< BUILD_LOG( kPacket_.m_cCCUType )
				<< BUILD_LOG( kPacket_.m_wstrDate )
				<< END_LOG;

			return;
		}
	}
	else
	{
		if( SiKCCUToolManager()->SetCCUData( tCCUDate.GetTime(), kPacket_.m_iServerUID, kPacket_.m_vecCCUInfo, kPacket_.m_cCCUType ) == false ) 
		{
			return;
		}
	}

    SendPacket( E_TOOL_GET_CCU_INFO_ACK, kAck );
}
//}}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
	m_bDestroyReserved = true;
}



//{{ 2011. 01. 20 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
IMPL_ON_FUNC( E_TOOL_CHECK_LOGIN_REQ )
{
	// ID, PW 둘 중 하나라도 특수 문자가 포함 되어 있다면 
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrID ) || KODBC::IsInvalidMarkIn( kPacket_.m_wstrPW ) )
	{
		START_LOG( cout, L"특수 문자 사용한 ID, PW 입력으로 접속 종료!")
			<< BUILD_LOG( kPacket_.m_wstrID )
			<< BUILD_LOG( kPacket_.m_wstrPW )
			<< END_LOG;

		ReserveDestroy();
		return;
	}

	KDBE_GET_CONCURRENT_USER_LOGIN_REQ kPacketReq;

	kPacketReq.m_wstrID = kPacket_.m_wstrID;
	kPacketReq.m_wstrPW	= kPacket_.m_wstrPW;

 	SendToAccountDB(DBE_GET_CONCURRENT_USER_LOGIN_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_GET_CONCURRENT_USER_LOGIN_ACK )
{
	KE_TOOL_CHECK_LOGIN_ACK Login_ACK;

	if( kPacket_.m_iAuthLevel < 0 )
	{
		START_LOG( cout, L"로그인 실패로 인한 접속 종료!")
		<< BUILD_LOG( kPacket_.m_iAuthLevel )
			<< END_LOG;
		ReserveDestroy();
		return;
	}

	Login_ACK.m_iAuthLevel = kPacket_.m_iAuthLevel;
	
	SendPacket( E_TOOL_CHECK_LOGIN_ACK, Login_ACK );	
}

IMPL_ON_FUNC_NOPARAM( E_TOOL_SERVER_LIST_REQ )
{
	SendToLogDB( DBE_GET_GAME_SERVER_LIST_REQ );
}

IMPL_ON_FUNC( DBE_GET_GAME_SERVER_LIST_ACK )
{
	SendPacket( E_TOOL_SERVER_LIST_ACK, kPacket_ );
}

#endif SERV_CCU_MONITORING_TOOL
//}}

//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
IMPL_ON_FUNC( ECH_HACKING_TOOL_LIST_ACK )
{
	KDBE_UPDATE_CHANNEL_RANDOMKEY_REQ kReq;
	m_bVerifyAccount = false;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 성공 했으니 인증키 발급
		kReq.m_wstrUserID = GetUserID();

		// 체험 아이디의 경우 검사하지 않으므로 DB 기록 하지 않는다.
		if( kReq.m_wstrUserID.empty() == true )  // 체험 아이디로 판단한다.  
		{										 // 채널 서버에서는 UserID를 클라이언트에게 받는데 클라이언트는 체엄 아이디 일때 알지 못한다.
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
			kPacket.m_iOK = kPacket_.m_iOK;
			kPacket.m_iChannelRandomKey = -58566;
			kPacket.m_bUseVerify = true;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			return;
		}

		SendToAccountDB( DBE_UPDATE_CHANNEL_RANDOMKEY_REQ, kReq );
	}
}

IMPL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_ACK )
{
	KECH_VERIFY_ACCOUNT_ACK kPacket;
	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();

	switch( dwAuthFlag )
	{
	case KSimLayer::AF_INTERNAL:
	case KSimLayer::AF_NEXON_KOREA:
		{
			kPacket.m_iOK = kPacket_.m_iOK;
			kPacket.m_iChannelRandomKey = kPacket_.m_iRandomKey;
			kPacket.m_bUseVerify = true;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
	default:
		{
			START_LOG( cerr, L"인증 플래그 이상." )
				<< BUILD_LOG( dwAuthFlag )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			break;
		}
	}
}

void KChannelUser::CheckVerifyAccountRecv()
{
	if( m_bVerifyAccount == true )
	{
		CTime tNow = CTime::GetCurrentTime();
		if( m_tVerifyAccount + 10 < tNow )
		{
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );

			m_bVerifyAccount = false;

			// 종료 예약
			ReserveDestroy();
		}
	}
}

void KChannelUser::SetUserID( IN std::wstring& wstrUserID )
{
	// 쿼리에 직접 안들어가지만 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( wstrUserID ) )
	{
		START_LOG( cerr, L"ChannelUserID 가 정상적이지 않은 문자가 있다." )
			<< BUILD_LOG( wstrUserID );
		return;
	}

	m_wstrUserID = wstrUserID;
}
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
bool KChannelUser::SetMachineID( IN const std::string& strMachineID )
{
	if( m_kUserSecurityManager.CheckValidMachineID( strMachineID ) == true )
	{
		m_kUserSecurityManager.SetMachineID( strMachineID );
		return true;
	}

	return false;
}

std::string KChannelUser::GetMachineID()
{
	return m_kUserSecurityManager.GetStringMachineID();
}
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
//}}

//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
void KChannelUser::SendChannelList()
{
	KECH_GET_CHANNEL_LIST_NOT kPacketNot;
	kPacketNot.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketNot.m_mapChannelList = GetKChannelServer()->GetChannelList();
	//{{ 2012. 07. 2	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	kPacketNot.m_mapChannelBonusList = GetKChannelServer()->GetChannelBonusList();
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
	SendPacket( ECH_GET_CHANNEL_LIST_NOT, kPacketNot );
}
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
IMPL_ON_FUNC( DBE_CHECK_SERVER_SN_ACK )
{
	// DB에 존재하는 SN이다.
	switch( kPacket_.m_iOK )
	{
	case NetError::NET_OK:
		{
			
		}break;
	case NetError::ERR_SERVERSN_01:
		{
			// 재발급
			KECH_NEW_SERVER_SN_NOT kNot;
			if( kPacket_.m_iServerSN > 0 && KServerSNMaker::MakeServerSN( kPacket_.m_iServerSN, kNot.m_charServerSN ) == true )
			{
				SendPacket( ECH_NEW_SERVER_SN_NOT, kNot );
			}
			
			// 메일
			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
		}break;
	case NetError::ERR_SERVERSN_02:
		{
			// 메일
			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SN_AND_MID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SN_AND_MID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
			
			// 생성
			KDBE_CREATE_SERVER_SN_REQ kCreateSN;
			kCreateSN.m_strMachineID = GetMachineID();;
			SendToAccountDB( DBE_CREATE_SERVER_SN_REQ, kCreateSN );
			return;
		}break;
	case NetError::ERR_SERVERSN_03:
		{
			// 메일
			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_MACHINE_ID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
			kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_MACHINE_ID_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
		}break;
	case NetError::ERR_SERVERSN_07:   // 제재된 sn
		{
			// 종료
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// 종료 예약
			ReserveDestroy();
		}break;
	default:
		{
			// 실패한 이유가 무엇인가?
			START_LOG( cerr, L"SN & MID 체크에서 실패한 이유가 무엇인가?" )
				<< BUILD_LOG( kPacket_.m_iOK );

			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// 종료 예약
			ReserveDestroy();
		}break;
	}

	KECH_HACKING_TOOL_LIST_REQ kReq;
	SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
	SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

	m_bVerifyAccount = true;
	m_tVerifyAccount = CTime::GetCurrentTime();
}

IMPL_ON_FUNC( DBE_CREATE_SERVER_SN_ACK )
{
	// DB에서 생성하였다.
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// send ECH_NEW_SERVER_SN_NOT
		KECH_NEW_SERVER_SN_NOT kNot;
		if( KServerSNMaker::MakeServerSN( kPacket_.m_iServerSN, kNot.m_charServerSN ) == true )
		{
			SendPacket( ECH_NEW_SERVER_SN_NOT, kNot );
		}
		else
		{
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// 종료 예약
			ReserveDestroy();
			return;
		}

		// send ECH_HACKING_TOOL_LIST_REQ
		KECH_HACKING_TOOL_LIST_REQ kReq;
		SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
		SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

		m_bVerifyAccount = true;
		m_tVerifyAccount = CTime::GetCurrentTime();
	}
	else if( kPacket_.m_iOK == NetError::ERR_SERVERSN_05 )
	{
		// send ECH_NEW_SERVER_SN_NOT
		KECH_NEW_SERVER_SN_NOT kNot;
		if( KServerSNMaker::MakeServerSN( kPacket_.m_iServerSN, kNot.m_charServerSN ) == true )
		{
			SendPacket( ECH_NEW_SERVER_SN_NOT, kNot );
		}
		else
		{
			KECH_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
			// 종료 예약
			ReserveDestroy();
			return;
		}

		// send ECH_HACKING_TOOL_LIST_REQ
		KECH_HACKING_TOOL_LIST_REQ kReq;
		SiKHackingProtectionManager()->GetHackingToolList( kReq.m_iRandomKey, kReq.m_vecHackingToolList );
		SendPacket( ECH_HACKING_TOOL_LIST_REQ, kReq );

		m_bVerifyAccount = true;
		m_tVerifyAccount = CTime::GetCurrentTime();

		// 감시 모드 발동
		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kHack;
		kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_SEND_MAIL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kHack );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kHack;
		kHack.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_SEND_MAIL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kHack );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
	}
	else
	{
		// DB에서 생성하지 못했다.
		KECH_VERIFY_ACCOUNT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_VERIFY_17;
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacket );
		// 종료 예약
		ReserveDestroy();
	}
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}

#ifdef SERV_GLOBAL_AUTH
IMPL_ON_FUNC( EPUBLISHER_AUTHENTICATION_ACK )
{
    VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	// 국가별 특이 처리 추가 (국가별 넷에러값 처리)//
	bool bDoNotDisconnect = false;

#ifdef SERV_LOGIN_RESULT_INFO
	ProcessLoginResultCount( kPacket_.m_iOK );			// 로그인 인증 결과 처리( 퍼블리셔 인증을 거치는 국가는 여기에서 처리 )
#endif SERV_LOGIN_RESULT_INFO

	HandlePublisherLoginAck(kPacket_, bDoNotDisconnect);
		
	if (kPacket_.m_iOK == NetError::NET_OK)				// 퍼블리셔 인증 성공
	{
		KDBE_CH_USER_GENERAL_LOGIN_REQ kPacketReq;
		kPacketReq.m_wstrServiceAccountID = kPacket_.m_wstrID;
		kPacketReq.m_uiPublisherUID = kPacket_.m_uiPublisherUID;
		kPacketReq.m_wstrIP = KncUtil::toWideString(KSession::GetIPStr());
		kPacketReq.m_iChannelingCode = kPacket_.m_iChannelingCode;
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		kPacketReq.m_uiAccountType = kPacket_.m_uiAccountType;
#endif SERV_ANTI_ADDICTION_SYSTEM

#ifdef SERV_PURCHASE_TOKEN
		kPacketReq.m_wstrPurchaseTok = kPacket_.wstrPurchTok;
#endif SERV_PURCHASE_TOKEN
		kPacketReq.m_bServerUseKogOTP = KSimLayer::GetKObj()->GetUseKogOTP();
#ifdef SERV_COUNTRY_TH
		SetMasterID( kPacket_.m_wstrMasterID );
		SetSocketID( kPacket_.m_wstrSocketID );
		kPacketReq.m_wstrMasterID = kPacket_.m_wstrMasterID;
		kPacketReq.m_wstrSocketID = kPacket_.m_wstrSocketID;
#endif // SERV_COUNTRY_TH

		SendToAccountDB( DBE_CH_USER_GENERAL_LOGIN_REQ, kPacketReq );
	}
	else												// 퍼블리셔 인증 실패
	{
		KECH_VERIFY_ACCOUNT_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrUserID = L"";
		//	kPacketAck.m_iUserUID = kPacket_.m_iUserUID;
		kPacketAck.m_wstrPassport = L"";
#ifdef SERV_PURCHASE_TOKEN
		kPacketAck.m_wstrPurchaseTok = L"";
#endif SERV_PURCHASE_TOKEN

#ifdef SERV_LOGIN_TOU // 약관 미동의로 로그인 실패 시에 사용함
		kPacketAck.m_strAgreementURL = kPacket_.m_strAgreementURL;
#endif //SERV_LOGIN_TOU

		// Client ACK 날림 //
		SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacketAck );
		
		if (bDoNotDisconnect)
		{
			// 패스워드 잘못 입력했을 때에는 끊지 않는다.
			START_LOG( clog2, L"아이디 또는 패스워드 다름 또는 GF 계정 인증 안된 것임. (접속 안 끊음)" );
		}
		else
		{
			ReserveDestroy();
		}
	}	
}

IMPL_ON_FUNC( DBE_CH_USER_GENERAL_LOGIN_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );
	
	KECH_VERIFY_ACCOUNT_ACK kPacketAck;

	kPacketAck.m_iOK = kPacket_.m_iOK;
	kPacketAck.m_wstrUserID = kPacket_.m_wstrUserID;
	kPacketAck.m_iUserUID = kPacket_.m_iUserUID;
	kPacketAck.m_wstrPassport = kPacket_.m_wstrOTP;
	kPacketAck.m_iChannelingCode = kPacket_.m_iChannelingCode;
#ifdef SERV_PURCHASE_TOKEN
	kPacketAck.m_wstrPurchaseTok = kPacket_.m_wstrPurchaseTok;
#endif SERV_PURCHASE_TOKEN
#ifndef SERV_SERVER_TIME_GET
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrent = CTime::GetCurrentTime();
	kPacketAck.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_MASSFILE_MAPPING_FUNCTION
#endif SERV_SERVER_TIME_GET

#ifdef SERV_COUNTRY_TH
	kPacketAck.m_wstrSocketID = GetSocketID();
#endif //SERV_COUNTRY_TH
	
	START_LOG( clog, L"[테스트 로그] Send Packet Token 정상 확인" )
		<< BUILD_LOG( kPacketAck.m_iOK )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacketAck.m_wstrUserID )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacketAck.m_iUserUID )
		<< BUILD_LOG( kPacketAck.m_wstrPassport )
		<< BUILD_LOG( kPacketAck.m_wstrPurchaseTok )
		<< END_LOG;

	SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacketAck );

#ifdef SERV_LOGIN_RESULT_INFO
	// 사내 인증 또는 퍼블리셔 인증 서버로부터 인증받지 않고 자체 인증하는 경우(인도네시아, 필리핀)는 여기서 로그인 카운트 올림
	// 인도네시아, 필리핀은 EPUBLISHER_AUTHENTICATION_ACK 이벤트를 거치지 않기에 여기서 로그인 카운트 올림
	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();

	if( KSimLayer::AF_INTERNAL == dwAuthFlag )							// 사내 로그인 인증 결과 처리
	{
		ProcessLoginResultCount( kPacket_.m_iOK );
	}
#if defined( SERV_COUNTRY_ID ) || defined( SERV_COUNTRY_PH )
	else																// 인도네시아, 필리핀 로그인 인증 결과 처리
	{
		ProcessLoginResultCount( kPacket_.m_iOK );
	}
#endif // defined( SERV_COUNTRY_ID ) || defined( SERV_COUNTRY_PH )
#endif SERV_LOGIN_RESULT_INFO

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		StateTransition( KCHFSM::I_TO_AUTHENTICATED );
		
		//{{ 2011.03.16  임규수 IP 블록 거래 차단
#ifdef SERV_IP_TRADE_BLOCK
		KDBE_IP_CHECK_LIST_NOT kPacketNot;
		kPacketNot.m_wstrID = kPacket_.m_wstrUserID;
		//kPacketNot.iUserUID = kPacket_.m_iUserUID;
		kPacketNot.strIP = GetIPStr();

		START_LOG( clog, L"[테스트 로그] 유저 UID" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
#else
			<< BUILD_LOG( kPacketNot.m_wstrID )
			<< BUILD_LOG( GetIPStr() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SendToAccountDB(DBE_IP_CHECK_LIST_NOT, kPacketNot);
#endif SERV_IP_TRADE_BLOCK
		//}}
	}
	else
	{
		ReserveDestroy();
	}
}

IMPL_ON_FUNC( DBE_CH_USER_KOGOTP_LOGIN_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	// 국가별 특이 처리 있는 경우 사용
	HandleKOGOTPLoginAck();


	KECH_VERIFY_ACCOUNT_ACK kPacketAck;

	kPacketAck.m_iOK = kPacket_.m_iOK;
	kPacketAck.m_wstrUserID = kPacket_.m_wstrUserID;
	kPacketAck.m_iUserUID = kPacket_.m_iUserUID;
	kPacketAck.m_wstrPassport = kPacket_.m_wstrOTP;
#ifdef SERV_PURCHASE_TOKEN
	kPacketAck.m_wstrPurchaseTok = L"";	// 토큰 값은 GENERAL_LOGIN에서 받음. 여기선 NULL 세팅 
#endif SERV_PURCHASE_TOKEN

	START_LOG( clog, L"[테스트 로그] Send Packet Token 정상 확인" )
		<< BUILD_LOG( kPacketAck.m_iOK )
		<< BUILD_LOG( kPacketAck.m_wstrUserID )
		<< BUILD_LOG( kPacketAck.m_iUserUID )
		<< BUILD_LOG( kPacketAck.m_wstrPassport )
		<< BUILD_LOG( kPacketAck.m_wstrPurchaseTok )
		<< END_LOG;

	SendPacket( ECH_VERIFY_ACCOUNT_ACK, kPacketAck );

    if( kPacket_.m_iOK == NetError::NET_OK )
    {
        StateTransition( KCHFSM::I_TO_AUTHENTICATED );
    }
    else
    {
        ReserveDestroy();
    }
}

IMPL_ON_FUNC( ECH_SECURITY_AUTH_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );

	KECH_SECURITY_AUTH_ACK kPacket;

	DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
	if (KSimLayer::AF_INTERNAL == dwAuthFlag)
	{
		StateTransition( KCHFSM::I_TO_AUTHENTICATED );
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( ECH_SECURITY_AUTH_ACK, kPacket );
	}

	RequestPublisherSecurityAuth(kPacket_);
}

IMPL_ON_FUNC( EPUBLISHER_SECURITY_AUTH_ACK )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );	// 인증전에 요청되기에 S_INIT임

	HandlePublisherSecurityAuthAck(kPacket_);
}

IMPL_ON_FUNC( EPUBLISHER_SECURITY_INFO_NOT )
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );	// 인증전에 요청되기에 S_INIT임

	HandlePublisherSecurityInfoNot(kPacket_);
}

#endif // SERV_GLOBAL_AUTH

#ifdef SERVER_GROUP_UI_ADVANCED
IMPL_ON_FUNC_NOPARAM( ECH_GET_SERVERGROUP_LIST_REQ )
{
	VERIFY_STATE( ( 1, KCHFSM::S_LOGINED ) );

	KECH_GET_SERVERGROUP_LIST_ACK kPacketAck;

	kPacketAck.m_iOK = NetError::NET_OK;
	SendPacket( ECH_GET_SERVERGROUP_LIST_ACK, kPacketAck );

	KECH_GET_SERVERGROUP_LIST_NOT kPacketNot;
	kPacketNot.m_mapServerGroupList = GetKChannelServer()->GetServerGroupList();
	SendPacket( ECH_GET_SERVERGROUP_LIST_NOT, kPacketNot );
}
#endif SERVER_GROUP_UI_ADVANCED
//{{김준환 서버시간 받아오기
#ifdef	SERV_SERVER_TIME_GET
IMPL_ON_FUNC_NOPARAM(ECH_GET_SERVER_TIME_REQ)
{
	VERIFY_STATE( ( 1, KCHFSM::S_INIT ) );
	KECH_GET_SERVER_TIME_ACK kPacket;
	CTime tCurrent = CTime::GetCurrentTime();
	kPacket.m_wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	SendPacket( ECH_GET_SERVER_TIME_ACK, kPacket );
}
#endif  SERV_SERVER_TIME_GET

#ifdef SERV_ID_NETMARBLE_PCBANG
IMPL_ON_FUNC(ECH_PCBANG_IP_AND_MAC_INFO_NOT)
{

	KDBE_PCBANG_IP_AND_MAC_INFO_NOT kPacketToDB;
	kPacketToDB.m_wstrPublicIP = kPacket_.m_wstrPublicIP;
	kPacketToDB.m_wstrMacInfo = kPacket_.m_wstrMacInfo;
	SendToLogDB( DBE_PCBANG_IP_AND_MAC_INFO_NOT, kPacketToDB );
}
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_LOGIN_RESULT_INFO
void KChannelUser::ProcessLoginResultCount( IN const int iResult )
{
	switch( iResult )
	{
	case NetError::NET_OK:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_SUCCESS);
		break;
	case NetError::ERR_VERIFY_21:	// 인증 서버 문제		
	case NetError::ERR_GIANT_VERIFY_07:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_SYSTEM_ERROR);
		break;
	case NetError::ERR_GAMEFORGE_01:		
	case NetError::ERR_GIANT_VERIFY_06:
#ifdef SERV_COUNTRY_US
	case NetError::ERR_BUY_CASH_ITEM_25:
#endif //SERV_COUNTRY_US
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_ACCOUNT_NOT_EXIST);
		break;
	case NetError::ERR_GAMEFORGE_00:
	case NetError::ERR_GIANT_VERIFY_05:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_PASSWORD_ERROR);
		break;
	case NetError::ERR_GIANT_VERIFY_02:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_OTP_PASSWORD_ERROR);
		break;
	case NetError::ERR_K3RCOMBO_AUTH_01:
	case NetError::ERR_GIANT_VERIFY_04:
	case NetError::ERR_GASH_17:	
	case NetError::ERR_VERIFY_11:	
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
	case NetError::ERR_ACCOUNT_BLOCK_01:	
	case NetError::ERR_ACCOUNT_BLOCK_02:	
	case NetError::ERR_ACCOUNT_BLOCK_03:	
	case NetError::ERR_ACCOUNT_BLOCK_04:	
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_ACCOUNT_BLOCK);
		break;
	case NetError::ERR_GIANT_VERIFY_03:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_MATRIX_NUMBER_ERROR);
		break;
	case NetError::ERR_GASH_12:	
	case NetError::ERR_GASH_13:
	case NetError::ERR_GASH_16:	
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_REJECT);
		break;		
	case NetError::ERR_GAMEFORGE_02:
	case NetError::ERR_K3RCOMBO_AUTH_02:	
	case NetError::ERR_UNKNOWN:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_ACCOUNT_NOT_ACTIVATE);
		break;
	default:
		GetKChannelServer()->PlusLoginResultCount(KLoginResultInfo::LRTE_FAIL_UNKNOWN);
		break;
	}
}
#endif SERV_LOGIN_RESULT_INFO