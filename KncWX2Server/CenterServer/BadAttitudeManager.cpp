#include "BadAttitudeManager.h"
#include "BaseServer.h"
#include "BadAttitudeTable.h"
#include "X2Data/XSLDungeonManager.h"
#include "ResultProcess.h"

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

KBadAttitudeManager::KBadAttitudeManager(void)
{
}

KBadAttitudeManager::~KBadAttitudeManager(void)
{
}

void KBadAttitudeManager::init( IN KRoomUserManagerPtr m_spRoomUserManager, IN int iDungeonIDAndDiff )
{
	m_mapUnitInfoList.clear();
	m_mapSubStageMonsterScore.clear();
	m_eDungeonType = CXSLDungeon::DT_INVALID;
	m_iWaveID = 1;

	for( int iUnitIndex = 0 ; iUnitIndex < m_spRoomUserManager->GetNumMember() ; ++iUnitIndex )
	{
		KRoomUserPtr spRoomUser = m_spRoomUserManager->GetUser( iUnitIndex );
		if( IS_NULL( spRoomUser ) )
			continue;

		KBadAttitudeInfo kUnitInfo( spRoomUser->GetCID() );

		m_mapUnitInfoList.insert( std::make_pair( kUnitInfo.m_iUnitUID, kUnitInfo ) );
	}

	m_eDungeonType = SiCXSLDungeonManager()->GetDungeonType( iDungeonIDAndDiff );
	
	// 디펜스 던전이라면 비던이랑 동일한 조건이다
	if( CXSLDungeon::IsDefenceDungeon( iDungeonIDAndDiff ) == true )
	{
		m_eDungeonType = CXSLDungeon::DT_DEFENCE_DUNGEON;
	}
}

bool KBadAttitudeManager::RemoveUnit( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData )
{
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		int iBeforeUnitCount = static_cast<int>(m_mapUnitInfoList.size());

		kUnitData = mit->second;

		m_mapUnitInfoList.erase( mit );

		START_LOG( clog, L"유저 삭제 성공!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iBeforeUnitCount )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"존재하지 않는 유저를 지우려고 합니다." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;

		return false;
	}

	// 퇴장한 유저가 투표를 한것이 있다면 찾아서 지워 주자
	std::map<UidType, KBadAttitudeInfo>::iterator mitAfter = m_mapUnitInfoList.begin();
	for( ; mitAfter != m_mapUnitInfoList.end() ; ++mitAfter )
	{
		std::set<UidType>::iterator sit = mitAfter->second.m_setVotedUnitUID.find( iUnitUID );
		if( sit != mitAfter->second.m_setVotedUnitUID.end() )
		{
			mitAfter->second.m_setVotedUnitUID.erase( sit );
			--mitAfter->second.m_iVotePoint;
		}
	}

	return true;
}

void KBadAttitudeManager::IncreaseBadAttitudeOnePoint( IN UidType iUnitUID )
{
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		mit->second.m_iBadAttitudePoint += 1;
	}
	else
	{
		START_LOG( cerr, L"존재하지 않는 유저에게 불량 점수를 추가하려고 합니다." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;
	}
}

