#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

#ifdef SERV_COUNTRY_TH

class KAsiaSoftAuth : public KSubThread
{
public:
	KAsiaSoftAuth( void );
	virtual ~KAsiaSoftAuth( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	//{{ 허상형 : [2012/6/26] //	AsiaSoft Auth
	DECL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ );	// EAS_AUTH_LOGIN_REQ
	DECL_ON_FUNC( EAS_AUTH_LOGIN_ACK );

	DECL_ON_FUNC( EPUBLISHER_REG_USER_REQ );
	DECL_ON_FUNC( EPUBLISHER_UNREG_USER_REQ );
	DECL_ON_FUNC( EAS_GET_IPBONUS_ACK );

	DECL_ON_FUNC( EAS_AUTH_AKEY_ACK );
	//}} 허상형 : [2012/6/26] //	AsiaSoft Auth
};

#endif // SERV_COUNTRY_TH