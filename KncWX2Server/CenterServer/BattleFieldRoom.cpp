//#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
#include <WinSock2.h>	// windows.h redefinition 오류 대처용
#pragma pack( push, 8 )
//#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK
#include "BattleFieldRoom.h"
#include "NetError.h"
#include "Room/RoomFSM.h"
#include "X2Data/XSLRoom.h"
#include "KDropTable.h"
#include "ResultProcess.h"
#include "CnRoomManager.h"

#include "x2data/XSLItemManager.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLUnitManager.h"
#include "KAttribNpcTable.h"

//{{ 2009. 5. 22  최육사	드롭률이벤트
#include "GameEvent/GameEventManager.h"
//}}
#include "GameSysVal/GameSysVal.h"
//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	#include "CnHackingCheckManager.h"
#endif SERV_CN_SERVER_HACKING_CHECK
//}}
//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	#include "DungeonEventManager.h"
#endif SERV_TIME_DROP_MONSTER_EVENT
//}}

//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// 릴레이 서버(현재는 룸 서버) IP
#include "CnNetLayer.h"				// 릴리이 서버(현재는 룸 서버) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	#include "InteriorUdpSession.h"
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-31	// 박세훈
	#include "X2Data/XSLFieldBossData.h"

#ifdef _CONVERT_VS_2010// 작업날짜: 2013-12-03	// 박세훈
#include <intsafe.h>
#else // _CONVERT_VS_2010
#define	BYTE_MAX	0xff
#endif // _CONVERT_VS_2010

#endif // SERV_BATTLE_FIELD_BOSS


DWORD KBattleFieldRoom::m_dwMonsterRespawnCheckTick = 0;


NiImplementRTTI( KBattleFieldRoom, KRoom );

#define CLASS_TYPE KBattleFieldRoom

KBattleFieldRoom::KBattleFieldRoom()
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	: m_byteBossFieldState( 0 )
	, m_tFieldHoldingTime( 0 )
	, m_tFieldOpenTime( 0 )
	, m_tFieldCloseTime( 0 )
#endif // SERV_BATTLE_FIELD_BOSS
{
	//{{ 2011. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_spUserManager = KBattleFieldUserManagerPtr( new KBattleFieldUserManager );
	SetRoomUserManager( m_spUserManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_spUserManager->Init( CXSLRoom::RT_BATTLE_FIELD, SEnum::BATTLE_FIELD_MAX_NUM );

	m_bEndPlay				= false;
	m_cGetItemType			= CXSLRoom::DGIT_RANDOM;

	//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	m_fItemDropEventProbRate = REE_DROP_EVENT_PROB_DEFAULT;
#else // SERV_DROP_EVENT_RENEWAL
	m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
	//}}
	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	m_fQuestItemDropEventProbRate = 1.f;
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}

}

KBattleFieldRoom::~KBattleFieldRoom()
{
}

ImplToStringW( KBattleFieldRoom )
{
    return START_TOSTRING_PARENTW( KRoom );
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-12	// 박세훈
void KBattleFieldRoom::Tick()
{
    KRoom::Tick();

	switch( GetStateID() )
	{
	case KRoomFSM::S_WAIT:
		if( m_tTimer[TE_WAIT_TIMER].elapsed() > 10.0 )
		{
			// 배틀필드 방의 대기 시간이 10초를 넘으면 방을 소멸 시킨다!
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
		break;

	case KRoomFSM::S_PLAY:
		if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
		{
			CheckFieldBossSystem();
		}
		else
		{
			// 리스폰 몬스터를 체크합니다.
			CheckMonsterRespawn();

			//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			// 중간 보스 몬스터 체크!
			CheckMiddleBossMonster();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}

			//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
			CheckEventMonster();
#endif SERV_FIELD_EVENT_MONSTER
			//}}
		}

		// 난입자 동기화 체크
		CheckIntrudeUserLoadComplete();

		// 몬스터 P2P동기화 유저 체크
		CheckNpcP2PSyncUser();

		// 좀비 유저 체크 에코 패킷
		CheckZombieUserEcho();

		//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
		CheckDropEventCount();
#endif SERV_ITEM_DROP_EVENT
		//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
		if ( m_spUserManager->ZombieAlert_Tick() == true )
		{
#ifdef SERV_CHOOSE_FASTEST_HOST
			CheckPingScore();
			m_kTimer[TM_CHECK_CHANGE_HOST].restart();
#endif  SERV_CHOOSE_FASTEST_HOST
		}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
		break;
	}
}
#else // SERV_BATTLE_FIELD_BOSS
void KBattleFieldRoom::Tick()
{
	KRoom::Tick();

	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		//////////////////////////////////////////////////////////////////////////
		// 컨티뉴 타이머 체크
		CheckContinueTimer();
		//////////////////////////////////////////////////////////////////////////

		// 국지 이벤트를 체크합니다.
		CheckDangerousEvent();

		// 리스폰 몬스터를 체크합니다.
		CheckMonsterRespawn();

		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		// 중간 보스 몬스터 체크!
		CheckMiddleBossMonster();
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

		//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
		CheckEventMonster();
#endif SERV_FIELD_EVENT_MONSTER
		//}}

		// 난입자 동기화 체크
		CheckIntrudeUserLoadComplete();

		// 몬스터 P2P동기화 유저 체크
		CheckNpcP2PSyncUser();

		// 좀비 유저 체크 에코 패킷
		CheckZombieUserEcho();

		//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
		CheckDropEventCount();
#endif SERV_ITEM_DROP_EVENT
		//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
		if ( m_spUserManager->ZombieAlert_Tick() == true )
		{
#ifdef SERV_CHOOSE_FASTEST_HOST
			CheckPingScore();
			m_kTimer[TM_CHECK_CHANGE_HOST].restart();
#endif  SERV_CHOOSE_FASTEST_HOST
		}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	}
	else if( GetStateID() == KRoomFSM::S_WAIT )
	{
		if( m_tTimer[TE_WAIT_TIMER].elapsed() > 10.0 )
		{
			// 배틀필드 방의 대기 시간이 10초를 넘으면 방을 소멸 시킨다!
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
	}
}
#endif // SERV_BATTLE_FIELD_BOSS

void KBattleFieldRoom::SetRoomInfo( const KRoomInfo& kInfo )
{
    m_cRoomType			= kInfo.m_RoomType;
    m_uiRoomListID		= kInfo.m_uiRoomListID;
    m_bPublic			= kInfo.m_bPublic;
    m_bTeamBalance		= false;
    m_wstrPassword		= kInfo.m_wstrPassword;
    m_cGameType			= CXSLRoom::PGT_TEAM;
    m_cWinningNumKill	= 0;
    m_sWorldID			= kInfo.m_WorldID;
    m_cDifficultyLevel	= kInfo.m_DifficultyLevel;
    m_iDungeonID		= kInfo.m_iDungeonID;
	m_cDungeonMode		= kInfo.m_cDungeonMode;

	if( kInfo.m_cGetItemType <= CXSLRoom::DGIT_NONE || kInfo.m_cGetItemType >= CXSLRoom::DGIT_END )
	{
		START_LOG( cwarn, L"던전게임 아이템 습득타입이 잘못되었음.!" )
			<< BUILD_LOGc( m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_iDungeonID )
			<< BUILD_LOGc( kInfo.m_DifficultyLevel )
			<< END_LOG;

		m_cGetItemType		= CXSLRoom::DGIT_RANDOM;		
	}
	else
	{
		m_cGetItemType		= kInfo.m_cGetItemType;
	}

	SetName( kInfo.m_RoomName );
	SetPlayTimeLimit( kInfo.m_fPlayTime );
	SetBattleFieldID( static_cast<SEnum::BATTLE_FIELD_ID>(kInfo.m_iBattleFieldID) );
	m_spUserManager->SetMaxSlot( SiCXSLBattleFieldManager()->GetMaxJoinUserCountInBattleField( static_cast<SEnum::BATTLE_FIELD_ID>(kInfo.m_iBattleFieldID) ) );
}

void KBattleFieldRoom::GetRoomInfo( KRoomInfo& kInfo )
{
	KRoom::GetRoomInfo( kInfo );
    kInfo.m_iBattleFieldID = GetBattleFieldID();
	kInfo.m_MaxSlot = m_spUserManager->GetMaxSlot(); // 해당 배틀필드만의 최대 슬롯 갯수를 가져오자!
}

//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
void KBattleFieldRoom::LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom )
{
	if( kInfo.m_vecGamePlayNetworkInfo.empty() == false )
	{
		CTime tCurTime = CTime::GetCurrentTime();
		KELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT kPacketToLog;

		BOOST_TEST_FOREACH( const KGamePlayNetworkInfo&, kGamePlayNetworkInfo, kInfo.m_vecGamePlayNetworkInfo )
		{
			if( kGamePlayNetworkInfo.m_iOpponentUnitUID == iUnitUID )
				continue;

			KDBUpdateGamePlayNetworkInfo kDBUpdateInfo;
			kDBUpdateInfo.m_iGameType			  = m_cRoomType;
			kDBUpdateInfo.m_iHolePunchingTryCount = kGamePlayNetworkInfo.m_iHolePunchingTryCount;
			kDBUpdateInfo.m_iP2PPlayTime		  = static_cast<int>(kGamePlayNetworkInfo.m_fP2PPlayTime);		// int로 타입 캐스팅하면 초만 남는다.
			kDBUpdateInfo.m_iRelayPlayTime		  = static_cast<int>(kGamePlayNetworkInfo.m_fRelayPlayTime);	// int로 타입 캐스팅하면 초만 남는다.
			kDBUpdateInfo.m_wstrRelayServerIP	  = NetCommon::GetLocalIPW();
			kDBUpdateInfo.m_wstrRegDate			  = (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:%M:%S"));
			kPacketToLog.m_vecDBUpdateInfo.push_back( kDBUpdateInfo );
		}

		if( kPacketToLog.m_vecDBUpdateInfo.empty() == false )
		{
			SendToLogDB( ELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT, kPacketToLog );
		}
	}
}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

void KBattleFieldRoom::ProcessNativeEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
	CASE( ERM_OPEN_BATTLE_FIELD_REQ );
	CASE( ERM_JOIN_BATTLE_FIELD_REQ );

   _CASE( ERM_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT, KEGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT );

   _CASE( ERM_BATTLE_FIELD_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ );

	CASE( ERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_REQ, KERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, KERM_LEAVE_ROOM_REQ );

   _CASE( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ );
   _CASE( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ );
   _CASE( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ );
    CASE_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ );
   
   _CASE( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ );
	//{{ 2009. 5. 11  최육사	실시간아이템
	CASE( ERM_GET_ITEM_COMPLETE_NOT );
	//}}
   _CASE( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT );   
   _CASE( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT );

   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );

   _CASE( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ );
	//CASE_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT );
	
   _CASE( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ );

   //{{ 2011. 01. 25  김민성  월드 트리거 전달(던전만 적용중 - 이후 PVP 확장)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
   _CASE( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif SERV_WORLD_TRIGGER_RELOCATION
   //}}
	
	CASE_NOPARAM( ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ );
//#ifndef SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//	CASE_NOPARAM( ERM_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT );
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	CASE( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT );

	//{{ 2011. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( ERM_DUMP_ROOM_MONSTER_NOT );
	CASE( ERM_DUMP_BATTLE_FIELD_NOT );
	CASE_NOPARAM( ERM_BATTLE_FIELD_ZU_ECHO_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    CASE_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	//}}
	//{{ 배틀필드 위험도 확인 치트 - 김민성
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
	CASE_NOPARAM( ERM_BATTLE_FIELD_DANGEROUS_VALUE_REQ );
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
	//}}
	//{{ 배틀필드 커뮤니티 리스트 확인 - 김민성
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
	_CASE( ERM_COMMUNITY_IN_BATTLE_FIELD_REQ, KEGS_COMMUNITY_USER_LIST_REQ );
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
	//}}
	//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	CASE( ERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}
	//{{ 2013. 03. 12	최육사	몬스터 유효 검사
#ifdef SERV_CHECK_EXIST_MONSTER_UID
	_CASE( ERM_CHECK_EXIST_MONSTER_UID_REQ, KEGS_CHECK_EXIST_MONSTER_UID_REQ );
#endif SERV_CHECK_EXIST_MONSTER_UID
	//}}
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	_CASE( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ, KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ );
	_CASE( ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ, KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ );
	_CASE( ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	CASE( ERM_CALL_MY_LOVER_CHECK_ROOM_REQ );
	_CASE( ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_REQ, KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ );
	CASE( ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ );
	CASE( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-09	// 박세훈
	_CASE( ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_REQ, KERM_LEAVE_ROOM_REQ );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-07	// 박세훈
	CASE_NOPARAM( ERM_BOSS_FIELD_INTRUDE_RESTRICTION_REQ );
	CASE( ERM_BOSS_FIELD_LOG_NOT );
#endif // SERV_BATTLE_FIELD_BOSS

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음 - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
    }
}

void KBattleFieldRoom::StartGame()
{
    KRoom::StartGame();

	// 배틀필드는 남는시간체크를 하지 않습니다.
	EnableRemainingPlaytime( false );
	
	const int iPlayerCount = m_spUserManager->GetNumMember();
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-14	// 박세훈
	m_kMonsterManager.StartGame( GetBattleFieldID(), iPlayerCount, m_kGameManager.GetDangerousValue(), SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) );
#else // SERV_BATTLE_FIELD_BOSS
	m_kMonsterManager.StartGame( GetBattleFieldID(), iPlayerCount, m_kGameManager.GetDangerousValue() );
#endif // SERV_BATTLE_FIELD_BOSS
	m_kSecurityManager.StartGame();
	m_kGameManager.StartGame();

	m_nDropItemUID		= 1;
	m_fTotalPlayTime	= 0.0;	

	m_mapDropItem.clear();
    
    const int iPlayingUserCount = m_spUserManager->GetNumPlaying();
    if( iPlayingUserCount < 1 || iPlayingUserCount > 12 )
    {
        START_LOG( cerr, L"플레이 중인 유저 수가 이상함." )
            << BUILD_LOG( iPlayingUserCount )
            << END_LOG;
    }

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	// 해당 유저가 특정 아이템 착용 퀘스트를 진행중이면서 아이템을 착용하고 있는지 확인한다.
	m_spUserManager->CheckItemEquipQuestCondition( true );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

}

void KBattleFieldRoom::StartPlay()
{
	//{{ 2010. 8. 25	최육사	던전 플레이 상태 체크!
	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		if( m_spUserManager->GetNumMember() == 1 )
		{
			START_LOG( cout, L"이미 KRoomFSM::S_PLAY상태인데 또 StartPlay를 하려고 하네!? 해킹인가?" )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetTotalPlayTime() )
				<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
				<< BUILD_LOG( GetDungeonIDAndDif() );

			KRoomUserPtr spUser = m_spUserManager->GetHostUser();
			if( spUser != NULL )
			{
				START_LOG( cout, L"어떤 유저가 이렇게 이상한 동작을 하는지 확인해보자!" )
					<< BUILD_LOG( spUser->GetCID() )
					<< BUILD_LOG( spUser->GetNickName() );

				// 파티장에게 해킹체크패킷을 보내자!
				//KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				//kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_PLAY_START;
				//SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
			}
		}
	}
	//}}

    KRoom::StartPlay();

}

void KBattleFieldRoom::EndPlay()
{
	m_bEndPlay = false;
    KRoom::EndPlay();

    m_fTotalPlayTime = m_kTimer[TM_PLAY].elapsed();

	// 플레이 타임이 정상적으로 계산되어 나오는지 확인해보자!
	if( GetTotalPlayTime() < 0.0 )
	{
		START_LOG( cout, L"로딩시간을 뺀 실제 던전 플레이 시간이 마이너스값이다! 있을 수 없는 에러!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( GetTotalPlayTime() )
			<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
			<< BUILD_LOG( GetDungeonIDAndDif() )
			<< BUILD_LOG( m_spUserManager->GetNumMember() )
			<< END_LOG;
	}
}

void KBattleFieldRoom::EndGame()
{
	// 부활석 사용 통계
	const int iPlayerCount = m_spUserManager->GetUsedRessurectionStonePlayerCount();

	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_RES_STONE, kKey, KStatistics::eSIColResStone_PlayerCount, iPlayerCount );
	
    KRoom::EndGame();

	m_kMonsterManager.EndGame();
	m_kSecurityManager.EndGame();
	m_kGameManager.EndGame();

	// 설마하는 마음에 이곳에서 다시 초기화를 해본다.(07.12.28 demiahun)
	m_mapDropItem.clear();
}

int KBattleFieldRoom::DecideWinTeam()
{
	// 배틀필드는 승패결정이 없다! 만약 호출된다고 해도 몬스터가 승리한것으로 처리하자!
	return CXSLRoom::TN_MONSTER;
}

int KBattleFieldRoom::GetPlayMode()
{
    return m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
}

void KBattleFieldRoom::HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS )
{
    if( mapUserCIDNGS.empty() )
    {
        return;
    }

    std::map< UidType, UidType >::iterator mit;
    for( mit = mapUserCIDNGS.begin(); mit != mapUserCIDNGS.end(); ++mit )
    {
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"해당 유저가 존재하지 않습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		const UidType iUnitUID = mit->first;
		const UidType iPartyUID = spRoomUser->GetPartyUID();

		if( !m_spUserManager->LeaveRoom( mit->first ) )
		{
			START_LOG( cerr, L"방 나가기 실패." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}

        // 문제 유저에게 패킷을 날린다.
        SendToGSCharacter( mit->second, mit->first, ERM_GAME_LOADING_TIME_OUT_NOT );

		// 방의 모든 유저들에게 슬롯 정보를 날린다.
		KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iUnitUID;
		kPacketNot.m_iPartyUID = iPartyUID;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
    }    
	
	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"배틀필드에서는 사용하지 않는 상태입니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( mapUserCIDNGS.size() )
			<< END_LOG;
	}

    // 방에서 모든 유저가 나갔으면 방을 일단 WAIT상태로 이동시킨다!
    if( IsEmpty() )
    {
		StateTransition( KRoomFSM::I_TO_RESULT );
        StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
    }
}

void KBattleFieldRoom::DeleteZombie( std::vector<UidType>& vecZombieUnit )
{
	START_LOG( cout2, L"[알림] 배틀필드 좀비 유저 삭제!" )
		<< BUILD_LOG( vecZombieUnit.size() );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( iUnitUID );
		if( IS_NOT_NULL( spUser ) )
		{
			START_LOG( cout2, L"[알림] 배틀필드 좀비 유저 삭제!" )
				<< BUILD_LOG( spUser->GetNickName() )
				<< BUILD_LOG( spUser->IsHost() );

			KERM_CHECK_INVALID_ZOMBIE_USER_NOT kPacket;
			kPacket.m_iRoomUID = GetUID();
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_INVALID_ZOMBIE_USER_NOT, kPacket );
		}
		else
		{
			START_LOG( cerr, L"룸유저 포인터 이상.!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
		}

		if( m_spUserManager->LeaveRoom( iUnitUID ) == false )
		{
			START_LOG( cerr, L"좀비 유저 방 나가기 실패." )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( vecZombieUnit.size() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}

	// 방에서 모든 유저가 나갔으면 방을 닫는다.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
		{
			// 방의 모든 유저들에게 슬롯 정보를 날린다.
			KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
			kPacketNot.m_iReason = 0;
			kPacketNot.m_cRoomState = GetStateID();
			kPacketNot.m_iUnitUID = iUnitUID;
			kPacketNot.m_iPartyUID = 0;
			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
		}
	}
}

void KBattleFieldRoom::CloseRoom()
{
	// 방이 닫힐때 정리할것들을 정리하자.
	//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kMonsterManager.OnCloseRoom();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// 배틀필드 방이 닫힐때 좀비체크를 끄자
	m_spUserManager->ZU_CheckEnd();
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckEnd();
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_DELETE );

	//방생성중 조건이 안되어 생성을 못하는 경우라면
	if( GetStateID() == KRoomFSM::S_INIT )
	{
		StateTransition( KRoomFSM::I_TO_WAIT );
	}

	StateTransition( KRoomFSM::I_TO_CLOSE );

	//{{ 2013. 01. 07	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	// 초기화 하기 전에 검사 한번만~
	if( m_spUserManager->GetNumTotalParty() != 0  ||  m_spUserManager->GetNumMemberTotalParty() != 0 )
	{
		START_LOG( cout, L"[경고] 배틀필드를 닫을려고 하는데 삭제되어있어야할 정보가 아직 살아있다! 이래서 버그가.." )
			<< BUILD_LOG( m_spUserManager->GetNumTotalParty() )
			<< BUILD_LOG( m_spUserManager->GetNumMemberTotalParty() );
	}

	// 방을 닫기 전에 초기화 한번 해주자!
	m_spUserManager->Reset();
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-12-03	// 박세훈
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
	{
		// 로그 정보 DB 전송 후 초기화
		SendToLogDB( DBE_BOSS_FIELD_LOG_NOT, m_kBossFieldLog );
		m_kBossFieldLog.clear();
	}
#endif // SERV_BATTLE_FIELD_BOSS

	KCnBaseRoom::CloseRoom();
}

KDropItemData KBattleFieldRoom::CreateItemProcess( IN const int iItemID, 
												   IN const char cNpcLevel,
												   IN const int iNpcMaxHP,
												   IN const std::map< UidType, float >& mapDamageByUnit,
												   IN const bool bIsGP /*= false*/, 
												   IN const int iED /*= 0*/ )
{
	KDropItemData	kDropItem;	//not 패킷을 들어갈 개별 아이템정보
	ITEM_DATA		sItemData;	//서버에서 가지고있을 아이템정보

	int iDropItemUID = m_nDropItemUID++;

	kDropItem.m_iItemID					= iItemID;
	kDropItem.m_iDropItemUID			= iDropItemUID;
	{
		std::map< UidType, float >::const_iterator mit;
		for( mit = mapDamageByUnit.begin(); mit != mapDamageByUnit.end(); ++mit )
		{
			kDropItem.m_setGetPossibleUnitUID.insert( mit->first );
		}
	}	

	if( (rand()%2) == 1 )
		kDropItem.m_bLeft = true;
	else
		kDropItem.m_bLeft = false;

	kDropItem.m_fSpeed	= (float)(rand()%800) + 1.0f;	

	//아이템을 떨어뜨리고 아직 먹지 않은것으로 체크,.
	sItemData.m_iItemID	 	 	= iItemID;
	sItemData.m_iDropItemUID 	= iDropItemUID;
	sItemData.m_bIsGet		 	= false;
	sItemData.m_bIsGP		 	= bIsGP;
	sItemData.m_cNpcLevel		= cNpcLevel;
	sItemData.m_iNpcMaxHP		= iNpcMaxHP;
	sItemData.m_mapDamageByUser	= mapDamageByUnit;

	if( bIsGP == true )
	{
		sItemData.m_iED = iED;
	}
	//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	else
	{
		SiKDropTable()->EnchantItemDropEventProcess( iItemID, sItemData.m_cEnchantLevel, sItemData.m_bNotify );
		kDropItem.m_cEnchantLevel = sItemData.m_cEnchantLevel;
	}
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	m_mapDropItem.insert( std::make_pair( iDropItemUID, sItemData ) );

	return kDropItem;
}

void KBattleFieldRoom::CheckLoadingSuccess()
{
	// 아무것도 하지 않습니다. 로딩상태를 쓰지 않기 때문!
}

