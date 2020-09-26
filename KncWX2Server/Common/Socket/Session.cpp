#include "Session.h"
#include "Event.h"
#include "Thread/Locker.h"
#include "dbg/dbg.hpp"
#include "NetCommon.h"
#include "Statistics.h"
#include <time.h>   // localtime

//#ifndef KNC_DONT_USE_IOCP
#include "IOCP.h"
//#endif

void KSkSession::OnRecvCompleted( DWORD dwTransfered_ )
{
    if( dwTransfered_ == 0 )
    {
        START_LOG( clog, L"closed by remote machine" );

        SetDisconnectReason( KStatistics::eSIColDR_Session_Close );
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

				// 육사 : dos공격이 들어온다면 결국 에러로그를 공격량 만큼 찍어낼것이기 때문에 서버가 위험해지는건 마찬가지다.
                if( !kSecBuff.IsAuthentic_notRWM( spSession->IsCheckSequenceNum() ) )
                {
                    START_LOG( cwarn, L"packet authentication failed." )
                        << BUILD_LOG( kSecBuff.GetSize() )
                        << BUILD_LOG( spSession->GetName() );

					// 패킷 인증이 실패하면 실패카운트를 올린다.
					spSession->IncreasePacketAuthFailCnt();

					//{{ 2009. 10. 21  최육사	패킷 인증 실패가 특정갯수 이상 발생시 접속 차단
					if( spSession->GetPacketAuthFailCnt() > spSession->GetPacketAuthFailLimit() )
					{
						START_LOG( clog, L"Find Traffic Attacker!" );

						SetDisconnectReason( KStatistics::eSIColDR_Traffic_Attack );
						OnSocketError();

						// 여기서 return해야만 InitRecv()를 호출하지 않으므로 IOCP완료 이벤트가 더이상 오지 않는다.
						return;
					}
					//}}
                }
				else
				{
					// 인증 및 복호화 처리를 마치고 순수 payload만을 얻어낸다.
					KncSecurity::KByteStream payload;
					JIF( kSecBuff.GetPayload_notRWM( payload, false ) ); // 위에서 이미 IsAuthentic()을 호출했으므로 두번째 인자는 false를 전달!

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
		//{{ 2012. 02. 01	최육사	recv buffer full
#ifdef SERV_CHECK_RECV_BUFFER_FULL
		// 수신된 패킷의 길이가 Max Packet Size를 넘으면 종료시키자!
		else if( usPacketLen > MAX_PACKET_SIZE )
		{
			START_LOG( cout, L"[Disconnect] Recv Buffer Full!" )
				<< BUILD_LOG( usPacketLen );

			SetDisconnectReason( KStatistics::eSIColDR_RecvBufferFull );
			OnSocketError();
			return;
		}
#endif SERV_CHECK_RECV_BUFFER_FULL
		//}}
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

//{{ 2009. 10. 18  최육사	SequenceNum
KSession::KPacketAuthInfo KSession::ms_kPacketAuthInfo = {
	KncCriticalSection(), L"실패패킷없음", 0, 0, CTime::GetCurrentTime()
};
//}}

KSession::KSession( bool bIsProxy ) 
:
    m_spSockObj         ( new KSkSession ),
    m_bIsProxy          ( bIsProxy ),
    m_bUseIocp          ( true ),
    m_bDestroyReserved  ( false ),
    m_nSPIndex          ( 0 ),
    m_bAuthKeyRecved    ( false ),
    m_bCheckHBTick      ( true ),
	//{{ 2009. 10. 18  최육사	Sequence Number
	m_bCheckSequenceNum ( false ),
	m_uiPacketAuthFailLimit( PACKET_AUTH_FAIL_DEFAULT ),
	m_uiPacketAuthFailCnt ( 0 ),
	//}}
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	m_eSessionType		( SST_NONE ),
#endif SERV_KEVENT_FROM
	//}}
	//{{ 2011. 10. 31	최육사	하트비트 오류 원인 분석
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	m_dwHBTickDump		( 0 ),
	m_dwCurrentTickDump	( 0 ),
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
	//}}
	//{{ 2009. 8. 10  최육사	send buffer full 원인 찾기		
	m_usLastSendEventID ( EVENT_EVENTID_SENTINEL )
	//}}
{
    m_dwHBTick = GetTickCount();

    m_hEvents[EVENT_RECV_COMPLETED] = NULL;
    m_hEvents[EVENT_SEND_COMPLETED] = NULL;
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
        KncSecurity::GetSADB().Delete( m_nSPIndex );
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

bool KSession::Init( bool bUseIocp_ )
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

	// [참고] -- m_dwHBTick 변수가 동기화 처리가 안되서 잘못된 값이 들어갈수 있음.

	// KSession::Tick() 함수에서 ::GetTickCount()를 호출하여 이전의 저장되었던 m_dwHBTick값과의 차로 하트비트 체크를 하는데
	// 아래에 ::GetTickCount()가 호출되고 나서 m_dwHBTick값에 새로운 ::GetTickCount()값이 들어가버리면
	// 과거의 Tick값이 아니라 더 최근의 Tick값이 되버려서 결국 마이너스 값이 나오게 된다.

	//{{ 2013. 01. 15	최육사	세션 하트비트 체크 오류 수정
#ifdef SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
	const DWORD dwOldHBTick = m_dwHBTick;
#endif SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
	//}}

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
		//{{ 2013. 01. 15	최육사	세션 하트비트 체크 오류 수정
		//////////////////////////////////////////////////////////////////////////
#ifdef SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
		//////////////////////////////////////////////////////////////////////////
		if( m_bCheckHBTick && dwCurrentTick - dwOldHBTick > 60000 )  // 60s 단위로 heart-bit check
		{
			//{{ 2011. 10. 31	최육사	하트비트 오류 원인 분석
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			m_dwHBTickDump = dwOldHBTick;
			m_dwCurrentTickDump = dwCurrentTick;
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			//}}

			START_LOG( cwarn, L"Reguard " << m_strName 
				<< L" as a Zombie. (tick: " 
				<< dwCurrentTick - dwOldHBTick 
				<< L" )" );

			SetDisconnectReason( KStatistics::eSIColDR_Heart_Beat );
			ReserveDestroy();
			//OnDestroy();
		}
		//////////////////////////////////////////////////////////////////////////
#else
		//////////////////////////////////////////////////////////////////////////
		if( m_bCheckHBTick && dwCurrentTick - m_dwHBTick > 60000 )  // 60s 단위로 heart-bit check
		{
			//{{ 2011. 10. 31	최육사	하트비트 오류 원인 분석
#ifdef SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			m_dwHBTickDump = m_dwHBTick;
			m_dwCurrentTickDump = dwCurrentTick;
#endif SERV_UNKNOWN_HEART_BEAT_ERROR_CHECK
			//}}

			START_LOG( cwarn, L"Reguard " << m_strName 
				<< L" as a Zombie. (tick: " 
				<< dwCurrentTick - m_dwHBTick 
				<< L" )" );

			SetDisconnectReason( KStatistics::eSIColDR_Heart_Beat );
			ReserveDestroy();
			//OnDestroy();
		}
		//////////////////////////////////////////////////////////////////////////
#endif SERV_SESSION_HEART_BEAT_CHECK_BUG_FIX
		//////////////////////////////////////////////////////////////////////////
		//}}
    }
}

void KSession::ReserveDestroy()
{
	KEventPtr spEvent( new KEvent );
    spEvent->m_usEventID = E_RESERVE_DESTROY;
    spEvent->m_kDestPerformer.m_dwPerformerID = GetPfID();

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
    _JIF( kSecBuff.Create_notRWM( bsbuff ), return false );			// 암호화 및 인증 절차 처리.

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

    if( kEvent.m_usEventID != E_HEART_BEAT )
        dbg::clog << L"ⓢ " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

	//{{ 2009. 8. 10  최육사	Send Buffer Full사유찾기
	m_usLastSendEventID = kEvent.m_usEventID;
	//}}
	//{{ 2012. 02. 01	최육사	recv buffer full
#ifdef SERV_CHECK_RECV_BUFFER_FULL
	if( bsbuff.length() > MAX_PACKET_SIZE )
	{
		START_LOG( cout, L"[check] max packet size를 넘어서는 패킷이 전송되었습니다. recv buffer full이 예상됩니다!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kEvent.GetIDStr() )
			<< BUILD_LOG( bsbuff.length() );
		// 아무런 처리를 하지 않고 로그만 찍습니다!
	}
#endif SERV_CHECK_RECV_BUFFER_FULL
	//}}

#ifdef SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL// 작업날짜: 2013-09-23	// 박세훈
	const bool bResult = m_spSockObj->SendData( ( const char* )bsbuff.data(), bsbuff.length() );

	if( m_bIsProxy == true )
	{
		if( 30 < m_queEventID.size() )
		{
			m_queEventID.pop();
		}
		m_queEventID.push( TYPE_SEND_BUFFER_FULL_LOG::value_type( kEvent.m_usEventID, CTime::GetCurrentTime() ) );

		if( ( bResult == false ) && ( GetDisconnectReason() == KStatistics::eSIColDR_SendBufferFull ) )
		{
			while( m_queEventID.empty() == false )
			{
				const TYPE_PAIR_USHORT_CTIME& sInfo = m_queEventID.front();
				START_LOG( cerr, L"Send Buffer Full. Latest Packet ID : " << (LPCTSTR)sInfo.second.Format( KNC_TIME_FORMAT ) << L", " << KEvent::GetIDStr( sInfo.first ) );
				m_queEventID.pop();
			}
		}
	}

	return bResult;
#else // SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL
	return m_spSockObj->SendData( ( const char* )bsbuff.data(), bsbuff.length() );
#endif // SERV_DISCONNECT_LOG_FOR_SENDBUFFERFULL
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

    dbg::clog << L"ⓢ " << kEvent.GetIDStr() << L" (name:" << m_strName << L")" << END_LOG;

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

//{{ 2009. 10. 18  최육사	SequenceNum
void KSession::DumpPacketAuthInfo( std::wostream& stm_ )
{
	KLocker lock( ms_kPacketAuthInfo.m_cs );

	stm_ << L" -- Packet Auth Fail Max Cnt -- " << std::endl
		<< L"  User Name   : " << ms_kPacketAuthInfo.m_strName << std::endl
		<< L"  Fail Count  : " << ms_kPacketAuthInfo.m_uiMaxFailCnt << std::endl
		<< L"  Total Fail  : " << ms_kPacketAuthInfo.m_uiTotalFailCnt << std::endl
		<< L"  Reg Time    : " << (LPCWSTR)ms_kPacketAuthInfo.m_cRegTime.Format( KNC_TIME_FORMAT ) << std::endl;
}
//}}

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

	// 2009-10-16  최육사 comment
	// 1. 클라이언트의 접속이 성공하면 호출됨
	// 2. 패킷 보안을 위한 KSecurityAssociation객체를 여기서 생성함 (Session당 하나씩있음)
	// 3. 클라이언트로 KSecurityAssociation객체 정보를 보내어 보안인증을 동기화시키려는 목적임

    const KncSecurity::KSecurityAssociation* pkSA = NULL;
    SpiType nSpiNew = 0;
    pkSA = KncSecurity::GetSADB().CreateNewSA( nSpiNew );

    KSerializer ks;	
	KEvent kEvent;

    kEvent.m_usEventID  = E_ACCEPT_CONNECTION_NOT;
    ks.BeginWriting( &kEvent.m_kbuff );
    ks.Put( nSpiNew );
    ks.Put( *pkSA );
    ks.EndWriting();

    SendPacket( kEvent );   // 상수키로 보낸 후,
    m_nSPIndex = nSpiNew;   // 새 키로 변경해준다.
}

void KSession::OnRecvCompleted( IN const KncSecurity::KByteStream& payload_ )
{
    KSerializer ks;
    KSerBuffer kbuff;

    // payload를 KEvent 형식으로 전환한다.
    kbuff.Write( payload_.data(), payload_.length() );
	KEventPtr spEvent( new KEvent );
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	if( GetSessionType() == SST_CLIENT )
	{
		// 게임 클라이언트로 부터 도착한 패킷이라고 표시해두자!
		spEvent->SetFromType( KEvent::EFT_CLIENT );
	}
#endif SERV_KEVENT_FROM
	//}}
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

			// 2009-10-16  최육사 comment
			// 1. 클라이언트의 서버접속이 성공하면 가장 처음 서버로부터 받는 패킷임
			// 2. 패킷 보안을 위한 KSecurityAssociation객체를 서버로부터 받아서 SADB에 저장함
			// 3. 서버로부터 도착한 SPIndex는 쓰이지 않음
			// 4. 클라이언트에서는 중복되지않는 SPIndex를 랜덤하게 생성하여 사용 ( KSecurityAssociation는 서버로부터 받은것 그대로 써야함 )

            KncSecurity::KSecurityAssociation kSA;

            ks.BeginReading( &spEvent->m_kbuff );
            ks.Get( m_nSPIndex );
            ks.Get( kSA );
            ks.EndReading();

            KncSecurity::GetSADB().Insert( m_nSPIndex, kSA );
            START_LOG( clog, m_strName << L", 보안키 설정. spi:" << m_nSPIndex  );			

            m_bAuthKeyRecved = true;
        }
        break;
		//{{ 2012. 07. 18	최육사		재접속 로직
#ifdef SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
	case E_CHECK_DDOS_GUARD_REQ:
		{
			//////////////////////////////////////////////////////////////////////////
			START_LOG( cout, L"[테스트로그] DDOS Guard를 통과했습니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetIPStr() );
			//////////////////////////////////////////////////////////////////////////

			KEvent kEvent;
			kEvent.SetData( PC_USER, NULL, E_CHECK_DDOS_GUARD_ACK, char() );
			SendPacket( kEvent );
		}
		break;
#endif SERV_RECONNECT_LOGIC_FOR_DDOS_SOLUTION
		//}}
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
		//{{ 2009. 10. 18  최육사	SequenceNum
		const unsigned int uiPacketAuthFailCnt = GetPacketAuthFailCnt();
		if( uiPacketAuthFailCnt > 0 )
		{
			KLocker lock( ms_kPacketAuthInfo.m_cs );

			ms_kPacketAuthInfo.m_uiTotalFailCnt += uiPacketAuthFailCnt; // 전체 fail count를 구함.

			if( ms_kPacketAuthInfo.m_uiMaxFailCnt < uiPacketAuthFailCnt ) // 최대 fail count를 구함.
			{
				ms_kPacketAuthInfo.m_strName	  = m_strName;
				ms_kPacketAuthInfo.m_cRegTime	  = CTime::GetCurrentTime();
				ms_kPacketAuthInfo.m_uiMaxFailCnt = uiPacketAuthFailCnt;
				START_LOG( clog2, L"Packet Auth Fail 갱신. Name : " << m_strName )
					<< BUILD_LOG( ms_kPacketAuthInfo.m_uiMaxFailCnt );
			}
		}
		//}}

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

void KSession::SetP2PIP( const std::wstring& wstrIP )
{ 
    m_wstrIP = wstrIP; 
}

//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
	void			KSession::SetP2PInternalIP( const std::wstring& wstrIP )
    { m_wstrInternalIP = wstrIP; }
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP