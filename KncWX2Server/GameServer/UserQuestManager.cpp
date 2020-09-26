#include "GSUser.h"
#include "NetError.h"

#include ".\userquestmanager.h"
#include "x2data/XSLItemManager.h"
#include "x2data/XSLSocketItem.h"
#include "ExpTable.h"
//{{ 2010. 06. 20  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
	#include "ThreadStatisticsManager.h"
#else
	#include "StatisticsManager.h"
#endif SERV_STATISTICS_THREAD
//}}
#include "GameSysVal/GameSysVal.h"
#include "AbuserLog/KAbuserLogManager.h"
//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
#include "GameEvent/GameEventManager.h"
//}}
//{{ 2010. 05. 19  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	#include "RoomListManager.h"
#endif SERV_INTEGRATION
//}}

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	#include "ClassChangeTable.h"
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

KUserQuestManager::KUserQuestManager(void) :
m_iRewardItemCount( 0 )
{
}

KUserQuestManager::~KUserQuestManager(void)
{
	ClearUnitQuest();
}

//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
void KUserQuestManager::SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< KCompleteQuestInfo >& vecCompleteQuest, IN KGSUserPtr spUser )
#else
void KUserQuestManager::SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< int >& vecCompleteQuest, IN KGSUserPtr spUser )
#endif SERV_DAILY_QUEST
//}}
{
	ClearUnitQuest();

	//진행중 퀘스트 셋팅하기
	BOOST_TEST_FOREACH( const KQuestInstance&, kQuestInstance, vecQuest )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET // 퀘스트 템플릿 부재
			CTime kRegDate = CTime::GetCurrentTime();
			KE_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT kNot;
			kNot.m_iQuestID	= kQuestInstance.m_iID;
			kNot.m_wstrRegDate = (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
			KSIManager.QueueingEvent( E_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT, kNot );
			continue;
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET 

			START_LOG( cerr, L"QUEST TEMPLET가 없음." )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
				<< END_LOG;
			continue;
		}

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
		if ( SiKGameEventManager()->IsEnableEventQuest(kQuestInstance.m_iID) == false )
		{
			continue; // 2013.06.10 lygan_조성욱 // 어차피 활성화 되지 않은 퀘스트는 AfterQuest 를 찾을 필요가 없다
		}
#endif //SERV_ALLOW_EVENT_ERASE

		KQuestInstance kQuest;
		kQuest.m_iID			= kQuestInstance.m_iID;
		kQuest.m_OwnorUnitUID	= kQuestInstance.m_OwnorUnitUID;

		//sub quest seting(완료했는건지 아닌건지도 파악)
		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			KSubQuestInstance kSub;

			const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET가 없음." )
					<< BUILD_LOG( kQuest.m_iID )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[uiIndexSub] )
					<< BUILD_LOG( kQuest.m_OwnorUnitUID )
					<< END_LOG;

				continue;
			}

			kSub.m_iID			= pSubTemplet->m_iID;
			kSub.m_ucClearData	= kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData;

			switch( pSubTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_DUNGEON_TIME:
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
#else // SERV_UPGRADE_SKILL_SYSTEM_2013
/*
			case CXSLQuestManager::SQT_DUNGEON_RANK:
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
			case CXSLQuestManager::SQT_NPC_TALK:
				//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
			case CXSLQuestManager::SQT_ITEM_ENCHANT:
			case CXSLQuestManager::SQT_ITEM_ATTRIB:
#endif SERV_QUEST_CLEAR_EXPAND
				//}}
				//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			case CXSLQuestManager::SQT_VISIT_VILLAGE:
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}
				//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
			case CXSLQuestManager::SQT_VISIT_FIELD:
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
				//}}
				//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			case CXSLQuestManager::SQT_VISIT_DUNGEON:
			case CXSLQuestManager::SQT_FIND_NPC:
#endif SERV_REFORM_QUEST
					//}}
				{
					if( kSub.m_ucClearData != 0 )
						kSub.m_bIsSuccess	= true;
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_DAMAGE:
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData <= pSubTemplet->m_ClearCondition.m_iDungeonDamage )
						kSub.m_bIsSuccess = true;						
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
				//{{ 2010. 05. 19  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
			case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
#endif SERV_INTEGRATION
				//}}
				//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
			case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
#endif SERV_QUEST_CLEAR_EXPAND
				//}}
				//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
				//}}
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
			case CXSLQuestManager::SQT_DUNGEON_RANK:
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iDungeonClearCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#ifdef SERV_SUB_QUEST_USE_ITEM
			case CXSLQuestManager::SQT_ITEM_USE:
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iUseItemNum )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_SUB_QUEST_USE_ITEM
			case CXSLQuestManager::SQT_NPC_HUNT:
				{
					if( kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iKillNum )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_ITEM_COLLECTION:
			case CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION:
				{
					if( kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iCollectionItemNum )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_PVP_PLAY:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPPlay )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_PVP_WIN:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPWin )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_PVP_KILL:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPKill )
						kSub.m_bIsSuccess = true;
				}
				break;

				//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
			case CXSLQuestManager::SQT_ITEM_SOCKET:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iSocketCount )
						kSub.m_bIsSuccess = true;
				}
				break;
			
			case CXSLQuestManager::SQT_ITEM_RESOLVE:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iResolveCount )
						kSub.m_bIsSuccess = true;
				}
				break;

			case CXSLQuestManager::SQT_USE_SKILL_POINT:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iSpUseCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_QUEST_CLEAR_EXPAND
				//}}
				//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
			case CXSLQuestManager::SQT_PVP_NPC_HUNT:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPWin )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_NEW_PVP_QUEST
				//}} 

#ifdef SERV_SKILL_USE_SUBQUEST
			case CXSLQuestManager::SQT_SKILL_USE:
				{
					if( kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iUseSkillCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_SKILL_USE_SUBQUEST

#ifdef SERV_POINT_COUNT_SYSTEM
			case CXSLQuestManager::SQT_POINT_COUNT:
				{
					if ( spUser != NULL )
					{			
						//여기에 성공 조건 달아 주면 된다.
						if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iPointCount )
							kSub.m_bIsSuccess = true;
					}
				}
				break;
#endif SERV_POINT_COUNT_SYSTEM
				//{{ 2012. 09. 18	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
			case CXSLQuestManager::SQT_PVP_PLAY_ARRANGE:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPPlay )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_2012_PVP_SEASON2
				//}}

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
			case CXSLQuestManager::SQT_PVP_HERO_NPC_KILL:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPKill )
						kSub.m_bIsSuccess = true;
				}
				break;
			case CXSLQuestManager::SQT_PVP_HERO_NPC_PLAY:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iPVPPlay )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND

#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
			case CXSLQuestManager::SQT_LEARN_NEW_SKILL:
				{
					if( kSub.m_ucClearData >= (UCHAR)pSubTemplet->m_ClearCondition.m_iLearnNewSkillCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

			default:
				{
					START_LOG( cerr, L"없는 서브퀘스트 타입." )
						<< BUILD_LOG( pSubTemplet->m_eClearType )
						<< BUILD_LOG( kQuest.m_iID )
						<< BUILD_LOG( kQuest.m_OwnorUnitUID )
						<< END_LOG;
				}
			}

			kQuest.m_vecSubQuestInstance.push_back( kSub );
		}

		m_mapQuesting[kQuest.m_iID] = kQuest;
	}

	//정리된 진행퀘스트 데이터를 세로 셋팅
	vecQuest.clear();
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		vecQuest.push_back( mit->second );
	}

	//완료 퀘스트 정리해서 넣기
	//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecCompleteQuest )
	{
		std::map< int, KCompleteQuestInfo >::iterator mitQC = m_mapCompleteQuest.find( kInfo.m_iQuestID );
		if( mitQC == m_mapCompleteQuest.end() )
		{
			m_mapCompleteQuest.insert( std::make_pair( kInfo.m_iQuestID, kInfo ) );
		}
		else
		{
			++(mitQC->second.m_iCompleteCount);
		}
	}
//#else
//	for( int i = 0; i < (int)vecCompleteQuest.size(); ++i )
//	{
//		const int iSubQuestID = vecCompleteQuest[i];
//
//		std::map< int, int >::iterator mitQC = m_mapCompleteQuest.find( iSubQuestID );
//		if( mitQC == m_mapCompleteQuest.end() )
//		{
//			m_mapCompleteQuest[iSubQuestID] = 1;
//		}
//		else
//		{
//			++(mitQC->second);
//		}
//	}
#endif SERV_DAILY_QUEST
	//}}

	// DB로부터 가져온 완료된 퀘스트 리스트를 가지고 After Quest의 open조건을 체크한다!
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecCompleteQuest )
	{
		CheckEpicQuest_NewQuestByAfterQuest( kInfo.m_iQuestID, spUser, false, false );
	}

	// Before Quest의 open조건을 체크한다!
	CheckEpicQuest_NewQuestByBeforeQuest( spUser, false );
}

bool KUserQuestManager::AddQuest( IN KQuestInstance& kInfo )
{
	SET_ERROR( NET_OK );

	if( IsQuest( kInfo.m_iID ) == true )
	{
		START_LOG( cerr, L"동일한 QUEST 수행중.. 왜 또왔노..?" )
			<< BUILD_LOG( kInfo.m_iID )
			<< BUILD_LOG( kInfo.m_OwnorUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_01 );

		return false;
	}

	m_mapQuesting[kInfo.m_iID] = kInfo;
	return true;
}

bool KUserQuestManager::RemoveQuest( IN int iQuestID )
{
	SET_ERROR( NET_OK );

	std::map< int, KQuestInstance >::iterator mit;
	mit = m_mapQuesting.find( iQuestID );
	if( mit == m_mapQuesting.end() )
	{
		START_LOG( cerr, L"없는걸 지우네 .헐~!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );

		return false;
	}

	m_mapQuesting.erase( mit );
	return true;
}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
bool KUserQuestManager::Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool IsAfterQuest/*= true*/, IN const bool bSendAck /*= true*/ )
{
	SET_ERROR( NET_OK );

	KEGS_NEW_QUEST_ACK	kPacket;

	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 퀘스트.!" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// PC방 전용 퀘스트인지 체크
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_16 );
			goto error_proc;
		}
	}

	// 이벤트 퀘스트라면 현재 진행중인 이벤트인지 확인한다.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// 현재 진행중인 이벤트인지 확인!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	int iRecruitEventQuestIDForNewUserSize = sizeof( arriRecruitEventQuestIDForNewUser ) / sizeof( arriRecruitEventQuestIDForNewUser[0] );
	for( int iRecruitEventQuestIDForNewUserIndex = 0; iRecruitEventQuestIDForNewUserIndex < iRecruitEventQuestIDForNewUserSize; ++iRecruitEventQuestIDForNewUserIndex )
	{
		// 추천받은 자 전용 퀘스트인지 확인
		if( arriRecruitEventQuestIDForNewUser[iRecruitEventQuestIDForNewUserIndex] == kReq.m_iQuestID )
		{
			// 추천받은 자가 아니라면 퀘스트를 못 받는다!
			if( spUser->IsRecruit() == false )
			{
				SET_ERROR( ERR_QUEST_21 );
				goto error_proc;
			}

			break;
		}
	}
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

	//# 에픽 퀘스트 이면 최대수락 개수의 영향을 받지 않는다.
	//# 이벤트 퀘스트 이면 최대수락 개수의 영향을 받지 않는다.
	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType
	 && CXSLQuestManager::QT_EVENT != pQuestTemplet->m_eQuestType )
	{
		if( GetNormalQuestSize() >= MAX_ING_QUEST_NUM )
		{
			SET_ERROR( ERR_QUEST_28 );
			goto error_proc;
		}
	}

	//이미 하고 있는 퀘스트인지..
	if( IsQuest( kReq.m_iQuestID ) == true )
	{
		START_LOG( cwarn, L"이미 가지고 있는 퀘스트를 등록하려함" )
			<< BUILD_LOG( kReq.m_iQuestID );

		SET_ERROR( ERR_QUEST_01 );
		goto error_proc;
	}

	//전직퀘일경우 다른 전직퀘스트를 하고있지는 않는지..
	if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
	{
		if( IsChangeJobQuest() == true )
		{
			START_LOG( cerr, L"전직퀘를 하고있는데 새로운 전직퀘스트를 시작 할려고함.!" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_17 );
			goto error_proc;
		}
	}

	//반복가능하지 않은 QUEST일 경우
	// 일반 퀘스트
	if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
	{
		if( GetQuestCompleteCount( kReq.m_iQuestID ) > 0 )
		{
			START_LOG( cwarn, L"이미 클리어한 퀘스트.!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_04 );
			goto error_proc;
		}
	}
	// 하루 단위 퀘스트
	else if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
	{
		// 이미 완료한 퀘스트 인가?
		KCompleteQuestInfo* pCompleteQuestInfo = GetCompleteQuestInfo( kReq.m_iQuestID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"퀘스트 완료는 했는데 완료날짜 정보가 이상합니다." )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}

			// 이미 완료한 퀘스트라면 클리어한 날짜를 확인하여 오늘 수행 가능한지 체크한다.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
			{
				// 6시보다 넘는 시각이라면 다음날로 세팅
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// 오늘 수행 가능한가요?
			if( tCheckTime > tCurTime )
			{
				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}
#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
			BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
			{
				if( CheckCompletedQuestToday( iBeforeQuestID ) == false )
				{
					START_LOG( clog, L"오늘 이전 퀘스트들을 완료 안했음" )
						<< BUILD_LOG( kReq.m_iQuestID )
						<< BUILD_LOG( iBeforeQuestID )
						;

					SET_ERROR( ERR_QUEST_06 );
					goto error_proc;
				}
			}
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX	
		}
	}

	//모든 유닛이 할수있는 퀘스트가 아닐경우

	//#1. 유닛 그룹이 할수있는 경우.
	if( pQuestTemplet->m_Condition.m_eUnitType != CXSLUnit::UT_NONE )
	{
		if( pQuestTemplet->m_Condition.m_eUnitType != spUser->GetUnitType() )
		{
			START_LOG( cerr, L"QUEST 수행가능 유닛이 아님.! 누구냐.. 넌." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_eUnitType )
				<< BUILD_LOGc( spUser->GetUnitType() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	//#2. 특정유닛이 할수있는 경우.
	if( pQuestTemplet->m_Condition.m_setUnitClass.empty() == false )
	{
		//유닛과 맞지 않는 퀘스트로 체크하여 시작할수 없음.
		if( pQuestTemplet->m_Condition.m_setUnitClass.find( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == pQuestTemplet->m_Condition.m_setUnitClass.end() )
		{
			START_LOG( cerr, L"QUEST 수행가능 유닛이 아님.! 누구냐.. 넌." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_setUnitClass.size() )
				<< BUILD_LOGc( spUser->GetUnitClass() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	// To do...
	//[ing]QUEST 수행을 위해 모아야할 아이템 체크
	//if( pQuestTemplet->m_Condition.m_iConditionItemID)

	//QUEST 수행을 위해 이전에 완료해야할 퀘스트 검사.
	BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
	{
		if( GetQuestCompleteCount( iBeforeQuestID ) == 0 )
		{
			START_LOG( clog, L"어여가서 하나 더해오셔.. !!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iBeforeQuestID )
				;

			SET_ERROR( ERR_QUEST_06 );
			goto error_proc;
		}
	}

	// 서버에서 수락 요청하는 퀘스트는 검사하지 않도록하자
	if( IsAfterQuest == false )
	{
		if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_eStartNPCID != kReq.m_iTalkNPCID &&
			kReq.m_iTalkNPCID != CXSLUnitManager::NUI_BILLBOARD
			)
		{
			START_LOG( cerr, L"잘못된 NPC에게 QUEST를 받았음..모지..?" )
				<< BUILD_LOG( pQuestTemplet->m_eStartNPCID )
				<< BUILD_LOG( kReq.m_iTalkNPCID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_07 );
			goto error_proc;
		}
	}

	// 레벨 검사
	// 퀘스트를 받기 낮은 레벨이라면
	if( pQuestTemplet->m_Condition.m_iLevel	> spUser->GetLevel() )
	{
		START_LOG( clog, L"레벨 좀 더 올려봐봐..ㅡㅡ^" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_12 );
		goto error_proc;
	}
	// 퀘스트를 받기 높은 레벨이라면
	//	m_iLimitLevel 가 -1 이면 존재하지 않는다는 의미
	if( pQuestTemplet->m_Condition.m_iLimitLevel != -1 && pQuestTemplet->m_Condition.m_iLimitLevel < spUser->GetLevel() )
	{
		START_LOG( clog, L"레벨 넘 높잖아..ㅡㅡ^" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLimitLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_27 );
		goto error_proc;
	}

	// 일일 랜덤 퀘스트 인가?
	if( pQuestTemplet->m_iRandomGroupID > 0 )
	{
		int iTodayQuest = SiCXSLQuestManager()->GetTodayRandomQuest( pQuestTemplet->m_iRandomGroupID );

		if( iTodayQuest <= 0 || pQuestTemplet->m_iID != iTodayQuest )
		{
			START_LOG( clog, L"오늘 할 수 있는 일일 랜덤 퀘스트가 아니다!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( pQuestTemplet->m_iRandomGroupID )
				<< BUILD_LOGc( iTodayQuest )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_26 ); 
			goto error_proc;
		}
	}

	{
		KDBE_NEW_QUEST_REQ kDBReq;

		if( pQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// 퀘스트 수락 보상이 인벤토리에 남아 있으면 퀘스트 수락 못함
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"퀘스트 수락시 받는 보상이 이미 인벤토리에 있으면 퀘스트 수락 못함." )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_24 );
					goto error_proc;
				}
			}

			// 퀘스트 수락 보상이 들어갈만한 빈슬롯이 있는지 검사
			if( spUser->IsEnoughSpaceExist( pQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"퀘스트 수락시 받는 보상이 들어갈 인벤토리 빈공간이 부족합니다." )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< BUILD_LOG( pQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_25 );
				goto error_proc;
			}

			std::map< int, KItemInfo > mapReward;

			std::map< int, int >::const_iterator mit;
			for( mit = pQuestTemplet->m_mapAcceptRewardItem.begin(); mit != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mit )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"아이템 템플릿 포인터 이상.!" )
						<< BUILD_LOG( mit->first )
						<< END_LOG;
					continue;
				}

				KItemInfo kNewItem;
				kNewItem.m_iItemID = mit->first;
				kNewItem.m_iQuantity = mit->second;
				kNewItem.m_cUsageType = pItemTemplet->m_PeriodType;
				mapReward.insert( std::make_pair( mit->first, kNewItem ) );

				// 통계 : Local Item
				{
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( mit->first );
					KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_QUEST, mit->second );
					KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_QUEST, mit->second );

				}

				// 유저 통계 습득 아이템
				spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, mit->second );

				// 아이템 어뷰저
				spUser->CheckItemAbuser( KAbuserLogManager::RS_QUEST, kDBReq.m_mapInsertedItem );
			}

			LIF( spUser->m_kInventory.PrepareInsert( mapReward, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo ) );

			// DB업데이트
			spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
			spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
		}

		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_iTalkNPCID	= kReq.m_iTalkNPCID;
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		// 안쓰는 값이지만 그래도 초기화용으로 해준다.
		kDBReq.m_UserUID	= spUser->GetUID();
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		spUser->SendToGameDB( DBE_NEW_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  최육사  DB 통계 퀘스트
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Accept, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_AcceptLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  최육사  유저 통계
		int iColumnIndex;
		if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
		{
			iColumnIndex = KUserStatistics::US_Etc_NQuestAccept;
		}
		else
		{
			iColumnIndex = KUserStatistics::US_Etc_RQuestAccept;
		}

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return true;

error_proc:
	kPacket.m_iOK	= NetError::GetLastError();

	if( bSendAck )
	{
		spUser->SendPacket( EGS_NEW_QUEST_ACK, kPacket );
	}

	return false;
}

#else					// SERV_REFORM_QUEST

//{{ 2011. 01. 03	최육사	에픽 퀘스트
//#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
bool KUserQuestManager::Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool bSendAck /*= true*/ )
//#else
//bool KUserQuestManager::Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser )
//#endif SERV_EPIC_QUEST_CHECK_UPDATE
//}}
{
	SET_ERROR( NET_OK );

	KEGS_NEW_QUEST_ACK	kPacket;

	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 퀘스트.!" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2010. 01. 29  최육사	PC방 전용 퀘스트
#ifdef SERV_PC_BANG_QUEST
	//////////////////////////////////////////////////////////////////////////	

	// PC방 전용 퀘스트인지 체크
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_16 );
			goto error_proc;
		}
	}

	//////////////////////////////////////////////////////////////////////////
#endif SERV_PC_BANG_QUEST
	//////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 02. 06  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST

	// 이벤트 퀘스트라면 현재 진행중인 이벤트인지 확인한다.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// 현재 진행중인 이벤트인지 확인!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

#endif SERV_DAY_QUEST
	//}}
	//////////////////////////////////////////////////////////////////////////	

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	int iRecruitEventQuestIDForNewUserSize = sizeof( arriRecruitEventQuestIDForNewUser ) / sizeof( arriRecruitEventQuestIDForNewUser[0] );
	for( int iRecruitEventQuestIDForNewUserIndex = 0; iRecruitEventQuestIDForNewUserIndex < iRecruitEventQuestIDForNewUserSize; ++iRecruitEventQuestIDForNewUserIndex )
	{
		// 추천받은 자 전용 퀘스트인지 확인
		if( arriRecruitEventQuestIDForNewUser[iRecruitEventQuestIDForNewUserIndex] == kReq.m_iQuestID )
		{
			// 추천받은 자가 아니라면 퀘스트를 못 받는다!
			if( spUser->IsRecruit() == false )
			{
				SET_ERROR( ERR_QUEST_21 );
				goto error_proc;
			}
			break;
		}
	}
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER

	//퀘스트를 한번에 수행할수있는 최대개수
#ifdef SERV_EPIC_QUEST
	//# 에픽퀘스트 이면 최대수락 개수의 영향을 받지 않는다.
	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType )
	{
		if( GetNormalQuestSize() >= MAX_ING_QUEST_NUM )
		{
			SET_ERROR( ERR_QUEST_28 );
			goto error_proc;
		}
	}
#else SERV_EPIC_QUEST
	if( m_mapQuesting.size() >= MAX_ING_QUEST_NUM )
	{
		SET_ERROR( ERR_QUEST_28 );
		goto error_proc;
	}
#endif SERV_EPIC_QUEST

	//이미 하고 있는 퀘스트인지..
	if( IsQuest( kReq.m_iQuestID ) == true )
	{
		START_LOG( cwarn, L"이미 가지고 있는 퀘스트를 등록하려함" )
			<< BUILD_LOG( kReq.m_iQuestID );

		SET_ERROR( ERR_QUEST_01 );
		goto error_proc;
	}

	//전직퀘일경우 다른 전직퀘스트를 하고있지는 않는지..
	if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
	{
		if( IsChangeJobQuest() == true )
		{
			START_LOG( cerr, L"전직퀘를 하고있는데 새로운 전직퀘스트를 시작 할려고함.!" )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_17 );
			goto error_proc;
		}
	}

	//반복가능하지 않은 QUEST일 경우
	//{{ 2010. 04. 01  최육사	비밀던전 헬모드
#ifdef SERV_DAILY_QUEST
	// 일반 퀘스트
	if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
	{
		if( GetQuestCompleteCount( kReq.m_iQuestID ) > 0 )
		{
			START_LOG( cwarn, L"이미 클리어한 퀘스트.!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_04 );
			goto error_proc;
		}
	}
	// 하루 단위 퀘스트
	else if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
	{
		// 이미 완료한 퀘스트 인가?
		KCompleteQuestInfo* pCompleteQuestInfo = GetCompleteQuestInfo( kReq.m_iQuestID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"퀘스트 완료는 했는데 완료날짜 정보가 이상합니다." )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}

			// 이미 완료한 퀘스트라면 클리어한 날짜를 확인하여 오늘 수행 가능한지 체크한다.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
			{
				// 6시보다 넘는 시각이라면 다음날로 세팅
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// 오늘 수행 가능한가요?
			if( tCheckTime > tCurTime )
			{
				SET_ERROR( ERR_QUEST_04 );
				goto error_proc;
			}
		}
	}
