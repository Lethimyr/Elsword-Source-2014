#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "CnGameDBThread.h"
#include "CnServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "CnSimLayer.h"
#include "X2Data/XSLUnit.h"
#include "X2Data/XSLItem.h"

#define CLASS_TYPE KCnGameDBThread
ImplPfID( KCnGameDBThread, PI_CN_GAME_DB );

IMPL_PROFILER_DUMP( KCnGameDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		

		DO_QUERY_NO_PROFILE( L"exec dbo.gup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
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

KCnGameDBThread::~KCnGameDBThread(void)
{
}

void KCnGameDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
		//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
		CASE( DBE_DB_UPDATE_TRADE_RESULT_REQ );
#endif SERV_TRADE_DDOS_DEFENCE
		//}}
		//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
		CASE( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
		//}}
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		CASE( DBE_WEDDING_COMPLETE_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KCnGameDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KCnGameDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

bool KCnGameDBThread::Query_InsertItemList( IN const SEnum::GET_ITEM_REASON eGetItemReason, 
										    IN const UidType iUnitUID, 
											IN const std::vector< KItemInfo >& vecItemInfo, 
											OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, 
											IN const bool bUnSeal /*= true*/ )
{
	if( SEnum::IsValidGetItemReason( eGetItemReason ) == false )
	{
		START_LOG( cerr, L"유효하지않은 아이템 생성 사유입니다!" )
			<< BUILD_LOG( eGetItemReason )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( vecItemInfo.size() )
			<< END_LOG;
	}

	BOOST_TEST_FOREACH( const KItemInfo&, kNewItemInfo, vecItemInfo )
	{
		KItemInfo kNewItemInfoResult = kNewItemInfo; // 복사본 생성

		//////////////////////////////////////////////////////////////////////////
		// 아이템 생성
		UidType iItemUID = 0;
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.gup_insert_item", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% iUnitUID
			% kNewItemInfo.m_iItemID
			% static_cast<int>(kNewItemInfo.m_cUsageType)
			% kNewItemInfo.m_iQuantity
			% kNewItemInfo.m_sEndurance
			% kNewItemInfo.m_sPeriod
			% static_cast<int>(kNewItemInfo.m_cEnchantLevel)
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
			% static_cast<int>(eGetItemReason)
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK
				>> iItemUID
				>> kNewItemInfoResult.m_wstrExpirationDate );

			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"아이템 삽입 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( kNewItemInfo.m_iItemID )
				<< BUILD_LOGc( kNewItemInfo.m_cUsageType )
				<< BUILD_LOG( kNewItemInfo.m_iQuantity )
				<< BUILD_LOG( kNewItemInfo.m_sEndurance )
				<< BUILD_LOG( kNewItemInfo.m_sPeriod )
				<< BUILD_LOGc( kNewItemInfo.m_cEnchantLevel )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
				<< END_LOG;
			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////		
		//{{ 2009. 8. 27  최육사	밀봉
		if( kNewItemInfo.m_ucSealData > 0 )
		{
			//{{ 2009. 9. 2  최육사		밀봉
			if( kNewItemInfoResult.IsSealedItem()  &&  bUnSeal )
			{
				kNewItemInfoResult.UnsealItem(); // 밀봉 해제
			}
			//}}

			DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % iItemUID % static_cast<int>(kNewItemInfoResult.m_ucSealData) );

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"밀봉 정보 업데이트 실패." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOGc( kNewItemInfoResult.m_ucSealData )
					<< END_LOG;
				goto end_proc;
			}
		}
		//}}

		//////////////////////////////////////////////////////////////////////////		
		// 소켓 정보가 존재한다면 역시 DB에 insert하자!
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-12-11	// 박세훈
		{
			bool bNeedSocketInfoDBUpdate = false;

			if( 0 < kNewItemInfo.m_byteExpandedSocketNum )
			{
				bNeedSocketInfoDBUpdate = true;
			}
			else if( kNewItemInfo.m_vecItemSocket.empty() == false )
			{
				for( std::vector<int>::const_iterator it = kNewItemInfo.m_vecItemSocket.begin(); it != kNewItemInfo.m_vecItemSocket.end(); ++it )
				{
					if( 0 < *it )
					{
						bNeedSocketInfoDBUpdate = true;
						break;
					}
				}
			}

			if( bNeedSocketInfoDBUpdate == true )
			{
				const byte byteArraySize = 5;
				int arrSocketInfo[byteArraySize]; // DB 테이블의 소켓테이블 참고
				memset( arrSocketInfo, 0, sizeof(int) * byteArraySize );

				std::vector<int>::const_iterator it = kNewItemInfo.m_vecItemSocket.begin();
				for( int i = 0; i < byteArraySize; ++i )
				{
					if( it == kNewItemInfo.m_vecItemSocket.end() )
						continue;

					arrSocketInfo[i] = *it;
					++it;
				}

				DO_QUERY( L"exec dbo.P_GItemSocket_SET", L"%d, %d, %d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					% arrSocketInfo[4]			// @iSoket5 smallint
					% kNewItemInfo.m_byteExpandedSocketNum
					);

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( iItemUID )
						<< BUILD_LOG( arrSocketInfo[0] )
						<< BUILD_LOG( arrSocketInfo[1] )
						<< BUILD_LOG( arrSocketInfo[2] )
						<< BUILD_LOG( arrSocketInfo[3] )
						<< BUILD_LOG( arrSocketInfo[4] )
						<< END_LOG;
				}
			}
		}
#else // SERV_BATTLE_FIELD_BOSS
		if( kNewItemInfo.m_vecItemSocket.empty() == false )
		{
			bool bSocketExist = false;
			int arrSocketInfo[4] = {0,0,0,0}; // DB 테이블의 소켓테이블 참고
			std::vector< int >::const_iterator vitSocket = kNewItemInfo.m_vecItemSocket.begin();
			for( int iIdx = 0; iIdx < 4; ++iIdx )
			{
				if( vitSocket == kNewItemInfo.m_vecItemSocket.end() )
					continue;

				if( *vitSocket > 0 )
				{
					bSocketExist = true;
				}
				arrSocketInfo[iIdx] = *vitSocket;
				++vitSocket;
			}

			if( bSocketExist )
			{
				DO_QUERY( L"exec dbo.gup_update_Socket", L"%d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					);

					if( m_kODBC.BeginFetch() )
					{
						FETCH_DATA( iOK );
						m_kODBC.EndFetch();
					}

					if( iOK != NetError::NET_OK )
					{
						START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
							<< BUILD_LOG( iOK )
							<< BUILD_LOG( iUnitUID )
							<< BUILD_LOG( iItemUID )
							<< BUILD_LOG( arrSocketInfo[0] )
							<< BUILD_LOG( arrSocketInfo[1] )
							<< BUILD_LOG( arrSocketInfo[2] )
							<< BUILD_LOG( arrSocketInfo[3] )
							<< END_LOG;
					}
			}
		}
#endif // SERV_BATTLE_FIELD_BOSS

		// 생성되는 아이템 정보를 모두 DB에 insert하였으므로 컨테이너에도 넣자!
		mapInsertedItemInfo.insert( std::make_pair( iItemUID, kNewItemInfoResult ) );
	}

	return true;

end_proc:
	return false;
}

bool KCnGameDBThread::Query_UpdateItemQuantity( IN const UidType iUnitUID, 
											    IN const std::map< UidType, int >& mapUpdated, 
												OUT std::map< UidType, int >& mapFailed, 
												OUT bool& bUpdateFailed )
{
	//{{ 2009. 11. 17  최육사	DB쿼리실패	
	bool bQuerySuccess = true;
	//}}
	bUpdateFailed = false;
	mapFailed.clear();

	std::map< UidType, int >::const_iterator mit = mapUpdated.begin();
	while( mit != mapUpdated.end() )
	{
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.gup_update_item", L"%d, %d, %d, %d", 
			% iUnitUID 
			% mit->first 
			% CXSLItem::PT_QUANTITY 
			% mit->second
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 정보 업데이트 실패." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;

				//{{ 2008. 10. 23  최육사	증분값 버그한번 잡아보자~!
				if( iOK == -5 )
				{
					START_LOG( cout, L"증분값 업데이트 버그 발생!" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( mit->first )
						<< BUILD_LOG( mit->second );

					bUpdateFailed = true;
				}
				//}}
			}
		}
		else
		{
			START_LOG( cerr, L"아이템 수량 업데이트중 BeginFetch 실패." );
		}

end_proc:
		if( iOK != NetError::NET_OK  &&  iOK != -5 )
		{
			//{{ 2009. 11. 17  최육사	DB쿼리실패
			bQuerySuccess = false;
			//}}

			mapFailed.insert( std::make_pair( mit->first, mit->second ) );
		}
		mit++;
	}

	return bQuerySuccess;
}

