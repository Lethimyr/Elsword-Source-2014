#include "GSUser.h"
#include ".\UserEventManager.h"
//{{ 2012. 10. 13	박세훈	접속 이벤트 인자 정리
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
#else
	#include "GameEvent\GameEventManager.h"
#endif SERV_CONNECT_EVENT_FACTOR_SET
//}}



KUserEventManager::KUserEventManager(void)
{
	Clear();
}

KUserEventManager::~KUserEventManager(void)
{
}

void KUserEventManager::Clear()
{
	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	m_bAttendanceCheck = false;
#endif SERV_WEB_POINT_EVENT
	//}}

	m_mapConnectTimeEvent.clear();
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	m_mapCumulativeTimeEvent.clear();
	m_setCompletedEvent.clear();
	m_setDirectRewardEvent.clear();
#endif CUMULATIVE_TIME_EVENT
	//}}
}

//{{ 2012. 10. 13	박세훈	접속 이벤트 인자 정리
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
void KUserEventManager::InitTimeEvent( IN KGameEventManager::ConnectEventFactorSet& kFactor )
{
	if( ( kFactor.m_pVecConnectTimeEvent == NULL ) ||
		( kFactor.m_pVecCumulativeTimeEvent == NULL ) )
	{
		START_LOG( cerr, L"접속 이벤트 인자 값 이상!! ( 있어선 안되는 오류입니다. 즉시 확인해주세요 )" )
			<< BUILD_LOG( kFactor.m_pVecConnectTimeEvent )
			<< BUILD_LOG( kFactor.m_pVecCumulativeTimeEvent )
			<< END_LOG;
		return;
	}

	//////////////////////////////////////////////////////////////////////////	
	// 1. 이벤트 정보 초기화
	Clear();

	//////////////////////////////////////////////////////////////////////////	
	// 2. DB로부터 받아온 유닛별 접속 시간 이벤트 정보 저장

	CTime tEventTime;
	CTime tCurTime = CTime::GetCurrentTime();

	// 누적 시간 이벤트 정보
	for( u_int ui = 0; ui < kFactor.m_pVecCumulativeTimeEvent->size(); ++ui )
	{
		std::pair< KUserCumulativeTimeEventInfo, KTimerManager > kUserCumulativeEvent;
		kUserCumulativeEvent.first.m_iEventUID = ( *kFactor.m_pVecCumulativeTimeEvent )[ui].m_iEventUID;
		kUserCumulativeEvent.second.restart();
		kUserCumulativeEvent.second.SetAddTime( static_cast<double>( ( *kFactor.m_pVecCumulativeTimeEvent )[ui].m_iCumulativeTime * 60 ) );
		m_mapCumulativeTimeEvent.insert( std::make_pair( ( *kFactor.m_pVecCumulativeTimeEvent )[ui].m_iEventUID, kUserCumulativeEvent ) );
	}

	// 접속 시간 이벤트 정보
	for( u_int ui = 0; ui < kFactor.m_pVecConnectTimeEvent->size(); ++ui )
	{
		if( !KncUtil::ConvertStringToCTime( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_wstrEventTime, tEventTime ) )
		{
			START_LOG( cerr, L"문자열 시간 변환 실패." )
				<< BUILD_LOG( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_wstrEventTime )
				<< END_LOG;

			// 문자열 변환 실패시 보상 주지 않는것으로 처리
			tEventTime = CTime( 2050, tCurTime.GetMonth(), tCurTime.GetDay(), 23, 59, 59 );
		}

		// 이미 이벤트 보상을 받았는가?
		if( tCurTime <= tEventTime )
		{
			// 완료 리스트에 넣는다.
			m_setCompletedEvent.insert( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_iEventUID );
			continue;
		}

		// 시간 이벤트 리스트에서 누적 시간 이벤트가 아닌것만 찾아서 저장한다.
		if( m_mapCumulativeTimeEvent.find( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_iEventUID ) == m_mapCumulativeTimeEvent.end() )
		{
			std::pair< KUserConnectTimeEventInfo, KTimerManager > kUserConnectEvent;
			kUserConnectEvent.first.m_fEventTime = END_EVENT; // 기본값으로 종료된 이벤트값을 넣는다
			kUserConnectEvent.first.m_kCTime		= tEventTime.GetTime();
			kUserConnectEvent.first.m_bGiveOnce	= true;
			kUserConnectEvent.second.restart(); // 타이머 초기화
			m_mapConnectTimeEvent.insert( std::make_pair( ( *kFactor.m_pVecConnectTimeEvent )[ui].m_iEventUID, kUserConnectEvent ) );
		}
	}

	kFactor.m_pMapEventTime			= &m_mapConnectTimeEvent;
	kFactor.m_pSetCompletedEvent	= &m_setCompletedEvent;

	// 3. 현재 진행중인 이벤트를 체크하여 EventTime을 얻는다.
	SiKGameEventManager()->CheckEnableConnectTimeEventForCharInit( kFactor );

	// 4. 현재 진행중인 이벤트를 체크하여 EventTime을 얻는다.
	SiKGameEventManager()->CheckEnableCumulativeTimeEventForCharInit(
		kFactor.m_ucLevel,
		kFactor.m_bIsPcBang

		//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		, kFactor.m_iChannelCode
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		, kFactor
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}

		, m_mapCumulativeTimeEvent
		, m_setCompletedEvent
		);

