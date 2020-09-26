#include "StdAfx.h"

#include "Session.h"


//#ifdef KNC_USE_IOCP
#include "IOCP.h"
//#endif

void KSkSession::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {                    
        START_LOG( clog, L"closed by remote machine" );
        OnSocketError();
        return;
    }

    USHORT          usPacketLen = 0;

    m_ovlRecv.m_dwLeft += dwTransfered_;

    // 패킷 길이 진행량보다 크다면 계속 새 패킷 가능성을 체크한다.
    while( m_ovlRecv.m_dwLeft >= sizeof(usPacketLen) )
    {
        // 패킷 길이를 알아낸다.
        memcpy( &usPacketLen, m_ovlRecv.m_pBuffer, sizeof(usPacketLen) );

        // 버퍼에 받아둔 데이터로 완전한 packet이 가능한 경우 다음으로 진행한다.
        if( m_ovlRecv.m_dwLeft >= usPacketLen )
        {
            KSessionPtr spSession = GetSessionPtr();
            if( spSession )
            {
                // 길이를 제외한 데이터만 버퍼에 담는다.
                KncSecurity::KSecureBuffer kSecBuff( spSession->m_nSPIndex, m_ovlRecv.m_pBuffer + sizeof( usPacketLen ),
                    usPacketLen - sizeof( usPacketLen ) );
                //std::cout << "RECV " << kSecBuff << std::endl;
				
				//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
				if( !kSecBuff.IsAuthentic_Local( spSession->GetSADatabase() ) ) // 클라이언트에서는 IsAuthentic()를 써서 sequence number를 체크 안함.
#else
                if( !kSecBuff.IsAuthentic_notRWM( true ) ) // 클라이언트에서는 IsAuthentic()를 써서 sequence number를 체크 안함.
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
					//}}
                {
					static int iCount = 0;
					++iCount;

					if( iCount >= 10 )
					{						
						spSession->OnAuthenticFailed();
						iCount = 0;
					}
				}
				else
				{
					// 인증 및 복호화 처리를 마치고 순수 payload만을 얻어낸다.
					KncSecurity::KByteStream payload;
					//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
					JIF( kSecBuff.GetPayload_Local( payload, spSession->GetSADatabase() ) );
#else
					JIF( kSecBuff.GetPayload( payload ) );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
					//}}

					spSession->OnRecvCompleted( payload );
				}
            }
            else
            {
                START_LOG( clog, L"세션 객체가 소멸되었음." );
            }

            // buffer management
            m_ovlRecv.m_dwLeft -= usPacketLen;
            memmove(&m_ovlRecv.m_pBuffer[0], &m_ovlRecv.m_pBuffer[usPacketLen], m_ovlRecv.m_dwLeft );
        }
        else
        {
            break;     // 온전한 패킷을 구성하기에 충분한 데이터가 없음. loop를 빠져나간다.
        }
    }

    // recv function call
    LIF( InitRecv() );
}

void KSkSession::OnSocketError()
{
    KSessionPtr spSession = GetSessionPtr();
    if( spSession )
    {
        spSession->OnSocketError();
    }
    else
    {
        START_LOG( clog, L"Session 객체가 이미 지워짐." )
            << END_LOG;
    }
}

void KSkSession::OnAcceptConnection()
{
    KSessionPtr spSession = GetSessionPtr();
    if( spSession )
    {
        spSession->OnAcceptConnection();
    }
    else
    {
        START_LOG( clog, L"Session 객체가 이미 지워짐." )
            << END_LOG;
    }
}

KSessionPtr KSkSession::GetSessionPtr()
{
    KSessionPtr spSession = m_pkSession.lock();

    if( spSession )
    {
        return spSession;
    }
    else
    {
        return KSessionPtr();
    }
}

NiImplementRTTI( KSession, KPerformer );

#define PROXY_ONLY cassert( m_bIsProxy )(m_strName).fatal()
#define UNPROXY_ONLY cassert( !m_bIsProxy )(m_strName).fatal()

