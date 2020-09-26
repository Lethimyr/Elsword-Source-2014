#include ".\PersonalShop.h"

#include "Room/PersonalShopFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "X2data/XSLItem.h"
//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	#include "CnServer.h"
#endif SERV_INTEGRATION
//}}
//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	#include "Enum/Enum.h"
#endif SERV_PSHOP_AGENCY
//}}

#define CLASS_TYPE KPersonalShop

NiImplementRTTI( KPersonalShop, KCnBaseRoom );

ImplToStringW( KPersonalShop )
{
	return START_TOSTRING_PARENTW( KCnBaseRoom );
}

KPersonalShop::KPersonalShop(void)
{
	m_kPersonalShopUserManager.Init( 8 );

	m_wstrPersonalShopName.clear();

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = SEnum::AST_NONE;
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = CXSLSquareUnit::PST_NONE;
#endif //SERV_UPGRADE_TRADE_SYSTEM
}

KPersonalShop::~KPersonalShop(void)
{
}

void KPersonalShop::CloseRoom()
{
	m_kPersonalShopUserManager.Reset();

	m_vecSellItemInfo.clear();
	m_wstrPersonalShopName.clear();

	KCnBaseRoom::CloseRoom();
	return;
}

void KPersonalShop::Tick()
{
	KCnBaseRoom::Tick();

	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
		break;

	case KPersonalShopFSM::S_CLOSE:
		{
			CloseRoom();
			//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
			SendRoomCountInfo( CXSLRoom::RT_PERSONAL_SHOP, false );
#endif SERV_ROOM_COUNT
			//}}
		}
		break;

	case KPersonalShopFSM::S_WAIT:
		// 상점개설을 하고 일정시간안에 물품등록을 하지 않을경우 닫아버리자..
		if( m_kPersonalShopUserManager.GetNumMember() <= 1 )
		{
			if( m_kTimer[TM_WAIT_OTHER].elapsed() >= 1800.0 )
			{
				UidType iHostUID = 0;
				if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == false )
				{
					START_LOG( cerr, L"not take host uid.!" )
						<< END_LOG;
				}
				BreakPersonalShop( ( int )NetError::NOT_LEAVE_ROOM_REASON_17, iHostUID );
			}
		}
		break;

	case KPersonalShopFSM::S_SHOP:
		break;

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}
}

bool KPersonalShop::IsEmpty()
{
	if( m_kPersonalShopUserManager.GetNumMember() < 0 )
	{
		START_LOG( cerr, L"개인 상점 인원 수가 이상함." )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< END_LOG;
	}

	return ( m_kPersonalShopUserManager.GetNumMember() <= 0 );
}

bool KPersonalShop::IsFull()
{
	if( m_kPersonalShopUserManager.GetNumMember() > m_kPersonalShopUserManager.GetMaxMember() )
	{
		START_LOG( cerr, L"개인상점 인원 수가 이상함." )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< END_LOG;
	}

	return ( m_kPersonalShopUserManager.GetNumMember() >= m_kPersonalShopUserManager.GetMaxMember() );
}

void KPersonalShop::BreakPersonalShop( int iReason, UidType iHostUID )
{
	KERM_BREAK_PERSONAL_SHOP_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iHostUID = iHostUID;
	BroadCast( ERM_BREAK_PERSONAL_SHOP_NOT, kPacketNot );

	StateTransition( KPersonalShopFSM::I_TO_CLOSE );

#ifdef DEF_TRADE_BOARD
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, m_vecSellItemInfo );
#endif DEF_TRADE_BOARD
}

void KPersonalShop::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

	switch( spEvent_->m_usEventID )
	{
   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );
	CASE( ERM_OPEN_PERSONAL_SHOP_REQ );
	CASE( ERM_REG_PERSONAL_SHOP_ITEM_REQ );
	CASE( ERM_JOIN_PERSONAL_SHOP_REQ );
   _CASE( ERM_BREAK_PERSONAL_SHOP_REQ, KEGS_BREAK_PERSONAL_SHOP_REQ );
    CASE( ERM_LEAVE_PERSONAL_SHOP_REQ );
	CASE( ERM_BUY_PERSONAL_SHOP_ITEM_REQ );
	CASE( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK );
	//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	CASE( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}
	default:
		//{{ 2011. 04. 06	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		ProcessNativeEvent( spEvent_ );
#else
		START_LOG( cerr, L"핸들러가 정의되지 않았음." )
			<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
			<< END_LOG;
#endif SERV_PSHOP_AGENCY
		//}}		
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

void KPersonalShop::BroadCastID( u_short usEventID )
{
	BroadCast( usEventID, char() );
}

