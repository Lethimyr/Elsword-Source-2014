#include ".\ktdnudp.h"

#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_CHECKSUM
#include <ws2tcpip.h> //newer functions and structs used to retrieve IP addresses
#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_CHECKSUM

#ifdef  SERV_OPTIMIZE_UDP_DISABLE_SIO_CONNRESET
#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
#endif  SIO_UDP_CONNRESET
#endif  SERV_OPTIMIZE_UDP_DISABLE_SIO_CONNRESET

#define ZLIB_WINAPI
#include <Serializer/zlib/zlib.h> 
#include <malloc.h>
#include <dbg/dbg.hpp>
//{{ 2013. 2. 7	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
#include "GameSysVal/GameSysVal.h"
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

inline void ConvertCharToWCHAR( wstring& dest, const char* pSrc )
{
	dest = KncUtil::toWideString( pSrc, strlen(pSrc) );
	//int iSize = strlen( pSrc );
	//WCHAR* wcharBuf = (WCHAR*) alloca( (iSize + 1) * sizeof( WCHAR ) );
	//ZeroMemory( wcharBuf, (iSize + 1) * sizeof( WCHAR ) );

	//MultiByteToWideChar( CP_ACP, 0, pSrc, -1, wcharBuf, iSize );
	//dest = wcharBuf;
}
inline void ConvertWCHARToChar( string& dest, const WCHAR* pSrc )
{
	dest = KncUtil::toNarrowString( pSrc, wcslen(pSrc) );
	//int iSize = wcslen( pSrc );
	//char* charBuf = (char*) alloca( iSize + 1 );
	//ZeroMemory( charBuf, iSize + 1 );

	//WideCharToMultiByte( CP_ACP, 0, pSrc, -1, charBuf, iSize, NULL, NULL );
	//dest = charBuf;
}

CKTDNUDP::CKTDNUDP( int port )
{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_MyIPAddress = INADDR_ANY;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	m_MyPort	= port;
	m_MyExtPort	= m_MyPort;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_RelayPort	= 0;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	char strTemp[256];
	::gethostname( strTemp, 256 );

	HOSTENT* hostent;
	hostent		= ::gethostbyname( strTemp );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_MyIPAddress = ( ( struct in_addr* )*hostent->h_addr_list )->S_un.S_addr;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	string IP	= ::inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );
//	ConvertCharToWCHAR( m_MyIP, IP.c_str() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	m_Socket		= socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	int sndBuf		= 65534;
	int rcvBuf		= 65534;
	setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf) );
	setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, sizeof(rcvBuf) );

#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_CHECKSUM
    {
        DWORD   dwUDPChecksum = 1;
        setsockopt( m_Socket, IPPROTO_UDP, UDP_CHECKSUM_COVERAGE, (char*)&dwUDPChecksum, sizeof(dwUDPChecksum) );
    }//if
#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_CHECKSUM
#ifdef  SERV_OPTIMIZE_UDP_DISABLE_SIO_CONNRESET
    {
        DWORD   dwBytesReturned = 0;
        BOOL    bNewBehavior = FALSE;
        DWORD   dwStatus;

        dwStatus = ::WSAIoctl( m_Socket, SIO_UDP_CONNRESET
            , &bNewBehavior, sizeof(bNewBehavior)
            , NULL, 0, &dwBytesReturned
            , NULL, NULL );

        if ( dwStatus == SOCKET_ERROR )
        {
            DWORD dwError = WSAGetLastError();
            //ASSERT( dwError == WSAEWOULDBLOCK );
        }//if
    }
#endif  SERV_OPTIMIZE_UDP_DISABLE_SIO_CONNRESET

	m_LocalAddr.sin_family		= AF_INET;
	m_LocalAddr.sin_port		= htons( m_MyPort );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_LocalAddr.sin_addr.s_addr	= m_MyIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_LocalAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	int ret		= bind( m_Socket, (SOCKADDR*)&m_LocalAddr, sizeof(m_LocalAddr) );

	while( ret != 0 )
	{
		m_MyPort++;
		m_MyExtPort					= m_MyPort;
		m_LocalAddr.sin_family		= AF_INET;
		m_LocalAddr.sin_port		= htons( m_MyPort );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        m_LocalAddr.sin_addr.s_addr	= m_MyIPAddress;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		m_LocalAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		ret							= bind( m_Socket, (SOCKADDR*)&m_LocalAddr, sizeof(m_LocalAddr) );
	}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_RecvEvent = WSACreateEvent();
//	::WSAEventSelect( m_Socket, m_RecvEvent, FD_READ | FD_CLOSE );
//
//	ZeroMemory( m_pRecvBuffer, RECV_BUFFER_SIZE );
//	m_RecvDataList.reserve( 1000 );
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	//m_fLocalWaitTime		= 0.0f;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_fConnectTestTime		= 0.0f;
//
//	m_bConnectRelay			= false;
//	m_fConnectRelayTime		= 0.0f;
//
//	m_MyUID					= 0;
//	m_fDefencePortTime		= 30.0f;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



	//{{ 2009. 12. 14  최육사	RELAY서버예외처리
	//m_iPacketLoss			= 0;
	//}}
	//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
	m_iUDPSendTraffic		= 0;
	m_iUDPRecvTraffic		= 0;
	//}}
	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_DesAddr.sin_family		= AF_INET;
//	m_DesAddr.sin_port			= htons( IUS_PORT );
//	m_DesAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
//
//	// IP 체크용 정보 저장
//	m_ulLocalIP1 = inet_addr( IP.c_str() );
//	m_ulLocalIP2 = inet_addr( "127.0.0.1" );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	m_kTimer.restart();
	m_kCheckTimer.restart();
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	m_iMaxNumOverlappedRecvData = 512;
	m_fElapsedTime_FrameMove = 0.0f;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

