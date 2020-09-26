#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "NexonBillingTCPManager.h"
#include "X2Data/XSLCashItemManager.h"
//{{ 2008. 11. 24  최육사	랭킹 매니저
#include "GSRankingManager.h"
//}}

//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "Enum/Enum.h"

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "./GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	#include "GSContentManager.h"
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2012. 04. 23	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
#include "../Common/X2Data/XSLBuffManager.h"
#include "UserManager.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
#include "GameSysVal/GameSysVal.h"
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
	#include "NewPartyListManager.h"
#endif SERV_RECRUIT_EVENT_SUBQUEST

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_STAGE_LOAD_REQ )
{
	//{{ 2010. 9. 8	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_REQ, EGS_DUNGEON_STAGE_LOAD_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_sPartyFever );
#else
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_cPartyFever );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	SendToCnRoom( ERM_DUNGEON_STAGE_LOAD_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_ACK, KEGS_DUNGEON_STAGE_LOAD_ACK )
{
	//{{ 2010. 04. 13  최육사	상태 오류	
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_ACK );
	//}}

	SendPacket( EGS_DUNGEON_STAGE_LOAD_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_NOT, KEGS_DUNGEON_STAGE_LOAD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_STAGE_LOAD_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ )
{
	//{{ 2010. 9. 8	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ, EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK );
#else
	_VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	SendToCnRoom( ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK )
{
	//{{ 2010. 04. 13  최육사	상태 오류
	_VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK );
	//}}

	SendPacket( EGS_DUNGEON_STAGE_LOAD_COMPLETE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_COMPLETE_NOT, KEGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_START_NOT, KEGS_DUNGEON_STAGE_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_STAGE_START_NOT, kPacket_ );
}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_CLEAR_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_CLEAR_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_CLEAR_ACK, KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_CLEAR_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_SECRET_STAGE_LOAD_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_STAGE_LOAD_ACK );

	SendToCnRoom( ERM_SECRET_STAGE_LOAD_REQ, kPacket_ );
}
_IMPL_ON_FUNC( ERM_SECRET_STAGE_LOAD_ACK, KEGS_DUNGEON_STAGE_LOAD_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_sPartyFever );
#else
	SiKPartyListManager()->GetPartyFever( GetPartyUID(), kPacket_.m_cPartyFever );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

#ifdef SERV_PVP_EQUIPMENT
	UnequipPvpEquipment();
#endif SERV_PVP_EQUIPMENT

	SendToCnRoom( ERM_DUNGEON_STAGE_LOAD_REQ, kPacket_ );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_OPEN_REQ )
{
	//{{ 2010. 9. 8	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_OPEN_REQ, EGS_DUNGEON_SUB_STAGE_OPEN_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_OPEN_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_OPEN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_OPEN_ACK, KEGS_DUNGEON_SUB_STAGE_OPEN_ACK )
{
	//{{ 2010. 04. 13  최육사	상태 오류
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_OPEN_ACK );
	//}}

	SendPacket( EGS_DUNGEON_SUB_STAGE_OPEN_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_OPEN_NOT, KEGS_DUNGEON_SUB_STAGE_OPEN_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_OPEN_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ )
{
	//{{ 2010. 9. 8	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ, EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_GO_NEXT_ACK, KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK )
{
	//{{ 2010. 04. 13  최육사	상태 오류
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
	//}}

	SendPacket( EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT, KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ, EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	SendToCnRoom( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK )
{
	//{{ 2010. 03. 17  최육사	상태 오류 수정
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );
	//}}

	SendPacket( EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_START_NOT, KEGS_DUNGEON_SUB_STAGE_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SUB_STAGE_START_NOT, kPacket_ );

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	if( IsReserveKickUser() == true )
	{
		KERM_LEAVE_ROOM_REQ kPacketReq;
		kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
		kPacketReq.m_iItemID = GetDungeonRequiredItemID();
		SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

		SetReserveKickUser( false );
		SetDungeonRequiredItemID( 0 );
	}
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}
}

//{{ 2009. 7. 20  최육사	비밀스테이지
IMPL_ON_FUNC_NOPARAM( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ )
{
	//{{ 2010. 9. 8	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ, EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	if( GetPartyUID() <= 0 )
	{
		START_LOG( cerr, L"partyuid값이 이상하다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK kPacket;
		kPacket.m_bCanEnterSecretStage = false;
		SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket );
		return;
	}

	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_DungeonSecretStageEnterCheck( GetThisPtr<KGSUser>() ) == false )
#else
#pragma ELSE_MESSAGE
	//if( SiKPartyManager()->E_DungeonSecretStageEnterCheck( GetPartyUID(), GetThisPtr<KGSUser>() ) == false )
#endif SERV_CHANNEL_PARTY
	//}}	
	{
		START_LOG( cerr, L"비밀 스테이지 입장 조건 체크 실패" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )			
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket_ );
}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
_IMPL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT, kPacket_ );
}
#else // SERV_STAGE_CLEAR_IN_SERVER
IMPL_ON_FUNC_NOPARAM( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendID( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

IMPL_ON_FUNC( EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT, kPacket_ );
}
//}}

_IMPL_ON_FUNC( ERM_END_GAME_DUNGEON_RESULT_DATA_NOT, KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_EVENT_CHECK_POWER
	IF_EVENT_ENABLED( CEI_CHECK_POWER )
	{
		bool bSuitableLevelDungeon = false;
		if( kPacket_.m_mapSuitableLevelInfo.find( GetCharUID() ) != kPacket_.m_mapSuitableLevelInfo.end() )
			bSuitableLevelDungeon = kPacket_.m_mapSuitableLevelInfo[ GetCharUID() ];

		BOOST_TEST_FOREACH( KDungeonUnitResultInfo&, kDungeonUnitResultInfo, kPacket_.m_vecDungeonUnitInfo )
		{
			if( kDungeonUnitResultInfo.m_UnitUID == GetCharUID() )
			{
				UpdateCheckPowerScore( kPacket_.m_iDungeonID, kDungeonUnitResultInfo.m_cTotalRank, kPacket_.m_vecDungeonUnitInfo.size(), bSuitableLevelDungeon, kPacket_.m_cDifficulty, kDungeonUnitResultInfo.m_bIsMVP, kPacket_.m_bIsWin );
				break;
			}
		}
	}
#endif SERV_EVENT_CHECK_POWER

	//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( ( CXSLDungeon::IsDefenceDungeon( kPacket_.m_iDungeonID ) == true ) &&
		( SiKGSWorldMissionManager()->GetIsActive() == true )
		)
	{
		if( kPacket_.m_bIsWin == true )
		{
			KGSPartyPtr partyPtr = SiKPartyListManager()->GetParty( GetPartyUID() );

			// 솔플 유저 였음
			if( partyPtr == NULL)
			{
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, 1 );
			}
			else if( partyPtr != NULL && partyPtr->GetHostUID() == GetCharUID() )
			{
				//	방장의 경우에만 글로벌서버에 기록한다.
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, partyPtr->GetNumMember() );
			}
		}

#ifdef SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
#else //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
		// 표식 용도의 가짜 버프
		int iBuffID = CXSLBuffManager::BTI_BUFF_2013_DEFENSE_FAKE_BUFF;

		// 현재 이 버프가 없다? 버프를 추가 한다.
		if( m_kUserBuffManager.IsBuffActivated( iBuffID ) == false )
		{
			const int iWorldBuffPriod = SiKGSWorldMissionManager()->GetWorldBuffEndTime();
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			ActivateBuff( iBuffID, kReq.m_vecActivateBuff, 0, iWorldBuffPriod );
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
#endif //SERV_NEW_DEFENCE_DUNGEON_NO_USE_DEFENSE_BUFF_AND_DEBUFF
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION	
	std::set< UidType > setPartyMembers;
#endif SERV_INTEGRATION	
	//}}

	// 던전 클리어 했는가?
	if( kPacket_.m_bIsWin == false )
	{
		// 던전 클리어 실패이므로 아무런 처리를 하지 않는다!
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		kPacket_.m_iProtectedSmallCrystal= 0;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가

#ifdef SERV_POINT_COUNT_SYSTEM
		m_kUserQuestManager.SetUpdateQuestInstance(GetThisPtr<KGSUser>());
#endif //SERV_POINT_COUNT_SYSTEM

		goto end_proc;
	}

	// RoomType 검사
	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"RoomType이 던전이 아닌데 던전결과창 정보가 내려왔다?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	{
		bool bRefreshBuff = false;		// 버프 갱신 해야하는가?
		int iBuffID = CXSLBuffManager::BTI_NONE;
		std::vector<KBuffInfo> vecActiveBuff;
		std::vector<int> vecDeleteBuff;
		std::vector<int> vecDeactiveBuff;

		// 귀환자가 아닌데 버프가 있다면 지운다.
		if( m_kComeBackUserInfo.GetIsComeBackUser() == false && kPacket_.m_bComeBackUserInParty == false )
		{
			DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, vecDeactiveBuff );

			bRefreshBuff = true;
		}

		//{{ 2013. 04. 13	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