void KBadAttitudeManager::IncreaseVoteOnePoint( IN UidType iBadAttitudeUnitUID, IN UidType iVoteUnitUID )
{ 
	std::map<UidType, KBadAttitudeInfo>::iterator mitVoteUnit = m_mapUnitInfoList.find( iVoteUnitUID );
	if( mitVoteUnit == m_mapUnitInfoList.end() )
	{
		START_LOG( cerr, L"존재하지 않는 유저가 투표하려 합니다." )
			<< BUILD_LOG( iBadAttitudeUnitUID )
			<< BUILD_LOG( iVoteUnitUID )
			<< END_LOG;

		return;
	}

	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iBadAttitudeUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		std::set<UidType>::iterator sit = mit->second.m_setVotedUnitUID.find( iVoteUnitUID );
		if( sit == mit->second.m_setVotedUnitUID.end() )
		{
			mit->second.m_iVotePoint += 1;
			mit->second.m_setVotedUnitUID.insert( iVoteUnitUID );
		}
		else
		{
			START_LOG( clog, L"중보 투표를 하려고 합니다." )
				<< BUILD_LOG( iBadAttitudeUnitUID )
				<< BUILD_LOG( iVoteUnitUID )
				<< BUILD_LOG( static_cast<int>(mit->second.m_setVotedUnitUID.size()) )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"존재하지 않는 유저에게 강퇴 투표 점수를 추가하려고 합니다." )
			<< BUILD_LOG( iBadAttitudeUnitUID )
			<< BUILD_LOG( iVoteUnitUID )
			<< BUILD_LOG( static_cast<int>(m_mapUnitInfoList.size()) )
			<< END_LOG;
	}
}
 
void KBadAttitudeManager::CheckBadAttitudeUnit( OUT std::vector<UidType>& vecNewBadAttitudeUnit, OUT std::vector<UidType>& vecNewForceExitUnit )
{
	vecNewBadAttitudeUnit.clear();
	vecNewForceExitUnit.clear();

	// 던전 타입별 불량 점수, 강퇴 추가 점수 가져오기
	int iBadAttitudeCutLinePoint = SiKBadAttitudeTable()->GetBadAttitudeCutLinePoint( m_eDungeonType );
	int iForceExitPoint = SiKBadAttitudeTable()->GetForceExitPoint( m_eDungeonType );

	if( iBadAttitudeCutLinePoint <= 0 || iForceExitPoint <= 0 )
	{
		START_LOG( cerr, L"던전 타입별 불량 점수 강퇴 추가 점수 값이 이상합니다." )
			<< BUILD_LOG( m_eDungeonType )
			<< BUILD_LOG( iBadAttitudeCutLinePoint )
			<< BUILD_LOG( iForceExitPoint )
			<< END_LOG;
		return;
	}

	// 새로 발견된 불량 유저를 추려 내자
	{
		// 불량유저 임시 저장
		std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.begin();
		for(  ; mit != m_mapUnitInfoList.end() ; ++mit )
		{
			// 불량 유저 발견!
			if( mit->second.m_iBadAttitudePoint >= iBadAttitudeCutLinePoint )
			{
				// 이전에 확인된 불량 유저는 제외
				if( mit->second.m_bIsBadAttitudeUnit == true )
					continue;
				
				mit->second.m_bIsBadAttitudeUnit = true;
				vecNewBadAttitudeUnit.push_back( mit->first );
			}
		}
	}

	BOOST_TEST_FOREACH( UidType, iUnitUID, vecNewBadAttitudeUnit )
	{
		BadAttitudeForceVote( iUnitUID );
	}
	
	// 강제 퇴장 유저를 추려 내자
	{
		int iVoteUnitCountHalf = 100;

		// 전체 투표 인원 수 - 과반수 기준값 구하기
		switch( m_mapUnitInfoList.size() )
		{
		case 2:		// 투표 가능 인원 1  -> 과반수 1
		case 3:		// 투표 가능 인원 2  -> 과반수 1
			{
				iVoteUnitCountHalf = 1;
			}break;
		case 4:		// 투표 가능 인원 3  -> 과반수 2
			{
				iVoteUnitCountHalf = 2;
			}break;
		}
			
		std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.begin();
		for(  ; mit != m_mapUnitInfoList.end()  ; ++mit )
		{
			// 정보 확인
			if( mit->second.m_bIsBadAttitudeUnit == true )
			{
				// 과반수 이상 득표 했는가?
				if( mit->second.m_iVotePoint >= iVoteUnitCountHalf )
				{
					// 전체 벌점을 확인하자
					if( mit->second.m_iBadAttitudePoint >= iBadAttitudeCutLinePoint + iForceExitPoint )
					{
						mit->second.m_bForceExit = true;

						// 새로 발견 했다면 임시 저장해 두자
						vecNewForceExitUnit.push_back( mit->second.m_iUnitUID );
					}
				}
			}
		}
	}
}