KSession::KMaxPacketInfo KSession::ms_kMaxPacketInfo = {
	KncCriticalSection(), L"초기상태", L"초기상태", 0, 0, CTime::GetCurrentTime()
};

KSession::KSession( bool bIsProxy ) 
:
    m_spSockObj         ( new KSkSession ),
    m_bIsProxy          ( bIsProxy ),
    m_bUseIocp          ( true ),
    m_bDestroyReserved  ( false ),
    m_nSPIndex          ( 0 ),
    m_bAuthKeyRecved    ( false ),
    m_bCheckHBTick      ( true )
{
    m_dwHBTick = GetTickCount();

    m_hEvents[EVENT_RECV_COMPLETED] = NULL;
    m_hEvents[EVENT_SEND_COMPLETED] = NULL;

	//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	m_pSADatabase = NULL;
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}
}

KSession::~KSession(void)
{
	if( m_spSockObj )
	{
		m_spSockObj->CloseSocket();
	}
	else
	{
		START_LOG( cerr, L"소켓오브젝트 포인터 이상." )
			<< END_LOG;
	}

    if( !m_bUseIocp )
    {
        End();  // thread use

		::CloseHandle( m_hEvents[EVENT_RECV_COMPLETED] );
		::CloseHandle( m_hEvents[EVENT_SEND_COMPLETED] );
	}

	if( m_nSPIndex != 0 )
	{
		START_LOG( clog, L"SA 삭제. Name : " << m_strName << L", SPI : " << m_nSPIndex );
		//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
#ifdef  X2OPTIMIZE_SESSION_THREAD_SAFETY
        KncSecurity::KSADatabase* pSADatabase = static_cast<KncSecurity::KSADatabase*>( InterlockedExchangePointer( (void**) &m_pSADatabase, NULL ) );
        if ( pSADatabase != NULL )
        {
			pSADatabase->Delete( m_nSPIndex );
			delete pSADatabase;
			pSADatabase = NULL;
        }
#else   X2OPTIMIZE_SESSION_THREAD_SAFETY
		if( m_pSADatabase != NULL )
		{
			m_pSADatabase->Delete( m_nSPIndex );
			delete m_pSADatabase;
			m_pSADatabase = NULL;
		}
#endif  X2OPTIMIZE_SESSION_THREAD_SAFETY
#else
		KncSecurity::GetSADB().Delete( m_nSPIndex );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
		//}}
		m_nSPIndex = 0;
	}
}

ImplToStringW( KSession )
{
    return START_TOSTRING_PARENTW( KPerformer )
        << TOSTRINGW( m_dwHBTick )
        << TOSTRINGW( m_bDestroyReserved )
        << TOSTRINGW( m_nSPIndex )
        << TOSTRINGW( m_bIsProxy )
        << TOSTRINGW( m_bUseIocp )
        << TOSTRINGW( m_bCheckHBTick );
}

bool KSession::Init( bool bUseIocp_  )
{
    _JIF( m_spSockObj, return false );
    m_spSockObj->m_pkSession = GetThisPtr< KSession >();

    if( m_bIsProxy )
    {
        m_bUseIocp = bUseIocp_;

        if( !m_bUseIocp )
        {
            m_hEvents[EVENT_RECV_COMPLETED] = CreateEvent( NULL, false, false, NULL );
            m_spSockObj->SetIOEvent( KOVERLAPPED::IO_RECV, m_hEvents[EVENT_RECV_COMPLETED] );
            m_hEvents[EVENT_SEND_COMPLETED] = CreateEvent( NULL, false, false, NULL );
            m_spSockObj->SetIOEvent( KOVERLAPPED::IO_SEND, m_hEvents[EVENT_SEND_COMPLETED] );
        }

        return true;
    }
    else
    {
        LIF( bUseIocp_ );
        return true;
    }
}

