#include "BRPayletterBillingPacket.h"

#ifdef SERV_COUNTRY_BR

#include "_BRGameProtocol.h"

KBRPayletterBillingPacket::KBRPayletterBillingPacket()
{
	m_usReqLen = 0;
	m_usReqType = 0;
	m_usRetCode = 0;
}

KBRPayletterBillingPacket::~KBRPayletterBillingPacket()
{
}

bool KBRPayletterBillingPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	::memcpy( &m_usReqLen, pbyteBuffer,  sizeof( WORD ) );
	::memcpy( &m_usReqType, pbyteBuffer + 2,  sizeof( WORD ) );
	::memcpy( &m_usRetCode, pbyteBuffer + 4,  sizeof( WORD ) );

	if( m_usReqLen > HEADER_IN_PACKET_LENGTH_PLB )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 6,  m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB );
	}

	return true;
}

bool KBRPayletterBillingPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	_JIF( m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB <= MAX_PACKET_CONTENT_SIZE_PLB, return false );
 	::memcpy( pbyteBuffer, &m_usReqLen, sizeof( WORD ) );
 	::memcpy( pbyteBuffer + 2, &m_usReqType, sizeof( WORD ) );
 	::memcpy( pbyteBuffer + 4, &m_usRetCode, sizeof( WORD ) );

	if( m_usReqLen > HEADER_IN_PACKET_LENGTH_PLB )
 	{
 		::memcpy( pbyteBuffer + 6, m_abytePacketContent, m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB );
 	}

	return true;
}

bool KBRPayletterBillingPacket::ReadByte( BYTE& byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( BYTE ) )
	{
		return false;
	}

	BYTE byteDataTemp;
	::memcpy( &byteDataTemp, m_abytePacketContent + iCP, sizeof( BYTE ) );
	byteData = byteDataTemp;
	iCP = iCP + ( int )sizeof( BYTE );

	return true;
}

bool KBRPayletterBillingPacket::Read2Byte( unsigned short& usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned short ) )
	{
		return false;
	}

	unsigned short usDataTemp;
	::memcpy( &usDataTemp, m_abytePacketContent + iCP, sizeof( unsigned short ) );
	usData = usDataTemp;
	iCP = iCP + ( int )sizeof( unsigned short );
	return true;
}

//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
bool KBRPayletterBillingPacket::Read2ByteSigned( short& sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( short ) )
	{
		return false;
	}

	short sDataTemp;
	::memcpy( &sDataTemp, m_abytePacketContent + iCP, sizeof( short ) );
	sData = sDataTemp;
	iCP = iCP + ( int )sizeof( short );
	return true;
}
//}}

bool KBRPayletterBillingPacket::Read4Byte( unsigned long& ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned long ) )
	{
		return false;
	}

	unsigned long ulDataTemp;
	::memcpy( &ulDataTemp, m_abytePacketContent + iCP, sizeof( unsigned long ) );
	ulData = ulDataTemp;
	iCP = iCP + ( int )sizeof( unsigned long );
	return true;
}

bool KBRPayletterBillingPacket::Read8Byte( __int64& iData, int& iCP )	// 4바이트씩 바꾸지 않는다.
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( __int64 ) )
	{
		return false;
	}

	__int64 iDataTemp;
	::memcpy( &iDataTemp, m_abytePacketContent + iCP, sizeof( __int64 ) );
	iData = iDataTemp;
	iCP = iCP + ( int )sizeof( __int64 );
	return true;
}

bool KBRPayletterBillingPacket::ReadString( std::wstring& wstrData, int& iCP )
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

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )usLength )
	{
		return false;
	}

	char szData[2050];
	::memcpy( szData, m_abytePacketContent + iCP, ( size_t )usLength );
	szData[usLength] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + ( int )usLength;
	return true;
}

bool KBRPayletterBillingPacket::ReadString( std::wstring& wstrData, int size, int& iCP )
{
	if( size == 0 )
	{
		wstrData.clear();
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )size )
	{
		return false;
	}

	char szData[2050];
	::memcpy( szData, m_abytePacketContent + iCP, ( size_t )size );
	//szData[size] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + ( int )size;
	return true;
}

bool KBRPayletterBillingPacket::ReadString( char* cpData, int size, int& iCP )
{
	if( size == 0 )
	{
		cpData = NULL;
		return true;
	}

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )size )
	{
		return false;
	}

	::memcpy( cpData, m_abytePacketContent + iCP, ( size_t )size );

	iCP = iCP + ( int )size;
	return true;
}

