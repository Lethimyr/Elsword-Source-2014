#include "PayletterBillingPacket.h"

#ifdef SERV_COUNTRY_US

#include "_GameProtocol.h"

KPayletterBillingPacket::KPayletterBillingPacket()
{
	m_usReqLen = 0;
	m_usReqType = 0;
	m_ulReqKey = 0;

	m_usRetCode = 0;
}

KPayletterBillingPacket::~KPayletterBillingPacket()
{
}

bool KPayletterBillingPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	::memcpy( &m_usReqLen, pbyteBuffer,  sizeof( unsigned short ) );
	//unsigned long ulPacketLength = ::htons( m_usReqLen );
	::memcpy( &m_usReqType, pbyteBuffer + 2,  sizeof( unsigned short ) );
	//unsigned long ulPacketNo = ::htonl( m_ulReqKey );
	::memcpy( &m_ulReqKey, pbyteBuffer + 4,  sizeof( unsigned long ) );
	::memcpy( &m_usRetCode, pbyteBuffer + 8,  sizeof( unsigned short ) );
	if( m_usReqLen > HEADER_IN_PACKET_LENGTH_PLB )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 10,  m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB );
	}

	return true;
}

bool KPayletterBillingPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

	_JIF( m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB <= MAX_PACKET_CONTENT_SIZE_PLB, return false );
	::memcpy( pbyteBuffer, &m_usReqLen, sizeof( unsigned short ) );
	//unsigned long ulPacketLength = ::htons( m_usReqLen );
	::memcpy( pbyteBuffer + 2, &m_usReqType, sizeof( unsigned short ) );
	//unsigned long ulPacketNo = ::htonl( m_ulReqKey );
	::memcpy( pbyteBuffer + 4, &m_ulReqKey, sizeof( unsigned long ) );
	::memcpy( pbyteBuffer + 8, &m_usRetCode, sizeof( unsigned short ) );
	if( m_usReqLen > HEADER_IN_PACKET_LENGTH_PLB )
	{
		::memcpy( pbyteBuffer + 10, m_abytePacketContent, m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB );
	}

	return true;
}

bool KPayletterBillingPacket::ReadByte( BYTE& byteData, int& iCP )
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

bool KPayletterBillingPacket::Read2Byte( unsigned short& usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned short ) )
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
bool KPayletterBillingPacket::Read2ByteSigned( short& sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( short ) )
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

bool KPayletterBillingPacket::Read4Byte( unsigned long& ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned long ) )
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

bool KPayletterBillingPacket::Read8Byte( __int64& iData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( __int64 ) )
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

bool KPayletterBillingPacket::ReadString( std::wstring& wstrData, int& iCP )
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

	char szData[MAX_PATH];
	::memcpy( szData, m_abytePacketContent + iCP, ( size_t )usLength );
	szData[usLength] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + ( int )usLength;
	return true;
}

bool KPayletterBillingPacket::ReadString( std::wstring& wstrData, int size, int& iCP )
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

	char szData[MAX_PATH];
	::memcpy( szData, m_abytePacketContent + iCP, ( size_t )size );
	//szData[size] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + ( int )size;
	return true;
}

bool KPayletterBillingPacket::WriteByte( BYTE byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( BYTE ) )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &byteData, sizeof( BYTE ) );
	iCP = iCP + ( int )sizeof( BYTE );
	return true;
}

bool KPayletterBillingPacket::Write2Byte( unsigned short usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned short ) )
	{
		return false;
	}

	//usData = ::htons( usData );
	::memcpy( m_abytePacketContent + iCP, &usData, sizeof( unsigned short ) );
	iCP = iCP + ( int )sizeof( unsigned short );
	return true;
}

//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
bool KPayletterBillingPacket::Write2ByteSigned( short sData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( short ) )
	{
		return false;
	}

	//sData = ::htons( sData );
	::memcpy( m_abytePacketContent + iCP, &sData, sizeof( short ) );
	iCP = iCP + ( int )sizeof( short );
	return true;
}
//}}

