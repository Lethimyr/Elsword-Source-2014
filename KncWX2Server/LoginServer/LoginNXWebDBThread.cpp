#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "LoginNXWebDBThread.h"
#include "LoginServer.h"
#include "ActorManager.h"
#include "NetError.h"


//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT


#define CLASS_TYPE KLoginNXWebDBThread
ImplPfID( KLoginNXWebDBThread, PI_LOGIN_NX_WEB_DB );

IMPL_PROFILER_DUMP( KLoginNXWebDBThread )
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

KLoginNXWebDBThread::~KLoginNXWebDBThread(void)
{
}

void KLoginNXWebDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
		CASE_NOPARAM( DBE_GET_WEB_POINT_REWARD_REQ );
	   _CASE( DBE_RESULT_WEB_POINT_REWARD_NOT, KDBE_GET_WEB_POINT_REWARD_ACK );

	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	////////////////////////////////////////////////////////////////////////// - 넥슨DB이기 때문에 프로파일링을 할 수 없다.
//#ifdef SP_PROFILER
//	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
//#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KLoginNXWebDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KLoginNXWebDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

IMPL_ON_FUNC_NOPARAM( DBE_GET_WEB_POINT_REWARD_REQ )
{
	KDBE_GET_WEB_POINT_REWARD_ACK kPacket;	
	int iServerGroupID = 0;
	int iCategory = 0;
	int iPoint = 0;
	int iState = 0;
	std::wstring wstrModefiedDate;
	std::wstring wstrCreateDate;

	int iCurrentServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();

	// 웹 포인트 증가
	DO_QUERY_NO_ARG( L"exec dbo.elp_CheckEvent_Apply_GetList" );

	while( m_kODBC.Fetch() )
	{
		KWebPointRewardInfo kInfo;

		FETCH_DATA( kInfo.m_iApplySN
			>> kInfo.m_wstrUserID
			>> iServerGroupID
			>> kInfo.m_wstrNickName
			>> iCategory
			>> kInfo.m_iRewardItemID
			>> iPoint
			>> iState
			>> wstrModefiedDate
			>> wstrCreateDate
			);

		if( iCurrentServerGroupID != iServerGroupID )
		{
			// 서버군이 다르면 보상하지 않는다.
			continue;
		}

		kPacket.m_vecWebPointReward.push_back( kInfo );
	}

	if( kPacket.m_vecWebPointReward.empty() )
		return;

end_proc:
	SendToServer( DBE_GET_WEB_POINT_REWARD_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_RESULT_WEB_POINT_REWARD_NOT, KDBE_GET_WEB_POINT_REWARD_ACK )
{
	std::vector< KWebPointRewardInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecWebPointReward.begin(); vit != kPacket_.m_vecWebPointReward.end(); ++vit )
	{
		// 닉네임이 존재하지 않는다면 결과처리 하지 않는다.
		if( vit->m_iUnitUID <= 0 )
			continue;

		// 웹 포인트 보상 결과 업데이트
		int iOK = NetError::ERR_ODBC_01;

		std::wstring wstrQuery;
		wstrQuery += L"declare @P1 int;";
		wstrQuery += boost::str( boost::wformat( L"exec @P1 = dbo.elp_CheckEvent_Apply_State_Modify %d, N\'%s\';" ) % vit->m_iApplySN % vit->m_wstrUserID );
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
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"웹 포인트 보상 결과 업데이트 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( vit->m_iApplySN )
				<< BUILD_LOG( vit->m_wstrUserID )
				<< END_LOG;
		}
	}
}

#endif SERV_WEB_POINT_EVENT
//}}