bool KBRPayletterBillingPacket::WriteByte( BYTE byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( BYTE ) )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &byteData, sizeof( BYTE ) );
	iCP = iCP + ( int )sizeof( BYTE );
	return true;
}

bool KBRPayletterBillingPacket::Write2Byte( unsigned short usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned short ) )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &usData, sizeof( unsigned short ) );
	iCP = iCP + ( int )sizeof( unsigned short );
	return true;
}

//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
bool KBRPayletterBillingPacket::Write2ByteSigned( short sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( short ) )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &sData, sizeof( short ) );
	iCP = iCP + ( int )sizeof( short );
	return true;
}
//}}

bool KBRPayletterBillingPacket::Write4Byte( unsigned long ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned long ) )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &ulData, sizeof( unsigned long ) );
	iCP = iCP + ( int )sizeof( unsigned long );
	return true;
}

bool KBRPayletterBillingPacket::Write8Byte( const __int64& iData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( __int64 ) )
	{
		return false;
	}

	__int64 iDataTemp = iData;
	unsigned long aulDataTemp[2];

	::memcpy( aulDataTemp, &iDataTemp, sizeof( __int64 ) );
	
	unsigned long ulTemp = aulDataTemp[0];
	aulDataTemp[0] = aulDataTemp[1];
	aulDataTemp[1] = ulTemp;

	::memcpy( m_abytePacketContent + iCP, aulDataTemp, sizeof( __int64 ) );
	iCP = iCP + ( int )sizeof( __int64 );
	return true;
}

bool KBRPayletterBillingPacket::WriteString( const std::wstring& wstrData, int& iCP )
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

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )usLength )
	{
		return false;
	}

	char szData[MAX_PATH];
	::strncpy( szData, strData.c_str(), MAX_PATH );

	::memcpy( m_abytePacketContent + iCP, szData, ( size_t )usLength );
	iCP = iCP + ( int )usLength;
	return true;
}

// 숫자를 스트링으로~
std::string KBRPayletterBillingPacket::toString(int iSource)
{
	char szData[20];
	::itoa( iSource, szData, 10 );

	std::string str = szData;
	return str;
}

bool KBRPayletterBillingPacket::Read( KEPL_BR_BALANCE_CHECK_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_usReqLen = m_usReqLen;	// IN	1
	kPacket.m_usReqType = m_usReqType;	// IN	2
	kPacket.m_usRetCode = m_usRetCode;	// OUT	3

	__int64 iPublisherSN = 0;
	_JIF( Read8Byte( iPublisherSN,  iCP ), return false );
	kPacket.m_ulUserNo = iPublisherSN;
	_JIF( ReadString( kPacket.m_wstrUserID,  50+1, iCP ), return false );

	_JIF( Read4Byte( kPacket.m_ulRealCash,  iCP ), return false );
	kPacket.m_ulBonusCash = 0;

	LIF( iCP == m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB );

	return true;
}

bool KBRPayletterBillingPacket::Write( const KEPL_BR_BALANCE_CHECK_REQ& kPacket )
{
	BILL_PACK_BALANCE tempPacket = {0};

	m_usReqLen = sizeof(BILL_PACK_BALANCE);
	m_usReqType = kPacket.m_usReqType;
	m_usRetCode = 0;

	tempPacket.GUserNo = kPacket.m_ulUserNo;

	std::string strData = KncUtil::toNarrowString( kPacket.m_wstrUserID );
	::strncpy(tempPacket.GUserID, strData.c_str(), 50);
	tempPacket.CashRemain = 0;

	::memcpy(m_abytePacketContent, ((char*)&tempPacket) + HEADER_IN_PACKET_LENGTH_PLB, sizeof(BILL_PACK_BALANCE) - HEADER_IN_PACKET_LENGTH_PLB);

	return true;
}

