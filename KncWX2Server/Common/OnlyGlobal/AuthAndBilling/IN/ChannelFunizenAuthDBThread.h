#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

//{{ 2009. 8. 13  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_IN
class KChannelFunizenAuthDBThread : public KSubThread
{
    DeclareDBThread( KChannelFunizenAuthDBThread );
    DeclareSPProfiler;
    DeclPfID;
public:
    KChannelFunizenAuthDBThread();  // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KChannelFunizenAuthDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );    
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

    // event handler
	DECL_ON_FUNC( DBE_PUBLISHER_AUTHENTICATION_REQ );
};

template < class T >
void KChannelFunizenAuthDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CHANNEL_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KChannelFunizenAuthDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CHANNEL_USER, nTo, NULL, usEventID, data );
}
#endif SERV_COUNTRY_IN