void KBattleFieldRoom::CheckMonsterRespawn()
{
	// 일정 간격으로 몬스터 리스폰을 체크합니다!
	if( m_tTimer[TE_MONSTER_RESPAWN].elapsed() < 1.0 )
		return;

	m_tTimer[TE_MONSTER_RESPAWN].restart();

	// 리스폰 필요 정보
	//const int iPlayerCount = m_spUserManager->GetNumMember();
	const int iPlayerCount = SiCXSLBattleFieldManager()->GetMaxJoinUserCountInBattleField( m_eBattleFieldID ); // 일단 해당 배틀필드에 최대 입장 가능한 인원수 만큼 있다고 가정한다.

	//////////////////////////////////////////////////////////////////////////
	// 몬스터 리스폰 최대 지연시간 체크
	DWORD dwElapTime = ::GetTickCount();  

	// 몬스터 리스폰 체크
	KEGS_NPC_UNIT_CREATE_NOT kPacketNot;
	kPacketNot.m_iBattleFieldID = GetBattleFieldID();
	if( m_kMonsterManager.CheckRespawnMonster( GetBattleFieldID(), 
											   iPlayerCount, 
											   m_kGameManager.GetDangerousEventInfo(), 
											   kPacketNot ) == true )
	{
		if( kPacketNot.m_vecNPCUnitAck.empty() == false )
		{
			// 몬스터 리스폰 처리가 완료되었다면 
			BroadCast( ERM_NPC_UNIT_CREATE_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( m_dwMonsterRespawnCheckTick < dwElapTime )
	{
		START_LOG( cout, L"[알림]몬스터 리스폰 체크 타임 최대 시간 갱신!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( dwElapTime )
			<< BUILD_LOG( m_dwMonsterRespawnCheckTick );

		m_dwMonsterRespawnCheckTick = dwElapTime;
	}
	//////////////////////////////////////////////////////////////////////////	
}

void KBattleFieldRoom::CheckDangerousEvent()
{
	// 일정 간격으로 국지 이벤트를 체크합니다.
	if( m_tTimer[TE_DANGEROUS_EVENT].elapsed() < 5.0 )
		return;

	m_tTimer[TE_DANGEROUS_EVENT].restart();

	if( m_kGameManager.CheckAndDeleteReservedDangerousEvent( KDangerousEventInfo::DE_WARNING_MESSAGE ) == true )
	{
		KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT kNot;
		kNot.m_cEventType = KEGS_BATTLE_FIELD_EVENT_MESSAGE_NOT::BET_WARNING_MESSAGE;
		BroadCast( ERM_BATTLE_FIELD_EVENT_MESSAGE_NOT, kNot, KRoomUserManager::UL_NPC_LOAD_USER );
	}
}

void KBattleFieldRoom::CheckContinueTimer()
{
    // 컨티뉴 타이머를 체크하여 9초가 지나면 
}

bool KBattleFieldRoom::IsDropDieState( char cState )
{
	switch( cState )
	{
	case KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER:
	case KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF:
	case KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP:
		return true;

	case KEGS_NPC_UNIT_DIE_REQ::NDS_ALLY:
	case KEGS_NPC_UNIT_DIE_REQ::NDS_ESCAPE:
		return false;
	}

	START_LOG( cerr, L"npc유닛이 죽은 상태가 이상합니다." )
		<< BUILD_LOGc( cState )
		<< END_LOG;

	return false;
}

void KBattleFieldRoom::CheckIntrudeUserLoadComplete()
{
	// 일정 간격으로 몬스터 동기화 유저 체크
	if( m_tTimer[TE_INTRUDE_USER_LOAD].elapsed() < 5.0 )
		return;

	m_tTimer[TE_INTRUDE_USER_LOAD].restart();

	// 다른 유저들로부터의 로딩이 완료된 난입자 리스트를 얻자!
	std::vector< UidType > vecNpcLoadUserList;
	m_spUserManager->CheckAndGetIntrudeUnitLoadCompleteUserList( vecNpcLoadUserList );

	if( vecNpcLoadUserList.empty() == false )
	{
		// 해당 난입자에게 줄 몬스터 리스트를 뽑아내자!
		KEGS_BATTLE_FIELD_NPC_LOAD_NOT kPacket;
		kPacket.m_iBattleFieldID = GetBattleFieldID();
		m_kMonsterManager.GetAliveMonsterList( kPacket.m_vecNPCData, kPacket.m_mapAttribNpcInfo );

		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		KERM_UPDATE_QUEST_INFO_NOT kNot; 
		std::vector< int > vecNPC;
		BOOST_TEST_FOREACH( KNPCUnitReq&, kData, kPacket.m_vecNPCData )
		{
			kNot.m_vecNPC.push_back( kData.m_NPCID );
		}
#endif SERV_REFORM_QUEST
		//}}

		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecNpcLoadUserList )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"해당 캐릭터를 찾을 수 없습니다!" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_BATTLE_FIELD_NPC_LOAD_NOT, kPacket );

			//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			std::vector< std::vector<KNPCUnitReq> >	vecTempMiddleBossList;
			m_kMonsterManager.GetMiddleBossMonsterList( vecTempMiddleBossList );

			BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempMiddleBossList )
			{
				KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kPacketNot;

				BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
				{
					KNPCUnitNot kMiddleBoss;
					kMiddleBoss.m_kNPCUnitReq = kNpc;

					kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kMiddleBoss );
				}

				kPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kPacketNot );
			}		
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			std::vector< std::vector<KNPCUnitReq> >	vecTempEventBossList;
			m_kMonsterManager.GetEventBossMonsterList( vecTempEventBossList );

			BOOST_TEST_FOREACH( std::vector<KNPCUnitReq>&, vecList, vecTempEventBossList )
			{
				KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kPacketNot;

				BOOST_TEST_FOREACH( KNPCUnitReq&, kNpc, vecList )
				{
					KNPCUnitNot kEventBoss;
					kEventBoss.m_kNPCUnitReq = kNpc;

					kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.push_back( kEventBoss );
				}

				kPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kPacketNot );
			}		
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			if( kNot.m_vecNPC.empty() == true )
				continue;

			kNot.m_isDungeon = false;

			if( spRoomUser->CheckFindNPCQuest( GetBattleFieldID(), kNot.m_vecNPC, false ) == true )
			{
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_QUEST_INFO_NOT, kNot );
			}
#endif SERV_REFORM_QUEST
			//}}
		}
		
	}
}

void KBattleFieldRoom::CheckNpcP2PSyncUser( IN const bool bForce /*= false*/ )
{
	// 일정 간격으로 몬스터 동기화 유저 체크
	if( m_tTimer[TE_NPC_P2P_SYNC_USER_CHECK].elapsed() < 3.0  &&  bForce == false )
		return;

	m_tTimer[TE_NPC_P2P_SYNC_USER_CHECK].restart();

	KEGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT kNot;
	m_spUserManager->GetBattleFieldNpcSyncSubjectsList( kNot.m_vecNonNpcSyncUserList );
	if( kNot.m_vecNonNpcSyncUserList.empty() )
		return;

	KRoomUserPtr spRoomUser = m_spUserManager->GetHostUser();
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"방장을 찾는데에 실패하였습니다! 절대 일어나면 안되는 에러!" )
			<< BUILD_LOG( kNot.m_vecNonNpcSyncUserList.size() )
			<< END_LOG;
		return;
	}

	SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_BATTLE_FIELD_NPC_P2P_SYNC_NOT, kNot );	
}

void KBattleFieldRoom::CheckZombieUserEcho()
{
	if( m_tTimer[TE_ZOMBIE_USER_ECHO].elapsed() < GetKCnRoomManager()->GetBattleFieldZUEchoTime() )
		return;

	m_tTimer[TE_ZOMBIE_USER_ECHO].restart();

	for( int idx = 0; idx < m_spRoomUserManager->GetNumMember(); ++idx )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( idx );
		if( IS_NULL( spRoomUser ) )
			continue;

#ifdef  SERV_PVP_NEW_SYSTEM
		if( spRoomUser->IsPvpNpc() == true )
			continue;
#endif  SERV_PVP_NEW_SYSTEM

		KERM_BATTLE_FIELD_ZU_ECHO_REQ kPacket;
		kPacket.m_iUnitUID = spRoomUser->GetCID();
		//{{ 2012. 11. 20	최육사		배틀필드 좀비 유저 체크 강화
#ifdef SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		kPacket.m_iRoomUID = GetUID();
#endif SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		//}}

		SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_BATTLE_FIELD_ZU_ECHO_REQ, kPacket );
	}
}

void KBattleFieldRoom::GetBattleFieldRoomInfo( OUT KBattleFieldRoomInfo& kInfo )
{
	kInfo.m_iBattleFieldID = GetBattleFieldID();
	kInfo.m_RoomUID = GetUID();
	kInfo.m_MaxSlot = m_spUserManager->GetMaxSlot(); // 해당 배틀필드만의 최대 슬롯 갯수를 가져오자!
	m_spUserManager->GetAllPartyMemberListInfo( kInfo.m_mapUnitUIDPartyUID, kInfo.m_mapPartyList );
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-06	// 박세훈
	kInfo.m_byteBossFieldState = m_byteBossFieldState;
#endif // SERV_BATTLE_FIELD_BOSS
}

void KBattleFieldRoom::SendRoomListInfo( IN const int iCode )
{
	KERM_BATTLE_FIELD_ROOM_INFO_NOT kPacket;
	kPacket.m_cType = static_cast<char>(iCode);
	GetBattleFieldRoomInfo( kPacket.m_kBFRoomInfo );
	BroadCastAllGSServer( ERM_BATTLE_FIELD_ROOM_INFO_NOT, kPacket );
}

//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KBattleFieldRoom::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	KRoom::OnLeaveRoom( iLAST_SENDER_UID, iFIRST_SENDER_UID, kPacket_, usEventIDAck );

	//=== 게임중 이탈 결과처리 ===
	START_LOG( clog, L"=== BATTLE FIELD : 배틀필드는 이탈한다고 페널티를 주지 않습니다. ===" )
		<< BUILD_LOG( iFIRST_SENDER_UID )
		<< END_LOG;	

	KEGS_LEAVE_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
	
	// 배틀필드 입장을 위한 이탈이면 필드 입장 정보도 같이 넣어주자!
	if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_27 )
	{
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;		
	}
	else
	{
		kPacket.m_iReason = NetError::NET_OK;
	}
	
	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iFIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 방에서 이탈하기 전에 RoomUserInfo정보를 얻자
	KELOG_BATTLE_FIELD_LEAVE_LOG_NOT kPacketToLog;
	MakePacket_BattleFieldLeaveLog( iFIRST_SENDER_UID, kPacket_.m_iReason, kPacketToLog );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-28	// 박세훈
	_MakeBossFieldUserLogAtLeave( iFIRST_SENDER_UID );
#endif // SERV_BATTLE_FIELD_BOSS

	// 슬롯 정보 변경.
	if( m_spUserManager->LeaveRoom( iFIRST_SENDER_UID ) == false )
	{
		// 슬롯에서 나가기 실패.
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}
		return;
	}
	else
	{
		// 배틀필드에서 이탈하는게 성공했다면 임시 인벤토리도 날려버리자!
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_CLEAR_TEMP_INVENTORY_NOT );

		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}

		// 배틀필드 이탈 로그 남기기
		SendToLogDB( ELOG_BATTLE_FIELD_LEAVE_LOG_NOT, kPacketToLog );

		//{{ 2012. 11. 15	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
		{
			KERM_FIELD_USER_STATISTICS_NOT kPacketNot;
			kPacketNot.m_iGamePlayTime	= static_cast<int>( spRoomUser->GetPlayTime() );
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_FIELD_USER_STATISTICS_NOT, kPacketNot );
		}
#endif SERV_FIELD_PLAY_TIME_LOG
		//}}

		//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		LogToDB_GamePlayNetWorkInfo( iFIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo, true );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		//}}

		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

		//{{ 배틀 필드에서 몬스터 생성 오류 수정 - 김민성 -- 2012년 11월 15일 패치 예정
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
		std::vector< int > vecDeleteNpcList;
		m_kMonsterManager.GetNpcOwnerListByUnitUID( iFIRST_SENDER_UID, vecDeleteNpcList );

		BOOST_TEST_FOREACH( int, iNPCUID, vecDeleteNpcList )
		{
			LIF( m_kMonsterManager.SetMonsterDie( iNPCUID, 0 ) );

			KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
			kNpcDietNot.m_nDieNPCUID		 = iNPCUID;
			kNpcDietNot.m_bNoDrop			 = true;
			kNpcDietNot.m_cUserCount		 = 0;
			kNpcDietNot.m_EXPList.clear();
			
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
		//}
	}

	if( IsEmpty() == false )
	{
		// 방의 모든 유저들에게 슬롯 정보를 날린다.
		KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
		kPacketNot.m_iReason = usEventIDAck; // 전송 되어지는 ACK패킷타입을 넣자! 나중에 구분해서 처리할수 있도록
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iFIRST_SENDER_UID;
		kPacketNot.m_iPartyUID = spRoomUser->GetPartyUID();
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
	}

	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		START_LOG( cerr, L"배틀필드에서는 로딩 상태를 쓰지 않습니다!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
	}

	// 방에서 모든 유저가 나갔으면 일단 WAIT상태로 이동시킨다!
	if( IsEmpty() == true && IsEmpty( KBattleFieldUserManager::UT_OBSERVER ) == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KBattleFieldRoom::IncreasePartyFever( IN const std::map< UidType, float >& mapDamageByUser )
{
	std::set< UidType > setIncreasedPartyUID;

	std::map< UidType, float >::const_iterator mit;
	for( mit = mapDamageByUser.begin(); mit != mapDamageByUser.end(); ++mit )
	{
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
		if( IS_NULL( spRoomUser ) )
		{
			// 시간차로 방에서 이미 방을 이탈한 유저인 경우엔 정상적인 상황입니다. 따라서 에러로그레벨을 cwarn으로 낮춥니다.
			START_LOG( cwarn, L"해당 유저가 없습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return;
		}

		if( spRoomUser->GetPartyUID() == 0 )
			continue;

		if( setIncreasedPartyUID.find( spRoomUser->GetPartyUID() ) != setIncreasedPartyUID.end() )
			continue;

		// 같은 파티원이 필드에 없으면 패스
		//if( m_spUserManager->GetSamePartyMember( iKillUnitUID ) <= 1 )
		//	return;

		// 파티 피버 상승!
		KERM_INCREASE_PARTY_FEVER_NOT kNot;
		kNot.m_iPartyUID = spRoomUser->GetPartyUID();
		kNot.m_cRoomType = CXSLRoom::RT_BATTLE_FIELD;
		kNot.m_sSubStageNpcCount = m_kMonsterManager.GetAtStartedMonsterCount();
		SendToGSServer( spRoomUser->GetGSUID(), ERM_INCREASE_PARTY_FEVER_NOT, kNot );

		// 피버 수치 증가된 파티는 넣자!
		setIncreasedPartyUID.insert( spRoomUser->GetPartyUID() );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( ERM_OPEN_BATTLE_FIELD_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_BATTLE_FIELD_ACK, KERM_OPEN_BATTLE_FIELD_ACK );

	// 배틀필드 ID 검증
	if( SiCXSLBattleFieldManager()->IsExistBattleFieldID( static_cast<SEnum::BATTLE_FIELD_ID>(kPacket_.m_kRoomInfo.m_iBattleFieldID) ) == false )
	{
		START_LOG( cerr, L"유효하지 않은 배틀필드ID입니다!" )
			<< BUILD_LOG( kPacket_.m_kRoomInfo.m_iBattleFieldID )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_vecPartyMemberList.size() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_bMoveForMyParty )
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_bNowBattleFieldPositionInfoStartPosition )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_usBattleFieldPositionValue )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
			<< END_LOG;
	}

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
//#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_BATTLE_FIELD, true );
//#endif SERV_ROOM_COUNT
	//}}

    // 방 옵션 정보 세팅.
	SetRoomInfo( kPacket_.m_kRoomInfo );

    // 방 생성이므로 슬롯 정보를 초기화 한다.
    m_spUserManager->Reset();
    m_spUserManager->AssignTeam( -1 );

    // 슬롯에 생성 요청자를 넣는다.
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	InitBossFieldCreateInfo( kPacket_.m_KBossFieldCreateInfo );

	if( EnterRoom( kPacket_.m_kRoomUserInfo
				 , kPacket_.m_kBattleFieldJoinInfo
				 , false
				 ) == false )
#else // SERV_BATTLE_FIELD_BOSS
    if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo
								  , kPacket_.m_kBattleFieldJoinInfo
								  , false
								  ) == false )
#endif // SERV_BATTLE_FIELD_BOSS
	{
        START_LOG( cerr, L"방 입장 실패. 배틀필드를 생성하는것인데 여기서 실패가 났다! 절대 일어나서는 안되는 문제!" )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
            << END_LOG;

        kPacket.m_iOK = NetError::GetLastError();
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_BATTLE_FIELD_ACK, kPacket );
        return;
    }

	//080409.hoons. 제자 리스트를 셋팅한다.
	m_spUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );

    // 방 상태를 wait 로 바꾼다.
    StateTransition( KRoomFSM::I_TO_WAIT );
	m_tTimer[TE_WAIT_TIMER].restart();
    m_kTimer[TM_WAIT].restart();

	//모든유저상태를 레디로 바꾼다.
	m_spUserManager->SetAllReady( true );

	//gm 이상이면 센터서버의 ip를 넣어주자.
	if( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

	// 게임 초기화.
	StartGame();

	// 기존의 방 로딩 과정 없이 배틀필드에서는 바로 게임을 시작한다!
	StartPlay();

    // 방 생성 요청자에게 ack를 날린다.
    kPacket.m_iOK = NetError::NET_OK;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_StartPosIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	GetRoomInfo( kPacket.m_RoomInfo );
	GetRoomSlotInfo( kPacket.m_vecSlot );
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	kPacket.m_bBossField = SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD );
	if( kPacket.m_bBossField == true )
	{
		kPacket.m_tRemainFieldHoldingTime	= m_tFieldHoldingTime;
		kPacket.m_kBossFieldJoinInfo		= kPacket_.m_kBossFieldJoinInfo;
	}
