#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT


class KCnScriptRefreshManager : public KThreadManager
{
	DeclareSingleton( KCnScriptRefreshManager );
	DeclPfID;

public:
	KCnScriptRefreshManager();
	virtual ~KCnScriptRefreshManager();

	// derived from KThreadManager
	virtual void Init();
	virtual KThread* CreateThread();

	lua_State* GetRefreshLuaState() { return m_pRefreshLuaState; }

protected:
	lua_State* m_pRefreshLuaState;
};

DefSingletonInline( KCnScriptRefreshManager );


#endif SERV_REALTIME_SCRIPT
//}}