#else
		if( kPacket_.m_bIsClearDefenceDungeon == true )
		{
			KGSPartyPtr partyPtr = SiKPartyListManager()->GetParty( GetPartyUID() );

			// 솔플 유저 였음
			if( partyPtr == NULL)
			{
				KEGB_CLEAR_MISSION_DUNGEON_NOT KNot;
				KNot = kPacket_.m_iProtectedSmallCrystal + 1;
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, KNot );
			}
			else if( partyPtr != NULL && partyPtr->GetHostUID() == GetCharUID() )
			{
				//	디펜스던전 클리어 했을경우 글로벌서버에 기록한다(방장의 경우에만) 작은 수정 + 큰 수정 
				KEGB_CLEAR_MISSION_DUNGEON_NOT KNot;
				KNot = kPacket_.m_iProtectedSmallCrystal + 1;
				SendToGlobalServer( EGB_CLEAR_MISSION_DUNGEON_NOT, KNot );
			}

			SiKGSWorldMissionManager()->GetIndividualBuffID( iBuffID, vecDeleteBuff );

			// 현재 이 버프가 없다? 낮은 레벨의 버프는 지우고 현재 버프를 추가 한다.
			if( m_kUserBuffManager.IsBuffActivated( iBuffID ) == false )
			{
				ActivateBuff( iBuffID, vecActiveBuff );

				BOOST_TEST_FOREACH(int, iDeleteBuff, vecDeleteBuff)
				{
					DeactivateBuff( iDeleteBuff, vecDeactiveBuff );
				}

				bRefreshBuff = true;
			}		
		}
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

		if( bRefreshBuff == true )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecActivateBuff = vecActiveBuff;
			kReq.m_vecDeActivateBuff = vecDeactiveBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	BOOST_TEST_FOREACH( const KDungeonUnitResultInfo&, kDungeonUnitResultInfo, kPacket_.m_vecDungeonUnitInfo )
	{
		setPartyMembers.insert( kDungeonUnitResultInfo.m_UnitUID );
	}
#endif SERV_INTEGRATION
	//}}

	// 해당 캐릭터 정보를 찾아서 업데이트
	BOOST_TEST_FOREACH( KDungeonUnitResultInfo&, kDungeonUnitResultInfo, kPacket_.m_vecDungeonUnitInfo )
	{
		if( kDungeonUnitResultInfo.m_UnitUID != GetCharUID() )
			continue;

		std::map< int, KSubQuestInfo > mapOngoingQuest;

		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND		
		kPacket_.GetOngoingQuestByUnitUID( GetCharUID(), mapOngoingQuest );
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		bool bHasFriend = false;

		if( GetPartyUID() > 0 )
		{
			START_LOG( cout, L"파티가 있음. 추천인 확인합니다." )
				<< BUILD_LOG( GetPartyUID() )
				<< END_LOG;

			// 추천인 있는지 우선 찾고
			std::vector< KRecommendUserInfo > vecRecruitUnitList;
			std::vector< UidType > vecRecruitUnitUID;
			m_kUserRecommendManager.GetRecruitUnitList( vecRecruitUnitList );

			if( vecRecruitUnitList.empty() == true )
			{
				m_kUserRecommendManager.GetRecruiterUnitList( vecRecruitUnitList );
			}

			BOOST_TEST_FOREACH( KRecommendUserInfo&, kRecruitUnitList, vecRecruitUnitList )
			{
				vecRecruitUnitUID.push_back( kRecruitUnitList.m_iUnitUID );
				START_LOG( cout, L"(피)추천인 추가" )
					<< BUILD_LOG( kRecruitUnitList.m_iUnitUID )
					<< END_LOG;
			}

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRecruitUnitUID )
			{
				START_LOG( cout, L"파티원UID 임" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;

				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					START_LOG( cout, L"파티원 중 (피)추천인을 찾음" )
						<< BUILD_LOG( iUnitUID )
						<< END_LOG;
					bHasFriend = true;
					break;
				}
			}
		}
#endif SERV_RECRUIT_EVENT_SUBQUEST

		//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
		m_kUserQuestManager.Handler_OnDungeonClear(
			GetThisPtr<KGSUser>(),
			kPacket_.m_iDungeonID,
			kPacket_.m_cDifficulty,
			kPacket_.m_cDungeonMode,
			kDungeonUnitResultInfo.m_cTotalRank,
			kPacket_.m_nSec,
			kDungeonUnitResultInfo.m_nDamaged,
			setPartyMembers,
			mapOngoingQuest,
			kPacket_.m_mapHaveExpInDungeon
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			, kPacket_.m_mapSuitableLevelInfo
#endif //SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
			, kPacket_.m_bSpecChar1, kPacket_.m_bSpecChar2
#endif //SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
			, kPacket_.m_vecRelation
#endif //SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
			, bHasFriend
#endif //SERV_RECRUIT_EVENT_SUBQUEST
#ifdef SERV_THREE_COLOR_EVENT
			, kPacket_.m_setEquippedTitle
#endif SERV_THREE_COLOR_EVENT
			);
		//}}
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
		{
			///여기서 던전 클리어 체크 해서 클라에 전달
			DungeonClearCountNot(kPacket_.m_iDungeonID, kPacket_.m_mapSuitableLevelInfo );
		}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		EventDungeonClearCountAdd( kPacket_.m_iDungeonID ); //던전 아이디를 넣어 주자
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

		//{{ 2013. 03. 21	 계정 퀘스트 - 적정 레벨 던전 클리어 조건 추가 - 김민성
#ifdef SERV_ACCOUNT_QUEST_ADD_CONDITION
		m_kUserAccountQuestManager.Handler_OnDungeonClear( GetThisPtr<KGSUser>(), kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, setPartyMembers, kPacket_.m_mapHaveExpInDungeon, kPacket_.m_mapSuitableLevelInfo );
#endif SERV_ACCOUNT_QUEST_ADD_CONDITION
		//}

		//{{ 2010. 8. 23	최육사	헬모드 칭호
#ifdef SERV_HELL_MODE_TITLE
		m_kUserTitleManager.OnDungeonClear( GetThisPtr<KGSUser>(), kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, kDungeonUnitResultInfo.m_cTotalRank, kPacket_.m_nSec, kDungeonUnitResultInfo.m_nDamaged, setPartyMembers );
#else
		//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
		//#ifdef SERV_INTEGRATION
		m_kUserTitleManager.OnDungeonClear( GetThisPtr<KGSUser>(), kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kDungeonUnitResultInfo.m_cTotalRank, kPacket_.m_nSec, kDungeonUnitResultInfo.m_nDamaged, setPartyMembers );
		//#endif SERV_INTEGRATION
		//}}
#endif SERV_HELL_MODE_TITLE
		//}}

		//튜토리얼이면 보상을 한번만 주기위해..
		if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true )
		{
			std::map< int, KDungeonClearInfo >::iterator mit;
			mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );

			if( mit != m_mapDungeonClear.end() )
			{
				kDungeonUnitResultInfo.m_nED = 0;
				kDungeonUnitResultInfo.ResetEXP();
			}
		}

		//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		START_LOG( clog2, L"방침미 시스템. 보상률1" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nED )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nClearBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nRankBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPartyBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nGameBangBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPremiumBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEventEXP )
			<< END_LOG;

		// 보너스 ED
		kDungeonUnitResultInfo.m_nED = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nED) * m_AntiAddictionManager.GetRewardRatio());

		// 보너스 경험치
		kDungeonUnitResultInfo.m_nEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nClearBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nClearBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nRankBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nRankBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nPartyBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nPartyBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nGameBangBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nGameBangBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nPremiumBonusEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nPremiumBonusEXP) * m_AntiAddictionManager.GetRewardRatio());
		kDungeonUnitResultInfo.m_nEventEXP = static_cast<int>(static_cast<float>(kDungeonUnitResultInfo.m_nEventEXP) * m_AntiAddictionManager.GetRewardRatio());

		START_LOG( clog2, L"방침미 시스템. 보상률2" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nED )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nClearBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nRankBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPartyBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nGameBangBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nPremiumBonusEXP )
			<< BUILD_LOG( kDungeonUnitResultInfo.m_nEventEXP )
			<< END_LOG;
