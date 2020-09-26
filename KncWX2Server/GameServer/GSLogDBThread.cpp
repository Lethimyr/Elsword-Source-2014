#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSLogDBThread.h"
#include "GameServer.h"
#include "NetError.h"
#include "UserStatistics.h"
#include "X2Data/XSLRoom.h"
#include "x2data/XSLDungeon.h"

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/ReleaseTickTypeDefine.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	#include "UdpRelayChecker/UdpRelayChecker.h"
#endif SERV_UDP_RELAY_CHECKER
//}}

// [참고!] DO_QUERY() 에서 전달인자수 정확하게 넣지 않으면 서버 크래쉬 납니다!

//ImplementDBThread( KGSLogDBThread );
ImplPfID( KGSLogDBThread, PI_GS_LOG_DB );

#define CLASS_TYPE KGSLogDBThread

IMPL_PROFILER_DUMP( KGSLogDBThread )
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

KGSLogDBThread::~KGSLogDBThread(void)
{
}

void KGSLogDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
	   _CASE( DBE_SERVER_ON_REQ, KNetAddress );
        CASE( DBE_UPDATE_SERVER_INFO_REQ );
		CASE_NOPARAM( DBE_ADMIN_SERVER_STATUS_REQ );
		CASE_NOPARAM( DBE_SERVER_OFF_NOT );
		//{{ 2009. 2. 13  최육사	채널리스트
		CASE( DBE_CHANNEL_LIST_REQ );
		//}}
		CASE( DBE_CHARACTER_LEVELUP_NOT );
		CASE( ELOG_STAT_PLAY_TIME );
	   _CASE( ELOG_STAT_PVP_MAP, KStatisticsData );
	   _CASE( ELOG_STAT_FPS, KStatisticsData );
	   _CASE( ELOG_STAT_ED, KStatisticsData );
	   _CASE( ELOG_STAT_QUEST, KStatisticsData );
	   //{{ 2011. 02. 28	최육사	사내 통합 통계
#ifdef SERV_CHAR_CREATE_DEL_STAT
	   _CASE( ELOG_STAT_CHARACTER, KStatisticsData );
#endif SERV_CHAR_CREATE_DEL_STAT
	   //}}
		CASE( ELOG_USER_STATISTICS );
		CASE( ELOG_CHEAT_LOG_NOT );
	   _CASE( ELOG_STAT_DISCONNECT_USERFSM, KStatisticsData );
	   _CASE( ELOG_STAT_DISCONNECT_REASON, KStatisticsData );
	   _CASE( DBE_DELETE_UNIT_STAT_NOT, KEGS_DELETE_UNIT_REQ );
        CASE( DBE_UPDATE_UNIT_CLASS_NOT );		
		CASE( ELOG_RESURRECTION_STONE_LOG_NOT );
		CASE( ELOG_RESOLVE_JACKPOT_LOG_NOT );
        CASE( ELOG_GET_PURCHASED_CASH_ITEM_NOT );
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
		CASE_NOPARAM( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
		
		//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		CASE_NOPARAM( DBE_RELEASE_TICK_UPDATE_REQ );
		CASE_NOPARAM( DBE_EVENT_DATA_UPDATE_REQ );
		CASE_NOPARAM( DBE_EVENT_MONSTER_UPDATE_REQ );
		CASE_NOPARAM( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_REQ );
		CASE_NOPARAM( DBE_EVENT_REWARD_DATA_UPDATE_REQ );
		//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
		CASE_NOPARAM( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_REQ );
#endif SERV_CHAR_LEVEL_UP_EVENT
		//}}

		//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
		CASE_NOPARAM( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_REQ );
#endif SERV_NEXON_SESSION_PROTOCOL
		//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
		CASE( ELOG_ITEM_ABUSER_LOG_NOT );
		CASE( ELOG_ENCHANT_ABUSER_LOG_NOT );
		CASE( ELOG_ED_ABUSER_LOG_NOT );		
		//{{ 2008. 10. 22  최육사	캐쉬 분해 로그
		CASE( ELOG_CASH_ITEM_RESOLVE_LOG_NOT );
		//}}
		//{{ 2008. 11. 10  최육사
		CASE( ELOG_ENCHANT_ITEM_BREAK_LOG_NOT );
		//}}
		//{{ 2008. 10. 31  최육사	어뷰저 리스트
		CASE( DBE_ABUSER_LIST_REQ );
		//}}
		CASE( DBE_REG_ABUSER_AUTOMATIC_NOT );
		//{{ 2008. 12. 18  최육사	보조제 로그
		CASE( ELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT );
		//}}
		//{{ 2008. 12. 22  최육사	깨짐 복구 로그
		CASE( ELOG_RESTORE_ITEM_LOG_NOT );
		//}}
		//{{ 2008. 12. 22  최육사	부여 로그
		CASE( ELOG_ATTACH_ITEM_ENCHANT_LOG_NOT );
		//}}
		//{{ 2008. 12. 30  최육사	매력아이템 로그
		CASE( ELOG_CHARM_ITEM_LOG_NOT );
		//}}
		//{{ 2009. 4. 14  최육사	닉네임 변경
		CASE( ELOG_DELETE_NICK_NAME_NOT );
		CASE( ELOG_CHANGE_NICK_NAME_NOT );
		//}}

		CASE( ELOG_INIT_SKILL_TREE_NOT );
		CASE( ELOG_INSERT_CASH_SKILL_POINT_NOT );

		//{{ 2009. 7. 22  최육사	메가폰로그
		CASE( ELOG_MEGAPHONE_LOG_NOT );
		//}}

		//{{ 2009. 9. 1  최육사		비정상종료사유
		CASE( ELOG_ABNORMAL_DISCONNECT );
		//}}
		//{{ 2009. 9. 9  최육사		밀봉
		CASE( ELOG_USE_SEAL_ITEM_NOT );
		//}}
		//{{ 2009. 10. 22  최육사	트래픽 유발 유저 해킹유저로 자동등록
		CASE( ELOG_TRAFFIC_ATTACKER_LOG_NOT );
		//}}
		//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
		CASE( DBE_CHECK_HACKING_TOOL_LIST_REQ );
#endif SERV_HACKING_TOOL_LIST
		//}}
		//{{ 2010. 06. 30  최육사	근성도 통계
#ifdef SERV_SPIRIT_STATISTICS
		CASE( DBE_SPIRIT_STATISTICS_NOT );
#endif SERV_SPIRIT_STATISTICS
		//}}

#ifdef SERV_KOG_STATISTICS
		CASE_NOPARAM(DBE_CCU_AGENT_REQ);
#endif SERV_KOG_STATISTICS

		//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING
		CASE( ELOG_INSERT_TRAFFIC_ABUSER_LOG_NOT );
#endif SERV_TRAFFIC_USER_MONITORING
		//}}
		//{{ 2011. 01. 05	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM
		CASE( ELOG_ATTRIB_ATTACH_ITEM_LOG_NOT );
#endif SERV_ATTRIBUTE_CHARM
		//}}
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		CASE( ELOG_USER_CHARACTER_LOG_NOT );
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 02. 09	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG
		CASE( ELOG_BUY_CASH_ITEM_LOG_NOT );
#endif SERV_BUY_CASH_ITEM_LOG
		//}}
		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
		CASE( ELOG_CASH_PRODUCT_LIST_UPDATE_NOT );
#endif SERV_CASH_ITEM_LIST
		//}}
		//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
		CASE( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ );
#endif SERV_MONITORING_LETTER_AND_TRADE
		//}}
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	   _CASE( ELOG_STAT_ITEM_DB, KStatisticsData );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
		_CASE( ELOG_STAT_SYSTEM_INFO, KEGS_SYSTEM_INFO );
		_CASE( ELOG_STAT_KOG_GAME_PERFORMANCE_VILLAGE, KEGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG );
		_CASE( ELOG_STAT_KOG_GAME_PERFORMANCE_FIELD, KEGS_KOG_GAME_PERFORMANCE_FIELD_LOG );
		_CASE( ELOG_STAT_KOG_GAME_PERFORMANCE_DUNGEON, KEGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG );
		_CASE( ELOG_STAT_KOG_GAME_PERFORMANCE_PVP, KEGS_KOG_GAME_PERFORMANCE_PVP_LOG );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

	   //{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
	   CASE( DBE_FIRST_WRITE_SECURITY_TYPE_NOT );
#endif SERV_SECOND_SECURITY
	   //}}

#ifdef	SERV_SHARING_BANK_TEST
	   _CASE( ELOG_UPDATE_SHARE_ITEM_NOT, std::vector< KTradeShareItemInfo > );
	    CASE( ELOG_ITEM_TRADE_ERROR_NOT );
#endif	SERV_SHARING_BANK_TEST
#ifdef SERV_GOLD_TICKET
		CASE( ELOG_CHARGE_POINT_NOT );
#endif //SERV_GOLD_TICKET

	   //{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		 CASE( DBE_FIRST_WRITE_COME_BACK_INFO_NOT );
#endif SERV_COME_BACK_USER_REWARD
	   //}} 
		 //{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
		 CASE( DBE_INSERT_LOG_RETAINING_USER_NOT );
#endif SERV_NEXON_SESSION_PROTOCOL
		 //}
		//{{  2011.11.08     김민성    강화 로그
#ifdef SERV_ENCHANT_ITEM_LOG
		 CASE( ELOG_HIGH_ENCHANT_ITEM_LOG_NOT );
#endif SERV_ENCHANT_ITEM_LOG
		//}}
		 //{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
		 CASE( DBE_UNIT_CONNECT_LOG_NOT );
#endif SERV_CHAR_CONNECT_LOG
		 //}}
		 //{{ 2011. 12. 02  김민성	AP 구매 로그 수치
#ifdef SERV_BUY_AP_ITEM_LOG
		 CASE( DBE_BUY_AP_ITEM_LOG_NOT );
#endif SERV_BUY_AP_ITEM_LOG
		 //}}
		 //{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
		 CASE( DBE_UPDATE_PAKCET_MORNITORING_LOG_NOT );
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
		 //}}
		 //{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
		 CASE( DBE_CHECK_MODULE_INFO_LOG_NOT );
#endif SERV_HACKING_DLL_CHECK
		 //}}
		 //{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
		 CASE_NOPARAM( DBE_GET_CENTER_SERVER_LIST_REQ );
		 CASE( DBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT );
#endif SERV_UDP_RELAY_CHECKER
		 //}}
		 //{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
		 CASE( DBE_MODULE_EXIST_LOG_NOT );
#endif SERV_CHECK_X2_EXE_MODULE
		 //}}
		 //{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		 CASE_NOPARAM( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_REQ );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		 //}}
		 //{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		 _CASE( DBE_BAD_ATTITUDE_USER_LOG_NOT, KERM_BAD_ATTITUDE_USER_LOG_NOT );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
		 //}
		 //{{ 2013. 02. 13	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		 CASE( DBE_TICK_PERFORMANCE_LOG_NOT );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
		 //}}
		 //{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
		 CASE( DBE_ITEM_EXCHANGE_LOG_NOT );
#endif SERV_EXCHANGE_LOG
		 //}

		 //{{ 허상형 : [2010/8/31/] //	부활석 통계 분리 작업
#ifdef SERV_SELECT_UNIT_NEW
		 CASE( ELOG_UPDATE_STONE_NOT );
#endif	//	SERV_SELECT_UNIT_NEW
		 //}} 허상형 : [2010/8/31/] //	부활석 통계 분리 작업

		 //{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		 CASE( DBE_SYNTHESIS_LOG_NOT );
#endif SERV_SYNTHESIS_AVATAR
		 //}}

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
		 CASE_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

#ifdef SERV_CATCH_HACKUSER_INFO
		 _CASE( DBE_CATCH_HACKUSER_INTO_NOT, KEGS_CATCH_HACKUSER_INFO_NOT );
#endif SERV_CATCH_HACKUSER_INFO

		 //{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		 CASE( DBE_LOG_STATISTICS_INFO_NOT );
#endif SERV_LOG_SYSTEM_NEW
		 //}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
		CASE( DBE_GET_ITEM_ONOFF_NPCSHOP_REQ );
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
		CASE( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

#ifdef SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
		 CASE( DBE_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT );
#endif //SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
#ifdef SERV_SKILL_POINT_CORRECTION
		CASE( DBE_SKILL_POINT_CORRECTION_INFO_NOT );
#endif //SERV_SKILL_POINT_CORRECTION		 
		 //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		 CASE( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
		 //}

#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		 CASE( DBE_LOCAL_RANKING_SYSTEM_STATISTICS_LOG_NOT );
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG// 작업날짜: 2013-05-15	// 박세훈
		 CASE( DBE_FIELD_WORKINGS_BLOCK_LOG_NOT );
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
		 CASE( ELOG_KOM_FILE_CHECK_LOG_NOT );
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#ifdef SERV_GOOD_ELSWORD
         CASE( ELOG_EXPAND_BANK_INVENTORY_NOT );
#endif // SERV_GOOD_ELSWORD

#ifdef SERV_SKILL_PAGE_SYSTEM
		 CASE( ELOG_EXPAND_SKILL_PAGE_NOT );
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
		 CASE( DBE_CHANGE_PET_ID_LOG_NOT );
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
		_CASE( DBE_PAYLETTER_BILLING_ERROR_BUY_PACKET_NOT, KEBILL_BUY_PRODUCT_REQ );
		_CASE( DBE_PAYLETTER_BILLING_ERROR_GIFT_PACKET_NOT, KEBILL_GIFT_ITEM_REQ );
		_CASE( DBE_PAYLETTER_BILLING_ERROR_COUPON_PACKET_NOT, KEBILL_USE_COUPON_REQ );
#endif //SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		CASE( DBE_UNLIMITED_SECOND_CHANGE_JOB_NOT );
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KGSLogDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSLogDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

void KGSLogDBThread::UserStatisticsTableParsing( std::map< std::pair< int, int >, std::vector< int > >& mapStatistics, 
								int iTableID, int iSubTableID, int iMaxColumn, int* arrayData )
{
	// 반드시 초기화!
	ZeroMemory( arrayData, iMaxColumn );

	std::map< std::pair< int, int >, std::vector< int > >::iterator mit;
	mit = mapStatistics.find( std::make_pair( iTableID, iSubTableID ) );
	if( mit == mapStatistics.end() )	
		return;

	std::vector< int >::iterator vit = mit->second.begin();
	for( int iIndex = 0; iIndex < iMaxColumn; iIndex++ )
	{
		if( vit == mit->second.end() )
		{
			arrayData[iIndex] = 0;
		}
		else
		{
			arrayData[iIndex] = *vit;
			vit++;
		}
	}
}

void KGSLogDBThread::WriteServerInfoToDB( IN KDBE_UPDATE_SERVER_INFO_REQ& kPacket_, OUT KServerList& kPacket )
{
    // 동접을 DB에 적는다.
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_UPD_CCU", L"%d, N\'%s\', %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_wstrRegDate.c_str()
		% kPacket_.m_iServerUID
		% kPacket_.m_iConcurrentUser
		% kPacket_.m_iCurPartyCount
		% kPacket_.m_iPlayGamePartyCount
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_CCU_UPD", L"%d, N\'%s\', %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_wstrRegDate.c_str()
		% kPacket_.m_iServerUID
		% kPacket_.m_iConcurrentUser
		% kPacket_.m_iCurPartyCount
		% kPacket_.m_iPlayGamePartyCount
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_update_concurrent_user", L"N\'%s\', %d, %d, %d, %d",
		% kPacket_.m_wstrRegDate.c_str()
		% kPacket_.m_iServerUID
		% kPacket_.m_iConcurrentUser
		% kPacket_.m_iCurPartyCount
		% kPacket_.m_iPlayGamePartyCount
		);
#endif //SERV_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();
    }

    // DB로부터 서버 리스트 데이터를 받아온다
	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_LServerList_GET_ALL" );
#endif //SERV_RENEWAL_SP
#else
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.lup_get_server_list" );
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}

    while( m_kODBC.Fetch() )
    {
        KServerInfo kServerInfo;

		//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
		FETCH_DATA( kServerInfo.m_iUID
			>> kServerInfo.m_wstrName
			>> kServerInfo.m_iServerGroupID
			>> kServerInfo.m_iServerClass
			>> kServerInfo.m_wstrPublicIP
			>> kServerInfo.m_wstrPrivateIP
			>> kServerInfo.m_usMasterPort
			>> kServerInfo.m_usNCUDPPort
			>> kServerInfo.m_nMaxUser
			>> kServerInfo.m_nCurrentUser
			>> kServerInfo.m_bOn );
#else
		FETCH_DATA( kServerInfo.m_iUID
			>> kServerInfo.m_wstrName
			>> kServerInfo.m_iServerGroupID
			>> kServerInfo.m_iServerClass
			>> kServerInfo.m_wstrIP
			>> kServerInfo.m_usMasterPort
			>> kServerInfo.m_usNCUDPPort
			>> kServerInfo.m_nMaxUser
			>> kServerInfo.m_nCurrentUser
			>> kServerInfo.m_bOn );
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
		//}}		

        std::pair< KServerList::iterator, bool > pairIB;
        std::vector< KServerInfo > vecServerList;
        pairIB = kPacket.insert( std::make_pair( kServerInfo.m_iServerGroupID, vecServerList ) );
        pairIB.first->second.push_back( kServerInfo );
    }

end_proc:
    return;
}

IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_REQ )
{
    KServerList kPacket;
    WriteServerInfoToDB( kPacket_, kPacket );

    SendToServer( DBE_UPDATE_SERVER_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( ELOG_STAT_PLAY_TIME )
{
    int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	int iChannelCode = 0;
	bool bPCBangFlag = kPacket_.m_bIsPcbang;	//m_bIsPcbang 해외도 필요해서 오과금 define 밖으로 뺌

#ifdef SERV_CONNECT_LOG_CHANNELING
	iChannelCode = static_cast< int >(kPacket_.m_ucChannelCode);
#endif //SERV_CONNECT_LOG_CHANNELING
	// SERV_FIELD_PLAY_TIME_LOG define 내부 포함임

#ifdef SERV_STEAM
	DO_QUERY( L"exec dbo.P_LConnectLog_INS", L"%d, %d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% iChannelCode
		% kPacket_.m_bSteamClient
		% bPCBangFlag
		);
#else //SERV_STEAM
	DO_QUERY( L"exec dbo.P_LConnectLog_INS", L"%d, %d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% iChannelCode
		% bPCBangFlag
		);
#endif //SERV_STEAM
#else //SERV_RENEWAL_SP

	//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
	DO_QUERY( L"exec dbo.lup_insert_user_play_time", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		);
#else
    DO_QUERY( L"exec dbo.lup_insert_user_play_time", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d",
        % kPacket_.m_iUserUID
        % kPacket_.m_wstrIP
        % kPacket_.m_wstrLoginTime
        % kPacket_.m_wstrLogoutTime
        % kPacket_.m_bLoginFail
        % kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		);
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}
#endif //SERV_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();
    }

end_proc:
    LOG_SUCCESS( iOK == NetError::NET_OK )
        << BUILD_LOG( iOK )
        << END_LOG;
}

//{{ 2007. 10. 25  최육사  대전 맵 통계
_IMPL_ON_FUNC( ELOG_STAT_PVP_MAP, KStatisticsData )
{
	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); mit++ )
	{
		if( mit->first.m_vecIntKey.empty() )
		{
			START_LOG( cerr, L"유닛 ID가 비었음." )
				<< END_LOG;

			continue;
		}
		int iWorldID = mit->first.m_vecIntKey[0];		
		__int64 aiCount[KStatistics::eSIColPvpMap_Max];
		std::vector< __int64 >::iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::eSIColPvpMap_Max; iIndex++ )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )
					<< BUILD_LOG( iWorldID )
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsVsMap_MER", L"%d, %d, %d",
					% KBaseServer::GetKObj()->GetServerGroupID()
					% iWorldID
					% aiCount[KStatistics::eSIColPvpMap_PlayCount]
					);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsVsMap_SET", L"%d, %d, %d",
					% KBaseServer::GetKObj()->GetServerGroupID()
					% iWorldID
					% aiCount[KStatistics::eSIColPvpMap_PlayCount]
					);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsVsMapUpdate", L"%d, %d",
					% iWorldID
					% aiCount[KStatistics::eSIColPvpMap_PlayCount]
					);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		START_LOG( clog2, L"대전 맵 통계 [검사]" )
			<< BUILD_LOG( iWorldID )
			<< BUILD_LOG( aiCount[KStatistics::eSIColPvpMap_PlayCount] )
			<< BUILD_LOG( iOK );
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;
}
//}}

//{{ 2007. 11. 3  최육사  FPS 통계
_IMPL_ON_FUNC( ELOG_STAT_FPS, KStatisticsData )
{
	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); mit++ )
	{
		if( mit->first.m_vecIntKey.empty() )
		{
			START_LOG( cerr, L"통계키값이 비었음" )
				<< END_LOG;

			continue;
		}
		int iGameMode = mit->first.m_vecIntKey[0];
		__int64 aiCount[KStatistics::eSIColFps_Max];
		std::vector< __int64 >::iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::eSIColFps_Max; iIndex++ )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )
					<< BUILD_LOG( iGameMode )
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}
		
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_FrameRate_MER", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iGameMode										//  @iPlayType  [tinyint] - 던전(훈련소 포함), 대전, 광장 등
			% aiCount[KStatistics::eSIColFps_0_20]			//	@iSUnder20  [int]     - 20 이하 카운트
			% aiCount[KStatistics::eSIColFps_21_30]			//	@iS21_30    [int]     - 21~30 카운트
			% aiCount[KStatistics::eSIColFps_31_40]			//	@iS31_40    [int]  
			% aiCount[KStatistics::eSIColFps_41_50]			//	@iS41_50    [int]  
			% aiCount[KStatistics::eSIColFps_51_60]			//	@iS51_60    [int]  
			% aiCount[KStatistics::eSIColFps_61_70]			//	@iS61_70    [int]  
			% aiCount[KStatistics::eSIColFps_71_80]			//	@iS71_80    [int]  
			% aiCount[KStatistics::eSIColFps_81_90]			//	@iS81_90    [int]  
			% aiCount[KStatistics::eSIColFps_91_unlimited]	//	@iSOver90   [int]     - 90 이상 카운트
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_FrameRate_SET", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iGameMode										//  @iPlayType  [tinyint] - 던전(훈련소 포함), 대전, 광장 등
			% aiCount[KStatistics::eSIColFps_0_20]			//	@iSUnder20  [int]     - 20 이하 카운트
			% aiCount[KStatistics::eSIColFps_21_30]			//	@iS21_30    [int]     - 21~30 카운트
			% aiCount[KStatistics::eSIColFps_31_40]			//	@iS31_40    [int]  
			% aiCount[KStatistics::eSIColFps_41_50]			//	@iS41_50    [int]  
			% aiCount[KStatistics::eSIColFps_51_60]			//	@iS51_60    [int]  
			% aiCount[KStatistics::eSIColFps_61_70]			//	@iS61_70    [int]  
			% aiCount[KStatistics::eSIColFps_71_80]			//	@iS71_80    [int]  
			% aiCount[KStatistics::eSIColFps_81_90]			//	@iS81_90    [int]  
			% aiCount[KStatistics::eSIColFps_91_unlimited]	//	@iSOver90   [int]     - 90 이상 카운트
			);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsFrameRate", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% iGameMode										//  @iPlayType  [tinyint] - 던전(훈련소 포함), 대전, 광장 등
			% aiCount[KStatistics::eSIColFps_0_20]			//	@iSUnder20  [int]     - 20 이하 카운트
			% aiCount[KStatistics::eSIColFps_21_30]			//	@iS21_30    [int]     - 21~30 카운트
			% aiCount[KStatistics::eSIColFps_31_40]			//	@iS31_40    [int]  
			% aiCount[KStatistics::eSIColFps_41_50]			//	@iS41_50    [int]  
			% aiCount[KStatistics::eSIColFps_51_60]			//	@iS51_60    [int]  
			% aiCount[KStatistics::eSIColFps_61_70]			//	@iS61_70    [int]  
			% aiCount[KStatistics::eSIColFps_71_80]			//	@iS71_80    [int]  
			% aiCount[KStatistics::eSIColFps_81_90]			//	@iS81_90    [int]  
			% aiCount[KStatistics::eSIColFps_91_unlimited]	//	@iSOver90   [int]     - 90 이상 카운트
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		START_LOG( clog2, L"FPS 통계 [검사]" )
			<< BUILD_LOG( iGameMode )			
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_0_20] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_21_30] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_31_40] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_41_50] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_51_60] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_61_70] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_71_80] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_81_90] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColFps_91_unlimited] )
			<< BUILD_LOG( iOK );
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;
}
//}}

_IMPL_ON_FUNC( DBE_SERVER_ON_REQ, KNetAddress )
{
    KDBE_GAME_SERVER_ON_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL_VerifyPublicIP", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_GAME 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_By_PublicIP_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% KBaseServer::ESC_GAME 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_verify_PublicIP", L"%d, N\'%s\'", 
		% KBaseServer::ESC_GAME 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#else //SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_GAME 
		% kPacket_.m_wstrIP 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_Verify_On_SEL", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% KBaseServer::ESC_GAME 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_verify_server_on", L"%d, N\'%s\'", 
		% KBaseServer::ESC_GAME 
		% kPacket_.m_wstrIP 
		);
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}

    if( m_kODBC.BeginFetch() )
    {
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
			>> kPacket.m_kServerInfo.m_bOn
			>> kPacket.m_kServerInfo.m_cServerRollType );
#else
		//{{ 2011. 02. 23	최육사	서버별 역할 타입
		//#ifdef SERV_SERVER_ROLL_TYPE
		FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
			>> kPacket.m_kServerInfo.m_wstrName
			>> kPacket.m_kServerInfo.m_iServerGroupID
			>> kPacket.m_kServerInfo.m_iServerClass
			>> kPacket.m_kServerInfo.m_wstrIP
			>> kPacket.m_kServerInfo.m_usMasterPort
			>> kPacket.m_kServerInfo.m_usNCUDPPort
			>> kPacket.m_kServerInfo.m_nMaxUser
			>> kPacket.m_kServerInfo.m_nCurrentUser
			>> kPacket.m_kServerInfo.m_bOn
			>> kPacket.m_kServerInfo.m_cServerRollType );
		//#else
		//		FETCH_DATA( kPacket.m_kServerInfo.m_iUID 
		//			>> kPacket.m_kServerInfo.m_wstrName
		//			>> kPacket.m_kServerInfo.m_iServerGroupID
		//			>> kPacket.m_kServerInfo.m_iServerClass
		//			>> kPacket.m_kServerInfo.m_wstrIP
		//			>> kPacket.m_kServerInfo.m_usMasterPort
		//			>> kPacket.m_kServerInfo.m_usNCUDPPort
		//			>> kPacket.m_kServerInfo.m_nMaxUser
		//			>> kPacket.m_kServerInfo.m_nCurrentUser
		//			>> kPacket.m_kServerInfo.m_bOn );
		//#endif SERV_SERVER_ROLL_TYPE
		//}}
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
		//}}		

        m_kODBC.EndFetch();
    }
    else
    {
        START_LOG( cerr, L"게임 서버 인증 실패." )
            << BUILD_LOG( kPacket_.m_wstrIP )
			<< END_LOG;

        kPacket.m_iOK = NetError::ERR_VERIFY_02;
        goto end_proc;
    }

	//////////////////////////////////////////////////////////////////////////	
	// 센터들의 주소를 알아온다.
	kPacket.m_mapCenterList.clear();

	//{{ 2013. 01. 22	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCenterProxy_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCenterProxy_By_ServerSetID_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_center_proxyinfo", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_RENEWAL_SP
#else
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCenterProxy_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_center_proxy_info", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
	
	while( m_kODBC.Fetch() )
	{
		int iProxyID;
		KNetAddress kAddres;
		FETCH_DATA( iProxyID
			>> kAddres.m_wstrIP
			>> kAddres.m_usPort );

		kPacket.m_mapCenterList.insert( std::make_pair( iProxyID, kAddres ) );
	}

	//////////////////////////////////////////////////////////////////////////	
	// 로긴서버들의 주소를 알아온다.
	kPacket.m_mapLoginList.clear();

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	//{{ 20100114. jseop. PC방 리스트
	kPacket.m_mapPCBangIPList.clear();

	DO_QUERY( L"exec dbo.lup_get_LNetCafeList" /*, L"%d ", % kPacket.m_kServerInfo.m_iServerGroupID */ );
	while( m_kODBC.Fetch() )
	{
		std::wstring wstrIP;
		int		iPCBangType;

		FETCH_DATA( wstrIP 
			>>iPCBangType );

		//kPacket.m_setNetCafeList.insert( wstrIP );
		kPacket.m_mapPCBangIPList.insert(std::make_pair(wstrIP, iPCBangType) );

		START_LOG( clog2, L"PC방 IP" )
			<< BUILD_LOG( wstrIP )
			<< BUILD_LOG( iPCBangType );
	}
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST

	//{{ 2013. 01. 22	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LLoginProxy_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LLoginProxy_By_ServerSetID_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_login_proxyinfo", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_RENEWAL_SP
#else
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LLoginProxy_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_login_proxy_info", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
	
	while( m_kODBC.Fetch() )
	{
		int iProxyID;
		KNetAddress kAddres;
		FETCH_DATA( iProxyID
			>> kAddres.m_wstrIP
			>> kAddres.m_usPort );

		kPacket.m_mapLoginList.insert( std::make_pair( iProxyID, kAddres ) );
	}

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
            kPacket.m_iOK = NetError::ERR_VERIFY_08;
			goto end_proc;
        }
    }

	//{{ 2011. 07. 22	최육사	모든 쿼리 SP사용
#ifdef SERV_ALL_DB_QUERY_USE_SP
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_SEL_ByServerUID", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID()
																	 % kPacket.m_kServerInfo.m_iUID 	);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelList_By_ServerUID_SEL", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID()
																	 % kPacket.m_kServerInfo.m_iUID 	);
#endif //SERV_ALL_RENEWAL_SP

#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_channelid_by_serveruid", L"%d", % kPacket.m_kServerInfo.m_iUID );
#endif //SERV_RENEWAL_SP
#else
	//DO_QUERY_NO_PROFILE( L"SELECT ChannelID FROM dbo.LChannelList(NOLOCK)", L"WHERE UID = %d", % kPacket.m_kServerInfo.m_iUID );
#endif SERV_ALL_DB_QUERY_USE_SP
	//}}

	if( m_kODBC.BeginFetch() )
	{
		kPacket.m_iChannelID = 0;
		
		FETCH_DATA( kPacket.m_iChannelID );
		m_kODBC.EndFetch();

		if( kPacket.m_iChannelID <= 0 )
		{
			START_LOG( cerr, L"해당 게임서버의 채널id가 이상합니다" )
				<< BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
				<< BUILD_LOG( kPacket.m_iChannelID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_VERIFY_15;
			goto end_proc;
		}
	}
	else
	{
		START_LOG( cerr, L"게임서버의 채널정보가 없다? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_VERIFY_15;
		goto end_proc;
	}

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LChannelReward_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID() 
															% kPacket.m_iChannelID );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_channel_reward", L"%d", % kPacket.m_iChannelID );
#endif //SERV_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		int iChannelID = 0;
		FETCH_DATA( iChannelID
			>> kPacket.m_kBonusInfo.m_iBeginLv
			>> kPacket.m_kBonusInfo.m_iEndLv
			>> kPacket.m_kBonusInfo.m_iPerExp
			>> kPacket.m_kBonusInfo.m_iPerED );
		m_kODBC.EndFetch();

		if( iChannelID != kPacket.m_iChannelID )
		{
			START_LOG( cerr, L"채널 보너스 정보가 이상합니다" )
				<< BUILD_LOG( kPacket.m_kServerInfo.m_iUID )
				<< BUILD_LOG( iChannelID )
				<< BUILD_LOG( kPacket.m_iChannelID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_VERIFY_15;
			goto end_proc;
		}

		if( kPacket.m_kBonusInfo.m_iPerExp > 0  ||  kPacket.m_kBonusInfo.m_iPerED > 0 )
		{
			// 보너스값이 있으면 enable
			kPacket.m_kBonusInfo.m_bEnable = true;
		}
	}
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	kPacket.m_mapGlobalList.clear();

	//{{ 2013. 01. 22	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LGlobalProxy_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LGlobalProxy_By_ServerSetID_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_GET_global_proxyinfo", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_RENEWAL_SP
#else
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LGlobalProxy_SEL", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_global_proxy_info", L"%d", % kPacket.m_kServerInfo.m_iServerGroupID );
#endif //SERV_RENEWAL_SP
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
	
	while( m_kODBC.Fetch() )
	{
		int iProxyID;
		KNetAddress kAddres;
		FETCH_DATA( iProxyID
			>> kAddres.m_wstrIP
			>> kAddres.m_usPort );

		kPacket.m_mapGlobalList.insert( std::make_pair( iProxyID, kAddres ) );
	}

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
			kPacket.m_iOK = NetError::ERR_VERIFY_08;
			goto end_proc;
		}
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

end_proc:
    //결과 보내기
    SendToServer( DBE_SERVER_ON_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( DBE_ADMIN_SERVER_STATUS_REQ )
{
    KEGS_ADMIN_SERVER_STATUS_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_SEL_TotalCCU", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LServerList_CCU_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY_NO_ARG( L"exec dbo.lup_get_server_concurrent_user" );
#endif //SERV_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        kPacket.m_iOK = NetError::NET_OK;
        FETCH_DATA( kPacket.m_iCCU );
        m_kODBC.EndFetch();
    }

#ifdef SERV_CHANNELING_USER_MANAGER
	int iChannelingCode = 0;
	int iChannelingCurrentUser = 0;

	for ( int iChannelingCode = 0; iChannelingCode < KNexonAccountInfo::JCE_MAX; iChannelingCode++ )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatCCUMinuteChannel_SEL", L"%d", % iChannelingCode  );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_get_StatCCUMinuteChannel_NHN", L"%d", % iChannelingCode  );
#endif //SERV_ALL_RENEWAL_SP	

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iChannelingCurrentUser );

			m_kODBC.EndFetch();
		}

		kPacket.m_mapChannelingUserList.insert( std::make_pair( iChannelingCode, iChannelingCurrentUser ) );
	}
#endif SERV_CHANNELING_USER_MANAGER

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_ADMIN_SERVER_STATUS_ACK, kPacket );
}

//{{ 2009. 2. 13  최육사	채널리스트
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
    DO_QUERY( L"exec dbo.lup_set_server_enable_flag", L"%d, 0", % GetKGameServer()->GetUID() );
#endif //SERV_RENEWAL_SP

    int iOK;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

        if( iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"서버 켜짐 표시 실패." )
                << BUILD_LOG( GetKGameServer()->GetUID() )
                << END_LOG;

            iOK = NetError::ERR_VERIFY_08;
        }
    }

end_proc:
    return;
}

IMPL_ON_FUNC( DBE_CHARACTER_LEVELUP_NOT )
{
    //서버 Enable을 끈다
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCharLvUp_INS", L"%d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_nCharUID
		% (int)kPacket_.m_ucLevelBefore
		% (int)kPacket_.m_ucLevelAfter );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCharLvUp_SET", L"%d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_nCharUID
		% (int)kPacket_.m_ucLevelBefore
		% (int)kPacket_.m_ucLevelAfter );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_insert_lvup", L"%d, %d, %d",
        % kPacket_.m_nCharUID
        % (int)kPacket_.m_ucLevelBefore
        % (int)kPacket_.m_ucLevelAfter );
#endif //SERV_RENEWAL_SP

end_proc:
    return;
}

// boost::format의 문자열 길이 제한에 따른 클래스 재정의
typedef KUserStatistics US;

IMPL_ON_FUNC( ELOG_USER_STATISTICS )
{
	int iOK = NetError::ERR_ODBC_01;
	int iOK_2 = NetError::ERR_ODBC_01;

	if( kPacket_.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"유닛UID가 이상합니다." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		return;
	}

#ifdef SERV_USER_STATISTICS_RENEWAL
	int etcData[KUserStatistics::US_Etc_Max] = {0};
	int edData[KUserStatistics::US_ED_Max] = {0};

	int pvpMcTeam[KUserStatistics::US_Pvp_MC_Max] = {0};
	int pvpMcTeamDeath[KUserStatistics::US_Pvp_MC_Max] = {0};
	int pvpMcDeath[KUserStatistics::US_Pvp_MC_Max] = {0};

	int pvpPtTeam[KUserStatistics::US_Pvp_PT_Max] = {0};
	int pvpPtTeamDeath[KUserStatistics::US_Pvp_PT_Max] = {0};
	int pvpPtDeath[KUserStatistics::US_Pvp_PT_Max] = {0};

	//std::map< int, std::vector< int > > map_dungeon; // 던전 통계는 이 변수로 값을 추출하자 //2011.10.18 lygan_조성욱 // 중국 전용 유저 통계값 추출
	int dungeon[KUserStatistics::US_D_Max] = {0};
	//map_dungeon.clear();
	
	int TrainingCenter[KUserStatistics::US_TC_Max] = {0};

	int iIsPcBang = 0;
	
	if( kPacket_.m_mapEtcStatistics.empty() == false )
	{
		UserStatisticsTableParsing( kPacket_.m_mapEtcStatistics, KUserStatistics::USTable_EtcData, 
			0, KUserStatistics::US_Etc_Max, etcData );

		iIsPcBang = ( etcData[KUserStatistics::US_Etc_IsPcBang] == 0 ? 0 : 1 );

	}

	if( kPacket_.m_mapEDStatistics.empty() == false )
	{

		UserStatisticsTableParsing( kPacket_.m_mapEDStatistics, KUserStatistics::USTable_EDData, 
			0, KUserStatistics::US_ED_Max, edData );


	}

	if( kPacket_.m_mapPvpStatistics.empty() == false )
	{
		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpModeCount, 
			CXSLRoom::PGT_TEAM, KUserStatistics::US_Pvp_MC_Max, pvpMcTeam );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpModeCount,
			CXSLRoom::PGT_TEAM_DEATH, KUserStatistics::US_Pvp_MC_Max, pvpMcTeamDeath );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpModeCount,
			CXSLRoom::PGT_SURVIVAL, KUserStatistics::US_Pvp_MC_Max, pvpMcDeath );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpPlayTime, 
			CXSLRoom::PGT_TEAM, KUserStatistics::US_Pvp_PT_Max, pvpPtTeam );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpPlayTime,
			CXSLRoom::PGT_TEAM_DEATH, KUserStatistics::US_Pvp_PT_Max, pvpPtTeamDeath );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpPlayTime,
			CXSLRoom::PGT_SURVIVAL, KUserStatistics::US_Pvp_PT_Max, pvpPtDeath );

	}


	if( kPacket_.m_mapDungeonStatistics.empty() == false )
	{
		int dungeonTemp[KUserStatistics::US_D_Max] = {0};

		UserStatisticsIterator mit = kPacket_.m_mapDungeonStatistics.begin();
		for( ; mit != kPacket_.m_mapDungeonStatistics.end(); mit++ )
		{
			
			if( mit->first.first != KUserStatistics::USTable_Dungeon )
			{
				START_LOG( cerr, L"던전통계테이블도 아닌것이 던전통계에 들어왔다?" )
					<< BUILD_LOG( mit->first.first )
					<< END_LOG;

				continue;
			}

			UserStatisticsTableParsing( kPacket_.m_mapDungeonStatistics, KUserStatistics::USTable_Dungeon,
				mit->first.second, KUserStatistics::US_D_Max, dungeonTemp );

			for(int i =0; i< KUserStatistics::US_D_Max; i++)
			{
				dungeon[i] += dungeonTemp[i];
			}

		}
	}

	// 훈련소 통계
	if( kPacket_.m_mapTCenterStatistics.empty() == false )
	{
		int TrainingCenterTemp[KUserStatistics::US_TC_Max] = {0};

		UserStatisticsIterator mit = kPacket_.m_mapTCenterStatistics.begin();
		for( ; mit != kPacket_.m_mapTCenterStatistics.end(); mit++ )
		{
			if( mit->first.first != KUserStatistics::USTable_TrainingCenter )
			{
				START_LOG( cerr, L"훈련소통계테이블도 아닌것이 훈련소통계에 들어왔다?" )
					<< BUILD_LOG( mit->first.first )
					<< END_LOG;

				continue;
			}

			UserStatisticsTableParsing( kPacket_.m_mapTCenterStatistics, KUserStatistics::USTable_TrainingCenter,
				mit->first.second, KUserStatistics::US_TC_Max, TrainingCenterTemp );

			for(int i =0; i< KUserStatistics::US_TC_Max; i++)
			{
				TrainingCenter[i] += TrainingCenterTemp[i];
			}

		}
	}

	int dungeonSiglePlayCount	= 0;
	int dungeonParyPlayCount	= 0;

	dungeonSiglePlayCount = dungeon[US::US_D_MC_Win] + dungeon[US::US_D_MC_Lose] + dungeon[US::US_D_MC_Drop];
	dungeonParyPlayCount = dungeon[US::US_D_MC_Party_Win] + dungeon[US::US_D_MC_Party_Lose] + dungeon[US::US_D_MC_Party_Drop];

	int dungeonSiglePlayPT = 0;
	int dungeonParyPlayPT = 0;

	dungeonSiglePlayPT = dungeon[US::US_D_PT_Win] + dungeon[US::US_D_PT_Lose] + dungeon[US::US_D_PT_Drop];
	dungeonParyPlayPT = dungeon[US::US_D_PT_Party_Win] + dungeon[US::US_D_PT_Party_Lose] + dungeon[US::US_D_PT_Party_Drop];

	int pvpWinCount = 0;
	int pvpLoseCount = 0;

	pvpWinCount = pvpMcTeam[US::US_Pvp_MC_Win] + pvpMcTeamDeath[US::US_Pvp_MC_Win] + pvpMcDeath[US::US_Pvp_MC_Win];
	pvpLoseCount = pvpMcTeam[US::US_Pvp_MC_Lose] + pvpMcTeamDeath[US::US_Pvp_MC_Lose] + pvpMcDeath[US::US_Pvp_MC_Lose];

	int trainingcenterPT = 0;
	int trainingcenterCount = 0;

	trainingcenterPT = TrainingCenter[US::US_TC_PT_Success] + TrainingCenter[US::US_TC_PT_Fail] + TrainingCenter[US::US_TC_PT_Drop];
	trainingcenterCount = TrainingCenter[US::US_TC_MC_Success] + TrainingCenter[US::US_TC_MC_Fail] + TrainingCenter[US::US_TC_MC_Drop];

	int reStoneCount = 0;

	reStoneCount = dungeon[US::US_D_MC_ReStone] + dungeon[US::US_D_MC_Party_ReStone];

	// 기존 SP명 : P_LUnitStatisticsTotal
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LUnitStatistics_MER", 
		L"%d, "
		//Connect 5개 
		L"%d, N\'%s\', N\'%s\', %d, %d,"											 //서버번호, 로그인일시, 로그아웃 일시, PC방 여부, 유저 권한레벨
		//User 3개 
		L"%d, %d, N\'%s\', "													//계정고유번호, 퍼블리셔SN, 계정생성일시
		//Unit 9개 
		L"%d, N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, "					//고유번호, 닉네임, 클래스, 경험치, 레벨, ED, 생성일시, 총 완료 퀘스트 수, 총 중복 완료 퀘스트수
		//ED 26개
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//던전 획득, 트레이닝 획득, 퀘스트 획득, 아이템 판매 획득, 아이템 거래 획득, 아이템 개인거래 획득, 우편 수령 획득, 아이템 판매, 거래지불, 아이템 수리비,
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//아이템 강화 수수료, 아이템 소켓 수수료, 아이템 속성 수수료, 아이템 제조 수수료, 던전 이탈 패널티, 개인거래지불, 개인상점 개설 수수료, 개인상점 구매 수수료, 개인상점 판매 수수료, 우편첨부,
		L"%d, %d, %d, %d, %d, %d,"												// 우편발송 수수료, 우편 아이템 첨부 수수료, 큐브 오픈 수수료, 필드 획득, 코보 익스프레스 수수료, 분해 수수료
		//Play 23개
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//사용 근성도, 근송도 모두 소모여부, 총플래이 시간, 총대기 시간, 총대전 시간, 던전 싱글 플래이 횟수, 던전 파티 플래이 횟수, 던전 싱글 클리어 횟수, 던전 파티 클리어 횟수, 던전 싱글 플래이 시간,
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//던전 파티 플래이 시간, 대전  Kill수 , 대전 승수, 대전 패수, 트레이닝 플래이 시간, 트레이닝 플래이 횟수, 개인 거래 횟수, 광장 출입 횟수, 광장 머문시간, 구매한 아이템수, 
		L"%d, %d, %d" ,															//판매한 아이템 수, 획득한 아이템 수, 총 사용한 부활석
		//Connect 5개 
		% KBaseServer::GetKObj()->GetServerGroupID()							//@iServerSetID TINYINT   
		% KBaseServer::GetKObj()->GetUID()										//@ServerUID					int					-- 서버 번호
		% kPacket_.m_wstrLoginTime												//@logintime					datetime			-- 로그인 일시
		% kPacket_.m_wstrLogoutTime												//@logouttime					datetime			-- 로그아웃 일시
		% iIsPcBang																//@PCBangFlag					bit					-- PC방 여부
		% kPacket_.m_iAuthLevel													//@AuthLevel					tinyint				-- 계정 권한
		//User 3개 
		% kPacket_.m_iUserUID													//@UserUID						bigint				-- 계정 고유번호
		% kPacket_.m_uiGiantUID													//@PublisherSN					int					-- 퍼블리셔 SN
		% kPacket_.m_wstrAccountCreateDate										//@UserRegDate					smalldatetime		-- 계정 생성일시
		//Unit 9개 
		% kPacket_.m_iUnitUID													//@UnitUID						bigint				-- 고유번호
		% kPacket_.m_wstrNickName												//@NickName						nvarchar(20)		-- 닉네임
		% ( int )kPacket_.m_cUnitClass											//@UnitClass					tinyint				-- 클래스
		% kPacket_.m_iEXP														//@Exp							int					-- 경험치
		% (int)kPacket_.m_ucLevel												//@Level						tinyint				-- 레벨
		% kPacket_.m_iED														//@GamePoint					int					-- ED
		% kPacket_.m_wstrUnitCreateDate											//@UnitRegDate					smalldatetime		-- 생성일시
		% etcData[US::US_Etc_NQuestComplete]									//@QuestCnt						int					-- 총 완료 퀘스트 수
		% etcData[US::US_Etc_RQuestComplete]									//@ReQuestCnt					int					-- 총 중복 완료 퀘스트 수
		//ED 23개
		% edData[US::US_ED_PDungeon]											//@PDungeon						int					-- 던전 획득
		% edData[US::US_ED_PTraining]											//PTraining						int					-- 트레이닝 획득
		% edData[US::US_ED_PQuest]												//@PQuest						int					-- 퀘스트 획득
		% edData[US::US_ED_PItemSell]											//@PItemSell					int					-- 아이템 판매 획득
		% edData[US::US_ED_PItemTrade]											//@PItemTrade					bigint				-- 아이템 거래 획득
		% edData[US::US_ED_PPersonalShop]										//@PPersonalShop				bigint				-- 아이템 개인 거래 획득
		% edData[US::US_ED_PPostLetter]											//@PPostLetter					bigint				-- 우편 수령 획득
		% edData[US::US_ED_MItemBuy]											//@MItemBuy						int					-- 아이템 판매
		% edData[US::US_ED_MItemTrade]											//@MItemTrade					bigint				-- 거래 지불
		% edData[US::US_ED_MItemRepair]											//@MItemRepair					int					-- 아이템 수리비
		% edData[US::US_ED_MEnchant]											//@MItemEnchant					bigint				-- 아이템 강화 수수료
		% edData[US::US_ED_MSocket]												//@MItemSocket					int					-- 아이템 소켓 수수료
		% edData[US::US_ED_MAttribEnchant]										//@MItemAttribute				int					-- 아이템 속성 수수료
		% edData[US::US_ED_MManufacture]										//@MManufacture					int					-- 아이템 제조 수수료
		% edData[US::US_ED_MDunPenalty]											//@MDunPenalty					int					-- 던전 이탈 패널티
		% edData[US::US_ED_MPersonalShop]										//@MPersonalShop				bigint				-- 개인 거래 지불
		% edData[US::US_ED_MOpenPShop]											//@MOpenPShop					bigint				-- 개인 상점 개설 수수료
		% edData[US::US_ED_MSellTaxPShop]										//@MSellTaxPShop				int					-- 개인 상점 구매 수수료
		% edData[US::US_ED_MBuyTaxPShop]										//@MBuyTaxPShop					int					-- 개인 상점 판매 수수료
		% edData[US::US_ED_MPostLetter]											//@MPostLetter					bigint				-- 우편 첨부
		% edData[US::US_ED_MSendLetterBasic]									//@MSendLetterBasic				int					-- 우편 발송 수수료
		% edData[US::US_ED_MSendLetterItem]										//@MSendLetterItem				int					-- 우편 아이템 첨부 수수료
		% etcData[US::US_ED_OpenCubeSpendED]									//@iOpenCube					int					-- 큐브 오픈 수수료 // SERV_CUBE_OPEN_ED_CONDITION
		% etcData[US::US_ED_PBattleFieldED]									//@iPBattleFieldED				int					-- 필드 획득		// SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
		% etcData[US::US_ED_MCoboExpressTicketED]								//@iMCoboExpressTicketED		int					-- 코보 익스프레스 수수료 // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
		% etcData[US::US_ED_MItemResolve]										//@iMItemDecom					bigint				-- 분해 수수료		// SERV_MULTI_RESOLVE
		//Play 23개
		% etcData[US::US_Etc_Used_Spirit]										//@UseSpirit					int					-- 사용 근성도
		% etcData[US::US_Etc_AllUsed_Spirit_Count]								//@AllSpirit					bit					-- 근성도 모두 소모 여부
		% etcData[US::US_Etc_Dungeon_Play_Time]									//@TotalPlayTime				int					-- 총 플래이 시간(초)
		% kPacket_.m_iiDurationTime												//@DurationTime					int					-- 총 대기 시간(초)
		% etcData[US::US_Etc_PVP_Play_Time]										//@VsPlayTime					int					-- 총 대전 시간(초)
		% dungeonSiglePlayCount													//@DSoloCnt						int					-- 던전 싱글 플래이 횟수
		% dungeonParyPlayCount													//@DPartyCnt					int					-- 던전 파티 플래이 횟수
		% dungeon[US::US_D_MC_Win]												//@DSWCnt						int					-- 던전 싱글 클리이 횟수
		% dungeon[US::US_D_MC_Party_Win]										//@DPWCnt						int					-- 던전 파티 클리이 횟수
		% dungeonSiglePlayPT													//@DSPlayTime					int					-- 던전 싱글 플래이 시간(초)
		% dungeonParyPlayPT														//@DPPlayTime					int					-- 던전 파티 플래이 시간(초)
		% etcData[US::US_Etc_Pvp_Kill_Count]									//@VsKillCnt					int					-- 대전 Kill 수
		% pvpWinCount															//@VsWinCnt						smallint			-- 대전 승 수
		% pvpLoseCount															//@VsLoseCnt					smallint			-- 대전 패 수
		% trainingcenterPT														//@TrainingPT					int					-- 트레이닝 플래이 시간(초)
		% trainingcenterCount													//@TrainingCnt					int					-- 트레이닝 플래이 횟수
		% etcData[US::US_Etc_TradeCount]										//@P2PCnt						smallint			-- 개인 거래 횟 수
		% etcData[US::US_Etc_Square_Count]										//@SquareCnt					int					-- 광장 출입 횟 수
		% etcData[US::US_Etc_Square_PlayTime]									//@SquarePlayTime				int					-- 광장 머문 시간
		% etcData[US::US_Etc_Trade_Buy_Item]									//@BuyItemCnt					int					-- 구매한 아이템 수
		% etcData[US::US_Etc_Trade_Sell_Item]									//@SellItemCnt					int					-- 판매한 아이템 수
		% etcData[US::US_Etc_Obtained_Item]										//@GetItemCnt					int					-- 획득한 아이템 수
		% reStoneCount															//@ReStoneCnt					int					-- 총 사용한 부활석 수

			)
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LUnitStatistics_SET", 
		L"%d, "
		//Connect 5개 
		L"%d, N\'%s\', N\'%s\', %d, %d,"											 //서버번호, 로그인일시, 로그아웃 일시, PC방 여부, 유저 권한레벨
		//User 3개 
		L"%d, %d, N\'%s\', "													//계정고유번호, 퍼블리셔SN, 계정생성일시
		//Unit 9개 
		L"%d, N\'%s\', %d, %d, %d, %d, N\'%s\', %d, %d, "					//고유번호, 닉네임, 클래스, 경험치, 레벨, ED, 생성일시, 총 완료 퀘스트 수, 총 중복 완료 퀘스트수
		//ED 26개
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//던전 획득, 트레이닝 획득, 퀘스트 획득, 아이템 판매 획득, 아이템 거래 획득, 아이템 개인거래 획득, 우편 수령 획득, 아이템 판매, 거래지불, 아이템 수리비,
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//아이템 강화 수수료, 아이템 소켓 수수료, 아이템 속성 수수료, 아이템 제조 수수료, 던전 이탈 패널티, 개인거래지불, 개인상점 개설 수수료, 개인상점 구매 수수료, 개인상점 판매 수수료, 우편첨부,
		L"%d, %d, %d, %d, %d, %d,"												// 우편발송 수수료, 우편 아이템 첨부 수수료, 큐브 오픈 수수료, 필드 획득, 코보 익스프레스 수수료, 분해 수수료
		//Play 23개
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//사용 근성도, 근송도 모두 소모여부, 총플래이 시간, 총대기 시간, 총대전 시간, 던전 싱글 플래이 횟수, 던전 파티 플래이 횟수, 던전 싱글 클리어 횟수, 던전 파티 클리어 횟수, 던전 싱글 플래이 시간,
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d,"								//던전 파티 플래이 시간, 대전  Kill수 , 대전 승수, 대전 패수, 트레이닝 플래이 시간, 트레이닝 플래이 횟수, 개인 거래 횟수, 광장 출입 횟수, 광장 머문시간, 구매한 아이템수, 
		L"%d, %d, %d" ,															//판매한 아이템 수, 획득한 아이템 수, 총 사용한 부활석
		//Connect 5개 
		% KBaseServer::GetKObj()->GetServerGroupID()							//@iServerSetID TINYINT   
		% KBaseServer::GetKObj()->GetUID()										//@ServerUID					int					-- 서버 번호
		% kPacket_.m_wstrLoginTime												//@logintime					datetime			-- 로그인 일시
		% kPacket_.m_wstrLogoutTime												//@logouttime					datetime			-- 로그아웃 일시
		% iIsPcBang																//@PCBangFlag					bit					-- PC방 여부
		% kPacket_.m_iAuthLevel													//@AuthLevel					tinyint				-- 계정 권한
		//User 3개 
		% kPacket_.m_iUserUID													//@UserUID						bigint				-- 계정 고유번호
		% kPacket_.m_uiGiantUID													//@PublisherSN					int					-- 퍼블리셔 SN
		% kPacket_.m_wstrAccountCreateDate										//@UserRegDate					smalldatetime		-- 계정 생성일시
		//Unit 9개 
		% kPacket_.m_iUnitUID													//@UnitUID						bigint				-- 고유번호
		% kPacket_.m_wstrNickName												//@NickName						nvarchar(20)		-- 닉네임
		% ( int )kPacket_.m_cUnitClass											//@UnitClass					tinyint				-- 클래스
		% kPacket_.m_iEXP														//@Exp							int					-- 경험치
		% (int)kPacket_.m_ucLevel												//@Level						tinyint				-- 레벨
		% kPacket_.m_iED														//@GamePoint					int					-- ED
		% kPacket_.m_wstrUnitCreateDate											//@UnitRegDate					smalldatetime		-- 생성일시
		% etcData[US::US_Etc_NQuestComplete]									//@QuestCnt						int					-- 총 완료 퀘스트 수
		% etcData[US::US_Etc_RQuestComplete]									//@ReQuestCnt					int					-- 총 중복 완료 퀘스트 수
		//ED 23개
		% edData[US::US_ED_PDungeon]											//@PDungeon						int					-- 던전 획득
		% edData[US::US_ED_PTraining]											//PTraining						int					-- 트레이닝 획득
		% edData[US::US_ED_PQuest]												//@PQuest						int					-- 퀘스트 획득
		% edData[US::US_ED_PItemSell]											//@PItemSell					int					-- 아이템 판매 획득
		% edData[US::US_ED_PItemTrade]											//@PItemTrade					bigint				-- 아이템 거래 획득
		% edData[US::US_ED_PPersonalShop]										//@PPersonalShop				bigint				-- 아이템 개인 거래 획득
		% edData[US::US_ED_PPostLetter]											//@PPostLetter					bigint				-- 우편 수령 획득
		% edData[US::US_ED_MItemBuy]											//@MItemBuy						int					-- 아이템 판매
		% edData[US::US_ED_MItemTrade]											//@MItemTrade					bigint				-- 거래 지불
		% edData[US::US_ED_MItemRepair]											//@MItemRepair					int					-- 아이템 수리비
		% edData[US::US_ED_MEnchant]											//@MItemEnchant					bigint				-- 아이템 강화 수수료
		% edData[US::US_ED_MSocket]												//@MItemSocket					int					-- 아이템 소켓 수수료
		% edData[US::US_ED_MAttribEnchant]										//@MItemAttribute				int					-- 아이템 속성 수수료
		% edData[US::US_ED_MManufacture]										//@MManufacture					int					-- 아이템 제조 수수료
		% edData[US::US_ED_MDunPenalty]											//@MDunPenalty					int					-- 던전 이탈 패널티
		% edData[US::US_ED_MPersonalShop]										//@MPersonalShop				bigint				-- 개인 거래 지불
		% edData[US::US_ED_MOpenPShop]											//@MOpenPShop					bigint				-- 개인 상점 개설 수수료
		% edData[US::US_ED_MSellTaxPShop]										//@MSellTaxPShop				int					-- 개인 상점 구매 수수료
		% edData[US::US_ED_MBuyTaxPShop]										//@MBuyTaxPShop					int					-- 개인 상점 판매 수수료
		% edData[US::US_ED_MPostLetter]											//@MPostLetter					bigint				-- 우편 첨부
		% edData[US::US_ED_MSendLetterBasic]									//@MSendLetterBasic				int					-- 우편 발송 수수료
		% edData[US::US_ED_MSendLetterItem]										//@MSendLetterItem				int					-- 우편 아이템 첨부 수수료
		% etcData[US::US_ED_OpenCubeSpendED]									//@iOpenCube					int					-- 큐브 오픈 수수료 // SERV_CUBE_OPEN_ED_CONDITION
		% etcData[US::US_ED_PBattleFieldED]									//@iPBattleFieldED				int					-- 필드 획득		// SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
		% etcData[US::US_ED_MCoboExpressTicketED]								//@iMCoboExpressTicketED		int					-- 코보 익스프레스 수수료 // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
		% etcData[US::US_ED_MItemResolve]										//@iMItemDecom					bigint				-- 분해 수수료		// SERV_MULTI_RESOLVE
		//Play 23개
		% etcData[US::US_Etc_Used_Spirit]										//@UseSpirit					int					-- 사용 근성도
		% etcData[US::US_Etc_AllUsed_Spirit_Count]								//@AllSpirit					bit					-- 근성도 모두 소모 여부
		% etcData[US::US_Etc_Dungeon_Play_Time]									//@TotalPlayTime				int					-- 총 플래이 시간(초)
		% kPacket_.m_iiDurationTime												//@DurationTime					int					-- 총 대기 시간(초)
		% etcData[US::US_Etc_PVP_Play_Time]										//@VsPlayTime					int					-- 총 대전 시간(초)
		% dungeonSiglePlayCount													//@DSoloCnt						int					-- 던전 싱글 플래이 횟수
		% dungeonParyPlayCount													//@DPartyCnt					int					-- 던전 파티 플래이 횟수
		% dungeon[US::US_D_MC_Win]												//@DSWCnt						int					-- 던전 싱글 클리이 횟수
		% dungeon[US::US_D_MC_Party_Win]										//@DPWCnt						int					-- 던전 파티 클리이 횟수
		% dungeonSiglePlayPT													//@DSPlayTime					int					-- 던전 싱글 플래이 시간(초)
		% dungeonParyPlayPT														//@DPPlayTime					int					-- 던전 파티 플래이 시간(초)
		% etcData[US::US_Etc_Pvp_Kill_Count]									//@VsKillCnt					int					-- 대전 Kill 수
		% pvpWinCount															//@VsWinCnt						smallint			-- 대전 승 수
		% pvpLoseCount															//@VsLoseCnt					smallint			-- 대전 패 수
		% trainingcenterPT														//@TrainingPT					int					-- 트레이닝 플래이 시간(초)
		% trainingcenterCount													//@TrainingCnt					int					-- 트레이닝 플래이 횟수
		% etcData[US::US_Etc_TradeCount]										//@P2PCnt						smallint			-- 개인 거래 횟 수
		% etcData[US::US_Etc_Square_Count]										//@SquareCnt					int					-- 광장 출입 횟 수
		% etcData[US::US_Etc_Square_PlayTime]									//@SquarePlayTime				int					-- 광장 머문 시간
		% etcData[US::US_Etc_Trade_Buy_Item]									//@BuyItemCnt					int					-- 구매한 아이템 수
		% etcData[US::US_Etc_Trade_Sell_Item]									//@SellItemCnt					int					-- 판매한 아이템 수
		% etcData[US::US_Etc_Obtained_Item]										//@GetItemCnt					int					-- 획득한 아이템 수
		% reStoneCount															//@ReStoneCnt					int					-- 총 사용한 부활석 수
		
		)
