//{{ 2013. 2. 6	박세훈	랜선렉 방지 코드2
//#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	#include <WinSock2.h>	// windows.h redefinition 오류 대처용
//#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

#include "Room.h"
#include "Room/RoomFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLDungeonManager.h"

//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "GameSysVal/GameSysVal.h"
#endif SERV_CHOOSE_FASTEST_HOST
//}}

//{{ 2013. 2. 6	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	#include "InteriorUdpSession.h"
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

float KRoom::ms_fPlayTimeNotifyGap = 5.f;

#define CLASS_TYPE KRoom

NiImplementRTTI( KRoom, KCnBaseRoom );

ImplToStringW( KRoom )
{
    return START_TOSTRING_PARENTW( KCnBaseRoom );
}

KRoom::KRoom()
{
	//{{ 2011. 11. 7	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//	if( m_spRoomUserManager == NULL )
//	{
//		START_LOG( cerr, L"RoomUserManager객체가 생성되지 않았습니다! 위험합니다!" )
//			<< BUILD_LOG( GetUID() )
//			<< END_LOG;
//	}
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	EnableRemainingPlaytime( true );

	m_cGetItemType		= CXSLRoom::DGIT_NONE;
	m_cDungeonMode		= static_cast<char>(CXSLDungeon::DM_INVALID);

#ifdef DUNGEON_ITEM
	m_bIsItemMode = false;
#endif DUNGEON_ITEM

	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_usEventIDForTimeCount = 0;
	m_cCurrentTimeCount = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

//#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//	m_bForceHost = false;
//#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_bCheckPingScoreExecuted = false;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

}

KRoom::~KRoom()
{
}

void KRoom::Tick()
{
    KCnBaseRoom::Tick();

	CXSLRoom::ROOM_TYPE eRoomType;

    switch( GetStateID() )
    {
    case KRoomFSM::S_INIT:
        break;

    case KRoomFSM::S_CLOSE:
		{
			//{{ 2012. 06. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			CheckReserveRegroupOldParty(); // 혹시 방이 닫히기 전까지 전송 안되었을까봐 여기서 한번 체크함.
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			eRoomType = static_cast<CXSLRoom::ROOM_TYPE>(m_cRoomType); // 방 닫히기 전에 저장
			m_spRoomUserManager->ZU_CheckEnd();
			CloseRoom();
			//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
			SendRoomCountInfo( eRoomType, false );
#endif SERV_ROOM_COUNT
			//}}
		}
        break;

    case KRoomFSM::S_WAIT:
        break;

		//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case KRoomFSM::S_TIME_COUNT:
		{
			CheckTimeCount();
		}
		break;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

    case KRoomFSM::S_LOAD:
		{
			CheckGameLoadingTimeOut();
		}
        break;

    case KRoomFSM::S_PLAY:
		{
			CheckRemainingPlayTime();
			CheckGameLoadingTimeOut();
			//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
			CheckCaughtBuffSkill();
#endif SERV_ADD_BUFF_SKILL_INFO
			//}}
			//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			//if( m_bForceHost )
            if ( m_spRoomUserManager->GetForceHostCID() != 0 )
			{
				CheckPingScore();
				m_kTimer[TM_CHECK_CHANGE_HOST].restart();
			}
			else
#endif  SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
            if( m_kTimer[TM_CHECK_CHANGE_HOST].elapsed() >  ( ( m_cRoomType == CXSLRoom::RT_BATTLE_FIELD ) 
                    ? SiKGameSysVal()->GetBattleFieldCheckChangeHostTime()
                    : SiKGameSysVal()->GetCheckChangeHostTime() ) )
#else   SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
			if( m_kTimer[TM_CHECK_CHANGE_HOST].elapsed() > SiKGameSysVal()->GetCheckChangeHostTime() )
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
			{
				CheckPingScore();
				m_kTimer[TM_CHECK_CHANGE_HOST].restart();
			}
#endif SERV_CHOOSE_FASTEST_HOST
			//}
		}
        break;

	case KRoomFSM::S_RESULT:
		break;

		//{{ 2012. 06. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	case KRoomFSM::S_RETURN_TO_FIELD:
		{
			// 현재 복귀 시도중인 멤버가 3초가 지나도 돌아오지 않는다면 해당 멤버는 강제 이탈 시키고 다음사람을 이탈 시도 하자!
            if( m_kTimer[TM_RETURN_TO_FIELD].elapsed() > 3.0 )
			{
				m_kTimer[TM_RETURN_TO_FIELD].restart();
                
				// 해당 멤버는 강제이탈!
				LIF( m_spRoomUserManager->LeaveRoom( m_spRoomUserManager->GetSendReturnToFieldUnitUID() ) ); // ---> leave room하고나서 남은 사람 없으면 방정리 해야하지 않을까?

				// 패킷 정리
				KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
				GetRoomInfo( kNot.m_RoomInfo );
				GetRoomSlotInfo( kNot.m_vecSlot );

				// 첫번째 멤버부터 이탈 시도!
				UidType iGSUID = 0;
				UidType iNextUnitUID = 0;
				if( m_spRoomUserManager->GetNextReturnToFieldUnitInfo( GetUID(), iGSUID, iNextUnitUID, kNot.m_kReturnToFieldInfo ) == false )
				{
					// 더이상 이탈 처리할 유저가 없으면 여기서 끝!!
					StateTransition( KRoomFSM::I_TO_WAIT );

					// 옛날 파티 재정렬!
					CheckReserveRegroupOldParty();
					break;
				}

				// 전송!
				SendToGSCharacter( iGSUID, iNextUnitUID, ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );
			}
		}
		break;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	case KRoomFSM::S_WAIT_FOR_DEFENCE:
		break;
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

    default:
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << END_LOG;
        break;
    }
	
	if( m_cRoomType != CXSLRoom::RT_DUNGEON )
	{
		std::vector<UidType> vecZombieUnit;
		if( m_spRoomUserManager->ZU_Tick( static_cast<CXSLRoom::ROOM_TYPE>(m_cRoomType), vecZombieUnit ) == true )
		{
			DeleteZombie( vecZombieUnit );

			START_LOG( cout2, L"좀비유저 삭제.!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( vecZombieUnit.size() )
				<< BUILD_LOGc( m_cRoomType )
				<< END_LOG;
		}
	}

    std::vector< UidType > vecUID;
    m_spRoomUserManager->ExpireTradeRequest( vecUID );
    if( !vecUID.empty() )
    {
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUID )
        {
            // 방의 모든 유저에게 바뀐 정보를 날린다.
            KEGS_CHANGE_TRADE_NOT kNot;
            kNot.m_iUnitUID = iUnitUID;
            if( m_spRoomUserManager->IsInTrade( iUnitUID, kNot.m_bTrade ) )
            {
                BroadCast( ERM_CHANGE_TRADE_NOT, kNot );
            }
            else
            {
                START_LOG( cerr, L"거래 상태 확인 실패." )
                    << BUILD_LOG( iUnitUID )
                    << END_LOG;
            }

            START_LOG( cout2, L"거래 신청 삭제." )
                << BUILD_LOG( iUnitUID )
                << END_LOG;
        }
    }
}

void KRoom::GetRoomInfo( OUT KRoomInfo& kInfo )
{
    kInfo.m_RoomType		= m_cRoomType;
    kInfo.m_RoomUID			= GetUID();
    kInfo.m_uiRoomListID	= m_uiRoomListID;
    kInfo.m_RoomName		= GetName();
    kInfo.m_RoomState		= GetStateID();
    kInfo.m_bPublic			= m_bPublic;
    kInfo.m_bTeamBalance	= m_bTeamBalance;
    kInfo.m_wstrPassword	= m_wstrPassword;
    kInfo.m_MaxSlot			= GetNumOpenSlot();
    kInfo.m_JoinSlot		= GetNumOccupiedSlot();
    kInfo.m_wstrUDPRelayIP	= m_wstrUDPRelayIP;
    kInfo.m_usUDPRelayPort	= m_usUDPRelayPort;
    kInfo.m_PVPGameType		= m_cGameType;
    kInfo.m_WinKillNum		= m_cWinningNumKill;
    kInfo.m_fPlayTime		= GetPlayTimeLimit();
    kInfo.m_WorldID			= m_sWorldID;	
	kInfo.m_ShowPvpMapWorldID = m_sShowPvpMapWorldID;
    kInfo.m_DifficultyLevel	= m_cDifficultyLevel;
    kInfo.m_iDungeonID		= m_iDungeonID;
	kInfo.m_bCanIntrude		= m_bCanIntrude;
	kInfo.m_cGetItemType	= m_cGetItemType;
	kInfo.m_cDungeonMode	= m_cDungeonMode;
#ifdef DUNGEON_ITEM
	kInfo.m_bIsItemMode		= m_bIsItemMode;
#endif DUNGEON_ITEM

#ifdef SERV_PVP_REMATCH
	kInfo.m_mapAllPlayersSelectedMap = m_mapAllPlayersSelectedMap;
#endif SERV_PVP_REMATCH
}

