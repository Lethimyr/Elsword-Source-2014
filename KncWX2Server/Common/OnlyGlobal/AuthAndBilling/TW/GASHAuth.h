#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"
#include "KncException.h"

#ifdef SERV_COUNTRY_TWHK

class KGASHAuth : public KSubThread
{
public:
	KGASHAuth( void );
	virtual ~KGASHAuth( void );

public:
	enum GASH_RECEIVE_PACKET_AUTHENTICATION
	{
		GRPAC_SERVICE_CODE,
		GRPAC_SERVICE_REGION,
		GRPAC_SERVICE_ACCOUNTID,
		GRPAC_SERVICE_ACCOUNTPASSWORD,
		GRPAC_LOCKFLAG,
		GRPAC_IPADDRESS,
		GRPAC_PACKETID,

		GRPAC_NUM,
	};

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	void GetGashAuthenticationReqPacket( const KEPUBLISHER_AUTHENTICATION_REQ& kPacket_, 
										 const std::string strServiceAccountID,
										 const std::string strIPAddress,
										 OUT std::string& strPacket );

	void GetGashAuthorizationReqPacket( const KEPUBLISHER_REG_USER_REQ& kPacket_, 
										 const std::string strServiceAccountID,
										 const std::string strIPAddress,
										 OUT std::string& strPacket );

	void GetGashAccountingReqPacket( const KEPUBLISHER_UNREG_USER_REQ& kPacket_, 
										const std::string strServiceAccountID,
										const std::string strIPAddress,
										OUT std::string& strPacket );

	DECL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ );	// EGASH_AUTHENTICATION_REQ 퍼블에 요청
    DECL_ON_FUNC( EGASH_AUTHENTICATION_ACK );
	DECL_ON_FUNC( EPUBLISHER_REG_USER_REQ );		// EGASH_AUTHORIZATION_REQ 퍼블에 요청
    DECL_ON_FUNC( EGASH_AUTHORIZATION_ACK );
	DECL_ON_FUNC( EPUBLISHER_UNREG_USER_REQ );			// EGASH_ACCOUNTING_REQ 퍼블에 요청
    DECL_ON_FUNC( EGASH_ACCOUNTING_ACK );
};

#endif // SERV_COUNTRY_TWHK