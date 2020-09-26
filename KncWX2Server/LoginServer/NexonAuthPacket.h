#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"


//{{ 2010. 06. 04  √÷¿∞ªÁ	≥ÿΩºPCπÊ ¿Œ¡ı º≠πˆ ∞≥∆Ì
#ifdef SERV_PCBANG_AUTH_NEW


#define MAX_PACKET_SIZE_OF_NEXON_AUTH			1024
#define MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH	MAX_PACKET_SIZE_OF_NEXON_AUTH - 4

//{{ 2011. 06. 22    ±ËπŒº∫    ¿Ã≈ª πÊ¡ˆ ∏µ® - NEXON ººº« Ω√Ω∫≈€ ºˆ¡§
#ifdef SERV_NEXON_SESSION_PROTOCOL
#define MAX_STRING_SIZE_OF_NEXON_AUTH			512
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

#define KNexonAuthPacketPtr KNexonAuthPacket*
#undef KNexonAuthPacketPtr
SmartPointer( KNexonAuthPacket );

class KNexonAuthPacket
{
public:
#   undef  _ENUM
#   define _ENUM( name, id ) name = id,
	enum ENUM_PACKET_TYPE { 
#       include "NexonAuthPacket_def.h" 
	};

	KNexonAuthPacket();
	virtual ~KNexonAuthPacket();

	int  GetTotalLength()				{ return ( int )m_usPacketLength + 3; }
	int  GetPacketType()				{ return ( int )m_bytePacketType; }

	bool ReadFromBuffer( IN BYTE* pbyteBuffer );
	bool WriteToBuffer( OUT BYTE* pbyteBuffer );

	bool ReadByte( BYTE& byteData, int& iCP );
	bool Read2Byte( unsigned short& usData, int& iCP );
	bool Read4Byte( unsigned long& ulData, int& iCP );
	bool Read8Byte( __int64& i64Data, int& iCP );
	bool ReadIP( unsigned int& uiData, int& iCP );
	bool ReadString( std::wstring& wstrData, int& iCP );

	bool WriteByte( BYTE byteData, int& iCP );
	bool Write2Byte( unsigned short usData, int& iCP );
	bool Write4Byte( unsigned long ulData, int& iCP );
	bool Write8Byte( __int64 i64Data, int& iCP );
	bool WriteIP( unsigned int uiData, int& iCP );
	bool WriteString( const std::wstring& wstrData, int& iCP );

	//////////////////////////////////////////////////////////////////////////
	// ∞‘¿”º≠πˆ ∆–≈∂ ∆ƒΩÃ

	bool Read( KENX_AUTH_INITIALIZE_REQ& kPacket );
	bool Write( const KENX_AUTH_INITIALIZE_REQ& kPacket );

	bool Read( KENX_AUTH_INITIALIZE_ACK& kPacket );
	bool Write( const KENX_AUTH_INITIALIZE_ACK& kPacket );

	bool Read( KENX_AUTH_LOGIN_REQ& kPacket );
	bool Write( const KENX_AUTH_LOGIN_REQ& kPacket );

	bool Read( KENX_AUTH_LOGIN_ACK& kPacket );
	bool Write( const KENX_AUTH_LOGIN_ACK& kPacket );

	bool Read( KENX_AUTH_LOGOUT_NOT& kPacket );
	bool Write( const KENX_AUTH_LOGOUT_NOT& kPacket );

	bool Read( KENX_AUTH_TERMINATE_NOT& kPacket );
	bool Write( const KENX_AUTH_TERMINATE_NOT& kPacket );

	bool Read( KENX_AUTH_MESSAGE_NOT& kPacket );
	bool Write( const KENX_AUTH_MESSAGE_NOT& kPacket );

	bool Read( KENX_AUTH_SYNCHRONIZE_REQ& kPacket );
	bool Write( const KENX_AUTH_SYNCHRONIZE_REQ& kPacket );

	bool Read( KENX_AUTH_SYNCHRONIZE_ACK& kPacket );
	bool Write( const KENX_AUTH_SYNCHRONIZE_ACK& kPacket );

	bool Read( KENX_AUTH_ALIVE_NOT& kPacket );
	bool Write( const KENX_AUTH_ALIVE_NOT& kPacket );

protected:
public:
protected:
	BYTE			m_byteHeader;
	unsigned short	m_usPacketLength;
	BYTE			m_bytePacketType;
	BYTE			m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH];
};


#endif SERV_PCBANG_AUTH_NEW
//}}

