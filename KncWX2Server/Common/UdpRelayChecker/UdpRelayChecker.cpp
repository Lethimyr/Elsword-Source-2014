#include <winsock2.h>
#include "UdpRelayChecker.h"
#include <dbg/dbg.hpp>
#include "BaseServer.h"
#include "NetError.h"
#include "socket/NetCommon.h"
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "KAutoPath.h"



//{{ 2012. 04. 23	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER

#define CLASS_TYPE  KUdpRelayChecker
ImplPfID( KUdpRelayChecker, PI_NULL );
ImplInstanceKeeping( KUdpRelayChecker );


KUdpRelayChecker::KUdpRelayChecker(void) :
m_bCheckerOn( false ),
m_bPrintLog( false ),
m_sUdpRelayCheckerPort( 0 ),
m_fRelayCheckPacketGap( 5.0 ), // 디폴트는 5초간격
m_fDBUpdateGap( 60.0 * 5 ) // 디폴트는 5분간격
{
}

KUdpRelayChecker::~KUdpRelayChecker(void)
{
}

ImplToStringW( KUdpRelayChecker )
{
	stm_	<< L"----------[ Udp Relay Checker ]----------" << std::endl
		<< TOSTRINGW( m_bCheckerOn )
		<< TOSTRINGW( m_bPrintLog )
		<< TOSTRINGW( m_sUdpRelayCheckerPort )
		<< TOSTRINGW( m_fRelayCheckPacketGap )
		<< TOSTRINGW( m_fDBUpdateGap )
		<< TOSTRINGW( m_mapUdpRelayServerInfo.size() )
		;

	return stm_;
}

void KUdpRelayChecker::OpenScriptFile()
{
	KAutoPath		kAutoPath;
	std::string		strFile;

	strFile = "UdpRelayCheckerConfig.lua";
	kAutoPath.GetPullPath( strFile );

	if( LUA_DOFILE( g_pLua, strFile.c_str() ) == 0 )
	{
		START_LOG( cout, "UdpRelayChecker스크립트 파일 로딩 성공!" );
	}
	else
	{
		START_LOG( cout, "UdpRelayChecker스크립트 파일 로딩 실패!" )
			<< BUILD_LOG( KncUtil::toWideString( lua_tostring( g_pLua, -1 ) ) );
	}
}

void KUdpRelayChecker::RegToLua()
{
	lua_tinker::class_add<KUdpRelayChecker>( g_pLua, "KUdpRelayChecker" );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerOn",			&KUdpRelayChecker::SetUdpRelayCheckerOn_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "AddUdpRelayCheckerOnServerClass", &KUdpRelayChecker::AddUdpRelayCheckerOnServerClass_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerPrintLog",		&KUdpRelayChecker::SetUdpRelayCheckerPrintLog_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerPort",			&KUdpRelayChecker::SetUdpRelayCheckerPort_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetUdpRelayCheckerTimeGap",		&KUdpRelayChecker::SetUdpRelayCheckerTimeGap_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "SetDBUpdateTimeGap",				&KUdpRelayChecker::SetDBUpdateTimeGap_LUA );
	lua_tinker::class_def<KUdpRelayChecker>( g_pLua, "dump",							&KUdpRelayChecker::Dump );

	lua_tinker::decl( g_pLua, "UdpRelayChecker", this );
}

void KUdpRelayChecker::SetUdpRelayCheckerOn_LUA( bool bOn )
{
	m_bCheckerOn = bOn;

	if( m_bCheckerOn )
	{
		START_LOG( cout, L"UDP Relay Checker를 켭니다!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() );
	}
	else
	{
		START_LOG( cout, L"UDP Relay Checker를 끕니다!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() );
	}
}

void KUdpRelayChecker::AddUdpRelayCheckerOnServerClass_LUA( int iServerClass )
{
	switch( iServerClass )
	{
	case KBaseServer::ESC_CHANNEL:
		{
			START_LOG( cout, L"UDP Relay Checker를 채널서버에서 켭니다!" );
		}
		break;

	case KBaseServer::ESC_GAME:
		{
			START_LOG( cout, L"UDP Relay Checker를 게임서버에서 켭니다!" );
		}
		break;

	default:
		{
			START_LOG( cout, L"해당 서버는 UDP Relay Checker를 켜도록 허용되지 않았습니다." )
				<< BUILD_LOG( iServerClass );
		}
		return;
	}

	m_setInitServerClass.insert( iServerClass );
}

void KUdpRelayChecker::SetUdpRelayCheckerPrintLog_LUA( bool bOn )
{
	m_bPrintLog = bOn;

	START_LOG( cout, L"UDP Relay Checker Print Log : " << m_bPrintLog );
}

void KUdpRelayChecker::SetUdpRelayCheckerPort_LUA( short sPort )
{
    m_sUdpRelayCheckerPort = sPort;

	START_LOG( cout, L"UDP Relay Checker Port : " << sPort );
}

void KUdpRelayChecker::SetUdpRelayCheckerTimeGap_LUA( double fGap )
{
	m_fRelayCheckPacketGap = fGap;

	START_LOG( cout, L"UDP Relay Check Packet Time Gap : " << fGap );
}

void KUdpRelayChecker::SetDBUpdateTimeGap_LUA( double fGap )
{
	m_fDBUpdateGap = fGap;

	START_LOG( cout, L"UDP Relay Check DB Update Time Gap : " << fGap );
}

void KUdpRelayChecker::Run()
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}
}

