#include "NexonAuthPacket.h"


//{{ 2010. 06. 04  ÃÖÀ°»ç	³Ø½¼PC¹æ ÀÎÁõ ¼­¹ö °³Æí
#ifdef SERV_PCBANG_AUTH_NEW


//////////////////////////////////////////////////////////////////////////
//{{ ³Ø½¼ ¼¼¼Ç ½Ã½ºÅÛ ¿À·ù ¼öÁ¤ - ±è¹Î¼º
#ifdef SERV_SESSION_SYSTEM_ERROR
unsigned long long htonll( unsigned long long v )
{
	union { unsigned long lv[2]; unsigned long long llv; } u;
	u_long temp = (u_long)(v >> 32);

	u.lv[0] = ::htonl( temp ); 
	u.lv[1] = ::htonl( (u_long)(v & 0xFFFFFFFFULL)); 
	return u.llv;
}
#else
unsigned long long htonll( unsigned long long v )
{
	union { unsigned long lv[2]; unsigned long long llv; } u;
	u.lv[0] = ::htonl( (u_long)v >> 32); 
	u.lv[1] = ::htonl( (u_long)v & 0xFFFFFFFFULL); 
	return u.llv;
}
#endif SERV_SESSION_SYSTEM_ERROR
//}}

unsigned long long ntohll( unsigned long long v )
{
	union { unsigned long lv[2]; unsigned long long llv; } u; 
	u.llv = v; 
	return ((unsigned long long)ntohl(u.lv[0]) << 32) | (unsigned long long)ntohl(u.lv[1]); 
}
//////////////////////////////////////////////////////////////////////////



KNexonAuthPacket::KNexonAuthPacket()
{
	m_byteHeader = 0xAA; // ¹Ì¸® ÁöÁ¤µÈ °íÁ¤ »ó¼ö°ª 0xAA
	m_usPacketLength = 0;
	m_bytePacketType = 0;
}

KNexonAuthPacket::~KNexonAuthPacket()
{
}

bool KNexonAuthPacket::ReadFromBuffer( IN BYTE* pbyteBuffer )
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
	_JIF( m_usPacketLength <= MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH + 1, return false );
	::memcpy( &m_bytePacketType, pbyteBuffer + 3, sizeof( BYTE ) );
	if( m_usPacketLength > 1 )
	{
		::memcpy( m_abytePacketContent, pbyteBuffer + 4, m_usPacketLength - 1 );
	}

	return true;
}

bool KNexonAuthPacket::WriteToBuffer( OUT BYTE* pbyteBuffer )
{
	if( !pbyteBuffer )
	{
		return false;
	}

    _JIF( m_usPacketLength >= 1, return false );
    _JIF( m_usPacketLength <= MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH + 1, return false );
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

bool KNexonAuthPacket::ReadByte( BYTE& byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 1 )
	{
		return false;
	}

	BYTE byteDataTemp;
	::memcpy( &byteDataTemp, m_abytePacketContent + iCP, sizeof( BYTE ) );
	byteData = byteDataTemp;
	iCP = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read2Byte( unsigned short& usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 2 )
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

bool KNexonAuthPacket::Read4Byte( unsigned long& ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 4 )
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

bool KNexonAuthPacket::Read8Byte( __int64& i64Data, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 8 )
	{
		return false;
	}

	__int64 i64DataTemp;
	::memcpy( &i64DataTemp, m_abytePacketContent + iCP, sizeof( __int64 ) );
	i64Data = i64DataTemp;
	i64Data = ntohll( i64Data );
	iCP = iCP + 8;
	return true;    
}

bool KNexonAuthPacket::ReadIP( unsigned int& uiData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 4 )
	{
		return false;
	}

	unsigned int uiDataTemp;
	::memcpy( &uiDataTemp, m_abytePacketContent + iCP, sizeof( unsigned long ) );
	uiData = uiDataTemp;
	iCP = iCP + 4;
	return true;
}