CKTDNUDP::~CKTDNUDP(void)
{
	if( m_Socket != INVALID_SOCKET )
	{
		shutdown( m_Socket, SD_BOTH );
		closesocket( m_Socket );
		m_Socket = INVALID_SOCKET;
	}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_listSendOverlappedInOp.clear();
    m_listRecvOverlappedInOp.clear();
    m_listOverlappedFree.clear();

	m_RecvDataList.clear();
	m_RecvFreeDataList.clear();

	m_PeerMap.clear();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	WSACloseEvent( m_RecvEvent );
//
//	for( int i = 0; i < (int)m_RecvDataList.size(); i++ )
//	{
//		RecvData* pRecvData = m_RecvDataList[i];
//		SAFE_DELETE( pRecvData );
//	}
//
//	m_PeerMap.clear();
//	m_RecvDataList.clear();
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
void CKTDNUDP::OnFrameMove( void )
#else
void CKTDNUDP::OnFrameMove( double fTime, float fElapsedTime )
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
{
	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
	m_fElapsedTime_FrameMove = static_cast<float>( m_kTimer.elapsed() );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	float fElapsedTime = static_cast<float>( m_kTimer.elapsed() );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
	m_kTimer.restart();

	if( 0.1f < m_kCheckTimer.elapsed() )
	{
		m_kCheckTimer.restart();
		LanBugCheckProcess();
	}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	KTDXPROFILE();

	//}}

	//패킷 리시브
	KTDXPROFILE_BEGIN( "RECEIVE" );
	Recv();
	KTDXPROFILE_END();

//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	KTDXPROFILE();
//
//
//	//}}
//
//	WSANETWORKEVENTS	netEvent;
//	bool				bRetVal = false;
//
//	//KTDXPROFILE_BEGIN( "RECEIVE_WAIT" );
//	//for( int i = 0; i < (int)m_RecvDataWaitList.size(); i++ )
//	//{
//	//	RecvData* pRecvData = m_RecvDataWaitList[i];
//	//	pRecvData->m_fLocalWaitTime -= fElapsedTime;
//	//	if( pRecvData->m_fLocalWaitTime < 0.0f )
//	//	{
//	//		m_RecvDataList.push_back( pRecvData );
//	//		m_RecvDataWaitList.erase( m_RecvDataWaitList.begin() + i );
//	//		i--;
//	//	}
//	//}
//	//KTDXPROFILE_END();
//
//	if( m_fDefencePortTime < 0.0f )
//	{
//		DefencePort();
//		m_fDefencePortTime = 30.0f;
//	}
//	else
//	{
//		m_fDefencePortTime -= fElapsedTime;
//	}
//
//	//패킷 리시브
//	KTDXPROFILE_BEGIN( "RECEIVE" );
//	while( true )
//	{
//		KTDXPROFILE_BEGIN( "RECEIVE_EVENT" );
//		//Event select
//		::ZeroMemory( &netEvent, sizeof(netEvent) );
//
//		::WSAEnumNetworkEvents( m_Socket, m_RecvEvent, &netEvent );
//
//		KTDXPROFILE_END();
//
//		if( (netEvent.lNetworkEvents & FD_READ) == FD_READ )
//			Recv();
//		else
//			break;
//	}
//	KTDXPROFILE_END();
//
//    ProcessRecvData();
//
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

bool CKTDNUDP::Recv()
{
	KTDXPROFILE();


//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	while( (int) m_listRecvOverlappedInOp.size() < m_iMaxNumOverlappedRecvData )
	{
		list<OverlappedDataInOp>::iterator iter = _GetEmptyRecvOverlappedData();
		OverlappedDataInOp& recvInOp = *iter;
		DWORD   dwFlags = 0;
		int iRet = ::WSARecvFrom( m_Socket
			, &recvInOp.m_wsaBuf
			, 1
			, NULL
			, &dwFlags
			, (struct sockaddr*) &recvInOp.m_sin
			, &recvInOp.m_iSinSize
			, &recvInOp.m_overlapped
			, &CKTDNUDP::_CompletionRoutine_Recv );

		DWORD dwError = ::WSAGetLastError();

		if( iRet == 0 )//이미 completion routine이 스케줄 되었다.
		{
			continue;
		}
		else if( iRet == SOCKET_ERROR && ::WSAGetLastError() == WSA_IO_PENDING )
		{
			continue;
		}
		_ReturnRecvOverlappedData( recvInOp );
		return false;
	}

	return true;
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//    SOCKADDR_IN SenderAddr;
//
//	int SenderAddrSize	= sizeof(SenderAddr);
//	int recvSize		= recvfrom( m_Socket, m_pRecvBuffer, RECV_BUFFER_SIZE, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize );
//
//	if( recvSize == SOCKET_ERROR )
//	{
//		return false;
//	}
//    else if( recvSize == 0 )
//    {
//        // 소켓이 끊겼다? UDP인데 이럴 수 있나?
//        START_LOG( cerr, L"UDP 수신 패킷 사이즈 0" )
//            << END_LOG;
//
//        return false;
//    }
//    else if( recvSize < 0 || recvSize > RECV_BUFFER_SIZE )
//    {
//        // 음수는 SOCKET_ERROR 를 제외하고는 이상한 경우이다. 사이즈가 너무 커도 이상한 경우이다.
//        START_LOG( cerr, L"UDP 수신 패킷 사이즈가 음수." )
//            << BUILD_LOG( recvSize )
//            << END_LOG;
//
//        return false;
//    }
//	else
//	{
//		//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
//		m_iUDPRecvTraffic += static_cast<__int64>(recvSize);
//		//}}
//
//		switch( m_pRecvBuffer[0] )
//		{
//		case SP_RELAY:
//			{
//				Relay( recvSize );
//			}
//			break;
//
//			//{{ 2013. 2. 15	박세훈	랜선렉 방지 코드2
//#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
//		case SP_CONNECT_CHECK_ACK:
//			{
//				SSP_CONNECT_CHECK_ACK* pSSP_CONNECT_CHECK_ACK = reinterpret_cast<SSP_CONNECT_CHECK_ACK*>( m_pRecvBuffer + 1 );
//
//				std::map< __int64, LB_INFO_STRUCT_VERIFY >::iterator it = m_mapVerifyInfo.find( pSSP_CONNECT_CHECK_ACK->m_iUnitUID );
//				if( ( it != m_mapVerifyInfo.end() ) &&
//					( it->second.m_bState == true ) &&
//					( it->second.m_iHeartBeatUID == pSSP_CONNECT_CHECK_ACK->m_iHeartBeatUID )
//					)
//				{
//					if( it->second.m_kTimer.elapsed() < SiKGameSysVal()->GetLanBugOutVerifyTime() )
//					{
//						// 제시간에 도착함. 다음 검증 패킷 날릴 준비를 하자
//						it->second.m_bState			= false;
//						it->second.m_fVerifyTerm	= SiKGameSysVal()->GetLanBugOutVerifyTerm();
//						it->second.m_kTimer.restart();
//
//						START_LOG( clog2, L"랜선렉 로그: 신고자 검증 체크 패킷 도착" )
//							<< BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iUnitUID )
//							<< BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iHeartBeatUID );
//					}
//					else
//					{
//						// 제시간에 도착하지 못함. 악용 유저로 판단.
//						// Tick에서 처리 하도록 하자.
//						START_LOG( clog2, L"랜선렉 로그: 신고자 검증 체크 패킷 도착( TimeOut )" )
//							<< BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iUnitUID )
//							<< BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iHeartBeatUID );
//					}
//				}
//			}
//			break;
//
//		case SP_HEART_BEAT_ACK:
//			{
//				SSP_HEART_BEAT_ACK* pSSP_HEART_BEAT_ACK = reinterpret_cast<SSP_HEART_BEAT_ACK*>( m_pRecvBuffer + 1 );
//
//				std::map< __int64, LB_INFO_STRUCT_CHECKER >::iterator it = m_mapCheckInfo.find( pSSP_HEART_BEAT_ACK->m_iUnitUID );
//				if( ( it != m_mapCheckInfo.end() ) &&
//					( it->second.m_bState == true ) &&
//					( it->second.m_iHeartBeatUID == pSSP_HEART_BEAT_ACK->m_iHeartBeatUID )
//					)
//				{
//					if( it->second.m_kTimer.elapsed() < SiKGameSysVal()->GetLanBugOutCheckTime() )
//					{
//						// 제시간에 도착함. 다음 검증 패킷 날릴 준비를 하자
//						it->second.m_bState		= false;
//						it->second.m_fCheckTerm	= SiKGameSysVal()->GetLanBugOutCheckTerm();
//						it->second.m_kTimer.restart();
//
//						START_LOG( clog, L"랜선렉 로그: 신고자 감시 체크 패킷 도착" )
//							<< BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iUnitUID )
//							<< BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iHeartBeatUID );
//					}
//					else
//					{
//						// 제시간에 도착하지 못함. 악용 유저로 판단.
//						// Tick에서 처리 하도록 하자.
//						START_LOG( clog, L"랜선렉 로그: 신고자 감시 체크 패킷 도착( TimeOut )" )
//							<< BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iUnitUID )
//							<< BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iHeartBeatUID );
//					}
//				}
//			}
//			break;
//#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//			//}}
//
//		default:
//			{
//				unsigned long uncompressSize = RECV_BUFFER_SIZE;
//				char uncompressBuffer[RECV_BUFFER_SIZE] = {0,};
//				if( uncompress( (BYTE*)uncompressBuffer, &uncompressSize, (BYTE*)(&m_pRecvBuffer[1]), recvSize-1 ) != Z_OK )
//					return false;
//
//				RecvData* pRecvData		= new RecvData;
//				string addr				= inet_ntoa( SenderAddr.sin_addr );
//				ConvertCharToWCHAR( pRecvData->m_SenderIP, addr.c_str() );
//				pRecvData->m_SenderPort	= ntohs( SenderAddr.sin_port );
//				pRecvData->m_ID			= m_pRecvBuffer[0];
//				pRecvData->m_Size		= uncompressSize;
//				pRecvData->SetData( uncompressBuffer, uncompressSize );
//				m_RecvDataList.push_back( pRecvData );
//			}
//			break;
//		}
//		return true;
//	}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
bool CKTDNUDP::SendToIP( u_long  ulIP, int port, const char ID, const void* pBuffer, int size )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//bool CKTDNUDP::Send( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
	KTDXPROFILE();

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    if ( ulIP == INADDR_ANY )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//    if( !pIP )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    {
        return false;
    }

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
    if( size < 0 || size > 0 && pBuffer == NULL )
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//    if( !pBuffer )
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
    {
        return false;
    }

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    WSABUF  alpBuffers[2];
    alpBuffers[0].buf = (CHAR*) &ID;
    alpBuffers[0].len = 1;
    alpBuffers[1].buf = (CHAR*) pBuffer;
    alpBuffers[1].len = size;
    return SendPure_Scattered( ulIP, port, alpBuffers, (size>0) ? 2 : 1 );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	unsigned long	compressSize			= 2000;
//	char			pCompressBuffer[2000]	= {0,};
//	if( !Compress( ID, pCompressBuffer, compressSize, pBuffer, size ) )
//    {
//        return false;
//    }
//	return SendPure( pIP, port, ID, pCompressBuffer, compressSize );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

bool CKTDNUDP::SendPure_Scattered( u_long ulIP, int port, WSABUF alpBuffers[], DWORD dwBuffCount )
{
    if( ulIP == INADDR_ANY )
    {
        return false;
    }
    if( dwBuffCount == 0 || alpBuffers == NULL )
    {
        return false;
    }

	//목적지 설정
	SOCKADDR_IN	destination;
	destination.sin_family		= AF_INET;
	destination.sin_addr.s_addr	= ulIP;
	destination.sin_port		= htons( port );  //포트번호

    DWORD   dwSize = 0;
    for( DWORD i = 0; i < dwBuffCount; i++ )
    {
        dwSize += alpBuffers[i].len;
    }
    if ( dwSize == 0 || dwSize > RECV_BUFFER_SIZE )
    {
        return false;
    }

	list<OverlappedDataInOp>::iterator iter = _GetEmptySendOverlappedData();
	OverlappedDataInOp& sendInOp = *iter;
    sendInOp.m_wsaBuf.len = dwSize;
	sendInOp.m_iSendCounter = 1;
    DWORD   dwOffset = 0;
    for ( DWORD i = 0; i < dwBuffCount; i++ )
    {
	    memcpy( &sendInOp.m_wsaBuf.buf[dwOffset], alpBuffers[i].buf, alpBuffers[i].len );
        dwOffset += alpBuffers[i].len;
    }
	int iRet = ::WSASendTo( m_Socket 
		, &sendInOp.m_wsaBuf
		, 1
		, NULL
		, 0
		, (const struct sockaddr*) &destination
		, sizeof(destination)
		, &sendInOp.m_overlapped
		, &CKTDNUDP::_CompletionRoutine_Send );

	if( iRet == 0 )//이미 completion routine이 스케줄 되었다.
	{
		return true;
	}
	else if( iRet == SOCKET_ERROR && ::WSAGetLastError() == WSA_IO_PENDING )
	{
		return true;
	}

	sendInOp.m_iSendCounter--;
	if( sendInOp.m_iSendCounter == 0 )
		_ReturnSendOverlappedData( sendInOp );
	return false;

}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//bool CKTDNUDP::SendPure( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size )
//{
//    if( !pIP )
//    {
//        return false;
//    }
//
//    if( !pBuffer )
//    {
//        return false;
//    }
//
//	string mbIP;
//	ConvertWCHARToChar( mbIP, pIP );
//	inet_addr( mbIP.c_str() );
//
//	//목적지 설정
//	SOCKADDR_IN	destination;
//	destination.sin_family		= AF_INET;
//	destination.sin_addr.s_addr	= inet_addr( mbIP.c_str() );
//	destination.sin_port		= htons( port );  //포트번호
//
//	int ret = sendto( m_Socket, pBuffer, size, 0, (SOCKADDR *)&destination, sizeof(SOCKADDR_IN) );
//
//	if( ret == SOCKET_ERROR )
//	{
//		return false;
//	}
//	else
//	{
//		//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
//		m_iUDPSendTraffic += static_cast<__int64>(size);
//		//}}
//
//		return true;
//	}
//
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//bool CKTDNUDP::SendRelay( const char ID, const char* pBuffer, int size, vector<__int64>& UIDList )
//{
//	KTDXPROFILE();
//
//    if( !pBuffer )
//    {
//        return false;
//    }
//
//	unsigned long	compressSize			= 2000;
//	char			pCompressBuffer[2000]	= {0,};
//	if( !Compress( ID, pCompressBuffer, compressSize, pBuffer, size ) )
//    {
//        return false;
//    }
//
//	char	pFinalBuffer[3000]	= {0,};
//	int		finalBufSize = 0;
//	AddRelayHeader( ID, pFinalBuffer, finalBufSize, pCompressBuffer + 1, compressSize - 1, UIDList );
//
//	return SendPure( m_RelayIP.c_str(), m_RelayPort, ID, pFinalBuffer, finalBufSize );
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
bool CKTDNUDP::SendToPeer( __int64 UID, const char ID, const void* pBuffer, int size )
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//bool CKTDNUDP::Send( __int64 UID, const char ID, const char* pBuffer, int size )
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		
{
	KTDXPROFILE();

    if( !pBuffer )
    {
        return false;
    }

	const Peer* pPeer = GetPeer( UID );
	if( pPeer == NULL )
		return false;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	if( pPeer->m_bUseRelay == true )
//	{
//		vector<__int64> UIDList;
//		UIDList.push_back( pPeer->m_UID );
//		return SendRelay( ID, pBuffer, size, UIDList );
//	}
//	else
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		return SendToIP( pPeer->m_IPAddress, pPeer->m_Port, ID, pBuffer, size );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		return Send( pPeer->m_IP.c_str(), pPeer->m_Port, ID, pBuffer, size );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}

	return false;
}

CKTDNUDP::Peer* CKTDNUDP::_AddPeer( __int64 UID, 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    u_long  ulIP,
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//    const WCHAR* pIP, 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    int port )
{
	Peer peer;
	peer.m_UID	= UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    peer.m_IPAddress	= ulIP;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	peer.m_IP	= pIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	peer.m_Port	= port;

    {
        map<__int64,Peer>::iterator iter;
	    iter = m_PeerMap.find( UID );
        if ( iter != m_PeerMap.end() )
            m_PeerMap.erase( iter );
    }

	return &m_PeerMap.insert( std::make_pair(peer.m_UID,peer) ).first->second;
}

bool CKTDNUDP::RemovePeer( __int64 UID )
{
	map<__int64,Peer>::iterator iter;
	iter = m_PeerMap.find( UID );
	if( iter == m_PeerMap.end() )
		return false;

	m_PeerMap.erase( iter );
	return true;
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
CKTDNUDP::Peer* CKTDNUDP::_GetPeer( __int64 UID )
{
	map<__int64,Peer>::iterator iter;
	iter = m_PeerMap.find( UID );
	if( iter == m_PeerMap.end() )
		return NULL;

	Peer* pPeer = &iter->second;
	pPeer->m_SleepTime = 0.0f;

	return pPeer;
}
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		

const CKTDNUDP::Peer* CKTDNUDP::GetPeer( __int64 UID )
{
	map<__int64,Peer>::iterator iter;
	iter = m_PeerMap.find( UID );
	if( iter == m_PeerMap.end() )
		return NULL;

	Peer* pPeer = &iter->second;
	pPeer->m_SleepTime = 0.0f;

	return pPeer;
}


//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//CKTDNUDP::Peer* CKTDNUDP::GetMyPeer()
//{
//	return GetPeer( m_MyUID );
//}
//
//void CKTDNUDP::DefencePort()
//{
//	Peer* pPeer = GetMyPeer();
//	if( pPeer != NULL )
//	{
//		SSP_DEFENCE_PORT sSP_DEFENCE_PORT;
//		Send( pPeer->m_IP.c_str(), GetMyExtPort(), SP_DEFENCE_PORT, (const char*)&sSP_DEFENCE_PORT, sizeof(SSP_DEFENCE_PORT) );
//		Send( m_RelayIP.c_str(), m_RelayPort, SP_DEFENCE_PORT, (const char*)&sSP_DEFENCE_PORT, sizeof(SSP_DEFENCE_PORT) );
//	}
//}
//
//void CKTDNUDP::ConnectTestToPeer()
//{
//	m_fConnectTestTime = 0.5f;
//	map<__int64,Peer>::iterator iter;
//	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
//	{
//		Peer* pPeer = &iter->second;
//		if( pPeer->m_bConnected == false )
//		{
//			pPeer->m_bUseRelay		= true;
//			pPeer->m_bConnected	= true;
//			pPeer->m_ConnectTestCount++;
//			if( pPeer->m_ConnectTestCount >= 5 )
//			{
//				pPeer->m_bUseRelay		= true;
//				pPeer->m_bConnected	= true;
//			}
//			else
//			{
//				Peer* pMyPeer = GetMyPeer();
//				if( pMyPeer != NULL )
//				{
//					SSP_CONNECT_TEST_REQ sSP_CONNECT_TEST_REQ;
//					sSP_CONNECT_TEST_REQ.m_UID			= pMyPeer->m_UID;
//					sSP_CONNECT_TEST_REQ.m_SendTime		= timeGetTime();
//					if( pMyPeer == pPeer )
//						Send( m_MyIP.c_str(), m_MyPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
//					else
//						Send( pPeer->m_IP.c_str(), pPeer->m_Port, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
//				}
//			}
//		}
//	}
//}
//
//bool CKTDNUDP::ConnectTestResult()
//{
//	if( m_PeerMap.size() < 1 )
//		return false;
//
//	map<__int64,Peer>::iterator iter;
//	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
//	{
//		Peer* pPeer = &iter->second;
//		if( pPeer->m_bConnected == false )
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
//void CKTDNUDP::ConnectTestToRelay()
//{
//	m_fConnectRelayTime = 0.5f;
//
//	SSP_CONNECT_RELAY_REQ sSP_CONNECT_RELAY_REQ;
//	sSP_CONNECT_RELAY_REQ.m_UID = m_MyUID;
//	Send( m_RelayIP.c_str(), m_RelayPort, SP_CONNECT_RELAY_REQ, (const char*)&sSP_CONNECT_RELAY_REQ, sizeof(SSP_CONNECT_RELAY_REQ) );
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
//#ifdef SERV_UDP_RELAY_CHECKER	

void CKTDNUDP::AddPeerForRelayTest( __int64 UID, 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    u_long      ulIP,
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//    const WCHAR* pIP, 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    int port )
{
	Peer peer;
	peer.m_UID	= UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    peer.m_IPAddress	= ulIP;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	peer.m_IP	= pIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	peer.m_Port	= port;
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	peer.m_bUseRelay = true; // UDP Relay Server테스트를 위해 무조건 relay사용을 체크하자!
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

    {
	    const Peer* pPeer = GetPeer( UID );
	    if( pPeer != NULL )
		    RemovePeer( UID );
    }

	m_PeerMap.insert( std::make_pair(peer.m_UID,peer) );
}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
bool CKTDNUDP::RecvForRelayTest()
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//bool CKTDNUDP::RecvForRelayTest( RecvData& kRecvData )
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
	KTDXPROFILE();

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	while( (int) m_listRecvOverlappedInOp.size() < m_iMaxNumOverlappedRecvData )
	{
		list<OverlappedDataInOp>::iterator iter = _GetEmptyRecvOverlappedData();
		OverlappedDataInOp& recvInOp = *iter;
		DWORD   dwFlags = 0;
		int iRet = ::WSARecvFrom( m_Socket
			, &recvInOp.m_wsaBuf
			, 1
			, NULL
			, &dwFlags
			, (struct sockaddr*) &recvInOp.m_sin
			, &recvInOp.m_iSinSize
			, &recvInOp.m_overlapped
			, &CKTDNUDP::_CompletionRoutine_RecvOverlappedForRelayTest );

		if( iRet == 0 )//이미 completion routine이 스케줄 되었다.
		{
			continue;
		}
		else if( iRet == SOCKET_ERROR && ::WSAGetLastError() == WSA_IO_PENDING )
		{
			continue;
		}

		_ReturnRecvOverlappedData( recvInOp );
		return false;
	}
	
	return true;
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	SOCKADDR_IN SenderAddr;
//
//	int SenderAddrSize	= sizeof(SenderAddr);
//	int recvSize		= recvfrom( m_Socket, m_pRecvBuffer, RECV_BUFFER_SIZE, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize );
//
//	if( recvSize == SOCKET_ERROR )
//	{
//		return false;
//	}
//	else if( recvSize == 0 )
//	{
//		// 소켓이 끊겼다? UDP인데 이럴 수 있나?
//		START_LOG( cerr, L"UDP 수신 패킷 사이즈 0" )
//			<< END_LOG;
//
//		return false;
//	}
//	else if( recvSize < 0 || recvSize > RECV_BUFFER_SIZE )
//	{
//		// 음수는 SOCKET_ERROR 를 제외하고는 이상한 경우이다. 사이즈가 너무 커도 이상한 경우이다.
//		START_LOG( cerr, L"UDP 수신 패킷 사이즈가 음수." )
//			<< BUILD_LOG( recvSize )
//			<< END_LOG;
//
//		return false;
//	}
//	else
//	{
//		unsigned long uncompressSize = 2000;
//		char uncompressBuffer[2000] = {0,};
//		if( uncompress( (BYTE*)uncompressBuffer, &uncompressSize, (BYTE*)(&m_pRecvBuffer[1]), recvSize-1 ) != Z_OK )
//			return false;
//
//		string addr				= inet_ntoa( SenderAddr.sin_addr );
//		ConvertCharToWCHAR( kRecvData.m_SenderIP, addr.c_str() );
//		kRecvData.m_SenderPort	= ntohs( SenderAddr.sin_port );
//		kRecvData.m_ID			= m_pRecvBuffer[0];
//		kRecvData.m_Size		= uncompressSize;
//		kRecvData.SetData( uncompressBuffer, uncompressSize );
//		return true;
//	}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}
//#endif SERV_UDP_RELAY_CHECKER
//}}
//{{ 2013. 2. 5	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
void    CKTDNUDP::ThreadSafeSwapLBInfo( std::vector<CKTDNUDP::LB_INFO_STRUCT>& vecInOut )
{
    KLocker lock( m_csLBInfo );
    m_vecLBInfo.swap( vecInOut );
}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void CKTDNUDP::LanBugCheckProcess( void )
{
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	char pSendBuffer[RECV_BUFFER_SIZE];
//	const size_t iNumOnce		= RECV_BUFFER_SIZE / sizeof( CKTDNUDP::LB_INFO_STRUCT );
//	size_t iNumStorage	= 0;
//	LB_INFO_STRUCT* pSend = reinterpret_cast< LB_INFO_STRUCT* >( pSendBuffer );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	// 랜선렉 감시
	if( SiKGameSysVal()->GetLanBugOutCheck() == true )
	{
		std::map< __int64, LB_INFO_STRUCT_CHECKER >::iterator it = m_mapCheckInfo.begin();
		const float fCheckTime			= SiKGameSysVal()->GetLanBugOutCheckTime();
		const byte	byteCheckMaxCount	= SiKGameSysVal()->GetLanBugOutCheckMaxCount();
		const byte	byteCheckCount		= SiKGameSysVal()->GetLanBugOutCheckCount();
		const float	fCheckRepeatTerm	= SiKGameSysVal()->GetLanBugOutCheckRepeatTerm();
		while( ( it != m_mapCheckInfo.end() ) 
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//            && ( iNumStorage < iNumOnce ) 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            )
		{
			if( it->second.m_bState == true )
			{
                DWORD   dwCurTickCount = ::GetTickCount() ;
                LONG    lTimerStampDiff = (LONG) ( dwCurTickCount - it->second.m_dwTimerStamp );
                float   fTimeDiff = lTimerStampDiff * ( 1.f/1000.f );
				//if( it->second.m_kTimer.elapsed() < fCheckTime )
                if ( fTimeDiff < fCheckTime )
				{
                    LONG    lRepeatTimerStampDiff = (LONG) ( dwCurTickCount - it->second.m_dwRepeatTimerStamp );
                    float   fRepeatTimeDiff = lRepeatTimerStampDiff * ( 1.f/1000.f );
					if( ( 0.0f < fCheckRepeatTerm ) &&
						//( fCheckRepeatTerm <= it->second.m_kRepeatTimer.elapsed() )
                        ( fCheckRepeatTerm <= fRepeatTimeDiff )
						)
					{
						SSP_HEART_BEAT_REQ sSSP_HEART_BEAT_REQ;
						sSSP_HEART_BEAT_REQ.m_iHeartBeatUID	= it->second.m_iHeartBeatUID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
						SendToPeer( it->first, SP_HEART_BEAT_REQ, reinterpret_cast<char*>( &sSSP_HEART_BEAT_REQ ), sizeof( SSP_HEART_BEAT_REQ ) );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//						Send( it->first, SP_HEART_BEAT_REQ, reinterpret_cast<char*>( &sSSP_HEART_BEAT_REQ ), sizeof( SSP_HEART_BEAT_REQ ) );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK								
						//it->second.m_kRepeatTimer.restart();
                        it->second.m_dwRepeatTimerStamp = ::GetTickCount() ;
					}

					++it;
					continue;
				}

				if( ++it->second.m_byteCount < byteCheckCount )
				{
					START_LOG( clog, L"감시자 패킷 지연 판정" )
						<< BUILD_LOG( it->first )
						<< BUILD_LOG( it->second.m_byteCount )
						<< BUILD_LOG( it->second.m_byteMaxCount );

					// 다음 검증 패킷 날릴 준비를 하자
					it->second.m_bState		= false;
					it->second.m_fCheckTerm	= 0.0f;

					++it;
					continue;
				}

				START_LOG( clog, L"랜선렉 유저로 판정!!" )
					<< BUILD_LOG( it->first )
					<< BUILD_LOG( it->second.m_byteCount )
					<< BUILD_LOG( it->second.m_byteMaxCount );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_vecLocalLBInfo.resize( m_vecLocalLBInfo.size() + 1 );
                LB_INFO_STRUCT& kInfo   = m_vecLocalLBInfo.back();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				LB_INFO_STRUCT& kInfo	= pSend[iNumStorage++];
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				kInfo.m_eEventID		= LEI_CONFIRM_NOT;
				kInfo.m_iRoomUID		= it->second.m_iRoomUID;
				kInfo.m_iUnitUID		= it->first;
				it = m_mapCheckInfo.erase( it );
			}
			else
			{
                DWORD   dwCurTimeStamp = ::GetTickCount();
                LONG    lTimerStampDiff = (LONG) ( dwCurTimeStamp - it->second.m_dwTimerStamp );
                float   fTimeDiff = lTimerStampDiff * ( 1.f/1000.f );
				//if( it->second.m_kTimer.elapsed() < it->second.m_fCheckTerm )
                if ( fTimeDiff < it->second.m_fCheckTerm )
				{
					++it;
					continue;
				}

				if( ( 0 < byteCheckMaxCount ) &&
					( byteCheckMaxCount <= ++it->second.m_byteMaxCount )
					)
				{
					// 최대 횟수만큼 테스트를 끝냈다.
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    m_vecLocalLBInfo.resize( m_vecLocalLBInfo.size() + 1 );
                    LB_INFO_STRUCT& kInfo   = m_vecLocalLBInfo.back();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					LB_INFO_STRUCT& kInfo	= pSend[iNumStorage++];
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					kInfo.m_eEventID		= LEI_CHECK_CANCLE_NOT;
					kInfo.m_iRoomUID		= it->second.m_iRoomUID;
					kInfo.m_iUnitUID		= it->first;

					START_LOG( clog, L"랜선렉 로그: 피 신고자 감시 종료." )
						<< BUILD_LOG( kInfo.m_iUnitUID )
						<< BUILD_LOG( it->second.m_byteCount )
						<< BUILD_LOG( it->second.m_byteMaxCount );

					it = m_mapCheckInfo.erase( it );
					continue;
				}

				SSP_HEART_BEAT_REQ sSSP_HEART_BEAT_REQ;

				// uid 발급
				const int iHeartBeatUID = rand();
				sSSP_HEART_BEAT_REQ.m_iHeartBeatUID	= iHeartBeatUID;
				it->second.m_iHeartBeatUID			= iHeartBeatUID;
				it->second.m_bState = true;
				//it->second.m_kTimer.restart();
                it->second.m_dwTimerStamp = dwCurTimeStamp;
				//it->second.m_kRepeatTimer.restart();
                it->second.m_dwRepeatTimerStamp = dwCurTimeStamp;

				// 클라이언트에 보냄
				START_LOG( clog, L"랜선렉 로그: 피 신고자 감시 체크 패킷 출발." )
					<< BUILD_LOG( it->first )
					<< BUILD_LOG( it->second.m_iHeartBeatUID )
					<< BUILD_LOG( it->second.m_byteCount )
					<< BUILD_LOG( it->second.m_byteMaxCount );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
				SendToPeer( it->first, SP_HEART_BEAT_REQ, reinterpret_cast<char*>( &sSSP_HEART_BEAT_REQ ), sizeof( SSP_HEART_BEAT_REQ ) );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				Send( it->first, SP_HEART_BEAT_REQ, reinterpret_cast<char*>( &sSSP_HEART_BEAT_REQ ), sizeof( SSP_HEART_BEAT_REQ ) );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK							
			}
		}
	}

	// 신고자 검증
	if( SiKGameSysVal()->GetLanBugOutVerify() == true )
	{
		std::map< __int64, LB_INFO_STRUCT_VERIFY >::iterator it = m_mapVerifyInfo.begin();
		const float fVerifyTime			= SiKGameSysVal()->GetLanBugOutVerifyTime();
		const byte	byteVerifyMaxCount	= SiKGameSysVal()->GetLanBugOutVerifyMaxCount();
		const byte	byteVerifyCount		= SiKGameSysVal()->GetLanBugOutVerifyCount();
		const float	fVerifyRepeatTerm	= SiKGameSysVal()->GetLanBugOutVerifyRepeatTerm();
		while( ( it != m_mapVerifyInfo.end() ) 
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//            && ( iNumStorage < iNumOnce ) 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            )
		{
			if( it->second.m_bState == true )
			{
                DWORD   dwCurTickCount = ::GetTickCount();
                LONG    lTimerStampDiff = (LONG) ( dwCurTickCount - it->second.m_dwTimerStamp );
                float   fTimeDiff = lTimerStampDiff * ( 1.f/1000.f );
				//if( it->second.m_kTimer.elapsed() < fVerifyTime )
                if ( fTimeDiff < fVerifyTime )
				{
                    LONG    lRepeatTimerStampDiff = (LONG) ( dwCurTickCount - it->second.m_dwRepeatTimerStamp );
                    float   fRepeatTimeDiff = lRepeatTimerStampDiff * ( 1.f/1000.f );
					if( ( 0.0f < fVerifyRepeatTerm ) &&
						//( fVerifyRepeatTerm <= it->second.m_kRepeatTimer.elapsed() )
                        ( fVerifyRepeatTerm <= fRepeatTimeDiff )
						)
					{
						SSP_CONNECT_CHECK_REQ sSSP_CONNECT_CHECK_REQ;
						sSSP_CONNECT_CHECK_REQ.m_iHeartBeatUID	= it->second.m_iHeartBeatUID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
						SendToPeer( it->first, SP_CONNECT_CHECK_REQ, reinterpret_cast<char*>( &sSSP_CONNECT_CHECK_REQ ), sizeof( SSP_CONNECT_CHECK_REQ ) );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//						Send( it->first, SP_CONNECT_CHECK_REQ, reinterpret_cast<char*>( &sSSP_CONNECT_CHECK_REQ ), sizeof( SSP_CONNECT_CHECK_REQ ) );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK								
						//it->second.m_kRepeatTimer.restart();
                        it->second.m_dwRepeatTimerStamp = ::GetTickCount();
					}

					++it;
					continue;
				}

				if( ++it->second.m_byteCount < byteVerifyCount )
				{
					START_LOG( clog2, L"검증자 패킷 지연 판정" )
						<< BUILD_LOG( it->first )
						<< BUILD_LOG( it->second.m_byteCount )
						<< BUILD_LOG( it->second.m_byteMaxCount );

					// 다음 검증 패킷 날릴 준비를 하자
					it->second.m_bState			= false;
					it->second.m_fVerifyTerm	= 0.0f;

					++it;
					continue;
				}

				START_LOG( clog, L"팅버그 유저로 판정!!" )
					<< BUILD_LOG( it->first )
					<< BUILD_LOG( it->second.m_byteCount )
					<< BUILD_LOG( it->second.m_byteMaxCount );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_vecLocalLBInfo.resize( m_vecLocalLBInfo.size() + 1 );
                LB_INFO_STRUCT& kInfo   = m_vecLocalLBInfo.back();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				LB_INFO_STRUCT& kInfo	= pSend[iNumStorage++];
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				kInfo.m_eEventID		= LEI_CONFIRM_NOT;
				kInfo.m_iRoomUID		= it->second.m_iRoomUID;
				kInfo.m_iUnitUID		= it->first;

				it = m_mapVerifyInfo.erase( it );
			}
			else
			{
                DWORD   dwCurTimeStamp = ::GetTickCount();
                LONG    lTimerStampDiff = (LONG) ( dwCurTimeStamp - it->second.m_dwTimerStamp );
                float   fTimeDiff = lTimerStampDiff * ( 1.f/1000.f );
				//if( it->second.m_kTimer.elapsed() < it->second.m_fVerifyTerm )
                if ( fTimeDiff < it->second.m_fVerifyTerm )
				{
					++it;
					continue;
				}

				if( ( 0 < byteVerifyMaxCount ) &&
					( byteVerifyMaxCount <= ++it->second.m_byteMaxCount )
					)
				{
					// 최대 횟수만큼 테스트를 끝냈다.
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                    m_vecLocalLBInfo.resize( m_vecLocalLBInfo.size() + 1 );
                    LB_INFO_STRUCT& kInfo   = m_vecLocalLBInfo.back();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				    LB_INFO_STRUCT& kInfo	= pSend[iNumStorage++];
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					kInfo.m_eEventID		= LEI_VERIFY_CANCLE_NOT;
					kInfo.m_iRoomUID		= it->second.m_iRoomUID;
					kInfo.m_iUnitUID		= it->first;

					START_LOG( clog2, L"랜선렉 로그: 신고자 검증 종료." )
						<< BUILD_LOG( kInfo.m_iUnitUID )
						<< BUILD_LOG( it->second.m_byteCount )
						<< BUILD_LOG( it->second.m_byteMaxCount );

					it = m_mapVerifyInfo.erase( it );
					continue;
				}

				SSP_CONNECT_CHECK_REQ sSSP_CONNECT_CHECK_REQ;

				// uid 발급
				const int iHeartBeatUID = rand();
				sSSP_CONNECT_CHECK_REQ.m_iHeartBeatUID	= iHeartBeatUID;
				it->second.m_iHeartBeatUID			= iHeartBeatUID;
				it->second.m_bState = true;
				//it->second.m_kTimer.restart();
                it->second.m_dwTimerStamp = dwCurTimeStamp;
				//it->second.m_kRepeatTimer.restart();
                it->second.m_dwRepeatTimerStamp = dwCurTimeStamp;

				// 클라이언트에 보냄
				START_LOG( clog2, L"랜선렉 로그: 신고자 검증 체크 패킷 출발." )
					<< BUILD_LOG( it->first )
					<< BUILD_LOG( it->second.m_iHeartBeatUID )
					<< BUILD_LOG( it->second.m_byteCount )
					<< BUILD_LOG( it->second.m_byteMaxCount );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
				SendToPeer( it->first, SP_CONNECT_CHECK_REQ, reinterpret_cast<char*>( &sSSP_CONNECT_CHECK_REQ ), sizeof( SSP_CONNECT_CHECK_REQ ) );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				Send( it->first, SP_CONNECT_CHECK_REQ, reinterpret_cast<char*>( &sSSP_CONNECT_CHECK_REQ ), sizeof( SSP_CONNECT_CHECK_REQ ) );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK							
			}
		}
	}


//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	if( iNumStorage <= 0 )
//	{
//		return;
//	}
//
//	// 압축
//	unsigned long compressSize = RECV_BUFFER_SIZE;
//	char pCompressBuffer[RECV_BUFFER_SIZE];
//	if( Compress( SP_MORNITORING_NOT, pCompressBuffer, compressSize, pSendBuffer, iNumStorage * sizeof( LB_INFO_STRUCT ) ) == false )
//	{
//		START_LOG( cwarn, L"UDP: Compress 실패" )
//			<< BUILD_LOG( iNumStorage )
//			<< END_LOG;
//		return;
//	}
//	sendto( m_Socket, pCompressBuffer, compressSize, 0, ( SOCKADDR* )&m_DesAddr, sizeof( SOCKADDR_IN ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

void CKTDNUDP::_ProcessMonitoringNot( const LB_INFO_STRUCT& kInfo )
{
	switch( kInfo.m_eEventID )
	{
	case CKTDNUDP::LEI_CHECK_NOT:
		{
			// 감시 목록에 추가
			LB_INFO_STRUCT_CHECKER sLB_INFO_STRUCT_CHECKER( kInfo.m_iRoomUID );

			std::pair< std::map< __int64, LB_INFO_STRUCT_CHECKER >::iterator, bool > pairResult;
			pairResult = m_mapCheckInfo.insert( std::map< __int64, LB_INFO_STRUCT_CHECKER >::value_type( kInfo.m_iUnitUID, sLB_INFO_STRUCT_CHECKER ) );

			if( pairResult.second == true )
			{
				// 감시 리스트에 추가 성공. 검증 패킷을 날리자!
				SSP_HEART_BEAT_REQ sSSP_HEART_BEAT_REQ;

				// uid 발급
				const int iHeartBeatUID = rand();
				sSSP_HEART_BEAT_REQ.m_iHeartBeatUID			= iHeartBeatUID;
				pairResult.first->second.m_iHeartBeatUID	= iHeartBeatUID;

				// 클라이언트에 보냄
				START_LOG( clog, L"랜선렉 로그: 피 신고자 감시 시작" )
					<< BUILD_LOG( kInfo.m_iUnitUID )
					<< BUILD_LOG( iHeartBeatUID );

				SendToPeer( kInfo.m_iUnitUID, SP_HEART_BEAT_REQ, reinterpret_cast<char*>( &sSSP_HEART_BEAT_REQ ), sizeof( SSP_HEART_BEAT_REQ ) );

                DWORD   dwCurTickCount = ::GetTickCount();
				//pairResult.first->second.m_kTimer.restart();
                pairResult.first->second.m_dwTimerStamp = dwCurTickCount;
				//pairResult.first->second.m_kRepeatTimer.restart();
                pairResult.first->second.m_dwRepeatTimerStamp = dwCurTickCount;

				return;
			}

			if( sLB_INFO_STRUCT_CHECKER.m_iRoomUID == pairResult.first->second.m_iRoomUID )
			{
				// 이미 등록되어 있는 것과 정보가 동일하다면, 덮어 씌우지 않는다.
				START_LOG( clog, L"랜선렉 로그: 이미 감시 중인 피 신고자입니다." )
					<< BUILD_LOG( kInfo.m_iUnitUID );
				return;
			}

			// 정보를 덮어 씌우고, 시간을 초기화 한다.
			START_LOG( clog, L"랜선렉 로그: 피 신고자의 정보를 갱신합니다." )
				<< BUILD_LOG( kInfo.m_iUnitUID );
			pairResult.first->second.m_iRoomUID = sLB_INFO_STRUCT_CHECKER.m_iRoomUID;
		}
		break;

	case CKTDNUDP::LEI_CHECK_CANCLE_NOT:
		{
			// 감시 목록에서 제거
			std::map< __int64, LB_INFO_STRUCT_CHECKER >::iterator it = m_mapCheckInfo.find( kInfo.m_iUnitUID );
			if( it == m_mapCheckInfo.end() )
			{
				// 대상 감시자가 존재하지 않습니다.
				START_LOG( clog, L"랜선렉 로그: 감시 상태에서 해제하려는 피 신고자가 존재하지 않습니다." )
					<< BUILD_LOG( kInfo.m_iUnitUID );
				return;
			}

			if( kInfo.m_iRoomUID != it->second.m_iRoomUID )
			{
				// Room 정보가 일치하지 않으므로 해당 Unit을 감시에서 제외시키지 않습니다.
				START_LOG( clog, L"랜선렉 로그: 피 신고자에 대한 정보가 일치하지 않아 감시 상태를 해제할 수 없습니다." )
					<< BUILD_LOG( kInfo.m_iUnitUID );
				return;
			}

			// 감시 대상 제거
			START_LOG( clog, L"랜선렉 로그: 피 신고자 감시 종료. ( 요청 )" )
				<< BUILD_LOG( kInfo.m_iUnitUID );
			m_mapCheckInfo.erase( it );
		}
		break;

	case CKTDNUDP::LEI_VERIFY_NOT:
		{
			// 신고자 검증 요청
			LB_INFO_STRUCT_VERIFY sLB_INFO_STRUCT_VERIFY( kInfo.m_iRoomUID );

			std::pair< std::map< __int64, LB_INFO_STRUCT_VERIFY >::iterator, bool > pairResult;
			pairResult = m_mapVerifyInfo.insert( std::map< __int64, LB_INFO_STRUCT_VERIFY >::value_type( kInfo.m_iUnitUID, sLB_INFO_STRUCT_VERIFY ) );

			if( pairResult.second == true )
			{
				// 감시 리스트에 추가 성공. 검증 패킷을 날리자!
				SSP_CONNECT_CHECK_REQ sSSP_CONNECT_CHECK_REQ;

				// uid 발급
				const int iHeartBeatUID = rand();
				sSSP_CONNECT_CHECK_REQ.m_iHeartBeatUID		= iHeartBeatUID;
				pairResult.first->second.m_iHeartBeatUID	= iHeartBeatUID;

				// 클라이언트에 보냄
				START_LOG( clog2, L"랜선렉 로그: 신고자 검증 시작" )
					<< BUILD_LOG( kInfo.m_iUnitUID )
					<< BUILD_LOG( iHeartBeatUID );

				SendToPeer( kInfo.m_iUnitUID, SP_CONNECT_CHECK_REQ, reinterpret_cast<char*>( &sSSP_CONNECT_CHECK_REQ ), sizeof( SSP_CONNECT_CHECK_REQ ) );

                DWORD   dwCurTickCount = ::GetTickCount();
				//pairResult.first->second.m_kTimer.restart();
                pairResult.first->second.m_dwTimerStamp = dwCurTickCount;
				//pairResult.first->second.m_kRepeatTimer.restart();
                pairResult.first->second.m_dwRepeatTimerStamp = dwCurTickCount;
				return;
			}

			if( sLB_INFO_STRUCT_VERIFY.m_iRoomUID == pairResult.first->second.m_iRoomUID )
			{
				// 이미 등록되어 있는 것과 정보가 동일하다면, 덮어 씌우지 않는다.
				START_LOG( clog2, L"랜선렉 로그: 이미 검증 중인 신고자입니다." )
					<< BUILD_LOG( kInfo.m_iUnitUID );
				return;
			}

			// 정보를 덮어 씌운다.
			START_LOG( clog2, L"랜선렉 로그: 신고자의 정보를 갱신합니다." )
				<< BUILD_LOG( kInfo.m_iUnitUID );
			pairResult.first->second.m_iRoomUID = sLB_INFO_STRUCT_VERIFY.m_iRoomUID;
		}
		break;

	case CKTDNUDP::LEI_VERIFY_CANCLE_NOT:
		{
			// 검증 목록에서 제거
			std::map< __int64, LB_INFO_STRUCT_VERIFY >::iterator it = m_mapVerifyInfo.find( kInfo.m_iUnitUID );
			if( it == m_mapVerifyInfo.end() )
			{
				// 대상 피 검증자가 존재하지 않습니다.
				START_LOG( clog2, L"랜선렉 로그: 검증 상태에서 해제하려는 신고자가 존재하지 않습니다." )
					<< BUILD_LOG( kInfo.m_iUnitUID );
				return;
			}

			if( kInfo.m_iRoomUID != it->second.m_iRoomUID )
			{
				// Room 정보가 일치하지 않으므로 해당 Unit을 감시에서 제외시키지 않습니다.
				START_LOG( clog2, L"랜선렉 로그: 신고자에 대한 정보가 일치하지 않아 검증 상태를 해제할 수 없습니다." )
					<< BUILD_LOG( kInfo.m_iUnitUID );
				return;
			}

			// 감시 대상 제거
			START_LOG( clog2, L"랜선렉 로그: 신고자 검증 종료. ( 요청 )" )
				<< BUILD_LOG( kInfo.m_iUnitUID );
			m_mapVerifyInfo.erase( it );
		}
		break;
	}
}


#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}





void CKTDNUDP::ProcessRecvData()
{
	KTDXPROFILE_BEGIN( "SWITCH" );

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	list<RecvData>::iterator it;
	list<RecvData>::iterator iend = m_RecvDataList.end();
	for( it = m_RecvDataList.begin(); it != iend; )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	for( int i = 0; i < (int)m_RecvDataList.size(); i++ )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		RecvData* pRecvData = &(*it);
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		RecvData* pRecvData = m_RecvDataList[i];
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

        bool    bSkipReturnRecvData = false;

		switch( pRecvData->GetID() )
		{			
		case SP_CONNECT_TEST_REQ: // for client
		case SP_CONNECT_TEST_ACK: // for client
			break;

        case SP_DEFENCE_PORT:
			break;

//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
        case SP_RETRY_INTERNAL_CONNECT_REQ:
        case SP_RETRY_INTERNAL_CONNECT_ACK:
            break;
//#endif  SERV_KTDX_RETRY_USING_INTERNAL_IP

		case SP_CONNECT_RELAY_REQ: // for relay server
			{
                int iPacketSize = pRecvData->GetSize();
                if ( iPacketSize < sizeof(SSP_CONNECT_RELAY_REQ) )
                    break;
				const SSP_CONNECT_RELAY_REQ* pSSP_CONNECT_RELAY_REQ = (const SSP_CONNECT_RELAY_REQ*) pRecvData->GetData();
				Peer* pPeer		= _GetPeer( pSSP_CONNECT_RELAY_REQ->m_UID );
				if( pPeer != NULL )
				{
                    pPeer->m_IPAddress		= pRecvData->m_SenderIPAddress;
					pPeer->m_Port	= pRecvData->m_SenderPort;
                    if ( pPeer->m_dwRelayUIDsStamp != pSSP_CONNECT_RELAY_REQ->m_dwStamp )
                    {
                        pPeer->m_dwRelayUIDsStamp = 0;
                        pPeer->m_vecRelayUIDs.resize( 0 );
                    }
				}
				else
				{
					pPeer = _AddPeer( pSSP_CONNECT_RELAY_REQ->m_UID, 
                        pRecvData->m_SenderIPAddress, 
                        pRecvData->m_SenderPort );
                    //ASSERT( pPeer != NULL );
				}
				SSP_CONNECT_RELAY_ACK sSP_CONNECT_RELAY_ACK;
				sSP_CONNECT_RELAY_ACK.m_UID = pSSP_CONNECT_RELAY_REQ->m_UID;
                sSP_CONNECT_RELAY_ACK.m_dwStamp = pPeer->m_dwRelayUIDsStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                sSP_CONNECT_RELAY_ACK.m_dwTimestamp = pSSP_CONNECT_RELAY_REQ->m_dwTimestamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
   				SendToIP( pRecvData->m_SenderIPAddress, pRecvData->m_SenderPort, SP_CONNECT_RELAY_ACK, (char*)&sSP_CONNECT_RELAY_ACK, sizeof(SSP_CONNECT_RELAY_ACK) );
                if ( iPacketSize > sizeof(SSP_CONNECT_RELAY_REQ) && pPeer->m_dwRelayUIDsStamp != 0
                    && pPeer->m_vecRelayUIDs.empty() == false )
                {
                    const char* pBuffer = pRecvData->GetData() + sizeof(SSP_CONNECT_RELAY_REQ);
                    int size = iPacketSize - sizeof(SSP_CONNECT_RELAY_REQ);
                    Relay( pPeer->m_vecRelayUIDs.size(), &pPeer->m_vecRelayUIDs.front(), pBuffer, size );
                }//if
			}
			break;

		case SP_CONNECT_RELAY_ACK: // for client
			break;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
        case SP_RELAY_SET_UID_LIST_REQ:
            {
                int iPacketSize = pRecvData->GetSize();
                if ( iPacketSize < sizeof(SSP_RELAY_SET_UID_LIST_REQ) )
                    break;
                const SSP_RELAY_SET_UID_LIST_REQ* pSP_RELAY_SET_UID_LIST_REQ = (const SSP_RELAY_SET_UID_LIST_REQ*) pRecvData->GetData();
                int iNumUIDs = (int) pSP_RELAY_SET_UID_LIST_REQ->m_byNumUIDs;
                int iRealPacketSize = sizeof(SSP_RELAY_SET_UID_LIST_REQ) + iNumUIDs * sizeof(__int64);
                if ( iPacketSize < iRealPacketSize )
                    break;
				Peer* pPeer		= _GetPeer( pSP_RELAY_SET_UID_LIST_REQ->m_UID );
                if ( pPeer == NULL )
                    break;
                pPeer->m_dwRelayUIDsStamp = pSP_RELAY_SET_UID_LIST_REQ->m_dwStamp;
                if ( pSP_RELAY_SET_UID_LIST_REQ->m_dwStamp == 0 )
                {
                    pPeer->m_vecRelayUIDs.resize( 0 );
                }
                else
                {
                    pPeer->m_vecRelayUIDs.resize( iNumUIDs );
                    if ( iNumUIDs > 0 )
                    {
                        pPeer->m_vecRelayUIDs.assign( &pSP_RELAY_SET_UID_LIST_REQ->m_aUIDs[0], &pSP_RELAY_SET_UID_LIST_REQ->m_aUIDs[iNumUIDs] );
                    }
                }
                SSP_RELAY_SET_UID_LIST_ACK kACK;
                kACK.m_UID = pSP_RELAY_SET_UID_LIST_REQ->m_UID;
                kACK.m_dwStamp = pSP_RELAY_SET_UID_LIST_REQ->m_dwStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                kACK.m_dwTimestamp = pSP_RELAY_SET_UID_LIST_REQ->m_dwTimestamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
				SendToIP( 
                    pRecvData->m_SenderIPAddress, 
                    pRecvData->m_SenderPort, SP_RELAY_SET_UID_LIST_ACK, (char*)&kACK, sizeof(SSP_RELAY_SET_UID_LIST_ACK) );
                if ( iPacketSize > iRealPacketSize && pPeer->m_dwRelayUIDsStamp != 0
                    && pPeer->m_vecRelayUIDs.empty() == false )
                {
                    const char* pBuffer = pRecvData->GetData() + iRealPacketSize;
                    int size = iPacketSize - iRealPacketSize;
                    Relay( pPeer->m_vecRelayUIDs.size(), &pPeer->m_vecRelayUIDs.front(), pBuffer, size );
                }//if
			}
			break;


        case SP_RELAY_SET_UID_LIST_ACK:
			break;


        case SP_RELAY_PRECONFIG:
            {
                int iPacketSize = pRecvData->GetSize();
                if ( iPacketSize <= sizeof(SSP_RELAY_PRECONFIG) )
                    break;
                const SSP_RELAY_PRECONFIG* pSSP_RELAY_PRECONFIG = (const SSP_RELAY_PRECONFIG*) pRecvData->GetData();
				Peer* pPeer		= _GetPeer( pSSP_RELAY_PRECONFIG->m_UID );
                if ( pPeer == NULL )
                    break;
                if ( pPeer->m_dwRelayUIDsStamp != 0 && pPeer->m_vecRelayUIDs.empty() == false )
                {
                    const char* pBuffer = pRecvData->GetData() + sizeof(SSP_RELAY_PRECONFIG);
                    int size = iPacketSize - sizeof(SSP_RELAY_PRECONFIG);
                    Relay( pPeer->m_vecRelayUIDs.size(), &pPeer->m_vecRelayUIDs.front(), pBuffer, size );
                }//if
            }
            break;

        case SP_RELAY:
            {
                int iPacketSize = pRecvData->GetSize();
                if ( iPacketSize <= sizeof(SSP_RELAY) )
                    break;
                const SSP_RELAY* pSSP_RELAY = (const SSP_RELAY*) pRecvData->GetData();
                int iNumUIDs = (int) pSSP_RELAY->m_byNumUIDs;
                int iRealPacketSize = sizeof(SSP_RELAY) + iNumUIDs * sizeof(__int64);
                if ( iNumUIDs <= 0 || iPacketSize <= iRealPacketSize )
                    break;
				Peer* pPeer	= _GetPeer( pSSP_RELAY->m_UID );
                if ( pPeer == NULL )
                    break;
                const __int64* pUIDs = (const __int64*) ( pRecvData->GetData() + sizeof(SSP_RELAY) );
                const char* pBuffer = pRecvData->GetData() + iRealPacketSize;
                int size = iPacketSize - iRealPacketSize;
                Relay( iNumUIDs, pUIDs, pBuffer, size );
			}
			break;

//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		

			//{{ 2012. 10. 09	박세훈	랜선 렉 악용 유저 UDP 체커
		case SP_CONNECT_CHECK_REQ: // for relay server
			break;

		case SP_CONNECT_CHECK_ACK:
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
		    {
                int iPacketSize = pRecvData->GetSize();
                if ( iPacketSize < sizeof(SSP_CONNECT_CHECK_ACK) )
                    break;
			    const SSP_CONNECT_CHECK_ACK* pSSP_CONNECT_CHECK_ACK = ( const SSP_CONNECT_CHECK_ACK* ) pRecvData->GetData();

			    std::map< __int64, LB_INFO_STRUCT_VERIFY >::iterator it = m_mapVerifyInfo.find( pSSP_CONNECT_CHECK_ACK->m_iUnitUID );
			    if( ( it != m_mapVerifyInfo.end() ) &&
				    ( it->second.m_bState == true ) &&
				    ( it->second.m_iHeartBeatUID == pSSP_CONNECT_CHECK_ACK->m_iHeartBeatUID )
				    )
			    {
                    LONG    lTimerStampDiff = (LONG) ( pRecvData->m_dwRecvTimestamp - it->second.m_dwTimerStamp );
                    float   fTimeDiff = lTimerStampDiff * ( 1.f/1000.f );
				    if( fTimeDiff < SiKGameSysVal()->GetLanBugOutVerifyTime() )
				    {
					    // 제시간에 도착함. 다음 검증 패킷 날릴 준비를 하자
					    it->second.m_bState			= false;
					    it->second.m_fVerifyTerm	= SiKGameSysVal()->GetLanBugOutVerifyTerm();
					    //it->second.m_kTimer.restart();
                        it->second.m_dwTimerStamp = ::GetTickCount();

					    START_LOG( clog2, L"랜선렉 로그: 신고자 검증 체크 패킷 도착" )
						    << BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iUnitUID )
						    << BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iHeartBeatUID );
				    }
				    else
				    {
					    // 제시간에 도착하지 못함. 악용 유저로 판단.
					    // Tick에서 처리 하도록 하자.
					    START_LOG( clog2, L"랜선렉 로그: 신고자 검증 체크 패킷 도착( TimeOut )" )
						    << BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iUnitUID )
						    << BUILD_LOG( pSSP_CONNECT_CHECK_ACK->m_iHeartBeatUID );
				    }
			    }
		    }
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
			break;
			//}}

        case SP_HEART_BEAT_REQ:
            break;

        case SP_HEART_BEAT_ACK:
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
		    {
                int iPacketSize = pRecvData->GetSize();
                if ( iPacketSize < sizeof(SSP_HEART_BEAT_ACK) )
                    break;
			    const SSP_HEART_BEAT_ACK* pSSP_HEART_BEAT_ACK = ( const SSP_HEART_BEAT_ACK* ) pRecvData->GetData();
			    std::map< __int64, LB_INFO_STRUCT_CHECKER >::iterator it = m_mapCheckInfo.find( pSSP_HEART_BEAT_ACK->m_iUnitUID );
			    if( ( it != m_mapCheckInfo.end() ) &&
				    ( it->second.m_bState == true ) &&
				    ( it->second.m_iHeartBeatUID == pSSP_HEART_BEAT_ACK->m_iHeartBeatUID )
				    )
			    {
                    DWORD   dwCurTimerStamp = ::GetTickCount();
                    LONG    lTimerStampDiff = (LONG) ( pRecvData->m_dwRecvTimestamp - it->second.m_dwTimerStamp );
                    float   fTimeDiff = lTimerStampDiff * ( 1.f/1000.f );
				    //if( it->second.m_kTimer.elapsed() < SiKGameSysVal()->GetLanBugOutCheckTime() )
                    if( fTimeDiff < SiKGameSysVal()->GetLanBugOutCheckTime() )
				    {
					    // 제시간에 도착함. 다음 검증 패킷 날릴 준비를 하자
					    it->second.m_bState		= false;
					    it->second.m_fCheckTerm	= SiKGameSysVal()->GetLanBugOutCheckTerm();
					    //it->second.m_kTimer.restart();
                        it->second.m_dwTimerStamp = ::GetTickCount();
					    START_LOG( clog, L"랜선렉 로그: 신고자 감시 체크 패킷 도착" )
						    << BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iUnitUID )
						    << BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iHeartBeatUID );
				    }
				    else
				    {
					    // 제시간에 도착하지 못함. 악용 유저로 판단.
					    // Tick에서 처리 하도록 하자.
					    START_LOG( clog, L"랜선렉 로그: 신고자 감시 체크 패킷 도착( TimeOut )" )
						    << BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iUnitUID )
						    << BUILD_LOG( pSSP_HEART_BEAT_ACK->m_iHeartBeatUID );
				    }
			    }
		    }
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
		    break;

		default:
            bSkipReturnRecvData = true;
			break;
		}

        if ( bSkipReturnRecvData == true )
        {
            ++it;
        }
        else
        {
			ReturnRecvData( it++ );
        }
	}
	KTDXPROFILE_END();

#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    {
        KLocker csLock( m_csLBInfo );
        m_vecLBInfo.swap( m_vecLocalLBInfo );
    }
    if ( m_vecLocalLBInfo.empty() == false )
    {
        for( std::vector<LB_INFO_STRUCT>::const_iterator it = m_vecLocalLBInfo.begin();
            it != m_vecLocalLBInfo.end(); ++it )
        {
            _ProcessMonitoringNot( *it );
        }
        m_vecLocalLBInfo.resize( 0 );
    }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY

	map<__int64,Peer>::iterator iter;
	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;
		pPeer->m_SleepTime += m_fElapsedTime_FrameMove;

		if( pPeer->m_SleepTime > 350.0f )
		{
			RemovePeer( pPeer->m_UID );
			break;
		}
	}

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	if( m_fConnectTestTime > 0.0f && ConnectTestResult() == false )
//	{
//		m_fConnectTestTime -= m_fElapsedTime_FrameMove;
//		if( m_fConnectTestTime <= 0.0f )
//		{
//			m_fConnectTestTime = 0.0f;
//			ConnectTestToPeer();
//		}		
//	}	
//
//	if( m_fConnectRelayTime > 0.0f && ConnectRelayTestResult() == false )
//	{
//		m_fConnectRelayTime -= m_fElapsedTime_FrameMove;
//		if( m_fConnectRelayTime <= 0.0f )
//		{
//			m_fConnectRelayTime = 0.0f;
//			ConnectTestToRelay();
//		}		
//	}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void CKTDNUDP::ReturnRecvData( list<RecvData>::iterator it )
{
	m_RecvFreeDataList.splice( m_RecvFreeDataList.end(), m_RecvDataList, it );
}

