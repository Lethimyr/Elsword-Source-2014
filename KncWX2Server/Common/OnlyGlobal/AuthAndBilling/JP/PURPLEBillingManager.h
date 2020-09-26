#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"

#ifdef SERV_COUNTRY_JP

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"


class KPurpleBillingManager : public KThreadManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KPurpleBillingManager );

public:
	KPurpleBillingManager();
	virtual ~KPurpleBillingManager();

	void RegToLua();



	// derived from KThreadManager
	virtual void Init( int nThreadNum );
	virtual KThread* CreateThread();
	virtual void BeginThread();
	virtual void EndThread();
	virtual void Tick();


};

DefSingletonInline( KPurpleBillingManager );

#endif // SERV_COUNTRY_JP