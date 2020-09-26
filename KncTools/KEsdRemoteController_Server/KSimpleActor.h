#pragma once
#include "KTCPSocket.h"

class KSimpleActor : public KTCPSocket
{
public:
	KSimpleActor(void);
	KSimpleActor( SOCKET kSocket );
	KSimpleActor( SOCKET kSocket, SOCKADDR_IN kSockAddr_In );
	virtual ~KSimpleActor(void);

	virtual void OnRecvCompleted( DWORD dwTransfered );
};