void CALLBACK CKTDNUDP::_CompletionRoutine_Send(
	DWORD dwError
	, DWORD cbTransferred
	, LPWSAOVERLAPPED lpOverlapped
	, DWORD dwFlags )
{
	OverlappedDataInOp* pSendInOp = (OverlappedDataInOp*) ( lpOverlapped );
	if ( pSendInOp == NULL )
		return;

	CKTDNUDP* pKTDNUDP = pSendInOp->m_pKTDNUDP;
	if ( pKTDNUDP == NULL )
		return;

	if ( dwError == 0 && cbTransferred != 0 )
	{
		pKTDNUDP->_SendOverlapped( *pSendInOp, cbTransferred );
	}//if

	pSendInOp->m_iSendCounter--;
	if( pSendInOp->m_iSendCounter <= 0 )
	{
		pKTDNUDP->_ReturnSendOverlappedData( *pSendInOp );
	}
}

void CALLBACK CKTDNUDP::_CompletionRoutine_Recv(
	DWORD dwError
	, DWORD cbTransferred
	, LPWSAOVERLAPPED lpOverlapped
	, DWORD dwFlags )
{
	OverlappedDataInOp* pRecvInOp = (OverlappedDataInOp*) ( lpOverlapped );
	if ( pRecvInOp == NULL )
		return;

	CKTDNUDP* pKTDNUDP = pRecvInOp->m_pKTDNUDP;
	if ( pKTDNUDP == NULL )
		return;

	if ( dwError == 0 && cbTransferred != 0 )
	{
		pKTDNUDP->_RecvOverlapped( *pRecvInOp, cbTransferred );
	}//if

	pKTDNUDP->_ReturnRecvOverlappedData( *pRecvInOp );
}//_CompletionRoutine_Recv()

