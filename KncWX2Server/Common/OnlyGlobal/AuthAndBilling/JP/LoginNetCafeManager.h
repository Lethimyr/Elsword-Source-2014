#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_JP

class KLoginNetCafeManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KLoginNetCafeManager );

public:
	KLoginNetCafeManager();
	virtual ~KLoginNetCafeManager();

	void RegToLua();



	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();


};

DefSingletonInline( KLoginNetCafeManager );

#endif //SERV_COUNTRY_JP