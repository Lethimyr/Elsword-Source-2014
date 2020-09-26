#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "ExpTable.h"
#include "BaseServer.h"
//{{ 2008. 11. 24  최육사	방 리스트
#include "RoomListManager.h"
//}}

#include "Enum/Enum.h"

//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
//#ifdef SERV_2012_PVP_SEASON2_EVENT
//	#include "GameSysVal/GameSysVal.h"
//#endif SERV_2012_PVP_SEASON2_EVENT
//}}

//{{ 2012. 09. 18   김민성   2012 대전 시즌2 이벤트
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
	#include "GameSysVal/GameSysVal.h"
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
//}}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
	#include "NewPartyListManager.h"
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC_NOPARAM( EGS_OPEN_PVP_ROOM_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_OPEN_PVP_ROOM_LIST_ACK kPacket;
	KStatisticsKey kKey;

	if( SiKRoomListManager()->OpenPVPRoomList( GetThisPtr<KGSUser>(), kPacket ) == false )
	{
		START_LOG_WITH_NAME( cerr )
			<< L"pvp 채널 접속 실패" << dbg::endl
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendPacket( EGS_OPEN_PVP_ROOM_LIST_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 필드 이탈 처리 - 필드에서는 나오지만 S_FIELD_MAP은 유지됩니다!
		SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );		

		// 파티가 맺어져 있을경우 파티 탈퇴처리
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_01 );

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
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_FUNC( EGS_CHANGE_PVP_ROOM_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_CHANGE_PVP_ROOM_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	if( SiKRoomListManager()->ChangePVPRoomList( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
	}

	SendPacket( EGS_CHANGE_PVP_ROOM_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_INVITE_PVP_ROOM_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_INVITE_PVP_ROOM_REQ, EGS_INVITE_PVP_ROOM_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	if( GetRoomUID() < 0  ||  CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_PVP )
	{
		KEGS_INVITE_PVP_ROOM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_43;
		SendPacket( EGS_INVITE_PVP_ROOM_ACK, kPacketAck );
		return;
	}

	KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( GetRoomUID() );
	if( !spRoomInfo )
	{
		START_LOG( cerr, L"대전방 초대를 하려는데 방정보가 없다?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_INVITE_PVP_ROOM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_16;
		SendPacket( EGS_INVITE_PVP_ROOM_ACK, kPacketAck );
		return;
	}

	// 로그인 서버로 보내자!
	KELG_INVITE_PVP_ROOM_REQ kPacketReq;
	kPacketReq.m_iToUnitUID		  = kPacket_.m_iUnitUID;	
	kPacketReq.m_wstrFromNickName = GetCharName();
	kPacketReq.m_kRoomInfo		  = *spRoomInfo;
	SendToLoginServer( ELG_INVITE_PVP_ROOM_REQ, kPacketReq );
}

IMPL_ON_FUNC( ELG_INVITE_PVP_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_INVITE_PVP_ROOM_ACK kPacket;
	kPacket.m_iOK		   = kPacket_.m_iOK;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	SendPacket( EGS_INVITE_PVP_ROOM_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_INVITE_PVP_ROOM_NOT, KEGS_INVITE_PVP_ROOM_NOT )
{
	// 상태제한 걸어야하나?

	// 초대할 수 없는 상태
	if( GetStateID() != KGSFSM::S_FIELD_MAP  &&  GetStateID() != KGSFSM::S_ROOM )
		return;

	char cUnitState = GetUnitState();
	if( cUnitState != CXSLUnit::CUS_PVP_LOBBY  &&  cUnitState != CXSLUnit::CUS_PVP_WAIT )
	{
		// 초대 메시지는 씹힌다!
		return;
	}	

	// 초대 메시지 전송
	KEGS_INVITE_PVP_ROOM_NOT kPacketNot;
	kPacketNot.m_wstrNickName = kPacket_.m_wstrNickName;
	kPacketNot.m_kRoomInfo	  = kPacket_.m_kRoomInfo;
	SendPacket( EGS_INVITE_PVP_ROOM_NOT, kPacketNot );
}

IMPL_ON_FUNC_NOPARAM( EGS_STATE_CHANGE_GAME_INTRUDE_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_STATE_CHANGE_GAME_INTRUDE_REQ, EGS_STATE_CHANGE_GAME_INTRUDE_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-10-08	// 박세훈
	if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
	{
		KEGS_STATE_CHANGE_GAME_INTRUDE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ROOM_53;	// 공식 대전에는 난입할 수 없습니다.
		SendPacket( EGS_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	SendToCnRoom( ERM_STATE_CHANGE_GAME_INTRUDE_REQ );
}

IMPL_ON_FUNC( ERM_STATE_CHANGE_GAME_INTRUDE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );	

	KEGS_STATE_CHANGE_GAME_INTRUDE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_STATE_CHANGE_GAME_INTRUDE_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_GAME_INTRUDE_NOT, KEGS_STATE_CHANGE_GAME_INTRUDE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_STATE_CHANGE_GAME_INTRUDE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_INTRUDE_START_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_INTRUDE_START_REQ, EGS_INTRUDE_START_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-10-08	// 박세훈
	if( ( m_kUserCheatManager.IsObserverMode() == false ) && ( GetRoomListID() == SiKRoomListManager()->GetPvpOffcialRoomListID() ) )
	{
		KEGS_INTRUDE_START_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ROOM_53;	// 공식 대전에는 난입할 수 없습니다.
		SendPacket( EGS_INTRUDE_START_ACK, kPacket );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	SendToCnRoom( ERM_INTRUDE_START_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_INTRUDE_START_ACK, KEGS_INTRUDE_START_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_INTRUDE_START_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_INTRUDE_START_NOT, KEGS_INTRUDE_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

#ifdef SERV_SERVER_BUFF_SYSTEM// 작업날짜: 2013-06-08	// 박세훈
#else // SERV_SERVER_BUFF_SYSTEM
	//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	KRoomUserInfo kRoomUserInfo;
	KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT kPacket;
	const int iRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	kPacket.m_iUnitUID = GetCharUID();
	m_kUserBuffManager.GetBuffList( kPacket.m_vecActivateBuffList, CXSLRoom::RT_PVP );
	GetRoomUserInfo( kRoomUserInfo, iRoomType );
	kPacket.m_kGameStat = kRoomUserInfo.m_kGameStat;
	SendToCnRoom( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket );
#endif SERV_2012_PVP_SEASON2
	//}}
#endif // SERV_SERVER_BUFF_SYSTEM

	SendPacket( EGS_INTRUDE_START_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CURRENT_KILL_SCORE_NOT, KEGS_CURRENT_KILL_SCORE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CURRENT_KILL_SCORE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CURRENT_TEAM_KILL_SCORE_NOT, KEGS_CURRENT_TEAM_KILL_SCORE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CURRENT_TEAM_KILL_SCORE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_MOTION_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CHANGE_MOTION_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MOTION_ACK, KEGS_CHANGE_MOTION_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_MOTION_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MOTION_NOT, KEGS_CHANGE_MOTION_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_MOTION_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_TEAM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_CHANGE_TEAM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_TEAM_ACK, KEGS_CHANGE_TEAM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_CHANGE_TEAM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_TEAM_NOT, KEGS_CHANGE_TEAM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_TEAM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_READY_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
//#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_READY_REQ, EGS_CHANGE_READY_ACK );
//#else
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//#endif SERV_REPEAT_FILTER_REFAC
	//}}	

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	if( GetTradeUID() != 0 )
	{
		START_LOG( cerr, L"거래 중 레디 변경." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetTradeUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_CHANGE_READY_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_32;
		SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
		return;
	}

	if( GetRoomUID() != 0 )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				// 근성도 검사 - 던전id를 0으로 넣자 <-- 여기는 의미없는 코드이므로 차후에 삭제해야함.
				//if( CheckingSpirit( 0 ) == false )
				//{
				//	KEGS_CHANGE_READY_ACK kPacketAck;
				//	kPacketAck.m_iOK = NetError::ERR_ROOM_27;
				//	SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
				//	return;
				//}
			}
			break;

			//{{ 2010. 7. 28  최육사	대회 채널 예외 처리
#ifdef SERV_TOURNAMENT_CONDITION
		case CXSLRoom::RT_PVP:
			{
				int iPvpChannelClass = 0;
				LIF( GetPVPChannelClass( iPvpChannelClass ) );
				if( iPvpChannelClass == KPVPChannelInfo::PCC_NONE )
				{
					START_LOG( cerr, L"방리스트에서 방을 찾을 수 없습니다." )
						<< BUILD_LOG( GetRoomUID() )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					KEGS_CHANGE_READY_ACK kPacketAck;
					kPacketAck.m_iOK = NetError::ERR_ROOM_31;
					SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
					return;
				}
				else if( iPvpChannelClass == KPVPChannelInfo::PCC_TOURNAMENT )
				{
#ifdef SERV_MAIN_TOURNAMENT
					// 오프라인 티켓 없으면 대회채널 못들어감.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == false )
					{
						KEGS_CHANGE_READY_ACK kPacketAck;
						kPacketAck.m_iOK = NetError::ERR_ROOM_50;
						SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
						return;
					}
#else
					// 오프라인 티켓 있으면 대회채널 못들어감.
					if( m_kInventory.IsExistWithBank( CXSLItem::EI_OFFLINE_TICKET ) == true )
					{
						KEGS_CHANGE_READY_ACK kPacketAck;
						kPacketAck.m_iOK = NetError::ERR_ROOM_39;
						SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
						return;
					}
#endif SERV_MAIN_TOURNAMENT
				}
			}
			break;			
#endif SERV_TOURNAMENT_CONDITION
			//}}

		default:
			break;
		}
	}
	else
	{
		START_LOG( cerr, L"방에 입장해있는데 RoomUID가 없다?" )
			<< BUILD_LOG( GetRoomUID() )
			<< END_LOG;

		KEGS_CHANGE_READY_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ROOM_31;
		SendPacket( EGS_CHANGE_READY_ACK, kPacketAck );
		return;
	}

	SendToCnRoom( ERM_CHANGE_READY_REQ, kPacket_ );
}

IMPL_ON_FUNC( ERM_CHANGE_READY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	KEGS_CHANGE_READY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_CHANGE_READY_ACK, kPacket );

	//{{ 2008. 10. 29  최육사	에러 메시지 띄우기	
	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			switch( kPacket_.m_iDungeonID )
			{
			case SEnum::DI_ELDER_WALLY_CASTLE_LAB_NORMAL:
			case SEnum::DI_ELDER_WALLY_CASTLE_LAB_HARD:
			case SEnum::DI_ELDER_WALLY_CASTLE_LAB_EXPERT:
			case SEnum::DI_BESMA_SECRET_NORMAL:
			case SEnum::DI_BESMA_SECRET_HARD:
			case SEnum::DI_BESMA_SECRET_EXPERT:
			case SEnum::DI_ELDER_HALLOWEEN_NORMAL:
			case SEnum::DI_ELDER_HALLOWEEN_HARD:
			case SEnum::DI_ELDER_HALLOWEEN_EXPERT:
			case SEnum::DI_ALTERA_SECRET_NORMAL:
			case SEnum::DI_ALTERA_SECRET_HARD:
			case SEnum::DI_ALTERA_SECRET_EXPERT:
			case SEnum::DI_ELDER_NEWYEAR_NORMAL:
				{
					// 입장필요 아이템검사.
					if( !CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID, (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode ) )
					{
						KEGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT kPacketNot;
						kPacketNot.m_iOK = NetError::ERR_ROOM_41;
						SendPacket( EGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT, kPacketNot );
						return;
					}
				}
				break;
			}
		}
		break;
	}
	//}}	
}

_IMPL_ON_FUNC( ERM_CHANGE_READY_NOT, KEGS_CHANGE_READY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_READY_NOT, kPacket_ );
}

//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( EGS_CHANGE_PITIN_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_CHANGE_PITIN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PITIN_ACK, KEGS_CHANGE_PITIN_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_CHANGE_PITIN_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PITIN_NOT, KEGS_CHANGE_PITIN_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PITIN_NOT, kPacket_ );
}
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EGS_CHANGE_MAP_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_MAP_REQ, EGS_CHANGE_MAP_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetRoomUID() );

	SendToCnRoom( ERM_CHANGE_MAP_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MAP_ACK, KEGS_CHANGE_MAP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	SendPacket( EGS_CHANGE_MAP_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_MAP_NOT, KEGS_CHANGE_MAP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_WorldID );

	SendPacket( EGS_CHANGE_MAP_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_SLOT_OPEN_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_SLOT_OPEN_REQ, EGS_CHANGE_SLOT_OPEN_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_SLOT_OPEN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_ACK, KEGS_CHANGE_SLOT_OPEN_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_SLOT_OPEN_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_NOT, KEGS_CHANGE_SLOT_OPEN_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_SLOT_OPEN_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_PLAY_TIME_LIMIT_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_PLAY_TIME_LIMIT_REQ, EGS_CHANGE_PLAY_TIME_LIMIT_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PLAY_TIME_LIMIT_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PLAY_TIME_LIMIT_ACK, KEGS_CHANGE_PLAY_TIME_LIMIT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PLAY_TIME_LIMIT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PLAY_TIME_LIMIT_NOT, KEGS_CHANGE_PLAY_TIME_LIMIT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PLAY_TIME_LIMIT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_WINNING_NUM_KILL_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC 
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CHANGE_WINNING_NUM_KILL_REQ, EGS_CHANGE_WINNING_NUM_KILL_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_WINNING_NUM_KILL_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_WINNING_NUM_KILL_ACK, KEGS_CHANGE_WINNING_NUM_KILL_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_WINNING_NUM_KILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_WINNING_NUM_KILL_NOT, KEGS_CHANGE_WINNING_NUM_KILL_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_WINNING_NUM_KILL_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_LEAVE_GAME_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_LEAVE_GAME_REQ, EGS_LEAVE_GAME_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}    

	SendToCnRoom( ERM_LEAVE_GAME_REQ );
}

_IMPL_ON_FUNC( ERM_LEAVE_GAME_ACK, KEGS_LEAVE_GAME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_GAME_ACK, kPacket_ );
	//StateTransition( KGSFSM::I_TO_ROOM );

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::vector< int > vecPvpBuff;
	m_kUserBuffManager.GetBuffIDList( vecPvpBuff, CXSLRoom::RT_PVP );

	KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
	kReq.m_iUnitUID = GetCharUID();
	BOOST_TEST_FOREACH( int, iBuff, vecPvpBuff )
	{
		DeactivateBuff( iBuff, kReq.m_vecDeActivateBuff );
	}

	if( kReq.m_vecDeActivateBuff.empty() == false )
	{
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

_IMPL_ON_FUNC( ERM_LEAVE_GAME_NOT, KEGS_LEAVE_GAME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_GAME_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_END_GAME_PVP_RESULT_DATA_NOT, KEGS_END_GAME_PVP_RESULT_DATA_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
	bool bCouplePvp = false;

	if( m_bCouple == true )
	{
		for( int i = 0; i < (int)kPacket_.m_vecPVPUnitInfo.size(); ++i )
		{
			if( kPacket_.m_vecPVPUnitInfo[i].m_UnitUID == m_iRelationTargetUserUid )
			{
				bCouplePvp = true;
			}
		}
	}
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
	bool bHasFriend = false;

	// 추천인 있는지 우선 찾고
	std::vector< KRecommendUserInfo > vecRecruitUnitList;
	m_kUserRecommendManager.GetRecruitUnitList( vecRecruitUnitList );
	if( vecRecruitUnitList.empty() == true )
	{
		m_kUserRecommendManager.GetRecruiterUnitList( vecRecruitUnitList );
	}

	// 로그-테스트 후 지울 것
	START_LOG( clog, L"Recruit : Recruit Size!" )
		<< BUILD_LOG( vecRecruitUnitList.size() );
	BOOST_TEST_FOREACH( KRecommendUserInfo&, kRecruitUnitInfo, vecRecruitUnitList )
	{
		START_LOG( clog, L"Recruit : Recruit UID!" )
			<< BUILD_LOG( kRecruitUnitInfo.m_iUnitUID );
	}

	START_LOG( clog, L"Recruit : Party Size!" )
		<< BUILD_LOG( kPacket_.m_vecPVPUnitInfo.size() );
	BOOST_TEST_FOREACH( KPVPUnitResultInfo&, kPVPUnitResultInfo, kPacket_.m_vecPVPUnitInfo )
	{
		START_LOG( clog, L"Recruit : Party UID!" )
			<< BUILD_LOG( kPVPUnitResultInfo.m_UnitUID );
	}
	// 로그-테스트 후 지울 것

	BOOST_TEST_FOREACH( KRecommendUserInfo&, kRecruitUnitInfo, vecRecruitUnitList )
	{		
		UidType iRecruitUID = kRecruitUnitInfo.m_iUnitUID;
		BOOST_TEST_FOREACH( KPVPUnitResultInfo&, kPVPUnitResultInfo, kPacket_.m_vecPVPUnitInfo )
		{
			UidType iPVPUnitUID = kPVPUnitResultInfo.m_UnitUID;
			if( iRecruitUID == iPVPUnitUID )
			{
				bHasFriend = true;
			}
		}
	}
#endif SERV_RECRUIT_EVENT_SUBQUEST

	for( int i = 0; i < (int)kPacket_.m_vecPVPUnitInfo.size(); ++i )
	{
		//{{ 2012. 09. 18   김민성   2012 대전 시즌2 이벤트
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
		CTime tCurr = CTime::GetCurrentTime();
		if( kPacket_.m_bOfficialMatch == true && SiKGameSysVal()->IsPvpEventTime( tCurr ) == true )
		{
			kPacket_.m_vecPVPUnitInfo[i].m_nEXP = kPacket_.m_vecPVPUnitInfo[i].m_nEXP * 2;
			kPacket_.m_vecPVPUnitInfo[i].m_iAPoint = kPacket_.m_vecPVPUnitInfo[i].m_iAPoint * 2;
		}
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
		//}}

		//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
		// 대전에서 1킬이라도 하였다면(승패 상관 없음)
		if( GetCharUID() == kPacket_.m_vecPVPUnitInfo[i].m_UnitUID && kPacket_.m_vecPVPUnitInfo[i].m_nKillNum > 0 && kPacket_.m_bOfficialMatch == true )
		{
			CTime tCurr = CTime::GetCurrentTime();
			int iEventIndex = 0;

			std::wstring wstrTemp;
			bool bTemp = false;

			// 오늘 이벤트 시간인가?
			if( m_bIsPvpEventUser == true && SiKGameSysVal()->IsPvpEventTime( tCurr, iEventIndex, wstrTemp, bTemp ) == true )
			{
				// 보상 아이템 지급( 우편 )
				KDBE_PVP_WIN_EVENT_CHECK_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID = GetCharUID();
				kPacketToDB.m_iIndex   = iEventIndex;
				kPacketToDB.m_wstrRegDate = tCurr.Format(_T("%Y-%m-%d %H:%M:%S"));
				SendToGameDB( DBE_PVP_WIN_EVENT_CHECK_REQ, kPacketToDB );
			}
		}
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}

		if( GetCharUID() == kPacket_.m_vecPVPUnitInfo[i].m_UnitUID )
		{
			//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
			IncreasedLocalRankingPoint( 0, kPacket_.m_vecPVPUnitInfo[i].m_iAPoint );
#endif SERV_LOCAL_RANKING_SYSTEM
			//}}

			//{{ 2012. 09. 18	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
			// 공식 대전 플레이 카운트
			if( kPacket_.m_bOfficialMatch == true )
			{
				m_kUserQuestManager.Handler_OnPVPPlay_AccumulatedOfficialMatchCount( GetThisPtr<KGSUser>(), true );
			}
#endif SERV_2012_PVP_SEASON2
			//}}

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

			m_kUserQuestManager.Handler_OnPVPPlay( kPacket_.m_iGameType
												 , GetThisPtr<KGSUser>()
												 // 대전 플레이 퀘스트 조건 변경	- 김민성
#ifdef SERV_PVP_PLAY_QUEST
												 , kPacket_.m_bOfficialMatch
#endif SERV_PVP_PLAY_QUEST
												 // 대전 플레이 퀘스트 조건 변경 - 김민성
#ifdef PVP_QUEST_HERO_KILL_COUNT
												 , kPacket_.m_bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
												 , bCouplePvp
												 , kPacket_.m_bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
												 , bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST
												 );

			if( kPacket_.m_bIsDrawn == false )
			{
				//quest
				if( kPacket_.m_vecPVPUnitInfo[i].m_bWin == true )
				{
					m_kUserQuestManager.Handler_OnPVPWin( kPacket_.m_iGameType
														, GetThisPtr<KGSUser>()
														//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
														//{{ 2012. 02. 22	김민성	대전 퀘스트, PVP NPC 관련 퀘스트	오류 수정
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
														, kPacket_.m_bMatchNPC
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
														//}}
														, kPacket_.m_bOfficialMatch
#endif SERV_NEW_PVP_QUEST
														//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
														, kPacket_.m_bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
														);
				}
			}
			break;
		}
	}


	SendPacket( EGS_END_GAME_PVP_RESULT_DATA_NOT, kPacket_ );

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::vector< int > vecPvpBuff;
	m_kUserBuffManager.GetBuffIDList( vecPvpBuff, CXSLRoom::RT_PVP );

	KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
	kReq.m_iUnitUID = GetCharUID();
	BOOST_TEST_FOREACH( int, iBuff, vecPvpBuff )
	{
		DeactivateBuff( iBuff, kReq.m_vecDeActivateBuff );
	}

	if( kReq.m_vecDeActivateBuff.empty() == false )
	{
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

IMPL_ON_FUNC( ERM_UPDATE_PVP_UNIT_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_PVP_PLAY_TIME, kPacket_.m_iPlayTime / 60 );
#endif SERV_CHAR_LOG
	//}}
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PVP_Play_Time, kPacket_.m_iPlayTime );
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2012. 09. 18   김민성   2012 대전 시즌2 이벤트
#ifdef SERV_2012_PVP_SEASON2_NEW_EVENT
	CTime tCurr = CTime::GetCurrentTime();
	if( kPacket_.m_bIsOfficialMatch == true && SiKGameSysVal()->IsPvpEventTime( tCurr ) == true )
	{
		kPacket_.m_iEXP = kPacket_.m_iEXP * 2;
		kPacket_.m_iAPoint = kPacket_.m_iAPoint * 2;
	}
#endif SERV_2012_PVP_SEASON2_NEW_EVENT
	//}}

	//{{ 2009. 1. 20  최육사	만렙경험치 예외처리	
	m_kEXP.AddExp( kPacket_.m_iEXP );
	//}}
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	// 대전 결과 업데이트
	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	m_kUserPvpManager.UpdatePvpResult( kPacket_ );
#else
	m_kUserPvpManager.UpdatePvpResult( kPacket_.m_iRating, kPacket_.m_iRPoint, kPacket_.m_iAPoint, kPacket_.m_iWin, kPacket_.m_iLose, kPacket_.m_bIsOfficialMatch, kPacket_.m_bOutRoom );
#endif SERV_2012_PVP_SEASON2
	//}}

	//{{ 2012. 05. 30	최육사		공식 대전 버그 수정
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
	if( GetPartyUID() != 0 )
	{
		KEPM_UPDATE_PVP_INFO_NOT kPacketToPM;
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		kPacketToPM.m_cRank = GetPvpRankForClient();
		kPacketToPM.m_iOfficialMatchCnt	= GetOfficialMatchCount();
#endif SERV_2012_PVP_SEASON2
		//}}
		kPacketToPM.m_iRating = m_kUserPvpManager.GetRating();
		kPacketToPM.m_iRPoint = m_kUserPvpManager.GetRPoint();
		SendToCnParty( EPM_UPDATE_PVP_INFO_NOT, kPacketToPM );
	}
#endif SERV_OFFCIAL_PVP_BUG_FIX
	//}}
#else
	m_iVSPoint	+= kPacket_.m_iVSPoint;

	//페널티로 받는 값은 추가하지 않는다.
	if( kPacket_.m_iVSPoint > 0  )
	{
		m_iVSPointMax	+= kPacket_.m_iVSPoint;	 //맥스값은 게임이 끝나고 받는 업데이트 데이터만 받는다.
		m_ePVPEmblem	= CXSLUnit::GetPVPEmblem( m_iVSPointMax );
	}

	if( kPacket_.m_iWin > 1 && kPacket_.m_iLose > 1 )
	{
		START_LOG( cerr, L"승리결과 값이 동시에 2개가 셋팅되어 왔다.!" )
			<< BUILD_LOG( kPacket_.m_iWin )
			<< BUILD_LOG( kPacket_.m_iLose )
			<< END_LOG;
	}
	if( kPacket_.m_iWin > 1 )
	{
		START_LOG( cerr, L"WIN 포인트 이상.!" )
			<< BUILD_LOG( kPacket_.m_iWin )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket_.m_iWin = 1;
	}
	if( kPacket_.m_iLose > 1 )
	{
		START_LOG( cerr, L"LOSE 포인트 이상.!" )
			<< BUILD_LOG( kPacket_.m_iLose )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket_.m_iLose = 1;
	}

	m_iWin		+= kPacket_.m_iWin;
	m_iLose		+= kPacket_.m_iLose;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	CheckCharLevelUp();

	//{{ 2009. 11. 16  최육사	대전길드포인트
	if( GetGuildUID() > 0  &&  kPacket_.m_bOutRoom == false )
	{
		// 승 또는 패가 있다면 길드 경험치 준다 + 대전 플레이 타임이 20초 이상이어야함.
		if( ( kPacket_.m_iWin > 0  ||  kPacket_.m_iLose > 0 )  &&  kPacket_.m_iPlayTime > SEnum::ET_PVP_DRAWN_TIME )
		{
			m_kUserGuildManager.OnUpdatePvpUnitInfo( GetThisPtr<KGSUser>(), kPacket_.m_iMemberCount );

			KEGS_DECREASE_SPIRIT_NOT kNot;
			//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit(); // 현재 근성도값
#else
			kNot.m_iSpirit = m_iSpirit; // 현재 근성도값
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}			
			kNot.m_iHonorPoint = m_kUserGuildManager.GetHonorPoint(); // 현재 명예포인트

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
	//}}

	//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	m_kUserPetManager.Handler_ERM_UPDATE_PVP_UNIT_INFO_NOT();
#endif SERV_PET_SYSTEM
	//}}

	//유저에게 수정된 데이터를 넘겨준다.
	KEGS_UPDATE_UNIT_INFO_NOT kUpdatePacketNot;

	//아이템이 있으면 신규아이템에 대한 db처리를 할것인지 체크
	if( !kPacket_.m_mapItem.empty() )
	{
		KDBE_INSERT_ITEM_REQ kPacketReq;
		kPacketReq.m_iUnitUID = GetCharUID();
		//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
		kPacketReq.m_cGetItemReason = SEnum::GIR_PVP_RESULT;
#endif SERV_GET_ITEM_REASON
		//}}
#ifdef SERV_INT_ONLY
		m_kInventory.PrepareInsert( kPacket_.m_mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );
#else //SERV_INT_ONLY
		if ( false == m_kInventory.PrepareInsert( kPacket_.m_mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo, true ) ) 
        {
            typedef std::map< int, KItemInfo > KPVPItem;
            BOOST_TEST_FOREACH( const KPVPItem::value_type& , kItem, kPacket_.m_mapItem )
            {
                KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
                kPacketToDB.m_iFromUnitUID = GetCharUID();
                kPacketToDB.m_iToUnitUID   = GetCharUID();
                kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
                kPacketToDB.m_iRewardID	   = 10613;
                kPacketToDB.m_sQuantity    = kItem.second.m_iQuantity;
                SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

                START_LOG( cwarn, L"카밀라 코인을 우편으로 지급")
                    << BUILD_LOG( kPacket_.m_mapItem.size() )
                    << END_LOG;
            }
        }
#endif //SERV_INT_ONLY

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

			// 일반 아이템이라면 카운트
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

		// 아이템 어뷰저
		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_PVP, kPacketReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
		//}}
		//////////////////////////////////////////////////////////////////////////


		if( !kPacketReq.m_vecItemInfo.empty() )
		{
			m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );			
			m_kInventory.FlushEnduranceChange( kPacketReq.m_kItemEnduranceUpdate );
			m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
			kPacketReq.m_bOutRoom = kPacket_.m_bOutRoom;
			SendToGameDB( DBE_INSERT_ITEM_REQ, kPacketReq );
			return;
		}
		else
		{
			kUpdatePacketNot.m_mapItemObtained = kPacketReq.m_mapInsertedItem;
			std::map< int, int >::iterator mit;
			for( mit = kUpdatePacketNot.m_mapItemObtained.begin(); mit != kUpdatePacketNot.m_mapItemObtained.end(); ++mit )
			{
				START_LOG( clog2, L"획득 아이템." )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second );
			}
			kUpdatePacketNot.m_vecKInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
		}
	}

	//{{ 2011. 07. 29	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	// 공식대전인 경우 RoomListID를 강제로 세팅해주자!
	if( kPacket_.m_bIsOfficialMatch )
	{
        SetRoomListID( SiKRoomListManager()->GetPvpOffcialRoomListID() );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KRoomUserInfo kInfo;
	GetRoomUserInfo( kInfo );

	//방에서 나가는 경우가 아닐경우 센터서버에 업데이트를 시켜준다.
	if( kPacket_.m_bOutRoom == false )
	{
		SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
	}

	GetUnitInfo( kUpdatePacketNot.m_kUnitInfo );
	kUpdatePacketNot.m_kUnitInfo.m_nNowBaseLevelEXP		= SiKExpTable()->GetRequireTotalExpbyLevel( static_cast<int>(GetLevel()) );
	kUpdatePacketNot.m_kUnitInfo.m_nNextBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( static_cast<int>(GetLevel() + 1) );

	SendPacket( EGS_UPDATE_UNIT_INFO_NOT, kUpdatePacketNot );

	//{{ 2011. 07. 29	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	// 공식대전이 끝났다! RoomListID를 0으로 바꾸자!
	if( kPacket_.m_bIsOfficialMatch )
	{
		if( GetRoomListID() != SiKRoomListManager()->GetPvpOffcialRoomListID() )
		{
			START_LOG( cerr, L"공식 매치인데 RoomListID값이 이상합니다! 버그다!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetRoomListID() )
				<< BUILD_LOG( SiKRoomListManager()->GetPvpOffcialRoomListID() )
				<< END_LOG;

			// 그대로 진행!
		}

        SetRoomListID( 0 );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

IMPL_ON_FUNC_NOPARAM( ERM_DECREASE_ENDURANCE_PVP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// ItemTemplet에 설정된 내구도를 참조하여 깎는다
	m_kInventory.DecreaseEquippedItemEnduranceInPvp();
}

IMPL_ON_FUNC( ERM_PVP_USER_STATISTICS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iGamePlayTime < 0 )
		kPacket_.m_iGamePlayTime = 0;

	// 유저 플레이 타임
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_TOTAL_PLAY_TIME, kPacket_.m_iGamePlayTime );
	m_kUserStatistics.AddPlayTime( KUserStatistics::PT_PVP_PLAY_TIME, kPacket_.m_iGamePlayTime );	

	// 유저 통계 대전
	switch( kPacket_.m_cGameResult )
	{
	case KERM_PVP_USER_STATISTICS_NOT::UGR_WIN:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Win, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Win, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_PVP_USER_STATISTICS_NOT::UGR_LOSE:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount,	static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Lose, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Lose, kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_PVP_USER_STATISTICS_NOT::UGR_DRAW:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Draw, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Draw,	kPacket_.m_iGamePlayTime );
		}
		break;

	case KERM_PVP_USER_STATISTICS_NOT::UGR_DROP:
		{
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Drop, 1 );
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Drop, kPacket_.m_iGamePlayTime );
		}
		break;

	default:
		START_LOG( cerr, L"이런 결과 타입은 없음." )
			<< BUILD_LOGc( kPacket_.m_cGameResult )
			<< END_LOG;
		break;
	}

	// 유저 통계 난입
	if( kPacket_.m_bIntrude == true )
	{				
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpModeCount, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_MC_Intrude, 1 );
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_PvpPlayTime, static_cast<int>(kPacket_.m_cGameType), KUserStatistics::US_Pvp_PT_Intrude, kPacket_.m_iGamePlayTime );
	}

	// 유저 통계 대전 킬수
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Pvp_Kill_Count, kPacket_.m_iKillNum );

	// 대전 맵 통계
	KStatisticsKey kKeyPvpMap;
	kKeyPvpMap.m_vecIntKey.push_back( kPacket_.m_sWorldID );
	KSIManager.IncreaseCount( KStatistics::SI_PVP_MAP, kKeyPvpMap, KStatistics::eSIColPvpMap_PlayCount, 1 );
}

