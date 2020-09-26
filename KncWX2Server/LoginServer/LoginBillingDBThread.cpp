#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "LoginBillingDBThread.h"
#include "LoginServer.h"
#include "NetError.h"

#ifdef SERV_COUNTRY_CN
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantBillingManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantCouponManager.h"
#endif // SERV_COUNTRY_CN

#ifdef SERV_COUNTRY_PH
#include "../Common/OnlyGlobal/AuthAndBilling/PH/GarenaBillingServer.h"
#endif //SERV_COUNTRY_PH

ImplementDBThread( KLoginBillingDBThread );
ImplPfID( KLoginBillingDBThread, PI_LOGIN_KOG_BILLING_DB );

#define CLASS_TYPE KLoginBillingDBThread

KLoginBillingDBThread::~KLoginBillingDBThread()
{
}

void KLoginBillingDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	KSerializer ks;
	ks.BeginReading( &spEvent_->m_kbuff );

	switch( spEvent_->m_usEventID )
	{
#ifdef	SERV_COUNTRY_CN
		CASE( EBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ );
#endif	SERV_COUNTRY_CN

		CASE( EBILL_EXCHANGE_CASH_RESERVE_REQ );
		CASE( EBILL_EXCHANGE_CASH_REQ );

		_CASE( EBILL_USE_COUPON_RESERVE_REQ, KEBILL_USE_COUPON_REQ );
		CASE( EBILL_USE_COUPON_RESULT_REQ );

#ifdef SERV_CBT2_CASH_DISTRIBUTION_EVENT
		CASE( EBILL_GET_TRANS_FOR_CBT2_CASH_DISTRIBUTION_EVENT_REQ );
#endif SERV_CBT2_CASH_DISTRIBUTION_EVENT
#ifdef SERV_COUNTRY_PH
		CASE( EBILL_GN_CHANGE_GAME_CURRENCY_REQ );
		CASE( EBILL_LOGIN_CHECK_BALANCE_REQ );
		_CASE( EBILL_GN_CHANGE_GAME_CURRENCY_FAIL_NOT, KEJSON_GN_CHANGE_GAME_CURRENCY_ACK );
		_CASE( EBILL_GN_TRANSACTION_CHECK_REQ, KEJSON_GN_TRANSACTION_REQ );
#endif //SERV_COUNTRY_PH

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
}

//void KLoginBillingDBThread::SendToBillManager( unsigned short usEventID )
//{
//	SendToBillManager( usEventID, char() );
//}

void KLoginBillingDBThread::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KLoginBillingDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

#ifdef	SERV_COUNTRY_CN
IMPL_ON_FUNC( EBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ )
{
	KEBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kCheckGiantBalanceReq = kPacket_.m_kCheckGiantBalanceReq;

	__int64 iTransactionNo = 0;
	time_t iTime = 0;

	kPacket.m_iOK = DoQuery_TransactionNumberServerGet( iTransactionNo, iTime );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_05;
			break;
		}

		START_LOG( cerr, L"빌링 트랜잭션 번호 생성 실패" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		goto end_proc;
	}

	MakeTID( iTransactionNo, iTime, kPacket.m_wstrTransaction );

end_proc:
	SendToUser( LAST_SENDER_UID, EBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK, kPacket );
}
/*
IMPL_ON_FUNC( EBILL_GET_TRANS_FOR_EXCHANGE_REQ )
{
	KEBILL_GET_TRANS_FOR_EXCHANGE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kExchangeCashReq = kPacket_.m_kExchangeCashReq;

	kPacket.m_iOK = DoQuery_TransactionNumberServerGet( kPacket.m_iTransactionNo );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"빌링 트랜잭션 번호 생성 실패" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, EBILL_GET_TRANS_FOR_EXCHANGE_ACK, kPacket );
}
*/
#endif	SERV_COUNTRY_CN

IMPL_ON_FUNC( EBILL_EXCHANGE_CASH_RESERVE_REQ )
{
	KEBILL_EXCHANGE_CASH_RESERVE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kPacketReq = kPacket_;

	// 트랜잭션 생성
	__int64 iTransactionNo = 0;
	time_t iTime = 0;
	kPacket.m_iOK = DoQuery_TransactionNumberServerGet( iTransactionNo, iTime );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_GIANT_BILLING_08;
			break;
		}

		START_LOG( cerr, L"빌링 트랜잭션 번호 생성 실패" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		goto end_proc;
	}

	MakeTID( iTransactionNo, iTime, kPacket.m_wstrTransaction );

#if defined( SERV_COUNTRY_PH ) || defined( SERV_ALL_RENEWAL_SP )
	__int64 i64TransactionID = 0LL;
