#include "EventRewardLevelUpRefreshManager.h"
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB

//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT

#include <KncSend.h>
#include "DBLayer.h"

ImplementSingleton( KEventRewardLevelUpRefreshManager );

KEventRewardLevelUpRefreshManager::KEventRewardLevelUpRefreshManager(void)
{
}

KEventRewardLevelUpRefreshManager::~KEventRewardLevelUpRefreshManager(void)
{
}

ImplToStringW( KEventRewardLevelUpRefreshManager )
{
	stm_	<< L"----------[ Event Reward LevelUp ]----------" << std::endl
		<< TOSTRINGW( m_mapLevelUpRewardItem.size() )
		;

	return stm_;
}

bool KEventRewardLevelUpRefreshManager::GetLevelUpRewardItem( IN u_char& ucLevel, OUT std::vector< int >& vecRewardItem )
{
	std::map< u_char, std::vector< int > >::iterator mit;
	mit = m_mapLevelUpRewardItem.find( ucLevel );
	if( mit == m_mapLevelUpRewardItem.end() )
	{
		START_LOG( clog , L"레벨업 아이템 보상이 없다." )
			<< BUILD_LOGc( ucLevel )
			<< END_LOG;

		return false;
	}

	std::vector< int >::iterator vit = mit->second.begin();
	if( vit == mit->second.end() )
	{
		return false;
	}

	vecRewardItem = mit->second;

	return true;
}
void KEventRewardLevelUpRefreshManager::UpdateRewardLevelUp( void )
{
	UidType						anTrace[2] = { 0, -1 };
	KEventPtr					spEvent( new KEvent );

	spEvent->SetData( PC_LOG_DB, anTrace, DBE_EVENT_REWARD_LEVEL_UP_UPDATE_REQ );

	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

void KEventRewardLevelUpRefreshManager::SetRewardLevelUp( IN const std::map< u_char, std::vector< int > >& mapLevelUpRewardItem )
{
	m_mapLevelUpRewardItem.clear();
	m_mapLevelUpRewardItem = mapLevelUpRewardItem;
}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}