#ifdef DUNGEON_ITEM
IMPL_ON_FUNC( EGS_CREATE_PVP_ITEM_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CREATE_PVP_ITEM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CREATE_PVP_ITEM_ACK, KEGS_CREATE_PVP_ITEM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CREATE_PVP_ITEM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CREATE_PVP_ITEM_NOT, KEGS_CREATE_PVP_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CREATE_PVP_ITEM_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEM_NOT, KEGS_CHANGE_PVP_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PVP_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_CHANGE_PVP_ITEMMODE_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CHANGE_PVP_ITEMMODE_REQ );

}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEMMODE_ACK, KEGS_CHANGE_PVP_ITEMMODE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PVP_ITEMMODE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEMMODE_NOT, KEGS_CHANGE_PVP_ITEMMODE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PVP_ITEMMODE_NOT, kPacket_ );
}
#endif DUNGEON_ITEM

//}}
//////////////////////////////////////////////////////////////////////////

#ifdef SERVER_PVP_BASE_DEFENCE_TEST
IMPL_ON_FUNC( EGS_CHANGE_PVP_GAME_OPTION_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PVP_GAME_OPTION_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_GAME_OPTION_ACK, KEGS_CHANGE_PVP_GAME_OPTION_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_GAME_OPTION_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_GAME_OPTION_NOT, KEGS_CHANGE_PVP_GAME_OPTION_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_GAME_OPTION_NOT, kPacket_ );
}
#endif SERVER_PVP_BASE_DEFENCE_TEST