void KPersonalShop::_BroadCast( const KEvent& kEvent_ )
{
	std::map< UidType, std::set< UidType > > mapUserList;
	m_kPersonalShopUserManager.GetUserList( mapUserList );

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

#ifdef DEF_TRADE_BOARD
void KPersonalShop::SendPShopListInfo( int _iCode, std::vector<KSellPersonalShopItemInfo> & _veckInfo )
{
	KERM_PERSONAL_SHOP_INFO_NOT kNot;
	kNot.m_iPSUID = GetUID();
	kNot.m_iCode = _iCode;
	kNot.m_vecItemInfo = _veckInfo;

	//아이템 추가 일겨우만 판매자 닉네임을 넣어줌.
	if( _iCode == NetError::ERR_PERSONAL_SHOP_26 )
		m_kPersonalShopUserManager.GetHostNickName( kNot.m_wstrSellerNickName );

	//{{ 2010. 05. 11  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
	BroadCastSameGroupGS( ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#else
	BroadCastAllGSServer( ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#endif SERV_INTEGRATION
	//}}

	START_LOG( clog, L"상점개설 아이템 전송.!" )
		<< BUILD_LOG( kNot.m_vecItemInfo.size() )
		<< BUILD_LOG( NetError::GetErrStr(_iCode) )
		;
}
void KPersonalShop::SendPShopListInfo( int _iCode, KSellPersonalShopItemInfo & kInfo )
{
	std::vector<KSellPersonalShopItemInfo> veckInfo;
	veckInfo.push_back(kInfo);

	SendPShopListInfo(_iCode, veckInfo);
}
#endif DEF_TRADE_BOARD

//게임서버가 죽었을때..처리
_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<UidType>	vecDelUserUID;
	int iCount = m_kPersonalShopUserManager.DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() );

	//현재파티에서 삭제된 유저가 없으면 종료.
	if( iCount == 0 )
		return;

	//먼저 다 빼주고
	std::vector< UidType >::iterator vit;
	for( vit = vecDelUserUID.begin(); vit != vecDelUserUID.end(); vit++ )
	{
		if( m_kPersonalShopUserManager.IsHost(*vit) == true )
		{
			BreakPersonalShop( ( int )NetError::NOT_LEAVE_ROOM_REASON_09, *vit );
			return;
		}

		m_kPersonalShopUserManager.LeavePersonalShop( *vit );
	}
}

IMPL_ON_FUNC( ERM_OPEN_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_INIT ), ERM_OPEN_PERSONAL_SHOP_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_PERSONAL_SHOP, true );
#endif SERV_ROOM_COUNT
	//}}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//////////////////////////////////////////////////////////////////////////
	// 같은 서버군의 센터에 거래방을 생성한건지 확인!
	if( KBaseServer::GetKObj()->GetServerGroupID() != KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr2, L"다른 서버군의 유저가 센터 개인상점방을 생성하였습니다! 이러면 거래로그 이상하게 남아요!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( KBaseServer::GetKObj()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
			<< END_LOG;
		// 로그만 찍자!
	}
	//////////////////////////////////////////////////////////////////////////	
#endif DEPRECATED_SERVER_GROUP_MASK

	// 물품등록을 기다리기 위해 타이머 셋팅.(초기 5분)
	m_kTimer[TM_WAIT_OTHER].restart();

	//방은 생성중이라 wait로 변경..
	// 방 상태를 wait 로 바꾸고 물품등록이 올때까지 기다린다.
	StateTransition( KPersonalShopFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////

	// 생성이므로 유닛 정보를 초기화 한다.
	m_kPersonalShopUserManager.Reset();

	KPersonalShopUserInfo kInfo;

	// 상점을 개설할수있는 비용이 있는지 검사한다.
	if( kPacket_.m_iED < PS_OPENING_ED )
	{
		START_LOG( clog, L"상점 개설비용이 부족해 상점을 열수없음.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iED )
			<< BUILD_LOG( PS_OPENING_ED );

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_10;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );

		goto end_proc;
	}

	// 생성 요청자를 넣는다.
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = true;
#endif DEF_TRADE_BOARD

	if( !m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) )
	{
		START_LOG( cerr, L"개인상점 유저 추가 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
		
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		
		goto end_proc;
	}

	// ack를 보낸다.
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iPersonalShopUID = GetUID();

	kPacket.m_cPersonalShopType = kPacket_.m_cPersonalShopType;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<SEnum::AGENCY_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<CXSLSquareUnit::PERSONAL_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#endif //SERV_UPGRADE_TRADE_SYSTEM

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_REG_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_REG_PERSONAL_SHOP_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

	std::vector< KSellPersonalShopItemInfo >::const_iterator vit;

	//상점 간판이름 설정.
	m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;

	kPacket.m_iOK = NetError::NET_OK;

	//개설자가 아닐경우..
	if( m_kPersonalShopUserManager.IsHost(FIRST_SENDER_UID) == false )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_08;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REG_PERSONAL_SHOP_ITEM_ACK, kPacket );

		UidType iHostUID = 0;
		m_kPersonalShopUserManager.GetHostUID( iHostUID );
		START_LOG( cerr, L"개인상점 개설자가 아닌데 판매물품을 등록 하려고함.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 2008. 9. 11  최육사	개인상점 예외처리
	for( vit = kPacket_.m_vecSellItemInfo.begin(); vit != kPacket_.m_vecSellItemInfo.end(); ++vit )
	{
		__int64 iPricePerOne  = static_cast<__int64>(vit->m_iPricePerOne);
		__int64 iItemQuantity = static_cast<__int64>(vit->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > 2000000000 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_23;

			START_LOG( cout, L"판매 액수값이 한계치를 넘어섰다. 버그유저로 의심됨.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			goto end_proc;
		}
	}
	//}}

	//물품이 등록되어 있는데 아직 wait이고 등록패킷이 다시오면 오류메세지를 보내고 shop 상태로 바꾸어준다.
	if( m_vecSellItemInfo.size() > 0 )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_07;

		START_LOG( cerr, L"개인상점 물품이 등록되어있는데 다시 등록패킷이 날아옴.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_vecSellItemInfo.size() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_SHOP );

		goto end_proc;
	}

	//현재는 임시로 3개까지만 가능하고 추후 유료화 모델에 따라 판매개수가 달라진다.
	int iSellItemNum = 3;

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	if( m_PersonalShopType == SEnum::AST_PREMIUM )
#else //SERV_UPGRADE_TRADE_SYSTEM
	if( m_PersonalShopType == CXSLSquareUnit::PST_PREMIUM )
#endif //SERV_UPGRADE_TRADE_SYSTEM
		iSellItemNum = 9;

	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > iSellItemNum )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_09;

		goto end_proc;
	}

	kPacket.m_iOpeningED = PS_OPENING_ED;	//개설비용 설정을 임시로 상수설정..
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;

	//예외체크 사항이 끝났으면 물품을 등록 시킨다.
	m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;


	//물품등록이 끝났으면 판매 상태로 바꾼다.
	StateTransition( KPersonalShopFSM::I_TO_SHOP );

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REG_PERSONAL_SHOP_ITEM_ACK, kPacket );

