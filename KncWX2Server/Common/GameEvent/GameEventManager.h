#pragma once

#include <KNCSingleton.h>
#include <ToString.h>
#include "DBLayer.h"
#include <KncSend.h>
#include <boost/timer.hpp>
#include "ServerPacket.h"

#include "GameEventQuest.h"
#include "GameEventItemDrop.h"
#include "GameEventReward.h"
#include "GameEventTournament.h"
#include "GameEventConnectTime.h"
#include "GameEventNotifyMSG.h"
#include "GameEventSecretDungeon.h"
//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	#include "GameEventCumulativeTime.h"
#endif CUMULATIVE_TIME_EVENT
//}}
//{{ 2010. 07. 16  최육사	이벤트 몬스터 시스템
#ifdef SERV_EVENT_MONSTER
	#include "GameEventMonster.h"
#endif SERV_EVENT_MONSTER
//}}
//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	#include "GameEventHenirReward.h"
#endif SERV_NEW_HENIR_TEST
//}}

#ifdef SERV_ADVERTISEMENT_EVENT
#include "GameEventAdvertisement.h"
#endif SERV_ADVERTISEMENT_EVENT

//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "TickDumper.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}
//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	#include "GameEventQuestItemDrop.h"
#endif SERV_QUEST_ITEM_DROP_EVENT
//}
#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
	#include "GameEventDefenseDungeon.h"
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef SERV_CRITERION_DATE_EVENT// 작업날짜: 2013-06-26	// 박세훈
	#include "GameEventScriptManager.h"
#endif // SERV_CRITERION_DATE_EVENT

#include "TimerManager.h"

#define END_EVENT		-1.f

#ifdef SERV_CODE_EVENT
#include "ScriptID_Code.h"
#define IF_EVENT_ENABLED(id)		if( SiKGameEventManager()->IsEnableCode( id ) == true )
#define IF_EVENT_NOT_ENABLED(id)	if( SiKGameEventManager()->IsEnableCode( id ) == false )
#define ELSE_IF_EVENT_ENABLED(id)	else if( SiKGameEventManager()->IsEnableCode( id ) == true )
#define ELSE						else
#endif SERV_CODE_EVENT

class KGameEventManager
{
	DeclareSingleton( KGameEventManager );
	DeclDump;
	DeclToStringW;
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	DeclareTickDumper;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

	enum SERVER_TYPE
	{
		ST_NONE = 0,
		ST_LOGIN,
		ST_CENTER,
		ST_GAME,
	};

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	enum EventReleaseTickType
	{
		ERTT_EVENT_DB_SCRIPT_CHECK	= 1, // SERV_EVENT_DB_CONTROL_SYSTEM 에서 사용
		ERTT_EVENT_CHECK	= 2, // SERV_REFRESH_EVENT_USING_RELEASE_TICK 에서 사용
	};
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	enum EventReleaseTickType
	{
		ERTT_EVENT_CHECK	= 30,
	};
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	//{{ 2012. 12. 17	최육사	이벤트 보너스 코드 리팩토링
	struct SGameEventBonusRate
	{
		float	m_fEXPRate;
		float	m_fVPRate;
		float	m_fEDRate;
		float	m_fPartyEXPRate;
		float	m_fPartyVPRate;
		float	m_fPartyEDRate;
		//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		float	m_fWithCharEXPRate;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
		float	m_fRankingPointRate;
		float	m_fPartyRankingPointRate;
#endif	// SERV_RANKING_POINT_EVENT

		SGameEventBonusRate()
		{
			Clear();
		}

		void Clear()
		{
			m_fEXPRate = 0.f;
			m_fVPRate = 0.f;
			m_fEDRate = 0.f;
			m_fPartyEXPRate = 0.f;
			m_fPartyVPRate = 0.f;
			m_fPartyEDRate = 0.f;
			//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			m_fWithCharEXPRate = 0.f;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			//}}
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
			m_fRankingPointRate			= 0.0f;
			m_fPartyRankingPointRate	= 0.0f;
#endif	// SERV_RANKING_POINT_EVENT
		}
	};
	//}}

public:
	//{{ 2012. 10. 13	박세훈	접속 이벤트 인자 정리
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
	struct ConnectEventFactorSet
	{
		UidType m_iUnitUID;
		u_char m_ucLevel;
		bool m_bIsPcBang;
		//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		int m_iChannelCode;
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}
		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		int m_iBF_Team;
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
		//{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
		char m_cUnitType;
		char m_cUnitClass;
#endif SERV_CHAR_CONNECT_EVENT
		//}}
		/*const */std::vector< KConnectTimeEventInfo >*							m_pVecConnectTimeEvent;
		/*const */std::vector< KCumulativeTimeEventInfo >*						m_pVecCumulativeTimeEvent;