bool KNexonAuthPacket::ReadString( std::wstring& wstrData, int& iCP )
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

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - ( int )byteLength )
	{
		return false;
	}

	//{{ 2011. 06. 22    ±è¹Î¼º    ÀÌÅ» ¹æÁö ¸ðµ¨ - NEXON ¼¼¼Ç ½Ã½ºÅÛ ¼öÁ¤
#ifdef SERV_NEXON_SESSION_PROTOCOL
	char szData[MAX_STRING_SIZE_OF_NEXON_AUTH];
#else
	char szData[MAX_PATH];
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	
	::memcpy( szData, m_abytePacketContent + iCP, ( size_t )byteLength );
	szData[byteLength] = '\0';
	std::string strData = szData;
	wstrData = KncUtil::toWideString( strData );
	iCP = iCP + ( int )byteLength;
	return true;
}

bool KNexonAuthPacket::WriteByte( BYTE byteData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 1 )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &byteData, sizeof( BYTE ) );
	iCP = iCP + 1;
	return true;
}

bool KNexonAuthPacket::Write2Byte( unsigned short usData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 2 )
	{
		return false;
	}

	usData = ::htons( usData );
	::memcpy( m_abytePacketContent + iCP, &usData, sizeof( unsigned short ) );
	iCP = iCP + 2;
	return true;
}

bool KNexonAuthPacket::Write4Byte( unsigned long ulData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 4 )
	{
		return false;
	}

	ulData = ::htonl( ulData );
	::memcpy( m_abytePacketContent + iCP, &ulData, sizeof( unsigned long ) );
	iCP = iCP + 4;
	return true;
}

bool KNexonAuthPacket::Write8Byte( __int64 i64Data, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 8 )
	{
		return false;
	}

	i64Data = htonll( i64Data );
	::memcpy( m_abytePacketContent + iCP, &i64Data, sizeof( __int64 ) );
	iCP = iCP + 8;
	return true;
}

bool KNexonAuthPacket::WriteIP( unsigned int uiData, int& iCP )
{
	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - 4 )
	{
		return false;
	}

	::memcpy( m_abytePacketContent + iCP, &uiData, sizeof( unsigned long ) );
	iCP = iCP + 4;
	return true;
}

bool KNexonAuthPacket::WriteString( const std::wstring& wstrData, int& iCP )
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

	if( iCP < 0 || iCP > MAX_PACKET_CONTENT_SIZE_OF_NEXON_AUTH - ( int )byteLength )
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

