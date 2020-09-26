#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"

#define MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH 1024
#define MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH - 4

SmartPointer( KNexonBillingAuthPacket );

class KNexonBillingAuthPacket
{
public:
#   undef  _ENUM
#   define _ENUM( name, id ) name = id,
	enum ENUM_PACKET_TYPE { 
#       include "NexonBillingAuthPacket_def.h" 
	};

	KNexonBillingAuthPacket();
	virtual ~KNexonBillingAuthPacket();

	int  GetTotalLength()				{ return ( int )m_usPacketLength + 3; }
	int  GetPacketType()				{ return ( int )m_bytePacketType; }

	bool ReadFromBuffer( IN BYTE* pbyteBuffer );
	bool WriteToBuffer( OUT BYTE* pbyteBuffer );

	bool ReadByte( BYTE& byteData, int& iCP );
	bool Read2Byte( unsigned short& usData, int& iCP );
	bool Read4Byte( unsigned long& ulData, int& iCP );
	bool ReadIP( unsigned int& uiData, int& iCP );
	bool ReadString( std::wstring& wstrData, int& iCP );

	bool WriteByte( BYTE byteData, int& iCP );
	bool Write2Byte( unsigned short usData, int& iCP );
	bool Write4Byte( unsigned long ulData, int& iCP );
	bool WriteIP( unsigned int uiData, int& iCP );
	bool WriteString( std::wstring wstrData, int& iCP );

	bool Read( KENX_INITIALIZE_BILLING_AUTH_NOT& kPacket );
	bool Read( KENX_USER_LOGIN_REQ& kPacket );
	bool Read( KENX_USER_LOGIN_ACK& kPacket );
	bool Read( KENX_ALIVE_NOT& kPacket );

	bool Write( KENX_INITIALIZE_BILLING_AUTH_NOT& kPacket );
	bool Write( KENX_USER_LOGIN_REQ& kPacket );
	bool Write( KENX_USER_LOGIN_ACK& kPacket );
	bool Write( KENX_ALIVE_NOT& kPacket );

protected:
public:
protected:
	BYTE			m_byteHeader;
	unsigned short	m_usPacketLength;
	BYTE			m_bytePacketType;
	BYTE			m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH];
};
