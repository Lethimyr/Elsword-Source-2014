#include ".\UserUnitSelectRewardManager.h"
#include "GameServer.h"


//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY


KUserUnitSelectRewardManager::KUserUnitSelectRewardManager(void)
{
	Clear();
}

KUserUnitSelectRewardManager::~KUserUnitSelectRewardManager(void)
{
}

void KUserUnitSelectRewardManager::Clear()
{
	m_vecAccessItem.clear();
}

void KUserUnitSelectRewardManager::AddRewardItemInfo( IN const int iAddRewardID )
{
	// 중복 되는 아이템은 지급 하지 않는다
	BOOST_TEST_FOREACH( const int, iRewardID, m_vecAccessItem )
	{
		if( iRewardID == iAddRewardID )
		{
			START_LOG( clog, L"중복 되는 아이템을 지급하려 합니다." )
				<< BUILD_LOG( iAddRewardID )
				<< BUILD_LOG( iRewardID )
				<< END_LOG;
			return;
		}
	}

	m_vecAccessItem.push_back( iAddRewardID );
}

void KUserUnitSelectRewardManager::GetRewardAndClear( OUT std::vector< int >& vecRewardInfo )
{
	// 보상 정보를 얻는다!
	vecRewardInfo = m_vecAccessItem;

	// 보상 정보를 넘겨주었으면 초기화 시킨다!
	Clear();
}

//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
void KUserUnitSelectRewardManager::SetFirstSelectUnitRewardInfo( IN std::vector<KFirstSelectUnitReward>& vecInfo )
{
	if( vecInfo.empty() == true )
		return;

	m_mapFirstSelectUnitRewardInfo.clear();

	BOOST_TEST_FOREACH( KFirstSelectUnitReward&, kInfo, vecInfo )
	{
		m_mapFirstSelectUnitRewardInfo.insert( std::make_pair(kInfo.m_iDBIndexID, kInfo) );
	}
}

bool KUserUnitSelectRewardManager::GetFirstSelectUnitRewardInfo( IN int iServerGroupID, OUT std::vector<KFirstSelectUnitReward>& vecInfo )
{
	if( m_mapFirstSelectUnitRewardInfo.empty() == true )
	{
		return false;
	}

	vecInfo.clear();

	std::map<int, KFirstSelectUnitReward>::iterator mit = m_mapFirstSelectUnitRewardInfo.begin();
	for( ; mit != m_mapFirstSelectUnitRewardInfo.end() ; ++mit )
	{
		if( mit->second.m_cGiveType == iServerGroupID 
		 || mit->second.m_cGiveType == KFirstSelectUnitReward::GGT_ONLY_ONE )
		{
			vecInfo.push_back( mit->second );
		}
	}

	if( vecInfo.empty() == true )
	{
		return false;
	}

	return true;
}

void KUserUnitSelectRewardManager::UpdateFirstSelectUnitRewardInfo( IN int iDBIndexID, UidType iUnitUID )
{
	std::map<int, KFirstSelectUnitReward>::iterator mit = m_mapFirstSelectUnitRewardInfo.begin();
	for( ; mit != m_mapFirstSelectUnitRewardInfo.end() ; ++mit )
	{
		if( mit->first == iDBIndexID )
		{
			mit->second.m_iUnitUID = iUnitUID;
			return;
		}
	}
}

void KUserUnitSelectRewardManager::DeleteAndGetFirstSelectUnitRewardInfo( IN int iDBIndexID, KFirstSelectUnitReward& kReward )
{
	std::map<int, KFirstSelectUnitReward>::iterator mit = m_mapFirstSelectUnitRewardInfo.begin();
	for( ; mit != m_mapFirstSelectUnitRewardInfo.end() ; ++mit )
	{
		if( mit->first == iDBIndexID )
		{
			kReward = mit->second;

			m_mapFirstSelectUnitRewardInfo.erase( mit );

			return;
		}
	}
}

#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

#endif SERV_SECOND_SECURITY
//}}