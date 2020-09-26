#include "ActorManager.h"
#include "NexonBillingTCP.h"

#include "NexonBillingTCPPacket.h"
#include "NexonBillingTCPManager.h"
#include "NetError.h"
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLSkillTree.h"

//{{ 2012. 09. 25	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	#include "Temp.h"
#endif SERV_EVENT_BINGO
//}}

#define CLASS_TYPE  KNexonBillingTCP

KNexonBillingTCP::KNexonBillingTCP( void )
{
}

KNexonBillingTCP::~KNexonBillingTCP( void )
{
	End( 15000 );
}

void KNexonBillingTCP::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
    CASE( ENX_BT_INITIALIZE_ACK );
    CASE( ENX_BT_HEART_BEAT_ACK );
    CASE( ENX_BT_CATEGORY_INQUIRY_ACK );
    CASE( ENX_BT_PRODUCT_INQUIRY_ACK );
    CASE( ENX_BT_CHECK_BALANCE_REQ );
    CASE( ENX_BT_CHECK_BALANCE_ACK );
    CASE( ENX_BT_PURCHASE_ITEM_REQ );
    CASE( ENX_BT_PURCHASE_ITEM_ACK );
    CASE( ENX_BT_PURCHASE_GIFT_REQ );
    CASE( ENX_BT_PURCHASE_GIFT_ACK );
	//{{ 2009. 10. 14  최육사	자동결제
	CASE( ENX_BT_CHECK_AUTO_PAYMENT_USER_REQ );
	CASE( ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK );
	//}}
    CASE( ENX_BT_COUPON_INQUIRY_REQ );
    CASE( ENX_BT_COUPON_INQUIRY_ACK );
	//{{ 2009. 3. 24  최육사	쿠폰사용여부 조회
	CASE( ENX_BT_COUPON_INQUIRY_NEW_REQ );
	CASE( ENX_BT_COUPON_INQUIRY_NEW_ACK );
	//}}
    CASE( ENX_BT_COUPON_USING_REQ );
    CASE( ENX_BT_COUPON_USING_ACK );	
    CASE( ENX_BT_NISMS_INVENTORY_INQUIRY_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_INQUIRY_ACK );
	//{{ 2008. 6. 20  최육사  선물하기
	CASE( ENX_BT_NISMS_INVENTORY_CHECK_REQ ); 
	CASE( ENX_BT_NISMS_INVENTORY_CHECK_ACK );
	CASE( ENX_BT_NISMS_INVENTORY_READABLE_REQ );
	CASE( ENX_BT_NISMS_INVENTORY_READABLE_ACK );
	//}}
    CASE( ENX_BT_NISMS_INVENTORY_READ_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_READ_ACK );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ACK );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ );
    CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK );
	//{{ 2008. 6. 16  최육사  넥슨빌링 패키지 픽업
	CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ );
	CASE( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK );
	//}}
	//{{ 2010. 11. 17	최육사	넥슨 빌링 청약 철회
#ifdef SERV_NX_BILLING_REFUND
	CASE( ENX_BT_REFUND_REQ );
	CASE( ENX_BT_REFUND_ACK );
#endif SERV_NX_BILLING_REFUND
	//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-21	// 박세훈
	CASE( ENX_BT_NISMS_ITEM_COUPON_USING_REQ );
	CASE( ENX_BT_NISMS_ITEM_COUPON_USING_ACK );

	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ );
	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK );

	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ );
	CASE( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK );
#endif // SERV_NEXON_COUPON_SYSTEM

	default:
		START_LOG( cerr, L"핸들러가 지정되지 않은 이벤트." )
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

