#include "ActorManager.h"
#include "PURPLEBilling.h"

#ifdef SERV_COUNTRY_JP

#include "KncSend.h"
#include "PURPLEBillingManager.h"
#include "NetError.h"

//{{ 2009. 12. 30  김정협	일본빌링
#include "HanBillingForSvr.h"
//}}



#define CLASS_TYPE  KPURPLEBilling

KPURPLEBilling::KPURPLEBilling( void )
{
}

KPURPLEBilling::~KPURPLEBilling( void )
{
	End( 15000 );
}

void KPURPLEBilling::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();
	

	//KSerializer ks;
	//ks.BeginReading( &spEvent_->m_kbuff );

	switch( spEvent_->m_usEventID )
	{
		CASE( EPUBLISHER_BILLING_BALANCE_REQ );
		
		CASE( EBILL_BUY_PRODUCT_REQ );

		CASE( EBILL_GIFT_ITEM_REQ );	// 실제 사용안함
		CASE( EBILL_USE_COUPON_REQ );	// 실제 사용안함 (퍼블 체크없이 바로 사용)
	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}

void KPURPLEBilling::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KPURPLEBilling::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

void KPURPLEBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID )
{
	SendToKOGBillingDB( nUserUID, usEventID, char() );
}


IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ )
{
	KEPUBLISHER_BILLING_BALANCE_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
	kPacketAck.m_ulBalance = 0;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// 캐시 종류를 하나로 보는곳은 일반적으로 퍼블캐쉬로 본다.
	kPacketAck.m_iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	//int iCashPoint;
	//kPacket.m_iOK = GetCashPoint( kPacket_, iCashPoint );
	char szBillNo[MAX_BILLNO_LEN] = {0x00};
	char szRetMsgBillNo[1024] = {0x00};

	char	csID[128] = "";
	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrAccount.c_str(), -1, csID, 128, NULL, NULL );

	
	int ret = HanBillingGetBillNoAcc(csID, szBillNo, szRetMsgBillNo, 1024 );
	if( ret == HAN_BILLING_OK )
	{
		HANBILL_BALANCEINFO sBalanceInfo;
		ret = HanBillingBalanceInquire(csID, szBillNo, szRetMsgBillNo, 1024, &sBalanceInfo);

		if(ret  == HAN_BILLING_OK)
		{
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_ulBalance = ( unsigned long )sBalanceInfo.nTotalBalance;		
		}
		else
		{
			kPacketAck.m_iOK = NetError::ERR_GASH_00;
			kPacketAck.m_ulBalance = 0;

			START_LOG( cerr, L"HanBillingBalanceInquire 빌링 값 얻어오기 실패." )
				<< BUILD_LOG( ret )
				<< BUILD_LOG( csID )
				<< BUILD_LOG( szBillNo )
				<< BUILD_LOG( szRetMsgBillNo )
				<< END_LOG;
		}
	}
	else if( ret == HAN_BILLING_SCD_NOTEXIST )
	{
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_ulBalance = 0;
	}
	else
	{
		kPacketAck.m_iOK = NetError::ERR_GASH_00;
		kPacketAck.m_ulBalance = 0;

		START_LOG( cerr, L"HanBillingGetBillNoAcc 빌링 값 얻어오기 실패." )
			<< BUILD_LOG( ret )
			<< BUILD_LOG( csID )
			<< BUILD_LOG( szBillNo )
			<< BUILD_LOG( szRetMsgBillNo )
			<< END_LOG;

		
	}

	SendToUser( LAST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_REQ )
{	
	KEBILL_BUY_PRODUCT_REQ kPacketReq = kPacket_;
	kPacketReq.m_vecBillBuyInfo.clear();
	KEBILL_BUY_PRODUCT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;


	// 구입 금액을 계산한다.
	int iTotalPrice = 0;
	std::vector< KBillBuyInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); vit++ )
	{
		iTotalPrice += vit->m_iPoint;
	}

	// 잔액을 확인한다.
	int iBalance;

    
	char szBillNo[MAX_BILLNO_LEN] = {0x00};
	char szRetMsgBillNo[1024] = {0x00};

	char	csID[128] = "";		
	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrUserID.c_str(), -1, csID, 128, NULL, NULL );


	int ret = HanBillingGetBillNoAcc(csID, szBillNo, szRetMsgBillNo, 1024 );
	if( ret == HAN_BILLING_OK )
	{
		HANBILL_BALANCEINFO sBalanceInfo;
		ret = HanBillingBalanceInquire(csID, szBillNo, szRetMsgBillNo, 1024, &sBalanceInfo);

		if(ret  == HAN_BILLING_OK)
		{
			kPacket.m_iOK = NetError::NET_OK;
			iBalance = ( unsigned long )sBalanceInfo.nTotalBalance;		
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_GASH_00;
			iBalance = 0;
		}
	}
	else if( ret == HAN_BILLING_SCD_NOTEXIST )
	{
		kPacket.m_iOK = NetError::NET_OK;
		iBalance = 0;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_GASH_00;
		iBalance = 0;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"구입전 잔액조회 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( csID )
			<< BUILD_LOG( szBillNo )
			<< BUILD_LOG( szRetMsgBillNo )
			<< END_LOG;
		SendToUser( kPacket_.m_iUserUID, EBILL_BUY_PRODUCT_ACK, kPacket );
		return;
	}

	if( iBalance < iTotalPrice )
	{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
		//kPacket.m_iOK = NetError::ERR_PURPLE_09;
		kPacket.m_iOK = NetError::ERR_GASH_09;
		SendToUser( kPacket_.m_iUserUID, EBILL_BUY_PRODUCT_ACK, kPacket );
		return;
	}



	// 리스트에 있는 아이템에 대해 차례로 포인트를 차감한다.
	for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); vit++ )
	{
		KBillBuyInfo& refBillBuyInfo = *vit;

		wchar_t wszNumber[32];
		::_itow( refBillBuyInfo.m_iProductID, wszNumber, 10 );
		std::wstring wstrItemInfo = wszNumber;
		::_itow( refBillBuyInfo.m_iCategory, wszNumber, 10 );
		std::wstring wstrItemType = wszNumber;

		char csNumber[32];
		//::_itoa( vit->m_iProductNo, csNumber, 10 );
		sprintf( csNumber, "ELS%d", vit->m_iProductNo );


		char	csMemo[128] = "";		
		WideCharToMultiByte( CP_ACP, 0, refBillBuyInfo.m_wstrMemo.c_str(), -1, csMemo, 128, NULL, NULL );

		std::wstring wstrRet;
		char	csOrderID[128] = "";		
		

		int nNumber = SiKBillManager()->GetNextPurchaseNo(); 
		
		_i64tow(nNumber , wszNumber, 10 );		
		if(nNumber < 10 )
		{
			wstrRet += L"0";
		}
		wstrRet += wszNumber;		
		wstrRet += refBillBuyInfo.m_wstrOrderID;
		
		
		WideCharToMultiByte( CP_ACP, 0, wstrRet.substr(0, 20).c_str(), -1, csOrderID, 128, NULL, NULL );

		//char	csUserIP[20] = "";		
		//WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrUserIP.c_str(), -1, csUserIP, 20, NULL, NULL );

		//int iRet = InGameLessGPoint( kPacket_.m_wstrOrderID, kPacket_.m_iServerGroupID, kPacket_.m_wstrUserID, kPacket_.m_wstrNickName, vit->m_iPoint, vit->m_iProductID, vit->m_iProductNo, vit->m_wstrMemo );
		
		HANBILL_BUYINFO stBuyInfo;
		int iRet = HanBillingBuy( csID, szBillNo, szRetMsgBillNo, 1024,  csNumber, csMemo, 1, refBillBuyInfo.m_iPoint, kPacket_.m_strUserIP.c_str(), csOrderID, &stBuyInfo);

		if(refBillBuyInfo.m_iPoint == 0)
		{
			iRet = NetError::NET_OK;
		}

		if( iRet != NetError::NET_OK )
		{
			START_LOG( cerr, L"구입 오류" )
				<< BUILD_LOG( kPacket_.m_iUserUID )				
				<< BUILD_LOG( iRet )				
				<< BUILD_LOG( csID )
				<< BUILD_LOG( szBillNo )
				<< BUILD_LOG( szRetMsgBillNo )
				<< BUILD_LOG( csNumber )
				<< BUILD_LOG( csMemo )
				<< BUILD_LOG( vit->m_iPoint )
				<< BUILD_LOG( kPacket_.m_strUserIP )	
				<< BUILD_LOG( csOrderID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_61;
			SendToUser( kPacket_.m_iUserUID, EBILL_BUY_PRODUCT_ACK, kPacket );
			return;
			
		}
		else
		{
			kPacketReq.m_vecBillBuyInfo.push_back( *vit );
			START_LOG( clog, L"구입 성공" )
				<< BUILD_LOG( iRet )				
				<< BUILD_LOG( csID )
				<< BUILD_LOG( szBillNo )
				<< BUILD_LOG( szRetMsgBillNo )
				<< BUILD_LOG( csNumber )
				<< BUILD_LOG( csMemo )
				<< BUILD_LOG( vit->m_iPoint )
			<< BUILD_LOG( kPacket_.m_strUserIP )
			<< BUILD_LOG( csOrderID )		
			<< END_LOG;
		}
	}

	SendToKOGBillingDB( kPacket_.m_iUserUID, EBILL_BUY_PRODUCT_REQ, kPacketReq );
}


IMPL_ON_FUNC( EBILL_GIFT_ITEM_REQ )
{
	KEBILL_GIFT_ITEM_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// 일본 쿠폰 사용 안함

	START_LOG( cerr, L"일본은 선물 기능 없기 때문에 여기 코드 타면 안됨!!!!!!" )
		<< BUILD_LOG( kPacket_.m_iSenderUserUID )
		<< BUILD_LOG( kPacket_.m_iSenderUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
		<< BUILD_LOG( kPacket_.m_iReceiverUserUID )
		<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_USE_COUPON_REQ )
{
	KEBILL_USE_COUPON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// 일본 쿠폰 사용 안함

	START_LOG( cerr, L"일본은 퍼블 쿠폰 인증 안 받기 때문에 여기 코드 타면 안됨!!!!!!" )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrSerialCode)
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, EBILL_USE_COUPON_ACK, kPacketAck );
}



#endif // SERV_COUNTRY_JP