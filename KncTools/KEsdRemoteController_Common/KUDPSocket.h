#pragma once

#include <WinSock2.h>

class KUDPSocket
{
public:
	KUDPSocket(void);
	~KUDPSocket(void);

	bool CreateSocket( int iPort = 0 );
	void CloseSocket();

	void SetEnableBroadCast();
	void SetDisableBroadCast();

	void SendBroadCast( const char* pcBuffer, int iBufferSize, int iPort );
	int SendTo( const char* pcBuffer, int iBufferSize, SOCKADDR* psSockAddr, int iFlag = 0 );
	int RecvFrom( char* pcBuffer, int iBufferSize,  SOCKADDR* psSockAddr, int iFlag = 0 );

    SOCKET GetSocket();
	SOCKADDR_IN& GetSockAddr();

protected:

	SOCKET m_hSocket;
	SOCKADDR_IN m_sSockAddr;
   
};
