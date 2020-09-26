#pragma once
#include "ServerDefine.h"
#include "RefreshSingleton.h"
//#include "ServerDefine.h"


//{{ 2011. 10. 12	ÃÖÀ°»ç	ÄÁÅÙÃ÷ °ü¸®ÀÚ
//#ifdef SERV_CONTENT_MANAGER
#ifdef SERV_CONTENT_MANAGER_INT
#include "TimerManager.h"
#include "Event.h"
#include "ServerPacket.h"
#endif SERV_CONTENT_MANAGER_INT

class KGSContentManager
{
private:
	DeclareRefreshSingleton( KGSContentManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;	
	DeclDump;
	DeclToStringW;

public:
	KGSContentManager(void);
	~KGSContentManager(void);

	// for lua
	void SetEnableCashShop_LUA( bool bVal );

	// result
	bool IsEnableCashShop() const	{ return m_bEnableCashShop; }
#ifdef SERV_CONTENT_MANAGER_INT
	void Init();
	void SetEnableCashShop( bool bVal );
	int GetReleaseTick() const { return m_iReleaseTick; }
	void SetReleaseTick( int iReleaseTick ) { m_iReleaseTick = iReleaseTick; }
#endif SERV_CONTENT_MANAGER_INT

private:
	bool			m_bEnableCashShop;
#ifdef SERV_CONTENT_MANAGER_INT
	int				m_iReleaseTick;
#endif SERV_CONTENT_MANAGER_INT
};

DefRefreshSingletonInline( KGSContentManager );


//#endif SERV_CONTENT_MANAGER
//}}