void KUdpRelayChecker::Init()
{
	// 스크립트 로딩
	RegToLua();
	OpenScriptFile();

	// 동작이 허용된 서버인지 확인!
	if( m_setInitServerClass.find( KBaseServer::GetKObj()->GetServerClass() ) == m_setInitServerClass.end() )
		return;

	// UdpCheckerSession을 위해 UDP세션 오픈!
	if( m_spUdpCheckerSession != NULL )
	{
		START_LOG( cerr, L"이미 UdpCheckerSession이 생성이 되어 있네? 이럴순 없어!" )
			<< END_LOG;
	}
	else
	{
		__int64 iUID = LLONG_MAX;
		KncUid::SetServerGroupID( iUID, KBaseServer::GetKObj()->GetServerGroupID() );

		m_spUdpCheckerSession = boost::shared_ptr<KUdpCheckerSession>( new KUdpCheckerSession( m_sUdpRelayCheckerPort ) );
		m_spUdpCheckerSession->Init( this, iUID );

		// udp checker session recv thread 시작!
		m_spUdpCheckerSession->Begin();
	}

	// UdpRelayChecker Tick을 위해 thread시작!
	Begin();

	// DB로부터 체크해야할 센터서버 리스트를 받아오자!	
	SendToLogDB( DBE_GET_CENTER_SERVER_LIST_REQ );
}

void KUdpRelayChecker::ShutDown()
{
	// udp checker session를 먼저 종료!
	if( IS_NOT_NULL( m_spUdpCheckerSession ) )
	{
		m_spUdpCheckerSession->ShutDown();
	}	

	// Tick thread종료!
	End();
}

void KUdpRelayChecker::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}

void KUdpRelayChecker::Tick()
{
	KPerformer::Tick();

	// Tick돌면서 UDP체크를 실시한다.
	CheckUdpServer();
}