#endif SERV_ANTI_ADDICTION_SYSTEM
		//}}

		break;		
	}

end_proc:

	SendPacket( EGS_END_GAME_DUNGEON_RESULT_DATA_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_UPDATE_DUNGEON_UNIT_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_bDungeonClear == true )
	{
		//////////////////////////////////////////////////////////////////////////
		//튜토리얼은 보상을 한번만 주기위해..
		if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true )
		{
			std::map< int, KDungeonClearInfo >::iterator mit;
			mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );

			if( mit != m_mapDungeonClear.end() )
			{
				kPacket_.m_iED	= 0;
				kPacket_.m_iEXP	= 0;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//게임시작후 첫번째 던전(초심자의 숲 노말) 클리어시 클리어 보너스 exp +800 주기위해
		//#첫번째 던전클리어인지는 룸서버에서 알수없고(현재 유저정보 최적화시 빠짐)
		// 첫번째 던전인 초심자의 숲은 혼자플레이만 될수있게 진행되기때문에 이곳에서 직접해준다.
		//if( m_mapDungeonClear.empty() == true &&
		//	kPacket_.m_iDungeonID == SEnum::DI_EL_FOREST_GATE_NORMAL )
		//{
		//	kPacket_.m_iEXP += 800;
		//}		
	}

	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG	
	m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_TIME, kPacket_.m_iPlayTime / 60 );
#endif SERV_CHAR_LOG
	//}}
	
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Dungeon_Play_Time, kPacket_.m_iPlayTime );
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	// 보너스 ED
	IncreaseED( kPacket_.m_iED, KUserEDManager::ER_IN_ED_DUNGEON_CLEAR_BONUS );