//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KRoom::TimeCountForStartGame( IN const u_short usEventID )
{
	if( GetStateID() != KRoomFSM::S_WAIT )
	{
		START_LOG( cerr, L"S_WAIT상태가 아닌데 이 함수가 호출되었다! 이건 잘못된거야!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		// 그냥 로그만 찍자!
	}

	// 어떤 패킷으로 시작되었는지 나중에 확인하기 위해 저장한다!
	m_usEventIDForTimeCount = usEventID;

	// 최대 카운트!
	SetCurrentTimeCount( RE_TIME_COUNT_LIMIT );

	// 상태 이동 및 타이머 초기화!
	StateTransition( KRoomFSM::I_TO_TIME_COUNT );
	m_kTimer[TM_TIME_COUNT].restart();

	// 3초 타이머가 시작되었다는 패킷을 전송하자!
	KEGS_GAME_START_TIME_COUNT_NOT kPacketNot;
	kPacketNot.m_cSecondCount = GetCurrentTimeCount();
	kPacketNot.m_bIsDungeonRoom = ( m_cRoomType == CXSLRoom::RT_DUNGEON );
	BroadCast( ERM_GAME_START_TIME_COUNT_NOT, kPacketNot );
}

void KRoom::CheckTimeCount()
{
	if( m_kTimer[TM_TIME_COUNT].elapsed() > 1.0 )
	{
		m_kTimer[TM_TIME_COUNT].restart();

		// 1초 감소 시키자!
		DecreaseCurrentTimeCount();

		// 더이상 감소할 값이 없다면 여기서 끝내자!
		if( GetCurrentTimeCount() <= 0 )
		{
			// 상태 이동하고
			StateTransition( KRoomFSM::I_TO_WAIT );
			
			// 방 타입에 따른 처리!
			TimeCountFinishProcess( m_usEventIDForTimeCount );
            return;
		}

		// 3초 타이머가 시작되었다는 패킷을 전송하자!
		KEGS_GAME_START_TIME_COUNT_NOT kPacketNot;
		kPacketNot.m_iRoomUID = GetUID();
		kPacketNot.m_cSecondCount = GetCurrentTimeCount();
#ifdef FIX_DUNGEON_START_STRING
		kPacketNot.m_bIsDungeonRoom = ( m_cRoomType == CXSLRoom::RT_DUNGEON );
#endif //FIX_DUNGEON_START_STRING
		BroadCast( ERM_GAME_START_TIME_COUNT_NOT, kPacketNot );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KRoom::ProcessEvent( const KEventPtr& spEvent_ )
{
	m_spRoomUserManager->ZU_Refresh( spEvent_->m_anTrace[0], spEvent_->m_usEventID );	// 080114. 좀비유저 티타임을 갱신한다.

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
    CASE( ERM_JOIN_ROOM_REQ );
   _CASE( ERM_BAN_USER_REQ, KEGS_BAN_USER_REQ );
   _CASE( ERM_CHANGE_READY_REQ,	KEGS_CHANGE_READY_REQ );
   _CASE( ERM_CHANGE_PITIN_REQ,	KEGS_CHANGE_PITIN_REQ );
   _CASE( ERM_CHANGE_SLOT_OPEN_REQ, KEGS_CHANGE_SLOT_OPEN_REQ );
   _CASE( ERM_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ );
   _CASE( ERM_STATE_CHANGE_RESULT_REQ, KEGS_STATE_CHANGE_RESULT_REQ );
   _CASE( ERM_CHAT_REQ, KEGS_CHAT_REQ );
   _CASE( ERM_CHANGE_MOTION_REQ, KEGS_CHANGE_MOTION_REQ );
   _CASE( ERM_CHANGE_PLAY_TIME_LIMIT_REQ, KEGS_CHANGE_PLAY_TIME_LIMIT_REQ );

   _CASE( ERM_SET_UNIT_INFO_NOT, KRoomUserInfo );
    CASE_NOPARAM( ERM_REFRESH_ROOM_REQ );
    CASE( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT );
	//{{ 2009. 5. 21  최육사	내구도 소모 개편
	CASE( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT );
	//}}
	//{{ 2012. 10. 17	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
   _CASE( ERM_RESULT_SUCCESS_REQ, KEGS_RESULT_SUCCESS_REQ );
#else
	CASE_NOPARAM( ERM_RESULT_SUCCESS_REQ );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

   _CASE( ERM_ADD_ON_STAT_REQ, KEGS_ADD_ON_STAT_REQ );

   _CASE( ERM_REPAIR_ITEM_NOT, KEGS_REPAIR_ITEM_NOT );

    CASE( ERM_REQUEST_TRADE_NOT );
    CASE( ERM_REQUEST_TRADE_REPLY_NOT );

    CASE_NOPARAM( ERM_TRADE_START_NOT );
    CASE_NOPARAM( ERM_TRADE_FINISH_NOT );

	CASE( ERM_SET_QUEST_ITEM_INFO_NOT );

	//{{ 2008. 4. 2  최육사  근성도 회복 아이템
   _CASE( ERM_RESTORE_SPIRIT_NOT, KEGS_RESTORE_SPIRIT_NOT );
	//}}

	//{{ 2009. 5. 8  최육사		실시간 레벨업
	CASE( ERM_CHAR_LEVEL_UP_NOT );
	//}}
	//{{ 2009. 9. 23  최육사	1차전직캐쉬템
   _CASE( ERM_CHANGE_ROOM_USER_INFO_NOT, KRoomUserInfo );
	//}}

	//{{ 2009. 11. 30  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	CASE( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT );
#endif GUILD_SKILL_TEST
	//}}

#ifdef DUNGEON_ITEM
	CASE_NOPARAM( ERM_CHANGE_PVP_ITEMMODE_REQ );
#endif DUNGEON_ITEM

	//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
   _CASE( ERM_SEARCH_UNIT_REQ, KELG_SEARCH_UNIT_REQ );
   _CASE( ERM_GET_UNIT_INFO_ACK, KELG_GET_UNIT_INFO_ACK );
#endif SERV_INTEGRATION
	//}}

	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	CASE( ERM_SUMMON_PET_NOT );
	CASE( ERM_PET_ACTION_NOT );
   _CASE( ERM_CHANGED_PET_SATIETY_NOT, KEGS_CHANGED_PET_SATIETY_NOT );
   _CASE( ERM_CHANGED_PET_INTIMACY_NOT, KEGS_CHANGED_PET_INTIMACY_NOT );
   _CASE( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT );
	CASE( ERM_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
	_CASE( ERM_BUFF_SKILL_INFO_CONVEY_REQ, KEGS_BUFF_SKILL_INFO_CONVEY_REQ );
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}
	//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	_CASE( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	_CASE( ERM_USE_SKILL_BUFF_IN_PLAY_NOT, KEGS_USE_SKILL_BUFF_IN_PLAY_NOT );
	_CASE( ERM_NEXT_RETURN_TO_FIELD_NOT, KERM_COMPLETE_RETURN_TO_FIELD_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	_CASE( ERM_UPDATE_USER_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_USER_UNIT_BUFF_INFO_NOT );
	_CASE( ERM_UPDATE_NPC_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT );
#endif SERV_SERVER_BUFF_SYSTEM
	//}}
	//{{ 2012. 07. 27	최육사		사제시스템 EXP 보너스 버그 수정
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
	CASE( ERM_UPDATE_TUTORIAL_INFO_NOT );
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
	//}}
	//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
	_CASE( ERM_VISIT_CASH_SHOP_NOT, KEGS_VISIT_CASH_SHOP_NOT );
#endif SERV_VISIT_CASH_SHOP
	//}}
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	_CASE( ERM_UDP_PING_SCORE_NOT, KEGS_UDP_PING_SCORE_NOT );
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
	_CASE( ERM_FIELD_USER_FRAME_INFO_NOT, int );
#endif SERV_FIELD_FRAME_LOG
	//}}

	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	_CASE( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, KRoomUserInfo );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-08
	CASE( ERM_LOCAL_RANKING_WATCH_UNIT_REQ );
	CASE( ERM_LOCAL_RANKING_WATCH_UNIT_ACK );
#endif	// SERV_LOCAL_RANKING_SYSTEM
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	_CASE( ERM_COUPLE_MAKING_SUCCESS_NOT, KERM_COUPLE_MAKING_SUCCESS_NOT );
	CASE_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT );
	_CASE( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	_CASE( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT );
	_CASE( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM

	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	_CASE( ERM_FORCE_RELAY_NOT, KEGS_FORCE_RELAY_NOT );
	_CASE( ERM_ADD_UDP_CHECK_KICK_USER_NOT, UidType );
	_CASE( ERM_REMOVE_LANBUG_CHECK_USER_NOT, UidType );
	_CASE( ERM_REMOVE_LANBUG_VERIFY_USER_NOT, UidType );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

    default:
        ProcessNativeEvent( spEvent_ );
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

void KRoom::CheckRemainingPlayTime()
{
    if( GetRemainingPlayTime() <= 0.f || m_bEnableRemainingPlayTime == false )
    {
        return;
    }

    if( m_kTimer[TM_PLAY_TIME_NOT].elapsed() >= ms_fPlayTimeNotifyGap )
    {
		//던전에서는 GetRemainingPlayTime()함수를 오버로딩하여 로딩시간을 빼주기때문에
		//SetRemainingPlayTime() 함수 호출시 m_fRemainingPlayTime 변수를 직접호출하여 빼준다.
        SetRemainingPlayTime( m_fRemainingPlayTime - ( float )m_kTimer[TM_PLAY_TIME_NOT].elapsed() ); 
        if( GetRemainingPlayTime() <= 0.f )
        {
            START_LOG( clog2, L"플레이 타임 아웃." )
                << BUILD_LOG( GetStateID() )
                << BUILD_LOG( GetStateIDString() );
            BroadCastID( ERM_PLAY_TIME_OUT_NOT );
        }
        else
        {
            m_kTimer[TM_PLAY_TIME_NOT].restart();
            BroadCast( ERM_REMAINING_PLAY_TIME_NOT, GetRemainingPlayTime() );
        }
    }
}

void KRoom::CheckGameLoadingTimeOut()
{
    std::map< UidType, UidType > mapUserCIDNGS;
    m_spRoomUserManager->GetLoadingTimeOutUser( mapUserCIDNGS );

    if( !mapUserCIDNGS.empty() )
    {
        HandleTimeOutUser( mapUserCIDNGS );
    }
}

void KRoom::CheckResultSuccess()
{
	if( GetStateID() != KRoomFSM::S_RESULT )
		return;

	//결과 상태인 유저가 결과창 확인이 끝났으면
	if( m_spRoomUserManager->IsAllPlayerSuccessResult() == true )
	{
		EndGame();

		KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		GetRoomInfo( kNot.m_RoomInfo );
		GetRoomSlotInfo( kNot.m_vecSlot );

		//결과처리까지 끝나고 서야 방에 대기중인 유저에게까지 패킷을 보낸다.
		BroadCast( ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );

		//결과종료 처리 되었으므로 방정보 업데인트
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  최육사	대전유저리스트		
		SendAllPVPUserInfo();
		//}}
	}
}

void KRoom::EnableRemainingPlaytime( bool bEnable )
{
	if( bEnable == true )
	{
		m_bEnableRemainingPlayTime = bEnable;
		m_kTimer[TM_PLAY_TIME_NOT].restart();
	}
	else
	{
		m_bEnableRemainingPlayTime = bEnable;
	}
}

//{{ 2011. 08. 05	최육사	대전방에 유저 혼자 남게되는 버그 수정
#ifdef SERV_CODE_REFACTORING_20110804
bool KRoom::CheckAllPlayerFinishLoading()
{
	// 모든 유저가 로딩 끝났으면 not를 날린다.
	if( m_spRoomUserManager->IsAllPlayerFinishLoading() )
	{
		BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

		// 스테이지를 초기화.
		StartPlay();

		KEGS_PLAY_START_NOT kNot;
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		kNot.m_iDungeonID = GetDungeonIDAndDif();
#endif SERV_CHAR_LOG
		//}}
		kNot.m_RoomState = GetStateID();
		GetRoomSlotInfo( kNot.m_vecSlot );

		// 모든 유저에게 플레이 시작을 알림.
		BroadCast( ERM_PLAY_START_NOT, kNot );

		// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  최육사	대전유저리스트
		SendAllPVPUserInfo();
		//}}
		return true;
	}

	return false;
}
#endif SERV_CODE_REFACTORING_20110804
//}}

void KRoom::StartGame()
{
	//좀비유저 체크정지
	m_spRoomUserManager->ZU_CheckEnd();

    SetRemainingPlayTime( -1.f );
    m_spRoomUserManager->StartGame();
#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// 작업날짜: 2013-06-10	// 박세훈
	BroadCast( ERM_UPDATE_BUFF_INFO_NOT, char(), KRoomUserManager::UL_ALL );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

    StateTransition( KRoomFSM::I_TO_LOAD );
    m_kTimer[TM_PLAY].restart();


}

void KRoom::StartPlay()
{
	m_bEndPlay = false;
    SetRemainingPlayTime( GetPlayTimeLimit() );
    m_spRoomUserManager->StartPlay();
    m_kTimer[TM_PLAY_TIME_NOT].restart();
	//{{ 2011. 07. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_kTimer[TM_PLAY_WITHOUT_LOADING].restart();
#endif SERV_PVP_NEW_SYSTEM
	//}}
    StateTransition( KRoomFSM::I_TO_PLAY );



}

void KRoom::EndPlay()
{
	//{{ 2013. 2. 7	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	BOOST_TEST_FOREACH( const UidType, iUnitUID, m_setRanBugCheckList )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_CHECK_CANCLE_NOT, GetUID(), iUnitUID );
	}
	ClearLanBugCheckUser();

	BOOST_TEST_FOREACH( const UidType, iUnitUID, m_setRanBugVerifyList )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_CANCLE_NOT, GetUID(), iUnitUID );
	}
	ClearLanBugVerifyUser();
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

	KEGS_END_GAME_NOT kPacket;
	kPacket.m_cRoomState = GetStateID();
	kPacket.m_iWinTeam = DecideWinTeam();
	m_spRoomUserManager->GetRoomSlotInfo( kPacket.m_vecSlot );

	//게임중 나간 유저에게는 보내지 않는다.
	// #1 결과패킷처리로 넘어가지 않게 하기위해
	// #2 방에있는유저에게(PVP 난입시) 결과처리까지 끝내고 서야 방정보를 주기위해
	BroadCast( ERM_END_GAME_NOT, kPacket, KRoomUserManager::UL_PLAY );

    m_spRoomUserManager->EndPlay();
    StateTransition( KRoomFSM::I_TO_RESULT );

	SendRoomListInfo( NetError::ERR_ROOM_15 );

	//{{ 2009. 4. 23  최육사	대전유저리스트	
	SendAllPVPUserInfo();
	//}}
}

void KRoom::EndGame()
{
	m_spRoomUserManager->EndGame();

	StateTransition( KRoomFSM::I_TO_WAIT );
    m_kTimer[TM_WAIT].restart();

	//좀비유저 체크시작.
	m_spRoomUserManager->ZU_CheckStart();
}

bool KRoom::CheckIfPlayEnd()
{
    return false;
}

bool KRoom::IsAbleToIntrude()
{
    return false;
}

void KRoom::DeleteZombie( std::vector<UidType>& vecZombieUnit )
{
	START_LOG( cout2, L"삭제될 좀비 유저개수.!" )
		<< BUILD_LOG( vecZombieUnit.size() )
		<< END_LOG;

	std::vector<UidType>::iterator vit;
	for( vit = vecZombieUnit.begin(); vit != vecZombieUnit.end(); ++vit )
	{
		KRoomUserPtr spUser = m_spRoomUserManager->GetUser( *vit );

		if( spUser != NULL )
		{
			KERM_CHECK_INVALID_ZOMBIE_USER_NOT kPacket;
			kPacket.m_iRoomUID = GetUID();

			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_INVALID_ZOMBIE_USER_NOT, kPacket );

			//{{ 2009. 4. 23  최육사	대전유저리스트
			SendPVPUserInfo( spUser->GetCID(), KCommunityUserInfo::US_PVP_LOBBY );
			//}}
		}
		else
		{
			START_LOG( cerr, L"룸유저 포인터 이상.!" )
				<< BUILD_LOG( *vit )
				<< END_LOG;
		}

		if( !m_spRoomUserManager->LeaveRoom( *vit ) )
		{
			START_LOG( cerr, L"방 나가기 실패." );

			continue;
		}
	}

	// 방에서 모든 유저가 나갔으면 방을 닫는다.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		std::vector<UidType>::iterator vit;
		for( vit = vecZombieUnit.begin(); vit != vecZombieUnit.end(); ++vit )
		{
			// 방의 모든 유저들에게 슬롯 정보를 날린다.
			KEGS_LEAVE_ROOM_NOT kPacketNot;
			kPacketNot.m_cRoomState	= GetStateID();
			kPacketNot.m_iUnitUID	= *vit;
			kPacketNot.m_iReason	= NetError::NOT_LEAVE_ROOM_REASON_02;	//강퇴당함으로 설정.
			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
		}
	}

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	if( GetStateID() == KRoomFSM::S_CLOSE )
	{
		SendRoomListInfo( NetError::ERR_ROOM_16 );
	}
	else
	{
		SendRoomListInfo( NetError::ERR_ROOM_15 );
	}
}

bool KRoom::IsEmpty( KRoomUserManager::USERLIST_TYPE eType )
{
    return m_spRoomUserManager->GetNumOccupiedSlot( eType ) == 0;
}

bool KRoom::IsFull()
{
    return m_spRoomUserManager->GetNumOpenedSlot() == m_spRoomUserManager->GetNumOccupiedSlot();
}

bool KRoom::IsHost( UidType nCID )
{
    return m_spRoomUserManager->IsHost( nCID );
}

void KRoom::BroadCastID( u_short usEventID, int iOption /*= BC_ALL */)
{
    BroadCast( usEventID, char(), iOption );
}

void KRoom::BroadCastRoomSlotInfo()
{
    BroadCastRoomSlotInfo( ERM_ROOM_SLOT_INFO_NOT );
}

void KRoom::BroadCastRoomSlotInfo( u_short usEventID )
{
    std::vector< KRoomSlotInfo > kPacket;
    GetRoomSlotInfo( kPacket );
    BroadCast( usEventID, kPacket );
}

void KRoom::SendRoomListInfo( IN const int iCode )
{
	// 던전은 방 리스트를 전송하지 않습니다.
	if( m_cRoomType == CXSLRoom::RT_DUNGEON )
		return;

    KERM_ROOM_LIST_INFO_NOT kPacket;
    kPacket.m_iCode = iCode;
    GetRoomInfo( kPacket.m_kRoomInfo );
    BroadCastAllGSServer( ERM_ROOM_LIST_INFO_NOT, kPacket );
}

//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
#else
void KPvPRoom::SendPVPUserInfo( UidType iUnitUID, char cState /*= KCommunityUserInfo::US_NONE*/ )
{
	if( m_cRoomType != CXSLRoom::RT_PVP )
		return;

	if( cState == KCommunityUserInfo::US_NONE )
	{
		switch( GetStateID() )
		{
		case KRoomFSM::S_INIT:
		case KRoomFSM::S_CLOSE:
			cState = KCommunityUserInfo::US_PVP_LOBBY;
			break;

		case KRoomFSM::S_WAIT:
		case KRoomFSM::S_RESULT: // 결과창은.. 대기실로하자! ( 대기실로 나올거니까 )
			cState = KCommunityUserInfo::US_PVP_WAIT;
			break;

		case KRoomFSM::S_LOAD:
		case KRoomFSM::S_PLAY:
			cState = KCommunityUserInfo::US_PVP_PLAY;
			break;

		default:
			return;
		}
	}	

	KPVPUserSimpleInfo kInfo;
	kInfo.m_iUnitUID	 = iUnitUID;
	kInfo.m_cState		 = cState;
	kInfo.m_iStateCode	 = 0;
	kInfo.m_iRoomUID	 = GetUID();

	KECN_UPDATE_PVP_USER_INFO_NOT kPacket;
	kPacket.m_vecUserInfo.push_back( kInfo );
	BroadCastAllGSServer( ECN_UPDATE_PVP_USER_INFO_NOT, kPacket );
}

void KPvPRoom::SendAllPVPUserInfo()
{
	if( m_cRoomType != CXSLRoom::RT_PVP )
		return;

	// 모든 유저에 대해 갱신
	KECN_UPDATE_PVP_USER_INFO_NOT kPacket;

	char cState = KCommunityUserInfo::US_NONE;

	switch( GetStateID() )
	{
	case KRoomFSM::S_INIT:
	case KRoomFSM::S_CLOSE:
		{
			cState = KCommunityUserInfo::US_PVP_LOBBY;

			for( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
			{
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"RoomUser가 존재하지 않음." )
						<< END_LOG;
					continue;
				}

				KPVPUserSimpleInfo kInfo;
				kInfo.m_iUnitUID	 = spRoomUser->GetCID();
				kInfo.m_cState		 = cState;
				kInfo.m_iStateCode	 = 0;
				kInfo.m_iRoomUID	 = GetUID();
				kPacket.m_vecUserInfo.push_back( kInfo );
			}
		}
		break;

	case KRoomFSM::S_WAIT:
	case KRoomFSM::S_RESULT: // 결과창은.. 대기실로하자! ( 대기실로 나올거니까 )
		{
			cState = KCommunityUserInfo::US_PVP_WAIT;

			for( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
			{
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"RoomUser가 존재하지 않음." )
						<< END_LOG;
					continue;
				}

				KPVPUserSimpleInfo kInfo;
				kInfo.m_iUnitUID	 = spRoomUser->GetCID();
				kInfo.m_cState		 = cState;
				kInfo.m_iStateCode	 = 0;
				kInfo.m_iRoomUID	 = GetUID();
				kPacket.m_vecUserInfo.push_back( kInfo );
			}
		}
		break;

	case KRoomFSM::S_LOAD:
	case KRoomFSM::S_PLAY:
		{
			cState = KCommunityUserInfo::US_PVP_PLAY;

			for( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
			{
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"RoomUser가 존재하지 않음." )
						<< END_LOG;
					continue;
				}

				// 게임중인 유저만 정보갱신한다.
				if( !spRoomUser->IsPlaying() )
					continue;

				KPVPUserSimpleInfo kInfo;
				kInfo.m_iUnitUID	 = spRoomUser->GetCID();
				kInfo.m_cState		 = cState;
				kInfo.m_iStateCode	 = 0;
				kInfo.m_iRoomUID	 = GetUID();
				kPacket.m_vecUserInfo.push_back( kInfo );
			}
		}		
		break;

	default:
		return;
	}

	if( !kPacket.m_vecUserInfo.empty() )
	{
		BroadCastAllGSServer( ECN_UPDATE_PVP_USER_INFO_NOT, kPacket );
	}	
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
void KRoom::CheckCaughtBuffSkill()
{
	KEGS_BUFF_SKILL_INFO_CONVEY_NOT KNot;

	if( m_spRoomUserManager->CheckCaughtBuffSkill( KNot ) == true)
	{
		BroadCast( ERM_BUFF_SKILL_INFO_CONVEY_NOT, KNot );
	}
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
void KRoom::CheckPingScore()
{
	KRoomUserPtr spHost = m_spRoomUserManager->GetHostUser();
	if( IS_NULL( spHost ) )
		return;

#ifdef SERV_DO_NOT_CHANGE_HOST_AT_DUNGEON// 작업날짜: 2013-05-22	// 박세훈
	if( m_cRoomType == CXSLRoom::RT_DUNGEON )
	{
		// 던전 게임일 경우에는 호스트 변경을 하지 않는다.
		return;
	}
#endif // SERV_DO_NOT_CHANGE_HOST_AT_DUNGEON

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_bCheckPingScoreExecuted = true;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST


	//{{ 2013. 03. 06   방장 변경 기준 수정 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST_NEW
//#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//	KRoomUserPtr spNewHost = m_spRoomUserManager->ChooseBetterHost( m_bForceHost, spHost->GetCID() );
//#else//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	KRoomUserPtr spNewHost = m_spRoomUserManager->ChooseBetterHost( spHost->GetCID() );
//#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
#else
	// 현재 방장의 핑 스코어가 좋다면 바꾸지 말자
	if( 
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        spHost->IsPlaying() == true && spHost->ZombieAlert_Get() == false &&
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        spHost->GetPingScore() < KRoomUserManager::PV_HIGH )
	{
		m_spRoomUserManager->ClearPingScore();
		return;
	}
	
	KRoomUserPtr spNewHost = m_spRoomUserManager->ChooseBetterHost( spHost->GetPingScore()
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        , spHost->IsPlaying(), spHost->ZombieAlert_Get()
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        );
#endif SERV_CHOOSE_FASTEST_HOST_NEW
	//}
	if( IS_NOT_NULL( spNewHost ) )
	{
//#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		//if( m_bForceHost && spHost == spNewHost )
		//	return;
        if ( spHost == spNewHost )
        {
            return;
        }
//#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

		// 방장이 변경 되었다!
		spHost->SetHost( false );
		spNewHost->SetHost( true );

		// 방장 변경 패킷 전송
		KEGS_CHECK_PING_SCORE_CHANGE_HOST_NOT kNot;
		kNot.m_iOldHostUID = spHost->GetCID();
		kNot.m_iNewHostUID = spNewHost->GetCID();
		BroadCast( ERM_CHECK_PING_SCORE_CHANGE_HOST_NOT, kNot );

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// 적용날짜: 2013-05-03
		m_kHostCheckTimer.restart();
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ
	
		START_LOG( clog, L"Host 변경 알림 패킷 브로드 발송!" )
			<< BUILD_LOG( kNot.m_iOldHostUID )
			<< BUILD_LOG( kNot.m_iNewHostUID )
			<< END_LOG;

		//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
		// host가 1명인지 확인!
		LIF( m_spRoomUserManager->GetHostCount() == 1 );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
		//}}
	}

	m_spRoomUserManager->ClearPingScore();
}

#endif SERV_CHOOSE_FASTEST_HOST
//}

void KRoom::_BroadCast( int iFlag_, const KEvent& kEvent_ )
{
    std::map< UidType, std::set<UidType> > mapUserList;
    m_spRoomUserManager->GetUserList( iFlag_, mapUserList );
	m_spRoomUserManager->GetUserList( KRoomUserManager::UL_ALL, mapUserList, KRoomUserManager::UT_OBSERVER );

    // GameServer에 대한 loop
    std::map< UidType, std::set<UidType> >::iterator mit;
    for( mit = mapUserList.begin(); mit != mapUserList.end(); mit++ )
    {
        KEventPtr spEvent( kEvent_.Clone() );
        spEvent->m_anTrace[0] = mit->first;
        spEvent->m_anTrace[1] = -1;

        // User 각각에 대한 loop
        std::set<UidType>::iterator sit;
        for( sit = mit->second.begin(); sit != mit->second.end(); sit++ )
        {
            LIF( spEvent->m_kDestPerformer.AddUID( *sit ) );
        }

        KncSend( PI_CN_ROOM, GetUID(), spEvent );
    }
}

//{{ 2013. 2. 7	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
void KRoom::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	// 의도적인 Leave Room 직후 접속이 끊겼을 때 한 번 더 Leave Room 하게 되는 경우가 있다.
	KEGS_LEAVE_ROOM_ACK kPacket;
	if( GetStateID() == KRoomFSM::S_INIT || GetStateID() == KRoomFSM::S_CLOSE )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		return;
	}

	if( IsLanBugCheckUser( iFIRST_SENDER_UID ) == true )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_CHECK_CANCLE_NOT, GetUID(), iFIRST_SENDER_UID );
		RemoveLanBugCheckUser( iFIRST_SENDER_UID );
	}

	if( IsLanBugVerifyUser( iFIRST_SENDER_UID ) == true )
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_CANCLE_NOT, GetUID(), iFIRST_SENDER_UID );
		RemoveLanBugVerifyUser( iFIRST_SENDER_UID );
	}
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// 적용날짜: 2013-05-03
bool KRoom::HostCheck( void ) const
{
	const float fHostCheckTerm = SiKGameSysVal()->GetHostCheckTerm();

	if( fHostCheckTerm < 0.0f )
	{
		return false;
	}

	return ( fHostCheckTerm <= m_kHostCheckTimer.elapsed() );
}
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ



