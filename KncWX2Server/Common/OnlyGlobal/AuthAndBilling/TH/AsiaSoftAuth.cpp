#include "ActorManager.h"
#include "AsiaSoftAuth.h"

#ifdef SERV_COUNTRY_TH

#include "KncSend.h"
#include "AsiaSoftAuthManager.h"
#include "NetError.h"
#include "BaseServer.h"

#include "LoginSimLayer.h"
#include "TCPThreadManager.h"

#include "LoginServer.h"

#define CLASS_TYPE  KAsiaSoftAuth

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

KAsiaSoftAuth::KAsiaSoftAuth( void )
{
}

KAsiaSoftAuth::~KAsiaSoftAuth( void )
{
	End( 15000 );
}

void KAsiaSoftAuth::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
		//{{ 허상형 : [2012/6/25] //	AsiaSoft Auth
		CASE( EPUBLISHER_AUTHENTICATION_REQ );	// EAS_AUTH_LOGIN_REQ
		CASE( EAS_AUTH_LOGIN_ACK );

		CASE( EPUBLISHER_REG_USER_REQ );
		CASE( EPUBLISHER_UNREG_USER_REQ );
		CASE( EAS_GET_IPBONUS_ACK );

		CASE( EAS_AUTH_AKEY_ACK );
		//}} 허상형 : [2012/6/25] //	AsiaSoft Auth
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
	START_LOG( clog, L"accounting req" );

	CTime tCurrentTime = CTime::GetCurrentTime();
	std::wstringstream wstrstm;
	wstrstm << kPacket_.m_wstrMACAddress << boost::str( boost::wformat( L"%02d%02d" ) % tCurrentTime.GetMinute() % tCurrentTime.GetSecond() );
	kPacket_.m_wstrSocketID = wstrstm.str();


	KAsiaSoftAuthManager::RequestInfo info( FIRST_SENDER_UID, kPacket_.m_iActorUID, kPacket_.m_wstrServiceAccountID, kPacket_.m_wstrDomain, kPacket_.m_wstrIP, kPacket_.m_wstrPublisherOTP );
	SiKAsiaSoftAuthManager()->RegisterRequestInfo( KAsiaSoftAuthManager::ASP_AUTH, wstrstm.str(), info );

	boost::shared_ptr< std::string > spPacket( new std::string );
	*spPacket = SiKAsiaSoftAuthManager()->GetLoginPacket( kPacket_ );
	
	SiKAsiaSoftAuthManager()->QueueingSendPacket( KAsiaSoftAuthManager::ASP_AUTH, spPacket );
}

