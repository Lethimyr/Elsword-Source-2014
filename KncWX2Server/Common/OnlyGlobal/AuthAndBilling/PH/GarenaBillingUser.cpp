#include "GarenaBillingUser.h"
#include "GarenaBillingIOCP.h"
//#include "Socket/IOCP.h"
#include <cassert>
#include <KncSecurity/KncSecurity.h>
//#include "RelayData.h"

#include "GarenaBillingUserFSM.h"
#include "GarenaBillingServer.h"

#include "Socket/NetCommon.h"
#include "Statistics.h"
#include "KncSend.h"

//////////////////////////////////////////////////////////////////////////
//des : KSkTRUser


//{{ 2013. 04. 08	최육사	넥슨 통합 운영툴 미들웨어
#ifdef SERV_COUNTRY_PH


void KSkGarenaBillingUser::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" )
			<< BUILD_LOG( ::WSAGetLastError() );

        OnSocketError();
        return;
    }

    m_ovlRecv.m_dwLeft += dwTransfered_;

    // 패킷 길이 진행량보다 크다면 계속 새 패킷 가능성을 체크한다.
    while( m_ovlRecv.m_dwLeft >= HEADER_SIZE_NGMT )
    {
        // 패킷 길이를 알아낸다.
		unsigned long ulLength = 0;
		::memcpy( &ulLength, m_ovlRecv.m_pBuffer, sizeof( ulLength ) );

        //패킷은 최대 20byte 이상의 길이를 가져야 한다.
		//ulLength = ::ntohl( ulLength ); -- 넥슨이 패킷을 little endian으로 바로 보내고 있어서 일단 주석처리!
		unsigned long ulTotalPacketSize = ulLength + HEADER_SIZE_NGMT;
        if( ulTotalPacketSize > MAX_PACKET_SIZE_NGMT )
        {
			START_LOG( cerr, L"추출한 패킷 사이즈 이상." )
				<< BUILD_LOG( ulTotalPacketSize )
				<< BUILD_LOG( MAX_PACKET_SIZE_NGMT )
				<< END_LOG;

            OnSocketError();
            return;
        }

        // 온전한 패킷을 구성하기에 충분한 데이터가 없음. loop를 빠져나간다.
        if( m_ovlRecv.m_dwLeft < ulTotalPacketSize )
		{
			break;
		}

		// 버퍼에 받아둔 데이터로 완전한 packet이 가능한 경우 다음으로 진행한다.
        KGarenaBillingUserPtr spUser = GetGarenaBillingUserPtr();
        if( IS_NULL( spUser ) )
		{
			START_LOG( clog, L"TRUser 객체가 이미 지워짐." )
				<< END_LOG;

			OnSocketError();
			break;
		}

        KGarenaBillingPacket kPacket;
		if( kPacket.ReadFromBuffer( m_ovlRecv.m_pBuffer ) == false )
		{
			START_LOG( cerr, L"버퍼에서 읽기 실패." )
				<< END_LOG;
		}
		else
		{
			spUser->OnRecvCompleted( kPacket );
		}

        // buffer management
        m_ovlRecv.m_dwLeft -= ulTotalPacketSize;
        memmove( &m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[ulTotalPacketSize], m_ovlRecv.m_dwLeft );
    }

    // recv function call
    LIF( InitRecv() );
}