IMPL_ON_FUNC( ENX_BT_INITIALIZE_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );
    SiKNexonBillingTCPManager()->HandleInitializeAck( bSucceed );

    if( bSucceed )
    {
        KENX_BT_CATEGORY_INQUIRY_REQ kPacketCIR;
        kPacketCIR.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
        kPacketCIR.m_bytePacketType = KNexonBillingTCPPacket::CATEGORY_INQUIRY;

        boost::shared_ptr< KNexonBillingTCPPacket > spPacketCIR( new KNexonBillingTCPPacket );
        spPacketCIR->Write( kPacketCIR );
        SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketCIR );
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_HEART_BEAT_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    if( kPacket_.m_ulResult == 0x11 )
    {
		//////////////////////////////////////////////////////////////////////////		
		//{{ 2010. 01. 28  최육사	빌링예외처리
#ifdef SERV_BILLING_PRODUCT
		if( SiKNexonBillingTCPManager()->GetProductPageNumber() != 0 )
		{
			START_LOG( cout, L"이미 상품 리스트를 다시 받는 중입니다. 상품 리스트를 모두 받을때까지 상품 정보 갱신 요청은 무시됩니다." )
				<< BUILD_LOG( SiKNexonBillingTCPManager()->GetProductPageNumber() );
			return;
		}
		
		// 상품 정보를 초기화 한다.
		SiKNexonBillingTCPManager()->ResetProductInfo();
#endif SERV_BILLING_PRODUCT
		//}}
		//////////////////////////////////////////////////////////////////////////

        // 상품정보를 다 다시 받는다.		
        SiKNexonBillingTCPManager()->ResetProductPageNumber();

        KENX_BT_PRODUCT_INQUIRY_REQ kPacket;
        kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
        kPacket.m_bytePacketType = KNexonBillingTCPPacket::PRODUCT_INQUIRY;
        kPacket.m_ulPageIndex = SiKNexonBillingTCPManager()->GetNextProductPageNumber();        
		kPacket.m_ulRowPerPage = ROW_PER_PAGE;

        boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
        spPacket->Write( kPacket );
        SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );

		START_LOG( cout, L"상품정보가 바뀌었습니다, 상품리스트를 다시 받습니다." );
		return;
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_CATEGORY_INQUIRY_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    if( bSucceed )
    {
        KENX_BT_PRODUCT_INQUIRY_REQ kPacket;
        kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
        kPacket.m_bytePacketType = KNexonBillingTCPPacket::PRODUCT_INQUIRY;
        kPacket.m_ulPageIndex = SiKNexonBillingTCPManager()->GetNextProductPageNumber();		
		kPacket.m_ulRowPerPage = ROW_PER_PAGE;

        boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
        spPacket->Write( kPacket );
        SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_PRODUCT_INQUIRY_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    if( bSucceed )
    {
        // 예전 릴리즈 틱과 다르면 상품 정보가 갱신되었다는 것이다. 넣기 전에 다 지운다.
        if( !SiKNexonBillingTCPManager()->SetReleaseTick( kPacket_.m_iReleaseTick ) )
        {
			//////////////////////////////////////////////////////////////////////////			
			//{{ 2010. 01. 28  최육사	빌링예외처리
#ifdef SERV_BILLING_PRODUCT
			START_LOG( cout, L"Release Tick 갱신" )
				<< BUILD_LOG( kPacket_.m_iReleaseTick );
#else
			SiKNexonBillingTCPManager()->ResetProductInfo();
#endif SERV_BILLING_PRODUCT
			//}}
			//////////////////////////////////////////////////////////////////////////			
        }

        std::vector< KNXBTProductInfo >::iterator vit;
        for( vit = kPacket_.m_vecNXBTProductInfo.begin(); vit != kPacket_.m_vecNXBTProductInfo.end(); ++vit )
        {
            SiKNexonBillingTCPManager()->AddProductInfo( *vit );
        }

        if( !kPacket_.m_vecNXBTProductInfo.empty() )
        {
            KENX_BT_PRODUCT_INQUIRY_REQ kPacket;
            kPacket.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
            kPacket.m_bytePacketType = KNexonBillingTCPPacket::PRODUCT_INQUIRY;
            kPacket.m_ulPageIndex = SiKNexonBillingTCPManager()->GetNextProductPageNumber();
			kPacket.m_ulRowPerPage = ROW_PER_PAGE;

            boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
            spPacket->Write( kPacket );
            SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
        }
        else
        {
			SiKNexonBillingTCPManager()->BuildProductPage();
			SiKNexonBillingTCPManager()->ResetProductPageNumber();

			// 클라이언트에 상품 정보를 다시 다 받으라고 알린다.
			KEvent kEvent;
			UidType anTrace[2] = { -1, -1 };
			kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_PRODUCT_INFO_CHANGED_NOT );
			KActorManager::GetKObj()->SendToAll( kEvent );

			START_LOG( cout, L"빌링서버로부터 모든 상품 리스트를 다 받았습니다!" )
				<< BUILD_LOG( SiKNexonBillingTCPManager()->GetTotalProductCount() );
        }
    }

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_CHECK_BALANCE_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_CHECK_BALANCE_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;

        return;
    }

    KEGS_CHECK_BALANCE_ACK kPacket;
    switch( kPacket_.m_ulResult )
    {
    case 1:
        kPacket.m_iOK = NetError::NET_OK;
        kPacket.m_ulBalance = kPacket_.m_ulBalance;
        break;
    case 12001:
        kPacket.m_iOK = NetError::ERR_NX_SHOP_06;
        break;
    case 12002:
        kPacket.m_iOK = NetError::ERR_NX_SHOP_05;
        break;
		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
    default:
        kPacket.m_iOK = NetError::ERR_NX_SHOP_04;
		START_LOG( cerr, L"잔액 조회 못하는 이유가 무엇인가?" )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( kPacket_.m_ulResult );
        break;
    }

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { iUserUID, -1 };
    spEvent->SetData( PI_GS_PROXY, anTrace, EGS_CHECK_BALANCE_ACK, kPacket );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_PURCHASE_ITEM_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_PURCHASE_ITEM_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;

        return;
    }

    KEGS_BUY_CASH_ITEM_ACK kPacket;

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	kPacket.m_bUseCoupon = ( kPacket_.m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON ) ? true : false;
#endif // SERV_NEXON_COUPON_SYSTEM

    switch( kPacket_.m_ulResult )
    {
    case 1:
        kPacket.m_iOK = NetError::NET_OK;
        break;
    case 0:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
        break;
    case 2:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_02;
        break;
    case 3:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
        break;
    case 4:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_04;
        break;
    case 5:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_05;
        break;

	case 10:
	case 11:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_07;	// 쿠폰 오류입니다.
		break;

    case 21:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_06;
        break;
    case 22:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_07;
        break;
    case 23:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_08;
        break;
    case 24:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_09;
        break;
    case 25:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_10;
        break;
    case 26:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_11;
        break;
    case 27:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_12;
        break;
    case 28:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_13;
        break;
		//{{ 2010. 10. 06	최육사	구매 오류
	case 81:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_20;	// 쿠폰 총 사용 횟수를 초과하여 사용하실 수 없습니다.
		break;
	case 99:
		START_LOG( cout, L"넥슨 빌링 DB오류로 인해 구매 실패!" )
			<< BUILD_LOG( kPacket_.m_ulOrderNo );
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		break;
		//}}
    case 12001:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_14;
        break;
    case 12002:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_15;
        break;
    case 12040:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_16;
        break;
    case 255:
        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_17;
        break;
		//{{ 2010. 02. 16  최육사	구매 에러 코드 추가
	case 11001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_42;
		break;
	case 11002:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_43;
		break;
		//}}
		//{{ 2010. 8. 18	최육사	구매 에러 코드 추가
	case 120062:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_44;
		break;
		//}}
		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
    default:
		START_LOG( cerr, L"정의되지 않은 캐쉬아이템 구매 에러 코드입니다." )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

        kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
        break;
    }

    if( bSucceed )
    {
        std::vector< KNXBTPurchaseAckInfo >::const_iterator vit;
        for( vit = kPacket_.m_vecNXBTPurchaseInfo.begin(); vit != kPacket_.m_vecNXBTPurchaseInfo.end(); ++vit )
        {
            // 부활석이면 바로 가져온다. 소지품 확장도 바로 가져온다.
            const int iItemID = SiKNexonBillingTCPManager()->GetItemID( vit->m_ulProductNo );
            if( iItemID <= 0 )
            {
                START_LOG( cerr, L"아이템 ID 변환 실패." )
                    << BUILD_LOG( vit->m_ulProductNo )
                    << BUILD_LOG( iItemID )
                    << END_LOG;

                continue;
            }

			// [참고] 부활석과 인벤토리 확장의 경우 아이템으로 존재하지 않으므로 바로 PICK_UP패킷을 날린다.
			switch( iItemID )
			{
			case CXSLItem::CI_RESURRECTION_STONE:
			case CXSLItem::CI_EXPAND_INVENTORY:
			case CXSLItem::CI_EXPAND_CHAR_SLOT_1:
			case CXSLItem::CI_EXPAND_CHAR_SLOT_2:
			case CXSLItem::CI_EXPAND_SKILL_SLOT_PERMANENT:
				//{{ 2012. 12. 19	최육사	아라 전용 캐쉬템
#ifdef SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ARA:
#endif SERV_EXPAND_SKILL_SLOT_CASH_ITEM_FOR_ARA
				//}}
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// 적용날짜: 2013-07-11
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#ifdef SERV_CASH_ITEM_FOR_ADD
			case CXSLItem::CI_EXPAND_SKILL_SLOT_ADD:
#endif	// SERV_CASH_ITEM_FOR_ELESIS

				//{{ 2009. 8. 7  최육사		은행
			case CXSLItem::CI_BANK_MEMBERSHIP_SILVER:
			case CXSLItem::CI_BANK_MEMBERSHIP_GOLD:
			case CXSLItem::CI_BANK_MEMBERSHIP_EMERALD:
			case CXSLItem::CI_BANK_MEMBERSHIP_DIAMOND:
			case CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM:
			case CXSLItem::CI_BANK_MEMBERSHIP_ONE_PACKAGE:
				//}}
				//{{ 2010. 04. 27  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
			case CXSLItem::CI_CASH_SKILL_NOTE_ITEM:
#endif SERV_SKILL_NOTE
				//}}
				//{{ 2011. 12. 13  김민성	퀵 슬롯 확장 아이템
#ifdef SERV_EXPAND_QUICK_SLOT
			case CXSLItem::CI_EXPAND_QUICK_SLOT:
			case CXSLItem::CI_EXPAND_QUICK_SLOT_ARA:
#ifdef SERV_CASH_ITEM_FOR_ELESIS	// 적용날짜: 2013-07-11
			case CXSLItem::CI_EXPAND_QUICK_SLOT_ELESIS:
#endif	// SERV_CASH_ITEM_FOR_ELESIS
#endif SERV_EXPAND_QUICK_SLOT

#ifdef SERV_SKILL_PAGE_SYSTEM
			case CXSLItem::CI_EXPAND_SKILL_PAGE:
#endif // SERV_SKILL_PAGE_SYSTEM

				//}}
				{
					KENX_BT_NISMS_INVENTORY_PICK_UP_REQ kPacketReq;
					kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
					kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
					kPacketReq.m_ulOrderNo = kPacket_.m_ulOrderNo;
					kPacketReq.m_ulProductNo = vit->m_ulProductNo;
					kPacketReq.m_usOrderQuantity = vit->m_usOrderQuantity;

					SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacketReq.m_ulPacketNo, iUserUID );
					boost::shared_ptr< KNexonBillingTCPPacket > spPacketReq( new KNexonBillingTCPPacket );
					spPacketReq->Write( kPacketReq );
					SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketReq );
				}
				break;

				//{{ 2009. 8. 4  최육사		봉인스킬
			default:
				{
					// 봉인 스킬 해제 아이템 or 전직 캐쉬템
                    if( SiCXSLSkillTree()->IsUnsealItemID( iItemID ) == true  ||
						//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
						CXSLItem::IsClassChangeCashItem( iItemID ) == true	  ||
#endif SERV_UNIT_CLASS_CHANGE_ITEM
						//}}
						CXSLItem::IsJobChangeCashItem( iItemID ) == true )
					{
						KENX_BT_NISMS_INVENTORY_PICK_UP_REQ kPacketReq;
						kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
						kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
						kPacketReq.m_ulOrderNo = kPacket_.m_ulOrderNo;
						kPacketReq.m_ulProductNo = vit->m_ulProductNo;
						kPacketReq.m_usOrderQuantity = vit->m_usOrderQuantity;

						SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacketReq.m_ulPacketNo, iUserUID );
						boost::shared_ptr< KNexonBillingTCPPacket > spPacketReq( new KNexonBillingTCPPacket );
						spPacketReq->Write( kPacketReq );
						SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketReq );
					}
				}
				break;
				//}}
			}

            // 구매한 정보를 ack 에 준다.
            KNXBTPurchaseReqInfo kPRInfo;
            kPRInfo.m_ulProductNo = vit->m_ulProductNo;
            kPRInfo.m_usOrderQuantity = vit->m_usOrderQuantity;
            kPacket.m_vecPurchaseReqInfo.push_back( kPRInfo );
        }
    }
	else
	{
		std::vector< KNXBTPurchaseAckInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecNXBTPurchaseInfo.begin(); vit != kPacket_.m_vecNXBTPurchaseInfo.end(); ++vit )
		{
			// 구매한 정보를 ack 에 준다.
			KNXBTPurchaseReqInfo kPRInfo;
			kPRInfo.m_ulProductNo = vit->m_ulProductNo;
			kPRInfo.m_usOrderQuantity = vit->m_usOrderQuantity;
			kPacket.m_vecPurchaseReqInfo.push_back( kPRInfo );
		}

		START_LOG( cerr, L"아이템 구매 실패" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		BOOST_TEST_FOREACH( const KNXBTPurchaseAckInfo&, kProductAckInfo, kPacket_.m_vecNXBTPurchaseInfo )
		{
			KNXBTProductInfo kProductInfo;
			if( SiKNexonBillingTCPManager()->GetProductInfo( kProductAckInfo.m_ulProductNo, kProductInfo ) == false )
			{
				START_LOG( cerr, L"ProductInfo를 찾을 수 없습니다." )
					<< BUILD_LOG( kProductInfo.m_ulProductNo )
					<< END_LOG;
				continue;
			}

			// 상품 아이템ID 얻기
			int iItemID = _wtoi( kProductInfo.m_wstrProductID.c_str() );
			if( iItemID <= 0 )
			{
				START_LOG( cerr, L"아이템 ID 변환 실패." )
					<< BUILD_LOG( kProductInfo.m_ulProductNo )
					<< BUILD_LOG( kProductInfo.m_wstrProductID )
					<< BUILD_LOG( iItemID )
					<< END_LOG;
				continue;
			}

			START_LOG( cerr, L"구매 실패 상품 정보" )
				<< BUILD_LOG( kProductAckInfo.m_ulProductNo )
				<< BUILD_LOG( kProductAckInfo.m_usOrderQuantity )
				<< BUILD_LOG( kProductInfo.m_wstrProductID )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
		}
	}

    //KEventPtr spEvent( new KEvent );
    //UidType anTrace[2] = { iUserUID, -1 };
    //spEvent->SetData( PI_GS_PROXY, anTrace, ENX_BT_PURCHASE_ITEM_ACK, kPacket );
    //KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	
	//{{ 2008. 5. 13  최육사  GSUser에 한번 거쳐서 가도록 수정
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_PURCHASE_ITEM_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	//}}

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

