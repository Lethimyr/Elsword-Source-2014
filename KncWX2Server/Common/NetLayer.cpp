#include "NetLayer.h"
#include "ActorManager.h"
#include "Socket/IOCP.h"
#include "Socket/IOThread.h"
#include "Socket/Session.h"
#include "Socket/NetCommon.h"
#include "BaseServer.h"

#include <dbg/dbg.hpp>
#include <KncSecurity/KncSecurity.h>
#include <boost/bind.hpp>

NiImplementRootRTTI( KNetLayer );
ImplInstanceKeeping( KNetLayer );

KNetLayer::KNetLayer(void) : m_usUdpPort( 0 )
{
    m_kMaxTimeInfo.m_dwTerm	= 0;
    m_kMaxTimeInfo.m_cTime	= CTime::GetCurrentTime();
    // m_kMaxTimeInfo의 나머지 변수들은 생성자가 따로 있으므로 초기화해줄 필요 없다.

    m_usPortMaster			= 0;

	//{{ 2009. 10. 18  최육사	SequenceNum
	m_bCheckSequenceNum		= false;
	//}}
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	m_uiSessionType = KSession::SST_NONE;
#endif SERV_KEVENT_FROM
	//}}
}

KNetLayer::~KNetLayer(void)
{
}

ImplToStringW( KNetLayer )
{
    SiKIocp()->ToString( stm_ );

    KSocketObject::DumpSendQueueLog( stm_ );

    stm_ << std::endl;

    KSession::DumpMaxSendData( stm_ );

    stm_ << std::endl;

	//{{ 2009. 10. 18  최육사	SequenceNum	
	KSession::DumpPacketAuthInfo( stm_ );
	//}}

	stm_ << std::endl;

    START_TOSTRINGW 
        << TOSTRINGW( m_usPortMaster )
        << TOSTRINGW( m_usUdpPort );

    KIOThread::DumpExceptionInfo( stm_ );

    DumpMaxTimeInfo( stm_ );

    return stm_;
}

//{{ 2009. 12. 15  최육사	서버관리
void KNetLayer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
//}}

bool KNetLayer::Init()
{
	m_kAccepter.Init( m_usPortMaster, KNetLayer::OnAccept );

    // 050502. florist. 보안 코드 초기화 설정
    KncSecurity::InitSecurity();

    // init iocp
    SiKIocp()->Init( 4 );

    SiKIocp()->BeginThread();

	m_kAccepter.Begin();
	
	if( m_usUdpPort > 0 )
	{
		_JIF( m_kUdpEcho.Init( m_usUdpPort ), return false );

		m_kUdpEcho.Begin();
	}

    return true;
}

bool KNetLayer::InitNCUDP( int port )
{
	if( port > 0 )
	{
		_JIF( m_kLBSUdpEcho.Init( port ), return false );
		m_kLBSUdpEcho.Begin();
	}
	else
	{
		return false;
	}

	return true;
}

void KNetLayer::CloseAccepter()
{
    m_kUdpEcho.ShutDown();

//{{ robobeg : 2013-08-08
    // 센터서버 종료시 크래쉬를 방지하기 위한 수정
    // 이것만으로는 충분하지 않고, NetLayer, SimLayer 와 CnRoomManager 쓰레드 종료시점을 재조정해야 한다.
    // 엘소드 해외팀 최육사님에게 문의하시오
    //m_kLBSUdpEcho.End();
    m_kLBSUdpEcho.End( INFINITE );
//}} robobeg : 2013-08-08

    m_kAccepter.End();
}

void KNetLayer::ShutDown()
{
    SiKIocp()->EndThread();
	Sleep( 100 );
    KIocp::ReleaseInstance();

    KncSecurity::ReleaseSecurity();
}

#include <KncLua.h>
#include <lua_tinker.h>

