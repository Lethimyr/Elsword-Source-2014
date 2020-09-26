#include ".\PersonalShopAgency.h"

#include "Room/PersonalShopFSM.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "X2Data/XSLRoom.h"
#include "X2data/XSLItem.h"
////{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	#include "LoginServer.h"
#endif SERV_INTEGRATION
////}}
#include "LoginRoomManager.h"

#include "X2Data/XSLSquareUnit.h"

//{{ 2011. 04. 13	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY


#define CLASS_TYPE KPersonalShopAgency

NiImplementRTTI( KPersonalShopAgency, KLoginBaseRoom );

ImplToStringW( KPersonalShopAgency )
{
	return START_TOSTRING_PARENTW( KLoginBaseRoom );
}

KPersonalShopAgency::KPersonalShopAgency(void)
{
	m_kPersonalShopUserManager.Init( 8 );
	m_kPersonalShopItemManager.Clear();
	m_wstrPersonalShopName.clear();

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = SEnum::AST_NONE;
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = CXSLSquareUnit::PST_NONE;
#endif //SERV_UPGRADE_TRADE_SYSTEM
}

KPersonalShopAgency::~KPersonalShopAgency(void)
{
}

void KPersonalShopAgency::CloseRoom()
{
	const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	if( iHostUnitUID != 0 )
	{
		// 대리상인 해고!
		m_kPersonalShopUserManager.DeletePersonalShopAgency( iHostUnitUID );

		// 대리상점 개설자 리스트에서 삭제합니다.
		GetKLoginRoomManager()->DelPShopOwner( iHostUnitUID );
	}

	m_kPersonalShopUserManager.Reset();
	m_kPersonalShopItemManager.Clear();
	m_wstrPersonalShopName.clear();	

	KLoginBaseRoom::CloseRoom();
}

void KPersonalShopAgency::Tick()
{
	KLoginBaseRoom::Tick();

	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
		break;

	case KPersonalShopFSM::S_CLOSE:
		{
			CloseRoom();
		}
		break;

	case KPersonalShopFSM::S_WAIT:
		// 상점개설을 하고 일정시간안에 물품등록을 하지 않을경우 닫아버리자..
		if( m_kPersonalShopUserManager.IsExistPShopAgency() == true )
		{
			if( m_kTimer[TM_WAIT_OTHER].elapsed() >= 1800.0 )
			{
                m_kTimer[TM_WAIT_OTHER].restart();
				const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
				const UidType iHostGSUID = m_kPersonalShopUserManager.GetPShopAgencyHostGSUID();
				BreakPersonalShop( ( int )NetError::NOT_LEAVE_ROOM_REASON_17, iHostGSUID, iHostUnitUID );
			}
		}
		break;

	case KPersonalShopFSM::S_SHOP:
		{
			if( m_kTimer[TM_CHECK_EXPIRATION].elapsed() >= 60.0 )
			{
				m_kTimer[TM_CHECK_EXPIRATION].restart();

				CTime tCurTime = CTime::GetCurrentTime();

				// 대리상점 기간 만료가 되면 자동으로 STOP_SALE처리!
				if( tCurTime > m_tAgencyExpirationDate )
				{
					// 판매 중지 상태로 이동!
					StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

					// DB에 판매 중지 업데이트 하러 가자!
					KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
					kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_21;
					SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );

					// 개설자에게 판매 내용 알려주기위해 전송!
					SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_EXPIRATION );
					break;
				}

				// 개설한지 24시간이 지나면 자동으로 STOP_SALE처리!
				if( tCurTime > m_tAgencyOpenOver24Hours )
				{
					// 판매 중지 상태로 이동!
					StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

					// DB에 판매 중지 업데이트 하러 가자!
					KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
					kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_21;
					SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );

					// 개설자에게 판매 내용 알려주기위해 전송!
					SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_OVER_24_HOURS );
					break;
				}
			}

			//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			CheckZombieUser();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			//}}
		}
		break;

	case KPersonalShopFSM::S_STOP_SALE:
		{
			//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			CheckZombieUser();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
			//}}
		}		
		break;

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}
}

void KPersonalShopAgency::SendPShopAgencyMessage( IN const KEGS_PSHOP_AGENCY_MESSAGE_NOT::PSHOP_AGENCY_MESSAGE_TYPE eType, IN const int iItemID /*= 0*/, IN const int iQuantity /*= 0*/, IN const int iSellED /*= 0*/ )
{
	KEGS_PSHOP_AGENCY_MESSAGE_NOT kPacketNot;
	kPacketNot.m_cMessageType = eType;
	kPacketNot.m_iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	kPacketNot.m_iSellItemID = iItemID;
	kPacketNot.m_iSellQuantity = iQuantity;
	kPacketNot.m_iSellED = iSellED;
	SendToServer( ERM_PSHOP_AGENCY_MESSAGE_NOT, kPacketNot );
}

void KPersonalShopAgency::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

bool KPersonalShopAgency::IsEmpty()
{
	if( m_kPersonalShopUserManager.GetNumMember() < 0 )
	{
		START_LOG( cerr, L"개인 상점 인원 수가 이상함." )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< END_LOG;
	}

	return ( m_kPersonalShopUserManager.GetNumMember() <= 0 );
}

bool KPersonalShopAgency::IsFull()
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

//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
void KPersonalShopAgency::CheckZombieUser()
{
	// 3분마다 한번씩 체크!
	if( m_kTimer[TM_CHECK_ZOMBIE_USER].elapsed() < 60.0 )
		return;

	m_kTimer[TM_CHECK_ZOMBIE_USER].restart();
	
	// 입장 인원이 없으면 패스!
	if( m_kPersonalShopUserManager.GetNumMember() == 0 )
		return;

	// 입장해 있는 모든 유저들에게 체크 패킷 보내기!
	KERM_CHECK_ZOMBIE_USER_REQ kPacket;
	kPacket.m_iRoomUID = GetUID();
	BroadCast( ERM_CHECK_ZOMBIE_USER_REQ, kPacket );
}
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}

bool KPersonalShopAgency::BreakPersonalShop( IN const int iReason, IN const UidType iHostGSUID, IN const UidType iHostUID )
{
	// 1. 판매중인 아이템이 있는지 확인
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == false )
	{
		START_LOG( cerr, L"판매중인 아이템이 있는데 왜 닫으려고 하지? 버그다!!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< END_LOG;
		return false;
	}

	// 2. 진짜 HostUID가 맞는지 확인하자!
	const UidType iCheckHostUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	if( iCheckHostUID != iHostUID )
	{
		START_LOG( cerr, L"개설자 UnitUID가 다른데 요청이 들어왔다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iCheckHostUID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;
		return false;
	}

	// 3. 방안에 있는 유저들에게 알림
	KERM_BREAK_PSHOP_AGENCY_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iHostGSUID = iHostGSUID;
	kPacketNot.m_iHostUID = iHostUID;
	BroadCast( ERM_BREAK_PSHOP_AGENCY_NOT, kPacketNot );

#ifdef DEF_TRADE_BOARD
	// 4. 게시판에도 정보 변경
	std::vector< KSellPersonalShopItemInfo > vecSellItemList;
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, vecSellItemList );
#endif DEF_TRADE_BOARD

	// 5. DB에도 업데이트 하러 가기!
	SendToGameDB( DBE_BREAK_PSHOP_AGENCY_REQ, kPacketNot );

	// 6. 상점 상태 변경
	StateTransition( KPersonalShopFSM::I_TO_CLOSE );
	return true;
}

