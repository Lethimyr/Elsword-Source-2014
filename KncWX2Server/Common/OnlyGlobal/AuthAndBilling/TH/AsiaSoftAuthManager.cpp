#include <WinSock2.h>
#include "AsiaSoftAuthManager.h"

#ifdef SERV_COUNTRY_TH

#include "Socket/NetCommon.h"
#include "KncSend.h"
#include "BaseServer.h"
#include "AsiaSoftAuth.h"

#include "LoginSimLayer.h"
#include "Socket/NetCommon.h"

#include "NetError.h"

DWORD KAsiaSoftAuthManager::ms_dwConnectionCheckGap[] = { 15 * 1000, 15 * 1000 };
DWORD KAsiaSoftAuthManager::ms_dwKeepConnectionGap[] = { 30 * 60 * 1000, 30 * 60 * 1000 };

ImplToStringW( KAsiaSoftAuthManager )
{
	return stm_;
}

ImplPfID( KAsiaSoftAuthManager, PI_NULL );

ImplementSingleton( KAsiaSoftAuthManager );

KAsiaSoftAuthManager::KAsiaSoftAuthManager()
{
	for( int iPortNum = 0; iPortNum < ASP_NUM; iPortNum++ )
	{
		m_sock[iPortNum] = INVALID_SOCKET;
		m_dwLastConnectionCheckTick[iPortNum] = ::GetTickCount();
		m_dwLastKeepConnectionTick[iPortNum] = ::GetTickCount();

		m_iRecvCP[iPortNum] = 0;
	}
}

KAsiaSoftAuthManager::~KAsiaSoftAuthManager()
{
}

#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;

void KAsiaSoftAuthManager::RegToLua()
{
	lua_tinker::class_add<KAsiaSoftAuthManager>( g_pLua, "KAsiaSoftAuthManager" );
	lua_tinker::class_def<KAsiaSoftAuthManager>( g_pLua, "dump",						&KAsiaSoftAuthManager::Dump );
	lua_tinker::class_def<KAsiaSoftAuthManager>( g_pLua, "InitAsiaSoftAuthValue",       &KAsiaSoftAuthManager::InitAsiaSoftAuthValue );
	lua_tinker::class_def<KAsiaSoftAuthManager>( g_pLua, "InitAsiaSoftAuthAddress",     &KAsiaSoftAuthManager::InitAsiaSoftAuthAddress );
	lua_tinker::class_def<KAsiaSoftAuthManager>( g_pLua, "InitAsiaSoftAKeyAddress",     &KAsiaSoftAuthManager::InitAsiaSoftAKeyAddress );
	lua_tinker::class_def<KAsiaSoftAuthManager>( g_pLua, "InitAsiaSoftIPBonusAddress",     &KAsiaSoftAuthManager::InitAsiaSoftIPBonusAddress );
	lua_tinker::class_def<KAsiaSoftAuthManager>( g_pLua, "SetNumThread",				&KAsiaSoftAuthManager::Init );

	lua_tinker::decl( g_pLua, "AsiaSoftAuthManager", this );
}

void KAsiaSoftAuthManager::InitAsiaSoftAuthValue( const char* szServiceCode, const char* szServiceRegion )
{
	m_kAsiaSoftAuthInfo.m_strServiceCode = szServiceCode;
	m_kAsiaSoftAuthInfo.m_strServiceRegion = szServiceRegion;

	return;
}

void KAsiaSoftAuthManager::InitAsiaSoftAuthAddress( const char* szGconsoleIP, unsigned short usPort )
{
	m_kAsiaSoftAuthInfo.m_strIP[ASP_AUTH] = szGconsoleIP;
	m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[ASP_AUTH] = usPort;

	return;
}

void KAsiaSoftAuthManager::InitAsiaSoftAKeyAddress( const char* szGconsoleIP, unsigned short usPort )
{
	m_strAKeyIP = szGconsoleIP;
	m_usAKeyPort = usPort;

	return;
}