		std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >*	m_pMapEventTime;
		std::set< int >*														m_pSetCompletedEvent;
		std::vector< KConnectTimeEventInfo >*									m_pVecUpdatedTime;
		std::vector< KTimeEventSimpleInfo >*									m_pVecBeginEvent;
		std::vector< KTimeEventSimpleInfo >*									m_pVecEndEvent;

#ifdef SERV_ADVERTISEMENT_EVENT
		std::vector< int >*														m_pVecCompletedAdvertisementEvent;
#endif SERV_ADVERTISEMENT_EVENT

		bool																	m_bDelFinishEvent;
		//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		const std::multimap<int, KDailyGiftBoxInfo>&												m_mapDailyGiftBoxList;
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		const std::wstring		m_wstrWinterVacationEventRegDate;
		int						m_iWinterVacationEventCount;
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
		UidType					m_iUserUID;
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
		//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		std::set< int >			m_setItemID;
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
		const byte				m_byteCriterionDateEventInfo;
		const std::wstring		m_wstrRegDate;
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_CUSTOM_CONNECT_EVENT
		int						m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
		bool					m_bIsSteamUser;
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		bool m_bIsNewUnit;
		bool m_bIsNewUnit2;
		bool m_bIsCurrentUnit;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_NEW_USER_QUEST_HARD_CODING
		const std::wstring		m_wstrRegDate;
#endif	// SERV_NEW_USER_QUEST_HARD_CODING

		ConnectEventFactorSet(
			IN const UidType iUnitUID,
			IN const u_char	ucLevel,
			IN const bool	bIsPcBang
			//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			, IN const int	iChannelCode
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			//}}
			//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			, IN const int	iBF_Team
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			//}}
			//{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
			, IN const char	cUnitType
			, IN const char cUnitClass
#endif SERV_CHAR_CONNECT_EVENT
			//}}

			, IN const bool bDelFinishEvent
			//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			, IN const std::multimap<int, KDailyGiftBoxInfo>& mapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
			//}}
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
			, IN const std::wstring& wstrWinterVacationEventRegDate
			, IN const int iWinterVacationEventCount
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
			, IN const UidType iUserUID
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
		//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
			, IN const std::set< int >& setItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
			, IN const byte byteCriterionDateEventInfo
			, IN const std::wstring& wstrRegDate
#endif	// SERV_CRITERION_DATE_EVENT
#ifdef SERV_CUSTOM_CONNECT_EVENT
			, IN const int iCustomEventID
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
			, IN const bool bIsSteamUser
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
			, IN bool bIsNewUnit
			, IN bool bIsNewUnit2
			, IN bool bIsCurrentUnit
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_NEW_USER_QUEST_HARD_CODING
			, IN const std::wstring& wstrRegDate
#endif	// SERV_NEW_USER_QUEST_HARD_CODING
			)
			:	m_iUnitUID( iUnitUID ),
			m_ucLevel( ucLevel ),
			m_bIsPcBang( bIsPcBang )
			//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			, m_iChannelCode( iChannelCode )
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			//}}
			//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			, m_iBF_Team( iBF_Team )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			//}}
			//{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
			, m_cUnitType( cUnitType )
			, m_cUnitClass( cUnitClass )
#endif SERV_CHAR_CONNECT_EVENT
			//}}
			
