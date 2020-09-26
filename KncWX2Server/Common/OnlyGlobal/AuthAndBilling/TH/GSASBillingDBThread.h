#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "BillManager.h"

#ifdef SERV_COUNTRY_TH

class KGSASBillingDBThread : public KSubThread
{
	DeclareDBThread( KGSASBillingDBThread );
	DeclPfID;

public:
	KGSASBillingDBThread();
	virtual ~KGSASBillingDBThread();

protected:
	// derived from KSubThread
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToClient( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToBillManager( unsigned short usEventID );
	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );
	void SendToClient( UidType nTo, unsigned short usEventID );
	void SendToBillingDB( UidType nUserUID, unsigned short usEventID );
	void SendToLogDB( unsigned short usEventID );

	int GetCashPoint( IN const std::wstring& wstrUserID, OUT int& iCashPoint );
	int UseCoupon( IN const int iServerGroupID, IN const std::wstring& wstrUserID, IN const std::wstring& wstrNickName, IN const std::wstring& wstrCardPassword, IN const std::wstring& wstrIP,
		OUT int& iEventID, OUT int& iGiftID );
	int InGameLessGPoint( IN const std::wstring& wstrTransID, IN const std::wstring& wstrUserID, IN const std::wstring& wstrReceiverUserID, IN int iPoint, IN int iItemID,
		IN int iProductNO, IN const unsigned short& usQuantity, IN const std::wstring& wstrUserIP );
	int ReportHackUser( IN UidType iUserUID, IN const std::wstring& wstrUserID, IN UidType iUnitUID, IN const std::wstring& wstrNickName, IN UidType m_iServerUID, IN const std::string& strIP );
	// event handler
	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );
	DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EBILL_GIFT_ITEM_REQ );

public:
protected:
};

template < class T >
void KGSASBillingDBThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSASBillingDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KGSASBillingDBThread::SendToClient( UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nTo, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_PROXY, nTo, anTrace, usEventID, data );
}

template < class T >
void KGSASBillingDBThread::SendToBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSASBillingDBThread::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_LOG_DB, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

#endif // SERV_COUNTRY_TH