#endif //( SERV_COUNTRY_PH ) || ( SERV_ALL_RENEWAL_SP )
	
	// 주문 등록
	kPacket.m_iOK = DoQuery_BillOrderGameServer( kPacket.m_wstrTransaction, 
		kPacket_.m_PurchaserInfo.m_iUserUID,
		kPacket_.m_PurchaserInfo.m_wstrUserName,
		kPacket_.m_PurchaserInfo.m_iServerGroupID,
		kPacket_.m_PurchaserInfo.m_iUnitUID,
		kPacket_.m_PurchaserInfo.m_ucLevel,
		kPacket_.m_iPoint,
		kPacket_.m_PurchaserInfo.m_wstrIP,
		1 
#if defined( SERV_COUNTRY_PH ) || defined( SERV_ALL_RENEWAL_SP )
		, i64TransactionID
#endif //( SERV_COUNTRY_PH ) || ( SERV_ALL_RENEWAL_SP )
		);

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"자이언트 캐시 전환 주문 등록 실패." )
			<< BUILD_LOG( kPacket.m_wstrTransaction )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrUserName )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iServerGroupID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_PurchaserInfo.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrIP )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_uiPublisherUID )
			<< BUILD_LOG( kPacket_.m_iPoint )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, EBILL_EXCHANGE_CASH_RESERVE_ACK, kPacket );
}

IMPL_ON_FUNC( EBILL_EXCHANGE_CASH_REQ )
{
	KEBILL_EXCHANGE_CASH_ACK kPacket;

	kPacket.m_iOK = DoQuery_BillOrderResult( kPacket_.m_wstrTransaction, 
											 kPacket_.m_iRet,
											 kPacket.m_iTotalCash,
											 kPacket.m_iChargeCash,
											 kPacket.m_iChargeBonus,
											 kPacket.m_iBonusItem,
											 kPacket.m_iUnitUID
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
											 , kPacket.m_RemainCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
											 );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"자이언트 캐시 전환 DB 기록 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;

		goto end_proc;
	}
	//2011.07.26 lygan_조성욱 // 잔액부족 에러 메시지 출력
	if(kPacket_.m_iRet == -3)
	{
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_02;
		goto end_proc;
	}


	/*
	if( kPacket_.m_iPoint != kPacket.m_iChargeCash )
	{
		START_LOG( cerr, L"충전 예정 금액과 실제 충전 금액이 다름." )
			<< BUILD_LOG( kPacket_.m_iPoint )
			<< BUILD_LOG( kPacket.m_iChargeCash )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_08;
	}
	*/

	if( kPacket_.m_iPoint < 0 )
	{
		START_LOG( cerr, L"충전 금액이 마이너스." )
			<< BUILD_LOG( kPacket_.m_iPoint )
			<< BUILD_LOG( kPacket.m_iChargeCash )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_08;
	}

end_proc:
	SendToGSUser( kPacket_.m_iGSUID, kPacket_.m_iUserUID, EBILL_EXCHANGE_CASH_ACK, kPacket );
}

_IMPL_ON_FUNC( EBILL_USE_COUPON_RESERVE_REQ, KEBILL_USE_COUPON_REQ )
{
	KEBILL_USE_COUPON_RESERVE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kPacketReq = kPacket_;

	// 트랜잭션 생성
	__int64 iTransactionNo = 0;
	time_t iTime = 0;
	kPacket.m_iOK = DoQuery_TransactionNumberServerGet( iTransactionNo, iTime );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_GIANT_BILLING_05;
			break;
		}

		START_LOG( cerr, L"빌링 트랜잭션 번호 생성 실패" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		goto end_proc;
	}

	MakeTID( iTransactionNo, iTime, kPacket.m_kPacketReq.m_wstrTransaction );

	// 주문 등록
	kPacket.m_iOK = DoQuery_CouponOrder( kPacket_.m_wstrSerialCode,
		kPacket.m_kPacketReq.m_wstrTransaction, 
		kPacket_.m_PurchaserInfo.m_iUserUID,
		kPacket_.m_PurchaserInfo.m_wstrUserName,
		kPacket_.m_PurchaserInfo.m_iServerGroupID,
		kPacket_.m_PurchaserInfo.m_iUnitUID,
		kPacket_.m_PurchaserInfo.m_ucLevel,
		kPacket_.m_PurchaserInfo.m_wstrIP,
#ifdef SERV_COUNTRY_CN
		KGiantBillingPacket::GB_RT_USE_ITEM_CARD //2011.10.28 lygan_조성욱 // 1로 하드코딩되어 있던거 아이템 카드 값으로 수정
#else //SERV_COUNTRY_CN
		1	// 중국 제외하고는 그냥 1로 타입 고정함
#endif //
		);

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"자이언트 쿠폰 주문 등록 실패." )			
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrSerialCode )
			<< BUILD_LOG( kPacket.m_kPacketReq.m_wstrTransaction )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrUserName )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iServerGroupID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_PurchaserInfo.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrIP )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, EBILL_USE_COUPON_RESERVE_ACK, kPacket );
}