			, m_bDelFinishEvent( bDelFinishEvent )
			//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
			, m_mapDailyGiftBoxList( mapDailyGiftBoxList )
#endif SERV_EVENT_DAILY_GIFT_BOX
			//}}
			//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
			, m_wstrWinterVacationEventRegDate( wstrWinterVacationEventRegDate )
			, m_iWinterVacationEventCount( iWinterVacationEventCount )
#endif SERV_2012_WINTER_VACATION_EVENT
			//}}
			//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
			, m_iUserUID( iUserUID )
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
			//}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
			, m_setItemID( setItemID )
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
			, m_byteCriterionDateEventInfo( byteCriterionDateEventInfo )
			, m_wstrRegDate( wstrRegDate )
#endif // SERV_CRITERION_DATE_EVENT
#ifdef SERV_CUSTOM_CONNECT_EVENT
			, m_iCustomEventID( iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
			, m_bIsSteamUser( bIsSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
			, m_bIsNewUnit(bIsNewUnit)
			, m_bIsNewUnit2(bIsNewUnit2)
			, m_bIsCurrentUnit(bIsCurrentUnit)
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_NEW_USER_QUEST_HARD_CODING
			, m_wstrRegDate( wstrRegDate )
#endif	// SERV_NEW_USER_QUEST_HARD_CODING
		{
			m_pVecConnectTimeEvent		= NULL;
			m_pVecCumulativeTimeEvent	= NULL;
			m_pMapEventTime				= NULL;
			m_pSetCompletedEvent		= NULL;
			m_pVecUpdatedTime			= NULL;
			m_pVecBeginEvent			= NULL;
			m_pVecEndEvent				= NULL;
#ifdef SERV_ADVERTISEMENT_EVENT
			m_pVecCompletedAdvertisementEvent = NULL;
#endif SERV_ADVERTISEMENT_EVENT
		}
	};
#endif SERV_CONNECT_EVENT_FACTOR_SET
	//}}

	KGameEventManager();
	virtual ~KGameEventManager();

	void		SetServerType( KGameEventManager::SERVER_TYPE eServerType ){ m_eServerType = eServerType; }
	SERVER_TYPE	GetServerType() const{ return m_eServerType; }

	virtual	void	Tick();

	void	Init();

	// DB에서 이벤트 패킷이 올경우..
	void	SetEvent( IN const std::vector< KEventInfo >& vecEventList );

	// 던전 보상 이벤트
	//{{ 2012. 12. 17	최육사	이벤트 보너스 코드 리팩토링
	void	GetRewardEvent( OUT SGameEventBonusRate& kBonusRateInfo
						  , IN const int iDungeonID
						  , IN const int iUnitLevel
						  , IN const char cUnitClass
						  , IN const int iPartyNum
#ifdef SERV_PCBANG_USER_REWARD_EVENT// 작업날짜: 2013-07-02	// 박세훈
						  , IN const bool isPcBang
#endif // SERV_PCBANG_USER_REWARD_EVENT
							);
	//}}

	//{{ 2010. 05. 11  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	void	GetRewardEvent( IN OUT KRoomUserInfo& kRoomUserInfo
						  , IN const int iDungeonID
						  , IN const int iUnitLevel
						  , IN const char cUnitClass
						  , IN const int iPartyNum
#ifdef SERV_PCBANG_USER_REWARD_EVENT// 작업날짜: 2013-07-02	// 박세훈
						  , IN const bool isPcBang
#endif // SERV_PCBANG_USER_REWARD_EVENT

#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP	// 작업날짜: 2014-01-08 // 김현철 // 특정 캐릭터와 플레이시 경험치 증가 이벤트가 적용 될 때 다른 경험치 증가가 적용되지 않는 문제 수정
						  , IN const bool bHasPlayWithSpecificCharacterBuff_
#endif // SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP


						  );
#endif SERV_INTEGRATION
	//}}

	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장	
#ifdef SERV_PC_BANG_DROP_EVENT
	//{{ 2012. 03. 28	박세훈	드롭률 설정 이벤트 개편 Merge 작업 ( 2012.03.20 lygan_조성욱 )
#ifdef DROPEVENT_RENEWAL
	bool	CheckItemDropProbEvent(IN int iDungeonID, IN std::vector<char> vecUintClass, IN std::vector<char> vecUintType, IN std::vector<int> vecUintLevel, OUT int& iDropCount, OUT bool& bWithPlayPcBang );	// 드롭률 이벤트
#else //DROPEVENT_RENEWAL

#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	bool	CheckItemDropProbEvent( OUT float& fDropRate, OUT bool& bWithPlayPcBang );	// 드롭률 이벤트
#else // SERV_DROP_EVENT_RENEWAL
	bool	CheckItemDropProbEvent( OUT int& iDropCount, OUT bool& bWithPlayPcBang );	// 드롭률 이벤트
#endif // SERV_DROP_EVENT_RENEWAL

#endif //DROPEVENT_RENEWAL
#endif SERV_PC_BANG_DROP_EVENT
	//}}