IMPL_ON_FUNC( ERM_JOIN_ROOM_REQ )
{
    unsigned short usEventID;
    if( kPacket_.m_bQuickJoin )
    {
        usEventID = ERM_QUICK_JOIN_ACK;
    }
    else
    {
        usEventID = ERM_JOIN_ROOM_ACK;
    }	

    KEGS_JOIN_ROOM_ACK kPacket;
	if( !( GetStateID() == KRoomFSM::S_WAIT || GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY || GetStateID() == KRoomFSM::S_RESULT ) )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_09;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
        return;
    }

	//gm 이상이면 센터서버의 ip를 넣어주자.
	if( kPacket_.m_kRoomUserInfo.m_cAuthLevel >= SEnum::UAL_GM )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

	//{{ 2012. 12. 5	최육사		옵저버 대전 난입 모드
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
#else
	//난입가능하면 방상태는 체크하지 않는다.
	if( IsAbleToIntrude() == false )
	{
		if( GetStateID() != KRoomFSM::S_WAIT )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_09;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
			return;
		}
	}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
	//}}	

	//080425.hoons.옵저버 일경우 체크를 하지 않는다.
	if( kPacket_.m_kRoomUserInfo.m_bIsObserver == false )
	{
		//{{ 2012. 12. 5	최육사		옵저버 대전 난입 모드
#ifdef SERV_OBSERVER_PVP_INTRUDE_MODE
		//난입가능하면 방상태는 체크하지 않는다.
		if( IsAbleToIntrude() == false )
		{
			if( GetStateID() != KRoomFSM::S_WAIT )
			{
				kPacket.m_iOK = NetError::ERR_ROOM_09;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
				return;
			}
		}
#endif SERV_OBSERVER_PVP_INTRUDE_MODE
		//}}

		// 방에 입장할 슬롯이 있는지 체크한다.
		if( IsFull() )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_03;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
			return;
		}

		// 비밀방이면 비밀번호를 체크한다.
		if( !m_bPublic )
		{
#ifdef SERV_NEW_PVPROOM_PROCESS
			if( kPacket_.m_bInvitation != true && m_wstrPassword != kPacket_.m_wstrPassword )
#else
			if( m_wstrPassword != kPacket_.m_wstrPassword )
#endif SERV_NEW_PVPROOM_PROCESS
			{
				kPacket.m_iOK = NetError::ERR_ROOM_04;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
				return;
			}
		}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-10-08	// 박세훈
		if( IsOfficialMatch() == true )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_53;	// 공식 대전에는 난입할 수 없습니다.
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
			return;
		}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM
	}

    // 입장한 유저를 슬롯에 넣는다.

	switch( m_cRoomType )
	{
	case CXSLRoom::RT_PVP:
		if( !m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, true ) )
        {
            START_LOG( cerr, L"방 입장 실패." )
                << BUILD_LOG( FIRST_SENDER_UID )
                << BUILD_LOG( GetUID() )
                << END_LOG;

            kPacket.m_iOK = NetError::ERR_ROOM_30;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );
            return;
        }

		break;
	case CXSLRoom::RT_DUNGEON:
		if( !m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) )
        {
            START_LOG( cerr, L"방 입장 실패." )
                << BUILD_LOG( FIRST_SENDER_UID )
                << BUILD_LOG( GetUID() )
                << END_LOG;

            kPacket.m_iOK = NetError::ERR_ROOM_30;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );

            return;
        }
		else
		{
			//080409.hoons. 제자 리스트를 셋팅한다.
			m_spRoomUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );
		}

		break;
	}

    // 방 상태 변화.
    // 없다.

    // 입장한 유저에게 ack를 날린다.
    kPacket.m_iOK = NetError::NET_OK;
    GetRoomInfo( kPacket.m_RoomInfo );
    GetRoomSlotInfo( kPacket.m_vecSlot );
	//옵저버 유저는 항상준다. 방접속시 모든유저가 옵저버 유저와도 접속을 해야하니..
	GetRoomSlotInfo( kPacket.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, usEventID, kPacket );

    // 방의 모든 유저에게 NOT를 날린다.
    KEGS_JOIN_ROOM_NOT kPacketNot;
	//080425.hoons.옵저버 상태별 유저정보를 셋팅해서 보내준다.
	if( kPacket_.m_kRoomUserInfo.m_bIsObserver == false )
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KRoomUserManager::UT_GAME );
	else
		m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KRoomUserManager::UT_OBSERVER );
    BroadCast( ERM_JOIN_ROOM_NOT, kPacketNot );

    // 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
    SendRoomListInfo( NetError::ERR_ROOM_15 );

	//{{ 2009. 4. 23  최육사	대전유저리스트
	SendPVPUserInfo( kPacket_.m_kRoomUserInfo.m_nUnitUID );
	//}}
}

