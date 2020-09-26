#include <WinSock2.h>
#include "BRPayletterBillingManager.h"

#ifdef SERV_COUNTRY_BR

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "BRPayletterBilling.h"
#include "Simlayer.h"

#include "_BRGameProtocol.h"

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}

DWORD KBRPayletterBillingManager::ms_dwHeartBeatGap( 1 * 60 * 1000 );
DWORD KBRPayletterBillingManager::ms_dwConnectionCheckGap( 20 * 1000 );

ImplToStringW( KBRPayletterBillingManager )
{
	return stm_;
}

ImplPfID( KBRPayletterBillingManager, PI_GS_PUBLISHER_BILLING );
ImplementSingleton( KBRPayletterBillingManager );


KBRPayletterBillingManager::KBRPayletterBillingManager() :
m_bInitialized(false)
{

	m_kPayletterBillingInfo.m_strIP.clear();
	m_kPayletterBillingInfo.m_usPort = 0;
	m_kPayletterBillingInfo.m_iDomain = 0;
	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	m_sock = INVALID_SOCKET;
	m_bFirstConnectSucc = false;
	//}}
	m_dwLastHeartBeatTick = ::GetTickCount();
	m_dwLastConnectionCheckTick = ::GetTickCount();

	m_ulRecvCP = 0;

	m_bSocketConnect = false;

	m_spEvent.reset( CreateEvent( NULL, false, false, NULL ), CloseHandle );
}

KBRPayletterBillingManager::~KBRPayletterBillingManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KBRPayletterBillingManager::RegToLua()
{
	lua_tinker::class_add<KBRPayletterBillingManager>( g_pLua, "KPayletterManager" );
	lua_tinker::class_def<KBRPayletterBillingManager>( g_pLua, "InitPayletterBillingInfo",	&KBRPayletterBillingManager::InitPayletterBillingInfo );
	lua_tinker::class_def<KBRPayletterBillingManager>( g_pLua, "SetNumThread",			 &KBRPayletterBillingManager::Init );

	lua_tinker::decl( g_pLua, "PayletterBillingManager", this );
}

void KBRPayletterBillingManager::InitPayletterBillingInfo( const char* szNexonAuthIP, unsigned short usPort, int iDomain )
{
	m_kPayletterBillingInfo.m_strIP		= szNexonAuthIP;
	m_kPayletterBillingInfo.m_usPort	= usPort;
	m_kPayletterBillingInfo.m_iDomain	= iDomain;	
}


void KBRPayletterBillingManager::Tick()
{	
}

void KBRPayletterBillingManager::Init( int nThreadNum )
{
	m_ulRecvCP = 0;

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
	m_spThreadRecv = boost::shared_ptr< KTThread< KBRPayletterBillingManager > >
		( new KTThread< KBRPayletterBillingManager >( *this, &KBRPayletterBillingManager::Recv, 50 ) );

	m_spThreadSend = boost::shared_ptr< KTThread< KBRPayletterBillingManager > >
		( new KTThread< KBRPayletterBillingManager >( *this, &KBRPayletterBillingManager::Send, 100 ) );

	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	m_spThreadKeepConnect = boost::shared_ptr< KTThread< KBRPayletterBillingManager > >
		( new KTThread< KBRPayletterBillingManager >( *this, &KBRPayletterBillingManager::KeepConnectionThread, 1000 ) );
	//}}

	KThreadManager::Init( nThreadNum );	
}

	

KThread* KBRPayletterBillingManager::CreateThread()
{
	return new KBRPayletterBilling;
}

void KBRPayletterBillingManager::BeginThread()
{	
	// 빌링패킷 수신스레드
	if( m_spThreadRecv )
	{
		m_spThreadRecv->Begin();
	}

	// 빌링패킷 전송스레드
	if( m_spThreadSend )
	{
		m_spThreadSend->Begin();
	}

	KThreadManager::BeginThread();

	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	// 빌링서버와의 하트비트 체크 및 접속 유지를 위한 접속유지스레드
	if( m_spThreadKeepConnect )
	{
		// Thread를 시작하면서 빌링서버에 접속시도한다.
		m_spThreadKeepConnect->Begin();
	}
	//}}

}

