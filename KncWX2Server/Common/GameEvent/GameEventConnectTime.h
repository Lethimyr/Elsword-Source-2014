#pragma once

#include "ServerDefine.h"
#include "GameEvent.h"
//#include "ServerDefine.h"
//#include "CommonPacket.h"
//#include "TimerManager.h"

//{{ 2009. 10. 26  최육사	PC방이벤트
//struct SUserConnectTimeEventInfo
//{
//	KUserConnectTimeEventInfo	m_kInfo;
//	KTimerManager				m_kEventTimer;
//};
//}}

class KGameEventConnectTime : public KGameEvent
{
public:
	KGameEventConnectTime(void);
	virtual ~KGameEventConnectTime(void);
	
	void	SetEventTime( float fEventTime )		{ m_fEventTime = fEventTime; } // 이벤트 타임 단위는 분단위입니다.
	float	GetEventTime()							{ return m_fEventTime; }

	void	SetEventID( int iEventID )				{ m_iEventID = iEventID; }
	int		GetEventID()							{ return m_iEventID; }
	
	void	SetRewardID( int iRewardID )			{ m_iRewardID = iRewardID; }
	int		GetRewardID()							{ return m_iRewardID; }

	void	SetGiveOnce( bool bGiveOnce )			{ m_bGiveOnce = bGiveOnce; }
	bool	IsGiveOnce()							{ return m_bGiveOnce; }

	void	SetDelEndEvent( bool bDelEndEvent ) 	{ m_bDelEndEvent = bDelEndEvent; }
	bool	IsDelEndEvent()							{ return m_bDelEndEvent; }

	//{{ 2009. 10. 26  최육사	PC방이벤트
	void	SetPcBangEvent( bool bPcBangEvent ) 	{ m_bPcBangEvent = bPcBangEvent; }
	bool	IsPcBangEvent()							{ return m_bPcBangEvent; }
	//}}

	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
	void	SetAccountEvent( bool bAccountEvent )	{ m_bAccountEvent = bAccountEvent; }
	bool	IsAccountEvent()						{ return m_bAccountEvent; }
	//}}

	//{{ 2009. 12. 16  최육사	이벤트개편
	void	SetUnitLevel( int iUnitLevel )			{ m_iUnitLevel = iUnitLevel; }
	int		GetUnitLevel()							{ return m_iUnitLevel; }
	//}}
	//{{ 2011. 05. 31	최육사	접속 이벤트 레벨 체크
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
	bool	IsMinMaxReward() const					{ return ( m_mapMinMaxReward.empty() == false ); }
	int		GetMinMaxRewardSize() const				{ return static_cast<int>(m_mapMinMaxReward.size()); }
	void	SetMinMaxReward( const std::map< std::pair< int, int >, int >& mapMinMaxReward ) { m_mapMinMaxReward = mapMinMaxReward; }
	int		GetMinMaxRewardID( const int iLevel ) const;
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
	//}}
	//{{ 2011. 08. 09	최육사	투니 랜드 채널링 이벤트
#ifdef SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	void	SetChannelCode( int iChannelCode )		{ m_iChannelCode = iChannelCode; }
	int		GetChannelCode()						{ return m_iChannelCode; }
#endif SERV_TOONILAND_CHANNELING_CONNECT_EVENT
	//}}
	//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	void	SetBF_Team( int iTeam )				{ m_iBF_Team = iTeam; }
	int		GetBF_Team()						{ return m_iBF_Team; }
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}
	//{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
	void	SetUnitType( char cUnitType )				{ m_cUnitType = cUnitType; }
	char	GetUnitType()								{ return m_cUnitType; }
	void	SetUnitClass( char cUnitClass )				{ m_cUnitClass = cUnitClass; }
	char	GetUnitClass()								{ return m_cUnitClass; }
#endif SERV_CHAR_CONNECT_EVENT
	//}}
	//{{ 2012. 06. 29	김민성       접속 시간에 따라 아이템 반복 지급
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	void	SetRepeatEvent( bool bRepeat )				{ m_bRepeatEvent = bRepeat; }
	bool	GetRepeatEvent()							{ return m_bRepeatEvent; }
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	//}}

#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
	void	SetRepeatRewardEvent( bool bRepeatRewardEvent )	{ m_bRepeatRewardEvent = bRepeatRewardEvent; }
	bool	IsRepeatRewardEvent()							{ return m_bRepeatRewardEvent; }

	void	SetBeforeEventID( int iBeforeEventID ) { m_iBeforeEventID = iBeforeEventID; }
	int		GetBeforeEventID()								{ return m_iBeforeEventID; }
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
	void	SetEventItemID( int iEventItemID )				{ m_iEventItemID = iEventItemID; }
	int		GetEventItemID()							{ return m_iEventItemID; }
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT

#ifdef SERV_CUSTOM_CONNECT_EVENT
	void	SetCustomEventID( IN const int iCustomEventID ){	m_iCustomEventID = iCustomEventID;	}
	int		GetCustomEventID( void ) const	{ return m_iCustomEventID;	}
