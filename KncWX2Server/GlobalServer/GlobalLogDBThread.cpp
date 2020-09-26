#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GlobalLogDBThread.h"
#include "GlobalServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"

#define CLASS_TYPE KGlobalLogDBThread
ImplPfID( KGlobalLogDBThread, PI_GLOBAL_LOG_DB );

IMPL_PROFILER_DUMP( KGlobalLogDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		

#ifdef SERV_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.P_QueryStats_INS", L"%d, N\'%s\', %d, %d, %d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#else //SERV_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.lup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#endif //SERV_RENEWAL_SP

		continue;

end_proc:
		START_LOG( cerr, vecDump[ui].m_wstrQuery )
			<< BUILD_LOG( vecDump[ui].m_iMinTime )
			<< BUILD_LOG( vecDump[ui].m_iMaxTime )
			<< BUILD_LOG( vecDump[ui].m_iTotalTime )
			<< BUILD_LOG( vecDump[ui].m_iQueryCount )
			<< BUILD_LOG( iAvg )
			<< BUILD_LOG( vecDump[ui].m_iOver1Sec )
			<< BUILD_LOG( vecDump[ui].m_iQueryFail )
			<< END_LOG;
	}
}

KGlobalLogDBThread::~KGlobalLogDBThread(void)
{
}

void KGlobalLogDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_REQ, KNetAddress );
		//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
		//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
	   _CASE( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
		//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
		//}}
        CASE_NOPARAM( DBE_SERVER_OFF_NOT );

		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		CASE( DBE_INCREASE_DEFENCE_CRYSTAL_REQ );
		CASE( DBE_GET_DEFENCE_CRYSTAL_REQ );
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가

		//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
		CASE_NOPARAM( DBE_JACKPOT_EVENT_INIT_REQ );
		_CASE( DBE_JACKPOT_EVENT_UPDATE_REQ, KJackPotEventWinInfo );
#endif SERV_EVENT_JACKPOT
		//}}
		//{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		CASE( DBE_TICK_PERFORMANCE_LOG_NOT );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		//}}

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
		CASE_NOPARAM( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
#ifdef SERV_ADD_EVENT_DB
#else // SERV_ADD_EVENT_DB
		CASE_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif // SERV_ADD_EVENT_DB
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

		//{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		CASE( DBE_LOG_STATISTICS_INFO_NOT );
#endif SERV_LOG_SYSTEM_NEW
		//}}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
		CASE_NOPARAM( DBE_TIME_ENCHANT_EVENT_INFO_REQ );
		CASE( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
		CASE_NOPARAM( DBE_EXCHANGE_LIMIT_INFO_REQ );
		CASE( DBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ );
		CASE( DBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT );
#endif // SERV_ITEM_EXCHANGE_LIMIT

		//{{ 2012. 09. 06	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
		CASE( DBE_UPDATE_GLOBAL_MISSION_INFO_REQ );
		CASE( DBE_GET_GLOBAL_MISSION_INFO_REQ );
		CASE( DBE_INSERT_CONNECT_EVENT_REQ );
#endif SERV_GLOBAL_MISSION_MANAGER
		//}} 2012. 09. 06	임홍락	글로벌 미션 매니저

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KGlobalLogDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGlobalLogDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

void KGlobalLogDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nGS, nTo, usEventID, char() );
}

_IMPL_ON_FUNC( DBE_SERVER_ON_REQ, KNetAddress )
{
    KDBE_LOGIN_SERVER_ON_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL_VerifyPublicIP", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_GLOBAL 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_By_PublicIP_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_GLOBAL 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_verify_PublicIP", L"%d, N\'%s\'", 
		% KBaseServer::ESC_GLOBAL 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#else //SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_GLOBAL
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_Verify_On_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_GLOBAL
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_verify_server_on", L"%d, N\'%s\'", 
		% KBaseServer::ESC_GLOBAL 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}

    if( !m_kODBC.BeginFetch() )
    {
        START_LOG( cerr, L"글로벌 서버 인증 실패." )
            << BUILD_LOG( kPacket_.m_wstrIP )
			<< END_LOG;

        kPacket.m_iOK = NetError::ERR_VERIFY_02;
        goto end_proc;
    }

	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
		>> kPacket.m_kServerInfo.m_wstrName
		>> kPacket.m_kServerInfo.m_iServerGroupID
		>> kPacket.m_kServerInfo.m_iServerClass
		>> kPacket.m_kServerInfo.m_wstrPublicIP
		>> kPacket.m_kServerInfo.m_wstrPrivateIP
		>> kPacket.m_kServerInfo.m_usMasterPort
		>> kPacket.m_kServerInfo.m_usNCUDPPort
		>> kPacket.m_kServerInfo.m_nMaxUser
		>> kPacket.m_kServerInfo.m_nCurrentUser
		>> kPacket.m_kServerInfo.m_bOn );