IMPL_ON_FUNC( EAS_AUTH_LOGIN_ACK )
{
	KAsiaSoftAuthManager::RequestInfo info = SiKAsiaSoftAuthManager()->UnregisterRequestInfo( KAsiaSoftAuthManager::ASP_AUTH, kPacket_.m_wstrSockID );

	KEPUBLISHER_AUTHENTICATION_ACK kAck;
	if( info.ServerUID && info.UserUID )
	{
		kAck.m_iOK = NetError::ERR_UNKNOWN;
		kAck.m_wstrID = kPacket_.m_wstrGameID;
		kAck.m_wstrMasterID = SiKAsiaSoftAuthManager()->MakeMasterID( info.wstrDomain, kPacket_.m_wstrGameID );
		kAck.m_wstrSocketID = kPacket_.m_wstrSockID;
		kAck.m_bIsAKeyUser = kPacket_.m_iAkeyFlag == 1 ? true : false;
		kAck.m_wstrAkeyUserID = kPacket_.m_wstrAkeyUserID;
		kAck.m_uiPublisherUID = info.UserUID;


		//{{ 허상형 : [2012/7/23] //	접속 유지 패킷일 경우 처리안함
		if( kAck.m_wstrMasterID == SiKAsiaSoftAuthManager()->MakeMasterID( GetKLoginSimLayer()->GetAuthDomain(), GetKLoginSimLayer()->GetAuthID() ) )
		{
			START_LOG( clog2, L"상형로그 : 접속 유지용 패킷 받음" )
				<< BUILD_LOG( kAck.m_wstrMasterID )
				<< BUILD_LOG( KAsiaSoftAuthManager::ASP_AUTH )
				<< END_LOG;

			SiKAsiaSoftAuthManager()->ClearRequestInfo( KAsiaSoftAuthManager::ASP_AUTH );
			
			return;
		}
		//}} 허상형 : [2012/7/23] //	접속 유지 패킷일 경우 처리안함

		switch( kPacket_.m_iRecvCode )
		{
		case 0:		kAck.m_iOK = NetError::NET_OK;					break;
		case -1002:	kAck.m_iOK = NetError::ERR_GASH_03;				break;
		case -1003:	kAck.m_iOK = NetError::ERR_VERIFY_20;			break;
		case -1004:	kAck.m_iOK = NetError::ERR_VERIFY_20;			break;
		case -9009:	kAck.m_iOK = NetError::ERR_NEW_ACCOUNT_02;		break;
		default:	kAck.m_iOK = NetError::ERR_VERIFY_10;			break;
		}
	}

	START_LOG( clog, L"로그인 결과" )
		<< BUILD_LOG( kPacket_.m_iRecvCode )
		<< BUILD_LOG( kPacket_.m_wstrRecvMsg )
		<< BUILD_LOG( kPacket_.m_wstrGameID )
		<< BUILD_LOG( kPacket_.m_iAkeyFlag )
		<< BUILD_LOG( kPacket_.m_wstrAkeyUserID )
		<< BUILD_LOG( kPacket_.m_wstrSockID )
		<< BUILD_LOG( kAck.m_iOK )
		<< BUILD_LOG( kAck.m_wstrMasterID )
		<< END_LOG;
	
	if( kAck.m_bIsAKeyUser == true )	//	@Key 유저이면 다시한번 체크
	{
		KEAS_AUTH_AKEY_REQ kPacketReq;
		kPacketReq.m_wstrAkeyUserID = kPacket_.m_wstrAkeyUserID;
		kPacketReq.m_wstrDomain = info.wstrDomain;
		kPacketReq.m_wstrAkey = info.wstrAkey;
		kPacketReq.m_wstrIP = info.wstrIP;
		kPacketReq.m_iServerUID = info.ServerUID;

		kPacketReq.m_kEPUBLISHER_AUTHENTICATION_ACK = kAck;

		GetKLoginServer()->SendToAuthTcpThread( EAS_AUTH_AKEY_REQ, kPacketReq );
	}
	else
	{
		SendToChannelUser( info.ServerUID, info.UserUID, EPUBLISHER_AUTHENTICATION_ACK, kAck );
	}
}

IMPL_ON_FUNC( EPUBLISHER_REG_USER_REQ )
{
	START_LOG( clog, L"IPBonus Reg Check req" );

	//{{ 통합하면서 기존코드 그대로 사용하기 위해 패킷 그대로 두고 내용 채우는 형식으로 바꿈
	KEAS_GET_IPBONUS_REQ kPacketReq;
	kPacketReq.m_iUserUID		= kPacket_.m_iActorUID;
	kPacketReq.m_wstrMasterID	= kPacket_.m_wstrServiceAccountID;
	kPacketReq.m_wstrIP			= KncUtil::toWideString(kPacket_.m_strIP);
	kPacketReq.m_bIsLogin		= true; // Reg User 니 로그인
	kPacketReq.m_wstrSockID		= kPacket_.m_wstrSockID;
	kPacketReq.m_iGSUID			= kPacket_.m_iGSUID;
	//}}


	std::wstring wstrUserID, wstrDomain;
	SiKAsiaSoftAuthManager()->GetIDDomain( kPacketReq.m_wstrMasterID, wstrUserID, wstrDomain );

	KAsiaSoftAuthManager::RequestInfo info( kPacketReq.m_iGSUID, kPacketReq.m_iUserUID, wstrUserID, wstrDomain, kPacketReq.m_wstrIP );
	info.m_bIsLogout = false;	// 로그인 임
	

	SiKAsiaSoftAuthManager()->RegisterRequestInfo( KAsiaSoftAuthManager::ASP_IPBONUS, kPacket_.m_wstrSockID, info );

	boost::shared_ptr< std::string > spPacket( new std::string );
	*spPacket = SiKAsiaSoftAuthManager()->GetIPBonusPacket( kPacketReq );

	SiKAsiaSoftAuthManager()->QueueingSendPacket( KAsiaSoftAuthManager::ASP_IPBONUS, spPacket );
}

