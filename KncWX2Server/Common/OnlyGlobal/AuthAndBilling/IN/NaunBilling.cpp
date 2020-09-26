#include "ActorManager.h"
#include "NaunBilling.h"

#include "KncSend.h"
#include "NaunBillingManager.h"
#include "NetError.h"

#ifdef SERV_COUNTRY_IN

#import "bxIPGClient.dll" no_namespace

#define CLASS_TYPE  KNaunBilling

KNaunBilling::KNaunBilling( void )
{
}

KNaunBilling::~KNaunBilling( void )
{
	End( 15000 );
}

void KNaunBilling::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		CASE( EPUBLISHER_BILLING_BALANCE_REQ );

		CASE( EBILL_BUY_PRODUCT_REQ );
		CASE( EBILL_GIFT_ITEM_REQ );

		CASE( EBILL_USE_COUPON_REQ );			// 실제 사용안함 (퍼블 체크없이 바로 사용) ???

	default:
		START_LOG( cerr, "핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
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

void KNaunBilling::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

void KNaunBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID )
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

	HRESULT hr = CoInitialize(NULL);
	if( FAILED(hr) )
	{
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN; // COM 초기화 실패
		SendToUser( FIRST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
		return ;
	}

	IIPGClientPtr ptrClient;
	ptrClient.CreateInstance(__uuidof(IPGClient));
	if( ptrClient == NULL )
	{
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN; // 인스턴스 생성 실패
		SendToUser( FIRST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
		return ;
	}

	_variant_t vstrUserUID, vstrUserID, vstrCompanyCode, vstrNickName;
	_variant_t vlRemainCashShop, vlRemainCashContents, vlRemainCashBonus, vlRemainCashEtc, vlRemainMileageShop, vlRemainMileageContents;
	_variant_t vstrReturnCode;

	/*
	wchar_t wszUserUID[40];
	_i64tow( kPacket_.m_uiActorUID, wszUserUID, 10 );

	vstrUserUID = wszUserUID;
	vstrUserID = kPacket_.m_wstrAccount.c_str();
	vstrCompanyCode = static_cast<LPSTR>(L"MELA");
	vstrNickName = kPacket_.m_wstrNickName.c_str();
	*/
	vstrUserUID = (LPSTR)"1234";
	vstrUserID = (LPSTR)"testid";
	vstrCompanyCode = static_cast<LPSTR>("MELA");
	vstrNickName = kPacket_.m_wstrNickName.c_str();

	vstrReturnCode = ptrClient->GetCash( &vstrUserUID, &vstrUserID, &vstrCompanyCode, &vstrNickName,
									&vlRemainCashShop, &vlRemainCashContents, &vlRemainCashBonus, &vlRemainCashEtc, 
									&vlRemainMileageShop, &vlRemainMileageContents );

	std::wstring wstrUserUID = vstrUserUID.bstrVal;
	std::wstring wstrUserID = vstrUserID.bstrVal;
	std::wstring wstrCompanyCode = vstrCompanyCode.bstrVal;
	std::wstring wstrNickName = vstrNickName.bstrVal;
	long lRemainCashshop = vlRemainCashShop.lVal;
	long lRemainCashContents = vlRemainCashContents.lVal;
	long lRemainCashBonus = vlRemainCashBonus.lVal;
	long lRemainCashEtc = vlRemainCashEtc.lVal;
	long lRemainMileageShop = vlRemainMileageShop.lVal;
	long lRemainMileageContents = vlRemainMileageContents.lVal;
	std::wstring wstrReturnCode = vstrReturnCode.bstrVal;

	if( wstrReturnCode.compare( L"0000" ) == 0 )			// 성공
	{
		kPacketAck.m_ulBalance = static_cast<unsigned long>(vlRemainCashShop.lVal) + static_cast<unsigned long>(vlRemainMileageContents.lVal) + static_cast<unsigned long>(vlRemainCashBonus.lVal) + static_cast<unsigned long>(vlRemainCashEtc);
		kPacketAck.m_iOK = NetError::NET_OK;

		START_LOG( clog, L"NaunBilling 잔액 조회 성공" )
			<< BUILD_LOG( wstrUserUID )
			<< BUILD_LOG( wstrUserID )
			<< BUILD_LOG( wstrCompanyCode )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( lRemainCashshop )
			<< BUILD_LOG( lRemainCashContents )
			<< BUILD_LOG( lRemainCashBonus )
			<< BUILD_LOG( lRemainCashEtc )
			<< BUILD_LOG( lRemainMileageShop )
			<< BUILD_LOG( lRemainMileageContents )
			<< END_LOG;
	}
	else
	{
		kPacketAck.m_iOK = NetError::ERR_GASH_02;

		START_LOG( cerr, L"NaunBilling 잔액 조회 실패" )
			<< BUILD_LOG( wstrUserUID )
			<< BUILD_LOG( wstrUserID )
			<< BUILD_LOG( wstrCompanyCode )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( lRemainCashshop )
			<< BUILD_LOG( lRemainCashContents )
			<< BUILD_LOG( lRemainCashBonus )
			<< BUILD_LOG( lRemainCashEtc )
			<< BUILD_LOG( lRemainMileageShop )
			<< BUILD_LOG( lRemainMileageContents )
			<< END_LOG;
	}

	CoUninitialize();

	SendToUser( FIRST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_REQ )
{
	long lTotalPrice = 0;
	std::wstring wstrEtc;
	std::vector< KBillBuyInfo >::iterator vit;

	for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); ++vit )
	{
		lTotalPrice += vit->m_iPoint;
		
		wchar_t wszProductNo[10], wszProductCnt[3];
		_itow( vit->m_iProductNo, wszProductNo, 10 );
		_itow( vit->m_usOrderQuantity, wszProductCnt, 10 );
		wstrEtc += wszProductNo;
		wstrEtc += L"-";
		wstrEtc += wszProductCnt;
		wstrEtc += L"|";
	}

	wchar_t wszFirstProductNo[10], wszFirstProductCategory[10];
	_itow( (kPacket_.m_vecBillBuyInfo.begin())->m_iProductNo, wszFirstProductNo, 10 );
	_itow( (kPacket_.m_vecBillBuyInfo.begin())->m_iCategory, wszFirstProductCategory, 10 );

	HRESULT hr = CoInitialize(NULL);
	if( FAILED(hr) )
	{
		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GASH_02;
		SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacketAck );
		return ;
	}

	IIPGClientPtr ptrClient;
	ptrClient.CreateInstance(__uuidof(IPGClient));
	if( ptrClient == NULL )
	{
		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN; // 인스턴스 생성 실패
		SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacketAck );
		return ;
	}

	_variant_t vstrUserUID, vstrUserID, vstrCompanyCode, vstrNickName, vstrIP;
	_variant_t vlBuyType, vstrProductName, vstrProductNo, vstrCategory, vlProductCnt, vstrProductEtc, vstrToUserUID, vstrToUserID;
	_variant_t vlPrice, vstrAgencyNo, vstrEtc2, vstrEtc3, vstrEtc4;
	_variant_t vstrOrderID, vstrReturnCode;

	/*
	wchar_t wszUserUID[40];
	_i64tow( kPacket_.m_iUserUID, wszUserUID, 10 );

	vstrUserUID = wszUserUID;
	vstrUserID = kPacket_.m_wstrAccount.c_str();
	vstrCompanyCode = static_cast<LPSTR>(L"MELA");
	vstrNickName = kPacket_.m_wstrNickName.c_str();
	*/
	vstrUserUID = (LPSTR)"1234";
	vstrUserID = (LPSTR)"testid";
	vstrCompanyCode = (LPSTR)"MELA";
	vstrNickName = kPacket_.m_wstrNickName.c_str();
	vstrIP = (KncUtil::toWideString( kPacket_.m_strUserIP )).c_str();
	//vlBuyType = ( kPacket_.m_vecBillBuyInfo.size() > 1 ) ? static_cast<LONG>(KNaunBillingManager::NBT_BUY_MANY_PRODUCT) : static_cast<LONG>(KNaunBillingManager::NBT_BUY_ONE_PRODUCT);	// 메뉴얼에는 상품 번호로 되어있는데 구매 타입임 헷갈리지 말 것.
	//패키지 하나 구매시에 따로 구분하기 위해 아래 분기문을 둠. 이게 다 철우형 때문이다.
	if( kPacket_.m_vecBillBuyInfo.size() > 1 )
	{
		vlBuyType = static_cast<LONG>(KNaunBillingManager::NBT_BUY_MANY_PRODUCT);
	}
	else
	{
		if( (kPacket_.m_vecBillBuyInfo.begin())->m_iCategory == 51 )
		{
			vlBuyType = static_cast<LONG>(KNaunBillingManager::NBT_BUY_ONE_PACKAGE);
		}
		else
		{
			vlBuyType = static_cast<LONG>(KNaunBillingManager::NBT_BUY_ONE_PRODUCT);
		}
	}

	vstrProductName = (LPSTR)"";
	vstrProductNo = wszFirstProductNo;
	vstrCategory = wszFirstProductCategory;
	vlProductCnt = static_cast<LONG>( (kPacket_.m_vecBillBuyInfo.begin())->m_usOrderQuantity );
	vstrProductEtc = (LPSTR)"";
	vstrToUserUID = (LPSTR)"";
	vstrToUserID = (LPSTR)"";
	vlPrice = lTotalPrice;
	vstrAgencyNo = (LPSTR)"";
	vstrEtc2 = wstrEtc.c_str();
	vstrEtc3 = (LPSTR)"";
	vstrEtc4 = kPacket_.m_wstrTotalOrderID.c_str();
	vstrOrderID = (LPSTR)"";
	vstrReturnCode = (LPSTR)"";

	vstrReturnCode = ptrClient->Output( &vstrUserUID, &vstrUserID, &vstrCompanyCode, &vstrNickName, &vstrIP, 
		&vlBuyType, &vstrProductName, &vstrProductNo, &vstrCategory, &vlProductCnt, &vstrProductEtc, &vstrToUserUID, &vstrToUserID, 
		&vlPrice, &vstrAgencyNo, &vstrEtc2, &vstrEtc3, &vstrEtc4, &vstrOrderID );

	std::wstring wstrUserUID = vstrUserUID.bstrVal;
	std::wstring wstrUserID = vstrUserID.bstrVal;
	std::wstring wstrCompanyCode = vstrCompanyCode.bstrVal;
	std::wstring wstrNickName = vstrNickName.bstrVal;
	std::wstring wstrIP = vstrIP.bstrVal;
	long lBuyType = vlBuyType.lVal;
	std::wstring wstrProductName = vstrProductName.bstrVal;
	std::wstring wstrProductNo = vstrProductNo.bstrVal;
	std::wstring wstrCategory = vstrCategory.bstrVal;
	long lProductCnt = vlProductCnt.lVal;
	std::wstring wstrProductEtc = vstrProductEtc.bstrVal;
	std::wstring wstrToUserUID = vstrToUserUID.bstrVal;
	std::wstring wstrToUserID = vstrToUserID.bstrVal;
	long lPrice = vlPrice.lVal;
	std::wstring wstrAgencyNo = vstrAgencyNo.bstrVal;
	std::wstring wstrEtc2 = vstrEtc2.bstrVal;
	std::wstring wstrEtc3 = vstrEtc3.bstrVal;
	std::wstring wstrEtc4 = vstrEtc4.bstrVal;
	std::wstring wstrOrderID = vstrOrderID.bstrVal;
	std::wstring wstrReturnCode = vstrReturnCode.bstrVal;

	if( wstrReturnCode.compare( L"0000" ) == 0 )			// 성공(빌링 주문번호 처리)
	{
		SendToKOGBillingDB( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_REQ, kPacket_ );

		START_LOG( clog, L"NaunBilling 아이템 구매 성공" )
			<< BUILD_LOG( wstrUserUID )
			<< BUILD_LOG( wstrUserID )
			<< BUILD_LOG( wstrCompanyCode )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( wstrIP )
			<< BUILD_LOG( lBuyType )
			<< BUILD_LOG( wstrProductName )
			<< BUILD_LOG( wstrProductNo )
			<< BUILD_LOG( wstrCategory )
			<< BUILD_LOG( lProductCnt )
			<< BUILD_LOG( wstrProductEtc )
			<< BUILD_LOG( wstrToUserUID )
			<< BUILD_LOG( wstrToUserID )
			<< BUILD_LOG( lPrice )
			<< BUILD_LOG( wstrAgencyNo )
			<< BUILD_LOG( wstrEtc2 )
			<< BUILD_LOG( wstrEtc3 )
			<< BUILD_LOG( wstrEtc4 )
			<< BUILD_LOG( wstrOrderID )
			<< BUILD_LOG( wstrReturnCode )
			<< END_LOG;
	}
	else
	{	
		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GASH_02;
		SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacketAck );

		START_LOG( cerr, L"NaunBilling 아이템 구매 실패" )
			<< BUILD_LOG( wstrUserUID )
			<< BUILD_LOG( wstrUserID )
			<< BUILD_LOG( wstrCompanyCode )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( wstrIP )
			<< BUILD_LOG( lBuyType )
			<< BUILD_LOG( wstrProductName )
			<< BUILD_LOG( wstrProductNo )
			<< BUILD_LOG( wstrCategory )
			<< BUILD_LOG( lProductCnt )
			<< BUILD_LOG( wstrProductEtc )
			<< BUILD_LOG( wstrToUserUID )
			<< BUILD_LOG( wstrToUserID )
			<< BUILD_LOG( lPrice )
			<< BUILD_LOG( wstrAgencyNo )
			<< BUILD_LOG( wstrEtc2 )
			<< BUILD_LOG( wstrEtc3 )
			<< BUILD_LOG( wstrEtc4 )
			<< BUILD_LOG( wstrOrderID )
			<< BUILD_LOG( wstrReturnCode )
			<< END_LOG;
	}

	CoUninitialize();
}

