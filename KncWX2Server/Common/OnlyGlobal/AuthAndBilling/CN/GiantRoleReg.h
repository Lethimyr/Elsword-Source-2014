#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

#ifdef SERV_COUNTRY_CN

class KGiantRoleReg : public KSubThread
{
public:
	KGiantRoleReg( void );
	virtual ~KGiantRoleReg( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	DECL_ON_FUNC( EGIANT_COMMON_NULL_SERVER );
	DECL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT );

	DECL_ON_FUNC( EGIANT_ROLEREG_CREATE_UNIT_REQ );
	_DECL_ON_FUNC( EGIANT_ROLEREG_CREATE_UNIT_ACK, KEGIANT_ROLEREG );
	DECL_ON_FUNC( EGIANT_ROLEREG_DELETE_UNIT_REQ );
	_DECL_ON_FUNC( EGIANT_ROLEREG_DELETE_UNIT_ACK, KEGIANT_ROLEREG );

	DECL_ON_FUNC( EGIANT_ROLEREG_CREATE_GUILD_REQ );
	_DECL_ON_FUNC( EGIANT_ROLEREG_CREATE_GUILD_ACK, KEGIANT_ROLEREG );
	DECL_ON_FUNC( EGIANT_ROLEREG_DELETE_GUILD_REQ );
	_DECL_ON_FUNC( EGIANT_ROLEREG_DELETE_GUILD_ACK, KEGIANT_ROLEREG );



};

#endif // SERV_COUNTRY_CN