bool KNexonAuthPacket::Read( KENX_AUTH_INITIALIZE_REQ& kPacket )
{
	int iCP = 0;
    kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteInitializeType, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteGameSN, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteDomainSN, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrDomainName, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteSynchronizeType, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulSynchronizeCount, iCP ), return false );
    LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_INITIALIZE_REQ& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteInitializeType, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteGameSN, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteDomainSN, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrDomainName, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteSynchronizeType, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulSynchronizeCount, iCP ), return false );
	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_INITIALIZE_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteInitializeType, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteResult, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteDomainSN, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrMessage, iCP ), return false );
	LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_INITIALIZE_ACK& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteInitializeType, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteResult, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteDomainSN, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrMessage, iCP ), return false );	
	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_LOGIN_REQ& kPacket )
{
	int iCP = 0;
    kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteLoginType, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( ReadIP( kPacket.m_uiIP, iCP ), return false );

	BYTE bytePropertyCount = 0;
	_JIF( ReadByte( bytePropertyCount, iCP ), return false );

	// PropertyCount¸¸Å­ PerpertyÁ¤º¸¸¦ ¾ò¾î³»ÀÚ!
	for( BYTE byteIndex = 0; byteIndex < bytePropertyCount; ++byteIndex )
	{
		unsigned short usPropertyNo = 0;
		KPropertyValue kPropertyValue;

		_JIF( Read2Byte( usPropertyNo, iCP ), return false );
		switch( usPropertyNo )
		{
		case 1: // CharacterName
		case 5: // MachinelID
			{
				_JIF( ReadString( kPropertyValue.m_wstrPropertyValue, iCP ), return false );
			}
			break;

		case 2: // LocalIP
			{
				_JIF( ReadIP( kPropertyValue.m_uintPropertyValue, iCP ), return false );
			}
			break;
			//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
		case 3: // IsCanTry
		case 4: // IsExperienceID
		case 12: // OnlyAuthentication
		case 13: // IsReturnPCBangNo
			{
				_JIF( ReadByte( kPropertyValue.m_bytePropertyValue, iCP ), return false );
			}
			break;
		case 17: // IsReturnPolicyResult
			{
				BYTE bytePropertyCount = 0;
				KRolicyResults kResult;
				_JIF( ReadByte( bytePropertyCount, iCP ), return false );
			
				for( BYTE byteCount = 0 ; byteCount < bytePropertyCount ; ++byteCount )
				{
					_JIF( ReadByte( kResult.m_byteRolicyListNo, iCP ), return false );	
					kPropertyValue.m_mapRolicyResult.insert( std::make_pair( kResult.m_byteRolicyListNo, kResult ) );
				}
			}
			break;
		case 7: // PCBangNo
			{
				_JIF( Read4Byte( kPropertyValue.m_ulongPropertyValue, iCP ), return false );
			}break;
		case 14: // RequestUniqueID
			{
				_JIF( Read8Byte( kPropertyValue.m_int64PropertyValue, iCP ), return false );
			}
			break;
#else
		case 7: // PCBangNo
		case 14: // RequestUniqueID
			{
				_JIF( Read4Byte( kPropertyValue.m_ulongPropertyValue, iCP ), return false );
			}
			break;

		case 3: // IsCanTry
		case 4: // IsExperienceID
		case 12: // OnlyAuthentication
		case 13: // IsReturnPCBangNo
		case 17: // IsReturnPolicyResult
			{
				_JIF( ReadByte( kPropertyValue.m_bytePropertyValue, iCP ), return false );
			}
			break;
#endif SERV_SHUTDOWN_SYSTEM
			//}}
		case 6: // Grcs
			{
				BYTE byteGrcCount = 0;
				_JIF( ReadByte( byteGrcCount, iCP ), return false );

				for( BYTE byteIndex = 0; byteIndex < byteGrcCount; ++byteIndex )
				{
					unsigned long ulGrcValue = 0;
                    _JIF( Read4Byte( ulGrcValue, iCP ), return false );
					kPropertyValue.m_vecPropertyValue.push_back( ulGrcValue );
				}
			}
			break;

		default:
			{
				START_LOG( cerr, L"PropertyNo Error : " << usPropertyNo )
					<< END_LOG;
				continue;
			}
		}

		// Property¸¦ ¾ò¾î³ÂÀ¸¸é ÀúÀåÇÏÀÚ!
		kPacket.m_mapProperty.insert( std::make_pair( usPropertyNo, kPropertyValue ) );
	}

    LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_LOGIN_REQ& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteLoginType, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( WriteIP( kPacket.m_uiIP, iCP ), return false );

	// Property °¹¼ö ±â·Ï
	_JIF( WriteByte( static_cast<BYTE>(kPacket.m_mapProperty.size()), iCP ), return false );

	std::map< unsigned short, KPropertyValue >::const_iterator mit;
	for( mit = kPacket.m_mapProperty.begin(); mit != kPacket.m_mapProperty.end(); ++mit )
	{
		_JIF( Write2Byte( mit->first, iCP ), return false );
		switch( mit->first )
		{
		case 1: // CharacterName
		case 5: // MachinelID
			{
				_JIF( WriteString( mit->second.m_wstrPropertyValue, iCP ), return false );
			}
			break;

		case 2: // LocalIP
			{
				_JIF( WriteIP( mit->second.m_uintPropertyValue, iCP ), return false );
			}
			break;

		case 7: // PCBangNo
		case 14: // RequestUniqueID
			{
				_JIF( Write4Byte( mit->second.m_ulongPropertyValue, iCP ), return false );
			}
			break;

			//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
		case 3: // IsCanTry
		case 4: // IsExperienceID
		case 12: // OnlyAuthentication
		case 13: // IsReturnPCBangNo
			{
				_JIF( WriteByte( mit->second.m_bytePropertyValue, iCP ), return false );
			}
			break;
		case 17: // IsReturnPolicyResult
			{
				_JIF( WriteByte( static_cast<BYTE>( mit->second.m_mapRolicyResult.size() ), iCP ), return false );

				std::map< BYTE, KRolicyResults >::const_iterator const_mit = mit->second.m_mapRolicyResult.begin();
				for(  ; const_mit != mit->second.m_mapRolicyResult.end() ; ++const_mit )
				{
					_JIF( WriteByte( const_mit->first, iCP ), return false );
				}
			}
			break;
#else
		case 3: // IsCanTry
		case 4: // IsExperienceID
		case 12: // OnlyAuthentication
		case 13: // IsReturnPCBangNo
		case 17: // IsReturnPolicyResult
			{
				_JIF( WriteByte( mit->second.m_bytePropertyValue, iCP ), return false );
			}
			break;
#endif SERV_SHUTDOWN_SYSTEM
			//}}

		case 6: // Grcs
			{
				_JIF( WriteByte( static_cast<BYTE>(mit->second.m_vecPropertyValue.size()), iCP ), return false );

				std::vector< unsigned long >::const_iterator vit;
				for( vit = mit->second.m_vecPropertyValue.begin(); vit != mit->second.m_vecPropertyValue.end(); ++vit )
				{
					_JIF( Write4Byte( *vit, iCP ), return false );
				}
			}
			break;

		default:
			{
				START_LOG( cerr, L"PropertyNo Error : " << mit->first )
					<< END_LOG;
				continue;
			}
		}
	}

	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_LOGIN_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteLoginType, iCP ), return false );
	_JIF( Read8Byte( kPacket.m_iSessionNo, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteAuthorizeResult, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteAuthorizeType, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteChargeType, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteOption, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulArgument, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteIsCharged, iCP ), return false );

	BYTE bytePropertyCount = 0;
	_JIF( ReadByte( bytePropertyCount, iCP ), return false );

	// PeropertyCount¸¸Å­ ÀÐÀÚ!
	for( BYTE byteIndex = 0; byteIndex < bytePropertyCount; ++byteIndex )
	{
		unsigned short usPropertyNo = 0;
		KPropertyValue kPropertyValue;

		_JIF( Read2Byte( usPropertyNo, iCP ), return false );
		switch( usPropertyNo )
		{
			//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
		case 7: // PCBangNo
			{
				_JIF( Read4Byte( kPropertyValue.m_ulongPropertyValue, iCP ), return false );
			}
			break;
		case 14: // RequestUniqueID
			{
				_JIF( Read8Byte( kPropertyValue.m_int64PropertyValue, iCP ), return false );
			}break;
#else
		case 7: // PCBangNo
		case 14: // RequestUniqueID
			{
				_JIF( Read4Byte( kPropertyValue.m_ulongPropertyValue, iCP ), return false );
			}
			break;
#endif SERV_SHUTDOWN_SYSTEM
			//}}

		case 9: // PCBangName
		case 10: // PCBangZipCode
			//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
#else
			//{{ 2011. 06. 22    ±è¹Î¼º    ÀÌÅ» ¹æÁö ¸ðµ¨ - NEXON ¼¼¼Ç ½Ã½ºÅÛ ¼öÁ¤
#ifdef SERV_NEXON_SESSION_PROTOCOL
		case 18: // PolicyResult
#endif SERV_NEXON_SESSION_PROTOCOL
			//}} 
#endif SERV_SHUTDOWN_SYSTEM
			//}}
			{
				_JIF( ReadString( kPropertyValue.m_wstrPropertyValue, iCP ), return false );
			}
			break;

		case 8: // PCBangValieded
		case 11: // PCBangLevel
			{
				_JIF( ReadByte( kPropertyValue.m_bytePropertyValue, iCP ), return false );
			}
			break;
			//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
		case 19: // PolicyResults
			{
				KRolicyResults kResults;
				_JIF( ReadByte( kResults.m_byteRolicyCount, iCP ), return false );

				for( BYTE byteCnt = 0; byteCnt < kResults.m_byteRolicyCount; ++byteCnt )
				{
					_JIF( ReadByte( kResults.m_byteRolicyListNo, iCP ), return false );
					_JIF( ReadString( kResults.m_wstrRolicyName, iCP ), return false );
					_JIF( ReadByte( kResults.m_byteRolicyResult, iCP ), return false );
					_JIF( ReadString( kResults.m_wstrOption, iCP ), return false );

					kPropertyValue.m_mapRolicyResult.insert( std::make_pair( kResults.m_byteRolicyListNo, kResults ) );

					/////////////////////////////////////////////////////////////////////////
					START_LOG( clog, L"PolicyResults µ¥ÀÌÅÍ ¹Þ±â" )
						<< BUILD_LOG( kResults.m_byteRolicyListNo )
						<< BUILD_LOG( kResults.m_wstrRolicyName )
						<< BUILD_LOG( kResults.m_byteRolicyResult )
						<< BUILD_LOG( kResults.m_wstrOption )
						<< END_LOG;
					//////////////////////////////////////////////////////////////////////////
					
				}
			}
			break;
#endif SERV_SHUTDOWN_SYSTEM
			//}}
		default:
			{
				START_LOG( cerr, L"PropertyNo Error : " << usPropertyNo )
					<< END_LOG;
				continue;
			}
		}

		// Property¸¦ ¾ò¾î³ÂÀ¸¸é ÀúÀåÇÏÀÚ!
		kPacket.m_mapProperty.insert( std::make_pair( usPropertyNo, kPropertyValue ) );
	}

	//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
    LIF( iCP == m_usPacketLength - 1 );