void KBRPayletterBillingManager::EndThread()
{
	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	if( m_spThreadKeepConnect )
	{
		m_spThreadKeepConnect->End( 3000 );

		START_LOG( cout, L"브라질 페이레터 빌링 서버 접속 체크 스레드 종료!" );
	}
	//}}

	KThreadManager::EndThread();

	if( m_spThreadSend )
	{
		m_spThreadSend->End( 10000 );

		START_LOG( cout, L"브라질 페이레터 빌링 서버 Send 스레드 종료!" );
	}

	// recv 스레드 죽이기 전에 소켓을 닫자!
	CLOSE_SOCKET( m_sock );

	if( m_spThreadRecv )
	{
		m_spThreadRecv->End( 3000 );

		START_LOG( cout, L"브라질 페이레터 빌링 서버 Recv 스레드 종료!" );
	}
}

bool KBRPayletterBillingManager::Connect()
{
	//{{ [참고] Connect()함수가 호출되는 상황에서는 RecvThread에서 recv()함수가 항상 호출실패한다.
	//          따라서 m_ulRecvCP변수는 0으로 초기화 되더라도 문제되지 않음.
	m_ulRecvCP = 0;
	//}}

	m_sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성

	if( INVALID_SOCKET == m_sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		return false;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( m_kPayletterBillingInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kPayletterBillingInfo.m_usPort );

	int ret = ::connect( m_sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		//{{ 2009. 11. 21  최육사	서버간접속구조개선
		if( IsFirstConnectSucc() )
		{
			START_LOG( cerr, GET_WSA_MSG )
				<< BUILD_LOG( WSAGetLastError() )
				<< BUILD_LOG( m_kPayletterBillingInfo.m_strIP )
				<< BUILD_LOG( m_kPayletterBillingInfo.m_usPort )
				<< END_LOG;
		}
		//}}
		CLOSE_SOCKET( m_sock );
		return false;
	}
	
	// 지헌 : 소켓 연결 전에 recv, send 스래드가 작동하면서 소캣을 닫아버린다.
	//        국내 소스 그래도 쓴 것 같은데 왜 이러는지 모르겠음.
	//		  일단 소캣 연결 할 때 까지 다른 스래드 작동 안하도록 막아뒀다가 여기서 부는 방식으로.
	m_bSocketConnect = true;

	
	// KENX_BR_INITIALIZE_REQ를 큐잉하기 전에 false 로 만들어야 한다.
	// 지헌 : 테스트 - 이건 서버간 접속 개선 방식 때 사용 하는 것. 그러므로 지금은 true 로
	m_bInitialized = true;	// true 로 세팅

	if( !m_bInitialized )
	{
		CLOSE_SOCKET( m_sock );
		return false;
	}
	
	return true;
}

void KBRPayletterBillingManager::Recv()
{

	if( !IsConnected() )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock,
		m_cRecvBuffer + m_ulRecvCP,
		( int )( MAX_PACKET_SIZE_PLB - m_ulRecvCP),
		0 );

	if(ret == 330)
	{
		int a =0;
		a = 0;
	}

	START_LOG( clog, L"패킷 받음." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		//{{ 2009. 11. 21  최육사	서버간접속구조개선
		if( IsFirstConnectSucc() )
		{
			START_LOG( cerr, GET_WSA_MSG );
		}
		//}}

		CLOSE_SOCKET( m_sock );

		return;
	}

	if( ret == 0 )
	{
		START_LOG( cerr, L"소켓 연결이 끊어짐." )
			<< END_LOG;
		return;
	}

	
	m_ulRecvCP += ret;

	while( m_ulRecvCP >= 6 )
	{
		// 헤더(9) + 타입(1) = 6
		// 모든 패킷이 6바이트 이상임
		unsigned short usLength;
		::memcpy( &usLength, m_cRecvBuffer, sizeof( usLength ) );


		unsigned long ulTotalPacketSize = usLength;
		if( ulTotalPacketSize > MAX_PACKET_SIZE_PLB )
		{
			START_LOG( cerr, L"추출한 패킷 사이즈 이상." )
				<< BUILD_LOG( ulTotalPacketSize )
				<< BUILD_LOG( MAX_PACKET_SIZE_PLB )
				<< END_LOG;

			CLOSE_SOCKET( m_sock );

			//{{ 2010. 10. 13	최육사	넥슨 빌링 접속 종료
			std::wstring wstrReason = boost::str( boost::wformat( L"최대 패킷 사이즈 초과! MaxPacketSize : %d bytes" ) % ulTotalPacketSize );
			OnDisconnectBillingServer( wstrReason );
			//}}
			return;
		}

		if( ulTotalPacketSize > m_ulRecvCP )
		{
			return;
		}

		MakeEventFromReceivedPacket();

		if( m_ulRecvCP > ulTotalPacketSize )
		{
			::memmove( m_cRecvBuffer, m_cRecvBuffer + ulTotalPacketSize, m_ulRecvCP - ulTotalPacketSize );
		}
		m_ulRecvCP -= ulTotalPacketSize;

		
	}
	
}