#endif // SERV_BATTLE_FIELD_BOSS
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_BATTLE_FIELD_ACK, kPacket );

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	// 배틀 필드 입장 시 UDP 검증 요청 보내자
	if( ( kPacket.m_iOK == NetError::NET_OK ) &&
		( SiKGameSysVal()->GetLanBugOutDisconnectCheckStartOnGameLoadingReq() == false ) &&
		( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	//방생성후 좀비유저 체크시작.
	m_spUserManager->ZU_CheckStart();
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckStart();
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	// 방 정보 전송
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [주의!!!] 반드시 방리스트를 먼저 보낸 이후에 이 패킷을 보내야 한다! 유저의 복귀를 위해 패킷을 다시 쏜다!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_JOIN_BATTLE_FIELD_REQ )
{
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
	KERM_JOIN_BATTLE_FIELD_ACK kPacket;
#else // SERV_BATTLE_FIELD_BOSS
	KEGS_JOIN_BATTLE_FIELD_ACK kPacket;
#endif // SERV_BATTLE_FIELD_BOSS

	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;

	if( GetStateID() != KRoomFSM::S_PLAY  &&  GetStateID() != KRoomFSM::S_WAIT )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// 제대로 찾아 왔는지 보자!
	if( GetBattleFieldID() != kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
	{
		START_LOG( cerr, L"다른 배틀필드에 입장했다! 헐 버그다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-07	// 박세훈
	if( SEnum::CheckFlag( m_byteBossFieldState, ( SEnum::BFRS_BOSS_FIELD | SEnum::BFRS_INTRUDE_RESTRICTION ) ) == true )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_55;	// 보스 필드 입장에 실패하였습니다. 다시 시도 해주세요.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}
#endif // SERV_BATTLE_FIELD_BOSS

	// 배틀필드에 입장 처리 한다!
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-28	// 박세훈
	if( EnterRoom( kPacket_.m_kRoomUserInfo
				 , kPacket_.m_kBattleFieldJoinInfo
				 , false
				 ) == false )
#else // SERV_BATTLE_FIELD_BOSS
	if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo
								  , kPacket_.m_kBattleFieldJoinInfo
								  , false
								  ) == false )
#endif // SERV_BATTLE_FIELD_BOSS
	{
		START_LOG( cerr, L"방 입장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->SetReady( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->StartGame( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// 작업날짜: 2013-06-10	// 박세훈
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_BUFF_INFO_NOT );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

	// 해당 유저는 바로 게임 시작 처리 하자!
	if( m_spUserManager->StartPlay( kPacket_.m_kRoomUserInfo.m_nUnitUID ) == false )
	{
		START_LOG( cerr, L"게임 시작 처리 실패!" )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// 방 상태 변화.
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
        StateTransition( KRoomFSM::I_TO_LOAD );
		StateTransition( KRoomFSM::I_TO_PLAY );
	}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckStart( kPacket_.m_kRoomUserInfo.m_nUnitUID );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	// 입장한 유저에게 ack를 날린다.
	kPacket.m_iOK = NetError::NET_OK;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_iLastTouchIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	GetRoomInfo( kPacket.m_RoomInfo );
	GetRoomSlotInfo( kPacket.m_vecSlot );
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2012. 11. 21	최육사		난입자에게 캐쉬샵 유저 정보 보내기
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	m_spUserManager->GetEnterCashShopUserList( kPacket.m_vecEnterCashShopUser );
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	//}}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	kPacket.m_bBossField = SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD );

	if( kPacket.m_bBossField == true )
	{
		const __time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

		if( tCurrentTime < m_tFieldCloseTime )
		{
			kPacket.m_tRemainFieldHoldingTime = m_tFieldCloseTime - tCurrentTime;
		}

		kPacket.m_kBossFieldJoinInfo = kPacket_.m_kBossFieldJoinInfo;
	}
#endif // SERV_BATTLE_FIELD_BOSS

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_BATTLE_FIELD_ACK, kPacket );

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	// 배틀 필드 입장 시 UDP 검증 요청 보내자
	if( ( kPacket.m_iOK == NetError::NET_OK ) &&
		( SiKGameSysVal()->GetLanBugOutDisconnectCheckStartOnGameLoadingReq() == false ) &&
		( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	// 방의 모든 유저에게 NOT를 날린다.
	KEGS_JOIN_BATTLE_FIELD_NOT kPacketNot;
	//080425.hoons.옵저버 상태별 유저정보를 셋팅해서 보내준다.
	m_spUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KBattleFieldUserManager::UT_GAME );
	kPacketNot.m_StartPosIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
	BroadCast( ERM_JOIN_BATTLE_FIELD_NOT, kPacketNot );

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [주의!!!] 반드시 방리스트를 먼저 보낸 이후에 이 패킷을 보내야 한다! 유저의 복귀를 위해 패킷을 다시 쏜다!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT, KEGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"처리 가능한 상태가 아닙니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}
	
	if( m_spUserManager->UnregIntrudeUnitLoadUserList( kPacket_.m_iLoadedUnitUID, FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"난입자 로딩 완료 정보 등록을 실패하였습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iLoadedUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}	
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ), ERM_BATTLE_FIELD_GAME_LOADING_ACK, KPacketOK );

	// loading 상태를 변화시킨다.
	if( m_spRoomUserManager->SetLoadingProgress( FIRST_SENDER_UID, kPacket_.m_iLoadingProgress ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_13;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_GAME_LOADING_ACK, kPacket );
		return;
	}

	// 보낸 사람에게 ack를 날린다.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_GAME_LOADING_ACK, kPacket );

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-06	// 박세훈
	// 배틀 필드 입장 시 UDP 검증 요청 보내자
	if( ( kPacket.m_iOK == NetError::NET_OK ) &&
		( SiKGameSysVal()->GetLanBugOutDisconnectCheckStartOnGameLoadingReq() == true ) &&
		( SiKGameSysVal()->GetLanBugOutVerify() == true ) &&
		( IsLanBugVerifyUser( FIRST_SENDER_UID ) == false ) &&
		( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
		)
	{
		KInteriorUdpSession::GetKObj()->DataPushBack( CKTDNUDP::LEI_VERIFY_NOT, GetUID(), FIRST_SENDER_UID );
		AddLanBugVerifyUser( FIRST_SENDER_UID );
	}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

	// 방의 모든 유저들에게 not를 날린다.
	//KEGS_GAME_LOADING_NOT kPacketBR;
	//kPacketBR.m_iUnitUID = FIRST_SENDER_UID;
	//kPacketBR.m_iLoadingProgress = kPacket_.m_iLoadingProgress;
	//BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );


    if ( kPacket_.m_iLoadingProgress >= 100 )
    {
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
        KEGS_GAME_LOADING_NOT kPacketBR;
        kPacketBR.m_iUnitUID = FIRST_SENDER_UID;
        kPacketBR.m_iLoadingProgress = 100;
        BroadCast( ERM_GAME_LOADING_NOT, kPacketBR );
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
        if ( KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID ) )
        {
            spRoomUser->ZombieAlert_Set( false );
            spRoomUser->ZombieAlert_Refresh();
        }
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

    }



}

IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_ACK );
}

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_NPC_UNIT_CREATE_ACK, KPacketOK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_CREATE_ACK, kPacket );

	KEGS_NPC_UNIT_CREATE_NOT kPacketNot;
	kPacketNot.m_iBattleFieldID = GetBattleFieldID();

	BOOST_TEST_FOREACH( KNPCUnitReq&, kCreateNpcInfo, kPacket_.m_vecNPCUnitReq )
	{
#ifdef NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC
		/// 적군 몬스터 이거나
		/// 아군 중 아래의 리스트 NPC 
		/// 아군 중 PVP_NPC_KEY_CODE 가 지정된 PC만 부를 수 있음
		if ( !kCreateNpcInfo.m_bAllyNpc ||
			CXSLUnitManager::CanCreateThisNpc( kCreateNpcInfo.m_NPCID ) ||
			kCreateNpcInfo.m_KeyCode == PVP_NPC_KEY_CODE )
#endif // NOT_CREATE_NPC_EXCEPT_FOR_SPECIFIC_NPC
		{
			KNPCUnitNot kNPCInfo;

			switch( kCreateNpcInfo.m_NPCID )
			{
			case CXSLUnitManager::NUI_EVENT_TEACHER_ECHO:
				{
					kCreateNpcInfo.m_bNoDrop = false; // ED 대량 드롭 
					kCreateNpcInfo.m_Level = SiCXSLBattleFieldManager()->GetStandardMonsterLevel( GetBattleFieldID() );

				}break;
			default:
				{
					kCreateNpcInfo.m_bNoDrop = true; // 클라이언트가 생성 요청 보내는 몬스터는 보안상 반드시 NoDrop으로 처리한다!
				}break;	
			}

			kNPCInfo.m_kNPCUnitReq = kCreateNpcInfo;

			// 몬스터 생성!
			LIF( m_kMonsterManager.CreateMonster( kCreateNpcInfo, kNPCInfo.m_kNPCUnitReq.m_UID ) );
			kPacketNot.m_vecNPCUnitAck.push_back( kNPCInfo );

			//{{ 배틀 필드에서 몬스터 생성 오류 수정 - 김민성 -- 2012년 11월 15일 패치 예정
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			// 몬스터 주인 등록 해 놓기
			// 주인이 나가게 되면 소환된 몬스터도 지워 주기 위해서
			m_kMonsterManager.SetNpcOwner( kNPCInfo.m_kNPCUnitReq.m_UID, kCreateNpcInfo.m_iAllyUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
			//}
		}
	}

	BroadCast( ERM_NPC_UNIT_CREATE_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// 적용날짜: 2013-05-03
	if( HostCheck() == true )
	{
		KRoomUserPtr spUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
		if( spUser == NULL )
		{
			START_LOG( cwarn, L"패킷을 보낸 유저가 존재하지 않는다?" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}

		if( spUser->IsHost() == false )
		{
			START_LOG( cwarn, L"호스트 유저가 아닌데도 ERM_NPC_UNIT_DIE_REQ 패킷을 날려왔다." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			//// 추방 로그 기록
			//KDBE_UDP_CHECK_KICK_USER_NOT kPacket;
			//kPacket.m_iRoomUID = GetUID();
			//kPacket.m_iPVPChannelClass	= KPVPChannelInfo::PCC_END;
			//kPacket.m_iMatchType		= SEnum::MT_MAX;
			//kPacket.m_iUnitUID			= FIRST_SENDER_UID;
			//SendToLogDB( DBE_UDP_CHECK_KICK_USER_NOT, kPacket );
			return;
		}
	}
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-13	// 박세훈
	// 보스 필드에서는 위험도 변화가 없다.
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
	{
		kPacket.m_iOK = BossMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket );
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
	}
	else
	{
		char cMonsterGrade = 0;

		if( m_kMonsterManager.IsEventMonster( kPacket_.m_nDieNPCUID ) == true )
		{
			kPacket.m_iOK = EventMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}
		else if( m_kMonsterManager.IsMiddleBossMonster( kPacket_.m_nDieNPCUID ) == true )
		{
			kPacket.m_iOK = MiddleBossMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		else if( m_kMonsterManager.IsEventBossMonster( kPacket_.m_nDieNPCUID ) == true )
		{
			kPacket.m_iOK = EventBossMonsterDieProcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
		else
		{
			kPacket.m_iOK = MonsterDiePrcess( FIRST_SENDER_UID, kPacket_, kPacket, cMonsterGrade );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		}

		if( kPacket.m_iOK == NetError::NET_OK )
		{
			//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
			// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
			OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss(), LAST_SENDER_UID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
			//}
		}
	}
#else // SERV_BATTLE_FIELD_BOSS
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	bool bIsEventBossMonster = false;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	bool bIsMiddleBossMonster = false;
#else
	// 속성 몬스터인지 체크
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	//{{ 2013. 1. 1	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
	if( m_kMonsterManager.IsEventMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		// 몬스터가 현재 살아있는지 체크!
		if( m_kMonsterManager.IsEventMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 몬스터가 아직 살아있다면 몬스터 정보 요청!
		if( m_kMonsterManager.GetEventNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 아직 살아있다면 죽인사람과 죽은것으로 설정..
		LIF( m_kMonsterManager.SetEventMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );
		BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID );
		return;
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
	}
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	else if( m_kMonsterManager.IsMiddleBossMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		bIsMiddleBossMonster = true;

		// 몬스터가 현재 살아있는지 체크!
		if( m_kMonsterManager.IsMiddleBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 몬스터가 아직 살아있다면 몬스터 정보 요청!
		if( m_kMonsterManager.GetMiddleBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 아직 살아있다면 죽인사람과 죽은것으로 설정..
		LIF( m_kMonsterManager.SetMiddleBossMonsterDie( kPacket_.m_nDieNPCUID ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );

        std::vector< KDropItemData > vecDropItem;
        vecDropItem.clear();
		BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID, vecDropItem );

        KFieldBossGenKillStat kLog;
        //kLog.m_nDieNPCUID = kPacket_.m_nDieNPCUID;
        kLog.m_nNumUser = kPacket_.m_mapDamageByUser.size();
        //kLog.m_nNumDropItem = vecDropItem.size();
        kLog.m_nSpawnID = kDieNpcInfo.m_iBossGroupID;
        kLog.m_nFieldID = GetBattleFieldID();
        SendToLogDB( ELOG_KILL_FIELD_MIDDLE_BOSS_STAT_NOT, kLog );

        START_LOG( cwarn, L"필드 중간 보스 킬 통계 테스트 " )
            << BUILD_LOG( kLog.m_nFieldID )
            //<< BUILD_LOG( kLog.m_nDieNPCUID )
            << BUILD_LOG( kLog.m_nNumUser )
            << BUILD_LOG( kLog.m_nSpawnID )
            << END_LOG;

		return;
	}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	else if( m_kMonsterManager.IsEventBossMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		bIsEventBossMonster = true;

		// 몬스터가 현재 살아있는지 체크!
		if( m_kMonsterManager.IsEventBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 몬스터가 아직 살아있다면 몬스터 정보 요청!
		if( m_kMonsterManager.GetEventBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 아직 살아있다면 죽인사람과 죽은것으로 설정..
		LIF( m_kMonsterManager.SetEventBossMonsterDie( kPacket_.m_nDieNPCUID ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );

		BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID );
		return;
	}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	else
	{
		// 몬스터가 현재 살아있는지 체크!
		if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_21;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 몬스터가 아직 살아있다면 몬스터 정보 요청!
		if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
		{
			kPacket.m_iOK = NetError::ERR_ROOM_20;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			return;
		}

		// 아직 살아있다면 죽인사람과 죽은것으로 설정..
		LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade, !m_kMonsterManager.IsRemainMiddleBoss() );
		BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, FIRST_SENDER_UID );
		return;
#else
		// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
		m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
	}
#else
	// 몬스터가 현재 살아있는지 체크!
	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_21;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	// 아직 살아있다면 죽인사람과 죽은것으로 설정..
	LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	kPacket.m_iOK		= NetError::NET_OK;
	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

	// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
	m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade );
#endif SERV_FIELD_EVENT_MONSTER
	//}}
#endif // SERV_BATTLE_FIELD_BOSS

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	// 함수로 빼놓음 아래 삭제 해야함
#else
	//{{ 2009. 5. 11  최육사	npc죽은상태
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
	//}}

	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet 읽기 오류.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// 몬스터 타입 상수 얻기
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// 필드 상수 얻기
	//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit값이 -1이면 유저의 공격을 받지 않고 npc가 자살한것임. [예:지뢰함정]

	// 플레이어가 몬스터를 죽인거라면 해당 플레이어의 킬몬스터수 체크
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"몹끼리 죽였네!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// 몬스터가 함정에 걸려 죽는경우임 : 아이템은 그대로 드롭처리함.
			}
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	// 1. 몬스터 종류에 따라 드롭 아이템이 결정되는 몬스터 드롭
	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// 각 유저마다 다른 내용의 패킷을 보내야 하므로 이렇게 처리한다.
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
		kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
		kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
		kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
		kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
		kNpcDietNot.m_cUserCount		 = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete	 = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능

		// 분할 지급을 위해 선 계산 되어야 할 내용
		// 1. 기여도 (X로 표기)
		std::map< UidType, float >	mapUserContribution;
		int iNpcMaxHP = kPacket_.m_iNpcMaxHP;
		std::map< UidType, float >::iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin();
		for( ; mitDamageByUser != kPacket_.m_mapDamageByUser.end() ; ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;
			float fContribution = m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) / (static_cast<float>(iNpcMaxHP));

			if( fContribution > 1.5f )
				fContribution = 1.5f;

			// 솔로 플레이면 파티가 없으니 목록이 없을 수도 있다. 그래서 한번 해줘야 한다.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// 파티 원도 동일한 기여도로 셋팅한다.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}

		// 2. 몬스터의 드롭 정보 얻기
		KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
		if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
		{
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();

			START_LOG( clog, L"배틀 필드 npc drop info 얻어오기 실패" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;

			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		// 3. 기여도가 있는 유저별로 EXP, ED, DropItem 계산
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"보내려는  리스트를 찾을 수가 없다." )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// 필드 전원에게 해당 npc가 죽은것에 대한 정보를 브로드 캐스팅 한다! ( 일부유저 드롭, 경험치 제외 )
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
		for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
		{
			KDropTable::KBattleFieldNpcDropInfo kDropInfo;
			kDropInfo = kDropInfo_Init;

			if( mitUserList->second == NULL )
				continue;

			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			if( mitUserList->second->IsBattleFieldNpcLoad() == false )
				continue;

			// 해당 유저가 몬스터를 죽일때 얼만큼 공헌을 했는지 검사하자!
			std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
			if( mitUserContribution == mapUserContribution.end() )
			{
				// 공헌도가 없으면 보상 받을 자격이 없다!
				SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
				continue;
			}

			KDropTable::DROP_DATA sDropDataNpc;

			// 현재 같은 필드에 있는 파티원 수
			kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

			// 레벨 상수값 얻기
			const float fLevelFactor = CXSLDungeon::GetMonsterLevelBalanceRate( m_spUserManager->GetUserLevel( mitUserList->first ), kDieNpcInfo.m_cLevel );

			// 파티 플레이일 경우 아이템 확률 증가
			const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

			// 파티의 기여도가 50% 이상인가?
			if( mitUserContribution->second >= 0.5f )
			{
				kNpcDietNot.m_bQuestComplete = true;
			}

			// 개별적으로 계산/지급 되어야 하는 품목
			// 1. ED
			// 2. EXP
			// 3. 아이템
			if( kDropInfo.m_iExp > 0 )
			{
				kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ) / kNpcDietNot.m_cUserCount);

				// 경험치를 주는 몬스터라면 내구도가 감소 된다.
				kNpcDietNot.m_bDecreaseEndurance = true;
			}

			if( kDropInfo.m_iED > 0 )
			{
				kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel )) / kNpcDietNot.m_cUserCount;
			}

			//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
			//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo, fFieldEDFactor, fFieldEXPFactor, fLevelFactor, mitUserContribution->second, fPartyDropBonus, m_iItemDropEventProbCount, kDieNpcInfo.m_bIsBoss, sDropDataNpc ) == false )
#else
			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo, fFieldFactor, fLevelFactor, mitUserContribution->second, fPartyDropBonus, m_iItemDropEventProbCount, kDieNpcInfo.m_bIsBoss, sDropDataNpc ) == false )
#endif SERV_FIELD_ED_EXP_FACTOR
			//}
#else
			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo, fFieldFactor, fLevelFactor, mitUserContribution->second, fPartyDropBonus, kDieNpcInfo.m_bIsBoss, sDropDataNpc ) == false )
#endif SERV_ITEM_DROP_EVENT
				//}}
			{
				START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< END_LOG;
			}

			// 공용으로 계산/지급 되어야 하는 품목
			// 1. 특수 아이템 드롭(ex. 마나히터)
			// 경험치를 주는 경우엔 내구도 소모를 시킨다.
			SiKDropTable()->NpcSpecialDropItem( kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cMonsterGrade, kPacket_.m_cNpcDieState, sDropDataNpc.m_vecItemID );

			//////////////////////////////////////////////////////////////////////////		
			// 필드의 유저에게 경험치 지급(보너스 포함)
			m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

			// 한번만 올려주어야 한다.
			if( mitUserList->second->GetCID() == FIRST_SENDER_UID )
			{
				IncreasePartyFever( kPacket_.m_mapDamageByUser ); // 해당 몬스터를 잡은 유저들의 공헌도에 따른 파티 피버 증가!
			}
			//////////////////////////////////////////////////////////////////////////	
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

			// 드롭을 하지 않는 npc라면 드롭을 하지 않는다.
			if( kDieNpcInfo.m_bNoDrop == true )
				continue;

			// 드롭상태가 아닌채로 죽은npc라면 아이템 드롭을 하지 않는다.
			if( bDropDieState == false )
				continue;

			// 개인 기여도 * 파티 보너스
			//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
			float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
			float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
			//}

			//////////////////////////////////////////////////////////////////////////
			// 2. 던전을 기준으로 드롭 아이템이 결정되는 던전 드롭
			KDropTable::DROP_DATA sDropDataStatic;	
			{
				if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
				{
					START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
						<< BUILD_LOG( GetBattleFieldID() )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 4. 속성 몬스터 드롭 아이템
			KDropTable::DROP_DATA sDropDataAttribNpc;

			if( bIsAttribNpc == true )
			{
				//{{ 2012. 12. 16  드롭 이벤트 - 김민성
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"속성 몬스터 아이템 드롭 실패!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}

				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;


				if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"속성 몬스터 스페셜 아이템 드롭 실패!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 5. 퀘스트 드롭 아이템 처리
			{
				std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

				//유닛별 퀘스트 드롭 아이템이 있는지 검사..
				//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
				{
					//있다면 유닛별로 루푸를 돌면서..
					BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
					{
						//드롭된 아이템 수만큼 처리를 시작..
						BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
						{
							KEGS_GET_ITEM_NOT kDropNot;
							kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
							kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
							kDropNot.m_iItemID		= iDropQuestItemID;
							//{{ 2009. 7. 9  최육사		임시 인벤토리
							kDropNot.m_iDropItemUID = m_nDropItemUID++;
							//}}
							SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
						}
					}
				}
				else
				{
					if( NetError::GetLastError() != NetError::NET_OK )
					{
						START_LOG( cerr, L"퀘스트 드롭 아이템 받아오기 실패.!" )
							<< BUILD_LOG( kPacket_.m_uiAttUnit )
							<< BUILD_LOG( m_iDungeonID )
							<< BUILD_LOG( NetError::GetLastErrMsg() )
							<< END_LOG;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 위에서 결정된 드롭 아이템 리스트를 실제 유저에게 보내기 위한 처리
			KEGS_DROP_ITEM_NOT kDropItemNot;
			kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

			if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
			{
				//{{ 2012. 11. 20	최육사		휴면 복귀 유저 버그 수정 ( 배틀필드 ) 
#ifdef SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
#else
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetNumMember() ) == true )
#endif SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
					//}}
				{
					// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// 새로운 모험의 징표
				}	
			}

			//1.ED 드롭처리
			if( sDropDataNpc.m_iED != -1 )
			{
				for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
				}
			}

			//2.NPC ITEM 드롭처리	
			if( sDropDataNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//{{ 2009. 4. 22  최육사	전체드롭
			if( sDropDataStatic.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}
			//}}

			//4.ATTRIB NPC ITEM 드롭처리
			if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//드롭처리한 아이템이 있으면 패킷을 보낸다.
			if( kDropItemNot.m_DropItemDataList.empty() == false )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
			}
		}

		//{{ 2012. 12. 26  특정 시간 몬스터 사냥 시 아이템 드롭 이벤트(필드적용) - 김민성
#ifdef SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//////////////////////////////////////////////////////////////////////////
		// 특정 시각 아이템 드롭 이벤트
		if( kDieNpcInfo.m_bActive == true )
		{
			//////////////////////////////////////////////////////////////////////////
			// 특정 시간에 NPC때려잡으면 보상주는 기능
			if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
				if( spRoomUser != NULL )
				{
					int iTimeDropItemID = 0;
					int iTimeDropID = 0;
					if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
					{
						const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
						if( pItemTemplet != NULL )
						{
							// 일단 DB로 가서 이 유저가 보상 받아도 되는 유저인지 확인하자!
							KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
							kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
							kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
							kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
							kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
							kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
							kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
							kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
							kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
							SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
						}
						else
						{
							START_LOG( cerr, L"존재하지 않는 itemid이다. 세팅오류인가?" )
								<< BUILD_LOG( iTimeDropItemID )
								<< END_LOG;
						}
					}
				}
				else
				{
					START_LOG( cwarn, L"이벤트 아이템 획득 처리 하려는데 유저정보가 없다!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
#endif SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//}}
	}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
	
}

#else		// SERV_REFORM_ITEM_DROP

//_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
//{
//	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );
//
//	// 몬스터가 현재 살아있는지 체크!
//	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_21;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//		return;
//	}
//
//	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
//	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
//	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_20;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//		return;
//	}
//
//	// 아직 살아있다면 죽인사람과 죽은것으로 설정..
//	LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );
//
//	kPacket.m_iOK		= NetError::NET_OK;
//	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//
//	// 게임 매니저에도 몬스터가 죽었다는것을 알리자.
//	m_kGameManager.OnNpcUnitDie( m_spUserManager->GetNumMember(), false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade );
//
//	//방안의 인원에게 알려줄 데이터 셋팅 시작..
//	KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
//	kNpcDietNot.m_iDungeonID	= m_iDungeonID;
//	kNpcDietNot.m_cDifficulty	= m_cDifficultyLevel;
//
//	kNpcDietNot.m_nDieNPCUID	= kPacket_.m_nDieNPCUID;
//	kNpcDietNot.m_iNPCID		= kDieNpcInfo.m_iNPCID;
//	kNpcDietNot.m_cAttUnitType	= kPacket_.m_cAttUnitType;
//	kNpcDietNot.m_uiAttUnit		= kPacket_.m_uiAttUnit;
//
//	kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( kPacket_.m_uiAttUnit ) );
//	kNpcDietNot.m_bNoDrop		= kDieNpcInfo.m_bNoDrop;
//
//	//{{ 2009. 12. 31  최육사	내구도소모개편	
//	kNpcDietNot.m_bDecreaseEndurance = false;
//	//}}
//
//	//{{ 2011. 08. 31  김민성	비밀던전 헬모드
//#ifdef SERV_RANDOM_DAY_QUEST
//	kNpcDietNot.m_cDungeonMode	= m_cDungeonMode;
//#endif SERV_RANDOM_DAY_QUEST
//	//}}
//
//	// 현재 배틀필드의 초기 몬스터 수를 얻어옵니다.
//	kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
//
//	//{{ 2009. 5. 11  최육사	npc죽은상태
//	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
//	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
//	//}}
//
//	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
//	if( pNPCTemplet == NULL )
//	{
//		START_LOG( cwarn, L"NPC Unit Templet 읽기 오류.!" )
//			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//			<< END_LOG;
//		return;
//	}
//
//	// 파티 플레이일 경우 아이템 드롭 카운트 증가
//	const int iDropItemNum = 1; // 배틀필드는 드롭 카운트 무조건 1로?
//
//	// 몬스터 타입 상수 얻기
//	const char cMonsterTypeFactor = static_cast<char>( CXSLUnitManager::GetMonsterTypeFactor( false, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade ) );	
//
//	// 속성 몬스터인지 체크
//	const bool bIsAttribNpc = false;
//
//	//////////////////////////////////////////////////////////////////////////
//	// uiAttUnit값이 -1이면 유저의 공격을 받지 않고 npc가 자살한것임. [예:지뢰함정]
//
//	// 플레이어가 몬스터를 죽인거라면 해당 플레이어의 킬몬스터수 체크
//	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
//	{
//		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
//		{
//			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
//			{
//				START_LOG( clog, L"몹끼리 죽였네!!" )
//					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//					<< BUILD_LOG( kPacket_.m_uiAttUnit );
//
//				// 몬스터가 함정에 걸려 죽는경우임 : 아이템은 그대로 드롭처리함.
//			}
//		}
//	}	
//
//	//////////////////////////////////////////////////////////////////////////
//	// 1. 몬스터 종류에 따라 드롭 아이템이 결정되는 몬스터 드롭
//	KDropTable::DROP_DATA sDropDataNpc;
//
//	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
//	{
//		if( SiKDropTable()->BattleFieldNpcDropItem( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cLevel, cMonsterTypeFactor, sDropDataNpc, iDropItemNum, kDieNpcInfo.m_bIsBoss ) == false )
//		{
//			START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
//				<< BUILD_LOG( GetBattleFieldID() )
//				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//				<< END_LOG;
//		}
//
//		// 경험치를 주는 경우엔 내구도 소모를 시킨다.
//		if( sDropDataNpc.m_iExp > 0 )
//		{
//			kNpcDietNot.m_bDecreaseEndurance = true;
//		}			
//
//		//{{ 2009. 5. 18  최육사	마나이터
//		switch( kDieNpcInfo.m_iNPCID )
//		{
//		case CXSLUnitManager::NUI_MANA_EATER:
//		case CXSLUnitManager::NUI_HALLOWEEN_MANA_EATER:
//			//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
//		case CXSLUnitManager::NUI_DEFENCE_MANA_EATER:
//#endif SERV_INSERT_GLOBAL_SERVER
//			//}}
//			//{{ 2012. 06. 25	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//		case CXSLUnitManager::NUI_MANA_EATER_FD:
//#endif SERV_BATTLE_FIELD_SYSTEM
//			//}}
//			{
//				if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
//				{
//					// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//					sDropDataNpc.m_vecItemID.push_back( 70055 ); // 70055
//				}
//			}
//			break;
//
//		case CXSLUnitManager::NUI_BLOOD_EATER:
//			//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
//		case CXSLUnitManager::NUI_DEFENCE_BLOOD_EATER:
//#endif SERV_INSERT_GLOBAL_SERVER
//			//}}
//			//{{ 2012. 06. 25	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//		case CXSLUnitManager::NUI_BLOOD_EATER_FD:
//#endif SERV_BATTLE_FIELD_SYSTEM
//			//}}
//			{
//				if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
//				{
//					// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//					sDropDataNpc.m_vecItemID.push_back( 70056 ); // 70055
//				}
//			}
//			break;
//		}
//		//}}
//
//		//////////////////////////////////////////////////////////////////////////		
//		//{{ 2009. 12. 18  최육사	던전경험치개편
//#ifdef DUNGEON_EXP_NEW
//		if( sDropDataNpc.m_iExp > 0 )
//		{
//			// 필드의 각 유저들에게 경험치 배분
//			m_spUserManager->AddBattleFieldRewardEXP( kDieNpcInfo.m_cLevel, 
//				cMonsterTypeFactor,
//				kPacket_.m_iNpcMaxHP, 
//				kPacket_.m_mapDamageByUser, 
//				GetDungeonIDAndDif(), 
//				GetDifficultyLevel(), 
//				kNpcDietNot );
//
//			//{{ 2012. 02. 28	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//			// 해당 몬스터를 잡은 유저들의 공헌도에 따른 파티 피버 증가!
//			IncreasePartyFever( kPacket_.m_mapDamageByUser );
//#endif SERV_BATTLE_FIELD_SYSTEM
//			//}}
//		}
//#endif DUNGEON_EXP_NEW
//		//}}
//		//////////////////////////////////////////////////////////////////////////		
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 던전 파티원들에게 해당 npc가 죽은것에 대한 정보를 브로드 캐스팅 한다! ( 경험치 정보 포함 )
//	BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
//
//
//	// 드롭을 하지 않는 npc라면 드롭을 하지 않는다.
//	if( kDieNpcInfo.m_bNoDrop == true )
//		return;
//
//	// 드롭상태가 아닌채로 죽은npc라면 아이템 드롭을 하지 않는다.
//	if( bDropDieState == false )
//		return;
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 2. 던전을 기준으로 드롭 아이템이 결정되는 던전 드롭
//	KDropTable::DROP_DATA sDropDataStatic;	
//	{
//		if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, iDropItemNum ) == false )
//		{
//			START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
//				<< BUILD_LOG( GetBattleFieldID() )
//				<< END_LOG;
//		}
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////	
//	// 3. 이벤트 아이템 드롭 체크
//	//KDropTable::DROP_DATA sDropDataEvent;
//
//	//if( kDieNpcInfo.m_bActive == true )
//	//{
//	//	if( SiKDropTable()->EventDropItem( iDropItemNum, sDropDataEvent ) == false )
//	//	{
//	//		START_LOG( cerr, L"Event Drop Item Data 얻어오기 실패 ===" )
//	//			<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//	//			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//	//			<< BUILD_LOGc( m_cDifficultyLevel )
//	//			<< END_LOG;
//	//	}
//
//	//	//////////////////////////////////////////////////////////////////////////
//	//	// 특정 시간에 NPC때려잡으면 보상주는 기능
//	//	if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
//	//	{
//	//		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
//	//		if( spRoomUser != NULL )
//	//		{
//	//			// 체험ID가 아닌 유저만 보상처리
//	//			if( spRoomUser->IsGuestUser() == false )
//	//			{
//	//				int iTimeDropItemID = 0;
//	//				int iTimeDropID = 0;
//	//				if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
//	//				{
//	//					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
//	//					if( pItemTemplet != NULL )
//	//					{
//	//						// 일단 DB로 가서 이 유저가 보상 받아도 되는 유저인지 확인하자!
//	//						KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
//	//						kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
//	//						kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
//	//						kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
//	//						kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
//	//						kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
//	//						kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
//	//						kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
//	//						kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
//	//						SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
//	//					}
//	//					else
//	//					{
//	//						START_LOG( cerr, L"존재하지 않는 itemid이다. 세팅오류인가?" )
//	//							<< BUILD_LOG( iTimeDropItemID )
//	//							<< END_LOG;
//	//					}
//	//				}
//	//			}
//	//		}
//	//		else
//	//		{
//	//			START_LOG( cwarn, L"이벤트 아이템 획득 처리 하려는데 유저정보가 없다!" )
//	//				<< BUILD_LOG( kPacket_.m_uiAttUnit )
//	//				<< END_LOG;
//	//		}
//	//	}
//	//	//////////////////////////////////////////////////////////////////////////		
//	//}
//
//	//////////////////////////////////////////////////////////////////////////
//	// 4. 속성 몬스터 드롭 아이템
//	KDropTable::DROP_DATA sDropDataAttribNpc;
//
//	if( bIsAttribNpc == true )
//	{
//		if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(GetDifficultyLevel()), 0, iDropItemNum, sDropDataAttribNpc.m_vecItemID ) == false )
//		{
//			START_LOG( cerr, L"속성 몬스터 아이템 드롭 실패!" )
//				<< BUILD_LOGc( GetDifficultyLevel() )
//				<< END_LOG;
//		}
//
//		switch( GetDifficultyLevel() )
//		{
//		case CXSLDungeon::DL_NORMAL:
//		case CXSLDungeon::DL_HARD:
//			{
//				sDropDataNpc.m_nGPNum = 2 * sDropDataNpc.m_nGPNum;
//			}
//			break;
//
//		case CXSLDungeon::DL_EXPERT:
//			{
//				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;
//			}
//			break;
//
//		default:
//			START_LOG( cerr, L"던전 난이도가 이상합니다." )
//				<< BUILD_LOGc( GetDifficultyLevel() )
//				<< END_LOG;
//			break;
//		}
//
//		//{{ 2009. 12. 18  최육사	던전사용아이템
//#ifdef DUNGEON_ITEM
//
//		if( SiKDropTable()->AttribNpcSpecialDrop( GetDifficultyLevel(), sDropDataAttribNpc.m_vecItemID ) == false )
//		{
//			START_LOG( cerr, L"속성 몬스터 스페셜 아이템 드롭 실패!" )
//				<< BUILD_LOGc( GetDifficultyLevel() )
//				<< END_LOG;
//		}
//
//#endif DUNGEON_ITEM
//		//}}
//	}	
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 5. 퀘스트 드롭 아이템 처리
//	{
//		std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;
//
//		//유닛별 퀘스트 드롭 아이템이 있는지 검사..
//		if( m_spUserManager->GetQuestDropItem( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
//		{
//			//있다면 유닛별로 루푸를 돌면서..
//			BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
//			{
//				//드롭된 아이템 수만큼 처리를 시작..
//				BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
//				{
//					KEGS_GET_ITEM_NOT kDropNot;
//					kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
//					kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
//					kDropNot.m_iItemID		= iDropQuestItemID;
//					//{{ 2009. 7. 9  최육사		임시 인벤토리
//					kDropNot.m_iDropItemUID = m_nDropItemUID++;
//					//}}
//					SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
//				}
//			}
//		}
//		else
//		{
//			if( NetError::GetLastError() != NetError::NET_OK )
//			{
//				START_LOG( cerr, L"퀘스트 드롭 아이템 받아오기 실패.!" )
//					<< BUILD_LOG( kPacket_.m_uiAttUnit )
//					<< BUILD_LOG( m_iDungeonID )
//					<< BUILD_LOG( NetError::GetLastErrMsg() )
//					<< END_LOG;
//			}
//		}
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 위에서 결정된 드롭 아이템 리스트를 실제 유저에게 보내기 위한 처리
//	KEGS_DROP_ITEM_NOT kDropItemNot;
//	kDropItemNot.m_CreatePos = kPacket_.m_DiePos;
//
//	//1.ED 드롭처리
//	if( sDropDataNpc.m_iED != -1 )
//	{
//		for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, kPacket_.m_mapDamageByUser, true, sDropDataNpc.m_iED ) );
//		}
//	}
//
//	//2.NPC ITEM 드롭처리	
//	if( sDropDataNpc.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, kPacket_.m_mapDamageByUser ) );
//		}
//	}
//
//	//{{ 2009. 4. 22  최육사	전체드롭
//	if( sDropDataStatic.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, kPacket_.m_mapDamageByUser ) );
//		}
//	}
//	//}}
//
//	//3.EVENT ITEM 드롭처리
//	//if( /*mit->second.m_bNoDrop == false &&*/ kDieNpcInfo.m_bActive == true && sDropDataEvent.m_vecItemID.empty() == false )
//	//{
//	//	BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataEvent.m_vecItemID )
//	//	{
//	//		// 특정 아이템이 특정던전에서 드롭되지 않도록 하는 코드
//	//		if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
//	//			continue;
//
//	//		kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
//	//	}
//	//}
//
//	//드롭처리한 아이템이 있으면 패킷을 보낸다.
//	if( kDropItemNot.m_DropItemDataList.empty() == false )
//	{
//		// 해당 몬스터에게 실제 대미지를 준 캐릭터에게만 아이템 드롭 정보를 보낸다!
//		std::map< UidType, float >::const_iterator mit;
//		for( mit = kPacket_.m_mapDamageByUser.begin(); mit != kPacket_.m_mapDamageByUser.end(); ++mit )
//		{
//			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
//			if( spRoomUser == NULL )
//				continue;
//
//			if( spRoomUser->IsBattleFieldNpcLoad() == false )
//				continue;
//
//			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
//		}
//	}
//}

#endif SERV_REFORM_ITEM_DROP
//}}

_IMPL_ON_FUNC( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ )
{
	//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_DIE_ACK, KERM_USER_UNIT_DIE_ACK );
#else
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_USER_UNIT_DIE_ACK, KEGS_USER_UNIT_DIE_ACK );
#endif SERV_ADD_TITLE_CONDITION
	//}}

	// Die 체크
	if( !m_spUserManager->SetDie( FIRST_SENDER_UID, true ) )
	{
		kPacket.m_iOK = NetError::ERR_SLOT_07;
		//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		KERM_USER_UNIT_DIE_ACK kAck;
		kAck.m_iOK = kPacket.m_iOK;
		kAck.m_ucDieReason = kPacket_.m_ucDieReason;
		kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
		kAck.m_VP			= kPacket.m_VP;
		kAck.m_EXP			= kPacket.m_EXP;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
		//}}
		return;
	}

	// 성공하면 성공패킷을 해당유저에게 보내고
	kPacket.m_iOK = NetError::NET_OK;

	//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
	KERM_USER_UNIT_DIE_ACK kAck;
	kAck.m_iOK = kPacket.m_iOK;
	kAck.m_ucDieReason = kPacket_.m_ucDieReason;
	kAck.m_fReBirthTime	= kPacket.m_fReBirthTime;
	kAck.m_VP			= kPacket.m_VP;
	kAck.m_EXP			= kPacket.m_EXP;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kAck );
#else
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_ACK, kPacket );
#endif SERV_ADD_TITLE_CONDITION
	//}}

	

	// 방의 모든 유저에게 정보를 알린다.
	KERM_USER_UNIT_DIE_NOT kPacketNot;
	kPacketNot.m_iGameType			= m_cGameType;
	kPacketNot.m_KillerNPCUID = kPacket_.m_KillerNPCUID;
	kPacketNot.m_KilledUserUnitUID = FIRST_SENDER_UID;
	BroadCast( ERM_USER_UNIT_DIE_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );

	//{{ 2008. 6. 17  최육사  실시간 부활석
	m_spUserManager->ReserveEndPlay( FIRST_SENDER_UID );
	//}}
}

