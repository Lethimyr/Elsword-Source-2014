#pragma once
#include "Event.h"
#include "CommonPacket.h"
//#include <boost/timer.hpp>
#include "TimerManager.h"
//#include "GameEvent\GameEventConnectTime.h"
//{{ 2009. 12. 7  최육사	누적시간이벤트
//#ifdef CUMULATIVE_TIME_EVENT
//	#include "GameEvent\GameEventCumulativeTime.h"
//#endif CUMULATIVE_TIME_EVENT
//}}

//{{ 2012. 10. 13	박세훈	접속 이벤트 인자 정리
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
	#include "GameEvent\GameEventManager.h"
#endif SERV_CONNECT_EVENT_FACTOR_SET
//}}


SmartPointer(KGSUser);

class KUserEventManager
{
public:

	enum USER_EVENT_ENUM
	{
		EVENT_TIME_CONVERT = 60,
	};

	KUserEventManager(void);
	~KUserEventManager(void);

	void		Clear();

	//////////////////////////////////////////////////////////////////////////	
	// 접속 시간 이벤트

	//{{ 2012. 10. 13	박세훈	접속 이벤트 인자 정리
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
	void		InitTimeEvent( IN KGameEventManager::ConnectEventFactorSet& kFactor );
	void		CheckConnectTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor );
#else
	void		InitTimeEvent( IN const u_char ucLevel,
							   IN const bool bIsPcBang, 
							   //{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
							   IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
							   //}}
							   //{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
							   IN const int iBF_Team,
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
							   //}}
							   //{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
							   IN const char cUnitType,
#endif SERV_CHAR_CONNECT_EVENT
							   //}}
							   IN const std::vector< KConnectTimeEventInfo >& vecEventTime, 
							   IN const std::vector< KCumulativeTimeEventInfo >& vecCumulativeEventTime
							   //{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
							   , IN const UidType& iUnitUID
							   , IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
							   //}}
							   );
	
	//////////////////////////////////////////////////////////////////////////	
	// 접속 시간 이벤트
		
	void		CheckConnectTimeEvent( IN KGSUserPtr spUser
		//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		, IN const UidType& iUnitUID
		, IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		, IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		);
#endif SERV_CONNECT_EVENT_FACTOR_SET
	//}}
	
	//{{ 2010. 7. 27  최육사	접속 시간 이벤트 버그 수정
#ifdef SERV_TIME_EVENT_BUG_FIX
	void		GetEventTimerInfo( OUT std::map< int, KUserConnectTimeEventInfo >& mapEventTimer );			// 이벤트 타이머 정보 얻기
	void		SetEventTimerInfo( IN const std::map< int, KUserConnectTimeEventInfo >& mapEventTimer );	// 이벤트 타이머 정보 저장
#else
#pragma ELSE_MESSAGE
	void		GetEventTimerInfo( OUT std::map< int, double >& mapEventTimer );		// 이벤트 타이머 정보 얻기
	void		SetEventTimerInfo( IN const std::map< int, double >& mapEventTimer );	// 이벤트 타이머 정보 저장
#endif SERV_TIME_EVENT_BUG_FIX
	//}}	

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	void		CheckCumulativeTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor );
#else
	void		CheckCumulativeTimeEvent( IN KGSUserPtr spUser );
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}

	void		GetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecEventTime );

	//{{ 2009. 12. 8  최육사	크리스마스이벤트
	bool		IsCompleteDirectRewardEvent() { return !m_setDirectRewardEvent.empty(); }
	void		DeleteDirectRewardAndGetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecCompleteEvent );
	void		RestartDirectRewardEvent( IN KGSUserPtr spUser, IN const std::vector< int >& vecRestartEvent );
	//}}

#endif CUMULATIVE_TIME_EVENT
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 공용 함수
	//{{ 2009. 12. 4  최육사	접속시간이벤트시스템개편
	void		GetNowProcessingTimeEvent( OUT std::vector< KTimeEventSimpleInfo >& vecTimeEvent );
	//}}	

	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	void		CheckWebPointEventQuest( IN int iQuestID, IN KGSUserPtr spUser );
	bool		CheckAttendance( IN KGSUserPtr spUser );
	void		CheckAttendanceSuccess( IN KGSUserPtr spUser, IN bool bWebPointReward );
#endif SERV_WEB_POINT_EVENT
	//}}

#ifdef SERV_ADVERTISEMENT_EVENT
	void		CheckAdvertisementEvent( IN KGSUserPtr spUser );
	void		GetAdvertisementURL( OUT std::vector< std::wstring >& vecAdvertisementURL );
#endif SERV_ADVERTISEMENT_EVENT
private:
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >		m_mapConnectTimeEvent;		// 현재 진행중인 접속시간 이벤트
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >	m_mapCumulativeTimeEvent;	// 현재 진행중인 누적시간 이벤트	
	std::set< int >									m_setCompletedEvent;		// 완료한 이벤트
	std::set< int >									m_setDirectRewardEvent;		// 직접 보상 받아야하는 이벤트
#endif CUMULATIVE_TIME_EVENT
	//}}	

	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	bool											m_bAttendanceCheck;
	CTime											m_tLastDBCheckAttendanceTime;
#endif SERV_WEB_POINT_EVENT
	//}}	
};

