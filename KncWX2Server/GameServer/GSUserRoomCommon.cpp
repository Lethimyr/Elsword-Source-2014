#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "TrainingCenterTable.h"
#include "GameSysVal/GameSysVal.h"
//{{ 2008. 11. 24  최육사	방 리스트
#include "RoomListManager.h"
//}}

//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 2	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	#include "GSWorldMissionManager.h"
	#include "../Common/X2Data/XSLBuffManager.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}
//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	#include "X2Data/XSLEnchantItemManager.h"
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "GSGlobalMissionManager.h"
#endif SERV_GLOBAL_MISSION_MANAGER

#include "Enum/Enum.h"
#include "GSWorldMissionManager.h"

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_CREATE_ROOM_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
//#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_ROOM_REQ, EGS_CREATE_ROOM_ACK );
//#else
//	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}    

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_RoomInfo.m_RoomName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_RoomInfo.m_RoomName.c_str(), L'♡' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CREATE_ROOM_ACK kPacketAck;
	u_short usEventID;

	KERM_OPEN_ROOM_REQ kPacket;

	kPacket_.m_RoomInfo.m_uiRoomListID = GetRoomListID();

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"클라이언트에서 체험ID가 레벨20되었을때 유저를 캐릭선택으로 이동시켰을텐데 아직 게임을 하고있네?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
			return;
		}
	}

	// [참고] 특정던전일 경우(예:초심자숲) 파티에 속하지 않아도 시작할 수 있게 만들어야 할까?
	if( CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) )
	{
		START_LOG( cerr, L"튜토리얼 던전은 이 패킷으로 생성하려 하면안되는데?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_ROOM_30;
		SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
		return;
	}

	switch( kPacket_.m_RoomInfo.m_RoomType )
	{
	case CXSLRoom::RT_PVP:
		{
			usEventID = ERM_OPEN_PVP_ROOM_REQ;

			//{{ 2010. 12. 7	최육사	다른 채널 체크
#ifdef SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			if( GetPVPChannelClass( kPacket_.m_RoomInfo.m_iPVPChannelClass ) == false )
			{
				kPacketAck.m_iOK = NetError::ERR_ROOM_30;
				SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
				return;
			}
#else
			LIF( GetPVPChannelClass( kPacket_.m_RoomInfo.m_iPVPChannelClass ) );
#endif SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			//}}

			//{{ 2010. 7. 29  최육사	서버 코드 정리
			if( kPacket_.m_RoomInfo.m_iPVPChannelClass == KPVPChannelInfo::PCC_NONE )
			{
				START_LOG( cerr, L"대전 채널 정보가 이상합니다! 있을 수 없는 에러!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetRoomListID() )
					<< BUILD_LOG( GetRoomUID() )
					<< BUILD_LOG( GetStateIDString() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_ROOM_30;
				SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
				return;
			}
			//}}

			//{{ 2009. 10. 22  최육사	대전킬수 검사 - 연습채널은 킬수로 인한 게임종료가 없으므로 제외
			if( kPacket_.m_RoomInfo.m_iPVPChannelClass != KPVPChannelInfo::PCC_PLAY )
			{
				if( CXSLRoom::IsValidWinKillNum( kPacket_.m_RoomInfo.m_PVPGameType, kPacket_.m_RoomInfo.m_WinKillNum ) == false )
				{
					START_LOG( cerr, L"대전방 생성시 킬수세팅이 이상함." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOGc( kPacket_.m_RoomInfo.m_PVPGameType )
						<< BUILD_LOGc( kPacket_.m_RoomInfo.m_WinKillNum )
						<< END_LOG;

					kPacketAck.m_iOK = NetError::ERR_ROOM_47;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}

				// 플레이 시간 검사
				if( CXSLRoom::IsValidPvpPlayTime( kPacket_.m_RoomInfo.m_PVPGameType, static_cast<int>(kPacket_.m_RoomInfo.m_fPlayTime) ) == false )
				{
					START_LOG( cerr, L"대전방 생성시 플레이 타임 세팅이 이상함." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOGc( kPacket_.m_RoomInfo.m_PVPGameType )
						<< BUILD_LOG( kPacket_.m_RoomInfo.m_fPlayTime )
						<< END_LOG;

					kPacketAck.m_iOK = NetError::ERR_ROOM_30;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}
			}
			//}}

			//////////////////////////////////////////////////////////////////////////
			// [이벤트] (2010-07-29 ~ 2010-08-11) 대회채널 입장 조건
#ifdef SERV_TOURNAMENT_CONDITION
			if( kPacket_.m_RoomInfo.m_iPVPChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
			{				
#ifdef SERV_MAIN_TOURNAMENT
				// 오프라인 티켓 없으면 대회채널 못들어감.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_50;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}
#else
				// 오프라인 티켓 있으면 대홰채널 못들어감.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_39;
					SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
					return;
				}
#endif SERV_MAIN_TOURNAMENT
			}
#endif SERV_TOURNAMENT_CONDITION
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CXSLRoom::RT_DUNGEON:
		{
			START_LOG( cerr, L"잘못된 방 생성 패킷입니다." )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_ROOM_46;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
			return;
		}
		break;

	default: //룸타입이 이상할 경우
		{
			START_LOG( cerr, L"잘못된 룸 타입 에러.!" )
				<< BUILD_LOGc( kPacket_.m_RoomInfo.m_RoomType )
				<< END_LOG;

			KEGS_CREATE_ROOM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_ROOM_00;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );
		}
		return;
	}

	kPacket.m_bQuickJoin = false;
	kPacket.m_kRoomInfo = kPacket_.m_RoomInfo;

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, kPacket_.m_RoomInfo.m_RoomType );

	//옵저버 모드로 방 만들기 체크
	if( kPacket_.m_bObserver == true )
	{
		if( GetAuthLevel() >= SEnum::UAL_SUPERIOR )
		{
			kPacket.m_kRoomUserInfo.m_bIsObserver = true;
		}
		else
		{
			START_LOG( cerr, L"특수 유저 이하의 계정이 옵저버 방 만들기 요청" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )
				<< END_LOG;

			KEGS_CREATE_ROOM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_ROOM_35;
			SendPacket( EGS_CREATE_ROOM_ACK, kPacketAck );

			return;
		}
	}

	START_LOG( clog2, L"만든 사람, 게임 서버" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )		
		<< BUILD_LOGc( kPacket_.m_RoomInfo.m_PVPGameType )
		<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID );

	SendToCnRoom( usEventID, kPacket );
}

//{{ 2008. 12. 4  최육사
IMPL_ON_FUNC( EGS_CREATE_TUTORIAL_ROOM_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ), EGS_CREATE_TUTORIAL_ROOM_REQ, EGS_CREATE_TUTORIAL_ROOM_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	KEGS_CREATE_TUTORIAL_ROOM_ACK kPacketAck;
	u_short usEventID;

	KERM_OPEN_ROOM_REQ kPacket;

	kPacket_.m_RoomInfo.m_uiRoomListID = GetRoomListID();

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"클라이언트에서 체험ID가 레벨20되었을때 유저를 캐릭선택으로 이동시켰을텐데 아직 게임을 하고있네?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
			SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
			return;
		}
	}

	// [참고] 특정던전일 경우(예:초심자숲) 파티에 속하지 않아도 시작할 수 있게 만들어야 할까?
	if( !CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) )
	{
		START_LOG( cerr, L"일반 던전은 이 패킷으로 생성하려 하면안되는데?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_ROOM_30;
		SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
		return;
	}

	switch( kPacket_.m_RoomInfo.m_RoomType )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			usEventID = ERM_OPEN_DUNGEON_ROOM_REQ;

			//운영자 이상이 아니면 체크한다.
			if( GetAuthLevel() >= SEnum::UAL_GM )
			{
				START_LOG( cout, L"운영자 던전방 생성.!" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID )
					;
			}

#ifdef SERV_FIX_TUTORIAL_DUNGEON_GAME_INFO// 작업날짜: 2013-05-15	// 박세훈
			// 던전 플레이 정보를 저장
			m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_RoomInfo.m_iDungeonID, kPacket_.m_RoomInfo.m_DifficultyLevel, kPacket_.m_RoomInfo.m_cDungeonMode );
#endif // SERV_FIX_TUTORIAL_DUNGEON_GAME_INFO
		}
		break;

	default: //룸타입이 이상할 경우
		{
			START_LOG( cerr, L"잘못된 룸 타입 에러.!" )
				<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomType )
				<< END_LOG;

			KEGS_CREATE_TUTORIAL_ROOM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_ROOM_00;
			SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
		}
		return;
	}

	kPacket.m_bQuickJoin = false;
	kPacket.m_kRoomInfo = kPacket_.m_RoomInfo;

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, kPacket_.m_RoomInfo.m_RoomType );

	START_LOG( clog2, L"만든 사람, 게임 서버" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )
		<< BUILD_LOG( kPacket_.m_RoomInfo.m_iDungeonID );

	// 방 만들으러 가기전에 FSM부터 바꿔놓는다.
	if( GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		StateTransition( KGSFSM::I_TO_FIELD_MAP );
	}

	SendToCnRoom( usEventID, kPacket );
}
//}}

_IMPL_ON_FUNC( ERM_OPEN_ROOM_ACK, KEGS_CREATE_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

		if( GetRoomUID() != 0 )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
				//SendUpdateUnitState( CXSLUnit::CUS_PVP_WAIT );
				//SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_01 );
				break;

			case CXSLRoom::RT_DUNGEON:
				{
					//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>( kPacket_.m_RoomInfo.m_DifficultyLevel ), kPacket_.m_vecInventorySlotInfo ) )
					//{
					//    // 방 입장 실패 처리 - 센터서버로 ERM_LEAVE_ROOM_REQ를 날려서 방이탈 처리 함
					//    // 돌아오는 ACK는 KGSFSM에 걸려서 클라이언트로 전송되지 않는다.
					//    KERM_LEAVE_ROOM_REQ kPacketReq;
					//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_13;
					//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

					//    // 방 입장 실패 코드
					//    kPacket_.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_01;
					//    goto end_proc;
					//}
				}

				//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_WAIT );
				break;
			}
		}

		//{{ 2007. 11. 7  최육사  예외처리로 인해 소스코드 위치이동
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();
		//}}

		// 필드에서 이탈
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		if( CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID )  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
		{
			// 튜토리얼 던전의 경우 파티에서 이탈시킴
			SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_01 );
		}

		//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}
	}
	//else
	//{
	//	LOG_SUCCESS( kPacket_.m_iOK == NetError::ERR_ROOM_05 )
	//		<< BUILD_LOG( L"방생성 실패.!" )
	//		<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomType )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( GetCharName() )
	//		<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
	//		<< END_LOG;
	//}

	//end_proc:
	if( CXSLDungeon::IsSinglePlayDungeon( kPacket_.m_RoomInfo.m_iDungeonID )  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		KEGS_CREATE_TUTORIAL_ROOM_ACK kPacketAck;
		kPacketAck.m_iOK			 = kPacket_.m_iOK;
		kPacketAck.m_RoomInfo		 = kPacket_.m_RoomInfo;
		kPacketAck.m_vecSlot		 = kPacket_.m_vecSlot;
		kPacketAck.m_wstrCNIP		 = kPacket_.m_wstrCNIP;
		SendPacket( EGS_CREATE_TUTORIAL_ROOM_ACK, kPacketAck );
	}
	else
	{
		SendPacket( EGS_CREATE_ROOM_ACK, kPacket_ );
	}    
}

IMPL_ON_FUNC( EGS_JOIN_ROOM_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_ROOM_REQ, EGS_JOIN_ROOM_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KEGS_JOIN_ROOM_ACK kPacketAck;
	KERM_JOIN_ROOM_REQ kPacket;

	KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( kPacket_.m_iRoomUID );
	if( !spRoomInfo )
	{
		// 시간 차이로 방이 삭제된 경우이다.
		kPacketAck.m_iOK = NetError::ERR_ROOM_16;
		SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );

		START_LOG( cwarn, L"방이 삭제되어 join이 안되는 경우.!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacketAck.m_iOK ) )
			<< END_LOG;

		return;
	}

	// 잘못된 채널의 방에 입장 못하도록 예외처리
	if( spRoomInfo->m_uiRoomListID != GetRoomListID() )
	{
		// [참고] 시간 차이로 클라이언트의 방리스트에 삭제된 방이 남아있어 일어나는 현상
		kPacketAck.m_iOK = NetError::ERR_ROOM_16;
		SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
		return;
	}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-10-08	// 박세훈
	if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ROOM_53;	// 공식 대전에는 난입할 수 없습니다.
		SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"클라이언트에서 체험ID가 레벨20되었을때 유저를 캐릭선택으로 이동시켰을텐데 아직 게임을 하고있네?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
			SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
			return;
		}
	}

	//:: 던전에 진입할경우.
	switch( CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) )
	{
	case CXSLRoom::RT_PVP:
		{
			//{{ 2010. 12. 7	최육사	다른 채널 체크
#ifdef SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			int iPVPChannelClass = 0;
			if( GetPVPChannelClass( iPVPChannelClass ) == false )
			{
				// [참고] 시간 차이로 클라이언트의 방리스트에 삭제된 방이 남아있어 일어나는 현상
				kPacketAck.m_iOK = NetError::ERR_ROOM_16;
				SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
				return;
			}
#else
			int iPVPChannelClass = 0;
			LIF( GetPVPChannelClass( iPVPChannelClass ) );
#endif SERV_WRONG_PVP_CHANNEL_JOIN_FIX
			//}}

			//////////////////////////////////////////////////////////////////////////
			// [이벤트] (2010-07-29 ~ 2010-08-11) 대회채널 입장 조건
#ifdef SERV_TOURNAMENT_CONDITION
			if( iPVPChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
			{
#ifdef SERV_MAIN_TOURNAMENT
				// 오프라인 티켓 없으면 대회채널 못들어감.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_50;
					SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
					return;
				}
#else
				// 오프라인 티켓 있으면 대회채널 못들어감.
				if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_39;
					SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
					return;
				}
#endif SERV_MAIN_TOURNAMENT				
			}
#endif SERV_TOURNAMENT_CONDITION
			//////////////////////////////////////////////////////////////////////////
		}
		break;

	case CXSLRoom::RT_DUNGEON:
		{
			START_LOG( cerr, L"잘못된 방 입장 패킷입니다." )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_ROOM_46;
			SendPacket( EGS_JOIN_ROOM_ACK, kPacketAck );
			return;
		}
		break;

	default:
		break;
	}

	kPacket.m_bQuickJoin = false;
	kPacket.m_wstrPassword = kPacket_.m_wstrPassword;
