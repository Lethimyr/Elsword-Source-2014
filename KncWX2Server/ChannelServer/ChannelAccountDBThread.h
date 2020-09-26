#pragma once
#include "SubThread.h"
#include "odbc/Odbc.h"
#include "KncSend.h"

//{{ 2009. 8. 13  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

class KChannelAccountDBThread : public KSubThread
{
    DeclareDBThread( KChannelAccountDBThread );
    DeclareSPProfiler;
    DeclPfID;
public:

    KChannelAccountDBThread();  // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KChannelAccountDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );    
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

    //void WriteServerInfoToDB( IN KServerInfo kPacket_, OUT KServerList& kPacket );

    // event handler
#ifdef SERV_KOG_OTP_VERIFY   
	DECL_ON_FUNC( DBE_CH_USER_GENERAL_LOGIN_REQ );
	DECL_ON_FUNC( DBE_CH_USER_KOGOTP_LOGIN_REQ );
#endif // SERV_KOG_OTP_VERIFY

	//DECL_ON_FUNC( DBE_GASH_USER_LOGIN_REQ );

	//{{ 2011. 01. 13 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
	DECL_ON_FUNC( DBE_GET_CONCURRENT_USER_LOGIN_REQ );
#endif SERV_CCU_MONITORING_TOOL
	//}}	
	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	DECL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_REQ );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	//{{ 2011. 09. 26  김민성	머신 ID 체크 - 게임 로딩 이전 단계
#ifdef SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	DECL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ );
#endif SERV_MACHINE_ID_CHECK_BEFOR_LOADING
	//}}
	//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	DECL_ON_FUNC( DBE_CHECK_SERVER_SN_REQ );
	DECL_ON_FUNC( DBE_CREATE_SERVER_SN_REQ );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
	//}}

	
};

template < class T >
void KChannelAccountDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CHANNEL_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KChannelAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_CHANNEL_USER, nTo, NULL, usEventID, data );
}



