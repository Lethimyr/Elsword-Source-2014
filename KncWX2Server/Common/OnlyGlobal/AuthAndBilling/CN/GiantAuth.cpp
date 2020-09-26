#include "ActorManager.h"
#include "GiantAuth.h"

#ifdef SERV_COUNTRY_CN

#include "KncSend.h"
#include "GiantAuthManager.h"
#include "NetError.h"

#define CLASS_TYPE  KGiantAuth

template < class T >
void SendToChannelUser( UidType nToChannelUID, UidType nToChannelUser, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_CHANNEL_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToChannelUser ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToChannelUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"다른 서버로 패킷 보내기 실패" )
			<< BUILD_LOG( nToChannelUID )
			<< BUILD_LOG( nToChannelUser )			
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

template < class T >
void SendToGameUser( UidType nToGameUID, UidType nToGameUser, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGameUser ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGameUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"다른 서버로 패킷 보내기 실패" )
			<< BUILD_LOG( nToGameUID )
			<< BUILD_LOG( nToGameUser )			
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

KGiantAuth::KGiantAuth( void )
{
}

KGiantAuth::~KGiantAuth( void )
{
	End( 15000 );
}

void KGiantAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	CASE( EGIANT_COMMON_NULL_SERVER );
	CASE( EGIANT_COMMON_NULL_CLIENT );

	CASE( EPUBLISHER_AUTHENTICATION_REQ );	// EGIANT_AUTH_LOGIN_REQ
    CASE( EGIANT_AUTH_LOGIN_SUCCESS );		
    CASE( EGIANT_AUTH_LOGIN_FAIL );
    
	CASE( EPUBLISHER_SECURITY_AUTH_REQ );	// EGIANT_AUTH_LOGIN_MTCARD

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	CASE( EGIANT_AUTH_DIRECT_CHARGE_REQ );
	CASE( EGIANT_AUTH_DIRECT_CHARGE_ACK );
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

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

IMPL_ON_FUNC( EGIANT_COMMON_NULL_SERVER )
{
	START_LOG( clog, L"HLServer Null Server" )
		<< END_LOG;

	boost::shared_ptr< KGiantAuthPacket > spPacket( new KGiantAuthPacket );
	spPacket->Write( kPacket_ );
	SiKGiantAuthManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EGIANT_COMMON_NULL_CLIENT )
{
	START_LOG( clog, L"HLServer Null Client" )
		<< END_LOG;

	boost::shared_ptr< KGiantAuthPacket > spPacket( new KGiantAuthPacket );
	spPacket->Write( kPacket_ );
	SiKGiantAuthManager()->QueueingSendPacket( spPacket );
};

IMPL_ON_FUNC( EPUBLISHER_AUTHENTICATION_REQ )
{
	KGiantAuthManager::RequestInfo info(FIRST_SENDER_UID, kPacket_.m_iActorUID, kPacket_.m_wstrIP);
	unsigned int iRequestID = SiKGiantAuthManager()->RegisterRequest( info );
	if( iRequestID == 0 )
	{
		START_LOG( cerr, L"RegisterRequest Failed" )
			<< BUILD_LOG( kPacket_.m_iActorUID )
			<< BUILD_LOG( kPacket_.m_uiRequestID )
			<< BUILD_LOG( kPacket_.m_usZone )
			<< BUILD_LOG( kPacket_.m_usGame )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOG( kPacket_.m_wstrServicePassword )
			<< BUILD_LOG( kPacket_.m_wstrIP )
			<< BUILD_LOG( kPacket_.m_wstrMACAddress )
			<< BUILD_LOG( kPacket_.m_wstrPublisherOTP )
			<< END_LOG;
		
		KEPUBLISHER_AUTHENTICATION_ACK kAck;
		kAck.m_iOK = NetError::ERR_UNKNOWN;
		SendToChannelUser( FIRST_SENDER_UID, kPacket_.m_iActorUID, EPUBLISHER_AUTHENTICATION_ACK, kAck );
		return;
	}

	kPacket_.m_uiRequestID = iRequestID;


	START_LOG( clog, L"EGIANT_AUTH_LOGIN_REQ" )
		<< BUILD_LOG( kPacket_.m_iActorUID )
		<< BUILD_LOG( kPacket_.m_uiRequestID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
		<< BUILD_LOG( kPacket_.m_wstrServicePassword )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_wstrMACAddress )
		<< BUILD_LOG( kPacket_.m_wstrPublisherOTP )
		<< END_LOG;

	boost::shared_ptr< KGiantAuthPacket > spPacket( new KGiantAuthPacket );
	spPacket->Write( kPacket_ );
	SiKGiantAuthManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( EGIANT_AUTH_LOGIN_SUCCESS )
{
	START_LOG( clog, L"HLServer Login Success" )
		<< BUILD_LOG( kPacket_.m_uiRequestID )
		<< BUILD_LOG( kPacket_.m_uiUID )				
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiType )
		<< BUILD_LOG( kPacket_.m_wstrEMail )
		<< BUILD_LOG( kPacket_.m_uiState )
		<< BUILD_LOG( kPacket_.m_uiBindType )
		<< BUILD_LOG( kPacket_.m_wstrSuperpasswd )
		<< BUILD_LOG( kPacket_.m_wstrMobileAccount )
		<< END_LOG;


	KGiantAuthManager::RequestInfo info;
	bool bResult = SiKGiantAuthManager()->UnregisterRequest( kPacket_.m_uiRequestID, info );
	if( bResult == false )
	{
		START_LOG( cerr, L"UnregisterRequest Failed" )
			<< BUILD_LOG( kPacket_.m_uiRequestID )
			<< BUILD_LOG( kPacket_.m_uiUID )				
			<< BUILD_LOG( kPacket_.m_wstrName )
			<< BUILD_LOG( kPacket_.m_uiType )
			<< BUILD_LOG( kPacket_.m_wstrEMail )
			<< BUILD_LOG( kPacket_.m_uiState )
			<< BUILD_LOG( kPacket_.m_uiBindType )
			<< BUILD_LOG( kPacket_.m_wstrSuperpasswd )
			<< BUILD_LOG( kPacket_.m_wstrMobileAccount )
			<< END_LOG;

		return;
	}

	KEPUBLISHER_AUTHENTICATION_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_wstrID = kPacket_.m_wstrName;
	kPacketAck.m_uiPublisherUID = kPacket_.m_uiUID;
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	kPacketAck.m_uiAccountType = kPacket_.m_uiType;
#endif SERV_ANTI_ADDICTION_SYSTEM
	kPacketAck.m_uiAccountState = kPacket_.m_uiState;
	kPacketAck.m_wstrIP = info.wstrIP;

	SendToChannelUser( info.ServerUID, info.UserUID, EPUBLISHER_AUTHENTICATION_ACK, kPacketAck );
}