	//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	bool	CheckItemDropProbEvent( OUT float& fDropRate );	// 드롭률 이벤트
#else // SERV_DROP_EVENT_RENEWAL
	bool	CheckItemDropProbEvent( OUT int& iDropCount );	// 드롭률 이벤트
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
	//}}

	bool	CheckTournamentEvent();														// 대회 이벤트
	bool	CheckSpiritEvent();															// 근성도 무제한 이벤트
	bool	CheckSecretDungeonEvent( int iDungeonID, int iDungeonMode );				// 비밀던전 무료 입장 이벤트

	//{{ 2010. 07. 16  최육사	이벤트 몬스터 시스템
#ifdef SERV_EVENT_MONSTER
	void	CheckEventMonster( OUT std::vector< int >& vecEventID );
#endif SERV_EVENT_MONSTER
	//}}
	//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	void	CheckEventMonster( OUT std::set<int>& setEventID );
#endif SERV_FIELD_EVENT_MONSTER
	//}}

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST

	//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST

	void	CheckEnableEventQuest( IN OUT std::map< int, KEventQuestInfo >& mapEventQuest, 
								   IN const std::map< int, KCompleteQuestInfo >& mapCompeletedQuest,
								   OUT std::vector< int >& vecBeginQuest, 
								   OUT std::vector< int >& vecEndQuest
								   );

#else

	void	CheckEnableEventQuest( IN OUT std::map< int, KEventQuestInfo >& mapEventQuest, 
								   IN const std::map< int, int >& mapCompeletedQuest,
								   OUT std::vector< int >& vecBeginQuest, 
								   OUT std::vector< int >& vecEndQuest
								   );

#endif SERV_DAILY_QUEST
	//}}	

#endif SERV_DAY_QUEST
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////
	// 접속 시간 이벤트 초기화 함수
	void	InitTimeEventForCharInit( IN const std::vector< KConnectTimeEventInfo >& vecEventTime, 
									  IN const std::vector< KCumulativeTimeEventInfo >& vecCumulativeEventTime,
									  OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime,
									  OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime,
									  OUT std::set< int >& setCompletedEvent 
									  );

	void	GetEventTimerInfo( IN const std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime, OUT std::map< int, double >& mapEventTimer );	// 이벤트 타이머 정보 얻기
	void	SetEventTimerInfo( OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime, IN const std::map< int, double >& mapEventTimer );	// 이벤트 타이머 정보 저장

	//////////////////////////////////////////////////////////////////////////	
	// 접속 시간 이벤트
	//{{ 2011. 05. 31	최육사	접속 이벤트 레벨 체크
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
	bool	GetConnectTimeEventReward( IN const u_char ucLevel, IN const int iEventUID, OUT int& iRewardID );
#else
	bool	GetConnectTimeEventReward( IN int iEventUID, OUT int& iRewardID );
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
	//}}

