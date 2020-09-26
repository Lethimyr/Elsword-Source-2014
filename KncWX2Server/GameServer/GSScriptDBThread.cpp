#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSScriptDBThread.h"
#include "GameServer.h"
#include "NetError.h"
#include "Inventory.h"      // 장착 아이템 얻어올 때 InventoryCategory 필요.
#include "StatTable.h"
#include "ExpTable.h"
#include "X2Data/XSLItem.h"
#include "GSSimLayer.h"
#include "TutorialManager.h"
#include "X2Data/XSLAttribEnchantItem.h"
#include "X2Data/XSLUnit.h"

//ImplementDBThread( KGSScriptDBThread );

#ifdef SERV_ADD_SCRIPT_DB


#define CLASS_TYPE KGSScriptDBThread
ImplPfID( KGSScriptDBThread, PI_GS_SCRIPT_DB );

IMPL_PROFILER_DUMP( KGSScriptDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		

		DO_QUERY_NO_PROFILE( L"exec dbo.P_QueryStats_INS", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);

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

KGSScriptDBThread::~KGSScriptDBThread(void)
{
}

void KGSScriptDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
#ifdef SERV_STRING_FILTER_USING_DB
		CASE_NOPARAM( DBE_CHECK_STRING_FILTER_UPDATE_REQ );
		CASE_NOPARAM( DBE_STRING_FILTER_UPDATE_REQ );
#endif //SERV_STRING_FILTER_USING_DB

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}


bool KGSScriptDBThread::Query_UpdateConnectTimeEventInfo( IN const UidType iUserUID, IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{	
	BOOST_TEST_FOREACH( const KCumulativeTimeEventInfo&, kEventInfo, kReq.m_vecUpdateEventTime )
	{
		//{{ 2010. 06. 15  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		if( kEventInfo.m_bAccountEvent )
		{
			int iOK = NetError::ERR_ODBC_01;

			DO_QUERY( L"exec dbo.P_EEventAttendUser_Acc_SET", L"%d, %d, %d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % iUserUID % kEventInfo.m_iEventUID % kEventInfo.m_iCumulativeTime );

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iUserUID )
					<< BUILD_LOG( kEventInfo.m_iEventUID )
					<< BUILD_LOG( kEventInfo.m_iCumulativeTime )
					<< END_LOG;
			}
		}
		else
#endif SERV_ACC_TIME_EVENT
			//}}
		{
			int iOK = NetError::ERR_ODBC_01;

			DO_QUERY( L"exec dbo.P_EEventAttendUnit_Acc_SET", L"%d, %d, %d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kReq.m_iUnitUID % kEventInfo.m_iEventUID % kEventInfo.m_iCumulativeTime );

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( kEventInfo.m_iEventUID )
					<< BUILD_LOG( kEventInfo.m_iCumulativeTime )
					<< END_LOG;
			}
		}
	}

	return true;

end_proc:
	START_LOG( cerr, L"접속 시간 이벤트 DB업데이트 실패" )
		<< BUILD_LOG( iUserUID )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< END_LOG;
	return false;
}




void KGSScriptDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSScriptDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

#ifdef SERV_STRING_FILTER_USING_DB
IMPL_ON_FUNC_NOPARAM( DBE_CHECK_STRING_FILTER_UPDATE_REQ )
{
	KDBE_CHECK_STRING_FILTER_UPDATE_ACK		AckPacket;
	int iReleaseTickType = 0;
	int iReleaseTick = 0;

	// ReleaseTick 얻기
#ifdef SERV_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ReleaseTick_SEL" );
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
	SendToServer( DBE_CHECK_STRING_FILTER_UPDATE_ACK, AckPacket );
}

IMPL_ON_FUNC_NOPARAM( DBE_STRING_FILTER_UPDATE_REQ )
{
	KDBE_STRING_FILTER_UPDATE_ACK kPacket;

	DO_QUERY( L"exec dbo.P_SStringFilter_SEL" );

	while( m_kODBC.Fetch() )
	{
		KStringFilterInfo kStringFilterInfo;

		FETCH_DATA( kStringFilterInfo.m_sStringFilterType
			>> kStringFilterInfo.m_wstrFilter
			);

		kPacket.m_vecStringFilterList.push_back( kStringFilterInfo );
	}

end_proc:
	SendToServer( DBE_STRING_FILTER_UPDATE_ACK, kPacket );
};
#endif //SERV_STRING_FILTER_USING_DB

#endif //SERV_ADD_SCRIPT_DB