#endif //SERV_ALL_RENEWAL_SP		
		
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}	
#else //SERV_USER_STATISTICS_RENEWAL
	if( kPacket_.m_mapEtcStatistics.empty() == false )
	{
		// [주의!] 모든 데이터 테이블들은 반드시 0으로 초기화!
		int etcData[KUserStatistics::US_Etc_Max] = {0};

		UserStatisticsTableParsing( kPacket_.m_mapEtcStatistics, KUserStatistics::USTable_EtcData, 
			0, KUserStatistics::US_Etc_Max, etcData );

		int iIsPcBang = ( etcData[KUserStatistics::US_Etc_IsPcBang] == 0 ? 0 : 1 );

		// []연산자로 배열의 원소를 참조할때 선언된 배열의 크기를 넘어서는
		// 값을 참조하지 않도록 조심
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_UnitStatistics_Etc_SET", 
			L"%d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% kPacket_.m_iUserUID						//  @iUserUID		[bigint]
			% kPacket_.m_iUnitUID						//	@iUnitUID		[bigint]
			% etcData[US::US_Etc_Used_Spirit]			//	@iUseSpirit		[smallint]
			% etcData[US::US_Etc_AllUsed_Spirit_Count]	//	@bAllSpirit		[bit]
			% etcData[US::US_Etc_Duration_Time]			//	@iDurationTime	[int]
			% etcData[US::US_Etc_Pvp_Kill_Count]		//	@iVsKill		[smallint]
			% etcData[US::US_Etc_Obtained_Item]			//	@iGetItem		[smallint]
			% etcData[US::US_Etc_Shop_Buy_Item]			//	@iBuyItem		[smallint]
			% etcData[US::US_Etc_Shop_Sell_Item]		//	@iSellItem		[smallint]
			% etcData[US::US_Etc_Trade_Buy_Item]		//	@iP2PBuyItem	[smallint]
			% etcData[US::US_Etc_Trade_Sell_Item]		//	@iP2PSellItem	[smallint]
			% etcData[US::US_Etc_Square_Count]			//	@iSquare_Cnt	[smallint]
			% etcData[US::US_Etc_Square_PlayTime]		//	@iSquare_Time	[int]
			% iIsPcBang									//	@bIsPCBang		[bit]
				);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsEtcUpdateFirst", 
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d",
			% kPacket_.m_iUserUID						//  @iUserUID		[bigint]
			% kPacket_.m_iUnitUID						//	@iUnitUID		[bigint]
			% etcData[US::US_Etc_Used_Spirit]			//	@iUseSpirit		[smallint]
			% etcData[US::US_Etc_AllUsed_Spirit_Count]	//	@bAllSpirit		[bit]
			% etcData[US::US_Etc_Duration_Time]			//	@iDurationTime	[int]
			% etcData[US::US_Etc_Pvp_Kill_Count]		//	@iVsKill		[smallint]
			% etcData[US::US_Etc_Obtained_Item]			//	@iGetItem		[smallint]
			% etcData[US::US_Etc_Shop_Buy_Item]			//	@iBuyItem		[smallint]
			% etcData[US::US_Etc_Shop_Sell_Item]		//	@iSellItem		[smallint]
			% etcData[US::US_Etc_Trade_Buy_Item]		//	@iP2PBuyItem	[smallint]
			% etcData[US::US_Etc_Trade_Sell_Item]		//	@iP2PSellItem	[smallint]
			% etcData[US::US_Etc_Square_Count]			//	@iSquare_Cnt	[smallint]
			% etcData[US::US_Etc_Square_PlayTime]		//	@iSquare_Time	[int]
			% iIsPcBang									//	@bIsPCBang		[bit]
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_UnitStatistics_Etc2_SET",
			L"%d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% kPacket_.m_iUserUID						//  @iUserUID		[bigint]
			% kPacket_.m_iUnitUID						//	@iUnitUID		[bigint]			
			% etcData[US::US_Etc_PDungeonED]			//	@iPDunED		[int]
			% etcData[US::US_Etc_PQuestED]				//	@iPQuestED		[int]
			% etcData[US::US_Etc_PItemSellED]			//	@iPItemSellED	[int]			
			% etcData[US::US_Etc_MItemBuyED]			//	@iMItemBuyED	[int]
			% etcData[US::US_Etc_MManufactureED]		//	@iMItemMakeED	[int]
			% etcData[US::US_Etc_MItemRepairED]			//	@iMItemRepairED [int]
			% etcData[US::US_Etc_MOpenPShopED]			//	@iMStoreP2P		[int]
			% etcData[US::US_Etc_MEnchantED]			//	@iMEnchant		[int]
			% etcData[US::US_Etc_MSocketED]				//	@iMSocket		[int]
			% etcData[US::US_Etc_TradeCount]			//	@iP2PCnt		[int]
			% etcData[US::US_Etc_NQuestAccept]			//	@iNQuestAcc		[int]
			% etcData[US::US_Etc_NQuestGiveUp]			//	@iNQuestGiveUp	[int]
			% etcData[US::US_Etc_NQuestComplete]		//	@iNQuestComplete[int]
			% etcData[US::US_Etc_RQuestAccept]			//	@iRQuestAcc		[int]
			% etcData[US::US_Etc_RQuestGiveUp]			//	@iRQuestGiveUp	[int]
			% etcData[US::US_Etc_RQuestComplete]		//	@iRQuestComplete[int]
			//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
			% etcData[US::US_Etc_OpenCubeSpendED]		//	@iOpenCube[int]
//#endif SERV_CUBE_OPEN_ED_CONDITION
			//}
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
			% etcData[US::US_Etc_PBattleFieldED]		//	@iPBattleFieldED[int]
			% etcData[US::US_Etc_MCoboExpressTicketED]	//	@iMCoboExpressTicketED[int]
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG	

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
			% etcData[US::US_Etc_MItemEvaluate]			//	@iMItemEvaluate[int]
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsEtcUpdateSecond",
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d",
			% kPacket_.m_iUserUID						//  @iUserUID		[bigint]
			% kPacket_.m_iUnitUID						//	@iUnitUID		[bigint]			
			% etcData[US::US_Etc_PDungeonED]			//	@iPDunED		[int]
			% etcData[US::US_Etc_PQuestED]				//	@iPQuestED		[int]
			% etcData[US::US_Etc_PItemSellED]			//	@iPItemSellED	[int]			
			% etcData[US::US_Etc_MItemBuyED]			//	@iMItemBuyED	[int]
			% etcData[US::US_Etc_MManufactureED]		//	@iMItemMakeED	[int]
			% etcData[US::US_Etc_MItemRepairED]			//	@iMItemRepairED [int]
			% etcData[US::US_Etc_MOpenPShopED]			//	@iMStoreP2P		[int]
			% etcData[US::US_Etc_MEnchantED]			//	@iMEnchant		[int]
			% etcData[US::US_Etc_MSocketED]				//	@iMSocket		[int]
			% etcData[US::US_Etc_TradeCount]			//	@iP2PCnt		[int]
			% etcData[US::US_Etc_NQuestAccept]			//	@iNQuestAcc		[int]
			% etcData[US::US_Etc_NQuestGiveUp]			//	@iNQuestGiveUp	[int]
			% etcData[US::US_Etc_NQuestComplete]		//	@iNQuestComplete[int]
			% etcData[US::US_Etc_RQuestAccept]			//	@iRQuestAcc		[int]
			% etcData[US::US_Etc_RQuestGiveUp]			//	@iRQuestGiveUp	[int]
			% etcData[US::US_Etc_RQuestComplete]		//	@iRQuestComplete[int]
			//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
			% etcData[US::US_Etc_OpenCubeSpendED]		//	@iOpenCube[int]
//#endif SERV_CUBE_OPEN_ED_CONDITION
			//}
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
			% etcData[US::US_Etc_PBattleFieldED]		//	@iPBattleFieldED[int]
			% etcData[US::US_Etc_MCoboExpressTicketED]	//	@iMCoboExpressTicketED[int]
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
			% etcData[US::US_Etc_MItemEvaluate]			//	@iMItemEvaluate[int]
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK_2 );

			m_kODBC.EndFetch();
		}

		LOG_SUCCESS( iOK == NetError::NET_OK && iOK_2 == NetError::NET_OK )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( iOK_2 )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_mapEtcStatistics.size() )
			<< BUILD_LOG( etcData[US::US_Etc_Used_Spirit] )
			<< BUILD_LOG( etcData[US::US_Etc_AllUsed_Spirit_Count] )
			<< BUILD_LOG( etcData[US::US_Etc_Duration_Time] )
			<< BUILD_LOG( etcData[US::US_Etc_Pvp_Kill_Count] )
			<< BUILD_LOG( etcData[US::US_Etc_Obtained_Item] )
			<< BUILD_LOG( etcData[US::US_Etc_Shop_Buy_Item] )
			<< BUILD_LOG( etcData[US::US_Etc_Shop_Sell_Item] )
			<< BUILD_LOG( etcData[US::US_Etc_Trade_Buy_Item] )
			<< BUILD_LOG( etcData[US::US_Etc_Trade_Sell_Item] )
			<< BUILD_LOG( etcData[US::US_Etc_Square_Count] )
			<< BUILD_LOG( etcData[US::US_Etc_Square_PlayTime] )
			<< BUILD_LOG( etcData[US::US_Etc_IsPcBang] )
			<< BUILD_LOG( etcData[US::US_Etc_PDungeonED] )
			<< BUILD_LOG( etcData[US::US_Etc_PQuestED] )
			<< BUILD_LOG( etcData[US::US_Etc_PItemSellED] )			
			<< BUILD_LOG( etcData[US::US_Etc_MItemBuyED] )
			<< BUILD_LOG( etcData[US::US_Etc_MManufactureED] )
			<< BUILD_LOG( etcData[US::US_Etc_MItemRepairED] )
			<< BUILD_LOG( etcData[US::US_Etc_MOpenPShopED] )
			<< BUILD_LOG( etcData[US::US_Etc_MEnchantED] )
			<< BUILD_LOG( etcData[US::US_Etc_MSocketED] )
			<< BUILD_LOG( etcData[US::US_Etc_TradeCount] )
			<< BUILD_LOG( etcData[US::US_Etc_NQuestAccept] )
			<< BUILD_LOG( etcData[US::US_Etc_NQuestGiveUp] )
			<< BUILD_LOG( etcData[US::US_Etc_NQuestComplete] )
			<< BUILD_LOG( etcData[US::US_Etc_RQuestAccept] )
			<< BUILD_LOG( etcData[US::US_Etc_RQuestGiveUp] )
			<< BUILD_LOG( etcData[US::US_Etc_RQuestComplete] )
			//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
			<< BUILD_LOG( etcData[US::US_Etc_OpenCubeSpendED] )
//#endif SERV_CUBE_OPEN_ED_CONDITION
			//}
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
			<< BUILD_LOG( etcData[US::US_Etc_PBattleFieldED] )
			<< BUILD_LOG( etcData[US::US_Etc_MCoboExpressTicketED] )
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
			<< BUILD_LOG( etcData[US::US_Etc_MItemEvaluate] )
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
			<< END_LOG;
	}

	if( kPacket_.m_mapPvpStatistics.empty() == false )
	{
		// 대전 통계 : [주의!] 모든 데이터 테이블들은 반드시 0으로 초기화!
		int pvpMcTeam[KUserStatistics::US_Pvp_MC_Max] = {0};
		int pvpMcTeamDeath[KUserStatistics::US_Pvp_MC_Max] = {0};
		int pvpMcDeath[KUserStatistics::US_Pvp_MC_Max] = {0};
		
		int pvpPtTeam[KUserStatistics::US_Pvp_PT_Max] = {0};
		int pvpPtTeamDeath[KUserStatistics::US_Pvp_PT_Max] = {0};
		int pvpPtDeath[KUserStatistics::US_Pvp_PT_Max] = {0};

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpModeCount, 
			CXSLRoom::PGT_TEAM, KUserStatistics::US_Pvp_MC_Max, pvpMcTeam );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpModeCount,
			CXSLRoom::PGT_TEAM_DEATH, KUserStatistics::US_Pvp_MC_Max, pvpMcTeamDeath );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpModeCount,
			CXSLRoom::PGT_SURVIVAL, KUserStatistics::US_Pvp_MC_Max, pvpMcDeath );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpPlayTime, 
			CXSLRoom::PGT_TEAM, KUserStatistics::US_Pvp_MC_Max, pvpPtTeam );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpPlayTime,
			CXSLRoom::PGT_TEAM_DEATH, KUserStatistics::US_Pvp_MC_Max, pvpPtTeamDeath );

		UserStatisticsTableParsing( kPacket_.m_mapPvpStatistics, KUserStatistics::USTable_PvpPlayTime,
			CXSLRoom::PGT_SURVIVAL, KUserStatistics::US_Pvp_MC_Max, pvpPtDeath );

		// []연산자로 배열의 원소를 참조할때 선언된 배열의 크기를 넘어서는 값을 참조하지 않도록 조심		
		// 배열값 참조할때 enum을 쓰려고 하면 "error C2026: 문자열이 너무 길어 뒤에 오는 문자가 잘립니다."에러 발생!
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsVs_SET",
					L"%d, "
					L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
					L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
					L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
					% KBaseServer::GetKObj()->GetServerGroupID()
					% kPacket_.m_iUnitUID
					% pvpMcTeam[0]			// @iTMW_Cnt		smallint
					% pvpMcTeam[1]			// @iTML_Cnt		smallint
					% pvpMcTeam[2]			// @iTMG_Cnt		smallint
					% pvpMcTeam[3]			// @iTMD_Cnt		smallint
					% pvpMcTeamDeath[0]	// @iTDW_Cnt		smallint
					% pvpMcTeamDeath[1]	// @iTDL_Cnt		smallint
					% pvpMcTeamDeath[2]	// @iTDG_Cnt		smallint
					% pvpMcTeamDeath[3]	// @iTDD_Cnt		smallint
					% pvpMcTeamDeath[4]	// @iTDNan_Cnt		smallint
					% pvpMcDeath[0]		// @iDW_Cnt			smallint
					% pvpMcDeath[1]		// @iDL_Cnt			smallint
					% pvpMcDeath[2]		// @iDG_Cnt			smallint
					% pvpMcDeath[3]		// @iDD_Cnt			smallint
					% pvpMcDeath[4]		// @iDNan_Cnt		smallint
					% pvpPtTeam[0]			// @iTMW_Time		int
					% pvpPtTeam[1]			// @iTML_Time		int
					% pvpPtTeam[2]			// @iTMG_Time		int
					% pvpPtTeam[3]			// @iTMD_Time		int
					% pvpPtTeamDeath[0]	// @iTDW_Time		int
					% pvpPtTeamDeath[1]	// @iTDL_Time		int
					% pvpPtTeamDeath[2]	// @iTDG_Time		int
					% pvpPtTeamDeath[3]	// @iTDD_Time		int
					% pvpPtTeamDeath[4]	// @iTDNan_Time		int
					% pvpPtDeath[0]		// @iDW_Time		int
					% pvpPtDeath[1]		// @iDL_Time		int
					% pvpPtDeath[2]		// @iDG_Time		int
					% pvpPtDeath[3]		// @iDD_Time		int
					% pvpPtDeath[4]		// @iDNan_Time		int
					);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsVsUpdate",
					L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
					L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
					L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
					 % kPacket_.m_iUnitUID
					 % pvpMcTeam[0]			// @iTMW_Cnt		smallint
					 % pvpMcTeam[1]			// @iTML_Cnt		smallint
					 % pvpMcTeam[2]			// @iTMG_Cnt		smallint
					 % pvpMcTeam[3]			// @iTMD_Cnt		smallint
					 % pvpMcTeamDeath[0]	// @iTDW_Cnt		smallint
					 % pvpMcTeamDeath[1]	// @iTDL_Cnt		smallint
					 % pvpMcTeamDeath[2]	// @iTDG_Cnt		smallint
					 % pvpMcTeamDeath[3]	// @iTDD_Cnt		smallint
					 % pvpMcTeamDeath[4]	// @iTDNan_Cnt		smallint
					 % pvpMcDeath[0]		// @iDW_Cnt			smallint
					 % pvpMcDeath[1]		// @iDL_Cnt			smallint
					 % pvpMcDeath[2]		// @iDG_Cnt			smallint
					 % pvpMcDeath[3]		// @iDD_Cnt			smallint
					 % pvpMcDeath[4]		// @iDNan_Cnt		smallint
					 % pvpPtTeam[0]			// @iTMW_Time		int
					 % pvpPtTeam[1]			// @iTML_Time		int
					 % pvpPtTeam[2]			// @iTMG_Time		int
					 % pvpPtTeam[3]			// @iTMD_Time		int
					 % pvpPtTeamDeath[0]	// @iTDW_Time		int
					 % pvpPtTeamDeath[1]	// @iTDL_Time		int
					 % pvpPtTeamDeath[2]	// @iTDG_Time		int
					 % pvpPtTeamDeath[3]	// @iTDD_Time		int
					 % pvpPtTeamDeath[4]	// @iTDNan_Time		int
					 % pvpPtDeath[0]		// @iDW_Time		int
					 % pvpPtDeath[1]		// @iDL_Time		int
					 % pvpPtDeath[2]		// @iDG_Time		int
					 % pvpPtDeath[3]		// @iDD_Time		int
					 % pvpPtDeath[4]		// @iDNan_Time		int
					 );
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			 FETCH_DATA( iOK );

			 m_kODBC.EndFetch();
		}

		LOG_SUCCESS( iOK == NetError::NET_OK )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( pvpMcTeam[0] )	
			<< BUILD_LOG( pvpMcTeam[1] )
			<< BUILD_LOG( pvpMcTeam[2] )
			<< BUILD_LOG( pvpMcTeam[3] )
			<< BUILD_LOG( pvpMcTeamDeath[0] )
			<< BUILD_LOG( pvpMcTeamDeath[1] )
			<< BUILD_LOG( pvpMcTeamDeath[2] )
			<< BUILD_LOG( pvpMcTeamDeath[3] )
			<< BUILD_LOG( pvpMcTeamDeath[4] )
			<< BUILD_LOG( pvpMcDeath[0]	)
			<< BUILD_LOG( pvpMcDeath[1]	)
			<< BUILD_LOG( pvpMcDeath[2]	)
			<< BUILD_LOG( pvpMcDeath[3]	)
			<< BUILD_LOG( pvpMcDeath[4]	)
			<< BUILD_LOG( pvpPtTeam[0] )	
			<< BUILD_LOG( pvpPtTeam[1] )	
			<< BUILD_LOG( pvpPtTeam[2] )
			<< BUILD_LOG( pvpPtTeam[3] )
			<< BUILD_LOG( pvpPtTeamDeath[0])
			<< BUILD_LOG( pvpPtTeamDeath[1])
			<< BUILD_LOG( pvpPtTeamDeath[2])
			<< BUILD_LOG( pvpPtTeamDeath[3])
			<< BUILD_LOG( pvpPtTeamDeath[4])
			<< BUILD_LOG( pvpPtDeath[0]	)
			<< BUILD_LOG( pvpPtDeath[1]	)
			<< BUILD_LOG( pvpPtDeath[2]	)
			<< BUILD_LOG( pvpPtDeath[3]	)
			<< BUILD_LOG( pvpPtDeath[4]	)
			<< BUILD_LOG( kPacket_.m_mapPvpStatistics.size() )
			<< END_LOG;
	}

	// 던전 통계
	if( kPacket_.m_mapDungeonStatistics.empty() == false )
	{
		int dungeon[KUserStatistics::US_D_Max] = {0};

		UserStatisticsIterator mit = kPacket_.m_mapDungeonStatistics.begin();
		for( ; mit != kPacket_.m_mapDungeonStatistics.end(); mit++ )
		{
			if( mit->first.first != KUserStatistics::USTable_Dungeon )
			{
				START_LOG( cerr, L"던전통계테이블도 아닌것이 던전통계에 들어왔다?" )
					<< BUILD_LOG( mit->first.first )
					<< END_LOG;

				continue;
			}

			UserStatisticsTableParsing( kPacket_.m_mapDungeonStatistics, KUserStatistics::USTable_Dungeon,
				mit->first.second, KUserStatistics::US_D_Max, dungeon );

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatsDungeon_MER",
				L"%d, "
				L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
				L"%d, %d, %d, %d, %d, %d",
				% KBaseServer::GetKObj()->GetServerGroupID()
				% kPacket_.m_iUnitUID
				% mit->first.second
				% dungeon[KUserStatistics::US_D_MC_Win]		  // @iNSW_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Lose]		  // @iNSL_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Drop]		  // @iNSG_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_ReStone]	  // @iNSRS_Cnt		[smallint]
				% dungeon[KUserStatistics::US_D_MC_Party_Win]	  // @iNPW_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Party_Lose]	  // @iNPL_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Party_Drop]	  // @iNPG_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Party_ReStone]// @iNPRS_Cnt		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Win]		  // @iNSW_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Lose]		  // @iNSL_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Drop]		  // @iNSG_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Party_Win]	  // @iNPW_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Party_Lose]	  // @iNPL_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Party_Drop]	  // @iNPG_Time		[smallint]
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatsDungeon_SET",
				L"%d, "
				L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
				L"%d, %d, %d, %d, %d, %d",
				% KBaseServer::GetKObj()->GetServerGroupID()
				% kPacket_.m_iUnitUID
				% mit->first.second
				% dungeon[KUserStatistics::US_D_MC_Win]		  // @iNSW_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Lose]		  // @iNSL_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Drop]		  // @iNSG_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_ReStone]	  // @iNSRS_Cnt		[smallint]
				% dungeon[KUserStatistics::US_D_MC_Party_Win]	  // @iNPW_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Party_Lose]	  // @iNPL_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Party_Drop]	  // @iNPG_Cnt		[tinyint]
				% dungeon[KUserStatistics::US_D_MC_Party_ReStone]// @iNPRS_Cnt		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Win]		  // @iNSW_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Lose]		  // @iNSL_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Drop]		  // @iNSG_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Party_Win]	  // @iNPW_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Party_Lose]	  // @iNPL_Time		[smallint]
				% dungeon[KUserStatistics::US_D_PT_Party_Drop]	  // @iNPG_Time		[smallint]
				);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.StatsDungeonUpdate",
						L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
						L"%d, %d, %d, %d, %d, %d",
						 % kPacket_.m_iUnitUID
						 % mit->first.second
						 % dungeon[KUserStatistics::US_D_MC_Win]		  // @iNSW_Cnt		[tinyint]
						 % dungeon[KUserStatistics::US_D_MC_Lose]		  // @iNSL_Cnt		[tinyint]
						 % dungeon[KUserStatistics::US_D_MC_Drop]		  // @iNSG_Cnt		[tinyint]
						 % dungeon[KUserStatistics::US_D_MC_ReStone]	  // @iNSRS_Cnt		[smallint]
						 % dungeon[KUserStatistics::US_D_MC_Party_Win]	  // @iNPW_Cnt		[tinyint]
						 % dungeon[KUserStatistics::US_D_MC_Party_Lose]	  // @iNPL_Cnt		[tinyint]
						 % dungeon[KUserStatistics::US_D_MC_Party_Drop]	  // @iNPG_Cnt		[tinyint]
						 % dungeon[KUserStatistics::US_D_MC_Party_ReStone]// @iNPRS_Cnt		[smallint]
						 % dungeon[KUserStatistics::US_D_PT_Win]		  // @iNSW_Time		[smallint]
						 % dungeon[KUserStatistics::US_D_PT_Lose]		  // @iNSL_Time		[smallint]
						 % dungeon[KUserStatistics::US_D_PT_Drop]		  // @iNSG_Time		[smallint]
						 % dungeon[KUserStatistics::US_D_PT_Party_Win]	  // @iNPW_Time		[smallint]
						 % dungeon[KUserStatistics::US_D_PT_Party_Lose]	  // @iNPL_Time		[smallint]
						 % dungeon[KUserStatistics::US_D_PT_Party_Drop]	  // @iNPG_Time		[smallint]
						 );
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				 FETCH_DATA( iOK );

				 m_kODBC.EndFetch();
			}

			LOG_SUCCESS( iOK == NetError::NET_OK )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( mit->first.first )
				<< BUILD_LOG( mit->first.second )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_Win]  )	
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_Lose]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_Drop]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_ReStone]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_Party_Win]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_Party_Lose]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_Party_Drop]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_MC_Party_ReStone]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_PT_Win]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_PT_Lose]  )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_PT_Drop] )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_PT_Party_Win] )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_PT_Party_Lose] )
				<< BUILD_LOG( dungeon[KUserStatistics::US_D_PT_Party_Drop] )
				<< BUILD_LOG( kPacket_.m_mapDungeonStatistics.size() )
				<< END_LOG;
		}
	}

	// 훈련소 통계
	if( kPacket_.m_mapTCenterStatistics.empty() == false )
	{
		int TrainingCenter[KUserStatistics::US_TC_Max] = {0};

		UserStatisticsIterator mit = kPacket_.m_mapTCenterStatistics.begin();
		for( ; mit != kPacket_.m_mapTCenterStatistics.end(); mit++ )
		{
			if( mit->first.first != KUserStatistics::USTable_TrainingCenter )
			{
				START_LOG( cerr, L"훈련소통계테이블도 아닌것이 훈련소통계에 들어왔다?" )
					<< BUILD_LOG( mit->first.first )
					<< END_LOG;

				continue;
			}

			UserStatisticsTableParsing( kPacket_.m_mapTCenterStatistics, KUserStatistics::USTable_TrainingCenter,
				mit->first.second, KUserStatistics::US_TC_Max, TrainingCenter );
		
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_StatsTraining_SET", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
							% KBaseServer::GetKObj()->GetServerGroupID()
							% kPacket_.m_iUnitUID									//	@iUnitUID		  [bigint]
							% mit->first.second										//	@iTrainingID	  [tinyint]
							% TrainingCenter[KUserStatistics::US_TC_MC_Success] 	//	@iWin_Cnt		  [int]
							% TrainingCenter[KUserStatistics::US_TC_MC_Fail]		//	@iLose_Cnt		  [int]
							% TrainingCenter[KUserStatistics::US_TC_MC_Drop]		//	@iGiveUp_Cnt	  [int]
							% TrainingCenter[KUserStatistics::US_TC_PT_Success] 	//	@iWin_Time		  [bigint]
							% TrainingCenter[KUserStatistics::US_TC_PT_Fail]		//	@iLose_Time		  [bigint]
							% TrainingCenter[KUserStatistics::US_TC_PT_Drop]		//	@iGiveUp_Time	  [bigint]
							);
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.StatsTrainingUpdate", L"%d, %d, %d, %d, %d, %d, %d, %d",
						% kPacket_.m_iUnitUID									//	@iUnitUID		  [bigint]
						% mit->first.second										//	@iTrainingID	  [tinyint]
						% TrainingCenter[KUserStatistics::US_TC_MC_Success] 	//	@iWin_Cnt		  [int]
						% TrainingCenter[KUserStatistics::US_TC_MC_Fail]		//	@iLose_Cnt		  [int]
						% TrainingCenter[KUserStatistics::US_TC_MC_Drop]		//	@iGiveUp_Cnt	  [int]
						% TrainingCenter[KUserStatistics::US_TC_PT_Success] 	//	@iWin_Time		  [bigint]
						% TrainingCenter[KUserStatistics::US_TC_PT_Fail]		//	@iLose_Time		  [bigint]
						% TrainingCenter[KUserStatistics::US_TC_PT_Drop]		//	@iGiveUp_Time	  [bigint]
						);
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );

				m_kODBC.EndFetch();
			}

			LOG_SUCCESS( iOK == NetError::NET_OK )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first.second )
				<< BUILD_LOG( TrainingCenter[KUserStatistics::US_TC_MC_Success] )
				<< BUILD_LOG( TrainingCenter[KUserStatistics::US_TC_MC_Fail] )
				<< BUILD_LOG( TrainingCenter[KUserStatistics::US_TC_MC_Drop] )
				<< BUILD_LOG( TrainingCenter[KUserStatistics::US_TC_PT_Success] )
				<< BUILD_LOG( TrainingCenter[KUserStatistics::US_TC_PT_Fail] )
				<< BUILD_LOG( TrainingCenter[KUserStatistics::US_TC_PT_Drop] )
				<< BUILD_LOG( kPacket_.m_mapTCenterStatistics.size() )
				<< END_LOG;
		}
	}

	// 아케이드 통계
	//if( kPacket_.m_mapArcadeStatistics.empty() == false )
	//{
	//	int arcade[KUserStatistics::US_A_Max] = {0};

	//	UserStatisticsIterator mit = kPacket_.m_mapArcadeStatistics.begin();
	//	for( ; mit != kPacket_.m_mapArcadeStatistics.end(); mit++ )
	//	{
	//		if( mit->first.first != KUserStatistics::USTable_Arcade )
	//		{
	//			START_LOG( cerr, L"아케이드통계테이블도 아닌것이 아케이드 통계에 들어왔다?" )
	//				<< BUILD_LOG( mit->first.first )
	//				<< END_LOG;

	//			continue;
	//		}

	//		UserStatisticsTableParsing( kPacket_.m_mapArcadeStatistics, KUserStatistics::USTable_Arcade,
	//			mit->first.second, KUserStatistics::US_A_Max, arcade );

	// 만약 주석 해제하면 P_StatsArcade_SET 이거 사용해야함
	//		DO_QUERY( L"exec dbo.StatsArcade_Update",
	//			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
	//			L"%d, %d, %d, %d, %d, %d, %d",
	//			% kPacket_.m_iUnitUID								// @iUnitUID [bigint]
	//			% mit->first.second									// @iArcadeID [int]
	//			% arcade[KUserStatistics::US_A_Floor]				// @iFloor  [tinyint]
	//			% arcade[KUserStatistics::US_A_MC_Single_Win]		// @iSW_Cnt [tinyint]
	//			% arcade[KUserStatistics::US_A_MC_Single_Lose]		// @iSL_Cnt [tinyint]
	//			% arcade[KUserStatistics::US_A_MC_Single_GiveUp]	// @iSG_Cnt [tinyint]
	//			% arcade[KUserStatistics::US_A_MC_Single_ReStone]	// @iSRS_Cnt [smallint]
	//			% arcade[KUserStatistics::US_A_MC_Party_Win]		// @iPW_Cnt [tinyint]
	//			% arcade[KUserStatistics::US_A_MC_Party_Lose]		// @iPL_Cnt [tinyint]
	//			% arcade[KUserStatistics::US_A_MC_Party_GiveUp]		// @iPG_Cnt [tinyint]
	//			% arcade[KUserStatistics::US_A_MC_Party_ReStone]	// @iPRS_Cnt [smallint]
	//			% arcade[KUserStatistics::US_A_PT_Single_Win]		// @iSW_Time [smallint]
	//			% arcade[KUserStatistics::US_A_PT_Single_Lose]		// @iSL_Time [smallint]
	//			% arcade[KUserStatistics::US_A_PT_Single_GiveUp]	// @iSG_Time [smallint]
	//			% arcade[KUserStatistics::US_A_PT_Party_Win]		// @iPW_Time [smallint]
	//			% arcade[KUserStatistics::US_A_PT_Party_Lose]		// @iPL_Time [smallint]
	//			% arcade[KUserStatistics::US_A_PT_Party_GiveUp]		// @iPG_Time [smallint]
	//			);				

	//		if( m_kODBC.BeginFetch() )
	//		{
	//			FETCH_DATA( iOK );

	//			m_kODBC.EndFetch();
	//		}

	//		LOG_SUCCESS( iOK == NetError::NET_OK )
	//			<< BUILD_LOG( iOK )
	//			<< BUILD_LOG( mit->first.first )
	//			<< BUILD_LOG( mit->first.second )
	//			<< BUILD_LOG( kPacket_.m_iUnitUID )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_Floor] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Single_Win] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Single_Lose] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Single_GiveUp] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Single_ReStone] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Party_Win] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Party_Lose] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Party_GiveUp] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_MC_Party_ReStone] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_PT_Single_Win] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_PT_Single_Lose] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_PT_Single_GiveUp] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_PT_Party_Win] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_PT_Party_Lose] )
	//			<< BUILD_LOG( arcade[KUserStatistics::US_A_PT_Party_GiveUp] )
	//			<< BUILD_LOG( kPacket_.m_mapArcadeStatistics.size() )
	//			<< END_LOG;
	//	}
	//}

	{
		// 통계 정리 프로시저 호출
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_UnitStatistics_Once_SET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUnitUID );
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsUnitUpdate_Once", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		LOG_SUCCESS( iOK == NetError::NET_OK )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
#endif //SERV_USER_STATISTICS_RENEWAL

		 
end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_mapEtcStatistics.size() )
		<< BUILD_LOG( kPacket_.m_mapPvpStatistics.size() )
		<< BUILD_LOG( kPacket_.m_mapDungeonStatistics.size() )
		<< BUILD_LOG( kPacket_.m_mapTCenterStatistics.size() )