IMPL_ON_FUNC_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_USER_UNIT_DIE_COMPLETE_ACK, KEGS_USER_UNIT_DIE_COMPLETE_ACK );	

	KEGS_USER_UNIT_DIE_COMPLETE_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_USER_UNIT_DIE_COMPLETE_ACK, kAck );

	KEGS_USER_UNIT_DIE_COMPLETE_NOT kNot;
	kNot.m_UnitUID = FIRST_SENDER_UID;

	BroadCast( ERM_USER_UNIT_DIE_COMPLETE_NOT, kNot );
}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
_IMPL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	KEGS_GET_ITEM_NOT kNot;

	std::map< int, ITEM_DATA >::iterator mit;

	KRoomUserPtr spGetUser = m_spUserManager->GetUser( FIRST_SENDER_UID );
	if( spGetUser == NULL )
	{
		START_LOG( cerr, L"습득하려는 유저가 없당." )
		<< BUILD_LOG( FIRST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_iDropItemUID )
		<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
		<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	UidType iGetUnitPartyUID = spGetUser->GetPartyUID();

	// 1. 습득할려는 item이 없는경우
	mit = m_mapDropItem.find( kPacket_.m_iDropItemUID );
	if( mit == m_mapDropItem.end() )
	{
		START_LOG( cerr, L"습득할려는 아이템이 없음" )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	KBattleFieldRoom::ITEM_DATA& sItemData = mit->second;

	// 2. 아이템 종류 검사
	if( kPacket_.m_kItemInfo.m_iItemID != sItemData.m_iItemID )
	{
		START_LOG( cerr, L"습득할려는 아이템의 종류가 잘못되었음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( sItemData.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_05;
		goto end_proc;
	}

	// 3. 다른 유저가 이미 습득한건가
	if( sItemData.m_bIsGet == true )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_01;
		goto end_proc;
	}

	// 4. 획득 가능한 유저인가?
	if( sItemData.IsGetItemPossibleUnit( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"아이템을 획득 할 수 없는 캐릭터인데 획득 요청을 보냈군.. 해킹인가?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	kNot.m_iDropItemUID	= kPacket_.m_iDropItemUID;
	kNot.m_iItemID		= kPacket_.m_kItemInfo.m_iItemID;
	//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	kNot.m_cEnchantLevel= sItemData.m_cEnchantLevel;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

	// 순간소비 아이템
	bool bNotCreateItem = false;
	UidType iGSUID = LAST_SENDER_UID; // ServerUID저장

	// 4. 먹은 아이템이 ED인가? Item인가?

	//////////////////////////////////////////////////////////////////////////	
	// ED아이템
	if( sItemData.m_bIsGP == true )
	{
		kNot.m_iTotalED = sItemData.m_iED;
		kNot.m_GetUnitUID = FIRST_SENDER_UID; // 클라이언트 화면에 어떤놈이 ED먹었나 표시해줄때 사용
		float fPartyBonusRate = m_spUserManager->GetBattleFieldPartyBonusRate( kNot.m_GetUnitUID );
		if( m_spUserManager->AddBattleFieldRewardED( sItemData.m_cNpcLevel, kNot.m_GetUnitUID, fPartyBonusRate, sItemData.m_iED, sItemData.m_mapDamageByUser, kNot ) == false )
		{
			START_LOG( cerr, L"ED보상처리가 이상합니다!" )
				<< BUILD_LOG( kNot.m_GetUnitUID )
				<< BUILD_LOGc( sItemData.m_cNpcLevel )
				<< BUILD_LOG( sItemData.m_iNpcMaxHP )
				<< BUILD_LOG( sItemData.m_mapDamageByUser.size() )
				<< BUILD_LOG( sItemData.m_iED )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;
			goto end_proc;
		}

		bNotCreateItem = true;
	}
	//////////////////////////////////////////////////////////////////////////	
	// 아이템	먹는 사람에게 무조건 들어간다
	else
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"Item Templet 정보가 없음. 이상한 아이템 먹었네." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( sItemData.m_iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;

			sItemData.m_bIsGet = true; // 먹은걸로 처리
			goto end_proc;
		}
		else
		{
			kNot.m_GetUnitUID	= FIRST_SENDER_UID;

			//순간소비 아이템이면 인벤에 넣지 않기위해
			if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
			{
				bNotCreateItem = true;	
			}
		}
	}

	sItemData.m_bIsGet	= true;	//오류가 없어야 아이템을 먹은것으로 체크

	// ack를 먼저 보낸다
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );

	// 아이템 획득 결과 보내기 - 수량성이 아닌 아이템
	if( bNotCreateItem == false )
	{
		// 유저들에게 아이템 획득을 알림 - 필드안의 파티원 정보 얻기
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
			return;

		std::map< UidType, KRoomUserPtr >::const_iterator mit;
		for( mit = mapRoomUserList.begin(); mit != mapRoomUserList.end(); ++mit )
		{
			KRoomUserPtr spRoomUser = mit->second;
			if( spRoomUser == NULL )
				continue;
	
			if( spRoomUser->IsBattleFieldNpcLoad() == false )
				continue;
			
			if( spRoomUser->IsHost() == true )
			{
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
				continue;		// 아래 구문을 타게 되면 중복으로 not 가 전송됨
			}

			if( spRoomUser->GetPartyUID() != iGetUnitPartyUID )
				continue;

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
		}

		// 실제로 아이템 획득 처리를 할 패킷을 획득 유저에게 전송
		SendToGSCharacter( iGSUID, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
		//}}

		//{{ 2009. 6. 23  최육사	드롭 획득 공지 아이템 - 비던아이템, 매력아이템
		//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		if(		( sItemData.m_bNotify == true )
			||	( SiKDropTable()->IsNotifyDropItem( sItemData.m_iItemID ) )
			)
#else
		if( SiKDropTable()->IsNotifyDropItem( sItemData.m_iItemID ) )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kNot.m_GetUnitUID );
			if( spRoomUser != NULL )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
				if( pItemTemplet != NULL )
				{
					KEGS_NOTIFY_MSG_NOT	kPacket;
					kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM; // 아이템 획득 공지
					kPacket.m_Count = 1;
					kPacket.m_wstrMSG = spRoomUser->GetNickName();
					kPacket.m_wstrMSG += L";";
					//{{ 2012. 1. 17	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
					if( sItemData.m_bNotify == true )
					{
#ifdef SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += boost::str( boost::wformat( L"+%d; " ) % static_cast<int>( sItemData.m_cEnchantLevel ) );
#else //SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += boost::str( boost::wformat( L"+%d " ) % static_cast<int>( sItemData.m_cEnchantLevel ) );
#endif //SERV_UNITED_SERVER_EU
					}
#endif SERV_ENCHANT_ITEM_DROP_EVENT
					//}}
#ifdef SERV_UNITED_SERVER_EU
					kPacket.m_wstrMSG += pItemTemplet->m_ItemID;
#else //SERV_UNITED_SERVER_EU
					kPacket.m_wstrMSG += pItemTemplet->m_Name;
#endif //SERV_UNITED_SERVER_EU					
					KncSend( PI_CN_ROOM, GetUID(), PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, ECN_NOTIFY_MSG_NOT, kPacket );
				}
				else
				{
					START_LOG( cerr, L"드롭 획득 아이템 공지를 날리려는데 존재하지 않는 아이템입니다" )
						<< BUILD_LOG( kNot.m_GetUnitUID )
						<< BUILD_LOG( sItemData.m_iItemID )
						<< END_LOG;
				}
			}
			else
			{
				START_LOG( cerr, L"드롭 획득 아이템 공지를 날리려는데 방에 유저가 없습니다" )
					<< BUILD_LOG( kNot.m_GetUnitUID )
					<< END_LOG;
			}
		}
	}
	else
	{
		if( true == SiKDropTable()->CheckPVPItem( kPacket_.m_kItemInfo.m_iItemID ) )
		{
			if( static_cast<float>(::rand()%100) > SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_LOSETEAM ) )
				kNot.m_bIsItemSuccess = true;
		}
		// 순간소비 아이템과 ED아이템은 브로드캐스팅

		// 유저들에게 아이템 획득을 알림 - 필드안의 파티원 정보 얻기
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
			return;

		std::map< UidType, KRoomUserPtr >::const_iterator mitUser;
		for( mitUser = mapRoomUserList.begin(); mitUser != mapRoomUserList.end(); ++mitUser )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUser->first );
			if( spRoomUser == NULL )
				continue;

			if( spRoomUser->IsBattleFieldNpcLoad() == false )
				continue;

			if( spRoomUser->IsHost() == true )
			{
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
				continue;		// 아래 구문을 타게 되면 중복으로 not 가 전송됨
			}

			if( spRoomUser->GetPartyUID() != iGetUnitPartyUID )
				continue;

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
		}
	}

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );	
}
#else
_IMPL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	KEGS_GET_ITEM_NOT kNot;

	std::map< int, ITEM_DATA >::iterator mit;
	mit = m_mapDropItem.find( kPacket_.m_iDropItemUID );

	// 1. 습득할려는 item이 없는경우
	if( mit == m_mapDropItem.end() )
	{
		START_LOG( cerr, L"습득할려는 아이템이 없음" )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	KBattleFieldRoom::ITEM_DATA& sItemData = mit->second;

	// 2. 아이템 종류 검사
	if( kPacket_.m_kItemInfo.m_iItemID != sItemData.m_iItemID )
	{
		START_LOG( cerr, L"습득할려는 아이템의 종류가 잘못되었음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( sItemData.m_iItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_05;
		goto end_proc;
	}

	// 3. 다른 유저가 이미 습득한건가
	if( sItemData.m_bIsGet == true )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_01;
		goto end_proc;
	}

	//{{ 2012. 02. 27	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 4. 획득 가능한 유저인가?
	if( sItemData.IsGetItemPossibleUnit( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"아이템을 획득 할 수 없는 캐릭터인데 획득 요청을 보냈군.. 해킹인가?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	kNot.m_iDropItemUID	= kPacket_.m_iDropItemUID;
	kNot.m_iItemID		= kPacket_.m_kItemInfo.m_iItemID;

	// 순간소비 아이템
	bool bNotCreateItem = false;
	UidType iGSUID = LAST_SENDER_UID; // ServerUID저장

	// 4. 먹은 아이템이 ED인가? Item인가?

	//////////////////////////////////////////////////////////////////////////	
	// ED아이템
	if( sItemData.m_bIsGP == true )
	{
		kNot.m_iTotalED = sItemData.m_iED;
		kNot.m_GetUnitUID = FIRST_SENDER_UID; // 클라이언트 화면에 어떤놈이 ED먹었나 표시해줄때 사용

		if( m_spUserManager->AddBattleFieldRewardED( sItemData.m_cNpcLevel, sItemData.m_iNpcMaxHP, sItemData.m_mapDamageByUser, sItemData.m_iED, kNot ) == false )
		{
			START_LOG( cerr, L"ED보상처리가 이상합니다!" )
				<< BUILD_LOGc( sItemData.m_cNpcLevel )
				<< BUILD_LOG( sItemData.m_iNpcMaxHP )
				<< BUILD_LOG( sItemData.m_mapDamageByUser.size() )
				<< BUILD_LOG( sItemData.m_iED )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;
			goto end_proc;
		}

		bNotCreateItem = true;
	}
	//////////////////////////////////////////////////////////////////////////	
	// 아이템
	else
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"Item Templet 정보가 없음. 이상한 아이템 먹었네." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( sItemData.m_iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ITEM_04;

			sItemData.m_bIsGet = true; // 먹은걸로 처리
			goto end_proc;
		}
		else
		{
			kNot.m_GetUnitUID	= FIRST_SENDER_UID;

			//순간소비 아이템이면 인벤에 넣지 않기위해
			if( pItemTemplet->m_ItemType != CXSLItem::IT_OUTLAY )
			{
				//아이템 습득방식에 따라..
				if( m_cGetItemType == CXSLRoom::DGIT_RANDOM )
				{
					// Item을 누가 받을지 알 수 없기 때문에 실제로 받은 Unit의 UID를 반환해준다.
					if( m_spUserManager->GetItemWithRandom( sItemData.m_mapDamageByUser, FIRST_SENDER_UID, sItemData.m_iItemID, GetDungeonID(), kNot.m_GetUnitUID, iGSUID ) == false )
					{
						kPacket.m_iOK = NetError::ERR_ITEM_03;
						goto end_proc;
					}
				}
			}
			else
			{
				bNotCreateItem = true;
			}
		}
	}

	sItemData.m_bIsGet	= true;	//오류가 없어야 아이템을 먹은것으로 체크

	// ack를 먼저 보낸다
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );

	{
		// 아이템 획득 결과 보내기
		if( bNotCreateItem == false )
		{
			// 실제로 획득한 유저에게 보냄
			//{{ 2010. 03. 03  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
			//{{ 2010. 03. 18  최육사	아이템 획득 코드 개선

			// 유저들에게 아이템 획득을 알림
			std::map< UidType, float >::const_iterator mit;
			for( mit = sItemData.m_mapDamageByUser.begin(); mit != sItemData.m_mapDamageByUser.end(); ++mit )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
			}

			// 실제로 아이템 획득 처리를 할 패킷을 획득 유저에게 전송
			SendToGSCharacter( iGSUID, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
			//}}

#else
#pragma ELSE_MESSAGE
			//SendToGSCharacter( LAST_SENDER_UID, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
#endif SERV_CHANNEL_PARTY
			//}}

			//{{ 2009. 6. 23  최육사	드롭 획득 공지 아이템 - 비던아이템, 매력아이템
			if( SiKDropTable()->IsNotifyDropItem( sItemData.m_iItemID ) )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kNot.m_GetUnitUID );
				if( spRoomUser != NULL )
				{
					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( sItemData.m_iItemID );
					if( pItemTemplet != NULL )
					{
						KEGS_NOTIFY_MSG_NOT	kPacket;
						kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_DROP_GET_ITEM; // 아이템 획득 공지
						kPacket.m_Count = 1;
						kPacket.m_wstrMSG = spRoomUser->GetNickName();
						kPacket.m_wstrMSG += L";";
#ifdef SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += pItemTemplet->m_ItemID;
#else //SERV_UNITED_SERVER_EU
						kPacket.m_wstrMSG += pItemTemplet->m_Name;
#endif //SERV_UNITED_SERVER_EU
						KncSend( PI_CN_ROOM, GetUID(), PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, ECN_NOTIFY_MSG_NOT, kPacket );
					}
					else
					{
						START_LOG( cerr, L"드롭 획득 아이템 공지를 날리려는데 존재하지 않는 아이템입니다" )
							<< BUILD_LOG( kNot.m_GetUnitUID )
							<< BUILD_LOG( sItemData.m_iItemID )
							<< END_LOG;
					}
				}
				else
				{
					START_LOG( cerr, L"드롭 획득 아이템 공지를 날리려는데 방에 유저가 없습니다" )
						<< BUILD_LOG( kNot.m_GetUnitUID )
						<< END_LOG;
				}
			}
			//}}
		}
		else
		{
#ifdef DUNGEON_ITEM
			if( true == SiKDropTable()->CheckPVPItem( kPacket_.m_kItemInfo.m_iItemID ) )
			{
				if( static_cast<float>(::rand()%100) > SiKDropTable()->GetPVPDropFailRate( KDropTable::PF_LOSETEAM ) )
					kNot.m_bIsItemSuccess = true;
			}
#endif DUNGEON_ITEM
			// 순간소비 아이템과 ED아이템은 브로드캐스팅
			std::map< UidType, float >::const_iterator mit;
			for( mit = sItemData.m_mapDamageByUser.begin(); mit != sItemData.m_mapDamageByUser.end(); ++mit )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_NOT, kNot );
			}
		}
	}

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );	
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2009. 5. 11  최육사	실시간아이템
IMPL_ON_FUNC( ERM_GET_ITEM_COMPLETE_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
		return;

	std::map< int, int >::const_iterator mit;
	for( mit = kPacket_.m_mapGetItem.begin(); mit != kPacket_.m_mapGetItem.end(); ++mit )
	{
		if( !m_spUserManager->AddItem( kPacket_.m_iUnitUID, mit->first ) )
		{
			START_LOG( cerr, L"아이템 획득 정보 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}
	}
}
//}}

//{{ 2008. 9. 19  최육사		통계
_IMPL_ON_FUNC( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cwarn, L"패킷이 허용되는 방 상태가 아님." )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		return;
	}

	if( !m_spUserManager->SetDungeonPlayInfo( FIRST_SENDER_UID, kPacket_ ) )
	{
		START_LOG( cwarn, L"업데이트 실패했네?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
}
//}}

_IMPL_ON_FUNC( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_PLAY ) );

    BroadCast( ERM_USE_QUICK_SLOT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<KRoomUserManager::KLeaveUserInfo>	vecDelUserUID;
	const int iCount = m_spUserManager->DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_spUserManager->GetNumMember() );

	//현재방에서 삭제된 유저가 없으면 종료.
	if( iCount == 0 )
		return;

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		m_spUserManager->LeaveRoom( kInfo.m_iUnitUID );
	}

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		if( IsEmpty() == false )
		{
			// 방의 모든 유저들에게 슬롯 정보를 날린다.
			KEGS_LEAVE_BATTLE_FIELD_NOT kPacketNot;
			kPacketNot.m_cRoomState		= GetStateID();
			kPacketNot.m_iUnitUID		= kInfo.m_iUnitUID;
			kPacketNot.m_iPartyUID		= kInfo.m_iPartyUID;
			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_BATTLE_FIELD_NOT, kPacketNot );
		}
	}

	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"배틀필드에서는 사용하지 않는 상태입니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( iCount )
			<< END_LOG;
	}

	// 방에서 모든 유저가 나갔으면 일단 WAIT상태로 바꾼다.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();

		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_DELETE );
	}
	else
	{
		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}
}

