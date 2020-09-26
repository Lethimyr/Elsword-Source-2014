#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "ChannelFunizenAuthDBThread.h"
#include "ChannelServer.h"
#include "NetError.h"

#ifdef SERV_COUNTRY_IN
#define CLASS_TYPE KChannelFunizenAuthDBThread
ImplPfID( KChannelFunizenAuthDBThread, PI_CHANNEL_AUTH_DB );

KChannelFunizenAuthDBThread::~KChannelFunizenAuthDBThread(void)
{
}

void KChannelFunizenAuthDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {       
	CASE( DBE_PUBLISHER_AUTHENTICATION_REQ );
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }
}

void KChannelFunizenAuthDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KChannelFunizenAuthDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

IMPL_ON_FUNC( DBE_PUBLISHER_AUTHENTICATION_REQ )
{
	KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
	kPacketAck.m_wstrID = kPacket_.m_wstrServiceAccountID;
	kPacketAck.m_iResult = -1;

	// CALL에 의한 호출인 경우 괄호를 꼭 넣어주어야함
	DO_QUERY( L"CALL CheckLogin(", L"\'%s\', \'%s\')", % kPacket_.m_wstrServiceAccountID % kPacket_.m_wstrServicePassword );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacketAck.m_uiPublisherUID
			>> kPacketAck.m_iResult );

		m_kODBC.EndFetch();
	}

	switch( kPacketAck.m_iResult )
	{
	case 0:	// 계정 없음
		//kPacketAck.m_iOK = NetError::ERR_FUNIZEN_00;		// 계정이 존재하지 않습니다.
		break;
	case 1:	// 성공
		kPacketAck.m_iOK = NetError::NET_OK;
		break;
	case 2:	// 비밀번호 틀림
		//kPacketAck.m_iOK = NetError::ERR_FUNIZEN_01;		// 비밀번호가 틀립니다.
		break;
	default:
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		break;
	}
end_proc:
	START_LOG( cerr, L"인증 DB 반환값 확인" )
		<< BUILD_LOG( kPacketAck.m_iResult )
		<< BUILD_LOG( kPacketAck.m_iOK )
		<< BUILD_LOG( kPacketAck.m_uiPublisherUID )
		<< END_LOG;

	SendToUser( FIRST_SENDER_UID, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
}
#endif SERV_COUNTRY_IN