void KPersonalShopAgency::BroadCastID( u_short usEventID )
{
	BroadCast( usEventID, char() );
}

void KPersonalShopAgency::_BroadCast( const KEvent& kEvent_ )
{
	std::map< UidType, std::set< UidType > > mapUserList;
	m_kPersonalShopUserManager.GetUserList( mapUserList );

	// GameServer에 대한 loop
	std::map< UidType, std::set< UidType > >::iterator mit;
	for( mit = mapUserList.begin(); mit != mapUserList.end(); ++mit )
	{
		KEventPtr spEvent( kEvent_.Clone() );
		spEvent->m_anTrace[0] = mit->first;
		spEvent->m_anTrace[1] = -1;

		// User 각각에 대한 loop
		BOOST_TEST_FOREACH( const UidType, iMemberUnitUID, mit->second )
		{
			LIF( spEvent->m_kDestPerformer.AddUID( iMemberUnitUID ) );
		}

		KncSend( PI_LOGIN_ROOM, GetUID(), spEvent );
	}
}

void KPersonalShopAgency::_BroadCastWithOutOneChar( const KEvent& kEvent_, const UidType iUnitUID )
{
	std::map< UidType, std::set< UidType > > mapUserList;
	m_kPersonalShopUserManager.GetUserList( mapUserList );

	// GameServer에 대한 loop
	std::map< UidType, std::set< UidType > >::iterator mit;
	for( mit = mapUserList.begin(); mit != mapUserList.end(); ++mit )
	{
		KEventPtr spEvent( kEvent_.Clone() );
		spEvent->m_anTrace[0] = mit->first;
		spEvent->m_anTrace[1] = -1;

		// User 각각에 대한 loop
		BOOST_TEST_FOREACH( const UidType, iMemberUnitUID, mit->second )
		{
			if( iMemberUnitUID == iUnitUID )
				continue;

			LIF( spEvent->m_kDestPerformer.AddUID( iMemberUnitUID ) );
		}

		KncSend( PI_LOGIN_ROOM, GetUID(), spEvent );
	}
}

#ifdef DEF_TRADE_BOARD
void KPersonalShopAgency::SendPShopListInfo( IN const int _iCode, IN const std::vector<KSellPersonalShopItemInfo> & _veckInfo, IN const UidType iGSUID /*= 0*/ )
{
	const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	LIF( iHostUnitUID != 0 );

	KERM_PERSONAL_SHOP_INFO_NOT kNot;
	kNot.m_iPSUID = GetUID();
	kNot.m_iHostUnitUID = iHostUnitUID;
	kNot.m_iCode = _iCode;
	kNot.m_vecItemInfo = _veckInfo;

	//아이템 추가 일겨우만 판매자 닉네임을 넣어줌.
	if( _iCode == NetError::ERR_PERSONAL_SHOP_26 )
		m_kPersonalShopUserManager.GetPShopAgencyHostNickName( kNot.m_wstrSellerNickName );

	if( iGSUID == 0 )
	{
		//{{ 2010. 05. 11  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
		BroadCastSameGroupGS( ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#else
		SendToGSServer( 0, ERM_PERSONAL_SHOP_INFO_NOT, kNot );
#endif SERV_INTEGRATION
		//}}
	}
	else
	{
		SendToGSServer( iGSUID, ERM_PERSONAL_SHOP_INFO_NOT, kNot );
	}

	START_LOG( clog, L"상점개설 아이템 전송.!" )
		<< BUILD_LOG( kNot.m_vecItemInfo.size() )
		<< BUILD_LOG( NetError::GetErrStr(_iCode) )
		;
}
void KPersonalShopAgency::SendPShopListInfo( IN const int _iCode, IN const KSellPersonalShopItemInfo& kInfo, IN const UidType iGSUID /*= 0*/ )
{
	std::vector< KSellPersonalShopItemInfo > veckInfo;
	veckInfo.push_back( kInfo );
	SendPShopListInfo( _iCode, veckInfo, iGSUID );
}
#endif DEF_TRADE_BOARD


void KPersonalShopAgency::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

	switch( spEvent_->m_usEventID )
	{
   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );

	CASE( ERM_LOAD_PSHOP_BOARD_INFO_NOT );

	CASE( ERM_OPEN_PSHOP_AGENCY_REQ );
   _CASE( DBE_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	CASE( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT );

	CASE_NOPARAM( ERM_CHECK_OPEN_PSHOP_AGENCY_REQ );

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	_CASE( ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_REQ, KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

   _CASE( ERM_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_REQ );
   _CASE( DBE_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

   _CASE( ERM_JOIN_MY_PSHOP_AGENCY_REQ, KERM_JOIN_PERSONAL_SHOP_REQ );
	CASE( ERM_JOIN_PERSONAL_SHOP_REQ );

   _CASE( ERM_LEAVE_PSHOP_AGENCY_REQ, KERM_LEAVE_PERSONAL_SHOP_REQ );
	CASE( ERM_LEAVE_PERSONAL_SHOP_REQ );

	CASE( ERM_BUY_PERSONAL_SHOP_ITEM_REQ );
   _CASE( DBE_BUY_PSHOP_AGENCY_ITEM_ACK, KDBE_BUY_PSHOP_AGENCY_ITEM_REQ );	

	CASE_NOPARAM( ERM_STOP_SALE_PSHOP_AGENCY_REQ );
	CASE( DBE_STOP_SALE_PSHOP_AGENCY_ACK );

	CASE( ERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT );

   _CASE( ERM_PICK_UP_FROM_PSHOP_AGENCY_REQ, KEGS_PICK_UP_FROM_PSHOP_AGENCY_REQ );
	CASE( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK );
	CASE( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK );
	//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	CASE( ERM_CHECK_ZOMBIE_USER_ACK );
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

    CASE( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ );

	default:
		START_LOG( cerr, L"핸들러가 정의되지 않았음." )
			<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
			<< END_LOG;
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

//게임서버가 죽었을때..처리
_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector< UidType > vecDelUserUID;
	int iCount = m_kPersonalShopUserManager.DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() );

	//현재파티에서 삭제된 유저가 없으면 종료.
	if( iCount == 0 )
		return;

	//먼저 다 빼주고
	BOOST_TEST_FOREACH( const UidType, iDelUnitUID, vecDelUserUID )
	{
		m_kPersonalShopUserManager.LeavePersonalShop( iDelUnitUID );
	}
}

IMPL_ON_FUNC( ERM_LOAD_PSHOP_BOARD_INFO_NOT )
{
	if( GetStateID() == KPersonalShopFSM::S_INIT  || 
		GetStateID() == KPersonalShopFSM::S_WAIT  || 
		GetStateID() == KPersonalShopFSM::S_STOP_SALE )
	{
		return;
	}

	// 판매중인 아이템이 있는지 확인!
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
	{
		START_LOG( cerr, L"판매중인 아이템이 없는데 방 상태가 이상하군?" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}	

	std::vector< KSellPersonalShopItemInfo > vecSellItemList;
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, vecSellItemList, kPacket_.m_iGSUID );
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_INIT ), ERM_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	// 물품등록을 기다리기 위해 타이머 셋팅.(초기 5분)
	m_kTimer[TM_WAIT_OTHER].restart();

	// 기간 만료 체크를 위한 타이머!
	m_kTimer[TM_CHECK_EXPIRATION].restart();

	//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	m_kTimer[TM_CHECK_ZOMBIE_USER].restart();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

	//방은 생성중이라 wait로 변경..
	// 방 상태를 wait 로 바꾸고 물품등록이 올때까지 기다린다.
	StateTransition( KPersonalShopFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////
	// 생성이므로 유닛 정보를 초기화 한다.
	m_kPersonalShopUserManager.Reset();	

	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = false;
#endif DEF_TRADE_BOARD

	// 대리상인 생성!
	if( m_kPersonalShopUserManager.CreatePersonalShopAgency( kInfo ) == false )
	{
		START_LOG( cerr, L"대리 상인 고용 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	// 개인상점 유저 입장!	
//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	if( m_kPersonalShopUserManager.EnterPShopAgencyOwner( kInfo ) == false )
#else
	if( m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) == false )
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}
	{
		START_LOG( cerr, L"대리상인 개설자 유저 추가 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}	

	// ack를 보낸다.
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iPersonalShopUID = GetUID();

	// 상점 타입 저장
	kPacket.m_cPersonalShopType = kPacket_.m_cPersonalShopType;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<SEnum::AGENCY_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<CXSLSquareUnit::PERSONAL_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#endif //SERV_UPGRADE_TRADE_SYSTEM

	// 상점 개설 시각 저장
	m_tAgencyOpenDate = CTime::GetCurrentTime();
	kPacket.m_wstrAgencyOpenDate = (const std::wstring)m_tAgencyOpenDate.Format( _T("%Y-%m-%d %H:%M:%S") );

	// 상점 개설 시각 24시간 이후 저장
	m_tAgencyOpenOver24Hours = m_tAgencyOpenDate + CTimeSpan( 1, 0, 0, 0 );

	// 대리상점 만료일 저장
	m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
	if( KncUtil::ConvertStringToCTime( m_wstrAgencyExpirationDate, m_tAgencyExpirationDate ) == false )
	{
		START_LOG( cerr, L"대리 상점 만료일 저장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	kPacket.m_iGSUID = LAST_SENDER_UID;
	kPacket.m_iUserUID = kInfo.m_iUserUID;
	kPacket.m_iUnitUID = FIRST_SENDER_UID;
	SendToGameDB( DBE_OPEN_PSHOP_AGENCY_REQ, kPacket );
	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 대리상점 개설자 리스트에 개설자 정보를 추가한다!
		GetKLoginRoomManager()->AddPShopOwner( kPacket_.m_iUnitUID, GetUID() );
		
		SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket_ );
		return;
	}

	// 다시 이탈!
	if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"대리상인 나가기 실패." )
			<< END_LOG;
	}

	// 응답 패킷
	kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_OPEN_PSHOP_AGENCY_ACK, kPacket );

	// 방 닫기
	BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, kPacket_.m_iGSUID, kPacket_.m_iUnitUID );
}

IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT )
{
	if( GetStateID() != KPersonalShopFSM::S_INIT )
	{
		START_LOG( cerr, L"방 상태가 이상합니다! 대리상인을 개설할 수 없습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
        return;
	}

	// 등록하려는 상품이 있는지 체크한다!
	if( kPacket_.m_vecSellItemInfo.empty() == true )
	{
		START_LOG( cerr, L"서버가 직접 상점을 개설하려는데 등록된 상품이 없다! 절대 일어나서는 안되는에러!" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_cPersonalShopType )
			<< BUILD_LOG( kPacket_.m_wstrAgencyExpirationDate )
			<< BUILD_LOG( kPacket_.m_wstrAgencyOpenDate )
			<< BUILD_LOG( kPacket_.m_wstrPersonalShopName )
			<< BUILD_LOG( kPacket_.m_bOnSale );

		KERM_BREAK_PSHOP_AGENCY_NOT kPacketNot;
		kPacketNot.m_iHostUID = kPacket_.m_iUnitUID;
		SendToGameDB( DBE_BREAK_PSHOP_AGENCY_REQ, kPacketNot );

		// 상점을 판매 중지상태로 돌린다!
		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		return;
	}

	// 물품등록을 기다리기 위해 타이머 셋팅.(초기 5분)
	m_kTimer[TM_WAIT_OTHER].restart();

	// 기간 만료 체크를 위한 타이머
	m_kTimer[TM_CHECK_EXPIRATION].restart();

	//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	m_kTimer[TM_CHECK_ZOMBIE_USER].restart();
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}

	//방은 생성중이라 wait로 변경..
	// 방 상태를 wait 로 바꾸고 물품등록이 올때까지 기다린다.
	StateTransition( KPersonalShopFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////

	// 생성이므로 유닛 정보를 초기화 한다.
	m_kPersonalShopUserManager.Reset();	

	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = kPacket_.m_iUnitUID;

	// 대리상인 유저 추가!
	if( m_kPersonalShopUserManager.CreatePersonalShopAgency( kInfo ) == false )
	{
		START_LOG( cerr, L"대리 상인 유저 추가 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	// 상점 타입 저장
#ifdef SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<SEnum::AGENCY_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#else //SERV_UPGRADE_TRADE_SYSTEM
	m_PersonalShopType = static_cast<CXSLSquareUnit::PERSONAL_SHOP_TYPE>(kPacket_.m_cPersonalShopType);
#endif //SERV_UPGRADE_TRADE_SYSTEM

	// 상점 개설 시각 저장
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrAgencyOpenDate, m_tAgencyOpenDate ) == false )
	{
		START_LOG( cerr, L"대리 상점 개설시각 저장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wstrAgencyOpenDate )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	// 상점 개설 시각 24시간 이후 저장
	m_tAgencyOpenOver24Hours = m_tAgencyOpenDate + CTimeSpan( 1, 0, 0, 0 );

	// 대리상점 만료일 저장
	m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
	if( KncUtil::ConvertStringToCTime( m_wstrAgencyExpirationDate, m_tAgencyExpirationDate ) == false )
	{
		START_LOG( cerr, L"대리 상점 만료일 저장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_wstrAgencyExpirationDate )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_CLOSE );
		goto end_proc;
	}

	//상점 간판이름 설정.
	m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;

	// 아이템 정보를 업데이트 한다!
	m_kPersonalShopItemManager.UpdateSellItemBackupDataList( kPacket_.m_vecSellItemInfo );

	// 물품등록이 끝났으면 판매 상태로 바꾼다.
	StateTransition( KPersonalShopFSM::I_TO_SHOP );

	// 대리상점 개설자 리스트에 개설자 정보를 추가한다!
	GetKLoginRoomManager()->AddPShopOwner( kPacket_.m_iUnitUID, GetUID() );

	// 만약 판매중지 상태라면 상태를 바꾼다!
	if( kPacket_.m_bOnSale )
	{
#ifdef DEF_TRADE_BOARD
		//아이템 등록이 되었으니 리스트에 추가.
		std::vector< KSellPersonalShopItemInfo > vecSellItemList;
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, vecSellItemList );		
#endif DEF_TRADE_BOARD
	}
	else
	{
		StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );
	}

	return;

end_proc:
	START_LOG( cerr, L"서버에서 자동으로 대리 상인 개설 실패!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;
	return;
}

IMPL_ON_FUNC_NOPARAM( ERM_CHECK_OPEN_PSHOP_AGENCY_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_CHECK_OPEN_PSHOP_AGENCY_ACK, KERM_CHECK_OPEN_PSHOP_AGENCY_ACK );

	// 현재 물품이 다 팔렸는지 체크!
	if( GetStateID() == KPersonalShopFSM::S_SHOP )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_37;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_55;
	}
 
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_OPEN_PSHOP_AGENCY_ACK, kPacket );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
_IMPL_ON_FUNC( ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_REQ, KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_ACK, KEGS_REG_PSHOP_AGENCY_ITEM_ACK );
    
	//개설자가 아닐경우..
	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"개인상점 개설자가 아닌데 판매물품을 등록 하려고함.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_08;
		goto end_proc;
	}

	//{{ 2008. 9. 11  최육사	개인상점 예외처리
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellPShopItemInfo, kPacket_.m_vecSellItemInfo )
	{
		__int64 iPricePerOne  = static_cast<__int64>(kSellPShopItemInfo.m_iPricePerOne);
		__int64 iItemQuantity = static_cast<__int64>(kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > SEnum::UI_MAX_ED_CHAR_HAVE )
		{
			START_LOG( cout, L"판매 액수 총 합이 캐릭터가 보유 가능한 ED한계치를 넘어섰다. 버그유저로 의심됨.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_23;
			goto end_proc;
		}
	}
	//}}

	//물품이 등록되어 있는데 아직 wait이고 등록패킷이 다시오면 오류메세지를 보내고 shop 상태로 바꾸어준다.
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == false )
	{
		START_LOG( cerr, L"개인상점 물품이 등록되어있는데 다시 등록패킷이 날아옴.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_SHOP );
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_07;
		goto end_proc;
	}

	// 대리상점은 최대 9개까지 물품 등록이 가능
	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > SEnum::PAE_SELL_ITEM_LIMIT_NUM )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_09;
		goto end_proc;
	}

	// 검사 완료했으니 다시 돌아가자!
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

