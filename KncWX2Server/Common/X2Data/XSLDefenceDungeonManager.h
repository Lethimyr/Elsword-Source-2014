#pragma once

#include "RefreshSingleton.h"
#include "X2Data/XSLMain.h"
#include <map>
#include "Lottery.h"


//{{ 2013. 04. 12	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON


class CXSLDefenceDungeonManager
{
	DeclareRefreshSingleton( CXSLDefenceDungeonManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum DEFENCE_DUNGEON_PHASE
	{
		DDP_INIT = 0,
		DDP_PHASE_1,
		DDP_PHASE_2,
		DDP_PHASE_3,

		DDP_PHASE_END,
	};

	struct SEnterLevelConditionInfo
	{
		int					m_iLevel;
		float				m_fRate;

		SEnterLevelConditionInfo()
		{
			m_iLevel = 0;
			m_fRate = 0.f;
		}
	};	

	struct SManufactureItemInfo
	{
		std::vector< int >	m_vecSocketOptionGroup;
	};

	struct SSpecialCardGroupInfo
	{
		std::map< int, int > m_mapSpecialCard[DDP_PHASE_END];		// key : SocketOptionID, value : SpecialCard ItemID
	};

	struct SQuickSlotItemGroupInfo
	{
		KLotterySameProb< int >	m_kLotQuickSlotItemGroup[DDP_PHASE_END];
	};

public:
	CXSLDefenceDungeonManager(void);
	~CXSLDefenceDungeonManager(void);

	// for lua
	void	SetNotOpenGateMultiply_LUA( float fMultiplyValue );
	void	AddEnterLevelCondition_LUA( int iLevel, float fRate );
	void	SetDefenceDungeonQuickSlotPotion_LUA( int iItemID );
	void	AddNotBeginDefenceDungeonID_LUA( int iDungeonID );
	void	AddDefenceGuardianMonster_LUA( int iNpcID );
	void	AddDefenceDungeonQuickSlotItem_LUA( int iGroupID, int iItemIDPhase1, int iItemIDPhase2, int iItemIDPhase3 );
	void	AddSocketOptionRandomGroup_LUA( int iGroupID, int iSocketOptionID, float fRate );
	bool	AddItemInfoForManufacture_LUA();

	// function
	bool	IsDefenceGuardianMonster( IN const int iNpcID ) const			{ return ( m_setDefenceGuardianMonster.find( iNpcID ) != m_setDefenceGuardianMonster.end() ); }
	bool	IsNotBeginDefenceDungeonID( IN const int iDungeonID ) const		{ return ( m_setNotBeginDefenceDungeonID.find( iDungeonID ) != m_setNotBeginDefenceDungeonID.end() ); }
	bool	IsManufactureDefenceDungeonItem( IN const int iItemID ) const	{ return ( m_mapManufactureItem.find( iItemID ) != m_mapManufactureItem.end() ); }
	bool	InsertSocketOptionForDefenceDungeonItem( IN const int iItemID, OUT std::vector< int >& vecSocketOption );
	
	float	GetNotOpenGateMultiplyValue() const								{ return m_fNotOpenGateMultiplyValue; }
	int		GetQuickSlotPotionItemID() const								{ return m_iQuickSlotPotionItemID; }
	bool	GetResultRandomSocketOption( IN const int iGroupID, IN const std::vector< int >& vecNotIncludeOptionID, OUT int& iSocketOptionID ) const;
	bool	GetResultSpecialCardByDefenceDungeonItem( IN const int iDefenceDungeonPhase, IN const int iItemID, IN const std::vector< int >& vecSocketOption, OUT int& iSpecialCardItemID ) const;
	void	GetResultQuickSlotItem( IN const int iDefenceDungeonPhase, OUT std::vector< int >& vecQuickSlotItemID ) const;
	float	GetEnterDefenceDungeonRate( IN const int iDungeonNpcLevel, IN const std::map< UidType, int >& mapUnitLevelList );

	//{{ 2013. 06. 05	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	void	AddOldDefenceItemID_LUA( int iItemID );
	bool	IsOldDefenceItemID( IN const int iItemID ) const				{ return ( m_setOldDefenceItem.find( iItemID ) != m_setOldDefenceItem.end() ); }
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

private:
	float											m_fNotOpenGateMultiplyValue;
	int												m_iQuickSlotPotionItemID;		// 네번째 퀵슬롯에 들어갈 포션 ItemID
	std::vector< SEnterLevelConditionInfo >			m_vecEnterLevelCondition;		// 입장 레벨별 확률 정보
	std::set< int >									m_setNotBeginDefenceDungeonID;	// 어둠의 문에서 제외되는 던전 리스트
	std::set< int >									m_setDefenceGuardianMonster;
	std::set< int >									m_setQuickSlotRandomGroupID;
	std::map< int, SQuickSlotItemGroupInfo >		m_mapQuickSlotItemGroup;		// key : GroupID, value : Random QuickSlotItem
	std::map< int, KLottery >						m_mapSocketOptRandomGroup;		// key : GroupID, value : Random SocketOption
	std::map< int, SManufactureItemInfo >			m_mapManufactureItem;			// key : ItemID, value : Random GroupID
	std::map< int, SSpecialCardGroupInfo >			m_mapSpecialCardGroup;			// key : ItemID, value : Special Card Group
	//{{ 2013. 06. 05	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::set< int >									m_setOldDefenceItem;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
};

DefRefreshSingletonInline( CXSLDefenceDungeonManager );


#endif SERV_NEW_DEFENCE_DUNGEON
//}}