_IMPL_ON_FUNC( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );

	bool bDie;
	if( !m_spUserManager->IsDie( kPacket_.m_iUnitUID, bDie ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

		return;
	}

	if( !bDie )
	{
		kPacket.m_iOK = NetError::ERR_RESURRECT_01;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

		return;
	}

	if( !m_spUserManager->SetDie( kPacket_.m_iUnitUID, false ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

		return;
	}

	kPacket.m_iDungeonID = (m_iDungeonID + static_cast<int>(m_cDifficultyLevel));
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacket );

	KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT kPacketNot;
	kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;

	KRoomUserPtr roomSlotPtr = m_spUserManager->GetUser( kPacketNot.m_iUnitUID );
	if( roomSlotPtr == NULL )
	{
		START_LOG( cerr, L"방에 룸유저 객체가 존재하지 않음." )
			<< BUILD_LOG( m_cRoomType )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
	else
	{
		//{{ 2007. 11. 5  최육사  부활석 사용 카운트
		roomSlotPtr->IncreaseUsedResurrectionStoneCount();
		//}}
		//{{ 2007. 12. 25  최육사  부활석 사용 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_RES_STONE, kKey, KStatistics::eSIColResStone_UseCount, 1 );
		//}}
	}

	kPacketNot.m_iUsedByUnitUID = kPacket_.m_iUsedByUnitUID;
	kPacketNot.m_iStartPosIndex = 0;//SiCXSLDungeonManager()->GetStartPosByRandom( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), m_iStageID, m_iSubStageID );
	BroadCast( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacketNot );

	//{{ 2008. 6. 17  최육사  실시간 부활석
	m_spUserManager->SetEndPlay( kPacket_.m_iUnitUID, false );
	//}}
}

//IMPL_ON_FUNC_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT )
//{
//	if( GetStateID() != KRoomFSM::S_PLAY )
//	{
//		START_LOG( cwarn, L"패킷이 허용되는 방 상태가 아님." )
//			<< BUILD_LOG( GetStateIDString() )
//			<< END_LOG;
//
//		return;
//	}
//
//	//{{ 2008. 6. 17  최육사  실시간 부활석
//	if( m_spUserManager->CheckEndPlayAnotherMember( FIRST_SENDER_UID ) )
//		EndPlay();
//	//}}
//}

//{{ 2008. 6. 16  최육사  실시간 부활석
_IMPL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK );

	// 컨티뉴 시간 멈추기 or 풀기
	m_spUserManager->StopDungeonContinueTime( FIRST_SENDER_UID, kPacket_.m_bIsStop );

	kPacket.m_iOK	  = NetError::NET_OK;
	kPacket.m_bIsStop = kPacket_.m_bIsStop;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, kPacket );
}
//}}

//{{ 2011. 01. 25  김민성  월드 트리거 전달(던전만 적용중 - 이후 PVP 확장)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
_IMPL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ECN_WORLD_TRIGGER_RELOCATION_ACK, KEGS_WORLD_TRIGGER_RELOCATION_ACK );

	// 방안에 모든 유저에게 브로드 캐스트	
	KEGS_WORLD_TRIGGER_RELOCATION_NOT _Not;
	_Not.m_iWorldTrigger = kPacket_.m_iWorldTrigger;
	BroadCast( ECN_WORLD_TRIGGER_RELOCATION_NOT, _Not );
	
	// GSUser에게 ACK전달
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ECN_WORLD_TRIGGER_RELOCATION_ACK, kPacket );
}
#endif SERV_WORLD_TRIGGER_RELOCATION
//}}

IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, KEGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK );

    if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"해당 방에 유저가 없는데 패킷이 날아왔다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );
		return;
	}

	if( m_spRoomUserManager->SetBattleFieldNpcSyncSubjects( FIRST_SENDER_UID, true ) == false )
	{
		START_LOG( cerr, L"NPC 동기화 대상자로 선정 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK, kPacket );

	// 방장에게 NPC정보 동기화 요청을 보내자!
	CheckNpcP2PSyncUser( true );
}


//#ifndef SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//
//IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT )
//{
//	if( GetStateID() != KRoomFSM::S_PLAY )
//	{
//		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
//			<< BUILD_LOG( GetUID() )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< END_LOG;
//		return;
//	}
//
//	//{{ 2012. 12. 07	최육사	배틀필드 시스템
//#ifdef SERV_CODE_CLEANUP_2012_12_07
//	// [2012-12-07][최육사] 정상적인 예외상황이므로 굳이 에러 출력을 할 필요가 없다.
//	if( m_spRoomUserManager->IsExist( FIRST_SENDER_UID ) == false )
//	{
//		START_LOG( cerr, L"NPC 동기화 대상자를 선정 해제하려고 하는데 유저가 방에 없다!" )
//			<< BUILD_LOG( GetUID() )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< END_LOG;
//		return;
//	}
//#endif SERV_CODE_CLEANUP_2012_12_07
//	//}}
//
//	if( m_spRoomUserManager->SetBattleFieldNpcSyncSubjects( FIRST_SENDER_UID, false ) == false )
//	{
//		START_LOG( cerr, L"NPC 동기화 대상자로 선정 해제 실패!" )
//			<< BUILD_LOG( GetUID() )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< END_LOG;
//	}
//}
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND


IMPL_ON_FUNC( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 파티 정보 갱신
	m_spUserManager->UpdatePartyInfo( FIRST_SENDER_UID, kPacket_ );

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
}

IMPL_ON_FUNC( ERM_DUMP_ROOM_MONSTER_NOT )
{
	UidType iGSUID = 0;
	UidType iUnitUID = 0;
    if( m_spRoomUserManager->GetRoomUserGS( kPacket_.m_wstrNickName, iGSUID, iUnitUID ) == true )
	{
		if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_iNpcUID ) == false )
		{
			START_LOG( cout, L"[덤프] 해당 몬스터는 죽었습니다!" )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< BUILD_LOG( kPacket_.m_iNpcUID );
		}
		else
		{
			START_LOG( cout, L"[덤프] 해당 몬스터는 살아있습니다!" )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< BUILD_LOG( kPacket_.m_iNpcUID );

			// 몬스터가 아직 살아있다면 몬스터 정보 요청!
			//KRoomMonsterManager::NPC_DATA kDieNpcInfo;
			//if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
			//{
			//	kPacket.m_iOK = NetError::ERR_ROOM_20;
			//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
			//	return;
			//}
		}
	}
}

IMPL_ON_FUNC( ERM_DUMP_BATTLE_FIELD_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( clog, L"플레이 중인 방이 아니면 체크할 필요 없다." )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;
		return;
	}

	START_LOG( cout, L"---------------------------------------------------------------------------------------" );

	//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//////////////////////////////////////////////////////////////////////////
	// 방에 입장한 유저가 있다면 
	if( m_spUserManager->GetNumMember() > 0 )
	{
		// 호스트가 제대로 있는지 검사
		if( m_spUserManager->GetHostCount() != 1 )
		{
			START_LOG( cout, L"host가 1명이 아니다!! 이런 병신같은 일이!!!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_spUserManager->GetNumMember() )
				<< BUILD_LOG( m_spUserManager->GetHostCount() );
		}

		// 좀비 체크 여부
		if( m_spUserManager->ZU_IsCheckZU() == false )
		{
			START_LOG( cout, L"유저는 존재하는데 좀비 유저 체크가 꺼져있다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_spUserManager->GetNumMember() )
				<< BUILD_LOG( m_spUserManager->ZU_IsCheckZU() );
		}
	}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}
	

	//////////////////////////////////////////////////////////////////////////
	// 해당 유저가 필드에 입장해있는지 찾아보자!
	if( kPacket_.m_wstrNickName.empty() == false )
	{
		std::vector< UidType > vecUserList;
		m_spUserManager->GetUnitUIDList( vecUserList );

		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
			if( IS_NULL( spRoomUser ) )
			{
				START_LOG( cerr, L"포인터값이 이상합니다." )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			if( spRoomUser->GetNickName() != kPacket_.m_wstrNickName )
				continue;

			START_LOG( cout, L"해당 유저를 찾았습니다!" )
				<< BUILD_LOG( spRoomUser->GetNickName() )
				<< BUILD_LOG( spRoomUser->IsHost() );

			if( spRoomUser->GetPartyUID() == 0 )
			{
				START_LOG( cout, L"파티에 속해 있지 않습니다!" )
					<< BUILD_LOG( spRoomUser->GetNickName() );
				continue;
			}

			std::vector< UidType > vecPartyMemberList;
			if( m_spUserManager->GetPartyMemberListInfo( spRoomUser->GetPartyUID(), vecPartyMemberList ) == false )
			{
				START_LOG( cerr, L"파티 리스트 정보가 없습니다! 일어나서는 안되는 에러!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( spRoomUser->GetPartyUID() )
					<< END_LOG;
				continue;
			}

			START_LOG( cout, L"파티에 속해 있습니다!" )
				<< BUILD_LOG( spRoomUser->GetNickName() )
				<< BUILD_LOG( spRoomUser->GetPartyUID() )
				<< BUILD_LOG( vecPartyMemberList.size() );

			BOOST_TEST_FOREACH( const UidType, iPartyMemberUnitUID, vecPartyMemberList )
			{
				START_LOG( cout, L"파티원 UnitUID : " << iPartyMemberUnitUID );
			}
		}
	}
}



IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZU_ECHO_ACK )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}
    
    // 아무것도 하지 않는다.
}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

    if ( m_spUserManager->ZombieAlert_IsCheck() == false )
    {
        return;
    }

    if ( KRoomUserPtr spRoomUser = m_spUserManager->GetUser( FIRST_SENDER_UID ) )
    {
#ifdef  SERV_PVP_NEW_SYSTEM
        if ( spRoomUser->IsPvpNpc() == false )
#endif  
        {
            spRoomUser->ZombieAlert_Set( false );
            spRoomUser->ZombieAlert_Refresh();
        }
    }
}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST



//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
IMPL_ON_FUNC( ERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( clog, L"플레이 중인 방이 아니면 체크할 필요 없다." )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 해당 유저가 필드에 입장해있는지 찾아보자!
	if( kPacket_.m_wstrNickName.empty() )
		return;

	std::vector< UidType > vecUserList;
	m_spUserManager->GetUnitUIDList( vecUserList );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
	{
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cerr, L"포인터값이 이상합니다." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		if( spRoomUser->GetNickName() != kPacket_.m_wstrNickName )
			continue;

		START_LOG( cout, L"좀비 유저를 찾았습니다! 해당 유저를 강제로 이탈 시킵니다!" )
			<< BUILD_LOG( spRoomUser->GetNickName() )
			<< BUILD_LOG( spRoomUser->IsHost() );

		// 강제 이탈 처리
		std::vector< UidType > vecDeleteZombie;
		vecDeleteZombie.push_back( spRoomUser->GetCID() );
		DeleteZombie( vecDeleteZombie );
	}
}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
//}}

//{{ 배틀필드 위험도 확인 치트 - 김민성
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_DANGEROUS_VALUE_REQ )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KERM_BATTLE_FIELD_DANGEROUS_VALUE_ACK kPacket;
	kPacket = m_kGameManager.GetDangerousValue();

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BATTLE_FIELD_DANGEROUS_VALUE_ACK, kPacket );
}
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
//}}


//{{ 2013. 03. 12	최육사	몬스터 유효 검사
#ifdef SERV_CHECK_EXIST_MONSTER_UID
_IMPL_ON_FUNC( ERM_CHECK_EXIST_MONSTER_UID_REQ, KEGS_CHECK_EXIST_MONSTER_UID_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_CHECK_EXIST_MONSTER_UID_ACK, KEGS_CHECK_EXIST_MONSTER_UID_ACK );

	// 루프를 돌면서 해당 몬스터가 실제로 있는지 확인한다
	BOOST_TEST_FOREACH( const int, iNpcUID, kPacket_.m_vecNpcUID )
	{
		// 없으면
        if( m_kMonsterManager.IsExistMonster( iNpcUID ) == false )
		{
			kPacket.m_vecNotExistNpcUID.push_back( iNpcUID );
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_EXIST_MONSTER_UID_ACK, kPacket );
}
#endif SERV_CHECK_EXIST_MONSTER_UID
//}}

//{{ 배틀필드 커뮤니티 리스트 확인 - 김민성
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
_IMPL_ON_FUNC( ERM_COMMUNITY_IN_BATTLE_FIELD_REQ, KEGS_COMMUNITY_USER_LIST_REQ )
{
	KEGS_COMMUNITY_USER_LIST_ACK kPacket;
	kPacket.m_iOK		  = NetError::NET_OK;
	kPacket.m_uiTotalPage = 1;
	kPacket.m_uiViewPage  = 1;

	std::vector< UidType > vecUserList;
	m_spUserManager->GetUnitUIDList( vecUserList );
	
	BOOST_TEST_FOREACH( UidType, iUnitUid, vecUserList )
	{
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUid );

		if( spRoomUser )
		{
			KRoomUserInfo kInfo;
			spRoomUser->GetRoomUserInfo( kInfo );

			KCommunityUserInfo kUser;
			kUser.m_iUnitUID = kInfo.m_nUnitUID;
			kUser.m_wstrNickName = kInfo.m_wstrNickName;
			kUser.m_cUnitClass = kInfo.m_cUnitClass;
			kUser.m_ucLevel = kInfo.m_ucLevel;
			kUser.m_cState = KCommunityUserInfo::US_NONE;	
			kUser.m_iStateCode = 0;
			kUser.m_iRoomUID = GetUID();
			kUser.m_iED = 0;

			kPacket.m_vecUserList.push_back( kUser );
		}
	}

	// 원하는 페이지의 정보를 전달하자(가공)
	GetPVPUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_COMMUNITY_IN_BATTLE_FIELD_ACK, kPacket );
};

bool KBattleFieldRoom::GetPVPUserList( IN UINT& uiViewPage, IN UINT& uiTotalPage, IN OUT std::vector< KCommunityUserInfo >& vecUserList )
{
	std::vector< KCommunityUserInfo > vecUserListCopy;

	vecUserListCopy = vecUserList;
	vecUserList.clear();

	_JIF( uiViewPage > 0, return false );

	if( vecUserListCopy.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return true;
	}

	uiTotalPage = vecUserListCopy.size() / PAGE_PER_BATTLE_FIELD_USER;
	if( ( vecUserListCopy.size() % PAGE_PER_BATTLE_FIELD_USER ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * PAGE_PER_BATTLE_FIELD_USER ) - PAGE_PER_BATTLE_FIELD_USER;
	if( vecUserListCopy.size() <= uiBeginIndex )
	{
		uiViewPage = uiTotalPage; // 마지막 페이지
		uiBeginIndex = ( uiViewPage * PAGE_PER_BATTLE_FIELD_USER ) - PAGE_PER_BATTLE_FIELD_USER;

		if( vecUserListCopy.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"일어날 수 없는 에러!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( vecUserListCopy.size() )
				<< END_LOG;
			return false;
		}
	}

	std::vector< KCommunityUserInfo >::reverse_iterator vit = vecUserListCopy.rbegin();
	vit += uiBeginIndex;

	while( vit != vecUserListCopy.rend() )
	{	
		vecUserList.push_back( *vit );

		if( vecUserList.size() >= PAGE_PER_BATTLE_FIELD_USER )
			break;

		++vit;
	}

	return true;
}
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
//}}

void KBattleFieldRoom::MakePacket_BattleFieldLeaveLog( IN const UidType iUnitUID, IN const int iReason, OUT KELOG_BATTLE_FIELD_LEAVE_LOG_NOT& kPacket )
{
	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	switch( iReason )
	{
	case NetError::NOT_LEAVE_ROOM_REASON_01:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_DISCONNECT;				break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_29:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_CHANNEL_CHANGE;			break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_27:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_WALK_TO_DIF_FIELD;		break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_30:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_USE_WARP_ITEM;			break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_31:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_MOVE_PARTY_FIELD;		break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_28:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_ENTER_THE_DUNGEON;		break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_32:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_ENTER_THE_PVP;			break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_33:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_ABNORMAL_DISCONNECT;	break;	// 체크
	case NetError::NOT_LEAVE_ROOM_REASON_23:	kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_CLIENT_CRASH;			break;	// 체크
	default:									kPacket.m_cLeaveType = KELOG_BATTLE_FIELD_LEAVE_LOG_NOT::LT_NONE;					break;
	}
	
	CTime tCurTime = CTime::GetCurrentTime();
	kPacket.m_wstrServerIP	 = NetCommon::GetLocalIPW();
	kPacket.m_iBattleFieldID = GetBattleFieldID();
	kPacket.m_iRoomUID		 = GetUID();
	kPacket.m_iUnitUID		 = iUnitUID;
	kPacket.m_wstrNickName	 = spRoomUser->GetNickName();
	kPacket.m_cUnitClass	 = spRoomUser->GetUnitClass_LUA();
	kPacket.m_ucLevel		 = spRoomUser->GetLevel();
	kPacket.m_iPlayTime		 = static_cast<int>(spRoomUser->GetPlayTime());
	kPacket.m_iGetEXP		 = spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP();
	kPacket.m_iGetED		 = spRoomUser->GetRewardED();
	kPacket.m_wstrRegDate	 = (const wchar_t*)tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
	kPacket.m_iFieldFrame	 = spRoomUser->GetFieldFrame();
#endif SERV_FIELD_FRAME_LOG
	//}}

#ifdef SERV_ADD_FIELD_LOG_COLUMN
	kPacket.m_sNpcDieCount		= spRoomUser->GetKillNPC_LUA();
#endif SERV_ADD_FIELD_LOG_COLUMN
}

//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
void KBattleFieldRoom::CheckDropEventCount()
{
	// 일정 간격으로 몬스터 동기화 유저 체크
	if( m_tTimer[TE_DROP_EVENT_CHECK].elapsed() < 10.0 )
		return;

#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	if( SiKGameEventManager()->CheckItemDropProbEvent( m_fItemDropEventProbRate ) == true )
	{
		if( m_fItemDropEventProbRate < REE_DROP_EVENT_PROB_DEFAULT )
			m_fItemDropEventProbRate = REE_DROP_EVENT_PROB_DEFAULT;
	}
#else // SERV_DROP_EVENT_RENEWAL
	if( SiKGameEventManager()->CheckItemDropProbEvent( m_iItemDropEventProbCount ) == true )
	{
		if( m_iItemDropEventProbCount < REE_DROP_EVENT_PROB_DEFAULT )
			m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
	}
#endif // SERV_DROP_EVENT_RENEWAL

	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	float fQuestItemDropEventProbRate = 0.f;
	if( SiKGameEventManager()->CheckQuestItemDropProbEvent( fQuestItemDropEventProbRate ) == true )
	{
		m_fQuestItemDropEventProbRate = fQuestItemDropEventProbRate + 1.f;

		if( m_fQuestItemDropEventProbRate < REE_DROP_EVENT_PROB_DEFAULT )
			m_fQuestItemDropEventProbRate = 1.f;
	}
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}
}
#endif SERV_ITEM_DROP_EVENT
//}}