#else
	if( pQuestTemplet->m_bRepeat == false )
	{
		if( GetQuestCompleteCount( kReq.m_iQuestID ) > 0 )
		{
			START_LOG( cwarn, L"이미 클리어한 퀘스트.!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_04 );
			goto error_proc;
		}
	}
#endif SERV_DAILY_QUEST
	//}}	

	//모든 유닛이 할수있는 퀘스트가 아닐경우

	//#1. 유닛 그룹이 할수있는 경우.
	if( pQuestTemplet->m_Condition.m_eUnitType != CXSLUnit::UT_NONE )
	{
		if( pQuestTemplet->m_Condition.m_eUnitType != spUser->GetUnitType() )
		{
			START_LOG( cerr, L"QUEST 수행가능 유닛이 아님.! 누구냐.. 넌." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_eUnitType )
				<< BUILD_LOGc( spUser->GetUnitType() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	//#2. 특정유닛이 할수있는 경우.
	if( pQuestTemplet->m_Condition.m_setUnitClass.empty() == false )
	{
		//유닛과 맞지 않는 퀘스트로 체크하여 시작할수 없음.
		if( pQuestTemplet->m_Condition.m_setUnitClass.find( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == pQuestTemplet->m_Condition.m_setUnitClass.end() )
		{
			START_LOG( cerr, L"QUEST 수행가능 유닛이 아님.! 누구냐.. 넌." )
				<< BUILD_LOG( pQuestTemplet->m_Condition.m_setUnitClass.size() )
				<< BUILD_LOGc( spUser->GetUnitClass() );

			SET_ERROR( ERR_QUEST_05 );
			goto error_proc;
		}
	}

	// To do...
	//[ing]QUEST 수행을 위해 모아야할 아이템 체크
	//if( pQuestTemplet->m_Condition.m_iConditionItemID)

	//QUEST 수행을 위해 이전에 완료해야할 퀘스트 검사.
	BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
	{
		if( GetQuestCompleteCount( iBeforeQuestID ) == 0 )
		{
			START_LOG( clog, L"어여가서 하나 더해오셔.. !!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iBeforeQuestID )
				;

			SET_ERROR( ERR_QUEST_06 );
			goto error_proc;
		}
	}

	//지정된 npc에게 퀘스트를 받아야함
#ifdef SERV_EPIC_QUEST
	//# 에픽은 자동수락이라 지정된 npc가 존재하지 않음.
	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_eStartNPCID != kReq.m_iTalkNPCID &&
		kReq.m_iTalkNPCID != CXSLUnitManager::NUI_BILLBOARD
		)
	{
		START_LOG( cerr, L"잘못된 NPC에게 QUEST를 받았음..모지..?" )
			<< BUILD_LOG( pQuestTemplet->m_eStartNPCID )
			<< BUILD_LOG( kReq.m_iTalkNPCID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_07 );
		goto error_proc;
	}
#else SERV_EPIC_QUEST
	if( pQuestTemplet->m_eStartNPCID != kReq.m_iTalkNPCID )
	{
		START_LOG( cerr, L"잘못된 NPC에게 QUEST를 받았음..모지..?" )
			<< BUILD_LOG( pQuestTemplet->m_eStartNPCID )
			<< BUILD_LOG( kReq.m_iTalkNPCID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_07 );

		goto error_proc;
	}
#endif SERV_EPIC_QUEST

	//레벨 검사
	if( pQuestTemplet->m_Condition.m_iLevel	> spUser->GetLevel() )
	{
		START_LOG( clog, L"레벨 좀 더 올려봐봐..ㅡㅡ^" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_12 );
		goto error_proc;
	}

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	// 일일 랜덤 퀘스트 인가?
	if( pQuestTemplet->m_iRandomGroupID > 0 )
	{
		int iTodayQuest = SiCXSLQuestManager()->GetTodayRandomQuest( pQuestTemplet->m_iRandomGroupID );

		if( iTodayQuest <= 0 || pQuestTemplet->m_iID != iTodayQuest )
		{
			START_LOG( clog, L"오늘 할 수 있는 일일 랜덤 퀘스트가 아니다!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( pQuestTemplet->m_iRandomGroupID )
				<< BUILD_LOGc( iTodayQuest )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_26 ); 
			goto error_proc;
		}
	}
#endif SERV_RANDOM_DAY_QUEST
	//}}


	{
		KDBE_NEW_QUEST_REQ kDBReq;

		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		if( pQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// 퀘스트 수락 보상이 인벤토리에 남아 있으면 퀘스트 수락 못함
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"퀘스트 수락시 받는 보상이 이미 인벤토리에 있으면 퀘스트 수락 못함." )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_24 );
					goto error_proc;
				}
			}

			// 퀘스트 수락 보상이 들어갈만한 빈슬롯이 있는지 검사
			if( spUser->IsEnoughSpaceExist( pQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"퀘스트 수락시 받는 보상이 들어갈 인벤토리 빈공간이 부족합니다." )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< BUILD_LOG( pQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_25 );
				goto error_proc;
			}

			std::map< int, KItemInfo > mapReward;

			std::map< int, int >::const_iterator mit;
			for( mit = pQuestTemplet->m_mapAcceptRewardItem.begin(); mit != pQuestTemplet->m_mapAcceptRewardItem.end(); ++mit )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"아이템 템플릿 포인터 이상.!" )
						<< BUILD_LOG( mit->first )
						<< END_LOG;
					continue;
				}

				KItemInfo kNewItem;
				kNewItem.m_iItemID = mit->first;
				kNewItem.m_iQuantity = mit->second;
				kNewItem.m_cUsageType = pItemTemplet->m_PeriodType;
				mapReward.insert( std::make_pair( mit->first, kNewItem ) );

				// 통계 : Local Item
				{
					KStatisticsKey kKey;
					kKey.m_vecIntKey.push_back( mit->first );
					KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_QUEST, mit->second );
					//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
					KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_QUEST, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
					//}}
				}

				// 유저 통계 습득 아이템
				spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Obtained_Item, mit->second );

				//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
				// 아이템 어뷰저
				spUser->CheckItemAbuser( KAbuserLogManager::RS_QUEST, kDBReq.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
				//}}
			}

			LIF( spUser->m_kInventory.PrepareInsert( mapReward, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo ) );

			// DB업데이트
			spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
			spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
		}
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_iTalkNPCID	= kReq.m_iTalkNPCID;
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		// 안쓰는 값이지만 그래도 초기화용으로 해준다.
		kDBReq.m_UserUID	= spUser->GetUserUID();
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		spUser->SendToGameDB( DBE_NEW_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  최육사  DB 통계 퀘스트
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Accept, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_AcceptLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  최육사  유저 통계
		int iColumnIndex;
		//{{ 2010. 04. 01  최육사	비밀던전 헬모드
#ifdef SERV_DAILY_QUEST
		if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
#else
		if( pQuestTemplet->m_bRepeat == false )
#endif SERV_DAILY_QUEST
			//}}
		{
			iColumnIndex = KUserStatistics::US_Etc_NQuestAccept;
		}
		else
		{
			iColumnIndex = KUserStatistics::US_Etc_RQuestAccept;
		}

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return true;

error_proc:
	kPacket.m_iOK	= NetError::GetLastError();

	//{{ 2011. 01. 03	최육사	에픽 퀘스트
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	if( bSendAck )
#endif SERV_EPIC_QUEST_CHECK_UPDATE
		//}}
	{
		spUser->SendPacket( EGS_NEW_QUEST_ACK, kPacket );
	}

	return false;
}
#endif SERV_REFORM_QUEST
//}}

void KUserQuestManager::Handler_EGS_GIVE_UP_QUEST_REQ( IN KEGS_GIVE_UP_QUEST_REQ& kReq, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	//수행중인 퀘스트가 없는데 포기할려고 할경우
	if( IsQuest( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_02 );
		goto error_proc;
	}

	{
		KDBE_GIVE_UP_QUEST_REQ kDBReq;
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		spUser->SendToGameDB( DBE_GIVE_UP_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  최육사  DB 통계 퀘스트
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_GiveUp, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_GiveUpLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  최육사  유저 통계
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 퀘스트ID입니다." )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< END_LOG;
			return; // 통계값을 처리하지 않고 리턴
		}

		int iColumnIndex;
		//{{ 2010. 04. 01  최육사	비밀던전 헬모드
#ifdef SERV_DAILY_QUEST
		if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
#else
		if( pQuestTemplet->m_bRepeat == false )
#endif SERV_DAILY_QUEST
		//}}
		{
			iColumnIndex = KUserStatistics::US_Etc_NQuestGiveUp;
		}
		else
		{
			iColumnIndex = KUserStatistics::US_Etc_RQuestGiveUp;
		}

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return;

error_proc:
	KEGS_GIVE_UP_QUEST_ACK kAck;
	kAck.m_iOK = NetError::GetLastError();
	spUser->SendPacket( EGS_GIVE_UP_QUEST_ACK, kAck );
}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::Handler_ERM_NPC_UNIT_DIE_NOT( IN const int iDungeonID,
													 IN const char cDifficulty,
													 IN const int iMonsterID,
													 IN KGSUserPtr spUser,
													 IN const char cDungeonMode
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
													 , IN const bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST
													 )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() )
			continue;

		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType 
		 &&	pQuestTemplet->m_iPlayLevel > spUser->GetLevel()	)
			continue;

		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_NPC_HUNT )
				continue;

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
			if( pSubQuestTemplet->m_iID == PLAY_TOGETHER_KILL_FIELD_BOSS_SUBQUEST_ID && bHasFriend == false )
			{
				continue;
			}
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
			// 라이딩 펫 타고 필드보스 3마리 잡기
			if( pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID1)
			{
				// 라이딩 펫 타고 있으면
				int iOnRidingPet = spUser->GetiRidingPetSummoned();

				//START_LOG( clog2, L"김석근_라이딩펫 이벤트 서브 퀘스트2!" )
				//	<<BUILD_LOG( iOnRidingPet )
				//	<<BUILD_LOG( pSubQuestTemplet->m_iID )
				//	<<END_LOG;

				// 이벤트용 라이딩 펫 타고 있지 않으면
				if(iOnRidingPet != 2)
					continue;
			}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

			//{{ 2009. 8. 3  최육사		퀘스트 조건 수정
			if( pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iMonsterID) ) == pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.end() )
				continue;
			//}}

			if( spUser->GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( spUser->GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )  // 배틀 필드라면
			{
				// 유효한 배틀 필드라면
				if( SiCXSLQuestManager()->CheckValidBattleFiledID( pQuestTemplet->m_vecSubQuest[iIndexSub] ) == true ) 
				{
					// SubQuest 에 있는 배틀 필드을 플레이 한 것이 아니라면
					if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[iIndexSub], spUser->GetMapID() ) == false )
					{
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// 작업날짜: 2013-11-28	// 박세훈
						if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
						{
							START_LOG( cerr, L"필드 보스 시스템 퀘스트 로그: 엘트리온을 잡았는데 보스 필드가 아니다?" )
								<< BUILD_LOG( spUser->GetCharUID() )
								<< BUILD_LOG( spUser->GetCharName() )
								<< BUILD_LOG( spUser->GetRoomUID() )
								<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
								<< BUILD_LOG( spUser->GetMapID() )
								<< END_LOG;
						}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
						continue;
					}
				}
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// 작업날짜: 2013-11-28	// 박세훈
				else if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"필드 보스 시스템 퀘스트 로그: 엘트리온을 잡았는데 보스 필드가 아니다?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
			}
			else
			{
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// 작업날짜: 2013-11-28	// 박세훈
				if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"필드 보스 시스템 퀘스트 로그: 엘트리온을 잡았는데 보스 필드가 아니다?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
				if( ( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID ) && ( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode ) )
					continue;
				
				// 유효한 던전이라면
				if( SiCXSLQuestManager()->CheckValidDungeonID( pQuestTemplet->m_vecSubQuest[iIndexSub] ) == true )
				{
					// SubQuest 에 있는 던전을 플레이 한 것이 아니라면
					if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[iIndexSub], iDungeonID + static_cast<int>(cDifficulty), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
						continue;
				}
			}
			
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == false )
			{
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// 작업날짜: 2013-11-28	// 박세훈
				if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"필드 보스 시스템 퀘스트 로그: 엘트리온을 잡았는데 하위 그룹 sub quest가 완료되지 않았다?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
				continue;
			}

#ifdef SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
			//일반 모드만
			if( pSubQuestTemplet->m_iID == 131141 || pSubQuestTemplet->m_iID == 131190 ||
				pSubQuestTemplet->m_iID == 131200 || pSubQuestTemplet->m_iID == 131210  )
			{
				if( CXSLDungeon::IsHenirChallengeMode( static_cast<int>( cDungeonMode ) ) == true )
				{
					continue;
				}
			} 
			//도전 모드만
			else if( pSubQuestTemplet->m_iID == 131151 || pSubQuestTemplet->m_iID == 131160 ||
				pSubQuestTemplet->m_iID == 131170 || pSubQuestTemplet->m_iID == 131180 || pSubQuestTemplet->m_iID == 131220 )
			{
				if( CXSLDungeon::IsHenirPracticeMode( static_cast<int>( cDungeonMode ) ) == true )
				{
					continue;
				}
			}
#endif SERV_HENIR_EVENT_SORT_NORMAL_CHALLENGE
			if( pSubQuestTemplet->m_ClearCondition.m_iKillNum > kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
			{
				++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;

				if( pSubQuestTemplet->m_ClearCondition.m_iKillNum <= kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// 작업날짜: 2013-11-28	// 박세훈
				else if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
				{
					START_LOG( cerr, L"필드 보스 시스템 퀘스트 로그: 엘트리온을 잡았는데 1회가 아니다?" )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
						<< BUILD_LOG( spUser->GetMapID() )
						<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
						<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						<< END_LOG;
				}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"퀘스트 SQT_NPC_HUNT 수행" )
					<< BUILD_LOG( iMonsterID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess );
			}
#ifdef SERV_BATTLE_FIELD_BOSS_QUEST_LOG// 작업날짜: 2013-11-28	// 박세훈
			else if( iMonsterID == CXSLUnitManager::NUI_FIELD_BOSS_IRON_ELTRION )
			{
				START_LOG( cerr, L"필드 보스 시스템 퀘스트 로그: 퀘스트 완료하지 않으시고 엘트리온 잡으신 듯" )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetRoomUID() )
					<< BUILD_LOG( CXSLRoom::GetRoomType( spUser->GetRoomUID() ) )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< END_LOG;
			}
#endif // SERV_BATTLE_FIELD_BOSS_QUEST_LOG
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

#else		// SERV_REFORM_QUEST

//{{ 2011. 08. 31  김민성	비밀던전 헬모드
//#ifdef SERV_RANDOM_DAY_QUEST
void KUserQuestManager::Handler_ERM_NPC_UNIT_DIE_NOT( IN const int iDungeonID, IN const char cDifficulty, IN const int iMonsterID, IN KGSUserPtr spUser, IN const char cDungeonMode )
//#else
//void KUserQuestManager::Handler_ERM_NPC_UNIT_DIE_NOT(IN int iDungeonID, IN char cDifficulty, IN int iMonsterID, IN KGSUserPtr spUser )
//#endif SERV_RANDOM_DAY_QUEST
//}}
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() )
			continue;

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_NPC_HUNT )
				continue;

			//{{ 2009. 8. 3  최육사		퀘스트 조건 수정
			if( pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iMonsterID) ) == pSubQuestTemplet->m_ClearCondition.m_setKillNPCID.end() )
				continue;
			//}}

			//던전 조건이 있는지
			if( pSubQuestTemplet->m_ClearCondition.m_eDungeonID > 0 )
			{
				//{{ 2011. 08. 31  김민성	비밀던전 헬모드
#ifdef SERV_RANDOM_DAY_QUEST
				// 비밀던전이라면..
				if( CXSLDungeon::IsSecretDungeon( pSubQuestTemplet->m_ClearCondition.m_eDungeonID ) == true )
				{
					// 해당 비밀던전이 맞는지 검사
					if( ( pSubQuestTemplet->m_ClearCondition.m_eDungeonID / 10 ) != ( iDungeonID / 10 ) )
						continue;

					// 난이도 검사
					if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID )
					{
						if( pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubQuestTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode )
								continue;
						}
					}
				}
				else
				{
					if( pSubQuestTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
						continue;

					//그리고 난이도 조건이 있는지 확인한다.
					if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty >= 0 )
					{
						//{{ 2009. 3. 31  최육사	특정 난이도 이상을 조건에 포함
						if( pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
								continue;
						}
						else
						{
							if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
								continue;
						}
						//}}
					}
				}

#else
				//				if( pSubQuestTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
				//					continue;
				//
				//				//그리고 난이도 조건이 있는지 확인한다.
				//				if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty >= 0 )
				//				{
				//					//{{ 2009. 3. 31  최육사	특정 난이도 이상을 조건에 포함
				//					if( pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty == false )
				//					{
				//						if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
				//							continue;
				//					}
				//					else
				//					{
				//						if( pSubQuestTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
				//							continue;
				//					}
				//					//}}
				//				}
#endif SERV_RANDOM_DAY_QUEST
				//}}
			}

			//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
			if( pSubQuestTemplet->m_ClearCondition.m_iVillageMapID > 0 )
			{
				if( pSubQuestTemplet->m_ClearCondition.m_iVillageMapID != spUser->GetMapID() )
					continue;
			}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
			//}}

			//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
			if( pSubQuestTemplet->m_ClearCondition.m_iBattleFieldID > 0 )
			{
				if( pSubQuestTemplet->m_ClearCondition.m_iBattleFieldID != spUser->GetMapID() )
					continue;
			}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
			//}}

			if( pSubQuestTemplet->m_ClearCondition.m_iKillNum > kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
			{
				++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;

				if( pSubQuestTemplet->m_ClearCondition.m_iKillNum <= kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"퀘스트 SQT_NPC_HUNT 수행" )
					<< BUILD_LOG( iMonsterID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess );
			}
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_REFORM_QUEST
//}}

void KUserQuestManager::Handler_EGS_TALK_WITH_NPC_REQ( IN int iNPCID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT	kNot;

	std::map<int, KQuestInstance>::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); mit++ )
	{
		KQuestInstance* pkQuestInstance = &(mit->second);

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );

		if( pQuestTemplet != NULL )
		{

#ifdef SERV_EPIC_QUEST
			// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
			if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
				pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
				)
				continue;
#endif SERV_EPIC_QUEST

			if( pQuestTemplet->m_vecSubQuest.empty() == false )
			{
				for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
				{
					const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );

					if( pSubQuestTemplet != NULL )
					{
						if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_NPC_TALK )
							continue;

						//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
						// 특정 Village 에서만 완료 가능하다면
						if( pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.size() > 0 )
						{
							// Village 검색하자
							std::set<int>::const_iterator sit = pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.find( spUser->GetMapID() );
							if( sit == pSubQuestTemplet->m_ClearCondition.m_setVillageMapID.end() )
							{
								continue;							
							}
						}
#endif SERV_REFORM_QUEST
						//}}

						if( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID != iNPCID )
							continue;

						//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
						// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
						if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == false )
							continue;
#endif SERV_REFORM_QUEST
						//}}

#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
						// 1차전직이 아니면
						if( pSubQuestTemplet->m_iID == _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBadCharQuestLevel1 )
						{
							if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == false && CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == false)
								continue;
						}
						// 2차전직이 아니면
						else if( pSubQuestTemplet->m_iID == _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBadCharQuestLevel2 )
						{
							if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass()) ) == false )
								continue;
						}
						// 50레벨이 아니면
						else if( pSubQuestTemplet->m_iID == _CONST_BURNING_CHAR_EVENT_SUB_QUEST_::iBadCharQuestLevel3 )
						{
							if( spUser->GetLevel() <= 49)
								continue;
						}
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
					
						if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData == 0 )
						{
							pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData = 1;
							pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

							kNot.m_vecQuestInst.push_back( *pkQuestInstance );

							START_LOG( clog, L"퀘스트 SQT_NPC_TALK 수행" )
								<< BUILD_LOG( iNPCID )
#ifdef SERV_PRIVACY_AGREEMENT
								<< BUILD_LOG( spUser->GetCharUID() )
								<< BUILD_LOG( spUser->GetUID() )
#else
								<< BUILD_LOG( spUser->GetCharName() )
								<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
								<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
								<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
								<< BUILD_LOGc( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
								<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
								;
						}
						else
							continue;
					}
					else
					{
						START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
							<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
							<< BUILD_LOG( spUser->GetCharUID() )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;

						SET_ERROR( ERR_QUEST_08 );
					}
				}
			}
		}
		else
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::Handler_OnDungeonClear( IN const KGSUserPtr spUser, 
											   IN const int iDungeonID, 
											   IN const char cDifficulty, 
											   IN const char cDungeonMode, 
											   IN const char cRank, 
											   IN const int iPlayTime, 
											   IN const int iDamage, 
											   IN const std::set< UidType >& setPartyMembers, 
											   IN const std::map< int, KSubQuestInfo >& mapOngoingQuest,
											   //{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
											   IN const std::map< UidType, bool >& mapHaveExpInDungeon,
											   IN const std::map< UidType, bool >& mapSuitableLevelInfo
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
											   ,IN const bool bSChar1
											   ,IN const bool bSChar2
#endif //SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
											   ,IN const std::vector<UidType> vecRelation
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
											   ,IN const bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST
#ifdef SERV_THREE_COLOR_EVENT
											   ,IN const std::set< int >& setEquippedTitle
#endif SERV_THREE_COLOR_EVENT
											   )
