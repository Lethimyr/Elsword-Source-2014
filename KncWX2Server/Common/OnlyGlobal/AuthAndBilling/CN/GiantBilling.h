#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

#ifdef SERV_COUNTRY_CN

//#include "../GameServer/BillManager.h"
#include "BaseServer.h"

class KGiantBilling : public KSubThread
{
public:
	KGiantBilling( void );
	virtual ~KGiantBilling( void );

	// packet send function
	template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToKOGBillingDB( unsigned short usEventID, const T& data );
	void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID );
	void SendToKOGBillingDB( unsigned short usEventID );

protected:
	int CheckReturn(int iRet);

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	// event handler
 	DECL_ON_FUNC( EGIANT_COMMON_NULL_SERVER );
 	DECL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT );

	// 충전 카드는 필요시에 재구현해야함
	//DECL_ON_FUNC( EGIANT_BILLING_USE_CARD_CMD );
	//DECL_ON_FUNC( EGIANT_BILLING_USE_CARD_RET_CMD );

	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );
	DECL_ON_FUNC( EGIANT_BILLING_CONSUME_REQ );
	DECL_ON_FUNC( EGIANT_BILLING_PCARD_CMD );
	DECL_ON_FUNC( EGIANT_BILLING_SCARD_CMD );
	DECL_ON_FUNC( EGIANT_BILLING_RETURN );
};

template < class T >
void KGiantBilling::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_LOGIN_GIANT_BILLING, 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}

template < class T >
void KGiantBilling::SendToKOGBillingDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_LOGIN_GIANT_BILLING, 0, PI_LOGIN_KOG_BILLING_DB, 0, NULL, usEventID, data );
}

#endif // SERV_COUNTRY_CN
