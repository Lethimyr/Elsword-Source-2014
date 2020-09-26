#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "LoginOtpAuthTcpThread.h"

#ifdef SERV_COUNTRY_TH

#include "LoginServer.h"
#include "NetError.h"
#include "SimLayer.h"
#include "Socket/NetCommon.h"

#include "AsiaSoftAuthManager.h"


#define CLASS_TYPE KLoginOtpAuthTcpThread
ImplPfID( KLoginOtpAuthTcpThread, PI_LOGIN_AS_OTP_THREAD );

KLoginOtpAuthTcpThread::KLoginOtpAuthTcpThread( const std::wstring& wstrIP, const short sPort )
{
}

KLoginOtpAuthTcpThread::~KLoginOtpAuthTcpThread(void)
{
}

void KLoginOtpAuthTcpThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
	   CASE( EAS_AUTH_AKEY_REQ );
        
	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }
}

void KLoginOtpAuthTcpThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nGS, nTo, usEventID, char() );
}

IMPL_ON_FUNC( EAS_AUTH_AKEY_REQ )
{
	KEAS_AUTH_AKEY_ACK kAck;
	std::string strPacket;
	std::string strPacketRecv;

	kAck.m_iOk = NetError::NET_OK;
	kAck.m_kEPUBLISHER_AUTHENTICATION_ACK = kPacket_.m_kEPUBLISHER_AUTHENTICATION_ACK;
	kAck.m_iServerUID = kPacket_.m_iServerUID;
	std::wstring wstrDomain, wstrUserID;

	SOCKET sock = ::socket( AF_INET, SOCK_STREAM, 0 );    // 소켓 생성

	if( INVALID_SOCKET == sock )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< END_LOG;

		kAck.m_iOk = NetError::ERR_VERIFY_21;
		goto end_proc;
	}

	if( AKeyConnect( sock ) == false )
	{
		kAck.m_iOk = NetError::ERR_VERIFY_21;
		goto end_proc;
	}

	strPacket = SiKAsiaSoftAuthManager()->GetAuthAkeyPacket( kPacket_ );
	START_LOG( clog, L"보내는 패킷" )
		<< BUILD_LOG( strPacket.c_str() )
		<< END_LOG;

	if( AKeySend( sock, strPacket ) == false )
	{
		kAck.m_iOk = NetError::ERR_VERIFY_21;
		goto end_proc;
	}

	if( AKeyRecv( sock, strPacketRecv ) == false )
	{
		kAck.m_iOk = NetError::ERR_VERIFY_21;
		goto end_proc;
	}

	START_LOG( clog2, L"받은 AsiaSoft 패킷 그냥" )
		<< BUILD_LOG( strPacketRecv )
		<< END_LOG;
	//////

	CLOSE_SOCKET( sock );

	//	여기까지 오면 패킷을 풀어준다.
	JIF( SiKAsiaSoftAuthManager()->ExtractAkeyPacket( strPacketRecv, kAck ) );

end_proc:
	KEventPtr spEvent( new KEvent );

	spEvent->SetData(PI_NULL, NULL, EAS_AUTH_AKEY_ACK, kAck );
	SiKAsiaSoftAuthManager()->QueueingEvent( spEvent );

	return;
}


bool	KLoginOtpAuthTcpThread::AKeyConnect( IN SOCKET sock )
{
	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= inet_addr( SiKAsiaSoftAuthManager()->GetAKeyIP().c_str() );
	addr.sin_port			= htons( SiKAsiaSoftAuthManager()->GetAKeyPort() );

	int ret = ::connect( sock, ( struct sockaddr* )&addr, sizeof( addr ) );

	if( SOCKET_ERROR == ret )
	{
		START_LOG( cerr, GET_WSA_MSG )
			<< BUILD_LOG( WSAGetLastError() )
			<< BUILD_LOG( SiKAsiaSoftAuthManager()->GetAKeyIP() )
			<< BUILD_LOG( SiKAsiaSoftAuthManager()->GetAKeyPort() )
			<< END_LOG;

		CLOSE_SOCKET( sock );
		return false;
	}

	return true;
}

bool	KLoginOtpAuthTcpThread::AKeySend( IN SOCKET sock, IN std::string sendPacket )
{
	int ret = ::send( sock,
		sendPacket.c_str(),
		sendPacket.size(),
		0 );

	if( ret == SOCKET_ERROR )
	{
		START_LOG( cwarn, GET_WSA_MSG );
		CLOSE_SOCKET( sock );

		return false;
	}

	return true;
}

bool	KLoginOtpAuthTcpThread::AKeyRecv( IN SOCKET sock, OUT std::string &sendRecvPacket )
{
	char	recvBuf[MAX_PACKET_SIZE_OF_GASH_AUTH] = {0};

	int ret = ::recv( sock,
		recvBuf,
		MAX_PACKET_SIZE_OF_GASH_AUTH,
		0 );

	START_LOG( clog, L"패킷 받음." )
		<< BUILD_LOG( ret );

	if( ret == SOCKET_ERROR )
	{
		START_LOG( cerr, GET_WSA_MSG );
		CLOSE_SOCKET( sock );
		return false;
	}

	//////
	sendRecvPacket.clear();
	for( int i = 0; i < ret; i++ )
	{
		sendRecvPacket.push_back( recvBuf[i] );
	}
	sendRecvPacket.push_back( '\0' );

	return true;
}

#endif SERV_COUNTRY_TH