#else
											   IN const std::map< UidType, bool >& mapHaveExpInDungeon )
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
											   //}}
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType 
		 &&	pQuestTemplet->m_iPlayLevel > spUser->GetLevel()	)
			continue;

		for( u_int iIndexSub = 0; iIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const int iSubQuestID = pQuestTemplet->m_vecSubQuest[iIndexSub];

			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			bool bRVal = false;
			bool bValidDungeonID = false;

			// 유효한 던전이라면
			if( SiCXSLQuestManager()->CheckValidDungeonID( iSubQuestID ) == true ) 
			{
				bValidDungeonID = true;

				// SubQuest 에 있는 던전을 플레이 한 것이 아니라면
				if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( iSubQuestID, iDungeonID + static_cast<int>(cDifficulty), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
					continue;
			}

			switch( pSubQuestTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_DUNGEON_TIME:
				{
					if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearTime >= iPlayTime )
						bRVal = true;
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_RANK:
				{
					if( (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
					{
#ifdef SERV_EVENT_TITLE_SUBQUEST
						if(pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1)
						{
							if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
							{
								bRVal = true;
							}
							else
							{
								bRVal = false;
							}
						}
						else
						{
							bRVal = true;
						}
#else //SERV_EVENT_TITLE_SUBQUEST
						bRVal = true;
#endif SERV_EVENT_TITLE_SUBQUEST
					}
				}
				break;

			case CXSLQuestManager::SQT_DUNGEON_DAMAGE:
				{
					if( pSubQuestTemplet->m_ClearCondition.m_iDungeonDamage >= iDamage )
						bRVal = true;
				}
				break;

#ifndef DEPRECATED_SERVER_GROUP_MASK
			case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
				{
					BOOST_TEST_FOREACH( const UidType, iPartyMemberUnitUID, setPartyMembers )
					{
						if( iPartyMemberUnitUID == spUser->GetCharUID() )
							continue;

						// 다른 서버군 유저와 플레이한것이라면..?
						if( spUser->IsSameServerGroupUID( iPartyMemberUnitUID ) == false )
						{
							bRVal = true;
						}
					}
				}
				break;
#endif DEPRECATED_SERVER_GROUP_MASK

			case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
				{
					if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT  ||
						pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_PCBANG )
					{

						if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false && 
							CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
						{
							 
							if( bValidDungeonID == false )
							{
								if( ExistHaveExpInDungeon( spUser->GetCharUID(), mapHaveExpInDungeon ) )
								{
#ifdef SERV_EVENT_TITLE_SUBQUEST		
										//칭호에 관한 서브퀘스트 조건이 있다.
										if(pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1 && pSubQuestTemplet->m_ClearCondition.m_eDungeonRank == CXSLDungeon::RT_NONE) 
										{
											if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
											{
												bRVal = true;
											}
											else
											{
												bRVal = false;
											}
										}
										//랭크에 관한 서브퀘스트 조건이 있다.
										else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID == -1)
										{
											if( (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
											{
												bRVal = true;
											}
											else
											{
												bRVal = false;
											}
										}
										//칭호 랭크에 서브퀘스트 조건이 있다.
										else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1) 
										{
											if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID && (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank)
											{
												bRVal = true;
											}
											else
											{
												bRVal = false;
											}
										}
										else
										{
											bRVal = true;
										}
#else //SERV_EVENT_TITLE_SUBQUEST
									bRVal = true;
#endif SERV_EVENT_TITLE_SUBQUEST
								}
							}
							else
							{
#ifdef SERV_EVENT_TITLE_SUBQUEST	
									//칭호에 관한 서브퀘스트 조건이 있다.
									if(pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1 && pSubQuestTemplet->m_ClearCondition.m_eDungeonRank == CXSLDungeon::RT_NONE) 
									{
										if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
										{
											bRVal = true;
										}
										else
										{
											bRVal = false;
										}
									}
									//랭크에 관한 서브퀘스트 조건이 있다.
									else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID == -1)
									{
										if( (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
										{
											bRVal = true;
										}
										else
										{
											bRVal = false;
										}
									}
									//칭호 랭크에 서브퀘스트 조건이 있다.
									else if(pSubQuestTemplet->m_ClearCondition.m_eDungeonRank != CXSLDungeon::RT_NONE && pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1) 
									{
										if (  spUser->GetEquippedTitleID() == pSubQuestTemplet->m_ClearCondition.m_iUseTitleID && (char)pSubQuestTemplet->m_ClearCondition.m_eDungeonRank <= cRank)
										{
											bRVal = true;
										}
										else
										{
											bRVal = false;
										}
									}
									else
									{
										bRVal = true;
									}
#else //SERV_EVENT_TITLE_SUBQUEST
								bRVal = true;
#endif SERV_EVENT_TITLE_SUBQUEST
							}
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
							if( pQuestTemplet->m_iID == 63780 )
							{
								bRVal = false;

								for(int i = 0; i < vecRelation.size(); i++ )
								{
									if( vecRelation[i] == spUser->GetCharUID() )
									{
										bRVal = true;
										break;
									}
								}					
							}
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
							START_LOG( cout, L"추천인 같이 돌기 서브퀘스트다!" )
								<< BUILD_LOG( pSubQuestTemplet->m_iID )
								<< BUILD_LOG( bRVal )
								<< BUILD_LOG( bHasFriend )
								<< END_LOG;

							if( pSubQuestTemplet->m_iID == PLAY_TOGETHER_DUNGEON_CLEAR_SUBQUEST_ID )
							{
								bRVal = bHasFriend;
							}
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_THREE_COLOR_EVENT
							if( pQuestTemplet->m_iID == 63900 )
							{
								bRVal = false;
								
								if( setEquippedTitle.size() == 3 && setPartyMembers.size() >= 3 )
								{
									bRVal = true;
								}
							}
#endif SERV_THREE_COLOR_EVENT
						}
					}				
					else
					{
						//던전 클리어 서브는 승리만 하면 되기때문에 이곳에서 처리
						//## 초심자의 숲만 예외처리한다.
						if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL && 
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
						{
							bRVal = true;
						}
					}
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
					for( int i = 0; i < _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::iSubQuestMaxNum; ++i )
					{
						if( pSubQuestTemplet->m_iID == _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::aSubQuestID[i] )
						{
							if( setPartyMembers.size() >= _CONST_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT_::cRequiredMembers )
							{
								// 충족
								bRVal = bSChar1 & bSChar2;
							}
							else
							{
								bRVal = false;
							}
						}
					}
#endif SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT

#ifdef SERV_PET_WITH_DUNGEON_CLEAR_COUNT
					if( pSubQuestTemplet->m_iID == 85560 )
					{
						KPetInfo kPetInfo;
						const UidType iSummonedPetUID = spUser->GetSummonedPetUID();
						kPetInfo = spUser->GetPetInfo();
											
						if( iSummonedPetUID != 0 && kPetInfo.m_iPetID == 30004 && kPetInfo.m_cEvolutionStep == 3 )
						{
							bRVal = true;
						}
						else
						{
							bRVal = false;
						}
					}
#endif
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
					// 라이딩 펫 타고 3회던전 클리어
					if( pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID2 || pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID3 )
					{
						// 라이딩 펫 타고 있으면
						int iOnRidingPet = spUser->GetiRidingPetSummoned();
						bool bOnRidingPet = false;

						if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false && 
							CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
						{
							if( ExistHaveExpInDungeon( spUser->GetCharUID(), mapHaveExpInDungeon ) )
							{
								if( pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID2 )
								{
									// 이벤트용 라이딩 펫
									if(iOnRidingPet == 2)
									{
										bOnRidingPet = true;
									}
								}
								else if(pSubQuestTemplet->m_iID == _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iOnEventRidingPetQuestID3)
								{
									// 캐시용 라이딩 펫
									if(iOnRidingPet == 1)
									{
										bOnRidingPet = true;
									}
								}

								
							}
						}

						bRVal = bOnRidingPet;

						//START_LOG( clog2, L"김석근_라이딩펫 이벤트 서브 퀘스트!" )
							//<<BUILD_LOG( pSubQuestTemplet->m_iID )
							//<<BUILD_LOG( iOnRidingPet )
							//<<BUILD_LOG( bRVal )
							//<<BUILD_LOG( iDungeonID )
							//<<END_LOG;
					}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST
#ifdef SERV_RIDING_PET_WITH_SUB_QUEST
					// 특정 라이딩 펫 타고 던전 클리어
					if( pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::iHaveSubQuestID || pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::iHaveSubQuestID )
					{
						// 라이딩 펫 타고 있으면
						int iOnRidingPetID = spUser->GetiRidingSummonedPetID();
						bool bOnRidingPet = false;

						if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false && 
							CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
						{
							if( ExistHaveExpInDungeon( spUser->GetCharUID(), mapHaveExpInDungeon ) )
							{
								if( pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::iHaveSubQuestID )
								{
									if(_CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::bAllRidingPet == true)
									{
										bOnRidingPet = true;
									}
									else
									{
										if( _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_1::iRidingPetID == iOnRidingPetID)
											bOnRidingPet = true;
									}
									
								}
								else if(pSubQuestTemplet->m_iID == _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::iHaveSubQuestID)
								{
									if(_CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::bAllRidingPet == true)
									{
										bOnRidingPet = true;
									}
									else
									{
										if( _CONST_RIDING_PET_WITH_SUB_QUEST::RIDING_SET_2::iRidingPetID == iOnRidingPetID)
											bOnRidingPet = true;
									}
								}
							}
						}

						bRVal = bOnRidingPet;

						//START_LOG( clog2, L"김석근_라이딩펫 이벤트 서브 퀘스트!" )
						//<<BUILD_LOG( pSubQuestTemplet->m_iID )
						//<<BUILD_LOG( iOnRidingPetID )
						//<<BUILD_LOG( bRVal )
						//<<BUILD_LOG( iDungeonID )
						//<<END_LOG;
					}
#endif //SERV_RIDING_PET_WITH_SUB_QUEST
				}
				break;
				//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
				{
					if( CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
						CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&								
						CXSLDungeon::IsTCDungeon( iDungeonID ) == false &&
						//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
						CXSLDungeon::IsValentineDungeon( iDungeonID ) == false &&
#endif SERV_EVENT_VALENTINE_DUNGEON
						//}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
						CXSLDungeon::IsValentineDungeonInt( iDungeonID ) == false &&
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / 강정훈
						CXSLDungeon::IsHalloweenDungeon( iDungeonID ) == false &&
#endif //SERV_HALLOWEEN_EVENT_2013
						CXSLDungeon::IsRubenDungeon( iDungeonID ) == false )
					{

						// 적정 레벨 던전을 플레이 한 것이 맞는가?
						if( IsSuitableLevelUser( spUser->GetCharUID(), mapSuitableLevelInfo ) == true )
						{
#ifdef SERV_THREE_COLOR_EVENT
							if( pQuestTemplet->m_iID == 63870 )
							{
								if( setPartyMembers.size() == 1 && spUser->GetEquippedTitleID() == 5470 )
								{
									bRVal = true;
								}
							}	
							else if( pQuestTemplet->m_iID == 63880 )
							{
								if( setPartyMembers.size() == 1 && spUser->GetEquippedTitleID() == 5480 )
								{
									bRVal = true;
								}			
							}
							else if( pQuestTemplet->m_iID == 63890 )
							{
								if( setPartyMembers.size() == 1 && spUser->GetEquippedTitleID() == 5490 )
								{
									bRVal = true;
								}			
							}
							else
							{
								bRVal = true;
							}
#else
							bRVal = true;
#endif SERV_THREE_COLOR_EVENT
						}
					}
				}
				break;
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
				//}}

			case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
				{
					if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT  ||
						pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_PCBANG )
					{
						if( iDungeonID != SEnum::DI_ELDER_HALLOWEEN_NORMAL  &&
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsHenirDungeon( iDungeonID ) == false  &&
							CXSLDungeon::IsTCDungeon( iDungeonID ) == false )
						{
							// 던전 지정이 되어있지 않다면
							if( bValidDungeonID == false )
							{
								// 경험치를 획득할 수 있는 던전인지 체크합니다.
								if( spUser->IsDungeonGetExp( iDungeonID, cDifficulty ) == true )
								{
									bRVal = true;
								}
							}
							// 던전 지정이 되어있다면?
							else
							{
								bRVal = true;
							}
						}
					}				
					else
					{
						//던전 클리어 서브는 승리만 하면 되기때문에 이곳에서 처리
						//## 초심자의 숲만 예외처리한다.
						if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL && 							
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
						{
							bRVal = true;
						}
					}					
				}
			
				if( bRVal )
				{
					bRVal = false;

					// 아이템을 장착한채 던전을 클리어 했는지 확인해보자!
					std::map< int, KSubQuestInfo >::const_iterator mitQ;
					mitQ = mapOngoingQuest.find( kQuestInstance.m_iID );
					if( mitQ == mapOngoingQuest.end() )
					{
						std::map< int, KSubQuestData >::const_iterator mitSQ;
						mitSQ = mitQ->second.m_mapSubQuestInfo.find( iSubQuestID );
						if( mitSQ != mitQ->second.m_mapSubQuestInfo.end() )
						{
							if( mitSQ->second.m_iInInventoryItemCount == 1 )
							{
								bRVal = true;
							}
						}
					}
				}
				break;
#ifdef SERV_POINT_COUNT_SYSTEM
			case CXSLQuestManager::SQT_POINT_COUNT:
				{
					//여기서 성공 했는지 안했는지 체크해 주면 된다.
					//일단 던전 클리어 하면 무조건 성공이지
					 bRVal = true;
				}
				break;
#endif SERV_POINT_COUNT_SYSTEM

				// 하위 그룹의 sub Quest 가 모두 완료 된 상태라면
				if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == true )
					bRVal = true;
				
			}

			if( bRVal == true )
			{
				

				switch( pSubQuestTemplet->m_eClearType )
				{
				case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
				case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
				case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
					//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
				case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
					//}}
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
				case CXSLQuestManager::SQT_DUNGEON_RANK:
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
					{
						//{{ 2010. 05. 03  최육사	던전 클리어 카운트 버그 수정
						// 클리어 조건 카운트를 넘지 않게 보정한다.
						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
						{
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData += 1;
						}
						//}}

						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

						kNot.m_vecQuestInst.push_back( kQuestInstance );
					}
					break;
#ifdef SERV_POINT_COUNT_SYSTEM
				case CXSLQuestManager::SQT_POINT_COUNT:
					{
						//조건은 던전 클리어 카운트 와 비슷하지만 점수가 넘어도 상관 없다.
						if( pSubQuestTemplet->m_ClearCondition.m_iPointCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
						{
							//현재는 이벤트 용으로 쓰고 있기 때문에 포인트를 얻어오는 정확한 변수가 없습니다.
							//추후 제가 추가를 하겠습니다.
						}
						//해당 퀘스트에 포인트가 넘었을 경우에 성공으로 간주 한다.
						if( pSubQuestTemplet->m_ClearCondition.m_iPointCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData) )
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

						kNot.m_vecQuestInst.push_back( kQuestInstance );
					}
					break;
#endif SERV_POINT_COUNT_SYSTEM


				default:
					{
						if( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData == 0 )
						{
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData = 1;
							kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

							kNot.m_vecQuestInst.push_back( kQuestInstance );
						}
					}
					break;
				}

				START_LOG( clog, L"퀘스트 SQT_DUNGEON_TIME or SQT_DUNGEON_RANK 수행" )
					<< BUILD_LOGc( cRank )
					<< BUILD_LOG( iPlayTime )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
					;
			}
		}		
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

#endif SERV_REFORM_QUEST
//}}

void KUserQuestManager::Handler_OnPVPPlay( IN int iGameType
										 , IN KGSUserPtr spUser
										 //{{ 2012. 09. 18   김민성   대전 플레이 퀘스트 조건 변경
#ifdef SERV_PVP_PLAY_QUEST
										 , IN bool bOfficialMatch
#endif SERV_PVP_PLAY_QUEST
										 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
										 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
										 , IN bool bCouplePvp
										 , IN bool bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
										 , IN const bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST
										 )
{
	//{{ 2010. 05. 19  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	const int iPvpChannelClass = SiKRoomListManager()->GetPVPChannelClassByRoomListID( spUser->GetRoomListID() );
	if( iPvpChannelClass == KPVPChannelInfo::PCC_PLAY )
	{
		START_LOG( clog, L"연습채널은 퀘스트 클리어 조건에 만족되지 않음." );
		return;
	}
#endif SERV_INTEGRATION
	//}}

	OnPVP( CXSLQuestManager::SQT_PVP_PLAY
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		 //{{ 2012. 09. 18   김민성   대전 플레이 퀘스트 조건 변경
#ifdef SERV_PVP_PLAY_QUEST
		 , bOfficialMatch
#else
		 , fasle
#endif SERV_PVP_PLAY_QUEST
		 //}}
#endif SERV_NEW_PVP_QUEST
		 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
		 , iPvpChannelClass
		 , bCouplePvp
		 , bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		 , bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );

#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
	OnPVP( CXSLQuestManager::SQT_PVP_HERO_NPC_PLAY
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		 //{{ 2012. 09. 18   김민성   대전 플레이 퀘스트 조건 변경
#ifdef SERV_PVP_PLAY_QUEST
		 , bOfficialMatch
#else
		 , false
#endif SERV_PVP_PLAY_QUEST
		 //}}
#endif SERV_NEW_PVP_QUEST
		 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
		 , bHasFriend
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );
#endif //SERV_PVP_NPC_HUNT_QUEST_EXTEND
}

//{{ 2012. 09. 18	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
void KUserQuestManager::Handler_OnPVPPlay_AccumulatedOfficialMatchCount( IN KGSUserPtr spUser, IN const bool bIncreaseCount )

{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	//퀘스트를 돌면서
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance* pkQuestInstance = &mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );
		if( pQuestTemplet == NULL )		
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		//서브퀘스트의 조건을 검사한다.
		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_PVP_PLAY_ARRANGE )	//클리언 조건이 같은지..
				continue;

			if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
				continue;

			// pvp info 테이블의 정보를 덮어 씌우기만 한다.
			pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData = spUser->GetOfficialMatchCount();
			if( bIncreaseCount == true )
			{
				++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iPVPPlay <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
				pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( *pkQuestInstance );

			START_LOG( clog, L"공식 대전 퀘스트 수행!" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iPVPPlay )
				<< BUILD_LOG( spUser->GetOfficialMatchCount() )
				<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_2012_PVP_SEASON2
//}}

void KUserQuestManager::Handler_OnPVPWin( IN int iGameType
										, IN KGSUserPtr spUser
										//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
										//{{ 2012. 02. 22	김민성	대전 퀘스트, PVP NPC 관련 퀘스트	오류 수정
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
										, IN bool bMatchNPC
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
										//}}
										, IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
										//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
										, IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
										)
{
	//{{ 2010. 05. 19  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	const int iPvpChannelClass = SiKRoomListManager()->GetPVPChannelClassByRoomListID( spUser->GetRoomListID() );
	if( iPvpChannelClass == KPVPChannelInfo::PCC_PLAY )
	{
		START_LOG( clog, L"연습채널은 퀘스트 클리어 조건에 만족되지 않음." );
		return;
	}
#endif SERV_INTEGRATION
	//}}

	//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
//{{ 2012. 02. 22	김민성	대전 퀘스트, PVP NPC 관련 퀘스트	오류 수정
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
	OnPVP( CXSLQuestManager::SQT_PVP_WIN
		   , iGameType
		   , spUser
		   , bOfficialMatch
#ifdef PVP_QUEST_HERO_KILL_COUNT	// 2013.02.08 lygan_조성욱 //( SERV_PVP_NPC_HUNT_QUEST_EXTEND 머지 해야 함 ) 기획 툴에서 뽑히는 조건이랑 여기에 걸리는 조건이랑 차이가 있다 기획툴은 win쪽 변수에 값이 있는데 실제 체크는 kill 쪽에서 하고있어서 의미를 하나로 통일 시킨다.
		   , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		   , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		   , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		   );

#ifdef PVP_QUEST_HERO_KILL_COUNT	// 2013.02.08 lygan_조성욱 //( SERV_PVP_NPC_HUNT_QUEST_EXTEND 머지 해야 함 ) 기획 툴에서 뽑히는 조건이랑 여기에 걸리는 조건이랑 차이가 있다 기획툴은 win쪽 변수에 값이 있는데 실제 체크는 kill 쪽에서 하고있어서 의미를 하나로 통일 시킨다.
	OnPVP( CXSLQuestManager::SQT_PVP_NPC_HUNT
		 , iGameType
		 , spUser
		 , bOfficialMatch
		 , bIsHeroNPC
#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );
#endif //PVP_QUEST_HERO_KILL_COUNT

#else
	if( bMatchNPC == true )
	{
		OnPVP( CXSLQuestManager::SUB_QUEST_TYPE::SQT_PVP_NPC_HUNT, iGameType, spUser, bMatchNPC );
	}
	else
	{
		OnPVP( CXSLQuestManager::SUB_QUEST_TYPE::SQT_PVP_WIN, iGameType, spUser, bOfficialMatch );
	}
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
//}}
#else
	OnPVP( CXSLQuestManager::SUB_QUEST_TYPE::SQT_PVP_WIN, iGameType, spUser );
#endif SERV_NEW_PVP_QUEST
	//}} 
}

void KUserQuestManager::Handler_OnPVPKill( IN int iGameType
										 , IN KGSUserPtr spUser
										 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
										 , IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
										 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
										 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
										 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
										 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
										 )
{
	//{{ 2010. 05. 19  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	const int iPvpChannelClass = SiKRoomListManager()->GetPVPChannelClassByRoomListID( spUser->GetRoomListID() );
	if( iPvpChannelClass == KPVPChannelInfo::PCC_PLAY )
	{
		START_LOG( clog, L"연습채널은 퀘스트 클리어 조건에 만족되지 않음." );
		return;
	}
#endif SERV_INTEGRATION
	//}}

	OnPVP( CXSLQuestManager::SQT_PVP_KILL
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		 , bOfficialMatch
#endif SERV_NEW_PVP_QUEST
		 //}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT // 2013.02.08 lygan_조성욱 // SERV_PVP_NPC_HUNT_QUEST_EXTEND 머지 해야 함 
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );

	//{{ 2012. 02. 22	김민성	대전 퀘스트, PVP NPC 관련 퀘스트	오류 수정
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
	OnPVP(
		//{{ 2013. 2. 14	Merge	박세훈	 대전 SQT_PVP_NPC_HUNT 퀘스트 타입에 에픽 NPC 와 플래이, 에픽 NPC 승리조건도 추가
#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
		   CXSLQuestManager::SQT_PVP_HERO_NPC_KILL
#else
		   CXSLQuestManager::SQT_PVP_NPC_HUNT
#endif SERV_PVP_NPC_HUNT_QUEST_EXTEND
		   //}}
		 , iGameType
		 , spUser
		 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		 , bOfficialMatch
#endif SERV_NEW_PVP_QUEST
		 //}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT // 2013.02.08 lygan_조성욱 // SERV_PVP_NPC_HUNT_QUEST_EXTEND 머지 해야 함 
		 , bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		 , 0
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 , 0
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		 );
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
	//}}
}

void KUserQuestManager::OnPVP( IN CXSLQuestManager::SUB_QUEST_TYPE eSubQuestType
							 , IN int iGameType
							 , IN KGSUserPtr spUser
							 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
							 , IN bool bMatch
#endif SERV_NEW_PVP_QUEST
							 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
							 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
							 , IN int iPvpChannelClass
							 , IN bool bCouplePvp
							 , IN bool bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
							 , IN bool bHasFriend /*= false*/
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
							 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
							 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
							 )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	//퀘스트를 돌면서
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance* pkQuestInstance = &mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );
		if( pQuestTemplet == NULL )		
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}
		
		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST
		
		//서브퀘스트의 조건을 검사한다.
		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

#ifdef SERV_RECRUIT_EVENT_SUBQUEST
			if( pSubQuestTemplet->m_iID == PLAY_TOGETHER_PLAY_PVP_SUBQUEST_ID && bHasFriend == false )
			{
				continue;
			}
#endif SERV_RECRUIT_EVENT_SUBQUEST
			
			if( pSubQuestTemplet->m_eClearType != eSubQuestType )	//클리언 조건이 같은지..
				continue;

			if( pSubQuestTemplet->m_ClearCondition.m_ePVPType != CXSLRoom::PGT_NULL )
			{
				if( pSubQuestTemplet->m_ClearCondition.m_ePVPType != iGameType )	//대전타입이 같은지..
					continue;
			}

			//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
			if( pSubQuestTemplet->m_ClearCondition.m_ePvpChannelClass == KPVPChannelInfo::PCC_OFFICIAL )
			{
				if( bMatch != true )
					continue;
			}