bool KPayletterBillingPacket::Write4Byte( unsigned long ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( unsigned long ) )
	{
		return false;
	}

	//ulData = ::htonl( ulData );
	::memcpy( m_abytePacketContent + iCP, &ulData, sizeof( unsigned long ) );
	iCP = iCP + ( int )sizeof( unsigned long );
	return true;
}

bool KPayletterBillingPacket::Write8Byte( const __int64& iData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_PLB - ( int )sizeof( __int64 ) )
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

bool KPayletterBillingPacket::WriteString( const std::wstring& wstrData, int& iCP )
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
std::string KPayletterBillingPacket::toString(int iSource)
{
	char szData[20];
	::itoa( iSource, szData, 10 );

	std::string str = szData;
	return str;
}

bool KPayletterBillingPacket::Read( KEPL_BT_HEALTH_CHECK_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_usReqLen = m_usReqLen;		// IN	1
	kPacket.m_usReqType = m_usReqType;	// IN	2
	kPacket.m_ulReqKey = m_ulReqKey;		// IN	3

	kPacket.m_usRetCode = m_usRetCode;	// OUT	4

	return true;
}

bool KPayletterBillingPacket::Write( const KEPL_BT_HEALTH_CHECK_REQ& kPacket )
{
	int iCP = 0;
	m_usReqLen = kPacket.m_usReqLen;		// IN	1
	m_usReqType = kPacket.m_usReqType;	// IN	2
	m_ulReqKey = kPacket.m_ulReqKey;		// IN	3

	m_usRetCode = kPacket.m_usRetCode;	// OUT	4
	
	return true;
}

bool KPayletterBillingPacket::Read( KEPL_BT_BALANCE_CHECK_ACK& kPacket )
{

	int iCP = 0;
	kPacket.m_usReqLen = m_usReqLen;		// IN	1
	kPacket.m_usReqType = m_usReqType;	// IN	2
	kPacket.m_ulReqKey = m_ulReqKey;		// IN	3
	kPacket.m_usRetCode = m_usRetCode;	// OUT	4


	_JIF( Read4Byte( kPacket.m_ulUserNo,  iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID,  50+1, iCP ), return false );

	_JIF( Read4Byte( kPacket.m_ulRealCash,  iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulBonusCash,  iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrRetMsg,  256+1, iCP ), return false );

	LIF( iCP == m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB );

	//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
	PGTX_PK_GIFTITEM tempPacket;
	tempPacket = (PGTX_PK_GIFTITEM)&m_usReqLen;
	char strData[256];

	for( int i = 0; i< sizeof( tempPacket->RetMsg ); i++ )
	{
		if( tempPacket->RetMsg[i] == '\0' )
		{
			::memcpy( strData, ( (char*)&tempPacket->RetMsg ), i );
			strData[i] = '\0';

			break;
		}
	}
	kPacket.m_wstrRetMsg = KncUtil::toWideString( strData );
	//}}

	return true;
}

bool KPayletterBillingPacket::Write( const KEPL_BT_BALANCE_CHECK_REQ& kPacket )
{
	/*
	int iCP = 0;
	m_usReqLen = kPacket.m_usReqLen;		// IN	1
	m_usReqType = kPacket.m_usReqType;	// IN	2
	m_ulReqKey = kPacket.m_ulReqKey;		// IN	3
	m_usRetCode = kPacket.m_usRetCode;	// OUT	4

	_JIF( Write4Byte( kPacket.m_ulUserNo,  iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID,  iCP ), return false );

	_JIF( Write4Byte( 0,  iCP ), return false );
	_JIF( Write4Byte( 0,  iCP ), return false );
	_JIF( WriteString( L"",  iCP ), return false );

	m_usReqLen = iCP + HEADER_IN_PACKET_LENGTH_PLB;
*/
	// 지헌 : 어라? 굳이 이럴 필요 있나?
	// 그냥 패킷 구조체로 처리해보자
	GTX_PK_GETBALANCE tempPacket;
	m_usReqLen = tempPacket.ReqLen = sizeof(GTX_PK_GETBALANCE);
	m_usReqType = tempPacket.ReqType = kPacket.m_usReqType;
	m_ulReqKey = tempPacket.ReqKey = kPacket.m_ulReqKey;
	m_usRetCode = tempPacket.RetCode = kPacket.m_usRetCode;

	tempPacket.UserNo = kPacket.m_ulUserNo;
	std::string strData = KncUtil::toNarrowString( kPacket.m_wstrUserID );
	::strncpy(tempPacket.UserID, strData.c_str(), 50);

	tempPacket.RealCash = 0;
	tempPacket.BonusCash = 0;
	//strData = KncUtil::toNarrowString( kPacket.m_wstrRetMsg );
	::strncpy(tempPacket.RetMsg, "", 256);

	::memcpy(m_abytePacketContent, ((char*)&tempPacket) + HEADER_IN_PACKET_LENGTH_PLB, sizeof(GTX_PK_GETBALANCE) - HEADER_IN_PACKET_LENGTH_PLB);

	return true;
}