IMPL_ON_FUNC( EBILL_USE_COUPON_RESULT_REQ )
{
	KEBILL_USE_COUPON_RESULT_ACK kPacket;

	kPacket.m_iOK = DoQuery_CouponOrderResult( kPacket_.m_wstrTransaction, 
		kPacket_.m_iRet,
		kPacket_.m_iChargeItem,
		kPacket_.m_iChargeCash,
		kPacket_.m_iChargeBonus,
		kPacket.m_iTotalCash );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"자이언트 쿠폰 사용 DB 기록 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;
	}

	switch( kPacket_.m_iRet )
	{
	case KEBILL_USE_COUPON_REQ::GB_RCT_FAIL:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_09;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ACCOUNT_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_01;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_CASH_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_02;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_NOT_EXIST:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_03;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_NOT_AREA:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_04;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ITEM_CARD_DUPLICATE:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_05;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ZONE_CARD_ID_ERROR:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_06;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_ZONE_CARD_PASSWORD_ERROR:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_07;
		break;
	case KEBILL_USE_COUPON_REQ::GB_RCT_DB_ERROR:
		kPacket.m_iOK = NetError::ERR_GIANT_BILLING_08;
		break;
	}

	START_LOG( clog, L"쿠폰 사용 결과" )
		<< BUILD_LOG( kPacket_.m_wstrTransaction )
		<< BUILD_LOG( kPacket_.m_iRet )
		<< BUILD_LOG( kPacket_.m_iChargeItem )
		<< BUILD_LOG( kPacket_.m_iChargeCash )
		<< BUILD_LOG( kPacket_.m_iChargeBonus )
		<< BUILD_LOG( kPacket.m_iOK )
		<< BUILD_LOG( NetError::GetErrStr(kPacket.m_iOK) )
		<< BUILD_LOG( kPacket.m_iTotalCash )
		<< END_LOG;

	SendToGSUser( kPacket_.m_iGSUID, kPacket_.m_iUserUID, EBILL_USE_COUPON_RESULT_ACK, kPacket );
}

#ifdef SERV_CBT2_CASH_DISTRIBUTION_EVENT
IMPL_ON_FUNC( EBILL_GET_TRANS_FOR_CBT2_CASH_DISTRIBUTION_EVENT_REQ )
{
	KEBILL_GET_TRANS_FOR_CBT2_CASH_DISTRIBUTION_EVENT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	
	__int64 iTransactionNo = 0;
	time_t iTime = 0;

	int iRet = DoQuery_TransactionNumberServerGet( iTransactionNo, iTime );

	if( iRet != NetError::NET_OK )
	{
		switch( iRet )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_05;
			break;
		}

		START_LOG( cerr, L"빌링 트랜잭션 번호 생성 실패" )
			<< BUILD_LOG( iRet )
			<< END_LOG;

		goto end_proc;
	}

	kPacket.m_iOK = NetError::NET_OK;
	MakeTID(iTransactionNo, iTime, kPacket.m_wstrTransaction);

end_proc:
	SendToGSUser( kPacket_.m_iGSUID, kPacket_.m_iUserUID, EBILL_GET_TRANS_FOR_CBT2_CASH_DISTRIBUTION_EVENT_ACK, kPacket );
}
#endif SERV_CBT2_CASH_DISTRIBUTION_EVENT

