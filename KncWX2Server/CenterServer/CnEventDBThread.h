#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  √÷¿∞ªÁ	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_ADD_EVENT_DB
class KCnEventDBThread : public KSubThread
{
    DeclareDBThread( KCnEventDBThread );
	DeclareSPProfiler;
	
	DeclPfID;

public:
    KCnEventDBThread();
    virtual ~KCnEventDBThread(void);

protected:
    // derived from KSubThread
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // DB query function

	
protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );	
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );
    void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID );

protected:
    // event handler
    	
	DECL_ON_FUNC_NOPARAM( DBE_EVENT_UPDATE_REQ );

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC_NOPARAM( DBE_EVENT_DB_SCRIPT_REQ );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_ADD_EVENT_DB
	DECL_ON_FUNC_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif // SERV_ADD_EVENT_DB


};

template < class T >
void KCnEventDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KCnEventDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CN_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KCnEventDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nGS, -1 };
    KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}
#endif //SERV_ADD_EVENT_DB