//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
void KBattleFieldRoom::CheckEventMonster()
{
	// 일정 간격으로 이벤트 몬스터 출현을 체크합니다!
	if( m_tTimer[TE_EVENT_MONSTER].elapsed() < 1.0 )
		return;

	m_tTimer[TE_EVENT_MONSTER].restart();

	//////////////////////////////////////////////////////////////////////////
	// 이벤트 몬스터 최대 지연시간 체크
	DWORD dwElapTime = GetTickCount();

	// 몬스터 체크
	KEGS_NPC_UNIT_CREATE_NOT			kNpcCreateNot;
	std::vector<KERM_NPC_UNIT_DIE_NOT>	veckPacketNpcDie;
	if( m_kMonsterManager.CheckEventMonster( GetBattleFieldID(), kNpcCreateNot, veckPacketNpcDie ) == true )
	{
		// 생성 처리
		if( kNpcCreateNot.m_vecNPCUnitAck.empty() == false )
		{
			kNpcCreateNot.m_iBattleFieldID = GetBattleFieldID();
			BroadCast( ERM_NPC_UNIT_CREATE_NOT, kNpcCreateNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
		
		// 제거 처리
		BOOST_TEST_FOREACH( KERM_NPC_UNIT_DIE_NOT&, kNpcDietNot, veckPacketNpcDie )
		{
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}
	}

	dwElapTime = GetTickCount() - dwElapTime;
	if( m_dwMonsterRespawnCheckTick < dwElapTime )
	{
		START_LOG( cout, L"[알림] 이벤트 몬스터 체크 타임 최대 시간 갱신!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( dwElapTime )
			<< BUILD_LOG( m_dwMonsterRespawnCheckTick );

		m_dwMonsterRespawnCheckTick = dwElapTime;
	}
	//////////////////////////////////////////////////////////////////////////	
}
#endif SERV_FIELD_EVENT_MONSTER
//}}

//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
void KBattleFieldRoom::CheckMiddleBossMonster()
{
	// 일정 간격으로 보스 몬스터 출현을 체크합니다!
	if( m_tTimer[TE_MIDDLE_BOSS_MONSTER].elapsed() < 5.0 )
		return;

	m_tTimer[TE_MIDDLE_BOSS_MONSTER].restart();

	//////////////////////////////////////////////////////////////////////////
	// 보스 몬스터 최대 지연시간 체크
	DWORD dwElapTime = ::GetTickCount();  

	// 몬스터 체크
	KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kPacketNot;	
	if( m_kMonsterManager.CheckMiddleBossMonster( GetBattleFieldID(), m_kGameManager.GetDangerousValue(), m_kGameManager.GetDangerousEventInfo(), kPacketNot ) == true )
	{
		if( kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.empty() == false )
		{
			// 몬스터 리스폰 처리가 완료되었다면 
			kPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
			BroadCast( ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
            
            KFieldBossGenKillStat kLog;
            kLog.m_nFieldID = kPacketNot.m_kCreatePacket.m_iBattleFieldID;
            //kLog.m_nNumGenBoss = kPacketNot.m_kCreatePacket.m_vecNPCUnitAck.size();

            //std::vector< KNPCUnitNot > m_vecNPCUnitAck
            BOOST_TEST_FOREACH( KNPCUnitNot, kNPCUnit, kPacketNot.m_kCreatePacket.m_vecNPCUnitAck )
            {
                 kLog.m_nSpawnID = kNPCUnit.m_kNPCUnitReq.m_iBossGroupID;
            }
            
            SendToLogDB( ELOG_GEN_FIELD_MIDDLE_BOSS_STAT_NOT, kLog );

            START_LOG( cwarn, L"필드 중간 보스 생성 통계 테스트 " )
                << BUILD_LOG( kLog.m_nFieldID )
                << BUILD_LOG( kLog.m_nSpawnID )
                //<< BUILD_LOG( kLog.m_nNumGenBoss )
                << END_LOG;
		}

		// 중간 보스 몬스터가 등장해도 위험도는 초기화 하지 않는다.
	}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT kEventPacketNot;	
	if( m_kMonsterManager.CheckEventBossMonster( GetBattleFieldID(), m_kGameManager.GetDangerousValue(), m_kGameManager.GetDangerousEventInfo(), kEventPacketNot ) == true )
	{
		if( kEventPacketNot.m_kCreatePacket.m_vecNPCUnitAck.empty() == false )
		{
			// 몬스터 리스폰 처리가 완료되었다면 
			kEventPacketNot.m_kCreatePacket.m_iBattleFieldID = GetBattleFieldID();
			BroadCast( ERM_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT, kEventPacketNot, KRoomUserManager::UL_NPC_LOAD_USER );
		}

		// 중간 보스 몬스터가 등장해도 위험도는 초기화 하지 않는다.
	}
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
	dwElapTime = ::GetTickCount() - dwElapTime;
	if( m_dwMonsterRespawnCheckTick < dwElapTime )
	{
		START_LOG( cout, L"[알림]중간 보스 몬스터 체크 타임 최대 시간 갱신!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( dwElapTime )
			<< BUILD_LOG( m_dwMonsterRespawnCheckTick );

		m_dwMonsterRespawnCheckTick = dwElapTime;
	}
	//////////////////////////////////////////////////////////////////////////	
}

void KBattleFieldRoom::BattleFieldMiddleBossDropProcess( IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID, OUT std::vector< KDropItemData >& vecDropItem_ )
{
	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet 읽기 오류.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// 속성 몬스터인지 체크
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );

	// npc죽은상태
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
	
	// 몬스터 타입 상수 얻기
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// 필드 상수 얻기
	//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit값이 -1이면 유저의 공격을 받지 않고 npc가 자살한것임. [예:지뢰함정]
	// 플레이어가 몬스터를 죽인거라면 해당 플레이어의 킬몬스터수 체크
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"몹끼리 죽였네!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// 몬스터가 함정에 걸려 죽는경우임 : 아이템은 그대로 드롭처리함.
			}
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	// 1. 몬스터 종류에 따라 드롭 아이템이 결정되는 몬스터 드롭
	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// 각 유저마다 다른 내용의 패킷을 보내야 하므로 이렇게 처리한다.
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
		kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
		kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
		kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
		kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
		kNpcDietNot.m_cUserCount		 = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete	 = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능

		// 분할 지급을 위해 선 계산 되어야 할 내용
		// 1. 기여도 (X로 표기)
		std::map< UidType, float >	mapUserContribution;
		int iNpcMaxHP = kPacket_.m_iNpcMaxHP;
		std::map< UidType, float >::const_iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin();
		for( ; mitDamageByUser != kPacket_.m_mapDamageByUser.end() ; ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;

			// 현재 같은 필드에 있는 파티원 수
			const int iPartyMemberCount = m_spUserManager->GetSamePartyMember( mitDamageByUser->first );

			float fContribution 
				= m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) 
				/ (static_cast<float>(iNpcMaxHP)) / iPartyMemberCount;

			if( fContribution > 1.5f )
				fContribution = 1.5f;

			if( fContribution < 0.08f )
				fContribution = 0.f;

			// 솔로 플레이면 파티가 없으니 목록이 없을 수도 있다. 그래서 한번 해줘야 한다.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// 파티 원도 동일한 기여도로 셋팅한다.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}

		// 기여도 순위 구하기
		// 상위 30% 파티
		std::map< UidType, bool >	mapBonusItem;  // <unit UID, 습득 여부>
		m_spUserManager->CalculatePartyRank( kPacket_.m_mapDamageByUser, mapBonusItem );
		
		// 2. 몬스터의 드롭 정보 얻기
		KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
		if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"배틀 필드 npc drop info 얻어오기 실패" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;

			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		// 3. 기여도가 있는 유저별로 EXP, ED, DropItem 계산
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"보내려는  리스트를 찾을 수가 없다." )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// 필드 전원에게 해당 npc가 죽은것에 대한 정보를 브로드 캐스팅 한다! ( 일부유저 드롭, 경험치 제외 )
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
		for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
		{
			KDropTable::KBattleFieldNpcDropInfo kDropInfo;
			kDropInfo = kDropInfo_Init;

			if( mitUserList->second == NULL )
				continue;

			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			if( mitUserList->second->IsBattleFieldNpcLoad() == false )
				continue;

			// 해당 유저가 몬스터를 죽일때 얼만큼 공헌을 했는지 검사하자!
			std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
			if( mitUserContribution == mapUserContribution.end() )
			{
				// 공헌도가 없으면 보상 받을 자격이 없다!
				SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
				continue;
			}

			KDropTable::DROP_DATA sDropDataNpc;

			// 현재 같은 필드에 있는 파티원 수
			kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

			// 레벨 상수값 얻기( 파티 레벨 팩터)
			float fPartyLevelFactorMin = 0.f;
			float fPartyLevelFactorMax = 0.f;

			m_spUserManager->GetPartyLevelFactor( mitUserList->first, kDieNpcInfo.m_cLevel, fPartyLevelFactorMin, fPartyLevelFactorMax );

			const float fLevelFactor = (fPartyLevelFactorMin < fPartyLevelFactorMax) ? fPartyLevelFactorMin : fPartyLevelFactorMin;
            
			// 파티 플레이일 경우 아이템 확률 증가
			const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

			// 파티의 기여도가 50% 이상인가?
			if( mitUserContribution->second * kNpcDietNot.m_cUserCount >= 0.5f )
			{
				kNpcDietNot.m_bQuestComplete = true;
			}

			// 개별적으로 계산/지급 되어야 하는 품목
			// 1. ED
			// 2. EXP
			// 3. 아이템
			if( kDropInfo.m_iExp > 0 )
			{
				kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ));

				// 경험치를 주는 몬스터라면 내구도가 감소 된다.
				kNpcDietNot.m_bDecreaseEndurance = true;
			}

			if( kDropInfo.m_iED > 0 )
			{
				kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel ));
				// 해당 필드몹 평균 드롭 ED * 100
				kDropInfo.m_iED = kDropInfo.m_iED * 20;
			}

			// 중보가 드롭하는 일반 드롭은 레벨 조건을 만족해야만 드롭한다.
			float fUserContribution = mitUserContribution->second;
			if( fLevelFactor <= 0.f )
			{
				fUserContribution = 0.f;
			}

			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo
				//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
													  , fFieldEDFactor
													  , fFieldEXPFactor
#else
													  , fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
													  //}
													  , fLevelFactor
													  , fUserContribution
													  , kNpcDietNot.m_cUserCount
													  , fPartyDropBonus
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
													  , m_fItemDropEventProbRate
#else // SERV_DROP_EVENT_RENEWAL
													  , m_iItemDropEventProbCount
#endif // SERV_DROP_EVENT_RENEWAL
													  , kDieNpcInfo.m_bIsBoss
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-14	// 박세훈
													  , 1.0f
#endif // SERV_BATTLE_FIELD_BOSS
													  , sDropDataNpc
													  ) == false )
			{
				START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< END_LOG;
			}
	
			//////////////////////////////////////////////////////////////////////////
			// 필드 중간 보스 보너스 아이템
			// 기여도 상위 30% 이상이면 필드 중보 큐브 2개 지급
			// 기여도 그외 이면			필드 중보 큐브 1개 지급		
			CXSLBattleFieldManager::KFieldMiddleBossDropInfo kMiddleBossDropInfo;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			CXSLBattleFieldManager::KFieldEventBossDropInfo kEventBossDropInfo;
			//////////////////////////////////////////////////////////////////////////
			// 필드 이벤트 보스 보너스 아이템
			// 기여도 상위 30% 이상이면 이벤트 보너스 아이템 2개 지급
			// 기여도 그외 이면			이벤트 보너스 아이템 1개 지급		
			if( SiCXSLBattleFieldManager()->GetBattleFieldEventBossDropInfo( static_cast<int>(GetBattleFieldID()), kDieNpcInfo.m_iNPCID, kEventBossDropInfo ) == true )
			{
				// 기여도가 8% 이상이고 , 파티 레벨차이가 10이 하라면 아이템 지급
				bool bGiveCommonBonusItem = ( mitUserContribution->second > 0.f && fLevelFactor > 0.f ) ? true : false;

				if( bGiveCommonBonusItem == true )
				{
					sDropDataNpc.m_vecItemID.push_back( 70070 ); // HP 100% 즉시회복 음식
					sDropDataNpc.m_vecItemID.push_back( 70050 ); // MP 150 즉시회복 음식

					// 중간 보스의 혼 과 같은 아이템 도 줄것인가? 
					for( int iBonusCount = 0 ; iBonusCount < kEventBossDropInfo.m_iEventBossCommonBonusItemCount ; ++iBonusCount )
					{
						if( kEventBossDropInfo.m_iEventBossCommonBonusItemID <= 0 )
							break;

						sDropDataNpc.m_vecItemID.push_back( kEventBossDropInfo.m_iEventBossCommonBonusItemID );
					}

					// 필드 중간 보스 보너스 아이템 - 필드 중간 보스 큐브
					std::map< UidType, bool >::iterator mitBonus = mapBonusItem.find( mitUserList->second->GetCID() );
					if( mitBonus != mapBonusItem.end() )
					{
						if( mitBonus->second == true )				
						{
							// 존재하지만 기여도가 상위 30%가 되면 두개 줌
							for( int iCount = 0 ; iCount < kEventBossDropInfo.m_iEventBossBonusItemIDCount_High ; ++iCount )
							{
								if( kEventBossDropInfo.m_iEventBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kEventBossDropInfo.m_iEventBossBonusItemID ); // 이벤트 보너스 아이템 큐브
							}
						}
						else
						{
							// 존재하지만 기여도가 낮아 상위 30%가 안되면 한개 줌
							for( int iCount = 0 ; iCount < kEventBossDropInfo.m_iEventBossBonusItemIDCount_Low ; ++iCount )
							{
								if( kEventBossDropInfo.m_iEventBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kEventBossDropInfo.m_iEventBossBonusItemID ); // 이벤트 보너스 아이템 큐브
							}
						}
					}
				}
				// 존재하지 않으면 당연 안줌
				//////////////////////////////////////////////////////////////////////////
			}
			else
#endif //SERV_BATTLEFIELD_EVENT_BOSS_INT
			if( SiCXSLBattleFieldManager()->GetBattleFieldMiddleBossDropInfo( static_cast<int>(GetBattleFieldID()), kDieNpcInfo.m_iNPCID, kMiddleBossDropInfo ) == true )
			{
				// 기여도가 8% 이상이고 , 파티 레벨차이가 10이 하라면 아이템 지급
				bool bGiveCommonBonusItem = ( mitUserContribution->second > 0.f && fLevelFactor > 0.f ) ? true : false;

				if( bGiveCommonBonusItem == true )
				{
#ifdef SERV_CAMERA_EVENT
					CTime tCurrentTime = CTime::GetCurrentTime();
					if( tCurrentTime < CTime(2013, 8, 21, 9, 0, 0)  )
					{
						if( mitUserList->second->IsEquippedItem( 60006500 ) == true || mitUserList->second->IsEquippedItem( 60006501 ) == true )
						{
							sDropDataNpc.m_vecItemID.push_back( 60006543 );
						}
					}
#endif SERV_CAMERA_EVENT

#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
					////bool isbCharMale = false;
					//bool isbCharQuest = false;
					////CXSLUnit::UNIT_TYPE eUnitType = (CXSLUnit::UNIT_TYPE)mitUserList->second->GetUnitType();

					//KRoomUserInfo kInfoChar;
					//mitUserList->second->GetRoomUserInfo(kInfoChar);

					//std::map< int, KSubQuestInfo >::const_iterator mitQ = kInfoChar.m_mapOngoingQuest.find(_CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerQuest);
					//if( mitQ != kInfoChar.m_mapOngoingQuest.end() )
					//{
					//	isbCharQuest = true;
					//}

					//// 현재 퀘스트 관련 정보와 캐릭터 성별 정보 비교에서 같을 경우에만 아이템 지급
					////if(_CONST_BURNING_CHAR_EVENT_SUB_QUEST_::bCharMale == isbCharMale && true == isbCharQuest)
					//if(true == isbCharQuest)
					//{
					//	sDropDataNpc.m_vecItemID.push_back( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerItem );

					//	START_LOG( clog, L"김석근_캐릭터 버닝이벤트_필드보스 처치 아이템지급" )
					//		<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerItem )
					//		<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestFieldPlayerQuest )
					//		<< END_LOG;
					//}
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST

					sDropDataNpc.m_vecItemID.push_back( 70070 ); // HP 100% 즉시회복 음식
					sDropDataNpc.m_vecItemID.push_back( 70050 ); // MP 150 즉시회복 음식

					// 중간 보스의 혼
					for( int iBonusCount = 0 ; iBonusCount < kMiddleBossDropInfo.m_iMiddleBossCommonBonusItemCount ; ++iBonusCount )
					{
						if( kMiddleBossDropInfo.m_iMiddleBossCommonBonusItemID <= 0 )
							break;

						sDropDataNpc.m_vecItemID.push_back( kMiddleBossDropInfo.m_iMiddleBossCommonBonusItemID );
					}

					// 필드 중간 보스 보너스 아이템 - 필드 중간 보스 큐브
					std::map< UidType, bool >::iterator mitBonus = mapBonusItem.find( mitUserList->second->GetCID() );
					if( mitBonus != mapBonusItem.end() )
					{
						if( mitBonus->second == true )				
						{
							// 존재하지만 기여도가 상위 30%가 되면 두개 줌
							for( int iCount = 0 ; iCount < kMiddleBossDropInfo.m_iMiddleBossBonusItemIDCount_High ; ++iCount )
							{
								if( kMiddleBossDropInfo.m_iMiddleBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kMiddleBossDropInfo.m_iMiddleBossBonusItemID ); // 필드 중간 보스 큐브
							}
						}
						else
						{
							// 존재하지만 기여도가 낮아 상위 30%가 안되면 한개 줌
							for( int iCount = 0 ; iCount < kMiddleBossDropInfo.m_iMiddleBossBonusItemIDCount_Low ; ++iCount )
							{
								if( kMiddleBossDropInfo.m_iMiddleBossBonusItemID <= 0 )
									break;

								sDropDataNpc.m_vecItemID.push_back( kMiddleBossDropInfo.m_iMiddleBossBonusItemID ); // 필드 중간 보스 큐브
							}
						}
					}
				}
				// 존재하지 않으면 당연 안줌
				//////////////////////////////////////////////////////////////////////////
			}
		
			//////////////////////////////////////////////////////////////////////////
			// 경험치를 주는 경우엔 내구도 소모를 시킨다.

			//////////////////////////////////////////////////////////////////////////		
			// 필드의 유저에게 경험치 지급(보너스 포함)
			m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

			// 한번만 올려주어야 한다.
			if( mitUserList->second->GetCID() == iSendUnitUID )
			{
				IncreasePartyFever( kPacket_.m_mapDamageByUser ); // 해당 몬스터를 잡은 유저들의 공헌도에 따른 파티 피버 증가!
			}
			//////////////////////////////////////////////////////////////////////////	
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

			// 드롭을 하지 않는 npc라면 드롭을 하지 않는다.
			if( kDieNpcInfo.m_bNoDrop == true )
				continue;

			// 드롭상태가 아닌채로 죽은npc라면 아이템 드롭을 하지 않는다.
			if( bDropDieState == false )
				continue;

			// 개인 기여도 * 파티 보너스
			//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
			float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
			float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
			//}

			//////////////////////////////////////////////////////////////////////////
			// 2. 던전을 기준으로 드롭 아이템이 결정되는 던전 드롭
			KDropTable::DROP_DATA sDropDataStatic;	
			{
				if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
				{
					START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
						<< BUILD_LOG( GetBattleFieldID() )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 4. 속성 몬스터 드롭 아이템
			KDropTable::DROP_DATA sDropDataAttribNpc;

			if( bIsAttribNpc == true )
			{
				//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_fItemDropEventProbRate, sDropDataAttribNpc.m_vecItemID ) == false )
#else // SERV_DROP_EVENT_RENEWAL
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
#endif // SERV_DROP_EVENT_RENEWAL
				{
					START_LOG( cerr, L"속성 몬스터 아이템 드롭 실패!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}

				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;
				

				if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"속성 몬스터 스페셜 아이템 드롭 실패!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}
			}	


			//////////////////////////////////////////////////////////////////////////
			// 5. 퀘스트 드롭 아이템 처리
			{
				std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

				//유닛별 퀘스트 드롭 아이템이 있는지 검사..
				//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
				{
					//있다면 유닛별로 루푸를 돌면서..
					BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
					{
						//드롭된 아이템 수만큼 처리를 시작..
						BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
						{
							KEGS_GET_ITEM_NOT kDropNot;
							kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
							kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
							kDropNot.m_iItemID		= iDropQuestItemID;
							//{{ 2009. 7. 9  최육사		임시 인벤토리
							kDropNot.m_iDropItemUID = m_nDropItemUID++;
							//}}
							SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
						}
					}
				}
				else
				{
					if( NetError::GetLastError() != NetError::NET_OK )
					{
						START_LOG( cerr, L"퀘스트 드롭 아이템 받아오기 실패.!" )
							<< BUILD_LOG( kPacket_.m_uiAttUnit )
							<< BUILD_LOG( m_iDungeonID )
							<< BUILD_LOG( NetError::GetLastErrMsg() )
							<< END_LOG;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 위에서 결정된 드롭 아이템 리스트를 실제 유저에게 보내기 위한 처리
			KEGS_DROP_ITEM_NOT kDropItemNot;
			kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

			if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
			{
				//{{ 2012. 11. 20	최육사		휴면 복귀 유저 버그 수정 ( 배틀필드 ) 
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
				{
					// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// 새로운 모험의 징표
				}	
			}

			//1.ED 드롭처리
			if( sDropDataNpc.m_iED != -1 )
			{
				for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
				}
			}

			//2.NPC ITEM 드롭처리	
			if( sDropDataNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//{{ 2009. 4. 22  최육사	전체드롭
			if( sDropDataStatic.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}
			//}}

			//4.ATTRIB NPC ITEM 드롭처리
			if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//드롭처리한 아이템이 있으면 패킷을 보낸다.
			if( kDropItemNot.m_DropItemDataList.empty() == false )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
                vecDropItem_ = kDropItemNot.m_DropItemDataList;
			}
		}

		//{{ 2012. 12. 26  특정 시간 몬스터 사냥 시 아이템 드롭 이벤트(필드적용) - 김민성
		//////////////////////////////////////////////////////////////////////////
		// 특정 시각 아이템 드롭 이벤트
		if( kDieNpcInfo.m_bActive == true )
		{
			//////////////////////////////////////////////////////////////////////////
			// 특정 시간에 NPC때려잡으면 보상주는 기능
			if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
				if( spRoomUser != NULL )
				{
					int iTimeDropItemID = 0;
					int iTimeDropID = 0;
					if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
					{
						const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
						if( pItemTemplet != NULL )
						{
							// 일단 DB로 가서 이 유저가 보상 받아도 되는 유저인지 확인하자!
							KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
							kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
							kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
							kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
							kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
							kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
							kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
							kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
							kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
							SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
						}
						else
						{
							START_LOG( cerr, L"존재하지 않는 itemid이다. 세팅오류인가?" )
								<< BUILD_LOG( iTimeDropItemID )
								<< END_LOG;
						}
					}
				}
				else
				{
					START_LOG( cwarn, L"이벤트 아이템 획득 처리 하려는데 유저정보가 없다!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
}

void KBattleFieldRoom::BattleFieldNormalDropProcess( const IN KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID )
{
	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet 읽기 오류.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// 속성 몬스터인지 체크
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );

	//{{ 2009. 5. 11  최육사	npc죽은상태
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );
	//}}

	// 몬스터 타입 상수 얻기
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// 필드 상수 얻기
	//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit값이 -1이면 유저의 공격을 받지 않고 npc가 자살한것임. [예:지뢰함정]

	// 플레이어가 몬스터를 죽인거라면 해당 플레이어의 킬몬스터수 체크
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"몹끼리 죽였네!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// 몬스터가 함정에 걸려 죽는경우임 : 아이템은 그대로 드롭처리함.
			}
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	// 1. 몬스터 종류에 따라 드롭 아이템이 결정되는 몬스터 드롭
	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// 각 유저마다 다른 내용의 패킷을 보내야 하므로 이렇게 처리한다.
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
		kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
		kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
		kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
		kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
		kNpcDietNot.m_cUserCount		 = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete	 = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능

		// 분할 지급을 위해 선 계산 되어야 할 내용
		// 1. 기여도 (X로 표기)
		std::map< UidType, float >	mapUserContribution;
		int iNpcMaxHP = kPacket_.m_iNpcMaxHP;
		std::map< UidType, float >::const_iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin();
		for( ; mitDamageByUser != kPacket_.m_mapDamageByUser.end() ; ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;
			const int iPartyMemberCount = m_spUserManager->GetSamePartyMember( mitDamageByUser->first );

			float fContribution 
				= m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) 
				/ (static_cast<float>(iNpcMaxHP));

			if( fContribution > 1.5f )
				fContribution = 1.5f;

			 fContribution /= iPartyMemberCount;

			// 솔로 플레이면 파티가 없으니 목록이 없을 수도 있다. 그래서 한번 해줘야 한다.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// 파티 원도 동일한 기여도로 셋팅한다.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}

		// 2. 몬스터의 드롭 정보 얻기
		KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
		if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"배틀 필드 npc drop info 얻어오기 실패" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;

			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		// 3. 기여도가 있는 유저별로 EXP, ED, DropItem 계산
		std::map< UidType, KRoomUserPtr > mapRoomUserList;
		if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
		{
			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			START_LOG( cerr, L"보내려는  리스트를 찾을 수가 없다." )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// 필드 전원에게 해당 npc가 죽은것에 대한 정보를 브로드 캐스팅 한다! ( 일부유저 드롭, 경험치 제외 )
			BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
			return;
		}

		std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
		for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
		{
			KDropTable::KBattleFieldNpcDropInfo kDropInfo;
			kDropInfo = kDropInfo_Init;

			if( mitUserList->second == NULL )
				continue;

			//////////////////////////////////////////////////////////////////////////
			// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
			//방안의 인원에게 알려줄 데이터 셋팅 시작..
			kNpcDietNot.m_cUserCount = 0;
			kNpcDietNot.m_bDecreaseEndurance = false;
			kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
			kNpcDietNot.m_EXPList.clear();
			//////////////////////////////////////////////////////////////////////////

			if( mitUserList->second->IsBattleFieldNpcLoad() == false )
				continue;

			// 해당 유저가 몬스터를 죽일때 얼만큼 공헌을 했는지 검사하자!
			std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
			if( mitUserContribution == mapUserContribution.end() )
			{
				// 공헌도가 없으면 보상 받을 자격이 없다!
				SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
				continue;
			}

			KDropTable::DROP_DATA sDropDataNpc;

			// 현재 같은 필드에 있는 파티원 수
			kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

			// 레벨 상수값 얻기
			const float fLevelFactor = CXSLDungeon::GetMonsterLevelBalanceRate( m_spUserManager->GetUserLevel( mitUserList->first ), kDieNpcInfo.m_cLevel );

			// 파티 플레이일 경우 아이템 확률 증가
			const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

			// 파티의 기여도가 50% 이상인가?
			if( mitUserContribution->second * kNpcDietNot.m_cUserCount >= 0.5f )
			{
				kNpcDietNot.m_bQuestComplete = true;
			}

			// 개별적으로 계산/지급 되어야 하는 품목
			// 1. ED
			// 2. EXP
			// 3. 아이템
			if( kDropInfo.m_iExp > 0 )
			{
				kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ));

				// 경험치를 주는 몬스터라면 내구도가 감소 된다.
				kNpcDietNot.m_bDecreaseEndurance = true;
			}

			if( kDropInfo.m_iED > 0 )
			{
				kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel ));
			}

			if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo
				//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
													  , fFieldEDFactor
													  , fFieldEXPFactor
#else
													  , fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
													  //}
													  , fLevelFactor
													  , mitUserContribution->second
													  , kNpcDietNot.m_cUserCount
													  , fPartyDropBonus
													  //{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
													  , m_fItemDropEventProbRate
#else // SERV_DROP_EVENT_RENEWAL
													  , m_iItemDropEventProbCount
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
													  //}}
													  , kDieNpcInfo.m_bIsBoss
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-14	// 박세훈
													  , 1.0f
#endif // SERV_BATTLE_FIELD_BOSS
													  , sDropDataNpc ) == false )
			{
				START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< END_LOG;
			}

			// 공용으로 계산/지급 되어야 하는 품목
			// 1. 특수 아이템 드롭(ex. 마나히터)
			// 경험치를 주는 경우엔 내구도 소모를 시킨다.
			SiKDropTable()->NpcSpecialDropItem( kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cMonsterGrade, kPacket_.m_cNpcDieState, sDropDataNpc.m_vecItemID );

			//////////////////////////////////////////////////////////////////////////		
			// 필드의 유저에게 경험치 지급(보너스 포함)
			m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

			// 한번만 올려주어야 한다.
			if( mitUserList->second->GetCID() == iSendUnitUID )
			{
				IncreasePartyFever( kPacket_.m_mapDamageByUser ); // 해당 몬스터를 잡은 유저들의 공헌도에 따른 파티 피버 증가!
			}
			//////////////////////////////////////////////////////////////////////////	
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

			// 드롭을 하지 않는 npc라면 드롭을 하지 않는다.
			if( kDieNpcInfo.m_bNoDrop == true )
				continue;

			// 드롭상태가 아닌채로 죽은npc라면 아이템 드롭을 하지 않는다.
			if( bDropDieState == false )
				continue;

			// 개인 기여도 * 파티 보너스
			//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
			float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
			float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
			//}

			//////////////////////////////////////////////////////////////////////////
			// 2. 던전을 기준으로 드롭 아이템이 결정되는 던전 드롭
			KDropTable::DROP_DATA sDropDataStatic;	
			{
				if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
				{
					START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
						<< BUILD_LOG( GetBattleFieldID() )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			// 4. 속성 몬스터 드롭 아이템
			KDropTable::DROP_DATA sDropDataAttribNpc;

			if( bIsAttribNpc == true )
			{
				//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_fItemDropEventProbRate, sDropDataAttribNpc.m_vecItemID ) == false )
#else // SERV_DROP_EVENT_RENEWAL
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
#endif // SERV_DROP_EVENT_RENEWAL
				{
					START_LOG( cerr, L"속성 몬스터 아이템 드롭 실패!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}

				sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;

				if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"속성 몬스터 스페셜 아이템 드롭 실패!" )
						<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
						<< END_LOG;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 5. 퀘스트 드롭 아이템 처리
			{
				std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

				//유닛별 퀘스트 드롭 아이템이 있는지 검사..
				//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
				if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
				{
					//있다면 유닛별로 루푸를 돌면서..
					BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
					{
						//드롭된 아이템 수만큼 처리를 시작..
						BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
						{
							KEGS_GET_ITEM_NOT kDropNot;
							kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
							kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
							kDropNot.m_iItemID		= iDropQuestItemID;
							//{{ 2009. 7. 9  최육사		임시 인벤토리
							kDropNot.m_iDropItemUID = m_nDropItemUID++;
							//}}
							SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
						}
					}
				}
				else
				{
					if( NetError::GetLastError() != NetError::NET_OK )
					{
						START_LOG( cerr, L"퀘스트 드롭 아이템 받아오기 실패.!" )
							<< BUILD_LOG( kPacket_.m_uiAttUnit )
							<< BUILD_LOG( m_iDungeonID )
							<< BUILD_LOG( NetError::GetLastErrMsg() )
							<< END_LOG;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 위에서 결정된 드롭 아이템 리스트를 실제 유저에게 보내기 위한 처리
			KEGS_DROP_ITEM_NOT kDropItemNot;
			kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

			if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
			{
				//{{ 2012. 11. 20	최육사		휴면 복귀 유저 버그 수정 ( 배틀필드 ) 
#ifdef SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
#else
				if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetNumMember() ) == true )
#endif SERV_COME_BACK_USER_REWARD_IN_BATTLE_FIELD_BUG_FIX
					//}}
				{
					// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// 새로운 모험의 징표
				}	
			}

			//1.ED 드롭처리
			if( sDropDataNpc.m_iED != -1 )
			{
				for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
				}
			}

			//2.NPC ITEM 드롭처리	
			if( sDropDataNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//{{ 2009. 4. 22  최육사	전체드롭
			if( sDropDataStatic.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}
			//}}

			//4.ATTRIB NPC ITEM 드롭처리
			if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
				}
			}

			//드롭처리한 아이템이 있으면 패킷을 보낸다.
			if( kDropItemNot.m_DropItemDataList.empty() == false )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
				if( spRoomUser == NULL )
					continue;

				if( spRoomUser->IsBattleFieldNpcLoad() == false )
					continue;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
			}
		}

		//{{ 2012. 12. 26  특정 시간 몬스터 사냥 시 아이템 드롭 이벤트(필드적용) - 김민성
#ifdef SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//////////////////////////////////////////////////////////////////////////
		// 특정 시각 아이템 드롭 이벤트
		if( kDieNpcInfo.m_bActive == true )
		{
			//////////////////////////////////////////////////////////////////////////
			// 특정 시간에 NPC때려잡으면 보상주는 기능
			if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
			{
				KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
				if( spRoomUser != NULL )
				{
					int iTimeDropItemID = 0;
					int iTimeDropID = 0;
					if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
					{
						const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
						if( pItemTemplet != NULL )
						{
							// 일단 DB로 가서 이 유저가 보상 받아도 되는 유저인지 확인하자!
							KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
							kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
							kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
							kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
							kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
							kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
							kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
							kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
							kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
							SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
						}
						else
						{
							START_LOG( cerr, L"존재하지 않는 itemid이다. 세팅오류인가?" )
								<< BUILD_LOG( iTimeDropItemID )
								<< END_LOG;
						}
					}
				}
				else
				{
					START_LOG( cwarn, L"이벤트 아이템 획득 처리 하려는데 유저정보가 없다!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< END_LOG;
				}
			}
			//////////////////////////////////////////////////////////////////////////
		}
#endif SERV_TIME_MONSTER_DROP_EVENT_IN_FIELD
		//}}
	}
}

