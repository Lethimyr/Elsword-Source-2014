#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"


//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT


class KGSNXWebDBThread : public KSubThread
{
    DeclareDBThread( KGSNXWebDBThread );
	//{{ 2009. 8. 17  최육사	sp profiler
	DeclareSPProfiler;
	//}}
    DeclPfID;

public:
    KGSNXWebDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KGSNXWebDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

	DECL_ON_FUNC( DBE_INCREASE_WEB_POINT_REQ );
};

template < class T >
void KGSNXWebDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_NX_WEB_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSNXWebDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_NX_WEB_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}


#endif SERV_WEB_POINT_EVENT
//}}

