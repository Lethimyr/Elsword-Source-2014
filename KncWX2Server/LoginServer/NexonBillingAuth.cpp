#include "NexonBillingAuth.h"

#include "NexonBillingAuthPacket.h"
#include "NexonBillingAuthManager.h"

#define CLASS_TYPE  KNexonBillingAuth

KNexonBillingAuth::KNexonBillingAuth( void )
{
}

KNexonBillingAuth::~KNexonBillingAuth( void )
{
	End( 15000 );
}

void KNexonBillingAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
    CASE( ENX_USER_LOGIN_REQ );
    CASE( ENX_USER_LOGOUT_REQ );
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

IMPL_ON_FUNC( ENX_USER_LOGIN_REQ )
{
    boost::shared_ptr< KNexonBillingAuthPacket > spPacket( new KNexonBillingAuthPacket );
    spPacket->Write( kPacket_ );
    
    SiKNexonBillingAuthManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( ENX_USER_LOGOUT_REQ )
{
    boost::shared_ptr< KNexonBillingAuthPacket > spPacket( new KNexonBillingAuthPacket );
    spPacket->Write( kPacket_ );

    SiKNexonBillingAuthManager()->QueueingSendPacket( spPacket );
}