IMPL_ON_FUNC( EGIANT_AUTH_LOGIN_FAIL )
{
	START_LOG( clog, L"HLServer Login Failed" )
		<< BUILD_LOG( kPacket_.m_uiRequestID )
		<< BUILD_LOG( kPacket_.m_iReturnCode )				
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_uiKey )
		<< BUILD_LOG( kPacket_.m_wstrCoordinate )
		<< END_LOG;

	KGiantAuthManager::RequestInfo info;
	bool bResult = SiKGiantAuthManager()->UnregisterRequest( kPacket_.m_uiRequestID, info );
	if( bResult == false )
	{
		START_LOG( cerr, L"UnregisterRequest Failed" )
			<< BUILD_LOG( kPacket_.m_uiRequestID )
			<< BUILD_LOG( kPacket_.m_iReturnCode )				
			<< BUILD_LOG( kPacket_.m_wstrName )
			<< BUILD_LOG( kPacket_.m_uiKey )
			<< BUILD_LOG( kPacket_.m_wstrCoordinate )
			<< END_LOG;

		return;
	}

	KEPUBLISHER_AUTHENTICATION_ACK kAck;	// 실패인 경우 IP 세팅할 필요 없음

	switch( kPacket_.m_iReturnCode )
	{
	case 0:
		kAck.m_iOK = NetError::NET_OK;
		break;
	case KGiantAuthPacket::GAP_RCT_SYSTEM_ERROR:
		kAck.m_iOK = NetError::ERR_GIANT_VERIFY_07;
		break;
	case KGiantAuthPacket::GAP_RCT_ACCOUNT_NOT_EXIST:
		kAck.m_iOK = NetError::ERR_GIANT_VERIFY_06;
		break;
	case KGiantAuthPacket::GAP_RCT_ACCOUNT_PASSWORD_ERROR:
		kAck.m_iOK = NetError::ERR_GIANT_VERIFY_05;
		break;
	case KGiantAuthPacket::GAP_RCT_OTP_PASSWORD_ERROR:
		kAck.m_iOK = NetError::ERR_GIANT_VERIFY_02;
		break;
	case KGiantAuthPacket::GAP_RCT_BINDING_MATRIX_CARD:
		{
			kAck.m_iOK = NetError::ERR_GIANT_VERIFY_01;

			KEPUBLISHER_SECURITY_INFO_NOT kNot;
			kNot.m_iSecurityInfoType = KEPUBLISHER_SECURITY_INFO_NOT::SIT_MATRIX_CARD;
			kNot.m_uiMatrixKey = kPacket_.m_uiKey;
			kNot.m_wstrCoordinate = kPacket_.m_wstrCoordinate;

			SendToChannelUser( info.ServerUID, info.UserUID, EPUBLISHER_SECURITY_INFO_NOT, kNot );
		}
		break;
	case KGiantAuthPacket::GAP_RCT_ACCOUNT_BLOCK:
		kAck.m_iOK = NetError::ERR_GIANT_VERIFY_04;
		break;
	case KGiantAuthPacket::GAP_RCT_MATRIX_NUMBER_ERROR:
		kAck.m_iOK = NetError::ERR_GIANT_VERIFY_03;
		break;
	case KGiantAuthPacket::GAP_RCT_ACCOUNT_NOT_ACTIVATE:
		kAck.m_iOK = NetError::ERR_UNKNOWN;
		break;
	default:
		kAck.m_iOK = NetError::ERR_UNKNOWN;
	}

	SendToChannelUser( info.ServerUID, info.UserUID, EPUBLISHER_AUTHENTICATION_ACK, kAck );

}