//{{ 2012. 02. 14    김민성   PVP 대전 Item 생성 위치 변경
#ifdef SERV_PVP_ITEM_CHANGE_POS
IMPL_ON_FUNC( EGS_CHANGE_PVP_ITEM_POS_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_CHANGE_PVP_ITEM_POS_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEM_POS_ACK, KEGS_CHANGE_PVP_ITEM_POS_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ITEM_POS_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ITEM_POS_NOT, KEGS_CHANGE_PVP_ITEM_POS_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ITEM_POS_NOT, kPacket_ );
}
#endif SERV_PVP_ITEM_CHANGE_POS
//}}

//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
IMPL_ON_FUNC( DBE_PVP_WIN_EVENT_CHECK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
    
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 보상을 받았으므로 이벤트 대상에서 제외
		m_bIsPvpEventUser = false;
	}
}
#endif SERV_2012_PVP_SEASON2_EVENT
//}}

#ifdef SERV_NEW_PVPROOM_PROCESS
_IMPL_ON_FUNC( EGS_CHANGE_PVP_ROOM_PUBLIC_REQ, KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PVP_ROOM_PUBLIC_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_PUBLIC_ACK, KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_PUBLIC_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_PUBLIC_NOT, KEGS_CHANGE_PVP_ROOM_PUBLIC_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_PUBLIC_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGS_CHANGE_PVP_ROOM_NAME_REQ, KEGS_CHANGE_PVP_ROOM_NAME_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrRoomName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrRoomName.c_str(), L'♡' );
#endif //SERV_STRING_FILTER_USING_DB

	START_LOG_WITH_NAME( clog );

	SendToCnRoom( ERM_CHANGE_PVP_ROOM_NAME_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_NAME_ACK, KEGS_CHANGE_PVP_ROOM_NAME_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_NAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGE_PVP_ROOM_NAME_NOT, KEGS_CHANGE_PVP_ROOM_NAME_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	START_LOG_WITH_NAME( clog );

	SendPacket( EGS_CHANGE_PVP_ROOM_NAME_NOT, kPacket_ );
}
#endif SERV_NEW_PVPROOM_PROCESS

#ifdef SERV_PVP_REMATCH
IMPL_ON_FUNC( EGS_PVP_REMATCH_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_PVP_REMATCH_REQ, kPacket_ );
}
_IMPL_ON_FUNC( ERM_PVP_REMATCH_ACK, KEGS_PVP_REMATCH_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PVP_REMATCH_ACK, kPacket_ );
}
_IMPL_ON_FUNC( ERM_PVP_REMATCH_NOT, KEGS_PVP_REMATCH_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PVP_REMATCH_NOT, kPacket_ );
}
#endif SERV_PVP_REMATCH

//{{ 2012. 10. 10	박세훈	튕긴 유저 마을로 보내기
#ifdef UDP_CAN_NOT_SEND_USER_KICK
_IMPL_ON_FUNC( ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT, UidType )
//IMPL_ON_FUNC_NOPARAM( ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_UDP_KICK_STATE_CHANGE_FIELD_NOT kPacket;
	kPacket.m_iAccusationUnitUID = kPacket_;
	SendPacket( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT, kPacket );
	//SendID( EGS_UDP_KICK_STATE_CHANGE_FIELD_NOT );
}

#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


