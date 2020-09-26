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


#ifdef SERV_COUNTRY_BR

#define MAX_PACKET_SIZE_PLB 13312
#define HEADER_IN_PACKET_LENGTH_PLB 6
#define HEADER_NOT_IN_PACKET_LENGTH_PLB 0
#define MAX_PACKET_CONTENT_SIZE_PLB MAX_PACKET_SIZE_PLB - ( HEADER_IN_PACKET_LENGTH_PLB + HEADER_NOT_IN_PACKET_LENGTH_PLB )

#define KBRPayletterBillingPacketPtr KBRPayletterBillingPacket*
#undef KBRPayletterBillingPacketPtr
SmartPointer( KBRPayletterBillingPacket );

class KBRPayletterBillingPacket
{
public:
#   undef  _ENUM
#   define _ENUM( name, id ) name = id,
	enum ENUM_PACKET_TYPE { 
#       include "BRPayletterBilling_def.h" 
	};

	KBRPayletterBillingPacket();
	virtual ~KBRPayletterBillingPacket();

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
	bool ReadString( char* cpData, int size, int& iCP );

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

	bool Read( KEPL_BR_BALANCE_CHECK_ACK& kPacket );
	bool Write( const KEPL_BR_BALANCE_CHECK_REQ& kPacket );

	// 아이템 구매 : 보낼때는 KEBILL_BUY_PRODUCT_REQ 에서 정보를 뽑아내서 보내고,
	//				 올때는 KEPL_BR_PURCHASEITEM_ACK 패킷으로 받는다. 이상한가?
	bool Read( KEPL_BR_PURCHASEITEM_ACK& kPacket );
	bool Write( const KEBILL_BUY_PRODUCT_REQ& kPacket );

protected:
	
	unsigned short  m_usReqLen;
	unsigned short  m_usReqType;
	unsigned short	m_usRetCode;
	BYTE			m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_PLB];
};


#endif //SERV_COUNTRY_BR