#include ".\Trade.h"

#include "Room/TradeFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	#include "CnServer.h"
#endif SERV_INTEGRATION
//}}

#define CLASS_TYPE KTrade

NiImplementRTTI( KTrade, KCnBaseRoom );

ImplToStringW( KTrade )
{
	return START_TOSTRING_PARENTW( KCnBaseRoom );
}

KTrade::KTrade(void)
{
	m_kTradeUserManager.Init( 2 );
}

KTrade::~KTrade(void)
{
}

void KTrade::CloseRoom()
{
	m_kTradeUserManager.Reset();

	KCnBaseRoom::CloseRoom();
	return;
}

void KTrade::Tick()
{
	KCnBaseRoom::Tick();

	switch( GetStateID() )
	{
	case KTradeFSM::S_INIT:
		break;

	case KTradeFSM::S_CLOSE:
		CloseRoom();
		//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
		SendRoomCountInfo( CXSLRoom::RT_TRADE, false );
#endif SERV_ROOM_COUNT
		//}}
		break;

	case KTradeFSM::S_WAIT:
		// 개인 거래 승인한 사람이 못들어오는 경우 거래 폭파.
		if( m_kTradeUserManager.GetNumMember() <= 1 )
		{
			if( m_kTimer[TM_WAIT_OTHER].elapsed() >= 5.0 )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_08 );
			}
		}
		break;

	case KTradeFSM::S_CONFIRM:
		break;

		//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	case KTradeFSM::S_DB_UPDATE:
		break;
#endif SERV_TRADE_DDOS_DEFENCE
		//}}

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}
}

bool KTrade::IsEmpty()
{
	if( m_kTradeUserManager.GetNumMember() < 0 )
	{
		START_LOG( cerr, L"개인 거래 인원 수가 이상함." )
			<< BUILD_LOG( m_kTradeUserManager.GetNumMember() )
			<< END_LOG;
	}

	return ( m_kTradeUserManager.GetNumMember() <= 0 );
}

bool KTrade::IsFull()
{
	if( m_kTradeUserManager.GetNumMember() > m_kTradeUserManager.GetMaxMember() )
	{
		START_LOG( cerr, L"개인 거래 인원 수가 이상함." )
			<< BUILD_LOG( m_kTradeUserManager.GetNumMember() )
			<< BUILD_LOG( m_kTradeUserManager.GetMaxMember() )
			<< END_LOG;
	}

	return ( m_kTradeUserManager.GetNumMember() >= m_kTradeUserManager.GetMaxMember() );
}

void KTrade::BreakTrade( int iReason )
{
	KEGS_BREAK_TRADE_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	BroadCast( ERM_BREAK_TRADE_NOT, kPacketNot );

	StateTransition( KTradeFSM::I_TO_CLOSE );
}

void KTrade::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

	switch( spEvent_->m_usEventID )
	{
	CASE( ERM_OPEN_TRADE_REQ );
   _CASE( ERM_BREAK_TRADE_REQ, KEGS_BREAK_TRADE_REQ );
    CASE( ERM_JOIN_TRADE_REQ );
   //_CASE( ERM_CHAT_REQ, KEGS_CHAT_REQ );
   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );
    CASE( ERM_UPDATE_TRADE_ITEM_REQ );
	CASE( ERM_UPDATE_TRADE_ED_REQ );
    CASE_NOPARAM( ERM_OFFER_TRADE_REQ );
    CASE_NOPARAM( ERM_APPROVE_TRADE_REQ );
	CASE( ERM_CONFIRM_TRADE_ACK );
	//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
	CASE( ERM_DB_UPDATE_TRADE_RESULT_NOT );
	CASE( DBE_DB_UPDATE_TRADE_RESULT_ACK );
#endif SERV_TRADE_DDOS_DEFENCE
	//}}

	default:
		START_LOG( cerr, L"핸들러가 정의되지 않았음." )
			<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
			<< END_LOG;
		break;
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

void KTrade::BroadCastID( u_short usEventID )
{
	BroadCast( usEventID, char() );
}

void KTrade::_BroadCast( const KEvent& kEvent_ )
{
    std::map< UidType, std::set< UidType > > mapUserList;
    m_kTradeUserManager.GetUserList( mapUserList );

    // GameServer에 대한 loop
    std::map< UidType, std::set< UidType > >::iterator mit;
    for( mit = mapUserList.begin(); mit != mapUserList.end(); mit++ )
    {
        KEventPtr spEvent( kEvent_.Clone() );
        spEvent->m_anTrace[0] = mit->first;
        spEvent->m_anTrace[1] = -1;

        // User 각각에 대한 loop
        std::set< UidType >::iterator sit;
        for( sit = mit->second.begin(); sit != mit->second.end(); sit++ )
        {
            LIF( spEvent->m_kDestPerformer.AddUID( *sit ) );
        }

        KncSend( PI_CN_ROOM, GetUID(), spEvent );
    }
}

