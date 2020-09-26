#include "SecureBuffer.h"
#include "SecurityAssociation.h"
#include "SADatabase.h"
#include "dbg/dbg.hpp"
#include "KncSecurity.h"

NAMESPACE_BEGIN( KncSecurity )

KSecureBuffer::KSecureBuffer( SpiType nSPI ) : m_nSPIndex( nSPI )
{
}

KSecureBuffer::KSecureBuffer( const KSecureBuffer& buf )
:
    m_bsBuf     ( buf.m_bsBuf ),
    m_nSPIndex  ( buf.m_nSPIndex )
{
}

KSecureBuffer::KSecureBuffer( SpiType nSPI, const void* pRecvData, unsigned long nBytes )
{
    m_bsBuf.assign( reinterpret_cast< const unsigned char* >( pRecvData ), nBytes );
    m_nSPIndex = nSPI;
}

KSecureBuffer::~KSecureBuffer()
{
}

// 나중에 없어질 것 같다. KEvent 포인터만 받아서 생성되도록 하자.
bool KSecureBuffer::Create( const void* pData, unsigned long nBytes )
{
    return Create( KByteStream( reinterpret_cast<const unsigned char*>(pData), nBytes ) );
}

bool KSecureBuffer::Create( const KByteStream& payload )
{
	// 이전의 데이터는 모두 삭제
	m_bsBuf.erase();

	// 버퍼 공간을 미리 확보한다.
	m_bsBuf.reserve( GetMaxSecureSize( payload ) );

	// 데이터 추가 : SPI
	m_bsBuf.Append( m_nSPIndex );

	// 데이터 추가 : sequence number
	m_bsBuf.Append( GetSA().GetSequenceNum() );

	// IV를 매번 랜덤하게 생성한다.
	KByteStream iv;
	GenerateIV( iv );
	m_bsBuf.append( iv );

	// 생성된 IV로 암호화한다. (padding과 padding 사이즈까지 암호화함)
	KByteStream crypt = GenerateCrypt( payload, iv );
	if( crypt.empty() )
		return false;

	// 데이터 추가 : payload, padding, padding size
	m_bsBuf.append( crypt );

	// 지금까지 추가된 데이터를 인풋으로 ICV 생성
	KByteStream icv = GenerateICV( m_bsBuf );
	if( icv.empty() )
		return false;

	// 데이터 추가 : ICV
	m_bsBuf.append( icv );

	// SA의 시퀀스 번호를 증가시켜준다. (다음에 보낼 패킷의 sequence number임.)
	GetSA().IncrSequenceNum();    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Copy a secure KByteStream

//void KSecureBuffer::Set( const KByteStream& secureBuffer )
//{
//    m_bsBuf = secureBuffer;
//    m_nSPIndex = 0;
//}

///////////////////////////////////////////////////////////////////////////////
//
// Validate a secure KByteStream by examining the authentication data

bool KSecureBuffer::IsAuthentic() const
{
	// 데이터에서 SPI를 얻어오면서, 현재 SADB에 있는 SPI인지 확인한다.
	SpiType nSPIndex = 0;
	if( !IsValidSPIndex( nSPIndex ) )
		return false;

	// 얻어온 SPI를 멤버 변수에 기억한다.
	//m_nSPIndex = nSPIndex;    // 060302. florist. 클라이언트와 서버간 SPindex가 달라도 처리가능하도록 수정.

	// Check packet size
	if( !IsValidSize() )
		return false;

	// Validate the Integrity Check Value (ICV)
	if( !IsValidICV() )
		return false;

	// Check sequence number
	SeqType nSequenceNum;
	if( !IsValidSequenceNum( nSequenceNum ) )
		return false;
    
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Extract the payload from a secure KByteStream. Returns false if the payload
// is invalid or cannot be retrieved. Assumes the KByteStream has already been
// authenticated.

bool KSecureBuffer::GetPayload( KByteStream& payload ) const
{
	_JIF( IsAuthentic(), false );

    // 데이터에서 IV를 얻어낸다. IV는 SPI와 SeqNum 뒤에 위치한다.
    unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
    unsigned long nSize = GetSA().GetIVSize();
    if( nPos + nSize > m_bsBuf.length() )
        return false;
    KByteStream iv = m_bsBuf.substr( nPos, nSize );

    // Fill in the remaining portion of the IV with zero bytes
    // 얻어낸 IV 뒤에 남은 자리들을 0으로 초기화한다.
    // 얻어낸 IV도 이미 GetSA().GetIVSize()의 길이다.
    iv.resize( GetSA().GetIVSize() );

    // 데이터에서 복호화 할 부분을 추출한다. IV 뒤에 있으며, 제일 끝의 ICV는 빼야한다.
    nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
    if( nPos + GetSA().GetICVSize() > m_bsBuf.length() )
        return false;
    nSize = m_bsBuf.length() - nPos - GetSA().GetICVSize();
    KByteStream crypt = m_bsBuf.substr( nPos, nSize );

    // 복호화한다.
    payload.clear();
    payload = GetSA().Decryption( crypt, iv );
    if( payload.empty() ) 
        return false;

    // padding 길이를 얻어내고, padding의 값이 올바른지 확인한다.
    unsigned long nPadBytes;
    if( !IsValidPadding( payload, nPadBytes ) )
        return false;

    // padding과 padding size 바이트를 제거한다.
    _JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
    payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Extract the payload from a secure KByteStream. This function is designed to
// work with socket recv.

bool KSecureBuffer::GetPayload( char* pPayload, int* pnBytes )
{
    _JIF( pPayload != NULL, false );
    _JIF( pnBytes != NULL, false );
    KByteStream buf;
    if( !GetPayload( buf ) )
        return false;

    *pnBytes = buf.length();
    buf.copy( reinterpret_cast< unsigned char* >( pPayload ), *pnBytes );
    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Marks a given KByteStream as authenticated and accepted. Automatically adjusts
// the replay window, so that IsAuthentic() will no longer validate the
// KByteStream correctly. Call this function only after calling IsAuthentic() and
// GetPayload(), but prior to calling IsAuthentic() on the next packet.

void KSecureBuffer::SetAccepted()
{
    SeqType nSequenceNum;
    bool bSuccess = IsValidSequenceNum( nSequenceNum );
    JIF( bSuccess );

    // Update the replay window
    GetSA().UpdateReplayWindow( nSequenceNum );
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 10. 15  최육사	Sequence Number ( not have replay window mask )

bool KSecureBuffer::Create_notRWM( const KByteStream& payload )
{
	// 이전의 데이터는 모두 삭제
	m_bsBuf.erase();

	// 버퍼 공간을 미리 확보한다.
	m_bsBuf.reserve( GetMaxSecureSize( payload ) );

	// 데이터 추가 : SPI
	m_bsBuf.Append( m_nSPIndex );

	// 데이터 추가 : sequence number
	m_bsBuf.Append( GetSA().GetSequenceNum() );

	// IV를 매번 랜덤하게 생성한다.
	KByteStream iv;
	GenerateIV( iv );
	m_bsBuf.append( iv );

	// 생성된 IV로 암호화한다. (padding과 padding 사이즈까지 암호화함)
	KByteStream crypt = GenerateCrypt( payload, iv );
	if( crypt.empty() )
		return false;

	// 데이터 추가 : payload, padding, padding size
	m_bsBuf.append( crypt );

	// 지금까지 추가된 데이터를 인풋으로 ICV 생성
	KByteStream icv = GenerateICV( m_bsBuf );
	if( icv.empty() )
		return false;

	// 데이터 추가 : ICV
	m_bsBuf.append( icv );

	// SPIndex가 0이 아닌경우에만 sequence number를 증가!
	// SA의 시퀀스 번호를 증가시켜준다. (다음에 보낼 패킷의 sequence number임.)
	if( !IsDefaultSPIndex() )
	{
		GetSA().IncrSequenceNum();
	}

	return true;
}

bool KSecureBuffer::IsAuthentic_notRWM( bool bCheckSequenceNum /*= true*/ )
{
	// 데이터에서 SPI를 얻어오면서, 현재 SADB에 있는 SPI인지 확인한다.
	SpiType nSPIndex = 0;
	if( !IsValidSPIndex( nSPIndex ) )
		return false;

	// 얻어온 SPI를 멤버 변수에 기억한다.
	//m_nSPIndex = nSPIndex;    // 060302. florist. 클라이언트와 서버간 SPindex가 달라도 처리가능하도록 수정.

	// Check packet size
	if( !IsValidSize() )
		return false;

	// Validate the Integrity Check Value (ICV)
	if( !IsValidICV() )
		return false;

	// SPIndex가 0이 아닌경우에만 sequence number를 체크! - 체크할지 안할지를 외부에서 전달인자로 결정가능
	if( !IsDefaultSPIndex()  &&  bCheckSequenceNum )
	{
		// Check sequence number
		SeqType nSequenceNum;
		if( !IsValidSequenceNum_notRWM( nSequenceNum ) )
			return false;
	}

	return true;
}

bool KSecureBuffer::GetPayload_notRWM( KByteStream& payload, bool bAuthentic /*= true*/, bool bCheckSequenceNum /*= true*/ )
{
	//{{ 2009. 10. 15  최육사	인증과정을 건너뛰는 기능 추가
	if( bAuthentic )
	{
		_JIF( IsAuthentic_notRWM( bCheckSequenceNum ), false );
	}
	//}}

	// 데이터에서 IV를 얻어낸다. IV는 SPI와 SeqNum 뒤에 위치한다.
	unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
	unsigned long nSize = GetSA().GetIVSize();
	if( nPos + nSize > m_bsBuf.length() )
		return false;
	KByteStream iv = m_bsBuf.substr( nPos, nSize );

	// Fill in the remaining portion of the IV with zero bytes
	// 얻어낸 IV 뒤에 남은 자리들을 0으로 초기화한다.
	// 얻어낸 IV도 이미 GetSA().GetIVSize()의 길이다.
	iv.resize( GetSA().GetIVSize() );

	// 데이터에서 복호화 할 부분을 추출한다. IV 뒤에 있으며, 제일 끝의 ICV는 빼야한다.
	nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
	if( nPos + GetSA().GetICVSize() > m_bsBuf.length() )
		return false;
	nSize = m_bsBuf.length() - nPos - GetSA().GetICVSize();
	KByteStream crypt = m_bsBuf.substr( nPos, nSize );

	// 복호화한다.
	payload.clear();
	payload = GetSA().Decryption( crypt, iv );
	if( payload.empty() ) 
		return false;

	// padding 길이를 얻어내고, padding의 값이 올바른지 확인한다.
	unsigned long nPadBytes;
	if( !IsValidPadding( payload, nPadBytes ) )
		return false;

	// padding과 padding size 바이트를 제거한다.
	_JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
	payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

	return true;
}

//}}
//////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// Returns a pointer to the beginning of the secure data. This function is
// designed to work with socket send().

const char* KSecureBuffer::GetDataPtr() const
{
    return( reinterpret_cast< const char* >( m_bsBuf.data() ) );
}

///////////////////////////////////////////////////////////////////////////////
//
// Returns the size of the secure KByteStream in bytes

unsigned long KSecureBuffer::GetSize() const
{
    return m_bsBuf.length();
}

const KSecurityAssociation& KSecureBuffer::GetSA() const
{
    return GetSADB().GetSA( m_nSPIndex );
}

KSecurityAssociation& KSecureBuffer::GetSA()
{
    return GetSADB().GetSA( m_nSPIndex );
}

void KSecureBuffer::GenerateIV( KByteStream& bsIV_ ) const
{
    bsIV_.clear();
    bsIV_.resize( GetSA().GetIVSize(), 'A' + rand() % 40 );

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

KByteStream KSecureBuffer::GenerateCrypt( const KByteStream& payload, const KByteStream& iv ) const
{
    KByteStream crypt( payload );

    // Append padding, if any
    KByteStream pad = GeneratePadding( payload );
    crypt.append( pad );

    // Append pad length
    crypt.Append( pad.length(), 1 );

    return GetSA().Encryption( crypt, iv );
}

///////////////////////////////////////////////////////////////////////////////
//
// Generate the padding bytes based on the given payload

KByteStream KSecureBuffer::GeneratePadding( const KByteStream& payload ) const
{
    // The size of the payload, the padding and the pad length (1 byte)
    // must be evenly divisible by nBlockBytes
    // 1블록은 무조건 패딩되어야 한다. 데이터 길이가 블록단위로 딱 맞으면 온전한 블록 하나를,
    // 딱 맞지 않으면 부족한 만큼을 채우기위한 불완전 블록 하나를 채운다.
    unsigned long nBlockBytes = GetSA().GetBlockSize();
    unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

    // Add some random padding to hide the true size of the payload
    // 그 다음, 데이터 사이즈를 숨기기 위해서 추가적으로 더할 패딩 블록 수를 잡는다.
    unsigned char nRand = 0; // GetRandomGenerator().GenerateByte();
    unsigned long nRandBlocks = 0;
    // 최대 extra block 개수가 5라고 치면, 5 + 1인 6으로 mod해야 [0,5]의 값이 나온다.
    // 여분 패딩 사이즈가 0이면, rand % 1이 되므로 값은 항상 0이다.
    nRandBlocks = nRand % (GetSA().GetMaxExtraPadBlocks() + 1);
    nPadBytes += nBlockBytes * nRandBlocks;

    // Create the padding KByteStream
    KByteStream pad;
    pad.resize( nPadBytes );

    // RFC 2406 says padding bytes are initialized with a series of 
    // one-byte integer values
    unsigned char* pData = &pad[0];
    for( unsigned long i = 1; i <= nPadBytes; ++i, ++pData )
        *pData = unsigned char( i );

    return pad;
}

///////////////////////////////////////////////////////////////////////////////
//
// Generate the Integrity Check Value of the given KByteStream

KByteStream KSecureBuffer::GenerateICV( const KByteStream& auth ) const
{
    return GetSA().GenerateICV( auth );
}

///////////////////////////////////////////////////////////////////////////////
//
// Determine the max size of the secure KByteStream based on the given payload

unsigned long KSecureBuffer::GetMaxSecureSize( const KByteStream& payload ) const
{
    unsigned long nSize = 0;
    nSize += sizeof( SpiType );         // SPI
    nSize += sizeof( SeqType );         // sequence number
    nSize += GetSA().GetIVSize();       // IV
    nSize += payload.size();            // payload
    nSize += GetMaxPadSize( payload );  // padding
    nSize += sizeof( unsigned char );   // pad length
    nSize += GetSA().GetICVSize();      // ICV
    return nSize;
}

unsigned long KSecureBuffer::GetMaxPadSize( const KByteStream& payload ) const
{
    // 블록 크기를 구한다.
    unsigned long nBlockBytes = GetSA().GetBlockSize();

    // 일단 기본적으로 패딩되어야 할 바이트 수를 구한다.
    unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

    // 여분 패딩 블록 수를 추가로 구해서 더해준다.
    nPadBytes += GetSA().GetMaxExtraPadBlocks() * nBlockBytes;

    LIF( ( payload.length() + 1 + nPadBytes ) % nBlockBytes  == 0 );
    LIF( nPadBytes < 255 );
    return nPadBytes;
}

bool KSecureBuffer::IsValidSize() const
{
    // Assume no padding for quick check
    if( m_bsBuf.length() < sizeof( SpiType ) +      // SPI
                        sizeof( SeqType ) +         // sequence number
                        GetSA().GetIVSize() +       // IV
                        sizeof( unsigned char ) +   // pad size
                        GetSA().GetICVSize() )      // ICV
        return false;

    // KByteStream meets minimum requires; other checks performed later
    return true;
}

bool KSecureBuffer::IsValidSPIndex( SpiType& nSPIndex ) const
{
    // 버퍼에 담겨있는 데이터의 SPI가 유효한지 검사한다.

    // 데이터의 크기가 SPI 사이즈보다도 작은 경우.
    if( m_bsBuf.length() < sizeof( SpiType ) ) return false;

    // 데이터에서 SPI를 뽑아낸다. 제일 처음에 들어있다.
    nSPIndex = *reinterpret_cast< const SpiType* >( m_bsBuf.data() );

    //return GetSADB().Find( nSPIndex );    // 060302. florist. 서버와 클라이언트간에 등록된 SPIndex가 달라도 무방하도록 수정.
    return GetSADB().Find( m_nSPIndex );
}

bool KSecureBuffer::IsValidSequenceNum( SeqType& nSequenceNum ) const
{
    // 데이터에서 Sequence Number를 읽는다. SPI 바로 뒤에 있다.
    nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

    return GetSA().IsValidSequenceNum( nSequenceNum );
}

//{{ 2009. 10. 15  최육사	LastSequenceNum저장
bool KSecureBuffer::IsValidSequenceNum_notRWM( SeqType& nSequenceNum )
{
	// 데이터에서 Sequence Number를 읽는다. SPI 바로 뒤에 있다.
	nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

	return GetSA().IsValidSequenceNum_notRWM( nSequenceNum );
}
//}}

///////////////////////////////////////////////////////////////////////////////
//
// Determines if the ICV is valid


bool KSecureBuffer::IsValidICV() const
{
    // 데이터 가장 마지막에 있는 ICV를 읽어온다.
    unsigned long nSize = GetSA().GetICVSize();
    unsigned long nPos = m_bsBuf.length() - nSize;
    KByteStream icv = m_bsBuf.substr( nPos, nSize );

    // ICV 값을 제외한 나머지 데이터 (ICV 계산의 인풋이 될 데이터)를 읽어온다.
    nSize = nPos;
    KByteStream auth = m_bsBuf.substr( 0, nSize );

    // ICV를 계산한다.
    KByteStream icvCompare = GetSA().GenerateICV( auth );

    // 계산한 ICV와 수신받은 ICV값을 비교한다.
    //return( icv == icvCompare );
    if( icv != icvCompare )
    {
        START_LOG( cerr, "SPI : " << m_nSPIndex << dbg::endl
            << "icv : " << icv << dbg::endl
            << "icvCompare : " << icvCompare << dbg::endl );

        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
//
// Determines if the given padding is valid

bool KSecureBuffer::IsValidPadding( const KByteStream& payload, unsigned long& nPadBytes ) const
{
    // 복호화를 마친 payload를 입력으로 받는다 (실제 데이터 + padding + padding size)
    
    // 최소한 padding size는 포함하고 있어야 한다.
    if( payload.length() == 0 )
        return false;

    // padding size를 얻어낸다. 가장 마지막 바이트에 위치한다.
    nPadBytes = *( payload.data() + payload.length() - 1 );

    // 데이터는 최소한 padding size만큼의 크기와 size 표시 바이트 크기보다는 커야 한다.
    if( nPadBytes + sizeof(unsigned char) > payload.length() )
        return false;

    // 데이터에서 padding 된 부분을 얻어낸다.
    unsigned long nPos = payload.length() - nPadBytes - sizeof(unsigned char);
    KByteStream pad = payload.substr( nPos, nPadBytes );

    // padding bytes가 올바르느지 확인한다. 1부터 시작하는 연속된 one-byte integer여야 한다.
    const unsigned char* pData = pad.data();
    for( unsigned long i = 1; i <= nPadBytes; ++i, ++pData )
    {
        if( *pData != unsigned char(i) )
            return false;
    }

    // Padding is good
    return true;
}

//{{
//////////////////////////////////////////////////////////////////////////
// SADatabase 를 Session 객체의 맴버로 사용 하기 위하여 추가된 변수
//////////////////////////////////////////////////////////////////////////

bool KSecureBuffer::Create_Local( const void* pData, unsigned long nBytes, KSADatabase* pSADatabase )
{
	return Create_Local( KByteStream( reinterpret_cast<const unsigned char*>(pData), nBytes ), pSADatabase );
}

bool KSecureBuffer::Create_Local( const KByteStream& payload, KSADatabase* pSADatabase )
{
	// 위의 Create 와 동일 파라미터만 추가된 버전
	if( pSADatabase == NULL )
	{
		return false;
	}

	m_bsBuf.erase();
	m_bsBuf.reserve( GetMaxSecureSize_Local( payload, pSADatabase ) );
	m_bsBuf.Append( m_nSPIndex );
	m_bsBuf.Append( pSADatabase->GetSA( m_nSPIndex ).GetSequenceNum() );

	KByteStream iv;
	GenerateIV_Local( iv, pSADatabase );
	m_bsBuf.append( iv );

	KByteStream crypt = GenerateCrypt_Local( payload, iv, pSADatabase );
	if( crypt.empty() )
		return false;

	m_bsBuf.append( crypt );

	KByteStream icv = GenerateICV_Local( m_bsBuf, pSADatabase );
	if( icv.empty() )
		return false;

	m_bsBuf.append( icv );

	pSADatabase->GetSA( m_nSPIndex ).IncrSequenceNum();    
	return true;
}

bool KSecureBuffer::GetPayload_Local( KByteStream& payload, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}
	_JIF( IsAuthentic_Local( pSADatabase ), false );

	unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
	unsigned long nSize = pSADatabase->GetSA( m_nSPIndex ).GetIVSize();
	if( nPos + nSize > m_bsBuf.length() )
		return false;
	KByteStream iv = m_bsBuf.substr( nPos, nSize );

	iv.resize( pSADatabase->GetSA( m_nSPIndex ).GetIVSize() );

	nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
	if( nPos + pSADatabase->GetSA( m_nSPIndex ).GetICVSize() > m_bsBuf.length() )
		return false;
	nSize = m_bsBuf.length() - nPos - pSADatabase->GetSA( m_nSPIndex ).GetICVSize();
	KByteStream crypt = m_bsBuf.substr( nPos, nSize );

	payload.clear();
	payload = pSADatabase->GetSA( m_nSPIndex ).Decryption( crypt, iv );
	if( payload.empty() ) 
		return false;

	unsigned long nPadBytes;
	if( !IsValidPadding( payload, nPadBytes ) )
		return false;

	_JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
	payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

	return true;
}

bool KSecureBuffer::GetPayload_Local( char* pPayload, int* pnBytes, KSADatabase* pSADatabase )
{
	_JIF( pPayload != NULL, false );
	_JIF( pnBytes != NULL, false );
	KByteStream buf;
	if( !GetPayload_Local( buf, pSADatabase ) )
		return false;

	*pnBytes = buf.length();
	buf.copy( reinterpret_cast< unsigned char* >( pPayload ), *pnBytes );
	return true;
}

void KSecureBuffer::SetAccepted_Local( KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return;
	}

	SeqType nSequenceNum;
	bool bSuccess = IsValidSequenceNum( nSequenceNum );
	JIF( bSuccess );

	pSADatabase->GetSA( m_nSPIndex ).UpdateReplayWindow( nSequenceNum );
}

bool KSecureBuffer::Create_notRWM_Local( const KByteStream& payload, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}

	m_bsBuf.erase();
	m_bsBuf.reserve( GetMaxSecureSize( payload ) );
	m_bsBuf.Append( m_nSPIndex );
	m_bsBuf.Append( pSADatabase->GetSA( m_nSPIndex ).GetSequenceNum() );

	KByteStream iv;
	GenerateIV_Local( iv, pSADatabase );
	m_bsBuf.append( iv );

	KByteStream crypt = GenerateCrypt_Local( payload, iv, pSADatabase );
	if( crypt.empty() )
		return false;

	m_bsBuf.append( crypt );

	KByteStream icv = GenerateICV_Local( m_bsBuf, pSADatabase );
	if( icv.empty() )
		return false;

	m_bsBuf.append( icv );

	if( !IsDefaultSPIndex() )
	{
		pSADatabase->GetSA( m_nSPIndex ).IncrSequenceNum();
	}

	return true;
}

bool KSecureBuffer::IsAuthentic_notRWM_Local( bool bCheckSequenceNum /*= true*/, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}

	SpiType nSPIndex = 0;
	if( !IsValidSPIndex_Local( nSPIndex, pSADatabase ) )
		return false;

	if( !IsValidSize_Local( pSADatabase ) )
		return false;

	if( !IsValidICV_Local( pSADatabase ) )
		return false;

	if( !IsDefaultSPIndex()  &&  bCheckSequenceNum )
	{
		SeqType nSequenceNum;
		if( !IsValidSequenceNum_notRWM_Local( nSequenceNum, pSADatabase ) )
			return false;
	}

	return true;
}

bool KSecureBuffer::GetPayload_notRWM_Local( KByteStream& payload, bool bAuthentic /*= true*/, bool bCheckSequenceNum /*= true*/, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return false;
	}

	if( bAuthentic )
	{
		_JIF( IsAuthentic_notRWM_Local( bCheckSequenceNum, pSADatabase ), false );
	}

	unsigned long nPos = sizeof( SpiType ) + sizeof( SeqType );
	unsigned long nSize = pSADatabase->GetSA( m_nSPIndex ).GetIVSize();
	if( nPos + nSize > m_bsBuf.length() )
		return false;
	KByteStream iv = m_bsBuf.substr( nPos, nSize );

	iv.resize( pSADatabase->GetSA( m_nSPIndex ).GetIVSize() );

	nPos = sizeof( SpiType ) + sizeof( SeqType ) + nSize;
	if( nPos + pSADatabase->GetSA( m_nSPIndex ).GetICVSize() > m_bsBuf.length() )
		return false;
	nSize = m_bsBuf.length() - nPos - GetSA().GetICVSize();
	KByteStream crypt = m_bsBuf.substr( nPos, nSize );

	payload.clear();
	payload = pSADatabase->GetSA( m_nSPIndex ).Decryption( crypt, iv );
	if( payload.empty() ) 
		return false;

	unsigned long nPadBytes;
	if( !IsValidPadding( payload, nPadBytes ) )
		return false;

	_JIF( payload.length() >= nPadBytes + sizeof(unsigned char), false );
	payload.resize( payload.length() - nPadBytes - sizeof(unsigned char) );

	return true;
}

void KSecureBuffer::GenerateIV_Local( KByteStream& bsIV_, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return;
	}

	bsIV_.clear();
	bsIV_.resize( pSADatabase->GetSA( m_nSPIndex ).GetIVSize(), 'A' + rand() % 40 );
}

KByteStream KSecureBuffer::GenerateCrypt_Local( const KByteStream& payload, const KByteStream& iv, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  KByteStream();
	}

	KByteStream crypt( payload );

	KByteStream pad = GeneratePadding_Local( payload, pSADatabase );
	crypt.append( pad );
	crypt.Append( pad.length(), 1 );

	return pSADatabase->GetSA( m_nSPIndex ).Encryption( crypt, iv );
}

KByteStream KSecureBuffer::GeneratePadding_Local( const KByteStream& payload, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  KByteStream();
	}

	unsigned long nBlockBytes = pSADatabase->GetSA( m_nSPIndex ).GetBlockSize();
	unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

	unsigned char nRand = 0;
	unsigned long nRandBlocks = 0;

	nRandBlocks = nRand % (pSADatabase->GetSA( m_nSPIndex ).GetMaxExtraPadBlocks() + 1);
	nPadBytes += nBlockBytes * nRandBlocks;

	KByteStream pad;
	pad.resize( nPadBytes );

	unsigned char* pData = &pad[0];
	for( unsigned long i = 1; i <= nPadBytes; ++i, ++pData )
		*pData = unsigned char( i );

	return pad;
}