#ifdef SERV_NEW_PVPROOM_PROCESS
	kPacket.m_bInvitation = kPacket_.m_bInvitation;
#endif SERV_NEW_PVPROOM_PROCESS

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) );

	//{{ 2012. 12. 5	최육사		옵저버 대전 난입 모드
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
	if( GetAuthLevel() >= SEnum::UAL_GM  &&  m_kUserCheatManager.IsObserverMode() == true )
	{
		START_LOG( cout, L"운영자가 옵저버 모드로 방에 난입합니다!" )
			<< BUILD_LOGc( GetAuthLevel() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() );

		kPacket.m_kRoomUserInfo.m_bIsObserver = true;
	}
#else
	kPacket.m_kRoomUserInfo.m_bIsObserver = kPacket_.m_bIsObserver;
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}

	_SendToCnRoom( kPacket_.m_iRoomUID, ERM_JOIN_ROOM_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_JOIN_ROOM_ACK, KEGS_JOIN_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

		if( GetRoomUID() != 0 )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
				//SendUpdateUnitState( CXSLUnit::CUS_PVP_WAIT );
				break;

			case CXSLRoom::RT_DUNGEON:
				{
					//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>( kPacket_.m_RoomInfo.m_DifficultyLevel ), kPacket_.m_vecInventorySlotInfo ) )
					//{
					//    // 방 입장 실패 처리 - 센터서버로 ERM_LEAVE_ROOM_REQ를 날려서 방이탈 처리 함
					//    // 돌아오는 ACK는 KGSFSM에 걸려서 클라이언트로 전송되지 않는다.
					//    KERM_LEAVE_ROOM_REQ kPacketReq;
					//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_13;
					//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

					//    // 방 입장 실패 코드
					//    kPacket_.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_01;
					//    goto end_proc;
					//}
				}
				//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_WAIT );
				break;
			}
		}

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
	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword );
	}

	//end_proc:
	SendPacket( EGS_JOIN_ROOM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_JOIN_ROOM_NOT, KEGS_JOIN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_JOIN_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_QUICK_JOIN_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_QUICK_JOIN_REQ, EGS_QUICK_JOIN_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	UidType iRoomUID = SiKRoomListManager()->GetWaitingRoomUID( GetRoomListID(), kPacket_ );

	KEGS_QUICK_JOIN_ACK kPacketAck;

	if( iRoomUID > 0 )
	{
		KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( iRoomUID );
		if( !spRoomInfo )
		{
			// 시간 차이로 방이 삭제된 경우이다.
			kPacketAck.m_iOK = NetError::ERR_ROOM_16;
			SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );

			START_LOG( cerr, L"방이 삭제되어 join이 안되는 경우.!" )
				<< BUILD_LOG( NetError::GetErrStr( kPacketAck.m_iOK ) )
				<< END_LOG
				;

			return;
		}

		// 체험ID 기능 제한
		if( IsGuestUser() )
		{
			if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
			{
				START_LOG( cerr, L"클라이언트에서 체험ID가 레벨20되었을때 유저를 캐릭선택으로 이동시켰을텐데 아직 게임을 하고있네?" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_GUEST_USER_02;
				SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
				return;
			}
		}

		KERM_JOIN_ROOM_REQ kPacket;

		switch( CXSLRoom::GetRoomType( iRoomUID ) )
		{
		case CXSLRoom::RT_PVP:
			{
#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-10-08	// 박세훈
				if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
				{
					kPacketAck.m_iOK = NetError::ERR_ROOM_53;	// 공식 대전에는 난입할 수 없습니다.
					SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
					return;
				}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

				//////////////////////////////////////////////////////////////////////////
				// [이벤트] (2010-07-29 ~ 2010-08-11) 대회채널 입장 조건
#ifdef SERV_TOURNAMENT_CONDITION
				int iPVPChannelClass = 0;
				LIF( GetPVPChannelClass( iPVPChannelClass ) );
				if( iPVPChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
				{
#ifdef SERV_MAIN_TOURNAMENT
					// 오프라인 티켓 없으면 대회채널 못들어감.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
					{
						kPacketAck.m_iOK = NetError::ERR_ROOM_50;
						SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
						return;
					}
#else
					// 오프라인 티켓 있으면 대회채널 못들어감.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
					{
						kPacketAck.m_iOK = NetError::ERR_ROOM_39;
						SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
						return;
					}
#endif SERV_MAIN_TOURNAMENT
				}
#endif SERV_TOURNAMENT_CONDITION
				//////////////////////////////////////////////////////////////////////////
			}
			break;

		case CXSLRoom::RT_DUNGEON:
			{
				START_LOG( cerr, L"잘못된 퀵조인 패킷입니다." )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacketAck.m_iOK = NetError::ERR_ROOM_46;
				SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
				return;
			}
			break;

		default:
			break;
		}

		kPacket.m_bQuickJoin = true;
#ifdef SERV_NEW_PVPROOM_PROCESS
		kPacket.m_bInvitation = false;
#endif SERV_NEW_PVPROOM_PROCESS
		GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::GetRoomType( iRoomUID ) );

		_SendToCnRoom( iRoomUID, ERM_JOIN_ROOM_REQ, kPacket );
	}
	else
	{
		kPacketAck.m_iOK = NetError::ERR_ROOM_07;
		SendPacket( EGS_QUICK_JOIN_ACK, kPacketAck );
	}
}

_IMPL_ON_FUNC( ERM_QUICK_JOIN_ACK, KEGS_JOIN_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

		switch(	CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>( kPacket_.m_RoomInfo.m_DifficultyLevel ), kPacket_.m_vecInventorySlotInfo ) )
				//{
				//    // 방 입장 실패 처리 - 센터서버로 ERM_LEAVE_ROOM_REQ를 날려서 방이탈 처리 함
				//    // 돌아오는 ACK는 KGSFSM에 걸려서 클라이언트로 전송되지 않는다.
				//    KERM_LEAVE_ROOM_REQ kPacketReq;
				//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_13;
				//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

				//    // 방 입장 실패 코드
				//    kPacket_.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_01;
				//    goto end_proc;
				//}
			}
			break;

		default:
			break;
		}

		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();

		// 필드에서 이탈
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}
	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_wstrPassword );
	}

	//end_proc:
	SendPacket( EGS_QUICK_JOIN_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_LEAVE_ROOM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_ROOM_REQ, EGS_LEAVE_ROOM_ACK );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	u_short usEventID;

	//훈련소의 통계데이터를 받기위해 패킷을 기능별로 분활함.
	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_TRAININGCENTER:
		usEventID = ERM_LEAVE_TC_ROOM_REQ;
		break;

		//{{ 2010. 03. 17  최육사	상태 오류 수정		
	case CXSLRoom::RT_SQUARE:
		{
			START_LOG( cerr, L"방 이탈인데 유저는 광장에 있네? 일어나서는 안되는 오류" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_LEAVE_SQUARE_REQ kPacket;
			kPacket.m_iReason = kPacket_.m_iReason;
			SendToCnRoom( ERM_LEAVE_SQUARE_REQ, kPacket );
			return;
		}
		break;
		//}}

	default:
		usEventID = ERM_LEAVE_ROOM_REQ;
		break;
	}

	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = kPacket_.m_iReason;
	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	kPacket.m_kResultInfo = kPacket_.m_kResultInfo;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	kPacket.m_kGamePlayNetworkInfo = kPacket_.m_kGamePlayNetworkInfo;
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

	SendToCnRoom( usEventID, kPacket );
}

//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
_IMPL_ON_FUNC( ERM_LEAVE_ROOM_ACK, KEGS_LEAVE_ROOM_ACK )
//#else
//_IMPL_ON_FUNC( ERM_LEAVE_ROOM_ACK, KPacketOK )
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}
{
	//{{ 2010. 03. 17  최육사	상태 오류 수정
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_ROOM_ACK );
	//}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	// 던전방에서 나갈경우 파티 탈퇴.
	const CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	//{{ 2012. 04. 16	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< int > vecDeleteBuff;
	std::vector< KBuffInfo > vecActivateBuff;

	switch( eRoomType )
	{
#ifdef SERV_FIX_REVENGE_BUFF// 작업날짜: 2013-08-09	// 박세훈
	case CXSLRoom::RT_PVP:
		m_kUserBuffManager.OnLeavePVPRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
		break;
#endif // SERV_FIX_REVENGE_BUFF

	case CXSLRoom::RT_DUNGEON:
		{
			//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
			bool bBadAttitudeUser = false;

			if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_34 )
			{
				bBadAttitudeUser = true;
				kPacket_.m_iReason = NetError::NET_OK;
			}

			m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff, bBadAttitudeUser );
#else
			m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecDeleteBuff );

			if( vecDeleteBuff.empty() == false )
			{
				// 로그인 서버 버프 매니저에 업데이트
				KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
				kReq.m_iUnitUID = GetCharUID();
				kReq.m_vecDeActivateBuff = vecDeleteBuff;
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
			//}
		}
		break;

	case CXSLRoom::RT_BATTLE_FIELD:
		m_kUserBuffManager.OnLeaveBattleField( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
		break;
	}

	if( vecDeleteBuff.empty() == false || vecActivateBuff.empty() == false )
	{
		// 로그인 서버 버프 매니저에 업데이트
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		kReq.m_vecActivateBuff = vecActivateBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	bool bIsSuccess = false;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		bIsSuccess = true;
	}
	else
	{
		if( GetRoomUID() != 0  &&  kPacket_.m_iOK == NetError::ERR_ROOM_34 )
			bIsSuccess = true;
	}

	if( bIsSuccess == true )
	{
		SetRoomUID( 0 );
		m_kUserDungeonManager.SetDungeonGameInfo( 0, 0, 0 );

		StateTransition( KGSFSM::I_TO_FIELD_MAP );

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );

		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
		}
	}

	//{{ 2013. 2. 7	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	if( ( eRoomType == CXSLRoom::RT_PVP ) ||
		( eRoomType == CXSLRoom::RT_DUNGEON )
		)
#else
	if( eRoomType == CXSLRoom::RT_DUNGEON )
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
		//}}
	{
		//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
		if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_22 )
		{
			SendLeaveParty( NetError::NOT_LEAVE_ROOM_REASON_22 );
		}
		else
		{
			SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
		}
//#else
//		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
		//}}
	}

	SendPacket( EGS_LEAVE_ROOM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_ROOM_NOT, KEGS_LEAVE_ROOM_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_BAN_USER_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
//#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_BAN_USER_REQ, EGS_BAN_USER_ACK, KPacketOK );
//#else
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iUnitUID );

	if( kPacket_.m_iUnitUID != GetCharUID() )
	{
		SendToCnRoom( ERM_BAN_USER_REQ, kPacket_ );
	}
	//자기자신을 강퇴 시킬수 없다.
	else
	{
		KPacketOK kPacket;
		kPacket.m_iOK = NetError::ERR_ROOM_26;
		SendPacket( EGS_BAN_USER_ACK, kPacket );
	}
}

_IMPL_ON_FUNC( ERM_BAN_USER_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_BAN_USER_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_BAN_USER_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	//{{ 2010. 8. 18	최육사	방에서 강퇴되었을때도 임시인벤 비우기
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		ClearTempInventory();
	}
	//}}

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_20;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}

	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendID( EGS_BAN_USER_NOT );
}

IMPL_ON_FUNC( EGS_STATE_CHANGE_GAME_START_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_STATE_CHANGE_GAME_START_REQ, EGS_STATE_CHANGE_GAME_START_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	START_LOG_WITH_NAME( clog );

	KERM_GAME_START_REQ kPacket;
	kPacket.m_sWorldID = 0;

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				// 근성도 검사 - 이 핸들러를 사용하는 던전이라면 튜토리얼인가..
				//if( CheckingSpirit( 0 ) == false )
				//{
				//	KEGS_STATE_CHANGE_GAME_START_ACK kPacketAck;
				//	kPacketAck.m_iOK = NetError::ERR_ROOM_27;
				//	SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacketAck );
				//	return;
				//}
			}
			break;

		case CXSLRoom::RT_PVP:
			{
				kPacket.m_sWorldID = kPacket_.m_sWorldID;

				//{{ 2010. 7. 28  최육사	대회 채널 예외 처리
#ifdef SERV_TOURNAMENT_CONDITION
				int iPvpChannelClass = 0;
				LIF( GetPVPChannelClass( iPvpChannelClass ) );
				if( iPvpChannelClass == KPVPChannelInfo::PCC_NONE )
				{
					START_LOG( cerr, L"방리스트에서 방을 찾을 수 없습니다." )
						<< BUILD_LOG( GetRoomUID() )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_ROOM_31;
					SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
					return;
				}
				else if( iPvpChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
				{
#ifdef SERV_MAIN_TOURNAMENT
					// 오프라인 티켓 없으면 대회채널 못들어감.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
					{
						KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_ROOM_50;
						SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
						return;
					}
#else
					// 오프라인 티켓 있으면 대회채널 못들어감.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
					{
						KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_ROOM_39;
						SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
						return;
					}
#endif SERV_MAIN_TOURNAMENT
				}
#endif SERV_TOURNAMENT_CONDITION
				//}}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"방에 입장해있는데 RoomUID가 없다?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_GAME_START_REQ, kPacket );
}

IMPL_ON_FUNC( EGS_GAME_LOADING_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iLoadingProgress );

	if( kPacket_.m_iLoadingProgress >= 100 )		// 로딩이 완료 된 시점 - 게임 시작 직전
	{
	//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
		kPacket_.m_iUnitLevel = static_cast<int>( GetLevel() );
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS_TEMP// 작업날짜: 2013-06-12	// 박세훈
		SendUpdateBuffInfo();
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS_TEMP
	}

	//{{ 2011. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		SendToCnRoom( ERM_BATTLE_FIELD_GAME_LOADING_REQ, kPacket_ );
	}
	else
	{
		SendToCnRoom( ERM_GAME_LOADING_REQ, kPacket_ );
	}
