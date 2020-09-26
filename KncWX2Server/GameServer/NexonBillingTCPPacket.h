#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"

//#define MAX_PACKET_SIZE_NBT 13312 ( 13kb ) -- 최대 패킷 크기가 이것을 넘어섰다 ㅠ  ㅠ
#define MAX_PACKET_SIZE_NBT 20480 // ( 20kb )
//#define MAX_PACKET_SIZE_NBT 8192 // ( 8kb ) -- 테스트
#define HEADER_IN_PACKET_LENGTH_NBT 5
#define HEADER_NOT_IN_PACKET_LENGTH_NBT 5
#define MAX_PACKET_CONTENT_SIZE_NBT MAX_PACKET_SIZE_NBT - ( HEADER_IN_PACKET_LENGTH_NBT + HEADER_NOT_IN_PACKET_LENGTH_NBT )

#define KNexonBillingTCPPacketPtr KNexonBillingTCPPacket*
#undef KNexonBillingTCPPacketPtr
SmartPointer( KNexonBillingTCPPacket );

class KNexonBillingTCPPacket
{
public:
#   undef  _ENUM
#   define _ENUM( name, id ) name = id,

//#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
#   undef  _ENUM_PROCESS
#   define _ENUM_PROCESS( name, id, pname ) _ENUM( name, id )
//#endif // SERV_NEXON_COUPON_SYSTEM

	enum ENUM_PACKET_TYPE { 
#       include "NexonBillingTCP_def.h" 
	};

	KNexonBillingTCPPacket();
	virtual ~KNexonBillingTCPPacket();

	int  GetTotalLength()				{ return ( int )m_ulPacketLength + HEADER_NOT_IN_PACKET_LENGTH_NBT; }
	int  GetPacketType()				{ return ( int )m_bytePacketType; }
	//{{ 2013. 01. 08	최육사	빌링 패킷 넘치는 유저 찾기
#ifdef SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
	static const wchar_t*   GetPacketTypeStr( BYTE bytePacketType );
#endif SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
	//}}

	bool ReadFromBuffer( IN BYTE* pbyteBuffer );
	bool WriteToBuffer( OUT BYTE* pbyteBuffer );

	bool ReadByte( BYTE& byteData, int& iCP );
	bool Read2Byte( unsigned short& usData, int& iCP );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
	bool Read2ByteSigned( short& sData, int& iCP );
	//}}
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-21	// 박세훈
	bool Read4Byte( int& iData, int& iCP );
#endif // SERV_NEXON_COUPON_SYSTEM
	bool Read4Byte( unsigned long& ulData, int& iCP );
    bool Read8Byte( __int64& iData, int& iCP );
	bool ReadString( std::wstring& wstrData, int& iCP );

	bool WriteByte( BYTE byteData, int& iCP );
	bool Write2Byte( unsigned short usData, int& iCP );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
	bool Write2ByteSigned( short sData, int& iCP );
	//}}
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-21	// 박세훈
	bool Write4Byte( int& iData, int& iCP );
#endif // SERV_NEXON_COUPON_SYSTEM
	bool Write4Byte( unsigned long ulData, int& iCP );
    bool Write8Byte( const __int64& iData, int& iCP );
	bool WriteString( const std::wstring& wstrData, int& iCP );
	
	bool Read( KNXBTCategoryInfo& kNXBTCategoryInfo, int& iCP );
	bool Write( const KNXBTCategoryInfo& kNXBTCategoryInfo, int& iCP );

    bool Read( KNXBTBonusProductInfo& kNXBTBonusProductInfo, int& iCP );
    bool Write( const KNXBTBonusProductInfo& kNXBTBonusProductInfo, int& iCP );

	//{{ 2008. 7. 10  최육사  빌링패킷 수정
	bool Read( KNXBTBonusProductInfoPickUp& kNXBTBonusProductInfoPickUp, int& iCP );
	bool Write( const KNXBTBonusProductInfoPickUp& kNXBTBonusProductInfoPickUp, int& iCP );
	//}}

	bool Read( KNXBTProductInfo& kNXBTProductInfo, int& iCP );
	bool Write( const KNXBTProductInfo& kNXBTProductInfo, int& iCP );

	bool Read( KNXBTPurchaseReqInfo& kNXBTPurchaseReqInfo, int& iCP );
	bool Write( const KNXBTPurchaseReqInfo& kNXBTPurchaseReqInfo, int& iCP );

