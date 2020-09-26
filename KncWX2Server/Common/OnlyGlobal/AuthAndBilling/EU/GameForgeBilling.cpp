#include ".\gameforgebilling.h"

#ifdef SERV_COUNTRY_EU

#include "NetError.h"


#include "SoapGameForge/soapH.h" // or whatever it is called, you must already have it
#include "SoapGameForge/ElswordBinding.nsmap" // this is what you have to add to fix the problem


#define CLASS_TYPE  KGameForgeBilling
ImplPfID( KGameForgeBilling, PI_GS_PUBLISHER_BILLING );


std::map< int, KGameForgeBilling::KWebMethod >  KGameForgeBilling::ms_mapWebMethodPath;

_ImplementException( KGameForgeBilling );



KGameForgeBilling::KGameForgeBilling(std::string WsdlFile)
{
	m_strWsdlPath = WsdlFile;
	//	m_WsmlFile = WsmlFile;

	p_gf_soap = NULL;
}


KGameForgeBilling::~KGameForgeBilling(void)
{
	End( 1500 );
}


void KGameForgeBilling::Run()     // derived from KThread
{
	std::cout << "KSubThread::Run(), thread start." << std::endl;
	START_LOG( cout2, L"[Test Log] KGameForgeBilling Start" )
		<< END_LOG;



	// 추가된 코드 //
	SoapInitialize();
	//


	START_LOG( cout2, L"[테스트로그] KGameForgeBilling Soap Initialized" )
		<< END_LOG;

	int tmp_value = 0;
	GetCashPoint(L"jin1234",tmp_value);
	GetCashPoint(L"jin5678",tmp_value);

	while( true )
	{
		//{{ 2010/11/11 조효진 SOAP 모듈 정상 생성 실패 시 다시 생성을 위해 //
		if( p_gf_soap == NULL ){
			SoapInitialize();
		}
		//}}

		DWORD ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

		else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

	}

	// 추가된 코드 //
	SoapUninitialize();
	//

	START_LOG( cout2, L"[테스트로그] KGameForgeBilling Soap End" )
		<< END_LOG;
}



void KGameForgeBilling::SoapInitialize()
{
	p_gf_soap = soap_new();
	if( !p_gf_soap )	//// couldn't allocate: stop
	{
		p_gf_soap = NULL;

		START_LOG( cerr, L"Soap_new Error" )
			<< END_LOG;

		return;
	}
}


