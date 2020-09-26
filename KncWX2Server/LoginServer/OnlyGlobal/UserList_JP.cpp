#include "NetError.h"
#include "ActorManager.h"
#include "LoginUser.h"
#include "SimLayer.h"
//{{ 2010. 06. 04  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
#include "NexonAuthManager.h"
#else
#include "NexonBillingAuthManager.h"
#endif SERV_PCBANG_AUTH_NEW
//}}
//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}
//#include "GASHAuthManager.h"

#include "UserList.h"
#include "BaseServer.h"

#ifdef SERV_COUNTRY_JP

#include "../Common/OnlyGlobal/AuthAndBilling/JP/LoginNetCafeManager.h"


void KUserList::RequestRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo, std::wstring wstrSockID )
{
	if( KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_INTERNAL )
	{
		
		KEPUBLISHER_REG_USER_REQ kPacketReq;
		kPacketReq.m_iActorUID = iUserUID;
		kPacketReq.m_wstrServiceAccountID = kInfo.m_wstrUserID;
		kPacketReq.m_wstrServicePassword = L"";
		in_addr sIP;
		sIP.s_addr = ( u_long )kInfo.m_uiIP;
		kPacketReq.m_strIP = inet_ntoa( sIP );

		KEventPtr spEvent( new KEvent );
		spEvent->SetData(PI_NULL, NULL, EPUBLISHER_REG_USER_REQ, kPacketReq );
		SiKLoginNetCafeManager()->QueueingEvent( spEvent );

		START_LOG( cerr, L"JP RequestRegUser" )
			<< BUILD_LOG( kPacketReq.m_iActorUID )
			<< BUILD_LOG( kPacketReq.m_wstrServiceAccountID )
			<< BUILD_LOG( kPacketReq.m_strIP )
			<< END_LOG;

	}
}

void KUserList::RequestUnRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo )
{
	/*
	KEPUBLISHER_UNREG_USER_REQ kPacketReq;
	kPacketReq.m_iActorUID = iUserUID;
	kPacketReq.m_wstrServiceAccountID = kInfo.m_wstrUserID;
	kPacketReq.m_wstrServicePassword = L"";
	in_addr sIP;
	sIP.s_addr = ( u_long )kInfo.m_uiIP;
	kPacketReq.m_strIP = inet_ntoa( sIP );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, EPUBLISHER_UNREG_USER_REQ, kPacketReq );
	SiKGASHAuthManager()->QueueingEvent( spEvent );
	*/
}


#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
void KUserList::LogoutPCBang( const KELG_PCBANG_LOGOUT_NOT& kInfo )
{
	// 일본은 기존 RequestUnRegUser에 버그 있다는 이유로 따로 프로세스 변경함

	if( KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_INTERNAL )
	{
		KEPUBLISHER_UNREG_USER_REQ kPacket;
		kPacket.m_wstrServiceAccountID = kInfo.m_wstrUserID;
		UidType anTrace[2] = { kInfo.m_iUserUID, -1 };
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( 0, anTrace, EPUBLISHER_UNREG_USER_REQ, kPacket );
		SiKLoginNetCafeManager()->QueueingEvent( spEvent );
	}
}
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#endif // SERV_COUNTRY_JP