#ifdef DEF_TRADE_BOARD
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		//아이템 등록이 되었으니 리스트에 추가.
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, m_vecSellItemInfo );
	}
#endif DEF_TRADE_BOARD
}

IMPL_ON_FUNC( ERM_JOIN_PERSONAL_SHOP_REQ )
{
#ifdef DEF_TRADE_BOARD
	KERM_JOIN_PERSONAL_SHOP_ACK kPacket;
	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
	case KPersonalShopFSM::S_CLOSE:
	case KPersonalShopFSM::S_WAIT:
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_32;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
		}
		return;
	}
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_JOIN_PERSONAL_SHOP_ACK, KERM_JOIN_PERSONAL_SHOP_ACK );
#endif

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( m_kPersonalShopUserManager.IsHostUserSameServerGroup( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"개인상점방은 해당 서버군의 유저만 들어올 수 있습니다." )
			<< BUILD_LOG( GetKCnServer()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_iUnitUID ) )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	kPacket.m_iOK = NetError::NET_OK;

	//참여자 정보를 셋팅한다.
	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = kPacket_.m_bIsSquare;
#endif DEF_TRADE_BOARD

	if( !m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) )
	{
		START_LOG( cwarn, L"개인상점 유저 추가 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();

		goto end_proc;
	}

	kPacket.m_iPersonalShopUID = GetUID();
	kPacket.m_wstrPersonalShopName = m_wstrPersonalShopName;
	//{{ 2011. 07. 19	최육사	대리상점 구매 수수료 없앤 이벤트
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	kPacket.m_bIsPShopAgency = false;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}

	if( m_kPersonalShopUserManager.GetHostNickName( kPacket.m_wstrNickName ) == false )
	{
		START_LOG( cerr, L"호스트 닉네임 얻기 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	kPacket.m_vecSellItemInfo = m_vecSellItemInfo;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_BREAK_PERSONAL_SHOP_REQ, KEGS_BREAK_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_WAIT, KPersonalShopFSM::S_SHOP ), ERM_BREAK_PERSONAL_SHOP_ACK, KEGS_BREAK_PERSONAL_SHOP_ACK );

	bool bIsHost = m_kPersonalShopUserManager.IsHost( FIRST_SENDER_UID );

	if( !m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"개인상점 나가기 실패." )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_PERSONAL_SHOP_ACK, kPacket );
		}
	}
	else
	{
		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_PERSONAL_SHOP_ACK, kPacket );
		}
	}

	//호스트가 나갈려고 했으므로 상점을 종료시킨다.
	if( bIsHost == true )
		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, FIRST_SENDER_UID );
}

IMPL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_LEAVE_PERSONAL_SHOP_ACK, KEGS_LEAVE_PERSONAL_SHOP_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	if( m_kPersonalShopUserManager.IsHost( FIRST_SENDER_UID ) == true )
	{
		START_LOG( cerr, L"개설자가 LEAVE 를 보냄.!이런.." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, FIRST_SENDER_UID );
		return;
	}

	if( !m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"개인상점 나가기 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
		}
	}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PERSONAL_SHOP_ACK, kPacket );

	//만약 사람이 없다면 방을 닫는다.
	if( m_kPersonalShopUserManager.GetNumMember() <= 0 )
	{
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );

		START_LOG( cerr, L"개인상점 leave 처리인데 사람이 없음.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< BUILD_LOG( m_vecSellItemInfo.size() )
			<< END_LOG;

#ifdef DEF_TRADE_BOARD
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, m_vecSellItemInfo );
#endif DEF_TRADE_BOARD
	}
}

