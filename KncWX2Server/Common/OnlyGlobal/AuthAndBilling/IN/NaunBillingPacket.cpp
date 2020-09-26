#include "NaunBillingPacket.h"

#ifdef SERV_COUNTRY_IN
KNaunBillingPacket::KNaunBillingPacket()
{
	m_usReqLen = 0;
}

KNaunBillingPacket::~KNaunBillingPacket()
{
}

bool KNaunBillingPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	_JIF( m_usReqLen >= HEADER_IN_PACKET_LENGTH_NUBT, return false );
	_JIF( m_usReqLen - HEADER_IN_PACKET_LENGTH_NUBT <= MAX_PACKET_CONTENT_SIZE_NUBT, return false );
	::memcpy( &m_usReqLen, pbyteBuffer,  sizeof( unsigned short ) );

	if( m_usReqLen > HEADER_IN_PACKET_LENGTH_NUBT )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 2, m_usReqLen - HEADER_IN_PACKET_LENGTH_NUBT );
	}

	return true;
}

bool KNaunBillingPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	_JIF( m_usReqLen - HEADER_IN_PACKET_LENGTH_NUBT <= MAX_PACKET_CONTENT_SIZE_NUBT, return false );
	::memcpy( pbyteBuffer, &m_usReqLen, sizeof( unsigned short ) );

	if( m_usReqLen > HEADER_IN_PACKET_LENGTH_NUBT )
	{
		::memcpy( pbyteBuffer + 2, m_abytePacketContent, m_usReqLen - HEADER_IN_PACKET_LENGTH_NUBT );
	}

	return true;
}

bool KNaunBillingPacket::ReadByte( BYTE& byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( BYTE ) )
	{
		return false;
	}

	BYTE byteDataTemp;
	::memcpy( &byteDataTemp, m_abytePacketContent + iCP, sizeof( BYTE ) );
	byteData = byteDataTemp;
	iCP = iCP + ( int )sizeof( BYTE );

	return true;
}

bool KNaunBillingPacket::Read2Byte( unsigned short& usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( unsigned short ) )
	{
		return false;
	}

	unsigned short usDataTemp;
	::memcpy( &usDataTemp, m_abytePacketContent + iCP, sizeof( unsigned short ) );
	usData = usDataTemp;
	//usData = ::ntohs( usData );
	iCP = iCP + ( int )sizeof( unsigned short );
	return true;
}

//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
bool KNaunBillingPacket::Read2ByteSigned( short& sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( short ) )
	{
		return false;
	}

	short sDataTemp;
	::memcpy( &sDataTemp, m_abytePacketContent + iCP, sizeof( short ) );
	sData = sDataTemp;
	//sData = ::ntohs( sData );
	iCP = iCP + ( int )sizeof( short );
	return true;
}
//}}

bool KNaunBillingPacket::Read4Byte( unsigned long& ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( unsigned long ) )
	{
		return false;
	}

	unsigned long ulDataTemp;
	::memcpy( &ulDataTemp, m_abytePacketContent + iCP, sizeof( unsigned long ) );
	ulData = ulDataTemp;
	//ulData = ::ntohl( ulData );
	iCP = iCP + ( int )sizeof( unsigned long );
	return true;
}

bool KNaunBillingPacket::Read8Byte( __int64& iData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( __int64 ) )
	{
		return false;
	}

	__int64 iDataTemp;
	::memcpy( &iDataTemp, m_abytePacketContent + iCP, sizeof( __int64 ) );

	unsigned long aulDataTemp[2];
	::memcpy( aulDataTemp, &iDataTemp, sizeof( __int64 ) );

	//aulDataTemp[0] = ::ntohl( aulDataTemp[0] );
	//aulDataTemp[1] = ::ntohl( aulDataTemp[1] );

	unsigned long ulTemp = aulDataTemp[0];
	aulDataTemp[0] = aulDataTemp[1];
	aulDataTemp[1] = ulTemp;

	::memcpy( &iDataTemp, aulDataTemp, sizeof( __int64 ) );

	iData = iDataTemp;
	iCP = iCP + ( int )sizeof( __int64 );
	return true;
}