	//{{ 2012. 10. 13	박세훈	접속 이벤트 인자 정리
#ifdef SERV_CONNECT_EVENT_FACTOR_SET
	//{{ 2008. 6. 19  최육사  개선된 아이템 이벤트
	void	CheckEnableConnectTimeEventForCharInit( IN OUT ConnectEventFactorSet& kFactor );
	void	CheckEnableConnectTimeEvent( IN OUT ConnectEventFactorSet& kFactor );
	void	CheckConnectTimeEvent( IN OUT ConnectEventFactorSet& kFactor );
#else
	//{{ 2008. 6. 19  최육사  개선된 아이템 이벤트
	void	CheckEnableConnectTimeEventForCharInit( IN const u_char ucLevel,
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
													IN OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapEventTime,
													IN OUT std::set< int >& setCompletedEvent
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

	void	CheckEnableConnectTimeEvent( IN const u_char ucLevel,
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
								  		 IN OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapEventTime,
										 IN OUT std::set< int >& setCompletedEvent,
								  		 OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
								  		 OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent,
								  		 IN const bool bDelFinishEvent
								   //{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
								   , IN const UidType& iUnitUID
								   , IN const std::multimap<int, KDailyGiftBoxInfo>& mmapDailyGiftBoxList
#endif SERV_EVENT_DAILY_GIFT_BOX
								   //}}
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
								   , IN const int iEventItemID
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
								   );

	void	CheckConnectTimeEvent( IN const u_char ucLevel,
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
								   IN OUT std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapEventTime,
								   IN OUT std::set< int >& setCompletedEvent,
								   OUT std::vector< KConnectTimeEventInfo >& vecUpdatedTime,
								   OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
								   OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent
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

	//}}
	//////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	bool	GetCumulativeTimeEventReward( IN const int iEventUID, OUT int& iRewardID );

	void	CheckEnableCumulativeTimeEventForCharInit( IN const u_char ucLevel,
													   IN const bool bIsPcBang, 
													   //{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													   IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													   //}}
													   //{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
													   IN ConnectEventFactorSet& kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
													   //}}
													   IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapEventTime,
													   IN OUT std::set< int >& setCompleteEvent
													   );

	void	CheckEnableAndCompleteCumulativeTimeEvent( IN const u_char ucLevel,
													   IN const bool bIsPcBang, 
													   //{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													   IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
													   //}}
													   //{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
													   IN ConnectEventFactorSet& kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
													   //}}
													   IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapEventTime,
													   IN OUT std::set< int >& setCompleteEvent,
													   OUT std::vector< KUserCumulativeTimeEventInfo >& vecCompleteEventInfo,
													   OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
													   OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent
													   );

	void	CheckCumulativeTimeEvent( IN const u_char ucLevel,
									  IN const bool bIsPcBang, 
									  //{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
									  IN const int iChannelCode,
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
									  //}}
									  //{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
									  IN ConnectEventFactorSet& kFactor,
#endif SERV_2012_WINTER_VACATION_EVENT
									  //}}
									  IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapEventTime,
									  IN OUT std::set< int >& setCompleteEvent,
									  IN OUT std::set< int >& setDirectRewardEvent,
									  OUT std::vector< KCumulativeTimeEventInfo >& vecUpdatedTime,
									  OUT std::vector< KTimeEventSimpleInfo >& vecBeginEvent,
									  OUT std::vector< KTimeEventSimpleInfo >& vecEndEvent
									  );

	void	GetDBUpdateInfo( IN const std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime, 
							 IN const std::set< int >& setCompletedEvent,
							 OUT std::vector< KCumulativeTimeEventInfo >& vecEventTime );

	void	GetNowProcessingTimeEvent( IN const std::map< int, std::pair< KUserConnectTimeEventInfo, KTimerManager > >& mapConnectEventTime,
									   IN const std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime,
									   IN const std::set< int >& setCompletedEvent,
									   OUT std::vector< KTimeEventSimpleInfo >& vecTimeEvent,
									   IN bool bAccountTimeEvent );

	void	DeleteDirectRewardAndGetDBUpdateInfo( IN OUT std::map< int, std::pair< KUserCumulativeTimeEventInfo, KTimerManager > >& mapCumulativeEventTime,
												  IN OUT std::set< int >& setCompletedEvent,
												  IN OUT std::set< int >& setDirectRewardEvent,
												  OUT std::vector< KCumulativeTimeEventInfo >& vecCompleteEvent );

#endif CUMULATIVE_TIME_EVENT
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	void	RefreshEventScript();
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}

	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	void	RefreshEventData( void );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	void	AddHenirRewardCountEvent( IN const KEventInfo& kInfo );

#ifdef SERV_HENIR_REWARD_EVENT// 작업날짜: 2013-09-09	// 박세훈
	void	GetHenirRewardCountEvent( IN const bool bPcBangUser
		, OUT bool& bUnLimited
		, OUT int& iEventMax
		);
#else // SERV_HENIR_REWARD_EVENT
	bool	GetHenirRewardCountEvent( OUT bool& bUnLimited, OUT int& iEventMax  );
#endif // SERV_HENIR_REWARD_EVENT

#endif SERV_NEW_HENIR_TEST
	//}}

#ifdef SERV_ADVERTISEMENT_EVENT
	void	CheckEnableAdvertisementEvent( IN OUT std::set< int >& setAdvertisementEvent, OUT std::vector< KTimeEventSimpleInfo >& wstrBeginEventURL, OUT std::vector< KTimeEventSimpleInfo >& wstrEndEventURL );
	void	AddAdvertisementEvent( IN const KEventInfo& kInfo );
	void	GetAdvertisementURL( OUT std::vector< std::wstring >& vecAdvertisementURL );
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		bool TickCheckEvent();
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
		bool TickCheckEvent();
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	void InitEventReleaseTick();
	int GetEventReleaseTick() { return m_iEventReleaseTick; }
	void SetEventReleaseTick( int iReleaseTick ) { m_iEventReleaseTick = iReleaseTick; }
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK

#ifdef SERV_ADD_EVENT_DB
	void InitEventDBScriptReleaseTick();
	int GetEventDBScriptReleaseTick() { return m_iEventDBScriptReleaseTick; }
	void SetEventDBScriptReleaseTick( int iReleaseTick ) { m_iEventDBScriptReleaseTick = iReleaseTick; }
#endif //SERV_ADD_EVENT_DB

#ifdef SERV_NEW_EVENT_TYPES
	int		GetMaxLevel();
	bool	IsEnableDungeon( int nDunegonID, bool bEventDungeon );
	void	CheckEnableEventDungeon( IN OUT std::map< int, bool >& mapCheckDungeon );
	int		GetAppliedEventTick() { return m_iAppliedEventTick; }
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
	bool	IsEnableCode( IN int iEventID );
	void	CheckEnableCode( OUT std::map< int, bool >& mapCheckCode );
#endif SERV_CODE_EVENT

	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	void	AddQuestItemDropEvent( IN const KEventInfo& kInfo );
	bool	CheckQuestItemDropProbEvent( OUT float& fRate );
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}

#ifdef SERV_ALLOW_EVENT_ERASE 
	bool	IsEnableEventQuest( IN int iQuestID ); // 2013.06.10 lygan_조성욱 // DB에서 값 on/off 부분 추가 작업 // 국내 필드 들어가고 퀘스트 자동 수락 들어가면서 해당 기능 추가
#endif //SERV_ALLOW_EVENT_ERASE

#ifdef SERV_CHECK_TIME_QUEST
	void GetWarningEventQuest( OUT KEGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT& kPacket_ );
#endif // SERV_CHECK_TIME_QUEST

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
	void	AddDefenseDungeonOpenRateEvent( IN const KEventInfo& kInfo );
	bool	GetDefenseDungeonOpenRate( OUT float& fDefenseDungeonOpenRate ) const;
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef SERV_LEVEL_UP_EVENT// 작업날짜: 2013-06-24	// 박세훈
	void	AddLevelUpEvent( IN const KEventInfo& kInfo );
	void	GetLevelUpEvent( IN const unsigned char ucBeforeLevel
#ifdef SERV_LEVEL_UP_EVENT_CRITERION_DATE// 작업날짜: 2013-06-26	// 박세훈
						   , IN const byte byteCriterionDateEventInfo
						   , IN const std::wstring& wstrRegDate
#endif // SERV_LEVEL_UP_EVENT_CRITERION_DATE
						   , OUT std::vector<int>& vecRewardID
						   ) const;
#endif // SERV_LEVEL_UP_EVENT

#ifdef SERV_CRITERION_DATE_EVENT// 작업날짜: 2013-06-26	// 박세훈
	bool	CheckCriterionUserEvent( IN const byte byteCriterionDateEventInfo
								   , IN const std::wstring& wstrRegDate
								   , IN const int iScriptID
								   ) const;

