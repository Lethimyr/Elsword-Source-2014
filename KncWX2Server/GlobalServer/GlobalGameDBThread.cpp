#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GlobalGameDBThread.h"
#include "GlobalServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "SimLayer.h"
#include "X2Data/XSLUnit.h"


//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


#define CLASS_TYPE KGlobalGameDBThread
ImplPfID( KGlobalGameDBThread, PI_GLOBAL_GAME_DB );

IMPL_PROFILER_DUMP( KGlobalGameDBThread )
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

KGlobalGameDBThread::~KGlobalGameDBThread(void)
{
}

void KGlobalGameDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
	   _CASE( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ );

	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KGlobalGameDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGlobalGameDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

_IMPL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	KELG_INVITE_PARTY_FIND_RECEIVER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iHostUnitUID = kPacket_.m_iHostUnitUID;

	// 닉네임 찾기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#endif //SERV_ALL_RENEWAL_SP
	UidType iUnitUID = 0;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iUnitUID );
		m_kODBC.EndFetch();
	}

	if( iUnitUID != 0 )
	{
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_03;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, kPacket );
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}
