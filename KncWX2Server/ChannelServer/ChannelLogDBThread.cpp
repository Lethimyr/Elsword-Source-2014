#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "ChannelLogDBThread.h"
#include "ChannelServer.h"
#include "ActorManager.h"
#include "NetError.h"

//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	#include "UdpRelayChecker/UdpRelayChecker.h"
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

#define CLASS_TYPE KChannelLogDBThread
ImplPfID( KChannelLogDBThread, PI_CHANNEL_LOG_DB );

IMPL_PROFILER_DUMP( KChannelLogDBThread )
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

KChannelLogDBThread::~KChannelLogDBThread(void)
{
}

void KChannelLogDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
       _CASE( DBE_SERVER_ON_REQ, KNetAddress );	   
        CASE_NOPARAM( DBE_SERVER_OFF_NOT );
		CASE( DBE_CHANNEL_LIST_REQ );

#ifdef SERVER_GROUP_UI_ADVANCED
		CASE_NOPARAM( DBE_SERVERGROUP_LIST_REQ );
#endif SERVER_GROUP_UI_ADVANCED

		//{{ 2009. 12. 16  최육사	동접툴
		CASE( DBE_GET_CONCURRENT_USER_INFO_REQ );
		//}}
		//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
		CASE( DBE_CHECK_HACKING_TOOL_LIST_REQ );
#endif SERV_HACKING_TOOL_LIST
		//}}

		//{{ 2011. 01. 20 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
		CASE_NOPARAM( DBE_GET_GAME_SERVER_LIST_REQ );
#endif SERV_CCU_MONITORING_TOOL
		//}}
		//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
		CASE_NOPARAM( DBE_GET_CENTER_SERVER_LIST_REQ );
		CASE( DBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT );
#endif SERV_UDP_RELAY_CHECKER
		//}}

#ifdef SERV_LOGIN_RESULT_INFO
		CASE( DBE_INSERT_LOGIN_RESULT_INFO_REQ );
#endif SERV_LOGIN_RESULT_INFO

		//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
		CASE_NOPARAM( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_REQ );
#endif SERV_WAIT_QUEUE_SYSTEM
		//}}
		//{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		CASE( DBE_TICK_PERFORMANCE_LOG_NOT );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		//}}
		//{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		CASE( DBE_LOG_STATISTICS_INFO_NOT );
#endif SERV_LOG_SYSTEM_NEW
		//}}
#ifdef SERV_ID_NETMARBLE_PCBANG
		_CASE( DBE_PCBANG_IP_AND_MAC_INFO_NOT, KECH_PCBANG_IP_AND_MAC_INFO_NOT );	   
#endif //SERV_ID_NETMARBLE_PCBANG

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KChannelLogDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KChannelLogDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

_IMPL_ON_FUNC( DBE_SERVER_ON_REQ, KNetAddress )
{
    KDBE_CHANNEL_SERVER_ON_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL_VerifyPublicIP", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_CHANNEL 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_By_PublicIP_SEL", L"%d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::ESC_CHANNEL % kPacket_.m_wstrIP );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_verify_PublicIP", L"%d, N\'%s\'", % KBaseServer::ESC_CHANNEL % kPacket_.m_wstrIP );
#endif //SERV_RENEWAL_SP
#else //SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_CHANNEL
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_Verify_On_SEL", L"%d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID()	% KBaseServer::ESC_CHANNEL % kPacket_.m_wstrIP );
#endif //SERV_ALL_RENEWAL_SP	
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_verify_server_on", L"%d, N\'%s\'", % KBaseServer::ESC_CHANNEL % kPacket_.m_wstrIP );
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}

    if( !m_kODBC.BeginFetch() )
    {
        START_LOG( cerr, L"채널 서버 인증 실패." )
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

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	// 로긴서버들의 주소를 알아온다.
	kPacket.m_mapLoginList.clear();

	DO_QUERY( L"exec dbo.lup_get_LLoginProxy", L"%d ", % kPacket.m_kServerInfo.m_iServerGroupID );

	while( m_kODBC.Fetch() )
	{
		int iProxyID;
		KNetAddress kAddres;
		FETCH_DATA( iProxyID
			>> kAddres.m_wstrIP
			>> kAddres.m_usPort );

		kPacket.m_mapLoginList.insert( std::make_pair( iProxyID, kAddres ) );
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

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
    DO_QUERY( L"exec dbo.lup_set_server_enable_flag", L"%d, 0", % GetKChannelServer()->GetUID() );
#endif //SERV_RENEWAL_SP

	int iOK = NetError::ERR_ODBC_01;

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

        if( iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"서버 종료 표시 실패." )
                << BUILD_LOG( GetKChannelServer()->GetUID() )
                << END_LOG;

            iOK = NetError::ERR_VERIFY_08;
        }
    }

