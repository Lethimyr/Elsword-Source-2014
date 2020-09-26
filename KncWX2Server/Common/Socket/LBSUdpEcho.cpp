#include <Socket/KTDNUDP.h>
#include "LBSUdpEcho.h"
#include <dbg/dbg.hpp>
#include "BaseServer.h"
#include "ActorManager.h"

#include <Serializer/SerBuffer.h>
#include <Serializer/SerPair.h>
#include <Serializer/SerList.h>
#include <Serializer/SerVector.h>
#include <Serializer/SerDeque.h>
#include <Serializer/SerMap.h>
#include <Serializer/SerSet.h>
#include <Serializer/SerializeMacro.h>

template < typename T >
inline void Serialize( KSerBuffer* pBuff, T* pData )
{
	KSerializer ks;
	ks.BeginWriting( pBuff );
	ks.Put( *pData );
	ks.EndWriting();
}

template < typename T >
inline void DeSerialize( KSerBuffer* pBuff, T* pData )
{
	KSerializer ks;
	ks.BeginReading( pBuff );
	ks.Get( *pData );
	ks.EndReading();
}

#include "X2Packet.h"

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
// 시스템 내에 KLBSUDPEcho 개체가 1개 밖에 없다고 가정
// 서로 다른 쓰레드에서 여러 개체가 생성될 때는 안전하지 않다. 
static std::list<CKTDNUDP::OverlappedDataInOp> s_listOverlappedData1;
static std::list<CKTDNUDP::OverlappedDataInOp> s_listOverlappedData2;
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK


KLBSUdpEcho::KLBSUdpEcho(void)
{
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    m_usPort = 0;
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
}

