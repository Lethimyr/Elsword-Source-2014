#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#ifdef SERV_COUNTRY_TWHK

#include "BillManager.h"

class KGSGASHBillingThread : public KSubThread
{
    DeclareDBThread( KGSGASHBillingThread );
    DeclPfID;

public:
    KGSGASHBillingThread();
    virtual ~KGSGASHBillingThread();

protected:
    // derived from KSubThread
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// Client Direct로 가는 거 없애고 GSUser 거치도록 함
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    template < class T > void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );
    void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID );
	void SendToLogDB( unsigned short usEventID );

    int GetCashPoint( IN const std::wstring& wstrUserID, OUT int& iCashPoint );
    int UseCoupon( IN const int iServerGroupID, IN const std::wstring& wstrUserID, IN const std::wstring& wstrNickName, IN const std::wstring& wstrCardPassword, IN const std::wstring& wstrIP,
                   OUT int& iEventID, OUT int& iGiftID );
    int GetCouponItem( IN const int iEventID, IN const int iGiftID,
                       OUT std::vector< KBillCouponItemInfo >& vecGASHCouponItemInfo );
    int InGameLessGPoint( IN const std::wstring& wstrTransID, IN int iSvr, IN const std::wstring& wstrUserID, IN const std::wstring& wstrNickName, IN int iPoint, IN int iItemID,
        IN int iProductID, IN const std::wstring& wstrMemo );
	int ReportHackUser( IN UidType iUserUID, IN const std::wstring& wstrUserID, IN UidType iUnitUID, IN const std::wstring& wstrNickName, IN UidType m_iServerUID, IN const std::string& strIP );
#ifdef SERV_BILLING_ADD_GASH_POINT
	int InsertUseGashLog( IN KEGASH_USE_GASH_LOG_NOT kPacket);
#endif
    // event handler
    DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );

	//{{ 성공 시 KOGBillingDB로 바로 던짐
    DECL_ON_FUNC( EBILL_USE_COUPON_REQ );
    DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EBILL_GIFT_ITEM_REQ );
	//}}

	DECL_ON_FUNC( EPUBLISHER_REPORT_HACK_USER_NOT );

#ifdef SERV_BILLING_ADD_GASH_POINT
	DECL_ON_FUNC( EGASH_USE_GASH_LOG_NOT );
#endif

#ifdef SERV_GOLD_TICKET
	DECL_ON_FUNC( EPUBLISHER_CHARGE_POINT_REQ );
#endif //SERV_GOLD_TICKET

public:
protected:
};

template < class T >
void KGSGASHBillingThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSGASHBillingThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KGSGASHBillingThread::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nUserUID, -1 };
    KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

template < class T >
void KGSGASHBillingThread::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING_DB, 0, PI_GS_LOG_DB, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

#endif //SERV_COUNTRY_TWHK