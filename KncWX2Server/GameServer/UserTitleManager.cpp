#include "GSUser.h"
#include "NetError.h"

#include ".\usertitlemanager.h"
#include "X2Data/XSLTitleManager.h"
#include "X2Data/XSLSocketItem.h"

#include "TrainingCenterTable.h"

KUserTitleManager::KUserTitleManager(void)
{
}

KUserTitleManager::~KUserTitleManager(void)
{
}

void KUserTitleManager::Clear()
{
	m_iEquippedTitle.SetValue( 0 );
	m_mapMission.clear();
	m_mapTitle.clear();
	m_setDBupdateMission.clear();

	//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
	m_bUsedResurrectionStone = false;
	m_iStartedNumMember		 = 0;
#endif SERV_ADD_TITLE_CONDITION
	//}}
	//{{ 2013. 02. 05  칭호 획득 조건 추가(부활석 사용 횟수, 샌더 마을) - 김민성
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	m_iUseResurrectionStoneCount	 = 0;
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}
}

void KUserTitleManager::Init( IN bool bIsPcBang, IN OUT std::vector< KMissionInstance >& vecMission, IN OUT std::vector< KTitleInfo >& vecTitle )
{
	// 진행중인 미션 세팅
	std::vector< KMissionInstance >::iterator vit;
	for( vit = vecMission.begin(); vit != vecMission.end(); ++vit )
	{
		KMissionInstance& kMissionInstance = *vit;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cwarn, L"미션 템플릿이 존재하지 않습니다." )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< END_LOG;
			continue;
		}

		//{{ 2008. 10. 22  최육사	종료된 이벤트 미션
		if( pMissionTemplet->m_bIsSecretTitle )
			continue;
		//}}

		KMissionInstance kMission;
		kMission.m_iID = kMissionInstance.m_iID;

		// 서브 미션 세팅		
		for( u_int iIdx = 0; iIdx < pMissionTemplet->m_vecSubMission.size(); ++iIdx )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[iIdx] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"서브 미션 템플릿이 존재하지 않습니다." )
					<< BUILD_LOG( kMissionInstance.m_iID )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[iIdx] )
					<< END_LOG;
				// 템플릿 읽기 실패한 미션을 추가되면 안됩니다.
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
				kMission.m_vecSubMissionInstance.clear();
				break;
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
				continue;
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
			}

            KSubMissionInstance kSubMission;
			kSubMission.m_iID		 = pSubMissionTemplet->m_iSubMissionID;
			kSubMission.m_sClearData = kMissionInstance.m_vecSubMissionInstance[iIdx].m_sClearData;

			kMissionInstance.m_vecSubMissionInstance[iIdx].m_iID = pSubMissionTemplet->m_iSubMissionID;

			switch( pSubMissionTemplet->m_eClearType )
			{
//			case CXSLTitleManager::TMCT_DUNGEON_TIME:
//			case CXSLTitleManager::TMCT_DUNGEON_RANK:
			case CXSLTitleManager::TMCT_NPC_TALK:
			case CXSLTitleManager::TMCT_QUEST:
			case CXSLTitleManager::TMCT_USE_ITEM:
			case CXSLTitleManager::TMCT_COLLECT_TITLE:
				{
					if( kSubMission.m_sClearData != 0 )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

// 			case CXSLTitleManager::TMCT_DUNGEON_DAMAGE:
// 				{
// 					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData <= pSubMissionTemplet->m_ClearCondition.m_iDungeonDamage )
// 					{
// 						kSubMission.m_bIsSuccess = true;
// 						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
// 					}
// 				}
// 				break;
			case CXSLTitleManager::TMCT_DUNGEON_TIME:
			case CXSLTitleManager::TMCT_DUNGEON_RANK:
			case CXSLTitleManager::TMCT_DUNGEON_DAMAGE:
			case CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT:
				//{{ 2010. 05. 19  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
			case CXSLTitleManager::TMCT_WITH_DIF_SERV_USER:
#endif SERV_INTEGRATION
				//}}
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_NPC_HUNT:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iKillNum )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_ITEM_COLLECTION:
			case CXSLTitleManager::TMCT_QUEST_ITEM_COLLECTION:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iCollectionItemNum )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_PVP_PLAY:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPVPPlay )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_PVP_WIN:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPVPWin )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

			case CXSLTitleManager::TMCT_PVP_KILL:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPVPKill )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;

				//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
			case CXSLTitleManager::TMCT_GIVE_PET_FEED:
				{
					if( kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPetFeedCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
			case CXSLTitleManager::TMCT_PLAYER_WITH_DUNGEON_CLEAR:
			case CXSLTitleManager::TMCT_RESURRECTION_STONE:
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
			case CXSLTitleManager::TMCT_TOGETHER_MISSION_CLEAR:
				{
// 					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
// 					{
// 						kSubMission.m_bIsSuccess = true;
// 
// 						for( int num = 0 ; num < (int)pMissionTemplet->m_vecSubMission.size(); ++num )		// 모든 미션 성공
// 						{
// 							kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
// 						}
// 					}
				}
				break;
			case CXSLTitleManager::TMCT_USER_UNIT_DIE:
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
#endif SERV_ADD_TITLE_CONDITION
				//}}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
			case CXSLTitleManager::TMCT_ITEM_SOCKET:
			case CXSLTitleManager::TMCT_ITEM_ENCHANT_LEVEL:
			case CXSLTitleManager::TMCT_ITEM_ENCHANT_COUNT:
			case CXSLTitleManager::TMCT_ITEM_ATTRIB:
			case CXSLTitleManager::TMCT_ITEM_RESOLVE:
				{
					if( kSubMission.m_sClearData != 0 && kSubMission.m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					{
						kSubMission.m_bIsSuccess = true;
						kMissionInstance.m_vecSubMissionInstance[iIdx].m_bIsSuccess = true;
					}
				}
				break;
#endif // SERV_ADD_TITLE_CONDITION_2013_08

			default:
				{
					START_LOG( cerr, L"없는 서브 미션 타입." )
						<< BUILD_LOG( pSubMissionTemplet->m_eClearType )
						<< BUILD_LOG( kSubMission.m_iID )
						<< END_LOG;
				}
			}

			kMission.m_vecSubMissionInstance.push_back( kSubMission );
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMission.m_vecSubMissionInstance.size() > 0 )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
		m_mapMission[kMission.m_iID] = kMission;
	}
	
	// 정리된 진행퀘스트 데이터를 세로 셋팅
	//kInfo.m_vecQuest.clear();
	//std::map<int,KQuestInstance>::iterator mit;
	//for( mit = m_mapQuest.begin(); mit != m_mapQuest.end(); mit++ )
	//{
	//	kInfo.m_vecQuest.push_back( mit->second );
	//}

	// 보유 타이틀(완료 미션) 정리해서 넣기
	CTime kInfinityTime( 2040, 1, 1, 0, 0, 0 );
	CTime kCurTime = CTime::GetCurrentTime();

	std::vector< KTitleInfo > vecCheckingTitle = vecTitle;
	vecTitle.clear();

	std::vector< KTitleInfo >::const_iterator vitT;
	for( vitT = vecCheckingTitle.begin(); vitT != vecCheckingTitle.end(); ++vitT )
	{
		KTitleInfo kTitle = *vitT;		

		CTime tEndDate;
		if( !KncUtil::ConvertStringToCTime( kTitle.m_wstrEndDate, tEndDate ) )
		{
			START_LOG( cerr, L"문자열 시간 변환 실패." )
				<< BUILD_LOG( kTitle.m_wstrEndDate )
				<< END_LOG;
			continue;
		}

		// 기간 무제한인지..
		kTitle.m_bInfinity = ( kInfinityTime < tEndDate );
		kTitle.m_kCTime = tEndDate.GetTime();
		m_mapTitle.insert( std::make_pair( kTitle.m_iTitleID, kTitle ) );

		vecTitle.push_back( kTitle );
	}

	//{{ 2009. 10. 26  최육사	PC방이벤트
	if( bIsPcBang == true )
	{
		std::map< int, KTitleInfo >::const_iterator mitPB = m_mapTitle.find( PC_BANG_TITLE_ID );
		if( mitPB == m_mapTitle.end() )
		{
			KTitleInfo kTitle;
			kTitle.m_iTitleID = PC_BANG_TITLE_ID;
			kTitle.m_bInfinity = true;
			kTitle.m_kCTime = kInfinityTime.GetTime();
			m_mapTitle.insert( std::make_pair( kTitle.m_iTitleID, kTitle ) );

			vecTitle.push_back( kTitle );
		}
		else
		{
			START_LOG( cerr, L"이미 pc방 칭호가 있다? 발생하면 안되는 오류!" )
				<< END_LOG;
		}
	}
	//}}
}

