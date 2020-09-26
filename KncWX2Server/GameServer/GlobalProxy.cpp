#include "GlobalProxy.h"
#include "NetError.h"

#include "GSUser.h"
#include "ActorManager.h"
#include "ProxyManager.h"
#include "KncSend.h"
#include "GameServer.h"
#include "DBLayer.h"
#include "RoomListManager.h"

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

//{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
	#include "PacketMornitoring/PacketMornitoring.h"
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}

NiImplementRTTI( KGlobalProxy, KActorProxy );
//ImplementProxy( KLoginProxy );
ImplPfID( KGlobalProxy, PI_GLOBAL_PROXY );

#define CLASS_TYPE KGlobalProxy

KGlobalProxy::KGlobalProxy(void)
{
}

KGlobalProxy::~KGlobalProxy(void)
{
}

ImplToStringW( KGlobalProxy )
{
	return START_TOSTRING_PARENTW( KActorProxy )
		<< TOSTRINGW( KncUtil::toWideString(GetIPStr()) )
		<< TOSTRINGW( GetPort() )
		<< TOSTRINGWb( IsConnected() );
}

// Call to ProcessEventQueue (Client)
void KGlobalProxy::ProcessEvent( const KEventPtr& spEvent_ )
{
	if( RoutePacket( spEvent_ ) )
	{
		return;
	}

	switch( spEvent_->m_usEventID )
	{
		//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
		//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
	   _CASE( EGB_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
		//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
		//}}
        CASE_NOPARAM( E_RESERVE_DESTROY );
	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음 - " << spEvent_->GetIDStr() );
	}
}

void KGlobalProxy::OnDestroy()
{
	KActorProxy::OnDestroy();

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	SiKProxyManager()->DestroyProxy( KProxyManager::PT_GLOBAL, GetProxyID() );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	if( GetUID() != 0 )
	{
		//{{ 2011. 07. 27	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_USER, NULL, EGB_CHECK_INVALID_MATCH_NOT, char() );
		KActorManager::GetKObj()->QueueingToAll( spEvent );
#endif SERV_PVP_NEW_SYSTEM
		//}}
	}	
	
	//{{ 2009. 1. 30  최육사	객체 소멸 로그
	if( GetDisconnectReason() == KStatistics::eSIColDR_Server_Shutdown )
	{
		START_LOG( cout, L"서버 셧다운으로 인한 LoginProxy객체 소멸" )
			<< BUILD_LOG( GetProxyID() );
	}
	else
	{
		START_LOG( cout2, L"비정상적인 원인으로 인한 LoginProxy객체 소멸" )
			<< BUILD_LOG( GetProxyID() )
			<< BUILD_LOG( GetDisconnectReason() )
			<< dbg::tab << L"종료 사유 : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl;

		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetProxyID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}

		//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
		{
			//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
			KE_DISCONNECT_SERVER_REPORT_NOT kNot;
			kNot.m_cType = KMornitoringManager::ET_DISCONNECT_GAME_GLOBAL_PROXY;
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( 0, NULL, E_DISCONNECT_SERVER_REPORT_NOT, kNot );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 2011. 04. 13  김민성  글로벌 서버 추가
		}
#endif SERV_MORNITORING
		//}}
	}
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-31	// 박세훈
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, EGB_VERIFY_SERVER_DISCONNECT_NOT );
		KBaseServer::GetKObj()->QueueingEvent( spEvent );
	}
#endif // SERV_BATTLE_FIELD_BOSS
}

//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
_IMPL_ON_FUNC( EGB_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK )
{
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nOK );

	m_nAckOK = kPacket_.m_nOK;
	if( m_nAckOK == NetError::NET_OK )
	{
		KBaseServer::GetKObj()->SetUID( kPacket_.m_nUID );

		SetUID( kPacket_.m_iServerUID );

		START_LOG( cout2, L"글로벌서버와 정상적으로 접속하였습니다!" )
			<< BUILD_LOG( kPacket_.m_iServerUID );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-31	// 박세훈
		{
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_GS_SERVER, NULL, EGB_VERIFY_SERVER_CONNECT_NOT );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
		{
			KEvent kEvent;
			kEvent.SetData( PI_GLOBAL_USER, NULL, EGB_TIME_ENCHANT_EVENT_INFO_REQ, char() );
			KSession::SendPacket( kEvent );
		}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
		{
			KEvent kEvent;
			kEvent.SetData( PI_GLOBAL_USER, NULL, EGB_EXCHANGE_LIMIT_INFO_REQ, char() );
			KSession::SendPacket( kEvent );
		}
#endif // SERV_ITEM_EXCHANGE_LIMIT
	}

	SetEvent( m_spEventAck.get() );
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}

