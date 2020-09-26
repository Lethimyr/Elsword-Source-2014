
//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
//#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK

#include "ServerSNMaker.h"
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>
#include <boost/format.hpp>

KServerSNMaker& KServerSNMaker::GetInstance( void )
{
    static KServerSNMaker obj;
    return obj;
}

std::string KServerSNMaker::GetCRCFromString( const std::string& str )
{
    DWORD dwCRCCode = CRC.CalcCRC( str.c_str(), str.size() );

    if ( 0 == dwCRCCode )
    {
        return std::string(); // failed
    }

    std::ostringstream ostrStream;
    ostrStream << std::resetiosflags( std::ios::uppercase )
        << std::hex << std::setw( 8 ) << std::setfill( '0' )
        << dwCRCCode;

    return ostrStream.str(); // succeeded
}

bool KServerSNMaker::ShuffleData( IN OUT unsigned char* pcharData )
{
	// 데이터를 일정한 규칙에 의해서 섞는다.
	// iSize 는 12로 고정 되어야 한다.
	if( pcharData == NULL )
	{
		return false;
	}

	unsigned char copyData[12] = {0,};

	//////////////////////////////////////////////////////////////////////////
	// 고정된 규칙(셔플)
	copyData[0]		= pcharData[3];
	copyData[1]		= pcharData[2];
	copyData[2]		= pcharData[11];
	copyData[3]		= pcharData[4];
	copyData[4]		= pcharData[1];
	copyData[5]		= pcharData[0];
	copyData[6]		= pcharData[7];
	copyData[7]		= pcharData[6];
	copyData[8]		= pcharData[9];
	copyData[9]		= pcharData[10];
	copyData[10]	= pcharData[5];
	copyData[11]	= pcharData[8];
	//////////////////////////////////////////////////////////////////////////

	// pcharData 는 문자열이 아니므로 strlen 을 사용하지 않는다.
	// 다시 데이터를 옮긴다.
	for( int iIndex = 0 ; iIndex < 12 ; ++iIndex )
	{
		pcharData[iIndex] = copyData[iIndex];
	}

	return true;
}

bool KServerSNMaker::UnShuffleData( IN OUT unsigned char* pcharData )
{
	// 데이터를 일정한 규칙에 의해서 섞는다.
	// iSize 는 12로 고정 되어야 한다.
	if( pcharData == NULL )
	{
		return false;
	}

	unsigned char copyData[12] = {0,};

	//////////////////////////////////////////////////////////////////////////
	// 고정된 규칙(셔플)
	copyData[3]		= pcharData[0];
	copyData[2]		= pcharData[1];
	copyData[11]	= pcharData[2];
	copyData[4]		= pcharData[3];
	copyData[1]		= pcharData[4];
	copyData[0]		= pcharData[5];
	copyData[7]		= pcharData[6];
	copyData[6]		= pcharData[7];
	copyData[9]		= pcharData[8];
	copyData[10]	= pcharData[9];
	copyData[5]		= pcharData[10];
	copyData[8]		= pcharData[11];
	//////////////////////////////////////////////////////////////////////////

	// pcharData 는 문자열이 아니므로 strlen 을 사용하지 않는다.
	// 다시 데이터를 옮긴다.
	for( int iIndex = 0 ; iIndex < 12 ; ++iIndex )
	{
		pcharData[iIndex] = copyData[iIndex];
	}

	return true;
}

bool KServerSNMaker::MakeServerSN( IN __int64& iSN, OUT unsigned char* pcharData )
{
	if( pcharData == NULL )
	{
		return false;
	}

	// __int64 형의 sn 을 std::string 형으로 변환한다.
	std::string strSN = boost::str( boost::format( "%d" ) % iSN );

	// sn 에 대한 check sum 을 구한다.
	std::string strCheckSum = GetCRCFromString( strSN );
	if( Change_CheckSum_8byteTo4byte( strCheckSum ) == false )
	{
		return false;
	}

	// Server SN 을 char 배열에 넣는다.(0~7)
	if( InsertString_ServerSN( iSN, pcharData ) == false )
	{
		return false;
	}

	// Check Sum 을 char 배열에 넣는다.(8~11)
	if( InsertString_CheckSum( strCheckSum, pcharData ) == false )
	{
		return false;
	}

	// char 배열을 섞는다
	if( ShuffleData( pcharData ) == false )
	{
		return false;
	}

	return true;
}

