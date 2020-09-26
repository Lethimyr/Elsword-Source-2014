#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

#include "odbc/DBUtil.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "ClientPacket.h"

#include "..\Common\ODBC\StoredProcedure.h"

class KGSLogDBThread2nd : public KSubThread
{
    DeclareDBThreadWithSP( KGSLogDBThread2nd );
    DeclareSPProfiler;
    DeclPfID;

public:
    KGSLogDBThread2nd(void);
    virtual ~KGSLogDBThread2nd(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );    
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

    KStoredProcedure m_kSP;

    DECL_ON_FUNC( DBE_CHANNEL_LIST_REQ );


};


template < class T >
void KGSLogDBThread2nd::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_LOG_DB_2ND, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSLogDBThread2nd::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_LOG_DB_2ND, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}