#endif SERV_NEW_PVP_QUEST
			//}} 

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[iIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
				continue;
			//{{ 2013. 2. 21	박세훈	대전 NPC 퀘스트에 에픽 NPC 만 카운트 되게 수정
#ifdef PVP_QUEST_HERO_KILL_COUNT
#else
			else
				++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
			//}}

			switch( eSubQuestType )
			{
			case CXSLQuestManager::SQT_PVP_PLAY:
				{
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
					if( pQuestTemplet->m_iID == 63790 )
					{
						if( bCouplePvp == true && iPvpChannelClass == KPVPChannelInfo::PCC_TOURNAMENT && bIsDrawn == false )
							++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
					}
					else
					{
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
					//{{ 2013. 2. 21	박세훈	대전 NPC 퀘스트에 에픽 NPC 만 카운트 되게 수정
#ifdef PVP_QUEST_HERO_KILL_COUNT
					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
					//}}
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
					}
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPPlay <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;									
				}
				break;
			case CXSLQuestManager::SQT_PVP_WIN:
				{
					//{{ 2013. 2. 21	박세훈	대전 NPC 퀘스트에 에픽 NPC 만 카운트 되게 수정
#ifdef PVP_QUEST_HERO_KILL_COUNT
					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
					//}}

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPWin <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
				}
				break;
			case CXSLQuestManager::SQT_PVP_KILL:
				{
#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL	// 레이븐을 킬할때 횟수와 레이븐이 아닌 캐릭터를 킬할때 횟수 퀘스트
					CXSLUnit::UNIT_TYPE killUserUnitType = (CXSLUnit::UNIT_TYPE)spUser->GetUnitType();
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
					CXSLUnit::UNIT_CLASS killUserUnitClass = static_cast<CXSLUnit::UNIT_CLASS>(spUser->GetUnitClass());
					int ikillUserUnitClass = static_cast<int>(killUserUnitClass);
					int ikilledUserUnitClass = static_cast<int>(killedUserUnitClass);
					if (pSubQuestTemplet->m_iID == 121210 ) //청에 관한 퀘스트 아이디
#else //SERV_EVENT_QUEST_CHUNG_PVP_KILL
					if (pSubQuestTemplet->m_iID == 130641 )
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					{	
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
						// 내가 청 2차전직이 아니고 나머지 2차전직 중에 하나이고 죽인게 청2차이다.
						if( ikillUserUnitClass >= 100 && 118 >= ikillUserUnitClass ) //이안에 있으면 2차 전직
						{
							if( killUserUnitClass != CXSLUnit::UC_CHUNG_IRON_PALADIN &&  killUserUnitClass != CXSLUnit::UC_CHUNG_DEADLY_CHASER )
							{
								if( CXSLUnit::UNIT_CLASS::UC_CHUNG_IRON_PALADIN == killedUserUnitClass || killedUserUnitClass == CXSLUnit::UNIT_CLASS::UC_CHUNG_DEADLY_CHASER )
								{
									++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
								}
							}
							
						}
#else SERV_EVENT_QUEST_CHUNG_PVP_KILL
						// 내가 레이븐이 아니고, 킬 상대가 레이븐이면,
						if( CXSLUnit::UT_RAVEN != killUserUnitType
							 && CXSLUnit::UT_RAVEN == killedUserUnitType
							)
						{
							++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
						}
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					}
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
					else if (pSubQuestTemplet->m_iID == 121220) //내가 청 2차 전직이고 킬 상대가 청 2차 전직이 아니면 퀘스트 클리어 
#else //SERV_EVENT_QUEST_CHUNG_PVP_KILL
					else if (pSubQuestTemplet->m_iID == 130640)
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					{
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
						if( killUserUnitClass == CXSLUnit::UC_CHUNG_IRON_PALADIN  ||  killUserUnitClass == CXSLUnit::UC_CHUNG_DEADLY_CHASER ) //난 청 2차이다
						{
							if( ikilledUserUnitClass >= 100 && 118 >= ikilledUserUnitClass ) //2차 전직 캐릭을 죽였냐?
							{
								if( killedUserUnitClass != CXSLUnit::UC_CHUNG_IRON_PALADIN  &&  killedUserUnitClass != CXSLUnit::UC_CHUNG_DEADLY_CHASER ) //2차 전직중에서 죽인게 청2차가 아니냐?
								{
									++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
								}
							}
						}
#else //SERV_EVENT_QUEST_CHUNG_PVP_KILL
						// 내가 레이븐이고, 킬 상대가 레이븐아니면,
						if(CXSLUnit::UT_RAVEN == killUserUnitType
							&& CXSLUnit::UT_RAVEN != killedUserUnitType)
							++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
					}
					else
					{
						++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
					}
#else
						//{{ 2013. 2. 21	박세훈	대전 NPC 퀘스트에 에픽 NPC 만 카운트 되게 수정
	#ifdef PVP_QUEST_HERO_KILL_COUNT
						++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
	#endif PVP_QUEST_HERO_KILL_COUNT
						//}}
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPKill <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
				}
				break;
				//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
			case CXSLQuestManager::SQT_PVP_NPC_HUNT:
				//{{ 2013. 2. 22	Merge	박세훈	 대전 SQT_PVP_NPC_HUNT 퀘스트 타입에 에픽 NPC 와 플래이, 에픽 NPC 승리조건도 추가
#ifdef SERV_PVP_NPC_HUNT_QUEST_EXTEND
			case CXSLQuestManager::SQT_PVP_HERO_NPC_KILL:
			case CXSLQuestManager::SQT_PVP_HERO_NPC_PLAY:
#endif SERV_PVP_NPC_HUNT_QUEST_EXTEND
				//}}
				{
					//{{ 2013. 2. 21	박세훈	대전 NPC 퀘스트에 에픽 NPC 만 카운트 되게 수정
#ifdef PVP_QUEST_HERO_KILL_COUNT
					if( bIsHeroNPC == false )
					{
						break;
					}

					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;
#endif PVP_QUEST_HERO_KILL_COUNT
					//}}

					if( pSubQuestTemplet->m_ClearCondition.m_iPVPWin <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					{
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
					}
				}
				break;
#endif SERV_NEW_PVP_QUEST
				//}}
				
				//{{ 2013. 2. 21	박세훈	대전 NPC 퀘스트에 에픽 NPC 만 카운트 되게 수정
#ifdef PVP_QUEST_HERO_KILL_COUNT
			default:
				{
					START_LOG( cwarn, L"OnPvp로 처리되지 않는 서브 퀘스트 타입입니다." )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( eSubQuestType )
						<< END_LOG;
				}
				break;
#endif PVP_QUEST_HERO_KILL_COUNT
				//}}
			}

			kNot.m_vecQuestInst.push_back( *pkQuestInstance );

			START_LOG( clog, L"퀘스트 대전쪽 수행.!" )
				<< BUILD_LOG( iGameType )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iKillNum )
				<< BUILD_LOGc( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
				<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
				;			
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
void KUserQuestManager::Handler_OnEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iEnchantLevel )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )		
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 아이템 강화 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_ENCHANT )
				continue;

			// 3. 어떤 아이템인지?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			// 4. 해당 강화 레벨이 맞는지?
			if( pSubQuestTemplet->m_ClearCondition.m_iEnchantLevel != iEnchantLevel )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData == 0 )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"퀘스트 SQT_ITEM_ENCHANT 수행" )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( iEnchantLevel )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
		
	}
}

void KUserQuestManager::Handler_OnSocketItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iSocketUseCount )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 아이템 소켓 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_SOCKET )
				continue;

			// 3. 해당 아이템이 맞는지?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			// 퀘스트 클리어 조건 카운트를 넘지않게 한다.
			if( pSubQuestTemplet->m_ClearCondition.m_iSocketCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				// 소켓 사용 카운트 만큼 올린다!
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += static_cast<UCHAR>(iSocketUseCount);

				// 클리어 조건을 넘어선다면 보정하자!
				if( pSubQuestTemplet->m_ClearCondition.m_iSocketCount < static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData ) )
					kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = static_cast<UCHAR>(pSubQuestTemplet->m_ClearCondition.m_iSocketCount);
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iSocketCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"퀘스트 SQT_ITEM_SOCKET 수행" )
				<< BUILD_LOG( iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;			
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

void KUserQuestManager::Handler_OnAttribEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const KItemAttributeEnchantInfo& kItemAttribEnchantInfo )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 아이템 속성 강화 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_ATTRIB )
				continue;

			// 3. 해당 아이템이 맞는지?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			// 4-1. 속성 단계인지?
			if( pSubQuestTemplet->m_ClearCondition.m_eAttribCountType != CXSLAttribEnchantItem::ACT_NONE )
			{
				if( SiCXSLAttribEnchantItem()->GetAttribCountType( kItemAttribEnchantInfo ) != pSubQuestTemplet->m_ClearCondition.m_eAttribCountType )
					continue;
			}
			// 5-2. 특정 속성 인지?
			else
			{
				if( pSubQuestTemplet->m_ClearCondition.m_kAttribEnchantInfo.Compare( kItemAttribEnchantInfo ) == false )
					continue;
			}

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData == 0 )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"퀘스트 SQT_ITEM_ATTRIB 수행" )
					<< BUILD_LOG( iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}    
}

void KUserQuestManager::Handler_OnResolveItem( IN KGSUserPtr spUser, IN const int iItemID )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 아이템 소켓 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_RESOLVE )
				continue;

			// 3. 해당 아이템이 맞는지?
			if( pSubQuestTemplet->m_ClearCondition.m_iItemID != iItemID  &&  pSubQuestTemplet->m_ClearCondition.m_iItemID != 0 )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			// 퀘스트 클리어 조건 카운트를 넘지않게 한다.
			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"퀘스트 SQT_ITEM_RESOLVE 수행" )
				<< BUILD_LOG( iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
void KUserQuestManager::Handler_OnResolveItem( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 아이템 소켓 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_RESOLVE )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}
			// 퀘스트 클리어 조건 카운트를 넘지않게 한다.
			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iResolveCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"퀘스트 SQT_ITEM_RESOLVE 수행" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_MULTI_RESOLVE
//}}

void KUserQuestManager::Handler_OnUseSkillPoint( IN KGSUserPtr spUser, IN const int iSkillID )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 아이템 소켓 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_USE_SKILL_POINT )
				continue;

			// 3. 해당 스킬이 맞는지?
			if( pSubQuestTemplet->m_ClearCondition.m_iSkillID != iSkillID  &&  pSubQuestTemplet->m_ClearCondition.m_iSkillID != 0 )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			// 퀘스트 클리어 조건 카운트를 넘지않게 한다.
			if( pSubQuestTemplet->m_ClearCondition.m_iSpUseCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iSpUseCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"퀘스트 SQT_USE_SKILL_POINT 수행" )
				<< BUILD_LOG( iSkillID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_eTalkNPCID )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess );
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_QUEST_CLEAR_EXPAND
//}}

//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
void KUserQuestManager::Handler_OnEnterTheVillage( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 필드 입장 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_VISIT_VILLAGE )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 3. 해당 필드가 맞는지 확인!
			if( SiCXSLQuestManager()->IsExistVillageInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
				continue;
#else
			// 3. 해당 필드가 맞는지 확인!
			if( pSubQuestTemplet->m_ClearCondition.m_iVillageMapID != spUser->GetMapID() )
				continue;
#endif SERV_REFORM_QUEST
			//}}
			
			// 4. 이미 완료한 퀘스트 인지 확인!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"퀘스트 SQT_VISIT_VILLAGE 수행" )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}

//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
void KUserQuestManager::Handler_OnEnterTheBattleField( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 필드 입장 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_VISIT_FIELD )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 3. 해당 필드가 맞는지 확인!
			if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
				continue;
#else
			// 3. 해당 필드가 맞는지 확인!
			if( pSubQuestTemplet->m_ClearCondition.m_iBattleFieldID != spUser->GetMapID() )
				continue;
#endif SERV_REFORM_QUEST
			//}}
#ifdef  SERV_EVENT_TITLE_SUBQUEST
			if( pSubQuestTemplet->m_ClearCondition.m_iUseTitleID != -1 ) //칭호에 관련된게 있다.
			{
				//현재 입장할때 착용한 칭호가 퀘스트 클리어 칭호가 아니라면....
				if( spUser->GetEquippedTitleID() !=  pSubQuestTemplet->m_ClearCondition.m_iUseTitleID )
				{
					continue;
				}
			}
#endif  SERV_EVENT_TITLE_SUBQUEST
			// 4. 이미 완료한 퀘스트 인지 확인!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
#endif SERV_REFORM_QUEST
			//}}

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"퀘스트 SQT_VISIT_FIELD 수행" )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
//}}
//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::Handler_EGS_QUEST_COMPLETE_REQ( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin /*= false*/ )
{
	SET_ERROR( NET_OK );

	bool bCheckAfterQuest = true;
	bool bIsVisitVillageQuest = false;
	std::map<int,int> mapAcceptRewardItem;
	std::vector<int>::const_iterator vitAfterQuest;
    
	if( m_mapQuesting.empty() == true )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 수행중인지 확인한다.	
	if( IsQuest( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 퀘스트 인스턴스 얻기
	KQuestInstance* pKQuestInst = GetQuestInstance( kReq.m_iQuestID );
	if( pKQuestInst == NULL )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 퀘스트 템플릿 얻기
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"퀘스트 진행중인데 템플릿 데이터가 없다." )
			<< BUILD_LOG( kReq.m_iQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetUID() )
#else
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SET_ERROR( ERR_QUEST_08 );
		goto error_proc;
	}


	// PC방 전용 퀘스트 인지 검사
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_20 );
			goto error_proc;
		}
	}


	// 이벤트 퀘스트라면 현재 진행중인 이벤트인지 확인한다.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// 현재 진행중인 이벤트인지 확인!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

	// 에픽 퀘스트 일경우 레벨이 안되면 퀘스트 완료 할수없음.
	if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_Condition.m_iLevel > spUser->GetLevel() )
		goto error_proc;

	vitAfterQuest = pQuestTemplet->m_vecAfterQuestID.begin();
	for( ; vitAfterQuest != pQuestTemplet->m_vecAfterQuestID.end() ; ++vitAfterQuest )
	{
		if( bCheckAfterQuest == false )
			break;

		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( *vitAfterQuest );
		if( pAfterQuestTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 퀘스트가 after퀘스트로 등록되어있습니다!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( *vitAfterQuest )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );

			bCheckAfterQuest = false;
			continue;
		}

		// 수락 보상이 있는지 확인해보자!
		if( pAfterQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// 퀘스트 수락 보상이 인벤토리에 남아 있으면 퀘스트 수락 못함
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pAfterQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pAfterQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"퀘스트 수락시 받는 보상이 이미 인벤토리에 있으면 퀘스트 수락 못함." )
						<< BUILD_LOG( pAfterQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_22 );

					bCheckAfterQuest = false;
					continue;
				}
				mapAcceptRewardItem.insert( std::make_pair( mitRI->first, mitRI->second ) );
			}
		}
	}

	// AfterQuest(복수 일수도 있음) 수락 보상이 들어갈만한 빈슬롯이 있는지 검사
	if( spUser->IsEnoughSpaceExist( mapAcceptRewardItem ) == false )
	{
		START_LOG( clog, L"퀘스트 수락시 받는 보상이 들어갈 인벤토리 빈공간이 부족합니다." )
			<< BUILD_LOG( mapAcceptRewardItem.size() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_23 );

		goto error_proc;
	}

	// AfterQuest 검사에서 실패를 했다면
	if( bCheckAfterQuest == false )
		goto error_proc;
	
	for( int iIndexSub = 0; iIndexSub < (int)pKQuestInst->m_vecSubQuestInstance.size(); ++iIndexSub )
	{
		// item 서브 퀘스트 수행완료 체크를 별도로 넣어야함.(완료체크시에만 하기때문)
		// 수집한 아이템 변동이 어떻게 될지 몰라 완료체크를 하는 순간에만 체크해야 하기 때문
		const int iSubQuestID = pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_iID;

		const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
		if( pSubQuestTemplet == NULL )
		{
			START_LOG( cerr, L"퀘스트 진행중인데 서브템플릿 데이터가 없다." )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_VISIT_VILLAGE )
			bIsVisitVillageQuest = true;

		//운영자 명령이면 수행하지 않는다.
		if( bIsAdmin == true )
			continue;

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION  ||
			pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
		{
			//수집해야할 아이템(개수만큼) 있는지 확인하여 서브퀘스트 완료체크를 한다.
			std::map<int,int> mapItemInfo;
			mapItemInfo.insert( std::make_pair( pSubQuestTemplet->m_ClearCondition.m_iItemID, pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum ) );

			//{{ 2009. 8. 7  최육사		은행
			if( spUser->IsEnoughItemExistOnlyInventory( mapItemInfo ) == false )
			{
				START_LOG( cerr, L"성공한 퀘스트가 아닌데 성공체크가 날아왔다?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
			//}}
		}
		else
		{
			if( pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == false )
			{
				//{{ 2009. 4. 6  최육사		에러로그 추가
				START_LOG( cerr, L"성공한 퀘스트가 아닌데 성공체크가 날아왔다?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< END_LOG;
				//}}

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
		}
	}

	//완료 퀘스트 보상처리
	if( NetError::GetLastError() == NetError::NET_OK )
	{
		//QUEST REWARD DB 처리
		KDBE_QUEST_COMPLETE_REQ kDBReq;
		kDBReq.m_bAutoComplete = kReq.m_bAutoComplete;
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_bIsRepeat	= ( pQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_NORMAL );	
		kDBReq.m_bIsNew		= false;
		kDBReq.m_bIsChangeJob = false;

#ifdef SERV_SKILL_PAGE_SYSTEM
		kDBReq.m_iTheNumberOfSkillPagesAvailable 
			= spUser->GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM

		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB &&
			pQuestTemplet->m_Reward.m_eChangeUnitClass != CXSLUnit::UC_NONE &&
			spUser->GetUnitClass() != pQuestTemplet->m_Reward.m_eChangeUnitClass )
		{
			kDBReq.m_bIsChangeJob = true;
			kDBReq.m_cChangeUnitClass = pQuestTemplet->m_Reward.m_eChangeUnitClass;

			if( kDBReq.m_cChangeUnitClass == spUser->GetUnitClass() )
			{
				START_LOG( cerr, L"전직할려는 퀘스트 보상 Unit Class가 현재 class와 동일함.?" )
					<< BUILD_LOG( spUser->GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT					
					<< BUILD_LOGc( spUser->GetUnitClass() )
					<< BUILD_LOG( kDBReq.m_cChangeUnitClass )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_18 );
				goto error_proc;
			}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
			int iDefaultSkill[6] = {0,};
			if( SiCXSLSkillTree()->GetUnitClassDefaultSkill( kDBReq.m_cChangeUnitClass, iDefaultSkill[0], iDefaultSkill[1], iDefaultSkill[2], iDefaultSkill[3], iDefaultSkill[4], iDefaultSkill[5] ) == false )
			{
				START_LOG( cerr, L"전직시 지급하려는 기본 스킬이 이상함" )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOGc( spUser->GetUnitClass() )
					<< BUILD_LOG( kDBReq.m_cChangeUnitClass )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_18 );
				goto error_proc;
			}

			// iDefaultSkill[0], iDefaultSkill[1] ->  이 스킬들은 노전직 스킬 이므로 퀘스트로 전직 가능한 것이 아니다.
			if( CXSLUnit::IsFirstChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kDBReq.m_cChangeUnitClass) ) == true )
			{
				kDBReq.m_iNewDefaultSkill1 = iDefaultSkill[2];
				kDBReq.m_iNewDefaultSkill2 = iDefaultSkill[3];
				
			}
			else if( CXSLUnit::IsSecondChangeJob( static_cast<CXSLUnit::UNIT_CLASS>(kDBReq.m_cChangeUnitClass) ) == true )
			{
				kDBReq.m_iNewDefaultSkill1 = iDefaultSkill[4];
				kDBReq.m_iNewDefaultSkill2 = iDefaultSkill[5];
			}
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
			
		}

		std::map<int,KItemInfo> mapItem;

		//선택보상 처리부분..
		if( kReq.m_mapSelectItemID.size() > 0 )
		{
			//선택보상 수량이 최대선택 수량보다 많으면 예외처리..
			if( (int)kReq.m_mapSelectItemID.size() > pQuestTemplet->m_SelectReward.m_iSelectionCount )
			{
				START_LOG( cerr, L"퀘스트 선택보상 수량이 최대선택 수량보다 많음.!" )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< BUILD_LOG( pQuestTemplet->m_SelectReward.m_iSelectionCount )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_14 );
				goto error_proc;
			}

			bool bIsSelectItem	= false;
			std::map<int,int>::const_iterator mit;

			//선택보상 아이템들의 정보를 셋팅함.
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestSelectReward, pQuestTemplet->m_SelectReward.m_vecSelectItem )
			{
				mit = kReq.m_mapSelectItemID.find( kQuestSelectReward.m_iItemID );
				if( mit != kReq.m_mapSelectItemID.end() )
				{
					bIsSelectItem = true;

					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestSelectReward.m_iItemID );
					if( pItemTemplet == NULL )
					{
						START_LOG( cerr, L"아이템 템플릿 포인터 이상.!" )
							<< BUILD_LOG( kQuestSelectReward.m_iItemID )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;
						continue;
					}

					KItemInfo kItemInfo;
					kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
					kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
					kItemInfo.m_iQuantity	= kQuestSelectReward.m_iQuantity;

					//보상아이템이 기간제 일경우..
					if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
					{
						//보상일수이 있는지 검사하여 날자를 셋팅해준다.
						if( kQuestSelectReward.m_iPeriod > 0 )
						{
							kItemInfo.m_sPeriod = static_cast<short>(kQuestSelectReward.m_iPeriod);
						}
					}

					//{{ 2008. 8. 25  최육사	보상아이템에 소켓옵션이 있는경우
					const int iRewardSocketOption = kQuestSelectReward.m_iSocketOption1;
					if( iRewardSocketOption > 0 )
					{
						if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
						{
							kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
						}
						else
						{
							START_LOG( cerr, L"존재하지 않는 소켓옵션을 퀘스트 보상 아이템에 넣으려 했습니다." )											
								<< BUILD_LOG( kReq.m_iQuestID )
								<< BUILD_LOG( kItemInfo.m_iItemID )
								<< BUILD_LOG( iRewardSocketOption )
								<< END_LOG;
						}
					}

					mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );

					START_LOG( clog, L"퀘스트 선택 아이템 처리" )
						<< BUILD_LOG( kQuestSelectReward.m_iItemID );
				}
			}

			if( bIsSelectItem == false )
			{
				SET_ERROR( ERR_QUEST_15 );

				START_LOG( cerr, L"퀘스트 템플릿에서 선택보상 아이템 찾기 실패.!" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< END_LOG;

				goto error_proc;
			}
		}

		//일반보상 처리부분..
		if( pQuestTemplet->m_Reward.m_vecItem.size() > 0 )
		{
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestRewardData, pQuestTemplet->m_Reward.m_vecItem )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestRewardData.m_iItemID );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"아이템 템플릿 포인터 이상.!" )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( kQuestRewardData.m_iItemID )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( spUser->GetCharUID() )
#else
						<< BUILD_LOG( spUser->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT						
						<< END_LOG;
					continue;
				}

				KItemInfo kItemInfo;
				kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
				kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
				kItemInfo.m_iQuantity	= kQuestRewardData.m_iQuantity;

				//보상아이템이 기간제 일경우..
				if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
				{
					//기간이 있는지 검사하여 날자를 셋팅해준다.
					if( kQuestRewardData.m_iPeriod > 0 )
					{
						kItemInfo.m_sPeriod = static_cast<short>(kQuestRewardData.m_iPeriod);
					}
				}

				//{{ 2008. 8. 25  최육사	보상아이템에 소켓옵션이 있는경우
				const int iRewardSocketOption = kQuestRewardData.m_iSocketOption1;
				if( iRewardSocketOption > 0 )
				{
					if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
					{
						kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
					}
					else
					{
						START_LOG( cerr, L"존재하지 않는 소켓옵션을 퀘스트 보상 아이템에 넣으려 했습니다." )											
							<< BUILD_LOG( kReq.m_iQuestID )
							<< BUILD_LOG( kItemInfo.m_iItemID )
							<< BUILD_LOG( iRewardSocketOption )
							<< END_LOG;
					}
				}

				mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );//pQuestTemplet->m_Reward.m_vecItem[i].m_iItemID, pQuestTemplet->m_Reward.m_vecItem[i].m_iQuantity ) );
			}
		}

		if( mapItem.size() > 0 || SiCXSLQuestManager()->IsSubItemCondition(kDBReq.m_iQuestID) == true )
		{
			std::map<int,int> mapQuestConditionItem;
			kDBReq.m_mapInsertedItem = mapItem;	//보상받을 아이템 리스트.

			//운영자면 아이템 수집퀘스트 경우도 해당 아이템을 삭제하지 않기위해.
			if( bIsAdmin == false )
			{
				SiCXSLQuestManager()->GetQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
			}

			std::map< UidType, int > mapDummy;
			if( spUser->m_kInventory.DeleteAndInsert( mapQuestConditionItem, mapDummy, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo, true, KDeletedItemInfo::DR_QUEST_COMPLETE ) == true )
			{
				if( kDBReq.m_vecItemInfo.empty() == false )
				{
					kDBReq.m_bIsNew = true;
					spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
					spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
				}
				else
				{
					kDBReq.m_bIsNew = false;
				}
			}
			else
			{
				
				if( bIsVisitVillageQuest )
				{
					std::map< int, KItemInfo >::const_iterator mit;
					for( mit = kDBReq.m_mapInsertedItem.begin(); mit != kDBReq.m_mapInsertedItem.end(); ++mit )
					{
						// 우편으로 지급!
						spUser->SendQuestRewardLetter( kDBReq.m_iQuestID, mit->second.m_iItemID, mit->second.m_iQuantity );
					}

					// 임시 인벤으로 지급되었기 때문에 실제 인벤에 관련된 데이터는 모두 지운다!
					kDBReq.m_mapInsertedItem.clear();
				}
				else
				{
					//인벤이 부족하면 그냥 유저에게 알려주고 완료를 하지 않는다.
					SET_ERROR( ERR_QUEST_13 );
					goto error_proc;
				}
			}

			//{{ 2007. 11. 25  최육사  일일통계 퀘스트 조건 아이템 사용통계
			std::map< int, int >::iterator mit = mapQuestConditionItem.begin();
			for( ; mit != mapQuestConditionItem.end(); mit++ )
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( mit->first );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, mit->second );
				KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, mit->second );

			}
			//}}
		}

		//{{ 2007. 12. 26  최육사  DB 통계 퀘스트
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Complete, 1 );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_CompleteLevel, static_cast<int>(spUser->GetLevel()) );

			//{{ 2007. 12. 26  최육사  유저 통계
			int iColumnIndex;

			if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
			{
				iColumnIndex = KUserStatistics::US_Etc_NQuestComplete;
			}
			else
			{
				iColumnIndex = KUserStatistics::US_Etc_RQuestComplete;
			}

			spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
			//}}
		}
		//}}

		spUser->SendToGameDB( DBE_QUEST_COMPLETE_REQ, kDBReq );

		return;
	}