#else
	SendToCnRoom( ERM_GAME_LOADING_REQ, kPacket_ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
}

IMPL_ON_FUNC( ERM_GAME_START_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_STATE_CHANGE_GAME_START_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_STATE_CHANGE_GAME_START_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_GAME_START_NOT )//, KEGS_STATE_CHANGE_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_STATE_CHANGE_GAME_START_NOT kPacket;
	kPacket.m_cRoomState	= kPacket_.m_cRoomState;
	kPacket.m_iGameType		= kPacket_.m_iGameType;	
	kPacket.m_vecSlot		= kPacket_.m_vecSlot;
	kPacket.m_sWorldID		= kPacket_.m_sWorldID;

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
			{
				//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_PVP_PLAY_COUNT );

				const int iPvpPlayCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_PVP_PLAY_COUNT );
				if( iPvpPlayCount == 1 )
				{
					WriteCharacterLogToDB( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_PVP );
				}
#endif SERV_CHAR_LOG
				//}}

				//SendUpdateUnitState( CXSLUnit::CUS_PVP_PLAY );
			}			
			break;

		case CXSLRoom::RT_DUNGEON:
			{
#ifdef SERV_EVENT_CHECK_POWER
				IF_EVENT_ENABLED( CEI_CHECK_POWER )
				{
					UpdateCheckPowerScore( kPacket_.m_iDungeonID, CXSLDungeon::RT_F, 1, false, CXSLDungeon::DL_NORMAL, false, false );
				}
#endif SERV_EVENT_CHECK_POWER

				//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
				m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );

				const int iDungeonPlayCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );
				if( iDungeonPlayCount == 1 )
				{
					WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_DUNGEON_PLAY, kPacket_.m_iDungeonID );
				}
#endif SERV_CHAR_LOG
				//}}

				//if( !DeleteRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID, kPacket.m_vecInventorySlotInfo ) )
				//if( !CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID ) )
				//{
				//	//우선 게임시작시 아이템이 없으면 오류니 로그를 남겨본다..
				//	START_LOG( clog, L"던전게임 시작시 필요아이템이 없음.!" )
				//		<< BUILD_LOG( GetCharName() )
				//		<< BUILD_LOG( kPacket_.m_iDungeonID )
				//		<< END_LOG;

				//    // 방 입장 실패 처리 - 센터서버로 ERM_LEAVE_ROOM_REQ를 날려서 방이탈 처리 함
				//    // 돌아오는 ACK는 KGSFSM에 걸려서 클라이언트로 전송되지 않는다.
				//    KERM_LEAVE_ROOM_REQ kPacketReq;
				//    kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
				//    SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );

				//    return;
				//}

				//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_PLAY );
			}
			break;
		}
	}

	// 근성도 보냄
	//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kPacket.m_iSpirit = m_kUserSpiritManager.GetSpirit();
#else
	kPacket.m_iSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_STATE_CHANGE_GAME_START_NOT, kPacket );
}

_IMPL_ON_FUNC( ERM_GAME_LOADING_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GAME_LOADING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GAME_LOADING_NOT, KEGS_GAME_LOADING_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GAME_LOADING_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_GAME_LOADING_TIME_OUT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	//{{ 2010. 11. 23	최육사	방 로딩 오류 수정
#ifdef SERV_ROOM_LOADING_BUG_FIX
	// 던전방에서 나갈경우 파티 탈퇴.
	CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	if( eRoomType == CXSLRoom::RT_DUNGEON )
	{
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );
	}
#endif SERV_ROOM_LOADING_BUG_FIX
	//}}

	//{{ 2010. 12. 30	최육사	방이탈 예외처리
#ifdef SERV_LEAVE_ROOM_BUG_FIX
	SetRoomUID( 0 );
#endif SERV_LEAVE_ROOM_BUG_FIX
	//}}

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendID( EGS_GAME_LOADING_TIME_OUT_NOT );
}

IMPL_ON_FUNC_NOPARAM( ERM_GAME_LOADING_ALL_UNIT_OK_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendID( EGS_GAME_LOADING_ALL_UNIT_OK_NOT );
}

_IMPL_ON_FUNC( ERM_PLAY_START_NOT, KEGS_PLAY_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );

				const int iDungeonPlayCount = m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_DUNGEON_PLAY_COUNT );
				if( iDungeonPlayCount == 1 )
				{
					WriteCharacterLogToDBWithDungeonID( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_DUNGEON_PLAY, kPacket_.m_iDungeonID );
				}

				//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
				// 유저에게 알려주자
				KEGS_HENIR_REWARD_COUNT_NOT	kPacket;
				m_kUserDungeonManager.GetHenirRewardCountInfo( kPacket );

				SendPacket( EGS_HENIR_REWARD_COUNT_NOT, kPacket );

				START_LOG( clog, L"유저에게 보상 획득 횟수 갱신!!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventUnLimited() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
					<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
					<< END_LOG;
#endif SERV_NEW_HENIR_TEST
				//}}

#ifdef SERV_COEXISTENCE_FESTIVAL// 작업날짜: 2013-05-14	// 박세훈
				if( ( CXSLDungeon::IsTutorialDungeon( GetDungeonIDAndDungeonDifficulty() ) == false ) &&
					( CXSLDungeon::IsHenirDungeon( GetDungeonIDAndDungeonDifficulty() ) == false )
					)
				{
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
				}
#endif // SERV_COEXISTENCE_FESTIVAL
			}break;
		}
	}	
#endif SERV_CHAR_LOG
	//}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	m_bTimeControlItemCheckDungeonPlay = true;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
#ifdef SERV_SERVER_BUFF_SYSTEM// 작업날짜: 2013-06-08	// 박세훈
#else // SERV_SERVER_BUFF_SYSTEM
	//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	KRoomUserInfo kRoomUserInfo;
	KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT kPacket;
	const int iRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	kPacket.m_iUnitUID = GetCharUID();
	m_kUserBuffManager.GetBuffList( kPacket.m_vecActivateBuffList, iRoomType );
	GetRoomUserInfo( kRoomUserInfo, iRoomType );
	kPacket.m_kGameStat = kRoomUserInfo.m_kGameStat;
	SendToCnRoom( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
#endif // SERV_SERVER_BUFF_SYSTEM

	SendPacket( EGS_PLAY_START_NOT, kPacket_ );

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	m_kUserQuestManager.CheckEpicQuest_EnterDungeon( kPacket_.m_iDungeonID, GetThisPtr<KGSUser>() );
	m_kUserQuestManager.Handler_OnEnterTheDungeon( GetThisPtr<KGSUser>() );
#endif SERV_REFORM_QUEST
	//}}

}

IMPL_ON_FUNC( EGS_END_GAME_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_END_GAME_REQ, EGS_END_GAME_ACK, KPacketOK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	SendToCnRoom( ERM_END_GAME_REQ, kPacket_ );

	//{{ 2009. 2. 4  최육사		던전 리플레이
	if( GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				// 파티에 속해있을때만 체크
				if( GetPartyUID() > 0 )
				{
					//{{ 2010. 02. 23  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
					if( SiKPartyListManager()->E_EndGame( GetThisPtr<KGSUser>() ) == false )
#else
#pragma ELSE_MESSAGE
					//if( SiKPartyManager()->E_EndGame( GetPartyUID(), GetThisPtr<KGSUser>() ) == false )
#endif SERV_CHANNEL_PARTY
					//}}					
					{
						START_LOG( cerr, L"게임 종료에 따른 파티유저정보 업데이트 실패!" )
							<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
							<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
							<< BUILD_LOG( GetPartyUID() )
							<< END_LOG;
					}
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"End Game을 보내왔는데 RoomUID값이 이상하다?" )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
	//}}
}

_IMPL_ON_FUNC( ERM_END_GAME_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_END_GAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_END_GAME_NOT, KEGS_END_GAME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	SendPacket( EGS_END_GAME_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_STATE_CHANGE_RESULT_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_STATE_CHANGE_RESULT_REQ, EGS_STATE_CHANGE_RESULT_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}


	SendToCnRoom( ERM_STATE_CHANGE_RESULT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_RESULT_ACK, KEGS_STATE_CHANGE_RESULT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_STATE_CHANGE_RESULT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_RESULT_NOT, KEGS_STATE_CHANGE_RESULT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
			//SendUpdateUnitState( CXSLUnit::CUS_PVP_WAIT );
			break;
		case CXSLRoom::RT_DUNGEON:			
			//SendUpdateUnitState( CXSLUnit::CUS_DUNGEON_WAIT );
			break;
		}
	}

	SendPacket( EGS_STATE_CHANGE_RESULT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_USER_UNIT_RE_BIRTH_POS_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_USER_UNIT_RE_BIRTH_POS_REQ, EGS_USER_UNIT_RE_BIRTH_POS_ACK, KPacketOK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	SendToCnRoom( ERM_USER_UNIT_RE_BIRTH_POS_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_RE_BIRTH_POS_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_RE_BIRTH_POS_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_RE_BIRTH_NOT, KEGS_USER_UNIT_RE_BIRTH_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_RE_BIRTH_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_ROOM_LIST_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_ROOM_LIST_ACK );

	KEGS_ROOM_LIST_ACK kPacket;

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-10-08	// 박세훈
	if( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() )
	{
		START_LOG( cerr, L"공식 대전 방 리스트를 왜 요청하지?? 뭔가 꼬였나?" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_ROOM_LIST_ACK, kPacket );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// RoomListID값이 유효할때만 방리스트를 준다
	if( GetRoomListID() > 0 )
	{
		if( SiKRoomListManager()->GetRoomInfoPage( GetRoomListID(), kPacket_, kPacket ) == false )
		{
			START_LOG_WITH_NAME( cerr )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_nViewPage )
				<< END_LOG;

			//06.12.08 특정오류체크가 없음.
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendPacket( EGS_ROOM_LIST_ACK, kPacket );
			return;
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_ROOM_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_USER_UNIT_DIE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_USER_UNIT_DIE_REQ, kPacket_ );
}

//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
IMPL_ON_FUNC( ERM_USER_UNIT_DIE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_USER_UNIT_DIE_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	kAck.m_fReBirthTime = kPacket_.m_fReBirthTime;
	kAck.m_EXP = kPacket_.m_EXP;
	kAck.m_VP = kPacket_.m_VP;

	//{{ 2012. 04. 23	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::vector< int > vecDeactivatedBuff;
	m_kUserBuffManager.OnUserDied( GetThisPtr<KGSUser>(), vecDeactivatedBuff );
	if( vecDeactivatedBuff.empty() == false )
	{
		// 로그인 서버 버프 매니저에 업데이트
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecDeActivateBuff = vecDeactivatedBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	SendPacket( EGS_USER_UNIT_DIE_ACK, kAck );

	m_kUserTitleManager.OnUserUnitDie( static_cast< KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON >(kPacket_.m_ucDieReason), GetThisPtr<KGSUser>() );
}
#endif SERV_ADD_TITLE_CONDITION
//}}

IMPL_ON_FUNC( ERM_USER_UNIT_DIE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_USER_UNIT_DIE_NOT kPacket;
	kPacket.m_KillerNPCUID					= kPacket_.m_KillerNPCUID;
	kPacket.m_KillerUserUnitUID				= kPacket_.m_KillerUserUnitUID;
	kPacket.m_MaxDamageKillerUserUnitUID	= kPacket_.m_MaxDamageKillerUserUnitUID;
	kPacket.m_KilledUserUnitUID				= kPacket_.m_KilledUserUnitUID;

	SendPacket( EGS_USER_UNIT_DIE_NOT, kPacket );

	if( GetCharUID() == kPacket_.m_KillerUserUnitUID || GetCharUID() == kPacket_.m_MaxDamageKillerUserUnitUID )
	{
		m_kUserQuestManager.Handler_OnPVPKill( kPacket_.m_iGameType
											 , GetThisPtr<KGSUser>()
											 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
											 , kPacket_.m_bOfficialMatch
#endif SERV_NEW_PVP_QUEST
											 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
											 , kPacket_.m_bHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
											, kPacket_.m_killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
											, kPacket_.m_killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
											 );
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_USER_UNIT_DIE_COMPLETE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_USER_UNIT_DIE_COMPLETE_REQ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_COMPLETE_ACK, KEGS_USER_UNIT_DIE_COMPLETE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_DIE_COMPLETE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_COMPLETE_NOT, KEGS_USER_UNIT_DIE_COMPLETE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USER_UNIT_DIE_COMPLETE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_REMAINING_PLAY_TIME_NOT, float )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_REMAINING_PLAY_TIME_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_PLAY_TIME_OUT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendID( EGS_PLAY_TIME_OUT_NOT );
}

IMPL_ON_FUNC( EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ )
{
	//{{ 2010. 9. 8	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	_VERIFY_STATE_REPEAT_FILTER( cerr, ( 1, KGSFSM::S_ROOM ), EGS_MY_USER_UNIT_INFO_TO_SERVER_REQ, EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK, KPacketOK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KERM_MY_USER_UNIT_INFO_TO_SERVER_REQ kPacket;
	//{{ 2011. 01. 21	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
	kPacket.m_kDungeonUnitInfo = kPacket_.m_kMyPlayResult;
#else
	kPacket.m_kDungeonUnitInfo.m_fHP			= kPacket_.m_fHP;
	kPacket.m_kDungeonUnitInfo.m_TotalScore		= kPacket_.m_TotalScore;
	kPacket.m_kDungeonUnitInfo.m_ComboScore		= kPacket_.m_ComboScore;
	kPacket.m_kDungeonUnitInfo.m_TechScore		= kPacket_.m_TechScore;
	kPacket.m_kDungeonUnitInfo.m_nDamageCount	= kPacket_.m_nDamageCount;
#endif SERV_DUNGEON_RANK_NEW
	//}}

	//{{ 2010. 05. 12  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	GetRoomUserInfo( kPacket.m_kRoomUserInfo );
#endif SERV_INTEGRATION
	//}}

	SendToCnRoom( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_NPC_UNIT_CREATE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_NPC_UNIT_CREATE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_ACK, KPacketOK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_NPC_UNIT_CREATE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_NOT, KEGS_NPC_UNIT_CREATE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 현재 입장한 배틀필드에 맞는 몬스터 출현 정보인지 체크!
	if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		if( kPacket_.m_iBattleFieldID != GetMapID() )
			return;
	}

	SendPacket( EGS_NPC_UNIT_CREATE_NOT, kPacket_ );

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	std::vector< int > vecNPCData;
	BOOST_TEST_FOREACH( KNPCUnitNot, kNPCData, kPacket_.m_vecNPCUnitAck )
	{
		int iNPCID = 0;
		iNPCID = kNPCData.m_kNPCUnitReq.m_NPCID;
		vecNPCData.push_back( iNPCID );
	}

	if( GetRoomUID() != 0  &&  ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, false );
	}
	else if( GetRoomUID() != 0  && ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, true );
	}
#endif SERV_REFORM_QUEST
	//}}
}

IMPL_ON_FUNC( EGS_NPC_UNIT_DIE_REQ )
{
	// [참고] npc유닛이 자살할때 어떤식으로 처리할지에 대해서 좀더 좋은 방법을 찾아야 할듯..

	// 현재 npc유닛이 자살할 경우 DieNPCUID에 해당 npcuid는 보내고 있는 상황, 따라서 droptable에 드롭처리 되어있으면 던전에서 템드롭이 됨. (대전은 상관없음)

	if( kPacket_.m_nDieNPCUID == -1 )
	{
		START_LOG( clog, L"삭제되는 유닛 추후 클라이언트 루틴수정" );

		return;
	}
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2012. 12. 07	최육사	배틀필드 시스템
#ifdef SERV_CODE_CLEANUP_2012_12_07
	// [2012-12-07][최육사] RoomUID값이 없는데 굳이 패킷을 센터서버로 보낼 필요가 없다.
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"S_ROOM상태인데 RoomUID값은 저장되어있지 않다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;
		return;
	}
#endif SERV_CODE_CLEANUP_2012_12_07
	//}}

	SendToCnRoom( ERM_NPC_UNIT_DIE_REQ, kPacket_ );
}

IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KERM_NPC_UNIT_DIE_ACK kPacket;
	kPacket.m_iOK	= kPacket_.m_iOK;
	SendPacket( EGS_NPC_UNIT_DIE_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_NPC_UNIT_DIE_NOT kPacketNpcDie;
	kPacketNpcDie.m_cAttUnitType	= kPacket_.m_cAttUnitType;
	kPacketNpcDie.m_EXPList			= kPacket_.m_EXPList;
	kPacketNpcDie.m_iNPCID			= kPacket_.m_iNPCID;
	kPacketNpcDie.m_nDieNPCUID		= kPacket_.m_nDieNPCUID;
	kPacketNpcDie.m_uiAttUnit		= kPacket_.m_uiAttUnit;

	//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( kPacket_.m_bIsPvpNpc )
	{
		m_kUserTitleManager.OnNpcUnitDie( 0, 0, 0, kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );

		//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		m_kUserQuestManager.Handler_OnPVPKill( 
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
											   kPacket_.m_iGameType
											   , GetThisPtr<KGSUser>()
											   , kPacket_.m_bIsPvpNpc	//{{ 2012. 02. 22	김민성	대전 퀘스트, PVP NPC 관련 퀘스트	오류 수정
#else
											   , CXSLRoom::PVP_GAME_TYPE::PGT_NULL
											   , GetThisPtr<KGSUser>()
											   , true						//{{ 2012. 02. 22	김민성	대전 퀘스트, PVP NPC 관련 퀘스트	오류 수정
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
#ifdef PVP_QUEST_HERO_KILL_COUNT
											   , kPacket_.m_bHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
											   , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
											   , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
											   );
#endif SERV_NEW_PVP_QUEST
		//}}
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 퀘스트
	{
		m_kUserQuestManager.Handler_ERM_NPC_UNIT_DIE_NOT( kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_iNPCID, GetThisPtr<KGSUser>(), kPacket_.m_cDungeonMode );
	}

	//////////////////////////////////////////////////////////////////////////
	// 칭호 미션
	{
		m_kUserTitleManager.OnNpcUnitDie( kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );
	}

	// 1. NoDrop이면 아무 처리도 안함
	if( kPacket_.m_bNoDrop )
	{
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
		return;
	}

	// 2. 내가 받을 경험치가 있남?
	std::map< UidType, KEXPData >::iterator mit = kPacketNpcDie.m_EXPList.find( GetCharUID() );
	if( mit == kPacketNpcDie.m_EXPList.end() )
	{
		// 보상받을 경험치가 없다면 바로 클라이언트로 보냄
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
	}
	else
	{
		float fAddPartyFeverExpRate = 0.0f;
		
		if( SiKPartyListManager()->GetPartyFeverExpRate( GetPartyUID(), fAddPartyFeverExpRate ) == true )
		{
			int iPartyFeverEXP = static_cast<int>( (mit->second.m_iEXP) * fAddPartyFeverExpRate );
			iPartyFeverEXP += static_cast<int>( (mit->second.m_iPartyEXP) * fAddPartyFeverExpRate );

			// party fever 보너스 경험치는 소켓옵션 보너스에 더한다
			mit->second.m_iSocketOptEXP += iPartyFeverEXP;
		}

#ifdef SERV_ANTI_ADDICTION_SYSTEM
		// 개선필요
		ApplyAntiAddictionRatio(kPacketNpcDie);
#endif SERV_ANTI_ADDICTION_SYSTEM

		// party fever보너스까지 처리완료된뒤 클라이언트로 보냄
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );

		// 경험치 보상
		//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		m_kEXP.AddExp( static_cast<int>(mit->second.SumEXP() * m_AntiAddictionManager.GetRewardRatio()) );

		START_LOG( clog2, L"ERM_NPC_UNIT_DIE_NOT 방침미 보상률 적용." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_AntiAddictionManager.GetRewardRatio() )
			<< END_LOG;
#else
		m_kEXP.AddExp( mit->second.SumEXP() );
#endif SERV_ANTI_ADDICTION_SYSTEM

		// 레벨업 체크
		CheckCharLevelUp();
	}

	//////////////////////////////////////////////////////////////////////////
	// 내구도 소모
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true )
	{
		KEGS_DECREASE_ENDURANCE_NOT kNot;
		std::vector< KInventoryItemInfo > vecZeroEndurance;

		// ItemTemplet에 설정된 내구도에서 경험치 주는 npc수 만큼 나눈값을 깎는다
		m_kInventory.DecreaseEquippedItemEnduranceInDungeon( kPacket_.m_sSubStageExpNpcNum, kPacket_.m_cUserCount, kNot.m_vecEnduranceUpdate, vecZeroEndurance );

		// 내구도가 소모되었다면 클라이언트에 업데이트한다
		if( !kNot.m_vecEnduranceUpdate.empty() )
		{
			SendPacket( EGS_DECREASE_ENDURANCE_NOT, kNot );

			// 내구도 모두 소모된 아이템이 있다면
			if( GetRoomUID() > 0  &&  vecZeroEndurance.empty() == false )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_DUNGEON:
				case CXSLRoom::RT_TRAININGCENTER:
					{
						KERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT kPacketNot;
						kPacketNot.m_UnitUID = GetCharUID();
						GetRoomUserInfo( kPacketNot.m_kRoomUserInfo );
						kPacketNot.m_vecInventorySlotInfo = vecZeroEndurance;
						SendToCnRoom( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacketNot );
					}
					break;
				}
			}
		}
	}

#ifdef SERV_GLOBAL_MISSION_MANAGER
#ifdef SERV_SECOND_CLASS_WORLD_WAR_EVENT
	bool bSecondChangeJob = false;

	if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
	{
		bSecondChangeJob = true;
	}
	else if( _CONST_SECOND_CLASS_WORLD_WAR_EVENT::bWorldWarEventForElesis1stClass == true )
	{
		if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true &&
			static_cast<CXSLUnit::UNIT_CLASS>( GetUnitType() ) == CXSLUnit::UT_ELESIS &&
			GetLevel() >= 35 )
		{
			bSecondChangeJob = true;
		}
	}

	if(kPacket_.m_uiAttUnit == GetCharUID() && bSecondChangeJob == true )
#else
	if(kPacket_.m_uiAttUnit == GetCharUID())
#endif SERV_SECOND_CLASS_WORLD_WAR_EVENT	
	{
		CTime tCurrentTime = CTime::GetCurrentTime();
		std::wstring wstrGlobalMissionDay = tCurrentTime.Format(_T("%Y-%m-%d"));
		std::wstring wstrGlobalMissionBeginDate = wstrGlobalMissionDay;
		wstrGlobalMissionBeginDate += L" 00:00:00";

		KEGS_GLOBAL_MISSION_UPDATE_NOT kInfoNot;
		SiKGSGlobalMissionManager()->GetGlobalMissionInfo( kInfoNot );
		
		std::map< std::wstring, KGlobalMissionInfo >::iterator mit = kInfoNot.m_mapGlobalMissionInfo.find(wstrGlobalMissionBeginDate);
		if( mit != kInfoNot.m_mapGlobalMissionInfo.end() )
		{
			if( mit->second.m_iMissionType == 1 && mit->second.m_iClearCondition == kPacket_.m_iNPCID )
			{
				KEGB_GLOBAL_MISSION_UPDATE_NOT kNot;
				kNot.m_wstrGlobalMissionDay = mit->first;
				kNot.m_iNowCount = 1;
				SendToGlobalServer( EGB_GLOBAL_MISSION_UPDATE_NOT, kNot );

				KDBE_REGIST_GLOBAL_MISSION_CLEAR_NOT kNot2;
				kNot2.m_wstrGlobalMissionDay = wstrGlobalMissionDay;
				kNot2.m_iUnitUID = GetCharUID();
				SendToGameDB( DBE_REGIST_GLOBAL_MISSION_CLEAR_NOT, kNot2 );
			}
		}
	}
#endif SERV_GLOBAL_MISSION_MANAGER

	//////////////////////////////////////////////////////////////////////////
	// 펫 포만도
#ifdef SERV_PET_SYSTEM
	// 펫 포만도 조건을 내구도 감소 조건이랑 동일하게 한다.
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true )
	{
		KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT kNot;

		// 펫 포만도 감소 체크!

		//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, m_kInventory.CheckPetToyItem(), kNot ) == true )
#else
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, kNot ) == true )
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
#endif SERV_PET_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 근성도 소모
	if( kPacket_.m_bDecreaseEndurance == true )
	{
		bool bPlayWithTuto = false;

		// 제자랑 같이 플레이 했는지 찾아보기
		if( GetPartyUID() > 0  &&  m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_TEACHER )
		{
			std::vector< UidType > vecTutoUserList;
			m_kUserTutorialInfo.GetTutorUnitUIDList( vecTutoUserList );

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTutoUserList )
			{
				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					bPlayWithTuto = true;
					break;
				}
			}
		}

		if( DecreaseSpirit( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty), kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, bPlayWithTuto, false ) == true )
		{
			KEGS_DECREASE_SPIRIT_NOT kNot;
			kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit();
			kNot.m_iHonorPoint = m_kUserGuildManager.GetHonorPoint();
#ifdef SERV_CHINA_SPIRIT_EVENT
			kNot.m_arrChinaSpirit[0] = GetChinaSpirit( 0 );
			kNot.m_arrChinaSpirit[1] = GetChinaSpirit( 1 );
			kNot.m_arrChinaSpirit[2] = GetChinaSpirit( 2 );
			kNot.m_arrChinaSpirit[3] = GetChinaSpirit( 3 );
			kNot.m_arrChinaSpirit[4] = GetChinaSpirit( 4 );
			kNot.m_arrChinaSpirit[5] = GetChinaSpirit( 5 );
#endif SERV_CHINA_SPIRIT_EVENT
			SendPacket( EGS_DECREASE_SPIRIT_NOT, kNot );
		}
	}
}

