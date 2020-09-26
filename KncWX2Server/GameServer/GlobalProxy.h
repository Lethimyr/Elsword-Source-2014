#pragma once
#include "ActorProxy.h"
#include <windows.h>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

SmartPointer( KGlobalProxy );

class KGlobalProxy : public KActorProxy
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

public:
    KGlobalProxy(void);
    virtual ~KGlobalProxy(void);

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );

    // derived from KSession
    virtual void OnDestroy();

protected:

	//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
   _DECL_ON_FUNC( EGB_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}
	DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

    bool RoutePacket( const KEventPtr& spEvent );
	void ProcbyNotExistUser( const KEventPtr& spEvent, std::vector<UidType>& vecUID );

protected:
};