bool KServerSNMaker::CheckServerSN( IN unsigned char* pcharData, OUT __int64& iSN )
{
	if( pcharData == NULL )
	{
		return false;
	}

	// 섞여있는 char 배열을 원상복구 시킨다.
	if( UnShuffleData( pcharData ) == false )
	{
		return false;
	}

	std::string strSN;
	std::string strOldCheckSum;
	std::string strNewCheckSum;

	// char 배열에서 SN을 뽑아낸다. 넣는다.(0~7)
	if( Extract_ServerSN( pcharData, strSN, iSN ) == false )
	{
		return false;
	}

	// char 배열에서 SN을 뽑아낸다. 넣는다.(8~11)
	if( Extract_CheckSum( pcharData, strOldCheckSum ) == false )
	{
		return false;
	}

	// sn 에 대한 check sum 을 구한다.
	strNewCheckSum = GetCRCFromString( strSN );
	if( Change_CheckSum_8byteTo4byte( strNewCheckSum ) == false )
	{
		return false;
	}

	// check sum 을 비교한다.
	if( strOldCheckSum.compare( strNewCheckSum ) != 0 )
	{
		return false;
	}

	return true;
}

bool KServerSNMaker::InsertString_ServerSN( IN __int64& iSN, IN OUT unsigned char* pcharData )
{
	if( pcharData == NULL )
	{
		return false;
	}

	// char 배열에 sn 값을 넣는다

	__int64 iSNcopy = 0;

	iSNcopy = iSN & 0xff00000000000000;
	iSNcopy = iSNcopy >> 56;
	pcharData[0] = (unsigned char)iSNcopy;

	iSNcopy = iSN & 0x00ff000000000000;
	iSNcopy = iSNcopy >> 48;
	pcharData[1] = (unsigned char)iSNcopy;

	iSNcopy = iSN & 0x0000ff0000000000;
	iSNcopy = iSNcopy >> 40;
	pcharData[2] = (unsigned char)iSNcopy;

	iSNcopy = iSN & 0x000000ff00000000;
	iSNcopy = iSNcopy >> 32;	
	pcharData[3] = (unsigned char)iSNcopy;

	iSNcopy = iSN & 0x00000000ff000000;
	iSNcopy = iSNcopy >> 24;
	pcharData[4] = (unsigned char)iSNcopy;

	iSNcopy = iSN & 0x0000000000ff0000;
	iSNcopy = iSNcopy >> 16;
	pcharData[5] = (unsigned char)iSNcopy;

	iSNcopy = iSN & 0x000000000000ff00;
	iSNcopy = iSNcopy >> 8;
	pcharData[6] = (unsigned char)iSNcopy;

	iSNcopy = iSN & 0x00000000000000ff;
	pcharData[7] = (unsigned char)iSNcopy;

	return true;
}

bool KServerSNMaker::InsertString_CheckSum( IN std::string& strCheckSum, IN OUT unsigned char* pcharData )
{
	if( pcharData == NULL )
	{
		return false;
	}

	if( ( strCheckSum.size() != 4 ) )
	{
		return false;
	}

	// char 배열에 check sum 값을 넣는다
	pcharData[8] = strCheckSum[0];
	pcharData[9] = strCheckSum[1];
	pcharData[10] = strCheckSum[2];
	pcharData[11] = strCheckSum[3];

	return true;
}

