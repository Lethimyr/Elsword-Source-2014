#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSEventDBThread.h"
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

//ImplementDBThread( KGSEventDBThread );

#ifdef SERV_ADD_EVENT_DB


#define CLASS_TYPE KGSEventDBThread
ImplPfID( KGSEventDBThread, PI_GS_EVENT_DB );

IMPL_PROFILER_DUMP( KGSEventDBThread )
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

KGSEventDBThread::~KGSEventDBThread(void)
{
}

void KGSEventDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    


		CASE( DBE_UPDATE_EVENT_TIME_REQ );


#ifdef SERV_ADVERTISEMENT_EVENT
		CASE( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT );
#endif SERV_ADVERTISEMENT_EVENT

		CASE(DBE_GAME_SELECT_UNIT_EVENT_DATA_NOT );
		CASE(DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT );
		_CASE(DBE_UPDATE_UNIT_CONNECT_TIME_EVENT_INFO_NOT, KDBE_UPDATE_UNIT_INFO_REQ);

		CASE_NOPARAM( DBE_EVENT_UPDATE_REQ );

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		CASE_NOPARAM( DBE_EVENT_DB_SCRIPT_REQ );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_ADD_EVENT_DB
		CASE_NOPARAM( DBE_CHECK_EVENT_UPDATE_REQ );
#endif // SERV_ADD_EVENT_DB

#ifdef SERV_GLOBAL_EVENT_TABLE
		CASE( DBE_GLOBAL_EVENT_TABLE_INSERT_NOT );
#endif //SERV_GLOBAL_EVENT_TABLE



    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}


bool KGSEventDBThread::Query_UpdateConnectTimeEventInfo( IN const UidType iUserUID, IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
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



//{{ 2008. 5. 19  최육사  이벤트 타임 업데이트
IMPL_ON_FUNC( DBE_UPDATE_EVENT_TIME_REQ )
{
	KDBE_UPDATE_EVENT_TIME_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	{
		//////////////////////////////////////////////////////////////////////////
		// 접속 시간 이벤트

		std::vector< KConnectTimeEventInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecConnectTimeEvent.begin(); vit != kPacket_.m_vecConnectTimeEvent.end(); ++vit )
		{
			//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
			if( vit->m_bAccountEvent )
			{
				DO_QUERY( L"exec dbo.P_EEventAttendUser_INS", L"%d, %d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % LAST_SENDER_UID % vit->m_iEventUID % vit->m_wstrEventTime );

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setConnectTimeEvent.insert( vit->m_iEventUID );

					//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
					if( vit->m_iScriptID == 573 )
					{
						CTime tCurrentTime = CTime::GetCurrentTime();
						std::wstring wstrCurrentTime = tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
						DO_QUERY( L"exec dbo.P_GEvent_AngelCoin_INT", L"%d, %d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % LAST_SENDER_UID % kPacket_.m_iUnitUID % wstrCurrentTime );

						int iResult = -1;
						if( m_kODBC.BeginFetch() )
						{
							FETCH_DATA( iResult );
							m_kODBC.EndFetch();
						}

						if( iResult != 0 )
						{
							START_LOG( cerr, L"대천사의 주화 이벤트 Insert BeginFetch() 실패")
								<< BUILD_LOG( LAST_SENDER_UID )
								<< BUILD_LOG( kPacket_.m_iUnitUID )
								<< BUILD_LOG( wstrCurrentTime )
								<< END_LOG;
						}
					}
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
					//}}
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vit->m_iEventUID )
						<< BUILD_LOG( vit->m_wstrEventTime )
						<< END_LOG;
				}
			}
			else
#endif SERV_ACC_TIME_EVENT
				//}}
			{
				DO_QUERY( L"exec dbo.P_EEventAttendUnit_INS", L"%d, %d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUnitUID % vit->m_iEventUID % vit->m_wstrEventTime );

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setConnectTimeEvent.insert( vit->m_iEventUID );
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vit->m_iEventUID )
						<< BUILD_LOG( vit->m_wstrEventTime )
						<< END_LOG;
				}
			}			
		}

		//////////////////////////////////////////////////////////////////////////		
	}


	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT

	{
		//////////////////////////////////////////////////////////////////////////
		// 누적 시간 이벤트

		//{{ 2013. 1. 8	박세훈	누적 이벤트에 반복 기능 추가
#ifdef SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
#else
		CTime tUpdateTime = CTime::GetCurrentTime();
		tUpdateTime += CTimeSpan( 18250, 0, 0, 0 ); // 한번만 줘야 하므로 50년을 더한다.
		std::wstring wstrUpdateDate = ( CStringW )( tUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
#endif SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
		//}}

		std::vector< KCumulativeTimeEventInfo >::const_iterator vitCT;
		for( vitCT = kPacket_.m_vecCumulativeTimeEvent.begin(); vitCT != kPacket_.m_vecCumulativeTimeEvent.end(); ++vitCT )
		{
			//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
			if( vitCT->m_bAccountEvent )
			{
				// 완료 정보를 업데이트 하자!
				//{{ 2013. 1. 8	박세훈	누적 이벤트에 반복 기능 추가
#ifdef SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
				DO_QUERY( L"exec dbo.P_EEventAttendUser_INS", L"%d, %d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % LAST_SENDER_UID % vitCT->m_iEventUID % vitCT->m_wstrEventTime );
#else
				DO_QUERY( L"exec dbo.P_EEventAttendUser_INS", L"%d, %d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % LAST_SENDER_UID % vitCT->m_iEventUID % wstrUpdateDate );
#endif SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
				//}}

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setCumulativeTimeEvent.insert( vitCT->m_iEventUID );
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vitCT->m_iEventUID )
						//{{ 2013. 1. 8	박세훈	누적 이벤트에 반복 기능 추가
#ifdef SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
						<< BUILD_LOG( vitCT->m_wstrEventTime )
#else
						<< BUILD_LOG( wstrUpdateDate )
#endif SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
						//}}
						<< END_LOG;
				}
			}
			else
#endif SERV_ACC_TIME_EVENT
				//}}
			{
				// 완료 정보를 업데이트 하자!
				//{{ 2013. 1. 8	박세훈	누적 이벤트에 반복 기능 추가
#ifdef SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
				DO_QUERY( L"exec dbo.P_EEventAttendUnit_INS", L"%d, %d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUnitUID % vitCT->m_iEventUID % vitCT->m_wstrEventTime );
#else
				DO_QUERY( L"exec dbo.P_EEventAttendUnit_INS", L"%d, %d, %d, N\'%s\'", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUnitUID % vitCT->m_iEventUID % wstrUpdateDate );
#endif SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
				//}}

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setCumulativeTimeEvent.insert( vitCT->m_iEventUID );
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vitCT->m_iEventUID )
						//{{ 2013. 1. 8	박세훈	누적 이벤트에 반복 기능 추가
#ifdef SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
						<< BUILD_LOG( vitCT->m_wstrEventTime )
#else
						<< BUILD_LOG( wstrUpdateDate )
#endif SERV_REPEAT_CUMULATIVE_REWARD_ITEM_EVENT
						//}}
						<< END_LOG;
				}
			}			
		}
	}	

