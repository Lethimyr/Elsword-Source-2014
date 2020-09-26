#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

class KNexonBillingAuth : public KSubThread
{
public:
	KNexonBillingAuth( void );
	virtual ~KNexonBillingAuth( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

    DECL_ON_FUNC( ENX_USER_LOGIN_REQ );
    DECL_ON_FUNC( ENX_USER_LOGOUT_REQ );
};