#ifdef SERV_ADVERTISEMENT_EVENT
	for( u_int ui = 0; ui < kFactor.m_pVecCompletedAdvertisementEvent->size(); ++ui )
	{
		if( m_setCompletedEvent.find( ( *kFactor.m_pVecCompletedAdvertisementEvent )[ui] ) == m_setCompletedEvent.end() )
		{
			m_setCompletedEvent.insert( ( *kFactor.m_pVecCompletedAdvertisementEvent )[ui] );
		}
	}
#endif SERV_ADVERTISEMENT_EVENT
}

void KUserEventManager::CheckConnectTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor )
{
	if( spUser == NULL )
		return;

	KDBE_UPDATE_EVENT_TIME_REQ kPacketReq;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotEnd;

	kFactor.m_pMapEventTime			= &m_mapConnectTimeEvent;
	kFactor.m_pSetCompletedEvent	= &m_setCompletedEvent;
	kFactor.m_pVecUpdatedTime		= &kPacketReq.m_vecConnectTimeEvent;
	kFactor.m_pVecBeginEvent		= &kPacketNotBegin.m_vecTimeEvent;
	kFactor.m_pVecEndEvent			= &kPacketNotEnd.m_vecTimeEvent;

	// 이벤트 보상 받을 시간이 다 되었는지 체크, 종료되는 이벤트 체크, 새로 시작하는 이벤트 체크
	SiKGameEventManager()->CheckConnectTimeEvent( kFactor );

	// 접속시간을 충족시켜서 보상받을 이벤트가 있는지?
	if( !kPacketReq.m_vecConnectTimeEvent.empty() )
	{
		// 접속 시간 이벤트 보상 받으러 가자!
		kPacketReq.m_iUnitUID = spUser->GetCharUID();
#ifdef SERV_ADD_EVENT_DB
		spUser->SendToEventDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#else //SERV_ADD_EVENT_DB
		spUser->SendToGameDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#endif //SERV_ADD_EVENT_DB
	}

	// 종료된 이벤트가 있는지?
	if( !kPacketNotEnd.m_vecTimeEvent.empty() )
	{
		kPacketNotEnd.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_END;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotEnd );
	}

	// 새로 시작되는 이벤트가 있는지?
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotBegin );
	}
}
#else
void KUserEventManager::InitTimeEvent( IN const u_char ucLevel,
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
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
									   , IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
									   )
{
	//////////////////////////////////////////////////////////////////////////	
	// 1. 이벤트 정보 초기화
	Clear();

	//////////////////////////////////////////////////////////////////////////	
	// 2. DB로부터 받아온 유닛별 접속 시간 이벤트 정보 저장

	CTime tEventTime;
	CTime tCurTime = CTime::GetCurrentTime();

	// 누적 시간 이벤트 정보
	for( u_int ui = 0; ui < vecCumulativeEventTime.size(); ++ui )
	{
		std::pair< KUserCumulativeTimeEventInfo, KTimerManager > kUserCumulativeEvent;
		kUserCumulativeEvent.first.m_iEventUID = vecCumulativeEventTime[ui].m_iEventUID;
		kUserCumulativeEvent.second.restart();
		kUserCumulativeEvent.second.SetAddTime( static_cast<double>( vecCumulativeEventTime[ui].m_iCumulativeTime * 60 ) );
		m_mapCumulativeTimeEvent.insert( std::make_pair( vecCumulativeEventTime[ui].m_iEventUID, kUserCumulativeEvent ) );
	}

	// 접속 시간 이벤트 정보
	for( u_int ui = 0; ui < vecEventTime.size(); ++ui )
	{
		if( !KncUtil::ConvertStringToCTime( vecEventTime[ui].m_wstrEventTime, tEventTime ) )
		{
			START_LOG( cerr, L"문자열 시간 변환 실패." )
				<< BUILD_LOG( vecEventTime[ui].m_wstrEventTime )
				<< END_LOG;

			// 문자열 변환 실패시 보상 주지 않는것으로 처리
			tEventTime = CTime( 2050, tCurTime.GetMonth(), tCurTime.GetDay(), 23, 59, 59 );
		}

		// 이미 이벤트 보상을 받았는가?
		if( tCurTime <= tEventTime )
		{
			// 완료 리스트에 넣는다.
			m_setCompletedEvent.insert( vecEventTime[ui].m_iEventUID );
			continue;
		}

		// 시간 이벤트 리스트에서 누적 시간 이벤트가 아닌것만 찾아서 저장한다.
		if( m_mapCumulativeTimeEvent.find( vecEventTime[ui].m_iEventUID ) == m_mapCumulativeTimeEvent.end() )
		{
			std::pair< KUserConnectTimeEventInfo, KTimerManager > kUserConnectEvent;
			kUserConnectEvent.first.m_fEventTime = END_EVENT; // 기본값으로 종료된 이벤트값을 넣는다
			kUserConnectEvent.first.m_kCTime		= tEventTime.GetTime();
			kUserConnectEvent.first.m_bGiveOnce	= true;
			kUserConnectEvent.second.restart(); // 타이머 초기화
			m_mapConnectTimeEvent.insert( std::make_pair( vecEventTime[ui].m_iEventUID, kUserConnectEvent ) );
		}
	}

	// 3. 현재 진행중인 이벤트를 체크하여 EventTime을 얻는다.
	SiKGameEventManager()->CheckEnableConnectTimeEventForCharInit(
		ucLevel,
		bIsPcBang

		//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		, iChannelCode
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}

		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		, iBF_Team
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}

		//{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
		, cUnitType
