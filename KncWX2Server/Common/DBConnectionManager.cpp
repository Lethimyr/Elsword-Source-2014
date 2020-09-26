#include "DBConnectionManager.h"
#include "BaseServer.h"

#include "cryptopp/des.h"
#include "cryptopp/modes.h"
#include "cryptopp/hmac.h"
#include "cryptopp/md5.h"
#include "KncSecurity/ByteStream.h"

#undef max
#include <boost/random.hpp>

// c++파일에만 두자!
#define MASK_FIRST( key )		( key & 0x000000ff )
#define MASK_SECOND( key )		( ( key >> 8 ) & 0x000000ff )
#define MASK_THIRD( key )		( ( key >> 16 ) & 0x000000ff )
#define MASK_CRYPTKEY( key )	( ( key >> 24 ) & 0x000000ff )

#define CRYPTO_KEY_SIZE 8
#define CRYPTO_IV_SIZE	8
#define BYTE_TO_STRING_SIZE 3

//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY	// 빌드 오류로 해외팀 추가

// 외부에서 참조당하지 못하도록 anonymous namespace 안에 선언. <---- 이거 나중에 서버 코드에서 빼자! ( lua스크립트는 어떨까? )
namespace
{
	class KCursor
	{
	public:
		KCursor()
		{
			m_uiPos = 0;
		}

		unsigned int Move( const unsigned int uiMoveCount )
		{
			m_uiPos += uiMoveCount;
			return GetCurrentPos();
		}

		int GetCurrentPos() const { return m_uiPos; }

	private:
		unsigned int m_uiPos;
	};

	std::string ConvertToString( const unsigned char ucData )
	{
		const std::string strTemp = boost::str( boost::format( "%03d" ) % static_cast<int>(ucData) );
		return strTemp;
	}

	unsigned char ConvertToChar( const std::string& strData )
	{
		unsigned char ucTemp = static_cast<unsigned char>( atoi( strData.c_str() ) );
		return ucTemp;
	}

	//////////////////////////////////////////////////////////////////////////
	// 기초적인 암호화
	void EncryptString( const std::string& strSource, std::string& strResult )
	{
		const char cFirst  = rand() % CHAR_MAX;
		const char cSecond = rand() % CHAR_MAX;
		const char cThird  = rand() % CHAR_MAX;
		const char cKeyCrypt = rand() % CHAR_MAX;

		const char cFirstRaelKey  = cFirst  ^ cKeyCrypt;
		const char cSecondRealKey = cSecond ^ cKeyCrypt;
		const char cThirdRealKey  = cThird  ^ cKeyCrypt;

		// 제일 첫번째 바이트에 암호화된걸 넣자!
		strResult += ConvertToString( cFirst );
		strResult += ConvertToString( cSecond );
		strResult += ConvertToString( cThird );
		strResult += ConvertToString( cKeyCrypt );

		for( unsigned int i = 0; i < strSource.size(); ++i )
		{
			// 암호화
			char cEncryptData = 0;
			cEncryptData = cFirstRaelKey  ^ strSource[i];
			cEncryptData = cSecondRealKey ^ cEncryptData;
			cEncryptData = cThirdRealKey  ^ cEncryptData;

			strResult += ConvertToString( cEncryptData );
		}
	}

