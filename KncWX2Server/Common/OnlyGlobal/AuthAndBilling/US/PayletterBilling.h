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

#ifdef SERV_COUNTRY_US

#include "SubThread.h"

#include "BaseServer.h"
#include "BillManager.h"


class KPayletterBilling : public KSubThread
{
public:
	KPayletterBilling( void );
	virtual ~KPayletterBilling( void );

protected:

	void _ProcessErrorPacket( IN const KEPL_BT_PURCHASEITEM_ACK kPacket_ );
	void _ProcessErrorPacket( IN const KEPL_BT_REGCOUPON_ACK kPacket_ );
	void _ProcessErrorPacket( IN const KEPL_BT_GIFTITEM_ACK kPacket_ );
	int _GetErrorPacketType( IN const unsigned short usRetCode_ );

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	DECL_ON_FUNC( EPL_BT_HEALTH_CHECK_ACK );	// 빌링 연결 정상 여부 확인
	
	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );
	DECL_ON_FUNC( EPL_BT_BALANCE_CHECK_ACK );

	DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EPL_BT_PURCHASEITEM_ACK );
	
	DECL_ON_FUNC( EBILL_GIFT_ITEM_REQ );
	DECL_ON_FUNC( EPL_BT_GIFTITEM_ACK );

	DECL_ON_FUNC( EBILL_USE_COUPON_REQ );
	DECL_ON_FUNC( EPL_BT_REGCOUPON_ACK );


	// packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );
	template < class T > void SendToLogDB( UidType nUserUID, unsigned short usEventID, const T& data );
	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );
	void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID );
	void SendToLogDB( UidType nUserUID, unsigned short usEventID );

};


template < class T >
void KPayletterBilling::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KPayletterBilling::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KPayletterBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KPayletterBilling::SendToLogDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_LOG_DB, 0, anTrace, usEventID, data );
}


#endif SERV_COUNTRY_US
