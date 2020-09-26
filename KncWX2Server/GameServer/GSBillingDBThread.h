#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 13  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "BillManager.h"

class KGSBillingDBThread : public KSubThread
{
	DeclareDBThread( KGSBillingDBThread );
	DeclareSPProfiler;
	DeclPfID;

public:
	KGSBillingDBThread();
	virtual ~KGSBillingDBThread();

protected:
	// derived from KSubThread
	virtual inline void ProcessEvent( const KEventPtr& spEvent );

	// packet send function	
	template < class T > void SendToBillManager( unsigned short usEventID, const T& data );
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );

	void SendToBillManager( unsigned short usEventID );
	void SendToServer( unsigned short usEventID );
	void SendToUser( UidType nTo, unsigned short usEventID );

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	enum CASH_SHOP_RELEASE_TICK_TYPE
	{
		CSRTT_NORMAL			= 0,	/// 캐시샵 목록 변경 tick
		CSRTT_BONUS_EVENT	= 1,	/// 캐시샵 보너스 캐쉬 이벤트 tick
#ifdef SERV_CONTENT_MANAGER_INT
		CSRTT_CASHSHOP_ON_OFF	= 2,	// 캐시샵 OnOff Tick 
#endif SERV_CONTENT_MANAGER_INT
	};

	int DoQuery_GetReleaseTick( OUT std::map<int, int> & mapReleaseTick );
#else
	int DoQuery_GetReleaseTick( OUT int& iReleaseTick );
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	
	int DoQuery_GetProductList( OUT std::map< int, KBillProductInfo >& mapBillProductInfo );
	int DoQuery_GetProductAttributeList( OUT std::map< int, KBillProductAttribute >& mapBillProductAttribute );
	int DoQuery_GetPackageInfoList( OUT std::map< int, KBillPackageInfo >& mapBillPackageInfo );
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	void DoQuery_GetCheckBonusEvent( OUT std::vector<std::wstring> &vecStartEvent,  OUT std::vector<std::wstring> &vecEndEvent);
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
	int DoQuery_GetProductSalePeriodList( OUT std::map< int, KBillProductInfo >& mapBillProductInfo );
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION

	//{{ 2011. 03. 03	최육사	중국 서버 빌링. 사내 버전도 이거 사용함
	int DoQuery_GetCashPointByUserUID( IN const UidType iUserUID, 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
										OUT KGlobalCashInfo& RemainCashInfo
#else
										OUT unsigned long& ulTotalCash 
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
										);
	//}}
#ifdef SERV_GLOBAL_CASH_PACKAGE
	int	DoQuery_GetSubPackageInfoList(IN const UidType iUserUID ,IN const int iServerGroupID,OUT std::vector<int>& _subPackageTrans, OUT std::vector<int>& _subPackageInfo);
#endif //SERV_GLOBAL_CASH_PACKAGE

	int DoQuery_GetMyOrderList( IN const UidType iUserUID, IN const int iServerGroupID, OUT std::vector< KBillOrderInfo >& vecOrderInfo );
	

	//2012.02.16 lygan_조성욱 // 아이템을 구매하기 전에 해당 아이템이 구매 제한 조건에 충족하는지 확인용
	int DoQuery_Buy_Product_CheckCondition( IN const std::wstring& wstrOrderID, 
							IN const std::wstring& wstrTotalOrderID, 
							IN const UidType iUserUID,
							IN const std::wstring& wstrUserID,
							IN const int iServerGroupID,
							IN const int iChannelingCode,
							IN const UidType iUnitUID,
							IN const char cLevel,
							IN const int iProductNo, 
							IN const int iQuantity,
							IN const int iPrice, 
							IN const bool bItemKeep,
							IN const UidType iToUserUID,
							IN const UidType iToUnitUID,
							IN const std::wstring& wstrGiftMessage,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
							IN const int iCashType
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
							);

	int DoQuery_Gift_Item_CheckCondition( IN const std::wstring& wstrOrderID, 
							IN const std::wstring& wstrTotalOrderID, 
							IN const UidType iUserUID,
							IN const std::wstring& wstrUserID,
							IN const int iServerGroupID,
							IN const int iChannelingCode,
							IN const UidType iUnitUID,
							IN const char cLevel,
							IN const int iProductNo, 
							IN const int iQuantity,
							IN const int iPrice, 
							IN const bool bItemKeep,
							IN const UidType iToUserUID,
							IN const UidType iToUnitUID,
							IN const std::wstring& wstrGiftMessage,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
							IN const int iCashType
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
							);

	// 구매와 선물 캐시 차감전에 안전상의 이유로 로그를 남김 (복구 확인 등)
	int DoQuery_Insert_Buy_Product_Log( IN const std::wstring& wstrOrderID, 
							IN const std::wstring& wstrTotalOrderID, 
							IN const UidType iUserUID,
							IN const std::wstring& wstrUserID,
							IN const int iServerGroupID,
							IN const int iChannelingCode,
							IN const UidType iUnitUID,
							IN const char cLevel,
							IN const int iProductNo, 
							IN const int iQuantity,
							IN const int iPrice, 
							IN const bool bItemKeep,
							IN const UidType iToUserUID,
							IN const UidType iToUnitUID,
							IN const std::wstring& wstrGiftMessage,
					#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
							IN const int iCashType
					#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
							);

	// 캐쉬 아이템 구매
	int DoQuery_BuyProduct( IN const std::wstring& wstrOrderID, 
							IN const std::wstring& wstrTotalOrderID, 
							IN const UidType iUserUID,
							IN const std::wstring& wstrUserID,
							IN const int iServerGroupID,
							IN const int iChannelingCode,
							IN const UidType iUnitUID,
							IN const char cLevel,
							IN const int iProductNo, 
							IN const int iQuantity,
							IN const int iPrice, 
							IN const bool bItemKeep,
							IN const UidType iToUserUID,
							IN const UidType iToUnitUID,
							IN const std::wstring& wstrGiftMessage,
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
							IN const int iCashType,
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
							OUT int& iTotalCash,
							OUT UidType& iTransNo
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
							,OUT KGlobalCashInfo& RemainCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
							);


	// 캐쉬 인벤토리에서 아이템 가져오기
	int DoQuery_PickUpProductInMyCashInventory( IN const UidType iTransNo, 
												IN const UidType iUserUID, 
												IN const int iServerGroupID, 
												IN const UidType iUnitUID );