_IMPL_ON_FUNC( ERM_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

	//개설자가 아닐경우..
	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"개인상점 개설자가 아닌데 판매물품을 등록 하려고함.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_08;
		goto end_proc;
	}

	kPacket.m_iOK = NetError::NET_OK;

	//상점 간판이름 설정.
	m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	
	//{{ 2008. 9. 11  최육사	개인상점 예외처리
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellPShopItemInfo, kPacket_.m_vecSellItemInfo )
	{
		__int64 iPricePerOne  = static_cast<__int64>(kSellPShopItemInfo.m_iPricePerOne);
		__int64 iItemQuantity = static_cast<__int64>(kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity);
		__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

		if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > SEnum::UI_MAX_ED_CHAR_HAVE )
		{
			START_LOG( cout, L"판매 액수 총 합이 캐릭터가 보유 가능한 ED한계치를 넘어섰다. 버그유저로 의심됨.." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( iPricePerOne )
				<< BUILD_LOG( iItemQuantity )
				<< BUILD_LOG( iCheckTotalPrice );

			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_23;
			goto end_proc;
		}
	}
	//}}

	//물품이 등록되어 있는데 아직 wait이고 등록패킷이 다시오면 오류메세지를 보내고 shop 상태로 바꾸어준다.
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == false )
	{
		START_LOG( cerr, L"개인상점 물품이 등록되어있는데 다시 등록패킷이 날아옴! 이미 첫번째에 검사를 마쳤다! 절대 호출되어선 안되는 오류!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		StateTransition( KPersonalShopFSM::I_TO_SHOP );
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_07;
		goto end_proc;
	}

	// 대리상점은 최대 9개까지 물품 등록이 가능 --> 15개로 변경. 2013.10
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > SEnum::PAE_SELL_ITEM_LIMIT_NUM )
#else
	if( static_cast<int>(kPacket_.m_vecSellItemInfo.size()) > PSHOP_AGENCY_ENUM::PAE_SELL_ITEM_LIMIT_NUM )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	{
		START_LOG( cerr, L"대리상점 물품 등록 가능 갯수를 초과했다! 먼저번에 검사를 했으므로 절대 호출되어선 안되는 오류!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
			<< BUILD_LOG( kPacket_.m_vecSellItemInfo.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_09;
		goto end_proc;
	}
	
	kPacket.m_iOpeningED = 0;	// 대리상인은 개설비용 없음..
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
#else
	//예외체크 사항이 끝났으면 물품을 등록 시킨다.

	m_kPersonalShopItemManager.UpdateSellItemList( kPacket_.m_vecSellItemInfo );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	// DB에 등록하러 가자!
	kPacket.m_iGSUID = LAST_SENDER_UID;
	kPacket.m_iUnitUID = FIRST_SENDER_UID;
	SendToGameDB( DBE_REG_PSHOP_AGENCY_ITEM_REQ, kPacket );
	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REG_PSHOP_AGENCY_ITEM_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_WAIT ), ERM_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
        kPacket_.m_iOK = NetError::ERR_PERSONAL_SHOP_45;

		START_LOG( cerr, L"대리 상인 물품 DB등록이 실패하였습니다!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// 롤백처리
		m_wstrPersonalShopName = L"";
		m_kPersonalShopItemManager.Clear();
		goto end_proc;
	}

	// 아이템 등록에 성공했다면 대리 상점을 나가자!
	if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"유저가 없는데 개인상점 나가기 요청이 들어왔다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	// 잠깐 보관해두었던 아이템 정보를 초기화 한다.
	m_kPersonalShopItemManager.Clear();

	// 새로 받은 아이템 정보를 업데이트 한다!
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	m_kPersonalShopItemManager.UpdateSellItemList( kPacket_.m_vecAddCompleteItemInfo );
#else
	m_kPersonalShopItemManager.UpdateSellItemList( kPacket_.m_vecSellItemInfo );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	// 물품등록이 끝났으면 판매 상태로 바꾼다.
	StateTransition( KPersonalShopFSM::I_TO_SHOP );

end_proc:
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_REG_PSHOP_AGENCY_ITEM_ACK, kPacket_ );

#ifdef DEF_TRADE_BOARD
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//아이템 등록이 되었으니 리스트에 추가.
		std::vector< KSellPersonalShopItemInfo > vecSellItemList;
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemList );
		SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_26, vecSellItemList );
	}
#endif DEF_TRADE_BOARD    
}