/************************************************************************/
/* void CheckNewMission() : 현재 진행 가능한 미션을 찾아서 진행중인 미션으로 등록해주는 함수
/* 1. 스크립트상의 모든 미션을 순차검색하여 조건비교를 하므로 최적화가 필요함.
/************************************************************************/

void KUserTitleManager::CheckNewMission( IN KGSUserPtr spUser, IN bool bFirst /*= false*/ )
{
	std::map< int, KMissionInstance >::const_iterator mitM;
	std::map< int, KTitleInfo >::const_iterator mitT;

	KEGS_NEW_MISSION_NOT kPacketNot;
	

	std::vector< CXSLTitleManager::MissionTemplet >::const_iterator vit;
	const std::vector< CXSLTitleManager::MissionTemplet >& vecMissionList = SiCXSLTitleManager()->GetMissionList();

	for( vit = vecMissionList.begin(); vit != vecMissionList.end(); ++vit )
	{
		const CXSLTitleManager::MissionTemplet& kMissionTemplet = *vit;

		// 1. 완료된 미션은 제외
		mitT = m_mapTitle.find( kMissionTemplet.m_iMissionID );
		if( mitT != m_mapTitle.end() )
			continue;

		// 2. 진행중인 미션은 제외
		mitM = m_mapMission.find( kMissionTemplet.m_iMissionID );
		if( mitM != m_mapMission.end() )
			continue;

		// 3. 비공개 타이틀은 제외
		if( kMissionTemplet.m_bIsSecretTitle )
			continue;

		// 3. 레벨 조건
		if( kMissionTemplet.m_kCondition.m_iLevel > spUser->GetLevel() )
			continue;

		// 4. 유닛 타입
		if( kMissionTemplet.m_kCondition.m_eUnitType != CXSLUnit::UT_NONE )
		{
			if( kMissionTemplet.m_kCondition.m_eUnitType != spUser->GetUnitType() )
				continue;
		}

		// 5. 유닛 클래스
		if( kMissionTemplet.m_kCondition.m_eUnitClass != CXSLUnit::UC_NONE )
		{
			if( kMissionTemplet.m_kCondition.m_eUnitClass != spUser->GetUnitClass() )
				continue;
		}

		// 6. 선행미션 체크
		if( kMissionTemplet.m_kCondition.m_iClearMissionID != 0 )
		{
			mitT = m_mapTitle.find( kMissionTemplet.m_kCondition.m_iClearMissionID );
			if( mitT == m_mapTitle.end() )
				continue;
		}

		// 모든 조건 만족시 new미션으로 등록 - 클라이언트에게도 알려줌
		KMissionInstance kMission;
		kMission.m_iID = kMissionTemplet.m_iMissionID;

		// 서브 미션 세팅
		for( u_int iIdx = 0; iIdx < kMissionTemplet.m_vecSubMission.size(); ++iIdx )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( kMissionTemplet.m_vecSubMission[iIdx] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"서브 미션 템플릿이 존재하지 않습니다." )
					<< BUILD_LOG( kMission.m_iID )
					<< BUILD_LOG( kMissionTemplet.m_vecSubMission[iIdx] )
					<< END_LOG;
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
				kMission.m_vecSubMissionInstance.clear();
				break;
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
				continue;
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
			}

			KSubMissionInstance kSubMission;
			kSubMission.m_iID = pSubMissionTemplet->m_iSubMissionID;
			kMission.m_vecSubMissionInstance.push_back( kSubMission );
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMission.m_vecSubMissionInstance.size() > 0 )
		{
			m_mapMission[kMission.m_iID] = kMission;
			// 클라이언트 보내줄것
			kPacketNot.m_vecNewMission.push_back( kMission );
		}
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
		m_mapMission[kMission.m_iID] = kMission;

		// 클라이언트 보내줄것
		kPacketNot.m_vecNewMission.push_back( kMission );
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
	}	

	if( kPacketNot.m_vecNewMission.empty() )
		return;

	kPacketNot.m_bFirst = bFirst;
	spUser->SendPacket( EGS_NEW_MISSION_NOT, kPacketNot );
}

KMissionInstance* KUserTitleManager::GetMissionInstance( IN int iMissionID )
{
	std::map< int, KMissionInstance >::iterator mit;
	mit = m_mapMission.find( iMissionID );
	if( mit == m_mapMission.end() )
		return NULL;
	
	return &(mit->second);
}

