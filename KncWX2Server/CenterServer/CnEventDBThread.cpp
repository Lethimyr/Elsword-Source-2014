#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "CnEventDBThread.h"
#include "CnServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "PvPRoom.h"
#include "DungeonRoom.h"
#include "CnRoomManager.h"

#ifdef SERV_ADD_EVENT_DB

#define CLASS_TYPE KCnEventDBThread
ImplPfID( KCnEventDBThread, PI_CN_EVENT_DB );

IMPL_PROFILER_DUMP( KCnEventDBThread )
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

KCnEventDBThread::~KCnEventDBThread(void)
{
}

void KCnEventDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
		CASE_NOPARAM( DBE_EVENT_UPDATE_REQ );

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		CASE_NOPARAM( DBE_EVENT_DB_SCRIPT_REQ );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_ADD_EVENT_DB
		CASE_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif // SERV_ADD_EVENT_DB

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_FUNC_NOPARAM( DBE_EVENT_UPDATE_REQ )
{
	KDBE_EVENT_UPDATE_ACK kPacket;

	// DB로부터 이벤트 리스트 데이터를 받아온다
	DO_QUERY( L"exec dbo.P_EEventInfo_GET", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );

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

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, DBE_EVENT_UPDATE_ACK, kPacket );
	GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );

}

#ifdef SERV_ADD_EVENT_DB
IMPL_ON_FUNC_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ )
{
	KDBE_CHECK_EVENT_UPDATE_ACK		AckPacket;
	int iReleaseTickType = 0;
	int iReleaseTick = 0;

	// ReleaseTick 얻기
	DO_QUERY( L"exec dbo.P_ReleaseTick_GET", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );


	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iReleaseTickType
			>> iReleaseTick );
		AckPacket.m_mapReleaseTick.insert( std::make_pair( iReleaseTickType, iReleaseTick ) );
	}

end_proc:
	SendToServer( DBE_CHECK_EVENT_UPDATE_ACK, AckPacket );
}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
IMPL_ON_FUNC_NOPARAM( DBE_EVENT_DB_SCRIPT_REQ )
{
	KDBE_EVENT_DB_SCRIPT_ACK kPacket;

	kPacket.m_mapEventDBData.clear();

	kPacket.m_mapDBRewardData.clear();

	std::map< int, std::string >::iterator mitEventData;


	// DB로부터 이벤트 정보 리스트를 받아 온다.


	DO_QUERY( L"exec dbo.P_EEventScriptInfo_GET", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );


	while( m_kODBC.Fetch() )
	{
		int iIndexID = 0;
		std::wstring wstrEventData = L"";

		FETCH_DATA( iIndexID
			>> wstrEventData
			);

		mitEventData = kPacket.m_mapEventDBData.find(iIndexID);

		if ( mitEventData == kPacket.m_mapEventDBData.end()  )
		{

			if ( wstrEventData.length() != 0 )
			{

				std::string strEventData = "";

				int size = ::WideCharToMultiByte( CP_ACP, 0, wstrEventData.c_str(), (int) wstrEventData.size(), NULL, 0, NULL, NULL );
				if ( size <= 0 )
				{
					continue;
				}

				CHAR*   szBuffer = (CHAR*) _alloca( ( size + 1 ) * sizeof( CHAR ) );
				if (szBuffer == NULL)
				{
					continue;
				}
				::WideCharToMultiByte( CP_ACP, 0, wstrEventData.c_str(), (int) wstrEventData.size(), szBuffer, size, NULL, NULL );
				szBuffer[ size ] = '\0';
				strEventData = szBuffer;

				kPacket.m_mapEventDBData.insert(std::make_pair(iIndexID, strEventData ));

			}

		}
		else
		{
			START_LOG( cerr, L"이벤트 DB 스크립트 내용중 Index 값이 중복으로 들어 왔다" )
				<< BUILD_LOG( iIndexID )
				<< END_LOG;
		}
	}


end_proc:

KEventPtr spEvent( new KEvent );
spEvent->SetData( PI_CN_SERVER, NULL, DBE_EVENT_DB_SCRIPT_ACK, kPacket );
GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );

}
#endif // SERV_EVENT_DB_CONTROL_SYSTEM

#endif SERV_ADD_EVENT_DB

void KCnEventDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KCnEventDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

#endif //SERV_ADD_EVENT_DB