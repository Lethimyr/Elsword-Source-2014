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


//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KGlobalGameDBThread : public KSubThread
{
    DeclareDBThread( KGlobalGameDBThread );
	DeclareSPProfiler;
    DeclPfID;
public:

    KGlobalGameDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KGlobalGameDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );
    
	// event handler
   _DECL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ );
};

template < class T >
void KGlobalGameDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGlobalGameDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}