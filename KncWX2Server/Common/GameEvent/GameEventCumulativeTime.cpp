#include ".\gameeventcumulativetime.h"

KGameEventCumulativeTime::KGameEventCumulativeTime(void) : 
m_fEventTime( 0.0f ),
m_iEventID( 0 ),
m_iRewardID( 0 ),
//{{ 2009. 10. 26  최육사	PC방이벤트
m_bPcBangEvent( false ),
//}}
//{{ 2009. 12. 8  최육사	접속시간이벤트
m_bDirectReward( false ),
//}}
//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
m_iChannelCode( -1 ),
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
//}}
//{{2010. 06. 11  최육사	계정단위 접속시간 이벤트
m_bAccountEvent( false )
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
, m_bNewUnitEvent( false )
, m_bNewUnitEvent2( false )
, m_bCurrentUnitEvent( false )
#endif SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
,m_iCustomEventID( 0 )
#endif //SERV_CUSTOM_CONNECT_EVENT
{
}

KGameEventCumulativeTime::~KGameEventCumulativeTime(void)
{
}
