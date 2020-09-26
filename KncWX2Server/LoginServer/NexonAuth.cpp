#include "NexonAuth.h"

#include "NexonAuthPacket.h"
#include "NexonAuthManager.h"


//{{ 2010. 06. 04  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW


#define CLASS_TYPE  KNexonAuth

KNexonAuth::KNexonAuth( void )
{
}

KNexonAuth::~KNexonAuth( void )
{
	End( 15000 );
}

void KNexonAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
    CASE( ENX_AUTH_LOGIN_REQ );
    CASE( ENX_AUTH_LOGOUT_NOT );
	CASE( ENX_AUTH_SYNCHRONIZE_ACK );

	default:
		START_LOG( cerr, L"핸들러가 지정되지 않은 이벤트." )
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

IMPL_ON_FUNC( ENX_AUTH_LOGIN_REQ )
{
    boost::shared_ptr< KNexonAuthPacket > spPacket( new KNexonAuthPacket );
    spPacket->Write( kPacket_ );
    
    SiKNexonAuthManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_AUTH_LOGOUT_NOT )
{
    boost::shared_ptr< KNexonAuthPacket > spPacket( new KNexonAuthPacket );
    spPacket->Write( kPacket_ );

    SiKNexonAuthManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_AUTH_SYNCHRONIZE_ACK )
{
	boost::shared_ptr< KNexonAuthPacket > spPacket( new KNexonAuthPacket );
	spPacket->Write( kPacket_ );

	SiKNexonAuthManager()->QueueingSendPacket( spPacket );
}


#endif SERV_PCBANG_AUTH_NEW
//}}

