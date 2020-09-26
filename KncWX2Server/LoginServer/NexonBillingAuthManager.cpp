#include <WinSock2.h>
#include "NexonBillingAuthManager.h"

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "NexonBillingAuth.h"
#include "LoginServer.h"
#include "LoginSimLayer.h"

DWORD KNexonBillingAuthManager::ms_dwHeartBeatGap( 1 * 60 * 1000 );
DWORD KNexonBillingAuthManager::ms_dwConnectionCheckGap( 1 * 60 * 1000 );

ImplToStringW( KNexonBillingAuthManager )
{
	return stm_;
}

ImplPfID( KNexonBillingAuthManager, PI_LOGIN_NX_AUTH );

ImplementSingleton( KNexonBillingAuthManager );

KNexonBillingAuthManager::KNexonBillingAuthManager()
{
	m_kNexonBillingAuthInfo.m_strIP.clear();
	m_kNexonBillingAuthInfo.m_usPort = 0;
	m_kNexonBillingAuthInfo.m_iDomain = 0;
	m_dwLastHeartBeatTick = ::GetTickCount();
	m_dwLastConnectionCheckTick = ::GetTickCount();
    m_iRecvCP = 0;
}

KNexonBillingAuthManager::~KNexonBillingAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KNexonBillingAuthManager::RegToLua()
{
	//class_< KNexonBillingAuthManager >( "KNexomBillingAuthManager" )
	//	.def( "dump",						KNexonBillingAuthManager::Dump )
	//	.def( "InitNexonBillingAuthInfo",	KNexonBillingAuthManager::InitNexonBillingAuthInfo )
	//	.def( "SetNumThread",				KNexonBillingAuthManager::Init );

	//decl( "NXBillingAuthManager", this );
	lua_tinker::class_add<KNexonBillingAuthManager>( g_pLua, "KNexonBillingAuthManager" );
	lua_tinker::class_def<KNexonBillingAuthManager>( g_pLua, "dump",					 &KNexonBillingAuthManager::Dump );
	lua_tinker::class_def<KNexonBillingAuthManager>( g_pLua, "InitNexonBillingAuthInfo", &KNexonBillingAuthManager::InitNexonBillingAuthInfo );
	lua_tinker::class_def<KNexonBillingAuthManager>( g_pLua, "SetNumThread",			 &KNexonBillingAuthManager::Init );

	lua_tinker::decl( g_pLua, "NXBillingAuthManager", this );
}

void KNexonBillingAuthManager::InitNexonBillingAuthInfo( const char* szNexonAuthIP, unsigned short usPort, int iDomain, const char*	szDomainName )
{
	m_kNexonBillingAuthInfo.m_strIP				= szNexonAuthIP;
	m_kNexonBillingAuthInfo.m_usPort			= usPort;
	m_kNexonBillingAuthInfo.m_iDomain			= iDomain;
	m_kNexonBillingAuthInfo.m_wstrDomainName	= KncUtil::toWideString_UTF8( std::string( szDomainName ) );

	return;
}

void KNexonBillingAuthManager::Init( int nThreadNum )
{
    m_iRecvCP = 0;

    //////////////////////////////////////////////////////////////////////////
    // thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
#ifdef _CONVERT_VS_2010
    m_spThreadRecv = boost::shared_ptr< KTThread< KNexonBillingAuthManager > >
		( new KTThread< KNexonBillingAuthManager >( *this, &KNexonBillingAuthManager::Recv, 50 ) );

    m_spThreadSend = boost::shared_ptr< KTThread< KNexonBillingAuthManager > >
        ( new KTThread< KNexonBillingAuthManager >( *this, &KNexonBillingAuthManager::Send, 100 ) );
#else
    m_spThreadRecv = boost::shared_ptr< KTThread< KNexonBillingAuthManager > >
        ( new KTThread< KNexonBillingAuthManager >( *this, KNexonBillingAuthManager::Recv, 50 ) );

    m_spThreadSend = boost::shared_ptr< KTThread< KNexonBillingAuthManager > >
        ( new KTThread< KNexonBillingAuthManager >( *this, KNexonBillingAuthManager::Send, 100 ) );
#endif _CONVERT_VS_2010

	KThreadManager::Init( nThreadNum );
}

KThread* KNexonBillingAuthManager::CreateThread()
{
	return new KNexonBillingAuth;
}

void KNexonBillingAuthManager::BeginThread()
{
    if( m_spThreadRecv )
    {
	    m_spThreadRecv->Begin();
    }

    if( m_spThreadSend )
    {
	    m_spThreadSend->Begin();
    }

	KThreadManager::BeginThread();

    if( !Connect() )
    {
        START_LOG( cerr, L"빌링 인증 서버 접속 실패." )
            << END_LOG;
    }
}

