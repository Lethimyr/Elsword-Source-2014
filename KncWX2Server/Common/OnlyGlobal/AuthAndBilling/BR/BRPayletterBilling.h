//////////////////////////////////////////////////////////////////////////
//	PayletterBilling.h
//
//	Payletter 빌링 클래스.
//	베이스는 KNexonBillingTCP
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_BR

#include "SubThread.h"

#include "BaseServer.h"
#include "BillManager.h"


class KBRPayletterBilling : public KSubThread
{
public:
	KBRPayletterBilling( void );
	virtual ~KBRPayletterBilling( void );

protected:

	void _ProcessErrorPacket( IN const KEPL_BR_PURCHASEITEM_ACK kPacket_ );
	int _GetErrorPacketType( IN const unsigned short usRetCode_ );

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );
	
	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );
	DECL_ON_FUNC( EPL_BR_BALANCE_CHECK_ACK );

	DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EPL_BR_PURCHASEITEM_ACK );

	// packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );
	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );
	void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID );

};


template < class T >
void KBRPayletterBilling::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KBRPayletterBilling::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KBRPayletterBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

#endif SERV_COUNTRY_BR
