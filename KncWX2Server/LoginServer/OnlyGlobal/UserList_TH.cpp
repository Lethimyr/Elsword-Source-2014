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

#ifdef SERV_COUNTRY_TH

#include "../Common/OnlyGlobal/AuthAndBilling/TH/AsiaSoftAuthManager.h"


void KUserList::RequestRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo, std::wstring wstrSockID )
{
	KEPUBLISHER_REG_USER_REQ kPacketReq;
	kPacketReq.m_iActorUID =	kInfo.m_nUserUID;
	kPacketReq.m_wstrServiceAccountID = kInfo.m_wstrUserID;
	in_addr sIP;
	sIP.s_addr = ( u_long )kInfo.m_uiIP;
	kPacketReq.m_strIP = inet_ntoa( sIP );
	kPacketReq.m_wstrSockID = wstrSockID;
	kPacketReq.m_iGSUID = kInfo.m_nGSUID;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, EPUBLISHER_REG_USER_REQ, kPacketReq );
	SiKAsiaSoftAuthManager()->QueueingEvent( spEvent );
}

void KUserList::RequestUnRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo )
{
	// 이거 사용안하고 LogoutPCBang 이거 사용함

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

	KEPUBLISHER_UNREG_USER_REQ kPacketReq;
	kPacketReq.m_iActorUID = kInfo.m_iUserUID;
	kPacketReq.m_wstrServiceAccountID = kInfo.m_wstrUserID;
	kPacketReq.m_strIP = KncUtil::toNarrowString(kInfo.m_wstrIP);
	kPacketReq.m_wstrSockID = kInfo.m_wstrSockID;
	kPacketReq.m_iGSUID = kInfo.m_iGSUID;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, EPUBLISHER_UNREG_USER_REQ, kPacketReq );
	SiKAsiaSoftAuthManager()->QueueingEvent( spEvent );


	START_LOG( clog, "IPBonus Logout 패킷 전송")
		<< BUILD_LOG( kPacketReq.m_iActorUID )
		<< BUILD_LOG( kPacketReq.m_strIP )
		<< BUILD_LOG( kPacketReq.m_wstrServiceAccountID )
		<< BUILD_LOG( kPacketReq.m_wstrSockID )
		<< END_LOG;
}
#endif

#endif // SERV_COUNTRY_TH