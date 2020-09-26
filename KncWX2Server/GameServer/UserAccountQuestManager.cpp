#include "GSUser.h"
#include "NetError.h"

#include "GSUser.h"
#include "NetError.h"

#include ".\useraccountquestmanager.h"
#include "x2data/XSLQuestManager.h"

#include "x2data/XSLItemManager.h"
#include "x2data/XSLSocketItem.h"
#include "ExpTable.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM

KUserAccountQuestManager::KUserAccountQuestManager(void) : 
m_iRewardItemCount( 0 )
{
}

KUserAccountQuestManager::~KUserAccountQuestManager(void)
{
}

int	KUserAccountQuestManager::GetQuestRewardItemCount()
{
	int iQuestRewardItemCount = m_iRewardItemCount;
	m_iRewardItemCount = 0;
	return iQuestRewardItemCount;
}

KQuestInstance* KUserAccountQuestManager::GetAccountQuestInstance( const IN int iQuestID )
{
	std::map<int, KQuestInstance>::iterator	mit;
	mit = m_mapAccountQuesting.find( iQuestID );

	if( mit == m_mapAccountQuesting.end() )
		return NULL;

	return &(mit->second);
}

void KUserAccountQuestManager::ClearAccountQuest()
{
	m_mapAccountQuesting.clear();
	m_mapCompletedAccountQuest.clear();
}

bool KUserAccountQuestManager::AddAccountQuest( IN KQuestInstance& kInfo )
{
	SET_ERROR( NET_OK );

	if( IsAccountQuesting( kInfo.m_iID ) == true )
	{
		START_LOG( cerr, L"동일한 QUEST 수행중.. 왜 또왔노..?" )
			<< BUILD_LOG( kInfo.m_iID )
			<< BUILD_LOG( kInfo.m_OwnorUnitUID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_01 );

		return false;
	}

	m_mapAccountQuesting[kInfo.m_iID] = kInfo;
	return true;
}

bool KUserAccountQuestManager::RemoveAccountQuest( IN int iAccountQuestID )
{
	SET_ERROR( NET_OK );

	std::map< int, KQuestInstance >::iterator mit;
	mit = m_mapAccountQuesting.find( iAccountQuestID );
	if( mit == m_mapAccountQuesting.end() )
	{
		START_LOG( cerr, L"없는걸 지우네 .헐~!" )
			<< BUILD_LOG( iAccountQuestID )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );

		return false;
	}

	m_mapAccountQuesting.erase( mit );
	return true;
}

void KUserAccountQuestManager::SetAccountQuest( IN std::vector< KQuestInstance >& vecAccountQuest, IN std::vector< KCompleteQuestInfo >& vecAccountCompleteQuest, IN OUT std::vector< KQuestInstance >& vecQuest, IN OUT std::vector< KCompleteQuestInfo >& vecCompleteQuest, IN KGSUserPtr spUser )
{
	// 계정 퀘스트 초기화
	ClearAccountQuest();

	// 진행 중인 계정 퀘스트 입력
	BOOST_TEST_FOREACH( KQuestInstance&, kQuestInstance, vecAccountQuest )
	{
		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kQuestInstance.m_iID );
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

			START_LOG( cerr, L"계정 QUEST TEMPLET가 없음." )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( kQuestInstance.m_OwnorUnitUID )
				<< END_LOG;
			continue;
		}

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
			case CXSLQuestManager::SQT_CHAR_LEVEL_UP:
				{
					if( static_cast<int>(spUser->GetLevel()) >= pSubTemplet->m_ClearCondition.m_iCharLevel )
						kSub.m_bIsSuccess = true;
				}
				break;
				//{{ 2013. 03. 21	 계정 퀘스트 - 적정 레벨 던전 클리어 조건 추가 - 김민성
#ifdef SERV_ACCOUNT_QUEST_ADD_CONDITION
			case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
				{
					if( kSub.m_ucClearData != 0 && kSub.m_ucClearData >= pSubTemplet->m_ClearCondition.m_iDungeonClearCount )
						kSub.m_bIsSuccess = true;
				}
				break;