#endif SERV_CHAR_CONNECT_EVENT
		//}}

		, m_mapConnectTimeEvent
		, m_setCompletedEvent
		//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		, iUnitUID
		, mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		, setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		);

	// 4. 현재 진행중인 이벤트를 체크하여 EventTime을 얻는다.
	SiKGameEventManager()->CheckEnableCumulativeTimeEventForCharInit(
		ucLevel,
		bIsPcBang
		
		//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		, iChannelCode
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}

		, m_mapCumulativeTimeEvent
		, m_setCompletedEvent
		);
}

void KUserEventManager::CheckConnectTimeEvent( IN KGSUserPtr spUser
											  //{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
											  , IN const UidType& iUnitUID
											  , IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
											  //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
											  , IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
											  )
{
	if( spUser == NULL )
		return;
	
	KDBE_UPDATE_EVENT_TIME_REQ kPacketReq;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotEnd;

	// 이벤트 보상 받을 시간이 다 되었는지 체크, 종료되는 이벤트 체크, 새로 시작하는 이벤트 체크
	SiKGameEventManager()->CheckConnectTimeEvent( spUser->GetLevel(),
												  spUser->IsPcBang(),
												  //{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
												  spUser->GetChannelCode(),
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
												  //}}
												  //{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
												  spUser->GetBF_Team(),
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
												  //}}
												  //{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
												  spUser->GetUnitType(),
#endif SERV_CHAR_CONNECT_EVENT
												  //}}
												  m_mapConnectTimeEvent, 
												  m_setCompletedEvent, 
												  kPacketReq.m_vecConnectTimeEvent, 
												  kPacketNotBegin.m_vecTimeEvent, 
												  kPacketNotEnd.m_vecTimeEvent
												  //{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
												  , iUnitUID
												  , mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
												 //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
												  , setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
												  );

	// 접속시간을 충족시켜서 보상받을 이벤트가 있는지?
	if( !kPacketReq.m_vecConnectTimeEvent.empty() )
	{
		// 접속 시간 이벤트 보상 받으러 가자!
		kPacketReq.m_iUnitUID = spUser->GetCharUID();
#ifdef SERV_ADD_EVENT_DB
		spUser->SendToEventDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#else //SERV_ADD_EVENT_DB
		spUser->SendToGameDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#endif //SERV_ADD_EVENT_DB
	}

	// 종료된 이벤트가 있는지?
	if( !kPacketNotEnd.m_vecTimeEvent.empty() )
	{
		kPacketNotEnd.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_END;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotEnd );
	}

	// 새로 시작되는 이벤트가 있는지?
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotBegin );
	}
}
#endif SERV_CONNECT_EVENT_FACTOR_SET
//}}

