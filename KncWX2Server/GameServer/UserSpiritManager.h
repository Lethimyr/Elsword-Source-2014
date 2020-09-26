#pragma once
#include "Event.h"
#include "CommonPacket.h"


//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KUserSpiritManager
{
public:
	KUserSpiritManager(void);
	~KUserSpiritManager(void);

	void	Clear();
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	void	Init( IN const int iSpirit, IN const int iSpiritMax, IN const int iAccumulationSpirit );
#else
	void	Init( IN const int iSpirit, IN const int iSpiritMax );	
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
    
	int		GetSpirit() const										{ return m_iSpirit; }
	void	SetSpirit( IN const int iSpirit )						{ m_iSpirit = iSpirit; }
	int		GetSpiritMax() const									{ return m_iSpiritMax; }
	void	SetSpiritMax( IN const int iSpiritMax )					{ m_iSpiritMax = iSpiritMax; }
	bool	IsSpiritUpdated() const									{ return m_bIsSpiritUpdated; }
	void	SetIsSpiritUpdated( IN const bool bIsSpiritUpdated )	{ m_bIsSpiritUpdated =  bIsSpiritUpdated; }

	int		RestoreSpirit( IN const int iRestoreSpirit );

	bool	CheckingSpirit( IN const int iDungeonID, IN const bool bIsPcBang ) const;
	int		CalculateDecreaseSpirit( IN const char cUserCount, 
									 IN const short sStageNpcCount, 
									 IN const bool bIsTutorial, 
									 IN const bool bHalfDecreaseEvent,
									 IN const bool bIsBattleField );

	bool	DecreaseSpirit( IN const int iDungeonID
						  , IN const char cUserCount
						  , IN const short sStageNpcCount
						  , IN const bool bIsTutorial
						  , IN const bool bIsPcBang
						  , IN const bool bIsBattleField
						  , OUT int& iDecreaseSpirit
						  , OUT bool& bIsSpiritUpdated
#if defined( SERV_LOCAL_RANKING_SYSTEM ) || defined( SERV_CHINA_SPIRIT_EVENT )
						  , OUT int& iAbsoluteDecreaseSpirit
#endif	// SERV_LOCAL_RANKING_SYSTEM
#ifdef	SERV_ACCUMULATION_SPIRIT_SYSTEM
						  , OUT bool& bReward
#ifdef SERV_BURNING_CHAR_EVENT_SUB_QUEST
						  ,IN const bool bCharQuest
#endif //SERV_BURNING_CHAR_EVENT_SUB_QUEST
#endif	SERV_ACCUMULATION_SPIRIT_SYSTEM
						  );

	void	GetDBUpdateInfo( OUT int& iSpirit, OUT bool& bIsSpiritUpdated );

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	void	SetAccumulationSpirit( IN const int iAccumulationSpirit )	{ m_iAccumulationSpirit = iAccumulationSpirit; }
	int		GetAccumulationSpirit() const								{ return m_iAccumulationSpirit; }
	void	CalculateAccumulationSpirit( IN char cUserCount, IN short sStageNpcCount, IN bool bIsTutorial, IN bool bHalfDecreaseEvent, IN bool bIsBattleField, OUT bool& bReward );
	void	GetDBUpdateInfo2( OUT int& iAccumulationSpirit );
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

private:
	int								m_iSpiritMax;
	int								m_iSpirit;
	bool							m_bIsSpiritUpdated;		// 근성도 하루치를 다 썼을경우 true
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	int								m_iAccumulationSpirit;
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}