#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ServerPacket.h"


#define MAX_PACKET_SIZE_OF_GIANT_AUTH			1024
#define MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH	MAX_PACKET_SIZE_OF_GIANT_AUTH - 4

//#define KGiantCommonPacketPtr KGiantCommonPacket*
//#undef KGiantCommonPacketPtr
SmartPointer( KGiantCommonPacket );

class KGiantCommonPacket
{
public:
	enum ENUM_COMMON_COMMAND_TYPE { 
		GCP_CCT_NULL					= 0,
		GCP_CCT_INITIALIZE				= 1,
	};

	enum ENUM_COMMON_PARACOMMAND_TYPE { 
		GCP_PCT_NULL_SERVER				= 0,
		GCP_PCT_NULL_CLIENT				= 1,
		GCP_PCT_INITIALIZE_REQ			= 1,
		GCP_PCT_INITIALIZE_ACK			= 2,
	};

	const static int ms_iHeaderSize;

public:
	KGiantCommonPacket();
	virtual ~KGiantCommonPacket();

	int  GetTotalLength()				{ return m_iPacketLength + sizeof(int); }
	int  GetCommand()					{ return ( int )m_byteCommand; }
	int  GetParaCommand()				{ return ( int )m_byteParaCommand; }

	bool ReadFromBuffer( IN BYTE* pbyteBuffer );
	bool WriteToBuffer( OUT BYTE* pbyteBuffer );

	bool ReadByte( BYTE& byteData, int& iCP );
	bool Read2Byte( short& sData, int& iCP );
	bool Read2Byte( unsigned short& usData, int& iCP );
	bool Read4Byte( unsigned long& ulData, int& iCP );
	bool Read4Byte( unsigned int& uiData, int& iCP );
	bool Read4Byte( int& iData, int& iCP );
	bool Read8Byte( __int64& i64Data, int& iCP );
	bool ReadString( std::wstring& wstrData, unsigned int iMaxLength, int& iCP );
	bool ReadString( wchar_t* pwstrData, unsigned int iMaxLength, int& iCP );

	bool WriteByte( BYTE byteData, int& iCP );
	bool Write2Byte( short sData, int& iCP );
	bool Write2Byte( unsigned short usData, int& iCP );
	bool Write4Byte( unsigned long ulData, int& iCP );
	bool Write8Byte( __int64 i64Data, int& iCP );
	bool WriteString( const std::wstring& wstrData, unsigned int iMaxLength, int& iCP );
	bool WriteString( const wchar_t* pwstrData, unsigned int iMaxLength, int& iCP );

public:
	virtual bool Read( KEGIANT_COMMON_INITIALIZE_REQ& kPacket );
	virtual bool Write( const KEGIANT_COMMON_INITIALIZE_REQ& kPacket );

	virtual bool Read( KEGIANT_COMMON_INITIALIZE_ACK& kPacket );
	virtual bool Write( const KEGIANT_COMMON_INITIALIZE_ACK& kPacket );

	virtual bool Read( KEGIANT_COMMON_NULL_SERVER& kPacket );
	virtual bool Write( const KEGIANT_COMMON_NULL_SERVER& kPacket );

	virtual bool Read( KEGIANT_COMMON_NULL_CLIENT& kPacket );
	virtual bool Write( const KEGIANT_COMMON_NULL_CLIENT& kPacket );

protected:
	// Header
	int				m_iPacketLength;
	BYTE			m_byteCommand;
	BYTE			m_byteParaCommand;
	// Body
	BYTE			m_abytePacketContent[MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH];
};