error_proc:
	
	if( kReq.m_bAutoComplete )
	{
		// 초기화
		ClearAutoCompleteQuest();

		KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kAck;
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kAck );
	}
	else
	{
		KEGS_QUEST_COMPLETE_ACK kAck;
		//완료체크를 위해 퀘스트 ID를 다시 돌려준다.
		kAck.m_kCompleteQuestInfo.m_iQuestID = kReq.m_iQuestID;
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kAck );
	}
}

#else

void KUserQuestManager::Handler_EGS_QUEST_COMPLETE_REQ( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin /*= false*/ )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 수행중인지 확인한다.	
	if( IsQuest( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 퀘스트 인스턴스 얻기
	KQuestInstance* pKQuestInst = GetQuestInstance( kReq.m_iQuestID );
	if( pKQuestInst == NULL )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 퀘스트 템플릿 얻기
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"퀘스트 진행중인데 템플릿 데이터가 없다." )
			<< BUILD_LOG( kReq.m_iQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetUID() )
#else
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		SET_ERROR( ERR_QUEST_08 );
		goto error_proc;
	}

	//{{ 2010. 01. 29  최육사	PC방 전용 퀘스트
#ifdef SERV_PC_BANG_QUEST

	// PC방 전용 퀘스트 인지 검사
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			SET_ERROR( ERR_QUEST_20 );
			goto error_proc;
		}
	}

#endif SERV_PC_BANG_QUEST
	//}}

	//{{ 2010. 02. 06  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST

	// 이벤트 퀘스트라면 현재 진행중인 이벤트인지 확인한다.
	if( pQuestTemplet->m_bIsTimeEvent == true )
	{
		// 현재 진행중인 이벤트인지 확인!
		if( IsExistEventQuest( kReq.m_iQuestID ) == false )
		{
			SET_ERROR( ERR_QUEST_21 );
			goto error_proc;
		}
	}

#endif SERV_DAY_QUEST
	//}}

#ifdef SERV_EPIC_QUEST
	// 에픽 퀘스트 일경우 레벨이 안되면 퀘스트 완료 할수없음.
	if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_Condition.m_iLevel > spUser->GetLevel()
		)
		goto error_proc;
#endif SERV_EPIC_QUEST

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	if( pQuestTemplet->m_iAfterQuestID != 0 )
	{
		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pQuestTemplet->m_iAfterQuestID );
		if( pAfterQuestTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 퀘스트가 after퀘스트로 등록되어있습니다!" )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< BUILD_LOG( pQuestTemplet->m_iAfterQuestID )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		// 수락 보상이 있는지 확인해보자!
		if( pAfterQuestTemplet->m_mapAcceptRewardItem.empty() == false )
		{
			// 퀘스트 수락 보상이 인벤토리에 남아 있으면 퀘스트 수락 못함
			std::map< int, int >::const_iterator mitRI;
			for( mitRI = pAfterQuestTemplet->m_mapAcceptRewardItem.begin(); mitRI != pAfterQuestTemplet->m_mapAcceptRewardItem.end(); ++mitRI )
			{
				if( spUser->IsExistInventoryItemWithBank( mitRI->first ) == true )
				{
					START_LOG( clog, L"퀘스트 수락시 받는 보상이 이미 인벤토리에 있으면 퀘스트 수락 못함." )
						<< BUILD_LOG( pAfterQuestTemplet->m_iID )
						<< BUILD_LOG( mitRI->first )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_22 );
					goto error_proc;
				}
			}

			// 퀘스트 수락 보상이 들어갈만한 빈슬롯이 있는지 검사
			if( spUser->IsEnoughSpaceExist( pAfterQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"퀘스트 수락시 받는 보상이 들어갈 인벤토리 빈공간이 부족합니다." )
					<< BUILD_LOG( pAfterQuestTemplet->m_iID )
					<< BUILD_LOG( pAfterQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_23 );
				goto error_proc;
			}
		}
	}
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	bool bIsVisitVillageQuest = false;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	for( int iIndexSub = 0; iIndexSub < (int)pKQuestInst->m_vecSubQuestInstance.size(); ++iIndexSub )
	{
		// item 서브 퀘스트 수행완료 체크를 별도로 넣어야함.(완료체크시에만 하기때문)
		// 수집한 아이템 변동이 어떻게 될지 몰라 완료체크를 하는 순간에만 체크해야 하기 때문

		const int iSubQuestID = pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_iID;

		const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
		if( pSubQuestTemplet == NULL )
		{
			START_LOG( cerr, L"퀘스트 진행중인데 서브템플릿 데이터가 없다." )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_VISIT_VILLAGE )
			bIsVisitVillageQuest = true;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		//운영자 명령이면 수행하지 않는다.
		if( bIsAdmin == true )
			continue;

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION  ||
			pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
		{
			//수집해야할 아이템(개수만큼) 있는지 확인하여 서브퀘스트 완료체크를 한다.
			std::map<int,int> mapItemInfo;
			mapItemInfo.insert( std::make_pair( pSubQuestTemplet->m_ClearCondition.m_iItemID, pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum ) );

			//{{ 2009. 8. 7  최육사		은행
			if( spUser->IsEnoughItemExistOnlyInventory( mapItemInfo ) == false )
			{
				START_LOG( cerr, L"성공한 퀘스트가 아닌데 성공체크가 날아왔다?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
			//}}
		}
#ifdef SERV_POINT_COUNT_SYSTEM
		else if ( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_POINT_COUNT )
		{

		}
#endif //SERV_POINT_COUNT_SYSTEM
		else
		{
			if( pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == false )
			{
				//{{ 2009. 4. 6  최육사		에러로그 추가
				START_LOG( cerr, L"성공한 퀘스트가 아닌데 성공체크가 날아왔다?" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( iSubQuestID )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;
				//}}

				SET_ERROR( ERR_QUEST_09 );
				goto error_proc;
			}
		}
	}

	//완료 퀘스트 보상처리
	if( NetError::GetLastError() == NetError::NET_OK )
	{
		//QUEST REWARD DB 처리
		KDBE_QUEST_COMPLETE_REQ kDBReq;
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		kDBReq.m_bAutoComplete = kReq.m_bAutoComplete;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		//{{ 2010. 04. 01  최육사	비밀던전 헬모드
		//#ifdef SERV_DAILY_QUEST
		kDBReq.m_bIsRepeat	= ( pQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_NORMAL );
		//#else
		//		kDBReq.m_bIsRepeat	= pQuestTemplet->m_bRepeat;
		//#endif SERV_DAILY_QUEST
		//}}		
		kDBReq.m_bIsNew		= false;
		kDBReq.m_bIsChangeJob = false;

#ifdef SERV_SKILL_PAGE_SYSTEM
		kDBReq.m_iTheNumberOfSkillPagesAvailable = spUser->GetTheNumberOfSkillPagesAvailable();
#endif // SERV_SKILL_PAGE_SYSTEM

		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB &&
			pQuestTemplet->m_Reward.m_eChangeUnitClass != CXSLUnit::UC_NONE &&
			spUser->GetUnitClass() != pQuestTemplet->m_Reward.m_eChangeUnitClass )
		{
			kDBReq.m_bIsChangeJob = true;
			kDBReq.m_cChangeUnitClass = pQuestTemplet->m_Reward.m_eChangeUnitClass;

			if( kDBReq.m_cChangeUnitClass == spUser->GetUnitClass() )
			{
				START_LOG( cerr, L"전직할려는 퀘스트 보상 Unit Class가 현재 class와 동일함.?" )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOGc( spUser->GetUnitClass() )
					<< BUILD_LOG( kDBReq.m_cChangeUnitClass )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_18 );
				goto error_proc;
			}
		}

		std::map<int,KItemInfo> mapItem;

		//선택보상 처리부분..
		if( kReq.m_mapSelectItemID.size() > 0 )
		{
			//선택보상 수량이 최대선택 수량보다 많으면 예외처리..
			if( (int)kReq.m_mapSelectItemID.size() > pQuestTemplet->m_SelectReward.m_iSelectionCount )
			{
				START_LOG( cerr, L"퀘스트 선택보상 수량이 최대선택 수량보다 많음.!" )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< BUILD_LOG( pQuestTemplet->m_SelectReward.m_iSelectionCount )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_14 );
				goto error_proc;
			}

			bool bIsSelectItem	= false;
			std::map<int,int>::const_iterator mit;

			//선택보상 아이템들의 정보를 셋팅함.
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestSelectReward, pQuestTemplet->m_SelectReward.m_vecSelectItem )
			{
				mit = kReq.m_mapSelectItemID.find( kQuestSelectReward.m_iItemID );
				if( mit != kReq.m_mapSelectItemID.end() )
				{
					bIsSelectItem = true;

					const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestSelectReward.m_iItemID );
					if( pItemTemplet == NULL )
					{
						START_LOG( cerr, L"아이템 템플릿 포인터 이상.!" )
							<< BUILD_LOG( kQuestSelectReward.m_iItemID )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;
						continue;
					}

					KItemInfo kItemInfo;
					kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
					kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
					kItemInfo.m_iQuantity	= kQuestSelectReward.m_iQuantity;

					//보상아이템이 기간제 일경우..
					if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
					{
						//보상일수이 있는지 검사하여 날자를 셋팅해준다.
						if( kQuestSelectReward.m_iPeriod > 0 )
						{
							kItemInfo.m_sPeriod = static_cast<short>(kQuestSelectReward.m_iPeriod);
						}
					}

					//{{ 2008. 8. 25  최육사	보상아이템에 소켓옵션이 있는경우
					//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
					//#ifdef SERV_ITEM_OPTION_DATA_SIZE
					const int iRewardSocketOption = kQuestSelectReward.m_iSocketOption1;
					if( iRewardSocketOption > 0 )
					{
						if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
						{
							kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
						}
						else
						{
							START_LOG( cerr, L"존재하지 않는 소켓옵션을 퀘스트 보상 아이템에 넣으려 했습니다." )											
								<< BUILD_LOG( kReq.m_iQuestID )
								<< BUILD_LOG( kItemInfo.m_iItemID )
								<< BUILD_LOG( iRewardSocketOption )
								<< END_LOG;
						}
					}
					//#else
					//					short sRewardSocketOption = pQuestTemplet->m_SelectReward.m_vecSelectItem[i].m_sSocketOption1;
					//					if( sRewardSocketOption > 0 )
					//					{
					//						if( SiCXSLSocketItem()->GetSocketData( sRewardSocketOption ) != NULL )
					//						{
					//							kItemInfo.m_vecItemSocket.push_back( sRewardSocketOption );
					//						}
					//						else
					//						{
					//							START_LOG( cerr, L"존재하지 않는 소켓옵션을 퀘스트 보상 아이템에 넣으려 했습니다." )
					//								<< BUILD_LOG( kReq.m_iQuestID )
					//								<< BUILD_LOG( kItemInfo.m_iItemID )
					//								<< BUILD_LOG( sRewardSocketOption )
					//								<< END_LOG;
					//						}
					//					}
					//#endif SERV_ITEM_OPTION_DATA_SIZE
					//}} 
					//}}

					//{{ 2012. 11. 04	박세훈	퀘스트 보상 아이템 강화 레벨 설정
#ifdef SERV_QUEST_REWARD_ITEM_ENCHANT
					kItemInfo.m_cEnchantLevel	= kQuestSelectReward.m_cEnchantLevel;
#endif SERV_QUEST_REWARD_ITEM_ENCHANT
					//}}

					mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );//pQuestTemplet->m_SelectReward.m_vecSelectItem[i].m_iItemID, pQuestTemplet->m_SelectReward.m_vecSelectItem[i].m_iQuantity ) );

					START_LOG( clog, L"퀘스트 선택 아이템 처리" )
						<< BUILD_LOG( kQuestSelectReward.m_iItemID );
				}
			}

			if( bIsSelectItem == false )
			{
				SET_ERROR( ERR_QUEST_15 );

				START_LOG( cerr, L"퀘스트 템플릿에서 선택보상 아이템 찾기 실패.!" )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< BUILD_LOG( kReq.m_mapSelectItemID.size() )
					<< END_LOG;

				goto error_proc;
			}
		}

		//일반보상 처리부분..
		if( pQuestTemplet->m_Reward.m_vecItem.size() > 0 )// || SiCXSLQuestManager()->IsSubItemCondition(kDBReq.m_iQuestID) == true )
		{
			BOOST_TEST_FOREACH( const CXSLQuestManager::ITEM_DATA&, kQuestRewardData, pQuestTemplet->m_Reward.m_vecItem )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kQuestRewardData.m_iItemID );
				if( pItemTemplet == NULL )
				{
					START_LOG( cerr, L"아이템 템플릿 포인터 이상.!" )
						<< BUILD_LOG( pQuestTemplet->m_iID )
						<< BUILD_LOG( kQuestRewardData.m_iItemID )
						<< BUILD_LOG( spUser->GetCharName() )
						<< END_LOG;
					continue;
				}

				KItemInfo kItemInfo;
				kItemInfo.m_iItemID		= pItemTemplet->m_ItemID;
				kItemInfo.m_cUsageType	= pItemTemplet->m_PeriodType;
				kItemInfo.m_iQuantity	= kQuestRewardData.m_iQuantity;

				//보상아이템이 기간제 일경우..
				if( pItemTemplet->m_PeriodType == CXSLItem::PT_INFINITY )
				{
					//기간이 있는지 검사하여 날자를 셋팅해준다.
					if( kQuestRewardData.m_iPeriod > 0 )
					{
						kItemInfo.m_sPeriod = static_cast<short>(kQuestRewardData.m_iPeriod);
					}
					//보상기간이 없으면 예외처리를 함.
					//else
					//{
					//	SET_ERROR( ERR_QUEST_14 );

					//	START_LOG( cerr, L"퀘스트 기간제아이템 보상처리중 기간설정이 없음.!" )
					//		<< BUILD_LOG( pQuestTemplet->m_iID )
					//		<< BUILD_LOG( pQuestTemplet->m_Reward.m_vecItem[i].m_iPeriod )
					//		<< END_LOG;

					//	goto error_proc;
					//}
				}

				//{{ 2008. 8. 25  최육사	보상아이템에 소켓옵션이 있는경우
				//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
				//#ifdef SERV_ITEM_OPTION_DATA_SIZE
				const int iRewardSocketOption = kQuestRewardData.m_iSocketOption1;
				if( iRewardSocketOption > 0 )
				{
					if( SiCXSLSocketItem()->GetSocketData( iRewardSocketOption ) != NULL )
					{
						kItemInfo.m_vecItemSocket.push_back( iRewardSocketOption );
					}
					else
					{
						START_LOG( cerr, L"존재하지 않는 소켓옵션을 퀘스트 보상 아이템에 넣으려 했습니다." )											
							<< BUILD_LOG( kReq.m_iQuestID )
							<< BUILD_LOG( kItemInfo.m_iItemID )
							<< BUILD_LOG( iRewardSocketOption )
							<< END_LOG;
					}
				}
				//#else
				//				short sRewardSocketOption = pQuestTemplet->m_Reward.m_vecItem[i].m_sSocketOption1;
				//				if( sRewardSocketOption > 0 )
				//				{
				//					if( SiCXSLSocketItem()->GetSocketData( sRewardSocketOption ) != NULL )
				//					{
				//						kItemInfo.m_vecItemSocket.push_back( sRewardSocketOption );
				//					}
				//					else
				//					{
				//						START_LOG( cerr, L"존재하지 않는 소켓옵션을 퀘스트 보상 아이템에 넣으려 했습니다." )
				//							<< BUILD_LOG( kReq.m_iQuestID )
				//							<< BUILD_LOG( kItemInfo.m_iItemID )
				//							<< BUILD_LOG( sRewardSocketOption )
				//							<< END_LOG;
				//					}
				//				}
				//#endif SERV_ITEM_OPTION_DATA_SIZE
				//}} 
				//}}

				mapItem.insert( std::make_pair( kItemInfo.m_iItemID, kItemInfo ) );//pQuestTemplet->m_Reward.m_vecItem[i].m_iItemID, pQuestTemplet->m_Reward.m_vecItem[i].m_iQuantity ) );
			}
		}

		if( mapItem.size() > 0 || SiCXSLQuestManager()->IsSubItemCondition(kDBReq.m_iQuestID) == true )
		{
			std::map<int,int> mapQuestConditionItem;
			kDBReq.m_mapInsertedItem = mapItem;	//보상받을 아이템 리스트.

			//운영자면 아이템 수집퀘스트 경우도 해당 아이템을 삭제하지 않기위해.
			if( bIsAdmin == false )
			{
				SiCXSLQuestManager()->GetQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
			}

			std::map< UidType, int > mapDummy;
			if( spUser->m_kInventory.DeleteAndInsert( mapQuestConditionItem, mapDummy, kDBReq.m_mapInsertedItem, kDBReq.m_vecUpdatedInventorySlot, kDBReq.m_vecItemInfo, true, KDeletedItemInfo::DR_QUEST_COMPLETE ) == true )
			{
				if( kDBReq.m_vecItemInfo.empty() == false )
				{
					kDBReq.m_bIsNew = true;
					spUser->m_kInventory.FlushQuantityChange( kDBReq.m_kItemQuantityUpdate.m_mapQuantityChange );
					spUser->m_kInventory.FlushDeletedItem( kDBReq.m_kItemQuantityUpdate.m_vecDeleted );
				}
				else
				{
					kDBReq.m_bIsNew = false;
				}
			}
			else
			{
				//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
				if( bIsVisitVillageQuest )
				{
					std::map< int, KItemInfo >::const_iterator mit;
					for( mit = kDBReq.m_mapInsertedItem.begin(); mit != kDBReq.m_mapInsertedItem.end(); ++mit )
					{
						// 우편으로 지급!
						spUser->SendQuestRewardLetter( kDBReq.m_iQuestID, mit->second.m_iItemID, mit->second.m_iQuantity );
					}

					// 임시 인벤으로 지급되었기 때문에 실제 인벤에 관련된 데이터는 모두 지운다!
					kDBReq.m_mapInsertedItem.clear();
				}
				else
				{
					//인벤이 부족하면 그냥 유저에게 알려주고 완료를 하지 않는다.
					SET_ERROR( ERR_QUEST_13 );
					goto error_proc;
				}
#else
				//인벤이 부족하면 그냥 유저에게 알려주고 완료를 하지 않는다.
				SET_ERROR( ERR_QUEST_13 );
				goto error_proc;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
				//}}
			}

			//{{ 2007. 11. 25  최육사  일일통계 퀘스트 조건 아이템 사용통계
			std::map< int, int >::iterator mit = mapQuestConditionItem.begin();
			for( ; mit != mapQuestConditionItem.end(); mit++ )
			{
				KStatisticsKey kKey;
				kKey.m_vecIntKey.push_back( mit->first );
				KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, mit->second );
				//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
				KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
				//}}
			}
			//}}
		}

		//{{ 2007. 12. 26  최육사  DB 통계 퀘스트
		{
			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Complete, 1 );
			KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_CompleteLevel, static_cast<int>(spUser->GetLevel()) );

			//{{ 2007. 12. 26  최육사  유저 통계
			int iColumnIndex;
			//{{ 2010. 04. 01  최육사	비밀던전 헬모드
#ifdef SERV_DAILY_QUEST
			if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_NORMAL )
#else
			if( pQuestTemplet->m_bRepeat == false )
#endif SERV_DAILY_QUEST
				//}}
			{
				iColumnIndex = KUserStatistics::US_Etc_NQuestComplete;
			}
			else
			{
				iColumnIndex = KUserStatistics::US_Etc_RQuestComplete;
			}

			spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
			//}}
		}
		//}}

		spUser->SendToGameDB( DBE_QUEST_COMPLETE_REQ, kDBReq );
		return;
	}

error_proc:
	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( kReq.m_bAutoComplete )
	{
		// 초기화
		ClearAutoCompleteQuest();

		KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kAck;
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kAck );
	}
	else
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
	{
		KEGS_QUEST_COMPLETE_ACK kAck;
		//완료체크를 위해 퀘스트 ID를 다시 돌려준다.
		//{{ 2010. 04. 05  최육사	일일퀘스트
		//#ifdef SERV_DAILY_QUEST
		kAck.m_kCompleteQuestInfo.m_iQuestID = kReq.m_iQuestID;
		//#else
		//		kAck.m_iQuestID	= kReq.m_iQuestID;
		//#endif SERV_DAILY_QUEST
		//}}
		kAck.m_iOK	= NetError::GetLastError();
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kAck );
	}
}

#endif SERV_REFORM_QUEST
//}}

//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST

void KUserQuestManager::Handler_DBE_QUEST_COMPLETE_ACK( IN const KDBE_QUEST_COMPLETE_ACK& kAck, IN KGSUserPtr spUser )
{
	const int iQuestID = kAck.m_kCompleteQuestInfo.m_iQuestID;

	//DB에서 결과처리 실패한 경우
	if( kAck.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		if( kAck.m_bAutoComplete )
		{
			// 초기화
			ClearAutoCompleteQuest();

			KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacket;
			kPacket.m_iOK = kAck.m_iOK;
			spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacket );
		}
		else
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
		{
			KEGS_QUEST_COMPLETE_ACK	kPacket;
			kPacket.m_iOK = kAck.m_iOK;
			spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
		}
		return;
	}

	//DB처리 성공한 경우
	SET_ERROR( NET_OK );

	KEGS_QUEST_COMPLETE_ACK	kPacket;
	kPacket.m_kCompleteQuestInfo.m_iQuestID = iQuestID;

	if( IsQuest( iQuestID ) == false )
	{
		START_LOG( cerr, L"+ㅁ+;; 완료한 퀘스트가 진행중 퀘스트에 없네." )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );
		goto end_proc;
	}
	
	START_LOG( clog, L"퀘스트 완료 ::::" )
		<< BUILD_LOG( spUser->GetCharUID() )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( spUser->GetUID() )