bool KSession::Begin()  // KThread의 이벤트 객체를 배열에 보관해, WaitMultipleObject를 사용한다.
{
    if( KThread::Begin() )
    {
        m_hEvents[EVENT_DISCONNECTED] = m_hKillEvent;
        return true;
    }

    return false;
}

void KSession::Tick()
{
    KPerformer::Tick();

    //////////////////////////////////////////////////////////////////////////
    // 종료 처리
    if( m_bDestroyReserved )    // 종료가 예약된 상태라면 다시한번 종료처리를 시도한다.
    {
        OnDestroy();            // derived class call
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    // check heart beat
    DWORD dwCurrentTick = ::GetTickCount();
    if( m_bIsProxy )
    {
        if( IsConnected() == false )    // UserProxy의 경우, 연결이 없어도 살아있거나 혹은 재접속 할 수도 있다 (Robot)
            return;

        if( dwCurrentTick - m_dwHBTick <= 15000 ) // 15s 단위로 heart-beat 전송
            return;

        if( !m_bAuthKeyRecved )   // 아직 보안키를 받기 이전이다. 이땐 heart-beat를 보내지 않는다.
            return;

        //START_LOG( clog, L" Heart-beat " << m_strName << L"  :" << ::GetTickCount() - m_dwHBTick );
        m_dwHBTick = dwCurrentTick;

		KEvent kEvent;
		kEvent.m_usEventID = E_HEART_BEAT;
		SendPacket( kEvent );
        
    }
    else    // un-proxy heart-beat check
    {
        if( m_bCheckHBTick && dwCurrentTick - m_dwHBTick > 60000 )  // 60s 단위로 heart-bit check
        {
            START_LOG( cwarn, L"Reguard " << m_strName 
                << L" as a Zombie. (tick: " 
                << dwCurrentTick - m_dwHBTick 
                << L" )" );

            ReserveDestroy();
            //SetDisconnectReason( KStatistics::eSIColDR_Heart_Beat );
            //OnDestroy();
        }
    }
}

void KSession::ReserveDestroy()
{
    KEventPtr spEvent( new KEvent );
    spEvent->m_usEventID = E_RESERVE_DESTROY;
//    spEvent->m_kDestPerformer.m_dwPerformerID = GetPfID();

    QueueingEvent( spEvent );
}

bool KSession::CheckExceedRefCount( int nCount_ )
{
    // 입력받은 수와 같거나 적은 경우는 문제를 일으키지 않으므로, true를 리턴한다.
    // false가 리턴된 경우, 로그출력 등의 추가작업을 위해선 child 클래스에서 상속받아 구현한다.
    return ( GetRefCount() <= nCount_ );
}

bool KSession::SendPacket( IN const KEvent& kEvent )
{
    //if( m_bDestroyReserved ) return true;       ///< 종료가 예약되었을때 send 시도. 이런 경우가 다분하므로, true 인정.
    _JIF( m_spSockObj, return false );
    _JIF( m_spSockObj->IsConnected(), return false );    ///< 소켓이 유효하지 않음.

    // serialize - full event object
    KSerializer ks;
    KSerBuffer kbuff;
    ks.BeginWriting( &kbuff );
    ks.Put( kEvent );
    ks.EndWriting();

    // 암호화한다.
    KncSecurity::KByteStream bsbuff;
    KncSecurity::KSecureBuffer kSecBuff( m_nSPIndex );

    bsbuff.assign( (byte*)kbuff.GetData(), kbuff.GetLength() );		// 실제 전송할 데이터 넣기.

	//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
#ifdef  X2OPTIMIZE_SESSION_THREAD_SAFETY
	_JIF( kSecBuff.Create_Local( bsbuff, GetSADatabase()
        ), return false );				// 암호화 및 인증 절차 처리.
#else   X2OPTIMIZE_SESSION_THREAD_SAFETY
	_JIF( kSecBuff.Create_Local( bsbuff, m_pSADatabase
        ), return false );				// 암호화 및 인증 절차 처리.
#endif  X2OPTIMIZE_SESSION_THREAD_SAFETY

#else
	_JIF( kSecBuff.Create( bsbuff ), return false );				// 암호화 및 인증 절차 처리.
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}

    // bsbuff를 재활용해, 암호화된 데이터 앞에 총 길이를 덧붙인 byte stream을 만든다.
    bsbuff.clear();
    bsbuff.reserve( kSecBuff.GetSize() + sizeof(USHORT) );   // 패킷 총 사이즈[4] + 전송할 데이터[가변]
    bsbuff.Assign( kSecBuff.GetSize() + sizeof(USHORT), sizeof(USHORT) );
    bsbuff.append( (byte*)kSecBuff.GetDataPtr(), kSecBuff.GetSize() );

    {   // locking scope

        KLocker lock2( ms_kMaxPacketInfo.m_cs );

        if( ms_kMaxPacketInfo.m_nPacketSize < bsbuff.length() )
        {
            ms_kMaxPacketInfo.m_strName     = m_strName;
            ms_kMaxPacketInfo.m_strDataDesc = kEvent.GetIDStr();
            ms_kMaxPacketInfo.m_cTime       = CTime::GetCurrentTime();
            ms_kMaxPacketInfo.m_nDataSize   = kbuff.GetLength();
            ms_kMaxPacketInfo.m_nPacketSize = bsbuff.length();
            START_LOG( clog2, L"패킷 최대사이즈 갱신. Name : " << m_strName )
                << BUILD_LOG( ms_kMaxPacketInfo.m_strDataDesc )
                << BUILD_LOG( ms_kMaxPacketInfo.m_nDataSize )
                << BUILD_LOG( ms_kMaxPacketInfo.m_nPacketSize );
        }

    }   // locking scope

    return m_spSockObj->SendData( ( const char* )bsbuff.data(), bsbuff.length() );
}

