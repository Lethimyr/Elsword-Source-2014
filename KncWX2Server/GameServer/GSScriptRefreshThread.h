#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"


//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT


class KGSScriptRefreshThread : public KSubThread
{
public:
	KGSScriptRefreshThread( void );
	virtual ~KGSScriptRefreshThread( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	template < class T > void SendToServer( unsigned short usEventID, const T& data );

   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );
};


#endif SERV_REALTIME_SCRIPT
//}}