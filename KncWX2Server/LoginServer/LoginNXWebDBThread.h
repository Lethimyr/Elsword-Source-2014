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


class KLoginNXWebDBThread : public KSubThread
{
    DeclareDBThread( KLoginNXWebDBThread );
	//{{ 2009. 8. 17  최육사	sp profiler
	DeclareSPProfiler;
	//}}
    DeclPfID;

public:
    KLoginNXWebDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KLoginNXWebDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

	DECL_ON_FUNC_NOPARAM( DBE_GET_WEB_POINT_REWARD_REQ );
   _DECL_ON_FUNC( DBE_RESULT_WEB_POINT_REWARD_NOT, KDBE_GET_WEB_POINT_REWARD_ACK );
};

template < class T >
void KLoginNXWebDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_LOGIN_NX_WEB_DB, 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginNXWebDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_NX_WEB_DB, 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}


#endif SERV_WEB_POINT_EVENT
//}}