#ifdef SERV_COUNTRY_PH
IMPL_ON_FUNC( EBILL_GN_CHANGE_GAME_CURRENCY_REQ )
{
	KEBILL_GN_CHANGE_GAME_CURRENCY_ACK kPacketAck;

	CTime tCurrent;
	std::string strTransaction = "";
	std::wstring wstrTransaction = L"";

	int iOK = NetError::ERR_ODBC_01;

	kPacketAck.m_bIsOffLine = false;	

	if (kPacket_.m_PurchaserInfo.m_iUnitUID == 0)
	{
		kPacketAck.m_bIsOffLine = true;
	}

	if(kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount < 0) // 2013.08.06 lygan_조성욱 // 전환 할 금액이 음수면 문제가 있는거다
	{
		kPacketAck.m_kGarenaACK.m_iResult = 2; // 2. Add game currency failed
		tCurrent  = CTime::GetCurrentTime();
		kPacketAck.m_kGarenaACK.m_strDateTime = (CStringA)tCurrent.Format(_T("%Y-%m-%d %H:%M:%S"));
		kPacketAck.m_kGarenaACK.m_uiShell_Amount = kPacket_.m_kGarenaREQ.m_uiShell_Amount;
		kPacketAck.m_kGarenaACK.m_uiGameCurrency_BeforeAmount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;
		kPacketAck.m_kGarenaACK.m_iSessionUID = kPacket_.m_kGarenaREQ.m_iSessionUID;
		kPacketAck.m_kGarenaACK.m_uiGarenaUID = kPacket_.m_kGarenaREQ.m_uiGarenaUID;
		kPacketAck.m_kGarenaACK.m_strGarenaTransactionID = kPacket_.m_kGarenaREQ.m_strGarenaTransactionID;

		KEJSON_GN_CHANGE_GAME_CURRENCY_ACK KPacketFaillogNOT;
		KPacketFaillogNOT = kPacketAck.m_kGarenaACK;
		
		DoQuery_SetChangeGameCurrencyFailLog(IN KPacketFaillogNOT);

		START_LOG( cerr, L"전환 할 금액이 음수면 문제가 있는거다" )
			<< BUILD_LOG( kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount )
			<< END_LOG;
		goto end_proc;
	}


	wstrTransaction = KncUtil::toWideString(kPacket_.m_kGarenaREQ.m_strGarenaTransactionID);


	//// 트랜잭션 생성
	//__int64 iTransactionNo = 0;
	//time_t iTime = 0;
	//iOK = DoQuery_TransactionNumberServerGet( iTransactionNo, iTime );

	//if( iOK != NetError::NET_OK )
	//{
	//	switch( iOK )
	//	{
	//	case -1:
	//		iOK = NetError::ERR_GIANT_BILLING_08;
	//		break;
	//	}

	//	START_LOG( cerr, L"빌링 트랜잭션 번호 생성 실패" )
	//		<< BUILD_LOG( iOK )
	//		<< END_LOG;

	//	kPacketAck.m_kGarenaACK.m_iResult = 99;
	//	tCurrent  = CTime::GetCurrentTime();
	//	kPacketAck.m_kGarenaACK.m_strDateTime = (CStringA)tCurrent.Format(_T("%Y-%m-%d %H:%M:%S"));
	//	kPacketAck.m_kGarenaACK.m_uiShell_Amount = kPacket_.m_kGarenaREQ.m_uiShell_Amount;
	//	kPacketAck.m_kGarenaACK.m_uiGameCurrency_BeforeAmount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;

	//	goto end_proc;
	//}

	//MakeTID( iTransactionNo, iTime, wstrTransaction );


	__int64 i64TransactionID = 0LL;
	iOK = NetError::ERR_ODBC_01;
	// 주문 등록
	iOK = DoQuery_BillOrderGameServer( wstrTransaction, 
		kPacket_.m_PurchaserInfo.m_iUserUID,
		kPacket_.m_PurchaserInfo.m_wstrUserName,
		kPacket_.m_PurchaserInfo.m_iServerGroupID,
		kPacket_.m_PurchaserInfo.m_iUnitUID,
		kPacket_.m_PurchaserInfo.m_ucLevel,
		kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount,
		kPacket_.m_PurchaserInfo.m_wstrIP,
		1,
		i64TransactionID
		);

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Garena 캐시 전환 주문 등록 실패." )
			<< BUILD_LOG( wstrTransaction )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrUserName )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iServerGroupID )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_PurchaserInfo.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_wstrIP )
			<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_uiPublisherUID )
			<< BUILD_LOG( kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount )
			<< BUILD_LOG( i64TransactionID )
			<< END_LOG;

		kPacketAck.m_kGarenaACK.m_iResult = 99;
		tCurrent  = CTime::GetCurrentTime();
		kPacketAck.m_kGarenaACK.m_strDateTime = (CStringA)tCurrent.Format(_T("%Y-%m-%d %H:%M:%S"));
		kPacketAck.m_kGarenaACK.m_uiShell_Amount = kPacket_.m_kGarenaREQ.m_uiShell_Amount;
		kPacketAck.m_kGarenaACK.m_uiGameCurrency_BeforeAmount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;
		kPacketAck.m_kGarenaACK.m_iSessionUID = kPacket_.m_kGarenaREQ.m_iSessionUID;
		kPacketAck.m_kGarenaACK.m_int64GameTransactionID = i64TransactionID;
		kPacketAck.m_kGarenaACK.m_strGarenaTransactionID = kPacket_.m_kGarenaREQ.m_strGarenaTransactionID;
		kPacketAck.m_kGarenaACK.m_uiGarenaUID = kPacket_.m_kGarenaREQ.m_uiGarenaUID;
		kPacketAck.m_kGarenaACK.m_uiGameCurrency_Amount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;

		KEJSON_GN_CHANGE_GAME_CURRENCY_ACK KPacketFaillogNOT;
		KPacketFaillogNOT = kPacketAck.m_kGarenaACK;

		DoQuery_SetChangeGameCurrencyFailLog(IN KPacketFaillogNOT);

		goto end_proc;
	}


	int iChargeBonus = 0;
	int iBonusItem = 0;
	int	iTotalCash = 0;
	int iChargeCash = 0;

	UidType uidPurchaserUnit = 0;


	iOK = NetError::ERR_ODBC_01;
	iOK = DoQuery_BillOrderResult( wstrTransaction, 
		0,
		iTotalCash,
		iChargeCash,
		iChargeBonus,
		iBonusItem,
		uidPurchaserUnit
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		, kPacketAck.m_AddCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		);

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Garena 캐시 전환 DB 기록 실패." )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( NetError::GetErrStr( iOK ) )
			<< END_LOG;

		kPacketAck.m_kGarenaACK.m_iResult = 99;
		tCurrent  = CTime::GetCurrentTime();
		kPacketAck.m_kGarenaACK.m_strDateTime = (CStringA)tCurrent.Format(_T("%Y-%m-%d %H:%M:%S"));
		kPacketAck.m_kGarenaACK.m_uiShell_Amount = kPacket_.m_kGarenaREQ.m_uiShell_Amount;
		kPacketAck.m_kGarenaACK.m_uiGameCurrency_BeforeAmount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;
		kPacketAck.m_kGarenaACK.m_iSessionUID = kPacket_.m_kGarenaREQ.m_iSessionUID;
		kPacketAck.m_kGarenaACK.m_int64GameTransactionID = i64TransactionID;
		kPacketAck.m_kGarenaACK.m_strGarenaTransactionID = kPacket_.m_kGarenaREQ.m_strGarenaTransactionID;
		kPacketAck.m_kGarenaACK.m_uiGarenaUID = kPacket_.m_kGarenaREQ.m_uiGarenaUID;
		kPacketAck.m_kGarenaACK.m_uiGameCurrency_Amount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;

		KEJSON_GN_CHANGE_GAME_CURRENCY_ACK KPacketFaillogNOT;
		KPacketFaillogNOT = kPacketAck.m_kGarenaACK;

		DoQuery_SetChangeGameCurrencyFailLog(IN KPacketFaillogNOT);

		goto end_proc;
	}

	kPacketAck.m_kGarenaACK.m_iResult = 0;
	kPacketAck.m_kGarenaACK.m_int64GameTransactionID = i64TransactionID;
	kPacketAck.m_kGarenaACK.m_strGarenaTransactionID = kPacket_.m_kGarenaREQ.m_strGarenaTransactionID;
	kPacketAck.m_kGarenaACK.m_uiGameCurrency_Balance = static_cast<unsigned int>(iTotalCash);
	kPacketAck.m_kGarenaACK.m_uiGameCurrency_Amount = static_cast<unsigned int>(iChargeCash);
	kPacketAck.m_kGarenaACK.m_uiGarenaUID = kPacket_.m_kGarenaREQ.m_uiGarenaUID;
	tCurrent  = CTime::GetCurrentTime();
	kPacketAck.m_kGarenaACK.m_strDateTime = (CStringA)tCurrent.Format(_T("%Y-%m-%d %H:%M:%S"));
	kPacketAck.m_kGarenaACK.m_uiShell_Amount = kPacket_.m_kGarenaREQ.m_uiShell_Amount;
	kPacketAck.m_kGarenaACK.m_uiGameCurrency_BeforeAmount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;
	kPacketAck.m_kGarenaACK.m_iSessionUID = kPacket_.m_kGarenaREQ.m_iSessionUID;



