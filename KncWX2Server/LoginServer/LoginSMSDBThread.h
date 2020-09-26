#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KLoginSMSDBThread : public KSubThread
{
	DeclareDBThread( KLoginSMSDBThread );

public:
	KLoginSMSDBThread();
	virtual ~KLoginSMSDBThread(void);

protected:
	// derived from KSubThread
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
	// packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

	// event handler
	DECL_ON_FUNC( DBE_SEND_PHONE_MSG_NOT );
	//{{ 2010. 11. 2	최육사	서버간 접속 모니터링
#ifdef SERV_MORNITORING
   _DECL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_REQ, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
	//}}
};

template < class T >
void KLoginSMSDBThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SMS_DB, 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginSMSDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_SMS_DB, 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}