void KUserTitleManager::OnNpcUnitDie( IN const int iDungeonID, IN const char cDifficulty, IN const char cDungeonMode, IN const int iMonsterID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
		
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET 과 INSTANCE 의 SUB MISSION 개수가 틀림! 스크립트 확인 필요!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_NPC_HUNT )
				continue;

			//{{ 2012. 1. 11	Merge 박세훈	2012.12.26 임규수 타이틀 미션 클리어 NPC 복수 적용
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
			if( pSubMissionTemplet->m_ClearCondition.m_setKillNPCID.find( static_cast<CXSLUnitManager::NPC_UNIT_ID>(iMonsterID) ) == pSubMissionTemplet->m_ClearCondition.m_setKillNPCID.end() )
				continue;
#else
			if( pSubMissionTemplet->m_ClearCondition.m_eKillNPCID != iMonsterID )
				continue;
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
			//}}

			//던전 조건이 있는지
			if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID > 0 )
			{
				if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
					continue;

				//그리고 난이도 조건이 있는지 확인한다.
				if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty >= 0 )
				{
					// 특정 난이도 이상을 조건에 포함
					if( pSubMissionTemplet->m_ClearCondition.m_bUpperDifficulty == false )
					{
						if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
							continue;
					}
					else
					{
						if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
							continue;
					}
				}

				if( ( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID ) && ( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode ) )
					continue;
			}

#ifdef DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM
			/// 던전 조건을 지정하지 않았는데, 
			/// 현재 플레이중인 던전이 자유훈련소라면 카운트 하지 않는다.
			else if ( KTrainingCenterTable::GetInstance()->IsTrainingCenter( iDungeonID ) )
				continue;
#endif // DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM
			
			if( pSubMissionTemplet->m_ClearCondition.m_iKillNum > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iKillNum <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_NPC_HUNT 수행" )
					<< BUILD_LOG( iMonsterID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iKillNum )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}
	
	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnTalkWithNpc( IN int iNPCID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET 과 INSTANCE 의 SUB MISSION 개수가 틀림! 스크립트 확인 필요!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
		
		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_NPC_TALK )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_eTalkNPCID != iNPCID )
				continue;

			if( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData == 0 )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_NPC_TALK 수행" )
					<< BUILD_LOG( iNPCID )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_eTalkNPCID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

//{{ 2010. 8. 23	최육사	헬모드 칭호
#ifdef SERV_HELL_MODE_TITLE
void KUserTitleManager::OnDungeonClear( IN KGSUserPtr spUser, IN int iDungeonID, IN char cDifficulty, IN char cDungeonMode, IN char cRank, IN int iPlayTime, IN int iDamage, IN const std::set< UidType >& setPartyMembers )
#else
void KUserTitleManager::OnDungeonClear( IN KGSUserPtr spUser, IN int iDungeonID, IN char cDifficulty, IN char cRank, IN int iPlayTime, IN int iDamage, IN const std::set< UidType >& setPartyMembers )
#endif SERV_HELL_MODE_TITLE
//}}
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET 과 INSTANCE 의 SUB MISSION 개수가 틀림! 스크립트 확인 필요!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			bool bRVal = false;

			//{{ 2010. 8. 23	최육사	헬모드 칭호
//#ifdef SERV_HELL_MODE_TITLE
			//던전 조건이 있는지
			if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID > 0 )
			{
				// 비밀던전이라면..
				if( CXSLDungeon::IsSecretDungeon( pSubMissionTemplet->m_ClearCondition.m_eDungeonID ) == true )
				{
					// 해당 비밀던전이 맞는지 검사
					if( ( pSubMissionTemplet->m_ClearCondition.m_eDungeonID / 10 ) != ( iDungeonID / 10 ) )
						continue;

					// 난이도 검사
					if( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != CXSLDungeon::DM_INVALID )
					{
						if( pSubMissionTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_eDungeonMode != cDungeonMode )
								continue;
						}
					}
				}
				// 일반던전이라면..
				else
				{
					if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
						continue;

					//그리고 난이도 조건이 있는지 확인한다.
					if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty >= 0 )
					{
						// 특정 난이도 이상을 조건에 포함
						if( pSubMissionTemplet->m_ClearCondition.m_bUpperDifficulty == false )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
								continue;
						}
						else
						{
							if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty > cDifficulty )
								continue;
						}
					}
				}
			}
//#else
//			//던전 조건이 있는지
//			if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID > 0 )
//			{
//				if( pSubMissionTemplet->m_ClearCondition.m_eDungeonID != iDungeonID )
//					continue;
//
//				//그리고 난이도 조건이 있는지 확인한다.
//				if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty >= 0 )
//				{
//					if( pSubMissionTemplet->m_ClearCondition.m_cDifficulty != cDifficulty )
//						continue;
//				}
//			}
//#endif SERV_HELL_MODE_TITLE
			//}}

			switch( pSubMissionTemplet->m_eClearType )
			{
			case CXSLTitleManager::TMCT_DUNGEON_TIME:
				{
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearTime >= iPlayTime )
						bRVal = true;
				}
				break;

			case CXSLTitleManager::TMCT_DUNGEON_RANK:
				{
					if( (char)pSubMissionTemplet->m_ClearCondition.m_eDungeonRank <= cRank )
						bRVal = true;
				}
				break;

			case CXSLTitleManager::TMCT_DUNGEON_DAMAGE:
				{
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonDamage >= iDamage )
						bRVal = true;
				}
				break;

#ifndef DEPRECATED_SERVER_GROUP_MASK
				//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
			case CXSLTitleManager::TMCT_WITH_DIF_SERV_USER:
				{
					std::set< UidType >::const_iterator sit;
					for( sit = setPartyMembers.begin(); sit != setPartyMembers.end(); ++sit )
					{
						if( *sit == spUser->GetCharUID() )
							continue;

						// 다른 서버군 유저와 플레이한것이라면..?
						if( spUser->IsSameServerGroupUID( *sit ) == false )
						{
							bRVal = true;
						}
					}
				}
				break;
#endif SERV_INTEGRATION
				//}}
