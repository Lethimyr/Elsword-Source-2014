#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 13  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//{{ 2009. 10. 6  최육사	길드
#include "Enum/Enum.h"
//}}

#ifdef SERV_ADD_EVENT_DB
class KGSEventDBThread : public KSubThread
{
    DeclareDBThread( KGSEventDBThread );
	DeclareSPProfiler;

	DeclPfID;

public:
    KGSEventDBThread();
    virtual ~KGSEventDBThread(void);

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

	//{{ 2009. 10. 30  최육사	길드레벨
	template < class T > void SendToLoginServer( unsigned short usEventID, const T& data );	
	//}}

	//{{ 2010. 11. 25	조효진	추가 수정 임규수 일본 추가
#ifdef SERV_SELECT_UNIT_NEW
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );
#endif SERV_SELECT_UNIT_NEW
	//}}

protected:
    // event handler
    

	DECL_ON_FUNC( DBE_UPDATE_EVENT_TIME_REQ );

#ifdef SERV_ADVERTISEMENT_EVENT
	DECL_ON_FUNC( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT );
#endif SERV_ADVERTISEMENT_EVENT

	DECL_ON_FUNC( DBE_GAME_SELECT_UNIT_EVENT_DATA_NOT );
	DECL_ON_FUNC( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT );
	
	_DECL_ON_FUNC( DBE_UPDATE_UNIT_CONNECT_TIME_EVENT_INFO_NOT, KDBE_UPDATE_UNIT_INFO_REQ );
	
	DECL_ON_FUNC_NOPARAM( DBE_EVENT_UPDATE_REQ );

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC_NOPARAM( DBE_EVENT_DB_SCRIPT_REQ );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_ADD_EVENT_DB
	DECL_ON_FUNC_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif // SERV_ADD_EVENT_DB


#ifdef SERV_GLOBAL_EVENT_TABLE
	DECL_ON_FUNC( DBE_GLOBAL_EVENT_TABLE_INSERT_NOT );
#endif //SERV_GLOBAL_EVENT_TABLE

};

template < class T >
void KGSEventDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_EVENT_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSEventDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_EVENT_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

//{{ 2009. 10. 30  최육사	길드레벨
template < class T >
void KGSEventDBThread::SendToLoginServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_EVENT_DB, 0, PI_LOGIN_USER, 0, NULL, usEventID, data );
}
//}}

//{{ 2010. 11. 25	조효진	추가 수정 임규수 일본 추가
#ifdef SERV_SELECT_UNIT_NEW
template < class T >
void KGSEventDBThread::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend(PI_GS_EVENT_DB, 0, PI_LOG_DB, 0, NULL, usEventID, data );
}
#endif SERV_SELECT_UNIT_NEW
//}}
#endif //SERV_ADD_EVENT_DB