bool KBRPayletterBillingPacket::Read( KEPL_BR_PURCHASEITEM_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_usReqLen = m_usReqLen;	// IN	1
	kPacket.m_usReqType = m_usReqType;	// IN	2
	kPacket.m_usRetCode = m_usRetCode;	// OUT	3

	__int64 iPublisherSN = 0;
	_JIF( Read8Byte( iPublisherSN,  iCP ), return false );
	kPacket.m_ulReqKey = iPublisherSN;
	_JIF( ReadString( kPacket.m_wstrUserID,  50+1, iCP ), return false );
	unsigned long CashRemain = 0;
	_JIF( Read4Byte( CashRemain,  iCP ), return false );

	std::wstring ChargeNo;
	_JIF( ReadString( ChargeNo,  512+1, iCP ), return false );

	int nStart = 0;
	char strData[MAX_PATH];
	char cItemID[MAX_PATH];
	int iProductNo;

	_JIF( ReadString( cItemID,  256+1, iCP ), return false );

	for(int i = 0; i< sizeof(cItemID); i++)
	{
		if(cItemID[i] == ',' || cItemID[i] == '\0')
		{
			::memcpy(strData, ((char*)&cItemID) + nStart,  (i - nStart));
			strData[i] = '\0';			
			nStart = i+1;			
			iProductNo = atoi(strData);
			if(iProductNo == 51)
			{
				kPacket.m_bResurrectionStone = true;
				break;
			}

			if(cItemID[i] == '\0')
			{
				break;
			}
		}
	}

	std::wstring ItemName;
	_JIF( ReadString( ItemName,  2048+1, iCP ), return false );

	std::wstring ItemPrice;
	_JIF( ReadString( ItemPrice,  256+1, iCP ), return false );
	
	return true;
}

bool KBRPayletterBillingPacket::Write( const KEBILL_BUY_PRODUCT_REQ& kPacket )
{
	BILL_PACK_BUY tempPacket = {0};
	
	m_usReqLen = sizeof(BILL_PACK_BUY);
	m_usReqType = GTX_PK_TYPE_PURCHASEITEM;
	m_usRetCode = 0;

	tempPacket.GUserNo = kPacket.m_uiPublisherUID;
	std::string strUserID = KncUtil::toNarrowString( kPacket.m_wstrUserID );
	::strncpy(tempPacket.GUserID, strUserID.c_str(), 50);

	tempPacket.CashRemain = 0;
	::strncpy(tempPacket.ChargeNos, "", 512);

	int addSizeID = 0;
	int addSizeName = 0;
	int addSizePrice = 0;

	for(int i = 0; i < kPacket.m_vecBillBuyInfo.size(); i++)
	{
		// 아이디
		std::string strItemID = toString(kPacket.m_vecBillBuyInfo[i].m_iProductNo);
		if(i != 0)
		{
			tempPacket.ItemIDs[addSizeID] = ',';
			addSizeID++;
		}
		::memcpy(tempPacket.ItemIDs + addSizeID, strItemID.c_str(), strItemID.size()); 
		addSizeID += strItemID.size();
		tempPacket.ItemIDs[addSizeID] = '\0';

		// 이름
		std::string strItemName = KncUtil::toNarrowString( kPacket.m_vecBillBuyInfo[i].m_wstrMemo );
		if(i != 0)
		{
			tempPacket.ItemNames[addSizeName] = ',';
			addSizeName++;
		}
		::memcpy(tempPacket.ItemNames + addSizeName, strItemName.c_str(), strItemName.size()); 
		addSizeName += strItemName.size();
		tempPacket.ItemNames[addSizeName] = '\0';

		// 가격
		std::string strItemPrice = toString((int)kPacket.m_vecBillBuyInfo[i].m_iPoint);
		if(i != 0)
		{
			tempPacket.ItemUnitPrices[addSizePrice] = ',';
			addSizePrice++;
		}
		::memcpy(tempPacket.ItemUnitPrices + addSizePrice, strItemPrice.c_str(), strItemPrice.size()); 
		addSizePrice += strItemPrice.size();
		tempPacket.ItemUnitPrices[addSizePrice] = '\0';

		START_LOG( clog2, L"BillingPrice" )
			<< BUILD_LOG( strItemPrice )
			<< BUILD_LOG( addSizePrice )
			<< BUILD_LOG( tempPacket.ItemUnitPrices )
			<< END_LOG;
	}

	// 마무리로 해더(6byte) 빼고 버퍼에 다 복사
	::memcpy(m_abytePacketContent, ((char*)&tempPacket) + HEADER_IN_PACKET_LENGTH_PLB,  sizeof(BILL_PACK_BUY) - HEADER_IN_PACKET_LENGTH_PLB);

	START_LOG( clog2, L"BillingPrice" )
		<< BUILD_LOG( m_abytePacketContent )
		<< END_LOG;

	return true;
}

#endif SERV_COUNTRY_BR