bool KNaunBillingPacket::ReadString( std::wstring& wstrData, int& iCP )
{
	unsigned short usLength;
	if( !Read2Byte( usLength, iCP ) )
	{
		return false;
	}

	if( usLength == 0 )
	{
		wstrData.clear();
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )usLength )
	{
		return false;
	}

	char szData[MAX_PATH];
	::memcpy( szData, m_abytePacketContent + iCP, ( size_t )usLength );
	szData[usLength] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + ( int )usLength;
	return true;
}

bool KNaunBillingPacket::ReadString( std::wstring& wstrData, int iFixedLength, int& iCP )
{
	if( iFixedLength == 0 )
	{
		wstrData.clear();
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - iFixedLength )
	{
		return false;
	}

	char szData[MAX_PATH];
	::memcpy( szData, m_abytePacketContent + iCP, static_cast<size_t>(iFixedLength) );
	szData[iFixedLength] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + iFixedLength - 1;
	return true;
}

bool KNaunBillingPacket::WriteByte( BYTE byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( BYTE ) )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &byteData, sizeof( BYTE ) );
	iCP = iCP + ( int )sizeof( BYTE );
	return true;
}

bool KNaunBillingPacket::Write2Byte( unsigned short usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( unsigned short ) )
	{
		return false;
	}

	//usData = ::htons( usData );
	::memcpy( m_abytePacketContent + iCP, &usData, sizeof( unsigned short ) );
	iCP = iCP + ( int )sizeof( unsigned short );
	return true;
}

//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
bool KNaunBillingPacket::Write2ByteSigned( short sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( short ) )
	{
		return false;
	}

	//sData = ::htons( sData );
	::memcpy( m_abytePacketContent + iCP, &sData, sizeof( short ) );
	iCP = iCP + ( int )sizeof( short );
	return true;
}
//}}

bool KNaunBillingPacket::Write4Byte( unsigned long ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( unsigned long ) )
	{
		return false;
	}

	//ulData = ::htonl( ulData );
	::memcpy( m_abytePacketContent + iCP, &ulData, sizeof( unsigned long ) );
	iCP = iCP + ( int )sizeof( unsigned long );
	return true;
}

bool KNaunBillingPacket::Write8Byte( const __int64& iData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )sizeof( __int64 ) )
	{
		return false;
	}

	__int64 iDataTemp = iData;
	unsigned long aulDataTemp[2];

	::memcpy( aulDataTemp, &iDataTemp, sizeof( __int64 ) );
	//aulDataTemp[0] = ::htonl( aulDataTemp[0] );
	//aulDataTemp[1] = ::htonl( aulDataTemp[1] );

	unsigned long ulTemp = aulDataTemp[0];
	aulDataTemp[0] = aulDataTemp[1];
	aulDataTemp[1] = ulTemp;

	::memcpy( m_abytePacketContent + iCP, aulDataTemp, sizeof( __int64 ) );
	iCP = iCP + ( int )sizeof( __int64 );
	return true;
}

bool KNaunBillingPacket::WriteString( const std::wstring& wstrData, int& iCP )
{
	std::string strData = KncUtil::toNarrowString( wstrData );
	unsigned short usLength = ( unsigned short )strData.size();
	if( !Write2Byte( usLength, iCP ) )
	{
		return false;
	}

	if( usLength == 0 )
	{
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - ( int )usLength )
	{
		return false;
	}

	char szData[MAX_PATH];
	::strncpy( szData, strData.c_str(), MAX_PATH );

	::memcpy( m_abytePacketContent + iCP, szData, ( size_t )usLength );
	iCP = iCP + ( int )usLength;
	return true;
}

bool KNaunBillingPacket::WriteString( const std::wstring& wstrData, const int iFixedLength, int& iCP )
{
	std::string strData = KncUtil::toNarrowString( wstrData );

	_JIF( iFixedLength <= MAX_PACKET_CONTENT_SIZE_NUBT, return false );

	if( iFixedLength == 0 )
	{
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NUBT - static_cast<signed>(iFixedLength) )
	{
		return false;
	}

	char szData[MAX_PATH];
	::strncpy( szData, strData.c_str(), iFixedLength );

	::memcpy( m_abytePacketContent + iCP, szData, static_cast<size_t>(iFixedLength) );
	iCP = iCP + iFixedLength;
	return true;
}