//{{ 2010. 7. 27  최육사	접속 시간 이벤트 버그 수정
#ifdef SERV_TIME_EVENT_BUG_FIX

void KUserEventManager::GetEventTimerInfo( OUT std::map< int, KUserConnectTimeEventInfo >& mapEventTimer )
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapConnectTimeEvent.begin(); mit != m_mapConnectTimeEvent.end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kConnectEventInfo = mit->second.first;
		const KTimerManager& kEventTimer = mit->second.second;

		// 1. 이미 보상을 받았는지 검사
		if( tCurrentTime.GetTime() <= kConnectEventInfo.m_kCTime )
			continue;

		// 2. END_EVENT값은 이미 '끝나버린 이벤트'임.
		if( kConnectEventInfo.m_fEventTime <= END_EVENT )
			continue;

		// 3. 현재 까지 진행한 이벤트 타이머 얻기
		KUserConnectTimeEventInfo kInfo;
		kInfo = kConnectEventInfo;
		kInfo.m_fBackUpTime = kEventTimer.elapsed();
		mapEventTimer.insert( std::make_pair( mit->first, kInfo ) );
	}
}

void KUserEventManager::SetEventTimerInfo( IN const std::map< int, KUserConnectTimeEventInfo >& mapEventTimer )
{
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitFind;
	std::map< int, KUserConnectTimeEventInfo >::const_iterator mit;
	for( mit = mapEventTimer.begin(); mit != mapEventTimer.end(); ++mit )
	{
		mitFind = m_mapConnectTimeEvent.find( mit->first );
		if( mitFind == m_mapConnectTimeEvent.end() )
		{
			// 이벤트가 진행중이 아니면 옛날 채널에서 직접 받아온걸 넣자!
			std::pair< KUserConnectTimeEventInfo, KTimerManager > pairTimeEvent;
			pairTimeEvent.first = mit->second;
			pairTimeEvent.second.SetAddTime( mit->second.m_fBackUpTime );

			m_mapConnectTimeEvent.insert( std::make_pair( mit->first, pairTimeEvent ) );
		}
		else
		{
			// 진행중인 이벤트 정보가 있다면 시간만 연장 하자!
			KTimerManager& kEventTimer = mitFind->second.second;

			kEventTimer.SetAddTime( mit->second.m_fBackUpTime );
		}
	}
}

