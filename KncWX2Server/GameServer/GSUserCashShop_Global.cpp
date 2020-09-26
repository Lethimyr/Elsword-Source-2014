#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "NexonBillingTCPManager.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"


//{{ 2010. 11. 15  조효진	include 추가
#include "GSSimLayer.h"
//}}

//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	#include "GSContentManager.h"
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	#include "ClassChangeTable.h"
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	#include "NexonSOAPManager.h"
	#include "NexonSOAP.h"
#endif SERV_NEXON_AUTH_SOAP
//}}

//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	#include "..\Common\GameEvent\DailyGiftBoxManager.h"
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 10. 2	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	#include "..\Common\X2Data\XSLRandomItemManager.h"
	#include "..\Common\GameSysVal\GameSysVal.h"
#endif SERV_EVENT_BINGO
//}}

//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
#include "X2Data/XSLSocketItem.h"
#endif SERV_CASH_ITEM_SOCKET_OPTION
//}

#ifdef SERV_EXCEPT_NO_SELL
#include "X2Data/XSLCashItemManager.h"
#endif //SERV_EXCEPT_NO_SELL


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//======================================================================//
#ifdef SERV_GLOBAL_BILLING

IMPL_ON_FUNC_NOPARAM( EGS_CHECK_BALANCE_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHECK_BALANCE_ACK );

	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"캐쉬샵 점검중 잔액 요청" )
			<< BUILD_LOG( GetName() );			

		KEGS_CHECK_BALANCE_ACK kAck;
		kAck.m_iOK = NetError::NET_OK;
		kAck.m_ulBalance = 0;
		SendPacket( EGS_CHECK_BALANCE_ACK, kAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_CHECK_BALANCE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;
	kPacket.m_ulBalance = 0;
	// kPacket.m_GlobalCashInfo 생성자에서 초기화됨

	DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
	DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
	DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();

	if (dwBillingFlag == KSimLayer::BF_NONE || dwBillingFlag == KSimLayer::BF_INTERNAL)
	{
		// 사내 빌링일 때는 중국 내부 캐시 차감 프로세스를 활용함
		KEBILL_CHECK_BALANCE_REQ kPacket;
		kPacket.m_iPublisherCashBalance = 0;
		SendToKOGBillingDB( EBILL_CHECK_BALANCE_REQ, kPacket );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
	{
		// 대만, 홍콩 바이너리 같은거 쓰면서 구분되어야 해서 국가플래그로 사용
		// 대만은 TCP 빌링, DB빌링 다 사용함. 구매나 선물 시에는 조건 체크 후 둘중 하나 보냄

		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiActorUID = GetUID();
		kPacket.m_wstrAccount = GetName();
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );;

		// DB Billing
		SendToPublisherBillingDB( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );

		// TCP Billing
		SendToPublisherBilling( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
	else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
	{
		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiActorUID = GetUID();
		kPacket.m_wstrAccount = GetName();
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );;
		SendToPublisherBillingDB( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
#ifdef SERV_COUNTRY_CN
	else if( dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL )
	{
		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_wstrAccount = GetName();
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );;

		START_LOG( clog, L"EPUBLISHER_BILLING_BALANCE_REQ" )
			<< BUILD_LOG( kPacket.m_uiPublisherUID )
			<< BUILD_LOG( kPacket.m_wstrAccount )
			<< END_LOG;

		SendToLoginServer( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
#endif // SERV_COUNTRY_CN
#ifdef SERV_COUNTRY_PH
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_PH)
	{		
		// 동남아시아는 엘소드 캐쉬 KOG에서 관리 
		KEBILL_CHECK_BALANCE_REQ kPacket;
		kPacket.m_iPublisherCashBalance = 0;
		SendToKOGBillingDB( EBILL_CHECK_BALANCE_REQ, kPacket );
	}
#endif //SERV_COUNTRY_PH
	else // if(dwPublisherBillingConnectType == KSimLayer::PBCT_NORMAL)
	{
		KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
		kPacket.m_uiActorUID = GetUID();
		kPacket.m_wstrAccount = GetName();
		kPacket.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_wstrIP = KncUtil::toWideString( GetIPStr() );
#ifdef SERV_COUNTRY_IN
		kPacket.m_wstrNickName = GetCharName();
#endif SERV_COUNTRY_IN
		SendToPublisherBilling( EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	}
}

IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHECK_BALANCE_ACK );

#ifdef SERV_COUNTRY_CN
	// 중국은 실제 캐시 + 사내 전환된 캐시 둘다 보여줘야 하므로
	START_LOG(clog, L"자이언트 캐시 잔액 조회")
		<< BUILD_LOG( kPacket_.m_ulBalance )
		<< END_LOG;

	KEBILL_CHECK_BALANCE_REQ kPacket;
	kPacket.m_iPublisherCashBalance = kPacket_.m_ulBalance;
	SendToKOGBillingDB( EBILL_CHECK_BALANCE_REQ, kPacket );
	//m_GlobalCashInfo 값 최종 캐시값 다 받을 때 채워짐

#else // SERV_COUNTRY_CN
    KEGS_CHECK_BALANCE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_ulBalance = kPacket_.m_ulBalance;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	// EPUBLISHER_BILLING_BALANCE_ACK는 한가지 타입에 대한 캐시임.
	// 중국의 경우는 위처럼 특이 처리 후 아래로 _IMPL_ON_FUNC( EBILL_CHECK_BALANCE_ACK, KEGS_CHECK_BALANCE_ACK )에서 처리함
	
	if(kPacket_.m_iCashType >= 0 && kPacket_.m_iCashType < KGlobalCashInfo::GCT_TOTAL_NUM)
	{
		kPacket.m_bOnlyType = true;
		kPacket.m_iCashType = kPacket_.m_iCashType;

		kPacket.m_GlobalCashInfo.m_ulCash[kPacket_.m_iCashType] = kPacket_.m_ulBalance;

		m_GlobalCashInfo.m_ulCash[kPacket_.m_iCashType] = kPacket_.m_ulBalance;
	}
	else if(kPacket_.m_iOK == NetError::NET_OK)
	{
		START_LOG( cerr, L"정상 타입의 캐시가 아닌데 어떻게 NET_OK가 나오지!!!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_iCashType )
			<< BUILD_LOG( kPacket_.m_ulBalance)
			<< END_LOG;


		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SendPacket( EGS_CHECK_BALANCE_ACK, kPacket );
#endif // SERV_COUNTRY_CN
}

// 중국 서버 빌링 플로우에서 사용하는 함수(사내 버전도 해당 플로우 사용)
_IMPL_ON_FUNC( EBILL_CHECK_BALANCE_ACK, KEGS_CHECK_BALANCE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHECK_BALANCE_ACK );

	m_GlobalCashInfo = kPacket_.m_GlobalCashInfo;

	SendPacket( EGS_CHECK_BALANCE_ACK, kPacket_ );
}
//}}

IMPL_ON_FUNC( EGS_APPLY_COUPON_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_APPLY_COUPON_ACK );

	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"캐쉬샵 점검중 쿠폰 사용 요청" )
			<< BUILD_LOG( GetName() );

		KEGS_APPLY_COUPON_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	START_LOG( clog, L"쿠폰 입력." )
		<< BUILD_LOG( kPacket_.m_wstrCouponSerial );

	KEGS_APPLY_COUPON_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_UNKNOWN;

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		kPacketAck.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}


#if defined (SERV_COUNTRY_TWHK)
	const int iCouponSerialSize = 20;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_TH)
	const int iCouponSerialSize = 15;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_CN)
	const int iCouponSerialSize = 20;
	int iSerialSizeNetError = NetError::ERR_GIANT_BILLING_03;
#elif defined (SERV_COUNTRY_JP)
	const int iCouponSerialSize = 30;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_US)
	const int iCouponSerialSize = 16;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#elif defined (SERV_COUNTRY_EU) || defined (SERV_COUNTRY_PH)
	const int iCouponSerialSize = 0;
	int iSerialSizeNetError = NetError::ERR_UNKNOWN;	// 유럽은 쿠폰 사용이 안됨
#else	
	const int iCouponSerialSize = 20;
	int iSerialSizeNetError = NetError::ERR_NX_WEB_SERVICE_01;
#endif // SERV_COUNTRY_TWHK


	// 쿠폰 값 길이 체크
	if( kPacket_.m_wstrCouponSerial.size() != iCouponSerialSize 
		|| iCouponSerialSize == 0 )
	{
		kPacketAck.m_iOK = iSerialSizeNetError;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}

	START_LOG( clog, L"쿠폰 사용 요청" )
		<< BUILD_LOG( kPacket_.m_wstrCouponSerial )
		<< END_LOG;


	// 국가가 다르더라도 전달 정보 구성은 동일하게 맞췄으며 어디로 보낼지가 중요함
	KEBILL_USE_COUPON_REQ kPacketReq;
	kPacketReq.m_PurchaserInfo.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketReq.m_PurchaserInfo.m_iUserUID = GetUID();
	kPacketReq.m_PurchaserInfo.m_wstrUserName = GetName();
	kPacketReq.m_PurchaserInfo.m_iUnitUID = GetCharUID();
	kPacketReq.m_PurchaserInfo.m_wstrUnitNickName = GetCharName();
	kPacketReq.m_PurchaserInfo.m_ucLevel = GetLevel();
	kPacketReq.m_PurchaserInfo.m_wstrIP = KncUtil::toWideString( GetIPStr() );
	kPacketReq.m_PurchaserInfo.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
	kPacketReq.m_PurchaserInfo.m_iChannelingCode = GetChannelCode();
	kPacketReq.m_wstrSerialCode = kPacket_.m_wstrCouponSerial;


//////////////////////////////////////////////////////////////////////////
#if defined(SERV_COUNTRY_CN)
	switch( kPacket_.m_iCouponType )
	{
	case KEGS_APPLY_COUPON_REQ::CT_ITEM_CARD:
		{
			kPacketReq.m_iCouponType = KEGS_APPLY_COUPON_REQ::CT_ITEM_CARD;
			SendToLoginServer( ELG_USE_COUPON_REQ, kPacketReq );
			return;
		}
		break;
		//case KEGS_APPLY_COUPON_REQ::CT_ZONE_CARD:
		//	break;
	default:
		{
			START_LOG(cerr, L"잘못된 카드 타입.")
				<< BUILD_LOG( kPacket_.m_iCouponType )
				<< BUILD_LOG( kPacket_.m_wstrCouponSerial )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GIANT_BILLING_03;
			SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
			return;
		}
		break;
	}