KLBSUdpEcho::~KLBSUdpEcho(void)
{
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    End();
    s_listOverlappedData1.clear();
    s_listOverlappedData2.clear();
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

bool KLBSUdpEcho::Init( USHORT usPort_ )
{
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    m_usPort = usPort_;
#else   SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    m_spUDP = CKTDNUDPPtr( new CKTDNUDP( usPort_ ) );
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX

    return true;
}

void KLBSUdpEcho::Run()
{
    START_LOG( clog, L"Enter Run()" );

    DWORD ret;
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    m_spUDP = CKTDNUDPPtr( new CKTDNUDP( m_usPort ) );
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX

    while( true )
    {
//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
		m_spUDP->OnFrameMove();

		ret = ::WaitForSingleObjectEx( m_hKillEvent, INFINITE, TRUE );

		if( ret == WAIT_OBJECT_0 )
			break;

		m_spUDP->ProcessRecvData();

		_ProcessRecvData();
//#else//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        ret = ::WaitForSingleObject( m_hKillEvent, 1 );     // sleep 타임을 주면 안된다.
//
//        if( ret == WAIT_OBJECT_0 ) break;
//
//        if( ret == WAIT_TIMEOUT ) Loop();
//
//        else std::cout << "*** WaitForSingleObject() - return : " << ret << std::endl;
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
    }// while

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    // overlapped io 는 io 를 요청한 thread 가 파괴될 때 모두 같이 파괴된다.
    // 아래 구문의 역할은 udp manager 에서 사용되는 overlapped io 데이타가 쓰레드 파괴시까지 유지되도록 하는 것이다.
    m_spUDP->ThreadUnsafe_ObtainOverlappedData( s_listOverlappedData1, s_listOverlappedData2 );
    m_spUDP.reset();
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
}

u_short KLBSUdpEcho::GetPort()
{
    if( m_spUDP )
    {
        return m_spUDP->GetMyPort();
    }
    else
    {
#ifdef  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
        return  m_usPort;
#else   SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
        return 0;
#endif  SERV_OPTIMIZE_UDP_MANAGER_CRASH_BUG_FIX
    }
}

//#ifndef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//void KLBSUdpEcho::Loop()
//{
//	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
//#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
//	m_spUDP->OnFrameMove();
//#else
//    m_spUDP->OnFrameMove( 0.0f, 0.001f );
//#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//	//}}
//
//    CKTDNUDP::RecvData* pRecvData = m_spUDP->PopRecvData();
//
//    while( pRecvData != NULL )
//    {
//        switch( pRecvData->m_ID )
//        {
//     //       case XPT_CHECK_SERVER_ALIVE_REQ:
//     //           {
//					//KXPT_CHECK_SERVER_ALIVE_ACK kXPT_CHECK_SERVER_ALIVE_ACK;
//					//kXPT_CHECK_SERVER_ALIVE_ACK.UID = ( int )KBaseServer::GetKObj()->GetUID();
//
//					//KSerBuffer buff;
//					//Serialize( &buff, &kXPT_CHECK_SERVER_ALIVE_ACK );
//
//     //               m_spUDP->Send( pRecvData->m_SenderIP.c_str(), pRecvData->m_SenderPort, XPT_CHECK_SERVER_ALIVE_ACK, (char*)buff.GetData(), buff.GetLength() );
//     //           }
//     //           break;
//
//			case XPT_PORT_CHECK_REQ:
//				{
//					KSerBuffer ksBuff;
//					ksBuff.Write( pRecvData->m_pRecvBuffer, pRecvData->m_Size );
//					KXPT_PORT_CHECK_REQ kXPT_PORT_CHECK_REQ;
//					DeSerialize( &ksBuff, &kXPT_PORT_CHECK_REQ );
//
//					KActorPtr pkActor = KActorManager::GetKObj()->Get( kXPT_PORT_CHECK_REQ.m_UserUID );
//
//					if( pkActor != NULL )
//					{
//						pkActor->SetP2PIP( pRecvData->m_SenderIP );
//						pkActor->SetP2PPort( pRecvData->m_SenderPort );
//						//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
////#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//						pkActor->SetP2PInternalIP( kXPT_PORT_CHECK_REQ.m_wstrInternalIP );
//						pkActor->SetP2PInternalPort( kXPT_PORT_CHECK_REQ.m_usInternalPort );
//
//						START_LOG( clog, L"IP, 포트 확인" )
//							<< BUILD_LOG( kXPT_PORT_CHECK_REQ.m_wstrInternalIP )
//							<< BUILD_LOG( kXPT_PORT_CHECK_REQ.m_usInternalPort )
//							<< END_LOG;
////#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//						//}}
//					}
//					else
//					{
//						START_LOG( cwarn, L"pkActor 찾기 실패.!" )
//							<< BUILD_LOG( kXPT_PORT_CHECK_REQ.m_UserUID )
//							<< END_LOG;
//					}
//
//					if( m_spUDP != NULL )
//					{
//						KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;
//						kXPT_PORT_CHECK_ACK.m_IP	= pRecvData->m_SenderIP;
//						kXPT_PORT_CHECK_ACK.m_Port	= pRecvData->m_SenderPort;
//
//						KSerBuffer buff;
//						Serialize( &buff, &kXPT_PORT_CHECK_ACK );
//
//						m_spUDP->Send( pRecvData->m_SenderIP.c_str(), pRecvData->m_SenderPort, XPT_PORT_CHECK_ACK, (char*)buff.GetData(), buff.GetLength() );
//					}
//				}
//				break;
//        }
//
//        if( pRecvData )
//        {
//            delete pRecvData;
//            pRecvData = NULL;
//        }
//
//        pRecvData = m_spUDP->PopRecvData();
//    }
//}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

//#ifdef SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
void KLBSUdpEcho::_ProcessRecvData()
{
	std::list< CKTDNUDP::RecvData >::iterator itRecvData;
	while( m_spUDP->GetFrontRecvData( itRecvData ) )
	{
		switch( itRecvData->GetID() )
		{
			//       case XPT_CHECK_SERVER_ALIVE_REQ:
			//           {
			//KXPT_CHECK_SERVER_ALIVE_ACK kXPT_CHECK_SERVER_ALIVE_ACK;
			//kXPT_CHECK_SERVER_ALIVE_ACK.UID = ( int )KBaseServer::GetKObj()->GetUID();

			//KSerBuffer buff;
			//Serialize( &buff, &kXPT_CHECK_SERVER_ALIVE_ACK );

			//               m_spUDP->Send( itRecvData->m_SenderIP.c_str(), itRecvData->m_SenderPort, XPT_CHECK_SERVER_ALIVE_ACK, (char*)buff.GetData(), buff.GetLength() );
			//           }
			//           break;

		case XPT_PORT_CHECK_REQ:
			{

                if ( itRecvData->GetSize() != sizeof(KXPT_PORT_CHECK_REQ) )
                    break;
                const KXPT_PORT_CHECK_REQ* pkReq = (const KXPT_PORT_CHECK_REQ*) itRecvData->GetData();
                const KXPT_PORT_CHECK_REQ& kXPT_PORT_CHECK_REQ = *pkReq;

				KActorPtr pkActor = KActorManager::GetKObj()->Get( kXPT_PORT_CHECK_REQ.m_UserUID );

				if( pkActor != NULL )
				{
                    u_long uAddress = itRecvData->m_SenderIPAddress;
                    std::wstring wstrAddr = CKTDNUDP::ConvertAddressToIP( itRecvData->m_SenderIPAddress );


                    pkActor->SetP2PIP( wstrAddr );

					pkActor->SetP2PPort( itRecvData->m_SenderPort );
					//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP

                    pkActor->SetP2PInternalIP( CKTDNUDP::ConvertAddressToIP( kXPT_PORT_CHECK_REQ.m_InternalIPAddress ) );

					pkActor->SetP2PInternalPort( kXPT_PORT_CHECK_REQ.m_usInternalPort );

					START_LOG( clog, L"IP, 포트 확인" )
						<< BUILD_LOG( pkActor->GetP2PInternalIP() )
						<< BUILD_LOG( kXPT_PORT_CHECK_REQ.m_usInternalPort )
						<< END_LOG;
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
					//}}
				}
				else
				{
					START_LOG( cwarn, L"pkActor 찾기 실패.!" )
						<< BUILD_LOG( kXPT_PORT_CHECK_REQ.m_UserUID )
						<< END_LOG;
				}

				if( m_spUDP != NULL )
				{
					KXPT_PORT_CHECK_ACK kXPT_PORT_CHECK_ACK;

                    kXPT_PORT_CHECK_ACK.m_IPAddress	= itRecvData->m_SenderIPAddress;

					kXPT_PORT_CHECK_ACK.m_Port	= itRecvData->m_SenderPort;


					m_spUDP->SendToIP( 
                        itRecvData->m_SenderIPAddress, 
                        itRecvData->m_SenderPort, XPT_PORT_CHECK_ACK, &kXPT_PORT_CHECK_ACK, sizeof(kXPT_PORT_CHECK_ACK) );

				}
			}
			break;
		}

		//
		m_spUDP->ReturnRecvData( itRecvData );
	}
}
//#endif//SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