#else
#pragma ELSE_MESSAGE

void KUserEventManager::GetEventTimerInfo( OUT std::map< int, double >& mapEventTimer )
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapConnectTimeEvent.begin(); mit != m_mapConnectTimeEvent.end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kConnectEventInfo = mit->second.first;
		const KTimerManager& kEventTimer = mit->second.second;

		// 1. 이미 보상을 받았는지 검사
		if( tCurrentTime.GetTime() <= kConnectEventInfo.m_kCTime )
			continue;

		// 2. END_EVENT값은 이미 '끝나버린 이벤트'임.
		if( kConnectEventInfo.m_fEventTime <= END_EVENT )
			continue;

		// 3. 현재 까지 진행한 이벤트 타이머 얻기
		mapEventTimer.insert( std::make_pair( mit->first, kEventTimer.elapsed() ) );
	}
}

void KUserEventManager::SetEventTimerInfo( IN const std::map< int, double >& mapEventTimer )
{
	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::iterator mitFind;
	std::map< int, double >::const_iterator mit;
	for( mit = mapEventTimer.begin(); mit != mapEventTimer.end(); ++mit )
	{
		mitFind = m_mapConnectTimeEvent.find( mit->first );
		if( mitFind == m_mapConnectTimeEvent.end() )
			continue;

		KTimerManager& kEventTimer = mitFind->second.second;

		// 채널이동으로 인한 이벤트 타이머 연장
		kEventTimer.SetAddTime( mit->second );
	}
}

#endif SERV_TIME_EVENT_BUG_FIX
//}}

//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
void KUserEventManager::CheckCumulativeTimeEvent( IN KGSUserPtr spUser, IN KGameEventManager::ConnectEventFactorSet& kFactor )
#else
void KUserEventManager::CheckCumulativeTimeEvent( IN KGSUserPtr spUser )
#endif SERV_2012_WINTER_VACATION_EVENT
//}}
{
	if( spUser == NULL )
		return;

	KDBE_UPDATE_EVENT_TIME_REQ kPacketReq;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_TIME_EVENT_INFO_NOT kPacketNotEnd;

	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	kFactor.m_iWinterVacationEventCount = spUser->GetWinterVacationEventCount();
	kFactor.m_pSetCompletedEvent = &m_setCompletedEvent;
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

	// 이벤트 보상 받을 시간이 다 되었는지 체크, 종료되는 이벤트 체크, 새로 시작하는 이벤트 체크
	SiKGameEventManager()->CheckCumulativeTimeEvent( spUser->GetLevel(),
													 spUser->IsPcBang(), 
													 //{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													 spUser->GetChannelCode(),
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													 //}}
													 //{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
													 kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
													 //}}
													 m_mapCumulativeTimeEvent, 
													 m_setCompletedEvent, 
													 m_setDirectRewardEvent,
													 kPacketReq.m_vecCumulativeTimeEvent, 
													 kPacketNotBegin.m_vecTimeEvent, 
													 kPacketNotEnd.m_vecTimeEvent );

	// 접속시간을 충족시켜서 보상받을 이벤트가 있는지?
	if( !kPacketReq.m_vecCumulativeTimeEvent.empty() )
	{
		// 접속 시간 이벤트 보상 받으러 가자!
		kPacketReq.m_iUnitUID = spUser->GetCharUID();
#ifdef SERV_ADD_EVENT_DB
		spUser->SendToEventDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#else //SERV_ADD_EVENT_DB
		spUser->SendToGameDB( DBE_UPDATE_EVENT_TIME_REQ, kPacketReq );
#endif //SERV_ADD_EVENT_DB
	}

	// 새로 시작되는 이벤트가 있는지?
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotBegin );
	}

	// 종료된 이벤트가 있는지?
	if( !kPacketNotEnd.m_vecTimeEvent.empty() )
	{
		kPacketNotEnd.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_END;
		spUser->SendPacket( EGS_TIME_EVENT_INFO_NOT, kPacketNotEnd );
	}
}

