#include "StdAfx.h"
#include ".\ktdnudp.h"
#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_CHECKSUM
#include <ws2tcpip.h> //newer functions and structs used to retrieve IP addresses
#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_CHECKSUM




CKTDNUDP::CKTDNUDP( int port )
#ifdef UDP_PACKET_ANALYSIS_LOG
	:  m_bUdpPacketAnalysisLog( false )
	, m_fSendAnalysisTime( 0.0f )
	, m_fReceiveAnalysisTime( 0.0f )
#endif//UDP_PACKET_ANALYSIS_LOG
//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//	, m_pkServerProtocol( NULL )
//#endif//X2OPTIMIZE_TCP_RELAY_TEST
{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_MyIPAddress = INADDR_ANY;
    m_RelayIPAddress = INADDR_ANY;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef NO_P2P_NO_GAME
	m_uiElapsedFrameAfterPacketReceive = 0;
#endif NO_P2P_NO_GAME

//#ifdef UDP_DOWNLOAD_BLOCK_CHECK
//	m_fConnectCheckTime = 0.f;
//	m_bConnectCheck = true;
//	m_bReceiveCheckResult = false;
//#endif //UDP_DOWNLOAD_BLOCK_CHECK
    m_fElapsedTime = 0.0f;
    m_dwSendByte = 0;
    m_dwSendCount = 0;

	m_MyPort	= port;
	m_MyExtPort	= m_MyPort;
	m_RelayPort	= 0;

#ifdef CLIENT_PORT_CHANGE_REQUEST
	m_MyPortIndex = 0;
	m_MyOldPort = 0;
#endif //CLIENT_PORT_CHANGE_REQUEST

	char strTemp[256];
	::gethostname( strTemp, 256 );

	HOSTENT* hostent;
	hostent		= ::gethostbyname( strTemp );
	for ( int i = 0; hostent->h_addr_list[ i ] != 0 && i < 10; ++i )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        u_long IPAddress = ( ( struct in_addr* )*hostent->h_addr_list )->S_un.S_addr;
        m_MyIPList.push_back( IPAddress );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		struct in_addr addr;
//
//		memcpy( &addr, hostent->h_addr_list[ i ], sizeof( struct in_addr ) );
//		string IP = inet_ntoa( addr );
//		ConvertCharToWCHAR( m_MyIP, IP.c_str() );
//		m_MyIPList.push_back( m_MyIP );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}

	m_MyIPIndex = -1;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_MyIPAddress = m_MyIPList.front();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_MyIP = m_MyIPList[0].c_str();
//	string IP;
//	ConvertWCHARToChar( IP, m_MyIP.c_str() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	m_Socket		= socket( AF_INET, SOCK_DGRAM, IPPROTO_IP );
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

	m_LocalAddr.sin_family		= AF_INET;
	m_LocalAddr.sin_port		= htons( m_MyPort );
	m_LocalAddr.sin_addr.s_addr = INADDR_ANY;
	//m_LocalAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
	int ret		= bind( m_Socket, (SOCKADDR*)&m_LocalAddr, sizeof(m_LocalAddr) );

	while( ret != 0 )
	{
		m_MyPort++;
		m_MyExtPort					= m_MyPort;
		m_LocalAddr.sin_family		= AF_INET;
		m_LocalAddr.sin_port		= htons( m_MyPort );
		m_LocalAddr.sin_addr.s_addr = INADDR_ANY;
		//m_LocalAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
		ret							= bind( m_Socket, (SOCKADDR*)&m_LocalAddr, sizeof(m_LocalAddr) );
	}
	m_RecvEvent = WSACreateEvent();
	if( m_RecvEvent == WSA_INVALID_EVENT )
	{
		int retVal = WSAGetLastError();
		switch( retVal )
		{
			case WSANOTINITIALISED:
				{
					ErrorLogMsg( KEM_ERROR116, L"WSANOTINITIALISED" );
				}
				break;

			case WSAENETDOWN:
				{
					ErrorLogMsg( KEM_ERROR116, L"WSAENETDOWN" );
				}
				break;

			case WSAEINPROGRESS:
				{
					ErrorLogMsg( KEM_ERROR116, L"WSAEINPROGRESS" );
				}
				break;

			case WSA_NOT_ENOUGH_MEMORY:
				{
					ErrorLogMsg( KEM_ERROR116, L"WSA_NOT_ENOUGH_MEMORY" );
				}
				break;

			default:
				{
					ErrorLogMsg( KEM_ERROR116, L"UNKNOWN" );
				}
				break;
		}
	}

	int retval = ::WSAEventSelect( m_Socket, m_RecvEvent, FD_READ | FD_CLOSE );
	if( retval != 0 )
	{
		int retVal = WSAGetLastError();
		switch( retVal )
		{
			case WSANOTINITIALISED:
				{
					ErrorLogMsg( KEM_ERROR117, L"WSANOTINITIALISED" );
				}
				break;

			case WSAENETDOWN:
				{
					ErrorLogMsg( KEM_ERROR117, L"WSAENETDOWN" );
				}
				break;

			case WSAEINVAL:
				{
					ErrorLogMsg( KEM_ERROR117, L"WSAEINVAL" );
				}
				break;

			case WSAEINPROGRESS:
				{
					ErrorLogMsg( KEM_ERROR117, L"WSAEINPROGRESS" );
				}
				break;

			case WSAENOTSOCK:
				{
					ErrorLogMsg( KEM_ERROR117, L"WSAENOTSOCK" );
				}
				break;

			default:
				{
					ErrorLogMsg( KEM_ERROR117, L"UNKNOWN" );
				}
				break;
		}
	}

	ZeroMemory( m_pRecvBuffer, RECV_BUFFER_SIZE );
	m_RecvDataList.reserve( 1000 );

	//m_fLocalWaitTime		= 0.0f;
	m_fConnectTestTime		= 0.0f;

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_eRelayState = RELAY_STATE_DISCONNECTED;
    m_fRelayTimer = 0.f;
    m_iRelayTimerCounter = 0;
    m_dwRelayUIDStamp = 0;
    m_dwRelayUIDStampFromServer = 0;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_bConnectRelay			= false;
//	m_fConnectRelayTime		= 0.0f;
//	m_fRelayContinueTime	= 0.0f;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    m_kRelayStatistic.Init();
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

	m_MyUID					= 0;
	m_fDefencePortTime		= 30.0f;

//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//치트키용
	m_eForceConnectMode = FORCE_CONNECT_MODE_DEFAULT;
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
}

CKTDNUDP::~CKTDNUDP(void)
{
	if( m_Socket != INVALID_SOCKET )
	{
		shutdown( m_Socket, SD_BOTH );
		closesocket( m_Socket );
		m_Socket = INVALID_SOCKET;
	}
	
	WSACloseEvent( m_RecvEvent );

	for( int i = 0; i < (int)m_RecvDataList.size(); i++ )
	{
		RecvData* pRecvData = m_RecvDataList[i];
		SAFE_DELETE( pRecvData );
	}

	m_PeerMap.clear();
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_vecNonRelayUIDs.clear();
    m_vecRelayUIDs.clear();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	m_RecvDataList.clear();
}

void CKTDNUDP::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	WSANETWORKEVENTS	netEvent;
	bool				bRetVal = false;


	
#ifdef NO_P2P_NO_GAME
	m_uiElapsedFrameAfterPacketReceive += 1;
