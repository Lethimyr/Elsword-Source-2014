#include "TRUser.h"
#include "Socket/IOCP.h"
#include <cassert>
#include <KncSecurity/KncSecurity.h>
//#include "TRUserManager.h"

void KSkTRUser::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" );
        OnSocketError();
        return;
    }

    USHORT usPacketLen = 0;

    m_ovlRecv.m_dwLeft += dwTransfered_;

    // 패킷 길이 진행량보다 크다면 계속 새 패킷 가능성을 체크한다.
    while( m_ovlRecv.m_dwLeft >= sizeof( usPacketLen ) )
    {
        // 패킷 길이를 알아낸다.
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof(usPacketLen) );

        // 패킷은 최소 3byte 이상의 길이를 가져야 한다.
        if( usPacketLen < MINIMUM_PACKET_SIZE )
        {
            START_LOG( cerr, L"invalid packet size - " << usPacketLen );
            OnSocketError();
            return;
        }

        // 버퍼에 받아둔 데이터로 완전한 packet이 가능한 경우 다음으로 진행한다.
        if( m_ovlRecv.m_dwLeft >= usPacketLen )
        {
            //{{AFX
            //// get recvCnt
            //char cRecvCnt = -99; // 사용하지 않는 값으로 초기화.
            //memcpy( &cRecvCnt, &m_ovlRecv.m_pBuffer[ sizeof(usPacketLen) ], sizeof(cRecvCnt) );

            //// get (recvUIDn, data)
            //KSerBuffer buffer;
            //buffer.Write( &m_ovlRecv.m_pBuffer[MINIMUM_PACKET_SIZE], usPacketLen - MINIMUM_PACKET_SIZE );
            //}}AFX

            KTRUserPtr spTRUser = GetTRUserPtr();
            if( spTRUser )
            {
                //길이를 제외한 데이터만 버퍼에 담는다.
                KSerBuffer buffer;
                buffer.Write( &m_ovlRecv.m_pBuffer[sizeof(usPacketLen)], usPacketLen - sizeof(usPacketLen) );

                //m_pkTRUser->OnRecvCompleted( cRecvCnt, buffer );
                //spTRUser->OnRecvCompleted( buffer );
            }
            else
            {
                START_LOG( clog, L"TRUser 객체가 이미 지워짐." )
                    << END_LOG;
            }

            // buffer management
            m_ovlRecv.m_dwLeft -= usPacketLen;
            memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );            

        } // if( m_ovlRecv.m_dwLeft >= dwHeadPacketLen )    // 완전한 패킷이 가능한 경우
        else
        {
            break;     // 온전한 패킷을 구성하기에 충분한 데이터가 없음. loop를 빠져나간다.
        }
    } // while( m_ovlRecv.m_dwLeft >= sizeof(DWORD) )

    // recv function call
    LIF( InitRecv() );
}

void KSkTRUser::OnSocketError()
{
    KTRUserPtr spTRUser = GetTRUserPtr();
    if( spTRUser )
    {
        spTRUser->OnSocketError();
    }
    else
    {
        START_LOG( clog, L"TRUser 객체가 이미 지워짐." )
            << END_LOG;
    }
}

KTRUserPtr KSkTRUser::GetTRUserPtr()
{
    KTRUserPtr spTRUser = m_pkTRUser.lock();
    if( spTRUser )
    {
        return spTRUser;
    }
    else
    {
        return KTRUserPtr();
    }
}

KTRUser::KTRUser(void)
:
m_spSockObj( new KSkTRUser ),
m_iUnitUID( -1 ),
m_bDestroyReserved( false ),
m_ServerMsgID( 0 ),
ret( NULL ),
bLoop( true )
{
	for( int i = 0; i < ENUM_TICKS::TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}

	//m_hEvents[EVENT_RECV_COMPLETED] = NULL;
	//m_hEvents[EVENT_SEND_COMPLETED] = NULL;

	START_LOG( clog, L"유저 생성.!" ) << END_LOG;
}

KTRUser::~KTRUser(void)
{
	//::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
	//::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
}

void KTRUser::Tick()
{
    //if( bLoop == false )
    //	return;

    //ret = NULL;
    //ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE, m_hEvents, false, 1 );     // 0.1s 간격

    //switch( ret )
    //{
    //case EVENT_DISCONNECTED + WAIT_OBJECT_0:
    //	bLoop = false;
    //	break;

    //case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
    //	{
    //		m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_RECV );
    //	}
    //	break;
    //case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
    //	{
    //		m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_SEND );
    //	}
    //	break;
    //case WAIT_TIMEOUT:
    //	break;
    //}

    if( m_iUnitUID == -1 )
    {
        START_LOG( cwarn, "종료 상태에서 다시 Tick 으로 들어 왔다" );
        return;
    }

    if( m_bDestroyReserved )    // 종료가 예약된 상태라면 다시한번 종료처리를 시도한다.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart bit
    if( !IsConnected() )    // UserProxy의 경우, 연결이 없어도 살아있거나 혹은 재접속 할 수도 있다 (Robot)
        return;

    if( ::GetTickCount() - GetTick( ENUM_TICKS::HB_TICK ) <= 30000 ) // 30s 단위로 heart-bit 전송
        return;

    JIF( m_spSockObj );
    START_LOG( clog, L" Heart-bit " << m_iUnitUID << L"  :" << ::GetTickCount() - GetTick( ENUM_TICKS::HB_TICK ) );
    SetTick( ENUM_TICKS::HB_TICK );

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT) + sizeof(char), sizeof(USHORT) );
    buff.Append( E_HEART_BEAT, sizeof(char) );
    LIF( m_spSockObj->SendData( ( const char* )buff.data(), buff.length() ) );
}