end_proc:

	KEventPtr spEvent( new KEvent );
	spEvent->SetData(PI_LOGIN_GARENA_BILLING, NULL, EBILL_GN_CHANGE_GAME_CURRENCY_ACK, kPacketAck );

	SiKGarenaBillingServer()->QueueingEvent( spEvent );

	//SendToUser( LAST_SENDER_UID, EBILL_GN_CHANGE_GAME_CURRENCY_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_LOGIN_CHECK_BALANCE_REQ )
{
	KEBILL_LOGIN_CHECK_BALANCE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_ulBalance = 0;
	kPacket.m_iSessionUID = kPacket_.m_iSessionUID;

	kPacket.m_uiPublisherUID = kPacket_.m_uiPublisherUID;
	kPacket.m_wstrCharName = kPacket_.m_wstrCharName;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_GlobalCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_CASH] = 0;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


	kPacket.m_iOK = DoQuery_GetCashPointByUserUID( LAST_SENDER_UID
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		, kPacket.m_GlobalCashInfo 
#else // SERV_SUPPORT_SEVERAL_CASH_TYPES
		, kPacket.m_ulBalance
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		);

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_ulBalance = kPacket.m_GlobalCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_CASH];
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	KEventPtr spEvent( new KEvent );
	spEvent->SetData(PI_LOGIN_GARENA_BILLING, NULL, EBILL_LOGIN_CHECK_BALANCE_ACK, kPacket );

	SiKGarenaBillingServer()->QueueingEvent( spEvent );

	//SendToUser( LAST_SENDER_UID, EBILL_LOGIN_CHECK_BALANCE_ACK, kPacket );
	
}


_IMPL_ON_FUNC( EBILL_GN_CHANGE_GAME_CURRENCY_FAIL_NOT, KEJSON_GN_CHANGE_GAME_CURRENCY_ACK )
{
	DoQuery_SetChangeGameCurrencyFailLog(kPacket_);
}

