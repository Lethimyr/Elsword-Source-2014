#include "StdAfx.h"

#include "TRUser.h"

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

NiImplementRootRTTI( KTRUser );

KTRUser::KTRUser(void)
:
m_spSockObj( new KSkTRUser ),
m_iUnitUID( 0L ),
m_bDestroyReserved( false ),
m_ServerMsgID( 0 ),
m_pSendGameMessage(NULL)
{
	for( int i = 0; i < KTRUser::TICKS_NUM; i++ )
	{
		m_adwTickCount[i] = ::GetTickCount();
	}

	m_hEvents[EVENT_RECV_COMPLETED] = NULL;
	m_hEvents[EVENT_SEND_COMPLETED] = NULL;
}

KTRUser::~KTRUser(void)
{
	End( INFINITE );
	OnDestroy();

	::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
	::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
}

bool KTRUser::Begin()
{
    if( KThread::Begin() )
    {
        m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
        return true;
    }

    return false;
}

void KTRUser::Tick()
{
    if( m_bDestroyReserved )    // 종료가 예약된 상태라면 다시한번 종료처리를 시도한다.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart bit
    if( !IsConnected() )    // UserProxy의 경우, 연결이 없어도 살아있거나 혹은 재접속 할 수도 있다 (Robot)
        return;

    if( ::GetTickCount() - GetTick( KTRUser::HB_TICK ) <= 30000 ) // 30s 단위로 heart-bit 전송
        return;

    JIF( m_spSockObj );
#ifdef _IN_HOUSE_
    START_LOG( clog, L" Heart-bit " << m_iUnitUID << L"  :" << ::GetTickCount() - GetTick( KTRUser::HB_TICK ) );
#endif
    SetTick( KTRUser::HB_TICK );

    KncSecurity::KByteStream buff;
    buff.Append( sizeof(USHORT) + sizeof(char), sizeof(USHORT) );
    buff.Append( E_HEART_BEAT, sizeof(char) );

#ifdef _IN_HOUSE_
	LIF( m_spSockObj->SendData( ( const char* )buff.data(), buff.length() ) );
#else
	m_spSockObj->SendData( ( const char* )buff.data(), buff.length() );    
#endif
}

bool KTRUser::Init()
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkTRUser = shared_from_this();

	m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	m_spSockObj->SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
	m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
	m_spSockObj->SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );

	return true;
}

bool KTRUser::Connect( const char* szIP_, unsigned short usPort_, const UidType& iUnitUID_ )
{
	std::vector< std::pair<int,int> > vecOpt;
	vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // 사용중인 address를 재사용할 수 있게 허용. 
	vecOpt.push_back( std::make_pair( IPPROTO_TCP, TCP_NODELAY ) ); // Nagle option을 제거. 데이터를 모아서 보내지 않고 그 즉시 전송하도록 한다.

    _JIF( m_spSockObj, return false );
	_JIF( m_spSockObj->Connect( szIP_, usPort_, false, &vecOpt ), false );

	Begin();    // thread run

	// 접속 후 즉시 UID를 등록한다.
	m_iUnitUID = iUnitUID_;

	//{{AFX : 접속 성공후 클라이언트 에서 직접 UID 등록 패킷을 보내준다.
	//KncSecurity::KByteStream buff;
	//buff.Append( sizeof(USHORT) + sizeof(char) + sizeof(iUnitUID_), sizeof(USHORT) );
	//buff.Append( 0, sizeof(char) );
	//buff.Append( iUnitUID_ );
	//LIF( m_kSockObj.SendData( (const char*)buff.data(), buff.length() ) );
	//}}AFX

	START_LOG( clog, "Send UID : " << iUnitUID_ );

	return true;
}

bool KTRUser::SendID(unsigned short usEventID, bool bCompress_ /* = true  */)
{
    _JIF( m_spSockObj, return false );
	//if( m_bDestroyReserved ) return true;///< 종료가 예약되었을때 send 시도. 이런 경우가 다분하므로, true 인정.
	_JIF( m_spSockObj->IsConnected(), return false );    ///< 소켓이 유효하지 않음.

	KEvent kEvent;
	kEvent.SetData( 0, NULL, usEventID );

	if( kEvent.m_kbuff.GetLength() > KSkTRUser::MAXIMUM_PACKET_SIZE ) return false;

	if( bCompress_ ) kEvent.m_kbuff.Compress();

	// serialize - full event object
	KSerializer     ks;
	KSerBuffer      kbuff;
	ks.BeginWriting( &kbuff );
	ks.Put( kEvent );
	ks.EndWriting();

	KncSecurity::KByteStream    bsbuff;
	bsbuff.reserve( kbuff.GetLength() + sizeof(USHORT) );
	bsbuff.Assign( kbuff.GetLength() + sizeof(USHORT), sizeof(USHORT) );
	bsbuff.append( (byte*)kbuff.GetData(), kbuff.GetLength() );   // 실제 전송할 데이터 넣기.

	if( kEvent.m_usEventID != E_HEART_BEAT )
		dbg::clog << L"ⓢⓢ " << kEvent.GetIDStr() << L" - TR SEND" << dbg::endl;//END_LOG;

	return m_spSockObj->SendData( (const char*)bsbuff.data(), bsbuff.length() );
}