KByteStream KSecureBuffer::GenerateICV_Local( const KByteStream& auth, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  KByteStream();
	}

	return pSADatabase->GetSA( m_nSPIndex ).GenerateICV( auth );
}

unsigned long KSecureBuffer::GetMaxSecureSize_Local( const KByteStream& payload, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  0;
	}

	unsigned long nSize = 0;
	nSize += sizeof( SpiType );         // SPI
	nSize += sizeof( SeqType );         // sequence number
	nSize += pSADatabase->GetSA( m_nSPIndex ).GetIVSize();       // IV
	nSize += payload.size();            // payload
	nSize += GetMaxPadSize_Local( payload, pSADatabase );  // padding
	nSize += sizeof( unsigned char );   // pad length
	nSize += pSADatabase->GetSA( m_nSPIndex ).GetICVSize();      // ICV
	return nSize;
}

unsigned long KSecureBuffer::GetMaxPadSize_Local( const KByteStream& payload, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  0;
	}

	unsigned long nBlockBytes = pSADatabase->GetSA( m_nSPIndex ).GetBlockSize();

	unsigned long nPadBytes = nBlockBytes - ( ( payload.length() + 1 ) % nBlockBytes );

	nPadBytes += pSADatabase->GetSA( m_nSPIndex ).GetMaxExtraPadBlocks() * nBlockBytes;

	LIF( ( payload.length() + 1 + nPadBytes ) % nBlockBytes  == 0 );
	LIF( nPadBytes < 255 );
	return nPadBytes;
}