IMPL_ON_FUNC( EPUBLISHER_UNREG_USER_REQ )
{
	START_LOG( clog, L"IPBonus UnReg Check req" );

	//{{ 통합하면서 기존코드 그대로 사용하기 위해 패킷 그대로 두고 내용 채우는 형식으로 바꿈
	KEAS_GET_IPBONUS_REQ kPacketReq;
	kPacketReq.m_iUserUID		= kPacket_.m_iActorUID;
	kPacketReq.m_wstrMasterID	= kPacket_.m_wstrServiceAccountID;
	kPacketReq.m_wstrIP			= KncUtil::toWideString(kPacket_.m_strIP);
	kPacketReq.m_bIsLogin		= false; // Reg User 니 로그인
	kPacketReq.m_wstrSockID		= kPacket_.m_wstrSockID;
	kPacketReq.m_iGSUID			= kPacket_.m_iGSUID;
	//}}

	std::wstring wstrUserID, wstrDomain;
	SiKAsiaSoftAuthManager()->GetIDDomain( kPacketReq.m_wstrMasterID, wstrUserID, wstrDomain );

	KAsiaSoftAuthManager::RequestInfo info( kPacketReq.m_iGSUID, kPacketReq.m_iUserUID, wstrUserID, wstrDomain, kPacketReq.m_wstrIP );
	info.m_bIsLogout = true;		// 로그아웃 임
	
	SiKAsiaSoftAuthManager()->RegisterRequestInfo( KAsiaSoftAuthManager::ASP_IPBONUS, kPacket_.m_wstrSockID, info );

	boost::shared_ptr< std::string > spPacket( new std::string );
	*spPacket = SiKAsiaSoftAuthManager()->GetIPBonusPacket( kPacketReq );

	SiKAsiaSoftAuthManager()->QueueingSendPacket( KAsiaSoftAuthManager::ASP_IPBONUS, spPacket );
}

