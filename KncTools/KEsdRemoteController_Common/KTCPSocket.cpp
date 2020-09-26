#include "stdafx.h"

#include "KTCPSocket.h"
#include "KNetUtil.h"

KTCPSocket::KTCPSocket()
{
}

KTCPSocket::KTCPSocket( SOCKET kSocket )
{
	SetSocket( kSocket );
}

KTCPSocket::KTCPSocket( SOCKET kSocket, SOCKADDR_IN kSockAddr_In )
{
	SetSocket( kSocket );
	SetSockAddr( kSockAddr_In );

	m_sSockAddr.sin_family   = AF_INET;

	m_ovlRecv.m_eIOMode     = KOVERLAPPED::IO_RECV;
	m_ovlSend.m_eIOMode     = KOVERLAPPED::IO_SEND;
}

KTCPSocket::~KTCPSocket()
{
}

bool KTCPSocket::CreateSocket( int iPort /*= 0 */ )
{
	m_hSocket = socket( AF_INET, SOCK_STREAM, 0 );

	if ( m_hSocket == INVALID_SOCKET )
	{
		KDbgStm << "Socket 생성 실패!" << trace;
		CloseSocket();
		return false;
	}

	::ZeroMemory( &m_sSockAddr, sizeof(m_sSockAddr) );
	m_sSockAddr.sin_family = AF_INET;
	m_sSockAddr.sin_addr.S_un.S_addr = ADDR_ANY;

	if( iPort == 0 )
	{
		iPort = _KNetUtil()->GetRandomPort();
	}
	m_sSockAddr.sin_port = htons( iPort );

	if ( bind( m_hSocket, (SOCKADDR*) &m_sSockAddr, sizeof(m_sSockAddr) ) == SOCKET_ERROR )
	{
		KDbgStm << "Bind 실패!" << trace;
		return false;
	}

	if ( listen( m_hSocket, 1 ) == SOCKET_ERROR )
	{
		KDbgStm << "Listen 실패!" << trace;
		CloseSocket();
		return false;
	}

	return true;
}


void KTCPSocket::CloseSocket()
{
	closesocket( m_hSocket );
}

bool KTCPSocket::Connect( const char* pcAddress, int iPort )
{
	// try to connect
	m_hSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if (m_hSocket == INVALID_SOCKET)
	{
		KDbgStm << "Socket 생성 실패!" << trace;
		CloseSocket();
		return false;
	}

	::ZeroMemory( &m_sSockAddr, sizeof(m_sSockAddr) );
	m_sSockAddr.sin_family = AF_INET;
	m_sSockAddr.sin_addr.S_un.S_addr = inet_addr(pcAddress);
	m_sSockAddr.sin_port = htons( iPort );

	if ( 0 != connect( m_hSocket, (SOCKADDR*) &m_sSockAddr, sizeof( m_sSockAddr ) ) )
	{
		KDbgStm << "Connect 실패!" << trace;
		CloseSocket();
		return false;
	}

	return true;
}

SOCKET KTCPSocket::Accept( SOCKADDR* sSockAddr /*= NULL*/, int* iSockAddrSize /*= NULL */ )
{
	SOCKET kOutputSocket = INVALID_SOCKET;
	kOutputSocket = accept( m_hSocket, sSockAddr, iSockAddrSize );

	if (INVALID_SOCKET == kOutputSocket)
	{
		KDbgStm << "Accept 실패!" << trace;
		return NULL;
	}

	return kOutputSocket;
}

int KTCPSocket::SendTo( SOCKET kSocket, const char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return ::send( kSocket, pcBuffer, iBufferSize, iFlag );
}

int KTCPSocket::RecvFrom( SOCKET kSocket, char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return ::recv( kSocket, pcBuffer, iBufferSize, iFlag );
}

int KTCPSocket::Send( const char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return SendTo( m_hSocket, pcBuffer, iBufferSize, iFlag );
}

int KTCPSocket::Recv( char* pcBuffer, int iBufferSize, int iFlag /*= 0 */ )
{
	return RecvFrom( m_hSocket, pcBuffer, iBufferSize, iFlag );
}

void KTCPSocket::SetSocket( SOCKET kSocket )
{
	m_hSocket = kSocket;
}

SOCKET KTCPSocket::GetSocket()
{
	return m_hSocket;
}

SOCKADDR_IN& KTCPSocket::GetSockAddr()
{
	return m_sSockAddr;
}