#endif DEPRECATED_SERVER_GROUP_MASK

			case CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT:
				{
					//080811.hoons. 이벤트 퀘스트 일때와 아닐때 클리어 타입이 틀려진다.
					//if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT )
					//{
					//	if( CXSLDungeon::IsRUBENDungeon( iDungeonID ) == false && CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
					//		bRVal = true;
					//}
					//else
					{
						//던전 클리어 서브는 승리만 하면 되기때문에 이곳에서 처리
						//## 초심자의 숲만 예외처리한다.
						if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL  &&
							CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
						{
							bRVal = true;
						}
					}
				}
				break;
				//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
			case CXSLTitleManager::TMCT_PLAYER_WITH_DUNGEON_CLEAR:
			case CXSLTitleManager::TMCT_RESURRECTION_STONE:
				{
					if( iDungeonID != SEnum::DI_EL_FOREST_GATE_NORMAL  
					&&	CXSLDungeon::IsTutorialDungeon( iDungeonID ) == false )
					{
						bRVal = true;
					}
				}
				break;
			case CXSLTitleManager::TMCT_TOGETHER_MISSION_CLEAR:
				{
					bRVal = true;
				}
				break;
#endif SERV_ADD_TITLE_CONDITION
				//}}
			}

			// 서브 미션 클리어
			if( bRVal )
			{
				//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
//#ifdef SERV_INTEGRATION
				if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT  ||
					//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
//#ifdef SERV_ADD_TITLE_CONDITION
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_RANK  ||
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_TIME  ||
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_DAMAGE  ||
//#endif SERV_ADD_TITLE_CONDITION
					//}}
					pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_WITH_DIF_SERV_USER )
//#else
//				if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_DUNGEON_CLEAR_COUNT )
//#endif SERV_INTEGRATION
				//}}				
				{
					//{{ 2010. 05. 03  최육사	던전 클리어 카운트 버그 수정
					// 클리어 조건 카운트를 넘지 않게 보정한다.
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}
					//}}

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

					kNot.m_vecMissionInst.push_back( kMissionInstance );
				}
				//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
				else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_PLAYER_WITH_DUNGEON_CLEAR )
				{
					if( pSubMissionTemplet->m_ClearCondition.m_iPlayerCount >= GetStartedNumMember() )
					{
						if(	pSubMissionTemplet->m_ClearCondition.m_iDungeonClearTime != 0 )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearTime < iPlayTime )
								continue;
						}
						if(	pSubMissionTemplet->m_ClearCondition.m_eDungeonRank != 0 )
						{
							if( pSubMissionTemplet->m_ClearCondition.m_eDungeonRank > cRank )
								continue;
						}

						// 세부 조건 만족시 클리어 카운터 증가
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
					}

					kNot.m_vecMissionInst.push_back( kMissionInstance );
				}
				else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_RESURRECTION_STONE )
				{
					//{{ 2013. 02. 05  칭호 획득 조건 추가(부활석 사용 횟수, 샌더 마을) - 김민성
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
					if( pSubMissionTemplet->m_ClearCondition.m_bCheckResurrectionStone == false && spUser->IsUseResurrectionStone() == false )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}
					else if( pSubMissionTemplet->m_ClearCondition.m_bCheckResurrectionStone == true && spUser->IsUseResurrectionStone() == true )
					{
						if( pSubMissionTemplet->m_ClearCondition.m_iUseResurrectionStoneCount <= spUser->GetUseResurrectionStoneCount() )
						{
							kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
						}
					}
#else
					if( pSubMissionTemplet->m_ClearCondition.m_bCheckResurrectionStone == true && spUser->IsUseResurrectionStone() == false )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
					}
#endif SERV_ADD_TITLE_CONDITION_SANDER
					//}
					
					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
					}

					kNot.m_vecMissionInst.push_back( kMissionInstance );
				}
				else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_TOGETHER_MISSION_CLEAR )
				{
					bool bResult = true;
					
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
					for( u_int nMission = 0; nMission < kMissionInstance.m_vecSubMissionInstance.size(); ++nMission )
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
					for( u_int nMission = 0; nMission < pMissionTemplet->m_vecSubMission.size(); ++nMission )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
					{
						// 앞의 서브 미션 모두 클리어 되어야 한다. 
						if( kMissionInstance.m_vecSubMissionInstance[nMission].m_bIsSuccess == false )
						{
							if( nMission != nSub )	// 미션 타입이 동시 만족이 아니어야 한다.// 현재 서브미션이 동시만족 미션이다.
							{
								bResult = false;
								break;
							}
						}
					}

					if( bResult == true )		// 모두 클리어되었다면
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1; // 클리어 횟수 증가
						
						if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData) )
						{
							kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
						}
						else
						{
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
							for( u_int num = 0; num < kMissionInstance.m_vecSubMissionInstance.size(); ++num )
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
							for( u_int num = 0; num < pMissionTemplet->m_vecSubMission.size(); ++num )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
							{
								if( num != nSub )
								{
									kMissionInstance.m_vecSubMissionInstance[num].m_sClearData = 0;
									kMissionInstance.m_vecSubMissionInstance[num].m_bIsSuccess = false;	// 앞의 서브 미션 실패로 되돌린다
									// DB 업데이트에 추가된 ID 삭제
									DeleteUpdateMission( kNot.m_vecMissionInst, kMissionInstance.m_iID );
								}
							}
						}
					}
					else					// 모두 클리어 되지 않았다면 
					{
#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
						for( u_int num = 0; num < kMissionInstance.m_vecSubMissionInstance.size(); ++num )
#else //SERV_SUB_TITLE_MISSION_BUG_FIX
						for( u_int num = 0; num < pMissionTemplet->m_vecSubMission.size(); ++num )
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX
						{
							if( num != nSub )
							{
								kMissionInstance.m_vecSubMissionInstance[num].m_sClearData = 0;
								kMissionInstance.m_vecSubMissionInstance[num].m_bIsSuccess = false;	// 모든 미션 실패로 되돌린다
								// DB 업데이트에 추가된 ID 삭제
								DeleteUpdateMission( kNot.m_vecMissionInst, kMissionInstance.m_iID );
							}
						}

						// DB 업데이트 누락 되지 않도록 한번 add 해준다.
						AddUpdateMission( kMissionInstance.m_iID );
						continue;				// 성공으로 처리 되지 않게 건너뛰게 한다.
					}

					kNot.m_vecMissionInst.push_back( kMissionInstance );

				}