void CALLBACK CKTDNUDP::_CompletionRoutine_RecvOverlappedForRelayTest(
	DWORD dwError
	, DWORD cbTransferred
	, LPWSAOVERLAPPED lpOverlapped
	, DWORD dwFlags )
{
	OverlappedDataInOp* pRecvInOp = (OverlappedDataInOp*) ( lpOverlapped );
	if ( pRecvInOp == NULL )
		return;

	CKTDNUDP* pKTDNUDP = pRecvInOp->m_pKTDNUDP;
	if ( pKTDNUDP == NULL )
		return;

	bool    bProcessed = false;

	if ( dwError == 0 && cbTransferred != 0 )
	{
		bProcessed = pKTDNUDP->_RecvOverlappedForRelayTest( *pRecvInOp, cbTransferred );
	}//if

	pKTDNUDP->_ReturnRecvOverlappedData( *pRecvInOp );
}//_CompletionRoutine_Recv()

list<CKTDNUDP::OverlappedDataInOp>::iterator CKTDNUDP::_GetEmptySendOverlappedData()
{
	if( m_listOverlappedFree.empty() )
	{
		m_listSendOverlappedInOp.resize( m_listSendOverlappedInOp.size() + 1 );
        m_listSendOverlappedInOp.back().m_wsaBuf.len = RECV_BUFFER_SIZE;
	}
	else
	{
		m_listSendOverlappedInOp.splice( m_listSendOverlappedInOp.end()
			, m_listOverlappedFree, m_listOverlappedFree.begin() );
		m_listSendOverlappedInOp.back().Init();
        m_listSendOverlappedInOp.back().m_wsaBuf.len = RECV_BUFFER_SIZE;
	}

	list<OverlappedDataInOp>::iterator iter = m_listSendOverlappedInOp.end(); iter--;
    iter->m_iterList = iter;
    iter->m_pKTDNUDP = this;
	return iter;
}

