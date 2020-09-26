#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
#include "odbc/DBUtil.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"


//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
//#ifdef SERV_RELAY_DB_CONNECTION


class KGSRelayDBThread : public KSubThread
{
    DeclareDBThread( KGSRelayDBThread );
	DeclareSPProfiler;

public:
    KGSRelayDBThread();
    virtual ~KGSRelayDBThread(void);

protected:
    // derived from KSubThread
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

protected:
    // event handler
	DECL_ON_FUNC( DBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT );
	DECL_ON_FUNC( DBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_REQ );
	DECL_ON_FUNC( DBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT );
};

template < class T >
void KGSRelayDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_JP_RELAY_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSRelayDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_JP_RELAY_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}


//#endif SERV_RELAY_DB_CONNECTION
//}}