bool KPayletterBillingPacket::Read( KEPL_BT_PURCHASEITEM_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_usReqLen = m_usReqLen;		// IN	1
	kPacket.m_usReqType = m_usReqType;	// IN	2
	kPacket.m_ulReqKey = m_ulReqKey;		// IN	3
	kPacket.m_usRetCode = m_usRetCode;	// OUT	4

	// 여기서 오류 처리는 어떻게 해야 할까?

	PGTX_PK_PURCHASEITEM tempPacket;
	tempPacket = (PGTX_PK_PURCHASEITEM)&m_usReqLen;
	// 받는 패킷에서 필요한 정보는 유저 UID.
	// 나머지는 필요 없는데...패킷을 바꿔야 하나?
	kPacket.m_iUserUID = (UidType)tempPacket->ReqKey;
	kPacket.m_bResurrectionStone = false;

	int nStart = 0;
	char strData[256];
	int iProductNo;
	for(int i = 0; i< sizeof(tempPacket->ItemID); i++)
	{
		if(tempPacket->ItemID[i] == 0x11 || tempPacket->ItemID[i] == '\0')
		{
			::memcpy(strData, ((char*)&tempPacket->ItemID) + nStart,  (i - nStart));
			strData[i] = '\0';			
			nStart = i+1;			
			iProductNo = atoi(strData);
			if(iProductNo == 51)
			{
				kPacket.m_bResurrectionStone = true;
				break;
			}

			if(tempPacket->ItemID[i] == '\0')
			{
				break;
			}
		}
	}

	//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
	for( int i = 0; i< sizeof( tempPacket->RetMsg ); i++ )
	{
		if( tempPacket->RetMsg[i] == '\0' )
		{
			::memcpy( strData, ( (char*)&tempPacket->RetMsg ), i );
			strData[i] = '\0';

			break;
		}
	}
	kPacket.m_wstrRetMsg = KncUtil::toWideString( strData );
	//}}

	return true;
}

