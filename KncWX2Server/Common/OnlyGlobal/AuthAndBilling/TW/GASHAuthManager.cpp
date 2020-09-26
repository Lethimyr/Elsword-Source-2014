#include <WinSock2.h>
#include "GASHAuthManager.h"

#ifdef SERV_COUNTRY_TWHK

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "GASHAuth.h"

DWORD KGASHAuthManager::ms_dwConnectionCheckGap[] = { 15 * 1000,  15 * 1000 ,  15 * 1000  };

ImplToStringW( KGASHAuthManager )
{
	return stm_;
}

ImplPfID( KGASHAuthManager, PI_NULL );

ImplementSingleton( KGASHAuthManager );

KGASHAuthManager::KGASHAuthManager()
{
	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{
		m_dwLastConnectionCheckTick[iPortNum] = ::GetTickCount();
		m_iRecvCP[iPortNum] = 0;
	}
}

KGASHAuthManager::~KGASHAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KGASHAuthManager::RegToLua()
{
	lua_tinker::class_add<KGASHAuthManager>( g_pLua, "KGASHAuthManager" );
	lua_tinker::class_def<KGASHAuthManager>( g_pLua, "dump",                    &KGASHAuthManager::Dump );
    lua_tinker::class_def<KGASHAuthManager>( g_pLua, "InitGASHAuthValue",       &KGASHAuthManager::InitGASHAuthValue );
	lua_tinker::class_def<KGASHAuthManager>( g_pLua, "InitGASHAuthAddress",     &KGASHAuthManager::InitGASHAuthAddress );
	lua_tinker::class_def<KGASHAuthManager>( g_pLua, "SetNumThread",            &KGASHAuthManager::Init );

	lua_tinker::decl( g_pLua, "GASHAuthManager", this );
}

void KGASHAuthManager::InitGASHAuthValue( const char* szServiceCode, const char* szServiceRegion )
{
    m_kGASHAuthInfo.m_strServiceCode = szServiceCode;
    m_kGASHAuthInfo.m_strServiceRegion = szServiceRegion;

    return;
}

void KGASHAuthManager::InitGASHAuthAddress( const char* szGconsoleIP, unsigned short  usAuthenticationPort, unsigned short  usAuthorizationPort, unsigned short  usAccountingPort )
{
	m_kGASHAuthInfo.m_strIP = szGconsoleIP;
    m_kGASHAuthInfo.m_usGash_Port[GP_AUTHENTICATION] = usAuthenticationPort;
    m_kGASHAuthInfo.m_usGash_Port[GP_AUTHORIZATION]	 = usAuthorizationPort;
    m_kGASHAuthInfo.m_usGash_Port[GP_ACCOUNTING]	 = usAccountingPort;

	return;
}

