#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;


//{{ 2012. 10. 15	박세훈	채널 서버 실시간 스크립트 갱신 기능
#ifdef SERV_CHANNEL_SERVER_REALTIME_SCRIPT

#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance( g_pLua ); \
	START_LOG( cout, L#className L" 스크립트 갱신을 완료하였습니다!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT


class KChannelScriptRefreshManager : public KThreadManager
{
	DeclareSingleton( KChannelScriptRefreshManager );
	DeclPfID;

public:
	KChannelScriptRefreshManager();
	virtual ~KChannelScriptRefreshManager();

	void RegToLua( lua_State* pLuaState );

	// 스크립트 갱신 요청 lua함수
	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	void SmsManager_Lua( void );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	void LogManager_Lua();
#endif SERV_LOG_SYSTEM_NEW
	//}}

	// derived from KThreadManager
	virtual void Init();
	virtual KThread* CreateThread();

	lua_State* GetRefreshLuaState() { return m_pRefreshLuaState; }

protected:
	lua_State* m_pRefreshLuaState;
};

DefSingletonInline( KChannelScriptRefreshManager );


#endif SERV_REALTIME_SCRIPT
//}}

#endif SERV_CHANNEL_SERVER_REALTIME_SCRIPT
//}}