void KGameForgeBilling::SoapUninitialize()
{
	if( p_gf_soap != NULL )
	{
		soap_destroy(p_gf_soap);
		soap_end(p_gf_soap);
		p_gf_soap = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
void KGameForgeBilling::AddWebMethod( int iSoapAction, std::string szMethod )
{
	if( iSoapAction < 0 || iSoapAction >= GFBilling_NUM )
	{
		START_LOG( cerr, L"이상한 SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}


	if( szMethod == "" )
	{
		START_LOG( cerr, L"method 이상." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	KWebMethod kMethod;
	kMethod.m_iSoapAction = iSoapAction;
	kMethod.m_strMethodPath = szMethod;

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit != ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"이미 존재하는 soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return;
	}

	// 무조건 넣는다.
	ms_mapWebMethodPath[iSoapAction] = kMethod;
}

bool KGameForgeBilling::GetWebMethod( int iSoapAction, KWebMethod& kInfo )
{
	if( iSoapAction < 0 || iSoapAction >= GFBilling_NUM )
	{
		START_LOG( cerr, L"이상한 SOAP Action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	std::map< int, KWebMethod >::iterator mit = ms_mapWebMethodPath.find( iSoapAction );
	if( mit == ms_mapWebMethodPath.end() )
	{
		START_LOG( cerr, L"존재하지 않는 soap action." )
			<< BUILD_LOG( iSoapAction )
			<< END_LOG;

		return false;
	}

	kInfo = mit->second;

	return true;
}



// packet send function

void KGameForgeBilling::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KGameForgeBilling::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}


void KGameForgeBilling::SendToKOGBillingDB( UidType nUserUID, unsigned short usEventID )
{
	SendToKOGBillingDB( nUserUID, usEventID, char() );
}

//----------------------//
// Process Event handle //
//----------------------//
void KGameForgeBilling::ProcessEvent( const KEventPtr& spEvent_ )
{
	KSerializer ks;
	ks.BeginReading( &spEvent_->m_kbuff );

	switch( spEvent_->m_usEventID )
	{
		CASE( EPUBLISHER_BILLING_BALANCE_REQ );
		
		CASE( EBILL_BUY_PRODUCT_REQ );
		CASE( EBILL_GIFT_ITEM_REQ );
		
		CASE( EBILL_USE_COUPON_REQ );

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
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

    int iCashPoint;
    kPacketAck.m_iOK = GetCashPoint( kPacket_.m_wstrAccount, iCashPoint );

    if( kPacketAck.m_iOK == NetError::NET_OK )
    {
        kPacketAck.m_ulBalance = ( unsigned long )iCashPoint;
    }

    SendToUser( FIRST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacketAck );
}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_REQ )
{
    KEBILL_BUY_PRODUCT_REQ kPacketReq = kPacket_;
    
	// 목록 전체를 한번에 요청하기 때문에 
	// 구매 목록을 clear 후 성공한것만 push하는 과정 필요없음 //

	//kPacketReq.m_vecBillBuyInfo.clear();
    KEBILL_BUY_PRODUCT_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_UNKNOWN;

    // 구입 금액과 아이템 목록을 구성한다. 
    int iTotalPrice = 0;
	
	// sTotalList 는 아이템마다 ProductNo_ProductID_count_price 로 구성되며 price는 count를 고려한 해당 물품 총액을 의미
	// 여러개의 아이템 시 ProductNo_ProductID_count_price|ProductNo_ProductID_count_price 이렇게 됨
	std::string strTotalList = "";



	int count = 0;
    std::vector< KBillBuyInfo >::iterator vit;
    for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); vit++ )
    {
        char tmp[1024];
		char* p_tmp = NULL;


		if( vit != kPacket_.m_vecBillBuyInfo.begin() )
			strTotalList += "|";

		p_tmp = _itoa( vit->m_iProductNo, tmp, 10 ); // 10진수 (int) 
		strTotalList += p_tmp;
		strTotalList += "_";

		p_tmp = _itoa( vit->m_iProductID, tmp, 10 ); // 10진수 (int)
		strTotalList += p_tmp;
		strTotalList += "_";

		//p_tmp = _itoa( vit->m_usOrderQuantity, tmp, 10 );
		//strTotalList += p_tmp;
		//strTotalList += "_";

		p_tmp = _itoa( vit->m_iPoint, tmp, 10 );
		strTotalList += p_tmp;
		//strTotalList += "_";
		
		
		iTotalPrice += vit->m_iPoint;


		count++;
    }


    // 잔액을 확인한다.
    int iBalance;
    kPacket.m_iOK = GetCashPoint( kPacket_.m_wstrUserID, iBalance );
    if( kPacket.m_iOK != NetError::NET_OK )
    {
        SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacket );
        return;
    }

    if( iBalance < iTotalPrice )
    {
        kPacket.m_iOK = NetError::ERR_GASH_09;
        SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacket );
        return;
    }

	int iTmpBalance = 0;

	// 리스트에 있는 아이템에 대해 한꺼번에 포인트를 차감한다. 
	int iRet = ReduceCashPoint( kPacket_.m_wstrTotalOrderID, kPacket_.m_wstrUserID,
		kPacket_.m_iServerGroupID, kPacket_.m_iUnitUID,	count, strTotalList, iTmpBalance );
		

	if( iRet != NetError::NET_OK )
	{
		START_LOG( cerr, L"구입 오류" )
			<< BUILD_LOG( iRet )
			<< BUILD_LOG( NetError::GetErrStr( iRet ) )
			<< BUILD_LOG( kPacket_.m_wstrTotalOrderID )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( strTotalList )	//<< BUILD_LOG( vit->m_wstrMemo )	// 여기서 vit 접근하면 예상 불가능한 에러 날수 있음
			<< END_LOG;

		kPacket.m_iOK = iRet;
		SendToUser( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_ACK, kPacket );
		return;
	}

/*
    // 리스트에 있는 아이템에 대해 차례로 포인트를 차감한다.
    for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); vit++ )
    {
		

        wchar_t wszNumber[32];
        ::_itow( vit->m_iProductID, wszNumber, 10 );
        std::wstring wstrItemInfo = wszNumber;
        ::_itow( vit->m_iCategory, wszNumber, 10 );
        std::wstring wstrItemType = wszNumber;
		int iRet = NetError::NET_OK;
     //   int iRet = InGameLessGPoint( kPacket_.m_wstrOrderID, kPacket_.m_iServerGroupID, kPacket_.m_wstrUserID, kPacket_.m_wstrNickName, vit->m_iPoint, vit->m_iProductID, vit->m_iProductNo, vit->m_wstrMemo );
        if( iRet != NetError::NET_OK )
        {
            START_LOG( cerr, L"구입 오류" )
                << BUILD_LOG( iRet )
                << BUILD_LOG( NetError::GetErrStr( iRet ) )
                << BUILD_LOG( kPacket_.m_wstrOrderID )
                << BUILD_LOG( kPacket_.m_wstrUserID )
                << BUILD_LOG( kPacket_.m_wstrNickName )
                << BUILD_LOG( vit->m_iProductNo )
                << BUILD_LOG( vit->m_iProductID )
                << BUILD_LOG( vit->m_iPoint )
                << BUILD_LOG( vit->m_wstrMemo );

            continue;
        }
        else
        {
            kPacketReq.m_vecBillBuyInfo.push_back( *vit );
        }
    }
//*/
	

    SendToKOGBillingDB( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_REQ, kPacketReq );
}

IMPL_ON_FUNC( EBILL_GIFT_ITEM_REQ )
{
    KEBILL_GIFT_ITEM_REQ kPacketReq = kPacket_;
    
	// 목록 전체를 한번에 요청하기 때문에 
	// 구매 목록을 clear 후 성공한것만 push하는 과정 필요없음 //

	//kPacketReq.m_vecBillBuyInfo.clear();
	KEBILL_GIFT_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;

    // 구입 금액과 아이템 목록을 구성한다. 
    int iTotalPrice = 0;
	
	// sTotalList 는 아이템마다 ProductNo_ProductID_count_price 로 구성되며 price는 count를 고려한 해당 물품 총액을 의미
	// 여러개의 아이템 시 ProductNo_ProductID_count_price|ProductNo_ProductID_count_price 이렇게 됨
	std::string strTotalList = "";



	int count = 0;
    std::vector< KBillBuyInfo >::iterator vit;
    for( vit = kPacket_.m_vecBillBuyInfo.begin(); vit != kPacket_.m_vecBillBuyInfo.end(); vit++ )
    {
        char tmp[1024];
		char* p_tmp = NULL;


		if( vit != kPacket_.m_vecBillBuyInfo.begin() )
			strTotalList += "|";

		p_tmp = _itoa( vit->m_iProductNo, tmp, 10 ); // 10진수 (int) 
		strTotalList += p_tmp;
		strTotalList += "_";

		p_tmp = _itoa( vit->m_iProductID, tmp, 10 ); // 10진수 (int)
		strTotalList += p_tmp;
		strTotalList += "_";

		//p_tmp = _itoa( vit->m_usOrderQuantity, tmp, 10 );
		//strTotalList += p_tmp;
		//strTotalList += "_";

		p_tmp = _itoa( vit->m_iPoint, tmp, 10 );
		strTotalList += p_tmp;
		//strTotalList += "_";
		
		
		iTotalPrice += vit->m_iPoint;


		count++;
    }


    // 잔액을 확인한다.
    int iBalance;
    kPacket.m_iOK = GetCashPoint( kPacket_.m_wstrSenderUserID, iBalance );
    if( kPacket.m_iOK != NetError::NET_OK )
    {
        SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacket );
        return;
    }

    if( iBalance < iTotalPrice )
    {
        kPacket.m_iOK = NetError::ERR_GASH_09;
        SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacket );
        return;
    }

	int iTmpBalance = 0;

	// 리스트에 있는 아이템에 대해 한꺼번에 포인트를 차감한다. 
	int iRet = ReduceCashPoint( kPacket_.m_wstrTotalOrderID, kPacket_.m_wstrSenderUserID,
		kPacket_.m_iServerGroupID, kPacket_.m_iSenderUnitUID,	count, strTotalList, iTmpBalance );
		

	if( iRet != NetError::NET_OK )
	{
		START_LOG( cerr, L"구입 오류" )
			<< BUILD_LOG( iRet )
			<< BUILD_LOG( NetError::GetErrStr( iRet ) )
			<< BUILD_LOG( kPacket_.m_wstrTotalOrderID )
			<< BUILD_LOG( kPacket_.m_wstrSenderUserID )
			<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
			<< BUILD_LOG( strTotalList )	//<< BUILD_LOG( vit->m_wstrMemo )	// 여기서 vit 접근하면 예상 불가능한 에러 날수 있음
			<< END_LOG;

		kPacket.m_iOK = iRet;
		SendToUser( FIRST_SENDER_UID, EBILL_GIFT_ITEM_ACK, kPacket );
		return;
	}

    SendToKOGBillingDB( FIRST_SENDER_UID, EBILL_GIFT_ITEM_REQ, kPacketReq );
}