IMPL_ON_FUNC( ERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT )
{
	switch( GetStateID() )
	{
	case KPersonalShopFSM::S_INIT:
	case KPersonalShopFSM::S_CLOSE:
	case KPersonalShopFSM::S_STOP_SALE:
		{
			START_LOG( cerr, L"이미 방이 닫혀있는데 대리상점 기간을 연장하려고 하네?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		return;
	}
    
    // 기간 정보 업데이트!	
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrAgencyExpirationDate, m_tAgencyExpirationDate ) == false )
	{
		START_LOG( cerr, L"대리 상점 만료일 저장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_wstrAgencyExpirationDate )
			<< BUILD_LOG( m_wstrAgencyExpirationDate )
			<< END_LOG;
		return;
	}

	m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
}

_IMPL_ON_FUNC( ERM_JOIN_MY_PSHOP_AGENCY_REQ, KERM_JOIN_PERSONAL_SHOP_REQ )
{
	KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_cJoinType = kPacket_.m_cJoinType;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    kPacket.m_iUsedItemID = kPacket_.m_iUsedItemID;
#endif //SERV_UPGRADE_TRADE_SYSTEM

	switch( kPacket_.m_cJoinType )
	{
	case KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST:
		{
			if( GetStateID() == KPersonalShopFSM::S_STOP_SALE )
			{
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_41;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
				return;
			}
		}
		break;

	case KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_FOR_PICK_UP:
		//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
#else
		{
			if( GetStateID() == KPersonalShopFSM::S_SHOP )
			{
				kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_42;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
				return;
			}
		}
#endif SERV_PSHOP_AGENCY_NEW
		//}}		
		break;

	default:
		{
			START_LOG( cerr, L"이상한 타입으로 입장요청이 들어왔습니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< END_LOG;
		}
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( m_kPersonalShopUserManager.IsPShopAgencyHostUserSameServerGroup( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"개인상점방은 해당 서버군의 유저만 들어올 수 있습니다." )
			<< BUILD_LOG( GetKLoginServer()->GetServerGroupID() )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// 만약 아이템 리스트 요청을 유저가 개설자가 아니라면 실패 시킨다!
	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"대리 상점 개설자만 이용 가능 합니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostGSUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_46;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	//참여자 정보를 셋팅한다.
	KPersonalShopUserInfo kInfo;
	kInfo.m_iUserUID = kPacket_.m_iUserUID;
	kInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kInfo.m_iUnitUID = FIRST_SENDER_UID;
	kInfo.m_iGSUID = LAST_SENDER_UID;
#ifdef DEF_TRADE_BOARD
	kInfo.m_bIsSquare = kPacket_.m_bIsSquare;
#endif DEF_TRADE_BOARD

	// 만약 join을 시도하는 유저가 개설자라면 Enter시키지 않고 지나간다!
	//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	if( m_kPersonalShopUserManager.EnterPShopAgencyOwner( kInfo ) == false )
#else
	if( m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) == false )
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}
	{
		START_LOG( cerr, L"대리상점 개설자 입장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetMaxMember() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// 개설자 닉네임 얻기
	if( m_kPersonalShopUserManager.GetPShopAgencyHostNickName( kPacket.m_wstrNickName ) == false )
	{
		START_LOG( cerr, L"호스트 닉네임 얻기 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_wstrPersonalShopName = m_wstrPersonalShopName;
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	if( kPacket_.m_cJoinType == KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST )
	{
		// 판매중지UI에서는 판매중인 물품만 보여준다.
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, KPersonalShopItemManager::ILT_ONLY_ON_SALE_ITEMS );
	}
	else
	{
		if( GetStateID() == KPersonalShopFSM::S_SHOP )
		{
			// 판매중인 물품받기UI에서는 완판된 물품만 보여준다.
			m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, KPersonalShopItemManager::ILT_ONLY_SOLD_OUT_ITEMS );
		}
		else
		{
			// 판매 중지된 상태에서의 물품받기UI에서는 모든 물품을 다 보여준다.
			m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, KPersonalShopItemManager::ILT_ALL_ITEMS );	
		}		
	}

	if( kPacket.m_vecSellItemInfo.empty() == true )
	{
		if( m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"대리상인 나가기 실패." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_52;
        BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, LAST_SENDER_UID, FIRST_SENDER_UID );

	}
#else
	if( kPacket_.m_cJoinType == KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_SHOW_MY_ITEM_LIST )
	{
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, false ); // 판매중지UI에서는 완판된 물품은 제외한다.
	}
	else
	{	
		m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo, true );	// 물품받기UI에서는 모든 물품을 다 보여준다.
	}
#endif SERV_PSHOP_AGENCY_NEW
	//}}
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
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
	case KPersonalShopFSM::S_STOP_SALE:
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_32;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
		}
		return;
	}
#else
	//VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_JOIN_PERSONAL_SHOP_ACK, KERM_JOIN_PERSONAL_SHOP_ACK );
#endif

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( m_kPersonalShopUserManager.IsPShopAgencyHostUserSameServerGroup( kPacket_.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"개인상점방은 해당 서버군의 유저만 들어올 수 있습니다." )
			<< BUILD_LOG( GetKLoginServer()->GetServerGroupID() )
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

	// 만약 join을 시도하는 유저가 개설자라면 Enter시키지 않고 지나간다!
	//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	if( m_kPersonalShopUserManager.EnterPShopAgencyCustomer( kInfo ) == false )
#else
	if( m_kPersonalShopUserManager.EnterPersonalShop( kInfo ) == false )
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	//}}
	{
		START_LOG( cwarn, L"개인상점 유저 추가 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
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
	kPacket.m_bIsPShopAgency = true;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}

	if( m_kPersonalShopUserManager.GetPShopAgencyHostNickName( kPacket.m_wstrNickName ) == false )
	{
		START_LOG( cerr, L"호스트 닉네임 얻기 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( kPacket.m_vecSellItemInfo );

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_PERSONAL_SHOP_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_PSHOP_AGENCY_REQ, KERM_LEAVE_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 3, KPersonalShopFSM::S_WAIT, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_LEAVE_PSHOP_AGENCY_ACK, KEGS_LEAVE_PSHOP_AGENCY_ACK );

	// 입장한 유저 이탈 처리
	if( m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"유저가 없는데 개인상점 나가기 요청이 들어왔다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PSHOP_AGENCY_ACK, kPacket );
		}
	}
	else
	{
		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PSHOP_AGENCY_ACK, kPacket );
		}
	}

	// 이탈한 유저가 개설자인지 확인!
	const bool bIsHost = m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID );
	if( bIsHost )
	{
		// 판매중인 물품이 하나도 없다면 방을 닫는다!
		if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
		{
			BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, LAST_SENDER_UID, FIRST_SENDER_UID );
		}
	}
}

