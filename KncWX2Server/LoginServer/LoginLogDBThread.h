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

class KLoginLogDBThread : public KSubThread
{
    DeclareDBThread( KLoginLogDBThread );
	//{{ 2009. 8. 17  최육사	sp profiler	
	DeclareSPProfiler;
	//}}
    DeclPfID;
public:

    KLoginLogDBThread();  // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KLoginLogDBThread(void);

protected:
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );
    void SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID );
	
    //void WriteServerInfoToDB( IN KServerInfo kPacket_, OUT KServerList& kPacket );

    // event handler
   _DECL_ON_FUNC( DBE_SERVER_ON_REQ, KNetAddress );
   //{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
   //#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING   
   //{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
#else
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
   //}}
   //#endif SERV_SERVER_CONNECT_CODE_REFACTORING
   //}}
    //{{ 2008. 5. 27  최육사  체험ID 동접
    DECL_ON_FUNC( DBE_UPDATE_SERVER_INFO_REQ );
    //}}   
	//{{ 2009. 7. 6  최육사		랭킹 개편	
	DECL_ON_FUNC_NOPARAM( DBE_GET_HENIR_RANKING_INFO_REQ );
	DECL_ON_FUNC( DBE_DELETE_RANKING_INFO_NOT );
	DECL_ON_FUNC( DBE_BACKUP_RANKING_INFO_NOT );
	//}}
	//{{ 2010. 06. 01  최육사	동접 처리 개편
#ifdef SERV_CCU_NEW
	DECL_ON_FUNC( DBE_UPDATE_CCU_INFO_NOT );
#endif SERV_CCU_NEW
	//}}
	//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
	DECL_ON_FUNC_NOPARAM( DBE_GET_MODULE_INFO_LIST_REQ );
   _DECL_ON_FUNC( DBE_UPDATE_MODULE_INFO_LIST_REQ, KDBE_GET_MODULE_INFO_LIST_ACK );
#endif SERV_DLL_MANAGER
	//}}
	//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	DECL_ON_FUNC_NOPARAM( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_REQ );
	DECL_ON_FUNC( DBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}
	//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	DECL_ON_FUNC( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	DECL_ON_FUNC( DBE_TICK_PERFORMANCE_LOG_NOT );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}
	//{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	DECL_ON_FUNC( DBE_LOG_STATISTICS_INFO_NOT );
#endif SERV_LOG_SYSTEM_NEW
	//}}

    DECL_ON_FUNC_NOPARAM( DBE_SERVER_OFF_NOT );
};

template < class T >
void KLoginLogDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KLoginLogDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_LOGIN_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KLoginLogDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nGS, -1 };
    KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}