void KBadAttitudeManager::BadAttitudeForceVote( IN UidType iBadAttitudeUnitUID )
{
	// 불량 유저가 아니라면 패스!
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iBadAttitudeUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		std::map<UidType, KBadAttitudeInfo>::iterator mitAther = m_mapUnitInfoList.begin();
		for( ; mitAther != m_mapUnitInfoList.end() ; ++mitAther )
		{
			// 자신에게는 투표하지 않음
			if( mitAther->first == mit->first )
				continue;
			
			// 불량 유저라면 강제 투표
			if( mitAther->second.m_bIsBadAttitudeUnit == true )
			{
				IncreaseVoteOnePoint( mitAther->first, mit->first );
			}
		}
	}
}

bool KBadAttitudeManager::IsAllUnitGetScore()
{
	// 모든 유저의 rankinfo 에서 stage, substage 값이 같다면 모두 값을 받았다고 할 수 있다.

	int iStage = -1;
	int iSubStage = -1;
	bool bFirst = true;

	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.begin();
	for( ; mit != m_mapUnitInfoList.end() ; ++mit )
	{
		if( bFirst == true )
		{
			iStage = mit->second.m_kRankInfo.m_iStage;
			iSubStage = mit->second.m_kRankInfo.m_iSubStage;
			bFirst = false;
			continue;
		}
		
		if( (iStage != mit->second.m_kRankInfo.m_iStage) || (iSubStage != mit->second.m_kRankInfo.m_iSubStage) )
		{
			return false;
		}
	}

	return true;
}