#endif SERV_ADD_TITLE_CONDITION
				//}}
				else
				{
					if( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData == 0 )
					{
						kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

						kNot.m_vecMissionInst.push_back( kMissionInstance );
					}
				}

				START_LOG( clog, L"미션 TMCT_DUNGEON_TIME or TMCT_DUNGEON_RANK 수행" )
					<< BUILD_LOGc( cRank )
					<< BUILD_LOG( iPlayTime )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnQuestComplete( IN int iQuestID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET 과 INSTANCE 의 SUB MISSION 개수가 틀림! 스크립트 확인 필요!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_QUEST )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iQuestID == iQuestID )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_QUEST 수행" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iQuestID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnUseItem( IN const int iItemID, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET 과 INSTANCE 의 SUB MISSION 개수가 틀림! 스크립트 확인 필요!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_USE_ITEM )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iItemID == iItemID )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData = 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_USE_ITEM 수행" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iItemID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::CheckCompleteMission( IN int iMissionID, IN KGSUserPtr spUser )
{
	KMissionInstance* pMissionInstance = GetMissionInstance( iMissionID );
	if( pMissionInstance == NULL )
	{
		START_LOG( cerr, L"진행중인 미션이 아닙니다!" )
			<< BUILD_LOG( iMissionID )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetCharName() )
			<< END_LOG;
		return;
	}

	const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( iMissionID );
	if( pMissionTemplet == NULL )
	{
		START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
			<< BUILD_LOG( iMissionID )
			<< BUILD_LOG( spUser->GetCharUID() )
			<< BUILD_LOG( spUser->GetCharName() )
			<< END_LOG;
		return;
	}

	for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
	{
		const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
		if( pSubMissionTemplet == NULL )
		{
			START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;
			return;
		}

		if( !pMissionInstance->m_vecSubMissionInstance[nSub].m_bIsSuccess )
			return;
	}

	if( !SiCXSLTitleManager()->IsExistTitleID( pMissionInstance->m_iID ) )
	{
		START_LOG( cerr, L"미션ID와 타이틀ID가 일치하지 않는게 있다?" )
			<< BUILD_LOG( pMissionInstance->m_iID )
			<< END_LOG;
		return;
	}
	
	// 미션 수행 완료! 칭호 보상을 주자!
	KDBE_INSERT_TITLE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = spUser->GetCharUID();
	kPacketToDB.m_iTitleID = pMissionInstance->m_iID;
	kPacketToDB.m_sPeriod  = pMissionTemplet->m_sPeriod;
	spUser->SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );

	// 중복 보상을 받지 않도록 미리 진행중인 미션 삭제 - TitleID와 MissionID는 동일함
	DeleteMissionInstance( kPacketToDB.m_iTitleID );
}

bool KUserTitleManager::IsExistTitle( IN int iTitleID )
{
	std::map< int, KTitleInfo >::const_iterator mit;
	mit = m_mapTitle.find( iTitleID );
	if( mit == m_mapTitle.end() )
		return false;

	return true;
}

void KUserTitleManager::DeleteMissionInstance( IN int iMissionID )
{
	// 진행중인 미션과 DB업데이트할 미션을 삭제
	m_mapMission.erase( iMissionID );
	m_setDBupdateMission.erase( iMissionID );
}

bool KUserTitleManager::AddTitle( IN int iTitleID, IN std::wstring&	wstrEndDate, OUT __int64& tCTime, OUT bool& bInfinity )
{
	CTime kInfinityTime( 2040, 1, 1, 0, 0, 0 );

	KTitleInfo kTitle;
	kTitle.m_iTitleID = iTitleID;
	kTitle.m_wstrEndDate = wstrEndDate;	

	CTime tEndDate;
	if( !KncUtil::ConvertStringToCTime( kTitle.m_wstrEndDate, tEndDate ) )
	{
		START_LOG( cerr, L"문자열 시간 변환 실패." )
			<< BUILD_LOG( kTitle.m_iTitleID )
			<< BUILD_LOG( kTitle.m_wstrEndDate )
			<< END_LOG;
		return false;
	}

	// 기간 무제한인지..
	kTitle.m_bInfinity = ( kInfinityTime < tEndDate );
	bInfinity = kTitle.m_bInfinity;

	kTitle.m_kCTime = tEndDate.GetTime();
	tCTime = kTitle.m_kCTime;

	// 이미 가지고 있는 칭호인지 검사!
	std::map< int, KTitleInfo >::iterator mit = m_mapTitle.find( kTitle.m_iTitleID );
	if( mit == m_mapTitle.end() )
	{
		m_mapTitle.insert( std::make_pair( kTitle.m_iTitleID, kTitle ) );
	}
	else
	{
		START_LOG( clog, L"기존 타이틀 정보에 덮어씌워서 기간연장하자!" )
			<< BUILD_LOG( kTitle.m_iTitleID )
			<< BUILD_LOG( mit->second.m_iTitleID )
			<< BUILD_LOG( kTitle.m_wstrEndDate )
			<< BUILD_LOG( mit->second.m_wstrEndDate );

        mit->second = kTitle; // 덮어씌우자!
	}
	
	return true;
}

bool KUserTitleManager::EquipTitle( IN int iTitleID )
{
	SET_ERROR( ERR_UNKNOWN );

	// 1. 이미 장착하고 있는지 검사 - TitleID가 0으로 오는것은 장착해제를 의미
	if( iTitleID != 0 )
	{
		if( !SiCXSLTitleManager()->IsExistTitleID( iTitleID ) )
		{
			START_LOG( cerr, L"존재하지 않는 칭호를 장착하려고 하네..?" )
				<< BUILD_LOG( iTitleID )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			return false;		
		}

		// 2. 이미 장착하고 있는지 검사
		if( m_iEquippedTitle == iTitleID )
		{
			SET_ERROR( ERR_TITLE_01 );
			return false;		
		}

		// 3. 보유중인 타이틀인지 검사
		std::map< int, KTitleInfo >::const_iterator mit;
		mit = m_mapTitle.find( iTitleID );
		if( mit == m_mapTitle.end() )
		{
			START_LOG( cerr, L"가지고 있지도 않은 칭호를 장착하려고 하네..? 해킹인가?" )
				<< BUILD_LOG( iTitleID )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_00 );
			return false;
		}

		// 4. 기간 타이틀이면 기간 검사
		CTime tCurTime = CTime::GetCurrentTime();

		if( tCurTime.GetTime() > mit->second.m_kCTime )
		{
			SET_ERROR( ERR_TITLE_02 );
			return false;
		}
	}

	// 5. 장착 조건이 만족되면 장착
	m_iEquippedTitle += ( iTitleID - m_iEquippedTitle );

	SET_ERROR( NET_OK );
	return true;
}

//{{ 2010. 10. 18	최육사	펫 오라 스킬
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PET_AURA_SKILL
//////////////////////////////////////////////////////////////////////////

