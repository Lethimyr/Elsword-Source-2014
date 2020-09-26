#include "TRUser.h"
//#include "TRIOCP.h"
#include "Socket/IOCP.h"
#include <cassert>
#include <KncSecurity/KncSecurity.h>
//#include "RelayData.h"

#include "TRServer.h"
#include "TRUserFSM.h"

#include "Socket/NetCommon.h"
#include "Statistics.h"

//////////////////////////////////////////////////////////////////////////
//des : KSkTRUser

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
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof( usPacketLen ) );

        //패킷은 최소 3byte 이상의 길이를 가져야 한다.
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
            // get recvCnt
            //char cRecvCnt = -99; // 사용하지 않는 값으로 초기화.
            //memcpy( &cRecvCnt, &m_ovlRecv.m_pBuffer[ sizeof(usPacketLen) ], sizeof(cRecvCnt) );

            // get (recvUIDn, data)
            //KSerBuffer buffer;
            //buffer.Write( &m_ovlRecv.m_pBuffer[MINIMUM_PACKET_SIZE], usPacketLen - MINIMUM_PACKET_SIZE );
            //}}AFX

            KTRUserPtr spTRUser = GetTRUserPtr();
            if( spTRUser )
            {
                KSerBuffer buffer;
                buffer.Write( &m_ovlRecv.m_pBuffer[sizeof(usPacketLen)], usPacketLen - sizeof(usPacketLen) );

                spTRUser->OnRecvCompleted( buffer );
            }
            else
            {
                START_LOG( clog, L"TRUser 객체가 이미 지워짐." )
                    << END_LOG;
            }

            // buffer management
            m_ovlRecv.m_dwLeft -= usPacketLen;
            memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );            
        }
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

void KSkTRUser::OnAcceptConnection()
{
    KTRUserPtr spTRUser = GetTRUserPtr();
    if( spTRUser )
    {
        spTRUser->OnAcceptConnection();
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

//{{ 2009. 3. 12  최육사	프로파일러 체크용 함수
bool KSkTRUser::SendDataWithProfile( const char* szData_, int iSize_ )
{
	KLocker lock( m_csSock );

	_JIF( m_sock != INVALID_SOCKET, return false );    // 소켓이 유효하지 않음.
	_JIF( iSize_ > 0, return false );                  // 0 byte 전송 시도도 에러처리.
	_JIF( szData_ != NULL, return false );

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"MovDataQue2Buf 처리" );
	//////////////////////////////////////////////////////////////////////////

	MovDataQue2Buf(); // send-queue에 대기중인 데이터가 있다면 버퍼로 옮긴다.

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"MovDataQue2Buf 처리" );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"버퍼에 빈 공간이 있다면 우선 복사 시도" );
	//////////////////////////////////////////////////////////////////////////

	// input data의 처리 - 1. 버퍼에 빈 공간이 있다면 우선 복사 시도.
	int iFreeSpace = MAX_PACKET_SIZE - m_ovlSend.m_dwLeft;
	if( iFreeSpace > 0 )
	{
		int iCopySize = ( iFreeSpace < iSize_ ? iFreeSpace : iSize_ );
		::memcpy( &m_ovlSend.m_pBuffer[m_ovlSend.m_dwLeft], szData_, iCopySize );
		m_ovlSend.m_dwLeft += iCopySize;
		szData_ += iCopySize;
		iSize_ -= iCopySize;
	}

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"버퍼에 빈 공간이 있다면 우선 복사 시도" );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"아직 데이터가 남아 있다면 SendQueue에 복사 시도." );
	//////////////////////////////////////////////////////////////////////////

	// input data의 처리 - 2. 아직 데이터가 남아 있다면 SendQueue에 복사 시도.
	if( iSize_ > 0 )
	{
		KLocker lock2( m_csSendQueue );

		iSize_ -= m_kSendQueue.Push( szData_, iSize_ );

		ms_kMaxSizeInfo.Check( m_kSendQueue.GetDataSize(), m_kSendQueue.size() );
	}

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"아직 데이터가 남아 있다면 SendQueue에 복사 시도." );
	//////////////////////////////////////////////////////////////////////////

	// input data의 처리 - 3. 그래도 아직 복사하지 못한 데이터가 남아 있다면 buffer full.
	if( iSize_ > 0 )
	{
		//////////////////////////////////////////////////////////////////////////
		//BEGIN_PROFILE( L"그래도 아직 복사하지 못한 데이터가 남아 있다면 buffer full." );
		//////////////////////////////////////////////////////////////////////////

		START_LOG( cerr, L"Send Buffer Full. data size : " << iSize_
			<< L", left : " << m_ovlSend.m_dwLeft
			<< L", bSending : " << m_ovlSend.m_bSending );

		// 데이터를 보내지 않았음. 다시 보내려면 SendPakcet 한번 더 호출해야 함.
		m_ovlSend.m_dwLeft = 0;
		m_ovlSend.m_bSending = false;

		SetDisconnectReason( KStatistics::eSIColDR_SendBufferFull );
		OnSocketError();

		//////////////////////////////////////////////////////////////////////////
		//END_PROFILE( L"그래도 아직 복사하지 못한 데이터가 남아 있다면 buffer full." );
		//////////////////////////////////////////////////////////////////////////

		return false;
	}

	// 전송중이지 않으면 다시한번 WsaSend를 호출해준다.
	if( m_ovlSend.m_bSending == false )
	{
		//////////////////////////////////////////////////////////////////////////
		//BEGIN_PROFILE( L"전송중이지 않으면 다시한번 WsaSend를 호출해준다." );
		//////////////////////////////////////////////////////////////////////////

		DWORD dwWrite = 0;
		m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

		int ret = ::WSASend( m_sock,    // socket
			&m_ovlSend.m_wsaBuffer,     // 버퍼 포인터
			1,                          // WSABUF의 갯수
			&dwWrite,                   // [out] 즉시 전송된 량
			0,                          // Flag
			&m_ovlSend,                 // OVERLAPPED
			NULL );

		//////////////////////////////////////////////////////////////////////////
		//END_PROFILE( L"전송중이지 않으면 다시한번 WsaSend를 호출해준다." );
		//////////////////////////////////////////////////////////////////////////

		//std::cout << "raw length : " << kbuff_.GetLength() << std::endl << "SEND " << bsbuff << std::endl;

		if( ret == SOCKET_ERROR && ::WSAGetLastError() == ERROR_IO_PENDING )    // 현재 pending 중..
		{
			m_ovlSend.m_bSending   = true;            
			return true;
		} 

		if( ret != SOCKET_ERROR ) // 호출 하자 마자 다 보냄.
		{
			m_ovlSend.m_bSending   = true;
			return true;
		}

		START_LOG( cwarn, L"WSASend Failed. LastError : " << GET_WSA_MSG << ret );

		SetDisconnectReason( KStatistics::eSIColDR_WSASendFailed );
		OnSocketError();
		return false;
	}

	return true;
}
//}}