bool CKTDNUDP::_SendOverlapped( OverlappedDataInOp& sendInOp, DWORD dwSendLength )
{
	//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
	m_iUDPSendTraffic += static_cast<__int64>(dwSendLength);
	//}}

	return true;
}

void CKTDNUDP::_ReturnSendOverlappedData( OverlappedDataInOp& sendInOp )
{
	m_listOverlappedFree.splice( m_listOverlappedFree.end(), m_listSendOverlappedInOp, sendInOp.m_iterList );
}

list<CKTDNUDP::OverlappedDataInOp>::iterator CKTDNUDP::_GetEmptyRecvOverlappedData()
{
	if( m_listOverlappedFree.empty() )
	{
		m_listRecvOverlappedInOp.resize( m_listRecvOverlappedInOp.size() + 1 );
        m_listRecvOverlappedInOp.back().m_wsaBuf.len = RECV_BUFFER_SIZE;
	}
	else
	{
		m_listRecvOverlappedInOp.splice( m_listRecvOverlappedInOp.end()
			, m_listOverlappedFree, m_listOverlappedFree.begin() );
		m_listRecvOverlappedInOp.back().Init();
        m_listRecvOverlappedInOp.back().m_wsaBuf.len = RECV_BUFFER_SIZE;
	}
	list<OverlappedDataInOp>::iterator iter = m_listRecvOverlappedInOp.end(); iter--;
    iter->m_iterList = iter;
    iter->m_pKTDNUDP = this;
	return iter;
}

