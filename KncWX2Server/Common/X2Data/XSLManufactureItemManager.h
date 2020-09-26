#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLItem.h"

class CXSLManufactureItemManager
{
	DeclareRefreshSingleton( CXSLManufactureItemManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:

	struct MaterialData
	{
		int			m_MaterialItemID;
		int			m_MaterialCount;
	};

	struct ManufactureData
	{
		int							m_ResultGroupID;
		int							m_Cost;

		std::vector< MaterialData >	m_vecMaterials;
		KLottery					m_kLotteryResultItem;
	};

public:
	CXSLManufactureItemManager(void);
	~CXSLManufactureItemManager(void);

	// for lua
	bool AddManufactureTemplet_LUA();
	bool AddManufactureResultTemplet_LUA();
	bool AddManufactureVillageData_LUA( int iManufactureID, int iVillageMapID );
	bool AddManufactureResultGroup_LUA( int iGroupID, int iItemID );
	//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN

#ifdef SERV_MANUFACTURE_PERIOD
	bool AddManufactureResultGroupWithRate_LUA( int iGroupID, int iItemID, float fRate, bool bSealed = false, int iPeriod = 0);
#else //SERV_MANUFACTURE_PERIOD
	bool AddManufactureResultGroupWithRate_LUA( int iGroupID, int iItemID, float fRate, bool bSealed = false );
#endif //SERV_MANUFACTURE_PERIOD
	
#else
	bool AddManufactureResultGroupWithRate_LUA( int iGroupID, int iItemID, float fRate );
#endif SERV_ADD_SEALED_ITEM_SIGN
	//}}

	// function
	const CXSLManufactureItemManager::ManufactureData* GetManufactureData( int iManufactureID ) const;
	int	 GetManufactureResultItemID( int iGroupID ) const;
#ifdef SERV_MANUFACTURE_PERIOD
	int	 GetManufactureResultItemIDPeriod(int iGroupID, int iItemID ) const;
#endif //SERV_MANUFACTURE_PERIOD
	//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN
	bool IsItemSealed( int iItemID ) const;
	void CheckSealedState_Lua( int iItemID ) const;
#endif SERV_ADD_SEALED_ITEM_SIGN
	//}}

	//{{ 2012. 08. 07	박세훈	소비성 아이템 획득 시 퀵슬롯 자동 등록
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	bool IsRandomResult( const int iGroupID ) const;
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}
	
#ifdef SERV_CHECK_POSSIBLE_MANUFACTURE_ID	// 적용날짜: 미정
	bool IsPossibleManufactureID( IN int iManufactureID )	{ return (m_setPossibleManufactureID.find( iManufactureID ) != m_setPossibleManufactureID.end()); }
#endif	// SERV_CHECK_POSSIBLE_MANUFACTURE_ID

private:
	std::map< int, ManufactureData >	m_mapManufacture;
	std::map< int, KLottery >			m_mapResultGroup;
	//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN
	std::map< int, bool >				m_mapIsSealed;
#endif SERV_ADD_SEALED_ITEM_SIGN
	//}}
	//{{ 2011. 12. 14	최육사	서버 에러 로그 버그 수정
#ifdef SERV_SERVER_ERROR_LOG_BUG_FIX
	std::set< int >						m_setDifServerGroupManufactureID;
#endif SERV_SERVER_ERROR_LOG_BUG_FIX
	//}}
#ifdef SERV_CHECK_POSSIBLE_MANUFACTURE_ID	// 적용날짜: 미정
	std::set< int >						m_setPossibleManufactureID;
#endif	// SERV_CHECK_POSSIBLE_MANUFACTURE_ID

#ifdef SERV_MANUFACTURE_PERIOD_FIX
	std::map< int, std::vector<int> >			m_mapManufactureVillage;
#endif //SERV_MANUFACTURE_PERIOD_FIX
#ifdef SERV_MANUFACTURE_PERIOD_FIX
	public:
	std::map< int, int >				m_mapPeriodGroup;
#endif SERV_MANUFACTURE_PERIOD_FIX
};

DefRefreshSingletonInline( CXSLManufactureItemManager );


