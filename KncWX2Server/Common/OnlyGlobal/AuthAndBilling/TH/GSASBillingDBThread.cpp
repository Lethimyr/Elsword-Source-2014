#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSASBillingDBThread.h"
#include "GameServer.h"
#include "NetError.h"

#ifdef SERV_COUNTRY_TH

ImplementDBThread( KGSASBillingDBThread );
ImplPfID( KGSASBillingDBThread, PI_GS_PUBLISHER_BILLING_DB );

#define CLASS_TYPE KGSASBillingDBThread
#define AS_SERVICE_CID	114			//	AsiaSoft Elsword CID

KGSASBillingDBThread::~KGSASBillingDBThread()
{
}

void KGSASBillingDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    switch( spEvent_->m_usEventID )
    {
    CASE( EPUBLISHER_BILLING_BALANCE_REQ );
    CASE( EBILL_BUY_PRODUCT_REQ );
	CASE( EBILL_GIFT_ITEM_REQ );
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }
}

void KGSASBillingDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSASBillingDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

void KGSASBillingDBThread::SendToClient( UidType nTo, unsigned short usEventID )
{
    SendToClient( nTo, usEventID, char() );
}

void KGSASBillingDBThread::SendToBillingDB( UidType nUserUID, unsigned short usEventID )
{
    SendToBillingDB( nUserUID, usEventID, char() );
}

void KGSASBillingDBThread::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

int KGSASBillingDBThread::GetCashPoint( const std::wstring& wstrUserID, int& iCashPoint )
{
    int iRet = NetError::ERR_ODBC_01;
    DO_QUERY_NO_PROFILE( L"exec dbo.USP_request_checkbalance", L"N\'%s\'", % wstrUserID );

    int iReturnValue, iCash, iBonus;	//	iCashPoint = iCash + iBonus
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iReturnValue
            >> iCash
			>> iBonus );

        m_kODBC.EndFetch();
    }
    else
    {
        goto end_proc;
    }

	iCashPoint = iCash + iBonus;

    switch( iReturnValue )
    {
    case 0:
        iRet = NetError::NET_OK;
        break;
	case 1:	//	Invalid UserID
		iRet = NetError::ERR_GASH_03;
		break;
    case 2:	//	Record not found
        //iRet = NetError::ERR_AS_BILLING_01;
		iRet = NetError::NET_OK;	//	넘어가기로 함
        break;
	case 3:	//	invalid ServiceID
		iRet = NetError::ERR_AS_BILLING_00;
		break;
    default:
        break;
    }

end_proc:
    LOG_SUCCESS( iRet == NetError::NET_OK )
        << BUILD_LOG( iRet )
        << BUILD_LOG( NetError::GetErrStr( iRet ) )
        << BUILD_LOG( wstrUserID )
		<< BUILD_LOGc( iReturnValue )
		<< BUILD_LOG( iCashPoint )
		<< BUILD_LOG( iBonus )
        << END_LOG;

    return iRet;
}

