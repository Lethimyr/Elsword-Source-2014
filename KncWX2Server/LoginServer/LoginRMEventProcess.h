#pragma once

#include "Performer.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "KncSend.h"


class KLoginRMEventProcess : public KPerformer
{
	//{{ 2010. 8. 29	최육사	이벤트 큐 사이즈 덤프
#ifdef SERV_MAX_QUEUE_SIZE_DUMP
	DeclToStringW;
#endif SERV_MAX_QUEUE_SIZE_DUMP
	//}}	
	DeclPfID;

public:
	KLoginRMEventProcess();
	virtual ~KLoginRMEventProcess();

	template < class T > void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );

	// override.	
	virtual void ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer   

	DECL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_REQ );
	DECL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT );
	DECL_ON_FUNC( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ );

protected:
};

template < class T >
void KLoginRMEventProcess::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_LOGIN_ROOM, 0, PI_GS_CHARACTER, nTo, anTrace, usEventID, data );
}