void KUserEventManager::GetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecEventTime )
{
	vecEventTime.clear();

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapCumulativeTimeEvent.begin(); mit != m_mapCumulativeTimeEvent.end(); ++mit )
	{
		// 초기화가 되지 않은 이벤트는 이미 종료된 이벤트이므로 DB에 업데이트 할 필요가 없다.
		if( mit->second.first.m_bInit == false )
			continue;

		// 완료한 이벤트는 DB에 업데이트할 필요가 없다.
		if( m_setCompletedEvent.find( mit->first ) != m_setCompletedEvent.end() )
			continue;

		const KTimerManager& kEventTimer = mit->second.second;

		KCumulativeTimeEventInfo kInfo;
		kInfo.m_iEventUID = mit->first;
		kInfo.m_iCumulativeTime = static_cast<int>( kEventTimer.elapsed() / 60.0 );
		//{{ 2010. 06. 15  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		kInfo.m_bAccountEvent = mit->second.first.m_bAccountEvent;
#endif SERV_ACC_TIME_EVENT
		//}}
		vecEventTime.push_back( kInfo );
	}
}

void KUserEventManager::DeleteDirectRewardAndGetDBUpdateInfo( OUT std::vector< KCumulativeTimeEventInfo >& vecCompleteEvent )
{
	vecCompleteEvent.clear();

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::iterator mit;
	std::set< int >::const_iterator sit;
	for( sit = m_setDirectRewardEvent.begin(); sit != m_setDirectRewardEvent.end(); ++sit )
	{
		mit = m_mapCumulativeTimeEvent.find( *sit );
		if( mit == m_mapCumulativeTimeEvent.end() )
			continue;

		KUserCumulativeTimeEventInfo& kCumulativeEventInfo = mit->second.first;

		// DB에 업데이트할 정보 저장
		KCumulativeTimeEventInfo kInfo;
		kInfo.m_iEventUID = kCumulativeEventInfo.m_iEventUID;
		kInfo.m_iCumulativeTime = static_cast<int>(kCumulativeEventInfo.m_fEventTime);
		vecCompleteEvent.push_back( kInfo );

		// 진행중인 이벤트에서 삭제
		m_mapCumulativeTimeEvent.erase( mit );

		// 완료 이벤트에 추가
		m_setCompletedEvent.insert( *sit );
	}

	// DirectReward 초기화
	m_setDirectRewardEvent.clear();
}

void KUserEventManager::RestartDirectRewardEvent( IN KGSUserPtr spUser, IN const std::vector< int >& vecRestartEvent )
{
	if( spUser == NULL )
		return;

	std::vector< int >::const_iterator vit;
	for( vit = vecRestartEvent.begin(); vit != vecRestartEvent.end(); ++vit )
	{
		// 완료 이벤트에서 삭제 한다.
		m_setCompletedEvent.erase( *vit );
	}

	// 이벤트를 재시작하기위해 이벤트 체크 함수 호출
	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	KGameEventManager::ConnectEventFactorSet kFactor;
	CheckCumulativeTimeEvent( spUser, kFactor );
#else
	CheckCumulativeTimeEvent( spUser );
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}
}

#endif CUMULATIVE_TIME_EVENT
//}}