//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
void KUserTitleManager::GetEquippedStat( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat, IN unsigned int& uiHP_OnePoint, IN unsigned int& uiUnitLevel )
#else
void KUserTitleManager::GetEquippedStat( IN bool bIsDungeon, IN const KStat& kBaseStat, IN OUT float& fTotalIncHPRate, OUT KStat& kAddStat )
#endif SERV_USE_PERCENT_IN_OPTION_DATA
//}} 
{
	kAddStat.Init();

	if( GetEquippedTitleID() == 0 )
		return;

	const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( GetEquippedTitleID() );
	if( pTitleInfo == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 타이틀을 장착중?" )
			<< BUILD_LOG( GetEquippedTitleID() )
			<< END_LOG;
		return;
	}

	// 1. 기본 스탯 적용
	kAddStat.m_iBaseHP		+= pTitleInfo->m_iBaseHP;
	kAddStat.m_iAtkPhysic	+= pTitleInfo->m_iAtkPhysic;
	kAddStat.m_iAtkMagic	+= pTitleInfo->m_iAtkMagic;
	kAddStat.m_iDefPhysic	+= pTitleInfo->m_iDefPhysic;
	kAddStat.m_iDefMagic	+= pTitleInfo->m_iDefMagic;

	// 2. 소켓 옵션 적용
	KStat kSocketOptionStat;
	//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	SiCXSLSocketItem()->GetSocketOptionStat( pTitleInfo->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat, uiHP_OnePoint, uiUnitLevel );
#else
	SiCXSLSocketItem()->GetSocketOptionStat( pTitleInfo->m_vecSocketOption, bIsDungeon, kBaseStat, fTotalIncHPRate, kSocketOptionStat );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 

	// 스탯 더하기
	kAddStat.AddStat( kSocketOptionStat );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

void KUserTitleManager::GetEquippedStat( IN UINT uiBaseHP, OUT KStat& kStat, IN OUT float& fTotalIncHPRate, IN bool bIsDungeon )
{
	kStat.Init();

	if( GetEquippedTitleID() == 0 )
		return;

	const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( GetEquippedTitleID() );
	if( pTitleInfo == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 타이틀을 장착중?" )
			<< BUILD_LOG( GetEquippedTitleID() )
			<< END_LOG;
		return;
	}

	// 1. 기본 스탯 적용
	kStat.m_iBaseHP		+= pTitleInfo->m_iBaseHP;
	kStat.m_iAtkPhysic	+= pTitleInfo->m_iAtkPhysic;
	kStat.m_iAtkMagic	+= pTitleInfo->m_iAtkMagic;
	kStat.m_iDefPhysic	+= pTitleInfo->m_iDefPhysic;
	kStat.m_iDefMagic	+= pTitleInfo->m_iDefMagic;

	// 2. 소켓 옵션 적용
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit;
#else
	std::vector< short >::const_iterator vit;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	for( vit = pTitleInfo->m_vecSocketOption.begin(); vit != pTitleInfo->m_vecSocketOption.end(); ++vit )
	{
		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( *vit );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"존재하지않는 소켓옵션이 타이틀의 소켓옵션으로 세팅되어 있습니다." )
				<< BUILD_LOG( m_iEquippedTitle )
				<< BUILD_LOG( *vit )
				<< END_LOG;
			continue;
		}

		//{{ 2009. 4. 1  최육사		던전온리
		if( pSocketData->m_bDungeonOnly  &&  !bIsDungeon )
			continue;
		//}}


		if( true == pSocketData->m_bPVPOnly && true == bIsDungeon )	
			continue;


		//{{ 2008. 11. 20  최육사	HP증가율 50% 제한
		float fIncreaseHPRate = pSocketData->m_IncreaseHPRate;

		if( ( fTotalIncHPRate + fIncreaseHPRate ) > 0.5f )
		{
			fIncreaseHPRate = 0.5f - fTotalIncHPRate;
		}

		fTotalIncHPRate += fIncreaseHPRate;
		//}}

		kStat.m_iBaseHP		+= (int)( uiBaseHP * fIncreaseHPRate );
		kStat.m_iBaseHP		+= (int)( pSocketData->m_Stat.m_fBaseHP );
		kStat.m_iAtkPhysic	+= (int)( pSocketData->m_Stat.m_fAtkPhysic );
		kStat.m_iAtkMagic	+= (int)( pSocketData->m_Stat.m_fAtkMagic );
		kStat.m_iDefPhysic	+= (int)( pSocketData->m_Stat.m_fDefPhysic );
		kStat.m_iDefMagic	+= (int)( pSocketData->m_Stat.m_fDefMagic );
	}
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PET_AURA_SKILL
//////////////////////////////////////////////////////////////////////////
//}}

void KUserTitleManager::GetEquippedTitleBonusRate( IN const bool bIsDungeon, OUT float& fAddTitleExpRate, OUT float& fAddTitleEDRate )
{
	if( GetEquippedTitleID() == 0 )
		return;

	const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( GetEquippedTitleID() );
	if( pTitleInfo == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 타이틀을 장착중?" )
			<< BUILD_LOG( GetEquippedTitleID() )
			<< END_LOG;
		return;
	}

	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	BOOST_TEST_FOREACH( const int, iSocketOption, pTitleInfo->m_vecSocketOption )
	{
		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( iSocketOption );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"존재하지않는 소켓옵션이 타이틀의 소켓옵션으로 세팅되어 있습니다." )
				<< BUILD_LOG( GetEquippedTitleID() )
				<< BUILD_LOG( iSocketOption )
				<< END_LOG;
			continue;
		}
#else
	BOOST_TEST_FOREACH( const short, sSocketOption, pTitleInfo->m_vecSocketOption )
	{
		const CXSLSocketItem::SocketData* pSocketData = SiCXSLSocketItem()->GetSocketData( sSocketOption );
		if( pSocketData == NULL )
		{
			START_LOG( cerr, L"존재하지않는 소켓옵션이 타이틀의 소켓옵션으로 세팅되어 있습니다." )
				<< BUILD_LOG( GetEquippedTitleID() )
				<< BUILD_LOG( sSocketOption )
				<< END_LOG;
			continue;
		}

#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		//{{ 2009. 4. 1  최육사		던전온리
		if( pSocketData->m_bDungeonOnly == true  &&  bIsDungeon == false )
			continue;
		//}}

		if( pSocketData->m_bPVPOnly == true  &&  bIsDungeon == true )
			continue;

		if( pSocketData->m_fIncreaseExpPer > 0.0f )
			fAddTitleExpRate += pSocketData->m_fIncreaseExpPer;

		if( pSocketData->m_fIncreaseEDPer > 0.0f )
			fAddTitleEDRate += pSocketData->m_fIncreaseEDPer;
	}
}

