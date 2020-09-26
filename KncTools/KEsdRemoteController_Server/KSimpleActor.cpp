#include "StdAfx.h"
#include "KEsdRemoteController_Server.h"
#include ".\ksimpleactor.h"

KSimpleActor::KSimpleActor(void)
{
}

KSimpleActor::KSimpleActor( SOCKET kSocket )
: KTCPSocket( kSocket )
{
}

KSimpleActor::KSimpleActor( SOCKET kSocket, SOCKADDR_IN kSockAddr_In )
: KTCPSocket( kSocket, kSockAddr_In )
{
}


KSimpleActor::~KSimpleActor(void)
{
}

void KSimpleActor::OnRecvCompleted( DWORD dwTransfered )
{
	// packet parsing
	theApp.m_kImp.OnRecv( m_ovlRecv.m_pBuffer, dwTransfered );

	// recv function call
	InitRecv();
}