#else
	FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
		>> kPacket.m_kServerInfo.m_wstrName
		>> kPacket.m_kServerInfo.m_iServerGroupID
		>> kPacket.m_kServerInfo.m_iServerClass
		>> kPacket.m_kServerInfo.m_wstrIP
		>> kPacket.m_kServerInfo.m_usMasterPort
		>> kPacket.m_kServerInfo.m_usNCUDPPort
		>> kPacket.m_kServerInfo.m_nMaxUser
		>> kPacket.m_kServerInfo.m_nCurrentUser
		>> kPacket.m_kServerInfo.m_bOn );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}    

    m_kODBC.EndFetch();

	// ProxyID를 받아온다.
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LGlobalProxy_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_LGlobalProxy", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"SELECT ProxyID FROM dbo.LGlobalProxy(NOLOCK)", L"WHERE ServerUID = %d", % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_RENEWAL_SP
	if( !m_kODBC.BeginFetch() )
	{
		START_LOG( cerr, L"난 누군가의 로긴 서버다." )
			<< BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_13;
		goto end_proc;
	}

	FETCH_DATA( kPacket.m_iProxyID );
	m_kODBC.EndFetch();

    //서버가 켜졌다고 표시한다.
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD_Enable", L"%d, %d, 1", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket.m_kServerInfo.m_iUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD", L"%d, %d, 1", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_set_server_enable_flag", L"%d, 1", % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();

        if( kPacket.m_iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"서버 켜짐 표시 실패." )
                << BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
                << END_LOG;

            kPacket.m_iOK = NetError::ERR_VERIFY_08;
        }
    }

end_proc:
    //결과 보내기
    SendToServer( DBE_SERVER_ON_ACK, kPacket );
}

//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
_IMPL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ )
{
	KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
	kPacket.m_nOK = NetError::ERR_ODBC_01;
	kPacket.m_iServerUID = KBaseServer::GetKObj()->GetUID();
	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	kPacket.m_iLocalServerGroupID	= kPacket_.m_iServerGroupID;
#endif SERV_EVENT_JACKPOT
	//}}

#ifdef SERV_RENEWAL_SP
	int iServerSetID = KBaseServer::GetKObj()->GetServerGroupID();
	bool bUsePrivateIP = false;
	int iServerType = (int)KBaseServer::ESC_GAME;	// 국내기준 Login Server에는 게임 서버만 붙는다

#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	bUsePrivateIP = true;
#endif // SERV_PRIVATE_IP_SERVER_NETWORKING

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	iServerType = 0;	// 중국의 경우 Login Server에 ChannelServer, GameServer 다 붙을수 있음.
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"exec dbo.P_LServerList_SEL_VerifyConnect", L"%d, %d, N\'%s\', %d, %d", 
		% iServerSetID
		% iServerType
		% kPacket_.m_kNetAddress.m_wstrIP 
		% kPacket_.m_kNetAddress.m_usPort
		% (int)bUsePrivateIP );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_PROFILE( L"exec dbo.P_LServerList_Verify_Connect_SEL", L"%d, %d, N\'%s\', %d, %d", 
		% iServerSetID
		% iServerType
		% kPacket_.m_kNetAddress.m_wstrIP 
		% kPacket_.m_kNetAddress.m_usPort
		% (int)bUsePrivateIP );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	//{{ 2013. 01. 21	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL_VerifyConnect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_verify_connect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#endif //SERV_ALL_RENEWAL_SP