#else
#endif SERV_SHUTDOWN_SYSTEM
	//}}
	
	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_LOGIN_ACK& kPacket )
//bool KNexonAuthPacket::Write(  KENX_AUTH_LOGIN_ACK& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteLoginType, iCP ), return false );
	_JIF( Write8Byte( kPacket.m_iSessionNo, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );	
	_JIF( WriteByte( kPacket.m_byteAuthorizeResult, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteAuthorizeType, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteChargeType, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteOption, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulArgument, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteIsCharged, iCP ), return false );

	// Property °¹¼ö ±â·Ï
	_JIF( WriteByte( static_cast<BYTE>(kPacket.m_mapProperty.size()), iCP ), return false );

	std::map< unsigned short, KPropertyValue >::const_iterator mit;
	for( mit = kPacket.m_mapProperty.begin(); mit != kPacket.m_mapProperty.end(); ++mit )
	{
		_JIF( Write2Byte( mit->first, iCP ), return false );
		switch( mit->first )
		{
			//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
		case 7: // PCBangNo
			{
				_JIF( Write4Byte( mit->second.m_ulongPropertyValue, iCP ), return false );
			}
			break;
		case 14: // RequestUniqueID
			{
				_JIF( Write8Byte( mit->second.m_int64PropertyValue, iCP ), return false );
			}break;
#else
		case 7: // PCBangNo
		case 14: // RequestUniqueID
			{
				_JIF( Write4Byte( mit->second.m_ulongPropertyValue, iCP ), return false );
			}
			break;
#endif SERV_SHUTDOWN_SYSTEM
			//}}
		case 9: // PCBangName
		case 10: // PCBangZipCode
			//{{ 2011. 06. 22    ±è¹Î¼º    ÀÌÅ» ¹æÁö ¸ðµ¨ - NEXON ¼¼¼Ç ½Ã½ºÅÛ ¼öÁ¤
#ifdef SERV_NEXON_SESSION_PROTOCOL
		case 18: // PolicyResult
#endif SERV_NEXON_SESSION_PROTOCOL
			//}} 
			{
				_JIF( WriteString( mit->second.m_wstrPropertyValue, iCP ), return false );
			}
			break;

		case 8: // PCBangValieded
		case 11: // PCBangLevel
			{
				_JIF( WriteByte( mit->second.m_bytePropertyValue, iCP ), return false );
			}
			break;
			//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
		case 19: // PolicyResults
			{
				_JIF( WriteByte( static_cast<BYTE>(mit->second.m_mapRolicyResult.size()), iCP ), return false );

				std::map< BYTE, KRolicyResults >::const_iterator mitRolicy;
				for( mitRolicy = mit->second.m_mapRolicyResult.begin() ; mitRolicy != mit->second.m_mapRolicyResult.end() ; ++mitRolicy )
				{
					_JIF( WriteByte( mitRolicy->second.m_byteRolicyListNo, iCP ), return false );
					_JIF( WriteString( mitRolicy->second.m_wstrRolicyName, iCP ), return false );
					_JIF( WriteByte( mitRolicy->second.m_byteRolicyResult, iCP ), return false );
					_JIF( WriteString( mitRolicy->second.m_wstrOption, iCP ), return false );

					START_LOG( clog, L"PolicyResults µ¥ÀÌÅÍ ³Ö±â" )
						<< BUILD_LOG( kPacket.m_wstrUserID )
						<< BUILD_LOG( mitRolicy->second.m_byteRolicyListNo )
						<< BUILD_LOG( mitRolicy->second.m_wstrRolicyName )
						<< BUILD_LOG( mitRolicy->second.m_byteRolicyResult )
						<< BUILD_LOG( mitRolicy->second.m_wstrOption )
						<< END_LOG;
				}
			}
			break;
#endif SERV_SHUTDOWN_SYSTEM
			//}}
		default:
			{
				START_LOG( cerr, L"PropertyNo Error : " << mit->first )
					<< END_LOG;
				continue;
			}
		}
	}
	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_LOGOUT_NOT& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteLogoutType, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( Read8Byte( kPacket.m_iSessionNo, iCP ), return false );
	LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_LOGOUT_NOT& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteLogoutType, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( Write8Byte( kPacket.m_iSessionNo, iCP ), return false );
	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_TERMINATE_NOT& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteTerminateType, iCP ), return false );
	_JIF( Read8Byte( kPacket.m_iSessionNo, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCharacterName, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteOption, iCP ), return false );