void KTRUser::OnSocketError()
{
    m_bDestroyReserved = true;
}

void KTRUser::Run()
{
	DWORD ret       = NULL;
	bool bLoop      = true;

	while( bLoop )
	{
		ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE, m_hEvents, false, 100 );     // 0.1s 간격

		switch( ret )
		{
		case EVENT_DISCONNECTED + WAIT_OBJECT_0:
			bLoop = false;
			break;
		case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
			{
                _JIF( m_spSockObj, break );
				m_spSockObj->OnIOCompleted( KOVERLAPPED::IO_RECV );
			}
			break;
		case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
			{
                _JIF( m_spSockObj, break );
				m_spSockObj->OnIOCompleted( KOVERLAPPED::IO_SEND );
			}
			break;
		case WAIT_TIMEOUT:
			break;
		}

		Tick();
	}
}

//void KTRUser::OnRecvCompleted( IN const char cRecvCnt_, IN KSerBuffer& buff_ )
void KTRUser::OnRecvCompleted( IN KSerBuffer& buffer_ )
{
    //Client 쪽은 시스템적 이벤트 식별이 필요없기 때문에 바로 보내줌.
    KSerializer ks;
    KEventPtr spEvent( new KEvent );
    ks.BeginReading( &buffer_ );
    ks.Get( *spEvent );
    ks.EndReading();

    spEvent->m_kbuff.UnCompress();

    KSerBuffer* pBuff = new KSerBuffer( spEvent->m_kbuff );

    dbg::clog << L"ⓡⓡ " << spEvent->GetIDStr() << L" - TRU Recv" << dbg::endl;//END_LOG;

    if( m_pSendGameMessage != NULL )
        m_pSendGameMessage(
        m_ServerMsgID,
        static_cast<WPARAM>(spEvent->m_usEventID),
        reinterpret_cast<LPARAM>(pBuff),
        false 
        );

    //{{AFX
    //switch( cRecvCnt_ ) 
    //{
    //case EVENTID_RELAYED:   // relay. 윈도우 메시지로 데이터를 던진다.
    //	if( IsWindow( m_hWnd ) )
    //	{
    //		// SendMessage하면 메세지가 처리될때까지 기다리지만, PostMessage이므로 큐잉만 한다.
    //		// cmd : WM_APP + Event ID
    //		// wParam : KTRUser 객체의 포인터
    //		// lParam : data 의 포인터

    //		//KSerBuffer* pBuff = new KSerBuffer( buff_ );
    //		//::PostMessage( m_hWnd, WM_APP + KUserEvent::EVENT_P2P_RELAY_ACK,
    //		//	reinterpret_cast<WPARAM>(this),
    //		//	reinterpret_cast<LPARAM>(pBuff) );
    //	}
    //	break;

    //default:
    //	if( cRecvCnt_ < 0 ) // 위에서 필터링 되지 않은 음수 영역. 문제가 있다.
    //	{
    //		START_LOG( cerr, "Invalide cRecvCnt field value. data size : " << buff_.GetLength() )
    //			<< BUILD_LOGc( cRecvCnt_ )
    //			<< END_LOG;
    //		return;
    //	}
    //}
    //}}AFX
}

void KTRUser::OnDestroy()
{
	m_bDestroyReserved = false;     // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

	SetEvent( m_hKillEvent );
    if( m_spSockObj )
    {
        m_spSockObj->CloseSocket();
    }
    else
    {
        START_LOG( cerr, L"소켓오브젝트 포인터 이상." )
            << END_LOG;
    }
}

//{{AFX
//bool KTRUser::Send( IN const std::vector<UidType> vecUID_, IN const void* pvData_, IN const size_t dwDataSize_ )
//{
//	//PROXY_ONLY;
//
//	if( vecUID_.empty() ) return false;
//	if( dwDataSize_ > KSkTRUser::MAXIMUM_PACKET_SIZE ) return false;
//
//	KncSecurity::KByteStream buff;
//	buff.Append( sizeof(USHORT)                 // 패킷 총 사이즈
//		+ sizeof(char)                          // recvCnt (전송대상자 수)
//		+ sizeof(m_iUnitUID) * vecUID_.size()	// 전송받은 상대의 UIDs...
//		+ dwDataSize_                           // 전송할 데이터
//		, sizeof(USHORT) );
//
//	buff.Append( vecUID_.size(), sizeof(char) );
//
//	std::vector<UidType>::const_iterator vit;
//	for( vit = vecUID_.begin(); vit != vecUID_.end(); vit++ )
//	{
//		buff.Append( *vit );
//	}
//
//	buff.append( (byte*)pvData_, dwDataSize_ );
//
//	return m_kSockObj.SendData( (const char*)buff.data(), buff.length() );
//}
//}}AFX