void CKTDNUDP::_ReturnRecvOverlappedData( OverlappedDataInOp& recvInOp )
{
	m_listOverlappedFree.splice( m_listOverlappedFree.begin(), m_listRecvOverlappedInOp, recvInOp.m_iterList );
}

bool CKTDNUDP::_RecvOverlapped( OverlappedDataInOp& recvInOp, DWORD dwRecvLength )
{
	if( dwRecvLength <= 0 || dwRecvLength > RECV_BUFFER_SIZE )
		return false;

	//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
	m_iUDPRecvTraffic += static_cast<__int64>(dwRecvLength);
	//}}

	RecvData& kNewRecvData = GetEmptyRecvData();

#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
    kNewRecvData.m_dwRecvTimestamp = 0;
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
	kNewRecvData.m_SenderIPAddress = recvInOp.m_sin.sin_addr.S_un.S_addr;
	kNewRecvData.m_SenderPort	= ntohs( recvInOp.m_sin.sin_port );
	//kNewRecvData.m_ID			= recvInOp.m_wsaBuf.buf[0];
    recvInOp.SwapRecvBuffer( kNewRecvData );
    kNewRecvData.m_iRecvBufferSize = dwRecvLength;
    recvInOp.m_wsaBuf.len = RECV_BUFFER_SIZE;
	switch( kNewRecvData.GetID() )
	{
//{{ 2013. 2. 15	박세훈	랜선렉 방지 코드2
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
    case SP_CONNECT_CHECK_ACK:
    case SP_HEART_BEAT_ACK:
        kNewRecvData.m_dwRecvTimestamp = ::GetTickCount();
        break;
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
	}

	return true;
}

