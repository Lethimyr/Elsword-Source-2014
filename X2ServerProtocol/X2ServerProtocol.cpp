#include "StdAfx.h"

bool	g_bIsSERVICE = false;
CX2ServerProtocol::CX2ServerProtocol( SEND_MSG_FUNC pSendGameMessage, HWND hWnd, UINT serverMsgID, const WCHAR** ppEventIDStrList, int eventIDEnd, bool bIsSERVICE )
{
	g_bIsSERVICE = bIsSERVICE;

	//m_pkUserProxy			= NULL;
	//m_pkTRUserProxy			= NULL;

	m_pSendGameMessage		= pSendGameMessage;
	m_hWnd					= hWnd;
	m_ServerMsgID			= serverMsgID;

#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	if( serverMsgID != 1141 ) // XGM_RECEIVE_FROM_COLLECTSERVER = 1141
		KEvent::SetEventID( ppEventIDStrList, eventIDEnd );
#else
	KEvent::SetEventID( ppEventIDStrList, eventIDEnd );
#endif	

	NetCommon::InitWinSock();
	KncSecurity::InitSecurity();
}

CX2ServerProtocol::~CX2ServerProtocol(void)
{
	//SAFE_DELETE( m_pkUserProxy );
	//SAFE_DELETE( m_pkTRUserProxy );

	KncSecurity::ReleaseSecurity();
}

//const char*	CX2ServerProtocol::GetMyIP()
//{
//	char strTemp[256];
//	::gethostname( strTemp, 256 );
//
//	HOSTENT* hostent;
//	char* ip;
//
//	hostent = ::gethostbyname( strTemp );
//
//	ip = ::inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );
//	return ip;
//}

bool CX2ServerProtocol::ConnectedToGameServer( const WCHAR* pIP, const int port )
{


	if( IsConnected() ) 
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( KUserProxyPtr spUserProxy = boost::atomic_load( &m_pkUserProxy ) )
        {
		    START_LOG( cerr, "이미 접속된 상태에서 재호출됨." )
			    << BUILD_LOG( spUserProxy->GetIPStr() )
			    << BUILD_LOG( spUserProxy->GetPort() )
			    << BUILD_LOG( pIP )
			    << BUILD_LOG( port );
        }
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		START_LOG( cerr, "이미 접속된 상태에서 재호출됨." )
			<< BUILD_LOG( m_pkUserProxy->GetIPStr() )
			<< BUILD_LOG( m_pkUserProxy->GetPort() )
			<< BUILD_LOG( pIP )
			<< BUILD_LOG( port );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

		return false;
	}

	//if( m_pkUserProxy != NULL )
	//{
	//	delete m_pkUserProxy;
	//}

	KUserProxyPtr spProxy( new KUserProxy( KUserProxy::UPT_GAME_SERVER ) );
	//KUserProxy* spProxy = new KUserProxy();
	spProxy->SetServerMsgID( m_ServerMsgID );

	spProxy->SetClassID( GSPROXY_ID );
	spProxy->Init( false );
	spProxy->SetHwnd( m_hWnd );
	spProxy->SetSendMsgCallBack( m_pSendGameMessage );

	std::wstring wIP = pIP;
	std::string IP = KncUtil::toNarrowString( wIP );

#ifdef CLIENT_COUNTRY_EU
	bool bConnect = false;

	for(int i = 0; i < 3; i++)
	{
		if( spProxy->Connect( IP.c_str(), port ) )
		{
			bConnect = true;

			break;
		}

		START_LOG( cerr, "테스트 코드" )
			<< BUILD_LOG( i );
	}
	
	if(bConnect == false)
#else
	if( !spProxy->Connect( IP.c_str(), port ) )
#endif
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        DisconnectFromGameServer();
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        m_pkUserProxy.reset();
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		//SAFE_DELETE( spProxy );
		return false;
	}

#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
    boost::atomic_store( &m_pkUserProxy, spProxy );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	m_pkUserProxy = spProxy;
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

	return true;
}

bool CX2ServerProtocol::ConnectedToTCPRelay( const WCHAR* pIP, const UidType& iUnitUID, const int port /* = 9301  */)
{
	if( IsTRConnected() ) 
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( KTRUserPtr spTRUserProxy = boost::atomic_load( &m_pkTRUserProxy ) )
        {
		    START_LOG( cerr, "이미 접속된 상태에서 재호출됨." )
			    << BUILD_LOG( spTRUserProxy->GetIPStr() )
			    << BUILD_LOG( spTRUserProxy->GetPort() )
			    << BUILD_LOG( pIP )
			    << BUILD_LOG( port );
        }
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		START_LOG( cerr, "이미 접속된 상태에서 재호출됨." )
			<< BUILD_LOG( m_pkTRUserProxy->GetIPStr() )
			<< BUILD_LOG( m_pkTRUserProxy->GetPort() )
			<< BUILD_LOG( pIP )
			<< BUILD_LOG( port );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

		return false;
	}

	//if( m_pkTRUserProxy != NULL )
	//{
	//	delete m_pkTRUserProxy;
	//}

	KTRUserPtr spTRUser( new KTRUser );
	spTRUser->SetServerMsgID( m_ServerMsgID );
	spTRUser->SetClassID( RTPROXY_ID );
	spTRUser->Init();
	spTRUser->SetSendMsgCallBack(m_pSendGameMessage);

	std::wstring wIP = pIP;
	std::string IP = KncUtil::toNarrowString( wIP );

	if( !spTRUser->Connect( IP.c_str(), port, iUnitUID ) )
	{
		//SAFE_DELETE( pkTRUser );
        spTRUser.reset();
        return false;
	}
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
    boost::atomic_store( &m_pkTRUserProxy, spTRUser );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	m_pkTRUserProxy = spTRUser;
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

	return true;
}