int KGSASBillingDBThread::UseCoupon( IN const int iServerGroupID, IN const std::wstring& wstrUserID, IN const std::wstring& wstrNickName, IN const std::wstring& wstrCardPassword, IN const std::wstring& wstrIP,
              OUT int& iEventID, OUT int& iGiftID )
{
    int iRet = NetError::ERR_ODBC_01;
    DO_QUERY_NO_PROFILE( L"exec dbo.sp_ConvertCenter_Exchange", L"%d, N\'%s\', N\'%s\', N\'%s\', N\'%s\'",
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
}

int KGSASBillingDBThread::InGameLessGPoint( const std::wstring& wstrTransID, const std::wstring& wstrUserID, const std::wstring& wstrReceiverUserID, int iPoint,
                                           int iItemID, int iProductNO, const unsigned short& usQuantity, const std::wstring& wstrUserIP )
{
    int iRet = NetError::ERR_ODBC_01;

    DO_QUERY_NO_PROFILE( L"exec dbo.USP_Request_BuyItem", L"N\'%s\', N\'%s\', %d, %d, %d, %d, N\'%s\', N\'%s\'",
        % wstrUserID
		% wstrReceiverUserID
		% iPoint
		% iProductNO
		% 1						//	% usQuantity (아이템의 갯수가 아니라 ProductNO의 갯수란다)
		% 1						//	1:Game Item Shop, 2:Item Web Shop
		% wstrTransID
		% wstrUserIP
		);

    int iReturnValue;
	int iGamePoint;
	int iBonusPoint, iCashTransactionConfirm;	//	not use
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iReturnValue
            >> iGamePoint
			>> iBonusPoint
			>> iCashTransactionConfirm
            );

        m_kODBC.EndFetch();
    }
    else
    {
        goto end_proc;
    }

    switch( iReturnValue )
    {
    case 0:
        iRet = NetError::NET_OK;
        break;
	case 1:
		iRet = NetError::ERR_GASH_03;
		break;
	case 2:
	case 3:
	case 5:
	case 7:
		iRet = NetError::ERR_GASH_10;
		break;
	case 4:
		iRet = NetError::ERR_AS_BILLING_01;
		break;
	case 6:
		iRet = NetError::ERR_AS_BILLING_00;
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
		<< BUILD_LOG( iReturnValue )
        << BUILD_LOG( iPoint )
        << BUILD_LOG( wstrUserID )
        << BUILD_LOG( iItemID )
        << BUILD_LOG( iProductNO )
		<< BUILD_LOG( usQuantity )
		<< BUILD_LOG( iGamePoint )
        << BUILD_LOG( wstrUserIP )
		<< BUILD_LOG( iBonusPoint )
		<< BUILD_LOG( iCashTransactionConfirm )
        << END_LOG;

    return iRet;
}

int KGSASBillingDBThread::ReportHackUser( IN UidType iUserUID, IN const std::wstring& wstrUserID, IN UidType iUnitUID, IN const std::wstring& wstrNickName, IN UidType iServerUID, IN const std::string& strIP )
{
	int iRet = NetError::ERR_ODBC_01;
	DO_QUERY_NO_PROFILE( L"exec dbo.sp_KOG_AddSuspend", L"%d, N\'%s\', %d, N\'%s\', %d, N\'%s\'", % iUserUID % wstrUserID % iUnitUID % wstrNickName % iServerUID % strIP.c_str() );

	int iReturnValue;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iReturnValue );

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}

	START_LOG( clog, L"hack user report" )
		<< BUILD_LOG( iRet )
		<< END_LOG;

end_proc:

	return iRet;
}

IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ )
{
    KEPUBLISHER_BILLING_BALANCE_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_UNKNOWN;
    kPacket.m_ulBalance = 0;

    int iCashPoint;
    kPacket.m_iOK = GetCashPoint( kPacket_.m_wstrAccount, iCashPoint );
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// 캐시 종류를 하나로 보는곳은 일반적으로 퍼블캐쉬로 본다.
	kPacket.m_iCashType = KGlobalCashInfo::GCT_PUBLISHER_CASH;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
    if( kPacket.m_iOK == NetError::NET_OK )
    {
        kPacket.m_ulBalance = ( unsigned long )iCashPoint;
    }

	SendToUser( FIRST_SENDER_UID, EPUBLISHER_BILLING_BALANCE_ACK, kPacket );
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
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		iTotalPrice += kBillBuyInfo.m_iPoint;
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

    // 리스트에 있는 아이템에 대해 차례로 포인트를 차감한다.
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		wchar_t wszNumber[32];
		::_itow( kBillBuyInfo.m_iProductID, wszNumber, 10 );
		std::wstring wstrItemInfo = wszNumber;
		::_itow( kBillBuyInfo.m_iCategory, wszNumber, 10 );
		std::wstring wstrItemType = wszNumber;
		int iRet = InGameLessGPoint( kBillBuyInfo.m_wstrOrderID, kPacket_.m_wstrUserID, kPacket_.m_wstrUserID, kBillBuyInfo.m_iPoint, kBillBuyInfo.m_iProductID, kBillBuyInfo.m_iProductNo, kBillBuyInfo.m_usOrderQuantity, KncUtil::toWideString(kPacket_.m_strUserIP) );
		if( iRet != NetError::NET_OK )
		{
			START_LOG( cerr, L"구입 오류" )
				<< BUILD_LOG( iRet )
				<< BUILD_LOG( NetError::GetErrStr( iRet ) )
				<< BUILD_LOG( kBillBuyInfo.m_wstrOrderID)
				<< BUILD_LOG( kPacket_.m_wstrUserID )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< BUILD_LOG( kBillBuyInfo.m_iProductNo )
				<< BUILD_LOG( kBillBuyInfo.m_iProductID )
				<< BUILD_LOG( kBillBuyInfo.m_iPoint )
				<< BUILD_LOG( kBillBuyInfo.m_wstrMemo );
			
			continue;
		}
		else
		{
			kPacketReq.m_vecBillBuyInfo.push_back( kBillBuyInfo );
		}
	}

    SendToBillingDB( FIRST_SENDER_UID, EBILL_BUY_PRODUCT_REQ, kPacketReq );
}

