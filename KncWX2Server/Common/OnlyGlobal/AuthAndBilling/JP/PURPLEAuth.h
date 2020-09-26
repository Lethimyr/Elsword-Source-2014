#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"
#include "KncException.h"

#ifdef SERV_COUNTRY_JP

class KPURPLEAuth : public KSubThread
{
public:
	KPURPLEAuth( void );
	virtual ~KPURPLEAuth( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	
	DECL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ );
	
};

#endif // SERV_COUNTRY_JP