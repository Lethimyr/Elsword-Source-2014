#include "UdpCheckerSession.h"
#include "../socket/NetCommon.h"  // CLOSE_SOCKET
#include <dbg/dbg.hpp>
#include "../socket/Session.h"    // EVENT_UDP_PORT_NOT
#include "KncSend.h"
#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"
#include "UdpRelayChecker.h"

#ifdef SERV_UDP_RELAY_CHECKER


KUdpCheckerSession::KUdpCheckerSession( short sUdpPort ) : 
CKTDNUDP( sUdpPort ), // 부모 생성자 호출!
m_pUdpRelayChecker( NULL )
{
}

KUdpCheckerSession::~KUdpCheckerSession(void)
{
}

void KUdpCheckerSession::Init( KUdpRelayChecker* pUdpRelayChecker, const __int64 iPeerUID )
{
	m_pUdpRelayChecker = pUdpRelayChecker;

	// UID값 세팅
	SetMyUID( iPeerUID );

	// 테스트를 위한 peer추가
	AddPeerForRelayTest( iPeerUID, NetCommon::GetLocalIPW().c_str(), GetMyPort() );
}

void KUdpCheckerSession::SetRelayIPPort( const std::wstring& wstrRelayServIP, const short sRelayServPort )
{
	SetRelayIP( wstrRelayServIP.c_str() );
	SetRelayPort( sRelayServPort );
}

void KUdpCheckerSession::ConnectRelayServer( const std::wstring& wstrRelayServIP, const short sRelayServPort )
{
    SetRelayIPPort( wstrRelayServIP, sRelayServPort );

	// 릴레이 서버 연결 시도!
	ConnectTestToRelay();
}

