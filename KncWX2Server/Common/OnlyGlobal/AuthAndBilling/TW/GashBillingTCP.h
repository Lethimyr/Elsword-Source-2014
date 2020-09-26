#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"

#include "BaseServer.h"

#ifdef SERV_COUNTRY_TWHK

//{{ 2011.7.12 지헌 : Gash 포인트 통합
#ifdef SERV_BILLING_ADD_GASH_POINT

class KGASHBillingTCP : public KSubThread
{
public:
	KGASHBillingTCP( void );
	virtual ~KGASHBillingTCP( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );
	template < class T > void SendToGASHBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );
	void SendToUser( UidType nTo, unsigned short usEventID );
	void SendToKOGBillingDB( UidType nTo, unsigned short usEventID );
	void SendToGASHBillingDB( UidType nTo, unsigned short usEventID );

	
	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );
	DECL_ON_FUNC( EGASH_GET_GASHPOINT_ACK );

	DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EBILL_GIFT_ITEM_REQ );

	DECL_ON_FUNC( EGASH_DECREASE_GASHPOINT_ACK );	// 구매든 선물이든 둘다 해당 패킷으로 Return 옴

	DECL_ON_FUNC( EBILL_USE_COUPON_REQ );			// 실제 사용안함 (퍼블 체크없이 바로 사용)
};

template < class T >
void KGASHBillingTCP::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KGASHBillingTCP::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGASHBillingTCP::SendToGASHBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_PUBLISHER_BILLING_DB, 0, anTrace, usEventID, data );
}


#endif // SERV_BILLING_ADD_GASH_POINT
//}}

#endif // SERV_COUNTRY_TWHK