void KTCPSocket::SetSockAddr( SOCKADDR_IN val )
{
	m_sSockAddr = val;
}

void KTCPSocket::OnSendCompleted( DWORD dwTransfered_ )
{
	return; // 임시 처리

	//KLocker lock( m_csSock );

	if( m_hSocket == INVALID_SOCKET ) return;

	m_ovlSend.m_bSending    = false;    // 050127. 다 보냈는지 여부를 떠나서 항상 false. 재전송할 경우 아래에서 다시 true해줌.

	if( dwTransfered_ >= m_ovlSend.m_dwLeft )   // 요청된 데이터를 모두 다 보냄
	{
		m_ovlSend.m_dwLeft      = 0;        
	}
	else    // 데이터를 덜 보냄 : 다시 전송을 신청 
	{
		m_ovlSend.m_dwLeft -= m_ovlSend.InternalHigh;
		memmove( &m_ovlSend.m_pBuffer[0], &m_ovlSend.m_pBuffer[dwTransfered_], m_ovlSend.m_dwLeft );
	}

	MovDataQue2Buf();   // send-queue에 대기중인 데이터가 있다면 버퍼로 옮긴다.

	if( m_ovlSend.m_dwLeft == 0 )   // 보내야 할 데이터가 더이상 남아있지 않다면 리턴.
		return;

	m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

	DWORD dwWrite = 0;
	int ret = ::WSASend( m_hSocket,    // socket
		&m_ovlSend.m_wsaBuffer,     // 버퍼 포인터
		1,                          // WSABUF의 갯수
		&dwWrite,                   // [out] 즉시 전송된 량
		0,                          // Flag
		&m_ovlSend,                 // OVERLAPPED
		NULL );

	if( ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING )    // 현재 pending 중..
	{
		m_ovlSend.m_bSending   = true;            
		return;
	} 

	if( ret != SOCKET_ERROR ) // 호출 하자 마자 다 보냄.
	{
		m_ovlSend.m_bSending   = true;
		return;
	}

	//START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG );

	OnSocketError();

	return;
}

void KTCPSocket::MovDataQue2Buf()
{
	//KLocker lock( m_csSock );
	//KLocker lock2( m_csSendQueue );

	//int nSize = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
	//nSize = m_kSendQueue.Pop( nSize, &m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft] );
	//m_ovlSend.m_dwLeft += nSize;
}

bool KTCPSocket::InitRecv()
{
	//KSimpleLocker lock( m_csSock );

	if( m_hSocket == INVALID_SOCKET ) return false;

	DWORD dwRead = 0;
	DWORD dwFlag = 0;    

	// Note: Win2000에서는 아래의 WSARecv() 대신 ReadFile()을 사용할 수 있다.
	//       Win98에서는 ReadFile()을 사용할 수 없다.

	m_ovlRecv.Clear();
	
	ZeroMemory( m_ovlRecv.m_pBuffer, MAX_PACKET_SIZE );
	m_ovlRecv.m_eIOMode		  = KOVERLAPPED::IO_RECV;
	m_ovlRecv.m_wsaBuffer.buf = &m_ovlRecv.m_pBuffer[m_ovlRecv.m_dwLeft];
	m_ovlRecv.m_wsaBuffer.len = MAX_PACKET_SIZE - m_ovlRecv.m_dwLeft;

	int ret = ::WSARecv( m_hSocket,                    // socket
		&m_ovlRecv.m_wsaBuffer,     // buffer pointer, size
		1,                          // lpBuffers 배열에 있는 WSABUF 구조체의 수.
		&dwRead,                    // I/O 작업이 끝나자마자 보내진 바이트 수
		&dwFlag,                    // [in,out] Option Flag
		&m_ovlRecv,                 // struct LPWSAOVERLAPPED
		NULL);                      // Recv 완료시 호출될 함수 포인터

	if( SOCKET_ERROR == ret )
	{
		switch( ::GetLastError() ) {
		case WSA_IO_PENDING:    // the overlapped operation has been successfully initiated            
			return true;

		case WSAECONNRESET:     // The virtual circuit was reset by the remote side.
			OnSocketError();
			return false;

		default:
			OnSocketError();
			return false;
		}
	} // if

	// If no error occurs and the receive operation has completed immediately, WSARecv returns zero.
	if( ret == 0 )
	{        
		return true;
	}
	return false;
}

void KTCPSocket::OnSocketError()
{
	// 소켓 종료 처리
}
