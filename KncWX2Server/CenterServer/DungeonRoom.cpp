#include "DungeonRoom.h"
#include "NetError.h"
#include "Room/RoomFSM.h"
#include "X2Data/XSLRoom.h"
#include "KDropTable.h"
#include "ResultProcess.h"

#include "x2data/XSLItemManager.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLUnitManager.h"
#include "KAttribNpcTable.h"

//{{ 2009. 5. 22  최육사	드롭률이벤트
#include "GameEvent/GameEventManager.h"
//}}
//{{ 2009. 7. 27  최육사	헤니르 시공
#include "HenirResultTable.h"
#include "GameSysVal/GameSysVal.h"
//}}
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
//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	#include <boost/random.hpp>
#endif SERV_MAKE_MONSTER_UID_CHANGE
//}}
//{{ 2012. 03. 05	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

NiImplementRTTI( KDungeonRoom, KRoom );

#define CLASS_TYPE KDungeonRoom

KDungeonRoom::KDungeonRoom()
{
	//{{ 2011. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_spRoomUserManager = KRoomUserManagerPtr( new KRoomUserManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	m_spRoomUserManager->Init( CXSLRoom::RT_DUNGEON, 4 );

	m_bEndPlay				= false;
	m_cGetItemType			= CXSLRoom::DGIT_RANDOM;
	m_usLoadingState		= 0;

	//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
	m_iLoadingCompleteStageID = -1;
#endif SERV_DUNGEON_STAGE_LOG
	//}}

	// 속성 몬스터 카운트
	m_iAttribNpcStageCnt	= 0;
	m_iAttribNpcDropCnt		= 0;

	//{{ 2010. 04. 07  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	m_iExtraStageLoadCount	= 0;
	m_bAdventExtraNpc		= false;	
	m_fPoisonTimeLimit		= 0.0f;
#endif SERV_SECRET_HELL
	//}}

	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장
#ifdef SERV_PC_BANG_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	m_fItemDropEventProbRate = REE_DROP_EVENT_PROB_DEFAULT;
#else // SERV_DROP_EVENT_RENEWAL
	m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
#endif // SERV_DROP_EVENT_RENEWAL
	m_bWithPlayPcBangEvent	= false;
#endif SERV_PC_BANG_DROP_EVENT
	//}}

	m_iHighestUserLevelAtDungeonStart = 0;
	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	m_iAverageUserLevelAtDungeonStart = 0;
#endif SERV_DUNGEON_NPC_LEVEL
	//}}
	m_eDungeonType			= CXSLDungeon::DT_NORMAL; // 헤니르 시공

	//{{ 2009. 7. 10  최육사	사기 시스템
	m_iPartyUID				= 0;
	//}}

	//{{ 2010. 06. 17  최육사	오토핵 방지
#ifdef SERV_PROTECT_AUTO_HACK
	m_iHackUserCheckCount	= 0;
	m_iNpcDieHackCount		= 0;
	m_iGetItemHackCount		= 0;
	m_iStartedNumMember		= 0;
#endif SERV_PROTECT_AUTO_HACK
	//}}
	//{{ 2012. 03. 15	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_bStartedByAutoParty	= false;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	m_iDefenseMainCrystalNpcUID = 0;
	m_setDefenseSmallCrystalNpcUID.clear();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	m_bHackingNpcUid	= false;
#endif SERV_MAKE_MONSTER_UID_CHANGE	
	//}}

	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	m_fQuestItemDropEventProbRate = 1.f;
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_iSubStageCount = 0;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	m_kNextStageData.ClearData();
	m_bFirstStage = true;
	m_bFirstSubStage = true;
	m_iClearConditionIndex = 0;
#endif // SERV_STAGE_CLEAR_IN_SERVER
}

KDungeonRoom::~KDungeonRoom()
{
}

ImplToStringW( KDungeonRoom )
{
    return START_TOSTRING_PARENTW( KRoom );
}

void KDungeonRoom::Tick()
{
    KRoom::Tick();
	
	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		//{{ 2010. 05. 06  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
		// 비밀던전이라면..
		if( m_eDungeonType == CXSLDungeon::DT_SECRET )
		{
			if( m_fPoisonTimeLimit > 0.0f )
			{
				// 독댐 남은시간과 남은 플레이시간을 합한값이 전체 시간제한보다 작으면 독댐 적용 시간이다!
				if( ( m_fPoisonTimeLimit + GetRemainingPlayTime() ) < GetPlayTimeLimit() )
				{
					KEGS_DUNGEON_EFFECT_TIME_OUT_NOT kPacketNot;
					kPacketNot.m_cType = KEGS_DUNGEON_EFFECT_TIME_OUT_NOT::DDT_POISON;
					BroadCast( ERM_DUNGEON_EFFECT_TIME_OUT_NOT, kPacketNot );

					// 한번 날리고 나서는 -1.f값을 넣어서 다음부터 안날리도록 한다.
					m_fPoisonTimeLimit = -1.f;
				}
			}
		}
#endif SERV_SECRET_HELL
		//}}

		//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
		if( CXSLDungeon::IsValentineDungeon( GetDungeonIDAndDif() ) == true )
		{
			int iRet = KValentineManager::TT_NONE;

			m_kValentineManager.CheckTimer( KValentineManager::TEM_AFTER_DUNGEON_START, iRet );

			switch( iRet )
			{
			case KValentineManager::TT_SYNC_TIME:
				{
					// 모두에게 시간 전달
					KEGS_SYNC_DUNGEON_TIMER_NOT kNot;
					kNot.m_iRemainTime = KValentineManager::LDT_VALENTINE - m_kValentineManager.GetRemainTime( KValentineManager::TEM_AFTER_DUNGEON_START );
					BroadCast( ERM_SYNC_DUNGEON_TIMER_NOT, kNot );
				}
				break;
			case KValentineManager::TT_END_GAME:
				{
					// 무조건 승리
					KEGS_SYNC_DUNGEON_TIMER_NOT kNot;
					kNot.m_iRemainTime = 0;
					BroadCast( ERM_SYNC_DUNGEON_TIMER_NOT, kNot );
				}
				break;
			}
		}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
		if( CXSLDungeon::IsValentineDungeonInt( GetDungeonIDAndDif() ) == true )
		{
			int iRet = KValentineManager::TT_NONE;

			m_kValentineManager.CheckTimerForInt( KValentineManager::TEM_AFTER_DUNGEON_START, iRet );

			switch( iRet )
			{
			case KValentineManager::TT_SYNC_TIME:
				{
					// 모두에게 플레이 시간 전달
					KEGS_SYNC_DUNGEON_TIMER_NOT kNot;
					kNot.m_iRemainTime = m_kValentineManager.GetRemainTime( KValentineManager::TEM_AFTER_DUNGEON_START );
					BroadCast( ERM_SYNC_DUNGEON_TIMER_NOT, kNot );
				}
				break;
			case KValentineManager::TT_END_GAME:
				{
					// 무조건 승리
					KEGS_SYNC_DUNGEON_TIMER_NOT kNot;
					kNot.m_iRemainTime = 0;
					BroadCast( ERM_SYNC_DUNGEON_TIMER_NOT, kNot );
				}
				break;
			}
		}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
#endif SERV_EVENT_VALENTINE_DUNGEON
		//}


		// 게임중일때 모든유저의 상태가 EndPlay라면 게임을 종료한다!
		if( m_spRoomUserManager->CheckEndPlay() )
		{
			EndPlay();
		}
	}
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	else if( GetStateID() == KRoomFSM::S_WAIT_FOR_DEFENCE )
	{
		// 5초가 지나면 현재 인원을 가지고 그대로 어둠의 문 진행 하자!
        if( m_kTimer[TM_WAIT_FOR_DEFENCE].elapsed() > 5.0 )
		{
			m_kTimer[TM_WAIT_FOR_DEFENCE].restart();

			// 아직도 정보가 오지 않은 유저는 
			std::map< UidType, UidType > mapUserList;
			m_spRoomUserManager->GetUnitUIDListNotPrepareForDefenceDungeon( mapUserList );

			// 방에서 이탈 시키자!
			HandleTimeOutUser( mapUserList );			

			//{{ 2013. 04. 23	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
			// 5초동안 어둠의 문 입장 의사를 밝히지 않은 유저는 강제로 입장 처리 하자!
			m_spRoomUserManager->SetAllEnterDefenceDungeon();
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
			//}}

			if( IsEmpty() == false )
			{
				//{{ 2013. 04. 23	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
				// 입장을 거절한 유저들 리스트를 얻자!
				std::vector< UidType > vecLeaveUnitUID;
				m_spRoomUserManager->GetUnitUIDListDisagreeEnterDefenceDungeon( vecLeaveUnitUID );

				// 어둠의 문 던전 입장!
				EnterDefenceDungeon( vecLeaveUnitUID );

				START_LOG( clog, L"5초가 지난 상태에서 어둠의 문 던전을 시작합니다!" );
#else
				// 어둠의 문 던전 입장!
				EnterDefenceDungeon();
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
				//}}
			}
			else
			{
				StateTransition( KRoomFSM::I_TO_CLOSE );
			}
		}
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
}

void KDungeonRoom::SetRoomInfo( const KRoomInfo& kInfo )
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
	m_iPartyUID			= kInfo.m_iPartyUID;
	//{{ 2012. 03. 15	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_bStartedByAutoParty = kInfo.m_bStartedByAutoParty;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

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
	//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	m_kDefenceDungeonManager.SetOpenDefenceDungeon( kInfo.m_bDefenceDungeonOpen );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
}

void KDungeonRoom::GetRoomInfo( OUT KRoomInfo& kInfo )
{
	KRoom::GetRoomInfo( kInfo );
	kInfo.m_bStartedByAutoParty = IsStartedByAutoParty();
}

//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
void KDungeonRoom::LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom )
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

//{{ 2012. 11. 9	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
void KDungeonRoom::LogToDB_DungeonLog( IN KRoomUserPtr spRoomUser, IN const int iClearValue, IN const KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfo, IN const KDungeonUnitResultInfo& kDungeonUnitInfo, IN const int iLeaveReason /*= 0*/ )
#else
void KDungeonRoom::LogToDB_DungeonLog( IN KRoomUserPtr spRoomUser, IN const int iClearValue, IN const KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfo, IN const KDungeonUnitResultInfo& kDungeonUnitInfo )
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
//}}
{
	// 운영자는 통계를 남기지 않습니다.
	if( spRoomUser->GetAuthLevel() >= SEnum::UAL_GM )
	{
		START_LOG( cout, L"운영자는 통계데이터를 남기지 않습니다." )
			<< BUILD_LOGc( spRoomUser->GetAuthLevel() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spRoomUser->GetCID() );
#else
			<< BUILD_LOG( spRoomUser->GetNickName() );
#endif SERV_PRIVACY_AGREEMENT
		return;
	}

	CTime kRegDate = CTime::GetCurrentTime();
	//////////////////////////////////////////////////////////////////////////
	KE_LOCAL_LOG_DUNGEON_NOT kNot;
	kNot.m_iDungeonID				= (m_iDungeonID + static_cast<int>(m_cDifficultyLevel));
	kNot.m_nUnitUID					= spRoomUser->GetCID();
	kNot.m_wstrNickName				= spRoomUser->GetNickName();
	kNot.m_iDifficultyLevel			= (int) m_cDifficultyLevel;
	kNot.m_iIsChallenge				= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
	kNot.m_iUnitClass				= (int) spRoomUser->GetUnitClass_LUA();
	kNot.m_iClear					= iClearValue;
	kNot.m_iStartNumMember			= m_iStartedNumMember;
	kNot.m_iEndNumMember			= GetNumMember();
	kNot.m_ucLevel					= spRoomUser->GetLevel();
	kNot.m_iPlayTime				= (int) GetTotalPlayTime();
	kNot.m_iEXP						= kUpdateUnitInfo.m_iEXP;
	kNot.m_iED						= spRoomUser->GetRewardED();
	kNot.m_cComboRank				= kDungeonUnitInfo.m_cComboRank;
	kNot.m_iComboScore				= spRoomUser->GetComboScore_LUA();
	kNot.m_cTechnicalRank			= kDungeonUnitInfo.m_cTechnicalRank;
	kNot.m_iTechnicalScore			= spRoomUser->GetTechnicalScore_LUA();
	kNot.m_cTimeRank				= kDungeonUnitInfo.m_cTimeRank;
	kNot.m_cDamagedRank				= kDungeonUnitInfo.m_cDamagedRank;
	kNot.m_iDamageNum				= spRoomUser->GetDamageNum_LUA();
	kNot.m_cTotalRank				= kDungeonUnitInfo.m_cTotalRank;
	kNot.m_iRessurectionStoneCount	= spRoomUser->GetUsedRessurectionStoneCount();
	kNot.m_iPassedStageCount		= spRoomUser->GetPassedStageCount();
	kNot.m_iPassedSubStageCount		= spRoomUser->GetPassedSubStageCount();
	//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	kNot.m_bIsWithPet				= spRoomUser->HavePet();
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 11. 8	최육사	던전 로그 DB
#ifdef SERV_DUNGEON_LOG_DB
	kNot.m_wstrRegDate				= (const wchar_t*)kRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_DUNGEON_LOG_DB
	//}}
	//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
	kNot.m_sNpcDieCount				= spRoomUser->GetKillNPC_LUA();
	kNot.m_iGivenDamage				= spRoomUser->GetTotalGivenDamamge_LUA();
	kNot.m_iAttackDamage			= spRoomUser->GetTotalAttackedDamage_LUA();
	kNot.m_sUsingSkillKind			= spRoomUser->GetUsingKindOfSkill_LUA();
	kNot.m_iTotalEXP				= ( spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP() + kUpdateUnitInfo.m_iEXP );
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
	kNot.m_iPetID					= spRoomUser->GetPetID();
#else //SERV_PETID_DATA_TYPE_CHANGE
	kNot.m_cPetID					= spRoomUser->GetPetID();
#endif //SERV_PETID_DATA_TYPE_CHANGE
	kNot.m_cPetEvoStep				= spRoomUser->GetPetEvolutionStep();
#endif SERV_ADD_DUNGEON_LOG_COLUMN
	//}}
	//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
	kNot.m_iDashCount				= spRoomUser->GetDashCount();
#endif SERV_DUNGEON_DASH_LOG
	//}}
	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	kNot.m_iBaseHP					= spRoomUser->GetBaseHP_LUA();
	kNot.m_iEndHP					= spRoomUser->GetLastUpdatedHP();
	kNot.m_iRecovHP					= spRoomUser->GetTotalRecoveryHP_LUA();
	kNot.m_iContinue				= spRoomUser->GetUsedRessurectionStoneCount();
	kNot.m_iStartContinue			= spRoomUser->GetStartTimeNumResurrectionStone();
	kNot.m_iContinueUseStage		= spRoomUser->GetFirstUseResurrectionStoneStageID();
	kNot.m_iContinueUseSub			= spRoomUser->GetFirstUseResurrectionStoneSubStageID();
	kNot.m_iFrame					= spRoomUser->GetInGameAverageFrame();
	kNot.m_wstrChannelIP			= spRoomUser->GetGameChannelIP();
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2012. 07. 11 김민성
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
	kNot.m_iUserUID					= spRoomUser->GetUserUID();
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
	//}}
	//{{ 2012. 11. 6	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kNot.m_cAutoPartyPlay			= ( IsStartedByAutoParty() ? 1 : 0 );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	kNot.m_iAutoPartyWaitTime		= spRoomUser->GetAutoPartyWaitTime();
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}
	//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	kNot.m_iLeaveReason				= iLeaveReason;
	kNot.m_cRoomState				= GetStateID();
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	//}}
	//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
	kNot.m_bSkillSlotType				= spRoomUser->GetSkillSlotType();
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
	//}}
	KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_NOT, kNot );
	//////////////////////////////////////////////////////////////////////////
	
	// DB에도 남기자
	SendToLogDB( DBE_DB_LOG_DUNGEON_NOT, kNot );

	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	bool bExitResult = false;

	if( iLeaveReason == NetError::NOT_LEAVE_ROOM_REASON_34 )
	{
		bExitResult = true;
	}

	// 던전 강퇴 시스템 로그 기록하자
	// 시작 인원이 2인 이상일 경우
	if( m_bStartedByAutoParty == true && m_iStartedNumMember >= 2 )
	{
		KBadAttitudeManager::KBadAttitudeInfo kUnitData( spRoomUser->GetCID() );
		if( m_kBadAttitudeManager.GetUnitData( spRoomUser->GetCID(), kUnitData ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 유저를 지우려고 한다." )
				<< BUILD_LOG( spRoomUser->GetCID() )
				<< END_LOG;

			// 그래도 계속 진행 한다.
		}
		else
		{
			if( kUnitData.m_iBadAttitudePoint > 0 )
			{
				// 강퇴 시스템으로 인한 강퇴 로그 추가
				KERM_BAD_ATTITUDE_USER_LOG_NOT kLog;
				kLog.m_iUnitUID = spRoomUser->GetCID();
				kLog.m_iDungeonIDAndDif = GetDungeonIDAndDif();
				kLog.m_sStartMember = static_cast<short>(m_iStartedNumMember);
				kLog.m_sNowMember = static_cast<short>(GetNumMember());
				kLog.m_sVoteCount = static_cast<short>(kUnitData.m_iVotePoint);
				kLog.m_sExitStage = static_cast<short>(kUnitData.m_kRankInfo.m_iStage);
				kLog.m_sExitSubStage = static_cast<short>(kUnitData.m_kRankInfo.m_iSubStage);
				kLog.m_sBadAttitudePoint = static_cast<short>(kUnitData.m_iBadAttitudePoint);
				kLog.m_wstrRegDate = (const wchar_t*)kRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
				kLog.m_bExitResult = bExitResult;

				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_BAD_ATTITUDE_USER_LOG_NOT, kLog );
			}
		}
	}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

}
//#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KDungeonRoom::ProcessNativeEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
   _CASE( ERM_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ );

	//////////////////////////////////////////////////////////////////////////
    // party
	CASE( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ );
   //_CASE( ERM_PARTY_GAME_REPLAY_START_REQ, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ );
   //{{ 2011. 12. 09	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _CASE( ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ );
   //_CASE( ERM_SINGLE_DUNGEON_GAME_REPLAY_REQ, KERM_OPEN_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
   //}}
	// party
	//////////////////////////////////////////////////////////////////////////

    CASE( ERM_GAME_START_REQ );
	CASE( ERM_LEAVE_ROOM_REQ );
	//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _CASE( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ );
   _CASE( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, KERM_LEAVE_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   _CASE( ERM_DUNGEON_STAGE_LOAD_REQ, KEGS_DUNGEON_STAGE_LOAD_REQ );
   _CASE( ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ, KEGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ );

   _CASE( ERM_DUNGEON_SUB_STAGE_OPEN_REQ,			KEGS_DUNGEON_SUB_STAGE_OPEN_REQ );
   _CASE( ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ,		KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ );
   _CASE( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ,	KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ );

	//{{ 2009. 7. 20  최육사	비밀스테이지
	CASE_NOPARAM( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
   _CASE( ERM_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT );
	//}}

   _CASE( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ );
   _CASE( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ );
   _CASE( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ );
    CASE_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ );
   
   _CASE( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ );
	//{{ 2009. 5. 11  최육사	실시간아이템
	CASE( ERM_GET_ITEM_COMPLETE_NOT );
	//}}
	CASE( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ );
   _CASE( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT );
   _CASE( ERM_END_GAME_REQ, KEGS_END_GAME_REQ );
   _CASE( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT );

   _CASE( ERM_CHECK_INVALID_USER_NOT, UidType );

   _CASE( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ );
	CASE_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT );

    CASE( ERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT );
	
   _CASE( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ );

   _CASE( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT );

	CASE_NOPARAM( ERM_PARTY_BAN_USER_REQ );

	//{{ 2009. 12. 29  최육사	NPCKILLALL해킹방지
	CASE_NOPARAM( ERM_DUNGEON_KILLALLNPC_CHECK_REQ );
	//}}
	//{{ 2010. 11. 18	최육사	아이템 판매 차단
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM
   _CASE( ERM_CHECK_SELL_ED_ITEM_REQ, KEGS_SELL_ED_ITEM_REQ );
#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
	//}}

   //{{ 2011. 01. 25  김민성  월드 트리거 전달(던전만 적용중 - 이후 PVP 확장)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
   _CASE( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif SERV_WORLD_TRIGGER_RELOCATION
   //}}
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
   _CASE( ERM_ADMIN_NPC_UNIT_CREATE_REQ, KEGS_ADMIN_NPC_UNIT_CREATE_REQ );
#endif SERV_CREATE_ELITE_FOR_ADMIN
   
   //{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
   _CASE( ERM_UDP_CHECK_KICK_USER_NOT, KEGS_UDP_CHECK_KICK_USER_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
   //}}
	//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE_NOPARAM( ERM_UPDATE_HENIR_REWARD_USER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	_CASE( ERM_BAD_ATTITUDE_USER_CHECK_INFO_NOT, KEGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT );
	_CASE( ERM_DEFENCE_DUNGEON_WAVE_ID_NOT, KEGS_DEFENCE_DUNGEON_WAVE_ID_NOT );
	_CASE( ERM_FORCED_EXIT_VOTE_REQ, KEGS_FORCED_EXIT_VOTE_REQ );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	CASE_NOPARAM( ERM_SYNC_DUNGEON_TIMER_NOT );
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CASE( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	_CASE( ERM_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT, KEGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    CASE_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	_CASE( ERM_DUNGEON_SUB_STAGE_CLEAR_REQ, KEGS_DUNGEON_SUB_STAGE_CLEAR_REQ );
	_CASE( ERM_SECRET_STAGE_LOAD_REQ, KEGS_SECRET_STAGE_LOAD_REQ );
#endif // SERV_STAGE_CLEAR_IN_SERVER

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음 - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
    }
}

void KDungeonRoom::StartGame()
{
    KRoom::StartGame();

	//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
//#ifdef SERV_MAKE_MONSTER_UID_CHANGE
//	m_nNPCUID = GetDungeonNpcRandomInitUid();
//#else
//	m_nNPCUID			= 1;
//#endif SERV_MAKE_MONSTER_UID_CHANGE
	//}}

	//{{ 2011. 10. 24	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kDungeonMonsterManager.StartGame();
	m_kDungeonSecurityManager.StartGame();
#else
	m_nNPCUID			= 1;
	m_mapNPCData.clear();
	m_mapSubStageNpcInfo.clear();	// NPCKILLALL해킹방지
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	m_nDropItemUID		= 1;
	//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
	m_iLoadingCompleteStageID = -1;
#endif SERV_DUNGEON_STAGE_LOG
	//}}

	m_iStageID			= 0;
	m_iSubStageID		= 0;
	m_fTotalPlayTime	= 0.0;
	m_fLoadingTime		= 0.0;

	m_bIsWin			= false;

	m_kDSSGoNextAllNot.m_iBeforeStageID = -1;
	m_kDSSGoNextAllNot.m_iNextStageID	= -1;

	m_mapDropItem.clear();
	//{{ 2009. 5. 13  최육사	서브스테이지별 drop npc수
	m_mapSubStageExpNpcNum.clear();
	//}}
	m_mapAttribNpcData.clear(); // 속성 몬스터	
	//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	m_mapTimeDropEventMonster.clear();
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}
	//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	m_mapSubStageActiveNpcNum.clear();
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}

	m_usLoadingState	= 0;

	//{{ 2010. 04. 07  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	m_iExtraStageLoadCount	= 0;
	m_bAdventExtraNpc		= false;	
	m_fPoisonTimeLimit		= SiCXSLDungeonManager()->GetPoisonTimeLimit( GetDungeonIDAndDif() );
#endif SERV_SECRET_HELL
	//}}
    
    const int iPlayerCount = m_spRoomUserManager->GetNumPlaying();
    if( iPlayerCount < 1 || iPlayerCount > 4 )
    {
        START_LOG( cerr, L"플레이 중인 유저 수가 이상함." )
            << BUILD_LOG( iPlayerCount )
            << END_LOG;
    }

	// 던전 타입 얻기
	m_eDungeonType = SiCXSLDungeonManager()->GetDungeonType( GetDungeonIDAndDif() );

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-23	// 박세훈

	m_iHighestUserLevelAtDungeonStart = m_spRoomUserManager->GetHighestUserLevel();

	//{{ 2012. 05. 14	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	// 평균 레벨값을 구하되 최대레벨과의 차이가 5렙이상인 유저는 계산에 포함하지 말자
	m_iAverageUserLevelAtDungeonStart = m_spRoomUserManager->GetAverageUserLevel( CXSLDungeon::DE_CALC_AVERAGE_INCLUDE_LEVEL_GAP );
#endif SERV_DUNGEON_NPC_LEVEL
	//}}

#else // SERV_HENIR_RENEWAL_2013
	switch( m_eDungeonType )
	{
	case CXSLDungeon::DT_HENIR:
		{
			// 헤니르 시공 도전모드라면 캐릭터 스탯이 1로 보정되기 때문에 유저 최고레벨을 1로 둔다
			if( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE )
			{
				m_iHighestUserLevelAtDungeonStart = 1;
				//{{ 2012. 05. 14	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
				m_iAverageUserLevelAtDungeonStart = 1;
#endif SERV_DUNGEON_NPC_LEVEL
				//}}
			}
			else
			{
				m_iHighestUserLevelAtDungeonStart = m_spRoomUserManager->GetHighestUserLevel();

				//{{ 2012. 05. 14	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
				// 평균 레벨값을 구하되 최대레벨과의 차이가 5렙이상인 유저는 계산에 포함하지 말자
				m_iAverageUserLevelAtDungeonStart = m_spRoomUserManager->GetAverageUserLevel( CXSLDungeon::DE_CALC_AVERAGE_INCLUDE_LEVEL_GAP );
#endif SERV_DUNGEON_NPC_LEVEL
				//}}
			}
		}
		break;

	default:
		{
			m_iHighestUserLevelAtDungeonStart = m_spRoomUserManager->GetHighestUserLevel();

			//{{ 2012. 05. 14	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
			// 평균 레벨값을 구하되 최대레벨과의 차이가 5렙이상인 유저는 계산에 포함하지 말자
			m_iAverageUserLevelAtDungeonStart = m_spRoomUserManager->GetAverageUserLevel( CXSLDungeon::DE_CALC_AVERAGE_INCLUDE_LEVEL_GAP );
#endif SERV_DUNGEON_NPC_LEVEL
			//}}
		}
		break;
	}
#endif // SERV_HENIR_RENEWAL_2013

	if( GetHighestUserLevelAtDungeonStart() <= 0 )
	{
		START_LOG( cerr, L"던전 시작시에 유저 최고레벨 수치가 이상합니다." )
			<< BUILD_LOG( iPlayerCount )
			<< BUILD_LOG( GetHighestUserLevelAtDungeonStart() )
			<< BUILD_LOG( GetDungeonID() )
			<< BUILD_LOGc( GetDifficultyLevel() )
			<< END_LOG;

		m_iHighestUserLevelAtDungeonStart = SiKGameSysVal()->GetLimitsLevel();
	}

	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	if( GetAverageUserLevelAtDungeonStart() <= 0 )
	{
		START_LOG( cerr, L"던전 시작시에 유저 평균레벨 수치가 이상합니다." )
			<< BUILD_LOG( iPlayerCount )
			<< BUILD_LOG( GetAverageUserLevelAtDungeonStart() )
			<< BUILD_LOG( GetDungeonID() )
			<< BUILD_LOGc( GetDifficultyLevel() )
			<< END_LOG;

		m_iAverageUserLevelAtDungeonStart = SiKGameSysVal()->GetLimitsLevel();
	}
#endif SERV_DUNGEON_NPC_LEVEL
	//}}	

	//////////////////////////////////////////////////////////////////////////
	// [이벤트]	이벤트 몬스터 드롭
#ifdef SERV_EVENT_MONSTER
	m_vecEventMonster.clear();

	if( CXSLDungeon::IsTutorialDungeon( GetDungeonID() ) == false  &&
		CXSLDungeon::IsTCDungeon( GetDungeonID() ) == false  &&
		CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == false )
	{
		std::vector< int > vecEventID;
		SiKGameEventManager()->CheckEventMonster( vecEventID );
		//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		SiKEventMonsterRefreshManager()->GetAdventNpcEventInfo( vecEventID, m_vecEventMonster );
#else
		SiKGameEventScriptManager()->GetAdventNpcEventInfo( vecEventID, m_vecEventMonster );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
	}
#endif SERV_EVENT_MONSTER
	//////////////////////////////////////////////////////////////////////////
	
	//{{ 2010. 04. 07  최육사	속성드롭 이벤트 몹
#ifdef SERV_SECRET_HELL
	m_iAttribNpcStageCnt = 0;
	m_iAttribNpcDropCnt = 0;

	switch( m_eDungeonType )
	{
	case CXSLDungeon::DT_NORMAL:
		{
			// 엘의 나무에서는 속성 몬스터 안나온다.
			if( SEnum::DI_RUBEN_EL_TREE_NORMAL != GetDungeonID() )
			{
				// 속성 몬스터 드롭 카운트
				//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
				SiKAttribNpcTable()->GetAttribNpcDropCount( static_cast<char>(m_eDungeonType), m_cDifficultyLevel, iPlayerCount, m_iAttribNpcStageCnt, m_iAttribNpcDropCnt );
#else
				SiKAttribNpcTable()->GetAttribNpcDropCount( static_cast<char>(m_eDungeonType), m_cDifficultyLevel, m_iAttribNpcStageCnt, m_iAttribNpcDropCnt );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
				//}}
			}
		}
		break;

	case CXSLDungeon::DT_HENIR: // 헤니르 시공에서는 속성몬스터 안나옴.
		break;

	case CXSLDungeon::DT_SECRET:
		{
			// 속성 몬스터 드롭 카운트
			//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
			SiKAttribNpcTable()->GetAttribNpcDropCount( static_cast<char>(m_eDungeonType), m_cDungeonMode, iPlayerCount, m_iAttribNpcStageCnt, m_iAttribNpcDropCnt );
#else
			SiKAttribNpcTable()->GetAttribNpcDropCount( static_cast<char>(m_eDungeonType), m_cDungeonMode, m_iAttribNpcStageCnt, m_iAttribNpcDropCnt );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
			//}}
		}
		break;

	default:
		{
            START_LOG( cerr, L"잘못된 던전 타입입니다." )
				<< BUILD_LOG( m_eDungeonType )
				<< END_LOG;
		}
		break;
	}
#else
#pragma ELSE_MESSAGE
	// 속성 몬스터 드롭 카운트
	//SiKAttribNpcTable()->GetAttribNpcDropCount( static_cast<int>(m_cDifficultyLevel), m_iAttribNpcStageCnt, m_iAttribNpcDropCnt );
#endif SERV_SECRET_HELL
	//}}

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	// 해당 유저가 특정 아이템 착용 퀘스트를 진행중이면서 아이템을 착용하고 있는지 확인한다.
	m_spRoomUserManager->CheckItemEquipQuestCondition( true );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2010. 06. 17  최육사	오토핵 방지
#ifdef SERV_PROTECT_AUTO_HACK
	m_iHackUserCheckCount = ( rand() % 6 ) + 5; // 5에서 10사이의 체크 숫자 얻기!
	m_iNpcDieHackCount = 0;
	m_iGetItemHackCount = 0;
	m_iStartedNumMember = iPlayerCount; // 게임 시작할 당시의 유저 수 저장!
#endif SERV_PROTECT_AUTO_HACK
	//}}

	//{{ 2010. 7. 28  최육사	오토핵 차단 NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
    m_setFindedHackUserUnitUID.clear();
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}

	//{{ 2010. 11. 18	최육사	오토 핵 스테이지 로딩 체크
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	m_mapStageLoadingCheck.clear();
	m_iLastLoadingUnitUID = 0;
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
	//}}

	//{{ 2010. 12. 8	최육사	적정 레벨 장비 통계
#ifdef SERV_APPROPRIATE_LEVEL_STAT
	{
		KELOG_APPROPRIATE_LEVEL_STATISTICS_NOT kPacketNot;
		kPacketNot.m_iDungeonID = GetDungeonIDAndDif();
		kPacketNot.m_cDifficultyLevel = GetDifficultyLevel();
		kPacketNot.m_tDungeonStartTime = CTime::GetCurrentTime().GetTime();
		m_spRoomUserManager->GetAppropriateLevelStatistics( kPacketNot );
        SendToLogDB( ELOG_APPROPRIATE_LEVEL_STATISTICS_NOT, kPacketNot );
	}
#endif SERV_APPROPRIATE_LEVEL_STAT
	//}}

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	m_iDefenseMainCrystalNpcUID = 0;
	m_setDefenseSmallCrystalNpcUID.clear();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	SetHackingNpcUid( false );
#endif SERV_MAKE_MONSTER_UID_CHANGE	
	//}}
	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	m_kBadAttitudeManager.init( m_spRoomUserManager, GetDungeonIDAndDif() );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

	//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	m_kValentineManager.Init();
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}
	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	m_kDefenceDungeonManager.StartGame();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
#ifdef SERV_CREATED_NPC_LIMITED_DROPS
	m_mapDroppedTimes.clear();
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_iSubStageCount = 0;
    m_kTimer[TM_CHECK_CHANGE_HOST].restart();
    m_bCheckPingScoreExecuted = false;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	m_kNextStageData.ClearData();
	m_bFirstStage = true;
	m_bFirstSubStage = true;
	m_iClearConditionIndex = 0;
#endif // SERV_STAGE_CLEAR_IN_SERVER
}

void KDungeonRoom::StartPlay()
{
	//{{ 2010. 8. 25	최육사	던전 플레이 상태 체크!
	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		if( m_spRoomUserManager->GetNumMember() == 1 )
		{
			START_LOG( cout, L"이미 KRoomFSM::S_PLAY상태인데 또 StartPlay를 하려고 하네!? 해킹인가?" )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetTotalPlayTime() )
				<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
				<< BUILD_LOG( GetLoadingTime() )
				<< BUILD_LOG( GetDungeonIDAndDif() )
				<< BUILD_LOG( GetStageID() )
				<< BUILD_LOG( GetSubStageID() );

			KRoomUserPtr spUser = m_spRoomUserManager->GetHostUser();
			if( spUser != NULL )
			{
				START_LOG( cout, L"어떤 유저가 이렇게 이상한 동작을 하는지 확인해보자!" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCID() );
#else
					<< BUILD_LOG( spUser->GetCID() )
					<< BUILD_LOG( spUser->GetNickName() );
#endif SERV_PRIVACY_AGREEMENT

				// 파티장에게 해킹체크패킷을 보내자!
				//KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				//kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_PLAY_START;
				//SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
			}
		}
	}
	//}}

    KRoom::StartPlay();

	//게임초기로딩시 부모클래스에서 사용되는 로딩시간을 체크하기위해
	//이곳에서 체크를 시작한다.

	// 로딩 타이머 시작!
	if( m_LoadingTimer.IsStart() == true )
	{
		// 이미 시작된 상태라면 종료 상태로 초기화 합니다!
		m_LoadingTimer.End();
	}
	m_LoadingTimer.Start();

	// 던전게임 처음 시작할때 최초로 독대미지 타임을 보낸다!
	if( m_eDungeonType == CXSLDungeon::DT_SECRET  &&  m_fPoisonTimeLimit > 0.0f )
	{
		KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT kNot;
		kNot.m_fRemainingTime = SiCXSLDungeonManager()->GetPoisonTimeLimit( GetDungeonIDAndDif() );
		BroadCast( ERM_DUNGEON_EFFECT_REMAINING_TIME_NOT, kNot );		
	}

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_iSubStageCount = 0;
    m_kTimer[TM_CHECK_CHANGE_HOST].restart();
    m_bCheckPingScoreExecuted = false;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST
}

void KDungeonRoom::EndPlay()
{
	m_bEndPlay = false;
    KRoom::EndPlay();

#ifdef SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE// 작업날짜: 2013-09-04	// 박세훈
	ComputeTotalPlayTime();
#else // SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE
    m_fTotalPlayTime = m_kTimer[TM_PLAY].elapsed() - GetLoadingTime();

	// 플레이 타임이 정상적으로 계산되어 나오는지 확인해보자!
	if( GetTotalPlayTime() < 0.0 )
	{
		START_LOG( cout, L"로딩시간을 뺀 실제 던전 플레이 시간이 마이너스값이다! 있을 수 없는 에러!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( GetTotalPlayTime() )
			<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
			<< BUILD_LOG( GetLoadingTime() )
			<< BUILD_LOG( GetDungeonIDAndDif() )
			<< BUILD_LOG( GetStageID() )
			<< BUILD_LOG( GetSubStageID() )
			<< BUILD_LOG( m_spRoomUserManager->GetNumMember() )
			<< BUILD_LOG( m_iStartedNumMember )
			<< END_LOG;
	}	
#endif // SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE
}

void KDungeonRoom::EndGame()
{
	// 부활석 사용 통계
	int iPlayerCount = m_spRoomUserManager->GetUsedRessurectionStonePlayerCount();

	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_RES_STONE, kKey, KStatistics::eSIColResStone_PlayerCount, iPlayerCount );

    KRoom::EndGame();

	// 설마하는 마음에 이곳에서 다시 초기화를 해본다.(07.12.28 demiahun)
	m_mapDropItem.clear();
	//{{ 2011. 10. 24	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	ReservedRegroupOldParty();
	m_kDungeonMonsterManager.EndGame();
	m_kDungeonSecurityManager.EndGame();
#else
	m_mapNPCData.clear();
	m_mapSubStageNpcInfo.clear();	// NPCKILLALL해킹방지
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	//{{ 2009. 5. 13  최육사	서브스테이지별 drop npc수
	m_mapSubStageExpNpcNum.clear();
	//}}
	m_mapAttribNpcData.clear(); // 속성 몬스터	
	//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	m_mapTimeDropEventMonster.clear();
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

	//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	m_mapSubStageActiveNpcNum.clear();
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}
	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	m_kDefenceDungeonManager.EndGame();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
}

bool KDungeonRoom::CheckIfPlayEnd()
{
    if( KRoom::CheckIfPlayEnd() )
    {
        return true;
    }

    if( m_spRoomUserManager->IsAllPlayerDie() )
    {
        return true;
    }

    return false;
}

int KDungeonRoom::DecideWinTeam()
{
    if( /*m_spRoomUserManager->IsAllPlayerDie() || */m_bIsWin == false )
    {
        return CXSLRoom::TN_MONSTER;
    }
    else
    {
        return CXSLRoom::TN_RED;
    }
}

int KDungeonRoom::GetPlayMode()
{
    return m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
}

void KDungeonRoom::HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS )
{
    if( mapUserCIDNGS.empty() )
    {
        return;
    }

    std::map< UidType, UidType >::iterator mit;
    for( mit = mapUserCIDNGS.begin(); mit != mapUserCIDNGS.end(); mit++ )
    {
		if( !m_spRoomUserManager->LeaveRoom( mit->first ) )
		{
			START_LOG( cerr, L"방 나가기 실패." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}

        // 문제 유저에게 패킷을 날린다.
        SendToGSCharacter( mit->second, mit->first, ERM_GAME_LOADING_TIME_OUT_NOT );
    }

    // 방의 모든 유저들에게 슬롯 정보를 날린다.
    KEGS_LEAVE_ROOM_NOT kPacketNot;
    kPacketNot.m_cRoomState = GetStateID();
    kPacketNot.m_iUnitUID = mit->first;
    GetRoomSlotInfo( kPacketNot.m_vecSlot );
    BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );

	//{{ 2010. 11. 23	최육사	방 로딩 오류 수정 - 크래쉬 문제가 있어서 적용은 보류합니다.
#ifdef SERV_ROOM_LOADING_BUG_FIX
	//////////////////////////////////////////////////////////////////////////
	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		if( m_spRoomUserManager->IsAllPlayerFinishLoading() == true )
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
	}
	else if( GetStateID() == KRoomFSM::S_PLAY )
	{
		CheckLoadingSuccess();
	}
	//////////////////////////////////////////////////////////////////////////	
#else
	//////////////////////////////////////////////////////////////////////////
	if( GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY )
	{
		//// 모든 유저가 로딩 끝났으면 not를 날린다.
		//if( m_spRoomUserManager->IsAllPlayerFinishLoading() )
		//{
		//    BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );

		//    // 스테이지를 초기화.
		//    StartPlay();

		//    KEGS_PLAY_START_NOT kNot;
		//    kNot.m_RoomState = GetStateID();
		//    GetRoomSlotInfo( kNot.m_vecSlot );

		//    // 모든 유저에게 플레이 시작을 알림.
		//    BroadCast( ERM_PLAY_START_NOT, kNot );
		//}
		
		CheckLoadingSuccess();
	}
	//////////////////////////////////////////////////////////////////////////
#endif SERV_ROOM_LOADING_BUG_FIX
	//}}

    // 스테이지가 끝나게 되는 경우 처리.
    if( GetStateID() == KRoomFSM::S_PLAY )
    {
        if( CheckIfPlayEnd() )
        {
            EndPlay();
        }
		else
		{
			//{{ 2009. 7. 27  최육사	비밀스테이지
			if( m_eDungeonType == CXSLDungeon::DT_HENIR )
			{
				CheckEnterSecretStageSuccess();
			}
			//}}
		}
    }

    // 방에서 모든 유저가 나갔으면 방을 닫는다.
    if( IsEmpty() )
    {
        StateTransition( KRoomFSM::I_TO_CLOSE );

		//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
		CTime kEndGameTime = CTime::GetCurrentTime();
		//////////////////////////////////////////////////////////////////////////
		KE_LOCAL_LOG_DUNGEON_ROOM_NOT kNot;
		kNot.m_iDungeonID		= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
		kNot.m_iDifficultyLevel	= (int) GetDifficultyLevel();
		kNot.m_iIsChallenge		= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
		kNot.m_iWin				= 0;
		kNot.m_iLose			= 0;
		kNot.m_iStartNumMember	= m_iStartedNumMember;
		kNot.m_iEndNumMember	= GetNumMember();
		kNot.m_iTotalPlayTime	= static_cast<int>(GetTotalPlayTime());
		kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_ROOM_NOT, kNot );
		//////////////////////////////////////////////////////////////////////////
#endif SERV_STATISTICS_THREAD
		//}}
    }
    else
    {
        if( m_kDSSGoNextAllNot.m_iBeforeStageID != -1 &&
            m_kDSSGoNextAllNot.m_iNextStageID	!= -1 )
            SendDSSGoNextAllNot( m_kDSSGoNextAllNot );
    }
}

void KDungeonRoom::CloseRoom()
{
	// 방이 닫힐때 정리할것들을 정리하자.
	//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kDungeonMonsterManager.OnCloseRoom();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	m_kDefenceDungeonManager.OnCloseRoom();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//방생성중 조건이 안되어 생성을 못하는 경우라면
	if( GetStateID() == KRoomFSM::S_INIT )
	{
		StateTransition( KRoomFSM::I_TO_WAIT );
	}

	StateTransition( KRoomFSM::I_TO_CLOSE );

	//{{ 2013. 01. 07	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	// 방을 닫기 전에 초기화 한번 해주자!
	m_spRoomUserManager->Reset();
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

	KCnBaseRoom::CloseRoom();
}

void KDungeonRoom::CheckResultSuccess()
{
	if( GetStateID() != KRoomFSM::S_RESULT )
		return;

	CheckDungeonUnitInfoSuccess();

	KRoom::CheckResultSuccess();
}

void KDungeonRoom::SendDSSGoNextAllNot( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT& kDSSGoNextAllNot )
{
	if( m_spRoomUserManager->IsAllPlayerStageID() == true )
	{
		m_spRoomUserManager->ResetStageID();
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		m_spRoomUserManager->ResetSubStageID();
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}

		BroadCast( ERM_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT, kDSSGoNextAllNot );

		kDSSGoNextAllNot.m_iBeforeStageID	= -1;
		kDSSGoNextAllNot.m_iNextStageID		= -1;
	}
}

void KDungeonRoom::BroadCastUpdateUnitInfoWithPartyCheck( std::vector< KRoomUserPtr >& vecKRoomUserPtr,
														 unsigned short usEventID, 
														 std::vector< KERM_UPDATE_DUNGEON_UNIT_INFO_NOT >& data, int iDungeonID )
{
	for( u_int ui = 0; ui < vecKRoomUserPtr.size(); ++ui )
	{
		data[ui].m_iDungeonID	= iDungeonID;

		SendToGSCharacter( vecKRoomUserPtr[ui]->GetGSUID(), vecKRoomUserPtr[ui]->GetCID(), usEventID, data[ui] );
	}
}

KDropItemData KDungeonRoom::CreateItemProcess( IN int iItemID, IN bool bIsGP /* = false */, IN int iED /* = 0  */ )
{
	KDropItemData	kDropItem;	//not 패킷을 들어갈 개별 아이템정보
	ITEM_DATA		sItemData;	//서버에서 가지고있을 아이템정보

	int iDropItemUID = m_nDropItemUID++;

	kDropItem.m_iItemID		 = iItemID;
	kDropItem.m_iDropItemUID = iDropItemUID;

	if( (rand()%2) == 1 )
		kDropItem.m_bLeft = true;
	else
		kDropItem.m_bLeft = false;

	kDropItem.m_fSpeed	= (float)(rand()%800) + 1.0f;

	//아이템을 떨어뜨리고 아직 먹지 않은것으로 체크,.
	sItemData.m_iItemID	 	 = iItemID;
	sItemData.m_iDropItemUID = iDropItemUID;
	sItemData.m_bIsGet		 = false;
	sItemData.m_bIsGP		 = bIsGP;	

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

void KDungeonRoom::CheckLoadingSuccess()
{
	if( m_usLoadingState != 0 )
	{
		switch( m_usLoadingState )
		{
		case ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ:
			{
				if( m_spRoomUserManager->IsAllPlayerStageLoaded() )
				{
					// Loading 변수 리셋.
					m_spRoomUserManager->ResetStageLoaded();

					//모든 로딩처리를 끝내고 시작을 알려줌
					KEGS_DUNGEON_STAGE_START_NOT kPacketStartNot;
					kPacketStartNot.m_iStageID = m_iStageID;
					BroadCast( ERM_DUNGEON_STAGE_START_NOT, kPacketStartNot );

					//{{ 2009. 12. 14  최육사	헤니르임시인벤
					CheckTempInventoryClear();
					//}}

					//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
					// 로딩 완료된 스테이지ID를 기록한다!
					m_iLoadingCompleteStageID = GetStageID();
#endif SERV_DUNGEON_STAGE_LOG
					//}}

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                    if ( m_iSubStageCount == 0 )
                    {
                        m_kTimer[TM_CHECK_CHANGE_HOST].restart();
                        m_bCheckPingScoreExecuted = false;
                    }
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

				}
			}
			break;

		case ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ:
			{
				SendDSSGoNextAllNot( m_kDSSGoNextAllNot );
			}
			break;

		case ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ:
			{
				if( m_spRoomUserManager->IsAllPlayerStageLoaded() == true )
				{
					//{{ 2010. 8. 16	최육사	서버 코드 정리
					if( DungeonSubStageStartProcess( GetSubStageID() ) == false )
					{
						START_LOG( cerr, L"던전 서브 스테이지 시작 에러!" )
							<< BUILD_LOG( GetSubStageID() )
							<< END_LOG;
					}
					//}}
				}
			}
			break;
		}
	}
}

void KDungeonRoom::CheckDungeonUnitInfoSuccess()
{
	if( m_spRoomUserManager->IsAllPlayerDungeonUnitInfoSet() == true )
	{
		int iPassedStageCount = 0;
		KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT kUnitInfoNot;
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		kUnitInfoNot.m_bIsClearDefenceDungeon = false;
		kUnitInfoNot.m_iProtectedSmallCrystal= 0;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가

		std::vector< KRoomUserPtr > vecKRoomUserPtr;
		std::vector< KERM_UPDATE_DUNGEON_UNIT_INFO_NOT > vecUpdateUnitInfo;
		std::vector< KEGS_GET_ITEM_NOT > vecResultGetItem;

		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		const int iProtectedCrystalCount = GetProtectedSmallCrystalCount() + 1; // 작은 수정의 수만 얻어 오기 때문 +1 큰 수정

		if( CXSLDungeon::IsDefenceDungeon( m_iDungeonID ) == true )
		{
			if( SiKResultProcess()->Result_DefenceDUNGEON(  m_spRoomUserManager, 
															m_kDungeonMonsterManager, 
															m_bIsWin, 
															m_eDungeonType, 
															m_cDungeonMode,
															m_cDifficultyLevel, 
															m_iDungeonID, 
															(int)GetTotalPlayTime(), 
															(int)GetRemainingPlayTime(), 
															m_nDropItemUID, 
															m_iStartedNumMember, 
															//{{ 2012. 04. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
															IsStartedByAutoParty(),
#endif SERV_BATTLE_FIELD_SYSTEM
															//}}
															iPassedStageCount, 
															kUnitInfoNot, 
															vecKRoomUserPtr, 
															vecUpdateUnitInfo,
															iProtectedCrystalCount ) == false )
			{
				START_LOG( cerr, L"던전결과 처리 실패.!" )
					<< END_LOG;
				return;
			}
		}
		else
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 2011. 04. 13  김민성  글로벌 서버 추가
		{
			if( SiKResultProcess()->Result_DUNGEON( m_spRoomUserManager, 
													m_kDungeonMonsterManager, 
													m_bIsWin, 
													m_eDungeonType, 
													m_cDungeonMode,
													m_cDifficultyLevel, 
													m_iDungeonID, 
													//{{ 2011. 08. 22	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
													GetStageID(),
#endif SERV_NEW_HENIR_TEST
													//}}
													(int)GetTotalPlayTime(), 
													(int)GetRemainingPlayTime(), 
													m_nDropItemUID, 
													m_iStartedNumMember, 
													//{{ 2012. 04. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
													IsStartedByAutoParty(),
#endif SERV_BATTLE_FIELD_SYSTEM
													//}}
													iPassedStageCount, 
													kUnitInfoNot, 
													vecKRoomUserPtr, 
													vecUpdateUnitInfo
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
													,m_kValentineManager.GetRemainTime( KValentineManager::TEM_AFTER_DUNGEON_START )
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
													) == false )
			{
				START_LOG( cerr, L"던전결과 처리 실패.!" )
					<< END_LOG;
				return;
			}

#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
            bool bIsDetectedHackUser = false;
            std::vector< KHenirHackUserData > vecHackUser;
            BOOST_TEST_FOREACH( const KRoomUserPtr, spRoomUser, vecKRoomUserPtr )
            {
                KHenirHackUserData kHackUser;
                if( spRoomUser == NULL )
                {
                    START_LOG( cerr, L"룸 유저의 포인터가 이상함." )
                        << END_LOG;
                    continue;
                }
                if( SiKCnHackingCheckManager()->IsHenirClearTimeCheckEnable() == true )
                {
                    if( SiKCnHackingCheckManager()->CheckHenirClearTimeHackUser( spRoomUser->GetAuthLevel(),
                        spRoomUser->GetUserUID(),
                        GetDungeonID(), 
                        //GetDifficultyLevel(),
                        //spRoomUser->GetUnitClass_LUA(), 
                        //spRoomUser->GetLevel(),
                        //m_bIsWin, 
                        //m_iStartedNumMember,
                        GetStageID(),
                        (int)spRoomUser->GetPlayTime()
                        ) == true )
                    {
                        bIsDetectedHackUser = true;
                        kHackUser.m_iUserUID = spRoomUser->GetUserUID();
                        kHackUser.m_iUnitUID = spRoomUser->GetCID();
                        kHackUser.m_iPlayTime = static_cast<int>( spRoomUser->GetPlayTime() );
                        kHackUser.m_iMemberNum = m_spRoomUserManager->GetNumMember();

                        vecHackUser.push_back( kHackUser );
                    }
                }
            }
            if ( bIsDetectedHackUser == true ) 
            {
                std::vector< KERM_UPDATE_DUNGEON_UNIT_INFO_NOT >::iterator vit;
                for ( vit = vecUpdateUnitInfo.begin(); vit != vecUpdateUnitInfo.end(); ++vit ) 
                {
                    vit->m_kHenirRankingInfo.clear();
                }

                SendToLogDB( ELOG_HENIR_HACKING_LOG_NOT, vecHackUser );
            }

#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		}

		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		if( CXSLDungeon::IsDefenceDungeon( m_iDungeonID ) == true )
		{
			kUnitInfoNot.m_bIsClearDefenceDungeon = m_bIsWin;
			kUnitInfoNot.m_iProtectedSmallCrystal= GetProtectedSmallCrystalCount();
		}
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가

		//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM		
		kUnitInfoNot.m_bComeBackUserInParty = m_spRoomUserManager->IsComeBackUserInRoom();
#endif SERV_SERVER_BUFF_SYSTEM
		//}

#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
		bool m_bSCHAR1 = false; 
		bool m_bSCHAR2 = false; 
		char m_CheckClass;
		char m_CheckType;

		int m_iRoomUserCnt = m_spRoomUserManager->GetLiveMember();

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
		std::vector<UidType> vecRelation;
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

		if(m_spRoomUserManager->GetLiveMember() != NULL)
		{
			KRoomUserPtr spRoomUserCheck = m_spRoomUserManager->GetUser( 0 );

			if( spRoomUserCheck != NULL)
			{
				// 상태 파악
				for(int rc = 0; rc < m_iRoomUserCnt; rc++)
				{
					spRoomUserCheck = m_spRoomUserManager->GetUser(rc);

					if( spRoomUserCheck != NULL)
					{
						m_CheckType = spRoomUserCheck->GetUnitType();
						m_CheckClass = spRoomUserCheck->GetUnitClass_LUA();
					}

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
					// 상태 파악
					KRoomUserInfo kInfo;
					spRoomUserCheck->GetRoomUserInfo( kInfo );

					if( kInfo.m_bCouple == true && kInfo.m_iRelationTargetUserUid > 0 )
					{
						vecRelation.push_back( kInfo.m_iRelationTargetUserUid );
					}
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

					// 상수 참조하도록 수정
					if( _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::cUnitType == 0 ||
						_CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::cUnitType == m_CheckType )
					{
						m_bSCHAR1 = true;
					}
					if( _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::cUnitClass == 0 ||
						_CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::cUnitClass == m_CheckClass )
					{
						m_bSCHAR2 = true;
					}
				}
			}
		}		
		
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
		kUnitInfoNot.m_vecRelation = vecRelation;
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
		kUnitInfoNot.m_bSpecChar1 = m_bSCHAR1; // 엘소드 인지 파악
		kUnitInfoNot.m_bSpecChar2 = m_bSCHAR2; // 시스나이트 인지 파악
#endif SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT

#ifdef SERV_THREE_COLOR_EVENT
		int iRoomUserCnt = m_spRoomUserManager->GetLiveMember();

		if( m_spRoomUserManager->GetLiveMember() != NULL )
		{
			KRoomUserPtr spRoomUserCheck = m_spRoomUserManager->GetUser( 0 );

			if( spRoomUserCheck != NULL )
			{
				for( int rc = 0; rc < iRoomUserCnt; rc++ )
				{
					KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser(rc);

					if( spRoomUser != NULL )
					{
						KRoomUserInfo kInfo;
						spRoomUser->GetRoomUserInfo( kInfo );

						if( kInfo.m_iTitleID == 5470 || kInfo.m_iTitleID == 5480 || kInfo.m_iTitleID == 5490 )
						{
							std::set< int >::iterator sit = kUnitInfoNot.m_setEquippedTitle.find( kInfo.m_iTitleID );
							if( sit == kUnitInfoNot.m_setEquippedTitle.end() )
							{
								kUnitInfoNot.m_setEquippedTitle.insert( kInfo.m_iTitleID );
							}
						}
					}
				}
			}
		}
#endif SERV_THREE_COLOR_EVENT

		BroadCast( ERM_END_GAME_DUNGEON_RESULT_DATA_NOT, kUnitInfoNot );

		//게임서버로 각 UNIT 자신의 결과값 업데이트
		BroadCastUpdateUnitInfoWithPartyCheck( vecKRoomUserPtr, ERM_UPDATE_DUNGEON_UNIT_INFO_NOT, vecUpdateUnitInfo, (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)) );
		m_spRoomUserManager->ResetDungeonUnitInfo();

		//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		// 어둠의 문이 시작되는 조건을 만족하는가?
		if( m_kDefenceDungeonManager.IsEnterDefenceDungeon() == true  &&  CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) == false )
		{
			// 그렇다면 어둠의 문을 준비하자!
			PrepareForEnterDefenceDungeon();
		}
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

		//{{ 2009. 7. 27  최육사	헤니르 시공 클리어시 전체공지
		if( m_eDungeonType == CXSLDungeon::DT_HENIR  &&  
			m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE  &&
			SiKHenirResultTable()->IsClearNotifyStage( iPassedStageCount ) == true  &&
			m_bIsWin == true
#ifdef SERV_HENIR_NO_NOTIFY
			&& SiKHenirResultTable()->GetEnbleNotify() == true
#endif // SERV_HENIR_NO_NOTIFY
			)
		{
			// 전체 공지 날린다!
			KEGS_NOTIFY_MSG_NOT kNotMsg;
			kNotMsg.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_HENIR_CLEAR;
			kNotMsg.m_Count = 1;

			BOOST_TEST_FOREACH( const KRoomUserPtr, spRoomUser, vecKRoomUserPtr )
			{
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"룸 유저의 포인터가 이상함." )
						<< END_LOG;
					continue;
				}

				kNotMsg.m_wstrMSG += L"[";
				kNotMsg.m_wstrMSG += spRoomUser->GetNickName();
				kNotMsg.m_wstrMSG += L"]";

				// 클리어 로그
				KELOG_HENIR_DUNGEON_CLEAR_NOT kPacketLog;
				kPacketLog.m_iUserUID = spRoomUser->GetUserUID();
				kPacketLog.m_iUnitUID = spRoomUser->GetCID();
				kPacketLog.m_iPlayTime = static_cast<int>(GetTotalPlayTime());
				SendToLogDB( ELOG_HENIR_DUNGEON_CLEAR_NOT, kPacketLog );
			}
			
			// 센터 서버로 날림
			KncSend( PI_CN_ROOM, GetUID(), PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, ECN_NOTIFY_MSG_NOT, kNotMsg );
		}
		//}}

		// 던전 일일 통계 데이터 기록
		for ( unsigned int iR = 0; iR < vecKRoomUserPtr.size(); ++iR )
		{
			const KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfo = vecUpdateUnitInfo[iR];
			const KDungeonUnitResultInfo& kDungeonUnitInfo = kUnitInfoNot.m_vecDungeonUnitInfo[iR];

			KRoomUserPtr spRoomUser = vecKRoomUserPtr[iR];
			if( spRoomUser == NULL )
			{
				START_LOG( cerr, L"룸 유저의 포인터가 이상함." )
					<< END_LOG;
				continue;
			}

			//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			const int iClearValue = ( m_bIsWin ? KE_LOCAL_LOG_DUNGEON_NOT::DCT_SUCCESS : KE_LOCAL_LOG_DUNGEON_NOT::DCT_FAILED );
			
			// 던전 로그
			LogToDB_DungeonLog( spRoomUser, iClearValue, kUpdateUnitInfo, kDungeonUnitInfo );
#else
//			// 운영자는 통계를 남기지 않습니다.
//			//if( spRoomUser->GetAuthLevel() >= SEnum::UAL_GM )
//			//{
//			//	START_LOG( cout, L"운영자는 통계데이터를 남기지 않습니다." )
//			//		<< BUILD_LOGc( spRoomUser->GetAuthLevel() )
//			//		<< BUILD_LOG( spRoomUser->GetNickName() );
//			//	continue;
//			//}
//
//			KRoomUserInfo kInfo;
//			spRoomUser->GetRoomUserInfo( kInfo );
//
//			//{{ 2010. 06. 21  최육사	통계 스레드
//			//#ifdef SERV_STATISTICS_THREAD
//			//{{ 2010. 11. 8	최육사	던전 로그 DB
//			//#ifdef SERV_DUNGEON_LOG_DB
//			CTime kRegDate = CTime::GetCurrentTime();
//			//#endif SERV_DUNGEON_LOG_DB
//			//}}
//			//////////////////////////////////////////////////////////////////////////
//			KE_LOCAL_LOG_DUNGEON_NOT kNot;
//			kNot.m_iDungeonID				= (m_iDungeonID + static_cast<int>(m_cDifficultyLevel));
//			kNot.m_nUnitUID					= kInfo.m_nUnitUID;
//			kNot.m_wstrNickName				= kInfo.m_wstrNickName;
//			kNot.m_iDifficultyLevel			= (int) m_cDifficultyLevel;
//			kNot.m_iIsChallenge				= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
//			kNot.m_iUnitClass				= (int) kInfo.m_cUnitClass;
//			kNot.m_iClear					= ( m_bIsWin ? KE_LOCAL_LOG_DUNGEON_NOT::DCT_SUCCESS : KE_LOCAL_LOG_DUNGEON_NOT::DCT_FAILED );
//			kNot.m_iStartNumMember			= m_iStartedNumMember;
//			kNot.m_iEndNumMember			= GetNumMember();
//			kNot.m_ucLevel					= kInfo.m_ucLevel;
//			kNot.m_iPlayTime				= (int) GetTotalPlayTime();
//			kNot.m_iEXP						= kUpdateUnitInfo.m_iEXP;
//			kNot.m_iED						= spRoomUser->GetRewardED();
//			kNot.m_cComboRank				= kDungeonUnitInfo.m_cComboRank;
//			kNot.m_iComboScore				= spRoomUser->GetComboScore_LUA();
//			kNot.m_cTechnicalRank			= kDungeonUnitInfo.m_cTechnicalRank;
//			kNot.m_iTechnicalScore			= spRoomUser->GetTechnicalScore_LUA();
//			kNot.m_cTimeRank				= kDungeonUnitInfo.m_cTimeRank;
//			kNot.m_cDamagedRank				= kDungeonUnitInfo.m_cDamagedRank;			
//			kNot.m_iDamageNum				= spRoomUser->GetDamageNum_LUA();
//			kNot.m_cTotalRank				= kDungeonUnitInfo.m_cTotalRank;
//			kNot.m_iRessurectionStoneCount	= spRoomUser->GetUsedRessurectionStoneCount();
//			kNot.m_iPassedStageCount		= spRoomUser->GetPassedStageCount();
//			kNot.m_iPassedSubStageCount		= spRoomUser->GetPassedSubStageCount();
//			//{{ 2010. 9. 14	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//			kNot.m_bIsWithPet				= spRoomUser->HavePet();
//#endif SERV_PET_SYSTEM
//			//}}
//			//{{ 2010. 11. 8	최육사	던전 로그 DB
//#ifdef SERV_DUNGEON_LOG_DB
//			kNot.m_wstrRegDate				= (const wchar_t*)kRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
//#endif SERV_DUNGEON_LOG_DB
//			//}}
//			//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
//			kNot.m_sNpcDieCount				= spRoomUser->GetKillNPC_LUA();
//			kNot.m_iGivenDamage				= spRoomUser->GetTotalGivenDamamge_LUA();
//			kNot.m_iAttackDamage			= spRoomUser->GetTotalAttackedDamage_LUA();
//			kNot.m_sUsingSkillKind			= spRoomUser->GetUsingKindOfSkill_LUA();
//			kNot.m_iTotalEXP				= ( spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP() + kUpdateUnitInfo.m_iEXP );
//			kNot.m_cPetID					= spRoomUser->GetPetID();
//			kNot.m_cPetEvoStep				= spRoomUser->GetPetEvolutionStep();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN
//			//}}
//			//{{ 2012. 03. 05	김민성	던전 대시 로그
//#ifdef SERV_DUNGEON_DASH_LOG
//			kNot.m_iDashCount				= spRoomUser->GetDashCount();
//#endif SERV_DUNGEON_DASH_LOG
//			//}}
//			//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//			kNot.m_iBaseHP					= spRoomUser->GetBaseHP_LUA();
//			kNot.m_iEndHP					= spRoomUser->GetLastUpdatedHP();
//			kNot.m_iRecovHP					= spRoomUser->GetTotalRecoveryHP_LUA();
//			kNot.m_iContinue				= spRoomUser->GetUsedRessurectionStoneCount();
//			kNot.m_iStartContinue			= spRoomUser->GetStartTimeNumResurrectionStone();
//			kNot.m_iContinueUseStage		= spRoomUser->GetFirstUseResurrectionStoneStageID();
//			kNot.m_iContinueUseSub			= spRoomUser->GetFirstUseResurrectionStoneSubStageID();
//			kNot.m_iFrame					= spRoomUser->GetInGameAverageFrame();
//			kNot.m_wstrChannelIP			= spRoomUser->GetGameChannelIP();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//			//}}
//			//{{ 2012. 07. 11 김민성
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
//			kNot.m_iUserUID					= spRoomUser->GetUserUID();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
//			//}}
//			//{{ 2012. 11. 6	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//			kNot.m_cAutoPartyPlay			= ( IsStartedByAutoParty() ? 1 : 0 );
//#endif SERV_BATTLE_FIELD_SYSTEM
//			//}}
//			KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_NOT, kNot );
//			//////////////////////////////////////////////////////////////////////////
//			//{{ 2010. 11. 8	최육사	던전 로그 DB
//			//#ifdef SERV_DUNGEON_LOG_DB
//			SendToLogDB( DBE_DB_LOG_DUNGEON_NOT, kNot );
//			//#endif SERV_DUNGEON_LOG_DB
//			//}}
//			//#endif SERV_STATISTICS_THREAD
//			//}}
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}			

			//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG			
			{
				KELOG_DUNGEON_STAGE_LOG_NOT kPacketToLog;
				kPacketToLog.m_iDungeonID		  = GetDungeonID();
				kPacketToLog.m_iStageID			  = m_iLoadingCompleteStageID; // 이전 스테이지
				kPacketToLog.m_iUnitUID			  = spRoomUser->GetCID();
				kPacketToLog.m_cLevel			  = spRoomUser->GetLevel();
				kPacketToLog.m_cDifficulty		  = GetDifficultyLevel();
				kPacketToLog.m_bIsChallenge 	  = ( m_cDungeonMode == CXSLDungeon::DM_HENIR_PRACTICE );
				kPacketToLog.m_cUnitClass		  = spRoomUser->GetUnitClass_LUA();
				kPacketToLog.m_iStagePlayTime	  = static_cast<int>(spRoomUser->GetStagePlayTime()); // 해당 스테이지를 플레이한 플레이 타임
				kPacketToLog.m_iStageDamage		  = ( spRoomUser->GetTotalAttackedDamage_LUA() - spRoomUser->GetStageDamage() ); // 해당 스테이지 동안 받은 대미지
				kPacketToLog.m_iResurrectionCount = spRoomUser->GetUsedRessurectionStoneCount();
				kPacketToLog.m_wstrRegDate		  = KncUtil::GetCurrentTimeString();
				SendToLogDB( ELOG_DUNGEON_STAGE_LOG_NOT, kPacketToLog );
			}
#endif SERV_DUNGEON_STAGE_LOG
			//}}

			//{{ 2007. 12. 18  최육사  유저 통계 [수정]
			{
				KERM_DUNGEON_USER_STATISTICS_NOT kPacketNot;
				kPacketNot.m_iDungeonID	   = m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
				kPacketNot.m_cGameResult   = ( m_bIsWin == true ? KERM_DUNGEON_USER_STATISTICS_NOT::UGR_WIN : KERM_DUNGEON_USER_STATISTICS_NOT::UGR_LOSE );
				kPacketNot.m_bIsPartyPlay  = ( m_iStartedNumMember > 1 );
				kPacketNot.m_iGamePlayTime = static_cast<int>(GetTotalPlayTime());
				kPacketNot.m_iUsedResurrectionStone = spRoomUser->GetUsedRessurectionStoneCount();
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DUNGEON_USER_STATISTICS_NOT, kPacketNot );
			}
			//}}

			//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
			if( SiKCnHackingCheckManager()->IsDungeonResultCheckEnable() == true )
			{
				if( SiKCnHackingCheckManager()->CheckDungeonResultAutoHackUser( GetDungeonID(), 
																		GetDifficultyLevel(),
																		spRoomUser->GetUnitClass_LUA(), 
																		spRoomUser->GetLevel(),
																		m_bIsWin, 
																		m_iStartedNumMember,
																		static_cast<CXSLDungeon::RANK_TYPE>(kDungeonUnitInfo.m_cComboRank), 
																		static_cast<CXSLDungeon::RANK_TYPE>(kDungeonUnitInfo.m_cTechnicalRank), 
																		static_cast<CXSLDungeon::RANK_TYPE>(kDungeonUnitInfo.m_cTimeRank),
																		static_cast<CXSLDungeon::RANK_TYPE>(kDungeonUnitInfo.m_cDamagedRank), 
																		spRoomUser->GetTechnicalScore_LUA(), 
																		spRoomUser->GetUsedRessurectionStoneCount(), 
																		spRoomUser->HavePet()
																		) == true )
				{
					// 해킹 체크된 유저에게 패킷을 보내자!
					//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
					KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
					kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_RESULT;
#else
					KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
					kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CLEAR_RESULT;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
					//}}
	
					SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

					START_LOG( clog, L"Dungeon Result 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
						<< BUILD_LOG( spRoomUser->GetGSUID() )
						<< BUILD_LOG( spRoomUser->GetCID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( spRoomUser->GetNickName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetDungeonID() )
						<< BUILD_LOGc( GetDifficultyLevel() )
						<< BUILD_LOGc( spRoomUser->GetUnitClass_LUA() )
						<< BUILD_LOGc( spRoomUser->GetLevel() )
						<< BUILD_LOG( m_bIsWin )
						<< BUILD_LOG( m_iStartedNumMember )
						<< BUILD_LOGc( kDungeonUnitInfo.m_cComboRank )
						<< BUILD_LOGc( kDungeonUnitInfo.m_cTechnicalRank )
						<< BUILD_LOGc( kDungeonUnitInfo.m_cTimeRank )
						<< BUILD_LOGc( kDungeonUnitInfo.m_cDamagedRank )
						<< BUILD_LOG( spRoomUser->GetTechnicalScore_LUA() )
						<< BUILD_LOG( spRoomUser->GetUsedRessurectionStoneCount() )
						<< BUILD_LOG( spRoomUser->HavePet() );
				}
			}
#endif SERV_CN_SERVER_HACKING_CHECK
			//}}
			//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
			if( SiKCnHackingCheckManager()->IsDungeonClearTimeCheckEnable() == true )
			{
				if( SiKCnHackingCheckManager()->CheckDungeonClearTimeAutoHackUser( spRoomUser->GetAuthLevel(),
																				spRoomUser->GetUserUID(),
																				GetDungeonID(), 
																				GetDifficultyLevel(),
																				spRoomUser->GetUnitClass_LUA(), 
																				spRoomUser->GetLevel(),
																				m_bIsWin, 
																				m_iStartedNumMember,
																				(int)spRoomUser->GetPlayTime()
																				) == true )
				{
					// 해킹 체크된 유저에게 패킷을 보내자!
					//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
					KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
					kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_TIME;
#else
					KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
					kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CLEAR_TIME;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
					//}}
					SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

					// 해킹 체크된 유저를 DB에 로그로 남기자!
					CTime kCurTime = CTime::GetCurrentTime();
					KDBE_REG_DUNGEON_CLEAR_TIME_HACK_USER_NOT kNotReg;
					kNotReg.m_iUserUID = spRoomUser->GetUserUID();
					kNotReg.m_wstrRegDate = (std::wstring)kCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
					SendToLogDB( DBE_REG_DUNGEON_CLEAR_TIME_HACK_USER_NOT, kNotReg );

					START_LOG( clog, L"Dungeon Clear Time 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
						<< BUILD_LOG( spRoomUser->GetGSUID() )
						<< BUILD_LOG( spRoomUser->GetUserUID() )
						<< BUILD_LOG( spRoomUser->GetCID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( spRoomUser->GetNickName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetDungeonID() )
						<< BUILD_LOGc( GetDifficultyLevel() )
						<< BUILD_LOGc( spRoomUser->GetUnitClass_LUA() )
						<< BUILD_LOGc( spRoomUser->GetLevel() )
						<< BUILD_LOG( m_bIsWin )
						<< BUILD_LOG( m_iStartedNumMember )
						<< BUILD_LOG( spRoomUser->GetPlayTime() );
				}
			}
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
			//}}
		}

		//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
		CTime kEndGameTime = CTime::GetCurrentTime();
		//////////////////////////////////////////////////////////////////////////
		KE_LOCAL_LOG_DUNGEON_ROOM_NOT kNot;
		kNot.m_iDungeonID		= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
		kNot.m_iDifficultyLevel	= (int) GetDifficultyLevel();
		kNot.m_iIsChallenge		= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
		kNot.m_iWin				= ( m_bIsWin ? 1 : 0 );
		kNot.m_iLose			= ( m_bIsWin ? 0 : 1 );
		kNot.m_iStartNumMember	= m_iStartedNumMember;
		kNot.m_iEndNumMember	= GetNumMember();
		kNot.m_iTotalPlayTime	= static_cast<int>(GetTotalPlayTime());
		kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_ROOM_NOT, kNot );
		//////////////////////////////////////////////////////////////////////////
#endif SERV_STATISTICS_THREAD
		//}}
	}
}

//{{ 2009. 7. 27  최육사	헤니르 시공
void KDungeonRoom::CheckEnterSecretStageSuccess()
{
	if( GetStateID() != KRoomFSM::S_PLAY )
		return;

	// 모든 유저가 의사 표시를 하였는가?
	if( m_spRoomUserManager->IsAllPlayerAgreed() == true )
	{
		// 그렇다면 바로 이동처리!
		KEGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT kNot;
		kNot.m_bProceedToEnterSecretStage = m_spRoomUserManager->IsEnterSecretStage(); // 비밀 스테이지 입장 여부
		BroadCast( ERM_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT, kNot );

		// 유저 의견 정보 초기화
		m_spRoomUserManager->ResetAgreeEnterSecretStage();

		// 유저 의견 타이머 삭제
		//m_kEnterSecretStageTimer.KillTimer();

		// 비밀 스테이지 입장 성공이라면 플레이 시간을 5분 더 늘려주자!
		//## des second(hoons) : 비밀 스테이지 라고 추가로 시간을 주는 기능 없어짐.
		//if( kNot.m_bProceedToEnterSecretStage == true )
		//{
		//	SetRemainingPlayTime( m_fRemainingPlayTime + 300.f );
		//	BroadCast( ERM_REMAINING_PLAY_TIME_NOT, GetRemainingPlayTime() );
		//}
	}	
}
//}}

//{{ 2010. 8. 16	최육사	서버 코드 정리
bool KDungeonRoom::DungeonSubStageStartProcess( IN int iSubStageID )
{
	bool bSuccess = true;

	// Loading 변수 리셋.
	m_spRoomUserManager->ResetStageLoaded();

	KEGS_DUNGEON_SUB_STAGE_START_NOT kNot;
	kNot.m_iSubStageID = iSubStageID;
	BroadCast( ERM_DUNGEON_SUB_STAGE_START_NOT, kNot );

//////////////////////////////////////////////////////////////////////////
	//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	const int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( GetDungeonIDAndDif() );
	if( m_spRoomUserManager->GetNumMember() == 1 && GetHighestUserLevelAtDungeonStart() - iDungeonMinLevel < 10  )
	{
		// 현재 서브 스테이지의 액티브 몬스터 수 초기화
		m_iSubStageActiveNpc = 0;
		std::map< int, int >::const_iterator mit;
		mit = m_mapSubStageExpNpcNum.find( iSubStageID );

		if( mit != m_mapSubStageExpNpcNum.end() )
			m_iSubStageActiveNpc = mit->second;
		
		// 서브 스테이지에서 잡은 몬스터 수를 초기화
		m_iSubStageKillActiveNpc = 0;

		// 서브 스테이지에서 처음 몬스터 잡은 시간을 초기화	
		m_tSubStageFirstKillTime = CTime( 0 );
	}
	else
	{
		m_iSubStageActiveNpc = 0;
		m_iSubStageKillActiveNpc = 0;
		m_tSubStageFirstKillTime = CTime( 0 );
	}
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}
//////////////////////////////////////////////////////////////////////////


	//여기까지 로딩시간을 체크함.
	const double fLoadingTimerEnd = m_LoadingTimer.End();
	m_fLoadingTime += fLoadingTimerEnd;	

	// 혹시나 로딩 타임이 플레이 타임보다 더 크게 나오는지 검사해본다!
	if( m_kTimer[TM_PLAY].elapsed() < GetLoadingTime() )
	{
		START_LOG( cout, L"로딩 타임이 던전 플레이 타임보다 더 크게 나오는 이상한 현상이 발생했다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
			<< BUILD_LOG( GetLoadingTime() )
			<< BUILD_LOG( fLoadingTimerEnd )
			<< BUILD_LOG( GetDungeonIDAndDif() )
			<< BUILD_LOG( GetStageID() )
			<< BUILD_LOG( GetSubStageID() )
			<< BUILD_LOG( m_spRoomUserManager->GetNumMember() )
			<< BUILD_LOG( m_iStartedNumMember )
			<< END_LOG;

		bSuccess = false;
	}	

	//{{ 2012. 02. 28	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
	//if( IsFirstSubStage() == false )
	//{
	//	IncreasePartyFever();
	//}
//#else
//	if( m_iPartyUID > 0  &&  
//		m_spRoomUserManager->GetNumMember() > 1  &&  
//		IsFirstSubStage() == false )
//	{
//		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( 0 ); // 0번 인덱스의 유저
//		if( spRoomUser != NULL )
//		{
//			KERM_INCREASE_PARTY_FEVER_NOT kNot;
//			kNot.m_iPartyUID = m_iPartyUID;
//			SendToGSServer( spRoomUser->GetGSUID(), ERM_INCREASE_PARTY_FEVER_NOT, kNot );
//		}
//		else
//		{
//			START_LOG( cerr, L"유저가 한명도 없는방인감?" )
//				<< BUILD_LOG( GetStateIDString() )
//				<< END_LOG;
//
//			bSuccess = false;
//		}
//	}
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}


#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    m_iSubStageCount++;
    if ( m_iSubStageCount == 2 && m_bCheckPingScoreExecuted == false )
    {
		CheckPingScore();
		m_kTimer[TM_CHECK_CHANGE_HOST].restart();
    }
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

	return bSuccess;
}
//}}

//{{ 2010. 03. 24  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL

void KDungeonRoom::NormalDungeonAttribMonsterDrop( IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot )
{
	int	iRandomSubStage = -1;
	bool bIsBossStage = false;
	bool bDropAttribNpc = false;

	// 1. 속성NPC 드롭카운트가 0이면 출현시키지 않는다.
	if( m_iAttribNpcStageCnt <= 0 )
		return;

	// 2. 초보자숲, 튜토리얼, 훈련소에서는 엘리트 몬스터 안나옴
	if( m_iDungeonID == SEnum::DI_EL_FOREST_GATE_NORMAL  ||
		//{{ 2011. 09. 28  임규수		할로윈 던전
#ifdef SERV_DUNGEON_STAGE_RANDOM_NPC_POS
		m_iDungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL ||
#endif SERV_DUNGEON_STAGE_RANDOM_NPC_POS
		//}}
		CXSLDungeon::IsTutorialDungeon( m_iDungeonID ) == true  ||
		CXSLDungeon::IsTCDungeon( m_iDungeonID ) == true )
	{
		return;
	}

	// 3. 해당 스테이지에서 출현할지를 결정
	bDropAttribNpc = ( (rand() % 2) == 0 );
	if( bDropAttribNpc == false )
	{
		return;
	}

	// 4. 어느 서브 스테이지에 엘리트 몬스터가 등장할지 결정
	if( kNot.m_mapNPCData.size() > 0 )
	{
		iRandomSubStage = rand() % static_cast<int>(kNot.m_mapNPCData.size());
	}

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	const int iPlayerCount = m_spRoomUserManager->GetNumPlaying();
	int tempAttribNpcCnt = 0;				// m_iAttribNpcCnt 는 게임 시작시에만 적용 되게 한다.
	switch( m_eDungeonType )
	{
	case CXSLDungeon::DT_NORMAL:
		{
			// 엘의 나무에서는 속성 몬스터 안나온다.
			if( SEnum::DI_RUBEN_EL_TREE_NORMAL != GetDungeonID() )
			{
				SiKAttribNpcTable()->GetAttribNpcDropCount( static_cast<char>(m_eDungeonType), m_cDifficultyLevel, iPlayerCount, tempAttribNpcCnt, m_iAttribNpcDropCnt );
			}
		}break;
	case CXSLDungeon::DT_HENIR: // 헤니르 시공에서는 속성몬스터 안나옴.
		break;
	case CXSLDungeon::DT_SECRET:
		{
			SiKAttribNpcTable()->GetAttribNpcDropCount( static_cast<char>(m_eDungeonType), m_cDungeonMode, iPlayerCount, tempAttribNpcCnt, m_iAttribNpcDropCnt );
		}break;
	default:
		{
			START_LOG( cerr, L"잘못된 던전 타입입니다." )
				<< BUILD_LOG( m_eDungeonType )
				<< END_LOG;
		}
		break;
	}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	// 속성 몬스터 설정하기
	std::map< int, KNPCList >::const_iterator mit;
	for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
	{
		// 속성 몬스터를 결정할 KNpcList의 Index리스트
		std::set< int > setAttribNpc;
		
		const KNPCList& kNPCList = mit->second;
		for( u_int ui = 0; ui < kNPCList.m_NPCList.size(); ++ui )
		{
			if( kNPCList.m_NPCList[ui].m_bHasBossGage )
				bIsBossStage = true;

			const CXSLUnitManager::NPCUnitTemplet* pNpcTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( static_cast<CXSLUnitManager::NPC_UNIT_ID>(kNPCList.m_NPCList[ui].m_NPCID) );
			if( pNpcTemplet == NULL )
			{
				START_LOG( cerr, L"npc templet을 찾을수 없습니다." )
					<< BUILD_LOG( GetDungeonID() )
					<< BUILD_LOGc( GetDifficultyLevel() )
					<< BUILD_LOG( kNPCList.m_NPCList[ui].m_NPCID )
					<< BUILD_LOG( ui )
					<< END_LOG;
				continue;
			}

			if( pNpcTemplet->m_ClassType != CXSLUnitManager::NCT_BASIC )
				continue;

			// 속성NPC를 선택하기위한 Pool
			if( SiKAttribNpcTable()->IsNoDropNpc( kNPCList.m_NPCList[ui].m_NPCID ) )
				continue;

			setAttribNpc.insert( ui );
		}

		//////////////////////////////////////////////////////////////////////////
		// 속성 NPC 드롭 - npcUID 발급후에 처리할것!

		// 1. 한 스테이지에 한번만..
		if( !bDropAttribNpc )
			continue;

		// 2. 보스 스테이지에서는 속성 npc 등장 안하도록
		if( bIsBossStage )
			continue;

		// 3. 속성NPC 드롭카운트
		if( m_iAttribNpcStageCnt <= 0 )
			continue;

		// 4. 해당 SubStage에서 드롭여부 결정
		if( iRandomSubStage != mit->first )
			continue;

		// 5. 랜덤 대상이 될 npc가 있나 검사
		if( setAttribNpc.empty() )
			continue;

		// 6. 속성NPC 결정 m_iAttribNpcDropCnt : 속성 NPC count
		for( int iDropCnt = 0; iDropCnt < m_iAttribNpcDropCnt; ++iDropCnt )
		{
			if( setAttribNpc.size() <= 0 )
				break;

			// 6-1. SubStage의 Npc리스트에서 랜덤 카운트 얻기
			int iNpcIdx = rand() % static_cast<int>(setAttribNpc.size());

			// 6-2. 해당 Npc Index얻기
			int iCnt = 0;
			std::set< int >::const_iterator sit;
			for( sit = setAttribNpc.begin(); sit != setAttribNpc.end(); ++sit )
			{
				if( iNpcIdx == iCnt )
				{
					iNpcIdx = *sit;
					break;
				}

				++iCnt;
			}
			setAttribNpc.erase( iNpcIdx );

			// 6-3. 예외처리
			if( iNpcIdx < 0  ||  iNpcIdx >= static_cast<int>(kNPCList.m_NPCList.size()) )
				continue;

			// 6-4. Attrib Npc 정보 얻기
			const KNPCUnitReq& kNpcInfo = kNPCList.m_NPCList[iNpcIdx];

			KAttribEnchantNpcInfo kAttribNpcInfo;

			if( m_eDungeonType == CXSLDungeon::DT_NORMAL )
			{
				if( SiKAttribNpcTable()->GetAttribNpcInfo( m_eDungeonType, m_cDifficultyLevel, kNpcInfo.m_Level, kNpcInfo.m_NPCID, kAttribNpcInfo ) == false )
				{
					START_LOG( cerr, L"속성 NPC정보 얻기 실패!" )
						<< BUILD_LOG( GetDungeonID() )
						<< BUILD_LOG( m_eDungeonType )
						<< BUILD_LOGc( m_cDifficultyLevel )
						<< BUILD_LOGc( kNpcInfo.m_Level )
						<< BUILD_LOG( kNpcInfo.m_NPCID )
						<< END_LOG;
					continue;
				}
			}
			else
			{
				if( SiKAttribNpcTable()->GetAttribNpcInfo( m_eDungeonType, m_cDungeonMode, kNpcInfo.m_Level, kNpcInfo.m_NPCID, kAttribNpcInfo ) == false )
				{
					START_LOG( cerr, L"속성 NPC정보 얻기 실패!" )
						<< BUILD_LOG( GetDungeonID() )
						<< BUILD_LOG( m_eDungeonType )
						<< BUILD_LOGc( m_cDungeonMode )
						<< BUILD_LOGc( kNpcInfo.m_Level )
						<< BUILD_LOG( kNpcInfo.m_NPCID )
						<< END_LOG;
					continue;
				}
			}

			kNot.m_mapAttribNpcInfo.insert( std::make_pair( kNpcInfo.m_UID, kAttribNpcInfo ) );

			// 6-5. 해당 속성 몬스터의 보상을 위해 정보 보관
			m_mapAttribNpcData.insert( std::make_pair( kNpcInfo.m_UID, static_cast<int>(kNpcInfo.m_Level) ) );
		}

		// 7. 해당 SubStage에 속성NPC 등장 시켰으므로 카운트 1삭제
		--m_iAttribNpcStageCnt;

		// 8. 엘리트 몬스터 출현시켜 줬으니 나머지 서브스테이지는 출현 시키지 말자~!
		bDropAttribNpc = false;

		//////////////////////////////////////////////////////////////////////////
	}
}

void KDungeonRoom::SecretDungeonExtraStageAttribMonsterDrop( IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot )
{
	bool bIsBossStage = false;

	// 속성 몬스터 설정하기
	std::map< int, KNPCList >::const_iterator mit;
	for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
	{
		// 속성 몬스터를 결정할 KNpcList의 Index리스트
		std::set< int > setAttribNpc;

		const KNPCList& kNPCList = mit->second;
		for( u_int ui = 0; ui < kNPCList.m_NPCList.size(); ++ui )
		{
			if( kNPCList.m_NPCList[ui].m_bHasBossGage )
				bIsBossStage = true;

			const CXSLUnitManager::NPCUnitTemplet* pNpcTemplet = SiCXSLUnitManager()->GetNPCUnitTemplet( static_cast<CXSLUnitManager::NPC_UNIT_ID>(kNPCList.m_NPCList[ui].m_NPCID) );
			if( pNpcTemplet == NULL )
			{
				START_LOG( cerr, L"npc templet을 찾을수 없습니다." )
					<< BUILD_LOG( GetDungeonID() )
					<< BUILD_LOGc( GetDifficultyLevel() )
					<< BUILD_LOG( kNPCList.m_NPCList[ui].m_NPCID )
					<< END_LOG;
				continue;
			}

			if( pNpcTemplet->m_ClassType != CXSLUnitManager::NCT_BASIC )
				continue;

			// 속성NPC를 선택하기위한 Pool
			if( SiKAttribNpcTable()->IsNoDropNpc( kNPCList.m_NPCList[ui].m_NPCID ) )
				continue;

			setAttribNpc.insert( ui );
		}

		// 1. 보스 스테이지에서는 속성 npc 등장 안하도록 정말?
		if( bIsBossStage )
			continue;

		// 2. 랜덤 대상이 될 npc가 있나 검사
		if( setAttribNpc.empty() )
			continue;

		// 3. 속성NPC 결정
#ifdef _CONVERT_VS_2010
		const int iAttribNpcDropCnt = setAttribNpc.size() / 2; // 해당 서브 스테이지의 npc수에서 2분의 1을 속성몬스터로 설정!
#else
		const iAttribNpcDropCnt = setAttribNpc.size() / 2; // 해당 서브 스테이지의 npc수에서 2분의 1을 속성몬스터로 설정!
#endif _CONVERT_VS_2010

		for( int iDropCnt = 0; iDropCnt < iAttribNpcDropCnt; ++iDropCnt )
		{
			if( setAttribNpc.size() <= 0 )
				break;

			// 4-1. SubStage의 Npc리스트에서 랜덤 카운트 얻기
			int iNpcIdx = rand() % static_cast<int>(setAttribNpc.size());

			// 4-2. 해당 Npc Index얻기
			int iCnt = 0;
			std::set< int >::const_iterator sit;
			for( sit = setAttribNpc.begin(); sit != setAttribNpc.end(); ++sit )
			{
				if( iNpcIdx == iCnt )
				{
					iNpcIdx = *sit;
					break;
				}

				++iCnt;
			}
			setAttribNpc.erase( iNpcIdx );

			// 4-3. 예외처리
			if( iNpcIdx < 0  ||  iNpcIdx >= static_cast<int>(kNPCList.m_NPCList.size()) )
				continue;

			// 4-4. Attrib Npc 정보 얻기
			const KNPCUnitReq& kNpcInfo = kNPCList.m_NPCList[iNpcIdx];

			KAttribEnchantNpcInfo kAttribNpcInfo;
			if( SiKAttribNpcTable()->GetAttribNpcInfoExtraStage( kNpcInfo.m_NPCID, kAttribNpcInfo ) == false )
			{
				START_LOG( cerr, L"속성 NPC정보 얻기 실패!" )
					<< BUILD_LOG( kNpcInfo.m_NPCID )
					<< END_LOG;
				continue;
			}

			kNot.m_mapAttribNpcInfo.insert( std::make_pair( kNpcInfo.m_UID, kAttribNpcInfo ) );

			// 4-5. 해당 속성 몬스터의 보상을 위해 정보 보관
			m_mapAttribNpcData.insert( std::make_pair( kNpcInfo.m_UID, static_cast<int>(kNpcInfo.m_Level) ) );
		}
	}
}

#endif SERV_SECRET_HELL
//}}

bool KDungeonRoom::IsAttribNpc( IN int iNpcUID, OUT int& iNpcLv )
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapAttribNpcData.find( iNpcUID );
	if( mit == m_mapAttribNpcData.end() )
		return false;

	iNpcLv = mit->second;
	return true;
}

bool KDungeonRoom::IsDropDieState( char cState )
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

//{{ 2009. 12. 14  최육사	헤니르임시인벤
void KDungeonRoom::CheckTempInventoryClear()
{
	// 헤니르 시공이라면..
	if( m_eDungeonType == CXSLDungeon::DT_HENIR )
	{
		// 상자스테이지에서 다음 스테이지로 넘어갈때 임시 인벤 삭제
		if( SiKHenirResultTable()->IsClearTempInventoryStage( m_iStageID ) == true )
		{
			// 임시 인벤 비우기!
			BroadCastID( ERM_CLEAR_TEMP_INVENTORY_NOT );
		}
	}
	// 일반 던전이라면..
	else
	{
		// 임시 인벤 비우기!
		BroadCastID( ERM_CLEAR_TEMP_INVENTORY_NOT );
	}
}
//}}

//{{ 2010. 01. 05  최육사	내구도개편
short KDungeonRoom::GetSubStageExpNpcNum( IN int iSubStageID ) const
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapSubStageExpNpcNum.find( iSubStageID );
	if( mit == m_mapSubStageExpNpcNum.end() )
		return 0;

	return mit->second;
}
//}}

#ifdef SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE// 작업날짜: 2013-09-04	// 박세훈
void KDungeonRoom::ComputeTotalPlayTime( void )
{
	m_fTotalPlayTime = m_kTimer[TM_PLAY].elapsed() - GetLoadingTime();

	// 플레이 타임이 정상적으로 계산되어 나오는지 확인해보자!
	if( GetTotalPlayTime() < 0.0 )
	{
		START_LOG( cout, L"로딩시간을 뺀 실제 던전 플레이 시간이 마이너스값이다! 있을 수 없는 에러!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( GetTotalPlayTime() )
			<< BUILD_LOG( m_kTimer[TM_PLAY].elapsed() )
			<< BUILD_LOG( GetLoadingTime() )
			<< BUILD_LOG( GetDungeonIDAndDif() )
			<< BUILD_LOG( GetStageID() )
			<< BUILD_LOG( GetSubStageID() )
			<< BUILD_LOG( m_spRoomUserManager->GetNumMember() )
			<< BUILD_LOG( m_iStartedNumMember )
			<< END_LOG;
	}
}
#endif // SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE

//{{ 2012. 07. 04	최육사		헤니르 시공 자동 부활 시점 변경
#ifdef SERV_AUTO_RESURRECTION_BUG_FIX
void KDungeonRoom::HenirAutoResurrection( IN const UidType iUnitUID )
{
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iUnitUID );
	if( IS_NULL( spRoomUser ) )
	{
		START_LOG( cerr, L"유저가 존재하지 않습니다" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	// 특정 스테이지 마다 부활!
	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	if( SiKHenirResultTable()->IsResurrectionStage( GetStageID() ) == true )
#else
	if( SiKHenirResultTable()->IsResurrectionStage( spRoomUser->GetPassedStageCount() ) == true )
#endif SERV_NEW_HENIR_TEST
		//}}
	{
		// 죽은 유저인지 확인!
		if( spRoomUser->IsDie() == true )
		{
			// 되살리자!
			spRoomUser->SetDie( false );

			KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT kPacketNot;
			kPacketNot.m_iUnitUID = spRoomUser->GetCID();
			kPacketNot.m_iUsedByUnitUID = spRoomUser->GetCID();
			kPacketNot.m_iStartPosIndex = SiCXSLDungeonManager()->GetStartPosByRandom( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), m_iStageID, m_iSubStageID );
			BroadCast( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacketNot );

			//{{ 2008. 6. 17  최육사  실시간 부활석
			m_spRoomUserManager->SetEndPlay( spRoomUser->GetCID(), false );
			//}}
		}
	}	
}
#endif SERV_AUTO_RESURRECTION_BUG_FIX
//}}

//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KDungeonRoom::IncreasePartyFever()
{
	if( m_iPartyUID <= 0 )
		return;

	if( m_spRoomUserManager->GetNumMember() > 1 )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetHostUser(); // 파티장
		if( spRoomUser != NULL )
		{
			KERM_INCREASE_PARTY_FEVER_NOT kNot;
			kNot.m_iPartyUID = m_iPartyUID;
			kNot.m_cRoomType = CXSLRoom::RT_DUNGEON;
			kNot.m_sSubStageNpcCount = GetSubStageExpNpcNum( GetSubStageID() );
			SendToGSServer( spRoomUser->GetGSUID(), ERM_INCREASE_PARTY_FEVER_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"유저가 한명도 없는방인감?" )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
	}
}

void KDungeonRoom::MaxPartyFever()
{
	if( m_iPartyUID <= 0 )
		return;

	if( m_spRoomUserManager->GetNumMember() > 1 )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetHostUser(); // 파티장
		if( spRoomUser != NULL )
		{
			KERM_MAX_PARTY_FEVER_NOT kNot;
			kNot.m_iPartyUID = m_iPartyUID;
			SendToGSServer( spRoomUser->GetGSUID(), ERM_MAX_PARTY_FEVER_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"유저가 한명도 없는방인감?" )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
	}
}

void KDungeonRoom::ReservedRegroupOldParty()
{
	if( m_iPartyUID <= 0 )
		return;

	if( IsStartedByAutoParty() == false )
		return;

	if( m_spRoomUserManager->GetNumMember() > 1 )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetHostUser(); // 파티장
		if( spRoomUser != NULL )
		{
			m_vecReservedRegroupOldParty.clear();

			KERM_END_GAME_REGROUP_PARTY_NOT kNot;
			kNot.m_iGSUID = spRoomUser->GetGSUID();
			kNot.m_iPartyUID = m_iPartyUID;
			m_vecReservedRegroupOldParty.push_back( kNot );
		}
		else
		{
			START_LOG( cerr, L"유저가 한명도 없는방인감?" )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
	}    
}

void KDungeonRoom::CheckReserveRegroupOldParty()
{
	BOOST_TEST_FOREACH( const KERM_END_GAME_REGROUP_PARTY_NOT&, kPacketNot, m_vecReservedRegroupOldParty )
	{
		SendToGSServer( kPacketNot.m_iGSUID, ERM_END_GAME_REGROUP_PARTY_NOT, kPacketNot );
		break;
	}
		
	m_vecReservedRegroupOldParty.clear();
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KDungeonRoom::TimeCountFinishProcess( IN const u_short usEventID )
{
	switch( usEventID )
	{
	case ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ:
		{
			// 게임 초기화.
			StartGame();	

			//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
			// 방 생성 요청자에게 ack를 날린다.
			KERM_PARTY_OPEN_DUNGEON_ROOM_NOT kNot;
			GetRoomInfo( kNot.m_RoomInfo );
			GetRoomSlotInfo( kNot.m_vecSlot );
			GetRoomSlotInfo( kNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
			
			if( IsStartedByAutoParty() )
			{
				AutoPartyBuff( kNot.m_mapActivateBuff );
			}

			//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			PlayWithCharBuff( kNot.m_mapActivateBuff );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			//}}
#ifdef SERV_BUFF_BONUSRATE_HAMEL_EVENT
			PlayHamelEventBuff(kNot.m_mapActivateBuff);
#endif SERV_BUFF_BONUSRATE_HAMEL_EVENT

#ifdef SERV_CRAYON_POP_EVENT_BUFF// 작업날짜: 2013-06-08	// 박세훈
			CrayonPopEventBuff( kNot.m_mapActivateBuff );
#endif // SERV_CRAYON_POP_EVENT_BUFF

#ifdef SERV_BONUS_BUFF_SYSTEM
			PlayWithBonusBuff( kNot.m_mapActivateBuff );
#endif //SERV_BONUS_BUFF_SYSTEM

			BroadCast( ERM_PARTY_OPEN_DUNGEON_ROOM_NOT, kNot );
#endif SERV_SERVER_BUFF_SYSTEM
			//}
		}
		break;

	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ:
		{
			// 게임 초기화.
			StartGame();

			//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
			// 방 생성 요청자에게 ack를 날린다.
			KERM_PARTY_OPEN_DUNGEON_ROOM_NOT kNot;
			GetRoomInfo( kNot.m_RoomInfo );
			GetRoomSlotInfo( kNot.m_vecSlot );
			GetRoomSlotInfo( kNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );

			if( IsStartedByAutoParty() )
			{
				if( AutoPartyBuff( kNot.m_mapActivateBuff ) == true )
				{
					MaxPartyFever();
				}
			}

			//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			PlayWithCharBuff( kNot.m_mapActivateBuff );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			//}}
#ifdef SERV_BUFF_BONUSRATE_HAMEL_EVENT
			PlayHamelEventBuff(kNot.m_mapActivateBuff);
#endif SERV_BUFF_BONUSRATE_HAMEL_EVENT

#ifdef SERV_CRAYON_POP_EVENT_BUFF// 작업날짜: 2013-06-08	// 박세훈
			CrayonPopEventBuff( kNot.m_mapActivateBuff );
#endif // SERV_CRAYON_POP_EVENT_BUFF

#ifdef SERV_BONUS_BUFF_SYSTEM
			PlayWithBonusBuff( kNot.m_mapActivateBuff );
#endif //SERV_BONUS_BUFF_SYSTEM

			BroadCast( ERM_PARTY_OPEN_DUNGEON_ROOM_NOT, kNot );
#endif SERV_SERVER_BUFF_SYSTEM
			//}
		}
		break;

	default:
		{
			START_LOG( cerr, L"여기로 오면 안된다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
				<< END_LOG;
		}
		break;
	}   
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
void KDungeonRoom::PrepareForEnterDefenceDungeon()
{
	// 게임 종료 처리
	EndGame();

	// 던전 속성을 어둠의 문으로 바꾸자!
	m_iDungeonID = SEnum::DI_DEFENCE_DUNGEON_ELDER_NORMAL;
	m_cDifficultyLevel = 0;
	m_eDungeonType = CXSLDungeon::DT_NORMAL;
	m_cDungeonMode = CXSLDungeon::DM_INVALID;

	// 던전의 상태를 바꾸자!
	StateTransition( KRoomFSM::I_TO_WAIT_FOR_DEFENCE );
	m_kTimer[TM_WAIT_FOR_DEFENCE].restart();

	// 모든 유저 상태를 대기 상태로 바꾸자!
	m_spRoomUserManager->SetAllPrepareForDefenceDungeon( false );

	// 방안의 모든 유저에게 현재 던전 정보를 어둠의 문으로 변경하도록 처리한다.
	KERM_PREPARE_FOR_DEFENCE_DUNGEON_REQ kPacketReq;
	kPacketReq.m_kDefenceDungeonInfo.m_iDungeonID = GetDungeonID();
	kPacketReq.m_kDefenceDungeonInfo.m_cDifficulty = GetDifficultyLevel();
	kPacketReq.m_kDefenceDungeonInfo.m_cDungeonMode = m_cDungeonMode;
	BroadCast( ERM_PREPARE_FOR_DEFENCE_DUNGEON_REQ, kPacketReq );
}

//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
void KDungeonRoom::EnterDefenceDungeon( IN const std::vector< UidType >& vecLeaveUnitUID )
{
	// 일단 먼저 이탈자들부터 이탈처리하자!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecLeaveUnitUID )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iUnitUID );
		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cerr, L"유저가 존재하지 않습니다." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// 해당 멤버는 강제이탈!
		if( m_spRoomUserManager->LeaveRoom( iUnitUID ) == false )
		{
			START_LOG( cerr, L"방 나가기 실패." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
		}

		// 거절한 유저는 어둠의 문으로 입장 시키지 말고 방을 나가게 하자!
		//KEGS_RESULT_ALL_UNIT_SUCCESS_NOT kNot;
		//GetRoomInfo( kNot.m_RoomInfo );
		//GetRoomSlotInfo( kNot.m_vecSlot );
		SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT, char() );

		// 방의 모든 유저들에게 슬롯 정보를 날린다.
		KEGS_LEAVE_ROOM_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iUnitUID;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
	}

	// 이탈 하고나서 만약에 방에 아무도 없으면 그대로 게임 종료 시키자!
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
		return;
	}

	// 상태 변경
	StateTransition( KRoomFSM::I_TO_WAIT );

	//모든유저상태를 레디로 바꾼다.
	m_spRoomUserManager->SetAllReady( true );

	// 게임 초기화.
	StartGame();
#ifdef SERV_CRAYON_POP_EVENT_BUFF// 작업날짜: 2013-06-12	// 박세훈
	{
		KBuffInfo	kBuff;
		KLottery	kLot;
		kLot.AddCase( CXSLBuffManager::BTI_BUFF_ELRIN, 45.0f );
		kLot.AddCase( CXSLBuffManager::BTI_BUFF_SOYUL, 30.0f );
		kLot.AddCase( CXSLBuffManager::BTI_BUFF_KUMI, 5.0f );
		kLot.AddCase( CXSLBuffManager::BTI_BUFF_CHOA, 10.0f );
		kLot.AddCase( CXSLBuffManager::BTI_BUFF_WEY, 10.0f );
		kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID	= kLot.Decision();

		if( kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID == KLottery::CASE_BLANK )
		{
			kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_SOYUL;
		}

		KERM_ACTIVATE_BUFF_NOT kPacketNot;
		kPacketNot.m_vecActivateBuffList.push_back( kBuff );
		BroadCast( ERM_ACTIVATE_BUFF_NOT, kPacketNot );
	}
#endif // SERV_CRAYON_POP_EVENT_BUFF

	// 바뀐 속성대로 게임을 다시 시작하자!
	KEGS_DEFENCE_DUNGEON_GAME_START_NOT kDefenceDungeonStart;
	GetRoomInfo( kDefenceDungeonStart.m_RoomInfo );
	GetRoomSlotInfo( kDefenceDungeonStart.m_vecSlot );
	GetRoomSlotInfo( kDefenceDungeonStart.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
	BroadCast( ERM_DEFENCE_DUNGEON_GAME_START_NOT, kDefenceDungeonStart );
}
#else
//void KDungeonRoom::EnterDefenceDungeon()
//{
//	// 상태 변경
//	StateTransition( KRoomFSM::I_TO_WAIT );
//
//	//모든유저상태를 레디로 바꾼다.
//	m_spRoomUserManager->SetAllReady( true );
//
//	// 게임 초기화.
//	StartGame();
//
//	// 바뀐 속성대로 게임을 다시 시작하자!
//	KEGS_DEFENCE_DUNGEON_GAME_START_NOT kDefenceDungeonStart;
//	GetRoomInfo( kDefenceDungeonStart.m_RoomInfo );
//	GetRoomSlotInfo( kDefenceDungeonStart.m_vecSlot );
//	GetRoomSlotInfo( kDefenceDungeonStart.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
//	BroadCast( ERM_DEFENCE_DUNGEON_GAME_START_NOT, kDefenceDungeonStart );    
//}
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
//}}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//////////////////////////////////////////////////////////////////////////
// 현재로서는 튜토리얼 던전만 이 패킷으로 게임을 시작하고있다!
//////////////////////////////////////////////////////////////////////////
_IMPL_ON_FUNC( ERM_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_ROOM_ACK, KEGS_CREATE_ROOM_ACK );

	//{{ 2013. 04. 16	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( CXSLDungeon::IsDefenceDungeon( kPacket_.m_kRoomInfo.m_iDungeonID ) == true )
	{
		START_LOG( cerr, L"패킷 변조 의심 유저! ( 어둠의 문 던전을 직접적으로 생성하려 하였다 )" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_30;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_ROOM_ACK, kPacket );
		return;
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_DUNGEON, true );
#endif SERV_ROOM_COUNT
	//}}

    // 방 옵션 정보 세팅.
	SetRoomInfo( kPacket_.m_kRoomInfo );

    // 방 생성이므로 슬롯 정보를 초기화 한다.
    m_spRoomUserManager->Reset();
    m_spRoomUserManager->AssignTeam( -1 );

    // 슬롯에 생성 요청자를 넣는다.
    if( m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) == false )
    {
        START_LOG( cerr, L"방 입장 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( GetUID() )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_ROOM_30;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_ROOM_ACK, kPacket );
        return;
    }

	//080409.hoons. 제자 리스트를 셋팅한다.
	m_spRoomUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );

    // 방 상태를 wait 로 바꾼다.
    StateTransition( KRoomFSM::I_TO_WAIT );
    m_kTimer[TM_WAIT].restart();

	//gm 이상이면 센터서버의 ip를 넣어주자.
	//if( kPacket_.m_kUnitInfo.m_cAuthLevel >= SEnum::UAL_GM )
	if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

    // 방 생성 요청자에게 ack를 날린다.
    kPacket.m_iOK = NetError::NET_OK;
	GetRoomInfo( kPacket.m_RoomInfo );

	GetRoomSlotInfo( kPacket.m_vecSlot );
	GetRoomSlotInfo( kPacket.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_ROOM_ACK, kPacket );

	//방생성후 좀비유저 체크시작.
	m_spRoomUserManager->ZU_CheckStart();
}

IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_PARTY_OPEN_DUNGEON_ROOM_ACK, KERM_PARTY_OPEN_DUNGEON_ROOM_ACK );

	//{{ 2013. 04. 16	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( CXSLDungeon::IsDefenceDungeon( kPacket_.m_kRoomInfo.m_iDungeonID ) == true )
	{
		START_LOG( cerr, L"패킷 변조 의심 유저! ( 어둠의 문 던전을 직접적으로 생성하려 하였다 )" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_30;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PARTY_OPEN_DUNGEON_ROOM_ACK, kPacket );
		return;
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_DUNGEON, true );
#endif SERV_ROOM_COUNT
	//}}

	// 방 옵션 정보 세팅.
	SetRoomInfo( kPacket_.m_kRoomInfo );

	// 방 생성이므로 슬롯 정보를 초기화 한다.
	m_spRoomUserManager->Reset();
	m_spRoomUserManager->AssignTeam( -1 );

	BOOST_TEST_FOREACH( const KPartyRoomUserInfo&, kInfo, kPacket_.m_vecUserInfo )
	{
		// 슬롯에 유저 넣는다.
		if( m_spRoomUserManager->EnterRoom( kInfo.m_kRoomUserInfo, false ) == false )
		{
			START_LOG( cerr, L"파티 던장방 입장 실패." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_30;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PARTY_OPEN_DUNGEON_ROOM_ACK, kPacket );
			return;
		}
		
		m_spRoomUserManager->SetTutorialUIDList( kInfo.m_kRoomUserInfo.m_nUnitUID, kInfo.m_vecStudentUnitUID );
		//{{ 2009. 6. 10  최육사	게임 시작시 EXP,ED
		m_spRoomUserManager->UpdateetOldExpED( kInfo.m_kRoomUserInfo.m_nUnitUID, kInfo.m_iCurEXP, kInfo.m_iCurED );
		//}}
		//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		m_spRoomUserManager->SetGameChannelIP( kInfo.m_kRoomUserInfo.m_nUnitUID, kInfo.m_wstrChannelIP );
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 03. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_spRoomUserManager->SetActivatedAutoPartyBonus( kInfo.m_kRoomUserInfo.m_nUnitUID, kInfo.m_bAutoPartyBonus );
		m_spRoomUserManager->SetOldPartyInfo( kInfo.m_kRoomUserInfo.m_nUnitUID, kInfo.m_kOldPartyInfo );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		m_spRoomUserManager->SetAutoPartyWaitTime( kInfo.m_kRoomUserInfo.m_nUnitUID, kInfo.m_iAutoPartyWaitTime );
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
	}

	// 인원접속처리가 잘못되어 파티를 시작할수 없음.
	if( true == IsEmpty() )
	{
		kPacket.m_iOK = NetError::ERR_PARTY_23;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PARTY_OPEN_DUNGEON_ROOM_ACK, kPacket );
		return;
	}

	// 방 상태를 wait 로 바꾼다.
	StateTransition( KRoomFSM::I_TO_WAIT );
	m_kTimer[TM_WAIT].restart();

	//모든유저상태를 레디로 바꾼다.
	m_spRoomUserManager->SetAllReady(true);

	//////////////////////////////////////////////////////////////////////////
	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 3초 카운트
	TimeCountForStartGame( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//////////////////////////////////////////////////////////////////////////

	//던전방 준비가 완료되어다고 파티장에게 알려줌.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PARTY_OPEN_DUNGEON_ROOM_ACK, kPacket );	
}

//{{ 2011. 12. 09	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_SINGLE_OPEN_DUNGEON_ROOM_ACK, KERM_SINGLE_OPEN_DUNGEON_ROOM_ACK );

	//{{ 2013. 04. 16	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	if( CXSLDungeon::IsDefenceDungeon( kPacket_.m_kRoomInfo.m_iDungeonID ) == true )
	{
		START_LOG( cerr, L"패킷 변조 의심 유저! ( 어둠의 문 던전을 직접적으로 생성하려 하였다 )" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_30;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SINGLE_OPEN_DUNGEON_ROOM_ACK, kPacket );
		return;
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_DUNGEON, true );
#endif SERV_ROOM_COUNT
	//}}

	// 방 옵션 정보 세팅.
	SetRoomInfo( kPacket_.m_kRoomInfo );

	// 방 생성이므로 슬롯 정보를 초기화 한다.
	m_spRoomUserManager->Reset();
	m_spRoomUserManager->AssignTeam( -1 );

	// 슬롯에 생성 요청자를 넣는다.
	if( m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) == false )
	{
		START_LOG( cerr, L"방 입장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_30;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SINGLE_OPEN_DUNGEON_ROOM_ACK, kPacket );
		return;
	}

	//080409.hoons. 제자 리스트를 셋팅한다.
	m_spRoomUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );
	//{{ 2009. 6. 10  최육사	게임 시작시 EXP,ED
	m_spRoomUserManager->UpdateetOldExpED( kPacket_.m_kRoomUserInfo.m_nUnitUID, kPacket_.m_iCurEXP, kPacket_.m_iCurED );
	//}}
	//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	m_spRoomUserManager->SetGameChannelIP( kPacket_.m_kRoomUserInfo.m_nUnitUID, kPacket_.m_wstrChannelIP );
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

	// 방 상태를 wait 로 바꾼다.
	StateTransition( KRoomFSM::I_TO_WAIT );
	m_kTimer[TM_WAIT].restart();

	//gm 이상이면 센터서버의 ip를 넣어주자.
	//if( kPacket_.m_kUnitInfo.m_cAuthLevel >= SEnum::UAL_GM )
	//if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
	//{
	//	kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	//}

	m_spRoomUserManager->SetAllReady(true);

	//////////////////////////////////////////////////////////////////////////
	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 3초 카운트
	TimeCountForStartGame( ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//////////////////////////////////////////////////////////////////////////
	
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SINGLE_OPEN_DUNGEON_ROOM_ACK, kPacket );
}

//_IMPL_ON_FUNC( ERM_SINGLE_DUNGEON_GAME_REPLAY_REQ, KERM_OPEN_ROOM_REQ )
//{
//	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_SINGLE_DUNGEON_GAME_REPLAY_ACK, KEGS_QUICK_START_DUNGEON_GAME_ACK );
//
//	// 방 옵션 정보 세팅.
//	SetRoomInfo( kPacket_.m_kRoomInfo );
//
//	// 해당 유저정보를 업데이트한다.
//	if( m_spRoomUserManager->SetRoomUserInfo( kPacket_.m_kRoomUserInfo.m_nUnitUID, kPacket_.m_kRoomUserInfo ) == false )
//	{
//		START_LOG( cerr, L"파티 던장방 리플레이 실패. 업데이트 하려는 유저가 방에 없음" )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< BUILD_LOG( GetUID() )
//			<< END_LOG;
//
//		kPacket.m_iOK = NetError::ERR_ROOM_30;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SINGLE_DUNGEON_GAME_REPLAY_ACK, kPacket );
//
//		// [고민] 실패 처리를 할까? 업데이트 전에 이탈한거라면 그녀석은 제외하고 게임을 시작해버릴까?
//		return;
//	}
//	else
//	{
//		m_spRoomUserManager->SetTutorialUIDList( kPacket_.m_kRoomUserInfo.m_nUnitUID, kPacket_.m_vecStudentUnitUID );
//		//{{ 2009. 6. 10  최육사	게임 시작시 EXP,ED
//		m_spRoomUserManager->UpdateetOldExpED( kPacket_.m_kRoomUserInfo.m_nUnitUID, kPacket_.m_iCurEXP, kPacket_.m_iCurED );
//		//}}
//	}
//
//	//모든유저상태를 레디로 바꾼다.
//	m_spRoomUserManager->SetAllReady(true);
//
//	//던전방 준비가 완료되어다고 파티장에게 알려줌.
//	kPacket.m_iOK = NetError::NET_OK;
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SINGLE_DUNGEON_GAME_REPLAY_ACK, kPacket );
//
//	// 게임 초기화.
//	StartGame();
//
//	//던장방 전보 & 게임시작정보를 구성해서 Not 를 날려줌.
//	KEGS_PARTY_GAME_START_NOT kNot;
//
//	//센터서버의 ip를 넣어주고 gsuser에서 개발자 이상이 아니면 지우자.
//	if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
//	{
//		kNot.m_wstrCNIP = NetCommon::GetLocalIPW();
//	}
//
//	GetRoomInfo( kNot.m_RoomInfo );
//	GetRoomSlotInfo( kNot.m_vecSlot );
//	GetRoomSlotInfo( kNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
//
//	BroadCast( ERM_PARTY_GAME_REPLAY_START_NOT, kNot );
//}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( ERM_GAME_START_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_GAME_START_ACK, KERM_GAME_START_ACK );

	if( m_spRoomUserManager->IsObserver( FIRST_SENDER_UID ) == false )
	{
		// 방장인지 체크한다.
		VERIFY_HOST( ERM_GAME_START_ACK );
	}

	// 모든 플레이어가 준비 상태인지 체크한다.
	if( !m_spRoomUserManager->IsAllPlayerReady() )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_10;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );
		return;
	}

	// 게임 초기화.
	StartGame();

	// 방장에게 ack를 날린다. 근성도 감소치도 포함
	kPacket.m_iOK			  = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GAME_START_ACK, kPacket );

	// 방의 모든 유저들에게 not를 날린다.
	KERM_GAME_START_NOT kPacketNot;
	kPacketNot.m_cRoomState = GetStateID();
	kPacketNot.m_iGameType	= m_cGameType;
	GetRoomSlotInfo( kPacketNot.m_vecSlot );
	kPacketNot.m_iDungeonID	= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
	kPacketNot.m_sWorldID	= m_sWorldID;
	BroadCast( ERM_GAME_START_NOT, kPacketNot );
}

//{{ 2011. 10. 25	최육사	배틀필드 시스템
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, KERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_ACK );
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_REQ, KEGS_DUNGEON_STAGE_LOAD_REQ )
{
	//////////////////////////////////////////////////////////////////////////
	// 상태 오류 케이스
	// 1. S_RESULT : 2명이상 파티플중 한명 죽은상태에서 다른 한명이 스테이지 클리어와 동시에 던전이탈을 하면 발생
	//////////////////////////////////////////////////////////////////////////

	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_STAGE_LOAD_ACK, KEGS_DUNGEON_STAGE_LOAD_ACK );

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
	{
		if( m_bFirstStage == false )
		{
			if( m_kNextStageData.m_eClearType != CXSLDungeonSubStage::CT_STAGE &&
				m_kNextStageData.m_eClearType != CXSLDungeonSubStage::CT_STAGE_FORCE &&
				m_kNextStageData.m_eClearType != CXSLDungeonSubStage::CT_STAGE_SECRET )
			{
				// 서버가 골라준 클리어 타입이 아님. 클라이언트 해킹이다. by 박진웅
				START_LOG( cerr, L"클리어타입은 아래와 같은데 이 패킷이 왜 왔지? 해커!" )
					<< BUILD_LOG( m_bFirstStage )
					<< BUILD_LOG( m_kNextStageData.m_eClearType )
					<< BUILD_LOG( m_kNextStageData.m_iStageIndex )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_ROOM_51;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
				return;
			}

			// 타입 자체는 같고 스테이지만 다르다면 셋팅만 제대로 해서 정상 동작하게 하자. by 박진웅
			kPacket_.m_iStageID = m_kNextStageData.m_iStageIndex;
		}
		else
		{
			// 스테이지 로드 체크를 안 해도 되는건 처음 한번 뿐이다. by 박진웅
			kPacket_.m_iStageID = 0;
			m_bFirstStage = false;
		}
	}

	// 스테이지 오픈 후엔 바로 0번 서브 스테이지를 열어야 하므로 셋팅 by 박진웅
	m_bFirstSubStage = true;

	// 다음에 다시 막기 위해 값 초기화.
	m_kNextStageData.ClearData();
#endif // SERV_STAGE_CLEAR_IN_SERVER

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	// 디펜스 던전용 초기화
	m_iDefenseMainCrystalNpcUID = 0;
	m_setDefenseSmallCrystalNpcUID.clear();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2012. 12. 03 던전 스테이지 중복 로딩 예외 확인 - 김민성
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD_2
	{
		std::map< int, int >::iterator mitSLC = m_mapStageLoadingCheck.find( kPacket_.m_iStageID );
		if( mitSLC != m_mapStageLoadingCheck.end() )
		{
			START_LOG( cout, L"똑같은 스테이지 또 로딩했네? 이거 괜찮은거임?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( GetDungeonIDAndDif() )
				<< BUILD_LOG( GetStageID() )
				<< BUILD_LOG( GetSubStageID() )
				<< BUILD_LOG( m_mapStageLoadingCheck.size() )
				<< BUILD_LOG( mitSLC->second )
				<< BUILD_LOG( kPacket_.m_iStageID )
				<< BUILD_LOG( m_spRoomUserManager->GetNumMember() );

			if( m_iLastLoadingUnitUID != FIRST_SENDER_UID )
			{
				START_LOG( cout, L"방장이 바뀌었나? 이 전 스테이지 로딩한 유저와 다른 유저군.." )					
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( m_iLastLoadingUnitUID );
			}

			// 동일 스테이지 로딩 카운트를 1 올리자!
			++mitSLC->second;

			// 5번 이상 똑같은 스테이지 로딩하는 경우라면 해킹유저로 판별!
			if( mitSLC->second >= 5 )
			{
				START_LOG( cout, L"Stage Loading 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( GetDungeonIDAndDif() )
					<< BUILD_LOG( GetStageID() )
					<< BUILD_LOG( m_mapStageLoadingCheck.size() )
					<< BUILD_LOG( mitSLC->second )
					<< BUILD_LOG( kPacket_.m_iStageID );

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// 작업날짜: 2013-05-15	// 박세훈
				if( SiKGameSysVal()->GetDungeonStageLoadLog() == true )
				{
					DungonStageLoadLog sDungonStageLoadLog;
					sDungonStageLoadLog.m_iUnitUID					= FIRST_SENDER_UID;
					sDungonStageLoadLog.m_cNormalStage				= kPacket_.m_cNormalStage;
					sDungonStageLoadLog.m_cSecretStage				= kPacket_.m_cSecretStage;
					sDungonStageLoadLog.m_cEliteStage				= kPacket_.m_cEliteStage;
					sDungonStageLoadLog.m_cClearConditionCheckCount	= kPacket_.m_cClearConditionCheckCount;
					sDungonStageLoadLog.m_iDungeonID				= GetDungeonIDAndDif();
					sDungonStageLoadLog.m_iNowStageID				= GetStageID();
					sDungonStageLoadLog.m_iReqStageID				= kPacket_.m_iStageID;
					sDungonStageLoadLog.m_bHost						= m_spRoomUserManager->IsHost( FIRST_SENDER_UID );
					sDungonStageLoadLog.m_cMemberNum				= m_spRoomUserManager->GetNumMember();
					m_vecDungonStageLoadLog.push_back( sDungonStageLoadLog );

					START_LOG( cout, L"테스트로그" )
						<< BUILD_LOG( GetStateID() )
						<< BUILD_LOG( sDungonStageLoadLog.m_iNowStageID )
						<< END_LOG;

					KDBE_DUNGEON_STAGE_LOAD_LOG_NOT kPacketToDB;
					kPacketToDB.m_iRoomUID				= GetUID();
					kPacketToDB.m_vecDungonStageLoadLog	= m_vecDungonStageLoadLog;
					SendToLogDB( DBE_DUNGEON_STAGE_LOAD_LOG_NOT, kPacketToDB );
					m_vecDungonStageLoadLog.clear();
				}
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

				kPacket.m_iOK = NetError::ERR_ROOM_22;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );

				// 파티장에게 해킹체크패킷을 보내자!
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_STAGE_LOADING;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
				return;
			}
			else
			{
#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// 작업날짜: 2013-05-15	// 박세훈
				if( SiKGameSysVal()->GetDungeonStageLoadLog() == true )
				{
					DungonStageLoadLog sDungonStageLoadLog;
					sDungonStageLoadLog.m_iUnitUID					= FIRST_SENDER_UID;
					sDungonStageLoadLog.m_cNormalStage				= kPacket_.m_cNormalStage;
					sDungonStageLoadLog.m_cSecretStage				= kPacket_.m_cSecretStage;
					sDungonStageLoadLog.m_cEliteStage				= kPacket_.m_cEliteStage;
					sDungonStageLoadLog.m_cClearConditionCheckCount	= kPacket_.m_cClearConditionCheckCount;
					sDungonStageLoadLog.m_iDungeonID				= GetDungeonIDAndDif();
					sDungonStageLoadLog.m_iNowStageID				= GetStageID();
					sDungonStageLoadLog.m_iReqStageID				= kPacket_.m_iStageID;
					sDungonStageLoadLog.m_bHost						= m_spRoomUserManager->IsHost( FIRST_SENDER_UID );
					sDungonStageLoadLog.m_cMemberNum				= m_spRoomUserManager->GetNumMember();
					m_vecDungonStageLoadLog.push_back( sDungonStageLoadLog );
				}
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

				// 한번만 처리 하도록 수정
				kPacket.m_iOK = NetError::ERR_ROOM_22;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
				return;
			}
		}
		// NOT 를 보내야지만 스테이지ID 등록을 하자
	}	
#else
	/*
	//{{ 2010. 11. 18	최육사	오토 핵 스테이지 로딩 체크
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	{
		std::map< int, int >::iterator mitSLC = m_mapStageLoadingCheck.find( kPacket_.m_iStageID );
		if( mitSLC != m_mapStageLoadingCheck.end() )
		{
			START_LOG( cout, L"똑같은 스테이지 또 로딩했네? 이거 괜찮은거임?" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( GetDungeonIDAndDif() )
				<< BUILD_LOG( GetStageID() )
				<< BUILD_LOG( GetSubStageID() )
				<< BUILD_LOG( m_mapStageLoadingCheck.size() )
				<< BUILD_LOG( mitSLC->second )
				<< BUILD_LOG( kPacket_.m_iStageID )
				<< BUILD_LOG( m_spRoomUserManager->GetNumMember() );

			if( m_iLastLoadingUnitUID != FIRST_SENDER_UID )
			{
				START_LOG( cout, L"방장이 바뀌었나? 이 전 스테이지 로딩한 유저와 다른 유저군.." )					
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( m_iLastLoadingUnitUID );
			}

			// 동일 스테이지 로딩 카운트를 1 올리자!
			++mitSLC->second;

			// 마지막으로 로딩 성공한 유저의 UnitUID를 저장!
			m_iLastLoadingUnitUID = FIRST_SENDER_UID;

			// 5번 이상 똑같은 스테이지 로딩하는 경우라면 해킹유저로 판별!
			if( mitSLC->second >= 5 )
			{
				START_LOG( cout, L"Stage Loading 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( GetDungeonIDAndDif() )
					<< BUILD_LOG( GetStageID() )
					<< BUILD_LOG( m_mapStageLoadingCheck.size() )
					<< BUILD_LOG( mitSLC->second )
					<< BUILD_LOG( kPacket_.m_iStageID );

				kPacket.m_iOK = NetError::ERR_ROOM_22;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );

				// 파티장에게 해킹체크패킷을 보내자!
				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_STAGE_LOADING;
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_STAGE_LOADING;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
				return;
			}
		}
		else
		{
			// 로딩하지 않은 StageID를 저장한다.
			m_mapStageLoadingCheck.insert( std::make_pair( kPacket_.m_iStageID, 1 ) );

			// 마지막으로 로딩 성공한 유저의 UnitUID를 저장!
			m_iLastLoadingUnitUID = FIRST_SENDER_UID;
		}
	}	
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
	//}}
	*/
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD_2
	//}


	//로딩시간 체크를 시작함..
	//이곳에서 시작을 할경우 2가지의 경우에 로딩시간을 체크하게 된다.
	//::1. 게임초기 로딩시
	//::2. 스테이지 로딩
	//## 장막로딩은 빼기로 함.
	m_LoadingTimer.Start();

	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	const CXSLDungeonStage::STAGE_TYPE eStageType = SiCXSLDungeonManager()->GetStageType( GetDungeonIDAndDif(), kPacket_.m_iStageID );
#endif SERV_SECRET_HELL
	//}}

	KEGS_DUNGEON_STAGE_LOAD_NOT kNot;
	kNot.m_iNumMember = m_spRoomUserManager->GetNumMember();

	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	int iRelativeMonsterLevel = GetAverageUserLevelAtDungeonStart(); // 던전 게임 시작시 유저 평균 레벨이 보정 던전의 몬스터 레벨이 된다.
#else
	int iHighestUserLevelAtDungeonStart = GetHighestUserLevelAtDungeonStart();
#endif SERV_DUNGEON_NPC_LEVEL
	//}}	

	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	if( m_eDungeonType == CXSLDungeon::DT_SECRET )
	{
		// 비밀던전의 경우 레벨 보정 적용
		{
			const int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( GetDungeonIDAndDif() );
			if( iRelativeMonsterLevel < iDungeonMinLevel )
			{
				iRelativeMonsterLevel = iDungeonMinLevel;
			}

			const int iDungeonMaxLevel = SiCXSLDungeonManager()->GetDungeonMaxLevel( GetDungeonIDAndDif() );
			if( iRelativeMonsterLevel > iDungeonMaxLevel )
			{
				iRelativeMonsterLevel = iDungeonMaxLevel;
			}
		}

		if( m_cDungeonMode == CXSLDungeon::DM_SECRET_HELL )
		{
			// 비밀던전 헬모드 EXTRA STAGE
			if( eStageType == CXSLDungeonStage::ST_EXTRA )
			{
				// Extra Stage Load Count
				++m_iExtraStageLoadCount;

				iRelativeMonsterLevel = iRelativeMonsterLevel + 5 + SiCXSLDungeonManager()->GetSecretDungeonExtraStageNpcLevel();
			}
			// 비밀던전 헬모드
			else
			{
				iRelativeMonsterLevel = iRelativeMonsterLevel + 5;
			}
		}
	}
#endif SERV_SECRET_HELL
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	else
	{
		if( CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) )
		{
			const int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( GetDungeonIDAndDif() );
			if( iRelativeMonsterLevel < iDungeonMinLevel )
			{
				iRelativeMonsterLevel = iDungeonMinLevel;
			}
		}
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가
	
	// 보스 스테이지 여부
	bool bIsBossStage = false;

	// NPC정보 불러오기!
	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	if( SiCXSLDungeonManager()->GetNPCData( GetDungeonIDAndDif(), kPacket_.m_iStageID, iRelativeMonsterLevel, kNot, bIsBossStage ) == false )
#else
	if( SiCXSLDungeonManager()->GetNPCData( GetDungeonIDAndDif(), kPacket_.m_iStageID, iHighestUserLevelAtDungeonStart, kNot, bIsBossStage ) == false )
#endif SERV_DUNGEON_NPC_LEVEL
	//}}
	{
		kPacket.m_iOK = NetError::ERR_ROOM_22;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
		return;
	}
	else
	{
		// 정상적인 StageID라고 판단되면 현재 스테이지ID 정보로 업데이트 하자!
		SetStageID( kPacket_.m_iStageID );

		// 정상적인 StageID값이므로 클라이언트로 OK값 보냄.
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
	}
	//}}	

	//////////////////////////////////////////////////////////////////////////
	//{{ 2010. 07. 16  최육사	이벤트 몬스터 시스템	
#ifdef SERV_EVENT_MONSTER
	if( CXSLDungeon::IsTutorialDungeon( GetDungeonID() ) == false  &&
		CXSLDungeon::IsTCDungeon( GetDungeonID() ) == false  &&
		CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == false 
		//{{ 2011. 07. 25    김민성    이벤트 몬스터 출현 던전 제한
#ifdef SERV_EVENT_MONSTER_DUNGEON_RESTRICT
		&& CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) == false    // 디펜스 던전에는 이벤트 몬스터가 출현하지 않도록한다.
#endif SERV_EVENT_MONSTER_DUNGEON_RESTRICT
		//}} 
		)
	{
		//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		BOOST_TEST_FOREACH( KEventMonsterRefreshManager::KAdventNpcEventInfo&, kInfo, m_vecEventMonster )
#else
		BOOST_TEST_FOREACH( KGameEventScriptManager::KAdventNpcEventInfo&, kInfo, m_vecEventMonster )
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
			//}}
		{
			// 보스 스테이지에서 등장 시킬지 체크!
			if( kInfo.m_bIsAdventInBossStage == false  &&  bIsBossStage == true )
			{
				continue;
			}

			// 더이상 출현 시킬 카운트가 남아있지 않다면 패스!
			if( kInfo.m_iMonsterDropCount <= 0 )
			{
				continue;
			}

			// 등장할 수 있는 던전인지 체크!
			if( kInfo.m_setAdventDungeon.empty() == false )
			{
				if( kInfo.m_setAdventDungeon.find( GetDungeonID() ) == kInfo.m_setAdventDungeon.end() )
				{
					continue;
				}
			}

			// 등장 하면 안되는 던전인지 체크!
			if( kInfo.m_setNoAdventDungeon.empty() == false )
			{
				if( kInfo.m_setNoAdventDungeon.find( GetDungeonID() ) != kInfo.m_setNoAdventDungeon.end() )
				{
					continue;
				}
			}

			// 등장 하면 안되는 스테이지 인지 체크!
			if( kInfo.m_setNoAdventStage.empty() == false )
			{
				if( kInfo.m_setNoAdventStage.find( GetStageID() ) != kInfo.m_setNoAdventStage.end() )
				{
					continue;
				}
			}

			// 등장할 몬스터를 결정하자!
			const int iAdventNpcID = kInfo.m_kAdventNpcLot.Decision();
			if( iAdventNpcID == KLottery::CASE_BLANK )
			{
				continue;
			}

			// 이벤트 몬스터 등장 처리!
			//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
#ifdef SERV_ACTIVE_EVENT_MONSTER
			std::map<int, bool>::iterator mit = kInfo.m_mapMonsterActive.find( iAdventNpcID );
			bool bActive = true;

			if(mit != kInfo.m_mapMonsterActive.end())
			{
				bActive = mit->second;
			}

			if( EventDropMonster( iAdventNpcID, kInfo.m_bAllyNPC, GetStageID(), bActive, kNot ) == true )
#else
			if( EventDropMonster( iAdventNpcID, kInfo.m_bAllyNPC, GetStageID(), kNot ) == true )
#endif SERV_ACTIVE_EVENT_MONSTER
#else
			if( EventDropMonster( iAdventNpcID, GetStageID(), kNot ) == true )
#endif SERV_ALLY_EVENT_MONSTER
			//}}
			{
				// 몬스터 등장 성공했다면 카운트 하나 깎자!
				--kInfo.m_iMonsterDropCount;
			}
		}
	}
#endif SERV_EVENT_MONSTER
	//////////////////////////////////////////////////////////////////////////

	//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	if( CXSLDungeon::IsTutorialDungeon( GetDungeonID() ) == false  &&
		CXSLDungeon::IsTCDungeon( GetDungeonID() ) == false  &&
		CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == false )
	{
		if( m_mapTimeDropEventMonster.empty() == true )
		{
			int iTimeDropNpcEventUID = 0;
			int iTimeDropEventNpcID = 0;
			if( SiKDungeonEventManager()->CheckTimeDropEventMonster( GetDungeonIDAndDif(), GetStageID(), kNot, iTimeDropNpcEventUID, iTimeDropEventNpcID ) == true )
			{
				m_mapTimeDropEventMonster.insert( std::make_pair( iTimeDropEventNpcID, iTimeDropNpcEventUID ) );
			}
		}
	}
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

	//{{ 2010. 03. 21  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	if( m_cDungeonMode == CXSLDungeon::DM_SECRET_HELL  &&  eStageType == CXSLDungeonStage::ST_EXTRA_NPC )
	{
		// 비밀던전 extra 몬스터 등장 처리
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		SecretDungeonExtraStageMonster( GetStageID(), kPacket_.m_sPartyFever, kNot );
#else
		SecretDungeonExtraStageMonster( GetStageID(), kPacket_.m_cPartyFever, kNot );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		
	}
#endif SERV_SECRET_HELL
	//}}

	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장
#ifdef SERV_PC_BANG_DROP_EVENT

	//{{ 2012. 03. 28	박세훈	드롭률 설정 이벤트 개편 Merge 작업 ( 2012.03.20 lygan_조성욱 )
#ifdef DROPEVENT_RENEWAL
	std::vector<char> vecUintClass;
	std::vector<char> vecUintType;
	std::vector<int> vecUintLevel;
	for( int iU = 0; iU < m_kRoomUserManager.GetNumMember(); ++iU )
	{
		KRoomUserPtr spRoomUser = m_kRoomUserManager.GetUser( iU );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"RoomUser가 존재하지 않음." )
				<< END_LOG;
			continue;
		}
		vecUintClass.push_back(spRoomUser->GetUnitClass_LUA());
		vecUintType.push_back(spRoomUser->GetUnitType());
		vecUintLevel.push_back(spRoomUser->GetLevel());
	}

	if( SiKGameEventManager()->CheckItemDropProbEvent(GetDungeonID(), vecUintClass, vecUintType, vecUintLevel, m_iItemDropEventProbCount, m_bWithPlayPcBangEvent ) == true )
	{
		// PC방 유저와 함께 플레이 해야만 이벤트가 적용되는 경우! 방안에 PC방 유저가 있는지 체크하자!
		if( m_bWithPlayPcBangEvent )
		{
			if( m_spRoomUserManager->IsExistPcBangPlayer() == false )
			{
				// 디폴트값 넣어준다!
				m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
				m_bWithPlayPcBangEvent = false;
			}
		}
	}
	else
	{
		m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
		m_bWithPlayPcBangEvent = false;
	}

	// Not패킷에 저장
	kNot.m_iItemDropEventProbCount = m_iItemDropEventProbCount;
	kNot.m_bWithPlayPcBangEvent = m_bWithPlayPcBangEvent;
#else //DROPEVENT_RENEWAL
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	if( SiKGameEventManager()->CheckItemDropProbEvent( m_fItemDropEventProbRate, m_bWithPlayPcBangEvent ) == true )
	{
		// PC방 유저와 함께 플레이 해야만 이벤트가 적용되는 경우! 방안에 PC방 유저가 있는지 체크하자!
		if( m_bWithPlayPcBangEvent )
		{
			if( m_spRoomUserManager->IsExistPcBangPlayer() == false )
			{
				// 디폴트값 넣어준다!
				m_fItemDropEventProbRate = REE_DROP_EVENT_PROB_DEFAULT;
				m_bWithPlayPcBangEvent = false;
			}
		}
	}
	else
	{
		m_fItemDropEventProbRate = REE_DROP_EVENT_PROB_DEFAULT;
		m_bWithPlayPcBangEvent = false;
	}

	// Not패킷에 저장
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	kNot.m_fItemDropEventProbRate = m_fItemDropEventProbRate;
#else // SERV_DROP_EVENT_RENEWAL
	kNot.m_iItemDropEventProbCount = m_fItemDropEventProbRate;
#endif // SERV_DROP_EVENT_RENEWAL
	kNot.m_bWithPlayPcBangEvent = m_bWithPlayPcBangEvent;
#else // SERV_DROP_EVENT_RENEWAL
	if( SiKGameEventManager()->CheckItemDropProbEvent( m_iItemDropEventProbCount, m_bWithPlayPcBangEvent ) == true )
	{
		// PC방 유저와 함께 플레이 해야만 이벤트가 적용되는 경우! 방안에 PC방 유저가 있는지 체크하자!
		if( m_bWithPlayPcBangEvent )
		{
			if( m_spRoomUserManager->IsExistPcBangPlayer() == false )
			{
				// 디폴트값 넣어준다!
				m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
				m_bWithPlayPcBangEvent = false;
			}
		}
	}
	else
	{
		m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
		m_bWithPlayPcBangEvent = false;
	}

	// Not패킷에 저장
	kNot.m_iItemDropEventProbCount = m_iItemDropEventProbCount;
	kNot.m_bWithPlayPcBangEvent = m_bWithPlayPcBangEvent;
#endif // SERV_DROP_EVENT_RENEWAL
#endif //DROPEVENT_RENEWAL
#endif SERV_PC_BANG_DROP_EVENT
	//}}	

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

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
	//#ifdef SERV_SECRET_HELL
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// NPC UID 발급 처리!

	//{{ 2009. 5. 13  최육사	서브스테이지별 drop npc수
	m_mapSubStageExpNpcNum.clear();
	//}}

	//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	m_mapSubStageActiveNpcNum.clear();
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}

	std::map< int, KNPCList >::iterator mit;
	for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
	{
		const int iSubStageID = mit->first;
		KNPCList& kNPCList = mit->second;

		//////////////////////////////////////////////////////////////////////////	
		//{{ 2010. 7. 28  최육사	오토핵 차단 NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
		if( kNPCList.m_NPCList.empty() == false )
		{
			KNPCUnitReq kNPCData;
			kNPCData.Init();

			//NPC DATA SET..
			kNPCData.m_NPCID			= CXSLUnitManager::NUI_DUNGEON_CHECKER1 + ( rand() % 5 );
			kNPCData.m_KeyCode			= -1;
			kNPCData.m_bFocusCamera		= false;
			kNPCData.m_nStartPos		= SiCXSLDungeonManager()->GetStartPosByRandom( GetDungeonIDAndDif(), GetStageID(), iSubStageID );
			kNPCData.m_bActive			= false;
			kNPCData.m_bShowGage		= false;
			kNPCData.m_Level			= SiKGameSysVal()->GetLimitsLevel();
			kNPCData.m_bNoDrop			= true;

			if( kNPCData.m_nStartPos != -1 )
			{
				kNPCList.m_NPCList.push_back( kNPCData );
			}
			else
			{
				// 출현 위치가 -1이면 이미 출현 결정된 NPC의 위치를 주자!
				BOOST_TEST_FOREACH( const KNPCUnitReq&, kNpc, kNPCList.m_NPCList )
				{
					kNPCData.m_nStartPos = kNpc.m_nStartPos;
					kNPCList.m_NPCList.push_back( kNPCData );
					break;
				}
			}
		}
#endif SERV_AUTO_HACK_CHECK_NPC
		//}}
		//////////////////////////////////////////////////////////////////////////

		
		int iExpNpcNum = 0;

		//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
		int iSubStageActiveNpc = 0;
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}

		for( int i = 0; i < (int)kNPCList.m_NPCList.size(); ++i )
		{
			const KNPCUnitReq& kNpcInfo = kNPCList.m_NPCList[i];

			// 몬스터 생성!
			LIF( m_kDungeonMonsterManager.CreateMonster( kNpcInfo, kNPCList.m_NPCList[i].m_UID ) );
			const int iNpcUID = kNPCList.m_NPCList[i].m_UID;

			//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
			if( kNPCList.m_NPCList[i].m_NPCID == CXSLUnitManager::NUI_DEFENCE_CRYSTAL )
			{
				if( m_iDefenseMainCrystalNpcUID != 0 )
				{
					START_LOG( cerr, L"큰 수정이 1 보다 많다? 절대로 일어나서는 안되는 에러!" )
						<< BUILD_LOG( m_iDefenseMainCrystalNpcUID )
						<< BUILD_LOG( kNPCList.m_NPCList[i].m_NPCID )
						<< END_LOG;
				}
				else
				{
					m_iDefenseMainCrystalNpcUID = iNpcUID;
				}
			}
			if( kNPCList.m_NPCList[i].m_NPCID == CXSLUnitManager::NUI_DEFENCE_CRYSTAL_SMALL )
			{
				if( m_setDefenseSmallCrystalNpcUID.size() > MAX_SMALL_CRYSTAL )
				{
					START_LOG( cerr, L"작 수정이 4 보다 많다? 절대로 일어나서는 안되는 에러!" )
						<< BUILD_LOG( m_setDefenseSmallCrystalNpcUID.size() )
						<< END_LOG;
				}
				else
				{
					m_setDefenseSmallCrystalNpcUID.insert( iNpcUID );
				}
			}
#endif SERV_INSERT_GLOBAL_SERVER
			//}} 2011. 04. 13  김민성  글로벌 서버 추가

			//////////////////////////////////////////////////////////////////////////			
			//{{ 2009. 5. 13  최육사	서브스테이지별 경험치 주는 npc수
			if( m_eDungeonType == CXSLDungeon::DT_HENIR )
			{
				if( SiKDropTable()->IsHenirExpDropNpc( static_cast<CXSLDungeon::DUNGEON_MODE>(m_cDungeonMode), kNpcInfo.m_NPCID ) == true  /*&&  !npcData.m_bNoDrop*/ )
				{
					iExpNpcNum++;
				}
				//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
				if( kNpcInfo.m_bActive == true )
				{
					iSubStageActiveNpc++;
				}
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
				//}}
			}
			else
			{
				if( SiKDropTable()->IsExpDropNpc( m_iDungeonID + static_cast<int>(m_cDifficultyLevel), kNpcInfo.m_NPCID ) == true /*&&  !npcData.m_bNoDrop*/ )
				{
					iExpNpcNum++;
				}
				//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
				if( kNpcInfo.m_bActive == true )
				{
					iSubStageActiveNpc++;
				}
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
				//}}
			}
			//}}
			//////////////////////////////////////////////////////////////////////////
		}

		// 해킹 체크를 위해 서브 스테이지 NPC정보를 등록하자!
		m_kDungeonMonsterManager.InsertSubStageNpcList( GetStageID(), iSubStageID, kNPCList.m_NPCList );

		// drop npc수
		m_mapSubStageExpNpcNum.insert( std::make_pair( iSubStageID, iExpNpcNum ) );

		//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
		m_mapSubStageActiveNpcNum.insert( std::make_pair( iSubStageID, iExpNpcNum ) );
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}
	}

	//////////////////////////////////////////////////////////////////////////
	// 속성NPC 세팅 - NPC UID발급과는 관계 없습니다.
	if( eStageType == CXSLDungeonStage::ST_EXTRA )
	{
		// 첫번째 Extra Stage진입인가?
		if( m_iExtraStageLoadCount == 1 )
		{
			// 알테라 비던의 경우에만 플레이 시간 초기화!
			if( CXSLDungeon::IsAlteraDungeon( GetDungeonIDAndDif() ) == true )
			{
				// 남은 시간 초기화
				SetRemainingPlayTime( GetPlayTimeLimit() );
				BroadCast( ERM_REMAINING_PLAY_TIME_NOT, GetRemainingPlayTime() );

				// EXTRA STAGE 전용 독대미지 타임을 얻자!
				m_fPoisonTimeLimit = SiCXSLDungeonManager()->GetExtraStagePoisonTimeLimit( GetDungeonIDAndDif() );

				KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT kNot;
				kNot.m_fRemainingTime = m_fPoisonTimeLimit;
				BroadCast( ERM_DUNGEON_EFFECT_REMAINING_TIME_NOT, kNot );
			}
		}

		SecretDungeonExtraStageAttribMonsterDrop( kNot );
	}
	else
	{
		NormalDungeonAttribMonsterDrop( kNot );
	}

	//////////////////////////////////////////////////////////////////////////	
	//#endif SERV_SECRET_HELL
	//}}
	//////////////////////////////////////////////////////////////////////////

	BroadCast( ERM_DUNGEON_STAGE_LOAD_NOT, kNot );

	//{{ 2012. 12. 03 던전 스테이지 중복 로딩 예외 확인 - 김민성
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD_2
	// DUNGEON_STAGE_LOAD_NOT 를 보내야지 등록한다.
	{
		// 로딩하지 않은 StageID를 저장한다.
		m_mapStageLoadingCheck.insert( std::make_pair( kPacket_.m_iStageID, 1 ) );

		// 마지막으로 로딩 성공한 유저의 UnitUID를 저장!
		m_iLastLoadingUnitUID = FIRST_SENDER_UID;
	}
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD_2
	//}
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

//IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
//{
//	// 의도적인 Leave Room 직후 접속이 끊겼을 때 한 번 더 Leave Room 하게 되는 경우가 있다.
//	KEGS_LEAVE_ROOM_ACK kPacket;
//	if( GetStateID() == KRoomFSM::S_INIT || GetStateID() == KRoomFSM::S_CLOSE )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_09;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_ROOM_ACK, kPacket );
//		return;
//	}
//
//	// 이탈자 기록.
//	if( GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY )
//	{
//
//		//=== 게임중 이탈 결과처리 ===
//		START_LOG( clog, L"=== DUNGEON GAME : 게임중 이탈시 결과처리 ===" )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< END_LOG;
//
//		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
//
//		if( !spRoomUser )
//		{
//			spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
//		}
//
//		if( !spRoomUser )
//		{
//			START_LOG( cerr, L"룸 유저 포인터 이상" )
//				<< BUILD_LOG( FIRST_SENDER_UID )
//				<< END_LOG;
//		}
//		else
//		{
//			if( spRoomUser->IsObserver() == false )
//			{
//				//튜토리얼 모드이면 패널티가 없음..
//				int iEXP = 0;
//				int iED = 0;
//				if( CXSLDungeon::IsTutorialDungeon( m_iDungeonID ) == false )
//				{
//					KERM_UPDATE_DUNGEON_UNIT_INFO_NOT kPacket;
//					kPacket.m_bOutRoom = true;
//					kPacket.m_iUserCount = GetNumMember();
//					kPacket.m_iDungeonID = m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
//					kPacket.m_iPlayTime = ( int )( m_kTimer[TM_PLAY].elapsed() + 0.5 - GetLoadingTime() );
//					kPacket.m_iEXP = 0;
//
//					if( spRoomUser->IsDie() == true )
//					{
//						// 실시간 경험치
//						m_spRoomUserManager->GetRewardEXP( FIRST_SENDER_UID, iEXP );
//						m_spRoomUserManager->GetRewardPartyEXP( FIRST_SENDER_UID, iEXP );
//
//						//개인별 아이템 정보주기
//						m_spRoomUserManager->GetItemList( FIRST_SENDER_UID, kPacket.m_mapGetItem );
//					}
//					else
//					{
//						// ED페널티
//						SiKResultProcess()->Result_DUNGEON_BREAK( spRoomUser, kPacket );
//					}
//
//					// [중요] 클라이언트 크래쉬 및 해킹으로 인한 방 이탈일 경우 게임서버로 보상패킷을 보내지 않습니다!
//					if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_23 )
//					{
//						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_UPDATE_DUNGEON_UNIT_INFO_NOT, kPacket );
//
//						//{{ 2009. 5. 14  최육사	내구도 개편
//						KERM_DECREASE_ENDURANCE_DUNGEON_NOT kNot;
//						kNot.m_iEnduranceDamage = CXSLDungeon::DE_LEAVE_ROOM_ENDURANCE_DAMAGE;
//						kNot.m_cPartyNumMember  = kPacket.m_iUserCount;
//						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DECREASE_ENDURANCE_DUNGEON_NOT, kNot );
//						//}}
//					}
//
//					iED = kPacket.m_iED;
//				}
//
//				//////////////////////////////////////////////////////////////////////////
//				// 던전 일일 이탈 로그
//				// 운영자는 통계를 남기지 않습니다.
//				if( spRoomUser->GetAuthLevel() >= SEnum::UAL_GM )
//				{
//					START_LOG( cout, L"운영자는 통계데이터를 남기지 않습니다." )
//						<< BUILD_LOGc( spRoomUser->GetAuthLevel() )
//						<< BUILD_LOG( spRoomUser->GetNickName() );
//				}
//				else
//				{
//					KRoomUserInfo kInfo;
//					spRoomUser->GetRoomUserInfo( kInfo );
//
//					int iClear = ( spRoomUser->IsDie() ? KE_LOCAL_LOG_DUNGEON_NOT::DCT_UNIT_DIE_LEAVE_ROOM : KE_LOCAL_LOG_DUNGEON_NOT::DCT_UNIT_ALIVE_LEAVE_ROOM ); // 2. 캐릭터 죽고 이탈   3. 캐릭터 안죽고 이탈	
//
//					if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_23 ) // 4. 클라이언트 크래쉬 이탈
//					{
//						iClear = KE_LOCAL_LOG_DUNGEON_NOT::DCT_CLIENT_CRASH_LEAVE_ROOM;
//					}
//					else if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_25 ) // 5. 클라이언트 해킹으로 인한 이탈
//					{
//						iClear = KE_LOCAL_LOG_DUNGEON_NOT::DCT_CLIENT_HACKING_LEAVE_ROOM;
//					}
//
//					//{{ 2010. 06. 21  최육사	통계 스레드
////#ifdef SERV_STATISTICS_THREAD
//					//{{ 2010. 11. 8	최육사	던전 로그 DB
////#ifdef SERV_DUNGEON_LOG_DB
//					CTime kRegDate = CTime::GetCurrentTime();
////#endif SERV_DUNGEON_LOG_DB
//					//}}
//					//////////////////////////////////////////////////////////////////////////					
//					KE_LOCAL_LOG_DUNGEON_NOT kNot;
//					kNot.m_iDungeonID				= (m_iDungeonID + static_cast<int>(m_cDifficultyLevel));
//					kNot.m_nUnitUID					= kInfo.m_nUnitUID;
//					kNot.m_wstrNickName				= kInfo.m_wstrNickName;
//					kNot.m_iDifficultyLevel			= (int) m_cDifficultyLevel;
//					kNot.m_iIsChallenge				= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
//					kNot.m_iUnitClass				= (int) kInfo.m_cUnitClass;
//					kNot.m_iClear					= iClear;
//					kNot.m_iStartNumMember			= m_iStartedNumMember;
//					kNot.m_iEndNumMember			= GetNumMember();
//					kNot.m_ucLevel					= kInfo.m_ucLevel;
//					kNot.m_iPlayTime				= (int) spRoomUser->GetPlayTime();
//					kNot.m_iEXP						= iEXP;
//					kNot.m_iED						= iED;
//					kNot.m_cComboRank				= CXSLDungeon::RT_F;
//					kNot.m_iComboScore				= spRoomUser->GetComboScore_LUA();
//					kNot.m_cTechnicalRank			= CXSLDungeon::RT_F;
//					kNot.m_iTechnicalScore			= spRoomUser->GetTechnicalScore_LUA();
//					kNot.m_cTimeRank				= CXSLDungeon::RT_F;
//					kNot.m_cDamagedRank				= CXSLDungeon::RT_F;
//					kNot.m_iDamageNum				= spRoomUser->GetDamageNum_LUA();
//					kNot.m_cTotalRank				= CXSLDungeon::RT_F;
//					kNot.m_iRessurectionStoneCount	= spRoomUser->GetRessurectionStoneCount();
//					kNot.m_iPassedStageCount		= spRoomUser->GetPassedStageCount();
//					kNot.m_iPassedSubStageCount		= spRoomUser->GetPassedSubStageCount();
//					//{{ 2010. 9. 14	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//					kNot.m_bIsWithPet				= spRoomUser->HavePet();
//#endif SERV_PET_SYSTEM
//					//}}
//					//{{ 2010. 11. 8	최육사	던전 로그 DB
//#ifdef SERV_DUNGEON_LOG_DB
//					kNot.m_wstrRegDate				= (const wchar_t*)kRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
//#endif SERV_DUNGEON_LOG_DB
//					//}}
//					//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
//					kNot.m_sNpcDieCount				= spRoomUser->GetKillNPC_LUA();
//					kNot.m_iGivenDamage				= 0;
//					kNot.m_iAttackDamage			= 0;
//					kNot.m_sUsingSkillKind			= 0;
//					kNot.m_iTotalEXP				= ( spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP() );
//					kNot.m_cPetID					= spRoomUser->GetPetID();
//					kNot.m_cPetEvoStep				= spRoomUser->GetPetEvolutionStep();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN
//					//}}
//					//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//					kNot.m_iBaseHP					= spRoomUser->GetBaseHP_LUA();
//					kNot.m_iEndHP					= spRoomUser->GetLastUpdatedHP();
//					kNot.m_iRecovHP					= spRoomUser->GetTotalRecoveryHP_LUA();
//					kNot.m_iContinue				= spRoomUser->GetUsedRessurectionStoneCount();
//					kNot.m_iStartContinue			= spRoomUser->GetStartTimeNumResurrectionStone();
//					kNot.m_iContinueUseStage		= spRoomUser->GetFirstUseResurrectionStoneStageID();
//					kNot.m_iContinueUseSub			= spRoomUser->GetFirstUseResurrectionStoneSubStageID();
//					kNot.m_iFrame					= spRoomUser->GetInGameAverageFrame();
//					kNot.m_wstrChannelIP			= spRoomUser->GetGameChannelIP();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//					//}}
//					//{{ 2012. 07. 11 김민성
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
//					kNot.m_iUserUID					= spRoomUser->GetUserUID();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
//					//}}
//					//{{ 2012. 11. 6	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//					kNot.m_cAutoPartyPlay			= ( IsStartedByAutoParty() ? 1 : 0 );
//#endif SERV_BATTLE_FIELD_SYSTEM
//					//}}
//					KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_NOT, kNot );
//					//////////////////////////////////////////////////////////////////////////
//					//{{ 2010. 11. 8	최육사	던전 로그 DB
////#ifdef SERV_DUNGEON_LOG_DB
//					SendToLogDB( DBE_DB_LOG_DUNGEON_NOT, kNot );
////#endif SERV_DUNGEON_LOG_DB
//					//}}
////#endif SERV_STATISTICS_THREAD
//					//}}
//				}
//				//////////////////////////////////////////////////////////////////////////
//
//				//{{ 2007. 12. 18  최육사  유저 통계 [수정]
//				KERM_DUNGEON_USER_STATISTICS_NOT kPacketNot;
//				kPacketNot.m_iDungeonID	   = m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
//				kPacketNot.m_cGameResult   = KERM_DUNGEON_USER_STATISTICS_NOT::UGR_DROP;
//				kPacketNot.m_bIsPartyPlay  = ( m_iStartedNumMember > 1 );
//				kPacketNot.m_iGamePlayTime = ( int )( m_kTimer[TM_PLAY].elapsed() + 0.5 - GetLoadingTime() );
//				kPacketNot.m_iUsedResurrectionStone = m_spRoomUserManager->GetRessurectionStoneCount( FIRST_SENDER_UID );
//				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_USER_STATISTICS_NOT, kPacketNot );
//				//}}
//
//				//{{ 2007. 12. 25  최육사  부활석 통계
//				if( m_spRoomUserManager->GetRessurectionStoneCount( FIRST_SENDER_UID ) > 0 )
//				{
//					KStatisticsKey kKey;
//					kKey.m_vecIntKey.push_back( 0 );
//					KSIManager.IncreaseCount( KStatistics::SI_RES_STONE, kKey, KStatistics::eSIColResStone_PlayerCount, 1 );
//				}
//				//}}
//			}
//		}
//	}
//
//	kPacket.m_iOK = NetError::NET_OK;
//
//	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
//	//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
//	//던전게임 진행아이템이 없을경우 처리..
//	if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_22 )
//	{
//		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
//		kPacket.m_iItemID = kPacket_.m_iItemID;
//	}
//	//{{ 2011. 11. 1	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//	else if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_27 )
//	{
//		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
//		kPacket.m_iBattleFieldID = kPacket_.m_iBattleFieldID;
//	}
//#endif SERV_BATTLE_FIELD_SYSTEM
//	//}}
//	else
//	{
//		kPacket.m_iReason = NetError::NET_OK;
//		kPacket.m_iItemID = 0;
//	}
//	//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//	//}}
//
//	// 슬롯 정보 변경.
//	if( !m_spRoomUserManager->LeaveRoom( FIRST_SENDER_UID ) )
//	{
//		// 슬롯에서 나가기 실패.
//		kPacket.m_iOK = NetError::ERR_SLOT_04;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_ROOM_ACK, kPacket );
//		return;
//	}
//	else
//	{
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_ROOM_ACK, kPacket );
//	}
//
//	if( IsEmpty() == false || IsEmpty( KRoomUserManager::UT_OBSERVER ) == false )
//	{
//		// 방의 모든 유저들에게 슬롯 정보를 날린다.
//		KEGS_LEAVE_ROOM_NOT kPacketNot;
//		kPacketNot.m_cRoomState = GetStateID();
//		kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
//		GetRoomSlotInfo( kPacketNot.m_vecSlot );
//		GetRoomSlotInfo( kPacketNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
//		BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
//	}
//
//	if( (GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY) && IsEmpty() == false )
//	{
//		//{{ 2010. 11. 23	최육사	방 로딩 오류 수정 - 크래쉬 문제가 있어서 적용은 보류합니다.
//#ifdef SERV_ROOM_LOADING_BUG_FIX
//		//////////////////////////////////////////////////////////////////////////
//		if( GetStateID() == KRoomFSM::S_LOAD )
//		{
//			if( m_spRoomUserManager->IsAllPlayerFinishLoading() == true )
//			{
//				BroadCastID( ERM_GAME_LOADING_ALL_UNIT_OK_NOT );
//
//				// 스테이지를 초기화.
//				StartPlay();
//
//				KEGS_PLAY_START_NOT kNot;
//				//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
//#ifdef SERV_CHAR_LOG
//				kNot.m_iDungeonID = GetDungeonIDAndDif();
//#endif SERV_CHAR_LOG
//				//}}
//				kNot.m_RoomState = GetStateID();
//				GetRoomSlotInfo( kNot.m_vecSlot );
//
//				// 모든 유저에게 플레이 시작을 알림.
//				BroadCast( ERM_PLAY_START_NOT, kNot );
//
//				// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
//				SendRoomListInfo( NetError::ERR_ROOM_15 );
//
//				//{{ 2009. 4. 23  최육사	대전유저리스트
//				SendAllPVPUserInfo();
//				//}}
//			}
//		}
//		else if( GetStateID() == KRoomFSM::S_PLAY )
//		{
//			CheckLoadingSuccess();
//		}
//		//////////////////////////////////////////////////////////////////////////	
//#else
//		//////////////////////////////////////////////////////////////////////////
//		CheckLoadingSuccess();
//		//////////////////////////////////////////////////////////////////////////
//#endif SERV_ROOM_LOADING_BUG_FIX
//		//}}
//	}
//
//	// 스테이지가 끝나게 되는 경우 처리.
//	if( GetStateID() == KRoomFSM::S_PLAY && IsEmpty() == false )
//	{
//		if( CheckIfPlayEnd() )
//		{
//			EndPlay();
//		}
//		else
//		{
//			//{{ 2009. 7. 27  최육사	비밀스테이지
//			if( m_eDungeonType == CXSLDungeon::DT_HENIR )
//			{
//				CheckEnterSecretStageSuccess();
//			}
//			//}}
//		}
//	}
//
//	//결과창을 보고있을때 방을 나갈경우처리
//	if( GetStateID() == KRoomFSM::S_RESULT && IsEmpty() == false )
//	{
//		CheckResultSuccess();
//	}
//
//	// 방에서 모든 유저가 나갔으면 방을 닫는다.
//	if( IsEmpty() == true && IsEmpty( KRoomUserManager::UT_OBSERVER ) == true )
//	{
//		StateTransition( KRoomFSM::I_TO_CLOSE );
//
//		//{{ 2010. 06. 21  최육사	통계 스레드
//#ifdef SERV_STATISTICS_THREAD
//		CTime kEndGameTime = CTime::GetCurrentTime();
//		//////////////////////////////////////////////////////////////////////////
//		KE_LOCAL_LOG_DUNGEON_ROOM_NOT kNot;
//		kNot.m_iDungeonID		= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
//		kNot.m_iDifficultyLevel	= (int) GetDifficultyLevel();
//		kNot.m_iIsChallenge		= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
//		kNot.m_iWin				= 0;
//		kNot.m_iLose			= 0;
//		kNot.m_iStartNumMember	= m_iStartedNumMember;
//		kNot.m_iEndNumMember	= GetNumMember();
//		kNot.m_iTotalPlayTime	= static_cast<int>(GetTotalPlayTime());
//		kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
//		KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_ROOM_NOT, kNot );
//		//////////////////////////////////////////////////////////////////////////
//#endif SERV_STATISTICS_THREAD
//		//}}
//	}
//	else
//	{
//		if( m_kDSSGoNextAllNot.m_iBeforeStageID != -1 &&
//			m_kDSSGoNextAllNot.m_iNextStageID	!= -1 )
//			SendDSSGoNextAllNot( m_kDSSGoNextAllNot );
//	}
//}

//_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_REQ, KEGS_DUNGEON_STAGE_LOAD_REQ )
//{
//	//////////////////////////////////////////////////////////////////////////
//	// 상태 오류 케이스
//	// 1. S_RESULT : 2명이상 파티플중 한명 죽은상태에서 다른 한명이 스테이지 클리어와 동시에 던전이탈을 하면 발생
//	//////////////////////////////////////////////////////////////////////////
//
//	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_STAGE_LOAD_ACK, KEGS_DUNGEON_STAGE_LOAD_ACK );
//
//	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
//	// 디펜스 던전용 초기화
//	m_iDefenseMainCrystalNpcUID = 0;
//	m_setDefenseSmallCrystalNpcUID.clear();
//#endif SERV_INSERT_GLOBAL_SERVER
//	//}} 2011. 04. 13  김민성  글로벌 서버 추가
//
//	//{{ 2010. 11. 18	최육사	오토 핵 스테이지 로딩 체크
//#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
//	{
//		std::map< int, int >::iterator mitSLC = m_mapStageLoadingCheck.find( kPacket_.m_iStageID );
//		if( mitSLC != m_mapStageLoadingCheck.end() )
//		{
//			START_LOG( cout, L"똑같은 스테이지 또 로딩했네? 이거 괜찮은거임?" )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOG( FIRST_SENDER_UID )
//				<< BUILD_LOG( GetDungeonIDAndDif() )
//				<< BUILD_LOG( GetStageID() )
//				<< BUILD_LOG( GetSubStageID() )
//				<< BUILD_LOG( m_mapStageLoadingCheck.size() )
//				<< BUILD_LOG( mitSLC->second )
//				<< BUILD_LOG( kPacket_.m_iStageID )
//				<< BUILD_LOGc( kPacket_.m_cPartyFever )
//				<< BUILD_LOG( m_spRoomUserManager->GetNumMember() );
//
//			if( m_iLastLoadingUnitUID != FIRST_SENDER_UID )
//			{
//				START_LOG( cout, L"방장이 바뀌었나? 이 전 스테이지 로딩한 유저와 다른 유저군.." )					
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( m_iLastLoadingUnitUID );
//			}
//
//			// 동일 스테이지 로딩 카운트를 1 올리자!
//			++mitSLC->second;
//
//			// 마지막으로 로딩 성공한 유저의 UnitUID를 저장!
//			m_iLastLoadingUnitUID = FIRST_SENDER_UID;
//
//			// 5번 이상 똑같은 스테이지 로딩하는 경우라면 해킹유저로 판별!
//			if( mitSLC->second >= 5 )
//			{
//				START_LOG( cout, L"Stage Loading 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
//					<< BUILD_LOG( LAST_SENDER_UID )
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( GetDungeonIDAndDif() )
//					<< BUILD_LOG( GetStageID() )
//					<< BUILD_LOG( m_mapStageLoadingCheck.size() )
//					<< BUILD_LOG( mitSLC->second )
//					<< BUILD_LOG( kPacket_.m_iStageID );
//
//				kPacket.m_iOK = NetError::ERR_ROOM_22;
//				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
//
//				// 파티장에게 해킹체크패킷을 보내자!
//				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
//				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_STAGE_LOADING;
//				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
//				return;
//			}
//		}
//		else
//		{
//			// 로딩하지 않은 StageID를 저장한다.
//			m_mapStageLoadingCheck.insert( std::make_pair( kPacket_.m_iStageID, 1 ) );
//
//			// 마지막으로 로딩 성공한 유저의 UnitUID를 저장!
//			m_iLastLoadingUnitUID = FIRST_SENDER_UID;
//		}
//	}	
//#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
//	//}}
//
//	//로딩시간 체크를 시작함..
//	//이곳에서 시작을 할경우 2가지의 경우에 로딩시간을 체크하게 된다.
//	//::1. 게임초기 로딩시
//	//::2. 스테이지 로딩
//	//## 장막로딩은 빼기로 함.
//	m_LoadingTimer.Start();
//
//	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
//#ifdef SERV_SECRET_HELL
//	const CXSLDungeonStage::STAGE_TYPE eStageType = SiCXSLDungeonManager()->GetStageType( GetDungeonIDAndDif(), kPacket_.m_iStageID );
//#endif SERV_SECRET_HELL
//	//}}
//
//	KEGS_DUNGEON_STAGE_LOAD_NOT kNot;
//	kNot.m_iNumMember = m_spRoomUserManager->GetNumMember();
//
//	int iHighestUserLevelAtDungeonStart = m_iHighestUserLevelAtDungeonStart;
//
//	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
//#ifdef SERV_SECRET_HELL
//	if( m_eDungeonType == CXSLDungeon::DT_SECRET )
//	{
//		// 비밀던전의 경우 레벨 보정 적용
//		{
//			const int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( GetDungeonIDAndDif() );
//			if( iHighestUserLevelAtDungeonStart < iDungeonMinLevel )
//			{
//				iHighestUserLevelAtDungeonStart = iDungeonMinLevel;
//			}
//
//			const int iDungeonMaxLevel = SiCXSLDungeonManager()->GetDungeonMaxLevel( GetDungeonIDAndDif() );
//			if( iHighestUserLevelAtDungeonStart > iDungeonMaxLevel )
//			{
//				iHighestUserLevelAtDungeonStart = iDungeonMaxLevel;
//			}
//		}
//
//		if( m_cDungeonMode == CXSLDungeon::DM_SECRET_HELL )
//		{
//			// 비밀던전 헬모드 EXTRA STAGE
//			if( eStageType == CXSLDungeonStage::ST_EXTRA )
//			{
//				// Extra Stage Load Count
//				++m_iExtraStageLoadCount;
//
//				iHighestUserLevelAtDungeonStart = iHighestUserLevelAtDungeonStart + 5 + SiCXSLDungeonManager()->GetSecretDungeonExtraStageNpcLevel();
//			}
//			// 비밀던전 헬모드
//			else
//			{
//				iHighestUserLevelAtDungeonStart = iHighestUserLevelAtDungeonStart + 5;
//			}
//		}
//	}
//#endif SERV_SECRET_HELL
//	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
//	else
//	{
//		if( CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) )
//		{
//			const int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( GetDungeonIDAndDif() );
//			if( iHighestUserLevelAtDungeonStart < iDungeonMinLevel )
//			{
//				iHighestUserLevelAtDungeonStart = iDungeonMinLevel;
//			}
//		}
//	}
//#endif SERV_INSERT_GLOBAL_SERVER
//	//}} 2011. 04. 13  김민성  글로벌 서버 추가
//
//	// 보스 스테이지 여부
//	bool bIsBossStage = false;
//
//	// NPC정보 불러오기!
//	if( SiCXSLDungeonManager()->GetNPCData( GetDungeonIDAndDif(), kPacket_.m_iStageID, iHighestUserLevelAtDungeonStart, kNot, bIsBossStage ) == false )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_22;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
//		return;
//	}
//	else
//	{
//		// 정상적인 StageID라고 판단되면 현재 스테이지ID 정보로 업데이트 하자!
//		SetStageID( kPacket_.m_iStageID );
//
//		// 정상적인 StageID값이므로 클라이언트로 OK값 보냄.
//		kPacket.m_iOK = NetError::NET_OK;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
//	}
//	//}}	
//
//	//////////////////////////////////////////////////////////////////////////
//	//{{ 2010. 07. 16  최육사	이벤트 몬스터 시스템	
//#ifdef SERV_EVENT_MONSTER
//	if( CXSLDungeon::IsTutorialDungeon( GetDungeonID() ) == false  &&
//		CXSLDungeon::IsTCDungeon( GetDungeonID() ) == false  &&
//		CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == false 
//		//{{ 2011. 07. 25    김민성    이벤트 몬스터 출현 던전 제한
//#ifdef SERV_EVENT_MONSTER_DUNGEON_RESTRICT
//		&& CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) == false    // 디펜스 던전에는 이벤트 몬스터가 출현하지 않도록한다.
//#endif SERV_EVENT_MONSTER_DUNGEON_RESTRICT
//		//}} 
//		)
//	{
//		BOOST_TEST_FOREACH( KGameEventScriptManager::KAdventNpcEventInfo&, kInfo, m_vecEventMonster )
//		{
//			// 보스 스테이지에서 등장 시킬지 체크!
//			if( kInfo.m_bIsAdventInBossStage == false  &&  bIsBossStage == true )
//				continue;
//
//			// 더이상 출현 시킬 카운트가 남아있지 않다면 패스!
//			if( kInfo.m_iMonsterDropCount <= 0 )
//				continue;
//
//			// 등장할 수 있는 던전인지 체크!
//			if( kInfo.m_setAdventDungeon.empty() == false )
//			{
//				if( kInfo.m_setAdventDungeon.find( GetDungeonID() ) == kInfo.m_setAdventDungeon.end() )
//					continue;
//			}
//
//			// 등장 하면 안되는 던전인지 체크!
//			if( kInfo.m_setNoAdventDungeon.empty() == false )
//			{
//				if( kInfo.m_setNoAdventDungeon.find( GetDungeonID() ) != kInfo.m_setNoAdventDungeon.end() )
//					continue;
//			}
//
//			// 등장 하면 안되는 스테이지 인지 체크!
//			if( kInfo.m_setNoAdventStage.empty() == false )
//			{
//				if( kInfo.m_setNoAdventStage.find( GetStageID() ) != kInfo.m_setNoAdventStage.end() )				
//					continue;
//			}
//
//			// 등장할 몬스터를 결정하자!
//			const int iAdventNpcID = kInfo.m_kAdventNpcLot.Decision();
//			if( iAdventNpcID == KLottery::CASE_BLANK )
//				continue;
//
//			// 이벤트 몬스터 등장 처리!
//			if( EventDropMonster( iAdventNpcID, GetStageID(), kNot ) == true )
//			{
//				// 몬스터 등장 성공했다면 카운트 하나 깎자!
//				--kInfo.m_iMonsterDropCount;
//			}
//		}
//	}
//#endif SERV_EVENT_MONSTER
//	//////////////////////////////////////////////////////////////////////////
//
//	//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
//#ifdef SERV_TIME_DROP_MONSTER_EVENT
//	if( CXSLDungeon::IsTutorialDungeon( GetDungeonID() ) == false  &&
//		CXSLDungeon::IsTCDungeon( GetDungeonID() ) == false  &&
//		CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == false )
//	{
//		if( m_mapTimeDropEventMonster.empty() == true )
//		{
//			int iTimeDropNpcEventUID = 0;
//			int iTimeDropEventNpcID = 0;
//			if( SiKDungeonEventManager()->CheckTimeDropEventMonster( GetDungeonIDAndDif(), GetStageID(), kNot, iTimeDropNpcEventUID, iTimeDropEventNpcID ) == true )
//			{
//				m_mapTimeDropEventMonster.insert( std::make_pair( iTimeDropEventNpcID, iTimeDropNpcEventUID ) );
//			}
//		}
//	}
//#endif SERV_TIME_DROP_MONSTER_EVENT
//	//}}
//
//	//{{ 2010. 03. 21  최육사	비밀던전 헬모드
//#ifdef SERV_SECRET_HELL
//
//	if( m_cDungeonMode == CXSLDungeon::DM_SECRET_HELL  &&  eStageType == CXSLDungeonStage::ST_EXTRA_NPC )
//	{
//		// 비밀던전 extra 몬스터 등장 처리
//		SecretDungeonExtraStageMonster( GetStageID(), kPacket_.m_cPartyFever, kNot );
//	}
//#endif SERV_SECRET_HELL
//	//}}
//
//	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장
//#ifdef SERV_PC_BANG_DROP_EVENT	
//	if( SiKGameEventManager()->CheckItemDropProbEvent( m_iItemDropEventProbCount, m_bWithPlayPcBangEvent ) == true )
//	{
//		// PC방 유저와 함께 플레이 해야만 이벤트가 적용되는 경우! 방안에 PC방 유저가 있는지 체크하자!
//		if( m_bWithPlayPcBangEvent )
//		{
//			if( m_spRoomUserManager->IsExistPcBangPlayer() == false )
//			{
//				// 디폴트값 넣어준다!
//				m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
//				m_bWithPlayPcBangEvent = false;
//			}
//		}
//	}
//	else
//	{
//		m_iItemDropEventProbCount = REE_DROP_EVENT_PROB_DEFAULT;
//		m_bWithPlayPcBangEvent = false;
//	}
//
//	// Not패킷에 저장
//	kNot.m_iItemDropEventProbCount = m_iItemDropEventProbCount;
//	kNot.m_bWithPlayPcBangEvent = m_bWithPlayPcBangEvent;
//#endif SERV_PC_BANG_DROP_EVENT
//	//}}	
//
//	//////////////////////////////////////////////////////////////////////////	
//	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
//	//#ifdef SERV_SECRET_HELL
//	//////////////////////////////////////////////////////////////////////////
//
//	//////////////////////////////////////////////////////////////////////////
//	// NPC UID 발급 처리!
//
//	NPC_DATA	npcData;
//
//	//{{ 2009. 5. 13  최육사	서브스테이지별 drop npc수
//	m_mapSubStageExpNpcNum.clear();
//	//}}
//
//	//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
//	m_mapSubStageActiveNpcNum.clear();
//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
//	//}}
//
//	std::map< int, KNPCList >::iterator mit;
//	for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
//	{
//		const int iSubStageID = mit->first;
//		KNPCList& kNPCList = mit->second;
//
//		//////////////////////////////////////////////////////////////////////////	
//		//{{ 2010. 7. 28  최육사	오토핵 차단 NPC
//#ifdef SERV_AUTO_HACK_CHECK_NPC
//		if( kNPCList.m_NPCList.empty() == false )
//		{
//			KNPCUnitReq kNPCData;
//			kNPCData.Init();
//
//			//NPC DATA SET..
//			kNPCData.m_NPCID			= CXSLUnitManager::NUI_DUNGEON_CHECKER1 + ( rand() % 5 );
//			kNPCData.m_KeyCode			= -1;
//			kNPCData.m_bFocusCamera		= false;
//			kNPCData.m_nStartPos		= SiCXSLDungeonManager()->GetStartPosByRandom( GetDungeonIDAndDif(), GetStageID(), iSubStageID );
//			kNPCData.m_bActive			= false;
//			kNPCData.m_bShowGage		= false;
//			kNPCData.m_Level			= SiKGameSysVal()->GetLimitsLevel();
//			kNPCData.m_bNoDrop			= true;
//			
//			if( kNPCData.m_nStartPos != -1 )
//			{
//				kNPCList.m_NPCList.push_back( kNPCData );
//			}
//			else
//			{
//				// 출현 위치가 -1이면 이미 출현 결정된 NPC의 위치를 주자!
//				BOOST_TEST_FOREACH( const KNPCUnitReq&, kNpc, kNPCList.m_NPCList )
//				{
//					kNPCData.m_nStartPos = kNpc.m_nStartPos;
//					kNPCList.m_NPCList.push_back( kNPCData );
//					break;
//				}
//			}
//		}
//#endif SERV_AUTO_HACK_CHECK_NPC
//		//}}
//		//////////////////////////////////////////////////////////////////////////
//
//		//{{ 2009. 12. 29  최육사	NPCKILLALL해킹방지
//		SUB_STAGE_NPC_INFO sSubStageNpcInfo;
//		//}}
//		int iExpNpcNum = 0;
//
//		//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
//		int iSubStageActiveNpc = 0;
//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
//		//}}
//		
//		for( int i = 0; i < (int)kNPCList.m_NPCList.size(); ++i )
//		{
//			// 1. NPC UID 발급
//			const int nNPCUID = m_nNPCUID++;
//
//			// 2. NPC UID 저장
//			kNPCList.m_NPCList[i].m_UID = nNPCUID;
//
//			// 3. NPC(Monster) 관리를 위한 data 보관
//			npcData.m_bDie		= false;
//			npcData.m_cLevel	= kNPCList.m_NPCList[i].m_Level;
//			npcData.m_iNPCID	= kNPCList.m_NPCList[i].m_NPCID;
//			npcData.m_bActive	= kNPCList.m_NPCList[i].m_bActive;
//			npcData.m_bNoDrop	= kNPCList.m_NPCList[i].m_bNoDrop;
//			npcData.m_bIsBoss	= ( kNPCList.m_NPCList[i].m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC );	// 몬스터 등급으로 보스몬스터인지 체크.
//			//npcData.m_bIsBoss	= kNPCList.m_NPCList[i].m_bHasBossGage;	//보스몬스터인지 체크.
//			//{{ 2009. 12. 18  최육사	던전경험치개편
//			npcData.m_cMonsterGrade = kNPCList.m_NPCList[i].m_cMonsterGrade; //일반몹인지 중간보스인지 보스인지 체크.
//			//}}
//
//			//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
//			if( kNPCList.m_NPCList[i].m_NPCID == CXSLUnitManager::NUI_DEFENCE_CRYSTAL )
//			{
//				if( m_iDefenseMainCrystalNpcUID != 0 )
//				{
//					START_LOG( cerr, L"큰 수정이 1 보다 많다? 절대로 일어나서는 안되는 에러!" )
//						<< BUILD_LOG( m_iDefenseMainCrystalNpcUID )
//						<< BUILD_LOG( kNPCList.m_NPCList[i].m_NPCID )
//						<< END_LOG;
//				}
//				else
//				{
//					m_iDefenseMainCrystalNpcUID = nNPCUID;
//				}
//			}
//			if( kNPCList.m_NPCList[i].m_NPCID == CXSLUnitManager::NUI_DEFENCE_CRYSTAL_SMALL )
//			{
//				if( m_setDefenseSmallCrystalNpcUID.size() > MAX_SMALL_CRYSTAL )
//				{
//					START_LOG( cerr, L"작 수정이 4 보다 많다? 절대로 일어나서는 안되는 에러!" )
//						<< BUILD_LOG( m_setDefenseSmallCrystalNpcUID.size() )
//						<< END_LOG;
//				}
//				else
//				{
//					m_setDefenseSmallCrystalNpcUID.insert( nNPCUID );
//				}
//			}
//#endif SERV_INSERT_GLOBAL_SERVER
//			//}} 2011. 04. 13  김민성  글로벌 서버 추가
//
//			m_mapNPCData.insert( std::make_pair( nNPCUID, npcData ) );
//			
//			// 4. NPCKILLALL해킹방지
//			if( npcData.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC )
//			{
//				sSubStageNpcInfo.m_setBossNpcUID.insert( nNPCUID );
//			}
//			else if( npcData.m_cMonsterGrade == CXSLUnitManager::MG_MIDDLE_BOSS_NPC )
//			{
//				sSubStageNpcInfo.m_setMiddleBossNpcUID.insert( nNPCUID );
//			}
//			else if( npcData.m_bActive )
//			{
//				sSubStageNpcInfo.m_setNormalNpcUID.insert( nNPCUID );
//			}
//
//			//////////////////////////////////////////////////////////////////////////			
//			//{{ 2009. 5. 13  최육사	서브스테이지별 경험치 주는 npc수
//			if( m_eDungeonType == CXSLDungeon::DT_HENIR )
//			{
//				if( SiKDropTable()->IsHenirExpDropNpc( static_cast<CXSLDungeon::DUNGEON_MODE>(m_cDungeonMode), npcData.m_iNPCID ) == true  /*&&  !npcData.m_bNoDrop*/ )
//				{
//					iExpNpcNum++;
//				}
//				//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
//				if( npcData.m_bActive == true )
//				{
//					iSubStageActiveNpc++;
//				}
//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
//				//}}
//			}
//			else
//			{
//				if( SiKDropTable()->IsExpDropNpc( m_iDungeonID + static_cast<int>(m_cDifficultyLevel), npcData.m_iNPCID ) == true /*&&  !npcData.m_bNoDrop*/ )
//				{
//					iExpNpcNum++;
//				}
//				//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
//				if( npcData.m_bActive == true )
//				{
//					iSubStageActiveNpc++;
//				}
//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
//				//}}
//			}
//			//}}
//			//////////////////////////////////////////////////////////////////////////
//		}
//
//		//{{ 2009. 12. 29  최육사	NPCKILLALL해킹방지
//		KSubStageKey kSubStageKey( kNot.m_iStageID, iSubStageID );
//		m_mapSubStageNpcInfo.insert( std::make_pair( kSubStageKey, sSubStageNpcInfo ) );
//		//}}
//
//		// drop npc수
//		m_mapSubStageExpNpcNum.insert( std::make_pair( iSubStageID, iExpNpcNum ) );
//
//		//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
//		m_mapSubStageActiveNpcNum.insert( std::make_pair( iSubStageID, iExpNpcNum ) );
//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
//		//}}
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// 속성NPC 세팅 - NPC UID발급과는 관계 없습니다.
//	if( eStageType == CXSLDungeonStage::ST_EXTRA )
//	{
//		// 첫번째 Extra Stage진입인가?
//		if( m_iExtraStageLoadCount == 1 )
//		{
//			// 알테라 비던의 경우에만 플레이 시간 초기화!
//			if( CXSLDungeon::IsAlteraDungeon( GetDungeonIDAndDif() ) == true )
//			{
//				// 남은 시간 초기화
//				SetRemainingPlayTime( GetPlayTimeLimit() );
//				BroadCast( ERM_REMAINING_PLAY_TIME_NOT, GetRemainingPlayTime() );
//
//				// EXTRA STAGE 전용 독대미지 타임을 얻자!
//				m_fPoisonTimeLimit = SiCXSLDungeonManager()->GetExtraStagePoisonTimeLimit( GetDungeonIDAndDif() );
//
//				KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT kNot;
//				kNot.m_fRemainingTime = m_fPoisonTimeLimit;
//				BroadCast( ERM_DUNGEON_EFFECT_REMAINING_TIME_NOT, kNot );
//			}
//		}
//
//		SecretDungeonExtraStageAttribMonsterDrop( kNot );
//	}
//	else
//	{
//		NormalDungeonAttribMonsterDrop( kNot );
//	}
//	
//	//////////////////////////////////////////////////////////////////////////	
////#endif SERV_SECRET_HELL
//	//}}
//	//////////////////////////////////////////////////////////////////////////
//	
//	BroadCast( ERM_DUNGEON_STAGE_LOAD_NOT, kNot );
//}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
#ifdef SERV_ACTIVE_EVENT_MONSTER
bool KDungeonRoom::EventDropMonster( IN int iNpcID, IN bool bAllyNpc, IN int iStageID, IN bool bActiveNpc, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot )
#else
bool KDungeonRoom::EventDropMonster( IN int iNpcID, IN bool bAllyNpc, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot )
#endif SERV_ACTIVE_EVENT_MONSTER
#else
bool KDungeonRoom::EventDropMonster( IN int iNpcID, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot )
#endif SERV_ALLY_EVENT_MONSTER
//}}
{
	// 등장하는 몬스터가 한마리도 없으면 이벤트 몬스터도 출현시키지 않는다.
	if( kNot.m_mapNPCData.size() <= 0 )
	{
		START_LOG( cerr, L"이벤트 몬스터 출현 결정중 몬스터 정보가 없다?" )
			<< BUILD_LOG( iNpcID )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOG( kNot.m_mapNPCData.size() )
			<< END_LOG;
		return false;
	}

	// 몇번째 SubStage에 등장시킬지 랜덤하게 결정!
	int iRandomSubStage = rand() % static_cast<int>(kNot.m_mapNPCData.size());

	std::map< int, KNPCList >::const_iterator mit;
	int iSubStageCount = 0;
	for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
	{
		if( iRandomSubStage == iSubStageCount )
		{
			iRandomSubStage = mit->first;
			break;
		}
		++iSubStageCount;
	}

	// 랜덤으로 이벤트 몬스터 드롭
	bool bEventMonDrop = false;
	if( ( rand() % 2 ) == 0 )
	{
		bEventMonDrop = true;
	}
	// 랜덤으로 드롭하지 못했을 경우 보스 스테이지에서 무조건 드롭한다.
	else
	{
		std::map< int, KNPCList >::const_iterator mit;
		for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
		{
			const KNPCList& kNPCList = mit->second;
			for( u_int ui = 0; ui < kNPCList.m_NPCList.size(); ++ui )
			{
				if( kNPCList.m_NPCList[ui].m_bHasBossGage == true )
				{
					bEventMonDrop = true;
					break;
				}
			}
		}
	}
	
	if( bEventMonDrop )
	{
		KNPCUnitReq kNPCData;
		kNPCData.Init();

		//NPC DATA SET..
		kNPCData.m_NPCID			= iNpcID;
		kNPCData.m_KeyCode			= -1;
		kNPCData.m_bFocusCamera		= false;
		kNPCData.m_nStartPos		= SiCXSLDungeonManager()->GetStartPosByRandom( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), iStageID, iRandomSubStage );
#ifdef SERV_ACTIVE_EVENT_MONSTER
		kNPCData.m_bActive			= bActiveNpc;
#else
		kNPCData.m_bActive			= true;
#endif SERV_ACTIVE_EVENT_MONSTER
		kNPCData.m_bShowGage		= true;
		kNPCData.m_AddPos.x			= 0.0f;
		kNPCData.m_AddPos.y			= 0.0f;
		kNPCData.m_AddPos.z			= 0.0f;
		//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
		kNPCData.m_Level			= SiCXSLDungeonManager()->GetDungeonNpcLevel( m_iDungeonID );
#else
		kNPCData.m_Level			= SiCXSLDungeonManager()->GetDungeonMinLevel( m_iDungeonID );
#endif SERV_DUNGEON_NPC_LEVEL
		//}}
		kNPCData.m_bHasBossGage		= false;
		kNPCData.m_bShowBossName	= false;
		kNPCData.m_bShowSubBossName	= false;
		kNPCData.m_bSiegeMode		= false;
		kNPCData.m_bIsRight			= (rand()%2) ? true : false;
		kNPCData.m_bNoDrop			= false;
		kNPCData.m_fUnitScale		= -1.0f;

		//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
		kNPCData.m_cTeamNum = ( bAllyNpc == true ) ? CXSLRoom::TN_RED : CXSLRoom::TN_MONSTER;
		kNPCData.m_cAIType = ( bAllyNpc == true ) ? CXSLUnit::NAT_ALLY : CXSLUnit::NAT_NORMAL;
		kNPCData.m_bAllyNpc = bAllyNpc;
#endif SERV_ALLY_EVENT_MONSTER
		//}}

		//{{ 2008. 9. 12  최육사	npc없는 스테이지면?
		if( kNPCData.m_nStartPos == -1 )
		{
			bEventMonDrop = false;
		}
		else
		{
			kNot.m_mapNPCData[iRandomSubStage].m_NPCList.push_back( kNPCData );
		}
		//}}
	}

	return bEventMonDrop;
}

//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KDungeonRoom::SecretDungeonExtraStageMonster( IN const int iStageID, IN const short sPartyFever, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot )
#else
void KDungeonRoom::SecretDungeonExtraStageMonster( IN int iStageID, IN char cPartyFever, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot )
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
{
	// 이미 등장한적이 있다면?
	if( m_bAdventExtraNpc == true )
		return;

	if( kNot.m_mapNPCData.size() <= 0 )
	{
		START_LOG( cerr, L"이벤트 몬스터 출현 결정중 몬스터 정보가 없다?" )
			<< BUILD_LOG( iStageID )
			<< BUILD_LOG( kNot.m_mapNPCData.size() )
			<< END_LOG;
		return;
	}

	// 등장할지 안할지 결정
	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	const char cLevel = static_cast<char>(GetAverageUserLevelAtDungeonStart());
#else
	const char cLevel = static_cast<char>(m_iHighestUserLevelAtDungeonStart);
#endif SERV_DUNGEON_NPC_LEVEL
	//}}	
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	float fExtraNpcRate = static_cast<float>(sPartyFever) * SiCXSLDungeonManager()->GetExtraStageFactor() * SiCXSLDungeonManager()->GetExtraStageConditionFactor( cLevel );
#else
	float fExtraNpcRate = static_cast<float>(cPartyFever) * 2.0f * SiCXSLDungeonManager()->GetExtraStageConditionFactor( cLevel );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
	if( fExtraNpcRate > 100.f )
		fExtraNpcRate = 100.f;

	KLottery kLot;
	kLot.AddCase( KLottery::CASE_BLANK + 1, fExtraNpcRate );

	if( kLot.Decision() == KLottery::CASE_BLANK )
	{
		return;
	}
	else
	{
		// extra npc 등장 처리!
		m_bAdventExtraNpc = true;
	}

	// 등장 위치 결정
	int iRandomSubStage = rand() % static_cast<int>(kNot.m_mapNPCData.size());

	std::map<int, KNPCList>::iterator mit;
	int iCount = 0;
	for( mit = kNot.m_mapNPCData.begin(); mit != kNot.m_mapNPCData.end(); ++mit )
	{
		if( iRandomSubStage == iCount )
		{
			iRandomSubStage = mit->first;
			break;
		}
		++iCount;
	}

	// Extra스테이지 몬스터 드롭
	{
		KNPCUnitReq kNPCData;
		kNPCData.Init();

		//NPC DATA SET..
		kNPCData.m_NPCID			= CXSLUnitManager::NUI_LUTO;
		kNPCData.m_KeyCode			= -1;
		kNPCData.m_bFocusCamera		= true;
		kNPCData.m_bActive			= false;
		kNPCData.m_bShowGage		= true;
		kNPCData.m_AddPos.x			= 0.0f;
		kNPCData.m_AddPos.y			= 0.0f;
		kNPCData.m_AddPos.z			= 0.0f;
		//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
		kNPCData.m_Level			= GetAverageUserLevelAtDungeonStart() + 5 + SiCXSLDungeonManager()->GetSecretDungeonExtraStageNpcLevel();
#else
		kNPCData.m_Level			= SiCXSLDungeonManager()->GetDungeonMinLevel( m_iDungeonID );
#endif SERV_DUNGEON_NPC_LEVEL
		//}}		
		kNPCData.m_bHasBossGage		= false;
		kNPCData.m_bShowBossName	= false;
		kNPCData.m_bShowSubBossName	= false;
		kNPCData.m_bSiegeMode		= false;
		kNPCData.m_bNoDrop			= true;
		kNPCData.m_fUnitScale		= -1.0f;

		// 던전 스크립트에서 EXTRA NPC정보 얻기
		SiCXSLDungeonManager()->GetExtraNpcInfo( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), iStageID, iRandomSubStage, kNPCData.m_nStartPos, kNPCData.m_bIsRight );

		//{{ 2008. 9. 12  최육사	npc없는 스테이지면?
		if( kNPCData.m_nStartPos == -1 )
		{
			START_LOG( cwarn, L"Npc가 없는 스테이지다?" )
				<< BUILD_LOG( iStageID )
				<< BUILD_LOG( iRandomSubStage )
				<< END_LOG;
		}
		else
		{
			kNot.m_mapNPCData[iRandomSubStage].m_NPCList.push_back( kNPCData );
		}
		//}}
	}
}

_IMPL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ, KEGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_STAGE_LOAD_COMPLETE_ACK, KPacketOK );

	m_usLoadingState = ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ;

    // loading 상태를 변화시킨다.
    if( !m_spRoomUserManager->SetStageLoaded( FIRST_SENDER_UID, true ) )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_13;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_COMPLETE_ACK, kPacket );
        return;
    }

    // 로딩 상태가 변경된 슬롯 ID를 알아낸다.    
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
    if( spRoomUser == NULL )
    {
        kPacket.m_iOK = NetError::ERR_ROOM_02;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_COMPLETE_ACK, kPacket );
        return;
    }
	const int iSlotID = spRoomUser->GetSlotID();

	//{{ 2009. 7. 21  최육사	통과한 스테이지 카운트
	if( m_eDungeonType == CXSLDungeon::DT_NORMAL  ||  kPacket_.m_bIsSecretStage == false )
	{
		spRoomUser->IncreasePassedStageCount();
	}
	//}}

	//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
	// 첫번째 스테이지가 아닌 경우에만 남기자!
	if( m_iLoadingCompleteStageID != -1 )
	{
		KELOG_DUNGEON_STAGE_LOG_NOT kPacketToLog;
		kPacketToLog.m_iDungeonID		  = GetDungeonID();
		kPacketToLog.m_iStageID			  = m_iLoadingCompleteStageID; // 이전 스테이지
		kPacketToLog.m_iUnitUID			  = spRoomUser->GetCID();
		kPacketToLog.m_cLevel			  = spRoomUser->GetLevel();
		kPacketToLog.m_cDifficulty		  = GetDifficultyLevel();
		kPacketToLog.m_bIsChallenge 	  = ( m_cDungeonMode == CXSLDungeon::DM_HENIR_PRACTICE );
		kPacketToLog.m_cUnitClass		  = spRoomUser->GetUnitClass_LUA();
		kPacketToLog.m_iStagePlayTime	  = static_cast<int>(spRoomUser->GetStagePlayTime()); // 해당 스테이지를 플레이한 플레이 타임
		kPacketToLog.m_iStageDamage		  = ( kPacket_.m_iDamage - spRoomUser->GetStageDamage() ); // 해당 스테이지 동안 받은 대미지
		kPacketToLog.m_iResurrectionCount = spRoomUser->GetUsedRessurectionStoneCount();
		kPacketToLog.m_wstrRegDate		  = KncUtil::GetCurrentTimeString();
		SendToLogDB( ELOG_DUNGEON_STAGE_LOG_NOT, kPacketToLog );

		// 스테이지 단위 대미지 정보 기록
        spRoomUser->SetStageDamage( kPacket_.m_iDamage );
	}
#endif SERV_DUNGEON_STAGE_LOG
	//}}

    // 보낸 사람에게 ack를 날린다.
    kPacket.m_iOK = NetError::NET_OK;
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_COMPLETE_ACK, kPacket );

	// 어떤 유저가 로딩완료 되었는지 브로드캐스팅
	KEGS_DUNGEON_STAGE_LOAD_COMPLETE_NOT kPacketNot;
	kPacketNot.m_iSlotID = iSlotID;
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
    kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
	BroadCast( ERM_DUNGEON_STAGE_LOAD_COMPLETE_NOT, kPacketNot );

    // 모든 유저가 로딩 끝났으면 not를 날린다.
    if( m_spRoomUserManager->IsAllPlayerStageLoaded() )
    {
        // Loading 변수 리셋.
        m_spRoomUserManager->ResetStageLoaded();

		//모든 로딩처리를 끝내고 시작을 알려줌
		KEGS_DUNGEON_STAGE_START_NOT kPacketStartNot;
		kPacketStartNot.m_iStageID = m_iStageID;
		BroadCast( ERM_DUNGEON_STAGE_START_NOT, kPacketStartNot );

		//{{ 2009. 12. 14  최육사	헤니르임시인벤
		CheckTempInventoryClear();
		//}}

		//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
		// 로딩 완료된 스테이지ID를 기록한다!
		m_iLoadingCompleteStageID = GetStageID();
#endif SERV_DUNGEON_STAGE_LOG
		//}}
#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
        if ( m_iSubStageCount == 0 )
        {
            m_kTimer[TM_CHECK_CHANGE_HOST].restart();
            m_bCheckPingScoreExecuted = false;
        }
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

    }

	//{{ 2012. 07. 04	최육사		자동부활 버그 수정
#ifdef SERV_AUTO_RESURRECTION_BUG_FIX
#else
	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 7. 3  최육사		헤니르 시공	
	if( m_eDungeonType == CXSLDungeon::DT_HENIR )
	{
		// 특정 스테이지 마다 부활!
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		if( SiKHenirResultTable()->IsResurrectionStage( GetStageID() ) == true )
#else
		if( SiKHenirResultTable()->IsResurrectionStage( spRoomUser->GetPassedStageCount() ) == true )
#endif SERV_NEW_HENIR_TEST
		//}}
		{
			// 죽은 유저인지 확인!
			if( spRoomUser->IsDie() == true )
			{
				// 되살리자!
				spRoomUser->SetDie( false );

				KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT kPacketNot;
				kPacketNot.m_iUnitUID = spRoomUser->GetCID();
				kPacketNot.m_iUsedByUnitUID = spRoomUser->GetCID();
				kPacketNot.m_iStartPosIndex = SiCXSLDungeonManager()->GetStartPosByRandom( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), m_iStageID, m_iSubStageID );
				BroadCast( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacketNot );

				//{{ 2008. 6. 17  최육사  실시간 부활석
				m_spRoomUserManager->SetEndPlay( spRoomUser->GetCID(), false );
				//}}
			}
		}
	}
	//}}
	//////////////////////////////////////////////////////////////////////////
#endif SERV_AUTO_RESURRECTION_BUG_FIX
	//}}
}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_CLEAR_REQ, KEGS_DUNGEON_SUB_STAGE_CLEAR_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_SUB_STAGE_CLEAR_ACK, KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	if( SiCXSLDungeonManager()->GetNextStage( m_kNextStageData, GetDungeonIDAndDif(), GetStageID(), GetSubStageID(), kPacket_.m_iClearConditionIndex ) == false )
	{
		// 다음 스테이지를 찾는데 실패. 해킹이다 by 박진웅
		START_LOG( cerr, L"다음 스테이지를 찾는데 실패. 해킹이다." )
			<< BUILD_LOG( GetDungeonIDAndDif() )
			<< BUILD_LOG( GetStageID() )
			<< BUILD_LOG( GetSubStageID() )
			<< BUILD_LOG( kPacket_.m_iClearConditionIndex )
			<< END_LOG;

		m_kNextStageData.m_eClearType = CXSLDungeonSubStage::CT_GAME;
		m_kNextStageData.m_iStageIndex = -1;
		m_kNextStageData.m_iSubStageIndex = -1;
		m_kNextStageData.m_iRate = 100;
		m_kNextStageData.m_iDungeonEndingSpeechTableIndex = 0;

		kPacket.m_iOK = NetError::ERR_ROOM_51;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SUB_STAGE_CLEAR_ACK, kPacket );
		return;
	}
	// 비던 때 재사용을 위해 보관함
	m_iClearConditionIndex = kPacket_.m_iClearConditionIndex;

	kPacket.m_iClearType = static_cast<int>( m_kNextStageData.m_eClearType );
	kPacket.m_iStageIndex = m_kNextStageData.m_iStageIndex;
	kPacket.m_iSubStageIndex = m_kNextStageData.m_iSubStageIndex;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SUB_STAGE_CLEAR_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_SECRET_STAGE_LOAD_REQ, KEGS_SECRET_STAGE_LOAD_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_STAGE_LOAD_ACK, KEGS_DUNGEON_STAGE_LOAD_ACK );

	if( m_kNextStageData.m_eClearType != CXSLDungeonSubStage::CT_STAGE_SECRET )
	{
		// 서버가 골라준 클리어 타입이 아님. 클라이언트 해킹이다. by 박진웅
		START_LOG( cerr, L"클리어타입은 아래와 같은데 이 패킷이 왜 왔지? 해커!" )
			<< BUILD_LOG( m_kNextStageData.m_eClearType )
			<< BUILD_LOG( m_iClearConditionIndex )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_51;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
		return;
	}

	if( SiCXSLDungeonManager()->GetNextStage( m_kNextStageData, GetDungeonIDAndDif(), GetStageID(), GetSubStageID(), m_iClearConditionIndex, kPacket_.m_iPadID ) == false )
	{
		// 다음 스테이지를 찾는데 실패. 해킹이다 by 박진웅
		START_LOG( cerr, L"다음 스테이지를 찾는데 실패. 해킹이다." )
			<< BUILD_LOG( GetDungeonIDAndDif() )
			<< BUILD_LOG( GetStageID() )
			<< BUILD_LOG( GetSubStageID() )
			<< BUILD_LOG( m_iClearConditionIndex )
			<< BUILD_LOG( kPacket_.m_iPadID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_51;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_STAGE_LOAD_ACK, kPacket );
		return;
	}

	KEGS_DUNGEON_STAGE_LOAD_REQ kPacketReq;
	kPacketReq.m_iStageID = m_kNextStageData.m_iStageIndex;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SECRET_STAGE_LOAD_ACK, kPacketReq );
}
#endif // SERV_STAGE_CLEAR_IN_SERVER

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_OPEN_REQ, KEGS_DUNGEON_SUB_STAGE_OPEN_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_SUB_STAGE_OPEN_ACK, KEGS_DUNGEON_SUB_STAGE_OPEN_ACK );

	kPacket.m_iOK = NetError::NET_OK;

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	if( m_spRoomUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == false )
	{
		if( m_bFirstSubStage == false )
		{
			if( m_kNextStageData.m_eClearType != CXSLDungeonSubStage::CT_SUB_STAGE )
			{
				// 서버가 골라준 클리어 타입이 아님. 클라이언트 해킹이다. by 박진웅
				START_LOG( cerr, L"클리어타입은 아래와 같은데 이 패킷이 왜 왔지? 해커!" )
					<< BUILD_LOG( m_bFirstStage )
					<< BUILD_LOG( m_kNextStageData.m_eClearType )
					<< BUILD_LOG( m_kNextStageData.m_iStageIndex )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_ROOM_51;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SUB_STAGE_OPEN_ACK, kPacket );
				return;
			}

			// 타입 자체는 같고 서브 스테이지만 다르다면 셋팅만 제대로 해서 정상 동작하게 하자. by 박진웅
			kPacket_.m_iSubStageID = m_kNextStageData.m_iSubStageIndex;
		}
		else
		{
			// 서브스테이지 체크를 안 해도 되는건 스테이지 오픈 후 처음 한번 뿐이다. by 박진웅
			kPacket_.m_iSubStageID = 0;
			m_bFirstSubStage = false;
		}
	}

	// 다음에 다시 막기 위해 값 초기화.
	m_kNextStageData.ClearData();
#endif // SERV_STAGE_CLEAR_IN_SERVER
	
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SUB_STAGE_OPEN_ACK, kPacket );

	// 모든 유저에게 서브스테이지 오픈준비를 하라고 알린다. 
	KEGS_DUNGEON_SUB_STAGE_OPEN_NOT kNot;
	kNot.m_iSubStageID = kPacket_.m_iSubStageID;
	SetSubStageID( kPacket_.m_iSubStageID );
	BroadCast( ERM_DUNGEON_SUB_STAGE_OPEN_NOT, kNot );

	//{{ 2012. 07. 04	최육사		자동 부활 버그 수정
#ifdef SERV_AUTO_RESURRECTION_BUG_FIX
	if( m_eDungeonType == CXSLDungeon::DT_HENIR )
	{
		for( int idx = 0; idx < m_kRoomUserManager.GetNumMember(); ++idx )
		{
			KRoomUserPtr spRoomUser  = m_kRoomUserManager.GetUser( idx );
			if( IS_NULL( spRoomUser ) )
			{
				START_LOG( cerr, L"룸 유저의 포인터가 이상함." )
					<< END_LOG;
				continue;
			}

			HenirAutoResurrection( spRoomUser->GetCID() );
		}
	}
#endif SERV_AUTO_RESURRECTION_BUG_FIX
	//}}
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ, KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_SUB_STAGE_GO_NEXT_ACK, KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );

	m_usLoadingState = ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ;

	if( m_spRoomUserManager->SetStageID( FIRST_SENDER_UID, kPacket_.m_iNextStageID ) == false )
	{
		START_LOG( cerr, L"스테이지 이동 데이터 저장 실패.!" )
			<< BUILD_LOGc( kPacket_.m_iNextStageID )
			<< BUILD_LOGc( kPacket_.m_iBeforeStageID )
			<< END_LOG;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SUB_STAGE_GO_NEXT_ACK, kPacket );

	//게임조건에 따라 한명이상의 User Unit이 죽을경우 남은 게임을 진행하기위해 담아둔다.
	m_kDSSGoNextAllNot.m_iBeforeStageID	= kPacket_.m_iBeforeStageID;
	m_kDSSGoNextAllNot.m_iNextStageID	= kPacket_.m_iNextStageID;

	SendDSSGoNextAllNot( m_kDSSGoNextAllNot );
}

_IMPL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ, KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );

	m_usLoadingState = ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ;

	// loading 상태를 변화시킨다.
	if( !m_spRoomUserManager->SetStageLoaded( FIRST_SENDER_UID, true ) )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_13;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, kPacket );
		return;
	}

	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	if( m_spRoomUserManager->SetSubStageID( FIRST_SENDER_UID, kPacket_.m_iSubStageID ) == false )
	{
		START_LOG( cerr, L"SubStageID값 저장할 유저가 없다! 위에서 검사했는데?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iSubStageID )
			<< END_LOG;
	}
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

	// 보낸 사람에게 ack를 날린다.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK, kPacket );

	// 모든 유저가 로딩 끝났으면 not를 날린다.
	if( m_spRoomUserManager->IsAllPlayerStageLoaded() == true )
	{
		//{{ 2010. 8. 16	최육사	서버 코드 정리
		if( DungeonSubStageStartProcess( kPacket_.m_iSubStageID ) == false )
		{
			START_LOG( cerr, L"던전 서브 스테이지 시작 에러!" )
				<< BUILD_LOG( kPacket_.m_iSubStageID )
				<< END_LOG;
		}
		//}}
	}

	//{{ 2008. 9. 17  최육사	서브 스테이지 로딩 카운트
	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( spRoomUser != NULL )
	{
		spRoomUser->IncreasePassedSubStageCount();

		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		// 서브 스테이지 npc 정보
		KERM_UPDATE_QUEST_INFO_NOT kNot;
		m_kDungeonMonsterManager.GetSubStageNpcList( GetStageID(), kPacket_.m_iSubStageID, kNot.m_vecNPC );

		if(kNot.m_vecNPC.empty() == false )
		{
			
			kNot.m_isDungeon = true;

			if( spRoomUser->CheckFindNPCQuest( GetDungeonIDAndDif(), kNot.m_vecNPC, true ) == true )
			{
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_UPDATE_QUEST_INFO_NOT, kNot );
			}
		}
#endif SERV_REFORM_QUEST
		//}}
	}
	//}}
}

//{{ 2009. 7. 20  최육사	비밀스테이지
IMPL_ON_FUNC_NOPARAM( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ )
{
	KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK kPacket;

	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"상태 오류. Name : " << m_strName )
			<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
			<< L"    허용 상태 : KRoomFSM::S_PLAY" << END_LOG;

		kPacket.m_bCanEnterSecretStage = false;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket );
		return;
	}

	// 헤니르 시공이 맞나?
	if( m_eDungeonType != CXSLDungeon::DT_HENIR )
	{
		START_LOG( cerr, L"헤니르 시공이 아닌데 이 이벤트가 왜왔지? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_eDungeonType )
			<< END_LOG;
	}

	// 모든 유저가 살아있는가?
	if( m_spRoomUserManager->IsAllPlayerAlive() == false )
	{
		kPacket.m_bCanEnterSecretStage = false;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket );
		return;
	}

	// 유저 의견 초기화
	m_spRoomUserManager->ResetAgreeEnterSecretStage();

	// 비밀 스테이지 의견 타이머 작동!
	//m_kEnterSecretStageTimer.CreateTimer();

	// 비밀 스테이지 입장 조건 만족함!
	kPacket.m_bCanEnterSecretStage = true;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket );

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT kPacketNot;
	kPacketNot.m_iStartSecretStageEnteringEvent = SiCXSLDungeonManager()->GetSecretStageEnteringEvent( GetDungeonIDAndDif(), GetStageID(), GetSubStageID(), m_iClearConditionIndex );

	BroadCast( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT, kPacketNot );
#else // SERV_STAGE_CLEAR_IN_SERVER
	// 모든 유저에게 비밀 스테이지 입장 여부 확인하기
	BroadCastID( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT );
#endif // SERV_STAGE_CLEAR_IN_SERVER
}

_IMPL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_PLAY ) );

	// 헤니르 시공이 맞나?
	if( m_eDungeonType != CXSLDungeon::DT_HENIR )
	{
		START_LOG( cerr, L"헤니르 시공이 아닌데 이 이벤트가 왜왔지? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_eDungeonType )
			<< END_LOG;
	}

	// 의사 표시 타이머가 살아있는가?
	//if( m_kEnterSecretStageTimer.IsExist() == false )
	//{	
	//	return;
	//}

    // 동의 처리
	if( kPacket_.m_bWantToEnterSecretStage )
	{
		m_spRoomUserManager->AgreeEnterSecretStage( FIRST_SENDER_UID, CXSLRoom::ESS_AGREE );
	}
	else
	{
		m_spRoomUserManager->AgreeEnterSecretStage( FIRST_SENDER_UID, CXSLRoom::ESS_NOT_AGREE );
	}
	
	// 모든 유저가 의사 표시를 하였는가?
	CheckEnterSecretStageSuccess();
}
//}}

//{{ 2011. 10. 25	최육사	배틀필드 시스템
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

_IMPL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_NPC_UNIT_CREATE_ACK, KPacketOK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_CREATE_ACK, kPacket );

	KEGS_NPC_UNIT_CREATE_NOT kPacketNot;	

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	KEGS_CREATE_ATTRIB_NPC_NOT kAttribNPCNot;
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가


	BOOST_TEST_FOREACH( KNPCUnitReq&, kCreateNpcInfo, kPacket_.m_vecNPCUnitReq )
	{
		KNPCUnitNot kNPCInfo;

		switch( kCreateNpcInfo.m_NPCID )
		{
		case CXSLUnitManager::NUI_EVENT_TEACHER_ECHO:
			{
				kCreateNpcInfo.m_bNoDrop = false; // ED 대량 드롭 
				
				if( m_eDungeonType == CXSLDungeon::DT_NORMAL )
				{
					if( CXSLDungeon::IsEventDungeon( GetDungeonID() ) == true )
					{
						kCreateNpcInfo.m_Level = 1;
					}
					else
					{
						kCreateNpcInfo.m_Level = SiCXSLDungeonManager()->GetDungeonNpcLevel( GetDungeonIDAndDif() );
					}
				}
				else
				{
					kCreateNpcInfo.m_Level = GetAverageUserLevelAtDungeonStart();
				}
			}break;
		default:
			{
				//////////////////////////////////////////////////////////////////////////				
				// 클라이언트에서 NoDrop = false를 해서 드롭을 시켜야하는 몬스터가 존재한다. 따라서 이 코드는 다시 주석처리함. -- 육사
				//////////////////////////////////////////////////////////////////////////

				//kCreateNpcInfo.m_bNoDrop = true; // 클라이언트가 생성 요청 보내는 몬스터는 보안상 반드시 NoDrop으로 처리한다!
			}break;	
		}

		kNPCInfo.m_kNPCUnitReq = kCreateNpcInfo;

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
		// 일단은 무조건 노드랍 처리 // 일단 저장해둠
		bool bTempNoDrop = kCreateNpcInfo.m_bNoDrop;
		kCreateNpcInfo.m_bNoDrop = true;

		int iNpcDropTime = SiCXSLDungeonManager()->GetNPCDropTime( GetDungeonIDAndDif(), GetStageID(), GetSubStageID(), kCreateNpcInfo.m_NPCID );
		if( iNpcDropTime > 0 )
		{
			NPC_INFO_QUADRUPLE npcInfoQuadruple;
			npcInfoQuadruple.iDungeonID		= GetDungeonIDAndDif();
			npcInfoQuadruple.iStageID		= GetStageID();
			npcInfoQuadruple.iSubStageID	= GetSubStageID();
			npcInfoQuadruple.iNpcID			= kCreateNpcInfo.m_NPCID;

			// 소환요청되었던 수가 전체 드랍가능한 수 미만일 때만 드랍 가능
			if( m_mapDroppedTimes[npcInfoQuadruple] < iNpcDropTime )
			{
				m_mapDroppedTimes[npcInfoQuadruple] = m_mapDroppedTimes[npcInfoQuadruple] + 1;
				kCreateNpcInfo.m_bNoDrop = bTempNoDrop;
			}
		}
#endif SERV_CREATED_NPC_LIMITED_DROPS

		// 몬스터 생성!
		LIF( m_kDungeonMonsterManager.CreateMonster( kCreateNpcInfo, kNPCInfo.m_kNPCUnitReq.m_UID ) );
		const int iNpcUID = kNPCInfo.m_kNPCUnitReq.m_UID;

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
		if( kCreateNpcInfo.m_iAllyUID > 0 )
			m_spRoomUserManager->AddAllyNPC( kCreateNpcInfo.m_iAllyUID, kNPCInfo.m_kNPCUnitReq );
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		if( kCreateNpcInfo.m_bAttributeNpc == true &&  CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) == true ) // 디펜스 던전, 엘리트 몬스터
		{
			KAttribNpcUnitInfo kAttribNpcInfo;
			kAttribNpcInfo.m_sNPCUnitReq = kNPCInfo.m_kNPCUnitReq;
			kAttribNpcInfo.m_sNPCUnitReq.m_UID = kNPCInfo.m_kNPCUnitReq.m_UID;

			if( SiKAttribNpcTable()->GetAttribNpcInfo( m_eDungeonType, GetDifficultyLevel(), kCreateNpcInfo.m_Level, kCreateNpcInfo.m_NPCID, kAttribNpcInfo.m_sAttribEnchantNpcInfo ) == false )
			{
				START_LOG( cerr, L"속성 NPC정보 얻기 실패!" )
					<< BUILD_LOG( GetDungeonID() )
					<< BUILD_LOG( m_eDungeonType )
					<< BUILD_LOGc( GetDifficultyLevel() )
					<< BUILD_LOGc( kCreateNpcInfo.m_Level )
					<< BUILD_LOG( kCreateNpcInfo.m_NPCID )
					<< END_LOG;
				continue;
			}

			kAttribNPCNot.m_vecAttribNPCUnit.push_back( kAttribNpcInfo );
			continue;
		}
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가

		//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		if( CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) == true )
		{
			const char cMonsterTypeFactor = static_cast<char>( CXSLUnitManager::GetMonsterTypeFactor( kCreateNpcInfo.m_bAttributeNpc, GetDifficultyLevel(), kCreateNpcInfo.m_cMonsterGrade ) );
			m_kBadAttitudeManager.IncreaseSubStageMonsterDieCount( GetStageID(), GetSubStageID(), cMonsterTypeFactor, true );
		}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
		//}
		kPacketNot.m_vecNPCUnitAck.push_back( kNPCInfo );

		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		// 어둠의 문 던전인 경우
		if( CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) == true )
		{
			// 죽은 몬스터가 수호신 몬스터라면 페이즈를 하나 올린다!
			if( m_kDefenceDungeonManager.IncreaseDefenceDungeonPhase( kCreateNpcInfo.m_NPCID ) == true )
			{
				// 페이즈 2단계가 될때부터 퀵슬롯 아이템을 갱신 하자!
				switch( m_kDefenceDungeonManager.GetDefenceDungeonPhase() )
				{
				case CXSLDefenceDungeonManager::DDP_PHASE_2:
				case CXSLDefenceDungeonManager::DDP_PHASE_3:
					{
						KERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT kPacketNot;
						kPacketNot.m_iDefenceDungeonPhase = m_kDefenceDungeonManager.GetDefenceDungeonPhase();
						BroadCast( ERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT, kPacketNot );
					}
					break;
				}
			}
		}
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
	}

	BroadCast( ERM_NPC_UNIT_CREATE_NOT, kPacketNot );

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	if( kAttribNPCNot.m_vecAttribNPCUnit.size() > 0 )
	{
		BroadCast( ERM_CREATE_ATTRIB_NPC_NOT, kAttribNPCNot );
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가
}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );

	std::set< int >::iterator sit = m_setDefenseSmallCrystalNpcUID.find( kPacket_.m_nDieNPCUID );
	if( sit != m_setDefenseSmallCrystalNpcUID.end() )
	{
		kPacket.m_iOK		= NetError::NET_OK;
		kPacket.m_iNPCID	= kPacket_.m_nDieNPCUID;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

		// std::set 에서 삭제
		m_setDefenseSmallCrystalNpcUID.erase( sit );

		START_LOG( clog, L"디펜스 던전 작은 수정이 깨졌음!!" )
			<< END_LOG;
		return;
	}

	// 몬스터가 현재 살아있는지 체크!
	if( m_kDungeonMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_21;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	// 몬스터가 아직 살아있다면 몬스터 정보 요청!
	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
	if( m_kDungeonMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_20;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
		return;
	}

	// 아직 살아있다면 죽인사람과 죽은것으로 설정..
	LIF( m_kDungeonMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );

	kPacket.m_iOK		= NetError::NET_OK;
	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
	if( kPacket_.m_cNpcDieState != KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_SELF )
		m_spRoomUserManager->DeleteAllyNPC( kPacket_.m_nDieNPCUID );
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	// 죽은 몬스터가 보스 몬스터라면 어둠의 문 입장 조건 체크
	if( kDieNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC  &&  CXSLDungeon::IsDefenceDungeon( GetDungeonID() ) == false )
	{
		// 현재 모든 보스 몬스터가 죽었는가?
		if( m_kDungeonMonsterManager.IsAllBossMonstersDead() == true )
		{
			// 어둠의 문 입장 확률 이벤트
			float fAddDefenseDungeonOpenRate = 0.0f;
			SiKGameEventManager()->GetDefenseDungeonOpenRate( fAddDefenseDungeonOpenRate );

			// 각 파티원별 캐릭터 레벨 정보가 있어야 한다.
			std::map< UidType, int > mapUnitLevelList;
			m_spRoomUserManager->GetUnitLevelList( mapUnitLevelList );

			// 어둠의 문 입장 결정!
			//{{ 2013. 04. 29	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
			bool b100PercentCheck = false;

			IF_EVENT_ENABLED( CEI_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT )
			{
				b100PercentCheck = true;
			}
			if( m_kDefenceDungeonManager.DecisionEnterDefenceDungeon( GetDungeonID(), kDieNpcInfo.m_cLevel, mapUnitLevelList, fAddDefenseDungeonOpenRate, b100PercentCheck ) == true )
#else //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
			if( m_kDefenceDungeonManager.DecisionEnterDefenceDungeon( GetDungeonID(), kDieNpcInfo.m_cLevel, mapUnitLevelList, fAddDefenseDungeonOpenRate ) == true )
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
#else
			if( m_kDefenceDungeonManager.DecisionEnterDefenceDungeon( GetDungeonID(), mapUnitLevelList, fAddDefenseDungeonOpenRate ) == true )
#endif SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
			//}}			
			{
				// 방안의 모든 유저에게 어둠의 문 입장 시작을 알리자!
				KEGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT kPacketNot;
				kPacketNot.m_DieBossPos = kPacket_.m_DiePos;
				BroadCast( ERM_DECISION_ENTER_DEFENCE_DUNGEON_NOT, kPacketNot );
			}
		}
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

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

#ifdef SERV_TIME_DROP_MONSTER_EVENT //SERV_ALL_RENEWAL_SP
	//////////////////////////////////////////////////////////////////////////	
	{
		std::map< int, int >::const_iterator mitTDM = m_mapTimeDropEventMonster.find( kDieNpcInfo.m_iNPCID );
		if( mitTDM != m_mapTimeDropEventMonster.end() )
		{
			KDBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT kPacketToLog;
			kPacketToLog.m_iEventUID = mitTDM->second;
			kPacketToLog.m_iKillUnitUID = kPacket_.m_uiAttUnit;
			kPacketToLog.m_tNpcDieDate = CTime::GetCurrentTime().GetTime();
			SendToLogDB( DBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT, kPacketToLog );

			// 이벤트 몬스터 정보를 초기화 합니다.
			m_mapTimeDropEventMonster.clear();
		}
	}
	//////////////////////////////////////////////////////////////////////////	
#endif //SERV_TIME_DROP_MONSTER_EVENT //SERV_ALL_RENEWAL_SP
	if( m_spRoomUserManager->GetNumMember() == 1 )
	{
		switch( kDieNpcInfo.m_iNPCID )
		{
		case CXSLUnitManager::NUI_DUNGEON_CHECKER1:
		case CXSLUnitManager::NUI_DUNGEON_CHECKER2:
		case CXSLUnitManager::NUI_DUNGEON_CHECKER3:
		case CXSLUnitManager::NUI_DUNGEON_CHECKER4:
		case CXSLUnitManager::NUI_DUNGEON_CHECKER5:
			{
				// 핵유저를 접속 종료시키기 위해 예약 해놓자!
				m_setFindedHackUserUnitUID.insert( FIRST_SENDER_UID );

				// 파티장에게 해킹체크패킷을 보내자!
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_FIND;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
#ifdef SERV_MINIMUM_LOG_HAKC_USER
				START_LOG( cout, L"Cant Kill Npc pos3. UID : " << FIRST_SENDER_UID );
#else //SERV_MINIMUM_LOG_HAKC_USER
				START_LOG( cout, L"Cant Kill Npc 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_nDieNPCUID )
					<< BUILD_LOGc( kPacket_.m_cAttUnitType )
					<< BUILD_LOG( kPacket_.m_uiAttUnit )
					<< BUILD_LOG( kPacket_.m_uiMDAttUnit )
					<< BUILD_LOG( kPacket_.m_DiePos.x )
					<< BUILD_LOG( kPacket_.m_DiePos.y )
					<< BUILD_LOG( kPacket_.m_DiePos.z )
					<< BUILD_LOGc( kPacket_.m_cNpcDieState );
#endif //SERV_MINIMUM_LOG_HAKC_USER
			}
			break;
		}
	}	
	//////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////
	if( SiKGameSysVal()->GetSubStageNpcDieCheck() == true )
	{
		// 솔플 인가? // 액티브 NPC 가 5마리 이상인가?
		if( m_spRoomUserManager->GetNumMember() == 1 
			&& m_iSubStageActiveNpc >= 5 
			&& kDieNpcInfo.m_bActive == true )
		{
			++m_iSubStageKillActiveNpc;

			if( m_iSubStageKillActiveNpc == 1 )
			{
				// 서브 스테이지에서 처음 액티브 몬스터를 잡은 시간
				m_tSubStageFirstKillTime = CTime::GetCurrentTime();
			}
			else
			{
				if( m_iSubStageActiveNpc <= m_iSubStageKillActiveNpc ) // 마지막 몬스터를 잡았다!
				{
					CTime tLastKillTime = CTime::GetCurrentTime();
					CTimeSpan tSpan = CTimeSpan( 0, 0, 0, 1);
					if( tLastKillTime <= m_tSubStageFirstKillTime + tSpan ) 		// 처음 몬스터를 잡은 시간에서 1초이하 밖에 걸리지 않았다
					{
						// 파티장에게 해킹체크패킷을 보내자!
						KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_NPC_DIE_TIME;
						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

						START_LOG( cout, L"Sub Stage의 모든 Npc를 죽인 시간이 1초밖에 걸리지 않는다." )
							<< BUILD_LOG( LAST_SENDER_UID )
							<< BUILD_LOG( FIRST_SENDER_UID )
							<< BUILD_LOG( m_tSubStageFirstKillTime.GetMinute() )
							<< BUILD_LOG( m_tSubStageFirstKillTime.GetSecond() )
							<< BUILD_LOG( tLastKillTime.GetMinute() )
							<< BUILD_LOG( tLastKillTime.GetSecond() )
							<< BUILD_LOG( m_iSubStageActiveNpc )
							<< BUILD_LOG( m_iSubStageKillActiveNpc )
							<< END_LOG;
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 속성 몬스터인지 여부 검사	
	int iNpcLv = 0;
	const bool bIsAttribNpc = IsAttribNpc( kPacket_.m_nDieNPCUID, iNpcLv );

	// 몬스터 타입 상수 얻기
	const char cMonsterTypeFactor = static_cast<char>( CXSLUnitManager::GetMonsterTypeFactor( bIsAttribNpc, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade ) );

	// 파티 플레이일 경우 아이템 드롭 확률 상수
	const float fPartyFactor = GetDungeonPartyBonusRate() + 1.0f;
	// 던전 상수 드롭 확률 상수
	const float fDungeonFactor = static_cast<float>(cMonsterTypeFactor);
	//////////////////////////////////////////////////////////////////////////
	// uiAttUnit값이 -1이면 유저의 공격을 받지 않고 npc가 자살한것임. [예:지뢰함정]

	// 플레이어가 몬스터를 죽인거라면 해당 플레이어의 킬몬스터수 체크
	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
	{
		if( m_spRoomUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
		{
			if( m_kDungeonMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
			{
				START_LOG( clog, L"몹끼리 죽였네!!" )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( kPacket_.m_uiAttUnit );

				// 몬스터가 함정에 걸려 죽는경우임 : 아이템은 그대로 드롭처리함.
			}
			else
			{
				// 던전 게임 시작할 당시에 유저 수와 현재 유저 수가 모두 1명일 경우만 체크!
				if( m_iStartedNumMember == 1  &&  m_spRoomUserManager->GetNumMember() == 1 )
				{
					START_LOG( cerr, L"몹끼리 죽인게 아니네? 해킹유저로 의심된다!" )
						<< BUILD_LOG( LAST_SENDER_UID )
						<< BUILD_LOG( FIRST_SENDER_UID )
						<< BUILD_LOG( kPacket_.m_nDieNPCUID )
						<< BUILD_LOGc( kPacket_.m_cAttUnitType )
						<< BUILD_LOG( kPacket_.m_uiAttUnit )
						<< BUILD_LOG( kPacket_.m_uiMDAttUnit )
						<< BUILD_LOG( kPacket_.m_DiePos.x )
						<< BUILD_LOG( kPacket_.m_DiePos.y )
						<< BUILD_LOG( kPacket_.m_DiePos.z )
						<< BUILD_LOGc( kPacket_.m_cNpcDieState )
						<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
						<< END_LOG;

					// 오토핵 체크 카운트 증가!
					++m_iNpcDieHackCount;

					// 이 에러가 5회 이상 발생한다면 그것은 잘못된 것이다!
					if( m_iNpcDieHackCount == m_iHackUserCheckCount )
					{
						// 파티장에게 해킹체크패킷을 보내자!
						KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_NPC_UNIT_DIE;
						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

						START_LOG( cout, L"NpcDie 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
							<< BUILD_LOG( LAST_SENDER_UID )
							<< BUILD_LOG( FIRST_SENDER_UID )
							<< BUILD_LOG( m_iNpcDieHackCount );
					}
				}
			}
		}
	}	

	//////////////////////////////////////////////////////////////////////////
	if( kDieNpcInfo.m_bNoDrop == false  &&  bDropDieState == true )
	{
		if( pNPCTemplet->m_bIsNPC == true  &&  kDieNpcInfo.m_bActive == true )
		{
			m_kDungeonMonsterManager.IncreaseMonsterDieCount( cMonsterTypeFactor );
			//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
			if( CXSLDungeon::IsDefenceDungeon( GetDungeonIDAndDif() ) == false )
			{
				m_kBadAttitudeManager.IncreaseSubStageMonsterDieCount( GetStageID(), GetSubStageID(), cMonsterTypeFactor, false );
			}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
			//}
		}

		// 방안의 인원에게 알려줄 데이터 셋팅 시작..
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID		 = GetDungeonID();
		kNpcDietNot.m_cDifficulty		 = GetDifficultyLevel();
		kNpcDietNot.m_nDieNPCUID		 = kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID			 = kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType		 = kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit			 = kPacket_.m_uiAttUnit;
		kNpcDietNot.m_cUserCount		 = static_cast<char>( m_spRoomUserManager->GetNumMember() );
		kNpcDietNot.m_bNoDrop			 = kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_sSubStageExpNpcNum = GetSubStageExpNpcNum( GetSubStageID() );
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_cDungeonMode		 = m_cDungeonMode;
		kNpcDietNot.m_EXPList.clear();
		kNpcDietNot.m_bQuestComplete	 = true;

		// 파티 기여도
		float fPartyContribution = 0.f;
		float fPartyTotalDamage = 0.f;

		std::map< UidType, float >::iterator mitParty = kPacket_.m_mapDamageByUser.begin();
		for( ; mitParty != kPacket_.m_mapDamageByUser.end() ; ++mitParty )
		{
			fPartyTotalDamage += mitParty->second;
		}
		
		// 살아있는 파티원 수
		int iPartyUserCount = m_spRoomUserManager->GetLiveMember();

		fPartyContribution = fPartyTotalDamage / kPacket_.m_iNpcMaxHP;

		if( fPartyContribution < 1.f )
			fPartyContribution = 1.f;

		if( fPartyContribution > 1.5f )
			fPartyContribution = 1.5f;

		 fPartyContribution /= iPartyUserCount;

		// 개인 기여도
		// 던전은 파티 기여도가 개인 기여도
		const float fContribution = fPartyContribution;

		// 유저별로 EXP, ED, DropItem 계산
		std::vector< UidType > vecLiveMember;
		m_spRoomUserManager->GetLiveMemberList( vecLiveMember );

		BOOST_TEST_FOREACH( UidType, iPartyUserUID, vecLiveMember )
		{
			KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iPartyUserUID );
			if( IS_NULL( spRoomUser ) )
			{
				START_LOG( cerr, L"유저 포인터값이 이상합니다" )
					<< BUILD_LOG( iPartyUserUID )
					<< END_LOG;
				continue;
			}

			// 레벨 factor값 구하기
			const float fLevelFactor = CXSLDungeon::GetMonsterLevelBalanceRate( spRoomUser->GetLevel(), kDieNpcInfo.m_cLevel );

			// 개별 아이템 확률로 아이템 드롭
			KDropTable::DROP_DATA sDropDataNpc;

			//////////////////////////////////////////////////////////////////////////			
			// 아이템과 기본 경험치 값 얻기
			switch( GetDungeonType() )
			{
			case CXSLDungeon::DT_HENIR:
				{
					//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
					if( SiKDropTable()->HenirNpcDropItem( static_cast<CXSLDungeon::DUNGEON_MODE>(m_cDungeonMode), 
						kDieNpcInfo.m_iNPCID,
						kDieNpcInfo.m_cLevel,
						fDungeonFactor,
						fContribution,
						fPartyFactor,
						iPartyUserCount,
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
						m_fItemDropEventProbRate,
#else // SERV_DROP_EVENT_RENEWAL
						m_iItemDropEventProbCount,
#endif // SERV_DROP_EVENT_RENEWAL
						kNpcDietNot.m_bDecreaseEndurance,
						sDropDataNpc,
						kDieNpcInfo.m_bIsBoss ) == false )
#else
					if( SiKDropTable()->HenirNpcDropItem( static_cast<CXSLDungeon::DUNGEON_MODE>(m_cDungeonMode), 
						kDieNpcInfo.m_iNPCID,
						kDieNpcInfo.m_cLevel,
						fDungeonFactor,
						fContribution,
						fPartyFactor,
						iPartyUserCount,
						kNpcDietNot.m_bDecreaseEndurance,
						sDropDataNpc,
						kDieNpcInfo.m_bIsBoss ) == false )
#endif SERV_ITEM_DROP_EVENT
					//}}
					{
						START_LOG( cerr, L"Henir Drop Item Data 얻어오기 실패 ===" )
							<< BUILD_LOG( kPacket_.m_nDieNPCUID )
							<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
							<< BUILD_LOGc( m_cDungeonMode )
							<< END_LOG;
					}

					// 드롭테이블에 EXP가 세팅되어있는 몬스터라면 내구도 감소 체크를 한다.
					if( sDropDataNpc.m_iExp > 0 )
					{
						sDropDataNpc.m_iExp = 0; // 헤니르 시공은 경험치 획득을 하면 안되기 때문에 0으로 변경한다.
					}
				}
				break;

			case CXSLDungeon::DT_SECRET:
				{
					// EXTRA STAGE인 경우..
					if( IsEnteredExtraStage() == true )
					{
						if( SiKDropTable()->ExtraStageNpcDropItem( kDieNpcInfo.m_iNPCID,
							kDieNpcInfo.m_cLevel,
							fLevelFactor,
							fDungeonFactor,
							fContribution,
							fPartyFactor,
							iPartyUserCount,
							//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
							m_fItemDropEventProbRate,
#else // SERV_DROP_EVENT_RENEWAL
							m_iItemDropEventProbCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
							//}}
							kNpcDietNot.m_bDecreaseEndurance,
							sDropDataNpc,
							kDieNpcInfo.m_bIsBoss ) == false )
						{
							START_LOG( cerr, L"Henir Drop Item Data 얻어오기 실패 ===" )
								<< BUILD_LOG( kPacket_.m_nDieNPCUID )
								<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
								<< END_LOG;
						}
					}
					else
					{
						if( SiKDropTable()->NormalNpcDropItem( GetDungeonIDAndDif(), 
							kDieNpcInfo.m_iNPCID,
							kDieNpcInfo.m_cLevel,
							fLevelFactor,
							fDungeonFactor,
							fContribution,
							fPartyFactor,
							iPartyUserCount,
							//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
							m_fItemDropEventProbRate,
#else // SERV_DROP_EVENT_RENEWAL
							m_iItemDropEventProbCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
							//}}
							kNpcDietNot.m_bDecreaseEndurance,
							sDropDataNpc,
							kDieNpcInfo.m_bIsBoss ) == false )
						{
							START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
								<< BUILD_LOG( kPacket_.m_nDieNPCUID )
								<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
								<< BUILD_LOGc( GetDifficultyLevel() )
								<< END_LOG;
						}
					}
				}
				break;	

			default:
				{
					if( SiKDropTable()->NormalNpcDropItem( GetDungeonIDAndDif(), 
						kDieNpcInfo.m_iNPCID,
						kDieNpcInfo.m_cLevel,
						fLevelFactor,
						fDungeonFactor,
						fContribution,
						fPartyFactor,
						iPartyUserCount,
						//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
						m_fItemDropEventProbRate,
#else // SERV_DROP_EVENT_RENEWAL
						m_iItemDropEventProbCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
						//}}
						kNpcDietNot.m_bDecreaseEndurance,
						sDropDataNpc,
						kDieNpcInfo.m_bIsBoss ) == false )
					{
						START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
							<< BUILD_LOG( kPacket_.m_nDieNPCUID )
							<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
							<< BUILD_LOGc( GetDifficultyLevel() )
							<< END_LOG;
					}
				}
				break;
			}

			//{{ 2012. 11. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			// 마나 이터, 블러드 이터, 중간보스, 보스 스페셜 드롭
			SiKDropTable()->NpcSpecialDropItem( kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cMonsterGrade, kPacket_.m_cNpcDieState, sDropDataNpc.m_vecItemID );
#else
//			//{{ 2009. 5. 18  최육사	마나이터
//			switch( kDieNpcInfo.m_iNPCID )
//			{
//			case CXSLUnitManager::NUI_MANA_EATER:
//			case CXSLUnitManager::NUI_HALLOWEEN_MANA_EATER:
//			case CXSLUnitManager::NUI_DEFENCE_MANA_EATER:
//				{
//					if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
//					{
//						// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//						sDropDataNpc.m_vecItemID.push_back( 70055 ); // 70055
//					}
//				}
//				break;
//
//			case CXSLUnitManager::NUI_BLOOD_EATER:
//			case CXSLUnitManager::NUI_DEFENCE_BLOOD_EATER:
//				{
//					if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_BONUS_DROP )
//					{
//						// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//						sDropDataNpc.m_vecItemID.push_back( 70056 ); // 70055
//					}
//				}
//				break;
//			}		
//			//}}
//
//			//{{ 보스, 중보 잡을 때 아이템 드롭 되는 기능(MonsterGrade 기준) - 김민성
//#ifdef SERV_KILL_BOSS_MIDDLE_BOSS_DROP_ITEM
//			
//			if( kDieNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_MIDDLE_BOSS_NPC )
//			{
//				// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//				if( rand() % 100 < 50 )	// 중보스는 50% 확률로만 드랍
//				{
//					sDropDataNpc.m_vecItemID.push_back( 70057 ); // 70057
//				}
//			}
//			else if( kDieNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC )
//			{
//				// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//				sDropDataNpc.m_vecItemID.push_back( 70058 ); // 70057
//			}
//#endif SERV_KILL_BOSS_MIDDLE_BOSS_DROP_ITEM
//			//}	
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}

#ifdef SERV_DROP_FOR_FINISHER_ONLY
			bool bIsFinisher = false;
			if( spRoomUser->GetCID() == kPacket_.m_iFinisherUID )
				bIsFinisher = true;
#endif SERV_DROP_FOR_FINISHER_ONLY

#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
			// 보스 NPC를 죽였을 때에는
			if( kDieNpcInfo.m_cMonsterGrade == CXSLUnitManager::MG_BOSS_NPC )
			{
				//bool isbCharMale = false;
				bool isbCharQuest = false;
				//CXSLUnit::UNIT_TYPE eUnitType = (CXSLUnit::UNIT_TYPE)kPacket_.m_cAttUnitType;


				KRoomUserInfo kInfoChar;
				spRoomUser->GetRoomUserInfo(kInfoChar);

				std::map< int, KSubQuestInfo >::const_iterator mitQ = kInfoChar.m_mapOngoingQuest.find(_CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestDungeonPlayerQuest);
				if( mitQ != kInfoChar.m_mapOngoingQuest.end() )
				{
					isbCharQuest = true;
				}

				// 현재 퀘스트 관련 정보와 캐릭터 성별 정보 비교에서 같을 경우에만 아이템 지급
				//if(_CONST_BURNING_CHAR_EVENT_SUB_QUEST_::bCharMale == isbCharMale && true == isbCharQuest)
				if( isbCharQuest && bIsFinisher && CXSLDungeon::IsRubenDungeon( GetDungeonID() ) == false )
				{
					sDropDataNpc.m_vecItemID.push_back( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestDungeonPlayerItem );

					//START_LOG( clog, L"김석근_캐릭터 버닝이벤트_던전보스 처치 아이템지급" )
					//	<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestDungeonPlayerItem )
					//	<< BUILD_LOG( _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBestDungeonPlayerQuest )
					//	<< BUILD_LOG( GetDungeonID() )
					//	<< END_LOG;
				}
			}
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST

			//////////////////////////////////////////////////////////////////////////		
			//if( sDropDataNpc.m_iExp > 0 )
			{
#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// 적용날짜: 2013-08-13
				float fNpcExpRate = SiCXSLDungeonManager()->GetNPCExpRate( GetDungeonIDAndDif(), GetStageID(), GetSubStageID(), kDieNpcInfo.m_iNPCID );
				sDropDataNpc.m_iExp = static_cast<int>(sDropDataNpc.m_iExp * fNpcExpRate);
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE
				
				m_spRoomUserManager->AddDungeonRewardEXP( spRoomUser->GetCID(), sDropDataNpc.m_iExp, GetDungeonIDAndDif(), GetDifficultyLevel(), fLevelFactor, kNpcDietNot );
			}

			//{{ 2012. 11. 15	최육사		피버 증가 버그 수정
#ifdef SERV_FEVER_INCREASE_BUG_FIX
			//////////////////////////////////////////////////////////////////////////
			// 내구도를 깎는 몬스터는 경험치를 주는 몬스터 이므로 파티 fever도 증가시키자!
			if( kNpcDietNot.m_bDecreaseEndurance )
			{
				// 한번만 올려주어야 한다.
				if( spRoomUser->GetCID() == FIRST_SENDER_UID )
				{
					IncreasePartyFever(); // 경험치 보상을 주는 몬스터를 죽인경우 party fever수치를 올린다!
				}
			}
#endif SERV_FEVER_INCREASE_BUG_FIX
			//}}

			//////////////////////////////////////////////////////////////////////////
			// 던전 파티원들에게 각자 얻게 되는 정보 전달 ( 경험치 정보 포함 )
			SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_NPC_UNIT_DIE_NOT, kNpcDietNot );

			// 드롭을 하지 않는 npc라면 드롭을 하지 않는다.
			if( kDieNpcInfo.m_bNoDrop == true )
				continue;

			// 드롭상태가 아닌채로 죽은npc라면 아이템 드롭을 하지 않는다.
			if( bDropDieState == false )
				continue;

			// 개인 기여도 * 파티 보너스
			const float fTotalDropRate = fContribution;// * fPartyFactor;

			//////////////////////////////////////////////////////////////////////////
			// 2. 던전을 기준으로 드롭 아이템이 결정되는 던전 드롭
			KDropTable::DROP_DATA sDropDataStatic;

			if( SiKDropTable()->StaticDropItem( GetDungeonIDAndDif(), sDropDataStatic, fTotalDropRate ) == false )
			{
				START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
					<< BUILD_LOG( kPacket_.m_nDieNPCUID )
					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
					<< BUILD_LOG( GetDungeonID() )
					<< BUILD_LOGc( GetDifficultyLevel() )
					<< END_LOG;
			}

			//////////////////////////////////////////////////////////////////////////	
			// 3. 이벤트 아이템 드롭 체크
			KDropTable::DROP_DATA sDropDataEvent;

			if( kDieNpcInfo.m_bActive == true )
			{
				// 이벤트 아이템은 초심자숲, 튜토리얼, 훈련소에서 드롭되서는 안된다!
				if( SEnum::DI_EL_FOREST_GATE_NORMAL != GetDungeonID()  &&
					CXSLDungeon::IsTutorialDungeon( GetDungeonID() ) == false &&
					CXSLDungeon::IsTCDungeon( GetDungeonID() ) == false )
				{
					if( SiKDropTable()->EventDropItem( fTotalDropRate, sDropDataEvent ) == false )
					{
						START_LOG( cerr, L"Event Drop Item Data 얻어오기 실패 ===" )
							<< BUILD_LOG( kPacket_.m_nDieNPCUID )
							<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
							<< BUILD_LOGc( GetDifficultyLevel() )
							<< END_LOG;
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 4. 속성 몬스터 드롭 아이템
			KDropTable::DROP_DATA sDropDataAttribNpc;

			if( bIsAttribNpc == true  /*&&  bDropDieState == true*/ )
			{
				char cDifficultyLevel = GetDifficultyLevel();

				// 비밀던전인가?
				if( m_eDungeonType == CXSLDungeon::DT_SECRET )
				{
					if( m_cDungeonMode == CXSLDungeon::DM_SECRET_NORMAL )
					{
						cDifficultyLevel = CXSLDungeon::DL_HARD;
					}
					else if( m_cDungeonMode == CXSLDungeon::DM_SECRET_HELL )
					{
						// Extra Stage인가?
						if( IsEnteredExtraStage() == true )
						{
							cDifficultyLevel = CXSLDungeon::DL_NORMAL;
						}
						else
						{
							cDifficultyLevel = CXSLDungeon::DL_EXPERT;
						}
					}
				}

				//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(cDifficultyLevel), iNpcLv, fTotalDropRate, m_fItemDropEventProbRate, sDropDataAttribNpc.m_vecItemID ) == false )
#else // SERV_DROP_EVENT_RENEWAL
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(cDifficultyLevel), iNpcLv, fTotalDropRate, m_iItemDropEventProbCount, sDropDataAttribNpc.m_vecItemID ) == false )
#endif // SERV_DROP_EVENT_RENEWAL
#else
				if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(cDifficultyLevel), iNpcLv, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
#endif SERV_ITEM_DROP_EVENT
				//}}
				{
					START_LOG( cerr, L"속성 몬스터 아이템 드롭 실패!" )
						<< BUILD_LOGc( cDifficultyLevel )
						<< BUILD_LOG( iNpcLv )
						<< END_LOG;
				}

				switch( cDifficultyLevel )
				{
				case CXSLDungeon::DL_NORMAL:
				case CXSLDungeon::DL_HARD:
					{
						sDropDataNpc.m_nGPNum = 2 * sDropDataNpc.m_nGPNum;
					}
					break;

				case CXSLDungeon::DL_EXPERT:
					{
						sDropDataNpc.m_nGPNum = 3 * sDropDataNpc.m_nGPNum;
					}
					break;

				default:
					START_LOG( cerr, L"던전 난이도가 이상합니다." )
						<< BUILD_LOGc( cDifficultyLevel )
						<< END_LOG;
					break;
				}

				if( SiKDropTable()->AttribNpcSpecialDrop( cDifficultyLevel, fTotalDropRate, sDropDataAttribNpc.m_vecItemID ) == false )
				{
					START_LOG( cerr, L"속성 몬스터 스페셜 아이템 드롭 실패!" )
						<< BUILD_LOGc( cDifficultyLevel )
						<< END_LOG;
				}
			}	

			//////////////////////////////////////////////////////////////////////////
			// 5. 퀘스트 드롭 아이템 처리

			std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;

			// 해당 캐릭터가 퀘스트 드롭 아이템이 있는지 검사..
			//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			if( m_spRoomUserManager->GetQuestDropItem( spRoomUser->GetCID(), GetDungeonID(), GetDifficultyLevel(), m_cDungeonMode, kDieNpcInfo.m_iNPCID, vecDropInfo, m_fQuestItemDropEventProbRate ) == true )
#else
			if( m_spRoomUserManager->GetQuestDropItem( spRoomUser->GetCID(), GetDungeonID(), GetDifficultyLevel(), m_cDungeonMode, kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
#endif SERV_QUEST_ITEM_DROP_EVENT
			//}
			{
				BOOST_TEST_FOREACH( const KRoomUser::QUEST_ITEM_DROP_INFO&, kDropInfo, vecDropInfo )
				{
					//드롭된 아이템 수만큼 처리를 시작..
					BOOST_TEST_FOREACH( const int, iDropQuestItemID, kDropInfo.m_vecDropQuestItem )
					{
						KEGS_GET_ITEM_NOT kDropNot;
						kDropNot.m_cGetItemType = KEGS_GET_ITEM_NOT::GIT_QUEST_ITEM;
						kDropNot.m_GetUnitUID	= spRoomUser->GetCID();
						kDropNot.m_iItemID		= iDropQuestItemID;
						kDropNot.m_iDropItemUID = m_nDropItemUID++;
						SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kDropNot );
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

			//////////////////////////////////////////////////////////////////////////
			// 6. 헤니르 시공 스페셜 드롭 처리
			KDropTable::DROP_DATA sDropDataHenir;

			if( CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == true )
			{
				//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
				SiKDropTable()->GetHenirSpecialNpcDropInfo( kDieNpcInfo.m_iNPCID, fTotalDropRate, sDropDataHenir );
#else
				SiKDropTable()->GetHenirSpecialNpcDropInfo( kDieNpcInfo.m_iNPCID, sDropDataHenir );
#endif SERV_REFORM_ITEM_DROP
					//}}
			}

			//////////////////////////////////////////////////////////////////////////	
			// 위에서 결정된 드롭 아이템 리스트를 실제 유저에게 보내기 위한 처리
			KEGS_DROP_ITEM_NOT kDropItemNot;
			kDropItemNot.m_CreatePos = kPacket_.m_DiePos;

			// 헤니르 던전이라면 드롭 가능한 아이템인지 확인 하자.
			if( CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == true )
			{
				// 보상 받을 수 있는 유저가 없다.
				if( spRoomUser->IsHenirRewardUser() == false )
				{
					CheckDeleteHenirDropItem( sDropDataNpc, sDropDataStatic, sDropDataEvent, sDropDataAttribNpc, sDropDataHenir );
				}
			}

			if( spRoomUser->ExistActiveBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO ) == true )
			{
				if( m_cRoomType == CXSLRoom::RT_DUNGEON )
				{
					if( CXSLDungeon::IsHenirDungeon( GetDungeonID() )  == true )
					{
						if( spRoomUser->IsHenirRewardUser() == false )
						{
							if( SiKDropTable()->GetComeBackBuffDropCount( m_spRoomUserManager->GetNumMember() ) == true )
							{
								// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
								kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260 ) );		// 새로운 모험의 징표
							}
						}
					}
					else
					{
						if( SiKDropTable()->GetComeBackBuffDropCount( m_spRoomUserManager->GetNumMember() ) == true )
						{
							// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
							kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260 ) );		// 새로운 모험의 징표
						}
					}
				}
			}
			
			//1.ED 드롭처리
			if( sDropDataNpc.m_iED != -1 )
			{
				for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, true, sDropDataNpc.m_iED ) );
				}
			}

			//2.NPC ITEM 드롭처리	
			if( sDropDataNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
				{
					// 특정 아이템이 특정던전에서 드롭되지 않도록 하는 코드
					if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
						continue;

					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
				}
			}

			//{{ 2009. 4. 22  최육사	전체드롭
			if( sDropDataStatic.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
				{
					// 특정 아이템이 특정던전에서 드롭되지 않도록 하는 코드
					if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
						continue;

					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
				}
			}
			//}}

			//3.EVENT ITEM 드롭처리
			if( kDieNpcInfo.m_bActive == true && sDropDataEvent.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataEvent.m_vecItemID )
				{
					// 특정 아이템이 특정던전에서 드롭되지 않도록 하는 코드
					if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
						continue;

					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
				}
			}

			//4.ATTRIB NPC ITEM 드롭처리
			if( kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
				}
			}

			//5.HENIR NPC SPECIAL 드롭처리
			if( sDropDataHenir.m_vecItemID.empty() == false )
			{
				BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataHenir.m_vecItemID )
				{
					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
				}
			}

			//드롭처리한 아이템이 있으면 패킷을 보낸다.
			if( kDropItemNot.m_DropItemDataList.empty() == false )
			{	
				SendToGSCharacter( spRoomUser->GetGSUID(), spRoomUser->GetCID(), ERM_DROP_ITEM_NOT, kDropItemNot );
			}
		}	// for 문 end


		//////////////////////////////////////////////////////////////////////////
		// 특정 시각 아이템 드롭 이벤트
		if( kDieNpcInfo.m_bActive == true )
		{
			// 이벤트 아이템은 초심자숲, 튜토리얼, 훈련소에서 드롭되서는 안된다!
			if( SEnum::DI_EL_FOREST_GATE_NORMAL != GetDungeonID()  &&
				CXSLDungeon::IsTutorialDungeon( GetDungeonID() ) == false &&
				CXSLDungeon::IsTCDungeon( GetDungeonID() ) == false )
			{
				//////////////////////////////////////////////////////////////////////////
				// 특정 시간에 NPC때려잡으면 보상주는 기능
				if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
				{
					KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_uiAttUnit );
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
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 던전 파티원들에게 해당 npc가 죽은것에 대한 정보를 브로드 캐스팅 한다! ( 경험치 정보 포함 )
		// 방안의 인원에게 알려줄 데이터 셋팅 시작..
		KERM_NPC_UNIT_DIE_NOT kNpcDietNot;
		kNpcDietNot.m_iDungeonID	= GetDungeonID();
		kNpcDietNot.m_cDifficulty	= GetDifficultyLevel();
		kNpcDietNot.m_nDieNPCUID	= kPacket_.m_nDieNPCUID;
		kNpcDietNot.m_iNPCID		= kDieNpcInfo.m_iNPCID;
		kNpcDietNot.m_cAttUnitType	= kPacket_.m_cAttUnitType;
		kNpcDietNot.m_uiAttUnit		= kPacket_.m_uiAttUnit;
		kNpcDietNot.m_cUserCount	= static_cast<char>( m_spRoomUserManager->GetNumMember() );
		kNpcDietNot.m_bNoDrop		= kDieNpcInfo.m_bNoDrop;
		kNpcDietNot.m_sSubStageExpNpcNum = GetSubStageExpNpcNum( m_iSubStageID );
		kNpcDietNot.m_bDecreaseEndurance = false;
		kNpcDietNot.m_cDungeonMode	= m_cDungeonMode;
		kNpcDietNot.m_EXPList.clear();
		kNpcDietNot.m_bQuestComplete = true;

		BroadCast( ERM_NPC_UNIT_DIE_NOT, kNpcDietNot );
	}
}

//////////////////////////////////////////////////////////////////////////
#else			// SERV_REFORM_ITEM_DROP
//////////////////////////////////////////////////////////////////////////

//_IMPL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ )
//{
//	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_NPC_UNIT_DIE_ACK, KERM_NPC_UNIT_DIE_ACK );
//
//	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
//	std::set< int >::iterator sit = m_setDefenseSmallCrystalNpcUID.find( kPacket_.m_nDieNPCUID );
//	if( sit != m_setDefenseSmallCrystalNpcUID.end() )
//	{
//		kPacket.m_iOK		= NetError::NET_OK;
//		kPacket.m_iNPCID	= kPacket_.m_nDieNPCUID;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//
//		// std::set 에서 삭제
//		m_setDefenseSmallCrystalNpcUID.erase( sit );
//
//		START_LOG( clog, L"디펜스 던전 작은 수정이 깨졌음!!" )
//			<< END_LOG;
//		return;
//	}
//#endif SERV_INSERT_GLOBAL_SERVER
//	//}} 2011. 04. 13  김민성  글로벌 서버 추가
//
//	//생성된 몬스터 리스트에서 찾는데 없을 경우..
//	KRoomMonsterManager::NPC_DATA kDieNpcInfo;
//	if( m_kDungeonMonsterManager.GetNpcData( kPacket_.m_nDieNPCUID, kDieNpcInfo ) == false )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_20;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//
//		//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
//#ifdef SERV_MAKE_MONSTER_UID_CHANGE
//		// 발급 되지 않는 NPC UID 를 클라이언트가 보냈다! 
//		// 해킹으로 간주하여 메일을 쏘게 하자! 
//		// 해당 룸에서 한번만....
//		if( GetHackingNpcUid() == false )
//		{
//			SetHackingNpcUid( true );
//
//			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
//#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//			KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
//			kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_VERIFY_DUNGEON_NPC_UID;
//#else
//			KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
//			kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_VERIFY_DUNGEON_NPC_UID;
//#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//			//}}
//			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
//
//			START_LOG( clog, L"발급 되지 않는 NPC UID 를 클라이언트가 보냈다!" )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOGc( kNot.m_cHackingCheckType )
//				<< END_LOG;
//		}
//#endif SERV_MAKE_MONSTER_UID_CHANGE	
//		//}}
//
//		return;
//	}
//
//	//있다면 이미 죽었는지 체크..
//	if( m_kDungeonMonsterManager.IsMonsterAlive( kPacket_.m_nDieNPCUID ) == false )
//	{
//		kPacket.m_iOK = NetError::ERR_ROOM_21;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
//		return;
//	}
//
//	//아직 살아있다면 죽인사람과 죽은것으로 설정..
//	LIF( m_kDungeonMonsterManager.SetMonsterDie( kPacket_.m_nDieNPCUID, kPacket_.m_uiAttUnit ) );
//
//	kPacket.m_iOK		= NetError::NET_OK;
//	kPacket.m_iNPCID	= kDieNpcInfo.m_iNPCID;
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_NPC_UNIT_DIE_ACK, kPacket );
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
//	kNpcDietNot.m_cUserCount	= static_cast<char>( m_spRoomUserManager->GetNumMember() );
//	kNpcDietNot.m_bNoDrop		= kDieNpcInfo.m_bNoDrop;
//
//	//{{ 2009. 12. 31  최육사	내구도소모개편
//	kNpcDietNot.m_sSubStageExpNpcNum = GetSubStageExpNpcNum( m_iSubStageID );
//	kNpcDietNot.m_bDecreaseEndurance = false;
//	//}}
//
//	//{{ 2011. 08. 31  김민성	비밀던전 헬모드
//#ifdef SERV_RANDOM_DAY_QUEST
//	kNpcDietNot.m_cDungeonMode	= m_cDungeonMode;
//#endif SERV_RANDOM_DAY_QUEST
//	//}}
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
//	//////////////////////////////////////////////////////////////////////////	
//	//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
//#ifdef SERV_TIME_DROP_MONSTER_EVENT
//	{
//		std::map< int, int >::const_iterator mitTDM = m_mapTimeDropEventMonster.find( kDieNpcInfo.m_iNPCID );
//		if( mitTDM != m_mapTimeDropEventMonster.end() )
//		{
//			KDBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT kPacketToLog;
//			kPacketToLog.m_iEventUID = mitTDM->second;
//			kPacketToLog.m_iKillUnitUID = kPacket_.m_uiAttUnit;
//			kPacketToLog.m_tNpcDieDate = CTime::GetCurrentTime().GetTime();
//			SendToLogDB( DBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT, kPacketToLog );
//
//			// 이벤트 몬스터 정보를 초기화 합니다.
//			m_mapTimeDropEventMonster.clear();
//		}
//	}
//#endif SERV_TIME_DROP_MONSTER_EVENT
//	//}}
//
//	//////////////////////////////////////////////////////////////////////////	
//	//{{ 2010. 7. 28  최육사	오토핵 차단 NPC
//#ifdef SERV_AUTO_HACK_CHECK_NPC
//	if( kNpcDietNot.m_cUserCount == 1 )
//	{
//		switch( kDieNpcInfo.m_iNPCID )
//		{
//		case CXSLUnitManager::NUI_DUNGEON_CHECKER1:
//		case CXSLUnitManager::NUI_DUNGEON_CHECKER2:
//		case CXSLUnitManager::NUI_DUNGEON_CHECKER3:
//		case CXSLUnitManager::NUI_DUNGEON_CHECKER4:
//		case CXSLUnitManager::NUI_DUNGEON_CHECKER5:
//			{
//				// 핵유저를 접속 종료시키기 위해 예약 해놓자!
//				m_setFindedHackUserUnitUID.insert( FIRST_SENDER_UID );
//
//				// 파티장에게 해킹체크패킷을 보내자!
//				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
//#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
//				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_FIND;
//#else
//				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
//				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CANT_KILL_NPC_FIND;
//#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//				//}}
//				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
//
//				START_LOG( cout, L"Cant Kill Npc 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
//					<< BUILD_LOG( LAST_SENDER_UID )
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//					<< BUILD_LOGc( kPacket_.m_cAttUnitType )
//					<< BUILD_LOG( kPacket_.m_uiAttUnit )
//					<< BUILD_LOG( kPacket_.m_uiMDAttUnit )
//					<< BUILD_LOG( kPacket_.m_DiePos.x )
//					<< BUILD_LOG( kPacket_.m_DiePos.y )
//					<< BUILD_LOG( kPacket_.m_DiePos.z )
//					<< BUILD_LOGc( kPacket_.m_cNpcDieState );
//			}
//			break;
//		}
//	}	
//#endif SERV_AUTO_HACK_CHECK_NPC
//	//}}
//	//////////////////////////////////////////////////////////////////////////	
//
//	//////////////////////////////////////////////////////////////////////////
//	//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
//#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
//	if( SiKGameSysVal()->GetSubStageNpcDieCheck() == true )
//	{
//		// 솔플 인가? // 액티브 NPC 가 5마리 이상인가?
//		if( m_spRoomUserManager->GetNumMember() == 1 
//			&& m_iSubStageActiveNpc >= 5 
//			&& kDieNpcInfo.m_bActive == true )
//		{
//			++m_iSubStageKillActiveNpc;
//
//			if( m_iSubStageKillActiveNpc == 1 )
//			{
//				// 서브 스테이지에서 처음 액티브 몬스터를 잡은 시간
//				m_tSubStageFirstKillTime = CTime::GetCurrentTime();
//			}
//			else
//			{
//				if( m_iSubStageActiveNpc <= m_iSubStageKillActiveNpc ) // 마지막 몬스터를 잡았다!
//				{
//					CTime tLastKillTime = CTime::GetCurrentTime();
//					CTimeSpan tSpan = CTimeSpan( 0, 0, 0, 1);
//					if( tLastKillTime <= m_tSubStageFirstKillTime + tSpan ) 		// 처음 몬스터를 잡은 시간에서 1초이하 밖에 걸리지 않았다
//					{
//						// 파티장에게 해킹체크패킷을 보내자!
//						//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
//#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//						KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
//						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_NPC_DIE_TIME;
//#else
//						KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
//						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_NPC_DIE_TIME;
//#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//						//}}
//						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
//
//						START_LOG( cout, L"Sub Stage의 모든 Npc를 죽인 시간이 1초밖에 걸리지 않는다." )
//							<< BUILD_LOG( LAST_SENDER_UID )
//							<< BUILD_LOG( FIRST_SENDER_UID )
//							<< BUILD_LOG( m_tSubStageFirstKillTime.GetMinute() )
//							<< BUILD_LOG( m_tSubStageFirstKillTime.GetSecond() )
//							<< BUILD_LOG( tLastKillTime.GetMinute() )
//							<< BUILD_LOG( tLastKillTime.GetSecond() )
//							<< BUILD_LOG( m_iSubStageActiveNpc )
//							<< BUILD_LOG( m_iSubStageKillActiveNpc )
//							<< END_LOG;
//					}
//				}
//			}
//		}
//	}
//#endif SERV_SUB_STAGE_NPC_DIE_CHECK
//	//}}
//	//////////////////////////////////////////////////////////////////////////
//
//
//	// 파티 플레이일 경우 아이템 드롭 카운트 증가
//	const int iDropItemNum = ( m_spRoomUserManager->GetLiveMember() >= 3 ? 2 : 1 );
//	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장
//#ifdef SERV_PC_BANG_DROP_EVENT
//	const int iEventDropItemNum = m_iItemDropEventProbCount * iDropItemNum;
//#endif SERV_PC_BANG_DROP_EVENT
//	//}}
//
//	// 속성 몬스터인지 여부 검사	
//	int iNpcLv = 0;
//	const bool bIsAttribNpc = IsAttribNpc( kPacket_.m_nDieNPCUID, iNpcLv );
//
//	// 몬스터 타입 상수 얻기
//	const char cMonsterTypeFactor = static_cast<char>( CXSLUnitManager::GetMonsterTypeFactor( bIsAttribNpc, GetDifficultyLevel(), kDieNpcInfo.m_cMonsterGrade ) );	
//
//	//////////////////////////////////////////////////////////////////////////
//	// uiAttUnit값이 -1이면 유저의 공격을 받지 않고 npc가 자살한것임. [예:지뢰함정]
//
//	// 플레이어가 몬스터를 죽인거라면 해당 플레이어의 킬몬스터수 체크
//	if( pNPCTemplet->m_bIsNPC  &&  bAddRoomUserKillCount == true  &&  kPacket_.m_uiAttUnit != -1 )
//	{
//		if( m_spRoomUserManager->AddKillNPC( kPacket_.m_uiAttUnit ) == false )
//		{
//			if( m_kDungeonMonsterManager.IsExistMonster( static_cast<int>(kPacket_.m_uiAttUnit) ) == true )
//			{
//				START_LOG( clog, L"몹끼리 죽였네!!" )
//					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//					<< BUILD_LOG( kPacket_.m_uiAttUnit );
//
//				// 몬스터가 함정에 걸려 죽는경우임 : 아이템은 그대로 드롭처리함.
//			}
//			else
//			{
//				//{{ 2010. 06. 17  최육사	오토핵 방지
//#ifdef SERV_PROTECT_AUTO_HACK
//
//				// 던전 게임 시작할 당시에 유저 수와 현재 유저 수가 모두 1명일 경우만 체크!
//				if( m_iStartedNumMember == 1  &&  kNpcDietNot.m_cUserCount == 1 )
//				{
//					START_LOG( cerr, L"몹끼리 죽인게 아니네? 해킹유저로 의심된다!" )
//						<< BUILD_LOG( LAST_SENDER_UID )
//						<< BUILD_LOG( FIRST_SENDER_UID )
//						<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//						<< BUILD_LOGc( kPacket_.m_cAttUnitType )
//						<< BUILD_LOG( kPacket_.m_uiAttUnit )
//						<< BUILD_LOG( kPacket_.m_uiMDAttUnit )
//						<< BUILD_LOG( kPacket_.m_DiePos.x )
//						<< BUILD_LOG( kPacket_.m_DiePos.y )
//						<< BUILD_LOG( kPacket_.m_DiePos.z )
//						<< BUILD_LOGc( kPacket_.m_cNpcDieState )
//						<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//						<< END_LOG;
//
//					// 오토핵 체크 카운트 증가!
//					++m_iNpcDieHackCount;
//
//					// 이 에러가 5회 이상 발생한다면 그것은 잘못된 것이다!
//					if( m_iNpcDieHackCount == m_iHackUserCheckCount )
//					{
//						// 파티장에게 해킹체크패킷을 보내자!
//						//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
//#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//						KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
//						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_NPC_UNIT_DIE;
//#else
//						KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
//						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_NPC_UNIT_DIE;
//#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//						//}}
//						SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );
//
//						START_LOG( cout, L"NpcDie 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
//							<< BUILD_LOG( LAST_SENDER_UID )
//							<< BUILD_LOG( FIRST_SENDER_UID )
//							<< BUILD_LOG( m_iNpcDieHackCount );
//					}
//				}
//#endif SERV_PROTECT_AUTO_HACK
//				//}}
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
//		//{{ 2011. 01. 20	최육사	던전 랭크 개편
//#ifdef SERV_DUNGEON_RANK_NEW
//		if( pNPCTemplet->m_bIsNPC == true  &&  kDieNpcInfo.m_bActive == true )
//		{
//			m_kDungeonMonsterManager.IncreaseMonsterDieCount( cMonsterTypeFactor );
//		}
//#endif SERV_DUNGEON_RANK_NEW
//		//}}
//
//		//{{ 2009. 7. 26  최육사	헤니르 시공
//		//ITEM
//		switch( m_eDungeonType )
//		{
//		case CXSLDungeon::DT_HENIR:
//			{
//				if( SiKDropTable()->HenirNpcDropItem( static_cast<CXSLDungeon::DUNGEON_MODE>(m_cDungeonMode), kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cLevel, cMonsterTypeFactor, sDropDataNpc, iEventDropItemNum, kDieNpcInfo.m_bIsBoss ) == false )
//				{
//					START_LOG( cerr, L"Henir Drop Item Data 얻어오기 실패 ===" )
//						<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//						<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//						<< BUILD_LOGc( m_cDungeonMode )
//						<< END_LOG;
//				}
//
//				//{{ 2010. 01. 05  최육사	내구도개편
//				// 드롭테이블에 EXP가 세팅되어있는 몬스터라면 내구도 감소 체크를 한다.
//				if( sDropDataNpc.m_iExp > 0 )
//				{
//					kNpcDietNot.m_bDecreaseEndurance = true;
//
//					sDropDataNpc.m_iExp = 0; // 헤니르 시공은 경험치 획득을 하면 안되기 때문에 0으로 변경한다.
//				}
//				//}}			
//			}
//			break;
//
//			//{{ 2010. 05. 02  최육사	비밀던전 헬모드
//#ifdef SERV_SECRET_HELL
//		case CXSLDungeon::DT_SECRET:
//			{
//				// EXTRA STAGE인 경우..
//				if( IsEnteredExtraStage() == true )
//				{
//					if( SiKDropTable()->ExtraStageNpcDropItem( kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cLevel, cMonsterTypeFactor, sDropDataNpc, iEventDropItemNum, kDieNpcInfo.m_bIsBoss ) == false )
//					{
//						START_LOG( cerr, L"Henir Drop Item Data 얻어오기 실패 ===" )
//							<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//							<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//							<< END_LOG;
//					}
//				}
//				else
//				{
//					if( SiKDropTable()->NormalNpcDropItem( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cLevel, cMonsterTypeFactor, sDropDataNpc, iEventDropItemNum, kDieNpcInfo.m_bIsBoss ) == false )
//					{
//						START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
//							<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//							<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//							<< BUILD_LOGc( GetDifficultyLevel() )
//							<< END_LOG;
//					}
//				}
//
//				//{{ 2010. 01. 05  최육사	내구도개편
//				// 드롭테이블에 EXP가 세팅되어있는 몬스터라면 내구도 감소 체크를 한다.
//				if( sDropDataNpc.m_iExp > 0 )
//				{
//					kNpcDietNot.m_bDecreaseEndurance = true;
//				}
//				//}}
//			}
//			break;	
//#endif SERV_SECRET_HELL
//			//}}
//
//		default:
//			{
//				if( SiKDropTable()->NormalNpcDropItem( GetDungeonIDAndDif(), kDieNpcInfo.m_iNPCID, kDieNpcInfo.m_cLevel, cMonsterTypeFactor, sDropDataNpc, iEventDropItemNum, kDieNpcInfo.m_bIsBoss ) == false )
//				{
//					START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
//						<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//						<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//						<< BUILD_LOGc( GetDifficultyLevel() )
//						<< END_LOG;
//				}
//
//				//{{ 2010. 01. 05  최육사	내구도개편
//				// 드롭테이블에 EXP가 세팅되어있는 몬스터라면 내구도 감소 체크를 한다.
//				if( sDropDataNpc.m_iExp > 0 )
//				{
//					kNpcDietNot.m_bDecreaseEndurance = true;
//				}
//				//}}
//			}
//		}
//		//}}
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
//		//{{ 2011. 01. 19	최육사	마을 이동 던전
//#ifdef SERV_SHIP_DUNGEON
//		if( CXSLDungeon::IsShipDungeon( GetDungeonID() ) == true )
//		{
//			// 마을 이동 던전에서 잡은 몬스터는 경험치가 없습니다.
//			sDropDataNpc.m_iExp = 0;
//		}
//#endif SERV_SHIP_DUNGEON
//		//}}
//
//		//////////////////////////////////////////////////////////////////////////		
//		//{{ 2009. 12. 18  최육사	던전경험치개편
//#ifdef DUNGEON_EXP_NEW
//		if( sDropDataNpc.m_iExp > 0 )
//		{
//			// 새로 만든 테이블로부터 경험치 데이터를 가져와야함!
//			const int nTotalExp = SiKDropTable()->GetNpcExp( kDieNpcInfo.m_cLevel ) * static_cast<int>(cMonsterTypeFactor);
//
//			//{{ 2011. 03. 21  김민성	던전 난이도 보정
//			//#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
//			m_spRoomUserManager->AddDungeonRewardEXP( nTotalExp, GetDungeonIDAndDif(), kDieNpcInfo.m_cLevel, kNpcDietNot, GetDifficultyLevel() );
//			//#else
//			//			//{{ 2010. 04. 30  최육사	비밀던전 헬모드
//			//#ifdef SERV_SECRET_HELL
//			//			m_spRoomUserManager->AddDungeonRewardEXP( nTotalExp, GetDungeonIDAndDif(), mit->second.m_cLevel, kNpcDietNot );
//			//#endif SERV_SECRET_HELL
//			//			//}}
//			//#endif SERV_DUNGEON_DIFFICULTY_REVISION
//			//}}
//
//			//{{ 2012. 02. 28	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//			IncreasePartyFever(); // 경험치 보상을 주는 몬스터를 죽인경우 party fever수치를 올린다!
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
//	BroadCast( ERM_NPC_UNIT_DIE_NOT, kNpcDietNot );
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
//
//	//if( mit->second.m_bNoDrop == false  &&  bDropDieState == true )
//	{
//		if( SiKDropTable()->StaticDropItem( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), sDropDataStatic, iDropItemNum ) == false )
//		{
//			START_LOG( cerr, L"Drop Item Data 얻어오기 실패 ===" )
//				<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//				<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//				<< BUILD_LOG( m_iDungeonID )
//				<< BUILD_LOGc( m_cDifficultyLevel )
//				<< END_LOG;
//		}
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////	
//	// 3. 이벤트 아이템 드롭 체크
//	KDropTable::DROP_DATA sDropDataEvent;
//
//	if( /*mit->second.m_bNoDrop == false  &&*/
//		kDieNpcInfo.m_bActive == true  /*&&*/
//		/*bDropDieState == true*/ )
//	{
//		// 이벤트 아이템은 초심자숲, 튜토리얼, 훈련소에서 드롭되서는 안된다!
//		if( SEnum::DI_EL_FOREST_GATE_NORMAL != m_iDungeonID  &&
//			CXSLDungeon::IsTutorialDungeon( m_iDungeonID ) == false &&
//			CXSLDungeon::IsTCDungeon( m_iDungeonID ) == false )
//		{
//			if( SiKDropTable()->EventDropItem( iDropItemNum, sDropDataEvent ) == false )
//			{
//				START_LOG( cerr, L"Event Drop Item Data 얻어오기 실패 ===" )
//					<< BUILD_LOG( kPacket_.m_nDieNPCUID )
//					<< BUILD_LOG( kDieNpcInfo.m_iNPCID )
//					<< BUILD_LOGc( m_cDifficultyLevel )
//					<< END_LOG;
//			}
//
//			//////////////////////////////////////////////////////////////////////////
//			// 특정 시간에 NPC때려잡으면 보상주는 기능
//			if( kPacket_.m_cNpcDieState == KEGS_NPC_UNIT_DIE_REQ::NDS_KILL_BY_USER )
//			{
//				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kPacket_.m_uiAttUnit );
//				if( spRoomUser != NULL )
//				{
//					// 체험ID가 아닌 유저만 보상처리
//					if( spRoomUser->IsGuestUser() == false )
//					{
//						int iTimeDropItemID = 0;
//						int iTimeDropID = 0;
//						if( SiKDropTable()->CheckTimeDrop( iTimeDropID, iTimeDropItemID ) )
//						{
//							const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iTimeDropItemID );
//							if( pItemTemplet != NULL )
//							{
//								// 일단 DB로 가서 이 유저가 보상 받아도 되는 유저인지 확인하자!
//								KDBE_CHECK_TIME_DROP_RESULT_REQ kGetTimeDrop;
//								kGetTimeDrop.m_iGSUID		 = spRoomUser->GetGSUID();
//								kGetTimeDrop.m_iUserUID		 = spRoomUser->GetUserUID();
//								kGetTimeDrop.m_iUnitUID		 = spRoomUser->GetCID();
//								kGetTimeDrop.m_wstrNickName	 = spRoomUser->GetNickName();
//								kGetTimeDrop.m_iTimeDropID	 = iTimeDropID;
//								kGetTimeDrop.m_iRewardItemID = iTimeDropItemID;
//								kGetTimeDrop.m_wstrRewardItemName = pItemTemplet->m_Name;
//								kGetTimeDrop.m_iDropItemUID	 = m_nDropItemUID++;
//								SendToLogDB( DBE_CHECK_TIME_DROP_RESULT_REQ, kGetTimeDrop );
//							}
//							else
//							{
//								START_LOG( cerr, L"존재하지 않는 itemid이다. 세팅오류인가?" )
//									<< BUILD_LOG( iTimeDropItemID )
//									<< END_LOG;
//							}
//						}
//					}
//				}
//				else
//				{
//					START_LOG( cwarn, L"이벤트 아이템 획득 처리 하려는데 유저정보가 없다!" )
//						<< BUILD_LOG( kPacket_.m_uiAttUnit )
//						<< END_LOG;
//				}
//			}
//			//////////////////////////////////////////////////////////////////////////
//		}
//	}
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 4. 속성 몬스터 드롭 아이템
//	KDropTable::DROP_DATA sDropDataAttribNpc;
//
//	if( bIsAttribNpc == true  /*&&  bDropDieState == true*/ )
//	{
//		char cDifficultyLevel = GetDifficultyLevel();
//
//		//{{ 2010. 05. 02  최육사	비밀던전 헬모드
//#ifdef SERV_SECRET_HELL
//		// 비밀던전인가?
//		if( m_eDungeonType == CXSLDungeon::DT_SECRET )
//		{
//			if( m_cDungeonMode == CXSLDungeon::DM_SECRET_NORMAL )
//			{
//				cDifficultyLevel = CXSLDungeon::DL_HARD;
//			}
//			else if( m_cDungeonMode == CXSLDungeon::DM_SECRET_HELL )
//			{
//				// Extra Stage인가?
//				if( IsEnteredExtraStage() == true )
//				{
//					cDifficultyLevel = CXSLDungeon::DL_NORMAL;
//				}
//				else
//				{
//					cDifficultyLevel = CXSLDungeon::DL_EXPERT;
//				}
//			}
//		}
//#endif SERV_SECRET_HELL
//		//}}
//
//		if( SiKAttribNpcTable()->AttribNpcDropItem( static_cast<int>(cDifficultyLevel), iNpcLv, iEventDropItemNum, sDropDataAttribNpc.m_vecItemID ) == false )
//		{
//			START_LOG( cerr, L"속성 몬스터 아이템 드롭 실패!" )
//				<< BUILD_LOGc( cDifficultyLevel )
//				<< BUILD_LOG( iNpcLv )
//				<< END_LOG;
//		}
//
//		switch( cDifficultyLevel )
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
//				<< BUILD_LOGc( cDifficultyLevel )
//				<< END_LOG;
//			break;
//		}
//
//		//{{ 2009. 12. 18  최육사	던전사용아이템
//#ifdef DUNGEON_ITEM
//
//		if( SiKDropTable()->AttribNpcSpecialDrop( cDifficultyLevel, sDropDataAttribNpc.m_vecItemID ) == false )
//		{
//			START_LOG( cerr, L"속성 몬스터 스페셜 아이템 드롭 실패!" )
//				<< BUILD_LOGc( cDifficultyLevel )
//				<< END_LOG;
//		}
//
//#endif DUNGEON_ITEM
//		//}}
//	}	
//
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// 5. 퀘스트 드롭 아이템 처리
//
//	//if( mit->second.m_bNoDrop == false )
//	{
//		std::vector< KRoomUser::QUEST_ITEM_DROP_INFO > vecDropInfo;
//
//		//유닛별 퀘스트 드롭 아이템이 있는지 검사..
//		//{{ 2011. 08. 31  김민성	비밀던전 헬모드
//#ifdef SERV_RANDOM_DAY_QUEST
//		if( m_spRoomUserManager->GetQuestDropItem( m_iDungeonID, m_cDifficultyLevel, m_cDungeonMode, kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
//#else
//		if( m_spRoomUserManager->GetQuestDropItem( m_iDungeonID, m_cDifficultyLevel, kDieNpcInfo.m_iNPCID, vecDropInfo ) == true )
//#endif SERV_RANDOM_DAY_QUEST
//			//}}
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
//	//{{ 2011. 08. 23	최육사	헤니르 시공 개편
//#ifdef SERV_NEW_HENIR_TEST
//	// 6. 헤니르 시공 스페셜 드롭 처리
//	KDropTable::DROP_DATA sDropDataHenir;
//
//	if( CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == true )
//	{
//		SiKDropTable()->GetHenirSpecialNpcDropInfo( kDieNpcInfo.m_iNPCID, sDropDataHenir );
//	}
//#endif SERV_NEW_HENIR_TEST
//	//}}
//
//
//	//////////////////////////////////////////////////////////////////////////	
//	// 위에서 결정된 드롭 아이템 리스트를 실제 유저에게 보내기 위한 처리
//	KEGS_DROP_ITEM_NOT kDropItemNot;
//	kDropItemNot.m_CreatePos = kPacket_.m_DiePos;
//
//	//{{ 2011. 08. 12   김민성      헤니르 개편 
//#ifdef SERV_NEW_HENIR_TEST
//	// 헤니르 던전이라면 드롭 가능한 아이템인지 확인 하자.
//	if( CXSLDungeon::IsHenirDungeon( GetDungeonID() ) == true )
//	{
//		// 보상 받을 수 있는 유저가 없다.
//		if( m_spRoomUserManager->GetHenirRewardUserNum() <= 0 )
//		{
//			CheckDeleteHenirDropItem( sDropDataNpc, sDropDataStatic, sDropDataEvent, sDropDataAttribNpc, sDropDataHenir );
//		}
//	}
//#endif SERV_NEW_HENIR_TEST
//	//}}
//
//	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상 드롭 아이템
//#ifdef SERV_COME_BACK_USER_REWARD
//	if( m_spRoomUserManager->IsComeBackUserInRoom() == true )
//	{
//		if( m_cRoomType == CXSLRoom::RT_DUNGEON )
//		{
//			//{{ 2011. 08. 12   김민성      헤니르 개편 
//#ifdef SERV_NEW_HENIR_TEST
//			if( CXSLDungeon::IsHenirDungeon( m_iDungeonID )  == true)
//			{
//				if( m_spRoomUserManager->GetHenirRewardUserNum() > 0 )
//				{
//					if( SiKDropTable()->GetComeBackBuffDropCount( m_spRoomUserManager->GetNumMember() ) == true )
//					{
//						// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//						kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260 ) );		// 새로운 모험의 징표
//					}
//				}
//			}
//			else
//#endif SERV_NEW_HENIR_TEST
//				//}}
//			{
//				if( SiKDropTable()->GetComeBackBuffDropCount( m_spRoomUserManager->GetNumMember() ) == true )
//				{
//					// 드롭 테이블을 확장해서 'Special Drop'기능으로 대체할예정
//					kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( 127260 ) );		// 새로운 모험의 징표
//				}
//			}
//		}
//	}
//#endif SERV_COME_BACK_USER_REWARD
//	//}} 
//
//	//1.ED 드롭처리
//	if( sDropDataNpc.m_iED != -1 )
//	{
//		for( int i = 0; i < sDropDataNpc.m_nGPNum; ++i )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( sDropDataNpc.m_iGPItemID, true, sDropDataNpc.m_iED ) );
//		}
//	}
//
//	//2.NPC ITEM 드롭처리	
//	if( sDropDataNpc.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataNpc.m_vecItemID )
//		{
//			// 특정 아이템이 특정던전에서 드롭되지 않도록 하는 코드
//			if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
//				continue;
//
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
//		}
//	}
//
//	//{{ 2009. 4. 22  최육사	전체드롭
//	if( sDropDataStatic.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataStatic.m_vecItemID )
//		{
//			// 특정 아이템이 특정던전에서 드롭되지 않도록 하는 코드
//			if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
//				continue;
//
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
//		}
//	}
//	//}}
//
//	//3.EVENT ITEM 드롭처리
//	if( /*mit->second.m_bNoDrop == false &&*/ kDieNpcInfo.m_bActive == true && sDropDataEvent.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataEvent.m_vecItemID )
//		{
//			// 특정 아이템이 특정던전에서 드롭되지 않도록 하는 코드
//			if( !CXSLDungeon::IsItemDropDungeon( iDropItemID, m_iDungeonID ) )
//				continue;
//
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
//		}
//	}
//
//	//4.ATTRIB NPC ITEM 드롭처리
//	if( /*mit->second.m_bNoDrop == false &&*/ kDieNpcInfo.m_bActive == true && sDropDataAttribNpc.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataAttribNpc.m_vecItemID )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
//		}
//	}
//
//	//{{ 2011. 08. 23	최육사	헤니르 시공 개편
//#ifdef SERV_NEW_HENIR_TEST
//	//5.HENIR NPC SPECIAL 드롭처리
//	if( /*mit->second.m_bNoDrop == false &&*/ sDropDataHenir.m_vecItemID.empty() == false )
//	{
//		BOOST_TEST_FOREACH( const int, iDropItemID, sDropDataHenir.m_vecItemID )
//		{
//			kDropItemNot.m_DropItemDataList.push_back( CreateItemProcess( iDropItemID ) );
//		}
//	}
//#endif SERV_NEW_HENIR_TEST
//	//}}
//
//	//드롭처리한 아이템이 있으면 패킷을 보낸다.
//	if( kDropItemNot.m_DropItemDataList.empty() == false )
//	{	
//		BroadCast( ERM_DROP_ITEM_NOT, kDropItemNot );
//	}
//}
#endif SERV_REFORM_ITEM_DROP
//}}
//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
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
	if( !m_spRoomUserManager->SetDie( FIRST_SENDER_UID, true ) )
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
	BroadCast( ERM_USER_UNIT_DIE_NOT, kPacketNot );

	//{{ 2008. 6. 17  최육사  실시간 부활석
	m_spRoomUserManager->ReserveEndPlay( FIRST_SENDER_UID );
	//}}

    if( !CheckIfPlayEnd() )
	{
		//나머지 다른 유저의 게임 진행을 위해 스테이지 체크를 한다.
		if( m_kDSSGoNextAllNot.m_iBeforeStageID != -1 &&
			m_kDSSGoNextAllNot.m_iNextStageID	!= -1 )
			SendDSSGoNextAllNot( m_kDSSGoNextAllNot );
	}
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
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	KEGS_GET_ITEM_NOT kNot;

	std::map< UidType, UidType > mapHenirRewardUserList;

	std::map< int, ITEM_DATA >::iterator mit;
	mit = m_mapDropItem.find( kPacket_.m_iDropItemUID );

	// 1. 습득할려는 item이 없는경우
	if( mit == m_mapDropItem.end() )
	{
		START_LOG( cerr, L"습득할려는 아이템이 없음" )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< END_LOG;

		// 절대로 일어날 수 없는 에러!
		if( kPacket_.m_iDropItemUID == -1  &&  kPacket_.m_kItemInfo.m_iItemID == -1 )
		{
			++m_iGetItemHackCount;

			if( m_iGetItemHackCount == m_iHackUserCheckCount )
			{
				// 파티장에게 해킹체크패킷을 보내자!
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_GET_ITEM;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

				START_LOG( cout, L"GetItem 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( m_iGetItemHackCount );
			}
		}


		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	ITEM_DATA& sItemData = mit->second;

	// 2. 아이템 종류 검사
	if( kPacket_.m_kItemInfo.m_iItemID != sItemData.m_iItemID )
	{
		START_LOG( cerr, L"습득할려는 아이템의 종류가 잘못되었음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( sItemData.m_iItemID )
			<< END_LOG;

		// 절대로 일어날 수 없는 에러!
		if( kPacket_.m_kItemInfo.m_iItemID == -1 )
		{
			++m_iGetItemHackCount;

			if( m_iGetItemHackCount == m_iHackUserCheckCount )
			{
				// 파티장에게 해킹체크패킷을 보내자!
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_GET_ITEM;

				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

				START_LOG( cout, L"GetItem 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( m_iGetItemHackCount );
			}
		}

		kPacket.m_iOK = NetError::ERR_ITEM_05;
		goto end_proc;
	}

	// 3. 다른 유저가 이미 습득한건가
	if( sItemData.m_bIsGet == true )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_01;
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

// 	const bool bIsGetItemDistribute = ( kPacket_.m_kItemInfo.m_iItemID == CXSLItem::HDR_TIMESPACE_PIECE );
// 	if( bIsGetItemDistribute == true )
// 	{
// 		m_spRoomUserManager->GetHenirRewardUserList( mapHenirRewardUserList );
// 	}
// 	else
	{
		// 4. 먹은 아이템이 ED인가? Item인가?
		if( sItemData.m_bIsGP == true )
		{
			kNot.m_iTotalED = sItemData.m_iED;
			kNot.m_GetUnitUID = FIRST_SENDER_UID; // 클라이언트 화면에 어떤놈이 ED먹었나 표시해줄때 사용
			const float fPartyBonusRate = GetDungeonPartyBonusRate();
			if( !m_spRoomUserManager->AddDungeonRewardED( sItemData.m_iED, FIRST_SENDER_UID, fPartyBonusRate, GetDungeonID(), GetDifficultyLevel(), kNot ) )

			{
				START_LOG( cerr, L"ED보상처리가 이상합니다!" )
					<< BUILD_LOG( sItemData.m_iED )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_ITEM_04;
				goto end_proc;
			}

			bNotCreateItem = true;
		}
		else		// 아이템
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

				// 순간소비 아이템이면 인벤에 넣지 않기위해
				if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
				{
					bNotCreateItem = true;
				}
			}
		}
	}

	sItemData.m_bIsGet	= true;	//오류가 없어야 아이템을 먹은것으로 체크

	// ack를 먼저 보낸다
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////	
	if( m_setFindedHackUserUnitUID.empty() == false )
	{
		if( m_spRoomUserManager->GetNumMember() == 1 )
		{
			// 핵유저가 맞는지 검사!
			if( m_setFindedHackUserUnitUID.find( FIRST_SENDER_UID ) != m_setFindedHackUserUnitUID.end() )
			{
				// 파티장에게 해킹체크패킷을 보내자!
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_KICK;

				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

#ifdef SERV_MINIMUM_LOG_HAKC_USER
				START_LOG( cout, L"Cant Kill Npc pos1. UnitUID : " << FIRST_SENDER_UID  );
#else //SERV_MINIMUM_LOG_HAKC_USER
				START_LOG( cout, L"Cant Kill Npc 해킹유저로 판단된 유저를 서버에서 접속종료시키기 위한 패킷 전송!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iDropItemUID )
					<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( m_setFindedHackUserUnitUID.size() );
#endif //SERV_MINIMUM_LOG_HAKC_USER
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////	

// 	if( bIsGetItemDistribute )
// 	{
// 		std::map< UidType, UidType>::iterator mit = mapHenirRewardUserList.begin();
// 		for( ; mit != mapHenirRewardUserList.end() ; ++mit )
// 		{
// 			kNot.m_GetUnitUID = mit->first;
// 
// 			// 유저들에게 아이템 획득을 알림
//			BroadCast( ERM_GET_ITEM_NOT, kNot );
//
//			// 실제로 아이템 획득 처리를 할 패킷을 획득 유저에게 전송
//			SendToGSCharacter( mit->second, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
//			//}}
// 		}
// 	}
// 	else

	{
		// 아이템 획득 결과 보내기
		if( bNotCreateItem == false )
		{
			// 실제로 획득한 유저에게 보냄

			// 유저들에게 아이템 획득을 알림
			BroadCast( ERM_GET_ITEM_NOT, kNot );

			// 실제로 아이템 획득 처리를 할 패킷을 획득 유저에게 전송
			SendToGSCharacter( iGSUID, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
			//}}


			//{{ 드롭 획득 공지 아이템 - 비던아이템, 매력아이템
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
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kNot.m_GetUnitUID );
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
			BroadCast( ERM_GET_ITEM_NOT, kNot );
		}
	}

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );	
}

#else		// SERV_REFORM_ITEM_DROP

_IMPL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 2, KRoomFSM::S_PLAY, KRoomFSM::S_RESULT ), ERM_GET_ITEM_ACK, KEGS_GET_ITEM_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	KEGS_GET_ITEM_NOT kNot;

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	std::map< UidType, UidType > mapHenirRewardUserList;
#endif SERV_NEW_HENIR_TEST
	//}}

	std::map< int, ITEM_DATA >::iterator mit;
	mit = m_mapDropItem.find( kPacket_.m_iDropItemUID );

	// 1. 습득할려는 item이 없는경우
	if( mit == m_mapDropItem.end() )
	{
		START_LOG( cerr, L"습득할려는 아이템이 없음" )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< END_LOG;

		//{{ 2010. 06. 17  최육사	오토핵 방지
#ifdef SERV_PROTECT_AUTO_HACK
		// 절대로 일어날 수 없는 에러!
		if( kPacket_.m_iDropItemUID == -1  &&  kPacket_.m_kItemInfo.m_iItemID == -1 )
		{
			++m_iGetItemHackCount;

			if( m_iGetItemHackCount == m_iHackUserCheckCount )
			{
				// 파티장에게 해킹체크패킷을 보내자!
				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_GET_ITEM;
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_GET_ITEM;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

				START_LOG( cout, L"GetItem 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( m_iGetItemHackCount );
			}
		}
#endif SERV_PROTECT_AUTO_HACK
		//}}

		kPacket.m_iOK = NetError::ERR_ITEM_00;
		goto end_proc;
	}

	ITEM_DATA& sItemData = mit->second;

	// 2. 아이템 종류 검사
	if( kPacket_.m_kItemInfo.m_iItemID != sItemData.m_iItemID )
	{
		START_LOG( cerr, L"습득할려는 아이템의 종류가 잘못되었음." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iDropItemUID )
			<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
			<< BUILD_LOG( sItemData.m_iItemID )
			<< END_LOG;

		//{{ 2010. 06. 17  최육사	오토핵 방지
#ifdef SERV_PROTECT_AUTO_HACK
		// 절대로 일어날 수 없는 에러!
		if( kPacket_.m_kItemInfo.m_iItemID == -1 )
		{
			++m_iGetItemHackCount;

			if( m_iGetItemHackCount == m_iHackUserCheckCount )
			{
				// 파티장에게 해킹체크패킷을 보내자!
				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_GET_ITEM;
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_GET_ITEM;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

				START_LOG( cout, L"GetItem 해킹유저로 판단되어 체크 결과를 클라이언트 전송하였음!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( m_iGetItemHackCount );
			}
		}
#endif SERV_PROTECT_AUTO_HACK
		//}}

		kPacket.m_iOK = NetError::ERR_ITEM_05;
		goto end_proc;
	}

	// 3. 다른 유저가 이미 습득한건가
	if( sItemData.m_bIsGet == true )
	{
		kPacket.m_iOK = NetError::ERR_ITEM_01;
		goto end_proc;
	}

	kNot.m_iDropItemUID	= kPacket_.m_iDropItemUID;
	kNot.m_iItemID		= kPacket_.m_kItemInfo.m_iItemID;

	// 순간소비 아이템
	bool bNotCreateItem = false;
	UidType iGSUID = LAST_SENDER_UID; // ServerUID저장

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	const bool bIsGetItemDistribute = ( kPacket_.m_kItemInfo.m_iItemID == CXSLItem::HDR_TIMESPACE_PIECE );
	if( bIsGetItemDistribute == true )
	{
		m_spRoomUserManager->GetHenirRewardUserList( mapHenirRewardUserList );
	}
	else
#endif SERV_NEW_HENIR_TEST
		//}}
	{
		// 4. 먹은 아이템이 ED인가? Item인가?
		if( sItemData.m_bIsGP == true )
		{
			kNot.m_iTotalED = sItemData.m_iED;
			kNot.m_GetUnitUID = FIRST_SENDER_UID; // 클라이언트 화면에 어떤놈이 ED먹었나 표시해줄때 사용

			//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
			if( !m_spRoomUserManager->AddDungeonRewardED( sItemData.m_iED, GetDungeonID(), kNot ) )
#else
			if( !m_spRoomUserManager->AddDungeonRewardED( sItemData.m_iED, kNot ) )
#endif SERV_NEW_HENIR_TEST
				//}}
			{
				START_LOG( cerr, L"ED보상처리가 이상합니다!" )
					<< BUILD_LOG( sItemData.m_iED )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_ITEM_04;
				goto end_proc;
			}

			bNotCreateItem = true;
		}
		else		// 아이템
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
						//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
						if( m_spRoomUserManager->GetItemWithRandom( sItemData.m_iItemID, GetDungeonID(), kNot.m_GetUnitUID, iGSUID ) == false )
#else
						if( m_spRoomUserManager->GetItemWithRandom( sItemData.m_iItemID,  kNot.m_GetUnitUID, iGSUID ) == false )
#endif SERV_NEW_HENIR_TEST
							//}}
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
	}

	sItemData.m_bIsGet	= true;	//오류가 없어야 아이템을 먹은것으로 체크

	// ack를 먼저 보낸다
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 7. 28  최육사	오토핵 차단 NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
	if( m_setFindedHackUserUnitUID.empty() == false )
	{
		if( m_spRoomUserManager->GetNumMember() == 1 )
		{
			// 핵유저가 맞는지 검사!
			if( m_setFindedHackUserUnitUID.find( FIRST_SENDER_UID ) != m_setFindedHackUserUnitUID.end() )
			{
				// 파티장에게 해킹체크패킷을 보내자!
				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_KICK;
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CANT_KILL_NPC_KICK;
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SERVER_CHECK_HACK_USER_NOT, kNot );

#ifdef SERV_MINIMUM_LOG_HAKC_USER
				START_LOG( cout, L"Cant Kill Npc pos2. UnitUID : " << FIRST_SENDER_UID  );
#else //SERV_MINIMUM_LOG_HAKC_USER
				START_LOG( cout, L"Cant Kill Npc 해킹유저로 판단된 유저를 서버에서 접속종료시키기 위한 패킷 전송!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iDropItemUID )
					<< BUILD_LOG( kPacket_.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( m_setFindedHackUserUnitUID.size() );
#endif //SERV_MINIMUM_LOG_HAKC_USER
			}
		}
	}
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	if( bIsGetItemDistribute )
	{
		std::map< UidType, UidType>::iterator mit = mapHenirRewardUserList.begin();
		for( ; mit != mapHenirRewardUserList.end() ; ++mit )
		{
			kNot.m_GetUnitUID = mit->first;

			// 유저들에게 아이템 획득을 알림
			BroadCast( ERM_GET_ITEM_NOT, kNot );

			// 실제로 아이템 획득 처리를 할 패킷을 획득 유저에게 전송
			SendToGSCharacter( mit->second, kNot.m_GetUnitUID, ERM_GET_ITEM_INSERT_TO_INVENTORY_NOT, kNot );
			//}}
		}
	}
	else
#endif SERV_NEW_HENIR_TEST
		//}}
	{
		// 아이템 획득 결과 보내기
		if( bNotCreateItem == false )
		{
			// 실제로 획득한 유저에게 보냄
			//{{ 2010. 03. 03  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
			//{{ 2010. 03. 18  최육사	아이템 획득 코드 개선

			// 유저들에게 아이템 획득을 알림
			BroadCast( ERM_GET_ITEM_NOT, kNot );

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
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( kNot.m_GetUnitUID );
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
			BroadCast( ERM_GET_ITEM_NOT, kNot );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 7. 3  최육사		헤니르 시공	
	//## des second(hoons) : 시간을 추가로 주는 아이템이 없어짐.
	//if( m_eDungeonType == CXSLDungeon::DT_HENIR )
	//{
	//	if( kPacket.m_iOK == NetError::NET_OK  &&  sItemData.m_iItemID == 70085 ) // 마법의 모래시계
	//	{
	//		SetRemainingPlayTime( m_fRemainingPlayTime + 300.f );
	//		BroadCast( ERM_REMAINING_PLAY_TIME_NOT, GetRemainingPlayTime() );
	//	}
	//}
	//}}
	//////////////////////////////////////////////////////////////////////////
	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_GET_ITEM_ACK, kPacket );	
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2009. 5. 11  최육사	실시간아이템
IMPL_ON_FUNC( ERM_GET_ITEM_COMPLETE_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY  &&
		GetStateID() != KRoomFSM::S_RESULT )
	{
		return;
	}

	std::map< int, int >::const_iterator mit;
	for( mit = kPacket_.m_mapGetItem.begin(); mit != kPacket_.m_mapGetItem.end(); ++mit )
	{
		if( !m_spRoomUserManager->AddItem( kPacket_.m_iUnitUID, mit->first ) )
		{
			START_LOG( cerr, L"아이템 획득 정보 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}
	}
}
//}}

IMPL_ON_FUNC( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ )
{
	//서버룸의 부모 room의 end_game 에서 wait으로 교체를 하기때문에
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_RESULT ), ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, KPacketOK );

	//{{ 2010. 05. 12  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket );
	}

	// RoomUserInfo의 변경정보를 날리자
	BroadCast( ERM_CHANGE_ROOM_USER_INFO_NOT, kPacket_.m_kRoomUserInfo );
#endif SERV_INTEGRATION
	//}}

	if( m_spRoomUserManager->SetDungeonUnitInfo( FIRST_SENDER_UID, kPacket_.m_kDungeonUnitInfo ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_01;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket );
	}

	kPacket.m_iOK       = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_MY_USER_UNIT_INFO_TO_SERVER_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	//===Result===
	CheckDungeonUnitInfoSuccess();
}

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

	if( !m_spRoomUserManager->SetDungeonPlayInfo( FIRST_SENDER_UID, kPacket_ ) )
	{
		START_LOG( cwarn, L"업데이트 실패했네?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
}
//}}

_IMPL_ON_FUNC( ERM_END_GAME_REQ, KEGS_END_GAME_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_END_GAME_ACK, KPacketOK );

	// 방장인지 체크한다.
	VERIFY_HOST( ERM_END_GAME_ACK );

	// 보낸 사람에게 ack를 날린다.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_END_GAME_ACK, kPacket );

	if( kPacket_.m_iWinTeam == CXSLRoom::TN_RED )
    {
        m_bIsWin = true;

		//////////////////////////////////////////////////////////////////////////
		//{{ 2009. 7. 3  최육사		헤니르 시공
		// 이벤트 던전을 클리어하면 죽었던 모든 유저를 다시 되살린다!
		if( m_eDungeonType == CXSLDungeon::DT_HENIR )
		{
			for( int iU = 0; iU < m_spRoomUserManager->GetNumMember(); ++iU )
			{
				KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iU );
				if( spRoomUser == NULL )
				{
					START_LOG( cerr, L"RoomUser가 존재하지 않음." )
						<< END_LOG;
					continue;
				}

				// 살아있는 유저는 패스~
				if( spRoomUser->IsDie() == false )
					continue;

				// 되살리자!
				spRoomUser->SetDie( false );

				KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT kPacketNot;
				kPacketNot.m_iUnitUID = spRoomUser->GetCID();
				kPacketNot.m_iUsedByUnitUID = spRoomUser->GetCID();
				kPacketNot.m_iStartPosIndex = SiCXSLDungeonManager()->GetStartPosByRandom( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), m_iStageID, m_iSubStageID );
				BroadCast( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacketNot );

				//{{ 2008. 6. 17  최육사  실시간 부활석
				m_spRoomUserManager->SetEndPlay( spRoomUser->GetCID(), false );
				//}}
			}
		}
		//}}

		//{{ 2012. 02. 28	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//		IncreasePartyFever();
//#else
//		if( m_iPartyUID > 0  &&  
//			m_spRoomUserManager->GetNumMember() > 1 )
//		{
//			KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( 0 ); // 0번 인덱스의 유저
//			if( spRoomUser != NULL )
//			{
//				KERM_INCREASE_PARTY_FEVER_NOT kNot;
//				kNot.m_iPartyUID = m_iPartyUID;
//				SendToGSServer( spRoomUser->GetGSUID(), ERM_INCREASE_PARTY_FEVER_NOT, kNot );
//			}
//			else
//			{
//				START_LOG( cerr, L"유저가 한명도 없는방인감?" )
//					<< BUILD_LOG( GetStateIDString() )
//					<< END_LOG;
//			}
//		}
//#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//////////////////////////////////////////////////////////////////////////
    }

	// 게임을 끝낸다.
	EndPlay();
}

_IMPL_ON_FUNC( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_PLAY ) );

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	kPacket_.m_bUseItemNeedPayment = false;

	for ( int iR = 0; iR < m_spRoomUserManager->GetNumMember(); ++iR )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser(iR);
		// 유닛 UID 가져와서 체크 후, 동일 유닛에게만 셋 시켜 주십ㄴ.다
		if( spRoomUser->GetCID() == kPacket_.m_UnitUID )
		{
			// 우선 사용한 아이템이 올바른 아이템인가 체크
			if(SiKDropTable()->CheckPaymentUseItem(kPacket_.m_ItemID) == true)
			{
				//spRoomUser->SetUseItemForPayment( true );
				kPacket_.m_bUseItemNeedPayment = true;
				START_LOG(clog, L"지헌로그-테스트 : 아이템 사용 체크 완료!")
					<< BUILD_LOG(kPacket_.m_bUseItemNeedPayment)
					<< END_LOG;
				break;
			}

		}
	}
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

    BroadCast( ERM_USE_QUICK_SLOT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<KRoomUserManager::KLeaveUserInfo>	vecDelUserUID;
	int iCount = m_spRoomUserManager->DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_spRoomUserManager->GetNumMember() );

	//현재방에서 삭제된 유저가 없으면 종료.
	if( iCount == 0 )
		return;

	for( int i = 0; i < (int)vecDelUserUID.size(); i++ )
	{
		m_spRoomUserManager->LeaveRoom( vecDelUserUID[i].m_iUnitUID );
	}

	for( int i = 0; i < (int)vecDelUserUID.size(); i++ )
	{
		if( IsEmpty() == false )
		{
			// 방의 모든 유저들에게 슬롯 정보를 날린다.
			KEGS_LEAVE_ROOM_NOT kPacketNot;
			kPacketNot.m_cRoomState		= GetStateID();
			kPacketNot.m_iUnitUID		= vecDelUserUID[i].m_iUnitUID;

			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
		}
	}

	if( (GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY) && IsEmpty() == false )
	{
		CheckLoadingSuccess();
	}

	// 스테이지가 끝나게 되는 경우 처리.
	if( GetStateID() == KRoomFSM::S_PLAY && IsEmpty() == false )
	{
		if( CheckIfPlayEnd() )
		{
			EndPlay();
		}
		else
		{
			//{{ 2009. 7. 27  최육사	비밀스테이지
			if( m_eDungeonType == CXSLDungeon::DT_HENIR )
			{
				CheckEnterSecretStageSuccess();
			}
			//}}
		}
	}

	//결과창을 보고있을때 방을 나갈경우처리
	if( GetStateID() == KRoomFSM::S_RESULT && IsEmpty() == false )
	{
		CheckResultSuccess();
	}

	// 방에서 모든 유저가 나갔으면 방을 닫는다.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );

		//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
		CTime kEndGameTime = CTime::GetCurrentTime();
		//////////////////////////////////////////////////////////////////////////
		KE_LOCAL_LOG_DUNGEON_ROOM_NOT kNot;
		kNot.m_iDungeonID		= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
		kNot.m_iDifficultyLevel	= (int) GetDifficultyLevel();
		kNot.m_iIsChallenge		= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
		kNot.m_iWin				= 0;
		kNot.m_iLose			= 0;
		kNot.m_iStartNumMember	= m_iStartedNumMember;
		kNot.m_iEndNumMember	= GetNumMember();
		kNot.m_iTotalPlayTime	= static_cast<int>(GetTotalPlayTime());
		kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_ROOM_NOT, kNot );
		//////////////////////////////////////////////////////////////////////////
#endif SERV_STATISTICS_THREAD
		//}}
	}
	else
	{
		//던전에 남아있는 유저들의 다음스테이지 이동을 시키기 위해
		if( m_kDSSGoNextAllNot.m_iBeforeStageID != -1 &&
			m_kDSSGoNextAllNot.m_iNextStageID	!= -1 )
			SendDSSGoNextAllNot( m_kDSSGoNextAllNot );
	}
}

_IMPL_ON_FUNC( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );

	bool bDie;
	if( !m_spRoomUserManager->IsDie( kPacket_.m_iUnitUID, bDie ) )
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

	if( !m_spRoomUserManager->SetDie( kPacket_.m_iUnitUID, false ) )
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

	KRoomUserPtr roomSlotPtr = m_spRoomUserManager->GetUser( kPacketNot.m_iUnitUID );
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
	kPacketNot.m_iStartPosIndex = SiCXSLDungeonManager()->GetStartPosByRandom( (m_iDungeonID + static_cast<int>(m_cDifficultyLevel)), m_iStageID, m_iSubStageID );
	BroadCast( EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT, kPacketNot );

	//{{ 2008. 6. 17  최육사  실시간 부활석
	m_spRoomUserManager->SetEndPlay( kPacket_.m_iUnitUID, false );
	//}}
}

IMPL_ON_FUNC_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT )
{
	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cwarn, L"패킷이 허용되는 방 상태가 아님." )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		return;
	}

	//{{ 2008. 6. 17  최육사  실시간 부활석
	if( m_spRoomUserManager->CheckEndPlayAnotherMember( FIRST_SENDER_UID ) )
		EndPlay();
	//}}
}

IMPL_ON_FUNC( ERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT )
{
	if( GetStateID() != KRoomFSM::S_WAIT )
	{
		START_LOG( cwarn, L"패킷이 허용되는 방 상태가 아님." )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		//return;
	}

	m_spRoomUserManager->SetTutorialUIDList( FIRST_SENDER_UID, kPacket_.m_vecStudentUnitUID );
}

//{{ 2008. 6. 16  최육사  실시간 부활석
_IMPL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, KEGS_STOP_DUNGEON_CONTINUE_TIME_ACK );

	// 컨티뉴 시간 멈추기 or 풀기
	m_spRoomUserManager->StopDungeonContinueTime( FIRST_SENDER_UID, kPacket_.m_bIsStop );

	kPacket.m_iOK	  = NetError::NET_OK;
	kPacket.m_bIsStop = kPacket_.m_bIsStop;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_STOP_DUNGEON_CONTINUE_TIME_ACK, kPacket );
}
//}}

//{{ 2009. 2. 4  최육사	던전 리플레이방 싱크
_IMPL_ON_FUNC( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT )
{
	if( GetStateID() != KRoomFSM::S_WAIT )
	{
		START_LOG( cwarn, L"패킷이 허용되는 방 상태가 아님." )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	KEGS_FIELD_UNIT_SYNC_DATA_BROAD_NOT kPacketNot;
	kPacketNot.m_UnitUID		= FIRST_SENDER_UID;
	kPacketNot.m_vecFieldFrame	= kPacket_.m_vecFieldFrame;
	kPacketNot.m_vPos			= kPacket_.m_vPos;
	kPacketNot.m_bIsRight		= kPacket_.m_bIsRight;	
	BroadCast( ERM_FIELD_UNIT_SYNC_DATA_BROAD_NOT, kPacketNot );
}
//}}

//{{ 2009. 3. 18  최육사	파티유저밴
IMPL_ON_FUNC_NOPARAM( ERM_PARTY_BAN_USER_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_PARTY_BAN_USER_ACK, KERM_PARTY_BAN_USER_ACK );

	// 방에 있는 유저인가?
	KRoomUserPtr pkRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );	
	if( pkRoomUser == NULL )
	{
		START_LOG( cerr, L"강퇴시킬 룸유져 찾기실패" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SLOT_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PARTY_BAN_USER_ACK, kPacket );
		return;
	}

	// 방 슬롯 정보 변경.
	if( !m_spRoomUserManager->LeaveRoom( FIRST_SENDER_UID ) )
	{
		START_LOG( cerr, L"방 이탈 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 강퇴 실패.
		kPacket.m_iOK = NetError::ERR_SLOT_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PARTY_BAN_USER_ACK, kPacket );
		return;
	}

	// 강퇴 대상자에게 not를 날린다.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_PARTY_BAN_USER_ACK, kPacket );

	//만일의 경우 강퇴로 인한 모든 유저가 나갈시
	if( IsEmpty() )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );

		//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
		CTime kEndGameTime = CTime::GetCurrentTime();
		//////////////////////////////////////////////////////////////////////////
		KE_LOCAL_LOG_DUNGEON_ROOM_NOT kNot;
		kNot.m_iDungeonID		= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
		kNot.m_iDifficultyLevel	= (int) GetDifficultyLevel();
		kNot.m_iIsChallenge		= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
		kNot.m_iWin				= 0;
		kNot.m_iLose			= 0;
		kNot.m_iStartNumMember	= m_iStartedNumMember;
		kNot.m_iEndNumMember	= GetNumMember();
		kNot.m_iTotalPlayTime	= static_cast<int>(GetTotalPlayTime());
		kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_ROOM_NOT, kNot );
		//////////////////////////////////////////////////////////////////////////
#endif SERV_STATISTICS_THREAD
		//}}
	}
}
//}}

//{{ 2011. 10. 25	최육사	배틀필드 시스템
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( ERM_DUNGEON_KILLALLNPC_CHECK_REQ )
{
#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES// 작업날짜: 2013-05-22	// 박세훈
	if( IsHost( FIRST_SENDER_UID ) == false )
	{
		return;
	}
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES

	if( GetStateID() != KRoomFSM::S_PLAY )
	{
		START_LOG( cerr, L"패킷이 허용되는 방 상태가 아님." )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	KEGS_DUNGEON_KILLALLNPC_CHECK_ACK kPacket;
	kPacket.m_bResult = true;

	//{{ 2010. 12. 28	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
	if( CXSLDungeon::IsShipDungeon( GetDungeonID() ) == true )
	{
		START_LOG( clog, L"마을 이동 던전이면 그냥 성공 처리 한다!" )
			<< BUILD_LOG( GetDungeonID() );

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket );
		return;
	}
#endif SERV_SHIP_DUNGEON
	//}}
	//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	if( CXSLDungeon::IsValentineDungeon( GetDungeonID() ) == true )
	{
		START_LOG( clog, L"이벤트 발렌타인 던전이면 그냥 성공 처리 한다!" )
			<< BUILD_LOG( GetDungeonID() );

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket );
		return;
	}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	if( CXSLDungeon::IsValentineDungeonInt( GetDungeonID() ) == true )
	{
		START_LOG( clog, L"엘소드 해외 이벤트 발렌타인 던전이면 그냥 성공 처리 한다!" )
			<< BUILD_LOG( GetDungeonID() );

		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket );
	}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}

	// 해당 서브 스테이지에 모든 몬스터가 죽었는지 체크하자!
	if( m_kDungeonMonsterManager.IsAllSubStageNpcDie( GetStageID(), GetSubStageID() ) == false )
	{
		START_LOG( cerr, L"Npc가 없는 스테이지인데 모든NPC 죽여달라고 요청패킷이 왔네? 해킹인가?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetStageID() )
			<< BUILD_LOG( GetSubStageID() )
			<< END_LOG;

		kPacket.m_bResult = false;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket );
		return;
	}

	// 클라이언트로 결과 전송
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket );

#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES// 작업날짜: 2013-05-22	// 박세훈
	// 혹시나 모르니 결과 체크 하자
	if( kPacket.m_bResult == true )
	{
		BroadCastID( ERM_DUNGEON_KILLALLNPC_CHECK_NOT );
	}
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

//IMPL_ON_FUNC_NOPARAM( ERM_DUNGEON_KILLALLNPC_CHECK_REQ )
//{
//	if( GetStateID() != KRoomFSM::S_PLAY )
//	{
//		START_LOG( cerr, L"패킷이 허용되는 방 상태가 아님." )
//			<< BUILD_LOG( GetStateIDString() )
//			<< END_LOG;
//		return;
//	}
//
//	KEGS_DUNGEON_KILLALLNPC_CHECK_ACK kPacket;
//	kPacket.m_bResult = true;
//
//	// 모든 NPC를 죽여달라는 요청이 들어온 스테이지가 보스가 있는 서브 스테이지인지 확인하자!
//	KSubStageKey kSubStageKey( m_iStageID, m_iSubStageID );
//
//	std::map< KSubStageKey, SUB_STAGE_NPC_INFO >::const_iterator mit;
//	mit = m_mapSubStageNpcInfo.find( kSubStageKey );
//	if( mit == m_mapSubStageNpcInfo.end() )
//	{
//		START_LOG( cerr, L"Npc가 없는 스테이지인데 모든NPC 죽여달라고 요청패킷이 왔네? 해킹인가?" )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			<< BUILD_LOG( m_iStageID )
//			<< BUILD_LOG( m_iSubStageID )
//			<< END_LOG;
//
//		kPacket.m_bResult = false;
//		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket );
//		return;
//	}
//
//	const SUB_STAGE_NPC_INFO& sSubStageNpcInfo = mit->second;
//
//	// 보스 스테이지 인지 일반 스테이지 인지 확인
//	if( sSubStageNpcInfo.m_setBossNpcUID.empty() )
//	{
//		// 해당 서브 스테이지에 모든 일반 몬스터가 죽었는지 확인!
//		std::set< int >::const_iterator sit;
//		for( sit = sSubStageNpcInfo.m_setNormalNpcUID.begin(); sit != sSubStageNpcInfo.m_setNormalNpcUID.end(); ++sit )
//		{
//			const int iNpcUID = *sit;
//			std::map< int, NPC_DATA >::const_iterator mitND;
//			mitND = m_mapNPCData.find( iNpcUID );
//			if( mitND == m_mapNPCData.end() )
//			{
//				START_LOG( cerr, L"일반 npc 리스트에 있던 NpcUID로 검사했더니 NPC_DATA는 없네?" )
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( m_iStageID )
//					<< BUILD_LOG( m_iSubStageID )
//					<< BUILD_LOG( iNpcUID )
//					<< END_LOG;
//				continue;
//			}
//
//			//{{ 2010. 12. 28	최육사	하멜 마을 추가
//#ifdef SERV_SHIP_DUNGEON
//			if( CXSLDungeon::IsShipDungeon( GetDungeonID() ) == true )
//			{
//				START_LOG( clog, L"마을 이동 던전이면 그냥 성공 처리 한다!" )
//					<< BUILD_LOG( GetDungeonID() );
//				break;
//			}
//#endif SERV_SHIP_DUNGEON
//			//}}
//
//			// npc가 죽지 않았다면?
//			if( mitND->second.m_bDie == false )
//			{
//				START_LOG( cerr, L"모든 npc가 죽지도 않았는데 모든 몬스터 죽여달라고 패킷이 왔네.. 해킹인가?" )
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( m_iStageID )
//					<< BUILD_LOG( m_iSubStageID )
//					<< BUILD_LOG( iNpcUID )
//					<< END_LOG;
//
//				kPacket.m_bResult = false;
//				break;
//			}
//		}
//	}
//	else
//	{
//		// 해당 서브 스테이지에 모든 보스 몬스터가 죽었는지 확인!
//		std::set< int >::const_iterator sit;
//		for( sit = sSubStageNpcInfo.m_setBossNpcUID.begin(); sit != sSubStageNpcInfo.m_setBossNpcUID.end(); ++sit )
//		{
//			const int iNpcUID = *sit;
//			std::map< int, NPC_DATA >::const_iterator mitND;
//			mitND = m_mapNPCData.find( iNpcUID );
//			if( mitND == m_mapNPCData.end() )
//			{
//				START_LOG( cerr, L"보스 리스트에 있던 NpcUID로 검사했더니 NPC_DATA는 없네?" )
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( m_iStageID )
//					<< BUILD_LOG( m_iSubStageID )
//					<< BUILD_LOG( iNpcUID )
//					<< END_LOG;
//				continue;
//			}
//
//			//{{ 2010. 12. 28	최육사	하멜 마을 추가
//#ifdef SERV_SHIP_DUNGEON
//			if( CXSLDungeon::IsShipDungeon( GetDungeonID() ) == true )
//			{
//				START_LOG( clog, L"마을 이동 던전이면 그냥 성공 처리 한다!" )
//					<< BUILD_LOG( GetDungeonID() );
//				break;
//			}
//#endif SERV_SHIP_DUNGEON
//			//}}
//
//			// 보스가 죽지 않았다면?
//			if( mitND->second.m_bDie == false )
//			{
//				START_LOG( cerr, L"보스가 죽지도 않았는데 모든 몬스터 죽여달라고 패킷이 왔네.. 해킹인가?" )
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( m_iStageID )
//					<< BUILD_LOG( m_iSubStageID )
//					<< BUILD_LOG( iNpcUID )
//					<< END_LOG;
//
//				kPacket.m_bResult = false;
//				break;
//			}
//		}
//	}
//
//	// 클라이언트로 결과 전송
//	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket );
//}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

//{{ 2010. 11. 18	최육사	아이템 판매 차단
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM
_IMPL_ON_FUNC( ERM_CHECK_SELL_ED_ITEM_REQ, KEGS_SELL_ED_ITEM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_WAIT ), ERM_CHECK_SELL_ED_ITEM_ACK, KERM_CHECK_SELL_ED_ITEM_ACK );

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iItemUID = kPacket_.m_iItemUID;
	kPacket.m_iQuantity = kPacket_.m_iQuantity;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_SELL_ED_ITEM_ACK, kPacket );
}
#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
//}}

#ifdef SERV_CREATE_ELITE_FOR_ADMIN
_IMPL_ON_FUNC( ERM_ADMIN_NPC_UNIT_CREATE_REQ, KEGS_ADMIN_NPC_UNIT_CREATE_REQ )
{
	VERIFY_ROOM_STATE_AND_WARN( ( 1, KRoomFSM::S_PLAY ), (1, KRoomFSM::S_RESULT), ERM_NPC_UNIT_CREATE_ACK, KPacketOK );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_ADMIN_NPC_UNIT_CREATE_ACK, kPacket );

	KEGS_ADMIN_NPC_UNIT_CREATE_NOT kPacketNot;
	kPacketNot.m_kNPCUnitNot = kPacket_.m_kNPCUnitReq;
	kPacketNot.m_cAttrib1 = kPacket_.m_cAttrib1;
	kPacketNot.m_cAttrib2 = kPacket_.m_cAttrib2;
	kPacketNot.m_cSA1 = kPacket_.m_cSA1;
	kPacketNot.m_cSA2 = kPacket_.m_cSA2;
	
// #ifdef SERV_CREATED_NPC_LIMITED_DROPS
// 	// 일단은 무조건 노드랍 처리 // 일단 저장해둠
// 	bool bTempNoDrop = kPacket_.m_kNPCUnitReq.m_bNoDrop;
// 	kPacket_.m_kNPCUnitReq.m_bNoDrop = true;
// 
// 	int iNpcDropTime = SiCXSLDungeonManager()->GetNPCDropTime( GetDungeonIDAndDif(), GetStageID(), GetSubStageID(), kPacket_.m_kNPCUnitReq.m_NPCID );
// 	if( iNpcDropTime > 0 )
// 	{
// 		NPC_INFO_QUADRUPLE npcInfoQuadruple;
// 		npcInfoQuadruple.iDungeonID		= GetDungeonIDAndDif();
// 		npcInfoQuadruple.iStageID		= GetStageID();
// 		npcInfoQuadruple.iSubStageID	= GetSubStageID();
// 		npcInfoQuadruple.iNpcID			= kPacket_.m_kNPCUnitReq.m_NPCID;
// 
// 		// 소환요청되었던 수가 전체 드랍가능한 수 미만일 때만 드랍 가능
// 		if( m_mapDroppedTimes[npcInfoQuadruple] < iNpcDropTime )
// 		{
// 			m_mapDroppedTimes[npcInfoQuadruple] = m_mapDroppedTimes[npcInfoQuadruple] + 1;
// 			kPacket_.m_kNPCUnitReq.m_bNoDrop = bTempNoDrop;
// 		}
// 	}
//#endif SERV_CREATED_NPC_LIMITED_DROPS
    
	// 몬스터 생성 함수 호출!
	LIF( m_kDungeonMonsterManager.CreateMonster( kPacket_.m_kNPCUnitReq, kPacketNot.m_kNPCUnitNot.m_UID ) );	

	// 속성 정보는 원래대로 
	if( kPacket_.m_cAttrib1 != 0 || kPacket_.m_cAttrib2 != 0 || kPacket_.m_cSA1 != 0 || kPacket_.m_cSA2 != 0 )
	{
		m_mapAttribNpcData.insert( std::make_pair( kPacketNot.m_kNPCUnitNot.m_UID, static_cast<int>(kPacketNot.m_kNPCUnitNot.m_Level) ) );
	}

	BroadCast( ERM_ADMIN_NPC_UNIT_CREATE_NOT, kPacketNot );
}
#endif SERV_CREATE_ELITE_FOR_ADMIN

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

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
int KDungeonRoom::GetProtectedSmallCrystalCount()
{	
	const int iCount = m_setDefenseSmallCrystalNpcUID.size();
	return ( m_bIsWin == true ) ? iCount : 0;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
void KDungeonRoom::CheckDeleteHenirDropItem( IN OUT KDropTable::DROP_DATA& sDropDataNpc, 
											 IN OUT KDropTable::DROP_DATA& sDropDataStatic, 
											 IN OUT KDropTable::DROP_DATA& sDropDataEvent, 
											 IN OUT KDropTable::DROP_DATA& sDropDataAttribNpc,
											 IN OUT KDropTable::DROP_DATA& sDropDataHenir )
{
	std::vector< int > vecItemID;
	std::vector< int >::iterator vit;
	
	// 1. sDropDataNpc 검사
	vecItemID = sDropDataNpc.m_vecItemID;
	sDropDataNpc.Clear();

	for( vit = vecItemID.begin(); vit != vecItemID.end() ; ++vit )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( *vit );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"sDropDataNpc 검사 아이템 정보가 없다." )
				<< BUILD_LOG( *vit );
			continue;
		}

		if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
		{
			sDropDataNpc.m_vecItemID.push_back( *vit );
		}
	}

	// 2. sDropDataStatic 검사
	vecItemID = sDropDataStatic.m_vecItemID;
	sDropDataStatic.Clear();

	for( vit = vecItemID.begin(); vit != vecItemID.end() ; ++vit )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( *vit );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"sDropDataStatic 검사 아이템 정보가 없다." )
				<< BUILD_LOG( *vit );
			continue;
		}

		if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
		{
			sDropDataStatic.m_vecItemID.push_back( *vit );
		}
	}

	// 3. sDropDataEvent 검사
	vecItemID = sDropDataEvent.m_vecItemID;
	sDropDataEvent.Clear();

	for( vit = vecItemID.begin(); vit != vecItemID.end() ; ++vit )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( *vit );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"sDropDataEvent 검사 아이템 정보가 없다." )
				<< BUILD_LOG( *vit );
			continue;
		}

		if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
		{
			sDropDataEvent.m_vecItemID.push_back( *vit );
		}
	}

	// 4. sDropDataAttribNpc 검사
	vecItemID = sDropDataAttribNpc.m_vecItemID;
	sDropDataAttribNpc.Clear();

	for( vit = vecItemID.begin(); vit != vecItemID.end() ; ++vit )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( *vit );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"sDropDataAttribNpc 검사 아이템 정보가 없다." )
				<< BUILD_LOG( *vit );
			continue;
		}

		if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
		{
			sDropDataAttribNpc.m_vecItemID.push_back( *vit );
		}
	}

	// 5. sDropDataAttribNpc 검사
	vecItemID = sDropDataHenir.m_vecItemID;
	sDropDataHenir.Clear();

	for( vit = vecItemID.begin(); vit != vecItemID.end() ; ++vit )
	{
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( *vit );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"sDropDataAttribNpc 검사 아이템 정보가 없다." )
				<< BUILD_LOG( *vit );
			continue;
		}

		if( pItemTemplet->m_ItemType == CXSLItem::IT_OUTLAY )
		{
			sDropDataHenir.m_vecItemID.push_back( *vit );
		}
	}
}
#endif SERV_NEW_HENIR_TEST
//}}

//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KDungeonRoom::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	KRoom::OnLeaveRoom( iLAST_SENDER_UID, iFIRST_SENDER_UID, kPacket_, usEventIDAck );

	//{{ 2013. 02. 25   던전 이탈 시 랭크 기록 - 김민성
#ifdef SERV_LEAVE_ROOM_DUNGEON_RANK
	const int iEndNumMember = m_spRoomUserManager->GetNumMember();
	int iPartyTotalGivenDamage = 0;
	for( int idx = 0; idx < m_spRoomUserManager->GetNumMember(); ++idx )
	{
		KRoomUserPtr pkRoomUser  = m_spRoomUserManager->GetUser( idx );
		_JIF( pkRoomUser != NULL, continue; );

		iPartyTotalGivenDamage	+= pkRoomUser->GetTotalGivenDamamge_LUA();
	}
#endif SERV_LEAVE_ROOM_DUNGEON_RANK
	//}

	
	// 이탈자 기록.
	if( GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY )
	{

		//=== 게임중 이탈 결과처리 ===
		START_LOG( clog, L"=== DUNGEON GAME : 게임중 이탈시 결과처리 ===" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;

		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iFIRST_SENDER_UID );

		if( !spRoomUser )
		{
			spRoomUser = m_spRoomUserManager->GetUser( iFIRST_SENDER_UID, KRoomUserManager::UT_OBSERVER );
		}

		if( !spRoomUser )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상" )
				<< BUILD_LOG( iFIRST_SENDER_UID )
				<< END_LOG;
		}
		else
		{
			if( spRoomUser->IsObserver() == false )
			{
				//튜토리얼 모드이면 패널티가 없음..
				int iEXP = 0;
				int iED = 0;
				if( CXSLDungeon::IsTutorialDungeon( m_iDungeonID ) == false )
				{
					KERM_UPDATE_DUNGEON_UNIT_INFO_NOT kPacket;
					kPacket.m_bOutRoom = true;
					kPacket.m_iUserCount = GetNumMember();
					kPacket.m_iDungeonID = m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
					kPacket.m_iPlayTime = ( int )( m_kTimer[TM_PLAY].elapsed() + 0.5 - GetLoadingTime() );
					kPacket.m_iEXP = 0;

					if( spRoomUser->IsDie() == true )
					{
						// 실시간 경험치
						m_spRoomUserManager->GetRewardEXP( iFIRST_SENDER_UID, iEXP );
						m_spRoomUserManager->GetRewardPartyEXP( iFIRST_SENDER_UID, iEXP );

						//개인별 아이템 정보주기
						m_spRoomUserManager->GetItemList( iFIRST_SENDER_UID, kPacket.m_mapGetItem );
					}
					else
					{
						// ED페널티
						SiKResultProcess()->Result_DUNGEON_BREAK( spRoomUser, kPacket );
					}

					// [중요] 클라이언트 크래쉬 및 해킹으로 인한 방 이탈일 경우 게임서버로 보상패킷을 보내지 않습니다!
					if( kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_23 )
					{
						SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_UPDATE_DUNGEON_UNIT_INFO_NOT, kPacket );

						//{{ 2009. 5. 14  최육사	내구도 개편
						KERM_DECREASE_ENDURANCE_DUNGEON_NOT kNot;
						kNot.m_iEnduranceDamage = CXSLDungeon::DE_LEAVE_ROOM_ENDURANCE_DAMAGE;
						kNot.m_cPartyNumMember  = kPacket.m_iUserCount;
						SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_DECREASE_ENDURANCE_DUNGEON_NOT, kNot );
						//}}
					}

					iED = kPacket.m_iED;
				}

				//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
				int iClearValue = ( spRoomUser->IsDie() ? KE_LOCAL_LOG_DUNGEON_NOT::DCT_UNIT_DIE_LEAVE_ROOM : KE_LOCAL_LOG_DUNGEON_NOT::DCT_UNIT_ALIVE_LEAVE_ROOM ); // 2. 캐릭터 죽고 이탈   3. 캐릭터 안죽고 이탈
				
				if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_23 ) // 4. 클라이언트 크래쉬 이탈
				{
					iClearValue = KE_LOCAL_LOG_DUNGEON_NOT::DCT_CLIENT_CRASH_LEAVE_ROOM;
				}
				else if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_25 ) // 5. 클라이언트 해킹으로 인한 이탈
				{
					iClearValue = KE_LOCAL_LOG_DUNGEON_NOT::DCT_CLIENT_HACKING_LEAVE_ROOM;
				}

				// 던전 로그
				KERM_UPDATE_DUNGEON_UNIT_INFO_NOT kUpdateUnitInfoDummy;
				KDungeonUnitResultInfo kDungeonUnitInfoDummy;

				//{{ 2013. 02. 25   던전 이탈 시 랭크 기록 - 김민성
#ifdef SERV_LEAVE_ROOM_DUNGEON_RANK
				SiKResultProcess()->LeaveRoomResultRank( m_iDungeonID, m_cDifficultyLevel, spRoomUser, m_kDungeonMonsterManager, iEndNumMember, iPartyTotalGivenDamage, (int)GetTotalPlayTime(), kUpdateUnitInfoDummy, kDungeonUnitInfoDummy );
#endif SERV_LEAVE_ROOM_DUNGEON_RANK
				//}

				//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD

#ifdef SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE// 작업날짜: 2013-09-04	// 박세훈
				// 이탈 시 EndPlay처리가 이 구문 뒤에 오기 때문에 TotalPlayTime이 정상적으로 남지 않고 있다.
				// EndPlay와 이 구문의 처리 순서에 의미가 있을 것 같아서 이와 같이 처리하였다.
				ComputeTotalPlayTime();
#endif // SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE

				LogToDB_DungeonLog( spRoomUser, iClearValue, kUpdateUnitInfoDummy, kDungeonUnitInfoDummy, kPacket_.m_iReason );
#else
				LogToDB_DungeonLog( spRoomUser, iClearValue, kUpdateUnitInfoDummy, kDungeonUnitInfoDummy );
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
				//}}
#else
//				//////////////////////////////////////////////////////////////////////////
//				// 던전 일일 이탈 로그
//				// 운영자는 통계를 남기지 않습니다.
//				//if( spRoomUser->GetAuthLevel() >= SEnum::UAL_GM )
//				//{
//				//	START_LOG( cout, L"운영자는 통계데이터를 남기지 않습니다." )
//				//		<< BUILD_LOGc( spRoomUser->GetAuthLevel() )
//				//		<< BUILD_LOG( spRoomUser->GetNickName() );
//				//}
//				//else
//				{
//					KRoomUserInfo kInfo;
//					spRoomUser->GetRoomUserInfo( kInfo );
//
//					int iClear = ( spRoomUser->IsDie() ? KE_LOCAL_LOG_DUNGEON_NOT::DCT_UNIT_DIE_LEAVE_ROOM : KE_LOCAL_LOG_DUNGEON_NOT::DCT_UNIT_ALIVE_LEAVE_ROOM ); // 2. 캐릭터 죽고 이탈   3. 캐릭터 안죽고 이탈
//
//					if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_23 ) // 4. 클라이언트 크래쉬 이탈
//					{
//						iClear = KE_LOCAL_LOG_DUNGEON_NOT::DCT_CLIENT_CRASH_LEAVE_ROOM;
//					}
//					else if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_25 ) // 5. 클라이언트 해킹으로 인한 이탈
//					{
//						iClear = KE_LOCAL_LOG_DUNGEON_NOT::DCT_CLIENT_HACKING_LEAVE_ROOM;
//					}
//
//					//{{ 2010. 06. 21  최육사	통계 스레드
//					//#ifdef SERV_STATISTICS_THREAD
//					//{{ 2010. 11. 8	최육사	던전 로그 DB
//					//#ifdef SERV_DUNGEON_LOG_DB
//					CTime kRegDate = CTime::GetCurrentTime();
//					//#endif SERV_DUNGEON_LOG_DB
//					//}}
//					//////////////////////////////////////////////////////////////////////////					
//					KE_LOCAL_LOG_DUNGEON_NOT kNot;
//					kNot.m_iDungeonID				= (m_iDungeonID + static_cast<int>(m_cDifficultyLevel));
//					kNot.m_nUnitUID					= kInfo.m_nUnitUID;
//					kNot.m_wstrNickName				= kInfo.m_wstrNickName;
//					kNot.m_iDifficultyLevel			= (int) m_cDifficultyLevel;
//					kNot.m_iIsChallenge				= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
//					kNot.m_iUnitClass				= (int) kInfo.m_cUnitClass;
//					kNot.m_iClear					= iClear;
//					kNot.m_iStartNumMember			= m_iStartedNumMember;
//					kNot.m_iEndNumMember			= GetNumMember();
//					kNot.m_ucLevel					= kInfo.m_ucLevel;
//					kNot.m_iPlayTime				= (int) spRoomUser->GetPlayTime();
//					kNot.m_iEXP						= iEXP;
//					kNot.m_iED						= iED;
//					kNot.m_cComboRank				= CXSLDungeon::RT_F;
//					kNot.m_iComboScore				= spRoomUser->GetComboScore_LUA();
//					kNot.m_cTechnicalRank			= CXSLDungeon::RT_F;
//					kNot.m_iTechnicalScore			= spRoomUser->GetTechnicalScore_LUA();
//					kNot.m_cTimeRank				= CXSLDungeon::RT_F;
//					kNot.m_cDamagedRank				= CXSLDungeon::RT_F;
//					kNot.m_iDamageNum				= spRoomUser->GetDamageNum_LUA();
//					kNot.m_cTotalRank				= CXSLDungeon::RT_F;
//					kNot.m_iRessurectionStoneCount	= spRoomUser->GetUsedRessurectionStoneCount();
//					kNot.m_iPassedStageCount		= spRoomUser->GetPassedStageCount();
//					kNot.m_iPassedSubStageCount		= spRoomUser->GetPassedSubStageCount();
//					//{{ 2010. 9. 14	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//					kNot.m_bIsWithPet				= spRoomUser->HavePet();
//#endif SERV_PET_SYSTEM
//					//}}
//					//{{ 2010. 11. 8	최육사	던전 로그 DB
//#ifdef SERV_DUNGEON_LOG_DB
//					kNot.m_wstrRegDate				= (const wchar_t*)kRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
//#endif SERV_DUNGEON_LOG_DB
//					//}}
//					//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
//					kNot.m_sNpcDieCount				= spRoomUser->GetKillNPC_LUA();
//					kNot.m_iGivenDamage				= 0;
//					kNot.m_iAttackDamage			= 0;
//					kNot.m_sUsingSkillKind			= 0;
//					kNot.m_iTotalEXP				= ( spRoomUser->GetRewardEXP() + spRoomUser->GetRewardPartyEXP() );
//					kNot.m_cPetID					= spRoomUser->GetPetID();
//					kNot.m_cPetEvoStep				= spRoomUser->GetPetEvolutionStep();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN
//					//}}
//					//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//					kNot.m_iBaseHP					= spRoomUser->GetBaseHP_LUA();
//					kNot.m_iEndHP					= spRoomUser->GetLastUpdatedHP();
//					kNot.m_iRecovHP					= spRoomUser->GetTotalRecoveryHP_LUA();
//					kNot.m_iContinue				= spRoomUser->GetUsedRessurectionStoneCount();
//					kNot.m_iStartContinue			= spRoomUser->GetStartTimeNumResurrectionStone();
//					kNot.m_iContinueUseStage		= spRoomUser->GetFirstUseResurrectionStoneStageID();
//					kNot.m_iContinueUseSub			= spRoomUser->GetFirstUseResurrectionStoneSubStageID();
//					kNot.m_iFrame					= spRoomUser->GetInGameAverageFrame();
//					kNot.m_wstrChannelIP			= spRoomUser->GetGameChannelIP();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//					//}}
//					//{{ 2012. 07. 11 김민성
//#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
//					kNot.m_iUserUID					= spRoomUser->GetUserUID();
//#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
//					//}}
//					//{{ 2012. 11. 6	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//					kNot.m_cAutoPartyPlay			= ( IsStartedByAutoParty() ? 1 : 0 );
//#endif SERV_BATTLE_FIELD_SYSTEM
//					//}}
//					KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_NOT, kNot );
//					//////////////////////////////////////////////////////////////////////////
//					//{{ 2010. 11. 8	최육사	던전 로그 DB
//					//#ifdef SERV_DUNGEON_LOG_DB
//					SendToLogDB( DBE_DB_LOG_DUNGEON_NOT, kNot );
//					//#endif SERV_DUNGEON_LOG_DB
//					//}}
//					//#endif SERV_STATISTICS_THREAD
//					//}}
//				}
//				//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}

				

				//{{ 2007. 12. 18  최육사  유저 통계 [수정]
				KERM_DUNGEON_USER_STATISTICS_NOT kPacketNot;
				kPacketNot.m_iDungeonID	   = m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
				kPacketNot.m_cGameResult   = KERM_DUNGEON_USER_STATISTICS_NOT::UGR_DROP;
				kPacketNot.m_bIsPartyPlay  = ( m_iStartedNumMember > 1 );
				kPacketNot.m_iGamePlayTime = ( int )( m_kTimer[TM_PLAY].elapsed() + 0.5 - GetLoadingTime() );
				kPacketNot.m_iUsedResurrectionStone = m_spRoomUserManager->GetUsedRessurectionStoneCount( iFIRST_SENDER_UID );
				SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_DUNGEON_USER_STATISTICS_NOT, kPacketNot );
				//}}

				//{{ 2007. 12. 25  최육사  부활석 통계
				if( m_spRoomUserManager->GetUsedRessurectionStoneCount( iFIRST_SENDER_UID ) > 0 )
				{
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( 0 );
					KSIManager.IncreaseCount( KStatistics::SI_RES_STONE, kKey, KStatistics::eSIColResStone_PlayerCount, 1 );
				}
				//}}
			}
		}
	}

	KEGS_LEAVE_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iItemID = kPacket_.m_iItemID;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
	//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	//던전게임 진행아이템이 없을경우 처리..
	if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_22 )
	{
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
	}
	else if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_27 )
	{
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_27;
	}
	else
	{
		kPacket.m_iReason = NetError::NET_OK;
	}
	//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	//{{ 2012. 01. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 대기 상태거나 필드로 나가는 상태일때 이탈하는거라면
	if( GetStateID() == KRoomFSM::S_WAIT  ||  GetStateID() == KRoomFSM::S_RETURN_TO_FIELD )
	{
		// 파티 이탈 처리를 하지 않는다.
		kPacket.m_bNotLeaveParty = true;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 12. 18	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP	// UNIT_TYPE 처리
	const bool bPlayWithCharLeaveBefore = m_spRoomUserManager->IsExistCharType( CXSLUnit::UT_EVE );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

	// 슬롯 정보 변경.
	if( !m_spRoomUserManager->LeaveRoom( iFIRST_SENDER_UID ) )
	{
		// 슬롯에서 나가기 실패.
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		return;
	}
	else
	{
		//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		if( kPacket_.m_iReason == NetError::NOT_LEAVE_ROOM_REASON_34 )
		{
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_34;
		}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
		//}

		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );

		//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		LogToDB_GamePlayNetWorkInfo( iFIRST_SENDER_UID, kPacket_.m_kGamePlayNetworkInfo, true );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		//}}

		//{{ 2012. 12. 18	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP	// UNIT_TYPE 처리
		if( bPlayWithCharLeaveBefore == true )
		{
			// 이탈 당사자는 버프 소멸
			KERM_DEACTIVATE_BUFF_NOT kPacketNot;
			kPacketNot.m_vecDeactivateBuffID.push_back( CXSLBuffManager::BTI_EMPTY_EXP_BUFF );
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_DEACTIVATE_BUFF_NOT, kPacketNot );

			// 아라가 파티에 아예 없다면?
			const bool bPlayWithCharLeaveAfter = m_spRoomUserManager->IsExistCharType( CXSLUnit::UT_EVE );
			if( bPlayWithCharLeaveAfter == false )
			{
                // 파티원 전체에게 버프 소멸
				BroadCast( ERM_DEACTIVATE_BUFF_NOT, kPacketNot );
			}
		}
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}
	}

	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	KBadAttitudeManager::KBadAttitudeInfo kUnitData( iFIRST_SENDER_UID );
	m_kBadAttitudeManager.RemoveUnit( iFIRST_SENDER_UID, kUnitData );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

	if( IsEmpty() == false || IsEmpty( KRoomUserManager::UT_OBSERVER ) == false )
	{
		// 방의 모든 유저들에게 슬롯 정보를 날린다.
		KEGS_LEAVE_ROOM_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iFIRST_SENDER_UID;
		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		GetRoomSlotInfo( kPacketNot.m_vecObserverSlot, KRoomUserManager::UT_OBSERVER );
		BroadCast( ERM_LEAVE_ROOM_NOT, kPacketNot );
	}

	if( (GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY) && IsEmpty() == false )
	{
		//{{ 2010. 11. 23	최육사	방 로딩 오류 수정 - 크래쉬 문제가 있어서 적용은 보류합니다.
#ifdef SERV_ROOM_LOADING_BUG_FIX
		//////////////////////////////////////////////////////////////////////////
		if( GetStateID() == KRoomFSM::S_LOAD )
		{
			if( m_spRoomUserManager->IsAllPlayerFinishLoading() == true )
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
		}
		else if( GetStateID() == KRoomFSM::S_PLAY )
		{
			CheckLoadingSuccess();
		}
		//////////////////////////////////////////////////////////////////////////	
#else
		//////////////////////////////////////////////////////////////////////////
		CheckLoadingSuccess();
		//////////////////////////////////////////////////////////////////////////
#endif SERV_ROOM_LOADING_BUG_FIX
		//}}
	}

	// 스테이지가 끝나게 되는 경우 처리.
	if( GetStateID() == KRoomFSM::S_PLAY && IsEmpty() == false )
	{
		if( CheckIfPlayEnd() )
		{
			EndPlay();
		}
		else
		{
			//{{ 2009. 7. 27  최육사	비밀스테이지
			if( m_eDungeonType == CXSLDungeon::DT_HENIR )
			{
				CheckEnterSecretStageSuccess();
			}
			//}}
		}
	}

	//결과창을 보고있을때 방을 나갈경우처리
	if( GetStateID() == KRoomFSM::S_RESULT && IsEmpty() == false )
	{
		CheckResultSuccess();
	}

	//{{ 2012. 12. 12	최육사	결과창 이탈 유저 로그
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	if( GetStateID() == KRoomFSM::S_RESULT )
	{
		CTime kCurTime = CTime::GetCurrentTime();
		KDBE_DUNGEON_RESULT_LEAVE_LOG_NOT kPacketToLog;
		kPacketToLog.m_iUnitUID = iFIRST_SENDER_UID;
		kPacketToLog.m_wstrLeaveReason = NetError::GetErrStr( kPacket_.m_iReason );
		kPacketToLog.m_wstrRoomState = GetStateIDString();
		kPacketToLog.m_wstrRegDate = (const wchar_t*)kCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		SendToLogDB( DBE_DUNGEON_RESULT_LEAVE_LOG_NOT, kPacketToLog );
	}
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	//}}

	//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	if( GetStateID() == KRoomFSM::S_WAIT_FOR_DEFENCE )
	{
		// 모든 유저의 정보가 갱신되었는지 확인한다!
		std::vector< UidType > vecLeaveUnitUID;
		if( m_spRoomUserManager->IsAllPlayerPrepareForDefenceDungeon( vecLeaveUnitUID ) == true )
		{
			// 어둠의 문 던전 입장!
			EnterDefenceDungeon( vecLeaveUnitUID );

			START_LOG( clog, L"방 이탈자에 의해 어둠의 문 던전을 시작합니다!" );
		}
	}
#else
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
//#ifdef SERV_NEW_DEFENCE_DUNGEON
//	if( GetStateID() == KRoomFSM::S_WAIT_FOR_DEFENCE )
//	{
//		// 모든 유저의 정보가 갱신되었는지 확인한다!
//		if( m_spRoomUserManager->IsAllPlayerPrepareForDefence() == true )
//		{
//			// 어둠의 문 던전 입장!
//			EnterDefenceDungeon();
//		}
//	}
//#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}

	// 방에서 모든 유저가 나갔으면 방을 닫는다.
	if( IsEmpty() == true && IsEmpty( KRoomUserManager::UT_OBSERVER ) == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );

		//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
		CTime kEndGameTime = CTime::GetCurrentTime();
		//////////////////////////////////////////////////////////////////////////
		KE_LOCAL_LOG_DUNGEON_ROOM_NOT kNot;
		kNot.m_iDungeonID		= m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
		kNot.m_iDifficultyLevel	= (int) GetDifficultyLevel();
		kNot.m_iIsChallenge		= ( m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE ? 1 : 0 );
		kNot.m_iWin				= 0;
		kNot.m_iLose			= 0;
		kNot.m_iStartNumMember	= m_iStartedNumMember;
		kNot.m_iEndNumMember	= GetNumMember();
		kNot.m_iTotalPlayTime	= static_cast<int>(GetTotalPlayTime());
		kNot.m_wstrEndGameTime	= (const wchar_t*)kEndGameTime.Format(_T("%H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_DUNGEON_ROOM_NOT, kNot );
		//////////////////////////////////////////////////////////////////////////
#endif SERV_STATISTICS_THREAD
		//}}
	}
	else
	{
		if( m_kDSSGoNextAllNot.m_iBeforeStageID != -1 &&
			m_kDSSGoNextAllNot.m_iNextStageID	!= -1 )
			SendDSSGoNextAllNot( m_kDSSGoNextAllNot );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 02. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KDungeonRoom::AutoPartyBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList )
{
	bool bMaxPartyFever = false;
	bool bExistComeBackUser = m_spRoomUserManager->IsComeBackUserInRoom();
	const int iNumMember = m_spRoomUserManager->GetNumMember();

	for( int iIndex = 0; iIndex < iNumMember; ++iIndex )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iIndex );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 유저입니다!" )
				<< BUILD_LOG( iIndex )
				<< END_LOG;
			continue;
		}

		// 컴백 유저가 있다.
		if( bExistComeBackUser == true )
		{
			if( spRoomUser->IsComeBackUser() == false )
			{
				std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
				if( mit == mapActivateBuffList.end() )
				{
					// 해당 유닛의 버프 정보가 없다
					std::vector< KBuffInfo > ActivateBuffList;

					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO;
					ActivateBuffList.push_back( kBuff );

					mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
				}
				else
				{
					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO;
					mit->second.push_back( kBuff );
				}
			}
		}

		// 현재 자동파티 인원이 1명보다 많아야 영웅의 집결 버프를 건다!
		if( iNumMember > 1 )
		{
			// 자동파티로 던전게임을 시작했다면 '영웅의 집결' 버프를 걸어주자!
			//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
			if( spRoomUser->GetOldPartyMemberCount() == 3 ) // 자동파티 신청시 파티원이 3명이었다면
			{
				std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
				if( mit == mapActivateBuffList.end() )
				{
					std::vector< KBuffInfo > ActivateBuffList;

					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_1;
					ActivateBuffList.push_back( kBuff );

					// 현재 자동파티 인원이 1명보다 많아야 재집결을 위한 휴식 버프를 건다!
					KBuffInfo kDebuff;
					kDebuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;
					ActivateBuffList.push_back( kDebuff );

					mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
				}
				else
				{
					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_1;
					mit->second.push_back( kBuff );

					// 현재 자동파티 인원이 1명보다 많아야 재집결을 위한 휴식 버프를 건다!
					KBuffInfo kDebuff;
					kDebuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;
					mit->second.push_back( kDebuff );
				}

				bMaxPartyFever = true;
			}
			else if( spRoomUser->GetOldPartyMemberCount() == 2 )	// 자동파티 신청시 파티원이 2명이었다면
			{
				std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
				if( mit == mapActivateBuffList.end() )
				{
					std::vector< KBuffInfo > ActivateBuffList;

					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_2;
					ActivateBuffList.push_back( kBuff );

					// 현재 자동파티 인원이 1명보다 많아야 재집결을 위한 휴식 버프를 건다!
					KBuffInfo kDebuff;
					kDebuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;
					ActivateBuffList.push_back( kDebuff );

					mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
				}
				else
				{
					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_2;
					mit->second.push_back( kBuff );

					// 현재 자동파티 인원이 1명보다 많아야 재집결을 위한 휴식 버프를 건다!
					KBuffInfo kDebuff;
					kDebuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;
					mit->second.push_back( kDebuff );
				}
				
				bMaxPartyFever = true;
			}
			else if( spRoomUser->GetOldPartyMemberCount() == 0 )	// 자동파티 신청시 파티가 아니었다면
			{
				std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
				if( mit == mapActivateBuffList.end() )
				{
					std::vector< KBuffInfo > ActivateBuffList;

					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_3;
					ActivateBuffList.push_back( kBuff );

					// 현재 자동파티 인원이 1명보다 많아야 재집결을 위한 휴식 버프를 건다!
					KBuffInfo kDebuff;
					kDebuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;
					ActivateBuffList.push_back( kDebuff );

					mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
				}
				else
				{
					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_BUFF_RALLY_OF_HERO_LEVEL_3;
					mit->second.push_back( kBuff );

					// 현재 자동파티 인원이 1명보다 많아야 재집결을 위한 휴식 버프를 건다!
					KBuffInfo kDebuff;
					kDebuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY;
					mit->second.push_back( kDebuff );
				}

				bMaxPartyFever = true;
			}
#endif SERV_SERVER_BUFF_SYSTEM
			//}
		}
	}
	
	return bMaxPartyFever;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP 	// UNIT_TYPE 처리
bool KDungeonRoom::PlayWithCharBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList )
{
	// 일단 현재 아리가 있는지 확인한다!
	if( m_spRoomUserManager->IsExistCharType( CXSLUnit::UT_EVE ) == false )
		return false;

	// 아리가 파티에 있으면 파티원 전체에게 캐릭터와 함께 플레이 보너스 경험치 버프를 적용시킨다.	
	const int iNumMember = m_spRoomUserManager->GetNumMember();
	for( int iIndex = 0; iIndex < iNumMember; ++iIndex )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iIndex );
		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cerr, L"존재하지 않는 유저입니다!" )
				<< BUILD_LOG( iIndex )
				<< END_LOG;
			continue;
		}
		
		// 버프 정보를 담는다!
		std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
		if( mit == mapActivateBuffList.end() )
		{
			// 해당 유닛의 버프 정보가 없다
			std::vector< KBuffInfo > ActivateBuffList;

			KBuffInfo kBuff;
			kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_EMPTY_EXP_BUFF;
			ActivateBuffList.push_back( kBuff );

			mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
		}
		else
		{
			KBuffInfo kBuff;
			kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_EMPTY_EXP_BUFF;
			mit->second.push_back( kBuff );
		}
	}

	return true;
}
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
//}}

//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
float KDungeonRoom::GetDungeonPartyBonusRate()
{ 
	int iPartyCount = m_spRoomUserManager->GetLiveMember();

	return m_spRoomUserManager->GetPartyBonusRate( iPartyCount ); 
}
#endif SERV_REFORM_ITEM_DROP
//}}

//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
_IMPL_ON_FUNC( ERM_UDP_CHECK_KICK_USER_NOT, KEGS_UDP_CHECK_KICK_USER_NOT )
{
	// PVP 에서만 사용하고 있으므로 던전에서는 아무 기능도 하지 않는다.
	// 추후에 던전에서의 기능 추가는 여기에 하면된다.
}
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC_NOPARAM( ERM_UPDATE_HENIR_REWARD_USER_NOT )
{
	VERIFY_STATE( ( 3, KRoomFSM::S_WAIT, KRoomFSM::S_LOAD, KRoomFSM::S_PLAY ) );

	KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( IS_NULL( spRoomUser ) )
	{
		START_LOG( cerr, L"존재하지 않는 유저에게 헤니르 보상 여부 정보를 업데이트 하려고 했습니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 헤니르 보상 지급 설정!
	spRoomUser->SetHenirRewardUser( true );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
_IMPL_ON_FUNC( ERM_BAD_ATTITUDE_USER_CHECK_INFO_NOT, KEGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_PLAY ) );

	// 해당 서브 스테이지 or wave 의 랭크를 구한다.
	// 해당 서브 스테이지 몬스터 접수 구하자

	// 솔플이면 검사하지 않음
	int iPartyMemberCount = m_spRoomUserManager->GetNumMember();
	if( iPartyMemberCount <= 1 )
		return;

	// 자동 파티만 검사한다.
	if( m_bStartedByAutoParty == false )
		return;

	m_kBadAttitudeManager.SetUnitSubStageInfo(  FIRST_SENDER_UID, 
												kPacket_.m_iStageID, 
												kPacket_.m_iSubStageID, 
												GetDungeonIDAndDif(), 
												m_spRoomUserManager->GetLiveMember(),
												kPacket_.m_iComboScore,
												kPacket_.m_iTechScore );
	
	// 동일 스테이지, 서브 스테이지 모든 유저가 랭크를 다 구했는가? 
	if( m_kBadAttitudeManager.IsAllUnitGetScore() == true )
	{
		std::vector<UidType> vecNewBadAttitudeUser;
		std::vector<UidType> vecNewForceExitUser;

		// 불량 유저를 찾자
		// 강퇴 유저를 찾자
		m_kBadAttitudeManager.CheckBadAttitudeUnit( vecNewBadAttitudeUser, vecNewForceExitUser );

		// 불량 유저를 찾았다
		if( vecNewBadAttitudeUser.empty() == false )
		{
			BOOST_TEST_FOREACH( UidType, iBadAttitudeUserUID, vecNewBadAttitudeUser )
			{
				// 불량 유저에게 자신이 불량 유저가 되었으니 조심하라는 메시지를 출력하게 한다.
				KRoomUserPtr pUser = m_spRoomUserManager->GetUser( iBadAttitudeUserUID );
				if( pUser != NULL )
				{
					KEGS_BAD_ATTITUDE_USER_MSG_NOT kPacket;

					kPacket.m_MsgType = KEGS_BAD_ATTITUDE_USER_MSG_NOT::MT_BAD_ATTITUDE_USER_WARNING;
					kPacket.m_iUserUID = iBadAttitudeUserUID;

					SendToGSCharacter( pUser->GetGSUID(), pUser->GetCID(), ERM_BAD_ATTITUDE_USER_MSG_NOT, kPacket );
				}

				// 불량 유저를 찾았으니 투표 가능하게 알려준다.
				KEGS_BAD_ATTITUDE_USER_NOT kNot;
				kNot.m_iUserUID = iBadAttitudeUserUID;
				BroadCast( ERM_BAD_ATTITUDE_USER_NOT, kNot );
			}
		}

		// 강퇴 유저를 찾았다
		if( vecNewForceExitUser.empty() == false )
		{
			BOOST_TEST_FOREACH( UidType, iForceExitUserUID, vecNewForceExitUser )
			{
				KRoomUserPtr pUser = m_spRoomUserManager->GetUser( iForceExitUserUID );
				if( pUser != NULL )
				{
					KERM_LEAVE_ROOM_REQ kPacketReq;
					kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_34;

					UidType iLAST_SENDER_UID  = pUser->GetGSUID();
					UidType iFIRST_SENDER_UID = pUser->GetCID();

					// 강퇴 시킴
					OnLeaveRoom( pUser->GetGSUID(), pUser->GetCID(), kPacketReq, ERM_LEAVE_ROOM_ACK );

					// 이런 이런 사유로 너님 팅경음을 알려준다.
					KEGS_BAD_ATTITUDE_USER_MSG_NOT kKick;
					kKick.m_MsgType = KEGS_BAD_ATTITUDE_USER_MSG_NOT::MT_FORCED_EXIT;
					kKick.m_iUserUID = iForceExitUserUID;
					SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_BAD_ATTITUDE_USER_MSG_NOT, kKick );

					// 이런 이런 사유로 이 유저가 나갔음을 알려준다.
					KEGS_BAD_ATTITUDE_USER_MSG_NOT kNot;
					kNot.m_MsgType = KEGS_BAD_ATTITUDE_USER_MSG_NOT::MT_BAD_ATTITUDE_USER_EXIT;
					kNot.m_iUserUID = iForceExitUserUID;
					BroadCast( ERM_BAD_ATTITUDE_USER_MSG_NOT, kNot );
				}
			}
		}
	}
};

_IMPL_ON_FUNC( ERM_DEFENCE_DUNGEON_WAVE_ID_NOT, KEGS_DEFENCE_DUNGEON_WAVE_ID_NOT )
{
	m_kBadAttitudeManager.SetDefenceDungeonWaveID( kPacket_.m_iWaveID );
}

_IMPL_ON_FUNC( ERM_FORCED_EXIT_VOTE_REQ, KEGS_FORCED_EXIT_VOTE_REQ )
{
	KEGS_FORCED_EXIT_VOTE_ACK kPacket;

	// 강퇴 투표를 한다!
	if( kPacket_.m_bWantForceExit == false )
	{
		START_LOG( cerr, L"투표 한다면서 이 값이 false?" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_bWantForceExit )
			<< END_LOG;
	}

	// 투표하는 사람을 찾자
	KRoomUserPtr pVoteUser = m_spRoomUserManager->GetUser( FIRST_SENDER_UID );
	if( pVoteUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저가 투표하려 한다." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;
	
		kPacket.m_iOK = NetError::ERR_BAD_ATTITUDE_00;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_FORCED_EXIT_VOTE_ACK, kPacket );
		return;
	}

	// 투표 당한 사람을 찾자
	KRoomUserPtr pBadUser = m_spRoomUserManager->GetUser( kPacket_.m_iUserUID );
	if( pBadUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저가 투표하려 한다." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BAD_ATTITUDE_01;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_FORCED_EXIT_VOTE_ACK, kPacket );
		return;
	}

	// 득표수를 증가 시키자
	m_kBadAttitudeManager.IncreaseVoteOnePoint( kPacket_.m_iUserUID, FIRST_SENDER_UID );

	kPacket.m_iUserUID = kPacket_.m_iUserUID;
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_FORCED_EXIT_VOTE_ACK, kPacket );
}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
IMPL_ON_FUNC_NOPARAM( ERM_SYNC_DUNGEON_TIMER_NOT )
{
	if( IsHost( FIRST_SENDER_UID ) == false )
	{
		return;
	}

	// 발렌타인 던전만 해당됨
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	if( CXSLDungeon::IsValentineDungeonInt( GetDungeonIDAndDif() ) == false )
#else
	if( CXSLDungeon::IsValentineDungeon( GetDungeonIDAndDif() ) == false )
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
	{
		return;
	}

	m_kValentineManager.SetStartTimer( KValentineManager::TEM_AFTER_DUNGEON_START, true );

	KEGS_SYNC_DUNGEON_TIMER_NOT kNot;
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	kNot.m_iRemainTime = m_kValentineManager.GetRemainTime( KValentineManager::TEM_AFTER_DUNGEON_START );
#else
	kNot.m_iRemainTime = KValentineManager::LDT_VALENTINE - m_kValentineManager.GetRemainTime( KValentineManager::TEM_AFTER_DUNGEON_START );
#endif SERV_EVENT_VALENTINE_DUNGEON_INT
	BroadCast( ERM_SYNC_DUNGEON_TIMER_NOT, kNot );
}
#endif SERV_EVENT_VALENTINE_DUNGEON
//}

//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_WAIT_FOR_DEFENCE ) );

	// 어둠의 문 던전을 시작하기 위한 RoomUserInfo정보 받음 체크
	if( m_spRoomUserManager->SetPrepareForDefenceDungeon( FIRST_SENDER_UID, true ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 유저 입니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 게임 스탯 적용!
	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );
}

_IMPL_ON_FUNC( ERM_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT, KEGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_WAIT_FOR_DEFENCE ) );

	// 어둠의 문 입장 팝업
	if( m_spRoomUserManager->SetEnterDefenceDungeon( FIRST_SENDER_UID, kPacket_.m_bAgree ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 유저 입니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_bAgree )
			<< END_LOG;
		return;
	}

	START_LOG( clog, L"어둠의 문 입장에 대한 응답을 받았습니다." )
		<< BUILD_LOG( FIRST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_bAgree );

	// 모든 유저의 정보가 갱신되었는지 확인한다!
	std::vector< UidType > vecLeaveUnitUID;
	if( m_spRoomUserManager->IsAllPlayerPrepareForDefenceDungeon( vecLeaveUnitUID ) == true )
	{
		// 어둠의 문 던전 입장!
		EnterDefenceDungeon( vecLeaveUnitUID );
	}
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
IMPL_ON_FUNC( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK )
{
	VERIFY_STATE( ( 1, KRoomFSM::S_WAIT_FOR_DEFENCE ) );

	if( m_spRoomUserManager->SetPrepareForDefenceDungeon( FIRST_SENDER_UID, true ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 유저 입니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 게임 스탯 적용!
	m_spRoomUserManager->SetRoomUserInfo( FIRST_SENDER_UID, kPacket_.m_kRoomUserInfo );

	// 모든 유저의 정보가 갱신되었는지 확인한다!
	if( m_spRoomUserManager->IsAllPlayerPrepareForDefenceDungeon() == true )
	{
		// 어둠의 문 던전 입장!
		EnterDefenceDungeon();
	}
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}
//////////////////////////////////////////////////////////////////////////
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
//////////////////////////////////////////////////////////////////////////
//}}

#ifdef SERV_CRAYON_POP_EVENT_BUFF// 작업날짜: 2013-06-10	// 박세훈
void KDungeonRoom::CrayonPopEventBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList ) const
{
	int			iBuffTempletID;
	KLottery	kLot;
	kLot.AddCase( CXSLBuffManager::BTI_BUFF_ELRIN, 45.0f );
	kLot.AddCase( CXSLBuffManager::BTI_BUFF_SOYUL, 30.0f );
	kLot.AddCase( CXSLBuffManager::BTI_BUFF_KUMI, 5.0f );
	kLot.AddCase( CXSLBuffManager::BTI_BUFF_CHOA, 10.0f );
	kLot.AddCase( CXSLBuffManager::BTI_BUFF_WEY, 10.0f );
	iBuffTempletID = kLot.Decision();

	if( iBuffTempletID == KLottery::CASE_BLANK )
	{
		iBuffTempletID = CXSLBuffManager::BTI_BUFF_SOYUL;
	}

	const int iNumMember = m_spRoomUserManager->GetNumMember();

	for( int iIndex = 0; iIndex < iNumMember; ++iIndex )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iIndex );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 유저입니다!" )
				<< BUILD_LOG( iIndex )
				<< END_LOG;
			continue;
		}

		std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
		if( mit == mapActivateBuffList.end() )
		{
			// 해당 유닛의 버프 정보가 없다
			std::vector< KBuffInfo > ActivateBuffList;

			KBuffInfo kBuff;
			kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = iBuffTempletID;
			ActivateBuffList.push_back( kBuff );

			mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
		}
		else
		{
			KBuffInfo kBuff;
			kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = iBuffTempletID;
			mit->second.push_back( kBuff );
		}
	}
}
#endif // SERV_CRAYON_POP_EVENT_BUFF

#ifdef SERV_BUFF_BONUSRATE_HAMEL_EVENT
void	KDungeonRoom::PlayHamelEventBuff(IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList )
{
	IF_EVENT_ENABLED( CEI_HAMEL_EVENT_BUFF ) //코드 이벤트 일단 제외 
	{
		// 자신의 레벨이 45~50사이인지를 확인 하자
		const int iNumMember = m_spRoomUserManager->GetNumMember();
		for( int iIndex = 0; iIndex < iNumMember; ++iIndex )
		{
			KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iIndex );
			if( IS_NULL( spRoomUser ) )
			{
				START_LOG( cerr, L"존재하지 않는 유저입니다!" )
					<< BUILD_LOG( iIndex )
					<< END_LOG;
				continue;
			}
			//레벨 구간을 확인하자
			if( spRoomUser->GetLevel() >= _CONST_EVENT_BUFF_EXP_::iMinLevel && spRoomUser->GetLevel() <= _CONST_EVENT_BUFF_EXP_::iMaxLevel )
			{
				//버프 정보를 담는다!
				std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
				if( mit == mapActivateBuffList.end() )
				{
					//해당 유닛의 버프 정보가 없다면...
					std::vector< KBuffInfo > ActivateBuffList;
					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_SECRET_OF_THOUSAND_YEARS_FOX;
					ActivateBuffList.push_back( kBuff );

					mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
				}
				else
				{
					KBuffInfo kBuff;
					kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = CXSLBuffManager::BTI_SECRET_OF_THOUSAND_YEARS_FOX;
					mit->second.push_back( kBuff );
				}
			}
		}
	}
}
#endif SERV_BUFF_BONUSRATE_HAMEL_EVENT

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
IMPL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT )
{
}
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST


#ifdef SERV_BONUS_BUFF_SYSTEM
bool KDungeonRoom::PlayWithBonusBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList )
{
	// 여기서 조건 채크 하고, 버프 걸어주자

	const int iNumMember = m_spRoomUserManager->GetNumMember();
	for( int iIndex = 0; iIndex < iNumMember; ++iIndex )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iIndex );
		if( IS_NULL( spRoomUser ) )
		{
			START_LOG( cerr, L"존재하지 않는 유저입니다!" )
				<< BUILD_LOG( iIndex )
				<< END_LOG;
			continue;
		}

		// 이벤트 악세사리 착용중인지 확인한다
		KRoomUserInfo kRoomUserInfo;
		spRoomUser->GetRoomUserInfo(kRoomUserInfo);
		std::map< int, KInventoryItemInfo >::const_iterator mit = kRoomUserInfo.m_mapEquippedItem.find(CXSLUnit::NESI_AC_FACE1);
		
		if( kRoomUserInfo.m_mapEquippedItem.end() != mit 
			&& BUFF_CONDITION_ITEM_ID_BEGIN <= mit->second.m_kItemInfo.m_iItemID && BUFF_CONDITION_ITEM_ID_END >= mit->second.m_kItemInfo.m_iItemID )
		{
			// 이벤트 악세사리 착용 중이므로 버프를 적용시킨다.
			// 우선 어느 버프를 적용시킬지 정한다.
			int iBuffID = 0;
			KLottery	kLot;
			kLot.AddCase( CXSLBuffManager::BTI_BUFF_2YEARS_EVENT_CN_DAMAGE_UP, 20.0f );
			kLot.AddCase( CXSLBuffManager::BTI_BUFF_2YEARS_EVENT_CN_MOVE_JUMP_SPEED_UP, 20.0f );
			kLot.AddCase( CXSLBuffManager::BTI_BUFF_2YEARS_EVENT_CN_ACCURACY_AVOID_UP, 20.0f );
			kLot.AddCase( CXSLBuffManager::BTI_BUFF_2YEARS_EVENT_CN_CRI_SPEED_UP, 20.0f );
			kLot.AddCase( CXSLBuffManager::BTI_BUFF_2YEARS_EVENT_CN_DAMAGE_INC_DEC_UP, 18.0f );
			kLot.AddCase( CXSLBuffManager::BTI_BUFF_2YEARS_EVENT_CN_ALL_IN_ONE, 2.0f );
			iBuffID = kLot.Decision();

			if( iBuffID == KLottery::CASE_BLANK )
			{
				iBuffID = CXSLBuffManager::BTI_BUFF_2YEARS_EVENT_CN_ALL_IN_ONE;
			}

			// 버프 정보를 담는다!
			std::map< UidType, std::vector< KBuffInfo > >::iterator mit = mapActivateBuffList.find( spRoomUser->GetCID() );
			if( mit == mapActivateBuffList.end() )
			{
				// 해당 유닛의 버프 정보가 없다
				std::vector< KBuffInfo > ActivateBuffList;

				KBuffInfo kBuff;
				kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = iBuffID;
				ActivateBuffList.push_back( kBuff );

				mapActivateBuffList.insert( std::make_pair( spRoomUser->GetCID(), ActivateBuffList ) );
			}
			else
			{
				KBuffInfo kBuff;
				kBuff.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID = iBuffID;
				mit->second.push_back( kBuff );
			}
		}
	}

	return true;
}
#endif SERV_BONUS_BUFF_SYSTEM