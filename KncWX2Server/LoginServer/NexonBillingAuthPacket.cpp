#include "NexonBillingAuthPacket.h"

KNexonBillingAuthPacket::KNexonBillingAuthPacket()
{
	m_byteHeader = 0xAA;
	m_usPacketLength = 0;
	m_bytePacketType = 0;
}

KNexonBillingAuthPacket::~KNexonBillingAuthPacket()
{
}

bool KNexonBillingAuthPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	BYTE byteHeader;
	::memcpy( &byteHeader, pbyteBuffer, sizeof( BYTE ) );
	_JIF( byteHeader == 0xAA, return false );
	::memcpy( &m_usPacketLength, pbyteBuffer + 1, sizeof( unsigned short ) );
	m_usPacketLength = ::ntohs( m_usPacketLength );
	_JIF( m_usPacketLength >= 1, return false );
	_JIF( m_usPacketLength <= MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH + 1, return false );
	::memcpy( &m_bytePacketType, pbyteBuffer + 3, sizeof( BYTE ) );
	if( m_usPacketLength > 1 )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 4, m_usPacketLength - 1 );
	}

	return true;
}

bool KNexonBillingAuthPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

    _JIF( m_usPacketLength >= 1, return false );
    _JIF( m_usPacketLength <= MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH + 1, return false );
	::memcpy( pbyteBuffer, &m_byteHeader, sizeof( BYTE ) );
	unsigned short usPacketLength = ::htons( m_usPacketLength );
	::memcpy( pbyteBuffer + 1, &usPacketLength, sizeof( unsigned short ) );
	::memcpy( pbyteBuffer + 3, &m_bytePacketType, sizeof( BYTE ) );
	if( m_usPacketLength > 1 )
	{
		::memcpy( pbyteBuffer + 4, m_abytePacketContent, m_usPacketLength - 1 );
	}

	return true;
}

bool KNexonBillingAuthPacket::ReadByte( BYTE& byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 1 )
	{
		return false;
	}

	BYTE byteDataTemp;
	::memcpy( &byteDataTemp, m_abytePacketContent + iCP, sizeof( BYTE ) );
	byteData = byteDataTemp;
	iCP = iCP + 1;

	return true;
}

bool KNexonBillingAuthPacket::Read2Byte( unsigned short& usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 2 )
	{
		return false;
	}

	unsigned short usDataTemp;
	::memcpy( &usDataTemp, m_abytePacketContent + iCP, sizeof( unsigned short ) );
	usData = usDataTemp;
	usData = ::ntohs( usData );
	iCP = iCP + 2;
	return true;
}

bool KNexonBillingAuthPacket::Read4Byte( unsigned long& ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 4 )
	{
		return false;
	}

	unsigned long ulDataTemp;
	::memcpy( &ulDataTemp, m_abytePacketContent + iCP, sizeof( unsigned long ) );
	ulData = ulDataTemp;
	ulData = ::ntohl( ulData );
	iCP = iCP + 4;
	return true;
}

bool KNexonBillingAuthPacket::ReadIP( unsigned int& uiData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 4 )
	{
		return false;
	}

	unsigned int uiDataTemp;
	::memcpy( &uiDataTemp, m_abytePacketContent + iCP, sizeof( unsigned long ) );
	uiData = uiDataTemp;
	iCP = iCP + 4;
	return true;
}

bool KNexonBillingAuthPacket::ReadString( std::wstring& wstrData, int& iCP )
{
    BYTE byteLength;
    if( !ReadByte( byteLength, iCP ) )
    {
        return false;
    }

    if( byteLength == 0 )
    {
        wstrData.clear();
        return true;
    }

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - ( int )byteLength )
	{
		return false;
	}

	char szData[MAX_PATH];
	::memcpy( szData, m_abytePacketContent + iCP, ( size_t )byteLength );
	szData[byteLength] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + ( int )byteLength;
	return true;
}

bool KNexonBillingAuthPacket::WriteByte( BYTE byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 1 )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &byteData, sizeof( BYTE ) );
	iCP = iCP + 1;
	return true;
}