	bool Read( KNXBTPurchaseAckInfo& kNXBTPurchaseAckInfo, int& iCP );
	bool Write( const KNXBTPurchaseAckInfo& kNXBTPurchaseAckInfo, int& iCP );

	bool Read( KNXBTOrderInfo& kNXBTOrderInfo, int& iCP );
	bool Write( const KNXBTOrderInfo& kNXBTOrderInfo, int& iCP );

	bool Read( KNXBTPackageInfo& kNXBTPackageInfo, int& iCP );
	bool Write( const KNXBTPackageInfo& kNXBTPackageInfo, int& iCP );

	bool Read( KNXBTLotteryInfo& kNXBTLotteryInfo, int& iCP );
	bool Write( const KNXBTLotteryInfo& kNXBTLotteryInfo, int& iCP );	

    bool Read( KNXBTCouponProductInfo& kNXBTCouponProductInfo, int& iCP );
    bool Write( const KNXBTCouponProductInfo& kNXBTCouponProductInfo, int& iCP );

    bool Read( KNXBTSubProductInfo& kNXBTSubProductInfo, int& iCP );
    bool Write( const KNXBTSubProductInfo& kNXBTSubProductInfo, int& iCP );

	bool Read( KENX_BT_INITIALIZE_REQ& kPacket );
    bool Write( const KENX_BT_INITIALIZE_REQ& kPacket );

    bool Read( KENX_BT_INITIALIZE_ACK& kPacket );
    bool Write( const KENX_BT_INITIALIZE_ACK& kPacket );

    bool Read( KENX_BT_HEART_BEAT_REQ& kPacket );
    bool Write( const KENX_BT_HEART_BEAT_REQ& kPacket );

    bool Read( KENX_BT_HEART_BEAT_ACK& kPacket );
    bool Write( const KENX_BT_HEART_BEAT_ACK& kPacket );
	
	bool Read( KENX_BT_CATEGORY_INQUIRY_REQ& kPacket );
	bool Write( const KENX_BT_CATEGORY_INQUIRY_REQ& kPacket );

	bool Read( KENX_BT_CATEGORY_INQUIRY_ACK& kPacket );
	bool Write( const KENX_BT_CATEGORY_INQUIRY_ACK& kPacket );

    bool Read( KENX_BT_PRODUCT_INQUIRY_XML_REQ& kPacket );
    bool Write( const KENX_BT_PRODUCT_INQUIRY_XML_REQ& kPacket );

    bool Read( KENX_BT_PRODUCT_INQUIRY_XML_ACK& kPacket );
    bool Write( const KENX_BT_PRODUCT_INQUIRY_XML_ACK& kPacket );

	bool Read( KENX_BT_PRODUCT_INQUIRY_REQ& kPacket );
	bool Write( const KENX_BT_PRODUCT_INQUIRY_REQ& kPacket );

	bool Read( KENX_BT_PRODUCT_INQUIRY_ACK& kPacket );
	bool Write( const KENX_BT_PRODUCT_INQUIRY_ACK& kPacket );

	bool Read( KENX_BT_CHECK_BALANCE_REQ& kPacket );
	bool Write( const KENX_BT_CHECK_BALANCE_REQ& kPacket );

	bool Read( KENX_BT_CHECK_BALANCE_ACK& kPacket );
	bool Write( const KENX_BT_CHECK_BALANCE_ACK& kPacket );

	bool Read( KENX_BT_PURCHASE_ITEM_REQ& kPacket );
	bool Write( const KENX_BT_PURCHASE_ITEM_REQ& kPacket );

	bool Read( KENX_BT_PURCHASE_ITEM_ACK& kPacket );
	bool Write( const KENX_BT_PURCHASE_ITEM_ACK& kPacket );

	bool Read( KENX_BT_PURCHASE_GIFT_REQ& kPacket );
	bool Write( const KENX_BT_PURCHASE_GIFT_REQ& kPacket );

	bool Read( KENX_BT_PURCHASE_GIFT_ACK& kPacket );
	bool Write( const KENX_BT_PURCHASE_GIFT_ACK& kPacket );

	//{{ 2009. 10. 14  최육사	자동결제
	bool Read( KENX_BT_CHECK_AUTO_PAYMENT_USER_REQ& kPacket );
	bool Write( const KENX_BT_CHECK_AUTO_PAYMENT_USER_REQ& kPacket );

	bool Read( KENX_BT_CHECK_AUTO_PAYMENT_USER_ACK& kPacket );
	bool Write( const KENX_BT_CHECK_AUTO_PAYMENT_USER_ACK& kPacket );
	//}}