IMPL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	kPacket.m_iOK = NetError::ERR_UNKNOWN;

	// 살려는 아이템을 찾는다.
	u_int uiSellItemIndex = 0;
	for( ; uiSellItemIndex < m_vecSellItemInfo.size(); ++uiSellItemIndex )
	{
		if( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_iItemUID == kPacket_.m_iBuyItemUID )
			break;
	}

	int iTotalPrice = 0;

	// 아이템이 있다면..
	if( uiSellItemIndex < m_vecSellItemInfo.size() )
	{
		// 1. 거래 가능한 카테고리의 아이템인지?
		if( !CXSLInventory::IsTradePossibleCategory( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_cSlotCategory ) )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_17;

			START_LOG( cerr, L"판매중인 아이템이 거래가능한 카테고리가 아니다." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			goto end_proc;
		}		

		// 1. 수량 검사..
		if( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iBuyQuantity )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_14;

			START_LOG( cerr, L"판매물품이 부족함." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kPacket_.m_iBuyQuantity )
				<< END_LOG;

			goto end_proc;
		}

		// 2. 구매 액수의 한계치 검사
		__int64 iPricePerOne  = static_cast<__int64>(m_vecSellItemInfo[uiSellItemIndex].TotalPricePerOne());
		__int64 iItemQuantity = static_cast<__int64>(kPacket_.m_iBuyQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > 2000000000 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_24;

			START_LOG( cout, L"구매 액수값이 한계치를 넘어섰다. 버그유저로 의심됨.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			goto end_proc;
		}

		// 3. 가격 검사..
		iTotalPrice = (m_vecSellItemInfo[uiSellItemIndex].TotalPricePerOne() * kPacket_.m_iBuyQuantity);
#ifdef DEF_TRADE_BOARD
		if( false == m_kPersonalShopUserManager.IsSquare(FIRST_SENDER_UID) )
		{
			// 거래게시판을 통해서 왔으면 수수료를 더 붙인다.
			iTotalPrice += static_cast<int>(iTotalPrice * TD_COMMISSION);
		}
#endif DEF_TRADE_BOARD

		// 4. 구매자가 물품살 ED가 있는지 검사.
		if( iTotalPrice > kPacket_.m_iBuyUnitNowED )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_16;

			START_LOG( clog, L"ED가 부족하여 물품을 구매할수 없음.!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iTotalPrice )
				<< BUILD_LOG( kPacket_.m_iBuyUnitNowED )
				;

			goto end_proc;
		}

		// 5. 구매성공시의 ED값을 구하여 임시 저장
		kPacket_.m_iEDTemp = kPacket_.m_iBuyUnitNowED - iTotalPrice; //( m_vecSellItemInfo[i].TotalPricePerOne() * kPacket_.m_iBuyQuantity ) );

		// 6. 한번에 구매가능한 아이템은 최대 1종류뿐
		if( kPacket_.m_vecItemInfoTemp.size() > 1 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_05;

			START_LOG( cerr, L"한번에 구매가능한 아이템 종류는 한종류뿐인데? 일어날수 없는 에러!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_vecItemInfoTemp.size() )
				<< END_LOG;

			goto end_proc;
		}
		else if( kPacket_.m_vecItemInfoTemp.size() == 1 )
		{
			//{{ 2009. 2. 12  최육사	개인상점 ED버그 방지
			std::vector< KItemInfo >::iterator vit = kPacket_.m_vecItemInfoTemp.begin();
			for( ; vit != kPacket_.m_vecItemInfoTemp.end(); ++vit )
			{
				if( vit->m_iItemID == m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				{
					// GSUser소멸시 강제 업데이트기능으로 DB에 들어갈 아이템 정보 복사
					*vit = m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo.m_kItemInfo;

					// 구매한 수량
					vit->m_iQuantity = kPacket_.m_iBuyQuantity;
					break;
				}
			}

			if( vit == kPacket_.m_vecItemInfoTemp.end() )
			{
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_05;

				START_LOG( cerr, L"유저가 존재하지 않음." )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iBuyQuantity )
					<< BUILD_LOG( kPacket_.m_iBuyItemUID )
					<< END_LOG;

				goto end_proc;
			}
			//}}
		}
	}
	// 없다면.
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;

		START_LOG( cerr, L"판매물품 없음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 2009. 2. 11  최육사	개인상점 ED버그 방지
	if( !m_kPersonalShopUserManager.SetUpdateTemp( FIRST_SENDER_UID, kPacket_.m_kItemQuantityUpdateTemp, kPacket_.m_vecItemInfoTemp, kPacket_.m_iEDTemp ) )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_05;

		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		goto end_proc;
	}
	//}}

	//모든 구매사항 체크가 끝났으면 판매자 gsuser로 가서 판매물품이 있는지 체크하고 온다.
	UidType iHostUID = 0;
	UidType iHostGSUID = 0;

	if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == true )
	{
		if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iHostUID, iHostGSUID ) == true )
		{
			KERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ kReq;
			kReq.m_kSellItemInfo = m_vecSellItemInfo[uiSellItemIndex].m_kInventoryItemInfo;
			kReq.m_iBuyUnitUID	 = FIRST_SENDER_UID;
			kReq.m_iBuyQuantity	 = kPacket_.m_iBuyQuantity;
			kReq.m_iTotalPrice	 = iTotalPrice;

            SendToGSCharacter( iHostGSUID, iHostUID, ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ, kReq );
			return;
		}
		else
		{
			START_LOG( cerr, L"개인상점 판매자 GSUID 얻기 실패.!" )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"개인상점 판매자 UID 얻기 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

		goto end_proc;
	}

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
}

