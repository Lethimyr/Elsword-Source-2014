#pragma once


//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON


class KDefenceDungeonManager
{
public:
    KDefenceDungeonManager();
    virtual ~KDefenceDungeonManager();
	
	void	Clear();

	void	StartGame();
	void	EndGame();
	void	OnCloseRoom();

	// get
	bool	IsEnterDefenceDungeon() const						{ return m_bEnterDefenceDungeon; }
	void	SetOpenDefenceDungeon( IN const bool bVal )			{ m_bOpenDefenceDungeon = bVal; }
	int		GetDefenceDungeonPhase() const						{ return m_iDefenceDungeonPhase; }

	// process
	//{{ 2013. 04. 29	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	bool	DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const int iNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate, IN bool b100PerEventOn = false );
#else //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
	bool	DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const int iNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate );
#endif //SERV_NEW_DEFENCE_DUNGEON_ENTER_100_PERCENT_EVENT
#else
	bool	DecisionEnterDefenceDungeon( IN const int iDungeonID, IN const std::map< UidType, int >& mapUnitLevelList, IN const float fMultipleRate );
#endif SERV_MODIFY_DEFENCE_DUNGEON_ENTER_LOGIC_IN_SECRET_DUNGEON
	//}}	
	bool	IncreaseDefenceDungeonPhase( IN const int iNpcID );
		
protected:
	bool					m_bOpenDefenceDungeon;		// 어둠의 문이 열렸는지 여부
	bool					m_bEnterDefenceDungeon;		// 어둠의 문 던전에 진입결정이 났는지 여부	
	int						m_iDefenceDungeonPhase;
};


#endif SERV_NEW_DEFENCE_DUNGEON
//}}