void KUdpRelayChecker::CheckUdpServer()
{
	if( !m_bCheckerOn )
		return;

	KDBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT kPacketNot;

	std::map< std::wstring, SUdpRelayServerInfo >::iterator mit;
	for( mit = m_mapUdpRelayServerInfo.begin(); mit != m_mapUdpRelayServerInfo.end(); ++mit )
	{
		SUdpRelayServerInfo& sUdpRelayServerInfo = mit->second;

		//////////////////////////////////////////////////////////////////////////
		// Relay Check Packet 전송 로직 : 5초마다 체크!
		if( sUdpRelayServerInfo.m_tRelayCheckTimer.elapsed() > m_fRelayCheckPacketGap )
		{
			sUdpRelayServerInfo.m_tRelayCheckTimer.restart();

			// UDP 릴레이 서버와 연결된 상태인지 확인! 안되어있다면 연결 시도!
			if( sUdpRelayServerInfo.m_bRelayConnected == false )
			{
				m_spUdpCheckerSession->ConnectRelayServer( sUdpRelayServerInfo.m_wstrServerIP, sUdpRelayServerInfo.m_sServerPort );

				if( IsPrintLog() )
				{
					START_LOG( cout, L"[알림] UDP Relay Checker Send Connect Packet!" )
						<< BUILD_LOG( sUdpRelayServerInfo.m_wstrServerIP );
				}
				continue;
			}

			// 전송 체크
			if( sUdpRelayServerInfo.m_bSendRelayPacket )
			{
				if( IsPrintLog() )
				{
					START_LOG( cout, L"[알림] 5초동안 릴레이 패킷이 돌아오지 않았다! 다시 UDP 접속 시도를 하자!" )
						<< BUILD_LOG( sUdpRelayServerInfo.m_wstrServerIP );
				}

				// 30초동안 릴레이 패킷이 돌아오지 않았다. peer가 끊어진걸까? 다시 재접속처리를 해보자!
				sUdpRelayServerInfo.m_bRelayConnected = false;
				sUdpRelayServerInfo.m_bSendRelayPacket = false;
				continue;
			}

			// UDP 릴레이 서버와 연결된 상태라면 테스트 패킷을 보내자!
			KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT m_kTestPacket;
			KSerBuffer buff;
			Serialize( &buff, &m_kTestPacket );
			m_spUdpCheckerSession->SetRelayIPPort( sUdpRelayServerInfo.m_wstrServerIP, sUdpRelayServerInfo.m_sServerPort );

			// 패킷 보내기 직전 TickCount
			//sUdpRelayServerInfo.m_dwLastTickCount = ::GetTickCount();
			sUdpRelayServerInfo.m_dwLastTimeGetTime = ::timeGetTime();
			//m_kQPCounter.GetQPCounter( sUdpRelayServerInfo.m_qwLastQPCounter );

			// 패킷 전송!
			m_spUdpCheckerSession->BroadCast( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT, (char*)buff.GetData(), buff.GetLength() );

			if( IsPrintLog() )
			{
				START_LOG( cout, L"[알림] UDP Relay Server Check Packet 전송!" )
					<< BUILD_LOG( sUdpRelayServerInfo.m_wstrServerIP );
			}

			// 전송 체크!
			sUdpRelayServerInfo.m_bSendRelayPacket = true;
		}

		//////////////////////////////////////////////////////////////////////////
		// 통계 정보 DB업데이트! : 5분마다 체크!
		if( sUdpRelayServerInfo.m_tDBUpdateTimer.elapsed() > m_fDBUpdateGap )
		{
			sUdpRelayServerInfo.m_tDBUpdateTimer.restart();

			CTime tCurTime = CTime::GetCurrentTime();

			KUdpRelayCheckLog kInfo;
			kInfo.m_iServerUID			= sUdpRelayServerInfo.m_iServerUID;
			kInfo.m_wstrServerIP		= sUdpRelayServerInfo.m_wstrServerIP;
			if( sUdpRelayServerInfo.m_dwCountTimeGetTime != 0 )
			{
				kInfo.m_dwAvgDelayTime	= sUdpRelayServerInfo.m_dwTotalTimeGetTime / sUdpRelayServerInfo.m_dwCountTimeGetTime;
			}
			kInfo.m_dwMaxDelayTime		= sUdpRelayServerInfo.m_dwMaxTimeGetTime;
			kInfo.m_wstrRegDate			= (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
			kPacketNot.m_vecUdpRelayCheckLog.push_back( kInfo );

			//////////////////////////////////////////////////////////////////////////
			START_LOG( cout, L"5분동안 수집된 UDP Relay Check 정보 정리!" )
				<< BUILD_LOG( kInfo.m_wstrServerIP )
				<< BUILD_LOG( sUdpRelayServerInfo.m_dwTotalTimeGetTime )
				<< BUILD_LOG( sUdpRelayServerInfo.m_dwCountTimeGetTime )
				<< BUILD_LOG( kInfo.m_dwAvgDelayTime )
				<< BUILD_LOG( kInfo.m_dwMaxDelayTime );
			//////////////////////////////////////////////////////////////////////////			

			// 값 초기화!
            sUdpRelayServerInfo.ClearStatistics();
		}
	}

	// DB업데이트
	if( kPacketNot.m_vecUdpRelayCheckLog.empty() == false )
	{
        SendToLogDB( DBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT, kPacketNot );
	}
}

void KUdpRelayChecker::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

	switch( spEvent_->m_usEventID )
	{
		CASE( DBE_GET_CENTER_SERVER_LIST_ACK );
		CASE( E_CONNECT_RELAY_ACK );
		CASE( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT );

	default:
		dbg::cerr << L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr();
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

IMPL_ON_FUNC( DBE_GET_CENTER_SERVER_LIST_ACK )
{
	switch( KBaseServer::GetKObj()->GetServerClass() )
	{
	case KBaseServer::ESC_CHANNEL:
		{
			BOOST_TEST_FOREACH( const KServerInfo&, kCnServerInfo, kPacket_.m_vecCenterServerList )
			{
				//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
				START_LOG( cout, L"UDP Relay Server 모니터링 대상 등록! : " << kCnServerInfo.m_wstrPublicIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrPublicIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrPublicIP, sInsertInfo ) );
#else
				START_LOG( cout, L"UDP Relay Server 모니터링 대상 등록! : " << kCnServerInfo.m_wstrIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrIP, sInsertInfo ) );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
				//}}
			}
		}
		break;

	case KBaseServer::ESC_GAME:
		{
			BOOST_TEST_FOREACH( const KServerInfo&, kCnServerInfo, kPacket_.m_vecCenterServerList )
			{
				//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
				// 게임서버와 같은 IP만 체크합니다!
				if( NetCommon::GetLocalIPW() != kCnServerInfo.m_wstrPublicIP )
					continue;

				START_LOG( cout, L"UDP Relay Server 모니터링 대상 등록! : " << kCnServerInfo.m_wstrPublicIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrPublicIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrPublicIP, sInsertInfo ) );
#else
				// 게임서버와 같은 IP만 체크합니다!
				if( NetCommon::GetLocalIPW() != kCnServerInfo.m_wstrIP )
					continue;

				START_LOG( cout, L"UDP Relay Server 모니터링 대상 등록! : " << kCnServerInfo.m_wstrIP );

				SUdpRelayServerInfo sInsertInfo;
				sInsertInfo.m_iServerUID;
				sInsertInfo.m_wstrServerIP = kCnServerInfo.m_wstrIP;
				sInsertInfo.m_sServerPort = kCnServerInfo.m_usNCUDPPort;
				m_mapUdpRelayServerInfo.insert( std::make_pair( kCnServerInfo.m_wstrIP, sInsertInfo ) );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
				//}}
			}            
		}
		break;

	default:
		{
			START_LOG( cerr, L"이 서버에서는 Udp Relay Checker가 동작하면 안됩니다!" )
				<< BUILD_LOG( KBaseServer::GetKObj()->GetServerClass() )
				<< END_LOG;
		}
		break;
	}
}