#endif SERV_ACCOUNT_QUEST_ADD_CONDITION
				//}
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

		m_mapAccountQuesting.insert( std::make_pair( kQuest.m_iID, kQuest ) );
	}

	// 정리된 진행 중인 계정 퀘스트 데이터를 기존 Quest 목록에 추가(클라에게 전달용)
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
	{
		vecQuest.push_back( mit->second );
	}

	// 완료한 계정 퀘스트 입력
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecAccountCompleteQuest )
	{
		std::map< int, KCompleteQuestInfo >::iterator mitQC = m_mapCompletedAccountQuest.find( kInfo.m_iQuestID );
		if( mitQC == m_mapCompletedAccountQuest.end() )
		{
			m_mapCompletedAccountQuest.insert( std::make_pair( kInfo.m_iQuestID, kInfo ) );

			vecCompleteQuest.push_back( kInfo );
		}
		else
		{
			++(mitQC->second.m_iCompleteCount);
		}
	}

	// DB로부터 가져온 완료된 계정 퀘스트 리스트를 가지고 After Quest의 open조건을 체크한다!
	BOOST_TEST_FOREACH( const KCompleteQuestInfo&, kInfo, vecAccountCompleteQuest )
	{
		CheckAccountQuest_NewQuestByAfterQuest( kInfo.m_iQuestID, spUser, false );
	}

	// 계정 퀘스트의 Before Quest의 open조건을 체크한다!
	CheckAccountQuest_NewQuestByBeforeQuest( spUser );
}

void KUserAccountQuestManager::CheckAccountQuest_NewQuestByAfterQuest( IN const int iQuestID, IN KGSUserPtr spUser, IN const bool bForce )
{
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cwarn, L"존재하지 않는 계정 퀘스트 ID입니다!" ) // cwarn 으로 변경하기
			<< BUILD_LOG( iQuestID )
			<< END_LOG;
		return;
	}

	// 연결퀘스트가 존재하지 않는다면
	if( pQuestTemplet->m_vecAfterQuestID.size() <= 0 )
		return;

	// 다음 퀘스트가 에픽퀘스트인지 검사
	BOOST_TEST_FOREACH( const int, iAfterQuestID, pQuestTemplet->m_vecAfterQuestID )
	{
		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iAfterQuestID );
		if( IS_NULL( pAfterQuestTemplet ) )
		{
			START_LOG( cerr, L"존재 하지 않는 계정 퀘스트 정보입니다!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( iAfterQuestID )
				<< END_LOG;
			continue;
		}

		// after퀘스트가 반복이냐 아니냐를 체크!
		if( pAfterQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_REPEAT )
		{
			// after 가 셋팅 되어 있으면 종류 타입에 상관없이 모두 수락 받도록 하자
			if( bForce == false )	// 강제 수락이 아니라면
			{
				// 에픽과 이벤트는 수락하게 한다.
				if( ( pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EPIC || pAfterQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT ) == false )
				{
					START_LOG( clog, L"강제 수락이 아닌데(캐릭터선택시)...After Quest 로 셋팅된 퀘스트가 에픽, 이벤트가 아니다." ) // clog 로 변경하기
						<< BUILD_LOG( iAfterQuestID )
						<< END_LOG;
					continue;
				}
			}

			// 완료중 이면 검사에서 제외
			if( IsAccountCompleteQuest( iAfterQuestID ) == true )
				continue;
		}

		// 진행중 이면 검사에서 제외
		if( IsAccountQuesting( iAfterQuestID ) == true )
			continue;

		KEGS_NEW_QUEST_REQ kTempReq;
		kTempReq.m_iQuestID = iAfterQuestID;
		kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
		if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
		{
			START_LOG( cwarn, L"New Account Quest Req Fail.!(After)" ) // cwarn 으로 변경하기 
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( kTempReq.m_iQuestID )
				<< END_LOG;
		}
	}
}

void KUserAccountQuestManager::CheckAccountQuest_NewQuestByBeforeQuest( IN KGSUserPtr spUser )
{
	//{{Begin} 퀘스트 클리어시 에픽퀘스트중 BeforeQuestID를 검사하여 자동수락 해주는 기능작성.
	std::vector< CXSLQuestManager::QuestTemplet* > vecAccountQuestTemplet;
	SiCXSLQuestManager()->GetAccountQuestTempletList( vecAccountQuestTemplet );

	// 계정 퀘스트 리스트를 순회하면서 BeforeQuestID 를 검사함.
	BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pAccountQuest, vecAccountQuestTemplet )
	{
		if( pAccountQuest == NULL )
		{
			START_LOG( cerr, L"계정 퀘스트 포인터값이 이상합니다!" )
				<< BUILD_LOG( vecAccountQuestTemplet.size() )
				<< END_LOG;
			continue;
		}

		// 완료중 이면 검사에서 제외
		if( IsAccountCompleteQuest( pAccountQuest->m_iID ) == true )
			continue;

		// 진행중 이면 검사에서 제외
		if( IsAccountQuesting( pAccountQuest->m_iID ) == true )
			continue;

		// 아직 받지않은 계정 퀘스트 중 받을수 있는 퀘스트를 체크
		bool bIsSuccessAccount = true;

		// m_vecBeforeQuestID의 내용이 비었을시 검사에서 제외
		if( pAccountQuest->m_Condition.m_vecBeforeQuestID.size() == 0 )
		{
			bIsSuccessAccount = false;
		}

		BOOST_TEST_FOREACH( const int, iBeforeQuestID, pAccountQuest->m_Condition.m_vecBeforeQuestID )
		{
			if( iBeforeQuestID != 0  &&  IsAccountCompleteQuest( iBeforeQuestID ) == false )
			{
				bIsSuccessAccount = false;
				break;
			}
		}

		if( bIsSuccessAccount )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pAccountQuest->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
			{
				START_LOG( cwarn, L"New Account Quest Req Fail.!(Before)" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( kTempReq.m_iQuestID )
					<< END_LOG;
			}
		}		
	}
}

