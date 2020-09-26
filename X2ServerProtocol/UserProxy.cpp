#include "StdAfx.h"

#include "UserProxy.h"

NiImplementRTTI( KUserProxy, KActorProxy );

#define CLASS_TYPE KUserProxy

KUserProxy::KUserProxy( USER_PROXY_TYPE eType ) :
m_eProxyType( eType ),
m_bIntendedDisconnect( false )
{
}

KUserProxy::~KUserProxy(void)
{
	End( INFINITE );
}

ImplToStringW( KUserProxy )
{
    return START_TOSTRING_PARENTW( KActorProxy );
}

void KUserProxy::OnSocketError()
{    
	KSession::OnSocketError();


	//{{ dmlee 2009.3.11 의도적인 접속 종료가 아니라면 에러 event를 발생한다.
	switch( m_eProxyType )
	{
	default:
	case UPT_INVALID:
		{
			assert( !"invalid proxy type" );
		} break;

	case UPT_GAME_SERVER:
	case UPT_CHANNEL_SERVER:
#ifdef ADD_COLLECT_CLIENT_INFO_PROTOCOL
	case UPT_COLLECT_SERVER:
#endif
		{
			if( false == m_bIntendedDisconnect )
			{
				// 게임 클라이언트에게 연결이 끊어졌음을 알려야 한다.
				KEvent kEvent;
				kEvent.m_usEventID = E_CONNECTION_LOST_NOT;

				// windows msg sendmessage
				PostEvent( &kEvent );
			}
		} break;

	}
	//}} dmlee 2009.3.11 의도적인 접속 종료가 아니라면 에러처리 event를 발생한다.

}

void KUserProxy::OnAuthenticFailed()
{
	KSession::OnAuthenticFailed(); // 아무런 동작을 하지 않지만 일단 호출

	// proxy type이 gameserver일때만 동작하도록 처리
	if( m_eProxyType == UPT_GAME_SERVER )
	{
		char cDummy = 0;
		SendPacket( PI_GS_USER, 0, NULL, E_CHECK_SEQUENCE_COUNT_NOT, cDummy, false, false );
	}
}

void KUserProxy::ProcessEvent( IN KEvent* pkEvent_ )
{
	PostEvent( pkEvent_ );

	KSerializer     ks;
	ks.BeginReading( &pkEvent_->m_kbuff );
}

//template < typename T >
//bool KUserProxy::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
//{
//    return KSession::SendPacket( PI_GS_USER, GetUID(), std::vector< UidType >(), usEventID, data, bLogging, bCompress );
//}
//
//bool KUserProxy::SendID( unsigned short usEvenID )
//{
//    return KSession::SendID( PI_GS_USER, GetUID(), std::vector< UidType >(), usEvenID );
//}