end_proc:
    return;
}

IMPL_ON_FUNC( DBE_CHANNEL_LIST_REQ )
{
	KDBE_CHANNEL_LIST_ACK kPacket;

	// DB로부터 서버 리스트 데이터를 받아온다
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_SEL_ByServerSetID", L"%d", % kPacket_.m_iServerGroupID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_SEL", L"%d", % kPacket_.m_iServerGroupID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_channel_list", L"%d", % kPacket_.m_iServerGroupID );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KChannelInfo kInfo;

		FETCH_DATA( kInfo.m_iServerUID
			>> kInfo.m_iChannelID
			>> kInfo.m_wstrChannelName
			>> kInfo.m_iServerGroupID
			>> kInfo.m_wstrIP
			>> kInfo.m_usMasterPort
			>> kInfo.m_usNCUDPPort
			>> kInfo.m_iMaxUser
			>> kInfo.m_iCurrentUser
			>> kInfo.m_iCurPartyCount
			>> kInfo.m_iPlayGamePartyCount
			);

		kPacket.m_mapChannelList.insert( std::make_pair( kInfo.m_iChannelID, kInfo ) );
	}

	//{{ 2012. 06. 29	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	{
		std::map<int, KChannelInfo>::const_iterator mit;
		for( mit = kPacket.m_mapChannelList.begin(); mit != kPacket.m_mapChannelList.end(); ++mit )
		{
			KChannelBonusInfo tempChannelBonusInfo;

#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LChannelReward_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID() 
																	% mit->first );
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.lup_get_channel_reward", L"%d", % mit->first );
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				int iChannelID = 0;
				FETCH_DATA( iChannelID
					>> tempChannelBonusInfo.m_iBeginLv
					>> tempChannelBonusInfo.m_iEndLv
					>> tempChannelBonusInfo.m_iPerExp
					>> tempChannelBonusInfo.m_iPerED
					);
				m_kODBC.EndFetch();

				if( iChannelID != mit->first )
				{
					START_LOG( cerr, L"채널 보너스 정보가 이상합니다" )
						<< BUILD_LOG( iChannelID )
						<< BUILD_LOG( mit->first )
						<< END_LOG;
				}

				//if( tempChannelBonusInfo.m_iPerExp > 0  ||  tempChannelBonusInfo.m_iPerED > 0 )
				{
					// 보너스값이 있으면 enable
					tempChannelBonusInfo.m_bEnable = true;
					kPacket.m_mapChannelBonusList.insert(std::make_pair(mit->first, tempChannelBonusInfo));
				}
			}
		}
	}
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}

end_proc:
	SendToServer( DBE_CHANNEL_LIST_ACK, kPacket );
}

