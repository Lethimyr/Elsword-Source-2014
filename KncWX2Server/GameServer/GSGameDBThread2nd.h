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

class KGSGameDBThread2nd : public KSubThread
{
    DeclareDBThreadWithSP( KGSGameDBThread2nd );
    DeclareSPProfiler;
    DeclPfID;

public:
    KGSGameDBThread2nd(void);
    virtual ~KGSGameDBThread2nd(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );    
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

    _DECL_ON_FUNC( DBE_CHARACTER_LIST_2ND_REQ, KEGS_CHARACTER_LIST_1ST_ACK );
    _DECL_ON_FUNC( DBE_CHANNEL_CHECK_NICK_NAME_REQ, KEGS_CHECK_NICK_NAME_REQ );
    _DECL_ON_FUNC( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, KEGS_ENTRY_POINT_CHECK_NICK_NAME_REQ );
     DECL_ON_FUNC( DBE_GAME_CREATE_UNIT_REQ );
    
    _DECL_ON_FUNC( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ, KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK );
    _DECL_ON_FUNC( DBE_GAME_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_REQ );
    
    _DECL_ON_FUNC( DBE_CHANGE_NICK_NAME_REQ, KEGS_CHANGE_NICK_NAME_REQ );
#ifdef SERV_UNIT_WAIT_DELETE
	_DECL_ON_FUNC( DBE_GAME_FINAL_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_REQ );
	_DECL_ON_FUNC( DBE_GAME_RESTORE_UNIT_REQ, KEGS_RESTORE_UNIT_REQ );
#endif SERV_UNIT_WAIT_DELETE

    KStoredProcedure m_kSP;
};


template < class T >
void KGSGameDBThread2nd::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_GAME_DB_2ND, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSGameDBThread2nd::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_GAME_DB_2ND, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}