//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
	BYTE bytePropertyCount = 0;
	_JIF( ReadByte( bytePropertyCount, iCP ), return false );

	// PeropertyCount¸¸Å­ ÀÐÀÚ!
	for( BYTE byteIndex = 0; byteIndex < bytePropertyCount; ++byteIndex )
	{
		unsigned short usPropertyNo = 0;
		KPropertyValue kPropertyValue;

		_JIF( Read2Byte( usPropertyNo, iCP ), return false );
		switch( usPropertyNo )
		{
		case 19: // PolicyResults
			{
				KRolicyResults kResults;
				_JIF( ReadByte( kResults.m_byteRolicyCount, iCP ), return false );

				for( BYTE byteCnt = 0; byteCnt < kResults.m_byteRolicyCount; ++byteCnt )
				{
					_JIF( ReadByte( kResults.m_byteRolicyListNo, iCP ), return false );
					_JIF( ReadString( kResults.m_wstrRolicyName, iCP ), return false );
					_JIF( ReadByte( kResults.m_byteRolicyResult, iCP ), return false );
					_JIF( ReadString( kResults.m_wstrOption, iCP ), return false );

					kPropertyValue.m_mapRolicyResult.insert( std::make_pair( kResults.m_byteRolicyListNo, kResults ) );

				}
			}
			break;
		default:
			{
				START_LOG( cerr, L"PropertyNo Error : " << usPropertyNo )
					<< END_LOG;
				continue;
			}break;
		}

		// Property¸¦ ¾ò¾î³ÂÀ¸¸é ÀúÀåÇÏÀÚ!
		kPacket.m_mapProperty.insert( std::make_pair( usPropertyNo, kPropertyValue ) );
	}
