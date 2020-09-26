#include "TRServer.h"
//#include "TRIOCP.h"
#include "Socket/IOCP.h"
#include "Socket/NetCommon.h"

#include <KncLua.h>
#include <lua_tinker.h>
#include "lua/KLuaManager.h"
#include "BaseServer.h"
#include "TRWorkerThread.h"
#include "GameServer.h"

//////////////////////////////////////////////////////////////////////////
// For Dump
KTRServerDumpManager::KTRServerDumpManager()
{
	Reset();
	m_iEventCount = 0;
}

KTRServerDumpManager::~KTRServerDumpManager()
{
}

void KTRServerDumpManager::Tick()
{
	if( m_kDumpTimer.elapsed() < 60.0 )
		return;

	// 덤프
	DumpToLogFile();

	m_kDumpTimer.restart();
}

void KTRServerDumpManager::DumpToLogFile()
{
	// 프로파일러 덤프
	//SiKProfiler()->DumpToLogFile();

	START_LOG( cout, L"=== TCP RELAY SERVER ===" );

	//////////////////////////////////////////////////////////////////////////
	// 덤프 데이터 얻기
	//int iTot = 0;
	//int iReg = 0;
	int iEventCount = 0;
	u_int uiSendSize = 0;
	u_int uiSendCount = 0;
	GetSendInfo( uiSendSize, uiSendCount );
	//GetTRUserCount( iTot, iReg );
	GetEventCount( iEventCount );
	//int iQueueSize = GetQueueSize();
	//int iFieldSyncCount = GetKGameServer()->FlushFieldSyncCount();

	//////////////////////////////////////////////////////////////////////////
	// 덤프 출력
	dbg::cout << L"TRThreadManager에 푸쉬된 이벤트 수  : " << iEventCount << dbg::endl;
	//dbg::cout << L"TRWorkerThread가 가져가지 않은 남은 이벤트 : " << iQueueSize << dbg::endl;	

	//dbg::cout << L"전체 TRUser수 : " << iTot << dbg::endl;
	//dbg::cout << L"인증 TRUser수 : " << iReg << dbg::endl << dbg::endl;

	dbg::cout << L"Send   사이즈 : " << uiSendSize << dbg::endl;
	dbg::cout << L"Send   카운트 : " << uiSendCount << dbg::endl << dbg::endl;

	//dbg::cout << L"Field Sync카운트 : " << iFieldSyncCount << dbg::endl << dbg::endl;

	dbg::cout << L"===================================" << dbg::endl << dbg::endl;
	//////////////////////////////////////////////////////////////////////////
}

void KTRServerDumpManager::GetSendInfo( u_int& uiTotalSendSize, u_int& uiTotalSendCount )
{
	KLocker kLock( m_csSendInfo );

	uiTotalSendSize  = m_uiTotalSendSize;
	uiTotalSendCount = m_uiTotalSendCount;

	Reset();
}

void KTRServerDumpManager::UpdateSendPacketInfo( u_int uiSendSize )
{
	KLocker kLock( m_csSendInfo );

	m_uiTotalSendSize += uiSendSize;
	++m_uiTotalSendCount;
}
// For Dump
//////////////////////////////////////////////////////////////////////////


KThread* KTRThreadManager::CreateThread()
{
	return new KTRWorkerThread;
}

ImplPfID( KTRThreadManager, PI_NULL );


ImplementSingleton( KTRServer );

KTRServer::KTRServer()
{
	m_kMaxTimeInfo.m_dwTerm	= 0;
	m_kMaxTimeInfo.m_cTime	= CTime::GetCurrentTime();
	m_iTRWorkerThreadCount	= 6; // default
}

KTRServer::~KTRServer()
{
}

ImplToStringW( KTRServer )
{
	return stm_;
}

void KTRServer::RegToLua()
{
	lua_tinker::class_add<KTRServer>( g_pLua, "KTRServer" );
	lua_tinker::class_def<KTRServer>( g_pLua, "TRWorkerThreadCount",	&KTRServer::SetTRWorkerThreadCount );
	lua_tinker::class_def<KTRServer>( g_pLua, "dump",					&KTRServer::Dump );

	lua_tinker::decl( g_pLua, "TRServer", this );
}