void KBRPayletterBillingManager::Send()
{	
	if( !IsConnected() )
	{
		return;
	}
	
	int ret;
	char buf[MAX_PACKET_SIZE_PLB];
	KBRPayletterBillingPacketPtr spPacket;
	while( GetSendPacket( spPacket ) )
	{		
		if( !spPacket )
		{
			unsigned int uiSendQueueSize;
			KCSLOCK_BEGIN( m_kSendQueue )
				uiSendQueueSize = m_kSendQueue.size();
			KCSLOCK_END()

				START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( uiSendQueueSize )
				<< END_LOG;

			continue;
		}

		spPacket->WriteToBuffer( ( BYTE* )buf );
		DumpBuffer( ( BYTE* )buf, false );

		START_LOG( clog, L"PL_BILLING 보내는 패킷 타입 : " << spPacket->GetPacketType())
			<< END_LOG;

		char* cCurBufPos = buf;
		int iRemainedSendSize = spPacket->GetTotalLength();
		while( iRemainedSendSize > 0 )
		{
			ret = ::send( m_sock,
				cCurBufPos,
				iRemainedSendSize,
				0 );

			if( ret == SOCKET_ERROR )
			{
				START_LOG( cwarn, GET_WSA_MSG );
				CLOSE_SOCKET( m_sock );
				return;
			}

			iRemainedSendSize -= ret;
			cCurBufPos += ret;
		}
	}
}

//{{ 2009. 11. 21  최육사	서버간접속구조개선
void KBRPayletterBillingManager::KeepConnectionThread()
{
	if( !IsFirstConnectSucc() )
	{
		// 최초 빌링서버 접속 시도. connect()함수 호출중에는 Thread Block
		if( Connect() )
		{
			FirstConnectSucc();
		}

		return;
	}

	CheckConnection();

}
//}}

void KBRPayletterBillingManager::KeepConnection()
{
	if( ::GetTickCount() - m_dwLastHeartBeatTick < ms_dwHeartBeatGap )
	{
		return;
	}

	m_dwLastHeartBeatTick = ::GetTickCount();

	//{{ 2008. 5. 30  최육사  넥슨빌링 초기화 과정
	if( !m_bInitialized  ||  !IsConnected() )
		return;
	//}}

}