//{{ 2009. 12. 4  최육사	접속시간이벤트시스템개편
void KUserEventManager::GetNowProcessingTimeEvent( OUT std::vector< KTimeEventSimpleInfo >& vecTimeEvent )
{
	vecTimeEvent.clear();

	std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >::const_iterator mit;
	for( mit = m_mapConnectTimeEvent.begin(); mit != m_mapConnectTimeEvent.end(); ++mit )
	{
		const KUserConnectTimeEventInfo& kConnectEventInfo = mit->second.first;

		KTimeEventSimpleInfo kInfo;
		kInfo.m_iEventUID	  = mit->first;
		kInfo.m_iEventID	  = kConnectEventInfo.m_iEventID;
		kInfo.m_wstrEventName = kConnectEventInfo.m_wstrEventName;
		kInfo.m_fEventTime	  = kConnectEventInfo.m_fEventTime;
		vecTimeEvent.push_back( kInfo );
	}

	std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >::const_iterator mitCT;
	for( mitCT = m_mapCumulativeTimeEvent.begin(); mitCT != m_mapCumulativeTimeEvent.end(); ++mitCT )
	{
		// 완료한 이벤트는 클라이언트에 줄필요 없다.
		if( m_setCompletedEvent.find( mitCT->first ) != m_setCompletedEvent.end() )
			continue;

		const KUserCumulativeTimeEventInfo& kCumulativeEventInfo = mitCT->second.first;
		const KTimerManager& kEventTimer = mitCT->second.second;

		// 초기화 되지 않은 이벤트는 클라이언트에 줄필요 없다.
		if( !kCumulativeEventInfo.m_bInit )
			continue;

		KTimeEventSimpleInfo kInfo;
		kInfo.m_iEventUID	  = mitCT->first;
		kInfo.m_iEventID	  = kCumulativeEventInfo.m_iEventID;
		kInfo.m_wstrEventName = kCumulativeEventInfo.m_wstrEventName;
		kInfo.m_fEventTime	  = kCumulativeEventInfo.m_fEventTime - static_cast<float>( kEventTimer.elapsed() / 60.0 );
		if( kInfo.m_fEventTime < 0.0f )
			kInfo.m_fEventTime = 0.0f;
		vecTimeEvent.push_back( kInfo );
	}
}
//}}

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

void KUserEventManager::CheckWebPointEventQuest( IN int iQuestID, IN KGSUserPtr spUser )
{
	switch( iQuestID )
	{
	case 8650: // 매일매일 포인트 찬스!(2월 25일)
	case 8660: // 매일매일 포인트 찬스!(2월 26일)
	case 8670: // 매일매일 포인트 찬스!(2월 27일)
	case 8680: // 매일매일 포인트 찬스!(2월 28일)
	case 8690: // 매일매일 포인트 찬스!(3월 1일)
	case 8700: // 매일매일 포인트 찬스!(3월 2일)
	case 8710: // 매일매일 포인트 찬스!(3월 3일)
	case 8720: // 매일매일 포인트 찬스!(3월 4일)
	case 8730: // 매일매일 포인트 찬스!(3월 5일)
	case 8740: // 매일매일 포인트 찬스!(3월 6일)
	case 8750: // 매일매일 포인트 찬스!(3월 7일)
	case 8760: // 매일매일 포인트 찬스!(3월 8일)
	case 8770: // 매일매일 포인트 찬스!(3월 9일)
	case 8780: // 매일매일 포인트 찬스!(3월 10일)
	case 8790: // 매일매일 포인트 찬스!(3월 11일)
	case 8800: // 매일매일 포인트 찬스!(3월 12일)
	case 8810: // 매일매일 포인트 찬스!(3월 13일)
	case 8820: // 매일매일 포인트 찬스!(3월 14일)
	case 8830: // 매일매일 포인트 찬스!(3월 15일)
	case 8840: // 매일매일 포인트 찬스!(3월 16일)
	case 8850: // 매일매일 포인트 찬스!(3월 17일)
		break;

	default:
		return;
	}
    
	// 웹 포인트를 올리러 가자!
	KDBE_INCREASE_WEB_POINT_REQ kPacket;
	kPacket.m_iUnitUID = spUser->GetCharUID();
	kPacket.m_wstrUserID = spUser->GetName();
	kPacket.m_cPointType = 2; // 2 : 퀘스트
	kPacket.m_iIncreasePoint = 20;
	spUser->SendToNXWebDB( DBE_INCREASE_WEB_POINT_REQ, kPacket );

	// 클라이언트에 팝업 띄우기
	spUser->SendID( EGS_GET_WEB_POINT_NOT );
}