//{{ 2008. 6. 12  최육사  선물하기
IMPL_ON_FUNC( ENX_BT_PURCHASE_GIFT_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_PURCHASE_GIFT_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	KEGS_PRESENT_CASH_ITEM_ACK kPacket;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	kPacket.m_bUseCoupon = ( kPacket_.m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON ) ? true : false;
#endif // SERV_NEXON_COUPON_SYSTEM

	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;
	case 0:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		break;
	case 2:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_02;
		break;
	case 3:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_03;
		break;
	case 4:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_04;
		break;
	case 5:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_05;
		break;
	case 10:
	case 11:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_07;	// 쿠폰 오류입니다.
		break;
	case 21:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_06;
		break;
	case 22:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_07;
		break;
	case 23:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_08;
		break;
	case 24:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_09;
		break;
	case 25:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_10;
		break;
	case 26:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_11;
		break;
	case 27:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_12;
		break;
	case 28:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_13;
		break;
	case 81:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_20;	// 쿠폰 총 사용 횟수를 초과하여 사용하실 수 없습니다.
		break;
	case 1000:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_24;
		break;
		//{{ 2008. 6. 26  최육사  선물하기
	case 1001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_28;
		break;
		//}}
	case 12001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_14;
		break;
	case 12002:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_15;
		break;
	case 12040:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_16;
		break;
	case 255:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_17;
		break;
		//{{ 2010. 02. 16  최육사	구매 에러 코드 추가
	case 11001:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_42;
		break;
	case 11002:
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_43;
		break;
		//}}
		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"정의되지 않은 선물 구매 에러 코드입니다." )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_01;
		break;
	}

	if( bSucceed )
	{
		std::vector< KNXBTPurchaseAckInfo >::iterator vit;
		for( vit = kPacket_.m_vecNXBTPurchaseInfo.begin(); vit != kPacket_.m_vecNXBTPurchaseInfo.end(); vit++ )
		{
			// 부활석이면 바로 가져온다. 소지품 확장도 바로 가져온다.
			//int iItemID = SiKNexonBillingTCPManager()->GetItemID( vit->m_ulProductNo );
			//if( iItemID <= 0 )
			//{
			//	START_LOG( cerr, L"아이템 ID 변환 실패." )
			//		<< BUILD_LOG( vit->m_ulProductNo )
			//		<< BUILD_LOG( iItemID )
			//		<< END_LOG;

			//	continue;
			//}

			// [참고] 선물하기가 불가능한 상품은 미리 검사를 했기때문에 여기서 따로 처리할 필요는 없다.

			// 스레드에 안전하지 못한 코드 [딱히 호출할 이유도 없는듯.. 이미 구매는 이루어 졌는데뭐..]
			//const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
			//if( pItemTemplet == NULL )
			//{
			//    START_LOG( cerr, L"아이템 템플릿을 얻지 못함." )
			//        << BUILD_LOG( iItemID )
			//        << END_LOG;

			//    continue;
			//}

			// [참고] 부활석과 인벤토리 확장의 경우 아이템으로 존재하지 않으므로 바로 PICK_UP패킷을 날린다.

			//if( iItemID == CXSLItem::CI_RESURRECTION_STONE ||
			//	iItemID == CXSLItem::CI_EXPAND_INVENTORY )
			//{
			//	KENX_BT_NISMS_INVENTORY_PICK_UP_REQ kPacketReq;
			//	kPacketReq.m_ulPacketNo = SiKNexonBillingTCPManager()->GetNextPacketNo();
			//	kPacketReq.m_bytePacketType = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
			//	kPacketReq.m_ulOrderNo = kPacket_.m_ulOrderNo;
			//	kPacketReq.m_ulProductNo = vit->m_ulProductNo;
			//	kPacketReq.m_usOrderQuantity = vit->m_usOrderQuantity;

			//	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacketReq.m_ulPacketNo, iUserUID );
			//	boost::shared_ptr< KNexonBillingTCPPacket > spPacketReq( new KNexonBillingTCPPacket );
			//	spPacketReq->Write( kPacketReq );
			//	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacketReq );
			//}

			// 구매한 정보를 ack 에 준다.
			KNXBTPurchaseReqInfo kPRInfo;
			kPRInfo.m_ulProductNo = vit->m_ulProductNo;
			kPRInfo.m_usOrderQuantity = vit->m_usOrderQuantity;
			kPacket.m_vecPurchaseReqInfo.push_back( kPRInfo );
		}
	}
	else
	{
		START_LOG( cerr, L"아이템 선물 실패" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
	}

	//KEventPtr spEvent( new KEvent );
	//UidType anTrace[2] = { iUserUID, -1 };
	//spEvent->SetData( PI_GS_PROXY, anTrace, EGS_BUY_CASH_ITEM_ACK, kPacket );
	//KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	//{{ 2008. 5. 13  최육사  GSUser에 한번 거쳐서 가도록 수정
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_PURCHASE_GIFT_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	//}}

	//LOG_SUCCESS( bSucceed )
	//	<< BUILD_LOG( kPacket_.m_ulResult )
	//	<< END_LOG;
}
//}}

