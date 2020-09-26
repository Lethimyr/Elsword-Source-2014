#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GlobalAccountDBThread.h"
#include "GlobalServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "SimLayer.h"



#define CLASS_TYPE KGlobalAccountDBThread
ImplPfID( KGlobalAccountDBThread, PI_GLOBAL_ACCOUNT_DB );

IMPL_PROFILER_DUMP( KGlobalAccountDBThread )
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
		DO_QUERY_NO_PROFILE( L"exec dbo.mup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
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

KGlobalAccountDBThread::~KGlobalAccountDBThread(void)
{
}

void KGlobalAccountDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
	case 0: // dummy
		break;

		//{{ 2013. 05. 14	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
		CASE( DBE_CHECK_BLOCK_LIST_REQ );
#endif SERV_BLOCK_LIST
		//}}
        
	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

//#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
void KGlobalAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}
//#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 05. 14	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
IMPL_ON_FUNC( DBE_CHECK_BLOCK_LIST_REQ )
{
	// releasetick을 먼저 체크한다.
	KDBE_CHECK_BLOCK_LIST_ACK kPacket;
	kPacket.m_iReleaseTick = kPacket_.m_iReleaseTick;

	bool bGetBlockList = false;
	int iReleaseTickType = 0;
	int iReleaseTick = 0;
	
	// ReleaseTick 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_ReleaseTick_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_ReleaseTick_SEL", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );
#endif //SERV_ALL_RENEWAL_SP	

	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iReleaseTickType
			>> iReleaseTick );

		if( iReleaseTickType != 4 )
			continue;
        
		if( iReleaseTick > kPacket_.m_iReleaseTick )
		{
			kPacket.m_iReleaseTick = iReleaseTick;
			bGetBlockList = true;
		}
	}
	
	// releasetick값이 증가되었다면 테이블 전체를 가져온다.
	if( bGetBlockList )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_ARG( L"exec dbo.P_MPenalty_SEL" );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_ARG( L"exec dbo.P_MPenalty_GET" );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KBlockInfo kInfo;

			FETCH_DATA( kInfo.m_iBlockUID
				>> kInfo.m_cBlockType
				>> kInfo.m_wstrBlockTarget
				>> kInfo.m_wstrStartDate
				>> kInfo.m_wstrEndDate
				);

			kInfo.m_iBlockTarget = ::_wtoi64( kInfo.m_wstrBlockTarget.c_str() );

			std::map< int, std::vector< KBlockInfo > >::iterator mit;
			mit = kPacket.m_mapBlockInfo.find( kInfo.m_cBlockType );
			if( mit == kPacket.m_mapBlockInfo.end() )
			{
				std::vector< KBlockInfo > vecBlockInfo;
				vecBlockInfo.push_back( kInfo );
				kPacket.m_mapBlockInfo.insert( std::make_pair( kInfo.m_cBlockType, vecBlockInfo ) );
			}
			else
			{
				mit->second.push_back( kInfo );
			}
		}
	}

end_proc:
	SendToServer( DBE_CHECK_BLOCK_LIST_ACK, kPacket );
}
#endif SERV_BLOCK_LIST
//}}