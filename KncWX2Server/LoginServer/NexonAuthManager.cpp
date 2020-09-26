#include <WinSock2.h>
#include "NexonAuthManager.h"

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "NexonAuth.h"
#include "LoginServer.h"
#include "LoginSimLayer.h"

//{{ 2012. 01. 27  김민성	로그인 인증서버 연결 끊김 자동 sms 전송
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
//}}


//{{ 2010. 06. 04  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW


DWORD KNexonAuthManager::ms_dwHeartBeatGap( 1 * 60 * 1000 );		// 1분 마다 하트비트
DWORD KNexonAuthManager::ms_dwConnectionCheckGap( 1 * 30 * 1000 );	// 30초 마다 접속 체크

ImplToStringW( KNexonAuthManager )
{
	return stm_;
}

ImplPfID( KNexonAuthManager, PI_LOGIN_NX_AUTH );

ImplementSingleton( KNexonAuthManager );

KNexonAuthManager::KNexonAuthManager()
{
	m_dwLastHeartBeatTick = ::GetTickCount();
	m_dwLastConnectionCheckTick = ::GetTickCount();
    m_iRecvCP = 0;
}

KNexonAuthManager::~KNexonAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KNexonAuthManager::RegToLua()
{
	lua_tinker::class_add<KNexonAuthManager>( g_pLua, "KNexonAuthManager" );
	lua_tinker::class_def<KNexonAuthManager>( g_pLua, "dump",				 &KNexonAuthManager::Dump );
	lua_tinker::class_def<KNexonAuthManager>( g_pLua, "InitNexonAuthInfo",	 &KNexonAuthManager::InitNexonAuthInfo_LUA );
	lua_tinker::class_def<KNexonAuthManager>( g_pLua, "SetNumThread",		 &KNexonAuthManager::Init );
	lua_tinker::class_def<KNexonAuthManager>( g_pLua, "Disconnect",			 &KNexonAuthManager::DisconnectAuthServer_LUA );

	lua_tinker::decl( g_pLua, "NXAuthManager", this );
}

void KNexonAuthManager::InitNexonAuthInfo_LUA( const char* szNexonAuthIP, unsigned short usPort, int iDomain, const char* szDomainName, int iSynchronizeType, int iSynchronizeCount )
{
	m_kNexonAuthInfo.m_strIP			 = szNexonAuthIP;
	m_kNexonAuthInfo.m_usPort			 = usPort;
	m_kNexonAuthInfo.m_iDomain			 = iDomain;
	m_kNexonAuthInfo.m_wstrDomainName	 = KncUtil::toWideString_UTF8( std::string( szDomainName ) );
	m_kNexonAuthInfo.m_iSynchronizeType	 = iSynchronizeType;
	m_kNexonAuthInfo.m_iSynchronizeCount = iSynchronizeCount;
}

void KNexonAuthManager::DisconnectAuthServer_LUA()
{
	START_LOG( cout, L"테스트를 위한 인증 서버 접속 끊기!" );

    CLOSE_SOCKET( m_sock );
}

void KNexonAuthManager::Init( int nThreadNum )
{
    m_iRecvCP = 0;

    //////////////////////////////////////////////////////////////////////////
    // thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
#ifdef _CONVERT_VS_2010
    m_spThreadRecv = boost::shared_ptr< KTThread< KNexonAuthManager > >
        ( new KTThread< KNexonAuthManager >( *this, &KNexonAuthManager::Recv, 50 ) );

    m_spThreadSend = boost::shared_ptr< KTThread< KNexonAuthManager > >
        ( new KTThread< KNexonAuthManager >( *this, &KNexonAuthManager::Send, 100 ) );
#else
    m_spThreadRecv = boost::shared_ptr< KTThread< KNexonAuthManager > >
        ( new KTThread< KNexonAuthManager >( *this, KNexonAuthManager::Recv, 50 ) );

    m_spThreadSend = boost::shared_ptr< KTThread< KNexonAuthManager > >
        ( new KTThread< KNexonAuthManager >( *this, KNexonAuthManager::Send, 100 ) );
#endif _CONVERT_VS_2010

	KThreadManager::Init( nThreadNum );
}

KThread* KNexonAuthManager::CreateThread()
{
	return new KNexonAuth;
}

void KNexonAuthManager::BeginThread()
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
        START_LOG( cerr, L"넥슨 PC방 인증 서버 접속 실패." )
            << END_LOG;
    }
}

void KNexonAuthManager::EndThread()
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

void KNexonAuthManager::Tick()
{
    CheckConnection();

    KeepConnection();
}