NiImplementRootRTTI(KTRUser);

KTRUser::KTRUser(void)
:
m_spSockObj(new KSkTRUser),
m_iUnitUID(-1),
m_bDestroyReserved( false )
{
	for( int i = 0; i < TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}
}

KTRUser::~KTRUser(void)
{
}

bool KTRUser::Init()
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkTRUser = shared_from_this();
	return true;
}

void KTRUser::Tick()
{
	if( GetStateID() == KTRUserFSM::STATE_EXIT )
	{
		START_LOG( cout, "종료 상태에서 다시 Tick 으로 들어 왔다" );
		return ;
	}

	if( m_bDestroyReserved == true )    // 종료가 예약된 상태라면 다시한번 종료처리를 시도한다.
	{
		OnDestroy();            // derived class call
		return;
	}

	switch( GetStateID() )
	{
	case KTRUserFSM::STATE_CONNECTED:

		if( ::GetTickCount() - GetTick( CONNECT_TICK ) > 15000) // Connect 이후 15초간 UID등록이 없다.
		{
			START_LOG( cwarn, L"Reguard " << m_iUnitUID 
				<< L" as a Zombie.(Reason : Not Regist UID) (tick: " 
				<< ::GetTickCount() - GetTick( CONNECT_TICK ) 
				<< L" )" );

			ReserveDestroy();
		}
		break;

	case KTRUserFSM::STATE_REGISTERING:
        {
            KTRUserPtr spTRUser = shared_from_this();
		    if( !SiKTRServer()->GetTRUserManager().Insert( spTRUser ) )     // 등록한다.
		    {
			    START_LOG( cwarn, L"Unit UID 등록실패.!" )
				    << BUILD_LOG( m_iUnitUID )
				    << END_LOG;

			    ReserveDestroy();
		    }
		    else
			    StateTransition( KTRUserFSM::INPUT_END_REG_UID );
        }
		break;

	case KTRUserFSM::STATE_REGISTERED:
		//로봇 테스트에서는 뺀다.
		if( ::GetTickCount() - GetTick( HB_TICK ) > 600000 ) // 60s 단위로 heart-bit check
		{
			START_LOG( cwarn, L"Reguard " << m_iUnitUID 
				<< L" as a Zombie.(TRUser) (tick: " 
				<< ::GetTickCount() - GetTick( HB_TICK ) 
				<< L")" );

			ReserveDestroy(); 
		}

		break;

	case KTRUserFSM::STATE_EXIT:
		break;
	} // end switch
}
//{{AFX
//bool KTRUser::Send( IN const std::vector<unsigned char>& buff_ )
//{
//	KncSecurity::KByteStream buff;
//	// 패킷 총 사이즈 + 릴레이 패킷을 의미하는 recvCnt + 전송할 데이터
//	buff.Append( sizeof(USHORT) + sizeof(char) + buff_.size(), sizeof(USHORT) );
//	buff.Append( EVENTID_RELAYED, sizeof(char) );
//	buff.append( &buff_[0] , buff_.size() );
//
//	return m_spSockObj->SendData( (const char*)buff.data(), buff.length() );
//}
//}}AFX