#else
	DO_QUERY( L"exec dbo.lup_verify_server_connect", L"%d, N\'%s\', %d", % KBaseServer::ESC_GAME % kPacket_.m_kNetAddress.m_wstrIP % kPacket_.m_kNetAddress.m_usPort );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}	
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		//{{ 2010. 04. 22  최육사	서버군 전체 통합
		FETCH_DATA( kPacket.m_nUID
			>> kPacket.m_iDBRegServerGroupID
			>> kPacket.m_strName
			>> kPacket.m_sMaxNum );
		//}}

		m_kODBC.EndFetch();

		kPacket.m_nOK = NetError::NET_OK;
	}
	else
	{
		START_LOG( cerr, LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_usPort );
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_VERIFY_SERVER_CONNECT_ACK, kPacket );
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

IMPL_ON_FUNC_NOPARAM( DBE_SERVER_OFF_NOT )
{
    //서버 Enable을 끈다
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD_Enable", L"%d, %d, 0", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::GetKObj()->GetUID() );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD", L"%d, %d, 0", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::GetKObj()->GetUID() );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_set_server_enable_flag", L"%d, 0", % GetKGlobalServer()->GetUID() );
#endif //SERV_RENEWAL_SP

    int iOK;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

        if( iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"서버 종료 표시 실패." )
                << BUILD_LOG( GetKGlobalServer()->GetUID() )
                << END_LOG;

            iOK = NetError::ERR_VERIFY_08;
        }
    }

end_proc:
    return;
}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
IMPL_ON_FUNC( DBE_INCREASE_DEFENCE_CRYSTAL_REQ )
{
	int iOK = NetError::ERR_ODBC_01;

	// 보호한 수정을 기록 한다.
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDefense_Crystal_MER", L"%d, %d, N\'%s\', %d", 
														% KBaseServer::GetKObj()->GetServerGroupID() 
														% kPacket_.m_iTargetCrystal
														% kPacket_.m_wsrtStartTime 
														% kPacket_.m_iProtectedCrystalCount );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDefense_Crystal_SET", L"%d, %d, N\'%s\', %d", 
														% KBaseServer::GetKObj()->GetServerGroupID() 
														% kPacket_.m_iTargetCrystal
														% kPacket_.m_wsrtStartTime 
														% kPacket_.m_iProtectedCrystalCount );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_Defense_Crystal", L"%d, N\'%s\', %d", 
														% kPacket_.m_iTargetCrystal
														% kPacket_.m_wsrtStartTime 
														% kPacket_.m_iProtectedCrystalCount );
#endif //SERV_RENEWAL_SP
	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();		
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"월드 미션 정보 입력 실패" )
			<< BUILD_LOG( kPacket_.m_wsrtStartTime )
			<< BUILD_LOG( kPacket_.m_iProtectedCrystalCount )
			<< BUILD_LOG( kPacket_.m_iTargetCrystal )
			<< END_LOG;
	}

end_proc:
	return;
}

IMPL_ON_FUNC( DBE_GET_DEFENCE_CRYSTAL_REQ )
{
	// 미션 시작 시간을 넣어 쌓여 있던 크리스탈 수를 가져 온다
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LDefense_Crystal_SEL", L"%d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() 
																  % kPacket_.m_wsrtStartTime );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_ProtectedCrystalCount", L"N\'%s\'", % kPacket_.m_wsrtStartTime  );
#endif //SERV_RENEWAL_SP

	// 보호된 크리스탈 수 초기화
	kPacket_.m_iProtectedCrystalCount = 0;

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( kPacket_.m_iTargetCrystal
				 >> kPacket_.m_iProtectedCrystalCount );

		START_LOG( cout, L"월드 미션 정보 얻기 성공" )
			<< BUILD_LOG( kPacket_.m_wsrtStartTime )
			<< BUILD_LOG( kPacket_.m_iProtectedCrystalCount )
			<< BUILD_LOG( kPacket_.m_iTargetCrystal )
			<< END_LOG;
	}

end_proc:
	SendToServer( DBE_GET_DEFENCE_CRYSTAL_ACK, kPacket_ );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