_IMPL_ON_FUNC( ERM_BAN_USER_REQ, KEGS_BAN_USER_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_BAN_USER_ACK, KPacketOK );

	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2008. 1. 29  최육사  방장 강퇴 기능
	KRoomUserPtr pkSenderRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	
	//{{ 2012. 11. 28	박세훈	옵저버 모드 수정
#ifdef SERV_FIX_OBSERVE_MODE
	if( pkSenderRoomUser == NULL )
	{
		pkSenderRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
	}
#endif SERV_FIX_OBSERVE_MODE
	//}}

	if( pkSenderRoomUser == NULL )
	{
		START_LOG( cerr, L"유저가 존재하지 않습니다." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BAN_USER_ACK, kPacket );
		return;
	}

	// 운영자 체크
	char cSenderAuthLevel = pkSenderRoomUser->GetAuthLevel();
	bool bSenderIsHost = pkSenderRoomUser->IsHost();
	if( cSenderAuthLevel < SEnum::UAL_GM  &&  m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// 운영자가 아닐경우 방장인지 체크한다.
		VERIFY_HOST( ERM_BAN_USER_ACK );
	}
	//}}

	UidType nGSUID;
	bool bIsHost = false;

	KRoomUserPtr pkRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iUnitUID );	
	if( pkRoomUser == NULL )
	{
		START_LOG( cerr, L"강퇴시킬 룸유져 찾기실패" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
	else
	{
		nGSUID = pkRoomUser->GetGSUID();
		bIsHost = pkRoomUser->IsHost(); // 강퇴당한 유저가 방장인지 얻기
	}

    // 방 슬롯 정보 변경.
    if( !m_spRoomUserManager->LeaveRoom( kPacket_.m_iUnitUID ) || pkRoomUser == NULL )
    {
        // 강퇴 실패.
        kPacket.m_iOK = NetError::ERR_SLOT_05;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BAN_USER_ACK, kPacket );
        return;
    }
	else
	{
		if( cSenderAuthLevel >= SEnum::UAL_GM && bSenderIsHost == false )
		{
			START_LOG( cout, L"운영자가 유저를 강퇴시켰습니다." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( cSenderAuthLevel )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( bIsHost );
		}
	}

    // 강퇴 대상자에게 not를 날린다.
    SendToGSCharacter( nGSUID, kPacket_.m_iUnitUID, ERM_BAN_USER_NOT );

	//{{ 2009. 4. 23  최육사	대전유저리스트
	SendPVPUserInfo( kPacket_.m_iUnitUID, KCommunityUserInfo::US_PVP_LOBBY );
	//}}

	//만일의 경우 강퇴로 인한 모든 유저가 나갈시
	//{{ 2012. 11. 28	박세훈	옵저버 모드 수정
#ifdef SERV_FIX_OBSERVE_MODE
	if( ( IsEmpty( KRoomUserManager::UT_GAME ) == false ) || ( IsEmpty( KRoomUserManager::UT_OBSERVER ) == false ) )
#else
	if( IsEmpty() == false )
#endif SERV_FIX_OBSERVE_MODE
		//}}
	{
		// 방장에게 ack를 날린다.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BAN_USER_ACK, kPacket );

		// 방의 모든 유저들에게 슬롯 정보를 날린다.
		KEGS_LEAVE_ROOM_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );

		//방리스트 갱신
		SendRoomListInfo( NetError::ERR_ROOM_15 );
	}
	else
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );	

		//방리스트 삭제.
		SendRoomListInfo( NetError::ERR_ROOM_16 );
	}
}

_IMPL_ON_FUNC( ERM_CHANGE_READY_REQ, KEGS_CHANGE_READY_REQ )
{
	//{{ 2010. 02. 10  최육사	로그 레벨 변경
	VERIFY_ROOM_STATE_WARN( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_READY_ACK, KERM_CHANGE_READY_ACK );
	//}}
	
	if( m_spRoomUserManager->SetReady( FIRST_SENDER_UID, kPacket_.m_bReady ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_03;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_READY_ACK, kPacket );
		return;
	}

	// 요청한 유저에게 ack를 날린다.
	kPacket.m_iDungeonID	= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
	kPacket.m_cDungeonMode	= m_cDungeonMode;
	kPacket.m_iOK			= NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_READY_ACK, kPacket );

	// 방의 모든 유저에게 슬롯 정보를 날린다.
	KEGS_CHANGE_READY_NOT kNot;
	kNot.m_UnitUID	= FIRST_SENDER_UID;
	kNot.m_bReady	= kPacket_.m_bReady;

	BroadCast( ERM_CHANGE_READY_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_CHANGE_PITIN_REQ,	KEGS_CHANGE_PITIN_REQ )
{
	VERIFY_ROOM_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), ERM_CHANGE_PITIN_ACK, KEGS_CHANGE_PITIN_ACK );

	if( m_spRoomUserManager->SetPitIn( FIRST_SENDER_UID, kPacket_.m_bPitIn ) == false )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PITIN_ACK, kPacket );
		return;
	}

	kPacket.m_iOK			  = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PITIN_ACK, kPacket );

	// 방의 모든 유저에게 슬롯 정보를 날린다.
	KEGS_CHANGE_PITIN_NOT kNot;
	kNot.m_UnitUID	= FIRST_SENDER_UID;
	kNot.m_bPitIn	= kPacket_.m_bPitIn;

	BroadCast( ERM_CHANGE_PITIN_NOT, kNot );
}

