#include "ActorManager.h"
#include "LoginNetCafe.h"

#ifdef SERV_COUNTRY_JP

#include "KncSend.h"
#include "LoginNetCafeManager.h"
#include "NetError.h"

//{{ 2009. 12. 30  김정협	일본빌링
#include "HanNetCafeForSvr.h"
//}}

#define CLASS_TYPE  KLoginNetCafe

KLoginNetCafe::KLoginNetCafe( void )
{
}

KLoginNetCafe::~KLoginNetCafe( void )
{
	End( 15000 );
}

void KLoginNetCafe::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();
	

	//KSerializer ks;
	//ks.BeginReading( &spEvent_->m_kbuff );

	switch( spEvent_->m_usEventID )
	{
		CASE( EPUBLISHER_REG_USER_REQ );		
		CASE( EPUBLISHER_UNREG_USER_REQ );		
	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
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


IMPL_ON_FUNC( EPUBLISHER_REG_USER_REQ )
{
	KEPUBLISHER_REG_USER_ACK	kPacketAck;
	char charBuf[255] = {0};
	std::string strID;
	
	if(kPacket_.m_wstrServiceAccountID.size() <= 0 || kPacket_.m_wstrServiceAccountID.size() > 100 )
	{
		// ID 비정상적인 상황
		kPacketAck.m_iOK = NetError::NET_OK;
		START_LOG( cerr, L"넷카페 Reg 에러 상황" )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOGc( kPacket_.m_wstrServiceAccountID.size() )			
			<< END_LOG;
	}
	else
	{
		// 정상 처리 

		WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrServiceAccountID.c_str(), -1, charBuf, 255, NULL, NULL );
		strID = charBuf;

		kPacketAck.m_iCheckState =  HanNetCafe_User_Login(strID.c_str(), kPacket_.m_strIP.c_str());

		kPacketAck.m_wstrServiceAccountID = kPacket_.m_wstrServiceAccountID;
		if(kPacketAck.m_iCheckState == HAN_NETCAFE_OK)
		{
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_bIsPcBang = true;
			kPacketAck.m_iPCBangType = 0;	// 일본은 PCBangType이 따로 있는게 아니라서 PcBangType Default 값인 0 세팅

			START_LOG( clog, L"넷카페 아이피로 로그인" )				
				<< BUILD_LOGc( kPacketAck.m_iCheckState )
				<< BUILD_LOG( strID )
				<< BUILD_LOG( kPacket_.m_strIP.c_str() )
				<< END_LOG;
		}
		else if(kPacketAck.m_iCheckState == HAN_NETCAFE_OK_NOT_NETCAFE_IP)
		{
			kPacketAck.m_iOK = NetError::NET_OK;
			START_LOG( clog, L"넷카페 아이피가 아님" )				
				<< BUILD_LOGc( kPacketAck.m_iCheckState )
				<< BUILD_LOG( strID )
				<< BUILD_LOG( kPacket_.m_strIP.c_str() )
				<< END_LOG;
		}
		else
		{
			kPacketAck.m_iOK = NetError::NET_OK;
			START_LOG( cerr, L"넷카페 모듈 릴리즈 에러" )
				<< BUILD_LOGc( kPacketAck.m_iCheckState )			
				<< BUILD_LOG( strID )
				<< BUILD_LOG( kPacket_.m_strIP.c_str() )
				<< END_LOG;
		}

	}
	

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_SERVER, NULL, EPUBLISHER_REG_USER_ACK, kPacketAck );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( EPUBLISHER_UNREG_USER_REQ )
{
	char charBuf[255] = {0};
	std::string strID;

	KEPUBLISHER_UNREG_USER_ACK	kPacketAck;

	if(kPacket_.m_wstrServiceAccountID.size() <= 0 || kPacket_.m_wstrServiceAccountID.size() > 100 )
	{
		// ID 비정상적인 상황
		kPacketAck.m_iOK = NetError::NET_OK;
		START_LOG( cerr, L"넷카페 Unreg 에러 상황" )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOGc( kPacket_.m_wstrServiceAccountID.size() )			
			<< END_LOG;
	}
	else
	{

		WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrServiceAccountID.c_str(), -1, charBuf, 255, NULL, NULL );
		strID = charBuf;

		int ret = HanNetCafe_User_Logout(strID.c_str());

		if(ret == HAN_NETCAFE_OK)
		{
			kPacketAck.m_iOK = NetError::NET_OK;
			START_LOG( clog, L"넷카페 모듈 정상적 로그아웃 완료" )		
				<< BUILD_LOGc( ret )		
				<< BUILD_LOG( strID )			
				<< END_LOG;
		}	
		else if(ret == HAN_NETCAFE_E_NOT_REGISTERED_USER)
		{
			kPacketAck.m_iOK = NetError::NET_OK;
			START_LOG( clog, L"넷카페 로그인 아이디가 아님." )		
				<< BUILD_LOGc( ret )		
				<< BUILD_LOG( strID )			
				<< END_LOG;
		}
		else
		{
			kPacketAck.m_iOK = NetError::NET_OK;
			START_LOG( cerr, L"넷카페 모듈 로그아웃 에러" )
				<< BUILD_LOGc( ret )		
				<< BUILD_LOG( strID )			
				<< END_LOG;
		}
	}

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_SERVER, NULL, EPUBLISHER_UNREG_USER_ACK, kPacketAck );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

#endif //SERV_COUNTRY_JP