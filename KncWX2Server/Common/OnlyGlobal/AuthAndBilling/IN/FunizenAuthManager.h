#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_IN
class KFunizenAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KFunizenAuthManager );

public:
	KFunizenAuthManager();
	virtual ~KFunizenAuthManager();
	void RegToLua();

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();	
};
DefSingletonInline( KFunizenAuthManager );
#endif SERV_COUNTRY_IN