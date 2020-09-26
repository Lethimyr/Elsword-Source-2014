#pragma once
#include "ActorProxy.h"
#include <windows.h>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

SmartPointer( KChannelLoginProxy );

class KChannelLoginProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KChannelLoginProxy(void);
    virtual ~KChannelLoginProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );

    // derived from KSession
    virtual void OnDestroy();

protected:

    _DECL_ON_FUNC( ELG_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
    DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

    bool RoutePacket( const KEventPtr& spEvent );
	void ProcbyNotExistUser( const KEventPtr& spEvent, std::vector<UidType>& vecUID );

protected:
};