_IMPL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_REQ, KEGS_CHANGE_SLOT_OPEN_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_SLOT_OPEN_ACK, KEGS_CHANGE_SLOT_OPEN_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// 방장인지 체크한다.
		VERIFY_HOST( ERM_CHANGE_SLOT_OPEN_ACK );
	}

	bool	bValue = true;
	int		nTeamBalanceSlotIndex = -1;
    switch( kPacket_.m_SlotState )
    {
    case CXSLRoom::SS_EMPTY:

		if( m_cRoomType == CXSLRoom::RT_PVP )
		{
			switch( m_cGameType )
			{
			case CXSLRoom::PGT_TEAM_DEATH:
			case CXSLRoom::PGT_TEAM:
				bValue = m_spRoomUserManager->OpenSlotTeam( kPacket_.m_SlotIndex, nTeamBalanceSlotIndex );
				break;

			default:
				bValue = m_spRoomUserManager->OpenSlot( kPacket_.m_SlotIndex );
			}
		}
		else
		{
			bValue = m_spRoomUserManager->OpenSlot( kPacket_.m_SlotIndex );
		}

        if( bValue == false )
        {
            kPacket.m_iOK = NetError::ERR_SLOT_02;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );
            return;
        }
        break;

    case CXSLRoom::SS_CLOSE:
		if( m_cRoomType == CXSLRoom::RT_PVP )
		{
			switch( m_cGameType )
			{
			case CXSLRoom::PGT_TEAM_DEATH:
			case CXSLRoom::PGT_TEAM:
				bValue = m_spRoomUserManager->CloseSlotTeam( kPacket_.m_SlotIndex, nTeamBalanceSlotIndex );
				break;

			default:
				bValue = m_spRoomUserManager->CloseSlot( kPacket_.m_SlotIndex );
			}
		}
		else
		{
			bValue = m_spRoomUserManager->CloseSlot( kPacket_.m_SlotIndex );
		}

		if( bValue == false )
        {
            kPacket.m_iOK = NetError::ERR_SLOT_02;
            SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );
            return;
        }
        break;

    default:
        kPacket.m_iOK = NetError::ERR_SLOT_02;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );
        return;
    }

    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_SLOT_OPEN_ACK, kPacket );

	KEGS_CHANGE_SLOT_OPEN_NOT kPacketNot;
	kPacketNot.m_SlotIndex				= kPacket_.m_SlotIndex;
	kPacketNot.m_SlotState				= kPacket_.m_SlotState;
	kPacketNot.m_TeamBalanceSlotIndex	= nTeamBalanceSlotIndex;
    BroadCast( ERM_CHANGE_SLOT_OPEN_NOT, kPacketNot );

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	SendRoomListInfo( NetError::ERR_ROOM_15 );

	START_LOG( clog, L"::: Change Slot ::: " )
		<< BUILD_LOGc( kPacketNot.m_SlotState )
		<< BUILD_LOGc( kPacketNot.m_SlotIndex )
		<< BUILD_LOGc( kPacketNot.m_TeamBalanceSlotIndex )
		;
}

_IMPL_ON_FUNC( ERM_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_GAME_LOADING_ACK, KPacketOK );

	// 난입시 로딩 요청 무시.
	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_LOADING_ACK, kPacket );
		return;
	}

    // loading 상태를 변화시킨다.
    if( m_spRoomUserManager->SetLoadingProgress( FIRST_SENDER_UID, kPacket_.m_iLoadingProgress ) == false )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_13;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_LOADING_ACK, kPacket );
        return;
    }

	//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	if( kPacket_.m_iLoadingProgress >= 100 )
	{
		const int iSuitableMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( GetDungeonIDAndDif() ) - 2;
		const int iSuitableMaxLevel = SiCXSLDungeonManager()->GetDungeonMaxLevel( GetDungeonIDAndDif() ) + 2;

		bool bSuitableLevel = false;

		if( iSuitableMinLevel <= kPacket_.m_iUnitLevel &&  kPacket_.m_iUnitLevel <= iSuitableMaxLevel )
		{
			bSuitableLevel = true;
		}		
		m_spRoomUserManager->SetUnitLevelBeforGameStart( FIRST_SENDER_UID, bSuitableLevel );
	}
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}

    // 보낸 사람에게 ack를 날린다.
    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_LOADING_ACK, kPacket );

    // 방의 모든 유저들에게 not를 날린다.
    KEGS_GAME_LOADING_NOT kPacketBR;
    kPacketBR.m_iUnitUID = FIRST_SENDER_UID;
    kPacketBR.m_iLoadingProgress = kPacket_.m_iLoadingProgress;
    BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );

	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( m_spRoomUserManager->IsHost( FIRST_SENDER_UID ) == true )
	{
		for( int i = 0; i < m_spRoomUserManager->GetNumMember(); ++i )
		{
			KRoomUserPtr pkRoomUser = m_spRoomUserManager->GetUser( i );
			if( pkRoomUser == NULL )
				continue;

			if( pkRoomUser->IsPvpNpc() == false )
				continue;

			pkRoomUser->SetLoadingProgress( kPacket_.m_iLoadingProgress );

			KEGS_GAME_LOADING_NOT kPacketBR;
			kPacketBR.m_iUnitUID = pkRoomUser->GetCID();
			kPacketBR.m_iLoadingProgress = kPacket_.m_iLoadingProgress;
			BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );
		}
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2011. 08. 05	최육사	대전방에 유저 혼자 남게되는 버그 수정
#ifdef SERV_CODE_REFACTORING_20110804
	if( CheckAllPlayerFinishLoading() == true )
	{
		//{{ 2011. 08. 05	최육사	대전방에 유저 혼자 남게되는 버그 수정
#ifdef SERV_PVP_USER_ALONE_IN_ROOM_BUG_FIX
		if( IsOfficialMatch() )
		{
			if( CheckIfPlayEnd() )
			{
				EndPlay();
			}
		}
#endif SERV_PVP_USER_ALONE_IN_ROOM_BUG_FIX
		//}}
	}
#else
	// 모든 유저가 로딩 끝났으면 not를 날린다.
	if( m_spRoomUserManager->IsAllPlayerFinishLoading() )
	{
		BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

		// 스테이지를 초기화.
		StartPlay();

		KEGS_PLAY_START_NOT kNot;
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		kNot.m_iDungeonID = GetDungeonIDAndDif();
#endif SERV_CHAR_LOG
		//}}
		kNot.m_RoomState = GetStateID();
		GetRoomSlotInfo( kNot.m_vecSlot );

		// 모든 유저에게 플레이 시작을 알림.
		BroadCast( ERM_PLAY_START_NOT, kNot );

		// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  최육사	대전유저리스트
		SendAllPVPUserInfo();
		//}}
	}
#endif SERV_CODE_REFACTORING_20110804
	//}}
}

_IMPL_ON_FUNC( ERM_STATE_CHANGE_RESULT_REQ, KEGS_STATE_CHANGE_RESULT_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_RESULT ), ERM_STATE_CHANGE_RESULT_ACK, KEGS_STATE_CHANGE_RESULT_ACK );

    // 방장인지 체크한다.
    VERIFY_HOST( ERM_STATE_CHANGE_RESULT_ACK );

    // 보낸 사람에게 ack를 날린다.
    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STATE_CHANGE_RESULT_ACK, kPacket );

    // 모든 유저에게 상태 변화 하라고 알린다.
    KEGS_STATE_CHANGE_RESULT_NOT kPacketNot;
    kPacketNot.m_cRoomState = GetStateID();
    m_spRoomUserManager->GetRoomSlotInfo( kPacketNot.m_vecSlot );
	BroadCast( ERM_STATE_CHANGE_RESULT_NOT, kPacketNot, KRoomUserManager::UL_RESULT );
}

//{{ 2007. 8. 20  최육사  채팅 통합
_IMPL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ )
{
    VERIFY_ROOM_STATE_CHAT_ONLY( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ),
                       ERM_CHAT_ACK, KERM_CHAT_ACK );

	if( kPacket_.m_cChatPacketType != KEGS_CHAT_REQ::CPT_TOTAL )
	{
		START_LOG( cerr, L"일반채팅 패킷타입이 아닌데 여기왔네?" )
			<< BUILD_LOGc( kPacket_.m_cChatPacketType )
			<< END_LOG;
		return;
	}

    // 보낸 사람에게 ack를 날린다.
    kPacket.m_iOK			  = NetError::NET_OK;
	kPacket.m_cChatPacketType = kPacket_.m_cChatPacketType; 

    // 모든 유저에게 채팅 내용을 날린다.
    KEGS_CHAT_NOT kPacketBR;
    kPacketBR.m_SenderUnitUID	= FIRST_SENDER_UID;
	kPacketBR.m_RecieverUnitUID = kPacket_.m_ToUnitUID;
	kPacketBR.m_cChatPacketType = kPacket_.m_cChatPacketType; 
	kPacketBR.m_cRoomChatType	= kPacket_.m_cRoomChatType;
    kPacketBR.m_wstrMsg			= kPacket_.m_wstrMsg;

    /*  070709. jseop. 채팅 로그 삭제.
    KELOG_CHAT_LOG_NOT kNot;
    kNot.m_nRoomUID = GetUID();
    kNot.m_nCharUID = FIRST_SENDER_UID;
    //kNot.m_nUserUID = ; // 아래에서 처리한다.
    kNot.m_cChatType = kPacket_.m_cRoomChatType;
    kNot.m_strMsg = kPacket_.m_wstrMsg.substr( 0, 62 );

    KRoomUserPtr spUser = m_spRoomUserManager->GetUser( kNot.m_nCharUID );
    if( spUser )
    {
        KRoomUserInfo kInfo;
        spUser->GetRoomUserInfo( kInfo );
        kNot.m_nUserUID = kInfo.m_kUnitInfo.m_iOwnerUserUID;
    }

    KncSend( PI_CN_ROOM, GetUID(), PI_LOG_DB, 0, NULL, ELOG_CHAT_LOG_NOT, kNot );
    */
	
    if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_ALL )
	{
		BroadCast( ERM_CHAT_NOT, kPacketBR );
	}
	else if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_TEAM )
	{
		int iTeam;
        if( m_spRoomUserManager->GetTeam( FIRST_SENDER_UID, iTeam ) )
        {
            BroadCast( ERM_CHAT_NOT, kPacketBR, 
                ( ( iTeam == CXSLRoom::TN_RED ) ? KRoomUserManager::UL_RED_TEAM : KRoomUserManager::UL_BLUE_TEAM ) );
        }
        else
        {
            kPacket.m_iOK = NetError::ERR_ROOM_23;
        }
	}
	else if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacketBR.m_cRoomChatType == CXSLRoom::RCT_WHISPER )
	{
#ifndef DEPRECATED_SERVER_GROUP_MASK
		//{{ 2010. 05. 19  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		if( KncUid::ExtractServerGroupID( kPacket_.m_ToUnitUID ) != KncUid::ExtractServerGroupID( FIRST_SENDER_UID ) )
		{
			kPacket.m_iOK = NetError::ERR_CHAT_04;
		}
		else
#endif SERV_INTEGRATION
		//}}
#endif DEPRECATED_SERVER_GROUP_MASK
		{
			UidType nGS;
			if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_ToUnitUID, nGS ) )
			{
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_NOT, kPacketBR );
				SendToGSCharacter( nGS, kPacket_.m_ToUnitUID, ERM_CHAT_NOT, kPacketBR );
			}
			else
			{
				kPacket.m_iOK = NetError::ERR_ROOM_23;
			}
		}
	}

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
}
//}}

_IMPL_ON_FUNC( ERM_CHANGE_MOTION_REQ, KEGS_CHANGE_MOTION_REQ )
{
	VERIFY_ROOM_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), 
		ERM_CHANGE_MOTION_ACK, KEGS_CHANGE_MOTION_ACK );

	// 보낸 사람에게 ack를 날린다.
	kPacket.m_iOK = NetError::NET_OK;

	// 모든 유저에게 Motion Not 날린다.
	KEGS_CHANGE_MOTION_NOT kEGS_CHANGE_MOTION_NOT;
	kEGS_CHANGE_MOTION_NOT.m_UnitUID = FIRST_SENDER_UID;
	kEGS_CHANGE_MOTION_NOT.m_cMotionID = kPacket_.m_cMotionID;
	
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_MOTION_ACK, kPacket );

	BroadCast( ERM_CHANGE_MOTION_NOT, kEGS_CHANGE_MOTION_NOT );
}