//{{ 2009. 10. 14  최육사	자동결제
IMPL_ON_FUNC( ENX_BT_CHECK_AUTO_PAYMENT_USER_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK )
{
	//bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )			
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		return;
	}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK, kPacket_ );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	//LOG_SUCCESS( bSucceed )
	//	<< BUILD_LOG( kPacket_.m_ulResult )
	//	<< END_LOG;
}
//}}

IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_REQ )
{
	// 현재 엘소드에서는 사용하지 않는 기능입니다.
}

IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_ACK )
{
	// 현재 엘소드에서는 사용하지 않는 기능입니다.
}

//{{ 2009. 3. 24  최육사	쿠폰사용여부 조회
IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_NEW_REQ )
{
	// 현재 엘소드에서는 사용하지 않는 기능입니다.
}

IMPL_ON_FUNC( ENX_BT_COUPON_INQUIRY_NEW_ACK )
{
	// 현재 엘소드에서는 사용하지 않는 기능입니다.
}
//}}

IMPL_ON_FUNC( ENX_BT_COUPON_USING_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_COUPON_USING_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 1 );

    KEGS_APPLY_COUPON_ACK kPacket;
    switch( kPacket_.m_ulResult )
    {
    case 1:
        kPacket.m_iOK = NetError::NET_OK;
        break;
	case 5:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_05;
		break;
    case 25:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_03;
        break;
    case 51:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_00;
        break;
    case 52:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_01;
        break;
	case 61:		
    case 62:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
        break;
		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
    default:
        kPacket.m_iOK = NetError::ERR_NX_COUPON_04;
		START_LOG( cerr, L"정의되지 않은 쿠폰 에러코드입니다." )
		    << BUILD_LOG( kPacket_.m_ulResult )
		    << END_LOG;
        break;
    }

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;
        
        return;
    }

    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { iUserUID, -1 };
    spEvent->SetData( PI_GS_PROXY, anTrace, EGS_APPLY_COUPON_ACK, kPacket );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_INQUIRY_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_INQUIRY_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    KEGS_PURCHASED_CASH_ITEM_LIST_ACK kPacket;
    kPacket.m_iOK = bSucceed ? NetError::NET_OK : NetError::ERR_NX_SHOP_00;
    kPacket.m_vecOrderInfo	 = kPacket_.m_vecNXBTOrderInfo;	
	kPacket.m_vecPackageInfo = kPacket_.m_vecNXBTPackageInfo;

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << END_LOG;

        return;
    }

	//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_INQUIRY_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