IMPL_ON_FUNC_NOPARAM( DBE_JACKPOT_EVENT_INIT_REQ )
{
	KDBE_JACKPOT_EVENT_INIT_ACK kPacket;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LEvent_Jackpot_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_LEvent_Jackpot_GET" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KJackPotEventInitInfo kInfo;
		FETCH_DATA( kInfo.m_wstrCuponNumber
			>> kInfo.m_wstrBeginDate
			>> kInfo.m_wstrEndDate
			>> kInfo.m_iUserUID
			>> kInfo.m_iUnitUID
			>> kInfo.m_iState
			);

		kPacket.m_vecJackpotEventInitInfo.push_back( kInfo );
	}

end_proc:
	SendToServer( DBE_JACKPOT_EVENT_INIT_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_JACKPOT_EVENT_UPDATE_REQ, KJackPotEventWinInfo )
{
	KDBE_JACKPOT_EVENT_UPDATE_ACK kPacket;
	kPacket.m_iOK				= NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID			= kPacket_.m_iUnitUID;
	kPacket.m_wstrCuponNumber	= kPacket_.m_wstrCuponNumber;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LEvent_Jackpot_UPD", L"%d, N\'%s\', %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_wstrCuponNumber
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iState
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LEvent_Jackpot_UPT", L"N\'%s\', %d, %d, %d",
		% kPacket_.m_wstrCuponNumber
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iState
		);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	SendToServer( DBE_JACKPOT_EVENT_UPDATE_ACK, kPacket );
}
#endif SERV_EVENT_JACKPOT
//}}

//{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
IMPL_ON_FUNC( DBE_TICK_PERFORMANCE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	// SimLayer Tick 퍼포먼스 기록
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerSimLayerTickLog_INS", L"%d, %d, %d, %d, N\'%s\', N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iServerUID
		% kPacket_.m_iServerType
		% kPacket_.m_dwMaxTickCount
		% kPacket_.m_wstrMaxTickRegDate
		% kPacket_.m_wstrRegDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerSimLayerTickLog_INS", L"%d, %d, %d, N\'%s\', N\'%s\'", 
		% kPacket_.m_iServerUID
		% kPacket_.m_iServerType
		% kPacket_.m_dwMaxTickCount
		% kPacket_.m_wstrMaxTickRegDate
		% kPacket_.m_wstrRegDate
		);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"SimLayer Tick 퍼포먼스 로그 기록 실패!" )
			<< BUILD_LOG( kPacket_.m_iServerUID )
			<< BUILD_LOG( kPacket_.m_iServerType )
			<< BUILD_LOG( kPacket_.m_dwMaxTickCount )
			<< BUILD_LOG( kPacket_.m_wstrMaxTickRegDate )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}
}
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
//}}

#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
IMPL_ON_FUNC_NOPARAM( DBE_EVENT_UPDATE_REQ )
{
	KDBE_EVENT_UPDATE_ACK kPacket;

	// DB로부터 이벤트 리스트 데이터를 받아온다
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EventInfo_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_event" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KEventInfo kEventInfo;

		FETCH_DATA( kEventInfo.m_iEventUID
			>> kEventInfo.m_wstrEventName
			>> kEventInfo.m_iEventType
			>> kEventInfo.m_wstrBeginDate
			>> kEventInfo.m_wstrEndDate
			>> kEventInfo.m_iScriptID
			//#ifdef SERV_ALLOW_EVENT_ERASE
			>> kEventInfo.m_bOn
			//#else SERV_ALLOW_EVENT_ERASE
			//			>> kEventInfo.m_bEnable
			//#endif SERV_ALLOW_EVENT_ERASE
			);


#ifdef SERV_ALLOW_EVENT_ERASE
		kEventInfo.m_bEnable = true;
#endif SERV_ALLOW_EVENT_ERASE

		kPacket.m_vecEventList.push_back( kEventInfo );
	}

end_proc:
	SendToServer( DBE_EVENT_UPDATE_ACK, kPacket );
}
#endif //SERV_ADD_EVENT_DB
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
#ifdef SERV_ADD_EVENT_DB
#else // SERV_ADD_EVENT_DB
IMPL_ON_FUNC_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ )
{
	KDBE_CHECK_EVENT_UPDATE_ACK		AckPacket;
	int iReleaseTickType = 0;
	int iReleaseTick = 0;

	// ReleaseTick 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ReleaseTick_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_release_tick" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iReleaseTickType
			>> iReleaseTick );
		AckPacket.m_mapReleaseTick.insert( std::make_pair( iReleaseTickType, iReleaseTick ) );
	}

