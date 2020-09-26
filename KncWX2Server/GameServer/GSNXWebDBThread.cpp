#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSNXWebDBThread.h"
#include "GameServer.h"
#include "ActorManager.h"
#include "NetError.h"


//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT


#define CLASS_TYPE KGSNXWebDBThread
ImplPfID( KGSNXWebDBThread, PI_GS_NX_WEB_DB );

IMPL_PROFILER_DUMP( KGSNXWebDBThread )
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

KGSNXWebDBThread::~KGSNXWebDBThread(void)
{
}

void KGSNXWebDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
		CASE( DBE_INCREASE_WEB_POINT_REQ );

	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	////////////////////////////////////////////////////////////////////////// - 넥슨DB이기 때문에 프로파일링을 할 수 없다.
//#ifdef SP_PROFILER
//	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
//#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KGSNXWebDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSNXWebDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

IMPL_ON_FUNC( DBE_INCREASE_WEB_POINT_REQ )
{
	KDBE_INCREASE_WEB_POINT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_wstrUserID = kPacket_.m_wstrUserID;
	kPacket.m_cPointType = kPacket_.m_cPointType;
	kPacket.m_iIncreasePoint = kPacket_.m_iIncreasePoint;

	// 웹 포인트 증가
	//DO_QUERY( L"exec dbo.elp_CheckEvent_Saving_Create", L"N\'%s\', %d, %d", % kPacket_.m_wstrUserID % (int)kPacket_.m_cPointType % kPacket_.m_iIncreasePoint );

	std::wstring wstrQuery;
	wstrQuery += L"declare @P1 int;";
	wstrQuery += boost::str( boost::wformat( L"exec @P1 = dbo.elp_CheckEvent_Saving_Create N\'%s\', %d, %d;" ) % kPacket_.m_wstrUserID % (int)kPacket_.m_cPointType % kPacket_.m_iIncreasePoint );
	wstrQuery += L"select @P1;";

	if( !m_kODBC.Query( wstrQuery ) )
	{
		START_LOG( cerr, wstrQuery )
			<< BUILD_LOG( m_kODBC.m_strLastQuery )
			<< BUILD_LOG( m_kODBC.m_szDiagMsg )
			<< BUILD_LOG( (wchar_t*)m_kODBC.m_sqlState );

		goto end_proc;
	}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"웹 포인트 증가 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOGc( kPacket_.m_cPointType )
			<< BUILD_LOG( kPacket_.m_iIncreasePoint )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_WEB_POINT_EVENT_01;
	}

end_proc:
	SendToServer( DBE_INCREASE_WEB_POINT_ACK, kPacket );
}

#endif SERV_WEB_POINT_EVENT
//}}