IMPL_ON_FUNC( ERM_OPEN_TRADE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KTradeFSM::S_INIT ), ERM_OPEN_TRADE_ACK, KERM_OPEN_TRADE_ACK );

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_TRADE, true );
#endif SERV_ROOM_COUNT
	//}}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//////////////////////////////////////////////////////////////////////////
	// 같은 서버군의 센터에 거래방을 생성한건지 확인!
	if( KBaseServer::GetKObj()->GetServerGroupID() != KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
	{
        START_LOG( cerr2, L"다른 서버군의 유저가 센터 거래방을 생성하였습니다! 이러면 거래로그 이상하게 남아요!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( KBaseServer::GetKObj()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
			<< END_LOG;
		// 로그만 찍자!
	}
	//////////////////////////////////////////////////////////////////////////	
#endif DEPRECATED_SERVER_GROUP_MASK

	// 생성이므로 유닛 정보를 초기화 한다.
	m_kTradeUserManager.Reset();

	// 생성 요청자를 넣는다.
	KTradeUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
	kInfo.m_wstrNickName = kPacket_.m_wstrSenderNickName;
	//{{ 2009. 4. 28  최육사	최대ED제한
	kInfo.m_iED = kPacket_.m_iED;
	//}}
	if( !m_kTradeUserManager.EnterTrade( kInfo ) )
	{
		START_LOG( cerr, L"개인 거래 유저 추가 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TRADE_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_TRADE_ACK, kPacket );
		StateTransition( KTradeFSM::I_TO_CLOSE );
		return;
	}

	// 방 상태를 wait 로 바꾼다.
	StateTransition( KTradeFSM::I_TO_WAIT );

	// ack를 보낸다.
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iTradeUID = GetUID();
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_TRADE_ACK, kPacket );

	// 나머지 유저를 5초만 기다리기 위하여 타이머를 세팅한다.
	m_kTimer[TM_WAIT_OTHER].restart();

	//가입권유자에게 조인하라고 게임서버에 알려준다.
	KERM_ACCEPT_JOIN_TRADE_NOT kNot;
	kNot = GetUID();
	SendToGSCharacter( kPacket_.m_iMemberGSUID, kPacket_.m_iMemberUID, ERM_ACCEPT_JOIN_TRADE_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_BREAK_TRADE_REQ, KEGS_BREAK_TRADE_REQ )
{
	//VERIFY_ROOM_STATE( ( 3, KTradeFSM::S_WAIT, KTradeFSM::S_CONFIRM, KTradeFSM::S_SUCCESS ), ERM_BREAK_TRADE_ACK, KEGS_BREAK_TRADE_ACK );
	VERIFY_ROOM_STATE( ( 2, KTradeFSM::S_WAIT, KTradeFSM::S_CONFIRM ), ERM_BREAK_TRADE_ACK, KEGS_BREAK_TRADE_ACK );

	if( !m_kTradeUserManager.LeaveTrade( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"거래 나가기 실패." )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_TRADE_01;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_TRADE_ACK, kPacket );
		}
	}
	else
	{
		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_TRADE_ACK, kPacket );
			BreakTrade( kPacket_.m_iReason );
		}
	}
}

IMPL_ON_FUNC( ERM_JOIN_TRADE_REQ )
{
	KERM_JOIN_TRADE_ACK kPacket;
	if( GetStateID() != KTradeFSM::S_WAIT )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : " << L"KTradeFSM::S_WAIT" << END_LOG;
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_TRADE_ACK, kPacket );
		return;
	}

	START_LOG_WITH_NAME( clog );

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( m_kTradeUserManager.IsAllTradeUsersSameServerGroup( kPacket_.m_kMemberInfo.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"개인거래방은 해당 서버군의 유저만 들어올 수 있습니다." )
			<< BUILD_LOG( GetKCnServer()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_kMemberInfo.m_iUnitUID ) )
			<< BUILD_LOG( kPacket_.m_kMemberInfo.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_TRADE_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	if( !m_kTradeUserManager.EnterTrade( kPacket_.m_kMemberInfo ) )
	{
		START_LOG( cerr, L"유저 추가 실패." )
			<< BUILD_LOG( kPacket_.m_kMemberInfo.m_iGSUID )
			<< BUILD_LOG( kPacket_.m_kMemberInfo.m_iUnitUID )
			<< END_LOG;

		if( m_kTradeUserManager.GetNumMember() <= 1 )
		{
			BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_07 );
		}
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_iTradeUID = GetUID();
		m_kTradeUserManager.GetUserInfoList( kPacket.m_vecTradeUserInfo );
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_TRADE_ACK, kPacket );

		//현재 방 안에 있는 유저들에게 새로 들어온 유저 정보를 알려준다.
		KEGS_JOIN_TRADE_NOT kJoinNot;
		kJoinNot.m_kTradeUserInfo = kPacket_.m_kMemberInfo;
		BroadCast( ERM_JOIN_TRADE_NOT, kJoinNot );
	}
}

//_IMPL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ )
//{
//	VERIFY_ROOM_STATE_CHAT_ONLY( ( 2, KTradeFSM::S_WAIT, KTradeFSM::S_CONFIRM ), ERM_CHAT_ACK, KERM_CHAT_ACK );
//
//	if( kPacket_.m_cChatPacketType != KEGS_CHAT_REQ::CPT_TRADE )
//	{
//		START_LOG( cerr, L"개인 거래 채팅 패킷타입이 아닌데 여기왔네?" )
//			<< BUILD_LOG( kPacket_.m_cChatPacketType )
//			<< END_LOG;
//		return;
//	}
//
//	// 보낸 사람에게 ack를 날린다.
//	kPacket.m_iOK			  = NetError::NET_OK;
//	kPacket.m_cChatPacketType = kPacket_.m_cChatPacketType;
//
//	// 모든 유저에게 채팅 내용을 날린다.
//	KEGS_CHAT_NOT kPacketBR;
//	kPacketBR.m_SenderUnitUID	= FIRST_SENDER_UID;
//	kPacketBR.m_RecieverUnitUID	= kPacket_.m_ToUnitUID;
//	kPacketBR.m_cChatPacketType = kPacket_.m_cChatPacketType;
//	kPacketBR.m_cRoomChatType	= kPacket_.m_cRoomChatType;
//	kPacketBR.m_wstrMsg			= kPacket_.m_wstrMsg;
//
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
//	BroadCast( ERM_CHAT_NOT, kPacketBR );
//}

