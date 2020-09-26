#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "SystemPacket.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "ClientPacket.h"



class KGlobalAccountDBThread : public KSubThread
{
    DeclareDBThread( KGlobalAccountDBThread );
	DeclareSPProfiler;
    DeclPfID;
public:

    KGlobalAccountDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KGlobalAccountDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToNXGMToolTcpManager( unsigned short usEventID, const T& data );    
	template < class T > void SendToNXGMToolTcpServer( unsigned short usEventID, const T& data );
//#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	void SendToUser( UidType nTo, unsigned short usEventID );
//#endif	// SERV_LOCAL_RANKING_SYSTEM

	//{{ 2013. 05. 14	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	DECL_ON_FUNC( DBE_CHECK_BLOCK_LIST_REQ );
#endif SERV_BLOCK_LIST
	//}}
};

template < class T >
void KGlobalAccountDBThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGlobalAccountDBThread::SendToNXGMToolTcpManager( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GLOBAL_NX_GMTOOL_TCP, NULL, usEventID, data );
	SiKNexonGMToolTCPManager()->QueueingEvent( spEvent );
}

template < class T >
void KGlobalAccountDBThread::SendToNXGMToolTcpServer( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GLOBAL_NX_GMTOOL_TCP, NULL, usEventID, data );
	SiKNexonGMToolServer()->QueueingEvent( spEvent );
}

//#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
template < class T >
void KGlobalAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}
//#endif	// SERV_LOCAL_RANKING_SYSTEM

