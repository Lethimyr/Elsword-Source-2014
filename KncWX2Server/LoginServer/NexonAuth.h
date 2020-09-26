#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"


//{{ 2010. 06. 04  √÷¿∞ªÁ	≥ÿΩºPCπÊ ¿Œ¡ı º≠πˆ ∞≥∆Ì
#ifdef SERV_PCBANG_AUTH_NEW


class KNexonAuth : public KSubThread
{
public:
	KNexonAuth( void );
	virtual ~KNexonAuth( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

    DECL_ON_FUNC( ENX_AUTH_LOGIN_REQ );
    DECL_ON_FUNC( ENX_AUTH_LOGOUT_NOT );
	DECL_ON_FUNC( ENX_AUTH_SYNCHRONIZE_ACK );
};


#endif SERV_PCBANG_AUTH_NEW
//}}