#else
	int iBeforeED = m_iED;

	//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	if(m_AntiAddictionManager.IsEnable())
	{
		START_LOG( clog, L"방침미 시스템. 보상률 적용전" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kPacket_.m_iED )
			<< BUILD_LOG( kPacket_.m_iEXP )
			<< BUILD_LOG( m_iED )
			<< BUILD_LOG( m_kEXP.GetExp() )
			<< END_LOG;

		// 보너스 ED
		kPacket_.m_iED = static_cast<int>(static_cast<float>(kPacket_.m_iED) * m_AntiAddictionManager.GetRewardRatio());

		// 보너스 경험치
		kPacket_.m_iEXP = static_cast<int>(static_cast<float>(kPacket_.m_iEXP) * m_AntiAddictionManager.GetRewardRatio());

		START_LOG( clog, L"방침미 시스템. 보상률 적용후" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< BUILD_LOG( kPacket_.m_iED )
			<< BUILD_LOG( kPacket_.m_iEXP )
			<< BUILD_LOG( m_iED )
			<< BUILD_LOG( m_kEXP.GetExp() )
			<< END_LOG;
	}
#endif // SERV_ANTI_ADDICTION_SYSTEM
	//}}

	// 보너스 ED
	m_iED += kPacket_.m_iED;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_DUNGEON_REWARD, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	

	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );

	// ED 페널티
	if( kPacket_.m_iED < 0 )
	{
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MDunPenalty, -kPacket_.m_iED );
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MDunPenalty, kPacket_.m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL
	}
	else
	{
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PDungeon, kPacket_.m_iED );

		// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PDungeon, kPacket_.m_iED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PDungeonED, kPacket_.m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL
		
	}

	// 보너스 경험치
	m_kEXP.AddExp( kPacket_.m_iEXP );

	//Update Level/Stat 
	CheckCharLevelUp();

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	// 버프 처리
	{
		std::vector< int > vecDeactivateBuff;
		m_kUserBuffManager.OnEndDungeonGame( GetThisPtr<KGSUser>(), kPacket_.m_bDungeonClear, kPacket_.m_bStartedByAutoParty, vecDeactivateBuff );
		if( vecDeactivateBuff.empty() == false )
		{
			// 로그인 서버 버프 매니저에 업데이트
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecDeActivateBuff = vecDeactivateBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	std::map< int, KDungeonPlayInfo >::iterator mitDungeonPlay = m_mapDungeonPlay.find( kPacket_.m_iDungeonID );
	if( mitDungeonPlay == m_mapDungeonPlay.end() )
	{
		// 일단 이 놈들은 해킹유저. 큰 문제는 아니니 차감횟수만 셋팅하자.
		KDungeonPlayInfo kDungeonPlayInfo;
		kDungeonPlayInfo.m_iDungeonID = kPacket_.m_iDungeonID;
		kDungeonPlayInfo.m_iPlayTimes = 1;
		kDungeonPlayInfo.m_iClearTimes = static_cast<int>( kPacket_.m_bDungeonClear );
		kDungeonPlayInfo.m_bNew = true;

		m_mapDungeonPlay.insert( std::make_pair( kDungeonPlayInfo.m_iDungeonID, kDungeonPlayInfo ) );
	}
	else
	{
		mitDungeonPlay->second.m_iClearTimes += static_cast<int>( kPacket_.m_bDungeonClear );
		mitDungeonPlay->second.m_bNew = true;
	}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

	//{{ 2012. 05. 14	최육사	던전 몬스터 레벨 지정
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_DUNGEON_NPC_LEVEL
	//////////////////////////////////////////////////////////////////////////
	//Update Dungeon Clear data 
	if( kPacket_.m_bDungeonClear == true )
	{
		std::map< int, KDungeonClearInfo >::iterator mit;
		mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );
		if( mit == m_mapDungeonClear.end() )
		{
			bool bIsClear = false;

			// 튜토리얼 던전은 무조건 클리어
			if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true )
			{	
				bIsClear = true;

				KDungeonClearInfo kDCInfo;

				CTime tCurrentTime = CTime::GetCurrentTime();
				kDCInfo.m_iDungeonID	= kPacket_.m_iDungeonID;
				kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
				kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
				kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
				kDCInfo.m_bNew = true;
				m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

				//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.SetLastClearDungeonID( kPacket_.m_iDungeonID );
				WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, kPacket_.m_iDungeonID );
#endif SERV_CHAR_LOG
				//}}
			}
			else
			{
				// 선행 던전을 클리어 했는지 검사
				const int iDifficulty = (kPacket_.m_iDungeonID % 10);
				const int iDungeonIDWithOutDif = kPacket_.m_iDungeonID - iDifficulty;
				int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonIDWithOutDif );			
				if( iRequireDungeonID > 0 )
				{
					std::map<int,KDungeonClearInfo>::iterator mit;
					mit = m_mapDungeonClear.find( iRequireDungeonID );
					if( mit != m_mapDungeonClear.end() )
					{
						bIsClear = true;
					}
				}
				else
				{
					bIsClear = true;
				}

				if( bIsClear == true )
				{
					switch( iDifficulty )
					{
					case CXSLDungeon::DL_EXPERT:
						{
							const int iExpertDungeonID = iDungeonIDWithOutDif + CXSLDungeon::DL_EXPERT;

							if( m_mapDungeonClear.find( iExpertDungeonID ) == m_mapDungeonClear.end() )
							{
								KDungeonClearInfo kDCInfo;
								CTime tCurrentTime = CTime::GetCurrentTime();
								kDCInfo.m_iDungeonID	= iExpertDungeonID;
								kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
								kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
								kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
								kDCInfo.m_bNew = true;
								m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

								//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
								m_kUserStatistics.SetLastClearDungeonID( iExpertDungeonID );
								WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, iExpertDungeonID );
#endif SERV_CHAR_LOG
								//}}
							}
						}
						// 의도적으로 break를 쓰지 않았습니다.

					case CXSLDungeon::DL_HARD:
						{
							const int iHardDungeonID = iDungeonIDWithOutDif + CXSLDungeon::DL_HARD;

							if( m_mapDungeonClear.find( iHardDungeonID ) == m_mapDungeonClear.end() )
							{
								KDungeonClearInfo kDCInfo;
								CTime tCurrentTime = CTime::GetCurrentTime();
								kDCInfo.m_iDungeonID	= iHardDungeonID;
								kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
								kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
								kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
								kDCInfo.m_bNew = true;
								m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

								//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
								m_kUserStatistics.SetLastClearDungeonID( iHardDungeonID );
								WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, iHardDungeonID );
#endif SERV_CHAR_LOG
								//}}
							}
						}
						// 의도적으로 break를 쓰지 않았습니다.

					case CXSLDungeon::DL_NORMAL:
						{
							const int iNormalDungeonID = iDungeonIDWithOutDif;

							if( m_mapDungeonClear.find( iNormalDungeonID ) == m_mapDungeonClear.end() )
							{
								KDungeonClearInfo kDCInfo;
								CTime tCurrentTime = CTime::GetCurrentTime();
								kDCInfo.m_iDungeonID	= iNormalDungeonID;
								kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
								kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
								kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
								kDCInfo.m_bNew = true;
								m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

								//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
								m_kUserStatistics.SetLastClearDungeonID( iNormalDungeonID );
								WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, iNormalDungeonID );
#endif SERV_CHAR_LOG
								//}}
							}
						}
						break;

					default:
						START_LOG( cerr, "난이도 정보가 이상합니다!" )
							<< BUILD_LOG( kPacket_.m_iDungeonID )
							<< BUILD_LOG( iDifficulty )
							<< END_LOG;
						break;
					}					
				}
			}
		}
		//클리어했던 던전이 있다면 최고점수 갱신체크
		else
		{
			if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == false )
			{
				bool bIsChange = false;

				if( mit->second.m_iMaxScore < kPacket_.m_iMaxScore )
				{
					bIsChange = true;
					mit->second.m_iMaxScore = kPacket_.m_iMaxScore;
				}

				if( mit->second.m_cMaxTotalRank < kPacket_.m_cMaxTotalRank )
				{
					bIsChange = true;
					mit->second.m_cMaxTotalRank = kPacket_.m_cMaxTotalRank;
				}

				if( bIsChange == true )
					mit->second.m_bNew = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
#else
	//////////////////////////////////////////////////////////////////////////
	//Update Dungeon Clear data 
	if( kPacket_.m_bDungeonClear == true )
	{
		std::map< int, KDungeonClearInfo >::iterator mit;
		mit = m_mapDungeonClear.find( kPacket_.m_iDungeonID );
		if( mit == m_mapDungeonClear.end() )
		{
			bool bIsClear = false;

			// 이전 난이도를 클리어했는지 검사
			int iDifficulty = (kPacket_.m_iDungeonID % 10);
			if( iDifficulty == 0 )
			{
				int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
				//선행던전이 있어야 한다.
				if( iRequireDungeonID > 0 )
				{
					std::map<int,KDungeonClearInfo>::iterator mit;
					mit = m_mapDungeonClear.find( iRequireDungeonID );

					if( mit != m_mapDungeonClear.end() )
					{
						bIsClear = true;
					}
				}
				else
				{
					bIsClear = true;
				}
			}
			else if ( 
				(iDifficulty > 0 && m_mapDungeonClear.find( kPacket_.m_iDungeonID - 1 ) != m_mapDungeonClear.end()) ||
				CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == true 
				)
			{	
				bIsClear = true;
			}

			if( bIsClear == true )
			{
				KDungeonClearInfo kDCInfo;

				CTime tCurrentTime = CTime::GetCurrentTime();
				kDCInfo.m_iDungeonID	= kPacket_.m_iDungeonID;
				kDCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
				kDCInfo.m_iMaxScore		= kPacket_.m_iMaxScore;
				kDCInfo.m_cMaxTotalRank	= kPacket_.m_cMaxTotalRank;
				kDCInfo.m_bNew = true;
				m_mapDungeonClear.insert( std::make_pair( kDCInfo.m_iDungeonID, kDCInfo ) );

				//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.SetLastClearDungeonID( kPacket_.m_iDungeonID );
				WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_DUNGEON_PLAY_COMPLETE, kPacket_.m_iDungeonID );
#endif SERV_CHAR_LOG
				//}}
			}
		}
		//클리어했던 던전이 있다면 최고점수 갱신체크
		else
		{
			if( CXSLDungeon::IsTutorialDungeon( kPacket_.m_iDungeonID ) == false )
			{
				bool bIsChange = false;

				if( mit->second.m_iMaxScore < kPacket_.m_iMaxScore )
				{
					bIsChange = true;
					mit->second.m_iMaxScore = kPacket_.m_iMaxScore;
				}

				if( mit->second.m_cMaxTotalRank < kPacket_.m_cMaxTotalRank )
				{
					bIsChange = true;
					mit->second.m_cMaxTotalRank = kPacket_.m_cMaxTotalRank;
				}

				if( bIsChange == true )
					mit->second.m_bNew = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
#endif SERV_DUNGEON_NPC_LEVEL
	//////////////////////////////////////////////////////////////////////////
	//}}

#ifndef SERV_HENIR_NO_REWARD
	//{{ 2009. 7. 6  최육사		랭킹정보 업데이트
	if( !kPacket_.m_kHenirRankingInfo.empty()  &&  !IsGuestUser() )
	{
		std::vector< KHenirRankingInfo >::const_iterator vitHR = kPacket_.m_kHenirRankingInfo.begin();
		if( vitHR != kPacket_.m_kHenirRankingInfo.end() )
		{
			// 개인 기록 저장
			m_kUserRankingManager.SetRankingInfo( *vitHR );

			// 100위안에 드는 랭킹인지 검사!
			if( SiKGSRankingManager()->CheckNewRecord( *vitHR ) == true )
			{
				// 100위안에 들었다!
				KELG_NEW_RECORD_HENIR_RANKING_NOT kNot;
				kNot.m_kNewRecordRanking = *vitHR;
				SendToLoginServer( ELG_NEW_RECORD_HENIR_RANKING_NOT, kNot );
			}
		}
		else
		{
			START_LOG( cerr, L"랭킹정보가 이상하다? 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_kHenirRankingInfo.size() )
				<< END_LOG;
		}
	}
	//}}
#endif //SERV_HENIR_NO_REWARD

	//{{ 2012. 02. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( GetPartyUID() > 0 )
	{
		SendToCnParty( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	m_kUserPetManager.Handler_ERM_UPDATE_DUNGEON_UNIT_INFO_NOT();
#endif SERV_PET_SYSTEM
	//}}

	KEGS_UPDATE_UNIT_INFO_NOT kUpdatePacketNot;

	//아이템이 있으면 신규아이템에 대한 db처리를 할것인지 체크
	if( !kPacket_.m_mapResultItem.empty() )
	{
		KDBE_INSERT_ITEM_REQ kPacketReq;
		kPacketReq.m_iUnitUID = GetCharUID();
		//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
		kPacketReq.m_cGetItemReason = SEnum::GIR_DUNGEON_RESULT;
#endif SERV_GET_ITEM_REASON
		//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX
		int iEventItemID = SiKGameSysVal()->GetDungeonClearPaymentItemID();

		std::map< int, KItemInfo >::iterator mitEventItemCheck = kPacket_.m_mapResultItem.find( iEventItemID );
		if( mitEventItemCheck != kPacket_.m_mapResultItem.end() )
		{
			int iQuantity = 0;

			if( m_kInventory.GetQuantity( iEventItemID, iQuantity ) == true )   // 인벤토리에 현자의 주문서가 있다면 지급하지 않는다.
			{
				std::map< int, int >::iterator mitGetItem = kPacket_.m_mapGetItem.find( iEventItemID );
				if( mitGetItem != kPacket_.m_mapGetItem.end() )
				{
					kPacket_.m_mapGetItem.erase( mitGetItem );	// 결과창에서 보여지는 아이템
				}

				kPacket_.m_mapResultItem.erase( mitEventItemCheck );	// 실제 습득 아이템
			}
			else if( m_kUserPetManager.ExistItemInPetInventory( iEventItemID ) == true ) // 펫 인벤토리에 있으면 지급하지 않는다.
			{
				std::map< int, int >::iterator mitGetItem = kPacket_.m_mapGetItem.find( iEventItemID );
				if( mitGetItem != kPacket_.m_mapGetItem.end() )
				{
					kPacket_.m_mapGetItem.erase( mitGetItem );	// 결과창에서 보여지는 아이템
				}

				kPacket_.m_mapResultItem.erase( mitEventItemCheck );	// 실제 습득 아이템
			}
		}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT_EX

		//{{ 2011. 12.13    김민성	던전 클리어 시 아이템 지급 이벤트 - 현자의 주문서(중복 지급 금지)
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
		std::map< int, KItemInfo >::iterator mitEventItemCheck = kPacket_.m_mapResultItem.find( CXSLItem::EI_CHAR_LEVEL_UP_ITEM );
		if( mitEventItemCheck != kPacket_.m_mapResultItem.end() )
		{
			int iQuantity = 0;
			if( ( m_kInventory.GetQuantity( CXSLItem::EI_CHAR_LEVEL_UP_ITEM, iQuantity ) == true )	||	// 인벤토리나
				( m_kUserPetManager.ExistItemInPetInventory( CXSLItem::EI_CHAR_LEVEL_UP_ITEM ) == true ) // 펫 인벤토리에 현자의 주문서가 있다면 지급하지 않는다.
				)
			{
				std::map< int, int >::iterator mitGetItem = kPacket_.m_mapGetItem.find( CXSLItem::EI_CHAR_LEVEL_UP_ITEM );
				if( mitGetItem != kPacket_.m_mapGetItem.end() )
				{
					kPacket_.m_mapGetItem.erase( mitGetItem );	// 결과창에서 보여지는 아이템
				}

				kPacket_.m_mapResultItem.erase( mitEventItemCheck );	// 실제 습득 아이템
			}
		}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
		//}}
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		// 삭제할 아이템 목록은 따로 관리
		std::map< int, int > mapConsumingItem;

		std::map< int, KItemInfo >::iterator mitResultItem = kPacket_.m_mapResultItem.begin();
		while( mitResultItem != kPacket_.m_mapResultItem.end() )
		{
			int iItemID = mitResultItem->first;
			KItemInfo kItemInfo = mitResultItem->second;

			START_LOG( clog, L"소모되는 아이템" )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( kItemInfo.m_iQuantity );

			if( kItemInfo.m_iQuantity <= 0 )
			{
				kPacket_.m_mapResultItem.erase( mitResultItem++ );

				if( kItemInfo.m_iQuantity < 0 )
				{
					mapConsumingItem.insert( std::make_pair( iItemID, -kItemInfo.m_iQuantity ) );
				}
			}
			else
			{
				mitResultItem++;
			}
		}

		KEGS_UPDATE_INVENTORY_SLOT_INFO_NOT kPacket;
		m_kInventory.DeleteItemOnlyInventory( mapConsumingItem, kPacket.m_vecInventorySlotInfo, KDeletedItemInfo::DR_DUNGEON_REWARD );
		SendPacket( EGS_UPDATE_INVENTORY_SLOT_INFO_NOT, kPacket );
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

		m_kInventory.PrepareInsert( kPacket_.m_mapResultItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

		//////////////////////////////////////////////////////////////////////////
		// 통계 : 게임중 습득아이템 정보
		int iDropItemCnt = 0;
		std::map< int, int >::const_iterator mit;
		for( mit = kPacketReq.m_mapInsertedItem.begin(); mit != kPacketReq.m_mapInsertedItem.end(); ++mit )
		{
			const int iItemID = mit->first;
			const int iQuantity = mit->second;

			// ED아이템인지 검사
			switch( iItemID )
			{
			case CXSLItem::EDI_BRONZE_ED:
			case CXSLItem::EDI_SILVER_ED:
			case CXSLItem::EDI_GOLD_ED:
				continue;
			}

			// 일반아이템이라면 카운트
			iDropItemCnt += iQuantity;

			// 통계 : Local Item
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( iItemID );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
				//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
				KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_DROP, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
				//}}
			}
		}

		// 유저 통계 습득 아이템
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, iDropItemCnt );

		//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
		m_kUserAbuserManager.IncreaseGetItemCount( GetThisPtr<KGSUser>(), iDropItemCnt );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
		//}}

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// 아이템 어뷰저
		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, kPacketReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
		//}}
		//////////////////////////////////////////////////////////////////////////

		// 인벤토리에 들어가지 못한 아이템이 있는지 검사!
		std::map< int, KItemInfo >::const_iterator mitResult;
		for( mitResult = kPacket_.m_mapResultItem.begin(); mitResult != kPacket_.m_mapResultItem.end(); ++mitResult )
		{
			std::map< int, int >::const_iterator mitFind = kPacketReq.m_mapInsertedItem.find( mitResult->first );
			if( mitFind == kPacketReq.m_mapInsertedItem.end() )
			{
				// 인벤토리에 하나도 못넣었단 말이냐!?
				for( int i = 0; i < mitResult->second.m_iQuantity; ++i )
				{
					//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
					AddTempInventoryItem( mitResult->first
										, 0
										//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
										, mitResult->second
#endif SERV_ENCHANT_ITEM_DROP_EVENT
										//}}
										);
#else
					// 임시 인벤토리로 저장하러 가기!
					KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
					if( m_kTempInventory.AddTempInventoryItem( mitResult->first, 0, ++kPacket_.m_iDropItemUID, kNot.m_kTempInvenItem ) )
					{
						SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
					}
					else
					{
						START_LOG( cerr, L"임시 인벤토리에 아이템 저장 실패!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( mitResult->first )
							<< END_LOG;
					}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
					//}}
				}
			}
			else
			{
				// 몇개정도 넣었나?
				int iGetCount = mitResult->second.m_iQuantity - mitFind->second;
				LIF( iGetCount >= 0 );
				for( int i = 0; i < iGetCount; ++i )
				{
					//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
					AddTempInventoryItem( mitResult->first
										, 0
										//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
										, mitResult->second
#endif SERV_ENCHANT_ITEM_DROP_EVENT
										//}}
										);
#else
					// 임시 인벤토리로 저장하러 가기!
					KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
					if( m_kTempInventory.AddTempInventoryItem( mitResult->first, 0, ++kPacket_.m_iDropItemUID, kNot.m_kTempInvenItem ) )
					{
						SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
					}
					else
					{
						START_LOG( cerr, L"임시 인벤토리에 아이템 저장 실패!" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( mitResult->first )
							<< END_LOG;
					}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
					//}}					
				}
			}
		}

		// DB로 보낼정보가 있는가?
		if( !kPacketReq.m_vecItemInfo.empty() )
		{
			m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
			m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
			kPacketReq.m_bOutRoom = kPacket_.m_bOutRoom;

			//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
			kPacketReq.m_mapGetItem = kPacket_.m_mapGetItem;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
			//}}

			SendToGameDB( DBE_INSERT_ITEM_REQ, kPacketReq );
			return;
		}
		else
		{
			kUpdatePacketNot.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
		}
	}

	// Update Server Unit data
	KRoomUserInfo kInfo;
	GetRoomUserInfo( kInfo );

	// 방에서 나가는 경우가 아닐경우 센터서버에 업데이트를 시켜준다.
	// 아이템 처리가 끝난후 업데이트를 해야하기때문에 이곳에서 함.
	if( kPacket_.m_bOutRoom == false )
	{
		SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
	}

	// 유저에게 수정된 데이터를 넘겨준다.
	GetUnitInfo( kUpdatePacketNot.m_kUnitInfo );
	kUpdatePacketNot.m_bGameEnd = true;
	kUpdatePacketNot.m_mapItemObtained = kPacket_.m_mapGetItem;	

	SendPacket( EGS_UPDATE_UNIT_INFO_NOT, kUpdatePacketNot );
}

IMPL_ON_FUNC( EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
	
	if( IsHenirDungeonChallengeMode() )
	{
		// 헤니르 시공에서 도전모드는 부활석 사용 불가!
		START_LOG( cerr, L"클라이언트에서 도전모드인지 체크했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_RESURRECT_03;
		SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
		return;
	}

	if( !kPacket_.m_bUseCash )
	{
		//{{ 2009. 10. 14  최육사	자동결제 부활석
#ifdef AP_RESTONE
		if( m_iNumResurrectionStone <= 0  &&  m_iNumAutoPaymentResStone <= 0 )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RESURRECT_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}
#else
		if( m_iNumResurrectionStone <= 0 )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_RESURRECT_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}
#endif AP_RESTONE		
		//}}		

		kPacket_.m_iUsedByUnitUID = GetCharUID(); // 부활석 사용자
		SendToCnRoom( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, kPacket_ );
	}
	else
	{	
		//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
		if( SiKGSContentManager()->IsEnableCashShop() == false )
		{
			START_LOG( clog, L"캐쉬샵 점검중 캐쉬 부활석 사용 요청" )
				<< BUILD_LOG( GetName() );

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
			return;
		}
#endif SERV_CONTENT_MANAGER
		//}}

		// 체험ID 기능 제한
		if( IsGuestUser() )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		// 실시간 부활석 사용 시간 제한
		if( m_kTimer[TM_CASH_RES_STONE].elapsed() < 2.0 )
			return;

		m_kTimer[TM_CASH_RES_STONE].restart();

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING

#if defined SERV_COUNTRY_TWHK
		const int ciResurrectionStoneID = 51;
#elif defined SERV_COUNTRY_JP
		const int ciResurrectionStoneID = 1000051;
#elif defined SERV_COUNTRY_CN
		const int ciResurrectionStoneID = 10001;
#elif defined SERV_COUNTRY_BR
		const int ciResurrectionStoneID = 50051;
#else // SERV_COUNTRY_XX
		const int ciResurrectionStoneID = 51;
#endif //SERV_COUNTRY_XX

		KBillProductInfo kBillProductInfo;
		if( !SiKBillManager()->GetProductInfo( ciResurrectionStoneID, kBillProductInfo ) )
		{
			START_LOG( cerr, L"실시간 부활석 상품 정보 추출 실패." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		KEBILL_BUY_PRODUCT_REQ kPacketReq;
		kPacketReq.m_wstrTotalOrderID = MakeOrderID();	// TotalOrderID를 먼저 발급
		kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacketReq.m_wstrUserID = GetName();
		kPacketReq.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacketReq.m_iUserUID = GetUID();
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_wstrNickName = GetCharName();
		kPacketReq.m_strUserIP = GetIPStr();

		// 허상형 : [2010/7/29/] //	캐시 로그
		kPacketReq.m_BuyCharInfo.m_cLevel = GetLevel();
		kPacketReq.m_BuyCharInfo.m_iUnitClass = GetUnitClass();

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		kPacketReq.m_iUserIP = GetIP();

#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
		kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_KOG_ELSWORD_CASH;
#elif defined (SERV_COUNTRY_TWHK)
		// 일단 UI 작업 전까지는 GASH로 요청
		kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_CASH;

// 		// Gash 가 남아있을 때는 Gash로 요청
// 		if(m_GlobalCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_CASH] > kBillProductInfo.m_iSalePrice)
// 		{
// 			kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_CASH;
// 		}
// 		// Gash 없을 경우 엘소드 포인트로 요청
// 		else
// 		{
// 			kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH;
// 		}
		
#else // SERV_COUNTRY_XX
		kPacketReq.m_iUseCashType = (int)KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif // SERV_COUNTRY_CN
		
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef SERV_BALANCE_CHEAK_BEFORE_REALTIME_RESSTONE		
		//잔액 검사
		if( m_GlobalCashInfo.m_ulCash[kPacketReq.m_iUseCashType] < kBillProductInfo.m_iSalePrice )
		{
			START_LOG( cerr, L"잔액 없는데 실시간 부활석 사용 했다 " )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( m_GlobalCashInfo.m_ulCash[kPacketReq.m_iUseCashType] )
				<< BUILD_LOG( kBillProductInfo.m_iSalePrice )
				<< END_LOG;

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_16;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}
#endif //SERV_BALANCE_CHEAK_BEFORE_REALTIME_RESSTONE

		KBillBuyInfo kInfo;
		kInfo.m_wstrOrderID = MakeOrderID();
		kInfo.m_iProductNo = kBillProductInfo.m_iProductNo;
		kInfo.m_iPoint = kBillProductInfo.m_iSalePrice;
		kInfo.m_iProductID = kBillProductInfo.m_iProductID;
		kInfo.m_usOrderQuantity = 1;
#if defined(SERV_COUNTRY_BR) || defined(SERV_COUNTRY_ID)
		kInfo.m_wstrMemo = kBillProductInfo.m_wstrProductName;		
#endif //SERV_COUNTRY_BR
		kInfo.m_iCategory = ( int )kBillProductInfo.m_cCategoryNo;
		kPacketReq.m_vecBillBuyInfo.push_back( kInfo );

		DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
		DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
		DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();

#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
		SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
#else // SERV_COUNTRY_CN
		if(dwBillingFlag == KSimLayer::BF_INTERNAL)
		{
			SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
		else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
		{
			// 구분해서 DB로 보낼지 TCP로 보낼지 결정해야 함
			if (kPacketReq.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_CASH)
				SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacketReq );
			else //
				SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
		else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
		{
			SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
		else
		{
			SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacketReq );
		}
#endif // SERV_COUNTRY_CN
		// 찜에서 캐쉬구매
		m_kUserWishList.SetBuyCashItemInWishList( false );

		// 임시 데이터 설정
		m_kUserCashInventory.SetCashResurrectionStone( true );
		m_kUserCashInventory.SetCashRStoneUnitUID( kPacket_.m_iUnitUID );

#else // SERV_GLOBAL_BILLING
		// 부활석 한도를 넘지 못하게 한다.
		KNXBTProductInfo kProductInfo;
		if( !SiKNexonBillingTCPManager()->GetProductInfo( SiCXSLCashItemManager()->GetProductNoResStone(), kProductInfo ) )
		{
			START_LOG( cerr, L"실시간 부활석 상품 정보 추출 실패." )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		if( m_iNumResurrectionStone + kProductInfo.m_usProductPieces > 2000 )
		{
			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_18;
			SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );
			return;
		}

		KENX_BT_PURCHASE_ITEM_REQ kPacketReq;
		kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
		kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::PURCHASE_ITEM;
		kPacketReq.m_ulRemoteIP = GetIP();
		kPacketReq.m_byteReason = 1;            // Game Client
		//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
		if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
		{
			GetToonilandID( kPacketReq.m_wstrGameID );
			GetToonilandID( kPacketReq.m_wstrUserID );
			if( kPacketReq.m_wstrGameID.empty() )
			{
				START_LOG( cerr, L"투니랜드ID가 없다? 솝이 맛이 갓나?" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetName() )
					<< END_LOG;

				KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
				return;
			}
		}
		else
		{
			kPacketReq.m_wstrGameID = GetName();    // 인벤토리 공유를 위해 Nexon ID를 쓴다.
			kPacketReq.m_wstrUserID = GetName();
		}
#else
		kPacketReq.m_wstrGameID = GetName();    // 인벤토리 공유를 위해 Nexon ID를 쓴다.
		kPacketReq.m_wstrUserID = GetName();
#endif SERV_CHANNELING_SOAP
		//}}
		kPacketReq.m_ulUserOID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacketReq.m_wstrUserName = GetName();
		kPacketReq.m_byteUserAge = BYTE( m_kNexonAccountInfo.m_uiAge );
		CTime tCurrent = CTime::GetCurrentTime();
		kPacketReq.m_wstrOrderID = ( CStringW )tCurrent.Format( _T( "%Y%m%d%H%M%S" ) );
		wchar_t wszNumber[32];
#ifdef _CONVERT_VS_2010
		_i64tow_s( KBaseServer::GetKObj()->GetUID(), wszNumber, 32, 10 );
#else
		_i64tow( KBaseServer::GetKObj()->GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
		kPacketReq.m_wstrOrderID += L"_";
		kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
		_i64tow_s( GetUID(), wszNumber, 32, 10 );
#else
		_i64tow( GetUID(), wszNumber, 10 );
#endif _CONVERT_VS_2010
		kPacketReq.m_wstrOrderID += L"_";
		kPacketReq.m_wstrOrderID += wszNumber;
#ifdef _CONVERT_VS_2010
		_i64tow_s( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 32, 10 );
#else
		_i64tow( SiKNexonBillingTCPManager()->GetNextPurchaseNo(), wszNumber, 10 );
#endif _CONVERT_VS_2010
		kPacketReq.m_wstrOrderID += L"_";
		kPacketReq.m_wstrOrderID += wszNumber;

		START_LOG( clog, L"주문번호" )
			<< BUILD_LOG( kPacketReq.m_wstrOrderID );

		//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
		if( GetChannelCode() == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )  // 투니랜드 채널링 유저
		{
			kPacketReq.m_ulPaymentType = 14001;
		}
		else
		{
			kPacketReq.m_ulPaymentType = 13001;
		}
#else
		kPacketReq.m_ulPaymentType = 13001;
#endif SERV_TOONILAND_CHANNELING
		//}}

		kPacketReq.m_ulTotalAmount = kProductInfo.m_ulSalePrice;

		KNXBTPurchaseReqInfo kPurchaseReqInfo;
		kPurchaseReqInfo.m_ulProductNo	   = SiCXSLCashItemManager()->GetProductNoResStone();
		kPurchaseReqInfo.m_usOrderQuantity = 1;
		kPacketReq.m_vecNXBTPurchaseInfo.push_back( kPurchaseReqInfo );

		// 찜에서 캐쉬구매
		m_kUserWishList.SetBuyCashItemInWishList( false );

		// 임시 데이터 설정
		m_kUserCashInventory.SetCashResurrectionStone( true );
		m_kUserCashInventory.SetCashRStoneUnitUID( kPacket_.m_iUnitUID );

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_PURCHASE_ITEM_REQ, kPacketReq );
		SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
#endif // SERV_GLOBAL_BILLING
	}
}

IMPL_ON_FUNC( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK )
{
	// 성공적으로 부활처리 되었는데 나가버리면 어떡하지? 그러면 소환석이 소모되지 않는다.
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 2009. 10. 14  최육사	자동결제 부활석
#ifdef AP_RESTONE
		if( m_iNumResurrectionStone <= 0  &&  m_iNumAutoPaymentResStone <= 0 )
		{
			START_LOG( cerr, L"소환석 부족. 이미 이전에 확인한 내용이기 때문에 이런 상황이 벌어지면 안된다." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( m_iNumResurrectionStone )
				<< BUILD_LOG( m_iNumAutoPaymentResStone )
				<< END_LOG;
		}
		else
		{
			// 만약 자동결제 부활석이 남아있다면 자동결제 부활석 부터 소모시킵니다.
			if( m_iNumAutoPaymentResStone > 0 )
			{
				m_iNumAutoPaymentResStone -= 1;
			}
			else
			{
				m_iNumResurrectionStone -= 1;
			}

#ifdef GIANT_RESURRECTION_CASHSTONE
			if(m_iNumResurrectionStone == 0)
			{
				KDBE_REALTIME_RESURRECTION_CASHSTONE_NOT kPakcetToGame;
				kPakcetToGame.m_iUnitUID = GetCharUID();
				kPakcetToGame.m_iNumResurrection_CashStone = m_iNumResurrectionStone.GetChangeValue();
				m_iNumResurrectionStone.SetValue( m_iNumResurrectionStone );

				SendToGameDB(DBE_REALTIME_RESURRECTION_CASHSTONE_NOT , kPakcetToGame);
			}
#endif //GIANT_RESURRECTION_CASHSTONE

			// 부활석 로그
			KELOG_RESURRECTION_STONE_LOG_NOT kPacketToLog;
			CTime kUsedTime = CTime::GetCurrentTime();
			kPacketToLog.m_UnitUID	   = GetCharUID();
			kPacketToLog.m_iDungeonID  = kPacket_.m_iDungeonID;
			kPacketToLog.m_wstrRegDate = ( CStringW )( kUsedTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
			SendToLogDB( ELOG_RESURRECTION_STONE_LOG_NOT, kPacketToLog );
		}

		//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		UseResurrectionStone( true );		// 부활석 사용을 저장
#endif SERV_ADD_TITLE_CONDITION
		//}}
		//{{ 2013. 02. 05  칭호 획득 조건 추가(부활석 사용 횟수, 샌더 마을) - 김민성
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
		AddUseResurrectionStoneCount();
#endif SERV_ADD_TITLE_CONDITION_SANDER
		//}
		kPacket_.m_iNumResurrectionStone = m_iNumResurrectionStone;				// 부활석
		kPacket_.m_iNumAutoPaymentResStone = m_iNumAutoPaymentResStone;			// 자동결제 부활석
		kPacket_.m_bUseCash = m_kUserCashInventory.IsCashResurrectionStone();	// 실시간 부활석 여부
#else
		if( m_iNumResurrectionStone <= 0 )
		{
			START_LOG( cerr, L"소환석 부족. 이미 이전에 확인한 내용이기 때문에 이런 상황이 벌어지면 안된다." )
				<< BUILD_LOG( m_iNumResurrectionStone )
				<< END_LOG;
		}
		else
		{
			m_iNumResurrectionStone -= 1;

			// 부활석 로그
			KELOG_RESURRECTION_STONE_LOG_NOT kPacketToLog;
			CTime kUsedTime = CTime::GetCurrentTime();
			kPacketToLog.m_UnitUID	   = GetCharUID();
			kPacketToLog.m_iDungeonID  = kPacket_.m_iDungeonID;
			kPacketToLog.m_wstrRegDate = ( CStringW )( kUsedTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
			SendToLogDB( ELOG_RESURRECTION_STONE_LOG_NOT, kPacketToLog );			
		}

		kPacket_.m_iNumResurrectionStone = m_iNumResurrectionStone;		
		kPacket_.m_bUseCash = m_kUserCashInventory.IsCashResurrectionStone(); // 실시간 부활석
#endif AP_RESTONE
		//}}

		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		// 어둠의 문에서 부활한거라면 전용 버프를 다시 걸자!
		if( CXSLDungeon::IsDefenceDungeon( m_kUserDungeonManager.GetDungeonID() ) == true )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;

			if( ActivateBuff( CXSLBuffManager::BTI_BUFF_GATE_DEFENCE, kReq.m_vecActivateBuff ) == true )
			{
				kReq.m_iUnitUID = GetCharUID();
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
	}

	m_kUserCashInventory.SetCashResurrectionStone( false ); // 초기화

	SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_STOP_DUNGEON_CONTINUE_TIME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_STOP_DUNGEON_CONTINUE_TIME_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_SKIP_CONTINUE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_SKIP_CONTINUE_DUNGEON_NOT );
}

//{{ 2010. 05. 06  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL

_IMPL_ON_FUNC( ERM_DUNGEON_EFFECT_REMAINING_TIME_NOT, KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_EFFECT_TIME_OUT_NOT, KEGS_DUNGEON_EFFECT_TIME_OUT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_EFFECT_TIME_OUT_NOT, kPacket_ );
}

#endif SERV_SECRET_HELL
//}}

IMPL_ON_FUNC( ERM_DUNGEON_USER_STATISTICS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iGamePlayTime < 0 )
		kPacket_.m_iGamePlayTime = 0;

	// 유저 플레이 타임
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_TOTAL_PLAY_TIME, kPacket_.m_iGamePlayTime );
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_DUNGEON_PLAY_TIME, kPacket_.m_iGamePlayTime );
	//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_SPIRIT_PLAY_TIME, kPacket_.m_iGamePlayTime );
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}

	// 유저 통계 대전
	switch( kPacket_.m_cGameResult )
	{
	case KERM_DUNGEON_USER_STATISTICS_NOT::UGR_WIN:
		{
			// 유저 통계 던전 승
			if( kPacket_.m_bIsPartyPlay == false )
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Win, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Win, kPacket_.m_iGamePlayTime );
			}
			else
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_Win, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Party_Win, kPacket_.m_iGamePlayTime );
			}			
		}
		break;

	case KERM_DUNGEON_USER_STATISTICS_NOT::UGR_LOSE:
		{
			// 유저 통계 던전 패
			if( kPacket_.m_bIsPartyPlay == false )
			{			
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Lose, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Lose, kPacket_.m_iGamePlayTime );
			}
			else
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_Lose, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Party_Lose, kPacket_.m_iGamePlayTime );
			}			
		}
		break;

	case KERM_DUNGEON_USER_STATISTICS_NOT::UGR_DROP:
		{
			// 유저 통계 던전 이탈
			if( kPacket_.m_bIsPartyPlay == false )
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Drop, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Drop, kPacket_.m_iGamePlayTime );
			}
			else
			{
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_Drop, 1 );
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_PT_Party_Drop, kPacket_.m_iGamePlayTime );
			}
		}
		break;

	default:
		START_LOG( cerr, L"이런 결과 타입은 없음." )
			<< BUILD_LOGc( kPacket_.m_cGameResult )
			<< END_LOG;
		break;
	}

	// 유저 통계 던전 부활석
	if( kPacket_.m_bIsPartyPlay == false )
	{
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_ReStone, kPacket_.m_iUsedResurrectionStone );
	}
	else
	{
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_Dungeon, kPacket_.m_iDungeonID, KUserStatistics::US_D_MC_Party_ReStone, kPacket_.m_iUsedResurrectionStone );
	}
}

