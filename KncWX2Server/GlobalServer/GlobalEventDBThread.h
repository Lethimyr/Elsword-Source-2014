#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  최육사	sp profiler
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
class KGlobalEventDBThread : public KSubThread
{
    DeclareDBThread( KGlobalEventDBThread );
	//{{ 2009. 8. 17  최육사	sp profiler	
	DeclareSPProfiler;
	//}}
    DeclPfID;
public:

    KGlobalEventDBThread();  // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KGlobalEventDBThread(void);

protected:
	virtual void Run()     // derived from KThread
	{
		std::cout << "KSubThread::Run(), thread start." << std::endl;

		

		DWORD ret;

		while( true )
		{
			ret = ::WaitForSingleObject( m_hKillEvent, 1 );

			if( ret == WAIT_OBJECT_0 ) break;

			if( ret == WAIT_TIMEOUT ) Loop();

			else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

		}
	}
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );
    void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID );

    

    // event handler

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
	DECL_ON_FUNC_NOPARAM( DBE_EVENT_UPDATE_REQ );
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

	DECL_ON_FUNC_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );

};

template < class T >
void KGlobalEventDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGlobalEventDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_GLOBAL_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KGlobalEventDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nGS, -1 };
    KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}
#endif //SERV_ADD_EVENT_DB