bool KSecureBuffer::IsValidSize_Local( KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	if( m_bsBuf.length() < sizeof( SpiType ) +      // SPI
		sizeof( SeqType ) +         // sequence number
		pSADatabase->GetSA( m_nSPIndex ).GetIVSize() +       // IV
		sizeof( unsigned char ) +   // pad size
		pSADatabase->GetSA( m_nSPIndex ).GetICVSize() )      // ICV
		return false;

	return true;
}

bool KSecureBuffer::IsValidSPIndex_Local( SpiType& nSPIndex, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	if( m_bsBuf.length() < sizeof( SpiType ) ) return false;

	nSPIndex = *reinterpret_cast< const SpiType* >( m_bsBuf.data() );

	return pSADatabase->Find( m_nSPIndex );
}

bool KSecureBuffer::IsValidSequenceNum_Local( SeqType& nSequenceNum, KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

	return pSADatabase->GetSA( m_nSPIndex ).IsValidSequenceNum( nSequenceNum );
}

bool KSecureBuffer::IsValidSequenceNum_notRWM_Local( SeqType& nSequenceNum, KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	nSequenceNum = *reinterpret_cast< const SeqType* >( m_bsBuf.data() + sizeof( m_nSPIndex ) );

	return pSADatabase->GetSA( m_nSPIndex ).IsValidSequenceNum_notRWM( nSequenceNum );
}