bool KPayletterBillingPacket::Write( const KEBILL_BUY_PRODUCT_REQ& kPacket )
{
	GTX_PK_PURCHASEITEM tempPacket;
	m_usReqLen = tempPacket.ReqLen = sizeof(GTX_PK_PURCHASEITEM);
	m_usReqType = tempPacket.ReqType = GTX_PK_TYPE_PURCHASEITEM;
	m_ulReqKey = tempPacket.ReqKey = (int)kPacket.m_iUserUID;// 패킷 넘버는 필요 없는 듯.
	m_usRetCode = tempPacket.RetCode = 0;

	tempPacket.UserNo =  _wtoi(kPacket.m_wstrUserID.c_str());//0;//(int)kPacket.m_iUnitUID;
	tempPacket.ClientIP = inet_addr( kPacket.m_strUserIP.c_str());

	int addSizeID = 0;
	int addSizeCnt = 0;
	int addSizePrice = 0;

	for(int i = 0; i < kPacket.m_vecBillBuyInfo.size(); i++)
	{
		// 이름
		std::string strItemID = toString(kPacket.m_vecBillBuyInfo[i].m_iProductNo);
		if(i != 0)
		{
			tempPacket.ItemID[addSizeID] = 0x11;
			addSizeID++;
		}
		::memcpy(tempPacket.ItemID + addSizeID, strItemID.c_str(), strItemID.size()); 
		addSizeID += strItemID.size();
		tempPacket.ItemID[addSizeID] = '\0';

		// 구매 개수
		std::string strItemCnt = "1";//toString(kPacket.m_vecBillBuyInfo[i].m_usOrderQuantity);
		if(i != 0)
		{
			tempPacket.ItemCnt[addSizeCnt] = 0x11;
			addSizeCnt++;
		}
		::memcpy(tempPacket.ItemCnt + addSizeCnt, strItemCnt.c_str(), strItemCnt.size()); 
		addSizeCnt += strItemCnt.size();
		tempPacket.ItemCnt[addSizeCnt] = '\0';

		// 가격
		std::string strItemPrice = toString((int)kPacket.m_vecBillBuyInfo[i].m_iPoint);
		if(i != 0)
		{
			tempPacket.ItemUnitPrice[addSizePrice] = 0x11;
			addSizePrice++;
		}
		::memcpy(tempPacket.ItemUnitPrice + addSizePrice, strItemPrice.c_str(), strItemPrice.size()); 
		addSizePrice += strItemPrice.size();
		tempPacket.ItemUnitPrice[addSizePrice] = '\0';


		START_LOG( clog2, L"BillingPrice" )
			<< BUILD_LOG( strItemPrice )
			<< BUILD_LOG( addSizePrice )
			<< BUILD_LOG( tempPacket.ItemUnitPrice )
			<< END_LOG;
	}


	tempPacket.GameServerNo = kPacket.m_iServerGroupID;
	tempPacket.WorldNo = 0;

	//std::string strData = KncUtil::toNarrowString( kPacket.m_wstrUserID );
	//::strncpy(tempPacket.UserID, strData.c_str(), 50);
	::strncpy(tempPacket.UserID, "", 50);

	std::string strData = KncUtil::toNarrowString( kPacket.m_wstrNickName );
	::strncpy(tempPacket.CharacterID, strData.c_str(), 50);

	char szLevel[4];
	::itoa( kPacket.m_BuyCharInfo.m_cLevel, szLevel, 10 );
	//::strncpy(tempPacket.StatProperty1, "", 50);
	::strncpy(tempPacket.StatProperty1, szLevel, 50);

	char szUnitClass[128];
	::_itoa( kPacket.m_BuyCharInfo.m_iUnitClass, szUnitClass, 10 );
	strData = szUnitClass;
	::strncpy(tempPacket.StatProperty2, strData.c_str(), 50);

	::strncpy(tempPacket.StatProperty3, "", 50);

	START_LOG( clog2, L"캐릭터 정보 확인" )
		<< BUILD_LOG( tempPacket.StatProperty1 )
		<< BUILD_LOG( tempPacket.StatProperty2 )		
		<< END_LOG;

	tempPacket.RealCash = 0;
	tempPacket.BonusCash = 0;
	tempPacket.ChargedCashAmt = 0;

	// 지헌 : 과금 번호, 이벤트 등등인데 보낼때는 "" 로
	::strncpy(tempPacket.ChargeNo, "", 512);
	::strncpy(tempPacket.EventItemID, "", 256);
	::strncpy(tempPacket.EventChargeNo, "", 512);

	::strncpy(tempPacket.RetMsg, "", 256);


	// 마무리로 해더(10byte) 빼고 버퍼에 다 복사
	::memcpy(m_abytePacketContent, ((char*)&tempPacket) + HEADER_IN_PACKET_LENGTH_PLB,  sizeof(GTX_PK_PURCHASEITEM) - HEADER_IN_PACKET_LENGTH_PLB);

	START_LOG( clog2, L"BillingPrice2" )
		<< BUILD_LOG( m_abytePacketContent )
		<< END_LOG;

	return true;
}

