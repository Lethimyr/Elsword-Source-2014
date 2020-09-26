#pragma once

#include "RefreshSingleton.h"
#include <set>
#include <map>
#include "Lottery.h"
#include "GameEventMonster.h"
#include "CommonPacket.h"



class KGameEventScriptManager
{
	DeclareRefreshSingleton( KGameEventScriptManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
public:
	struct KAdventNpcEventInfo
	{
		int					m_iEventID;
		int					m_iMonsterDropCount;
		bool				m_bIsAdventInBossStage;
		KLottery			m_kAdventNpcLot;
		std::set< int >		m_setAdventDungeon;
		std::set< int >		m_setNoAdventDungeon;
		std::set< int >		m_setNoAdventStage;
		//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
		bool				m_bAllyNPC;
#endif SERV_ALLY_EVENT_MONSTER
		//}}
		//{{ 2012. 12. 31	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
		int						m_iAddLevel;
		bool					m_bDrop;
		bool					m_bAggressive;
		float					m_fAddPosY;
		std::set<int>			m_setAdventField;
		std::set<int>			m_setNoAdventField;
#endif SERV_FIELD_EVENT_MONSTER
		//}}
#ifdef SERV_ACTIVE_EVENT_MONSTER
		std::map<int, bool>		m_mapMonsterActive;
#endif SERV_ACTIVE_EVENT_MONSTER
		KAdventNpcEventInfo()
			: m_iEventID( 0 )
			, m_iMonsterDropCount( 0 )
			, m_bIsAdventInBossStage( false )
			//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
			, m_bAllyNPC( false )
#endif SERV_ALLY_EVENT_MONSTER
			//}}
			//{{ 2012. 12. 31	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
			, m_iAddLevel( 0 )
			, m_bDrop( false )
			, m_bAggressive( true )
			, m_fAddPosY( 0.0f )
#endif SERV_FIELD_EVENT_MONSTER
			//}}
		{
		}
	};

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	struct EVENT_DATA
	{
		float	m_fEXPRate;
		float	m_fVPRate;
		float	m_fEDRate;
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
		float	m_fRankingPointRate;
#endif	// SERV_RANKING_POINT_EVENT

		int		m_iDungeonID;
		int		m_iDungeonMode;
		int		m_iUnitLevel;		
		char	m_cUnitType;
		char    m_cUnitClass;
		int		m_iPartyNum;

		// 아이템 이벤트
		float	m_fEventTime;
		int		m_iEventReward;
		//{{ 2011. 05. 31	최육사	접속 이벤트 레벨 체크
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
		std::map< std::pair< int, int >, int >	m_mapMinMaxReward;
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
		//}}
		bool	m_bGiveOnce;
		bool	m_bDelEndEvent;
		bool	m_bPcBangEvent;
		bool	m_bDirectReward;
		bool	m_bAccountEvent;
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		float	m_fDropRate;
#else // SERV_DROP_EVENT_RENEWAL
		int		m_iDropCount;
#endif // SERV_DROP_EVENT_RENEWAL
		bool	m_bWithPlayPcBang;
		//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		int		m_iChannelCode;
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
		//}}
		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		int		m_iBF_Team;
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		int		m_iHenirRewardEventCount;
		bool	m_bHenirRewardUnLimited;
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		int		m_iServerGroup;
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}
		//{{ 2012. 06. 20	김민성       이벤트 경험치 중첩 안됨 조건 추가
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
		bool	m_bAccumulateEXP;
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
		//}}
		//{{ 2012. 03. 28	박세훈	드롭률 설정 이벤트 개편 Merge 작업 ( 2012.03.20 lygan_조성욱 )
#ifdef DROPEVENT_RENEWAL
		int		m_iStartLevel;
		int		m_iEndLevel;
#endif DROPEVENT_RENEWAL
		//}}
		//{{ 2012. 06. 29	김민성       접속 시간에 따라 아이템 반복 지급
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		bool	m_bRepeatEvent;
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
		//}}

#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
		bool	m_bRepeatRewardEvent;
		int		m_iBeforeEventID;
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

		//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		bool	m_bPlayWithCharEXP;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}
		//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
		int		m_iEventGroupID;
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
		//}}
		//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
		float	m_fQuestItemDropRate;
#endif SERV_QUEST_ITEM_DROP_EVENT
		//}

#ifdef SERV_ADVERTISEMENT_EVENT
		std::wstring	m_wstrAdvertisementURL;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		int		m_iEventItemID;
#endif 

#ifdef SERV_CUSTOM_CONNECT_EVENT
		int				m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
		float	m_fDefenseDungeonOpenRate;
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
		byte	m_byteCriterionUserType;
		byte	m_byteCriterionPresentUserType;
		CTime	m_tCriterionDateBegin;
		CTime	m_tCriterionDateEnd;
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
		bool			m_bOnlySteamUser;
		bool			m_bOnlyNotSteamUser;
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		bool	m_bNewUnitEvent;
		bool	m_bNewUnitEvent2;
		bool	m_bCurrentUnitEvent;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		int		m_iUnitClassLevel;
#endif SERV_UNIT_CLASS_LEVEL_EVENT

#ifdef SERV_HENIR_REWARD_EVENT// 작업날짜: 2013-09-09	// 박세훈
		bool	m_bAccumulate;
#endif // SERV_HENIR_REWARD_EVENT

		EVENT_DATA()
			: m_fEXPRate( 0.f )
			, m_fVPRate( 0.f )
			, m_fEDRate( 0.f )
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
			, m_fRankingPointRate( 0.0f )
#endif	// SERV_RANKING_POINT_EVENT
			, m_iDungeonID( 0 )
			, m_iDungeonMode( 0 )
			, m_iUnitLevel( 0 )
			, m_cUnitType( 0 )
			, m_cUnitClass( 0 )
			, m_iPartyNum( 0 )
			, m_fEventTime( 0.f )
			, m_iEventReward( 0 )
			, m_bGiveOnce( true )
			, m_bDelEndEvent( false )
			, m_bPcBangEvent( false )
			, m_bDirectReward( false )
			, m_bAccountEvent( false )
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
			, m_fDropRate( 0.0f )
#else // SERV_DROP_EVENT_RENEWAL
			, m_iDropCount( 0 )
#endif // SERV_DROP_EVENT_RENEWAL
			, m_bWithPlayPcBang( false )
			//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			, m_iChannelCode( 0 )
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
			//}}
			//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			, m_iBF_Team( 0 )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
			//}}
			//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
			, m_iHenirRewardEventCount( 0 )
			, m_bHenirRewardUnLimited( false )
#endif SERV_NEW_HENIR_TEST
			//}}
			//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
			, m_iServerGroup( -1 )
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
			//}}
			//{{ 2012. 06. 20	김민성       이벤트 경험치 중첩 안됨 조건 추가
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
			, m_bAccumulateEXP( false )
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
			//}}
			//{{ 2012. 03. 28	박세훈	드롭률 설정 이벤트 개편 Merge 작업 ( 2012.03.20 lygan_조성욱 )
#ifdef DROPEVENT_RENEWAL
			, m_iStartLevel( 0 )
			, m_iEndLevel( 0 )
#endif DROPEVENT_RENEWAL
			//}}
			//{{ 2012. 06. 29	김민성       접속 시간에 따라 아이템 반복 지급
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
			, m_bRepeatEvent( false )
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
			//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
			, m_bRepeatRewardEvent( false )
			, m_iBeforeEventID( -1 )			
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD			

			//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			, m_bPlayWithCharEXP( false )
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			//}}
			//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
			, m_iEventGroupID( -1 )
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
			//}}
			//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
			, m_fQuestItemDropRate( 1.f )
#endif SERV_QUEST_ITEM_DROP_EVENT
		//}

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
			, m_iEventItemID( 0 )
#endif 
#ifdef SERV_CUSTOM_CONNECT_EVENT
			, m_iCustomEventID( 0 )
#endif //SERV_CUSTOM_CONNECT_EVENT		
		
#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
			, m_fDefenseDungeonOpenRate( 1.0f )
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
			, m_byteCriterionUserType( 0 )
			, m_byteCriterionPresentUserType( 0 )
#endif	// SERV_CRITERION_DATE_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
			, m_bOnlySteamUser( false )
			, m_bOnlyNotSteamUser( false )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
			, m_bNewUnitEvent( false )
			, m_bNewUnitEvent2( false )
			, m_bCurrentUnitEvent( false )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
			, m_iUnitClassLevel( -1 )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
#ifdef SERV_HENIR_REWARD_EVENT// 작업날짜: 2013-09-09	// 박세훈
			, m_bAccumulate( false )
#endif // SERV_HENIR_REWARD_EVENT
		{
		}
	};
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

public:
	KGameEventScriptManager(void);
	~KGameEventScriptManager(void);