_IMPL_ON_FUNC( ERM_CHANGE_PLAY_TIME_LIMIT_REQ, KEGS_CHANGE_PLAY_TIME_LIMIT_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHANGE_PLAY_TIME_LIMIT_ACK, KEGS_CHANGE_PLAY_TIME_LIMIT_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		VERIFY_HOST( ERM_CHANGE_PLAY_TIME_LIMIT_ACK );
	}

	//{{ 2009. 10. 22  최육사	대전방 버그수정	
	if( m_cRoomType == CXSLRoom::RT_PVP )
	{
		if( CXSLRoom::IsValidPvpPlayTime( m_cGameType, static_cast<int>(kPacket_.m_fPlayTime) ) == false )
		{
			START_LOG( cerr, L"대전방 플레이 타임 세팅값이 이상하다." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( m_cGameType )
				<< BUILD_LOG( kPacket_.m_fPlayTime )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_30;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PLAY_TIME_LIMIT_ACK, kPacket );
			return;
		}
	}
	//}}

    SetPlayTimeLimit( kPacket_.m_fPlayTime );

    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PLAY_TIME_LIMIT_ACK, kPacket );

    KEGS_CHANGE_PLAY_TIME_LIMIT_NOT kPacketNot;
    kPacketNot.m_fPlayTime = kPacket_.m_fPlayTime;
    BroadCast( ERM_CHANGE_PLAY_TIME_LIMIT_NOT, kPacketNot );
    SendRoomListInfo( NetError::ERR_ROOM_15 );
}

_IMPL_ON_FUNC( ERM_SET_UNIT_INFO_NOT, KRoomUserInfo )
{
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( !( GetStateID() == KRoomFSM::S_WAIT || GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY || GetStateID() == KRoomFSM::S_RESULT || GetStateID() == KRoomFSM::S_WAIT_FOR_DEFENCE ) )
#else
	if( !( GetStateID() == KRoomFSM::S_WAIT || GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY || GetStateID() == KRoomFSM::S_RESULT ) )
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}    
    {
        START_LOG( cerr, L"상태 오류." )
            << BUILD_LOG( GetStateID() )
            << BUILD_LOG( GetStateIDString() )
            << END_LOG;

        return;
    }

    m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_REFRESH_ROOM_REQ )
{
    VERIFY_ROOM_STATE( ( 3, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_REFRESH_ROOM_ACK, KEGS_REFRESH_ROOM_ACK );

    kPacket.m_iOK = NetError::NET_OK;
    GetRoomInfo( kPacket.m_RoomInfo );
    GetRoomSlotInfo( kPacket.m_vecSlot );

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REFRESH_ROOM_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    m_spRoomUserManager->SetUserEquipItem( FIRST_SENDER_UID, kPacket_.m_vecInventorySlotInfo );
    m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );
	
	// 던전 라운지라면?
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
		if( m_cRoomType == CXSLRoom::RT_DUNGEON )
		{
			KEGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT kPacketNot;
			kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
			kPacketNot.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
			//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
			kPacketNot.m_iEquippedTitleID = kPacket_.m_kRoomUserInfo.m_iTitleID;
#else
			kPacketNot.m_sEquippedTitleID = kPacket_.m_kRoomUserInfo.m_sTitleID;
#endif SERV_TITLE_DATA_SIZE
			//}}			
			BroadCast( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, kPacketNot );
			return;
		}
	}
	
	// 그외에 방위치라면..
	BroadCast( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, kPacket_ );
}

//{{ 2009. 5. 21  최육사	내구도 소모 개편
IMPL_ON_FUNC( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetUserEquipItem( FIRST_SENDER_UID, kPacket_.m_vecInventorySlotInfo );
	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );

	BroadCast( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, kPacket_ );
}
//}}

//{{ 2012. 10. 17	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
_IMPL_ON_FUNC( ERM_RESULT_SUCCESS_REQ, KEGS_RESULT_SUCCESS_REQ )
#else
IMPL_ON_FUNC_NOPARAM( ERM_RESULT_SUCCESS_REQ )
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_RESULT ), ERM_RESULT_SUCCESS_ACK, KEGS_RESULT_SUCCESS_ACK );

	if( m_spRoomUserManager->SetSuccessResult( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_RESULT_SUCCESS_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2009. 4. 28  최육사	이벤트던전
	kPacket.m_iDungeonID = m_iDungeonID;
	//}}
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_RESULT_SUCCESS_ACK, kPacket );

	//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	LogToDB_GamePlayNetWorkInfo( FIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo, false );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

	//결과 상태인 유저가 결과창 확인이 끝났으면
	if( m_spRoomUserManager->IsAllPlayerSuccessResult() == true )
	{
		// 게임 종료 처리
		EndGame();

#ifdef SERV_PVP_REMATCH
		if( CanRematch() == true )
		{
			Rematch();
			return;
		}
#endif SERV_PVP_REMATCH

		//{{ 2012. 06. 07	최육사	배틀필드 시스템
		//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
		//////////////////////////////////////////////////////////////////////////

		// S_RETURN_TO_FIELD 상태로 변경한다음 복귀 작업을 진행하자!
		StateTransition( KRoomFSM::I_TO_RETURN_TO_FIELD );

		// 타이머 초기화!
		m_kTimer[TM_RETURN_TO_FIELD].restart();

		// 패킷 정리
		KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		GetRoomInfo( kNot.m_RoomInfo );
		GetRoomSlotInfo( kNot.m_vecSlot );		

		// 첫번째 멤버부터 이탈 시도!
		UidType iGSUID = 0;
		UidType iFirstUnitUID = 0;
		m_spRoomUserManager->PrepareForReturnToFieldProcess( GetUID(), iGSUID, iFirstUnitUID, kNot.m_kReturnToFieldInfo );

		// 전송!
		SendToGSCharacter( iGSUID, iFirstUnitUID, ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );

		//////////////////////////////////////////////////////////////////////////
#else
		//////////////////////////////////////////////////////////////////////////

		//결과처리까지 끝나고 서야 방에 대기중인 유저에게까지 패킷을 보낸다.
		KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		GetRoomInfo( kNot.m_RoomInfo );
		GetRoomSlotInfo( kNot.m_vecSlot );
		BroadCast( ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );

		//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
		//////////////////////////////////////////////////////////////////////////
		//}}

		//결과종료 처리 되었으므로 방정보 업데인트
		SendRoomListInfo( NetError::ERR_ROOM_15 );

		//{{ 2009. 4. 23  최육사	대전유저리스트		
		SendAllPVPUserInfo();
		//}}
	}
}