#endif CUMULATIVE_TIME_EVENT
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_UPDATE_EVENT_TIME_ACK, kPacket );
}
//}}


#ifdef SERV_ADVERTISEMENT_EVENT
IMPL_ON_FUNC( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	BOOST_TEST_FOREACH( const int&, iEventUID, kPacket_.m_vecCompletedEvent )
	{
		DO_QUERY( L"exec dbo.P_EEventAttendUser_INS", L"%d, %d, %d",
			% KBaseServer::GetKObj()->GetServerGroupID()
			% kPacket_.m_iUserUID
			% iEventUID
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"실시간 광고 이벤트 정보 기록 실패" )
				<< BUILD_LOG( iOK )
				<< END_LOG;
		}
	}
end_proc:
	return;
}
#endif SERV_ADVERTISEMENT_EVENT

IMPL_ON_FUNC( DBE_GAME_SELECT_UNIT_EVENT_DATA_NOT )
{

	KDBE_SELECT_UNIT_ACK kPacket;
	kPacket = kPacket_.m_kSelectUnitAck;

	
	// 접속 시간 이벤트 정보 얻기
	DO_QUERY( L"exec dbo.P_EEventAttendUnit_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID );
	while( m_kODBC.Fetch() )
	{
		KConnectTimeEventInfo kConnectTimeEvent;

		FETCH_DATA( kConnectTimeEvent.m_iEventUID
			>> kConnectTimeEvent.m_wstrEventTime
			);

		kPacket.m_vecConnectTimeEvent.push_back( kConnectTimeEvent );
	}
	//}}

	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
	DO_QUERY( L"exec dbo.P_EEventAttendUser_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUserUID );
	while( m_kODBC.Fetch() )
	{
		KConnectTimeEventInfo kConnectTimeEvent;

		FETCH_DATA( kConnectTimeEvent.m_iEventUID
			>> kConnectTimeEvent.m_wstrEventTime
			);

		kPacket.m_vecConnectTimeEvent.push_back( kConnectTimeEvent );
	}



	//{{ 2009. 12. 7  최육사	누적시간이벤트
	DO_QUERY( L"exec dbo.P_EEventAttendUnit_Acc_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID );
	while( m_kODBC.Fetch() )
	{
		KCumulativeTimeEventInfo kCumulativeTimeEvent;

		FETCH_DATA( kCumulativeTimeEvent.m_iEventUID
			>> kCumulativeTimeEvent.m_iCumulativeTime
			);

		kPacket.m_vecCumulativeTimeEvent.push_back( kCumulativeTimeEvent );
	}

	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT

	DO_QUERY( L"exec dbo.P_EEventAttendUser_Acc_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUserUID );
	while( m_kODBC.Fetch() )
	{
		KCumulativeTimeEventInfo kCumulativeTimeEvent;

		FETCH_DATA( kCumulativeTimeEvent.m_iEventUID
			>> kCumulativeTimeEvent.m_iCumulativeTime
			);

		kPacket.m_vecCumulativeTimeEvent.push_back( kCumulativeTimeEvent );
	}
#endif SERV_ACC_TIME_EVENT
	//}}

#ifdef SERV_ADVERTISEMENT_EVENT
	int iEventUID = 0;

	DO_QUERY( L"exec dbo.P_EEventAttendUser_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUserUID );
	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iEventUID );

		kPacket.m_vecAdvertisementEvent.push_back( iEventUID );
	}
#endif SERV_ADVERTISEMENT_EVENT


#ifdef SERV_GLOBAL_EVENT_TABLE

	std::map< int, KGlobalEventTableData >::iterator mit = kPacket_.m_mapGlobalEventData.begin();

	for (; mit != kPacket_.m_mapGlobalEventData.end(); ++mit )
	{

		bool bCheckGetData = false;
		bool bAccountOnlyEvent = false;
		UidType iUserUID = 0;
		UidType iUnitUID = 0;

		switch(mit->second.m_cEventAccountType)
		{
		case KGlobalEventTableData::EAE_ACCOUNT_NORMAL:
			{
				iUserUID = kPacket_.m_iUserUID;
				iUnitUID = kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID;
			}
			break;
		case KGlobalEventTableData::EAE_ACCOUNT_SUM:
			{
				iUserUID = kPacket_.m_iUserUID;
				iUnitUID = 0;
			}
			break;
		default:
			{
				iUserUID = 0;
				iUnitUID = kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID;
			}
			break;

		}
		

		DO_QUERY( L"exec dbo.P_EEventData_GET", L"%d, %d, %d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % iUserUID % iUnitUID % mit->first );

		while( m_kODBC.Fetch() )
		{

			KGlobalEventTableData kGobalEventTableData;
			int iParam1 = 0;
			int iParam2 = 0;
			int iParam3 = 0;

			UidType iGetUserUID = 0;
			UidType iGetUnitUID = 0;
			

			FETCH_DATA( iGetUserUID
				>> iGetUnitUID
				>> kGobalEventTableData.m_usCategory
				>> iParam1
				>> iParam2
				>> iParam3
				);

				if ( mit->second.m_cEventAccountType == KGlobalEventTableData::EAE_ACCOUNT_NORMAL )
				{
					if ( iGetUnitUID != kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID )
					{
						//2014.01.02 darkstarbt_조성욱 // 계정내 케릭터 한번만 하는것은 이벤트 추가 하지 않는다. 추후 계정내에 어떤 캐릭터 때문에 안되는지 표시 할려면 여기 이후에 별도 처리 해야 함
						bAccountOnlyEvent = true;
						continue;
					}
				}
				
				kGobalEventTableData.m_iEventID = mit->first;
				kGobalEventTableData.m_iUnitUID = kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID;
				kGobalEventTableData.m_iUserUID = kPacket_.m_iUserUID;
				kGobalEventTableData.m_iEventScriptID = mit->second.m_iEventScriptID;
				kGobalEventTableData.m_iEventType = mit->second.m_iEventType;
				kGobalEventTableData.m_cEventAccountType = mit->second.m_cEventAccountType;

				kGobalEventTableData.m_veciParamData.push_back(iParam1);
				kGobalEventTableData.m_veciParamData.push_back(iParam2);
				kGobalEventTableData.m_veciParamData.push_back(iParam3);



				std::map< int, KGlobalEventTableData >::iterator mitEventTableData = kPacket.m_mapGlobalEventData.find(mit->first);

				if (mitEventTableData != kPacket.m_mapGlobalEventData.end() )
				{
					for (int i = 0; i < kGobalEventTableData.m_veciParamData.size(); ++i)
					{
						mitEventTableData->second.m_veciParamData.push_back(kGobalEventTableData.m_veciParamData[i]);
					}
				}
				else
				{
					kPacket.m_mapGlobalEventData.insert(std::make_pair(mit->first, kGobalEventTableData ));
				}


				bCheckGetData = true;
		}

		if ( bCheckGetData == false && bAccountOnlyEvent == false) // 2014.01.02 darkstarbt_조성욱 // 계정당 한번 진행 되는 이벤트는 애초에 등록을 하지 않는다.
		{ 
			std::map< int, KGlobalEventTableData >::iterator mitEventTableNoData = kPacket.m_mapGlobalEventData.find(mit->first);

			if (mitEventTableNoData == kPacket.m_mapGlobalEventData.end() )
			{
				kPacket.m_mapGlobalEventData.insert(std::make_pair(mit->first, mit->second ));
			}
			
		}

	}
	
#endif //SERV_GLOBAL_EVENT_TABLE



	SendToUser( kPacket_.m_iUserUID, DBE_GAME_SELECT_UNIT_ACK, kPacket );

end_proc:
	return;

}


IMPL_ON_FUNC( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT )
{

	KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK kPacket;

	kPacket = kPacket_.m_kSelectUnitAck;


	// 접속 시간 이벤트 정보 얻기
	DO_QUERY( L"exec dbo.P_EEventAttendUnit_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_kSelectUnitAck.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID );
	while( m_kODBC.Fetch() )
	{
		KConnectTimeEventInfo kConnectTimeEvent;

		FETCH_DATA( kConnectTimeEvent.m_iEventUID
			>> kConnectTimeEvent.m_wstrEventTime
			);

		kPacket.m_kSelectUnitAck.m_vecConnectTimeEvent.push_back( kConnectTimeEvent );
	}
	//}}

	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
	DO_QUERY( L"exec dbo.P_EEventAttendUser_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUserUID );
	while( m_kODBC.Fetch() )
	{
		KConnectTimeEventInfo kConnectTimeEvent;

		FETCH_DATA( kConnectTimeEvent.m_iEventUID
			>> kConnectTimeEvent.m_wstrEventTime
			);

		kPacket.m_kSelectUnitAck.m_vecConnectTimeEvent.push_back( kConnectTimeEvent );
	}



	//{{ 2009. 12. 7  최육사	누적시간이벤트
	DO_QUERY( L"exec dbo.P_EEventAttendUnit_Acc_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_kSelectUnitAck.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID );
	while( m_kODBC.Fetch() )
	{
		KCumulativeTimeEventInfo kCumulativeTimeEvent;

		FETCH_DATA( kCumulativeTimeEvent.m_iEventUID
			>> kCumulativeTimeEvent.m_iCumulativeTime
			);

		kPacket.m_kSelectUnitAck.m_vecCumulativeTimeEvent.push_back( kCumulativeTimeEvent );
	}

	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT

	DO_QUERY( L"exec dbo.P_EEventAttendUser_Acc_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUserUID );
	while( m_kODBC.Fetch() )
	{
		KCumulativeTimeEventInfo kCumulativeTimeEvent;

		FETCH_DATA( kCumulativeTimeEvent.m_iEventUID
			>> kCumulativeTimeEvent.m_iCumulativeTime
			);

		kPacket.m_kSelectUnitAck.m_vecCumulativeTimeEvent.push_back( kCumulativeTimeEvent );
	}
#endif SERV_ACC_TIME_EVENT
	//}}

#ifdef SERV_ADVERTISEMENT_EVENT
	int iEventUID = 0;

	DO_QUERY( L"exec dbo.P_EEventAttendUser_GET", L"%d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % kPacket_.m_iUserUID );
	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iEventUID );

		kPacket.m_kSelectUnitAck.m_vecAdvertisementEvent.push_back( iEventUID );
	}
#endif SERV_ADVERTISEMENT_EVENT


	SendToUser( kPacket_.m_iUserUID, DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK, kPacket );

end_proc:
	return;

}

_IMPL_ON_FUNC( DBE_UPDATE_UNIT_CONNECT_TIME_EVENT_INFO_NOT, KDBE_UPDATE_UNIT_INFO_REQ )
{
	LIF( Query_UpdateConnectTimeEventInfo( FIRST_SENDER_UID, kPacket_ ) );
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
	SendToServer( DBE_EVENT_UPDATE_ACK, kPacket );
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
#endif // SERV_ADD_EVENT_DB


#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
IMPL_ON_FUNC_NOPARAM( DBE_EVENT_DB_SCRIPT_REQ )
{
	KDBE_EVENT_DB_SCRIPT_ACK kPacket;

	kPacket.m_mapEventDBData.clear();

	kPacket.m_mapDBRewardData.clear();

	std::map< int, std::string >::iterator mitEventData;

	std::map< int, std::vector< KRewardData > >::iterator mitReward;

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


	DO_QUERY( L"exec dbo.P_EEventRewardInfo_GET", L"%d", % KBaseServer::GetKObj()->GetServerGroupID() );

	while( m_kODBC.Fetch() )
	{
		int iRewardID = 0;
		int iItemID = 0;
		int iQuantity = 0;
		int iPeriod = 0;
		
		FETCH_DATA( iRewardID
			>> iItemID
			>> iQuantity
			>> iPeriod
			);
	


		if( iRewardID <= 0  ||  iItemID <= 0  ||  iQuantity <= 0  ||  iPeriod < 0 )
		{
			START_LOG( cerr, L"잘못된 보상 정보 입니다." )
				<< BUILD_LOG( iRewardID )
				<< BUILD_LOG( iItemID )
				<< BUILD_LOG( iQuantity )
				<< BUILD_LOG( iPeriod )
				<< END_LOG;

			continue;
		}

		KRewardData kRewardData;
		kRewardData.m_iRewardItemID = iItemID;
		kRewardData.m_iQuantity		= iQuantity;
		kRewardData.m_sPeriod		= static_cast<short>(iPeriod);	


		mitReward = kPacket.m_mapDBRewardData.find(iRewardID);

		if ( mitReward == kPacket.m_mapDBRewardData.end()  )
		{
			std::vector< KRewardData > vecRewardData;
			vecRewardData.push_back( kRewardData );

			kPacket.m_mapDBRewardData.insert(std::make_pair(iRewardID, vecRewardData ));
		}
		else
		{
			mitReward->second.push_back(kRewardData);
		}
	}



end_proc:
	SendToServer( DBE_EVENT_DB_SCRIPT_ACK, kPacket );
	
}

#endif //SERV_EVENT_DB_CONTROL_SYSTEM


#ifdef SERV_GLOBAL_EVENT_TABLE
IMPL_ON_FUNC( DBE_GLOBAL_EVENT_TABLE_INSERT_NOT )
{
	
	int iOK = NetError::ERR_ODBC_01;

	std::map< int, KGlobalEventTableData >::iterator mit = kPacket_.m_mapGlobalEventData.begin();

	

	for (; mit != kPacket_.m_mapGlobalEventData.end(); ++mit )
	{

		int tesat = mit->second.m_veciParamData.size();
		// 공용 이벤트에서 사용할 임의의 데이터
		int iCheckParamCount = 0;
		int iParam[3] = {0};
		unsigned short usCategory = 0;
		for ( int i = 0; i < mit->second.m_veciParamData.size(); ++i )
		{

			iParam[iCheckParamCount] = mit->second.m_veciParamData[i];


			iCheckParamCount += 1;

			if (iCheckParamCount == 3 || i == mit->second.m_veciParamData.size()-1)
			{

				UidType iUserUID = 0;
				UidType iUnitUID = 0;

				switch(mit->second.m_cEventAccountType)
				{
				case KGlobalEventTableData::EAE_ACCOUNT_NORMAL:
					{
						iUserUID = mit->second.m_iUserUID;
						iUnitUID = mit->second.m_iUnitUID;
					}
					break;
				case KGlobalEventTableData::EAE_ACCOUNT_SUM:
					{
						iUserUID = mit->second.m_iUserUID;
						iUnitUID = 0;
					}
					break;
				default:
					{
						iUserUID = 0;
						iUnitUID = mit->second.m_iUnitUID;
					}
					break;

				}


				DO_QUERY( L"exec dbo.P_EEventData_SET", L"%d, %d, %d, %d, %d, %d, %d, %d", % KBaseServer::GetKObj()->GetServerGroupID() % iUserUID
																							% iUnitUID % mit->first
																							% usCategory % iParam[0] % iParam[1] % iParam[2]
					);

					if( m_kODBC.BeginFetch() )
					{
						FETCH_DATA( iOK );
						m_kODBC.EndFetch();
					}

				iCheckParamCount = 0;

				iParam[0] = 0;
				iParam[1] = 0;
				iParam[2] = 0;

				usCategory += 1;
			}

		}

	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< END_LOG;

}
#endif //SERV_GLOBAL_EVENT_TABLE

void KGSEventDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSEventDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

#endif //SERV_ADD_EVENT_DB