	bool	CheckCriterionUserEvent( IN const byte byteCriterionDateEventInfo
								   , IN const std::wstring& wstrRegDate
								   , IN const KGameEventScriptManager::EVENT_DATA* pEventData
								   ) const;
#endif // SERV_CRITERION_DATE_EVENT

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-09	// 박세훈
	void	AddJumpingCharacterEvent( IN const KEventInfo& kInfo );
	bool	GetJumpingCharacterEvent( IN const char cUnitType
		, IN const int iLevel
		, OUT CTime& tBeginDate
		, OUT CTime& tEndDate
		, OUT int& iTargetLevel
		, OUT bool& bAccountEvent
		, OUT CTime& tCurrentDate
		) const;
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	void	SetTotalEventData( IN const std::map< int,  EVENT_DATA > mapEventScriptData, IN const std::map< int,  EVENT_DATA > mapEventDBData );
	const EVENT_DATA* GetTotalEventData( IN int iEventID ) const;
	bool	CheckMapIngEventDataEmpty();
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_GLOBAL_EVENT_TABLE
	void CheckEnableCodeAndSetGlobalEventdata( OUT std::map< int, KGlobalEventTableData >& mapCheckCodeAndData );
	void CheckInGameDisableCodeEvent( IN std::map< int, KGlobalEventTableData >& mapCheckEnableCodeAndData, OUT std::map< int, KGlobalEventTableData >& mapCheckDisableCodeAndData );
#endif //SERV_GLOBAL_EVENT_TABLE

protected:
	bool	RefreshEventTime( IN const KEventInfo& kInfo, OUT KGameEvent* pkGameEvent );
	bool	SetPeriodEventTime( IN const KEventInfo& kInfo, OUT CTime& ctBegin, OUT CTime& ctEnd );

