#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_JP

class KPurpleAuthManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KPurpleAuthManager );

public:
	KPurpleAuthManager();
	virtual ~KPurpleAuthManager();

	void RegToLua();

	

	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();

	
};

DefSingletonInline( KPurpleAuthManager );

#endif // SERV_COUNTRY_JP