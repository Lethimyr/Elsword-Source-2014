#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "odbc/Odbc.h"

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	#include "X2Data/XSLRidingPetManager.h"
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
IMPL_ON_FUNC( EGS_CREATE_PET_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CREATE_PET_REQ, EGS_CREATE_PET_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrPetName ) == false )
	{
		KEGS_CREATE_PET_ACK kAck;
		kAck.m_iOK = NetError::ERR_STRING_FILTER_01;
		SendPacket( EGS_CREATE_PET_ACK, kAck );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_CREATE_PET_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );


			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_CREATE_PET_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_CREATE_PET_ACK kPacket;
	KDBE_CREATE_PET_REQ kPacketToDB;

	// 1. 펫 이름 검사
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrPetName ) == true )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// 2. 생명의 결정 아이템이 존재하는지 체크!
	if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// 3. 펫을 생성하는 아이템이 맞는지 확인!
	const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	// 4. 지어준 이름이 valid한지 검사 ( 내 펫 이름이랑 타 유저 펫 이름이랑 같을 수 있음 )
	if( m_kUserPetManager.Handler_EGS_CREATE_PET_REQ( kPacket_, iItemID, kPacketToDB ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// 5. 아이템 삭제!
	KInventoryItemInfo kInvenotyItem;
	if( m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInvenotyItem, KDeletedItemInfo::DR_PET_CREATE_ITEM ) == false )
	{
		START_LOG( cerr, L"아이템이 존재 하는지 검사를 했는데 삭제 실패 했네? 있을 수 없는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		// 예약해두었던 펫 정보를 지우자!
		m_kUserPetManager.ClearReservedPet();

		kPacket.m_iOK = NetError::ERR_PET_00;
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

	// 아이템 통계
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 6. DB로 펫 생성하러 갑시다!
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInvenotyItem );
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_CREATE_PET_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CREATE_PET_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_CREATE_PET_ACK kPacket;

	// 아이템 롤백 체크
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	// 펫 생성 결과 처리
	if( m_kUserPetManager.Handler_DBE_CREATE_PET_ACK( kPacket_ ) == false )
	{
		START_LOG( cerr, L"펫 생성 실패! 여기까지 왔는데 생성 실패는 있을 수 없다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_CREATE_PET_ACK, kPacket );
		return;
	}

#ifdef SERV_FREE_AUTO_LOOTING
	if( kPacket_.m_kPetInfo.m_bFreeAutoLooting == true )
	{
		SetAutoLootingPet( kPacket_.m_kPetInfo.m_iPetUID );
	}
#endif SERV_FREE_AUTO_LOOTING

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kCreatedPetInfo = kPacket_.m_kPetInfo;
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_CREATE_PET_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_GET_PET_LIST_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_PET_LIST_ACK );

	KEGS_GET_PET_LIST_ACK kPacket;

	// 펫 리스트 요청하기
	m_kUserPetManager.Handler_EGS_GET_PET_LIST_REQ( kPacket_, kPacket );

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_GET_PET_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_SUMMON_PET_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SUMMON_PET_REQ, EGS_SUMMON_PET_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_SUMMON_PET_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_SUMMON_PET_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
			{
				START_LOG( cerr, L"거래 광장에서는 펫 소환을 할 수 없습니다!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iSummonPetUID )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_SUMMON_PET_ACK, kAck );
				return;
			}
		}
		else if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			if( GetRoomListID() != 0 )
			{
				START_LOG( cerr, L"대전 로비에서는 펫 소환을 할 수 없습니다!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iSummonPetUID )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_SUMMON_PET_ACK, kAck );
				return;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_SUMMON_PET_ACK kPacket;
	KDBE_SUMMON_PET_REQ kPacketToDB;

	// 펫 소환 요청
	if( m_kUserPetManager.Handler_EGS_SUMMON_PET_REQ( GetThisPtr<KGSUser>(), kPacket_, kPacketToDB ) == false )
	{
		if( kPacket_.m_iSummonPetUID == 0 )
		{
			START_LOG( cerr, L"펫 소환 해제 요청 실패!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< BUILD_LOG( GetRoomListID() )
				<< BUILD_LOG( kPacket_.m_iSummonPetUID )
				<< BUILD_LOG( m_kUserPetManager.GetSummonCount() )
				<< BUILD_LOG( m_kUserPetManager.GetLastSummonedPetUID() )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"펫 소환 요청 실패!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< BUILD_LOG( GetRoomListID() )
				<< BUILD_LOG( kPacket_.m_iSummonPetUID )
				<< BUILD_LOG( m_kUserPetManager.GetSummonCount() )
				<< BUILD_LOG( m_kUserPetManager.GetLastSummonedPetUID() )
				<< END_LOG;
		}

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SUMMON_PET_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 펫 인벤토리 처리!

	// 펫 인벤토리를 비우기 전에 DB로 업데이트할 인벤토리의 변경 정보를 얻자!
	m_kInventory.FlushEnduranceChange( kPacketToDB.m_kItemEnduranceUpdate );
	m_kInventory.FlushPositionChange( kPacketToDB.m_kItemPositionUpdate );
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	// 펫 인벤토리를 비우고 펫 정보에 백업할 아이템 정보를 얻자!
	std::map< UidType, KInventoryItemInfo > mapRemovedPetItem;
	m_kInventory.RemovePetInventory( mapRemovedPetItem );

	// 정상적으로 처리했는지 확인!
	if( kPacketToDB.m_iBeforeSummonPetUID == 0 )
	{
		if( mapRemovedPetItem.size() > 0 )
		{
			START_LOG( cerr, L"소환된 펫은 없었는데 펫 인벤토리에 아이템은 있었다? 절대 일어나서는 안되는 에러!!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( mapRemovedPetItem.size() )
				<< END_LOG;
		}
	}
	else
	{
		// 해당 펫에 최신 인벤토리 정보를 저장하자! -> 이걸 외부로 노출시키는게 께름칙하군.. 방법을 찾아보자..
		if( m_kUserPetManager.UpdatePetInventory( kPacketToDB.m_iBeforeSummonPetUID, mapRemovedPetItem ) == false )
		{
			START_LOG( cerr, L"펫 인벤토리 정보 업데이트 실패!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacketToDB.m_iBeforeSummonPetUID )
				<< BUILD_LOG( mapRemovedPetItem.size() )
				<< END_LOG;
		}
	}	

	// 펫 소환 성공하면 DB로 소환 결과 업데이트 하러 가자!
	kPacketToDB.m_iUnitUID = GetCharUID();
	SendToGameDB( DBE_SUMMON_PET_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_SUMMON_PET_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SUMMON_PET_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_SUMMON_PET_ACK kPacket;
	std::map< UidType, KInventoryItemInfo > mapPetItem;

	// 인벤 정보 DB업데이트 결과 처리
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.RollBackInitEndurance( kPacket_.m_kItemEnduranceUpdate );
	m_kInventory.PositionUpdateComplete( kPacket_.m_kItemPositionUpdate );

	// 펫 소환 결과 처리!
	int iPetCategorySlotSize = 0;
	if( m_kUserPetManager.Handler_DBE_SUMMON_PET_ACK( kPacket_, kPacket.m_kSummonedPetInfo, iPetCategorySlotSize, mapPetItem ) == false )
	{
		START_LOG( cerr, L"펫 소환 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< BUILD_LOG( kPacket_.m_iSummonPetUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SUMMON_PET_ACK, kPacket );
		return;
	}

	// 펫 소환하는 경우에만 펫 인벤토리 초기화 하자!
	if( kPacket.m_kSummonedPetInfo.m_iPetUID > 0 )
	{
		m_kInventory.InitPetInventory( kPacket.m_kSummonedPetInfo.m_iPetUID, iPetCategorySlotSize, mapPetItem, kPacket.m_vecPetInventorySlotInfo );
	}

	// 펫 소환과 관련된 모든 처리를 다 했으니 클라이언트로 결과 통보!
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_SUMMON_PET_ACK, kPacket );

	// 펫 인벤토리가 초기화 될때마다 기간제 아이템 검사를 하자!
	if( kPacket.m_kSummonedPetInfo.m_iPetUID > 0 )
	{
		ExpireItem( true );
	}

	//////////////////////////////////////////////////////////////////////////
	// 펫 소환 결과를 클라에도 알려주고 필드 또는 방안에 있는 주변 유저에게도 알리자!
	//{{ 2008. 12. 3  최육사	필드에서 기간다된 아이템 갱신
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		if( GetFieldUID() <= 0 )
		{
			KEGS_SUMMON_PET_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			if( kPacket_.m_iSummonPetUID != 0 )
			{
				kPacketNot.m_vecPetInfo.push_back( kPacket.m_kSummonedPetInfo );
			}
			SendPacket( ERM_SUMMON_PET_NOT, kPacketNot );
		}
		else
		{
			KERM_SUMMON_PET_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			if( kPacket_.m_iSummonPetUID != 0 )
			{
				kPacketNot.m_vecPetInfo.push_back( kPacket.m_kSummonedPetInfo );
			}
			SendToGSField( ERM_SUMMON_PET_NOT, kPacketNot );
		}
	}
	//}}
	// 장착 타이틀을 룸에서 바꿨으면 룸 멤버들에게 알려준다.
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"룸 UID 이상." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KERM_SUMMON_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID = GetCharUID();
				if( kPacket_.m_iSummonPetUID != 0 )
				{
					kPacketNot.m_vecPetInfo.push_back( kPacket.m_kSummonedPetInfo );
				}
				SendToCnRoom( ERM_SUMMON_PET_NOT, kPacketNot );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// 광장이면 안보낸다!
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방 타입." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}

IMPL_ON_FUNC( ERM_SUMMON_PET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_SUMMON_PET_NOT kPacket;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_vecPetInfo = kPacket_.m_vecPetInfo;
	SendPacket( EGS_SUMMON_PET_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_COMMANDS_FOR_PETS_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_COMMANDS_FOR_PETS_ACK );

	// 유저의 명령에 따른 펫의 행동 결정
	// 유저의 명령은 채팅으로 입력되지만 enum값으로 정의하여 명시적으로 명령을 정의한다.
	KEGS_COMMANDS_FOR_PETS_ACK kPacket;
	CXSLPetManager::PET_ACTION_COMMAND ePetActionResult;

	if( m_kUserPetManager.Handler_EGS_COMMANDS_FOR_PETS_REQ( GetThisPtr<KGSUser>(), kPacket_, ePetActionResult ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_COMMANDS_FOR_PETS_ACK, kPacket );

		// 펫이 소환되지 않았는데 펫 명령이 왔다. 강제로 소환해제 시키자.
		if( kPacket.m_iOK == NetError::ERR_PET_06 )
		{
			START_LOG( cout, L"펫이 소환되지 않았는데 펫 명령이 왔다. 강제로 소환해제 시키자!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );
#else
				<< BUILD_LOG( GetCharUID() );
#endif SERV_PRIVACY_AGREEMENT
			KEGS_SUMMON_PET_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_vecPetInfo.clear();
			SendPacket( EGS_SUMMON_PET_NOT, kPacketNot );
		}
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_COMMANDS_FOR_PETS_ACK, kPacket );

	// 펫 액션을 나를 포함한 주변 유저들에게 보내자!
	SendPetAction( ePetActionResult );
}

IMPL_ON_FUNC( ERM_PET_ACTION_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PET_ACTION_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_DECREASE_PET_SATIETY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT kNot;

	// 펫 포만도 감소 체크!	

	//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
	if( m_kUserPetManager.DecreaseSatietyOnPvpRoom( kPacket_.m_cTeamNumMember, m_kInventory.CheckPetToyItem(), kNot ) == true )
#else
	if( m_kUserPetManager.DecreaseSatietyOnPvpRoom( kPacket_.m_cTeamNumMember, kNot ) == true )
#endif SERV_PET_TOY
		//}}
	{
		kNot.m_iUnitUID = GetCharUID();
		SendToCnRoom( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, kNot );

		// 자동 먹이 기능 체크!
		CheckAutoFeed();

		//{{ 2010. 10. 18	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL
		if( m_kUserPetManager.IsSummonedPetHungry() == true )
		{
			if( GetRoomUID() <= 0 )
			{
				START_LOG( cerr, L"룸 UID 이상." )
					<< BUILD_LOG( GetRoomUID() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
				return;
			}

			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				{
					KERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kPacketNot;
					//{{ 2010. 12. 23	최육사	장비 변경 패킷 타입
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
					kPacketNot.m_cChangeEquippedPacketType = KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT::CEPT_CHANGE_PET;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
					//}}
					kPacketNot.m_UnitUID = GetCharUID();
					GetRoomUserInfo( kPacketNot.m_kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, kPacketNot );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				break;

			default:
				{
					START_LOG( cerr, L"이상한 방 타입." )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;
				}
				break;
			}            
		}
#endif SERV_PET_AURA_SKILL
		//}}
	}
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_NOT, KEGS_CHANGED_PET_SATIETY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGED_PET_SATIETY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_INTIMACY_NOT, KEGS_CHANGED_PET_INTIMACY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGED_PET_SATIETY_INTIMACY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_FEED_PETS_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_FEED_PETS_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_FEED_PETS_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_FEED_PETS_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// 펫 관련 동작 제한
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
			{
				START_LOG( cerr, L"거래 광장에서는 펫 먹이를 줄 수 없습니다!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_FEED_PETS_ACK, kAck );
				return;
			}
		}
		else if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			if( GetRoomListID() != 0 )
			{
				START_LOG( cerr, L"대전 로비에서는 펫 먹이를 줄 수 없습니다!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_FEED_PETS_ACK, kAck );
				return;
			}
		}

		// 먹이를 주려고 하는 아이템이 실제로 존재하는 아이템인지 체크!
		if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 아이템을 먹이로 주려고 했다!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_04;
			SendPacket( EGS_FEED_PETS_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_FEED_PETS_ACK kPacket;
	KEGS_CHANGED_PET_SATIETY_NOT kNotSatiety;
	KEGS_CHANGED_PET_INTIMACY_NOT kNotIntimacy;

	// 펫 먹이 ItemID얻기
	const int iFeedItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	// 펫 먹이주기 체크!
	bool bIsFirstFeed = false;

	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	int	iAddIntimacy = 0;
	if( m_kUserPetManager.Handler_EGS_FEED_PETS_REQ( GetLevel(), iFeedItemID, bIsFirstFeed, kNotSatiety, kNotIntimacy, iAddIntimacy ) == false )
#else
	if( m_kUserPetManager.Handler_EGS_FEED_PETS_REQ( GetLevel(), iFeedItemID, bIsFirstFeed, kNotSatiety, kNotIntimacy ) == false )
#endif SERV_ADD_KEWPIEL_JELLY
	//}}
	{
#ifdef SERV_EVENT_PET_INVENTORY
//먹이 먹이기가 실패 했거나 이미 먹었던 먹이는 안먹으니까 체크 해서 알려주자
//이벤트 펫 먹이가 실패 한건지 체크해서 다른 넷에러를 추가해 주자.
#endif SERV_EVENT_PET_INVENTORY
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_FEED_PETS_ACK, kPacket );

		// 배불러서 안먹는건지 체크해서 그렇다면 주변 유저들에게 액션을 날리자!
		if( kPacket.m_iOK == NetError::ERR_PET_10 )
		{
			SendPetAction( CXSLPetManager::PAC_STUFFED );
		}
		return;
	}

	// 해당 아이템 삭제!
	if( m_kInventory.FeedPetInInventory( kPacket_.m_iItemUID, kPacket.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, L"위에서 아이템이 있는지 체크 했는데 먹이 아이템 삭제 실패했다? 절대 일어나면 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_11;
		SendPacket( EGS_FEED_PETS_ACK, kPacket );
		return;
	}
#ifdef SERV_EVENT_PET_INVENTORY
	///여기까지 왔다 그럼 먹이 먹이기 성공이다. 근데 이벤트 먹이는 친밀도와 관계 없이 펫을 영구로 만들고
	//인벤토리를 활성화 시키기 위함 이기 때문에 여기서 먹이 ID로 조건문 걸고 리턴 시킨다.
	//패킷에는 먹이를 먹었다는 정보를 날려준다. 또 한 DB에 정보를 업데이트 한다.
	if( iFeedItemID == 141000440 )
	{
		// 소환된 펫 정보얻기!
		KUserPetPtr spSummonedPet = m_kUserPetManager.GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"소환 펫의 PetUID값은 있는데 펫 정보는 없네?" )
				<< BUILD_LOG( m_kUserPetManager.GetSummonedPet() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PET_11;
			SendPacket( EGS_FEED_PETS_ACK, kPacket );
			return;
		}
		//여기서 DB에 값 저장해 주자.
		KDBE_EVENT_PET_EVENT_FOOD_EAT_REQ kPacketDB;
		kPacketDB.m_bEventFoodEat = spSummonedPet->IsEventFoodEat();
		kPacketDB.m_iPetID		  = spSummonedPet->GetPetID();
		kPacketDB.m_iPetUID		  = spSummonedPet->GetPetUID();
		kPacketDB.m_wstrPetName	  = spSummonedPet->GetPetName();
		kPacketDB.m_iUnitUID      = GetCharUID();
		kPacketDB.m_vecInventorySlotInfo = kPacket.m_vecInventorySlotInfo;

		SendToGameDB( DBE_EVENT_PET_EVENT_FOOD_EAT_REQ, kPacketDB );
		return;
	}	
#endif SERV_EVENT_PET_INVENTORY
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_sSatiety = kNotSatiety.m_sCurrentSatiety;
	SendPacket( EGS_FEED_PETS_ACK, kPacket );

	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	if( iAddIntimacy != 0 )
	{
		// 소환된 펫 정보얻기!
		KUserPetPtr spSummonedPet = m_kUserPetManager.GetSummonedPet();
		if( spSummonedPet == NULL )
		{
			START_LOG( cerr, L"소환 펫의 PetUID값은 있는데 펫 정보는 없네?" )
				<< BUILD_LOG( m_kUserPetManager.GetSummonedPet() )
				<< END_LOG;
			return;
		}

		KEGS_EAT_SPECIAL_FEED_NOT kPacketNot;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_iPetUID = m_kUserPetManager.GetSummonedPetUID();
		kPacketNot.m_sPercent = static_cast<short>(iAddIntimacy);
		kPacketNot.m_iCurrentIntimacy = spSummonedPet->GetIntimacy();
		SendPacket( EGS_EAT_SPECIAL_FEED_NOT, kPacketNot );
	}
#endif SERV_ADD_KEWPIEL_JELLY
	//}}

	// 첫먹이라면 첫 먹이 패킷을 날리자!
	if( bIsFirstFeed == true )
	{
		KEGS_FIRST_FEED_PETS_NOT kPacketNot;
		kPacketNot.m_bAutoFeed = false;
		SendPacket( EGS_FIRST_FEED_PETS_NOT, kPacketNot );
	}

	// 나를 포함한 주변 유저들에게 액션 정보를 날리자!
	SendPetAction( CXSLPetManager::PAC_EAT );

	//////////////////////////////////////////////////////////////////////////
	// 방이라면 포만도와 친밀도를 알리자!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// 첫 먹이 주기 라면 친밀도 정보를 클라이언트로 전송!
		if( bIsFirstFeed )
		{
			kNotIntimacy.m_iUnitUID = GetCharUID();
			SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );
		}
	}
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"룸 UID 이상." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 10. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				kNotSatiety.m_iUnitUID = GetCharUID();
				SendToCnRoom( ERM_CHANGED_PET_SATIETY_NOT, kNotSatiety );

				// 첫 먹이 주기 라면 친밀도 정보도 브로드캐스팅!
				if( bIsFirstFeed )
				{
					kNotIntimacy.m_iUnitUID = GetCharUID();
					SendToCnRoom( ERM_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );
				}
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// 광장이면 안보낸다!
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방 타입." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}

	//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iFeedItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 아이템입니다!" )
			<< BUILD_LOG( iFeedItemID )
			<< END_LOG;

		return;
	}
	
	m_kUserTitleManager.GivePetFeed( pItemTemplet->m_ItemGrade, GetThisPtr<KGSUser>() );

#endif SERV_ADD_TITLE_CONDITION
	//}}
}

IMPL_ON_FUNC( EGS_SET_AUTO_FEED_PETS_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SET_AUTO_FEED_PETS_ACK );

	KEGS_SET_AUTO_FEED_PETS_ACK kPacket;

	// 자동 먹이 설정
	if( m_kUserPetManager.Handler_EGS_SET_AUTO_FEED_PETS_REQ( kPacket_ ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SET_AUTO_FEED_PETS_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_bAutoFeed = kPacket_.m_bAutoFeed;
	SendPacket( EGS_SET_AUTO_FEED_PETS_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_PET_EVOLUTION_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PET_EVOLUTION_ACK );

	KEGS_PET_EVOLUTION_ACK kPacket;
	int iPetCategorySlotSize = 0;
	bool bIsSummonedPet = false;

	//////////////////////////////////////////////////////////////////////////
	// 펫 관련 동작 제한
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
		{
			START_LOG( cerr, L"거래 광장에서는 펫 진화를 할 수 없습니다!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PET_05;
			SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );
			return;
		}
	}
	else if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		if( GetRoomListID() != 0 )
		{
			START_LOG( cerr, L"대전 로비에서는 펫 진화를 할 수 없습니다!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PET_05;
			SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// 펫 진화
	if( m_kUserPetManager.Handler_EGS_PET_EVOLUTION_REQ( kPacket_.m_iPetUID, kPacket, iPetCategorySlotSize, bIsSummonedPet ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );
		return;
	}

	// 펫 진화 결과를 클라이언트에게 알려주자!
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_PET_EVOLUTION_ACK, kPacket );

	// 소환된 펫이 진화된게 아니라면 여기서 리턴!
	if( bIsSummonedPet == false )
		return;

	// 펫 진화 성공 했다면 인벤토리 크기 변경하자!
	m_kInventory.ResetPetInventorySize( iPetCategorySlotSize );

#ifdef SERV_EVENT_VC
	// 용병 뽀루가 진화했다면 우편으로 아이템 지급
	if( kPacket.m_kEvolutionResultPetInfo.m_iPetID == CXSLPetManager::PUI_PET_MERCENARY_PPORU_EVENT_INT )
	{
		// 성체 진화 보상
		if( kPacket.m_kEvolutionResultPetInfo.m_cEvolutionStep == 2 )
		{
			// 이벤트 보상을 주자!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 51417, //듀얼 마법석 랜덤 큐브		
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
		else if( kPacket.m_kEvolutionResultPetInfo.m_cEvolutionStep == 3 )
		{
			// 이벤트 보상을 주자!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 51418, //아리엘이 플루오르 스톤 교환권	
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif //SERV_EVENT_VC

	//////////////////////////////////////////////////////////////////////////
	// 필드이거나 방이면 알리자!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		if( GetFieldUID() <= 0 )
		{
			KEGS_PET_EVOLUTION_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_kPetInfo = kPacket.m_kEvolutionResultPetInfo;
			SendPacket( EGS_PET_EVOLUTION_NOT, kPacketNot );
		}
		else
		{
			KERM_PET_EVOLUTION_NOT kPacketNot;
			kPacketNot.m_iUnitUID = GetCharUID();
			kPacketNot.m_kEvolutionPetInfo = kPacket.m_kEvolutionResultPetInfo;
			SendToGSField( ERM_PET_EVOLUTION_NOT, kPacketNot );
		}		
	}
	//}}
	// 장착 타이틀을 룸에서 바꿨으면 룸 멤버들에게 알려준다.
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"룸 UID 이상." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 11. 08	박세훈	배틀필드 : 필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KERM_PET_EVOLUTION_NOT kPacketNot;
				kPacketNot.m_iUnitUID = GetCharUID();
				kPacketNot.m_kEvolutionPetInfo = kPacket.m_kEvolutionResultPetInfo;
				SendToCnRoom( ERM_PET_EVOLUTION_NOT, kPacketNot );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// 광장이면 안보낸다!
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방 타입." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}

IMPL_ON_FUNC( ERM_PET_EVOLUTION_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_PET_EVOLUTION_NOT kPacket;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_kPetInfo = kPacket_.m_kEvolutionPetInfo;
	SendPacket( EGS_PET_EVOLUTION_NOT, kPacket );
}

//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
IMPL_ON_FUNC( EGS_CHANGE_PET_NAME_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_PET_NAME_REQ, EGS_CHANGE_PET_NAME_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrPetName ) == false )
	{
		KEGS_CHANGE_PET_NAME_ACK kAck;
		kAck.m_iOK = NetError::ERR_STRING_FILTER_01;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kAck );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_CHANGE_PET_NAME_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_CHANGE_PET_NAME_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_CHANGE_PET_NAME_ACK kPacket;
	KDBE_CHANGE_PET_NAME_REQ kPacketToDB;

	// 0. 소환되어 있는 펫이 존재하는지 검사
	const UidType iSummonedPetUID = m_kUserPetManager.GetSummonedPetUID();
	if( iSummonedPetUID == 0 )
	{
		kPacket.m_iOK = NetError::ERR_PET_06;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 1. 해당 아이템이 존재하는지 체크!
	if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	// 2. 사용한 아이템이 펫 이름 변경권 아이템이 맞는가?
	if( CXSLItem::IsPetNameChangeCashItem( iItemID ) == false )
	{
		START_LOG( cerr, L"펫이름 변경 아이템도 아니면서 사용하려고 했다!" )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_18;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 3-1. 펫 이름 유효 검사
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrPetName ) == true )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 3-2. 펫 이름 글자 길이 체크
	std::string strPetName = KncUtil::toNarrowString( kPacket_.m_wstrPetName );
	if( strPetName.size() > 12 )
	{
		kPacket.m_iOK = NetError::ERR_PET_02;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 3-3. 펫 이름 중복 체크
	if( m_kUserPetManager.IsExistPet( kPacket_.m_wstrPetName ) == true )
	{
		kPacket.m_iOK = NetError::ERR_PET_01;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 4. DB로 보낼 패킷 구성
	if( m_kUserPetManager.GetPetInfo( iSummonedPetUID, kPacketToDB.m_kPetInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_PET_19;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}
	kPacketToDB.m_iItemUID = kPacket_.m_iItemUID;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_kPetInfo.m_wstrPetName = kPacket_.m_wstrPetName;

	// 5. 아이템 삭제!
	KInventoryItemInfo kInvenotyItem;
	if( m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInvenotyItem, KDeletedItemInfo::DR_PET_NAME_CHANGE ) == false )
	{
		START_LOG( cerr, L"아이템이 존재 하는지 검사를 했는데 삭제 실패 했네? 있을 수 없는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_19;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}
	kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInvenotyItem );

	// 아이템 통계
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 6. DB로 펫 이름 변경하러 갑시다!
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_CHANGE_PET_NAME_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_PET_NAME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_PET_NAME_ACK kPacket;

	// 아이템 롤백 체크
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
		return;
	}

	// 혹시나 해서 검사해봄
	if( m_kUserPetManager.IsExistPet( kPacket_.m_kPetInfo.m_iPetUID ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 Pet의 이름이 변경 되었네? 있을 수 없는 에러!" )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_wstrPetName )
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cEvolutionStep )
			<< END_LOG;

		// 그냥 넘어 가자!
	}
	else
	{
		// 펫 이름 변경!
		m_kUserPetManager.GetPet( kPacket_.m_kPetInfo.m_iPetUID )->SetPetName( kPacket_.m_kPetInfo.m_wstrPetName );
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kChangedPetInfo = kPacket_.m_kPetInfo;
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_CHANGE_PET_NAME_ACK, kPacket );
}

#endif SERV_PET_CHANGE_NAME
//}}
#ifdef SERV_EVENT_PET_INVENTORY
IMPL_ON_FUNC( DBE_EVENT_PET_EVENT_FOOD_EAT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	KEGS_FEED_PETS_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		SendPacket( EGS_FEED_PETS_ACK, kPacket );
	}
	else
	{
		kPacket.m_EventFoodEat = kPacket_.m_bEventFoodEat;
		kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
		SendPacket( EGS_FEED_PETS_ACK, kPacket );
	}
}
#endif SERV_EVENT_PET_INVENTORY

#ifdef SERV_PERIOD_PET
_IMPL_ON_FUNC( DBE_RELEASE_PET_ACK, KEGS_RELEASE_PET_ACK )
{
	int iPetID = 0;
	bool bExpire = m_kUserPetManager.IsExpirePet( kPacket_.m_iPetUID, iPetID );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( m_kUserPetManager.ReleasePet( kPacket_.m_iPetUID ) == false )
		{
			// 강제로 소환 해제
			KEGS_SUMMON_PET_REQ kPacketReq;
			KDBE_SUMMON_PET_REQ kDbePacketReq;
			kPacketReq.m_iSummonPetUID = 0;
			m_kUserPetManager.Handler_EGS_SUMMON_PET_REQ( GetThisPtr<KGSUser>(), kPacketReq, kDbePacketReq );

			// 라이딩 펫 소환 결과를 클라에도 알려주고 필드 또는 방안에 있는 주변 유저에게도 알리자!
			if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
				( 0 < GetFieldUID() )
				)
			{
				KEGS_SUMMON_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID		= GetCharUID();
				SendToGSField( ERM_SUMMON_PET_NOT, kPacketNot );
			}

			if( GetStateID() == KGSFSM::S_ROOM && 0 < GetRoomUID() )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_PVP:
				case CXSLRoom::RT_DUNGEON:
				case CXSLRoom::RT_TRAININGCENTER:
					//{{ 2012. 02. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
				case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
					//}}
					{
						KEGS_SUMMON_PET_NOT kPacketNot;
						kPacketNot.m_iUnitUID		= GetCharUID();
						SendToCnRoom( ERM_SUMMON_PET_NOT, kPacketNot );
					}
					break;
				}
			}

			// 언서먼 후 다시 제거 시도
			m_kUserPetManager.ReleasePet( kPacket_.m_iPetUID );
		}

		//if( bExpire )
		{
			// 기한이 만료된 펫의 Release 처리 완료
			KEGS_RELEASE_PET_NOT kPacket;
			kPacket.m_iPetUID	= kPacket_.m_iPetUID;
			kPacket.m_iUnitUID	= GetCharUID();
			kPacket.m_iPetID	= iPetID;
			SendPacket( EGS_RELEASE_PET_NOT, kPacket );
		}
		//else
		//{
		//	// 놓아주기로 인한 Release 처리 완료
		//	SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		//}
	}
	else
	{
		//if( bExpire == false )
		//{
		//	// 놓아주기로 인한 Release 처리 실패
		//	SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		//}
	}
}
#endif SERV_PERIOD_PET

#endif SERV_PET_SYSTEM
//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
IMPL_ON_FUNC( EGS_GET_RIDING_PET_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_GET_RIDING_PET_LIST_ACK kPacket;
	kPacket.m_iOK = m_kUserRidingPetManager.GetUserRidingPetList( kPacket_.m_byteViewPage, kPacket.m_vecRidingPetList );
	
	switch( kPacket.m_iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_GET_RIDING_PET_LIST_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key값을 얻을 수 없습니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	default:
		START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
		break;
	};
}

IMPL_ON_FUNC( EGS_SUMMON_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_SUMMON_RIDING_PET_ACK kPacket;

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			break;

		default:
			if( kPacket_.m_bNeedAck == true )
			{
				kPacket.m_iOK = NetError::ERR_RIDING_PET_12;	// 라이딩 펫을 소환할 수 없는 지역입니다.
				SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
				return;
			}
			break;
		}
	}

	kPacket.m_iOK = m_kUserRidingPetManager.SummonRidingPet( kPacket_.m_iRidingPetUID, kPacket.m_kRidingPetInfo );

	switch( kPacket.m_iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
	case KUserRidingPetManager::RIDING_PET_ALREADY_SUMMONED:
		kPacket.m_iOK = NetError::NET_OK;
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_SUMMON_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key값을 얻을 수 없습니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
				return;
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_06;	// 존재하지 않는 라이딩 펫입니다.
		break;

	case KUserRidingPetManager::RIDING_PET_SOMETHING_SUMMONED:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_07;	// 이미 다른 라이딩 펫이 소환되어 있습니다.
		break;

	case KUserRidingPetManager::RIDING_PET_EMPTY:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_13;	// 보유 중인 라이딩 펫이 없습니다.
		break;

	default:
		START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		break;
	}

	if( kPacket_.m_bNeedAck == true )
	{
		SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		return;
	}

	// 라이딩 펫 소환 결과를 클라에도 알려주고 필드 또는 방안에 있는 주변 유저에게도 알리자!
	if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
		( 0 < GetFieldUID() )
		)
	{
		KEGS_SUMMON_RIDING_PET_NOT kPacketNot;
		kPacketNot.m_iUnitUID		= GetCharUID();
		kPacketNot.m_iRidingPetUID	= kPacket.m_kRidingPetInfo.m_iRidingPetUID;
		kPacketNot.m_usRidingPetID	= kPacket.m_kRidingPetInfo.m_usRindingPetID;
		SendToGSField( ERM_SUMMON_RIDING_PET_NOT, kPacketNot );
	}
	
	if( ( GetStateID() == KGSFSM::S_ROOM ) &&
		( 0 < GetRoomUID() )
		)
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KEGS_SUMMON_RIDING_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID		= GetCharUID();
				kPacketNot.m_iRidingPetUID	= kPacket.m_kRidingPetInfo.m_iRidingPetUID;
				kPacketNot.m_usRidingPetID	= kPacket.m_kRidingPetInfo.m_usRindingPetID;
				SendToCnRoom( ERM_SUMMON_RIDING_PET_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_UNSUMMON_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_UNSUMMON_RIDING_PET_ACK kPacket;
	kPacket.m_iOK = m_kUserRidingPetManager.UnSummonRidingPet( kPacket_.m_iRidingPetUID, kPacket_.m_fStamina );

	switch( kPacket.m_iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		kPacket.m_iOK			= NetError::NET_OK;
		kPacket.m_iRidingPetUID	= kPacket_.m_iRidingPetUID;
		SendPacket( EGS_UNSUMMON_RIDING_PET_ACK, kPacket );
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_UNSUMMON_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key값을 얻을 수 없습니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_UNSUMMON_RIDING_PET_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_ALREADY_UNSUMMONED:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_08;	// 소환되어 있는 라이딩 펫이 없습니다.
		SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_SUMMONED:
		kPacket.m_iOK = NetError::ERR_RIDING_PET_09;	// 해당 펫은 소환되어 있지 않습니다.
		SendPacket( EGS_SUMMON_RIDING_PET_ACK, kPacket );
		break;

	default:
		START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_UNSUMMON_RIDING_PET_ACK, kPacket );
		break;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		return;
	}

	// 라이딩 펫 소환 결과를 클라에도 알려주고 필드 또는 방안에 있는 주변 유저에게도 알리자!
	if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
		( 0 < GetFieldUID() )
		)
	{
		KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
		kPacketNot.m_iUnitUID		= GetCharUID();
		SendToGSField( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
	}

	if( ( GetStateID() == KGSFSM::S_ROOM ) &&
		( 0 < GetRoomUID() )
		)
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 02. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
				kPacketNot.m_iUnitUID		= GetCharUID();
				SendToCnRoom( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_CREATE_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	int iOK = m_kUserRidingPetManager.RidingPetCreateCheck();

	switch( iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		{
			//////////////////////////////////////////////////////////////////////////
			// 예외처리
			{
				KEGS_CREATE_RIDING_PET_ACK kAck;

				//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
				if( m_kInventory.IsLocked() == true )
				{
					START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
						<< BUILD_LOG( GetUID() )
						<< BUILD_LOG( GetName() )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetCharName() );

					kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
					SendPacket( EGS_CREATE_RIDING_PET_ACK, kAck );
					return;
				}
#endif SERV_INVENTORY_LOCK
				//}}
			}
			//////////////////////////////////////////////////////////////////////////

			// 해당 아이템이 존재하는지 체크!
			if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
			{
				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ITEM_04;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
				return;
			}

			// 라이딩 펫을 생성하는 아이템이 맞는지 확인!
			KDBE_CREATE_RIDING_PET_REQ kPacketToDB;
			const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
			SiCXSLRidingPetManager()->GetRidingPetID( iItemID, kPacketToDB.m_usCreatePetID, kPacketToDB.m_sPeriod );

			if( kPacketToDB.m_usCreatePetID == CXSLPetManager::RPI_NONE )
			{
				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RIDING_PET_01;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
				return;
			}

			// 아이템 삭제!
			KInventoryItemInfo kInvenotyItem;
			if( m_kInventory.DeleteItem( kPacket_.m_iItemUID, kInvenotyItem, KDeletedItemInfo::DR_RIDING_PET_CREATE_ITEM ) == false )
			{
				START_LOG( cerr, L"아이템이 존재 하는지 검사를 했는데 삭제 실패 했네? 있을 수 없는 에러!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< END_LOG;

				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_RIDING_PET_02;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
				return;
			}

			// 아이템 통계
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( iItemID );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
				//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
				KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
				//}}
			}

			// DB로 라이딩 펫 생성하러 갑시다!
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInvenotyItem );
			m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
			m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
			SendToGameDB( DBE_CREATE_RIDING_PET_REQ, kPacketToDB );
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_CREATE_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key값을 얻을 수 없습니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				KEGS_CREATE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_CREATE_COUNT_LIMITED:
		{
			KEGS_CREATE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RIDING_PET_03;
			SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
		}
		break;

	default:
		{
			START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_CREATE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
		}
		break;
	};
}

IMPL_ON_FUNC( DBE_GET_RIDING_PET_LIST_ACK )
{
	bool bSuccess = false;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( m_kUserRidingPetManager.Init( kPacket_.m_vecRidingPetList ) != KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			START_LOG( cerr, L"Riding Pet System: 정보 초기화 실패" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
		}
		else
		{
			CheckRidingPetProcess();
			bSuccess = true;
		}
	}

	if( bSuccess == true )
	{
		if( kPacket_.m_iAfterWorkStorageKey != 0 )
		{
			KEventPtr spEvent = KEventPtr();
			if( ( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, spEvent ) != KAfterWorkStorage::AWS_SUCCEED )
				|| ( spEvent == NULL )
				)
			{
				START_LOG( cerr, L"After Work Storage: 꺼내오기 실패" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
				return;
			}

			QueueingEvent( spEvent );
		}
	}
	else
	{
		if( kPacket_.m_iAfterWorkStorageKey == 0 )
		{
			KEGS_GET_RIDING_PET_LIST_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RIDING_PET_04;
			SendPacket( EGS_GET_RIDING_PET_LIST_ACK, kPacket );
		}
		else
		{
			KEventPtr spEvent = KEventPtr();
			if( ( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, spEvent ) != KAfterWorkStorage::AWS_SUCCEED )
				|| ( spEvent == NULL )
				)
			{
				START_LOG( cerr, L"After Work Storage: 꺼내오기 실패" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
				return;
			}
		}
	}
}

IMPL_ON_FUNC( DBE_CREATE_RIDING_PET_ACK )
{
	KEGS_CREATE_RIDING_PET_ACK kPacket;
	kPacket.m_iOK					= NetError::NET_OK;
	kPacket.m_kCreatedPetInfo		= kPacket_.m_kCreatedPetInfo;
	kPacket.m_vecKInventorySlotInfo	= kPacket_.m_vecUpdatedInventorySlot;

	// 아이템 롤백 체크
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	// 펫 생성 결과 처리
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		const int iOK = m_kUserRidingPetManager.AddRidingPet( kPacket_.m_kCreatedPetInfo, true );
		if( iOK != KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			START_LOG( cerr, L"Riding Pet System: 초기화 되지 않은 KUserRidingPetManager에 생성된 라이딩 펫 정보를 저장하려 하였습니다." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}

		KUserRidingPetPtr spInfo = KUserRidingPetPtr();
		if( m_kUserRidingPetManager.GetRidingPet( kPacket.m_kCreatedPetInfo.m_iRidingPetUID, spInfo ) == KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			spInfo->GetData( kPacket.m_kCreatedPetInfo );
		}
	}

	SendPacket( EGS_CREATE_RIDING_PET_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_RELEASE_RIDING_PET_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	const int iOK = m_kUserRidingPetManager.ReleaseRidingPetCheck( kPacket_.m_iRidingPetUID );

	switch( iOK )
	{
	case KUserRidingPetManager::RIDING_PET_SUCCEED:
		{
			KDBE_RELEASE_RIDING_PET_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID		= GetCharUID();
			kPacketToDB.m_iRidingPetUID	= kPacket_.m_iRidingPetUID;
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
			// 이벤트용 펫이고, 퀘스트 보유시
			if(kPacket_.m_bCEventQuest == true) 
			{
				kPacketToDB.m_bEventQuest = true;
			}
			else
			{
				kPacketToDB.m_bEventQuest = false;
			}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

			SendToGameDB( DBE_RELEASE_RIDING_PET_REQ, kPacketToDB );
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_INITED:
		{
			KDBE_GET_RIDING_PET_LIST_REQ KPacketToDB;
			if( m_kAfterWorkStorage.InsertData( PI_GS_USER, GetUID(), NULL, EGS_RELEASE_RIDING_PET_REQ, kPacket_, KPacketToDB.m_iAfterWorkStorageKey ) != KAfterWorkStorage::AWS_SUCCEED )
			{
				START_LOG( cerr, L"Riding Pet System: Key값을 얻을 수 없습니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				KEGS_RELEASE_RIDING_PET_ACK kPacket;
				kPacket.m_iOK			= NetError::ERR_UNKNOWN;
				SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
			}
			else
			{
				KPacketToDB.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_GET_RIDING_PET_LIST_REQ, KPacketToDB );
			}
		}
		break;

	case KUserRidingPetManager::RIDING_PET_ALREADY_SUMMONED:
		{
			KEGS_RELEASE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK			= NetError::ERR_RIDING_PET_10;	// 소환되어 있는 라이딩 펫은 놓아줄 수 없습니다.
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
		}
		break;

	case KUserRidingPetManager::RIDING_PET_NOT_EXIST:
		{
			KEGS_RELEASE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK			= NetError::ERR_RIDING_PET_06;	// 존재하지 않는 라이딩 펫입니다.
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
		}
		break;

	default:
		{
			START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_RELEASE_RIDING_PET_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket );
		}
		break;
	};
}

_IMPL_ON_FUNC( DBE_RELEASE_RIDING_PET_ACK, KEGS_RELEASE_RIDING_PET_ACK )
{
	USHORT		usRidingPetID	= 0;
	const int	iResult			= m_kUserRidingPetManager.IsExpirationRidingPet( kPacket_.m_iRidingPetUID, usRidingPetID );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		const int iOK = m_kUserRidingPetManager.ReleaseRidingPet( kPacket_.m_iRidingPetUID );
		switch( iOK )
		{
		case KUserRidingPetManager::RIDING_PET_SUCCEED:
			break;

		case KUserRidingPetManager::RIDING_PET_ALREADY_SUMMONED:
			{
				// 강제로 소환 해제
				m_kUserRidingPetManager.UnSummonRidingPet( kPacket_.m_iRidingPetUID, 0 );

				// 라이딩 펫 소환 결과를 클라에도 알려주고 필드 또는 방안에 있는 주변 유저에게도 알리자!
				if( ( GetStateID() == KGSFSM::S_FIELD_MAP ) &&
					( 0 < GetFieldUID() )
					)
				{
					KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
					kPacketNot.m_iUnitUID		= GetCharUID();
					SendToGSField( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
				}

				if( ( GetStateID() == KGSFSM::S_ROOM ) &&
					( 0 < GetRoomUID() )
					)
				{
					switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
					{
					case CXSLRoom::RT_PVP:
					case CXSLRoom::RT_DUNGEON:
					case CXSLRoom::RT_TRAININGCENTER:
						//{{ 2012. 02. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
					case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
						//}}
						{
							KEGS_UNSUMMON_RIDING_PET_NOT kPacketNot;
							kPacketNot.m_iUnitUID		= GetCharUID();
							SendToCnRoom( ERM_UNSUMMON_RIDING_PET_NOT, kPacketNot );
						}
						break;
					}
				}

				// 다시 제거 시도
				m_kUserRidingPetManager.ReleaseRidingPet( kPacket_.m_iRidingPetUID );
			}
			break;

		default:
			START_LOG( cerr, L"Riding Pet System: Unhandled Result" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( usRidingPetID )
				<< BUILD_LOG( kPacket_.m_iRidingPetUID )
				<< END_LOG;
			break;
		}

		if( iResult == KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			// 기한이 만료된 펫의 Release 처리 완료
			KEGS_RELEASE_RIDING_PET_NOT kPacket;
			kPacket.m_iRidingPetUID		= kPacket_.m_iRidingPetUID;
			kPacket.m_usRindingPetID	= usRidingPetID;
			SendPacket( EGS_RELEASE_RIDING_PET_NOT, kPacket );
		}
		else
		{
			// 놓아주기로 인한 Release 처리 완료
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		}
	}
	else
	{
		if( iResult != KUserRidingPetManager::RIDING_PET_SUCCEED )
		{
			// 놓아주기로 인한 Release 처리 실패
			SendPacket( EGS_RELEASE_RIDING_PET_ACK, kPacket_ );
		}
	}
}

_IMPL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT )
{
	SendPacket( EGS_SUMMON_RIDING_PET_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT )
{
	SendPacket( EGS_UNSUMMON_RIDING_PET_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_UPDATE_RIDING_PET_NOT )
{
	const int iOK = m_kUserRidingPetManager.UpdateSummonedRidingPetInfo( kPacket_.m_iRidingPetUID, kPacket_.m_fStamina );

	if( iOK != KUserRidingPetManager::RIDING_PET_SUCCEED )
	{
		START_LOG( cwarn, L"Riding Pet System: 라이딩 펫 스테미너 값 갱신 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iRidingPetUID )
			<< BUILD_LOG( kPacket_.m_fStamina )
			<< END_LOG;
	}
}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_EVENT_VC
IMPL_ON_FUNC( EGS_USE_INTIMACY_UP_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_INTIMACY_UP_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_USE_INTIMACY_UP_ITEM_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// 펫 관련 동작 제한
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  &&  GetRoomUID() > 0 )
			{
				START_LOG( cerr, L"거래 광장에서는 친밀 상승아이템을 줄 수 없습니다!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
				return;
			}
		}
		else if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			if( GetRoomListID() != 0 )
			{
				START_LOG( cerr, L"대전 로비에서는 친밀 상승아이템을 줄 수 없습니다!" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_PET_05;
				SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
				return;
			}
		}

		// 먹이를 주려고 하는 아이템이 실제로 존재하는 아이템인지 체크!
		if( m_kInventory.IsExist( kPacket_.m_iItemUID ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 아이템을 친밀 상승아이템으로 주려고 했다!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_04;
			SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_USE_INTIMACY_UP_ITEM_ACK kPacket;
	KEGS_CHANGED_PET_INTIMACY_NOT kNotIntimacy;

	// 펫 먹이 ItemID얻기
	const int iFeedItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	if( m_kUserPetManager.Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( kNotIntimacy, kPacket.m_iUpPercent ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kPacket );

		return;
	}

	// 해당 아이템 삭제!
	if( m_kInventory.FeedPetInInventory( kPacket_.m_iItemUID, kPacket.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, L"위에서 아이템이 있는지 체크 했는데 먹이 아이템 삭제 실패했다? 절대 일어나면 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_11;
		SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_USE_INTIMACY_UP_ITEM_ACK, kPacket );

	// 나를 포함한 주변 유저들에게 액션 정보를 날리자!
	SendPetAction( CXSLPetManager::PAC_EAT );

	//////////////////////////////////////////////////////////////////////////
	// 방이라면 포만도와 친밀도를 알리자!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		kNotIntimacy.m_iUnitUID = GetCharUID();
		SendPacket( EGS_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );

	}
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() <= 0 )
		{
			START_LOG( cerr, L"룸 UID 이상." )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			return;
		}

		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			//{{ 2012. 10. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			{
				kNotIntimacy.m_iUnitUID = GetCharUID();
				SendToCnRoom( ERM_CHANGED_PET_INTIMACY_NOT, kNotIntimacy );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			// 광장이면 안보낸다!
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방 타입." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}
#endif //SERV_EVENT_VC
//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////