	//## DB에서 이벤트 데이터를 받아 셋팅하는 함수들..
	void	AddQuestEvent( IN const KEventInfo& kInfo );
	void	AddItemDropEvent( IN const KEventInfo& kInfo );
	void	AddRewardEvent( IN const KEventInfo& kInfo );
	//{{ 2010. 07. 16  최육사	이벤트 몬스터 시스템
#ifdef SERV_EVENT_MONSTER
	void	AddMonsterEvent( IN const KEventInfo& kInfo );
#endif SERV_EVENT_MONSTER
	//}}
	void	AddTournamentEvent( IN const KEventInfo& kInfo );
	void	AddSpiritEvent( IN const KEventInfo& kInfo );
	void	AddConnectTimeEvent( IN const KEventInfo& kInfo );
	void	AddSecretDungeonEvent( IN const KEventInfo& kInfo );
	void	AddNotifyMSGEvent( IN const KEventInfo& kInfo );
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	void	AddCumulativeTimeEvent( IN const KEventInfo& kInfo );
#endif CUMULATIVE_TIME_EVENT
	//}}

#ifdef SERV_NEW_EVENT_TYPES
	void	AddDungeonEvent( IN const KEventInfo& kInfo );
	void	AddMaxLevelEvent( IN const KEventInfo& kInfo );
	void	AddContents( IN const KEventInfo& kInfo );
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
	void	AddCodeEvent( IN const KEventInfo& kInfo );
#endif SERV_CODE_EVENT

	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	bool	CheckWinterVacationEvent( IN const int iEventUID, IN const int iScriptID, IN ConnectEventFactorSet& kFactor );
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}


protected:
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void	SendToLogDB( unsigned short usEventID ){ SendToLogDB( usEventID, char() ); }

#ifdef SERV_ADD_EVENT_DB
	template < class T > void SendToEventDB( unsigned short usEventID, const T& data );
	void	SendToEventDB( unsigned short usEventID ){ SendToEventDB( usEventID, char() ); }
#endif //SERV_ADD_EVENT_DB

protected:
	SERVER_TYPE						m_eServerType;

	std::map< int, KGameEvent* >	m_mapIngEventData[KGameEvent::GET_MAX];	// [EventUID, Event Struct]	
	boost::timer					m_TimerEnableCheck;						// 일정 시간마다 이벤트 진행 상태를 변경하기 위해..
	boost::timer					m_TimerRefreshEvent;					// 일정 시간마다 DB에서 이벤트를 받아오기 위해..	

#ifdef SERV_ADD_EVENT_DB
	boost::timer					m_tEventReleaseTickTimer;
	int								m_iEventDBScriptReleaseTick;
	int								m_iEventReleaseTick;
#else //SERV_ADD_EVENT_DB
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	boost::timer					m_tEventCheckTimer;
	int								m_iEventReleaseTick;
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK
#endif //SERV_ADD_EVENT_DB	

#ifdef SERV_NEW_EVENT_TYPES
	int								m_iAppliedEventTick;
#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	std::map< int, EVENT_DATA >				m_mapTotalEventData; // 2013.10.30 darkstarbt_조성욱 // 기존 Eventdata.lua와 DB 에 있는 데이터 통합해서 사용하는 용도
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
};

template < class T >
void KGameEventManager::SendToLogDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_LOG_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}

#ifdef SERV_ADD_EVENT_DB
template < class T >
void KGameEventManager::SendToEventDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PC_EVENT_DB, anTrace, usEventID, data );
	LIF( spEvent->m_kDestPerformer.AddUID( 0 ) );

	SiKDBLayer()->QueueingEvent( spEvent );
}
#endif //SERV_ADD_EVENT_DB

DefSingletonInline( KGameEventManager );