#else
    KEventPtr spEvent( new KEvent );
    UidType anTrace[2] = { iUserUID, -1 };
    spEvent->SetData( PI_GS_PROXY, anTrace, EGS_PURCHASED_CASH_ITEM_LIST_ACK, kPacket );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
#endif SERV_EVENT_BINGO
	//}}

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

//{{ 2008. 6. 20  최육사  선물하기
IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_CHECK_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_CHECK_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	if( bSucceed  &&  kPacket_.m_byteOrderType == 2 )
	{
		if( kPacket_.m_ulTotalCount > 0 )
		{
			KEGS_NEW_PRESENT_CASH_INVENTORY_NOT kPacket;
			kPacket.m_iPresentCount = kPacket_.m_ulTotalCount;

			KEventPtr spEvent( new KEvent );
			UidType anTrace[2] = { iUserUID, -1 };
			spEvent->SetData( PI_GS_PROXY, anTrace, EGS_NEW_PRESENT_CASH_INVENTORY_NOT, kPacket );
			KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
		}
	}
	else
	{
		//START_LOG( cerr, L"이상한 캐쉬 인벤 체크 정보가 왔습니다." )
		//	<< BUILD_LOG( iUserUID )
		//	<< BUILD_LOG( kPacket_.m_ulResult )
		//	<< BUILD_LOG( kPacket_.m_byteOrderType )
		//	<< BUILD_LOG( kPacket_.m_ulTotalCount )
		//	<< END_LOG;
	}

	//LOG_SUCCESS( bSucceed )
	//    << BUILD_LOG( kPacket_.m_ulResult )
	//    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READABLE_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READABLE_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	if( bSucceed  &&  kPacket_.m_byteOrderType == 2 )
	{
		if( kPacket_.m_ulTotalCount > 0 )
		{
			KEGS_NEW_PRESENT_CASH_INVENTORY_NOT kPacket;
			kPacket.m_iPresentCount = kPacket_.m_ulTotalCount;

			KEventPtr spEvent( new KEvent );
			UidType anTrace[2] = { iUserUID, -1 };
			spEvent->SetData( PI_GS_PROXY, anTrace, EGS_NEW_PRESENT_CASH_INVENTORY_NOT, kPacket );
			KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
		}
	}
	else
	{
		//START_LOG( cerr, L"이상한 캐쉬 인벤 체크 정보가 왔습니다." )
		//	<< BUILD_LOG( iUserUID )
		//	<< BUILD_LOG( kPacket_.m_ulResult )
		//	<< BUILD_LOG( kPacket_.m_byteOrderType )
		//	<< BUILD_LOG( kPacket_.m_ulTotalCount )
		//	<< END_LOG;
	}

	//LOG_SUCCESS( bSucceed )
	//    << BUILD_LOG( kPacket_.m_ulResult )
	//    << END_LOG;
}
//}}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READ_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_READ_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	//if( iUserUID <= 0 )
	//{
	//	START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
	//		<< BUILD_LOG( kPacket_.m_ulPacketNo )
	//		<< BUILD_LOG( kPacket_.m_ulOrderNo )
	//		<< BUILD_LOG( kPacket_.m_ulProductNo )
	//		<< END_LOG;

	//	return;
	//}

	//KEventPtr spEvent( new KEvent );
	//spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ACK, kPacket_ );
	//KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	LOG_SUCCESS( bSucceed )
	    << BUILD_LOG( kPacket_.m_ulResult )
	    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << BUILD_LOG( kPacket_.m_ulOrderNo )
            << BUILD_LOG( kPacket_.m_ulProductNo )
            << END_LOG;

        return;
    }

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ACK, kPacket_ );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << BUILD_LOG( kPacket_.m_ulOrderNo )
            << BUILD_LOG( kPacket_.m_ulProductNo )
            << END_LOG;

        return;
    }

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK, kPacket_ );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    //LOG_SUCCESS( bSucceed )
    //    << BUILD_LOG( kPacket_.m_ulResult )
    //    << END_LOG;
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_SEND;
	kNot.m_iUserUID				= FIRST_SENDER_UID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

    SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

    boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
    spPacket->Write( kPacket_ );
    SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK )
{
    bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

    UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
    SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

    if( iUserUID <= 0 )
    {
        START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
            << BUILD_LOG( kPacket_.m_ulPacketNo )
            << BUILD_LOG( kPacket_.m_ulOrderNo )
            << BUILD_LOG( kPacket_.m_ulProductNo )
            << END_LOG;

        return;
    }

	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그	
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_RECV;
	kNot.m_iUserUID				= iUserUID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK, kPacket_ );
    KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

    LOG_SUCCESS( bSucceed )
        << BUILD_LOG( kPacket_.m_ulResult )
        << END_LOG;	
}