bool KTRUser::Init()
{
	//m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	//m_kSockObj.SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
	//m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	//m_kSockObj.SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );

    m_spSockObj->m_pkTRUser = shared_from_this();
	return true;
}

bool KTRUser::Connect( const char* szIP_, unsigned short usPort_, const UidType iUnitUID_ )
{
	std::vector< std::pair<int,int> > vecOpt;
	vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // 사용중인 address를 재사용할 수 있게 허용. 
	vecOpt.push_back( std::make_pair( IPPROTO_TCP, TCP_NODELAY ) ); // Nagle option을 제거. 데이터를 모아서 보내지 않고 그 즉시 전송하도록 한다.

    _JIF( m_spSockObj, return false );
	_JIF( m_spSockObj->Connect( szIP_, usPort_, false, &vecOpt ), false );

	//Begin();    // thread run

	// 접속 후 즉시 UID를 등록한다.
	m_iUnitUID = iUnitUID_;

	// 접속이 성공하면 IOCP에 등록 시킨다.
	//LIF( SiKTRIocp()->AssociateSocket( &m_kSockObj ) );
	//_JIF( SiKTRIocp()->AssociateSocket( &m_kSockObj ), return false );
	if( SiKIocp()->AssociateSocket( m_spSockObj ) == false )
	{
		START_LOG(cerr, L"IOCP 등록실패.!")
			<< END_LOG;
	}

	//{{AFX : 접속 성공후 클라이언트 에서 직접 UID 등록 패킷을 보내준다.
	//KncSecurity::KByteStream buff;
	//buff.Append( sizeof(USHORT) + sizeof(char) + sizeof(iUnitUID_), sizeof(USHORT) );
	//buff.Append( 0, sizeof(char) );
	//buff.Append( iUnitUID_ );
	//LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length() ) );
	//}}AFX

	//SendPacket( ETR_REG_UID, m_iUnitUID );

	//START_LOG( clog, "Send UID : " << iUnitUID_ );

	return true;
}

void KTRUser::OnSocketError()
{
    m_bDestroyReserved = true;
}

//bool KTRUser::Begin()
//{
//	if( KThread::Begin() )
//	{
//		m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
//		return true;
//	}
//
//	return false;
//}

//void KTRUser::Run()
//{
//	DWORD ret       = NULL;
//	bool bLoop      = true;
//
//	while( bLoop )
//	{
//		ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE, m_hEvents, false, 100 );     // 0.1s 간격
//
//		switch( ret )
//		{
//		case EVENT_DISCONNECTED + WAIT_OBJECT_0:
//			bLoop = false;
//			break;
//		case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
//			{
//				m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_RECV );
//			}
//			break;
//		case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
//			{
//				m_kSockObj.OnIOCompleted( KOVERLAPPED::IO_SEND );
//			}
//			break;
//		case WAIT_TIMEOUT:
//			break;
//		}
//
//		Tick();
//	}
//
//}

void KTRUser::OnRecvCompleted( IN KSerBuffer& buffer_ )
{
	//Client 쪽은 시스템적 이벤트 식별이 필요없기 때문에 바로 보내줌.
	KSerializer ks;
	//KEventPtr spEvent( new KEvent );
	KEvent kEvent;
	ks.BeginReading( &buffer_ );
	ks.Get( kEvent );
	ks.EndReading();

	kEvent.m_kbuff.UnCompress();

	KSerBuffer* pBuff = new KSerBuffer( kEvent.m_kbuff );

	dbg::clog << L"ⓡⓡ " << kEvent.GetIDStr() << L" - TRU Recv" << END_LOG;

	//if( m_pSendGameMessage != NULL )
	//	m_pSendGameMessage(
	//	m_ServerMsgID,
	//	static_cast<WPARAM>(spEvent->m_usEventID),
	//	reinterpret_cast<LPARAM>(pBuff),
	//	false 
	//	);
}

void KTRUser::OnDestroy()
{
    m_bDestroyReserved = false;     // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

    //SetEvent( m_hKillEvent );

    //SiKTRUserManager()->DeleteRobot(GetUID());

    m_iUnitUID = -1;

    START_LOG( cout, L"Closed by OnDestroy" );

    JIF( m_spSockObj );
    m_spSockObj->CloseSocket();
    SiKIocp()->DeleteCompKey( m_spSockObj->GetKey() );
}
