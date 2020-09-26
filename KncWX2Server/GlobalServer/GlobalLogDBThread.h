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

class KGlobalLogDBThread : public KSubThread
{
    DeclareDBThread( KGlobalLogDBThread );
	//{{ 2009. 8. 17  최육사	sp profiler	
	DeclareSPProfiler;
	//}}
    DeclPfID;
public:

    KGlobalLogDBThread();  // 구현부는 없음. 사용하지 않는다. (사용시 linking error 유발)
    virtual ~KGlobalLogDBThread(void);

protected:
	virtual void Run()     // derived from KThread
	{
		std::cout << "KSubThread::Run(), thread start." << std::endl;

		

		DWORD ret;

		while( true )
		{
			ret = ::WaitForSingleObject( m_hKillEvent, 1 );

			if( ret == WAIT_OBJECT_0 ) break;

			if( ret == WAIT_TIMEOUT ) Loop();

			else std::cout << "*** WaitForSingleObject() - return :" << ret << std::endl;

		}
	}
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
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}
    DECL_ON_FUNC_NOPARAM( DBE_SERVER_OFF_NOT );

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	DECL_ON_FUNC( DBE_INCREASE_DEFENCE_CRYSTAL_REQ );
	DECL_ON_FUNC( DBE_GET_DEFENCE_CRYSTAL_REQ );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	DECL_ON_FUNC_NOPARAM( DBE_JACKPOT_EVENT_INIT_REQ );
	_DECL_ON_FUNC( DBE_JACKPOT_EVENT_UPDATE_REQ, KJackPotEventWinInfo );
#endif SERV_EVENT_JACKPOT
	//}}
	//{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	DECL_ON_FUNC( DBE_TICK_PERFORMANCE_LOG_NOT );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
	//}}

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	DECL_ON_FUNC_NOPARAM( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
#ifdef SERV_ADD_EVENT_DB
#else // SERV_ADD_EVENT_DB
	DECL_ON_FUNC_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

	//{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	DECL_ON_FUNC( DBE_LOG_STATISTICS_INFO_NOT );
#endif SERV_LOG_SYSTEM_NEW
	//}}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
	DECL_ON_FUNC_NOPARAM( DBE_TIME_ENCHANT_EVENT_INFO_REQ );
	DECL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
	DECL_ON_FUNC_NOPARAM( DBE_EXCHANGE_LIMIT_INFO_REQ );
	DECL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ );
	DECL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT );
#endif // SERV_ITEM_EXCHANGE_LIMIT

	//{{ 2011. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
	DECL_ON_FUNC( DBE_UPDATE_GLOBAL_MISSION_INFO_REQ );
	DECL_ON_FUNC( DBE_GET_GLOBAL_MISSION_INFO_REQ );
	DECL_ON_FUNC( DBE_INSERT_CONNECT_EVENT_REQ );
#endif SERV_GLOBAL_MISSION_MANAGER
	//}} 2011. 09. 03	임홍락	글로벌 미션 매니저
};

template < class T >
void KGlobalLogDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_GLOBAL_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGlobalLogDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( GetPfID(), 0, PI_GLOBAL_USER, nTo, NULL, usEventID, data );
}

template < class T >
void KGlobalLogDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID, const T& data )
{
    UidType anTrace[2] = { nGS, -1 };
    KncSend( GetPfID(), 0, PI_GS_USER, nTo, anTrace, usEventID, data );
}