#else //SERV_COUNTRY_CN
	// 사내 버전 테스트를 위해서 //
	DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
	if (dwBillingFlag == KSimLayer::BF_NONE || dwBillingFlag == KSimLayer::BF_INTERNAL)
	{
		SendToKOGBillingDB( EBILL_USE_COUPON_REQ, kPacketReq );
		return;
	}
		
	if ( KSimLayer::GetKObj()->IsCheckCouponByPublisher() )
	{
		// 쿠폰 체크 퍼블리셔 하고나서 아이템 처리할 때 (실제 시리얼 코드 매칭을 할 수 없을 때)

		// 이 경우 KOG빌링에 쿠폰사용 예약 -> 퍼블 쿠폰 인증 -> KOG빌링 쿠폰 아이템 처리 순으로 됨
		SendToKOGBillingDB( EBILL_USE_COUPON_RESERVE_REQ, kPacketReq );
	}
	else
	{
		// 퍼블 체크가 아니기에 바로 사용 요청
		SendToKOGBillingDB( EBILL_USE_COUPON_REQ, kPacketReq );
	}
#endif //SERV_COUNTRY_CN
//////////////////////////////////////////////////////////////////////////

}

IMPL_ON_FUNC( EBILL_USE_COUPON_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"should be NET_OK" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		KEGS_APPLY_COUPON_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
		return;
	}

	// 아래 DirectPickup 수정 시 EBILL_USE_COUPON_RESULT_ACK에도 수정해줘야 함

	std::vector< KBillCouponItemInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillCouponItemInfo.begin(); vit != kPacket_.m_vecBillCouponItemInfo.end(); vit++ )
	{
		KBillCouponItemInfo& refCouponItemInfo = *vit;

		int iProductNo = refCouponItemInfo.m_iProductNO;
		KBillProductInfo kInfo;
		if( !SiKBillManager()->GetProductInfo( iProductNo, kInfo ) )
		{
			START_LOG( cerr, L"coupon error" )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			continue;
		}

		int iItemID = kInfo.m_iProductID;
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"아이템 ID 변환 실패." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			continue;
		}

		//{{ 바로 PickUp 해야 하는 아이템들 PickUp 요청함
		if ( IsDirectPickUpItem(iItemID) )
		{
			KEBILL_PICK_UP_REQ kPacketReq;
			kPacketReq.m_iTransNo = refCouponItemInfo.m_iTranNo;
			kPacketReq.m_iProductNo = refCouponItemInfo.m_iProductNO;
			kPacketReq.m_iUnitUID = GetCharUID();
#ifdef SERV_GLOBAL_CASH_PACKAGE
			// 구조상의 이유로 쿠폰의 Direct Pickup은 패키지 상품은 되지 않는다.
			// 유저가 PickUp 해야함
			kPacketReq.m_byteProductKind = 0;
#endif //SERV_GLOBAL_CASH_PACKAGE
			SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacketReq );
		}
		//}}
	}


	KEGS_APPLY_COUPON_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
}

IMPL_ON_FUNC( EBILL_USE_COUPON_RESERVE_ACK )
{
#ifdef SERV_COUNTRY_CN
	// 중국은 여기 들어오면 안됨 에러 상황임
	START_LOG( cerr, L"중국은 여기 코드 타면 안됨!!! LoginServer 쪽거 타야함." )
		<< BUILD_LOGc( kPacket_.m_iOK )
		<< END_LOG;
	kPacket_.m_iOK = NetError::ERR_UNKNOWN;
#endif //SERV_COUNTRY_CN

	//{{ megagame / 2011.08.04 / 주문 예약 실패시에 절대 사용 요청하면 안됨. 주의!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_APPLY_COUPON_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
		return;
	}
	//}}

#if defined(SERV_COUNTRY_TWHK)
	// 대만, 홍콩 빌링 처리가 같고 대만 빌링 구조가 특이해서 이렇게 따로 분리
	SendToPublisherBillingDB( EBILL_USE_COUPON_REQ, kPacket_.m_kPacketReq );
#else //SERV_COUNTRY_XX
	// 해당 코드는 퍼블리셔 쿠폰 체크인 국가만 타야함
	if ( KSimLayer::GetKObj()->IsCheckCouponByPublisher() )
	{
		DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();
		if(dwPublisherBillingConnectType == KSimLayer::PBCT_NORMAL)
		{
			SendToPublisherBilling( EBILL_USE_COUPON_REQ, kPacket_.m_kPacketReq );
		}
		else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
		{
			SendToPublisherBillingDB( EBILL_USE_COUPON_REQ, kPacket_.m_kPacketReq );
		}
		else
		{
			START_LOG( cerr, L"현재 쿠폰 지원에 문제가 있음" )
				<< BUILD_LOG( dwPublisherBillingConnectType )
				<< END_LOG;
			
			KEGS_APPLY_COUPON_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_17;
			SendPacket( EGS_APPLY_COUPON_ACK, kPacketAck );
			return;
		}

	}
	else
	{
		START_LOG( cerr, L"퍼블 쿠폰 인증 아닌데 왜 여기로 들어오지???" )
			<< BUILD_LOG( kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kPacketReq.m_wstrSerialCode )
			<< END_LOG;
	}
#endif //defined(SERV_COUNTRY_TWHK)

}


IMPL_ON_FUNC( EBILL_USE_COUPON_RESULT_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"쿠폰 사용 실패." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		KEGS_APPLY_COUPON_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
		return;
	}


	// 아래 DirectPickup 수정 시 EBILL_USE_COUPON_RESULT_ACK에도 수정해줘야 함

	std::vector< KBillCouponItemInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillCouponItemInfo.begin(); vit != kPacket_.m_vecBillCouponItemInfo.end(); vit++ )
	{
		KBillCouponItemInfo& refCouponItemInfo = *vit;

		int iProductNo = refCouponItemInfo.m_iProductNO;
		KBillProductInfo kInfo;
		if( !SiKBillManager()->GetProductInfo( iProductNo, kInfo ) )
		{
			START_LOG( cerr, L"coupon error" )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			continue;
		}

		int iItemID = kInfo.m_iProductID;
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"아이템 ID 변환 실패." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			continue;
		}

		//{{ 바로 PickUp 해야 하는 아이템들 PickUp 요청함
		if ( IsDirectPickUpItem(iItemID) )
		{
			KEBILL_PICK_UP_REQ kPacketReq;
			kPacketReq.m_iTransNo = refCouponItemInfo.m_iTranNo;
			kPacketReq.m_iProductNo = refCouponItemInfo.m_iProductNO;
			kPacketReq.m_iUnitUID = GetCharUID();
#ifdef SERV_GLOBAL_CASH_PACKAGE
			// 구조상의 이유로 쿠폰의 Direct Pickup은 패키지 상품은 되지 않는다.
			// 유저가 PickUp 해야함
			kPacketReq.m_byteProductKind = 0;
#endif //SERV_GLOBAL_CASH_PACKAGE
			SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacketReq );
		}
		//}}
	}


	KEGS_APPLY_COUPON_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_APPLY_COUPON_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_GET_PURCHASED_CASH_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_PURCHASED_CASH_ITEM_ACK );

	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"캐쉬샵 점검중 쿠폰 사용 요청" )
			<< BUILD_LOG( GetName() );

		KEGS_APPLY_COUPON_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ kPacketDummy;
	int iRet = OnGetPurchasedCashItemReq( kPacket_, kPacketDummy );

	if( iRet != NetError::NET_OK )
	{
		KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = iRet;

		SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
	}
}


