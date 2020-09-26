#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "X2Data/XSLResolveItemManager.h"
#include "X2Data/XSLRandomItemManager.h"
//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
	#include "X2Data/XSLTitleManager.h"
#endif SERV_TITLE_ITEM_NEW
//}}

#ifdef SERV_SUB_QUEST_USE_ITEM
#include "RoomListManager.h"
#endif SERV_SUB_QUEST_USE_ITEM

#include "Enum/Enum.h"

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/EventRewardRetainUserItemRefreshManager.h"
#else
	#include "RewardTable.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 
//{{ 2011. 08. 26	최육사	캐릭터 레벨업 아이템
#ifdef SERV_CHAR_LEVEL_UP_ITEM
	#include "GameSysVal/GameSysVal.h"
#endif SERV_CHAR_LEVEL_UP_ITEM
//}}
//{{ 2011. 08. 29	최육사	캐릭터 레벨업 아이템
#ifdef SERV_CHAR_LEVEL_UP_ITEM
	#include "ExpTable.h"
#endif SERV_CHAR_LEVEL_UP_ITEM
//}}

//{{ 2012. 04. 29	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
#include "X2Data/XSLBuffManager.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}
//{{ 2012. 07. 12	최육사		랜덤 큐브 오픈 딜레이 시스템
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	#include "EventDelaySystemManager.h"
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
//}}

//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "..\Common\X2Data\XSLMapData.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#ifdef SERV_SYNTHESIS_AVATAR
	#include "X2Data/XSLSynthesisManager.h"
#endif //SERV_SYNTHESIS_AVATAR
//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	#include "X2Data/XSLEnchantItemManager.h"
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#if defined(SERV_RELATIONSHIP_EVENT_INT) || defined(SERV_RECRUIT_EVENT_BASE)
	#include "X2Data/XSLStringFilter.h"
	#include "odbc/Odbc.h"
#endif //defined(SERV_RELATIONSHIP_EVENT_INT) || defined(SERV_RECRUIT_EVENT_BASE)

#if defined(SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING) || defined(SERV_ITEM_ACTION_BY_DBTIME_SETTING)
#include "GSSimLayer.h"
#endif // defined(SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING) || defined(SERV_ITEM_ACTION_BY_DBTIME_SETTING)

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
#include "GSSimLayer.h"
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#include "../Common/X2Data/XSLItem.h"
#include <boost/bind.hpp>

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_BUY_ED_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BUY_ED_ITEM_REQ, EGS_BUY_ED_ITEM_ACK );
	
	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_BUY_ED_ITEM_ACK kAck;

		// 운영자 치트인지 검사
		if( kPacket_.m_bDebug )
		{
			if( GetAuthLevel() < SEnum::UAL_GM )
			{
				START_LOG( cerr, L"운영자도 아닌데 감히." )
					<< BUILD_LOG( GetName() )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOGc( GetAuthLevel() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_VERIFY_12;
				SendPacket( EGS_BUY_ED_ITEM_ACK, kAck );
				return;
			}

			if( kPacket_.m_mapItem.empty() == false )
			{
				// 치트 로그			
				std::wstring wstrCheatContents;
				std::map< int, int >::const_iterator mitLog = kPacket_.m_mapItem.begin();
				if( mitLog != kPacket_.m_mapItem.end() )
				{
					CStringW cwstrTemp;
					cwstrTemp.Format( L"%d %d", mitLog->first, mitLog->second );
					wstrCheatContents = static_cast<LPCTSTR>(cwstrTemp);
				}

				WriteCheatLogToDB( L"/b", wstrCheatContents );
			}
		}

		//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
		if( m_kUserGuildManager.CheckBuyGuildItem( GetThisPtr<KGSUser>(), kPacket_.m_mapItem ) == false )
		{
			kAck.m_iOK = NetError::GetLastError();
			SendPacket( EGS_BUY_ED_ITEM_ACK, kAck );
			return;
		}
#endif GUILD_TEST
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_BUY_ED_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////


	KEGS_BUY_ED_ITEM_ACK kPacketAck;
	KDBE_BUY_ED_ITEM_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
#ifdef SERV_GET_ITEM_REASON_BY_CHEAT
	kPacket.m_bCheat = kPacket_.m_bDebug;
#endif SERV_GET_ITEM_REASON_BY_CHEAT

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
	std::set<int> setBanBuyItemInven;
	CTime m_tStartDate;
	CTime m_tEndDate;
	std::vector<KPacketGetItemOnOff>::iterator vitrGetItemOnOff;
	std::map<int , std::vector<KPacketGetItemOnOff> > mapGetItemOnOff =  GetKGSSimLayer()->GetTimeControlItem_Info();
	std::map<int , std::vector<KPacketGetItemOnOff> >::iterator mitGetItemOnOff = mapGetItemOnOff.find( TCIT_SHOP );

	if ( mitGetItemOnOff != mapGetItemOnOff.end() )
	{
		for ( vitrGetItemOnOff = mitGetItemOnOff->second.begin(); vitrGetItemOnOff != mitGetItemOnOff->second.end(); ++vitrGetItemOnOff )
		{
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrStartTime , m_tStartDate);
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrEndTime , m_tEndDate);

			if( CTime::GetCurrentTime() >= m_tStartDate && CTime::GetCurrentTime() <= m_tEndDate )
			{
				setBanBuyItemInven.insert(vitrGetItemOnOff->m_iItemID);
			}
		}
	}
	
	std::map< int, int >::const_iterator mitBanItem = kPacket_.m_mapItem.begin();
	bool m_bBanItemCheck = false;

	for( ; mitBanItem != kPacket_.m_mapItem.end() ; mitBanItem++)
	{
		if ( setBanBuyItemInven.find(mitBanItem->first) != setBanBuyItemInven.end())
		{
			m_bBanItemCheck = true;

			if( m_bBanItemCheck )
			{
				kPacketAck.m_iOK = NetError::ERR_ITEM_22;
				SendPacket( EGS_BUY_ED_ITEM_ACK, kPacketAck );
				return;
			}
		}
	}
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_EVENT_ADAMS_UI_SHOP
	std::map< int, int >::iterator mit2;
	for( mit2 = kPacket_.m_mapItem.begin(); mit2 != kPacket_.m_mapItem.end() ; ++mit2 )
	{
		if( mit2->first == 141000894 || mit2->first == 67006451 || mit2->first == 141000906 || mit2->first == 141000895 || mit2->first == 141000897
			|| mit2->first == 141000802 || mit2->first == 141000898 )
		{
			kPacketAck.m_iOK = NetError::ERR_BUY_ED_ITEM_06;
			SendPacket( EGS_BUY_ED_ITEM_ACK, kPacketAck );
			START_LOG( cout, L"교환상품인데 ED로 구매하려고하네?이벤트 상점" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			return;
		}
	}
#endif SERV_EVENT_ADAMS_UI_SHOP

	//{{ 2011. 06. 18	최육사	ED아이템 구매 체크
//#ifdef SERV_CHECK_BUY_ED_ITEM
	int iTotalPrice = 0;
	int iTotalVSPoint = 0;
	if( m_kInventory.PrepareBuy( kPacket_.m_mapItem, kPacket_.m_bDebug, kPacket_.m_sPeriod, iTotalPrice, iTotalVSPoint, kPacket.m_vecUpdatedInventorySlot, kPacket.m_vecBuyGPItemInfo ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_BUY_ED_ITEM_ACK, kPacketAck );

		if( kPacketAck.m_iOK == NetError::ERR_BUY_ED_ITEM_06 )
		{
			START_LOG( cout, L"NPC상점에서 팔지 않는 아이템을 구매하려 했습니다! 해킹유저로 의심됩니다!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
#else
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			// 해킹 유저에 대한 추가 처리는 이곳에서..
		}
		return;
	}
//#endif SERV_CHECK_BUY_ED_ITEM
	//}}
	
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iTotalPrice, KUserEDManager::ER_OUT_ED_BUY_ITEM_NPC_SHOP );
#else
	int iBeforeED = m_iED;

	m_iED -= iTotalPrice;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_BUY_ED_ITEM, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	// DB통계 ED
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MItemBuy, iTotalPrice );

	// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MItemBuy, iTotalPrice );
#else //SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MItemBuyED, iTotalPrice );
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_kUserPvpManager.AddAPoint( -iTotalVSPoint );
#else
	m_iVSPoint -= iTotalVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( !kPacket.m_vecBuyGPItemInfo.empty() )
	{
		m_kInventory.FlushQuantityChange( kPacket.m_kItemQuantityUpdate.m_mapQuantityChange );		
		m_kInventory.FlushEnduranceChange( kPacket.m_kItemEnduranceUpdate );
		m_kInventory.FlushDeletedItem( kPacket.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_BUY_ED_ITEM_REQ, kPacket );
	}
	else
	{
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_iED = GetED();
		//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		kPacketAck.m_iAPoint = GetAPoint();
#else
		kPacketAck.m_iVSPoint = m_iVSPoint;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		kPacketAck.m_vecInventorySlotInfo = kPacket.m_vecUpdatedInventorySlot;
		SendPacket( EGS_BUY_ED_ITEM_ACK, kPacketAck );
	}

	std::map< int, int >::iterator mit;
	for( mit = kPacket_.m_mapItem.begin(); mit != kPacket_.m_mapItem.end(); mit++ )
	{
		if( kPacket_.m_bDebug )
			continue;

		// 아이템 구입 정보 기록
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_BUY, mit->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_BUY, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}

		// 유저 통계 아이템 구입 정보 기록
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Shop_Buy_Item, 1 );


		//{{ 2011. 12. 02  김민성	AP 구매 로그 수치
#ifdef SERV_BUY_AP_ITEM_LOG
		if( 0 < iTotalVSPoint )
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( mit->first );
			if( pItemTemplet != NULL )
			{
				if( 0 < pItemTemplet->m_PricePvPPoint )
				{
					CTime tCurr = CTime::GetCurrentTime();

					KDBE_BUY_AP_ITEM_LOG_NOT kApNot;
					kApNot.m_iUnitUID = GetCharUID();
					kApNot.m_iItemID = mit->first;
					kApNot.m_iQuantity = mit->second;
					kApNot.m_iBeforAP = GetAPoint() + iTotalVSPoint;
					kApNot.m_iAfterAP = GetAPoint();
					kApNot.m_wstrBuyDate = (std::wstring)tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );

					SendToLogDB( DBE_BUY_AP_ITEM_LOG_NOT, kApNot );
				}
			}
		}
#endif SERV_BUY_AP_ITEM_LOG
		//}}
	}	

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저
	const int iRewardState = ( kPacket_.m_bDebug ? KAbuserLogManager::RS_BUY_CHEAT : KAbuserLogManager::RS_BUY_SHOP );

	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), iRewardState, kPacket_.m_mapItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( DBE_BUY_ED_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BUY_ED_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_BUY_ED_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	IncreaseED( kPacket_.m_iED, KUserEDManager::ER_IN_ED_BUY_ITEM_NPC_SHOP_ROLLBACK );
#else
	int iBeforeED = m_iED;

	m_iED += kPacket_.m_iED;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_BUY_ED_ITEM_ROLLBACK, iBeforeED );
//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	// VSPoint
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_kUserPvpManager.AddAPoint( kPacket_.m_iAPoint );
#else
	m_iVSPoint += kPacket_.m_iVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	// 결과처리
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackInitEndurance( kPacket_.m_kItemEnduranceUpdate );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	kPacket.m_iED = GetED();
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kPacket.m_iAPoint = GetAPoint();
#else
	kPacket.m_iVSPoint = m_iVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}	
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	SendPacket( EGS_BUY_ED_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_SELL_ED_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SELL_ED_ITEM_REQ, EGS_SELL_ED_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_SELL_ED_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_SELL_ED_ITEM_ACK, kAck );
			return;
		}
		//}}	

		//{{ 2010. 03. 18  최육사	클라이언트 데이터 검증
		if( kPacket_.m_iQuantity < 0 )
		{
			START_LOG( cout2, L"아이템 판매 패킷의 수량값이 마이너스 값입니다. 해킹유저로 의심됨." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( kPacket_.m_iQuantity );

			kAck.m_iOK = NetError::ERR_SELL_ED_ITEM_05;
			SendPacket( EGS_SELL_ED_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_SELL_ED_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		//{{ 2012. 05. 23	김민성       판매 불가 아이템 판매 시도 해킹 유저로 판단
#ifdef SERV_IMPOSSIBLE_SELL_ITEM
		{
			KInventoryItemInfo kInfo;
			if( m_kInventory.GetInventoryItemInfo( kPacket_.m_iItemUID, kInfo ) == true )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
				if( pItemTemplet != NULL )
				{
					if( pItemTemplet->m_PriceType == CXSLItem::SPT_NONE )
					{
						KEGS_SELL_ED_ITEM_ACK kAck;
						kAck.m_iOK = NetError::ERR_SELL_ED_ITEM_05;
						SendPacket( EGS_SELL_ED_ITEM_ACK, kAck );

						//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
						// 클라이언트에게 알려주자
						KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_IMPOSSIBLE_SELL_ITEM;
						SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kNot );

						m_kUserHackingManager.SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_IMPOSSIBLE_SELL_ITEM );
#else
						// 클라이언트에게 알려주자
						KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
						kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_IMPOSSIBLE_SELL_ITEM;
						SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kNot );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
						//}}

						// 랜덤 시간 뒤에 종료 되도록 하자
						SetRandomTimeKick( KStatistics::eSIColDR_ImpossibleSellItem, 15, 5 );

						// 계정 블럭
						HackUserRegRejectedUser( kNot.m_cHackingCheckType );

						// sn 블럭은 일단 빼자


						START_LOG( clog2, L"팔지 못하는 아이템을 팔려고 합니다. 제재 합시다." )
							<< BUILD_LOG( GetUID() )
							<< BUILD_LOG( GetCharName() )
							<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID)
							<< END_LOG;
					}
				}
			}
		}
#endif SERV_IMPOSSIBLE_SELL_ITEM
		//}}

#ifdef SERV_IMPOSSIBLE_SELL_ITEM_ACCESSORY
		{
			KInventoryItemInfo kInfo;
			if( m_kInventory.GetInventoryItemInfo( kPacket_.m_iItemUID, kInfo ) == true )
			{
				const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );
				if( pItemTemplet != NULL )
				{
					if ( pItemTemplet->m_PriceType == CXSLItem::SPT_GP && pItemTemplet->m_ItemType == CXSLItem::IT_ACCESSORY && kInfo.m_kItemInfo.m_sPeriod != 0 )
					{
						KEGS_SELL_ED_ITEM_ACK kAck;
						kAck.m_iOK = NetError::ERR_SELL_ED_ITEM_05;
						SendPacket( EGS_SELL_ED_ITEM_ACK, kAck );
					}
				}
			}
		}
#endif // SERV_IMPOSSIBLE_SELL_ITEM_ACCESSORY

		//{{ 2010. 11. 18	최육사	아이템 판매 차단
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
			{
				// 센터로 가서 게임중인지 체크하자!
				SendToCnRoom( ERM_CHECK_SELL_ED_ITEM_REQ, kPacket_ );
				return;
			}
		}
#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_SELL_ED_ITEM_ACK kPacket;
	int iItemID = 0;
	int iED = 0;
	if( !m_kInventory.ProcessSell( kPacket_.m_iItemUID, kPacket_.m_iQuantity, iItemID, iED, kPacket.m_kInventorySlotInfo ) )
	{
		kPacket.m_iOK = NetError::ERR_SELL_ED_ITEM_05;
		SendPacket( EGS_SELL_ED_ITEM_ACK, kPacket );
		return;
	}

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	IncreaseED( iED, KUserEDManager::ER_IN_ED_SELL_ITEM_NPC_SHOP );
#else
	int iBeforeED = m_iED;

	m_iED += iED;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_SELL_ED_ITEM, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	{
		// DB통계 ED
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PItemSell, iED );

		// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PItemSell, iED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PItemSellED, iED );
#endif //SERV_USER_STATISTICS_RENEWAL
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iED = GetED();
	SendPacket( EGS_SELL_ED_ITEM_ACK, kPacket );

	SendUpdateDropQuestItemByIngQuest();

	// 아이템 판매 정보 기록
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_SELL, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_SELL, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 유저 통계 아이템 판매 정보 기록
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Shop_Sell_Item, 1 );
}

//{{ 2010. 11. 18	최육사	아이템 판매 차단
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM

IMPL_ON_FUNC( ERM_CHECK_SELL_ED_ITEM_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SELL_ED_ITEM_ACK );

	KEGS_SELL_ED_ITEM_ACK kPacket;
	int iItemID = 0;
	int iED = 0;
	if( !m_kInventory.ProcessSell( kPacket_.m_iItemUID, kPacket_.m_iQuantity, iItemID, iED, kPacket.m_kInventorySlotInfo ) )
	{
		kPacket.m_iOK = NetError::ERR_SELL_ED_ITEM_05;
		SendPacket( EGS_SELL_ED_ITEM_ACK, kPacket );
		return;
	}
	
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	IncreaseED( iED, KUserEDManager::ER_IN_ED_SELL_ITEM_NPC_SHOP );
#else
	int iBeforeED = m_iED;

	m_iED += iED;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_SELL_ED_ITEM, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	{
		// DB통계 ED
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PItemSell, iED );

		// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PItemSell, iED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_PItemSellED, iED );
#endif //SERV_USER_STATISTICS_RENEWAL
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iED = GetED();
	SendPacket( EGS_SELL_ED_ITEM_ACK, kPacket );

	SendUpdateDropQuestItemByIngQuest();

	// 아이템 판매 정보 기록
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_SELL, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_SELL, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 유저 통계 아이템 판매 정보 기록
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_Shop_Sell_Item, 1 );    
}

#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
//}}

IMPL_ON_FUNC( DBE_INSERT_ITEM_ACK )
{
	// TODO : 상태 막아야 하나..

	// DB에 추가된 아이템을 인벤토리에 넣고 유저에게 수정된 데이터를 넘겨준다.
	KEGS_UPDATE_UNIT_INFO_NOT kPacket;

	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	GetUnitInfo( kPacket.m_kUnitInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	kPacket.m_mapItemObtained = kPacket_.m_mapGetItem;
#else
	kPacket.m_mapItemObtained = kPacket_.m_mapInsertedItem;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}
	
	kPacket.m_bGameEnd = true;	

	//방상태이고 pvp & dungeon 방이면 아이템처리후 정보업데이트 한다.
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			{
				if( kPacket_.m_bOutRoom == false )
				{
					//Update Server Unit data 
					KRoomUserInfo kInfo;
					GetRoomUserInfo( kInfo );

					//방에서 나가는 경우가 아닐경우 센터서버에 업데이트를 시켜준다.
					SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );
				}
			}
			break;

		default:
			break;
		}
	}	

	SendPacket( EGS_UPDATE_UNIT_INFO_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_CHANGE_INVENTORY_SLOT_ITEM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
#ifdef SERV_RESTRICTED_TO_MOVE_TO_BANK
	if( kPacket_.m_cToSlotType == CXSLInventory::ST_BANK ||
		kPacket_.m_cToSlotType == CXSLInventory::ST_PET ||
		kPacket_.m_cToSlotType == CXSLInventory::ST_SHARE_BANK )
	{
		int iItemID = m_kInventory.GetItemID( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID );
		if( SiCXSLItemManager()->IsInventoryOnly( iItemID ) == true )
		{
			KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kAck;
			SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kAck );
			return;
		}
	}
#endif SERV_RESTRICTED_TO_MOVE_TO_BANK

	//////////////////////////////////////////////////////////////////////////
#ifdef	SERV_SHARING_BANK_TEST
	if( kPacket_.m_cFromSlotType == CXSLInventory::ST_SHARE_BANK  ||  kPacket_.m_cToSlotType == CXSLInventory::ST_SHARE_BANK )
	{
		if( kPacket_.m_iShareUnitUID != m_kInventory.GetShareUnitUID() )
		{
			START_LOG( cwarn, L"클라이언트와 서버가 보고 있는 캐릭터가 다르다!" )
				<< BUILD_LOG( (int)kPacket_.m_cFromSlotType )
				<< BUILD_LOG( kPacket_.m_iFromSlotID )
				<< BUILD_LOG( (int)kPacket_.m_cToSlotType )
				<< BUILD_LOG( kPacket_.m_iToSlotID )
				<< BUILD_LOG( kPacket_.m_iShareUnitUID )
				<< BUILD_LOG( m_kInventory.GetShareUnitUID() )
				<< END_LOG;

			KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kAck;
			SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kAck );
			return;
		}

		if( kPacket_.m_cToSlotType == CXSLInventory::ST_SHARE_BANK && m_kInventory.GetShareBankSize() <= kPacket_.m_iToSlotID )
		{
			START_LOG( cwarn, L"공유은행 크기보다 큰 곳에 넣으려 했다!" )
				<< BUILD_LOG( (int)kPacket_.m_cFromSlotType )
				<< BUILD_LOG( kPacket_.m_iFromSlotID )
				<< BUILD_LOG( (int)kPacket_.m_cToSlotType )
				<< BUILD_LOG( kPacket_.m_iToSlotID )
				<< BUILD_LOG( m_kInventory.GetShareBankSize() )
				<< END_LOG;

			KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kAck;
			SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kAck );
			return;
		}

		// 공유 은행과 관련된 슬롯 이동이라면 현재 DB에 업데이트 되지 않은 정보들을 미리 모두 업데이트 하자!
		KDBE_CHANGE_INVENTORY_SLOT_ITEM_REQ kPacketToDB;
		kPacketToDB.m_cFromSlotType = kPacket_.m_cFromSlotType;
		kPacketToDB.m_iFromSlotID = kPacket_.m_iFromSlotID;
		kPacketToDB.m_cToSlotType = kPacket_.m_cToSlotType;
		kPacketToDB.m_iToSlotID = kPacket_.m_iToSlotID;
		kPacketToDB.m_iUnitUID = GetCharUID();
		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushEnduranceChange( kPacketToDB.m_kItemEnduranceUpdate );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
		m_kInventory.FlushPositionChange( kPacketToDB.m_kItemPositionUpdate );
		SendToGameDB( DBE_CHANGE_INVENTORY_SLOT_ITEM_REQ, kPacketToDB );

		//////////////////////////////////////////////////////////////////////////
		//START_LOG( cout, L"[테스트로그] 공유 은행에 관련된 슬롯 이동 동작입니다!" )
		//	<< BUILD_LOG( GetCharName() )
		//	<< BUILD_LOGc( kPacketToDB.m_cFromSlotType )
		//	<< BUILD_LOG( kPacketToDB.m_iFromSlotID )
		//	<< BUILD_LOGc( kPacketToDB.m_cToSlotType )
		//	<< BUILD_LOG( kPacketToDB.m_iToSlotID );
		//////////////////////////////////////////////////////////////////////////
		return;
	}
#endif	SERV_SHARING_BANK_TEST
	//////////////////////////////////////////////////////////////////////////

	bool bCheckCoolTime = ( GetUnitState() == CXSLUnit::CUS_DUNGEON_PLAY );

	KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kPacket;

#ifdef	SERV_SHARING_BANK_TEST
	KDBE_SEAL_ITEM_REQ kPacketSealReq;
	kPacketSealReq.m_iUnitUID = 0;
	kPacketSealReq.m_iItemUID = 0;
	kPacketSealReq.m_ucSealResult = 0;

	// 이동되는 각 슬롯에 있는 아이템 UID얻기
	const UidType iFromItemUID = m_kInventory.GetItemUID( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID );
	const UidType iToItemUID = m_kInventory.GetItemUID( kPacket_.m_cToSlotType, kPacket_.m_iToSlotID );

#ifdef SERV_PERSONAL_SHOP_NO_MOVE
	if( m_kInventory.MoveItem( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID, kPacket_.m_cToSlotType, kPacket_.m_iToSlotID, bCheckCoolTime, kPacket.m_vecInventorySlotInfo, kPacketSealReq, m_vecPersonalShopSell_ItemInfo ) == false )
#else //SERV_PERSONAL_SHOP_NO_MOVE
	if( m_kInventory.MoveItem( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID, kPacket_.m_cToSlotType, kPacket_.m_iToSlotID, bCheckCoolTime, kPacket.m_vecInventorySlotInfo, kPacketSealReq ) == false )
#endif //SERV_PERSONAL_SHOP_NO_MOVE
#else	SERV_SHARING_BANK_TEST
	if( m_kInventory.MoveItem( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID, kPacket_.m_cToSlotType, kPacket_.m_iToSlotID, bCheckCoolTime, kPacket.m_vecInventorySlotInfo ) == false )
#endif	SERV_SHARING_BANK_TEST
	{
		// 일단 클라이언트로 ack보낸다.
		SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kPacket );
		return;
	}

	// 장착일 경우, 기록
	if( kPacket_.m_cToSlotType == CXSLInventory::ST_E_EQUIP )
	{
		// 장착 정보 기록
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( m_kInventory.GetItemID( kPacket_.m_cToSlotType, kPacket_.m_iToSlotID ) );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_EQUIP, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_EQUIP, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	//////////////////////////////////////////////////////////////////////////
#ifdef	SERV_SHARING_BANK_TEST
	if( kPacketSealReq.m_iItemUID > 0 )
	{
		SendToGameDB( DBE_UPDATE_SEAL_DATA_NOT, kPacketSealReq );
	}

//  이 시점에 해줘야 하는거 아닐까? 2013.06.30 조효진 테스트 코드 (문제 해결되면 삭제해야함)
// 	KDBE_UPDATE_SHARE_ITEM_REQ kPacketSendDB;
// 	//	은행공유 기록
// 	if( true == m_kInventory.GetShareItemNeedDBUpdate( kPacketSendDB ) )
// 	{
// 		kPacketSendDB.m_bReload = false;
// 		SendToGameDB( DBE_UPDATE_SHARE_ITEM_REQ, kPacketSendDB );
// 	}
#endif	SERV_SHARING_BANK_TEST
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_SHARING_BANK_EVENT
	if( kPacket_.m_cFromSlotType == CXSLInventory::ST_BANK || kPacket_.m_cFromSlotType == CXSLInventory::ST_SHARE_BANK  &&  kPacket_.m_cToSlotType != CXSLInventory::ST_SHARE_BANK )
	{
		KInventoryItemInfo kDestItemInfo;
		if( m_kInventory.GetInventoryItemInfo( iFromItemUID, kDestItemInfo ) == false )
		{
			// 에러
		}
		else
		{
			if( kDestItemInfo.m_kItemInfo.m_iItemID == _CONST_SHARING_BANK_EVENT_ITEM_::iKeyItem && kDestItemInfo.m_cSlotCategory != CXSLInventory::ST_BANK 
				&& kDestItemInfo.m_cSlotCategory != CXSLInventory::ST_SHARE_BANK && kDestItemInfo.m_kItemInfo.GetSealCount() == 1)
			{

				KDBE_SHARING_BANK_EVENT_REQ kPacketEvent;
				kPacketEvent.m_iUserUID = GetUID();
				kPacketEvent.m_iUnitUID = GetCharUID();
				kPacketEvent.m_iItemID = _CONST_SHARING_BANK_EVENT_ITEM_::iKeyItem;

				START_LOG( clog2, L"지헌로그:이벤트 아이템을 꺼냈다!! 내 은행에서!")
					<< BUILD_LOG( kPacketEvent.m_iItemID)
					<< BUILD_LOG( kPacketEvent.m_iUserUID)
					<< BUILD_LOG( kPacketEvent.m_iUnitUID)
					<< END_LOG;

				SendToGameDB( DBE_SHARING_BANK_EVENT_REQ, kPacketEvent);
			}
		}
	}
#endif //SERV_SHARING_BANK_EVENT

	SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kPacket );

	// 장착 아이템 바꿨으면 다른 유저들에게 알려준다.
	SendUpdateChangeEquippedItem( kPacket.m_vecInventorySlotInfo );
}

//{{ 2013. 07. 10	최육사	공유 은행
#ifdef SERV_SHARING_BANK_TEST
IMPL_ON_FUNC( DBE_CHANGE_INVENTORY_SLOT_ITEM_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// [2013.7.30] 최육사, SP 중복 호출로 펫 인벤 Item, 펫UID 0 되어서 보이지 않는 문제 수정
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.RollBackInitEndurance( kPacket_.m_kItemEnduranceUpdate );
	m_kInventory.PositionUpdateComplete( kPacket_.m_kItemPositionUpdate );
	// [2013.7.30] 최육사, SP 중복 호출로 펫 인벤 Item, 펫UID 0 되어서 보이지 않는 문제 수정
	const bool bCheckCoolTime = ( GetUnitState() == CXSLUnit::CUS_DUNGEON_PLAY );

	KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kPacket;

	// 봉인 정보 업데이트
	KDBE_SEAL_ITEM_REQ kPacketSealReq;
	kPacketSealReq.m_iUnitUID = 0;
	kPacketSealReq.m_iItemUID = 0;
	kPacketSealReq.m_ucSealResult = 0;

	// 이동되는 각 슬롯에 있는 아이템 UID얻기
	const UidType iFromItemUID = m_kInventory.GetItemUID( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID );
	const UidType iToItemUID = m_kInventory.GetItemUID( kPacket_.m_cToSlotType, kPacket_.m_iToSlotID );

#ifdef SERV_PERSONAL_SHOP_NO_MOVE
	if( m_kInventory.MoveItem( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID, kPacket_.m_cToSlotType, kPacket_.m_iToSlotID, bCheckCoolTime, kPacket.m_vecInventorySlotInfo, kPacketSealReq, m_vecPersonalShopSell_ItemInfo ) == false )
#else //SERV_PERSONAL_SHOP_NO_MOVE
	if( m_kInventory.MoveItem( kPacket_.m_cFromSlotType, kPacket_.m_iFromSlotID, kPacket_.m_cToSlotType, kPacket_.m_iToSlotID, bCheckCoolTime, kPacket.m_vecInventorySlotInfo, kPacketSealReq ) == false )
#endif //SERV_PERSONAL_SHOP_NO_MOVE
	{
		// 일단 클라이언트로 ack보낸다.
		SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_SHARING_BANK_EVENT
	if( kPacket_.m_cFromSlotType == CXSLInventory::ST_BANK || kPacket_.m_cFromSlotType == CXSLInventory::ST_SHARE_BANK  &&  kPacket_.m_cToSlotType != CXSLInventory::ST_SHARE_BANK )
	{
		KInventoryItemInfo kDestItemInfo;
		if( m_kInventory.GetInventoryItemInfo( iFromItemUID, kDestItemInfo ) == false )
		{
			// 에러
		}
		else
		{
			if( kDestItemInfo.m_kItemInfo.m_iItemID == _CONST_SHARING_BANK_EVENT_ITEM_::iKeyItem && kDestItemInfo.m_cSlotCategory != CXSLInventory::ST_BANK 
				&& kDestItemInfo.m_cSlotCategory != CXSLInventory::ST_SHARE_BANK && kDestItemInfo.m_kItemInfo.GetSealCount() == 1)
			{

				KDBE_SHARING_BANK_EVENT_REQ kPacketEvent;
				kPacketEvent.m_iUserUID = GetUID();
				kPacketEvent.m_iUnitUID = GetCharUID();
				kPacketEvent.m_iItemID = _CONST_SHARING_BANK_EVENT_ITEM_::iKeyItem;

				START_LOG( clog2, L"지헌로그:이벤트 아이템을 꺼냈다!! 내 은행에서!")
					<< BUILD_LOG( kPacketEvent.m_iItemID)
					<< BUILD_LOG( kPacketEvent.m_iUserUID)
					<< BUILD_LOG( kPacketEvent.m_iUnitUID)
					<< END_LOG;

				SendToGameDB( DBE_SHARING_BANK_EVENT_REQ, kPacketEvent);
			}
		}
	}
#endif //SERV_SHARING_BANK_EVENT
	//////////////////////////////////////////////////////////////////////////

	// 장착일 경우, 기록
	if( kPacket_.m_cToSlotType == CXSLInventory::ST_E_EQUIP )
	{
		// 장착 정보 기록
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( m_kInventory.GetItemID( kPacket_.m_cToSlotType, kPacket_.m_iToSlotID ) );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_EQUIP, 1 );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_EQUIP, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 봉인 정보 업데이트
	if( kPacketSealReq.m_iItemUID > 0 )
	{
		SendToGameDB( DBE_UPDATE_SEAL_DATA_NOT, kPacketSealReq );
	}

	//////////////////////////////////////////////////////////////////////////
	// 이동한 아이템에 대해서만 DB업데이트를 하자!
	std::set< UidType > setMovedItemUID;
	setMovedItemUID.insert( iFromItemUID );
	setMovedItemUID.insert( iToItemUID );

	KDBE_UPDATE_SHARE_ITEM_REQ kPacketSendDB;
	kPacketSendDB.m_vecInventorySlotItem = kPacket.m_vecInventorySlotInfo;

	//	은행공유 기록
	if( m_kInventory.GetShareItemNeedDBUpdate( setMovedItemUID, kPacketSendDB ) == true )
	{
		//////////////////////////////////////////////////////////////////////////
		//START_LOG( cout, L"[테스트로그] 공유 은행 슬롯 이동을 위해 공유 은행의 변경점들을 DB에 업데이트 준비 하자!" )
		//	<< BUILD_LOG( GetCharName() )
		//	<< BUILD_LOG( kPacketSendDB.m_mapUpdateEndurance.size() )
		//	<< BUILD_LOG( kPacketSendDB.m_mapUpdatePosition.size() )
		//	<< BUILD_LOG( kPacketSendDB.m_mapUpdateQuantity.size() )
		//	<< BUILD_LOG( kPacketSendDB.m_mapInItem.size() )
		//	<< BUILD_LOG( kPacketSendDB.m_mapOutItem.size() )
		//	<< BUILD_LOG( kPacketSendDB.m_vecDeletedItem.size() );
		//////////////////////////////////////////////////////////////////////////

		kPacketSendDB.m_bReload = false;
		SendToGameDB( DBE_UPDATE_SHARE_ITEM_FOR_MOVE_SLOT_REQ, kPacketSendDB );

		// 여기서 리턴한다.
		return;
	}

	SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kPacket );

	// 장착 아이템 바꿨으면 다른 유저들에게 알려준다.
	SendUpdateChangeEquippedItem( kPacket.m_vecInventorySlotInfo );
}
#endif SERV_SHARING_BANK_TEST
//}}

IMPL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	// 던전 라운지에서 장비교체
	KEGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT kPacketNot;
	kPacketNot.m_iUnitUID			  = kPacket_.m_iUnitUID;
	kPacketNot.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
//#ifdef SERV_TITLE_DATA_SIZE
	kPacketNot.m_iEquippedTitleID	  = kPacket_.m_iEquippedTitleID;
//#else
//	kPacketNot.m_sEquippedTitleID	  = kPacket_.m_sEquippedTitleID;
//#endif SERV_TITLE_DATA_SIZE
	//}}
	SendPacket( EGS_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, kPacketNot );
}

IMPL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT kPacket;
	kPacket.m_UnitUID			   = kPacket_.m_UnitUID;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
//#ifdef SERV_TITLE_DATA_SIZE
	kPacket.m_iEquippedTitleID	   = kPacket_.m_iEquippedTitleID;
//#else
//	kPacket.m_sEquippedTitleID	   = kPacket_.m_sEquippedTitleID;
//#endif SERV_TITLE_DATA_SIZE
	//}}
	SendPacket( EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT, kPacket );
}

IMPL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT kPacket;
	//{{ 2010. 12. 23	최육사	장비 변경 패킷 타입
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
	kPacket.m_cChangeEquippedPacketType = kPacket_.m_cChangeEquippedPacketType;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
	//}}
	kPacket.m_UnitUID			   = kPacket_.m_UnitUID;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
	kPacket.m_kGameStat			   = kPacket_.m_kRoomUserInfo.m_kGameStat;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
//#ifdef SERV_TITLE_DATA_SIZE
	kPacket.m_iEquippedTitleID	   = kPacket_.m_kRoomUserInfo.m_iTitleID;
//#else
//	kPacket.m_sEquippedTitleID	   = kPacket_.m_kRoomUserInfo.m_sTitleID;
//#endif SERV_TITLE_DATA_SIZE
	//}}	
	SendPacket( EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_DELETE_ITEM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_DELETE_ITEM_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_DELETE_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	const bool bDebug = ( GetAuthLevel() >= SEnum::UAL_GM );
	
	//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
	// 아이템 삭제	
	const int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY

	KEGS_DELETE_ITEM_ACK kPacket;
	if( m_kInventory.DiscardItem( kPacket_.m_iItemUID, kPacket_.m_iQuantity, kPacket.m_kInventoryItemInfo, bDebug ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_DELETE_ITEM_ACK, kPacket );
		return;
	}
	
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_DELETE_ITEM_ACK, kPacket );

	SendUpdateDropQuestItemByIngQuest();

	//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY
	switch( iItemID )
	{
	case CXSLItem::EI_EXPAND_SKILL_SLOT_EVENT:
	case CXSLItem::CI_EXPAND_SKILL_SLOT_IN_PACKAGE:
		{
#ifdef	SERV_SKILL_SLOT_ITEM_BUG_FIX	// 적용날짜: 2013-07-04				
			// 무제한 기준 (2049-12-31 23:59) 으로 기간 셋팅 되어 있음
			CTime tLimitEndDate = CTime( 2049, 12, 31, 23, 59, 0 );
			CTime tSkillSlotBEndDate;
			m_kSkillTree.GetSkillSolotBEndDate( tSkillSlotBEndDate );		
			if( tLimitEndDate == tSkillSlotBEndDate )
			{
				// 스킬 슬롯 무제한 구매했었다면 갱신 할 필요 없다.
				break;
			}
#endif	// SERV_SKILL_SLOT_ITEM_BUG_FIX

			// 비어있는걸로 넣으면 초기화 된다
			std::wstring wstrSkillSlotBEndDate;
			m_kSkillTree.SetSkillSolotBEndDate( wstrSkillSlotBEndDate );		
			m_kSkillTree.ExpireSkillSlotB();

			KEGS_SKILL_SLOT_CHANGE_ITEM_NOT kpacket;

			kpacket.m_cSkillSlotBExpirationState = KUserSkillTree::SSBES_EXPIRED;
			kpacket.m_wstrSkillSlotBEndDate = wstrSkillSlotBEndDate;

			SendPacket( EGS_SKILL_SLOT_CHANGE_ITEM_NOT, kpacket );
		}
		break;
	}
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY
}

IMPL_ON_FUNC( EGS_USE_QUICK_SLOT_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_USE_QUICK_SLOT_ACK kAck;

		//{{ 2009. 7. 3  최육사		헤니르 시공
		if( IsHenirDungeonChallengeMode() )
		{
			START_LOG( cerr, L"클라이언트에서 이벤트 던전인지 체크했을텐데.." )
				<< BUILD_LOG( GetCharUID() )			
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_09;
			SendPacket( EGS_USE_QUICK_SLOT_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_USE_QUICK_SLOT_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
        
#ifdef SERV_BATTLE_FIELD_SYSTEM // 배틀필드에서 npc소환 카드 사용 금지
        if ( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
        {
            int iItemID = m_kInventory.GetItemID( CXSLInventory::ST_E_QUICK_SLOT, kPacket_.m_sSlotID );
            const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );

            if ( pItemTemplet != NULL ) 
            {
                std::vector<CXSLItem::SpecialAbility>::const_iterator vit;
                vit = std::find_if( pItemTemplet->m_SpecialAbilityList.begin(), pItemTemplet->m_SpecialAbilityList.end(), 
                    boost::bind( &CXSLItem::SpecialAbility::m_Type, _1 ) == CXSLItem::SAT_SUMMON_MONSTER );

                if ( vit != pItemTemplet->m_SpecialAbilityList.end() ) 
                {
                    kAck.m_iOK = NetError::ERR_USE_ITEM_IN_QUICKSLOT_00;
                    SendPacket( EGS_USE_QUICK_SLOT_ACK, kAck );
                    return;
                }
            }
        }
#endif SERV_BATTLE_FIELD_SYSTEM
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_USE_QUICK_SLOT_ACK kPacket;

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_ItemID = 0;

	//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
	if( !m_kInventory.UseQuickSlotItem( ( int )kPacket_.m_sSlotID, kPacket.m_ItemID, kPacket.m_InventorySlotInfo ) )
//#else
//	if( !m_kInventory.UseQuickSlotItem( ( int )kPacket_.m_cSlotID, kPacket.m_ItemID, kPacket.m_InventorySlotInfo ) )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
	//}}	
	{
		// cool time에 걸려 사용못하는 경우가 있음.
		START_LOG( cwarn, L"퀵슬롯 아이템 사용 실패." )
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			<< BUILD_LOG( kPacket_.m_sSlotID )
//#else
//			<< BUILD_LOGc( kPacket_.m_cSlotID )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			<< BUILD_LOG( kPacket.m_ItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_08;
		SendPacket( EGS_USE_QUICK_SLOT_ACK, kPacket );
		return;
	}

	SendPacket( EGS_USE_QUICK_SLOT_ACK, kPacket );

	// 아이템 사용 정보 기록
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( kPacket.m_ItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( GetRoomUID() != 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	{
		KEGS_USE_QUICK_SLOT_NOT kPacketNot;
		kPacketNot.m_UnitUID = GetCharUID();
		kPacketNot.m_ItemID = kPacket.m_ItemID;
		
#ifdef SERV_SUB_QUEST_USE_ITEM
	int iDungeonID = 0;
	char cDifficulty = -1;

	if( GetRoomUID() != 0 )
	{
		KRoomInfoPtr spRoomInfo = SiKRoomListManager()->GetRoomInfo( GetRoomUID() );
		if( spRoomInfo != NULL )
		{
			if( spRoomInfo->m_RoomState == CXSLRoom::RS_PLAY )
			{
				iDungeonID = spRoomInfo->m_iDungeonID;
				cDifficulty = spRoomInfo->m_DifficultyLevel;
			}
		}
	}

	m_kUserQuestManager.CountUseItem( GetThisPtr<KGSUser>() ,iDungeonID, cDifficulty, kPacket.m_ItemID );
#endif SERV_SUB_QUEST_USE_ITEM
		
		SendToCnRoom( ERM_USE_QUICK_SLOT_NOT, kPacketNot );
	}
}

//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
IMPL_ON_FUNC( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK kAck;

		//{{ 2009. 7. 3  최육사		헤니르 시공
		if( IsHenirDungeonChallengeMode() )
		{
			START_LOG( cerr, L"클라이언트에서 이벤트 던전인지 체크했을텐데.." )
				<< BUILD_LOG( GetCharUID() )			
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_09;
			SendPacket( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_ItemID = 0;
	
	if( m_kInventory.UseDefenceDungeonQuickSlotItem( ( int )kPacket_.m_sSlotID, kPacket.m_ItemID, kPacket.m_InventorySlotInfo ) == false )
	{
		// cool time에 걸려 사용못하는 경우가 있음.
		START_LOG( cwarn, L"퀵슬롯 아이템 사용 실패." )
			<< BUILD_LOG( kPacket_.m_sSlotID )
			<< BUILD_LOG( kPacket.m_ItemID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_08;
		SendPacket( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK, kPacket );
		return;
	}

	SendPacket( EGS_USE_DEFENCE_DUNGEON_QUICK_SLOT_ACK, kPacket );

	// 아이템 사용 정보 기록
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( kPacket.m_ItemID );
	KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_USE, 1 );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_USE, 1 );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}

	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( GetRoomUID() != 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	{
		KEGS_USE_QUICK_SLOT_NOT kPacketNot;
		kPacketNot.m_UnitUID = GetCharUID();
		kPacketNot.m_ItemID = kPacket.m_ItemID;
		SendToCnRoom( ERM_USE_QUICK_SLOT_NOT, kPacketNot );
	}
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

_IMPL_ON_FUNC( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_USE_QUICK_SLOT_NOT, kPacket_ );

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	// 보상 지급에 필요한 아이템을 사용 했다고 체크 해 두자.
	if(kPacket_.m_bUseItemNeedPayment == true && GetCharUID() == kPacket_.m_UnitUID)
	{
		if(m_kInventory.GetUseItemNeedPayment() != true)
			m_kInventory.SetUseItemNeedPayment( true );
	}
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
}

IMPL_ON_FUNC( EGS_REPAIR_ITEM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_ROOM, KGSFSM::S_FIELD_MAP ) );	

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_REPAIR_ITEM_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_REPAIR_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_REPAIR_ITEM_ACK kPacket;

	int		iED;
	int		iVP;
	//방에 있을경우 장착아이템중 내구도가 0인 아이템이 수리되었으면 방에 접속중인 유저들에게 알려주기위해.
	std::vector<UidType>	vecZeroItemUID;
	bool	bIsNot = m_kInventory.GetZeroOfEndurancebyEquippedItem( kPacket_.m_vecItemUID, vecZeroItemUID );

	//수리가능한지 검사하여 아이템을 수리한다.
	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	if( !m_kInventory.RepairItem( kPacket_.m_vecItemUID, iED, iVP, kPacket.m_vecInventorySlotInfo, GetThisPtr<KGSUser>() ) )
#endif SERV_SERVER_BUFF_SYSTEM
	//}
	{
		kPacket.m_iOK = NetError::ERR_ITEM_07;
		SendPacket( EGS_REPAIR_ITEM_ACK, kPacket );
		return;
	}
	
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iED, KUserEDManager::ER_OUT_ED_ITEM_REPAIR ); //수리가 완료되었으면 비용을 정산후 유저에게 알려줌.
#else
	int iBeforeED = m_iED;
	
	m_iED -= iED; //수리가 완료되었으면 비용을 정산후 유저에게 알려줌.

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_ITEM_REPAIR, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	// DB통계 ED
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MItemRepair, iED );

	// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MItemRepair, iED );
#else //SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MItemRepairED, iED );
#endif //SERV_USER_STATISTICS_RENEWAL

	// VSPoint감소처리
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_kUserPvpManager.AddAPoint( -iVP );
#else
	m_iVSPoint -= iVP;
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iED = GetED();
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kPacket.m_iAPoint = GetAPoint();
#else
	kPacket.m_iVP = GetVSPoint();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	SendPacket( EGS_REPAIR_ITEM_ACK, kPacket );

	//게임방에서 장비를 수정할경우 방유저 정보에 업데이트를 시켜준다.
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 1.방에 입장해있는지 검사.
		if( GetRoomUID() == 0 )
			return;

		// 2.방이라면 대전/던전 방인지 검사.
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
			{
				KRoomUserInfo kInfo;
				GetRoomUserInfo( kInfo );

				//3.조건이 만족한다면 현재속한 방에 개인정보를 업데이트 한다.
				if( GetRoomUID() != 0 )
				{
					SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );

					//3.1 수리된 아이템중 장착중이고 내구도가 0 이었던 아이템은 방에 접속중인 유저들에게 알려준다.
					KEGS_REPAIR_ITEM_NOT kNot;
					if( bIsNot == true && vecZeroItemUID.size() > 0 )
					{
						//조건에 맞는 아이템중..
						for( int i = 0; i < (int)vecZeroItemUID.size(); ++i )
						{
							//수리된 아이템이 있다면..
							for( int j = 0; j < (int)kPacket.m_vecInventorySlotInfo.size(); ++j )
							{
								//패킷에 셋팅한다.
								if( kPacket.m_vecInventorySlotInfo[j].m_iItemUID == vecZeroItemUID[i] )
								{
									kNot.m_vecInventorySlotInfo.push_back( kPacket.m_vecInventorySlotInfo[j] );
									break;
								}
							}
						}

						//수리된 아이템을 방에 있는 유저에게 알려줌.
						if( kNot.m_vecInventorySlotInfo.size() > 0 )
						{
							kNot.m_UnitUID = GetCharUID();
							SendToCnRoom( ERM_REPAIR_ITEM_NOT, kNot );
						}
					}
				}
			}
			break;
		}
	}
}

_IMPL_ON_FUNC( ERM_REPAIR_ITEM_NOT, KEGS_REPAIR_ITEM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_REPAIR_ITEM_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_OPEN_RANDOM_ITEM_REQ )
{
	//{{ 2012. 07. 12	최육사		랜덤 큐브 오픈 딜레이 시스템
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	if( SiKGameSysVal()->IsEnableOpenRandomCubeDelaySystem() == true )
	{
		if( kPacket_.m_bDelayedOK == false )
		{
			VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_OPEN_RANDOM_ITEM_REQ, EGS_OPEN_RANDOM_ITEM_ACK );

			SiKEventDelaySystemManager()->RegOpenRandomCubeEvent( GetCharUID(), kPacket_ );

			// 여기서 리턴
			return;
		}
	}
	else
	{
		VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_OPEN_RANDOM_ITEM_REQ, EGS_OPEN_RANDOM_ITEM_ACK );
	}
#else
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_OPEN_RANDOM_ITEM_REQ, EGS_OPEN_RANDOM_ITEM_ACK );
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	//}}	

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_OPEN_RANDOM_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_BUY_ED_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_OPEN_RANDOM_ITEM_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();

	// 부활석 충전, 근성도 회복 큐브
	int iRessurectionCount = 0;	
	int iRestoreSpirit = 0;
	bool bIsCharmItem = false;

	// 큐브의 ItemID얻기
	const int iRandomItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	if( iRandomItemID == 0 )
	{
		START_LOG( cerr, L"존재하지 않는 아이템 입니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
		return;
	}

#ifdef SERV_CUBE_IN_ITEM_MAPPING
#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	std::map< int, std::vector<KRandomItemMappingToInfoServer> > m_mapCubeInItemMapping = GetKGSSimLayer()->GetTimeControlCubeInItemMapping_Info();
	std::map< int, std::vector<KRandomItemMappingToInfoServer> >::const_iterator itCubeInItemMapping = m_mapCubeInItemMapping.begin();

	SiCXSLRandomItemManager()->m_mapRandomItemMappingTime.clear();
	SiCXSLRandomItemManager()->m_mapRandomItemMappingData.clear();

	for(; itCubeInItemMapping != m_mapCubeInItemMapping.end(); itCubeInItemMapping++)
	{
		std::vector<KRandomItemMappingToInfoServer> KnowCubeMappingData = itCubeInItemMapping->second;
		std::vector<KRandomItemMappingToInfoServer>::const_iterator itKnowCubeMappingData = KnowCubeMappingData.begin();

		for(; itKnowCubeMappingData != KnowCubeMappingData.end(); itKnowCubeMappingData++)
		{
			if(itCubeInItemMapping->first == itKnowCubeMappingData->m_iIndex)
			{
				//const char* tStartTime = KncUtil::toNarrowString(itKnowCubeMappingData->m_tStartTime).c_str();
				//const char* tEndTime = KncUtil::toNarrowString(itKnowCubeMappingData->m_tEndTime).c_str();

				//START_LOG( cerr, L"SiCXSLRandomItemManager()->AddRandomItemMapping" )
					//<< BUILD_LOG( itKnowCubeMappingData->m_iIndex )
					//<< BUILD_LOG( itKnowCubeMappingData->m_iCubeID )
					//<< BUILD_LOG( itKnowCubeMappingData->m_iKeyItemID )
					//<< BUILD_LOG( itKnowCubeMappingData->m_iBeforeGroupID )
					//<< BUILD_LOG( itKnowCubeMappingData->m_iAfterGroupID )
					//<< BUILD_LOG( itKnowCubeMappingData->m_tStartTime )
					//<< BUILD_LOG( itKnowCubeMappingData->m_tEndTime )
					//<< END_LOG;


				SiCXSLRandomItemManager()->AddRandomItemMappingData_LUA(itKnowCubeMappingData->m_iIndex, itKnowCubeMappingData->m_iCubeID, itKnowCubeMappingData->m_iKeyItemID, itKnowCubeMappingData->m_iBeforeGroupID, itKnowCubeMappingData->m_iAfterGroupID);
				SiCXSLRandomItemManager()->AddRandomItemMappingTime_LUA(itKnowCubeMappingData->m_iIndex, itKnowCubeMappingData->m_tStartTime, itKnowCubeMappingData->m_tEndTime );
			}
		}
	}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	std::map<int, RANDOMITEM_MAPPING_TIME>::iterator mitRITime = SiCXSLRandomItemManager()->m_mapRandomItemMappingTime.begin();

	CTime tCurr = CTime::GetCurrentTime();

	KEGS_CUBE_IN_ITEM_MAPPING_NOT kPacketCubeInItemMapping;
	std::map<int, std::map<int, kRandomItemMappingToInfoClient> > mapNowMapping;
	mapNowMapping.clear();

	for(; mitRITime != SiCXSLRandomItemManager()->m_mapRandomItemMappingTime.end(); mitRITime++)
	{
		// 시작시간과 종료시간 안이면, 바꿔줌.
		int nIndexData = mitRITime->first;
		std::map<int, RANDOMITEM_MAPPING_DATA>::iterator mitRIData = SiCXSLRandomItemManager()->m_mapRandomItemMappingData.find(nIndexData);
		//예외처리
		if(mitRIData == SiCXSLRandomItemManager()->m_mapRandomItemMappingData.end())
			continue;

		CTime tStartTime = mitRITime->second.tStartTime;
		CTime tEndTime = mitRITime->second.tEndTime;

		// 매핑 여부 확인해주자
		kRandomItemMappingToInfoClient kRandomToInfoClient;
		kRandomToInfoClient.m_bCheckMapping  = false;
		if(  tStartTime < tCurr && tEndTime > tCurr )
		{

			START_LOG( clog, L"ModifyMapItemGroup, 큐브매핑작업Start" )
				<< BUILD_LOG( nIndexData )
				<< BUILD_LOG( tStartTime.GetYear() )
				<< BUILD_LOG( tStartTime.GetMonth() )
				<< BUILD_LOG( tStartTime.GetDay() )
				<< BUILD_LOG( tStartTime.GetHour() )
				<< BUILD_LOG( tStartTime.GetMinute() )
				<< BUILD_LOG( tEndTime.GetYear() )
				<< BUILD_LOG( tEndTime.GetMonth() )
				<< BUILD_LOG( tEndTime.GetDay() )
				<< BUILD_LOG( tEndTime.GetHour() )
				<< BUILD_LOG( tEndTime.GetMinute() )
				<< END_LOG;

			if(!SiCXSLRandomItemManager()->ModifyMapItemGroup(mitRIData->second))
			{
				START_LOG( cerr, L"해당 랜덤 아이템에 대한 키템 정보가 없다. 있을수 없는에러!" )
					<< BUILD_LOG( mitRIData->second.iCubeID )
					<< BUILD_LOG( mitRIData->second.iKeyItemID )
					<< END_LOG;
			}
			else
			{
				kRandomToInfoClient.m_bCheckMapping = true;
			}
		}
		// 매핑 정보를 담아서 넘겨준다. 
		int iCubeID = mitRIData->second.iCubeID;
		kRandomToInfoClient.m_iBeforeGroupID = mitRIData->second.iBeforeGroupID;
		kRandomToInfoClient.m_iAfterGroupID = mitRIData->second.iAfterGroupID;
		kRandomToInfoClient.m_tStartTime = (CStringW)(tStartTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		kRandomToInfoClient.m_tEndTime = (CStringW)(tEndTime.Format(_T("%Y-%m-%d %H:%M:%S")));

		std::map<int, kRandomItemMappingToInfoClient> mapToInfoClient;
		
		// 벡터에서 시간을 가져온다.
		std::map<int, std::map<int,kRandomItemMappingToInfoClient> >::iterator mitToInfoC = mapNowMapping.find(iCubeID);
		if(mitToInfoC == mapNowMapping.end())
		{
			mapToInfoClient.insert(std::make_pair(nIndexData, kRandomToInfoClient));
			mapNowMapping.insert(std::make_pair(iCubeID,mapToInfoClient));
		} 
		else // 이미 존재하고 있는 아이템이라면
		{
			mitToInfoC->second.insert(std::make_pair(nIndexData, kRandomToInfoClient));
		}
	}
	kPacketCubeInItemMapping.m_mapCubeInItemMapping = mapNowMapping;
	SendPacket( EGS_CUBE_IN_ITEM_MAPPING_NOT, kPacketCubeInItemMapping );
#endif SERV_CUBE_IN_ITEM_MAPPING

	//{{ 2012. 07. 09	김민성       이벤트 큐브 오픈 가능 시간
#ifdef SERV_EVENT_CUBE_OPEN_TIME
	if( iRandomItemID == CXSLItem::EI_EVENT_CASH_CUBE )
	{
		CTime tCurr = CTime::GetCurrentTime();

		// 오픈 가능 시간이 되지 않았습니다.
		if( SiKGameSysVal()->GetEventCubeOpenEventStartTime() > tCurr )
		{
			KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_RANDOM_ITEM_10;
			SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
			return;
		}

		// 오픈 가능 시간이 지나 오픈 할 수 없습니다.
		if( SiKGameSysVal()->GetEventCubeOpenEventEndTime() < tCurr )
		{
			KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_RANDOM_ITEM_11;
			SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
			return;
		}
	}
#endif SERV_EVENT_CUBE_OPEN_TIME
	//}}

	//{{ 2011. 12. 05	최육사	축복의 메달 큐브 체크
#ifdef SERV_CHECK_BLESSING_MEDAL_CUBE
	if( iRandomItemID == CXSLItem::EI_BLESSING_MEDAL_CUBE )
	{
		if( m_kInventory.IsExistWithBank( CXSLItem::SI_BLESSING_MEDAL ) == true )
		{
			KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_RANDOM_ITEM_09;
			SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
			return;
		}
	}
#endif SERV_CHECK_BLESSING_MEDAL_CUBE
	//}}

	//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	if( iRandomItemID == CXSLItem::EI_XMAS_RANDOM_CUBE 
		//{{ 2012. 07. 09	김민성       이벤트 큐브 오픈 가능 시간
#ifdef SERV_EVENT_CUBE_OPEN_TIME
	 || iRandomItemID == CXSLItem::EI_EVENT_LOTTO_CUBE 
	 || iRandomItemID == CXSLItem::EI_EVENT_GOLD_CUBE 
#endif SERV_EVENT_CUBE_OPEN_TIME
		//}}		
	  )
	{
		if( m_kInventory.IsEnoughEmptySlot( CXSLInventory::ST_QUICK_SLOT, 1 ) == false
		||  m_kInventory.IsEnoughEmptySlot( CXSLInventory::ST_SPECIAL, 2 ) == false )
		{
			KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_RANDOM_ITEM_07;
			SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
			return;
		}

		KELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ kPacketToLg;
		//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		kPacketToLg.m_iRandomItemID = iRandomItemID;
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}
		kPacketToLg.m_iItemUID = kPacket_.m_iItemUID;
		kPacketToLg.m_iUserUID = GetUID();
		kPacketToLg.m_iUnitUID = GetCharUID();
		SendToLoginServer( ELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ, kPacketToLg );
		return;
	}
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}

	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	kPacketToDB.m_iSpendED = 0;
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}

	// 랜덤 아이템 열기
	if( m_kInventory.OpenRandomItem( GetUnitClass(), 
		kPacket_.m_iItemUID, 
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT //SERV_ALL_RENEWAL_SP
		0,
#endif //SERV_TIME_OPEN_RANDOM_ITEM_EVENT //SERV_ALL_RENEWAL_SP
		kPacketToDB.m_mapInsertedItem,
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo, 
		iRessurectionCount, 
		iRestoreSpirit, 
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		kPacketToDB.m_iSpendED, 
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		bIsCharmItem ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();

		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
		return;
	}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
	KOpenRandomItemStorage	kStorage;
	int						iAfterWorkStorageKey;

	kStorage.m_iRessurectionCount		= iRessurectionCount;
	kStorage.m_iRestoreSpirit			= iRestoreSpirit;
	kStorage.m_iRandomItemID			= iRandomItemID;
	kStorage.m_bIsCharmItem				= bIsCharmItem;
	kStorage.m_iSpendED					= kPacketToDB.m_iSpendED;
	kStorage.m_mapInsertedItem			= kPacketToDB.m_mapInsertedItem;
	kStorage.m_vecUpdatedInventorySlot	= kPacketToDB.m_vecUpdatedInventorySlot;
	kStorage.m_vecItemInfo				= kPacketToDB.m_vecItemInfo;

	if( m_kAfterWorkStorage.InsertData( kStorage, iAfterWorkStorageKey ) == KAfterWorkStorage::AWS_SUCCEED )
	{
		bool bCheck = false;
		BOOST_TEST_FOREACH( KItemInfo&, kGetItemInfo, kPacketToDB.m_vecItemInfo )
		{
			//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			bool isEnchantCube = false;
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kGetItemInfo.m_iItemID );
			if( pItemTemplet != NULL )
			{
				if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
				{
					// 고정 강화 큐브인가?
					if( CXSLItem::UpdateEnchantedItemCubeReward( iRandomItemID, kGetItemInfo ) == true )
					{
						isEnchantCube = true;
					}
				}
			}
			
			// 랜덤 큐브인가? ( 고정 강화 큐브가 아니어야 한다 )
			if( isEnchantCube == false && SiCXSLEnchantItemManager()->CheckRandomEnchant( kGetItemInfo.m_iItemID, CXSLEnchantItemManager::RET_CUBE ) == true )
			{
				bCheck = true;
				break;
			}
#else
			if( SiCXSLEnchantItemManager()->CheckRandomEnchant( kGetItemInfo.m_iItemID, CXSLEnchantItemManager::RET_CUBE ) == true )
			{
				bCheck = true;
				break;
			}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
		}

		if( ( bCheck == true ) &&
			( GetKGameServer()->PopFirstTimeEnchantEvent() == true )
			)
		{
			KEGB_TIME_ENCHANT_EVENT_NOT kPacket;
			kPacket.m_iGSUID				= GetKGameServer()->GetUID();
			kPacket.m_iUnitUID				= GetCharUID();
			kPacket.m_iAfterWorkStorageKey	= iAfterWorkStorageKey;
			kPacket.m_usEventID				= EGB_TIME_ENCHANT_EVENT_CUBE_NOT;
			SendToGlobalServer( EGB_TIME_ENCHANT_EVENT_NOT, kPacket );
		}
		else
		{
			KEGB_TIME_ENCHANT_EVENT_CUBE_NOT kPacket;
			kPacket.m_iAfterWorkStorageKey = iAfterWorkStorageKey;
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
			kPacket.m_bNotifyGetItem = kPacket_.m_bNotifyGetItem;
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_GS_USER, NULL, EGB_TIME_ENCHANT_EVENT_CUBE_NOT, kPacket );
			QueueingEvent( spEvent );
		}
	}
	else
	{
		START_LOG( cerr, L"AfterWorkStorage: Key값을 얻을 수 없습니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// 랜덤 큐브 오픈 실패( AfterWorkStorage Key 값을 얻을 수 없습니다. )
		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
		return;
	}
#else // SERV_TIME_ENCHANT_EVENT
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	BOOST_TEST_FOREACH( KItemInfo&, kGetItemInfo, kPacketToDB.m_vecItemInfo )
	{
		// 랜덤 강화 레벨 적용
		SiCXSLEnchantItemManager()->RandomEnchant( kGetItemInfo.m_iItemID, CXSLEnchantItemManager::RET_CUBE, kGetItemInfo.m_cEnchantLevel );

		// 9강 이상인 경우 전체 공지
		SendEnchantNotice( kGetItemInfo.m_iItemID, kGetItemInfo.m_cEnchantLevel, iRandomItemID );

		// 특정 등급의 아이템인 경우 밀봉 처리
		SiCXSLItemManager()->ItemSealProcess( kGetItemInfo.m_iItemID
			, kGetItemInfo.m_sPeriod
			, kGetItemInfo.m_ucSealData
			);
	}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2008. 12. 30  최육사	매력아이템 로그
	if( bIsCharmItem )
	{		
		//{{ 2009. 6. 22  최육사	매력아이템 공지
		BOOST_MAP_CONST_FOREACH( int, int, kGetItem, kPacketToDB.m_mapInsertedItem )
		{
			// 공지 날릴만한 아이템이면 날리자!
			if( SiCXSLRandomItemManager()->IsAttrationItem( kGetItem.first ) == true )
			{
				KEGS_NOTIFY_MSG_NOT	kPacket;
				if( SiCXSLRandomItemManager()->MakeNotifyMsgAttrationItem( kGetItem.first, GetCharName(), kPacket.m_wstrMSG ) == true )
				{
					kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM; // 아이템 획득 공지
					kPacket.m_Count = 1;
					//{{ 아이템 획득 공지에 큐브 이름 출력 - 김민성
#ifdef SERV_GET_ITEM_NOTIFY
					kPacket.m_iRandomItem = iRandomItemID;
#endif SERV_GET_ITEM_NOTIFY
					//}}
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
					if( true == kPacket_.m_bNotifyGetItem )
						SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
					else
						SendPacket( EGS_NOTIFY_MSG_NOT, kPacket );
#else // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
					SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
				}
			}

#ifdef SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
			//	봉인시킬 아이템 찾기
			if( SiCXSLRandomItemManager()->IsSealedItem_Global( kGetItem.first ) == true )
			{
				//	봉인
				kPacketToDB.m_setSealRandomItem.insert(kGetItem.first );
			}
#endif //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL

			// 매력 아이템의 결과물에 대한 로그 기록!
			KELOG_CHARM_ITEM_LOG_NOT kCharmLog;
			kCharmLog.m_iUnitUID	  = GetCharUID();
			kCharmLog.m_iCharmItemID  = iRandomItemID;
			kCharmLog.m_iResultItemID = kGetItem.first;
			kCharmLog.m_iQuantity	  = kGetItem.second;
			SendToLogDB( ELOG_CHARM_ITEM_LOG_NOT, kCharmLog );
		}
	}
	//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
	else
	{
		// 봉인된 아이템이 나오는 랜덤 아이템 큐브인지 확인!
		if( SiCXSLRandomItemManager()->IsSealRandomItemCube( iRandomItemID ) == true )
		{
			BOOST_MAP_CONST_FOREACH( int, int, kGetItem, kPacketToDB.m_mapInsertedItem )
			{
				// 공지 날릴만한 아이템이면 날리자!
				if( SiCXSLRandomItemManager()->IsNotifyResultItem( kGetItem.first ) == true )
				{
					KEGS_NOTIFY_MSG_NOT	kPacket;
					if( SiCXSLRandomItemManager()->MakeNotifyMsgRandomResultItem( kGetItem.first, GetCharName(), kPacket.m_wstrMSG ) == true )
					{
						kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM; // 아이템 획득 공지
						kPacket.m_Count = 1;
						//{{ 아이템 획득 공지에 큐브 이름 출력 - 김민성
#ifdef SERV_GET_ITEM_NOTIFY
						kPacket.m_iRandomItem = iRandomItemID;
#endif SERV_GET_ITEM_NOTIFY
						//}}
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
						if( true == kPacket_.m_bNotifyGetItem )
							SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
						else
							SendPacket( EGS_NOTIFY_MSG_NOT, kPacket );
#else // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
						SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
					}
				}
			}
		}		
	}
#endif SERV_SEALED_RANDOM_ITEM
	//}}
	//}}

	//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	bool bResurrectionStone = false;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	//}}

	// 부활석 충전 큐브
	if( iRessurectionCount > 0 )
	{
		// 부활석 충전
		m_iNumResurrectionStone += iRessurectionCount;
		iRessurectionCount = m_iNumResurrectionStone;
		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		bResurrectionStone = true;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}
	}

	// 근성도 회복 큐브
	if( iRestoreSpirit > 0 )
	{
		//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		iRestoreSpirit = m_kUserSpiritManager.RestoreSpirit( iRestoreSpirit );
#else
		// 근성도 최대치 검사
		if( m_iSpiritMax < ( m_iSpirit + iRestoreSpirit ) )
		{
			m_iSpirit = m_iSpiritMax;
		}
		else
		{
			m_iSpirit += iRestoreSpirit;
		}
		iRestoreSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		

		// 1.채널에 속해있는지 검사.
		if( GetRoomUID() != 0 )
		{
			// 2.채널이라면 대전/던전 방인지 검사.
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_DUNGEON:
				{
					KRoomUserInfo kInfo;
					GetRoomUserInfo( kInfo );

					// 3.조건이 만족한다면 현재속한 방에 개인정보를 업데이트 한다.
					if( GetRoomUID() != 0 )
					{
						SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );

						// 3.1 채워진 근성도를 방의 다른 유저들에게 전송한다.
						KEGS_RESTORE_SPIRIT_NOT kNot;
						kNot.m_iUnitUID = GetCharUID();
						//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
						kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit();
#else
						kNot.m_iSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
						//}}						
						SendToCnRoom( ERM_RESTORE_SPIRIT_NOT, kNot );
					}
				}
				break;
			}			
		}
	}

	//{{ 2011. 12. 19	최육사	랜덤큐브 DB실시간 적용
#ifdef SERV_OPEN_CUBE_REALTIME_DB_UPDATE
#else
	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( kPacketToDB.m_vecItemInfo.empty() == false )
#endif SERV_OPEN_CUBE_REALTIME_DB_UPDATE
	//}}
	{
		//{{ 2010. 7. 26  최육사	밀봉 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
		if( SiCXSLRandomItemManager()->IsSealRandomItemCube( iRandomItemID ) == true )
		{
			BOOST_TEST_FOREACH( const KItemInfo&, kItem, kPacketToDB.m_vecItemInfo )
			{
				if( SiCXSLItemManager()->IsCanSealForRandomItem( kItem.m_iItemID ) == true
#ifdef SERV_TREASURE_BOX_ITEM
				|| SiCXSLItemManager()->IsTreasureBox( iRandomItemID, kItem.m_iItemID ) == true
#endif  SERV_TREASURE_BOX_ITEM					
					)
				{
					kPacketToDB.m_setSealRandomItem.insert( kItem.m_iItemID );
				}
			}
		}
#endif SERV_SEALED_RANDOM_ITEM
		//}}

		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		if( bResurrectionStone == true )
		{
			kPacketToDB.m_iNumResurrectionStone = m_iNumResurrectionStone;
		}
		else
		{
			kPacketToDB.m_iNumResurrectionStone = 0;
		}
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}

		SendToGameDB( DBE_OPEN_RANDOM_ITEM_REQ, kPacketToDB );
	}
	//{{ 2011. 12. 19	최육사	랜덤큐브 DB실시간 적용
#ifdef SERV_OPEN_CUBE_REALTIME_DB_UPDATE
#else
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );

		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK				   = NetError::NET_OK;
		kPacketAck.m_iRessurectionCount	   = iRessurectionCount;
		kPacketAck.m_iRestoreSpirit		   = iRestoreSpirit;
		kPacketAck.m_mapInsertedItem	   = kPacketToDB.m_mapInsertedItem;
		kPacketAck.m_vecKInventorySlotInfo = kPacketToDB.m_vecUpdatedInventorySlot;

		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		DecreaseED( kPacketToDB.m_iSpendED, KUserEDManager::ER_OUT_ED_USE_CUBE_OPEN );
#else
		const int iBeforeED = m_iED;
		m_iED -= kPacketToDB.m_iSpendED;
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_CUBE_OPEN_ED_COST, iBeforeED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		kPacketAck.m_iED = m_iED;

		// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_OpenCubeSpendED, kPacketToDB.m_iSpendED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_OpenCubeSpendED, kPacketToDB.m_iSpendED );
#endif //SERV_USER_STATISTICS_RENEWAL

		// ED 사용량 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_POpenCube, kPacketToDB.m_iSpendED );

#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
	}
#endif SERV_OPEN_CUBE_REALTIME_DB_UPDATE
	//}}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_RANDOM, kPacketToDB.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}

#endif // SERV_TIME_ENCHANT_EVENT
}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT

IMPL_ON_FUNC( ELG_CHECK_TIME_RANDOM_ITEM_EVENT_ACK )
{	
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_OPEN_RANDOM_ITEM_ACK );
    
	//{{ 2009. 10. 15  최육사	거래 예외처리
	if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
	{
		START_LOG( cerr, L"거래중에는 사용할 수 없는 기능! 이미 검사했는데?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iEventItemID )
			<< END_LOG;

		KEGS_OPEN_RANDOM_ITEM_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_ITEM_14;
		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacket );
		return;
	}
	//}}

	KDBE_OPEN_RANDOM_ITEM_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();

	// 부활석 충전, 근성도 회복 큐브
	int iRessurectionCount = 0;	
	int iRestoreSpirit = 0;
	bool bIsCharmItem = false;

	// 큐브의 ItemID얻기
	const int iRandomItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	if( iRandomItemID == 0 )
	{
		START_LOG( cerr, L"존재하지 않는 아이템 입니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
		return;
	}

	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	kPacketToDB.m_iSpendED = 0;
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}

	// 랜덤 아이템 열기
	if( m_kInventory.OpenRandomItem( GetUnitClass(), 
		kPacket_.m_iItemUID, 
		kPacket_.m_iEventItemID,
		kPacketToDB.m_mapInsertedItem,
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo, 
		iRessurectionCount, 
		iRestoreSpirit, 
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		kPacketToDB.m_iSpendED,
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		bIsCharmItem ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();

		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
		return;
	}

	//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	bool bResurrectionStone = false;

	// 부활석 충전 큐브
	if( iRessurectionCount > 0 )
	{
		// 부활석 충전
		m_iNumResurrectionStone += iRessurectionCount;
		iRessurectionCount = m_iNumResurrectionStone;

		bResurrectionStone = true;
	}
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	//}}

	//{{ 2011. 07. 04    김민성    네이버 프로모션
#ifdef SERV_NAVER_PROMOTIONS

	// 여기로 오는 것은 특수 랜덤 큐브이니깐 bIsCharmItem 을 검사하지 않아도 된다.

	//{{ 2009. 6. 22  최육사	매력아이템 공지
	BOOST_MAP_CONST_FOREACH( int, int, kGetItem, kPacketToDB.m_mapInsertedItem )
	{
		// 공지 날릴만한 아이템이면 날리자!
		if( SiCXSLRandomItemManager()->IsAttrationItem( kGetItem.first ) == true )
		{
			KEGS_NOTIFY_MSG_NOT	kPacket;
			if( SiCXSLRandomItemManager()->MakeNotifyMsgAttrationItem( kGetItem.first, GetCharName(), kPacket.m_wstrMSG ) == true )
			{
				kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM; // 아이템 획득 공지
				kPacket.m_Count = 1;
				//{{ 아이템 획득 공지에 큐브 이름 출력 - 김민성
#ifdef SERV_GET_ITEM_NOTIFY
				kPacket.m_iRandomItem = iRandomItemID;
#endif SERV_GET_ITEM_NOTIFY
				//}}
				SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
			}
		}

		// 매력 아이템의 결과물에 대한 로그 기록!
		KELOG_CHARM_ITEM_LOG_NOT kCharmLog;
		kCharmLog.m_iUnitUID	  = GetCharUID();
		kCharmLog.m_iCharmItemID  = iRandomItemID;
		kCharmLog.m_iResultItemID = kGetItem.first;
		kCharmLog.m_iQuantity	  = kGetItem.second;
		SendToLogDB( ELOG_CHARM_ITEM_LOG_NOT, kCharmLog );
	}

// RandomItemData 에 추가 하였으므로 전체 공지 보내지 않아도 된다.
// 	KEGS_NOTIFY_MSG_NOT	kPacket;
// 
// 	// 스트링 조합
//	kPacket.m_wstrMSG = GetCharName();
//	kPacket.m_wstrMSG += L";";
// 	kPacket.m_Count = 1;
// 	
// 	switch( kPacket_.m_iEventItemID )
// 	{
// 	case CXSLItem::EI_SUMMER_TEN_ENCHANT_CHARM:
// 		{
// 			kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM; // 아이템 획득 해당 서버군에만 공지
// 			kPacket.m_wstrMSG += L"강화의 부적 Lv.10";
// 			SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
// 		}
// 		break;
// 	case CXSLItem::EI_SUMMER_RANDOM_NDS:
// 		{
// 			kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM_ALL_NOTIFY; // 아이템 획득 서버군 상관 없이 공지
// 			kPacket.m_wstrMSG += L"닌텐도 NDSi";
// 			SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
// 		}
// 		break;
// 	case CXSLItem::EI_SUMMER_NEXON_CASH:
// 		{
// 			kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM_ALL_NOTIFY; // 아이템 획득 서버군 상관 없이 공지
// 			kPacket.m_wstrMSG += L"넥슨캐시 10,000원 당첨권";
// 			SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
// 		}
// 		break;
// 	case CXSLItem::EI_SUMMER_NEXON_CASH_50000:
// 		{
// 			kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM_ALL_NOTIFY; // 아이템 획득 서버군 상관 없이 공지
// 			kPacket.m_wstrMSG += L"문화상품권 50,000원 당첨권";
// 			SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
// 		}
// 		break;
// 	default:
// 		{
// 			START_LOG( clog, L"이벤트에 등록된 아이템이 아닙니다!" )
// 				<< BUILD_LOG( GetCharUID() )
// 				<< BUILD_LOG( kPacket_.m_iEventItemID )
// 				<< END_LOG;
// 		}
// 	}
#endif SERV_NAVER_PROMOTIONS
	//}} 
	
	//{{ 2011. 12. 19	최육사	랜덤큐브 DB실시간 적용
#ifdef SERV_OPEN_CUBE_REALTIME_DB_UPDATE
#else
	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( kPacketToDB.m_vecItemInfo.empty() == false )
#endif SERV_OPEN_CUBE_REALTIME_DB_UPDATE
	//}}
	{
		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		if( bResurrectionStone == true )
		{
			kPacketToDB.m_iNumResurrectionStone = m_iNumResurrectionStone;
		}
		else
		{
			kPacketToDB.m_iNumResurrectionStone = 0;
		}
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}

		SendToGameDB( DBE_OPEN_RANDOM_ITEM_REQ, kPacketToDB );
	}
	//{{ 2011. 12. 19	최육사	랜덤큐브 DB실시간 적용
#ifdef SERV_OPEN_CUBE_REALTIME_DB_UPDATE
#else
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );
		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK				   = NetError::NET_OK;
		kPacketAck.m_iRessurectionCount	   = iRessurectionCount;
		kPacketAck.m_iRestoreSpirit		   = iRestoreSpirit;
		kPacketAck.m_mapInsertedItem	   = kPacketToDB.m_mapInsertedItem;
		kPacketAck.m_vecKInventorySlotInfo = kPacketToDB.m_vecUpdatedInventorySlot;

		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		DecreaseED( kPacketToDB.m_iSpendED, KUserEDManager::ER_OUT_ED_USE_CUBE_OPEN );
#else
		const int iBeforeED = m_iED;
		m_iED -= kPacketToDB.m_iSpendED;
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_CUBE_OPEN_ED_COST, iBeforeED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		kPacketAck.m_iED = m_iED;

		// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_OpenCubeSpendED, kPacketToDB.m_iSpendED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_OpenCubeSpendED, kPacketToDB.m_iSpendED );
#endif //SERV_USER_STATISTICS_RENEWAL

		// ED 사용량 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_POpenCube, kPacketToDB.m_iSpendED );
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
	}
#endif SERV_OPEN_CUBE_REALTIME_DB_UPDATE
	//}}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_RANDOM, kPacketToDB.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( DBE_OPEN_RANDOM_ITEM_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_OPEN_RANDOM_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::ERR_RANDOM_ITEM_06 )
	{
		// TODO : DB 에 아이템 삽입 실패. 추후에 로그 추가.

		kPacket.m_iOK = NetError::NET_OK;
	}

	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	kPacket.m_iRessurectionCount	= kPacket_.m_iNumResurrectionStone;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	//}}

	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( kPacket_.m_iSpendED, KUserEDManager::ER_OUT_ED_USE_CUBE_OPEN );
#else
	const int iBeforeED = m_iED;
	m_iED -= kPacket_.m_iSpendED;
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_CUBE_OPEN_ED_COST, iBeforeED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	kPacket.m_iED = m_iED;

	// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_OpenCubeSpendED, kPacket_.m_iSpendED );
#else //SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_OpenCubeSpendED, kPacket_.m_iSpendED );
#endif //SERV_USER_STATISTICS_RENEWAL

	// ED 사용량 통계
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_POpenCube, kPacket_.m_iSpendED );
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}

#ifdef SERV_EVENT_VIP_SYSTEM
	std::map< UidType, KItemInfo >::const_iterator cmit = kPacket_.m_mapItemInfo.begin();

	for ( ; cmit != kPacket_.m_mapItemInfo.end(); ++cmit  )
	{
		if ( cmit->second.m_iItemID == 83000170 )
		{
			KDBE_INSERT_TITLE_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_iTitleID = 10390;
			kPacketToDB.m_sPeriod  = (short)15;
			kPacketToDB.m_bGameServerEvent = false;
			kPacketToDB.m_bExpandPeriod = false;
			SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );
			break;
		}
	}
#endif //SERV_EVENT_VIP_SYSTEM

#ifdef SERV_CUBE_IN_ITEM_MAPPING
	std::map<int, RANDOMITEM_MAPPING_TIME>::iterator mitRITime = SiCXSLRandomItemManager()->m_mapRandomItemMappingTime.begin();
	CTime tCurr = CTime::GetCurrentTime();

	for(; mitRITime != SiCXSLRandomItemManager()->m_mapRandomItemMappingTime.end(); mitRITime++)
	{
		// 시작시간과 종료시간 안이면, 바꿔줌.
		int nIndexData = mitRITime->first;
		std::map<int, RANDOMITEM_MAPPING_DATA>::iterator mitRIData = SiCXSLRandomItemManager()->m_mapRandomItemMappingData.find(nIndexData);
		//예외처리
		if(mitRIData == SiCXSLRandomItemManager()->m_mapRandomItemMappingData.end())
			continue;

		CTime tStartTime = mitRITime->second.tStartTime;
		CTime tEndTime = mitRITime->second.tEndTime;

		// 한번 오픈 관련 초기화 해주고, 시간되서 안에 들어가는 경우에 다시 해주자!
		if(!SiCXSLRandomItemManager()->RestoreMapItemGroup(mitRIData->second))
		{
			START_LOG( cerr, L"해당 랜덤 아이템에 대한 키템 정보가 없다. 있을수 없는에러!" )
				<< BUILD_LOG( mitRIData->second.iCubeID )
				<< BUILD_LOG( mitRIData->second.iKeyItemID )
				<< END_LOG;
		}

	}
#endif SERV_CUBE_IN_ITEM_MAPPING

	SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////

	//{{ 2011. 01. 06  김민성  스킬슬롯체인지 체크(인벤토리-기간제) 기능 구현
#ifdef SERV_SKILL_SLOT_CHANGE_INVENTORY	
	std::map< UidType, KItemInfo>::const_iterator mit = kPacket_.m_mapItemInfo.begin();
	for(  ; mit != kPacket_.m_mapItemInfo.end() ; ++mit )
	{
		switch( mit->second.m_iItemID )
		{
		case CXSLItem::EI_EXPAND_SKILL_SLOT_EVENT:
			{
#ifdef	SERV_SKILL_SLOT_ITEM_BUG_FIX	// 적용날짜: 2013-07-04				
				// 무제한 기준 (2049-12-31 23:59) 으로 기간 셋팅 되어 있음
				CTime tLimitEndDate = CTime( 2049, 12, 31, 23, 59, 0 );
				CTime tSkillSlotBEndDate;
				m_kSkillTree.GetSkillSolotBEndDate( tSkillSlotBEndDate );		
				if( tLimitEndDate == tSkillSlotBEndDate )
				{
					// 스킬 슬롯 무제한 구매했었다면 갱신 할 필요 없다.
					break;
				}
#endif	// SERV_SKILL_SLOT_ITEM_BUG_FIX

				std::wstring wstrSkillSlotBEndDate;
				if( m_kInventory.GetSlotChangeBEndDate( mit->second.m_iItemID, wstrSkillSlotBEndDate ) )
				{
					m_kSkillTree.SetSkillSolotBEndDate( wstrSkillSlotBEndDate );

					KEGS_SKILL_SLOT_CHANGE_ITEM_NOT kpacket;

					kpacket.m_cSkillSlotBExpirationState = KUserSkillTree::SSBES_NOT_EXPIRED;
					kpacket.m_wstrSkillSlotBEndDate = wstrSkillSlotBEndDate;

					SendPacket( EGS_SKILL_SLOT_CHANGE_ITEM_NOT, kpacket );
					break;
				}
			}
			break;
		}
	}
#endif SERV_SKILL_SLOT_CHANGE_INVENTORY
	//}}


	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_FUNC( EGS_ITEM_MANUFACTURE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_MANUFACTURE_REQ, EGS_ITEM_MANUFACTURE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ITEM_MANUFACTURE_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_ITEM_MANUFACTURE_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ITEM_MANUFACTURE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	std::set<int> setBanBuyItemInven;
	CTime m_tStartDate;
	CTime m_tEndDate;
	std::vector<KPacketGetItemOnOff>::iterator vitrGetItemOnOff;
	std::map<int , std::vector<KPacketGetItemOnOff> > mapGetItemOnOff =  GetKGSSimLayer()->GetTimeControlItem_Info();
	std::map<int , std::vector<KPacketGetItemOnOff> >::iterator mitGetItemOnOff = mapGetItemOnOff.find( TCIT_MANUFACTURE );

	if( mitGetItemOnOff != mapGetItemOnOff.end() )
	{
		for( vitrGetItemOnOff = mitGetItemOnOff->second.begin(); vitrGetItemOnOff != mitGetItemOnOff->second.end(); ++vitrGetItemOnOff )
		{
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrStartTime , m_tStartDate);
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrEndTime , m_tEndDate);

			if( CTime::GetCurrentTime() >= m_tStartDate && CTime::GetCurrentTime() <= m_tEndDate )
			{
				setBanBuyItemInven.insert(vitrGetItemOnOff->m_iItemID);
			}
		}
	}

	if( setBanBuyItemInven.find( kPacket_.m_iManufactureID ) != setBanBuyItemInven.end() )
	{
		KEGS_ITEM_MANUFACTURE_ACK kAck;
		kAck.m_iOK	= NetError::ERR_ITEM_22;
		SendPacket( EGS_ITEM_MANUFACTURE_ACK, kAck );
		return;
	}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	KDBE_ITEM_MANUFACTURE_REQ kPacket;
	int iED;

#ifdef SERV_MANUFACTURE_FIX
	if( !m_kInventory.ItemManufacture( kPacket_.m_iManufactureID, kPacket_.m_iQuantity, iED, kPacket.m_mapInsertedItem, kPacket.m_vecUpdatedInventorySlot, kPacket.m_vecItemInfo ) )
#else
	if( !m_kInventory.ItemManufacture( kPacket_.m_iManufactureID, iED, kPacket.m_mapInsertedItem, kPacket.m_vecUpdatedInventorySlot, kPacket.m_vecItemInfo ) )
#endif
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_ITEM_MANUFACTURE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();

		SendPacket( EGS_ITEM_MANUFACTURE_ACK, kPacketAck );
		return;
	}

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iED, KUserEDManager::ER_OUT_ED_ITEM_MANUFACTURE );
#else
	const int iBeforeED = m_iED;

	m_iED -= iED;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_MANUFACTURE, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MManufacture, iED );
#else //SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );
#endif //SERV_USER_STATISTICS_RENEWAL

	// 획득 아이템 통계
	std::map< int, int >::iterator mit;
	for( mit = kPacket.m_mapInsertedItem.begin(); mit != kPacket.m_mapInsertedItem.end(); mit++ )
	{
		// 일일통계 제조 획득 아이템
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_MAKE, mit->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_MAKE, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( !kPacket.m_vecItemInfo.empty() )
	{
		kPacket.m_iUnitUID = GetCharUID();
		m_kInventory.FlushQuantityChange( kPacket.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacket.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_ITEM_MANUFACTURE_REQ, kPacket );
	}
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );
		KEGS_ITEM_MANUFACTURE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_mapInsertedItem = kPacket.m_mapInsertedItem;
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		kPacketAck.m_iED = GetED();
#else
		kPacketAck.m_iED = m_iED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		
		kPacketAck.m_vecKInventorySlotInfo = kPacket.m_vecUpdatedInventorySlot;

		SendPacket( EGS_ITEM_MANUFACTURE_ACK, kPacketAck );
	}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_MANUFACTURE, kPacket.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( DBE_ITEM_MANUFACTURE_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_MANUFACTURE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_ITEM_MANUFACTURE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::ERR_MANUFACTURE_11 )
	{
		// TODO : DB 에 아이템 삽입 실패. 추후에 로그 추가.

		kPacket.m_iOK = NetError::NET_OK;
	}

	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;

	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	// 현재 ED
	kPacket.m_iED = GetED();

	SendPacket( EGS_ITEM_MANUFACTURE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_RESOLVE_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESOLVE_ITEM_REQ, EGS_RESOLVE_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_RESOLVE_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
			
			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_RESOLVE_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_RESOLVE_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_RESOLVE_ITEM_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	int iResolveItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	
	// 대박 검사
	bool bJackpot = false;
	if( m_kTimer[TM_RESOLVE_JACKPOT].elapsed() > m_dResolveJackpotTime )
	{
		bJackpot = true;
	}

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	std::map< UidType, int >   mapResolveItem; // 분해시킬 아이템
	std::map< int, KItemInfo > mapResultItem;  // 결과템
#endif SERV_MULTI_RESOLVE
	//}}
	// 아이템 분해
	int iResolveQuantity = 0;
	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	int iAfterItem = 0;
	int iAfterItemQuantity = 0;
#endif SERV_MULTI_RESOLVE
	//}}
	bool bFashion = false;
	if( m_kInventory.ResolveItem( kPacket_.m_iItemUID, bJackpot, bFashion, iResolveQuantity, kPacketToDB.m_mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_RESOLVE_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();

		SendPacket( EGS_RESOLVE_ITEM_ACK, kPacketAck );
		return;
	}

	// 대박 후처리
	if( bJackpot )
	{
		// 대박 로그
		KELOG_RESOLVE_JACKPOT_LOG_NOT kPacketToLog;
		CTime kUsedTime = CTime::GetCurrentTime();
		kPacketToLog.m_UnitUID		  = GetCharUID();
		kPacketToLog.m_iResolveItemID = iResolveItemID;
		kPacketToLog.m_iElapsedTime	  = static_cast<int>( m_kTimer[TM_RESOLVE_JACKPOT].elapsed() );
		kPacketToLog.m_wstrRegDate	  = ( CStringW )( kUsedTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
		SendToLogDB( ELOG_RESOLVE_JACKPOT_LOG_NOT, kPacketToLog );

		// 대박 시간 초기화
		m_kTimer[TM_RESOLVE_JACKPOT].restart();

		// 대박 시간대 새로 얻기
		m_dResolveJackpotTime = SiCXSLResolveItemManager()->GetJackpotTime();
	}

	// 캐쉬아이템을 분해 했다면 로그를 남긴다.
	if( bFashion )
	{
		KELOG_CASH_ITEM_RESOLVE_LOG_NOT kPacketLog;
		kPacketLog.m_iUnitUID = GetCharUID();
		kPacketLog.m_iItemUID = kPacket_.m_iItemUID;
		kPacketLog.m_iResultQuantity = iResolveQuantity;
		//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
kPacketLog.m_iAfterItemID = iAfterItem;
		kPacketLog.m_iAfterItemQuantity = iAfterItemQuantity;
		kPacketLog.m_iLevel	= GetLevel();
#endif SERV_MULTI_RESOLVE
		//}}		
		SendToLogDB( ELOG_CASH_ITEM_RESOLVE_LOG_NOT, kPacketLog );
	}

	// 분해 획득 아이템 출력
	std::map< int, int >::iterator mit;
	for( mit = kPacketToDB.m_mapInsertedItem.begin(); mit != kPacketToDB.m_mapInsertedItem.end(); mit++ )
	{
		START_LOG( clog2, L"분해 획득 아이템." )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second );
	}

	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( !kPacketToDB.m_vecItemInfo.empty() )
	{
		kPacketToDB.m_bJackpot = bJackpot;
		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_RESOLVE_ITEM_REQ, kPacketToDB );
	}
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );
		KEGS_RESOLVE_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK				   = NetError::NET_OK;
		kPacketAck.m_bJackpot			   = bJackpot;
		kPacketAck.m_mapInsertedItem	   = kPacketToDB.m_mapInsertedItem;
		kPacketAck.m_vecKInventorySlotInfo = kPacketToDB.m_vecUpdatedInventorySlot;
		SendPacket( EGS_RESOLVE_ITEM_ACK, kPacketAck );		
	}

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	m_kUserQuestManager.Handler_OnResolveItem( GetThisPtr<KGSUser>(), iResolveItemID );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
	m_kUserTitleManager.OnResolveItem( GetThisPtr<KGSUser>() );
#endif // SERV_ADD_TITLE_CONDITION_2013_08

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_RESOLVE, kPacketToDB.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( DBE_RESOLVE_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESOLVE_ITEM_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_RESOLVE_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::ERR_RESOLVE_ITEM_05 )
	{
		// TODO : DB 에 아이템 삽입 실패. 추후에 로그 추가.

		kPacket.m_iOK = NetError::NET_OK;
	}

	kPacket.m_bJackpot = kPacket_.m_bJackpot;
	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	kPacket.m_iED = GetED();
#endif SERV_MULTI_RESOLVE
	//}}
	SendPacket( EGS_RESOLVE_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_ENCHANT_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ENCHANT_ITEM_REQ, EGS_ENCHANT_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ENCHANT_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_ENCHANT_ITEM_ACK, kAck );
			return;
		}
		//}}

		if( kPacket_.m_bDebug )
		{
			if( GetAuthLevel() < SEnum::UAL_GM )
			{
				START_LOG( cerr, L"운영자도 아닌데 감히." )
					<< BUILD_LOG( GetName() )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOGc( GetAuthLevel() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_VERIFY_12;
				SendPacket( EGS_ENCHANT_ITEM_ACK, kAck );
				return;
			}

			// 치트 로그
			CStringW cwstrCheatContents;
			cwstrCheatContents.Format( L"%d", kPacket_.m_iItemUID );
			WriteCheatLogToDB( L"/enchant", cwstrCheatContents );
		}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ENCHANT_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_ENCHANT_ITEM_ACK kPacket;
	KDBE_ENCHANT_ITEM_REQ kPacketReq;
	KELOG_ENCHANT_ABUSER_LOG_NOT kPacketToLog;

	// 강화 아이템ID 얻기 : EnchantItem() 함수에서 삭제되기 전에
	kPacketReq.m_iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	// 강화 레벨 얻기 : EnchantItem() 함수에서 삭제되기 전에
	int iEnchantLevel;
	if( m_kInventory.GetEnchantLevel( kPacket_.m_iItemUID, iEnchantLevel ) == false )
	{
		START_LOG( cerr, L"강화 레벨 얻기 실패." )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ENCHANT_ITEM_03;
		SendPacket( EGS_ENCHANT_ITEM_ACK, kPacket );
		return;
	}

	// 강화 어뷰저
	if( IsEnchantAbuser() )
	{
		kPacketToLog.m_iUserUID = GetUID();
		kPacketToLog.m_iUnitUID = GetCharUID();
		kPacketToLog.m_iItemID  = kPacketReq.m_iItemID;
		kPacketToLog.m_iItemUID = kPacket_.m_iItemUID;
		kPacketToLog.m_cEnchantLevel = iEnchantLevel + 1;
	}

	int iED;
#ifdef SERV_ENCHANT_PLUS_ITEM
	if( m_kInventory.EnchantItem( kPacket_.m_iItemUID, 
		kPacket_.m_bIsRareEnchantStone, 
		kPacket_.m_bIsNewEnchant, 
		kPacket_.m_bIsSupportMaterial, 
		kPacket_.m_bIsDestroyGuard,
		kPacket_.m_bIsEnchantPlus,
		kPacket_.m_bDebug, 
		kPacketReq.m_iEnchantResult, 
		kPacketReq.m_iLevelAfterEnchant, 
		iED, 
		kPacketReq.m_vecUpdatedInventorySlot ) == false )
#else
	if( m_kInventory.EnchantItem( kPacket_.m_iItemUID, 
		kPacket_.m_bIsRareEnchantStone, 
		kPacket_.m_bIsNewEnchant, 
		kPacket_.m_bIsSupportMaterial, 
		kPacket_.m_bDebug, 
		kPacketReq.m_iEnchantResult, 
		kPacketReq.m_iLevelAfterEnchant, 
		iED, 
		kPacketReq.m_vecUpdatedInventorySlot ) == false )
#endif//SERV_ENCHANT_PLUS_ITEM
		//}}
	{
		// 강화 어뷰저
		if( IsEnchantAbuser() )
		{
			kPacketToLog.m_iEnchantResult = SiKAbuserLogManager()->ChangeEnchantAbuserResult( kPacketReq.m_iEnchantResult );
			SendToLogDB( ELOG_ENCHANT_ABUSER_LOG_NOT, kPacketToLog );
		}

		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ENCHANT_ITEM_ACK, kPacket );
		return;
	}
	else
	{
		// 강화 어뷰저
		if( IsEnchantAbuser() )
		{
			kPacketToLog.m_iEnchantResult = SiKAbuserLogManager()->ChangeEnchantAbuserResult( kPacketReq.m_iEnchantResult );
			SendToLogDB( ELOG_ENCHANT_ABUSER_LOG_NOT, kPacketToLog );
		}
	}

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iED, KUserEDManager::ER_OUT_ED_ITEM_ENCHANT ); // 강화 수수료 차감
#else
	const int iBeforeED = m_iED;
	
	m_iED -= iED; // 강화 수수료 차감

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_ITEM_ENCHANT, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MEnchant, iED );
#else //SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MEnchantED, iED );
#endif //SERV_USER_STATISTICS_RENEWAL

	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가 - 로그 남기기
	// 파괴 방지까지 합쳐서 한번에 적용. 
#ifdef SERV_ENCHANT_PLUS_ITEM
	if( kPacket_.m_bIsSupportMaterial || kPacket_.m_bIsEnchantPlus || kPacket_.m_bIsDestroyGuard )
	{
		KELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT kPacketLog;
		kPacketLog.m_iItemUID	  = kPacket_.m_iItemUID;
		kPacketLog.m_cBeforeLevel = static_cast<char>(iEnchantLevel);
		kPacketLog.m_cAfterLevel  = static_cast<char>(kPacketReq.m_iLevelAfterEnchant);
		kPacketLog.m_bUseSupport = kPacket_.m_bIsSupportMaterial;
		kPacketLog.m_bUsePlus = kPacket_.m_bIsEnchantPlus;
		kPacketLog.m_bUseDestroyGuard = kPacket_.m_bIsDestroyGuard;
		if( kPacketReq.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
		{
			kPacketLog.m_cAfterLevel = kPacketLog.m_cBeforeLevel + 100; // 로그에 기록할때는 이전 강화레벨에 100을 더한다.
		}
		SendToLogDB( ELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT, kPacketLog );
	}
	//}}
#else
	//{{ 2008. 12. 18  최육사	강화 보조제 로그
	if( kPacket_.m_bIsSupportMaterial )
	{
		KELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT kPacketLog;
		kPacketLog.m_iItemUID	  = kPacket_.m_iItemUID;
		kPacketLog.m_cBeforeLevel = static_cast<char>(iEnchantLevel);
		kPacketLog.m_cAfterLevel  = static_cast<char>(kPacketReq.m_iLevelAfterEnchant);
		if( kPacketReq.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
		{
			kPacketLog.m_cAfterLevel = kPacketLog.m_cBeforeLevel + 100; // 로그에 기록할때는 이전 강화레벨에 100을 더한다.
		}
		SendToLogDB( ELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT, kPacketLog );
	}
	//}}
#endif
	//}}

	START_LOG( clog, L"아이템 강화." )
		<< BUILD_LOG( NetError::GetLastError() )
		<< BUILD_LOG( NetError::GetLastErrMsg() );

	//{{  2011.11.08     김민성    강화 로그
#ifdef SERV_ENCHANT_ITEM_LOG
	if( iEnchantLevel >= KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_START_LOG_LEVEL ) // 7강에서 제련 부터 기록
	{
		CTime tCurr = CTime::GetCurrentTime();

		KELOG_HIGH_ENCHANT_ITEM_LOG_NOT kHighEnchantLog;
		kHighEnchantLog.m_iUnitUID		 = GetCharUID(); 
		kHighEnchantLog.m_iItemUID		 = kPacket_.m_iItemUID;
		kHighEnchantLog.m_iItemID		 = kPacketReq.m_iItemID;
		kHighEnchantLog.m_iBeforeLevel  = iEnchantLevel;
		kHighEnchantLog.m_iAfterLevel	 = kPacketReq.m_iLevelAfterEnchant;
		kHighEnchantLog.m_bSupportMaterial	 = kPacket_.m_bIsSupportMaterial;
		kHighEnchantLog.m_wstrTime		 = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );

		switch( kPacketReq.m_iEnchantResult )
		{
		case NetError::ERR_ENCHANT_RESULT_00: // 성공
			{
				kHighEnchantLog.m_iEnchantResult = KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_SUCCESS_ENCHANT;
			}break;
		case NetError::ERR_ENCHANT_RESULT_01: // 변화 없음
			{
				kHighEnchantLog.m_iEnchantResult = KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_NO_CHANGE;
			}break;
		case NetError::ERR_ENCHANT_RESULT_02: // 1 하락
			{
				kHighEnchantLog.m_iEnchantResult = KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_DOWN_ENCHANT;
			}break;
		case NetError::ERR_ENCHANT_RESULT_03: // 초기화
			{
				kHighEnchantLog.m_iEnchantResult = KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_INIT_ITEM;
			}break;
		case NetError::ERR_ENCHANT_RESULT_04: // 파손
			{
				kHighEnchantLog.m_iEnchantResult = KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_BREAK_ITEM;
				kHighEnchantLog.m_iAfterLevel	 = iEnchantLevel + 100;

			}break;
		default:
			{
				kHighEnchantLog.m_iEnchantResult = KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_NONE;
			}
		}
				
		SendToLogDB( ELOG_HIGH_ENCHANT_ITEM_LOG_NOT, kHighEnchantLog );
	}
#endif SERV_ENCHANT_ITEM_LOG
	//}}

	if( kPacketReq.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_01 ) // 강화 변화 없음
	{
		kPacket.m_iOK = NetError::GetLastError();
		kPacket.m_iED = GetED();
		kPacket.m_iEnchantedItemUID = kPacket_.m_iItemUID;
		kPacket.m_iEnchantedItemID = kPacketReq.m_iItemID;
		kPacket.m_iEnchantResult = kPacketReq.m_iEnchantResult;
		kPacket.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
		SendPacket( EGS_ENCHANT_ITEM_ACK, kPacket );
		return;
	}

	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iItemUID = kPacket_.m_iItemUID;

	m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_ENCHANT_ITEM_REQ, kPacketReq );

	//{{ 2008. 11. 10  최육사	강화 아이템 파괴 로그
	if( kPacketReq.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_04 )
	{
		KELOG_ENCHANT_ITEM_BREAK_LOG_NOT kPacketBreakLog;
		kPacketBreakLog.m_iUnitUID = GetCharUID();
		kPacketBreakLog.m_iItemUID = kPacket_.m_iItemUID;
		kPacketBreakLog.m_iItemID  = kPacketReq.m_iItemID;
		kPacketBreakLog.m_cEnchantLevel = static_cast<char>( abs(iEnchantLevel) );
		SendToLogDB( ELOG_ENCHANT_ITEM_BREAK_LOG_NOT, kPacketBreakLog );
	}
	//}}
}

IMPL_ON_FUNC( DBE_ENCHANT_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ENCHANT_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"강화 정보 DB업데이트 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// DB업데이트 실패!
	}
	else
	{
		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		m_kUserQuestManager.Handler_OnEnchantItem( GetThisPtr<KGSUser>(), kPacket_.m_iItemID, kPacket_.m_iLevelAfterEnchant );
#endif SERV_QUEST_CLEAR_EXPAND
		//}}

#ifdef SERV_ENCHANT_ITEM_EVENT
		IF_EVENT_ENABLED(CEI_ENCHANT_ITEM_EVENT)
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kPacket_.m_iItemID );
			if( pItemTemplet != NULL )
			{
				if( kPacket_.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_00 &&
					kPacket_.m_iLevelAfterEnchant == 11 &&
					pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON &&
					pItemTemplet->m_UseLevel >= 42) 
				{
					// 이벤트 보상을 주자!
					KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
					kPacketToDB.m_iFromUnitUID = GetCharUID();
					kPacketToDB.m_iToUnitUID   = GetCharUID();
					kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
					kPacketToDB.m_iRewardID	   = 620;	// 아리엘의 강화부적 Lv.9
					SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
				}
			}
			else
			{
				START_LOG( cerr, L"존재하지 않는 아이템을 강화했다? 보상 지급 실패!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iItemID )
					<< END_LOG;
			}
		}
#endif SERV_ENCHANT_ITEM_EVENT

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kPacket_.m_iItemID );
		if( pItemTemplet != NULL )
		{
			int iItemLevel = pItemTemplet->m_iItemLevel;
			if( iItemLevel <= 0 )
			{
				iItemLevel = static_cast<int>( GetLevel() );
			}
			m_kUserTitleManager.OnEnchantItemLevel( GetThisPtr<KGSUser>(), iItemLevel, kPacket_.m_iLevelAfterEnchant, kPacket_.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_00 );
		}
#endif // SERV_ADD_TITLE_CONDITION_2013_08

		//{{ 2009. 6. 29  최육사	10강 이상 강화 성공시 전체 공지!
		if( kPacket_.m_iEnchantResult == NetError::ERR_ENCHANT_RESULT_00  &&  
			kPacket_.m_iLevelAfterEnchant >= 10  &&  
			GetAuthLevel() < SEnum::UAL_GM )
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kPacket_.m_iItemID );
			if( pItemTemplet != NULL )
			{
				KEGS_NOTIFY_MSG_NOT	kPacket;
				kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_ENCHANT_SUCCESS;
				kPacket.m_Count = 1;				

				CStringW cwstrMessage;
#ifdef SERV_UNITED_SERVER_EU
				cwstrMessage.Format( L"%s;%d;%d", GetCharName().c_str(), pItemTemplet->m_ItemID, kPacket_.m_iLevelAfterEnchant );
#else SERV_UNITED_SERVER_EU
				cwstrMessage.Format( L"%s;%s;%d", GetCharName().c_str(), pItemTemplet->m_Name.c_str(), kPacket_.m_iLevelAfterEnchant );
#endif SERV_UNITED_SERVER_EU
				kPacket.m_wstrMSG = static_cast<LPCTSTR>(cwstrMessage);

				SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
			}
			else
			{
				START_LOG( cerr, L"존재하지 않는 아이템을 강화했다? 공지 날리기 실패!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iItemID )
					<< END_LOG;
			}
		}
		//}}
	}

	KEGS_ENCHANT_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iEnchantResult = kPacket_.m_iEnchantResult;
	kPacket.m_iED = GetED();
	kPacket.m_iEnchantedItemUID = kPacket_.m_iItemUID;
	kPacket.m_iEnchantedItemID = kPacket_.m_iItemID;
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	SendPacket( EGS_ENCHANT_ITEM_ACK, kPacket );
}

//{{ 2010. 04. 15  최육사	소켓 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EGS_SOCKET_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SOCKET_ITEM_REQ, EGS_SOCKET_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_SOCKET_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_SOCKET_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2010. 06. 11  최육사	소켓 옵션 치트
		if( kPacket_.m_bCheat == true )
		{
			if( GetAuthLevel() < SEnum::UAL_GM )
			{
				START_LOG( cerr, L"운영자도 아닌데 감히" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetName() )
					<< BUILD_LOGc( GetAuthLevel() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_VERIFY_12;
				SendPacket( EGS_SOCKET_ITEM_ACK, kAck );
				return;
			}
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_SOCKET_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////


	KDBE_SOCKET_ITEM_REQ kPacketReq;
	KEGS_SOCKET_ITEM_ACK kPacket;
	int iED;

	// 소켓 아이템ID 얻기
	if( m_kInventory.SocketItem( kPacket_.m_iItemUID
							   , kPacket_.m_mapSocketInfo
							   , kPacket_.m_bCheat
							   , kPacketReq.m_iItemID
							   , iED
							   , kPacketReq.m_vecSocketInfo
							   , kPacketReq.m_vecUpdatedInventorySlot
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
							   , kPacketReq.m_byteExpandedSocketNum
#endif //SERV_BATTLE_FIELD_BOSS
							   ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SOCKET_ITEM_ACK, kPacket );
		return;
	}

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iED, KUserEDManager::ER_OUT_ED_ITEM_SOCKET ); // 소켓 수수료 차감
#else
	const int iBeforeED = m_iED;
	
	m_iED -= iED; // 소켓 수수료 차감

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_ITEM_SOCKET, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MSocket, iED );
#else //SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MSocketED, iED );
#endif //SERV_USER_STATISTICS_RENEWAL

#ifdef	SERV_SHARING_BANK_TEST
	if( m_kInventory.GetIsShareBank() == true && m_kInventory.IsShareItem( kPacket_.m_iItemUID ) == true )
	{
		kPacketReq.m_iUnitUID = m_kInventory.GetShareUnitUID();
	}
	else
	{
		kPacketReq.m_iUnitUID = GetCharUID();
	}
#else	SERV_SHARING_BANK_TEST
	kPacketReq.m_iUnitUID = GetCharUID();
#endif	SERV_SHARING_BANK_TEST
	kPacketReq.m_iItemUID = kPacket_.m_iItemUID;
	//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	kPacketReq.m_iSocketUseCount = static_cast<int>(kPacket_.m_mapSocketInfo.size());
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_SOCKET_ITEM_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_SOCKET_ITEM_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kUserQuestManager.Handler_OnSocketItem( GetThisPtr<KGSUser>(), kPacket_.m_iItemID, kPacket_.m_iSocketUseCount );

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kPacket_.m_iItemID );
		if( pItemTemplet != NULL )
		{
			int iItemLevel = pItemTemplet->m_iItemLevel;
			if( iItemLevel <= 0 )
			{
				iItemLevel = static_cast<int>( GetLevel() );
			}
			m_kUserTitleManager.OnSocketItem( GetThisPtr<KGSUser>(), iItemLevel, kPacket_.m_iSocketUseCount );
		}
#endif // SERV_ADD_TITLE_CONDITION_2013_08
	}
#endif SERV_QUEST_CLEAR_EXPAND
	//}}


	

	KEGS_SOCKET_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iED = GetED();
	kPacket.m_iItemUID = kPacket_.m_iItemUID;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	SendPacket( EGS_SOCKET_ITEM_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

//IMPL_ON_FUNC( EGS_SOCKET_ITEM_REQ )
//{
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//
//	KDBE_SOCKET_ITEM_REQ kPacketReq;
//	KEGS_SOCKET_ITEM_ACK kPacket;
//	int iED;
//
//	//{{ 2009. 10. 15  최육사	거래 예외처리
//	if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
//	{
//		START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
//			<< BUILD_LOG( GetCharUID() )
//			<< END_LOG;
//
//		kPacket.m_iOK	= NetError::ERR_ITEM_14;
//		SendPacket( EGS_SOCKET_ITEM_ACK, kPacket );
//		return;
//	}
//	//}}
//
//	if( kPacket_.m_bDebug )
//	{
//		if( GetAuthLevel() < SEnum::UAL_GM )
//		{
//			START_LOG( cerr, L"운영자도 아닌데 감히." )
//				<< BUILD_LOG( GetName() )
//				<< BUILD_LOG( GetUID() )
//				<< BUILD_LOG( GetCharUID() )
//				<< BUILD_LOGc( GetAuthLevel() )
//				<< END_LOG;
//
//			kPacket.m_iOK = NetError::ERR_VERIFY_12;
//			SendPacket( EGS_SOCKET_ITEM_ACK, kPacket );
//			return;
//		}
//
//		// 치트 로그
//		CStringW cwstrCheatContents;
//		cwstrCheatContents.Format( L"%d", kPacket_.m_iItemUID );
//		WriteCheatLogToDB( L"/socket", cwstrCheatContents );
//	}
//
//	// 소켓 아이템ID 얻기
//	if( !m_kInventory.SocketItem( kPacket_.m_iItemUID, 
//		kPacket_.m_iSocketIndex, 
//		kPacket_.m_bIsRareMagicStone, 
//		kPacket_.m_bIsRemoveSocketOption, 
//		kPacket_.m_bDebug, 
//		kPacket_.m_sDebugSocketOption, 
//		kPacketReq.m_iSocketResult, 
//		iED, 
//		kPacketReq.m_vecSocketInfo, 
//		kPacketReq.m_vecUpdatedInventorySlot ) )
//	{
//		kPacket.m_iOK = NetError::GetLastError();
//		SendPacket( EGS_SOCKET_ITEM_ACK, kPacket );
//		return;
//	}
//
//	int iBeforeED = m_iED;
//
//	// 소켓 수수료 차감
//	m_iED -= iED;
//
//	// ED 어뷰저
//	if( m_bIsEDAbuserLog )
//	{
//		KELOG_ED_ABUSER_LOG_NOT kPacketEDLog;
//		kPacketEDLog.m_iUnitUID  = GetCharUID();
//		kPacketEDLog.m_iState	 = KAbuserLogManager::ELS_ITEM_SOCKET;
//		kPacketEDLog.m_iBeforeED = iBeforeED;
//		kPacketEDLog.m_iAfterED	 = m_iED;
//		kPacketEDLog.m_iED		 = iED;
//		SendToLogDB( ELOG_ED_ABUSER_LOG_NOT, kPacketEDLog );
//	}
//
//	// 유저 통계
//	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MSocketED, iED );
//
//	kPacketReq.m_iUnitUID = GetCharUID();
//	kPacketReq.m_iItemUID = kPacket_.m_iItemUID;
//	kPacketReq.m_iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
//	m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );	
//	m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
//	SendToGameDB( DBE_SOCKET_ITEM_REQ, kPacketReq );
//}
//
//IMPL_ON_FUNC( DBE_SOCKET_ITEM_ACK )
//{
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//
//	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
//#ifdef SERV_QUEST_CLEAR_EXPAND
//	m_kUserQuestManager.Handler_OnSocketItem( GetThisPtr<KGSUser>(), kPacket_.m_iItemID );
//#endif SERV_QUEST_CLEAR_EXPAND
//	//}}
//
//	KEGS_SOCKET_ITEM_ACK kPacket;
//	kPacket.m_iOK = NetError::NET_OK;
//	kPacket.m_iSocketResult  = kPacket_.m_iSocketResult;
//	kPacket.m_iED = GetED();
//	kPacket.m_iSocketItemUID = kPacket_.m_iItemUID;
//	kPacket.m_iSocketItemID  = kPacket_.m_iItemID;
//	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
//	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
//	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
//
//	SendPacket( EGS_SOCKET_ITEM_ACK, kPacket );
//}

//////////////////////////////////////////////////////////////////////////
#endif SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////
//}}

//{{ 2008. 9. 3  최육사		속성강화
IMPL_ON_FUNC( EGS_ATTRIB_ENCHANT_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ATTRIB_ENCHANT_ITEM_REQ, EGS_ATTRIB_ENCHANT_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ATTRIB_ENCHANT_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_ATTRIB_ENCHANT_ITEM_REQ kPacketToDB;
	KEGS_ATTRIB_ENCHANT_ITEM_ACK kPacket;

	const int iBeforeED = GetED();

	if( m_kInventory.AttribEnchantItem( kPacket_.m_iItemUID, kPacket_.m_cAttribEnchantSlotNo, kPacket_.m_cAttribEnchantID, kPacketToDB.m_vecUpdatedInventorySlot ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK, kPacket );
		return;
	}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
#else
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_ATTRIB_ENCHANT, iBeforeED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	
//#endif SERV_USER_ABUSER_MANAGER
	//}}
#ifdef SERV_USER_STATISTICS_RENEWAL
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MAttribEnchant, iBeforeED - GetED() );
#endif //SERV_USER_STATISTICS_RENEWAL	

	// DB업데이트
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iItemUID = kPacket_.m_iItemUID;
	kPacketToDB.m_iItemID  = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	kPacketToDB.m_cAttribEnchantSlotNo = kPacket_.m_cAttribEnchantSlotNo;
	kPacketToDB.m_cAttribEnchantID = kPacket_.m_cAttribEnchantID;
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_ATTRIB_ENCHANT_ITEM_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ATTRIB_ENCHANT_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ATTRIB_ENCHANT_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KInventoryItemInfo kItemInfo;
		if( m_kInventory.GetInventoryItemInfo( kPacket_.m_iItemUID, kItemInfo ) == true )
		{
			m_kUserQuestManager.Handler_OnAttribEnchantItem( GetThisPtr<KGSUser>(), kPacket_.m_iItemID, kItemInfo.m_kItemInfo.m_kAttribEnchantInfo );
		}
	}	
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	KEGS_ATTRIB_ENCHANT_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iED = GetED();
	kPacket.m_iEnchantedItemID = kPacket_.m_iItemID;
	kPacket.m_iEnchantedItemUID = kPacket_.m_iItemUID;
	kPacket.m_cAttribEnchantSlotNo = kPacket_.m_cAttribEnchantSlotNo;
	kPacket.m_cAttribEnchantID = kPacket_.m_cAttribEnchantID;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	SendPacket( EGS_ATTRIB_ENCHANT_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_IDENTIFY_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_IDENTIFY_ITEM_REQ, EGS_IDENTIFY_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_IDENTIFY_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_IDENTIFY_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_IDENTIFY_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_IDENTIFY_ITEM_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();

	// 아이템 감정 하기
	if( !m_kInventory.IdentifyItem( kPacket_.m_iItemUID, kPacket_.m_iQuantity, kPacketToDB.m_mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo ) )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_IDENTIFY_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();

		SendPacket( EGS_IDENTIFY_ITEM_ACK, kPacketAck );
		return;
	}

	// 획득 아이템 출력
	std::map< int, int >::iterator mit;
	for( mit = kPacketToDB.m_mapInsertedItem.begin(); mit != kPacketToDB.m_mapInsertedItem.end(); mit++ )
	{
		START_LOG( clog2, L"랜덤 획득 아이템." )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second );
	}

	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( !kPacketToDB.m_vecItemInfo.empty() )
	{
		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_IDENTIFY_ITEM_REQ, kPacketToDB );
	}
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );
		KEGS_IDENTIFY_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK				   = NetError::NET_OK;
		kPacketAck.m_mapInsertedItem	   = kPacketToDB.m_mapInsertedItem;
		kPacketAck.m_vecKInventorySlotInfo = kPacketToDB.m_vecUpdatedInventorySlot;
		SendPacket( EGS_IDENTIFY_ITEM_ACK, kPacketAck );
	}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_IDENTIFY, kPacketToDB.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( DBE_IDENTIFY_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_IDENTIFY_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_IDENTIFY_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::ERR_IDENTIFY_04 )
	{
		// TODO : DB 에 아이템 삽입 실패. 추후에 로그 추가.

		kPacket.m_iOK = NetError::NET_OK;
	}

	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	SendPacket( EGS_IDENTIFY_ITEM_ACK, kPacket );
}
//}}

//{{ 2008. 10. 9  최육사	아이템사용
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
IMPL_ON_FUNC( EGS_USE_ITEM_IN_INVENTORY_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_ITEM_IN_INVENTORY_REQ, EGS_USE_ITEM_IN_INVENTORY_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_USE_ITEM_IN_INVENTORY_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kAck );
			return;
		}
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 아이템 사용 전 처리되는 부분

	bool bNotDeleteItem = false;
	int iWarpPointMapID = 0;
	const int iUsedItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	switch( iUsedItemID )
	{
		//{{ 2009. 10. 9  최육사	길드
	case CXSLItem::SI_GUILD_MAX_MEMBER_UPGRADE:
#ifdef EVENT_GUILD_ITEM
	case CXSLItem::SI_EVENT_GUILD_MAX_MEMBER_UPGRADE:
#endif //EVENT_GUILD_ITEM
		{
			KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ kPacketToLg;
			kPacketToLg.m_iUnitUID = GetCharUID();
			kPacketToLg.m_iGuildUID = GetGuildUID();
			kPacketToLg.m_iItemUID = kPacket_.m_iItemUID;
			SendToLoginServer( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ, kPacketToLg );
			return;
		}
		break;
		//}}

		//{{ hoons.2009-10-19.
		//자동결재 마을 이동아이템일 경우 이동할수 있는 지역인지 체크..(흠~ 인벤사용함수가 범용적이지 않게 사용중이라 이곳에서 직접 마을아이디만 체크)
	case CXSLItem::SI_USE_FREE_BY_FIELD:
		//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
	case CXSLItem::SI_USE_COBO_EXPRESS_TICKET:
		//}}
#ifdef SERV_VIP_SYSTEM
	case CXSLItem::SI_USE_FREE_BY_FIELD_VIP:
#endif  //SERV_VIP_SYSTEM	
		{
			// 아이템 삭제하지 않도록 처리
			bNotDeleteItem = true;

			//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
			int iTempLevel = 0;
			int iTempDungeonID = 0;
			if( CheckEnterTheVillage( static_cast<int>( kPacket_.m_iTempCode ), iTempLevel, iTempDungeonID ) == false )
				//}}
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_05;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
			else
			{
				//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
				if(	( GetMapID() == kPacket_.m_iTempCode )
					|| ( SiCXSLMapData()->CheckCOBOExpressTicketMapID( GetMapID() ) == 0 )
					|| ( SiCXSLMapData()->CheckCOBOExpressTicketMapID( static_cast<int>( kPacket_.m_iTempCode ) ) == 0 )
				 )
				{
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_UNKNOWN;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
				else if( ( iUsedItemID == CXSLItem::SI_USE_COBO_EXPRESS_TICKET )
					&& ( GetED() < SiCXSLMapData()->ComputeCOBOExpressTicketCost( GetMapID(), static_cast<int>( kPacket_.m_iTempCode ), GetLevel() ) )
					)
				{
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_08;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
				//}}

				iWarpPointMapID = static_cast<int>( kPacket_.m_iTempCode );
			}
		}
		break;
		//}} hoons.2009-10-19.

		// 마을 이동 아이템 사용 조건 검사
	case CXSLItem::SI_WARP_RUBEN:
	case CXSLItem::SI_WARP_ELDER:
	case CXSLItem::SI_WARP_BESMA:
	case CXSLItem::SI_WARP_ALTERA:
	case CXSLItem::SI_WARP_PEITA:
	case CXSLItem::SI_WARP_VELDER:
		{
			// 포탈아이템의 경우 이동할 mapid얻기
			switch( iUsedItemID )
			{
			case CXSLItem::SI_WARP_RUBEN:  iWarpPointMapID = SEnum::VMI_RUBEN; break;
			case CXSLItem::SI_WARP_ELDER:  iWarpPointMapID = SEnum::VMI_ELDER; break;
			case CXSLItem::SI_WARP_BESMA:  iWarpPointMapID = SEnum::VMI_BESMA; break;
			case CXSLItem::SI_WARP_ALTERA: iWarpPointMapID = SEnum::VMI_ALTERA; break;
			case CXSLItem::SI_WARP_PEITA:  iWarpPointMapID = SEnum::VMI_PEITA; break;
			case CXSLItem::SI_WARP_VELDER: iWarpPointMapID = SEnum::VMI_VELDER; break;
			default:					   iWarpPointMapID = SEnum::VMI_INVALID; break;
			}

			// 해당 map에 들어갈 수 있는지 조건 체크
			//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
			int iTempLevel = 0;
			int iTempDungeonID = 0;
			if( CheckEnterTheVillage( iWarpPointMapID, iTempLevel, iTempDungeonID ) == false )
				//}}
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::GetLastError();
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;

		//{{ 2009. 11. 27  최육사	길드스킬
	case CXSLItem::SI_GUILD_CASH_SKILL_POINT:
#ifdef EVENT_GUILD_ITEM
	case CXSLItem::SI_EVENT_GUILD_CASH_SKILL_POINT:
#endif //EVENT_GUILD_ITEM
#ifdef SERV_EVENT_GUILD_SKILL_GLOBAL
	case CXSLItem::SI_EVENT_GUILD_SKILL_5_POINT_15_DAY:
#endif //SERV_EVENT_GUILD_SKILL_GLOBAL
		{
			// 길드 가입 유저인지 검사
			if( GetGuildUID() <= 0 )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUILD_19;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}

			// 길드 마스터만 그노시스의 대축복 아이템을 사용할 수 있음.
			if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUILD_SKILL_15;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}

			// 이미 그노시스 대축복 아이템을 사용중이라면 다시 사용할 수 없다.
			if( m_kUserGuildManager.IsCashSkillPointExpired() == false )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUILD_SKILL_17;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
		//}}

		//{{ 2010. 04. 09  최육사	기술의 노트
	case CXSLItem::SI_SKILL_NOTE_ITEM:
		{
			if( m_kSkillTree.IsHaveSkillNote() == true )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SKILL_NOTE_06;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
		//}}

		//{{ 2011. 05. 11	최육사	대리상인
	case CXSLItem::SI_PSHOP_AGENCY_1_DAY:
	case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_15_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:
		{
			// 체험ID 기능 제한
			if( IsGuestUser() )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
		//}}
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS_ED: // 캐시 대리상점이 열려있다면 ED , ED 프리미엄 아이템 사용 불가
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS_ED:
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS_ED:
        {
            if (
#ifdef SERV_UPGRADE_TRADE_SYSTEM
			 m_kUserPersonalShopManager.GetPShopType() == SEnum::AST_PREMIUM &&
#endif //SERV_UPGRADE_TRADE_SYSTEM
                m_kUserPersonalShopManager.IsOpenPossiblePShopAgency() )
            {
                KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
                kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_57; // 실패 사유
                SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
                return;
            }
        }
        
        break;

		//{{ 2011. 08. 26	최육사	캐릭터 레벨업 아이템
	case CXSLItem::EI_CHAR_LEVEL_UP_ITEM:
#ifdef SERV_CHAR_LEVEL_UP_ITEM_EVENT_RENA
	case CXSLItem::EL_CHAR_LEVEL_UP_ITEM2:
#endif SERV_CHAR_LEVEL_UP_ITEM_EVENT_RENA
		{
			// 만렙이면 사용할 수 없다!
			if( SiKGameSysVal()->GetLimitsLevel() == GetLevel() )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
#ifdef SERV_LIMIT_LEVEL_NOT_USE_ITEM_MSG
				kPacket.m_iOK = NetError::ERR_NOT_USE_01;
#else //SERV_LIMIT_LEVEL_NOT_USE_ITEM_MSG
				kPacket.m_iOK = NetError::ERR_UNKNOWN;
#endif //SERV_LIMIT_LEVEL_NOT_USE_ITEM_MSG
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
		//}}

		//{{  그노시스 축복
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN:
	case CXSLItem::CI_SKILL_POINT_30_USE_INVEN:
	case CXSLItem::CI_SKILL_POINT_60_USE_INVEN:
	case CXSLItem::CI_SKILL_POINT_60_USE_INVEN_ARA:
	case CXSLItem::EI_SKILL_POINT_30_DAY_7_USE_INVEN:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_30_POINT_15_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_30_POINT_30_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_30_POINT_60_DAY:
	case CXSLItem::CI_CASH_SKILL_POINT_30_15:
	case CXSLItem::CI_CASH_SKILL_POINT_60_15:
	case CXSLItem::CI_CASH_SKILL_POINT_30_30:
	case CXSLItem::CI_CASH_SKILL_POINT_60_30:
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_CASH_SKILL_POINT_TW
	case CXSLItem::CI_CASH_SKILL_POINT_30_7:
	case CXSLItem::CI_CASH_SKILL_POINT_60_7:
#endif SERV_CASH_SKILL_POINT_TW
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN_2:
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_GNOSIS_BR
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
	case CXSLItem::EI_SKILL_POINT_30_14DAY_USE_INVEN:
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
	case CXSLItem::EI_LURIEL_GNOSIS_30_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_7DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
		{
			if( m_kSkillTree.IsCashSkillPointExpired() == false )
			{
				if( SiCXSLItemManager()->GetItemCSPoint( iUsedItemID ) != m_kSkillTree.GetMaxCSPoint() )
				{
					START_LOG( cwarn, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;

					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
			}
		}
		break;
		
		//{{ 2012. 04. 29	박세훈	어둠의 문 개편
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_NUT:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_PTAH:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_RACHEL:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_ZADKIEL:
		if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_STRENGTHENING_BODY ) == true )
		{
			KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_07;
			SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
			return;
		}

		if( kPacket_.m_iTempCode != 0 )
		{
			if( ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_NUT )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_PTAH )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_LAHEL )	== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_ZACHIEL )	== true )
				)
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_07;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_SERAPHIM:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_GEB:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_AMON:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_CRONOS:
		//{{ 2013. 04. 12	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_GIANT_POTION:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_BLAZING_BOMB:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_SPIRIT_OF_CHASER:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_CRADLE_OF_LITTLE_FAIRY:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_ICE_BALL_OF_DENIPH:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_FEATHER_OF_VENTUS:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_FLAME_RING_OF_ROSSO:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_BIG_HAND_POTION:
#endif SERV_NEW_DEFENCE_DUNGEON
#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
	case CXSLItem::EI_BUFF_RURIEL_MANA_ENERGIZE_POTION:
#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
		//}}
		if( kPacket_.m_iTempCode != 0 )
		{
			if( ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_SERAPHIM )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_GEB )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_AMON )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_CRONOS )				== true )
				//{{ 2013. 04. 12	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_GIANT_POTION )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_BLAZING_BOMB )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_SPIRIT_OF_CHASER )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_CRADLE_OF_LITTLE_FAIRY )	== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_ICE_BALL_OF_DENIPH )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_FEATHER_OF_VENTUS )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_FLAME_RING_OF_ROSSO )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_BIG_HAND_POTION )			== true )
#endif SERV_NEW_DEFENCE_DUNGEON
				//}}
#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_THE_ESSENCE_OF_WEAK_HERETIC_POTION )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_THE_ESSENCE_OF_HERETIC_POTION )			== true )
#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
#ifdef SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_RURIEL_MANA_ENERGIZE_POTION )			== true )
#endif SERV_EVENT_RURIEL_MANA_ENERGIZE_POTION

				)
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_06;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
		//}}
#ifdef SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC
	case CXSLItem::EI_THE_ESSENCE_OF_WEAK_HERETIC_POTION:
	case CXSLItem::EI_THE_ESSENCE_OF_HERETIC_POTION:
		if( kPacket_.m_iTempCode != 0 )
		{
			if( ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_THE_ESSENCE_OF_WEAK_HERETIC_POTION )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_THE_ESSENCE_OF_HERETIC_POTION )				== true ))
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_ITEM_INT_01;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
			else if( ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_SERAPHIM )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_GEB )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_AMON )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_CRONOS )				== true )
				//{{ 2013. 04. 12	박세훈	어둠의 문 개편
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_GIANT_POTION )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_BLAZING_BOMB )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_SPIRIT_OF_CHASER )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_CRADLE_OF_LITTLE_FAIRY )	== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_ICE_BALL_OF_DENIPH )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_FEATHER_OF_VENTUS )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_FLAME_RING_OF_ROSSO )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_BIG_HAND_POTION )			== true )
				//}}
				)
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_06;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_EVENT_ARA_NEW_CHAR_THE_ESSENCE_OF_HERETIC

		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
	case CXSLItem::SI_PET_AUTO_LOOTING_ITEM:
#ifdef SERV_PET_AUTO_LOOTING_ITEM_CN
	case CXSLItem::SI_PET_AUTO_LOOTING_ITEM_CN:
#endif // SERV_PET_AUTO_LOOTING_ITEM_CN
		{
			int iOK = CanIUseTheAutoLootingItem( kPacket_.m_iTempCode );
			if( iOK != NetError::NET_OK )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = iOK;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
		//}}
#ifdef SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
	case CXSLItem::SI_CHARACTER_SLOT_EXPAND_ITEM1:
	case CXSLItem::SI_CHARACTER_SLOT_EXPAND_ITEM2:
	case CXSLItem::SI_CHARACTER_SLOT_EXPAND_ITEM3:
		{
			KDBE_EXPAND_CHAR_SLOT_REQ kPacketToDB;
			kPacketToDB.m_iExpandSlotSize = 1;
			kPacketToDB.m_iCharSlotMax = CXSLUnit::CHAR_SLOT_MAX;

			SendToGameDB( DBE_EXPAND_CHAR_SLOT_REQ, kPacketToDB );
		}
		break;
#endif SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
#ifdef SERV_TOUR_TICKET_EVENT
	case CXSLItem::SI_TOUR_TICKET_ITEM:
		{
			KDBE_REGIST_TOUR_TICKET_NOT kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			SendToLogDB( DBE_REGIST_TOUR_TICKET_NOT, kPacketToDB );
		}
		break;
#endif SERV_TOUR_TICKET_EVENT
#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	case CXSLItem::SI_HALLOWEEN_TRANSFORM_POSION_ITEM:
		{
			int iOK = CanIUseTransformItem( kPacket_.m_iTempCode );
			if( iOK != NetError::NET_OK )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = iOK;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_EQUIP_EVENT:
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_EQUIP, 1 ) )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				kPacketAck.m_iUsedItemID = iUsedItemID;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacketAck );
				return;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_ACCESSORY_EVENT:
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_ACCESSORY, 1 ) )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				kPacketAck.m_iUsedItemID = iUsedItemID;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacketAck );
				return;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_QUICK_SLOT_EVENT:
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUICK_SLOT, 1 ) )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				kPacketAck.m_iUsedItemID = iUsedItemID;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacketAck );
				return;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_MATERIAL_EVENT:
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_MATERIAL, 1 ) )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				kPacketAck.m_iUsedItemID = iUsedItemID;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacketAck );
				return;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_QUEST_EVENT:
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_QUEST, 1 ) )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				kPacketAck.m_iUsedItemID = iUsedItemID;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacketAck );
				return;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_SPECIAL_EVENT:
		{
			if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::ST_SPECIAL, 1 ) )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_BUY_CASH_ITEM_19;
				kPacketAck.m_iUsedItemID = iUsedItemID;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacketAck );
				return;
			}// 슬롯 확장 체크 하는 부분. 이부분을 각 탭 별로 추가 해야 한다.
		}
		break;
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

	default:
		{
			//{{ 2009. 8. 4  최육사		캐쉬 스킬 아이템
			if( SiCXSLSkillTree()->IsUnsealItemID( iUsedItemID ) == true )
			{
				std::vector<int> vecSkillID;
				if( SiCXSLSkillTree()->GetUnsealItemInfo( iUsedItemID, GetUnitClass(), vecSkillID ) == false )
				{
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_00;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}

				if( isCanUseUnsealCashSkillItem( iUsedItemID ) == false )
				{
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_00;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
				if( isCanUseUnsealFinalitySkill( iUsedItemID, GetLevel() ) == false )
				{
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_00;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
#endif // SERV_FINALITY_SKILL_SYSTEM
			}

			//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
			CXSLTitleManager::TitleSimpleInfo kTitleInfo;
			if( SiCXSLTitleManager()->GetUseItemTitleInfoByItemID( iUsedItemID, kTitleInfo ) == true )
			{
				// 기간 무제한 칭호인 경우
				if( kTitleInfo.m_sPeriod == 0 )
				{
					// 칭호를 보유하고 있다면 아이템 사용을 할수 없다!
					if( m_kUserTitleManager.IsExistTitle( kTitleInfo.m_iTitleID ) == true )
					{
						KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_TITLE_05;
						SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
						return;
					}
				}
			}
			//}}
		}
		break;
	}

	//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
	OnUseItemInInventory( kPacket_.m_iItemUID
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
		, kPacket_.m_iTempCode
		//}}
		, bNotDeleteItem
		, iWarpPointMapID
		);
	//}}
}

//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
IMPL_ON_FUNC( DBE_USE_ITEM_IN_INVENTORY_ACK )
{	
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_ITEM_IN_INVENTORY_ACK );

	KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iUsedItemID = kPacket_.m_iUsedItemID;
	kPacket.m_iWarpPointMapID = kPacket_.m_iWarpPointMapID;
	//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
	kPacket.m_iED	= kPacket_.m_iED;
	//}}
	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
	kPacket.m_iTempCode	= kPacket_.m_iTempCode;
	//}}
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 아이템 사용 성공 후 처리되는 부분

#ifdef SERV_SUB_QUEST_USE_ITEM
	m_kUserQuestManager.CountUseItem( GetThisPtr<KGSUser>() ,0, 0, kPacket_.m_iUsedItemID );
#endif SERV_SUB_QUEST_USE_ITEM

	// title mission
	m_kUserTitleManager.OnUseItem( kPacket.m_iUsedItemID, GetThisPtr<KGSUser>() );

	// use item result
	switch( kPacket.m_iUsedItemID )
	{
	case CXSLItem::CI_CHANGE_NICKNAME:
		{
			// DB로 닉네임을 __DELETED__로 바꾸는 이벤트를 날리자!
			KDBE_DELETE_NICK_NAME_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_wstrNickName = GetCharName();
			SendToGameDB( DBE_DELETE_NICK_NAME_REQ, kPacketToDB );
			return;
		}
		break;

	case CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE:
		{
			if( CXSLInventory::GetNextBankMemberShip( m_kInventory.GetBankMemberShip() ) != CXSLInventory::MPB_NONE )
			{
				// 은행 확장하러 가자!
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
			}

			return;
		}
		break;

		//{{ 2009. 11. 27  최육사	길드스킬
	case CXSLItem::SI_GUILD_CASH_SKILL_POINT:
		{
			// 길드매니저로 가서 길드 캐쉬 스킬포인트를 얻자!
			KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT kPacketToLg;
			kPacketToLg.m_iGuildUID = GetGuildUID();
			kPacketToLg.m_iUnitUID = GetCharUID();
			kPacketToLg.m_iGuildCSPoint = 5; // 5포인트 준다!
			kPacketToLg.m_iPeriod = 15; // 15일
			SendToLoginServer( ELG_INSERT_GUILD_CASH_SKILL_POINT_NOT, kPacketToLg );
			return;
		}
		break;
		//}}

		//{{ 2010. 04. 09  최육사	기술의 노트
	case CXSLItem::SI_SKILL_NOTE_ITEM:
		{
			char cSkillNoteMaxPageNum = 0;
			if( m_kSkillTree.GetExpandSkillNotePage( GetLevel(), cSkillNoteMaxPageNum ) == true )
			{
				// 일단 기술의 노트 페이지 수를 저장하자!
				m_kSkillTree.UpdateSkillNoteMaxPageNum( cSkillNoteMaxPageNum );

				// 기술의 노트를 획득 하러 가자!
				KDBE_EXPAND_SKILL_NOTE_PAGE_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID = GetCharUID();
				kPacketToDB.m_cExpandedMaxPageNum = cSkillNoteMaxPageNum;
				kPacketToDB.m_cRollBackMaxPageNum = 0;
				SendToGameDB( DBE_EXPAND_SKILL_NOTE_PAGE_REQ, kPacketToDB );
			}
			else
			{
				START_LOG( cerr, L"기술의 노트 페이지 정보 얻기 실패! 절대 일어날 수 없는 에러!" )
					<< BUILD_LOGc( GetLevel() )
					<< END_LOG;
			}
		}
		break;
		//}}

		//{{ 2011. 05. 11	최육사	대리상인
	case CXSLItem::SI_PSHOP_AGENCY_1_DAY:
	case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_15_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:
        {
            //if (  m_kUserPersonalShopManager.GetPShopType() == SEnum::AST_PREMIUM ) // 캐시 보증서의 연장(기존과 동일)
            {
			    KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
			    kPacketToDB.m_iUnitUID = GetCharUID();
			    kPacketToDB.m_sAgencyPeriod = CXSLItem::GetPShopAgencyDays( kPacket.m_iUsedItemID );
			    kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
                kPacketToDB.m_cShopType = CXSLItem::GetPShopAgencyType( kPacket.m_iUsedItemID );
			    SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
            }
//            else
//            { // ED 보증서 사용중이라면 등록된 아이템이 없어야 한다.
//                //KEGS_JOIN_MY_PSHOP_AGENCY_REQ kPacketToLogin;
//                //kPacketToLogin.m_cJoinType = KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_CHECK_ENABLE_UPGRADE_PSHOP;
//                //_SendToLoginRoom( GetPShopAgencyUID(), EGS_JOIN_MY_PSHOP_AGENCY_REQ, kPacketToLogin );
//                
//                KERM_JOIN_PERSONAL_SHOP_REQ kPacketToRoom;
//                kPacketToRoom.m_iUserUID = GetUID();
//                //{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
//#ifdef SERV_INTEGRATION
//                kPacketToRoom.m_iUnitUID = GetCharUID();
//#endif SERV_INTEGRATION
//                //}}
//                kPacketToRoom.m_wstrNickName = GetCharName();
//                kPacketToRoom.m_bIsSquare = false;
//                kPacketToRoom.m_cJoinType = KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_CHECK_ENABLE_UPGRADE_PSHOP;
//                kPacketToRoom.m_iUsedItemID = kPacket.m_iUsedItemID;
//                _SendToLoginRoom( GetPShopAgencyUID(), ERM_CHECK_ENABLE_UPGRADE_PSHOP_AGENCY_REQ, kPacketToRoom );
//            }
		}
		break;
		//}}
    case CXSLItem::SI_PSHOP_AGENCY_7_DAYS_ED:
    case CXSLItem::SI_PSHOP_AGENCY_15_DAYS_ED:
    case CXSLItem::SI_PSHOP_AGENCY_30_DAYS_ED:
        {
            // 현재 무료, ED 보증서 사용 상태이면 ok
            if ( 
#ifdef SERV_UPGRADE_TRADE_SYSTEM
				m_kUserPersonalShopManager.GetPShopType() <= SEnum::AST_NORMAL ||
#endif //SERV_UPGRADE_TRADE_SYSTEM
                m_kUserPersonalShopManager.IsOpenPossiblePShopAgency() == false ) // 캐시보증서가 진행중일 때는 불가. 그 외에는 ok
            {
                KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
                kPacketToDB.m_iUnitUID = GetCharUID();
                kPacketToDB.m_sAgencyPeriod = CXSLItem::GetPShopAgencyDays( kPacket.m_iUsedItemID );
                kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
                kPacketToDB.m_cShopType = CXSLItem::GetPShopAgencyType( kPacket.m_iUsedItemID );
                SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
            }
        }
        break;

		//{{ 2011. 08. 26	최육사	캐릭터 레벨업 아이템
	case CXSLItem::EI_CHAR_LEVEL_UP_ITEM:
		{
			// 1레벨업하기 위해서 필요한 경험치 구하기
			const int iNextLevel = GetLevel() + 1;
			const int iTotalExpByNextLevel = SiKExpTable()->GetRequireTotalExpbyLevel( iNextLevel );

			// 1레벨업 할수 있을 정도의 경험치 더함.
			m_kEXP.AddExp( iTotalExpByNextLevel - GetEXP() );

			// 캐릭터 레벨업 체크
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT// 작업날짜: 2013-07-14	// 박세훈
			CheckCharLevelUp( false );
#else
			CheckCharLevelUp();
#endif // SERV_DUNGEON_CLEAR_PAYMENT_ITEM_EVENT
		}
		break;
		//}}

		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN:
	case CXSLItem::CI_SKILL_POINT_30_USE_INVEN:
	case CXSLItem::CI_SKILL_POINT_60_USE_INVEN:
	case CXSLItem::CI_SKILL_POINT_60_USE_INVEN_ARA:
	case CXSLItem::EI_SKILL_POINT_30_DAY_7_USE_INVEN:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_30_POINT_15_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_30_POINT_30_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_30_POINT_60_DAY:
	case CXSLItem::CI_CASH_SKILL_POINT_30_15:
	case CXSLItem::CI_CASH_SKILL_POINT_60_15:
	case CXSLItem::CI_CASH_SKILL_POINT_30_30:
	case CXSLItem::CI_CASH_SKILL_POINT_60_30:
#ifdef SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_130_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_130_1DAY_USE_INVEN
#ifdef SERV_CASH_SKILL_POINT_TW
	case CXSLItem::CI_CASH_SKILL_POINT_30_7:
	case CXSLItem::CI_CASH_SKILL_POINT_60_7:
#endif SERV_CASH_SKILL_POINT_TW
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_15DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_30DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_60_7DAY_USE_INVEN_2:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_TWHK
#ifdef SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
	case CXSLItem::EI_SKILL_POINT_60_1DAY_USE_INVEN:
	case CXSLItem::EI_SKILL_POINT_30_1DAY_USE_INVEN:
#endif SERV_EVENT_SKILL_POINT_1DAY_USE_INVEN
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_JP	
	case CXSLItem::EI_SKILL_POINT_30_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_30DAY_USE_INVEN_JP:
	case CXSLItem::EI_SKILL_POINT_10_15DAY_USE_INVEN_JP:
#endif //SERV_EVENT_CASH_SKILL_POINT_ITEM_JP
#ifdef SERV_GNOSIS_BR
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_7_DAY:
	case CXSLItem::CI_EVENT_SKILL_POINT_5_USE_INVEN_15_DAY:
#endif SERV_GNOSIS_BR
#ifdef SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
	case CXSLItem::EI_SKILL_POINT_30_14DAY_USE_INVEN:
#endif SERV_EVENT_GNOSIS_HAPP_NEW_YEAR
#ifdef SERV_LURIEL_GNOSIS
	case CXSLItem::EI_LURIEL_GNOSIS_30_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_15DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_30DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_60DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_30_7DAY:
	case CXSLItem::EI_LURIEL_GNOSIS_60_7DAY:
#endif //SERV_LURIEL_GNOSIS
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
	case CXSLItem::EI_SKILL_POINT_30_7DAY_USE_INVEN_INT:
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM_INT
		{
			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID	= GetCharUID();
				kPacketToDB.m_iCSPoint	= SiCXSLItemManager()->GetItemCSPoint( kPacket.m_iUsedItemID );
				kPacketToDB.m_iPeriod	= SiCXSLItemManager()->GetItemCSPointPeriod( kPacket.m_iUsedItemID );
				kPacketToDB.m_bUpdateEndDateOnly = false;
				kPacketToDB.m_iSkillPointItemID = kPacket.m_iUsedItemID;
				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacketToDB );
			}
			else
			{
				if( SiCXSLItemManager()->GetItemCSPoint( kPacket.m_iUsedItemID ) == m_kSkillTree.GetMaxCSPoint() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = GetCharUID();
					kPacketToDB.m_iCSPoint = SiCXSLItemManager()->GetItemCSPoint( kPacket.m_iUsedItemID );
					kPacketToDB.m_iPeriod = SiCXSLItemManager()->GetItemCSPointPeriod( kPacket.m_iUsedItemID );
					kPacketToDB.m_bUpdateEndDateOnly = true;
					//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
					kPacketToDB.m_iSkillPointItemID = kPacket.m_iUsedItemID;
					//}}
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacketToDB );
				}
				else
				{
					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데.." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;
				}
			}
		}
		break;
		//}}

		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
	case CXSLItem::SI_PET_AUTO_LOOTING_ITEM:
#ifdef SERV_PET_AUTO_LOOTING_ITEM_CN
	case CXSLItem::SI_PET_AUTO_LOOTING_ITEM_CN:
#endif // SERV_PET_AUTO_LOOTING_ITEM_CN
		{
			SetAutoLootingPet( kPacket_.m_iTempCode );
			KDBE_PET_AUTO_LOOTING_NOT kPacketToDB;
			kPacketToDB.m_iPetUID = kPacket_.m_iTempCode;
			SendToGameDB( DBE_PET_AUTO_LOOTING_NOT, kPacketToDB );
		}
		break;
#ifdef SERV_GOLD_TICKET
	case CXSLItem::SI_GOLD_TICKET_10_ID:
	case CXSLItem::SI_GOLD_TICKET_50_ID:
	case CXSLItem::SI_GOLD_TICKET_100_ID:
	case CXSLItem::SI_GOLD_TICKET_500_ID:
	case CXSLItem::SI_GOLD_TICKET_1000_ID:
	case CXSLItem::SI_GOLD_TICKET_2000_ID:
		{
			// 골드 티켓이 사용 되면 처리할 곳
			// 캐쉬 충전하라는 패킷을 날려 줘야겠지?
			KEPUBLISHER_CHARGE_POINT_REQ kPacketToDB;
			kPacketToDB.m_iItemUID = kPacket_.m_iItemUID;
			kPacketToDB.m_iItemID =  kPacket.m_iUsedItemID;
			kPacketToDB.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
			kPacketToDB.m_wstrUserIP = KncUtil::toWideString(GetIPStr());
			kPacketToDB.m_iCharUID = GetCharUID();
			kPacketToDB.m_wstrUserID = GetName();

			// 대만만 사용하는 거라 그냥 PublisherBillingDB로 보냄
			// 원래라면 체크해서 SendToPublisherBillingDB인지 SendToPublisherBilling인지 어디로 갈지 정해야함
			SendToPublisherBillingDB( EPUBLISHER_CHARGE_POINT_REQ, kPacketToDB );
		}
		break;
#endif //SERV_GOLD_TICKET
#ifdef SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
	case CXSLItem::SI_CHARACTER_SLOT_EXPAND_ITEM1:
	case CXSLItem::SI_CHARACTER_SLOT_EXPAND_ITEM2:
	case CXSLItem::SI_CHARACTER_SLOT_EXPAND_ITEM3:
		{
			if( ( m_nUnitSlot + 1 ) > CXSLUnit::CHAR_SLOT_MAX )
			{
				KEGS_BUY_CASH_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_30;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_EVENT_CHARACTER_SLOT_EXPAND_ITEM
#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	case CXSLItem::SI_HALLOWEEN_TRANSFORM_POSION_ITEM:
		{
			int iBeforePetID = 0;
			int iAfterPetID = 0;
			UseTransformItem( kPacket.m_iUsedItemID, kPacket_.m_iTempCode, iBeforePetID, iAfterPetID );

			KDBE_CHANGE_PET_ID_REQ kPacketToDB;
			kPacketToDB.m_iPetUID = kPacket_.m_iTempCode;
			kPacketToDB.m_iBeforePetID = iBeforePetID;
			kPacketToDB.m_iAfterPetID = iAfterPetID;
			SendToGameDB( DBE_CHANGE_PET_ID_REQ, kPacketToDB );
		}
		break;
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_EXPAND_INVENTORY_BY_EVENT_ITEM
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_EQUIP_EVENT:
		{
			int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE;
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;

			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_ACCESSORY_EVENT:
		{
			int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE;
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;

			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_QUICK_SLOT_EVENT:
		{
			int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE;
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;

			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_MATERIAL_EVENT:
		{
			int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE;
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;

			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_QUEST_EVENT:
		{
			int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE;
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;

			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );
		}
		break;
	case CXSLItem::SI_INVENTORY_SLOT_ADD_ITEM_SPECIAL_EVENT:
		{
			int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE;
			KDBE_EXPAND_INVENTORY_SLOT_REQ kPacketToDB;

			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot ) );
			SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacketToDB );
		}
		break;
#endif //SERV_EXPAND_INVENTORY_BY_EVENT_ITEM

	default:
		{
			// unseal skill
			//{{ 2009. 8. 4  최육사		캐쉬 스킬 아이템
			std::vector<int> vecSkillID;
			if( SiCXSLSkillTree()->GetUnsealItemInfo( kPacket.m_iUsedItemID, GetUnitClass(), vecSkillID ) == true )
			{
				BOOST_TEST_FOREACH( int, iSkillID, vecSkillID )
				{
					if( m_kSkillTree.IsSkillUnsealed( iSkillID ) == false )
					{
						KDBE_UNSEAL_SKILL_REQ kPacketToDB;
						kPacketToDB.m_iUnitUID = GetCharUID();
						kPacketToDB.m_iSkillID = iSkillID;
						SendToGameDB( DBE_UNSEAL_SKILL_REQ, kPacketToDB );

						// 혹시나 해서 스킬찍었는지도 검사
#ifdef SERV_SKILL_PAGE_SYSTEM
						if ( m_kSkillTree.IsExistOnUsedPage( iSkillID ) )
#else // SERV_SKILL_PAGE_SYSTEM
						if( m_kSkillTree.IsExist( iSkillID ) )
#endif // SERV_SKILL_PAGE_SYSTEM
						{
							START_LOG( cerr, L"봉인해제도 안되었는데 스킬포인트를 어떻게 찍었지?" )
								<< BUILD_LOG( GetCharUID() )
								<< BUILD_LOG( iSkillID )
								<< END_LOG;
						}
					}
				}
			}
			
			//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
			CXSLTitleManager::TitleSimpleInfo kTitleInfo;
			if( SiCXSLTitleManager()->GetUseItemTitleInfoByItemID( kPacket.m_iUsedItemID, kTitleInfo ) == true )
			{
				// 기간 무제한 칭호가 아닌 경우
				if( kTitleInfo.m_sPeriod > 0 )
				{
					// 칭호를 보유하고 있다면 기간 연장을 한다!
					if( m_kUserTitleManager.IsExistTitle( kTitleInfo.m_iTitleID ) == true )
					{
						// 칭호 기간을 연장하자!
						KDBE_INSERT_TITLE_REQ kPacketToDB;
						kPacketToDB.m_iUnitUID = GetCharUID();
						kPacketToDB.m_iTitleID = kTitleInfo.m_iTitleID;
						kPacketToDB.m_sPeriod  = kTitleInfo.m_sPeriod;
						kPacketToDB.m_bGameServerEvent = false;
						kPacketToDB.m_bExpandPeriod = true;
						SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );
						return;
					}
				}
			}
		}
		break;
	}
}
//}}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC( EGS_USE_ITEM_IN_INVENTORY_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_ITEM_IN_INVENTORY_REQ, EGS_USE_ITEM_IN_INVENTORY_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_USE_ITEM_IN_INVENTORY_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 아이템 사용 전 처리되는 부분

	bool bNotDeleteItem = false;
	int iWarpPointMapID = 0;
	const int iUsedItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );

	switch( iUsedItemID )
	{
		//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
	case CXSLItem::SI_GUILD_MAX_MEMBER_UPGRADE:
		{
			KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ kPacketToLg;
			kPacketToLg.m_iUnitUID = GetCharUID();
			kPacketToLg.m_iGuildUID = GetGuildUID();
			kPacketToLg.m_iItemUID = kPacket_.m_iItemUID;
			SendToLoginServer( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ, kPacketToLg );
			return;
		}
		break;
#endif GUILD_TEST
		//}}

		//{{ hoons.2009-10-19.
		//자동결재 마을 이동아이템일 경우 이동할수 있는 지역인지 체크..(흠~ 인벤사용함수가 범용적이지 않게 사용중이라 이곳에서 직접 마을아이디만 체크)
	case CXSLItem::SI_USE_FREE_BY_FIELD:
		//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
	case CXSLItem::SI_USE_COBO_EXPRESS_TICKET:
#endif SERV_ADD_COBO_EXPRESS_TICKET
		//}}
		{
			// 아이템 삭제하지 않도록 처리
			bNotDeleteItem = true;

			//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
#ifdef SERV_ENTER_VILLAGE_ERROR
			int iTempLevel = 0;
			int iTempDungeonID = 0;
			if( CheckEnterTheVillage( static_cast<int>( kPacket_.m_iTempCode ), iTempLevel, iTempDungeonID ) == false )
#else
			if( CheckEnterTheVillage( static_cast<int>( kPacket_.m_iTempCode ) ) == false )
#endif SERV_ENTER_VILLAGE_ERROR
			//}}
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_05;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
			else
			{
				//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
				if(	( GetMapID() == kPacket_.m_iTempCode )
				 || ( SiCXSLMapData()->CheckCOBOExpressTicketMapID( GetMapID() ) == 0 )
				 || ( SiCXSLMapData()->CheckCOBOExpressTicketMapID( static_cast<int>( kPacket_.m_iTempCode ) ) == 0 )
				 )
				{
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_UNKNOWN;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
				else if( ( iUsedItemID == CXSLItem::SI_USE_COBO_EXPRESS_TICKET )
					  && ( GetED() < SiCXSLMapData()->ComputeCOBOExpressTicketCost( GetMapID(), static_cast<int>( kPacket_.m_iTempCode ), GetLevel() ) )
					  )
				{
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_08;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
#endif SERV_ADD_COBO_EXPRESS_TICKET
				//}}

				iWarpPointMapID = static_cast<int>( kPacket_.m_iTempCode );
			}
		}
		break;
		//}} hoons.2009-10-19.

		// 마을 이동 아이템 사용 조건 검사
	case CXSLItem::SI_WARP_RUBEN:
	case CXSLItem::SI_WARP_ELDER:
	case CXSLItem::SI_WARP_BESMA:
	case CXSLItem::SI_WARP_ALTERA:
	case CXSLItem::SI_WARP_PEITA:
	case CXSLItem::SI_WARP_VELDER:
		{
			// 포탈아이템의 경우 이동할 mapid얻기
			switch( iUsedItemID )
			{
			case CXSLItem::SI_WARP_RUBEN:  iWarpPointMapID = SEnum::VMI_RUBEN; break;
			case CXSLItem::SI_WARP_ELDER:  iWarpPointMapID = SEnum::VMI_ELDER; break;
			case CXSLItem::SI_WARP_BESMA:  iWarpPointMapID = SEnum::VMI_BESMA; break;
			case CXSLItem::SI_WARP_ALTERA: iWarpPointMapID = SEnum::VMI_ALTERA; break;
			case CXSLItem::SI_WARP_PEITA:  iWarpPointMapID = SEnum::VMI_PEITA; break;
			case CXSLItem::SI_WARP_VELDER: iWarpPointMapID = SEnum::VMI_VELDER; break;
			default:					   iWarpPointMapID = SEnum::VMI_INVALID; break;
			}

			// 해당 map에 들어갈 수 있는지 조건 체크
			//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
#ifdef SERV_ENTER_VILLAGE_ERROR
			int iTempLevel = 0;
			int iTempDungeonID = 0;
			if( CheckEnterTheVillage( iWarpPointMapID, iTempLevel, iTempDungeonID ) == false )
#else
			if( CheckEnterTheVillage( iWarpPointMapID ) == false )
#endif SERV_ENTER_VILLAGE_ERROR
			//}}
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::GetLastError();
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;

		//{{ 2009. 11. 27  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	case CXSLItem::SI_GUILD_CASH_SKILL_POINT:
		{
			// 길드 가입 유저인지 검사
			if( GetGuildUID() <= 0 )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUILD_19;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}

			// 길드 마스터만 그노시스의 대축복 아이템을 사용할 수 있음.
			if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUILD_SKILL_15;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}

			// 이미 그노시스 대축복 아이템을 사용중이라면 다시 사용할 수 없다.
			if( m_kUserGuildManager.IsCashSkillPointExpired() == false )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUILD_SKILL_17;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif GUILD_SKILL_TEST
		//}}

		//{{ 2010. 04. 09  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	case CXSLItem::SI_SKILL_NOTE_ITEM:
		{
			if( m_kSkillTree.IsHaveSkillNote() == true )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_SKILL_NOTE_06;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_SKILL_NOTE
		//}}

		//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	case CXSLItem::SI_PSHOP_AGENCY_1_DAY:
	case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_15_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:
		{
			// 체험ID 기능 제한
			if( IsGuestUser() )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_PSHOP_AGENCY
		//}}

		//{{ 2011. 08. 26	최육사	캐릭터 레벨업 아이템
#ifdef SERV_CHAR_LEVEL_UP_ITEM
	case CXSLItem::EI_CHAR_LEVEL_UP_ITEM:
		{
			// 만렙이면 사용할 수 없다!
			if( SiKGameSysVal()->GetLimitsLevel() == GetLevel() )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_CHAR_LEVEL_UP_ITEM
		//}}

		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
	case CXSLItem::EI_SKILL_POINT_5_USE_INVEN:
		{
			if( m_kSkillTree.IsCashSkillPointExpired() == false )
			{
				if( 5 != m_kSkillTree.GetMaxCSPoint() )
				{
					START_LOG( cwarn, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;

					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
			}
		}
		break;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
		//}}
		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
	case CXSLItem::CI_SKILL_POINT_5_USE_INVEN:
		{
			if( m_kSkillTree.IsCashSkillPointExpired() == false )
			{
				if( 5 != m_kSkillTree.GetMaxCSPoint() )
				{
					START_LOG( cwarn, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;

					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
			}
		}
		break;

	case CXSLItem::CI_SKILL_POINT_10_USE_INVEN:
	case CXSLItem::CI_SKILL_POINT_10_USE_INVEN_ARA:
		{
			if( m_kSkillTree.IsCashSkillPointExpired() == false )
			{
				if( 10 != m_kSkillTree.GetMaxCSPoint() )
				{
					START_LOG( cwarn, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;

					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
			}
		}
		break;
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
		//{{ 2013. 03. 27	 그노시스의 축복 5point 7일 추가 - 김민성
#ifdef SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
	case CXSLItem::EI_SKILL_POINT_5_DAY_7_USE_INVEN:
		{
			if( m_kSkillTree.IsCashSkillPointExpired() == false )
			{
				if( 5 != m_kSkillTree.GetMaxCSPoint() )
				{
					START_LOG( cwarn, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;

					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
			}
		}
		break;
#endif SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
		//}
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_15_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_30_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_60_DAY:
		{
			if( m_kSkillTree.IsCashSkillPointExpired() == false )
			{
				if( 5 != m_kSkillTree.GetMaxCSPoint() )
				{
					START_LOG( cwarn, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;

					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_32;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
			}
		}
		break;
#endif SERV_COME_BACK_USER_REWARD
		//}}

		//{{ 2012. 04. 29	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_NUT:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_PTAH:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_RACHEL:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ARCANUM_ZADKIEL:
		if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_STRENGTHENING_BODY ) == true )
		{
			KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_07;
			SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
			return;
		}
		
		if( kPacket_.m_iTempCode != 0 )
		{
			if( ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_NUT )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_PTAH )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_LAHEL )	== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_PROTECTION_OF_ZACHIEL )	== true )
				)
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_07;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_SERAPHIM:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_GEB:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_AMON:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_CRONOS:
		//{{ 2013. 04. 12	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_GIANT_POTION:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_BLAZING_BOMB:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_SPIRIT_OF_CHASER:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_CRADLE_OF_LITTLE_FAIRY:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_ICE_BALL_OF_DENIPH:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_FEATHER_OF_VENTUS:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_FLAME_RING_OF_ROSSO:
	case CXSLItem::SI_THE_GATE_OF_DARKNESS_ELIXIR_BIG_HAND_POTION:
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
		if( kPacket_.m_iTempCode != 0 )
		{
			if( ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_SERAPHIM )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_GEB )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_AMON )				== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_BELSSING_OF_CRONOS )				== true )
				//{{ 2013. 04. 12	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_GIANT_POTION )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_BLAZING_BOMB )			== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_SPIRIT_OF_CHASER )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_CRADLE_OF_LITTLE_FAIRY )	== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_ICE_BALL_OF_DENIPH )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_FEATHER_OF_VENTUS )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_FLAME_RING_OF_ROSSO )		== true )
				|| ( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_BUFF_ELIXIR_BIG_HAND_POTION )			== true )
#endif SERV_NEW_DEFENCE_DUNGEON
				//}}
				)
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_06;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}

		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	case CXSLItem::SI_PET_AUTO_LOOTING_ITEM:
		{
			int iOK = CanIUseTheAutoLootingItem( kPacket_.m_iTempCode );
			if( iOK != NetError::NET_OK )
			{
				KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
				kPacket.m_iOK = iOK;
				SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
				return;
			}
		}
		break;
#endif SERV_PET_AUTO_LOOTING
		//}}

	default:
		{
			//{{ 2009. 8. 4  최육사		캐쉬 스킬 아이템
			int iSkillID = 0;
			char cUnitClass = 0;
			if( SiCXSLSkillTree()->GetUnsealItemInfo( iUsedItemID, iSkillID, cUnitClass ) == true )
			{
				// 해당 직업이 맞는가?
				if( CXSLSkillTree::CheckUnitClassForUnsealSkill( GetUnitClass(), cUnitClass ) == false )
				{					
					KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
					kPacket.m_iOK = NetError::ERR_USE_ITEM_IN_INVENTORY_00;
					SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
					return;
				}
			}

			//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
			CXSLTitleManager::TitleSimpleInfo kTitleInfo;
			if( SiCXSLTitleManager()->GetUseItemTitleInfoByItemID( iUsedItemID, kTitleInfo ) == true )
			{
				// 기간 무제한 칭호인 경우
				if( kTitleInfo.m_sPeriod == 0 )
				{
					// 칭호를 보유하고 있다면 아이템 사용을 할수 없다!
					if( m_kUserTitleManager.IsExistTitle( kTitleInfo.m_iTitleID ) == true )
					{
						KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
						kPacket.m_iOK = NetError::ERR_TITLE_05;
						SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
						return;
					}
				}
			}
#endif SERV_TITLE_ITEM_NEW
			//}}
		}
		break;
	}

	//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
	OnUseItemInInventory( kPacket_.m_iItemUID
						//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
						, kPacket_.m_iTempCode
#endif SERV_PET_AUTO_LOOTING
						//}}
						, bNotDeleteItem
						, iWarpPointMapID
						);
#else
	//////////////////////////////////////////////////////////////////////////
	// 인벤토리에서 아이템 사용!
//	if( m_kInventory.UseItemInInventory( kPacket_.m_iItemUID, kPacket.m_vecKInventorySlotInfo, bNotDeleteItem ) == false )
//	{
//		KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
//		kPacket.m_iOK = NetError::GetLastError();
//		SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
//		return;
//	}
//
//	KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
//	kPacket.m_iOK = NetError::NET_OK;
//	SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
//
//	//////////////////////////////////////////////////////////////////////////
//	// 아이템 사용 성공 후 처리되는 부분
//
//	// title mission
//	m_kUserTitleManager.OnUseItem( kPacket.m_iUsedItemID, GetThisPtr<KGSUser>() );
//
//	switch( kPacket.m_iUsedItemID )
//	{
//	case CXSLItem::CI_CHANGE_NICKNAME:
//		{
//			// DB로 닉네임을 __DELETED__로 바꾸는 이벤트를 날리자!
//			KDBE_DELETE_NICK_NAME_REQ kPacketToDB;
//			kPacketToDB.m_iUnitUID = GetCharUID();
//			kPacketToDB.m_wstrNickName = GetCharName();
//			SendToGameDB( DBE_DELETE_NICK_NAME_REQ, kPacketToDB );
//			return;
//		}
//		break;
//
//	case CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE:
//		{
//			if( CXSLInventory::GetNextBankMemberShip( m_kInventory.GetBankMemberShip() ) != CXSLInventory::MPB_NONE )
//			{
//				// 은행 확장하러 가자!
//				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
//				kPacket.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
//				kPacket.m_iUnitUID = GetCharUID();
//				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
//				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
//			}
//
//			return;
//		}
//		break;
//
//		//{{ 2009. 11. 27  최육사	길드스킬
//#ifdef GUILD_SKILL_TEST
//	case CXSLItem::SI_GUILD_CASH_SKILL_POINT:
//		{
//			// 길드매니저로 가서 길드 캐쉬 스킬포인트를 얻자!
//			KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT kPacketToLg;
//			kPacketToLg.m_iGuildUID = GetGuildUID();
//			kPacketToLg.m_iUnitUID = GetCharUID();
//			kPacketToLg.m_iGuildCSPoint = 5; // 5포인트 준다!
//			kPacketToLg.m_iPeriod = 15; // 15일
//			SendToLoginServer( ELG_INSERT_GUILD_CASH_SKILL_POINT_NOT, kPacketToLg );
//			return;
//		}
//		break;
//#endif GUILD_SKILL_TEST
//		//}}
//
//		//{{ 2010. 04. 09  최육사	기술의 노트
//#ifdef SERV_SKILL_NOTE
//	case CXSLItem::SI_SKILL_NOTE_ITEM:
//		{
//			char cSkillNoteMaxPageNum = 0;
//			if( m_kSkillTree.GetExpandSkillNotePage( GetLevel(), cSkillNoteMaxPageNum ) == true )
//			{
//				// 일단 기술의 노트 페이지 수를 저장하자!
//				m_kSkillTree.UpdateSkillNoteMaxPageNum( cSkillNoteMaxPageNum );
//
//				// 기술의 노트를 획득 하러 가자!
//				KDBE_EXPAND_SKILL_NOTE_PAGE_REQ kPacketToDB;
//				kPacketToDB.m_iUnitUID = GetCharUID();
//				kPacketToDB.m_cExpandedMaxPageNum = cSkillNoteMaxPageNum;
//				kPacketToDB.m_cRollBackMaxPageNum = 0;
//				SendToGameDB( DBE_EXPAND_SKILL_NOTE_PAGE_REQ, kPacketToDB );
//			}
//			else
//			{
//				START_LOG( cerr, L"기술의 노트 페이지 정보 얻기 실패! 절대 일어날 수 없는 에러!" )
//					<< BUILD_LOGc( GetLevel() )
//					<< END_LOG;
//			}
//		}
//		break;
//#endif SERV_SKILL_NOTE
//		//}}
//
//		//{{ 2011. 05. 11	최육사	대리상인
//#ifdef SERV_PSHOP_AGENCY
//	case CXSLItem::SI_PSHOP_AGENCY_1_DAY:
//	case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:
//	case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:
//	case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:
//	case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:
//	case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:
//	case CXSLItem::SI_PSHOP_AGENCY_15_DAYS:
//	case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:
//		{
//			KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
//			kPacketToDB.m_iUnitUID = GetCharUID();
//			kPacketToDB.m_sAgencyPeriod = CXSLItem::GetPShopAgencyDays( kPacket.m_iUsedItemID );
//			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
//			SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
//		}
//		break;
//#endif SERV_PSHOP_AGENCY
//		//}}
//
//		//{{ 2011. 08. 26	최육사	캐릭터 레벨업 아이템
//#ifdef SERV_CHAR_LEVEL_UP_ITEM
//	case CXSLItem::EI_CHAR_LEVEL_UP_ITEM:
//		{
//			// 1레벨업하기 위해서 필요한 경험치 구하기
//			const int iNextLevel = GetLevel() + 1;
//			const int iTotalExpByNextLevel = SiKExpTable()->GetRequireTotalExpbyLevel( iNextLevel );
//
//			// 1레벨업 할수 있을 정도의 경험치 더함.
//			m_kEXP.AddExp( iTotalExpByNextLevel - GetEXP() );
//
//			// 캐릭터 레벨업 체크
//			CheckCharLevelUp();
//		}
//		break;
//#endif SERV_CHAR_LEVEL_UP_ITEM
//		//}}
//
//		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
//#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
//	case CXSLItem::EI_SKILL_POINT_5_USE_INVEN:
//		{
//			if( true == m_kSkillTree.IsCashSkillPointExpired() )
//			{
//				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
//				kPacket.m_iUnitUID	= GetCharUID();
//				kPacket.m_iCSPoint	= 5;
//				kPacket.m_iPeriod	= 14;
//				kPacket.m_bUpdateEndDateOnly = false;
//				//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
//#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
//				kPacket.m_iSkillPointItemID = CXSLItem::EI_SKILL_POINT_5_USE_INVEN;
//#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
//				//}}
//				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
//			}
//			else
//			{
//				if( 5 == m_kSkillTree.GetMaxCSPoint() )
//				{
//					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
//					kPacket.m_iUnitUID = GetCharUID();
//					kPacket.m_iCSPoint = 5;
//					kPacket.m_iPeriod = 14;
//					kPacket.m_bUpdateEndDateOnly = true;
//					//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
//#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
//					kPacket.m_iSkillPointItemID = CXSLItem::EI_SKILL_POINT_5_USE_INVEN;
//#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
//					//}}
//					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
//				}
//				else
//				{
//					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데.." )
//						<< BUILD_LOG( GetCharUID() )
//						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
//						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
//						<< END_LOG;
//				}
//			}
//		}
//		break;
//#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
//		//}}
//
//		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
//#ifdef SERV_COME_BACK_USER_REWARD
//	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_15_DAY:
//	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_30_DAY:
//	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_60_DAY:
//		{
//			KDBE_INSERT_CASH_SKILL_POINT_REQ kReq;
//			if( true == m_kSkillTree.IsCashSkillPointExpired() )
//			{
//				kReq.m_iUnitUID	= GetCharUID();
//				kReq.m_iCSPoint	= 5;
//				kReq.m_iPeriod	= CXSLItem::GetComeBackRewardSkillPointDays( kPacket.m_iUsedItemID );;
//				kReq.m_bUpdateEndDateOnly = false;
//				kReq.m_iSkillPointItemID = kPacket.m_iUsedItemID;
//
//				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kReq );
//			}
//			else
//			{
//				if( 5 == m_kSkillTree.GetMaxCSPoint() )
//				{
//					kReq.m_iUnitUID = GetCharUID();
//					kReq.m_iCSPoint = 5;
//					kReq.m_iPeriod = CXSLItem::GetPShopAgencyDays( kPacket.m_iUsedItemID );;
//					kReq.m_bUpdateEndDateOnly = true;
//					kReq.m_iSkillPointItemID = kPacket.m_iUsedItemID;
//					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kReq );
//				}
//				else
//				{
//					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데" )
//						<< BUILD_LOG( GetCharUID() )
//						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
//						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
//						<< END_LOG;
//				}
//			}
//		}
//		break;		
//#endif SERV_COME_BACK_USER_REWARD
//		//}} 
//
//	default:
//		{
//			// unseal skill
//			//{{ 2009. 8. 4  최육사		캐쉬 스킬 아이템
//			int iSkillID = 0;
//			char cUnitClass = 0;
//			if( SiCXSLSkillTree()->GetUnsealItemInfo( kPacket.m_iUsedItemID, iSkillID, cUnitClass ) == true )
//			{
//				// 봉인해제된 스킬인지 검사
//				if( m_kSkillTree.IsSkillUnsealed( iSkillID ) == false )
//				{
//					KDBE_UNSEAL_SKILL_REQ kPacketToDB;
//					kPacketToDB.m_iUnitUID = GetCharUID();
//					kPacketToDB.m_iSkillID = iSkillID;
//					SendToGameDB( DBE_UNSEAL_SKILL_REQ, kPacketToDB );
//
//					// 혹시나 해서 스킬찍었는지도 검사
//					if( m_kSkillTree.IsExist( iSkillID ) == true )
//					{
//						START_LOG( cerr, L"봉인해제도 안되었는데 스킬포인트를 어떻게 찍었지?" )
//							<< BUILD_LOG( GetCharUID() )
//							<< BUILD_LOG( iSkillID )
//							<< END_LOG;
//					}
//				}
//			}
//
//			//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
//#ifdef SERV_TITLE_ITEM_NEW
//			CXSLTitleManager::TitleSimpleInfo kTitleInfo;
//			if( SiCXSLTitleManager()->GetUseItemTitleInfoByItemID( kPacket.m_iUsedItemID, kTitleInfo ) == true )
//			{
//				// 기간 무제한 칭호가 아닌 경우
//				if( kTitleInfo.m_sPeriod > 0 )
//				{
//					// 칭호를 보유하고 있다면 기간 연장을 한다!
//					if( m_kUserTitleManager.IsExistTitle( kTitleInfo.m_iTitleID ) == true )
//					{
//						// 칭호 기간을 연장하자!
//						KDBE_INSERT_TITLE_REQ kPacketToDB;
//						kPacketToDB.m_iUnitUID = GetCharUID();
//						kPacketToDB.m_iTitleID = kTitleInfo.m_iTitleID;
//						kPacketToDB.m_sPeriod  = kTitleInfo.m_sPeriod;
//						kPacketToDB.m_bGameServerEvent = false;
//						kPacketToDB.m_bExpandPeriod = true;
//						SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );
//						return;
//					}
//				}
//			}
//#endif SERV_TITLE_ITEM_NEW
//			//}}
//		}
//		break;
//	}
#endif SERV_USE_ITEM_DB_UPDATE_FIX
	//}}
}

//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
IMPL_ON_FUNC( DBE_USE_ITEM_IN_INVENTORY_ACK )
{	
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_USE_ITEM_IN_INVENTORY_ACK );

	KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iUsedItemID = kPacket_.m_iUsedItemID;
	kPacket.m_iWarpPointMapID = kPacket_.m_iWarpPointMapID;
	//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
	kPacket.m_iED	= kPacket_.m_iED;
#endif SERV_ADD_COBO_EXPRESS_TICKET
	//}}
	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	kPacket.m_iTempCode	= kPacket_.m_iTempCode;
#endif SERV_PET_AUTO_LOOTING
	//}}
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 아이템 사용 성공 후 처리되는 부분

	// title mission
	m_kUserTitleManager.OnUseItem( kPacket.m_iUsedItemID, GetThisPtr<KGSUser>() );

	// use item result
	switch( kPacket.m_iUsedItemID )
	{
	case CXSLItem::CI_CHANGE_NICKNAME:
		{
			// DB로 닉네임을 __DELETED__로 바꾸는 이벤트를 날리자!
			KDBE_DELETE_NICK_NAME_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_wstrNickName = GetCharName();
			SendToGameDB( DBE_DELETE_NICK_NAME_REQ, kPacketToDB );
			return;
		}
		break;

	case CXSLItem::SI_BANK_MEMBERSHIP_UPGRADE:
		{
			if( CXSLInventory::GetNextBankMemberShip( m_kInventory.GetBankMemberShip() ) != CXSLInventory::MPB_NONE )
			{
				// 은행 확장하러 가자!
				KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
				kPacket.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
				kPacket.m_iUnitUID = GetCharUID();
				kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
				SendToGameDB( DBE_EXPAND_INVENTORY_SLOT_REQ, kPacket );
			}

			return;
		}
		break;

		//{{ 2009. 11. 27  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	case CXSLItem::SI_GUILD_CASH_SKILL_POINT:
		{
			// 길드매니저로 가서 길드 캐쉬 스킬포인트를 얻자!
			KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT kPacketToLg;
			kPacketToLg.m_iGuildUID = GetGuildUID();
			kPacketToLg.m_iUnitUID = GetCharUID();
			kPacketToLg.m_iGuildCSPoint = 5; // 5포인트 준다!
			kPacketToLg.m_iPeriod = 15; // 15일
			SendToLoginServer( ELG_INSERT_GUILD_CASH_SKILL_POINT_NOT, kPacketToLg );
			return;
		}
		break;
#endif GUILD_SKILL_TEST
		//}}

		//{{ 2010. 04. 09  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	case CXSLItem::SI_SKILL_NOTE_ITEM:
		{
			char cSkillNoteMaxPageNum = 0;
			if( m_kSkillTree.GetExpandSkillNotePage( GetLevel(), cSkillNoteMaxPageNum ) == true )
			{
				// 일단 기술의 노트 페이지 수를 저장하자!
				m_kSkillTree.UpdateSkillNoteMaxPageNum( cSkillNoteMaxPageNum );

				// 기술의 노트를 획득 하러 가자!
				KDBE_EXPAND_SKILL_NOTE_PAGE_REQ kPacketToDB;
				kPacketToDB.m_iUnitUID = GetCharUID();
				kPacketToDB.m_cExpandedMaxPageNum = cSkillNoteMaxPageNum;
				kPacketToDB.m_cRollBackMaxPageNum = 0;
				SendToGameDB( DBE_EXPAND_SKILL_NOTE_PAGE_REQ, kPacketToDB );
			}
			else
			{
				START_LOG( cerr, L"기술의 노트 페이지 정보 얻기 실패! 절대 일어날 수 없는 에러!" )
					<< BUILD_LOGc( GetLevel() )
					<< END_LOG;
			}
		}
		break;
#endif SERV_SKILL_NOTE
		//}}

		//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	case CXSLItem::SI_PSHOP_AGENCY_1_DAY:
	case CXSLItem::SI_PSHOP_AGENCY_2_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_3_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_5_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_7_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_10_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_15_DAYS:
	case CXSLItem::SI_PSHOP_AGENCY_30_DAYS:
		{
			KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
			kPacketToDB.m_iUnitUID = GetCharUID();
			kPacketToDB.m_sAgencyPeriod = CXSLItem::GetPShopAgencyDays( kPacket.m_iUsedItemID );
			kPacketToDB.m_usEventID = EGS_USE_ITEM_IN_INVENTORY_REQ;
			SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
		}
		break;
#endif SERV_PSHOP_AGENCY
		//}}

		//{{ 2011. 08. 26	최육사	캐릭터 레벨업 아이템
#ifdef SERV_CHAR_LEVEL_UP_ITEM
	case CXSLItem::EI_CHAR_LEVEL_UP_ITEM:
		{
			// 1레벨업하기 위해서 필요한 경험치 구하기
			const int iNextLevel = GetLevel() + 1;
			const int iTotalExpByNextLevel = SiKExpTable()->GetRequireTotalExpbyLevel( iNextLevel );

			// 1레벨업 할수 있을 정도의 경험치 더함.
			m_kEXP.AddExp( iTotalExpByNextLevel - GetEXP() );

			// 캐릭터 레벨업 체크
			CheckCharLevelUp();
		}
		break;
#endif SERV_CHAR_LEVEL_UP_ITEM
		//}}

		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
	case CXSLItem::EI_SKILL_POINT_5_USE_INVEN:
		{
			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
				kPacket.m_iUnitUID	= GetCharUID();
				kPacket.m_iCSPoint	= 5;
				kPacket.m_iPeriod	= 14;
				kPacket.m_bUpdateEndDateOnly = false;
				//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
				kPacket.m_iSkillPointItemID = CXSLItem::EI_SKILL_POINT_5_USE_INVEN;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
				//}}
				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
			}
			else
			{
				if( 5 == m_kSkillTree.GetMaxCSPoint() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_iCSPoint = 5;
					kPacket.m_iPeriod = 14;
					kPacket.m_bUpdateEndDateOnly = true;
					//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
					kPacket.m_iSkillPointItemID = CXSLItem::EI_SKILL_POINT_5_USE_INVEN;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
					//}}
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
				}
				else
				{
					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데.." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;
				}
			}
		}
		break;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
		//}}
		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
	case CXSLItem::CI_SKILL_POINT_5_USE_INVEN:
		{
			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
				kPacket.m_iUnitUID	= GetCharUID();
				kPacket.m_iCSPoint	= 5;
				kPacket.m_iPeriod	= 15;
				kPacket.m_bUpdateEndDateOnly = false;
				kPacket.m_iSkillPointItemID = CXSLItem::CI_SKILL_POINT_5_USE_INVEN;
				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
			}
			else
			{
				if( 5 == m_kSkillTree.GetMaxCSPoint() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_iCSPoint	= 5;
					kPacket.m_iPeriod	= 15;
					kPacket.m_bUpdateEndDateOnly = true;
					kPacket.m_iSkillPointItemID = CXSLItem::EI_SKILL_POINT_5_USE_INVEN;
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
				}
				else
				{
					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데.." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;
				}
			}
		}
		break;

	case CXSLItem::CI_SKILL_POINT_10_USE_INVEN:
		{
			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
				kPacket.m_iUnitUID	= GetCharUID();
				kPacket.m_iCSPoint	= 10;
				kPacket.m_iPeriod	= 15;
				kPacket.m_bUpdateEndDateOnly = false;
				kPacket.m_iSkillPointItemID = CXSLItem::CI_SKILL_POINT_10_USE_INVEN;
				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
			}
			else
			{
				if( 10 == m_kSkillTree.GetMaxCSPoint() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_iCSPoint	= 10;
					kPacket.m_iPeriod	= 15;
					kPacket.m_bUpdateEndDateOnly = true;
					kPacket.m_iSkillPointItemID = CXSLItem::CI_SKILL_POINT_10_USE_INVEN;
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
				}
				else
				{
					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데.." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;
				}
			}
		}
		break;

	case CXSLItem::CI_SKILL_POINT_10_USE_INVEN_ARA:
		{

			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
				kPacket.m_iUnitUID	= GetCharUID();
				kPacket.m_iCSPoint	= 10;
				kPacket.m_iPeriod	= 30;
				kPacket.m_bUpdateEndDateOnly = false;
				kPacket.m_iSkillPointItemID = CXSLItem::CI_SKILL_POINT_10_USE_INVEN_ARA;
				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
			}
			else
			{
				if( 10 == m_kSkillTree.GetMaxCSPoint() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_iCSPoint	= 10;
					kPacket.m_iPeriod	= 30;
					kPacket.m_bUpdateEndDateOnly = true;
					kPacket.m_iSkillPointItemID = CXSLItem::CI_SKILL_POINT_10_USE_INVEN_ARA;
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
				}
				else
				{
					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데.." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;
				}
			}
		}
		break;
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
		//{{ 2013. 03. 27	 그노시스의 축복 5point 7일 추가 - 김민성
#ifdef SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
	case CXSLItem::EI_SKILL_POINT_5_DAY_7_USE_INVEN:
		{
			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
				kPacket.m_iUnitUID	= GetCharUID();
				kPacket.m_iCSPoint	= 5;
				kPacket.m_iPeriod	= 7;
				kPacket.m_bUpdateEndDateOnly = false;
				kPacket.m_iSkillPointItemID = CXSLItem::EI_SKILL_POINT_5_DAY_7_USE_INVEN;
				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
			}
			else
			{
				if( 5 == m_kSkillTree.GetMaxCSPoint() )
				{
					KDBE_INSERT_CASH_SKILL_POINT_REQ kPacket;
					kPacket.m_iUnitUID = GetCharUID();
					kPacket.m_iCSPoint	= 5;
					kPacket.m_iPeriod	= 7;
					kPacket.m_bUpdateEndDateOnly = true;
					kPacket.m_iSkillPointItemID = CXSLItem::EI_SKILL_POINT_5_DAY_7_USE_INVEN;
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kPacket );
				}
				else
				{
					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데.." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;
				}
			}
		}
		break;
#endif SERV_EVENT_CASH_SKILL_5_POINT_7_DAY
		//}
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_15_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_30_DAY:
	case CXSLItem::SI_COME_BACK_REWARD_SKILL_5_POINT_60_DAY:
		{
			KDBE_INSERT_CASH_SKILL_POINT_REQ kReq;
			if( true == m_kSkillTree.IsCashSkillPointExpired() )
			{
				kReq.m_iUnitUID	= GetCharUID();
				kReq.m_iCSPoint	= 5;
				kReq.m_iPeriod	= CXSLItem::GetComeBackRewardSkillPointDays( kPacket.m_iUsedItemID );;
				kReq.m_bUpdateEndDateOnly = false;
				kReq.m_iSkillPointItemID = kPacket.m_iUsedItemID;

				SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kReq );
			}
			else
			{
				if( 5 == m_kSkillTree.GetMaxCSPoint() )
				{
					kReq.m_iUnitUID = GetCharUID();
					kReq.m_iCSPoint = 5;
					kReq.m_iPeriod = CXSLItem::GetComeBackRewardSkillPointDays( kPacket.m_iUsedItemID );;
					kReq.m_bUpdateEndDateOnly = true;
					kReq.m_iSkillPointItemID = kPacket.m_iUsedItemID;
					SendToGameDB( DBE_INSERT_CASH_SKILL_POINT_REQ, kReq );
				}
				else
				{
					START_LOG( cerr, L"현재 사용하고 있는 캐시스킬포인트와 다른 것으로 기간연장 하려고함!! 위에서 이미 검사했을텐데" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( m_kSkillTree.GetMaxCSPoint() )
						<< BUILD_LOG( m_kSkillTree.GetCSPointEndDateString() )
						<< END_LOG;
				}
			}
		}
		break;		
#endif SERV_COME_BACK_USER_REWARD
		//}}

		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	case CXSLItem::SI_PET_AUTO_LOOTING_ITEM:
		{
			SetAutoLootingPet( kPacket_.m_iTempCode );
			KDBE_PET_AUTO_LOOTING_NOT kPacket;
			kPacket.m_iPetUID = kPacket_.m_iTempCode;
			SendToGameDB( DBE_PET_AUTO_LOOTING_NOT, kPacket );
		}
		break;
#endif SERV_PET_AUTO_LOOTING
		//}}

	default:
		{
			// unseal skill
			//{{ 2009. 8. 4  최육사		캐쉬 스킬 아이템
			int iSkillID = 0;
			char cUnitClass = 0;
			if( SiCXSLSkillTree()->GetUnsealItemInfo( kPacket.m_iUsedItemID, iSkillID, cUnitClass ) == true )
			{
				// 봉인해제된 스킬인지 검사
				if( m_kSkillTree.IsSkillUnsealed( iSkillID ) == false )
				{
					KDBE_UNSEAL_SKILL_REQ kPacketToDB;
					kPacketToDB.m_iUnitUID = GetCharUID();
					kPacketToDB.m_iSkillID = iSkillID;
					SendToGameDB( DBE_UNSEAL_SKILL_REQ, kPacketToDB );

					// 혹시나 해서 스킬찍었는지도 검사
					if( m_kSkillTree.IsExist( iSkillID ) == true )
					{
						START_LOG( cerr, L"봉인해제도 안되었는데 스킬포인트를 어떻게 찍었지?" )
							<< BUILD_LOG( GetCharUID() )
							<< BUILD_LOG( iSkillID )
							<< END_LOG;
					}
				}
			}
			
			//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
			CXSLTitleManager::TitleSimpleInfo kTitleInfo;
			if( SiCXSLTitleManager()->GetUseItemTitleInfoByItemID( kPacket.m_iUsedItemID, kTitleInfo ) == true )
			{
				// 기간 무제한 칭호가 아닌 경우
				if( kTitleInfo.m_sPeriod > 0 )
				{
					// 칭호를 보유하고 있다면 기간 연장을 한다!
					if( m_kUserTitleManager.IsExistTitle( kTitleInfo.m_iTitleID ) == true )
					{
						// 칭호 기간을 연장하자!
						KDBE_INSERT_TITLE_REQ kPacketToDB;
						kPacketToDB.m_iUnitUID = GetCharUID();
						kPacketToDB.m_iTitleID = kTitleInfo.m_iTitleID;
						kPacketToDB.m_sPeriod  = kTitleInfo.m_sPeriod;
						kPacketToDB.m_bGameServerEvent = false;
						kPacketToDB.m_bExpandPeriod = true;
						SendToGameDB( DBE_INSERT_TITLE_REQ, kPacketToDB );
						return;
					}
				}
			}
#endif SERV_TITLE_ITEM_NEW
			//}}
		}
		break;
	}
}
#endif SERV_USE_ITEM_DB_UPDATE_FIX
//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
//}}


//{{ 2008. 12. 19  최육사	아이템 복구
IMPL_ON_FUNC( EGS_RESTORE_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESTORE_ITEM_REQ, EGS_RESTORE_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_RESTORE_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_RESTORE_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_RESTORE_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_RESTORE_ITEM_REQ kPacketReq;
	KEGS_RESTORE_ITEM_ACK kPacket;

	int iEnchantLevel = 0;
	if( !m_kInventory.GetEnchantLevel( kPacket_.m_iItemUID, iEnchantLevel ) )
	{		
		kPacket.m_iOK = NetError::ERR_RESTORE_ITEM_00;
		SendPacket( EGS_RESTORE_ITEM_ACK, kPacket );
		return;
	}

	// 사용불가 아이템 복구
	if( !m_kInventory.RestoreItem( kPacket_.m_iItemUID, kPacketReq.m_iLevelAfterEnchant, kPacketReq.m_vecUpdatedInventorySlot ) )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_RESTORE_ITEM_ACK, kPacket );
		return;
	}

	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iItemUID = kPacket_.m_iItemUID;
	m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_RESTORE_ITEM_REQ, kPacketReq );

	//{{ 2008. 12. 22  최육사	복구 로그
	KELOG_RESTORE_ITEM_LOG_NOT kPacketLog;
	kPacketLog.m_iUnitUID		  = GetCharUID();
	kPacketLog.m_iItemUID		  = kPacket_.m_iItemUID;
	kPacketLog.m_iBeforeEnchantLv = iEnchantLevel;
	kPacketLog.m_iAfterEnchantLv  = kPacketReq.m_iLevelAfterEnchant;
	SendToLogDB( ELOG_RESTORE_ITEM_LOG_NOT, kPacketLog );
	//}}
}

IMPL_ON_FUNC( DBE_RESTORE_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESTORE_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 복구 업데이트 실패!
	}

	KEGS_RESTORE_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_RESTORE_ITEM_ACK, kPacket );
}
//}}

//{{ 2008. 12. 25  최육사	부여
IMPL_ON_FUNC( EGS_ENCHANT_ATTACH_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ENCHANT_ATTACH_ITEM_REQ, EGS_ENCHANT_ATTACH_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ENCHANT_ATTACH_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_ENCHANT_ATTACH_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ENCHANT_ATTACH_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_ENCHANT_ATTACH_ITEM_REQ kPacketReq;
	KEGS_ENCHANT_ATTACH_ITEM_ACK kPacket;

	// 부여 아이템ID 얻기
	const int iAttachItemID = m_kInventory.GetItemID( kPacket_.m_iAttachItemUID );

	// 목표 아이템의 적용전 강화 레벨 얻기
	int iBeforeEnchantLevel = 0;
	if( m_kInventory.GetEnchantLevel( kPacket_.m_iDestItemUID, iBeforeEnchantLevel ) == false )
	{
		kPacket.m_iOK = NetError::ERR_ATTACH_ITEM_00;
		SendPacket( EGS_ENCHANT_ATTACH_ITEM_ACK, kPacket );
		return;
	}

	// 부여 아이템
	if( m_kInventory.EnchantAttachItem( kPacket_.m_iDestItemUID, kPacket_.m_iAttachItemUID, kPacketReq.m_iLevelAfterEnchant, kPacketReq.m_vecUpdatedInventorySlot ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ENCHANT_ATTACH_ITEM_ACK, kPacket );
		return;
	}

	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iItemUID = kPacket_.m_iDestItemUID;
	m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_ENCHANT_ATTACH_ITEM_REQ, kPacketReq );

	// 로그 기록
	KELOG_ATTACH_ITEM_ENCHANT_LOG_NOT kPacketLog;
	kPacketLog.m_iUnitUID		  = GetCharUID();
	kPacketLog.m_iAttachItemID	  = iAttachItemID;
	kPacketLog.m_iDestItemUID	  = kPacket_.m_iDestItemUID;
	kPacketLog.m_iBeforeEnchantLv = iBeforeEnchantLevel;
	kPacketLog.m_iAfterEnchantLv  = kPacketReq.m_iLevelAfterEnchant;
	SendToLogDB( ELOG_ATTACH_ITEM_ENCHANT_LOG_NOT, kPacketLog );
}
//}}

IMPL_ON_FUNC( DBE_ENCHANT_ATTACH_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ENCHANT_ATTACH_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 부여 DB업데이트 실패
	}

	KEGS_ENCHANT_ATTACH_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecKInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_ENCHANT_ATTACH_ITEM_ACK, kPacket );
}

//{{ 2008. 11. 18  최육사	아이템 교환
IMPL_ON_FUNC( EGS_ITEM_EXCHANGE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_EXCHANGE_REQ, EGS_ITEM_EXCHANGE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ITEM_EXCHANGE_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_ITEM_EXCHANGE_ACK kPacket;
			kPacket.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacket );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.12 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	std::set<int> setBanBuyItemInven;
	CTime m_tStartDate;
	CTime m_tEndDate;
	std::vector<KPacketGetItemOnOff>::iterator vitrGetItemOnOff;
	std::map<int , std::vector<KPacketGetItemOnOff> > mapGetItemOnOff =  GetKGSSimLayer()->GetTimeControlItem_Info();
	std::map<int , std::vector<KPacketGetItemOnOff> >::iterator mitGetItemOnOff = mapGetItemOnOff.find( TCIT_EXCHANGE_SHOP );

	if( mitGetItemOnOff != mapGetItemOnOff.end() )
	{
		for( vitrGetItemOnOff = mitGetItemOnOff->second.begin(); vitrGetItemOnOff != mitGetItemOnOff->second.end(); ++vitrGetItemOnOff )
		{
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrStartTime , m_tStartDate);
			KncUtil::ConvertStringToCTime(vitrGetItemOnOff->m_wstrEndTime , m_tEndDate);

			if( CTime::GetCurrentTime() >= m_tStartDate && CTime::GetCurrentTime() <= m_tEndDate )
			{
				setBanBuyItemInven.insert(vitrGetItemOnOff->m_iItemID);
			}
		}
	}

	if( setBanBuyItemInven.find(kPacket_.m_iSourceItemID) != setBanBuyItemInven.end() )
	{
		KEGS_ITEM_EXCHANGE_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_ITEM_22;
		SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacket );
		return;
	}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	int iEventMoney = GetEventMoney();
#endif // SERV_EVENT_MONEY

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
	//////////////////////////////////////////
	// 최소한 인벤토리 공간 체크는 해야한다.//
	//////////////////////////////////////////
	if( m_kInventory.ItemExchangeCheck( kPacket_.m_iHouseID
		, kPacket_.m_iSourceItemUID
		, kPacket_.m_iSourceItemID
		, kPacket_.m_iSourceQuantity
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		, iEventMoney
#endif // SERV_EVENT_MONEY
		, kPacket_.m_iDestItemID ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_ITEM_EXCHANGE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
		return;
	}

	const int iResult = GetKGameServer()->DecreaseExchangeLimit( kPacket_.m_iDestItemID );

	switch( iResult )
	{
	case KGameServer::ELR_NOT_INITED:
		{
			START_LOG( cerr, L"ExchangeLimit가 아직 초기화 되지 않은 상태입니다.")
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;

			KEGS_ITEM_EXCHANGE_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_EXCHANGE_LIMIT_01;	// 교환 제한_아이템 교환에 실패하였습니다.
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
			return;
		}
		break;

	case KGameServer::ELR_EXHAUSTED:
		{
			KEGS_ITEM_EXCHANGE_ACK kPacketAck;
#ifdef SERV_ITEM_EXCHANGE_LIMIT_POPUP// 작업날짜: 2013-07-02	// 박세훈
			switch( kPacket_.m_iDestItemID )
			{
			case 135422:
			case 135423:
			case 135424:
				kPacketAck.m_iOK = NetError::ERR_EXCHANGE_LIMIT_03;	// 오늘의 붉은 기사단 초대 확정권은\n모두 소진되었습니다.\n일일 수량은 06시에 갱신됩니다.\n내일 다시 도전해주세요!
				break;

			default:
				kPacketAck.m_iOK = NetError::ERR_EXCHANGE_LIMIT_02;	// 교환 가능한 수량이 모두 소진되었습니다.
				break;
			}
#else // SERV_ITEM_EXCHANGE_LIMIT_POPUP
			kPacketAck.m_iOK = NetError::ERR_EXCHANGE_LIMIT_02;	// 교환 가능한 수량이 모두 소진되었습니다.
#endif // SERV_ITEM_EXCHANGE_LIMIT_POPUP
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
			return;
		}
		break;

	case KGameServer::ELR_CHECK:
		{
			// 교환 체크하러 갑시다.
			KEGB_EXCHANGE_LIMIT_REQ kPacket;
			if( m_kAfterWorkStorage.InsertData( kPacket_, kPacket.m_iAfterWorkStorageKey ) == KAfterWorkStorage::AWS_SUCCEED )
			{
				kPacket.m_iGSUID				= GetKGameServer()->GetUID();
				kPacket.m_iUnitUID				= GetCharUID();
				kPacket.m_iDestItemID			= kPacket_.m_iDestItemID;
				SendToGlobalServer( EGB_EXCHANGE_LIMIT_REQ, kPacket );
				return;
			}
			else
			{
				START_LOG( cerr, L"AfterWorkStorage: Key값을 얻을 수 없습니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				KEGS_ITEM_EXCHANGE_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_EXCHANGE_LIMIT_01;	// 교환 제한_아이템 교환에 실패하였습니다.
				SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
				return;
			}
		}
		break;

	case KGameServer::ELR_PASS:
		{	
			// 교환 제한 대상이 아닙니다.
			KEGB_EXCHANGE_LIMIT_ACK kPacket;
			if( m_kAfterWorkStorage.InsertData( kPacket_, kPacket.m_iAfterWorkStorageKey ) == KAfterWorkStorage::AWS_SUCCEED )
			{
				kPacket.m_iOK = NetError::NET_OK;
				KEventPtr spEvent( new KEvent );
				spEvent->SetData( PI_GS_USER, NULL, EGB_EXCHANGE_LIMIT_ACK, kPacket );
				QueueingEvent( spEvent );
				return;
			}
			else
			{
				START_LOG( cerr, L"AfterWorkStorage: Key값을 얻을 수 없습니다." )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				KEGS_ITEM_EXCHANGE_ACK kPacketAck;
				kPacketAck.m_iOK = NetError::ERR_EXCHANGE_LIMIT_01;	// 교환 제한_아이템 교환에 실패하였습니다.
				SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
				return;
			}
		}

	default:
		{
			START_LOG( cerr, L"DecreaseExchangeLimit: 알 수 없는 결과 값입니다.")
				<< BUILD_LOG( iResult )
				<< END_LOG;

			KEGS_ITEM_EXCHANGE_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_EXCHANGE_LIMIT_01;	// 교환 제한_아이템 교환에 실패하였습니다.
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
			return;
		}
	}
#else // SERV_ITEM_EXCHANGE_LIMIT
	KDBE_ITEM_EXCHANGE_REQ kPacketToDB;
	std::map< int, int > mapInsertedItem;

	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	kPacketToDB.m_iSourceItemID = kPacket_.m_iSourceItemID;
	kPacketToDB.m_iSourceItemQuantity = kPacket_.m_iSourceQuantity;
#endif SERV_EXCHANGE_LOG
	//}

	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	if( m_kInventory.ItemExchange( kPacket_.m_iHouseID, 
		kPacket_.m_iSourceItemUID, 
		kPacket_.m_iSourceItemID, 
		kPacket_.m_iSourceQuantity, 
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		iEventMoney,
#endif // SERV_EVENT_MONEY
		kPacket_.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo, 
		kPacketToDB.m_cExchangeType		) == false )
#else
/*
	//{{ 2011. 08. 16	최육사	아이템 교환 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	int iUserEventMoney = GetAngelFeather();
	if( m_kInventory.ItemExchange( kPacket_.m_iHouseID, 
		kPacket_.m_iSourceItemUID, 
		kPacket_.m_iSourceItemID, 
		kPacket_.m_iSourceQuantity, 
		kPacket_.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo,
		//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		iUserEventMoney,
		kPacketToDB.m_vecDestItem
#else
		iUserEventMoney
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		//}}
		) == false )
#else // SERV_EVENT_MONEY
	if( m_kInventory.ItemExchange( kPacket_.m_iHouseID, 
		kPacket_.m_iSourceItemUID, 
		kPacket_.m_iSourceItemID, 
		kPacket_.m_iSourceQuantity, 
		kPacket_.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo 
		) == false )
#endif // SERV_EVENT_MONEY
#else
	if( m_kInventory.ItemExchange( kPacket_.m_iSourceItemID, 
		kPacket_.m_iHouseID, 
		kPacket_.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo 
		) == false )
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
*/
#endif SERV_EXCHANGE_LOG
	//}
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_ITEM_EXCHANGE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
		return;
	}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	// 천사의 깃털을 사용하였다.
	if( iEventMoney > 0 )
	{
		CTime tCurr = CTime::GetCurrentTime();

		KDBE_UPDATE_EVENT_MONEY_REQ kReq;
		kReq.m_iUserUID = GetUID();
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_iOldQuantity = GetEventMoney();
		kReq.m_iNewQuantity = (-1) * iEventMoney; // 마이너스로 변경
		kReq.m_iRewardItemID = 0;
		std::map<int,int>::iterator mitInsert = mapInsertedItem.begin();
		if( mitInsert != mapInsertedItem.end() )
		{
			kReq.m_iRewardItemID = mitInsert->first;
		}
		kReq.m_wstrRegDate = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		SendToAccountDB( DBE_UPDATE_EVENT_MONEY_REQ, kReq );

		SetEventMoney( GetEventMoney() - iEventMoney );
	}
#endif // SERV_EVENT_MONEY

	//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
	kPacketToDB.m_mapResultItem	= mapInsertedItem;
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
	//}}

	// 교환된 아이템을 생성하기위해 DB로 간다
	kPacketToDB.m_iUnitUID = GetCharUID();	
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	
	//{{ 2012. 12. 19  교환(한정 수량) - 김민성
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
	// 한정 수량 교환 물품인지 확인
	std::map<int, int>::iterator mit = mapInsertedItem.find( CXSLItem::EI_ARA_EVENT_NEXON_CASH_1000 );
	if( mit != mapInsertedItem.end() )
	{
		// 결과물이 여러 개 인데 한정 수량 교환은 이상하다!
		if( mapInsertedItem.size() != 1 )
		{
			KEGS_ITEM_EXCHANGE_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_ITEM_EXCHANGE_00;
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
			return;
		}

		KDBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ kDBReq;

		kDBReq.m_kReq = kPacket_;
		kDBReq.m_kToDB = kPacketToDB;

		// 한정 수량 아이템이면 수량 확인 후 교환 진행 하도록 한다.
		SendToAccountDB( DBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ, kDBReq );
		return;
	}
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
	//}}
	SendToGameDB( DBE_ITEM_EXCHANGE_REQ, kPacketToDB );

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_ITEM_EXCHANGE, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif // SERV_ITEM_EXCHANGE_LIMIT
}

IMPL_ON_FUNC( DBE_ITEM_EXCHANGE_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP,	KGSFSM::S_ROOM ), EGS_ITEM_EXCHANGE_ACK );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_ITEM_EXCHANGE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
	kPacket.m_mapResultItem = kPacket_.m_mapResultItem;
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
	//}}

#ifdef SERV_GROW_UP_SOCKET
	m_iExchangeCount = kPacket_.m_iExchangeCount;
	kPacket.m_iExchangeCount = kPacket_.m_iExchangeCount;
#endif SERV_GROW_UP_SOCKET

#ifdef SERV_2013_JUNGCHU_TITLE
#ifdef SERV_2013_SILVER_WEEK_TITLE
	// 일본은 보상을 주지 않는다.
#else //SERV_2013_SILVER_WEEK_TITLE
	IF_EVENT_ENABLED( CEI_2013_JUNGCHU_TITLE )
	{
		if( kPacket_.m_b12TimesRewarded == true )
		{
			// 이벤트 보상을 주자!
			KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
			kPacketToDB.m_iFromUnitUID = GetCharUID();
			kPacketToDB.m_iToUnitUID   = GetCharUID();
			kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
			kPacketToDB.m_iRewardID	   = 1488;	// 달나라 우주선 100 보너스 캐시 쿠폰
			SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
		}
	}
#endif //SERV_2013_SILVER_WEEK_TITLE
#endif SERV_2013_JUNGCHU_TITLE

	//std::map< UidType, KItemInfo >::const_iterator mit = kPacket_.m_mapItemInfo.begin();
	//if( mit != kPacket_.m_mapItemInfo.end() )
	//{
	//	// 생성된 아이템 정보 얻기
	//	kPacket.m_kChangedItemInfo = mit->second;

	//	// 로그 기록
	//	KELOG_ITEM_EXCHANGE_LOG_NOT kPacketLog;
	//	kPacketLog.m_iUnitUID = GetCharUID();
	//	kPacketLog.m_iDeletedItemID  = kPacket_.m_iDeletedItemID;
	//	kPacketLog.m_iDeletedItemUID = kPacket_.m_iDeletedItemUID;
	//	kPacketLog.m_iChangedItemID  = mit->second.m_iItemID;
	//	kPacketLog.m_iChangedItemUID = mit->first;
	//	SendToLogDB( ELOG_ITEM_EXCHANGE_LOG_NOT, kPacketLog );
	//}
	//else
	//{
	//	START_LOG( cerr, L"아이템 교환 로그 기록 실패! 아이템 생성 실패인가?" )
	//		<< BUILD_LOG( GetCharUID() )
	//		<< BUILD_LOG( kPacket_.m_iDeletedItemID )
	//		<< BUILD_LOG( kPacket_.m_iDeletedItemUID )
	//		<< END_LOG;
	//}

	SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacket );

	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		CTime tCurr = CTime::GetCurrentTime();
		
		KDBE_ITEM_EXCHANGE_LOG_NOT kNot;
		kNot.m_iUserUID = GetUID();
		kNot.m_iUnitUID = GetCharUID();
		kNot.m_iSourceItemID = kPacket_.m_iSourceItemID;
		kNot.m_iSourceItemQuantity = kPacket_.m_iSourceItemQuantity;
		kNot.m_cExchangeType = kPacket_.m_cExchangeType;
		kNot.m_mapResultItem = kPacket_.m_mapResultItem;
		kNot.m_wstrRegDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );

		SendToLogDB( DBE_ITEM_EXCHANGE_LOG_NOT, kNot );
	}
#endif SERV_EXCHANGE_LOG
	//}
}
//}}

//{{ 2009. 8. 7  최육사		은행
IMPL_ON_FUNC_NOPARAM( EGS_GET_MY_BANK_INFO_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_GET_MY_BANK_INFO_ACK kPacket;
	m_kInventory.GetInventoryInfo( CXSLInventory::ST_BANK, kPacket.m_mapItem );
	SendPacket( EGS_GET_MY_BANK_INFO_ACK, kPacket );
}
//}}

//{{ 2009. 8. 27  최육사	밀봉
IMPL_ON_FUNC( EGS_SEAL_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SEAL_ITEM_REQ, EGS_SEAL_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_SEAL_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_SEAL_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_SEAL_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_SEAL_ITEM_REQ kPacketToDB;
	u_char ucBeforeSealCount = 0;

	if( m_kInventory.SealItem( kPacket_.m_iDestItemUID, kPacket_.m_iSealItemUID, ucBeforeSealCount, kPacketToDB.m_ucSealResult, kPacketToDB.m_vecUpdatedInventorySlot ) == false )
	{
		KEGS_SEAL_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_SEAL_ITEM_ACK, kPacketAck );
		return;
	}

	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iItemUID = kPacket_.m_iDestItemUID;
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_SEAL_ITEM_REQ, kPacketToDB );

	// 사용 로그 남기기
	KELOG_USE_SEAL_ITEM_NOT kPacketToLog;
	kPacketToLog.m_iUnitUID = GetCharUID();
	kPacketToLog.m_iDestItemUID = kPacket_.m_iDestItemUID;
	kPacketToLog.m_iSealItemUID = kPacket_.m_iSealItemUID;
	kPacketToLog.m_ucSealCount = ucBeforeSealCount;
	SendToLogDB( ELOG_USE_SEAL_ITEM_NOT, kPacketToLog );
}

IMPL_ON_FUNC( DBE_SEAL_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SEAL_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"봉인 정보 DB업데이트 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// DB업데이트 실패!
	}	

	KEGS_SEAL_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	SendPacket( EGS_SEAL_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_UNSEAL_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_UNSEAL_ITEM_REQ, EGS_UNSEAL_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_UNSEAL_ITEM_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_UNSEAL_ITEM_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_UNSEAL_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_UNSEAL_ITEM_REQ kPacketToDB;

	if( m_kInventory.UnsealItem( kPacket_.m_iDestItemUID, kPacketToDB.m_ucSealResult, kPacketToDB.m_vecUpdatedInventorySlot ) == false )
	{
		KEGS_UNSEAL_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_UNSEAL_ITEM_ACK, kPacketAck );
		return;
	}

	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iItemUID = kPacket_.m_iDestItemUID;
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_UNSEAL_ITEM_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_UNSEAL_ITEM_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
//#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_UNSEAL_ITEM_ACK );
//#else
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"봉인 해제 정보 DB업데이트 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )			
			<< END_LOG;

		// DB업데이트 실패!
	}

	KEGS_UNSEAL_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	SendPacket( EGS_UNSEAL_ITEM_ACK, kPacket );
}
//}}

//{{ 2009. 12. 14  최육사	헤니르임시인벤
IMPL_ON_FUNC_NOPARAM( ERM_CLEAR_TEMP_INVENTORY_NOT )
{
	VERIFY_STATE_WARN( ( 1, KGSFSM::S_ROOM ) );	// [2012-12-07][최육사] 발생 가능한 예외상황이므로 에러로그레벨을 warn으로 낮춤.

	// 임시 인벤토리를 비우자!
	ClearTempInventory();
}
//}}

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

IMPL_ON_FUNC( DBE_RESET_PERIOD_ITEM_ACK )
{
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"기간 초기화 아이템 DB업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_vecItemPeriodInfo.size() )
			<< END_LOG;
		return;
	}

	KEGS_RESET_PERIOD_ITEM_NOT kPacketNot;

	// DB 갔다와서 인벤토리에 기간 연장된거 업데이트 하고 클라로 결과 뿌리자!
	BOOST_TEST_FOREACH( const KItemPeriodInfo&, kInfo, kPacket_.m_vecItemPeriodInfo )
	{
		KInventoryItemInfo kInventorySlotInfo;
		if( m_kInventory.UpdatePeriod( kInfo.m_iItemUID, kInfo.m_sPeriod, kInfo.m_wstrExpirationDate, kInventorySlotInfo ) == true )
		{
			kPacketNot.m_vecInventorySlotInfo.push_back( kInventorySlotInfo );
		}
	}

	if( kPacketNot.m_vecInventorySlotInfo.empty() == false )
	{
		SendPacket( EGS_RESET_PERIOD_ITEM_NOT, kPacketNot );
	}
}

#endif SERV_RESET_PERIOD_EVENT
//}}

//{{ 2011. 01. 04  김민성	속성 부적
// 리팩토링 필요 K_M_S
#ifdef SERV_ATTRIBUTE_CHARM
IMPL_ON_FUNC( EGS_ATTRIB_ATTACH_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ATTRIB_ATTACH_ITEM_REQ, EGS_ATTRIB_ATTACH_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ATTRIB_ATTACH_ITEM_ACK kAck;

		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_ITEM_14;
			SendPacket( EGS_ATTRIB_ATTACH_ITEM_ACK, kAck );
			return;
		}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ATTRIB_ATTACH_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_ATTRIB_ATTACH_ITEM_REQ kPacketReq;
	KEGS_ATTRIB_ATTACH_ITEM_ACK kPacketAck;

	// 부여 아이템ID 얻기
	const int iAttachItemID = m_kInventory.GetItemID( kPacket_.m_iAttachItemUID );

	if( iAttachItemID <= 0 )
	{
		START_LOG( cwarn, L"아이템 정보를 찾지 못했습니다." )
			<< BUILD_LOG( iAttachItemID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_ITEM_04;
		SendPacket( EGS_ATTRIB_ATTACH_ITEM_ACK, kPacketAck );
		return;
	}

	// 목표 아이템의 적용전 강화 레벨 얻기
	KItemAttributeEnchantInfo kBeforeAttribEnchantInfo;
	if( m_kInventory.GetAttribEnchant( kPacket_.m_iDestItemUID, kBeforeAttribEnchantInfo ) == false )
	{
		kPacketAck.m_iOK = NetError::ERR_ATTACH_ITEM_00;
		SendPacket( EGS_ENCHANT_ATTACH_ITEM_ACK, kPacketAck );
		return;
	}

	// 부여 아이템
	if( m_kInventory.AttribAttachItem( kPacket_.m_iDestItemUID, kPacket_.m_iAttachItemUID, kPacketReq.m_kAttribEnchantInfo, kPacketReq.m_vecUpdatedInventorySlot ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ATTRIB_ATTACH_ITEM_ACK, kPacketAck );
		return;
	}

	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iItemUID = kPacket_.m_iDestItemUID;
	m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_ATTRIB_ATTACH_ITEM_REQ, kPacketReq );

	// 로그 기록
	KELOG_ATTRIB_ATTACH_ITEM_LOG_NOT kPacketToLog;
	kPacketToLog.m_iUnitUID					= GetCharUID();
	kPacketToLog.m_iAttachItemID			= iAttachItemID;
	kPacketToLog.m_iDestItemUID				= kPacket_.m_iDestItemUID;
	kPacketToLog.m_kBeforeAttribEnchantInfo = kBeforeAttribEnchantInfo;
	kPacketToLog.m_kAfterAttribEnchantInfo	= kPacketReq.m_kAttribEnchantInfo;
	SendToLogDB( ELOG_ATTRIB_ATTACH_ITEM_LOG_NOT, kPacketToLog );
}

IMPL_ON_FUNC( DBE_ATTRIB_ATTACH_ITEM_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ATTRIB_ATTACH_ITEM_ACK );

	KEGS_ATTRIB_ATTACH_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KItemInfo kInfo;
		if( m_kInventory.GetItemInfo( kPacket_.m_iItemUID, kInfo ) == true )
		{
			const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_iItemID );
			if( pItemTemplet != NULL )
			{
				int iItemLevel = pItemTemplet->m_iItemLevel;
				if( iItemLevel <= 0 )
				{
					iItemLevel = static_cast<int>( GetLevel() );
				}
				m_kUserTitleManager.OnAttribItem( GetThisPtr<KGSUser>(), iItemLevel );
			}
		}
	}
#endif // SERV_ADD_TITLE_CONDITION_2013_08

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 부여 DB업데이트 실패
		START_LOG( cwarn, L"속성 부여 DB 업데이트 실패." )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ODBC_01;
		kPacket.m_vecInventorySlotInfo.clear();
	}

	SendPacket( EGS_ATTRIB_ATTACH_ITEM_ACK, kPacket );
}

#endif SERV_ATTRIBUTE_CHARM
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC( EGS_OUT_USER_RETAINING_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 이탈 유저인지 확인하자
	KDBE_CHECK_RETAINING_USER_REQ kReq;
	kReq.m_iUserUID = GetUID();
	kReq.m_iUnitUID = GetCharUID();
	SendToAccountDB( DBE_CHECK_RETAINING_USER_REQ, kReq );
}

IMPL_ON_FUNC( DBE_CHECK_RETAINING_USER_ACK )
{
	CTime tCurr = CTime::GetCurrentTime();
	CTimeSpan tWeek = CTimeSpan( 7, 0, 0, 0 );
	CTime tReg;

	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrDate, tReg ) == false )
	{
		START_LOG( cerr, L"이탈 방지 보상 지급 시간이 이상합니다" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrDate )
			<< END_LOG;
		tReg = CTime( 0 );
	}
	if( tCurr < tReg + tWeek ) // 이전에 보상 받았던 시간이 7일보다 적다. 주면 안된다.
	{

		START_LOG( clog, L"이탈 방지 보상 이값에서 False 로 변환한다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrDate )
			<< BUILD_LOG( m_kUserInfoByNexon.GetOutUserRetaining() )
			<< END_LOG;

		m_kUserInfoByNexon.SetOutUserRetaining( false );
	}

	KEGS_OUT_USER_RETAINING_ACK kAck;
	kAck.m_bDepartureUser = false;

	if( kPacket_.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"잘못된 UnitUID 값입니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		SendPacket( EGS_OUT_USER_RETAINING_ACK, kAck );
	}

	if( m_kUserInfoByNexon.GetOutUserRetaining() == true )	// 이탈 예상 유저인가?
	{
		kAck.m_bDepartureUser = true;
		//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		SiKEventRewardRetainUserItemRefreshManager()->GetOutUserRetainingRewardItemInfo( kAck.m_mapSlotItem );
#else
		SiKRewardTable()->GetOutUserRetainingRewardItemInfo( kAck.m_mapSlotItem );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		//}}
	}

	SendPacket( EGS_OUT_USER_RETAINING_ACK, kAck );

}

IMPL_ON_FUNC( EGS_RETAINING_SELECT_REWARD_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RETAINING_SELECT_REWARD_REQ, EGS_RETAINING_SELECT_REWARD_ACK );

	KEGS_RETAINING_SELECT_REWARD_ACK kAck;

	kAck.m_iOK = NetError::NET_OK;
	kAck.m_bIsPost = false;

	if( m_kUserInfoByNexon.GetOutUserRetaining() == false )	// 이탈 예상 유저가 아니다.
 	{
 		kAck.m_iOK = NetError::ERR_UNKNOWN;
 
 		START_LOG( cerr, L"이탈 예상 유저가 아닌데 보상을 받으려고 한다." )
 			<< BUILD_LOG( GetUID() )
 			<< BUILD_LOG( GetCharUID() )
 			<< BUILD_LOG( kPacket_.m_iSlotID )
 			<< BUILD_LOG( kAck.m_kRewardItem.m_iItemID )
 			<< END_LOG;
 
 		SendPacket( EGS_RETAINING_SELECT_REWARD_ACK, kAck );
 		return;
 	}

	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	if( SiKEventRewardRetainUserItemRefreshManager()->GetOutUserRetainingSelectItemInfo( kPacket_.m_iSlotID , kAck.m_kRewardItem ) == false ) // 아이템 정보 얻기 실패
#else
	if( SiKRewardTable()->GetOutUserRetainingSelectItemInfo( kPacket_.m_iSlotID , kAck.m_kRewardItem ) == false ) // 아이템 정보 얻기 실패
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	{
		kAck.m_iOK = NetError::ERR_ITEM_04;

		START_LOG( cerr, L"해당 슬롯의 아이템 정보가 없습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iSlotID )
			<< BUILD_LOG( kAck.m_kRewardItem.m_iItemID )
			<< END_LOG;

		SendPacket( EGS_RETAINING_SELECT_REWARD_ACK, kAck );
		return;
	}

	// 부활석 이면
	if( kAck.m_kRewardItem.m_iItemID == CXSLItem::CI_RESURRECTION_STONE )
	{
		m_iNumResurrectionStone += kAck.m_kRewardItem.m_iQuantity;
		kAck.m_iResurrectionStone = m_iNumResurrectionStone;

		m_kUserInfoByNexon.SetOutUserRetaining( false );

		CTime tNow = CTime::GetCurrentTime();
		KDBE_INSERT_RETAINING_USER_REQ kReq;
		kReq.m_iUserUID = GetUID();
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_iItemID = kAck.m_kRewardItem.m_iItemID;
		kReq.m_wstrRegDate = tNow.Format( _T("%Y-%m-%d %H:%M:%S") );

		SendToAccountDB( DBE_INSERT_RETAINING_USER_REQ, kReq );

		SendPacket( EGS_RETAINING_SELECT_REWARD_ACK, kAck );
		return;
	}

	// 인벤토리 검색
	std::map< int, int > mapItem;
	mapItem.insert( std::make_pair( kAck.m_kRewardItem.m_iItemID, kAck.m_kRewardItem.m_iQuantity ) );
	if( m_kInventory.IsEnoughSpaceExist( mapItem ) == false ) // 인벤 검사
	{
		// 공간이 없다.
		// 우편으로 전송
		int iRewardID = 0;
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		switch( kPacket_.m_iSlotID )
		{
		case 1:
			{
				iRewardID = 10220;
			}break;
		case 2:
			{
				iRewardID = 10221;
			}break;
		case 3:
			{
				iRewardID = 10222;
			}break;
		case 4:
			{
				iRewardID = 10223;
			}break;
		case 5:
			{
				iRewardID = 10224;
			}break;
		default:
			{
				START_LOG( cerr, L"슬롯 번호가 이상하다" )
					<< BUILD_LOG( kPacket_.m_iSlotID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_ITEM_04;

				SendPacket( EGS_RETAINING_SELECT_REWARD_ACK, kAck );
				return;
			}
		}
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_OUT_USER_RETANING_REWARD;
		kPacketToDB.m_iRewardID	   = iRewardID;
		kPacketToDB.m_sQuantity	   = 1;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

		kAck.m_bIsPost = true;
		kAck.m_iResurrectionStone = m_iNumResurrectionStone;

		m_kUserInfoByNexon.SetOutUserRetaining( false );

		CTime tNow = CTime::GetCurrentTime();
		KDBE_INSERT_RETAINING_USER_REQ kReq;
		kReq.m_iUserUID = GetUID();
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_iItemID = kAck.m_kRewardItem.m_iItemID;
		kReq.m_wstrRegDate = tNow.Format( _T("%Y-%m-%d %H:%M:%S") );

		SendToAccountDB( DBE_INSERT_RETAINING_USER_REQ, kReq );

		SendPacket( EGS_RETAINING_SELECT_REWARD_ACK, kAck );
		return;
	}
	
	KDBE_RETAINING_SELECT_REWARD_REQ kPacketToDB;
	if( m_kInventory.GiveRetainingRewardItem( kAck.m_kRewardItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo ) == false )
	{
		kAck.m_iOK = NetError::ERR_ITEM_06;

		START_LOG( cerr, L"이탈방지 유저 보상 지급 실패? 인벤 검사하고 지급했는데 왜 지급 실패지? GiveRetainingRewardItem 실패" )
			<< BUILD_LOG( kAck.m_kRewardItem.m_iItemID )
			<< BUILD_LOG( kAck.m_kRewardItem.m_iQuantity )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		SendPacket( EGS_RETAINING_SELECT_REWARD_ACK, kAck );
		return;
	}

	m_kUserInfoByNexon.SetOutUserRetaining( false );

	// 아이템을 생성하기위해 DB로 간다		
	kPacketToDB.m_iUnitUID = GetCharUID();
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	SendToGameDB( DBE_RETAINING_SELECT_REWARD_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_RETAINING_SELECT_REWARD_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RETAINING_SELECT_REWARD_ACK );

	KEGS_RETAINING_SELECT_REWARD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_bIsPost = false;
	kPacket.m_iResurrectionStone = m_iNumResurrectionStone;	

	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	
	// 현재 구현상으로 무조건 보상은 1개이기 때문에 인벤정보는 무조건 size 1이어야함.
	LIF( kPacket.m_vecKInventorySlotInfo.size() == 1 );
	
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInfo, kPacket.m_vecKInventorySlotInfo )
	{
		kPacket.m_kRewardItem.m_iItemID = kInfo.m_kItemInfo.m_iItemID;
		break;
	}
	
	CTime tNow = CTime::GetCurrentTime();

	KDBE_INSERT_RETAINING_USER_REQ kReq;
	kReq.m_iUserUID = GetUID();
	kReq.m_iUnitUID = GetCharUID();
	kReq.m_iItemID = kPacket.m_kRewardItem.m_iItemID;

	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKEventRewardRetainUserItemRefreshManager()->GetOutUserRetainingSelectItemInfo( kPacket.m_kRewardItem.m_iItemID, kPacket.m_kRewardItem.m_iQuantity );
#else
	SiKRewardTable()->GetOutUserRetainingSelectItemInfo( kPacket.m_kRewardItem.m_iItemID, kPacket.m_kRewardItem.m_iQuantity );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	kReq.m_wstrRegDate = tNow.Format( _T("%Y-%m-%d %H:%M:%S") );

	SendToAccountDB( DBE_INSERT_RETAINING_USER_REQ, kReq );
	
	SendPacket( EGS_RETAINING_SELECT_REWARD_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_INSERT_RETAINING_USER_ACK, KDBE_INSERT_RETAINING_USER_REQ )
{
	if( kPacket_.m_iUserUID > 0 )
	{
		KDBE_INSERT_LOG_RETAINING_USER_NOT kNot;
		kNot.m_iUserUID = kPacket_.m_iUserUID;
		kNot.m_iUnitUID = kPacket_.m_iUnitUID;
		kNot.m_iItemID = kPacket_.m_iItemID;
		kNot.m_wstrRegDate = kPacket_.m_wstrRegDate;

		// 로그 기록하자
		SendToLogDB( DBE_INSERT_LOG_RETAINING_USER_NOT, kNot );
	}
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2012. 02. 21	김민성	인벤토리 정렬
#ifdef SERV_SORT_CATEGORY_ITEM 
IMPL_ON_FUNC( EGS_SORT_CATEGORY_ITEM_REQ )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SORT_CATEGORY_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_SORT_CATEGORY_ITEM_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_SORT_CATEGORY_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	if( m_kTimer[TM_SORT_INVENTORY].elapsed() < 3.0 )
	{
		KEGS_SORT_CATEGORY_ITEM_ACK kAck;
		kAck.m_iOK = NetError::ERR_SORT_CATEGORY_01;
		SendPacket( EGS_SORT_CATEGORY_ITEM_ACK, kAck );
		return ;
	}

	m_kTimer[TM_SORT_INVENTORY].restart();

	//{{ 2012. 02. 27	김민성	인벤토리 정렬	- 무거운 기능을 가볍게 변경
#ifdef SERV_SORT_CATEGORY_ITEM_FIX
	m_kTimer[TM_SORT_INVENTORY_USE_TIME].restart();
#endif SERV_SORT_CATEGORY_ITEM_FIX
	//}}

	KEGS_SORT_CATEGORY_ITEM_ACK kAck;
	kAck.m_iCategoryType = kPacket_.m_iCategoryType;

#ifdef SERV_PERSONAL_SHOP_NO_MOVE
	m_kInventory.SortCategory(kPacket_.m_iCategoryType, kAck.m_vecUpdatedInventorySlot, kAck.m_iOK, m_vecPersonalShopSell_ItemInfo);
#else
	m_kInventory.SortCategory(kPacket_.m_iCategoryType, kAck.m_vecUpdatedInventorySlot, kAck.m_iOK);
#endif SERV_PERSONAL_SHOP_NO_MOVE

	//{{ 2012. 02. 27	김민성	인벤토리 정렬	- 무거운 기능을 가볍게 변경
#ifdef SERV_SORT_CATEGORY_ITEM_FIX
	ms_kMaxInventorySortTime.CheckMaxProcessTime( GetName(), ( float )m_kTimer[TM_SORT_INVENTORY_USE_TIME].elapsed() );
#endif SERV_SORT_CATEGORY_ITEM_FIX
	//}}

	SendPacket( EGS_SORT_CATEGORY_ITEM_ACK, kAck );
}
#endif SERV_SORT_CATEGORY_ITEM 
//}}

//{{ 2012. 12. 19  교환(한정 수량) - 김민성
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
IMPL_ON_FUNC( DBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_EXCHANGE_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 교환 수량이 다 되어 교환 불가
		KEGS_ITEM_EXCHANGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ITEM_EXCHANGE_05;
		SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacket );
		return;
	}

	SendToGameDB( DBE_ITEM_EXCHANGE_REQ, kPacket_.m_kDB );

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_ITEM_EXCHANGE, kPacket_.m_kDB.m_mapResultItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
//}}

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
IMPL_ON_FUNC( EGS_RESOLVE_ITEM_AVATAR_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESOLVE_ITEM_AVATAR_REQ, EGS_RESOLVE_ITEM_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	//{{ 2009. 10. 15  최육사	거래 예외처리
	if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
	{
		START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_RESOLVE_ITEM_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_ITEM_14;
		SendPacket( EGS_RESOLVE_ITEM_ACK, kPacket );
		return;
	}
	//}}

		KDBE_RESOLVE_ITEM_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = GetCharUID();


		bool bJackpot = false;		// 대박 검사
		int iED;

	//	if( m_kTimer[TM_RESOLVE_JACKPOT].elapsed() > m_dResolveJackpotTime )
	//	{
	//		bJackpot = true;
	//	}

		if( m_kInventory.ResolveAvatarItem( bJackpot, kPacket_.m_mapResolveDataAvatar , iED , kPacketToDB.m_mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo ) == false )
		{
			LIF( NetError::GetLastError() != NetError::NET_OK );
			KEGS_RESOLVE_ITEM_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::GetLastError();

			SendPacket( EGS_RESOLVE_ITEM_ACK, kPacketAck );
			return;
		}

		const int iBeforeED = m_iED;

		// 소켓 수수료 차감
		m_iED -= iED;


		// 14. ED 사용량 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MItemResolve, m_iED );


		// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MItemResolve, m_iED );
#endif //SERV_USER_STATISTICS_RENEWAL

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
//		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_ITEM_SOCKET, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}

		// 유저 통계
//		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MSocketED, iED );


		// 대박 후처리
		/*if( bJackpot )
		{
			// 대박 로그
			KELOG_RESOLVE_JACKPOT_LOG_NOT kPacketToLog;
			CTime kUsedTime = CTime::GetCurrentTime();
			kPacketToLog.m_UnitUID		  = GetCharUID();
			kPacketToLog.m_iResolveItemID = iResolveItemID;
			kPacketToLog.m_iElapsedTime	  = static_cast<int>( m_kTimer[TM_RESOLVE_JACKPOT].elapsed() );
			kPacketToLog.m_wstrRegDate	  = ( CStringW )( kUsedTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
			SendToLogDB( ELOG_RESOLVE_JACKPOT_LOG_NOT, kPacketToLog );

			// 대박 시간 초기화
			m_kTimer[TM_RESOLVE_JACKPOT].restart();

			// 대박 시간대 새로 얻기
			m_dResolveJackpotTime = SiCXSLResolveItemManager()->GetJackpotTime();
		}*/


	// 분해 획득 아이템 출력
//	std::map< int, int >::iterator mit;
//	for( mit = kPacketToDB.m_mapInsertedItem.begin(); mit != kPacketToDB.m_mapInsertedItem.end(); mit++ )
//	{
//		START_LOG( clog2, L"분해 획득 아이템." )
//			<< BUILD_LOG( mit->first )
//			<< BUILD_LOG( mit->second );
//	}

	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( !kPacketToDB.m_vecItemInfo.empty() )
	{
		//kPacketToDB.m_bJackpot = bJackpot;
		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_RESOLVE_ITEM_REQ, kPacketToDB );
	}
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );
		KEGS_RESOLVE_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK				   = NetError::NET_OK;
		//kPacketAck.m_bJackpot			   = bJackpot;
		kPacketAck.m_mapInsertedItem	   = kPacketToDB.m_mapInsertedItem;
		kPacketAck.m_vecKInventorySlotInfo = kPacketToDB.m_vecUpdatedInventorySlot;

		//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
		kPacketAck.m_iED = GetED();
#endif SERV_MULTI_RESOLVE
		//}}

		SendPacket( EGS_RESOLVE_ITEM_ACK, kPacketAck );	
	}

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	m_kUserQuestManager.Handler_OnResolveItem( GetThisPtr<KGSUser>() );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
	m_kUserTitleManager.OnResolveItem( GetThisPtr<KGSUser>() );
#endif // SERV_ADD_TITLE_CONDITION_2013_08

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_RESOLVE, kPacketToDB.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#endif SERV_MULTI_RESOLVE
//}}

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
IMPL_ON_FUNC_NOPARAM( EGS_SYNTHESIS_SOCKET_GROUPID_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SYNTHESIS_SOCKET_GROUPID_REQ, EGS_SYNTHESIS_SOCKET_GROUPID_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KEGS_SYNTHESIS_SOCKET_GROUPID_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;
	kPacketAck.m_mapAvatarSocketOptionGroupID = SiCXSLSynthesisManager()->GetMapAvatarSocketOptionGroupID();

	SendPacket( EGS_SYNTHESIS_SOCKET_GROUPID_ACK, kPacketAck );
}

IMPL_ON_FUNC( EGS_SYNTHESIS_ITEM_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SYNTHESIS_ITEM_REQ, EGS_SYNTHESIS_ITEM_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	//{{ 2009. 10. 15  최육사	거래 예외처리
	if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
	{
		START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_SYNTHESIS_ITEM_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_ITEM_14;
		SendPacket( EGS_SYNTHESIS_ITEM_ACK, kPacket );
		return;
	}
	//}}

	if(kPacket_.m_mapSynthesisData.size() != 4 || kPacket_.m_iKeyData == 0 || kPacket_.m_iSocketNo == 0 )
	{
		START_LOG( cerr, L"합성 있을수 없는 에러" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_SYNTHESIS_ITEM_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_ITEM_10;
		SendPacket( EGS_SYNTHESIS_ITEM_ACK, kPacket );
		return;
	}

//------------------------------------------------
	KDBE_OPEN_SYNTHESIS_ITEM_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iLevel = GetLevel();
	kPacketToDB.m_mapSynthesisData = kPacket_.m_mapSynthesisData;
	kPacketToDB.m_iKeyData = kPacket_.m_iKeyData;

	if( !m_kInventory.OpenSynthesisItem( kPacket_.m_iKeyData , kPacket_.m_iSocketNo , kPacket_.m_mapSynthesisData,
		kPacketToDB.m_mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo , kPacketToDB.m_setSealCashItem))
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		KEGS_SYNTHESIS_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();

		SendPacket( EGS_SYNTHESIS_ITEM_ACK, kPacketAck );
		return;
	}

//------------------------------------------------

	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( kPacketToDB.m_vecItemInfo.empty() == false )
	{

		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
		SendToGameDB( DBE_OPEN_SYNTHESIS_ITEM_REQ, kPacketToDB );
	}
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );
		KEGS_SYNTHESIS_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK				   = NetError::NET_OK;
		kPacketAck.m_mapInsertedItem	   = kPacketToDB.m_mapInsertedItem;
		kPacketAck.m_vecKInventorySlotInfo = kPacketToDB.m_vecUpdatedInventorySlot;
		SendPacket( EGS_SYNTHESIS_ITEM_ACK, kPacketAck );
	}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
//	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_RANDOM, kPacketToDB.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}

IMPL_ON_FUNC( DBE_OPEN_SYNTHESIS_ITEM_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_SYNTHESIS_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::ERR_RANDOM_ITEM_06 )
	{
		// TODO : DB 에 아이템 삽입 실패. 추후에 로그 추가.

		kPacket.m_iOK = NetError::NET_OK;
	}

	KDBE_SYNTHESIS_LOG_NOT kPacketNot;
	kPacketNot.m_iUnitUID				 = GetCharUID();
	kPacketNot.m_iLevel					 = GetLevel();
	kPacketNot.m_iKeyUID				 = kPacket_.m_iKeyData;
	kPacketNot.m_mapSynthesisData		 = kPacket_.m_mapSynthesisData;
	kPacketNot.m_mapInsertedItemInfo	 = kPacket_.m_mapItemInfo;

	SendToLogDB( DBE_SYNTHESIS_LOG_NOT, kPacketNot );

	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	SendPacket( EGS_SYNTHESIS_ITEM_ACK, kPacket );
}
#endif SERV_SYNTHESIS_AVATAR
//}}

//{{ 지헌 - 은행 개편 퀘스트, 캐쉬 작업 
#ifdef SERV_SHARING_BANK_QUEST_CASH
IMPL_ON_FUNC( DBE_SHARING_BACK_OPEN_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), DBE_SHARING_BACK_OPEN_ACK );

	KEGS_SHARING_BACK_OPEN_NOT kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 은행 공유 실패
		START_LOG( cerr, L"계정 은행 뚫기 실패!" )
			<< BUILD_LOG(kPacket_.m_iOK)
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ODBC_01;
	}
#ifdef SERV_SHARING_BANK_EVENT
	else
	{
		// 이벤트 보상을 주자!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDB.m_iRewardID	   = _CONST_SHARING_BANK_EVENT_ITEM_::iRewardOpenBankSharing;	// 휴대용 갈철 금고, 
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
#endif SERV_SHARING_BANK_EVENT
	kPacket.m_iOpenType = kPacket_.m_iOpenType;
	SendPacket( EGS_SHARING_BACK_OPEN_NOT, kPacket );
}
#endif
//}}

#ifdef SERV_SHARING_BANK_TEST
IMPL_ON_FUNC( EGS_GET_SHARE_BANK_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_SHARE_BANK_REQ, EGS_GET_SHARE_BANK_ACK );

	KDBE_UPDATE_SHARE_ITEM_REQ kPacketSendDB;

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KEGS_GET_SHARE_BANK_ACK kAck;
		kAck.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_GET_SHARE_BANK_ACK, kAck );
		return;
	}

	kPacket_.m_iNewUnitTradeBlockDay		= SiKGameSysVal()->GetNewUnitTradeBlockDay();
	kPacket_.m_iNewUnitTradeBlockUnitClass	= SiKGameSysVal()->GetNewUnitTradeBlockUnitClass();

	if( SiKGameSysVal()->IsNewUnitTradeBlock() == false )
	{
		kPacket_.m_iNewUnitTradeBlockDay = 0;
		kPacket_.m_iNewUnitTradeBlockUnitClass = 0;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	//	즉시 업데이트 해야할 품목을 받아온다 -- 실시간 업데이트로 바꿨기 때문에 여기서 DB업데이트 처리할 필요가 없다!
//	if( m_kInventory.GetShareItemNeedDBUpdate( kPacketSendDB ) == true )
//	{
//		kPacketSendDB.m_bReload = true;
//
//#ifdef SERV_NEW_UNIT_TRADE_LIMIT
//		kPacketSendDB.m_wstrReloadNickname = kPacket_.m_wstrNickName;
//#else // SERV_NEW_UNIT_TRADE_LIMIT
//		kPacketSendDB.m_wstrReloadNickname = kPacket_;
//#endif // SERV_NEW_UNIT_TRADE_LIMIT
//
//		SendToGameDB( DBE_UPDATE_SHARE_ITEM_REQ, kPacketSendDB );
//		return;
//	}
//	else
	{
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		if( m_kInventory.GetShareNickname() == kPacket_.m_wstrNickName )
#else // SERV_NEW_UNIT_TRADE_LIMIT
		if( m_kInventory.GetShareNickname() == kPacket_ )
#endif // SERV_NEW_UNIT_TRADE_LIMIT
		{
			KEGS_GET_SHARE_BANK_ACK kAck;
			kAck.m_iOK = NetError::NET_OK;
			kAck.m_iBankSize = m_kInventory.GetShareBankSize();
			kAck.m_iUnitUID = m_kInventory.GetShareUnitUID();

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
			kAck.m_wstrNickName = kPacket_.m_wstrNickName;
#else // SERV_NEW_UNIT_TRADE_LIMIT
			kAck.m_wstrNickName = kPacket_;
#endif // SERV_NEW_UNIT_TRADE_LIMIT

			m_kInventory.GetShareItemMap( kAck.m_mapItem );

			SendPacket( EGS_GET_SHARE_BANK_ACK, kAck );
			return;
		}
		else
		{
			m_kInventory.ClearShareBank();

			//	초기화 하고 다시 보내준다.
			SendToGameDB( DBE_GET_SHARE_BANK_REQ, kPacket_ );
		}
	}
}

_IMPL_ON_FUNC( DBE_GET_SHARE_BANK_ACK, KEGS_GET_SHARE_BANK_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_SHARE_BANK_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
#ifdef SERV_SHARE_BANK_ITEM_EVALUATE_FIX
		std::map< UidType, KInventoryItemInfo >::iterator mit;
		for( mit = kPacket_.m_mapItem.begin(); mit != kPacket_.m_mapItem.end(); ++mit )
		{
			const UidType iItemUID = mit->first;
			KInventoryItemInfo& kInventoryItemInfo = mit->second;

			m_kInventory.InsertShareItem( iItemUID, kInventoryItemInfo.m_kItemInfo, kInventoryItemInfo.m_cSlotCategory, kInventoryItemInfo.m_sSlotID );

			// 여기서 해당 아이템의 itemstate값을 얻어서 클라이언트로 보내는 패킷에도 반영해주자!
			kInventoryItemInfo.m_kItemInfo.m_cItemState = m_kInventory.GetItemState( iItemUID );
		}
#else //SERV_SHARE_BANK_ITEM_EVALUATE_FIX
		std::map< UidType, KInventoryItemInfo >::const_iterator mit;
		for( mit = kPacket_.m_mapItem.begin(); mit != kPacket_.m_mapItem.end(); ++mit )
		{
			const UidType iItemUID = mit->first;
			const KInventoryItemInfo& kInventoryItemInfo = mit->second;

			m_kInventory.InsertShareItem( iItemUID, kInventoryItemInfo.m_kItemInfo, kInventoryItemInfo.m_cSlotCategory, kInventoryItemInfo.m_sSlotID );
		}
#endif //SERV_SHARE_BANK_ITEM_EVALUATE_FIX

		m_kInventory.SetShareBankSize( kPacket_.m_iBankSize );
		m_kInventory.SetShareUnitUID( kPacket_.m_iUnitUID );
		m_kInventory.SetShareNickname( kPacket_.m_wstrNickName );
	}

	SendPacket( EGS_GET_SHARE_BANK_ACK, kPacket_ );

#ifdef	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
	KEGS_SHARE_BANK_UPDATE_OK_NOT kNot;
	kNot.m_iOK = NetError::NET_OK;
	SendPacket( EGS_SHARE_BANK_UPDATE_OK_NOT, kNot );
#endif	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
}

IMPL_ON_FUNC( DBE_UPDATE_SHARE_ITEM_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( kPacket_.m_vecTradeShareItemResult.empty() == false )
		{
			KEGS_UPDATE_ITEM_POSITION_NOT kNot;

			BOOST_TEST_FOREACH( const KTradeShareItemInfo&, kInfo, kPacket_.m_vecTradeShareItemResult )
			{
				KTradeShareItemResult kResult;
				kResult.m_iBeforeItemUID = kInfo.m_iItemUID;
				kResult.m_iNewItemUID = kInfo.m_iNewItemUID;
				kResult.m_wstrRegDate = kInfo.m_wstrRegDate;

				if( m_kInventory.UpdateShareItemPosition( kResult ) == true )
				{
					kNot.m_vecShareItemResult.push_back( kResult );
				}
			}

			SendPacket( EGS_UPDATE_ITEM_POSITION_NOT, kNot );

			//	로그 남기기
			SendToLogDB( ELOG_UPDATE_SHARE_ITEM_NOT, kPacket_.m_vecTradeShareItemResult );
		}

		m_kInventory.ClearShareBank();

		// 공유 은행 정보를 다시 불러온다.
		if( kPacket_.m_bReload == true )
		{
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
			KEGS_GET_SHARE_BANK_REQ kPacket;
			kPacket.m_wstrNickName = kPacket_.m_wstrReloadNickname;
			kPacket.m_iNewUnitTradeBlockDay		= SiKGameSysVal()->GetNewUnitTradeBlockDay();
			kPacket.m_iNewUnitTradeBlockUnitClass	= SiKGameSysVal()->GetNewUnitTradeBlockUnitClass();

			if( SiKGameSysVal()->IsNewUnitTradeBlock() == false )
			{
				kPacket.m_iNewUnitTradeBlockDay = 0;
				kPacket.m_iNewUnitTradeBlockUnitClass = 0;
			}

			SendToGameDB( DBE_GET_SHARE_BANK_REQ, kPacket );
#else // SERV_NEW_UNIT_TRADE_LIMIT
			SendToGameDB( DBE_GET_SHARE_BANK_REQ, kPacket_.m_wstrReloadNickname );
#endif // SERV_NEW_UNIT_TRADE_LIMIT
		}
#ifdef	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
		else
		{
			KEGS_SHARE_BANK_UPDATE_OK_NOT kNot;
			kNot.m_iOK = NetError::NET_OK;
			SendPacket( EGS_SHARE_BANK_UPDATE_OK_NOT, kNot );
		}
#endif	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
	}
}

_IMPL_ON_FUNC( DBE_UPDATE_SHARE_ITEM_FOR_MOVE_SLOT_ACK, KDBE_UPDATE_SHARE_ITEM_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 일단 성공했든 실패했든 원래 전송했어야할 정보를 전달한다.
	KEGS_CHANGE_INVENTORY_SLOT_ITEM_ACK kPacket;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotItem;
	SendPacket( EGS_CHANGE_INVENTORY_SLOT_ITEM_ACK, kPacket );

	// 장착 아이템 바꿨으면 다른 유저들에게 알려준다.
	SendUpdateChangeEquippedItem( kPacket.m_vecInventorySlotInfo );
    
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//////////////////////////////////////////////////////////////////////////
		//START_LOG( cout, "[테스트로그] 공유은행 DB업데이트 결과를 인벤토리에 업데이트 합니다!" )
		//	<< BUILD_LOG( GetCharName() )
		//	<< BUILD_LOG( kPacket_.m_vecTradeShareItemResult.size() );
		//////////////////////////////////////////////////////////////////////////

		// 공유은행과 인벤토리간의 이동이 일어났다면
		if( kPacket_.m_vecTradeShareItemResult.empty() == false )
		{
			KEGS_UPDATE_ITEM_POSITION_NOT kNot;

			BOOST_TEST_FOREACH( const KTradeShareItemInfo&, kInfo, kPacket_.m_vecTradeShareItemResult )
			{
				KTradeShareItemResult kResult;
				kResult.m_iBeforeItemUID = kInfo.m_iItemUID;
				kResult.m_iNewItemUID = kInfo.m_iNewItemUID;
				kResult.m_wstrRegDate = kInfo.m_wstrRegDate;

				if( m_kInventory.UpdateShareItemPosition( kResult ) == true )
				{
					kNot.m_vecShareItemResult.push_back( kResult );
				}
			}

			SendPacket( EGS_UPDATE_ITEM_POSITION_NOT, kNot );

			//	로그 남기기
			SendToLogDB( ELOG_UPDATE_SHARE_ITEM_NOT, kPacket_.m_vecTradeShareItemResult );
		}

		KEGS_SHARE_BANK_UPDATE_OK_NOT kNot;
		kNot.m_iOK = NetError::NET_OK;
		SendPacket( EGS_SHARE_BANK_UPDATE_OK_NOT, kNot );
	}
}

//IMPL_ON_FUNC_NOPARAM( EGS_CLOSE_BANK_NOT )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
//
//	KDBE_UPDATE_SHARE_ITEM_REQ kPacketSendDB;
//
//	//	은행공유 기록
//	if( true == m_kInventory.GetShareItemNeedDBUpdate( kPacketSendDB ) )
//	{
//		kPacketSendDB.m_bReload = false;
//		SendToGameDB( DBE_UPDATE_SHARE_ITEM_REQ, kPacketSendDB );
//	}
//	else
//	{
//		m_kInventory.ClearShareBank();
//
//#ifdef	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
//		KEGS_SHARE_BANK_UPDATE_OK_NOT kNot;
//		kNot.m_iOK = NetError::NET_OK;
//		SendPacket( EGS_SHARE_BANK_UPDATE_OK_NOT, kNot );
//#endif	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
//	}
//}
#endif SERV_SHARING_BANK_TEST
#ifdef SERV_SHARING_BANK_EVENT
IMPL_ON_FUNC_NOPARAM( DBE_SHARING_BANK_EVENT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// 이제 메일을 쏴 주자

	// 이벤트 보상을 주자!
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_iToUnitUID   = GetCharUID();
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
	kPacketToDB.m_iRewardID	   = _CONST_SHARING_BANK_EVENT_ITEM_::iRewardUseBankSharing;	// 루리엘 고객감동 큐브 
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

}
#endif

#ifdef SERV_GOLD_TICKET
IMPL_ON_FUNC( EPUBLISHER_CHARGE_POINT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	START_LOG(clog2,L"지헌로그-테스트:여기까지 오긴 오나")
		<< END_LOG;
	// 골드 티켓 사용 하는데 문제가 생겼는지 응답을 보내야 할 까?
	KEGS_CHARGE_POINT_NOT kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iChargedCash = kPacket_.m_iChargedCash;

	SendPacket( EGS_CHARGE_POINT_NOT, kPacket);	
}
#endif //SERV_GOLD_TICKET

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
IMPL_ON_FUNC( EGS_OPEN_RANDOM_ITEM_DEVELOPER_REQ )
{

	if( GetAuthLevel() < SEnum::UAL_DEVELOPER )
	{
		return;
	}


	KEGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK kPacket;

	std::map< int, int > m_mapInsertedItem;
	int iRessurectionCount = 0;
	int iRestoreSpirit = 0;
	bool bCharmItem = false;
	bool bIsSealItem = false;
	bool bIsNotifyMsgAttration = false;

#ifdef SERV_CUBE_IN_ITEM_MAPPING
	std::map<int, RANDOMITEM_MAPPING_TIME>::iterator mitRITime = SiCXSLRandomItemManager()->m_mapRandomItemMappingTime.begin();
	CTime tCurr = CTime::GetCurrentTime();

	KEGS_CUBE_IN_ITEM_MAPPING_NOT kPacketCubeInItemMapping;
	std::map<int, std::map<int, kRandomItemMappingToInfoClient> > mapNowMapping;
	mapNowMapping.clear();

	for(; mitRITime != SiCXSLRandomItemManager()->m_mapRandomItemMappingTime.end(); mitRITime++)
	{
		// 시작시간과 종료시간 안이면, 바꿔줌.
		int nIndexData = mitRITime->first;
		std::map<int, RANDOMITEM_MAPPING_DATA>::iterator mitRIData = SiCXSLRandomItemManager()->m_mapRandomItemMappingData.find(nIndexData);
		//예외처리
		if(mitRIData == SiCXSLRandomItemManager()->m_mapRandomItemMappingData.end())
			continue;

		CTime tStartTime = mitRITime->second.tStartTime;
		CTime tEndTime = mitRITime->second.tEndTime;

		// 매핑 여부 확인해주자
		kRandomItemMappingToInfoClient kRandomToInfoClient;
		kRandomToInfoClient.m_bCheckMapping  = false;
		if(  tStartTime < tCurr && tEndTime > tCurr )
		{

			START_LOG( clog, L"ModifyMapItemGroup, 큐브매핑작업Start" )
				<< BUILD_LOG( nIndexData )
				<< BUILD_LOG( tStartTime.GetYear() )
				<< BUILD_LOG( tStartTime.GetMonth() )
				<< BUILD_LOG( tStartTime.GetDay() )
				<< BUILD_LOG( tStartTime.GetHour() )
				<< BUILD_LOG( tStartTime.GetMinute() )
				<< BUILD_LOG( tEndTime.GetYear() )
				<< BUILD_LOG( tEndTime.GetMonth() )
				<< BUILD_LOG( tEndTime.GetDay() )
				<< BUILD_LOG( tEndTime.GetHour() )
				<< BUILD_LOG( tEndTime.GetMinute() )
				<< END_LOG;

			if(!SiCXSLRandomItemManager()->ModifyMapItemGroup(mitRIData->second))
			{
				START_LOG( cerr, L"해당 랜덤 아이템에 대한 키템 정보가 없다. 있을수 없는에러!" )
					<< BUILD_LOG( mitRIData->second.iCubeID )
					<< BUILD_LOG( mitRIData->second.iKeyItemID )
					<< END_LOG;
			}
			else
			{
				kRandomToInfoClient.m_bCheckMapping = true;
			}
		}
		// 매핑 정보를 담아서 넘겨준다. 
		int iCubeID = mitRIData->second.iCubeID;
		kRandomToInfoClient.m_iBeforeGroupID = mitRIData->second.iBeforeGroupID;
		kRandomToInfoClient.m_iAfterGroupID = mitRIData->second.iAfterGroupID;
		kRandomToInfoClient.m_tStartTime = (CStringW)(tStartTime.Format(_T("%Y-%m-%d %H:%M:%S")));
		kRandomToInfoClient.m_tEndTime = (CStringW)(tEndTime.Format(_T("%Y-%m-%d %H:%M:%S")));

		std::map<int, kRandomItemMappingToInfoClient> mapToInfoClient;

		// 벡터에서 시간을 가져온다.
		std::map<int, std::map<int,kRandomItemMappingToInfoClient> >::iterator mitToInfoC = mapNowMapping.find(iCubeID);
		if(mitToInfoC == mapNowMapping.end())
		{
			mapToInfoClient.insert(std::make_pair(nIndexData, kRandomToInfoClient));
			mapNowMapping.insert(std::make_pair(iCubeID,mapToInfoClient));
		} 
		else // 이미 존재하고 있는 아이템이라면
		{
			mitToInfoC->second.insert(std::make_pair(nIndexData, kRandomToInfoClient));
		}
	}
	kPacketCubeInItemMapping.m_mapCubeInItemMapping = mapNowMapping;
	SendPacket( EGS_CUBE_IN_ITEM_MAPPING_NOT, kPacketCubeInItemMapping );
#endif SERV_CUBE_IN_ITEM_MAPPING

	std::map<int, KRandomItemResult> m_map_RandomItemResult;

	for ( int i = 0; i < kPacket_.m_iOpenCount; i++ )
	{

		if (m_kInventory.OpenRandomItem_DEV( GetUnitClass(), 
			kPacket_.m_iItemID,
			kPacket_.m_iItemKeyID,
			m_mapInsertedItem,
			iRessurectionCount,
			iRestoreSpirit,
			bCharmItem
			) == false	)
		{
			break;
		}
		else
		{
			if ( m_mapInsertedItem.empty() == false)
			{
				std::map< int, int >::iterator mitInsert = m_mapInsertedItem.begin();


				for (;mitInsert != m_mapInsertedItem.end(); ++mitInsert )
				{

					bIsSealItem = false;
					bIsNotifyMsgAttration = false;

					std::map<int, KRandomItemResult>::iterator mit = m_map_RandomItemResult.find(mitInsert->first);

					if( SiCXSLRandomItemManager()->IsSealRandomItemCube( kPacket_.m_iItemID ) == true )
					{
						if( SiCXSLItemManager()->IsCanSealForRandomItem( mitInsert->first ) == true )
						{
							bIsSealItem = true;
						}
					}

					if( SiCXSLRandomItemManager()->IsAttrationItem( mitInsert->first ) == true )
					{

						std::wstring tmpMSG;
						if( SiCXSLRandomItemManager()->MakeNotifyMsgAttrationItem(  mitInsert->first, GetCharName(), tmpMSG ) == true )
						{
							bIsNotifyMsgAttration = true;
						}
					}

					if ( mit == m_map_RandomItemResult.end()  )
					{
						KRandomItemResult kResult;
						kResult.m_bIsSealItem = bIsSealItem;
						kResult.m_bIsNotifyMsgAttration = bIsNotifyMsgAttration;
						kResult.m_iItemCount = mitInsert->second;
						kResult.m_dItemTotalCount = 1;
						kResult.m_dRate = static_cast<double>((kResult.m_dItemTotalCount / kPacket_.m_iOpenCount) * 100.0f);
						m_map_RandomItemResult.insert(std::make_pair(mitInsert->first,kResult ));

					}
					else
					{
						mit->second.m_bIsSealItem = bIsSealItem;
						mit->second.m_bIsNotifyMsgAttration = bIsNotifyMsgAttration;
						mit->second.m_dItemTotalCount++;
						mit->second.m_dRate = static_cast<double>(( mit->second.m_dItemTotalCount / kPacket_.m_iOpenCount) * 100.f);
						mit->second.m_iItemCount = mitInsert->second;
					}

				}

			}	

		}
	}

	kPacket.m_iOK = NetError::GetLastError();
	kPacket.m_map_RandomItemResult = m_map_RandomItemResult;
	SendPacket( EGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK, kPacket );
}
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
IMPL_ON_FUNC( EGS_ITEM_EVALUATE_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_EVALUATE_REQ, EGS_ITEM_EVALUATE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ITEM_EVALUATE_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_ITEM_EVALUATE_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ITEM_EVALUATE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_ITEM_EVALUATE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iItemUID = kPacket_.m_iItemUID;

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
	// ED 어뷰저 체크
	const int iBeforeED = m_iED;
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	// 아이템 감정
	if( m_kInventory.ItemEvaluateCheck( kPacket_.m_iItemUID, kPacketToDB.m_vecRandomSocket, kPacketToDB.m_iCost ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );

		KEGS_ITEM_EVALUATE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ITEM_EVALUATE_ACK, kPacketAck );
		return;
	}

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
	// ED 어뷰저 체크
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_ITEM_EVALUATE_COST, iBeforeED );
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
		
	SendToGameDB( DBE_ITEM_EVALUATE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ITEM_EVALUATE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_EVALUATE_ACK );

	KEGS_ITEM_EVALUATE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;	

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, "감정 결과 업데이트 실패! DB오류 인가?" )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iCost )
			<< BUILD_LOG( kPacket_.m_vecRandomSocket.size() )
			<< END_LOG;
		goto end_proc;
	}
	
	// DB업데이트가 성공했다면 인벤토리에 해당 아이템에도 정보를 업데이트 하자!
	if( m_kInventory.ItemEvaluateResult( kPacket_.m_iItemUID, kPacket_.m_vecRandomSocket, kPacket.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, "감정 결과 업데이트 실패! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iCost )
			<< BUILD_LOG( kPacket_.m_vecRandomSocket.size() )
			<< END_LOG;
		goto end_proc;
	}

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
	// ED 사용량 통계
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MItemEvaluate, kPacket_.m_iCost );
	}

	// 유저 통계
	m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MItemEvaluate, kPacket_.m_iCost );
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	kPacket.m_iED = GetED();
	SendPacket( EGS_ITEM_EVALUATE_ACK, kPacket );
	return;

end_proc:
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
	// ED 어뷰저 체크
	const int iBeforeED = m_iED;
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	// 수수료 롤백
	m_iED += kPacket_.m_iCost;

//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
	// ED 어뷰저 체크
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_ITEM_EVALUATE_COST_ROLLBACK, iBeforeED );
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	kPacket.m_iED = GetED();
	SendPacket( EGS_ITEM_EVALUATE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_RESTORE_ITEM_EVALUATE_REQ )
{	
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESTORE_ITEM_EVALUATE_REQ, EGS_RESTORE_ITEM_EVALUATE_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_RESTORE_ITEM_EVALUATE_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_RESTORE_ITEM_EVALUATE_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_RESTORE_ITEM_EVALUATE_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KDBE_RESTORE_ITEM_EVALUATE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iTargetItemUID = kPacket_.m_iTargetItemUID;

	// 아이템 미감정
	if( m_kInventory.RestoreItemEvaluateCheck( kPacket_.m_iSupportItemUID, kPacket_.m_iTargetItemUID, kPacketToDB.m_vecUpdatedInventorySlot ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );

		KEGS_RESTORE_ITEM_EVALUATE_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_RESTORE_ITEM_EVALUATE_ACK, kPacketAck );
		return;
	}

	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_RESTORE_ITEM_EVALUATE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_RESTORE_ITEM_EVALUATE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESTORE_ITEM_EVALUATE_ACK );

	KEGS_RESTORE_ITEM_EVALUATE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;

	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, "미감정 결과 업데이트 실패! DB오류 인가?" )
			<< BUILD_LOG( kPacket_.m_iTargetItemUID )
			<< END_LOG;
		goto end_proc;
	}

	// DB업데이트가 성공했다면 인벤토리에 해당 아이템에도 정보를 업데이트 하자!
	if( m_kInventory.RestoreItemEvaluateResult( kPacket_.m_iTargetItemUID, kPacket.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, "미감정 결과 업데이트 실패! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kPacket_.m_iTargetItemUID )
			<< BUILD_LOG( kPacket.m_vecInventorySlotInfo.size() )
			<< END_LOG;
	}
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05_EVENT
	else
	{
		// 이벤트 보상을 주자!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
#ifdef SERV_COUNTRY_US
		kPacketToDB.m_iRewardID	   = 51438;	// 아이템 감정 증명서
#else //SERV_COUNTRY_US
		kPacketToDB.m_iRewardID	   = 20260;	// 아이템 감정 증명서
#endif //SERV_COUNTRY_US
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
#endif SERV_NEW_ITEM_SYSTEM_2013_05_EVENT

end_proc:
	SendPacket( EGS_RESTORE_ITEM_EVALUATE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_ITEM_CONVERT_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_CONVERT_REQ, EGS_ITEM_CONVERT_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_ITEM_CONVERT_ACK kAck;

		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK	= NetError::ERR_ITEM_14;
			SendPacket( EGS_ITEM_CONVERT_ACK, kAck );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ITEM_CONVERT_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	// 아이템 전환
	KDBE_ITEM_CONVERT_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();

	// 아이템 미감정
	std::map< int, int > mapInsertedItem;
	if( m_kInventory.ItemConvert( kPacket_.m_iItemUID, kPacket_.m_iQuantity, mapInsertedItem, kPacketToDB.m_vecItemInfo, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_iCommissionED ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );

		KEGS_ITEM_CONVERT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_ITEM_CONVERT_ACK, kPacketAck );
		return;
	}

	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_ITEM_CONVERT_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ITEM_CONVERT_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_CONVERT_ACK );

	KEGS_ITEM_CONVERT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05_EVENT
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 이벤트 보상을 주자!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
#ifdef SERV_COUNTRY_US
		kPacketToDB.m_iRewardID	   = 51439;	// 신아이템 체험 증명서
#else //SERV_COUNTRY_US
		kPacketToDB.m_iRewardID	   = 20261;	// 신아이템 체험 증명서
#endif //SERV_COUNTRY_US
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}
#endif SERV_NEW_ITEM_SYSTEM_2013_05_EVENT

	if( kPacket_.m_iOK == NetError::ERR_RANDOM_ITEM_06 )
	{
		// TODO : DB 에 아이템 삽입 실패. 추후에 로그 추가.

		kPacket.m_iOK = NetError::NET_OK;
	}

	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	//const int iBeforeED = m_iED;
	//m_iED -= kPacket_.m_iSpendED;
	//m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_CUBE_OPEN_ED_COST, iBeforeED );}

	kPacket.m_iED = GetED();

	// 유저 통계
	//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_OpenCubeSpendED, kPacket_.m_iSpendED );

	// ED 사용량 통계
	//KStatisticsKey kKey;
	//kKey.m_vecIntKey.push_back( 0 );
	//KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_POpenCube, kPacket_.m_iSpendED );

	SendPacket( EGS_ITEM_CONVERT_ACK, kPacket );
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_CUBE_NOT )
{
	KOpenRandomItemStorage kStorage;
	if( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, kStorage ) != KAfterWorkStorage::AWS_SUCCEED )
	{
		START_LOG( cerr, L"AfterWorkStorage: 꺼내오기 실패" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		
		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;	// 랜덤 큐브 오픈 실패( AfterWorkStorage 에서 데이터를 꺼내오지 못하였습니다. )
		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
		return;
	}

	int& iRessurectionCount	= kStorage.m_iRessurectionCount;
	int& iRestoreSpirit		= kStorage.m_iRestoreSpirit;
	const int& iRandomItemID		= kStorage.m_iRandomItemID;
	const bool& bIsCharmItem		= kStorage.m_bIsCharmItem;

	KDBE_OPEN_RANDOM_ITEM_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID					= GetCharUID();
	kPacketToDB.m_iSpendED					= kStorage.m_iSpendED;
	kPacketToDB.m_mapInsertedItem			= kStorage.m_mapInsertedItem;
	kPacketToDB.m_vecUpdatedInventorySlot	= kStorage.m_vecUpdatedInventorySlot;
	kPacketToDB.m_vecItemInfo				= kStorage.m_vecItemInfo;

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	BOOST_TEST_FOREACH( KItemInfo&, kGetItemInfo, kPacketToDB.m_vecItemInfo )
	{
		// 고정 강화 레벨 적용
		bool isEnchantCube = false;
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kGetItemInfo.m_iItemID );
		if( pItemTemplet != NULL )
		{
			if( pItemTemplet->m_ItemType == CXSLItem::IT_WEAPON  || pItemTemplet->m_ItemType == CXSLItem::IT_DEFENCE )
			{
				// 고정 강화 큐브인가?
				if( CXSLItem::UpdateEnchantedItemCubeReward( iRandomItemID, kGetItemInfo ) == true )
				{
					isEnchantCube = true;
				}
			}
		}

		if( isEnchantCube == false )
		{
			// 랜덤 강화 레벨 적용
			SiCXSLEnchantItemManager()->RandomEnchant( kGetItemInfo.m_iItemID
				, CXSLEnchantItemManager::RET_CUBE
				, kPacket_.m_cTimeEnchantEventLevel
				, kGetItemInfo.m_cEnchantLevel
				);
		}

		// 9강 이상인 경우 전체 공지
		SendEnchantNotice( kGetItemInfo.m_iItemID, kGetItemInfo.m_cEnchantLevel, kStorage.m_iRandomItemID );

		// 특정 등급의 아이템인 경우 밀봉 처리
		SiCXSLItemManager()->ItemSealProcess( kGetItemInfo.m_iItemID
			, kGetItemInfo.m_sPeriod
			, kGetItemInfo.m_ucSealData
			);
	}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2008. 12. 30  최육사	매력아이템 로그
	if( bIsCharmItem )
	{		
		//{{ 2009. 6. 22  최육사	매력아이템 공지
		BOOST_MAP_CONST_FOREACH( int, int, kGetItem, kPacketToDB.m_mapInsertedItem )
		{
			// 공지 날릴만한 아이템이면 날리자!
			if( SiCXSLRandomItemManager()->IsAttrationItem( kGetItem.first ) == true )
			{
				KEGS_NOTIFY_MSG_NOT	kPacket;
				if( SiCXSLRandomItemManager()->MakeNotifyMsgAttrationItem( kGetItem.first, GetCharName(), kPacket.m_wstrMSG ) == true )
				{
					kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM; // 아이템 획득 공지
					kPacket.m_Count = 1;
					//{{ 아이템 획득 공지에 큐브 이름 출력 - 김민성
#ifdef SERV_GET_ITEM_NOTIFY
					kPacket.m_iRandomItem = iRandomItemID;
#endif SERV_GET_ITEM_NOTIFY
					//}}
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
					if( true == kPacket_.m_bNotifyGetItem )
						SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
					else
						SendPacket( EGS_NOTIFY_MSG_NOT, kPacket );
#else // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
					SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
				}
			}

			// 매력 아이템의 결과물에 대한 로그 기록!
			KELOG_CHARM_ITEM_LOG_NOT kCharmLog;
			kCharmLog.m_iUnitUID	  = GetCharUID();
			kCharmLog.m_iCharmItemID  = iRandomItemID;
			kCharmLog.m_iResultItemID = kGetItem.first;
			kCharmLog.m_iQuantity	  = kGetItem.second;
			SendToLogDB( ELOG_CHARM_ITEM_LOG_NOT, kCharmLog );
		}
	}
	//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
	else
	{
		// 봉인된 아이템이 나오는 랜덤 아이템 큐브인지 확인!
		if( SiCXSLRandomItemManager()->IsSealRandomItemCube( iRandomItemID ) == true )
		{
			BOOST_MAP_CONST_FOREACH( int, int, kGetItem, kPacketToDB.m_mapInsertedItem )
			{
				// 공지 날릴만한 아이템이면 날리자!
				if( SiCXSLRandomItemManager()->IsNotifyResultItem( kGetItem.first ) == true )
				{
					KEGS_NOTIFY_MSG_NOT	kPacket;
					if( SiCXSLRandomItemManager()->MakeNotifyMsgRandomResultItem( kGetItem.first, GetCharName(), kPacket.m_wstrMSG ) == true )
					{
						kPacket.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_RANDOM_GET_ITEM; // 아이템 획득 공지
						kPacket.m_Count = 1;
						//{{ 아이템 획득 공지에 큐브 이름 출력 - 김민성
#ifdef SERV_GET_ITEM_NOTIFY
						kPacket.m_iRandomItem = iRandomItemID;
#endif SERV_GET_ITEM_NOTIFY
						//}}
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
						if( true == kPacket_.m_bNotifyGetItem )
							SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
						else
							SendPacket( EGS_NOTIFY_MSG_NOT, kPacket );
#else // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
						SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket );
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
					}
				}
			}
		}		
	}
#endif SERV_SEALED_RANDOM_ITEM
	//}}
	//}}

	//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	bool bResurrectionStone = false;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	//}}

	// 부활석 충전 큐브
	if( iRessurectionCount > 0 )
	{
		// 부활석 충전
		m_iNumResurrectionStone += iRessurectionCount;
		iRessurectionCount = m_iNumResurrectionStone;
		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		bResurrectionStone = true;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}
	}

	// 근성도 회복 큐브
	if( iRestoreSpirit > 0 )
	{
		//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		iRestoreSpirit = m_kUserSpiritManager.RestoreSpirit( iRestoreSpirit );
#else
		// 근성도 최대치 검사
		if( m_iSpiritMax < ( m_iSpirit + iRestoreSpirit ) )
		{
			m_iSpirit = m_iSpiritMax;
		}
		else
		{
			m_iSpirit += iRestoreSpirit;
		}
		iRestoreSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}		

		// 1.채널에 속해있는지 검사.
		if( GetRoomUID() != 0 )
		{
			// 2.채널이라면 대전/던전 방인지 검사.
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_DUNGEON:
				{
					KRoomUserInfo kInfo;
					GetRoomUserInfo( kInfo );

					// 3.조건이 만족한다면 현재속한 방에 개인정보를 업데이트 한다.
					if( GetRoomUID() != 0 )
					{
						SendToCnRoom( ERM_SET_UNIT_INFO_NOT, kInfo );

						// 3.1 채워진 근성도를 방의 다른 유저들에게 전송한다.
						KEGS_RESTORE_SPIRIT_NOT kNot;
						kNot.m_iUnitUID = GetCharUID();
						//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
						kNot.m_iSpirit = m_kUserSpiritManager.GetSpirit();
#else
						kNot.m_iSpirit = m_iSpirit;
#endif SERV_BATTLE_FIELD_SYSTEM
						//}}						
						SendToCnRoom( ERM_RESTORE_SPIRIT_NOT, kNot );
					}
				}
				break;
			}			
		}
	}

	//{{ 2011. 12. 19	최육사	랜덤큐브 DB실시간 적용
#ifdef SERV_OPEN_CUBE_REALTIME_DB_UPDATE
#else
	// 새로 추가된 아이템이 있으면 DB로 보낸다.
	if( kPacketToDB.m_vecItemInfo.empty() == false )
#endif SERV_OPEN_CUBE_REALTIME_DB_UPDATE
		//}}
	{
		//{{ 2010. 7. 26  최육사	밀봉 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
		if( SiCXSLRandomItemManager()->IsSealRandomItemCube( iRandomItemID ) == true )
		{
			BOOST_TEST_FOREACH( const KItemInfo&, kItem, kPacketToDB.m_vecItemInfo )
			{
				//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
				// 이미 봉인 되어 있다면 봉인 처리 하지 않는다.
				if( kItem.IsSealedItem() == true )
					continue;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
				//}}

				if( SiCXSLItemManager()->IsCanSealForRandomItem( kItem.m_iItemID ) == true )
				{
					kPacketToDB.m_setSealRandomItem.insert( kItem.m_iItemID );
				}
			}
		}
#endif SERV_SEALED_RANDOM_ITEM
		//}}

		m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		if( bResurrectionStone == true )
		{
			kPacketToDB.m_iNumResurrectionStone = m_iNumResurrectionStone;
		}
		else
		{
			kPacketToDB.m_iNumResurrectionStone = 0;
		}
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}

		SendToGameDB( DBE_OPEN_RANDOM_ITEM_REQ, kPacketToDB );
	}
	//{{ 2011. 12. 19	최육사	랜덤큐브 DB실시간 적용
#ifdef SERV_OPEN_CUBE_REALTIME_DB_UPDATE
#else
	else
	{
		LIF( NetError::GetLastError() == NetError::NET_OK );

		KEGS_OPEN_RANDOM_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK				   = NetError::NET_OK;
		kPacketAck.m_iRessurectionCount	   = iRessurectionCount;
		kPacketAck.m_iRestoreSpirit		   = iRestoreSpirit;
		kPacketAck.m_mapInsertedItem	   = kPacketToDB.m_mapInsertedItem;
		kPacketAck.m_vecKInventorySlotInfo = kPacketToDB.m_vecUpdatedInventorySlot;

		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		DecreaseED( kPacketToDB.m_iSpendED, KUserEDManager::ER_OUT_ED_USE_CUBE_OPEN );
#else
		const int iBeforeED = m_iED;
		m_iED -= kPacketToDB.m_iSpendED;
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_CUBE_OPEN_ED_COST, iBeforeED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		kPacketAck.m_iED = m_iED;

		// 유저 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_OpenCubeSpendED, kPacketToDB.m_iSpendED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_ED_OpenCubeSpendED, kPacketToDB.m_iSpendED );
#endif //SERV_USER_STATISTICS_RENEWAL


		// ED 사용량 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_POpenCube, kPacketToDB.m_iSpendED );

#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

		SendPacket( EGS_OPEN_RANDOM_ITEM_ACK, kPacketAck );
	}
#endif SERV_OPEN_CUBE_REALTIME_DB_UPDATE
	//}}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_RANDOM, kPacketToDB.m_mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
IMPL_ON_FUNC( EGB_EXCHANGE_LIMIT_ACK )
{
	int iOK = kPacket_.m_iOK;
	KDBE_ITEM_EXCHANGE_REQ kPacketToDB;
	std::map< int, int > mapInsertedItem;
	KEGS_ITEM_EXCHANGE_REQ kStorage;

	if( m_kAfterWorkStorage.RetrieveData( kPacket_.m_iAfterWorkStorageKey, kStorage ) != KAfterWorkStorage::AWS_SUCCEED )
	{
		START_LOG( cerr, L"AfterWorkStorage: 꺼내오기 실패" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		iOK = NetError::ERR_EXCHANGE_LIMIT_01;	// 교환 제한_아이템 교환에 실패하였습니다.
		goto end_proc;
	}

	if( iOK != NetError::NET_OK )
	{
		goto end_proc;
	}

	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	kPacketToDB.m_iSourceItemID = kStorage.m_iSourceItemID;
	kPacketToDB.m_iSourceItemQuantity = kStorage.m_iSourceQuantity;
#endif SERV_EXCHANGE_LOG
	//}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	int	iEventMoney = GetEventMoney();
#endif // SERV_EVENT_MONEY

	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	if( m_kInventory.ItemExchange( kStorage.m_iHouseID, 
		kStorage.m_iSourceItemUID, 
		kStorage.m_iSourceItemID, 
		kStorage.m_iSourceQuantity, 
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		iEventMoney,
#endif // SERV_EVENT_MONEY
		kStorage.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo, 
		kPacketToDB.m_cExchangeType		) == false )
#else
/*
	//{{ 2011. 08. 16	최육사	아이템 교환 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	int iUserEventMoney = GetEventMoney();
	if( m_kInventory.ItemExchange( kStorage.m_iHouseID, 
		kStorage.m_iSourceItemUID, 
		kStorage.m_iSourceItemID, 
		kStorage.m_iSourceQuantity, 
		kStorage.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo,
		//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		iUserEventMoney,
		kPacketToDB.m_vecDestItem
#else
		iUserEventMoney
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		//}}
		) == false )
#else // SERV_EVENT_MONEY
	if( m_kInventory.ItemExchange( kStorage.m_iHouseID, 
		kStorage.m_iSourceItemUID, 
		kStorage.m_iSourceItemID, 
		kStorage.m_iSourceQuantity, 
		kStorage.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo 
		) == false )
#endif // SERV_EVENT_MONEY
#else
	if( m_kInventory.ItemExchange( kStorage.m_iSourceItemID, 
		kStorage.m_iHouseID, 
		kStorage.m_iDestItemID, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo 
		) == false )
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
*/
#endif SERV_EXCHANGE_LOG
	//}
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );
		iOK = NetError::GetLastError();
		goto end_proc;
	}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	// 천사의 깃털을 사용하였다.
	if( iEventMoney > 0 )
	{
		CTime tCurr = CTime::GetCurrentTime();

		KDBE_UPDATE_EVENT_MONEY_REQ kReq;
		kReq.m_iUserUID = GetUID();
		kReq.m_iUnitUID = GetCharUID();
		kReq.m_iOldQuantity = GetEventMoney();
		kReq.m_iNewQuantity = (-1) * iEventMoney; // 마이너스로 변경
		kReq.m_iRewardItemID = 0;
		std::map<int,int>::iterator mitInsert = mapInsertedItem.begin();
		if( mitInsert != mapInsertedItem.end() )
		{
			kReq.m_iRewardItemID = mitInsert->first;
		}
		kReq.m_wstrRegDate = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		SendToAccountDB( DBE_UPDATE_EVENT_MONEY_REQ, kReq );

		SetEventMoney( GetEventMoney() - iEventMoney );
	}
#endif // SERV_EVENT_MONEY

	//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
	kPacketToDB.m_mapResultItem	= mapInsertedItem;
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
	//}}

	// 교환된 아이템을 생성하기위해 DB로 간다
	kPacketToDB.m_iUnitUID = GetCharUID();	
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_ITEM_EXCHANGE_REQ, kPacketToDB );

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_ITEM_EXCHANGE, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}

	return;

end_proc:
	if( iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iRollBackUID != 0 )
		{
			// 교환 기록 롤백하러 갑시다.
			KEGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT kPacket;
			kPacket.m_iExchangeLimitUID	= kPacket_.m_iExchangeLimitUID;
			kPacket.m_iLimitUID			= kPacket_.m_iLimitUID;
			kPacket.m_iRollBackUID		= kPacket_.m_iRollBackUID;
			SendToGlobalServer( EGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT, kPacket );
		}

		KEGS_ITEM_EXCHANGE_ACK kPacketAck;
		kPacketAck.m_iOK = iOK;
		SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacketAck );
	}
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_COUPON_EVENT
IMPL_ON_FUNC( EGS_COUPON_ITEM_CHECK_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_COUPON_ITEM_CHECK_REQ, EGS_COUPON_ITEM_CHECK_ACK );

	// 이곳에서 버튼 타입을 체크하고, 인벤에 아이템 있는 지를 확인해서(시간을 기준으로) 리턴값을 정의해주어야 한다.
	int iCouponType = 0;
	int iItemID		= 0;
	KEGS_COUPON_ITEM_CHECK_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;

	// 아이템이 존재하고, 코드 이벤트에 해당하는 아이템이라면 보낸다.
	IF_EVENT_ENABLED(CEI_EVENT_COUPON_WEEKDAY)		// 평일
	{
		if( kPacket_.m_iButtonType == 1 )
		{
			kPacketAck.m_iCouponType = 1;		// 골드티켓(100)
			kPacketAck.m_iItemID = 60006512;
		}
		else if( kPacket_.m_iButtonType == 2 )
		{
			kPacketAck.m_iCouponType = 2;		// 골드티켓(100) 한번더!!
			kPacketAck.m_iItemID = 60006519;
		}
		else
		{
			kPacketAck.m_iOK = NetError::ERR_DANO_EVENT_02;
		}
	}	
	ELSE_IF_EVENT_ENABLED(CEI_EVENT_COUPON_WEEKEND)
	{
		if( kPacket_.m_iButtonType == 1 )
		{
			kPacketAck.m_iCouponType = 3;		// 골드티켓(500)
			kPacketAck.m_iItemID = 60006513;
		}
		else if( kPacket_.m_iButtonType == 2 )
		{
			kPacketAck.m_iCouponType = 4;		// 골드티켓(500) 한번더!!
			kPacketAck.m_iItemID = 60006520;
		}
		else
		{
			kPacketAck.m_iOK = NetError::ERR_DANO_EVENT_02;
		}
	}
	ELSE
	{
		kPacketAck.m_iOK = NetError::ERR_DANO_EVENT_03;
	}

	if(m_kInventory.IsExistOnlyInventory(kPacketAck.m_iItemID) == false) 
	{
		kPacketAck.m_iOK = NetError::ERR_DANO_EVENT_01; // 인벤토리에 아이템이 존재하지 않으면
	}

	START_LOG( clog, L"COUPON_ITEM_CHECK_REQ 정보확인")
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( GetCharName() )
		<< BUILD_LOG( kPacketAck.m_iOK )
		<< BUILD_LOG( kPacketAck.m_iCouponType )
		<< BUILD_LOG( kPacketAck.m_iItemID )
		<< END_LOG;

	SendPacket( EGS_COUPON_ITEM_CHECK_ACK, kPacketAck );
}

IMPL_ON_FUNC( EGS_COUPON_ENTRY_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_COUPON_ENTRY_REQ, EGS_COUPON_ENTRY_ACK );

	KEGS_COUPON_ENTRY_ACK kPacketAck;
	int iItemID = 0;

	switch(kPacket_.m_iCouponType)
	{
	case 1:					// 골드티켓(100)
		{
			iItemID = 60006512;
		} break;
	case 2:					// 골드티켓(100) 한번더!
		{
			iItemID = 60006519;
		} break;
	case 3:					// 골드티켓(500) 
		{
			iItemID = 60006513;
		} break;
	case 4:					// 골드티켓(500) 한번더!
		{
			iItemID = 60006520;
		} break;
	default:
		{
			kPacketAck.m_iOK = NetError::ERR_DANO_EVENT_02;
			SendPacket(EGS_COUPON_ENTRY_ACK, kPacketAck);
			return;
		}		
	}

	KDBE_COUPON_ENTRY_REQ kPacketReq;
	std::map< int, int > mapRequiredItem;
	int iQuantity = 1;
	mapRequiredItem.insert( std::make_pair( iItemID, iQuantity ) );

	if( m_kInventory.DeleteItemOnlyInventory( mapRequiredItem, kPacketReq.m_vecUpdatedInventorySlot, KDeletedItemInfo::DR_NO_REASON ) == false )
	{
		kPacketAck.m_iOK = NetError::ERR_DANO_EVENT_02;
		SendPacket(EGS_COUPON_ENTRY_ACK, kPacketAck);
		return ;
	}
	else
	{		
		m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
		
		kPacketReq.m_iUserUID = GetUID();
		kPacketReq.m_iUnitUID = GetCharUID();
		kPacketReq.m_iCouponType = kPacket_.m_iCouponType;
		SendToGameDB(DBE_COUPON_ENTRY_REQ, kPacketReq);

		KStatisticsKey kKeyES;
		kKeyES.m_vecIntKey.push_back( iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyES, KStatistics::SI_ITEM_USE, iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyES, KStatistics::SI_ITEM_USE, iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}
}

IMPL_ON_FUNC( DBE_COUPON_ENTRY_ACK )
{
	KEGS_COUPON_ENTRY_ACK kPacketAck;
	kPacketAck.m_iOK = kPacket_.m_iOK;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	kPacketAck.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	SendPacket(EGS_COUPON_ENTRY_ACK, kPacketAck);
}
#endif SERV_COUPON_EVENT

#ifdef SERV_READY_TO_SOSUN_EVENT
IMPL_ON_FUNC_NOPARAM( EGS_READY_TO_SOSUN_EVENT_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_READY_TO_SOSUN_EVENT_REQ, EGS_READY_TO_SOSUN_EVENT_ACK );

	// 소선이 나올 확률을 10%로 하기위해서 iFirstUnitClass가 28과 29가 나오면 소선이 나온것으로 함.(UnitClass 29는 존재하지 않지만 29가 나오면 소선이 나온 것처럼 처리)
	KDBE_READY_TO_SOSUN_EVENT_REQ kPacketDB;
	kPacketDB.m_iUnitUID = GetCharUID();
	kPacketDB.m_iFirstUnitClass = 10 + (rand() % 20);

	if( kPacketDB.m_iFirstUnitClass == 29 )
	{
		kPacketDB.m_iFirstUnitClass = 28;
	}

	if( m_kInventory.IsEnoughEmptySlot( CXSLInventory::ST_SPECIAL, 1 ) == false )
	{
		START_LOG(cerr, L"인벤토리 공간 부족")
			<< END_LOG;

		KEGS_READY_TO_SOSUN_EVENT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_TRADE_09;
		kPacketAck.m_iFirstUnitClass = kPacketDB.m_iFirstUnitClass;
		SendPacket( EGS_READY_TO_SOSUN_EVENT_ACK, kPacketAck );
	}

	START_LOG(cout, L"아라 1차 전직 이름 맞추기 이벤트")
		<< BUILD_LOG( kPacketDB.m_iFirstUnitClass )
		<< END_LOG;

	if( m_kInventory.ExchangeToEvent( CXSLItem::EI_READY_TO_SOSUN_EVENT_ITEM, 1, CXSLItem::EI_READY_TO_SOSUN_EVENT_WIN_ITEM, 1, kPacketDB.m_vecUpdatedInventorySlot, kPacketDB.m_vecItemInfo, kPacketDB.m_iFirstUnitClass ) == false )
	{
		LIF( NetError::GetLastError() != NetError::NET_OK );

		START_LOG(cerr, L"아라 1차 전직 이름 맞추기 이벤트 보상 지급 실패?")
			<< BUILD_LOG( kPacketDB.m_iFirstUnitClass )
			<< END_LOG;

		KEGS_READY_TO_SOSUN_EVENT_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		kPacketAck.m_iFirstUnitClass = kPacketDB.m_iFirstUnitClass;
		SendPacket( EGS_READY_TO_SOSUN_EVENT_ACK, kPacketAck );
	}
	else
	{
		// 새로 추가된 아이템이 있으면 DB로 보낸다.
		if( kPacketDB.m_vecItemInfo.empty() == false )
		{
			m_kInventory.FlushQuantityChange( kPacketDB.m_kItemQuantityUpdate.m_mapQuantityChange );
			m_kInventory.FlushDeletedItem( kPacketDB.m_kItemQuantityUpdate.m_vecDeleted );

			SendToGameDB( DBE_READY_TO_SOSUN_EVENT_REQ, kPacketDB );
		}
		else
		{
			LIF( NetError::GetLastError() == NetError::NET_OK );

			KEGS_READY_TO_SOSUN_EVENT_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_vecKInventorySlotInfo = kPacketDB.m_vecUpdatedInventorySlot;
			kPacketAck.m_iFirstUnitClass = kPacketDB.m_iFirstUnitClass;
			SendPacket( EGS_READY_TO_SOSUN_EVENT_ACK, kPacketAck );
		}
	}
}

IMPL_ON_FUNC( DBE_READY_TO_SOSUN_EVENT_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_READY_TO_SOSUN_EVENT_ACK );

	KEGS_READY_TO_SOSUN_EVENT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iFirstUnitClass = kPacket_.m_iFirstUnitClass;

	if( kPacket_.m_iOK == NetError::ERR_RANDOM_ITEM_06 )
	{
		// TODO : DB 에 아이템 삽입 실패. 추후에 로그 추가.
		kPacket.m_iOK = NetError::NET_OK;
	}

	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	SendPacket( EGS_READY_TO_SOSUN_EVENT_ACK, kPacket );
}
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
IMPL_ON_FUNC( EGS_USE_PROPOSE_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_USE_PROPOSE_ITEM_REQ, EGS_USE_PROPOSE_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 조건 검사
	//////////////////////////////////////////////////////////////////////////
	IF_EVENT_ENABLED( CEI_RELATIONSHIP_CODE_EVENT )
	{
		
	}
	ELSE
	{
		// 가상 결혼 이벤트 종료
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_DANO_EVENT_03;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	// 정상적인 문자열인지 검사
	char charBuf[255] = {0};
	std::string strUnitName;

	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrNickName.c_str(), -1, charBuf, 255, NULL, NULL );
	strUnitName = charBuf;

	if( strUnitName.empty() == true || strUnitName.size() > 12 )   // 비어있는 값인지 확인
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_01;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

#ifdef SERV_STRING_FILTER_USING_DB
	if( SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
#else //SERV_STRING_FILTER_USING_DB
	if( SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrNickName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_01;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) == true )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_01;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	// 가상 결혼 신청서를 가지고 있는가?
	if( m_kInventory.IsExistOnlyInventory( CXSLItem::EI_EVENT_PROPOSE_ITEM ) == false )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_02;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	// 자신이 가상 결혼 중인가
	if( m_bCouple == true )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_03;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	SendToGameDB( DBE_EVENT_PROPOSE_USER_FIND_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_EVENT_PROPOSE_USER_FIND_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	// 캐릭터 성별 비교
#ifdef SERV_NEW_CHARACTER_EL	// 적용날짜: 2013-07-18
	int iOK = m_kUserRelationshipManager.CheckChar( GetUnitClass(), kPacket_.m_cUnitClass );
	if( iOK != NetError::NET_OK )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = iOK;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}
#else	// SERV_NEW_CHARACTER_EL
	if( m_kUserRelationshipManager.CheckChar( GetUnitType(), kPacket_.m_cUnitClass ) == false )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_05;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}
#endif	// SERV_NEW_CHARACTER_EL

	// 상대방 가상 결혼 상태 확인
	if( kPacket_.m_bCouple == true )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_06;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	// 상대방 데이터 무결성 확인
	if( kPacket_.m_wstrOtherNickName.empty() == true || kPacket_.m_iOtherUserUID <= 0 || kPacket_.m_iOtherUnitUID <= 0 )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_07;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 로그인 서버로 가서 접속 중인지 확인 하자
	KELG_EVENT_PROPOSE_CHECK_CONNECT_USER_REQ kReq;
	kReq.m_iOtherUserUID = kPacket_.m_iOtherUserUID;
	kReq.m_iOtherUnitUID = kPacket_.m_iOtherUnitUID;
	kReq.m_wstrRequestUnitName = GetCharName();
	kReq.m_iRequestUserUID = GetUID();
	kReq.m_iRequestUnitUID = GetCharUID();
	kReq.m_wstrAcceptUnitName = kPacket_.m_wstrOtherNickName;

	SendToLoginServer( ELG_EVENT_PROPOSE_CHECK_CONNECT_USER_REQ, kReq );
}

IMPL_ON_FUNC( ELG_EVENT_PROPOSE_CHECK_CONNECT_USER_ACK )
{
	KEGS_USE_PROPOSE_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_EVENT_PROPOSE_NOT )
{
	KEGS_COUPLE_PROPOSE_NOT kPacket;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	SendPacket( EGS_EVENT_PROPOSE_NOT, kPacket );
}

IMPL_ON_FUNC( EGS_EVENT_PROPOSE_AGREE_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_cAnswer == KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_YES )
	{
		// 수락 한 경우
		// 인벤토리에서 가상 결혼 신청서를 찾아 지워라
		bool bFindItem = false;

		KInventoryItemInfo kResultItemInfo;
		std::vector< UidType > vecItemUIDList;
		m_kInventory.GetItemUIDListThisItem( CXSLItem::EI_EVENT_PROPOSE_ITEM, vecItemUIDList, true );

		if( vecItemUIDList.empty() == true )	// 아이템이 없는 경우 자동으로 거절 처리하고, 수락자가 가진 아이템의 삭제 처리는 신청자의 요청에 의해 하도록 한다.
		{
			kPacket_.m_cAnswer = KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_NO;
		}
		/*
		std::map< UidType, int > mapDeleteItem;
		std::map< int, KItemInfo> mapDummy;
		std::vector<KItemInfo> vecNewItem;
		std::vector< KInventoryItemInfo > vecUpdatedInventorySlot;
		
		if( vecItemUIDList.empty() == false )
		{
			// 아무거나 하나면 된다. 첫번째 꺼로 하자
			mapDeleteItem.insert( std::make_pair( vecItemUIDList[0], 1 ) );
			if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, vecUpdatedInventorySlot, vecNewItem, KDeletedItemInfo::DR_RELATIONSHIP ) == true )
			{
				bFindItem = true;
			}
		}

		if( bFindItem == false )
		{
			kPacket_.m_cAnswer = KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_NO;
		}
		else
		{
			// 수락한 캐릭터의 아이템을 찾았으니 DB에 삭제 요청만 함. 나머지 처리는 프로포즈한 캐릭터가 처리
			KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ kPacketDB;
			//kPacketDB.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
			//kPacketDB.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
			//kPacketDB.m_iAcceptUserUID = GetUID();
			//kPacketDB.m_ucAcceptUnitLevel = GetLevel();
			//kPacketDB.m_cAcceptUnitClass = GetUnitClass();
			//kPacketDB.m_iAcceptUnitUID = GetCharUID();
			//kPacketDB.m_wstrAcceptUnitName = GetCharName();
			kPacketDB.m_vecUpdatedInventorySlot = vecUpdatedInventorySlot;
			m_kInventory.FlushDeletedItem( kPacketDB.m_vecDeleted );

			SendToGameDB( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ, kPacketDB );
		}
		*/
	}

	// 신청자에게 결과를 전달한다.
	kPacket_.m_iAcceptUnitUID = GetCharUID();
	kPacket_.m_wstrAcceptUnitName = GetCharName();
	kPacket_.m_iAcceptUserUID = GetUID();
	kPacket_.m_ucAcceptUnitLevel = GetLevel();
	kPacket_.m_cAcceptUnitClass = GetUnitClass();
	SendToLoginServer( ELG_EVENT_PROPOSE_AGREE_CHECK_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_EVENT_PROPOSE_AGREE_CHECK_NOT, KEGS_EVENT_PROPOSE_AGREE_NOT )
{
	// 거절 한 경우 - 그냥 거절 결과만 전달
	if( kPacket_.m_cAnswer != KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_YES )
	{
		KEGS_EVENT_PROPOSE_RESULT_NOT kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_cResult = kPacket_.m_cAnswer;
		kPacket.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
		kPacket.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
		SendPacket( EGS_EVENT_PROPOSE_RESULT_NOT, kPacket );

		return;
	}

	// 수락 한 경우
	// 인벤토리에서 가상 결혼 신청서를 찾아 지워라
	bool bFindItem = false;

	KInventoryItemInfo kResultItemInfo;
	std::vector< UidType > vecItemUIDList;
	m_kInventory.GetItemUIDListThisItem( CXSLItem::EI_EVENT_PROPOSE_ITEM, vecItemUIDList, true );

	std::map< UidType, int > mapDeleteItem;
	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;
	std::vector< KInventoryItemInfo > vecUpdatedInventorySlot;

	if( vecItemUIDList.empty() == false )
	{
		// 아무거나 하나면 된다. 첫번째 꺼로 하자
		mapDeleteItem.insert( std::make_pair( vecItemUIDList[0], 1 ) );
		if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, vecUpdatedInventorySlot, vecNewItem, KDeletedItemInfo::DR_RELATIONSHIP ) == true )
		{
			bFindItem = true;
		}
	}

	if( bFindItem == false )
	{
		// 신청자에게 전달
		KEGS_EVENT_PROPOSE_RESULT_NOT kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_05;
		kPacket.m_cResult = KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_NO;
		kPacket.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
		kPacket.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
		SendPacket( EGS_EVENT_PROPOSE_RESULT_NOT, kPacket );

		// 수락자에게 전달
		KERM_EVENT_PROPOSE_RESULT_FAIL_NOT kFail;
		kFail.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_06;
		kFail.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
		kFail.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
		kFail.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
		kFail.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
		SendToLoginServer( ELG_EVENT_PROPOSE_RESULT_FAIL_NOT, kFail );

		return;
	}

	// 아이템을 찾았으니 DB 에 삭제 요청 하고 커플 성사 시키자
	KDBE_EVENT_MAKING_SUCCESS_REQ kPacketDB;
	kPacketDB.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kPacketDB.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacketDB.m_iAcceptUserUID = kPacket_.m_iAcceptUserUID;
	kPacketDB.m_ucAcceptUnitLevel = kPacket_.m_ucAcceptUnitLevel;
	kPacketDB.m_cAcceptUnitClass = kPacket_.m_cAcceptUnitClass;
	kPacketDB.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
	kPacketDB.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	kPacketDB.m_vecUpdatedInventorySlot = vecUpdatedInventorySlot;
	m_kInventory.FlushDeletedItem( kPacketDB.m_vecDeleted );

	SendToGameDB( DBE_EVENT_MAKING_SUCCESS_REQ, kPacketDB );
}

IMPL_ON_FUNC( DBE_EVENT_MAKING_SUCCESS_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 신청장에게 전달
		KEGS_EVENT_PROPOSE_RESULT_NOT kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		kPacket.m_cResult = KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_NO;
		kPacket.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
		kPacket.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
		SendPacket( EGS_EVENT_PROPOSE_RESULT_NOT, kPacket );

		if( kPacket_.m_iOK == NetError::ERR_RELATIONSHIP_EVENT_01 )
		{
			m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		}

		return;
	}

	// 수락자에게는 커플 성공 정보를 보낸다.
	KERM_EVENT_PROPOSE_RESULT_SUCCESS_NOT kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
	kPacket.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	kPacket.m_tDate = kPacket_.m_tDate;
	SendToLoginServer( ELG_EVENT_PROPOSE_RESULT_SUCCESS_NOT, kPacket );

	// 신청자에게는 가상 결혼 신청서 삭제 정보와 가상 결혼 정보를 보낸다.
	// 신청장에게 전달	
	// 커플 정보 셋팅
	m_bCouple = true;
	m_iRelationTargetUserUid = kPacket_.m_iAcceptUnitUID;
	m_wstrRelationTargetUserNickname = kPacket_.m_wstrAcceptUnitName;

	// DB로 보상 정보를 보낸다.
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_iToUnitUID   = GetCharUID();
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT; // 보상 타입
	kPacketToDB.m_iRewardID	   = _CONST_SERV_RELATIONSHIP_EVENT_INT_REWARD_ID::iRelationshipEventReward; // 로맨틱 조각 케이크, 로맨틱 샴페인, 가상 결혼 계약서
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

	KEGS_EVENT_PROPOSE_RESULT_NOT kResult;
	kResult.m_cResult = KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_YES;
	kResult.m_iUnitUID = kPacket_.m_iAcceptUnitUID;
	kResult.m_wstrUnitName = kPacket_.m_wstrAcceptUnitName;
	kResult.m_tDate = kPacket_.m_tDate;
	kResult.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_EVENT_PROPOSE_RESULT_NOT, kResult );
}

_IMPL_ON_FUNC( ELG_EVENT_PROPOSE_RESULT_SUCCESS_NOT, KERM_EVENT_PROPOSE_RESULT_SUCCESS_NOT )
{	
	// 프로포즈 수락자만 받는 결과 패킷
	bool bFindItem = false;

	KInventoryItemInfo kResultItemInfo;
	std::vector< UidType > vecItemUIDList;
	m_kInventory.GetItemUIDListThisItem( CXSLItem::EI_EVENT_PROPOSE_ITEM, vecItemUIDList, true );

	std::map< UidType, int > mapDeleteItem;
	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;
	std::vector< KInventoryItemInfo > vecUpdatedInventorySlot;
		
	if( vecItemUIDList.empty() == false )
	{
		// 아무거나 하나면 된다. 첫번째 꺼로 하자
		mapDeleteItem.insert( std::make_pair( vecItemUIDList[0], 1 ) );
		if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, vecUpdatedInventorySlot, vecNewItem, KDeletedItemInfo::DR_RELATIONSHIP ) == true )
		{
			bFindItem = true;
		}
	}

	if( bFindItem == true )
	{
		KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ kPacketDB;
		//kPacketDB.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
		//kPacketDB.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
		//kPacketDB.m_iAcceptUserUID = GetUID();
		//kPacketDB.m_ucAcceptUnitLevel = GetLevel();
		//kPacketDB.m_cAcceptUnitClass = GetUnitClass();
		//kPacketDB.m_iAcceptUnitUID = GetCharUID();
		//kPacketDB.m_wstrAcceptUnitName = GetCharName();
		kPacketDB.m_vecUpdatedInventorySlot = vecUpdatedInventorySlot;
		m_kInventory.FlushDeletedItem( kPacketDB.m_vecDeleted );

		SendToGameDB( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ, kPacketDB );
	}
	
	m_bCouple = true;
	m_iRelationTargetUserUid = kPacket_.m_iRequestUnitUID;
	m_wstrRelationTargetUserNickname = kPacket_.m_wstrRequestUnitName;

	KEGS_EVENT_PROPOSE_RESULT_NOT kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_cResult = KEGS_EVENT_PROPOSE_AGREE_NOT::CRT_YES;
	kPacket.m_iUnitUID = kPacket_.m_iRequestUnitUID;	// 상대방 
	kPacket.m_wstrUnitName = kPacket_.m_wstrRequestUnitName;	// 상대방 
	kPacket.m_tDate = kPacket_.m_tDate;

	SendPacket( EGS_EVENT_PROPOSE_RESULT_NOT, kPacket );
}

IMPL_ON_FUNC( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iOK == NetError::ERR_RELATIONSHIP_EVENT_01 )
		{
			m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		}

		return;
	}

	// DB로 보상 정보를 보낸다.
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = GetCharUID();
	kPacketToDB.m_iToUnitUID   = GetCharUID();
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT; // 보상 타입
	kPacketToDB.m_iRewardID	   = _CONST_SERV_RELATIONSHIP_EVENT_INT_REWARD_ID::iRelationshipEventReward; // 로맨틱 조각 케이크, 로맨틱 샴페인, 가상 결혼 계약서
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

	KEGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT kResult;
	kResult.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT, kResult );
}

IMPL_ON_FUNC_NOPARAM( EGS_USE_DIVORCE_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_USE_DIVORCE_ITEM_REQ, EGS_USE_DIVORCE_ITEM_ACK );

	IF_EVENT_ENABLED( CEI_RELATIONSHIP_CODE_EVENT )
	{

	}
	ELSE
	{
		// 가상 결혼 이벤트 종료
		KEGS_USE_DIVORCE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_DANO_EVENT_03;
		SendPacket( EGS_USE_DIVORCE_ITEM_ACK, kPacket );
		return ;
	}

	if( m_bCouple == false )
	{
		KEGS_USE_DIVORCE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_02;
		SendPacket( EGS_USE_DIVORCE_ITEM_ACK, kPacket );
		return ;
	}

	if( m_iRelationTargetUserUid <= 0 )
	{
		KEGS_USE_DIVORCE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_02;
		SendPacket( EGS_USE_DIVORCE_ITEM_ACK, kPacket );
		return ;
	}

	// 가상 결혼 계약서를 가지고 있는가?
	/*
	if( m_kInventory.IsExistOnlyInventory( CXSLItem::EI_EVENT_DIVORCE_ITEM ) == false )
	{
		KEGS_USE_PROPOSE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_02;
		SendPacket( EGS_USE_PROPOSE_ITEM_ACK, kPacket );
		return;
	}
	*/

	// 인벤토리에서 가상 결혼 계약서를 찾아 지워라
	bool bFindItem = false;

	KInventoryItemInfo kResultItemInfo;
	std::vector< UidType > vecItemUIDList;
	m_kInventory.GetItemUIDListThisItem( CXSLItem::EI_EVENT_DIVORCE_ITEM, vecItemUIDList, true );

	std::map< UidType, int > mapDeleteItem;
	std::map< int, KItemInfo> mapDummy;
	std::vector<KItemInfo> vecNewItem;
	std::vector< KInventoryItemInfo > vecUpdatedInventorySlot;

	if( vecItemUIDList.empty() == false )
	{
		// 아무거나 하나면 된다. 첫번째 꺼로 하자
		mapDeleteItem.insert( std::make_pair( vecItemUIDList[0], 1 ) );
		if( m_kInventory.DeleteAndInsert( mapDeleteItem, mapDummy, vecUpdatedInventorySlot, vecNewItem, KDeletedItemInfo::DR_RELATIONSHIP ) == true )
		{
			bFindItem = true;
		}
	}

	if( bFindItem == false )
	{
		KEGS_USE_DIVORCE_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_03;
		SendPacket( EGS_USE_DIVORCE_ITEM_ACK, kPacket );
		return;
	}
	else
	{
		KDBE_EVENT_DIVORCE_REQ kPacketToDB;
		kPacketToDB.m_iRelationTargetUserUid = m_iRelationTargetUserUid;
		kPacketToDB.m_iUnitUID = GetCharUID();
		kPacketToDB.m_vecUpdatedInventorySlot = vecUpdatedInventorySlot;
		m_kInventory.FlushDeletedItem( kPacketToDB.m_vecDeleted );

		SendToGameDB( DBE_EVENT_DIVORCE_REQ, kPacketToDB );
	}	
}

IMPL_ON_FUNC( DBE_EVENT_DIVORCE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iOK == NetError::ERR_RELATIONSHIP_EVENT_04 )
		{
			m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
		}

		KEGS_USE_DIVORCE_ITEM_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_USE_DIVORCE_ITEM_ACK, kPacket );

		return ;
	}

	m_bCouple = false;
	m_iRelationTargetUserUid = 0;
	m_wstrRelationTargetUserNickname = L"";

	KELG_DIVORCE_NOT kNot;
	kNot.m_iLoverUnitUID = kPacket_.m_iRelationTargetUserUid;
	SendToLoginServer( ELG_DIVORCE_NOT, kNot );

	KEGS_USE_DIVORCE_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_USE_DIVORCE_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_DIVORCE_NOT )
{
	m_bCouple = false;
	m_iRelationTargetUserUid = 0;
	m_wstrRelationTargetUserNickname = L"";

	KEGS_DIVORCE_NOT kPacket;
	kPacket.m_iCost = 0;
	SendPacket( EGS_DIVORCE_NOT, kPacket );
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
IMPL_ON_FUNC( EGS_USE_RECRUIT_TICKET_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_USE_RECRUIT_TICKET_REQ, EGS_USE_RECRUIT_TICKET_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KEGS_USE_RECRUIT_TICKET_ACK kPacket;
	int iUsedItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	switch( iUsedItemID )
	{
	case CXSLItem::SI_RECRUIT_TICKET:
		{
			KDBE_USE_RECRUIT_TICKET_REQ kPacket;
			kPacket.m_iUnitUID	= GetCharUID();
			SendToGameDB( DBE_USE_RECRUIT_TICKET_REQ, kPacket );
		}
		break;
	default:
		{
			kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_05;
			SendPacket( EGS_USE_RECRUIT_TICKET_ACK, kPacket );
		} break;
	}
}

IMPL_ON_FUNC( DBE_USE_RECRUIT_TICKET_ACK )
{	
	KEGS_USE_RECRUIT_TICKET_ACK kPacket;

	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_USE_RECRUIT_TICKET_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_REGISTER_RECRUITER_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REGISTER_RECRUITER_REQ, EGS_REGISTER_RECRUITER_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrNickname ) == false )
	{
		KEGS_REGISTER_RECRUITER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_STRING_FILTER_01;
		SendPacket( EGS_REGISTER_RECRUITER_ACK, kPacketAck );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	// 자기 자신을 추천한 경우.
#ifdef SERV_NICKNAME_CHECK_IGNORE_CASE
	if( boost::iequals(GetCharName(),kPacket_.m_wstrNickname) )
#else
	if( GetCharName().compare( kPacket_.m_wstrNickname ) == 0 )
#endif SERV_NICKNAME_CHECK_IGNORE_CASE
	{
		KEGS_REGISTER_RECRUITER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_RECOMMEND_USER_01;
		SendPacket( EGS_REGISTER_RECRUITER_ACK, kPacketAck );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickname ) )
	{
		KEGS_REGISTER_RECRUITER_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_REGISTER_RECRUITER_ACK, kPacketAck );
		return;
	}

	KDBE_REGISTER_RECRUITER_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_wstrNickname = kPacket_.m_wstrNickname;
	SendToGameDB( DBE_REGISTER_RECRUITER_REQ, kPacket );
}

IMPL_ON_FUNC( DBE_REGISTER_RECRUITER_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REGISTER_RECRUITER_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_REGISTER_RECRUITER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 사용이 성공했으니 아이템을 지웁니다.
		m_kInventory.DeleteItemAll( CXSLItem::SI_RECRUIT_TICKET, kPacket.m_vecKInventorySlotInfo, KDeletedItemInfo::DR_USE_ITEM_IN_INVENTORY );
		if( kPacket.m_vecKInventorySlotInfo.empty() == true )
		{
			START_LOG( cerr, L"헐 무슨 조건 때문에 아이템이 안 지워졌나.. 해킹?" )
				<< END_LOG;
			kPacket.m_iOK = NetError::GetLastError();
			SendPacket( EGS_REGISTER_RECRUITER_ACK, kPacket );
			return;
		}

		// 추천인 UnitUID 등록
		m_kUserRecommendManager.AddRecruiterUnitInfo( kPacket_.m_kRecruiterUnitInfo );

		// 이벤트용 아이템 우편지급
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = GetCharUID();
		kPacketToDB.m_iToUnitUID   = GetCharUID();
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_MESSAGE;
		kPacketToDB.m_iRewardID	   = CXSLItem::EI_RECRUIT_CUBE;	// Elrecruit 보상 큐브
		kPacketToDB.m_sQuantity	   = 1;
		kPacketToDB.m_wstrMessage  = SiCXSLItemManager()->GetItemTemplet( kPacketToDB.m_iRewardID )->m_Name;
		kPacketToDB.m_bGameServerEvent = false;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );

		CTime tCurTime = CTime::GetCurrentTime();
		std::wstring wstrCurTime = ( CStringW )( tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

		// 추천 메시지를 추천인에게 보냄.
		KEGS_REGISTER_RECRUITER_NOT kNot;
		kNot.m_iRecruiterUnitUID					= kPacket_.m_kRecruiterUnitInfo.m_iUnitUID;
		kNot.m_kRecruitUnitInfo.m_iUnitUID			= GetCharUID();
		kNot.m_kRecruitUnitInfo.m_wstrNickName		= GetCharName();
		kNot.m_kRecruitUnitInfo.m_ucLevel			= GetLevel();
		kNot.m_kRecruitUnitInfo.m_wstrRecommendDate	= wstrCurTime;
		kNot.m_kRecruitUnitInfo.m_cUnitClass		= GetUnitClass();

		SendToLoginServer( ELG_REGISTER_RECRUITER_NOT, kNot );

#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		m_kUserQuestManager.CheckEventQuest( GetThisPtr<KGSUser>() );
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
	}

	SendPacket( EGS_REGISTER_RECRUITER_ACK, kPacket );
}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
IMPL_ON_FUNC( EGS_ITEM_EXTRACT_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_ITEM_EXTRACT_REQ, EGS_ITEM_EXTRACT_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		//{{ 2009. 10. 15  최육사	거래 예외처리
		if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
		{
			START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_ITEM_EXTRACT_ACK kPacket;
			kPacket.m_iOK	= NetError::ERR_EXTRACT_00;
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacket );
			return;
		}
		//}}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			KEGS_ITEM_EXTRACT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_ITEM_EXCHANGE_ACK, kPacket );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	// 1. 추출 가능 아이템인지 확인
	int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	if( m_kInventory.IsPossibleExtractItem( iItemID ) == false )
	{
		// 추출 가능한 아이템이 아닙니다.
		KEGS_ITEM_EXTRACT_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_EXTRACT_00;
		SendPacket( EGS_ITEM_EXTRACT_ACK, kPacket );
		return;
	}

	// 2. 수량 확인
	KItemInfo kSourceItemInfo;
	if( m_kInventory.GetItemInfo( kPacket_.m_iItemUID, kSourceItemInfo ) == false )
	{
		// 추출 아이템 정보가 이상합니다.
		KEGS_ITEM_EXTRACT_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_EXTRACT_01;
		SendPacket( EGS_ITEM_EXTRACT_ACK, kPacket );
		return;
	}

	if( kSourceItemInfo.m_iQuantity < kPacket_.m_iQuantity )
	{
		// 아이템 수량이 이상합니다.
		KEGS_ITEM_EXTRACT_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_EXTRACT_02;
		SendPacket( EGS_ITEM_EXTRACT_ACK, kPacket );
		return;
	}

	KDBE_ITEM_EXTRACT_REQ kPacketToDB;
	std::map< int, int > mapInsertedItem;

	// 3. 실제 추출 처리
	if( m_kInventory.ItemExtract( kPacket_.m_iItemUID, 
		kPacket_.m_iQuantity, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo	) == false )
	{
		// 아이템 추출이 실패 했습니다.
		KEGS_ITEM_EXTRACT_ACK kPacket;
		kPacket.m_iOK	= NetError::GetLastError();
		SendPacket( EGS_ITEM_EXTRACT_ACK, kPacket );
		return;
	}

	// 아이템 어뷰저	
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_ITEM_EXTRACT, mapInsertedItem );

	// 4. DB 기록 데이터 처리
	kPacketToDB.m_mapResultItem	= mapInsertedItem;

	// 교환된 아이템을 생성하기위해 DB로 간다
	kPacketToDB.m_iUnitUID = GetCharUID();	
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	// Send DB Packet 
	SendToGameDB( DBE_ITEM_EXTRACT_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_ITEM_EXTRACT_ACK )
{
	KEGS_ITEM_EXTRACT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_mapInsertedItem = kPacket_.m_mapResultItem;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecKInventorySlotInfo );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	SendPacket( EGS_ITEM_EXTRACT_ACK, kPacket );	
}

IMPL_ON_FUNC( EGS_USE_FINALITY_SKILL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_USE_FINALITY_SKILL_REQ, EGS_USE_FINALITY_SKILL_ACK );

#ifdef SERV_BALANCE_FINALITY_SKILL_EVENT
	// 0. 궁극기 사용 아이템이 필요 없는 상태인지 확인(이벤트)
	if( true == kPacket_.m_bNoConsume )
	{
		if( true == m_kInventory.IsExistOnlyInventory(CXSLItem::EI_INFINITY_EL_ESSENCE_ITEM_EVENT) )
		{
			START_LOG( clog, L"[TEST] 엘의 정수 공급기 소지하고 하엑 사용" )
				<< BUILD_LOG( GetCharName() );

			KEGS_USE_FINALITY_SKILL_ACK kPacket;
			kPacket.m_iOK	= NetError::NET_OK;
			kPacket.m_bNoConsume = true;
			SendPacket( EGS_USE_FINALITY_SKILL_ACK, kPacket );
			return;
		}
		else
		{
			START_LOG( cerr, L"엘의 정수 공급기 소지하고 있다고 클라에서 왔는데, 서버에는 없다?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharName() );

			KEGS_USE_FINALITY_SKILL_ACK kPacket;
			kPacket.m_iOK	= NetError::ERR_FINALITY_SKILL_00;
			SendPacket( EGS_USE_FINALITY_SKILL_ACK, kPacket );
			return;
		}
	}
#endif //SERV_BALANCE_FINALITY_SKILL_EVENT

	// 1. 궁극기 사용 아이템인지 확인
	int iItemID = m_kInventory.GetItemID( kPacket_.m_iItemUID );
	if( iItemID != CXSLAttribEnchantItem::ATI_ESSENCE )
	{
		// 궁극기 사용 아이템이 아닙니다.
		KEGS_USE_FINALITY_SKILL_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_FINALITY_SKILL_00;
		SendPacket( EGS_USE_FINALITY_SKILL_ACK, kPacket );
		return;
	}

	// 2. 수량 확인
	KItemInfo kSourceItemInfo;
	if( m_kInventory.GetItemInfo( kPacket_.m_iItemUID, kSourceItemInfo ) == false )
	{
		// 추출 아이템 정보가 이상합니다.
		KEGS_USE_FINALITY_SKILL_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_FINALITY_SKILL_01;
		SendPacket( EGS_USE_FINALITY_SKILL_ACK, kPacket );
		return;
	}

	// 1회 - 1개 사용
	if( kSourceItemInfo.m_iQuantity < 1 )
	{
		// 아이템 수량이 이상합니다.
		KEGS_USE_FINALITY_SKILL_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_FINALITY_SKILL_02;
		SendPacket( EGS_USE_FINALITY_SKILL_ACK, kPacket );
		return;
	}

	KDBE_USE_FINALITY_SKILL_REQ kPacketToDB;
	std::map< int, int > mapInsertedItem;

	// 3. 실제 삭제 처리
	if( m_kInventory.UseFinalitySkill( kPacket_.m_iItemUID, 
		1, 
		mapInsertedItem, 
		kPacketToDB.m_vecUpdatedInventorySlot, 
		kPacketToDB.m_vecItemInfo	) == false )
	{
		// 아이템 사용 실패
		KEGS_USE_FINALITY_SKILL_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_FINALITY_SKILL_03;
		SendPacket( EGS_USE_FINALITY_SKILL_ACK, kPacket );
		return;
	}

	// 교환된 아이템을 생성하기위해 DB로 간다
	kPacketToDB.m_iUnitUID = GetCharUID();	
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );

	// Send DB Packet 
	SendToGameDB( DBE_USE_FINALITY_SKILL_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_USE_FINALITY_SKILL_ACK )
{
	KEGS_ITEM_EXTRACT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	kPacket.m_vecKInventorySlotInfo.insert( kPacket.m_vecKInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );

	SendPacket( EGS_USE_FINALITY_SKILL_ACK, kPacket );	
}
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_GOOD_ELSWORD
IMPL_ON_FUNC_NOPARAM( EGS_EXPAND_BANK_SLOT_ED_REQ )
{
    // 상태 체크, 중복 패킷 처리.
    VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_EXPAND_BANK_SLOT_ED_ACK );
    
#ifdef SERV_CLOSE_GOOD_ELSWORD
	 goto end_proc;
#endif //SERV_CLOSE_GOOD_ELSWORD

    // 현재 은행 등급
    int iMyBankGrade = m_kInventory.GetBankMemberShip();

    if ( iMyBankGrade == CXSLInventory::MPB_PLATINUM ) 
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_02 );
        goto end_proc;
    }

    // 확장 요청한 은행 등급의 필요 ED 가져오기
    int iNextED = m_kInventory.GetNextUpgradeBankED( CXSLInventory::GetNextBankMemberShip( m_kInventory.GetBankMemberShip() ) );

    // 현재 ED 충분한가 
    if ( m_iED < iNextED ) 
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_01 );
        goto end_proc;
    }
    
    m_iED -= iNextED;
    // 디비로 큐잉
    if( CXSLInventory::GetNextBankMemberShip( m_kInventory.GetBankMemberShip() ) != CXSLInventory::MPB_NONE )
    {
        // 은행 확장하러 가자!
        KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
        kPacket.m_usEventID = EGS_EXPAND_BANK_SLOT_ED_REQ;
        kPacket.m_iUnitUID = GetCharUID();
        kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_BANK, CXSLInventory::SLOT_COUNT_ONE_LINE ) );
        kPacket.m_iED = iNextED;
        SendToGameDB( DBE_EXPAND_BANK_INVENTORY_REQ, kPacket ); // TODO : 같은 기능에 + 디비 로그 남기는 sp 추가한 함수로 큐잉하도록 수정하기
        SET_ERROR( NET_OK );
    }

end_proc:
    if ( NetError::GetLastError() != NetError::NET_OK )
    {
        KEGS_EXPAND_BANK_SLOT_ED_ACK kPacket;
        SendPacket( EGS_EXPAND_BANK_SLOT_ED_ACK, kPacket );
    }
    
}

//{{ 2009. 8. 7  최육사		은행
IMPL_ON_FUNC( DBE_EXPAND_BANK_INVENTORY_ACK )
{
    VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

    KEGS_EXPAND_BANK_SLOT_ED_ACK kAck; // == KEGS_EXPAND_INVENTORY_ED_ACK 동일하다. typedef
    KELOG_EXPAND_BANK_INVENTORY_NOT kLog;

    std::map< int, int >::const_iterator mit;
    for( mit = kPacket_.m_mapExpandedSlot.begin(); mit != kPacket_.m_mapExpandedSlot.end(); ++mit )
    {
        int iExpanded = 0;
        //{{ 2012. 12. 26	박세훈	인벤토리 개편 테스트	- 허상형 ( Merged by 박세훈 )
#ifdef SERV_REFORM_INVENTORY_TEST
        if( mit->first == CXSLInventory::ST_BANK )
        {
            m_kInventory.ExpandSlot( mit->first, mit->second, iExpanded );
        }
        else
        {
            m_kInventory.ExpandSlot( mit->first, mit->second - GetExpandedSlotSize( mit->first ), iExpanded );
        }
#else
        m_kInventory.ExpandSlot( mit->first, mit->second, iExpanded );
#endif SERV_REFORM_INVENTORY_TEST
        //}}
        kAck.m_mapExpandedCategorySlot.insert( std::make_pair( mit->first, iExpanded ) );
    }

    if ( kPacket_.m_iOK != NetError::NET_OK ) 
    {
        m_iED += kPacket_.m_iED;
    }
    kAck.m_iOK = kPacket_.m_iOK;
    kAck.m_iED = m_iED;

    switch( kPacket_.m_usEventID )
    {
    case EGS_EXPAND_BANK_SLOT_ED_REQ:
        {
            kLog.m_iCategory = CXSLInventory::ST_BANK;
            SendPacket( EGS_EXPAND_BANK_SLOT_ED_ACK, kAck );

            // DB통계 ED
            KStatisticsKey kKey;
            kKey.m_vecIntKey.push_back( 0 );
            KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_ExpandBank, ( kPacket_.m_iED ) );
        }
        break;
    case EGS_EXPAND_INVENTORY_ED_REQ:
        {
            kLog.m_iCategory = CXSLInventory::ST_EQUIP;
            SendPacket( EGS_EXPAND_INVENTORY_ED_ACK, kAck );

            // DB통계 ED
            KStatisticsKey kKey;
            kKey.m_vecIntKey.push_back( 0 );
            KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_ExpandInventory, ( kPacket_.m_iED ) );
        }
        break;

    default:
        {
            START_LOG( cerr, L"이상한 이벤트가 왔다?" )
                << BUILD_LOG( GetCharUID() )
                << BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
                << END_LOG;
        }
        break;
    }
    
    kLog.m_iUserUID = GetUID();
    kLog.m_iUnitUID = GetCharUID();
    kLog.m_iUnitLevel = GetLevel();
    kLog.m_iBeforeED = m_iED - kPacket_.m_iED;
    kLog.m_iUseED = kPacket_.m_iED;
    
	kLog.m_iNumSlot = CXSLInventory::SLOT_COUNT_ONE_LINE;
	SendToLogDB( ELOG_EXPAND_BANK_INVENTORY_NOT, kLog );
}

IMPL_ON_FUNC_NOPARAM( EGS_GET_NEXT_BANK_ED_REQ )
{
    // 상태 체크
    VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_NEXT_BANK_ED_ACK );
    KEGS_GET_NEXT_BANK_ED_ACK kPacket;
#ifdef SERV_CLOSE_GOOD_ELSWORD
	goto end_proc;
#endif //SERV_CLOSE_GOOD_ELSWORD
    int iMyBankGrade = m_kInventory.GetBankMemberShip();
    if ( iMyBankGrade == CXSLItem::CI_BANK_MEMBERSHIP_PLATINUM ) 
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_02 );
        goto end_proc;
    }
    
    int iNextED = m_kInventory.GetNextUpgradeBankED( CXSLInventory::GetNextBankMemberShip( m_kInventory.GetBankMemberShip() ) );
    if ( iNextED <= 0 ) 
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_03 );
        goto end_proc;

    }

    kPacket.m_iED = iNextED;
    SET_ERROR( NET_OK );
    
end_proc:
    
    kPacket.m_iOK = NetError::GetLastError();
    SendPacket( EGS_GET_NEXT_BANK_ED_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_GET_NEXT_INVENTORY_ED_REQ )
{
    // 상태 체크
    VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_NEXT_INVENTORY_ED_ACK );
    KEGS_GET_NEXT_INVENTORY_ED_ACK kPacket;
#ifdef SERV_CLOSE_GOOD_ELSWORD
	goto end_proc;
#endif //SERV_CLOSE_GOOD_ELSWORD
    if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::SLOT_COUNT_ONE_LINE ) )
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_02 );
        goto end_proc;
    }
    
    int iExtendedSlotSize = m_kInventory.GetExpandedSlotSize( CXSLInventory::ST_EQUIP );

    int iExtendedTime = iExtendedSlotSize / CXSLInventory::SLOT_COUNT_ONE_LINE;

    // 확장 요청한 인벤토리 필요 ED 가져오기
    int iNextED = m_kInventory.GetNextUpgradeInventoryED( iExtendedTime + 1 );
   
    if ( iNextED <= 0 ) 
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_03 );
        goto end_proc;

    }

    kPacket.m_iED = iNextED;
    SET_ERROR( NET_OK );

end_proc:
    kPacket.m_iOK = NetError::GetLastError();
    SendPacket( EGS_GET_NEXT_INVENTORY_ED_ACK, kPacket );
}


IMPL_ON_FUNC_NOPARAM( EGS_EXPAND_INVENTORY_ED_REQ )
{
    VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_EXPAND_INVENTORY_ED_ACK );

    KDBE_EXPAND_INVENTORY_SLOT_REQ kPacket;
    KEGS_EXPAND_INVENTORY_ED_ACK kAck;
#ifdef SERV_CLOSE_GOOD_ELSWORD
	goto end_proc;
#endif //SERV_CLOSE_GOOD_ELSWORD
    if( !m_kInventory.IsAbleToExpandSlot( CXSLInventory::SLOT_COUNT_ONE_LINE ) )
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_02 );
        goto end_proc;
    }

    int iExtendedSlotSize = m_kInventory.GetExpandedSlotSize( CXSLInventory::ST_EQUIP ); // 확장한 슬롯 수

    int iExtendedTime = iExtendedSlotSize / CXSLInventory::SLOT_COUNT_ONE_LINE; // 확장 횟수. 안했으면 0

    // 확장 요청한 인벤토리 필요 ED 가져오기
    int iNextED = m_kInventory.GetNextUpgradeInventoryED( iExtendedTime + 1 );

    if ( iNextED <= 0 ) 
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_03 );
        goto end_proc;

    }

    if ( m_iED < iNextED ) 
    {
        SET_ERROR( ERR_EXPAND_INVENTORY_ED_01 );
        goto end_proc;
    }

    m_iED -= iNextED;

    // DB로 이벤트를 보낸다.
    int nSlot = CXSLInventory::SLOT_COUNT_ONE_LINE; // * kPacket_.m_usProductPieces * kPacket_.m_usOrderQuantity;
    
    kPacket.m_usEventID = EGS_EXPAND_INVENTORY_ED_REQ;
    kPacket.m_iUnitUID = GetCharUID();
    kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_EQUIP, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_EQUIP ) ) ) );
    kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_ACCESSORY, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_ACCESSORY ) ) ) );
    kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_MATERIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_MATERIAL ) ) ) );
    kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_SPECIAL, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_SPECIAL ) ) ) );
    kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUEST, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUEST ) ) ) );
    kPacket.m_mapExpandedSlot.insert( std::make_pair( ( int )CXSLInventory::ST_QUICK_SLOT, nSlot + GetExpandedSlotSize( static_cast<int>( CXSLInventory::ST_QUICK_SLOT ) ) ) );
    kPacket.m_iED = iNextED;

    SendToGameDB( DBE_EXPAND_BANK_INVENTORY_REQ, kPacket );

    SET_ERROR( NET_OK );

end_proc:
    if ( NetError::GetLastError() != NetError::NET_OK ) 
    {
        kAck.m_iOK = NetError::GetLastError();
        SendPacket( EGS_EXPAND_INVENTORY_ED_ACK, kAck );

        START_LOG( cerr, L"인벤토리 ED 로 확장 실패 , User ID :" << GetUserID() )
            << BUILD_LOG( NetError::GetLastErrMsg() )
            << END_LOG;
    }
}

#endif // SERV_GOOD_ELSWORD

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-17	// 박세훈
IMPL_ON_FUNC( EGS_SOCKET_EXPAND_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_SOCKET_EXPAND_ITEM_REQ, EGS_SOCKET_EXPAND_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	//{{ 2009. 10. 15  최육사	거래 예외처리
	if( GetTradeUID() > 0  ||  GetPersonalShopUID() > 0 )
	{
		START_LOG( cwarn, L"거래중에는 사용할 수 없는 기능!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ITEM_14;
		SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
		return;
	}
	//}}

	//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
	if( m_kInventory.IsLocked() == true )
	{
		START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() );

		KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
		SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
		return;
	}
#endif SERV_INVENTORY_LOCK
	//}}
	//////////////////////////////////////////////////////////////////////////

	// 소켓 슬롯을 추가할 수 있는지 검사하자.
	CXSLItem::ITEM_TYPE typeItemType = CXSLItem::IT_NONE;
	KDBE_SOCKET_EXPAND_ITEM_REQ kPacketToDB;
	{
		// 대상 아이템을 가지고 있는지 확인
		KInventoryItemInfo kInfo;
		if( m_kInventory.GetInventoryItemInfo( kPacket_.m_iTargetItemUID, kInfo ) == false )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_08;	// 소켓 슬롯 추가에 필요한 대상 아이템이 존재하지 않습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		// 인벤토리에 있는 것인지 확인
		if( CXSLInventory::IsUserInventoryCategory( kInfo.m_cSlotCategory ) == false )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_09;	// 인벤토리의 아이템에만 사용할 수 있는 기능입니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		// 파괴된 아이템에는 사용할 수 없다.
		if( m_kInventory.IsBrokenItem( kPacket_.m_iTargetItemUID ) == true )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_10;	// 파괴된 아이템에는 사용할 수 없습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		// PC방 프리미엄 아이템에는 사용할 수 없다.
		if( m_kInventory.IsDBUpdateItem( kPacket_.m_iTargetItemUID ) == false )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_11;	// PC방 프리미엄 아이템에는 사용할 수 없습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}
		
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );

		// 장착 불가 아이템에는 사용할 수 없다.
		if( pItemTemplet->m_bNoEquip == true )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_12;	// 장착 불가 아이템에는 사용할 수 없습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		// 사용 가능한 부위인지 확인
		if( ( pItemTemplet->m_ItemType != CXSLItem::IT_WEAPON ) && ( pItemTemplet->m_ItemType != CXSLItem::IT_DEFENCE ) )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_13;	// 무기와 방어구 아이템에만 사용 할 수 있습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		// 아바타에는 사용할 수 없다.
		if( pItemTemplet->m_bFashion == true )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_13;	// 무기와 방어구 아이템에만 사용 할 수 있습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		// 추가 가능한 소켓 슬롯 수가 남아있는지 확인.
		if( SiCXSLFieldBossData()->GetMaximumExpandedSocketCount() <= kInfo.m_kItemInfo.m_byteExpandedSocketNum )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_14;	// 더 이상 소켓 슬롯을 추가 할 수 없습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		typeItemType						= pItemTemplet->m_ItemType;
		kPacketToDB.m_vecItemSocket			= kInfo.m_kItemInfo.m_vecItemSocket;
		kPacketToDB.m_byteExpandedSocketNum	= kInfo.m_kItemInfo.m_byteExpandedSocketNum + 1;
	}

	// 소켓 슬롯을 추가하기 위해 필요한 재료 아이템 검사
	int iMaterialItemCategory, iMaterialItemSlotID;
	{
		// 대상 아이템을 가지고 있는지 확인
		KInventoryItemInfo kInfo;
		if( m_kInventory.GetInventoryItemInfo( kPacket_.m_iMaterialItemUID, kInfo ) == false )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_15;	// 소켓 슬롯 추가에 필요한 재료 아이템이 존재하지 않습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		// 인벤토리에 있는 것인지 확인
		if( CXSLInventory::IsUserInventoryCategory( kInfo.m_cSlotCategory ) == false )
		{
			KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_FIELD_BOSS_16;	// 인벤토리의 재료 아이템만 사용할 수 있습니다.
			SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
			return;
		}

		if( typeItemType == CXSLItem::IT_WEAPON )
		{
			if( SiCXSLFieldBossData()->IsSocketExpandItemForWeapon( kInfo.m_kItemInfo.m_iItemID ) == false )
			{
				KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_FIELD_BOSS_17;	// 무기 소켓 슬롯을 추가하기 위한 재료 아이템을 가지고 있지 않습니다.
				SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
				return;
			}
		}
		else if( typeItemType == CXSLItem::IT_DEFENCE )
		{
			if( SiCXSLFieldBossData()->IsSocketExpandItemForDefence( kInfo.m_kItemInfo.m_iItemID ) == false )
			{
				KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_FIELD_BOSS_18;	// 방어구 소켓 슬롯을 추가하기 위한 재료 아이템을 가지고 있지 않습니다.
				SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
				return;
			}
		}

		iMaterialItemCategory	= kInfo.m_cSlotCategory;
		iMaterialItemSlotID		= kInfo.m_sSlotID;
	}

	int iDecreased = 0;
	m_kInventory.DecreaseQuantity( kPacket_.m_iMaterialItemUID, 1, iDecreased, KDeletedItemInfo::DR_EXPAND_SOCKET_SLOT );
	if( iDecreased != 1 )
	{
		KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_19;	// 소켓 슬롯 추가에 실패하였습니다.
		SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
		return;
	}

	KInventoryItemInfo kInventoryItemInfo_Material;
	m_kInventory.GetInventorySlotInfo( iMaterialItemCategory, iMaterialItemSlotID, kInventoryItemInfo_Material );

	kPacketToDB.m_iUnitUID				= GetCharUID();
	kPacketToDB.m_iItemUID				= kPacket_.m_iTargetItemUID;
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	kPacketToDB.m_vecUpdatedInventorySlot.push_back( kInventoryItemInfo_Material );
	SendToGameDB( DBE_SOCKET_EXPAND_ITEM_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_SOCKET_EXPAND_ITEM_ACK )
{
	KEGS_SOCKET_EXPAND_ITEM_ACK kPacket;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 추가 소켓 슬롯 정보는 처리되었는데, 수량이 변경된 아이템이나 삭제된 아이템에 대한 정보가 최신화되지 못하였다면,
		// 다음 기회에 업데이트 시도하도록 하자
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

		// 추가 소켓 슬롯 정보를 갱신할 아이템을 찾아보자
		KItemInfo kInfo;
		if( m_kInventory.UpdateExpandedSocketNum( kPacket_.m_iItemUID, kPacket_.m_byteExpandedSocketNum ) == false )
		{
			START_LOG( cerr, L"추가된 소켓 슬롯 정보를 갱신할 대상 아이템이 없습니다. 있어서는 안되는 오류입니다. 로직을 확인 해주세요." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( kPacket_.m_byteExpandedSocketNum )
				<< END_LOG;
		}
		else
		{
			int	iCategory, iSlotID;
			if( m_kInventory.GetCategorySlot( kPacket_.m_iItemUID, iCategory, iSlotID ) == false )
			{
				START_LOG( cerr, L"아이템 정보를 찾지 못하였습니다." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< END_LOG;
			}

			// 갱신된 인벤토리 정보를 클라에 전달하자
			kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

			KInventoryItemInfo kInventoryItemInfo;
			if( m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInventoryItemInfo ) == true )
			{
				kPacket.m_vecUpdatedInventorySlot.push_back( kInventoryItemInfo );
			}
			else
			{
				START_LOG( cerr, L"갱신 완료한 아이템의 정보를 찾을 수 없습니다. 이상한 문제입니다." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< END_LOG;
			}
		}
		
		kPacket.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_FIELD_BOSS_19;	// 소켓 슬롯 추가에 실패하였습니다.
	}

	SendPacket( EGS_SOCKET_EXPAND_ITEM_ACK, kPacket );
}
#endif // SERV_BATTLE_FIELD_BOSS