bool KUserAccountQuestManager::IsAccountQuesting( IN int iAccountQuestID )
{
	std::map< int, KQuestInstance >::iterator mit = m_mapAccountQuesting.find( iAccountQuestID );
	if( mit != m_mapAccountQuesting.end() )
	{
		// 존재함
		// 현재 진행중인 계정 단위 퀘스트입니다.
		return true;
	}

	return false;
}

bool KUserAccountQuestManager::IsAccountCompleteQuest( IN int iAccountQuestID )
{
	std::map< int, KCompleteQuestInfo >::iterator mit = m_mapCompletedAccountQuest.find( iAccountQuestID );
	if( mit != m_mapCompletedAccountQuest.end() )
	{
		// 존재함
		// 완료한 계정 단위 퀘스트입니다.
		return true;
	}

	return false;
}

void KUserAccountQuestManager::CheckAccountQuest( IN KGSUserPtr spUser )
{
	// 수락 상태가 아니지만 스크립트에 있으면 수락하도록 한다.
	std::vector<int> vecNewEventQuest;
	std::vector< CXSLQuestManager::QuestTemplet* > vecAccountQuestTemplet;
	SiCXSLQuestManager()->GetAccountQuestTempletList( vecAccountQuestTemplet );

	if( vecAccountQuestTemplet.empty() == false )
	{
		BOOST_TEST_FOREACH( const CXSLQuestManager::QuestTemplet*, pAccountQuest, vecAccountQuestTemplet )
		{
			if( pAccountQuest == NULL )
			{
				START_LOG( cerr, L"계정 퀘스트 포인터값이 이상합니다!" )
					<< BUILD_LOG( vecAccountQuestTemplet.size() )
					<< END_LOG;
				continue;
			}

			std::map< int, KQuestInstance >::iterator mitQuest = m_mapAccountQuesting.find( pAccountQuest->m_iID );
			// 수락된 상태가 아니라면, 진행중인
			if( mitQuest == m_mapAccountQuesting.end() )
			{
				if( IsAccountCompleteQuest( pAccountQuest->m_iID ) == true )
					continue;

				KEGS_NEW_QUEST_REQ kTempReq;
				kTempReq.m_iQuestID = pAccountQuest->m_iID;
				kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;

				if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
				{
					START_LOG( cwarn, L"신규 계정 퀘스트 수락 실패!" )
						<< BUILD_LOG( spUser->GetCharName() )
						<< BUILD_LOG( kTempReq.m_iQuestID )
						<< END_LOG;
				}
			}
		}
	}
}

int KUserAccountQuestManager::GetAccountQuestCompleteCount( IN const int iQuestID )
{
	std::map< int, KCompleteQuestInfo >::const_iterator mit;
	mit = m_mapCompletedAccountQuest.find( iQuestID );
	if( mit == m_mapCompletedAccountQuest.end() )
	{
		return 0;
	}

	return mit->second.m_iCompleteCount;
}

bool KUserAccountQuestManager::Handler_EGS_NEW_QUEST_REQ_Account( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser )
{
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 계정 퀘스트.!" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		return false;
	}
#ifdef SERV_NAVER_CHANNELING
    // 채널링 퀘스트로 설정한 경우에 퍼블리셔가 다르면 해당 퀘스트를 등록할 수 없게 한다. 2014-01-22 우상혁
    if ( pQuestTemplet->m_ePublisher != spUser->GetChannelCode() ) 
    { // AccountQuest.lua 에 m_eQuestType 항목이 없는 경우는 default 로 0 이다.
        return false;
    }