//게임서버가 죽었을때..처리
_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<UidType>	vecDelUserUID;
	int iCount = m_kTradeUserManager.DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_kTradeUserManager.GetNumMember() );

	//현재파티에서 삭제된 유저가 없으면 종료.
	if( iCount == 0 )
		return;

	//먼저 다 빼주고
	std::vector< UidType >::iterator vit;
	for( vit = vecDelUserUID.begin(); vit != vecDelUserUID.end(); vit++ )
	{
		m_kTradeUserManager.LeaveTrade( *vit );
	}

	//그리고 살아남은 유저가 있으면 처리한다
	BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_09 );
}

IMPL_ON_FUNC( ERM_UPDATE_TRADE_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KTradeFSM::S_WAIT ), ERM_UPDATE_TRADE_ITEM_ACK, KEGS_UPDATE_TRADE_ITEM_ACK );

	//{{ 2009. 4. 6  최육사		상태체크
	if( !m_kTradeUserManager.CheckStateUpdateTradeItem( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"개인거래 아이템을 등록 또는 해제할 수 있는 상태가 아닙니다." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TRADE_03;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
		return;
	}
	//}}

	KInventoryItemInfo kInventoryItemInfo;

	if( !kPacket_.m_bDelete )
	{
		// 아이템 추가
		if( !m_kTradeUserManager.IsCanInsertTradeItem( FIRST_SENDER_UID ) )
		{
			START_LOG( cerr, L"거래 가능한 아이템 수량 제한." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_TRADE_17;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
			return;
		}

		if( m_kTradeUserManager.GetTradeItem( FIRST_SENDER_UID, kPacket_.m_kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) )
		{
			START_LOG( cerr, L"이미 올린 아이템이네." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_TRADE_15;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
			return;
		}

		if( !m_kTradeUserManager.UpdateTradeItem( FIRST_SENDER_UID, kPacket_.m_kInventoryItemInfo ) )
		{
			START_LOG( cerr, L"거래 아이템 업데이트 실패." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_TRADE_03;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
			return;
		}
	}
	else
	{
		// 아이템 삭제
		if( !m_kTradeUserManager.GetTradeItem( FIRST_SENDER_UID, kPacket_.m_kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) )
		{
			START_LOG( cerr, L"아이템을 삭제하려는데 존재하지 않는 아이템?" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_TRADE_16;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
			return;
		}

		// 업데이트 하려는 아이템이 기존과 같으면 그냥 OK를 보낸다.
		//if( kPacket_.m_kInventoryItemInfo.m_iItemUID <= 0 && kInventoryItemInfo.m_iItemUID <= 0 )
		//{
		//	kPacket.m_iOK = NetError::NET_OK;
		//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
		//	return;
		//}

		//if( ( kPacket_.m_kInventoryItemInfo.m_iItemUID == kInventoryItemInfo.m_iItemUID ) &&
		//	( kPacket_.m_kInventoryItemInfo.m_kItemInfo.m_iItemID == kInventoryItemInfo.m_kItemInfo.m_iItemID ) &&
		//	( kPacket_.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity == kInventoryItemInfo.m_kItemInfo.m_iQuantity ) )
		//{
		//	kPacket.m_iOK = NetError::NET_OK;
		//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
		//	return;
		//}

		if( !m_kTradeUserManager.RemoveTradeItem( FIRST_SENDER_UID, kPacket_.m_kInventoryItemInfo.m_iItemUID ) )
		{
			START_LOG( cerr, L"거래 아이템 삭제 실패." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_TRADE_03;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );
			return;
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ITEM_ACK, kPacket );

	if( !kPacket_.m_bDelete )
	{
		LIF( m_kTradeUserManager.GetTradeItem( FIRST_SENDER_UID, kPacket_.m_kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
	}
	KEGS_UPDATE_TRADE_ITEM_NOT kPacketNot;
	kPacketNot.m_iUnitUID  = FIRST_SENDER_UID;
	kPacketNot.m_bDelete   = kPacket_.m_bDelete;
	kPacketNot.m_iItemUID  = kPacket_.m_kInventoryItemInfo.m_iItemUID;
	kPacketNot.m_kItemInfo = kInventoryItemInfo.m_kItemInfo;
    m_kTradeUserManager.GetOfferState( kPacketNot.m_mapOfferState );
	BroadCast( ERM_UPDATE_TRADE_ITEM_NOT, kPacketNot );
}

IMPL_ON_FUNC( ERM_UPDATE_TRADE_ED_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KTradeFSM::S_WAIT ), ERM_UPDATE_TRADE_ED_ACK, KEGS_UPDATE_TRADE_ED_ACK );

    int iED;
    if( !m_kTradeUserManager.GetTradeED( FIRST_SENDER_UID, iED ) )
    {
        START_LOG( cerr, L"거래 ED 검색 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_TRADE_02;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ED_ACK, kPacket );
        return;
    }

    // 업데이트 하려는 ED 양이 기존과 같으면 그냥 OK를 보낸다.
    if( iED == kPacket_ )
    {
        kPacket.m_iOK = NetError::NET_OK;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ED_ACK, kPacket );
        return;
    }

	if( !m_kTradeUserManager.UpdateTradeED( FIRST_SENDER_UID, kPacket_ ) )
	{
		START_LOG( cerr, L"거래 ED 업데이트 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TRADE_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ED_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_TRADE_ED_ACK, kPacket );

    LIF( m_kTradeUserManager.GetTradeED( FIRST_SENDER_UID, iED ) );
	KEGS_UPDATE_TRADE_ED_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_iED = iED;
    m_kTradeUserManager.GetOfferState( kPacketNot.m_mapOfferState );
	BroadCast( ERM_UPDATE_TRADE_ED_NOT, kPacketNot );
}

IMPL_ON_FUNC_NOPARAM( ERM_OFFER_TRADE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KTradeFSM::S_WAIT ), ERM_OFFER_TRADE_ACK, KEGS_OFFER_TRADE_ACK );

	//{{ 2009. 4. 28  최육사	최대ED제한
	int iED = 0;
	if( !m_kTradeUserManager.GetTradeED( FIRST_SENDER_UID, iED ) )
	{
		START_LOG( cerr, L"유저를 찾을 수 없습니다." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TRADE_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OFFER_TRADE_ACK, kPacket );
		return;
	}

	KTradeUserPtr spOtherUser = m_kTradeUserManager.GetOtherUser( FIRST_SENDER_UID );
	if( spOtherUser == NULL )
	{
		START_LOG( cerr, L"거래 상대를 찾을 수 없습니다" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TRADE_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OFFER_TRADE_ACK, kPacket );
		return;
	}

	if( !spOtherUser->CheckMaxED( iED ) )
	{
		START_LOG( clog, L"거래하려는 상대방의 ED가 최대 보유 가능량을 초과했습니다." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iED );

		kPacket.m_iOK = NetError::ERR_TRADE_18;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OFFER_TRADE_ACK, kPacket );
		return;
	}
	//}}

	if( !m_kTradeUserManager.Offer( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"거래 아이템 등록 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TRADE_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OFFER_TRADE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OFFER_TRADE_ACK, kPacket );

	KEGS_OFFER_TRADE_NOT kPacketNot = FIRST_SENDER_UID;
	BroadCast( ERM_OFFER_TRADE_NOT, kPacketNot );

	if( m_kTradeUserManager.IsOffered() )
	{
        if( m_kTradeUserManager.IsThereNothingToTrade() )
        {
            BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_15 );
            return;
        }

		StateTransition( KTradeFSM::I_TO_CONFIRM );
	}
}

IMPL_ON_FUNC_NOPARAM( ERM_APPROVE_TRADE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KTradeFSM::S_CONFIRM ), ERM_APPROVE_TRADE_ACK, KEGS_APPROVE_TRADE_ACK );

	// 해당 유저정보에 거래 승인 등록!
	if( !m_kTradeUserManager.Approve( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"거래 승인 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TRADE_02;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_APPROVE_TRADE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_APPROVE_TRADE_ACK, kPacket );

	KEGS_APPROVE_TRADE_NOT kPacketNot = FIRST_SENDER_UID;
	BroadCast( ERM_APPROVE_TRADE_NOT, kPacketNot );

	// 두 거래자 모두 승인 하였는가?
	if( m_kTradeUserManager.IsApproved() )
	{
		std::vector< KTradeUserInfo > vecUserList;
		m_kTradeUserManager.GetUserInfoList( vecUserList );
		if( ( int )vecUserList.size() != 2 )
		{
			START_LOG( cerr, L"거래 유저 수가 이상함." )
				<< BUILD_LOG( ( int )vecUserList.size() )
				<< END_LOG;

			BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
			return;
		}

		BOOST_TEST_FOREACH( const KTradeUserInfo&, kInfo, vecUserList )
		{
			KERM_CONFIRM_TRADE_REQ kPacketReq;

			UidType nOtherCID;
			if( !m_kTradeUserManager.GetOtherCID( kInfo.m_iUnitUID, nOtherCID ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeItem( kInfo.m_iUnitUID, kPacketReq.m_vecItemOut ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeItem( nOtherCID, kPacketReq.m_vecItemIn ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeED( kInfo.m_iUnitUID, kPacketReq.m_iEDOut ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeED( nOtherCID, kPacketReq.m_iEDIn ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			SendToGSCharacter( kInfo.m_iGSUID, kInfo.m_iUnitUID, ERM_CONFIRM_TRADE_REQ, kPacketReq );
		}
	}
}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
IMPL_ON_FUNC( ERM_CONFIRM_TRADE_ACK )
{
	if( GetStateID() != KTradeFSM::S_CONFIRM )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : " << L"KTradeFSM::S_CONFIRM" << END_LOG;

		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"인벤토리 확인 중 오류." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_12 );
		return;
	}

	//{{ 2009. 2. 10  최육사	개인거래 버그 예방
	if( !m_kTradeUserManager.Confirm( FIRST_SENDER_UID, kPacket_ ) )
	{
		START_LOG( cerr, L"거래 전 최종 확인 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 거래 깨야 하나..
	}
	//}}

	if( m_kTradeUserManager.IsConfirmed() )
	{
		std::vector< KTradeUserInfo > vecUserList;
		m_kTradeUserManager.GetUserInfoList( vecUserList );
		if( ( int )vecUserList.size() != 2 )
		{
			START_LOG( cerr, L"거래 유저 수가 이상함." )
				<< BUILD_LOG( ( int )vecUserList.size() )
				<< END_LOG;

			BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
			return;
		}

		BOOST_TEST_FOREACH( const KTradeUserInfo&, kConfirmTradeUserInfo, vecUserList )
		{
			KERM_CONFIRM_TRADE_NOT kPacketNot;
			kPacketNot.m_iTradeUID = GetUID();

			UidType nOtherCID;
			if( !m_kTradeUserManager.GetOtherCID( kConfirmTradeUserInfo.m_iUnitUID, nOtherCID ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeItem( kConfirmTradeUserInfo.m_iUnitUID, kPacketNot.m_vecItemOut ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeItem( nOtherCID, kPacketNot.m_vecItemIn ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeED( kConfirmTradeUserInfo.m_iUnitUID, kPacketNot.m_iEDOut ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeED( nOtherCID, kPacketNot.m_iEDIn ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			//{{ 2009. 2. 10  최육사	개인거래 버그 예방
			if( !m_kTradeUserManager.GetUpdateTemp( kConfirmTradeUserInfo.m_iUnitUID, kPacketNot ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}
			//}}
			//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
			kPacketNot.m_iOtherCID = nOtherCID;
#endif SERV_MONITORING_LETTER_AND_TRADE
			//}}

			SendToGSCharacter( kConfirmTradeUserInfo.m_iGSUID, kConfirmTradeUserInfo.m_iUnitUID, ERM_CONFIRM_TRADE_NOT, kPacketNot );
		}

		//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
		// DB 업데이트 상태로 상태 이동!
		StateTransition( KTradeFSM::I_TO_DB_UPDATE );
#endif SERV_TRADE_DDOS_DEFENCE
		//}}

		//{{ DB 처리 이후 거래 로그 기록 - 김민성
#ifdef SERV_WIRTE_TRADE_LOG_AFTER_DB
#else
		// 개인 거래 기록.
		BOOST_TEST_FOREACH( const KTradeUserInfo&, kLogTradeUserInfo, vecUserList )
		{
			// 거래 상대자 찾기
			KTradeUserInfo kOther;
			if( !m_kTradeUserManager.GetOtherTradeUserInfo( kLogTradeUserInfo.m_iUnitUID, kOther ) )
			{
				START_LOG( cerr, L"거래 상대를 알아내지 못함." )
					<< BUILD_LOG( kLogTradeUserInfo.m_iUnitUID )
					<< END_LOG;

				continue;
			}

			// 아이템 거래 로그
			std::vector< KInventoryItemInfo > vecItemOut;
			m_kTradeUserManager.GetTradeItem( kLogTradeUserInfo.m_iUnitUID, vecItemOut );			

			for( unsigned int ui = 0; ui < vecItemOut.size(); ++ui )
			{
				if( vecItemOut[ui].m_iItemUID <= 0 )
				{
					START_LOG( cerr, L"거래된 아이템uid가 이상함." )
						<< BUILD_LOG( kLogTradeUserInfo.m_iUnitUID )
						<< BUILD_LOG( kOther.m_iUnitUID )
						<< BUILD_LOG( vecItemOut[ui].m_iItemUID )
						<< BUILD_LOG( vecItemOut[ui].m_kItemInfo.m_iItemID )
						<< BUILD_LOG( vecItemOut[ui].m_kItemInfo.m_iQuantity )
						<< END_LOG;
				}

				KELOG_INSERT_TRADE_LOG_NOT kPacketLog;
				kPacketLog.m_iFromUserUID = kLogTradeUserInfo.m_iUserUID;
				kPacketLog.m_iFromUnitUID = kLogTradeUserInfo.m_iUnitUID;
				kPacketLog.m_iToUserUID	  = kOther.m_iUserUID;
				kPacketLog.m_iToUnitUID	  = kOther.m_iUnitUID;
				kPacketLog.m_iItemUID  = vecItemOut[ui].m_iItemUID;
				kPacketLog.m_iItemID   = vecItemOut[ui].m_kItemInfo.m_iItemID;
				kPacketLog.m_iQuantity = vecItemOut[ui].m_kItemInfo.m_iQuantity;
				SendToLogDB( ELOG_INSERT_TRADE_LOG_NOT, kPacketLog );
			}

			// ED거래 로그
			int iEDOut;
			m_kTradeUserManager.GetTradeED( kLogTradeUserInfo.m_iUnitUID, iEDOut );

			if( iEDOut > 0 )
			{
				KELOG_INSERT_TRADE_LOG_NOT kPacketLog;
				kPacketLog.m_iFromUserUID = kLogTradeUserInfo.m_iUserUID;
				kPacketLog.m_iFromUnitUID = kLogTradeUserInfo.m_iUnitUID;
				kPacketLog.m_iToUserUID	  = kOther.m_iUserUID;
				kPacketLog.m_iToUnitUID	  = kOther.m_iUnitUID;
				kPacketLog.m_iItemUID	  = 0;
				kPacketLog.m_iItemID	  = 0;
				kPacketLog.m_iQuantity	  = iEDOut;
				SendToLogDB( ELOG_INSERT_TRADE_LOG_NOT, kPacketLog );
			}

		}
#endif SERV_WIRTE_TRADE_LOG_AFTER_DB
		//}}
	}
}
#else

IMPL_ON_FUNC( ERM_CONFIRM_TRADE_ACK )
{
	if( GetStateID() != KTradeFSM::S_CONFIRM )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : " << L"KTradeFSM::S_CONFIRM" << END_LOG;

		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"인벤토리 확인 중 오류." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_12 );
		return;
	}

	//{{ 2009. 2. 10  최육사	개인거래 버그 예방
	if( !m_kTradeUserManager.Confirm( FIRST_SENDER_UID, kPacket_ ) )
	{
		START_LOG( cerr, L"거래 전 최종 확인 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 거래 깨야 하나..
	}
	//}}

	if( m_kTradeUserManager.IsConfirmed() )
	{
		std::vector< KTradeUserInfo > vecUserList;
		m_kTradeUserManager.GetUserInfoList( vecUserList );
		if( ( int )vecUserList.size() != 2 )
		{
			START_LOG( cerr, L"거래 유저 수가 이상함." )
				<< BUILD_LOG( ( int )vecUserList.size() )
				<< END_LOG;

			BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
			return;
		}

		BOOST_TEST_FOREACH( const KTradeUserInfo&, kConfirmTradeUserInfo, vecUserList )
		{
			KERM_CONFIRM_TRADE_NOT kPacketNot;
			kPacketNot.m_iTradeUID = GetUID();

			UidType nOtherCID;
			if( !m_kTradeUserManager.GetOtherCID( kConfirmTradeUserInfo.m_iUnitUID, nOtherCID ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeItem( kConfirmTradeUserInfo.m_iUnitUID, kPacketNot.m_vecItemOut ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeItem( nOtherCID, kPacketNot.m_vecItemIn ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeED( kConfirmTradeUserInfo.m_iUnitUID, kPacketNot.m_iEDOut ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			if( !m_kTradeUserManager.GetTradeED( nOtherCID, kPacketNot.m_iEDIn ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}

			//{{ 2009. 2. 10  최육사	개인거래 버그 예방
			if( !m_kTradeUserManager.GetUpdateTemp( kConfirmTradeUserInfo.m_iUnitUID, kPacketNot ) )
			{
				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_11 );
				return;
			}
			//}}
			//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
			kPacketNot.m_iOtherCID = nOtherCID;
#endif SERV_MONITORING_LETTER_AND_TRADE
			//}}

			SendToGSCharacter( kConfirmTradeUserInfo.m_iGSUID, kConfirmTradeUserInfo.m_iUnitUID, ERM_CONFIRM_TRADE_NOT, kPacketNot );
		}

		//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
		// DB 업데이트 상태로 상태 이동!
		StateTransition( KTradeFSM::I_TO_DB_UPDATE );
#endif SERV_TRADE_DDOS_DEFENCE
		//}}

		// 개인 거래 기록.
		BOOST_TEST_FOREACH( const KTradeUserInfo&, kLogTradeUserInfo, vecUserList )
		{
			// 거래 상대자 찾기
			KTradeUserInfo kOther;
			if( !m_kTradeUserManager.GetOtherTradeUserInfo( kLogTradeUserInfo.m_iUnitUID, kOther ) )
			{
				START_LOG( cerr, L"거래 상대를 알아내지 못함." )
					<< BUILD_LOG( kLogTradeUserInfo.m_iUnitUID )
					<< END_LOG;

				continue;
			}

			// 아이템 거래 로그
			std::vector< KInventoryItemInfo > vecItemOut;
			m_kTradeUserManager.GetTradeItem( kLogTradeUserInfo.m_iUnitUID, vecItemOut );			

			for( unsigned int ui = 0; ui < vecItemOut.size(); ++ui )
			{
				if( vecItemOut[ui].m_iItemUID <= 0 )
				{
					START_LOG( cerr, L"거래된 아이템uid가 이상함." )
						<< BUILD_LOG( kLogTradeUserInfo.m_iUnitUID )
						<< BUILD_LOG( kOther.m_iUnitUID )
						<< BUILD_LOG( vecItemOut[ui].m_iItemUID )
						<< BUILD_LOG( vecItemOut[ui].m_kItemInfo.m_iItemID )
						<< BUILD_LOG( vecItemOut[ui].m_kItemInfo.m_iQuantity )
						<< END_LOG;
				}

				KELOG_INSERT_TRADE_LOG_NOT kPacketLog;
				kPacketLog.m_iFromUserUID = kLogTradeUserInfo.m_iUserUID;
				kPacketLog.m_iFromUnitUID = kLogTradeUserInfo.m_iUnitUID;
				kPacketLog.m_iToUserUID	  = kOther.m_iUserUID;
				kPacketLog.m_iToUnitUID	  = kOther.m_iUnitUID;
				kPacketLog.m_iItemUID  = vecItemOut[ui].m_iItemUID;
				kPacketLog.m_iItemID   = vecItemOut[ui].m_kItemInfo.m_iItemID;
				kPacketLog.m_iQuantity = vecItemOut[ui].m_kItemInfo.m_iQuantity;
				SendToLogDB( ELOG_INSERT_TRADE_LOG_NOT, kPacketLog );
			}

			// ED거래 로그
			int iEDOut;
			m_kTradeUserManager.GetTradeED( kLogTradeUserInfo.m_iUnitUID, iEDOut );

			if( iEDOut > 0 )
			{
				KELOG_INSERT_TRADE_LOG_NOT kPacketLog;
				kPacketLog.m_iFromUserUID = kLogTradeUserInfo.m_iUserUID;
				kPacketLog.m_iFromUnitUID = kLogTradeUserInfo.m_iUnitUID;
				kPacketLog.m_iToUserUID	  = kOther.m_iUserUID;
				kPacketLog.m_iToUnitUID	  = kOther.m_iUnitUID;
				kPacketLog.m_iItemUID	  = 0;
				kPacketLog.m_iItemID	  = 0;
				kPacketLog.m_iQuantity	  = iEDOut;
				SendToLogDB( ELOG_INSERT_TRADE_LOG_NOT, kPacketLog );
			}

		}

		//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
#else
		BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_06 );
#endif SERV_TRADE_DDOS_DEFENCE
		//}}		
	}
}

#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
IMPL_ON_FUNC( ERM_DB_UPDATE_TRADE_RESULT_NOT )
{
	if( GetStateID() != KTradeFSM::S_DB_UPDATE )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : " << L"KTradeFSM::S_CONFIRM" << END_LOG;
		return;
	}

	// DB업데이트 정보 등록!
	if( m_kTradeUserManager.SetDBUpdateInfo( FIRST_SENDER_UID, kPacket_ ) == false )
	{
		START_LOG( cerr, L"거래 DB업데이트 정보 등록 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_09 );
		return;
	}

	if( m_kTradeUserManager.IsDBUpdateReady() )
	{
		std::vector< KTradeUserInfo > vecUserList;
		m_kTradeUserManager.GetUserInfoList( vecUserList );
		if( ( int )vecUserList.size() != 2 )
		{
			START_LOG( cerr, L"거래 유저 수가 이상함." )
				<< BUILD_LOG( ( int )vecUserList.size() )
				<< END_LOG;

			BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
			return;
		}

		KDBE_DB_UPDATE_TRADE_RESULT_REQ kPacketToDB;

		BOOST_TEST_FOREACH( const KTradeUserInfo&, kUserInfo, vecUserList )
		{
			if( m_kTradeUserManager.GetDBUpdateInfo( kUserInfo.m_iUnitUID, kPacketToDB.m_vecDBUpdate ) == false )
			{
				START_LOG( cerr, L"거래 DB처리 정보가 없습니다! 절대 일어나서는 안되는 에러!" )
					<< BUILD_LOG( kUserInfo.m_iUnitUID )
					<< END_LOG;

				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
				return;
			}
		}

		if( static_cast<int>(kPacketToDB.m_vecDBUpdate.size()) != 2 )
		{
			START_LOG( cerr, L"개인 거래인데 두명의 정보가 아니다?!" )
				<< BUILD_LOG( static_cast<int>(kPacketToDB.m_vecDBUpdate.size()) )
				<< END_LOG;
		}		

		SendToGameDB( DBE_DB_UPDATE_TRADE_RESULT_REQ, kPacketToDB );
	}
}

IMPL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_ACK )
{
	if( GetStateID() != KTradeFSM::S_DB_UPDATE )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : " << L"KTradeFSM::S_DB_UPDATE" << END_LOG;
		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB업데이트가 실패하였습니다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_vecDBUpdate.size() )
			<< END_LOG;

		BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
		return;
	}

	//{{ DB 처리 이후 거래 로그 기록 - 김민성
#ifdef SERV_WIRTE_TRADE_LOG_AFTER_DB
	// 개인 거래 기록.
	BOOST_TEST_FOREACH( const KERM_DB_UPDATE_TRADE_RESULT_NOT&, UserInfo, kPacket_.m_vecDBUpdate )
	{
		KTradeUserPtr spTradeUser = m_kTradeUserManager.GetUser( UserInfo.m_iUnitUID );
		if( spTradeUser == NULL )
		{
			START_LOG( cerr, L"거래중인 유저 정보가 없습니다. 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( UserInfo.m_iUnitUID )
				<< END_LOG;
			continue;
		}

		KTradeUserInfo TradeUserInfo;
		spTradeUser->GetTradeUserInfo( TradeUserInfo );

		// 거래 상대자 찾기
		KTradeUserInfo kOther;
		if( !m_kTradeUserManager.GetOtherTradeUserInfo( TradeUserInfo.m_iUnitUID, kOther ) )
		{
			START_LOG( cerr, L"거래 상대를 알아내지 못함." )
				<< BUILD_LOG( UserInfo.m_iUnitUID )
				<< END_LOG;

			continue;
		}

		// 아이템 거래 로그
		std::vector< KInventoryItemInfo > vecItemOut = UserInfo.m_vecUpdated;
		for( unsigned int ui = 0; ui < vecItemOut.size(); ++ui )
		{
			if( vecItemOut[ui].m_iItemUID <= 0 )
			{
				START_LOG( cerr, L"거래된 아이템uid가 이상함." )
					<< BUILD_LOG( TradeUserInfo.m_iUnitUID )
					<< BUILD_LOG( kOther.m_iUnitUID )
					<< BUILD_LOG( vecItemOut[ui].m_iItemUID )
					<< BUILD_LOG( vecItemOut[ui].m_kItemInfo.m_iItemID )
					<< BUILD_LOG( vecItemOut[ui].m_kItemInfo.m_iQuantity )
					<< END_LOG;
			}

			KELOG_INSERT_TRADE_LOG_NOT kPacketLog;
			kPacketLog.m_iFromUserUID = TradeUserInfo.m_iUserUID;
			kPacketLog.m_iFromUnitUID = TradeUserInfo.m_iUnitUID;
			kPacketLog.m_iToUserUID	  = kOther.m_iUserUID;
			kPacketLog.m_iToUnitUID	  = kOther.m_iUnitUID;
			kPacketLog.m_iItemUID  = vecItemOut[ui].m_iItemUID;
			kPacketLog.m_iItemID   = vecItemOut[ui].m_kItemInfo.m_iItemID;
			kPacketLog.m_iQuantity = vecItemOut[ui].m_kItemInfo.m_iQuantity;
			SendToLogDB( ELOG_INSERT_TRADE_LOG_NOT, kPacketLog );
		}

		// ED거래 로그
		int iEDOut;
		m_kTradeUserManager.GetTradeED( TradeUserInfo.m_iUnitUID, iEDOut );

		if( iEDOut > 0 )
		{
			KELOG_INSERT_TRADE_LOG_NOT kPacketLog;
			kPacketLog.m_iFromUserUID = TradeUserInfo.m_iUserUID;
			kPacketLog.m_iFromUnitUID = TradeUserInfo.m_iUnitUID;
			kPacketLog.m_iToUserUID	  = kOther.m_iUserUID;
			kPacketLog.m_iToUnitUID	  = kOther.m_iUnitUID;
			kPacketLog.m_iItemUID	  = 0;
			kPacketLog.m_iItemID	  = 0;
			kPacketLog.m_iQuantity	  = iEDOut;
			SendToLogDB( ELOG_INSERT_TRADE_LOG_NOT, kPacketLog );
		}
	}
#endif SERV_WIRTE_TRADE_LOG_AFTER_DB
	//}}

	BOOST_TEST_FOREACH( const KERM_DB_UPDATE_TRADE_RESULT_NOT&, kPacketNot, kPacket_.m_vecDBUpdate )
	{
		KTradeUserPtr spTradeUser = m_kTradeUserManager.GetUser( kPacketNot.m_iUnitUID );
		if( spTradeUser == NULL )
		{
			START_LOG( cerr, L"거래중인 유저 정보가 없습니다. 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kPacketNot.m_iUnitUID )
				<< END_LOG;
			continue;
		}

		SendToGSCharacter( spTradeUser->GetGSUID(), spTradeUser->GetCID(), ERM_DB_UPDATE_TRADE_COMPLETE_NOT, kPacketNot );
	}

	// 거래 처리가 완전히 종료되었으니 거래방을 닫자!
	BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_06 );
}
#else

//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
IMPL_ON_FUNC( ERM_DB_UPDATE_TRADE_RESULT_NOT )
{
	if( GetStateID() != KTradeFSM::S_DB_UPDATE )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : " << L"KTradeFSM::S_CONFIRM" << END_LOG;
		return;
	}

	// DB업데이트 정보 등록!
	if( m_kTradeUserManager.SetDBUpdateInfo( FIRST_SENDER_UID, kPacket_ ) == false )
	{
		START_LOG( cerr, L"거래 DB업데이트 정보 등록 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_09 );
		return;
	}

	if( m_kTradeUserManager.IsDBUpdateReady() )
	{
		std::vector< KTradeUserInfo > vecUserList;
		m_kTradeUserManager.GetUserInfoList( vecUserList );
		if( ( int )vecUserList.size() != 2 )
		{
			START_LOG( cerr, L"거래 유저 수가 이상함." )
				<< BUILD_LOG( ( int )vecUserList.size() )
				<< END_LOG;

			BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
			return;
		}

		KDBE_DB_UPDATE_TRADE_RESULT_REQ kPacketToDB;

		BOOST_TEST_FOREACH( const KTradeUserInfo&, kUserInfo, vecUserList )
		{
			if( m_kTradeUserManager.GetDBUpdateInfo( kUserInfo.m_iUnitUID, kPacketToDB.m_vecDBUpdate ) == false )
			{
                START_LOG( cerr, L"거래 DB처리 정보가 없습니다! 절대 일어나서는 안되는 에러!" )
					<< BUILD_LOG( kUserInfo.m_iUnitUID )
					<< END_LOG;

				BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
				return;
			}
		}

		SendToGameDB( DBE_DB_UPDATE_TRADE_RESULT_REQ, kPacketToDB );
	}
}

IMPL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_ACK )
{
	if( GetStateID() != KTradeFSM::S_DB_UPDATE )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : " << L"KTradeFSM::S_DB_UPDATE" << END_LOG;
		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB업데이트가 실패하였습니다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_vecDBUpdate.size() )
			<< END_LOG;

		BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_10 );
		return;
	}

	BOOST_TEST_FOREACH( const KERM_DB_UPDATE_TRADE_COMPLETE_NOT&, kPacketNot, kPacket_.m_vecDBUpdate )
	{
        KTradeUserPtr spTradeUser = m_kTradeUserManager.GetUser( kPacketNot.m_iUnitUID );
		if( spTradeUser == NULL )
		{
			START_LOG( cerr, L"거래중인 유저 정보가 없습니다. 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kPacketNot.m_iUnitUID )
				<< END_LOG;
			continue;
		}

		SendToGSCharacter( spTradeUser->GetGSUID(), spTradeUser->GetCID(), ERM_DB_UPDATE_TRADE_COMPLETE_NOT, kPacketNot );
	}

	// 거래 처리가 완전히 종료되었으니 거래방을 닫자!
	BreakTrade( ( int )NetError::NOT_LEAVE_ROOM_REASON_06 );
}

#endif SERV_TRADE_DDOS_DEFENCE
//}}

#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}




