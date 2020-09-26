#pragma once

#include "GameEvent.h"
#include "X2Data/XSLUnit.h"
#include "X2Data/XSLItem.h"
//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
//}}


class KGameEventReward : public KGameEvent
{
public:
	struct REWARD_DATA
	{
		float	m_fEXPRate;
		float	m_fVPRate;
		float	m_fEDRate;
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
		float	m_fRankingPointRate;
#endif	// SERV_RANKING_POINT_EVENT

		int		m_iDungeonID;
		int		m_iUnitLevel;
		char	m_cUnitType;
		char    m_cUnitClass;
		int		m_iPartyNum;

		// 아이템 이벤트
		float	m_fEventTime;
		int		m_iEventReward;
		bool	m_bGiveOnce;
		bool	m_bDelEndEvent;
		bool	m_bPcBangEvent;
		bool	m_bDirectReward;
		bool	m_bAccountEvent;
		int		m_iDropCount;
		bool	m_bWithPlayPcBang;
		//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		int		m_iServerGroupID;
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}
		//{{ 2012. 06. 20	김민성       이벤트 경험치 중첩 안됨 조건 추가
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
		bool	m_bAccumulateEXP;
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
		//}}
		//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		bool	m_bPlayWithCharEXP;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		int		m_iUnitClassLevel;
#endif SERV_UNIT_CLASS_LEVEL_EVENT

		REWARD_DATA()
		{
			m_fEXPRate			= 0.f;
			m_fVPRate			= 0.f;
			m_fEDRate			= 0.f;
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
			m_fRankingPointRate	= 0.0f;
#endif	// SERV_RANKING_POINT_EVENT
			m_iDungeonID		= 0;
			m_iUnitLevel		= 0;
			m_cUnitType			= CXSLItem::UT_NONE;
			m_cUnitClass		= CXSLItem::UC_NONE;
			m_iPartyNum			= 0;
			m_fEventTime		= 0.f;
			m_iEventReward		= 0;
			m_bGiveOnce			= false;
			m_bDelEndEvent		= false;
			m_bPcBangEvent		= false;
			m_bDirectReward		= false;
			m_bAccountEvent		= false;
			m_iDropCount		= 0;
			m_bWithPlayPcBang	= false;
			//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
#ifdef EXTEND_SERVER_GROUP_MASK
			m_iServerGroupID	= -1;
#else EXTEND_SERVER_GROUP_MASK
			m_iServerGroupID	= SEnum::SGI_INVALID;
#endif EXTEND_SERVER_GROUP_MASK
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
			//}}
			//{{ 2012. 06. 20	김민성       이벤트 경험치 중첩 안됨 조건 추가
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
			m_bAccumulateEXP	= true; // 기본 중첩 허용
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
			//}}
			//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			m_bPlayWithCharEXP	= false;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
			//}}
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
			m_iUnitClassLevel	= -1;
#endif SERV_UNIT_CLASS_LEVEL_EVENT
		}
	};

public:
	KGameEventReward();
	virtual ~KGameEventReward();

	//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	void	SetEXPRate( IN const float fEXPRate );
	void	SetVPRate( IN const float fVPRate );
	void	SetEDRate( IN const float fEDRate );
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
	void	SetRankingPointRate( IN const float fRankingPointRate );
#endif	// SERV_RANKING_POINT_EVENT
	void	SetDungeonID( IN const int iDungeonID );
	void	SetUnitLevel( IN const int iUnitLevel );
	void	SetUnitType( IN const char cUnitType );
	void	SetUnitClass( IN const char cUnitClass );
	void	SetPartyNum( IN const int iPartyNum );
	void	SetServerGroupID( IN const int iServerGroupID );
	void	Dump();
#else
	void	SetData( float fEXPRate, float fVPRate, float fEDRate, int iDungeonID, int iUnitLevel, char cUnitType, char cUnitClass, int iPartyNum );
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
	//}}	

	float	GetEXPRate() const;
	float	GetVPRate() const;
	float	GetEDRate() const;
#ifdef	SERV_RANKING_POINT_EVENT // 적용날짜: 2013-03-26
	float	GetRankingPointRate( void ) const;
#endif	// SERV_RANKING_POINT_EVENT
	int		GetDungeonID() const				{ return m_sData.m_iDungeonID; }
	int		GetUnitLevel() const				{ return m_sData.m_iUnitLevel; }
	char	GetUnitType() const					{ return m_sData.m_cUnitType; }
	char	GetUnitClass() const				{ return m_sData.m_cUnitClass; }
	int		GetPartyNum() const					{ return m_sData.m_iPartyNum; }
	//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	int		GetServerGroupID() const			{ return m_sData.m_iServerGroupID; }
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
	//}}

	//{{ 2012. 06. 20	김민성       이벤트 경험치 중첩 안됨 조건 추가
#ifdef SERV_NOT_ACCUMULATE_EVENT_EXP
 	void		SetAccumulateEXP( IN bool bAccumulateEXP ) 		{ m_sData.m_bAccumulateEXP = bAccumulateEXP; }
 	bool		IsAccumulateEXP() const							{ return m_sData.m_bAccumulateEXP; }
#endif SERV_NOT_ACCUMULATE_EVENT_EXP
	//}}
	//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	void		SetPlayWithCharEXP( IN const bool bVal )		{ m_sData.m_bPlayWithCharEXP = bVal; }
	bool		IsPlayWithCharEXP() const		{ return m_sData.m_bPlayWithCharEXP; }
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

#ifdef SERV_PCBANG_USER_REWARD_EVENT// 작업날짜: 2013-07-02	// 박세훈
	void	SetPcBangEvent( IN const bool bPcBangEvent ){	m_sData.m_bPcBangEvent = bPcBangEvent;	}
	bool	IsPcBangEvent( void ) const{	return m_sData.m_bPcBangEvent;	}
#endif // SERV_PCBANG_USER_REWARD_EVENT

protected:
    REWARD_DATA	m_sData;
};