#endif SERV_SHUTDOWN_SYSTEM
			//}}
	LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_TERMINATE_NOT& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteTerminateType, iCP ), return false );
	_JIF( Write8Byte( kPacket.m_iSessionNo, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCharacterName, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteOption, iCP ), return false );

	//{{ 2011. 09. 29  ±è¹Î¼º	¼Ë´Ù¿îÁ¦ µµÀÔ
#ifdef SERV_SHUTDOWN_SYSTEM
	_JIF( WriteByte( static_cast<BYTE>(kPacket.m_mapProperty.size()), iCP ), return false );

	std::map< unsigned short, KPropertyValue >::const_iterator mit;
	for( mit = kPacket.m_mapProperty.begin(); mit != kPacket.m_mapProperty.end(); ++mit )
	{
		_JIF( Write2Byte( mit->first, iCP ), return false );
		switch( mit->first )
		{
		case 19: // PolicyResults
			{
				_JIF( WriteByte( static_cast<BYTE>(mit->second.m_mapRolicyResult.size()), iCP ), return false );

				std::map< BYTE, KRolicyResults >::const_iterator mitRolicy;
				for( mitRolicy = mit->second.m_mapRolicyResult.begin() ; mitRolicy != mit->second.m_mapRolicyResult.end() ; ++mitRolicy )
				{
					_JIF( WriteByte( mitRolicy->second.m_byteRolicyListNo, iCP ), return false );
					_JIF( WriteString( mitRolicy->second.m_wstrRolicyName, iCP ), return false );
					_JIF( WriteByte( mitRolicy->second.m_byteRolicyResult, iCP ), return false );
					_JIF( WriteString( mitRolicy->second.m_wstrOption, iCP ), return false );
				}
			}
			break;
		default:
			{
				START_LOG( cerr, L"PropertyNo Error : " << mit->first )
					<< END_LOG;
				continue;
			}
		}
	}