#endif NO_P2P_NO_GAME

	//KTDXPROFILE_BEGIN( "RECEIVE_WAIT" );
	//for( int i = 0; i < (int)m_RecvDataWaitList.size(); i++ )
	//{
	//	RecvData* pRecvData = m_RecvDataWaitList[i];
	//	pRecvData->m_fLocalWaitTime -= fElapsedTime;
	//	if( pRecvData->m_fLocalWaitTime < 0.0f )
	//	{
	//		m_RecvDataList.push_back( pRecvData );
	//		m_RecvDataWaitList.erase( m_RecvDataWaitList.begin() + i );
	//		i--;
	//	}
	//}
	//KTDXPROFILE_END();

	if( m_fDefencePortTime < 0.0f )
	{
		DefencePort();
		m_fDefencePortTime = 30.0f;
	}
	else
	{
		m_fDefencePortTime -= fElapsedTime;
	}

	//패킷 리시브
	KTDXPROFILE_BEGIN( "RECEIVE" );
	while( true )
	{
		KTDXPROFILE_BEGIN( "RECEIVE_EVENT" );
		//Event select
		::ZeroMemory( &netEvent, sizeof(netEvent) );
		
		::WSAEnumNetworkEvents( m_Socket, m_RecvEvent, &netEvent );
		KTDXPROFILE_END();

		if( (netEvent.lNetworkEvents & FD_READ) == FD_READ )
			Recv();
		else
			break;
	}
	KTDXPROFILE_END();

	KTDXPROFILE_BEGIN( "SWITCH" );
	for( int i = 0; i < (int)m_RecvDataList.size(); i++ )
	{
		RecvData* pRecvData = m_RecvDataList[i];
        bool bKeepRecvData = false;
		switch( pRecvData->m_ID )
		{
		case SP_CONNECT_TEST_REQ:
			{
                int iPacketSize = pRecvData->m_Size;
                if ( iPacketSize < sizeof(SSP_CONNECT_TEST_REQ) )
                    break;
                //StateLog( L"피어 요청" );
				const SSP_CONNECT_TEST_REQ* pSSP_CONNECT_TEST_REQ = (const SSP_CONNECT_TEST_REQ*)pRecvData->m_pRecvBuffer;
				SSP_CONNECT_TEST_ACK sSP_CONNECT_TEST_ACK;
				const Peer* pMyPeer = GetMyPeer();
				if( pMyPeer != NULL )
				{
					sSP_CONNECT_TEST_ACK.m_UID		= pMyPeer->m_UID;
                    sSP_CONNECT_TEST_ACK.m_bInternalIP = pSSP_CONNECT_TEST_REQ->m_bInternalIP;
					SendToIP( pRecvData->m_SenderIPAddress, pRecvData->m_SenderPort, SP_CONNECT_TEST_ACK, (char*)&sSP_CONNECT_TEST_ACK, sizeof(SSP_CONNECT_TEST_ACK) );
				}
				Peer* pPeer = _GetPeer( pSSP_CONNECT_TEST_REQ->m_UID );
                if ( pPeer != NULL )
                {
					if( pPeer->m_UID != m_MyUID )
					{
                        pPeer->m_IPAddress		= pRecvData->m_SenderIPAddress;
						pPeer->m_Port	= pRecvData->m_SenderPort;
                        if ( pPeer->m_eP2PConnectType == P2PCONNECT_EXTERNAL
                            && pPeer->m_InternalIPAddress != ADDR_ANY
                            && pPeer->m_InternalIPAddress == pPeer->m_IPAddress
                            && pPeer->m_InternalPort == pPeer->m_Port )
                        {
                            pPeer->m_eP2PConnectType = P2PCONNECT_INTERNAL;
                        }
					}
                    else
					{
                        pPeer->m_IPAddress		= m_MyIPAddress;
						pPeer->m_Port	= m_MyPort;
                        if ( pPeer->m_eP2PConnectType == P2PCONNECT_EXTERNAL )
                            pPeer->m_eP2PConnectType = P2PCONNECT_INTERNAL;
					}
                }

			}
			break;

		case SP_CONNECT_TEST_ACK:
			{
                int iPacketSize = pRecvData->m_Size;
                if ( iPacketSize < sizeof(SSP_CONNECT_TEST_ACK) )
                    break;
				const SSP_CONNECT_TEST_ACK* pSSP_CONNECT_TEST_ACK = (const SSP_CONNECT_TEST_ACK*)pRecvData->m_pRecvBuffer;
				Peer* pPeer = _GetPeer( pSSP_CONNECT_TEST_ACK->m_UID );
				if( pPeer != NULL )
				{
                    if  ( m_eForceConnectMode != FORCE_CONNECT_MODE_RELAY )
                    {
                        _SetUseRelay( pPeer, false );
                    }
					if( pPeer->m_UID != m_MyUID )
					{
                        pPeer->m_IPAddress			= pRecvData->m_SenderIPAddress;
						pPeer->m_Port		= pRecvData->m_SenderPort;
                        if ( pSSP_CONNECT_TEST_ACK->m_bInternalIP == true
                            || ( pPeer->m_InternalIPAddress != ADDR_ANY
                            && pPeer->m_InternalIPAddress == pPeer->m_IPAddress
                            && pPeer->m_InternalPort == pPeer->m_Port ) )
                            pPeer->m_eP2PConnectType = P2PCONNECT_INTERNAL;
                        else
                        {
                            pPeer->m_eP2PConnectType = P2PCONNECT_EXTERNAL;
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
                            if ( pPeer->m_IPAddress != ADDR_ANY 
                                && false == ( pPeer->m_InternalIPAddress == pPeer->m_IPAddress
                                && pPeer->m_InternalPort == pPeer->m_Port ) )
                            {
                                pPeer->m_ConnectTestCount = 0;
                            }
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
                        }
					}
					else
					{
                        pPeer->m_IPAddress		= m_MyIPAddress;
						pPeer->m_Port	= m_MyPort;
                        pPeer->m_eP2PConnectType = P2PCONNECT_INTERNAL;
					}
				}
			}
			break;

        case SP_DEFENCE_PORT:
            break;

			//{{ 2013. 1. 16	박세훈	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
        case SP_RETRY_INTERNAL_CONNECT_REQ:
//				{
//                    SSP_RETRY_INTERNAL_CONNECT_REQ* pSSP_REQ = (SSP_RETRY_INTERNAL_CONNECT_REQ*)pRecvData->m_pRecvBuffer;
//
//					SSP_RETRY_INTERNAL_CONNECT_ACK sSP_ACK;
//					sSP_ACK.m_UID = m_MyUID;
//					Send( pRecvData->m_SenderIP.c_str(), pRecvData->m_SenderPort, SP_RETRY_INTERNAL_CONNECT_ACK, (char*)&sSP_ACK, sizeof(SSP_RETRY_INTERNAL_CONNECT_ACK) );
//
//					Peer* pPeer		= GetPeer( pSSP_REQ->m_UID );
//
//					if( pPeer != NULL )
//					{
//						pPeer->m_IP		= pRecvData->m_SenderIP;
//						pPeer->m_Port	= pRecvData->m_SenderPort;
//					}
//					SAFE_DELETE( pRecvData );
//					m_RecvDataList.erase( m_RecvDataList.begin() + i );
//					--i;
//				}
            break;
        case SP_RETRY_INTERNAL_CONNECT_ACK:
//				{
//					SSP_RETRY_INTERNAL_CONNECT_ACK* sSP_ACK = (SSP_RETRY_INTERNAL_CONNECT_ACK*)pRecvData->m_pRecvBuffer;
//
//					Peer* pPeer = _GetPeer( sSP_ACK->m_UID );
//					if( pPeer != NULL )
//					{
//                        if  ( m_eForceConnectMode != FORCE_CONNECT_MODE_RELAY )
//                        {
//						    pPeer->m_bUseRelay		= false;
//                        }
//                        pPeer->m_bP2PConnected	= true;
//                        //pPeer->m_bUseInternalIP = true;
//
//						if( pPeer->m_UID != m_MyUID )
//						{
//							//StateLog( L"피어 응답 남" );
//							pPeer->m_IP			= pRecvData->m_SenderIP;
//							pPeer->m_Port		= pRecvData->m_SenderPort;
//						}
//						else
//						{
//							//StateLog( L"피어 응답 나" );
//							pPeer->m_IP		= m_MyIP.c_str();
//							pPeer->m_Port	= m_MyPort;
//							//StateLog( m_MyIP.c_str() );
//							//StateLog( m_MyPort );
//						}
//					}
//					SAFE_DELETE( pRecvData );
//					m_RecvDataList.erase( m_RecvDataList.begin() + i );
//					--i;
//				}
            break;
////#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//				//}}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		case SP_CONNECT_RELAY_REQ:
			break;

		case SP_CONNECT_RELAY_ACK:
			{
                int iPacketSize = pRecvData->m_Size;
                if ( iPacketSize < sizeof(SSP_CONNECT_RELAY_ACK) )
                    break;
                CONST SSP_CONNECT_RELAY_ACK* pSSP_CONNECT_RELAY_ACK = (const SSP_CONNECT_RELAY_ACK*)pRecvData->m_pRecvBuffer;
                switch( m_eRelayState )
                {
                case RELAY_STATE_TRYING_TO_CONNECT:
                    m_eRelayState = RELAY_STATE_CONNECTED;
                    m_dwRelayUIDStampFromServer = 0;
                    m_fRelayTimer = 0.f;
                    m_iRelayTimerCounter = 0;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                    m_kRelayStatistic.ProcessPingReceive( pSSP_CONNECT_RELAY_ACK->m_dwTimestamp, pRecvData->m_dwReceiveTimestamp );
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                    break;
                case RELAY_STATE_CONNECTED:
                    if ( pSSP_CONNECT_RELAY_ACK->m_dwStamp == m_dwRelayUIDStamp )
                        m_dwRelayUIDStampFromServer = pSSP_CONNECT_RELAY_ACK->m_dwStamp;
                    else
                        m_dwRelayUIDStampFromServer = 0;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                    m_kRelayStatistic.ProcessPingReceive( pSSP_CONNECT_RELAY_ACK->m_dwTimestamp, pRecvData->m_dwReceiveTimestamp );
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                    break;
                }
			}
			break;

        case SP_RELAY_SET_UID_LIST_REQ:
			break;

        case SP_RELAY_SET_UID_LIST_ACK:
            {
                int iPacketSize = pRecvData->m_Size;
                if ( iPacketSize < sizeof(SSP_RELAY_SET_UID_LIST_ACK) )
                    break;
                SSP_RELAY_SET_UID_LIST_ACK* pSSP_RELAY_SET_UID_LIST_ACK = (SSP_RELAY_SET_UID_LIST_ACK*)pRecvData->m_pRecvBuffer;
                if ( m_eRelayState == RELAY_STATE_CONNECTED )
                {
                    if ( pSSP_RELAY_SET_UID_LIST_ACK->m_dwStamp == m_dwRelayUIDStamp )
                        m_dwRelayUIDStampFromServer = pSSP_RELAY_SET_UID_LIST_ACK->m_dwStamp;
                    else
                        m_dwRelayUIDStampFromServer = 0;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                    m_kRelayStatistic.ProcessPingReceive( pSSP_RELAY_SET_UID_LIST_ACK->m_dwTimestamp, pRecvData->m_dwReceiveTimestamp );
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
                }//if
			}
			break;
        case SP_RELAY_PRECONFIG:
			break;

        case SP_RELAY:
			break;

			//{{ 2013. 2. 15	박세훈	랜선렉 방지 코드2

		case SP_CONNECT_CHECK_REQ:
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
            if ( ConnectRelayTestResult() == true )
            {
                int iPacketSize = pRecvData->m_Size;
                if ( iPacketSize < sizeof(SSP_CONNECT_CHECK_REQ) )
                    break;
				const SSP_CONNECT_CHECK_REQ* pSSP_CONNECT_CHECK_REQ = (const SSP_CONNECT_CHECK_REQ*) pRecvData->m_pRecvBuffer;
				const Peer* pPeer = GetMyPeer();
				if( pPeer != NULL )
				{
                    char ID = SP_CONNECT_CHECK_ACK;
                    SSP_CONNECT_CHECK_ACK kSSP_CONNECT_CHECK_ACK;
                    kSSP_CONNECT_CHECK_ACK.m_iUnitUID = pPeer->m_UID;
                    kSSP_CONNECT_CHECK_ACK.m_iHeartBeatUID = pSSP_CONNECT_CHECK_REQ->m_iHeartBeatUID;
                    WSABUF  alpBuffers[2];
                    alpBuffers[0].buf = (CHAR*) &ID;
                    alpBuffers[0].len = 1;
                    alpBuffers[1].buf = (CHAR*) &kSSP_CONNECT_CHECK_ACK;
                    alpBuffers[1].len = sizeof( SSP_CONNECT_CHECK_ACK );
                    SendPure_Scattered( m_RelayIPAddress, m_RelayPort, alpBuffers, 2 );
				}
            }
#endif  SERV_FIX_SYNC_PACKET_USING_RELAY
			break;

        case SP_CONNECT_CHECK_ACK:
            break;

		case SP_HEART_BEAT_REQ:
#ifdef  SERV_FIX_SYNC_PACKET_USING_RELAY
            if ( ConnectRelayTestResult() == true )
            {
                int iPacketSize = pRecvData->m_Size;
                if ( iPacketSize < sizeof(SSP_HEART_BEAT_REQ) )
                    break;
				const SSP_HEART_BEAT_REQ* pSSP_HEART_BEAT_REQ = (const SSP_HEART_BEAT_REQ*) pRecvData->m_pRecvBuffer;
				const Peer* pPeer = GetMyPeer();
				if( pPeer != NULL )
				{
                    char ID = SP_HEART_BEAT_ACK;
                    SSP_HEART_BEAT_ACK  kSSP_HEART_BEAT_ACK;
					kSSP_HEART_BEAT_ACK.m_iUnitUID			= pPeer->m_UID;
					kSSP_HEART_BEAT_ACK.m_iHeartBeatUID		= pSSP_HEART_BEAT_REQ->m_iHeartBeatUID;
                    WSABUF  alpBuffers[2];
                    alpBuffers[0].buf = &ID;
                    alpBuffers[0].len = 1;
                    alpBuffers[1].buf = (CHAR*) &kSSP_HEART_BEAT_ACK;
                    alpBuffers[1].len = sizeof( SSP_HEART_BEAT_ACK );
                    SendPure_Scattered( m_RelayIPAddress, m_RelayPort, alpBuffers, 2 );
				}
            }
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
			break;

        default:
            bKeepRecvData = true;
            break;
		}

        if ( bKeepRecvData == false )
        {
			SAFE_DELETE( pRecvData );
			m_RecvDataList.erase( m_RecvDataList.begin() + i );
			i--;
        }
	}
	KTDXPROFILE_END();

	map<__int64,Peer>::iterator iter;
	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;
		pPeer->m_SleepTime += fElapsedTime;
		if( pPeer->m_UID != m_MyUID && pPeer->m_SleepTime > 350.0f )
		{
			RemovePeer( pPeer->m_UID );
			break;
		}
	}

	// ConnectTestToPeer를 시도 했는데도 peer 중에 연결되지 않은 것이 있다면, 0.5초에 한번 씩 연결을 시도한다.
	// 연결 시도 시 Relay로 연결된 peer는 시도 횟수 40회를 넘지 않는 선에서 P2P연결을 재시도한다.

    if( m_fConnectTestTime > 0.0f && 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
        _RobustConnectTestResult() == false
#else   SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
        ConnectTestResult() == false 
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        ConnectTestResult() == false 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        )
	{
		m_fConnectTestTime -= fElapsedTime;
		if( m_fConnectTestTime <= 0.0f )
		{
			m_fConnectTestTime = 0.0f;
			ConnectTestToPeer();
		}		
	}	

	// ConnectTestToRelay를 시도 했는데도 아직 연결되지 않았다면,
	// 0.5초에 한번씩 재시도 한다.
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

    switch( m_eRelayState )
    {
    case RELAY_STATE_TRYING_TO_CONNECT:
        {
            if ( m_fRelayTimer >= 0.5f )
            {
                m_fRelayTimer = 0.f;
                m_iRelayTimerCounter = 0;
                ConnectTestToRelay();
            }
            m_fRelayTimer += fElapsedTime;
        }
        break;
    case RELAY_STATE_CONNECTED:
        {
            bool bTriggered = false;
            if ( m_fRelayTimer >= 1.0f )
            {
                m_fRelayTimer = 0.f;
                m_iRelayTimerCounter++;
                bTriggered = true;
            }//if

            if ( bTriggered == true )
            {
                if ( GetRelayUIDsUpdated() == false
                    && m_vecRelayUIDs.empty() == false )
                {
                    m_iRelayTimerCounter = 0;
                    SendPure_Relay( SP_RELAY_SET_UID_LIST_REQ, m_vecRelayUIDs, 0, NULL, 0 );
                }
            }

	        // Realy에 연결되어 있을 때 30초에 한번 씩 Connect 시도 Req/Ack를 송수신하여
	        // 루트가 닫히지 않도록 한다.
	        if( m_iRelayTimerCounter >= 5 )
	        {
			    m_iRelayTimerCounter = 0;
                SendPure_Relay( SP_CONNECT_RELAY_REQ, m_vecRelayUIDs, 0, NULL, 0 );
	        }//if
            m_fRelayTimer += fElapsedTime;
        }
        break;
    }//switch

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	if( m_fConnectRelayTime > 0.0f && ConnectRelayTestResult() == false )
//	{
//		m_fConnectRelayTime -= fElapsedTime;
//		if( m_fConnectRelayTime <= 0.0f )
//		{
//			m_fConnectRelayTime = 0.0f;
//			ConnectTestToRelay();
//		}		
//	}	
//
//	// Realy에 연결되어 있을 때 30초에 한번 씩 Connect 시도 Req/Ack를 송수신하여
//	// 루트가 닫히지 않도록 한다.
//	if( m_fRelayContinueTime >= 0.0f && ConnectRelayTestResult() == true )
//	{
//		m_fRelayContinueTime -= fElapsedTime;
//		if( m_fRelayContinueTime <= 0.0f )
//		{
//			m_fRelayContinueTime = 30.f;
//			ConnectTestToRelay();
//		}	
//	}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
#ifndef SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION
	//{{ 2013. 1. 16	박세훈	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	// 새로운 유저가 들어올 때마다 이미 Relay로 연결된 유저도 public, internal 한번씩 재시도 해본다. ( 시도 제한 40회 )
	CheckAndProcessInternalConnect( fElapsedTime );
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
	//}}
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