//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
IMPL_ON_FUNC( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	std::vector< KSellPersonalShopItemInfo >::iterator vitItem;

	UidType iBuyUnitGSUID = 0;
	UidType iBuyUnitUID = kPacket_.m_iBuyUnitUID;
	if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iBuyUnitUID, iBuyUnitGSUID ) == false )
	{
		START_LOG( cerr, L"구매자 서버 UID 찾기 실패.!" )
			<< BUILD_LOG( iBuyUnitUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< END_LOG;

		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"개인상점 구매실패.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( iBuyUnitUID )
			<< END_LOG
			;

		kPacket.m_iOK = kPacket_.m_iOK;

		goto end_proc;
	}

	UidType iHostUID = 0;
	UidType iHostGSUID = 0;
	if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == true )
	{
		if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iHostUID, iHostGSUID ) == false )
		{
			START_LOG( cerr, L"개인상점 판매자 GSUID 얻기 실패.!" )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"개인상점 판매자 UID 얻기 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

		goto end_proc;
	}

	//팔려는 아이템을 찾는다.
	for( vitItem = m_vecSellItemInfo.begin(); vitItem != m_vecSellItemInfo.end(); ++vitItem )
	{
		if( vitItem->m_kInventoryItemInfo.m_iItemUID == kPacket_.m_iBuyItemUID )
			break;
	}

	// 아이템이 있다면..
	if( vitItem != m_vecSellItemInfo.end() )
	{
		//수량 검사..
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iBuyQuantity )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_14;

			START_LOG( cwarn, L"판매물품이 부족함." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kPacket_.m_iBuyQuantity )
				<< END_LOG;

			goto end_proc;
		}

		//판매자 & 구매자 각각에게 업데이트 정보 보내주기.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType != CXSLItem::PT_QUANTITY )
		{
			if( kPacket_.m_iBuyQuantity > 1 )
			{
				kPacket_.m_iBuyQuantity = 1;

				START_LOG( cerr, L"수량성 아이템이 아닌데 사는 개수가 1이상임." )
					<< BUILD_LOG( m_wstrPersonalShopName )
					<< BUILD_LOG( iHostUID )
					<< BUILD_LOG( iBuyUnitUID )
					<< BUILD_LOGc( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType )
					<< BUILD_LOG( kPacket_.m_iBuyQuantity )
					<< END_LOG;
			}
		}

		KItemQuantityUpdate kBuyUserUpdateTemp;
		std::vector< KItemInfo > vecBuyUserUpdateTemp;
		int iBuyUserEDTemp = 0;

		if( !m_kPersonalShopUserManager.GetUpdateTemp( iBuyUnitUID, kBuyUserUpdateTemp, vecBuyUserUpdateTemp, iBuyUserEDTemp ) )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_25;

			START_LOG( cerr, L"구매자 UID 찾기 실패.!" )
				<< BUILD_LOG( iBuyUnitUID )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
				<< END_LOG;

			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////
		// DB 로 전송 해야 할 데이터 정리
		KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ kCompleteReq;
		kCompleteReq.m_iSellUnitUID = iHostUID;
		kCompleteReq.m_iSellUnitGSUID = iHostGSUID;
		kCompleteReq.m_iSellUnitEDIN = vitItem->m_iPricePerOne * kPacket_.m_iBuyQuantity;
		kCompleteReq.m_kSellItemInfo = vitItem->m_kInventoryItemInfo;
		kCompleteReq.m_kSellItemInfo.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;
		//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD
		kCompleteReq.m_iBuyCommissionED = 0; // 초기화
		kCompleteReq.m_bBuyUnitIsSquare = m_kPersonalShopUserManager.IsSquare( iBuyUnitUID ); // 구매자의 구매위치 얻기
#endif DEF_TRADE_BOARD
		//}}

		//{{ 2009. 3. 24  최육사	수수료
#ifdef SERV_UPGRADE_TRADE_SYSTEM
		if( m_PersonalShopType != SEnum::AST_PREMIUM )
#else //SERV_UPGRADE_TRADE_SYSTEM
		if( m_PersonalShopType != CXSLSquareUnit::PST_PREMIUM )