bool KSecureBuffer::IsValidICV_Local( KSADatabase* pSADatabase ) const
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	unsigned long nSize = pSADatabase->GetSA( m_nSPIndex ).GetICVSize();
	unsigned long nPos = m_bsBuf.length() - nSize;
	KByteStream icv = m_bsBuf.substr( nPos, nSize );

	nSize = nPos;
	KByteStream auth = m_bsBuf.substr( 0, nSize );

	KByteStream icvCompare = pSADatabase->GetSA( m_nSPIndex ).GenerateICV( auth );
	if( icv != icvCompare )
	{
		START_LOG( cerr, "SPI : " << m_nSPIndex << dbg::endl
			<< "icv : " << icv << dbg::endl
			<< "icvCompare : " << icvCompare << dbg::endl );

		return false;
	}

	return true;
}

bool KSecureBuffer::IsAuthentic_Local( KSADatabase* pSADatabase )
{
	if( pSADatabase == NULL )
	{
		return  false;
	}

	SpiType nSPIndex = 0;
	if( !IsValidSPIndex_Local( nSPIndex, pSADatabase ) )
		return false;

	if( !IsValidSize_Local( pSADatabase ) )
		return false;

	if( !IsValidICV_Local( pSADatabase ) )
		return false;

	SeqType nSequenceNum;
	if( !IsValidSequenceNum_Local( nSequenceNum, pSADatabase ) )
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// SecureBuffer 를 Session 객체의 맴버로 사용 하기 위하여 추가된 변수
//////////////////////////////////////////////////////////////////////////
//}}

ImplOstmOperatorA( KByteStream );
ImplOstmOperatorA2W( KByteStream );
ImplOstmOperatorA( KSecureBuffer );

ImplToStringA( KSecureBuffer )
{
    return stm_ << "KSecureBuffer - " << m_bsBuf;
}

NAMESPACE_END