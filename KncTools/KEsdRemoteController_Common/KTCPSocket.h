#pragma once

#include <WinSock2.h>
#include "../KEsdRemoteController_Common/Overlapped.h"

class KTCPSocket
{

public:
	KTCPSocket(void);
	KTCPSocket(SOCKET kSocket);
	KTCPSocket(SOCKET kSocket, SOCKADDR_IN kSockAddr_In);
	~KTCPSocket(void);

	bool CreateSocket( int iPort = 0 );
	void CloseSocket();

	bool Connect( const char* pcAddress, int iPort );
	SOCKET Accept( SOCKADDR* sSockAddr = NULL, int* iSockAddrSize = NULL );

	int SendTo( SOCKET kSocket, const char* pcBuffer, int iBufferSize, int iFlag = 0 );
	int RecvFrom( SOCKET kSocket, char* pcBuffer, int iBufferSize, int iFlag = 0 );

	int Send( const char* pcBuffer, int iBufferSize, int iFlag = 0 );
	int Recv( char* pcBuffer, int iBufferSize, int iFlag = 0 );

	void SetSocket( SOCKET kSocket );
	SOCKET GetSocket();
	void SetSockAddr( SOCKADDR_IN val );
	SOCKADDR_IN& GetSockAddr();

	//////////////////////////////////////////////////////////////////////////
	void SetSocketInfo( SOCKET socket, const SOCKADDR_IN& sockAddr ) { m_hSocket = socket; m_sSockAddr = sockAddr; }
	bool InitRecv();
	virtual void OnAcceptConnection() {} // accept 되자마자 호출.

	void    SetKey( DWORD dwKey )   { m_dwKey = dwKey; }
	DWORD   GetKey() const          { return m_dwKey; }
	virtual void OnSocketError();

	void OnSendCompleted( DWORD dwTransfered );
	virtual void OnRecvCompleted( DWORD dwTransfered ) {};

protected:
	inline void MovDataQue2Buf();   // send-queue에 대기중인 데이터가 있다면 버퍼로 옮긴다.

protected:

	SOCKET			m_hSocket;
	SOCKADDR_IN		m_sSockAddr;
	DWORD			m_dwKey;
	KOVERLAPPED		m_ovlRecv;
	KOVERLAPPED		m_ovlSend;

	friend class KSimpleIOCP;
};
