//////////////////////////////////////////////////////////////////////////
//	CJBilling.h
//
//	CJ 빌링 클래스.
//	베이스는 KNexonBillingTCP
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_ID

#include "SubThread.h"

#include "BaseServer.h"
#include "BillManager.h"

class KCJBilling : public KSubThread
{
public:
	KCJBilling( void );
	virtual ~KCJBilling( void );

protected:

	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	//bool GetBillingResult( const std::string& strObjectName, const std::string& strParameter, OUT std::string& strResult );
	std::string GetCNString( unsigned int uiCjUID );
	int GetNetError( const char* szResCD );

	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );
	DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );

	DECL_ON_FUNC( EBILL_GIFT_ITEM_REQ );		// 실제 사용안함
	DECL_ON_FUNC( EBILL_USE_COUPON_REQ );		// 실제 사용안함 (퍼블 체크없이 바로 사용)

	// packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );

	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );
	void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID );
};

template < class T >
void KCJBilling::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KCJBilling::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KCJBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

#endif // SERV_COUNTRY_ID
