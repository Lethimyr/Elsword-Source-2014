#pragma once
#include "BaseServer.h"
//#include "KncSend.h"

class KCnServer : public KBaseServer
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclDump;
    DeclPfID;

public:
    KCnServer(void);
    virtual ~KCnServer(void);

	int  GetProxyID()								{ return m_iProxyID; }
	void SetProxyID( int iProxyID )					{ m_iProxyID = iProxyID; }

    // derived from KBaseServer
    virtual bool Init();
    virtual void RegToLua();
    virtual void ShutDown();
    virtual bool DestroyAllSession();         // 접속중인 세션의 종료.

    // derived from KPerformer
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void Tick();

    //////////////////////////////////////////////////////////////////////////

protected:
	virtual void OnServerReadyComplete();

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	void SetActiveLagCheck_LUA( bool bActiveLagCheck );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

	template < class T > void BroadCastAllGS( unsigned short usEventID, const T& data );
	//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
	template < class T > void BroadCastSameGroupGS( unsigned short usEventID, const T& data );
#endif SERV_INTEGRATION
	//}}
	//{{ 2009. 6. 1  최육사		채널이동
	template < class T > void SendToGSServer( UidType nGS, unsigned short usEventID, const T& data );
	//}}
	//{{ 2009. 5. 23  최육사	시간드롭
	template < class T > void SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
	//}}

   _DECL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_CENTER_SERVER_ON_ACK );
   _DECL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
   _DECL_ON_FUNC( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT );    
	//{{ 2009. 4. 22  최육사	대전유저리스트
	DECL_ON_FUNC( ECN_NEW_PVP_USER_INFO_NOT );
	DECL_ON_FUNC( ECN_UPDATE_PVP_USER_INFO_NOT );
   _DECL_ON_FUNC( ECN_CHECK_INVALID_PVP_USER_NOT, UidType );
	//}}
	//{{ 2009. 5. 23  최육사	시간드롭	
	DECL_ON_FUNC( DBE_CHECK_TIME_DROP_RESULT_ACK );
	//}}
	//{{ 2009. 6. 1  최육사		채널이동
	DECL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_REQ );
	DECL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_ACK );
	//}}
	//{{ 2009. 6. 3  최육사		확성기
   _DECL_ON_FUNC( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT );
	//}}

	//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
   _DECL_ON_FUNC( ESR_ORDER_TO_CENTER_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );   
#endif SERV_REALTIME_SCRIPT
	//}}
   //{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
   _DECL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_LOG_SYSTEM_NEW
   //}}

   //{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	DECL_ON_FUNC( DBE_RELEASE_TICK_UPDATE_ACK );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	_DECL_ON_FUNC( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	_DECL_ON_FUNC( ECN_ADD_COMMON_FLAG_NOT, KECN_ADD_COMMON_FLAG_NOT );   
	_DECL_ON_FUNC( ECN_DEL_COMMON_FLAG_NOT, KECN_DEL_COMMON_FLAG_NOT );   
#endif SERV_MODFIY_FLAG_REALTIME_PATCH

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#else //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	DECL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
protected:
	int								m_iProxyID;	

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	boost::timer					m_tTimeProcessCommunicationONOFF;
	bool							m_bServerRunningProcessCommunicationOnOff;
#endif //SERV_PROCESS_COMMUNICATION_KSMS
};

DefKObjectInline( KCnServer, KBaseServer );


template < class T >
void KCnServer::BroadCastAllGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, usEventID, data );
}

//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
#ifdef SERV_INTEGRATION
template < class T >
void KCnServer::BroadCastSameGroupGS( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER_SAME, 0, anTrace, usEventID, data );
}
#endif SERV_INTEGRATION
//}}

//{{ 2009. 6. 1  최육사		채널이동
template < class T >
void KCnServer::SendToGSServer( UidType nGS, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_CN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, nGS, anTrace, usEventID, data );
}
//}}

//{{ 2009. 5. 23  최육사	시간드롭
template < class T >
void KCnServer::SendToGSCharacter( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { nGS, -1 };
	KncSend( PI_CN_SERVER, GetUID(), PI_GS_CHARACTER, nTo, anTrace, usEventID, data );
}
//}}


