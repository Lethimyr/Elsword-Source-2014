#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

#ifdef SERV_COUNTRY_CN

class KGiantInfo : public KSubThread
{
public:
	KGiantInfo( void );
	virtual ~KGiantInfo( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	DECL_ON_FUNC( EGIANT_COMMON_NULL_SERVER );
	DECL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT );

	DECL_ON_FUNC( EGIANT_INFO_GET_CCU_REQ );
	DECL_ON_FUNC( EGIANT_INFO_GET_TOTAL_CCU_ACK );
	DECL_ON_FUNC( EGIANT_INFO_GET_CHANNEL_CCU_ACK );
	DECL_ON_FUNC( EGIANT_INFO_CHAT_NOT );
	DECL_ON_FUNC( EGIANT_INFO_PUNISH_REQ );
#ifdef SERV_INFOSERVER_ADD_WORK
	DECL_ON_FUNC( EGIANT_INFO_USERONLINE_ACK );
	DECL_ON_FUNC( EGIANT_INFO_REQUESTID_ACK );
#endif //SERV_INFOSERVER_ADD_WORK
};

#endif // SERV_COUNTRY_CN