bool KGlobalProxy::RoutePacket( const KEventPtr& spEvent_ )
{
	//{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
	SiKPacketMornitoring()->CheckGlobalProxyPacketProcessingCount( spEvent_->m_usEventID );
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
	//}}

	//START_LOG( clog, L"라우팅" )
	//    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
	//    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_iUID )
	//    << BUILD_LOG( pkEvent_->GetLastSenderUID() )
	//    << BUILD_LOG( pkEvent_->m_usEventID )
	//    << BUILD_LOG( pkEvent_->GetIDStr() );

	if( spEvent_->m_kDestPerformer.m_dwPerformerID != PI_GLOBAL_PROXY )
	{
		int iEval = CompareServerLevel( spEvent_->m_kDestPerformer.m_dwPerformerID, PI_GLOBAL_PROXY );
		if( iEval < 0 )
		{
			_JIF( !spEvent_->IsEmptyTrace(), return false );
			KActorManager::GetKObj()->QueueingTo( spEvent_->GetLastSenderUID(), spEvent_ );
			return true;
		}
		else if( iEval > 0 )
		{
			START_LOG( cerr, L"상위 서버로 보낼 이벤트가 오면 안된다." )
				<< BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
				<< BUILD_LOG( spEvent_->m_usEventID )
				<< BUILD_LOG( spEvent_->GetIDStr() )
				<< END_LOG;
			return true;
		}
		else
		{
			switch( GetPerformerClass( spEvent_->m_kDestPerformer.m_dwPerformerID ) )
			{
			case PC_USER:
				{
					//{{ 2009. 7. 15  최육사	유저 정보가 없을때 처리
					std::vector< UidType > vecUID;
					int iNotExistUserCnt = KActorManager::GetKObj()->MultiQueueing( spEvent_, vecUID );
					if( iNotExistUserCnt > 0 )
					{
						ProcbyNotExistUser( spEvent_, vecUID );

						START_LOG( cwarn, L"패킷전달 유닛이 없음." )
							<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
							<< BUILD_LOG( iNotExistUserCnt )
							<< END_LOG;
					}
					//}}
				}
				return true;
			case PC_CHARACTER:
				KActorManager::GetKObj()->MultiQueueingByCID( spEvent_ );
				return true;
			case PC_SERVER:
				KBaseServer::GetKObj()->QueueingEvent( spEvent_ );
				return true;
			case PC_GAME_DB:
				SiKDBLayer()->QueueingEvent( spEvent_ );
				return true;
			default:
				START_LOG( cerr, L"도착지가 이상한 패킷." )
					<< BUILD_LOG( spEvent_->m_kDestPerformer.m_dwPerformerID )
					<< END_LOG;
				return true;
			}
		}
	}

	return false;
}

void KGlobalProxy::ProcbyNotExistUser( const KEventPtr& spEvent_, std::vector<UidType>& vecUID )
{
	switch( spEvent_->m_usEventID )
	{
	case ELG_GET_UNIT_INFO_REQ:
		{
			for( int i = 0; i < (int)vecUID.size(); ++i )
			{
				KSerializer ks;
				ks.BeginReading( &spEvent_->m_kbuff );
				KELG_GET_UNIT_INFO_REQ kData_;
				if( !ks.Get(kData_) )
				{
					START_LOG( cerr, L"deserialze failed." )
						<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
				else
				{
					KELG_GET_UNIT_INFO_ACK kPacket;
					kPacket.m_iOK						= NetError::ERR_SEARCH_UNIT_03;
					kPacket.m_usEventID					= kData_.m_usEventID;
					kPacket.m_iDemandGSUID				= kData_.m_iDemandGSUID;
					kPacket.m_iDemandUserUID			= kData_.m_iDemandUserUID;
					kPacket.m_kCUnitInfo.m_wstrCharName = kData_.m_wstrNickName;

					UidType anTrace[2] = { vecUID[i], -1 };
					KncSend( PI_GS_USER, vecUID[i], PI_GLOBAL_PROXY, 0, anTrace, ELG_GET_UNIT_INFO_ACK, kPacket );

					spEvent_->m_kbuff.Reset();

					// 일단 에러로그 찍자!
					START_LOG( cerr, L"유닛 정보 얻을려고 했는데 시간차로 유저가 끊겨버렸군!" )
						<< BUILD_LOG( vecUID[i] )
						<< END_LOG;
				}
			}
		}
		break;		
	}
}