void KCnGameDBThread::Query_DeleteItem( IN const std::vector< KDeletedItemInfo >& vecDeleted, OUT std::vector< KDeletedItemInfo >& vecFailed )
{
	vecFailed.clear();

	std::vector< KDeletedItemInfo >::const_iterator vit = vecDeleted.begin();
	while( vit != vecDeleted.end() )
	{
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.gup_delete_item", L"%d, %d", % vit->m_iItemUID % (int)vit->m_ucDeleteReason );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 삭제 실패." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( vit->m_iItemUID )
					<< BUILD_LOGc( vit->m_ucDeleteReason )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"아이템 삭제 중 BeginFetch 실패." );
		}
end_proc:
		if( iOK != NetError::NET_OK )
		{
			vecFailed.push_back( *vit );
		}
		++vit;
	}
}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
IMPL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ )
{
	KDBE_DB_UPDATE_TRADE_RESULT_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::ERR_ODBC_01;

	KERM_DB_UPDATE_TRADE_RESULT_NOT UnitA;
	KERM_DB_UPDATE_TRADE_RESULT_NOT UnitB;

	// SP 쿼리를 만들어 보자
	int iTemp_Type = 6;			// 개인 거래 sp 타입
	//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade_New ";
#else
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade ";
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}	
	wstrQuerySP += boost::str( boost::wformat( L"%d" ) % iTemp_Type );
	
	std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >::iterator vitDBUpdate;

	bool bFirstUnit = true;
	for( vitDBUpdate = kPacket_.m_vecDBUpdate.begin() ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		if( bFirstUnit == true )
		{
			UnitA = (*vitDBUpdate);
			bFirstUnit = false;
		}
		else
		{
			UnitB = (*vitDBUpdate);
			break;
		}
	}

	// UnitUID 를 먼저 적어 놓자
	
	for( vitDBUpdate = kPacket_.m_vecDBUpdate.begin() ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vitDBUpdate->m_iUnitUID );
	}

	// ed 를 먼저 적어 놓자
	for( vitDBUpdate = kPacket_.m_vecDBUpdate.begin() ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vitDBUpdate->m_iChangedED );
	}

	// 유닛별 정보로 정리
	vitDBUpdate = kPacket_.m_vecDBUpdate.begin();
	int iTemp_Zero = 0;
	bool bCheck = false;
	int index = 0;
	for(  ; vitDBUpdate != kPacket_.m_vecDBUpdate.end() ; ++vitDBUpdate )
	{
		std::vector<KInventoryItemInfo>::iterator vvit = vitDBUpdate->m_vecUpdated.begin();
		for( ; vvit != vitDBUpdate->m_vecUpdated.end() ; ++vvit, ++index )
		{
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vvit->m_iItemUID );
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % vvit->m_kItemInfo.m_iQuantity );
		}

		for( ; index < 10 ; ++index )
		{
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );	
		}
	}

	if( 10 <= index && index < 20 )
	{
		for( ; index < 20 ; ++index )
		{
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
			wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );	
		}
	}
	
	DO_QUERY_NO_ARG( wstrQuerySP.c_str() );

	// 아이템은 10개 + 10개 이므로 20개로 고정한다.
	UidType iItemUID[20] = {0,};

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacketAck.m_iOK
			>> iItemUID[0]			// A unit -> B
			>> iItemUID[1]
			>> iItemUID[2]
			>> iItemUID[3]
			>> iItemUID[4]
			>> iItemUID[5]
			>> iItemUID[6]
			>> iItemUID[7]
			>> iItemUID[8]
			>> iItemUID[9]
			>> iItemUID[10]			// B unit -> A
			>> iItemUID[11]
			>> iItemUID[12]
			>> iItemUID[13]
			>> iItemUID[14]
			>> iItemUID[15]
			>> iItemUID[16]
			>> iItemUID[17]
			>> iItemUID[18]
			>> iItemUID[19]	);

		m_kODBC.EndFetch();

		if( kPacketAck.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"개인 거래 SP 호출이 실패 했습니다." )
				<< BUILD_LOG( kPacketAck.m_iOK )
				<< END_LOG;

			goto end_proc;
		}
		
		UnitB.m_vecInsertDBItem.clear();
		std::vector<KInventoryItemInfo>::iterator vitA = UnitA.m_vecUpdated.begin();
		for( int indexA = 0 ; (indexA < 10 && vitA != UnitA.m_vecUpdated.end()) ; ++indexA )	
		{
			if( vitA != UnitA.m_vecUpdated.end() )
			{
				vitA->m_iItemUID = iItemUID[indexA];
				vitA->m_sSlotID = -1;					// 상대방의 인벤토리에 있던 정보 이므로 초기화 하자

				// 거래시 봉인 상태는 SP 에서 해제한다.
				if( vitA->m_kItemInfo.IsSealedItem() == true )
				{
					vitA->m_kItemInfo.UnsealItem();
				}

				UnitB.m_vecInsertDBItem.push_back( *vitA );
				++vitA;				
			}
		}

		UnitA.m_vecInsertDBItem.clear();
		std::vector<KInventoryItemInfo>::iterator vitB = UnitB.m_vecUpdated.begin();
		for( int indexB = 10 ; (indexB < 20 && vitB != UnitB.m_vecUpdated.end()) ; ++indexB )	
		{
			if( vitB != UnitB.m_vecUpdated.end() )
			{
				vitB->m_iItemUID = iItemUID[indexB];
				vitB->m_sSlotID = -1;				// 상대방의 인벤토리에 있던 정보 이므로 초기화 하자

				// 거래시 봉인 상태는 SP 에서 해제한다.
				if( vitB->m_kItemInfo.IsSealedItem() == true )
				{
					vitB->m_kItemInfo.UnsealItem();
				}

				UnitA.m_vecInsertDBItem.push_back( *vitB );
				++vitB;
			}
		}
	}

	kPacketAck.m_vecDBUpdate.push_back( UnitA );
	kPacketAck.m_vecDBUpdate.push_back( UnitB );