#endif //SERV_UPGRADE_TRADE_SYSTEM
		{
			// [참고] 프리미엄 상점이 아니라면 판매가의 10%는 수수료로 차감 지급
			kCompleteReq.m_iSellCommissionED = static_cast<int>(kCompleteReq.m_iSellUnitEDIN * TD_HOST_COMMISSION);
			kCompleteReq.m_iSellUnitEDIN -= kCompleteReq.m_iSellCommissionED;
		}
		//}}

		kCompleteReq.m_iBuyUnitUID = iBuyUnitUID;
		kCompleteReq.m_iBuyUnitGSUID = iBuyUnitGSUID;
		kCompleteReq.m_iBuyUnitEDOUT = vitItem->TotalPricePerOne() * kPacket_.m_iBuyQuantity;
		kCompleteReq.m_kBuyItemInfo = vitItem->m_kInventoryItemInfo;
		kCompleteReq.m_kBuyItemInfo.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;
#ifdef DEF_TRADE_BOARD
		// 거래게시판 수수료
		if( false == m_kPersonalShopUserManager.IsSquare(iBuyUnitUID) )
		{
			kCompleteReq.m_iBuyCommissionED = static_cast<int>(kCompleteReq.m_iBuyUnitEDOUT * TD_COMMISSION);
			kCompleteReq.m_iBuyUnitEDOUT += kCompleteReq.m_iBuyCommissionED;
		}
#endif DEF_TRADE_BOARD

		kCompleteReq.m_iBuyUnitEDOUT = (-1) * kCompleteReq.m_iBuyUnitEDOUT; // 변화값이 되므로 나가는 ED -> 마이너스로 처리

		//kCompleteReq.m_kSellItemQuantityUpdateTemp = kPacket_.m_kItemQuantityUpdateTemp;
		//kCompleteReq.m_vecSellItemInfoTemp = kPacket_.m_vecItemInfoTemp;
		
		//kCompleteReq.m_kBuyItemQuantityUpdateTemp = kBuyUserUpdateTemp;
		//kCompleteReq.m_vecBuyItemInfoTemp = vecBuyUserUpdateTemp;
		
		// DB 로 전송
        SendToGameDB( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ, kCompleteReq );

		// 클라이언트에 OK값 전송 - 상점거래 정보가 먼저 업데이트 되어야 버그를 방지할 수 있다!
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );

		//판매아이템 정보처리 & 상점참여 유저들에게 판매상품 정보 업데이트 처리.
		vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity -= kPacket_.m_iBuyQuantity;

		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
		{
			vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity = 0;
		}
	}
	// 없다면.
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;

		START_LOG( cwarn, L"판매물품 없음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			//<< BUILD_LOG( m_kPersonalShopUserManager.GetHostUID() )			
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK )
{
	// 실패 했으니 수량을 복원 해줘야 한다.
	std::vector< KSellPersonalShopItemInfo >::iterator vitItem;
	//팔려는 아이템을 찾는다.
	for( vitItem = m_vecSellItemInfo.begin(); vitItem != m_vecSellItemInfo.end(); ++vitItem )
	{
		if( vitItem->m_kInventoryItemInfo.m_iItemUID == kPacket_.m_kResult.m_kSellItemInfo.m_iItemUID )
			break;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"개인상점 구매실패.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_kResult.m_iBuyUnitUID )
			<< BUILD_LOG( kPacket_.m_kResult.m_iSellUnitUID )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_25;
		SendToGSCharacter( kPacket_.m_kResult.m_iSellUnitGSUID, kPacket_.m_kResult.m_iSellUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );

		// 아이템이 있다면..
		if( vitItem != m_vecSellItemInfo.end() )
		{
			vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity += kPacket_.m_kResult.m_kSellItemInfo.m_kItemInfo.m_iQuantity;
		}
		return;
	}

	// 아이템이 있다면..
	if( vitItem != m_vecSellItemInfo.end() )
	{
		KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT kUpdateNot;
		kUpdateNot.m_iItemUID = vitItem->m_kInventoryItemInfo.m_iItemUID;
		kUpdateNot.m_iQuantity = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		BroadCast( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot );

		//{{ 2008. 2. 26  최육사  개인상점 로그
		int iItemID = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		//}}		

		//판매한 아이템수량이 없으면 삭제해준다.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity == 0 )
			{
#ifdef DEF_TRADE_BOARD
				SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

				m_vecSellItemInfo.erase( vitItem );
			}

#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_27, *vitItem );
#endif DEF_TRADE_BOARD
		}
		else
		{
#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

			m_vecSellItemInfo.erase( vitItem );
		}

		//모든 물품이 팔렸을경우 개인상점 닫아주기.
		if( m_vecSellItemInfo.empty() == true )
		{
			BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_19, kPacket_.m_kResult.m_iSellUnitUID );
		}

		//{{ 2008. 2. 26  최육사  개인상점 로그
		KELOG_INSERT_PERSONAL_SHOP_LOG_NOT kPacketLog;
		//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		kPacketLog.m_cPShopType	  = SEnum::PST_PERSONAL_SHOP;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2010. 03. 31  최육사	거래게시판
		kPacketLog.m_bIsSquare	  = kPacket_.m_kResult.m_bBuyUnitIsSquare;
		//}}
		kPacketLog.m_iFromUnitUID = kPacket_.m_kResult.m_iSellUnitUID;
		kPacketLog.m_iToUnitUID	  = kPacket_.m_kResult.m_iBuyUnitUID;
		kPacketLog.m_iItemUID	  = kUpdateNot.m_iItemUID;
		kPacketLog.m_iItemID	  = iItemID;
		kPacketLog.m_iQuantity	  = kPacket_.m_kResult.m_kSellItemInfo.m_kItemInfo.m_iQuantity;
		kPacketLog.m_iED		  = kPacket_.m_kResult.m_iSellUnitEDIN;
		SendToLogDB( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT, kPacketLog );
		//}}
	}

	// 판매자에게 이벤트 전송
	SendToGSCharacter( kPacket_.m_kResult.m_iSellUnitGSUID, kPacket_.m_kResult.m_iSellUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacket_.m_kResult );

	// 구매자에게 이벤트 전송
	SendToGSCharacter( kPacket_.m_kResult.m_iBuyUnitGSUID, kPacket_.m_kResult.m_iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacket_.m_kResult );
}

