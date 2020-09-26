#include "GiantCommonPacket.h"


//////////////////////////////////////////////////////////////////////////

const int KGiantCommonPacket::ms_iHeaderSize( 1 + 1 );	// CommandLength는 포함되지 않는다


KGiantCommonPacket::KGiantCommonPacket()
{
	m_iPacketLength = 0;
	m_byteCommand = 0;
	m_byteParaCommand = 0;
}

KGiantCommonPacket::~KGiantCommonPacket()
{
}

bool KGiantCommonPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	::memcpy( &m_iPacketLength, pbyteBuffer, sizeof( int ) );
	_JIF( m_iPacketLength >= ms_iHeaderSize, return false );
	_JIF( m_iPacketLength <= MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - sizeof(m_iPacketLength), return false );
	::memcpy( &m_byteCommand, pbyteBuffer + 4, sizeof( BYTE ) );
	::memcpy( &m_byteParaCommand, pbyteBuffer + 5, sizeof( BYTE ) );
	if( m_iPacketLength > ms_iHeaderSize )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 6, m_iPacketLength - ms_iHeaderSize );
	}

	return true;
}

bool KGiantCommonPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	_JIF( m_iPacketLength >= ms_iHeaderSize, return false );
	_JIF( m_iPacketLength <= MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - sizeof(m_iPacketLength), return false );
	//	int iPacketLength = ::htonl( m_iPacketLength );
	::memcpy( pbyteBuffer, &m_iPacketLength, sizeof( int ) );
	::memcpy( pbyteBuffer + 4, &m_byteCommand, sizeof( BYTE ) );
	::memcpy( pbyteBuffer + 5, &m_byteParaCommand, sizeof( BYTE ) );
	if( m_iPacketLength > ms_iHeaderSize )
	{
		::memcpy( pbyteBuffer + 6, m_abytePacketContent, m_iPacketLength - ms_iHeaderSize );
	}

	return true;
}

bool KGiantCommonPacket::ReadByte( BYTE& byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 1 )
	{
		return false;
	}

	::memcpy( &byteData, m_abytePacketContent + iCP, sizeof( BYTE ) );
	iCP = iCP + 1;

	return true;
}

bool KGiantCommonPacket::Read2Byte( short& sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 2 )
	{
		return false;
	}

	::memcpy( &sData, m_abytePacketContent + iCP, sizeof( short ) );
	iCP = iCP + 2;
	return true;
}

bool KGiantCommonPacket::Read2Byte( unsigned short& usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 2 )
	{
		return false;
	}

	::memcpy( &usData, m_abytePacketContent + iCP, sizeof( unsigned short ) );
	iCP = iCP + 2;
	return true;
}

bool KGiantCommonPacket::Read4Byte( unsigned long& ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 4 )
	{
		return false;
	}

	::memcpy( &ulData, m_abytePacketContent + iCP, sizeof( unsigned long ) );
	iCP = iCP + 4;
	return true;
}

bool KGiantCommonPacket::Read4Byte( unsigned int& uiData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 4 )
	{
		return false;
	}

	::memcpy( &uiData, m_abytePacketContent + iCP, sizeof( unsigned int ) );
	iCP = iCP + 4;
	return true;
}

bool KGiantCommonPacket::Read4Byte( int& iData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 4 )
	{
		return false;
	}

	::memcpy( &iData, m_abytePacketContent + iCP, sizeof( unsigned int ) );
	iCP = iCP + 4;
	return true;
}

bool KGiantCommonPacket::Read8Byte( __int64& i64Data, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 8 )
	{
		return false;
	}

	::memcpy( &i64Data, m_abytePacketContent + iCP, sizeof( __int64 ) );
	iCP = iCP + 8;
	return true;    
}

bool KGiantCommonPacket::ReadString( std::wstring& wstrData, unsigned int iMaxLength, int& iCP )
{
	if( iMaxLength == 0 )
	{
		wstrData.clear();
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - static_cast<signed>(iMaxLength) )
	{
		return false;
	}

	wchar_t szData[MAX_PATH] = L"";
	MultiByteToWideChar( 936, 0, reinterpret_cast<char*>(m_abytePacketContent + iCP), iMaxLength, szData, MAX_PATH );
	wstrData = szData;
	iCP = iCP + iMaxLength;
	return true;
}

bool KGiantCommonPacket::ReadString( wchar_t* pwstrData, unsigned int iMaxLength, int& iCP )
{
	if( iMaxLength == 0 )
	{
		//pwstrData[0] = '\0';
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - static_cast<signed>(iMaxLength) )
	{
		return false;
	}

	wchar_t szData[MAX_PATH] = L"";
	MultiByteToWideChar( 936, 0, reinterpret_cast<char*>(m_abytePacketContent + iCP), iMaxLength, szData, MAX_PATH );
	StrCpyNW(pwstrData, szData, MAX_PATH);
	iCP = iCP + iMaxLength;
	return true;
}