#ifdef SERV_USER_STATISTICS_RENEWAL
		<< BUILD_LOG( kPacket_.m_mapEDStatistics.size() )
		<< BUILD_LOG( GetKGameServer()->GetServerGroupID() )
		<< BUILD_LOG( kPacket_.m_wstrLoginTime	 )
		<< BUILD_LOG( kPacket_.m_wstrLogoutTime	 )
		<< BUILD_LOG( iIsPcBang	 )
		<< BUILD_LOG( kPacket_.m_iAuthLevel	 )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_uiGiantUID )
		<< BUILD_LOG( kPacket_.m_wstrAccountCreateDate )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrNickName )
		<< BUILD_LOG( ( int )kPacket_.m_cUnitClass )
		<< BUILD_LOG( kPacket_.m_iEXP )
		<< BUILD_LOG( (int)kPacket_.m_ucLevel )
		<< BUILD_LOG( kPacket_.m_iED )
		<< BUILD_LOG( kPacket_.m_wstrUnitCreateDate )
		<< BUILD_LOG( etcData[US::US_Etc_NQuestComplete] )
		<< BUILD_LOG( etcData[US::US_Etc_RQuestComplete] )
		<< BUILD_LOG( edData[US::US_ED_PDungeon] )
		<< BUILD_LOG( edData[US::US_ED_PTraining] )
		<< BUILD_LOG( edData[US::US_ED_PQuest] )
		<< BUILD_LOG( edData[US::US_ED_PItemSell] )
		<< BUILD_LOG( edData[US::US_ED_PItemTrade] )
		<< BUILD_LOG( edData[US::US_ED_PPersonalShop] )
		<< BUILD_LOG( edData[US::US_ED_PPostLetter]	 )
		<< BUILD_LOG( edData[US::US_ED_MItemBuy] )
		<< BUILD_LOG( edData[US::US_ED_MItemTrade] )
		<< BUILD_LOG( edData[US::US_ED_MItemRepair]	 )
		<< BUILD_LOG( edData[US::US_ED_MEnchant] )
		<< BUILD_LOG( edData[US::US_ED_MSocket] )
		<< BUILD_LOG( edData[US::US_ED_MAttribEnchant] )
		<< BUILD_LOG( edData[US::US_ED_MManufacture] )
		<< BUILD_LOG( edData[US::US_ED_MDunPenalty]	 )
		<< BUILD_LOG( edData[US::US_ED_MPersonalShop] )
		<< BUILD_LOG( edData[US::US_ED_MOpenPShop] )
		<< BUILD_LOG( edData[US::US_ED_MSellTaxPShop] )
		<< BUILD_LOG( edData[US::US_ED_MBuyTaxPShop] )
		<< BUILD_LOG( edData[US::US_ED_MPostLetter]	 )
		<< BUILD_LOG( edData[US::US_ED_MSendLetterBasic] )
		<< BUILD_LOG( edData[US::US_ED_MSendLetterItem]	)

		<< BUILD_LOG( etcData[US::US_Etc_Used_Spirit] )
		<< BUILD_LOG( etcData[US::US_Etc_AllUsed_Spirit_Count] )
		<< BUILD_LOG( etcData[US::US_Etc_Dungeon_Play_Time]	 )
		<< BUILD_LOG( dungeon[US::US_Etc_Duration_Time] )
		<< BUILD_LOG( etcData[US::US_Etc_PVP_Play_Time] )
		<< BUILD_LOG( dungeonSiglePlayCount )
		<< BUILD_LOG( dungeonParyPlayCount )
		<< BUILD_LOG( dungeon[US::US_D_MC_Win] )
		<< BUILD_LOG( dungeon[US::US_D_MC_Party_Win] )
		<< BUILD_LOG( dungeonSiglePlayPT )
		<< BUILD_LOG( dungeonParyPlayPT	 )
		<< BUILD_LOG( etcData[US::US_Etc_Pvp_Kill_Count] )
		<< BUILD_LOG( pvpWinCount )
		<< BUILD_LOG( pvpLoseCount )
		<< BUILD_LOG( trainingcenterPT )
		<< BUILD_LOG( trainingcenterCount )
		<< BUILD_LOG( etcData[US::US_Etc_TradeCount]	 )
		<< BUILD_LOG( etcData[US::US_Etc_Square_Count] )
		<< BUILD_LOG( etcData[US::US_Etc_Square_PlayTime]	)
		<< BUILD_LOG( etcData[US::US_Etc_Trade_Buy_Item]	 )
		<< BUILD_LOG( etcData[US::US_Etc_Trade_Sell_Item] )
		<< BUILD_LOG( etcData[US::US_Etc_Obtained_Item]	)
		<< BUILD_LOG( reStoneCount	)

#endif //SERV_USER_STATISTICS_RENEWAL
		<< END_LOG;
}

IMPL_ON_FUNC( ELOG_CHEAT_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_CheatLog_INS", L"%d, %d, %d, %d, N\'%s\', N\'%s\'",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% (int)kPacket_.m_cAuthLevel
		% kPacket_.m_wsCheatCode
		% kPacket_.m_wsCheatContents );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_cheat_log", L"%d, %d, %d, N\'%s\', N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% (int)kPacket_.m_cAuthLevel
		% kPacket_.m_wsCheatCode
		% kPacket_.m_wsCheatContents );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}

typedef KStatistics KS;

_IMPL_ON_FUNC( ELOG_STAT_ED, KStatisticsData )
{
	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); mit++ )
	{
		if( mit->first.m_vecIntKey.empty() )
		{
			START_LOG( cerr, L"통계키값이 비었음" )
				<< END_LOG;

			continue;
		}		
		__int64 aiCount[KStatistics::eSIColDB_ED_Max];
		std::vector< __int64 >::iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::eSIColDB_ED_Max; iIndex++ )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )					
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}

		int ieSIColDB_ED_MItemResolve = 0;

#ifdef SERV_MULTI_RESOLVE
		ieSIColDB_ED_MItemResolve = aiCount[KS::eSIColDB_ED_MItemResolve];		// @iMItemDecom		bigint
#endif //SERV_MULTI_RESOLVE

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsMoneySupply_MER",
			L"%d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID() 
			% aiCount[KS::eSIColDB_ED_PDungeon]			// @iPDun			bigint
			% aiCount[KS::eSIColDB_ED_PTraining]		// @iPTraining		bigint
			% aiCount[KS::eSIColDB_ED_PQuest]			// @iPQuest			bigint
			% aiCount[KS::eSIColDB_ED_PItemSell]		// @iPItemSell		bigint
			% aiCount[KS::eSIColDB_ED_PItemTrade]		// @iPItemP2P		bigint
			% aiCount[KS::eSIColDB_ED_PPersonalShop]	// @iPStoreP2P		bigint
			% aiCount[KS::eSIColDB_ED_PPostLetter]		// @iPPostP2P		bigint
			% aiCount[KS::eSIColDB_ED_MItemBuy]			// @iMItemBuy		bigint
			% aiCount[KS::eSIColDB_ED_MItemTrade]		// @iMItemP2P		bigint
			% aiCount[KS::eSIColDB_ED_MItemRepair]		// @iMItemRepair	bigint
			% aiCount[KS::eSIColDB_ED_MManufacture]		// @iMItemMake		bigint
			% aiCount[KS::eSIColDB_ED_MDunPenalty]		// @iMDunPenalty	bigint
			% aiCount[KS::eSIColDB_ED_MPersonalShop]	// @iMStoreP2P		bigint
			% aiCount[KS::eSIColDB_ED_MOpenPShop]		// @iMStoreOpen		bigint
			% aiCount[KS::eSIColDB_ED_MEnchant]			// @iMEnchant		bigint
			% aiCount[KS::eSIColDB_ED_MSocket]			// @iMSocket		bigint
			% aiCount[KS::eSIColDB_ED_MPostLetter]		// @iMPostP2P		bigint
			% aiCount[KS::eSIColDB_ED_MSendLetterBasic]	// @iMPostBasic		bigint
			% aiCount[KS::eSIColDB_ED_MSendLetterItem]	// @iMPostItem		bigint
			% aiCount[KS::eSIColDB_ED_MAttribEnchant]	// @iMAttribute		bigint
			% aiCount[KS::eSIColDB_ED_MSellTaxPShop]	// @iMStoreP2PTax	bigint
			//{{ 2010. 03. 31  최육사	거래게시판
			% aiCount[KS::eSIColDB_ED_MBuyTaxPShop]		// @iMStoreP2PTax	bigint
			//}}
			//{{ 2011. 05. 11	최육사	대리상인
//#ifdef SERV_PSHOP_AGENCY
			% aiCount[KS::eSIColDB_ED_MPShopAgency]
			% aiCount[KS::eSIColDB_ED_MBuyTaxPShopAgency]
			% aiCount[KS::eSIColDB_ED_PPShopPickUp]
//#endif SERV_PSHOP_AGENCY
			//}}
			//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
			% aiCount[KS::eSIColDB_ED_POpenCube]
//#endif SERV_CUBE_OPEN_ED_CONDITION
			//}
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
			% aiCount[KS::eSIColDB_ED_PBattleField]			// @iPBattleFieldED			bigint
			% aiCount[KS::eSIColDB_ED_MCoboExpressTicketED]	// @iMCoboExpressTicketED	bigint
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
//#ifdef SERV_MULTI_RESOLVE
			% ieSIColDB_ED_MItemResolve
//#endif SERV_MULTI_RESOLVE
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
			% aiCount[KS::eSIColDB_ED_MItemEvaluate]		// @iMItemEvaluate	bigint

//#ifdef SERV_GOOD_ELSWORD // 작업날짜 : 2013-11-08 // 우상혁
			% aiCount[KS::eSIColDB_ED_ExpandInventory]
			% aiCount[KS::eSIColDB_ED_ExpandBank]
//#endif //SERV_GOOD_ELSWORD

//#ifdef SERV_SKILL_PAGE_SYSTEM
			% aiCount[KS::eSIColDB_ED_ExpandSkillPage]
//#endif // SERV_SKILL_PAGE_SYSTEM

			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsMoneySupply_INS",
			L"%d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID() 
			% aiCount[KS::eSIColDB_ED_PDungeon]			// @iPDun			bigint
			% aiCount[KS::eSIColDB_ED_PTraining]		// @iPTraining		bigint
			% aiCount[KS::eSIColDB_ED_PQuest]			// @iPQuest			bigint
			% aiCount[KS::eSIColDB_ED_PItemSell]		// @iPItemSell		bigint
			% aiCount[KS::eSIColDB_ED_PItemTrade]		// @iPItemP2P		bigint
			% aiCount[KS::eSIColDB_ED_PPersonalShop]	// @iPStoreP2P		bigint
			% aiCount[KS::eSIColDB_ED_PPostLetter]		// @iPPostP2P		bigint
			% aiCount[KS::eSIColDB_ED_MItemBuy]			// @iMItemBuy		bigint
			% aiCount[KS::eSIColDB_ED_MItemTrade]		// @iMItemP2P		bigint
			% aiCount[KS::eSIColDB_ED_MItemRepair]		// @iMItemRepair	bigint
			% aiCount[KS::eSIColDB_ED_MManufacture]		// @iMItemMake		bigint
			% aiCount[KS::eSIColDB_ED_MDunPenalty]		// @iMDunPenalty	bigint
			% aiCount[KS::eSIColDB_ED_MPersonalShop]	// @iMStoreP2P		bigint
			% aiCount[KS::eSIColDB_ED_MOpenPShop]		// @iMStoreOpen		bigint
			% aiCount[KS::eSIColDB_ED_MEnchant]			// @iMEnchant		bigint
			% aiCount[KS::eSIColDB_ED_MSocket]			// @iMSocket		bigint
			% aiCount[KS::eSIColDB_ED_MPostLetter]		// @iMPostP2P		bigint
			% aiCount[KS::eSIColDB_ED_MSendLetterBasic]	// @iMPostBasic		bigint
			% aiCount[KS::eSIColDB_ED_MSendLetterItem]	// @iMPostItem		bigint
			% aiCount[KS::eSIColDB_ED_MAttribEnchant]	// @iMAttribute		bigint
			% aiCount[KS::eSIColDB_ED_MSellTaxPShop]	// @iMStoreP2PTax	bigint
			//{{ 2010. 03. 31  최육사	거래게시판
			% aiCount[KS::eSIColDB_ED_MBuyTaxPShop]		// @iMStoreP2PTax	bigint
			//}}
			//{{ 2011. 05. 11	최육사	대리상인
//#ifdef SERV_PSHOP_AGENCY
			% aiCount[KS::eSIColDB_ED_MPShopAgency]
			% aiCount[KS::eSIColDB_ED_MBuyTaxPShopAgency]
			% aiCount[KS::eSIColDB_ED_PPShopPickUp]
//#endif SERV_PSHOP_AGENCY
			//}}
			//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
			% aiCount[KS::eSIColDB_ED_POpenCube]
//#endif SERV_CUBE_OPEN_ED_CONDITION
			//}
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
			% aiCount[KS::eSIColDB_ED_PBattleField]			// @iPBattleFieldED			bigint
			% aiCount[KS::eSIColDB_ED_MCoboExpressTicketED]	// @iMCoboExpressTicketED	bigint
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
//#ifdef SERV_MULTI_RESOLVE
			% ieSIColDB_ED_MItemResolve
//#endif SERV_MULTI_RESOLVE
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
			% aiCount[KS::eSIColDB_ED_MItemEvaluate]		// @iMItemEvaluate	bigint
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
//#ifdef SERV_GOOD_ELSWORD // 작업날짜 : 2013-11-08 // 우상혁
            % aiCount[KS::eSIColDB_ED_ExpandInventory]
            % aiCount[KS::eSIColDB_ED_ExpandBank]
//#endif //SERV_GOOD_ELSWORD
//#ifdef SERV_SKILL_PAGE_SYSTEM
			% aiCount[KS::eSIColDB_ED_ExpandSkillPage]
//#endif // SERV_SKILL_PAGE_SYSTEM
			);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsMoneySupplyUpdate",
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
            L"%d, %d",
			% aiCount[KS::eSIColDB_ED_PDungeon]			// @iPDun			bigint
			% aiCount[KS::eSIColDB_ED_PTraining]		// @iPTraining		bigint
			% aiCount[KS::eSIColDB_ED_PQuest]			// @iPQuest			bigint
			% aiCount[KS::eSIColDB_ED_PItemSell]		// @iPItemSell		bigint
			% aiCount[KS::eSIColDB_ED_PItemTrade]		// @iPItemP2P		bigint
			% aiCount[KS::eSIColDB_ED_PPersonalShop]	// @iPStoreP2P		bigint
			% aiCount[KS::eSIColDB_ED_PPostLetter]		// @iPPostP2P		bigint
			% aiCount[KS::eSIColDB_ED_MItemBuy]			// @iMItemBuy		bigint
			% aiCount[KS::eSIColDB_ED_MItemTrade]		// @iMItemP2P		bigint
			% aiCount[KS::eSIColDB_ED_MItemRepair]		// @iMItemRepair	bigint
			% aiCount[KS::eSIColDB_ED_MManufacture]		// @iMItemMake		bigint
			% aiCount[KS::eSIColDB_ED_MDunPenalty]		// @iMDunPenalty	bigint
			% aiCount[KS::eSIColDB_ED_MPersonalShop]	// @iMStoreP2P		bigint
			% aiCount[KS::eSIColDB_ED_MOpenPShop]		// @iMStoreOpen		bigint
			% aiCount[KS::eSIColDB_ED_MEnchant]			// @iMEnchant		bigint
			% aiCount[KS::eSIColDB_ED_MSocket]			// @iMSocket		bigint
			% aiCount[KS::eSIColDB_ED_MPostLetter]		// @iMPostP2P		bigint
			% aiCount[KS::eSIColDB_ED_MSendLetterBasic]	// @iMPostBasic		bigint
			% aiCount[KS::eSIColDB_ED_MSendLetterItem]	// @iMPostItem		bigint
			% aiCount[KS::eSIColDB_ED_MAttribEnchant]	// @iMAttribute		bigint
			% aiCount[KS::eSIColDB_ED_MSellTaxPShop]	// @iMStoreP2PTax	bigint
			//{{ 2010. 03. 31  최육사	거래게시판
			% aiCount[KS::eSIColDB_ED_MBuyTaxPShop]		// @iMStoreP2PTax	bigint
			//}}
			//{{ 2011. 05. 11	최육사	대리상인
//#ifdef SERV_PSHOP_AGENCY
			% aiCount[KS::eSIColDB_ED_MPShopAgency]
			% aiCount[KS::eSIColDB_ED_MBuyTaxPShopAgency]
			% aiCount[KS::eSIColDB_ED_PPShopPickUp]
//#endif SERV_PSHOP_AGENCY
			//}}
			//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
			% aiCount[KS::eSIColDB_ED_POpenCube]
//#endif SERV_CUBE_OPEN_ED_CONDITION
			//}
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
			% aiCount[KS::eSIColDB_ED_PBattleField]			// @iPBattleFieldED			bigint
			% aiCount[KS::eSIColDB_ED_MCoboExpressTicketED]	// @iMCoboExpressTicketED	bigint
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
			% aiCount[KS::eSIColDB_ED_MItemEvaluate]		// @iMItemEvaluate	bigint
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
//#ifdef SERV_GOOD_ELSWORD // 작업날짜 : 2013-11-08 // 우상혁
            % aiCount[KS::eSIColDB_ED_ExpandInventory]
            % aiCount[KS::eSIColDB_ED_ExpandBank]
//#endif //SERV_GOOD_ELSWORD

//#ifdef SERV_SKILL_PAGE_SYSTEM
			% aiCount[KS::eSIColDB_ED_ExpandSkillPage]
//#endif // SERV_SKILL_PAGE_SYSTEM

			);
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );

				m_kODBC.EndFetch();
			}

		START_LOG( clog2, L"ED 통계 [검사]" )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_PDungeon] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_PTraining] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_PQuest] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_PItemSell] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_PItemTrade] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_PPersonalShop] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_PPostLetter] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MItemBuy] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MItemTrade] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MItemRepair] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MManufacture] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MDunPenalty] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MPersonalShop] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MOpenPShop] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MEnchant] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MSocket] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MPostLetter] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MSendLetterBasic] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MSendLetterItem] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MAttribEnchant] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MSellTaxPShop] )
			//{{ 2010. 03. 31  최육사	거래게시판
			<< BUILD_LOG( aiCount[KStatistics::eSIColDB_ED_MBuyTaxPShop] )
			//}}
			//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_MPShopAgency] )
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_MBuyTaxPShopAgency] )
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_PPShopPickUp] )
#endif SERV_PSHOP_AGENCY
			//}}
			//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_POpenCube] )
#endif SERV_CUBE_OPEN_ED_CONDITION
			//}
#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_PBattleField] )
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_MCoboExpressTicketED] )
#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_MItemEvaluate] )
#endif // SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef SERV_MULTI_RESOLVE
			<< BUILD_LOG( ieSIColDB_ED_MItemResolve )				// 아이템 분해 수수료 
#endif SERV_MULTI_RESOLVE
#ifdef SERV_GOOD_ELSWORD // 작업날짜 : 2013-11-08 // 우상혁
            << BUILD_LOG( aiCount[KS::eSIColDB_ED_ExpandInventory] )
            << BUILD_LOG( aiCount[KS::eSIColDB_ED_ExpandBank] )
#endif //SERV_GOOD_ELSWORD
//#ifdef SERV_SKILL_PAGE_SYSTEM
			<< BUILD_LOG( aiCount[KS::eSIColDB_ED_ExpandSkillPage] )
//#endif // SERV_SKILL_PAGE_SYSTEM

			<< BUILD_LOG( iOK );
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;	
}

_IMPL_ON_FUNC( ELOG_STAT_QUEST, KStatisticsData )
{
	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); mit++ )
	{
		if( mit->first.m_vecIntKey.empty() )
		{
			START_LOG( cerr, L"통계키값이 비었음" )
				<< END_LOG;

			continue;
		}

		int iQuestID = mit->first.m_vecIntKey[0];
		__int64 aiCount[KStatistics::eSIColQuest_Max];
		std::vector< __int64 >::iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::eSIColQuest_Max; iIndex++ )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )
					<< BUILD_LOG( iQuestID )
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LQuestInfo_MER", L"%d, %d, %d, %d, %d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iQuestID											// @iQuestID		[bigint]
			% aiCount[KStatistics::eSIColQuest_Accept]			// @iAccept			[smallint]
			% aiCount[KStatistics::eSIColQuest_GiveUp]			// @iGiveUp			[smallint]
			% aiCount[KStatistics::eSIColQuest_Complete]		// @iComplete		[smallint]
			% aiCount[KStatistics::eSIColQuest_AcceptLevel]		// @iAcceptLevel	[smallint]
			% aiCount[KStatistics::eSIColQuest_GiveUpLevel]		// @iGiveUpLevel	[smallint]
			% aiCount[KStatistics::eSIColQuest_CompleteLevel]	// @iCompleteLevel  [smallint]
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsQuestInfo_SET", L"%d, %d, %d, %d, %d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iQuestID											// @iQuestID		[bigint]
			% aiCount[KStatistics::eSIColQuest_Accept]			// @iAccept			[smallint]
			% aiCount[KStatistics::eSIColQuest_GiveUp]			// @iGiveUp			[smallint]
			% aiCount[KStatistics::eSIColQuest_Complete]		// @iComplete		[smallint]
			% aiCount[KStatistics::eSIColQuest_AcceptLevel]		// @iAcceptLevel	[smallint]
			% aiCount[KStatistics::eSIColQuest_GiveUpLevel]		// @iGiveUpLevel	[smallint]
			% aiCount[KStatistics::eSIColQuest_CompleteLevel]	// @iCompleteLevel  [smallint]
			);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsQuestInfoUpdate", L"%d, %d, %d, %d, %d, %d, %d",
			% iQuestID											// @iQuestID		[bigint]
			% aiCount[KStatistics::eSIColQuest_Accept]			// @iAccept			[smallint]
			% aiCount[KStatistics::eSIColQuest_GiveUp]			// @iGiveUp			[smallint]
			% aiCount[KStatistics::eSIColQuest_Complete]		// @iComplete		[smallint]
			% aiCount[KStatistics::eSIColQuest_AcceptLevel]		// @iAcceptLevel	[smallint]
			% aiCount[KStatistics::eSIColQuest_GiveUpLevel]		// @iGiveUpLevel	[smallint]
			% aiCount[KStatistics::eSIColQuest_CompleteLevel]	// @iCompleteLevel  [smallint]
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		START_LOG( clog2, L"FPS 통계 [검사]" )
			<< BUILD_LOG( iQuestID )
			<< BUILD_LOG( aiCount[KStatistics::eSIColQuest_Accept] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColQuest_GiveUp] )			
			<< BUILD_LOG( aiCount[KStatistics::eSIColQuest_Complete] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColQuest_AcceptLevel] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColQuest_GiveUpLevel]	)
			<< BUILD_LOG( aiCount[KStatistics::eSIColQuest_CompleteLevel] )
			<< BUILD_LOG( iOK );
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;	
}

//{{ 2011. 02. 28	최육사	사내 통합 통계
#ifdef SERV_CHAR_CREATE_DEL_STAT

_IMPL_ON_FUNC( ELOG_STAT_CHARACTER, KStatisticsData )
{
	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::const_iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit )
	{
		const std::vector< int >& vecIntKey = mit->first.m_vecIntKey;
		if( vecIntKey.empty() )
		{
			START_LOG( cerr, L"통계키값이 비었음" )
				<< END_LOG;
			continue;
		}

		__int64 aiCount[KStatistics::eSIColDB_Num];
		std::vector< __int64 >::const_iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::eSIColDB_Num; ++iIndex )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsDaily_MER", L"%d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% aiCount[KStatistics::eSIColDB_Char_Create_Count]		// @iNewUnit		bigint
			% aiCount[KStatistics::eSIColDB_Char_Delete_Count]		// @iDelUnit		bigint
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsDaily_SET", L"%d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% aiCount[KStatistics::eSIColDB_Char_Create_Count]		// @iNewUnit		bigint
			% aiCount[KStatistics::eSIColDB_Char_Delete_Count]		// @iDelUnit		bigint
			);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.StatsDailyUpdate", L"%d, %d",
			% aiCount[KStatistics::eSIColDB_Char_Create_Count]		// @iNewUnit		bigint
			% aiCount[KStatistics::eSIColDB_Char_Delete_Count]		// @iDelUnit		bigint
			);
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );

				m_kODBC.EndFetch();
			}

			START_LOG( clog2, L"캐릭터 통계 [검사]" )
				<< BUILD_LOG( aiCount[KStatistics::eSIColDB_Char_Create_Count] )
				<< BUILD_LOG( aiCount[KStatistics::eSIColDB_Char_Delete_Count] )
				<< BUILD_LOG( iOK );
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;	
}

#endif SERV_CHAR_CREATE_DEL_STAT
//}}

_IMPL_ON_FUNC( ELOG_STAT_DISCONNECT_USERFSM, KStatisticsData )
{
	//////////////////////////////////////////////////////////////////////////
	// 이거 계속 남겨야할 필요가 있을까? 볼 일이 없는데.. 삭제할지 말지 결정하자..
	//////////////////////////////////////////////////////////////////////////	

	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); mit++ )
	{
		if( mit->first.m_vecIntKey.empty() )
		{
			START_LOG( cerr, L"통계키값이 비었음" )
				<< END_LOG;

			continue;
		}
		
		int iServerUID = mit->first.m_vecIntKey[0];
		__int64 aiCount[KStatistics::eSIColDU_Num];
		std::vector< __int64 >::iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::eSIColDU_Num; iIndex++ )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )					
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}
				
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LDisconnectUserFSM_INS",
			L"%d,"
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iServerUID
			% aiCount[KStatistics::eSIColDU_S_INIT]
			% aiCount[KStatistics::eSIColDU_S_UID_UPDATED]
			% aiCount[KStatistics::eSIColDU_S_LOGINED]
			% aiCount[KStatistics::eSIColDU_S_SERVER_SELECT]
			% aiCount[KStatistics::eSIColDU_S_UNIT_SELECT]
			% aiCount[KStatistics::eSIColDU_S_VILLAGE_MAP]
			% aiCount[KStatistics::eSIColDU_S_LOCAL_MAP]
			% aiCount[KStatistics::eSIColDU_S_WORLD_MAP]
			% aiCount[KStatistics::eSIColDU_S_CHANNEL]
			% aiCount[KStatistics::eSIColDU_S_ROOM]
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_disconnect_userfsm",
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d",
			% iServerUID
			% aiCount[KStatistics::eSIColDU_S_INIT]
			% aiCount[KStatistics::eSIColDU_S_UID_UPDATED]
			% aiCount[KStatistics::eSIColDU_S_LOGINED]
			% aiCount[KStatistics::eSIColDU_S_SERVER_SELECT]
			% aiCount[KStatistics::eSIColDU_S_UNIT_SELECT]
			% aiCount[KStatistics::eSIColDU_S_VILLAGE_MAP]
			% aiCount[KStatistics::eSIColDU_S_LOCAL_MAP]
			% aiCount[KStatistics::eSIColDU_S_WORLD_MAP]
			% aiCount[KStatistics::eSIColDU_S_CHANNEL]
			% aiCount[KStatistics::eSIColDU_S_ROOM]
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		START_LOG( clog2, L"Disconnect userfsm 통계" )
			<< BUILD_LOG( iServerUID )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_INIT] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_UID_UPDATED] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_LOGINED] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_SERVER_SELECT] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_UNIT_SELECT] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_VILLAGE_MAP] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_LOCAL_MAP] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_WORLD_MAP] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_CHANNEL] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDU_S_ROOM] )
			<< BUILD_LOG( iOK );
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;
}

