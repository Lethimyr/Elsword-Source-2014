#include "NexonBillingTCPPacket.h"

//{{ 2013. 01. 08	최육사	빌링 패킷 넘치는 유저 찾기
#ifdef SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
static std::map< BYTE, std::wstring > g_mapPacketType;
#endif SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
//}}


KNexonBillingTCPPacket::KNexonBillingTCPPacket()
{
	m_byteReservedChar = 0xAF;
	m_ulPacketLength = 0;
	m_ulPacketNo = 0;
    m_bytePacketType = 0;
}

KNexonBillingTCPPacket::~KNexonBillingTCPPacket()
{
}

//{{ 2013. 01. 08	최육사	빌링 패킷 넘치는 유저 찾기
#ifdef SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
const wchar_t*   KNexonBillingTCPPacket::GetPacketTypeStr( BYTE bytePacketType )
{
	if( g_mapPacketType.empty() )
	{
#undef _ENUM
#define _ENUM( name, id ) g_mapPacketType.insert( std::make_pair( id, L#name ) );

//#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
#undef _ENUM_PROCESS
#define _ENUM_PROCESS( name, id, pname ) _ENUM( name, id )
//#endif // SERV_NEXON_COUPON_SYSTEM

#       include "NexonBillingTCP_def.h"
	}

	// 패킷 이름을 찾자!
	std::map< BYTE, std::wstring >::const_iterator mit;
	mit = g_mapPacketType.find( bytePacketType );
	if( mit == g_mapPacketType.end() )
	{
		return L"";
	}

	return mit->second.c_str();
}
#endif SERV_BILLING_TCP_PACKET_MAX_OVER_USER_CHECK
//}}

//{{ 2013. 01. 10	최육사	빌링 TCP 패킷 크기 예외처리
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//////////////////////////////////////////////////////////////////////////
bool KNexonBillingTCPPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	BYTE byteReservedChar;
	::memcpy( &byteReservedChar, pbyteBuffer, sizeof( BYTE ) );
	_JIF( byteReservedChar == 0xAF, return false );
	::memcpy( &m_ulPacketLength, pbyteBuffer + 1, sizeof( unsigned long ) );
	m_ulPacketLength = ::ntohl( m_ulPacketLength );
	_JIF( m_ulPacketLength >= HEADER_IN_PACKET_LENGTH_NBT, return false );
	//_JIF( m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT <= MAX_PACKET_CONTENT_SIZE_NBT, return false );	// 가변 버퍼를 쓰면 최대 패킷 사이즈를 넘어설수 있다.
	::memcpy( &m_ulPacketNo, pbyteBuffer + 5, sizeof( unsigned long ) );
	m_ulPacketNo = ::ntohl( m_ulPacketNo );
	::memcpy( &m_bytePacketType, pbyteBuffer + 9, sizeof( BYTE ) );
	if( m_ulPacketLength > HEADER_IN_PACKET_LENGTH_NBT )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 10, m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
bool KNexonBillingTCPPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	BYTE byteReservedChar;
	::memcpy( &byteReservedChar, pbyteBuffer, sizeof( BYTE ) );
	_JIF( byteReservedChar == 0xAF, return false );
	::memcpy( &m_ulPacketLength, pbyteBuffer + 1, sizeof( unsigned long ) );
	m_ulPacketLength = ::ntohl( m_ulPacketLength );
	_JIF( m_ulPacketLength >= HEADER_IN_PACKET_LENGTH_NBT, return false );
	_JIF( m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT <= MAX_PACKET_CONTENT_SIZE_NBT, return false );
	::memcpy( &m_ulPacketNo, pbyteBuffer + 5, sizeof( unsigned long ) );
	m_ulPacketNo = ::ntohl( m_ulPacketNo );
	::memcpy( &m_bytePacketType, pbyteBuffer + 9, sizeof( BYTE ) );
	if( m_ulPacketLength > HEADER_IN_PACKET_LENGTH_NBT )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 10, m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_BILLING_TCP_RECV_VARIABLE_SIZE_BUFFER
//////////////////////////////////////////////////////////////////////////
//}}

bool KNexonBillingTCPPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

    _JIF( m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT <= MAX_PACKET_CONTENT_SIZE_NBT, return false );
	::memcpy( pbyteBuffer, &m_byteReservedChar, sizeof( BYTE ) );
	unsigned long ulPacketLength = ::htonl( m_ulPacketLength );
	::memcpy( pbyteBuffer + 1, &ulPacketLength, sizeof( unsigned long ) );
    unsigned long ulPacketNo = ::htonl( m_ulPacketNo );
	::memcpy( pbyteBuffer + 5, &ulPacketNo, sizeof( unsigned long ) );
    ::memcpy( pbyteBuffer + 9, &m_bytePacketType, sizeof( BYTE ) );
	if( m_ulPacketLength > HEADER_IN_PACKET_LENGTH_NBT )
	{
		::memcpy( pbyteBuffer + 10, m_abytePacketContent, m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );
	}

	return true;
}

bool KNexonBillingTCPPacket::ReadByte( BYTE& byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( BYTE ) )
	{
		return false;
	}

	BYTE byteDataTemp;
	::memcpy( &byteDataTemp, m_abytePacketContent + iCP, sizeof( BYTE ) );
	byteData = byteDataTemp;
	iCP = iCP + ( int )sizeof( BYTE );

	return true;
}

bool KNexonBillingTCPPacket::Read2Byte( unsigned short& usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( unsigned short ) )
	{
		return false;
	}

	unsigned short usDataTemp;
	::memcpy( &usDataTemp, m_abytePacketContent + iCP, sizeof( unsigned short ) );
	usData = usDataTemp;
	usData = ::ntohs( usData );
	iCP = iCP + ( int )sizeof( unsigned short );
	return true;
}

//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
bool KNexonBillingTCPPacket::Read2ByteSigned( short& sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( short ) )
	{
		return false;
	}

	short sDataTemp;
	::memcpy( &sDataTemp, m_abytePacketContent + iCP, sizeof( short ) );
	sData = sDataTemp;
	sData = ::ntohs( sData );
	iCP = iCP + ( int )sizeof( short );
	return true;
}
//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-21	// 박세훈
bool KNexonBillingTCPPacket::Read4Byte( int& iData, int& iCP )
{
	const int iSize = sizeof( int );

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - iSize )
	{
		return false;
	}

	::memcpy( &iData, m_abytePacketContent + iCP, iSize );
	iData = ::ntohl( iData );
	iCP = iCP + iSize;
	return true;
}
#endif // SERV_NEXON_COUPON_SYSTEM

bool KNexonBillingTCPPacket::Read4Byte( unsigned long& ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( unsigned long ) )
	{
		return false;
	}

	unsigned long ulDataTemp;
	::memcpy( &ulDataTemp, m_abytePacketContent + iCP, sizeof( unsigned long ) );
	ulData = ulDataTemp;
	ulData = ::ntohl( ulData );
	iCP = iCP + ( int )sizeof( unsigned long );
	return true;
}

bool KNexonBillingTCPPacket::Read8Byte( __int64& iData, int& iCP )
{
    if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( __int64 ) )
    {
        return false;
    }

    __int64 iDataTemp;
    ::memcpy( &iDataTemp, m_abytePacketContent + iCP, sizeof( __int64 ) );

    unsigned long aulDataTemp[2];
    ::memcpy( aulDataTemp, &iDataTemp, sizeof( __int64 ) );

    aulDataTemp[0] = ::ntohl( aulDataTemp[0] );
    aulDataTemp[1] = ::ntohl( aulDataTemp[1] );

    unsigned long ulTemp = aulDataTemp[0];
    aulDataTemp[0] = aulDataTemp[1];
    aulDataTemp[1] = ulTemp;

    ::memcpy( &iDataTemp, aulDataTemp, sizeof( __int64 ) );

    iData = iDataTemp;
    iCP = iCP + ( int )sizeof( __int64 );
    return true;
}

bool KNexonBillingTCPPacket::ReadString( std::wstring& wstrData, int& iCP )
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

    if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )usLength )
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

bool KNexonBillingTCPPacket::WriteByte( BYTE byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( BYTE ) )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &byteData, sizeof( BYTE ) );
	iCP = iCP + ( int )sizeof( BYTE );
	return true;
}