	// for lua
	bool AddEventMonsterInfo_LUA();
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	void AddReward_LUA( IN int iID );
	//{{ 2008. 7. 11  최육사  이벤트 공지
	void AddNotifyMSGServerInfo_LUA( int iServerGroupID, int iServerUID );
	//}}
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
	void AddConnectEventUserGroup_LUA( int iEventGroupID, UidType iUserUID );
	bool CheckEventGroup( IN const int iEventGroupID, IN const UidType iUserUID ) const;
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
	//}}

#ifdef SERV_ADVERTISEMENT_EVENT
	void AddAdvertisement_LUA( IN int iID );
	const EVENT_DATA* GetAdvertisementData( IN int iScriptID ) const;
#endif SERV_ADVERTISEMENT_EVENT

	// function
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	void Init();
	bool IsNotifyMSGEvent() const { return m_bNotifyMSGEvent; }
	const EVENT_DATA* GetEventData( IN int iScriptID ) const;
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	bool IsExistEvent( IN int iEventID )	{ return ( m_mapEventMonster.find( iEventID ) != m_mapEventMonster.end() ); }
	void GetAdventNpcEventInfo( IN const std::vector< int >& vecEventID, OUT std::vector< KAdventNpcEventInfo >& vecInfo );

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	const std::map< int, EVENT_DATA >&	GetMapEventScriptData() const { return m_mapEventData; }
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	///스크립트에서 정보 받을 함수
	void AddCoboEventData_LUA(const char* szChangeEventDate, const char* szWeekEndStartDate_One, const char* szWeekEndEndDate_One,const char* szWeekEndStartDate_Two, const char* szWeekEndEndDate_Two, const char* szWeekNextDay,int remaind );
	std::vector<CTime> GetCoboEventData(void);
	int GetRemainTime(void);
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	void	AddValenTineEventData_LUA( int iTemp_1, int iTemp_2, int iTemp_3, const char* szChangeEventDate, int iItemID,int iItemNum_1,int iItemNum_2,int iItemNum_3, int iItemGetCount );
	int		GetBeginnerTime(void);
	int		GetIntermediateTime(void);
	int		GetExpertTime(void);
	CTime	GetNextDayTime(void);
	int		GetValenItemID(void);
	int     GetValenItemNum_1(void);
	int     GetValenItemNum_2(void);
	int     GetValenItemNum_3(void);
	int     GetValenTinePlayCount(void);
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
private:
	std::map< int, KAdventNpcEventInfo >	m_mapEventMonster;
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	std::map< int, EVENT_DATA >				m_mapEventData;

	//{{ 2008. 7. 11  최육사	공지 이벤트
	UidType									m_iNotifyCnServerUID;
	bool									m_bNotifyMSGEvent;
	//}}
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
	std::map< int, std::set<UidType> >		m_mapEventUserGroup;
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
	//}}
#ifdef SERV_ADVERTISEMENT_EVENT
	std::map< int, EVENT_DATA >				m_mapAdvertisementData;
#endif SERV_ADVERTISEMENT_EVENT
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	int m_iTemp_1;
	int m_iTemp_2;
	int m_iTemp_3;
	CTime m_cTimeNextDay;
	int m_iItemID;
	int m_iItemNum_1;
	int m_iItemNum_2;
	int m_iItemNum_3;
	int m_iItemGetCount;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
};

DefRefreshSingletonInline( KGameEventScriptManager );