bool CKTDNUDP::_RecvOverlappedForRelayTest( OverlappedDataInOp& recvInOp, DWORD dwRecvLength )
{
	if( dwRecvLength <= 0 || dwRecvLength > RECV_BUFFER_SIZE )
		return false;

	//{{ 2010. 07. 05  최육사	UDP 트래픽 로그
	m_iUDPRecvTraffic += static_cast<__int64>(dwRecvLength);
	//}}

	RecvData& kNewRecvData = GetEmptyRecvData();
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
    kNewRecvData.m_dwRecvTimestamp = 0;
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
	kNewRecvData.m_SenderIPAddress = recvInOp.m_sin.sin_addr.S_un.S_addr;
	kNewRecvData.m_SenderPort	= ntohs( recvInOp.m_sin.sin_port );
	//kNewRecvData.m_ID			= recvInOp.m_wsaBuf.buf[0];
	//kNewRecvData.m_Size		= dwRecvLength-1;
	//kNewRecvData.SetData( &recvInOp.m_wsaBuf.buf[1], dwRecvLength-1 );
    recvInOp.SwapRecvBuffer( kNewRecvData );
	switch( kNewRecvData.GetID() )
	{
//{{ 2013. 2. 15	박세훈	랜선렉 방지 코드2
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
    case SP_CONNECT_CHECK_ACK:
    case SP_HEART_BEAT_ACK:
        kNewRecvData.m_dwRecvTimestamp = ::GetTickCount();
        break;
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
	}

	return true;
}