void KGASHAuthManager::Init( int nThreadNum )
{

	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{
		m_iRecvCP[iPortNum] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
	m_spThreadRecv[GP_AUTHENTICATION] = boost::shared_ptr< KTThread< KGASHAuthManager > >
		( new KTThread< KGASHAuthManager >( *this, KGASHAuthManager::RecvFromAuthentication, 50 ) );

	m_spThreadSend[GP_AUTHENTICATION] = boost::shared_ptr< KTThread< KGASHAuthManager > >
		( new KTThread< KGASHAuthManager >( *this, KGASHAuthManager::SendToAuthentication, 100 ) );

	m_spThreadRecv[GP_AUTHORIZATION] = boost::shared_ptr< KTThread< KGASHAuthManager > >
		( new KTThread< KGASHAuthManager >( *this, KGASHAuthManager::RecvFromAuthorization, 50 ) );

	m_spThreadSend[GP_AUTHORIZATION] = boost::shared_ptr< KTThread< KGASHAuthManager > >
		( new KTThread< KGASHAuthManager >( *this, KGASHAuthManager::SendToAuthorization, 100 ) );

	m_spThreadRecv[GP_ACCOUNTING] = boost::shared_ptr< KTThread< KGASHAuthManager > >
		( new KTThread< KGASHAuthManager >( *this, KGASHAuthManager::RecvFromAccounting, 50 ) );

	m_spThreadSend[GP_ACCOUNTING] = boost::shared_ptr< KTThread< KGASHAuthManager > >
		( new KTThread< KGASHAuthManager >( *this, KGASHAuthManager::SendToAccounting, 100 ) );


	KThreadManager::Init( nThreadNum );
}

KThread* KGASHAuthManager::CreateThread()
{
	return new KGASHAuth;
}

void KGASHAuthManager::BeginThread()
{
	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{	
		if( m_spThreadRecv[iPortNum] )
		{
			m_spThreadRecv[iPortNum]->Begin();
		}

		if( m_spThreadSend[iPortNum] )
		{
			m_spThreadSend[iPortNum]->Begin();
		}
	}

	KThreadManager::BeginThread();        

	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{	
		if( !Connect( iPortNum ) )
		{
			START_LOG( cerr, L"GASH 인증 서버 접속 실패." )
				<< END_LOG;
		}
	}
}

void KGASHAuthManager::EndThread()
{
	KThreadManager::EndThread();

	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{	
		if( m_spThreadSend[iPortNum] )
		{
			m_spThreadSend[iPortNum]->End( 10000 );
		}

		if( m_spThreadRecv[iPortNum] )
		{
			m_spThreadRecv[iPortNum]->End( 3000 );
		}
	}
}

void KGASHAuthManager::Tick()
{
    CheckConnection();
}

void KGASHAuthManager::RegisterUserIDUserUID( std::string& strUserID, UidType iUserUID )
{
    std::map< std::string, UidType >::iterator mit;

    KLocker lock( m_csUserIDUserUID );
    mit = m_mapUserIDUserUID.find( strUserID );
    if( mit != m_mapUserIDUserUID.end() )
    {
        START_LOG( cerr, L"user id exists." )
            << BUILD_LOG( mit->first )
            << BUILD_LOG( mit->second )
            << END_LOG;
    }
    else
    {
        START_LOG( clog2, L"user id reg" )
            << BUILD_LOG( strUserID )
            << BUILD_LOG( iUserUID )
            << BUILD_LOG( m_mapUserIDUserUID.size() )
            << END_LOG;
    }

    m_mapUserIDUserUID[strUserID] = iUserUID;
}

UidType KGASHAuthManager::UnregisterUserIDUserUID( std::string& strUserID )
{
    std::map< std::string, UidType >::iterator mit;

    KLocker lock( m_csUserIDUserUID );
    mit = m_mapUserIDUserUID.find( strUserID.c_str() );
    if( mit == m_mapUserIDUserUID.end() )
    {
        START_LOG( cerr, L"user id not exists." )
            << BUILD_LOG( strUserID )
            << END_LOG;

        return 0;
    }
    else
    {
        START_LOG( clog2, L"user id unreg" )
            << BUILD_LOG( strUserID )
            << BUILD_LOG( mit->second )
            << END_LOG;
    }

    UidType iRet = mit->second;
    m_mapUserIDUserUID.erase( mit );

    return iRet;
}

void KGASHAuthManager::RegisterUserIDUserIP( std::string& strUserID, std::string& strUserIP )
{
	std::map< std::string, std::string >::iterator mit;

	KLocker lock( m_csUserIDUserUID );
	mit = m_mapUserIDUserIP.find( strUserID );
	if( mit != m_mapUserIDUserIP.end() )
	{
		START_LOG( cerr, L"user id exists." )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second )
			<< END_LOG;
	}
	else
	{
		START_LOG( clog2, L"user id reg" )
			<< BUILD_LOG( strUserID )
			<< BUILD_LOG( strUserIP )
			<< BUILD_LOG( m_mapUserIDUserIP.size() )
			<< END_LOG;
	}

	m_mapUserIDUserIP[strUserID] = strUserIP;
}

std::string KGASHAuthManager::UnregisterUserIDUserIP( std::string& strUserID )
{
	std::map< std::string, std::string >::iterator mit;

	KLocker lock( m_csUserIDUserUID );
	mit = m_mapUserIDUserIP.find( strUserID.c_str() );
	if( mit == m_mapUserIDUserIP.end() )
	{
		START_LOG( cerr, L"user id not exists." )
			<< BUILD_LOG( strUserID )
			<< END_LOG;

		return "";
	}
	else
	{
		START_LOG( clog2, L"user id unreg" )
			<< BUILD_LOG( strUserID )
			<< BUILD_LOG( mit->second )
			<< END_LOG;
	}

	std::string srtRet = mit->second;
	m_mapUserIDUserIP.erase( mit );

	return srtRet;
}

