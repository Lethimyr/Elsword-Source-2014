#pragma once
#include "ServerDefine.h"

//{{ 2013. 2. 5	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
#include "..\Common\Socket\KTDNUDP.h"
#include <thread/thread.h>
#include <KNCSingleton.h>

// 일단은 랜선렉 방지 용도로만 사용한다.
// 다른 정보를 주고 받을 필요가 생긴다면, 수정하도록 하자.

// KTDNUDP에서 Relay 패킷이 오면 이쪽으로 UnitUID만 보내서 알려주는 것도 한 가지 방법
// 그렇게 되면 모든 처리는 이곳에서 하게 된다.
class KInteriorUdpSession
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK    
//    : public KThread
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK    
{
	DeclInstanceKeeping( KInteriorUdpSession );

public:
	KInteriorUdpSession();
	virtual ~KInteriorUdpSession(void);

	void	Init( IN u_short usTargetPort );

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	// derived from KThread
//	void	Run( void );
//	void	ShutDown( void );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK    

	// send info
	void	DataPushBack( IN const CKTDNUDP::LB_EVENT_ID eEventID, IN const __int64 iRoomUID, IN const __int64 iUnitUID ){	m_vecStorage.push_back( CKTDNUDP::LB_INFO_STRUCT( eEventID, iRoomUID, iUnitUID ) );	}
	void	SendAndFlush( void );


//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	__forceinline	bool	IsLocal( IN const u_long ulCheckIP )
//	{
//		if( ( ulCheckIP != m_ulLocalIP1 ) &&
//			( ulCheckIP != m_ulLocalIP2 )
//			)
//		{
//			return false;
//		}
//		return true;
//	}
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

private:
	void	OnFrameMove( void );
//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//	bool	Recv( void );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

	std::vector<CKTDNUDP::LB_INFO_STRUCT>	m_vecStorage;

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//
//	SOCKET					m_Socket;
//	u_long					m_ulLocalIP1;
//	u_long					m_ulLocalIP2;
//	WSAEVENT				m_RecvEvent;
//	char					m_pRecvBuffer[RECV_BUFFER_SIZE];
//	char					m_pUncompressBuffer[RECV_BUFFER_SIZE];
//	std::vector<CKTDNUDP::LB_INFO_STRUCT>	m_RecvDataList;
//
//	// send info
//	SOCKADDR_IN				m_DesAddr;
//	char					m_pSendBuffer[RECV_BUFFER_SIZE];
//	char					m_pCompressBuffer[RECV_BUFFER_SIZE];
//
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
};
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}