IMPL_ON_FUNC( ERM_TC_USER_STATISTICS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iGamePlayTime < 0 )
		kPacket_.m_iGamePlayTime = 0;

	// 유저 플레이 타임
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_TOTAL_PLAY_TIME, kPacket_.m_iGamePlayTime );	

	// 유저 통계 훈련소
	switch( kPacket_.m_cGameResult )
	{
	case KERM_TC_USER_STATISTICS_NOT::UGR_SUCCESS:
		{
			// 유저 통계 훈련소 성공
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_MC_Success, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_PT_Success, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_TC_USER_STATISTICS_NOT::UGR_FAIL:
		{
			// 유저 통계 훈련소 실패
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_MC_Fail, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_PT_Fail, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_TC_USER_STATISTICS_NOT::UGR_DROP:
		{
			// 유저 통계 훈련소 이탈
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_MC_Drop, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_TrainingCenter, kPacket_.m_iTCID, KUserStatistics::US_TC_PT_Drop, kPacket_.m_iGamePlayTime );
		}
		break;

	default:
		START_LOG( cerr, L"이런 결과 타입은 없음." )
			<< BUILD_LOGc( kPacket_.m_cGameResult )
			<< END_LOG;
		break;
	}
}

//{{ 2011. 01. 25  김민성  월드 트리거 전달(던전만 적용중 - 이후 PVP 확장)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
IMPL_ON_FUNC( EGS_WORLD_TRIGGER_RELOCATION_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방 상태인데 RoomUID가 없네? 이상하다." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_WORLD_TRIGGER_RELOCATION_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_WORLD_TRIGGER_RELOCATION_ACK, kPacket );
		return;
	}

	SendToCnRoom( ECN_WORLD_TRIGGER_RELOCATION_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_ACK, KEGS_WORLD_TRIGGER_RELOCATION_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_WORLD_TRIGGER_RELOCATION_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_NOT, KEGS_WORLD_TRIGGER_RELOCATION_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_WORLD_TRIGGER_RELOCATION_NOT, kPacket_ );
}