//#ifdef UDP_DOWNLOAD_BLOCK_CHECK
//	if( m_fConnectCheckTime > 0.0f )
//	{
//		m_fConnectCheckTime -= fElapsedTime;
//	}
//	else
//	{
//		m_bReceiveCheckResult = true;
//	}
//#endif //UDP_DOWNLOAD_BLOCK_CHECK

	m_fElapsedTime += fElapsedTime;
	if( m_fElapsedTime > 1.0f )
	{
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		int iRelay = 0;
//		std::map<__int64,Peer>::iterator mit;
//		for( mit = m_PeerMap.begin(); mit != m_PeerMap.end(); mit++ )
//		{
//			if( mit->second.m_bUseRelay )
//			{
//				iRelay++;
//			}
//		}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef HEAVY_LOG_TEST

		std::wstringstream wstrm;
		wstrm << L"1초당 센드 통계" << L"\t";
		wstrm << m_PeerMap.size() << L"\t";
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        wstrm << m_vecRelayUIDs.size() << L"\t";
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		wstrm << iRelay << L"\t";
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		wstrm << m_dwSendByte << L"\t";
		wstrm << m_dwSendCount << std::endl;
		StateLog( wstrm.str().c_str() );

#endif HEAVY_LOG_TEST

		m_fElapsedTime = 0.0f;
		m_dwSendByte = 0;
		m_dwSendCount = 0;
	}

#ifdef UDP_PACKET_ANALYSIS_LOG
	m_fSendAnalysisTime += fElapsedTime;
	m_fReceiveAnalysisTime += fElapsedTime;
#endif//UDP_PACKET_ANALYSIS_LOG
}

bool CKTDNUDP::Recv()
{
	KTDXPROFILE();

    SOCKADDR_IN SenderAddr;

	int SenderAddrSize	= sizeof(SenderAddr);
	int recvSize		= recvfrom( m_Socket, m_pRecvBuffer, RECV_BUFFER_SIZE, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize );

	if( recvSize == SOCKET_ERROR )
	{
		int errCode = WSAGetLastError();
		switch( errCode )
		{
			case WSANOTINITIALISED:
				ErrorLogMsg( KEM_ERROR65, "WSANOTINITIALISED" );
				break;

			case WSAENETDOWN:
				ErrorLogMsg( KEM_ERROR65, "WSAENETDOWN" );
				break;

			case WSAEFAULT:
				ErrorLogMsg( KEM_ERROR65, "WSAEFAULT" );
				break;

			case WSAEINTR:
				ErrorLogMsg( KEM_ERROR65, "WSAEINTR" );
				break;

			case WSAEINPROGRESS:
				ErrorLogMsg( KEM_ERROR65, "WSAEINPROGRESS" );
				break;

			case WSAEINVAL:
				ErrorLogMsg( KEM_ERROR65, "WSAEINVAL" );
				break;

			case WSAEISCONN:
				ErrorLogMsg( KEM_ERROR65, "WSAEISCONN" );
				break;

			case WSAENETRESET:
				ErrorLogMsg( KEM_ERROR65, "WSAENETRESET" );
				break;

			case WSAENOTSOCK:
				ErrorLogMsg( KEM_ERROR65, "WSAENOTSOCK" );
				break;

			case WSAEOPNOTSUPP:
				ErrorLogMsg( KEM_ERROR65, "WSAEOPNOTSUPP" );
				break;

			case WSAESHUTDOWN:
				ErrorLogMsg( KEM_ERROR65, "WSAESHUTDOWN" );
				break;

			case WSAEWOULDBLOCK:
				ErrorLogMsg( KEM_ERROR65, "WSAEWOULDBLOCK" );
				break;

			case WSAEMSGSIZE:
				ErrorLogMsg( KEM_ERROR65, "WSAEMSGSIZE" );
				break;

			case WSAETIMEDOUT:
				ErrorLogMsg( KEM_ERROR65, "WSAETIMEDOUT" );
				break;

			case WSAECONNRESET:
				//ErrorLogMsg( KEM_ERROR65, "WSAECONNRESET" );
				break;

			default:
				ErrorLogMsg( KEM_ERROR65, "UNKNOWN" );
				break;
		}
		
		return false;
	}
    else if( recvSize == 0 )
    {
        // 소켓이 끊겼다? UDP인데 이럴 수 있나?
        return false;
    }
    else if( recvSize < 0 || recvSize > CKTDNUDP_MTU_MAX )
    {
        // 음수는 SOCKET_ERROR 를 제외하고는 이상한 경우이다. 사이즈가 너무 커도 이상한 경우이다.
        return false;
    }
	else
	{

		//StateLog( L"뭔가 받았음" << ( int )m_pRecvBuffer[0] );

#ifdef NO_P2P_NO_GAME
		m_uiElapsedFrameAfterPacketReceive = 0;	
#endif NO_P2P_NO_GAME

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		if( m_pRecvBuffer[0] == SP_RELAY )
//		{
//			Relay( recvSize );
//		}
//		else
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
            DWORD   dwReceiveTimeStamp = 0;
            switch( m_pRecvBuffer[0] )
            {
            case SP_CONNECT_RELAY_ACK:
            case SP_RELAY_SET_UID_LIST_ACK:
                dwReceiveTimeStamp = timeGetTime();
            }//switch
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
			RecvData* pRecvData		= new RecvData;
            pRecvData->m_SenderIPAddress = SenderAddr.sin_addr.S_un.S_addr;
			pRecvData->m_SenderPort	= ntohs( SenderAddr.sin_port );
			pRecvData->m_ID			= m_pRecvBuffer[0];
			pRecvData->m_Size		= recvSize - 1;
			pRecvData->SetData( &m_pRecvBuffer[1], recvSize - 1 );
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
            pRecvData->m_dwReceiveTimestamp = dwReceiveTimeStamp;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			unsigned long uncompressSize = 2000;
//			char uncompressBuffer[2000] = {0,};
//			if( uncompress( (BYTE*)uncompressBuffer, &uncompressSize, (BYTE*)(&m_pRecvBuffer[1]), recvSize-1 ) != Z_OK )
//				return false;
//
//			RecvData* pRecvData		= new RecvData;
//			string addr				= inet_ntoa( SenderAddr.sin_addr );
//			ConvertCharToWCHAR( pRecvData->m_SenderIP, addr.c_str() );
//			pRecvData->m_SenderPort	= ntohs( SenderAddr.sin_port );
//			pRecvData->m_ID			= m_pRecvBuffer[0];
//			pRecvData->m_Size		= uncompressSize;
//			pRecvData->SetData( uncompressBuffer, uncompressSize );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

			//if( m_fLocalWaitTime > 0.0f
			//	&& pRecvData->m_SenderIP.compare( m_MyIP ) == 0 )
			//{
			//	pRecvData->m_fLocalWaitTime = m_fLocalWaitTime;
			//	m_RecvDataWaitList.push_back( pRecvData );
			//}
			//else
				m_RecvDataList.push_back( pRecvData );

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
			//혼자라면 측정 안함
			if( GetPeerMap().size() > 1 )
				KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->UpdateRecv( recvSize );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef UDP_PACKET_ANALYSIS_LOG
			if( !m_bUdpPacketAnalysisLog )
			{
				if( !m_mapReceiveAnalysisResult.empty() )
				{
					FILE* pUdpReceivePacketAnalysisFile = fopen( "Receive_UdpPacketAnalysisLog.txt", "w+");

					std::map< int, CKTDNUDP::KAnalysisResult >::const_iterator it;
					std::map< int, CKTDNUDP::KAnalysisResult >::const_iterator iend = m_mapReceiveAnalysisResult.end();
					for( it = m_mapReceiveAnalysisResult.begin(); it != iend; ++it )
						fprintf( pUdpReceivePacketAnalysisFile, "ID: %d, TransCountPerSec: %f TransSizePerSec: %f" /* " UnCompressTransSizePerSec: %f" */"\n"
                            , it->first, (float)it->second.m_dwTransCount/m_fReceiveAnalysisTime, (float)it->second.m_dwTransSize/m_fReceiveAnalysisTime /*, (float)it->second.m_dwUnCompressTransSize/m_fReceiveAnalysisTime */ );

					fclose( pUdpReceivePacketAnalysisFile );

					//
					m_mapReceiveAnalysisResult.clear();
				}

				m_fReceiveAnalysisTime = 0.0f;
			}
			else
			{
				std::map< int, KAnalysisResult >::iterator it = m_mapReceiveAnalysisResult.find( pRecvData->m_ID );
				if( it == m_mapReceiveAnalysisResult.end() )
				{
					KAnalysisResult kAnalysisResult;
					kAnalysisResult.m_dwTransCount = 1;
					kAnalysisResult.m_dwTransSize = recvSize;
					m_mapReceiveAnalysisResult.insert( std::make_pair( pRecvData->m_ID, kAnalysisResult ) );
				}
				else
				{
					it->second.m_dwTransCount++;
					it->second.m_dwTransSize += recvSize;
				}
			}
#endif//UDP_PACKET_ANALYSIS_LOG
		}
		return true;
	}
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
    return SendPure_Scattered( 
        ulIP, 
        port, alpBuffers, ( size > 0 ) ? 2 : 1 );

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	unsigned long	compressSize			= 2000;
//	char			pCompressBuffer[2000]	= {0,};
//	if( !Compress( ID, pCompressBuffer, compressSize, pBuffer, size ) )
//    {
//        return false;
//    }
//
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

    DWORD   dwNumberOfBytesSent = 0;
	int ret = ::WSASendTo( m_Socket, alpBuffers, dwBuffCount, &dwNumberOfBytesSent, 0
        , (const struct sockaddr*) &destination, sizeof(SOCKADDR_IN)
        , NULL, NULL );

	if( ret == SOCKET_ERROR )
	{
		return false;
	}
	else
	{
        //StateLog( L"센드 함 : " << (int)ID );
        m_dwSendCount++;
        //StateLog( m_dwSendCount );
        m_dwSendByte += dwNumberOfBytesSent;

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		//혼자라면 측정 안함
		if( GetPeerMap().size() > 1 )
			KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->UpdateSend( dwNumberOfBytesSent );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef UDP_PACKET_ANALYSIS_LOG
		if( !m_bUdpPacketAnalysisLog )
		{
			if( !m_mapSendAnalysisResult.empty() )
			{
				FILE* pUdpSendPacketAnalysisFile = fopen( "Send_UdpPacketAnalysisLog.txt", "w+");

				std::map< int, CKTDNUDP::KAnalysisResult >::const_iterator it;
				std::map< int, CKTDNUDP::KAnalysisResult >::const_iterator iend = m_mapSendAnalysisResult.end();
				for( it = m_mapSendAnalysisResult.begin(); it != iend; ++it )
					fprintf( pUdpSendPacketAnalysisFile, "ID: %d, TransCountPerSec: %f TransSizePerSec: %f\n", 
						it->first, (float)it->second.m_dwTransCount/m_fSendAnalysisTime, (float)it->second.m_dwTransSize/m_fSendAnalysisTime );

				fclose( pUdpSendPacketAnalysisFile );

				//
				m_mapSendAnalysisResult.clear();
			}

			m_fSendAnalysisTime = 0.0f;
		}
		else
		{
			std::map< int, KAnalysisResult >::iterator it = m_mapSendAnalysisResult.find( 0 );
			if( it == m_mapSendAnalysisResult.end() )
			{
				KAnalysisResult kAnalysisResult;
				kAnalysisResult.m_dwTransCount = 1;
				//if( bCompress )
				kAnalysisResult.m_dwTransSize = dwNumberOfBytesSent;
				//else
				//	kAnalysisResult.m_dwUnCompressTransSize = size;
				m_mapSendAnalysisResult.insert( std::make_pair( 0, kAnalysisResult ) );
			}
			else
			{
				it->second.m_dwTransCount++;
				//if( bCompress )
				it->second.m_dwTransSize += dwNumberOfBytesSent;
				//else
				//	it->second.m_dwUnCompressTransSize += size;				
			}
		}
#endif//UDP_PACKET_ANALYSIS_LOG

		return true;
	}
}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//#ifdef UDP_PACKET_ANALYSIS_LOG
//bool CKTDNUDP::SendPure( const WCHAR* pIP, int port, const char ID, const void* pBuffer, int size, bool bCompress )
//#else//UDP_PACKET_ANALYSIS_LOG
//bool CKTDNUDP::SendPure( const WCHAR* pIP, int port, const char ID, const char* pBuffer, int size )
//#endif//UDP_PACKET_ANALYSIS_LOG
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
//        //std::wstringstream wstrm;
//        //wstrm << L"센드 실패" << std::endl;
//        //StateLog( wstrm.str().c_str() );
//		return false;
//	}
//	else
//	{
//        //StateLog( L"센드 함 : " << (int)ID );
//        m_dwSendCount++;
//        //StateLog( m_dwSendCount );
//        m_dwSendByte += size;
//
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//		//혼자라면 측정 안함
//		if( GetPeerMap().size() > 1 )
//			KOGGamePerformanceCheck::GetInstance()->GetUDPTransCheckMgr()->UpdateSend( size );
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//
//#ifdef UDP_PACKET_ANALYSIS_LOG
//		if( !m_bUdpPacketAnalysisLog )
//		{
//			if( !m_mapSendAnalysisResult.empty() )
//			{
//				FILE* pUdpSendPacketAnalysisFile = fopen( "Send_UdpPacketAnalysisLog.txt", "w+");
//
//				std::map< int, CKTDNUDP::KAnalysisResult >::const_iterator it;
//				std::map< int, CKTDNUDP::KAnalysisResult >::const_iterator iend = m_mapSendAnalysisResult.end();
//				for( it = m_mapSendAnalysisResult.begin(); it != iend; ++it )
//					fprintf( pUdpSendPacketAnalysisFile, "ID: %d, TransCountPerSec: %f TransSizePerSec: %f" /* " UnCompressTransSizePerSec: %f" */ "\n"
//                    , it->first, (float)it->second.m_dwTransCount/m_fSendAnalysisTime, (float)it->second.m_dwTransSize/m_fSendAnalysisTime/*, (float)it->second.m_dwUnCompressTransSize/m_fSendAnalysisTime */ );
//
//				fclose( pUdpSendPacketAnalysisFile );
//
//				//
//				m_mapSendAnalysisResult.clear();
//			}
//
//			m_fSendAnalysisTime = 0.0f;
//		}
//		else
//		{
//			std::map< int, KAnalysisResult >::iterator it = m_mapSendAnalysisResult.find( ID );
//			if( it == m_mapSendAnalysisResult.end() )
//			{
//				KAnalysisResult kAnalysisResult;
//				kAnalysisResult.m_dwTransCount = 1;
//				//if( bCompress )
//					kAnalysisResult.m_dwTransSize = size;
//				//else
//				//	kAnalysisResult.m_dwUnCompressTransSize = size;
//				m_mapSendAnalysisResult.insert( std::make_pair( ID, kAnalysisResult ) );
//			}
//			else
//			{
//				it->second.m_dwTransCount++;
//				//if( bCompress )
//					it->second.m_dwTransSize += size;
//				//else
//				//	it->second.m_dwUnCompressTransSize += size;				
//			}
//		}
//#endif//UDP_PACKET_ANALYSIS_LOG
//	}
//	
//	return true;
//}
//
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
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
bool CKTDNUDP::SendToPeer( __int64 UID, const char ID, const void* pBuffer, int size )
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//bool CKTDNUDP::Send( __int64 UID, const char ID, const char* pBuffer, int size )
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			
{
	KTDXPROFILE();

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
    if( size < 0 || size > 0 && pBuffer == NULL )
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//    if( !pBuffer )
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			
    {
        return false;
    }

	if( UID == m_MyUID )
	{
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		SendToMe( ID, pBuffer, size );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		return true;
	}

	const Peer* pPeer = GetPeer( UID );
	if( pPeer == NULL )
		return false;

	if( pPeer->m_bUseRelay == true && ConnectRelayTestResult() == true )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        std::vector<__int64> vecUID;
        vecUID.push_back( UID );
        return  SendPure_Relay( SP_RELAY, vecUID, ID, pBuffer, size );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	    unsigned long	compressSize			= 2000;
//	    char			pCompressBuffer[2000]	= {0,};
//	    if( !Compress( ID, pCompressBuffer, compressSize, pBuffer, size ) )
//	    {
//		    return false;
//	    }
//	    char	pFinalBuffer[3000]	= {0,};
//	    int		finalBufSize = 0;
//		vector<__int64> UIDList;
//		UIDList.push_back( pPeer->m_UID );
//		return SendRelay( ID, pBuffer, size, UIDList );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}

    if ( 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        pPeer->m_eP2PConnectType != P2PCONNECT_NONE
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        pPeer->m_bP2PConnected == true 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        && m_eForceConnectMode != FORCE_CONNECT_MODE_RELAY )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		return SendToIP( pPeer->m_IPAddress, pPeer->m_Port, ID, pBuffer, size );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		return Send( pPeer->m_IP.c_str(), pPeer->m_Port, ID, pBuffer, size );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}

	return false;
}