//{{ 2008. 6. 16  최육사  넥슨빌링 패키지 픽업
IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_SEND;
	kNot.m_iUserUID				= FIRST_SENDER_UID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );	
}

IMPL_ON_FUNC( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< BUILD_LOG( kPacket_.m_ulOrderNo )
			<< BUILD_LOG( kPacket_.m_ulPackageProductNo )
			<< BUILD_LOG( kPacket_.m_ulProductNo )
			<< END_LOG;

		return;
	}

	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그	
#ifdef SERV_BILLING_PACKET_LOG
	CTime kRegDate = CTime::GetCurrentTime();
	KE_LOCAL_LOG_BILLING_PACKET_NOT kNot;
	kNot.m_cLogType				= KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_RECV;
	kNot.m_iUserUID				= iUserUID;
	kNot.m_ulOrderNo			= kPacket_.m_ulOrderNo;
	kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
	kNot.m_usEventID			= ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK;
	KSIManager.QueueingEvent( E_LOCAL_LOG_BILLING_PACKET_NOT, kNot );
#endif SERV_BILLING_PACKET_LOG
	//}}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK, kPacket_ );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );

	LOG_SUCCESS( bSucceed )
		<< BUILD_LOG( kPacket_.m_ulResult )
		<< END_LOG;
}
//}}