void KNetLayer::RegToLua()
{
	lua_tinker::class_add<KNetLayer>( g_pLua, "KNetLayer" );
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetPort",				&KNetLayer::SetPort );
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetUdpEcho",				&KNetLayer::SetUdpEcho );
	lua_tinker::class_def<KNetLayer>( g_pLua, "ResetMaxData",			&KNetLayer::ResetMaxData );
	//{{ 2009. 10. 18  최육사	SequenceNum
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetCheckSequenceNum",	&KNetLayer::SetCheckSequenceNum_LUA );
	lua_tinker::class_def<KNetLayer>( g_pLua, "SetPacketAuthFailLimit",	&KNetLayer::SetPacketAuthFailLimit_LUA );	
	//}}
}

void KNetLayer::ResetMaxData()
{
    std::wostringstream strstm;

    DumpMaxTimeInfo( strstm );
    KSession::DumpMaxSendData( strstm );

    dbg::cout << strstm.str();
    
    ResetMaxTimeInfo();
    KSession::ResetMaxSendData();
}

void KNetLayer::SetCheckSequenceNum_LUA( bool bVal )
{
	KLocker lock( m_csCheckSequenceNum );
	
	m_bCheckSequenceNum = bVal;

	START_LOG( cout, L"Check Sequence Number : " << m_bCheckSequenceNum );
}

void KNetLayer::SetPacketAuthFailLimit_LUA( u_int uiCount )
{
	KLocker lock( m_csPacketAuthFailLimit );

	m_uiPacketAuthFailLimit = uiCount;

	START_LOG( cout, L"Packet Auth Fail Limit : " << m_uiPacketAuthFailLimit );
}

void KNetLayer::OnAccept( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
	// 070423. florist. 단순한 위임(delegate)만 발생.
	// KAccepter가 특정 객체에 상관없이 독립적으로 동작될 수 있도록 하는데 의의가 있음.
	KNetLayer::GetKObj()->OnAccept_( sock, sinRemote );
}

void KNetLayer::OnAccept_( SOCKET sock, const SOCKADDR_IN& sinRemote )
{
    KActorPtr spActor( m_spActorFactory->CreateActor() );
    if( !spActor )
    {
        START_LOG( cerr, L"Create Actor Failed" );
        CLOSE_SOCKET( sock );
        return;
    }

	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	spActor->SetSessionType( static_cast<KSession::SESSION_TYPE>(m_uiSessionType) );
#endif SERV_KEVENT_FROM
	//}}

	//{{ 2009. 10. 18  최육사	SequenceNum
	spActor->InitCheckSequenceNumInfo( IsCheckSequenceNum(), GetPacketAuthFailLimit() ); // IOCP에 등록하기 전에 설정합니다.
	//}}

    KSocketObjectPtr spSockObj = spActor->GetSockObj();
    JIF( spSockObj );

    spSockObj->SetSocketInfo( sock, sinRemote );

    LIF( SiKIocp()->AssociateSocket( spSockObj ) );

    spSockObj->InitRecv();
    spSockObj->OnAcceptConnection();

    DWORD dwElapTime = ::GetTickCount();  // 040428. 처리 소요시간을 구한다.

    KActorManager::GetKObj()->ReserveAdd( spActor );

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

void KNetLayer::DumpMaxTimeInfo( OUT std::wostream& stm_ ) const
{
    KLocker lock( m_kMaxTimeInfo.m_cs );

    stm_ << " -- Max accepter waiting time. NagleOn : " 
         << std::boolalpha << m_kAccepter.GetNagleAlgOn() << std::noboolalpha << std::endl
         << "  user IP : " << m_kMaxTimeInfo.m_strIP << std::endl
         << "  duration : " << m_kMaxTimeInfo.m_dwTerm << std::endl
         << "  executed time : " << (const wchar_t*)m_kMaxTimeInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S")) << std::endl;
}

void KNetLayer::ResetMaxTimeInfo()
{
    KLocker lock( m_kMaxTimeInfo.m_cs );

    m_kMaxTimeInfo.m_dwTerm = 0;
    m_kMaxTimeInfo.m_cTime = CTime::GetCurrentTime();
    m_kMaxTimeInfo.m_strIP = (const wchar_t*)m_kMaxTimeInfo.m_cTime.Format(_T("%Y-%m-%d %H:%M:%S"));
    m_kMaxTimeInfo.m_strIP.append( L" 에 데이터 초기화 되었습니다." );
}