_IMPL_ON_FUNC( EBILL_GN_TRANSACTION_CHECK_REQ, KEJSON_GN_TRANSACTION_REQ )
{


	KEJSON_GN_TRANSACTION_ACK kPacket;

	kPacket.m_iSessionUID = kPacket_.m_iSessionUID;

	std::wstring wstrTransaction = KncUtil::toWideString(kPacket_.m_strGarenaTransactionID);

	std::wstring wstrRegDate = L"";

	int iRet = NetError::ERR_ODBC_01;
	int iServerInfo = -1;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BillOrder_Publisher_GET", L"%d, N\'%s\'", % kPacket_.m_uiGarenaUID % wstrTransaction );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_int64GameTransactionID
			>> kPacket.m_strGarenaTransactionID
			>> kPacket.m_uiGarenaUID
			>> iServerInfo
			>> kPacket.m_uiShell_Amount
			>> kPacket.m_uiGameCurrency_Amount
			>> kPacket.m_iResult
			>> wstrRegDate
			);
			m_kODBC.EndFetch();

			if ( wstrRegDate.length() != 0 )
			{

				int size = ::WideCharToMultiByte( CP_ACP, 0, wstrRegDate.c_str(), (int) wstrRegDate.size(), NULL, 0, NULL, NULL );
				if ( size <= 0 )
				{
					kPacket.m_iResult = 2;
					goto end_proc;
				}

				CHAR*   szBuffer = (CHAR*) _alloca( ( size + 1 ) * sizeof( CHAR ) );
				if (szBuffer == NULL)
				{
					kPacket.m_iResult = 2;
					goto end_proc;
				}
				::WideCharToMultiByte( CP_ACP, 0, wstrRegDate.c_str(), (int) wstrRegDate.size(), szBuffer, size, NULL, NULL );
				szBuffer[ size ] = '\0';
				kPacket.m_strDateTime = szBuffer;

			}

			// 2013.09.29 lygan_조성욱 // kPacket.m_iResult 에 대해서 DB에서 값을 받아 올때는 에러 가 났을때의 값이지만 위에 위와 아래에 새롭게 할당 하는 부분은 가레나에 전달용 의미로 변경됨
			//////////////////////////////////////////////////////////////////////////
			// 가레나 측에 전달될 실패에 대한 정보
			//0 : 성공 유저 정보
			//1 : 실패 유저 정보
			//2 : 없는 유저
			//99 : DB 에러
			//////////////////////////////////////////////////////////////////////////
			
			if ( kPacket.m_iResult == 0) // 2013.09.29 lygan_조성욱 // 성공했을때는 가레나 UID 를 저장하고 있지 않기 때문에 요청 들어 온거 그대로 전해 준다. 가레나 TXID 로 조회 값은 유니크 하기 때문에 굳이 가레나 UID 를 체크 할 필요가 없다.
			{
				kPacket.m_uiGarenaUID = kPacket_.m_uiGarenaUID;
			}
			else if ( kPacket.m_iResult == 99 || kPacket.m_iResult == 2) // 2013.09.29 lygan_조성욱 // 여기서 kPacket.m_iResult  값을 비교 할 수 있는건 위에 문제 생겼을때 2라는 값을 할당하고 goto end_proc; 로 넘어가기 때문에 여기서 받은 kPacket.m_iResult 값은 디비에 저장된 에러 값이다.
			{
				kPacket.m_iResult = 1;
			}
	}
	else
	{
		kPacket.m_uiGarenaUID = kPacket_.m_uiGarenaUID;
		kPacket.m_strGarenaTransactionID = kPacket_.m_strGarenaTransactionID;
		kPacket.m_iResult = 2;
		kPacket.m_int64GameTransactionID = 0;
		kPacket.m_uiGameCurrency_Amount = 0;
		kPacket.m_uiShell_Amount = 0;
		iRet = NetError::ERR_VERIFY_04;

		goto end_proc;
	}

	iRet = NetError::NET_OK;


end_proc:

	if ( iRet ==  NetError::ERR_ODBC_01 )
	{
		kPacket.m_iResult = 99;
	}


	KEventPtr spEvent( new KEvent );
	spEvent->SetData(PI_LOGIN_GARENA_BILLING, NULL, EJSON_GN_TRANSACTION_ACK, kPacket );

	SiKGarenaBillingServer()->QueueingEvent( spEvent );


	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG(kPacket.m_int64GameTransactionID)
		<< BUILD_LOG(kPacket.m_strGarenaTransactionID)
		<< BUILD_LOG(kPacket.m_uiGarenaUID)
		<< BUILD_LOG(iServerInfo)
		<< BUILD_LOG(kPacket.m_uiShell_Amount)
		<< BUILD_LOG(kPacket.m_uiGameCurrency_Amount)
		<< BUILD_LOG(kPacket.m_iResult)
		<< BUILD_LOG(kPacket.m_strDateTime)
		<< END_LOG;




}

#endif //SERV_COUNTRY_PH

int KLoginBillingDBThread::DoQuery_BillOrderGameServer( IN const std::wstring& wstrOrderID,
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
														)
{
	int iRet = NetError::ERR_ODBC_01;

	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BillOrder_GameServer_SET", L"N\'%s\', %d, N\'%s\', %d, %d, %d, %d, N\'%s\', %d",
		% wstrOrderID
		% iUserUID
		% wstrUserID
		% iServerGroupID
		% iUnitUID
		% (int)cLevel
		% iChargeCash
		% wstrUserIP
		% (int)cOrderType
		);

	int iOK;