IMPL_ON_FUNC( EGS_BUY_CASH_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BUY_CASH_ITEM_ACK );

	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"캐쉬샵 점검중 상품 구매 요청" )
			<< BUILD_LOG( GetName() );

		KEGS_BUY_CASH_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_BUY_CASH_ITEM_ACK kPacket;

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 부활석 한도를 넘지 못하게 한다.
	// 인벤토리 슬롯 증가면 슬롯 최대 한계를 못넘게 한다.	
	int iStoneToBuy = 0;
	int iInventorySlotToExpand = 1;
	std::map< int, int > mapInventorySlotToExpand;
	int iBankSlotToExpand = 0;
	int iCharSlotExpand = 0;
	int iExpandSkillSlot = 0;
	int iTotalLimitBuyCashItemCount = 0;

	//#ifdef SERV_VIP_SYSTEM
	//	int iVIPItemCount = 0;
	//#endif //SERV_VIP_SYSTEM

	std::vector< KNXBTPurchaseReqInfo >::iterator vit;
	for( vit = kPacket_.m_vecPurchaseReqInfo.begin(); vit != kPacket_.m_vecPurchaseReqInfo.end(); ++vit )
	{
		KBillProductInfo kBillProductInfo;
		int iItemID = 0;

		// {{ 아이템을 살 수 있는지 조건 검사
		if( !SiKBillManager()->GetProductInfo( ( int )vit->m_ulProductNo, kBillProductInfo ) )
		{
			START_LOG( cerr, L"ProductInfo error." )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//구입하려는 상품 정보가 이상합니다.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}

		if( kBillProductInfo.m_bSale == false )
		{
			START_LOG( cerr, L"not sale" )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			return;
		}

		//{{ 2010. 12. 08  최육사	특정 상품 구매 갯수 제한
#ifdef SERV_BUY_CASH_ITEM_LIMIT
		if( SiCXSLCashItemManager()->IsBuyCashItemLimitList( vit->m_ulProductNo ) == true )
		{
			const int iLimitBuyCashItemCount = m_kUserCashInventory.GetLimitBuyCashItemCount() + iTotalLimitBuyCashItemCount;
			if( iLimitBuyCashItemCount >= SiCXSLCashItemManager()->GetBuyCashItemLimitMaxCount() )
			{
				START_LOG( clog, L"특정 상품 구매 갯수 제한에 걸려서 상품 구매 못해욤!" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_ulProductNo )
					<< BUILD_LOG( m_kUserCashInventory.GetLimitBuyCashItemCount() )
					<< BUILD_LOG( SiCXSLCashItemManager()->GetBuyCashItemLimitMaxCount() );

				// 더이상 해당 상품을 구매할 수 없다!
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_55;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			++iTotalLimitBuyCashItemCount;
		}
#endif SERV_BUY_CASH_ITEM_LIMIT
		//}}

#ifdef SERV_EXCEPT_NO_SELL
		if( kBillProductInfo.m_cCategoryNo == CXSLCashItemManager::CSDC_COUPON 
			|| kBillProductInfo.m_cCategoryNo == CXSLCashItemManager::CSDC_NO_SELL_IN_PACKAGE )
		{
			START_LOG( cerr, L"쿠폰과 패키지 중 판매 안하는 아이템" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_60;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif //SERV_EXCEPT_NO_SELL

#ifdef SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM
		if( ( kBillProductInfo.m_iProductID == 60007354 || kBillProductInfo.m_iProductID == 60007355 || kBillProductInfo.m_iProductID == 60007356) 
			&& CXSLUnit::UC_ARA_LITTLE_HSIEN != GetUnitClass() )
		{
			// 현재 상품을 구매 할 수 없는 아이템이다!
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif SERV_BUY_ONLY_ARA_LITTLE_HSIEN_ITEM

		//{{ 허상형 : [2012/12/5] //		특정 아이템 가지고 있어야 캐시샵에서 구매 가능
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		if( SiCXSLItemManager()->IsKeepItemShowItem( kBillProductInfo.m_iProductID ) == true 
			&& m_kInventory.CheckKeepItem( kBillProductInfo.m_iProductID) == false )
		{			
			// 현재 상품을 구매 할 수 없는 아이템이다!
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
		//}} 허상형 : [2012/12/5] //		특정 아이템 가지고 있어야 캐시샵에서 구매 가능s

		iItemID = kBillProductInfo.m_iProductID;

		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"아이템 ID 변환 실패." )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//구입하려는 상품 정보가 이상합니다.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}


		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//구입하려는 상품 정보가 이상합니다.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
		// }} 아이템을 살 수 있는지 조건 검사

		switch(	pItemTemplet->m_ItemID )
		{
#ifdef SERV_EVENT_BUY_FAKE_ITEM
		case CXSLItem::CI_BUY_FAKE_ITEM:
			{
				unsigned long uiProductNo = vit->m_ulProductNo;
				kPacket_.m_vecPurchaseReqInfo.erase( vit-- );

				// 남은 구매 횟수 확인
				KEBILL_CHECK_BUY_FAKE_ITEM_REQ kPacketReq;
				kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
#ifdef SERV_EVENT_BUY_FAKE_ITEM_UNIT
				kPacketReq.m_iUserUID = GetCharUID();
#else //SERV_EVENT_BUY_FAKE_ITEM_UNIT
				kPacketReq.m_iUserUID = GetUID();
#endif //SERV_EVENT_BUY_FAKE_ITEM_UNIT
				kPacketReq.m_ulProductNo = uiProductNo;
				SendToKOGBillingDB( EBILL_CHECK_BUY_FAKE_ITEM_REQ, kPacketReq );

				START_LOG( clog, L"김석근_이제패킷을보낼때" )
					<< BUILD_LOG( pItemTemplet->m_ItemID )
					<< BUILD_LOG( kPacketReq.m_ulProductNo)
					<< BUILD_LOG( kPacketReq.m_iUserUID)
					<< BUILD_LOG( kPacketReq.m_iServerGroupID)
					<< END_LOG;
			} continue;
#endif //SERV_EVENT_BUY_FAKE_ITEM



	case CXSLItem::CI_RESURRECTION_STONE: // 부활석
		iStoneToBuy += ( int )kBillProductInfo.m_cQuantity * ( int )vit->m_usOrderQuantity;
		break;
	case CXSLItem::CI_EXPAND_INVENTORY: // 소지품 확장
		{
			START_LOG( cerr, L"This item is invalid" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_EQUIP: // 소지품 확장( 장비 )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_EQUIP, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.

			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_EQUIP, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_ACCESSORY: // 소지품 확장( 악세사리 )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_ACCESSORY, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_ACCESSORY, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_QUICK_SLOT: // 소지품 확장( 소비 )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUICK_SLOT, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_QUICK_SLOT, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_MATERIAL: // 소지품 확장( 재료 )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_MATERIAL, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_MATERIAL, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_QUEST: // 소지품 확장( 퀘스트 )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUEST, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_QUEST, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_INVENTORY_SPECIAL: // 소지품 확장( 특수 )
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_SPECIAL, iInventorySlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				break;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
			std::pair< std::map< int, int >::iterator, bool > pairResult;
			pairResult = mapInventorySlotToExpand.insert( std::make_pair( CXSLInventory::ST_SPECIAL, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
			if( !pairResult.second )
			{
				( pairResult.first )->second += CXSLInventory::SLOT_COUNT_ONE_LINE;
			}
		}
		break;
	case CXSLItem::CI_EXPAND_CHAR_SLOT_1:	// 케릭터 슬롯 확장
		iCharSlotExpand += 1;
		break;
	case CXSLItem::CI_EXPAND_CHAR_SLOT_2:	// 케릭터 슬롯 확장
		iCharSlotExpand += 2;
		break;
	case CXSLItem::CI_EXPAND_SKILL_SLOT:	// 스킬 슬롯 확장
	case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
		iExpandSkillSlot += 1;
		break;

		// 은행 확장
	case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
	case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
	case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
	case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
	case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
		{
			// 내가 이걸 구매 할수 있는 등급인지 본다!
			if( m_kInventory.GetMyNextUpgradeBankItemID() != pItemTemplet->m_ItemID )
			{
				START_LOG( cerr, L"구매 가능한 등급이 아닙니다." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( m_kInventory.GetMyNextUpgradeBankItemID() )
					<< BUILD_LOG( pItemTemplet->m_ItemID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_38;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			//////////////////////////////////////////////////////////////////////////
			if( CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM == iItemID )
			{
				// 인벤토리에 루리엘 신용보증서가 있으면 마지막 회원권은 구매할 수 없다!
				if( m_kInventory.IsExistWithBank( CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE ) == true )
				{
					START_LOG( cerr, L"루리엘 신용보증서가 인벤토리에 있으면 마지막 회원권은 구매못함." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetCharName() )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_39;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}
			}
			//////////////////////////////////////////////////////////////////////////

			// 주문수량이 한개 이상은 안되!
			if( vit->m_usOrderQuantity > 1 )
			{
				START_LOG( cerr, L"은행 회원권은 1개이상 주문할 수 없다." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_usOrderQuantity )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_37;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// 확장가능한지 검사하자!
			iBankSlotToExpand += CXSLInventory::SLOT_COUNT_ONE_LINE;
		}
		break;

	case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
		{
			// 일반 멤버쉽이 아니면 구매할 수 없다!
			if( m_kInventory.GetBankMemberShip() != CXSLInventory::MPB_NORMAL )
			{
				START_LOG( cerr, L"구매 가능한 등급이 아닙니다." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( m_kInventory.GetBankMemberShip() )						
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_38;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// 인벤토리에 루리엘 신용보증서가 있으면 마지막 회원권은 구매할 수 없다!
			if( m_kInventory.IsExistWithBank( CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE ) == true )
			{
				START_LOG( cerr, L"루리엘 신용보증서가 인벤토리에 있으면 마지막 회원권은 구매못함." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_39;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// 주문수량이 한개 이상은 안되!
			if( vit->m_usOrderQuantity > 1 )
			{
				START_LOG( cerr, L"은행 회원권은 1개이상 주문할 수 없다." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_usOrderQuantity )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_37;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// 확장가능한지 검사하자!
			iBankSlotToExpand += CXSLInventory::SLOT_COUNT_ONE_LINE * 5; // 8칸씩 5줄
		}
		break;

		//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	case CXSLItem::CI_CASH_SKILL_NOTE_ITEM:
		{
			// 현재 기술의 노트를 보유하고 있다면?
			if( m_kSkillTree.IsHaveSkillNote() == true )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SKILL_NOTE_06;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			// 캐릭터 레벨이 20레벨 미만이면 구입 불가!
			if( GetLevel() < 20 )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SKILL_NOTE_07;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_SKILL_NOTE
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	case CXSLItem::CI_UNLIMITED_CHANGE_SECOND_JOB:
		{
			// 전직 변경이 가능한 상태인가? - Normal 직업은 구입 할 수 없다.
			if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			CXSLUnit::UNIT_CLASS cCurrentUnitClass = static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass());

			if( cCurrentUnitClass == CXSLUnit::UC_NONE || CXSLUnit::IsSecondChangeJob( cCurrentUnitClass ) == false )	// 클래스 정보가 없거나 2차 전직이 아니면
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			//특수탭이 다 차있으면
			std::map< int, int > mapItem; // 인벤토리 검사용
			// 5. 여유공간 검사를 위한 준비
			mapItem.insert( std::make_pair( pItemTemplet->m_ItemID, 1 ) );	

			// 6. 인벤토리의 여유공간 검사
			if( !m_kInventory.IsEnoughSpaceExist( mapItem ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_02;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
			// 7. 아이템이 존재하면 못사게 한다. 
			if( m_kInventory.IsExistWithBank( pItemTemplet->m_ItemID ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_CLASS_CHANGE_EVENT_01;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_UNLIMITED_SKILL_RESET_ITEM
	case CXSLItem::CI_UNLIMITED_SKILL_RESET_ITEM:
		{
			//특수탭이 다 차있으면
			std::map< int, int > mapItem; // 인벤토리 검사용
			// 5. 여유공간 검사를 위한 준비
			mapItem.insert( std::make_pair( pItemTemplet->m_ItemID, 1 ) );	

			// 6. 인벤토리의 여유공간 검사
			if( !m_kInventory.IsEnoughSpaceExist( mapItem ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_NX_SHOP_02;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
			// 7. 아이템이 존재하면 못사게 한다. 
			if( m_kInventory.IsExistWithBank( pItemTemplet->m_ItemID ) )
			{
				KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_CLASS_CHANGE_EVENT_01;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		} break;
#endif SERV_UNLIMITED_SKILL_RESET_ITEM
		//}}
		//#ifdef SERV_VIP_SYSTEM
		//		case 202970: // VIP 패키지 아이템 구매 제한
		//			{
		//				if(m_kInventory.IsExistWithBank( 202970 ))
		//				{
		//					KEGS_BUY_CASH_ITEM_ACK kPacket;
		//					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_10;
		//					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		//					
		//					iVIPItemCount += 1;
		//
		//					break;
		//				}
		//			}
		//			break;
		//#endif //SERV_VIP_SYSTEM
		//{{ 2011. 12. 13  김민성	퀵 슬롯 확장 아이템
#ifdef SERV_EXPAND_QUICK_SLOT
	case CXSLItem::CI_EXPAND_QUICK_SLOT:
		{
			// 하나라도 슬롯이 남아있으면 구입 가능하도록 한다.
			int iQuickSlotToExpand = 1; 
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_E_QUICK_SLOT, iQuickSlotToExpand ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_EXPAND_QUICK_SLOT
		//}}
#ifdef SERV_SKILL_PAGE_SYSTEM
	case CXSLItem::CI_EXPAND_SKILL_PAGE:
		{
			KNXBTProductInfo kProductInfo;
			if( !SiKNexonBillingTCPManager()->GetProductInfo( vit->m_ulProductNo, kProductInfo ) )
			{
				START_LOG( cerr, L"프로덕트 정보 추출 실패." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( vit->m_ulProductNo )
					<< END_LOG;

				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}

			switch ( GetStateID() )
			{
			case KGSFSM::S_FIELD_MAP:
				break;

			default:
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_SKILL_PAGE_08;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				} break;
			}

			/// 구매한 확장권 개수가 최대로 확장할 수 있는 수보다 크면 구입 실패
			const int iTheNumberOfSkillPagesToBeAdded
				= kProductInfo.m_usProductPieces * vit->m_usOrderQuantity;

			if ( !m_kSkillTree.CanExpandSkillPage( iTheNumberOfSkillPagesToBeAdded ) )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;

				const int iTheNumberOfSkillPagesRemainedToLimit
					= KUserSkillTree::MAX_COUNT_OF_PAGES_AVAILABLE - static_cast<int>( m_kSkillTree.GetTheNumberOfSkillPagesAvailable() );

				// 2개권을 사려고 하는데 1페이지만 확장 가능 할 때
				if ( iTheNumberOfSkillPagesToBeAdded == 2 &&
					iTheNumberOfSkillPagesRemainedToLimit == 1 )
					kPacket.m_iOK = NetError::ERR_SKILL_PAGE_09;
				else
					kPacket.m_iOK = NetError::ERR_SKILL_PAGE_02;

				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			}
		}break;
#endif // SERV_SKILL_PAGE_SYSTEM
	default:
		{
			// 전직 캐쉬 아이템
			if( CXSLItem::IsJobChangeCashItem( iItemID ) )
			{
				// 1. 전직 가능한 레벨 검사
				if( GetLevel() < pItemTemplet->m_UseLevel )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = ( pItemTemplet->m_UseLevel <= 15 ? NetError::ERR_BUY_CASH_ITEM_40 : NetError::ERR_BUY_CASH_ITEM_41 );
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// 2. 전직 가능한 유닛 타입검사
				const char cChangeJobUnitClass = CXSLItem::GetCashItemChangeUnitClass( iItemID );
				if( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(cChangeJobUnitClass) ) != GetUnitType() )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// 3. 전직 가능한 유닛 클래스 검사
				if( CXSLUnit::GetUnitClassDownGrade( static_cast<CXSLUnit::UNIT_CLASS>(cChangeJobUnitClass) ) != GetUnitClass() )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}


				//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
				// 전직 변경 아이템 ClassChange
				if( CXSLItem::IsClassChangeCashItem( iItemID ) == true )
				{
					// 전직 변경이 가능한 상태인가? - Normal 직업은 구입 할 수 없다.
					if( CXSLUnit::IsInitNormalJob( static_cast<CXSLUnit::UNIT_CLASS>(GetUnitClass()) ) == true )
					{
						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}

					// 동일한 UnitClass Group 인지 확인 하자
					int iNewUnitClass = CXSLItem::GetClassChangeCashItem( iItemID );
					int iUnitClass = static_cast<int>(GetUnitClass());
					if( SiKClassChangeTable()->CompareUnitClass( iUnitClass, iNewUnitClass ) == false )
					{
						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}
				}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
				//}}
				break;
			}

			//{{ 2009. 8. 4  최육사		스킬 봉인 해제 캐쉬템
#ifdef	SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
			// 사용 가능한 전직인가?
			if( SiCXSLSkillTree()->IsUnsealItemID( iItemID ) == true )
			{
				if( isCanUseUnsealCashSkillItem( iItemID ) == false )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				std::vector<int> vecSkillID;
				if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, GetUnitClass(), vecSkillID ) == true )
				{
					BOOST_TEST_FOREACH( int, iSkillID, vecSkillID )
					{
						// 이미 봉인해제된 스킬인가?
						if( m_kSkillTree.IsSkillUnsealed( iSkillID ) )
						{
							KEGS_BUY_CASH_ITEM_ACK kPacket;
							kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_34;
							SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
							return;
						}

						// 혹시나해서 스킬찍었는지도 검사해봄
#ifdef SERV_SKILL_PAGE_SYSTEM
						if ( m_kSkillTree.IsExistOnUsedPage( iSkillID ) )
#else // SERV_SKILL_PAGE_SYSTEM
						if( m_kSkillTree.IsExist( iSkillID ) )
#endif // SERV_SKILL_PAGE_SYSTEM
						{
							START_LOG( cerr, L"봉인해제도 안되었는데 스킬이 찍혔다? 절대 일어나서는 안되는 에러!" )
								<< BUILD_LOG( GetCharUID() )
								<< BUILD_LOG( iSkillID )
								<< END_LOG;

							// 에러로그만 찍자!
						}

						std::vector< int > vecItemList;
						if( SiCXSLSkillTree()->GetUnsealSkillItemList( iSkillID, vecItemList ) == false )
						{
							START_LOG( cerr, L"봉인스킬 아이템 리스트가 없다?" )
								<< BUILD_LOG( GetCharUID() )
								<< BUILD_LOG( iSkillID )
								<< END_LOG;

							KEGS_BUY_CASH_ITEM_ACK kPacket;
							kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
							SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
							return;
						}

						std::vector< int >::const_iterator vit;
						for( vit = vecItemList.begin(); vit != vecItemList.begin(); ++vit )
						{
							if( m_kInventory.IsExistWithBank( iItemID ) == true )
							{
								KEGS_BUY_CASH_ITEM_ACK kPacket;
								kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_35;
								SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
								return;
							}
						}
					}
				}
				break;
			}
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
			/*
			int iSkillID = 0;
			char cUnitClass = 0;
			if( SiCXSLSkillTree()->GetUnsealItemInfo( iItemID, iSkillID, cUnitClass ) == true )
			{
				// 해당 직업이 맞는가?
				if( CXSLSkillTree::CheckUnitClassForUnsealSkill( GetUnitClass(), cUnitClass ) == false )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_33;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// 이미 봉인해제된 스킬인가?
				if( m_kSkillTree.IsSkillUnsealed( iSkillID ) )
				{
					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_34;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				// 혹시나해서 스킬찍었는지도 검사해봄
				if( m_kSkillTree.IsExist( iSkillID ) )
				{
					START_LOG( cerr, L"봉인해제도 안되었는데 스킬이 찍혔다? 절대 일어나서는 안되는 에러!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iSkillID )
						<< END_LOG;

					// 에러로그만 찍자!
				}

				std::vector< int > vecItemList;
				if( SiCXSLSkillTree()->GetUnsealSkillItemList( iSkillID, vecItemList ) == false )
				{
					START_LOG( cerr, L"봉인스킬 아이템 리스트가 없다?" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iSkillID )
						<< END_LOG;

					KEGS_BUY_CASH_ITEM_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
					SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
					return;
				}

				std::vector< int >::const_iterator vit;
				for( vit = vecItemList.begin(); vit != vecItemList.begin(); ++vit )
				{
					if( m_kInventory.IsExistWithBank( iItemID ) == true )
					{
						KEGS_BUY_CASH_ITEM_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_35;
						SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
						return;
					}
				}

				break;
			}
			*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
			//}}
		}
		break;			
		}

#ifdef CASH_SHOP_LIMIT_LEVEL
		if( kBillProductInfo.m_cLimitLevel > GetLevel() )
		{
			START_LOG( clog, L"구매 레벨 제한" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetLevel() )
				<< BUILD_LOG( kBillProductInfo.m_cLimitLevel )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_58;	// 레벨이 부족하여 구매할 수 없습니다.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
#endif CASH_SHOP_LIMIT_LEVEL
	}

#ifdef SERV_EVENT_BUY_FAKE_ITEM
	if( kPacket_.m_vecPurchaseReqInfo.empty() == true )
		return;
#endif //SERV_EVENT_BUY_FAKE_ITEM


	if( iStoneToBuy > 0 )
	{
		if( m_iNumResurrectionStone + iStoneToBuy > 2000 )
		{
			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_18;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
	}

	//#ifdef SERV_VIP_SYSTEM
	//	if(iVIPItemCount > 0)
	//	{
	//		return;
	//
	//	}
	//#endif //SERV_VIP_SYSTEM	

	if(iInventorySlotToExpand <= 0) //2011.07.22 lygan_조성욱 // 카테고리별 인벤토리 확장 버그 수정
	{
		return;
	}

	//{{ 2009. 8. 7  최육사		은행
	if( iBankSlotToExpand > 0 )
	{		
		if( !m_kInventory.IsAbleToExpandBank( iBankSlotToExpand ) )
		{
			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_36;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
	}
	//}}

	//{{ 2008. 12. 14  최육사	캐릭터 슬롯 확장
	if( iCharSlotExpand > 0 )
	{
		if( ( m_nUnitSlot + iCharSlotExpand ) > CXSLUnit::CHAR_SLOT_MAX )
		{
			KEGS_BUY_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_30;
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}
	}
	//}}

	if( iExpandSkillSlot > 0 )
	{
		KUserSkillTree::SKILL_SLOT_B_EXPIRATION_STATE eExpirationState = m_kSkillTree.GetSkillSlotBExpirationState();

		switch( eExpirationState )
		{
		case KUserSkillTree::SSBES_EXPIRED:
		case KUserSkillTree::SSBES_NOT_EXPIRED:
			{
				// 아이템을 살 수 있다

			} break;

		default: 
		case KUserSkillTree::SSBES_PERMANENT:
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
				SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
				return;
			} break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 아이템 구매 처리
	KEBILL_BUY_PRODUCT_REQ kPacketReq;
	kPacketReq.m_wstrTotalOrderID = MakeOrderID();	// TotalOrderID를 먼저 발급

	std::vector< KNXBTPurchaseReqInfo >::const_iterator vitReqInfo;
	for( vitReqInfo = kPacket_.m_vecPurchaseReqInfo.begin(); vitReqInfo != kPacket_.m_vecPurchaseReqInfo.end(); vitReqInfo++ )
	{
		KBillProductInfo kBillProductInfo;
		if( !SiKBillManager()->GetProductInfo( ( int )vitReqInfo->m_ulProductNo, kBillProductInfo ) )
		{
			START_LOG( cerr, L"ProductInfo error." )
				<< BUILD_LOG( vitReqInfo->m_ulProductNo )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;	//구입하려는 상품 정보가 이상합니다.
			SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
			return;
		}

		KBillBuyInfo kBuyInfo;
		kBuyInfo.m_wstrOrderID = MakeOrderID();
		kBuyInfo.m_iProductNo = ( int )vitReqInfo->m_ulProductNo;
		kBuyInfo.m_iPoint = kBillProductInfo.m_iSalePrice;
		kBuyInfo.m_iProductID = kBillProductInfo.m_iProductID;
		kBuyInfo.m_iCategory = ( int )kBillProductInfo.m_cCategoryNo;
		kBuyInfo.m_wstrMemo = kBillProductInfo.m_wstrProductName;
		kBuyInfo.m_usOrderQuantity = kBillProductInfo.m_cQuantity;	// 효진 추가
		kPacketReq.m_vecBillBuyInfo.push_back( kBuyInfo );
	}
	
	kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketReq.m_iChannelingCode = GetChannelCode();

	kPacketReq.m_wstrUserID = GetName();
	kPacketReq.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;

	kPacketReq.m_iUserUID = GetUID();
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_wstrNickName = GetCharName();
	kPacketReq.m_strUserIP = GetIPStr();

	//{{ 허상형 : [2010/7/29/] //	캐시 로그
	kPacketReq.m_BuyCharInfo.m_cLevel = GetLevel();
	kPacketReq.m_BuyCharInfo.m_iUnitClass = GetUnitClass();
	//}} 허상형 : [2010/7/29/] //	캐시 로그



#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(!IsAbleToUseCashType(kPacket_.m_iUseCashType))
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GLOBAL_BILLING_01;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}

	kPacketReq.m_iUseCashType = kPacket_.m_iUseCashType;

	kPacketReq.m_iUserIP = GetIP();

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	//	구매제한 체크
	SendToKOGBillingDB( EBILL_PREPARE_BUY_PRODUCT_REQ, kPacketReq );

	// 찜에서 캐쉬구매
	m_kUserWishList.SetBuyCashItemInWishList( kPacket_.m_bIsWishList );

	// 실시간 부활석
	m_kUserCashInventory.SetCashResurrectionStone( false );
}


//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( EGS_BILL_PRODUCT_INFO_REQ )
{
	KEGS_BILL_PRODUCT_INFO_ACK kPacket;

	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"캐쉬샵 점검중 상품 리스트 요청" )
			<< BUILD_LOG( GetName() );

		kPacket.m_bFinal = true;
		SendPacket( EGS_BILL_PRODUCT_INFO_ACK, kPacket );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	SiKBillManager()->GetBillProductPage( kPacket_.m_iPage, kPacket.m_vecProductInfo, kPacket.m_bFinal );

#ifdef SERV_QUICK_CASH_SHOP
	for( int i = 0; i < kPacket.m_vecProductInfo.size(); ++i )
	{
		kPacket.m_vecProductInfo[i].m_wstrProductName.resize( 0 );
		kPacket.m_vecProductInfo[i].m_wstrEndDate.resize( 0 );

#ifdef SERV_NET_CAFE_CASHSHOP
		if( CXSLItem::IsPcBangOnlyCashItem( kPacket.m_vecProductInfo[i].m_iProductID ) == true 
			&& IsPcBang() == false )
		{			
			kPacket.m_vecProductInfo.erase(&kPacket.m_vecProductInfo[i--]);	
		}
#endif //SERV_NET_CAFE_CASHSHOP
	}
#endif SERV_QUICK_CASH_SHOP

#ifdef SERV_NOTIFY_BONUS_CASH
 	KEGS_BONUS_CASH_EVENT_NOT kPacketNot;
 	kPacketNot.m_bEventStart = SiKBillManager()->GetBonusCashEvent();
 	SendPacket( EGS_BONUS_CASH_EVENT_NOT, kPacketNot );
#endif //SERV_NOTIFY_BONUS_CASH
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	kPacket.m_mapKeepShowItem = SiCXSLItemManager()->m_mapKeepShowItem;
	kPacket.m_DisCountInfoMap = SiCXSLItemManager()->m_DisCountInfoMap;
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#ifdef SERV_WISH_LIST_NO_ITEM
	kPacket.m_setWishListNoItemList = SiCXSLItemManager()->m_setWishListNoItemList;
#endif SERV_WISH_LIST_NO_ITEM
	SendPacket( EGS_BILL_PRODUCT_INFO_ACK, kPacket );

	START_LOG( clog2, L"product info ack" )
		<< BUILD_LOG( kPacket_.m_iPage )
		<< BUILD_LOG( kPacket.m_vecProductInfo.size() )
		<< BUILD_LOG( kPacket.m_bFinal )
// #ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
// 		<< BUILD_LOG( kPacketNot.m_bEventStart )
// #endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		<< END_LOG;
}

IMPL_ON_FUNC( EGS_BILL_INVENTORY_INQUIRY_REQ )
{
	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	if( SiKGSContentManager()->IsEnableCashShop() == false )
	{
		START_LOG( clog, L"캐쉬샵 점검중 캐쉬샵 인벤토리 정보 요청" )
			<< BUILD_LOG( GetName() );

		KEGS_PURCHASED_CASH_ITEM_LIST_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_CONTENT_00;
		SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacketAck );
		return;
	}
#endif SERV_CONTENT_MANAGER
	//}}

	KEGS_BILL_INVENTORY_INQUIRY_ACK kPacket;
	if( kPacket_.m_iCurrentPage < 1 )
	{
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket );
		return;
	}

	if( kPacket_.m_nItemPerPage < 1 )
	{
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket );
		return;
	}

	SendToKOGBillingDB( EBILL_INVENTORY_INQUIRY_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EBILL_INVENTORY_INQUIRY_ACK, KEGS_BILL_INVENTORY_INQUIRY_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
		std::vector< UidType > vecNeedNickName;
		//}} 허상형 : [2010/8/19/] //	선물 기능 개편

		std::vector< KBillOrderInfo >::iterator vit;
		for( vit = kPacket_.m_vecOrderInfo.begin(); vit != kPacket_.m_vecOrderInfo.end(); vit++ )
		{
			int iProductNo = vit->m_kBillProductInfo.m_iProductNo;
			if( !SiKBillManager()->GetProductInfo( iProductNo, vit->m_kBillProductInfo ) )
			{
				START_LOG( cerr, L"product info error" )
					<< BUILD_LOG( iProductNo )
					<< END_LOG;

				vit->m_kBillProductInfo.m_iProductNo = 0;
			}

			KBillProductAttribute kAttribute;
			if( !SiKBillManager()->GetProductAttribute( vit->m_kBillProductInfo.m_iProductNo, kAttribute ) )
			{
				vit->m_vecSocketOption.clear();
			}
			else
			{
				vit->m_vecSocketOption = kAttribute.m_vecAttribute;
			}

			//////////////////////////////////////////////////////////////////////////
			//{{ 허상형 : [2010/8/19/] //	선물 기능 개편

			if( vit->m_iFromUnitUID != 0 ) //	선물 일 경우
			{
				vit->m_byteType = KBillOrderInfo::BCIT_GIFT;
				vecNeedNickName.push_back( vit->m_iFromUnitUID );
			}
		}

#ifdef SERV_GLOBAL_CASH_PACKAGE

		for( int i = 0; i < kPacket_.m_vecSubPackageInfo.size(); i++)
		{
			
			KBillPackageInfo tempSubPackageInfo;
			KBillProductInfo tempSubPackProInfo;
			KBillProductAttribute tempAttribute;
			CTime tExpirationTime;

			SiKBillManager()->GetProductInfo(kPacket_.m_vecSubPackageInfo[i], tempSubPackProInfo);

			SiKBillManager()->GetProductAttribute(kPacket_.m_vecSubPackageInfo[i], tempAttribute);

			tempSubPackageInfo.m_iPackageNo	= kPacket_.m_vecSubPackageTrans[i];
			tempSubPackageInfo.m_ulProductNo = tempSubPackProInfo.m_iProductNo;
			tempSubPackageInfo.m_usProductPieces = tempSubPackProInfo.m_cQuantity;
			tempSubPackageInfo.m_vecSocketOption = tempAttribute.m_vecAttribute;
			tempSubPackageInfo.m_wstrProductID = tempSubPackProInfo.m_wstrProductName;
			tempSubPackageInfo.m_iProductID = tempSubPackProInfo.m_iProductID;
			tempSubPackageInfo.m_sProductExpire = tempSubPackProInfo.m_cPeriod;

			kPacket_.m_vecPackageInfo.push_back(tempSubPackageInfo);
		}

#endif //SERV_GLOBAL_CASH_PACKAGE

			//	닉네임 얻어오고	빠져나감
		if( !vecNeedNickName.empty() )
		{
			KDBE_GET_NICKNAME_BY_UNITUID_REQ kPacketReq;
			kPacketReq.m_vecBillOrderInfo = kPacket_.m_vecOrderInfo;
			kPacketReq.m_vecUID = vecNeedNickName;
#ifdef SERV_GLOBAL_CASH_PACKAGE
			kPacketReq.m_vecPackageInfo = kPacket_.m_vecPackageInfo;
			kPacketReq.m_vecSubPackageInfo = kPacket_.m_vecSubPackageInfo;
			kPacketReq.m_vecSubPackageTrans = kPacket_.m_vecSubPackageTrans;
#endif //SERV_GLOBAL_CASH_PACKAGE

			SendToGameDB( DBE_GET_NICKNAME_BY_UNITUID_REQ, kPacketReq );
			return;
		}
		//}} 허상형 : [2010/8/19/] //	선물 기능 개편
		//////////////////////////////////////////////////////////////////////////

	}

	SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket_ );
}


//{{ 2009-11-18  최육사  구매전 캐쉬인벤체크
IMPL_ON_FUNC( EBILL_PREPARE_BUY_PRODUCT_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;

		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}

	DWORD dwBillingFlag = GetKGSSimLayer()->GetBillingFlag();
	if (dwBillingFlag == KSimLayer::BF_NONE)
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;

		kPacket.m_iOK = NetError::ERR_GASH_10;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
		return;
	}
	
	DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
	DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();
	
#ifdef SERV_COUNTRY_CN
	// 중국은 캐시 차감을 우리가 처리함
	if ( true )
#else SERV_COUNTRY_CN
	if (dwBillingFlag == KSimLayer::BF_INTERNAL)
#endif SERV_COUNTRY_CN
	{
		// 사내일 떄는 바로 아이템 넣을 수 있게 함
		SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
	{	
		// 구분해서 DB로 보낼지 TCP보낼 지 결정해야 함

		// 일반 캐시는 TCP 로 차감
		if (kPacket_.m_kEBILL_BUY_PRODUCT_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_CASH)
			SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
		// 엘소드 전환 캐시는 DB로 차감
		else if (kPacket_.m_kEBILL_BUY_PRODUCT_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH)
			SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
		else
		{
			START_LOG( cerr, L"구입 UseCashType에 문제가 있습니다. 정상값 아님" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_kEBILL_BUY_PRODUCT_REQ.m_iUseCashType )
				<< END_LOG;
		}

	}
	else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
	{
		// 퍼블리셔 캐시 처리가 DB 연동 시스템일 때 사용
		SendToPublisherBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_PH)
	{
		SendToKOGBillingDB( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}
	else
	{
		// 퍼블리셔 캐시 처리가 TCP 연동(SOAP포함) 시스템일 떄 사용
		SendToPublisherBilling( EBILL_BUY_PRODUCT_REQ, kPacket_.m_kEBILL_BUY_PRODUCT_REQ );
	}

}
//}}

IMPL_ON_FUNC( EBILL_BUY_PRODUCT_ACK )
{
	std::vector< KBillOrderInfo >::iterator vit;
	for( vit = kPacket_.m_vecBillOrderInfo.begin(); vit != kPacket_.m_vecBillOrderInfo.end(); vit++ )
	{
		int iProductNo = vit->m_kBillProductInfo.m_iProductNo;
		if( !SiKBillManager()->GetProductInfo( iProductNo, vit->m_kBillProductInfo ) )
		{
			START_LOG( cerr, L"error" )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( vit->m_wstrOrderID )
				<< END_LOG;

			continue;
		}		

		//{{ 2010. 12. 08  최육사	특정 상품 구매 갯수 제한
#ifdef SERV_BUY_CASH_ITEM_LIMIT
		if( SiCXSLCashItemManager()->IsBuyCashItemLimitList( iProductNo ) == true )
		{
			m_kUserCashInventory.IncreaseBuyCashItemCount();

			START_LOG( clog, L"특정 상품 구매 성공하여 카운트 올린다!" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( iProductNo )
				<< BUILD_LOG( m_kUserCashInventory.GetLimitBuyCashItemCount() );
		}
#endif SERV_BUY_CASH_ITEM_LIMIT
		//}}

		int iItemID = vit->m_kBillProductInfo.m_iProductID;
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"아이템 ID 변환 실패." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			continue;
		}

		//{{ 바로 PickUp 해야 하는 아이템들 PickUp 요청함
		if ( IsDirectPickUpItem(iItemID) )
		{
			KEBILL_PICK_UP_REQ kPacketReq;
			kPacketReq.m_iTransNo = vit->m_iTransNo;
			kPacketReq.m_iProductNo = vit->m_kBillProductInfo.m_iProductNo;
			kPacketReq.m_iUnitUID = GetCharUID();
#ifdef SERV_GLOBAL_CASH_PACKAGE
			kPacketReq.m_byteProductKind = vit->m_byteProductKind;
#endif //SERV_GLOBAL_CASH_PACKAGE
			SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacketReq );
		}
		//}}
	}

	KEGS_BUY_CASH_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	for( vit = kPacket_.m_vecBillOrderInfo.begin(); vit != kPacket_.m_vecBillOrderInfo.end(); vit++ )
	{
		KNXBTPurchaseReqInfo kInfo;
		kInfo.m_ulProductNo = ( unsigned long )vit->m_kBillProductInfo.m_iProductNo;
		kInfo.m_usOrderQuantity = 1;
		kPacket.m_vecPurchaseReqInfo.push_back( kInfo );
	}
	kPacket.m_bIsWishList = m_kUserWishList.GetBuyCashItemInWishList();
	SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );

	// 실시간 부활석
	if( m_kUserCashInventory.IsCashResurrectionStone() == true  &&  kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;		
		SendPacket( EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK, kPacketAck );
	}

	//{{ 2011. 02. 08	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG
	BOOST_TEST_FOREACH( const KBillOrderInfo&, kReq, kPacket_.m_vecBillOrderInfo )
	{
		KBillProductInfo kProductInfo;
		if( SiKBillManager()->GetProductInfo( kReq.m_kBillProductInfo.m_iProductNo, kProductInfo ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 상품을 구매하였습니다!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kReq.m_kBillProductInfo.m_iProductNo )
				<< END_LOG;
			continue;
		}

		// 캐쉬템 구매 카운트 체크
#ifdef SERV_ACCOUNT_COUNT		
		m_kUserStatistics.IncreaseAccountCount( KUserStatistics::AGCT_BUY_CASH_ITEM );
#endif
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM );
		// 해당 캐릭터가 최초로 캐쉬템 구매한것이라면?
		if( m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM ) == 1 )
		{
			WriteCharacterLogToDBWithProductNo( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_BUY_CASH_ITEM,kReq.m_kBillProductInfo.m_iProductNo );
		}
#endif SERV_CHAR_LOG
		//}}

		// 캐쉬템 구매 로그 남기기
		CTime tCurTime = CTime::GetCurrentTime();
		KELOG_BUY_CASH_ITEM_LOG_NOT kPacketToLog;
		kPacketToLog.m_iUserUID			= GetUID();
		kPacketToLog.m_iServerGroupID	= KBaseServer::GetKObj()->GetServerGroupID();
		kPacketToLog.m_iUnitUID			= GetCharUID();
		kPacketToLog.m_cUnitClass		= GetUnitClass();
		kPacketToLog.m_ucLevel			= GetLevel();
#ifdef SERV_ACCOUNT_COUNT
		kPacketToLog.m_bIsFirstBuy		= m_kUserStatistics.IsFirstBuy();
#else
		kPacketToLog.m_bIsFirstBuy		= false;
#endif
		kPacketToLog.m_cBuyType			= KELOG_BUY_CASH_ITEM_LOG_NOT::CIBT_BUY;
		kPacketToLog.m_iProductNo		= kProductInfo.m_iProductNo;
		kPacketToLog.m_iBuyQuantity		= ( int )kReq.m_kBillProductInfo.m_cQuantity;
		//kPacketToLog.m_iTotalSalePrice	= kReq.m_kBillProductInfo.m_cQuantity * kProductInfo.m_iRealPrice;
		kPacketToLog.m_iTotalSalePrice	= kProductInfo.m_iRealPrice;
		kPacketToLog.m_wstrRegDate		= ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		SendToLogDB( ELOG_BUY_CASH_ITEM_LOG_NOT, kPacketToLog );
	}
#endif SERV_BUY_CASH_ITEM_LOG
	//}}
}

IMPL_ON_FUNC( EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ )
{
	KEGS_GET_PURCHASED_CASH_ITEM_REQ kPacketDummy;
	int iRet = OnGetPurchasedCashItemReq( kPacketDummy, kPacket_ );

	if( iRet != NetError::NET_OK )
	{
		KEGS_BILL_GET_PURCHASED_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = iRet;

		SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
	}
}

IMPL_ON_FUNC( EBILL_PICK_UP_ACK )
{
	KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK kPacketDummy;
	KEGS_BILL_GET_PURCHASED_CASH_ITEM_ACK kPacket;
	kPacket.m_iOK = OnPickUpAck( kPacketDummy, kPacket_, kPacket.m_vecInventorySlotInfo );
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		kPacket.m_iNumResurrectionStone = m_iNumResurrectionStone;
	}

	if( kPacket.m_iOK == NetError::ERR_NX_SHOP_09 )
	{
		return;
	}

	SendPacket( EGS_BILL_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
}


IMPL_ON_FUNC( EBILL_GIFT_ITEM_ACK )
{
	// 상태 막을까?
	SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket_ );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 현재 접속한 유저인지 찾아서 메시지 알려주기
		KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT kPacketToLg;
		kPacketToLg.m_wstrSenderNickName = GetCharName();
		kPacketToLg.m_wstrReceiverUserID = m_kUserCashInventory.GetReceiverUserID();
		kPacketToLg.m_wstrMessage		 = m_kUserCashInventory.GetMessage();

		std::vector< KBillOrderInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecBillOrderInfo.begin(); vit != kPacket_.m_vecBillOrderInfo.end(); ++vit )
		{
			SiKBillManager()->GetProductInfo( vit->m_kBillProductInfo.m_iProductNo, kPacketToLg.m_kBillProductInfo );
		}

		SendToLoginServer( ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, kPacketToLg );
	}

	//{{ 2011. 02. 08	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG
	BOOST_TEST_FOREACH( const KBillOrderInfo&, order, kPacket_.m_vecBillOrderInfo )
	{
		/*
		KNXBTProductInfo kProductInfo;
		if( SiKNexonBillingTCPManager()->GetProductInfo( kReq.m_ulProductNo, kProductInfo ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 상품을 구매하였습니다!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kReq.m_ulProductNo )
				<< END_LOG;
			continue;
		}
		}*/
		const KBillProductInfo& kProductInfo = order.m_kBillProductInfo;

		// 캐쉬템 구매 카운트 체크
		m_kUserStatistics.IncreaseCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM );
		m_kUserStatistics.IncreaseAccountCount( KUserStatistics::AGCT_BUY_CASH_ITEM );

		// 해당 캐릭터가 최초로 캐쉬템 구매한것이라면?
		if( m_kUserStatistics.GetCharacterCount( KUserStatistics::CGCT_BUY_CASH_ITEM ) == 1 )
		{
			//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
			WriteCharacterLogToDBWithProductNo( KELOG_USER_CHARACTER_LOG_NOT::LT_FIRST_BUY_CASH_ITEM, order.m_iTransNo );
#endif SERV_CHAR_LOG
			//}}
		}

		// 캐쉬템 구매 로그 남기기
		CTime tCurTime = CTime::GetCurrentTime();
		KELOG_BUY_CASH_ITEM_LOG_NOT kPacketToLog;
		kPacketToLog.m_iUserUID			= GetUID();
		kPacketToLog.m_iServerGroupID	= KBaseServer::GetKObj()->GetServerGroupID();
		kPacketToLog.m_iUnitUID			= GetCharUID();
		kPacketToLog.m_cUnitClass		= GetUnitClass();
		kPacketToLog.m_ucLevel			= GetLevel();
		kPacketToLog.m_bIsFirstBuy		= m_kUserStatistics.IsFirstBuy();
		kPacketToLog.m_cBuyType			= KELOG_BUY_CASH_ITEM_LOG_NOT::CIBT_PRESENT;
		kPacketToLog.m_iProductNo		= kProductInfo.m_iProductNo;
		kPacketToLog.m_iBuyQuantity		= kProductInfo.m_cQuantity;
		kPacketToLog.m_iTotalSalePrice	= kProductInfo.m_cQuantity * kProductInfo.m_iSalePrice;
		kPacketToLog.m_wstrRegDate		= ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		//{{ 2011. 10. 11    김민성    캐쉬 아이템 선물 할 때 받는 유저의 정보 DB에 기록하기
#ifdef SERV_PRESENT_CASH_ITEM_ADD_LOG
		kPacketToLog.m_wstrReceiverUserID = m_kUserCashInventory.GetReceiverUserID();
#endif SERV_PRESENT_CASH_ITEM_ADD_LOG
		//}} 

		START_LOG( clog, L"선물하기 로그 확인" )
			<< BUILD_LOG( m_kUserCashInventory.GetReceiverUserID() );

		SendToLogDB( ELOG_BUY_CASH_ITEM_LOG_NOT, kPacketToLog );
	}
#endif SERV_BUY_CASH_ITEM_LOG
	//}}
}


//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
IMPL_ON_FUNC( DBE_GET_NICKNAME_BY_UNITUID_ACK )
{
	KEGS_BILL_INVENTORY_INQUIRY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecOrderInfo = kPacket_.m_vecBillOrderInfo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	kPacket.m_vecPackageInfo = kPacket_.m_vecPackageInfo;
	kPacket.m_vecSubPackageInfo = kPacket_.m_vecSubPackageInfo;
	kPacket.m_vecSubPackageTrans = kPacket_.m_vecSubPackageTrans;
#endif //SERV_GLOBAL_CASH_PACKAGE
	

	//	취합
	std::vector< KBillOrderInfo >::iterator vit;
	for( vit = kPacket.m_vecOrderInfo.begin(); vit != kPacket.m_vecOrderInfo.end(); vit++ )
	{
		std::map< UidType, std::wstring >::iterator mit = kPacket_.m_mapNickName.find( vit->m_iFromUnitUID );

		if( mit != kPacket_.m_mapNickName.end() )
		{
			vit->m_wstrSenderNickName = mit->second;
		}
	}

	SendPacket( EGS_BILL_INVENTORY_INQUIRY_ACK, kPacket );
}
//}} 허상형 : [2010/8/19/] //	선물 기능 개편


IMPL_ON_FUNC( EGS_EXCHANGE_CASH_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_EXCHANGE_CASH_ACK );

	if(kPacket_.m_uiExchangeValue > 0)
	{
#ifdef SERV_COUNTRY_CN
		KELG_EXCHANGE_CASH_REQ kPacket;
		kPacket.m_PurchaserInfo.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacket.m_PurchaserInfo.m_iUserUID = GetUID();
		kPacket.m_PurchaserInfo.m_wstrUserName = GetName();
		kPacket.m_PurchaserInfo.m_iUnitUID = GetCharUID();
		kPacket.m_PurchaserInfo.m_wstrUnitNickName = GetCharName();
		kPacket.m_PurchaserInfo.m_ucLevel = GetLevel();
		kPacket.m_PurchaserInfo.m_wstrIP = KncUtil::toWideString( GetIPStr() );
		kPacket.m_PurchaserInfo.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_iPoint = static_cast<int>(kPacket_.m_uiExchangeValue);

		SendToLoginServer( ELG_EXCHANGE_CASH_REQ, kPacket );
#else // SERV_COUNTRY_CN
		KEGS_EXCHANGE_CASH_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		kPacket.m_iPoint = 0;

		SendPacket( EGS_EXCHANGE_CASH_ACK, kPacket );
#endif // SERV_COUNTRY_CN
	}
	else
	{
		KEGS_EXCHANGE_CASH_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_EXCHANGE_CASH;
		kPacket.m_iPoint = 0;

		SendPacket( EGS_EXCHANGE_CASH_ACK, kPacket );
	}
}

IMPL_ON_FUNC( EBILL_EXCHANGE_CASH_ACK )
{
	KEGS_EXCHANGE_CASH_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iPoint = kPacket_.m_iChargeCash;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_AddBonusCashInfo.m_ulCash[KGlobalCashInfo::GCT_KOG_ELSWORD_BONUS_POINT] = kPacket_.m_iChargeBonus;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SendPacket( EGS_EXCHANGE_CASH_ACK, kPacket );
}


IMPL_ON_FUNC( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK )
{
	// 상태 막을까?

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 닉네임 검사 결과
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 본인에게 선물하려는건지 체크하기
	if( kPacket_.m_wstrUserID == GetName() )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_20;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 받는사람이 체험ID인지 검사
	int iFindSize = static_cast<int>(kPacket_.m_wstrUserID.find( '/' ));
	if( iFindSize >= 0 )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_23;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}


	KEBILL_GIFT_ITEM_REQ kPacketReq;
	kPacketReq.m_wstrTotalOrderID = MakeOrderID();	// TotalOrderID를 먼저 발급

	std::vector< KNXBTPurchaseReqInfo >::const_iterator vitReqInfo;
	for( vitReqInfo = kPacket_.m_vecPurchaseReqInfo.begin(); vitReqInfo != kPacket_.m_vecPurchaseReqInfo.end(); vitReqInfo++ )
	{
		KBillProductInfo kBillProductInfo;
		if( !SiKBillManager()->GetProductInfo( ( int )vitReqInfo->m_ulProductNo, kBillProductInfo ) )
		{
			START_LOG( cerr, L"ProductInfo error." )
				<< BUILD_LOG( vitReqInfo->m_ulProductNo )
				<< END_LOG;

			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}

		KBillBuyInfo kBuyInfo;
		kBuyInfo.m_wstrOrderID = MakeOrderID();
		kBuyInfo.m_iProductNo = ( int )vitReqInfo->m_ulProductNo;
		kBuyInfo.m_iPoint = kBillProductInfo.m_iSalePrice;
		kBuyInfo.m_iProductID = kBillProductInfo.m_iProductID;
		kBuyInfo.m_iCategory = ( int )kBillProductInfo.m_cCategoryNo;
		kBuyInfo.m_wstrMemo = kBillProductInfo.m_wstrProductName;
		kBuyInfo.m_usOrderQuantity = kBillProductInfo.m_cQuantity;	// 효진 추가
		kPacketReq.m_vecBillBuyInfo.push_back( kBuyInfo );
	}	
	
	kPacketReq.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
	kPacketReq.m_iChannelingCode = GetChannelCode();

	kPacketReq.m_wstrSenderUserID = GetName();
	kPacketReq.m_uiSenderPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
	
	kPacketReq.m_wstrSenderNickName = GetCharName();

	kPacketReq.m_wstrReceiverUserID = kPacket_.m_wstrUserID;
	kPacketReq.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	
	kPacketReq.m_iSenderUserUID = GetUID();
	kPacketReq.m_iSenderUnitUID = GetCharUID();

	kPacketReq.m_iReceiverUserUID = kPacket_.m_iReceiverUserUID;
	kPacketReq.m_iReceiverUnitUID = kPacket_.m_iReceiverUnitUID;
	
	kPacketReq.m_wstrPresentMessage = kPacket_.m_wstrMessage;

	kPacketReq.m_BuyCharInfo.m_cLevel = GetLevel();
	kPacketReq.m_BuyCharInfo.m_iUnitClass = GetUnitClass();
	kPacketReq.m_RecvCharInfo.m_cLevel = kPacket_.m_cReceiverLevel;
	kPacketReq.m_RecvCharInfo.m_iUnitClass = kPacket_.m_iReceiverUnitClass;


	m_kUserCashInventory.SetReceiverUserID( kPacket_.m_wstrUserID ); // 선물할 UserID 임시 저장
	m_kUserCashInventory.SetMessage( kPacket_.m_wstrMessage );		 // 선물하기 메시지 임시 저장

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(!IsAbleToUseCashType(kPacket_.m_iUseCashType))
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GLOBAL_BILLING_01;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	kPacketReq.m_iUseCashType = kPacket_.m_iUseCashType;

#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	kPacketReq.m_wstrUserIP = KncUtil::toWideString( GetIPStr() );

	// 선물 가능 조건 여부 먼저 체크함 (구매 제한 등등)
	SendToKOGBillingDB( EBILL_PREPARE_GIFT_ITEM_REQ, kPacketReq );
}

IMPL_ON_FUNC( EBILL_PREPARE_GIFT_ITEM_ACK )
{
	if(kPacket_.m_iOK != NetError::NET_OK)
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

#if defined (SERV_COUNTRY_CN) || defined (SERV_COUNTRY_PH)
	// 중국은 사내에서 다 처리함
	SendToKOGBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
#else // SERV_COUNTRY_CN
	DWORD dwBillingFlag = KSimLayer::GetKObj()->GetBillingFlag();
	DWORD dwNationFlag = KSimLayer::GetKObj()->GetDwNationFlag();
	DWORD dwPublisherBillingConnectType = KSimLayer::GetKObj()->GetPublisherBillingConnectType();

	if(KSimLayer::BF_INTERNAL == dwBillingFlag)
	{
		SendToKOGBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
	}
	else if (dwPublisherBillingConnectType == KSimLayer::PBCT_UNUSUAL && dwNationFlag == KSimLayer::NF_TW)
	{
		// 구분해서 DB로 보낼지 TCP로 보낼지 결정해야 함

		// 일반 캐시는 TCP 로 차감
		if (kPacket_.m_kEBILL_GIFT_ITEM_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_CASH)
			SendToPublisherBilling( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
		// 엘소드 전환 캐시는 DB로 차감
		else if (kPacket_.m_kEBILL_GIFT_ITEM_REQ.m_iUseCashType == KGlobalCashInfo::GCT_PUBLISHER_ELSWORD_CASH)
			SendToPublisherBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
		else
		{
			START_LOG( cerr, L"선물 UseCashType에 문제가 있습니다. 정상값 아님" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_kEBILL_GIFT_ITEM_REQ.m_iUseCashType )
				<< END_LOG;
		}
	}
	else if(dwPublisherBillingConnectType == KSimLayer::PBCT_DB)
	{
		SendToPublisherBillingDB( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
	}
	else
	{
		SendToPublisherBilling( EBILL_GIFT_ITEM_REQ, kPacket_.m_kEBILL_GIFT_ITEM_REQ );
	}
#endif // SERV_COUNTRY_CN
}

#ifdef SERV_EVENT_BUY_FAKE_ITEM
IMPL_ON_FUNC( EBILL_CHECK_BUY_FAKE_ITEM_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_sQuantity	   = 1;
		kPacketToDB.m_iRewardID	   = 834;	// 참 잘했어요 도장! 우편지급

		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );

		//START_LOG( clog, L"김석근_우편이 정상으로 보내졌을까?" )
		//	<< BUILD_LOG( kPacketToDB.m_iFromUnitUID )
		//	<< BUILD_LOG( kPacketToDB.m_iToUnitUID )
		//	<< BUILD_LOG( kPacketToDB.m_iRewardType )
		//	<< BUILD_LOG( kPacketToDB.m_sQuantity )
		//	<< BUILD_LOG( kPacketToDB.m_iRewardID )
		//	<< END_LOG;
	}
	else
	{
		KEGS_BUY_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_BUY_CASH_ITEM_ACK, kPacket );
	}

}
#endif //SERV_EVENT_BUY_FAKE_ITEM


#ifdef SERV_COUNTRY_PH
IMPL_ON_FUNC( EJSON_GN_CHANGE_GAME_CURRENCY_REQ )
{
	KEBILL_GN_CHANGE_GAME_CURRENCY_REQ kPacket;	

	if (m_kNexonAccountInfo.m_uiNexonSN == kPacket_.m_uiGarenaUID)
	{		
		kPacket.m_kGarenaREQ = kPacket_;
		kPacket.m_PurchaserInfo.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacket.m_PurchaserInfo.m_iUserUID = GetUID();
		kPacket.m_PurchaserInfo.m_wstrUserName = GetName();
		kPacket.m_PurchaserInfo.m_iUnitUID = GetCharUID();
		kPacket.m_PurchaserInfo.m_wstrUnitNickName = GetCharName();
		kPacket.m_PurchaserInfo.m_ucLevel = GetLevel();
		kPacket.m_PurchaserInfo.m_wstrIP = KncUtil::toWideString( GetIPStr() );
		kPacket.m_PurchaserInfo.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacket.m_iResult = 0;
		//SendToKOGBillingDB( EBILL_GN_CHANGE_GAME_CURRENCY_REQ, kPacket);		

		START_LOG( clog, L"Garena 에서 캐쉬 전환을 위한 유저 조회 요청" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kNexonAccountInfo.m_uiNexonSN )
			<< BUILD_LOG( kPacket_.m_uiGameCurrency_Amount )
			<< BUILD_LOG( kPacket_.m_uiShell_Amount )
			<< BUILD_LOG( kPacket_.m_strGarenaTransactionID )
			<< END_LOG;
	}
	else
	{
		kPacket.m_iResult = -1;
		kPacket.m_kGarenaREQ.m_iSessionUID = kPacket_.m_iSessionUID;
		START_LOG( cerr, L"Garena에서 캐쉬 전환 요청 들어 왔는데 우리한테 없는 유저다!!" )
			<< BUILD_LOG( kPacket_.m_uiGarenaUID)
			<< END_LOG;
	}

	SendToLoginServer( ELG_GN_CHANGE_GAME_CURRENCY_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_EXCHANGE_CASH_NOT )
{
	KEGS_EXCHANGE_CASH_CLIENT_NOT kPacket;

	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iPoint = kPacket_.m_iPoint;
	kPacket.m_AddBonusCashInfo = kPacket_.m_AddBonusCashInfo;

	SendPacket( EGS_EXCHANGE_CASH_CLIENT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EBILL_GARENA_PREPARE_PRESENT_CHECK_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 닉네임 검사 결과
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName ) )
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}
	
	// 본인에게 선물하려는건지 체크하기
#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName, GetCharName()) )
#else 
	if( kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName == GetCharName() )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_20;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	// 정상적인 상품인지 검사 - 차후에 필요성 체크
	std::vector< KNXBTPurchaseReqInfo >::iterator vit;
	for( vit = kPacket_.m_kEGSPresentCashItemREQ.m_vecPurchaseReqInfo.begin(); vit != kPacket_.m_kEGSPresentCashItemREQ.m_vecPurchaseReqInfo.end(); vit++ )
	{
#ifdef SERV_GLOBAL_BILLING
		int iItemID = SiKBillManager()->GetItemID( vit->m_ulProductNo );;
#else // SERV_GLOBAL_BILLING
		int iItemID = SiKNexonBillingTCPManager()->GetItemID( vit->m_ulProductNo );
#endif // SERV_GLOBAL_BILLING
		if( iItemID <= 0 )
		{
			START_LOG( cerr, L"아이템 ID 변환 실패." )
				<< BUILD_LOG( vit->m_ulProductNo )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}

		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
				<< BUILD_LOG( iItemID )
				<< END_LOG;

			KEGS_PRESENT_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_00;
			SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
			return;
		}

#ifdef SERV_USE_ENABLE_GIFT
		KBillProductInfo kBillProductInfo2;
		if(SiKBillManager()->GetProductInfo((int)vit->m_ulProductNo, kBillProductInfo2))
		{
			if(kBillProductInfo2.m_bEnableGift == false)
			{
				START_LOG( cerr, L"선물할 수 없는 아이템" )
					<< BUILD_LOG( vit->m_ulProductNo )
					<< BUILD_LOG( iItemID )
					<< END_LOG;

				KEGS_PRESENT_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_11;
				SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
				return;
			}
		}
#else SERV_USE_ENABLE_GIFT
		// 소지품 확장과 슬롯 확장은 선물 불가능
		switch( pItemTemplet->m_ItemID )
		{
		case CXSLItem::CI_EXPAND_INVENTORY:
		case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
		case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
		case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
			//{{ 2012. 12. 19	최육사	아라 전용 캐쉬템
#ifdef SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
		case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
#endif SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
			//}}
			{
				KEGS_PRESENT_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_27;
				SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
				return;
			}
			break;
		}
#endif SERV_USE_ENABLE_GIFT
	}

	// 찜에서 캐쉬구매
	m_kUserWishList.SetBuyCashItemInWishList( kPacket_.m_kEGSPresentCashItemREQ.m_bIsWishList );

	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ kPacketToDB;
	kPacketToDB.m_wstrReceiverNickName = kPacket_.m_kEGSPresentCashItemREQ.m_wstrReceiverNickName;
	kPacketToDB.m_wstrMessage		   = kPacket_.m_kEGSPresentCashItemREQ.m_wstrMessage;
	kPacketToDB.m_vecPurchaseReqInfo   = kPacket_.m_kEGSPresentCashItemREQ.m_vecPurchaseReqInfo;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	if(!IsAbleToUseCashType(kPacket_.m_kEGSPresentCashItemREQ.m_iUseCashType))
	{
		KEGS_PRESENT_CASH_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GLOBAL_BILLING_01;
		SendPacket( EGS_PRESENT_CASH_ITEM_ACK, kPacket );
		return;
	}

	kPacketToDB.m_iUseCashType = kPacket_.m_kEGSPresentCashItemREQ.m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

	SendToGameDB( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, kPacketToDB );
}
#endif //SERV_COUNTRY_PH


#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
IMPL_ON_FUNC( EGS_CASH_DIRECT_CHARGE_CN_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CASH_DIRECT_CHARGE_CN_ACK );

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH_TEST_MODE
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		START_LOG( cerr, L"[CN] Direct Charge 주소 요청을 영자도 아닌데 했다!!." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		KEGS_CASH_DIRECT_CHARGE_CN_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket(EGS_CASH_DIRECT_CHARGE_CN_ACK, kPacketAck);

		return;
	}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

	if( kPacket_.m_iUserUID == GetUID())
	{
		START_LOG( cout, L"[TEST] Direct Charge 주소 요청." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		// 로그인 서버로 전달 (부족한 정보는 로그인서버가 채워서 마지막 전송) //

		KELG_CASH_DIRECT_CHARGE_CN_REQ kPacketReq;
		kPacketReq.m_iUserUID = GetUID();
		kPacketReq.m_wstrIP = KncUtil::toWideString( GetIPStr() );;
		kPacketReq.m_wstrServiceAccountID = GetName();
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_wstrUnitNickName = GetCharName();

		SendToLoginServer( ELG_CASH_DIRECT_CHARGE_CN_REQ, kPacketReq );	// SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	}
	else
	{
		START_LOG( cerr, L"[CN] Direct Charge 주소 요청 보낸 유저와 서버 유저가 틀리다!?." )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		// 오류 처리
		KEGS_CASH_DIRECT_CHARGE_CN_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		SendPacket(EGS_CASH_DIRECT_CHARGE_CN_ACK, kPacketAck);
	}
}

_IMPL_ON_FUNC( ELG_CASH_DIRECT_CHARGE_CN_ACK, KEGS_CASH_DIRECT_CHARGE_CN_ACK)
{
	START_LOG( cout, L"[TEST] Direct Charge 주소 요청 값이 왔다!" )
		<< BUILD_LOG( kPacket_.m_iOK )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() )
		<< END_LOG;

	SendPacket(EGS_CASH_DIRECT_CHARGE_CN_ACK, kPacket_);
}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#endif SERV_GLOBAL_BILLING

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