	bool DecryptString( const std::string& strSource, std::string& strResult )
	{
		KCursor kCursor;
		std::string strTemp;

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cFirst  = ConvertToChar( strTemp );

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cSecond = ConvertToChar( strTemp );

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cThird  = ConvertToChar( strTemp );

		strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
		kCursor.Move( BYTE_TO_STRING_SIZE );
		char cKeyCrypt = ConvertToChar( strTemp );

		const char cFirstRaelKey  = cFirst  ^ cKeyCrypt;
		const char cSecondRealKey = cSecond ^ cKeyCrypt;
		const char cThirdRealKey  = cThird  ^ cKeyCrypt;

		for( unsigned int i = kCursor.GetCurrentPos(); i < strSource.size(); i += BYTE_TO_STRING_SIZE )
		{
			char cData = ConvertToChar( strSource.substr( i, BYTE_TO_STRING_SIZE ) );

			// 복호화
			char cEncryptData = 0;
			cEncryptData = cFirstRaelKey  ^ cData;
			cEncryptData = cSecondRealKey ^ cEncryptData;
			cEncryptData = cThirdRealKey  ^ cEncryptData;

			strResult.push_back( cEncryptData );
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// des알고리즘 암호화
	KncSecurity::KByteStream MakeCryptKey()
	{
		//static boost::mt19937 rng;
		//static boost::uniform_int<DWORD> uint32( 1, std::numeric_limits<byte>::max() );   // 1 이상. SPI 0은 생성자에서 이미 설정했다.
		//static boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die(rng, uint32);

		KncSecurity::KByteStream bsKey;
		bsKey.clear();
		bsKey.resize( CRYPTO_KEY_SIZE );

		KncSecurity::KByteStream::iterator sit;
		for( sit = bsKey.begin(); sit != bsKey.end(); ++sit )
		{
			const byte randomValue = rand() % CHAR_MAX;//static_cast<byte>( die() );
			*sit = randomValue;
		}

		return bsKey;
	}

	KncSecurity::KByteStream GenerateIV()
	{
		KncSecurity::KByteStream bsIV;
		bsIV.clear();
		bsIV.resize( CRYPTO_IV_SIZE, 'A' + rand() % 40 );
		return bsIV;

		//switch( rand() % 5 ) {
		//case 0: bsIV_.Assign( "임시로 만든 아이브이", 8 ); break;
		//case 1: bsIV_.Assign( "crash가 또 발생할 수 있음", 8 ); break;
		//case 2: bsIV_.Assign( "만약 또 발생하지 않는다면", 8 ); break;
		//case 3: bsIV_.Assign( "GetIVSize로 크기를 넣어주는 부분을", 8 ); break;
		//case 4: bsIV_.Assign( "의심해볼 수 있음", 8 ); break;
		//default: bsIV_.Assign( "절대 발생하지 않아야 할 코드", 8 ); break;
		//}

		//GetRandomGenerator().GenerateBlock( &bsIV_[0], GetSA().GetIVSize() );
	}

	void EncryptStirngUseDes( const std::string& strSource, std::string& strResult )
	{
		// 암호화키를 랜덤하게 얻자!
		KncSecurity::KByteStream bsKey = MakeCryptKey();
		KncSecurity::KByteStream bsIV = GenerateIV();

		// 암호화할 문자열을 버퍼에 담자!
		KncSecurity::KByteStream payLoad;
		for( unsigned int i = 0; i < strSource.size(); ++i )
		{
			payLoad += strSource[i];
		}

		// 마지막 자리를 패딩으로 채우자!
		while( ( payLoad.length() % CRYPTO_KEY_SIZE ) != 0 )
		{
			payLoad += '@';
		}

		// DES알고리즘 암호화!
		CryptoPP::CBC_Mode<CryptoPP::DES>::Encryption encryptor( &bsKey[0], bsKey.length(), &bsIV[0] );
		KncSecurity::KByteStream crypt;
		crypt.resize( payLoad.length() );
		encryptor.ProcessData( &crypt[0], payLoad.c_str(), payLoad.length() );

		// crypt키를 젤 앞에 붙이자!
		for( unsigned int ui = 0; ui < bsKey.length(); ++ui )
		{
			strResult += ConvertToString( bsKey[ui] );
		}

		// IV값을 그 다음에 붙이자!
		for( unsigned int ui = 0; ui < bsIV.length(); ++ui )
		{
			strResult += ConvertToString( bsIV[ui] ^ bsKey[ui] ); // IV값이 너무 일정한 패턴이므로 Key와 xor연산을 한번 한다.
		}

		// 그 뒤에 바로 암호화된 payload를 붙이자!
		KncSecurity::KByteStream::iterator mitBS;
		for( mitBS = crypt.begin(); mitBS != crypt.end(); ++mitBS )
		{
			strResult += ConvertToString( *mitBS );
		}
	}

	bool DecryptStringUseDes( const std::string& strSource, std::string& strResult )
	{
		if( strSource.length() < CRYPTO_KEY_SIZE * BYTE_TO_STRING_SIZE )
			return false;

		KncSecurity::KByteStream bsKey;
		KncSecurity::KByteStream bsIV;

		// 암호화된 문자열에서 암호화키값을 뽑아내자!
		KCursor kCursor;
		for( unsigned int i = 0; i < CRYPTO_KEY_SIZE; ++i )
		{
			std::string strTemp;
			strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
			kCursor.Move( BYTE_TO_STRING_SIZE );
			const char cTemp = ConvertToChar( strTemp );
			bsKey += cTemp;
		}

		// 암호화된 문자열에서 IV값을 뽑아내자!
		for( unsigned int i = 0; i < CRYPTO_IV_SIZE; ++i )
		{
			std::string strTemp;
			strTemp = strSource.substr( kCursor.GetCurrentPos(), BYTE_TO_STRING_SIZE );
			kCursor.Move( BYTE_TO_STRING_SIZE );
			const char cTemp = ConvertToChar( strTemp );
			bsIV += cTemp ^ bsKey[i]; // Key값과 xor를 한번 해야 진짜 IV값이 나온다.
		}

		// 암호화된 본문 문자열을 따로 뽑아내자!
		KncSecurity::KByteStream crypt;
		for( unsigned int i = kCursor.GetCurrentPos(); i < strSource.size(); i += BYTE_TO_STRING_SIZE )
		{
			crypt += ConvertToChar( strSource.substr( i, BYTE_TO_STRING_SIZE ) );
		}

		// 복호화 작업!
		CryptoPP::CBC_Mode<CryptoPP::DES>::Decryption decryptor( &bsKey[0], bsKey.length(), &bsIV[0] );
		KncSecurity::KByteStream payload;
		payload.resize( crypt.length() );
		decryptor.ProcessData( &payload[0], crypt.c_str(), crypt.length() );

		// 복호화가 완료된 문자열을 따로 정리하자!
		KncSecurity::KByteStream::iterator mitBS;
		for( mitBS = payload.begin(); mitBS != payload.end(); ++mitBS )
		{
			std::string strData;
			strData += static_cast<char>(*mitBS);
			if( strData == "@" )
				continue;

			strResult += strData;
		}

		return true;
	}
}


KDBConnectionManager::KDBConnectionManager()
{
}


KDBConnectionManager::~KDBConnectionManager(void)
{
}

bool KDBConnectionManager::GetDBConnectionInfoFromManagerDB( IN const int iServerType, 
															 IN const wchar_t* szConnStr, 
															 IN const bool bDBConnStr, 
															 OUT std::vector< KDBConnectionInfo >& vecDBConnectionInfo )
{
	vecDBConnectionInfo.clear();

	if( m_kODBC.DriverConnect( szConnStr, bDBConnStr ) == false )
	{
		START_LOG( cerr, L"DB접속 실패!" )
			<< BUILD_LOG( bDBConnStr )
			<< END_LOG;
		return false;
	}
	
	DO_QUERY_NO_PROFILE( L"exec dbo.temp_get_info", L"%d", % iServerType );

	while( m_kODBC.Fetch() )
	{
		std::wstring wstrDBString;
		int iServerGroupID = 0;
		KDBConnectionInfo kInfo;
		FETCH_DATA( iServerGroupID
			>> kInfo.m_iDBType
			>> wstrDBString
			>> kInfo.m_iThreadCount
			);

		// 서버군 값이 -1이면 무조건 접속처리
		if( iServerGroupID != -1 )
		{
			if( iServerGroupID != KBaseServer::GetKObj()->GetServerGroupID() )
				continue;
		}

		// 복호화
		if( DecryptDBString( wstrDBString, kInfo.m_wstrConnectionString ) == false )
		{
			START_LOG( cerr, L"문자열 해석 실패!" )
				<< BUILD_LOG( wstrDBString )
				<< END_LOG;
			continue;
		}

		vecDBConnectionInfo.push_back( kInfo );
	}

	if( vecDBConnectionInfo.empty() )
	{
		START_LOG( cerr, L"매니저 DB로부터 정보를 가져오지 못하였습니다!" )
			<< BUILD_LOG( iServerType )
			<< END_LOG;
		return false;
	}
	
	return true;

end_proc:
	START_LOG( cerr, L"쿼리 호출 실패!" )
		<< END_LOG;
	return false;
}

bool KDBConnectionManager::DecryptDBString( IN const std::wstring& wstrDBString, OUT std::wstring& wstrDecryptString )
{
	const std::string strStringBuffer = KncUtil::toNarrowString( wstrDBString );
	std::string strDecryptString;
	if( DecryptStringUseDes( strStringBuffer, strDecryptString ) == false )
	{
		START_LOG( cerr, L"복호화 실패!" )
			<< BUILD_LOG( wstrDBString )
			<< END_LOG;
		return false;
	}
	
	wstrDecryptString = KncUtil::toWideString( strDecryptString );
	return true;
}


#endif SERV_DB_CONNECTION_SECURITY	// 빌드 오류로 해외팀 추가
//}}