#else
IMPL_ON_FUNC( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	std::vector< KSellPersonalShopItemInfo >::iterator vitItem;

	UidType iBuyUnitGSUID = 0;
	UidType iBuyUnitUID = kPacket_.m_iBuyUnitUID;
	if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iBuyUnitUID, iBuyUnitGSUID ) == false )
	{
		START_LOG( cerr, L"구매자 서버 UID 찾기 실패.!" )
			<< BUILD_LOG( iBuyUnitUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< END_LOG;

		return;
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"개인상점 구매실패.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( iBuyUnitUID )
			<< END_LOG
			;

		kPacket.m_iOK = kPacket_.m_iOK;

		goto end_proc;
	}

	UidType iHostUID = 0;
	UidType iHostGSUID = 0;
	if( m_kPersonalShopUserManager.GetHostUID( iHostUID ) == true )
	{
		if( m_kPersonalShopUserManager.GetPersonalShopUserGS( iHostUID, iHostGSUID ) == false )
		{
			START_LOG( cerr, L"개인상점 판매자 GSUID 얻기 실패.!" )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"개인상점 판매자 UID 얻기 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_wstrPersonalShopName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;

		goto end_proc;
	}

	//팔려는 아이템을 찾는다.
	for( vitItem = m_vecSellItemInfo.begin(); vitItem != m_vecSellItemInfo.end(); ++vitItem )
	{
		if( vitItem->m_kInventoryItemInfo.m_iItemUID == kPacket_.m_iBuyItemUID )
			break;
	}

	// 아이템이 있다면..
	if( vitItem != m_vecSellItemInfo.end() )
	{
		//수량 검사..
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < kPacket_.m_iBuyQuantity )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_14;

			START_LOG( cwarn, L"판매물품이 부족함." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kPacket_.m_iBuyQuantity )
				<< END_LOG;

			goto end_proc;
		}

		//판매자 & 구매자 각각에게 업데이트 정보 보내주기.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType != CXSLItem::PT_QUANTITY )
		{
			if( kPacket_.m_iBuyQuantity > 1 )
			{
				kPacket_.m_iBuyQuantity = 1;

				START_LOG( cerr, L"수량성 아이템이 아닌데 사는 개수가 1이상임." )
					<< BUILD_LOG( m_wstrPersonalShopName )
					<< BUILD_LOG( iHostUID )
					<< BUILD_LOG( iBuyUnitUID )
					<< BUILD_LOGc( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType )
					<< BUILD_LOG( kPacket_.m_iBuyQuantity )
					<< END_LOG;
			}
		}

		KItemQuantityUpdate kBuyUserUpdateTemp;
		std::vector< KItemInfo > vecBuyUserUpdateTemp;
		int iBuyUserEDTemp = 0;

		if( !m_kPersonalShopUserManager.GetUpdateTemp( iBuyUnitUID, kBuyUserUpdateTemp, vecBuyUserUpdateTemp, iBuyUserEDTemp ) )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_25;

			START_LOG( cerr, L"구매자 UID 찾기 실패.!" )
				<< BUILD_LOG( iBuyUnitUID )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
				<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
				<< END_LOG;

			goto end_proc;
		}

		//#1. 판매한 물품정보와 판매금액 정보
		KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kCompleteNot;
		kCompleteNot.m_iSellUnitUID = iHostUID;
		kCompleteNot.m_iSellUnitEDIN = vitItem->m_iPricePerOne * kPacket_.m_iBuyQuantity;
		kCompleteNot.m_kSellIntemInfoOUT = vitItem->m_kInventoryItemInfo;
		kCompleteNot.m_kSellIntemInfoOUT.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;
		kCompleteNot.m_iSellCommissionED = 0; // 초기화
		//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD
		kCompleteNot.m_iPriceCommissionED = 0; // 초기화
		kCompleteNot.m_bIsSquare = m_kPersonalShopUserManager.IsSquare( iBuyUnitUID ); // 구매자의 구매위치 얻기
