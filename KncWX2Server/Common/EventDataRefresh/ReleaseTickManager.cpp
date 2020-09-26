#include "ReleaseTickManager.h"
#include <KncSend.h>

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#include "ReleaseTickTypeDefine.h"
#include "DBLayer.h"

#include "EventDataRefreshManager.h"
#include "EventMonsterRefreshManager.h"
#include "EventNotifyMsgServerInfo.h"
#include "EventRewardDataRefreshManager.h"
#include "EventRewardLevelUpRefreshManager.h"
#include "EventRewardRetainUserItemRefreshManager.h"

#include "AbuserLog/KAbuserLogManager.h"

ImplementSingleton( KReleaseTickManager );

KReleaseTickManager::KReleaseTickManager()
{
}

KReleaseTickManager::~KReleaseTickManager()
{
}

ImplToStringW( KReleaseTickManager )
{
	stm_	<< L"----------[ Release Tick Manager ]----------" << std::endl
		<< TOSTRINGW( m_mapReleaseTick.size() )
		;

	return stm_;
}

void	KReleaseTickManager::Init( void )
{
	SendReleaseTickListReq();
}

void	KReleaseTickManager::Tick( void )
{
	if( 60.0 < m_TimerRefresh.elapsed() )
	{
		SendReleaseTickListReq();
		m_TimerRefresh.restart();
	}
}

void	KReleaseTickManager::SendReleaseTickListReq( void )
{
	UidType		anTrace[2] = { 0, -1 };
	KEventPtr	spEvent( new KEvent );

	spEvent->SetData( PC_LOG_DB, anTrace, DBE_RELEASE_TICK_UPDATE_REQ );

	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

void	KReleaseTickManager::SetReleaseTickData( IN const std::map< int, int >& mapNewReleaseTick )
{
	// 정의 되어 있는 범위를 벗어난다면 잘못된 데이터로 간주하여 처리하지 않는다.
	if( RTT_END < mapNewReleaseTick.size() )
	{
		START_LOG( cerr, L"Release Tick Type의 갯수가 정의되어진 것보다 많습니다." )
			<< BUILD_LOG( mapNewReleaseTick.size() )
			<< END_LOG;
		return;
	}
	if( mapNewReleaseTick.size() < m_mapReleaseTick.size() )
	{
		START_LOG( cwarn, L"Release Tick Type의 갯수가 감소 하였습니다." )
			<< BUILD_LOG( m_mapReleaseTick.size() )
			<< BUILD_LOG( mapNewReleaseTick.size() )
			<< END_LOG;
		return;
	}

	std::map< int, int >::const_iterator m_cit;

	// 변화가 있는 ReleaseTick의 경우에만 해당 Manager에게 통보해준다.
	std::map< int, int >::const_iterator cit;
	for( cit = mapNewReleaseTick.begin(); cit != mapNewReleaseTick.end(); ++cit )
	{
		m_cit = m_mapReleaseTick.find( cit->first );
		if( ( m_cit != m_mapReleaseTick.end() ) && ( cit->second <= m_cit->second ) )
			continue;

		switch( cit->first )
		{
		case RTT_ABUSER_TYPE_PVP:
		case RTT_ABUSER_TYPE_ITEM:
		case RTT_ABUSER_TYPE_ENCHANT:
		case RTT_ABUSER_TYPE_ED:
		case RTT_ABUSER_TYPE_MONITORING:
		case RTT_ABUSER_TYPE_TRAFFIC:			SiKAbuserLogManager()->UpdateAbuserList( cit->first );						break;

		case RTT_EVENT_DATA_TYPE:				SiKEventDataRefreshManager()->UpdateEventData();							break;
		case RTT_EVENT_MONSTER_TYPE:			SiKEventMonsterRefreshManager()->UpdateEventMonster();						break;
		case RTT_EVENT_NOTIFY_MSG_SERVER_INFO:	SiKEventNotifyMsgServerInfo()->UpdateEventNotifyMsgServerInfo();			break;

		case RTT_REWARD_DATA:					SiKEventRewardDataRefreshManager()->UpdateRewardData();						break;
		//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
		case RTT_REWARD_LEVEL_UP:				SiKEventRewardLevelUpRefreshManager()->UpdateRewardLevelUp();				break;
#endif SERV_CHAR_LEVEL_UP_EVENT
		//}}

		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		case RTT_REWARD_RETAIN_USER_ITEM:		SiKEventRewardRetainUserItemRefreshManager()->UpdateRewardRetainUserItem();	break;
#endif SERV_COME_BACK_USER_REWARD
		//}}
		}
	}

	// 가지고 있는 ReleaseTick 정보를 갱신 해준다.
	m_mapReleaseTick.clear();
	m_mapReleaseTick = mapNewReleaseTick;
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}