//{{ 2010. 11. 17	최육사	넥슨 빌링 청약 철회
#ifdef SERV_NX_BILLING_REFUND
IMPL_ON_FUNC( ENX_BT_REFUND_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );    
}

IMPL_ON_FUNC( ENX_BT_REFUND_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 0x01 );

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	KEGS_CASH_ITEM_REFUND_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;
	case 0: // 시스템 오류
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_45;
		break;
	case 2: // 환불이 불가능한 상품 입니다.
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_53; 
		break;
	case 3: // 환불가능 개수보다 요청갯수가 큼
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_46;
		break;
	case 4: // 환불불가능 ( 구매한지 7일이 초과된 상품 )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_47;
		break;
	case 5: // 환불불가능 ( 패키지중 일부를 수령함 )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_48;
		break;
	case 6: // 환불불가능 ( 가격이 0원인 상품 )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_49;
		break;
	case 7: // 존재하지 않는 OrderNo&ProductNo
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_52;
		break;
	case 8: // 환불불가능 ( 선물구매 )
		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_54;
		break;
		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}

	default:
		START_LOG( cerr, L"정의되지 않은 청약철회 에러 코드입니다." )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_45;
		break;
	}

	//{{ 2008. 5. 13  최육사  GSUser에 한번 거쳐서 가도록 수정
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_USER, NULL, ENX_BT_REFUND_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
	//}}

	//LOG_SUCCESS( bSucceed )
	//    << BUILD_LOG( kPacket_.m_ulResult )
	//    << END_LOG;
}
#endif SERV_NX_BILLING_REFUND
//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-21	// 박세훈
IMPL_ON_FUNC( ENX_BT_NISMS_ITEM_COUPON_USING_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_ITEM_COUPON_USING_ACK )
{
	bool bSucceed = ( kPacket_.m_ulResult == 1 );

	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( clog, L"쿠폰 에러코드" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;
	}

	KEGS_USE_ITEM_COUPON_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;
	case 81:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_20;	// 쿠폰 총 사용 횟수를 초과하여 사용하실 수 없습니다.
		break;

	case 51:
	case 101:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_00;	// 이미 사용된 쿠폰입니다.
		break;

	case 52:
	case 54:
	case 65:
	case 102:
	case 103:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_01;	// 기간이 만료된 쿠폰입니다.
		break;

	case 61:		
	case 62:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
		break;

	case 100:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_08;	// PC방에서만 사용할 수 있는 쿠폰입니다.
		break;

		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_04;
		break;
	}

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { iUserUID, -1 };
	spEvent->SetData( PI_GS_PROXY, anTrace, EGS_USE_ITEM_COUPON_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK )
{
	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( clog, L"쿠폰 에러코드" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;
	}

	KEGS_DISCOUNT_COUPON_INQUIRY_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;

	case 51:
	case 101:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_00;	// 이미 사용된 쿠폰입니다.
		break;

	case 52:
	case 54:
	case 65:
	case 102:
	case 103:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_01;	// 기간이 만료된 쿠폰입니다.
		break;

	case 61:		
	case 62:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
		break;

	case 100:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_08;	// PC방에서만 사용할 수 있는 쿠폰입니다.
		break;

		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// 쿠폰 정보 읽기에 실패하였습니다.
		break;
	}

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	kPacket.m_vecDiscountCoupon	= kPacket_.m_vecDiscountCoupon;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { iUserUID, -1 };
	spEvent->SetData( PI_GS_PROXY, anTrace, EGS_DISCOUNT_COUPON_INQUIRY_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ )
{
	//{{ 2008. 8. 13  최육사	초기화 되지 않았다면 날리지 않는다.
	if( !SiKNexonBillingTCPManager()->IsInitialized() ) return;
	//}}

	SiKNexonBillingTCPManager()->InsertPacketNoUserUID( kPacket_.m_ulPacketNo, FIRST_SENDER_UID );

	boost::shared_ptr< KNexonBillingTCPPacket > spPacket( new KNexonBillingTCPPacket );
	spPacket->Write( kPacket_ );
	SiKNexonBillingTCPManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK )
{
	if( kPacket_.m_ulResult != 1 )
	{
		START_LOG( clog, L"쿠폰 에러코드" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;
	}

	KEGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK kPacket;
	switch( kPacket_.m_ulResult )
	{
	case 1:
		kPacket.m_iOK = NetError::NET_OK;
		break;

	case 51:
	case 101:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_00;	// 이미 사용된 쿠폰입니다.
		break;

	case 52:
	case 54:
	case 65:
	case 102:
	case 103:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_01;	// 기간이 만료된 쿠폰입니다.
		break;

	case 61:		
	case 62:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_02;
		break;

	case 100:
		kPacket.m_iOK = NetError::ERR_NX_COUPON_08;	// PC방에서만 사용할 수 있는 쿠폰입니다.
		break;

		//{{ 2012. 04. 02	김민성		넥슨 auth soap, 통합 맴버쉽
#ifdef SERV_NEXON_AUTH_SOAP
	case 1001300:
		kPacket.m_iOK = NetError::ERR_NX_AUTH_00;
		START_LOG( clog, L"넥슨 통합맴버쉽 권한 상향 요청" )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;
		break;
#endif SERV_NEXON_AUTH_SOAP
		//}}
	default:
		START_LOG( cerr, L"" )
			<< BUILD_LOG( kPacket_.m_ulResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_NX_COUPON_09;	// 쿠폰 정보 읽기에 실패하였습니다.
		break;
	}

	UidType iUserUID = SiKNexonBillingTCPManager()->GetCorrespondingUserUID( kPacket_.m_ulPacketNo );
	SiKNexonBillingTCPManager()->DeletePacketNoUserUID( kPacket_.m_ulPacketNo );

	if( iUserUID <= 0 )
	{
		START_LOG( cerr, L"패킷 번호에 해당하는 UserUID를 찾지 못함." )
			<< BUILD_LOG( kPacket_.m_ulPacketNo )
			<< END_LOG;

		return;
	}

	kPacket.m_vecDiscountCoupon	= kPacket_.m_vecDiscountCoupon;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { iUserUID, -1 };
	spEvent->SetData( PI_GS_PROXY, anTrace, EGS_DISCOUNT_COUPON_LIST_INQUIRY_ACK, kPacket );
	KActorManager::GetKObj()->QueueingTo( iUserUID, spEvent );
}
#endif // SERV_NEXON_COUPON_SYSTEM
