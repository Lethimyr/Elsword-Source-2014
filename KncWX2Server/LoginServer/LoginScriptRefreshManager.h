#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;


#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance( g_pLua ); \
	START_LOG( cout, L#className L" 스크립트 갱신을 완료하였습니다!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT


class KLoginScriptRefreshManager : public KThreadManager
{
	DeclareSingleton( KLoginScriptRefreshManager );
	DeclPfID;

public:
	KLoginScriptRefreshManager();
	virtual ~KLoginScriptRefreshManager();

	// derived from KThreadManager
	virtual void Init();
	virtual KThread* CreateThread();

	lua_State* GetRefreshLuaState() { return m_pRefreshLuaState; }

protected:
	lua_State* m_pRefreshLuaState;
};

DefSingletonInline( KLoginScriptRefreshManager );


#endif SERV_REALTIME_SCRIPT
//}}