_IMPL_ON_FUNC( ELOG_STAT_DISCONNECT_REASON, KStatisticsData )
{
	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit )
	{
		if( mit->first.m_vecIntKey.empty() )
		{
			START_LOG( cerr, L"통계키값이 비었음" )
				<< END_LOG;

			continue;
		}

		int iServerUID = mit->first.m_vecIntKey[0];
		__int64 aiCount[KStatistics::eSIColDR_Num];
		std::vector< __int64 >::iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::eSIColDR_Num; ++iIndex )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LDisconnectReason_INS",
			L"%d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, "
			//{{ 2011. 09. 29  김민성	셧다운제 도입
			//#ifdef SERV_SHUTDOWN_SYSTEM   
			L"%d",
			//#endif SERV_SHUTDOWN_SYSTEM
			//}}
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iServerUID
			% aiCount[0]	// [0]  이유 없음
			% aiCount[1]	// [1]  Unknown
			% aiCount[2]	// [2]  HackShield 인증 실패 (클라이언트 응답 없음)
			% aiCount[3]	// [3]  PC방 인증 지연
			% aiCount[4]	// [4]  메인버전 다름
			% aiCount[5]	// [5]  수용 인원 초과
			% aiCount[6]	// [6]  넥슨 인증 실패
			% aiCount[7]	// [7]  동일 서버 이중 접속
			% aiCount[8]	// [8]  계정 블록
			% aiCount[9]	// [9]  IP 제한
			% aiCount[10]	// [10] NProtect 처음 인증
			% aiCount[11]	// [11] PC방 인증 실패
			% aiCount[12]	// [12] PC방 인증 결과 이상
			% aiCount[13]	// [13] 사내 계정 생성
			% aiCount[14]	// [14] 정상 종료
			% aiCount[15]	// [15] 클라이언트 크래쉬
			% aiCount[16]	// [16] 클라이언트 해킹
			% aiCount[17]	// [17] HackShield 인증 실패 (응답 틀림)
			% aiCount[18]	// [18] PC방 인증 서버의 종료 요청
			% aiCount[19]	// [19] 서버 Shutdown
			% aiCount[20]	// [20] 접속 종료 감지 (Session)
			% aiCount[21]	// [21] WSAECONNRESET (Socket Object)
			% aiCount[22]	// [22] WSARecvFailed (Socket Object)
			% aiCount[23]	// [23] WSASendFailed (Socket Object) - OnSendCompleted()
			% aiCount[24]	// [24] SendBufferFull (Socket Object)
			% aiCount[25]	// [25] WSASendFailed (Socket Object) - SendData()
			% aiCount[26]	// [26] 운영자 강제 접속 종료
			% aiCount[27]	// [27] 타 서버간 이중접속
			% aiCount[28]	// [28] 유저 등록 중 에러
			% aiCount[29]	// [29] 하트 비트
			% aiCount[30]	// [30] IOCP_GetStatus Closed by local machine
			% aiCount[31]	// [31] nProtect 해킹 감지
			% aiCount[32]	// [32] 채널이동으로 인한 접속종료
			% aiCount[33]	// [33] 채널이동 실패로 인한 접속종료
			% aiCount[34]	// [34] 트래픽 유발로 인한 접속종료
			% aiCount[35]	// [35] 게임서버 계정 블록
			//{{ 2011. 08. 09	최육사	머신ID 블럭기능
			//#ifdef SERV_MACHINE_ID_BLOCK
			% aiCount[36]	// [36] 머신ID 블럭
			//#endif SERV_MACHINE_ID_BLOCK
			//}}
			//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
			//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			% aiCount[37]	// [37] 머신ID 중복 접속
			//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
			//{{ 2011. 09. 29  김민성	셧다운제 도입
			//#ifdef SERV_SHUTDOWN_SYSTEM
			% aiCount[38]	// [38] 셧다운제로 인한 접속 불가
			//#endif SERV_SHUTDOWN_SYSTEM
			//}}
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_disconnect_reason",
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, "
	//{{ 2011. 09. 29  김민성	셧다운제 도입
//#ifdef SERV_SHUTDOWN_SYSTEM   
			L"%d",
//#endif SERV_SHUTDOWN_SYSTEM
	//}}
			% iServerUID
			% aiCount[0]	// [0]  이유 없음
			% aiCount[1]	// [1]  Unknown
			% aiCount[2]	// [2]  HackShield 인증 실패 (클라이언트 응답 없음)
			% aiCount[3]	// [3]  PC방 인증 지연
			% aiCount[4]	// [4]  메인버전 다름
			% aiCount[5]	// [5]  수용 인원 초과
			% aiCount[6]	// [6]  넥슨 인증 실패
			% aiCount[7]	// [7]  동일 서버 이중 접속
			% aiCount[8]	// [8]  계정 블록
			% aiCount[9]	// [9]  IP 제한
			% aiCount[10]	// [10] NProtect 처음 인증
			% aiCount[11]	// [11] PC방 인증 실패
			% aiCount[12]	// [12] PC방 인증 결과 이상
			% aiCount[13]	// [13] 사내 계정 생성
			% aiCount[14]	// [14] 정상 종료
			% aiCount[15]	// [15] 클라이언트 크래쉬
			% aiCount[16]	// [16] 클라이언트 해킹
			% aiCount[17]	// [17] HackShield 인증 실패 (응답 틀림)
			% aiCount[18]	// [18] PC방 인증 서버의 종료 요청
			% aiCount[19]	// [19] 서버 Shutdown
			% aiCount[20]	// [20] 접속 종료 감지 (Session)
			% aiCount[21]	// [21] WSAECONNRESET (Socket Object)
			% aiCount[22]	// [22] WSARecvFailed (Socket Object)
			% aiCount[23]	// [23] WSASendFailed (Socket Object) - OnSendCompleted()
			% aiCount[24]	// [24] SendBufferFull (Socket Object)
			% aiCount[25]	// [25] WSASendFailed (Socket Object) - SendData()
			% aiCount[26]	// [26] 운영자 강제 접속 종료
			% aiCount[27]	// [27] 타 서버간 이중접속
			% aiCount[28]	// [28] 유저 등록 중 에러
			% aiCount[29]	// [29] 하트 비트
			% aiCount[30]	// [30] IOCP_GetStatus Closed by local machine
			% aiCount[31]	// [31] nProtect 해킹 감지
			% aiCount[32]	// [32] 채널이동으로 인한 접속종료
			% aiCount[33]	// [33] 채널이동 실패로 인한 접속종료
			% aiCount[34]	// [34] 트래픽 유발로 인한 접속종료
			% aiCount[35]	// [35] 게임서버 계정 블록
			//{{ 2011. 08. 09	최육사	머신ID 블럭기능
//#ifdef SERV_MACHINE_ID_BLOCK
			% aiCount[36]	// [36] 머신ID 블럭
//#endif SERV_MACHINE_ID_BLOCK
			//}}
			//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			% aiCount[37]	// [37] 머신ID 중복 접속
//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
			//{{ 2011. 09. 29  김민성	셧다운제 도입
//#ifdef SERV_SHUTDOWN_SYSTEM
			% aiCount[38]	// [38] 셧다운제로 인한 접속 불가
//#endif SERV_SHUTDOWN_SYSTEM
			//}}
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		START_LOG( clog2, L"Disconnect reason 통계" )
			<< BUILD_LOG( iServerUID )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_No_Reason] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Unknown] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_HackShield_Auth_Fail] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_PCBang_Auth_Delay] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Diff_Main_Ver] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Limit_User_Num] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Nexon_Auth_Fail] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Same_Server_Connect] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Account_Block] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_IP_Block] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_NProtect_Auth_First] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_PCBang_Auth_Fail] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_PCBang_Auth_Wrong] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Internal_Account] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Normal_Disconnect] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Client_Crash] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Client_Hacking] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_HackShield_Wrong_Ack] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_PCBang_Auth_Server] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Server_Shutdown] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Session_Close] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_WSAECONNRESET] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_WSARecvFailed] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_WSASendFailed_OnSendComp] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_SendBufferFull] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_WSASendFailed] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_GM_Kick_User] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Diff_Server_Connect] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_User_Reg_Error] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Heart_Beat] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_IOCP_GetStatus] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_nProtect_Hacking] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Channel_Change] )
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Channel_Change_Fail] )
			//{{ 2009. 10. 22  최육사	트래픽 공격 방지
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Traffic_Attack] )
			//}}
			//{{ 2010. 06. 22  최육사	계정블럭기능
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_GameServer_Account_Block] )
			//}}
			//{{ 2011. 08. 09	최육사	머신ID 블럭기능
//#ifdef SERV_MACHINE_ID_BLOCK
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_MachineID_Block] )
//#endif SERV_MACHINE_ID_BLOCK
			//}}
			//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
//#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Same_MachineID_Connect] )
//#endif SERV_MACHINE_ID_DUPLICATE_CHECK
			//}}
			//{{ 2011. 09. 29  김민성	셧다운제 도입
//#ifdef SERV_SHUTDOWN_SYSTEM
			<< BUILD_LOG( aiCount[KStatistics::eSIColDR_Policy_Shutdown_Disconnect] )
//#endif SERV_SHUTDOWN_SYSTEM
			//}}
			<< BUILD_LOG( iOK );

		//{{ 2012. 01. 25  김민성	종료 통계 개편
#ifdef SERV_DISCONNECTION_STATISTICS_LOG
		CTime tCurr = CTime::GetCurrentTime();
		std::wstring wstrCurTime = (const wchar_t*)tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
		for( int i = 0 ; i < KStatistics::eSIColDR_Num ; ++i )
		{
			if( aiCount[i] > 0 )
			{
#ifdef SERV_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_LDisconnectReason_New_INS", L"%d, %d, N\'%s\', %d, %d", 
					% KBaseServer::GetKObj()->GetServerGroupID() 
					% iServerUID
					% wstrCurTime
					% i
					% aiCount[i] );
#else //SERV_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_LDisconnectReason_New_INT", L"%d, N\'%s\', %d, %d",
					% iServerUID
					% wstrCurTime
					% i
					% aiCount[i] );
#endif //SERV_RENEWAL_SP

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				START_LOG( clog2, L"Disconnect reason 통계 개편" )
					<< BUILD_LOG( iServerUID )
					<< BUILD_LOG( wstrCurTime )
					<< BUILD_LOG( i )
					<< BUILD_LOG( aiCount[i] )
					<< BUILD_LOG( iOK );
			}
		}
#endif SERV_DISCONNECTION_STATISTICS_LOG
		//}}
		
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;
}

_IMPL_ON_FUNC( DBE_DELETE_UNIT_STAT_NOT, KEGS_DELETE_UNIT_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_UnitStatistics_UPD_Deleted", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_UnitStatistics_Delete_SET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsUpdateDelUnit", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;	
}

IMPL_ON_FUNC( DBE_UPDATE_UNIT_CLASS_NOT )
{
    int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_UnitStatistics_UPD_UnitClass", L"%d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID 
		% ( int )kPacket_.m_cUnitClass 
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_UnitStatistics_UnitClass_UPD", L"%d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID 
		% ( int )kPacket_.m_cUnitClass 
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_update_unit_class", L"%d, %d", 
		% kPacket_.m_iUnitUID 
		% ( int )kPacket_.m_cUnitClass 
		);
#endif //SERV_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );

        m_kODBC.EndFetch();
    }

end_proc:
    LOG_SUCCESS( iOK == NetError::NET_OK )
        << BUILD_LOG( iOK )
        << END_LOG;	
}

IMPL_ON_FUNC( ELOG_RESURRECTION_STONE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ResurrectionStoneLog_INS", L"%d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iDungeonID 
		% kPacket_.m_wstrRegDate 
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_stone_Log", L"%d, %d, N\'%s\'", 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iDungeonID 
		% kPacket_.m_wstrRegDate 
		);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_UnitUID )
		<< BUILD_LOG( kPacket_.m_iDungeonID )
		<< BUILD_LOG( kPacket_.m_wstrRegDate )
		<< END_LOG;
}

IMPL_ON_FUNC( ELOG_RESOLVE_JACKPOT_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsJackPot_INS", L"%d, %d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iResolveItemID 
		% kPacket_.m_iElapsedTime 
		% kPacket_.m_wstrRegDate 
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_jackpot_log", L"%d, %d, %d, N\'%s\'", 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iResolveItemID 
		% kPacket_.m_iElapsedTime 
		% kPacket_.m_wstrRegDate 
		);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_UnitUID )
		<< BUILD_LOG( kPacket_.m_iResolveItemID )
		<< BUILD_LOG( kPacket_.m_iElapsedTime )
		<< BUILD_LOG( kPacket_.m_wstrRegDate )
		<< END_LOG;
}

IMPL_ON_FUNC( ELOG_GET_PURCHASED_CASH_ITEM_NOT )
{
    int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsCashLog_INS", L"%d, %d, %d, %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_sPeriod
		% kPacket_.m_iQuantity
		% (int)kPacket_.m_ucLevel
		% (int)kPacket_.m_cUnitClass
		);
#else //SERV_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_insert_cash_log", L"%d, %d, %d, %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_sPeriod
		% kPacket_.m_iQuantity
		% (int)kPacket_.m_ucLevel
		% (int)kPacket_.m_cUnitClass
		);
#endif //SERV_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );

        m_kODBC.EndFetch();
    }

end_proc:
    LOG_SUCCESS( iOK == NetError::NET_OK )
        << BUILD_LOG( iOK )
        << BUILD_LOG( kPacket_.m_iUnitUID )
        << BUILD_LOG( kPacket_.m_iItemID )
        << BUILD_LOG( kPacket_.m_sPeriod )
        << BUILD_LOG( kPacket_.m_iQuantity )
		<< BUILD_LOGc( kPacket_.m_ucLevel )
		<< BUILD_LOGc( kPacket_.m_cUnitClass )
        << END_LOG;
}

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

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
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
#endif //SERV_ADD_EVENT_DB
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC_NOPARAM( DBE_RELEASE_TICK_UPDATE_REQ )
{
	int								releaseTick = -1;
	KDBE_RELEASE_TICK_UPDATE_ACK	AckPacket;
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
		AckPacket.m_mapReleaseTick.insert( std::make_pair( iReleaseTickType, releaseTick ) );
	}

end_proc:
	SendToServer( DBE_RELEASE_TICK_UPDATE_ACK, AckPacket );
}

IMPL_ON_FUNC_NOPARAM( DBE_EVENT_DATA_UPDATE_REQ )
{
	KDBE_EVENT_DATA_UPDATE_ACK				AckPacket;
	std::pair< int, int >					pairMinMaxLevel;
	EVENT_DATA								eventdata;
	std::map< int, EVENT_DATA >::iterator	mit;

	// Event Data 얻기
	DO_QUERY_NO_ARG( L"exec dbo.P_EventInfo_New_GET" );

	while( m_kODBC.Fetch() )
	{
		int iEventID = 0;
		int iEXPRate = 0;
		int iVPRate = 0;
		int iEDRate = 0;
		int iEventTime = 0;

		FETCH_DATA( iEventID
			>> iEXPRate
			>> iVPRate
			>> iEDRate
			>> eventdata.m_iDungeonID
			>> eventdata.m_iDungeonMode
			>> eventdata.m_iUnitLevel
			>> eventdata.m_cUnitType
			>> eventdata.m_cUnitClass
			>> eventdata.m_iPartyNum
			>> eventdata.m_fEventTime
			>> eventdata.m_iEventReward
			>> pairMinMaxLevel.first
			>> pairMinMaxLevel.second
			>> eventdata.m_bGiveOnce
			>> eventdata.m_bDelEndEvent
			>> eventdata.m_bPcBangEvent
			>> eventdata.m_bDirectReward
			>> eventdata.m_bAccountEvent
			>> eventdata.m_iDropCount
			>> eventdata.m_bWithPlayPcBang
			>> eventdata.m_iChannelCode
			>> eventdata.m_iBF_Team
			>> eventdata.m_iHenirRewardEventCount
			>> eventdata.m_bHenirRewardUnLimited
			>> eventdata.m_iServerGroup
			);

		eventdata.m_fEXPRate	= static_cast<float>( iEXPRate	* 0.00001f );
		eventdata.m_fVPRate		= static_cast<float>( iVPRate	* 0.00001f );
		eventdata.m_fEDRate		= static_cast<float>( iEDRate	* 0.00001f );
		eventdata.m_fEventTime	= static_cast<float>( iEventTime* 0.00001f );

		//{{ 2011. 05. 31	최육사	접속 이벤트 레벨 체크
#ifdef SERV_CONNECT_EVENT_LEVEL_CHECK
#else
		if( AckPacket.m_mapEventData.find( iEventID ) != AckPacket.m_mapEventData.end() )
		{
			START_LOG( cerr, L"같은 아이디의 보상이벤트 스크립트.!" )
				<< BUILD_LOG( iID )
				<< END_LOG;
			continue;
		}
#endif SERV_CONNECT_EVENT_LEVEL_CHECK
		//}}

		// 최소 레벨, 최대 레벨 정보가 하나라도 있으면 저장!
		if( pairMinMaxLevel.first != 0  ||  pairMinMaxLevel.second != 0 )
		{
			mit = AckPacket.m_mapEventData.find( iEventID );
			if( mit != AckPacket.m_mapEventData.end() )
			{
				mit->second.m_mapMinMaxReward.insert( std::make_pair( pairMinMaxLevel, eventdata.m_iEventReward ) );
				START_LOG( clog, L"이벤트 보상 그룹 설정!" )
					<< BUILD_LOG( iEventID )
					<< BUILD_LOG( mit->second.m_mapMinMaxReward.size() );
				continue;
			}
			else
			{
				eventdata.m_mapMinMaxReward.insert( std::make_pair( pairMinMaxLevel, eventdata.m_iEventReward ) );
			}
		}
		AckPacket.m_mapEventData.insert( std::make_pair( iEventID, eventdata ) );
	}

end_proc:
	SendToServer( DBE_EVENT_DATA_UPDATE_ACK, AckPacket );
}

IMPL_ON_FUNC_NOPARAM( DBE_EVENT_MONSTER_UPDATE_REQ )
{
	KDBE_EVENT_MONSTER_UPDATE_ACK						AckPacket;
	EVENT_MONSTER_DATA									structMonsterData;
	std::map< int, EVENT_MONSTER_DATA >::iterator		it;

	// Event Monster 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Event_Monster_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_Event_Monster_GET" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		int iNpcID = 0;
		int iRate = 0;
		int	iAdventDungeon = 0;
		int	iNoAdventDungeon = 0;
		int	iNoAdventStage = -1;

		FETCH_DATA( structMonsterData.m_iEventID
			>> structMonsterData.m_iMonsterDropCount
			>> structMonsterData.m_bIsAdventInBossStage
			>> iNpcID
			>> iRate
			>> iAdventDungeon
			>> iNoAdventDungeon
			>> iNoAdventStage
			);

		it = AckPacket.m_mapEventMonster.find( structMonsterData.m_iEventID );
		if( it == AckPacket.m_mapEventMonster.end() )
		{
			it = AckPacket.m_mapEventMonster.insert( std::make_pair( structMonsterData.m_iEventID, structMonsterData ) ).first;	
		}

		if( 0 < iNpcID )
		{
			it->second.m_mapNpcIDAndRate.insert( std::make_pair( iNpcID, iRate * 0.00001f ) );
		}
		if( 0 < iAdventDungeon )
		{
			it->second.m_setAdventDungeon.insert( iAdventDungeon );
		}
		if( 0 < iNoAdventDungeon )
		{
			it->second.m_setNoAdventDungeon.insert( iNoAdventDungeon );
		}
		if( 0 <= iNoAdventStage )
		{
			it->second.m_setNoAdventStage.insert( iNoAdventStage );
		}
	}

end_proc:
	SendToServer( DBE_EVENT_MONSTER_UPDATE_ACK, AckPacket );
}

IMPL_ON_FUNC_NOPARAM( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_REQ )
{
	KDBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK AckPacket;
	int	iServerGroupID;
	int iServerUID;

	// Event Notify Msg Server Info 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Event_ServerInfo_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_Event_ServerInfo_GET" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iServerGroupID
			>> iServerUID
			);

		AckPacket.m_vecEventNotifyMsgServerInfo.push_back( std::make_pair( iServerGroupID, iServerUID ) );
	}

end_proc:
	SendToServer( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK, AckPacket );
}

IMPL_ON_FUNC_NOPARAM( DBE_EVENT_REWARD_DATA_UPDATE_REQ )
{
	KDBE_EVENT_REWARD_DATA_UPDATE_ACK	AckPacket;
	int									iRewardID;
	KRewardData							temp;

	// Event Reward Data 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Event_Reward_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_Event_Reward_GET" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iRewardID
			>> temp.m_iRewardItemID
			>> temp.m_iQuantity
			>> temp.m_sPeriod
			);

		if( iRewardID <= 0  ||  temp.m_iRewardItemID <= 0  ||  temp.m_iQuantity <= 0  ||  temp.m_sPeriod < 0 )
		{
			START_LOG( cerr, L"잘못된 EVENT_REWARD_DATA 정보입니다." )
				<< BUILD_LOG( iRewardID )
				<< BUILD_LOG( temp.m_iRewardItemID )
				<< BUILD_LOG( temp.m_iQuantity )
				<< BUILD_LOG( temp.m_sPeriod )
				<< END_LOG;
			continue;
		}

		std::map< int, std::vector< KRewardData > >::iterator it;
		it = AckPacket.m_mapRewardData.find( iRewardID );
		if( it == AckPacket.m_mapRewardData.end() )
		{
			// 없다면..
			std::vector< KRewardData > vecRewardData;
			vecRewardData.push_back( temp );
			AckPacket.m_mapRewardData.insert( std::make_pair( iRewardID, vecRewardData ) );
		}
		else
		{
			it->second.push_back( temp );
		}
	}

end_proc:
	SendToServer( DBE_EVENT_REWARD_DATA_UPDATE_ACK, AckPacket );
}

//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
IMPL_ON_FUNC_NOPARAM( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_REQ )
{
	KDBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK	AckPacket;
	u_char									ucLevel;
	int										iRewardID;

	// Event Reward Level Up 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Event_RewardLevelUp_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_Event_RewardLevelUp_GET" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( ucLevel
			>> iRewardID
			);

		if( iRewardID <= 0 || ucLevel < 0 )
		{
			START_LOG( cerr, L"잘못된 EVENT_REWARD_LEVEL_UP 정보입니다." )
				<< BUILD_LOG( iRewardID )
				<< BUILD_LOGc( ucLevel )
				<< END_LOG;
			continue;
		}

		std::map< u_char, std::vector< int > >::iterator it;
		it = AckPacket.m_mapLevelUpRewardItem.find( ucLevel );
		if( it == AckPacket.m_mapLevelUpRewardItem.end() )
		{
			std::vector< int > vecRewardID;
			vecRewardID.push_back( iRewardID );
			AckPacket.m_mapLevelUpRewardItem.insert( std::make_pair( ucLevel, vecRewardID ) );
		}
		else
		{
			std::vector< int >::iterator vit = it->second.begin();
			for( ; vit != it->second.end() ; ++vit )
			{
				if( *vit == iRewardID )
				{
					START_LOG( cerr, L"중복되는 레벨업 보상 아이템이 존재합니다." )
						<< BUILD_LOGc( ucLevel )
						<< BUILD_LOG( iRewardID )
						<< BUILD_LOG( *vit )
						<< END_LOG;

					goto END_OF_FOR_DO_NOT_PUSH_BACK;
				}
			}
			it->second.push_back( iRewardID );
END_OF_FOR_DO_NOT_PUSH_BACK:;
		}
	}

end_proc:
	SendToServer( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK, AckPacket );
}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC_NOPARAM( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_REQ )
{
	KDBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK	AckPacket;
	int												iSlot;
	KRewardItem										temp;
	
	// Event Reward Level Up 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Event_RewardRetainUserItem_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#else //SERV_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_Event_RewardRetainUserItem_GET" );
#endif //SERV_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iSlot
			>> temp.m_iItemID
			>> temp.m_iQuantity
			);

		/*if( ( iSlot < 0 ) || ( temp.m_iItemID < 0 ) || ( temp.m_iQuantity < 0 ) )
		{
			START_LOG( cerr, L"잘못된 EVENT_REWARD_RETAIN_USER_ITEM 정보입니다." )
				<< BUILD_LOG( iSlot )
				<< BUILD_LOGc( temp.m_iItemID )
				<< BUILD_LOGc( temp.m_iQuantity )
				<< END_LOG;
			continue;
		}*/

		AckPacket.m_mapRewardItem.insert( std::make_pair( iSlot, temp ) );

		START_LOG( cout, L"이탈 방지 유저 아이템 보상 셋팅")
			<< BUILD_LOG( iSlot )
			<< BUILD_LOG( temp.m_iItemID )
			<< BUILD_LOG( temp.m_iQuantity );
	}

end_proc:
	SendToServer( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK, AckPacket );
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

IMPL_ON_FUNC( ELOG_ITEM_ABUSER_LOG_NOT )
{
	int iOK = NetError::NET_OK;

	std::vector< KItemAbuserLogInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecItemAbuserLog.begin(); vit != kPacket_.m_vecItemAbuserLog.end(); ++vit )
	{
		const KItemAbuserLogInfo& kLogInfo = *vit;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsGetItemLog_INS", L"%d, %d, %d, %d, %d", 
			% KBaseServer::GetKObj()->GetServerGroupID() 
			% kPacket_.m_iUnitUID 
			% kLogInfo.m_iRewardState 
			% kLogInfo.m_iItemID 
			% kLogInfo.m_iQuantity );
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_getitem_log", L"%d, %d, %d, %d", 
			% kPacket_.m_iUnitUID 
			% kLogInfo.m_iRewardState 
			% kLogInfo.m_iItemID 
			% kLogInfo.m_iQuantity );
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"아이템 어뷰저 로그 DB업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kLogInfo.m_iRewardState )
				<< BUILD_LOG( kLogInfo.m_iItemID )
				<< BUILD_LOG( kLogInfo.m_iQuantity )
				<< END_LOG;
		}
	}
end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;
}

IMPL_ON_FUNC( ELOG_ENCHANT_ABUSER_LOG_NOT )
{
	int iOK = NetError::NET_OK;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsEnchantLog_INS", L"%d, %d, %d, %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iItemUID
		% (int)kPacket_.m_cEnchantLevel
		% kPacket_.m_iEnchantResult
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_enchant_log", L"%d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iItemUID
		% (int)kPacket_.m_cEnchantLevel
		% kPacket_.m_iEnchantResult
		);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"아이템 어뷰저 로그 DB업데이트 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_cEnchantLevel )
			<< BUILD_LOG( kPacket_.m_iEnchantResult )
			<< END_LOG;
	}
	
end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;
}

IMPL_ON_FUNC( ELOG_ED_ABUSER_LOG_NOT )
{
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsMoneyLog_INS", L"%d, %d, %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iState
		% kPacket_.m_iBeforeED
		% kPacket_.m_iAfterED
		% kPacket_.m_iED
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_moeny_log", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iState
		% kPacket_.m_iBeforeED
		% kPacket_.m_iAfterED
		% kPacket_.m_iED
		);
#endif //SERV_RENEWAL_SP

end_proc:
	return;
}

//{{ 2008. 10. 22  최육사	캐쉬 분해 로그
IMPL_ON_FUNC( ELOG_CASH_ITEM_RESOLVE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_MULTI_RESOLVE
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsDecomposition_INS", L"%d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iResultQuantity
		% kPacket_.m_iAfterItemID
		% kPacket_.m_iAfterItemQuantity
		% kPacket_.m_iLevel
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsDecompositionLog", L"%d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iResultQuantity
		% kPacket_.m_iAfterItemID
		% kPacket_.m_iAfterItemQuantity
		% kPacket_.m_iLevel
		);
#endif //SERV_RENEWAL_SP
#else // SERV_MULTI_RESOLVE
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsDecomposition_INS", L"%d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iResultQuantity
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsDecompositionLog", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iResultQuantity
		);
#endif // SERV_RENEWAL_SP
#endif // SERV_MULTI_RESOLVE
	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}	

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐쉬 아이템 분해 로그 DB업데이트 실패!" )
			<< BUILD_LOG( iOK )			
			<< BUILD_LOG( kPacket_.m_iUnitUID )			
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iResultQuantity )
			<< END_LOG;
	}

	return;
}
//}}

//{{ 2008. 11. 10  최육사
IMPL_ON_FUNC( ELOG_ENCHANT_ITEM_BREAK_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsItemBreak_INS", L"%d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iItemID
		% static_cast<int>(kPacket_.m_cEnchantLevel)
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertItemBreak", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iItemID
		% static_cast<int>(kPacket_.m_cEnchantLevel)
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
		START_LOG( cerr, L"강화 파괴 로그 DB업데이트 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< BUILD_LOGc( kPacket_.m_cEnchantLevel )
			<< END_LOG;
	}

	return;
}
//}}

//{{ 2008. 10. 31  최육사	어뷰저 리스트
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_ABUSER_LIST_REQ )
{
	KDBE_ABUSER_LIST_ACK ackPacket;

	// Abuser List 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_AbuserList_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID()
														% kPacket_.m_iRtt );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_get_abuser_list", L"%d", % kPacket_.m_iRtt );
#endif //SERV_RENEWAL_SP

	ackPacket.m_iRtt = kPacket_.m_iRtt;

	while( m_kODBC.Fetch() )
	{
		UidType iUnitUID = 0;

		FETCH_DATA( iUnitUID );

		if( iUnitUID <= 0 )
			continue;

		ackPacket.m_vecAbuserList.push_back( iUnitUID );
	}

end_proc:
	SendToServer( DBE_ABUSER_LIST_ACK, ackPacket );
}
#else
IMPL_ON_FUNC( DBE_ABUSER_LIST_REQ )
{
	KDBE_ABUSER_LIST_ACK kPacket;
	std::map< int, int >::const_iterator mit;	
	std::map< int, KAbuserList >::iterator mitAL;

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

		mit = kPacket_.m_mapReleaseTick.find( iAbuserType );
		if( mit != kPacket_.m_mapReleaseTick.end() )
		{
			// DB에 저장된 ReleaseTick이 더 크다면..
			if( iReleaseTick > mit->second )
			{
				KAbuserList kAbuserInfo;
				kAbuserInfo.m_iReleaseTick = iReleaseTick;
				kPacket.m_mapAbuserList.insert( std::make_pair( iAbuserType, kAbuserInfo ) );
			}
		}
		else
		{
			START_LOG( clog, L"Abuser List와는 관계없는 ReleaseTick입니다." )
				<< BUILD_LOG( iAbuserType );
		}
	}

	// Abuser List 얻기
	for( mitAL = kPacket.m_mapAbuserList.begin(); mitAL != kPacket.m_mapAbuserList.end(); ++mitAL )
	{
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_AbuserList_SEL", L"%d, %d",	% KBaseServer::GetKObj()->GetServerGroupID()
															% mitAL->first );
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_get_abuser_list", L"%d", % mitAL->first );
#endif //SERV_RENEWAL_SP

		while( m_kODBC.Fetch() )
		{
			UidType iUnitUID = 0;

			FETCH_DATA( iUnitUID );

			if( iUnitUID <= 0 )
				continue;

			mitAL->second.m_vecAbuserList.push_back( iUnitUID );
		}
	}

end_proc:
	SendToServer( DBE_ABUSER_LIST_ACK, kPacket );
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//}}

IMPL_ON_FUNC( DBE_REG_ABUSER_AUTOMATIC_NOT )
{
	std::map< int, UidType >::const_iterator mit;
	for( mit = kPacket_.m_mapRegAbuserInfo.begin(); mit != kPacket_.m_mapRegAbuserInfo.end(); ++mit )
	{
		const int iType = mit->first;
		const UidType iUID = mit->second;

		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_AbuserList_INS", L"%d, %d, %d",  % KBaseServer::GetKObj()->GetServerGroupID() 
																% iType 
																% iUID );
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_abuser_list", L"%d, %d", % iType % iUID );
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}	

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"어뷰저 자동 등록 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iType )
				<< BUILD_LOG( iUID )
				<< END_LOG;
		}
	}
}