void KBadAttitudeManager::SetUnitSubStageInfo( IN UidType iUnitUID, IN int iStage, IN int iSubStage, IN int iDungeonIDDiff, IN int iEndNumMember, IN int iCombo, IN int iTech )
{
	if( iStage < 0 || iSubStage < 0 )
	{
		START_LOG( cerr, L"스테이지 구분 값이 이상합니다." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< BUILD_LOG( iCombo )
			<< BUILD_LOG( iTech )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;
		return;
	}

	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit == m_mapUnitInfoList.end() )
	{
		START_LOG( cerr, L"존재하지 않는 유저에 값을 셋팅 하려 합니다." )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< BUILD_LOG( iCombo )
			<< BUILD_LOG( iTech )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;
		return;
	}

	if( iStage < mit->second.m_kRankInfo.m_iStage && iSubStage < mit->second.m_kRankInfo.m_iSubStage )
	{
		START_LOG( cerr, L"새로운 서브 스테이지 정보가 아닙니다" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< BUILD_LOG( iCombo )
			<< BUILD_LOG( iTech )
			<< BUILD_LOG( iEndNumMember )
			<< END_LOG;
		return;
	}

	// 몬스터 score 값을 구하자
	int iMonsterPoint = 0;
	std::map< std::pair< int, int >, int >::iterator mitMonster = m_mapSubStageMonsterScore.find( std::make_pair(iStage,iSubStage) );
	if( mitMonster != m_mapSubStageMonsterScore.end() )
	{
		iMonsterPoint = mitMonster->second;
	}
	else
	{
		START_LOG( clog, L"존재하지 않는 스테이지, 서브 스테이지에서의 몬스터 점수를 얻으려 합니다." )
			<< BUILD_LOG( iStage )
			<< BUILD_LOG( iSubStage )
			<< END_LOG;
	}

	// 서브 스테이지의 랭크 값을 구해보자
	char cComboRank = SiKResultProcess()->GetSubStageComboResult( iDungeonIDDiff, iEndNumMember, iCombo, iMonsterPoint );
	char cTechRank  = SiKResultProcess()->GetSubStageTechResult( iDungeonIDDiff, iEndNumMember, iTech, iMonsterPoint );

// 	START_LOG( cerr, L"현재 서브 스테이지의 랭크 값을 확인 하자" )
// 		<< BUILD_LOG( iUnitUID )
// 		<< BUILD_LOGc( cComboRank )
// 		<< BUILD_LOGc( cTechRank )
// 		<< BUILD_LOG( iCombo )
// 		<< BUILD_LOG( iTech )
// 		<< BUILD_LOG( iMonsterPoint )
// 		<< BUILD_LOG( iStage )
// 		<< BUILD_LOG( iSubStage )
// 		<< END_LOG;

	mit->second.m_kRankInfo.m_iStage = iStage;
	mit->second.m_kRankInfo.m_iSubStage = iSubStage;
	mit->second.m_kRankInfo.m_cRank = ( cComboRank > cTechRank ) ? cComboRank : cTechRank;		// 높은 랭크를 저장해 둔다

	if( mit->second.m_kRankInfo.m_cRank <= CXSLDungeon::RT_F )
	{
		if( iMonsterPoint > 0 )	// 몬스터 정보가 이상할 경우는 예외처리
		{
			IncreaseBadAttitudeOnePoint( iUnitUID );

//			START_LOG( cerr, L"현재 서브 스테이지의 랭크 값을 확인 하자" )
//				<< BUILD_LOG( iUnitUID )
//				<< BUILD_LOGc( mit->second.m_kRankInfo.m_cRank )
//				<< END_LOG;
		}
	}
}

void KBadAttitudeManager::IncreaseSubStageMonsterDieCount( IN int iStageID, IN int iSubStageID, IN const char cMonsterTypeFactor, IN bool bIsDefenceDungeon/* = false*/ )
{
	int iMonsterScore = static_cast<int>(cMonsterTypeFactor);
	
	std::map< std::pair< int, int >, int >::iterator mit;

	// 일반 던전 일때는 몬스터가 죽은 시점의 스테이지 값을 사용하면 된다.
	if( bIsDefenceDungeon == false )
	{
		// 파라메터의 stage , substage 값을 사용하면 된다.
		mit = m_mapSubStageMonsterScore.find( std::make_pair(iStageID,iSubStageID) );
		if( mit == m_mapSubStageMonsterScore.end() )
		{
			m_mapSubStageMonsterScore.insert( std::make_pair(std::make_pair(iStageID,iSubStageID), iMonsterScore) );
		}
		else
		{
			mit->second += iMonsterScore;
		}
	}
	else		// 디펜스 던전 일때는 
	{
		// 따로 저장해둔 stage , wave 사용하면 된다.
		mit = m_mapSubStageMonsterScore.find( std::make_pair(iStageID,m_iWaveID) );
		if( mit == m_mapSubStageMonsterScore.end() )
		{
			m_mapSubStageMonsterScore.insert( std::make_pair(std::make_pair(iStageID,m_iWaveID), iMonsterScore) );
		}
		else
		{
			mit->second += iMonsterScore;
		}
	}	
}

void KBadAttitudeManager::SetDefenceDungeonWaveID( IN int iWaveID )
{
	if( m_iWaveID > iWaveID )
	{
		START_LOG( cerr, L"이전 wave id 가 큽니다?!" )
			<< BUILD_LOG( m_iWaveID )
			<< BUILD_LOG( iWaveID )
			<< END_LOG;
	}

	m_iWaveID = iWaveID;
}

bool KBadAttitudeManager::GetUnitData( IN UidType iUnitUID, OUT KBadAttitudeInfo& kUnitData )
{
	std::map<UidType, KBadAttitudeInfo>::iterator mit = m_mapUnitInfoList.find( iUnitUID );
	if( mit != m_mapUnitInfoList.end() )
	{
		kUnitData = mit->second;
	}
	else
	{
		START_LOG( cerr, L"존재하지 않는 유저의 정보를 얻으로 합니다." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	return true;
}

#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}