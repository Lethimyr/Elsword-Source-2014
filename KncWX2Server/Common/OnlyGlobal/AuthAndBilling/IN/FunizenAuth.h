#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"
#include "KncException.h"

#ifdef SERV_COUNTRY_IN
class KFunizenAuth : public KSubThread
{
public:
	KFunizenAuth( void );
	virtual ~KFunizenAuth( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );
	template < class T > void SendToFunizenAuthDB( UidType nUserUID, unsigned short usEventID, const T& data);

	DECL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ );
};

template < class T >
void KFunizenAuth::SendToFunizenAuthDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_CHANNEL_USER, 0, PI_CHANNEL_AUTH_DB, 0, anTrace, usEventID, data );
}
#endif SERV_COUNTRY_IN