#endif SERV_NAVER_CHANNELING

	// PC방 전용 퀘스트인지 체크
	if( pQuestTemplet->m_bIsPcBang == true )
	{
		if( spUser->IsPcBang() == false )
		{
			return false;
		}
	}

	//# 계정 퀘스트 이면 최대수락 개수의 영향을 받지 않는다.

	//이미 하고 있는 퀘스트인지..
	if( IsAccountQuesting( kReq.m_iQuestID ) == true )
	{
		START_LOG( cwarn, L"이미 가지고 있는 계정 퀘스트를 등록하려함" )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< END_LOG;

		return false;
	}

	// 하루 단위 퀘스트
	if( pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_DAY )
	{
		// 이미 완료한 퀘스트 인가?
		KCompleteQuestInfo* pCompleteQuestInfo = GetAccountCompleteQuest( kReq.m_iQuestID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"퀘스트 완료는 했는데 완료날짜 정보가 이상합니다." )
					<< BUILD_LOG( kReq.m_iQuestID )
					<< END_LOG;

				SET_ERROR( ERR_QUEST_04 );
				return false;
			}

			// 이미 완료한 퀘스트라면 클리어한 날짜를 확인하여 오늘 수행 가능한지 체크한다.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR )
			{
				// 기준시(게임에서 리셋되는 시각) 를 넘는 시각이라면 다음날로 세팅
				tCheckTime += CTimeSpan( 1, 0, 0, 0 );
			}

			// 오늘 수행 가능한가요?
			if( tCheckTime > tCurTime )
			{
				SET_ERROR( ERR_QUEST_04 );
				return false;
			}
		}
	}

	// To do...
	//[ing]QUEST 수행을 위해 모아야할 아이템 체크
	if( pQuestTemplet->m_Condition.m_iConditionItemID != 0 )
	{
		if(spUser->IsExistInventoryItem( pQuestTemplet->m_Condition.m_iConditionItemID ) == false )
		{
			return false;
		}
	}

	//QUEST 수행을 위해 이전에 완료해야할 퀘스트 검사.
	BOOST_TEST_FOREACH( const int, iBeforeQuestID, pQuestTemplet->m_Condition.m_vecBeforeQuestID )
	{
		if( GetAccountQuestCompleteCount( iBeforeQuestID ) == 0 )
		{
			START_LOG( clog, L"어여가서 하나 더해오셔.. !!" )
				<< BUILD_LOG( kReq.m_iQuestID )
				<< BUILD_LOG( iBeforeQuestID )
				<< END_LOG;

			return false;
		}
	}

	// 레벨 검사
	// 퀘스트를 받기 낮은 레벨이라면
	if( pQuestTemplet->m_Condition.m_iLevel	> spUser->GetLevel() )
	{
		START_LOG( clog, L"계정 퀘스트인데 이런 조건은 정상적이지 않은것 같음" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		return false;
	}
	// 퀘스트를 받기 높은 레벨이라면
	//	m_iLimitLevel 가 -1 이면 존재하지 않는다는 의미
	if( pQuestTemplet->m_Condition.m_iLimitLevel != -1 && pQuestTemplet->m_Condition.m_iLimitLevel < spUser->GetLevel() )
	{
		START_LOG( clog, L"계정 퀘스트인데 이런 조건은 정상적이지 않은것 같음" )
			<< BUILD_LOG( pQuestTemplet->m_Condition.m_iLimitLevel )
			<< BUILD_LOGc( spUser->GetLevel() )
			<< END_LOG;

		return false;
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

					return false;
				}
			}

			// 퀘스트 수락 보상이 들어갈만한 빈슬롯이 있는지 검사
			if( spUser->IsEnoughSpaceExist( pQuestTemplet->m_mapAcceptRewardItem ) == false )
			{
				START_LOG( clog, L"퀘스트 수락시 받는 보상이 들어갈 인벤토리 빈공간이 부족합니다." )
					<< BUILD_LOG( pQuestTemplet->m_iID )
					<< BUILD_LOG( pQuestTemplet->m_mapAcceptRewardItem.size() )
					<< END_LOG;

				return false;
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

		kDBReq.m_UserUID	= spUser->GetUID();

		spUser->SendToAccountDB( DBE_NEW_ACCOUNT_QUEST_REQ, kDBReq );
	}

	//{{ 2007. 12. 26  최육사  DB 통계 퀘스트
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( kReq.m_iQuestID );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_Accept, 1 );
		KSIManager.IncreaseCount( KStatistics::SI_QUEST, kKey, KStatistics::eSIColQuest_AcceptLevel, static_cast<int>(spUser->GetLevel()) );

		//{{ 2007. 12. 26  최육사  유저 통계
		int iColumnIndex = KUserStatistics::US_Etc_NQuestAccept;

		spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
		//}}
	}
	//}}

	return true;
}