end_proc:

	SendToRoom( FIRST_SENDER_UID, DBE_DB_UPDATE_TRADE_RESULT_ACK, kPacketAck );
}
#else

//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
IMPL_ON_FUNC( DBE_DB_UPDATE_TRADE_RESULT_REQ )
{
	KDBE_DB_UPDATE_TRADE_RESULT_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;

	BOOST_TEST_FOREACH( const KERM_DB_UPDATE_TRADE_RESULT_NOT&, kPacketReq, kPacket_.m_vecDBUpdate )
	{
		KERM_DB_UPDATE_TRADE_COMPLETE_NOT kPacketResult;
		kPacketResult.m_iUnitUID = kPacketReq.m_iUnitUID;
		kPacketResult.m_iChangedED = kPacketReq.m_iChangedED;
		kPacketResult.m_mapInsertedItem = kPacketReq.m_mapInsertedItem;
		kPacketResult.m_vecUpdatedInventorySlot = kPacketReq.m_vecUpdatedInventorySlot;

		int iOK = NetError::ERR_ODBC_00;

		// ED정보 업데이트!
		DO_QUERY( L"exec dbo.gup_update_unit_info", L"%d, %d, %d, %d, %d, %d, %d, %d",
			% kPacketReq.m_iUnitUID
			% 0
			% 0
			% kPacketReq.m_iChangedED
			% 0
			% 0
			% 0
			% false
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"개인거래 결과 처리 ED정보 업데이트 실패!" )
				<< BUILD_LOG( kPacketReq.m_iUnitUID )
				<< BUILD_LOG( kPacketReq.m_iChangedED )
				<< END_LOG;
		}

		// 아이템 정보 업데이트!
		bool bUpdateFailed = false;
		LIF( Query_UpdateItemQuantity( kPacketReq.m_iUnitUID, kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange, kPacketResult.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
		Query_DeleteItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted, kPacketResult.m_kItemQuantityUpdate.m_vecDeleted );

		if( Query_InsertItemList( SEnum::GIR_TRADE, kPacketReq.m_iUnitUID, kPacketReq.m_vecItemInfo, kPacketResult.m_mapItemInfo ) == false )
		{
			START_LOG( cerr, L"아이템 업데이트 실패!" )
				<< BUILD_LOG( kPacketReq.m_iUnitUID )
				<< END_LOG;
		}

		kPacketAck.m_vecDBUpdate.push_back( kPacketResult );
	}

	SendToRoom( FIRST_SENDER_UID, DBE_DB_UPDATE_TRADE_RESULT_ACK, kPacketAck );
}
#endif SERV_TRADE_DDOS_DEFENCE
//}}

