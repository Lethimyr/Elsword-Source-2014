#include "EventMonsterRefreshManager.h"
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <boost/test/utils/foreach.hpp>
#include <KncSend.h>
#include "DBLayer.h"

ImplementSingleton( KEventMonsterRefreshManager );

KEventMonsterRefreshManager::KEventMonsterRefreshManager()
{
}

KEventMonsterRefreshManager::~KEventMonsterRefreshManager()
{
}

ImplToStringW( KEventMonsterRefreshManager )
{
	stm_	<< L"----------[ Event Data Refresh Manager ]----------" << std::endl
		<< TOSTRINGW( m_mapEventMonster.size() )
		;

	return stm_;
}

void	KEventMonsterRefreshManager::GetAdventNpcEventInfo( IN const std::vector< int >& vecEventID, OUT std::vector< KAdventNpcEventInfo >& vecInfo )
{
	vecInfo.clear();

	BOOST_TEST_FOREACH( const int, iEventID, vecEventID )
	{
		std::map< int, KAdventNpcEventInfo >::const_iterator mit;
		mit = m_mapEventMonster.find( iEventID );
		if( mit == m_mapEventMonster.end() )
			continue;

		vecInfo.push_back( mit->second );
	}
}

void	KEventMonsterRefreshManager::UpdateEventMonster( void )
{
	UidType						anTrace[2] = { 0, -1 };
	KEventPtr					spEvent( new KEvent );

	spEvent->SetData( PC_LOG_DB, anTrace, DBE_EVENT_MONSTER_UPDATE_REQ );

	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

void	KEventMonsterRefreshManager::SetEventMonsterData( IN const std::map< int, EVENT_MONSTER_DATA >& mapEventMonster )
{
	KAdventNpcEventInfo									temp;
	std::map< int, EVENT_MONSTER_DATA >::const_iterator cit;
	m_mapEventMonster.clear();

	for( cit=mapEventMonster.begin(); cit != mapEventMonster.end(); ++cit )
	{
		temp.m_iEventID				= cit->second.m_iEventID;
		temp.m_iMonsterDropCount	= cit->second.m_iMonsterDropCount;
		temp.m_bIsAdventInBossStage	= cit->second.m_bIsAdventInBossStage;
		//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
		temp.m_bAllyNPC				= cit->second.m_bAllyNPC;
#endif SERV_ALLY_EVENT_MONSTER
		//}}

		for( std::map<int, float>::const_iterator it=cit->second.m_mapNpcIDAndRate.begin(); it != cit->second.m_mapNpcIDAndRate.end(); ++it )
		{
			temp.m_kAdventNpcLot.AddCase( it->first, it->second );
		}
		temp.m_setAdventDungeon		= cit->second.m_setAdventDungeon;
		temp.m_setNoAdventDungeon	= cit->second.m_setNoAdventDungeon;
		temp.m_setNoAdventStage		= cit->second.m_setNoAdventStage;

		m_mapEventMonster.insert( std::make_pair( cit->second.m_iEventID, temp ) );
	}
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}