bool KSession::SendID( DWORD dwDestPI_, UidType nTo_, UidType anTrace[], unsigned short usEventID_ )
{
    // check valid state
    KEvent kEvent;
    kEvent.SetData( dwDestPI_, anTrace, usEventID_ );
    LIF( kEvent.m_kDestPerformer.AddUID( nTo_ ) );

    // send packet
    if( ! KSession::SendPacket( kEvent ) )
        return false;

    if( g_bIsSERVICE == false )
        dbg::clog << L"ⓢ " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << dbg::endl;//END_LOG;

    return true;
}

void KSession::DumpMaxSendData( std::wostream& stm_ )
{
    KLocker lock( ms_kMaxPacketInfo.m_cs );

    stm_ << L" -- Max size Data sending -- " << std::endl
        << L"  user name   : " << ms_kMaxPacketInfo.m_strName << std::endl
        << L"  data desc   : " << ms_kMaxPacketInfo.m_strDataDesc << std::endl
        << L"  data size   : " << ms_kMaxPacketInfo.m_nDataSize 
        << L" (" << ms_kMaxPacketInfo.m_nDataSize / 1024.0f << " Kb)" << std::endl
        << L"  packet size : " << ms_kMaxPacketInfo.m_nPacketSize
        << L" (" << ms_kMaxPacketInfo.m_nPacketSize / 1024.0f << " Kb)" << std::endl
        << L"  send time   : " << (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format( KNC_TIME_FORMAT ) << std::endl;
}

void KSession::ResetMaxSendData()
{
    KLocker lock( ms_kMaxPacketInfo.m_cs );

    ms_kMaxPacketInfo.m_strDataDesc.clear();
    ms_kMaxPacketInfo.m_nDataSize = 0;
    ms_kMaxPacketInfo.m_nPacketSize = 0;
    ms_kMaxPacketInfo.m_cTime = CTime::GetCurrentTime();
    ms_kMaxPacketInfo.m_strName = (LPCWSTR)ms_kMaxPacketInfo.m_cTime.Format( KNC_TIME_FORMAT );
    ms_kMaxPacketInfo.m_strName.append( L" 에 데이터 초기화 되었습니다." );
}

bool KSession::Connect( const char* szIP_, unsigned short usPort_ )
{
    PROXY_ONLY;
    _JIF( m_spSockObj, return false );

    std::vector< std::pair<int,int> > vecOpt;
    vecOpt.push_back( std::make_pair( SOL_SOCKET, SO_REUSEADDR ) ); // 사용중인 address를 재사용할 수 있게 허용. 

    START_LOG( clog, L"Connect To : " << szIP_ << L":" << usPort_ );

    _JIF( m_spSockObj->Connect( szIP_, usPort_, m_bUseIocp, &vecOpt ), return false );

    if( !m_bUseIocp )   // no use iocp
    {
        Begin();    // thread run
    }

    // 050520. 키를 받아 설정할 때까지 대기한다.
    DWORD dwWaitStartTick = ::GetTickCount();

    Sleep( 20 );

    // 대기시간을 초과하지 않았고 받은 메세지가 아무것도 없는 동안은
    while( ::GetTickCount() - dwWaitStartTick < SECURE_KEY_WAITING_TIME && !m_bAuthKeyRecved )
        Sleep( 20 );    // 계속 기다린다.

    if( !m_bAuthKeyRecved )  // 키를 받지 못했다면
    {
        START_LOG( cerr, L"서버에서 보안 키를 받지 못했습니다. name:" << m_strName << L", 대기시간:" 
            << ::GetTickCount() - dwWaitStartTick );
        ReserveDestroy();
        return false;
    }

    return true;
}

void KSession::OnAcceptConnection()
{
	UNPROXY_ONLY;

	const KncSecurity::KSecurityAssociation* pkSA = NULL;
	SpiType nSpiNew = 0;
	//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	pkSA = GetSADatabase()->CreateNewSA( nSpiNew );
#else
	pkSA = KncSecurity::GetSADB().CreateNewSA( nSpiNew );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
	//}}

	KSerializer ks;
	KEvent      kEvent;

	kEvent.m_usEventID  = E_ACCEPT_CONNECTION_NOT;
	ks.BeginWriting( &kEvent.m_kbuff );
	ks.Put( nSpiNew );
	ks.Put( *pkSA );
	ks.EndWriting();

	SendPacket( kEvent );    // 상수키로 보낸 후,
	m_nSPIndex = nSpiNew;   // 새 키로 변경해준다.
}

void KSession::OnRecvCompleted( IN const KncSecurity::KByteStream& payload_ )
{
    KSerializer     ks;
    KSerBuffer      kbuff;

    // payload를 KEvent 형식으로 전환한다.
    kbuff.Write( payload_.data(), payload_.length() );
    KEventPtr spEvent( new KEvent );
    ks.BeginReading( &kbuff );
    ks.Get( *spEvent );
    ks.EndReading();

    // 압축 된 경우만 자동으로 해제한다.
    spEvent->m_kbuff.UnCompress();

    switch( spEvent->m_usEventID )
    {
    case E_HEART_BEAT:                  // heart bit filtering - no queueing
        UNPROXY_ONLY;
        break;
    case E_ACCEPT_CONNECTION_NOT:
        {
            PROXY_ONLY;

            KncSecurity::KSecurityAssociation kSA;

            ks.BeginReading( &spEvent->m_kbuff );
            ks.Get( m_nSPIndex );
            ks.Get( kSA );
            ks.EndReading();

			//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
			GetSADatabase()->Insert( m_nSPIndex, kSA );
#else
			KncSecurity::GetSADB().Insert( m_nSPIndex, kSA );
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
			//}}
            START_LOG( clog, m_strName << L", 보안키 설정. spi:" << m_nSPIndex  );

            m_bAuthKeyRecved = true;
        }
        break;
    default:
        QueueingEvent( spEvent );
    }

    // renew m_dwHBRecvTick
    if( !m_bIsProxy )
        m_dwHBTick = ::GetTickCount();
}

void KSession::Run()
{
    PROXY_ONLY;

    DWORD ret       = NULL;
    bool bLoop      = true;

    while( bLoop )
    {
        ret = ::WaitForMultipleObjects( EVENT_MAX_VALUE,
            m_hEvents, 
            false, 
            100 );     // 0.1s 간격

        switch( ret )
        {
        case EVENT_DISCONNECTED + WAIT_OBJECT_0:    // 스레드 종료
            bLoop = false;
            break;
        case EVENT_RECV_COMPLETED + WAIT_OBJECT_0:
            {
#ifdef _IN_HOUSE_
                _JIF( m_spSockObj, break );
#else
				if( !m_spSockObj )
					break;
#endif
                m_spSockObj->OnIOCompleted( KOVERLAPPED::IO_RECV );
            }
            break;
        case EVENT_SEND_COMPLETED + WAIT_OBJECT_0:
            {
#ifdef _IN_HOUSE_
				_JIF( m_spSockObj, break );
#else
				if( !m_spSockObj )
					break;
#endif
                m_spSockObj->OnIOCompleted( KOVERLAPPED::IO_SEND );
            }
            break;
        case WAIT_TIMEOUT:
            break;

        case WAIT_FAILED:
            START_LOG( cerr, GET_WSA_MSG );
            break;
        }

        Tick(); // 050214. IOCP를 사용하지 않을 경우, Tick을 내부에서 처리한다.

    } // while

}

void KSession::OnSocketError()
{
    ReserveDestroy();
}

void KSession::OnDestroy()
{
	m_bDestroyReserved = false;     // 이전에 종료가 예약 되었든 아니든, 한번 Destory 호출이 성공하면 없어진다.

	// 050323. floris. IOCP 사용 안할 경우, ReserveDestroy 호출하면 Thread 멈춘다.
	if( !m_bUseIocp ) 
	{
		SetEvent( m_hKillEvent );
	}
    else
    {
        DWORD dwKey;
        if( GetKey( dwKey ) )
        {
            SiKIocp()->DeleteCompKey( dwKey );
        }
        else
        {
            START_LOG( cerr, L"소켓오브젝트의 키 값 얻기 실패." )
                << BUILD_LOG( GetUID() )
                << BUILD_LOG( GetName() )
                << END_LOG;
        }
    }
}

void KSession::OnAuthenticFailed()
{
	// Session클래스에서는 아무동작을 하지 않습니다.
}

//{{ 2012. 03. 19	김민성		KSADatabase 를 Session 의 멤버로 이동(클라이언트만)
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
KncSecurity::KSADatabase* KSession::GetSADatabase()
{
#ifdef  X2OPTIMIZE_SESSION_THREAD_SAFETY
	if( m_pSADatabase == NULL )
	{
        KncSecurity::KSADatabase* pSADatabase = new KncSecurity::KSADatabase();
        KncSecurity::KSADatabase* pSADatabaseRet = static_cast<KncSecurity::KSADatabase*>
            ( InterlockedCompareExchangePointer( (volatile PVOID*) &m_pSADatabase, pSADatabase, NULL ) );
        if ( pSADatabaseRet == NULL )
        {
            return pSADatabase;
        }
        else
        {
            delete pSADatabase;
            pSADatabase = NULL;
            return pSADatabaseRet;
        }
	}
	return m_pSADatabase;
#else   X2OPTIMIZE_SESSION_THREAD_SAFETY
	if( m_pSADatabase == NULL )
	{
		m_pSADatabase = new KncSecurity::KSADatabase();
		return m_pSADatabase;
	}
	return m_pSADatabase;
#endif  X2OPTIMIZE_SESSION_THREAD_SAFETY
}
#endif ADD_COLLECT_CLIENT_INFO_PROTOCOL
//}}