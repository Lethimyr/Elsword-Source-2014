#include "ActorManager.h"
#include "FunizenAuth.h"

#ifdef SERV_COUNTRY_IN
#include "KncSend.h"
#include "FunizenAuthManager.h"
#include "NetError.h"
#include "BaseServer.h"

#define CLASS_TYPE  KFunizenAuth

KFunizenAuth::KFunizenAuth( void )
{
}

KFunizenAuth::~KFunizenAuth( void )
{
	End( 15000 );
}

void KFunizenAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{	
		CASE( EPUBLISHER_AUTHENTICATION_REQ );
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
    KDBE_PUBLISHER_AUTHENTICATION_REQ kPacket;
	kPacket.m_wstrServiceAccountID = kPacket_.m_wstrServiceAccountID;
	kPacket.m_wstrServicePassword = kPacket_.m_wstrServicePassword;

	SendToFunizenAuthDB( kPacket_.m_iActorUID, DBE_PUBLISHER_AUTHENTICATION_REQ, kPacket );
}
#endif SERV_COUNTRY_IN