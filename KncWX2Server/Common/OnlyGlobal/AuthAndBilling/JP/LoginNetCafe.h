#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

#include "BaseServer.h"

#ifdef SERV_COUNTRY_JP

class KLoginNetCafe : public KSubThread
{
public:
	KLoginNetCafe( void );
	virtual ~KLoginNetCafe( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	DECL_ON_FUNC( EPUBLISHER_REG_USER_REQ );
	DECL_ON_FUNC( EPUBLISHER_UNREG_USER_REQ );
};

#endif //SERV_COUNTRY_JP