void KGASHAuthManager::QueueingSendPacket( int iPortSelect, const boost::shared_ptr< std::string >& spPacket )
{
    JIF( spPacket );
    {
        KLocker lock( m_csSendQueue[iPortSelect] );

        m_kSendQueue[iPortSelect].push( spPacket );
    }
}

void KGASHAuthManager::SendTo( int iPortEnum )
{
    JIF( iPortEnum >= 0 && iPortEnum < GP_NUM );

	if( !IsConnected(iPortEnum) )
	{
		return;
	}

	int ret;
    char buf[MAX_PACKET_SIZE_OF_GASH_AUTH];
	boost::shared_ptr< std::string > spPacket;

	while( GetSendPacket( iPortEnum, spPacket ) )
	{
		if( !spPacket )
		{
			START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( m_kSendQueue[iPortEnum].size() )
				<< END_LOG;

			continue;
		}

		START_LOG( clog, L"보내는 패킷" )
			<< BUILD_LOG( spPacket->c_str() )
			<< END_LOG;

        if( spPacket->size() > MAX_PACKET_SIZE_OF_GASH_AUTH )
        {
            START_LOG( cerr, L"패킷 사이즈 이상." )
                << BUILD_LOG( spPacket->size() )
                << END_LOG;

            continue;
        }

        ::memcpy( buf, spPacket->c_str(), spPacket->size() );

        char* cCurBufPos = buf;
		int iRemainedSendSize = spPacket->size();
		while( iRemainedSendSize > 0 )
		{
			ret = ::send( m_sock[iPortEnum],
				cCurBufPos,
				iRemainedSendSize,
				0 );

			if( ret == SOCKET_ERROR )
			{
				START_LOG( cwarn, GET_WSA_MSG );
				CLOSE_SOCKET( m_sock[iPortEnum] );
				return;
			}

			iRemainedSendSize -= ret;
            cCurBufPos += ret;
		}
	}

}

void KGASHAuthManager::RecvFrom(int iPortEnum)
{
    JIF( iPortEnum >= 0 && iPortEnum < GP_NUM );

	if( !IsConnected(iPortEnum) )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock[iPortEnum],
		m_cRecvBuffer[iPortEnum] + m_iRecvCP[iPortEnum],
		MAX_PACKET_SIZE_OF_GASH_AUTH - m_iRecvCP[iPortEnum],
		0 );

	START_LOG( clog, L"패킷 받음." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		START_LOG( cerr, GET_WSA_MSG );
		CLOSE_SOCKET( m_sock[iPortEnum] );
		return;
	}

	if( ret == 0 )
	{
		START_LOG( cerr, L"소켓 연결이 끊어짐." )
			<< END_LOG;

		CLOSE_SOCKET( m_sock[iPortEnum] );
		return;
	}

	if( ret > MAX_PACKET_SIZE_OF_GASH_AUTH - m_iRecvCP[iPortEnum] )
	{
		START_LOG( cerr, L"전송된 크기가 너무 크다." )
			<< BUILD_LOG( ret )
			<< BUILD_LOG( MAX_PACKET_SIZE_OF_GASH_AUTH )
			<< BUILD_LOG( m_iRecvCP[iPortEnum] )
			<< END_LOG;

		CLOSE_SOCKET( m_sock[iPortEnum] );
		return;
	}
	m_iRecvCP[iPortEnum] += ret;

    //////
    std::string strRecv;
    for( int i = 0; i < m_iRecvCP[iPortEnum]; i++ )
    {
        strRecv.push_back( m_cRecvBuffer[iPortEnum][i] );
    }
    strRecv.push_back( '\0' );
    START_LOG( clog2, L"받은 GASH 패킷 그냥" )
        << BUILD_LOG( strRecv )
        << END_LOG;
    //////

	int iIndex = 0;
	while( iIndex + 2 <= m_iRecvCP[iPortEnum] )
	{
		if( m_cRecvBuffer[iPortEnum][iIndex] == '\r' && m_cRecvBuffer[iPortEnum][iIndex + 1] == '\n' )
		{
			char szPacket[MAX_PACKET_SIZE_OF_GASH_AUTH + 1];
			::ZeroMemory( szPacket, MAX_PACKET_SIZE_OF_GASH_AUTH + 1 );
			::memcpy( szPacket, m_cRecvBuffer[iPortEnum], iIndex + 2 );
			std::string strPacket = szPacket;

            START_LOG( clog2, L"받은 GASH 패킷" )
                << BUILD_LOG( strPacket )
                << END_LOG;

			MakeEventFromReceived(iPortEnum, strPacket);
			if( m_iRecvCP[iPortEnum] > iIndex + 2 )
			{
				::memmove( m_cRecvBuffer[iPortEnum], m_cRecvBuffer[iPortEnum] + iIndex + 2, m_iRecvCP[iPortEnum] - ( iIndex + 2 ) );
			}
			m_iRecvCP[iPortEnum] -= ( iIndex + 2 );
			iIndex = 0;
			continue;
		}

		iIndex++;
	}

}