//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
void CKTDNUDP::AddPeer( __int64 UID, u_long  ulIP, int port, u_long  ulInternalIP, int iInternalPort )
//#else SERV_KTDX_RETRY_USING_INTERNAL_IP
//void CKTDNUDP::AddPeer( __int64 UID, u_long  ulIP, int port )
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//#else  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
////#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//void CKTDNUDP::AddPeer( __int64 UID, const WCHAR* pIP, int port, const WCHAR* pInternalIP, int iInternalPort )
////#else
////void CKTDNUDP::AddPeer( __int64 UID, const WCHAR* pIP, int port )
////#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//}}
{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
	Peer* pPeer = _GetPeer( UID );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	Peer* pPeer = GetPeer( UID );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			
	if( pPeer != NULL )
	{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        pPeer->m_IPAddress		= ulIP;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		pPeer->m_IP		= pIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		pPeer->m_Port	= port;
		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        if ( ulInternalIP != ADDR_ANY )
		{		
            pPeer->m_InternalIPAddress = ulInternalIP;
			pPeer->m_InternalPort = iInternalPort;
		}
        else
        {
            pPeer->m_InternalIPAddress = INADDR_ANY;
			pPeer->m_InternalPort = 0;
        }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		if( pInternalIP != NULL )
//		{		
//			pPeer->m_InternalIP = pInternalIP;
//			pPeer->m_InternalPort = iInternalPort;
//		}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

        if ( m_eForceConnectMode == FORCE_CONNECT_MODE_P2P )
        {
            _SetUseRelay( pPeer, false );
        }
        else if ( pPeer->m_eP2PConnectType == P2PCONNECT_NONE
            || m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
        {
            _SetUseRelay( pPeer, true );
        }//if.. else..
#else   SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

        if ( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
        {
            _SetUseRelay( pPeer, true );
        }//if

#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}
	else
	{
		Peer peer;
		peer.m_UID	= UID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        peer.m_IPAddress	= ulIP;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		peer.m_IP	= pIP;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		peer.m_Port	= port;
		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        if ( ulInternalIP != ADDR_ANY )
		{		
            peer.m_InternalIPAddress = ulInternalIP;
			peer.m_InternalPort = iInternalPort;
		}
        else
        {
            peer.m_InternalIPAddress = INADDR_ANY;
			peer.m_InternalPort = 0;
        }
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		if( pInternalIP != NULL )
//		{		
//			peer.m_InternalIP = pInternalIP;
//			peer.m_InternalPort = iInternalPort;
//		}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

        if ( m_eForceConnectMode != FORCE_CONNECT_MODE_P2P )
        {
            _SetUseRelay( pPeer, true );
        }
        else
        {
            _SetNonRelayUID( UID );
        }

#else   SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

        if ( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
        {
            _SetUseRelay( pPeer, true );
        }
        else
        {
            _SetNonRelayUID( UID );
        }

#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

		//}}
		m_PeerMap.insert( std::make_pair(peer.m_UID,peer) );
	}
}

bool CKTDNUDP::RemovePeer( __int64 UID )
{
	map<__int64,Peer>::iterator iter;
	iter = m_PeerMap.find( UID );
	if( iter == m_PeerMap.end() )
		return false;

	m_PeerMap.erase( iter );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    _RemoveRelayUID( UID );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	return true;
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
const CKTDNUDP::Peer* CKTDNUDP::GetPeer( __int64 UID )
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
//CKTDNUDP::Peer* CKTDNUDP::GetPeer( __int64 UID )
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK	
{
	map<__int64,Peer>::iterator iter;
	iter = m_PeerMap.find( UID );
	if( iter == m_PeerMap.end() )
		return NULL;
	
	Peer* pPeer = &iter->second;
	pPeer->m_SleepTime = 0.0f;

	return pPeer;
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


bool     CKTDNUDP::ForceUseRelay( __int64 UID )
{
    if ( m_eForceConnectMode == FORCE_CONNECT_MODE_P2P )
        return false;

    Peer* pPeer = _GetPeer( UID );
    if ( pPeer == NULL )
        return false;
    _SetUseRelay( pPeer, true );
    pPeer->m_ConnectTestCount = KTDX_MAX_P2P_CONNECT_TEST_COUNT;
    return true;
}
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			

const CKTDNUDP::Peer* CKTDNUDP::GetMyPeer()
{
	return GetPeer( m_MyUID );
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK			
//bool    CKTDNUDP::ForcePeerUseRelay( __int64 UID )
//{
////#ifndef X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
//    if ( UID == m_MyUID )
//        return false;
////#endif  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
//    Peer* pPeer = _GetPeer( UID );
//    if ( pPeer == NULL )
//        return false;
//    if ( pPeer->m_bUseRelay == true )
//        return true;
//
//    _SetUseRelay( pPeer, true );
//    return true;
//}
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK		

void CKTDNUDP::DefencePort()
{
	const Peer* pPeer = GetMyPeer();
	if( pPeer != NULL )
	{
		SSP_DEFENCE_PORT sSP_DEFENCE_PORT;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		SendToIP( pPeer->m_IPAddress, GetMyExtPort(), SP_DEFENCE_PORT, (const char*)&sSP_DEFENCE_PORT, sizeof(SSP_DEFENCE_PORT) );
//#else  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		Send( pPeer->m_IP.c_str(), GetMyExtPort(), SP_DEFENCE_PORT, (const char*)&sSP_DEFENCE_PORT, sizeof(SSP_DEFENCE_PORT) );
//        if ( ConnectRelayTestResult() == true )
//        {
//		    Send( m_RelayIP.c_str(), m_RelayPort, SP_DEFENCE_PORT, (const char*)&sSP_DEFENCE_PORT, sizeof(SSP_DEFENCE_PORT) );
//        }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}
}



//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

void    CKTDNUDP::ResetConnectTestToPeersAll()
{
	for( map<__int64,Peer>::iterator iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;

        switch( m_eForceConnectMode )
        {
        case FORCE_CONNECT_MODE_RELAY:
            _SetUseRelay( pPeer, true );
            break;
        case FORCE_CONNECT_MODE_P2P:
            _SetUseRelay( pPeer, false );
            pPeer->m_eP2PConnectType = P2PCONNECT_NONE;
            pPeer->m_ConnectTestCount = 0;
            break;
        default:
            _SetUseRelay( pPeer, true );
            pPeer->m_eP2PConnectType = P2PCONNECT_NONE;
            pPeer->m_ConnectTestCount = 0;
            break;
        }//switch
    }
    if ( m_eForceConnectMode != FORCE_CONNECT_MODE_RELAY )
        ConnectTestToPeer();
}

void    CKTDNUDP::ResetConnectTestToPeer( __int64 UID )
{
	Peer* pPeer = _GetPeer( UID );

    if ( pPeer == NULL )
        return;

    switch( m_eForceConnectMode )
    {
    case FORCE_CONNECT_MODE_RELAY:
        _SetUseRelay( pPeer, true );
        return;
    case FORCE_CONNECT_MODE_P2P:
        _SetUseRelay( pPeer, false );
        pPeer->m_eP2PConnectType = P2PCONNECT_NONE;
        pPeer->m_ConnectTestCount = 0;
        break;
    default:
        _SetUseRelay( pPeer, true );
        pPeer->m_eP2PConnectType = P2PCONNECT_NONE;
        pPeer->m_ConnectTestCount = 0;
        break;
    }//switch

    ConnectTestToPeer();
}

bool CKTDNUDP::_RobustConnectTestResult()
{
	if( m_PeerMap.size() < 1 )
		return false;

	map<__int64,Peer>::iterator iter;
    bool    bRequireRelayConnection = false;

	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;

        if ( pPeer->m_bUseRelay == false
            && ( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY
                 || m_eForceConnectMode != FORCE_CONNECT_MODE_P2P && pPeer->m_eP2PConnectType == P2PCONNECT_NONE ) )
        {
            _SetUseRelay( pPeer, true );
        }

        if ( pPeer->m_bUseRelay == true )
        {
            switch( m_eForceConnectMode )
            {
            case FORCE_CONNECT_MODE_RELAY:
                bRequireRelayConnection = true;
                continue;
            case FORCE_CONNECT_MODE_P2P:
                _SetUseRelay( pPeer, false );
                break;
            default:
                if ( pPeer->m_eP2PConnectType != P2PCONNECT_NONE )
                    _SetUseRelay( pPeer, false );
                else
                    bRequireRelayConnection = true;
                break;
            }//switch
        }

        switch( pPeer->m_eP2PConnectType )
        {
        case P2PCONNECT_EXTERNAL:
            {
                if ( pPeer->m_InternalIPAddress != ADDR_ANY
                    && false == ( pPeer->m_InternalIPAddress == pPeer->m_IPAddress && pPeer->m_InternalPort == pPeer->m_Port )
                    && pPeer->m_ConnectTestCount < KTDX_MAX_P2P_INTERNAL_CONNECT_TEST_COUNT )
                    return false;
            }
            break;
        case P2PCONNECT_INTERNAL:
            {
                continue;
            }
            break;
        default: // P2PCONNECT_NONE
            {
                if ( pPeer->m_bUseRelay == true && pPeer->m_ConnectTestCount >= KTDX_MAX_P2P_CONNECT_TEST_COUNT )
                    continue;
                return false;
            }
            break;
        }//switch
	}

    if ( bRequireRelayConnection == true && m_eRelayState != RELAY_STATE_CONNECTED )
        return false;

	return true;
}

bool CKTDNUDP::ConnectTestResult()
{
	if( m_PeerMap.size() < 1 )
		return false;

	map<__int64,Peer>::iterator iter;
    bool    bRequireRelayConnection = false;
	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;
        if ( pPeer->m_bUseRelay == false
            && ( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY
                 || m_eForceConnectMode != FORCE_CONNECT_MODE_P2P && pPeer->m_eP2PConnectType == P2PCONNECT_NONE ) )
        {
            _SetUseRelay( pPeer, true );
        }

        if ( pPeer->m_bUseRelay == true )
        {
            switch( m_eForceConnectMode )
            {
            case FORCE_CONNECT_MODE_RELAY:
                bRequireRelayConnection = true;
                continue;
            case FORCE_CONNECT_MODE_P2P:
                _SetUseRelay( pPeer, false );
                break;
            default:
                if ( pPeer->m_eP2PConnectType != P2PCONNECT_NONE )
                    _SetUseRelay( pPeer, false );
                else
                    bRequireRelayConnection = true;
                break;
            }//switch
        }
        if ( pPeer->m_bUseRelay == false && pPeer->m_eP2PConnectType == P2PCONNECT_NONE )
        {
            return false;
        }
	}

    if ( bRequireRelayConnection == true && m_eRelayState != RELAY_STATE_CONNECTED )
        return false;

	return true;
}



void CKTDNUDP::ConnectTestToPeer()
{
	m_fConnectTestTime = 0.5f;
	map<__int64,Peer>::iterator iter;

    //const Peer* pMyPeer = GetMyPeer();

	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;

        switch( m_eForceConnectMode )
        {
        case FORCE_CONNECT_MODE_P2P:
            if ( pPeer->m_bUseRelay == true )
                _SetUseRelay( pPeer, false );
            break;
        case FORCE_CONNECT_MODE_RELAY:
            if ( pPeer->m_bUseRelay == false )
                _SetUseRelay( pPeer, true );
            continue;
        default:
            if ( pPeer->m_bUseRelay == false && pPeer->m_eP2PConnectType == P2PCONNECT_NONE )
                _SetUseRelay( pPeer, true );
            if ( pPeer->m_ConnectTestCount >= KTDX_MAX_P2P_CONNECT_TEST_COUNT )
                continue;
            break;
        }//switch

        if ( pPeer->m_eP2PConnectType == P2PCONNECT_INTERNAL
            || 
            pPeer->m_eP2PConnectType == P2PCONNECT_EXTERNAL
            && ( pPeer->m_InternalIPAddress == ADDR_ANY
            || pPeer->m_InternalIPAddress == pPeer->m_IPAddress && pPeer->m_InternalPort == pPeer->m_Port
            || pPeer->m_ConnectTestCount >= KTDX_MAX_P2P_INTERNAL_CONNECT_TEST_COUNT ) )
            continue;

        //if ( pMyPeer != NULL )
        //{
		SSP_CONNECT_TEST_REQ sSP_CONNECT_TEST_REQ;
		sSP_CONNECT_TEST_REQ.m_UID			= m_MyUID;
        if ( pPeer->m_UID == m_MyUID )
        {
            if ( pPeer->m_eP2PConnectType == P2PCONNECT_EXTERNAL )
            {
                pPeer->m_eP2PConnectType = P2PCONNECT_INTERNAL;
            }
            else
            {
                pPeer->m_ConnectTestCount++;
                sSP_CONNECT_TEST_REQ.m_bInternalIP = true;
			    SendToIP( m_MyIPAddress, m_MyPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
            }
        }
        else
        {
            pPeer->m_ConnectTestCount++;
            if ( pPeer->m_eP2PConnectType == P2PCONNECT_EXTERNAL
                || 
                pPeer->m_InternalIPAddress != INADDR_ANY 
                && false == ( pPeer->m_InternalIPAddress == pPeer->m_IPAddress && pPeer->m_InternalPort == pPeer->m_Port )
                && ( pPeer->m_ConnectTestCount & 1 ) == 0 )
            {
                sSP_CONNECT_TEST_REQ.m_bInternalIP = true;
				SendToIP( pPeer->m_InternalIPAddress, pPeer->m_InternalPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
            }
            else
            {
                sSP_CONNECT_TEST_REQ.m_bInternalIP = false;
				SendToIP( pPeer->m_IPAddress, pPeer->m_Port, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
            }
        }
        //}
    }
}



#else   SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

void CKTDNUDP::SendConnectReq( const Peer* pOtherPeer_ )
{
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//치트키용
	if( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
		return;
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	const Peer* pMyPeer = GetMyPeer();
	if( pMyPeer != NULL )
	{
		SSP_CONNECT_TEST_REQ sSP_CONNECT_TEST_REQ;
		sSP_CONNECT_TEST_REQ.m_UID			= pMyPeer->m_UID;
		
		if( pMyPeer == pOtherPeer_ )
        {
            sSP_CONNECT_TEST_REQ.m_bInternalIP = true;
			SendToIP( 
            m_MyIPAddress, 
            m_MyPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
        }
		else
        {
            sSP_CONNECT_TEST_REQ.m_bInternalIP = false;
			SendToIP( 
            pOtherPeer_->m_IPAddress,
            pOtherPeer_->m_Port, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
        }
	}
}

bool CKTDNUDP::ConnectTestResult()
{
	if( m_PeerMap.size() < 1 )
		return false;

	map<__int64,Peer>::iterator iter;
	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;
        bool    bP2PConnected = pPeer->m_eP2PConnectType != P2PCONNECT_NONE;

		if( bP2PConnected == false 
            && pPeer->m_bUseRelay == false )
		{
			return false;
		}
	}

	return true;
}



void CKTDNUDP::ConnectTestToPeer()
{
	m_fConnectTestTime = 0.5f;
	map<__int64,Peer>::iterator iter;
	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;
        if ( pPeer->m_eP2PConnectType != P2PCONNECT_NONE )
            continue;
		if( pPeer->m_bUseRelay == false )
		{
            bool    bUseRelay = false;
            if ( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
                bUseRelay = true;
            else if ( pPeer->m_ConnectTestCount < 5 )
            {
                pPeer->m_ConnectTestCount++;
            }
            if ( pPeer->m_ConnectTestCount >= 5 )
                bUseRelay = true;
			if( bUseRelay == true )
			{
                _SetUseRelay( pPeer, true );
			}
			else
			{
				//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
				const Peer* pMyPeer = GetMyPeer();
				if( pMyPeer != NULL )
				{
					SSP_CONNECT_TEST_REQ sSP_CONNECT_TEST_REQ;
					sSP_CONNECT_TEST_REQ.m_UID			= pMyPeer->m_UID;
					if( pMyPeer == pPeer )
					{
                        sSP_CONNECT_TEST_REQ.m_bInternalIP = true;
						SendToIP( m_MyIPAddress, m_MyPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
					}
					else
					{
						if( pPeer->m_ConnectTestCount >= 3 
                            && pPeer->m_InternalIPAddress != INADDR_ANY
                            )
						{
							sSP_CONNECT_TEST_REQ.m_bInternalIP = true;
							SendToIP( pPeer->m_InternalIPAddress, pPeer->m_InternalPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
						}
						else
						{
                            sSP_CONNECT_TEST_REQ.m_bInternalIP = false;
							SendToIP( pPeer->m_IPAddress, pPeer->m_Port, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
						}
					}
				}
			}
		}
		else	/// 연결은 되었는데 릴레이인 경우
		{
            if( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
                continue;
			++pPeer->m_ConnectTestCount;
			if ( pPeer->m_ConnectTestCount < KTDX_MAX_P2P_CONNECT_TEST_COUNT )	/// 40번 까지는 p2p 연결을 시도해 보자
			{
				SendConnectReq( pPeer );
				//{{ 2013. 1. 16	박세훈	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
				ReseveInternalConnect( pPeer->m_UID );
			}
		}
	}
}

#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//void CKTDNUDP::SendConnectReq( const Peer* pOtherPeer_ )
//{
////#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//	//치트키용
//	if( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
//		return;
////#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
//
//	const Peer* pMyPeer = GetMyPeer();
//	if( pMyPeer != NULL )
//	{
//		SSP_CONNECT_TEST_REQ sSP_CONNECT_TEST_REQ;
//		sSP_CONNECT_TEST_REQ.m_UID			= pMyPeer->m_UID;
//		sSP_CONNECT_TEST_REQ.m_SendTime		= timeGetTime();
//		
//		if( pMyPeer == pOtherPeer_ )
//			SendToIP( 
//            m_MyIP.c_str(), 
//            m_MyPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
//		else
//			SendToIP( 
//            pOtherPeer_->m_IP.c_str(), 
//            pOtherPeer_->m_Port, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
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
//        bool    bP2PConnected = pPeer->m_bP2PConnected;
//
//		if( bP2PConnected == false 
//            && pPeer->m_bUseRelay == false )
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//
//
//void CKTDNUDP::ConnectTestToPeer()
//{
//	m_fConnectTestTime = 0.5f;
//	map<__int64,Peer>::iterator iter;
//	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
//	{
//		Peer* pPeer = &iter->second;
//        if ( pPeer->m_bP2PConnected == true )
//            continue;
//		if( pPeer->m_bUseRelay == false )
//		{
//            bool    bUseRelay = false;
//            if ( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
//                bUseRelay = true;
//            else if ( pPeer->m_ConnectTestCount < 5 )
//            {
//                pPeer->m_ConnectTestCount++;
//            }
//            if ( pPeer->m_ConnectTestCount >= 5 )
//                bUseRelay = true;
//			if( bUseRelay == true )
//			{
//				pPeer->m_bUseRelay		= true;
//			}
//			else
//			{
//				const Peer* pMyPeer = GetMyPeer();
//				if( pMyPeer != NULL )
//				{
//					SSP_CONNECT_TEST_REQ sSP_CONNECT_TEST_REQ;
//					sSP_CONNECT_TEST_REQ.m_UID			= pMyPeer->m_UID;
//					sSP_CONNECT_TEST_REQ.m_SendTime		= timeGetTime();
//					if( pMyPeer == pPeer )
//					{
//						Send( m_MyIP.c_str(), m_MyPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
//					}
//					else
//					{
//						if( pPeer->m_ConnectTestCount >= 3 
//                            && pPeer->m_InternalIP.empty() == false 
//                            )
//						{
//							//pPeer->m_bUseInternalIP = true;
//							Send( pPeer->m_InternalIP.c_str(), pPeer->m_InternalPort, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
//							std::wstringstream wstrstm;
//							wstrstm << L"External : " << pPeer->m_IP << L":" << pPeer->m_Port << std::endl << L"Internal : " << pPeer->m_InternalIP << L":" << pPeer->m_InternalPort;
//						}
//						else
//						{
//							Send( pPeer->m_IP.c_str(), pPeer->m_Port, SP_CONNECT_TEST_REQ, (const char*)&sSP_CONNECT_TEST_REQ, sizeof(SSP_CONNECT_TEST_REQ) );
//						}
//					}
//				}
//			}
//		}
//		else	/// 연결은 되었는데 릴레이인 경우
//		{
//            if( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
//                continue;
//			++pPeer->m_ConnectTestCount;
//			if ( pPeer->m_ConnectTestCount < KTDX_MAX_P2P_CONNECT_TEST_COUNT )	/// 40번 까지는 p2p 연결을 시도해 보자
//			{
//				SendConnectReq( pPeer );
//				//{{ 2013. 1. 16	박세훈	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//				ReseveInternalConnect( pPeer->m_UID );
//				//}}
//			}
//		}
//	}
//}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



void CKTDNUDP::ConnectTestToRelay()
{
//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
//	m_fConnectRelayTime = 0.5f;
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	//치트키용
	//if( m_eForceConnectMode == FORCE_CONNECT_MODE_P2P )
	//	return;
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_eRelayState = RELAY_STATE_TRYING_TO_CONNECT;
    m_dwRelayUIDStampFromServer = 0;
    m_fRelayTimer = 0.f;
    m_iRelayTimerCounter = 0;
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	m_fConnectRelayTime = 0.5f;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK\

#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    m_kRelayStatistic.Init();
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    SendPure_Relay( SP_CONNECT_RELAY_REQ, std::vector<__int64>(), 0, NULL, 0 );
}




//#ifdef UDP_DOWNLOAD_BLOCK_CHECK
///** @function : ConnectTestToCheck
//	@brief	: 랜선렉 악용 유저 신고 기능 전 UDP 릴레이 체크 기능
//*/
//void CKTDNUDP::ConnectTestToCheck()
//{
//	m_fConnectCheckTime = 2.f;
//	m_bConnectCheck = false;
//	m_bReceiveCheckResult = false;
//	SSP_CONNECT_RELAY_REQ sSP_CONNECT_CHECK_REQ;
//	sSP_CONNECT_CHECK_REQ.m_UID = m_MyUID;
//	SendToIP( 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        m_RelayIPAddress,
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        m_RelayIP.c_str(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        m_RelayPort, SP_CONNECT_CHECK_REQ, (const char*)&sSP_CONNECT_CHECK_REQ, sizeof(SSP_CONNECT_RELAY_REQ) );
//}
//#endif //UDP_DOWNLOAD_BLOCK_CHECK

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void CKTDNUDP::Relay( int recvSize )
//{
//	char packetType			= m_pRecvBuffer[1];
//	char UIDNum				= m_pRecvBuffer[2];
//	int relayDataSize		= recvSize - 2 - (UIDNum * sizeof(__int64));
//	char* pRelayDataOrg		= m_pRecvBuffer + (3 * sizeof(char)) + (UIDNum * sizeof(__int64));
//	char* pRelayData		= new char[relayDataSize];	
//	ZeroMemory( pRelayData, sizeof(char) * relayDataSize );
//	
//	pRelayData[0]		= packetType;
//	memcpy( &pRelayData[1], pRelayDataOrg, sizeof(char) * (relayDataSize-1) );
//	
//	char* pBufferIndex = m_pRecvBuffer + (3 * sizeof(char));
//
//	for( int i = 0; i < UIDNum; i++ )
//	{
//		__int64 UID;
//		memcpy( &UID, pBufferIndex, sizeof(__int64) );
//		pBufferIndex += sizeof(__int64);
//
//		Peer* pPeer = GetPeer( UID );
//		if( pPeer != NULL )
//		{
//			string mbIP;
//			ConvertWCHARToChar( mbIP, pPeer->m_IP.c_str() );
//			inet_addr( mbIP.c_str() );
//
//			//목적지 설정
//			SOCKADDR_IN	destination;
//			destination.sin_family		= AF_INET;
//			destination.sin_addr.s_addr	= inet_addr( mbIP.c_str() );
//			destination.sin_port		= htons( pPeer->m_Port );  //포트번호
//
//			int ret = sendto( m_Socket, pRelayData, relayDataSize, 0, (SOCKADDR *)&destination, sizeof(SOCKADDR_IN) );
//		}
//	}
//
//    SAFE_DELETE_ARRAY( pRelayData );
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void CKTDNUDP::CreateSocket()
{
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	string IP;
//	ConvertWCHARToChar( IP, m_MyIP.c_str() );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	m_Socket		= socket( AF_INET, SOCK_DGRAM, IPPROTO_IP );
	int sndBuf		= 65534;
	int rcvBuf		= 65534;
	setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBuf, sizeof(sndBuf) );
	setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, sizeof(rcvBuf) );

	m_LocalAddr.sin_family		= AF_INET;
	m_LocalAddr.sin_port		= htons( m_MyPort );
	//m_LocalAddr.sin_addr.s_addr = INADDR_ANY;
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
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
		m_LocalAddr.sin_addr.s_addr = INADDR_ANY;
		//m_LocalAddr.sin_addr.s_addr	= inet_addr( IP.c_str() );
		ret							= bind( m_Socket, (SOCKADDR*)&m_LocalAddr, sizeof(m_LocalAddr) );
	}
	m_RecvEvent = WSACreateEvent();
	if( m_RecvEvent == WSA_INVALID_EVENT )
	{
		int retVal = WSAGetLastError();
		switch( retVal )
		{
		case WSANOTINITIALISED:
			{
				ErrorLogMsg( KEM_ERROR116, L"WSANOTINITIALISED" );
			}
			break;

		case WSAENETDOWN:
			{
				ErrorLogMsg( KEM_ERROR116, L"WSAENETDOWN" );
			}
			break;

		case WSAEINPROGRESS:
			{
				ErrorLogMsg( KEM_ERROR116, L"WSAEINPROGRESS" );
			}
			break;

		case WSA_NOT_ENOUGH_MEMORY:
			{
				ErrorLogMsg( KEM_ERROR116, L"WSA_NOT_ENOUGH_MEMORY" );
			}
			break;

		default:
			{
				ErrorLogMsg( KEM_ERROR116, L"UNKNOWN" );
			}
			break;
		}
	}

	int retval = ::WSAEventSelect( m_Socket, m_RecvEvent, FD_READ | FD_CLOSE );
	if( retval != 0 )
	{
		int retVal = WSAGetLastError();
		switch( retVal )
		{
		case WSANOTINITIALISED:
			{
				ErrorLogMsg( KEM_ERROR117, L"WSANOTINITIALISED" );
			}
			break;

		case WSAENETDOWN:
			{
				ErrorLogMsg( KEM_ERROR117, L"WSAENETDOWN" );
			}
			break;

		case WSAEINVAL:
			{
				ErrorLogMsg( KEM_ERROR117, L"WSAEINVAL" );
			}
			break;

		case WSAEINPROGRESS:
			{
				ErrorLogMsg( KEM_ERROR117, L"WSAEINPROGRESS" );
			}
			break;

		case WSAENOTSOCK:
			{
				ErrorLogMsg( KEM_ERROR117, L"WSAENOTSOCK" );
			}
			break;

		default:
			{
				ErrorLogMsg( KEM_ERROR117, L"UNKNOWN" );
			}
			break;
		}
	}
}

void CKTDNUDP::DeleteSocket()
{
	if( m_Socket != INVALID_SOCKET )
	{
		shutdown( m_Socket, SD_BOTH );
		closesocket( m_Socket );
		m_Socket = INVALID_SOCKET;
	}

	WSACloseEvent( m_RecvEvent );
}

void CKTDNUDP::RemoveOtherPeer( std::vector<__int64> UIDList )
{
	std::vector<__int64> delUIDList;

	map<__int64,Peer>::iterator iter;
	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;
		if( pPeer != NULL )
		{
			bool bExist = false;
			for( int i = 0; i < (int)UIDList.size(); i++ )
			{				
				if( pPeer->m_UID == UIDList[i] )
				{
					bExist = true;
					break;
				}
			}
			if( bExist == false )
			{
				//지우자
				delUIDList.push_back( pPeer->m_UID );
			}
		}
	}

	for( int i = 0; i < (int)delUIDList.size(); i++ )
	{
		RemovePeer( delUIDList[i] );
	}
}

//#ifndef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void CKTDNUDP::SendToMe( const char ID, const char* pBuffer, int size )
//{
//	RecvData* pRecvData		= new RecvData;
//	pRecvData->m_SenderIP	= GetMyIP();
//	pRecvData->m_SenderPort	= GetMyPort();
//	pRecvData->m_ID			= ID;
//	pRecvData->m_Size		= size;
//	pRecvData->SetData( (char*)pBuffer, size );
//
//	//if( m_fLocalWaitTime > 0.0f )
//	//{
//	//	pRecvData->m_fLocalWaitTime = m_fLocalWaitTime;
//	//	m_RecvDataWaitList.push_back( pRecvData );
//	//}
//	//else
//		m_RecvDataList.push_back( pRecvData );
//}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//{{ 2013. 1. 16	박세훈	//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )

#ifndef SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
void CKTDNUDP::ReseveInternalConnect( const __int64 iUID )
{
	if( GetPeer( iUID ) == NULL )
	{
		return;
	}

	m_listResevedInternalConnect.push_back( std::make_pair( iUID, 0.5f ) );
}



void CKTDNUDP::CheckAndProcessInternalConnect( IN const float fElapsedTime )
{
	if( m_listResevedInternalConnect.empty() == true )
	{
		return;
	}

	std::list< std::pair<__int64, float> >::iterator it = m_listResevedInternalConnect.begin();
	while( it != m_listResevedInternalConnect.end() )
	{
		it->second -= fElapsedTime;
		if( 0.0f < it->second )
		{
			++it;
			continue;
		}
		
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		Peer* pPeer = _GetPeer( it->first );
//#else 	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		Peer* pPeer = GetPeer( it->first );
//#endif	SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK					
		if( pPeer != NULL )
		{
			// P2P로 연결되어 있다면 수행할 필요가 없다.
			if(	pPeer->m_bP2PConnected == true )
            {
			    it = m_listResevedInternalConnect.erase( it );
                continue;
            }

            if ( pPeer->m_bUseRelay == true
                && m_eForceConnectMode != FORCE_CONNECT_MODE_RELAY )
			{
				++pPeer->m_ConnectTestCount;
				if( pPeer->m_ConnectTestCount < KTDX_MAX_P2P_CONNECT_TEST_COUNT )
				{
//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
					//치트키용
					//if( m_eForceConnectMode == FORCE_CONNECT_MODE_P2P )
					//{
					//	it = m_listResevedInternalConnect.erase( it );
					//	continue;
					//}
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

					//SSP_CONNECT_RELAY_REQ sSP_REQ;
					//sSP_REQ.m_UID = m_MyUID;
                    SSP_RETRY_INTERNAL_CONNECT_REQ sSP_REQ;
                    sSP_REQ.m_UID = m_MyUID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					SendToIP( pPeer->m_InternalIPAddress, pPeer->m_InternalPort, SP_RETRY_INTERNAL_CONNECT_REQ, (const char*)&sSP_REQ, sizeof( SSP_RETRY_INTERNAL_CONNECT_REQ ) );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//					Send( pPeer->m_InternalIP.c_str(), pPeer->m_InternalPort, SP_RETRY_INTERNAL_CONNECT_REQ, (const char*)&sSP_CONNECT_RELAY_REQ, sizeof( SSP_CONNECT_RELAY_REQ ) );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				}
			}
		}

		it = m_listResevedInternalConnect.erase( it );
	}
}
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//}}
#endif  SERV_KTDX_OPTIMIZE_UDP_ROBUST_CONNECTION

//#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
void CKTDNUDP::SetForceConnectMode( EForceConnectMode eFCMode )
{ 
    if ( eFCMode < EForceConnectMode(0)
        || eFCMode >= FORCE_CONNECT_MODE_NUMS )
        return;

    if ( eFCMode == m_eForceConnectMode )
        return;


	//강제로 연결을 전환할 경우에는 다음과 같이 초기화...
	map<__int64,Peer>::iterator iter;
	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
	{
		Peer* pPeer = &iter->second;

        switch( eFCMode )
        {
        case FORCE_CONNECT_MODE_RELAY:
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            _SetUseRelay( pPeer, true );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		    pPeer->m_bUseRelay = true;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            break;
        case FORCE_CONNECT_MODE_P2P:
        default:
            if ( m_eForceConnectMode == FORCE_CONNECT_MODE_RELAY )
                pPeer->m_eP2PConnectType = P2PCONNECT_NONE;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            pPeer->m_ConnectTestCount = 0;
            //pPeer->m_bUseInternalIP = false;
            _SetUseRelay( pPeer, false );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//		    pPeer->m_bUseRelay = false;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            break;
        }
	}

    if ( eFCMode != FORCE_CONNECT_MODE_RELAY )
	    m_fConnectTestTime = 0.5f;
	//
	m_eForceConnectMode = eFCMode; 
}
//#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//void CKTDNUDP::ReceiveTcpRelay( const char ID, const void* pBuffer, int size )
//{
//	RecvData* pRecvData		= new RecvData;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//    pRecvData->m_SenderIPAddress	= GetMyIPAddress();
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	pRecvData->m_SenderIP	= GetMyIP();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	pRecvData->m_SenderPort	= GetMyPort();
//	pRecvData->m_ID			= ID;
//	pRecvData->m_Size		= size;
//	pRecvData->SetData( (char*)pBuffer, size );
//
//	//if( m_fLocalWaitTime > 0.0f )
//	//{
//	//	pRecvData->m_fLocalWaitTime = m_fLocalWaitTime;
//	//	m_RecvDataWaitList.push_back( pRecvData );
//	//}
//	//else
//	m_RecvDataList.push_back( pRecvData );
//}
//#endif//X2OPTIMIZE_TCP_RELAY_TEST


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


void    CKTDNUDP::SetMyUID( __int64 myUID )
{ 
    m_MyUID = myUID;
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifndef X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
    _RemoveRelayUID( myUID );
//#endif  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}


//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

int     CKTDNUDP::Estimate_RelayPacketSize( const char cRelayID, const std::vector<__int64>& vecUIDs, const char ID, const void* pBuffer, int size )
{
    int iSize = 0;
    switch( cRelayID )
    {
    case SP_CONNECT_RELAY_REQ:
        {
            iSize++;
            iSize+=sizeof(SSP_CONNECT_RELAY_REQ);
        }
        break;
    case SP_RELAY_SET_UID_LIST_REQ:
        {
            iSize++;
            iSize+=sizeof(SSP_RELAY_SET_UID_LIST_REQ);
            iSize+=vecUIDs.size() * sizeof(__int64);
        }
        break;
    case SP_RELAY_PRECONFIG:
        {
            iSize++;
            iSize+=sizeof(SSP_RELAY_PRECONFIG );
        }
        break;
    case SP_RELAY:
        {
            iSize++;
            iSize+=sizeof(SSP_RELAY);
            iSize+=vecUIDs.size() * sizeof(__int64);
        }
        break;
    default:
        return 0;
    }
    if ( ID != 0 )
    {
        iSize++;
        if ( pBuffer != NULL && size > 0 )
        {
            iSize += size;
        }//if
    }
    return  iSize;
}

bool    CKTDNUDP::SendPure_Relay( const char cRelayID, const std::vector<__int64>& vecUIDs, const char ID, const void* pBuffer, int size )
{
    WSABUF      alpBuffers[6];
    unsigned    uNumBuffers = 0;
    BYTE        abyBuffers[256];
    BYTE*       pbyBuffer = &abyBuffers[0];

#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    bool        bSendTimestamp = false;
    DWORD       dwTimestamp = 0;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY

    switch( cRelayID )
    {
    case SP_CONNECT_RELAY_REQ:
        {
            *pbyBuffer = SP_CONNECT_RELAY_REQ;
            pbyBuffer++;
            SSP_CONNECT_RELAY_REQ* pReq = (SSP_CONNECT_RELAY_REQ*) pbyBuffer;
            pReq->m_UID = m_MyUID;
            pReq->m_dwStamp = ( vecUIDs.empty() == true ) ? 0L : m_dwRelayUIDStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
            dwTimestamp = timeGetTime();
            pReq->m_dwTimestamp = dwTimestamp;
            bSendTimestamp = true;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
            pbyBuffer += sizeof(SSP_CONNECT_RELAY_REQ);
            alpBuffers[uNumBuffers].buf = (CHAR*) &abyBuffers[0];
            alpBuffers[uNumBuffers].len = pbyBuffer - &abyBuffers[0];
            uNumBuffers++;
             m_kRelayStatistic.AddPingSend( dwTimestamp );
        }
        break;
    case SP_RELAY_SET_UID_LIST_REQ:
        {
            *pbyBuffer = SP_RELAY_SET_UID_LIST_REQ;
            pbyBuffer++;
            SSP_RELAY_SET_UID_LIST_REQ* pReq = (SSP_RELAY_SET_UID_LIST_REQ*) pbyBuffer;
            pReq->m_UID = m_MyUID;
            pReq->m_dwStamp = ( vecUIDs.empty() == true ) ? 0L : m_dwRelayUIDStamp;
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
            dwTimestamp = timeGetTime();
            pReq->m_dwTimestamp = dwTimestamp;
            bSendTimestamp = true;
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
            pReq->m_byNumUIDs = vecUIDs.size();
            pbyBuffer += sizeof( SSP_RELAY_SET_UID_LIST_REQ );
            alpBuffers[uNumBuffers].buf = (CHAR*) &abyBuffers[0];
            alpBuffers[uNumBuffers].len = pbyBuffer - &abyBuffers[0];
            uNumBuffers++;
            alpBuffers[uNumBuffers].buf = (CHAR*) &vecUIDs.front();
            alpBuffers[uNumBuffers].len = vecUIDs.size() * sizeof(__int64);
            uNumBuffers++;
             m_kRelayStatistic.AddPingSend( dwTimestamp );
        }
        break;
    case SP_RELAY_PRECONFIG:
        {
            *pbyBuffer = SP_RELAY_PRECONFIG;
            pbyBuffer++;
            SSP_RELAY_PRECONFIG* pRelay = (SSP_RELAY_PRECONFIG*) pbyBuffer;
            pRelay->m_UID = m_MyUID;
            pbyBuffer += sizeof( SSP_RELAY_PRECONFIG );
            alpBuffers[uNumBuffers].buf = (CHAR*) &abyBuffers[0];
            alpBuffers[uNumBuffers].len = pbyBuffer - &abyBuffers[0];
            uNumBuffers++;
        }
        break;
    case SP_RELAY:
        {
            *pbyBuffer = SP_RELAY;
            pbyBuffer++;
            SSP_RELAY*  pRelay =  (SSP_RELAY*) pbyBuffer;
            pRelay->m_UID = m_MyUID;
            pRelay->m_byNumUIDs = vecUIDs.size();
            pbyBuffer += sizeof( SSP_RELAY );
            alpBuffers[uNumBuffers].buf = (CHAR*) &abyBuffers[0];
            alpBuffers[uNumBuffers].len = pbyBuffer - &abyBuffers[0];
            uNumBuffers++;
            alpBuffers[uNumBuffers].buf = (CHAR*) &vecUIDs.front();
            alpBuffers[uNumBuffers].len = vecUIDs.size() * sizeof(__int64);
            uNumBuffers++;
        }
        break;
    default:
        return false;
    }
    if ( ID != 0 )
    {
        *pbyBuffer = ID;
        alpBuffers[uNumBuffers].buf = (CHAR*) pbyBuffer;
        alpBuffers[uNumBuffers].len = 1;
        uNumBuffers++;
        pbyBuffer++;
        if ( pBuffer != NULL && size > 0 )
        {
            alpBuffers[uNumBuffers].buf = (CHAR*) pBuffer;
            alpBuffers[uNumBuffers].len = size;
            uNumBuffers++;
        }//if
    }
    bool bRetVal = SendPure_Scattered( m_RelayIPAddress, m_RelayPort, alpBuffers, uNumBuffers );
#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    if ( bRetVal == true && bSendTimestamp == true )
    {
        m_kRelayStatistic.AddPingSend( dwTimestamp );
    }//if
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
    return bRetVal;
}


bool    CKTDNUDP::BroadCast( const char ID, const void* pBuffer, int size )
{
    KTDXPROFILE();

    if( !pBuffer )
    {
        return false;
    }

	if ( m_vecNonRelayUIDs.empty() == true && m_vecRelayUIDs.empty() == true )
        return false;

	bool retval = true;

	for( int i = 0; i < (int)m_vecNonRelayUIDs.size(); i++ )
	{
		__int64 toUID = m_vecNonRelayUIDs[i];
//#ifndef X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
        if ( toUID == m_MyUID )
            continue;
//#endif  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
		const Peer* pPeer = GetPeer( toUID );
		if( pPeer != NULL && 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            pPeer->m_eP2PConnectType != P2PCONNECT_NONE
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//            pPeer->m_bP2PConnected == true
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            )
		{
			//StateLog( L"직접 센드" );
            WSABUF  alpBuffers[2];
            alpBuffers[0].buf = (CHAR*) &ID;
            alpBuffers[0].len = 1;
            alpBuffers[1].buf = (CHAR*) pBuffer;
            alpBuffers[1].len = size;
	        retval = SendPure_Scattered( pPeer->m_IPAddress, pPeer->m_Port, alpBuffers, (size>0) ? 2 : 1 );
		}
	}

	if( m_vecRelayUIDs.empty() == false )
	{
		if( ConnectRelayTestResult() == false )
		{
			ErrorLogMsg( KEM_ERROR111, "RelayServer Send Fail" );
		}
        else
        {
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            if ( GetRelayUIDsUpdated() == true )
            {
                if ( m_fRelayTimer >= 1.f )
                {
                    int iSizeEst = Estimate_RelayPacketSize( SP_CONNECT_RELAY_REQ, m_vecRelayUIDs, ID, pBuffer, size );
                    if ( iSizeEst <= CKTDNUDP_MTU_MAX )
                    {
                        m_fRelayTimer = 0.f;
                        m_iRelayTimerCounter = 0;
                        return SendPure_Relay( SP_CONNECT_RELAY_REQ, m_vecRelayUIDs, ID, pBuffer, size );
                    }
                }//if
                return  SendPure_Relay( SP_RELAY_PRECONFIG, m_vecRelayUIDs, ID, pBuffer, size );
            }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            if ( m_fRelayTimer >= 1.f )
            {
                int iSizeEst = Estimate_RelayPacketSize( SP_RELAY_SET_UID_LIST_REQ, m_vecRelayUIDs, ID, pBuffer, size );
                if ( iSizeEst <= CKTDNUDP_MTU_MAX )
                {
                    m_fRelayTimer = 0.f;
                    m_iRelayTimerCounter = 0;
                    return SendPure_Relay( SP_RELAY_SET_UID_LIST_REQ, m_vecRelayUIDs, ID, pBuffer, size );
                }
            }
            return  SendPure_Relay( SP_RELAY, m_vecRelayUIDs, ID, pBuffer, size );
        }
	}

    return  retval;
}

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//bool    CKTDNUDP::BroadCast( const char ID, const void* pBuffer, int size )
//{
//    KTDXPROFILE();
//
//    if( !pBuffer )
//    {
//        return false;
//    }
//
//	std::vector<__int64> toUIDList;
//
//	map<__int64,Peer>::iterator iter;
//	for( iter = m_PeerMap.begin(); iter != m_PeerMap.end(); iter++ )
//	{
//		Peer* pPeer = &iter->second;
//		if( pPeer->m_UID == m_MyUID )
//		{
//			SendToMe( ID, pBuffer, size );
//			if( m_PeerMap.size() <= 1 )
//				return true;
//		}
//		else
//			toUIDList.push_back( pPeer->m_UID );
//	}
//
//	return BroadCast( toUIDList, ID, pBuffer, size );
//}
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


bool CKTDNUDP::BroadCast( std::vector<__int64>& toUIDList, const char ID, const void* pBuffer, int size )
{
	KTDXPROFILE();

	if( toUIDList.empty() == true || size < 0 || size > 0 && pBuffer == NULL )
	{
		return false;
	}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	unsigned long	compressSize			= 2000;
//	char			pCompressBuffer[2000]	= {0,};
//    if( !Compress( ID, pCompressBuffer, compressSize, pBuffer, size ) )
//    {
//        return false;
//    }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	bool retval = true;
	vector<__int64> UIDList;			

	for( int i = 0; i < (int)toUIDList.size(); i++ )
	{
		__int64 toUID = toUIDList[i];

		if( toUID == m_MyUID )
		{
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//			SendToMe( ID, pBuffer, size );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		}
		else
		{
			const Peer* pPeer = GetPeer( toUID );
			if( pPeer != NULL )
			{
				if( pPeer->m_bUseRelay == true )
				{			
					UIDList.push_back( pPeer->m_UID );
				}
				else
				{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
					//StateLog( L"직접 센드" );
                    WSABUF  alpBuffers[2];
                    alpBuffers[0].buf = (CHAR*) &ID;
                    alpBuffers[0].len = 1;
                    alpBuffers[1].buf = (CHAR*) pBuffer;
                    alpBuffers[1].len = size;
	                retval = SendPure_Scattered( pPeer->m_IPAddress, pPeer->m_Port, alpBuffers, (size>0) ? 2 : 1 );
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//				    retval = SendPure( pPeer->m_IP.c_str(), pPeer->m_Port, ID, pCompressBuffer, compressSize );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
				}
			}
		}
	}

	if( UIDList.empty() == false )
	{
		if( ConnectRelayTestResult() == false )
		{
			ErrorLogMsg( KEM_ERROR111, "RelayServer Send Fail" );
            return false;
		}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

        retval = SendPure_Relay( SP_RELAY, UIDList, ID, pBuffer, size );

//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//		char	pFinalBuffer[3000]	= {0,};
//		int		finalBufSize = 0;
//
//		AddRelayHeader( ID, pFinalBuffer, finalBufSize, pDstBuffer + 1, ulDstSize - 1, UIDList );
//
//		//StateLog( L"릴레이 센드" );
//
//		retval = SendPure( m_RelayIP.c_str(), m_RelayPort, ID, pFinalBuffer, finalBufSize );
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	}

	return retval;
}

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

void   CKTDNUDP::_SetNonRelayUID( __int64 iUID )
{
//#ifndef X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
    if ( iUID != m_MyUID )
//#endif  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
    {
        std::vector<__int64>::iterator iter = std::lower_bound( m_vecNonRelayUIDs.begin(), m_vecNonRelayUIDs.end(), iUID );
        if ( iter == m_vecNonRelayUIDs.end() || *iter != iUID )
            m_vecNonRelayUIDs.insert( iter, iUID );
    }
}

void    CKTDNUDP::_SetUseRelay( Peer* pPeer, bool bOnOff )
{
    if ( pPeer == NULL )
        return;

    if ( pPeer->m_bUseRelay == true && bOnOff == false )
    {
        std::vector<__int64>::iterator iter = std::lower_bound( m_vecRelayUIDs.begin(), m_vecRelayUIDs.end(), pPeer->m_UID );
        if ( iter != m_vecRelayUIDs.end() && *iter == pPeer->m_UID )
        {
            m_vecRelayUIDs.erase( iter );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            if ( m_vecRelayUIDs.empty() == true )
            {
                m_dwRelayUIDStamp = 0;
                m_dwRelayUIDStampFromServer = 0;
            }
            else
            {
                m_dwRelayUIDStamp++;
                if ( m_dwRelayUIDStamp == 0 )
                    m_dwRelayUIDStamp = 1;
            }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        }
//#ifndef  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
        if ( pPeer->m_UID != m_MyUID )
//#endif  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
        {
            iter = std::lower_bound( m_vecNonRelayUIDs.begin(), m_vecNonRelayUIDs.end(), pPeer->m_UID );
            if ( iter == m_vecNonRelayUIDs.end() || *iter != pPeer->m_UID )
                m_vecNonRelayUIDs.insert( iter, pPeer->m_UID );
        }
        pPeer->m_bUseRelay = false;
    }
    else if ( pPeer->m_bUseRelay == false && bOnOff == true )
    {
        std::vector<__int64>::iterator iter = std::lower_bound( m_vecNonRelayUIDs.begin(), m_vecNonRelayUIDs.end(), pPeer->m_UID );
        if ( iter != m_vecNonRelayUIDs.end() && *iter == pPeer->m_UID )
            m_vecNonRelayUIDs.erase( iter );
//#ifndef  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
        if ( pPeer->m_UID != m_MyUID )
//#endif  X2OPTIMIZE_UDP_PACKET_SEND_ME_VIA_NETWORK
        {
            iter = std::lower_bound( m_vecRelayUIDs.begin(), m_vecRelayUIDs.end(), pPeer->m_UID );
            if ( iter == m_vecRelayUIDs.end() || *iter != pPeer->m_UID )
            {
                m_vecRelayUIDs.insert( iter, pPeer->m_UID );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
                m_dwRelayUIDStamp++;
                if ( m_dwRelayUIDStamp == 0 )
                    m_dwRelayUIDStamp = 1;
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
            }
        }
        pPeer->m_bUseRelay = true;
    }
}

void    CKTDNUDP::_RemoveRelayUID( __int64 iUID )
{
    std::vector<__int64>::iterator iter = std::lower_bound( m_vecRelayUIDs.begin(), m_vecRelayUIDs.end(), iUID );
    if ( iter != m_vecRelayUIDs.end() && *iter == iUID )
    {
        m_vecRelayUIDs.erase( iter );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        if ( m_vecRelayUIDs.empty() == true )
        {
            m_dwRelayUIDStamp = 0;
            m_dwRelayUIDStampFromServer = 0;
        }
        else
        {
            m_dwRelayUIDStamp++;
            if ( m_dwRelayUIDStamp == 0 )
                m_dwRelayUIDStamp = 1;
        }
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    }
    iter = std::lower_bound( m_vecNonRelayUIDs.begin(), m_vecNonRelayUIDs.end(), iUID);
    if ( iter != m_vecNonRelayUIDs.end() && *iter == iUID )
        m_vecNonRelayUIDs.erase( iter );
}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


void    CKTDNUDP::ClearPeer()
{ 
    m_PeerMap.clear(); 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    m_vecNonRelayUIDs.resize( 0 );
    m_vecRelayUIDs.resize( 0 );
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    DisconnectToRelay();
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}


WORD     CKTDNUDP::EstimateEffectiveMTUSize()
{
    int iUDPMTUSize = 0;
    if ( ConnectRelayTestResult() == false || m_vecRelayUIDs.empty() == true )
    {
        iUDPMTUSize = (int) CKTDNUDP_MTU_MAX - 1;
    }
    else
    {
        if ( GetRelayUIDsUpdated() == true )
            iUDPMTUSize = (int) CKTDNUDP_MTU_MAX - (int) ( 2 + sizeof(__int64) );
        else
            iUDPMTUSize = (int) CKTDNUDP_MTU_MAX - (int) ( 3 + m_vecRelayUIDs.size() * sizeof(__int64) );
    }
    return  iUDPMTUSize;
}


#ifdef  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY
void    CKTDNUDP::RemoveAllPendingPingSends()
{
    m_kRelayStatistic.RemoveAllPendingPingSends();
}
#endif  SERV_KTDX_OPTIMIZE_NEW_UDP_CONNECTION_STRATEGY