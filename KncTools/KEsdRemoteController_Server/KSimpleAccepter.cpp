#include "StdAfx.h"
#include <winsock2.h>
#include ".\ksimpleaccepter.h"

KSimpleAccepter::KSimpleAccepter(void)
:
m_fnOnAccept( NULL ),
m_bNagleAlgOn( true )
{
}

KSimpleAccepter::~KSimpleAccepter(void)
{
}

void KSimpleAccepter::Init( u_short usPort_, PFNOnAccept fnOnAccept_, bool bNagleAlgOn_ /* = true  */)
{
	m_fnOnAccept = fnOnAccept_;
	m_bNagleAlgOn = bNagleAlgOn_;

	m_sock = ::WSASocket( AF_INET, 
		SOCK_STREAM, 
		IPPROTO_TCP, 
		NULL,
		0, 
		WSA_FLAG_OVERLAPPED ); // overlapped I/O

	if( m_sock == INVALID_SOCKET )
	{        
		::WSACleanup();
		return;
	} // if

	// server restart와 같은 경우 기존의 address를 사용할 수 있게 허용한다.
	char bOK = true;
	int nRet;
	nRet = ::setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, &bOK, sizeof(char) );

	SOCKADDR_IN local;  // 특정 port와 binding을 한다.
	local.sin_family      = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port        = htons( usPort_ );

	if( ::bind( m_sock, (SOCKADDR *)&local, sizeof(local) ) == SOCKET_ERROR)
	{
		CLOSE_SOCKET( m_sock );
		::WSACleanup();
		return;
	} // if

	if( SOCKET_ERROR == ::listen(m_sock, SOMAXCONN) )
	{        
		::shutdown( m_sock, SD_BOTH );
		::closesocket( m_sock );
		m_sock = INVALID_SOCKET;
		::WSACleanup();
		return;
	} // if

	return;
}

void KSimpleAccepter::Run()
{
	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 0 );     // 0.5s 간격

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Loop();

	}// while
}

void KSimpleAccepter::End( DWORD dwTimeout_ /*dwTimeout = 3000*/ )
{
	if( NULL != m_hThread )
	{
		CLOSE_SOCKET( m_sock );     // 추가. accept 중인 소켓을 닫는다.

		SetEvent( m_hKillEvent );
		::WaitForSingleObject( m_hThread, dwTimeout_ );		
		::CloseHandle( m_hThread );
		m_hThread = NULL;
	} // if
} // End()

void KSimpleAccepter::Loop()
{
	SOCKET      socket      = INVALID_SOCKET;
	SOCKADDR_IN sinRemote;
	int         size        = sizeof(sinRemote);

	socket = ::accept( m_sock, (SOCKADDR*)&sinRemote, &size );

	if( socket == INVALID_SOCKET )
	{		
		return;
	}

	if( !m_bNagleAlgOn )    // Nagle option을 제거. 데이터를 모아서 보내지 않고 그 즉시 전송하도록 한다.
	{
		int sock_opt = 1;
		if( ::setsockopt( socket, IPPROTO_TCP, TCP_NODELAY, (char*)&sock_opt, sizeof(sock_opt) ) == SOCKET_ERROR )
		{
			return;
		}
	}

	m_fnOnAccept( socket, sinRemote );
}