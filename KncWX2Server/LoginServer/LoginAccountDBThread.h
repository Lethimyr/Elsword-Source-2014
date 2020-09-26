#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 17  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

class KLoginAccountDBThread : public KSubThread
{
    DeclareDBThread( KLoginAccountDBThread );
	//{{ 2009. 8. 17  최육사	sp profiler
	DeclareSPProfiler;
	//}}
    DeclPfID;
public:

    KLoginAccountDBThread();   // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KLoginAccountDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
	template < class T > void SendToServer( unsigned short usEventID, const T& data );
	template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	void SendToUser( UidType nTo, unsigned short usEventID );
    void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID );

   _DECL_ON_FUNC( DBE_NEW_USER_JOIN_REQ, KEGS_NEW_USER_JOIN_REQ );
    DECL_ON_FUNC( ELG_NEXON_USER_AUTHENTICATE_REQ );
	//{{ 2009. 5. 26  최육사	채널이동
	DECL_ON_FUNC( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ );
	//}}
	DECL_ON_FUNC( DBE_UPDATE_IS_LOGIN_NOT );
	//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
	DECL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ );
#endif SERV_ACCOUNT_BLOCK
	//}}
	//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
   _DECL_ON_FUNC( DBE_REG_REJECTED_USER_NOT, KELG_REG_REJECTED_USER_NOT );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
   //{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
   _DECL_ON_FUNC( DBE_REG_TRADE_BLOCK_USER_NOT, KELG_REG_REJECTED_USER_NOT );
#endif SERV_HACK_USER_TRADE_BLOCK
   //}}
   //{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
   DECL_ON_FUNC( DBE_GET_HACKING_MODULE_LIST_REQ );
#endif SERV_HACKING_DLL_CHECK
   //}}
   //{{ 2013. 3. 18	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
   DECL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_REQ );
   DECL_ON_FUNC( DBE_ACCOUNT_GET_GUILD_INFO_REQ );
   DECL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
   //}}

#ifdef SERV_COUNTRY_PH
   DECL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ );
   DECL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ );
#endif //SERV_COUNTRY_PH
};

template < class T >
void KLoginAccountDBThread::SendToServer( unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
	KncSend( GetPfID(), 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KLoginAccountDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nGS, -1 };
    KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}