void KNexonBillingAuthManager::EndThread()
{
	KThreadManager::EndThread();

    if( m_spThreadSend )
    {
	    m_spThreadSend->End( 10000 );
    }
    
    if( m_spThreadRecv )
    {
	    m_spThreadRecv->End( 3000 );
    }
}

void KNexonBillingAuthManager::Tick()
{
    CheckConnection();

    KeepConnection();
}

void KNexonBillingAuthManager::QueueingSendPacket( const KNexonBillingAuthPacketPtr& spPacket )
{
    JIF( spPacket );

    KLocker lock( m_csSendQueue );

    m_kSendQueue.push( spPacket );
}

void KNexonBillingAuthManager::Recv()
{
	if( !IsConnected() )
	{
        return;
	}

    int ret;
    ret = ::recv( m_sock,
        m_cRecvBuffer + m_iRecvCP,
        MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH - m_iRecvCP,
        0 );

    START_LOG( clog, L"패킷 받음." )
        << BUILD_LOG( ret );

    if( ret == SOCKET_ERROR )
    {
        START_LOG( cerr, GET_WSA_MSG );
        CLOSE_SOCKET( m_sock );
        return;
    }

    if( ret == 0 )
    {
        START_LOG( cerr, L"소켓 연결이 끊어짐." )
            << END_LOG;

        CLOSE_SOCKET( m_sock );
        return;
    }

    m_iRecvCP += ret;

    while( m_iRecvCP >= 4 )
    {
        // 헤더(1) + 사이즈(2) + 타입(1) = 4
        // 모든 패킷이 4바이트 이상임

        unsigned short usLength;
        ::memcpy( &usLength, m_cRecvBuffer + 1, sizeof( usLength ) );

        usLength = ::ntohs( usLength );
        int iTotalPacketSize = usLength + 3;
        //if( iTotalPacketSize > MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH ) 김민성
		if( iTotalPacketSize >= MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH )
        {
            START_LOG( cerr, L"추출한 패킷 사이즈 이상." )
                << BUILD_LOG( iTotalPacketSize )
                << BUILD_LOG( MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH )
                << END_LOG;

            CLOSE_SOCKET( m_sock );
            return;
        }

        if( iTotalPacketSize > m_iRecvCP )
        {
            return;
        }

        MakeEventFromReceivedPacket();

        if( m_iRecvCP > iTotalPacketSize )
        {
            ::memmove( m_cRecvBuffer, m_cRecvBuffer + iTotalPacketSize, m_iRecvCP - iTotalPacketSize );
        }
        m_iRecvCP -= iTotalPacketSize;
    }
}

void KNexonBillingAuthManager::Send()
{
	if( !IsConnected() )
	{
        return;
	}

	int ret;
	char buf[MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH];
    KNexonBillingAuthPacketPtr spPacket;
	while( GetSendPacket( spPacket ) )
	{
		if( !spPacket )
		{
			START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( m_kSendQueue.size() )
				<< END_LOG;

			continue;
		}

		spPacket->WriteToBuffer( ( BYTE* )buf );
        DumpBuffer( ( BYTE* )buf, false );

        char* cCurBufPos = buf;
		int iRemainedSendSize = spPacket->GetTotalLength();
		while( iRemainedSendSize > 0 )
		{
			ret = ::send( m_sock,
				cCurBufPos,
				iRemainedSendSize,
				0 );

			if( ret == SOCKET_ERROR )
			{
				START_LOG( cwarn, GET_WSA_MSG );
				CLOSE_SOCKET( m_sock );
				return;
			}

			iRemainedSendSize -= ret;
            cCurBufPos += ret;
		}
	}
}