#endif SERV_SHUTDOWN_SYSTEM
	//}}

	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_MESSAGE_NOT& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;
	_JIF( ReadByte( kPacket.m_byteMessageType, iCP ), return false );
	_JIF( Read8Byte( kPacket.m_iSessionNo, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( ReadString( kPacket.m_wstrCharacterName, iCP ), return false );
	_JIF( ReadByte( kPacket.m_byteOption, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulArgument, iCP ), return false );
	_JIF( Read4Byte( kPacket.m_ulSessionCount, iCP ), return false );
	LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_MESSAGE_NOT& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;
	_JIF( WriteByte( kPacket.m_byteMessageType, iCP ), return false );
	_JIF( Write8Byte( kPacket.m_iSessionNo, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrUserID, iCP ), return false );
	_JIF( WriteString( kPacket.m_wstrCharacterName, iCP ), return false );
	_JIF( WriteByte( kPacket.m_byteOption, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulArgument, iCP ), return false );
	_JIF( Write4Byte( kPacket.m_ulSessionCount, iCP ), return false );
	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_SYNCHRONIZE_REQ& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( ReadByte( kPacket.m_byteIsMonitoring, iCP ), return false );

	unsigned long ulCount = 0;
	_JIF( Read4Byte( ulCount, iCP ), return false );	

	for( unsigned long ulIndex = 0; ulIndex < ulCount; ++ulIndex )
	{
		__int64 iSessionNo = 0;
		std::wstring wstrUserID;

		_JIF( Read8Byte( iSessionNo, iCP ), return false );
		//{{  2011. 10. 07    ±è¹Î¼º
#ifdef SERV_PCBANG_INCORRECT_BILLING
#else
		_JIF( ReadString( wstrUserID, iCP ), return false );		// ÃÊ±âÈ­ ½Ã m_byteInitializeType °¡ 1 ÀÌ¹Ç·Î »ç¿ëÇÏÁö ¾Ê°Ô µÇ¾ú´Ù.
#endif SERV_PCBANG_INCORRECT_BILLING
		//}}
		kPacket.m_mapSynchronizeReq.insert( std::make_pair( iSessionNo, wstrUserID ) );
	}

	LIF( iCP == m_usPacketLength - 1 );


	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_SYNCHRONIZE_REQ& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( WriteByte( kPacket.m_byteIsMonitoring, iCP ), return false );
	_JIF( Write4Byte( static_cast<unsigned long>(kPacket.m_mapSynchronizeReq.size()), iCP ), return false );

	std::map< __int64, std::wstring >::const_iterator mit;
	for( mit = kPacket.m_mapSynchronizeReq.begin(); mit != kPacket.m_mapSynchronizeReq.end(); ++mit )
	{
		_JIF( Write8Byte( mit->first, iCP ), return false );
		//{{  2011. 10. 07    ±è¹Î¼º
#ifdef SERV_PCBANG_INCORRECT_BILLING
#else
		_JIF( WriteString( mit->second, iCP ), return false );		// ÃÊ±âÈ­ ½Ã m_byteInitializeType °¡ 1 ÀÌ¹Ç·Î »ç¿ëÇÏÁö ¾Ê°Ô µÇ¾ú´Ù.
#endif SERV_PCBANG_INCORRECT_BILLING
		//}}
	}

	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_SYNCHRONIZE_ACK& kPacket )
{
	int iCP = 0;
	kPacket.m_bytePacketType = m_bytePacketType;

	_JIF( ReadByte( kPacket.m_byteIsMonitoring, iCP ), return false );

	unsigned long ulCount = 0;
	_JIF( Read4Byte( ulCount, iCP ), return false );

	for( unsigned long ulIndex = 0; ulIndex < ulCount; ++ulIndex )
	{
		__int64 iSessionNo = 0;
		BYTE byteSessionAlived = 0;

		_JIF( Read8Byte( iSessionNo, iCP ), return false );
		_JIF( ReadByte( byteSessionAlived, iCP ), return false );

		kPacket.m_mapSynchronizeAck.insert( std::make_pair( iSessionNo, byteSessionAlived ) );
	}

	LIF( iCP == m_usPacketLength - 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_SYNCHRONIZE_ACK& kPacket )
{
	int iCP = 0;
	m_bytePacketType = kPacket.m_bytePacketType;

	_JIF( WriteByte( kPacket.m_byteIsMonitoring, iCP ), return false );
	_JIF( Write4Byte( static_cast<unsigned long>(kPacket.m_mapSynchronizeAck.size()), iCP ), return false );

	std::map< __int64, BYTE >::const_iterator mit;
	for( mit = kPacket.m_mapSynchronizeAck.begin(); mit != kPacket.m_mapSynchronizeAck.end(); ++mit )
	{
		_JIF( Write8Byte( mit->first, iCP ), return false );
		_JIF( WriteByte( mit->second, iCP ), return false );
	}

	m_usPacketLength = iCP + 1;

	return true;
}

bool KNexonAuthPacket::Read( KENX_AUTH_ALIVE_NOT& kPacket )
{
    kPacket.m_bytePacketType = m_bytePacketType;
    LIF( m_usPacketLength == 1 );

	return true;
}

bool KNexonAuthPacket::Write( const KENX_AUTH_ALIVE_NOT& kPacket )
{
	m_bytePacketType = kPacket.m_bytePacketType;
    m_usPacketLength = 1;

	return true;
}


#endif SERV_PCBANG_AUTH_NEW
//}}

