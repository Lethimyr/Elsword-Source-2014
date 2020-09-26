#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLItem.h"


class CXSLSynthesisManager
{
	DeclareRefreshSingleton( CXSLSynthesisManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
public:
		
	struct RandomUnitData
	{
		char		m_cUnitClass;
		int			m_iItemGroupID;
	};

	//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
	typedef std::pair< int, int >	RandomItemKey;
	//}}

	struct RandomItemData
	{
		int			m_ItemID;
		bool		m_bGiveAll;
		bool		m_bUnitSelect;
		bool		m_bGiveCharacter;
		int			m_KeyItemID;
		int			m_RequiredKeyCount;
		int			m_iRessurectionCount;
		int			m_iRestoreSpirit;

		std::vector< RandomUnitData > m_vecUnitData;
	};

	struct ItemGroupData
	{
		int			m_iItemID;
		float		m_fRate;
		int			m_iPeriod;
		int			m_iQuantity;

		ItemGroupData()
		{
			m_iItemID	= 0;
			m_fRate		= 0.0f;
			m_iPeriod	= 0;
			m_iQuantity	= 0;
		}
	};

	struct ItemGroupDataList
	{
		int			m_iGroupID;
		KLottery	m_kLottery;
		std::vector< ItemGroupData > m_vecItemData;

		ItemGroupDataList(){};
		ItemGroupDataList( const ItemGroupDataList& t ) { *this = t; }    // copy constructor
		ItemGroupDataList& operator=( const ItemGroupDataList& t )        // assign operator
		{
			m_iGroupID = t.m_iGroupID;
			m_kLottery = t.m_kLottery;
			m_vecItemData = t.m_vecItemData;
			return *this;
		}
	};


public:
	CXSLSynthesisManager(void);
	~CXSLSynthesisManager(void);

	bool	AddAvatarSocketOptionForEquipPosition_LUA( int iEquipPosition, int iSocketGroupID );	//SERV_CASH_ITEM_SOCKET_OPTION
	bool	AddSynthesisGroup_LUA( int iGroupID, int iItemID , float fRate);

	std::map< int, int > GetMapAvatarSocketOptionGroupID(){ return m_mapAvatarSocketOptionGroupID; };
	int GetAvatarSocketOptionGroupID(int iEquipPosition);

	const RandomItemData* GetSynthesisItemData( IN int iRandomItemID, IN int iKeyItemID ) const;

	bool	GetSynthesisResultItem( IN int iUnitType, IN int iEqipPosition , IN int iSocketNo ,  OUT std::map< int, KItemInfo >& mapResultItem , OUT std::set< int > &setSealCashItem ) const;

private:

	std::map< int, int >			m_mapAvatarSocketOptionGroupID;	// <EquipPos, SocketOptionGroupID>

	std::map< RandomItemKey, RandomItemData >	m_mapRandomItem;
	std::map< int, ItemGroupDataList >			m_mapItemGroup;

};

DefRefreshSingletonInline( CXSLSynthesisManager );