_IMPL_ON_FUNC( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ, KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ )
{
	KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK kPacket;

	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_07;
		kPacket.m_iDanger = m_kGameManager.GetDangerousValue();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
		return;
	}

	if( kPacket_.m_iDanger <= 0 )
	{
		kPacket_.m_iDanger = 1;
	}

	if( m_kMonsterManager.IsRemainMiddleBoss() == false )
	{
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
		IncreaseDangerousValue( kPacket_.m_iDanger, LAST_SENDER_UID );
#else // SERV_BATTLE_FIELD_BOSS
		m_kGameManager.IncreaseDangerousValue( kPacket_.m_iDanger );
#endif // SERV_BATTLE_FIELD_BOSS
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iDanger = m_kGameManager.GetDangerousValue();

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ, KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KRoomUserPtr spUser = m_spUserManager->GetHostUser();
	if( spUser != NULL )
	{
		SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_ACK, kPacket_ );
	}
}

_IMPL_ON_FUNC( ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KRoomUserPtr spUser = m_spUserManager->GetUser( kPacket_.m_iUnitUID );
	if( spUser != NULL )
	{
		SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, kPacket_ );
	}
}
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
//}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( ERM_CALL_MY_LOVER_CHECK_ROOM_REQ )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 패킷이 도착함." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		KERM_CALL_MY_LOVER_CHECK_ROOM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_29;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_CHECK_ROOM_ACK, kPacket );
		return;
	}

	// 들어 올 수 있는지 인원 수체크
	if( m_spUserManager->GetNumMember() + 1 > m_spUserManager->GetMaxSlot() )
	{
		KERM_CALL_MY_LOVER_CHECK_ROOM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_38;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_CHECK_ROOM_ACK, kPacket );
		return;
	}
	
	// 입장 가능 하다고 한다면
	KERM_CALL_MY_LOVER_CHECK_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vPos = kPacket_.m_vPos;
	kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
	kPacket.m_iLoverUnitUID = kPacket_.m_iLoverUnitUID;
	kPacket.m_cCallType = KEGS_CALL_MY_LOVER_ROOM_NOT::CT_OTHER_CHANNEL;
	GetBattleFieldRoomInfo( kPacket.m_kBTInfo );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_CHECK_ROOM_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_REQ, KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ )
{
	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_35;
	kPacket.m_bSendAck = false;
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket, ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_ACK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_ACK, kPacket_ );
}

IMPL_ON_FUNC( ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ )
{
	KEGS_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT kPacket;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
	kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
	kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;

	if( GetStateID() != KRoomFSM::S_PLAY  &&  GetStateID() != KRoomFSM::S_WAIT )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// 제대로 찾아 왔는지 보자!
	if( GetBattleFieldID() != kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
	{
		START_LOG( cerr, L"다른 배틀필드에 입장했다! 헐 버그다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( kPacket_.m_kBattleFieldJoinInfo.m_iBattleFieldID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// 배틀필드에 입장 처리 한다!
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-28	// 박세훈
	if( EnterRoom( kPacket_.m_kRoomUserInfo, kPacket_.m_kBattleFieldJoinInfo, false ) == false )
#else // SERV_BATTLE_FIELD_BOSS
	if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, kPacket_.m_kBattleFieldJoinInfo, false ) == false )
#endif // SERV_BATTLE_FIELD_BOSS
	{
		START_LOG( cerr, L"방 입장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->SetReady( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	if( m_spUserManager->StartGame( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

#ifdef SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS// 작업날짜: 2013-06-10	// 박세훈
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_BUFF_INFO_NOT );
#endif // SERV_CHANGE_UPDATE_BUFF_INFO_PROCESS

	// 해당 유저는 바로 게임 시작 처리 하자!
	if( m_spUserManager->StartPlay( kPacket_.m_kRoomUserInfo.m_nUnitUID ) == false )
	{
		START_LOG( cerr, L"게임 시작 처리 실패!" )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_06;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );
		return;
	}

	// 방 상태 변화.
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
		StateTransition( KRoomFSM::I_TO_LOAD );
		StateTransition( KRoomFSM::I_TO_PLAY );
	}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    m_spUserManager->ZombieAlert_CheckStart( kPacket_.m_kRoomUserInfo.m_nUnitUID );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST


	// 입장한 유저에게 ack를 날린다.
	kPacket.m_iOK = NetError::NET_OK;
#ifndef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_iLastTouchIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_vPos = kPacket_.m_vPos;
	GetRoomInfo( kPacket.m_RoomInfo );
	GetRoomSlotInfo( kPacket.m_vecSlot );
	//{{ 핑 상태체크 호스트 변경 - 김민성
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
	//{{ 2012. 11. 21	최육사		난입자에게 캐쉬샵 유저 정보 보내기
	m_spUserManager->GetEnterCashShopUserList( kPacket.m_vecEnterCashShopUser );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_ACK, kPacket );


	// 방의 모든 유저에게 NOT를 날린다.
	KEGS_JOIN_BATTLE_FIELD_NOT kPacketNot;
	//080425.hoons.옵저버 상태별 유저정보를 셋팅해서 보내준다.
	m_spUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KBattleFieldUserManager::UT_GAME );
	kPacketNot.m_StartPosIndex = kPacket_.m_kBattleFieldJoinInfo.m_iStartPosIndex;

	BroadCast( ERM_JOIN_BATTLE_FIELD_NOT, kPacketNot );

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [주의!!!] 반드시 방리스트를 먼저 보낸 이후에 이 패킷을 보내야 한다! 유저의 복귀를 위해 패킷을 다시 쏜다!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ )
{
	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_36;
	kPacket.m_bSendAck = false;
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket, ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_ACK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_ACK, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-09	// 박세훈
_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_ACK );
}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
void KBattleFieldRoom::IncreaseDangerousValue( IN const int iIncreaseValue, IN const UidType iGSUID )
{
	int iDangerousValue = m_kGameManager.GetDangerousValue();
	
	iDangerousValue += iIncreaseValue * SiCXSLBattleFieldManager()->GetDangerousValueEventRate();

	if( ( 0 <= iDangerousValue ) && ( iDangerousValue < SiCXSLBattleFieldManager()->GetDangerousValueMax() ) )
	{
		m_kGameManager.UpdateDangerousValue( iDangerousValue );
	}
	else
	{
		m_kGameManager.ResetDangerousValue();

		// TotalDangerousValue에 영향을 주는 배틀 필드인가?
		if( SiCXSLFieldBossData()->DoesFieldAffectTotalDangerousValue( GetBattleFieldID() ) == true )
		{
			// GlobalServer로 TotalDangerousValue 올리러 가자
			KERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT kPacket;
			kPacket.m_iBattleFieldID		= static_cast<int>( GetBattleFieldID() );
			kPacket.m_byteIncreasedValue	= 1;
			SendToGSServer( iGSUID, ERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT, kPacket );
		}
	}
}

void KBattleFieldRoom::OnNpcUnitDie( IN const int iPlayerCount
								   , IN const bool bIsAttribNpc
								   , IN const char cDifficultyLevel
								   , IN const char cMonsterGrade
								   , IN const bool bIncreaseDanger
								   , IN const UidType iGSUID
								   )
{
	// 죽은 몬스터의 몬스터 타입 상수값을 구한다.
	const int iMonsterTypeFactor = CXSLUnitManager::GetMonsterTypeFactor( bIsAttribNpc, cDifficultyLevel, cMonsterGrade );

	// 위험도를 증가 시킨다.
	if( bIncreaseDanger == true )
	{
		// 변경전의 위험도 저장
		const int iBeforeDangeroursValue = m_kGameManager.GetDangerousValue();

		IncreaseDangerousValue( iMonsterTypeFactor, iGSUID );

		// 경고 메시지 이벤트 체크
		m_kGameManager.CheckReserveWarningEvent( iBeforeDangeroursValue );

		// 엘리트 몬스터 출현 이벤트 체크
		m_kGameManager.CheckReserveEliteMonsterDropEvent( iBeforeDangeroursValue );

		// 중간 보스 출현 이벤트 체크
		m_kGameManager.CheckReserveMiddleBossDropEvent( iPlayerCount );

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		m_kGameManager.CheckReserveEventBossDropEvent( iPlayerCount );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

		// 보스 출현 이벤트 체크
		m_kGameManager.CheckReserveBossDropEvent( iPlayerCount );
	}
}

void KBattleFieldRoom::CheckFieldBossSystem( void )
{
	const __time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS ) == true )
	{
		// 보스 필드 강제 이탈 체크
		if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_RETURN_TO_FIELD ) == false )
		{
			if( ( m_tFieldCloseTime + SiCXSLFieldBossData()->GetFieldBossEndGameFieldHoldingTime_S() ) <= tCurrentTime )
			{
				// 유저들이 원래 필드로 이탈하도록 유도하자
				SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_RETURN_TO_FIELD );
				BroadCastID( ERM_BOSS_FIELD_RETURN_PROCESS_NOT );
			}
		}
		// 보스 필드 강제 종료 체크
		else if( ( m_tFieldCloseTime + SiCXSLFieldBossData()->GetFieldBossEndGameFieldHoldingTimeForce_S() ) <= tCurrentTime )
		{
			std::map< UidType, UidType > mapUserList;
			m_spRoomUserManager->GetUnitUIDListNotPrepareForDefenceDungeon( mapUserList );
			HandleTimeOutUser( mapUserList );
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
	}
	else
	{
		// 남은 시간이 20초 미만이라면, 난입 불가로 설정한다.
		if( ( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION ) == false )
			&& ( ( m_tFieldCloseTime - SiCXSLFieldBossData()->GetIntrudeRestrictionTime_S() ) < tCurrentTime )
			)
		{
			SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION );
			SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
		}

		if( m_tFieldCloseTime <= tCurrentTime )
		{
			// 종료 프로세스 시작하도록 알림
			_BossFieldCloseProcess( true );
		}
	}
}

void KBattleFieldRoom::InitBossFieldCreateInfo( IN const KBossFieldCreateInfo& kBossFieldCreateInfo )
{
	// 보스 필드 로그 정보 초기화
	m_kBossFieldLog.clear();

	// 보스 필드 방 상태 정보 초기화
	m_byteBossFieldState = 0;

	if( kBossFieldCreateInfo.m_bBossField == false )
		return;

	// 보스 필드 방 정보 설정
	SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD );
	m_tFieldHoldingTime	= kBossFieldCreateInfo.m_tFieldHoldingTime;
	m_tFieldOpenTime	= CTime::GetCurrentTime().GetTime();
	m_tFieldCloseTime	= m_tFieldOpenTime + m_tFieldHoldingTime;

	// 로그 데이터 복사
	m_kBossFieldLog.m_kFieldLog.m_iBossFieldID	= kBossFieldCreateInfo.m_iBossFieldID;
	m_kBossFieldLog.m_kFieldLog.m_iPortalMapID	= kBossFieldCreateInfo.m_iPortalMapID;

	CTime tPortalOpenTime( kBossFieldCreateInfo.m_tPortalOpenTime );
	m_kBossFieldLog.m_kFieldLog.m_wstrPortalOpenTime = tPortalOpenTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
}

void KBattleFieldRoom::BattleFieldBossDropProcess( const IN KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID )
{
	const CXSLUnitManager::NPCUnitTemplet* pNPCTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( (CXSLUnitManager::NPC_UNIT_ID)kDieNpcInfo.m_iNPCID );
	if( pNPCTemplet == NULL )
	{
		START_LOG( cwarn, L"NPC Unit Templet 읽기 오류.!" )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;
		return;
	}

	// 속성 몬스터인지 체크
	const bool bIsAttribNpc = m_kMonsterManager.IsAttribNpc( kPacket_.m_nDieNPCUID );
	const int iAttibAcount = m_kMonsterManager.GetAttribNpcAttribCount( kPacket_.m_nDieNPCUID );

	// npc죽은상태
	const bool bDropDieState = IsDropDieState( kPacket_.m_cNpcDieState );
	const bool bAddRoomUserKillCount = ( bDropDieState  &&  kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF );

	// 몬스터 타입 상수 얻기
	const float fMonsterTypeFactor = static_cast<float>( CXSLUnitManager::GetBattleFieldMonsterTypeFactor( iAttibAcount, kDieNpcInfo.m_cMonsterGrade ) );

	// 필드 상수 얻기
	//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
	const float fFieldEDFactor = SiCXSLBattleFieldManager()->GetBattleFieldEDFactor() * fMonsterTypeFactor;
	const float fFieldEXPFactor = SiCXSLBattleFieldManager()->GetBattleFieldEXPFactor() * fMonsterTypeFactor;
#else
	const float fFieldFactor = SiCXSLBattleFieldManager()->GetBattleFieldFactor() * fMonsterTypeFactor;
#endif SERV_FIELD_ED_EXP_FACTOR
	//}

	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit값이 -1이면 유저의 공격을 받지 않고 npc가 자살한것임. [예:지뢰함정]
	// 플레이어가 몬스터를 죽인거라면 해당 플레이어의 킬몬스터수 체크
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"몹끼리 죽였네!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// 몬스터가 함정에 걸려 죽는경우임 : 아이템은 그대로 드롭처리함.
			}
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	// 1. 몬스터 종류에 따라 드롭 아이템이 결정되는 몬스터 드롭
	if( ( kDieNpcInfo.m_bNoDrop == true ) || ( bDropDieState == false ) )
		return;

	//////////////////////////////////////////////////////////////////////////
	// 각 유저마다 다른 내용의 패킷을 보내야 하므로 이렇게 처리한다.
	KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
	kNpcDietNot.m_iDungeonID		 = m_iDungeonID;
	kNpcDietNot.m_cDifficulty		 = m_cDifficultyLevel;
	kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
	kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
	kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
	kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
	kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
	kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
	kNpcDietNot.m_sSubStageExpNpcNum = m_kMonsterManager.GetAtStartedMonsterCount();
	kNpcDietNot.m_cUserCount		 = 0;
	kNpcDietNot.m_bDecreaseEndurance = false;
	kNpcDietNot.m_bQuestComplete	 = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능

	// 분할 지급을 위해 선 계산 되어야 할 내용
	// 1. 기여도 (X로 표기)
	std::map< UidType, float > mapUserContribution;
	{
		const float fLimiteContribution = SiCXSLFieldBossData()->GetLimiteContribution();
		const float fMaximumContribution = SiCXSLFieldBossData()->GetMaximumContribution();

		for( std::map< UidType, float >::const_iterator mitDamageByUser = kPacket_.m_mapDamageByUser.begin(); mitDamageByUser != kPacket_.m_mapDamageByUser.end(); ++mitDamageByUser )
		{
			std::vector<UidType> vecPartyUserList;
			const int iPartyMemberCount = m_spUserManager->GetSamePartyMember( mitDamageByUser->first );

			float fContribution 
				= m_spUserManager->GetPartyTotalDamage( mitDamageByUser->first, kPacket_.m_mapDamageByUser ,vecPartyUserList ) 
				/ (static_cast<float>(kPacket_.m_iNpcMaxHP)) / iPartyMemberCount;

			if( fMaximumContribution < fContribution )
				fContribution = fMaximumContribution;

			if( fContribution < fLimiteContribution )
				continue;

			// 솔로 플레이면 파티가 없으니 목록이 없을 수도 있다. 그래서 한번 해줘야 한다.
			mapUserContribution.insert( std::make_pair( mitDamageByUser->first, fContribution ) );

			// 파티 원도 동일한 기여도로 셋팅한다.
			BOOST_TEST_FOREACH( UidType, iPartyUnitUID, vecPartyUserList )
			{
				mapUserContribution.insert( std::make_pair( iPartyUnitUID, fContribution ) );
			}
		}
	}

	// 파티별 기여도 순위 및 유저 리스트
	std::multimap< float, std::set< UidType > > mmapPartyContribution;
	_MakePartyContributionData( mapUserContribution, mmapPartyContribution );

	// 2. 몬스터의 드롭 정보 얻기
	KDropTable::KBattleFieldNpcDropInfo kDropInfo_Init;
	if( SiKDropTable()->GetBattleFieldNpcDropInfo( GetBattleFieldID(), kDieNpcInfo.m_iNPCID, kDropInfo_Init ) == false )
	{
		//////////////////////////////////////////////////////////////////////////
		// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
		//방안의 인원에게 알려줄 데이터 셋팅 시작..
		kNpcDietNot.m_cUserCount = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
		kNpcDietNot.m_EXPList.clear();
		//////////////////////////////////////////////////////////////////////////

		START_LOG( cerr, L"배틀 필드 npc drop info 얻어오기 실패" )
			<< BUILD_LOG( GetBattleFieldID() )
			<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
			<< END_LOG;

		BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		return;
	}

	// 3. 기여도가 있는 유저별로 EXP, ED, DropItem 계산
	std::map< UidType, KRoomUserPtr > mapRoomUserList;
	if( m_spUserManager->GetRoomUserList( mapRoomUserList ) == false )
	{
		//////////////////////////////////////////////////////////////////////////
		// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
		//방안의 인원에게 알려줄 데이터 셋팅 시작..
		kNpcDietNot.m_cUserCount = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
		kNpcDietNot.m_EXPList.clear();
		//////////////////////////////////////////////////////////////////////////

		START_LOG( cerr, L"보내려는  리스트를 찾을 수가 없다." )
			<< END_LOG;

		//////////////////////////////////////////////////////////////////////////
		// 필드 전원에게 해당 npc가 죽은것에 대한 정보를 브로드 캐스팅 한다! ( 일부유저 드롭, 경험치 제외 )
		BroadCast( ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot, KRoomUserManager::UL_NPC_LOAD_USER );
		return;
	}

	std::map< UidType, KRoomUserPtr >::iterator mitUserList = mapRoomUserList.begin();
	for(  ; mitUserList != mapRoomUserList.end() ; ++mitUserList )
	{
		KDropTable::KBattleFieldNpcDropInfo kDropInfo;
		kDropInfo = kDropInfo_Init;

		if( mitUserList->second == NULL )
			continue;

		//////////////////////////////////////////////////////////////////////////
		// 유저 별로 정리 하기 때문에 데이터 초기화가 필요하다.
		//방안의 인원에게 알려줄 데이터 셋팅 시작..
		kNpcDietNot.m_cUserCount = 0;
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_bQuestComplete = false;			// 파티원 포함하여 준 데미지가 npc hp 의 50% 이상이면 퀘스트 완료 가능
		kNpcDietNot.m_EXPList.clear();
		//////////////////////////////////////////////////////////////////////////

		if( mitUserList->second->IsBattleFieldNpcLoad() == false )
			continue;

		// 해당 유저가 몬스터를 죽일때 얼만큼 공헌을 했는지 검사하자!
		std::map< UidType, float >::iterator mitUserContribution = mapUserContribution.find( mitUserList->first );
		if( mitUserContribution == mapUserContribution.end() )
		{
			// 공헌도가 없으면 보상 받을 자격이 없다!
			SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );
			continue;
		}

		KDropTable::DROP_DATA sDropDataNpc;

		// 현재 같은 필드에 있는 파티원 수
		kNpcDietNot.m_cUserCount	= static_cast<char>( m_spUserManager->GetSamePartyMember( mitUserList->first ) );

		const float fLevelFactor = 1.0f;

		// 파티 플레이일 경우 아이템 확률 증가
		const float fPartyDropBonus = m_spUserManager->GetBattleFieldPartyBonusRate( mitUserList->first ) + 1.0f;	

		// 파티의 기여도가 50% 이상인가?
		if( mitUserContribution->second * kNpcDietNot.m_cUserCount >= 0.002f )
		{
			kNpcDietNot.m_bQuestComplete = true;
		}

		// 개별적으로 계산/지급 되어야 하는 품목
		// 1. ED
		// 2. EXP
		// 3. 아이템
		if( kDropInfo.m_iExp > 0 )
		{
			kDropInfo.m_iExp = static_cast<int>(SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ));

			// 경험치를 주는 몬스터라면 내구도가 감소 된다.
			kNpcDietNot.m_bDecreaseEndurance = true;
		}

		if( kDropInfo.m_iED > 0 )
		{
			kDropInfo.m_iED = (10 + ( kDropInfo.m_iED * kDieNpcInfo.m_cLevel ));
			// 해당 필드몹 평균 드롭 ED * 100
			kDropInfo.m_iED = kDropInfo.m_iED * 20;
		}

		// 배틀 필드 보스 상수 작업 요청자: 이승만
		const float fBattleFieldBossFactor = 16.0f * powf( 1.1f, static_cast<float>( mitUserList->second->GetLevel() - 49 ) );

		if( SiKDropTable()->BattleFieldNpcDropItem( kDropInfo
			//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
												  , fFieldEDFactor
												  , fFieldEXPFactor
#else
												  , fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
			//}
												  , fLevelFactor
												  , mitUserContribution->second
												  , kNpcDietNot.m_cUserCount
												  , fPartyDropBonus
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
												  , m_fItemDropEventProbRate
#else // SERV_DROP_EVENT_RENEWAL
												  , m_iItemDropEventProbCount
#endif // SERV_DROP_EVENT_RENEWAL
												  , kDieNpcInfo.m_bIsBoss
												  , fBattleFieldBossFactor
												  , sDropDataNpc
												  ) == false )
		{
			START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
				<< BUILD_LOG( GetBattleFieldID() )
				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
				<< END_LOG;
		}

		//////////////////////////////////////////////////////////////////////////
		// 필드 중간 보스 보너스 아이템 지급 처리
		_BossRewardProcess( mitUserList->first, mmapPartyContribution, sDropDataNpc.m_vecItemID );

		//////////////////////////////////////////////////////////////////////////
		// 경험치를 주는 경우엔 내구도 소모를 시킨다.

		//////////////////////////////////////////////////////////////////////////		
		// 필드의 유저에게 경험치 지급(보너스 포함)
		m_spUserManager->AddBattleFieldRewardEXP( mitUserList->first, fLevelFactor, sDropDataNpc.m_iExp, kNpcDietNot );

		// 한번만 올려주어야 한다.
		if( mitUserList->second->GetCID() == iSendUnitUID )
		{
			IncreasePartyFever( kPacket_.m_mapDamageByUser ); // 해당 몬스터를 잡은 유저들의 공헌도에 따른 파티 피버 증가!
		}
		//////////////////////////////////////////////////////////////////////////	

