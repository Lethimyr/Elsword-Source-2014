//////////////////////////////////////////////////////////////////////////
//	PayletterBilllingPacket.h
//
//	Payletter 패킷 클래스
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"


#ifdef SERV_COUNTRY_US

#define MAX_PACKET_SIZE_PLB 13312
#define HEADER_IN_PACKET_LENGTH_PLB 10
#define HEADER_NOT_IN_PACKET_LENGTH_PLB 0
#define MAX_PACKET_CONTENT_SIZE_PLB MAX_PACKET_SIZE_PLB - ( HEADER_IN_PACKET_LENGTH_PLB + HEADER_NOT_IN_PACKET_LENGTH_PLB )

#define KPayletterBillingPacketPtr KPayletterBillingPacket*
#undef KPayletterBillingPacketPtr
SmartPointer( KPayletterBillingPacket );

class KPayletterBillingPacket
{
public:
#   undef  _ENUM
#   define _ENUM( name, id ) name = id,
	enum ENUM_PACKET_TYPE { 
#       include "PayletterBilling_def.h" 
	};

	KPayletterBillingPacket();
	virtual ~KPayletterBillingPacket();

	int  GetTotalLength()				{ return ( int )m_usReqLen /*+ HEADER_NOT_IN_PACKET_LENGTH_PLB*/; }
	int  GetPacketType()				{ return ( int )m_usReqType; }

	bool ReadFromBuffer( IN BYTE* pbyteBuffer );
	bool WriteToBuffer( OUT BYTE* pbyteBuffer );

	bool ReadByte( BYTE& byteData, int& iCP );
	bool Read2Byte( unsigned short& usData, int& iCP );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
	bool Read2ByteSigned( short& sData, int& iCP );
	//}}
	bool Read4Byte( unsigned long& ulData, int& iCP );
	bool Read8Byte( __int64& iData, int& iCP );
	bool ReadString( std::wstring& wstrData, int& iCP );
	bool ReadString( std::wstring& wstrData, int size, int& iCP );

	bool WriteByte( BYTE byteData, int& iCP );
	bool Write2Byte( unsigned short usData, int& iCP );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
	bool Write2ByteSigned( short sData, int& iCP );
	//}}
	bool Write4Byte( unsigned long ulData, int& iCP );
	bool Write8Byte( const __int64& iData, int& iCP );
	bool WriteString( const std::wstring& wstrData, int& iCP );

	// 숫자를 스트링으로~
	std::string toString(int iSource);

	bool Read( KEPL_BT_HEALTH_CHECK_ACK& kPacket );
	bool Write( const KEPL_BT_HEALTH_CHECK_REQ& kPacket );

	bool Read( KEPL_BT_BALANCE_CHECK_ACK& kPacket );
	bool Write( const KEPL_BT_BALANCE_CHECK_REQ& kPacket );

	// 아이템 구매 : 보낼때는 KEBILL_BUY_PRODUCT_REQ 에서 정보를 뽑아내서 보내고,
	//				 올때는 KEPL_BT_PURCHASEITEM_ACK 패킷으로 받는다. 이상한가?
	bool Read( KEPL_BT_PURCHASEITEM_ACK& kPacket );
	
	bool Write( const KEBILL_BUY_PRODUCT_REQ& kPacket );

	bool Read( KEPL_BT_REGCOUPON_ACK& kPacket );
	bool Write( const KEBILL_USE_COUPON_REQ& kPacket );

	// 선물하기 기능
	bool Read( KEPL_BT_GIFTITEM_ACK& kPacket );
	//bool Write( const KEPL_BT_GIFTITEM_REQ& kPacket );
	bool Write( const KEBILL_GIFT_ITEM_REQ& kPacket );

protected:
	
	unsigned short  m_usReqLen;
	unsigned short  m_usReqType;
	unsigned long   m_ulReqKey;
	unsigned short	m_usRetCode;
	BYTE			m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_PLB];
};


#endif //SERV_COUNTRY_US