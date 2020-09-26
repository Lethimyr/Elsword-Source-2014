#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "Enum/Enum.h"
#include "GameSysVal/GameSysVal.h"
#include "GSWorldMissionManager.h"
//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "BattleFieldListManager.h"	
	#include "NewPartyListManager.h"
	#include "X2Data/XSLBattleFieldManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_COEXISTENCE_FESTIVAL// 작업날짜: 2013-05-14	// 박세훈
	#include "X2Data/XSLBuffManager.h"	
#endif // SERV_COEXISTENCE_FESTIVAL

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	#include "GSFieldBossManager.h"
	#include "X2Data/XSLBuffManager.h"
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS


#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_JOIN_BATTLE_FIELD_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_BATTLE_FIELD_REQ, EGS_JOIN_BATTLE_FIELD_ACK );


#ifdef SERV_ALTERA_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_ALTERA_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
	}
	ELSE
	{
		if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_01 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_02 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_ALTERA_FIELD_03 )
		{
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
		}
	}
#endif //SERV_ALTERA_AUTO_OPEN_HARD_CODE
#ifdef SERV_PEITA_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_PEITA_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
	}
	ELSE
	{
		if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_01 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_02 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_PEITA_FIELD_03 )
		{
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
		}
	}
#endif //SERV_PEITA_AUTO_OPEN_HARD_CODE
#ifdef SERV_VELDER_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_VELDER_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
	}
	ELSE
	{
		if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_01 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_02 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_03 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_04 )
		{
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
		}
	}
#endif //SERV_VELDER_AUTO_OPEN_HARD_CODE
#ifdef SERV_HAMEL_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_HAMEL_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
	}
	ELSE
	{
		if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_SHIP_STAGE || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_SHIP_STAGE )
		{
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
		}
	}
#endif //SERV_HAMEL_AUTO_OPEN_HARD_CODE
#ifdef SERV_SANDER_AUTO_OPEN_HARD_CODE
	IF_EVENT_ENABLED( CEI_SANDER_AUTO_OPEN_HARD_CODE )
	{
		// 2013.03.14 lygan_조성욱 // 코드 비어 있는게 맞습니다.
	}
	ELSE
	{
		if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02 ||
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_03 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_04 )
		{
			kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
		}
	}
#endif //SERV_SANDER_AUTO_OPEN_HARD_CODE

#ifdef NO_SANDER_VILLIAGE
	if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_01 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_SANDER_FIELD_02 )
	{
		kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
	}
#endif // NO_SANDER_VILLIAGE

#ifdef SERV_NO_VELDER_VILLIAGE
	if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_01 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_02 
		|| kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_03 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_VELDER_FIELD_04
		)
	{
		kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
	}
#endif //SERV_NO_VELDER_VILLIAGE

#ifdef SERV_NO_HAMEL_VILLIAGE
	if( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_01 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_02
		|| kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_03 || kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID == SEnum::VMI_BATTLE_FIELD_HAMEL_FIELD_04)
	{
		kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID = SEnum::VMI_BATTLE_FIELD_RUBEN_FIELD_01;
	}
#endif //SERV_NO_HAMEL_VILLIAGE

#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	int iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
	int	iStartPosIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
	bool bMoveForMyParty = kPacket_.m_kBattleFieldJoinInfo.m_bMoveForMyParty;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	int iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
	int	iStartPosIndex = kPacket_.m_StartPosIndex;
	bool bMoveForMyParty = kPacket_.m_bMoveForMyParty;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	// 배틀필드ID검증
	if( SiCXSLBattleFieldManager()->IsExistBattleFieldID( static_cast<SEnum::BATTLE_FIELD_ID>(iBattleFieldID) ) == false )
	{
		START_LOG( cerr, L"유효하지 않은 배틀필드ID입니다!" )
			<< BUILD_LOG( iBattleFieldID )
			<< BUILD_LOG( iStartPosIndex )
			<< BUILD_LOG( bMoveForMyParty )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kAck;
		kAck.m_iOK = NetError::ERR_BATTLEFIELD_05;
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
		return;
	}

	// 입장 조건 체크
	int iRequireLevel = 0;
	int iRequireDungeonID = 0;
	if( SiCXSLBattleFieldManager()->IsCanEnterBattleField( static_cast<SEnum::BATTLE_FIELD_ID>( iBattleFieldID ), GetLevel(), m_mapDungeonClear, iRequireLevel, iRequireDungeonID ) == false )
	{
		KEGS_JOIN_BATTLE_FIELD_ACK kAck;
		kAck.m_iRequireLevel = iRequireLevel;
		kAck.m_iRequireDungeonID = iRequireDungeonID;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
		return;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	const bool bBossField = SiCXSLBattleFieldManager()->IsBossFieldID( iBattleFieldID );
	if( bBossField == true )
	{
		if( ( GetAuthLevel() < SEnum::UAL_DEVELOPER )
			&& ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_DEBUFF_REST_OF_RECHALLENGE ) == true )
			)
		{
			KEGS_JOIN_BATTLE_FIELD_ACK kAck;
			kAck.m_iOK = NetError::ERR_BATTLEFIELD_20;	// 재도전을 위한 휴식 버프로 인해 시간의 포탈에 입장 할 수 없습니다. 다음 포탈 생성시까지 기다려 주세요.
			SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
			return;
		}

		// 현재 배틀 필드가 포탈이 열려있는 배틀 필드가 아니라면 입장을 제한한다.
		if( GetMapID() != SiKGSFieldBossManager()->GetPortalAppearanceMap() )
		{
			KEGS_JOIN_BATTLE_FIELD_ACK kAck;
			kAck.m_iOK = NetError::ERR_BATTLEFIELD_18;	// 현재 필드에서는 입장할 수 없습니다.
			SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
			return;
		}

		// 입장하고자 하는 배틀 필드가 현재 열려있는 포탈과 연결된 배틀 필드가 아니라면 입장을 제한한다.
		if( iBattleFieldID != SiKGSFieldBossManager()->GetPortalDestination() )
		{
			KEGS_JOIN_BATTLE_FIELD_ACK kAck;
			kAck.m_iOK = NetError::ERR_BATTLEFIELD_19;	// 해당 필드는 열려있지 않습니다.
			SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
			return;
		}
	}
