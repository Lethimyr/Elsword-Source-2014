#include "ActorManager.h"
#include "PURPLEAuth.h"

#ifdef SERV_COUNTRY_JP

#include "KncSend.h"
#include "PURPLEAuthManager.h"
#include "NetError.h"
#include "BaseServer.h"
#include "HanAuthForSvr.h"

#define CLASS_TYPE  KPURPLEAuth

KPURPLEAuth::KPURPLEAuth( void )
{
}

KPURPLEAuth::~KPURPLEAuth( void )
{
	End( 15000 );
}

void KPURPLEAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	
    CASE( EPUBLISHER_AUTHENTICATION_REQ );
//    CASE( EPURPLE_AUTHENTICATION_ACK );
	default:
		START_LOG( cerr, "핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() ) 
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}



IMPL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ )
{
    START_LOG( clog, L"JP (EPUBLISHER_AUTHENTICATION_REQ)" );

	UidType iActorUID = kPacket_.m_iActorUID;
	char	csID[128] = "";
	char	csPASS[2048] = "";

	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrServiceAccountID.c_str(), -1, csID, 128, NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrServicePassword.c_str(), -1, csPASS, 2048, NULL, NULL );

	
	// 퍼블 인증 함수 //
	char	szOutBuff[2048] = "";
	int ret = HanAuthForSvr( csID, csPASS, szOutBuff, 2048);

	if(ret == 0)
	{
		START_LOG( clog, L"퍼블 인증 함수 성공" )
			<< BUILD_LOG( ret )
			<< END_LOG;

		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iResult = ret;
		kPacketAck.m_wstrID = kPacket_.m_wstrServiceAccountID;
		kPacketAck.m_wstrIP = kPacket_.m_wstrIP;
		kPacketAck.m_iChannelingCode = kPacket_.m_iChannelingCode;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

	}		
	else
	{
		START_LOG( cerr, L"인증 실패." )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOG( ret )
			<< END_LOG;


		KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_VERIFY_20;
		kPacketAck.m_iResult = ret;
		
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_CHANNEL_USER, NULL, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
		KActorManager::GetKObj()->QueueingTo( iActorUID, spEvent );

	}

}

#endif // SERV_COUNTRY_JP