bool CX2ServerProtocol::ConnectedToChannelServer( const WCHAR* pIP, const int port )
{
	if( IsChConnected() ) 
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( KUserProxyPtr spChUserProxy = boost::atomic_load( &m_pkChUserProxy ) )
        {
		START_LOG( cerr, "이미 채널 서버에 접속된 상태에서 재접속 시도." )
			<< BUILD_LOG( spChUserProxy->GetIPStr() )
			<< BUILD_LOG(   spChUserProxy->GetPort() )
			<< BUILD_LOG( pIP )
			<< BUILD_LOG( port );
        }
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		START_LOG( cerr, "이미 채널 서버에 접속된 상태에서 재접속 시도." )
			<< BUILD_LOG( m_pkChUserProxy->GetIPStr() )
			<< BUILD_LOG( m_pkChUserProxy->GetPort() )
			<< BUILD_LOG( pIP )
			<< BUILD_LOG( port );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

		return false;
	}



	KUserProxyPtr spProxy( new KUserProxy( KUserProxy::UPT_CHANNEL_SERVER ) );
	spProxy->SetServerMsgID( m_ServerMsgID );
	spProxy->SetClassID( CHPROXY_ID );
	spProxy->Init( false );
	spProxy->SetHwnd( m_hWnd );
	spProxy->SetSendMsgCallBack( m_pSendGameMessage );

	std::wstring wIP = pIP;
	std::string IP = KncUtil::toNarrowString( wIP );

#ifdef CLIENT_COUNTRY_EU
	bool bConnect = false;

	for(int i = 0; i < 3; i++)
	{
		if( spProxy->Connect( IP.c_str(), port ) )
		{
			bConnect = true;

			break;
		}

		START_LOG( cerr, "테스트 코드" )
			<< BUILD_LOG( i );
	}
	
	if(bConnect == false)
#else
	if( !spProxy->Connect( IP.c_str(), port ) )
#endif
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        boost::atomic_store( &m_pkChUserProxy, KUserProxyPtr() );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		m_pkChUserProxy.reset();
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		return false;
	}
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
    boost::atomic_store( &m_pkChUserProxy, spProxy );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	m_pkChUserProxy = spProxy;
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

	return true;
}



void CX2ServerProtocol::ConnectedToGameServerForWorldTool()
{
	KUserProxyPtr spProxy( new KUserProxy( KUserProxy::UPT_GAME_SERVER ) );
	spProxy->SetServerMsgID( m_ServerMsgID );

	spProxy->SetClassID( GSPROXY_ID );
	spProxy->Init( false );
	spProxy->SetHwnd( m_hWnd );

#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
    boost::atomic_store( &m_pkUserProxy, spProxy );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	m_pkUserProxy = spProxy;
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
}

#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
bool CX2ServerProtocol::ConnectedToCollectServer( const WCHAR* pIP, const int port)
{
	if( IsCSConnected() ) 
	{
#ifdef _IN_HOUSE_
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( KUserProxyPtr spCSUserProxy = boost::atomic_load( &m_pkCSUserProxy ) )
        {
		    START_LOG( cerr, "이미 수집 서버에 접속된 상태에서 재접속 시도." )
			    << BUILD_LOG( spCSUserProxy->GetIPStr() )
			    << BUILD_LOG( spCSUserProxy->GetPort() )
			    << BUILD_LOG( pIP )
			    << BUILD_LOG( port );
        }
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

		START_LOG( cerr, "이미 수집 서버에 접속된 상태에서 재접속 시도." )
			<< BUILD_LOG( m_pkCSUserProxy->GetIPStr() )
			<< BUILD_LOG( m_pkCSUserProxy->GetPort() )
			<< BUILD_LOG( pIP )
			<< BUILD_LOG( port );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
#endif
		return false;
	}



	KUserProxyPtr spProxy( new KUserProxy( KUserProxy::UPT_COLLECT_SERVER ) );
	spProxy->SetServerMsgID( m_ServerMsgID );
	spProxy->SetClassID( CSPROXY_ID );
	spProxy->Init( false );
	spProxy->SetHwnd( m_hWnd );
	spProxy->SetSendMsgCallBack( m_pSendGameMessage );

	std::wstring wIP = pIP;
	std::string IP = KncUtil::toNarrowString( wIP );

	if( !spProxy->Connect( IP.c_str(), port ) )
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        boost::atomic_store( &m_pkCSUserProxy, KUserProxyPtr() );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		m_pkCSUserProxy.reset();
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		return false;
	}
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	// connection 열자마자 바로 끊기게 만드는 버그!!! - robobeg, 2014.1.31
    //boost::atomic_store( &m_pkCSUserProxy, KUserProxyPtr() );
    boost::atomic_store( &m_pkCSUserProxy, spProxy );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	m_pkCSUserProxy = spProxy;
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX


	return true;
}
#endif