#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// 작업날짜: 2013-11-28	// 박세훈
		if( kNpcDietNot.m_bQuestComplete == false )
		{
			START_LOG( cerr, L"필드 보스 시스템 퀘스트 로그: 이 루틴에서 퀘스트 완료는 절대적이어야 한다." )
				<< BUILD_LOG( mitUserList->second->GetCID() )
				<< BUILD_LOG( mitUserContribution->second )
				<< END_LOG;
		}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG

		SendToGSCharacter( mitUserList->second->GetGSUID(), mitUserList->second->GetCID(), ERM_BATTLE_FIELD_NPC_UNIT_DIE_NOT, kNpcDietNot );

		// 드롭을 하지 않는 npc라면 드롭을 하지 않는다.
		if( kDieNpcInfo.m_bNoDrop == true )
			continue;

		// 드롭상태가 아닌채로 죽은npc라면 아이템 드롭을 하지 않는다.
		if( bDropDieState == false )
			continue;

		// 개인 기여도 * 파티 보너스
		//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
		float fTotalDropRate = mitUserContribution->second * fFieldEXPFactor;// * fPartyDropBonus ;
#else
		float fTotalDropRate = mitUserContribution->second * fFieldFactor;// * fPartyDropBonus ;
#endif SERV_FIELD_ED_EXP_FACTOR
		//}

		//////////////////////////////////////////////////////////////////////////
		// 2. 던전을 기준으로 드롭 아이템이 결정되는 던전 드롭
		KDropTable::DROP_DATA sDropDataStatic;	
		{
			if( SiKDropTable()->BattleFieldStaticDropItem( GetBattleFieldID(), sDropDataStatic, fTotalDropRate ) == false )
			{
				START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
					<< BUILD_LOG( GetBattleFieldID() )
					<< END_LOG;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// 4. 속성 몬스터 드롭 아이템
		KDropTable::DROP_DATA sDropDataAttribNpc;

		if( bIsAttribNpc == true )
		{
			//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
			if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_fItemDropEventProbRate, sDropDataAttribNpc.m_vecItemID ) == false )
#else // SERV_DROP_EVENT_RENEWAL
			if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(CXSLDungeon::DL_EXPERT), static_cast<int>(kDieNpcInfo.m_cLevel), fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
#endif // SERV_DROP_EVENT_RENEWAL
			{
				START_LOG( cerr, L"속성 몬스터 아이템 드롭 실패!" )
					<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
					<< END_LOG;
			}

			sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;


			if( SiKDropTable()->AttribNpcSpecialDrop( CXSLDungeon::DL_EXPERT, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
			{
				START_LOG( cerr, L"속성 몬스터 스페셜 아이템 드롭 실패!" )
					<< BUILD_LOG( CXSLDungeon::DL_EXPERT )
					<< END_LOG;
			}
		}	


		//////////////////////////////////////////////////////////////////////////
		// 5. 퀘스트 드롭 아이템 처리
		{
			std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

			//유닛별 퀘스트 드롭 아이템이 있는지 검사..
			//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
			if( m_spUserManager->GetQuestDropItem( mitUserList->second->GetCID(), GetBattleFieldID(), kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
				//}
			{
				//있다면 유닛별로 루푸를 돌면서..
				BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
				{
					//드롭된 아이템 수만큼 처리를 시작..
					BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
					{
						KEGS_GET_ITEM_NOT kDropNot;
						kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
						kDropNot.m_GetUnitUID	= kDropInfo.m_iUnitUID;
						kDropNot.m_iItemID		= iDropQuestItemID;
						//{{ 2009. 7. 9  최육사		임시 인벤토리
						kDropNot.m_iDropItemUID = m_nDropItemUID++;
						//}}
						SendToGSCharacter( kDropInfo.m_iGSUID, kDropInfo.m_iUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
					}
				}
			}
			else
			{
				if( NetError::GetLastError() != NetError::NET_OK )
				{
					START_LOG( cerr, L"퀘스트 드롭 아이템 받아오기 실패.!" )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< BUILD_LOG( m_iDungeonID )
						<< BUILD_LOG( NetError::GetLastErrMsg() )
						<< END_LOG;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 위에서 결정된 드롭 아이템 리스트를 실제 유저에게 보내기 위한 처리
		KEGS_DROP_ITEM_NOT kDropItemNot;
		kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

		if( mitUserList->second->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
		{
			//{{ 2012. 11. 20	최육사		휴면 복귀 유저 버그 수정 ( 배틀필드 ) 
			if( SiKDropTable()->GetComeBackBuffDropCount( m_spUserManager->GetSamePartyMember( mitUserList->second->GetCID() ) ) == true )
			{
				// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );		// 새로운 모험의 징표
			}	
		}

		//1.ED 드롭처리
		if( sDropDataNpc.m_iED != -1 )
		{
			for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution, true, sDropDataNpc.m_iED ) );
			}
		}

		//2.NPC ITEM 드롭처리	
		if( sDropDataNpc.m_vecItemID.empty() == false )
		{
			BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
			}
		}

		//{{ 2009. 4. 22  최육사	전체드롭
		if( sDropDataStatic.m_vecItemID.empty() == false )
		{
			BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
			}
		}
		//}}

		//4.ATTRIB NPC ITEM 드롭처리
		if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
		{
			BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
			{
				kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID, kDieNpcInfo.m_cLevel, kPacket_.m_iNpcMaxHP, mapUserContribution ) );
			}
		}

		//드롭처리한 아이템이 있으면 패킷을 보낸다.
		if( kDropItemNot.m_DropItemDataList.empty() == false )
		{
			KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mitUserContribution->first );
			if( spRoomUser == NULL )
				continue;

			if( spRoomUser->IsBattleFieldNpcLoad() == false )
				continue;

			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
		}
	}

	////{{ 2012. 12. 26  특정 시간 몬스터 사냥 시 아이템 드롭 이벤트(필드적용) - 김민성
	////////////////////////////////////////////////////////////////////////////
	//// 특정 시각 아이템 드롭 이벤트
	//if( kDieNpcInfo.m_bActive == true )
	//{
	//	//////////////////////////////////////////////////////////////////////////
	//	// 특정 시간에 NPC때려잡으면 보상주는 기능
	//	if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
	//	{
	//		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( kPacket_.m_uiAttUnit );
	//		if( spRoomUser != NULL )
	//		{
	//			int iTimeDropItemID = 0;
	//			int iTimeDropID = 0;
	//			if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
	//			{
	//				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
	//				if( pItemTemplet != NULL )
	//				{
	//					// 일단 DB로 가서 이 유저가 보상 받아도 되는 유저인지 확인하자!
	//					KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
	//					kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
	//					kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
	//					kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
	//					kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
	//					kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
	//					kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
	//					kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
	//					kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
	//					SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
	//				}
	//				else
	//				{
	//					START_LOG( cerr, L"존재하지 않는 itemid이다. 세팅오류인가?" )
	//						<< BUILD_LOG( iTimeDropItemID )
	//						<< END_LOG;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			START_LOG( cwarn, L"이벤트 아이템 획득 처리 하려는데 유저정보가 없다!" )
	//				<< BUILD_LOG( kPacket_.m_uiAttUnit )
	//				<< END_LOG;
	//		}
	//	}
	//	//////////////////////////////////////////////////////////////////////////
	//}
}

void KBattleFieldRoom::_BossRewardProcess( IN const UidType iUnitUID, IN const std::multimap< float, std::set< UidType > > mmapPartyContribution, OUT std::vector<int>& vecItemID ) const
{
	if( mmapPartyContribution.empty() == true )
		return;

	size_t iRank = 1;
	std::multimap< float, std::set< UidType > >::const_reverse_iterator it;
	for( it = mmapPartyContribution.rbegin(); it != mmapPartyContribution.rend(); ++it )
	{
		const std::set<UidType>& setUnitList = it->second;
		
		if( setUnitList.find( iUnitUID ) != setUnitList.end() )
		{
			break;
		}

		++iRank;
	}

	if( it == mmapPartyContribution.rend() )
	{
		return;
	}

	std::map<int, byte> mapRewardData;
	SiCXSLFieldBossData()->GetPrecedenceRewardData( iRank, mmapPartyContribution.size(), mapRewardData );

	for( std::map<int, byte>::const_iterator it = mapRewardData.begin(); it != mapRewardData.end(); ++it )
	{
		int iItemID = it->first;
		byte byteNum = it->second;
		
		while( 0 < byteNum )
		{
			vecItemID.push_back( iItemID );
			--byteNum;
		}
	}

	if( 0 <= iRank )
	{
		// 로그 관련 정보라서 const_cast를 사용함. 이 함수의 상수성을 제거하거나 외부에서 처리해도 문제 없다.
		const_cast<KBattleFieldRoom*>( this )->_MakeBossFieldUserLog( iUnitUID, static_cast<byte>( min( iRank, BYTE_MAX ) ) );
	}
}

int KBattleFieldRoom::EventMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// 몬스터가 현재 살아있는지 체크!
	if( m_kMonsterManager.IsEventMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
	if( m_kMonsterManager.GetEventNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// 아직 살아있다면 죽인사람과 죽은것으로 설정..
	LIF( m_kMonsterManager.SetEventMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, iUnitUID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	return NetError::NET_OK;
}

int KBattleFieldRoom::MiddleBossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// 몬스터가 현재 살아있는지 체크!
	if( m_kMonsterManager.IsMiddleBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
	if( m_kMonsterManager.GetMiddleBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// 아직 살아있다면 죽인사람과 죽은것으로 설정..
	LIF( m_kMonsterManager.SetMiddleBossMonsterDie( kPacket_.m_nDieNPCUID ) );

	std::vector< KDropItemData > vecDropItem;
	vecDropItem.clear();
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, iUnitUID, vecDropItem );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	KFieldBossGenKillStat kLog;
	//kLog.m_nDieNPCUID = kPacket_.m_nDieNPCUID;
	kLog.m_nNumUser = kPacket_.m_mapDamageByUser.size();
	//kLog.m_nNumDropItem = vecDropItem.size();
	kLog.m_nSpawnID = kDieNpcInfo.m_iBossGroupID;
	kLog.m_nFieldID = GetBattleFieldID();
	SendToLogDB( ELOG_KILL_FIELD_MIDDLE_BOSS_STAT_NOT, kLog );

	START_LOG( cwarn, L"필드 중간 보스 킬 통계 테스트 " )
		<< BUILD_LOG( kLog.m_nFieldID )
		//<< BUILD_LOG( kLog.m_nDieNPCUID )
		<< BUILD_LOG( kLog.m_nNumUser )
		<< BUILD_LOG( kLog.m_nSpawnID )
		<< END_LOG;

	return NetError::NET_OK;
}

#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
int KBattleFieldRoom::EventBossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// 몬스터가 현재 살아있는지 체크!
	if( m_kMonsterManager.IsEventBossMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
	if( m_kMonsterManager.GetEventBossNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// 아직 살아있다면 죽인사람과 죽은것으로 설정..
	LIF( m_kMonsterManager.SetEventBossMonsterDie( kPacket_.m_nDieNPCUID ) );

	std::vector< KDropItemData > vecDropItem;
	vecDropItem.clear();
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldMiddleBossDropProcess( kPacket_, kDieNpcInfo, iUnitUID, vecDropItem );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	return NetError::NET_OK;
}
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

int KBattleFieldRoom::MonsterDiePrcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade )
{
	// 몬스터가 현재 살아있는지 체크!
	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	// 아직 살아있다면 죽인사람과 죽은것으로 설정..
	LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	BattleFieldNormalDropProcess( kPacket_, kDieNpcInfo, iUnitUID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	cMonsterGrade		= kDieNpcInfo.m_cMonsterGrade;

	return NetError::NET_OK;
}

int KBattleFieldRoom::BossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket )
{
	// 몬스터가 현재 살아있는지 체크!
	if( m_kMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		return NetError::ERR_ROOM_21;
	}

	KRoomMonsterManager::NPC_DATA kDieNpcInfo;

	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
	if( m_kMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		return NetError::ERR_ROOM_20;
	}

	if( m_kMonsterManager.IsAliveBossMonster( kPacket_.m_nDieNPCUID ) == true )
	{
		if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS ) == true )
		{
			return NetError::ERR_FIELD_BOSS_07;
		}

		// 아직 살아있다면 죽인사람과 죽은것으로 설정..
		LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

		BattleFieldBossDropProcess( kPacket_, kDieNpcInfo, iUnitUID );

		// 보스 필드 게임 끝 체크
		_BossFieldCloseProcess( false );
	}
	else
	{
		// 아직 살아있다면 죽인사람과 죽은것으로 설정..
		LIF( m_kMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );
	}

	kPacket.m_iNPCID = kDieNpcInfo.m_iNPCID;

	return NetError::NET_OK;
}

void KBattleFieldRoom::_MakePartyContributionData( IN const std::map<UidType, float>& mapUserContribution, OUT std::multimap< float, std::set< UidType > >& mmapPartyContribution ) const
{
	std::vector< std::set< UidType > > vecUnitListGroupdByParty;
	m_spUserManager->GetUnitListGroupdByParty( vecUnitListGroupdByParty );

	BOOST_TEST_FOREACH( const std::set<UidType>&, setUnitList, vecUnitListGroupdByParty )
	{
		if( setUnitList.empty() == true )
			continue;
			
		const UidType iUnitUID = *setUnitList.begin();

		std::map<UidType, float>::const_iterator it = mapUserContribution.find( iUnitUID );
		if( it == mapUserContribution.end() )
			continue;

		const float fContribution = it->second;
		mmapPartyContribution.insert( std::multimap< float, std::set< UidType > >::value_type( fContribution, setUnitList ) );
	}
}

void KBattleFieldRoom::_BossFieldCloseProcess( const bool bTimeOut )
{
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION ) == false )
	{
		// 난입 불가로 설정한다.
		SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION );
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}

	__time64_t tCurrentTime = CTime::GetCurrentTime().GetTime();

	// m_tFieldCloseTime 을 수정하여 이후 프로세스가 정상적인 시간으로 처리되도록 하자.
	if( bTimeOut == false )
	{
		m_tFieldCloseTime = tCurrentTime;
	}

	// 로그 정보 수집
#ifdef _CONVERT_VS_2010// 작업날짜: 2013-11-29	// 박세훈
	m_kBossFieldLog.m_kFieldLog.m_iPlayTime = static_cast<int>( min( ( tCurrentTime - m_tFieldOpenTime ), INT_MAX ) );
#else // _CONVERT_VS_2010
	m_kBossFieldLog.m_kFieldLog.m_iPlayTime = static_cast<int>( min( ( tCurrentTime - m_tFieldOpenTime ), 2147483647 ) );
#endif // _CONVERT_VS_2010
	m_kBossFieldLog.m_kFieldLog.m_byteRemainUserCount = m_spUserManager->GetNumMember();
	m_kBossFieldLog.m_kFieldLog.m_bSucceed = !bTimeOut;

	// 종료 프로세스 시작하도록 알림
	SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS );
	BroadCastID( ERM_BOSS_FIELD_CLOSE_PROCESS_NOT );
}

bool KBattleFieldRoom::EnterRoom( IN const KRoomUserInfo& kInfo, IN const KBattleFieldJoinInfo& kJoinInfo, IN const bool bConsiderTeam )
{
	if( m_spUserManager->EnterRoom( kInfo, kJoinInfo, bConsiderTeam ) == false )
	{
		return false;
	}

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == true )
	{
		// 보스 필드 유저 로그 정보 수집
		KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.find( kInfo.m_nUnitUID );
		if( it == m_kBossFieldLog.m_mapUserLog.end() )
		{
			std::pair<KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator, bool> pairResult = m_kBossFieldLog.m_mapUserLog.insert( KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::value_type( kInfo.m_nUnitUID, std::vector<KBossFieldUserLog>() ) );
			if( pairResult.second == false )
			{
				return true;
			}

			it = pairResult.first;
		}

		KBossFieldUserLog kBossFieldUserLog;
		kBossFieldUserLog.m_iUnitUID		= kInfo.m_nUnitUID;
		kBossFieldUserLog.m_byteLevel		= kInfo.m_ucLevel;
		kBossFieldUserLog.m_byteClass		= max( kInfo.m_cUnitClass, 0 );
		kBossFieldUserLog.m_wstrNickName	= kInfo.m_wstrNickName;
		it->second.push_back( kBossFieldUserLog );
	}

	return true;
}

void KBattleFieldRoom::_MakeBossFieldUserLogAtLeave( IN const UidType iUnitUID )
{
	KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.find( iUnitUID );
	if( it == m_kBossFieldLog.m_mapUserLog.end() )
	{
		return;
	}

	if( it->second.empty() == true )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		return;
	}

	KBossFieldUserLog& kBossFieldUserLog = it->second.back();
	kBossFieldUserLog.m_iEXP					= spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP();
	kBossFieldUserLog.m_iED						= spRoomUser->GetRewardED();
	kBossFieldUserLog.m_sNumResurrectionStone	= spRoomUser->GetUsedRessurectionStoneCount();

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_CLOSE_PROCESS ) == true )
	{
		// 보스가 죽었거나 도망갔다.
		kBossFieldUserLog.m_byteCompletionType	= ( m_kBossFieldLog.m_kFieldLog.m_bSucceed == true ) ? 0 : 2;
	}
	else
	{
		// 보스가 존재한다.
		kBossFieldUserLog.m_byteCompletionType	= 1;
	}
}

void KBattleFieldRoom::_MakeBossFieldUserLog( IN const UidType iUnitUID, IN const byte byteContributionRank )
{
	KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.find( iUnitUID );
	if( it == m_kBossFieldLog.m_mapUserLog.end() )
	{
		return;
	}

	if( it->second.empty() == true )
	{
		return;
	}

	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iUnitUID );
	if( spRoomUser == NULL )
	{
		return;
	}

	KBossFieldUserLog& kBossFieldUserLog = it->second.back();
	kBossFieldUserLog.m_byteContributionRank = byteContributionRank;
}

IMPL_ON_FUNC_NOPARAM( ERM_BOSS_FIELD_INTRUDE_RESTRICTION_REQ )
{
	KERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK kPacket;

	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_BOSS_FIELD ) == false )
	{
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_03;	// 보스 필드가 아닙니다.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK, kPacket );
		return;
	}

	if( m_spUserManager->IsHost( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_04;	// 사용 권한이 없습니다.
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK, kPacket );
		return;
	}
	
	if( SEnum::CheckFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION ) == false )
	{
		// 난입 불가 설정을 요청해왔다. ( 기획서 의도대로라면, 보스의 피가 10% 아래 쪽일 때 이 패킷이 올 것이다. )
		SEnum::AddFlag( m_byteBossFieldState, SEnum::BFRS_INTRUDE_RESTRICTION );
		SendRoomListInfo( KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE );
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_BOSS_FIELD_LOG_NOT )
{
	// 보스 필드 유저 로그 정보 수집
	for( KDBE_BOSS_FIELD_LOG_NOT::TYPE_BOSS_FIELD_USER_LOG::iterator it = m_kBossFieldLog.m_mapUserLog.begin(); it != m_kBossFieldLog.m_mapUserLog.end(); ++it )
	{
		const UidType		iUnitUID = it->first;
		KBossFieldUserLog&	kBossFieldUserLog = it->second.back();

		std::map<UidType, __int64>::iterator itFind = kPacket_.m_mapAttackDamage.find( iUnitUID );
		if( itFind != kPacket_.m_mapAttackDamage.end() )
		{
			kBossFieldUserLog.m_iAttackDamage = itFind->second;
		}

		itFind = kPacket_.m_mapGivenDamage.find( iUnitUID );
		if( itFind != kPacket_.m_mapGivenDamage.end() )
		{
			kBossFieldUserLog.m_iGivenDamage = itFind->second;
		}
	}
}
#endif // SERV_BATTLE_FIELD_BOSS

#pragma pack( pop )