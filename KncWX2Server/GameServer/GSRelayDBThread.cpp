#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSRelayDBThread.h"
#include "GameServer.h"
#include "NetError.h"
#include "GSSimLayer.h"


//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
//#ifdef SERV_RELAY_DB_CONNECTION


//ImplementDBThread( KGSRelayDBThread );

#define CLASS_TYPE KGSRelayDBThread

IMPL_PROFILER_DUMP( KGSRelayDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.P_QueryStats_INS", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.gup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#endif //SERV_ALL_RENEWAL_SP
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

KGSRelayDBThread::~KGSRelayDBThread(void)
{
}

void KGSRelayDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
        CASE( DBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT );		
		CASE( DBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_REQ );
		CASE( DBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT );

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_FUNC( DBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	// 캐릭터 정보를 중계DB에 업데이트 합니다.
	DO_QUERY( L"exec dbo.P_ESUnitInfo_SET", L"N\'%s\', %d, %d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d",
		% kPacket_.m_wstrUserID
		% kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% (int)kPacket_.m_cLevel
		% (int)kPacket_.m_cUnitClass
		% kPacket_.m_bDeletedUnit
		% kPacket_.m_wstrUserRegDate
		% kPacket_.m_wstrUnitRegDate
		% kPacket_.m_iUpdateType
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 정보를 중계DB로 업데이트 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_cLevel )
			<< BUILD_LOG( kPacket_.m_cUnitClass )
			<< BUILD_LOG( kPacket_.m_bDeletedUnit )
			<< BUILD_LOG( kPacket_.m_wstrUserRegDate )
			<< BUILD_LOG( kPacket_.m_wstrUnitRegDate )
			<< BUILD_LOG( kPacket_.m_iUpdateType )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_REQ )
{
	KDBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_ACK kPacket;

	int iOK = NetError::ERR_ODBC_01;

	// 이벤트 보상 정보 얻기
	DO_QUERY( L"exec dbo.P_ESReward_SEL", L"%d", % kPacket_.m_iUserUID );

	while( m_kODBC.Fetch() )
	{
		KJPEventRewardInfo kInfo;

		FETCH_DATA( kInfo.m_iRID
			>> kInfo.m_wstrUserID
			>> kInfo.m_iUserUID
			>> kInfo.m_iUnitUID
			>> kInfo.m_iRewardID
			);

		// 우체국 정보 넣기
		kPacket.m_vecRewardInfo.push_back( kInfo );
	}

	iOK = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	// 캐릭터 정보를 중계DB에 업데이트 합니다.
	DO_QUERY( L"exec dbo.P_ESRewardList_SET", L"%d", % kPacket_.m_iRID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"보상 ID값을 DB에 업데이트 하는데 실패했다!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iRID )
			<< END_LOG;
	}
}

//#endif SERV_RELAY_DB_CONNECTION
//}}