#endif SERV_WORLD_TRIGGER_RELOCATION
//}}

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
IMPL_ON_FUNC( EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방 상태인데 RoomUID가 없네? 이상하다." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_BAD_ATTITUDE_USER_CHECK_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BAD_ATTITUDE_USER_MSG_NOT, KEGS_BAD_ATTITUDE_USER_MSG_NOT )
{
	SendPacket( EGS_BAD_ATTITUDE_USER_MSG_NOT , kPacket_ );
}

_IMPL_ON_FUNC( ERM_BAD_ATTITUDE_USER_NOT, KEGS_BAD_ATTITUDE_USER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BAD_ATTITUDE_USER_NOT , kPacket_ );
}

IMPL_ON_FUNC( EGS_FORCED_EXIT_VOTE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방 상태인데 RoomUID가 없네? 이상하다." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_FORCED_EXIT_VOTE_REQ, kPacket_ );
}

IMPL_ON_FUNC( EGS_DEFENCE_DUNGEON_WAVE_ID_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방 상태인데 RoomUID가 없네? 이상하다." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_DEFENCE_DUNGEON_WAVE_ID_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_FORCED_EXIT_VOTE_ACK, KEGS_FORCED_EXIT_VOTE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_FORCED_EXIT_VOTE_ACK , kPacket_ );
}