bool KPayletterBillingPacket::Read( KEPL_BT_REGCOUPON_ACK& kPacket )
{
	
	kPacket.m_usReqLen = m_usReqLen;		// IN	1
	kPacket.m_usReqType = m_usReqType;	// IN	2
	kPacket.m_ulReqKey = m_ulReqKey;		// IN	3
	kPacket.m_usRetCode = m_usRetCode;	// OUT	4

	// 여기서 오류 처리는 어떻게 해야 할까?

	PGTX_PK_REGCOUPON tempPacket;
	tempPacket = (PGTX_PK_REGCOUPON)&m_usReqLen;
	// 받는 패킷에서 필요한 정보는 유저 UID.
	// 나머지는 필요 없는데...패킷을 바꿔야 하나?
	kPacket.m_iUserUID = (UidType)tempPacket->ReqKey;
	//kPacket.m_iItemID = (int)tempPacket->ItemID;
	//kPacket.m_iItemCount = (int)tempPacket->ItemCnt;

	int nStart = 0;
	char strData[256];
	for(int i = 0; i< sizeof(tempPacket->ItemID); i++)
	{
		if(tempPacket->ItemID[i] == 0x11 || tempPacket->ItemID[i] == '\0')
		{
			::memcpy(strData, ((char*)&tempPacket->ItemID) + nStart,  (i - nStart));
			strData[i] = '\0';			
			nStart = i+1;

			KBillBuyInfo info;
			info.m_iProductNo = atoi(strData);
			info.m_usOrderQuantity = 1;			 
			kPacket.m_vecBillBuyInfo.push_back(info);

			if(tempPacket->ItemID[i] == '\0')
			{
				break;
			}
		}
	}	

	//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
	for( int i = 0; i< sizeof( tempPacket->RetMsg ); i++ )
	{
		if( tempPacket->RetMsg[i] == '\0' )
		{
			::memcpy( strData, ( (char*)&tempPacket->RetMsg ), i );
			strData[i] = '\0';

			break;
		}
	}
	kPacket.m_wstrRetMsg = KncUtil::toWideString( strData );
	//}}

	return true;
}

bool KPayletterBillingPacket::Write( const KEBILL_USE_COUPON_REQ& kPacket )
{
	GTX_PK_REGCOUPON tempPacket;
	m_usReqLen = tempPacket.ReqLen = sizeof(_GTX_PK_REGCOUPON);
	m_usReqType = tempPacket.ReqType = GTX_PK_TYPE_REGCOUPON;
	m_ulReqKey = tempPacket.ReqKey = kPacket.m_PurchaserInfo.m_iUserUID;// 패킷 넘버는 필요 없는 듯.
	m_usRetCode = tempPacket.RetCode = 0;

	tempPacket.UserNo =  _wtoi(kPacket.m_PurchaserInfo.m_wstrUserName.c_str());//0;//(int)kPacket.m_iUnitUID;
	tempPacket.ClientIP = inet_addr( KncUtil::toNarrowString(kPacket.m_PurchaserInfo.m_wstrIP).c_str());

	
	tempPacket.GameServerNo = kPacket.m_PurchaserInfo.m_iServerGroupID;
	tempPacket.WorldNo = 0;

	//std::string strData = KncUtil::toNarrowString( L"" );
	//::strncpy(tempPacket.UserID, strData.c_str(), 50);
	::strncpy(tempPacket.UserID, "", 50);

	std::string strData = KncUtil::toNarrowString( kPacket.m_wstrSerialCode );
	
	::strncpy(tempPacket.CouponNo, strData.c_str(), 19);

	strData = KncUtil::toNarrowString( kPacket.m_PurchaserInfo.m_wstrUnitNickName );
	::strncpy(tempPacket.CharacterID, strData.c_str(), 50);
	::strncpy(tempPacket.StatProperty1, "", 50);
	::strncpy(tempPacket.StatProperty2, "", 50);
	::strncpy(tempPacket.StatProperty3, "", 50);

	tempPacket.RealCash = 0;
	tempPacket.BonusCash = 0;
	//tempPacket.ItemCnt = 0;

	// 지헌 : 과금 번호, 이벤트 등등인데 보낼때는 "" 로

	::strncpy(tempPacket.ItemID, "", 256);
	::strncpy(tempPacket.ItemCnt, "", 128);
	::strncpy(tempPacket.ChargeNo, "", 512);
	::strncpy(tempPacket.RetMsg, "", 256);


	// 마무리로 해더(10byte) 빼고 버퍼에 다 복사
	::memcpy(m_abytePacketContent, ((char*)&tempPacket) + HEADER_IN_PACKET_LENGTH_PLB, sizeof(_GTX_PK_REGCOUPON) - HEADER_IN_PACKET_LENGTH_PLB);

	START_LOG( clog2, L"BillingCouponNo" )
		<< BUILD_LOG( m_abytePacketContent )
		<< BUILD_LOG( tempPacket.CouponNo )
		<< END_LOG;
	return true;
}