end_proc:
	SendToServer( DBE_CHECK_EVENT_UPDATE_ACK, AckPacket );
}
#endif // SERV_ADD_EVENT_DB
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

//{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
IMPL_ON_FUNC( DBE_LOG_STATISTICS_INFO_NOT )
{
	CTime tCurTime = CTime::GetCurrentTime();
	const std::wstring wstrCurTime = ( std::wstring )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );	
	const UidType iServerUID = KBaseServer::GetKObj()->GetUID();

	BOOST_TEST_FOREACH( const KLogStatInfo&, kInfo, kPacket_.m_vecLogStat )
	{
		int iOK = NetError::ERR_ODBC_01;

		// 서버 에러 로그 통계
		DO_QUERY( L"exec dbo.P_LServer_ErrorLog_INS", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, N\'%s\'", 
			% iServerUID
			% kInfo.m_wstrFunctionName
			% kInfo.m_wstrFileName
			% kInfo.m_wstrLineNum
			% kInfo.m_iCount
			% wstrCurTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"Log Statistics 정보 기록 실패!" )
				<< BUILD_LOG( iServerUID )
				<< BUILD_LOG( kInfo.m_wstrFunctionName )
				<< BUILD_LOG( kInfo.m_wstrFileName )
				<< BUILD_LOG( kInfo.m_wstrLineNum )
				<< BUILD_LOG( kInfo.m_iCount )
				<< BUILD_LOG( wstrCurTime )
				<< END_LOG;
		}
	}	
}
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
IMPL_ON_FUNC_NOPARAM( DBE_TIME_ENCHANT_EVENT_INFO_REQ )
{
	KDBE_TIME_ENCHANT_EVENT_INFO_ACK kPacket;

	DO_QUERY_NO_ARG( L"exec dbo.P_LEvent_RandomEnchant_SEL" );

	while( m_kODBC.Fetch() )
	{
		KTimeEnchantEventInfo	kInfo;
		std::wstring			wstrBeginDate;
		std::wstring			wstrEndDate;

		FETCH_DATA( kInfo.m_iEventUID
			>> wstrBeginDate
			>> wstrEndDate
			>> kInfo.m_cEnchantLevel
			);

		CTime tBeginDate;
		CTime tEndDate;

		if( KncUtil::ConvertStringToCTime( wstrBeginDate, tBeginDate ) == false )
		{
			START_LOG( cerr, L"강화 부여 이벤트 시작 시각 변환 실패" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( wstrBeginDate )
				<< BUILD_LOG( wstrEndDate )
				<< BUILD_LOGc( kInfo.m_cEnchantLevel )
				<< END_LOG;
			continue;
		}

		if( KncUtil::ConvertStringToCTime( wstrEndDate, tEndDate ) == false )
		{
			START_LOG( cerr, L"강화 부여 이벤트 종료 시각 변환 실패" )
				<< BUILD_LOG( kInfo.m_iEventUID )
				<< BUILD_LOG( wstrBeginDate )
				<< BUILD_LOG( wstrEndDate )
				<< BUILD_LOGc( kInfo.m_cEnchantLevel )
				<< END_LOG;
			continue;
		}

        kInfo.m_tBeginDate	= tBeginDate.GetTime();
		kInfo.m_tEndDate	= tEndDate.GetTime();
		kPacket.m_vecTimeEnchantEventInfo.push_back( kInfo );
	}

end_proc:
	SendToServer( DBE_TIME_ENCHANT_EVENT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ )
{
	KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK kPacket;
	kPacket.m_iGSUID				= kPacket_.m_iGSUID;
	kPacket.m_iUnitUID				= kPacket_.m_iUnitUID;
	kPacket.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;
	kPacket.m_usEventID				= kPacket_.m_usEventID;

	int iOK = NetError::ERR_ODBC_01;

	std::wstring wstrRegDate = CTime::GetCurrentTime().Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	DO_QUERY( L"exec dbo.P_LEvent_RandomEnchant_UPD", L"%d, %d, N\'%s\'",
		% kPacket_.m_kTimeEnchantEventInfo.m_iEventUID
		% kPacket_.m_iUnitUID
		% wstrRegDate
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK == NetError::NET_OK )
	{
		kPacket.m_cEnchantLevel = kPacket_.m_kTimeEnchantEventInfo.m_cEnchantLevel;
	}
	else
	{
		kPacket.m_cEnchantLevel = 0;
		START_LOG( cerr, L"특정 시각 강화 부여 이벤트 처리 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_kTimeEnchantEventInfo.m_iEventUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		KDBE_TIME_ENCHANT_EVENT_INFO_NOT kPacketToServer;
		kPacketToServer.m_iTimeEnchantEventUID	= kPacket_.m_iTimeEnchantEventUID;
		kPacketToServer.m_kTimeEnchantEventInfo	= kPacket_.m_kTimeEnchantEventInfo;
		SendToServer( DBE_TIME_ENCHANT_EVENT_INFO_NOT, kPacketToServer );
	}

	SendToUser( LAST_SENDER_UID, DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK, kPacket );
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
IMPL_ON_FUNC_NOPARAM( DBE_EXCHANGE_LIMIT_INFO_REQ )
{
	KDBE_EXCHANGE_LIMIT_INFO_ACK kPacket;
	
	std::map< int, int >				mapTargetID;		// < iLimitUID, iTargetID >
	std::map< int, std::set< int > >	mapTargetItemInfo;	// < iTargetID, iTargetItemID >

	// 제한 정보 가져오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_LExchangeLimitInfo_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_LExchangeLimitInfo_GET" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KExchangeLimitInfo	kInfo;
		std::wstring		wstrBeginDate;
		std::wstring		wstrEndDate;
		int					iTargetID = 0;

		FETCH_DATA( kInfo.m_iLimitUID
			>> iTargetID
			>> kInfo.m_iLimitQuantity
			>> wstrBeginDate
			>> wstrEndDate
			);

		CTime tBeginDate;
		CTime tEndDate;

		if( KncUtil::ConvertStringToCTime( wstrBeginDate, tBeginDate ) == false )
		{
			START_LOG( cerr, L"교환 제한 시작 시각 변환 실패" )
				<< BUILD_LOG( kInfo.m_iLimitUID )
				<< END_LOG;
			continue;
		}

		if( KncUtil::ConvertStringToCTime( wstrEndDate, tEndDate ) == false )
		{
			START_LOG( cerr, L"교환 제한 종료 시각 변환 실패" )
				<< BUILD_LOG( kInfo.m_iLimitUID )
				<< END_LOG;
			continue;
		}

		kInfo.m_tBeginDate	= tBeginDate.GetTime();
		kInfo.m_tEndDate	= tEndDate.GetTime();
		kPacket.m_listExchangeLimitInfo.push_back( kInfo );

		mapTargetID.insert( std::map< int, int >::value_type( kInfo.m_iLimitUID, iTargetID ) );
		mapTargetItemInfo.insert( std::map< int, std::set< int > >::value_type( iTargetID, std::set< int >() ) );
	}

	// 제한 아이템 정보 가져오기
	for( std::map< int, std::set< int > >::iterator it = mapTargetItemInfo.begin(); it != mapTargetItemInfo.end(); ++it )
	{
		DO_QUERY( L"exec dbo.P_LExchangeLimitItem_GET", L"%d", %it->first );
		while( m_kODBC.Fetch() )
		{
			int iDestItemID = 0;
			
			FETCH_DATA( iDestItemID );

			it->second.insert( iDestItemID );
		}
	}

	for( std::list< KExchangeLimitInfo >::iterator it = kPacket.m_listExchangeLimitInfo.begin(); it != kPacket.m_listExchangeLimitInfo.end(); ++it )
	{
		KExchangeLimitInfo& kInfo	= *it;
		int iTargetID				= 0;

		{
			std::map< int, int >::const_iterator itFind = mapTargetID.find( kInfo.m_iLimitUID );
			if( itFind == mapTargetID.end() )
			{
				START_LOG( cerr, L"제한 목록 대상 정보를 가져올 수 없습니다." )
					<< BUILD_LOG( kInfo.m_iLimitUID )
					<< END_LOG;

				continue;
			}
			
			iTargetID = itFind->second;
		}

		{
			std::map< int, std::set< int > >::const_iterator itFind = mapTargetItemInfo.find( iTargetID );
			if( itFind == mapTargetItemInfo.end() )
			{
				START_LOG( cerr, L"제한 목록 대상 정보를 가져올 수 없습니다." )
					<< BUILD_LOG( kInfo.m_iLimitUID )
					<< BUILD_LOG( iTargetID )
					<< END_LOG;

				continue;
			}

			kInfo.m_setDestItemID = itFind->second;
		}
	}

end_proc:
	SendToServer( DBE_EXCHANGE_LIMIT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ )
{
	KDBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK kPacket;
	kPacket.m_iOK					= NetError::ERR_ODBC_01;
	kPacket.m_iExchangeLimitUID		= kPacket_.m_iExchangeLimitUID;

	kPacket.m_iGSUID				= kPacket_.m_iGSUID;
	kPacket.m_iUnitUID				= kPacket_.m_iUnitUID;
	kPacket.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;

	DO_QUERY( L"exec dbo.P_LExchangeLimitLog_INT", L"%d, %d, %d, N\'%s\'",
		% kPacket_.m_iLimitUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iDestItemID
		% kPacket_.m_wstrRegDate
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iRollBackUID
			);
		m_kODBC.EndFetch();
	}

end_proc:
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = NetError::ERR_EXCHANGE_LIMIT_01;

		START_LOG( cerr, L"교환 제한 시스템 유저 정보 업데이트 실패" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGSUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iLimitUID )
			<< BUILD_LOG( kPacket_.m_iDestItemID )
			<< END_LOG;

		KDBE_EXCHANGE_LIMIT_INFO_NOT kPacketToServer;
		kPacketToServer.m_iExchangeLimitUID	= kPacket_.m_iExchangeLimitUID;
		kPacketToServer.m_iLimitUID			= kPacket_.m_iLimitUID;
		SendToServer( DBE_EXCHANGE_LIMIT_INFO_NOT, kPacketToServer );
	}

	SendToUser( LAST_SENDER_UID, DBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LExchangeLimitLog_DEL", L"%d", % kPacket_.m_iRollBackUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"교환 제한 정보 롤백 실패!" )
			<< BUILD_LOG( kPacket_.m_iRollBackUID )
			<< END_LOG;
	}
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
IMPL_ON_FUNC( DBE_UPDATE_GLOBAL_MISSION_INFO_REQ )
{
	int iOK = NetError::ERR_ODBC_01;	

#ifdef SERV_COUNTRY_CN
	// 중국용으로 수정
	int iServerSetID = KBaseServer::GetKObj()->GetServerGroupID();
	DO_QUERY( L"exec dbo.P_LEvent_GlobalMission_UPD", L"%d, N\'%s\', %d",
		% iServerSetID
		% kPacket_.m_wstrGlobalMissionDay
		% kPacket_.m_iNowCount );
#else SERV_COUNTRY_CN
	DO_QUERY( L"exec dbo.P_LEvent_GlobalMission_UPD", L"N\'%s\', %d", % kPacket_.m_wstrGlobalMissionDay
		% kPacket_.m_iNowCount );
#endif SERV_COUNTRY_CN

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();		
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"글로벌 미션 정보 입력 실패" )
			<< BUILD_LOG( kPacket_.m_wstrGlobalMissionDay )
			<< BUILD_LOG( kPacket_.m_iNowCount )
			<< END_LOG;
	}		

end_proc:
	return;
}

IMPL_ON_FUNC( DBE_GET_GLOBAL_MISSION_INFO_REQ )
{
#ifdef SERV_COUNTRY_CN
	// 중국용으로 수정
	int iServerSetID = KBaseServer::GetKObj()->GetServerGroupID();
	//DO_QUERY_NO_ARG( L"exec dbo.P_LEvent_GlobalMission_SEL" );
	DO_QUERY( L"exec dbo.P_LEvent_GlobalMission_SEL", L"%d",
		% iServerSetID
		);
#else SERV_COUNTRY_CN
	DO_QUERY_NO_ARG( L"exec dbo.P_LEvent_GlobalMission_SEL" );
#endif SERV_COUNTRY_CN

	while( m_kODBC.Fetch() )
	{
		std::wstring wstrGlobalMissionBeginDate = L"";
		std::wstring wstrGlobalMissionEndDate = L"";
		CTime tGlobalMissionBeginDate;
		CTime tGlobalMissionEndDate;
		KGlobalMissionInfo kGlobalMissionInfo;
		
		FETCH_DATA( wstrGlobalMissionBeginDate
			>> wstrGlobalMissionEndDate
			>> kGlobalMissionInfo.m_iMissionType
			>> kGlobalMissionInfo.m_iClearCondition
			>> kGlobalMissionInfo.m_iClearCount
			>> kGlobalMissionInfo.m_iNowCount );
        
		if( false == KncUtil::ConvertStringToCTime( wstrGlobalMissionBeginDate, tGlobalMissionBeginDate ) ||
			false == KncUtil::ConvertStringToCTime( wstrGlobalMissionEndDate, tGlobalMissionEndDate ) )
		{
			START_LOG( cout, L"글로벌 미션 정보 얻는 도중 시간 변환 실패" )
				<< BUILD_LOG( wstrGlobalMissionBeginDate )
				<< BUILD_LOG( wstrGlobalMissionEndDate )
				<< BUILD_LOG( kGlobalMissionInfo.m_iMissionType )
				<< BUILD_LOG( kGlobalMissionInfo.m_iClearCondition )
				<< BUILD_LOG( kGlobalMissionInfo.m_iClearCount )
				<< BUILD_LOG( kGlobalMissionInfo.m_iNowCount )
				<< END_LOG;
		}
		else
		{
			CTimeSpan tTimeGap = tGlobalMissionEndDate - tGlobalMissionBeginDate;
            kGlobalMissionInfo.m_iDurationMinute = tTimeGap.GetTotalMinutes();

			kPacket_.m_mapGlobalMissionInfo.insert( std::make_pair( wstrGlobalMissionBeginDate, kGlobalMissionInfo ) );

			START_LOG( cout, L"글로벌 미션 정보 얻기 성공" )
				<< BUILD_LOG( wstrGlobalMissionBeginDate )
				<< BUILD_LOG( kGlobalMissionInfo.m_iMissionType )
				<< BUILD_LOG( kGlobalMissionInfo.m_iClearCondition )
				<< BUILD_LOG( kGlobalMissionInfo.m_iClearCount )
				<< BUILD_LOG( kGlobalMissionInfo.m_iNowCount )
				<< END_LOG;
		}
	}

end_proc:
	SendToServer( DBE_GET_GLOBAL_MISSION_INFO_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_INSERT_CONNECT_EVENT_REQ )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.lup_insert_pirate_EventInfo", L"N\'%s\', %d, N\'%s\', N\'%s\', %d",
		% kPacket_.m_wstrEventName
		% kPacket_.m_iEventType
		% kPacket_.m_wstrNextDayStartTime
		% kPacket_.m_wstrNextDayEndTime
		% kPacket_.m_iScriptID
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"글로벌 미션 보상 이벤트 설정 실패" )
			<< BUILD_LOG( kPacket_.m_wstrEventName )
			<< BUILD_LOG( kPacket_.m_iEventType )
			<< BUILD_LOG( kPacket_.m_wstrNextDayStartTime )
			<< BUILD_LOG( kPacket_.m_wstrNextDayEndTime )
			<< BUILD_LOG( kPacket_.m_iScriptID )
			<< BUILD_LOG( iOK )
			<< END_LOG;
	}	

	START_LOG( cout, L"글로벌 미션 보상 이벤트 시작" )
		<< BUILD_LOG( kPacket_.m_wstrEventName )
		<< BUILD_LOG( kPacket_.m_iEventType )
		<< BUILD_LOG( kPacket_.m_wstrNextDayStartTime )
		<< BUILD_LOG( kPacket_.m_wstrNextDayEndTime )
		<< BUILD_LOG( kPacket_.m_iScriptID )
		<< END_LOG;

end_proc:
	return ;
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}}{ 2012. 09. 03	임홍락	글로벌 미션 매니저