void KGASHAuthManager::RecvFromAuthentication( )
{
	RecvFrom(GP_AUTHENTICATION);
}

void KGASHAuthManager::SendToAuthentication( )
{
	SendTo(GP_AUTHENTICATION);
}

void KGASHAuthManager::RecvFromAuthorization( )
{
	RecvFrom(GP_AUTHORIZATION);
}

void KGASHAuthManager::SendToAuthorization( )
{
	SendTo(GP_AUTHORIZATION);
}

void KGASHAuthManager::RecvFromAccounting( )
{
	RecvFrom(GP_ACCOUNTING);
}

void KGASHAuthManager::SendToAccounting( )
{
	SendTo(GP_ACCOUNTING);
}


bool KGASHAuthManager::Connect( int iPortSelect )
{
    _JIF( iPortSelect >= 0 && iPortSelect < GP_NUM, return false );

	m_iRecvCP[iPortSelect] = 0;
	//m_sock[iPortSelect] = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성
	SOCKET sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성

	if( INVALID_SOCKET == sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		return false;
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( m_kGASHAuthInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kGASHAuthInfo.m_usGash_Port[iPortSelect] );

	int ret = ::connect( sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( m_kGASHAuthInfo.m_strIP )
			<< BUILD_LOG( m_kGASHAuthInfo.m_usGash_Port[iPortSelect] )
			<< END_LOG;

		CLOSE_SOCKET( sock );
		return false;
	}		
	
	m_sock[iPortSelect] = sock;

	return true;
}

bool KGASHAuthManager::IsConnected( int iPortSelect )
{
    _JIF( iPortSelect >= 0 && iPortSelect < GP_NUM, return false );

	return ( m_sock[iPortSelect] != INVALID_SOCKET );
}

void KGASHAuthManager::CheckConnection()
{
	for( int iPortNum = 0; iPortNum < GP_NUM; iPortNum++ )
	{
		if( ::GetTickCount() - m_dwLastConnectionCheckTick[iPortNum] < ms_dwConnectionCheckGap[iPortNum] )
		{
			continue;
		}

		m_dwLastConnectionCheckTick[iPortNum] = ::GetTickCount();

		if( !IsConnected(iPortNum) )
		{
			Connect(iPortNum);
		}
	}
}

bool KGASHAuthManager::GetSendPacket( int iPortSelect,  boost::shared_ptr< std::string >& spPacket )
{
    _JIF( iPortSelect >= 0 && iPortSelect < GP_NUM, return false );

    KLocker lock( m_csSendQueue[iPortSelect] );

    if( m_kSendQueue[iPortSelect].empty() )
    {
        return false;
    }

    spPacket = m_kSendQueue[iPortSelect].front();
    m_kSendQueue[iPortSelect].pop();

    return true;
}

void KGASHAuthManager::MakeEventFromReceived(int iPortEnum, std::string& strPacket)
{
    JIF( iPortEnum >= 0 && iPortEnum < GP_NUM );

	KGashResponseData kPacketAck;

    JIF( kPacketAck.ExtractFromGASHPacket( strPacket ) );

	KEventPtr spEvent( new KEvent );
	switch(iPortEnum)
	{
	case GP_AUTHENTICATION : 
		spEvent->SetData(PI_NULL, NULL, EGASH_AUTHENTICATION_ACK, kPacketAck );
		break;
	case GP_AUTHORIZATION : 
		spEvent->SetData(PI_NULL, NULL, EGASH_AUTHORIZATION_ACK, kPacketAck );
		break;
	case GP_ACCOUNTING : 
		spEvent->SetData(PI_NULL, NULL, EGASH_ACCOUNTING_ACK, kPacketAck );
		break;
    default:
        START_LOG( cerr, L"포트 종류 이상." )
            << BUILD_LOG( iPortEnum )
            << END_LOG;
        break;
	}

	QueueingEvent( spEvent );
}


#endif // SERV_COUNTRY_TWHK