//{{ 2012. 11. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, KERM_NPC_UNIT_DIE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_NPC_UNIT_DIE_NOT kPacketNpcDie;
	kPacketNpcDie.m_cAttUnitType	= kPacket_.m_cAttUnitType;
	kPacketNpcDie.m_EXPList			= kPacket_.m_EXPList;
	kPacketNpcDie.m_iNPCID			= kPacket_.m_iNPCID;
	kPacketNpcDie.m_nDieNPCUID		= kPacket_.m_nDieNPCUID;
	kPacketNpcDie.m_uiAttUnit		= kPacket_.m_uiAttUnit;
	
	// 1. NoDrop이면 아무 처리도 안함
	if( kPacket_.m_bNoDrop )
	{
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
		return;
	}

	// 2. 내가 받을 경험치가 있남?
	std::map< UidType, KEXPData >::iterator mit = kPacketNpcDie.m_EXPList.find( GetCharUID() );
	if( mit == kPacketNpcDie.m_EXPList.end() )
	{
		// 보상받을 경험치가 없다면 바로 클라이언트로 보냄
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );
	}
	else
	{
#ifdef SERV_PC_BANG_TYPE
		bool bSpiritFree = ExistActivateBuff( CXSLBuffManager::BTI_BUFF_PREMIUM_PC_ROOM ) & SiKGameSysVal()->IsSpiritFree( GetPcBangType() );
#endif SERV_PC_BANG_TYPE

		// 배틀 필드라면 근성도가 없으면 몬스터를 잡아도 EXP 를 얻지 못하게 한다.
		if( m_kUserSpiritManager.GetSpirit() <= 0
		 && SiKGameEventManager()->CheckSpiritEvent() == false
#ifdef SERV_PC_BANG_TYPE
		 && bSpiritFree == false
#else SERV_PC_BANG_TYPE
		 && ExistActivateBuff( CXSLBuffManager::BTI_BUFF_PREMIUM_PC_ROOM ) == false
#endif SERV_PC_BANG_TYPE
		 )
		{
			mit->second.m_iEXP = 0;
			mit->second.m_iPartyEXP = 0;
			mit->second.m_iSocketOptEXP = 0;
			//{{ 2012. 12. 18	최육사	필드에서도 경험치 이벤트 적용
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
			mit->second.m_iEventBonusEXP = 0;
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
			//}}
		}

		float fAddPartyFeverExpRate = 0.0f;

		if( SiKPartyListManager()->GetPartyFeverExpRate( GetPartyUID(), fAddPartyFeverExpRate ) == true )
		{
			int iPartyFeverEXP = static_cast<int>( (mit->second.m_iEXP) * fAddPartyFeverExpRate );
			iPartyFeverEXP += static_cast<int>( (mit->second.m_iPartyEXP) * fAddPartyFeverExpRate );

			// party fever 보너스 경험치는 소켓옵션 보너스에 더한다
			mit->second.m_iSocketOptEXP += iPartyFeverEXP;
		}

		// party fever보너스까지 처리완료된뒤 클라이언트로 보냄
		SendPacket( EGS_NPC_UNIT_DIE_NOT, kPacketNpcDie );

		// 경험치 보상
		m_kEXP.AddExp( mit->second.SumEXP() );

		// 레벨업 체크
		CheckCharLevelUp();
	}

	//////////////////////////////////////////////////////////////////////////
	// 내구도 소모	
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true  &&  kPacket_.m_bQuestComplete )
	{
		KEGS_DECREASE_ENDURANCE_NOT kNot;
		std::vector< KInventoryItemInfo > vecZeroEndurance;

		// 배틀필드 내구도 소모
		m_kInventory.DecreaseEquippedItemEnduranceInBattleField( kPacket_.m_sSubStageExpNpcNum, kPacket_.m_cUserCount, kNot.m_vecEnduranceUpdate, vecZeroEndurance );

		// 내구도가 소모되었다면 클라이언트에 업데이트한다
		if( !kNot.m_vecEnduranceUpdate.empty() )
		{
			SendPacket( EGS_DECREASE_ENDURANCE_NOT, kNot );

			// 내구도 모두 소모된 아이템이 있다면
			if( GetRoomUID() > 0  &&  vecZeroEndurance.empty() == false )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_BATTLE_FIELD:
					{
						KERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT kPacketNot;
						kPacketNot.m_UnitUID = GetCharUID();
						GetRoomUserInfo( kPacketNot.m_kRoomUserInfo );
						kPacketNot.m_vecInventorySlotInfo = vecZeroEndurance;
						SendToCnRoom( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacketNot );
					}
					break;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 퀘스트
	if( kPacket_.m_bQuestComplete == true )
	{
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		bool bHasFriend = false;

		if( GetPartyUID() > 0 )
		{
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
			}

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRecruitUnitUID )
			{
				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					bHasFriend = true;
					break;
				}
			}
		}
#endif SERV_RECRUIT_EVENT_SUBQUEST

		m_kUserQuestManager.Handler_ERM_NPC_UNIT_DIE_NOT( kPacket_.m_iDungeonID,
														kPacket_.m_cDifficulty,
														kPacket_.m_iNPCID,
														GetThisPtr<KGSUser>(),
														kPacket_.m_cDungeonMode
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
														, bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST
														);
	}

	//////////////////////////////////////////////////////////////////////////
	// 칭호 미션
	if( kPacket_.m_bQuestComplete == true )
	{
		m_kUserTitleManager.OnNpcUnitDie( kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, kPacket_.m_cDungeonMode, kPacket_.m_iNPCID, GetThisPtr<KGSUser>() );
	}
	
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
	{
		std::map< UidType, KEXPData >::iterator mit = kPacketNpcDie.m_EXPList.find( GetCharUID() );
		if( mit != kPacketNpcDie.m_EXPList.end() )
		{
			if( mit->second.m_iEXP > 0 || mit->second.m_iEventBonusEXP > 0 || mit->second.m_iPartyEXP > 0 || mit->second.m_iSocketOptEXP > 0 )
			{
				FieldMonsterKillCountNot(kPacket_.m_iNPCID,kPacket_.m_uiAttUnit);
			}
		}
	}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
	// 펫 포만도
#ifdef SERV_PET_SYSTEM
	// 펫 포만도 조건을 내구도 감소 조건이랑 동일하게 한다.
	if( kPacket_.m_sSubStageExpNpcNum > 0  &&  kPacket_.m_bDecreaseEndurance == true  &&  kPacket_.m_bQuestComplete )
	{
		KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT kNot;

		// 펫 포만도 감소 체크!

		//{{ 2012. 04. 26	박세훈	펫 장난감
#ifdef SERV_PET_TOY
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, m_kInventory.CheckPetToyItem(), kNot ) == true )
#else
		if( m_kUserPetManager.DecreaseSatietyOnDungeonRoom( kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, kNot ) == true )
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
				case CXSLRoom::RT_BATTLE_FIELD:
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
#endif SERV_PET_SYSTEM
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 근성도 소모
	if( kPacket_.m_bDecreaseEndurance == true  &&  kPacket_.m_bQuestComplete == true )
	{
		bool bPlayWithTuto = false;

		//////////////////////////////////////////////////////////////////////////
		// 제자랑 같이 플레이 했는지 찾아보기
		if( GetPartyUID() > 0  &&  m_kUserTutorialInfo.GetTutorUnitType() == KUserTutorialInfo::TUT_TEACHER )
		{
			std::vector< UidType > vecTutoUserList;
			m_kUserTutorialInfo.GetTutorUnitUIDList( vecTutoUserList );

			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecTutoUserList )
			{
				if( SiKPartyListManager()->IsPartyMember( GetPartyUID(), iUnitUID ) )
				{
					bPlayWithTuto = true;
					break;
				}
			}
		}

		if( DecreaseSpirit( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty), kPacket_.m_cUserCount, kPacket_.m_sSubStageExpNpcNum, bPlayWithTuto, true ) == true )
		{
			KEGS_DECREASE_SPIRIT_NOT kNot;
			kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit();
			kNot.m_iHonorPoint = m_kUserGuildManager.GetHonorPoint();
#ifdef SERV_CHINA_SPIRIT_EVENT
			kNot.m_arrChinaSpirit[0] = GetChinaSpirit( 0 );
			kNot.m_arrChinaSpirit[1] = GetChinaSpirit( 1 );
			kNot.m_arrChinaSpirit[2] = GetChinaSpirit( 2 );
			kNot.m_arrChinaSpirit[3] = GetChinaSpirit( 3 );
			kNot.m_arrChinaSpirit[4] = GetChinaSpirit( 4 );
			kNot.m_arrChinaSpirit[5] = GetChinaSpirit( 5 );
#endif SERV_CHINA_SPIRIT_EVENT
			SendPacket( EGS_DECREASE_SPIRIT_NOT, kNot );
		}
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

_IMPL_ON_FUNC( ERM_DROP_ITEM_NOT, KEGS_DROP_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 배틀 필드라면 근성도가 없으면 몬스터를 잡아도 아이템을 얻지 못하게 한다.
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD  )
	{
		if( m_kUserSpiritManager.GetSpirit() <= 0 )
		{
			kPacket_.m_DropItemDataList.clear();
		}
	}

	SendPacket( EGS_DROP_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_ITEM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_GET_ITEM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GET_ITEM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GET_ITEM_NOT, KEGS_GET_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	if(m_AntiAddictionManager.IsEnable())
	{
		std::map< UidType, KDungeonRewardED >::iterator mit;
		mit = kPacket_.m_mapGetED.find( GetCharUID() );
		if( mit != kPacket_.m_mapGetED.end() )
		{
			mit->second.m_iED		= static_cast<int>(m_AntiAddictionManager.GetRewardRatio() * mit->second.m_iED);
			mit->second.m_iBonusED	= static_cast<int>(m_AntiAddictionManager.GetRewardRatio() * mit->second.m_iBonusED);
		}
	}
#endif SERV_ANTI_ADDICTION_SYSTEM

	SendPacket( EGS_GET_ITEM_NOT, kPacket_ );

	//{{ 2009. 5. 11  최육사	실시간ED
	if( !kPacket_.m_mapGetED.empty() )
	{
		std::map< UidType, KDungeonRewardED >::const_iterator mit;
		mit = kPacket_.m_mapGetED.find( GetCharUID() );
		if( mit != kPacket_.m_mapGetED.end() )
		{
			//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			IncreaseED( mit->second.SumED(), KUserEDManager::ER_IN_ED_DUNGEON_DROP );
#else
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-06-17	// 박세훈
			const int iBeforeED = m_iED;
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG

			m_iED += mit->second.SumED();

//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-06-17	// 박세훈
			m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_BATTLEFIELD_REWARD, iBeforeED );
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}			

			{
#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
				if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
				{
					// DB통계 ED
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( 0 );
					KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PBattleField, mit->second.SumED() );

					// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PBattleFieldED, mit->second.SumED() );
#else //SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PBattleFieldED, mit->second.SumED() );
#endif //SERV_USER_STATISTICS_RENEWAL					
				}
				else
#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
				{
					// DB통계 ED
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( 0 );
					KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PDungeon, mit->second.SumED() );

					// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PDungeon, mit->second.SumED() );
#else //SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PDungeonED, mit->second.SumED() );
#endif //SERV_USER_STATISTICS_RENEWAL
				}
			}

			// ED 어뷰저 - 기록빈도가 높은데 DB가 감당할수 있을까..? 이거는 모아서 업데이트 해야겠다..
			//if( m_bIsEDAbuserLog )
			//{
			//	KELOG_ED_ABUSER_LOG_NOT kPacketEDLog;
			//	kPacketEDLog.m_iUnitUID  = GetCharUID();
			//	kPacketEDLog.m_iState	 = KAbuserLogManager::ELS_DUNGEON_REALTIME;
			//	kPacketEDLog.m_iBeforeED = iBeforeED;
			//	kPacketEDLog.m_iAfterED	 = m_iED;
			//	kPacketEDLog.m_iED		 = iED;
			//	SendToLogDB( ELOG_ED_ABUSER_LOG_NOT, kPacketEDLog );
			//}
		}
	}
	//}}
}

//{{ 2009. 5. 11  최육사		실시간아이템
_IMPL_ON_FUNC( ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, KEGS_GET_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-30	// 박세훈
	int iAfterWorkStorageKey;
	if( m_kAfterWorkStorage.InsertData( kPacket_, iAfterWorkStorageKey ) == KAfterWorkStorage::AWS_SUCCEED )
	{
		if( ( SiCXSLEnchantItemManager()->CheckRandomEnchant( kPacket_.m_iItemID, CXSLEnchantItemManager::RET_DROP ) == true ) &&
			( GetKGameServer()->PopFirstTimeEnchantEvent() == true )
			)
		{
			KEGB_TIME_ENCHANT_EVENT_NOT kPacket;
			kPacket.m_iGSUID				= GetKGameServer()->GetUID();
			kPacket.m_iUnitUID				= GetCharUID();
			kPacket.m_iAfterWorkStorageKey	= iAfterWorkStorageKey;
			kPacket.m_usEventID				= EGB_TIME_ENCHANT_EVENT_DROP_NOT;
			SendToGlobalServer( EGB_TIME_ENCHANT_EVENT_NOT, kPacket );
			return;
		}
		else
		{
			KEGB_TIME_ENCHANT_EVENT_DROP_NOT kPacket;
			kPacket.m_iAfterWorkStorageKey = iAfterWorkStorageKey;
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_GS_USER, NULL, EGB_TIME_ENCHANT_EVENT_DROP_NOT, kPacket );
			QueueingEvent( spEvent );
			return;
		}
	}
	else
	{
		START_LOG( cerr, L"AfterWorkStorage: Key값을 얻을 수 없습니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}
#else // SERV_TIME_ENCHANT_EVENT
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kPacket_.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< END_LOG;
		return;
	}

	// 획득 아이템
	std::map< int, KItemInfo > mapGetItem;
	std::map< int, int > mapInsertedItem;

	KItemInfo kItemInfo;
	kItemInfo.m_iItemID		  = kPacket_.m_iItemID;
	kItemInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
	kItemInfo.m_iQuantity	  = 1;
	kItemInfo.m_sEndurance	  = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
	//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kItemInfo.m_cEnchantLevel = kPacket_.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	// 랜덤 강화 레벨 적용
	SiCXSLEnchantItemManager()->RandomEnchant( kPacket_.m_iItemID
											 , CXSLEnchantItemManager::RET_DROP
											 , kItemInfo.m_cEnchantLevel
											 );

	// 9강 이상 강화 성공시 전체 공지!
	SendEnchantNotice( kPacket_.m_iItemID, kItemInfo.m_cEnchantLevel );

	// 특정 등급의 아이템인 경우 밀봉 처리
	SiCXSLItemManager()->ItemSealProcess( kPacket_.m_iItemID
		, kItemInfo.m_sPeriod
		, kItemInfo.m_ucSealData
		);
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2009. 11. 18  최육사	특정시각드롭이벤트
	if( kPacket_.m_cGetItemType == KEGS_GET_ITEM_NOT::GIT_DROP_EVENT_ITEM )
	{
		// 밀봉 가능한 아이템이라면 밀봉시킨다!
		if( SiCXSLItemManager()->IsCanSealForCashItem( kPacket_.m_iItemID ) == true )
		{
			kItemInfo.m_ucSealData = 100;
		}
	}
	//}}

	mapGetItem.insert( std::make_pair( kPacket_.m_iItemID, kItemInfo ) );

	KDBE_GET_ITEM_INSERT_TO_INVENTORY_REQ kPacketReq;
	m_kInventory.PrepareInsert( mapGetItem, mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( mapInsertedItem.empty() )
	{
		//////////////////////////////////////////////////////////////////////////
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		AddTempInventoryItem( kPacket_.m_iItemID
							, kItemInfo.m_ucSealData
							//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
							, kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
							//}}
							);
#else
		// 임시 인벤토리로 저장하러 가기!
		KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
		if( m_kTempInventory.AddTempInventoryItem( kPacket_.m_iItemID, kItemInfo.m_ucSealData, kPacket_.m_iDropItemUID, kNot.m_kTempInvenItem ) )
		{
			SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"임시 인벤토리에 아이템 저장 실패!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< END_LOG;
		}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
#else
#pragma ELSE_MESSAGE
		// 유저들 화면에 아이템 습득하는거 보여주기
		//if( GetPartyUID() > 0  &&  kPacket_.IsDungeonDropItem() )
		//{
		//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kPacket_ );
		//}
#endif SERV_CHANNEL_PARTY
		//}}

		return;
	}

	// DB업데이트
	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_mapInsertItem = mapInsertedItem;
		kPacketReq.m_kBroadInfo = kPacket_;
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ, kPacketReq );
		return;
	}

	//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY		