// 선물하기 기능
bool KPayletterBillingPacket::Read( KEPL_BT_GIFTITEM_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_usReqLen = m_usReqLen;		// IN	1
	kPacket.m_usReqType = m_usReqType;		// IN	2
	kPacket.m_ulReqKey = m_ulReqKey;		// IN	3
	kPacket.m_usRetCode = m_usRetCode;		// OUT	4

	// 여기서 오류 처리는 어떻게 해야 할까?

	PGTX_PK_GIFTITEM tempPacket;
	tempPacket = (PGTX_PK_GIFTITEM)&m_usReqLen;
	// 받는 패킷에서 필요한 정보는 유저 UID.
	// 나머지는 필요 없는데...패킷을 바꿔야 하나?
	kPacket.m_iUserUID = (UidType)tempPacket->ReqKey;

	int nStart = 0;
	char strData[256];
	int iProductNo;
	for(int i = 0; i< sizeof(tempPacket->ItemID); i++)
	{
		if(tempPacket->ItemID[i] == 0x11 || tempPacket->ItemID[i] == '\0')
		{
			::memcpy(strData, ((char*)&tempPacket->ItemID) + nStart,  (i - nStart));
			strData[i] = '\0';			
			nStart = i+1;			
			iProductNo = atoi(strData);

			if(tempPacket->ItemID[i] == '\0')
			{
				break;
			}
		}
	}

	//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
	for( int i = 0; i< sizeof( tempPacket->RetMsg ); i++ )
	{
		if( tempPacket->RetMsg[i] == '\0' )
		{
			::memcpy( strData, ( (char*)&tempPacket->RetMsg ), i );
			strData[i] = '\0';

			break;
		}
	}
	kPacket.m_wstrRetMsg = KncUtil::toWideString( strData );
	//}}

	return true;
}