#endif SERV_CUSTOM_CONNECT_EVENT

	//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
	void	SetEventGroupID( IN const int iEventGroupID ){	m_iEventGroupID = iEventGroupID;	}
	int		GetEventGroupID( void ) const	{ return m_iEventGroupID;	}
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
	//}}

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
	void	SetCriterionUserType( IN const byte byteCriterionUserType ){	m_byteCriterionUserType = byteCriterionUserType;	}
	byte	GetCriterionUserType( void ) const	{ return m_byteCriterionUserType;	}

	void	SetCriterionPresentUserType( IN const byte byteCriterionPresentUserType ){	m_byteCriterionPresentUserType = byteCriterionPresentUserType;	}
	byte	GetCriterionPresentUserType( void ) const	{ return m_byteCriterionPresentUserType;	}

	void	SetCriterionDateBegin( IN const CTime tCriterionDateBegin ){	m_tCriterionDateBegin = tCriterionDateBegin;	}
	CTime	GetCriterionDateBegin( void ) const	{ return m_tCriterionDateBegin;	}

	void	SetCriterionDateEnd( IN const CTime tCriterionDateEnd ){	m_tCriterionDateEnd = tCriterionDateEnd;	}
	CTime	GetCriterionDateEnd( void ) const	{ return m_tCriterionDateEnd;	}
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
	void	SetOnlySteamUserEvent( bool bVal_ ) { m_bOnlySteamUserEvent = bVal_; }
	void	SetOnlyNotSteamUserEvent( bool bVal_ ) { m_bOnlyNotSteamUserEvent = bVal_; }
	bool	IsOnlySteamUserEvent() const { return m_bOnlySteamUserEvent; }
	bool	IsOnlyNotSteamUserEvent() const { return m_bOnlyNotSteamUserEvent; }	
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	void	SetNewUnitEvent( bool bNewUnitEvent )		{ m_bNewUnitEvent = bNewUnitEvent; }
	bool	IsNewUnitEvent()							{ return m_bNewUnitEvent; }

	void	SetNewUnitEvent2( bool bNewUnitEvent2 )		{ m_bNewUnitEvent2 = bNewUnitEvent2; }
	bool	IsNewUnitEvent2()							{ return m_bNewUnitEvent2; }

	void	SetCurrentUnitEvent( bool bCurrentUnitEvent )		{ m_bCurrentUnitEvent = bCurrentUnitEvent; }
	bool	IsCurrentUnitEvent()							{ return m_bCurrentUnitEvent; }
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
	void	SetUnitClassLevel( int iUnitClassLevel )	{ m_iUnitClassLevel = iUnitClassLevel; }
	int		GetUnitClassLevel()							{ return m_iUnitClassLevel; }
#endif SERV_UNIT_CLASS_LEVEL_EVENT

private:
	float	m_fEventTime;
	int		m_iEventID;
	int		m_iRewardID;
	bool	m_bGiveOnce;
	bool	m_bDelEndEvent;
	//{{ 2009. 10. 26  최육사	PC방이벤트
	bool	m_bPcBangEvent;
	//}}
	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
	bool	m_bAccountEvent;
	//}}
	//{{ 2009. 12. 16  최육사	이벤트개편
	int		m_iUnitLevel;
	//}}
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
	//{{ 2011. 05. 31	최육사	접속 이벤트 레벨 체크
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
	std::map< std::pair< int, int >, int > m_mapMinMaxReward;
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
	//}}
	//{{  2011.11.30     김민성    캐릭터별 접속 이벤트
#ifdef SERV_CHAR_CONNECT_EVENT
	char		m_cUnitType;
	char		m_cUnitClass;
#endif SERV_CHAR_CONNECT_EVENT
	//}}
	//{{ 2012. 06. 29	김민성       접속 시간에 따라 아이템 반복 지급
#ifdef SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	bool	m_bRepeatEvent;
#endif SERV_REPEAT_CONNECT__REWARD_ITEM_EVENT
	//}}
	//{{ 2012. 12. 25	박세훈	특정 유저 전용 접속 이벤트
#ifdef SERV_SPECIFIC_USER_CONNECT_EVENT
	int				m_iEventGroupID;
#endif SERV_SPECIFIC_USER_CONNECT_EVENT
	//}}

#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
	bool	m_bRepeatRewardEvent;
	int		m_iBeforeEventID;
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD

#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
	int			m_iEventItemID;
#endif SERV_ITEM_IN_INVENTORY_CONNECT_EVENT

#ifdef SERV_CUSTOM_CONNECT_EVENT
	int		m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
	char	m_byteCriterionUserType;
	char	m_byteCriterionPresentUserType;
	CTime	m_tCriterionDateBegin;
	CTime	m_tCriterionDateEnd;
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
	bool	m_bOnlySteamUserEvent;
	bool	m_bOnlyNotSteamUserEvent;
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	bool	m_bNewUnitEvent;
	bool	m_bNewUnitEvent2;
	bool	m_bCurrentUnitEvent;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
	int		m_iUnitClassLevel;
#endif SERV_UNIT_CLASS_LEVEL_EVENT
};