IMPL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_REQ )
{
	VERIFY_ROOM_STATE( ( 3, KPersonalShopFSM::S_WAIT, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_LEAVE_PERSONAL_SHOP_ACK, KEGS_LEAVE_PERSONAL_SHOP_ACK );

	// 입장한 유저 이탈 처리
	if( m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"유저가 없는데 개인상점 나가기 요청이 들어왔다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PERSONAL_SHOP_ACK, kPacket );
		}
	}
	else
	{
		if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_01 )
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_PERSONAL_SHOP_ACK, kPacket );
		}
	}

	if( m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID ) == true )
	{
		// 호스트가 나갈려고 하는 경우
		START_LOG( cerr, L"개설자가 이 패킷을 왜 쏘나! 버그다! 어쨌든 체크는 하자!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	// 판매 중지 상태라면 구매 실패 에러를 보내자!
	if( GetStateID() == KPersonalShopFSM::S_STOP_SALE )
	{
		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PERSONAL_SHOP_36;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}

	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	// 호스트 정보 얻기
	const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
	if( iHostUnitUID == 0 )
	{
		START_LOG( cerr, L"호스트 정보가 없습니다. 일어나서는 안되는 에러!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}

	if( iHostUnitUID == 0 )
	{
		START_LOG( cerr, L"호스트 정보가 없습니다. 일어나서는 안되는 에러!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}

	// 구매 전 처리
	KDBE_BUY_PSHOP_AGENCY_ITEM_REQ kPacketToDB;

	if( m_kPersonalShopItemManager.PrepareForBuyItem( iHostUnitUID, m_PersonalShopType, kPacket_, kPacketToDB ) == false )
	{
		START_LOG( cwarn, L"구매 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iBuyItemUID )
			<< BUILD_LOG( iHostUnitUID )
			<< BUILD_LOG( m_PersonalShopType )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}	
	
	kPacketToDB.m_iHostUnitUID = iHostUnitUID;
	kPacketToDB.m_iPriceGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iPriceUnitUID = FIRST_SENDER_UID;

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	kPacketToDB.m_kSellItemDBUpdate = kPacketToDB.m_kPriceIntemInfoIN;

	// 증분 값이므로 마이너스 값으로 변환
	kPacketToDB.m_iPriceUnitEDOUT = (-1) * kPacketToDB.m_iPriceUnitEDOUT;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	//////////////////////////////////////////////////////////////////////////	
	//모든 구매사항 체크가 끝났으면 대리 상인 DB로 가서 실제 DB정보를 지우자!
	SendToGameDB( DBE_BUY_PSHOP_AGENCY_ITEM_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( DBE_BUY_PSHOP_AGENCY_ITEM_ACK, KDBE_BUY_PSHOP_AGENCY_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	// 실패한 경우에 대한 예외처리
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB로의 구매 요청 처리가 실패하였습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// 롤백 처리
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		m_kPersonalShopItemManager.Confirm( kPacket_.m_kSellItemDBUpdate.m_iItemUID, false );
#else
		m_kPersonalShopItemManager.Confirm( kPacket_.m_kPriceIntemInfoIN.m_iItemUID, false );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}

		// 에러 반환
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
		return;
	}

	// 구매 성공에 따른 컨펌!
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	if( m_kPersonalShopItemManager.Confirm( kPacket_.m_kSellItemDBUpdate.m_iItemUID, true ) == false )
	{
		START_LOG( cerr, L"컨펌이 실패했다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kSellItemDBUpdate.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
		return;
	}
#else
	if( m_kPersonalShopItemManager.Confirm( kPacket_.m_kPriceIntemInfoIN.m_iItemUID, true ) == false )
	{
		START_LOG( cerr, L"컨펌이 실패했다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kPriceIntemInfoIN.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
		return;
	}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 팔린 물건 리스트 옮기기	

	// 1. 구매자에게 이벤트 전송
	{
		KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kCompleteNot;
		kCompleteNot.m_iSellUnitUID			= 0; // 넣을 필요가 없음!
		kCompleteNot.m_iSellUnitEDIN		= 0; // 넣을 필요가 없음!
		kCompleteNot.m_iSellCommissionED	= 0; // 넣을 필요가 없음!
		kCompleteNot.m_kSellIntemInfoOUT;		 // 넣을 필요가 없음!

		kCompleteNot.m_iPriceUnitUID		= kPacket_.m_iPriceUnitUID;
		kCompleteNot.m_iPriceUnitEDOUT		= kPacket_.m_iPriceUnitEDOUT;
		//{{ 2010. 03. 31  최육사	거래게시판
		kCompleteNot.m_iPriceCommissionED	= kPacket_.m_iPriceCommissionED;
		//}}
		kCompleteNot.m_kPriceIntemInfoIN	= kPacket_.m_kPriceIntemInfoIN;
		//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD
		kCompleteNot.m_bIsSquare = false;
#endif DEF_TRADE_BOARD
		//}}
		// GSUser객체 소멸시 직접 DB업데이트용 정보
		kCompleteNot.m_kItemQuantityUpdateTemp  = kPacket_.m_kItemQuantityUpdateTemp;
		kCompleteNot.m_vecItemInfoTemp			= kPacket_.m_vecItemInfoTemp;
		kCompleteNot.m_iEDTemp					= kPacket_.m_iEDTemp;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PSHOP_AGENCY_ITEM_COMPLETE_NOT, kCompleteNot );

		//#5. 클라이언트에 OK값 전송 - 상점거래 정보가 먼저 업데이트 되어야 버그를 방지할 수 있다!
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( kPacket_.m_iPriceGSUID, kPacket_.m_iPriceUnitUID, ERM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
	}	

	// 2. 상점에 들어와있는 유저들에게 변경된 상점 정보 전달
	{
		const UidType iHostUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
		LIF( iHostUnitUID != 0 );

		KSellPersonalShopItemInfo kSellItemInfo;
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		if( m_kPersonalShopItemManager.GetSellPersonalShopItemInfo( kPacket_.m_kSellItemDBUpdate.m_iItemUID, kSellItemInfo ) == false )
#else
		if( m_kPersonalShopItemManager.GetSellPersonalShopItemInfo( kPacket_.m_kPriceIntemInfoIN.m_iItemUID, kSellItemInfo ) == false )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		{
			START_LOG( cerr, L"아이템 정보가 존재하지 않습니다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kPacket_.m_kPriceIntemInfoIN.m_iItemUID )
				<< END_LOG;
		}

		// 상점 UI에 내용 변경을 위해 전송!
		KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT kUpdateNot;
		kUpdateNot.m_iItemUID = kSellItemInfo.m_kInventoryItemInfo.m_iItemUID;
		kUpdateNot.m_iQuantity = kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity;
		BroadCast( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot );
		//BroadCastWithOutOneChar( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kUpdateNot, iHostUnitUID );

		// 개설자에게 판매 내용 알려주기위해 전송!
		if( m_kPersonalShopItemManager.IsAllItemSoldOut() == true )
		{
			SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_SOLD_OUT, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity, kPacket_.m_iSellUnitEDIn );
		}
		
		SendPShopAgencyMessage( KEGS_PSHOP_AGENCY_MESSAGE_NOT::PAMT_SELL_ITEM, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity, kPacket_.m_iSellUnitEDIn );

		// 개설자에게 우편 전송!
		{
			CStringW cwstrItemID;
			cwstrItemID.Format( L"%d", kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID );

			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = iHostUnitUID;
			kPacketToDB.m_iToUnitUID = iHostUnitUID;
			kPacketToDB.m_iRewardType = KPostItemInfo::LT_SELL_ITEM_PSHOP;
			kPacketToDB.m_wstrMessage = cwstrItemID.GetBuffer();
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}

		//판매한 아이템수량이 없으면 삭제해준다.
		if( kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
		{
			if( kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity == 0 )
			{
#ifdef DEF_TRADE_BOARD
				SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, kSellItemInfo );
#endif DEF_TRADE_BOARD
			}

#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_27, kSellItemInfo );
#endif DEF_TRADE_BOARD
		}
		else
		{
#ifdef DEF_TRADE_BOARD
			SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, kSellItemInfo );
#endif DEF_TRADE_BOARD
		}

		//{{ 2008. 2. 26  최육사  개인상점 로그
		KELOG_INSERT_PERSONAL_SHOP_LOG_NOT kPacketLog;
		kPacketLog.m_cPShopType	  = SEnum::PST_PSHOP_AGENCY;
		kPacketLog.m_bIsSquare	  = false;
		kPacketLog.m_iFromUnitUID = iHostUnitUID;
		kPacketLog.m_iToUnitUID	  = kPacket_.m_iPriceUnitUID;
		kPacketLog.m_iItemUID	  = kUpdateNot.m_iItemUID;
		kPacketLog.m_iItemID	  = kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		kPacketLog.m_iQuantity	  = kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity;
		kPacketLog.m_iED		  = kPacket_.m_iSellUnitEDIn;
		SendToLogDB( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT, kPacketLog );
		//}}
	}

	// 3. 모든 물품이 팔렸을경우 개인상점 닫아주기.
	if( m_kPersonalShopItemManager.IsAllItemSoldOut() == true )
	{
		// 모든 아이템들이 다 팔렸다! 자동으로 판매중지 상태로 들어가자!
		StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

		// DB에 판매 중지 업데이트 하러 가자!
		KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
		kPacketToDB.m_iGSUID = m_kPersonalShopUserManager.GetPShopAgencyHostGSUID();
		kPacketToDB.m_iUnitUID = m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID();
		kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_26;
		SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );
	}
}

IMPL_ON_FUNC_NOPARAM( ERM_STOP_SALE_PSHOP_AGENCY_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_SHOP ), ERM_STOP_SALE_PSHOP_AGENCY_ACK, KEGS_STOP_SALE_PSHOP_AGENCY_ACK );

	const bool bIsHost = m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID );
	if( bIsHost == false )
	{
		START_LOG( cerr, L"개설자도 아니면서 판매 중지를 하네?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_46;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// 판매 중지 상태로 이동!
	StateTransition( KPersonalShopFSM::I_TO_STOP_SALE );

	// DB에 판매 중지 업데이트 하러 가자!
	KDBE_STOP_SALE_PSHOP_AGENCY_REQ kPacketToDB;
	kPacketToDB.m_iGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iUnitUID = FIRST_SENDER_UID;
	kPacketToDB.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_21;
	SendToGameDB( DBE_STOP_SALE_PSHOP_AGENCY_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_STOP_SALE_PSHOP_AGENCY_ACK )
{
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_STOP_SALE_PSHOP_AGENCY_ACK, KEGS_STOP_SALE_PSHOP_AGENCY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리상점 판매 중지 쿼리 호출 실패! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// 일단 다시 상태를 원복 시킨다!
		StateTransition( KPersonalShopFSM::I_TO_SHOP );

		kPacket.m_iOK = kPacket_.m_iOK;
		if( kPacket_.m_iGSUID != 0 )
		{
			SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
		}		
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	if( kPacket_.m_iGSUID != 0 )
	{
		SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
	}	

	// 판매 중지가 성공했다면 현재 접속중인 유저들중 개설자를 제외하고 모두 Break를 보낸다.
	KERM_BREAK_PERSONAL_SHOP_NOT kPacketNot;
	kPacketNot.m_iReason = kPacket_.m_iReason;
	kPacketNot.m_iHostUID = kPacket_.m_iUnitUID;
	BroadCastWithOutOneChar( ERM_BREAK_PERSONAL_SHOP_NOT, kPacketNot, kPacket_.m_iUnitUID );

	// 판매중지가 성공했다면 거래게시판에서도 삭제하자!
	std::vector< KSellPersonalShopItemInfo > vecSellItemInfo;
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemInfo, KPersonalShopItemManager::ILT_ALL_ITEMS );
#else
	m_kPersonalShopItemManager.GetSellPersonalShopItemInfoList( vecSellItemInfo, true );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	
	SendPShopListInfo( NetError::ERR_PERSONAL_SHOP_28, vecSellItemInfo );

	// 우편 전송하기!
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iToUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iRewardType = KPostItemInfo::LT_STOP_SALE_PSHOP;
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( ERM_PICK_UP_FROM_PSHOP_AGENCY_REQ, KEGS_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}

	// 대리 상인 개설자인지 체크!
	const bool bIsHost = m_kPersonalShopUserManager.IsPShopAgencyHost( FIRST_SENDER_UID );
	if( bIsHost == false )
	{
		START_LOG( cerr, L"개설자도 아니면서 물품 가져오기를 하네?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_46;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// 물품 가져오기 요청
	KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketToGS;

	if( m_kPersonalShopItemManager.PrepareForPickUp( kPacket_.m_vecPickUpItemList, kPacketToGS ) == false )
	{
		START_LOG( cerr, L"대리상인 물품 가져오기 준비 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecPickUpItemList.size() )
            << BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	// 물품 가져오기 전에 인벤토리 공간 체크를 한다!
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketToGS );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
IMPL_ON_FUNC( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketToDB;

	// pick up
	if( m_kPersonalShopItemManager.PickUp( kPacket_.m_vecPickUpItemList, kPacketToDB ) == false )
	{
		START_LOG( cerr, L"대리상인 물품 가져오기 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecPickUpItemList.size() )
			<< BUILD_LOG( kPacketToDB.m_vecPickUpItemList.size() )
            << BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iUnitUID = FIRST_SENDER_UID;

	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kItem, kPacketToDB.m_vecPickUpItemList )
	{
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환
		if ( kPacket_.m_cShopType <= SEnum::AST_NORMAL ) 
#else // SERV_UPGRADE_TRADE_SYSTEM
		if ( kPacket_.m_cShopType <= CXSLSquareUnit::PST_NORMAL ) 
#endif // SERV_UPGRADE_TRADE_SYSTEM
        {
            kPacketToDB.m_iSellItemTotalED += ( kItem.m_iTotalSellEDIn * 0.9 );
        }
        else 
        {
            kPacketToDB.m_iSellItemTotalED += kItem.m_iTotalSellEDIn;
        }
        
	}

	SendToGameDB( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	


	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// pick up fail
		m_kPersonalShopItemManager.UnPickUp( kPacket_.m_vecPickUpFailList );

		SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	std::vector<UidType> vecPickUpItemList;
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kItem, kPacket_.m_vecPickUpItemList )
	{
		vecPickUpItemList.push_back( kItem.m_kInventoryItemInfo.m_iItemUID );
	}

	// pick up complete
	KPersonalShopItemManager::ITEM_LIST_TYPE eType;
	if( GetStateID() == KPersonalShopFSM::S_SHOP )
	{
		// 현재 상점이 판매중인 상태라면 완판된 아이템 정보만 얻자!
		eType = KPersonalShopItemManager::ILT_ONLY_SOLD_OUT_ITEMS;
	}
	else
	{
		// 현재 상점이 판매중단인 상태라면 모든 아이템 정보를 얻자!
		eType = KPersonalShopItemManager::ILT_ALL_ITEMS;
	}

	m_kPersonalShopItemManager.PickUpComplete( vecPickUpItemList, kPacket, eType );

	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kItem, kPacket_.m_vecPickUpItemList )
	{
		if( kItem.m_iInventoryItemUID != 0  )
		{
			kItem.m_kInventoryItemInfo.m_iItemUID = kItem.m_iInventoryItemUID;
			kPacket.m_vecPickUpSuccessItemList.push_back( kItem );
		}
	}

	kPacket.m_iTotalSellEDIn = kPacket_.m_iSellItemTotalED;

	// Pick Up 결과를 GSUser에게 보내자!
	kPacket.m_iOK = NetError::NET_OK;

    if ( m_kPersonalShopItemManager.IsEmptySellItemList() == true ) 
    {
        kPacket.m_bRemainSellItem = false;
    }
    else 
    {
        kPacket.m_bRemainSellItem = true;
    }

	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );

	// 판매중인 물품이 하나도 없다면 방을 닫는다!
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
	{
		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, kPacket_.m_iGSUID, kPacket_.m_iUnitUID );
	}
	//{{ 2011. 05. 24	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	else
	{
		// 완판된 아이템이 더 남았나 찾아보자!
		if( m_kPersonalShopItemManager.IsAllItemOnSale() == true )
		{
			// 더 안남았으면 UI가 자동으로 닫히므로 방에서도 이탈처리 하자!
			if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
			{
				START_LOG( cerr, L"대리상인 나가기 실패." )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< END_LOG;
			}
		}
	}
#endif SERV_PSHOP_AGENCY_NEW
	//}}
}
#else
IMPL_ON_FUNC( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketToDB;

	// pick up
	if( m_kPersonalShopItemManager.PickUp( kPacket_.m_vecPickUpItemList, kPacketToDB ) == false )
	{
		START_LOG( cerr, L"대리상인 물품 가져오기 준비 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecPickUpItemList.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iGSUID = LAST_SENDER_UID;
	kPacketToDB.m_iUnitUID = FIRST_SENDER_UID;
	SendToGameDB( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	VERIFY_ROOM_STATE( ( 2, KPersonalShopFSM::S_SHOP, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KPersonalShopFSM::S_STOP_SALE ), ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	// 여기까지 왔으면 성공한거다!

	// pick up fail
	m_kPersonalShopItemManager.UnPickUp( kPacket_.m_vecPickUpFailList );

	// pick up complete
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	KPersonalShopItemManager::ITEM_LIST_TYPE eType;
	if( GetStateID() == KPersonalShopFSM::S_SHOP )
	{
		// 현재 상점이 판매중인 상태라면 완판된 아이템 정보만 얻자!
		eType = KPersonalShopItemManager::ILT_ONLY_SOLD_OUT_ITEMS;
	}
	else
	{
		// 현재 상점이 판매중단인 상태라면 모든 아이템 정보를 얻자!
		eType = KPersonalShopItemManager::ILT_ALL_ITEMS;
	}

	m_kPersonalShopItemManager.PickUpComplete( kPacket_.m_vecPickUpItemList, kPacket, eType );
#else
	m_kPersonalShopItemManager.PickUpComplete( kPacket_.m_vecPickUpItemList, kPacket );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

	// Pick Up 결과를 GSUser에게 보내자!
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( kPacket_.m_iGSUID, kPacket_.m_iUnitUID, ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );

	// 판매중인 물품이 하나도 없다면 방을 닫는다!
	if( m_kPersonalShopItemManager.IsEmptySellItemList() == true )
	{
		BreakPersonalShop( NetError::NOT_LEAVE_ROOM_REASON_21, kPacket_.m_iGSUID, kPacket_.m_iUnitUID );
	}
	//{{ 2011. 05. 24	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	else
	{
		// 완판된 아이템이 더 남았나 찾아보자!
		if( m_kPersonalShopItemManager.IsAllItemOnSale() == true )
		{
			// 더 안남았으면 UI가 자동으로 닫히므로 방에서도 이탈처리 하자!
			if( m_kPersonalShopUserManager.LeavePersonalShop( kPacket_.m_iUnitUID ) == false )
			{
				START_LOG( cerr, L"대리상인 나가기 실패." )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< END_LOG;
			}
		}
	}
#endif SERV_PSHOP_AGENCY_NEW
	//}}
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}


#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
IMPL_ON_FUNC( ERM_CHECK_ZOMBIE_USER_ACK )
{
	switch( kPacket_.m_cZombieCheckType )
	{
	case KERM_CHECK_ZOMBIE_USER_ACK::ZCT_CONNECT_ZOMBIE:
	case KERM_CHECK_ZOMBIE_USER_ACK::ZCT_DISCONNECT_ZOMBIE:
	case KERM_CHECK_ZOMBIE_USER_ACK::ZCT_JOIN_DIF_ROOM:
		{
			// 해당 캐릭터가 남아있는지 검사!
			if( m_kPersonalShopUserManager.IsExistUnitUID( FIRST_SENDER_UID ) == true )
			{
				m_kPersonalShopUserManager.LeavePersonalShop( FIRST_SENDER_UID );

				START_LOG( cout2, L"유령 유저를 삭제 합니다!" )
					<< BUILD_LOGc( kPacket_.m_cZombieCheckType )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetStateIDString() )
					<< BUILD_LOG( m_kPersonalShopUserManager.GetPShopAgencyHostUnitUID() )
					<< BUILD_LOG( m_kPersonalShopUserManager.GetNumMember() )
					<< BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() )
					<< BUILD_LOG( m_kPersonalShopItemManager.IsAllItemSoldOut() )
					<< BUILD_LOG( m_kPersonalShopItemManager.IsAllItemOnSale() )
					<< BUILD_LOG( FIRST_SENDER_UID );
			}
		}
		break;
	}
}
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}


IMPL_ON_FUNC( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ )
{
    std::vector< KSellPShopItemBackupData > vecMissingSellItemInfo;
    BOOST_TEST_FOREACH( KSellPShopItemBackupData&, kItem, kPacket_.m_vecSellItemInfo )
    {
        if( false == m_kPersonalShopItemManager.IsExistItem( kItem.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID ) )
        {
            vecMissingSellItemInfo.push_back( kItem );
        }
        
    }
    if ( !vecMissingSellItemInfo.empty() ) 
    {
        START_LOG( cout, L"대리상점에 등록된 아이템이 DB 아이템과 다른 경우 발생하여 추가" )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( vecMissingSellItemInfo.size() )
            << BUILD_LOG( m_kPersonalShopItemManager.GetSellItemCount() );
            
        m_kPersonalShopItemManager.UpdateSellItemBackupDataList( vecMissingSellItemInfo );
    }
}