//{{ 2008. 12. 18  최육사	보조제 로그
IMPL_ON_FUNC( ELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가 - 로그 남기기
	// 파괴 방지까지 합쳐서 한번에 적용. 
#ifdef SERV_ENCHANT_PLUS_ITEM
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_statsEnchantAid_INS", L"%d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iItemUID 
		% static_cast<int>(kPacket_.m_cBeforeLevel) 
		% static_cast<int>(kPacket_.m_cAfterLevel)
		% static_cast<bool>(kPacket_.m_bUseSupport) 
		% static_cast<bool>(kPacket_.m_bUsePlus) 
		% static_cast<bool>(kPacket_.m_bUseDestroyGuard)
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertAidLog", L"%d, %d, %d, %d, %d, %d",
		% kPacket_.m_iItemUID 
		% static_cast<int>(kPacket_.m_cBeforeLevel) 
		% static_cast<int>(kPacket_.m_cAfterLevel)
		% static_cast<bool>(kPacket_.m_bUseSupport) 
		% static_cast<bool>(kPacket_.m_bUsePlus) 
		% static_cast<bool>(kPacket_.m_bUseDestroyGuard)
		);
#endif //SERV_RENEWAL_SP
#else
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_statsEnchantAid_INS", L"%d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iItemUID 
		% static_cast<int>(kPacket_.m_cBeforeLevel) 
		% static_cast<int>(kPacket_.m_cAfterLevel) 
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertAidLog", L"%d, %d, %d", 
		% kPacket_.m_iItemUID 
		% static_cast<int>(kPacket_.m_cBeforeLevel) 
		% static_cast<int>(kPacket_.m_cAfterLevel) 
		);
#endif //SERV_RENEWAL_SP
#endif
	//}}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가 - 로그 남기기
		// 파괴 방지까지 합쳐서 한번에 적용. 
#ifdef SERV_ENCHANT_PLUS_ITEM
		START_LOG( cerr, L"강화 보조제 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOGc( kPacket_.m_cBeforeLevel )
			<< BUILD_LOGc( kPacket_.m_cAfterLevel )
			<< BUILD_LOG( kPacket_.m_bUseSupport )
			<< BUILD_LOG( kPacket_.m_bUsePlus )
			<< BUILD_LOG( kPacket_.m_bUseDestroyGuard )
			<< END_LOG;
#else
		START_LOG( cerr, L"강화 보조제 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOGc( kPacket_.m_cBeforeLevel )
			<< BUILD_LOGc( kPacket_.m_cAfterLevel )
			<< END_LOG;
#endif//SERV_ENCHANT_PLUS_ITEM
		//}}
	}
	return;
}
//}}

//{{ 2008. 12. 22  최육사	깨짐 복구 로그
IMPL_ON_FUNC( ELOG_RESTORE_ITEM_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsItemResurrection_INS", L"%d, %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iItemUID 
		% kPacket_.m_iBeforeEnchantLv 
		% kPacket_.m_iAfterEnchantLv 
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertItemResurrection", L"%d, %d, %d, %d", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iItemUID 
		% kPacket_.m_iBeforeEnchantLv 
		% kPacket_.m_iAfterEnchantLv 
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
		START_LOG( cerr, L"강화 깨짐 복구 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iBeforeEnchantLv )
			<< BUILD_LOG( kPacket_.m_iAfterEnchantLv )
			<< END_LOG;
	}
	return;
}
//}}

//{{ 2008. 12. 22  최육사	부여 로그
IMPL_ON_FUNC( ELOG_ATTACH_ITEM_ENCHANT_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsCharmItemLog_Direct_INS", L"%d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUnitUID
		% kPacket_.m_iAttachItemID
		% kPacket_.m_iDestItemUID
		% kPacket_.m_iBeforeEnchantLv 
		% kPacket_.m_iAfterEnchantLv
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertCharmItemLog_Direct", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iAttachItemID
		% kPacket_.m_iDestItemUID
		% kPacket_.m_iBeforeEnchantLv 
		% kPacket_.m_iAfterEnchantLv
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
		START_LOG( cerr, L"강화 부여 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iAttachItemID )
			<< BUILD_LOG( kPacket_.m_iDestItemUID )
			<< BUILD_LOG( kPacket_.m_iBeforeEnchantLv )
			<< BUILD_LOG( kPacket_.m_iAfterEnchantLv )
			<< END_LOG;
	}
	return;
}
//}}

//{{ 2008. 12. 30  최육사	매력아이템 로그
IMPL_ON_FUNC( ELOG_CHARM_ITEM_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsCharmItemLog_INS", L"%d, %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iCharmItemID 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iResultItemID 
		% kPacket_.m_iQuantity 
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertCharmItemLog", L"%d, %d, %d, %d", 
		% kPacket_.m_iCharmItemID 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iResultItemID 
		% kPacket_.m_iQuantity 
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
		START_LOG( cerr, L"매력 아이템 사용 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iCharmItemID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iResultItemID )
			<< BUILD_LOG( kPacket_.m_iQuantity )
			<< END_LOG;
	}
	return;
}
//}}

//{{ 2009. 4. 14  최육사	닉네임 변경
IMPL_ON_FUNC( ELOG_DELETE_NICK_NAME_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsNickNameChange_INS", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrNickName );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_Nickname_Delete_Log", L"%d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrNickName );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"닉네임 초기화 로그 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;
	}
	return;
}

IMPL_ON_FUNC( ELOG_CHANGE_NICK_NAME_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsNickNameRestore_INS", L"%d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrNickName );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_Nickname_Restore_Log", L"%d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrNickName );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"닉네임 변경 로그 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;
	}
	return;
}
//}}

IMPL_ON_FUNC( ELOG_INIT_SKILL_TREE_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsAllSkillDelete_INS", L"%d, %d, %d, %d, %d", 
														% KBaseServer::GetKObj()->GetServerGroupID() 
														% kPacket_.m_iUnitUID
														% kPacket_.m_iItemUID
														% kPacket_.m_iSPoint
														% kPacket_.m_iCSPoint );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_allskilldelete", L"%d, %d, %d, %d",	% kPacket_.m_iUnitUID
																		% kPacket_.m_iItemUID
																		% kPacket_.m_iSPoint
																		% kPacket_.m_iCSPoint );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"스킬트리 초기화 로그 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iSPoint )
			<< BUILD_LOG( kPacket_.m_iCSPoint )
			<< END_LOG;
	}
	return;
}



IMPL_ON_FUNC( ELOG_INSERT_CASH_SKILL_POINT_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsCSPLog_INS", L"%d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUnitLevel
		% kPacket_.m_iMaxCSPoint
		% kPacket_.m_iPeriod
		% kPacket_.m_bUpdateEndDateOnly );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertCSPLog", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUnitLevel
		% kPacket_.m_iMaxCSPoint
		% kPacket_.m_iPeriod
		% kPacket_.m_bUpdateEndDateOnly );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐시스킬포인트 사용시작 로그 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUnitLevel )
			<< BUILD_LOG( kPacket_.m_iMaxCSPoint )
			<< BUILD_LOG( kPacket_.m_iPeriod )
			<< BUILD_LOG( kPacket_.m_bUpdateEndDateOnly )
			<< END_LOG;
	}
	return;
}

//{{ 2009. 7. 22  최육사	메가폰로그
IMPL_ON_FUNC( ELOG_MEGAPHONE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsMegaphone_INS", L"%d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_megaphone_log", L"%d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"메가폰 사용 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< END_LOG;
	}
	return;
}
//}}

//{{ 2009. 9. 1  최육사		비정상종료사유
IMPL_ON_FUNC( ELOG_ABNORMAL_DISCONNECT )
{
	std::map< DWORD, int >::const_iterator mit;
	for( mit = kPacket_.m_mapAbnormalDisconnect.begin(); mit != kPacket_.m_mapAbnormalDisconnect.end(); ++mit )
	{
#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_StatsAbnormalReason_SEL", L"%d, %d, %d, %d", % KBaseServer::GetKObj()->GetServerGroupID()
																			% kPacket_.m_iServerUID 
																			% mit->first 
																			% mit->second );
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_abnormal_reason", L"%d, %d, %d", % kPacket_.m_iServerUID % mit->first % mit->second );
#endif //SERV_RENEWAL_SP
		continue;

end_proc:
		START_LOG( cerr, L"비정상 종료 통계 DB업데이트 실패!" )			
			<< BUILD_LOG( kPacket_.m_iServerUID )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second )
			<< END_LOG;
	}
}
//}}

//{{ 2009. 9. 9  최육사		밀봉
IMPL_ON_FUNC( ELOG_USE_SEAL_ITEM_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsSealLog_INS", L"%d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSealItemUID
		% kPacket_.m_iDestItemUID
		% static_cast<int>(kPacket_.m_ucSealCount)
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertSealLog", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSealItemUID
		% kPacket_.m_iDestItemUID
		% static_cast<int>(kPacket_.m_ucSealCount)
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
		START_LOG( cerr, L"밀봉 사용 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iDestItemUID )
			<< BUILD_LOG( kPacket_.m_iSealItemUID )
			<< BUILD_LOGc( kPacket_.m_ucSealCount )
			<< END_LOG;
	}
	return;
}
//}}

//{{ 2009. 10. 22  최육사	트래픽 유발 유저 해킹유저로 자동등록
IMPL_ON_FUNC( ELOG_TRAFFIC_ATTACKER_LOG_NOT )
{
	// 로그 기록
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_statsHackUser_INS", L"%d, N\'%s\', %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_wstrUserID
		% kPacket_.m_iChannelID
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertHackUser", L"N\'%s\', %d",
		% kPacket_.m_wstrUserID
		% kPacket_.m_iChannelID
		);
#endif //SERV_RENEWAL_SP

	return;

end_proc:
	START_LOG( cerr, L"트래픽 공격자 로그 등록 실패!" )		
		<< BUILD_LOG( kPacket_.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_iChannelID )
		<< END_LOG;
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

//{{ 2010. 06. 30  최육사	근성도 통계
#ifdef SERV_SPIRIT_STATISTICS

//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
IMPL_ON_FUNC( DBE_SPIRIT_STATISTICS_NOT )
{
	int iOK = NetError::NET_OK;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsSpiritLog_INS", L"%d, %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSpiritPlayTime
		% kPacket_.m_iDurationTime
		% kPacket_.m_iUsedSpirit
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_spirit_log", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSpiritPlayTime
		% kPacket_.m_iDurationTime
		% kPacket_.m_iUsedSpirit
		);
#endif //SERV_RENEWAL_SP

	return;

end_proc:
	START_LOG( cerr, L"근성도 통계 정보 DB업데이트 실패!" )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iSpiritPlayTime )
		<< BUILD_LOG( kPacket_.m_iDurationTime )
		<< BUILD_LOG( kPacket_.m_iUsedSpirit )
		<< END_LOG;
}
#else
IMPL_ON_FUNC( DBE_SPIRIT_STATISTICS_NOT )
{
	int iOK = NetError::NET_OK;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsSpiritLog_INS", L"%d, %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iDurationTime
		% kPacket_.m_iUsedSpirit
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_spirit_log", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iDurationTime
		% kPacket_.m_iUsedSpirit
		);
#endif //SERV_RENEWAL_SP

	return;

end_proc:
	START_LOG( cerr, L"근성도 통계 정보 DB업데이트 실패!" )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iDungeonPlayTime )
		<< BUILD_LOG( kPacket_.m_iDurationTime )
		<< BUILD_LOG( kPacket_.m_iUsedSpirit )
		<< END_LOG;
}
#endif SERV_FIELD_PLAY_TIME_LOG
//}}

#endif SERV_SPIRIT_STATISTICS
//}}

#ifdef SERV_KOG_STATISTICS
IMPL_ON_FUNC_NOPARAM(DBE_CCU_AGENT_REQ)
{
	KDBE_CCU_AGENT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iDailyResisterredUser = 0;
	kPacket.m_iDailyUniqueUser = 0;

	CTime ct = CTime::GetCurrentTime();
	std::wstring wstrCurrentTime;

	wstrCurrentTime = ct.GetYear();
	wstrCurrentTime += L"-";
	wstrCurrentTime += ct.GetMonth();
	wstrCurrentTime += L"-";
	wstrCurrentTime += ct.GetDay();
	wstrCurrentTime += L" ";
	wstrCurrentTime += ct.GetHour();
	wstrCurrentTime += L":";
	wstrCurrentTime += ct.GetMinute();

	//기존 SP명 : Mail_Day_Info
	DO_QUERY_NO_ARG( L"exec dbo.P_CBT3rdDailyUserStats_SEL" );

	if( true == m_kODBC.BeginFetch() )
	{
		int iTopconnectcnt, iAvgconnectcnt, iNewuniquser, iTotaluser, iDayuniquser, iAvgDT, iTemp01, iTemp02, iAVGArcadePT;
		std::wstring wstrTopconnecttime;

		FETCH_DATA(
			iTopconnectcnt >>
			wstrTopconnecttime >>
			iAvgconnectcnt >>
			iNewuniquser >>
			iTotaluser >>
			iDayuniquser >>
			iAvgDT >>
			iTemp01 >>
			iTemp02 >>
			iAVGArcadePT
			);

		kPacket.m_iDailyResisterredUser = iNewuniquser;
		kPacket.m_iDailyUniqueUser = iDayuniquser;		

		START_LOG( clog, L"KOG STATISTICS - DAILY INFO SEND.!" )
			<< BUILD_LOG( wstrCurrentTime )
			<< BUILD_LOG( iTopconnectcnt )
			<< BUILD_LOG( wstrTopconnecttime )
			<< BUILD_LOG( iAvgconnectcnt )
			<< BUILD_LOG( iNewuniquser )
			<< BUILD_LOG( iTotaluser )
			<< BUILD_LOG( iDayuniquser )
			<< BUILD_LOG( iAvgDT )
			<< BUILD_LOG( iTemp01 )
			<< BUILD_LOG( iTemp02 )
			<< BUILD_LOG( iAVGArcadePT )
			;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_ODBC_01;
		START_LOG( cerr, L"KOG STATISTICS - QUERY FAIL.!" )
			<< BUILD_LOG( wstrCurrentTime )
			<< BUILD_LOG( NetError::GetErrStr(kPacket.m_iOK) )
			<< END_LOG;
	}

end_proc:
	SendToServer( DBE_CCU_AGENT_ACK, kPacket );
}
#endif SERV_KOG_STATISTICS


//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING

IMPL_ON_FUNC( ELOG_INSERT_TRAFFIC_ABUSER_LOG_NOT )
{
    // TO DO :
}

#endif SERV_TRAFFIC_USER_MONITORING
//}}

//{{ 2011. 01. 05	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM

IMPL_ON_FUNC( ELOG_ATTRIB_ATTACH_ITEM_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsAttrib_AttachLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUnitUID
		% kPacket_.m_iAttachItemID
		% kPacket_.m_iDestItemUID
		% static_cast<int>(kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant0)
		% static_cast<int>(kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant1)
		% static_cast<int>(kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant2)
		% static_cast<int>(kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant0)
		% static_cast<int>(kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant1)
		% static_cast<int>(kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant2)
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.StatsInsertAttrib_AttachLog", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iAttachItemID
		% kPacket_.m_iDestItemUID
		% static_cast<int>(kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant0)
		% static_cast<int>(kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant1)
		% static_cast<int>(kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant2)
		% static_cast<int>(kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant0)
		% static_cast<int>(kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant1)
		% static_cast<int>(kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant2)
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
		START_LOG( cerr, L"속성 부여 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iAttachItemID )
			<< BUILD_LOG( kPacket_.m_iDestItemUID )
			<< BUILD_LOGc( kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant0 )
			<< BUILD_LOGc( kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant1 )
			<< BUILD_LOGc( kPacket_.m_kBeforeAttribEnchantInfo.m_cAttribEnchant2 )
			<< BUILD_LOGc( kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant0 )
			<< BUILD_LOGc( kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant1 )
			<< BUILD_LOGc( kPacket_.m_kAfterAttribEnchantInfo.m_cAttribEnchant2 )
			<< END_LOG;
	}
}

#endif SERV_ATTRIBUTE_CHARM
//}}

//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
IMPL_ON_FUNC( ELOG_USER_CHARACTER_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCharacterLog_INS", L"%d, N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', N\'%s\', %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_wstrRegDate
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_bOnlyOneUnit
		% kPacket_.m_iServerUID
		% kPacket_.m_sLogType
		% static_cast<int>(kPacket_.m_ucLevel)
		% static_cast<int>(kPacket_.m_cUnitClass)
		% kPacket_.m_iDungeonID
		% kPacket_.m_iTotalDungeonPlayTime
		% kPacket_.m_iTotalPvpPlayTime
		% kPacket_.m_iDurationTime
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrUnitCreateDate
		% kPacket_.m_wstrAccountCreateDate
		% kPacket_.m_iProductNo
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_character_Log", L"N\'%s\', %d, N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', N\'%s\', %d",
		% kPacket_.m_wstrRegDate
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% kPacket_.m_bOnlyOneUnit
		% kPacket_.m_iServerUID
		% kPacket_.m_sLogType
		% static_cast<int>(kPacket_.m_ucLevel)
		% static_cast<int>(kPacket_.m_cUnitClass)
		% kPacket_.m_iDungeonID
		% kPacket_.m_iTotalDungeonPlayTime
		% kPacket_.m_iTotalPvpPlayTime
		% kPacket_.m_iDurationTime
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrUnitCreateDate
		% kPacket_.m_wstrAccountCreateDate
		% kPacket_.m_iProductNo
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
		START_LOG( cerr, L"캐릭터 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_bOnlyOneUnit )
			<< BUILD_LOG( kPacket_.m_iServerUID )
			<< BUILD_LOG( kPacket_.m_sLogType )
			<< BUILD_LOGc( kPacket_.m_ucLevel )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOG( kPacket_.m_iTotalDungeonPlayTime )
			<< BUILD_LOG( kPacket_.m_iTotalPvpPlayTime )
			<< BUILD_LOG( kPacket_.m_iDurationTime )
			<< BUILD_LOG( kPacket_.m_wstrUnitCreateDate )
			<< BUILD_LOG( kPacket_.m_wstrAccountCreateDate )
			<< BUILD_LOG( kPacket_.m_iProductNo )
			<< END_LOG;
	}
}
#endif SERV_CHAR_LOG
//}}

//{{ 2011. 02. 09	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG
IMPL_ON_FUNC( ELOG_BUY_CASH_ITEM_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCashSalesLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iProductNo
		% kPacket_.m_iBuyQuantity
		% kPacket_.m_iTotalSalePrice
		% kPacket_.m_wstrRegDate
		% kPacket_.m_bIsFirstBuy
		% static_cast<int>(kPacket_.m_cBuyType)
		% kPacket_.m_wstrReceiverUserID
		);
#else //SERV_RENEWAL_SP
	//{{ 2011. 10. 11    김민성    캐쉬 아이템 선물 할 때 받는 유저의 정보 DB에 기록하기
#ifdef SERV_PRESENT_CASH_ITEM_ADD_LOG
	DO_QUERY( L"exec dbo.lup_insert_CashSales_Log", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iProductNo
		% kPacket_.m_iBuyQuantity
		% kPacket_.m_iTotalSalePrice
		% kPacket_.m_wstrRegDate
		% kPacket_.m_bIsFirstBuy
		% static_cast<int>(kPacket_.m_cBuyType)
		% kPacket_.m_wstrReceiverUserID
		);
#else
	DO_QUERY( L"exec dbo.lup_insert_CashSales_Log", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_ucLevel)
		% kPacket_.m_iProductNo
		% kPacket_.m_iBuyQuantity
		% kPacket_.m_iTotalSalePrice
		% kPacket_.m_wstrRegDate
		% kPacket_.m_bIsFirstBuy
		% static_cast<int>(kPacket_.m_cBuyType)
		);
#endif SERV_PRESENT_CASH_ITEM_ADD_LOG
	//}} 
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐쉬템 로그 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_cUnitClass )
			<< BUILD_LOGc( kPacket_.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_iProductNo )
			<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			<< BUILD_LOG( kPacket_.m_iTotalSalePrice )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< BUILD_LOG( kPacket_.m_bIsFirstBuy )
			<< BUILD_LOGc( kPacket_.m_cBuyType )
			//{{ 2011. 10. 11    김민성    캐쉬 아이템 선물 할 때 받는 유저의 정보 DB에 기록하기
#ifdef SERV_PRESENT_CASH_ITEM_ADD_LOG
			<< BUILD_LOG( kPacket_.m_wstrReceiverUserID )
#endif SERV_PRESENT_CASH_ITEM_ADD_LOG
			//}} 
			<< END_LOG;
	}
}
#endif SERV_BUY_CASH_ITEM_LOG
//}}

//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
IMPL_ON_FUNC( ELOG_CASH_PRODUCT_LIST_UPDATE_NOT )
{
	// 캐쉬 상품 리스트를 먼저 비운다.
// 
//	DO_QUERY_NO_ARG( L"exec dbo.lup_delete_Product_list" ); // 주석 풀면 P_LProductList_DEL 이거 써야함

	// 캐쉬 상품 리스트를 다시 채워 넣는다.
	BOOST_TEST_FOREACH( const KCashPruductInfo&, kCashProductInfo, kPacket_.m_vecCashProductList )
	{
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LProductList_INS", L"%d, %d, %d, %d, %d, %d", 
			% KBaseServer::GetKObj()->GetServerGroupID() 
			% kCashProductInfo.m_ulProductNo
			% kCashProductInfo.m_sProductExpire
			% kCashProductInfo.m_usProductPieces
			% kCashProductInfo.m_iItemID
			% kCashProductInfo.m_ulSalePrice
			);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_Product_list", L"%d, %d, %d, %d, %d",
			% kCashProductInfo.m_ulProductNo
			% kCashProductInfo.m_sProductExpire
			% kCashProductInfo.m_usProductPieces
			% kCashProductInfo.m_iItemID
			% kCashProductInfo.m_ulSalePrice
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			//START_LOG( cerr, L"캐쉬템 로그 등록 실패!" )
			//	<< BUILD_LOG( iOK )
			//	<< BUILD_LOG( kPacket_.m_iUserUID )
			//	<< BUILD_LOG( kPacket_.m_iUnitUID )
			//	<< BUILD_LOGc( kPacket_.m_cUnitClass )
			//	<< BUILD_LOGc( kPacket_.m_ucLevel )
			//	<< BUILD_LOG( kPacket_.m_iProductNo )
			//	<< BUILD_LOG( kPacket_.m_iBuyQuantity )
			//	<< BUILD_LOG( kPacket_.m_iTotalSalePrice )
			//	<< BUILD_LOG( kPacket_.m_wstrRegDate )
			//	<< BUILD_LOG( kPacket_.m_iServerGroupID )
			//	<< BUILD_LOG( kPacket_.m_bIsFirstBuy )
			//	<< BUILD_LOGc( kPacket_.m_cBuyType )
			//	<< END_LOG;
		}
	}

end_proc:
	return;
}
#endif SERV_CASH_ITEM_LIST
//}}

//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
IMPL_ON_FUNC( DBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ )
{
	KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LPost_TradeMonitor_INS", L"%d, %d, N\'%s\', %d", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iRecvUnitUID
		% kPacket_.m_wstrRegDate
		% static_cast<int>(kPacket_.m_cType)
		);
#else //SERV_RENEWAL_SP	
	DO_QUERY( L"exec dbo.lup_insert_LPost_TradeMonitor", L"%d, N\'%s\', %d",
		% kPacket_.m_iRecvUnitUID
		% kPacket_.m_wstrRegDate
		% static_cast<int>(kPacket_.m_cType)
		);
#endif //SERV_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK) // SP반환값 -10 : 중복 데이터가 있다
		{
		case -10:
			{
				kPacket.m_iOK = (kPacket_.m_cType == 0) ? NetError::ERR_EXIST_LETTER_MONITORING : NetError::ERR_EXIST_TRADE_MONITORING;
			}break;
		case -11:
			{
				kPacket.m_iOK = (kPacket_.m_cType == 0) ? NetError::ERR_INSERT_FAILD_LETTER_MONITORING : NetError::ERR_INSERT_FAILD_TRADE_MONITORING;
			}break;
		default:
			{
				START_LOG( cout, L"우편 및 거래 감시 로그 기록 실패!!  :")
					<< BUILD_LOG( kPacket_.m_iRecvUnitUID )
					<< BUILD_LOGc( kPacket_.m_cType )
					<< END_LOG;
			}break;
		}
	}
	else
	{
		kPacket.m_iOK = static_cast<int>(kPacket_.m_cType);
		
		START_LOG( cout, L"우편 및 거래 감시 로그 기록 성공!!  :")
			<< BUILD_LOG( kPacket_.m_iRecvUnitUID )
			<< BUILD_LOGc( kPacket_.m_cType )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_WRITE_LETTER_AND_TRADE_MOINTORING_ACK, kPacket );
}
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB

_IMPL_ON_FUNC( ELOG_STAT_ITEM_DB, KStatisticsData )
{
	CTime tCurTime = CTime::GetCurrentTime();
	const std::wstring wstrCurTime = (const wchar_t*)tCurTime.Format(_T("%Y-%m-%d %H:00:00"));
	const UidType iServerUID = KBaseServer::GetKObj()->GetUID();

	int iOK = NetError::ERR_ODBC_01;
	KStatisticsData::const_iterator mit;
	for( mit = kPacket_.begin(); mit != kPacket_.end(); ++mit )
	{
		const std::vector< int >& vecIntKey = mit->first.m_vecIntKey;
		if( vecIntKey.empty() )
		{
			START_LOG( cerr, L"통계키값이 비었음" )
				<< END_LOG;
			continue;
		}

		const int iItemID = mit->first.m_vecIntKey[0];
		__int64 aiCount[KStatistics::SI_ITEM_MAX];
		std::vector< __int64 >::const_iterator vit = mit->second.begin();
		for( int iIndex = 0; iIndex < KStatistics::SI_ITEM_MAX; ++iIndex )
		{
			if( vit == mit->second.end() )
			{
				START_LOG( clog2, L"카운트 데이터가 비었음." )
					<< BUILD_LOG( iIndex )
					<< END_LOG;

				aiCount[iIndex] = 0;
			}
			else
			{
				aiCount[iIndex] = *vit;
				vit++;
			}
		}

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LItemLog_INS", 
			L"%d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, N\'%s\', %d, %d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iItemID										// @iItemID  INT
			% aiCount[KStatistics::SI_ITEM_TRADE]			// @iTrade   INT
			% aiCount[KStatistics::SI_ITEM_PERSONAL_SHOP]	// @iPShop   INT
			% aiCount[KStatistics::SI_ITEM_TRADE_BOARD]		// @iTBoard  INT
			% aiCount[KStatistics::SI_ITEM_BUY]				// @iBuy   INT
			% aiCount[KStatistics::SI_ITEM_SELL]			// @iSell   INT
			% aiCount[KStatistics::SI_ITEM_USE]				// @iUse   INT
			% aiCount[KStatistics::SI_ITEM_MAKE]			// @iMake   INT
			% aiCount[KStatistics::SI_ITEM_FAILED]			// @iFailed  INT
			% aiCount[KStatistics::SI_ITEM_DROP]			// @iDrop   INT
			% aiCount[KStatistics::SI_ITEM_QUEST]			// @iQuest   INT
			% aiCount[KStatistics::SI_ITEM_EQUIP]			// @iEquip   INT
			% aiCount[KStatistics::SI_ITEM_RANDOM]			// @iRandom  INT
			% aiCount[KStatistics::SI_ITEM_RESOLVE]			// @iResolve  INT
			% aiCount[KStatistics::SI_ITEM_R_RESULT]		// @iR_Result  INT
			% aiCount[KStatistics::SI_ITEM_MALL]			// @iMall   INT
			% aiCount[KStatistics::SI_ITEM_POST]			// @iPost   INT
			% aiCount[KStatistics::SI_ITEM_IDENTIFY]		// @iAttrib  INT
			% aiCount[KStatistics::SI_ITEM_EXCHANGE_S]		// @iExchange_S INT
			% aiCount[KStatistics::SI_ITEM_EXCHANGE_R]		// @iExchange_R INT
			% aiCount[KStatistics::SI_ITEM_DISCARD]			// @iDiscard  INT
			//{{ 2010. 9. 14	최육사	펫 시스템
			//#ifdef SERV_PET_SYSTEM
			% aiCount[KStatistics::SI_ITEM_PET_FEED]		// @iPetFeed  INT
			//#endif SERV_PET_SYSTEM
			//}}
			% wstrCurTime									// @sdtRegDate  SMALLDATETIME
				% iServerUID									// @iServerUID  INT
				//{{ 2011. 05. 02	최육사	대리상인
				//#ifdef SERV_PSHOP_AGENCY
				% aiCount[KStatistics::SI_ITEM_REG_PSHOP]
				% aiCount[KStatistics::SI_ITEM_BUY_PSHOP_AGENCY]
				% aiCount[KStatistics::SI_ITEM_PICK_UP_PSHOP]
				//#endif SERV_PSHOP_AGENCY
				//}}
				);
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.lup_insert_Item_Log", 
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, N\'%s\', %d, %d, %d, %d",
			% iItemID										// @iItemID  INT
			% aiCount[KStatistics::SI_ITEM_TRADE]			// @iTrade   INT
			% aiCount[KStatistics::SI_ITEM_PERSONAL_SHOP]	// @iPShop   INT
			% aiCount[KStatistics::SI_ITEM_TRADE_BOARD]		// @iTBoard  INT
			% aiCount[KStatistics::SI_ITEM_BUY]				// @iBuy   INT
			% aiCount[KStatistics::SI_ITEM_SELL]			// @iSell   INT
			% aiCount[KStatistics::SI_ITEM_USE]				// @iUse   INT
			% aiCount[KStatistics::SI_ITEM_MAKE]			// @iMake   INT
			% aiCount[KStatistics::SI_ITEM_FAILED]			// @iFailed  INT
			% aiCount[KStatistics::SI_ITEM_DROP]			// @iDrop   INT
			% aiCount[KStatistics::SI_ITEM_QUEST]			// @iQuest   INT
			% aiCount[KStatistics::SI_ITEM_EQUIP]			// @iEquip   INT
			% aiCount[KStatistics::SI_ITEM_RANDOM]			// @iRandom  INT
			% aiCount[KStatistics::SI_ITEM_RESOLVE]			// @iResolve  INT
			% aiCount[KStatistics::SI_ITEM_R_RESULT]		// @iR_Result  INT
			% aiCount[KStatistics::SI_ITEM_MALL]			// @iMall   INT
			% aiCount[KStatistics::SI_ITEM_POST]			// @iPost   INT
			% aiCount[KStatistics::SI_ITEM_IDENTIFY]		// @iAttrib  INT
			% aiCount[KStatistics::SI_ITEM_EXCHANGE_S]		// @iExchange_S INT
			% aiCount[KStatistics::SI_ITEM_EXCHANGE_R]		// @iExchange_R INT
			% aiCount[KStatistics::SI_ITEM_DISCARD]			// @iDiscard  INT
			//{{ 2010. 9. 14	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
			% aiCount[KStatistics::SI_ITEM_PET_FEED]		// @iPetFeed  INT
//#endif SERV_PET_SYSTEM
			//}}
			% wstrCurTime									// @sdtRegDate  SMALLDATETIME
			% iServerUID									// @iServerUID  INT
			//{{ 2011. 05. 02	최육사	대리상인
//#ifdef SERV_PSHOP_AGENCY
			% aiCount[KStatistics::SI_ITEM_REG_PSHOP]
			% aiCount[KStatistics::SI_ITEM_BUY_PSHOP_AGENCY]
			% aiCount[KStatistics::SI_ITEM_PICK_UP_PSHOP]
//#endif SERV_PSHOP_AGENCY
			//}}
			);
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		START_LOG( clog2, L"아이템 통계 [검사]" )
			<< BUILD_LOG( iItemID )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_TRADE] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_PERSONAL_SHOP] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_TRADE_BOARD] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_BUY] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_SELL] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_USE] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_MAKE] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_FAILED] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_DROP] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_QUEST] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_EQUIP] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_RANDOM] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_RESOLVE] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_R_RESULT] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_MALL] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_POST] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_IDENTIFY] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_EXCHANGE_S] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_EXCHANGE_R] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_DISCARD] )
			//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_PET_FEED] )