IMPL_ON_FUNC( ERM_BAD_ATTITUDE_USER_LOG_NOT )
{
	SendToLogDB( DBE_BAD_ATTITUDE_USER_LOG_NOT, kPacket_ );	
}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
_IMPL_ON_FUNC( ERM_DECISION_ENTER_DEFENCE_DUNGEON_NOT, KEGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PREPARE_FOR_DEFENCE_DUNGEON_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"현재 방에 입장한 상태가 아닙니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}
	
	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"현재 던전 방에 입장한 상태가 아닙니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kDefenceDungeonInfo );
#endif SERV_REFORM_QUEST
	//}}

	KERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK kPacketAck;
	GetRoomUserInfo( kPacketAck.m_kRoomUserInfo );
	SendToCnRoom( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK, kPacketAck );

	//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	// 어둠의 문 입장 여부를 확인하기 위한 팝업 띄우는 패킷 전송
	SendID( EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}
}

_IMPL_ON_FUNC( ERM_DEFENCE_DUNGEON_GAME_START_NOT, KEGS_DEFENCE_DUNGEON_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_LEAVE_ROOM_REQ kPacket;
		kPacket.m_bSendAck = false;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_28;
		SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

		// 이탈처리 했으니 여기서 쫑!
		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if( GetAuthLevel() < SEnum::UAL_GM )
		kPacket_.m_wstrCNIP.clear();

	//SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_RoomInfo.m_iDungeonID, kPacket_.m_RoomInfo.m_DifficultyLevel, kPacket_.m_RoomInfo.m_cDungeonMode );
#endif SERV_REFORM_QUEST
	//}}

#ifdef SERV_ADD_TITLE_CONDITION
	UseResurrectionStone( false );
	SetStartedNumMember( kPacket_.m_RoomInfo.m_JoinSlot );
#endif SERV_ADD_TITLE_CONDITION

	//{{ 2013. 02. 05  칭호 획득 조건 추가(부활석 사용 횟수, 샌더 마을) - 김민성
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	ClearUseResurrectionStoneCount();
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}

	//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 어둠의 문 전용 퀵슬롯 아이템 설정
	m_kInventory.SetupDefenceDungeonQuickSlotItems( CXSLDefenceDungeonManager::DDP_PHASE_1, kPacket_.m_vecInventorySlotInfo );
    //////////////////////////////////////////////////////////////////////////
	
	SendPacket( EGS_DEFENCE_DUNGEON_GAME_START_NOT, kPacket_ );

	//{{ 2009. 5. 30  최육사	임시 인벤
	ClearTempInventory();
	//}}

	// 던전이 새로 바뀌었으니 GameStat도 변경!
	//KRoomUserInfo kRoomUserInfo;
	//GetRoomUserInfo( kRoomUserInfo );
	//SendToCnRoom( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, kRoomUserInfo );

	// 어둠의 문 전용 버프도 걸자!
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;

		if( ActivateBuff( CXSLBuffManager::BTI_BUFF_GATE_DEFENCE, kReq.m_vecActivateBuff ) == true )
		{
			kReq.m_iUnitUID = GetCharUID();
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
}

