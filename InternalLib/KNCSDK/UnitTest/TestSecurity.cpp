#include <winsock2.h>
#include <boost/test/unit_test.hpp>

#include "KncSecurity/KncSecurity.h"
#include <iostream> // cout

void Security_GPGExample()
{
    KncSecurity::InitSecurity();

    KncSecurity::KSecureBuffer sb( 0 ); // SPI 0이면 기본 상수키를 사용한다.

    BOOST_CHECK( sb.Create( "payload", 7 ) );

    // Send the secure packet (fake call)
    SOCKET sock = INVALID_SOCKET;
    send( sock, sb.GetDataPtr(), sb.GetSize(), 0 );

    // Receive the secure packet (fake call)
    char* pData = new char [ 1024 ];
    recv( sock, pData, 1024, 0 );
    delete [] pData;
    pData = const_cast< char* >( sb.GetDataPtr() );
    int n = sb.GetSize();

    // Validate the packet
    KncSecurity::KSecureBuffer sbReceive( 0, pData, n );
    BOOST_CHECK( sbReceive.IsAuthentic() );

    // Extract the original payload
    BOOST_CHECK( sbReceive.GetPayload( pData, &n ) );
    KncSecurity::KByteStream payload( reinterpret_cast< unsigned char *>( pData ), n );
    KncSecurity::KByteStream verify;
    verify.Assign( "payload", 7 );
    BOOST_CHECK( payload == verify );

    std::cout << payload << std::endl
              << verify << std::endl;

    // Adjust the replay window
    sbReceive.SetAccepted();

    KncSecurity::ReleaseSecurity();
}

void ReceiveAndCheck( const KncSecurity::KSecureBuffer& sbSend, const KncSecurity::KByteStream& bsData )
{
    // Validate the packet
    KncSecurity::KSecureBuffer sbReceive( sbSend );
    BOOST_CHECK( sbReceive.IsAuthentic() );

    // Extract the original payload

    // payload를 얻어낼 버퍼를 마련한다.
    KncSecurity::KByteStream payload;
    BOOST_CHECK( sbSend.GetPayload( payload ) );

    BOOST_CHECK( payload == bsData );

    std::cout << "원본 : " << bsData << std::endl
              << "암호 : " << sbSend << std::endl
              << "복호 : " << payload << std::endl << std::endl << std::endl;

    // Adjust the replay window
    sbReceive.SetAccepted();
}

void Security_UseCaseSimulation()
{
    KncSecurity::InitSecurity();

    SpiType nSPI = 0;
    KncSecurity::KSecureBuffer sb( nSPI ); // SPI 0이면 기본 상수키를 사용한다.

    KncSecurity::KByteStream bsData;

    bsData.Assign( "ryuiqtqyeoqiueyqorqorqryqr", strlen("ryuiqtqyeoqiueyqorqorqryqr" ) );
    BOOST_CHECK( sb.Create( bsData ) );
    ReceiveAndCheck( sb, bsData );

    bsData.Assign( "기본 상수키 암호화", strlen("기본 상수키 암호화" ) );
    BOOST_CHECK( sb.Create( bsData ) );
    ReceiveAndCheck( sb, bsData );

    bsData.Assign( "다시한번 상수키 암호화", strlen("다시한번 상수키 암호화" ) );
    BOOST_CHECK( sb.Create( bsData ) );
    ReceiveAndCheck( sb, bsData );

    // change SA.
    const KncSecurity::KSecurityAssociation* pkSA = NULL;
    pkSA = KncSecurity::GetSADB().CreateNewSA( nSPI );
    KncSecurity::KSecureBuffer sb2( nSPI );

    bsData.Assign( "임의키로 변경한 암호화", strlen("임의키로 변경한 암호화" ) );
    BOOST_CHECK( sb2.Create( bsData ) );
    ReceiveAndCheck( sb2, bsData );

    bsData.Assign( "임의키로 변경한 암호화", strlen("임의키로 변경한 암호화" ) );
    BOOST_CHECK( sb2.Create( bsData ) );
    ReceiveAndCheck( sb2, bsData );

    KncSecurity::ReleaseSecurity();
}

#include "Serializer/Serializer.h"
#include "Serializer/SerBuffer.h"

void Security_SerializeSA()
{
    KncSecurity::InitSecurity();

    SpiType nSPI;
    const KncSecurity::KSecurityAssociation* pkSA = NULL;
    pkSA = KncSecurity::GetSADB().CreateNewSA( nSPI );

    KSerializer ks;
    KSerBuffer  kbuff;

    ks.BeginWriting( &kbuff );
    ks.Put( *pkSA );
    ks.EndWriting();

    KncSecurity::KSecurityAssociation kSA;

    ks.BeginReading( &kbuff );
    ks.Get( kSA );
    ks.EndReading();

    BOOST_CHECK( *pkSA == kSA );

    KncSecurity::ReleaseSecurity();
}

void TestSecurity()
{
    Security_GPGExample();
    Security_UseCaseSimulation();
    Security_SerializeSA();
}