#endif // SERV_BATTLE_FIELD_BOSS

	// 배틀필드에 입장가능한 상태인지 검사한다.
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				// 현재 입장해있는 배틀필드와 같은 곳으로 이동하려고 하는것이라면 최적의 장소인지 보자!
				if( GetMapID() == iBattleFieldID )
				{
					UidType iJoinBFRoomUID = 0;
					if( SiKBattleFieldListManager()->GetRoomUIDForJoinBattleField( iBattleFieldID, GetCharUID(), GetRoomUID(), GetPartyUID(), iJoinBFRoomUID ) == false )
					{
						START_LOG( clog, L"필드에 들어갈 수 없는 상태입니다." )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( iBattleFieldID )
							<< END_LOG;

						KEGS_JOIN_BATTLE_FIELD_ACK kAck;
						kAck.m_iOK = NetError::GetLastError();
						SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
						return;
					}

					// 이미 내가 입장한 곳과 최적의 선택이 같은 곳이라면 굳이 이동할 필요가 없다.
					if( GetRoomUID() == iJoinBFRoomUID )
					{
						KEGS_JOIN_BATTLE_FIELD_ACK kAck;
						kAck.m_iOK = NetError::ERR_BATTLEFIELD_15;
						SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
						return;
					}
				}

				// 필드에서 필드로 이동하는경우 임시 인벤토리를 비운다!
				ClearTempInventory();
			}
			break;

		case CXSLRoom::RT_SQUARE:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				START_LOG( cerr, L"해당 방에서 배틀필드로는 이동할 수 없습니다!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( iBattleFieldID )
					<< END_LOG;

				KEGS_JOIN_BATTLE_FIELD_ACK kAck;
				kAck.m_iOK = NetError::ERR_BATTLEFIELD_00;
				SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
			}
			return;
		}
	}
	else
	{
		if( GetRoomUID() != 0 )
		{
			START_LOG( cerr, L"필드 상태인데 RoomUID값이 존재한다! 버그다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iBattleFieldID )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;

			KEGS_JOIN_BATTLE_FIELD_ACK kAck;
			kAck.m_iOK = NetError::ERR_BATTLEFIELD_00;
			SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
			return;
		}		
	}

	//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
	// 배틀 필드 입장 시 캐시샵 입장 정보를 초기화 한다.
	m_kUserUnitManager.SetEnterCashShop( false );
#endif SERV_VISIT_CASH_SHOP
	//}}

	// 배틀필드 입장 정보 만들기
	KBattleFieldJoinInfo kBattleFieldJoinInfo;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kBattleFieldJoinInfo.m_iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
	kBattleFieldJoinInfo.m_iStartPosIndex = kPacket_.m_StartPosIndex;
	kBattleFieldJoinInfo.m_bMoveForMyParty = kPacket_.m_bMoveForMyParty;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	
	// 현재 파티에 소속이면 파티에서 파티원 정보를 얻어온다.
	if( GetPartyUID() != 0 )
	{	
		KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ kPacket;
		kPacket.m_kBattleFieldJoinInfo = kBattleFieldJoinInfo;
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ, kPacket );
		return;
	}

	// 현재 방에 입장해있는 상태라면 일단 이탈 처리부터 한다.
	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_BATTLE_FIELD:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				if( bMoveForMyParty )
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_31;
				}
				else
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
				}
				kPacket.m_kBattleFieldJoinInfo = kBattleFieldJoinInfo;
				SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_REQ, kPacket );
				return;
			}
			break;

		case CXSLRoom::RT_DUNGEON:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
				kPacket.m_kBattleFieldJoinInfo = kBattleFieldJoinInfo;
				SendToCnRoom( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, kPacket );
				return;
			}
			break;

		case CXSLRoom::RT_PVP:
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
				kPacket.m_kBattleFieldJoinInfo = kBattleFieldJoinInfo;
				SendToCnRoom( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, kPacket );
				return;
			}
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방에 입장한 상태에서 배틀필드로 들어가려고 하네?" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;

				KEGS_JOIN_BATTLE_FIELD_ACK kAck;
				kAck.m_iOK = NetError::ERR_BATTLEFIELD_00;
				SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
				return;
			}
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// 배틀필드 리스트를 검색해서 들어갈만한 곳을 찾아보자!
	UidType iJoinBFRoomUID = 0;
	if( SiKBattleFieldListManager()->GetRoomUIDForJoinBattleField( iBattleFieldID, GetCharUID(), GetBeforeRoomUID(), GetPartyUID(), iJoinBFRoomUID ) == false )
	{
		START_LOG( clog, L"필드에 들어갈 수 없는 상태입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iBattleFieldID )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
		return;
	}

	if( iJoinBFRoomUID == 0 )
	{
		//{{ 2013. 01. 08	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		if( GetRoomUID() != 0 )
		{
			START_LOG( cerr, L"새로운 배틀필드를 생성하려는데 지금 다른방에 입장해있다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		//}}

		KERM_OPEN_BATTLE_FIELD_REQ kPacket;
		kPacket.m_kRoomInfo.m_RoomType = CXSLRoom::RT_BATTLE_FIELD;
		kPacket.m_kRoomInfo.m_iBattleFieldID = iBattleFieldID;
		kPacket.m_kBattleFieldJoinInfo = kBattleFieldJoinInfo;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		if( bBossField == true )
		{
			GetBossFieldCreateInfo( iBattleFieldID, kPacket.m_KBossFieldCreateInfo );
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		SendToCnRoom( ERM_OPEN_BATTLE_FIELD_REQ, kPacket );
	}
	else
	{
		KERM_JOIN_BATTLE_FIELD_REQ kPacket;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		kPacket.m_kBattleFieldJoinInfo = kBattleFieldJoinInfo;
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		if( bBossField == true )
		{
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		_SendToCnRoom( iJoinBFRoomUID, ERM_JOIN_BATTLE_FIELD_REQ, kPacket );
	}
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_BATTLE_FIELD_ACK );

	// 상태 오류 검사
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"배틀필드 입장을 위한 파티 정보 얻기 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kPacketAck );
		return;
	}

	// 이미 방 상태라면 다른 방에 입장해있는지 보고 이탈처리하자!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		if( GetRoomUID() > 0 )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				if( kPacket_.m_kBattleFieldJoinInfo.m_bMoveForMyParty )
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_31;
				}
				else
				{
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
				}
				kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
				SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_REQ, kPacket );
				return;
			}
			//{{PVP 매칭 시 배틀 필드로 복귀 되지 않는 오류 수정 - 김민성