bool KTRServer::Init()
{
#ifdef SERV_COUNTRY_JP
	m_kAccepter.Init( 12301, KTRServer::OnAccept, false );	// Nagle off
#elif defined(SERV_COUNTRY_ID)
	m_kAccepter.Init( 14301, KTRServer::OnAccept, false );	// Nagle off
#else //SERV_COUNTRY_JP
	m_kAccepter.Init( 9301, KTRServer::OnAccept, false );	// Nagle off
#endif //SERV_COUNTRY_JP

	//보안코드 초기화는 NetLayer에서 하기때문에 하지 않음.

	//TCP IOCP START.

	//{{ 2009. 3. 13  최육사	그냥 Iocp를 쓴다	
	//SiKTRIocp()->Init( 4 );
	//SiKTRIocp()->BeginThread();
	//START_LOG( cout2, L"=== TCP IOCP START..! ===" );
	//}}

    //TCP ACCEPTER START.
	if( m_kAccepter.Begin() == false )
	{
		START_LOG( cerr, L"=== TCP ACCEPTER START FAILED.! ===" )
			<< END_LOG;

		return false;
	}
	else
	{
		START_LOG( cout2, L"=== TCP ACCEPTER START..! ===" );
	}

	m_kThreadManager.Init( m_iTRWorkerThreadCount );
	m_kThreadManager.BeginThread();

	START_LOG( cout2, L"=== TCP RELAY WORKER THREAD CREATED.! ===" )
		<< BUILD_LOG( m_iTRWorkerThreadCount );

	return true;
}

void KTRServer::ShutDown()
{
	// accepter down
	m_kAccepter.End();	//가장먼저 새로운 접속을 막음.
	
	// iocp down
	//{{ 2009. 3. 13  최육사	그냥 iocp쓴다
	//SiKTRIocp()->EndThread();
	//Sleep(100);
	//KTRIocp::ReleaseInstance();
	//}}

	m_kThreadManager.EndThread();
}

void KTRServer::QueueingRelayData( const KEventPtr& spEvent_ )
{
	m_kThreadManager.QueueingEvent( spEvent_ );

	//////////////////////////////////////////////////////////////////////////
	// For Dump
	//m_kDumpManager.IncreaseEventCount();
	//////////////////////////////////////////////////////////////////////////
}

void KTRServer::OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	SiKTRServer()->OnAccept_( sock, sinRemote );
}

void KTRServer::OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	KTRUserPtr spTRUser = m_kTRUserManager.CreateTRUser();
	if( !spTRUser )
	{
		START_LOG( cerr, L"Create TRUser Failed" );
		CLOSE_SOCKET( sock );
		return;
	}

    KSocketObjectPtr spSockObj = spTRUser->GetSkTRUserPtr();
    JIF( spSockObj );

	spSockObj->SetSocketInfo( sock, sinRemote );

	//{{ 2009. 3. 13  최육사	그냥 iocp쓴다
	//LIF( SiKTRIocp()->AssociateSocket( spSockObj ) );
	LIF( SiKIocp()->AssociateSocket( spSockObj ) );
	//}}

	spSockObj->InitRecv();
	spSockObj->OnAcceptConnection();

	DWORD dwElapTime = ::GetTickCount();

	//매니져에 유저등록은 iocp를 통해 uid 등록이 왔을때 진행.
	//KTRUser::OOnRecvCompleted() 패킷이 오면 reg상태로 변경후 Tick()에서 등록.
	//KActorManager::GetKObj()->ReserveAdd( spActor );

	dwElapTime = ::GetTickCount() - dwElapTime;

	{   // locking scope
		KLocker lock( m_kMaxTimeInfo.m_cs );

		if( dwElapTime > m_kMaxTimeInfo.m_dwTerm )
		{
			m_kMaxTimeInfo.m_strIP    = KncUtil::toWideString(inet_ntoa(sinRemote.sin_addr));
			m_kMaxTimeInfo.m_cTime    = CTime::GetCurrentTime();
			m_kMaxTimeInfo.m_dwTerm   = dwElapTime;

			START_LOG( clog2, L"accepter 객체 생성 대기시간 갱신. Remote IP : " << m_kMaxTimeInfo.m_strIP )
				<< BUILD_LOG( m_kMaxTimeInfo.m_dwTerm );
		}
	}   // locking scope
}

void KTRServer::Tick()
{
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	BEGIN_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

	m_kTRUserManager.Tick();

	//////////////////////////////////////////////////////////////////////////
	// TRServer Dump
	//m_kDumpManager.Tick();
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	END_CHECK_TICK_LATENCY;
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
}