IMPL_ON_FUNC( EBILL_GIFT_ITEM_REQ )
{
	KEBILL_GIFT_ITEM_REQ kPacketReq = kPacket_;
	kPacketReq.m_vecBillBuyInfo.clear();
	KEBILL_GIFT_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;


	// 구입 금액을 계산한다.
	int iTotalPrice = 0;
	std::vector< KBillBuyInfo >::iterator vit;
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		iTotalPrice += kBillBuyInfo.m_iPoint;
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

	// 리스트에 있는 아이템에 대해 차례로 포인트를 차감한다.
	BOOST_TEST_FOREACH( const KBillBuyInfo&, kBillBuyInfo, kPacket_.m_vecBillBuyInfo )
	{
		wchar_t wszNumber[32];
		::_itow( kBillBuyInfo.m_iProductID, wszNumber, 10 );
		std::wstring wstrItemInfo = wszNumber;
		::_itow( kBillBuyInfo.m_iCategory, wszNumber, 10 );
		std::wstring wstrItemType = wszNumber;
		int iRet = InGameLessGPoint( kBillBuyInfo.m_wstrOrderID, kPacket_.m_wstrSenderUserID, kPacket_.m_wstrReceiverUserID, kBillBuyInfo.m_iPoint, kBillBuyInfo.m_iProductID, kBillBuyInfo.m_iProductNo, kBillBuyInfo.m_usOrderQuantity, kPacket_.m_wstrUserIP );
		if( iRet != NetError::NET_OK )
		{
			START_LOG( cerr, L"구입 오류" )
				<< BUILD_LOG( iRet )
				<< BUILD_LOG( NetError::GetErrStr( iRet ) )
				<< BUILD_LOG( kBillBuyInfo.m_wstrOrderID )
				<< BUILD_LOG( kPacket_.m_wstrSenderUserID )
				<< BUILD_LOG( kPacket_.m_wstrReceiverUserID )
				<< BUILD_LOG( kBillBuyInfo.m_iProductNo )
				<< BUILD_LOG( kBillBuyInfo.m_iProductID )
				<< BUILD_LOG( kBillBuyInfo.m_iPoint )
				<< BUILD_LOG( kBillBuyInfo.m_usOrderQuantity )
				<< BUILD_LOG( kPacket_.m_wstrUserIP );

			continue;
		}
		else
		{
			kPacketReq.m_vecBillBuyInfo.push_back( kBillBuyInfo );
		}
	}

#ifdef SERV_BILLING_ADD_GASH_POINT
	kPacketReq.m_bIsGamePoint = true;
#endif
	SendToBillingDB( FIRST_SENDER_UID, EBILL_GIFT_ITEM_REQ, kPacketReq );
}

#endif //SERV_COUNTRY_TH