#else
		<< BUILD_LOG( spUser->GetCharName() )
		<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kAck.m_iOK )
		<< BUILD_LOG( iQuestID );

	// Quest Instance 삭제
	if( RemoveQuest( iQuestID ) == true )
	{
		// 완료 처리
		std::map< int, KCompleteQuestInfo >::iterator mitCQ;
		mitCQ = m_mapCompleteQuest.find( iQuestID );
		if( mitCQ == m_mapCompleteQuest.end() )
		{
			m_mapCompleteQuest.insert( std::make_pair( iQuestID, kAck.m_kCompleteQuestInfo ) );
		}
		else
		{
			++(mitCQ->second.m_iCompleteCount); // 완료 카운트를 올리고..
			mitCQ->second.m_tCompleteDate = kAck.m_kCompleteQuestInfo.m_tCompleteDate; // 완료 날짜를 변경한다.
		}

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto end_proc;
		}		
		
		kPacket.m_iOK		= NetError::GetLastError();
		//spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );

		//전직처리.
		if( kAck.m_bIsChangeJob == true )
		{
			spUser->SetUnitClass( kAck.m_cChangeUnitClass );
			spUser->ResetStat();
		}

		//{{ 지헌 - 은행 개편 퀘스트, 캐쉬 작업 
#ifdef SERV_SHARING_BANK_QUEST_CASH
		// 퀘스트가 은행 확장 퀘스트라면, 은행 확장했다는 sp 를 호출하도록 하자
		if(pQuestTemplet->m_iID == 60710)
		{
			KDBE_SHARING_BACK_OPEN_REQ kPackToDB;
			kPackToDB.m_iUnitUID = spUser->GetCharUID();
			kPackToDB.m_iUserUID = spUser->GetUID();
			kPackToDB.m_iOpenType = KDBE_SHARING_BACK_OPEN_REQ::SBOT_QUEST;
			spUser->SendToGameDB(DBE_SHARING_BACK_OPEN_REQ, kPackToDB);
		}
#endif SERV_SHARING_BANK_QUEST_CASH
		//}}
		
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		spUser->IncreaseED( pQuestTemplet->m_Reward.m_iED, KUserEDManager::ER_IN_ED_QUEST_REWARD );
#else
		const int iBeforeED = spUser->GetED();

		//캐릭터 속성 보상처리
		spUser->m_iED	+= pQuestTemplet->m_Reward.m_iED;

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		spUser->CheckEDAbuser( KAbuserLogManager::ELS_QUEST_REWARD, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		// DB통계 ED
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PQuest, pQuestTemplet->m_Reward.m_iED );

		//{{ 2007. 12. 26  최육사  유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PQuest, pQuestTemplet->m_Reward.m_iED );
#else //SERV_USER_STATISTICS_RENEWAL
		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PQuestED, pQuestTemplet->m_Reward.m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL
		//}}

		//{{ 2009. 1. 20  최육사	만렙경험치 예외처리
		if( spUser->GetLevel() < SiKGameSysVal()->GetLimitsLevel() )
		{
			spUser->m_kEXP.AddExp( pQuestTemplet->m_Reward.m_iEXP );
		}
		//}}

#ifdef SERV_PVP_NEW_SYSTEM // 잘못된 디파인 해외팀 수정
		spUser->AddAPoint( pQuestTemplet->m_Reward.m_iSP );
#else
		spUser->m_iSPoint += pQuestTemplet->m_Reward.m_iSP;
#endif SERV_PVP_NEW_SYSTEM
	
		spUser->CheckCharLevelUp();

#ifdef SERV_POINT_COUNT_SYSTEM	
		if(spUser != NULL && pQuestTemplet != NULL )
		{			
			SetUpdateQuestInstance(spUser);
		}
#endif //SERV_POINT_COUNT_SYSTEM


		//아이템 보상처리
		if( kAck.m_bIsNew == true )
		{
			spUser->m_kInventory.InsertItem( kAck.m_mapItemInfo, kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo );
			kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo.insert( kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo.end(), kAck.m_vecUpdatedInventorySlot.begin(), kAck.m_vecUpdatedInventorySlot.end() );						
		}
		else
		{
			kPacket.m_kUpdateUnitInfo.m_vecKInventorySlotInfo	= kAck.m_vecUpdatedInventorySlot;
		}

		//획득한 아이템 정보를 클라이언트용으로 전환하여 준다.
		std::map< int, KItemInfo >::const_iterator mitITEM;
		for( mitITEM = kAck.m_mapInsertedItem.begin(); mitITEM != kAck.m_mapInsertedItem.end(); ++mitITEM )
		{
			kPacket.m_kUpdateUnitInfo.m_mapItemObtained.insert( std::make_pair( mitITEM->second.m_iItemID, mitITEM->second.m_iQuantity ) );
		}

		spUser->GetUnitInfo( kPacket.m_kUpdateUnitInfo.m_kUnitInfo );
		kPacket.m_kUpdateUnitInfo.m_kUnitInfo.m_nNowBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)spUser->GetLevel() );
		kPacket.m_kUpdateUnitInfo.m_kUnitInfo.m_nNextBaseLevelEXP	= SiKExpTable()->GetRequireTotalExpbyLevel( (int)(spUser->GetLevel() + 1) );

		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		if( kAck.m_bAutoComplete )
		{
			AddAutoCompleteQuestResult( kPacket );
		}
		else
		{
			spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
		}
#else
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}

		// 통계 : Character Info, Quest
		{
			int iCount = 0;
			std::map< int, int >::iterator iO;
			for ( iO = kPacket.m_kUpdateUnitInfo.m_mapItemObtained.begin(); iO != kPacket.m_kUpdateUnitInfo.m_mapItemObtained.end(); ++iO )
			{
				iCount += (*iO).second;
			}

			// 유저 통계 퀘스트 보상 아이템 카운트
			m_iRewardItemCount += iCount;
		}

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// 아이템 어뷰저
		spUser->CheckItemAbuser( KAbuserLogManager::RS_QUEST, kPacket.m_kUpdateUnitInfo.m_mapItemObtained );
#endif SERV_USER_ABUSER_MANAGER
		//}}

		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		// 완료된 퀘스트의 AfterQuestID를 체크하여 새로운 퀘스트를 오픈한다!
		CheckEpicQuest_NewQuestByAfterQuest( pQuestTemplet->m_iID, spUser, true, true );

		// BeforeQuestID로 등록된 열리지 않은 완료 퀘스트가 있는지 확인하여 퀘스트를 오픈한다.
		CheckEpicQuest_NewQuestByBeforeQuest( spUser, true );	

		// 이벤트 반복 퀘스트라면 재수락 받도록 하자 
		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT
		 && pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_REPEAT )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pQuestTemplet->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, false ) == false )
			{
				START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(After)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< END_LOG;
			}
		}
#endif SERV_REFORM_QUEST
		//}}

		return;		
	}	

end_proc:
	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	if( kAck.m_bAutoComplete )
	{
		// 초기화
		ClearAutoCompleteQuest();

		KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		spUser->SendPacket( EGS_ALL_COMPLETED_QUEST_COMPLETE_ACK, kPacketAck );
	}
	else
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	{
		kPacket.m_iOK = NetError::GetLastError();
		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );
	}
}

#endif SERV_DAILY_QUEST
//}}

bool KUserQuestManager::IsQuest( const IN int iQuestID )
{
	return (GetQuestInstance( iQuestID ) != NULL );
}

bool KUserQuestManager::IsChangeJobQuest()
{
	std::map<int, KQuestInstance>::iterator	mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );

		if( pQuestTemplet != NULL )
		{
			if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
			{
				return true;
			}
		}
		else
		{
			START_LOG( cerr, L"퀘스트 템플릿 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}

	return false;
}

//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST

bool KUserQuestManager::IsExistEventQuest( IN int iQuestID )
{
	std::map< int, KEventQuestInfo >::const_iterator mit;
	for( mit = m_mapEventQuest.begin(); mit != m_mapEventQuest.end(); ++mit )
	{
        if( mit->second.m_iQuestID == iQuestID )
			return true;
	}

	return false;
}

#endif SERV_DAY_QUEST
//}}

KQuestInstance* KUserQuestManager::GetQuestInstance( const IN int iQuestID )
{
	std::map<int, KQuestInstance>::iterator	mit;
	mit = m_mapQuesting.find( iQuestID );

	if( mit == m_mapQuesting.end() )
		return NULL;

	return &(mit->second);
}

//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST

int KUserQuestManager::GetQuestCompleteCount( IN const int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::const_iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );
	if( mit == m_mapCompleteQuest.end() )
	{
		return 0;
	}

	return mit->second.m_iCompleteCount;
}

#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
bool KUserQuestManager::CheckCompletedQuestToday( IN const int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::const_iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );
	if( mit == m_mapCompleteQuest.end() )
	{
		return false;
	}

	CTime tCompleteDate( mit->second.m_tCompleteDate ); // 이전 퀘스트의 완료 시간
	CTime tCurTime = CTime::GetCurrentTime();			// 현재 시간
	CTime tCheckTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );	// 오늘의 퀘스트 갱신 시간
	if( tCurTime.GetHour() < CXSLQuestManager::QE_DAILY_REPEAT_HOUR )	// 현재 시 < 갱신 시
	{
		// 6시보다 작다면 시각이라면 이전날로 세팅
		tCheckTime -= CTimeSpan( 1, 0, 0, 0 );
	}

	// 오늘 완료를 했다면
	if( tCheckTime < tCompleteDate )
	{
		return true;
	}

	return false; 
}
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX

#else

int KUserQuestManager::GetQuestCompleteCount( IN const int iQuestID )
{
	std::map<int,int>::iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );

	if( mit == m_mapCompleteQuest.end() )
		return 0;

	return (mit->second);
}

#endif SERV_DAILY_QUEST
//}}

void KUserQuestManager::GetUpdatedQuestInfo( OUT std::vector< KQuestUpdate >& vecQuestData )
{
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const KQuestInstance& kQuest = mit->second;

		KQuestUpdate kQuestUpdate;
		kQuestUpdate.m_iQuestID = kQuest.m_iID;

		std::vector< KSubQuestInstance >::const_iterator vit;
		vit = kQuest.m_vecSubQuestInstance.begin();
		//db에 넘어가야 할 데이터는 정해져 있기때문에(5개) 없으면 0으로 채운다.
		for( int i = 0; i < CXSLQuestManager::MAX_SUB_QUEST_NUM; ++i )
		{
			if( vit != kQuest.m_vecSubQuestInstance.end() )
			{
				kQuestUpdate.m_vecClearData.push_back( vit->m_ucClearData );

				++vit;
			}
			else
			{
				kQuestUpdate.m_vecClearData.push_back( 0 );
			}
		}

		vecQuestData.push_back( kQuestUpdate );
	}
}

int	KUserQuestManager::GetQuestRewardItemCount()
{
	int iQuestRewardItemCount = m_iRewardItemCount;
	m_iRewardItemCount = 0;
	return iQuestRewardItemCount;
}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::GetOngoingQuestForRoom( IN KGSUserPtr spUser, OUT std::map< int, KSubQuestInfo >& mapOngoingQuest )
{
	mapOngoingQuest.clear();

	std::map< int, KQuestInstance >::iterator mitQ;

	//진행중 퀘스트 검사
	for( mitQ = m_mapQuesting.begin(); mitQ != m_mapQuesting.end(); ++mitQ )
	{
		const KQuestInstance& kQuestInstance = mitQ->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET가 없음." )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
				<< END_LOG;

			continue;
		}

		// PC방 전용 퀘스트인데 PC방 접속이 아니라면 드롭퀘스트리스트에 넣지 않는다.
		if( pQuestTemplet->m_bIsPcBang == true )
		{
			if( spUser->IsPcBang() == false )
				continue;
		}

		//서브퀘스트중 퀘스트 아이템 수집이 있는지 찾는다.
		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			KSubQuestInstance kSub;

			const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET가 없음." )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
					<< END_LOG;

				continue;
			}

			switch( pSubTemplet->m_eClearType )
			{
			case CXSLQuestManager::SQT_ITEM_COLLECTION:
			case CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION:
				{
					std::map< int, KSubQuestInfo >::iterator mitQ;
					mitQ = mapOngoingQuest.find( kQuestInstance.m_iID );

					//아직 없는 퀘스트라면 mapDropQuestitembyIngQuest 에다 바로 넣어준다.
					if( mitQ == mapOngoingQuest.end() )
					{
						KSubQuestInfo kInfo;

						// 해당 퀘스트의 서브 퀘스트들의 data를 전부 담는다.
                        for( u_int uiSub = 0; uiSub < kQuestInstance.m_vecSubQuestInstance.size(); ++uiSub )
						{
							int iQuantity = 0;
							//해당아이템의 수량확인
							if( spUser->m_kInventory.GetQuantity( pSubTemplet->m_ClearCondition.m_iItemID, iQuantity ) == true )
							{								
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = iQuantity;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiSub].m_bIsSuccess;
								kInfo.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiSub].m_iID, kData ) );

							}
							else
							{
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = 0;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiSub].m_bIsSuccess;
								kInfo.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiSub].m_iID, kData ) );
							}
						}

						mapOngoingQuest.insert( std::make_pair( kQuestInstance.m_iID, kInfo ) );
					}
					else	// 정보를 최신화 한다.
					{
						std::map< int, KSubQuestData>::iterator mitS = mitQ->second.m_mapSubQuestInfo.find( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID );
						if( mitS == mitQ->second.m_mapSubQuestInfo.end() )
						{
							int iQuantity = 0;
							//해당아이템의 수량확인
							if( spUser->m_kInventory.GetQuantity( pSubTemplet->m_ClearCondition.m_iItemID, iQuantity ) == true )
							{								
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = iQuantity;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
								mitQ->second.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID, kData ) );

							}
							else
							{
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = 0;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
								mitQ->second.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID, kData ) );
							}
						}
						else
						{
							int iQuantity = 0;
							//해당아이템의 수량확인
							if( spUser->m_kInventory.GetQuantity( pSubTemplet->m_ClearCondition.m_iItemID, iQuantity ) == true )
							{																	
								mitS->second.m_iInInventoryItemCount = iQuantity;
								mitS->second.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
							}
							else
							{
								mitS->second.m_iInInventoryItemCount = 0;
								mitS->second.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
							}
						}
					}
				}
				break;

			case CXSLQuestManager::SQT_NPC_HUNT:
			case CXSLQuestManager::SQT_NPC_TALK:
			case CXSLQuestManager::SQT_DUNGEON_TIME:
			case CXSLQuestManager::SQT_DUNGEON_RANK:
			case CXSLQuestManager::SQT_DUNGEON_DAMAGE:
			case CXSLQuestManager::SQT_DUNGEON_CLEAR_COUNT:
			case CXSLQuestManager::SQT_PVP_PLAY:
			case CXSLQuestManager::SQT_PVP_WIN:
			case CXSLQuestManager::SQT_PVP_KILL:
			case CXSLQuestManager::SQT_WITH_DIF_SERV_USER:
			case CXSLQuestManager::SQT_ITEM_ENCHANT:
			case CXSLQuestManager::SQT_ITEM_SOCKET:
			case CXSLQuestManager::SQT_ITEM_ATTRIB:
			case CXSLQuestManager::SQT_ITEM_RESOLVE:
			case CXSLQuestManager::SQT_ITEM_EQUIP_DUNGEON_CLEAR:
			case CXSLQuestManager::SQT_USE_SKILL_POINT:
			case CXSLQuestManager::SQT_FEED_PET:
			case CXSLQuestManager::SQT_USER_DIE:
			case CXSLQuestManager::SQT_PVP_NPC_HUNT:
			case CXSLQuestManager::SQT_ITEM_USE:
			case CXSLQuestManager::SQT_SKILL_USE:
			case CXSLQuestManager::SQT_VISIT_VILLAGE:
			case CXSLQuestManager::SQT_VISIT_FIELD:
			case CXSLQuestManager::SQT_VISIT_DUNGEON:
			case CXSLQuestManager::SQT_FIND_NPC:
				//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
			case CXSLQuestManager::SQT_LEARN_NEW_SKILL:
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL
				//}}
				{
					std::map< int, KSubQuestInfo >::iterator mitQ;
					mitQ = mapOngoingQuest.find( kQuestInstance.m_iID );

					//아직 없는 퀘스트라면 mapDropQuestitembyIngQuest 에다 바로 넣어준다.
					if( mitQ == mapOngoingQuest.end() )
					{
						KSubQuestInfo kInfo;

						// 해당 퀘스트의 서브 퀘스트들의 data를 전부 담는다.
						for( u_int uiSub = 0; uiSub < kQuestInstance.m_vecSubQuestInstance.size(); ++uiSub )
						{
							KSubQuestData kData;
							kData.m_iInInventoryItemCount = 0;
							kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiSub].m_bIsSuccess;
							kInfo.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiSub].m_iID, kData ) );
						}

						mapOngoingQuest.insert( std::make_pair( kQuestInstance.m_iID, kInfo ) );
					}
					//이미 있는 퀘스트라면 같은 퀘스트의 서브항목에 채워준다.
					else
					{
						std::map< int, KSubQuestData>::iterator mitS = mitQ->second.m_mapSubQuestInfo.find( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID );
						if( mitS == mitQ->second.m_mapSubQuestInfo.end() )
						{
								KSubQuestData kData;
								kData.m_iInInventoryItemCount = 0;
								kData.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
								mitQ->second.m_mapSubQuestInfo.insert( std::make_pair( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_iID, kData ) );
						}
						else
						{
							mitS->second.m_iInInventoryItemCount = 0;
							mitS->second.m_bSussece = kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess;
						}
					}
				}
				break;
			}
		}
	}
}

#else
	//void KUserQuestManager::GetDropQuestitembyIngQuest( OUT std::map< int, KSubQuestInfo >& mapDropQuestitembyIngQuest, IN KGSUserPtr spUser )
#endif SERV_REFORM_QUEST
//}}

//{{ 2010. 02. 06  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST

void KUserQuestManager::InitEventQuest( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	// 이벤트 퀘스트 초기화
	CheckQuestEvent( spUser );

	// 초기화 되지 못한 이벤트 퀘스트는 삭제한다!
	std::vector< int > vecDeleteEventQuest;

	std::map< int, KEventQuestInfo >::iterator mit;
	for( mit = m_mapEventQuest.begin(); mit != m_mapEventQuest.end(); ++mit )
	{		
		if( mit->second.m_bInit == false )
		{
            vecDeleteEventQuest.push_back( mit->first );
		}
	}

	for( u_int ui = 0; ui < vecDeleteEventQuest.size(); ++ui )
	{
		m_mapEventQuest.erase( vecDeleteEventQuest[ui] );
	}

	// 현재 진행중인 이벤트 퀘스트중에 초기화 안된 퀘스트를 찾아낸다.
	std::vector< int > vecDeletedQuest;

	std::map< int, KQuestInstance >::const_iterator mitQT;
	for( mitQT = m_mapQuesting.begin(); mitQT != m_mapQuesting.end(); ++mitQT )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitQT->second.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"퀘스트 템플릿 정보가 없습니다." )
				<< BUILD_LOG( mitQT->second.m_iID )
				<< END_LOG;

			continue;
		}
		
		// 이벤트 퀘스트인지 검사
		if( pQuestTemplet->m_bIsTimeEvent == false )
			continue;

		// 초기화된 이벤트 퀘스트인지 검사
		if( IsExistEventQuest( mitQT->second.m_iID ) == true )
			continue;

        // 초기화 되지 않았다면 진행중인 퀘스트에서 삭제하자!
		vecDeletedQuest.push_back( mitQT->second.m_iID );
	}

	for( u_int ui = 0; ui < vecDeletedQuest.size(); ++ui )
	{
		RemoveQuest( vecDeletedQuest[ui] );

		// DB에서 삭제 예약
		m_vecReservedGiveUpQuest.push_back( vecDeletedQuest[ui] );
	}

	if( !vecDeletedQuest.empty() )
	{
		KEGS_EVENT_QUEST_INFO_NOT kPacketNotEnd;
		kPacketNotEnd.m_cType = KEGS_EVENT_QUEST_INFO_NOT::EQIT_END;
		kPacketNotEnd.m_vecQuestID = vecDeletedQuest;
		spUser->SendPacket( EGS_EVENT_QUEST_INFO_NOT, kPacketNotEnd );
	}
}

void KUserQuestManager::CheckQuestEvent( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	KEGS_EVENT_QUEST_INFO_NOT kPacketNotBegin;
	KEGS_EVENT_QUEST_INFO_NOT kPacketNotEnd;

    // GameEventManager로부터 현재 진행중인 퀘스트 이벤트가 있는지 체크한다!
	SiKGameEventManager()->CheckEnableEventQuest( m_mapEventQuest, m_mapCompleteQuest, kPacketNotBegin.m_vecQuestID, kPacketNotEnd.m_vecQuestID );

	// 종료되는 이벤트가 있다면 역시 클라이언트에 알린다.
	if( !kPacketNotEnd.m_vecQuestID.empty() )
	{
		for( u_int ui = 0; ui < kPacketNotEnd.m_vecQuestID.size(); ++ui )
		{
			const int iEndQuestID = kPacketNotEnd.m_vecQuestID[ui];

			// 종료된 이벤트 퀘스트가 현재 진행중이라면..
			if( IsQuest( iEndQuestID ) == true )
			{
#ifdef SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
				KEGS_GIVE_UP_QUEST_REQ kPacket;
				kPacket.m_iQuestID = iEndQuestID;
				Handler_EGS_GIVE_UP_QUEST_REQ( kPacket, spUser );
#else // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
				// 삭제처리
				RemoveQuest( iEndQuestID );

				// DB에서 삭제 예약
				m_vecReservedGiveUpQuest.push_back( iEndQuestID );
#endif // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT

				// 방상태라면 드롭 퀘스트 정보 업데이트 하기 - 퀘스트중에 드롭 아이템 퀘스트는 없으므로 아직은 필요없음.
				//spUser->SendUpdateDropQuestItemByIngQuest();
			}
		}

		kPacketNotEnd.m_cType = KEGS_EVENT_QUEST_INFO_NOT::EQIT_END;
		spUser->SendPacket( EGS_EVENT_QUEST_INFO_NOT, kPacketNotEnd );
	}

	// 새로 시작되는 이벤트가 있다면 클라이언트에 알리고
	if( !kPacketNotBegin.m_vecQuestID.empty() )
	{
#ifdef SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
		BOOST_TEST_FOREACH( const int&, iQuestID, kPacketNotBegin.m_vecQuestID )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = iQuestID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, false ) == false )
			{
				START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(After)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( iQuestID )
					<< END_LOG;
			}
		}
#endif // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT

		kPacketNotBegin.m_cType = KEGS_EVENT_QUEST_INFO_NOT::EQIT_BEGIN;
		spUser->SendPacket( EGS_EVENT_QUEST_INFO_NOT, kPacketNotBegin );
	}

#ifdef SERV_CHECK_TIME_QUEST
	if( spUser->GetAuthLevel() == SEnum::UAL_DEVELOPER )
	{
		// 여기서 퀘스트 이벤트 체크 해서, TimeEvent 셋팅 안된 퀘스트들 알려주자
		KEGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT kPacket;

		SiKGameEventManager()->GetWarningEventQuest( kPacket );
		if( false == kPacket.m_vecWarningQuestID.empty() )
		{
			// 클라에 보내자
			spUser->SendPacket( EGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT, kPacket );
		}
	}
#endif // SERV_CHECK_TIME_QUEST
}

void KUserQuestManager::GetReservedGiveUpQuest( OUT std::vector< int >& vecGiveUpQuest )
{
	vecGiveUpQuest.clear();
    
	vecGiveUpQuest = m_vecReservedGiveUpQuest;
	m_vecReservedGiveUpQuest.clear();
}

#endif SERV_DAY_QUEST
//}}

//{{ 2010. 04. 05  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST

KCompleteQuestInfo* KUserQuestManager::GetCompleteQuestInfo( IN int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::iterator mit;
	mit = m_mapCompleteQuest.find( iQuestID );
	if( mit == m_mapCompleteQuest.end() )
		return NULL;

	return &(mit->second);
}

#endif SERV_DAILY_QUEST
//}}

