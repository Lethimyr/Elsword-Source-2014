// Precompiled Header
#include "stdafx.h"

#include "KUdpSocket.h"
#include "KNetUtil.h"

KUDPSocket::KUDPSocket(void)
{
}

KUDPSocket::~KUDPSocket(void)
{
}

bool KUDPSocket::CreateSocket( int iPort )
{
    m_hSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

	if( m_hSocket == INVALID_SOCKET )
	{
		KDbgStm << "Socket 생성 실패!" << trace;
		CloseSocket();
		return false;
	}

	m_sSockAddr.sin_family = AF_INET;

	if( iPort == 0 )
	{
		iPort = _KNetUtil()->GetRandomPort();
	}
	m_sSockAddr.sin_port = htons(iPort);
	m_sSockAddr.sin_addr.s_addr = inet_addr( _KNetUtil()->GetHostIP().c_str() ) ;

	if( bind( m_hSocket, (SOCKADDR*)&m_sSockAddr, sizeof(m_sSockAddr) ) != 0 )
	{
		KDbgStm << "Bind 실패!" << trace;
		CloseSocket();
		return false;
	}

	return true;
}

void KUDPSocket::CloseSocket()
{
	closesocket( m_hSocket );
}

void KUDPSocket::SetEnableBroadCast()
{
	BOOL bBroadcast = TRUE;
	::setsockopt( m_hSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL) );
}

void KUDPSocket::SetDisableBroadCast()
{
	BOOL bBroadcast = FALSE;
	::setsockopt( m_hSocket, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL) );
}

void KUDPSocket::SendBroadCast( const char* pcBuffer, int iBufferSize, int iPort )
{
	SOCKADDR_IN sAddr;
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	sAddr.sin_port = htons( iPort );

	int iSize = sendto( m_hSocket, pcBuffer, iBufferSize, 0, (SOCKADDR*)&sAddr, sizeof(sAddr) );
}

int KUDPSocket::SendTo( const char* pcBuffer, int iBufferSize, SOCKADDR* psSockAddr, int iFlag )
{
	return ::sendto( m_hSocket, pcBuffer, iBufferSize, iFlag, psSockAddr, sizeof(SOCKADDR_IN) );
}

int KUDPSocket::RecvFrom( char* pcBuffer, int iBufferSize, SOCKADDR* psSockAddr, int iFlag )
{
	int iAddrSize = sizeof( SOCKADDR_IN);
	return ::recvfrom( m_hSocket, pcBuffer, iBufferSize, iFlag, psSockAddr, &iAddrSize );
}

SOCKET KUDPSocket::GetSocket()
{
	return m_hSocket;
}

SOCKADDR_IN& KUDPSocket::GetSockAddr()
{
	return m_sSockAddr;
}