void KTRUser::OnSocketError()
{
	m_bDestroyReserved = true;
}

void KTRUser::OnAcceptConnection()
{
	SetTick( CONNECT_TICK );
}

void KTRUser::OnRecvCompleted( IN KSerBuffer& buff_ )
{
	KSerializer ks;
	KEventPtr spEvent( new KEvent );
	ks.BeginReading( &buff_ );
	ks.Get( *spEvent );
	ks.EndReading();

	spEvent->m_kbuff.UnCompress();

	dbg::clog << L"ⓡⓡ" << spEvent->GetIDStr() << L"TRU Recv" << END_LOG;

	switch( spEvent->m_usEventID )
	{
	case E_HEART_BEAT: //heart bit.
		break;

	case ETR_REG_UID:	// Regist UID. TRUserManager에 등록도 해야 함.
		{
			//일단 체크사항이므로 씨리얼 라이즈해서 UID 를 확인한다.
			KSerializer ks;
			UidType iUID;

			ks.BeginReading( &spEvent->m_kbuff );
			if( !ks.Get(iUID) )
			{
				START_LOG( cerr, L"deserialze failed.	ETR_REG_UID - UidType" );
			}
            ks.EndReading();

			if( m_iUnitUID != -1 )
			{
				START_LOG( cwarn, L"Duplicate UID regist. Current UID : " << m_iUnitUID 
					<< L"Recv UID : " << iUID << dbg::endl );
				break;
			}

			if( iUID <= -1 )
			{
				START_LOG( cerr, L"Recv Invalid UID: " << iUID ); 
				break;
			}

			m_iUnitUID = iUID;
			START_LOG( clog, L"Regist UID : " << m_iUnitUID );

			StateTransition( KTRUserFSM::INPUT_START_REG_UID );
		}
		break;

	case ETR_UNIT_DISCONNECT:
		{
			START_LOG( clog, L"closed by unit disconnect.!" )
				<< BUILD_LOG( m_iUnitUID )
				<< END_LOG;

			ReserveDestroy();
			return;
		}
		break;

		//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	case E_CHECK_DDOS_GUARD_REQ:
		{
			//////////////////////////////////////////////////////////////////////////
			START_LOG( cout, L"[테스트로그] DDOS Guard를 통과했습니다!" )
				<< BUILD_LOG( GetUID() );
			//////////////////////////////////////////////////////////////////////////
			
			KncSecurity::KByteStream kByteStream;
			char cDummy = 0;
			KTRUser::MakeByteStream( kByteStream, E_CHECK_DDOS_GUARD_ACK, cDummy );
			SendPacket( kByteStream );
		}
		break;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
		//}}

	default:
		if( spEvent->m_usEventID < E_HEART_BEAT )
		{
			START_LOG( cerr, L"Invalide cRecvCnt field value. data size : " << spEvent->m_kbuff.GetLength() )
				<< BUILD_LOGc( spEvent->m_usEventID )
				<< END_LOG;
			return;
		}

		SiKTRServer()->QueueingRelayData( spEvent );
	}


	//{{AFX
	//switch( cRecvCnt_ ) 
	//{
	//case EVENTID_HEART_BIT: // heart bit.
	//	break;

	//case EVENTID_REG_UID:   // Regist UID. TRUserManager에 등록도 해야 한다.
	//	{
	//		UidType iUID;
	//		buff_.Read( &iUID, sizeof(UidType) );

	//		if( m_iUnitUID != 0 )
	//		{
	//			START_LOG( cerr, L"Duplicate UID regist. Current UID : " << m_iUnitUID 
	//				<< L"Recv UID : " << iUID << dbg::endl );
	//			break;
	//		}

	//		if( iUID <= 0 )
	//		{
	//			START_LOG( cerr, L"Recv Invalid UID: " << iUID ); 
	//			break;
	//		}

	//		m_iUnitUID = iUID;
	//		START_LOG( clog, L"Regist UID : " << m_iUnitUID );

	//		StateTransition( KTRUserFSM::INPUT_START_REG_UID );
	//	}        
	//	break;

	//default:
	//	if( cRecvCnt_ < 0 ) // 위에서 필터링 되지 않은 음수 영역. 문제가 있다.
	//	{
	//		START_LOG( cerr, L"Invalide cRecvCnt field value. data size : " << buff_.GetLength() )
	//			<< BUILD_LOGc( cRecvCnt_ )
	//			<< END_LOG;
	//		return;
	//	}

	//	if( buff_.GetLength() < cRecvCnt_ * sizeof(UidType) )
	//	{
	//		START_LOG( cerr, L"Invalide packet received. data size : " << buff_.GetLength() )
	//			<< BUILD_LOGc( cRecvCnt_ )
	//			<< END_LOG;
	//		OnSocketError();
	//	}

	//	KRelayDataPtr spData( new KRelayData );
	//	spData->m_cRecvCnt = cRecvCnt_;

	//	// cRecvCnt_에 담긴 수만큼 DWORD를 읽어낸다.
	//	spData->m_vecUID.resize( cRecvCnt_ );
	//	for( int i = 0 ; i < cRecvCnt_ ; ++i )
	//	{
	//		buff_.Read( &spData->m_vecUID[i], sizeof(UidType) );
	//	}

	//	spData->m_buff.resize( buff_.GetReadLength() );
	//	buff_.Read( &spData->m_buff[0], buff_.GetReadLength() );

	//	SiKTRServer()->QueueingRelayData( spData );
	//}
	//}}AFX

	// renew m_dwHBRecvTick
	SetTick( HB_TICK );
}