//{{ 2009. 12. 16  최육사	동접툴
IMPL_ON_FUNC( DBE_GET_CONCURRENT_USER_INFO_REQ )
{
	KDBE_GET_CONCURRENT_USER_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_cCCUType = kPacket_.m_cCCUType;
	kPacket.m_iServerUID = kPacket_.m_iServerUID;
	kPacket.m_wstrDate = kPacket_.m_wstrDate;

	switch( kPacket_.m_cCCUType )
	{
	case KE_TOOL_GET_CCU_INFO_REQ::CT_TODAY:
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatCCUMinute_Today_SEL", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() 
																		% kPacket_.m_iServerUID );
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec lup_get_today_ccu", L"%d", % kPacket_.m_iServerUID );
#endif //SERV_RENEWAL_SP
		}
		break;

	case KE_TOOL_GET_CCU_INFO_REQ::CT_YESTERDAY:
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatCCUMinute_Yesterday_SEL", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() 
																			% kPacket_.m_iServerUID );
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec lup_get_yesterday_ccu", L"%d", % kPacket_.m_iServerUID );
#endif //SERV_RENEWAL_SP
		}
		break;

	case KE_TOOL_GET_CCU_INFO_REQ::CT_DATE:
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatCCUMinute_SEL", L"%d, %d, N\'%s\'",  % KBaseServer::GetKObj()->GetServerGroupID() 
																			% kPacket_.m_iServerUID 
																			% kPacket_.m_wstrDate );
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec lup_get_date_ccu", L"%d, N\'%s\'", % kPacket_.m_iServerUID % kPacket_.m_wstrDate );
#endif //SERV_RENEWAL_SP
		}
		break;
		
	case KE_TOOL_GET_CCU_INFO_REQ::CT_NOWTIME:
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatCCUMinute_Nowtime_SEL", L"%d, %d, N\'%s\'",  % KBaseServer::GetKObj()->GetServerGroupID() 
																					% kPacket_.m_iServerUID 
																					% kPacket_.m_wstrDate );
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec lup_get_nowtime_ccu", L"%d, N\'%s\'", % kPacket_.m_iServerUID % kPacket_.m_wstrDate );
#endif //SERV_RENEWAL_SP
		}
		break;

	default:
		{
            START_LOG( cerr, L"CCUType이 이상합니다." )
				<< BUILD_LOGc( kPacket_.m_cCCUType )
				<< END_LOG;

			goto end_proc;
		}
		break;
	}

	while( m_kODBC.Fetch() )
	{
		KCCUInfo kInfo;
		int iServerUID = 0;
		std::wstring wstrDate;

		FETCH_DATA( wstrDate
			>> iServerUID
			>> kInfo.m_iCCUCount
			);

		//{{ 2012. 10. 22	박세훈	동접 정보 DB 실패 처리 추가
#ifdef SERV_FIX_THE_CCU_INFO
		if( wstrDate.empty() == true )
		{
			kPacket.m_vecCCUInfo.clear();
			goto end_proc;
		}
#endif SERV_FIX_THE_CCU_INFO
		//}}

		CTime tDate;
		KncUtil::ConvertStringToCTime( wstrDate, tDate );
		kInfo.m_iCCUTime = tDate.GetTime();

		kPacket.m_vecCCUInfo.push_back( kInfo );
	}

	//{{ 2012. 10. 22	박세훈	동접 정보 DB 실패 처리 추가
#ifdef SERV_FIX_THE_CCU_INFO
	if( kPacket.m_vecCCUInfo.empty() == true )
	{
		goto end_proc;
	}
#endif SERV_FIX_THE_CCU_INFO
	//}}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_GET_CONCURRENT_USER_INFO_ACK, kPacket );
}
//}}

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST

IMPL_ON_FUNC( DBE_CHECK_HACKING_TOOL_LIST_REQ )
{
	KDBE_CHECK_HACKING_TOOL_LIST_ACK kPacket;
	kPacket.m_iReleaseTick = kPacket_.m_iReleaseTick;

	// Release Tick 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ReleaseTick_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_release_tick" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		int iAbuserType = 0;
		int iReleaseTick = 0;

		FETCH_DATA( iAbuserType
			>> iReleaseTick );

		if( iAbuserType != 5 )
			continue;

		if( iReleaseTick > kPacket_.m_iReleaseTick )
		{
			// 새로운 ReleaseTick값을 얻는다.
			kPacket.m_iReleaseTick = iReleaseTick;
		}
	}

	// Release Tick값이 올라갔다면 리스트 새로 받는다.
	if( kPacket.m_iReleaseTick > kPacket_.m_iReleaseTick )
	{
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LHackProcessList_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
		DO_QUERY_NO_ARG( L"exec dbo.lup_get_hackprocess_list" );
#endif //SERV_RENEWAL_SP

		while( m_kODBC.Fetch() )
		{
			KHackingToolInfo kInfo;

			FETCH_DATA( kInfo.m_wstrProcessName
				>> kInfo.m_wstrWindowClassName
				>> kInfo.m_cFlag );

			kPacket.m_vecHackingToolList.push_back( kInfo );
		}
	}

