#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"
#include "KncException.h"

#include "BillManager.h"
#include "BaseServer.h"

#ifdef SERV_COUNTRY_EU

#include "SoapGameForge/soapH.h" // or whatever it is called, you must already have it

class KGameForgeBilling : public KSubThread
{

	struct KWebMethod
	{
		int m_iSoapAction;
		std::string m_strMethodPath;

		KWebMethod()
		{
			m_iSoapAction = 0;
			m_strMethodPath = std::string();
			m_strMethodPath = "";
		};

		KWebMethod( const KWebMethod& t )
		{
			*this = t;
		};

		KWebMethod& operator=( const KWebMethod& t )
		{
			m_iSoapAction = t.m_iSoapAction;
			m_strMethodPath = t.m_strMethodPath;

			return *this;
		};
	};

	DeclPfID;
	_DeclareException;
    



public:
#   undef _ENUM
#   define _ENUM( id ) id,
	enum
	{
#   include "GameForgeBilling_def.h"
	};

public:
	KGameForgeBilling(std::string WsdlFile);
	~KGameForgeBilling(void);

public:
	virtual void Run();	// Soap Initialize 를 넣기 위해 재정의함 //

	void SoapInitialize();
	void SoapUninitialize();

	static void AddWebMethod( int iSoapAction, std::string szMethod );
	bool GetWebMethod( int iSoapAction, KWebMethod& kInfo );

	// packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data );
	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );
	void SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID );

	//----------------------//
	// Process Event handle //
	//----------------------//
	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	// event handler
	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );
	
	DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EBILL_GIFT_ITEM_REQ );

	DECL_ON_FUNC( EBILL_USE_COUPON_REQ );
	//}}

	// Other functions
	
	int GetCashPoint( IN const std::wstring& wstrUserID, OUT int& iCashPoint );
	int ReduceCashPoint( IN const std::wstring wstrOrderID, IN const std::wstring& wstrUserID, IN int iServerGroupID, IN UidType characterID, 
		IN int countLists, IN const std::string& products, OUT int& iCashPoint );

	int UseCoupon( IN const int iServerGroupID, IN const std::wstring& wstrUserID, IN const std::wstring& wstrNickName, IN const std::wstring& wstrCardPassword, IN const std::wstring& wstrIP,
		OUT int& iEventID, OUT int& iGiftID );
	int GetCouponItem( IN const int iEventID, IN const int iGiftID,
		OUT std::vector< KBillCouponItemInfo >& vecGASHCouponItemInfo );
	int InGameLessGPoint( IN const std::wstring& wstrTransID, IN int iSvr, IN const std::wstring& wstrUserID, IN const std::wstring& wstrNickName, IN int iPoint, IN int iItemID,
		IN int iProductID, IN const std::wstring& wstrMemo );
	



protected:
	struct soap* p_gf_soap;

	std::string m_strWsdlPath;

	static std::map< int, KWebMethod >  ms_mapWebMethodPath;
};


template < class T >
void KGameForgeBilling::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGameForgeBilling::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}


template < class T >
void KGameForgeBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nUserUID, -1 };
	KncSend( PI_GS_PUBLISHER_BILLING, 0, PI_GS_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}

#endif //SERV_COUNTRY_EU