void KUserTitleManager::ExpireTitle( IN KGSUserPtr spUser )
{
	CTime tCurTime = CTime::GetCurrentTime();

	if( m_iEquippedTitle > 0 )
	{
		// 장착중인 타이틀에 대한 기간검사
		std::map< int, KTitleInfo >::const_iterator mit;
		mit = m_mapTitle.find( m_iEquippedTitle );
		if( mit == m_mapTitle.end() )
		{
			START_LOG( cerr, L"보유하지 않은 타이틀을 장착중입니다?" )
				<< BUILD_LOG( m_iEquippedTitle )
				<< END_LOG;

			KEGS_TITLE_EXPIRATION_NOT kNot;
			kNot.m_iTitleID = GetEquippedTitleID();
			spUser->SendPacket( EGS_TITLE_EXPIRATION_NOT, kNot );

			// 장착해제
			EquipTitle( 0 );
		}
		else
		{
			const KTitleInfo& kTitle = mit->second;
			
			if( tCurTime.GetTime() > kTitle.m_kCTime )
			{
				KEGS_TITLE_EXPIRATION_NOT kNot;
				kNot.m_iTitleID = GetEquippedTitleID();
				spUser->SendPacket( EGS_TITLE_EXPIRATION_NOT, kNot );

				// 장착해제
				EquipTitle( 0 );
			}
		}
	}

	// 보유중인 타이틀에 대한 기간검사
	std::vector< int > vecDelTitle;

	std::map< int, KTitleInfo >::const_iterator mit;
	for( mit = m_mapTitle.begin(); mit != m_mapTitle.end(); ++mit )
	{
		if( tCurTime.GetTime() > mit->second.m_kCTime )
		{
			vecDelTitle.push_back( mit->first );
		}
	}

	if( vecDelTitle.empty() )
		return;

	// 보유 타이틀에서 삭제
	std::vector< int >::const_iterator vit;
	for( vit = vecDelTitle.begin(); vit != vecDelTitle.end(); ++vit )
	{
		m_mapTitle.erase( *vit );
	}

	// 삭제된 타이틀의 미션에 대한 체크 - 기간제 미션 새로 시작
	CheckNewMission( spUser, true );

	//{{ 2009. 6. 5  최육사		타이틀 기간 다됨에 따른 장착 정보 갱신
	spUser->SendUpdateChangeEquippedItem();
	//}}
}

void KUserTitleManager::AddUpdateMission( IN int iMissionID )
{
	m_setDBupdateMission.insert( iMissionID );
}

//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
void KUserTitleManager::DeleteUpdateMission( IN OUT std::vector< KMissionInstance >& vecMission , IN int iMissionID )
{
	std::set< int >::iterator sit = m_setDBupdateMission.find( iMissionID );
	if( sit != m_setDBupdateMission.end() )
	{
		m_setDBupdateMission.erase( sit );
		START_LOG( clog, L"m_setDBupdateMission 완료 미션 삭제 완료" )
			<< BUILD_LOG( iMissionID )
			<< END_LOG;
	}

	{
		std::vector< KMissionInstance >::iterator vit = vecMission.begin();
		for( ; vit != vecMission.end() ; ++vit )
		{
			const KMissionInstance& kMission = (*vit);
			if( kMission.m_iID == iMissionID )
				break;
		}

		if( vit != vecMission.end() )
		{
			vecMission.erase( vit );
			START_LOG( clog, L"vecMission 완료 미션 삭제 완료" )
				<< BUILD_LOG( iMissionID )
				<< END_LOG;
		}
	}
}
#endif SERV_ADD_TITLE_CONDITION
//}}

//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
void KUserTitleManager::GetUpdateInfo( IN bool bIsPcBang, OUT int& iInitTitleID, OUT int& iEquippedTitleID, OUT std::vector< KMissionUpdate >& vecMissionData )
#else
void KUserTitleManager::GetUpdateInfo( IN bool bIsPcBang, OUT short& sInitTitleID, OUT short& sEquippedTitleID, OUT std::vector< KMissionUpdate >& vecMissionData )
#endif SERV_TITLE_DATA_SIZE
//}}
{
	vecMissionData.clear();
	vecMissionData.reserve( m_setDBupdateMission.size() );

	// 진행중인 미션 업데이트
	std::set< int >::const_iterator sit;
	std::map< int, KMissionInstance >::const_iterator mit;
	for( sit = m_setDBupdateMission.begin(); sit != m_setDBupdateMission.end(); ++sit )
	{		
		mit = m_mapMission.find( *sit );
		if( mit == m_mapMission.end() )
		{
			START_LOG( cerr, L"미션 인스턴스 정보가 없는데 DB업데이트를 하려하네?" )
				<< BUILD_LOG( *sit )
				<< END_LOG;
			continue;
		}

		KMissionUpdate kMissionUpdate;
		kMissionUpdate.m_iTitleID = mit->second.m_iID;
		kMissionUpdate.m_vecClearData.reserve( 5 );
		
		std::vector< KSubMissionInstance >::const_iterator vitSub;
		vitSub = mit->second.m_vecSubMissionInstance.begin();

		for( int iIdx = 0; iIdx < MAX_SUB_MISSION_NUM; ++iIdx )
		{
			if( vitSub != mit->second.m_vecSubMissionInstance.end() )
			{
				kMissionUpdate.m_vecClearData.push_back( vitSub->m_sClearData );

				++vitSub;
			}
			else
			{
				kMissionUpdate.m_vecClearData.push_back( 0 );
			}
		}

		vecMissionData.push_back( kMissionUpdate );
	}

	//{{ 2009. 10. 26  최육사	PC방이벤트
	if( bIsPcBang == true )
	{
        if( m_iEquippedTitle == PC_BANG_TITLE_ID )
		{
			//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
			iInitTitleID = m_iEquippedTitle.GetInitValue();
			iEquippedTitleID = m_iEquippedTitle.GetInitValue();
#else
			sInitTitleID = m_iEquippedTitle.GetInitValue();
			sEquippedTitleID = m_iEquippedTitle.GetInitValue();
#endif SERV_TITLE_DATA_SIZE
			//}}			
			return;
		}
	}
	//}}
	
	// 장착중인 타이틀 업데이트
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	iInitTitleID	 = m_iEquippedTitle.GetInitValue(); // 이전에 장착했던 칭호
	iEquippedTitleID = m_iEquippedTitle;				// 현재 장착중인 칭호
	m_iEquippedTitle.SetValue( iEquippedTitleID );
#else
	sInitTitleID	 = m_iEquippedTitle.GetInitValue(); // 이전에 장착했던 칭호
	sEquippedTitleID = m_iEquippedTitle;				// 현재 장착중인 칭호
	m_iEquippedTitle.SetValue( sEquippedTitleID );
#endif SERV_TITLE_DATA_SIZE
	//}}	
}

