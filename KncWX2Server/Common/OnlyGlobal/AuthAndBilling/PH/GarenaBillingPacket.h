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


#ifdef SERV_COUNTRY_PH


//#define MAX_PACKET_SIZE_NGMT 13312 ( 13kb ) -- 최대 패킷 크기가 이것을 넘어섰다 ㅠ  ㅠ
#define MAX_PACKET_SIZE_NGMT 20480 // ( 20kb )
//#define MAX_PACKET_SIZE_NBT 8192 // ( 8kb ) -- 테스트
#define HEADER_SIZE_NGMT 4
#define MAX_PACKET_CONTENT_SIZE_NGMT MAX_PACKET_SIZE_NGMT - HEADER_SIZE_NGMT



#define KGarenaBillingPacketPtr KGarenaBillingPacket*
#undef KGarenaBillingPacketPtr
SmartPointer( KGarenaBillingPacket );

class KGarenaBillingPacket
{
public:
#   undef  _ENUM
#   define _ENUM( name, id ) name = id,
	enum ENUM_PACKET_TYPE { 
#       include "GarenaBilling_def.h" 
	};


	enum ERROR_CODE
	{
		EC_OK							= 0,	// 성공
		EC_NOT_EXIST_ACCOUNT			= 1,	// 존재 하지 않는 계정입니다.
		
		EC_INTERNAL_ERROR_HAPPENDED		= 99,	// DB 또는 네트워크 에러
	};

	KGarenaBillingPacket();
	virtual ~KGarenaBillingPacket();

	int  GetTotalLength()				{ return ( int )m_ulPacketLength + HEADER_SIZE_NGMT; }
	int  GetPacketType()				{ return ( int )m_iPacketType; }
	const std::string& GetStringBuffer() const	{ return m_strPacketContent; }

	bool ReadFromBuffer( IN const char*  pbyteBuffer );
	bool WriteToBuffer( OUT BYTE* pbyteBuffer );


	//////////////////////////////////////////////////////////////////////////	
	// 계정 확인
	bool Read( KEJSON_GN_CHECK_ACCOUNT_REQ& kPacket );
	bool Write( const KEJSON_GN_CHECK_ACCOUNT_ACK& kPacket );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////	
	// 캐쉬 전환
	bool Read( KEJSON_GN_CHANGE_GAME_CURRENCY_REQ& kPacket );
	bool Write( const KEJSON_GN_CHANGE_GAME_CURRENCY_ACK& kPacket );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////	
	// 전환 실패에 대한 피드백
	bool Read( KEJSON_GN_TRANSACTION_REQ& kPacket );
	bool Write( const KEJSON_GN_TRANSACTION_ACK& kPacket );
	//////////////////////////////////////////////////////////////////////////


protected:
	
	unsigned long   m_ulPacketLength;
	int				m_iPacketType;
	//BYTE			m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_NGMT];
	std::string		m_strPacketContent;
};


#endif //SERV_COUNTRY_PH