void KTRUser::OnDestroy()
{
	m_bDestroyReserved = false;     // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

	switch( GetStateID() ) 
	{
	case KTRUserFSM::STATE_INIT:
		StateTransition( KTRUserFSM::INPUT_CONNECT );
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_CONNECTED:
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_REGISTERING:
		StateTransition( KTRUserFSM::INPUT_REG_UID_FAILED );
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_REGISTERED:
		StateTransition( KTRUserFSM::INPUT_EXIT );
		break;
	case KTRUserFSM::STATE_EXIT:
		break;
	}

    JIF( m_spSockObj );
    m_spSockObj->CloseSocket();
	//{{ 2009. 3. 13  최육사	그냥iocp쓴다
    //SiKTRIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	SiKIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	//}}
}

void KTRUser::SetSendQueueSize( int iQueueSize )
{
    JIF( m_spSockObj );
    m_spSockObj->SetSendQueueSize( iQueueSize );
}

bool KTRUser::SendPacket( const KncSecurity::KByteStream& kByteStream )
{
    _JIF( m_spSockObj, return false );
	//if( m_bDestroyReserved ) return true;///< 종료가 예약되었을때 send 시도. 이런 경우가 다분하므로, true 인정.
	_JIF( m_spSockObj->IsConnected(), return false );    ///< 소켓이 유효하지 않음.

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"send size 통계" );
	//////////////////////////////////////////////////////////////////////////

	// send size
	//SiKTRServer()->UpdateSendPacketInfo( kByteStream.length() );

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"send size 통계" );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//BEGIN_PROFILE( L"SocketObject에서 SendData하기" );
	//////////////////////////////////////////////////////////////////////////

	//bool bRet = m_spSockObj->SendDataWithProfile( (const char*)kByteStream.data(), kByteStream.length() );
	bool bRet = m_spSockObj->SendData( (const char*)kByteStream.data(), kByteStream.length() );

	//////////////////////////////////////////////////////////////////////////
	//END_PROFILE( L"SocketObject에서 SendData하기" );
	//////////////////////////////////////////////////////////////////////////

	return bRet;
}