_IMPL_ON_FUNC( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

    SendPacket( EGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 어둠의 문 던전의 페이즈가 변경되었다 해당 페이즈에 맞는 퀵슬롯 아이템으로 변경하자!
	KEGS_UPDATE_INVENTORY_SLOT_INFO_NOT kPacket;
	m_kInventory.SetupDefenceDungeonQuickSlotItems( kPacket_.m_iDefenceDungeonPhase, kPacket.m_vecInventorySlotInfo );
	SendPacket( EGS_UPDATE_INVENTORY_SLOT_INFO_NOT, kPacket );
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
IMPL_ON_FUNC( EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"현재 방에 입장한 상태가 아닙니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"현재 던전 방에 입장한 상태가 아닙니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	SendToCnRoom( ERM_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 파티 소속이라면 자동으로 이탈 처리 하자!

#ifdef LOG_PARTY_BREAK // 용의자1 여긴거같은데,..
	SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_10 );
#else
	SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
#endif // LOG_PARTY_BREAK 

	// 임시 인벤토리 초기화
	ClearTempInventory();

	// 방 초기화
	SetRoomUID( 0 );

	// 마을로 상태 변경
	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	// 마을로 이동!
	SendID( EGS_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT );
}
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
//}}

//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
IMPL_ON_FUNC_NOPARAM( EGS_SYNC_DUNGEON_TIMER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방 상태인데 RoomUID가 없네? 이상하다." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		return;
	}

	SendToCnRoom( ERM_SYNC_DUNGEON_TIMER_NOT, char() );
}

_IMPL_ON_FUNC( ERM_SYNC_DUNGEON_TIMER_NOT, KEGS_SYNC_DUNGEON_TIMER_NOT )
{
	SendPacket( EGS_SYNC_DUNGEON_TIMER_NOT, kPacket_ );	
}
#endif SERV_EVENT_VALENTINE_DUNGEON
//}


//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


