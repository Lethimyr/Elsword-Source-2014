#pragma once
#include "ActorProxy.h"

#include <windows.h>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KNCSingleton.h"
#include "Socket/NetCommon.h"
#include "ClientPacket.h"

#include <queue>
#include <boost/shared_ptr.hpp>

SmartPointer( KGSProxy );

class KGSProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KGSProxy(void);
    virtual ~KGSProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );

    template < typename T >
    bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false )
    {
        return KSession::SendPacket( PI_GS_USER, 0, NULL, usEventID, data, bLogging, bCompress );
    }

    bool SendID( unsigned short usEventID )
    {
        return KSession::SendID( PI_GS_USER, 0, NULL, usEventID );
    }
        
    //////////////////////////////////////////////////////////////////////////
    // Interfaces for Client Programmer.

    //virtual bool Init( bool bUseIOCP );   // 로봇을 제작하는 경우 iocp를 사용하도록 셋팅 가능.
    //virtual bool Connect( const TCHAR* szIP, unsigned short usPort ); // 소켓 접속만 한다.

    //int Connect( IN const KServerInfo& kServerInfo, IN const KEventPtr spEvent, IN DWORD dwTimeOut );

    // 060305. florist. 서버 이동시 cached data가 문제를 일으켜, 싱크기능이 추가됨.
    // 060412. florist. 서버 이동 절차 강화. 확실한 req/ack 처리를 진행한다.
    //bool RequestServerMigration( IN const KServerInfo& kInfo, DWORD dwTimeOut = 3000 );

protected:  // Recv Msg Handler & Util Function
    virtual void OnSocketError();
   // DECL_ON_FUNC( EVENT_VERIFY_ACCOUNT_ACK );
   // DECL_ON_FUNC( EVENT_GMTOOL_AUTHENTICATION_ACK );
   //_DECL_ON_FUNC( EVENT_SERVER_MIGRATION_ACK, int );

	//DECL_ON_FUNC( EGS_CREATE_UNIT_ACK );
    DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );
};