//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
// 팻 관련 미션 검사
bool KUserTitleManager::GivePetFeed( IN const CXSLItem::ITEM_GRADE eGrade, IN KGSUserPtr spUser ) // 펫 먹이 주기
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return false;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;	
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType != CXSLTitleManager::TMCT_GIVE_PET_FEED )
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_eItemGrade == eGrade )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += 1;
				kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = false;

				if( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData >= pSubMissionTemplet->m_ClearCondition.m_iPetFeedCount )
				{
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;
				}

				kNot.m_vecMissionInst.push_back( kMissionInstance );
				AddUpdateMission( kMissionInstance.m_iID );

				START_LOG( clog, L"미션 TMCT_GIVE_PET_FEED 수행" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iItemID )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					<< END_LOG;
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
		return true;
	}
	return false;
}

//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
void KUserTitleManager::OnUserUnitDie( IN KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON eReason, IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

#ifdef SERV_SUB_TITLE_MISSION_BUG_FIX
		if( kMissionInstance.m_vecSubMissionInstance.size() != pMissionTemplet->m_vecSubMission.size() )
		{
			START_LOG( cerr, L"MISSION TEMPLET 과 INSTANCE 의 SUB MISSION 개수가 틀림! 스크립트 확인 필요!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}
#endif //SERV_SUB_TITLE_MISSION_BUG_FIX

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_USER_UNIT_DIE )
			{
				if( eReason != pSubMissionTemplet->m_ClearCondition.m_eDieReason )
					continue;
			}
			else			// 현재 물속 산소 부족으로 죽음 밖에 없다.
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_USER_UNIT_DIE 수행" )
					<< BUILD_LOG( eReason )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetUID() )
#else
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}
//}}

#endif SERV_ADD_TITLE_CONDITION
//}}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
void KUserTitleManager::OnSocketItem( IN KGSUserPtr spUser, IN int iItemLevel, IN int iSocketUseCount )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_SOCKET )
			{
				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;
			}
			else
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData += static_cast<short>(iSocketUseCount);

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_ITEM_SOCKET 수행" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnEnchantItemLevel( IN KGSUserPtr spUser, IN int iItemLevel, IN int iEnchantLevel, IN bool bEnchantResult )
{
	// 인첸트 레벨 및 인첸트 횟수 같이 처리
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_ENCHANT_LEVEL )
			{
				if( bEnchantResult == false )	// 강화 성공 시에만 처리하자
					continue;

				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;

				if( iEnchantLevel < pSubMissionTemplet->m_ClearCondition.m_iEnchantLevel )
					continue;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
				{
					++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

					kNot.m_vecMissionInst.push_back( kMissionInstance );

					START_LOG( clog, L"미션 TMCT_ITEM_ENCHANT_LEVEL 수행" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetUserName() )
						<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
						<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
						;

					// DB 업데이트
					AddUpdateMission( kMissionInstance.m_iID );
				}
			}
			else if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_ENCHANT_COUNT )
			{
				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
				{
					++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

					if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

					kNot.m_vecMissionInst.push_back( kMissionInstance );

					START_LOG( clog, L"미션 TMCT_ITEM_ENCHANT_COUNT 수행" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( spUser->GetUserName() )
						<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
						<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
						<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
						;

					// DB 업데이트
					AddUpdateMission( kMissionInstance.m_iID );
				}
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnAttribItem( IN KGSUserPtr spUser, IN int iItemLevel )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_ATTRIB )
			{
				if( iItemLevel < pSubMissionTemplet->m_ClearCondition.m_iItemLevel )
					continue;
			}
			else
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_ITEM_ATTRIB 수행" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}

void KUserTitleManager::OnResolveItem( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapMission.empty() )
		return;

	KEGS_UPDATE_MISSION_NOT	kNot;

	std::map< int, KMissionInstance >::iterator mit;
	for( mit = m_mapMission.begin(); mit != m_mapMission.end(); ++mit )
	{
		KMissionInstance& kMissionInstance = mit->second;

		const CXSLTitleManager::MissionTemplet* pMissionTemplet = SiCXSLTitleManager()->GetMissionInfo( kMissionInstance.m_iID );
		if( pMissionTemplet == NULL )
		{
			START_LOG( cerr, L"MISSION TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kMissionInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_TITLE_03 );
			continue;
		}

		for( u_int nSub = 0; nSub < pMissionTemplet->m_vecSubMission.size(); ++nSub )
		{
			const CXSLTitleManager::SubMissionTemplet* pSubMissionTemplet = SiCXSLTitleManager()->GetSubMissionInfo( pMissionTemplet->m_vecSubMission[nSub] );
			if( pSubMissionTemplet == NULL )
			{
				START_LOG( cerr, L"SUB MISSION TEMPLET 얻어오기 실패.!" )
					<< BUILD_LOG( pMissionTemplet->m_vecSubMission[nSub] )
					<< BUILD_LOG( spUser->GetCharUID() )
					<< BUILD_LOG( spUser->GetCharName() )
					<< END_LOG;

				SET_ERROR( ERR_TITLE_03 );
				continue;
			}

			if( pSubMissionTemplet->m_eClearType == CXSLTitleManager::TMCT_ITEM_RESOLVE )
			{
				// 검사 조건이 없다. 그냥 통과
			}
			else
				continue;

			if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount > kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
			{
				++kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData;

				if( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount <= kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess = true;

				kNot.m_vecMissionInst.push_back( kMissionInstance );

				START_LOG( clog, L"미션 TMCT_ITEM_RESOLVE 수행" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubMissionTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubMissionTemplet->m_ClearCondition.m_iDungeonClearCount )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_sClearData )
					<< BUILD_LOG( kMissionInstance.m_vecSubMissionInstance[nSub].m_bIsSuccess )
					;

				// DB 업데이트
				AddUpdateMission( kMissionInstance.m_iID );
			}
		}		
	}

	if( !kNot.m_vecMissionInst.empty() )
	{
		// 미션 중간결과 클라이언트 전송
		spUser->SendPacket( EGS_UPDATE_MISSION_NOT, kNot );

		// 미션 수행 완료 체크 및 보상
		std::vector< KMissionInstance >::const_iterator vitMC;
		for( vitMC = kNot.m_vecMissionInst.begin(); vitMC != kNot.m_vecMissionInst.end(); ++vitMC )
		{
			CheckCompleteMission( vitMC->m_iID, spUser );
		}
	}
}
#endif // SERV_ADD_TITLE_CONDITION_2013_08