bool KNexonBillingAuthPacket::Write2Byte( unsigned short usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 2 )
	{
		return false;
	}

	usData = ::htons( usData );
	::memcpy( m_abytePacketContent + iCP, &usData, sizeof( unsigned short ) );
	iCP = iCP + 2;
	return true;
}

bool KNexonBillingAuthPacket::Write4Byte( unsigned long ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 4 )
	{
		return false;
	}

	ulData = ::htonl( ulData );
	::memcpy( m_abytePacketContent + iCP, &ulData, sizeof( unsigned long ) );
	iCP = iCP + 4;
	return true;
}

bool KNexonBillingAuthPacket::WriteIP( unsigned int uiData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - 4 )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &uiData, sizeof( unsigned long ) );
	iCP = iCP + 4;
	return true;
}

bool KNexonBillingAuthPacket::WriteString( std::wstring wstrData, int& iCP )
{
    std::string strData = KncUtil::toNarrowString( wstrData );
    BYTE byteLength = ( BYTE )strData.size();
    if( !WriteByte( byteLength, iCP ) )
    {
        return false;
    }

    if( byteLength == 0 )
    {
        return true;
    }

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_BILLING_AUTH - ( int )byteLength )
	{
		return false;
	}

    char szData[MAX_PATH];
#ifdef _CONVERT_VS_2010
	strcpy_s( szData, strData.c_str() );
#else
	strcpy( szData, strData.c_str() );
#endif _CONVERT_VS_2010

	::memcpy( m_abytePacketContent + iCP, szData, ( size_t )byteLength );
	iCP = iCP + ( int )byteLength;
	return true;
}

bool KNexonBillingAuthPacket::Read( KENX_INITIALIZE_BILLING_AUTH_NOT& kPacket )
{
	int iCP = 0;
    kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteGameSN, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteDomainSN, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrDomainName, iCP ), return false );
    LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonBillingAuthPacket::Read( KENX_USER_LOGIN_REQ& kPacket )
{
	int iCP = 0;
    kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteLogin, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUnitNickName, iCP ), return false );
	_JIF( ReadIP( kPacket.m_uiIP, iCP ), return false );
	_JIF( ReadIP( kPacket.m_uiIP, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteCanTry, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteGuestUser, iCP ), return false );
    LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonBillingAuthPacket::Read( KENX_USER_LOGIN_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUnitNickName, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteResult, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteAddressDesc, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteAccountDesc, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteOption, iCP ), return false );
	_JIF( Read4Byte( ( unsigned long& )kPacket.m_uiArgument, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteIsGameBangBillType, iCP ), return false );
    LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonBillingAuthPacket::Read( KENX_ALIVE_NOT& kPacket )
{
    kPacket.m_bytePacketType = m_bytePacketType;
    LIF( m_usPacketLength == 1 );

	return true;
}

bool KNexonBillingAuthPacket::Write( KENX_INITIALIZE_BILLING_AUTH_NOT& kPacket )
{
	int iCP = 0;
    m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteGameSN, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteDomainSN, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrDomainName, iCP ), return false );
    m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonBillingAuthPacket::Write( KENX_USER_LOGIN_REQ& kPacket )
{
	int iCP = 0;
    m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteLogin, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUnitNickName, iCP ), return false );
	_JIF( WriteIP( kPacket.m_uiIP, iCP ), return false );
	_JIF( WriteIP( kPacket.m_uiIP, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteCanTry, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteGuestUser, iCP ), return false );
    m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonBillingAuthPacket::Write( KENX_USER_LOGIN_ACK& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUnitNickName, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteResult, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteAddressDesc, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteAccountDesc, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteOption, iCP ), return false );
	_JIF( Write4Byte( ( unsigned long )kPacket.m_uiArgument, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteIsGameBangBillType, iCP ), return false );
    m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonBillingAuthPacket::Write( KENX_ALIVE_NOT& kPacket )
{
	m_bytePacketType = kPacket.m_bytePacketType;
    m_usPacketLength = 1;

	return true;
}