#endif DEF_TRADE_BOARD
		//}}

		//{{ 2009. 3. 24  최육사	수수료
		if( m_PersonalShopType != SEnum::AST_PREMIUM )
		{
			// [참고] 프리미엄 상점이 아니라면 판매가의 10%는 수수료로 차감 지급
			kCompleteNot.m_iSellCommissionED = static_cast<int>(kCompleteNot.m_iSellUnitEDIN * TD_HOST_COMMISSION);
			kCompleteNot.m_iSellUnitEDIN -= kCompleteNot.m_iSellCommissionED;

			// iEDTemp는 증분값이 아니라 실값임
			kPacket_.m_iEDTemp -= kCompleteNot.m_iSellCommissionED;
		}
		//}}

		//#2. 구매한 물품정보와 구매금액 정보.
		kCompleteNot.m_iPriceUnitUID = iBuyUnitUID;
		kCompleteNot.m_iPriceUnitEDOUT = vitItem->TotalPricePerOne() * kPacket_.m_iBuyQuantity;
#ifdef DEF_TRADE_BOARD
		// 거래게시판 수수료
		if( false == m_kPersonalShopUserManager.IsSquare(iBuyUnitUID) )
		{
			kCompleteNot.m_iPriceCommissionED = static_cast<int>(kCompleteNot.m_iPriceUnitEDOUT * TD_COMMISSION);
			kCompleteNot.m_iPriceUnitEDOUT += kCompleteNot.m_iPriceCommissionED;
		}
#endif DEF_TRADE_BOARD
		kCompleteNot.m_kPriceIntemInfoIN = vitItem->m_kInventoryItemInfo;
		kCompleteNot.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity = kPacket_.m_iBuyQuantity;

		//#3. 판매자에게 이벤트 전송
		kCompleteNot.m_kItemQuantityUpdateTemp = kPacket_.m_kItemQuantityUpdateTemp;
		kCompleteNot.m_vecItemInfoTemp = kPacket_.m_vecItemInfoTemp;
		kCompleteNot.m_iEDTemp = kPacket_.m_iEDTemp;
		SendToGSCharacter( iHostGSUID, iHostUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kCompleteNot );

		//#4. 구매자에게 이벤트 전송
		kCompleteNot.m_kItemQuantityUpdateTemp = kBuyUserUpdateTemp;
		kCompleteNot.m_vecItemInfoTemp = vecBuyUserUpdateTemp;
		kCompleteNot.m_iEDTemp = iBuyUserEDTemp;
		SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kCompleteNot );

		//#5. 클라이언트에 OK값 전송 - 상점거래 정보가 먼저 업데이트 되어야 버그를 방지할 수 있다!
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );


		//판매아이템 정보처리 & 상점참여 유저들에게 판매상품 정보 업데이트 처리.
		vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity -= kPacket_.m_iBuyQuantity;

		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
		{
			//LIF( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity < 0 );

			vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity = 0;
		}

		KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT kUpdateNot;
		kUpdateNot.m_iItemUID = vitItem->m_kInventoryItemInfo.m_iItemUID;
		kUpdateNot.m_iQuantity = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		BroadCast( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot );

		//{{ 2008. 2. 26  최육사  개인상점 로그
		int iItemID = vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		//}}		

		//판매한 아이템수량이 없으면 삭제해준다.
		if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			if( vitItem->m_kInventoryItemInfo.m_kItemInfo.m_iQuantity == 0 )
			{
#ifdef DEF_TRADE_BOARD
				SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

				m_vecSellItemInfo.erase( vitItem );
			}

#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_27, *vitItem );
#endif DEF_TRADE_BOARD
		}
		else
		{
#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, *vitItem );
#endif DEF_TRADE_BOARD

			m_vecSellItemInfo.erase( vitItem );
		}

		//모든 물품이 팔렸을경우 개인상점 닫아주기.
		if( m_vecSellItemInfo.empty() == true )
		{
			BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_19, iHostUID );
		}

		//{{ 2008. 2. 26  최육사  개인상점 로그
		KELOG_INSERT_PERSONAL_SHOP_LOG_NOT kPacketLog;
		//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		kPacketLog.m_cPShopType	  = SEnum::PST_PERSONAL_SHOP;
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2010. 03. 31  최육사	거래게시판
		kPacketLog.m_bIsSquare	  = kCompleteNot.m_bIsSquare;
		//}}
		kPacketLog.m_iFromUnitUID = iHostUID;
		kPacketLog.m_iToUnitUID	  = iBuyUnitUID;
		kPacketLog.m_iItemUID	  = kUpdateNot.m_iItemUID;
		kPacketLog.m_iItemID	  = iItemID;
		kPacketLog.m_iQuantity	  = kPacket_.m_iBuyQuantity;
		kPacketLog.m_iED		  = kCompleteNot.m_iSellUnitEDIN;
		SendToLogDB( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT, kPacketLog );
		//}}

		return;
	}
	// 없다면.
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;

		START_LOG( cwarn, L"판매물품 없음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			//<< BUILD_LOG( m_kPersonalShopUserManager.GetHostUID() )			
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< END_LOG;

		goto end_proc;
	}


end_proc:
	SendToGSCharacter( iBuyUnitGSUID, iBuyUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}





