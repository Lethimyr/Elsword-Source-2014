#pragma once
//#ifdef SERV_ID_NETMARBLE_PCBANG

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



class KGSIDPcbangDBThread : public KSubThread
{
	DeclareDBThread( KGSIDPcbangDBThread );
	DeclareSPProfiler;
	DeclPfID;

public:
	KGSIDPcbangDBThread();
	virtual ~KGSIDPcbangDBThread();

protected:
	// derived from KSubThread
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// packet send function	
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );

	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

#ifdef SERV_ID_NETMARBLE_PCBANG
	DECL_ON_FUNC( DBE_ID_PCBANG_CEHCK_AUTH_REQ );
#endif //SERV_ID_NETMARBLE_PCBANG
	


};


template < class T >
void KGSIDPcbangDBThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_ID_PUBLISHER_PCBANG_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSIDPcbangDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_ID_PUBLISHER_PCBANG_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

//#endif //SERV_ID_NETMARBLE_PCBANG