void KNexonAuthManager::QueueingSendPacket( const KNexonAuthPacketPtr& spPacket )
{
    JIF( spPacket );

    KLocker lock( m_csSendQueue );

    m_kSendQueue.push( spPacket );
}

void KNexonAuthManager::Recv()
{
	if( !IsConnected() )
	{
        return;
	}

    int ret;
    ret = ::recv( m_sock,
        m_cRecvBuffer + m_iRecvCP,
        MAX_PACKET_SIZE_OF_NEXON_AUTH - m_iRecvCP,
        0 );

    START_LOG( clog, L"패킷 받음." )
        << BUILD_LOG( ret );

    if( ret == SOCKET_ERROR )
    {
        START_LOG( cerr, GET_WSA_MSG );
        CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= L"Nexon Auth";
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( m_kNexonAuthInfo.m_strIP );
		kNot.m_wstrReason			= L"유효 하지 않은 소켓";
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}

		//{{ 2012. 01. 27  김민성	로그인 인증서버 연결 끊김 자동 sms 전송
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
		{
			KE_DISCONNECT_SERVER_REPORT_NOT kNot;
			kNot.m_cType = KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH;
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( 0, NULL, E_DISCONNECT_SERVER_REPORT_NOT, kNot );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
		//}}

        return;
    }

    if( ret == 0 )
    {
        START_LOG( cerr, L"소켓 연결이 끊어짐." )
            << END_LOG;

        CLOSE_SOCKET( m_sock );

		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= L"Nexon Auth";
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( m_kNexonAuthInfo.m_strIP );
		kNot.m_wstrReason			= L"원격지에서 접속 종료";
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
        return;
    }

    m_iRecvCP += ret;

    while( m_iRecvCP >= 4 )
    {
        // 헤더(1) + 사이즈(2) + 타입(1) = 4
        // 모든 패킷이 4바이트 이상임

        unsigned short usLength;
        ::memcpy( &usLength, m_cRecvBuffer + 1, sizeof( usLength ) );

        usLength = ::ntohs( usLength );
        int iTotalPacketSize = usLength + 3;
        //if( iTotalPacketSize > MAX_PACKET_SIZE_OF_NEXON_AUTH ) 김민성
		if( iTotalPacketSize >= MAX_PACKET_SIZE_OF_NEXON_AUTH )
        {
            START_LOG( cerr, L"추출한 패킷 사이즈 이상." )
                << BUILD_LOG( iTotalPacketSize )
                << BUILD_LOG( MAX_PACKET_SIZE_OF_NEXON_AUTH )
                << END_LOG;

            CLOSE_SOCKET( m_sock );

			//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
			CTime kRegDate = CTime::GetCurrentTime();
			KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
			kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
			kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
			kNot.m_wstrDestinationName	= L"Nexon Auth";
			kNot.m_wstrDestinationIP	= KncUtil::toWideString( m_kNexonAuthInfo.m_strIP );
			kNot.m_wstrReason			= boost::str( boost::wformat( L"최대 패킷 사이즈 초과! MaxPacketSize : %d bytes" ) % iTotalPacketSize );
			kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
			KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
			//}}
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

void KNexonAuthManager::Send()
{
	if( !IsConnected() )
	{
        return;
	}

	int ret;
	char buf[MAX_PACKET_SIZE_OF_NEXON_AUTH];
    KNexonAuthPacketPtr spPacket;
	while( GetSendPacket( spPacket ) )
	{
		if( !spPacket )
		{
			START_LOG( cerr, L"포인터 이상." )
				<< BUILD_LOG( m_kSendQueue.size() )
				<< END_LOG;

			continue;
		}

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

bool KNexonAuthManager::Connect()
{
    m_iRecvCP = 0;
	m_sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성

	if( INVALID_SOCKET == m_sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		return false;
	}

	{
		KLocker lock( m_csSendQueue );

		// 새로 접속하는 거라면 sendqueue를 완전히 비우자! 
		// 접속 성공 후 제일 처음 가는 패킷은 초기화 패킷이어야 하기 때문!
		while( !m_kSendQueue.empty() )
		{
			m_kSendQueue.pop();
		}
	}

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( m_kNexonAuthInfo.m_strIP.c_str() );
	addr.sin_port			= htons( m_kNexonAuthInfo.m_usPort );

	int ret = ::connect( m_sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( m_kNexonAuthInfo.m_strIP )
			<< BUILD_LOG( m_kNexonAuthInfo.m_usPort )
			<< END_LOG;

		CLOSE_SOCKET( m_sock );
		return false;
	}

	// 현재 접속 인원 확인
	const BYTE byteSynchronizeType = ( GetKLoginSimLayer()->GetConcurrentUserCount() == 0 ? 0 : m_kNexonAuthInfo.m_iSynchronizeType );

	// 인증 시도
    KENX_AUTH_INITIALIZE_REQ kPacketInit;
    kPacketInit.m_bytePacketType	  = 41;
	kPacketInit.m_byteInitializeType  = 1;
    kPacketInit.m_byteGameSN		  = 29;									  // 엘소드 게임 번호
    kPacketInit.m_byteDomainSN		  = m_kNexonAuthInfo.m_iDomain;
    kPacketInit.m_wstrDomainName	  = m_kNexonAuthInfo.m_wstrDomainName;
	kPacketInit.m_byteSynchronizeType = byteSynchronizeType;				  // [주의] 0:동기화 안함, 1:세션번호 동기화, 2:세션번호,마스터 계정 아이디 동기화
	kPacketInit.m_ulSynchronizeCount  = m_kNexonAuthInfo.m_iSynchronizeCount; // 패킷당 동기화할 대상 세션 수

    boost::shared_ptr< KNexonAuthPacket > spPacket( new KNexonAuthPacket );
    spPacket->Write( kPacketInit );
    QueueingSendPacket( spPacket );

    START_LOG( cout, L"넥슨 PC방 인증 서버 접속 성공! 초기화 패킷 전송!" )
		<< BUILD_LOG( kPacketInit.m_byteGameSN )
		<< BUILD_LOG( kPacketInit.m_byteDomainSN )
		<< BUILD_LOG( kPacketInit.m_wstrDomainName )
		<< BUILD_LOG( byteSynchronizeType )
		<< BUILD_LOG( m_kNexonAuthInfo.m_iSynchronizeCount );

	// 접속성공 이후 1분 뒤에 하트비트를 보내기 위해서 여기서 TickCount저장해둔다.
	m_dwLastHeartBeatTick = ::GetTickCount();
	return true;
}

bool KNexonAuthManager::IsConnected()
{
	return ( m_sock != INVALID_SOCKET );
}

void KNexonAuthManager::KeepConnection()
{
	if( ::GetTickCount() - m_dwLastHeartBeatTick < ms_dwHeartBeatGap )
	{
		return;
	}
	
	if( IsConnected() == false )
	{
		// 접속한 상태가 아니라면 보내지 않는다.
		return;
	}

    m_dwLastHeartBeatTick = ::GetTickCount();

	// 허트빗 보내기
    KENX_AUTH_ALIVE_NOT kPacketNot;
    kPacketNot.m_bytePacketType = 100;
    boost::shared_ptr< KNexonAuthPacket > spPacket( new KNexonAuthPacket );
    spPacket->Write( kPacketNot );
    QueueingSendPacket( spPacket );
}

void KNexonAuthManager::CheckConnection()
{
	if( ::GetTickCount() - m_dwLastConnectionCheckTick < ms_dwConnectionCheckGap )
	{
		return;
	}

    m_dwLastConnectionCheckTick = ::GetTickCount();

	if( !IsConnected() )
	{
		Connect();
	}
}

bool KNexonAuthManager::GetSendPacket( KNexonAuthPacketPtr& spPacket )
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

void KNexonAuthManager::MakeEventFromReceivedPacket()
{
    DumpBuffer( ( BYTE* )m_cRecvBuffer, false );
	KNexonAuthPacket kReceivedPacket;
	if( !kReceivedPacket.ReadFromBuffer( ( BYTE* )m_cRecvBuffer ) )
    {
        START_LOG( cerr, L"버퍼에서 읽기 실패." )
            << END_LOG;

        DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
        return;
    }

	switch( kReceivedPacket.GetPacketType() )
	{
	case KNexonAuthPacket::NA_PT_INITIALIZE:
		{
			KENX_AUTH_INITIALIZE_ACK kPacket;
			kReceivedPacket.Read( kPacket );

			// 응답 왔으면 어떻게 처리할까?
			if( kPacket.m_byteResult == 0 )
			{
				START_LOG( cout, L"넥슨 PC방 인증서버 초기화 성공!" )
					<< BUILD_LOG( kPacket.m_byteInitializeType )
					<< BUILD_LOG( kPacket.m_byteResult )
					<< BUILD_LOG( kPacket.m_byteDomainSN )
					<< BUILD_LOG( kPacket.m_wstrMessage );
			}
			else
			{
				START_LOG( cerr, L"넥슨 PC방 인증서버 초기화 실패!" )
					<< BUILD_LOG( kPacket.m_byteInitializeType )
					<< BUILD_LOG( kPacket.m_byteResult )
					<< BUILD_LOG( kPacket.m_byteDomainSN )
					<< BUILD_LOG( kPacket.m_wstrMessage );

				// 인증 실패했으므로 소켓 연결을 종료한다.
				CLOSE_SOCKET( m_sock );
			}
		}
		break;

	case KNexonAuthPacket::NA_PT_LOGIN:
		{
			KENX_AUTH_LOGIN_ACK kPacket;
			kReceivedPacket.Read( kPacket );

			//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
			std::wstring wstrResult = KncUtil::EraseFrontWord( kPacket.m_wstrUserID, L';' );
			kPacket.m_wstrUserID = wstrResult;
#endif SERV_TOONILAND_CHANNELING
			//}}

			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_LOGIN_SERVER, NULL, ENX_AUTH_LOGIN_ACK, kPacket );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
		break;

	case KNexonAuthPacket::NA_PT_TERMINATE:
		{
			KENX_AUTH_TERMINATE_NOT kPacket;
			kReceivedPacket.Read( kPacket );

			//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
			std::wstring wstrResult = KncUtil::EraseFrontWord( kPacket.m_wstrUserID, L';' );
			kPacket.m_wstrUserID = wstrResult;
#endif SERV_TOONILAND_CHANNELING
			//}}
            
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_LOGIN_SERVER, NULL, ENX_AUTH_TERMINATE_NOT, kPacket );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
		break;

	case KNexonAuthPacket::NA_PT_MESSAGE:
		{
			KENX_AUTH_MESSAGE_NOT kPacket;
			kReceivedPacket.Read( kPacket );

			//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
			std::wstring wstrResult = KncUtil::EraseFrontWord( kPacket.m_wstrUserID, L';' );
			kPacket.m_wstrUserID = wstrResult;
#endif SERV_TOONILAND_CHANNELING
			//}}

			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_LOGIN_SERVER, NULL, ENX_AUTH_MESSAGE_NOT, kPacket );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
		break;

	case KNexonAuthPacket::NA_PT_SYNCHRONIZE:
		{
			KENX_AUTH_SYNCHRONIZE_REQ kPacket;
			kReceivedPacket.Read( kPacket );
			
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_LOGIN_SERVER, NULL, ENX_AUTH_SYNCHRONIZE_REQ, kPacket );
			KBaseServer::GetKObj()->QueueingEvent( spEvent );
		}
		break;

	default:
		{
			START_LOG( cerr, L"패킷 타입 이상." )
				<< BUILD_LOG( kReceivedPacket.GetPacketType() )
				<< END_LOG;

			DumpBuffer( ( BYTE* )m_cRecvBuffer, true );
			return;
		}
		break;
	}
}

void KNexonAuthManager::DumpBuffer( BYTE* buffer, bool bError )
{
    JIF( buffer );
    JIF( sizeof( buffer ) >= 4 );

    unsigned short usLength;
    ::memcpy( &usLength, buffer + 1, sizeof( usLength ) );

    usLength = ::ntohs( usLength );
    int iTotalPacketSize = usLength + 3;

    LIF( iTotalPacketSize <= MAX_PACKET_SIZE_OF_NEXON_AUTH );
    iTotalPacketSize =  std::min< int >( iTotalPacketSize, MAX_PACKET_SIZE_OF_NEXON_AUTH );

    char szBuffer[MAX_PACKET_SIZE_OF_NEXON_AUTH * 3 + 1];
    char szByte[4];

    for( int i = 0; i < iTotalPacketSize; i++ )
    {
        if( ( int )buffer[i] >= 16 )
        {
#ifdef _CONVERT_VS_2010
			_itoa_s( ( int )buffer[i], szByte, 4, 16 );
#else
			::itoa( ( int )buffer[i], szByte, 16 );
#endif _CONVERT_VS_2010
            szBuffer[i * 3] = szByte[0];
            szBuffer[i * 3 + 1] = szByte[1];
            szBuffer[i * 3 + 2] = ' ';
        }
        else
        {
#ifdef _CONVERT_VS_2010
			_itoa_s( ( int )buffer[i], szByte, 4, 16 );
#else
			::itoa( ( int )buffer[i], szByte, 16 );
#endif _CONVERT_VS_2010
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


#endif SERV_PCBANG_AUTH_NEW
//}}