void KBRPayletterBillingManager::CheckConnection()
{
	if( ::GetTickCount() - m_dwLastConnectionCheckTick < ms_dwConnectionCheckGap )
	{
		return;
	}

	m_dwLastConnectionCheckTick = ::GetTickCount();

	if( !IsConnected() )
	{
		// 초기화 되지 않은 상태로 설정!
		m_bInitialized = false;

		m_bSocketConnect = false;

		Connect();
	}
}

void KBRPayletterBillingManager::HandleInitializeAck( bool bSucceed )
{
	m_bInitialized = bSucceed;
	SetEvent( m_spEvent.get() );
}


void KBRPayletterBillingManager::QueueingSendPacket( const KBRPayletterBillingPacketPtr& spPacket )
{	
	JIF( spPacket );
	
	KCSLOCK_BEGIN( m_kSendQueue )
		m_kSendQueue.push( spPacket );
	KCSLOCK_END()
}
bool KBRPayletterBillingManager::IsConnected()
{
	return ( m_sock != INVALID_SOCKET );
}


void KBRPayletterBillingManager::OnDisconnectBillingServer( IN const std::wstring& wstrReason )
{
	// 최초 접속이 아직 성공하지 않았기 때문에 접속 종료 리포팅을 하지 않는다.
	if( IsFirstConnectSucc() == false )
		return;

	//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
	{
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= L"Nexon Billing";
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( m_kPayletterBillingInfo.m_strIP );
		kNot.m_wstrReason			= wstrReason;
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
	}
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	{
		KE_DISCONNECT_SERVER_REPORT_NOT kNot;
		kNot.m_cType = KMornitoringManager::ET_DISCONNECT_GAME_BILLING;
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( 0, NULL, E_DISCONNECT_SERVER_REPORT_NOT, kNot );
		KBaseServer::GetKObj()->QueueingEvent( spEvent );
	}
#endif SERV_MORNITORING
	//}}

	START_LOG( cout, L"[빌링] Payletter 빌링 서버 접속 종료!" )
		<< BUILD_LOG( wstrReason );
}

bool KBRPayletterBillingManager::GetSendPacket( KBRPayletterBillingPacketPtr& spPacket )
{
	bool bNotEmpty = false;
	KCSLOCK_BEGIN( m_kSendQueue )
		bNotEmpty = !(m_kSendQueue.empty());
		if(  bNotEmpty == true )
		{
			spPacket = m_kSendQueue.front();
			m_kSendQueue.pop();
		}
	KCSLOCK_END()

	return bNotEmpty;
}

void KBRPayletterBillingManager::MakeEventFromReceivedPacket()
{
	DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KBRPayletterBillingPacket kPacket;
	if( !kPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer ) )
	{
		START_LOG( cerr, L"버퍼에서 읽기 실패." )
			<< END_LOG;

		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		return;
	}

	// 받은 패킷 종류
	START_LOG( clog, L"PL_BILLING 받은 패킷 타입 : " << kPacket.GetPacketType())
		<< END_LOG;
	
	switch( kPacket.GetPacketType() )
	{
#   undef  _ENUM
#   define _ENUM( name, id ) \
	case id: \
		{ \
		K##EPL_BR_##name##_ACK kPacketAck; \
		kPacket.Read( kPacketAck ); \
		KEventPtr spEvent( new KEvent ); \
		spEvent->SetData( PI_GS_PUBLISHER_BILLING, NULL, EPL_BR_##name##_ACK, kPacketAck ); \
		QueueingEvent( spEvent ); \
		} \
		break;
#   include "BRPayletterBilling_def.h"
	default:
		START_LOG( cerr, L"패킷 타입이 이상함." )
			<< BUILD_LOG( kPacket.GetPacketType() )
			<< END_LOG;
		DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
		break;
	}
}

