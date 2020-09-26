#include "CnUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"

#include "DBLayer.h"
#include "NetError.h"
#include "CnSimLayer.h"
#include "CnRoomManager.h"
//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewCnPartyManager.h"
#else
	#include "CnPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
#include "CnServer.h"
//#include "Nexon/AuthCheck.h" -- 삭제 예정 코드
#include <boost/format.hpp>

//#pragma comment( lib, "../Common/Nexon/AuthCheck.lib" ) -- 삭제 예정 코드

NiImplementRTTI( KCnUser, KActor );
ImplPfID( KCnUser, PI_CN_USER );

#define CLASS_TYPE  KCnUser

KCnUser::KCnUser(void) 
//{{ 2010. 04. 16  최육사	서버군 전체 통합
#ifdef SERV_INTEGRATION
:
m_iCnServerGroupID( -1 ),
m_iGsServerGroupID( -1 )
#endif SERV_INTEGRATION
//}}
{
}

KCnUser::~KCnUser(void)
{
}

ImplToStringW( KCnUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KCnUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    if( RoutePacket( spEvent_.get() ) )
        return;

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
	//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	CASE( ECN_VERIFY_SERVER_CONNECT_REQ );
#else
   _CASE( ECN_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
	//}}   
   _CASE( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
   _CASE( ECN_UPDATE_SERVER_INFO_ACK, KServerList );
    CASE_NOPARAM( EGS_GET_SERVER_SET_DATA_REQ );
   _CASE( ECN_UPDATE_SERVER_INFO_REQ, KServerInfo );	

	CASE_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ );
	CASE_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ );
	CASE_NOPARAM( E_RESERVE_DESTROY );

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

void KCnUser::Tick()
{
    KActor::Tick();

    switch( GetStateID() ) 
	{
    case KDefaultFSM::S_EXIT:
        KActorManager::GetKObj()->ReserveDelete( m_nUID );    // 자기 자신을 스스로 삭제할 수 없음.
        break;
    }
}

//bool KCnUser::AuthenticateUser( const wchar_t* szPassport, KNexonAccountInfo& kInfo ) -- 삭제 예정 코드
//{
//    BOOL bRet;
//    wchar_t szID[MAX_PATH];
//    UINT32 nNexonSN;
//    wchar_t szLoginIP[MAX_PATH];
//    UINT32 uSex;
//    UINT32 uAge;
//    UINT32 uPwdHash;
//    UINT32 uSsnHash;
//    UINT32 uFlag0;
//    UINT32 uFlag1;
//
//    bRet = AuthCheck_LocalCheckPassport( szPassport, szID, &nNexonSN, szLoginIP, &uSex, &uAge, &uPwdHash, &uSsnHash, &uFlag0, &uFlag1 );
//
//    if( bRet )
//    {
//        if( kInfo.m_wstrLoginIP.compare( szLoginIP ) != 0 )
//        {
//            START_LOG( cerr, L"KSession의 유저 IP와 넥슨 패스워드에서 추출한 IP가 다름." )
//                << BUILD_LOG( kInfo.m_wstrLoginIP )
//                << BUILD_LOG( szLoginIP )
//                << END_LOG;
//
//            // 인증 실패 처리 할까 말까..
//        }
//
//        kInfo.m_wstrID = szID;
//        kInfo.m_uiNexonSN = nNexonSN;
//        kInfo.m_wstrLoginIP = szLoginIP;
//        if( uSex == 1 )
//        {
//            kInfo.m_bSex = true;
//        }
//        else if( uSex == 2 )
//        {
//            kInfo.m_bSex = false;
//        }
//        else
//        {
//            START_LOG( cerr, L"넥슨 인증 성별 값이 이상함." )
//                << BUILD_LOG( szID )
//                << BUILD_LOG( szPassport )
//                << BUILD_LOG( uSex )
//                << END_LOG;
//        }
//        kInfo.m_uiAge = uAge;
//        kInfo.m_uiPwdHash = uPwdHash;
//        kInfo.m_uiSsnHash = uSsnHash;
//        kInfo.m_uiFlag0 = uFlag0;
//        kInfo.m_uiFlag1 = uFlag1;
//    }
//
//    return (bRet == TRUE);  // BOOL -> bool
//}

void KCnUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() ) {
    case KDefaultFSM::S_INIT:
    case KDefaultFSM::S_LOGINED:
        StateTransition( KDefaultFSM::I_EXIT );    // OnDestroy()가 불린 현재 KSession::Tick 처리 중이며,
                                                   // KCnUser::Tick 처리 단계에서 종료 대기명단에 등록된다.
        break;
    }

	//{{ 2011.2.11  조효진  모니터링툴 서버 포트 체크 시 에러로그 남는거 수정
#ifdef SERV_PERMIT_PORT_CHECK
	// 모니터링 툴 서버의 IP에서는 게임 서버를 동작 할 수 없다고 가정함 //
	// 모니터링 서버에 의한 포트 체크일 경우 아래의 다른 동작들 할 필요없음
	if( KBaseServer::GetKObj()->IsMonitoringServerForPortCheck( GetIPStr() ) ){
		//KSession::OnDestroy();					// 이미 위에서 수행함
		//StateTransition( KDefaultFSM::I_EXIT );	// 이미 위에서 수행함
		START_LOG( clog, L"Mornitoring server check Port!" );
		return;
	}
#endif SERV_PERMIT_PORT_CHECK
	//}}

    // 게임서버가 죽은 경우, 해당 게임서버에 접속중이던 유저들을 방에서 모두 빼낸다.
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, ERM_CHECK_INVALID_USER_NOT, m_nUID );

		//SiKRoomManager()->QueueingToAll( spEvent );
		// 081017.hoons.
		GetKCnRoomManager()->QueueingToAll( spEvent );
	}

	//{{ 2010. 03. 09  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, EPM_CHECK_INVALID_USER_NOT, m_nUID );
		//SiKCnPartyManager()->QueueingToAll( spEvent );
		SiKCnPartyManager()->QueueingEvent( spEvent );
	}
#endif SERV_CHANNEL_PARTY
	//}}

	//{{ 2009. 4. 22  최육사	게임서버 접속끊김에 따라 대전유저 이탈처리
	{
		KEventPtr spEventPVP( new KEvent );
		spEventPVP->SetData( PI_GS_SERVER, NULL, ECN_CHECK_INVALID_PVP_USER_NOT, m_nUID );
		KBaseServer::GetKObj()->QueueingEvent( spEventPVP );
	}
	//}}

	//{{ 2009. 1. 30  최육사	객체 소멸 로그
	if( GetDisconnectReason() == KStatistics::eSIColDR_Server_Shutdown )
	{
		START_LOG( cout, L"서버 셧다운으로 인한 CnUser객체 소멸" );			
	}
	//{{ 2011. 10. 31	최육사	하트비트 오류 원인 분석
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	else if( GetDisconnectReason() == KStatistics::eSIColDR_Heart_Beat )
	{
		START_LOG( cout2, L"하트비트로 인한 CnUser객체 소멸" )
			<< BUILD_LOG( GetDisconnectReason() )
			<< BUILD_LOG( GetIPStr() )
			<< BUILD_LOG( m_dwHBTickDump )
			<< BUILD_LOG( m_dwCurrentTickDump )
			<< BUILD_LOG( m_dwCurrentTickDump - m_dwHBTickDump )
			<< dbg::tab << L"종료 사유 : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl;

		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetUID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
	}
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	//}}
	else
	{
		START_LOG( cout2, L"비정상적인 원인으로 인한 CnUser객체 소멸" )
			<< BUILD_LOG( GetDisconnectReason() )
			<< BUILD_LOG( GetIPStr() )
			<< dbg::tab << L"종료 사유 : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl;

		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetUID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
	}
	//}}
}