IMPL_ON_FUNC( EBILL_GIFT_ITEM_REQ )
{
	long lTotalPrice = 0;
	std::wstring wstrEtc;
	std::vector< KBillBuyInfo >::iterator vit;

	for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); ++vit )
	{
		lTotalPrice += vit->m_iPoint;

		wchar_t wszProductNo[10], wszProductCnt[3];
		_itow( vit->m_iProductNo, wszProductNo, 10 );
		_itow( vit->m_usOrderQuantity, wszProductCnt, 10 );
		wstrEtc += wszProductNo;
		wstrEtc += L"-";
		wstrEtc += wszProductCnt;
		wstrEtc += L"|";
	}

	wchar_t wszFirstProductNo[10], wszFirstProductCategory[10];
	_itow( (kPacket_.m_vecBillBuyInfo.begin())->m_iProductNo, wszFirstProductNo, 10 );
	_itow( (kPacket_.m_vecBillBuyInfo.begin())->m_iCategory, wszFirstProductCategory, 10 );

	HRESULT hr = CoInitialize(NULL);
	if( FAILED(hr) )
	{
		KEBILL_GIFT_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN; // COM 초기화 실패
		SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacketAck );
		return ;
	}

	IIPGClientPtr ptrClient;
	ptrClient.CreateInstance(__uuidof(IPGClient));
	if( ptrClient == NULL )
	{
		KEBILL_GIFT_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN; // 인스턴스 생성 실패
		SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacketAck );
		return ;
	}

	_variant_t vstrUserUID, vstrUserID, vstrCompanyCode, vstrNickName, vstrIP;
	_variant_t vlBuyType, vstrProductName, vstrProductNo, vstrCategory, vlProductCnt, vstrProductEtc, vstrToUserUID, vstrToUserID;
	_variant_t vlPrice, vstrAgencyNo, vstrEtc2, vstrEtc3, vstrEtc4;
	_variant_t vstrOrderID, vstrReturnCode;

	wchar_t wszSenderUserUID[40], wszReceiverUserUID[40];
	_i64tow( kPacket_.m_iSenderUserUID, wszSenderUserUID, 10 );
	_i64tow( kPacket_.m_iReceiverUserUID, wszReceiverUserUID, 10 );

	/*
	vstrUserUID = wszSenderUserUID;
	vstrUserID = kPacket_.m_wstrAccount.c_str();
	vstrCompanyCode = static_cast<LPSTR>(L"MELA");
	*/
	vstrUserUID = (LPSTR)"1234";
	vstrUserID = (LPSTR)"testid";
	vstrCompanyCode = (LPSTR)"MELA";
	vstrNickName = kPacket_.m_wstrSenderNickName.c_str();
	vstrIP = kPacket_.m_wstrUserIP.c_str();
	//vlBuyType = ( kPacket_.m_vecBillBuyInfo.size() > 1 ) ? static_cast<LONG>(KNaunBillingManager::NBT_BUY_MANY_PRODUCT) : static_cast<LONG>(KNaunBillingManager::NBT_BUY_ONE_PRODUCT);	// 메뉴얼에는 상품 번호로 되어있는데 구매 타입임 헷갈리지 말 것.
	//패키지 하나 구매시에 따로 구분하기 위해 아래 추가 분기문을 둠. 이게 다 철우형 때문이다.
	if( kPacket_.m_vecBillBuyInfo.size() > 1 )
	{
		vlBuyType = static_cast<LONG>(KNaunBillingManager::NBT_BUY_MANY_PRODUCT);
	}
	else
	{
		if( (kPacket_.m_vecBillBuyInfo.begin())->m_iCategory == 51 )
		{
			vlBuyType = static_cast<LONG>(KNaunBillingManager::NBT_BUY_ONE_PACKAGE);
		}
		else
		{
			vlBuyType = static_cast<LONG>(KNaunBillingManager::NBT_BUY_ONE_PRODUCT);
		}
	}
	vstrProductName = (LPSTR)"";
	vstrProductNo = wszFirstProductNo;
	vstrCategory = wszFirstProductCategory;
	vlProductCnt = static_cast<LONG>( (kPacket_.m_vecBillBuyInfo.begin())->m_usOrderQuantity );
	vstrProductEtc = (LPSTR)"";
	vstrToUserUID = wszReceiverUserUID;
	vstrToUserID = kPacket_.m_wstrReceiverUserID.c_str();
	vlPrice = lTotalPrice;
	vstrAgencyNo = (LPSTR)"";
	vstrEtc2 = wstrEtc.c_str();
	vstrEtc3 = (LPSTR)"";
	vstrEtc4 = kPacket_.m_wstrTotalOrderID.c_str();
	vstrOrderID = (LPSTR)"";
	vstrReturnCode = (LPSTR)"";

	vstrReturnCode = ptrClient->Output( &vstrUserUID, &vstrUserID, &vstrCompanyCode, &vstrNickName, &vstrIP, 
		&vlBuyType, &vstrProductName, &vstrProductNo, &vstrCategory, &vlProductCnt, &vstrProductEtc, &vstrToUserUID, &vstrToUserID, 
		&vlPrice, &vstrAgencyNo, &vstrEtc2, &vstrEtc3, &vstrEtc4, &vstrOrderID );

	std::wstring wstrUserUID = vstrUserUID.bstrVal;
	std::wstring wstrUserID = vstrUserID.bstrVal;
	std::wstring wstrCompanyCode = vstrCompanyCode.bstrVal;
	std::wstring wstrNickName = vstrNickName.bstrVal;
	std::wstring wstrIP = vstrIP.bstrVal;
	long lBuyType = vlBuyType.lVal;
	std::wstring wstrProductName = vstrProductName.bstrVal;
	std::wstring wstrProductNo = vstrProductNo.bstrVal;
	std::wstring wstrCategory = vstrCategory.bstrVal;
	long lProductCnt = vlProductCnt.lVal;
	std::wstring wstrProductEtc = vstrProductEtc.bstrVal;
	std::wstring wstrToUserUID = vstrToUserUID.bstrVal;
	std::wstring wstrToUserID = vstrToUserID.bstrVal;
	long lPrice = vlPrice.lVal;
	std::wstring wstrAgencyNo = vstrAgencyNo.bstrVal;
	std::wstring wstrEtc2 = vstrEtc2.bstrVal;
	std::wstring wstrEtc3 = vstrEtc3.bstrVal;
	std::wstring wstrEtc4 = vstrEtc4.bstrVal;
	std::wstring wstrOrderID = vstrOrderID.bstrVal;
	std::wstring wstrReturnCode = vstrOrderID.bstrVal;

	if( wstrReturnCode.compare( L"0000" ) == 0 )			// 성공(빌링 주문번호 처리)
	{
		SendToKOGBillingDB( FIRST_SENDER_UID, EBILL_GIFT_ITEM_REQ, kPacket_ );

		START_LOG( clog, L"NaunBilling 아이템 선물하기 성공" )
			<< BUILD_LOG( wstrUserUID )
			<< BUILD_LOG( wstrUserID )
			<< BUILD_LOG( wstrCompanyCode )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( wstrIP )
			<< BUILD_LOG( lBuyType )
			<< BUILD_LOG( wstrProductName )
			<< BUILD_LOG( wstrProductNo )
			<< BUILD_LOG( wstrCategory )
			<< BUILD_LOG( lProductCnt )
			<< BUILD_LOG( wstrProductEtc )
			<< BUILD_LOG( wstrToUserUID )
			<< BUILD_LOG( wstrToUserID )
			<< BUILD_LOG( lPrice )
			<< BUILD_LOG( wstrAgencyNo )
			<< BUILD_LOG( wstrEtc2 )
			<< BUILD_LOG( wstrEtc3 )
			<< BUILD_LOG( wstrEtc4 )
			<< BUILD_LOG( wstrOrderID )
			<< BUILD_LOG( wstrReturnCode )
			<< END_LOG;
	}
	else
	{	
		KEBILL_BUY_PRODUCT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GASH_02;
		SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacketAck );

		START_LOG( cerr, L"NaunBilling 아이템 선물하기 실패" )
			<< BUILD_LOG( wstrUserUID )
			<< BUILD_LOG( wstrUserID )
			<< BUILD_LOG( wstrCompanyCode )
			<< BUILD_LOG( wstrNickName )
			<< BUILD_LOG( wstrIP )
			<< BUILD_LOG( lBuyType )
			<< BUILD_LOG( wstrProductName )
			<< BUILD_LOG( wstrProductNo )
			<< BUILD_LOG( wstrCategory )
			<< BUILD_LOG( lProductCnt )
			<< BUILD_LOG( wstrProductEtc )
			<< BUILD_LOG( wstrToUserUID )
			<< BUILD_LOG( wstrToUserID )
			<< BUILD_LOG( lPrice )
			<< BUILD_LOG( wstrAgencyNo )
			<< BUILD_LOG( wstrEtc2 )
			<< BUILD_LOG( wstrEtc3 )
			<< BUILD_LOG( wstrEtc4 )
			<< BUILD_LOG( wstrOrderID )
			<< BUILD_LOG( wstrReturnCode )
			<< END_LOG;
	}

	CoUninitialize();
}

IMPL_ON_FUNC( EBILL_USE_COUPON_REQ )
{
	KEBILL_USE_COUPON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// 일본 쿠폰 사용 안함

	START_LOG( cerr, L"대만/홍콩은 퍼블 쿠폰 인증 안 받기 때문에 여기 코드 타면 안됨!!!!!!" )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrSerialCode)
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, EBILL_USE_COUPON_ACK, kPacketAck );
}
#endif SERV_COUNTRY_IN