#else
#pragma ELSE_MESSAGE
	// 다른 유저들에게 아이템 획득 알리기
	//if( GetPartyUID() > 0  &&  kPacket_.IsDungeonDropItem() )
	//{
	//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kPacket_ );
	//}
#endif SERV_CHANNEL_PARTY
	//}}

	// 결과창에 획득한 아이템들을 보여주기 위해 습득아이템정보를 방에 업데이트
	if( GetRoomUID() > 0  &&  kPacket_.m_cGetItemType != KEGS_GET_ITEM_NOT::GIT_RESULT_ITEM )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = mapInsertedItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// 클라이언트 인벤토리에 습득 아이템 업데이트
	KEGS_GET_ITEM_REALTIME_NOT kPacket;
	kPacket.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
	SendPacket( EGS_GET_ITEM_REALTIME_NOT, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 통계 : Character Info, Item
	// 동전일 경우, DB 통계 카운트 에서 제외
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
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
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}

#endif // SERV_TIME_ENCHANT_EVENT
}

IMPL_ON_FUNC( DBE_GET_ITEM_INSERT_TO_INVENTORY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"실시간 아이템 DB업데이트 실패! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )			
			<< END_LOG;

		// 여기다가 업데이트 실패한 아이템 모두 찍자!
		return;
	}

	// 클라이언트 인벤토리에 습득 아이템 업데이트
	KEGS_GET_ITEM_REALTIME_NOT kPacket;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY		
#else
#pragma ELSE_MESSAGE
	// 다른 유저들에게 아이템 획득 알리기
	//if( GetPartyUID() > 0  &&  kPacket_.m_kBroadInfo.m_cGetItemType == KEGS_GET_ITEM_NOT::GIT_DROP_ITEM )
	//{
	//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kPacket_.m_kBroadInfo );
	//}
#endif SERV_CHANNEL_PARTY
	//}}

	// 결과창에 획득한 아이템들을 보여주기 위해 습득아이템정보를 방에 업데이트
	if( GetRoomUID() > 0  &&  kPacket_.m_kBroadInfo.m_cGetItemType != KEGS_GET_ITEM_NOT::GIT_RESULT_ITEM )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = kPacket_.m_mapInsertItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// 클라이언트에 습득아이템 업데이트	
	SendPacket( EGS_GET_ITEM_REALTIME_NOT, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 통계 : Character Info, Item
	// 동전일 경우, DB 통계 카운트 에서 제외
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = kPacket_.m_mapInsertItem.begin(); mit != kPacket_.m_mapInsertItem.end(); ++mit )
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
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, kPacket_.m_mapInsertItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( EGS_GET_TEMP_ITEM_REQ )
{
	// 반드시 S_ROOM상태에서만 동작해야함! ( 게임중, 던전라운지에서 동작가능 )

	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_REQ, EGS_GET_TEMP_ITEM_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_REQ, EGS_GET_TEMP_ITEM_ACK );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}	

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_GET_TEMP_ITEM_ACK kAck;

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
			SendPacket( EGS_GET_TEMP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_GET_TEMP_ITEM_ACK kPacket;
	std::map< int, KItemInfo > mapGetItem;
	std::map< int, int > mapInsertedItem;

	if( m_kTempInventory.PrepareTempItemToInventory( kPacket_.m_iTempItemUID, kPacket.m_iItemID, mapGetItem ) == false )
	{
		START_LOG( cerr, L"임시 인벤 아이템 가져오기 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_11;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	KDBE_GET_TEMP_ITEM_REQ kPacketReq;
	m_kInventory.PrepareInsert( mapGetItem, mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( mapInsertedItem.empty() )
	{
		START_LOG( clog, L"임시 인벤 아이템을 넣을 인벤토리 공간이 부족함." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_12;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	// 임시 인벤에 해당 아이템 삭제처리
	if( m_kTempInventory.DeleteTempItem( kPacket_.m_iTempItemUID ) == false )
	{
		START_LOG( cerr, L"임시 인벤에 아이템 지우기 실패! 일어나서는 안되는에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_11;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	// DB업데이트
	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_iTempItemUID = kPacket_.m_iTempItemUID;
		kPacketReq.m_mapInsertItem = mapInsertedItem;
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_GET_TEMP_ITEM_REQ, kPacketReq );
		return;
	}

	// 결과창에 획득한 아이템들을 보여주기 위해 습득아이템정보를 방에 업데이트
	if( GetRoomUID() > 0 )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = mapInsertedItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// 클라이언트 인벤토리에 습득 아이템 업데이트
	kPacket.m_iTempItemUID = kPacket_.m_iTempItemUID;
	kPacket.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
	SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 통계 : Character Info, Item
	// 동전일 경우, DB 통계 카운트 에서 제외
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
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
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( DBE_GET_TEMP_ITEM_ACK )
{
	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_GET_TEMP_ITEM_ACK );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"실시간 아이템 DB업데이트 실패! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )			
			<< END_LOG;

		// 여기다가 업데이트 실패한 아이템 모두 찍자!
		KEGS_GET_TEMP_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );
		return;
	}

	// 클라이언트 인벤토리에 습득 아이템 업데이트
	KEGS_GET_TEMP_ITEM_ACK kPacket;	
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	// 결과창에 획득한 아이템들을 보여주기 위해 습득아이템정보를 방에 업데이트
	if( GetRoomUID() > 0 )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = kPacket_.m_mapInsertItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// 클라이언트에 습득아이템 업데이트	
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iTempItemUID = kPacket_.m_iTempItemUID;
	SendPacket( EGS_GET_TEMP_ITEM_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 통계 : Character Info, Item
	// 동전일 경우, DB 통계 카운트 에서 제외
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for( mit = kPacket_.m_mapInsertItem.begin(); mit != kPacket_.m_mapInsertItem.end(); ++mit )
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

		// 일반아이템이면 카운트
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
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, kPacket_.m_mapInsertItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( EGS_DELETE_TEMP_ITEM_REQ )
{
	//{{ 2011. 12. 14	최육사	서버 에러 로그 버그 수정
#ifdef SERV_SERVER_ERROR_LOG_BUG_FIX
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DELETE_TEMP_ITEM_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_SERVER_ERROR_LOG_BUG_FIX
	//}}

	if( m_kTempInventory.DeleteTempItem( kPacket_.m_iTempItemUID ) == false )
	{
		START_LOG( cwarn, L"존재하지 않는 임시 인벤 아이템을 버릴려고 했음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iTempItemUID )
			<< END_LOG;
	}

	KEGS_DELETE_TEMP_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iTempItemUID = kPacket_.m_iTempItemUID;
	SendPacket( EGS_DELETE_TEMP_ITEM_ACK, kPacket );
}
//}}

IMPL_ON_FUNC_NOPARAM( EGS_REFRESH_ROOM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_REFRESH_ROOM_REQ );
}

_IMPL_ON_FUNC( ERM_REFRESH_ROOM_ACK, KEGS_REFRESH_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_REFRESH_ROOM_ACK, kPacket_ );
}

IMPL_ON_FUNC( ERM_CHECK_INVALID_ZOMBIE_USER_NOT )
{
	// 방에서 방에 입장한 유저가 좀비라고 판단되면 날리는 이벤트!

	START_LOG( cout2, L"게임방 좀비유저 유저인데 접속되어있음.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetStateIDString() )
		<< BUILD_LOG( GetRoomUID() )
		<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
		<< BUILD_LOG( GetTradeUID() )
		<< BUILD_LOG( GetPersonalShopUID() );

	if( GetRoomUID() != 0  &&  GetRoomUID() != kPacket_.m_iRoomUID )
	{
		START_LOG( cerr, L"게임방 좀비유져인데 이미 다른방에 있음.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( kPacket_.m_iRoomUID )
			<< END_LOG;
	}

	if( GetStateID() == KGSFSM::S_ROOM )
	{
		StateTransition( KGSFSM::I_TO_FIELD_MAP );

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );

		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE  ||
			CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) == CXSLRoom::RT_SQUARE )
		{
			//leave square 보내기
			//KEGS_LEAVE_SQUARE_ACK kPacket;
			//kPacket.m_iOK = NetError::NET_OK;

			//SendPacket( EGS_LEAVE_SQUARE_ACK, kPacket );
			SendID( EGS_BAN_USER_NOT );

			START_LOG( cout2, L"좀비유저인데 아직 같은 광장에 있음.?" );
		}
		else
		{
			//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
			std::vector< int > vecDeleteBuff;
			std::vector< KBuffInfo > vecActivateBuff;

			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
			{
				//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
				m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff, false );
#else
				m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecDeleteBuff );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
				//}
			}
			else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
			{
				m_kUserBuffManager.OnLeaveBattleField( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
			}

			if( vecDeleteBuff.empty() == false )
			{
				// 로그인 서버 버프 매니저에 업데이트
				KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
				kReq.m_iUnitUID = GetCharUID();
				kReq.m_vecDeActivateBuff = vecDeleteBuff;
				kReq.m_vecActivateBuff = vecActivateBuff;
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
#endif SERV_SERVER_BUFF_SYSTEM
			//}}

			//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
			KEGS_CHECK_INVALID_ROOM_NOT	kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			//{{ 2012. 11. 20	최육사		배틀필드에서 마을로 이동처리
#ifdef SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
			kPacket.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( GetLevel(), m_mapDungeonClear );
#else
			kPacket.m_iMapID = GetMapID();
#endif SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
			//}}
			// 클라이언트는 아래 패킷 받고 필드로 이동해야함!
			SendPacket( EGS_CHECK_INVALID_ROOM_NOT, kPacket );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
			//}}

			KEGS_UPDATE_UNIT_INFO_NOT kNot;
			GetUnitInfo( kNot.m_kUnitInfo );
			kNot.m_bGameEnd = true;
			SendPacket( EGS_UPDATE_UNIT_INFO_NOT, kNot );
		}

		if( GetRoomUID() != 0  &&  GetRoomUID() == kPacket_.m_iRoomUID )
		{
			START_LOG( cout2, L"좀비유저인데 아직 같은방에 있음.?" );
		}

		SetRoomUID( 0 );
	}

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );

		// 여긴 왜 TradeUID에 0을 안넣어?
	}

	if( GetPersonalShopUID() != 0 )
	{
		//{{ 2011. 05. 02	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		if( CXSLRoom::GetRoomType( GetPersonalShopUID() ) == CXSLRoom::RT_PERSONAL_SHOP )
		{
			KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
			_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
		}
		else
		{
			KERM_LEAVE_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
			_SendToLoginRoom( GetPersonalShopUID(), ERM_LEAVE_PSHOP_AGENCY_REQ, kPacket );
		}
#else
		KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
		kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_01;
		_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
#endif SERV_PSHOP_AGENCY
		//}}

		SetPersonalShopUID( 0 );
	}
}

//센터서버 뻣었을시...처리
_IMPL_ON_FUNC( ERM_CHECK_INVALID_ROOM_NOT, UidType )
{
	// 1. 죽은 센터에서 생성된 방에 입장해 있었다면?
	if( GetRoomUID() != 0  &&  KncUid::ExtractServerID( GetRoomUID() ) == kPacket_ )
	{
		KEGS_CHECK_INVALID_ROOM_NOT	kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		//{{ 2012. 11. 20	최육사		배틀필드에서 마을로 이동처리
#ifdef SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
		kPacket.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( GetLevel(), m_mapDungeonClear );
#else
		kPacket.m_iMapID = GetMapID();
#endif SERV_CN_SERVER_SHUTDOWN_MOVE_TO_VILLAGE
		//}}

		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}

		// 클라이언트는 아래 패킷 받고 필드로 이동해야함!
		SendPacket( EGS_CHECK_INVALID_ROOM_NOT, kPacket );

		// 방에서 이탈했다면 파티 이탈도 같이 처리
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_02 );

		//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//{{ 2012. 02. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SendLeaveAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_02 );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}

	// 2. 죽은 센터에서 생성된 거래방에 있었다면?
	if( GetTradeUID() != 0  &&  KncUid::ExtractServerID( GetTradeUID() ) == kPacket_ )
	{
		KEGS_BREAK_TRADE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_REQUEST_TRADE_REPLY_04;
		SendPacket( EGS_BREAK_TRADE_NOT, kPacketNot );

		// 센터가 이미 죽었으니 TradeUID값은 0으로 그냥 넣어주자!
		m_iTradeUID = 0;
	}

	// 3. 죽은 센터에서 생성된 개인상점방에 있었다면?
	if( GetPersonalShopUID() != 0  &&  KncUid::ExtractServerID( GetPersonalShopUID() ) == kPacket_ )
	{
		KEGS_BREAK_PERSONAL_SHOP_NOT kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_01;
		SendPacket( EGS_BREAK_PERSONAL_SHOP_NOT, kPacket );

		// 센터가 이미 죽었으니 PersonalShopUID값은 0으로 그냥 넣어주자!
		m_iPersonalShopUID = 0;
	}

	//{{ 2010. 03. 09  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY

	// 4. 죽은 센터에서 생성된 파티에 있었다면?
	if( GetPartyUID() != 0  &&  KncUid::ExtractServerID( GetPartyUID() ) == kPacket_ )
	{
		//나간유저 파티정보 업데이트.
		std::set< UidType > setEmpty;
		SendUpdatePartyMemberList( GetPartyUID(), setEmpty );

		// 던전으로 가서 유저를 이탈시키자!
		if( GetRoomUID() > 0 )
		{
			SendToCnRoom( ERM_PARTY_BAN_USER_REQ );
		}
		else
		{
			SendID( EGS_PARTY_BAN_USER_NOT );
		}

		// 센터가 이미 죽었으니 PartyUID값은 0으로 넣어주자!
		SetPartyUID( 0 );
	}

#endif SERV_CHANNEL_PARTY
	//}}
}