end_proc:
	SendToServer( DBE_CHECK_HACKING_TOOL_LIST_ACK, kPacket );
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2011. 01. 13 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
IMPL_ON_FUNC_NOPARAM( DBE_GET_GAME_SERVER_LIST_REQ )
{
	KDBE_GET_GAME_SERVER_LIST_ACK kPacket;
	kPacket.m_iServerCount = 0;

	//서버 목록 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LToolServerList_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_ToolServer_list" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KToolServerList _Server;

		FETCH_DATA( _Server.m_iServerUID
			>> _Server.m_iServerSetID
			>> _Server.m_iServerType
			>> _Server.m_iChannelID
			>> _Server.m_wstrChannelName
			>> _Server.m_wstrIP
			>> _Server.m_iPort	);

		kPacket.m_vecServerList.push_back( _Server );

		kPacket.m_iServerCount++;		
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_GET_GAME_SERVER_LIST_ACK, kPacket );
}
#endif SERV_CCU_MONITORING_TOOL
//}}	

//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
IMPL_ON_FUNC_NOPARAM( DBE_GET_CENTER_SERVER_LIST_REQ )
{
	KDBE_GET_CENTER_SERVER_LIST_ACK kPacket;

	//서버 목록 얻기
	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_By_ServerType_SEL", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::ESC_CENTER );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_byServerType", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::ESC_CENTER );
#endif //SERV_RENEWAL_SP
#else
	DO_QUERY( L"exec dbo.P_LServerList_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % KBaseServer::ESC_CENTER );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}	

	while( m_kODBC.Fetch() )
	{
		KServerInfo kInfo;

		//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
		FETCH_DATA( kInfo.m_iUID
			>> kInfo.m_wstrName
			>> kInfo.m_iServerGroupID
			>> kInfo.m_iServerClass
			>> kInfo.m_wstrPublicIP
			>> kInfo.m_usMasterPort
			>> kInfo.m_usNCUDPPort
			);
#else
		FETCH_DATA( kInfo.m_iUID
			>> kInfo.m_wstrName
			>> kInfo.m_iServerGroupID
			>> kInfo.m_iServerClass
			>> kInfo.m_wstrIP
			>> kInfo.m_usMasterPort
			>> kInfo.m_usNCUDPPort
			);
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
		//}}		
		
		kPacket.m_vecCenterServerList.push_back( kInfo );
	}

end_proc:
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, DBE_GET_CENTER_SERVER_LIST_ACK, kPacket );
	KUdpRelayChecker::GetKObj()->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( DBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT )
{
	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout, L"UDP Relay Server 성능 체크 통계 DB업데이트!" )
		<< BUILD_LOG( kPacket_.m_vecUdpRelayCheckLog.size() );
	//////////////////////////////////////////////////////////////////////////

    BOOST_TEST_FOREACH( const KUdpRelayCheckLog&, kInfo, kPacket_.m_vecUdpRelayCheckLog )
	{
		int iOK = NetError::ERR_ODBC_01;

		// UDP Relay 퍼포먼스 기록
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LUDPRelayCheck_INS", L"%d, %d, N\'%s\', %d, %d, N\'%s\'", 
			% KBaseServer::GetKObj()->GetServerGroupID()
			% kInfo.m_iServerUID
			% kInfo.m_wstrServerIP
			% kInfo.m_dwAvgDelayTime
			% kInfo.m_dwMaxDelayTime
			% kInfo.m_wstrRegDate
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LUDPRelayCheck_INT", L"%d, N\'%s\', %d, %d, N\'%s\'", 
			% kInfo.m_iServerUID
			% kInfo.m_wstrServerIP
			% kInfo.m_dwAvgDelayTime
			% kInfo.m_dwMaxDelayTime
			% kInfo.m_wstrRegDate
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
			START_LOG( cerr, L"UDP Relay Server퍼포먼스 정보 기록 실패!" )
				<< BUILD_LOG( kInfo.m_iServerUID )
				<< BUILD_LOG( kInfo.m_wstrServerIP )
				<< BUILD_LOG( kInfo.m_dwAvgDelayTime )
				<< BUILD_LOG( kInfo.m_dwMaxDelayTime )
				<< BUILD_LOG( kInfo.m_wstrRegDate )
				<< END_LOG;
		}
	}
}
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
IMPL_ON_FUNC_NOPARAM( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_REQ )
{
	KDBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK kPacket;

	// DB로부터 서버 리스트 데이터를 받아온다
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_SEL_ByServerSetID", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_channel_list", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KChannelInfo kInfo;

		FETCH_DATA( kInfo.m_iServerUID
			>> kInfo.m_iChannelID
			>> kInfo.m_wstrChannelName
			>> kInfo.m_iServerGroupID
			>> kInfo.m_wstrIP
			>> kInfo.m_usMasterPort
			>> kInfo.m_usNCUDPPort
			>> kInfo.m_iMaxUser
			>> kInfo.m_iCurrentUser
			>> kInfo.m_iCurPartyCount
			>> kInfo.m_iPlayGamePartyCount
			);

		kPacket.m_iTotalCCU += kInfo.m_iCurrentUser;
	}