	bool Read( KENX_BT_COUPON_INQUIRY_REQ& kPacket );
	bool Write( const KENX_BT_COUPON_INQUIRY_REQ& kPacket );

	bool Read( KENX_BT_COUPON_INQUIRY_ACK& kPacket );
	bool Write( const KENX_BT_COUPON_INQUIRY_ACK& kPacket );

	//{{ 2009. 3. 24  최육사	쿠폰사용여부 조회
	bool Read( KENX_BT_COUPON_INQUIRY_NEW_REQ& kPacket );
	bool Write( const KENX_BT_COUPON_INQUIRY_NEW_REQ& kPacket );

	bool Read( KENX_BT_COUPON_INQUIRY_NEW_ACK& kPacket );
	bool Write( const KENX_BT_COUPON_INQUIRY_NEW_ACK& kPacket );
	//}}

	bool Read( KENX_BT_COUPON_USING_REQ& kPacket );
	bool Write( const KENX_BT_COUPON_USING_REQ& kPacket );

	bool Read( KENX_BT_COUPON_USING_ACK& kPacket );
	bool Write( const KENX_BT_COUPON_USING_ACK& kPacket );

	//{{ 2008. 6. 20  최육사  선물하기
	bool Read( KENX_BT_NISMS_INVENTORY_CHECK_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_CHECK_REQ& kPacket );

	bool Read( KENX_BT_NISMS_INVENTORY_CHECK_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_CHECK_ACK& kPacket );
	//}}

	bool Read( KENX_BT_NISMS_INVENTORY_INQUIRY_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_INQUIRY_REQ& kPacket );

	bool Read( KENX_BT_NISMS_INVENTORY_INQUIRY_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_INQUIRY_ACK& kPacket );

	//{{ 2008. 6. 26  최육사  선물하기
	bool Read( KENX_BT_NISMS_INVENTORY_READABLE_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_READABLE_REQ& kPacket );

	bool Read( KENX_BT_NISMS_INVENTORY_READABLE_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_READABLE_ACK& kPacket );
	//}}

	bool Read( KENX_BT_NISMS_INVENTORY_READ_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_READ_REQ& kPacket );

	bool Read( KENX_BT_NISMS_INVENTORY_READ_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_READ_ACK& kPacket );

	bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_REQ& kPacket );

	bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ACK& kPacket );	

    bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ& kPacket );
    bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ& kPacket );

    bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK& kPacket );
    bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK& kPacket );

    bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ& kPacket );
    bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ& kPacket );

    bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK& kPacket );
    bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK& kPacket );

	//{{ 2008. 6. 16  최육사  넥슨 빌링 패키지 픽업
	bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ& kPacket );

	bool Read( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK& kPacket );
	//}}

	//{{ 2010. 11. 17	최육사	넥슨 빌링 청약 철회
#ifdef SERV_NX_BILLING_REFUND
	bool Read( KENX_BT_REFUND_REQ& kPacket );
	bool Write( const KENX_BT_REFUND_REQ& kPacket );

	bool Read( KENX_BT_REFUND_ACK& kPacket );
	bool Write( const KENX_BT_REFUND_ACK& kPacket );
#endif SERV_NX_BILLING_REFUND
	//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-21	// 박세훈
	bool Read( KDiscountCoupon& kDiscountCoupon, int& iCP );
	bool Write( const KDiscountCoupon& kDiscountCoupon, int& iCP );

	bool Read( KDiscountCouponInquriyInfo& kDiscountCouponInquriyInfo, int& iCP );
	bool Write( const KDiscountCouponInquriyInfo& kDiscountCouponInquriyInfo, int& iCP );

	bool Read( KENX_BT_NISMS_ITEM_COUPON_USING_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_ITEM_COUPON_USING_REQ& kPacket );

	bool Read( KENX_BT_NISMS_ITEM_COUPON_USING_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_ITEM_COUPON_USING_ACK& kPacket );

	bool Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ& kPacket );

	bool Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK& kPacket );

	bool Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ& kPacket );
	bool Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ& kPacket );

	bool Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK& kPacket );
	bool Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK& kPacket );
#endif // SERV_NEXON_COUPON_SYSTEM

protected:
	BYTE			m_byteReservedChar;
	unsigned long   m_ulPacketLength;
	unsigned long   m_ulPacketNo;
    BYTE            m_bytePacketType;
	BYTE			m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_NBT];
};
