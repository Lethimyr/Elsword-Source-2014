#include <WinSock2.h>
#include "GiantBaseManager.h"

#include <lua_tinker.h>


void KGiantBaseManager::RegToLuaBase()
{
	lua_tinker::class_add<KGiantBaseManager>( g_pLua, "KGiantBaseManager" );
	lua_tinker::class_def<KGiantBaseManager>( g_pLua, "dump",						&KGiantBaseManager::Dump );
	lua_tinker::class_def<KGiantBaseManager>( g_pLua, "SetAddress",					&KGiantBaseManager::SetAddress );
	lua_tinker::class_def<KGiantBaseManager>( g_pLua, "SetNumThread",				&KGiantBaseManager::Init );
	lua_tinker::class_def<KGiantBaseManager>( g_pLua, "SetHeartBitInterval",		&KGiantBaseManager::SetHeartBitInterval );
	lua_tinker::class_def<KGiantBaseManager>( g_pLua, "SetConnectionCheckInterval",	&KGiantBaseManager::SetConnectionCheckInterval );
}

void KGiantBaseManager::Init( int nThreadNum )
{
	m_iRecvCP = 0;

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
	m_spThreadRecv = boost::shared_ptr< KTThread< KGiantBaseManager > >
		( new KTThread< KGiantBaseManager >( *this, KGiantBaseManager::Recv, 50 ) );

	m_spThreadSend = boost::shared_ptr< KTThread< KGiantBaseManager > >
		( new KTThread< KGiantBaseManager >(*this, KGiantBaseManager::Send, 100 ) );

	KThreadManager::Init( nThreadNum );
}

void KGiantBaseManager::SetAddress( const char* szServerIP, unsigned short usPort)
{
	m_kGiantConnectInfo.m_strIP = szServerIP;
	m_kGiantConnectInfo.m_usPort = usPort;
}

void KGiantBaseManager::SetHeartBitInterval(int time)
{
	m_dwHeartBeatInterval = time;
}

void KGiantBaseManager::SetConnectionCheckInterval(int time)
{
	m_dwConnectionCheckInterval = time;
}