end_proc:
	SendToServer( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK, kPacket );
}
#endif SERV_WAIT_QUEUE_SYSTEM
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

#ifdef SERVER_GROUP_UI_ADVANCED
IMPL_ON_FUNC_NOPARAM( DBE_SERVERGROUP_LIST_REQ )
{
	KDBE_SERVERGROUP_LIST_ACK kPacket;

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_LServerSetList_SEL_All" );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LServerList_By_ServerSetID_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_LServerChannelList" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		KServerGroupInfo kInfo;

		FETCH_DATA( kInfo.m_iOrder
			>> kInfo.m_iServerGroupUID
			>> kInfo.m_wstrServerGroupName
			>> kInfo.m_kServerIP
			>> kInfo.m_usMasterPort
			>> kInfo.m_usNCUDPPort
			>> kInfo.m_bEnable
			);

		kPacket.m_mapServerGroupList.insert( std::make_pair( kInfo.m_iServerGroupUID, kInfo ) );
	}

end_proc:
	SendToServer( DBE_SERVERGROUP_LIST_ACK, kPacket );
}
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_LOGIN_RESULT_INFO
IMPL_ON_FUNC( DBE_INSERT_LOGIN_RESULT_INFO_REQ )
{
	int iOK = NetError::ERR_ODBC_01;

	for(int i = 0; i < KLoginResultInfo::LRTE_SENTINEL; i++)
	{
		DO_QUERY( L"exec dbo.P_StatsLoginLOG_INS", L"%d, %d, %d, %d", 
			% kPacket_.m_iServerGroupID
			% kPacket_.m_uiServerUID
			% i
			% kPacket_.m_vecResultCount[i]
			);

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"로그인 결과 정보 등록 실패" )
			<< BUILD_LOG( kPacket_.m_iServerGroupID )
			<< BUILD_LOG( kPacket_.m_uiServerUID )
			<< BUILD_LOG( i )
			<< BUILD_LOG( kPacket_.m_vecResultCount[i] )
			<< END_LOG;
	}
}
#endif SERV_LOGIN_RESULT_INFO

#ifdef SERV_ID_NETMARBLE_PCBANG
_IMPL_ON_FUNC( DBE_PCBANG_IP_AND_MAC_INFO_NOT, KECH_PCBANG_IP_AND_MAC_INFO_NOT )
{


	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LMacAddressLog_INT", L"N\'%s\', N\'%s\'", 
		% kPacket_.m_wstrPublicIP
		% kPacket_.m_wstrMacInfo
		);
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"IP 와 Mac 정보 로그 기록 실패!" )
			<< BUILD_LOG( kPacket_.m_wstrPublicIP)
			<< BUILD_LOG( kPacket_.m_wstrMacInfo )
			<< END_LOG;
	}

}
#endif //SERV_ID_NETMARBLE_PCBANG