bool KNexonBillingTCPPacket::Write2Byte( unsigned short usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( unsigned short ) )
	{
		return false;
	}

	usData = ::htons( usData );
	::memcpy( m_abytePacketContent + iCP, &usData, sizeof( unsigned short ) );
	iCP = iCP + ( int )sizeof( unsigned short );
	return true;
}

//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
bool KNexonBillingTCPPacket::Write2ByteSigned( short sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( short ) )
	{
		return false;
	}

	sData = ::htons( sData );
	::memcpy( m_abytePacketContent + iCP, &sData, sizeof( short ) );
	iCP = iCP + ( int )sizeof( short );
	return true;
}
//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
bool KNexonBillingTCPPacket::Write4Byte( int& iData, int& iCP )
{
	const int iSize = sizeof( int );

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - iSize )
	{
		return false;
	}

	iData = ::htonl( iData );
	::memcpy( m_abytePacketContent + iCP, &iData, iSize );
	iCP = iCP + iSize;
	return true;
}
#endif // SERV_NEXON_COUPON_SYSTEM

bool KNexonBillingTCPPacket::Write4Byte( unsigned long ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( unsigned long ) )
	{
		return false;
	}

	ulData = ::htonl( ulData );
	::memcpy( m_abytePacketContent + iCP, &ulData, sizeof( unsigned long ) );
	iCP = iCP + ( int )sizeof( unsigned long );
	return true;
}

bool KNexonBillingTCPPacket::Write8Byte( const __int64& iData, int& iCP )
{
    if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )sizeof( __int64 ) )
    {
        return false;
    }

    __int64 iDataTemp = iData;
    unsigned long aulDataTemp[2];

    ::memcpy( aulDataTemp, &iDataTemp, sizeof( __int64 ) );
    aulDataTemp[0] = ::htonl( aulDataTemp[0] );
    aulDataTemp[1] = ::htonl( aulDataTemp[1] );

    unsigned long ulTemp = aulDataTemp[0];
    aulDataTemp[0] = aulDataTemp[1];
    aulDataTemp[1] = ulTemp;

    ::memcpy( m_abytePacketContent + iCP, aulDataTemp, sizeof( __int64 ) );
    iCP = iCP + ( int )sizeof( __int64 );
    return true;
}

bool KNexonBillingTCPPacket::WriteString( const std::wstring& wstrData, int& iCP )
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

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_NBT - ( int )usLength )
	{
		return false;
	}

    char szData[MAX_PATH];
#ifdef _CONVERT_VS_2010
	strcpy_s( szData, strData.c_str() );
#else
	::strcpy( szData, strData.c_str() );
#endif _CONVERT_VS_2010

	::memcpy( m_abytePacketContent + iCP, szData, ( size_t )usLength );
	iCP = iCP + ( int )usLength;
	return true;
}