void KUserAccountQuestManager::Handler_EGS_QUEST_COMPLETE_REQ_Account( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin /* = false */ )
{
	SET_ERROR( NET_OK );

	bool bCheckAfterQuest = true;
	std::map<int,int> mapAcceptRewardItem;
	std::vector<int>::const_iterator vitAfterQuest;

	// 수행중인지 확인한다.	
	if( IsAccountQuesting( kReq.m_iQuestID ) == false )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 퀘스트 인스턴스 얻기
	KQuestInstance* pKQuestInst = GetAccountQuestInstance( kReq.m_iQuestID );
	if( pKQuestInst == NULL )
	{
		SET_ERROR( ERR_QUEST_03 );
		goto error_proc;
	}

	// 퀘스트 템플릿 얻기
	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kReq.m_iQuestID );
	if( pQuestTemplet == NULL )
	{
		START_LOG( cerr, L"계정 퀘스트 진행중인데 템플릿 데이터가 없다." )
			<< BUILD_LOG( kReq.m_iQuestID )
			<< BUILD_LOG( spUser->GetCharName() )
			<< BUILD_LOG( spUser->GetUserName() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_08 );
		goto error_proc;
	}

	vitAfterQuest = pQuestTemplet->m_vecAfterQuestID.begin();
	for( ; vitAfterQuest != pQuestTemplet->m_vecAfterQuestID.end() ; ++vitAfterQuest )
	{
		if( bCheckAfterQuest == false )
			break;

		const CXSLQuestManager::QuestTemplet* pAfterQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( *vitAfterQuest );
		if( pAfterQuestTemplet == NULL )
		{
			START_LOG( cerr, L"존재하지 않는 계정 퀘스트가 after퀘스트로 등록되어있습니다!" )
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
				<< BUILD_LOG( spUser->GetCharName() )
				<< BUILD_LOG( spUser->GetUserName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto error_proc;
		}

		//운영자 명령이면 수행하지 않는다.
		if( bIsAdmin == true )
			continue;

		// 계정 퀘스트는 아직 해당 조건이 없다.
/*
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
*/
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
		kDBReq.m_bAutoComplete = kReq.m_bAutoComplete;
		kDBReq.m_UnitUID	= spUser->GetCharUID();
		kDBReq.m_iQuestID	= kReq.m_iQuestID;
		kDBReq.m_bIsRepeat	= ( pQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_NORMAL );	
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

		if( mapItem.size() > 0 )
		{
			std::map<int,int> mapQuestConditionItem;
			kDBReq.m_mapInsertedItem = mapItem;	//보상받을 아이템 리스트.

			// 계정퀘스트 에는 아이템 수집 퀘스트 조건이 없음
/*
			//운영자면 아이템 수집퀘스트 경우도 해당 아이템을 삭제하지 않기위해.
			if( bIsAdmin == false )
			{
				//SiCXSLQuestManager()->GetQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
				SiCXSLQuestManager()->GetAccountQuestConditionItemList( kDBReq.m_iQuestID, mapQuestConditionItem );
			}
*/
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
				//인벤이 부족하면 그냥 유저에게 알려주고 완료를 하지 않는다.
				SET_ERROR( ERR_QUEST_13 );
				goto error_proc;
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
			int iColumnIndex = KUserStatistics::US_Etc_NQuestComplete;

			spUser->UserStatisticsIncreaseCount( KUserStatistics::USTable_EtcData, 0, iColumnIndex, 1 );
			//}}
		}
		//}}

		spUser->SendToAccountDB( DBE_ACCOUNT_QUEST_COMPLETE_REQ, kDBReq );

		return;
	}

error_proc:

	KEGS_QUEST_COMPLETE_ACK kAck;
	//완료체크를 위해 퀘스트 ID를 다시 돌려준다.
	kAck.m_kCompleteQuestInfo.m_iQuestID = kReq.m_iQuestID;
	kAck.m_iOK	= NetError::GetLastError();
	spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kAck );
}