//bool KPayletterBillingPacket::Write( const KEPL_BT_GIFTITEM_REQ& kPacket )
//{
//	GTX_PK_GIFTITEM tempPacket;
//	m_usReqLen = tempPacket.ReqLen = sizeof(GTX_PK_GIFTITEM);
//	m_usReqType = tempPacket.ReqType = GTX_PK_TYPE_GIFTITEM;
//	m_ulReqKey = tempPacket.ReqKey = kPacket.m_ulReqKey;
//	m_usRetCode = tempPacket.RetCode = 0;
//
//	tempPacket.UserNo = (int)kPacket.m_iSenderUnitUID;
//	tempPacket.RUserNo = (int)kPacket.m_iReceiverUID;
//
//	tempPacket.ClientIP = inet_addr( kPacket.m_strUserIP.c_str());
//
//	int addSizeID = 0;
//	int addSizeCnt = 0;
//	int addSizePrice = 0;
//
//	for(int i = 0; i < kPacket.m_vecBillBuyInfo.size(); i++)
//	{
//		// 이름
//		std::string strItemID = toString(kPacket.m_vecBillBuyInfo[i].m_iProductID);
//		if(i != 0)
//		{
//			tempPacket.ItemID[addSizeID] = 0x11;
//			addSizeID++;
//		}
//		::memcpy(tempPacket.ItemID + addSizeID, strItemID.c_str(), strItemID.size()); 
//		addSizeID += strItemID.size();
//		tempPacket.ItemID[addSizeID] = '\0';
//
//		// 구매 개수
//		std::string strItemCnt = toString(kPacket.m_vecBillBuyInfo[i].m_usOrderQuantity);
//		if(i != 0)
//		{
//			tempPacket.ItemCnt[addSizeID] = 0x11;
//			addSizeCnt++;
//		}
//		::memcpy(tempPacket.ItemCnt + addSizeCnt, strItemCnt.c_str(), strItemCnt.size()); 
//		addSizeCnt += strItemCnt.size();
//		tempPacket.ItemCnt[addSizeCnt] = '\0';
//
//		// 가격
//		std::string strItemPrice = toString((int)kPacket.m_vecBillBuyInfo[i].m_iPoint);
//		if(i != 0)
//		{
//			tempPacket.ItemUnitPrice[addSizeID] = 0x11;
//			addSizePrice++;
//		}
//		::memcpy(tempPacket.ItemUnitPrice + addSizePrice, strItemPrice.c_str(), strItemPrice.size()); 
//		addSizePrice += strItemPrice.size();
//		tempPacket.ItemUnitPrice[addSizePrice] = '\0';
//	}
//
//
//	tempPacket.GameServerNo = kPacket.m_iServerGroupID;
//	tempPacket.WorldNo = 0;
//
//	std::string strData = KncUtil::toNarrowString( kPacket.m_wstrSenderUserID );
//	::strncpy(tempPacket.UserID, strData.c_str(), 50);
//
//	strData = KncUtil::toNarrowString( kPacket.m_wstrSenderNickName );
//	::strncpy(tempPacket.CharacterID, strData.c_str(), 50);
//	::strncpy(tempPacket.StatProperty1, "", 50);
//	::strncpy(tempPacket.StatProperty2, "", 50);
//	::strncpy(tempPacket.StatProperty3, "", 50);
//
//	tempPacket.RealCash = 0;
//	tempPacket.BonusCash = 0;
//	tempPacket.ChargedCashAmt = 0;
//
//	// 지헌 : 과금 번호, 이벤트 등등인데 보낼때는 "" 로
//	::strncpy(tempPacket.ChargeNo, "", 512);
//	::strncpy(tempPacket.EventItemID, "", 256);
//	::strncpy(tempPacket.EventChargeNo, "", 512);
//
//	::strncpy(tempPacket.RetMsg, "", 256);
//
//
//	// 마무리로 해더(10byte) 빼고 버퍼에 다 복사
//	::memcpy(m_abytePacketContent, ((char*)&tempPacket) + HEADER_IN_PACKET_LENGTH_PLB,  m_usReqLen - HEADER_IN_PACKET_LENGTH_PLB);
//
//	START_LOG( clog2, L"BillingPrice2" )
//		<< BUILD_LOG( m_abytePacketContent )
//		<< END_LOG;
//
//	return true;
//}