#endif SERV_PET_SYSTEM
			//}}
			//{{ 2011. 05. 02	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_REG_PSHOP] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_BUY_PSHOP_AGENCY] )
			<< BUILD_LOG( aiCount[KStatistics::SI_ITEM_PICK_UP_PSHOP] )
#endif SERV_PSHOP_AGENCY
			//}}
			<< BUILD_LOG( wstrCurTime )
			<< BUILD_LOG( iServerUID )
			<< BUILD_LOG( iOK );
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.size() )
		<< END_LOG;
}

#endif SERV_ITEM_STATISTICS_TO_DB
//}}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
_IMPL_ON_FUNC( ELOG_STAT_SYSTEM_INFO, KEGS_SYSTEM_INFO )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LClientPerformance_UserPCInfo_INS", L"%d, %d, N\'%s\', N\'%s\', N\'%s\', N\'%s\'",
		% kPacket_.m_iAccountUID
		% kPacket_.m_iSystemKey
		% kPacket_.m_wstrCPU
		% kPacket_.m_wstrMemory
		% kPacket_.m_wstrGPU
		% kPacket_.m_wstrOS
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}

_IMPL_ON_FUNC( ELOG_STAT_KOG_GAME_PERFORMANCE_VILLAGE, KEGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG )
{
	int iOK = NetError::NET_OK;

	//평균프레임, UDP Ping, TCP Ping, 메모리 사용량 측정 결과 DB에 저장
	if( kPacket_.m_fAvgFps != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Village_VillageInfo_INS", L"%d, %d, %f, %f, %d, %f, %f, %f, %f, %d, %d, %d, %d",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_fAvgFps
			% kPacket_.m_fAvgFps_StdDev
			% kPacket_.m_bFirstMemUsageCheck
			% kPacket_.m_fMinWorkingSetSize
			% kPacket_.m_fMaxWorkingSetSize
			% kPacket_.m_fMinPagefileUsage
			% kPacket_.m_fMaxPagefileUsage
			% kPacket_.m_iPartyType
			% kPacket_.m_iPartyUserNum
			% kPacket_.m_iMapID
			% kPacket_.m_iVillageUserNum
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려지지 않은 랙 측정 결과 DB에 저장
	if( kPacket_.m_kUnKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Village_UnknownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kUnKnownLagInfo.m_iLagType
			% kPacket_.m_kUnKnownLagInfo.m_iPartyType
			% kPacket_.m_kUnKnownLagInfo.m_iPartyUserNum
			% kPacket_.m_kUnKnownLagInfo.m_iMapID
			% kPacket_.m_kUnKnownLagInfo.m_iVillageUserNum
			% kPacket_.m_kUnKnownLagInfo.m_fLagTime
			% kPacket_.m_kUnKnownLagInfo.m_fPlayTime
			% kPacket_.m_kUnKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kUnKnownLagInfo.m_fLagTotalTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려진 랙 측정 결과 DB에 저장
	if( kPacket_.m_kKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;
		
		DO_QUERY( L"exec dbo.P_LClientPerformance_Village_knownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kKnownLagInfo.m_iLagType
			% kPacket_.m_kKnownLagInfo.m_iPartyType
			% kPacket_.m_kKnownLagInfo.m_iPartyUserNum
			% kPacket_.m_kKnownLagInfo.m_iMapID
			% kPacket_.m_kKnownLagInfo.m_iVillageUserNum
			% kPacket_.m_kKnownLagInfo.m_fLagTime
			% kPacket_.m_kKnownLagInfo.m_fPlayTime
			% kPacket_.m_kKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kKnownLagInfo.m_fLagTotalTime
		);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}

_IMPL_ON_FUNC( ELOG_STAT_KOG_GAME_PERFORMANCE_FIELD, KEGS_KOG_GAME_PERFORMANCE_FIELD_LOG )
{
	int iOK = NetError::NET_OK;

	//평균프레임, UDP Ping, TCP Ping, 메모리 사용량 측정 결과 DB에 저장
	if( kPacket_.m_fAvgFps != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Field_FieldInfo_INS", 
			L"%d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %f, %f, %f, %f, %d, %d, %d, %d",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_fAvgFps
			% kPacket_.m_fAvgFps_StdDev
			% kPacket_.m_fP2PMinPingTime
			% kPacket_.m_fP2PMaxPingTime
			% kPacket_.m_fP2PAvgPingTime
			% kPacket_.m_fP2PAvgPingTime_StdDev
			% kPacket_.m_fP2PAvgPingLossRate
			% kPacket_.m_fRelayMinPingTime
			% kPacket_.m_fRelayMaxPingTime
			% kPacket_.m_fRelayAvgPingTime
			% kPacket_.m_fRelayAvgPingTime_StdDev
			% kPacket_.m_fRelayAvgPingLossRate
			% kPacket_.m_fUdpMinPingTime
			% kPacket_.m_fUdpMaxPingTime
			% kPacket_.m_fUdpAvgPingTime
			% kPacket_.m_fUdpAvgPingTime_StdDev
			% kPacket_.m_fUdpAvgPingLossRate
			% kPacket_.m_fTcpMinPingTime
			% kPacket_.m_fTcpMaxPingTime
			% kPacket_.m_fTcpAvgPingTime
			% kPacket_.m_fTcpAvgPingTime_StdDev
			% kPacket_.m_fUdpSendCountPerSec_Host
			% kPacket_.m_fUdpSendAmountPerSec_Host
			% kPacket_.m_fUdpRecvCountPerSec_Host
			% kPacket_.m_fUdpRecvAmountPerSec_Host
			% kPacket_.m_fUdpSendCountPerSec_NonHost
			% kPacket_.m_fUdpSendAmountPerSec_NonHost
			% kPacket_.m_fUdpRecvCountPerSec_NonHost
			% kPacket_.m_fUdpRecvAmountPerSec_NonHost
			% kPacket_.m_fUdpUserSyncPacketLossRate
			% kPacket_.m_fUdpNpcSyncPacketLossRate
			% kPacket_.m_fNonHostNpcReaction_HostUdpAvgPingTime			
			% kPacket_.m_fNonHostNpcReaction_MinDelayTime_SimulOff
			% kPacket_.m_fNonHostNpcReaction_MaxDelayTime_SimulOff
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_SimulOff
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff
			% kPacket_.m_fNonHostNpcReaction_MinDelayTime_SimulOn
			% kPacket_.m_fNonHostNpcReaction_MaxDelayTime_SimulOn
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_SimulOn
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn
			% kPacket_.m_bFirstMemUsageCheck
			% kPacket_.m_fMinWorkingSetSize
			% kPacket_.m_fMaxWorkingSetSize
			% kPacket_.m_fMinPagefileUsage
			% kPacket_.m_fMaxPagefileUsage
			% kPacket_.m_iPartyType
			% kPacket_.m_iPartyUserNum
			% kPacket_.m_iMapID
			% kPacket_.m_iFieldUserNum
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려지지 않은 랙 측정 결과 DB에 저장
	if( kPacket_.m_kUnKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Field_UnknownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kUnKnownLagInfo.m_iLagType
			% kPacket_.m_kUnKnownLagInfo.m_iPartyType
			% kPacket_.m_kUnKnownLagInfo.m_iPartyUserNum
			% kPacket_.m_kUnKnownLagInfo.m_iMapID
			% kPacket_.m_kUnKnownLagInfo.m_iFieldUserNum
			% kPacket_.m_kUnKnownLagInfo.m_fLagTime
			% kPacket_.m_kUnKnownLagInfo.m_fPlayTime
			% kPacket_.m_kUnKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kUnKnownLagInfo.m_fLagTotalTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려진 랙 측정 결과 DB에 저장
	if( kPacket_.m_kKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Field_knownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kKnownLagInfo.m_iLagType
			% kPacket_.m_kKnownLagInfo.m_iPartyType
			% kPacket_.m_kKnownLagInfo.m_iPartyUserNum
			% kPacket_.m_kKnownLagInfo.m_iMapID
			% kPacket_.m_kKnownLagInfo.m_iFieldUserNum
			% kPacket_.m_kKnownLagInfo.m_fLagTime
			% kPacket_.m_kKnownLagInfo.m_fPlayTime
			% kPacket_.m_kKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kKnownLagInfo.m_fLagTotalTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}

_IMPL_ON_FUNC( ELOG_STAT_KOG_GAME_PERFORMANCE_DUNGEON, KEGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG )
{
	int iOK = NetError::NET_OK;

	//평균프레임, UDP Ping, TCP Ping, 메모리 사용량 측정 결과 DB에 저장
	if( kPacket_.m_fAvgFps != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Dungeon_DungeonInfo_INS", 
			L"%d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %f, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_fAvgFps
			% kPacket_.m_fAvgFps_StdDev
			% kPacket_.m_fP2PMinPingTime
			% kPacket_.m_fP2PMaxPingTime
			% kPacket_.m_fP2PAvgPingTime
			% kPacket_.m_fP2PAvgPingTime_StdDev
			% kPacket_.m_fP2PAvgPingLossRate
			% kPacket_.m_fRelayMinPingTime//10
			% kPacket_.m_fRelayMaxPingTime
			% kPacket_.m_fRelayAvgPingTime
			% kPacket_.m_fRelayAvgPingTime_StdDev
			% kPacket_.m_fRelayAvgPingLossRate
			% kPacket_.m_fUdpMinPingTime
			% kPacket_.m_fUdpMaxPingTime
			% kPacket_.m_fUdpAvgPingTime
			% kPacket_.m_fUdpAvgPingTime_StdDev
			% kPacket_.m_fUdpAvgPingLossRate
			% kPacket_.m_fTcpMinPingTime//20
			% kPacket_.m_fTcpMaxPingTime
			% kPacket_.m_fTcpAvgPingTime
			% kPacket_.m_fTcpAvgPingTime_StdDev
			% kPacket_.m_fUdpSendCountPerSec_Host
			% kPacket_.m_fUdpSendAmountPerSec_Host
			% kPacket_.m_fUdpRecvCountPerSec_Host
			% kPacket_.m_fUdpRecvAmountPerSec_Host
			% kPacket_.m_fUdpSendCountPerSec_NonHost
			% kPacket_.m_fUdpSendAmountPerSec_NonHost
			% kPacket_.m_fUdpRecvCountPerSec_NonHost//30
			% kPacket_.m_fUdpRecvAmountPerSec_NonHost
			% kPacket_.m_fUdpUserSyncPacketLossRate
			% kPacket_.m_fUdpNpcSyncPacketLossRate
			% kPacket_.m_fNonHostNpcReaction_HostUdpAvgPingTime			
			% kPacket_.m_fNonHostNpcReaction_MinDelayTime_SimulOff
			% kPacket_.m_fNonHostNpcReaction_MaxDelayTime_SimulOff
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_SimulOff
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOff
			% kPacket_.m_fNonHostNpcReaction_MinDelayTime_SimulOn
			% kPacket_.m_fNonHostNpcReaction_MaxDelayTime_SimulOn//40
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_SimulOn
			% kPacket_.m_fNonHostNpcReaction_AvgDelayTime_StdDev_SimulOn
			% kPacket_.m_bFirstMemUsageCheck
			% kPacket_.m_fMinWorkingSetSize
			% kPacket_.m_fMaxWorkingSetSize
			% kPacket_.m_fMinPagefileUsage
			% kPacket_.m_fMaxPagefileUsage
			% kPacket_.m_iPartyType
			% kPacket_.m_iPartyUserNum
			% kPacket_.m_iMapID//50
			% kPacket_.m_iDifficulty
			% kPacket_.m_bEscape
			% kPacket_.m_iStage
			% kPacket_.m_iBlock//54
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려지지 않은 랙 측정 결과 DB에 저장
	if( kPacket_.m_kUnKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Dungeon_UnknownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kUnKnownLagInfo.m_iLagType
			% kPacket_.m_kUnKnownLagInfo.m_iPartyType
			% kPacket_.m_kUnKnownLagInfo.m_iPartyUserNum
			% kPacket_.m_kUnKnownLagInfo.m_iMapID
			% kPacket_.m_kUnKnownLagInfo.m_iDifficulty
			% kPacket_.m_kUnKnownLagInfo.m_iStage
			% kPacket_.m_kUnKnownLagInfo.m_iBlock
			% kPacket_.m_kUnKnownLagInfo.m_fLagTime
			% kPacket_.m_kUnKnownLagInfo.m_fPlayTime
			% kPacket_.m_kUnKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kUnKnownLagInfo.m_fLagTotalTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려진 랙 측정 결과 DB에 저장
	if( kPacket_.m_kKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_Dungeon_knownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kKnownLagInfo.m_iLagType
			% kPacket_.m_kKnownLagInfo.m_iPartyType
			% kPacket_.m_kKnownLagInfo.m_iPartyUserNum
			% kPacket_.m_kKnownLagInfo.m_iMapID
			% kPacket_.m_kKnownLagInfo.m_iDifficulty
			% kPacket_.m_kKnownLagInfo.m_iStage
			% kPacket_.m_kKnownLagInfo.m_iBlock
			% kPacket_.m_kKnownLagInfo.m_fLagTime
			% kPacket_.m_kKnownLagInfo.m_fPlayTime
			% kPacket_.m_kKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kKnownLagInfo.m_fLagTotalTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}

_IMPL_ON_FUNC( ELOG_STAT_KOG_GAME_PERFORMANCE_PVP, KEGS_KOG_GAME_PERFORMANCE_PVP_LOG )
{
	int iOK = NetError::NET_OK;

	//평균프레임, UDP Ping, TCP Ping, 메모리 사용량 측정 결과 DB에 저장
	if( kPacket_.m_fAvgFps != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_PVP_PVPInfo_INS", 
			L"%d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %d, %f, %f, %f, %f, %d, %d, %d, %d, %d",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_fAvgFps
			% kPacket_.m_fAvgFps_StdDev
			% kPacket_.m_fP2PMinPingTime
			% kPacket_.m_fP2PMaxPingTime
			% kPacket_.m_fP2PAvgPingTime
			% kPacket_.m_fP2PAvgPingTime_StdDev
			% kPacket_.m_fP2PAvgPingLossRate
			% kPacket_.m_fRelayMinPingTime
			% kPacket_.m_fRelayMaxPingTime
			% kPacket_.m_fRelayAvgPingTime
			% kPacket_.m_fRelayAvgPingTime_StdDev
			% kPacket_.m_fRelayAvgPingLossRate
			% kPacket_.m_fUdpMinPingTime
			% kPacket_.m_fUdpMaxPingTime
			% kPacket_.m_fUdpAvgPingTime
			% kPacket_.m_fUdpAvgPingTime_StdDev
			% kPacket_.m_fUdpAvgPingLossRate
			% kPacket_.m_fTcpMinPingTime
			% kPacket_.m_fTcpMaxPingTime
			% kPacket_.m_fTcpAvgPingTime
			% kPacket_.m_fTcpAvgPingTime_StdDev
			% kPacket_.m_fUdpSendCountPerSec_Host
			% kPacket_.m_fUdpSendAmountPerSec_Host
			% kPacket_.m_fUdpRecvCountPerSec_Host
			% kPacket_.m_fUdpRecvAmountPerSec_Host
			% kPacket_.m_fUdpSendCountPerSec_NonHost
			% kPacket_.m_fUdpSendAmountPerSec_NonHost
			% kPacket_.m_fUdpRecvCountPerSec_NonHost
			% kPacket_.m_fUdpRecvAmountPerSec_NonHost
			% kPacket_.m_fUdpUserSyncPacketLossRate
			% kPacket_.m_fUdpNpcSyncPacketLossRate
			% kPacket_.m_bFirstMemUsageCheck
			% kPacket_.m_fMinWorkingSetSize
			% kPacket_.m_fMaxWorkingSetSize
			% kPacket_.m_fMinPagefileUsage
			% kPacket_.m_fMaxPagefileUsage
			% kPacket_.m_iMapID
			% kPacket_.m_iPvPUserNum
			% kPacket_.m_iRuleType
			% kPacket_.m_bEscape
			% kPacket_.m_iRound
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려지지 않은 랙 측정 결과 DB에 저장
	if( kPacket_.m_kUnKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_PVP_UnknownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kUnKnownLagInfo.m_iLagType
			% kPacket_.m_kUnKnownLagInfo.m_iMapID
			% kPacket_.m_kUnKnownLagInfo.m_iPvPUserNum
			% kPacket_.m_kUnKnownLagInfo.m_iRuleType
			% kPacket_.m_kUnKnownLagInfo.m_iRound
			% kPacket_.m_kUnKnownLagInfo.m_fLagTime
			% kPacket_.m_kUnKnownLagInfo.m_fPlayTime
			% kPacket_.m_kUnKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kUnKnownLagInfo.m_fLagTotalTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

	//알려진 랙 측정 결과 DB에 저장
	if( kPacket_.m_kKnownLagInfo.m_fLagTime != -1.0f )
	{
		iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_LClientPerformance_PVP_knownLagInfo_INS", L"%d, %d, %d, %d, %d, %d, %d, %f, %f, %d, %f",
			% kPacket_.m_iCharUID
			% kPacket_.m_iSystemKey
			% kPacket_.m_kKnownLagInfo.m_iLagType
			% kPacket_.m_kKnownLagInfo.m_iMapID
			% kPacket_.m_kKnownLagInfo.m_iPvPUserNum
			% kPacket_.m_kKnownLagInfo.m_iRuleType
			% kPacket_.m_kKnownLagInfo.m_iRound
			% kPacket_.m_kKnownLagInfo.m_fLagTime
			% kPacket_.m_kKnownLagInfo.m_fPlayTime
			% kPacket_.m_kKnownLagInfo.m_iLagTotalNum
			% kPacket_.m_kKnownLagInfo.m_fLagTotalTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
IMPL_ON_FUNC( DBE_FIRST_WRITE_SECURITY_TYPE_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LSecurityTypeLog_INS", L"%d, %d, %d, %d, N\'%s\'", 
				% KBaseServer::GetKObj()->GetServerGroupID()
				% kPacket_.m_iUserUID
				% kPacket_.m_iSecurityType
				% kPacket_.m_iAge
				% kPacket_.m_wstrCurrTime);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_securitytype_log", L"%d, %d, %d, N\'%s\'", 
				% kPacket_.m_iUserUID
				% kPacket_.m_iSecurityType
				% kPacket_.m_iAge
				% kPacket_.m_wstrCurrTime);
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2차 보안 타입 기록 로그 SP 호출 실패!!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iSecurityType )
			<< BUILD_LOG( kPacket_.m_iAge )
			<< BUILD_LOG( kPacket_.m_wstrCurrTime )
			<< END_LOG;
	}	
}
#endif SERV_SECOND_SECURITY
//}}

#ifdef	SERV_SHARING_BANK_TEST
_IMPL_ON_FUNC( ELOG_UPDATE_SHARE_ITEM_NOT, std::vector< KTradeShareItemInfo > )
{
	BOOST_TEST_FOREACH( const KTradeShareItemInfo&, kTradeShareItemInfo, kPacket_ )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LTradeBank_INS", L"%d, %d, %d, %d, %d, %d, %d",
			% GetKGameServer()->GetServerGroupID()
			% kTradeShareItemInfo.m_iFromUnitUID
			% kTradeShareItemInfo.m_iToUnitUID
			% kTradeShareItemInfo.m_iItemUID
			% kTradeShareItemInfo.m_iNewItemUID
			% kTradeShareItemInfo.m_iItemID
			% static_cast<int>(kTradeShareItemInfo.m_ucSealCnt)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LTradeBank_INT", L"%d, %d, %d, %d, %d, %d, %d",
			% GetKGameServer()->GetServerGroupID()
			% kTradeShareItemInfo.m_iFromUnitUID
			% kTradeShareItemInfo.m_iToUnitUID
			% kTradeShareItemInfo.m_iItemUID
			% kTradeShareItemInfo.m_iNewItemUID
			% kTradeShareItemInfo.m_iItemID
			% static_cast<int>(kTradeShareItemInfo.m_ucSealCnt)
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			/*
			START_LOG( cerr, L"은행 공유 : 로그 입력 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( GetKGameServer()->GetServerGroupID() )
				<< BUILD_LOG( kTradeShareItemInfo.m_iFromUnitUID )
				<< BUILD_LOG( kTradeShareItemInfo.m_iToUnitUID )
				<< BUILD_LOG( kTradeShareItemInfo.m_iItemUID )
				<< BUILD_LOG( kTradeShareItemInfo.m_iNewItemUID )
				<< BUILD_LOG( kTradeShareItemInfo.m_iItemID )
				<< BUILD_LOG( kTradeShareItemInfo.m_ucSealCnt )
				<< END_LOG;
				*/
		}
	}

	goto end_proc;

end_proc:
	return;
}

IMPL_ON_FUNC( ELOG_ITEM_TRADE_ERROR_NOT )
{
	int iOK = NetError::NET_OK;

	DO_QUERY( L"exec dbo.P_LItemTradeError_INT", L"%d, %d, %s",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iType
		% kPacket_.m_wsrtRegDate
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"은행 공유 : 에러 로그 입력 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iType )
			<< END_LOG;
	}

	goto end_proc;

end_proc:
	return;
}
#endif	SERV_SHARING_BANK_TEST

#ifdef SERV_GOLD_TICKET
IMPL_ON_FUNC( ELOG_CHARGE_POINT_NOT )
{
	START_LOG(cwarn, L"지헌로그-테스트 : 골드티켓 로그, 어디서 UID에 쓰래기값이 들어가니")
		<< BUILD_LOG(kPacket_.m_iItemUID)
		<< END_LOG;

	// 골드 티켓 사용 실패 로그 남깁시다.
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LChargeCashITem_INS", L"%d, %d, %d, %d, %d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iCharUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iItemID
		% kPacket_.m_iChargedCash
		% kPacket_.m_iNowCash
		% kPacket_.m_iOK
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iCharUID )
		<< BUILD_LOG( kPacket_.m_iItemUID )
		<< BUILD_LOG( kPacket_.m_iItemID )
		<< BUILD_LOG( kPacket_.m_iChargedCash )
		<< BUILD_LOG( kPacket_.m_iNowCash )
		<< BUILD_LOG( kPacket_.m_iOK )
		<< END_LOG;
}
#endif //SERV_GOLD_TICKET 

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
IMPL_ON_FUNC( DBE_FIRST_WRITE_COME_BACK_INFO_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LComebackUserLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% KBaseServer::GetKObj()->GetServerGroupID() 
			% kPacket_.m_iUserUID
			% kPacket_.m_iAge
			% kPacket_.m_iUnitUID
			% kPacket_.m_iUnitClass
			% kPacket_.m_iUnitLevel
			% kPacket_.m_bTodayFirst
			% kPacket_.m_iRewardLevel
			% kPacket_.m_wsrtRegDate  );
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_comebackuser_log", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% kPacket_.m_iUserUID
			% kPacket_.m_iAge
			% kPacket_.m_iUnitUID
			% kPacket_.m_iUnitClass
			% kPacket_.m_iUnitLevel
			% kPacket_.m_bTodayFirst
			% kPacket_.m_iRewardLevel
			% kPacket_.m_wsrtRegDate  );
#endif //SERV_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"휴면 복귀 유저 정보 DB 입력 실패!!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iAge )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUnitClass )
			<< BUILD_LOG( kPacket_.m_iUnitLevel )
			<< BUILD_LOG( kPacket_.m_bTodayFirst )
			<< BUILD_LOG( kPacket_.m_iRewardLevel )
			<< BUILD_LOG( kPacket_.m_wsrtRegDate )
			<< END_LOG;
	}	
}
#endif SERV_COME_BACK_USER_REWARD
//}} 

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC( DBE_INSERT_LOG_RETAINING_USER_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LRetainLog_INS", L"%d, %d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_wstrRegDate
		);
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_retain_log", L"%d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
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
			START_LOG( cerr, L"이탈 방지 보상 로그 입력 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< BUILD_LOG( kPacket_.m_wstrRegDate )
				<< END_LOG;
		}
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}

//{{  2011.11.08     김민성    강화 로그
#ifdef SERV_ENCHANT_ITEM_LOG
IMPL_ON_FUNC( ELOG_HIGH_ENCHANT_ITEM_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LEnchantLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iItemID
		% kPacket_.m_bSupportMaterial
		% kPacket_.m_iBeforeLevel
		% kPacket_.m_iAfterLevel
		% kPacket_.m_iEnchantResult
		% kPacket_.m_wstrTime
		)
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_insert_enchant_log_new", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% kPacket_.m_iItemID
		% kPacket_.m_bSupportMaterial
		% kPacket_.m_iBeforeLevel
		% kPacket_.m_iAfterLevel
		% kPacket_.m_iEnchantResult
		% kPacket_.m_wstrTime
		)
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"고강 아이템 강화 로그 기록 실패" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< BUILD_LOGc( kPacket_.m_iBeforeLevel )
				<< BUILD_LOGc( kPacket_.m_iAfterLevel )
				<< BUILD_LOG( kPacket_.m_bSupportMaterial )
				<< BUILD_LOGc( kPacket_.m_iEnchantResult )
				<< END_LOG;
		}
}
#endif SERV_ENCHANT_ITEM_LOG
//}}

//{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
IMPL_ON_FUNC( DBE_UNIT_CONNECT_LOG_NOT )
{
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LUnitConnectLog_INS", L"%d, %d, %d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, %d", 
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iServerUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrName
		% kPacket_.m_iUnitClass
		% kPacket_.m_iExp
		% kPacket_.m_iLevel
		% kPacket_.m_wstrLogin
		% kPacket_.m_wstrLogout
		% kPacket_.m_iPlayTime
		% kPacket_.m_bPcBang
		% static_cast<int>(kPacket_.m_cAuthLevel)
		% kPacket_.m_iLoginLevel
		)
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LUnitConnectLog_INT", L"%d, %d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, %d", 
		% kPacket_.m_iServerUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrName
		% kPacket_.m_iUnitClass
		% kPacket_.m_iExp
		% kPacket_.m_iLevel
		% kPacket_.m_wstrLogin
		% kPacket_.m_wstrLogout
		% kPacket_.m_iPlayTime
		% kPacket_.m_bPcBang
		% static_cast<int>(kPacket_.m_cAuthLevel)
		% kPacket_.m_iLoginLevel
		)
#endif //SERV_RENEWAL_SP
	
	START_LOG( clog, L"케릭터 단위 로그 추가" )
		<< BUILD_LOG( kPacket_.m_iServerUID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrName )
		<< BUILD_LOG( kPacket_.m_iUnitClass )
		<< BUILD_LOG( kPacket_.m_iExp )
		<< BUILD_LOG( kPacket_.m_iLevel )
		<< BUILD_LOG( kPacket_.m_wstrLogin )
		<< BUILD_LOG( kPacket_.m_wstrLogout )
		<< BUILD_LOG( kPacket_.m_iPlayTime )
		<< BUILD_LOG( kPacket_.m_bPcBang )
		<< BUILD_LOGc( kPacket_.m_cAuthLevel )
		<< BUILD_LOGc( kPacket_.m_iLoginLevel )
		<< END_LOG;
end_proc:;
}
#endif SERV_CHAR_CONNECT_LOG
//}}

//{{ 2011. 12. 02  김민성	AP 구매 로그 수치
#ifdef SERV_BUY_AP_ITEM_LOG
IMPL_ON_FUNC( DBE_BUY_AP_ITEM_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LApointLog_INS", L"%d, %d, %d, %d, %d, %d, N\'%s\'", 
		% KBaseServer::GetKObj()->GetServerGroupID() 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iQuantity
		% kPacket_.m_iBeforAP
		% kPacket_.m_iAfterAP
		% kPacket_.m_wstrBuyDate
		)
#else //SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LApointLog_INT", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemID
		% kPacket_.m_iQuantity
		% kPacket_.m_iBeforAP
		% kPacket_.m_iAfterAP
		% kPacket_.m_wstrBuyDate
		)
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"AP 아이템 구매 로그 기록 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< BUILD_LOG( kPacket_.m_iQuantity )
				<< BUILD_LOG( kPacket_.m_iBeforAP )
				<< BUILD_LOG( kPacket_.m_iAfterAP )
				<< BUILD_LOG( kPacket_.m_wstrBuyDate )
				<< END_LOG;
		}
}
#endif SERV_BUY_AP_ITEM_LOG
//}}