_IMPL_ON_FUNC( ERM_ADD_ON_STAT_REQ, KEGS_ADD_ON_STAT_REQ )
{
	//{{ 2009. 3. 30  최육사	cerr -> cwarn
	VERIFY_STATE_WARN( ( 1, KRoomFSM::S_PLAY ) );
	//}}

	//demiahun. 07.10.26. 현재로서는 특별한 제약이나 조건 기능이 없음.

	if( (int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_PVP ||
		(int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_DUNGEON ||
		(int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_BATTLE_FIELD ||
		(int)KncUid::ExtractReservedID( GetUID() ) == CXSLRoom::RT_TRAININGCENTER )
	{
		KEGS_ADD_ON_STAT_ACK kAck;
		kAck.m_iOK	= NetError::NET_OK;

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADD_ON_STAT_ACK, kAck );

		KEGS_ADD_ON_STAT_NOT kNot;
		kNot.m_UID			= kPacket_.m_UID;
		kNot.m_bIsUnit		= kPacket_.m_bIsUnit;
		kNot.m_cStatType	= kPacket_.m_cStatType;
		kNot.m_fStatVal		= kPacket_.m_fStatVal;
		kNot.m_fTime		= kPacket_.m_fTime;

		BroadCast( ERM_ADD_ON_STAT_NOT, kNot, KRoomUserManager::UL_PLAY );
	}
	else
	{
		KEGS_ADD_ON_STAT_ACK kAck;
		kAck.m_iOK	= NetError::ERR_ROOM_28;

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADD_ON_STAT_ACK, kAck );

		START_LOG( cerr, L"잘못된 게임타입[게임스탯]" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
			<< BUILD_LOG( (int)KncUid::ExtractReservedID( GetUID() ) )
			<< END_LOG
			;
	}
}

_IMPL_ON_FUNC( ERM_REPAIR_ITEM_NOT, KEGS_REPAIR_ITEM_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	BroadCast( ERM_REPAIR_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_NOT )
{
    VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    UidType iGSUID;
    if( !m_spRoomUserManager->GetRoomUserGS( kPacket_.m_iReceiverUID, iGSUID ) )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_iSenderUID )
            << BUILD_LOG( kPacket_.m_iReceiverUID )
            << BUILD_LOG( kPacket_.m_wstrSenderNickName )
            << END_LOG;
        return;
    }

    if( !m_spRoomUserManager->RequestTradeTo( FIRST_SENDER_UID, kPacket_.m_iReceiverUID ) )
    {
        START_LOG( clog, L"거래 요청 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;

        KERM_REQUEST_TRADE_REPLY_NOT kPacket;
        kPacket.m_iReceiverUID = FIRST_SENDER_UID;
        kPacket.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_03;
        kPacket.m_iMemberUID = kPacket_.m_iReceiverUID;
        kPacket.m_iMemberGSUID = iGSUID;

        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket );
        return;
    }

    // 방의 모든 유저에게 바뀐 정보를 날린다.
    KEGS_CHANGE_TRADE_NOT kNot;
    kNot.m_iUnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsInTrade( FIRST_SENDER_UID, kNot.m_bTrade ) )
    {
        START_LOG( cerr, L"거래 상태 확인 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_TRADE_NOT, kNot );

    KEGS_CHANGE_READY_NOT kReadyNot;
    kReadyNot.m_UnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsReady( FIRST_SENDER_UID, kReadyNot.m_bReady ) )
    {
        START_LOG( cerr, L"준비 상태 확인 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_READY_NOT, kReadyNot );

    SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT )
{
   VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    UidType iGSUID;
    if( !m_spRoomUserManager->GetRoomUserGS( kPacket_.m_iReceiverUID, iGSUID ) )
    {
        START_LOG_WITH_NAME( cerr )
            << BUILD_LOG( kPacket_.m_iReceiverUID )
            << END_LOG;
        return;
    }

    if( kPacket_.m_iOK == NetError::NOT_REQUEST_TRADE_REPLY_00 )
    {
        if( m_spRoomUserManager->TradeAcceptedBy( kPacket_.m_iReceiverUID, FIRST_SENDER_UID ) )
        {
            SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket_ );
            return;
        }
        else
        {
            kPacket_.m_iOK = NetError::NOT_REQUEST_TRADE_REPLY_03;
        }
    }

    if( m_spRoomUserManager->TradeRejectedBy( kPacket_.m_iReceiverUID, FIRST_SENDER_UID ) )
    {
        // 방의 모든 유저에게 바뀐 정보를 날린다.
        KEGS_CHANGE_TRADE_NOT kNot;
        kNot.m_iUnitUID = kPacket_.m_iReceiverUID;
        if( m_spRoomUserManager->IsInTrade( kPacket_.m_iReceiverUID, kNot.m_bTrade ) )
        {
            BroadCast( ERM_CHANGE_TRADE_NOT, kNot );
        }
        else
        {
            START_LOG( cerr, L"거래 상태 확인 실패." )
                << BUILD_LOG( kPacket_.m_iReceiverUID )
                << END_LOG;
        }

        KEGS_CHANGE_READY_NOT kReadyNot;
        kReadyNot.m_UnitUID = kPacket_.m_iReceiverUID;
        if( m_spRoomUserManager->IsReady( kPacket_.m_iReceiverUID, kReadyNot.m_bReady ) )
        {
            BroadCast( ERM_CHANGE_READY_NOT, kReadyNot );
        }
        else
        {
            START_LOG( cerr, L"준비 상태 확인 실패." )
                << BUILD_LOG( kPacket_.m_iReceiverUID )
                << END_LOG;
        }
    }

    SendToGSCharacter( iGSUID, kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( ERM_TRADE_START_NOT )
{
    VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    if( !m_spRoomUserManager->SetTrade( FIRST_SENDER_UID, true ) )
    {
        START_LOG( clog, L"거래 상황 업데이트 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;

        KEGS_BREAK_TRADE_REQ kPacket;
        kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_09;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BREAK_TRADE_REQ, kPacket );
        return;
    }

    // 방의 모든 유저에게 바뀐 정보를 날린다.
    KEGS_CHANGE_TRADE_NOT kNot;
    kNot.m_iUnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsInTrade( FIRST_SENDER_UID, kNot.m_bTrade ) )
    {
        START_LOG( cerr, L"거래 상태 확인 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_TRADE_NOT, kNot );

    KEGS_CHANGE_READY_NOT kReadyNot;
    kReadyNot.m_UnitUID = FIRST_SENDER_UID;
    if( !m_spRoomUserManager->IsReady( FIRST_SENDER_UID, kReadyNot.m_bReady ) )
    {
        START_LOG( cerr, L"준비 상태 확인 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }
    BroadCast( ERM_CHANGE_READY_NOT, kReadyNot );
}

IMPL_ON_FUNC_NOPARAM( ERM_TRADE_FINISH_NOT )
{
    VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

    if( !m_spRoomUserManager->SetTrade( FIRST_SENDER_UID, false ) )
    {
        START_LOG( clog, L"거래 상황 업데이트." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
        return;
    }

    bool bTrade;
    if( m_spRoomUserManager->IsInTrade( FIRST_SENDER_UID, bTrade ) )
    {
        // 방의 모든 유저에게 바뀐 정보를 날린다.
        KEGS_CHANGE_TRADE_NOT kNot;
        kNot.m_iUnitUID = FIRST_SENDER_UID;
        kNot.m_bTrade = bTrade;

        BroadCast( ERM_CHANGE_TRADE_NOT, kNot );
    }
    else
    {
        START_LOG( cerr, L"거래 상태 확인 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
    }
}

IMPL_ON_FUNC( ERM_SET_QUEST_ITEM_INFO_NOT )
{
	//현재는 던전에서만 받는다는 조건하에 웨이트 일때만 처리함.
	//{{ 2010. 02. 10  최육사	일일 이벤트 퀘스트
	// 실시간으로 퀘스트가 포기처리되는 기능이 추가됨에 따라 상태 제한이 걸리면 안된다. 따라서 주석처리함.
	//VERIFY_STATE( ( 1, KRoomFSM::S_WAIT ) );
	//}}	

	if( m_spRoomUserManager->SetUnitQuestInvenInfo( FIRST_SENDER_UID, kPacket_.m_mapDropQuestItembyIngQuest ) == false )
	{
		START_LOG( cerr, L"퀘스트 아이템 인벤토리 정보 셋팅 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
	
	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	if( m_spRoomUserManager->SetUnitAllQuestInfo( FIRST_SENDER_UID, kPacket_.m_setQuestInfo ) == false )
	{
		START_LOG( cerr, L"모든 퀘스트 정보 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	if( m_spRoomUserManager->SetUnitGoingQuestInfo( FIRST_SENDER_UID, kPacket_.m_setGoingQuestInfo ) == false )
	{
		START_LOG( cerr, L"진행중 퀘스트 정보 실패.!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
}

//{{ 2008. 4. 2  최육사  근성도 회복 아이템
_IMPL_ON_FUNC( ERM_RESTORE_SPIRIT_NOT, KEGS_RESTORE_SPIRIT_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	BroadCast( ERM_RESTORE_SPIRIT_NOT, kPacket_ );
}
//}}

//{{ 2009. 5. 8  최육사		실시간 레벨업
IMPL_ON_FUNC( ERM_CHAR_LEVEL_UP_NOT )
{
	// 모든 상태 다 열어둬도 되남? 클라와 상의후 결정!
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );

	KEGS_CHAR_LEVEL_UP_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_ucLevel = kPacket_.m_kRoomUserInfo.m_ucLevel;
	kPacketNot.m_kBaseStat = kPacket_.m_kBaseStat;
	kPacketNot.m_kGameStat = kPacket_.m_kRoomUserInfo.m_kGameStat;
#ifdef SERV_ELESIS_UPDATE_EVENT
	kPacketNot.m_iNoteViewCount = kPacket_.m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT
	BroadCast( ERM_CHAR_LEVEL_UP_NOT, kPacketNot );
}
//}}

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
_IMPL_ON_FUNC( ERM_CHANGE_ROOM_USER_INFO_NOT, KRoomUserInfo )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );
	
	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );

	// 변경정보를 날리자
	BroadCast( ERM_CHANGE_ROOM_USER_INFO_NOT, kPacket_ );
}
//}}

//{{ 2009. 11. 30  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );

	// 변경정보를 날리자
	BroadCast( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacket_ );
}

#ifdef DUNGEON_ITEM
IMPL_ON_FUNC_NOPARAM( ERM_CHANGE_PVP_ITEMMODE_REQ )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_WAIT ) );

	KEGS_CHANGE_PVP_ITEMMODE_ACK kAck;
	KEGS_CHANGE_PVP_ITEMMODE_NOT kNot;

	kAck.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHANGE_PVP_ITEMMODE_ACK, kAck );

	m_bIsItemMode = !m_bIsItemMode;

	kNot = m_bIsItemMode;
	BroadCast( ERM_CHANGE_PVP_ITEMMODE_NOT, kNot );

	//방정보 업데인트
	SendRoomListInfo( NetError::ERR_ROOM_15 );
}
#endif DUNGEON_ITEM


#endif GUILD_SKILL_TEST
//}}


//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION

_IMPL_ON_FUNC( ERM_SEARCH_UNIT_REQ, KELG_SEARCH_UNIT_REQ )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	UidType iTargetGS = 0;

	switch( kPacket_.m_usEventID )
	{
	case EGS_GET_CONNECTION_UNIT_INFO_REQ:
		{
			if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_wstrNickName, iTargetGS, kPacket_.m_iUnitUID ) == false )
			{
				KELG_SEARCH_UNIT_ACK kPacket;
				kPacket.m_iOK		= NetError::ERR_SEARCH_UNIT_03; // 현재 접속 중이 아닙니다.
				kPacket.m_usEventID = kPacket_.m_usEventID;
				kPacket.m_kCUnitInfo.m_wstrCharName = kPacket_.m_wstrNickName;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SEARCH_UNIT_ACK, kPacket );
				return;
			}
		}
		break;

	case EGS_WATCH_UNIT_REQ:
		{
			if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_iUnitUID, iTargetGS ) == false )
			{
				KELG_SEARCH_UNIT_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_03; // 현재 접속 중이 아닙니다.
				kPacket.m_usEventID = kPacket_.m_usEventID;
				kPacket.m_kCUnitInfo.m_iUnitUID = kPacket_.m_iUnitUID;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SEARCH_UNIT_ACK, kPacket );
				return;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"이상한 eventid가 담겨져 왔습니다." )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;
		}
		return;
	}
	
	//찾는 캐릭이 있으면 해당 GS로 정보요청
	KELG_GET_UNIT_INFO_REQ kReq;
	kReq.m_usEventID		= kPacket_.m_usEventID;
	kReq.m_iDemandGSUID		= LAST_SENDER_UID;
	kReq.m_iDemandUserUID	= FIRST_SENDER_UID;
	kReq.m_wstrNickName		= kPacket_.m_wstrNickName;
	SendToGSCharacter( iTargetGS, kPacket_.m_iUnitUID, ERM_GET_UNIT_INFO_REQ, kReq );
}

_IMPL_ON_FUNC( ERM_GET_UNIT_INFO_ACK, KELG_GET_UNIT_INFO_ACK )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	KELG_SEARCH_UNIT_ACK kPacket;
	kPacket.m_iOK			= kPacket_.m_iOK;
	kPacket.m_usEventID		= kPacket_.m_usEventID;
	kPacket.m_kCUnitInfo	= kPacket_.m_kCUnitInfo;

	SendToGSCharacter( kPacket_.m_iDemandGSUID, kPacket_.m_iDemandUserUID, ERM_SEARCH_UNIT_ACK, kPacket );
}

#endif SERV_INTEGRATION
//}}

//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( ERM_SUMMON_PET_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

    // RoomUserInfo에 업데이트 한다!
	if( m_spRoomUserManager->UpdatePetInfo( kPacket_.m_iUnitUID, kPacket_.m_vecPetInfo ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// 브로드 캐스팅해서 펫 소환을 알린다!
    BroadCast( ERM_SUMMON_PET_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PET_ACTION_NOT )
{
	// [2012-12-07][최육사] 굳이 에어로그를 찍을 필요는 없음.
	VERIFY_STATE_WARN( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	BroadCast( ERM_PET_ACTION_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_NOT, KEGS_CHANGED_PET_SATIETY_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	// 펫 포만도 정보 업데이트	
	if( m_spRoomUserManager->UpdatePetSatiety( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_sCurrentSatiety ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_sCurrentSatiety )
			<< END_LOG;
		return;
	}

	BroadCast( ERM_CHANGED_PET_SATIETY_NOT, kPacket_ );    
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_INTIMACY_NOT, KEGS_CHANGED_PET_INTIMACY_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	// 펫 친밀도 정보 업데이트	
	if( m_spRoomUserManager->UpdatePetIntimacy( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_iCurrentIntimacy ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_iCurrentIntimacy )
			<< END_LOG;
		return;
	}

	BroadCast( ERM_CHANGED_PET_INTIMACY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT )
{
	// 펫 포만도 정보 업데이트
	if( m_spRoomUserManager->UpdatePetSatiety( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_sCurrentSatiety ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_sCurrentSatiety )
			<< END_LOG;
		return;
	}

	// 펫 친밀도 정보 업데이트	
	if( m_spRoomUserManager->UpdatePetIntimacy( kPacket_.m_iUnitUID, kPacket_.m_iPetUID, kPacket_.m_iCurrentIntimacy ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_iCurrentIntimacy )
			<< END_LOG;
		return;
	}

	BroadCast( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PET_EVOLUTION_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	// RoomUserInfo에 업데이트 한다!
	if( m_spRoomUserManager->UpdatePetInfo( kPacket_.m_iUnitUID, kPacket_.m_kEvolutionPetInfo ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// 브로드 캐스팅해서 펫 소환을 알린다!
	BroadCast( ERM_PET_EVOLUTION_NOT, kPacket_ );
}
#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
_IMPL_ON_FUNC( ERM_BUFF_SKILL_INFO_CONVEY_REQ, KEGS_BUFF_SKILL_INFO_CONVEY_REQ )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_PLAY ) );
	KEGS_BUFF_SKILL_INFO_CONVEY_ACK KAck;
	KAck.m_iOK = NetError::NET_OK;

	KEGS_BUFF_SKILL_INFO_CONVEY_NOT KNot;

	if( m_spRoomUserManager->ProcessBuffSkill( kPacket_, KNot) == false )		// 버프 상태 처리
	{	
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUFF_SKILL_INFO_CONVEY_ACK, KAck );

		START_LOG( cerr, L"버프 스킬 처리 실패!" )
			<< BUILD_LOG( kPacket_.m_iCastingUnitUID )
			<< BUILD_LOG( kPacket_.m_iBuffEnum )
			<< BUILD_LOG( kPacket_.m_fBuffTime )
			<< BUILD_LOG( kPacket_.m_bIsBuffCasting )
			<< END_LOG;

		return;
	}
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BUFF_SKILL_INFO_CONVEY_ACK, KAck );

	BroadCast( ERM_BUFF_SKILL_INFO_CONVEY_NOT, KNot );
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
_IMPL_ON_FUNC( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT )
{
	//{{ 2013. 04. 23	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	VERIFY_STATE( ( 5, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT, KRoomFSM::S_WAIT_FOR_DEFENCE ) );	
#else
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );	
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}	
	
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		//{{ 2012. 11. 21	박세훈	대회 채널 입장 조건
#if defined( SERV_FIX_ZOMBIE_OBSERVER ) || defined( SERV_TOURNAMENT_CONDITION )
		spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}
#else
		START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
#endif SERV_TOURNAMENT_CONDITION
		//}}
	}

	spRoomUser->UpdateBuffInfo( kPacket_.m_vecActivateBuffList );
	spRoomUser->UpdateGameStat( kPacket_.m_kGameStat );
	//{{ 2012. 12. 18	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	spRoomUser->SetBonusRate( kPacket_.m_mapBonusRate );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

	// 방안의 모든 유저들에게 보내자!
	BroadCast( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, kPacket_ );
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_USE_SKILL_BUFF_IN_PLAY_NOT, KEGS_USE_SKILL_BUFF_IN_PLAY_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_WAIT ) );

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	if( kPacket_.m_bOn )
	{
		spRoomUser->AddUseSkillBuffInPlay( kPacket_.m_iSkillID );
	}
	else
	{
		spRoomUser->DelUseSkillBuffInPlay( kPacket_.m_iSkillID );
	}

	// 방안의 모든 유저들에게 보내자!
	KEGS_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_iSkillID = kPacket_.m_iSkillID;
	kPacketNot.m_bOn = kPacket_.m_bOn;
	BroadCast( ERM_USE_SKILL_BUFF_IN_PLAY_BROAD_NOT, kPacketNot );
}