void KBRPayletterBillingManager::DumpBuffer( BYTE* buffer, bool bError )
{
	JIF( buffer );
	JIF( sizeof( buffer ) >= 4 );

	unsigned short ulLength;
	::memcpy( &ulLength, buffer, sizeof( ulLength ) );

	//////////////////////////////////////////////////////////////////////////
	//START_LOG( clog2, L"[테스트로그] 패킷 사이즈 출력!" )
	//	<< BUILD_LOG( ulLength );	
	//////////////////////////////////////////////////////////////////////////	

	ulLength = ::ntohl( ulLength );
	unsigned short ulTotalPacketSize = ulLength + 5;

	LIF( ulTotalPacketSize <= MAX_PACKET_SIZE_PLB );
	ulTotalPacketSize = std::min< int >( ulTotalPacketSize, MAX_PACKET_SIZE_PLB );

	char szBuffer[MAX_PACKET_SIZE_PLB * 3 + 1];
	char szByte[4];

	for( unsigned short i = 0; i < ulTotalPacketSize; i++ )
	{
		if( ( int )buffer[i] >= 16 )
		{
			::itoa( ( int )buffer[i], szByte, 16 );
			szBuffer[i * 3] = szByte[0];
			szBuffer[i * 3 + 1] = szByte[1];
			szBuffer[i * 3 + 2] = ' ';
		}
		else
		{
			::itoa( ( int )buffer[i], szByte, 16 );
			szBuffer[i * 3] = '0';
			szBuffer[i * 3 + 1] = szByte[0];
			szBuffer[i * 3 + 2] = ' ';
		}
	}
	szBuffer[ulTotalPacketSize * 3] = '\0';

	std::string strBuffer = szBuffer;

	if( bError )
	{
		START_LOG( cerr2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
	else
	{
		START_LOG( clog2, L"BUFFER : ")
			<< BUILD_LOG( strBuffer );
	}
}

void KBRPayletterBillingManager::InsertUniqueKeyUserUID( UidType iUniqueKey, UidType iUserUID )
{
	if( iUniqueKey <= 0 || iUserUID <= 0 )
	{
		START_LOG( cerr, L"UniqueKey 또는 유저 UID 이상 " )
			<< BUILD_LOG( iUniqueKey )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;

		return;
	}

	KCSLOCK_BEGIN( m_mapUniqueKeyUserUID )
	std::map< UidType, UidType >::iterator mit;
	mit = m_mapUniqueKeyUserUID.find( iUniqueKey );
	if( mit == m_mapUniqueKeyUserUID.end() )
	{
		// 없으면 삽입하고 있으면 덮어 쓴다.
		m_mapUniqueKeyUserUID[iUniqueKey] = iUserUID;
	}
	else
	{
		START_LOG( cerr, L"퍼블리셔 유니크 키에 해당하는 UserUID 가 이미 등록되어 있다." )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second )
			<< BUILD_LOG( iUniqueKey )
			<< BUILD_LOG( iUserUID )
			<< END_LOG;
	}
	KCSLOCK_END()
}

void KBRPayletterBillingManager::DeleteUniqueKeyUserUID( UidType iUniqueKey )
{
	KCSLOCK_BEGIN( m_mapUniqueKeyUserUID )
	std::map< UidType, UidType >::iterator mit;
	mit = m_mapUniqueKeyUserUID.find( iUniqueKey );
	if( mit != m_mapUniqueKeyUserUID.end() )
	{
		m_mapUniqueKeyUserUID.erase( mit );
	}
	else
	{
		START_LOG( cerr, L"퍼블리셔 유니크 키에 해당하는 UserUID 가 등록되어 있지 않다." )
			<< BUILD_LOG( iUniqueKey )
			<< END_LOG;
	}
	KCSLOCK_END()
}

UidType KBRPayletterBillingManager::GetUniqueKeyUserUID( UidType iUniqueKey )
{
	UidType iUserUID = 0;

	KCSLOCK_BEGIN( m_mapUniqueKeyUserUID )
	std::map< UidType, UidType >::iterator mit;
	mit = m_mapUniqueKeyUserUID.find( iUniqueKey );
	if( mit != m_mapUniqueKeyUserUID.end() )
	{
		iUserUID = mit->second;
	}
	else
	{
		START_LOG( cerr, L"퍼블리셔 유니크 키에 해당하는 UserUID 가 등록되어 있지 않다." )
			<< BUILD_LOG( iUniqueKey )
			<< END_LOG;
	}
	KCSLOCK_END()

		return iUserUID;
}

//////////////////////////////////////////////////////////////////////////
// 구매하기 
void KBRPayletterBillingManager::InsertBuyPacket( IN KEBILL_BUY_PRODUCT_REQ kPacket_, OUT bool& bRet_ )
{
	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find( kPacket_.m_uiPublisherUID );
		if( m_mapBuyPacket.end() == itor )
		{
			KPayletterBuyPacket kBuyPacket(CTime::GetCurrentTime(), kPacket_);
			m_mapBuyPacket.insert(std::make_pair(kPacket_.m_uiPublisherUID, kBuyPacket));
			bRet_ = true;
		}
		else
		{
			CTime ctDeleteTime = itor->second.m_ctTime + CTimeSpan(0,0,PLBE_PACKET_DELETE_MIN,0);
			if( CTime::GetCurrentTime() < ctDeleteTime )
			{
				START_LOG( cerr, L"[빌링] 보관중인 구매 패킷이 이미 있다. 처리 지연 중..." )
					<< BUILD_LOG( kPacket_.m_uiPublisherUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;
				bRet_ = false;
			}
			else
			{
				// 처리 지연 한계 시간이 지났다. 패킷을 지우고 새로 처리하자. 지운 패킷 정보는 로그로 남길까?
				START_LOG( cout, L"[빌링] 보관중인 구매 패킷이 지우고 새로 넣는다" )
					<< BUILD_LOG( kPacket_.m_uiPublisherUID )
					<< BUILD_LOG( kPacket_.m_iServerGroupID )
					<< END_LOG;

				KPayletterBuyPacket kBuyPacket(CTime::GetCurrentTime(), kPacket_);
				itor->second = kBuyPacket;

				bRet_ = true;
			}
		}
	KCSLOCK_END()
}

void KBRPayletterBillingManager::GetBuyPacket( IN const UidType iUniqueKey, OUT KEBILL_BUY_PRODUCT_REQ& kPacket_, OUT bool& bRet_ )
{
	bool bRet = false;
	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find(iUniqueKey);
		if(itor != m_mapBuyPacket.end())
		{
			kPacket_ = (KEBILL_BUY_PRODUCT_REQ)itor->second.m_kPacket;	
			m_mapBuyPacket.erase(itor);

			bRet_ = true;
		}
		else
		{
			// 구매 패킷 보내기 전에 넣어뒀던 패킷이 사라졌다? 발생하면 안됨
			START_LOG( cerr, L"[빌링] 구매 패킷 응답이 왔는데, 보관해뒀던 패킷이 사라졌다? 발생하면 안됨" )
				<< BUILD_LOG( iUniqueKey )
				<< END_LOG;

			bRet_ = false;
		}
	KCSLOCK_END()
}

void KBRPayletterBillingManager::EraseBuyPacket( IN const UidType iUniqueKey )
{
	KEBILL_BUY_PRODUCT_REQ tempPacket;

	KCSLOCK_BEGIN( m_mapBuyPacket )
		MAP_BUYPACKET_ITOR itor = m_mapBuyPacket.find(iUniqueKey);

		if(itor != m_mapBuyPacket.end())
		{
			m_mapBuyPacket.erase(itor);
		}
		else
		{
			START_LOG( cerr, L"[빌링] 구매 패킷을 삭제 하려는데, 패킷이 없다" )
				<< BUILD_LOG( iUniqueKey )
				<< END_LOG;
		}
	KCSLOCK_END()
}

#endif //SERV_COUNTRY_BR