//{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
IMPL_ON_FUNC( DBE_UPDATE_PAKCET_MORNITORING_LOG_NOT )
{
	CTime tCurTime = CTime::GetCurrentTime();
	const std::wstring wstrCurTime = (std::wstring)tCurTime.Format( _T("%Y-%m-%d %H:%M:%S") );
	const UidType iServerUID = KBaseServer::GetKObj()->GetUID();

	BOOST_TEST_FOREACH( const KPacketProcessingLatencyInfo&, kInfo, kPacket_.m_vecGSUserPacketLatencyInfo )
	{
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LServer_GSUser_Packet_Latency_INS", L"%d, %d, N\'%s\', %d, N\'%s\', N\'%s\', %d", 
			% KBaseServer::GetKObj()->GetServerGroupID()
			% iServerUID
			% KEvent::GetIDStr( kInfo.m_usEventID )
			% kInfo.m_dwMaxLatency
			% kInfo.m_wstrMaxLatencyTime
			% wstrCurTime
			% kInfo.m_iUnitUID
			)
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LServer_GSUser_Packet_Latency_INT", L"%d, N\'%s\', %d, N\'%s\', N\'%s\', %d", 
			% iServerUID
			% KEvent::GetIDStr( kInfo.m_usEventID )
			% kInfo.m_dwMaxLatency
			% kInfo.m_wstrMaxLatencyTime
			% wstrCurTime
			% kInfo.m_iUnitUID
			)
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"AP 아이템 구매 로그 기록 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iServerUID )
				<< BUILD_LOG( kInfo.m_usEventID )
				<< BUILD_LOG( kInfo.m_dwMaxLatency )
				<< BUILD_LOG( kInfo.m_wstrMaxLatencyTime )
				<< BUILD_LOG( wstrCurTime )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< END_LOG;
		}
	}

	START_LOG( cout, L"--------------------- Login Proxy Packet count ---------------------" );

	BOOST_TEST_FOREACH( const KPacketProcessingCountInfo&, kInfo, kPacket_.m_vecLoginProxyPacketCountInfo )
	{
		START_LOG( cout, KEvent::GetIDStr( kInfo.m_usEventID ) << L" : " << kInfo.m_dwCount );

		//int iOK = NetError::ERR_ODBC_01;

		// 주석 해제 하면 P_LServer_CnProxy_Packet_Count_INS SP 사용해야함
		//DO_QUERY( L"exec dbo.P_LServer_CnProxy_Packet_Count_INT", L"%d, N\'%s\', %d, N\'%s\'", 
		//	% iServerUID
		//	% KEvent::GetIDStr( kInfo.m_usEventID )
		//	% kInfo.m_dwCount
		//	% wstrCurTime
		//	)

		//if( m_kODBC.BeginFetch() )
		//{
		//	FETCH_DATA( iOK );
		//	m_kODBC.EndFetch();
		//}

		//if( iOK != NetError::NET_OK )
		//{
		//	START_LOG( cerr, L"AP 아이템 구매 로그 기록 실패!" )
		//		<< BUILD_LOG( iOK )
		//		<< BUILD_LOG( iServerUID )
		//		<< BUILD_LOG( kInfo.m_usEventID )
		//		<< BUILD_LOG( kInfo.m_dwCount )
		//		<< BUILD_LOG( wstrCurTime )
		//		<< END_LOG;
		//}
	}

	BOOST_TEST_FOREACH( const KPacketProcessingCountInfo&, kInfo, kPacket_.m_vecCnProxyPacketCountInfo )
	{
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LServer_CnProxy_Packet_Count_INS", L"%d, %d, N\'%s\', %d, N\'%s\'", 
			% KBaseServer::GetKObj()->GetServerGroupID() 
			% iServerUID
			% KEvent::GetIDStr( kInfo.m_usEventID )
			% kInfo.m_dwCount
			% wstrCurTime
			)
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LServer_CnProxy_Packet_Count_INT", L"%d, N\'%s\', %d, N\'%s\'", 
			% iServerUID
			% KEvent::GetIDStr( kInfo.m_usEventID )
			% kInfo.m_dwCount
			% wstrCurTime
			)
#endif //SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"AP 아이템 구매 로그 기록 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iServerUID )
				<< BUILD_LOG( kInfo.m_usEventID )
				<< BUILD_LOG( kInfo.m_dwCount )
				<< BUILD_LOG( wstrCurTime )
				<< END_LOG;
		}
	}

	START_LOG( cout, L"--------------------- Global Proxy Packet count ---------------------" );

	BOOST_TEST_FOREACH( const KPacketProcessingCountInfo&, kInfo, kPacket_.m_vecGlobalProxyPacketCountInfo )
	{
		START_LOG( cout, KEvent::GetIDStr( kInfo.m_usEventID ) << L" : " << kInfo.m_dwCount );

		//int iOK = NetError::ERR_ODBC_01;

		// 주석 해제 하면 P_LServer_CnProxy_Packet_Count_INS SP 사용해야함
		//DO_QUERY( L"exec dbo.P_LServer_CnProxy_Packet_Count_INT", L"%d, N\'%s\', %d, N\'%s\'", 
		//	% iServerUID
		//	% KEvent::GetIDStr( kInfo.m_usEventID )
		//	% kInfo.m_dwCount
		//	% wstrCurTime
		//	)

		//if( m_kODBC.BeginFetch() )
		//{
		//	FETCH_DATA( iOK );
		//	m_kODBC.EndFetch();
		//}

		//if( iOK != NetError::NET_OK )
		//{
		//	START_LOG( cerr, L"AP 아이템 구매 로그 기록 실패!" )
		//		<< BUILD_LOG( iOK )
		//		<< BUILD_LOG( iServerUID )
		//		<< BUILD_LOG( kInfo.m_usEventID )
		//		<< BUILD_LOG( kInfo.m_dwCount )
		//		<< BUILD_LOG( wstrCurTime )
		//		<< END_LOG;
		//}
	}
	
	return;

end_proc:
	START_LOG( cerr, L"패킷 처리 관련 로그 DB기록 실패!" )
		<< BUILD_LOG( kPacket_.m_vecCnProxyPacketCountInfo.size() )
		<< BUILD_LOG( kPacket_.m_vecGSUserPacketLatencyInfo.size() )
		<< END_LOG;
}
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}

//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
IMPL_ON_FUNC( DBE_CHECK_MODULE_INFO_LOG_NOT )
{

	BOOST_TEST_FOREACH( const KModuleInfo&, kInfo, kPacket_.m_vecMonitoringDLL )
	{
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LUserDLLLog_INS", L"%d, %d, %d, N\'%s\', %d", 
				% KBaseServer::GetKObj()->GetServerGroupID()
				% kPacket_.m_iUserUID
				% kPacket_.m_iServerSN
				% kInfo.m_wstrModuleName.c_str()
				% kInfo.m_iAllow )
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LUserDLLLog_INT", L"%d, %d, N\'%s\', %d", 
				% kPacket_.m_iUserUID
				% kPacket_.m_iServerSN
				% kInfo.m_wstrModuleName.c_str()
				% kInfo.m_iAllow )
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"해킹 프로세스 검사 기능 로그 기록 실패!" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kInfo.m_wstrModuleName )
					<< BUILD_LOG( kInfo.m_iAllow )
					<< END_LOG;
			}
	}

	BOOST_TEST_FOREACH( const KModuleInfo&, kInfo, kPacket_.m_vecBlackListDLL )
	{
		int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LUserDLLLog_INS", L"%d, %d, %d, N\'%s\', %d", 
			% KBaseServer::GetKObj()->GetServerGroupID()
			% kPacket_.m_iUserUID
			% kPacket_.m_iServerSN
			% kInfo.m_wstrModuleName.c_str()
			% kInfo.m_iAllow )
#else //SERV_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_LUserDLLLog_INT", L"%d, %d, N\'%s\', %d", 
			% kPacket_.m_iUserUID
			% kPacket_.m_iServerSN
			% kInfo.m_wstrModuleName.c_str()
			% kInfo.m_iAllow )
#endif //SERV_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"해킹 프로세스 검사 기능 로그 기록 실패!" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kInfo.m_wstrModuleName )
					<< BUILD_LOG( kInfo.m_iAllow )
					<< END_LOG;
			}
	}
	return;

end_proc:
	START_LOG( cerr, L"해킹 프로세스 검사 기능 로그 기록 실패!" )
		<< BUILD_LOG( kPacket_.m_vecMonitoringDLL.size() )
		<< BUILD_LOG( kPacket_.m_vecBlackListDLL.size() )
		<< END_LOG;
}
#endif SERV_HACKING_DLL_CHECK
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
			>> kInfo.m_wstrPrivateIP
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

//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
IMPL_ON_FUNC( DBE_MODULE_EXIST_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_00;
	std::map<std::wstring, bool>::iterator mit = kPacket_.m_mapModule.begin();
	for( ; mit != kPacket_.m_mapModule.end() ; ++mit )
	{
		if( mit->second == false )
		{
#ifdef SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LNonExistentModuleLog_INS", L"%d, %d, %d, N\'%s\', N\'%s\'",
				% % KBaseServer::GetKObj()->GetServerGroupID()
				% kPacket_.m_iServerSN
				% kPacket_.m_iUserUID
				% mit->first.c_str()
				% kPacket_.m_wstrRegDate.c_str() )
#else //SERV_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_LNonExistentModuleLog_INS", L"%d, %d, N\'%s\', N\'%s\'",
				% kPacket_.m_iServerSN
				% kPacket_.m_iUserUID
				% mit->first.c_str()
				% kPacket_.m_wstrRegDate.c_str() )
#endif //SERV_RENEWAL_SP

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"필수 모듈 로그 기록 실패!" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_iUserUID )
						<< BUILD_LOG( mit->first )
						<< BUILD_LOG( kPacket_.m_wstrRegDate )
						<< END_LOG;
				}
		}
	}
	return;
end_proc:
	START_LOG( cerr, L"필수 모듈 로그 기록 실패!" )
		<< BUILD_LOG( kPacket_.m_iServerSN )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< END_LOG;
}
#endif SERV_CHECK_X2_EXE_MODULE
//}}

//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
IMPL_ON_FUNC_NOPARAM( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_REQ )
{
	KDBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK kPacket;

//end_proc:
	SendToServer( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK, kPacket );
}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
_IMPL_ON_FUNC( DBE_BAD_ATTITUDE_USER_LOG_NOT, KERM_BAD_ATTITUDE_USER_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_00;

#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LExitLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iUnitUID
		% kPacket_.m_iDungeonIDAndDif
		% kPacket_.m_sStartMember
		% kPacket_.m_sNowMember
		% kPacket_.m_sVoteCount
		% kPacket_.m_sExitStage
		% kPacket_.m_sExitSubStage
		% kPacket_.m_sBadAttitudePoint
		% kPacket_.m_wstrRegDate.c_str()
		% kPacket_.m_bExitResult )
#else // SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LExitLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iDungeonIDAndDif
		% kPacket_.m_sStartMember
		% kPacket_.m_sNowMember
		% kPacket_.m_sVoteCount
		% kPacket_.m_sExitStage
		% kPacket_.m_sExitSubStage
		% kPacket_.m_sBadAttitudePoint
		% kPacket_.m_wstrRegDate.c_str()
		% kPacket_.m_bExitResult )
#endif // SERV_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"던전 강퇴 로그 입력 실패" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iDungeonIDAndDif )
				<< BUILD_LOG( kPacket_.m_sStartMember )
				<< BUILD_LOG( kPacket_.m_sNowMember )
				<< BUILD_LOG( kPacket_.m_sVoteCount )
				<< BUILD_LOG( kPacket_.m_sExitStage )
				<< BUILD_LOG( kPacket_.m_sExitSubStage )
				<< BUILD_LOG( kPacket_.m_sBadAttitudePoint )
				<< BUILD_LOG( kPacket_.m_wstrRegDate )
				<< BUILD_LOG( kPacket_.m_bExitResult )
				<< END_LOG;
		}
	return;

end_proc:
	START_LOG( cerr, L"던전 강퇴 로그 입력 실패" )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iDungeonIDAndDif )
		<< BUILD_LOG( kPacket_.m_sStartMember )
		<< BUILD_LOG( kPacket_.m_sNowMember )
		<< BUILD_LOG( kPacket_.m_sVoteCount )
		<< BUILD_LOG( kPacket_.m_sExitStage )
		<< BUILD_LOG( kPacket_.m_sExitSubStage )
		<< BUILD_LOG( kPacket_.m_sBadAttitudePoint )
		<< BUILD_LOG( kPacket_.m_wstrRegDate )
		<< BUILD_LOG( kPacket_.m_bExitResult )
		<< END_LOG;
};
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

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

//{{ 허상형 : [2010/8/31/] //	부활석 통계 분리 작업
#ifdef SERV_SELECT_UNIT_NEW
IMPL_ON_FUNC( ELOG_UPDATE_STONE_NOT )
{
	int iOK;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_StatsStoneCnt_MER", L"%d, %d, %d",
		% KBaseServer::GetKObj()->GetServerGroupID()
		% kPacket_.m_iSupplyCnt
		% kPacket_.m_iQuantity
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.lup_update_stone_cnt", L"%d, %d",
		% kPacket_.m_iSupplyCnt
		% kPacket_.m_iQuantity
		);
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( kPacket_.m_iSupplyCnt )
		<< BUILD_LOG( kPacket_.m_iQuantity )
		<< END_LOG;
}
#endif	//	SERV_SELECT_UNIT_NEW
//}} 허상형 : [2010/8/31/] //	부활석 통계 분리 작업

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
IMPL_ON_FUNC( DBE_SYNTHESIS_LOG_NOT )
{
	UidType iBeforeUID[4] = {0,};
	int i = 0;

	for(std::map<int,UidType>::iterator it = kPacket_.m_mapSynthesisData.begin(); it !=kPacket_.m_mapSynthesisData.end(); ++it)
	{
		iBeforeUID[i] = it->second;
		i++;
	}

	std::map< UidType, KItemInfo >::const_iterator mit = kPacket_.m_mapInsertedItemInfo.begin(); 

	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.lup_insert_LItemSynthesis", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% 0
		% kPacket_.m_iUnitUID
		% kPacket_.m_iLevel
		% iBeforeUID[0]
		% iBeforeUID[1]
		% iBeforeUID[2]
		% iBeforeUID[3]
		% kPacket_.m_iKeyUID
			% mit->first
			% mit->second.m_iItemID
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		return;
}
#endif SERV_SYNTHESIS_AVATAR
//}}

#ifdef SERV_CATCH_HACKUSER_INFO
_IMPL_ON_FUNC( DBE_CATCH_HACKUSER_INTO_NOT, KEGS_CATCH_HACKUSER_INFO_NOT )
{
	int iOK = NetError::NET_OK;

	DO_QUERY( L"exec dbo.lup_insert_catch_hackuser_info", L"%d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iCrashType
		);

	if( m_kODBC.BeginFetch() )
	{
		m_kODBC.EndFetch();
	}
end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}
#endif SERV_CATCH_HACKUSER_INFO

//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
IMPL_ON_FUNC( DBE_ITEM_EXCHANGE_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	std::map<int, int>::iterator mit = kPacket_.m_mapResultItem.begin();
	for( ; mit != kPacket_.m_mapResultItem.end() ; ++mit )
	{
		DO_QUERY( L"exec dbo.P_LExchangeLog_INS", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% kPacket_.m_iUnitUID
			% kPacket_.m_iUserUID
			% kPacket_.m_iSourceItemID
			% static_cast<short>(kPacket_.m_iSourceItemQuantity)
			% mit->first
			% static_cast<short>(mit->second)
			% (int)kPacket_.m_cExchangeType
			% kPacket_.m_wstrRegDate
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"교환 결과 로그 기록 실패" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_iSourceItemID )
				<< BUILD_LOG( kPacket_.m_iSourceItemQuantity )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
				<< BUILD_LOGc( kPacket_.m_cExchangeType )
				<< BUILD_LOG( kPacket_.m_wstrRegDate )
				<< END_LOG;
		}
end_proc:;
	}	
}
#endif SERV_EXCHANGE_LOG
//}

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
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
IMPL_ON_FUNC( DBE_GET_ITEM_ONOFF_NPCSHOP_REQ )
{
	KDBE_GET_ITEM_ONOFF_NPCSHOP_ACK kPacket;
	kPacket.m_mapGetItemOnOff.clear();
	int iReleaseTickType = 0;
	int iReleaseTick = 0;

	std::map<int, int> mapReleaseTick;
	mapReleaseTick.clear();

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

		if ( iReleaseTickType == KBaseServer::TCIRTT_TIME_CONTROL_ITEM_CHECK )
		{
			mapReleaseTick.insert( std::make_pair( iReleaseTickType, iReleaseTick ) );
		}
	}

	std::map<int, int>::const_iterator cmitTimeControlItem = mapReleaseTick.find( KBaseServer::TCIRTT_TIME_CONTROL_ITEM_CHECK );
	std::map<int, int>::const_iterator cmitTimeControlItemREQ = kPacket_.m_mapTimeControlItem_StaticDBReleaseTick.find( KBaseServer::TCIRTT_TIME_CONTROL_ITEM_CHECK );

	if ( cmitTimeControlItem != mapReleaseTick.end() && cmitTimeControlItemREQ != kPacket_.m_mapTimeControlItem_StaticDBReleaseTick.end() )
	{
		if ( cmitTimeControlItem->second != cmitTimeControlItemREQ->second )
		{
			
			KPacketGetItemOnOff m_kPacketGItemBan;
			std::wstring m_wstrSecond = L":00";

			DO_QUERY_NO_ARG( L"exec dbo.P_LItemOnOff_SEL"); // 2012. 12.11 lygan_조성욱 // 해당 SP 명 통계 디비 이름 규칙에 맞게 변경해야 함 ( 지금은 게임 디비에 있는거 우선 사용 )
			while ( m_kODBC.Fetch() )
			{
				FETCH_DATA( m_kPacketGItemBan.m_iType 
					>> m_kPacketGItemBan.m_iItemID
					>> m_kPacketGItemBan.m_wstrStartTime
					>> m_kPacketGItemBan.m_wstrEndTime				
					);

				m_kPacketGItemBan.m_wstrStartTime += m_wstrSecond;
				m_kPacketGItemBan.m_wstrEndTime += m_wstrSecond;
		
				std::map<int, std::vector<KPacketGetItemOnOff> >::iterator mit = kPacket.m_mapGetItemOnOff.find(m_kPacketGItemBan.m_iType);
				if ( mit != kPacket.m_mapGetItemOnOff.end() )
				{
					
					mit->second.push_back(m_kPacketGItemBan);
				}
				else
				{
					std::vector<KPacketGetItemOnOff> vecGetItemOnOff;
					vecGetItemOnOff.push_back(m_kPacketGItemBan);
					kPacket.m_mapGetItemOnOff.insert(std::make_pair(m_kPacketGItemBan.m_iType, vecGetItemOnOff));
				}
			}
		}
	}
end_proc:
	kPacket.m_mapTimeControlItem_StaticDBReleaseTick = mapReleaseTick;
	SendToServer( DBE_GET_ITEM_ONOFF_NPCSHOP_ACK, kPacket );
}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
IMPL_ON_FUNC( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ )
{

	KDBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK kPacket;

	kPacket.m_mapGetCubeInItemMappingOnOff.clear();

	int iReleaseTickType = 0;
	int iReleaseTick = 0;


	std::map<int, int> mapReleaseTick;
	mapReleaseTick.clear();

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

		if ( iReleaseTickType == KBaseServer::TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING )
		{
			mapReleaseTick.insert( std::make_pair( iReleaseTickType, iReleaseTick ) );
		}
		
	}


	std::map<int, int>::const_iterator cmitTimeControlCubeInItemMapping = mapReleaseTick.find( KBaseServer::TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING );
	std::map<int, int>::const_iterator cmitTimeControlCubeInItemMappingREQ = kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find( KBaseServer::TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING );

	if ( cmitTimeControlCubeInItemMapping != mapReleaseTick.end() && cmitTimeControlCubeInItemMappingREQ != kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end() )
	{
		if ( cmitTimeControlCubeInItemMapping->second != cmitTimeControlCubeInItemMappingREQ->second )
		{
			
			KRandomItemMappingToInfoServer m_kPacketGCubeInItemMapping;
			std::wstring m_wstrSecond = L":00";	 // DB에서 분까지만 입력 받음.

			DO_QUERY_NO_ARG( L"exec dbo.P_ECubeGroupChange_SEL"); // 김석근 SP 수정해야 할 장소
			while ( m_kODBC.Fetch() )
			{
				FETCH_DATA( m_kPacketGCubeInItemMapping.m_iIndex 
					>> m_kPacketGCubeInItemMapping.m_iCubeID
					>> m_kPacketGCubeInItemMapping.m_iKeyItemID
					>> m_kPacketGCubeInItemMapping.m_iBeforeGroupID
					>> m_kPacketGCubeInItemMapping.m_iAfterGroupID
					>> m_kPacketGCubeInItemMapping.m_tStartTime
					>> m_kPacketGCubeInItemMapping.m_tEndTime				
					);

				//START_LOG( cerr, L"P_ECubeGroupChange_SEL" )
				//	<< BUILD_LOG( m_kPacketGCubeInItemMapping.m_iIndex )
				//	<< BUILD_LOG( m_kPacketGCubeInItemMapping.m_iCubeID )
				//	<< BUILD_LOG( m_kPacketGCubeInItemMapping.m_iKeyItemID )
				//	<< BUILD_LOG( m_kPacketGCubeInItemMapping.m_iBeforeGroupID )
				//	<< BUILD_LOG( m_kPacketGCubeInItemMapping.m_iAfterGroupID )
				//	<< BUILD_LOG( m_kPacketGCubeInItemMapping.m_tStartTime )
				//	<< BUILD_LOG( m_kPacketGCubeInItemMapping.m_tEndTime )
				//	<< BUILD_LOG( cmitTimeControlCubeInItemMapping->second )
				//	<< BUILD_LOG( cmitTimeControlCubeInItemMappingREQ->second )
				//	<< END_LOG;

				m_kPacketGCubeInItemMapping.m_tStartTime += m_wstrSecond;
				m_kPacketGCubeInItemMapping.m_tEndTime += m_wstrSecond;
		

				std::map<int, std::vector<KRandomItemMappingToInfoServer> >::iterator mit = kPacket.m_mapGetCubeInItemMappingOnOff.find(m_kPacketGCubeInItemMapping.m_iIndex);
				if ( mit != kPacket.m_mapGetCubeInItemMappingOnOff.end() )
				{
					
					mit->second.push_back(m_kPacketGCubeInItemMapping);
				}
				else
				{
					std::vector<KRandomItemMappingToInfoServer> vecGetCubeInItemMappingOnOff;
					vecGetCubeInItemMappingOnOff.push_back(m_kPacketGCubeInItemMapping);

					kPacket.m_mapGetCubeInItemMappingOnOff.insert(std::make_pair(m_kPacketGCubeInItemMapping.m_iIndex, vecGetCubeInItemMappingOnOff));
				}

			}

		}

	}
end_proc:
	kPacket.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick = mapReleaseTick;
	SendToServer( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK, kPacket );
}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

#ifdef SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
IMPL_ON_FUNC( DBE_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LPortLog_INT", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUDPPortSuccessType
		% kPacket_.m_iUseUdpPort );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"유저 UDP 포트 사용 정보 변경 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUDPPortSuccessType )
			<< BUILD_LOG( kPacket_.m_iUseUdpPort )
			<< END_LOG;
	}
}
#endif //SERV_CLIENT_PORT_CHANGE_REQUEST_LOG

#ifdef SERV_SKILL_POINT_CORRECTION
IMPL_ON_FUNC( DBE_SKILL_POINT_CORRECTION_INFO_NOT )
{	 
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LSkillPointLog_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iServerGroupID
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUnitClass
		% kPacket_.m_sLevel
		% kPacket_.m_iSPoint
		% kPacket_.m_iUsedSPoint
		% kPacket_.m_iCSPoint
		% kPacket_.m_iUsedCSPoint
		% kPacket_.m_iMaxCSPoint
		% kPacket_.m_iCorrectionSP		
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LSkillPointLog_INT", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iServerGroupID
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUnitClass
		% kPacket_.m_sLevel
		% kPacket_.m_iSPoint
		% kPacket_.m_iUsedSPoint
		% kPacket_.m_iCSPoint
		% kPacket_.m_iUsedCSPoint
		% kPacket_.m_iMaxCSPoint
		% kPacket_.m_iCorrectionSP		
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"스킬 포인트 보정 로그 기록 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iServerGroupID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUnitClass )
			<< BUILD_LOG( kPacket_.m_sLevel )
			<< BUILD_LOG( kPacket_.m_iSPoint )
			<< BUILD_LOG( kPacket_.m_iUsedSPoint )
			<< BUILD_LOG( kPacket_.m_iCSPoint )
			<< BUILD_LOG( kPacket_.m_iUsedCSPoint )
			<< BUILD_LOG( kPacket_.m_iMaxCSPoint )
			<< BUILD_LOG( kPacket_.m_iCorrectionSP )
			<< END_LOG;
	}
}
#endif //SERV_SKILL_POINT_CORRECTION

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ )
{
	KDBE_WRITE_RELATIONSHIP_INFO_LOG_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCoupleSystemLog_INS", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_ucLevel)
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_cCoupleType)
		% kPacket_.m_wstrRegDate	);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LCoupleSystemLog_INT", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_ucLevel)
		% static_cast<int>(kPacket_.m_cUnitClass)
		% static_cast<int>(kPacket_.m_cCoupleType)
		% kPacket_.m_wstrRegDate	);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_WRITE_RELATIONSHIP_INFO_LOG_ACK, kPacket );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}}

#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
IMPL_ON_FUNC( DBE_LOCAL_RANKING_SYSTEM_STATISTICS_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LFriendSystemClickLog_INS", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_ucLevel
		% kPacket_.m_bLocalRankingUser
		% kPacket_.m_usLocalRankingClickCount
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LFriendSystemClickLog_INT", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_ucLevel
		% kPacket_.m_bLocalRankingUser
		% kPacket_.m_usLocalRankingClickCount
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"로컬 랭킹 통계 로그 수집 실패" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_ucLevel )
			<< BUILD_LOG( kPacket_.m_bLocalRankingUser )
			<< BUILD_LOG( kPacket_.m_usLocalRankingClickCount )
			<< END_LOG;
	}
}
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG// 작업날짜: 2013-05-15	// 박세훈
IMPL_ON_FUNC( DBE_FIELD_WORKINGS_BLOCK_LOG_NOT )
{
	int iOK;

	DO_QUERY( L"exec dbo.P_LFieldBlockLog_INT", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% static_cast<unsigned char>( kPacket_.m_cX2State )
		% kPacket_.m_iPhysicalMemory
		% kPacket_.m_iVirtualMemory
		% static_cast<unsigned char>( kPacket_.m_cProcessTerm )
		% static_cast<unsigned char>( kPacket_.m_cFieldUserNum )
		% static_cast<unsigned char>( kPacket_.m_cFieldNpcNum )
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	return;
}
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
IMPL_ON_FUNC( ELOG_KOM_FILE_CHECK_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LKomFileLog_INS", L"%d, \'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrInvaildKomName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LKomFileLog_INT", L"%d, \'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrInvaildKomName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#ifdef SERV_GOOD_ELSWORD
IMPL_ON_FUNC( ELOG_EXPAND_BANK_INVENTORY_NOT )
{
    /*
    DB
    [Statistics] , [Statistics02]
    SP 이름
    [dbo].[lup_insert_inventory_ED_log]
    전달 인자
    @iUnitUID         BIGINT
    @iUserUID        INT
    @iBeforeED       INT
    @iUseED          INT
    @iInventory       TINYINT
    @iNumSlot        TINYINT
    @iLevel            TINYINT
    */
    int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_LInvenExpandLog_INS", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID
		% kPacket_.m_iBeforeED
		% kPacket_.m_iUseED
		% kPacket_.m_iCategory
		% kPacket_.m_iNumSlot
		% kPacket_.m_iUnitLevel
		);
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.lup_insert_inventory_ED_log", L"%d, %d, %d, %d, %d, %d, %d",
        % kPacket_.m_iUnitUID
        % kPacket_.m_iUserUID
        % kPacket_.m_iBeforeED
        % kPacket_.m_iUseED
        % kPacket_.m_iCategory
        % kPacket_.m_iNumSlot
        % kPacket_.m_iUnitLevel
        );
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();
    }

end_proc:

    LOG_SUCCESS( iOK == NetError::NET_OK )
        << BUILD_LOG( iOK )
        << END_LOG;
}
#endif //SERV_GOOD_ELSWORD

#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( ELOG_EXPAND_SKILL_PAGE_NOT )
{
    /*
	[dbo].[P_LSkillPageExpandLog_INT]전달 인자

	전달 인자
	@iUnitUID         BIGINT
	@strNickName   NVARCHAR(50)
	@iLevel            TINYINT
	@iUnitClass       TINYINT
	@iBeforePage    TINYINT           -- 확장하기 이전 보유한 스킬 페이지 수
	@ibCash           TINYINT           -- 0:ED, 1:Cash
    */
    int iOK = NetError::ERR_ODBC_01;

    DO_QUERY( L"exec dbo.P_LSkillPageExpandLog_INT", L"%d, N\'%s\', %d, %d, %d, %d",
        % kPacket_.m_iUnitUID
        % kPacket_.m_wstrSenderNickName
        % kPacket_.m_ucLevel
        % kPacket_.m_ucUnitClass
        % kPacket_.m_ucBeforeTheNumberOfSkillPagesAvailable
        % kPacket_.m_ucPaymentType
        );

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();
    }

end_proc:

    LOG_SUCCESS( iOK == NetError::NET_OK )
        << BUILD_LOG( iOK )
        << END_LOG;
}
#endif //SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
IMPL_ON_FUNC( DBE_CHANGE_PET_ID_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LChangePetLog_INS", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iPetUID
		% kPacket_.m_iBeforePetID
		% kPacket_.m_iAfterPetID
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"할로윈 요정 펫 로그 남기기 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_iBeforePetID )
			<< BUILD_LOG( kPacket_.m_iAfterPetID )
			<< END_LOG;
	}
}
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG
_IMPL_ON_FUNC( DBE_PAYLETTER_BILLING_ERROR_BUY_PACKET_NOT, KEBILL_BUY_PRODUCT_REQ )
{
	int iOK = NetError::ERR_ODBC_01;

	// 로그 남깁시다.
	/*
	DO_QUERY( L"exec dbo.P_LKomFileLog_INT", L"%d, \'%s\'",
		% kPacket_.m_UserUID
		% TempKomFile
		);

	///여기에 DB쿼리 하고 저장 하고  끝내면 된다
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	

end_proc:

	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;

		*/
}

_IMPL_ON_FUNC( DBE_PAYLETTER_BILLING_ERROR_GIFT_PACKET_NOT, KEBILL_GIFT_ITEM_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
	// 로그 남깁시다.
	/*
	// 서버 에러 로그 통계
	std::wstring TempKomFile = KncUtil::toWideString(kPacket_.m_KomFileName);
	DO_QUERY( L"exec dbo.P_LKomFileLog_INT", L"%d, \'%s\'",
		% kPacket_.m_UserUID
		% TempKomFile
		);

	///여기에 DB쿼리 하고 저장 하고  끝내면 된다
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	

end_proc:

	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;

		*/
}

_IMPL_ON_FUNC( DBE_PAYLETTER_BILLING_ERROR_COUPON_PACKET_NOT, KEBILL_USE_COUPON_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
	// 로그 남깁시다.
	/*
	// 서버 에러 로그 통계
	std::wstring TempKomFile = KncUtil::toWideString(kPacket_.m_KomFileName);
	DO_QUERY( L"exec dbo.P_LKomFileLog_INT", L"%d, \'%s\'",
		% kPacket_.m_UserUID
		% TempKomFile
		);

	///여기에 DB쿼리 하고 저장 하고  끝내면 된다
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}


end_proc:

	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
			*/
}
#endif // SERV_PAYLETTER_BILLING_ERROR_PACKET_LOG

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
IMPL_ON_FUNC( DBE_UNLIMITED_SECOND_CHANGE_JOB_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_LUnlimitUnitClassChangeLog_INS", L"%d, %d, %d",
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iOldUnitClass 
		% kPacket_.m_iNewUnitClass 
		); // 단순 UnitClass 만 변경

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"무제한 전직 변경권 사용 기록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iOldUnitClass )
			<< BUILD_LOG( kPacket_.m_iNewUnitClass )
			<< END_LOG;
	}
}
#endif	//	SERV_UNLIMITED_SECOND_CHANGE_JOB