#ifdef SERV_EPIC_QUEST
void KUserQuestManager::CheckEpicQuest_SuccessStateChange( IN int iMapID, IN KGSUserPtr spUser )
{
	//# iMapID 에 대한 유효성 검사는 
	//# EGS_STATE_CHANGE_FIELD_REQ 에서 하고 들어왔기 때문에 하지 않는다.
	std::vector<CXSLQuestManager::QuestTemplet*>::const_iterator cvit;
	std::vector<CXSLQuestManager::QuestTemplet*> vecEpicList;

	SiCXSLQuestManager()->GetEpicQuestTempletList(vecEpicList);

	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pTemplet, vecEpicList )
	{
		if( NULL == pTemplet )
			continue;

		// 진행중 퀘스트 X
		if( IsQuest( pTemplet->m_iID ) == true )
			continue;

		// 완료된 퀘스트 X
		if( IsCompleteQuest( pTemplet->m_iID ) == true )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
		if ( SiKGameEventManager()->IsEnableEventQuest(pTemplet->m_iID) == false )
			continue; // 2013.06.10 lygan_조성욱 // 어차피 활성화 되지 않은 퀘스트는 AfterQuest 를 찾을 필요가 없다
#endif //SERV_ALLOW_EVENT_ERASE

		// 마을 아이디가 값으면.
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		if( pTemplet->m_Condition.m_setEnableVillage.size() > 0 )
		{
			std::set<int>::const_iterator sit = pTemplet->m_Condition.m_setEnableVillage.find( iMapID );
			if( sit == pTemplet->m_Condition.m_setEnableVillage.end() )
				continue;
		}
		else
			continue;
#else
		if( iMapID != pTemplet->m_Condition.m_iEnableVillage )
			continue;
#endif SERV_REFORM_QUEST
		//}}
				
		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pTemplet->m_iID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

		if( false == Handler_EGS_NEW_QUEST_REQ(kTempReq, spUser) )
		{
			START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(State Change)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pTemplet->m_iID )
				<< BUILD_LOG( iMapID )
				<< END_LOG;
		}		
	}
}

#endif SERV_EPIC_QUEST

//{{ 2011. 01. 03	최육사	에픽 퀘스트
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE

void KUserQuestManager::CheckEpicQuest_NewQuestByBeforeQuest( IN KGSUserPtr spUser, IN const bool bSendAck )
{
	//{{Begin} 퀘스트 클리어시 에픽퀘스트중 BeforeQuestID를 검사하여 자동수락 해주는 기능작성.
	std::vector< CXSLQuestManager::QuestTemplet* > vecEpicQuestTemplet;
	SiCXSLQuestManager()->GetExistBeforeEpicQuestTempletList( vecEpicQuestTemplet );

	// 에픽 퀘스트 리스트를 순회하면서 BeforeQuestID 를 검사함.
	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pEpicQuest, vecEpicQuestTemplet )
	{
		if( pEpicQuest == NULL )
		{
			START_LOG( cerr, L"에픽 퀘스트 포인터값이 이상합니다!" )
				<< BUILD_LOG( vecEpicQuestTemplet.size() )
				<< END_LOG;
			continue;
		}	

		// 완료중 이면 검사에서 제외
		if( IsCompleteQuest( pEpicQuest->m_iID ) == true )
			continue;

		// 진행중 이면 검사에서 제외
		if( IsQuest( pEpicQuest->m_iID ) == true )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
		if ( SiKGameEventManager()->IsEnableEventQuest(pEpicQuest->m_iID) == false )
			continue; // 2013.06.10 lygan_조성욱 // 어차피 활성화 되지 않은 퀘스트는 AfterQuest 를 찾을 필요가 없다
#endif //SERV_ALLOW_EVENT_ERASE

		// 아직 받지않은 에픽중 받을수 있는 퀘스트를 체크
		bool bIsSuccessEpic = true;

		// oasis907 : 김상윤 [2010.7.10] // m_vecBeforeQuestID의 내용이 비었을시 검사에서 제외
		if( pEpicQuest->m_Condition.m_vecBeforeQuestID.size() == 0 )
		{
			bIsSuccessEpic = false;
		}

		BOOST_TEST_FOREACH( const int, iBeforeQuestID, pEpicQuest->m_Condition.m_vecBeforeQuestID )
		{
			if( iBeforeQuestID != 0  &&  IsCompleteQuest( iBeforeQuestID ) == false )
			{
				bIsSuccessEpic = false;
				break;
			}
		}

		if( bIsSuccessEpic )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pEpicQuest->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, bSendAck ) == false )
#endif SERV_REFORM_QUEST
			//}}
			{
				START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(Before)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( kTempReq.m_iQuestID )
					<< END_LOG;
			}
		}		
	}
}

void KUserQuestManager::CheckEpicQuest_NewQuestByAfterQuest( IN const int iQuestID, IN KGSUserPtr spUser, IN const bool bSendAck, IN const bool bForce )
{
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cwarn, L"존재하지 않는 퀘스트 ID입니다!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;
		return;
	}

	//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	// 완료된 퀘스트가 에픽이라면
	// 	if( CXSLQuestManager::QT_EPIC != pQuestTemplet->m_eQuestType )
	// 		return;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	// 연결퀘스트가 존재하지 않는다면
	if( pQuestTemplet->m_vecAfterQuestID.size() <= 0 )
		return;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
	if ( SiKGameEventManager()->IsEnableEventQuest(iQuestID) == false )
		return; // 2013.06.10 lygan_조성욱 // 어차피 활성화 되지 않은 퀘스트는 AfterQuest 를 찾을 필요가 없다
#endif //SERV_ALLOW_EVENT_ERASE

	// 다음 퀘스트가 에픽퀘스트인지 검사
	BOOST_TEST_FOREACH( const int, iAfterQuestID, pQuestTemplet->m_vecAfterQuestID )
	{
		//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iAfterQuestID );
		if( IS_NULL( pAfterQuestTemplet ) )
		{
			START_LOG( cerr, L"존재 하지 않는 퀘스트 정보입니다!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( iAfterQuestID )
				<< END_LOG;
			continue;
		}

		// after퀘스트가 반복이냐 아니냐를 체크!
		if( pAfterQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_REPEAT 
#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
			&& pAfterQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_DAY
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX
		)
		{
			// after 가 셋팅 되어 있으면 종류 타입에 상관없이 모두 수락 받도록 하자
			if( bForce == false )	// 강제 수락이 아니라면
			{
				// 에픽과 이벤트는 수락하게 한다.
				if( ( pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EPIC || pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT ) == false )
				{
					START_LOG( clog, L"강제 수락이 아닌데(캐릭터선택시)...After Quest 로 셋팅된 퀘스트가 에픽, 이벤트가 아니다." )
						<< BUILD_LOG( iAfterQuestID )
						<< END_LOG;
					continue;
				}
			}
			
			// 완료중 이면 검사에서 제외
			if( IsCompleteQuest( iAfterQuestID ) == true )
				continue;
		}
#else
		//if( SiCXSLQuestManager()->IsEpicQuest( iAfterQuestID ) == false )
		//{
		//	START_LOG( clog, L"에픽의 다음 퀘스트가 에픽타입이 아님" )
		//		<< BUILD_LOG( iAfterQuestID )
		//		<< END_LOG;
		//	continue;
		//}

		//// 완료중 이면 검사에서 제외
		//if( IsCompleteQuest( iAfterQuestID ) == true )
		//	return;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		// 진행중 이면 검사에서 제외
		if( IsQuest( iAfterQuestID ) == true )
			continue;
	
		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = iAfterQuestID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
		if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, bSendAck ) == false )
		{
			START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(After)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;
		}
	}

#else
	// 연결퀘스트가 존재한다면
	if( pQuestTemplet->m_iAfterQuestID == 0 )
		return;

	// 다음 퀘스트가 에픽퀘스트인지 검사
	if( SiCXSLQuestManager()->IsEpicQuest( pQuestTemplet->m_iAfterQuestID ) == false )
	{
		START_LOG( cerr, L"에픽의 다음 퀘스트가 에픽타입이 아님" )
			<< BUILD_LOG( pQuestTemplet->m_iAfterQuestID )
			<< END_LOG;
		return;
	}

	// 완료중 이면 검사에서 제외
	if( IsCompleteQuest( pQuestTemplet->m_iAfterQuestID ) == true )
		return;

	// 진행중 이면 검사에서 제외
	if( IsQuest( pQuestTemplet->m_iAfterQuestID ) == true )
		return;

	{
		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pQuestTemplet->m_iAfterQuestID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
		if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, bSendAck ) == false )
		{
			START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(After)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pQuestTemplet->m_iID )
				<< END_LOG;
		}
	}
#endif SERV_REFORM_QUEST
	//}}
}

#endif SERV_EPIC_QUEST_CHECK_UPDATE
//}}

void KUserQuestManager::ClearUnitQuest()
{
	m_mapQuesting.clear();
	m_mapCompleteQuest.clear();
	//{{ 2010. 02. 06  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST
	m_mapEventQuest.clear();
	m_vecReservedGiveUpQuest.clear();
#endif SERV_DAY_QUEST
	//}}
	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	ClearAutoCompleteQuest();
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
}


#ifdef SERV_EPIC_QUEST
int KUserQuestManager::GetNormalQuestSize()
{
	int iSize = 0;
	std::map< int, KQuestInstance >::const_iterator cmit;

	cmit = m_mapQuesting.begin();

	while(cmit != m_mapQuesting.end())
	{
		const CXSLQuestManager::QuestTemplet *pTemplet = SiCXSLQuestManager()->GetQuestTemplet(cmit->second.m_iID);
		if( NULL != pTemplet 
		 && CXSLQuestManager::QT_EPIC != pTemplet->m_eQuestType 
		 && CXSLQuestManager::QT_EVENT != pTemplet->m_eQuestType )
			++iSize;

		++cmit;
	}

	return iSize;
}
#endif SERV_EPIC_QUEST

//{{ 2011. 03. 08	김민성 경험치를 획득한 던전의 퀘스트 완료 조건 변경
#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
bool KUserQuestManager::ExistHaveExpInDungeon( IN UidType CharUID, IN const std::map< UidType, bool >& mapHaveExpInDungeon )
{
	std::map< UidType, bool >::const_iterator mit = mapHaveExpInDungeon.find( CharUID );
	if( mit == mapHaveExpInDungeon.end() )
	{
		START_LOG( cwarn, L"경험치 정보가 없다. 경험치를 못얻는 유저인가?" )
			<< BUILD_LOG( CharUID )
			<< BUILD_LOG( mapHaveExpInDungeon.size() )
			<< END_LOG;
		return false;
	}

	return mit->second;
}
#endif SERV_EXP_ACQUISITION_CONDITION_CHANGE
//}}

//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
void KUserQuestManager::GetUserAllQuest( OUT std::set< int >& mapPaymentQuest )
{
	mapPaymentQuest.clear();

	std::map< int, KQuestInstance >::iterator mitQ;

	//진행중 퀘스트 검사
	for( mitQ = m_mapQuesting.begin(); mitQ != m_mapQuesting.end(); ++mitQ )
	{
		const KQuestInstance& kQuestInstance = mitQ->second;
		mapPaymentQuest.insert( kQuestInstance.m_iID );
	}
}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
void KUserQuestManager::GetUserGoingQuest( IN KGSUserPtr spUser, OUT std::set< int >& mapPaymentQuest )
{
	mapPaymentQuest.clear();

	std::map< int, KQuestInstance >::iterator mitQ;

	//진행중 퀘스트 검사
	for( mitQ = m_mapQuesting.begin(); mitQ != m_mapQuesting.end(); ++mitQ )
	{
		const KQuestInstance& kQuestInstance = mitQ->second;

		if( CheckIsGoingComplete( kQuestInstance.m_iID, spUser ) == true )
			mapPaymentQuest.insert( kQuestInstance.m_iID );
	}
}
bool KUserQuestManager::CheckIsGoingComplete( IN const int iQuestID, IN KGSUserPtr spUser )
{
	if( m_mapQuesting.empty() == true )
	{
		return false;
	}

	// 수행중인지 확인한다.	
	if( IsQuest( iQuestID ) == false )
	{
		return false;
	}

	// 퀘스트 인스턴스 얻기
	KQuestInstance* pKQuestInst = GetQuestInstance( iQuestID );
	if( pKQuestInst == NULL )
	{
		return false;
	}

	// 퀘스트 템플릿 얻기
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"퀘스트 진행중인데 템플릿 데이터가 없다." )
			<< BUILD_LOG( iQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetUID() )