CKTDNUDP::RecvData& CKTDNUDP::GetEmptyRecvData()
{
	if( m_RecvFreeDataList.empty() )
	{
		m_RecvDataList.resize( m_RecvDataList.size() + 1 );
	}
	else
	{
		m_RecvDataList.splice( m_RecvDataList.end()
			, m_RecvFreeDataList, m_RecvFreeDataList.begin() );
		m_RecvDataList.back().Init();
	}

	return m_RecvDataList.back();
}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
/*static*/
u_long  CKTDNUDP::ConvertIPToAddress( const WCHAR* pIP )
{
    if ( pIP == NULL )
        return INADDR_ANY;

	string mbIP;
	ConvertWCHARToChar( mbIP, pIP );
	return inet_addr( mbIP.c_str() );
}

/*static*/
std::wstring    CKTDNUDP::ConvertAddressToIP( u_long ulIP )
{
    std::wstring    wstrIP;
    if ( ulIP == INADDR_ANY )
        return wstrIP;
    in_addr IP;
    IP.s_addr = ulIP;
    ConvertCharToWCHAR( wstrIP, inet_ntoa( IP ) );
    return  wstrIP;
}

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    // KTDNUDP 개체를 생성한 thread에서만 사용가능!
void    CKTDNUDP::ThreadUnsafe_ObtainOverlappedData( std::list<OverlappedDataInOp>& list1, std::list<OverlappedDataInOp>& list2 )
{
    list1.clear();
    list2.clear();
    list1.swap( m_listSendOverlappedInOp );
    list2.swap( m_listRecvOverlappedInOp );
}
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
void CKTDNUDP::Relay( unsigned uNumUIDS, const __int64* pUIDs, const char* pData, int iSize )
{
    if (uNumUIDS == 0 || pUIDs == NULL || pData == NULL || iSize <= 0 )
        return;

	list<OverlappedDataInOp>::iterator iter = _GetEmptySendOverlappedData();
	OverlappedDataInOp& sendInOp = *iter;
	sendInOp.m_wsaBuf.len = iSize;
	sendInOp.m_iSendCounter = uNumUIDS;

	memcpy( &sendInOp.m_wsaBuf.buf[0], pData, iSize );

	for( int i = 0; i < (int) uNumUIDS; i++ )
	{
		__int64 UID = pUIDs[i];
		if( const Peer* pPeer = GetPeer( UID ) )
		{
			//목적지 설정
			SOCKADDR_IN	destination;
			destination.sin_family		= AF_INET;
            destination.sin_addr.s_addr	= pPeer->m_IPAddress;
			destination.sin_port		= htons( pPeer->m_Port );  //포트번호

			int iRet = ::WSASendTo( m_Socket 
				, &sendInOp.m_wsaBuf
				, 1
				, NULL
				, 0
				, (const struct sockaddr*) &destination
				, sizeof(destination)
				, &sendInOp.m_overlapped
				, &CKTDNUDP::_CompletionRoutine_Send );

			if( iRet == 0 )//이미 completion routine이 스케줄 되었다.
			{
				continue;
			}
			else if( iRet == SOCKET_ERROR && ::WSAGetLastError() == WSA_IO_PENDING )
			{
				continue;
			}
		}
		sendInOp.m_iSendCounter--;
		if( sendInOp.m_iSendCounter == 0)
        {
			_ReturnSendOverlappedData( sendInOp );
            return;
        }
	}
}
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	