void KGiantBaseManager::DumpBuffer( BYTE* buffer, bool bError )
{
	JIF( buffer );
	//	JIF( sizeof( buffer ) >= 4+1+1 );

	int iLength;
	::memcpy( &iLength, buffer, sizeof( iLength ) );

	//	iLength = ::ntohl( iLength );
	int iTotalPacketSize = iLength + sizeof(int);

	LIF( iTotalPacketSize <= MAX_PACKET_SIZE_OF_GIANT_AUTH );
	iTotalPacketSize =  std::min< int >( iTotalPacketSize, MAX_PACKET_SIZE_OF_GIANT_AUTH );

	char szBuffer[MAX_PACKET_SIZE_OF_GIANT_AUTH * 3 + 1];
	char szByte[4];

	for( int i = 0; i < iTotalPacketSize; i++ )
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
	szBuffer[iTotalPacketSize * 3] = '\0';

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

void KGiantBaseManager::BeginThread()
{
	if( m_spThreadRecv )
	{
		m_spThreadRecv->Begin();
	}

	if( m_spThreadSend )
	{
		m_spThreadSend->Begin();
	}

	KThreadManager::BeginThread();        

	if( !Connect() )
	{
		START_LOG( cerr, L"Giant Platform Server 접속 실패." )
			<< END_LOG;
	}
}

void KGiantBaseManager::EndThread()
{
	KThreadManager::EndThread();

	if( m_spThreadSend )
	{
		m_spThreadSend->End( 10000 );
	}

	if( m_spThreadRecv )
	{
		m_spThreadRecv->End( 3000 );
	}
}

void KGiantBaseManager::Tick()
{
	CheckConnection();

	KeepConnection();
}

void KGiantBaseManager::QueueingSendPacket( const TPacketPtr& spPacket )
{
	JIF( spPacket );

	KLocker lock( m_csSendQueue );

	m_kSendQueue.push( spPacket );
}

void KGiantBaseManager::Send()
{
	if( !IsConnected() )
	{
		return;
	}

	int ret;
	char buf[MAX_PACKET_SIZE_OF_GIANT_AUTH];
	TPacketPtr spPacket;
	while( GetSendPacket( spPacket ) )
	{
		if( !spPacket )
		{
			START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( m_kSendQueue.size() )
				<< END_LOG;

			continue;
		}

		if( spPacket->GetTotalLength() > MAX_PACKET_SIZE_OF_GIANT_AUTH )
		{
			START_LOG( cerr, L"패킷 사이즈 이상." )
				<< BUILD_LOG( spPacket->GetTotalLength() )
				<< END_LOG;

			continue;
		}

		START_LOG( clog2, L"패킷 전송" )
			<< END_LOG;
		spPacket->WriteToBuffer( ( BYTE* )buf );
		DumpBuffer( ( BYTE* )buf, false );

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

void KGiantBaseManager::Recv()
{
	if( !IsConnected() )
	{
		return;
	}

	int ret;
	ret = ::recv( m_sock,
		m_cRecvBuffer + m_iRecvCP,
		MAX_PACKET_SIZE_OF_GIANT_AUTH - m_iRecvCP,
		0 );

	START_LOG( clog, L"패킷 받음." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		START_LOG( cerr, GET_WSA_MSG );
		CLOSE_SOCKET( m_sock );
		return;
	}

	if( ret == 0 )
	{
		START_LOG( cerr, L"소켓 연결이 끊어짐." )
			<< BUILD_LOG( GET_WSA_MSG )
			<< END_LOG;

		CLOSE_SOCKET( m_sock );
		return;
	}

	if( ret > MAX_PACKET_SIZE_OF_GIANT_AUTH - m_iRecvCP )
	{
		START_LOG( cerr, L"전송된 크기가 너무 크다." )
			<< BUILD_LOG( ret )
			<< BUILD_LOG( MAX_PACKET_SIZE_OF_GIANT_AUTH )
			<< BUILD_LOG( m_iRecvCP )
			<< END_LOG;

		CLOSE_SOCKET( m_sock );
		return;
	}
	m_iRecvCP += ret;

	while( m_iRecvCP >= 6 )
	{
		// 모든 패킷이 6바이트 이상임

		int iLength;
		::memcpy( &iLength, m_cRecvBuffer, sizeof(int) );

		//		usLength = ::ntohs( iLength );
		int iTotalPacketSize = iLength + sizeof(int);
		if( iTotalPacketSize > MAX_PACKET_SIZE_OF_GIANT_AUTH )
		{
			START_LOG( cerr, L"추출한 패킷 사이즈 이상." )
				<< BUILD_LOG( iTotalPacketSize )
				<< BUILD_LOG( MAX_PACKET_SIZE_OF_GIANT_AUTH )
				<< END_LOG;

			CLOSE_SOCKET( m_sock );
			return;
		}

		if( iTotalPacketSize > m_iRecvCP )
		{
			return;
		}

		MakeEventFromReceivedPacket();

		if( m_iRecvCP > iTotalPacketSize )
		{
			::memmove( m_cRecvBuffer, m_cRecvBuffer + iTotalPacketSize, m_iRecvCP - iTotalPacketSize );
		}
		m_iRecvCP -= iTotalPacketSize;
	}
}

void KGiantBaseManager::ClearSendQueue()
{
	KLocker lock( m_csSendQueue );

	std::queue<TPacketPtr> empty;
	std::swap( m_kSendQueue, empty );
}

bool KGiantBaseManager::Connect()
{
	START_LOG( cout, L"연결 시도" )
		<< BUILD_LOG( m_kGiantConnectInfo.m_strIP )
		<< BUILD_LOG( m_kGiantConnectInfo.m_usPort )				
		<< END_LOG;

	m_iRecvCP = 0;
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
	addr.sin_addr.s_addr	= inet_addr( m_kGiantConnectInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kGiantConnectInfo.m_usPort );

	int ret = ::connect( sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( m_kGiantConnectInfo.m_strIP )
			<< BUILD_LOG( m_kGiantConnectInfo.m_usPort )
			<< END_LOG;

		CLOSE_SOCKET( sock );
		return false;
	}		

	m_sock = sock;

	// 인증 패킷을 보내기 전에 쌓인 패킷을 클리어
	ClearSendQueue();

	// 인증 시도
	KEGIANT_COMMON_INITIALIZE_REQ kPacketInit;
	kPacketInit.m_wstrIP		= NetCommon::GetLocalIPW();
	kPacketInit.m_usPort		= 10000;						// 단순 플래그

	boost::shared_ptr< KGiantCommonPacket > spPacket( new KGiantCommonPacket );
	spPacket->Write( kPacketInit );
	QueueingSendPacket( spPacket );

	START_LOG( cout, L"EGIANT_COMMON_INITIALIZE_REQ" )
		<< BUILD_LOG( kPacketInit.m_wstrIP )
		<< BUILD_LOG( kPacketInit.m_usPort )				
		<< END_LOG;

	return true;
}

bool KGiantBaseManager::IsConnected()
{
	return ( m_sock != INVALID_SOCKET );
}

void KGiantBaseManager::KeepConnection()
{
	if( ::GetTickCount() - m_dwLastHeartBeatTick < m_dwHeartBeatInterval )
	{
		return;
	}

	m_dwLastHeartBeatTick = ::GetTickCount();

	// 허트빗 보내기
	KEGIANT_COMMON_NULL_CLIENT kPacket;
	boost::shared_ptr< KGiantCommonPacket > spPacket( new KGiantCommonPacket );
	spPacket->Write( kPacket );
	QueueingSendPacket( spPacket );
}

void KGiantBaseManager::CheckConnection()
{
	if( ::GetTickCount() - m_dwLastConnectionCheckTick < m_dwConnectionCheckInterval )
	{
		return;
	}

	m_dwLastConnectionCheckTick = ::GetTickCount();

	if( !IsConnected() )
	{
		Connect();
	}
}

bool KGiantBaseManager::GetSendPacket( TPacketPtr& spPacket )
{
	KLocker lock( m_csSendQueue );

	if( m_kSendQueue.empty() )
	{
		return false;
	}

	spPacket = m_kSendQueue.front();
	m_kSendQueue.pop();

	return true;
}