void KUdpCheckerSession::Run()
{
    START_LOG( clog, L"Enter Run()" );

    DWORD ret;

    while( true )
    {
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		OnFrameMove();

		ret = ::WaitForSingleObjectEx( m_hKillEvent, INFINITE, TRUE );     // sleep 타임을 주면 안된다.

		if( ret == WAIT_OBJECT_0 ) 
			break;

		_ProcessRecvData();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        ret = ::WaitForSingleObject( m_hKillEvent, 1 );     // sleep 타임을 주면 안된다.
//
//        if( ret == WAIT_OBJECT_0 ) break;
//
//        if( ret == WAIT_TIMEOUT ) OnFrameMove( 0.0, 0.f ); // 전달인자는 dummy다.
//
//        else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    }// while
}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
void KUdpCheckerSession::OnFrameMove()
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void KUdpCheckerSession::OnFrameMove( double fTime, float fElapsedTime )
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
{
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
	RecvForRelayTest();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	WSANETWORKEVENTS	netEvent;
//	RecvData kRecvData;
//	bool bRecvSuccess = false;
//
//	//패킷 리시브
//	while( true )
//	{
//		//Event select
//		::ZeroMemory( &netEvent, sizeof(netEvent) );
//
//		::WSAEnumNetworkEvents( GetSocketHandle(), GetRecvEvent(), &netEvent );
//
//		if( (netEvent.lNetworkEvents & FD_READ) == FD_READ )
//			bRecvSuccess = RecvForRelayTest( kRecvData );
//		else
//			break;
//	}
//
//	//bRecvSuccess = RecvForRelayTest( kRecvData );
//	if( bRecvSuccess )
//	{
//		switch( kRecvData.m_ID )
//		{
//		case SP_CONNECT_RELAY_ACK: // for client
//			{
//				// UDP Relay 서버에 peer가 생성되었다면 이 패킷이 도착할것이다!
//				if( m_pUdpRelayChecker->IsPrintLog() )
//				{
//					START_LOG( cout, L"[알림] Recv Packet : SP_CONNECT_RELAY_ACK" )
//						<< BUILD_LOG( kRecvData.m_SenderIP );
//				}
//
//				// 해당 Relay서버정보에 연결 결과를 저장해두자!
//				KE_CONNECT_RELAY_ACK kRelayTestPacket;
//				kRelayTestPacket.m_wstrRelayServerIP = kRecvData.m_SenderIP;
//
//				KEventPtr spEvent( new KEvent );
//				spEvent->SetData( PI_NULL, NULL, E_CONNECT_RELAY_ACK, kRelayTestPacket );
//				m_pUdpRelayChecker->QueueingEvent( spEvent );
//			}
//			break;
//
//		case E_UDP_RELAY_SERVER_CHECK_PACKET_NOT:
//			{
//				if( m_pUdpRelayChecker->IsPrintLog() )
//				{
//					START_LOG( cout, L"[알림] Recv Packet : E_UDP_RELAY_SERVER_CHECK_PACKET_NOT" )
//						<< BUILD_LOG( kRecvData.m_SenderIP );
//				}
//
//				KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT kRelayTestPacket;
//				//kRelayTestPacket.m_dwEndTickCount = ::GetTickCount();
//				kRelayTestPacket.m_dwEndTimeGetTime = ::timeGetTime();
//				kRelayTestPacket.m_wstrRelayServerIP = kRecvData.m_SenderIP;
//				
//				KEventPtr spEvent( new KEvent );
//				spEvent->SetData( PI_NULL, NULL, E_UDP_RELAY_SERVER_CHECK_PACKET_NOT, kRelayTestPacket );
//				m_pUdpRelayChecker->QueueingEvent( spEvent );
//			}
//			break;
//
//		default:
//			{
//                START_LOG( cerr, L"이상한 패킷이 도착하였습니다!" )
//					<< BUILD_LOG( kRecvData.m_ID )
//					<< END_LOG;
//			}
//			break;
//		}
//	}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

void KUdpCheckerSession::ShutDown()
{
	// thread종료!
    End();

    ::WSACleanup();
}

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
void KUdpCheckerSession::_ProcessRecvData()
{
	list<RecvData>::iterator itRecvData;
	while( GetFrontRecvData( itRecvData ) )
	{
		switch( itRecvData->m_ID )
		{
		case SP_CONNECT_RELAY_ACK: // for client
			{
				// UDP Relay 서버에 peer가 생성되었다면 이 패킷이 도착할것이다!
				if( m_pUdpRelayChecker->IsPrintLog() )
				{
					START_LOG( cout, L"[알림] Recv Packet : SP_CONNECT_RELAY_ACK" )
						<< BUILD_LOG( itRecvData->m_SenderIP );
				}

				// 해당 Relay서버정보에 연결 결과를 저장해두자!
				KE_CONNECT_RELAY_ACK kRelayTestPacket;
				kRelayTestPacket.m_wstrRelayServerIP = itRecvData->m_SenderIP;

				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_NULL, NULL, E_CONNECT_RELAY_ACK, kRelayTestPacket );
				m_pUdpRelayChecker->QueueingEvent( spEvent );
			}
			break;

		case E_UDP_RELAY_SERVER_CHECK_PACKET_NOT:
			{
				if( m_pUdpRelayChecker->IsPrintLog() )
				{
					START_LOG( cout, L"[알림] Recv Packet : E_UDP_RELAY_SERVER_CHECK_PACKET_NOT" )
						<< BUILD_LOG( itRecvData->m_SenderIP );
				}

				KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT kRelayTestPacket;
				//kRelayTestPacket.m_dwEndTickCount = ::GetTickCount();
				kRelayTestPacket.m_dwEndTimeGetTime = ::timeGetTime();
				kRelayTestPacket.m_wstrRelayServerIP = itRecvData->m_SenderIP;

				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_NULL, NULL, E_UDP_RELAY_SERVER_CHECK_PACKET_NOT, kRelayTestPacket );
				m_pUdpRelayChecker->QueueingEvent( spEvent );
			}
			break;

		default:
			{
				START_LOG( cerr, L"이상한 패킷이 도착하였습니다!" )
					<< BUILD_LOG( itRecvData->m_ID )
					<< END_LOG;
			}
			break;
		}

		ReturnRecvData( itRecvData );
	}
}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#endif  SERV_UDP_RELAY_CHECKER