#ifdef SERV_PARTY_PVP_MATCH_AFTER_RETURN_BATTLE_FIELD
			else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON || CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_PVP )
#else
			else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
#endif SERV_PARTY_PVP_MATCH_AFTER_RETURN_BATTLE_FIELD
				//}
			{
				KERM_LEAVE_ROOM_REQ kPacket;
				kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
				kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
				SendToCnRoom( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, kPacket );
				return;
			}
			else
			{
				START_LOG( cerr, L"이상한 방에 입장한 상태에서 배틀필드로 들어가려고 하네?" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< END_LOG;

				KEGS_JOIN_BATTLE_FIELD_ACK kAck;
				kAck.m_iOK = NetError::ERR_BATTLEFIELD_04;
				SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
				return;
			}
		}
	}

	// 1. 배틀필드 리스트를 검색해서 같은 파티원이 있는곳을 찾아 낸다.
	UidType iJoinBFRoomUID = 0;
	if( SiKBattleFieldListManager()->GetRoomUIDForJoinBattleField( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, GetCharUID(), GetBeforeRoomUID(), GetPartyUID(), iJoinBFRoomUID ) == false )
	{
		START_LOG( clog, L"필드에 들어갈 수 없는 상태입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
		return;
	}

	if( iJoinBFRoomUID == 0 )
	{
		//{{ 2013. 01. 08	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		if( GetRoomUID() != 0 )
		{
			START_LOG( cerr, L"새로운 배틀필드를 생성하려는데 지금 다른방에 입장해있다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		//}}

		KERM_OPEN_BATTLE_FIELD_REQ kPacket;
		kPacket.m_kRoomInfo.m_RoomType = CXSLRoom::RT_BATTLE_FIELD;
		kPacket.m_kRoomInfo.m_iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
		kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
		{
			GetBossFieldCreateInfo( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, kPacket.m_KBossFieldCreateInfo );
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		SendToCnRoom( ERM_OPEN_BATTLE_FIELD_REQ, kPacket );
	}
	else
	{
		KERM_JOIN_BATTLE_FIELD_REQ kPacket;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
		{
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		_SendToCnRoom( iJoinBFRoomUID, ERM_JOIN_BATTLE_FIELD_REQ, kPacket );
	}
}

_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_ACK, KEGS_LEAVE_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_JOIN_BATTLE_FIELD_ACK );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"배틀필드 입장을 위한 방이탈 처리가 실패하였습니다!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kPacketAck );
		return;
	}

	// 성공적으로 방 이탈 처리!
	SetRoomUID( 0 );
	m_kUserDungeonManager.SetDungeonGameInfo( 0, 0, 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}	

	// 배틀필드 리스트를 검색해서 같은 파티원이 있는곳을 찾아 낸다.
	UidType iJoinBFRoomUID = 0;
	if( SiKBattleFieldListManager()->GetRoomUIDForJoinBattleField( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, GetCharUID(), GetBeforeRoomUID(), GetPartyUID(), iJoinBFRoomUID ) == false )
	{
		START_LOG( clog, L"필드에 들어갈 수 없는 상태입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
		return;
	}

	if( iJoinBFRoomUID == 0 )
	{
		//{{ 2013. 01. 08	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		if( GetRoomUID() != 0 )
		{
			START_LOG( cerr, L"새로운 배틀필드를 생성하려는데 지금 다른방에 입장해있다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		//}}

		KERM_OPEN_BATTLE_FIELD_REQ kPacket;
		kPacket.m_kRoomInfo.m_RoomType = CXSLRoom::RT_BATTLE_FIELD;
		kPacket.m_kRoomInfo.m_iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
		kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
		{
			GetBossFieldCreateInfo( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, kPacket.m_KBossFieldCreateInfo );
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		SendToCnRoom( ERM_OPEN_BATTLE_FIELD_REQ, kPacket );
	}
	else
	{
		KERM_JOIN_BATTLE_FIELD_REQ kPacket;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
		{
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		_SendToCnRoom( iJoinBFRoomUID, ERM_JOIN_BATTLE_FIELD_REQ, kPacket );
	}
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_ACK, KEGS_LEAVE_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_JOIN_BATTLE_FIELD_ACK );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"배틀필드 입장을 위한 방이탈 처리가 실패하였습니다!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kPacketAck );
		return;
	}

	// 성공적으로 방 이탈 처리!
	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}	

	// 배틀필드 리스트를 검색해서 같은 파티원이 있는곳을 찾아 낸다.
	UidType iJoinBFRoomUID = 0;
	if( SiKBattleFieldListManager()->GetRoomUIDForJoinBattleField( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, GetCharUID(), GetBeforeRoomUID(), GetPartyUID(), iJoinBFRoomUID ) == false )
	{
		START_LOG( clog, L"필드에 들어갈 수 없는 상태입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		KEGS_JOIN_BATTLE_FIELD_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kAck );
		return;
	}

	if( iJoinBFRoomUID == 0 )
	{
		//{{ 2013. 01. 08	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		if( GetRoomUID() != 0 )
		{
			START_LOG( cerr, L"새로운 배틀필드를 생성하려는데 지금 다른방에 입장해있다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
		//}}

		KERM_OPEN_BATTLE_FIELD_REQ kPacket;
		kPacket.m_kRoomInfo.m_RoomType = CXSLRoom::RT_BATTLE_FIELD;
		kPacket.m_kRoomInfo.m_iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
		kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
		{
			GetBossFieldCreateInfo( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, kPacket.m_KBossFieldCreateInfo );
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		SendToCnRoom( ERM_OPEN_BATTLE_FIELD_REQ, kPacket );
	}
	else
	{
		KERM_JOIN_BATTLE_FIELD_REQ kPacket;
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
		GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
		kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
		m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
		{
			CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
		}
#endif // SERV_BATTLE_FIELD_BOSS
		_SendToCnRoom( iJoinBFRoomUID, ERM_JOIN_BATTLE_FIELD_REQ, kPacket );
	}
}

IMPL_ON_FUNC( ERM_OPEN_BATTLE_FIELD_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_BATTLE_FIELD_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 배틀필드 MapID값 저장
		m_kLastPos.Clear();
		SetMapID( kPacket_.m_RoomInfo.m_iBattleFieldID );

		// 배틀필드RoomUID저장
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		if( kPacket_.m_bBossField == true )
		{
			SetBossFieldJoinInfo( kPacket_.m_kBossFieldJoinInfo );

			// 보스 필드 입장에 성공했으니 재입장 불가 디버프를 걸어준다.
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;

			DeactivateBuff( CXSLBuffManager::BTI_DEBUFF_REST_OF_RECHALLENGE, kReq.m_vecDeActivateBuff );
			ActivateBuff( CXSLBuffManager::BTI_DEBUFF_REST_OF_RECHALLENGE, kReq.m_vecActivateBuff );

			if( ( kReq.m_vecDeActivateBuff.empty() == false )
				|| ( kReq.m_vecActivateBuff.empty() == false )
				)
			{
				kReq.m_iUnitUID = GetCharUID();
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif // SERV_BATTLE_FIELD_BOSS

		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		// 필드에서 이탈
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}

#ifdef SERV_COEXISTENCE_FESTIVAL// 작업날짜: 2013-05-14	// 박세훈
		// 배틀 필드 입장 성공했으므로 버프를 주자.
		std::vector< KBuffInfo > vecActivateBuff;
		ActivateBuff( CXSLBuffManager::BTI_FESTIVAL_EVENT_BUFF, vecActivateBuff );

		if( vecActivateBuff.empty() == false )
		{
			// 로그인 서버 버프 매니저에 업데이트
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecActivateBuff = vecActivateBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
#endif // SERV_COEXISTENCE_FESTIVAL
	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword );
	}

    KEGS_JOIN_BATTLE_FIELD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_RoomInfo = kPacket_.m_RoomInfo;
	kPacket.m_vecSlot = kPacket_.m_vecSlot;
	kPacket.m_wstrCNIP = kPacket_.m_wstrCNIP;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_iLastTouchIndex = kPacket_.m_StartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_wstrUDPRelayIP = kPacket_.m_wstrUDPRelayIP;
	kPacket.m_usUDPRelayPort = kPacket_.m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	kPacket.m_tRemainFieldHoldingTime = kPacket_.m_tRemainFieldHoldingTime;
#endif // SERV_BATTLE_FIELD_BOSS

	SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kPacket );
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
IMPL_ON_FUNC( ERM_JOIN_BATTLE_FIELD_ACK )
#else // SERV_BATTLE_FIELD_BOSS
_IMPL_ON_FUNC( ERM_JOIN_BATTLE_FIELD_ACK, KEGS_JOIN_BATTLE_FIELD_ACK )
#endif // SERV_BATTLE_FIELD_BOSS
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_BATTLE_FIELD_ACK );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
	KEGS_JOIN_BATTLE_FIELD_ACK kPacket;
	kPacket.m_iOK				= kPacket_.m_iOK;
	kPacket.m_iRequireLevel		= kPacket_.m_iRequireLevel;
	kPacket.m_iRequireDungeonID	= kPacket_.m_iRequireDungeonID;
#endif // SERV_BATTLE_FIELD_BOSS

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword );

		//{{ 2012. 12. 31	최육사	던전에서 필드로 돌아오지 못하는 오류 수정
#ifdef SERV_RETURN_TO_FIELD_BUG_FIX
		// 입장하려는 배틀필드에 이미 사람이 가득 차있어서 입장하지 못했습니다.  따라서 새로운 필드를 생성하러 갑니다.
		switch( kPacket_.m_iOK )
		{
		case NetError::ERR_BATTLEFIELD_02:
		case NetError::ERR_BATTLEFIELD_03:
			{
                // 이미 방에 입장한 상태라면 방에서 먼저 나가자!
				if( GetRoomUID() != 0 )
				{
					START_LOG( cerr, L"배틀필드에 입장하려는데 아직 다른 방에 입장한 상태이다. 일어나서는 안되는 에러!" )
						<< BUILD_LOG( GetUID() )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( GetStateIDString() )
						<< BUILD_LOG( GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< END_LOG;

					switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
					{
					case CXSLRoom::RT_SQUARE:
						{
							KEGS_LEAVE_SQUARE_REQ kPacket;
							kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;							
							SendToCnRoom( ERM_LEAVE_SQUARE_REQ, kPacket );
						}
						break;

					case CXSLRoom::RT_TRAININGCENTER:
						{
							KERM_LEAVE_ROOM_REQ kPacket;
							kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
							kPacket.m_bSendAck = false;
							SendToCnRoom( ERM_LEAVE_TC_ROOM_REQ, kPacket );
						}
						break;

					default:
						{
							KERM_LEAVE_ROOM_REQ kPacket;							
							kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
							kPacket.m_bSendAck = false;
							SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );
						}
						break;
					}

					SetRoomUID( 0 );
				}

				KERM_OPEN_BATTLE_FIELD_REQ kPacket;
				kPacket.m_kRoomInfo.m_RoomType = CXSLRoom::RT_BATTLE_FIELD;
				kPacket.m_kRoomInfo.m_iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
				kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
				GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
				GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
				m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
				if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
				{
					GetBossFieldCreateInfo( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, kPacket.m_KBossFieldCreateInfo );
					CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
				}
#endif // SERV_BATTLE_FIELD_BOSS
				SendToCnRoom( ERM_OPEN_BATTLE_FIELD_REQ, kPacket );
				return;
			}
			break;
		}
#else
		// 입장하려는 배틀필드에 이미 사람이 가득 차있어서 입장하지 못했습니다.  따라서 새로운 필드를 생성하러 갑니다.
		if( GetRoomUID() == 0 )
		{
			switch( kPacket_.m_iOK )
			{
			case NetError::ERR_BATTLEFIELD_02:
			case NetError::ERR_BATTLEFIELD_03:
				{
					KERM_OPEN_BATTLE_FIELD_REQ kPacket;
					kPacket.m_kRoomInfo.m_RoomType = CXSLRoom::RT_BATTLE_FIELD;
					kPacket.m_kRoomInfo.m_iBattleFieldID = kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID;
					kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
					GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_BATTLE_FIELD );
					GetTutorUnitUIDList( kPacket.m_vecStudentUnitUID );
					m_kUserUnitManager.GetAndDeleteResevedReturnToFieldInfo( kPacket.m_kReturnToFieldInfo );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
					if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID ) == true )
					{
						GetBossFieldCreateInfo( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID, kPacket.m_KBossFieldCreateInfo );
						CreateBossFieldJoinInfo( kPacket.m_kBossFieldJoinInfo );
					}
#endif // SERV_BATTLE_FIELD_BOSS
					SendToCnRoom( ERM_OPEN_BATTLE_FIELD_REQ, kPacket );
					return;
				}
				break;
			}
		}
#endif SERV_RETURN_TO_FIELD_BUG_FIX
		//}}		
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 입장 성공!

		// 배틀필드 MapID값 저장!
		m_kLastPos.Clear();
		SetMapID( kPacket_.m_RoomInfo.m_iBattleFieldID );

		// 배틀필드 RoomUID값 저장!
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		if( kPacket_.m_bBossField == true )
		{
			SetBossFieldJoinInfo( kPacket_.m_kBossFieldJoinInfo );

			// 보스 필드 입장에 성공했으니 재입장 불가 디버프를 걸어준다.
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;

			DeactivateBuff( CXSLBuffManager::BTI_DEBUFF_REST_OF_RECHALLENGE, kReq.m_vecDeActivateBuff );
			ActivateBuff( CXSLBuffManager::BTI_DEBUFF_REST_OF_RECHALLENGE, kReq.m_vecActivateBuff );

			if( ( kReq.m_vecDeActivateBuff.empty() == false )
				|| ( kReq.m_vecActivateBuff.empty() == false )
				)
			{
				kReq.m_iUnitUID = GetCharUID();
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif // SERV_BATTLE_FIELD_BOSS

		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		// 필드에서 이탈
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}

		//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
		// 캐시샵 입장 정보 초기화
		m_kUserUnitManager.SetEnterCashShop( false );
#endif SERV_VISIT_CASH_SHOP
		//}}

		// 배틀필드에 입장 성공했는데 자신의 파티 객체가 존재한다면 자신의 RoomUID값을 배틀필드로 업데이트 하자!

#ifdef SERV_COEXISTENCE_FESTIVAL// 작업날짜: 2013-05-14	// 박세훈
		// 배틀 필드 입장 성공했으므로 버프를 주자.
		std::vector< KBuffInfo > vecActivateBuff;
		ActivateBuff( CXSLBuffManager::BTI_FESTIVAL_EVENT_BUFF, vecActivateBuff );

		if( vecActivateBuff.empty() == false )
		{
			// 로그인 서버 버프 매니저에 업데이트
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecActivateBuff = vecActivateBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
#endif // SERV_COEXISTENCE_FESTIVAL

#ifdef SERV_BATTLE_FIELD_BOSS
		kPacket.m_kBattleFieldJoinInfo		= kPacket_.m_kBattleFieldJoinInfo;
		kPacket.m_RoomInfo					= kPacket_.m_RoomInfo;
		kPacket.m_vecSlot					= kPacket_.m_vecSlot;
		kPacket.m_wstrCNIP					= kPacket_.m_wstrCNIP;
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		kPacket.m_iLastTouchIndex			= kPacket_.m_iLastTouchIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		kPacket.m_wstrUDPRelayIP			= kPacket_.m_wstrUDPRelayIP;
		kPacket.m_usUDPRelayPort			= kPacket_.m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
		//}
		//{{ 2012. 11. 21	최육사		난입자에게 캐쉬샵 유저 정보 보내기
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
		kPacket.m_vecEnterCashShopUser		= kPacket_.m_vecEnterCashShopUser;
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
		//}}
		kPacket.m_tRemainFieldHoldingTime	= kPacket_.m_tRemainFieldHoldingTime;	// 보스 필드가 닫힐 때까지 남은 시간
#endif // SERV_BATTLE_FIELD_BOSS
	}

	SendPacket( EGS_JOIN_BATTLE_FIELD_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_JOIN_BATTLE_FIELD_NOT, KEGS_JOIN_BATTLE_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0  ||  CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
	{
		START_LOG( cerr, L"배틀필드에 입장한 상태가 아닌데 이 패킷이 오네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;
		return;
	}

	SendPacket( EGS_JOIN_BATTLE_FIELD_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_NOT, KEGS_LEAVE_BATTLE_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	switch( kPacket_.m_iReason )
	{
		// 특수한 이탈인 경우 아래와 같이 예외처리를 하여 클라이언트로 전송되지 않도록 한다.
	case ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_ACK:
		{
			if( kPacket_.m_iUnitUID == GetCharUID() )
				return;

			if( kPacket_.m_iPartyUID == GetPartyUID()  &&  kPacket_.m_iPartyUID != 0 )
			{
				// 같은 파티 소속의 이탈이라면 클라이언트로 전송하지 말자!
				return;
			}
		}
		break;
	}

	SendPacket( EGS_LEAVE_BATTLE_FIELD_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_GAME_LOADING_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GAME_LOADING_ACK, kPacket_ );

	//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		m_kUserQuestManager.CheckEpicQuest_EnterBattleField( GetMapID(), GetThisPtr<KGSUser>() );
#endif SERV_REFORM_QUEST
		//}}
		m_kUserQuestManager.Handler_OnEnterTheBattleField( GetThisPtr<KGSUser>() );
	}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	//}}
}

IMPL_ON_FUNC( EGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		SendToCnRoom( ERM_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_LOAD_NOT, KEGS_BATTLE_FIELD_NPC_LOAD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 현재 입장한 배틀필드에 맞는 몬스터 출현 정보인지 체크!
	if( kPacket_.m_iBattleFieldID != GetMapID() )
	{
		START_LOG( cerr, L"배틀필드 NPC 정보 동기화 id가 달라서 무효처리! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( kPacket_.m_vecNPCData.size() )
			<< END_LOG;
		return;
	}

	SendPacket( EGS_BATTLE_FIELD_NPC_LOAD_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ, EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"배틀필드 NPC 로딩 완료 패킷을 보냈는데 지금 현재 방에 입장한 상태가 아니다? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() ) 
			<< BUILD_LOG( GetCharUID() ) 
			<< BUILD_LOG( GetCharName() ) 
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		KEGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_16;
		SendPacket( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );
		return;
	}

	//{{ 2012. 12. 07	최육사	배틀필드 시스템
#ifdef SERV_CODE_CLEANUP_2012_12_07
	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
	{
		START_LOG( cerr, L"배틀필드 NPC 로딩 완료 패킷을 보냈는데 지금 현재 배틀필드방이 아니라 다른방에 입장한 상태이다!" )
			<< BUILD_LOG( GetUID() ) 
			<< BUILD_LOG( GetCharUID() ) 
			<< BUILD_LOG( GetCharName() ) 
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		KEGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_17;
		SendPacket( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );
		return;
	}
#endif SERV_CODE_CLEANUP_2012_12_07
	//}}

	SendToCnRoom( ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ, char() );
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, KEGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK );

	SendPacket( EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_P2P_SYNC_NOT, KEGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT, kPacket_ );
}

//#ifndef SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//IMPL_ON_FUNC_NOPARAM( EGS_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT )
//{
//	// 시간차로 발생할수 있는 상태 오류. cwarn으로 로그 레벨을 낮춘다.
//	VERIFY_STATE_WARN( ( 1, KGSFSM::S_ROOM ) );
//
//	if( GetRoomUID() == 0 )
//	{
//		START_LOG( cwarn, L"RoomUID값이 유효하지 않은데 배틀필드 몬스터 동기화 패킷이 왔네?" )
//			<< BUILD_LOG( GetCharUID() )
//			<< BUILD_LOG( GetCharName() )
//			<< END_LOG;
//		return;
//	}
//
//	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
//	{
//		// 시간차로 발생할수 있는 문제다. cwarn으로 에러 로그 레벨을 낮춤.
//		START_LOG( cwarn, L"현재 배틀필드에 입장한 상태가 아닌데 배틀필드 몬스터 동기화 패킷이 왔네?" )
//			<< BUILD_LOG( GetCharUID() )
//			<< BUILD_LOG( GetCharName() )
//			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
//			<< END_LOG;
//		return;
//	}
//
//	SendToCnRoom( ERM_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT, char() );
//}
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_EVENT_MESSAGE_NOT, KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BATTLE_FIELD_EVENT_MESSAGE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT )
{
	VERIFY_STATE_WARN( ( 1, KGSFSM::S_ROOM ) );

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    // 시간차로 인해 배틀필드 방이 아닌데 배틀필드 방으로 패킷이 날아왔으면 로그도 안 찍고 그냥 무시한다.
    if ( GetRoomUID() == 0 || CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
    {
        return;
    }
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	
	if( GetMapID() != kPacket_.m_kLastPos.m_iMapID )
	{
		return;
	}

	// 최종 좌표 업데이트
	m_kLastPos = kPacket_.m_kLastPos;

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    SendToCnRoom( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
}

_IMPL_ON_FUNC( EPM_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT, KEGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT )
{
	// 방 상태가 아니면 굳이 에러로그 찍지 않습니다.
	VERIFY_STATE_LOG( clog, ( 1, KGSFSM::S_ROOM ) );

    SendPacket( EGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_BATTLE_FIELD_ZU_ECHO_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

    if( kPacket_.m_iUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"다른 캐릭터에게 배틀필드 좀비 유저 에코 패킷이 왔습니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
        return;
	}

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"상태는 방인데 RoomUID값은 0이다?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
	{
		START_LOG( cerr, L"현재 배틀필드 방이 아닌데 배틀필드 좀비 패킷을 처리하려한다?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;
		return;
	}

	//{{ 2012. 11. 20	최육사		배틀필드 좀비 유저 체크 강화
#ifdef SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
	if( GetRoomUID() != kPacket_.m_iRoomUID )
	{
		START_LOG( cerr, L"GSUser에 등록된 RoomUID와 배틀필드 방 RoomUID값이 틀리다! 이건 심각한데?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< BUILD_LOG( kPacket_.m_iRoomUID )
			<< BUILD_LOG( CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) )
			<< END_LOG;
		return;
	}
#endif SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
	//}}

	SendToCnRoom( ERM_BATTLE_FIELD_ZU_ECHO_ACK );

}

//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
IMPL_ON_FUNC( ERM_UPDATE_VISIT_CASH_SHOP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iUnitUID == GetCharUID() )
	{
		return;
	}

	KEGS_VISIT_CASH_SHOP_NOT kNot;
	kNot.m_iUnitUID = kPacket_.m_iUnitUID;
	kNot.m_bEnterCashShop = kPacket_.m_bEnterCashShop;

	SendPacket( EGS_VISIT_CASH_SHOP_NOT, kPacket_ );
}
#endif SERV_VISIT_CASH_SHOP
//}}

//{{ 배틀필드 커뮤니티 리스트 확인 - 김민성
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
_IMPL_ON_FUNC( ERM_COMMUNITY_IN_BATTLE_FIELD_ACK, KEGS_COMMUNITY_USER_LIST_ACK )
{
	SendPacket( EGS_COMMUNITY_USER_LIST_ACK, kPacket_ );
}
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
//}}

//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT )
{
	SendPacket( EGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK );

	KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;
	SendPacket( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK, kAck );


	KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ kReq;
	kReq.m_iUnitUID = GetCharUID();
	kReq.m_iNPCUID = kPacket_.m_iNPCUID;
	SendToCnRoom( ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ, kReq );
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK, KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ )
{
	SendPacket( EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ, kPacket_ );
}

IMPL_ON_FUNC( EGS_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK )
{
	KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT kNot;
	kNot = kPacket_;
	SendToCnRoom( ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT )
{
	SendPacket( EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, kPacket_ );
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-09	// 박세훈
_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_ACK, KEGS_LEAVE_ROOM_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( 0 );

		StateTransition( KGSFSM::I_TO_FIELD_MAP );

		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
		}
	}
	else
	{
		START_LOG( cerr, L"배틀필드 방이탈 처리가 실패하였습니다!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;
	}

	SetLanBugOutDisconnectCheck( true );

	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT kPacket;
	kPacket.m_iOK = NetError::ERR_UDP_01;
	SendPacket( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT, kPacket );

	START_LOG( clog, L"필드에서의 랜선 유저 마을로 튕겨내기. 블럭 체크 시작")
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( GetCharName() )
		<< END_LOG;
}

IMPL_ON_FUNC( DBE_BLOCK_COUNT_CHECK_INFO_READ_ACK )
{
	InitBlockCount( kPacket_.m_byteBlockCheckCount, kPacket_.m_wstrBlockCheckResetDate );
	IncreaseBlockCount();
}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG// 작업날짜: 2013-05-15	// 박세훈
IMPL_ON_FUNC( EGS_FIELD_WORKINGS_BLOCK_LOG_ACK )
{
	KDBE_FIELD_WORKINGS_BLOCK_LOG_NOT kPacketToDB;
	kPacketToDB.m_iUnitUID			= GetCharUID();
	kPacketToDB.m_cX2State			= kPacket_.m_cX2State;
	kPacketToDB.m_iPhysicalMemory	= kPacket_.m_iPhysicalMemory;
	kPacketToDB.m_iVirtualMemory	= kPacket_.m_iVirtualMemory;
	kPacketToDB.m_cProcessTerm		= kPacket_.m_cProcessTerm;
	kPacketToDB.m_cFieldUserNum		= kPacket_.m_cFieldUserNum;
	kPacketToDB.m_cFieldNpcNum		= kPacket_.m_cFieldNpcNum;
	SendToLogDB( DBE_FIELD_WORKINGS_BLOCK_LOG_NOT, kPacketToDB );
}
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-07	// 박세훈
IMPL_ON_FUNC_NOPARAM( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( ( GetRoomUID() <= 0 ) || ( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD ) )
	{
		return;
	}

	SendToCnRoom( ERM_BOSS_FIELD_INTRUDE_RESTRICTION_REQ );
}

IMPL_ON_FUNC( ERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK )
{
	SendPacket( EGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_BOSS_FIELD_CLOSE_PROCESS_NOT )
{
	SendID( EGS_BOSS_FIELD_CLOSE_PROCESS_NOT );
}

IMPL_ON_FUNC_NOPARAM( ERM_BOSS_FIELD_RETURN_PROCESS_NOT )
{
	KEGS_BOSS_FIELD_RETURN_PROCESS_NOT kPacket;
	GetBossFieldJoinInfo( kPacket );
	SendPacket( EGS_BOSS_FIELD_RETURN_PROCESS_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_BOSS_FIELD_LOG_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
	SendToCnRoom( ERM_BOSS_FIELD_LOG_NOT, kPacket_ );
}
#endif // SERV_BATTLE_FIELD_BOSS