#ifdef SERV_GLOBAL_CASH_PACKAGE
	// 캐쉬 인벤토리에서  패키지 아이템 가져오기
	int DoQuery_PickUpPACKAGEProductInMyCashInventory( IN const UidType iTransNo, 
		IN const UidType iUserUID, 
		IN const int iServerGroupID, 
		IN const UidType iUnitUID,
		IN const int iProductPackageNUM);

#endif //SERV_GLOBAL_CASH_PACKAGE


	// 쿠폰 시스템 개편 (쿠폰 KOG 관리)
	int UseCoupon( IN KEBILL_USE_COUPON_REQ kPacket, OUT std::vector< KBillCouponItemInfo > &vecItemInfo );

	//////////////////////////////////////////////////////////////////////////
	// 쿠폰 시스템 개편 (쿠폰 퍼블리셔 관리)

	// 트랜잭션 시리얼넘버 생성
	int DoQuery_TransactionNumberServerGet( OUT __int64& iTranNo, OUT time_t& iTime );	
	void MakeTID(__int64 iTransactionNo, time_t iTime, std::wstring& wstrTransaction);

	// 쿠폰 주문 등록
	int DoQuery_CouponOrder( IN const std::wstring& wstrCouponID,
		IN const std::wstring& wstrOrderID,
		IN const UidType iUserUID,
		IN const std::wstring& wstrUserID,
		IN const int iServerGroupID,
		IN const UidType iUnitUID,
		IN const char cLevel,
		IN const std::wstring& wstrUserIP,
		IN const char cOrderType = 1 );		// 중국 제외 쿠폰 OrderType 기본값 1

	// 쿠폰 주문 완료
	int DoQuery_CouponOrderResult( IN const std::wstring& wstrOrderID,
		IN const int iOrderResult,
		IN int& iChargeItem,
		IN int& iChargeCash,
		IN int& iChargeBonus,
		OUT int& iTotalCash );
	//////////////////////////////////////////////////////////////////////////

	// event handler
	DECL_ON_FUNC( EBILL_CHECK_PRODUCT_INFO_REQ );
	//{{ 2011. 03. 03	최육사	중국 서버 빌링. 사내 버전 해당 플로우 사용
	DECL_ON_FUNC( EBILL_CHECK_BALANCE_REQ );
	//}}
	_DECL_ON_FUNC( EBILL_INVENTORY_INQUIRY_REQ, KEGS_BILL_INVENTORY_INQUIRY_REQ );

	// 구매 전 구매 가능 조건들 체크
	_DECL_ON_FUNC( EBILL_PREPARE_BUY_PRODUCT_REQ, KEBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EBILL_BUY_PRODUCT_REQ );
	DECL_ON_FUNC( EBILL_PICK_UP_REQ );

	_DECL_ON_FUNC( EBILL_PREPARE_GIFT_ITEM_REQ, KEBILL_GIFT_ITEM_REQ );
	DECL_ON_FUNC( EBILL_GIFT_ITEM_REQ );


	// 쿠폰 시스템 개편 (쿠폰 KOG 관리)
	DECL_ON_FUNC( EBILL_USE_COUPON_REQ );

	// 쿠폰 시스템 개편 (쿠폰 퍼블리셔 관리)
	_DECL_ON_FUNC( EBILL_USE_COUPON_RESERVE_REQ, KEBILL_USE_COUPON_REQ );
	DECL_ON_FUNC( EBILL_USE_COUPON_RESULT_REQ );

#ifdef SERV_EVENT_BUY_FAKE_ITEM
	DECL_ON_FUNC( EBILL_CHECK_BUY_FAKE_ITEM_REQ );
#endif //SERV_EVENT_BUY_FAKE_ITEM

	//{{ 2013. 09. 24	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
	DECL_ON_FUNC( EBILL_REWARD_COUPON_JP_EVENT_REQ );
#endif SERV_RELAY_DB_CONNECTION
	//}}

#ifdef SERV_CONTENT_MANAGER_INT
	DECL_ON_FUNC( DBE_GET_CASHSHOP_ON_OFF_INFO_REQ );
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_COUNTRY_PH
	DECL_ON_FUNC( EBILL_GARENA_PREPARE_PRESENT_CHECK_REQ );
#endif // SERV_COUNTRY_PH
};

template < class T >
void KGSBillingDBThread::SendToBillManager( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_KOG_BILLING_MANAGER, NULL, usEventID, data );
	SiKBillManager()->QueueingEvent( spEvent );

}

template < class T >
void KGSBillingDBThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_KOG_BILLING_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSBillingDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_KOG_BILLING_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}