#if defined( SERV_COUNTRY_PH ) || defined( SERV_ALL_RENEWAL_SP )
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK 
			>> i64TransactionID
			);
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

#else //( SERV_COUNTRY_PH ) || ( SERV_ALL_RENEWAL_SP )
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

#endif //( SERV_COUNTRY_PH ) || ( SERV_ALL_RENEWAL_SP )
	

	if( iOK != 0 )
	{
		iRet = NetError::ERR_EXCHANGE_CASH;
		goto end_proc;
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( wstrOrderID )
		<< BUILD_LOG( iUserUID )
		<< BUILD_LOG( wstrUserID )
		<< BUILD_LOG( iServerGroupID )
		<< BUILD_LOG( iUnitUID )
		<< BUILD_LOG( cLevel )
		<< BUILD_LOG( iChargeCash )
		<< BUILD_LOG( wstrUserIP )
		<< BUILD_LOG( (int)cOrderType )
		<< BUILD_LOG( iOK )
#if defined( SERV_COUNTRY_PH ) || defined( SERV_ALL_RENEWAL_SP )
		<< BUILD_LOG( i64TransactionID )
#endif //( SERV_COUNTRY_PH ) || ( SERV_ALL_RENEWAL_SP )
		<< END_LOG;

	return iRet;
}

int KLoginBillingDBThread::DoQuery_BillOrderResult( IN const std::wstring& wstrOrderID,
												IN const int iOrderResult,
												OUT int& iTotalCash,
												OUT int& iChargeCash,
												OUT int& iChargeBonus,
												OUT int& iBonusItem,
												OUT UidType& iUnitUID 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
												, OUT KGlobalCashInfo& kRemainCashInfo
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
												)
{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BillOrder_Result", L"N\'%s\', %d",
		% wstrOrderID
		% iOrderResult
		);

	int iOK;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTotalCash
			>> iChargeCash
			>> iChargeBonus
			>> iBonusItem
			>> iUnitUID 

			>> kRemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_CASH]
			>> kRemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT]

			);
		m_kODBC.EndFetch();
#else
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTotalCash
			>> iChargeCash
			>> iChargeBonus
			>> iBonusItem
			>> iUnitUID 
			);
		m_kODBC.EndFetch();
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES		
	}
	else
	{
		goto end_proc;
	}

	if( iOK != 0 )
	{
		iRet = NetError::ERR_NX_SHOP_03;
		goto end_proc;
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( wstrOrderID )
		<< BUILD_LOG( iOrderResult )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( iTotalCash )
		<< BUILD_LOG( iChargeCash )
		<< BUILD_LOG( iChargeBonus )
		<< BUILD_LOG( iBonusItem )
		<< BUILD_LOG( iUnitUID )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< BUILD_LOG( kRemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_CASH] )
		<< BUILD_LOG( kRemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT] )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< END_LOG;

	return iRet;
}

int KLoginBillingDBThread::DoQuery_CouponOrder( IN const std::wstring& wstrCouponID,
												IN const std::wstring& wstrOrderID,
												IN const UidType iUserUID,
												IN const std::wstring& wstrUserID,
												IN const int iServerGroupID,
												IN const UidType iUnitUID,
												IN const char cLevel,
												IN const std::wstring& wstrUserIP,
												IN const char cOrderType )
{
	int iOK = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_CouponOrder_SET", L"N\'%s\', N\'%s\', %d, N\'%s\', %d, %d, %d, N\'%s\', %d",
		% wstrCouponID
		% wstrOrderID
		% iUserUID
		% wstrUserID
		% iServerGroupID
		% iUnitUID
		% (int)cLevel
		% wstrUserIP
		% (int)cOrderType
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	switch( iOK )
	{
	case NetError::NET_OK:	// 성공
		break;
	case -11:	// 이미 사용한 쿠폰입니다.
		iOK = NetError::ERR_GIANT_BILLING_05;
		break;
	default:
		iOK = NetError::ERR_GIANT_BILLING_08;
		break;
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( wstrCouponID )
		<< BUILD_LOG( wstrOrderID )
		<< BUILD_LOG( iUserUID )
		<< BUILD_LOG( wstrUserID )
		<< BUILD_LOG( iServerGroupID )
		<< BUILD_LOG( iUnitUID )
		<< BUILD_LOGc( cLevel )
		<< BUILD_LOG( wstrUserIP )
		<< BUILD_LOGc( cOrderType )
		<< BUILD_LOG( iOK )
		<< END_LOG;

	return iOK;
}

int KLoginBillingDBThread::DoQuery_CouponOrderResult( IN const std::wstring& wstrOrderID,
														IN const int iOrderResult,
														IN int& iChargeItem,
														IN int& iChargeCash,
														IN int& iChargeBonus,
														OUT int& iTotalCash )

{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_CouponOrder_Result", L"N\'%s\', %d, %d, %d, %d",
		% wstrOrderID
		% iOrderResult
		% iChargeItem
		% iChargeCash
		% iChargeBonus
		);

	int iOK;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTotalCash );
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	if( iOK != 0 )
	{
		iRet = NetError::ERR_GIANT_BILLING_05;
		goto end_proc;
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( wstrOrderID )
		<< BUILD_LOG( iOrderResult )
		<< BUILD_LOG( iTotalCash )
		<< BUILD_LOG( iChargeItem )
		<< BUILD_LOG( iChargeCash )
		<< BUILD_LOG( iChargeBonus )
		<< END_LOG;

	return iRet;
}