IMPL_ON_FUNC( E_CONNECT_RELAY_ACK )
{
	std::map< std::wstring, SUdpRelayServerInfo >::iterator mit;
	mit = m_mapUdpRelayServerInfo.find( kPacket_.m_wstrRelayServerIP );
	if( mit == m_mapUdpRelayServerInfo.end() )
	{
		START_LOG( cerr, L"등록되지 않은 IP입니다!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() )
			<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
			<< END_LOG;
		return;
	}

	// 릴레이 접속 성공 처리!
	mit->second.m_bRelayConnected = true;

	if( IsPrintLog() )
	{
		START_LOG( cout, L"[알림] UDP Relay Checker Connect Success!" )
			<< BUILD_LOG( mit->second.m_wstrServerIP );
	}
}

IMPL_ON_FUNC( E_UDP_RELAY_SERVER_CHECK_PACKET_NOT )
{
	std::map< std::wstring, SUdpRelayServerInfo >::iterator mit;
	mit = m_mapUdpRelayServerInfo.find( kPacket_.m_wstrRelayServerIP );
	if( mit == m_mapUdpRelayServerInfo.end() )
	{
		START_LOG( cerr, L"등록되지 않은 IP입니다!" )
			<< BUILD_LOG( m_mapUdpRelayServerInfo.size() )
			<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
			<< END_LOG;
		return;
	}

	SUdpRelayServerInfo& sUdpRelayServerInfo = mit->second;

	// 릴레이 패킷이 도착했으므로 체크 해제
	sUdpRelayServerInfo.m_bSendRelayPacket = false;

	//{
	//	const DWORD dwUdpRelayLatency = kPacket_.m_dwEndTickCount - mit->second.m_dwLastTickCount;

	//	START_LOG( cout, L"[테스트로그] UDP Relay Server 지연 시간 체크!" )
	//		<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
	//		<< BUILD_LOG( mit->second.m_dwLastTickCount )
	//		<< BUILD_LOG( kPacket_.m_dwEndTickCount )
	//		<< BUILD_LOG( dwUdpRelayLatency );
	//}
	//
	{
		const DWORD dwUdpRelayLatency = kPacket_.m_dwEndTimeGetTime - sUdpRelayServerInfo.m_dwLastTimeGetTime;

		//START_LOG( cout, L"[테스트로그] UDP Relay Server 지연 시간 체크!" )
		//	<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
		//	<< BUILD_LOG( sUdpRelayServerInfo.m_dwLastTimeGetTime )
		//	<< BUILD_LOG( kPacket_.m_dwEndTimeGetTime )
		//	<< BUILD_LOG( dwUdpRelayLatency );

		// 통계
        if( sUdpRelayServerInfo.m_dwMaxTimeGetTime < dwUdpRelayLatency )
		{
            sUdpRelayServerInfo.m_dwMaxTimeGetTime = dwUdpRelayLatency;
		}

		sUdpRelayServerInfo.m_dwTotalTimeGetTime += dwUdpRelayLatency;
		++sUdpRelayServerInfo.m_dwCountTimeGetTime;

		if( IsPrintLog() )
		{
			START_LOG( cout, L"[알림] UDP Relay Check Packet 수신!" )
				<< BUILD_LOG( mit->second.m_wstrServerIP )
				<< BUILD_LOG( dwUdpRelayLatency );
		}
	}

	//{
	//	const float fUdpRelayLatency = m_kQPCounter.GetElapsedTime( mit->second.m_qwLastQPCounter );

	//	START_LOG( cout, L"[테스트로그] UDP Relay Server 지연 시간 체크!" )
	//		<< BUILD_LOG( kPacket_.m_wstrRelayServerIP )
	//		<< BUILD_LOG( fUdpRelayLatency );
	//}
}



#endif SERV_UDP_RELAY_CHECKER
//}}