#else
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		return false;
	}

	// 에픽 퀘스트 일경우 레벨이 안되면 퀘스트 완료 할수없음.
	if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
		pQuestTemplet->m_Condition.m_iLevel > spUser->GetLevel() )
		return false;

	for( int iIndexSub = 0; iIndexSub < (int)pKQuestInst->m_vecSubQuestInstance.size(); ++iIndexSub )
	{
		// item 서브 퀘스트 수행완료 체크를 별도로 넣어야함.(완료체크시에만 하기때문)
		// 수집한 아이템 변동이 어떻게 될지 몰라 완료체크를 하는 순간에만 체크해야 하기 때문
		const int iSubQuestID = pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_iID;

		const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( iSubQuestID );
		if( pSubQuestTemplet == NULL )
		{
			START_LOG( cerr, L"퀘스트 진행중인데 서브템플릿 데이터가 없다." )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( iSubQuestID )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			return false;
		}

		if( pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION  ||
			pSubQuestTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
		{
			//수집해야할 아이템(개수만큼) 있는지 확인하여 서브퀘스트 완료체크를 한다.
			std::map<int,int> mapItemInfo;
			mapItemInfo.insert( std::make_pair( pSubQuestTemplet->m_ClearCondition.m_iItemID, pSubQuestTemplet->m_ClearCondition.m_iCollectionItemNum ) );

			//{{ 2009. 8. 7  최육사		은행
			if( spUser->IsEnoughItemExist( mapItemInfo, false ) == true )
			{
				return false;
			}
			//}}
		}
		else
		{
			if( pKQuestInst->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
			{
				return false;
			}
		}
	}

	return true;
}
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

#ifdef SERV_SUB_QUEST_USE_ITEM
void KUserQuestManager::CountUseItem( IN KGSUserPtr spUser, IN const int iDungeonID, IN const char cDifficulty, IN const int iItemID )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		if( pQuestTemplet->m_vecSubQuest.empty() == true )		
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 아이템 사용 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_ITEM_USE )
				continue;

			// 던전이 존재한다면
			if( pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() == false )
			{
				if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetDungeonIDAndDungeonDifficulty(), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
					continue;
			}
			// 4. 해당 배틀 필드가 맞는지 확인!
			else
			{
				// 배틀 필드가 존재한다면
				if( pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
						continue;
				}
			}

			// 3. 어떤 아이템인지?
			std::vector<int>::const_iterator it;
			for( it = pSubQuestTemplet->m_ClearCondition.m_vecUseItemID.begin(); 
				it != pSubQuestTemplet->m_ClearCondition.m_vecUseItemID.end(); ++it )
			{
				if( (*it) == iItemID )
				{
					if( pSubQuestTemplet->m_ClearCondition.m_iUseItemNum > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
						kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
				}
			}

			if( pSubQuestTemplet->m_ClearCondition.m_iUseItemNum <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );
		}			
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_SUB_QUEST_USE_ITEM

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
void KUserQuestManager::GetCompleteChangeClassQuest( OUT std::vector< KCompleteQuestInfo >& vecQuest )
{
	std::map<int, KCompleteQuestInfo>::iterator	mit;
	for( mit = m_mapCompleteQuest.begin(); mit != m_mapCompleteQuest.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->second.m_iQuestID );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( mit->second );
		}
		else
		{
			START_LOG( cerr, L"퀘스트 템플릿 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::GetInProgressClassChangeQuest( OUT std::vector< KQuestInstance >& vecQuest )
{
	std::map<int, KQuestInstance>::iterator	mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( mit->second );
		}
		else
		{
			START_LOG( cerr, L"퀘스트 템플릿 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::GetCompleteChangeClassQuest( OUT std::vector< int >& vecQuest )
{
	std::map<int, KCompleteQuestInfo>::iterator	mit;
	for( mit = m_mapCompleteQuest.begin(); mit != m_mapCompleteQuest.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->second.m_iQuestID );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( pQuestTemplet->m_iID );
		}
		else
		{
			START_LOG( cerr, L"퀘스트 템플릿 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::GetInProgressClassChangeQuest( OUT std::vector< int >& vecQuest )
{
	std::map<int, KQuestInstance>::iterator	mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );

		if( pQuestTemplet != NULL )
		{
			vecQuest.push_back( pQuestTemplet->m_iID );
		}
		else
		{
			START_LOG( cerr, L"퀘스트 템플릿 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			break;
		}
	}
}

void KUserQuestManager::SetClassChangeQuest( IN UidType iUnitUID, IN std::map< int, int >& mapComplete, IN std::map< int, int >& mapInProgress )
{
	std::map< int, KCompleteQuestInfo >::iterator mitMyComplete;
	std::map< int, KQuestInstance >::iterator mitMyInProgress;
	CTime tCurr = CTime::GetCurrentTime();


	// 완료된 퀘스트 변환
	std::map< int, int >::iterator mitComplete = mapComplete.begin();
	for(  ; mitComplete != mapComplete.end() ; ++mitComplete )
	{
		mitMyComplete = m_mapCompleteQuest.find( mitComplete->first );
		if( mitMyComplete != m_mapCompleteQuest.end() )
		{
			KCompleteQuestInfo kTempQuestInfo;
			kTempQuestInfo = mitMyComplete->second;
			kTempQuestInfo.m_iQuestID = mitComplete->second;

			m_mapCompleteQuest.insert( std::make_pair( kTempQuestInfo.m_iQuestID, kTempQuestInfo ) );
			m_mapCompleteQuest.erase( mitMyComplete );
		}
		else
		{
			START_LOG( cerr, L"이런 완료된 퀘스트를 가지고 있지 않다고?! 있다고 해서 기록한거잖아!!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( mitComplete->first )
				<< BUILD_LOG( mitComplete->second )
				<< END_LOG;
		}
	}

	// 진행중 퀘스트 변환
	std::map< int, int >::iterator mitInProgress = mapInProgress.begin();
	for(  ; mitInProgress != mapInProgress.end() ; ++mitInProgress )
	{
		mitMyInProgress = m_mapQuesting.find( mitInProgress->first );
		if( mitMyInProgress != m_mapQuesting.end() )
		{
	
			const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitInProgress->second );
			if( pQuestTemplet == NULL )
			{
				START_LOG( cerr, L"QUEST TEMPLET가 없음." )
					<< BUILD_LOG( mitInProgress->first )
					<< BUILD_LOG( mitInProgress->second )
					<< END_LOG;
				continue;
			}

			KQuestInstance kTempQuestInstance;
			kTempQuestInstance.m_iID			= mitInProgress->second;
			kTempQuestInstance.m_OwnorUnitUID	= iUnitUID;

			//sub quest seting(완료했는건지 아닌건지도 파악)
			for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
			{
				KSubQuestInstance kSub;

				const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
				if( pSubTemplet == NULL )
				{
					START_LOG( cerr, L"SUB QUEST TEMPLET가 없음." )
						<< BUILD_LOG( kTempQuestInstance.m_iID )
						<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[uiIndexSub] )
						<< BUILD_LOG( kTempQuestInstance.m_OwnorUnitUID )
						<< END_LOG;

					continue;
				}

				kSub.m_iID			= pSubTemplet->m_iID;
				kSub.m_ucClearData	= 0;
				kSub.m_bIsSuccess	= false;

				kTempQuestInstance.m_vecSubQuestInstance.push_back( kSub );
			}
			
			m_mapQuesting.insert( std::make_pair( mitInProgress->second, kTempQuestInstance ) );
			m_mapQuesting.erase( mitMyInProgress );
		}
		else
		{
			START_LOG( cerr, L"이런 완료된 퀘스트를 가지고 있지 않다고?! 있다고 해서 기록한거잖아!!" )
				<< BUILD_LOG( mitInProgress->first )
				<< BUILD_LOG( mitInProgress->second )
				<< END_LOG;
		}
	}
}

void KUserQuestManager::SetClassChangeDeleteQuest( IN UidType iUnitUID, IN std::vector< int >& vecDeleteCompleteQuest, IN std::vector< int >& vecDeleteInProgressQuest )
{
	// 진행 중인 퀘스트 삭제
	for( u_short index = 0; index < vecDeleteInProgressQuest.size() ; ++index )
	{
		std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.find( vecDeleteInProgressQuest[index] );
		if( mit != m_mapQuesting.end() )
		{
			m_mapQuesting.erase( mit );
		}
		else
		{
			START_LOG( cerr, L"이런 진행중 퀘스트를 가지고 있지 않다고?! 있다고 해서 지운다고한거잖아!!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( vecDeleteInProgressQuest[index] )
				<< END_LOG;
		}
	}

	// 완료된 퀘스트 삭제
	for( u_short index = 0; index < vecDeleteCompleteQuest.size() ; ++index )
	{
		std::map< int, KCompleteQuestInfo >::iterator mit = m_mapCompleteQuest.find( vecDeleteCompleteQuest[index] );
		if( mit != m_mapCompleteQuest.end() )
		{
			m_mapCompleteQuest.erase( mit );
		}
		else
		{
			START_LOG( cerr, L"이런 완료된 퀘스트를 가지고 있지 않다고?! 있다고 해서 지운다고한거잖아!!" )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( vecDeleteCompleteQuest[index] )
				<< END_LOG;
		}
	}
}

void KUserQuestManager::GetClassChangeDeleteQuest( IN char cDownUnitClass, OUT std::vector< int >& vecCompleteDelete, OUT std::vector< int >& vecInProgressDelete )
{
	// 완료된 퀘스트에서 삭제 할(전직 퀘스트) 찾자
	std::map< int, KCompleteQuestInfo >::iterator mitComplete = m_mapCompleteQuest.begin();
	for(  ; mitComplete != m_mapCompleteQuest.end() ; ++mitComplete )
	{
		const CXSLQuestManager::QuestTemplet* pQeustTemplet = SiCXSLQuestManager()->GetQuestTemplet( mitComplete->first );
		if( pQeustTemplet != NULL )
		{
			std::set< CXSLUnit::UNIT_CLASS >::const_iterator sit = pQeustTemplet->m_Condition.m_setUnitClass.find( static_cast<CXSLUnit::UNIT_CLASS>(cDownUnitClass) );
			if( pQeustTemplet->m_eQuestType == CXSLQuestManager::QT_CHANGE_JOB )
			{
				if( sit != pQeustTemplet->m_Condition.m_setUnitClass.end() )
				{
					vecCompleteDelete.push_back( mitComplete->first );
				}
			}
		}
	}
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
void KUserQuestManager::ClearAutoCompleteQuest()
{
	m_mapAutoQuestCompleteReq.clear();
	m_vecAutoQuestCompleteAck.clear();
}

void KUserQuestManager::SetAutoCompleteQuest( IN OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_REQ& kPacketReq )
{
	ClearAutoCompleteQuest();

	BOOST_TEST_FOREACH( KEGS_QUEST_COMPLETE_REQ&, kInfo, kPacketReq.m_vecQuestCompleteReq )
	{
		kInfo.m_bAutoComplete = true; // 자동으로 퀘스트 완료하려면 요걸 true로 넣어줘야함.
		m_mapAutoQuestCompleteReq.insert( std::make_pair( kInfo.m_iQuestID, kInfo ) );
	}
}

bool KUserQuestManager::GetAndDeleteAutoCompleteQuest( OUT KEGS_QUEST_COMPLETE_REQ& kReq )
{
	if( m_mapAutoQuestCompleteReq.empty() )
		return false;

	std::map< int, KEGS_QUEST_COMPLETE_REQ >::iterator mit;
	for( mit = m_mapAutoQuestCompleteReq.begin(); mit != m_mapAutoQuestCompleteReq.end(); ++mit )
	{
		kReq = mit->second;
        break;
	}

	if( mit != m_mapAutoQuestCompleteReq.end() )
	{
        m_mapAutoQuestCompleteReq.erase( mit );
	}

	return true;
}

void KUserQuestManager::AddAutoCompleteQuestResult( IN const KEGS_QUEST_COMPLETE_ACK& kAck )
{
    m_vecAutoQuestCompleteAck.push_back( kAck );
}

void KUserQuestManager::GetAutoCompleteQuestResult( OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK& kPacketAck )
{
	kPacketAck.m_vecQuestCompleteAck = m_vecAutoQuestCompleteAck;

	ClearAutoCompleteQuest();
}
#endif SERV_ENTER_FIELD_QUEST_CLEAR
//}}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
void KUserQuestManager::CheckEventQuest( IN KGSUserPtr spUser )
{
	// 이벤트 퀘스트가 있으면 모두 수락 가능하도록 한다.
	// 수락된 상태지만 스크립트에 정보가 없으면 포기하도록 한다.
	std::vector<int> vecDeleteQuest;
	std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.begin();
	for( ; mit != m_mapQuesting.end() ; ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( clog, L"존재하지 않는 퀘스트 ID입니다! 스크립트 상에서 삭제된 퀘스트 입니다." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			vecDeleteQuest.push_back( mit->first );
			continue;
		}
	}

	// 스크립트에 없는 Quest 를 지우자
	BOOST_TEST_FOREACH( int, iQuest, vecDeleteQuest )
	{
		std::map< int, KQuestInstance >::iterator mitDelete = m_mapQuesting.find( iQuest );
		if( mitDelete != m_mapQuesting.end() )
		{
			m_mapQuesting.erase( mitDelete );
		}
	}

	// 수락 상태가 아니지만 스크립트에 있으면 수락하도록 한다.
	std::vector<int> vecNewEventQuest;
	std::map< int, CXSLQuestManager::QuestTemplet > mapTemplet;
	SiCXSLQuestManager()->GetQuestListByType( CXSLQuestManager::QT_EVENT, mapTemplet );

	if( mapTemplet.empty() == false )
	{
		std::map< int, CXSLQuestManager::QuestTemplet >::iterator mitTemplet = mapTemplet.begin();
		for( ; mitTemplet != mapTemplet.end() ; ++mitTemplet )
		{
			std::map< int, KQuestInstance >::iterator mitQuest = m_mapQuesting.find( mitTemplet->first );
			// 수락된 상태가 아니라면
			if( mitQuest == m_mapQuesting.end() )
			{
				KEGS_NEW_QUEST_REQ kTempReq;
				kTempReq.m_iQuestID = mitTemplet->first;
				kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
				if ( SiKGameEventManager()->IsEnableEventQuest(mitTemplet->first) == false )
				{
					continue; 
				}
#endif //SERV_ALLOW_EVENT_ERASE

				if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, true ) == false )
				{
					START_LOG( cwarn, L"신규 이벤트 퀘스트 수락 실패!" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( kTempReq.m_iQuestID )
						<< END_LOG;
				}
			}
		}
	}
}

void KUserQuestManager::CheckResetDayEventQuest_AutoAccept( IN KGSUserPtr spUser )
{
	// 이벤트 타입의 퀘스트를 모두 가져온다.
	std::map< int, CXSLQuestManager::QuestTemplet > mapTemplet;
	SiCXSLQuestManager()->GetQuestListByType( CXSLQuestManager::QT_EVENT, mapTemplet );

	// 수락이 가능한 퀘스트 인지 확인하자
	std::vector<int> vecCompleteDayQuest;
	std::map< int, CXSLQuestManager::QuestTemplet >::iterator mit = mapTemplet.begin();
	for( ; mit != mapTemplet.end() ; ++mit )
	{
		// 이벤트 일일 퀘스트가 아니면 다음!
		if( mit->second.m_eRepeatType != CXSLQuestManager::QRT_DAY )
		{
			continue;
		}

#ifdef SERV_TIME_EVENT_QUEST_AUTO_ACCEPT
		if ( mit->second.m_bIsTimeEvent == true )
		{
			continue;
		}
#endif // SERV_TIME_EVENT_QUEST_AUTO_ACCEPT

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
		if ( SiKGameEventManager()->IsEnableEventQuest(mit->first) == false )
			continue; // 2013.06.10 lygan_조성욱 // 어차피 활성화 되지 않은 퀘스트는 AfterQuest 를 찾을 필요가 없다
#endif //SERV_ALLOW_EVENT_ERASE

		// 수락 할 것인가?
		bool bAccept = false;

		// 이미 완료한 퀘스트 인가?
		KCompleteQuestInfo* pCompleteQuestInfo = GetCompleteQuestInfo( mit->first );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"퀘스트 완료는 했는데 완료날짜 정보가 이상합니다." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;

				continue;
			}

			// 이미 완료한 퀘스트라면 클리어한 날짜를 확인하여 오늘 수행 가능한지 체크한다.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_DAILY_REPEAT_HOUR )
			{
				// 6시보다 넘는 시각이라면 다음날로 세팅
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// 오늘 수행 가능한가요?
			if( tCheckTime <= tCurTime )
			{
				bAccept = true;
			}
		}
		else
		{
			bAccept = true;
		}

		if( bAccept == true )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = mit->first;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ( kTempReq, spUser, true, false ) == false )
			{
				START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(After)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
			}
		}
	}
}

void KUserQuestManager::CheckLimitLevelQuest( IN KGSUserPtr spUser )
{
	// 접속시/레벨업 시 유저의 레벨을 체크하여 LimitLv 이 유저의 레벨보다 낮게 설정된 퀘스트는 자동으로 포기 시키고 관련 퀘스트 아이템은 삭제 한다.
	// 현재 수락한 퀘스트를 검사하여 LimitLv 를 체크하자
	if( m_mapQuesting.empty() == true )
		return;

	std::vector<int> vecDelQuest;

	std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.begin();
	for( ; mit != m_mapQuesting.end() ; ++mit )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( mit->first );
		if( pQuestTemplet == NULL )
		{
			START_LOG( clog, L"존재하지 않는 퀘스트 ID입니다! 스크립트 상에서 삭제된 퀘스트 입니다." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// LimitLv 이 셋팅 되어 있고
		// 유저의 레벨보다 낮으면
		if( pQuestTemplet->m_Condition.m_iLimitLevel != -1 && pQuestTemplet->m_Condition.m_iLimitLevel < spUser->GetLevel() )
		{
			vecDelQuest.push_back( mit->first );
		}
	}

	BOOST_TEST_FOREACH( int, iQuestID, vecDelQuest )
	{
		KEGS_GIVE_UP_QUEST_REQ kQuest;
		kQuest.m_iQuestID = iQuestID;
		Handler_EGS_GIVE_UP_QUEST_REQ( kQuest, spUser );
	}
}

void KUserQuestManager::Handler_OnEnterTheDungeon( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 던전 입장 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_VISIT_DUNGEON )
				continue;

			// 3. 던전 입장 상태 인지 확인!
			if( spUser->GetRoomUID() <= 0 )
				continue;

			// 4. 해당 던전이 맞는지 확인!
			if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetDungeonIDAndDungeonDifficulty(), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
				continue;

			// 5. 이미 완료한 퀘스트 인지 확인!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"퀘스트 SQT_VISIT_DUNGEON 수행" )
					<< BUILD_LOG( spUser->GetMapID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

void KUserQuestManager::Handler_OnFindNPC( IN KGSUserPtr spUser, std::vector< int >& vecNPCData, bool bDungeon )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		for( u_int uiIndexSub = 0; uiIndexSub < pQuestTemplet->m_vecSubQuest.size(); ++uiIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[uiIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST 가 비어있습니다!" )
					<< BUILD_LOG( kQuestInstance.m_iID )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 던전 입장 퀘스트
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_FIND_NPC )
				continue;

			// 3. 던전 입장 상태 인지 확인!
			if( spUser->GetRoomUID() <= 0 && spUser->GetMapID() <= 0 )
				continue;

			// 4. 해당 던전이 맞는지 확인!
			if( bDungeon == true )
			{
				// 던전이 존재한다면
				if( pSubQuestTemplet->m_ClearCondition.m_setDungeonID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetDungeonIDAndDungeonDifficulty(), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
						continue;
				}
			}
			// 4. 해당 배틀 필드가 맞는지 확인!
			else
			{
				// 배틀 필드가 존재한다면
				if( pSubQuestTemplet->m_ClearCondition.m_setBattleFieldID.empty() == false )
				{
					if( SiCXSLQuestManager()->IsExistBattleFiledIInSubQuest( pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser->GetMapID() ) == false )
						continue;
				}
			}

			// 5. 찾는 npc 가 맞다면
			{
				bool bExistNpc = false;
				BOOST_TEST_FOREACH( int, kNPCData, vecNPCData )
				{
					if( kNPCData == pSubQuestTemplet->m_ClearCondition.m_iFindNPCID )
					{
						bExistNpc = true;
						break;
					}
				}

				if( bExistNpc == false )
					continue;
			}			
						
			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;
			
			kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
			kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

			kNot.m_vecQuestInst.push_back( kQuestInstance );

			START_LOG( clog, L"퀘스트 SQT_VISIT_DUNGEON 수행" )
				<< BUILD_LOG( spUser->GetMapID() )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetUID() )
#else
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
				<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
		spUser->SendUpdateDropQuestItemByIngQuest();
#endif SERV_REFORM_QUEST
		//}}
	}
}

bool KUserQuestManager::CheckCompleteSubQuest_BeforGroup( IN int iQuestID, IN int iSubQuestID, IN KGSUserPtr spUser )
{
	// 그룹핑 된 서브퀘스트 중 앞 그룹이 모두 클리어 되었는지 확인한다.

	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"QuestTemplet 포인터 이상.!" )
			<< BUILD_LOG( iQuestID )
			<< END_LOG;

		return false;
	}

	// 현재 서브 퀘스트가 몇번째 그룹인지 알자
	int iGroupID = 0;		// 그룹번호는 1~ 존재한다.

	std::map< int,std::vector<int> >::const_iterator mitGroup = pQuestTemplet->m_mapSubQuestGroup.begin();
	for( ; mitGroup != pQuestTemplet->m_mapSubQuestGroup.end() ; ++mitGroup )
	{
		BOOST_TEST_FOREACH( const int, iSub, mitGroup->second )
		{
			if( iSub == iSubQuestID )
			{
				iGroupID = mitGroup->first;
				break;
			}
		}

		if( iGroupID != 0 )
			break;
	}
	
	// 그룹을 찾지 못했다
	if( iGroupID == 0 )
	{
		START_LOG( cwarn, L"서브 퀘스트의 그룹 정보를 찾지 못했다. " )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( iSubQuestID )
			<< END_LOG;

		return false;
	}

	// 찾았다면...앞 그룹의  subquest 들이 모두 완료 되었는지 확인하자
	std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.find( iQuestID );
	if( mit == m_mapQuesting.end() )
	{
		return false;
	}

	KQuestInstance& kQuestInstance = mit->second;

	mitGroup = pQuestTemplet->m_mapSubQuestGroup.begin();
	for( ; mitGroup != pQuestTemplet->m_mapSubQuestGroup.end() ; ++mitGroup )
	{
		if( mitGroup->first < iGroupID )
		{
			BOOST_TEST_FOREACH( const int, iSub, mitGroup->second )
			{
				BOOST_TEST_FOREACH( const KSubQuestInstance, SubQuest, kQuestInstance.m_vecSubQuestInstance )
				{
					if( SubQuest.m_iID == iSub )
					{
						if( SubQuest.m_bIsSuccess == false )
						{
							const CXSLQuestManager::SubQuestTemplet* pSubTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( SubQuest.m_iID );
							if( pSubTemplet != NULL )
							{
								// 예외가 되는 서브 퀘스트 타입이 있다.
								if( pSubTemplet->m_eClearType == CXSLQuestManager::SQT_ITEM_COLLECTION 
								 || pSubTemplet->m_eClearType == CXSLQuestManager::SQT_QUEST_ITEM_COLLECTION )
								{
									//수집해야할 아이템(개수만큼) 있는지 확인하여 서브퀘스트 완료체크를 한다.
									std::map<int,int> mapItemInfo;
									mapItemInfo.insert( std::make_pair( pSubTemplet->m_ClearCondition.m_iItemID, pSubTemplet->m_ClearCondition.m_iCollectionItemNum ) );

									// 인벤토리에 수집해야 할 아이템이 있다면 완료 된 것이나 마찮가지므로 성공했다고 생각하자
									if( spUser->IsEnoughItemExistOnlyInventory( mapItemInfo ) == true )
									{
										break;
									}
								}
								else
								{
									return false;
								}
									
							}

							return false;
						}
						break;
					}
				}
			}
		}
	}

	return true;
}

void KUserQuestManager::CheckEpicQuest_EnterDungeon( IN int iDungeonID, IN KGSUserPtr spUser )
{
	//# iMapID 에 대한 유효성 검사는 
	//# EGS_STATE_CHANGE_FIELD_REQ 에서 하고 들어왔기 때문에 하지 않는다.
	std::vector<CXSLQuestManager::QuestTemplet*>::const_iterator cvit;
	std::vector<CXSLQuestManager::QuestTemplet*> vecEpicList;

	SiCXSLQuestManager()->GetEpicQuestTempletList(vecEpicList);

	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pTemplet, vecEpicList )
	{
		if( NULL == pTemplet )
			continue;

		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pTemplet->m_eQuestType &&
			pTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pTemplet->m_vecSubQuest.empty() == true )
			continue;

		// 진행중 퀘스트 X
		if( IsQuest( pTemplet->m_iID ) == true )
			continue;

		// 완료된 퀘스트 X
		if( IsCompleteQuest( pTemplet->m_iID ) == true )
			continue;

		// 던전 아이디가 같으면.
		if( iDungeonID <= 0 )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
		if ( SiKGameEventManager()->IsEnableEventQuest(pTemplet->m_iID) == false )
			continue; // 2013.06.10 lygan_조성욱 // 어차피 활성화 되지 않은 퀘스트는 AfterQuest 를 찾을 필요가 없다
#endif //SERV_ALLOW_EVENT_ERASE

		if( pTemplet->m_Condition.m_setEnableDungeon.size() > 0 )
		{
			std::set<int>::const_iterator sit = pTemplet->m_Condition.m_setEnableDungeon.find( iDungeonID );
			if( sit == pTemplet->m_Condition.m_setEnableDungeon.end() )
				continue;
		}
		else
			continue;

		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pTemplet->m_iID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

		if( false == Handler_EGS_NEW_QUEST_REQ(kTempReq, spUser) )
		{
			START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(State Change)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pTemplet->m_iID )
				<< BUILD_LOG( iDungeonID )
				<< END_LOG;
		}		
	}
}

void KUserQuestManager::CheckEpicQuest_EnterBattleField( IN int iMapID, IN KGSUserPtr spUser )
{
	//# iMapID 에 대한 유효성 검사는 
	//# EGS_STATE_CHANGE_FIELD_REQ 에서 하고 들어왔기 때문에 하지 않는다.
	std::vector<CXSLQuestManager::QuestTemplet*>::const_iterator cvit;
	std::vector<CXSLQuestManager::QuestTemplet*> vecEpicList;

	SiCXSLQuestManager()->GetEpicQuestTempletList(vecEpicList);

	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pTemplet, vecEpicList )
	{
		if( NULL == pTemplet )
			continue;

		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pTemplet->m_eQuestType &&
			pTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;

		if( pTemplet->m_vecSubQuest.empty() == true )
			continue;

		// 진행중 퀘스트 X
		if( IsQuest( pTemplet->m_iID ) == true )
			continue;

		// 완료된 퀘스트 X
		if( IsCompleteQuest( pTemplet->m_iID ) == true )
			continue;

		// 배틀필드 아이디가 같으면.
		if( iMapID <= 0 )
			continue;

#ifdef SERV_ALLOW_EVENT_ERASE // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
		if ( SiKGameEventManager()->IsEnableEventQuest(pTemplet->m_iID) == false )
			continue; // 2013.06.10 lygan_조성욱 // 어차피 활성화 되지 않은 퀘스트는 AfterQuest 를 찾을 필요가 없다
#endif //SERV_ALLOW_EVENT_ERASE

		if( pTemplet->m_Condition.m_setEnableBattleField.size() > 0 )
		{
			std::set<int>::const_iterator sit = pTemplet->m_Condition.m_setEnableBattleField.find( iMapID );
			if( sit == pTemplet->m_Condition.m_setEnableBattleField.end() )
				continue;
		}
		else
			continue;

		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = pTemplet->m_iID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

		if( false == Handler_EGS_NEW_QUEST_REQ(kTempReq, spUser) )
		{
			START_LOG( cwarn, L"[에픽] New Quest Req Fail.!(State Change)" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( pTemplet->m_iID )
				<< BUILD_LOG( iMapID )
				<< END_LOG;
		}		
	}
}
#endif SERV_REFORM_QUEST
//}}

//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
bool KUserQuestManager::IsSuitableLevelUser( IN UidType CharUID, IN const std::map< UidType, bool >& mapSuitableLevelInfo )
{
	std::map< UidType, bool >::const_iterator mit = mapSuitableLevelInfo.find( CharUID );
	if( mit == mapSuitableLevelInfo.end() )
	{
		START_LOG( cwarn, L"적정 레벨 유저 정보가 없당?" )
			<< BUILD_LOG( CharUID )
			<< BUILD_LOG( mapSuitableLevelInfo.size() )
			<< END_LOG;
		return false;
	}

	return mit->second;
}
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
//}}

#ifdef SERV_POINT_COUNT_SYSTEM
void KUserQuestManager::SetUpdateQuestInstance( IN KGSUserPtr spUser )
{
	if ( spUser != NULL)
	{
		std::map< int, KQuestInstance >::iterator mit = m_mapQuesting.begin();
		for ( ; mit != m_mapQuesting.end() ; ++mit)
		{
			for( int iIndexInstance = 0; iIndexInstance < (int)mit->second.m_vecSubQuestInstance.size(); ++iIndexInstance )
			{
				const CXSLQuestManager::SubQuestTemplet* pSubQuestInstanceTemplet = SiCXSLQuestManager()->GetSubQuestTemplet(mit->second.m_vecSubQuestInstance[iIndexInstance].m_iID );
				if( pSubQuestInstanceTemplet == NULL )
				{
					START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
						<< BUILD_LOG( mit->second.m_vecSubQuestInstance[iIndexInstance].m_iID )
						<< BUILD_LOG( spUser->GetCharUID() )
						<< BUILD_LOG( spUser->GetCharName() )
						<< END_LOG;

					SET_ERROR( ERR_QUEST_08 );
					continue;
				}
				else
				{
					if ( pSubQuestInstanceTemplet->m_eClearType == CXSLQuestManager::SQT_POINT_COUNT )
					{

					}
				}
			}
		}

		if ( m_mapQuesting.empty() == false )
		{

		}
	}
}
#endif //SERV_POINT_COUNT_SYSTEM

#ifdef SERV_SKILL_USE_SUBQUEST
void KUserQuestManager::Handler_EGS_SKILL_USE_REQ( IN int iSkillID, IN KGSUserPtr spUser )
{
	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map<int, KQuestInstance>::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); mit++ )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );

		if( pQuestTemplet != NULL )
		{

#ifdef SERV_EPIC_QUEST
			// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
			if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
				pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
				)
				continue;
#endif SERV_EPIC_QUEST

			if( pQuestTemplet->m_vecSubQuest.empty() == false )
			{
				for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
				{
					const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );

					if( pSubQuestTemplet != NULL )
					{
						if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_SKILL_USE )
							continue;

						if( pSubQuestTemplet->m_ClearCondition.m_setSkillID.empty() == true ||
							pSubQuestTemplet->m_ClearCondition.m_setSkillID.find( iSkillID ) != pSubQuestTemplet->m_ClearCondition.m_setSkillID.end() )
						{
							if( pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount > kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
							{
								++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;

								if( pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount <= kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
									kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;

								kNot.m_vecQuestInst.push_back( kQuestInstance );

								START_LOG( clog, L"퀘스트 SQT_SKILL_USE 수행" )
									<< BUILD_LOG( iSkillID )
									<< BUILD_LOG( spUser->GetCharName() )
									<< BUILD_LOG( spUser->GetUserName() )
									<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
									<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_setSkillID.size() )
									<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iUseSkillCount )
									<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData )
									<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess );
							}
						}

					}
					else
					{
						START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
							<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
							<< BUILD_LOG( spUser->GetCharUID() )
							<< BUILD_LOG( spUser->GetCharName() )
							<< END_LOG;

						SET_ERROR( ERR_QUEST_08 );
					}
				}
			}
		}
		else
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_SKILL_USE_SUBQUEST

#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
void KUserQuestManager::Handler_OnLearnNewSkill( IN KGSUserPtr spUser, IN std::vector<int> vecNowLearnSkill )
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	//퀘스트를 돌면서
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance* pkQuestInstance = &mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( pkQuestInstance->m_iID );
		if( pQuestTemplet == NULL )		
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( pkQuestInstance->m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

#ifdef SERV_EPIC_QUEST
		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType &&
			pQuestTemplet->m_iPlayLevel > spUser->GetLevel()
			)
			continue;
#endif SERV_EPIC_QUEST

		//서브퀘스트의 조건을 검사한다.
		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SUB_QUEST_TYPE::SQT_LEARN_NEW_SKILL )	//클리언 조건이 같은지..
				continue;

			if( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess == true )
				continue;

			if( vecNowLearnSkill.size() <= 0 )
				continue;

			if( pSubQuestTemplet->m_ClearCondition.m_setSkillID.empty() == true )
				continue;

			for( int i = 0; i < vecNowLearnSkill.size(); i++)
			{
				if( pSubQuestTemplet->m_ClearCondition.m_setSkillID.find(vecNowLearnSkill[i]) != pSubQuestTemplet->m_ClearCondition.m_setSkillID.end() )
				{
					++pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData;

					if( pSubQuestTemplet->m_ClearCondition.m_iLearnNewSkillCount <= pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_ucClearData )
					{
						pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
						break;
					}
				}
			}

			kNot.m_vecQuestInst.push_back( *pkQuestInstance );

			START_LOG( clog, L"새로운 스킬 배우기 퀘스트 수행!" )
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
				<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iLearnNewSkillCount )
				<< BUILD_LOG( vecNowLearnSkill.size() )
				<< BUILD_LOG( pkQuestInstance->m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
				;
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

#ifdef SERV_EVENT_LEVEL_UP_QUEST_CLEAR
bool KUserQuestManager::Handler_OnCheckLevelUpQuest(IN const KGSUserPtr spUser)
{
	SET_ERROR( NET_OK );

	if( m_mapQuesting.empty() == true )
		return false;

	KEGS_UPDATE_QUEST_NOT kNot;

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapQuesting.begin(); mit != m_mapQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() )
			continue;

		// 에픽 퀘스트 는 레벨이 안되면 진행이 안됨.
		if( CXSLQuestManager::QT_EPIC == pQuestTemplet->m_eQuestType 
			&&	pQuestTemplet->m_iPlayLevel > spUser->GetLevel()	)
			continue;

		for( int iIndexSub = 0; iIndexSub < (int)pQuestTemplet->m_vecSubQuest.size(); ++iIndexSub )
		{
			const CXSLQuestManager::SubQuestTemplet* pSubQuestTemplet = SiCXSLQuestManager()->GetSubQuestTemplet( pQuestTemplet->m_vecSubQuest[iIndexSub] );
			if( pSubQuestTemplet == NULL )
			{
				START_LOG( cerr, L"SUB QUEST TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pQuestTemplet->m_vecSubQuest[iIndexSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_08 );
				continue;
			}

			if( pSubQuestTemplet->m_iID == 121340 || pSubQuestTemplet->m_iID == 121350 )
			{
				++kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_ucClearData;
				kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess = true;
				kNot.m_vecQuestInst.push_back( kQuestInstance );
			}

			START_LOG( clog, L"퀘스트 SQT_NPC_HUNT 수행" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[iIndexSub].m_bIsSuccess )
					<< END_LOG;
		}
	}
	
	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
	return false;
}
#endif SERV_EVENT_LEVEL_UP_QUEST_CLEAR