void KAsiaSoftAuthManager::InitAsiaSoftIPBonusAddress( const char* szGconsoleIP, unsigned short usPort )
{
	m_kAsiaSoftAuthInfo.m_strIP[ASP_IPBONUS] = szGconsoleIP;
	m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[ASP_IPBONUS] = usPort;

	return;
}

void KAsiaSoftAuthManager::Init( int nThreadNum )
{

	for( int iPortNum = 0; iPortNum < ASP_NUM; iPortNum++ )
	{
		m_iRecvCP[iPortNum] = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// thread setting : recv는 recvfrom() 함수에서 알아서 block된다.
	m_spThreadRecv[ASP_AUTH] = boost::shared_ptr< KTThread< KAsiaSoftAuthManager > >
		( new KTThread< KAsiaSoftAuthManager >( *this, KAsiaSoftAuthManager::RecvFromAuth, 50 ) );

	m_spThreadSend[ASP_AUTH] = boost::shared_ptr< KTThread< KAsiaSoftAuthManager > >
		( new KTThread< KAsiaSoftAuthManager >( *this, KAsiaSoftAuthManager::SendToAuth, 100 ) );

	m_spThreadRecv[ASP_IPBONUS] = boost::shared_ptr< KTThread< KAsiaSoftAuthManager > >
		( new KTThread< KAsiaSoftAuthManager >( *this, KAsiaSoftAuthManager::RecvFromIPBonus, 50 ) );

	m_spThreadSend[ASP_IPBONUS] = boost::shared_ptr< KTThread< KAsiaSoftAuthManager > >
		( new KTThread< KAsiaSoftAuthManager >( *this, KAsiaSoftAuthManager::SendToIPBonus, 100 ) );

	KThreadManager::Init( nThreadNum );

	START_LOG(cout, L"AsiaSoft TCP Thread 생성 완료")
		<< BUILD_LOG( nThreadNum )
		<< END_LOG;
}

KThread* KAsiaSoftAuthManager::CreateThread()
{
	return new KAsiaSoftAuth;
}

void KAsiaSoftAuthManager::BeginThread()
{
	for( int iPortNum = 0; iPortNum < ASP_NUM; iPortNum++ )
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

	for( int iPortNum = 0; iPortNum < ASP_NUM; iPortNum++ )
	{	
		if( !Connect( iPortNum ) )
		{
			START_LOG( cerr, L"AsiaSoft 빌링 서버 접속 실패." )
				<< BUILD_LOG( iPortNum )
				<< END_LOG;
		}
	}
}

void KAsiaSoftAuthManager::EndThread()
{
	KThreadManager::EndThread();

	for( int iPortNum = 0; iPortNum < ASP_NUM; iPortNum++ )
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

void KAsiaSoftAuthManager::Tick()
{
	CheckConnection();
	KeepConnection();
}

void KAsiaSoftAuthManager::RegisterRequestInfo( int iPortEnum, std::wstring& wstrIndex, RequestInfo info )
{
	std::map< std::wstring, RequestInfo >::iterator mit;

	KLocker lock( m_csUserIDUserUID );
	mit = m_mapRequestInfo[iPortEnum].find( wstrIndex );
	if( mit != m_mapRequestInfo[iPortEnum].end() )
	{
		START_LOG( cerr, L"user id exists." )
			<< BUILD_LOG( iPortEnum )
			<< BUILD_LOG( wstrIndex )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second.wstrUserID )
			<< END_LOG;
	}
	else
	{
		START_LOG( clog2, L"user id reg" )
			<< BUILD_LOG( iPortEnum )
			<< BUILD_LOG( wstrIndex )
			<< BUILD_LOG( info.UserUID )
			<< BUILD_LOG( info.wstrUserID )
			<< BUILD_LOG( info.wstrDomain )
			<< BUILD_LOG( m_mapRequestInfo[iPortEnum].size() )
			<< END_LOG;
	}

	m_mapRequestInfo[iPortEnum][wstrIndex] = info;
}

KAsiaSoftAuthManager::RequestInfo KAsiaSoftAuthManager::UnregisterRequestInfo( int iPortEnum, std::wstring wstrSockID )
{
	std::map< std::wstring, RequestInfo >::iterator mit;

	KLocker lock( m_csUserIDUserUID );
	mit = m_mapRequestInfo[iPortEnum].find( wstrSockID.c_str() );
	if( mit == m_mapRequestInfo[iPortEnum].end() )
	{
		START_LOG( cerr, L"user id not exists." )
			<< BUILD_LOG( wstrSockID )
			<< END_LOG;

		return RequestInfo(0, 0, L"", L"");
	}
	else
	{
		START_LOG( clog2, L"user id unreg" )
			<< BUILD_LOG( wstrSockID )
			<< BUILD_LOG( mit->second.UserUID )
			<< BUILD_LOG( mit->second.wstrUserID )
			<< BUILD_LOG( mit->second.wstrDomain )
			<< END_LOG;
	}

	RequestInfo info = mit->second;
	m_mapRequestInfo[iPortEnum].erase( mit );

	return info;
}

void KAsiaSoftAuthManager::ClearRequestInfo( int iPortEnum )
{
	m_mapRequestInfo[iPortEnum].clear();
}

//{{ 2011.3.16 최초 계정 생성 시 IP 넣기 (HK/TW)
#ifdef SERV_INSERT_ACCOUNT_IP
void KAsiaSoftAuthManager::RegisterUserIDUserIP( std::string& strUserID, std::string& strUserIP )
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

std::string KAsiaSoftAuthManager::UnregisterUserIDUserIP( std::string& strUserID )
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
#endif//SERV_INSERT_ACCOUNT_IP
//}}

void KAsiaSoftAuthManager::QueueingSendPacket( int iPortSelect, const boost::shared_ptr< std::string >& spPacket )
{
	JIF( spPacket );
	{
		KLocker lock( m_csSendQueue[iPortSelect] );

		m_kSendQueue[iPortSelect].push( spPacket );
	}
}

void KAsiaSoftAuthManager::SendTo( int iPortEnum )
{
	JIF( iPortEnum >= 0 && iPortEnum < ASP_NUM );

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

void KAsiaSoftAuthManager::RecvFrom(int iPortEnum)
{
	JIF( iPortEnum >= 0 && iPortEnum < ASP_NUM );

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
		CLOSE_SOCKET( m_sock[iPortEnum] );

		START_LOG( cerr, L"소켓 연결이 끊어짐." )
			<< END_LOG;

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
	START_LOG( clog2, L"받은 AsiaSoft 패킷 그냥" )
		<< BUILD_LOG( strRecv )
		<< END_LOG;
	//////

	int iIndex = 0;
	while( iIndex + 1 <= m_iRecvCP[iPortEnum] )
	{
		//if( m_cRecvBuffer[iPortEnum][iIndex] == ';' )
		if( iIndex + 1 == m_iRecvCP[iPortEnum] )
		{
			char szPacket[MAX_PACKET_SIZE_OF_GASH_AUTH + 1];
			::ZeroMemory( szPacket, MAX_PACKET_SIZE_OF_GASH_AUTH + 1 );
			::memcpy( szPacket, m_cRecvBuffer[iPortEnum], iIndex + 1 );
			std::string strPacket = szPacket;

			START_LOG( clog2, L"받은 AsiaSoft 패킷" )
				<< BUILD_LOG( strPacket )
				<< END_LOG;

			MakeEventFromReceived(iPortEnum, strPacket);
			if( m_iRecvCP[iPortEnum] > iIndex + 1 )
			{
				::memmove( m_cRecvBuffer, m_cRecvBuffer[iPortEnum] + iIndex + 1, m_iRecvCP[iPortEnum] - ( iIndex + 1 ) );
			}
			m_iRecvCP[iPortEnum] -= ( iIndex + 1 );
			iIndex = 0;
			continue;
		}

		iIndex++;
	}
}

void KAsiaSoftAuthManager::RecvFromAuth( )
{
	RecvFrom(ASP_AUTH);
}

void KAsiaSoftAuthManager::SendToAuth( )
{
	SendTo(ASP_AUTH);
}

void KAsiaSoftAuthManager::RecvFromIPBonus( )
{
	RecvFrom(ASP_IPBONUS);
}

void KAsiaSoftAuthManager::SendToIPBonus( )
{
	SendTo(ASP_IPBONUS);
}


bool KAsiaSoftAuthManager::Connect( int iPortSelect )
{
	_JIF( iPortSelect >= 0 && iPortSelect < ASP_NUM, return false );

	if( m_kAsiaSoftAuthInfo.m_strIP[iPortSelect].empty() == true || m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[iPortSelect] == 0 )
	{
		return false;
	}

	m_iRecvCP[iPortSelect] = 0;
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
	addr.sin_addr.s_addr	= inet_addr( m_kAsiaSoftAuthInfo.m_strIP[iPortSelect].c_str() );
	addr.sin_port			= htons( m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[iPortSelect] );

	int ret = ::connect( sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( m_kAsiaSoftAuthInfo.m_strIP[iPortSelect] )
			<< BUILD_LOG( m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[iPortSelect] )
			<< END_LOG;

		CLOSE_SOCKET( sock );
		return false;
	}

	m_sock[iPortSelect] = sock;

	switch( iPortSelect )
	{
	case ASP_AUTH:
		{
			START_LOG( cout2, L"AsiaSoft 인증 서버 접속 성공!" )
				<< BUILD_LOG( m_kAsiaSoftAuthInfo.m_strIP[iPortSelect].c_str() )
				<< BUILD_LOG( m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[iPortSelect] )
				<< END_LOG;
		}
		break;
	case ASP_IPBONUS:
		{
			START_LOG( cout2, L"AsiaSoft IPBonus 서버 접속 성공!" )
				<< BUILD_LOG( m_kAsiaSoftAuthInfo.m_strIP[iPortSelect].c_str() )
				<< BUILD_LOG( m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[iPortSelect] )
				<< END_LOG;
		}
		break;
	}

	//Init();
	
	return true;
}

bool KAsiaSoftAuthManager::IsConnected( int iPortSelect )
{
	return ( m_sock[iPortSelect] != INVALID_SOCKET );
}

void KAsiaSoftAuthManager::CheckConnection()
{
	for( int iPortNum = 0; iPortNum < ASP_NUM; iPortNum++ )
	{
		if( ::GetTickCount() - m_dwLastConnectionCheckTick[iPortNum] < ms_dwConnectionCheckGap[iPortNum] )
		{
			return;
		}

		m_dwLastConnectionCheckTick[iPortNum] = ::GetTickCount();

		if( !IsConnected(iPortNum) )
		{
			Connect(iPortNum);
		}
	}
}

void KAsiaSoftAuthManager::KeepConnection()
{
	for( int iPortNum = 0; iPortNum < ASP_NUM; iPortNum++ )
	{
		if( m_kAsiaSoftAuthInfo.m_strIP[iPortNum].empty() == true || m_kAsiaSoftAuthInfo.m_usAsiaSoft_Port[iPortNum] == 0 )
		{
			continue;
		}

		if( ::GetTickCount() - m_dwLastKeepConnectionTick[iPortNum] < ms_dwKeepConnectionGap[iPortNum] )
		{
			return;
		}

		m_dwLastKeepConnectionTick[iPortNum] = ::GetTickCount();
		
		CTime tCurrentTime = CTime::GetCurrentTime();
		boost::shared_ptr< std::string > spPacket( new std::string );
		std::wstring wstrSockID = L"AF71B81311F0" + boost::str( boost::wformat( L"%02d%02d" ) % tCurrentTime.GetMinute() % tCurrentTime.GetSecond() );	//	임의맥어드레스(12) + 접속분초(4) = (16자)

		switch( iPortNum )
		{
		case ASP_AUTH:
			{
				KEPUBLISHER_AUTHENTICATION_REQ kPacket;	
				kPacket.m_wstrServiceAccountID = GetKLoginSimLayer()->GetAuthID();
				kPacket.m_wstrServicePassword = GetKLoginSimLayer()->GetAuthPassword();
				kPacket.m_wstrDomain = GetKLoginSimLayer()->GetAuthDomain();
				kPacket.m_wstrIP = NetCommon::GetLocalIPW();
				kPacket.m_iActorUID = -1;
				kPacket.m_wstrSocketID = wstrSockID;

				KAsiaSoftAuthManager::RequestInfo info( -1, kPacket.m_iActorUID, kPacket.m_wstrServiceAccountID, kPacket.m_wstrDomain, kPacket.m_wstrIP );
				SiKAsiaSoftAuthManager()->RegisterRequestInfo( KAsiaSoftAuthManager::ASP_AUTH, kPacket.m_wstrSocketID, info );

				*spPacket = GetLoginPacket( kPacket );
				QueueingSendPacket( KAsiaSoftAuthManager::ASP_AUTH, spPacket );
			}break;
		case ASP_IPBONUS:
			{
				KEAS_GET_IPBONUS_REQ kPacket;
				kPacket.m_iUserUID = -1;
				kPacket.m_wstrMasterID =  MakeMasterID(GetKLoginSimLayer()->GetAuthDomain(), GetKLoginSimLayer()->GetAuthID());
				kPacket.m_bIsLogin = true;
				kPacket.m_wstrIP = NetCommon::GetLocalIPW();
				kPacket.m_wstrSockID = wstrSockID;

				KAsiaSoftAuthManager::RequestInfo info( -1, kPacket.m_iUserUID, GetKLoginSimLayer()->GetAuthID(), GetKLoginSimLayer()->GetAuthDomain(), kPacket.m_wstrIP );
				SiKAsiaSoftAuthManager()->RegisterRequestInfo( KAsiaSoftAuthManager::ASP_IPBONUS, kPacket.m_wstrSockID, info );

				*spPacket = GetIPBonusPacket( kPacket );
				QueueingSendPacket( KAsiaSoftAuthManager::ASP_IPBONUS, spPacket );
			}break;
		default:
			{
				START_LOG( cerr, L"포트 종류 이상." )
					<< BUILD_LOG( iPortNum )
					<< END_LOG;
			}break;
		}

		START_LOG( clog, L"상형 로그 : 접속유지 패킷 보냄")
			<< BUILD_LOG( iPortNum )
			<< END_LOG;
	}
}

bool KAsiaSoftAuthManager::GetSendPacket( int iPortSelect, boost::shared_ptr< std::string >& spPacket )
{
	_JIF( iPortSelect >= 0 && iPortSelect < ASP_NUM, return false );

	KLocker lock( m_csSendQueue[iPortSelect] );

	if( m_kSendQueue[iPortSelect].empty() )
	{
		return false;
	}

	spPacket = m_kSendQueue[iPortSelect].front();
	m_kSendQueue[iPortSelect].pop();

	return true;
}

void KAsiaSoftAuthManager::MakeEventFromReceived(int iPortEnum, std::string& strPacket)
{
	JIF( iPortEnum >= 0 && iPortEnum < ASP_NUM );
	
	switch(iPortEnum)
	{
	case ASP_AUTH:
		{
			KEAS_AUTH_LOGIN_ACK kPacketAck;
			KEventPtr spEvent( new KEvent );

			JIF( SiKAsiaSoftAuthManager()->ExtractLoginPacket( strPacket, kPacketAck ) );

			spEvent->SetData(PI_NULL, NULL, EAS_AUTH_LOGIN_ACK, kPacketAck );
			QueueingEvent( spEvent );
		}break;
	case ASP_IPBONUS:
		{
			KEAS_GET_IPBONUS_ACK kPacketAck;
			KEventPtr spEvent( new KEvent );

			JIF( SiKAsiaSoftAuthManager()->ExtractIPBonusPacket( strPacket, kPacketAck ) );

			spEvent->SetData(PI_NULL, NULL, EAS_GET_IPBONUS_ACK, kPacketAck );
			QueueingEvent( spEvent );
		}break;
	default:
		{
			START_LOG( cerr, L"포트 종류 이상." )
				<< BUILD_LOG( iPortEnum )
				<< END_LOG;
		}break;
	}
}


//{{ 허상형 : [2012/6/21] //	AsiaSoft Auth
std::string KAsiaSoftAuthManager::GetLoginPacket( IN KEPUBLISHER_AUTHENTICATION_REQ kPacket_ )
{
	std::string strPasswordMD5 = KncUtil::GetMD5( KncUtil::toNarrowString( kPacket_.m_wstrServicePassword ) );

	std::wstringstream wstrstmReturn;
	wstrstmReturn << kPacket_.m_wstrServiceAccountID << L"|" << KncUtil::toWideString( strPasswordMD5.c_str() ) << L"|" 
		<< kPacket_.m_wstrDomain << L"|" << kPacket_.m_wstrIP << L"|" << kPacket_.m_wstrSocketID << L";";

	return KncUtil::toNarrowString( wstrstmReturn.str() );
}

std::string KAsiaSoftAuthManager::GetIPBonusPacket( IN KEAS_GET_IPBONUS_REQ kPacket_ )
{
	int iIsLogin = kPacket_.m_bIsLogin ? 1 : 2;
	std::wstringstream wstrstmReturn;
	wstrstmReturn << kPacket_.m_wstrMasterID << L"|" << kPacket_.m_wstrIP << L"|" << iIsLogin << L"|" << kPacket_.m_wstrSockID << L";";

	return KncUtil::toNarrowString( wstrstmReturn.str() );
}

std::string KAsiaSoftAuthManager::GetAuthAkeyPacket( IN KEAS_AUTH_AKEY_REQ kPacket_ )
{
	//	note : @key 인증 패킷은 중간에 길이 값이 들어가야 하기때문에 길이 인자값 기준 좌우로 나눈 후 합친다.
	std::wstringstream wstrstmReturnResult, wstrstmReturnRight;
	wstrstmReturnRight << L"|" << kPacket_.m_wstrDomain << L"|" << kPacket_.m_wstrAkeyUserID << L"||" 
		<< kPacket_.m_wstrAkey << L"|" << kPacket_.m_wstrIP << L"|125|" 
		<< kPacket_.m_kEPUBLISHER_AUTHENTICATION_ACK.m_wstrSocketID << L"|9999|\0";

	int iSize = static_cast<int>( wstrstmReturnRight.str().size() ) + 6;	//	+6 = L"|1001|"

	//	자릿수 구하기
	int iChiper, tmp;
	iChiper = 1, tmp = iSize;
	while( tmp >= 10 )
	{
		tmp /= 10;
		iChiper++;
	}

	if( iSize + iChiper >= pow(10, iChiper) )	iChiper++;

	wstrstmReturnResult << L"|1001|" << iSize + iChiper << wstrstmReturnRight.str();
	return KncUtil::toNarrowString( wstrstmReturnResult.str() );
}

//{{ 허상형 : [2012/6/21] //	AsiaSoft Auth
bool KAsiaSoftAuthManager::ExtractLoginPacket( IN std::string strPacket, OUT KEAS_AUTH_LOGIN_ACK& kPacketAck )
{
	char cPacket[MAX_PACKET_SIZE_OF_GASH_AUTH];
	

	if( strPacket.size() > MAX_PACKET_SIZE_OF_GASH_AUTH )
	{
		return false;
	}

	strPacket.copy(cPacket, strPacket.size());
	int iEnd = 0;
	std::string strData;

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_iRecvCode = ::atoi( strData.c_str() );

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_wstrRecvMsg = KncUtil::toWideString( strData );

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_wstrGameID = KncUtil::toWideString( strData );

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_iAkeyFlag = ::atoi( strData.c_str() );
	

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_wstrAkeyUserID = KncUtil::toWideString( strData );

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_wstrSockID = KncUtil::toWideString( strData );

	return true;
}

bool KAsiaSoftAuthManager::ExtractIPBonusPacket( IN std::string strPacket, OUT KEAS_GET_IPBONUS_ACK& kPacketAck )
{
	char cPacket[MAX_PACKET_SIZE_OF_GASH_AUTH];


	if( strPacket.size() > MAX_PACKET_SIZE_OF_GASH_AUTH )
	{
		return false;
	}

	strPacket.copy(cPacket, strPacket.size());
	int iEnd = 0;
	std::string strData;

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_iRecvCode = ::atoi( strData.c_str() );

	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_wstrRecvMsg = KncUtil::toWideString( strData );

	//	Bonus Level
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_iPCBangLevel = ::atoi( strData.c_str() );

	
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_wstrSockID = KncUtil::toWideString( strData );

	return true;
}

bool KAsiaSoftAuthManager::ExtractAkeyPacket( IN std::string strPacket, OUT KEAS_AUTH_AKEY_ACK& kPacketAck )
{
	char cPacket[MAX_PACKET_SIZE_OF_GASH_AUTH];


	if( strPacket.size() > MAX_PACKET_SIZE_OF_GASH_AUTH )
	{
		return false;
	}

	strPacket.copy(cPacket, strPacket.size());
	int iEnd = 1;
	std::string strData;

	//	Command(Not Use)
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	//	Packet Length(Not Use)
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	//	Status
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	kPacketAck.m_iOk = NetError::ERR_UNKNOWN;

	if( strData == "True" )
	{
		kPacketAck.m_iOk = NetError::NET_OK;
	}
	else if( strData == "False" )
	{
		kPacketAck.m_iOk = NetError::ERR_GIANT_VERIFY_02;
	}

	//	Domain(Not Use)
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	//	UserID(Not Use)
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	//	@Key Value(Not Use)
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	//	GameServiceID(Not Use)
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	//	SocketID
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_kEPUBLISHER_AUTHENTICATION_ACK.m_wstrSocketID = KncUtil::toWideString( strData );

	//	Receive Message
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}
	kPacketAck.m_wstrRecvMessage = KncUtil::toWideString( strData );

	//	EndPoint(Not Use)
	strData.clear();
	for( ; iEnd < ( int )strPacket.size() - 1; iEnd++ )
	{
		if( iEnd >= static_cast<signed>(strPacket.size()) )
		{
			break;
		}

		if( cPacket[iEnd] == ';' || cPacket[iEnd] == '|' )
		{
			iEnd++;
			break;
		}

		strData.push_back( cPacket[iEnd] );
	}

	return true;
}

void KAsiaSoftAuthManager::GetIDDomain( IN std::wstring wstrMasterID, OUT std::wstring &wstrUserID, OUT std::wstring &wstrDomain )
{
	int iDotLocation = wstrMasterID.find(L".");

	if( iDotLocation == -1 )
		return;

	wstrDomain = wstrMasterID.substr(0, iDotLocation);
	wstrUserID = wstrMasterID.substr(iDotLocation+1, wstrMasterID.length() - iDotLocation );
}
//}} 허상형 : [2012/6/21] //	AsiaSoft Auth

#endif // SERV_COUNTRY_TH