IMPL_ON_FUNC( EPUBLISHER_SECURITY_AUTH_REQ )
{

	KGiantAuthManager::RequestInfo info(FIRST_SENDER_UID, kPacket_.m_iUserUID, kPacket_.m_wstrIP);
	unsigned int iRequestID = SiKGiantAuthManager()->RegisterRequest( info );
	if(iRequestID == 0)
	{
		START_LOG( cerr, L"CreateRequestID Failed" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< END_LOG;

		// TODO: 에러 응답 처리

		return;
	}

	kPacket_.m_uiRequestID = iRequestID;


	START_LOG( clog, L"EGIANT_AUTH_LOGIN_MTCARD" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_uiRequestID )
		<< BUILD_LOG( kPacket_.m_usZone )
		<< BUILD_LOG( kPacket_.m_usGame )
		<< BUILD_LOG( kPacket_.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_wstrMatrix )
		<< BUILD_LOG( kPacket_.m_uiMatrixKey )
		<< END_LOG;

	boost::shared_ptr< KGiantAuthPacket > spPacket( new KGiantAuthPacket );
	spPacket->Write( kPacket_ );
	SiKGiantAuthManager()->QueueingSendPacket( spPacket );

}


#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
IMPL_ON_FUNC( EGIANT_AUTH_DIRECT_CHARGE_REQ )
{
	START_LOG( cout, L"[TEST] Direct Charge 주소 요청 GiantAuthManager 에 도착" )
		<< BUILD_LOG( kPacket_.m_uiUserUID )
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
		<< BUILD_LOG( kPacket_.m_ulGameZone )
		<< BUILD_LOG( kPacket_.m_uiUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< END_LOG;

	KGiantAuthManager::RequestInfo info(FIRST_SENDER_UID, kPacket_.m_uiUserUID, kPacket_.m_wstrIP);
	unsigned int iRequestID = SiKGiantAuthManager()->RegisterRequest( info );
	if( iRequestID == 0 )
	{
		START_LOG( cerr, L"RegisterRequest Failed" )
			<< BUILD_LOG( kPacket_.m_uiUserUID )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOG( kPacket_.m_ulGameZone )
			<< BUILD_LOG( kPacket_.m_uiUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
			<< BUILD_LOG( kPacket_.m_wstrIP )
			<< END_LOG;

		KEGIANT_AUTH_DIRECT_CHARGE_ACK kAck;
		kAck.m_iOK = NetError::ERR_UNKNOWN;
		SendToChannelUser( FIRST_SENDER_UID, kPacket_.m_uiUserUID, EGIANT_AUTH_DIRECT_CHARGE_ACK, kAck );
		return;
	}

	START_LOG( clog, L"EGIANT_AUTH_DIRECT_CHARGE_REQ" )
		<< BUILD_LOG( kPacket_.m_uiUserUID )
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
		<< BUILD_LOG( kPacket_.m_ulGameZone )
		<< BUILD_LOG( kPacket_.m_uiUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< END_LOG;

	boost::shared_ptr< KGiantAuthPacket > spPacket( new KGiantAuthPacket );
	spPacket->Write( kPacket_ );
	SiKGiantAuthManager()->QueueingSendPacket( spPacket );
}

IMPL_ON_FUNC( EGIANT_AUTH_DIRECT_CHARGE_ACK )
{
	START_LOG( cout, L"[TEST] HLServer Get Token Ack" )
		<< BUILD_LOG( kPacket_.m_uiUserUID )
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )				
		<< BUILD_LOG( kPacket_.m_uiUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
		<< BUILD_LOG( kPacket_.m_usTokenLen )
		<< BUILD_LOG( kPacket_.m_wstrToken )
		<< END_LOG;


	KGiantAuthManager::RequestInfo info;
	bool bResult = SiKGiantAuthManager()->UnregisterRequest( kPacket_.m_uiUserUID, info );
	if( bResult == false )
	{
		START_LOG( cerr, L"UnregisterRequest Failed" )
			<< BUILD_LOG( kPacket_.m_uiUserUID )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )				
			<< BUILD_LOG( kPacket_.m_uiUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
			<< BUILD_LOG( kPacket_.m_usTokenLen )
			<< BUILD_LOG( kPacket_.m_wstrToken )
			<< END_LOG;

		return;
	}

	KEGS_CASH_DIRECT_CHARGE_CN_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_wstrToken = kPacketAck.m_wstrToken;


	SendToGameUser( info.ServerUID, info.UserUID, ELG_CASH_DIRECT_CHARGE_CN_ACK, kPacketAck );
}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#endif // SERV_COUNTRY_CN