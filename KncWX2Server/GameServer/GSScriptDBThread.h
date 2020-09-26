#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 13  √÷¿∞ªÁ	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//{{ 2009. 10. 6  √÷¿∞ªÁ	±ÊµÂ
#include "Enum/Enum.h"
//}}

#ifdef SERV_ADD_SCRIPT_DB
class KGSScriptDBThread : public KSubThread
{
    DeclareDBThread( KGSScriptDBThread );
	DeclareSPProfiler;

	DeclPfID;

public:
    KGSScriptDBThread();
    virtual ~KGSScriptDBThread(void);

protected:
    // derived from KSubThread
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // DB query function

	bool Query_UpdateConnectTimeEventInfo( IN const UidType iUserUID, IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	
protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

#ifdef SERV_STRING_FILTER_USING_DB
	DECL_ON_FUNC_NOPARAM( DBE_CHECK_STRING_FILTER_UPDATE_REQ );
	DECL_ON_FUNC_NOPARAM( DBE_STRING_FILTER_UPDATE_REQ );
#endif //SERV_STRING_FILTER_USING_DB

protected:
    // event handler
    
};

template < class T >
void KGSScriptDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_EVENT_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSScriptDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_EVENT_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

#endif //SERV_ADD_SCRIPT_DB