bool KGiantCommonPacket::WriteByte( BYTE byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 1 )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &byteData, sizeof( BYTE ) );
	iCP = iCP + 1;
	return true;
}

bool KGiantCommonPacket::Write2Byte( short sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 2 )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &sData, sizeof( short ) );
	iCP = iCP + 2;
	return true;
}

bool KGiantCommonPacket::Write2Byte( unsigned short usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 2 )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &usData, sizeof( unsigned short ) );
	iCP = iCP + 2;
	return true;
}

bool KGiantCommonPacket::Write4Byte( unsigned long ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 4 )
	{
		return false;
	}

	//	ulData = ::htonl( ulData );
	::memcpy( m_abytePacketContent + iCP, &ulData, sizeof( unsigned long ) );
	iCP = iCP + 4;
	return true;
}

bool KGiantCommonPacket::Write8Byte( __int64 i64Data, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - 8 )
	{
		return false;
	}

	//	i64Data = htonll( i64Data );
	::memcpy( m_abytePacketContent + iCP, &i64Data, sizeof( __int64 ) );
	iCP = iCP + 8;
	return true;
}

bool KGiantCommonPacket::WriteString( const std::wstring& wstrData, unsigned int iMaxLength, int& iCP )
{
	_JIF( iMaxLength <= MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH, return false );

	if( iMaxLength == 0 )
	{
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - static_cast<signed>(iMaxLength) )
	{
		return false;
	}

	ZeroMemory(m_abytePacketContent + iCP, iMaxLength);
	WideCharToMultiByte( 936, 0, wstrData.c_str(), -1, reinterpret_cast<char*>(m_abytePacketContent + iCP), iMaxLength, NULL, NULL ); 
	iCP = iCP + iMaxLength;
	return true;
}

bool KGiantCommonPacket::WriteString( const wchar_t* pwstrData, unsigned int iMaxLength, int& iCP )
{
	_JIF( iMaxLength <= MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH, return false );

	if( iMaxLength == 0 )
	{
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_GIANT_AUTH - static_cast<signed>(iMaxLength) )
	{
		return false;
	}

	ZeroMemory(m_abytePacketContent + iCP, iMaxLength);
	WideCharToMultiByte( 936, 0, pwstrData, -1, reinterpret_cast<char*>(m_abytePacketContent + iCP), iMaxLength, NULL, NULL ); 
	iCP = iCP + iMaxLength;
	return true;
}

bool KGiantCommonPacket::Read( KEGIANT_COMMON_INITIALIZE_REQ& kPacket )
{
	int iCP = 0;
	_JIF( ReadString( kPacket.m_wstrIP, 16, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usPort, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantCommonPacket::Write( const KEGIANT_COMMON_INITIALIZE_REQ& kPacket )
{
	m_byteCommand = GCP_CCT_INITIALIZE;
	m_byteParaCommand = GCP_PCT_INITIALIZE_REQ;

	int iCP = 0;
	_JIF( WriteString( kPacket.m_wstrIP, 16, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usPort, iCP ), return false );
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantCommonPacket::Read( KEGIANT_COMMON_INITIALIZE_ACK& kPacket )
{
	int iCP = 0;
	_JIF( Read2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrZoneName, 32, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteNetType, iCP ), return false );
	LIF( iCP + ms_iHeaderSize == m_iPacketLength );

	return true;
}

bool KGiantCommonPacket::Write( const KEGIANT_COMMON_INITIALIZE_ACK& kPacket )
{
	m_byteCommand = GCP_CCT_INITIALIZE;
	m_byteParaCommand = GCP_PCT_INITIALIZE_ACK;

	int iCP = 0;
	_JIF( Write2Byte( kPacket.m_usZone, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usGame, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrZoneName, 32, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteNetType, iCP ), return false );	
	m_iPacketLength = iCP + ms_iHeaderSize;

	return true;
}

bool KGiantCommonPacket::Read( KEGIANT_COMMON_NULL_SERVER& kPacket )
{
	return true;
}

bool KGiantCommonPacket::Write( const KEGIANT_COMMON_NULL_SERVER& kPacket )
{
	m_byteCommand = GCP_CCT_NULL;
	m_byteParaCommand = GCP_PCT_NULL_SERVER;

	m_iPacketLength = ms_iHeaderSize;

	return true;
}

bool KGiantCommonPacket::Read( KEGIANT_COMMON_NULL_CLIENT& kPacket )
{
	return true;
}

bool KGiantCommonPacket::Write( const KEGIANT_COMMON_NULL_CLIENT& kPacket )
{
	m_byteCommand = GCP_CCT_NULL;
	m_byteParaCommand = GCP_PCT_NULL_CLIENT;

	m_iPacketLength = ms_iHeaderSize;

	return true;
}