void KSkGarenaBillingUser::OnSocketError()
{
    KGarenaBillingUserPtr spTRUser = GetGarenaBillingUserPtr();
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

void KSkGarenaBillingUser::OnAcceptConnection()
{
    KGarenaBillingUserPtr spTRUser = GetGarenaBillingUserPtr();
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

KGarenaBillingUserPtr KSkGarenaBillingUser::GetGarenaBillingUserPtr()
{
    KGarenaBillingUserPtr spTRUser = m_pkTRUser.lock();

    if( spTRUser )
    {
        return spTRUser;
    }
    else
    {
        return KGarenaBillingUserPtr();
    }
}

//{{ 2009. 3. 12  최육사	프로파일러 체크용 함수
bool KSkGarenaBillingUser::SendDataWithProfile( const char* szData_, int iSize_ )
{
	KLocker lock( m_csSock );

	_JIF( m_sock != INVALID_SOCKET, return false );    // 소켓이 유효하지 않음.
	_JIF( iSize_ > 0, return false );                  // 0 byte 전송 시도도 에러처리.
	_JIF( szData_ != NULL, return false );

	MovDataQue2Buf(); // send-queue에 대기중인 데이터가 있다면 버퍼로 옮긴다.

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

	// input data의 처리 - 2. 아직 데이터가 남아 있다면 SendQueue에 복사 시도.
	if( iSize_ > 0 )
	{
		KLocker lock2( m_csSendQueue );

		iSize_ -= m_kSendQueue.Push( szData_, iSize_ );

		ms_kMaxSizeInfo.Check( m_kSendQueue.GetDataSize(), m_kSendQueue.size() );
	}

	// input data의 처리 - 3. 그래도 아직 복사하지 못한 데이터가 남아 있다면 buffer full.
	if( iSize_ > 0 )
	{
		START_LOG( cerr, L"Send Buffer Full. data size : " << iSize_
			<< L", left : " << m_ovlSend.m_dwLeft
			<< L", bSending : " << m_ovlSend.m_bSending );

		// 데이터를 보내지 않았음. 다시 보내려면 SendPakcet 한번 더 호출해야 함.
		m_ovlSend.m_dwLeft = 0;
		m_ovlSend.m_bSending = false;

		SetDisconnectReason( KStatistics::eSIColDR_SendBufferFull );
		OnSocketError();
		return false;
	}

	// 전송중이지 않으면 다시한번 WsaSend를 호출해준다.
	if( m_ovlSend.m_bSending == false )
	{
		DWORD dwWrite = 0;
		m_ovlSend.m_wsaBuffer.len = m_ovlSend.m_dwLeft;

		int ret = ::WSASend( m_sock,    // socket
			&m_ovlSend.m_wsaBuffer,     // 버퍼 포인터
			1,                          // WSABUF의 갯수
			&dwWrite,                   // [out] 즉시 전송된 량
			0,                          // Flag
			&m_ovlSend,                 // OVERLAPPED
			NULL );

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

NiImplementRootRTTI(KGarenaBillingUser);

KGarenaBillingUser::KGarenaBillingUser(void)
:
m_spSockObj(new KSkGarenaBillingUser),
m_bDestroyReserved( false )
{
	for( int i = 0; i < ENUM_TICKS::TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}
}

KGarenaBillingUser::~KGarenaBillingUser(void)
{
}

bool KGarenaBillingUser::Init()
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkTRUser = shared_from_this();
	return true;
}

void KGarenaBillingUser::Tick()
{
	if( GetStateID() == KGarenaBillingUserFSM::STATE_EXIT )
	{
		START_LOG( cout, "종료 상태에서 다시 Tick 으로 들어 왔다" );
		return ;
	}

	if( m_bDestroyReserved == true )    // 종료가 예약된 상태라면 다시한번 종료처리를 시도한다.
	{
		OnDestroy();            // derived class call
		return;
	}
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

void KGarenaBillingUser::OnSocketError()
{
	m_bDestroyReserved = true;
}

void KGarenaBillingUser::OnAcceptConnection()
{
	SetTick( ENUM_TICKS::CONNECT_TICK );
}

void KGarenaBillingUser::OnRecvCompleted( KGarenaBillingPacket& kPacket )
{
	//////////////////////////////////////////////////////////////////////////	
	START_LOG( cwarn, L"[테스트로그] 패킷 받은 객체!" )
		<< BUILD_LOG( GetUID() );
	//////////////////////////////////////////////////////////////////////////	

	// 패킷을 하나 얻었으니 이걸 파싱하기 위해서 다른곳에 보내자!
	switch( kPacket.GetPacketType() )
	{
#   undef  _ENUM
#   define _ENUM( name, id ) \
			case id: \
		{ \
		K##EJSON_GN_##name##_REQ kPacketReq; \
		kPacket.Read( kPacketReq ); \
		KEventPtr spEvent( new KEvent ); \
		UidType anTrace[2] = { GetUID(), -1 }; \
		spEvent->SetData( PI_GS_PUBLISHER_BILLING, anTrace, EJSON_GN_##name##_REQ, kPacketReq ); \
		SiKGarenaBillingServer()->QueueingEvent( spEvent ); \
		} \
		break;
#   include "GarenaBilling_def.h"
			default:
				START_LOG( cerr, L"패킷 타입이 이상함." )
					<< BUILD_LOG( kPacket.GetPacketType() )
					<< END_LOG;
				break;
	}

	// renew m_dwHBRecvTick
	SetTick( ENUM_TICKS::HB_TICK );
}

void KGarenaBillingUser::OnDestroy()
{
	//////////////////////////////////////////////////////////////////////////	
	START_LOG( cwarn, L"[테스트로그] 세션 접속 종료!" )
		<< BUILD_LOG( GetUID() );
	//////////////////////////////////////////////////////////////////////////	

	m_bDestroyReserved = false;     // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

	switch( GetStateID() ) 
	{
	case KGarenaBillingUserFSM::STATE_INIT:
		StateTransition( KGarenaBillingUserFSM::INPUT_CONNECT );
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_CONNECTED:
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_REGISTERING:
		StateTransition( KGarenaBillingUserFSM::INPUT_REG_UID_FAILED );
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_REGISTERED:
		StateTransition( KGarenaBillingUserFSM::INPUT_EXIT );
		break;
	case KGarenaBillingUserFSM::STATE_EXIT:
		break;
	}

    JIF( m_spSockObj );
    m_spSockObj->CloseSocket();
	//{{ 2009. 3. 13  최육사	그냥iocp쓴다
    SiKGarenaBillingIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	//SiKIocp()->DeleteCompKey( m_spSockObj->GetKey() );
	//}}
}

void KGarenaBillingUser::SetSendQueueSize( int iQueueSize )
{
    JIF( m_spSockObj );
    m_spSockObj->SetSendQueueSize( iQueueSize );
}

bool KGarenaBillingUser::SendPacket( const KncSecurity::KByteStream& kByteStream )
{
    _JIF( m_spSockObj, return false );
	//if( m_bDestroyReserved ) return true;///< 종료가 예약되었을때 send 시도. 이런 경우가 다분하므로, true 인정.
	_JIF( m_spSockObj->IsConnected(), return false );    ///< 소켓이 유효하지 않음.

	// send size
	//SiKTRServer()->UpdateSendPacketInfo( kByteStream.length() );

	//bool bRet = m_spSockObj->SendDataWithProfile( (const char*)kByteStream.data(), kByteStream.length() );
	bool bRet = m_spSockObj->SendData( (const char*)kByteStream.data(), kByteStream.length() );

	return bRet;
}


#endif SERV_COUNTRY_PH
//}}