// 숫자를 스트링으로~
std::string KNaunBillingPacket::toString(int iSource)
{
	char szData[20];
	::itoa( iSource, szData, 10 );

	std::string str = szData;
	return str;
}

bool KNaunBillingPacket::Write( const KENAUN_BILLING_BALANCE_REQ& kPacket )
{
	int iCP = 0;
	__int64 iDummy = 0;

	_JIF( WriteString( kPacket.m_wstrUserUID, 40, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, 40, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCompanyCode, 4, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrNickName, 30, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulShopBalance, iCP ), return false );
	_JIF( Write8Byte( iDummy, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulContentsBalance, iCP ), return false );
	_JIF( Write8Byte( iDummy, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulBonusBalance, iCP ), return false );
	_JIF( Write8Byte( iDummy, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulEtcBalance, iCP ), return false );
	_JIF( Write8Byte( iDummy, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulShopMileage, iCP ), return false );
	_JIF( Write8Byte( iDummy, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulContentsMileage, iCP ), return false );
	_JIF( Write8Byte( iDummy, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrReturnCode, 4, iCP ), return false );

	return true;
}

bool KNaunBillingPacket::Read( KENAUN_BILLING_BALANCE_ACK& kPacket )
{
	int iCP = 0;
	__int64 iDummy = 0;

	_JIF( ReadString( kPacket.m_wstrUserUID, 40+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, 40+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCompanyCode, 4+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrNickName, 30+1, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulShopBalance, iCP ), return false );
	_JIF( Read8Byte( iDummy, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulContentsBalance, iCP ), return false );
	_JIF( Read8Byte( iDummy, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulBonusBalance, iCP ), return false );
	_JIF( Read8Byte( iDummy, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulEtcBalance, iCP ), return false );
	_JIF( Read8Byte( iDummy, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulShopMileage, iCP ), return false );
	_JIF( Read8Byte( iDummy, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulContentsMileage, iCP ), return false );
	_JIF( Read8Byte( iDummy, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrReturnCode, 4+1, iCP ), return false );

	LIF( iCP == m_usReqLen - HEADER_IN_PACKET_LENGTH_NUBT );
	return true;
}

bool KNaunBillingPacket::Write( IN const KNaunBuyProductRequestData& kPacket )
{
	int iCP = 0;
	unsigned short usDummy = 0;
	__int64 iDummy = 0;

	_JIF( WriteString( kPacket.m_wstrUserUID, 40, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, 40, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCompanyCode, 4, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrNickName, 30, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrIP, 15, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulBuyType, iCP ), return false );
	_JIF( Write2Byte( usDummy, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrProductName, 50, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrProductNo, 20, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCategory, 50, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductCnt, iCP ), return false );
	_JIF( Write2Byte( usDummy, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrProductEtc, 20, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrToUserUID, 40, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrToUserID, 40, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulPrice, iCP ), return false );
	_JIF( Write8Byte( iDummy, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrAgencyNo, 20, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrEtc2, 100, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrEtc3, 100, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrEtc4, 100, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrOrderNo, 20, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrReturnCode, 4, iCP ), return false );

	return true;
}

bool KNaunBillingPacket::Read( OUT KNaunBuyProductRequestData& kPacket )
{
	int iCP = 0;
	unsigned short usDummy = 0;
	__int64 iDummy = 0;

	_JIF( ReadString( kPacket.m_wstrUserUID, 40+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, 40+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCompanyCode, 4+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrNickName, 30+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrIP, 15+1, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulBuyType, iCP ), return false );
	_JIF( Read2Byte( usDummy, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrProductName, 50+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrProductNo, 20+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCategory, 50+1, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductCnt, iCP ), return false );
	_JIF( Read2Byte( usDummy, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrProductEtc, 20+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrToUserUID, 40+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrToUserID, 40+1, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulPrice, iCP ), return false );
	_JIF( Read8Byte( iDummy, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrAgencyNo, 20+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrEtc2, 100+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrEtc3, 100+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrEtc4, 100+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrOrderNo, 20+1, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrReturnCode, 4+1, iCP ), return false );
	
	LIF( iCP == m_usReqLen - HEADER_IN_PACKET_LENGTH_NUBT );
	return true;
}

#endif SERV_COUNTRY_IN