bool KNexonBillingAuthManager::Connect()
{
    m_iRecvCP = 0;
	m_sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성

	if( INVALID_SOCKET == m_sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		return false;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( m_kNexonBillingAuthInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kNexonBillingAuthInfo.m_usPort );

	int ret = ::connect( m_sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( m_kNexonBillingAuthInfo.m_strIP )
			<< BUILD_LOG( m_kNexonBillingAuthInfo.m_usPort )
			<< END_LOG;

		CLOSE_SOCKET( m_sock );
		return false;
	}

    KENX_INITIALIZE_BILLING_AUTH_NOT kPacketInit;
    kPacketInit.m_bytePacketType = 40;
    kPacketInit.m_byteGameSN = 29;
    kPacketInit.m_byteDomainSN = m_kNexonBillingAuthInfo.m_iDomain;
    kPacketInit.m_wstrDomainName = m_kNexonBillingAuthInfo.m_wstrDomainName;

    boost::shared_ptr< KNexonBillingAuthPacket > spPacket( new KNexonBillingAuthPacket );
    spPacket->Write( kPacketInit );
    QueueingSendPacket( spPacket );

    START_LOG( cout, L"넥슨 PC방 인증 서버 접속." );
	return true;
}

bool KNexonBillingAuthManager::IsConnected()
{
	return ( m_sock != INVALID_SOCKET );
}

void KNexonBillingAuthManager::KeepConnection()
{
	if( ::GetTickCount() - m_dwLastHeartBeatTick < ms_dwHeartBeatGap )
	{
		return;
	}

    m_dwLastHeartBeatTick = ::GetTickCount();

	// 허트빗 보내기
    KENX_ALIVE_NOT kPacketNot;
    kPacketNot.m_bytePacketType = 100;
    boost::shared_ptr< KNexonBillingAuthPacket > spPacket( new KNexonBillingAuthPacket );
    spPacket->Write( kPacketNot );
    QueueingSendPacket( spPacket );
}

void KNexonBillingAuthManager::CheckConnection()
{
	if( ::GetTickCount() - m_dwLastConnectionCheckTick < ms_dwConnectionCheckGap )
	{
		return;
	}

    m_dwLastConnectionCheckTick = ::GetTickCount();

	if( !IsConnected() )
	{
		Connect();
	}
}

bool KNexonBillingAuthManager::GetSendPacket( KNexonBillingAuthPacketPtr& spPacket )
{
    KLocker lock( m_csSendQueue );

    if( m_kSendQueue.empty() )
    {
        return false;
    }

    spPacket = m_kSendQueue.front();
    m_kSendQueue.pop();

    return true;
}

void KNexonBillingAuthManager::MakeEventFromReceivedPacket()
{
    DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KNexonBillingAuthPacket kPacket;
	if( !kPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer ) )
    {
        START_LOG( cerr, L"버퍼에서 읽기 실패." )
            << END_LOG;

        DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
        return;
    }

    if( kPacket.GetPacketType() != KNexonBillingAuthPacket::NBA_PT_COMMON )
    {
        START_LOG( cerr, L"패킷 타입 이상." )
            << BUILD_LOG( kPacket.GetPacketType() )
            << END_LOG;

        DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
        return;
    }

	KENX_COMMON_ACK_NOT kPacketAck;
	kPacket.Read( kPacketAck );

    KEventPtr spEvent( new KEvent );
    spEvent->SetData( PI_LOGIN_SERVER, NULL, ENX_COMMON_ACK_NOT, kPacketAck );
    KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

void KNexonBillingAuthManager::DumpBuffer( BYTE* buffer, bool bError )
{
    JIF( buffer );
    JIF( sizeof( buffer ) >= 4 );

    unsigned short usLength;
    ::memcpy( &usLength, buffer + 1, sizeof( usLength ) );

    usLength = ::ntohs( usLength );
    int iTotalPacketSize = usLength + 3;

    LIF( iTotalPacketSize <= MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH );
    iTotalPacketSize =  std::min< int >( iTotalPacketSize, MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH );

    char szBuffer[MAX_PACKET_SIZE_OF_NEXON_BILLING_AUTH * 3 + 1];
    char szByte[4];

    for( int i = 0; i < iTotalPacketSize; i++ )
    {
		if( ( int )buffer[i] >= 16 )
		{
#ifdef _CONVERT_VS_2010
			_itoa_s( ( int )buffer[i], szByte, 4, 16 );
#else
			::itoa( ( int )buffer[i], szByte, 16 );
#endif _CONVERT_VS_2010
			szBuffer[i * 3] = szByte[0];
			szBuffer[i * 3 + 1] = szByte[1];
			szBuffer[i * 3 + 2] = ' ';
		}
		else
        {
#ifdef _CONVERT_VS_2010
			_itoa_s( ( int )buffer[i], szByte, 4, 16 );
#else
			::itoa( ( int )buffer[i], szByte, 16 );
#endif _CONVERT_VS_2010
            szBuffer[i * 3] = '0';
            szBuffer[i * 3 + 1] = szByte[0];
            szBuffer[i * 3 + 2] = ' ';
        }
    }
    szBuffer[iTotalPacketSize * 3] = '\0';

    std::string strBuffer = szBuffer;

    if( bError )
    {
        START_LOG( cerr2, L"BUFFER : ")
            << BUILD_LOG( strBuffer );
    }
    else
    {
        START_LOG( clog2, L"BUFFER : ")
            << BUILD_LOG( strBuffer );
    }
}