bool KPayletterBillingPacket::Write( const KEBILL_GIFT_ITEM_REQ& kPacket )
{
	GTX_PK_GIFTITEM tempPacket;
	m_usReqLen = tempPacket.ReqLen = sizeof(GTX_PK_GIFTITEM);
	m_usReqType = tempPacket.ReqType = GTX_PK_TYPE_GIFTITEM;
	m_ulReqKey = tempPacket.ReqKey = (DWORD)kPacket.m_iSenderUserUID;// 패킷 넘버는 필요 없는 듯.
	m_usRetCode = tempPacket.RetCode = 0;

	tempPacket.UserNo =  _wtoi(kPacket.m_wstrSenderUserID.c_str());//0;//(int)kPacket.m_iSenderUnitUID;
	tempPacket.RUserNo =  _wtoi(kPacket.m_wstrReceiverUserID.c_str());//0;//(int)kPacket.m_iReceiverUnitUID;

	std::string strClientIP = KncUtil::toNarrowString(kPacket.m_wstrUserIP);
	tempPacket.ClientIP = inet_addr( strClientIP.c_str() );

	int addSizeID = 0;
	int addSizeCnt = 0;
	int addSizePrice = 0;

	for(int i = 0; i < kPacket.m_vecBillBuyInfo.size(); i++)
	{
		// 이름
		std::string strItemID = toString(kPacket.m_vecBillBuyInfo[i].m_iProductNo);
		if(i != 0)
		{
			tempPacket.ItemID[addSizeID] = 0x11;
			addSizeID++;
		}
		::memcpy(tempPacket.ItemID + addSizeID, strItemID.c_str(), strItemID.size()); 
		addSizeID += strItemID.size();
		tempPacket.ItemID[addSizeID] = '\0';

		// 구매 개수
		std::string strItemCnt = "1";//toString(kPacket.m_vecBillBuyInfo[i].m_usOrderQuantity);
		if(i != 0)
		{
			tempPacket.ItemCnt[addSizeCnt] = 0x11;
			addSizeCnt++;
		}
		::memcpy(tempPacket.ItemCnt + addSizeCnt, strItemCnt.c_str(), strItemCnt.size()); 
		addSizeCnt += strItemCnt.size();
		tempPacket.ItemCnt[addSizeCnt] = '\0';

		// 가격
		std::string strItemPrice = toString((int)kPacket.m_vecBillBuyInfo[i].m_iPoint);
		if(i != 0)
		{
			tempPacket.ItemUnitPrice[addSizePrice] = 0x11;
			addSizePrice++;
		}
		::memcpy(tempPacket.ItemUnitPrice + addSizePrice, strItemPrice.c_str(), strItemPrice.size()); 
		addSizePrice += strItemPrice.size();
		tempPacket.ItemUnitPrice[addSizePrice] = '\0';


		START_LOG( clog2, L"BillingPrice" )
			<< BUILD_LOG( strItemPrice )
			<< BUILD_LOG( addSizePrice )
			<< BUILD_LOG( tempPacket.ItemUnitPrice )
			<< END_LOG;
	}


	tempPacket.GameServerNo = kPacket.m_iServerGroupID;
	tempPacket.WorldNo = 0;

	//std::string strData = KncUtil::toNarrowString( kPacket.m_wstrUserID );
	//::strncpy(tempPacket.UserID, strData.c_str(), 50);
	::strncpy(tempPacket.UserID, "", 50);

	std::string strData = KncUtil::toNarrowString( kPacket.m_wstrSenderNickName );
	::strncpy(tempPacket.CharacterID, strData.c_str(), 50);

	::strncpy(tempPacket.RUserID, "", 50);
	strData = KncUtil::toNarrowString( kPacket.m_wstrReceiverNickName );
	::strncpy(tempPacket.RCharacterID, strData.c_str(), 50);

	char szLevel[4];
	::_itoa( (int)kPacket.m_BuyCharInfo.m_cLevel, szLevel, 10 );
	::strncpy(tempPacket.StatProperty1, szLevel, 50);

	char szUnitClass[128];
	::_itoa( kPacket.m_BuyCharInfo.m_iUnitClass, szUnitClass, 10 );
	::strncpy(tempPacket.StatProperty2, szUnitClass, 50);

	::_itoa( (int)kPacket.m_RecvCharInfo.m_cLevel, szLevel, 10 );
	::_itoa( kPacket.m_RecvCharInfo.m_iUnitClass, szUnitClass, 10 );
	strData = szLevel;
	strData = strData + "|" + szUnitClass;
	::strncpy(tempPacket.StatProperty3, strData.c_str(), 50);

	START_LOG( clog2, L"캐릭터 정보 확인" )
		<< BUILD_LOG( tempPacket.StatProperty1 )
		<< BUILD_LOG( tempPacket.StatProperty2 )		
		<< BUILD_LOG( tempPacket.StatProperty3 )		
		<< END_LOG;


	tempPacket.RealCash = 0;
	tempPacket.BonusCash = 0;
	tempPacket.ChargedCashAmt = 0;

	// 지헌 : 과금 번호, 이벤트 등등인데 보낼때는 "" 로
	::strncpy(tempPacket.ChargeNo, "", 512);
	::strncpy(tempPacket.EventItemID, "", 256);
	::strncpy(tempPacket.EventChargeNo, "", 512);

	::strncpy(tempPacket.RetMsg, "", 256);


	// 마무리로 해더(10byte) 빼고 버퍼에 다 복사
	::memcpy(m_abytePacketContent, ((char*)&tempPacket) + HEADER_IN_PACKET_LENGTH_PLB,  sizeof(GTX_PK_GIFTITEM) - HEADER_IN_PACKET_LENGTH_PLB);

	START_LOG( clog2, L"BillingPrice2" )
		<< BUILD_LOG( m_abytePacketContent )
		<< END_LOG;

	return true;
}

#endif SERV_COUNTRY_US