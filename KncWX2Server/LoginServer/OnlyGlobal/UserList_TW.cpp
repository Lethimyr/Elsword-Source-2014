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

#ifdef SERV_COUNTRY_TWHK

#include "../Common/OnlyGlobal/AuthAndBilling/TW/GASHAuthManager.h"


void KUserList::RequestRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo, std::wstring wstrSockID )
{
	// 해당 함수는 RegUser 에서 호출됨
	// Reguser는 ELG_REGISTER_USER_REQ (채널선택), ELG_CHANNEL_CHANGE_REGISTER_USER_REQ(채널이동)
	// 양쪽 다에서 호출됨

	KEPUBLISHER_REG_USER_REQ kPacketReq;
	kPacketReq.m_iActorUID = iUserUID;
	kPacketReq.m_wstrServiceAccountID = kInfo.m_wstrUserID;
	kPacketReq.m_wstrServicePassword = L"";
	in_addr sIP;
	sIP.s_addr = ( u_long )kInfo.m_uiIP;
	kPacketReq.m_strIP = inet_ntoa( sIP );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData(PI_NULL, NULL, EPUBLISHER_REG_USER_REQ, kPacketReq );
	SiKGASHAuthManager()->QueueingEvent( spEvent );
}

void KUserList::RequestUnRegUser( IN const UidType iUserUID, const KGSUserInfo& kInfo )
{
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
}

#endif // SERV_COUNTRY_TWHK