IMPL_ON_FUNC( EBILL_USE_COUPON_REQ )
{
	// 유럽은 쿠폰 사용안함
	KEBILL_USE_COUPON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

	START_LOG( cerr, L"유럽은 퍼블 쿠폰 인증 안 받기 때문에 여기 코드 타면 안됨!!!!!!" )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_PurchaserInfo.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrSerialCode )
		<< END_LOG;


	SendToUser( FIRST_SENDER_UID, EBILL_USE_COUPON_ACK, kPacketAck );
}

//---------------------------------------------------------------------------------//

int KGameForgeBilling::GetCashPoint( IN const std::wstring& wstrUserID, OUT int& iCashPoint )
{

	//{{ 2010. 11. 25. 조효진	아무것도 입력되지 않은 경우 에러 처리함
	if( wstrUserID == L"" )
	{
		return NetError::ERR_GASH_03;
	}
	//

	const int MaxNameSize = 256;

	//{{ 2010. 11. 24. 조효진 마지막 NULL값 보장해 주기 위해 수정
	char tmp_input_userName[MaxNameSize+1] = {0,};
	tmp_input_userName[MaxNameSize] = NULL;
	//

	// init //
	int iResult = -1;
	

	//struct _ns1__getCashFromWebDB tmp_input; 
	//struct _ns1__getCashFromWebDBResponse tmp_output;


	strncpy(tmp_input_userName,KncUtil::toNarrowString(wstrUserID).c_str(), MaxNameSize ); 
	//tmp_input.userName = tmp_input_userName;



	KWebMethod tmp_web_path;
	if( GetWebMethod(GFBilling_GetCash, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod GFBilling_GetCash" )
			<< BUILD_LOG( tmp_input_userName )
			<< END_LOG;

		
		return NetError::ERR_GASH_00;
	}


	//{{ 2010/11/11 조효진	soap 함수 호출 전에는 혹시 모르니 항상 확인하자.
	if( p_gf_soap == NULL )
	{// soap 모듈 정상 생성 안된 경우 // 예외처리
		return NetError::ERR_GASH_10;
	}

	//Amount of cash as integer.  
	// If wrong parameter it will return -1  If user not found it will return -2
// 	int soap_call_result = soap_call_ns1__getCashFromWebDB(p_gf_soap,
// 									m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
// 									&tmp_input, &tmp_output );
	int soap_call_result = soap_call_ns1__getCashFromWebDB(p_gf_soap,
									m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
									tmp_input_userName, &iResult );

	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{
		START_LOG( cerr, L"[ErrorLog] getCashFromWebDB ERROR. " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< END_LOG;

		return NetError::ERR_GASH_00;
	}


// 	if( tmp_output.getCashFromWebDBResult != NULL )
// 		iResult = *tmp_output.getCashFromWebDBResult;

	// Success Case //
	if ( iResult >= 0 )
	{
		iCashPoint = iResult;
		iResult = NetError::NET_OK;
	}
	// Faill Case //
	else
	{
		switch( iResult )
		{
		case -1:	// -1 = Wrong Parameter
			{
				iResult = NetError::ERR_GASH_00;
			}
			break;
		case -2:	// -2 = user not found
			{
				iResult = NetError::ERR_GASH_02;
			}
			break;
		default:	// Undefined Error
			{
				iResult = NetError::ERR_GASH_00;
			}
		}
	}

	START_LOG( clog, L"[테스트로그] GF GetCashPoint result" )
		<< BUILD_LOG( wstrUserID )
		<< BUILD_LOG( iResult )
		<< BUILD_LOG( iCashPoint )
		<< END_LOG;

// end_proc:
// 	LOG_SUCCESS( iRet == NetError::NET_OK )
// 		<< BUILD_LOG( iRet )
// 		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
// 		<< BUILD_LOG( wstrUserID )
// 		<< BUILD_LOG( iCashPoint )
// 		<< BUILD_LOGc( iReturnValue )
// 		<< END_LOG;

	return iResult;
}


int KGameForgeBilling::ReduceCashPoint( IN const std::wstring wstrOrderID, IN const std::wstring& wstrUserID, IN int iServerGroupID, IN UidType characterID, 
										   IN int countLists, IN const std::string& products, OUT int& iCashPoint )
{
	const int MAXOrderID = 256;
	const int MaxNameSize = 256;
	const int MaxProductsSize = 4096;

	//{{ 2010. 12. 03. 조효진 마지막 NULL값 보장해 주기 위해 수정
	char tmp_input_orderID[MAXOrderID+1] = {0,};
	char tmp_input_userName[MaxNameSize+1] = {0,};
	char tmp_input_products[MaxProductsSize+1] = {0,};
	tmp_input_orderID[MAXOrderID] = NULL;
	tmp_input_userName[MaxNameSize] = NULL;
	tmp_input_products[MaxProductsSize] = NULL;
	//}}


	// string 길이 문제 있을 경우 //
	if( wstrOrderID.length() >= MAXOrderID || wstrUserID.length() >= MaxNameSize || products.length() >= MaxProductsSize )
	{
		START_LOG( cerr, L"ReduceCashPoint  String Size  Overflow" )
			<< BUILD_LOG( wstrOrderID.size() )
			<< BUILD_LOG( wstrUserID.size() )
			<< BUILD_LOG( products.size() )
			<< END_LOG;

		return NetError::ERR_GASH_10;
	}


	// init //
	int iResult = -1;


//	struct _ns1__reduceCashOnWebDB tmp_input; 
//	struct _ns1__reduceCashOnWebDBResponse tmp_output;


	strncpy(tmp_input_orderID, KncUtil::toNarrowString(wstrOrderID).c_str(), MAXOrderID ); 
//	tmp_input.orderID = tmp_input_orderID;
	strncpy(tmp_input_userName, KncUtil::toNarrowString(wstrUserID).c_str(), MaxNameSize ); 
//	tmp_input.userName = tmp_input_userName;
	strncpy(tmp_input_products, products.c_str(), MaxProductsSize );
//	tmp_input.products = tmp_input_products;

	
//	tmp_input.characterID = &characterID;	//추후 수정 필요
//	tmp_input.countProducts = &countLists;		//추후 수정 필요 
//	tmp_input.serverGroupID = &iServerGroupID;

	

	KWebMethod tmp_web_path;
	if( GetWebMethod(GFBilling_ReduceCash, tmp_web_path) == false )
	{
		START_LOG( cerr, L"[ErrorLog] GetWebMethod ERROR. You must AddwebMethod GFBilling_ReduceCash" )
			<< BUILD_LOG( tmp_input_orderID )
			<< BUILD_LOG( tmp_input_userName )
			<< BUILD_LOG( characterID )
			<< END_LOG;


		return NetError::ERR_GASH_10;
	}


	//{{ 2010/11/11 조효진	soap 함수 호출 전에는 혹시 모르니 항상 확인하자.
	if( p_gf_soap == NULL )
	{// soap 모듈 정상 생성 안된 경우 // 예외처리
		return NetError::ERR_GASH_10;
	}

	//Amount of cash as integer.  
	// If wrong parameter it will return -1  If user not found it will return -2
// 	int soap_call_result = soap_call___ns1__reduceCashOnWebDB(p_gf_soap,
// 									m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
// 									&tmp_input, &tmp_output );
	int soap_call_result = soap_call_ns1__reduceCashOnWebDB(p_gf_soap,
									m_strWsdlPath.c_str(), tmp_web_path.m_strMethodPath.c_str(),
									tmp_input_orderID, tmp_input_userName, iServerGroupID,
									characterID, countLists, tmp_input_products, &iResult );
									
	// Soap 호출 오류 //
	if( 0 != soap_call_result )
	{

		START_LOG( cerr, L"[ErrorLog] GFBilling_ReduceCash ERROR. " )
			<< BUILD_LOG( m_strWsdlPath )
			<< BUILD_LOG( tmp_web_path.m_strMethodPath )
			<< END_LOG;

		return NetError::ERR_GASH_10;
	}

	
// 	if( tmp_output.reduceCashOnWebDBResult != NULL )
// 		iResult = *tmp_output.reduceCashOnWebDBResult;

	// Success Case //
	if ( iResult >= 0 )
	{
		iCashPoint = iResult;
		iResult = NetError::NET_OK;
	}
	// Faill Case //
	else
	{
		// for output
		int iTmp = iResult;

		switch( iResult )
		{
		case -1:	// -1 = Wrong Parameter
			{
				iResult = NetError::ERR_GASH_00;
			}
			break;
		case -2:	// -2 = user not found
			{
				iResult = NetError::ERR_GASH_02;
			}
			break;
		case -3:	// -3 = Error in product list
			{
				iResult = NetError::ERR_BUY_CASH_ITEM_00;
			}
			break;
		case -8:	// -8 = SOAP Service Exception
			{
				iResult = NetError::ERR_GASH_10;
			}
			break;
		default:	// Undefined Error
			{
				iResult = NetError::ERR_GASH_00;
			}
			break;
		}

		START_LOG( cerr, L"[ErrorLog] ReduceCashPoint. " )
			<< BUILD_LOG( wstrOrderID )
			<< BUILD_LOG( wstrUserID )
			<< BUILD_LOG( products )
			<< BUILD_LOG( iTmp )
			<< BUILD_LOG( iResult )
			<< END_LOG;
	}

	START_LOG( clog, L"[SOAP TEST LOG] GF ReduceCashPoint result" )
		<< BUILD_LOG( wstrUserID )
		<< BUILD_LOG( iResult )
		<< BUILD_LOG( iCashPoint )
		<< END_LOG;

	// end_proc:
	// 	LOG_SUCCESS( iRet == NetError::NET_OK )
	// 		<< BUILD_LOG( iRet )
	// 		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
	// 		<< BUILD_LOG( wstrUserID )
	// 		<< BUILD_LOG( iCashPoint )
	// 		<< BUILD_LOGc( iReturnValue )
	// 		<< END_LOG;

	return iResult;
}

int KGameForgeBilling::UseCoupon( IN const int iServerGroupID, IN const std::wstring& wstrUserID, IN const std::wstring& wstrNickName, IN const std::wstring& wstrCardPassword, IN const std::wstring& wstrIP,
									OUT int& iEventID, OUT int& iGiftID )
{

	int iResult = NetError::NET_OK;
	return iResult;
/*

	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.sp_ConvertCenter_Exchange", L"%d, \'%s\', \'%s\', \'%s\', \'%s\'",
		% iServerGroupID % wstrUserID % wstrNickName % wstrCardPassword % wstrIP );

	int iReturnValue;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iReturnValue
			>> iEventID
			>> iGiftID );

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	switch( iReturnValue )
	{
	case 1:
		iRet = NetError::NET_OK;
		break;
	case 2:
		iRet = NetError::ERR_GASH_01;
		break;
	case 4:
		iRet = NetError::ERR_GASH_02;
		break;
	case 8:
		iRet = NetError::ERR_GASH_03;
		break;
	case 10:
		iRet = NetError::ERR_GASH_15;
		break;
	case 12:
		iRet = NetError::ERR_GASH_04;
		break;
	case 13:
		iRet = NetError::ERR_GASH_05;
		break;
	case 14:
		iRet = NetError::ERR_GASH_06;
		break;
	case 33:
		iRet = NetError::ERR_GASH_07;
		break;
	case 99:
		iRet = NetError::ERR_GASH_08;
		break;
	default:
		break;
	}

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iServerGroupID )
		<< BUILD_LOG( wstrUserID )
		<< BUILD_LOG( wstrNickName )
		<< BUILD_LOG( wstrCardPassword )
		<< BUILD_LOG( wstrIP )
		<< BUILD_LOG( iReturnValue )
		<< BUILD_LOG( iEventID )
		<< BUILD_LOG( iGiftID )
		<< END_LOG;

	return iRet;
//*/
}

int KGameForgeBilling::GetCouponItem( IN const int iEventID, IN const int iGiftID,
										OUT std::vector< KBillCouponItemInfo >& vecGASHCouponItemInfo )
{
	int iResult = NetError::NET_OK;
	return iResult;
/*
    int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.spCoupenItemList '%d','%d'", L"", % iEventID % iGiftID );

	while( m_kODBC.Fetch() )
	{
		KGASHCouponItemInfo kInfo;
		//{{ 09. 11. 13 김정협 Gash 쿠폰패킷 수정
		FETCH_DATA( kInfo.m_iProductNO
			>> kInfo.m_iCount            
			);
		//}} 09. 11. 13 김정협 Gash 쿠폰패킷 수정

		vecGASHCouponItemInfo.push_back( kInfo );
	}

	iRet = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iEventID )
		<< BUILD_LOG( iGiftID )
		<< BUILD_LOG( vecGASHCouponItemInfo.size() )
		<< END_LOG;

	return iRet;
//*/
}

int KGameForgeBilling::InGameLessGPoint( const std::wstring& wstrTransID, int iSvr, const std::wstring& wstrUserID, const std::wstring& wstrNickName, int iPoint,
										   int iItemID, int iProductID, const std::wstring& wstrMemo )
{



	int iRet = NetError::ERR_ODBC_01;

	// 임시
	return iRet;

	//DO_QUERY_NO_PROFILE( L"exec dbo.InGameLessGPoint", L"\'%s\', \'%s\', %d, \'%s\', \'%s\', %d, \'%s\', \'%s\', \'%s\'",
	//    % L"EW" % wstrTransID % iSvr % wstrUserID % wstrNickName % iPoint % wstrItemInfo % wstrItemType % wstrMemo );
/*
	DO_QUERY_NO_PROFILE( L"exec dbo.ELSLessGPoint", L"\'%s\', \'%s\', %d, \'%s\', N\'%s\', %d, %d, %d, N\'%s\'",
		% L"EW" % wstrTransID % iSvr % wstrUserID % wstrNickName % iPoint % iItemID % iProductID % wstrMemo );

	int iGamePoint;
	int iReturnValue;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iGamePoint
			>> iReturnValue
			);

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	switch( iReturnValue )
	{
	case 1:
		iRet = NetError::NET_OK;
		break;
	case 2:
		iRet = NetError::ERR_GASH_09;
		break;
	case 9:
		iRet = NetError::ERR_GASH_10;
		break;
	default:
		START_LOG( cerr, L"not defined return value" )
			<< BUILD_LOG( iReturnValue )
			<< END_LOG;
		break;
	}

end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( NetError::GetErrStr( iRet ) )
		<< BUILD_LOG( iGamePoint )
		<< BUILD_LOG( wstrUserID )
		<< BUILD_LOG( wstrNickName )
		<< BUILD_LOG( iItemID )
		<< BUILD_LOG( iProductID )
		<< BUILD_LOG( wstrMemo )
		<< END_LOG;

	return iRet;
//*/
}

#endif // SERV_COUNTRY_EU