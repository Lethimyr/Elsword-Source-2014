#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

#ifdef SERV_COUNTRY_CN

class KGiantAuth : public KSubThread
{
public:
	KGiantAuth( void );
	virtual ~KGiantAuth( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	DECL_ON_FUNC( EGIANT_COMMON_NULL_SERVER );
	DECL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT );
 	DECL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ );	// EGIANT_AUTH_LOGIN_REQ
    DECL_ON_FUNC( EGIANT_AUTH_LOGIN_SUCCESS );
 	DECL_ON_FUNC( EGIANT_AUTH_LOGIN_FAIL );
    DECL_ON_FUNC( EPUBLISHER_SECURITY_AUTH_REQ );	// EGIANT_AUTH_LOGIN_MTCARD

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	DECL_ON_FUNC( EGIANT_AUTH_DIRECT_CHARGE_REQ );
	DECL_ON_FUNC( EGIANT_AUTH_DIRECT_CHARGE_ACK );
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
};

#endif // SERV_COUNTRY_CN