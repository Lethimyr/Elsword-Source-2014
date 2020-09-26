#pragma once

//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//#include "../KncWX2Server/Common/ClientPacket.h"
//#endif//X2OPTIMIZE_TCP_RELAY_TEST


#define ADD_COLLECT_CLIENT_INFO_PROTOCOL
#define X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
#define X2OPTIMIZE_SESSION_THREAD_SAFETY


class CX2ServerProtocol
{
public:
	enum
	{
		GSPROXY_ID = 1,
		RTPROXY_ID,
		CHPROXY_ID,
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
		CSPROXY_ID,
#endif
	};

public:
	CX2ServerProtocol( SEND_MSG_FUNC pSendGameMessage, HWND hWnd, UINT serverMsgID, const WCHAR** ppEventIDStrList, int eventIDEnd, bool bIsSERVICE = false );
	~CX2ServerProtocol(void);

#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	bool IsConnected()
    { 
        if ( m_pkUserProxy )
        {
            if ( KUserProxyPtr spUserProxy = boost::atomic_load( &m_pkUserProxy ) )
                return spUserProxy->IsConnected();
        }
        return false;
    }
	bool IsTRConnected()        
    { 
        if ( m_pkTRUserProxy )
        {
            if ( KTRUserPtr spTRUserProxy = boost::atomic_load( &m_pkTRUserProxy ) )
                return  spTRUserProxy->IsConnected();
        }
        return false;
    }
	bool IsChConnected()        
    { 
        if ( m_pkChUserProxy )
        {
            if ( KUserProxyPtr spChUserProxy =  boost::atomic_load( &m_pkChUserProxy ) )
                return  spChUserProxy->IsConnected();
        }
        return  false;
    }
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	bool IsConnected()          { return (m_pkUserProxy   && m_pkUserProxy->IsConnected() ); }
	bool IsTRConnected()        { return (m_pkTRUserProxy && m_pkTRUserProxy->IsConnected() ); }
	bool IsChConnected()        { return (m_pkChUserProxy && m_pkChUserProxy->IsConnected() ); }
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL

#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
    bool IsCSConnected()		
    { 
        if ( m_pkCSUserProxy )
        {
            if ( KUserProxyPtr spCSUserProxy = boost::atomic_load( &m_pkCSUserProxy ) )
                return  spCSUserProxy->IsConnected();
        }
        return false;
    }
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	bool IsCSConnected()		{ return (m_pkCSUserProxy && m_pkCSUserProxy->IsConnected() ); }
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

#endif

	bool ConnectedToGameServer( const WCHAR* pIP, const int port );		// 서버 IP/Port 등의 정보
#ifdef SERV_COUNTRY_JP
	bool ConnectedToTCPRelay( const WCHAR* pIP, const UidType& iUnitUID, const int port = 12301 );		// 임규수 일본 변경
#elif defined(SERV_COUNTRY_ID)
	bool ConnectedToTCPRelay( const WCHAR* pIP, const UidType& iUnitUID, const int port = 14301 );		// 2013.05.23 lygan_조성욱 // 인도네시아용 포트 변경
#else //SERV_COUNTRY_JP
	bool ConnectedToTCPRelay( const WCHAR* pIP, const UidType& iUnitUID, const int port = 9301 );
#endif //SERV_COUNTRY_JP
	bool ConnectedToChannelServer( const WCHAR* pIP, const int port );
	void ConnectedToGameServerForWorldTool();		
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	bool ConnectedToCollectServer( const WCHAR* pIP, const int port);
#endif

	void DisconnectFromGameServer() 
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        boost::atomic_store( &m_pkUserProxy, KUserProxyPtr() );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		m_pkUserProxy.reset();
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}

	void DisconnectFromTRServer()
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        boost::atomic_store( &m_pkTRUserProxy, KTRUserPtr() );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		m_pkTRUserProxy.reset();
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}
	
	void DisconnectFromChannelServer() 
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        boost::atomic_store( &m_pkChUserProxy, KUserProxyPtr() );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		m_pkChUserProxy.reset();
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX

	}

#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	void DisconnectFromCollectServer()
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        boost::atomic_store( &m_pkCSUserProxy, KUserProxyPtr() );
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		m_pkCSUserProxy.reset();
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}
#endif

#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	bool            IsUserProxyValid() { return m_pkUserProxy != NULL; }
	bool            IsTRUserProxyValid() { return m_pkTRUserProxy != NULL; }
	bool            IsChUserProxyValid() { return m_pkChUserProxy != NULL; }

	bool            UserProxy_SetIntendedDisconnect(bool val) 
    { 
        if ( m_pkUserProxy )
        {
            if ( KUserProxyPtr spUserProxy = boost::atomic_load( &m_pkUserProxy ) )
            {
                spUserProxy->SetIntendedDisconnect( val );
                return true;
            }
        }
        return false;
    }
	bool            ChUserProxy_SetIntendedDisconnect(bool val) 
    { 
        if ( m_pkChUserProxy )
        {
            if ( KUserProxyPtr spChUserProxy = boost::atomic_load( &m_pkChUserProxy ) )
            {
                spChUserProxy->SetIntendedDisconnect( val );
                return true;
            }
        }
        return false;
    }

#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	KUserProxyPtr	GetUserProxy() { return m_pkUserProxy; }
	KTRUserPtr		GetTRUserProxy() { return m_pkTRUserProxy; }
	KUserProxyPtr	GetChUserProxy() { return m_pkChUserProxy; }
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX


#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	bool            IsCSUserProxyValid() { return m_pkCSUserProxy != NULL; }
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	KUserProxyPtr	GetCSUserProxy() { return m_pkCSUserProxy; }
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
#endif

	const char*		GetMyIP()	
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkUserProxy )
        {
            if ( KUserProxyPtr spUserProxy = boost::atomic_load( &m_pkUserProxy ) )
                return spUserProxy->GetIPStr(); 
        }
        return NULL;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkUserProxy == NULL )
			return NULL;
		return m_pkUserProxy->GetIPStr(); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}
	unsigned short	GetPort()	
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkUserProxy )
        {
            if ( KUserProxyPtr spUserProxy = boost::atomic_load( &m_pkUserProxy ) )
                return spUserProxy->GetPort(); 
        }
        return 0;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkUserProxy == NULL )
			return 0;

		return m_pkUserProxy->GetPort(); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}

	template < typename T > 
	bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false )
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkUserProxy )
        {
            if ( KUserProxyPtr spUserProxy = boost::atomic_load( &m_pkUserProxy ) )
                return spUserProxy->SendPacket( PI_GS_USER, 0, NULL, usEventID, data, bLogging, bCompress ); 
        }
        return false;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkUserProxy == NULL )
			return false;
		return m_pkUserProxy->SendPacket( PI_GS_USER, 0, NULL, usEventID, data, bLogging, bCompress ); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}

//#ifdef X2OPTIMIZE_TCP_RELAY_TEST
//	bool SendPacket_TcpRelayTest( char* pBuffer, int iBufferSize )
//	{ 
//		if( m_pkUserProxy == NULL )
//			return false;
//
//		KEGS_TCP_RELAY_TEST kPacket;
//		for( int i = 0; i < iBufferSize; i++ )
//			kPacket.m_vecData.push_back( pBuffer[i] );
//
//		return SendPacket( 999, kPacket );
//	}
//#endif//X2OPTIMIZE_TCP_RELAY_TEST

	bool SendID( IN const unsigned short usEventID )
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkUserProxy )
        {
            if ( KUserProxyPtr spUserProxy = boost::atomic_load( &m_pkUserProxy ) )
                return spUserProxy->SendID( PI_GS_USER, 0, NULL, usEventID ); 
        }
        return false;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkUserProxy == NULL )
			return false;
		return m_pkUserProxy->SendID( PI_GS_USER, 0, NULL, usEventID ); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}

	template < typename T > 
	bool SendTRPacket( unsigned short usEventID, T& data, bool bCompress = false )
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkTRUserProxy )
        {
            if ( KTRUserPtr spTRUserProxy = boost::atomic_load( &m_pkTRUserProxy ) )
                return spTRUserProxy->SendPacket( usEventID, data, bCompress );
        }
        return false;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkTRUserProxy == NULL )
			return false;
		return m_pkTRUserProxy->SendPacket( usEventID, data, bCompress ); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}
	bool SendTRID( IN const unsigned short usEventID )
	{
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkTRUserProxy )
        {
            if ( KTRUserPtr spTRUserProxy = boost::atomic_load( &m_pkTRUserProxy ) )
                return spTRUserProxy->SendID( usEventID );
        }
        return false;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkTRUserProxy == NULL )
			return false;
		return m_pkTRUserProxy->SendID( usEventID );
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}
	

	template < typename T > 
	bool SendChPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false )
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkChUserProxy )
        {
            if ( KUserProxyPtr spChUserProxy = boost::atomic_load( &m_pkChUserProxy ) )
                return spChUserProxy->SendPacket( PI_CHANNEL_USER, 0, NULL, usEventID, data, bLogging, bCompress ); 
        }
        return false;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkChUserProxy == NULL )
			return false;
		return m_pkChUserProxy->SendPacket( PI_CHANNEL_USER, 0, NULL, usEventID, data, bLogging, bCompress ); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}
	bool SendChID( IN const unsigned short usEventID )
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkChUserProxy )
        {
            if ( KUserProxyPtr spChUserProxy = boost::atomic_load( &m_pkChUserProxy ) )
                return spChUserProxy->SendID( PI_CHANNEL_USER, 0, NULL, usEventID ); 
        }
        return false;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkChUserProxy == NULL )
			return false;
		return m_pkChUserProxy->SendID( PI_CHANNEL_USER, 0, NULL, usEventID ); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}

#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	template < typename T > 
	bool SendCSPacket( unsigned short usEventID, T& data, bool bCompress = false )
	{ 
#ifdef  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
        if ( m_pkCSUserProxy )
        {
            if ( KUserProxyPtr spCSUserProxy = boost::atomic_load( &m_pkCSUserProxy ) )
                return spCSUserProxy->SendPacket( 0x00000A00 | PC_USER , 0, NULL, usEventID, data, true, bCompress ); 
        }
        return false;
#else   X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
		if( m_pkCSUserProxy == NULL )
			return false;
		return m_pkCSUserProxy->SendPacket( 0x00000A00 | PC_USER , 0, NULL, usEventID, data, true, bCompress ); 
#endif  X2OPTIMIZE_X2SERVERPROTOCOL_MULTITHREAD_CRASH_BUG_FIX
	}
#endif



protected:
	KUserProxyPtr	m_pkUserProxy;
    KTRUserPtr      m_pkTRUserProxy;
	KUserProxyPtr	m_pkChUserProxy;
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	KUserProxyPtr	m_pkCSUserProxy;
#endif

	HWND			m_hWnd;
	UINT			m_ServerMsgID;
	SEND_MSG_FUNC	m_pSendGameMessage;
};