bool KCnUser::RoutePacket( const KEvent* pkEvent_ )
{
    if( pkEvent_->m_kDestPerformer.m_dwPerformerID != PI_CN_USER )
    {
		//{{ 2011.2.11  조효진  모니터링툴 서버 포트 체크 시 에러로그 남는거 수정
#ifdef SERV_PERMIT_PORT_CHECK
		// CnUser 모니터링 서버인 경우 정상적인 객체가 아니므로 RoutPacket 과정을 거치면 안됨
		if( KBaseServer::GetKObj()->IsMonitoringServerForPortCheck( GetIPStr() ) )
		{
			return true;
		}
#endif SERV_PERMIT_PORT_CHECK
		//}}

        int iEval = CompareServerLevel( pkEvent_->m_kDestPerformer.m_dwPerformerID, PI_CN_USER );
        if( iEval < 0 )
        {
            if( pkEvent_->IsEmptyTrace() )
            {
                START_LOG_WITH_NAME( cerr ) 
                    << BUILD_LOG( pkEvent_->GetIDStr() );

                return true;
            }

			//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
			// 서버군 정보가 초기화 되었는지 확인
			if( IsInitServerGroup() == false )
			{
				START_LOG( cout, L"Not Init Server Group Event : " << KEvent::GetIDStr( pkEvent_->m_usEventID ) )
					<< BUILD_LOG( m_iCnServerGroupID )
					<< BUILD_LOG( m_iGsServerGroupID );
				return true;
			}

			// 같은 서버군에게만 보내는 이벤트라면?
			if( GetSendType( pkEvent_->m_kDestPerformer.m_dwPerformerID ) == ST_SAME_SERV_GROUP )
			{
				if( IsSameServerGroup() == false )
				{
					START_LOG( clog, L"Not Same Server Group Event : " << KEvent::GetIDStr( pkEvent_->m_usEventID ) );
					return true;
				}
			}
#endif SERV_INTEGRATION
			//}}

            KEvent kEvent( *pkEvent_ );

            kEvent.PopTrace();

            LIF( KSession::SendPacket( kEvent ) );
            return true;
        }
        else if( iEval > 0 )
        {
            START_LOG( cerr, L"현재는 센터 서버가 최상위 서버이다." )
                << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( pkEvent_->m_usEventID )
                << BUILD_LOG( pkEvent_->GetIDStr() )
                << END_LOG;

            return true;
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
            case PC_ACCOUNT_DB:
            case PC_LOG_DB:
                SiKDBLayer()->QueueingEvent( spEvent );
                return true;
            case PC_ROOM:
                //SiKRoomManager()->QueueingEventToRoom( spEvent );
				//081017.hoons.
				GetKCnRoomManager()->QueueingEventToRoom( spEvent );
                return true;
				//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY
			case PC_PARTY:
				SiKCnPartyManager()->QueueingEventToParty( spEvent );
				return true;
#endif SERV_CHANNEL_PARTY
				//}}
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

void KCnUser::SendToAccountDB( unsigned short usEventID )
{
    SendToAccountDB( usEventID, char() );
}

void KCnUser::SendToGSUser( UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nTo, usEventID, char() );
}

//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION

IMPL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_usPort );

	if( kPacket_.m_kNetAddress.m_wstrIP != KncUtil::toWideString( GetIPStr() ) )
	{
		START_LOG( cerr, L"유효하지 않은 IP" )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() );
#else
			<< BUILD_LOG( GetIPStr() );
#endif SERV_PRIVACY_AGREEMENT

		KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
		kPacket.m_nOK = NetError::ERR_VERIFY_02;
		SendPacket( ECN_VERIFY_SERVER_CONNECT_ACK, kPacket );
		return;
	}

	SendToLogDB( DBE_VERIFY_SERVER_CONNECT_REQ, kPacket_ );
}

#else

_IMPL_ON_FUNC( ECN_VERIFY_SERVER_CONNECT_REQ, KNetAddress )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_usPort );

	if( kPacket_.m_wstrIP != KncUtil::toWideString( GetIPStr() ) )
	{
		START_LOG( cerr, L"유효하지 않은 IP" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
#else
			<< BUILD_LOG( kPacket_.m_wstrIP )
			<< BUILD_LOG( GetIPStr() );
#endif SERV_PRIVACY_AGREEMENT

		KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
		kPacket.m_nOK = NetError::ERR_VERIFY_02;
		SendPacket( ECN_VERIFY_SERVER_CONNECT_ACK, kPacket );
		return;
	}

	SendToLogDB( DBE_VERIFY_SERVER_CONNECT_REQ, kPacket_ );
}

#endif
//}}

_IMPL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );    

	//{{ 2010. 04. 16  최육사	서버군 전체 통합
#ifdef SERV_INTEGRATION
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nUID )
		<< BUILD_LOG( kPacket_.m_nOK )
		<< BUILD_LOG( kPacket_.m_strName )
		<< BUILD_LOG( kPacket_.m_iDBRegServerGroupID )
		<< BUILD_LOG( kPacket_.m_iLocalServerGroupID )
		<< BUILD_LOG( kPacket_.m_sMaxNum );

	kPacket_.m_iDBRegServerGroupID = GetKCnServer()->GetServerGroupID(); // 센터서버의 서버군ID를 게임서버에게 넘겨준다.