//{{ 2012. 10. 17	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
IMPL_ON_FUNC( EGS_RESULT_SUCCESS_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_RESULT_SUCCESS_REQ, kPacket_ );
}
#else
IMPL_ON_FUNC_NOPARAM( EGS_RESULT_SUCCESS_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_RESULT_SUCCESS_REQ );
}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

_IMPL_ON_FUNC( ERM_RESULT_SUCCESS_ACK, KEGS_RESULT_SUCCESS_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:		
			{
				if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
				{
					// 근성도 부족시 강제로 강퇴시킨다.
					kPacket_.m_iOK = NetError::NOT_LEAVE_ROOM_REASON_14;
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"방에 입장해있는데 RoomUID가 없다?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;
	}

	SendPacket( EGS_RESULT_SUCCESS_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_RESULT_ALL_UNIT_SUCCESS_NOT, KEGS_RESULT_ALL_UNIT_SUCCESS_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// 만약 여기서 해당 캐릭터의 복귀 지점이 필드가 아니라면 바로 다시 방으로 패킷을 쏴서 다음 사람이 필드로 복귀 하도록 처리한다!
	if( SEnum::IsBattleFieldID( GetMapID() ) == true )
	{
		m_kUserUnitManager.ReservedReturnToFieldInfo( kPacket_.m_kReturnToFieldInfo );
	}
	else
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketNot;
		kPacketNot.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToCnRoom( ERM_NEXT_RETURN_TO_FIELD_NOT, kPacketNot );
	}

	SendPacket( EGS_RESULT_ALL_UNIT_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_SQUARE_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_SQUARE_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_nTotalPage = 1;
		kPacket.m_nViewPage = 1;
		kPacket.m_vecSquareInfo.clear();
		SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_SQUARE_01;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_nTotalPage = 1;
			kPacket.m_nViewPage = 1;
			kPacket.m_vecSquareInfo.clear();
			SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
			return;
		}
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		kPacket.m_nTotalPage = 1;
		kPacket.m_nViewPage = 1;
		kPacket.m_vecSquareInfo.clear();
		SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	if( SiKRoomListManager()->GetSquareInfoPage( kPacket_, kPacket ) == false )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_nViewPage )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	}

	SendPacket( EGS_SQUARE_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_JOIN_SQUARE_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_SQUARE_REQ, EGS_JOIN_SQUARE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_SQUARE_ACK );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
		KEGS_JOIN_SQUARE_ACK kPacket;
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else //SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
#endif //SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
		{
			KEGS_JOIN_SQUARE_ACK kPacket;
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_06;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kPacket.m_iOK = NetError::ERR_SQUARE_01;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
			return;
		}
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KEGS_JOIN_SQUARE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifndef DEPRECATED_SERVER_GROUP_MASK

#ifdef SERV_IN_HOUSE
#else
	//{{ 2010. 05. 20  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( IsSameServerGroupUID( kPacket_.m_iSquareUID ) == false )
	{
		START_LOG( cerr, L"다른 서버군의 광장에는 입장할 수 없다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSquareUID )
			<< END_LOG;

		KEGS_JOIN_SQUARE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_SQUARE_01;
		SendPacket( EGS_JOIN_SQUARE_ACK, kPacket );
		return;
	}
#endif SERV_INTEGRATION
	//}}
#endif SERV_IN_HOUSE

#endif DEPRECATED_SERVER_GROUP_MASK

	KERM_JOIN_SQUARE_REQ kPacket;
	GetSquareUserInfo( kPacket );

	_SendToCnRoom( kPacket_.m_iSquareUID, ERM_JOIN_SQUARE_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_JOIN_SQUARE_ACK, KEGS_JOIN_SQUARE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_kSquareInfo.m_iSquareUID );

		StateTransition( KGSFSM::I_TO_ROOM );
		//SendUpdateUnitState( CXSLUnit::CUS_SQUARE );

		// 유저 통계 광장 입장 카운트
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Square_Count, 1 );

		// 광장 입장 타이머
		m_kTimer[TM_SQUARE_PLAY].restart();

		// 파티가 맺어져 있을경우 파티 탈퇴처리
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_03 );

		//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//{{ 2012. 02. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SendLeaveAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_02 );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
	else
	{
		START_LOG_WITH_NAME( clog )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			;
	}

	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	if( GetAuthLevel() < SEnum::UAL_GM || GetEDMonitoringMode() == false )
	{
		// ED 모니터링 중이 아니거나 권한이 낮으면 ED 정보를 0 으로 초기화 한다.
		for( int i = 0 ; i < (int)kPacket_.m_vecUserInfo.size() ; ++i )
		{
			kPacket_.m_vecUserInfo[i].m_iED = 0;
		}
	}
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	SendPacket( EGS_JOIN_SQUARE_ACK, kPacket_ );

	//////////////////////////////////////////////////////////////////////////
	// 필드에서 뺀다~
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

		//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
		SendUpdateGuildMemberInfo();
#endif GUILD_TEST
		//}}
	}
	//////////////////////////////////////////////////////////////////////////
}

_IMPL_ON_FUNC( ERM_JOIN_SQUARE_NOT, KEGS_JOIN_SQUARE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_JOIN_SQUARE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_LEAVE_SQUARE_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_SQUARE_REQ, EGS_LEAVE_SQUARE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_SQUARE_ACK );
#endif SERV_REPEAT_FILTER_REFAC	
	//}}

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_LEAVE_SQUARE_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_SQUARE_ACK, KEGS_LEAVE_SQUARE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );

		// 유저 통계 광장 플레이 타이머
		int iSqaurePlayTime = static_cast<int>( m_kTimer[TM_SQUARE_PLAY].elapsed() + 0.5 );
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Square_PlayTime, iSqaurePlayTime );

		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );

			// 응? 여긴왜 TradeUID에 0을 안넣어?
		}

		if( GetPersonalShopUID() != 0 )
		{
			//{{ 2011. 05. 02	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
			if( CXSLRoom::GetRoomType( GetPersonalShopUID() ) == CXSLRoom::RT_PERSONAL_SHOP )
			{
				KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
				kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_18;
				_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
			}
			else
			{
				KERM_LEAVE_PERSONAL_SHOP_REQ kPacket;
				kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_18;
				_SendToLoginRoom( GetPersonalShopUID(), ERM_LEAVE_PSHOP_AGENCY_REQ, kPacket );
			}
#else
			KEGS_BREAK_PERSONAL_SHOP_REQ kPacket;
			kPacket.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_18;
			_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket );
#endif SERV_PSHOP_AGENCY
			//}}

			SetPersonalShopUID( 0 );
		}
	}

	SendPacket( EGS_LEAVE_SQUARE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_SQUARE_NOT, KEGS_LEAVE_SQUARE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_SQUARE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_SQUARE_UNIT_SYNC_DATA_REQ )
{
	//{{ 2010. 03. 17  최육사	상태 오류 수정
	VERIFY_STATE_WARN( ( 1, KGSFSM::S_ROOM ) );
	//}}

	SendToCnRoom( ERM_SQUARE_UNIT_SYNC_DATA_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_SQUARE_UNIT_SYNC_DATA_NOT, KEGS_SQUARE_UNIT_SYNC_DATA_NOT )
{
	//{{ 2010. 03. 17  최육사	상태 오류 수정
	//VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
	if( GetStateID() != KGSFSM::S_ROOM )
	{
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			START_LOG( cerr, L"상태 오류. [거래광장] Name : " << m_strName )
				<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
				<< L"    허용 상태 : ( 1, KGSFSM::S_ROOM )"
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"상태 오류. Name : " << m_strName )
				<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
				<< L"    허용 상태 : ( 1, KGSFSM::S_ROOM )"
				<< END_LOG;
		}
		return;
	}
	//}}

	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	if( GetAuthLevel() < SEnum::UAL_GM || GetEDMonitoringMode() == false )
	{
		kPacket_.m_iED = 0;
	}
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	SendPacket( EGS_SQUARE_UNIT_SYNC_DATA_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CREATE_TC_ROOM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_CREATE_TC_ROOM_ACK kPacketAck;
	KERM_OPEN_TC_ROOM_REQ kPacket;

	if( SiKTrainingCenterTable()->CheckIfEnter( m_mapTCClear, kPacket_.m_iTCID ) == false )
	{
		START_LOG( cerr, L"수행조건이 되지않는 훈련소를 생성할려고함.!" )
			<< BUILD_LOG( kPacket_.m_iTCID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_TC_01;

		goto end_proc;
	}

	kPacket.m_kTCInfo.m_iTCID		= kPacket_.m_iTCID;	
	kPacket.m_kTCInfo.m_iDungeonID	= SiKTrainingCenterTable()->GetDungeonID( kPacket_.m_iTCID );
	kPacket.m_kTCInfo.m_fPlayTime	= SiKTrainingCenterTable()->GetPlayTime( kPacket_.m_iTCID );
	kPacket.m_kTCInfo.m_cRoomType	= CXSLRoom::RT_TRAININGCENTER;

	if( kPacket.m_kTCInfo.m_iDungeonID <= 0 )
	{
		START_LOG( cerr, L"해당 훈련소에 대한 던전아이디가 없음 허헉 +ㅁ+;" )
			<< BUILD_LOG( kPacket_.m_iTCID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacketAck.m_iOK	= NetError::ERR_TC_03;

		goto end_proc;
	}

	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_TRAININGCENTER );

	START_LOG( clog2, L"만든 사람, 게임 서버" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( KBaseServer::GetKObj()->GetUID() )
		;

	SendToCnRoom( ERM_OPEN_TC_ROOM_REQ, kPacket );
	return;

	//kPacketAck.m_iOK = NetError::ERR_TC_02;

end_proc:
	SendPacket( EGS_CREATE_TC_ROOM_ACK, kPacketAck );
}

_IMPL_ON_FUNC( ERM_OPEN_TC_ROOM_ACK, KEGS_CREATE_TC_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( kPacket_.m_kTCInfo.m_RoomUID );
		StateTransition( KGSFSM::I_TO_ROOM );
		m_kTimer[TM_ROOM].restart();

		if( GetRoomUID() != 0 )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_TRAININGCENTER )
			{
				//SendUpdateUnitState( CXSLUnit::CUS_TC_PLAY );
			}
		}

		// 파티가 맺어져 있을경우 파티 탈퇴처리
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_04 );

		//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
		//}}

		//{{ 2012. 02. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SendLeaveAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_02 );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2008. 12. 29	최육사	훈련소 입장시 필드 이탈
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );
		//}}
	}
	else
	{
		START_LOG( cerr, L"방생성 실패.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	SendPacket( EGS_CREATE_TC_ROOM_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_END_TC_GAME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_END_TC_GAME_REQ, kPacket_ );
}

IMPL_ON_FUNC( ERM_END_TC_GAME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_END_TC_GAME_ACK kPacket;
	kPacket.m_iOK			= kPacket_.m_iOK;
	kPacket.m_bIsSuccess	= kPacket_.m_bIsSuccess;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( kPacket_.m_bIsSuccess == true )
		{
			int iED, iEXP;
			iED		= 0;
			iEXP	= 0;

			if( m_mapTCClear.find( kPacket_.m_iTCID ) == m_mapTCClear.end() )
			{
				// 이전 난이도를 클리어했는지 검사(방을 만들때 검사했지만 그래도 한번더)
				if( SiKTrainingCenterTable()->CheckIfEnter( m_mapTCClear, kPacket_.m_iTCID ) == true )
				{
					KTCClearInfo kTCInfo;
					kTCInfo.m_iTCID = kPacket_.m_iTCID;
					CTime tCurrentTime = CTime::GetCurrentTime();
					kTCInfo.m_wstrClearTime = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
					kTCInfo.m_bNew = true;
					m_mapTCClear.insert( std::make_pair( kTCInfo.m_iTCID, kTCInfo ) );
				}

				//보상처리를 한번만 하기 위해서.
				if( SiKTrainingCenterTable()->GetReward( kPacket_.m_iTCID, iED, iEXP ) == true )
				{
					//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
					IncreaseED( iED, KUserEDManager::ER_IN_ED_TRAINING_REWARD );
#else
					int iBeforeED = m_iED;

					m_iED		+= iED;

					//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
					//#ifdef SERV_USER_ABUSER_MANAGER
					// ED 어뷰저
					m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_TRAINING_REWARD, iBeforeED );
					//#endif SERV_USER_ABUSER_MANAGER
					//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
					//}}

					// DB통계 ED
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( 0 );
					KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PTraining, iED );
#ifdef SERV_USER_STATISTICS_RENEWAL
					m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PTraining, iED );
#endif //SERV_USER_STATISTICS_RENEWAL

					//{{ 2009. 1. 20  최육사	만렙경험치 예외처리
					m_kEXP.AddExp( iEXP );
					//}}

					CheckCharLevelUp();					
				}
				else
				{
					kPacket.m_iOK	= NetError::ERR_TC_04;
				}
			}

			GetUnitInfo( kPacket.m_kUnitInfo );
		}
	}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	SetRoomUID( 0 );

	//훈련소 종료는 실패해도 방은 사라지기 때문에 상태를 채널로 바꾼다.
	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendPacket( EGS_END_TC_GAME_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_SET_TC_REMAINING_TIME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_SET_TC_REMAINING_TIME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_SET_TC_REMAINING_TIME_ACK, KEGS_SET_TC_REMAINING_TIME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_SET_TC_REMAINING_TIME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_LEAVE_TC_ROOM_ACK, KEGS_LEAVE_TC_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}

		//SendUpdateUnitState( CXSLUnit::CUS_ONLINE );	
	}

	KEGS_LEAVE_ROOM_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::vector< int > vecDeleteBuff;
	std::vector< KBuffInfo > vecActivateBuff;

	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff, false );