bool KUserEventManager::CheckAttendance( IN KGSUserPtr spUser )
{
	// 출석체크를 한적이 있는가?
	if( m_bAttendanceCheck == true )
	{
		CTime tCurTime = CTime::GetCurrentTime();

		// 마지막 출석 체크했던 날짜와 지금 날짜가 같다면 DB로 출석체크하러 안간다.
		if( tCurTime.GetDay() == m_tLastDBCheckAttendanceTime.GetDay() )		
			return false;
	}
    
	// DB로 출석체크 하러가자!
	KDBE_ATTENDANCE_CHECK_REQ kPacket;
	kPacket.m_iUnitUID = spUser->GetCharUID();
	spUser->SendToGameDB( DBE_ATTENDANCE_CHECK_REQ, kPacket );
	return true;
}

void KUserEventManager::CheckAttendanceSuccess( IN KGSUserPtr spUser, IN bool bWebPointReward )
{
	// 출석체크 성공 값
	m_bAttendanceCheck = true;

	// 출석체크가 성공한 현재 시간을 저장
	m_tLastDBCheckAttendanceTime = CTime::GetCurrentTime();

	if( bWebPointReward )
	{
		// 웹 포인트를 올리러 가자!
		KDBE_INCREASE_WEB_POINT_REQ kPacket;
		kPacket.m_iUnitUID = spUser->GetCharUID();
		kPacket.m_wstrUserID = spUser->GetName();
		kPacket.m_cPointType = 1; // 1 : 출석체크
		kPacket.m_iIncreasePoint = 10;
		spUser->SendToNXWebDB( DBE_INCREASE_WEB_POINT_REQ, kPacket );
	}
}

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef SERV_ADVERTISEMENT_EVENT
void KUserEventManager::CheckAdvertisementEvent( IN KGSUserPtr spUser )
{
	if( spUser == NULL )
		return;

	KEGS_ADVERTISEMENT_EVENT_INFO_NOT kPacketNotBegin;
	KEGS_ADVERTISEMENT_EVENT_INFO_NOT kPacketNotEnd;
	KDBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT kPacketDB;

	SiKGameEventManager()->CheckEnableAdvertisementEvent( m_setCompletedEvent, kPacketNotBegin.m_vecTimeEvent, kPacketNotEnd.m_vecTimeEvent );

	// 새로 시작되는 이벤트가 있다면 클라이언트에 알리고
	if( !kPacketNotBegin.m_vecTimeEvent.empty() )
	{
		kPacketNotBegin.m_cNotifyType = KEGS_TIME_EVENT_INFO_NOT::TENT_BEGIN;
		spUser->SendPacket( EGS_ADVERTISEMENT_EVENT_INFO_NOT, kPacketNotBegin );
		
		for( u_int ui = 0; ui < kPacketNotBegin.m_vecTimeEvent.size(); ++ui )
		{
			m_setCompletedEvent.insert( kPacketNotBegin.m_vecTimeEvent[ui].m_iEventUID );
			kPacketDB.m_vecCompletedEvent.push_back( kPacketNotBegin.m_vecTimeEvent[ui].m_iEventUID );
		}

		kPacketDB.m_iUserUID = spUser->GetUID();
#ifdef SERV_ADD_EVENT_DB
				spUser->SendToEventDB( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT, kPacketDB );
#else //SERV_ADD_EVENT_DB
				spUser->SendToGameDB( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT, kPacketDB );
#endif //SERV_ADD_EVENT_DB
	}
}

void KUserEventManager::GetAdvertisementURL( OUT std::vector< std::wstring >& vecAdvertisementURL )
{
	SiKGameEventManager()->GetAdvertisementURL( vecAdvertisementURL );
}
#endif SERV_ADVERTISEMENT_EVENT