#else
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nUID )
		<< BUILD_LOG( kPacket_.m_nOK )
		<< BUILD_LOG( kPacket_.m_strName )
		<< BUILD_LOG( kPacket_.m_sGroupID )
		<< BUILD_LOG( kPacket_.m_sMaxNum );
#endif  SERV_INTEGRATION
	//}}

    SendPacket( ECN_VERIFY_SERVER_CONNECT_ACK, kPacket_ );

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        ReserveDestroy();
        return;
    }
	else
	{
		//{{ 2010. 04. 16  최육사	서버군 전체 통합
#ifdef SERV_INTEGRATION
		m_iCnServerGroupID = GetKCnServer()->GetServerGroupID();
		m_iGsServerGroupID = kPacket_.m_iLocalServerGroupID;

		//{{ 2009. 3. 25  최육사	로그
		if( IsSameServerGroup() )
		{
			START_LOG( cout2, L"같은 서버군의 게임서버와 정상적으로 접속하였습니다!" )
				<< BUILD_LOG( kPacket_.m_nUID )
				<< BUILD_LOG( kPacket_.m_strName )
				<< BUILD_LOG( m_iCnServerGroupID );
		}
		else
		{
			START_LOG( cout2, L"다른 서버군의 게임서버와 정상적으로 접속하였습니다!" )
				<< BUILD_LOG( kPacket_.m_nUID )
				<< BUILD_LOG( kPacket_.m_strName )
				<< BUILD_LOG( m_iCnServerGroupID )
				<< BUILD_LOG( m_iGsServerGroupID );
		}
		//}}
#else
		//{{ 2009. 3. 25  최육사	로그
		START_LOG( cout2, L"게임서버와 정상적으로 접속하였습니다!" )
			<< BUILD_LOG( kPacket_.m_nUID )
			<< BUILD_LOG( kPacket_.m_strName );
		//}}
#endif SERV_INTEGRATION
		//}}
	}

    KActorPtr spActor( KActorManager::GetKObj()->Get( kPacket_.m_nUID ) );

    if( spActor != NULL )   // 서버 이중 접속
    {
        SET_ERROR( ERR_VERIFY_03 ); // 서버 UID 충돌
        ReserveDestroy();
        return;
    }

    SetName( kPacket_.m_strName );
    LIF( KActorManager::GetKObj()->UpdateUID( kPacket_.m_nUID, *this ) );
    StateTransition( KDefaultFSM::I_AUTHENTICATED );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_ROOM, NULL, ERM_SQUARE_LIST_INFO_NOT, GetUID() );

	//081020.hoons.
	//SiKRoomManager()->QueueingToAll( CXSLRoom::RT_SQUARE, spEvent );
	GetKCnRoomManager()->QueueingToAll( CXSLRoom::RT_SQUARE, spEvent );
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

_IMPL_ON_FUNC( ECN_UPDATE_SERVER_INFO_ACK, KServerList )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
}

IMPL_ON_FUNC_NOPARAM( EGS_GET_SERVER_SET_DATA_REQ )
{
	VERIFY_STATE(( 1, KDefaultFSM::S_LOGINED ));
	
	KEGS_GET_SERVER_SET_DATA_ACK kPacket;

	kPacket.m_iOK = NetError::NET_OK;
	//KCnServer::GetKObj()->GetServeSetList( kPacket.m_ServerSetList );

	SendToGSUser( LAST_SENDER_UID, EGS_GET_SERVER_SET_DATA_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ )
{
	//{{ 2008. 7. 1  최육사	 local statistics flush시 통계정보 삭제
	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD	
	KSIManager.QueueingEventID( ECN_SEND_STATISTICS_LOCLOG_REQ );
#else
	KSIManager.OnFlushLocalLog( true );
#endif SERV_STATISTICS_THREAD
	//}}
	//}}
}

IMPL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ )
{
	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD	
	KSIManager.QueueingEventID( ECN_SEND_STATISTICS_DBLOG_REQ );
#else
	KSIManager.FlushData();
#endif SERV_STATISTICS_THREAD
	//}}	
}

_IMPL_ON_FUNC( ECN_UPDATE_SERVER_INFO_REQ, KServerInfo )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( kPacket_.m_iUID )
        << BUILD_LOG( kPacket_.m_wstrName );

    SendToLogDB( ECN_UPDATE_SERVER_INFO_REQ, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}