#else
		m_kUserBuffManager.OnLeaveDungeonRoom( GetThisPtr<KGSUser>(), vecDeleteBuff );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
		//}
	}
	else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		m_kUserBuffManager.OnLeaveBattleField( GetThisPtr<KGSUser>(), vecActivateBuff, vecDeleteBuff );
	}

	if( vecDeleteBuff.empty() == false )
	{
		// 로그인 서버 버프 매니저에 업데이트
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		kReq.m_vecActivateBuff = vecActivateBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	SendPacket( EGS_LEAVE_ROOM_ACK, kAck );
}

IMPL_ON_FUNC( EGS_ADD_ON_STAT_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_ADD_ON_STAT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_ADD_ON_STAT_ACK, KEGS_ADD_ON_STAT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADD_ON_STAT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_ADD_ON_STAT_NOT, KEGS_ADD_ON_STAT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_ADD_ON_STAT_NOT, kPacket_ );
}

//{{ 2009. 5. 21  최육사		내구도 소모 개편
IMPL_ON_FUNC( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) )

		KEGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT kPacket;
	kPacket.m_UnitUID			   = kPacket_.m_UnitUID;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
	kPacket.m_kGameStat			   = kPacket_.m_kRoomUserInfo.m_kGameStat;	
	SendPacket( EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacket );
}
//}}

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
_IMPL_ON_FUNC( ERM_CHANGE_ROOM_USER_INFO_NOT, KRoomUserInfo )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_USER_INFO_NOT kPacket;
	kPacket.m_iUnitUID	 = kPacket_.m_nUnitUID;
	kPacket.m_cUnitClass = kPacket_.m_cUnitClass;
	//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
	kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif GUILD_TEST
	//}}
	SendPacket( EGS_CHANGE_USER_INFO_NOT, kPacket );
}

_IMPL_ON_FUNC( ERM_CHANGE_SQUARE_USER_INFO_NOT, KSquareUserInfo )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_USER_INFO_NOT kPacket;
	kPacket.m_iUnitUID	 = kPacket_.m_iUnitUID;
	kPacket.m_cUnitClass = kPacket_.m_cUnitClass;
	//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
	kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif GUILD_TEST
	//}}
	SendPacket( EGS_CHANGE_USER_INFO_NOT, kPacket );
}
//}}

//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
IMPL_ON_FUNC( EGS_BUFF_SKILL_INFO_CONVEY_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_BUFF_SKILL_INFO_CONVEY_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BUFF_SKILL_INFO_CONVEY_ACK, KEGS_BUFF_SKILL_INFO_CONVEY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BUFF_SKILL_INFO_CONVEY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BUFF_SKILL_INFO_CONVEY_NOT, KEGS_BUFF_SKILL_INFO_CONVEY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_BUFF_SKILL_INFO_CONVEY_NOT, kPacket_ );
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
_IMPL_ON_FUNC( ERM_CREATE_ATTRIB_NPC_NOT, KEGS_CREATE_ATTRIB_NPC_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CREATE_ATTRIB_NPC_NOT, kPacket_ );

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	std::vector< int > vecNPCData;
	BOOST_TEST_FOREACH( KAttribNpcUnitInfo, kNPCData, kPacket_.m_vecAttribNPCUnit )
	{
		int iNPCID = 0;
		iNPCID = kNPCData.m_sNPCUnitReq.m_NPCID;
		vecNPCData.push_back( iNPCID );
	}

	if( GetRoomUID() != 0  &&  ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, false );
	}
	else if( GetRoomUID() != 0  && ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON ) )
	{
		m_kUserQuestManager.Handler_OnFindNPC( GetThisPtr<KGSUser>(), vecNPCData, true );
	}
#endif SERV_REFORM_QUEST
	//}}
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
IMPL_ON_FUNC( ERM_ACTIVATE_BUFF_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	std::vector< KBuffInfo > vecActiveBuff;
	BOOST_TEST_FOREACH( const KBuffInfo&, kBuffInfo, kPacket_.m_vecActivateBuffList )
	{
		if( ActivateBuff( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, vecActiveBuff ) == false )
		{
			START_LOG( cerr, L"서버 버프 활성화 실패!" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kBuffInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID )
				<< END_LOG;
		}
	}

	if( vecActiveBuff.empty() == false )
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecActivateBuff = vecActiveBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
}
#endif SERV_SERVER_BUFF_SYSTEM
//}}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
_IMPL_ON_FUNC( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방 상태인데 RoomUID가 없네? 이상하다. 일단 진행 하자" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
	}

	SendPacket( EGS_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket_ );
}

IMPL_ON_FUNC( ECN_PARTY_BUFF_UPDATE_NOT )
{
	std::vector<KBuffInfo> vecActivateBuff;
	std::vector<int> vecDeleteBuff;

	std::map<int,KBuffInfo>::iterator mit = kPacket_.m_mapActivateBuffList.begin();
	for(  ; mit != kPacket_.m_mapActivateBuffList.end() ; ++mit )
	{
		ActivateBuff( mit->first, vecActivateBuff, mit->second.m_iBuffStartTime );
	}

	std::set<int>::iterator sit = kPacket_.m_setDeactivateBuffList.begin();
	for(  ; sit != kPacket_.m_setDeactivateBuffList.end() ; ++sit )
	{
		DeactivateBuff( (*sit), vecDeleteBuff );
	}
	
	if( vecActivateBuff.empty() == false || vecDeleteBuff.empty() == false )
	{
		// 로그인 서버 버프 매니저에 업데이트
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_vecActivateBuff = vecActivateBuff;
		kReq.m_vecDeActivateBuff = vecDeleteBuff;
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
}
#endif SERV_SERVER_BUFF_SYSTEM
//}

//{{ 2013. 03. 12	최육사	몬스터 유효 검사
#ifdef SERV_CHECK_EXIST_MONSTER_UID
IMPL_ON_FUNC( EGS_CHECK_EXIST_MONSTER_UID_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_CHECK_EXIST_MONSTER_UID_ACK );

    if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방에 있는것도 아닌데 이 패킷을 보냈네? 시간차 문제인가?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_CHECK_EXIST_MONSTER_UID_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		SendPacket( EGS_CHECK_EXIST_MONSTER_UID_ACK, kPacketAck );
		return;
	}

	SendToCnRoom( ERM_CHECK_EXIST_MONSTER_UID_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHECK_EXIST_MONSTER_UID_ACK, KEGS_CHECK_EXIST_MONSTER_UID_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_CHECK_EXIST_MONSTER_UID_ACK );

	SendPacket( EGS_CHECK_EXIST_MONSTER_UID_ACK, kPacket_ );
}
#endif SERV_CHECK_EXIST_MONSTER_UID
//}}

//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
IMPL_ON_FUNC( EGS_UDP_PING_SCORE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() == 0 )
	{
		START_LOG( clog, L"방에 입장한 상태가 아닌데 UDP PING SCORE패킷을 보냈다" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendToCnRoom( ERM_UDP_PING_SCORE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHECK_PING_SCORE_CHANGE_HOST_NOT, KEGS_CHECK_PING_SCORE_CHANGE_HOST_NOT )
{
	SendPacket( EGS_CHECK_PING_SCORE_CHANGE_HOST_NOT, kPacket_ );	
}
#endif SERV_CHOOSE_FASTEST_HOST
//}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-30	// 박세훈
IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_DROP_NOT )
{
	if( kPacket_.m_iAfterWorkStorageKey == 0 )
	{
		START_LOG( cerr, L"AfterWorkStorage: Key값이 이상합니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iAfterWorkStorageKey )
			<< END_LOG;
		return;
	}

	KEGS_GET_ITEM_NOT kStorage;
	if( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, kStorage ) != KAfterWorkStorage::AWS_SUCCEED )
	{
		START_LOG( cerr, L"AfterWorkStorage: 꺼내오기 실패" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kStorage.m_iItemID );
	if( pItemTemplet == NULL )
	{
		START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kStorage.m_iItemID )
			<< END_LOG;
		return;
	}

	// 획득 아이템
	std::map< int, KItemInfo > mapGetItem;
	std::map< int, int > mapInsertedItem;

	KItemInfo kItemInfo;
	kItemInfo.m_iItemID		  = kStorage.m_iItemID;
	kItemInfo.m_cUsageType	  = pItemTemplet->m_PeriodType;
	kItemInfo.m_iQuantity	  = 1;
	kItemInfo.m_sEndurance	  = ( pItemTemplet->m_PeriodType == CXSLItem::PT_ENDURANCE ) ? pItemTemplet->m_Endurance : 0;
	//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kItemInfo.m_cEnchantLevel = kStorage.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	// 랜덤 강화 레벨 적용
	SiCXSLEnchantItemManager()->RandomEnchant( kStorage.m_iItemID
		, CXSLEnchantItemManager::RET_DROP
		, kPacket_.m_cTimeEnchantEventLevel
		, kItemInfo.m_cEnchantLevel
		);

	// 9강 이상 강화 성공시 전체 공지!
	SendEnchantNotice( kStorage.m_iItemID, kItemInfo.m_cEnchantLevel );

	// 특정 등급의 아이템인 경우 밀봉 처리
	SiCXSLItemManager()->ItemSealProcess( kStorage.m_iItemID
		, kItemInfo.m_sPeriod
		, kItemInfo.m_ucSealData
		);
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2009. 11. 18  최육사	특정시각드롭이벤트
	if( kStorage.m_cGetItemType == KEGS_GET_ITEM_NOT::GIT_DROP_EVENT_ITEM )
	{
		// 밀봉 가능한 아이템이라면 밀봉시킨다!
		if( SiCXSLItemManager()->IsCanSealForCashItem( kStorage.m_iItemID ) == true )
		{
			kItemInfo.m_ucSealData = 100;
		}
	}
	//}}

	mapGetItem.insert( std::make_pair( kStorage.m_iItemID, kItemInfo ) );

	KDBE_GET_ITEM_INSERT_TO_INVENTORY_REQ kPacketReq;
	m_kInventory.PrepareInsert( mapGetItem, mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );

	if( mapInsertedItem.empty() )
	{
		//////////////////////////////////////////////////////////////////////////
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		AddTempInventoryItem( kStorage.m_iItemID
			, kItemInfo.m_ucSealData
			//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
			, kItemInfo
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
			);
#else
		// 임시 인벤토리로 저장하러 가기!
		KEGS_GET_ITEM_TEMP_INVENTORY_NOT kNot;
		if( m_kTempInventory.AddTempInventoryItem( kStorage.m_iItemID, kItemInfo.m_ucSealData, kStorage.m_iDropItemUID, kNot.m_kTempInvenItem ) )
		{
			SendPacket( EGS_GET_ITEM_TEMP_INVENTORY_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"임시 인벤토리에 아이템 저장 실패!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kStorage.m_iItemID )
				<< END_LOG;
		}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
#else
#pragma ELSE_MESSAGE
		// 유저들 화면에 아이템 습득하는거 보여주기
		//if( GetPartyUID() > 0  &&  kStorage.IsDungeonDropItem() )
		//{
		//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kStorage );
		//}
#endif SERV_CHANNEL_PARTY
		//}}

		return;
	}

	// DB업데이트
	if( !kPacketReq.m_vecItemInfo.empty() )
	{
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_mapInsertItem = mapInsertedItem;
		kPacketReq.m_kBroadInfo = kStorage;
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ, kPacketReq );
		return;
	}

	//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY		
#else
#pragma ELSE_MESSAGE
	// 다른 유저들에게 아이템 획득 알리기
	//if( GetPartyUID() > 0  &&  kStorage.IsDungeonDropItem() )
	//{
	//	SiKPartyManager()->BroadCastToPartyMembers( GetPartyUID(), EGS_GET_ITEM_NOT, kStorage );
	//}
#endif SERV_CHANNEL_PARTY
	//}}

	// 결과창에 획득한 아이템들을 보여주기 위해 습득아이템정보를 방에 업데이트
	if( GetRoomUID() > 0  &&  kStorage.m_cGetItemType != KEGS_GET_ITEM_NOT::GIT_RESULT_ITEM )
	{
		KERM_GET_ITEM_COMPLETE_NOT kPacketToRoom;
		kPacketToRoom.m_iUnitUID = GetCharUID();
		kPacketToRoom.m_mapGetItem = mapInsertedItem;
		SendToCnRoom( ERM_GET_ITEM_COMPLETE_NOT, kPacketToRoom );
	}

	// 클라이언트 인벤토리에 습득 아이템 업데이트
	KEGS_GET_ITEM_REALTIME_NOT kPacket;
	kPacket.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
	SendPacket( EGS_GET_ITEM_REALTIME_NOT, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 통계 : Character Info, Item
	// 동전일 경우, DB 통계 카운트 에서 제외
	int iDropItemCnt = 0;
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
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
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_DUNGEON, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// 작업날짜: 2013-06-08	// 박세훈
IMPL_ON_FUNC_NOPARAM( ERM_UPDATE_BUFF_INFO_NOT )
{
	VERIFY_STATE_LOG( clog, ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	SendUpdateBuffInfo();
}
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////