IMPL_ON_FUNC( EAS_GET_IPBONUS_ACK )
{

	KAsiaSoftAuthManager::RequestInfo info = SiKAsiaSoftAuthManager()->UnregisterRequestInfo( KAsiaSoftAuthManager::ASP_IPBONUS, kPacket_.m_wstrSockID );

	int iOK = NetError::NET_OK;	// 접속 끊으면 안되서 그냥 NET_OK 처리
	std::wstring wstrServiceAccountID = L"";
	int iCheckState = 0;
	std::wstring wstrSockID = L"";
	int iPCBangType = -1;	// 0은 PC방임

	if( info.ServerUID && info.UserUID )
	{
		wstrServiceAccountID = SiKAsiaSoftAuthManager()->MakeMasterID( info.wstrDomain, info.wstrUserID );

		//{{ 허상형 : [2012/7/23] //	접속 유지 패킷일 경우 처리안함
		if( wstrServiceAccountID == SiKAsiaSoftAuthManager()->MakeMasterID( GetKLoginSimLayer()->GetAuthDomain(), GetKLoginSimLayer()->GetAuthID() ) )
		{
			START_LOG( clog2, L"상형로그 : 접속 유지용 패킷 받음" )
				<< BUILD_LOG( KAsiaSoftAuthManager::ASP_IPBONUS )
				<< END_LOG;

			SiKAsiaSoftAuthManager()->ClearRequestInfo( KAsiaSoftAuthManager::ASP_IPBONUS );

			return;
		}
		//}} 허상형 : [2012/7/23] //	접속 유지 패킷일 경우 처리안함

		iCheckState = kPacket_.m_iRecvCode;
		wstrSockID = kPacket_.m_wstrSockID;
		
		if (0 == kPacket_.m_iRecvCode)
		{
			// 1레벨은 일반유저
			// 실제 PC방 유저는 2레벨 부터임 (2레벨 : 경험치 20%, 3레벨 : 경험치 30% + 아바타)

			iPCBangType = kPacket_.m_iPCBangLevel - 2; // 우리쪽 세팅은 0부터라서임
		}
		
// 		switch( kPacket_.m_iRecvCode )
// 		{
// 		case 0:		iCheckState = NetError::NET_OK;					break;
// 		case -1001:	iCheckState = NetError::ERR_AS_BILLING_02;		break;
// 		case -9009:	iCheckState = NetError::ERR_NEW_ACCOUNT_02;		break;
// 		default:	iCheckState = NetError::ERR_UNKNOWN;			break;
// 		}
	}

	START_LOG( clog, L"IPBonus 조회 결과" )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( iCheckState )
		<< BUILD_LOG( kPacket_.m_iRecvCode )
		<< BUILD_LOG( kPacket_.m_wstrRecvMsg )
		<< BUILD_LOG( kPacket_.m_iPCBangLevel )
		<< BUILD_LOG( kPacket_.m_wstrSockID )
		<< END_LOG;

	
	if (info.m_bIsLogout)
	{
		// 로그아웃 응답인 경우
		KEPUBLISHER_UNREG_USER_ACK kPacketAck;
		kPacketAck.m_iOK = iOK;
		kPacketAck.m_wstrServiceAccountID = wstrServiceAccountID;

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_SERVER, NULL, EPUBLISHER_UNREG_USER_ACK, kPacketAck );
		KBaseServer::GetKObj()->QueueingEvent( spEvent );
	}
	else
	{
		// 로그인 응답인 경우

		KEPUBLISHER_REG_USER_ACK kPacketAck;
		kPacketAck.m_iOK = iOK;
		kPacketAck.m_wstrServiceAccountID = wstrServiceAccountID;

		kPacketAck.m_iCheckState = iCheckState;
		kPacketAck.m_wstrSockID = wstrSockID;
		kPacketAck.m_iPCBangType = iPCBangType; // 우리쪽 세팅은 0부터라서임

		if (kPacketAck.m_iPCBangType >= 0)
		{
			kPacketAck.m_bIsPcBang = true;
		}

		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_LOGIN_SERVER, NULL, EPUBLISHER_REG_USER_ACK, kPacketAck );
		KBaseServer::GetKObj()->QueueingEvent( spEvent );
	}
	
}
//}} 허상형 : [2012/6/25] //	AsiaSoft Auth

IMPL_ON_FUNC( EAS_AUTH_AKEY_ACK )
{
	KEPUBLISHER_AUTHENTICATION_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOk;
	kAck.m_wstrID = kPacket_.m_kEPUBLISHER_AUTHENTICATION_ACK.m_wstrID;
	kAck.m_wstrMasterID = kPacket_.m_kEPUBLISHER_AUTHENTICATION_ACK.m_wstrMasterID;
	kAck.m_wstrSocketID = kPacket_.m_kEPUBLISHER_AUTHENTICATION_ACK.m_wstrSocketID;

	START_LOG( clog, L"@Key auth ack" )
		<< BUILD_LOG( kAck.m_iOK )
		<< BUILD_LOG( kPacket_.m_wstrRecvMessage )
		<< BUILD_LOG( kAck.m_wstrID )
		<< BUILD_LOG( kAck.m_wstrMasterID )
		<< BUILD_LOG( kAck.m_wstrSocketID )
		<< END_LOG;

	SendToChannelUser( kPacket_.m_iServerUID, kPacket_.m_kEPUBLISHER_AUTHENTICATION_ACK.m_uiPublisherUID, EPUBLISHER_AUTHENTICATION_ACK, kAck );
}

#endif SERV_COUNTRY_TH