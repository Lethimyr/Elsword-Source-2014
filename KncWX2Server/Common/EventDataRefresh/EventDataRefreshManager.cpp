#include "EventDataRefreshManager.h"
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include <KncSend.h>
#include "DBLayer.h"

ImplementSingleton( KEventDataRefreshManager );

KEventDataRefreshManager::KEventDataRefreshManager()
{
}

KEventDataRefreshManager::~KEventDataRefreshManager()
{
}

ImplToStringW( KEventDataRefreshManager )
{
	stm_	<< L"----------[ EventData Refresh Manager ]----------" << std::endl
		<< TOSTRINGW( m_mapEventData.size() )
		;

	return stm_;
}

const EVENT_DATA* KEventDataRefreshManager::GetEventData( IN int iScriptID ) const
{
	std::map< int, EVENT_DATA >::const_iterator mit = m_mapEventData.find( iScriptID );
	if( mit == m_mapEventData.end() )
	{
		START_LOG( cerr, L"존재 하지 않는 이벤트입니다." )
			<< BUILD_LOG( iScriptID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

void	KEventDataRefreshManager::UpdateEventData( void )
{
	UidType						anTrace[2] = { 0, -1 };
	KEventPtr					spEvent( new KEvent );

	spEvent->SetData( PC_LOG_DB, anTrace, DBE_EVENT_DATA_UPDATE_REQ );

	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

void	KEventDataRefreshManager::SetEventData( IN const std::map< int, EVENT_DATA >& mapEventData )
{
	m_mapEventData.clear();
	m_mapEventData	= mapEventData;
}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}