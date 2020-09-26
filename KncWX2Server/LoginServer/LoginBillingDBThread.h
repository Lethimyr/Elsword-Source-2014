#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
#include "odbc/DBUtil.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//#include "BillManager.h"

class KLoginBillingDBThread : public KSubThread
{
	DeclareDBThread( KLoginBillingDBThread );
	DeclareSPProfiler;
	DeclPfID;

public:
	KLoginBillingDBThread();
	virtual ~KLoginBillingDBThread();

protected:
	// derived from KSubThread
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// packet send function	
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );

	//void SendToBillManager( unsigned short usEventID );
	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );
	void SendToClient( UidType nTo, unsigned short usEventID );

	// 트랜잭션 시리얼넘버 생성
	int DoQuery_TransactionNumberServerGet( OUT __int64& iTranNo, OUT time_t& iTime );
	void MakeTID( __int64 iTransactionNo, time_t iTime, std::wstring& wstrTransaction
#ifdef SERV_COUNTRY_CN
		, bool bCoupon = false
#endif SERV_COUNTRY_CN
		);

	// 캐시 전환 주문 등록
	int DoQuery_BillOrderGameServer( IN const std::wstring& wstrOrderID,
										IN const UidType iUserUID,
										IN const std::wstring& wstrUserID,
										IN const int iServerGroupID,
										IN const UidType iUnitUID,
										IN const char cLevel,
										IN const int iChargeCash,
										IN const std::wstring& wstrUserIP,
										IN const char cOrderType
#if defined( SERV_COUNTRY_PH ) || defined( SERV_ALL_RENEWAL_SP )
										, OUT __int64&	i64TransactionID
#endif //( SERV_COUNTRY_PH ) || ( SERV_ALL_RENEWAL_SP )
										);

	// 캐시 전환 주문 완료
	int DoQuery_BillOrderResult( IN const std::wstring& wstrOrderID,
									IN const int iOrderResult,
									OUT int& iTotalCash,
									OUT int& iChargeCash,
									OUT int& iChargeBonus,
									OUT int& iBonusItem,
									OUT UidType& iUnitUID 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
									, OUT KGlobalCashInfo& kRemainCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
									);

	// 쿠폰 주문 등록
	int DoQuery_CouponOrder( IN const std::wstring& wstrCouponID,
		IN const std::wstring& wstrOrderID,
		IN const UidType iUserUID,
		IN const std::wstring& wstrUserID,
		IN const int iServerGroupID,
		IN const UidType iUnitUID,
		IN const char cLevel,
		IN const std::wstring& wstrUserIP,
		IN const char cOrderType );

	// 쿠폰 주문 완료
	int DoQuery_CouponOrderResult( IN const std::wstring& wstrOrderID,
		IN const int iOrderResult,
		IN int& iChargeItem,
		IN int& iChargeCash,
		IN int& iChargeBonus,
		OUT int& iTotalCash );

	int GetReleaseTick( OUT int& iReleaseTick );
	int GetProductInfo( OUT std::map< int, KBillProductInfo >& mapBillProductInfo );
	int GetProductAttribute( OUT std::map< int, KBillProductAttribute >& mapBillProductAttribute );
	int GetPackageInfo( OUT std::map< int, KBillPackageInfo >& mapBillPackageInfo );
	int GetOrderInfo( IN UidType iUserUID, OUT std::vector< KBillOrderInfo >& vecOrderInfo );
	int GetOrderInfoByOrderID( IN UidType iUserUID, IN const std::wstring& wstrOrderID, OUT std::vector< KBillOrderInfo >& vecOrderInfo );
	int InsertCashInventory( IN const std::wstring& wstrOrderNo, IN int iProductNo, IN UidType iUserUID, IN int iPrice, IN KBillOrderInfo::BUY_CASH_ITEM_TYPE eType, IN UidType iReceiverUserUID = 0, IN UidType iSenderUnitUID = 0, IN std::wstring wstrPresentMessage = L"" );
	int InsertCashLog( IN int iOrderIndex, IN UidType iUserUID, IN UidType iUnitUID, IN int iProductNoIn, OUT int& iProductNoOut );

#ifdef SERV_COUNTRY_PH
	//{{ 2011. 03. 03	최육사	중국 서버 빌링. 사내 버전도 이거 사용함
	int DoQuery_GetCashPointByUserUID( IN const UidType iUserUID, 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		OUT KGlobalCashInfo& RemainCashInfo
#else
		OUT unsigned long& ulTotalCash 
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		);
	//}}

	int DoQuery_SetChangeGameCurrencyFailLog(IN KEJSON_GN_CHANGE_GAME_CURRENCY_ACK kPaket);

#endif //SERV_COUNTRY_PH

	// event handler
#ifdef	SERV_COUNTRY_CN
	DECL_ON_FUNC( EBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ );
#endif	SERV_COUNTRY_CN
	DECL_ON_FUNC( EBILL_EXCHANGE_CASH_RESERVE_REQ );
	DECL_ON_FUNC( EBILL_EXCHANGE_CASH_REQ );

	_DECL_ON_FUNC( EBILL_USE_COUPON_RESERVE_REQ, KEBILL_USE_COUPON_REQ );
	DECL_ON_FUNC( EBILL_USE_COUPON_RESULT_REQ );

#ifdef SERV_CBT2_CASH_DISTRIBUTION_EVENT
	DECL_ON_FUNC( EBILL_GET_TRANS_FOR_CBT2_CASH_DISTRIBUTION_EVENT_REQ );
#endif SERV_CBT2_CASH_DISTRIBUTION_EVENT

#ifdef SERV_COUNTRY_PH
	DECL_ON_FUNC( EBILL_GN_CHANGE_GAME_CURRENCY_REQ );
	DECL_ON_FUNC( EBILL_LOGIN_CHECK_BALANCE_REQ );
	_DECL_ON_FUNC( EBILL_GN_CHANGE_GAME_CURRENCY_FAIL_NOT, KEJSON_GN_CHANGE_GAME_CURRENCY_ACK );
	_DECL_ON_FUNC( EBILL_GN_TRANSACTION_CHECK_REQ, KEJSON_GN_TRANSACTION_REQ );
#endif //SERV_COUNTRY_PH

};

template < class T >
void KLoginBillingDBThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginBillingDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KLoginBillingDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}