int KLoginBillingDBThread::DoQuery_TransactionNumberServerGet( OUT __int64& iTranNo, OUT time_t& iTime )
{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_ARG( L"exec dbo.EBP_TranNumberServer_INT" );

	int iOK;
	__int64 _time;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iTranNo
			>> _time );
		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	if( iOK != 0 )
	{
		iRet = NetError::ERR_GIANT_BILLING_08;
		goto end_proc;
	}

	iRet = NetError::NET_OK;
	iTime = static_cast<time_t>(_time);

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iTranNo )
		<< BUILD_LOG( iTime )
		<< END_LOG;

	return iRet;
}

void KLoginBillingDBThread::MakeTID(__int64 iTransactionNo, time_t iTime, std::wstring& wstrTransaction
#ifdef SERV_COUNTRY_CN
	, bool bCoupon /*= false*/
#endif SERV_COUNTRY_CN
	)
{
	boost::wformat wfmt(L"%02x%03x%08x%08x");

#ifdef SERV_COUNTRY_CN
	KGiantBaseManager::KGiantServerInfo kInfo;

	if( bCoupon == true )
		kInfo = SiKGiantCouponManager()->GetServerInfo();
	else
		kInfo = SiKGiantBillingManager()->GetServerInfo();

	wstrTransaction = boost::str(wfmt
		% kInfo.m_usGame 
		% kInfo.m_usZone 
		% iTime
		% iTransactionNo );
#else //SERV_COUNTRY_CN
	wstrTransaction = boost::str(wfmt
		% 1
		% 1
		% iTime
		% iTransactionNo );
#endif //SERV_COUNTRY_CN

}

#ifdef SERV_COUNTRY_PH

//{{ 2011. 03. 03	최육사	중국 서버 빌링. 사내 버전도 이 플로우 사용함
int KLoginBillingDBThread::DoQuery_GetCashPointByUserUID( IN const UidType iUserUID, 

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
													  OUT KGlobalCashInfo& RemainCashInfo
#else
													  OUT unsigned long& ulTotalCash 
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
													  )
{
	// 원래 이쪽 코드는 정상적인 경우라면 중국만 사용하게 된다.
	// 하지만 사내 버전의 경우 캐시를 보여주려고 여기 프로세스를 빌려서 사용한다.

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	unsigned long ulTotalCash = 0;
#else
	ulTotalCash = 0;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_Cash_Total_USERUID_SEL", L"%d", % iUserUID );
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if( m_kODBC.BeginFetch() )
	{
#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
		// 2013.08.07 lygan_조성욱 동남아시아 서비스용 포함
		FETCH_DATA( iRet 
			>> ulTotalCash
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_CASH]			// KOG 빌링에서 관리하는 캐시
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT]	// KOG 빌링에서 관리하는 보너스
			);
#else //SERV_COUNTRY_CN
		// 사내용임. 클라이언트 출력 처리 코드 때문에 GCT_KOG_ELSWORD_CASH, GCT_KOG_ELSWORD_BONUS_POINT을 그대로 사용할 수 없었음.
		FETCH_DATA( iRet 
			>> ulTotalCash
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_CASH]			// KOG 빌링에서 관리하는 캐시
			>> RemainCashInfo.m_ulCash[KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH]	// KOG 빌링에서 관리하는 보너스
			);
#endif //SERV_COUNTRY_CN


			m_kODBC.EndFetch();
	}
#else
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iRet 
			>> ulTotalCash
			);

		m_kODBC.EndFetch();
	}

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	else
	{
		goto end_proc;
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< END_LOG;

	return iRet;
}
//}}



int KLoginBillingDBThread::DoQuery_SetChangeGameCurrencyFailLog(IN KEJSON_GN_CHANGE_GAME_CURRENCY_ACK kPaket )
{


	std::wstring wstrTransaction = KncUtil::toWideString(kPaket.m_strGarenaTransactionID);

	DO_QUERY_NO_PROFILE( L"exec dbo.EBP_BillOrder_ErrorLog_INS", L"%d, N\'%s\', %d, %d, %d, %d, %d",
		% kPaket.m_int64GameTransactionID
		% wstrTransaction
		% kPaket.m_uiGarenaUID
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPaket.m_uiShell_Amount
		% kPaket.m_uiGameCurrency_BeforeAmount
		% kPaket.m_iResult
		);
	
end_proc:
	return true;
}

#endif //SERV_COUNTRY_PH