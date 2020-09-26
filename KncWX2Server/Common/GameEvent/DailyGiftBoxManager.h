#pragma once

#include "ServerDefine.h"

//{{ 2012. 06. 05	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
#include "RefreshSingleton.h"
#include <map>
#include <set>

class KDailyGiftBoxManager
{
	DeclareRefreshSingleton( KDailyGiftBoxManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;

public:
	enum PURCHASED_STATE
	{
		PURCHASED_NOT = 0,
		PURCHASED_BUY,
		PURCHASED_REFUND,
		PURCHASED_CONFIRM,
	};
	void AddPackageInfo_Lua( IN int iItemID, IN int iScriptID );
	bool IsDailyGiftBoxItemID( IN const int& iItemID ) const;
	bool IsDailyGiftBoxScriptID( IN const int& iScriptID, OUT int& iItemID ) const;

private:
	KDailyGiftBoxManager();
	~KDailyGiftBoxManager();

	std::set<int>									m_setItemID;
	std::map<int /*iScriptID*/, int /*iItemID*/>	m_mapPackageInfo;
};

DefRefreshSingletonInline( KDailyGiftBoxManager );

#endif SERV_EVENT_DAILY_GIFT_BOX
//}}