#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
IMPL_ON_FUNC( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ )
{
	KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;

	// SP 쿼리를 만들어 보자
	int iTemp_Type = 5;		// 개인 상점 sp 타입
	//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade_New ";
#else
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Trade ";
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	wstrQuerySP += boost::str( boost::wformat( L"%d" ) % iTemp_Type );

	// 판매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iSellUnitUID );
	
	// 구매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iBuyUnitUID );

	// 판매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iSellUnitEDIN );		// 수수료가 다르기 때문에 각자 처리 해야한다.

	// 구매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iBuyUnitEDOUT );		// 수수료가 다르기 때문에 각자 처리 해야한다.

	// 판매자 아이템
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_kSellItemInfo.m_iItemUID );
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_kSellItemInfo.m_kItemInfo.m_iQuantity );

	// 구매자 아이템 - 구매자는 아이템이 들어오기만 하기 때문에 없다.
	int index = 1;			// 물건 하나를 등록 했으므로 1로 초기화한다.
	int iTemp_Zero = 0;
	for( ; index < 20 ; ++index )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
	}

	DO_QUERY_NO_ARG( wstrQuerySP.c_str() );

	// 아이템은 10개 + 10개 이므로 20개로 고정한다.
	UidType iItemUID[20] = {0,};

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacketAck.m_iOK
			>> iItemUID[0]			// 판매되는 아이템
			>> iItemUID[1]
			>> iItemUID[2]
			>> iItemUID[3]
			>> iItemUID[4]
			>> iItemUID[5]
			>> iItemUID[6]
			>> iItemUID[7]
			>> iItemUID[8]
			>> iItemUID[9]
			>> iItemUID[10]			
			>> iItemUID[11]
			>> iItemUID[12]
			>> iItemUID[13]
			>> iItemUID[14]
			>> iItemUID[15]
			>> iItemUID[16]
			>> iItemUID[17]
			>> iItemUID[18]
			>> iItemUID[19]	);

			m_kODBC.EndFetch();

			if( kPacketAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"개인 거래 SP 호출이 실패 했습니다." )
					<< BUILD_LOG( kPacketAck.m_iOK )
					<< END_LOG;

				goto end_proc;
			}

			// 개인 상점은 하나의 물품만 구매 가능하므로 고정값으로 한다.
			kPacket_.m_kBuyItemInfo.m_iItemUID = iItemUID[0];
			// 거래시 봉인 상태는 SP 에서 해제한다.
			if( kPacket_.m_kBuyItemInfo.m_kItemInfo.IsSealedItem() == true )
			{
				kPacket_.m_kBuyItemInfo.m_kItemInfo.UnsealItem();
			}

			kPacketAck.m_kResult = kPacket_;
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK, kPacketAck );
}
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( DBE_WEDDING_COMPLETE_REQ )
{
	KDBE_WEDDING_COMPLETE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_wstrWeddingDate = kPacket_.m_wstrWeddingDate;

	std::wstring wstrLoveWord;

	// 결혼 했음 - 정보 업데이트
	DO_QUERY( L"exec dbo.P_GCouple_Info_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
		% 2
		% kPacket_.m_iGroom
		% kPacket_.m_iBride
		% kPacket_.m_wstrWeddingDate
		% wstrLoveWord
		% 0 );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		DO_QUERY( L"exec dbo.P_GCouple_WeddingInfo_UPD", L"%d", % kPacket_.m_iWeddingUID );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );

			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"결혼식 정보 기록 실패!" )
				<< BUILD_LOG( kPacket_.m_iWeddingUID )
				<< BUILD_LOG( kPacket_.m_iGroom )
				<< BUILD_LOG( kPacket_.m_iBride )
				<< BUILD_LOG( kPacket_.m_wstrWeddingDate )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"결혼하기 정보 기록 실패!" )
			<< BUILD_LOG( kPacket_.m_iWeddingUID )
			<< BUILD_LOG( kPacket_.m_iGroom )
			<< BUILD_LOG( kPacket_.m_iBride )
			<< BUILD_LOG( kPacket_.m_wstrWeddingDate )
			<< END_LOG;
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_WEDDING_COMPLETE_ACK, kPacket );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}