void KUserAccountQuestManager::Handler_DBE_QUEST_COMPLETE_ACK_Account( IN const KDBE_QUEST_COMPLETE_ACK& kAck, IN KGSUserPtr spUser )
{
	const int iQuestID = kAck.m_kCompleteQuestInfo.m_iQuestID;

	//DB처리 성공한 경우
	SET_ERROR( NET_OK );

	KEGS_QUEST_COMPLETE_ACK	kPacket;
	kPacket.m_kCompleteQuestInfo.m_iQuestID = iQuestID;

	if( IsAccountQuesting( iQuestID ) == false )
	{
		START_LOG( cerr, L"+ㅁ+;; 계정 퀘스트가 진행중 퀘스트에 없네." )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		SET_ERROR( ERR_QUEST_02 );
		goto end_proc;
	}

	START_LOG( clog, L"계정 퀘스트 완료 ::::" )
		<< BUILD_LOG( spUser->GetCharUID() )
		<< BUILD_LOG( spUser->GetCharName() )
		<< BUILD_LOG( spUser->GetUserName() )
		<< BUILD_LOG( kAck.m_iOK )
		<< BUILD_LOG( iQuestID );

	// Quest Instance 삭제
	if( RemoveAccountQuest( iQuestID ) == true )
	{
		// 완료 처리
		std::map< int, KCompleteQuestInfo >::iterator mitCQ;
		mitCQ = m_mapCompletedAccountQuest.find( iQuestID );
		if( mitCQ == m_mapCompletedAccountQuest.end() )
		{
			m_mapCompletedAccountQuest.insert( std::make_pair( iQuestID, kAck.m_kCompleteQuestInfo ) );
		}
		else
		{
			++(mitCQ->second.m_iCompleteCount); // 완료 카운트를 올리고..
			mitCQ->second.m_tCompleteDate = kAck.m_kCompleteQuestInfo.m_tCompleteDate; // 완료 날짜를 변경한다.
		}

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iQuestID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"ACCOUNT QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( iQuestID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			goto end_proc;
		}		

		kPacket.m_iOK		= NetError::GetLastError();

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

		//{{ 2009. 1. 20  최육사	만렙경험치 예외처리
		if( spUser->GetLevel() < SiKGameSysVal()->GetLimitsLevel() )
		{
			spUser->m_kEXP.AddExp( pQuestTemplet->m_Reward.m_iEXP );
		}
		//}}

#ifdef SERV_PVP_NEW_SYSTEM	// 잘못된 디파인 해외팀 수정
		spUser->AddAPoint( pQuestTemplet->m_Reward.m_iSP );
#else
		spUser->m_iSPoint += pQuestTemplet->m_Reward.m_iSP;
#endif SERV_PVP_NEW_SYSTEM

		spUser->CheckCharLevelUp();

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

		spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );


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
		CheckAccountQuest_NewQuestByAfterQuest( pQuestTemplet->m_iID, spUser, true );

		// BeforeQuestID로 등록된 열리지 않은 완료 퀘스트가 있는지 확인하여 퀘스트를 오픈한다.
		CheckAccountQuest_NewQuestByBeforeQuest( spUser );	

		// 이벤트 반복 퀘스트라면 재수락 받도록 하자 
		if( pQuestTemplet->m_eQuestType == CXSLQuestManager::QT_EVENT
			&& pQuestTemplet->m_eRepeatType == CXSLQuestManager::QRT_REPEAT )
		{
			KEGS_NEW_QUEST_REQ kTempReq;
			kTempReq.m_iQuestID = pQuestTemplet->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
			{
				START_LOG( cwarn, L"[에픽] New Account Quest Req Fail.!(After)" )
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

	kPacket.m_iOK = NetError::GetLastError();
	spUser->SendPacket( EGS_QUEST_COMPLETE_ACK, kPacket );

}

void KUserAccountQuestManager::Handler_CharaterLevelUp_Account( IN KGSUserPtr spUser )
{
	SET_ERROR( NET_OK );

	if( m_mapAccountQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	CTime tCurr = CTime::GetCurrentTime();
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"Account QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}
#ifdef SERV_NAVER_CHANNELING
        if( pQuestTemplet->m_ePublisher != spUser->GetChannelCode() ) // 채널링 전용 퀘스트
        {
            continue;
        }
#endif SERV_NAVER_CHANNELING

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		// 날짜, 요일 조건에 맞지 않으면 완료가 되지 않게 한다.
		// 활성화 시간이 아니면
		if( SiCXSLQuestManager()->IsValidAccountQuestDate( kQuestInstance.m_iID, tCurr ) == false )
		{
			continue;
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

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

			// 2. 레벨업 퀘스트가 맞는지
			if( pSubQuestTemplet->m_eClearType != CXSLQuestManager::SQT_CHAR_LEVEL_UP )
				continue;

			// 3. 해당 레벨이 맞는지
			if( pSubQuestTemplet->m_ClearCondition.m_iCharLevel > static_cast<int>( spUser->GetLevel() ) )
				continue;

			// 4. 이미 완료한 퀘스트 인지 확인!
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData != 0 )
				continue;

			// 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup_AccountQuest( pQuestTemplet->m_iID, pQuestTemplet->m_vecSubQuest[uiIndexSub], spUser ) == false )
				continue;

			{
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData = 1;
				kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

				kNot.m_vecQuestInst.push_back( kQuestInstance );

				START_LOG( clog, L"Account 퀘스트 SQT_CHAR_LEVEL_UP 수행" )
					<< BUILD_LOG( static_cast<int>( spUser->GetLevel() ) )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOG( pSubQuestTemplet->m_ClearCondition.m_iCharLevel )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					<< END_LOG;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
	}
}

bool KUserAccountQuestManager::CheckCompleteSubQuest_BeforGroup_AccountQuest( IN int iQuestID, IN int iSubQuestID, IN KGSUserPtr spUser )
{
	// 그룹핑 된 서브퀘스트 중 앞 그룹이 모두 클리어 되었는지 확인한다.

	const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( iQuestID );
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
	std::map< int, KQuestInstance >::iterator mit = m_mapAccountQuesting.find( iQuestID );
	if( mit == m_mapAccountQuesting.end() )
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

void KUserAccountQuestManager::GetAccountQuesting( OUT std::vector< KQuestUpdate >& vecQuestData )
{
	std::map< int, KQuestInstance >::const_iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
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

//{{ 2013. 03. 21	 계정 퀘스트 - 적정 레벨 던전 클리어 조건 추가 - 김민성
#ifdef SERV_ACCOUNT_QUEST_ADD_CONDITION
void KUserAccountQuestManager::Handler_OnDungeonClear( IN const KGSUserPtr spUser, 
													   IN const int iDungeonID, 
													   IN const char cDifficulty, 
													   IN const char cDungeonMode, 
													   IN const std::set< UidType >& setPartyMembers,
													   IN const std::map< UidType, bool >& mapHaveExpInDungeon,
													   IN const std::map< UidType, bool >& mapSuitableLevelInfo )
{
	SET_ERROR( NET_OK );

	if( m_mapAccountQuesting.empty() == true )
		return;

	KEGS_UPDATE_QUEST_NOT kNot;

	//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	CTime tCurr = CTime::GetCurrentTime();
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}

	std::map< int, KQuestInstance >::iterator mit;
	for( mit = m_mapAccountQuesting.begin(); mit != m_mapAccountQuesting.end(); ++mit )
	{
		KQuestInstance& kQuestInstance = mit->second;

		const CXSLQuestManager::QuestTemplet* pQuestTemplet = SiCXSLQuestManager()->GetAccountQuestTemplet( kQuestInstance.m_iID );
		if( pQuestTemplet == NULL )
		{
			START_LOG( cerr, L"ACCOUNT QUEST TEMPLET 얻어오기 실패.!" )
				<< BUILD_LOG( kQuestInstance.m_iID )
				<< BUILD_LOG( spUser->GetCharUID() )
				<< BUILD_LOG( spUser->GetCharName() )
				<< END_LOG;

			SET_ERROR( ERR_QUEST_08 );
			continue;
		}

		if( pQuestTemplet->m_vecSubQuest.empty() == true )
			continue;

		//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		// 날짜, 요일 조건에 맞지 않으면 완료가 되지 않게 한다.
		// 활성화 시간이 아니면
		if( SiCXSLQuestManager()->IsValidAccountQuestDate( kQuestInstance.m_iID, tCurr ) == false )
		{
			continue;
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

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

			bool bRVal = false;
			bool bValidDungeonID = false;

			// 1. 완료된건지 체크
			if( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
				continue;

			// 2. 유효한 던전이라면
			if( SiCXSLQuestManager()->CheckValidDungeonID( pSubQuestTemplet->m_iID ) == true ) 
			{
				bValidDungeonID = true;

				// SubQuest 에 있는 던전을 플레이 한 것이 아니라면
				if( SiCXSLQuestManager()->IsExistDungeonInSubQuest( pSubQuestTemplet->m_iID, iDungeonID + static_cast<int>(cDifficulty), pSubQuestTemplet->m_ClearCondition.m_bUpperDifficulty ) == false )
					continue;
			}

			// 3. 하위 그룹의 sub Quest 가 모두 완료 된 상태가 아니라면
			if( CheckCompleteSubQuest_BeforGroup_AccountQuest( pQuestTemplet->m_iID, pSubQuestTemplet->m_iID, spUser ) == false )
				continue;

			// 4. 타입에 따른 조건 체크
			switch( pSubQuestTemplet->m_eClearType )
			{

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
							bRVal = true;
						}
					}
				}
				break;
			}

			if( bRVal == true )
			{
				switch( pSubQuestTemplet->m_eClearType )
				{
				case CXSLQuestManager::SQT_SUITABLE_LEVEL_DUNGEON_CLEAR:
					{
						//{{ 2010. 05. 03  최육사	던전 클리어 카운트 버그 수정
						// 클리어 조건 카운트를 넘지 않게 보정한다.
						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount > static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
						{
							kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData += 1;
						}
						//}}

						if( pSubQuestTemplet->m_ClearCondition.m_iDungeonClearCount <= static_cast<int>(kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData) )
							kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess = true;

						kNot.m_vecQuestInst.push_back( kQuestInstance );
					}
					break;

				default:
					{
						START_LOG( cerr, L"존재하지 않는 Accout Quest 의 클리어 타입입니다." )
							<< BUILD_LOG( pQuestTemplet->m_iID )
							<< BUILD_LOG( pSubQuestTemplet->m_iID )
							<< BUILD_LOG( pSubQuestTemplet->m_eClearType )
							<< END_LOG;
						continue;
					}
					break;
				}

				START_LOG( clog, L"Account Quest 관련 던전 종료시 퀘스트 조건 완료" )
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( spUser->GetUserName() )
					<< BUILD_LOG( pSubQuestTemplet->m_wstrDescription )
					<< BUILD_LOGc( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_ucClearData )
					<< BUILD_LOG( kQuestInstance.m_vecSubQuestInstance[uiIndexSub].m_bIsSuccess )
					<< END_LOG;
			}
		}
	}

	if( kNot.m_vecQuestInst.empty() == false )
	{
		spUser->SendPacket( EGS_UPDATE_QUEST_NOT, kNot );
		spUser->SendUpdateDropQuestItemByIngQuest();
	}
}

bool KUserAccountQuestManager::IsSuitableLevelUser( IN UidType CharUID, IN const std::map< UidType, bool >& mapSuitableLevelInfo )
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
#endif SERV_ACCOUNT_QUEST_ADD_CONDITION
//}

//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
void KUserAccountQuestManager::CheckResetDayAccountQuest_AutoAccept( IN KGSUserPtr spUser )
{
	// 이벤트 타입의 퀘스트를 모두 가져온다.
	std::vector< CXSLQuestManager::QuestTemplet* > vecAccountQuestTemplet;
	SiCXSLQuestManager()->GetAccountQuestTempletList( vecAccountQuestTemplet );

	// 수락이 가능한 퀘스트 인지 확인하자
	std::vector<int> vecCompleteDayQuest;
	BOOST_TEST_FOREACH( CXSLQuestManager::QuestTemplet*, pAccountQuestTemplet, vecAccountQuestTemplet )
	{
		if( pAccountQuestTemplet == NULL )
		{
			continue;
		}

		// 이벤트 일일 퀘스트가 아니면 다음!
		if( pAccountQuestTemplet->m_eRepeatType != CXSLQuestManager::QRT_DAY )
		{
			continue;
		}

		// 수락 할 것인가?
		bool bAccept = false;

		// 이미 완료한 퀘스트 인가?
		KCompleteQuestInfo* pCompleteQuestInfo = GetAccountCompleteQuest( pAccountQuestTemplet->m_iID );
		if( pCompleteQuestInfo != NULL )
		{
			if( pCompleteQuestInfo->m_tCompleteDate == 0 )
			{
				START_LOG( cerr, L"퀘스트 완료는 했는데 완료날짜 정보가 이상합니다." )
					<< BUILD_LOG( pAccountQuestTemplet->m_iID )
					<< END_LOG;

				continue;
			}

			// 이미 완료한 퀘스트라면 클리어한 날짜를 확인하여 오늘 수행 가능한지 체크한다.
			CTime tCompleteDate( pCompleteQuestInfo->m_tCompleteDate );
			CTime tCurTime = CTime::GetCurrentTime();

			CTime tCheckTime = CTime( tCompleteDate.GetYear(), tCompleteDate.GetMonth(), tCompleteDate.GetDay(), CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR, 0, 0 );
			if( tCompleteDate.GetHour() >= CXSLQuestManager::QE_ACCOUNT_DAILY_REPEAT_HOUR )
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
			kTempReq.m_iQuestID = pAccountQuestTemplet->m_iID;
			kTempReq.m_iTalkNPCID = CXSLUnitManager::NUI_NONE;
			if( Handler_EGS_NEW_QUEST_REQ_Account( kTempReq, spUser ) == false )
			{
				START_LOG( cwarn, L"New Account Quest Req Fail.!(After)" ) // cwarn 으로 변경하기 
					<< BUILD_LOG( spUser->GetCharName() )
					<< BUILD_LOG( kTempReq.m_iQuestID )
					<< END_LOG;
			}
		}
	}
}

KCompleteQuestInfo*	KUserAccountQuestManager::GetAccountCompleteQuest( IN int iAccountQuestID )
{
	std::map< int, KCompleteQuestInfo >::iterator mit;
	mit = m_mapCompletedAccountQuest.find( iAccountQuestID );
	if( mit == m_mapCompletedAccountQuest.end() )
		return NULL;

	return &(mit->second);
}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
//}

#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}