_IMPL_ON_FUNC( ERM_NEXT_RETURN_TO_FIELD_NOT, KERM_COMPLETE_RETURN_TO_FIELD_NOT )
{
	if( GetStateID() == KRoomFSM::S_INIT )
	{
		// 시간차로 방이 초기화 되었을때 패킷이 올수도 있다.
		return;
	}

	VERIFY_STATE( ( 1, KRoomFSM::S_RETURN_TO_FIELD ) );

	// 타이머 초기화!
	m_kTimer[TM_RETURN_TO_FIELD].restart();

	// 패킷 정리
	KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
	GetRoomInfo( kNot.m_RoomInfo );
	GetRoomSlotInfo( kNot.m_vecSlot );

	// 첫번째 멤버부터 이탈 시도!
	UidType iGSUID = 0;
	UidType iNextUnitUID = 0;
	if( m_spRoomUserManager->GetNextReturnToFieldUnitInfo( GetUID(), iGSUID, iNextUnitUID, kNot.m_kReturnToFieldInfo ) == false )
	{
		// 더이상 이탈 처리할 유저가 없으면 여기서 끝!!
		StateTransition( KRoomFSM::I_TO_WAIT );

		// 옛날 파티 재정렬!
		CheckReserveRegroupOldParty();
		return;
	}

	// 전송!
	SendToGSCharacter( iGSUID, iNextUnitUID, ERM_RESULT_ALL_UNIT_SUCCESS_NOT, kNot );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
_IMPL_ON_FUNC( ERM_UPDATE_USER_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_USER_UNIT_BUFF_INFO_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"버프 정보를 보내려는 캐릭터는 현재 이방안에 없습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecBuffFactor.size() )
			<< END_LOG;
		return;
	}

	KEGS_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_vecBuffFactor = kPacket_.m_vecBuffFactor;

	// ToUnitUID값이 0이면 브로드 캐스팅
	if( kPacket_.m_iToUnitUID == 0 )
	{
		BroadCast( ERM_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
	}
	else
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iToUnitUID );
		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cerr, L"존재하지 않는 유저에게 버프정보를 보내려고 했다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iToUnitUID )
				<< END_LOG;
			return;
		}

		SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_USER_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
	}
}

_IMPL_ON_FUNC( ERM_UPDATE_NPC_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"버프 정보를 보내려는 캐릭터는 현재 이방안에 없습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecNpcUnitBuff.size() )
			<< END_LOG;
		return;
	}

	KEGS_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT kPacketNot;
	kPacketNot.m_vecNpcUnitBuff = kPacket_.m_vecNpcUnitBuff;

//#ifdef  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	// ToUnitUID값이 0이면 브로드 캐스팅
	if( kPacket_.m_vecToUnitUID.empty() == true )
	{
		BroadCast( ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
	}
	else
	{
        BOOST_TEST_FOREACH( UidType, uidUnitUID, kPacket_.m_vecToUnitUID )
        {
		    KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( uidUnitUID );
		    if( IS_NULL( spRoomUser ) )
		    {
			    START_LOG( cerr, L"존재하지 않는 유저에게 버프정보를 보내려고 했다!" )
				    << BUILD_LOG( GetUID() )
				    << BUILD_LOG( FIRST_SENDER_UID )
				    << BUILD_LOG( uidUnitUID )
				    << END_LOG;
		    }
            else
            {
#ifdef  SERV_BATTLE_FIELD_SYSTEM
                spRoomUser->SetBattleFieldNpcSyncSubjects( false );
#endif  SERV_BATTLE_FIELD_SYSTEM
    		    SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
            }
        }
	}


//#else   SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//
//	// ToUnitUID값이 0이면 브로드 캐스팅
//	if( kPacket_.m_iToUnitUID == 0 )
//	{
//		BroadCast( ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
//	}
//	else
//	{
//		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iToUnitUID );
//		if( IS_NULL( spRoomUser ) )
//		{
//			START_LOG( cerr, L"존재하지 않는 유저에게 버프정보를 보내려고 했다!" )
//				<< BUILD_LOG( GetUID() )
//				<< BUILD_LOG( FIRST_SENDER_UID )
//				<< BUILD_LOG( kPacket_.m_iToUnitUID )
//				<< END_LOG;
//			return;
//		}
//
//		SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_NPC_UNIT_BUFF_INFO_BROAD_NOT, kPacketNot );
//	}
//
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

}
#endif SERV_SERVER_BUFF_SYSTEM
//}}


//{{ 2012. 07. 27	최육사		사제시스템 EXP 보너스 버그 수정
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
IMPL_ON_FUNC( ERM_UPDATE_TUTORIAL_INFO_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );
}
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
//}}

//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
_IMPL_ON_FUNC( ERM_VISIT_CASH_SHOP_NOT, KEGS_VISIT_CASH_SHOP_NOT )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	KERM_UPDATE_VISIT_CASH_SHOP_NOT kPacket;
	kPacket.m_bEnterCashShop = kPacket_.m_bEnterCashShop;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	spRoomUser->UpdateUserCashShopInfo( kPacket.m_bEnterCashShop );

	// 방안의 모든 유저들에게 보내자!
	BroadCast( ERM_UPDATE_VISIT_CASH_SHOP_NOT, kPacket );
}
#endif SERV_VISIT_CASH_SHOP
//}}

//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
_IMPL_ON_FUNC( ERM_UDP_PING_SCORE_NOT, KEGS_UDP_PING_SCORE_NOT )
{
	//{{ 2013. 04. 24	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	VERIFY_STATE( ( 5, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT, KRoomFSM::S_WAIT_FOR_DEFENCE ) );
#else
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}	

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//m_bForceHost = kPacket_.m_bForceHost;
	//m_spRoomUserManager->SetPingScore( FIRST_SENDER_UID, m_bForceHost, kPacket_.m_dwPingScore );
    m_spRoomUserManager->SetPingScore( FIRST_SENDER_UID, kPacket_.m_bForceHost, kPacket_.m_dwPingScore );
#else//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	m_spRoomUserManager->SetPingScore( FIRST_SENDER_UID, kPacket_.m_dwPingScore );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
}
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
_IMPL_ON_FUNC( ERM_FIELD_USER_FRAME_INFO_NOT, int )
{
	VERIFY_STATE_WARN( ( 1, KRoomFSM::S_PLAY ) );

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	spRoomUser->SetFieldFrame( kPacket_ );
}
#endif SERV_FIELD_FRAME_LOG
//}}

//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
_IMPL_ON_FUNC( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, KRoomUserInfo )
{
	VERIFY_STATE( ( 4, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ) );

	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_ );

	// 변경정보를 날리자
	KEGS_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT kPacket;
	kPacket.m_iUnitUID = FIRST_SENDER_UID;
	kPacket.m_kGameStat = kPacket_.m_kGameStat;
	BroadCast( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, kPacket );
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-08
IMPL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	KRoomUserPtr spUser = m_spRoomUserManager->GetUser( kPacket_.m_iTargetUnitUID );

	if( spUser != NULL )
	{
		SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket_ );
		return;
	}

	KERM_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_LOCALRANKING_11;
	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ERM_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ERM_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket_ );
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
_IMPL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	// RoomUserInfo에 업데이트 한다!
	if( m_spRoomUserManager->SetRidingPetInfo( kPacket_.m_iUnitUID, kPacket_.m_iRidingPetUID, kPacket_.m_usRidingPetID ) == false )
	{
		START_LOG( cerr, L"라이딩 펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// 브로드 캐스팅해서 펫 소환을 알린다!
	BroadCast( ERM_SUMMON_RIDING_PET_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT )
{
	VERIFY_STATE( ( 2, KRoomFSM::S_WAIT, KRoomFSM::S_PLAY ) );

	// RoomUserInfo에 업데이트 한다!
	if( m_spRoomUserManager->SetRidingPetInfo( kPacket_.m_iUnitUID, 0, 0 ) == false )
	{
		START_LOG( cerr, L"라이딩 펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// 브로드 캐스팅해서 펫 소환을 알린다!
	BroadCast( ERM_UNSUMMON_RIDING_PET_NOT, kPacket_ );
}
#endif	// SERV_RIDING_PET_SYSTM

//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
_IMPL_ON_FUNC( ERM_FORCE_RELAY_NOT, KEGS_FORCE_RELAY_NOT )
{
	if( m_pkCurrentState->CheckState( 1, KRoomFSM::S_PLAY ) == false )
	{
		return;
	}

	if( m_spRoomUserManager->CheckAuthLevel( kPacket_.m_iTargetUnitUID ) == true )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_iTargetUnitUID );
	if( spRoomUser == NULL )
	{
		return;
	}

	// 신고 당한 유저 -> 신고자로의 패킷 흐름을 릴레이로 변경할 것을 강요한다.
	kPacket_.m_iTargetUnitUID = FIRST_SENDER_UID;
	UidType iTargetUnitUID = spRoomUser->GetCID();
	SendToGSCharacter( spRoomUser->GetGSUID(), iTargetUnitUID, ERM_FORCE_RELAY_NOT, kPacket_ );

	// 신고 당한 유저의 감시 요청을 보낸다.
	if( ( SiKGameSysVal()->GetLanBugOutCheck() == true ) &&
		( IsLanBugCheckUser( iTargetUnitUID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_CHECK_NOT, GetUID(), iTargetUnitUID );
		AddLanBugCheckUser( iTargetUnitUID );
	}

	// 신고자의 검증 요청도 보내자!!
	if( ( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
}

_IMPL_ON_FUNC( ERM_ADD_UDP_CHECK_KICK_USER_NOT, UidType )
{
	RemoveLanBugCheckUser( kPacket_ );
	RemoveLanBugVerifyUser( kPacket_ );

	if( m_pkCurrentState->CheckState( 1, KRoomFSM::S_PLAY ) == false )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_ );
	if( spRoomUser == NULL )
	{
		return;
	}

	// 추방 로그 기록
	KDBE_UDP_CHECK_KICK_USER_NOT kPacket;
	kPacket.m_iRoomUID			= GetUID();
	if( m_cRoomType == CXSLRoom::RT_PVP )
	{
		kPacket.m_iPVPChannelClass	= GetPVPChannelClass();
		kPacket.m_iMatchType		= GetMatchType();
	}

	kPacket.m_iUnitUID = kPacket_;
	SendToLogDB( DBE_UDP_CHECK_KICK_USER_NOT, kPacket );

	if( SiKGameSysVal()->GetLanBugOutJustLog() == true )
	{
		return;
	}

	SendToGSCharacter( spRoomUser->GetGSUID(), kPacket_, ERM_UDP_KICK_STATE_CHANGE_FIELD_NOT );
}

_IMPL_ON_FUNC( ERM_REMOVE_LANBUG_CHECK_USER_NOT, UidType )
{
	RemoveLanBugCheckUser( kPacket_ );
}

_IMPL_ON_FUNC( ERM_REMOVE_LANBUG_VERIFY_USER_NOT, UidType )
{
	RemoveLanBugVerifyUser( kPacket_ );
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
_IMPL_ON_FUNC( ERM_COUPLE_MAKING_SUCCESS_NOT, KERM_COUPLE_MAKING_SUCCESS_NOT )
{
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser != NULL )
	{
		spRoomUser->SetLoverUnitUID( ( FIRST_SENDER_UID == kPacket_.m_iRequestUnitUID ) ? kPacket_.m_iAcceptUnitUID : kPacket_.m_iRequestUnitUID );
	}

	KEGS_COUPLE_MAKING_SUCCESS_NOT kPacket;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	BroadCast( ERM_COUPLE_MAKING_SUCCESS_NOT, kPacket );
}

IMPL_ON_FUNC_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT )
{
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser != NULL )
	{
		spRoomUser->SetLoverUnitUID( 0 );
	}
}

_IMPL_ON_FUNC( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT )
{
	BroadCast( ERM_RELATIONSHIP_EFFECT_NOT, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}