//{{ 2008. 1. 25  최육사  구조체에 대한 Read & Write
bool KNexonBillingTCPPacket::Read( KNXBTCategoryInfo& kNXBTCategoryInfo, int& iCP )
{
	_JIF( Read4Byte(  kNXBTCategoryInfo.m_ulCategoryNo,			 iCP ), return false );
	_JIF( ReadString( kNXBTCategoryInfo.m_wstrCategoryName,		 iCP ), return false );
	_JIF( Read4Byte(  kNXBTCategoryInfo.m_ulParentCategoryNo,	 iCP ), return false );
	_JIF( Read4Byte(  kNXBTCategoryInfo.m_ulDisplayNo,			 iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTCategoryInfo& kNXBTCategoryInfo, int& iCP )
{
	_JIF( Write4Byte(  kNXBTCategoryInfo.m_ulCategoryNo,		iCP ), return false );
	_JIF( WriteString( kNXBTCategoryInfo.m_wstrCategoryName,	iCP ), return false );
	_JIF( Write4Byte(  kNXBTCategoryInfo.m_ulParentCategoryNo,	iCP ), return false );
	_JIF( Write4Byte(  kNXBTCategoryInfo.m_ulDisplayNo,			iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Read( KNXBTBonusProductInfo& kNXBTBonusProductInfo, int& iCP )
{
    _JIF( Read4Byte( kNXBTBonusProductInfo.m_ulProductNo, iCP ), return false );
    _JIF( ReadString( kNXBTBonusProductInfo.m_wstrExtend, iCP ), return false );
    return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTBonusProductInfo& kNXBTBonusProductInfo, int& iCP )
{
    _JIF( Write4Byte( kNXBTBonusProductInfo.m_ulProductNo, iCP ), return false );
    _JIF( WriteString( kNXBTBonusProductInfo.m_wstrExtend, iCP ), return false );
    return true;
}

//{{ 2008. 7. 10  최육사  빌링패킷 수정
bool KNexonBillingTCPPacket::Read( KNXBTBonusProductInfoPickUp& kNXBTBonusProductInfoPickUp, int& iCP )
{
	_JIF( Read4Byte( kNXBTBonusProductInfoPickUp.m_ulBonusProduct, iCP ), return false );
	_JIF( Read4Byte( kNXBTBonusProductInfoPickUp.m_ulBonusAmount, iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTBonusProductInfoPickUp& kNXBTBonusProductInfoPickUp, int& iCP )
{
	_JIF( Write4Byte( kNXBTBonusProductInfoPickUp.m_ulBonusProduct, iCP ), return false );
	_JIF( Write4Byte( kNXBTBonusProductInfoPickUp.m_ulBonusAmount, iCP ), return false );
	return true;
}
//}}

bool KNexonBillingTCPPacket::Read( KNXBTProductInfo& kNXBTProductInfo, int& iCP )
{
	_JIF( Read4Byte( kNXBTProductInfo.m_ulProductNo, iCP ), return false );
	_JIF( Read4Byte( kNXBTProductInfo.m_ulRelationProductNo, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Read2ByteSigned( kNXBTProductInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Read2Byte( kNXBTProductInfo.m_usProductPieces, iCP ), return false );
	_JIF( ReadString( kNXBTProductInfo.m_wstrProductID, iCP ), return false );
	_JIF( ReadString( kNXBTProductInfo.m_wstrProductGUID, iCP ), return false );
    _JIF( Read4Byte( kNXBTProductInfo.m_ulPaymentType, iCP ), return false );
    _JIF( ReadString( kNXBTProductInfo.m_wstrProductType, iCP ), return false );
    _JIF( Read4Byte( kNXBTProductInfo.m_ulSalePrice, iCP ), return false );
    _JIF( Read4Byte( kNXBTProductInfo.m_ulCategoryNo, iCP ), return false );	

    BYTE byteArrayLength = 0;
    _JIF( ReadByte( byteArrayLength, iCP ), return false );

    for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
    {
        KNXBTBonusProductInfo kNXBTBonusProductInfo;
        _JIF( Read( kNXBTBonusProductInfo, iCP ), return false );
        kNXBTProductInfo.m_vecBonusProductInfo.push_back( kNXBTBonusProductInfo );
    }

    return true;

}

bool KNexonBillingTCPPacket::Write( const KNXBTProductInfo& kNXBTProductInfo, int& iCP )
{
	_JIF( Write4Byte( kNXBTProductInfo.m_ulProductNo, iCP ), return false );
	_JIF( Write4Byte( kNXBTProductInfo.m_ulRelationProductNo, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Write2ByteSigned( kNXBTProductInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Write2Byte( kNXBTProductInfo.m_usProductPieces, iCP ), return false );
	_JIF( WriteString( kNXBTProductInfo.m_wstrProductID, iCP ), return false );
	_JIF( WriteString( kNXBTProductInfo.m_wstrProductGUID, iCP ), return false );
    _JIF( Write4Byte( kNXBTProductInfo.m_ulPaymentType, iCP ), return false );
    _JIF( WriteString( kNXBTProductInfo.m_wstrProductType, iCP ), return false );
    _JIF( Write4Byte( kNXBTProductInfo.m_ulSalePrice, iCP ), return false );
    _JIF( Write4Byte( kNXBTProductInfo.m_ulCategoryNo, iCP ), return false );	

    unsigned long ulArrayLength = static_cast< unsigned long >( kNXBTProductInfo.m_vecBonusProductInfo.size() );
    _JIF( Write4Byte( ulArrayLength, iCP ), return false );

    std::vector< KNXBTBonusProductInfo >::const_iterator vit;
    for( vit = kNXBTProductInfo.m_vecBonusProductInfo.begin(); vit != kNXBTProductInfo.m_vecBonusProductInfo.end(); vit++ )
    {
        _JIF( Write( *vit, iCP ), return false );
    }
	return true;
}

bool KNexonBillingTCPPacket::Read( KNXBTPurchaseReqInfo& kNXBTPurchaseReqInfo, int& iCP )
{
	_JIF( Read4Byte(  kNXBTPurchaseReqInfo.m_ulProductNo,			 iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Read2Byte(  kNXBTPurchaseReqInfo.m_usOrderQuantity,		 iCP ), return false );	
	//}}	
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( ( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON ) ||
		( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON )
		)
	{
		_JIF( Read4Byte(	kNXBTPurchaseReqInfo.m_iCouponCardNo,	iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTPurchaseReqInfo& kNXBTPurchaseReqInfo, int& iCP )
{
	_JIF( Write4Byte(  kNXBTPurchaseReqInfo.m_ulProductNo,			 iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Write2Byte(  kNXBTPurchaseReqInfo.m_usOrderQuantity,		 iCP ), return false );	
	//}}
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( ( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON ) ||
		( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON )
		)
	{
		_JIF( Write4Byte(	kNXBTPurchaseReqInfo.m_iCouponCardNo,	iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	return true;
}

bool KNexonBillingTCPPacket::Read( KNXBTPurchaseAckInfo& kNXBTPurchaseAckInfo, int& iCP )
{
	_JIF( Read4Byte(  kNXBTPurchaseAckInfo.m_ulProductNo,			 iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Read2Byte(  kNXBTPurchaseAckInfo.m_usOrderQuantity,		 iCP ), return false );
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( ( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON ) ||
		( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON )
		)
	{
		_JIF( Read4Byte(	kNXBTPurchaseAckInfo.m_ulCouponUseApproval,	iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	_JIF( ReadString( kNXBTPurchaseAckInfo.m_wstrExtendValue,		 iCP ), return false );
	//}}	
	return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTPurchaseAckInfo& kNXBTPurchaseAckInfo, int& iCP )
{
	_JIF( Write4Byte(  kNXBTPurchaseAckInfo.m_ulProductNo,			 iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Write2Byte(  kNXBTPurchaseAckInfo.m_usOrderQuantity,		 iCP ), return false );
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( ( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON ) ||
		( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON )
		)
	{
		_JIF( Write4Byte(	kNXBTPurchaseAckInfo.m_ulCouponUseApproval,	iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	_JIF( WriteString( kNXBTPurchaseAckInfo.m_wstrExtendValue,		 iCP ), return false );
	//}}
	return true;
}

bool KNexonBillingTCPPacket::Read( KNXBTOrderInfo& kNXBTOrderInfo, int& iCP )
{
	_JIF( Read4Byte( kNXBTOrderInfo.m_ulOrderNo, iCP ), return false );
	_JIF( Read4Byte( kNXBTOrderInfo.m_ulProductNo, iCP ), return false );
	_JIF( ReadByte( kNXBTOrderInfo.m_byteProductKind, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrProductName, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Read2ByteSigned( kNXBTOrderInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Read2Byte( kNXBTOrderInfo.m_usProductPieces, iCP ), return false );
	_JIF( Read2Byte( kNXBTOrderInfo.m_usOrderQuantity, iCP ), return false );
	_JIF( Read2Byte( kNXBTOrderInfo.m_usRemainOrderQuantity, iCP ), return false );
	_JIF( ReadByte( kNXBTOrderInfo.m_byteIsPresent, iCP ), return false );
	_JIF( ReadByte( kNXBTOrderInfo.m_byteIsRead, iCP ), return false );
	_JIF( ReadByte( kNXBTOrderInfo.m_byteSenderServerNo, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrSenderGameID, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrSenderPresentMessage, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrProductAttribute0, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrProductAttribute1, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrProductAttribute2, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrProductAttribute3, iCP ), return false );
	_JIF( ReadString( kNXBTOrderInfo.m_wstrProductAttribute4, iCP ), return false );
    _JIF( ReadString( kNXBTOrderInfo.m_wstrExtendValue, iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTOrderInfo& kNXBTOrderInfo, int& iCP )
{
	_JIF( Write4Byte( kNXBTOrderInfo.m_ulOrderNo, iCP ), return false );
	_JIF( Write4Byte( kNXBTOrderInfo.m_ulProductNo, iCP ), return false );
	_JIF( WriteByte( kNXBTOrderInfo.m_byteProductKind, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrProductName, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Write2ByteSigned( kNXBTOrderInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Write2Byte( kNXBTOrderInfo.m_usProductPieces, iCP ), return false );
	_JIF( Write2Byte( kNXBTOrderInfo.m_usOrderQuantity, iCP ), return false );
	_JIF( Write2Byte( kNXBTOrderInfo.m_usRemainOrderQuantity, iCP ), return false );
	_JIF( WriteByte( kNXBTOrderInfo.m_byteIsPresent, iCP ), return false );
	_JIF( WriteByte( kNXBTOrderInfo.m_byteIsRead, iCP ), return false );
	_JIF( WriteByte( kNXBTOrderInfo.m_byteSenderServerNo, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrSenderGameID, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrSenderPresentMessage, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrProductAttribute0, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrProductAttribute1, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrProductAttribute2, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrProductAttribute3, iCP ), return false );
	_JIF( WriteString( kNXBTOrderInfo.m_wstrProductAttribute4, iCP ), return false );
    _JIF( WriteString( kNXBTOrderInfo.m_wstrExtendValue, iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Read( KNXBTPackageInfo& kNXBTPackageInfo, int& iCP )
{
    _JIF( Read4Byte( kNXBTPackageInfo.m_ulOrderNo, iCP ), return false );
	_JIF( Read4Byte( kNXBTPackageInfo.m_ulPackageProductNo, iCP ), return false );
	_JIF( Read4Byte( kNXBTPackageInfo.m_ulProductNo, iCP ), return false );
	_JIF( ReadString( kNXBTPackageInfo.m_wstrProductName, iCP ), return false );
	_JIF( ReadString( kNXBTPackageInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Read2ByteSigned( kNXBTPackageInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Read2Byte( kNXBTPackageInfo.m_usProductPieces, iCP ), return false );
	_JIF( ReadString( kNXBTPackageInfo.m_wstrProductAttribute0, iCP ), return false );
	_JIF( ReadString( kNXBTPackageInfo.m_wstrProductAttribute1, iCP ), return false );
	_JIF( ReadString( kNXBTPackageInfo.m_wstrProductAttribute2, iCP ), return false );
	_JIF( ReadString( kNXBTPackageInfo.m_wstrProductAttribute3, iCP ), return false );
	_JIF( ReadString( kNXBTPackageInfo.m_wstrProductAttribute4, iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTPackageInfo& kNXBTPackageInfo, int& iCP )
{
    _JIF( Write4Byte( kNXBTPackageInfo.m_ulOrderNo, iCP ), return false );
	_JIF( Write4Byte( kNXBTPackageInfo.m_ulPackageProductNo, iCP ), return false );
	_JIF( Write4Byte( kNXBTPackageInfo.m_ulProductNo, iCP ), return false );
	_JIF( WriteString( kNXBTPackageInfo.m_wstrProductName, iCP ), return false );
	_JIF( WriteString( kNXBTPackageInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Write2ByteSigned( kNXBTPackageInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Write2Byte( kNXBTPackageInfo.m_usProductPieces, iCP ), return false );
	_JIF( WriteString( kNXBTPackageInfo.m_wstrProductAttribute0, iCP ), return false );
	_JIF( WriteString( kNXBTPackageInfo.m_wstrProductAttribute1, iCP ), return false );
	_JIF( WriteString( kNXBTPackageInfo.m_wstrProductAttribute2, iCP ), return false );
	_JIF( WriteString( kNXBTPackageInfo.m_wstrProductAttribute3, iCP ), return false );
	_JIF( WriteString( kNXBTPackageInfo.m_wstrProductAttribute4, iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Read( KNXBTLotteryInfo& kNXBTLotteryInfo, int& iCP )
{
    _JIF( Read4Byte( kNXBTLotteryInfo.m_ulOrderNo, iCP ), return false );
	_JIF( Read4Byte( kNXBTLotteryInfo.m_ulLotteryProductNo, iCP ), return false );
	_JIF( Read4Byte( kNXBTLotteryInfo.m_ulProductNo, iCP ), return false );
	_JIF( ReadString( kNXBTLotteryInfo.m_wstrProductName, iCP ), return false );
	_JIF( ReadString( kNXBTLotteryInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Read2ByteSigned( kNXBTLotteryInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Read2Byte( kNXBTLotteryInfo.m_usProductPieces, iCP ), return false );
	_JIF( ReadString( kNXBTLotteryInfo.m_wstrProductAttribute0, iCP ), return false );
	_JIF( ReadString( kNXBTLotteryInfo.m_wstrProductAttribute1, iCP ), return false );
	_JIF( ReadString( kNXBTLotteryInfo.m_wstrProductAttribute2, iCP ), return false );
	_JIF( ReadString( kNXBTLotteryInfo.m_wstrProductAttribute3, iCP ), return false );
	_JIF( ReadString( kNXBTLotteryInfo.m_wstrProductAttribute4, iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTLotteryInfo& kNXBTLotteryInfo, int& iCP )
{
    _JIF( Write4Byte( kNXBTLotteryInfo.m_ulOrderNo, iCP ), return false );
	_JIF( Write4Byte( kNXBTLotteryInfo.m_ulLotteryProductNo, iCP ), return false );
	_JIF( Write4Byte( kNXBTLotteryInfo.m_ulProductNo, iCP ), return false );
	_JIF( WriteString( kNXBTLotteryInfo.m_wstrProductName, iCP ), return false );
	_JIF( WriteString( kNXBTLotteryInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
	_JIF( Write2ByteSigned( kNXBTLotteryInfo.m_sProductExpire, iCP ), return false );
	//}}
	_JIF( Write2Byte( kNXBTLotteryInfo.m_usProductPieces, iCP ), return false );
	_JIF( WriteString( kNXBTLotteryInfo.m_wstrProductAttribute0, iCP ), return false );
	_JIF( WriteString( kNXBTLotteryInfo.m_wstrProductAttribute1, iCP ), return false );
	_JIF( WriteString( kNXBTLotteryInfo.m_wstrProductAttribute2, iCP ), return false );
	_JIF( WriteString( kNXBTLotteryInfo.m_wstrProductAttribute3, iCP ), return false );
	_JIF( WriteString( kNXBTLotteryInfo.m_wstrProductAttribute4, iCP ), return false );
	return true;
}
//}}

bool KNexonBillingTCPPacket::Read( KNXBTCouponProductInfo& kNXBTCouponProductInfo, int& iCP )
{
    _JIF( Read4Byte( kNXBTCouponProductInfo.m_ulProductNo, iCP ), return false );
    _JIF( ReadString( kNXBTCouponProductInfo.m_wstrExtendValue, iCP ), return false );
    return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTCouponProductInfo& kNXBTCouponProductInfo, int& iCP )
{
    _JIF( Write4Byte( kNXBTCouponProductInfo.m_ulProductNo, iCP ), return false );
    _JIF( WriteString( kNXBTCouponProductInfo.m_wstrExtendValue, iCP ), return false );
    return true;
}

bool KNexonBillingTCPPacket::Read( KNXBTSubProductInfo& kNXBTSubProductInfo, int& iCP )
{
    _JIF( Read4Byte( kNXBTSubProductInfo.m_ulProductNo, iCP ), return false );
    _JIF( ReadString( kNXBTSubProductInfo.m_wstrProductName, iCP ), return false );
    _JIF( ReadString( kNXBTSubProductInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
    _JIF( Read2ByteSigned( kNXBTSubProductInfo.m_sProductExpire, iCP ), return false );
	//}}
    _JIF( Read2Byte( kNXBTSubProductInfo.m_usProductPieces, iCP ), return false );
    _JIF( ReadString( kNXBTSubProductInfo.m_wstrProductAttribute0, iCP ), return false );
    _JIF( ReadString( kNXBTSubProductInfo.m_wstrProductAttribute1, iCP ), return false );
    _JIF( ReadString( kNXBTSubProductInfo.m_wstrProductAttribute2, iCP ), return false );
    _JIF( ReadString( kNXBTSubProductInfo.m_wstrProductAttribute3, iCP ), return false );
    _JIF( ReadString( kNXBTSubProductInfo.m_wstrProductAttribute4, iCP ), return false );
    return true;
}

bool KNexonBillingTCPPacket::Write( const KNXBTSubProductInfo& kNXBTSubProductInfo, int& iCP )
{
    _JIF( Write4Byte( kNXBTSubProductInfo.m_ulProductNo, iCP ), return false );
    _JIF( WriteString( kNXBTSubProductInfo.m_wstrProductName, iCP ), return false );
    _JIF( WriteString( kNXBTSubProductInfo.m_wstrProductID, iCP ), return false );
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경	
    _JIF( Write2ByteSigned( kNXBTSubProductInfo.m_sProductExpire, iCP ), return false );
	//}}
    _JIF( Write2Byte( kNXBTSubProductInfo.m_usProductPieces, iCP ), return false );
    _JIF( WriteString( kNXBTSubProductInfo.m_wstrProductAttribute0, iCP ), return false );
    _JIF( WriteString( kNXBTSubProductInfo.m_wstrProductAttribute1, iCP ), return false );
    _JIF( WriteString( kNXBTSubProductInfo.m_wstrProductAttribute2, iCP ), return false );
    _JIF( WriteString( kNXBTSubProductInfo.m_wstrProductAttribute3, iCP ), return false );
    _JIF( WriteString( kNXBTSubProductInfo.m_wstrProductAttribute4, iCP ), return false );
    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_INITIALIZE_REQ& kPacket )
{
	int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrServiceCode, iCP ), return false );
    _JIF( ReadByte( kPacket.m_byteDomain, iCP ), return false );
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_INITIALIZE_REQ& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( WriteString( kPacket.m_wstrServiceCode, iCP ), return false );
    _JIF( WriteByte( kPacket.m_byteDomain, iCP ), return false );
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_INITIALIZE_ACK& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( ReadString( kPacket.m_wstrServiceCode, iCP ), return false );
    _JIF( ReadByte( kPacket.m_byteDomain, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_INITIALIZE_ACK& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( WriteString( kPacket.m_wstrServiceCode, iCP ), return false );
    _JIF( WriteByte( kPacket.m_byteDomain, iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_HEART_BEAT_REQ& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read8Byte( kPacket.m_iReleaseTick, iCP ), return false );
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_HEART_BEAT_REQ& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write8Byte( kPacket.m_iReleaseTick, iCP ), return false );
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_HEART_BEAT_ACK& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_HEART_BEAT_ACK& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

//{{ 2008. 1. 25  최육사  
bool KNexonBillingTCPPacket::Read( KENX_BT_CATEGORY_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_CATEGORY_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_CATEGORY_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );

	unsigned long ulArrayLength = 0;
	_JIF( Read4Byte( ulArrayLength,		 iCP ), return false );
	
	for( unsigned long ulIndex = 0; ulIndex < ulArrayLength; ++ulIndex )
	{
		KNXBTCategoryInfo kNXBTCategoryInfo;
		_JIF( Read( kNXBTCategoryInfo,	 iCP ), return false );
		kPacket.m_vecNXBTCategoryInfo.push_back( kNXBTCategoryInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_CATEGORY_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );

	unsigned long ulArrayLength = static_cast<unsigned long>(kPacket.m_vecNXBTCategoryInfo.size());
	_JIF( Write4Byte( ulArrayLength,	  iCP ), return false );

	std::vector< KNXBTCategoryInfo >::const_iterator vit = kPacket.m_vecNXBTCategoryInfo.begin();
	for( ; vit != kPacket.m_vecNXBTCategoryInfo.end(); ++vit )
	{
		const KNXBTCategoryInfo& kNXBTCategoryInfo = *vit;
		_JIF( Write( kNXBTCategoryInfo,	  iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PRODUCT_INQUIRY_XML_REQ& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read4Byte( kPacket.m_ulPageIndex,	 iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulRowPerPage, iCP ), return false );
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PRODUCT_INQUIRY_XML_REQ& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte( kPacket.m_ulPageIndex,  iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulRowPerPage, iCP ), return false );
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PRODUCT_INQUIRY_XML_ACK& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
    _JIF( Read8Byte( kPacket.m_iReleaseTick, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulTotalProductCount, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrResultXML, iCP ), return false );

    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PRODUCT_INQUIRY_XML_ACK& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
    _JIF( Write8Byte( kPacket.m_iReleaseTick, iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulTotalProductCount, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrResultXML, iCP ), return false );

    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PRODUCT_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulPageIndex,	 iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulRowPerPage, iCP ), return false );
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PRODUCT_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulPageIndex,  iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulRowPerPage, iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PRODUCT_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult,			iCP ), return false );
	_JIF( Read8Byte( kPacket.m_iReleaseTick,		iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulTotalProductCount, iCP ), return false );

	unsigned long ulArrayLength = 0;
	_JIF( Read4Byte( ulArrayLength,		 iCP ), return false );

	for( unsigned long ulIndex = 0; ulIndex < ulArrayLength; ++ulIndex )
	{
		KNXBTProductInfo kNXBTProductInfo;
		_JIF( Read( kNXBTProductInfo,	 iCP ), return false );
		kPacket.m_vecNXBTProductInfo.push_back( kNXBTProductInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PRODUCT_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult,			 iCP ), return false );
	_JIF( Write8Byte( kPacket.m_iReleaseTick,		 iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulTotalProductCount, iCP ), return false );

	unsigned long ulArrayLength = static_cast<unsigned long>(kPacket.m_vecNXBTProductInfo.size());
	_JIF( Write4Byte( ulArrayLength,	  iCP ), return false );

	std::vector< KNXBTProductInfo >::const_iterator vit = kPacket.m_vecNXBTProductInfo.begin();
	for( ; vit != kPacket.m_vecNXBTProductInfo.end(); ++vit )
	{
		const KNXBTProductInfo& kNXBTProductInfo = *vit;
		_JIF( Write( kNXBTProductInfo,	  iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_CHECK_BALANCE_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
	_JIF( Read4Byte(  kPacket.m_ulPaymentType,	 iCP ), return false );
#endif SERV_TOONILAND_CHANNELING
	//}}
	_JIF( ReadString( kPacket.m_wstrUserID,	 iCP ), return false );	
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_CHECK_BALANCE_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
	_JIF( Write4Byte(  kPacket.m_ulPaymentType,		 iCP ), return false );
#endif SERV_TOONILAND_CHANNELING
	//}}
	_JIF( WriteString( kPacket.m_wstrUserID,	 iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_CHECK_BALANCE_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult,	 iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulBalance,	 iCP ), return false );
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_CHECK_BALANCE_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult,	 iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulBalance,	 iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PURCHASE_ITEM_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte(  kPacket.m_ulRemoteIP,		 iCP ), return false );
	_JIF( ReadByte(   kPacket.m_byteReason,		 iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGameID,		 iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID,		 iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulUserOID,		 iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserName,	 iCP ), return false );
	_JIF( ReadByte(   kPacket.m_byteUserAge,	 iCP ), return false );
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON )
	{
		_JIF( ReadByte(   kPacket.m_byteIsCafe,	 iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	_JIF( ReadString( kPacket.m_wstrOrderID,	 iCP ), return false );
	//{{ 2008. 1. 29  최육사  변수 추가	
	_JIF( Read4Byte(  kPacket.m_ulPaymentType,	 iCP ), return false );
	//}}
	_JIF( Read4Byte(  kPacket.m_ulTotalAmount,	 iCP ), return false );

	BYTE byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,			 iCP ), return false );

	for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTPurchaseReqInfo kNXBTPurchaseInfo;
		_JIF( Read( kNXBTPurchaseInfo,			 iCP ), return false );
		kPacket.m_vecNXBTPurchaseInfo.push_back( kNXBTPurchaseInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PURCHASE_ITEM_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte(  kPacket.m_ulRemoteIP,		 iCP ), return false );
	_JIF( WriteByte(   kPacket.m_byteReason,		 iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGameID,		 iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID,		 iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulUserOID,			 iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserName,		 iCP ), return false );
	_JIF( WriteByte(   kPacket.m_byteUserAge,		 iCP ), return false );
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_ITEM_WITH_DISCOUNT_COUPON )
	{
		_JIF( WriteByte(   kPacket.m_byteIsCafe,	 iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	_JIF( WriteString( kPacket.m_wstrOrderID,		 iCP ), return false );
	//{{ 2008. 1. 29  최육사  변수 추가
	_JIF( Write4Byte(  kPacket.m_ulPaymentType,		 iCP ), return false );
	//}}
	_JIF( Write4Byte(  kPacket.m_ulTotalAmount,		 iCP ), return false );

	BYTE byteArrayLength = static_cast<BYTE>(kPacket.m_vecNXBTPurchaseInfo.size());
	_JIF( WriteByte(   byteArrayLength,				 iCP ), return false );

	std::vector< KNXBTPurchaseReqInfo >::const_iterator vit = kPacket.m_vecNXBTPurchaseInfo.begin();
	for( ; vit != kPacket.m_vecNXBTPurchaseInfo.end(); ++vit )
	{
		const KNXBTPurchaseReqInfo& kNXBTPurchaseInfo = *vit;
		_JIF( Write( kNXBTPurchaseInfo,				 iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PURCHASE_ITEM_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrOrderID,	 iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulResult,		 iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulOrderNo,		 iCP ), return false );

	BYTE byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,			 iCP ), return false );

	for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTPurchaseAckInfo kNXBTPurchaseInfo;
		_JIF( Read( kNXBTPurchaseInfo,			 iCP ), return false );
		kPacket.m_vecNXBTPurchaseInfo.push_back( kNXBTPurchaseInfo );
	}
	
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PURCHASE_ITEM_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrOrderID,	 iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulResult,		 iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulOrderNo,		 iCP ), return false );

	BYTE byteArrayLength = static_cast<BYTE>(kPacket.m_vecNXBTPurchaseInfo.size());
	_JIF( WriteByte(   byteArrayLength,			 iCP ), return false );

	std::vector< KNXBTPurchaseAckInfo >::const_iterator vit = kPacket.m_vecNXBTPurchaseInfo.begin();
	for( ; vit != kPacket.m_vecNXBTPurchaseInfo.end(); ++vit )
	{
		const KNXBTPurchaseAckInfo& kNXBTPurchaseInfo = *vit;
		_JIF( Write( kNXBTPurchaseInfo,			 iCP ), return false );
	}
	
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PURCHASE_GIFT_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulRemoteIP, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteReason, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrSenderGameID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrSenderUserID, iCP ), return false );	
	_JIF( Read4Byte( kPacket.m_ulSenderUserOID, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrSenderUserName, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteSenderUserAge, iCP ), return false );
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON )
	{
		_JIF( ReadByte(   kPacket.m_byteIsCafe,	 iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	_JIF( ReadByte( kPacket.m_byteReceiverServerNo, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrReceiverGameID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrMessage, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrOrderID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulPaymentType, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulTotalAmount, iCP ), return false );

	BYTE byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength, iCP ), return false );

	for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTPurchaseReqInfo kNXBTPurchaseInfo;
		_JIF( Read( kNXBTPurchaseInfo, iCP ), return false );
		kPacket.m_vecNXBTPurchaseInfo.push_back( kNXBTPurchaseInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PURCHASE_GIFT_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulRemoteIP, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteReason, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrSenderGameID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrSenderUserID, iCP ), return false );	
	_JIF( Write4Byte( kPacket.m_ulSenderUserOID, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrSenderUserName, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteSenderUserAge, iCP ), return false );
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	if( m_bytePacketType == KNexonBillingTCPPacket::NISMS_PURCHASE_GIFT_WITH_DISCOUNT_COUPON )
	{
		_JIF( WriteByte(   kPacket.m_byteIsCafe,	 iCP ), return false );
	}
#endif // SERV_NEXON_COUPON_SYSTEM
	_JIF( WriteByte( kPacket.m_byteReceiverServerNo, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrReceiverGameID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrMessage, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrOrderID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulPaymentType, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulTotalAmount, iCP ), return false );

	BYTE byteArrayLength = static_cast< BYTE >( kPacket.m_vecNXBTPurchaseInfo.size() );
	_JIF( WriteByte( byteArrayLength, iCP ), return false );

	std::vector< KNXBTPurchaseReqInfo >::const_iterator vit;
	for( vit = kPacket.m_vecNXBTPurchaseInfo.begin(); vit != kPacket.m_vecNXBTPurchaseInfo.end(); ++vit )
	{
		_JIF( Write( *vit, iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_PURCHASE_GIFT_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;	
	_JIF( ReadString( kPacket.m_wstrOrderID,		iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulResult,			iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulOrderNo,			iCP ), return false );

	BYTE byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,				iCP ), return false );

	for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTPurchaseAckInfo kNXBTPurchaseInfo;
		_JIF( Read( kNXBTPurchaseInfo,				iCP ), return false );
		kPacket.m_vecNXBTPurchaseInfo.push_back( kNXBTPurchaseInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_PURCHASE_GIFT_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrOrderID,		iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulResult,			iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulOrderNo,			iCP ), return false );

	BYTE byteArrayLength = static_cast<BYTE>(kPacket.m_vecNXBTPurchaseInfo.size());
	_JIF( WriteByte(   byteArrayLength,				iCP ), return false );

	std::vector< KNXBTPurchaseAckInfo >::const_iterator vit = kPacket.m_vecNXBTPurchaseInfo.begin();
	for( ; vit != kPacket.m_vecNXBTPurchaseInfo.end(); ++vit )
	{
		const KNXBTPurchaseAckInfo& kNXBTPurchaseInfo = *vit;
		_JIF( Write( kNXBTPurchaseInfo,				iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

//{{ 2009. 10. 14  최육사	자동결제
bool KNexonBillingTCPPacket::Read( KENX_BT_CHECK_AUTO_PAYMENT_USER_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_CHECK_AUTO_PAYMENT_USER_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductNo, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_CHECK_AUTO_PAYMENT_USER_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_CHECK_AUTO_PAYMENT_USER_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;	
	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
//}}

bool KNexonBillingTCPPacket::Read( KENX_BT_COUPON_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte(   kPacket.m_byteIsCafe,			iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCouponString,	iCP ), return false );
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_COUPON_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte(   kPacket.m_byteIsCafe,		iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCouponString,	iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_COUPON_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte(  kPacket.m_ulResult,			iCP ), return false );

	BYTE byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,				iCP ), return false );

	for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTCouponProductInfo kNXBTCouponProductInfo;
		_JIF( Read( kNXBTCouponProductInfo,			iCP ), return false );
		kPacket.m_vecProductInfo.push_back( kNXBTCouponProductInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_COUPON_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;	
	_JIF( Write4Byte(  kPacket.m_ulResult,			iCP ), return false );	

	BYTE byteArrayLength = static_cast<BYTE>(kPacket.m_vecProductInfo.size());
	_JIF( WriteByte(   byteArrayLength,				iCP ), return false );

	std::vector< KNXBTCouponProductInfo >::const_iterator vit = kPacket.m_vecProductInfo.begin();
	for( ; vit != kPacket.m_vecProductInfo.end(); ++vit )
	{		
		_JIF( Write( *vit,							iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

//{{ 2009. 3. 24  최육사	쿠폰사용여부 조회
bool KNexonBillingTCPPacket::Read( KENX_BT_COUPON_INQUIRY_NEW_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte(  kPacket.m_ulUserIP,			iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGameID,			iCP ), return false );
	_JIF( ReadByte(   kPacket.m_byteIsCafe,			iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID,			iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulUserOID,			iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCouponString,	iCP ), return false );
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_COUPON_INQUIRY_NEW_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte(  kPacket.m_ulUserIP,			iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGameID,		iCP ), return false );
	_JIF( WriteByte(   kPacket.m_byteIsCafe,		iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID,		iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulUserOID,			iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCouponString,	iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_COUPON_INQUIRY_NEW_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte(  kPacket.m_ulResult,			iCP ), return false );

	BYTE byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,				iCP ), return false );

	for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTCouponProductInfo kNXBTCouponProductInfo;
		_JIF( Read( kNXBTCouponProductInfo,			iCP ), return false );
		kPacket.m_vecProductInfo.push_back( kNXBTCouponProductInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_COUPON_INQUIRY_NEW_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;	
	_JIF( Write4Byte(  kPacket.m_ulResult,			iCP ), return false );	

	BYTE byteArrayLength = static_cast<BYTE>(kPacket.m_vecProductInfo.size());
	_JIF( WriteByte(   byteArrayLength,				iCP ), return false );

	std::vector< KNXBTCouponProductInfo >::const_iterator vit = kPacket.m_vecProductInfo.begin();
	for( ; vit != kPacket.m_vecProductInfo.end(); ++vit )
	{		
		_JIF( Write( *vit,							iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
//}}

bool KNexonBillingTCPPacket::Read( KENX_BT_COUPON_USING_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read4Byte(  kPacket.m_ulRemoteIP,			iCP ), return false );
    _JIF( ReadByte(   kPacket.m_byteReason,			iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrGameID,			iCP ), return false );
	_JIF( ReadByte(   kPacket.m_byteIsCafe,			iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID,			iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulUserOID,			iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserName,		iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCouponString,	iCP ), return false );
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_COUPON_USING_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte(  kPacket.m_ulRemoteIP,		iCP ), return false );
    _JIF( WriteByte(   kPacket.m_byteReason,		iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrGameID,		iCP ), return false );
	_JIF( WriteByte(   kPacket.m_byteIsCafe,		iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID,		iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulUserOID,			iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserName,		iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCouponString,	iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_COUPON_USING_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrCouponTXID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );

	BYTE byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength, iCP ), return false );

	for( BYTE byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTCouponProductInfo kNXBTCouponProductInfo;
		_JIF( Read( kNXBTCouponProductInfo, iCP ), return false );
		kPacket.m_vecCouponProductInfo.push_back( kNXBTCouponProductInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_COUPON_USING_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( WriteString( kPacket.m_wstrCouponTXID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulOrderNo, iCP ), return false );

	BYTE byteArrayLength = static_cast<BYTE>(kPacket.m_vecCouponProductInfo.size());
	_JIF( WriteByte( byteArrayLength, iCP ), return false );

	std::vector< KNXBTCouponProductInfo >::const_iterator vit = kPacket.m_vecCouponProductInfo.begin();
	for( ; vit != kPacket.m_vecCouponProductInfo.end(); ++vit )
	{
		_JIF( Write( *vit, iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

//{{ 2008. 6. 20  최육사  선물하기
bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_CHECK_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteShowInventory, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteIsPresent, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_CHECK_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteShowInventory, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteIsPresent, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_CHECK_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteOrderType, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulTotalCount, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_CHECK_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteOrderType, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulTotalCount, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
//}}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;	
	_JIF( ReadString( kPacket.m_wstrGameID, iCP ), return false );
    _JIF( ReadByte( kPacket.m_byteShowInventory, iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulPageIndex, iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulRowPerPage, iCP ), return false );
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrGameID, iCP ), return false );
    _JIF( WriteByte( kPacket.m_byteShowInventory, iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulPageIndex, iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulRowPerPage, iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;	
	_JIF( Read4Byte( kPacket.m_ulResult,			iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulTotalCount,		iCP ), return false );

	// KNXBTOrderInfo
	unsigned long ulArrayLength = 0;
	_JIF( Read4Byte( ulArrayLength,					iCP ), return false );

	for( unsigned long ulIndex = 0; ulIndex < ulArrayLength; ++ulIndex )
	{
		KNXBTOrderInfo kNXBTOrderInfo;
		_JIF( Read( kNXBTOrderInfo,					iCP ), return false );
		kPacket.m_vecNXBTOrderInfo.push_back( kNXBTOrderInfo );
	}

	// KNXBTPackageInfo
	ulArrayLength = 0;
	_JIF( Read4Byte( ulArrayLength,					iCP ), return false );

	for( unsigned long ulIndex = 0; ulIndex < ulArrayLength; ++ulIndex )
	{
		KNXBTPackageInfo kNXBTPackageInfo;
		_JIF( Read( kNXBTPackageInfo,				iCP ), return false );
		kPacket.m_vecNXBTPackageInfo.push_back( kNXBTPackageInfo );
	}

	// KNXBTLotteryInfo
	ulArrayLength = 0;
	_JIF( Read4Byte( ulArrayLength,					iCP ), return false );

	for( unsigned long ulIndex = 0; ulIndex < ulArrayLength; ++ulIndex )
	{
		KNXBTLotteryInfo kNXBTLotteryInfo;
		_JIF( Read( kNXBTLotteryInfo,				iCP ), return false );
		kPacket.m_vecNXBTLotteryInfo.push_back( kNXBTLotteryInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulTotalCount, iCP ), return false );

	// KNXBTOrderInfo
	unsigned long ulArrayLength = static_cast<unsigned long>(kPacket.m_vecNXBTOrderInfo.size());
	_JIF( Write4Byte( ulArrayLength, iCP ), return false );

	std::vector< KNXBTOrderInfo >::const_iterator vitOrderInfo = kPacket.m_vecNXBTOrderInfo.begin();
	for( ; vitOrderInfo != kPacket.m_vecNXBTOrderInfo.end(); ++vitOrderInfo )
	{
		_JIF( Write( *vitOrderInfo, iCP ), return false );
	}

	// KNXBTPackageInfo
	ulArrayLength = static_cast<unsigned long>(kPacket.m_vecNXBTPackageInfo.size());
	_JIF( Write4Byte( ulArrayLength, iCP ), return false );

	std::vector< KNXBTPackageInfo >::const_iterator vitPackageInfo = kPacket.m_vecNXBTPackageInfo.begin();
	for( ; vitPackageInfo != kPacket.m_vecNXBTPackageInfo.end(); ++vitPackageInfo )
	{
		_JIF( Write( *vitPackageInfo, iCP ), return false );
	}

	// KNXBTLotteryInfo
	ulArrayLength = static_cast<unsigned long>(kPacket.m_vecNXBTLotteryInfo.size());
	_JIF( Write4Byte( ulArrayLength, iCP ), return false );

	std::vector< KNXBTLotteryInfo >::const_iterator vitLotteryInfo = kPacket.m_vecNXBTLotteryInfo.begin();
	for( ; vitLotteryInfo != kPacket.m_vecNXBTLotteryInfo.end(); ++vitLotteryInfo )
	{
		_JIF( Write( *vitLotteryInfo, iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

//{{ 2008. 6. 26  최육사  선물하기
bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_READABLE_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteShowInventory, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteIsPresent, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_READABLE_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteShowInventory, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteIsPresent, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_READABLE_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteOrderType, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulTotalCount, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_READABLE_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteOrderType, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulTotalCount, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
//}}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_READ_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_READ_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulOrderNo,			iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductNo,		iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_READ_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult,			iCP ), return false );	
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_READ_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult,			iCP ), return false );
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte(  kPacket.m_ulOrderNo,			iCP ), return false );
	_JIF( Read4Byte(  kPacket.m_ulProductNo,		iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Read2Byte(  kPacket.m_usOrderQuantity,	iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte(  kPacket.m_ulOrderNo,			iCP ), return false );
	_JIF( Write4Byte(  kPacket.m_ulProductNo,		iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Write2Byte(  kPacket.m_usOrderQuantity,	iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( Read4Byte( kPacket.m_ulResult,			iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulOrderNo,			iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductNo,			iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Read2Byte(  kPacket.m_usOrderQuantity,	iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( Write4Byte( kPacket.m_ulResult,			iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulOrderNo,			iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductNo,		iCP ), return false );
	//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
	_JIF( Write2Byte(  kPacket.m_usOrderQuantity,	iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
//}}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read4Byte(  kPacket.m_ulOrderNo,			iCP ), return false );
    _JIF( Read4Byte(  kPacket.m_ulProductNo,		iCP ), return false );
	//{{ 2008. 1. 29  최육사  변수 추가
	_JIF( ReadString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte(  kPacket.m_ulOrderNo, iCP ), return false );
    _JIF( Write4Byte(  kPacket.m_ulProductNo, iCP ), return false );
	//{{ 2008. 1. 29  최육사  변수 추가
	_JIF( WriteString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );
	//{{ 2008. 1. 29  최육사  변수 추가
	_JIF( ReadString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulOrderNo, iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulProductNo, iCP ), return false );
	//{{ 2008. 1. 29  최육사  변수 추가
	_JIF( WriteString( kPacket.m_wstrExtendValue,	iCP ), return false );
	//}}
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;
    _JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );
    _JIF( Read2Byte( kPacket.m_usOrderQuantity, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrExtendValue, iCP ), return false );
    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte( kPacket.m_ulOrderNo, iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulProductNo, iCP ), return false );
    _JIF( Write2Byte( kPacket.m_usOrderQuantity, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrExtendValue, iCP ), return false );
    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK& kPacket )
{
    int iCP = 0;
    kPacket.m_ulPacketNo = m_ulPacketNo;
    kPacket.m_bytePacketType = m_bytePacketType;

    _JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );
    _JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );
    _JIF( ReadByte( kPacket.m_byteProductKind, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrProductName, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrProductID, iCP ), return false );
    _JIF( Read2Byte( kPacket.m_usProductExpire, iCP ), return false );
    _JIF( Read2Byte( kPacket.m_usProductPieces, iCP ), return false );
    _JIF( Read2Byte( kPacket.m_usOrderQuantity, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrProductAttribute0, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrProductAttribute1, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrProductAttribute2, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrProductAttribute3, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrProductAttribute4, iCP ), return false );
    _JIF( ReadString( kPacket.m_wstrExtendValue, iCP ), return false );

    unsigned long ulArrayLength;
    ulArrayLength = 0;
    _JIF( Read4Byte( ulArrayLength, iCP ), return false );

    for( unsigned long ulIndex = 0; ulIndex < ulArrayLength; ++ulIndex )
    {
        KNXBTSubProductInfo kNXBTSubProductInfo;
        _JIF( Read( kNXBTSubProductInfo, iCP ), return false );
        kPacket.m_vecNXBTSubProductInfo.push_back( kNXBTSubProductInfo );
    }

	ulArrayLength = 0;
	_JIF( Read4Byte( ulArrayLength, iCP ), return false );

	for( unsigned long ulIndex = 0; ulIndex < ulArrayLength; ++ulIndex )
	{
		KNXBTBonusProductInfoPickUp kNXBTBonusProductInfoPickUp;
		_JIF( Read( kNXBTBonusProductInfoPickUp, iCP ), return false );
		kPacket.m_vecKNXBTBonusProductInfo.push_back( kNXBTBonusProductInfoPickUp );
	}

    LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

    return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK& kPacket )
{
    int iCP = 0;
    m_ulPacketNo = kPacket.m_ulPacketNo;
    m_bytePacketType = kPacket.m_bytePacketType;
    _JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulOrderNo, iCP ), return false );
    _JIF( Write4Byte( kPacket.m_ulProductNo, iCP ), return false );
    _JIF( WriteByte( kPacket.m_byteProductKind, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrProductName, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrProductID, iCP ), return false );
    _JIF( Write2Byte( kPacket.m_usProductExpire, iCP ), return false );
    _JIF( Write2Byte( kPacket.m_usProductPieces, iCP ), return false );
    _JIF( Write2Byte( kPacket.m_usOrderQuantity, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrProductAttribute0, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrProductAttribute1, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrProductAttribute2, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrProductAttribute3, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrProductAttribute4, iCP ), return false );
    _JIF( WriteString( kPacket.m_wstrExtendValue, iCP ), return false );

    unsigned long ulArrayLength;
    ulArrayLength = static_cast< unsigned long >( kPacket.m_vecNXBTSubProductInfo.size() );
    _JIF( Write4Byte( ulArrayLength, iCP ), return false );

    std::vector< KNXBTSubProductInfo >::const_iterator vit = kPacket.m_vecNXBTSubProductInfo.begin();
    for( ; vit != kPacket.m_vecNXBTSubProductInfo.end(); ++vit )
    {
        _JIF( Write( *vit, iCP ), return false );
    }

	ulArrayLength = static_cast< unsigned long >( kPacket.m_vecKNXBTBonusProductInfo.size() );
	_JIF( Write4Byte( ulArrayLength, iCP ), return false );

	std::vector< KNXBTBonusProductInfoPickUp >::const_iterator vit2 = kPacket.m_vecKNXBTBonusProductInfo.begin();
	for( ; vit2 != kPacket.m_vecKNXBTBonusProductInfo.end(); ++vit2 )
	{
		_JIF( Write( *vit2, iCP ), return false );
	}

    m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

    return true;
}

//{{ 2008. 6. 16  최육사  넥슨 빌링 패키지 픽업
bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulPackageProductNo, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );	

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulOrderNo, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulPackageProductNo, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductNo, iCP ), return false );	

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulPackageProductNo, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );	

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulOrderNo, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulPackageProductNo, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductNo, iCP ), return false );	

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
//}}

//{{ 2010. 11. 17	최육사	넥슨 빌링 청약 철회
#ifdef SERV_NX_BILLING_REFUND
bool KNexonBillingTCPPacket::Read( KENX_BT_REFUND_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulRemoteIP, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrRequestID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulOrderNo, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductNo, iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usQuantity, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_REFUND_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulRemoteIP, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrRequestID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGameID, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulOrderNo, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductNo, iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usQuantity, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_REFUND_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulResult, iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_REFUND_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulResult, iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
#endif SERV_NX_BILLING_REFUND
//}}

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-21	// 박세훈
bool KNexonBillingTCPPacket::Read( KDiscountCoupon& kDiscountCoupon, int& iCP )
{
	_JIF( Read4Byte(	kDiscountCoupon.m_iCouponCardNo,			iCP ), return false );
	_JIF( ReadString(	kDiscountCoupon.m_wstrContractDetailName,	iCP ), return false );
	_JIF( ReadString(	kDiscountCoupon.m_wstrDiscountValue,		iCP ), return false );
	_JIF( ReadString(	kDiscountCoupon.m_wstrExpireDateTime,		iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Write( const KDiscountCoupon& kDiscountCoupon, int& iCP )
{
	_JIF( Write4Byte(	kDiscountCoupon.m_iCouponCardNo,			iCP ), return false );
	_JIF( WriteString(	kDiscountCoupon.m_wstrContractDetailName,	iCP ), return false );
	_JIF( WriteString(	kDiscountCoupon.m_wstrDiscountValue,		iCP ), return false );
	_JIF( WriteString(	kDiscountCoupon.m_wstrExpireDateTime,		iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Read( KDiscountCouponInquriyInfo& kDiscountCouponInquriyInfo, int& iCP )
{
	_JIF( Read4Byte(	kDiscountCouponInquriyInfo.m_ulProductNo,		iCP ), return false );
	_JIF( Read2Byte(	kDiscountCouponInquriyInfo.m_usOrderQuantity,	iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Write( const KDiscountCouponInquriyInfo& kDiscountCouponInquriyInfo, int& iCP )
{
	_JIF( Write4Byte(	kDiscountCouponInquriyInfo.m_ulProductNo,		iCP ), return false );
	_JIF( Write2Byte(	kDiscountCouponInquriyInfo.m_usOrderQuantity,	iCP ), return false );
	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_ITEM_COUPON_USING_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulRemoteIP,		iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteReason,		iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrGameID,		iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteIsCafe,		iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID,		iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulUserOID,		iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserName,	iCP ), return false );
	_JIF( Read4Byte( kPacket.m_iCouponCardNo,	iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_ITEM_COUPON_USING_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulRemoteIP,		iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteReason,		iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrGameID,	iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteIsCafe,		iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID,	iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulUserOID,		iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserName,	iCP ), return false );
	_JIF( Write4Byte( kPacket.m_iCouponCardNo,	iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_ITEM_COUPON_USING_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadString( kPacket.m_wstrCouponTXID,	iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulResult,		iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulOrderNo,		iCP ), return false );

	byte byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength, iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KNXBTCouponProductInfo kNXBTCouponProductInfo;
		_JIF( Read( kNXBTCouponProductInfo,		iCP	), return false );
		kPacket.m_vecCouponProductInfo.push_back( kNXBTCouponProductInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_ITEM_COUPON_USING_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteString( kPacket.m_wstrCouponTXID,				iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulResult,						iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulOrderNo,						iCP ), return false );

	byte byteArrayLength = static_cast<byte>( kPacket.m_vecCouponProductInfo.size() );
	_JIF( WriteByte( byteArrayLength,							iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		_JIF( Write( kPacket.m_vecCouponProductInfo[byteIndex],	iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulRemoteIP,		iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID,		iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulUserOID,		iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulProductNo,		iCP ), return false );
	_JIF( Read2Byte( kPacket.m_usOrderQuantity,	iCP ), return false );

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulRemoteIP,			iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID,		iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulUserOID,			iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulProductNo,		iCP ), return false );
	_JIF( Write2Byte( kPacket.m_usOrderQuantity,	iCP ), return false );

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo		= m_ulPacketNo;
	kPacket.m_bytePacketType	= m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulResult,	iCP ), return false );

	byte byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,		iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KDiscountCoupon kDiscountCoupon;
		_JIF( Read( kDiscountCoupon,		iCP ), return false );
		kPacket.m_vecDiscountCoupon.push_back( kDiscountCoupon );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulResult,						iCP ), return false );

	byte byteArrayLength = static_cast<byte>( kPacket.m_vecDiscountCoupon.size() );
	_JIF( WriteByte( byteArrayLength,							iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		_JIF( Write( kPacket.m_vecDiscountCoupon[byteIndex],	iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo = m_ulPacketNo;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulRemoteIP,		iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID,		iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulUserOID,		iCP ), return false );
	
	byte byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,			iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KDiscountCouponInquriyInfo kDiscountCouponInquriyInfo;
		_JIF( Read( kDiscountCouponInquriyInfo,	iCP ), return false );
		kPacket.m_vecDiscountCouponInquiryInfo.push_back( kDiscountCouponInquriyInfo );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulRemoteIP,						iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID,					iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulUserOID,						iCP ), return false );
	
	byte byteArrayLength = static_cast<byte>( kPacket.m_vecDiscountCouponInquiryInfo.size() );
	_JIF( WriteByte( byteArrayLength,							iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		_JIF( Write( kPacket.m_vecDiscountCouponInquiryInfo[byteIndex],	iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}

bool KNexonBillingTCPPacket::Read( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_ulPacketNo		= m_ulPacketNo;
	kPacket.m_bytePacketType	= m_bytePacketType;

	_JIF( Read4Byte( kPacket.m_ulResult,	iCP ), return false );

	byte byteArrayLength = 0;
	_JIF( ReadByte( byteArrayLength,		iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		KDiscountCoupon kDiscountCoupon;
		_JIF( Read( kDiscountCoupon,		iCP ), return false );
		kPacket.m_vecDiscountCoupon.push_back( kDiscountCoupon );
	}

	LIF( iCP == m_ulPacketLength - HEADER_IN_PACKET_LENGTH_NBT );

	return true;
}

bool KNexonBillingTCPPacket::Write( const KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK& kPacket )
{
	int iCP = 0;
	m_ulPacketNo = kPacket.m_ulPacketNo;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( Write4Byte( kPacket.m_ulResult,						iCP ), return false );

	byte byteArrayLength = static_cast<byte>( kPacket.m_vecDiscountCoupon.size() );
	_JIF( WriteByte( byteArrayLength,							iCP ), return false );

	for( byte byteIndex = 0; byteIndex < byteArrayLength; ++byteIndex )
	{
		_JIF( Write( kPacket.m_vecDiscountCoupon[byteIndex],	iCP ), return false );
	}

	m_ulPacketLength = iCP + HEADER_IN_PACKET_LENGTH_NBT;

	return true;
}
#endif // SERV_NEXON_COUPON_SYSTEM