bool KServerSNMaker::Extract_ServerSN( IN unsigned char* pcharData, OUT std::string& strSN, OUT __int64& iSN )
{
	if( pcharData == NULL )
	{
		return false;
	}

	// char 배열에서 sn 값을 얻자 (0~7)
	__int64 iSNResult = 0;
	__int64 iSNTemp = 0;

	iSNTemp = (__int64)pcharData[0];
	iSNTemp = iSNTemp << 56;
	iSNResult = iSNResult | (iSNTemp & 0xff00000000000000);

	iSNTemp = (__int64)pcharData[1];
	iSNTemp = iSNTemp << 48;
	iSNResult = iSNResult | (iSNTemp & 0x00ff000000000000);

	iSNTemp = (__int64)pcharData[2];
	iSNTemp = iSNTemp << 40;
	iSNResult = iSNResult | (iSNTemp & 0x0000ff0000000000);

	iSNTemp = (__int64)pcharData[3];
	iSNTemp = iSNTemp << 32;
	iSNResult = iSNResult | (iSNTemp & 0x000000ff00000000);

	iSNTemp = (__int64)pcharData[4];
	iSNTemp = iSNTemp << 24;
	iSNResult = iSNResult | (iSNTemp & 0x00000000ff000000);

	iSNTemp = (__int64)pcharData[5];
	iSNTemp = iSNTemp << 16;
	iSNResult = iSNResult | (iSNTemp & 0x0000000000ff0000);

	iSNTemp = (__int64)pcharData[6];
	iSNTemp = iSNTemp << 8;
	iSNResult = iSNResult | (iSNTemp & 0x000000000000ff00);

	iSNTemp = (__int64)pcharData[7];
	iSNResult = iSNResult | (iSNTemp & 0x00000000000000ff);

	// __int64 형의 sn 을 std::string 형으로 변환한다.
	strSN = boost::str( boost::format( "%d" ) % iSNResult );

	iSN = iSNResult;

	return true;
}

bool KServerSNMaker::Extract_CheckSum( IN unsigned char* pcharData, OUT std::string& strCheckSum )
{
	if( pcharData == NULL )
	{
		return false;
	}

	// char 배열에서 check sum 값을 얻자 (8~11)
	for( int iIndex = 8 ; iIndex < 12 ; ++iIndex )
	{
		strCheckSum.push_back( pcharData[iIndex] );
	}

	return true;
}

bool KServerSNMaker::Change_CheckSum_8byteTo4byte( IN OUT std::string& strSN )
{
	if( strSN.size() != 8 )
	{
		return false;
	}

	unsigned char charTemp = 0;
	int shortValue = 0;
	int shortTemp = 0;
	std::string strResult;

	for( int iIndex = 0 ; iIndex < 8 ; ++iIndex )
	{
		charTemp = strSN[iIndex];
		shortTemp = Hex2Dec( charTemp );
		if( iIndex % 2 == 0 ) // 첫번째 일때 쉬프트 연산을 해야한다
		{
			shortValue = shortTemp << 4;
		}
		else
		{
			shortValue = shortValue | shortTemp;
			charTemp = (unsigned char)shortValue;
			strResult.push_back( charTemp );
		}
	}

	strSN = strResult;

	return true;
}

unsigned char KServerSNMaker::Hex2Dec( unsigned char hex) // 16진수를 10진수로 변환
{
	hex&=~(0x30);
	if(hex&0x40)
	{
		hex&=0x0f;
		hex+=9;
	}
	return hex;
}

bool KServerSNMaker::IsEmptyServerSN( IN unsigned char* pcharData )
{
	if( pcharData == NULL )
	{
		return true;
	}

	// 모두 0 이면 비어있다고 판단
	bool bRet = false;
	int nCount = 0;
	for( int iIndex = 0 ; iIndex < 12 ; ++iIndex )
	{
		if( pcharData[iIndex] == 0 )
		{
			++nCount;
		}
	}

	if( nCount == 12 )
	{
		bRet = true;
	}

	return bRet;
}

//#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}