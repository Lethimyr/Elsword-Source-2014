#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "X2Data/XSLSquareUnit.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"

#ifdef DEF_TRADE_BOARD
	#include "PersonalShopItemList.h"
#endif DEF_TRADE_BOARD
//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	#include "GameSysVal/GameSysVal.h"
#endif SERV_BLOCK_TRADE
//}}

//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////
#define REG_CHARGE_ED_PER_ONE_ITEM 1000
#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC_NOPARAM( EGS_CREATE_PERSONAL_SHOP_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_CREATE_PERSONAL_SHOP_REQ, EGS_CREATE_PERSONAL_SHOP_ACK );

	KEGS_CREATE_PERSONAL_SHOP_ACK kPacket;

	kPacket.m_iOK = NetError::ERR_UNKNOWN;

	//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
		goto end_proc;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_03;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		goto end_proc;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		goto end_proc;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT
	
	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}

	if( GetTradeUID() != 0 )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_21;
		goto end_proc;
	}	

	if( GetPersonalShopUID() != 0 )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_22;
		goto end_proc;
	}

	//{{ 2011. 04. 11	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	if( GetPShopAgencyUID() != 0 )
	{
		_SendToLoginRoom( GetPShopAgencyUID(), ERM_CHECK_OPEN_PSHOP_AGENCY_REQ, char() );
		return;
	}
#endif SERV_PSHOP_AGENCY
	//}}

	//광장에서만 개인상점을 개설 할수있다.
	if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
	{
		//광장이라면 개설할수있는 체크한다.(광장전체인원별 개설할수있는 상점수가 남아있는지)
		SendToCnRoom( ERM_CHECK_OPEN_PERSONAL_SHOP_REQ );
		return;
	}

	kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_01;

	START_LOG( cerr, L"개인상점 생성위치가 잘못되었음.!" )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
		<< END_LOG;

end_proc:
	SendPacket( EGS_CREATE_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_CHECK_OPEN_PERSONAL_SHOP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	//광장에 상점오픈을 할수있다면 상점방하나를 생성요청한다.
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KERM_OPEN_PERSONAL_SHOP_REQ kPacket;
		kPacket.m_iUserUID = GetUID();
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		kPacket.m_iED = GetED();
#else
		kPacket.m_iED = m_iED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		
		kPacket.m_wstrNickName = GetCharName();

		//아이템 체크하는 시점을 여러곳으로 분산하면 시간차로 아이템이 사라지는 경우가 생기므로
		//개인상점방으로 갔다온다.
		if( m_kInventory.IsExistOnlyInventory( CXSLItem::CI_PREMIUM_SHOP ) == true )
#ifdef SERV_UPGRADE_TRADE_SYSTEM
			kPacket.m_cPersonalShopType = SEnum::AST_PREMIUM;
		else
			kPacket.m_cPersonalShopType = SEnum::AST_NORMAL;
#else //SERV_UPGRADE_TRADE_SYSTEM
			kPacket.m_cPersonalShopType = CXSLSquareUnit::PST_PREMIUM;
		else
			kPacket.m_cPersonalShopType = CXSLSquareUnit::PST_NORMAL;
#endif //SERV_UPGRADE_TRADE_SYSTEM

		//{{ 2011. 06. 21	최육사	거래방 생성 버그 수정
#ifdef SERV_OPEN_TRADE_ROOM_BUG_FIX
		_SendToCnRoom( 0, ERM_OPEN_PERSONAL_SHOP_REQ, kPacket );
#else
		SendToCnRoom( ERM_OPEN_PERSONAL_SHOP_REQ, kPacket );
#endif SERV_OPEN_TRADE_ROOM_BUG_FIX
		//}}
		return;
	}

	//생성 할수 없으면..x2
	KEGS_CREATE_PERSONAL_SHOP_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CREATE_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_OPEN_PERSONAL_SHOP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CREATE_PERSONAL_SHOP_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iPersonalShopUID = kPacket_.m_iPersonalShopUID;
	KSquarePersonalShopInfo kInfo;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_iPersonalShopUID = kPacket_.m_iPersonalShopUID;

		kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_ALL;

		kInfo.m_cPersonalShopState	= CXSLSquareUnit::PSS_WAIT;
		kInfo.m_iPersonalShopUID	= kPacket_.m_iPersonalShopUID;
		kInfo.m_cPersonalShopType	= kPacket_.m_cPersonalShopType;

	}
	else
	{
		kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_DATA_CLEAR;
		//kInfo.m_cPersonalShopState	= CXSLSquareUnit::PSS_NONE;
	}

	//{{ 2010. 03. 17  최육사	상태 오류 수정
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"개인상점을 개설했는데 현재 위치가 거래광장이 아니다? 있을수 없는에러!" )
			<< BUILD_LOG( GetCharUID() )			
			<< END_LOG;
	}
	else
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_SQUARE )
		{
			START_LOG( cerr, L"개인상점을 개설했는데 현재 위치가 거래광장이 아니다?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
		else
		{
			//광장에 상점정보를 업데이트한다. 
			SendToCnRoom( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, kInfo );
		}
	}
	//}}

	SendPacket( EGS_CREATE_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_REG_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_REG_PERSONAL_SHOP_ITEM_REQ, EGS_REG_PERSONAL_SHOP_ITEM_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrPersonalShopName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrPersonalShopName.c_str(), L'♡' );
#endif //SERV_STRING_FILTER_USING_DB

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_REG_PERSONAL_SHOP_ITEM_ACK kAck;

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
			SendPacket( EGS_REG_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_REG_PERSONAL_SHOP_ITEM_ACK kAck;
	KERM_REG_PERSONAL_SHOP_ITEM_REQ	kReq;
	kReq.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;

	__int64 iTotalSellPrice = 0;

	//등록하려는 물품을 인벤토리에서 체크해본다.
	BOOST_TEST_FOREACH( const KRegPersonalShopItemInfo&, kRegPShopItemInfo, kPacket_.m_vecRegItem )
	{
		KInventoryItemInfo kInfo;
		const int iOK = m_kInventory.CheckItemForTrade( kRegPShopItemInfo.m_iItemUID, kRegPShopItemInfo.m_iQuantity, kInfo );

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"등록하려는 아이템 이상" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( NetError::GetErrStr( iOK ) )
				<< BUILD_LOG( kRegPShopItemInfo.m_iItemUID )
				<< BUILD_LOG( kRegPShopItemInfo.m_iQuantity )
				<< END_LOG;

			kAck.m_iOK = iOK;
			goto end_proc;
		}

		//{{ 2011. 04. 11	최육사	거래 제한
#ifdef SERV_MAX_ED_LIMIT
		if( kRegPShopItemInfo.m_iPricePerOne > SEnum::UI_MAX_PSHOP_ED )
		{
			START_LOG( cerr, L"등록 가능한 판매ED량을 초과하였습니다. 해킹인가?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kRegPShopItemInfo.m_iItemUID )
				<< BUILD_LOG( kRegPShopItemInfo.m_iQuantity )
				<< BUILD_LOG( kRegPShopItemInfo.m_iPricePerOne )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_23;
			goto end_proc;
		}
#endif SERV_MAX_ED_LIMIT
		//}}

		kInfo.m_kItemInfo.m_iQuantity = kRegPShopItemInfo.m_iQuantity;

		//개인상점 서버에 올릴 데이터 셋팅..
		KSellPersonalShopItemInfo kSellInfo;
		//#1. 아이템 정보.
		kSellInfo.m_kInventoryItemInfo = kInfo;
		//#2. 판매가격
		kSellInfo.m_iPricePerOne = kRegPShopItemInfo.m_iPricePerOne;
		if( kSellInfo.m_iPricePerOne < 0 )
			kSellInfo.m_iPricePerOne = 0;

		//CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( kInfo.m_kItemInfo.m_iItemID );

		//#3. 판매수수료.
		//if( pItemTemplet == NULL )
		//{
		//	START_LOG( cerr, L"ItemTemplet 포인터가 이상함.!" )
		//		<< BUILD_LOG( GetUID() )
		//		<< BUILD_LOG( GetName() )
		//		<< BUILD_LOG( GetCharUID() )
		//		<< BUILD_LOG( GetCharName() )
		//		<< BUILD_LOG( kInfo.m_iItemUID )
		//		<< BUILD_LOG( kInfo.m_kItemInfo.m_iItemID )
		//		<< END_LOG;

		//	goto end_proc;
		//}
		//if( pItemTemplet->m_ItemGrade == CXSLItem::IG_ELITE || pItemTemplet->m_ItemGrade == CXSLItem::IG_UNIQUE )
		//{
		//	kSellInfo.m_iFeePerOne = static_cast<int>(pItemTemplet->m_Price * 0.2f);
		//}
		//else
		{
			kSellInfo.m_iFeePerOne = 0;
		}

		//kReq.m_mapSellItemInfo.insert( std::make_pair( kPacket_.m_vecRegItem[i].m_iItemUID, kSellInfo ) );
		kReq.m_vecSellItemInfo.push_back( kSellInfo );

		//{{ 2009. 4. 28  최육사	최대ED제한
		iTotalSellPrice += ( kRegPShopItemInfo.m_iPricePerOne * kRegPShopItemInfo.m_iQuantity );
		//}}
	}

	if( kReq.m_vecSellItemInfo.size() == 0 )
	{
		START_LOG( cerr, L"판매할려는 아이템 종류 & 수량이 잘못되었음.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kReq.m_vecSellItemInfo.size() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_06;
		goto end_proc;
	}
	
	if( GetPersonalShopUID() == 0 )
	{
		//08.02.13 hoons : 이런상황이 발생한다면 임시방편으로 개인상점 중지 패킷을 보내야 할것 같음.

		START_LOG( cerr, L"물품을 등록할려는 상점 UID가 잘못되었음.!" )
			<< BUILD_LOG( GetPersonalShopUID() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT]
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_UNKNOWN;
		goto end_proc;
	}

	//{{ 2009. 4. 28  최육사	최대 ED제한
	// 프리미엄 상점이 아닐 경우..
	if( m_kInventory.IsExistOnlyInventory( CXSLItem::CI_PREMIUM_SHOP ) == false )
	{
		// 10% 삭감
		iTotalSellPrice -= static_cast<__int64>( iTotalSellPrice * 0.1f );
	}

	if( iTotalSellPrice > static_cast<__int64>(SEnum::UI_MAX_ED_CHAR_HAVE) )
	{
		START_LOG( cerr, L"최대 판매가능한 금액을 넘어서는 상품등록임." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iTotalSellPrice )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_06;
		goto end_proc;
	}

	if( CheckMaxED( iTotalSellPrice	) == false )
	{
		START_LOG( cerr, L"최대 ED를 초과하는 판매금액임. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iTotalSellPrice )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_06;
		goto end_proc;
	}
	//}}
	
	//예외 조건이 없다면 상점방으로 등록패킷을 보냄..
	_SendToCnRoom( GetPersonalShopUID(), ERM_REG_PERSONAL_SHOP_ITEM_REQ, kReq );

	START_LOG( clog, L"개인상점 물품등록 패킷 보냄.!" );
	return;

end_proc:
	SendPacket( EGS_REG_PERSONAL_SHOP_ITEM_ACK, kAck );
}

IMPL_ON_FUNC( ERM_REG_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_REG_PERSONAL_SHOP_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		DecreaseED( kPacket_.m_iOpeningED, KUserEDManager::ER_OUT_ED_PERSONAL_SHOP_OPEN_COMMISSION );
#else
		const int iBeforeED = m_iED;

		//개인상점 개설비용 처리..
		m_iED -= kPacket_.m_iOpeningED;

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_PERSONAL_SHOP_OPEN, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		

		// 개인상점 개설 수수료 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MOpenPShop, kPacket_.m_iOpeningED );

		// 개인상점 개설 수수료 통계
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MOpenPShop, kPacket_.m_iOpeningED );
#else //SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MOpenPShopED, kPacket_.m_iOpeningED );
#endif //SERV_USER_STATISTICS_RENEWAL

		kPacket.m_iED = GetED();
		kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
		kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;		
		//GetUnitInfo( kPacket.m_kUnitInfo );

#ifdef SERV_PERSONAL_SHOP_NO_MOVE
		m_vecPersonalShopSell_ItemInfo = kPacket_.m_vecSellItemInfo;
#endif SERV_PERSONAL_SHOP_NO_MOVE

		SendPacket( EGS_REG_PERSONAL_SHOP_ITEM_ACK, kPacket );

		//{{ 2011. 09. 06  김민성	개인 상점 물품 등록 가격 비교
#ifdef SERV_CHECK_PERSONALSHOP_ITEM
		{
			bool bRet = false;
			int iItemID = 0;
			int iPricePerOne = 0;
			// 등록 한 상품 목록을 조사해 본다.
			BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kSellInfo, kPacket_.m_vecSellItemInfo )
			{
				int iPrice = SiKGameSysVal()->GetItemPrice( kSellInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

				if( iPrice <= 0 )
					continue;

				if( kSellInfo.m_iPricePerOne >= iPrice )
				{
					bRet = true;
					iItemID = kSellInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
					iPricePerOne = kSellInfo.m_iPricePerOne;
					break;
				}
			}
			if( bRet == true )
			{
				KDBE_SEND_PHONE_MSG_NOT kPacketNot;
				//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
				SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_PERSONAL_SHOP_ITEM, kPacketNot.m_vecPhoneNum );
#else
				kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-4118-3867" ) ); // 김창호 팀장님
				kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-9317-0790" ) ); // 황정희 님
				kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-5598-4163" ) ); // 김민성 님
#endif SERV_SMS_TOTAL_MANAGER
				//}}
				kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"개인상점! : UserUID(%d), UnitUID(%d), NickName(%s)" ) % GetUID() % GetCharUID() % GetCharName() );
				SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
			}
		}
#endif SERV_CHECK_PERSONALSHOP_ITEM
		//}}
		
		{
			KSquarePersonalShopInfo kInfo;
			kInfo.m_cUpdateType				= KSquarePersonalShopInfo::UT_STATE_NAME;
			kInfo.m_cPersonalShopState		= CXSLSquareUnit::PSS_SHOP;
			kInfo.m_wstrPersonalShopName	= kPacket_.m_wstrPersonalShopName;

			//{{ 2010. 03. 17  최육사	상태오류수정
			if( GetRoomUID() == 0 )
			{
				START_LOG( cerr, L"개인상점에 아이템등록 성공했는데 현재 위치가 방이 아니다?" )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			else
			{
				if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_SQUARE )
				{
					START_LOG( cerr, L"개인상점에 아이템등록 성공했는데 현재 위치가 거래광장이 아니다?" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetRoomUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< END_LOG;
				}
				else
				{
					//광장에 상점정보를 업데이트한다. 
					SendToCnRoom( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, kInfo );
				}
			}
			//}}
		}

		//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
		// 개인 상점 등록 시 DB - Server 간의 데이터 동기화를 마춰준다.
		WriteUnitInfoToDB( false );

		//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
		BingoEventDBWrite();
#endif SERV_EVENT_BINGO
		//}}

#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
		//}}

		return;
	}

	SendPacket( EGS_REG_PERSONAL_SHOP_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_JOIN_PERSONAL_SHOP_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_JOIN_PERSONAL_SHOP_REQ, EGS_JOIN_PERSONAL_SHOP_ACK );

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		KEGS_JOIN_PERSONAL_SHOP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kPacket );
		return;
	}

	//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
		KEGS_JOIN_PERSONAL_SHOP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
		SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

// 운영팀 요청 개인상점 입장 가능하도록
// 	//{{ 2011. 03. 02	최육사	개인 거래 차단
// #ifdef SERV_HACK_USER_TRADE_BLOCK
// 	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
// 	{
// 		KEGS_JOIN_PERSONAL_SHOP_ACK kPacket;
// 		kPacket.m_iOK = NetError::ERR_TRADE_00;
// 		SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kPacket );
// 		return;
// 	}
// #endif SERV_HACK_USER_TRADE_BLOCK
// 	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KEGS_JOIN_PERSONAL_SHOP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

#ifndef DEF_TRADE_BOARD
	//광장에서만 개인상점에 참여할수있음.
	if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
	{
		//광장에서 개인상점유저의 정보를 받아온다.
		SendToCnRoom( ERM_CHECK_JOIN_PERSONAL_SHOP_REQ, kPacket_ );
		return;
	}
#else
	switch( kPacket_.m_cStageKey )
	{
	case KEGS_JOIN_PERSONAL_SHOP_REQ::SK_SQUARE:
		{
			//광장에서만 개인상점에 참여할수있음.
			if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
			{
				//광장에서 개인상점유저의 정보를 받아온다.
				SendToCnRoom( ERM_CHECK_JOIN_PERSONAL_SHOP_REQ, kPacket_ );
				return;
			}
		}
		break;
	case KEGS_JOIN_PERSONAL_SHOP_REQ::SK_BOARD:
		{
			KERM_JOIN_PERSONAL_SHOP_REQ kReq;
			kReq.m_iUserUID = GetUID();
			//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
			kReq.m_iUnitUID = GetCharUID();
#endif SERV_INTEGRATION
			//}}
			kReq.m_wstrNickName = GetCharName();
			kReq.m_bIsSquare = false;
			//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
			_SendToPShopRoom( kPacket_.m_iUnitUID, ERM_JOIN_PERSONAL_SHOP_REQ, kReq );
#else
			_SendToCnRoom( kPacket_.m_iUnitUID, ERM_JOIN_PERSONAL_SHOP_REQ, kReq );
#endif SERV_PSHOP_AGENCY
			//}}			

			// 빠른 예외처리를 위해 미리 상점 UID 를 넣어둔다.
			m_iPersonalShopUID = kPacket_.m_iUnitUID;

			return;
		}
		break;
		//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	case KEGS_JOIN_PERSONAL_SHOP_REQ::SK_AGENCY:
		{
			if( GetPShopAgencyUID() == 0 )
			{
				START_LOG( cerr, L"대리 상인 개설자가 아닌데 Agency타입으로 참여할려고 시도를함.!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetName() )
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< END_LOG;

				KEGS_JOIN_PERSONAL_SHOP_ACK kPacket;
				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kPacket );
				return;
			}

			KERM_JOIN_PERSONAL_SHOP_REQ kReq;
			kReq.m_iUserUID = GetUID();
			//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
			kReq.m_iUnitUID = GetCharUID();
#endif SERV_INTEGRATION
			//}}
			kReq.m_wstrNickName = GetCharName();
			kReq.m_bIsSquare = false;
			_SendToLoginRoom( GetPShopAgencyUID(), ERM_JOIN_PERSONAL_SHOP_REQ, kReq );

			// 빠른 예외처리를 위해 미리 상점 UID 를 넣어둔다.
			m_iPersonalShopUID = GetPShopAgencyUID();

			return;
		}
		break;
#endif SERV_PSHOP_AGENCY
		//}}

	default:
		{
			START_LOG( cerr, L"스테이지 키값이상." )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPacket_.m_cStageKey )
				<< END_LOG;
			return;
		}
	}
#endif DEF_TRADE_BOARD

	START_LOG( cerr, L"광장이 아닌데 개인상점에 참여할려고 시도를함.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetName() )
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;

	KEGS_JOIN_PERSONAL_SHOP_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;
	SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_CHECK_JOIN_PERSONAL_SHOP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KERM_JOIN_PERSONAL_SHOP_REQ kReq;
		kReq.m_iUserUID = GetUID();
		//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		kReq.m_iUnitUID = GetCharUID();
#endif SERV_INTEGRATION
		//}}
		kReq.m_wstrNickName = GetCharName();
#ifdef DEF_TRADE_BOARD
		kReq.m_bIsSquare = true;
#endif DEF_TRADE_BOARD
		_SendToCnRoom( kPacket_.m_iPersonalShopUID, ERM_JOIN_PERSONAL_SHOP_REQ, kReq );

		return;
	}

	KEGS_JOIN_PERSONAL_SHOP_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kAck );
}

IMPL_ON_FUNC( ERM_JOIN_PERSONAL_SHOP_ACK )
{
#ifndef DEF_TRADE_BOARD
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif DEF_TRADE_BOARD

	KEGS_JOIN_PERSONAL_SHOP_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	//{{ 2011. 07. 19	최육사	대리상점 구매 수수료 없앤 이벤트
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	kPacket.m_bIsPShopAgency = kPacket_.m_bIsPShopAgency;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_iPersonalShopUID = kPacket_.m_iPersonalShopUID; // SetPersonalShopUID() 으로 변경하자
	}
	else
	{
		m_iPersonalShopUID = 0;
	}

	SendPacket( EGS_JOIN_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_BREAK_PERSONAL_SHOP_REQ )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( GetPersonalShopUID() == 0 )
	{
		START_LOG( cerr, L"개인상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_BREAK_PERSONAL_SHOP_ACK kAck;
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
		SendPacket( EGS_BREAK_PERSONAL_SHOP_ACK, kAck );
		return;
	}

	//{{ 2011. 05. 02	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	// 대리상점 개설자인지 판단
	if( GetPShopAgencyUID() != 0 )
	{
		START_LOG( cerr, L"대리상점이 개설자인데 왜 이 패킷으로 방을 이탈하려고 하지? 절대 일어나서는 안되는 에러!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPShopAgencyUID() )
			<< END_LOG;

		KEGS_BREAK_PERSONAL_SHOP_ACK kAck;
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
		SendPacket( EGS_BREAK_PERSONAL_SHOP_ACK, kAck );
		return;
	}
#endif SERV_PSHOP_AGENCY
	//}}

#ifdef SERV_PERSONAL_SHOP_NO_MOVE // 2012.07.04 lygan_조성욱 // 센터서버가 중간에 끊어졌을때 센터서버와의 통신이 안됨으로 일단 여기서 풀어준다
	m_vecPersonalShopSell_ItemInfo.clear();
#endif SERV_PERSONAL_SHOP_NO_MOVE

	_SendToCnRoom( GetPersonalShopUID(), ERM_BREAK_PERSONAL_SHOP_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_BREAK_PERSONAL_SHOP_ACK, KEGS_BREAK_PERSONAL_SHOP_ACK )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_iPersonalShopUID = 0;

		KSquarePersonalShopInfo kInfo;
		kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_DATA_CLEAR;
		//kInfo.m_cPersonalShopState		= CXSLSquareUnit::PSS_NONE;
		//kInfo.m_cPersonalShopType = CXSLSquareUnit::PST_NONE;

		//광장에 상점정보를 업데이트한다. 
		if( GetRoomUID() != 0 )
		{
			if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
			{
				SendToCnRoom( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, kInfo );
			}
		}
	}

	SendPacket( EGS_BREAK_PERSONAL_SHOP_ACK, kPacket_ );
}

IMPL_ON_FUNC( ERM_BREAK_PERSONAL_SHOP_NOT )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( GetPersonalShopUID() != 0 )
	{
		if( GetCharUID() == kPacket_.m_iHostUID )
		{
			KSquarePersonalShopInfo kInfo;
			kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_DATA_CLEAR;
			//kInfo.m_cPersonalShopState		= CXSLSquareUnit::PSS_NONE;
			//kInfo.m_cPersonalShopType = CXSLSquareUnit::PST_NONE;

			if( GetRoomUID() != 0 )
			{
				if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
				{
					//광장에 상점정보를 업데이트한다. 
					SendToCnRoom( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, kInfo );
				}
			}
		}

		SetPersonalShopUID( 0 );
	}
	
#ifdef SERV_PERSONAL_SHOP_NO_MOVE // 2012.07.04 lygan_조성욱 // 센터서버가 중간에 끊어졌을때 센터서버와의 통신이 안됨으로 일단 여기서 풀어준다
	m_vecPersonalShopSell_ItemInfo.clear();
#endif //SERV_PERSONAL_SHOP_NO_MOVE	

	KEGS_BREAK_PERSONAL_SHOP_NOT kPacket;
	kPacket.m_iReason = kPacket_.m_iReason;
	SendPacket( EGS_BREAK_PERSONAL_SHOP_NOT, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_LEAVE_PERSONAL_SHOP_REQ )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( GetPersonalShopUID() == 0 )
	{
		START_LOG( cerr, L"개인상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		KEGS_LEAVE_PERSONAL_SHOP_ACK kAck;
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
		SendPacket( EGS_LEAVE_PERSONAL_SHOP_ACK, kAck );
		return;
	}

	KERM_LEAVE_PERSONAL_SHOP_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;

	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	_SendToPShopRoom( GetPersonalShopUID(), ERM_LEAVE_PERSONAL_SHOP_REQ, kPacket );
#else
	_SendToCnRoom( GetPersonalShopUID(), ERM_LEAVE_PERSONAL_SHOP_REQ, kPacket );
#endif SERV_PSHOP_AGENCY
	//}}
}

_IMPL_ON_FUNC( ERM_LEAVE_PERSONAL_SHOP_ACK, KEGS_LEAVE_PERSONAL_SHOP_ACK )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_iPersonalShopUID = 0;
	}

	SendPacket( EGS_LEAVE_PERSONAL_SHOP_ACK, kPacket_ );
}

//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
IMPL_ON_FUNC( EGS_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BUY_PERSONAL_SHOP_ITEM_REQ, EGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kAck;

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
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
		if( m_kUserAbuserManager.IsTradeBlockUser() == true )
		{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kAck.m_iOK = NetError::ERR_BLOCK_TRADE_07;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_HACK_USER_TRADE_BLOCK
		//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
		{
			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_NEW_UNIT_TRADE_LIMIT

		if( GetPersonalShopUID() == 0 )
		{
			START_LOG( cerr, L"개인상점 UID 이상.!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}

		if( kPacket_.m_iItemUID <= 0 )
		{
			START_LOG( cerr, L"개인상점 ItemUID 이상.!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_25;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
		if( kPacket_.m_iItemID <= 0 )
		{
			START_LOG( cerr, L"개인상점 ItemID 이상.!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_25;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
		if( kPacket_.m_iQuantity <= 0 )
		{
			START_LOG( cerr, L"개인상점 Quantity 이상.!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kPacket_.m_iQuantity )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_25;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KERM_BUY_PERSONAL_SHOP_ITEM_REQ kPacket;

	KInventoryItemInfo kItemIn;
	kItemIn.m_iItemUID = kPacket_.m_iItemUID;
	kItemIn.m_kItemInfo.m_iItemID = kPacket_.m_iItemID;
	kItemIn.m_kItemInfo.m_iQuantity = kPacket_.m_iQuantity;

	std::vector< KInventoryItemInfo > vecItemIn;
	vecItemIn.push_back( kItemIn );

	// 살려는 아이템을 넣을 인벤토리가 있는지 검사.
	if( m_kInventory.CheckTradeItemIn( vecItemIn, kPacket.m_kItemQuantityUpdateTemp, kPacket.m_vecItemInfoTemp, CXSLRoom::RT_PERSONAL_SHOP ) == false )
	{
		START_LOG( clog, L"개인상점 아이템 구매 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
		return;
	}

	kPacket.m_iBuyItemUID = kPacket_.m_iItemUID;
	kPacket.m_iBuyItemID = kPacket_.m_iItemID;
	kPacket.m_iBuyQuantity = kPacket_.m_iQuantity;
	kPacket.m_iBuyUnitNowED = GetED();
	
	SetPersonalShopBuyItemInfo( kPacket );

	START_LOG( clog, L"개인상점 구매 처리 정보 확인" )
		<< BUILD_LOG( GetPersonalShopUID() )
		<< BUILD_LOG( kPacket.m_iBuyItemUID )
		<< BUILD_LOG( kPacket.m_iBuyItemID )
		<< BUILD_LOG( kPacket.m_iBuyQuantity )
		<< BUILD_LOG( kPacket.m_iBuyUnitNowED )
		<< END_LOG;

	WriteUnitInfoToDB( false, EGS_BUY_PERSONAL_SHOP_ITEM_REQ );

	//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	BingoEventDBWrite();
#endif SERV_EVENT_BINGO
	//}}
}
#else
IMPL_ON_FUNC( EGS_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_BUY_PERSONAL_SHOP_ITEM_REQ, EGS_BUY_PERSONAL_SHOP_ITEM_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kAck;

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
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		if( GetPersonalShopUID() == 0 )
		{
			START_LOG( cerr, L"개인상점 UID 이상.!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KERM_BUY_PERSONAL_SHOP_ITEM_REQ kPacket;

	KInventoryItemInfo kItemIn;
	kItemIn.m_iItemUID = kPacket_.m_iItemUID;
	kItemIn.m_kItemInfo.m_iItemID = kPacket_.m_iItemID;
	kItemIn.m_kItemInfo.m_iQuantity = kPacket_.m_iQuantity;

	std::vector< KInventoryItemInfo > vecItemIn;
	vecItemIn.push_back( kItemIn );

	// 살려는 아이템을 넣을 인벤토리가 있는지 검사.
	if( m_kInventory.CheckTradeItemIn( vecItemIn, kPacket.m_kItemQuantityUpdateTemp, kPacket.m_vecItemInfoTemp, CXSLRoom::RT_PERSONAL_SHOP ) == false )
	{
		START_LOG( clog, L"개인상점 아이템 구매 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_BUY_PERSONAL_SHOP_ITEM_ACK kAck;
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kAck );
		return;
	}

	kPacket.m_iBuyItemUID = kPacket_.m_iItemUID;
	kPacket.m_iBuyItemID = kPacket_.m_iItemID;
	kPacket.m_iBuyQuantity = kPacket_.m_iQuantity;
	kPacket.m_iBuyUnitNowED = m_iED;
	//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	_SendToPShopRoom( GetPersonalShopUID(), ERM_BUY_PERSONAL_SHOP_ITEM_REQ, kPacket );
#else
	_SendToCnRoom( GetPersonalShopUID(), ERM_BUY_PERSONAL_SHOP_ITEM_REQ, kPacket );
#endif SERV_PSHOP_AGENCY
	//}}
}
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}

_IMPL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_ACK, KEGS_BUY_PERSONAL_SHOP_ITEM_ACK )
{
#ifndef DEF_TRADE_BOARD
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif DEF_TRADE_BOARD

	SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
	{
		SendPacket( EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, kPacket_ );

		return;
	}

	START_LOG( cerr, L"광장이 아닌데 광장 개인상점 업데이트 정보가 옴.!" )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetRoomUID() )
		<< END_LOG;
}

IMPL_ON_FUNC( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ )
{
#ifdef DEF_TRADE_BOARD
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
#endif DEF_TRADE_BOARD

	KERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK kPacketAck;

	kPacketAck.m_iOK = NetError::NET_OK;

	kPacketAck.m_iBuyUnitUID = kPacket_.m_iBuyUnitUID;
	kPacketAck.m_iBuyItemUID = kPacket_.m_kSellItemInfo.m_iItemUID;
	kPacketAck.m_iBuyQuantity = kPacket_.m_iBuyQuantity;

	std::vector< KInventoryItemInfo > vecItemOut;
	vecItemOut.push_back( kPacket_.m_kSellItemInfo );

	if( !m_kInventory.CheckTradeItemOut( vecItemOut, kPacketAck.m_kItemQuantityUpdateTemp, CXSLRoom::RT_PERSONAL_SHOP ) )
	{
		START_LOG( cerr, L"개인상점 confirm 체크 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_kSellItemInfo.m_iItemUID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::GetLastError();
		goto end_proc;
	}

	// 판매 성공했을때 얻는 ED를 더한 보유ED값
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	kPacketAck.m_iEDTemp = ( GetED() + kPacket_.m_iTotalPrice );
#else
	kPacketAck.m_iEDTemp = ( m_iED + kPacket_.m_iTotalPrice );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	
	
end_proc:
	_SendToCnRoom( GetPersonalShopUID(), ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
}

_IMPL_ON_FUNC( ERM_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, KEGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT )
{
#ifdef DEF_TRADE_BOARD
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );	
#endif DEF_TRADE_BOARD

	if( GetPersonalShopUID() == 0 )
	{
		START_LOG( cerr, L"개인상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		return;
	}

	START_LOG( clog, L"개인상점 아이템 정보 업데이트.!" )
		<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( kPacket_.m_iItemUID )
		<< BUILD_LOG( kPacket_.m_iQuantity )
		;

	SendPacket( EGS_UPDATE_PERSONAL_SHOP_ITEM_INFO_NOT, kPacket_ );
}

//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
_IMPL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ )
{
#ifdef DEF_TRADE_BOARD
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );	
#endif DEF_TRADE_BOARD

	if( GetPersonalShopUID() == 0 )
	{
		START_LOG( cerr, L"개인상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT			
			<< BUILD_LOG( GetCharName() )
	#endif SERV_PRIVACY_AGREEMENT		
			<< END_LOG;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	LIF( IsSameServerGroupUID( kPacket_.m_iSellUnitUID ) );
	LIF( IsSameServerGroupUID( kPacket_.m_iBuyUnitUID ) );
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	std::vector< KInventoryItemInfo > vecUpdatedSlot;

	//판매자일 경우 처리..
	if( kPacket_.m_iSellUnitUID == GetCharUID() )
	{
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		SetED( GetED() + kPacket_.m_iSellUnitEDIN, KUserEDManager::ER_SET_ED_PERSONAL_SHOP_SELL_ITEM );
#else
		const int iBeforeED = m_iED;

		//판매금액 받기.
		m_iED.SetValue( m_iED + kPacket_.m_iSellUnitEDIN );

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_IN_PERSONAL_SHOP, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			// 변화한 ED 정보를 SQUARE 로 갱신 시켜 주자.
			KERM_SQUARE_UNIT_REFRESH_ED_NOT kReq;
			kReq.m_iUnitUID = GetCharUID();
			//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			kReq.m_iED = GetED();
#else
			kReq.m_iED = m_iED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}			

			SendToCnRoom( ERM_SQUARE_UNIT_REFRESH_ED_NOT, kReq );
		}
#endif SERV_ED_MONITORING_IN_GAME
		//}}

		// 개인상점 ED 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PPersonalShop, kPacket_.m_iSellUnitEDIN );
		//{{ 2009. 3. 24  최육사	개인상점 판매 수수료 통계
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MSellTaxPShop, kPacket_.m_iSellCommissionED );
		//}}

		std::vector< KInventoryItemInfo > vecUpdatedSlot;
		if( kPacket_.m_kSellItemInfo.m_iItemUID > 0 )
		{
			int iCategory;
			int iSlotID;
			LIF( m_kInventory.GetCategorySlot( kPacket_.m_kSellItemInfo.m_iItemUID, iCategory, iSlotID ) );
			LIF( kPacket_.m_kSellItemInfo.m_cSlotCategory == static_cast<char>(iCategory) );
			//LIF( kPacket_.m_kSellIntemInfoOUT.m_iSlotID == iSlotID ); 상점 개설중에 아이템의 슬롯이동이 가능하기 때문에 체크하는 의미가 없음

			KInventoryItemInfo kInfo;
			if( kPacket_.m_kSellItemInfo.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
			{
				int iDecreased;
				m_kInventory.DecreaseQuantity( kPacket_.m_kSellItemInfo.m_iItemUID, kPacket_.m_kSellItemInfo.m_kItemInfo.m_iQuantity, iDecreased, KDeletedItemInfo::DR_PERSONAL_SHOP );
				LIF( kPacket_.m_kSellItemInfo.m_kItemInfo.m_iQuantity == iDecreased );
				m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInfo );
			}
			else
			{
				m_kInventory.DeleteItem( kPacket_.m_kSellItemInfo.m_iItemUID, kInfo, KDeletedItemInfo::DR_PERSONAL_SHOP );
			}
			vecUpdatedSlot.push_back( kInfo );
		}
		// 벌써 기록이 된 정보이므로 flush 시킨다.
		KItemQuantityUpdate kUpdateTemp;
		m_kInventory.FlushQuantityChange( kUpdateTemp.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kUpdateTemp.m_vecDeleted );

		KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
		kPacketNot.m_iED = GetED();
		kPacketNot.m_vecInventorySlotInfo = vecUpdatedSlot;
		SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
		START_LOG_WITH_NAME( clog2 )
			<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() )
			<< END_LOG;

		return;
	}

	//구매자일 경우 처리..
	if( kPacket_.m_iBuyUnitUID == GetCharUID() )
	{
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		SetED( GetED() + kPacket_.m_iBuyUnitEDOUT, KUserEDManager::ER_SET_ED_PERSONAL_SHOP_BUY_ITEM );
#else
		const int iBeforeED = m_iED;

		//구매금액 빼기.
		m_iED.SetValue( m_iED + kPacket_.m_iBuyUnitEDOUT );

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_OUT_PERSONAL_SHOP, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			// 변화한 ED 정보를 SQUARE 로 갱신 시켜 주자.
			KERM_SQUARE_UNIT_REFRESH_ED_NOT kReq;
			kReq.m_iUnitUID = GetCharUID();
			//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			kReq.m_iED = GetED();
#else
			kReq.m_iED = m_iED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}			

			SendToCnRoom( ERM_SQUARE_UNIT_REFRESH_ED_NOT, kReq );
		}
#endif SERV_ED_MONITORING_IN_GAME
		//}}

		//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD

		// 개인상점 ED 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		const int iStatUnitOnlyTradeED = ::abs( kPacket_.m_iBuyUnitEDOUT - kPacket_.m_iBuyCommissionED );		// 수수료 제외
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MPersonalShop, iStatUnitOnlyTradeED );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MBuyTaxPShop, kPacket_.m_iBuyCommissionED ); // 구매 수수료

		if( kPacket_.m_bBuyUnitIsSquare )
		{
			// 일일통계 개인상점 아이템
			KStatisticsKey kKeyPT;
			kKeyPT.m_vecIntKey.push_back( kPacket_.m_kBuyItemInfo.m_kItemInfo.m_iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyPT, KStatistics::SI_ITEM_PERSONAL_SHOP, kPacket_.m_kBuyItemInfo.m_kItemInfo.m_iQuantity );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyPT, KStatistics::SI_ITEM_PERSONAL_SHOP, kPacket_.m_kBuyItemInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
		else
		{
			// 일일통계 거래 게시판 아이템
			KStatisticsKey kKeyPT;
			kKeyPT.m_vecIntKey.push_back( kPacket_.m_kBuyItemInfo.m_kItemInfo.m_iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyPT, KStatistics::SI_ITEM_TRADE_BOARD, kPacket_.m_kBuyItemInfo.m_kItemInfo.m_iQuantity );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyPT, KStatistics::SI_ITEM_TRADE_BOARD, kPacket_.m_kBuyItemInfo.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
#endif DEF_TRADE_BOARD
		//}}

		KDBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ kPacketReq;
		std::vector<KInventoryItemInfo>	vecInsertDBItem;
		std::map<UidType,KItemInfo>	mapInsertItemInfo;

		kPacketReq.m_iUnitUID = GetCharUID();

		if( kPacket_.m_kBuyItemInfo.m_iItemUID > 0 )
		{
			vecInsertDBItem.clear();
			mapInsertItemInfo.clear();

			// 구매한 아이템 넣기
			kPacket_.m_kBuyItemInfo.m_sSlotID = -1;

			vecInsertDBItem.push_back(kPacket_.m_kBuyItemInfo);

// 			START_LOG( cerr, L"구매자 인벤토리 갱신 오류 확인" )
// 				<< BUILD_LOG( vecInsertDBItem.size() )
// 				<< BUILD_LOG( kPacketReq.m_vecUpdatedInventorySlot.size() )
// 				<< END_LOG;

			// 판매자 인벤토리 정보 이므로 다지우자
			kPacketReq.m_vecUpdatedInventorySlot.clear();

			// 새로 추가된 아이템 중 중복되는 인벤토리 목록을 정리하자
			m_kInventory.FixUpInventoryItem( vecInsertDBItem, kPacketReq.m_kItemQuantityUpdate , kPacketReq.m_vecUpdatedInventorySlot );

// 			START_LOG( cerr, L"구매자 인벤토리 갱신 오류 확인" )
// 				<< BUILD_LOG( vecInsertDBItem.size() )
// 				<< BUILD_LOG( kPacketReq.m_vecUpdatedInventorySlot.size() )
// 				<< END_LOG;
			
// 			BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, kPacketReq.m_vecUpdatedInventorySlot )
// 			{
// 				START_LOG( cerr, L"인벤토리 정보 확인" )
// 					<< BUILD_LOG( kInsertItemInfo.m_iItemUID )
// 					<< BUILD_LOG( kInsertItemInfo.m_sSlotID )
// 					<< BUILD_LOG( kInsertItemInfo.m_kItemInfo.m_iQuantity )
// 					<< END_LOG;
// 			}

			// 거래로 인하여 추가된 아이템을 먼저 추가 하자
			// 실제로 들어 온 item
			BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertDBItem )
			{
				KItemInfo kInfo = kInsertItemInfo.m_kItemInfo;

				if( kInsertItemInfo.m_iItemUID <= 0 )
					continue;

				if( kInfo.m_iQuantity <= 0 )
					continue;

				mapInsertItemInfo.insert( std::make_pair( kInsertItemInfo.m_iItemUID, kInfo ) );
			}

			std::vector< KInventoryItemInfo >	vecInsertInventoryTemp;

			if( m_kInventory.InsertItem( mapInsertItemInfo, vecInsertInventoryTemp ) == false )
			{
				BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
				{
					START_LOG( cerr, L"추가가 실패했다고?!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( kInsertItemInfo.m_iItemUID )
						<< BUILD_LOG( kInsertItemInfo.m_sSlotID )
						<< BUILD_LOG( kInsertItemInfo.m_kItemInfo.m_iQuantity )
						<< END_LOG;
				}
			}

			// 추가된 아이템을 갱신 목록에 추가한다.
			BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
			{
				BOOST_TEST_FOREACH( KInventoryItemInfo&, kTempInfo, kPacketReq.m_vecUpdatedInventorySlot )
				{
					if( kInsertItemInfo.m_iItemUID <= 0 )
						continue;

					if( kInsertItemInfo.m_kItemInfo.m_iQuantity <= 0 )
						continue;

					if( kInsertItemInfo.m_iItemUID == kTempInfo.m_iItemUID )
					{
						kTempInfo.m_sSlotID = kInsertItemInfo.m_sSlotID;
						break;
					}
				}

				kPacketReq.m_vecUpdatedInventorySlot.push_back( kInsertItemInfo );
			}

			// 중복 갱신,추가 방지
			KItemQuantityUpdate kQuantityUpdate;
			m_kInventory.FlushQuantityChange( kQuantityUpdate.m_mapQuantityChange );
			m_kInventory.FlushDeletedItem( kQuantityUpdate.m_vecDeleted );

			SendToGameDB( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ, kPacketReq );
		}
		else
		{
			KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
			//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			kPacketNot.m_iED = GetED();
#else
			kPacketNot.m_iED = m_iED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
			//}}
			kPacketNot.m_vecInventorySlotInfo = vecUpdatedSlot;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
			START_LOG_WITH_NAME( clog2 )
				<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() )
				<< END_LOG;
		}
		return;
	}

	START_LOG( cerr, L"개인상점 거래를 했는데 판매자도 구매자도 아니다.?" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( kPacket_.m_iBuyUnitUID )
		<< BUILD_LOG( kPacket_.m_iSellUnitUID )
		<< END_LOG;
}
#else
IMPL_ON_FUNC( ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT )
{
#ifdef DEF_TRADE_BOARD
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	
#else
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );	
#endif DEF_TRADE_BOARD

	if( GetPersonalShopUID() == 0 )
	{
		START_LOG( cerr, L"개인상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	LIF( IsSameServerGroupUID( kPacket_.m_iSellUnitUID ) );
	LIF( IsSameServerGroupUID( kPacket_.m_iPriceUnitUID ) );
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	std::vector< KInventoryItemInfo > vecUpdatedSlot;

	//판매자일 경우 처리..
	if( kPacket_.m_iSellUnitUID == GetCharUID() )
	{
		const int iBeforeED = m_iED;

		//판매금액 받기.
		m_iED += kPacket_.m_iSellUnitEDIN;

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_IN_PERSONAL_SHOP, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}

		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			// 변화한 ED 정보를 SQUARE 로 갱신 시켜 주자.
			KERM_SQUARE_UNIT_REFRESH_ED_NOT kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_iED = m_iED;

			SendToCnRoom( ERM_SQUARE_UNIT_REFRESH_ED_NOT, kReq );
		}
#endif SERV_ED_MONITORING_IN_GAME
		//}}

		// 개인상점 ED 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PPersonalShop, kPacket_.m_iSellUnitEDIN );
		//{{ 2009. 3. 24  최육사	개인상점 판매 수수료 통계
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MSellTaxPShop, kPacket_.m_iSellCommissionED );
		//}}
#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_PPersonalShop, kPacket_.m_iSellUnitEDIN );
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MSellTaxPShop, kPacket_.m_iSellCommissionED );
#endif //SERV_USER_STATISTICS_RENEWA

		std::vector< KInventoryItemInfo > vecUpdatedSlot;
		if( kPacket_.m_kSellIntemInfoOUT.m_iItemUID > 0 )
		{
			int iCategory;
			int iSlotID;
			LIF( m_kInventory.GetCategorySlot( kPacket_.m_kSellIntemInfoOUT.m_iItemUID, iCategory, iSlotID ) );
			LIF( kPacket_.m_kSellIntemInfoOUT.m_cSlotCategory == static_cast<char>(iCategory) );
			//LIF( kPacket_.m_kSellIntemInfoOUT.m_iSlotID == iSlotID ); 상점 개설중에 아이템의 슬롯이동이 가능하기 때문에 체크하는 의미가 없음

			KInventoryItemInfo kInfo;
			if( kPacket_.m_kSellIntemInfoOUT.m_kItemInfo.m_cUsageType == CXSLItem::PT_QUANTITY )
			{
				int iDecreased;
				m_kInventory.DecreaseQuantity( kPacket_.m_kSellIntemInfoOUT.m_iItemUID, kPacket_.m_kSellIntemInfoOUT.m_kItemInfo.m_iQuantity, iDecreased, KDeletedItemInfo::DR_PERSONAL_SHOP );
				LIF( kPacket_.m_kSellIntemInfoOUT.m_kItemInfo.m_iQuantity == iDecreased );
				m_kInventory.GetInventorySlotInfo( iCategory, iSlotID, kInfo );
			}
			else
			{
				m_kInventory.DeleteItem( kPacket_.m_kSellIntemInfoOUT.m_iItemUID, kInfo, KDeletedItemInfo::DR_PERSONAL_SHOP );
			}
			vecUpdatedSlot.push_back( kInfo );
		}

		KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
		kPacketNot.m_iED = GetED();
		kPacketNot.m_vecInventorySlotInfo = vecUpdatedSlot;
		SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
		START_LOG_WITH_NAME( clog2 )
			<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );

		return;
	}

	//구매자일 경우 처리..
	if( kPacket_.m_iPriceUnitUID == GetCharUID() )
	{
		const int iBeforeED = m_iED;

		//구매금액 빼기.
		m_iED -= kPacket_.m_iPriceUnitEDOUT;

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_OUT_PERSONAL_SHOP, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}

		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_SQUARE )
		{
			// 변화한 ED 정보를 SQUARE 로 갱신 시켜 주자.
			KERM_SQUARE_UNIT_REFRESH_ED_NOT kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_iED = m_iED;

			SendToCnRoom( ERM_SQUARE_UNIT_REFRESH_ED_NOT, kReq );
		}
#endif SERV_ED_MONITORING_IN_GAME
		//}}

		//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD

		// 개인상점 ED 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MPersonalShop, ( kPacket_.m_iPriceUnitEDOUT - kPacket_.m_iPriceCommissionED ) );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MBuyTaxPShop, kPacket_.m_iPriceCommissionED ); // 구매 수수료

#ifdef SERV_USER_STATISTICS_RENEWAL
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MPersonalShop, ( kPacket_.m_iPriceUnitEDOUT - kPacket_.m_iPriceCommissionED ) );
		m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EDData, 0, KUserStatistics::US_ED_MBuyTaxPShop, kPacket_.m_iPriceCommissionED );
#endif //SERV_USER_STATISTICS_RENEWAL

		if( kPacket_.m_bIsSquare )
		{
			// 일일통계 개인상점 아이템
			KStatisticsKey kKeyPT;
			kKeyPT.m_vecIntKey.push_back( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyPT, KStatistics::SI_ITEM_PERSONAL_SHOP, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyPT, KStatistics::SI_ITEM_PERSONAL_SHOP, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
		else
		{
			// 일일통계 거래 게시판 아이템
			KStatisticsKey kKeyPT;
			kKeyPT.m_vecIntKey.push_back( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID );
			KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyPT, KStatistics::SI_ITEM_TRADE_BOARD, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
			//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
			KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyPT, KStatistics::SI_ITEM_TRADE_BOARD, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
			//}}
		}
		//#else
		//		// 개인상점 ED 통계
		//		KStatisticsKey kKey;
		//		kKey.m_vecIntKey.push_back( 0 );
		//		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MPersonalShop, kPacket_.m_iPriceUnitEDOUT );
		//
		//		// 일일통계 개인상점 아이템
		//		KStatisticsKey kKeyPT;
		//		kKeyPT.m_vecIntKey.push_back( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID );
		//		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyPT, KStatistics::SI_ITEM_PERSONAL_SHOP, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
		//		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
		//#ifdef SERV_ITEM_STATISTICS_TO_DB
		//		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyPT, KStatistics::SI_ITEM_PERSONAL_SHOP, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
		//#endif SERV_ITEM_STATISTICS_TO_DB
		//		//}}
#endif DEF_TRADE_BOARD
		//}}

		KDBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ kPacketReq;
		if( kPacket_.m_kPriceIntemInfoIN.m_iItemUID > 0 )
		{
			kPacketReq.m_iUnitUID = GetCharUID();
			std::map< int, KItemInfo > mapItem;
			mapItem.insert( std::make_pair( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo ) );
			m_kInventory.PrepareInsert( mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );
			kPacketReq.m_vecUpdatedInventorySlot.insert( kPacketReq.m_vecUpdatedInventorySlot.begin(), vecUpdatedSlot.begin(), vecUpdatedSlot.end() );

			if( !kPacketReq.m_vecItemInfo.empty() )
			{
				m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
				m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
				SendToGameDB( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ, kPacketReq );
				START_LOG_WITH_NAME( clog2 )
					<< BUILD_LOG( ( int )kPacketReq.m_vecUpdatedInventorySlot.size() );
			}
			else
			{
				KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
				kPacketNot.m_iED = m_iED;
				kPacketNot.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
				SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
				START_LOG_WITH_NAME( clog2 )
					<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
			}

			//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
			// 아이템 어뷰저
			m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), 
				KAbuserLogManager::RS_PERSONAL_SHOP,
				kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, 
				kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
#endif SERV_USER_ABUSER_MANAGER
			//}}			
		}
		else
		{
			KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
			kPacketNot.m_iED = m_iED;
			kPacketNot.m_vecInventorySlotInfo = vecUpdatedSlot;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
			START_LOG_WITH_NAME( clog2 )
				<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
		}
		return;
	}

	START_LOG( cerr, L"개인상점 거래를 했는데 판매자도 구매자도 아니다.?" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( kPacket_.m_iPriceUnitUID )
		<< BUILD_LOG( kPacket_.m_iSellUnitUID )
		<< END_LOG;
}
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}

IMPL_ON_FUNC( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_ACK )
{
	// TODO : 상태 막아야 하나..

	// DB에 추가된 아이템을 인벤토리에 넣고 유저에게 수정된 데이터를 넘겨준다.
	KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacket;

	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	kPacket.m_iED = GetED();
#else
	kPacket.m_iED = m_iED;
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	

	SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacket );
	START_LOG_WITH_NAME( clog2 )
		<< BUILD_LOG( ( int )kPacket.m_vecInventorySlotInfo.size() );
}

#ifdef DEF_TRADE_BOARD
IMPL_ON_FUNC( EGS_SEARCH_TRADE_BOARD_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_SEARCH_TRADE_BOARD_ACK kAck;

	//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
		kAck.m_iOK = NetError::NET_OK;
		kAck.m_vecItemInfo.clear();
		SendPacket( EGS_SEARCH_TRADE_BOARD_ACK, kAck );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
#ifdef SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kAck.m_iOK = NetError::ERR_BLOCK_TRADE_05;
#else // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kAck.m_iOK = NetError::NET_OK;
#endif // SERV_GM_TOOL_TRADE_BLOCK_MESSAGE
		kAck.m_vecItemInfo.clear();
		SendPacket( EGS_SEARCH_TRADE_BOARD_ACK, kAck );
		return;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kAck.m_iOK = NetError::NET_OK;
		kAck.m_vecItemInfo.clear();
		SendPacket( EGS_SEARCH_TRADE_BOARD_ACK, kAck );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	if( 2.5 > m_kTimer[TM_REFRESH_TRADE_BOARD].elapsed() )
	{
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_31;
		m_kTimer[TM_REFRESH_TRADE_BOARD].restart();
	}
	else
    {
        if ( kPacket_.m_bStrictSearchOption == false )
        {
            SiKPersonalShopItemList()->GetSearchList(kPacket_,kAck);
        }
        else
        {
            SiKPersonalShopItemList()->GetStrictSearchList(kPacket_,kAck);
        }

        // 1. 아이템 이름 길이로 정렬
        //std::unique( kAck.m_vecItemInfo.begin(), kAck.m_vecItemInfo.end() ); // 2. 중복제거

    }

	SendPacket( EGS_SEARCH_TRADE_BOARD_ACK, kAck );
}
#endif DEF_TRADE_BOARD

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 04. 11	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY

IMPL_ON_FUNC( DBE_INSERT_PERIOD_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_kUserPersonalShopManager.SetPShopAgencyExpirationDate( kPacket_.m_wstrAgencyExpirationDate );
#ifdef SERV_UPGRADE_TRADE_SYSTEM
        m_kUserPersonalShopManager.SetPShopType( kPacket_.m_cShopType );
#endif //SERV_UPGRADE_TRADE_SYSTEM
		// 현재 개설된 대리상점이 있다면 기간 정보를 업데이트 하러 가자!
		if( GetPShopAgencyUID() != 0 )
		{
			KERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT kPacketNot;
			kPacketNot.m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
			_SendToPShopRoom( GetPShopAgencyUID(), ERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT, kPacketNot );
		}
	}

	switch( kPacket_.m_usEventID )
	{
	case EGS_GET_PURCHASED_CASH_ITEM_REQ:
	case EGS_ADMIN_PSHOP_AGENCY_CHEAT_REQ:
		{
			KEGS_GET_PURCHASED_CASH_ITEM_ACK kPacket;
			kPacket.m_iOK = kPacket_.m_iOK;
			SendPacket( EGS_GET_PURCHASED_CASH_ITEM_ACK, kPacket );
		}
		break;

	case EGS_USE_ITEM_IN_INVENTORY_REQ:	
		break;
	}
	
	// 기간 정보 날리자!
	{
		KEGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT kPacketNot;
		kPacketNot.m_wstrAgencyExpirationDate = kPacket_.m_wstrAgencyExpirationDate;
        kPacketNot.m_cShopType = kPacket_.m_cShopType;
		SendPacket( EGS_CHANGE_PSHOP_AGENCY_EXPIRATION_NOT, kPacketNot );
	}	
}

IMPL_ON_FUNC( ERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	m_kUserPersonalShopManager.SetPShopAgencyUID( kPacket_.m_iPShopAgencyUID );
}

IMPL_ON_FUNC_NOPARAM( EGS_CREATE_PSHOP_AGENCY_REQ )
{	
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_PSHOP_AGENCY_REQ, EGS_CREATE_PSHOP_AGENCY_ACK );

	KEGS_CREATE_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;

	//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
		goto end_proc;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_00;
		goto end_proc;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		goto end_proc;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}

	if( GetTradeUID() != 0 )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_21;
		goto end_proc;
	}
	
	if( GetPShopAgencyUID() != 0 )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_37;
		goto end_proc;
	}

	if( GetPersonalShopUID() != 0 )
	{
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_22;
		goto end_proc;
	}
	
	// 대리 상인 개설이 가능한가?
	//if( m_kUserPersonalShopManager.IsOpenPossiblePShopAgency() == false )
	//{
	//	START_LOG( clog, L"대리상인을 개설할 수 있는 상태가 아닙니다!" )
	//		<< BUILD_LOG( GetCharName() )
	//		<< END_LOG;

	//	kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_39;
	//	goto end_proc;
	//}

	// 대리상인은 마을에서만 개설 가능합니다!
	if( GetRoomUID() == 0 )
	{
		//아이템 체크하는 시점을 여러곳으로 분산하면 시간차로 아이템이 사라지는 경우가 생기므로
		//개인상점방으로 갔다온다.

		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		WriteUnitInfoToDB( false, EGS_CREATE_PSHOP_AGENCY_REQ );

		//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
		BingoEventDBWrite();
#endif SERV_EVENT_BINGO
		//}}
//#else
//		// 대리 상인 개설하러 갑시당!
//		KERM_OPEN_PSHOP_AGENCY_REQ kPacket;
//		kPacket.m_iUserUID = GetUID();
//		kPacket.m_iED = GetED();
//		kPacket.m_wstrNickName = GetCharName();
//		kPacket.m_wstrAgencyExpirationDate = m_kUserPersonalShopManager.GetAgencyExpirationDate();
//		kPacket.m_cPersonalShopType = CXSLSquareUnit::PST_PREMIUM; // 대리상점은 기본적으로 프리미엄 효과랑 동일하다.
//		
//		SendToLoginRoom( ERM_OPEN_PSHOP_AGENCY_REQ, kPacket );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}

#ifdef SERV_UPGRADE_TRADE_SYSTEM
        if ( m_kUserPersonalShopManager.IsOpenPossiblePShopAgency() == false ) // 초기 혹은 샵 만료 상태
        {
            KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ kPacketToDB;
            kPacketToDB.m_iUnitUID = GetCharUID();
            kPacketToDB.m_sAgencyPeriod = 1;// = CXSLItem::GetPShopAgencyDays( kPacket.m_iUsedItemID );
            kPacketToDB.m_usEventID = 0;
            kPacketToDB.m_cShopType = SEnum::AST_FREE;
            SendToGameDB( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, kPacketToDB );
        }
#endif SERV_UPGRADE_TRADE_SYSTEM

		return;
	}

	kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_01;

	START_LOG( cerr, L"개인상점 생성위치가 잘못되었음.!" )
#ifndef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
		<< END_LOG;

end_proc:
	SendPacket( EGS_CREATE_PSHOP_AGENCY_ACK, kPacket );    
}

_IMPL_ON_FUNC( ERM_OPEN_PSHOP_AGENCY_ACK, KERM_OPEN_PERSONAL_SHOP_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_PSHOP_AGENCY_ACK );

	KEGS_CREATE_PSHOP_AGENCY_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{	
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CREATE_PSHOP_AGENCY_ACK, kPacket );
		return;
	}
	
	// 대리상인 개설자인 경우 UID저장
	m_kUserPersonalShopManager.SetPShopAgencyUID( kPacket_.m_iPersonalShopUID );

	// 대리상인에 입장한 상태이므로 UID저장
	LIF( kPacket_.m_iPersonalShopUID != 0 );
	SetPersonalShopUID( kPacket_.m_iPersonalShopUID );

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iPersonalShopUID = kPacket_.m_iPersonalShopUID;
	SendPacket( EGS_CREATE_PSHOP_AGENCY_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_CHECK_OPEN_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_CREATE_PERSONAL_SHOP_ACK );

	KEGS_CREATE_PERSONAL_SHOP_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CREATE_PERSONAL_SHOP_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_REG_PSHOP_AGENCY_ITEM_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REG_PSHOP_AGENCY_ITEM_REQ, EGS_REG_PSHOP_AGENCY_ITEM_ACK );

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrPersonalShopName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrPersonalShopName.c_str(), L'♡' );
#endif //SERV_STRING_FILTER_USING_DB

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_REG_PSHOP_AGENCY_ITEM_ACK kAck;

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
			SendPacket( EGS_REG_PSHOP_AGENCY_ITEM_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}
	}
	//////////////////////////////////////////////////////////////////////////

	KEGS_REG_PSHOP_AGENCY_ITEM_ACK kAck;
	KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ kPacketToDB;

	char charBuf[255] = {0};
	std::string strPersonalShopName;
	WideCharToMultiByte( CP_ACP, 0, kPacket_.m_wstrPersonalShopName.c_str(), -1, charBuf, 255, NULL, NULL );
	strPersonalShopName = charBuf;

	// 상점 이름 길이 제한
	if( strPersonalShopName.empty() || strPersonalShopName.size() > 100 )
	{
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_40;
		goto end_proc;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrPersonalShopName ) )
	{
		kAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

    int iSellItemNum = 0;
   
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    switch ( m_kUserPersonalShopManager.GetPShopType() )
    {
    case SEnum::AST_FREE:
        iSellItemNum = 3;
        break;
    case SEnum::AST_NORMAL:
        iSellItemNum = 9;
        break;
    case SEnum::AST_PREMIUM:
        iSellItemNum = SEnum::PAE_SELL_ITEM_LIMIT_NUM; // 15
        break;
    default:
        break;
    }
#endif //SERV_UPGRADE_TRADE_SYSTEM

    if( static_cast<int>(kPacket_.m_vecRegItem.size()) > iSellItemNum )
    {
        kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_09;
        START_LOG( cerr, L"대리상인 샵 타입에 따른 아이템 수 허용 초과, User : " << GetUID() )
            << BUILD_LOG( kPacket_.m_vecRegItem.size() )
            << BUILD_LOG( iSellItemNum )
#ifdef SERV_UPGRADE_TRADE_SYSTEM
            << BUILD_LOGc( m_kUserPersonalShopManager.GetPShopType() )
#endif //SERV_UPGRADE_TRADE_SYSTEM
			;

        goto end_proc;
    }

	__int64 iTotalSellPrice = 0;

	//등록하려는 물품을 인벤토리에서 체크해본다.
	BOOST_TEST_FOREACH( const KRegPersonalShopItemInfo&, kRegPShopItemInfo, kPacket_.m_vecRegItem )
	{
		KInventoryItemInfo kInfo;
		const int iOK = m_kInventory.CheckItemForTrade( kRegPShopItemInfo.m_iItemUID, kRegPShopItemInfo.m_iQuantity, kInfo );

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"등록하려는 아이템 이상" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( NetError::GetErrStr( iOK ) )
				<< BUILD_LOG( kRegPShopItemInfo.m_iItemUID )
				<< BUILD_LOG( kRegPShopItemInfo.m_iQuantity )
				<< END_LOG;

			kAck.m_iOK = iOK;
			goto end_proc;
		}

		//{{ 2011. 04. 11	최육사	거래 제한
#ifdef SERV_MAX_ED_LIMIT
		if( kRegPShopItemInfo.m_iPricePerOne > SEnum::UI_MAX_PSHOP_ED )
		{
			START_LOG( cerr, L"등록 가능한 판매ED량을 초과하였습니다. 해킹인가?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( kRegPShopItemInfo.m_iItemUID )
				<< BUILD_LOG( kRegPShopItemInfo.m_iQuantity )
				<< BUILD_LOG( kRegPShopItemInfo.m_iPricePerOne )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_23;
			goto end_proc;
		}
#endif SERV_MAX_ED_LIMIT
		//}}

		kInfo.m_kItemInfo.m_iQuantity = kRegPShopItemInfo.m_iQuantity;

		//개인상점 서버에 올릴 데이터 셋팅..
		KSellPersonalShopItemInfo kSellInfo;
		//#1. 아이템 정보.
		kSellInfo.m_kInventoryItemInfo = kInfo;
		//#2. 판매가격
		kSellInfo.m_iPricePerOne = kRegPShopItemInfo.m_iPricePerOne;
		if( kSellInfo.m_iPricePerOne < 0 )
			kSellInfo.m_iPricePerOne = 0;
		//#3. 판매수수료.
		kSellInfo.m_iFeePerOne = 0;

		kPacketToDB.m_vecSellItemInfo.push_back( kSellInfo );

		//{{ 2009. 4. 28  최육사	최대ED제한
		iTotalSellPrice += ( kRegPShopItemInfo.m_iPricePerOne * kRegPShopItemInfo.m_iQuantity );
		//}}
	}

	if( kPacketToDB.m_vecSellItemInfo.size() == 0 )
	{
		START_LOG( cerr, L"판매할려는 아이템 종류 & 수량이 잘못되었음.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacketToDB.m_vecSellItemInfo.size() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_06;
		goto end_proc;
	}
	
	if( GetPersonalShopUID() == 0  &&  GetPShopAgencyUID() == 0 )
	{
		//08.02.13 hoons : 이런상황이 발생한다면 임시방편으로 개인상점 중지 패킷을 보내야 할것 같음.

		START_LOG( cerr, L"물품을 등록할려는 상점 UID가 잘못되었음.!" )
			<< BUILD_LOG( GetPersonalShopUID() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_45;
		goto end_proc;
	}

	if( iTotalSellPrice > static_cast<__int64>(SEnum::UI_MAX_ED_CHAR_HAVE) )
	{
		START_LOG( clog, L"최대 판매가능한 금액을 넘어서는 상품등록임." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iTotalSellPrice )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_56;
		goto end_proc;
	}

	if( CheckMaxED( iTotalSellPrice	) == false )
	{
		START_LOG( clog, L"최대 ED를 초과하는 판매금액임. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iTotalSellPrice )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_56;
		goto end_proc;
	}

	if( GetPShopAgencyUID() == 0 )
	{
		START_LOG( cerr, L"대리 상인 개설도 안되어있는데 아이템 등록하려고 하네? 일어나면 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPShopAgencyUID() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_34;
		goto end_proc;
	}

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	_SendToLoginRoom( GetPShopAgencyUID(), ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_REQ, kPacketToDB );
#else
	if( m_kInventory.DeleteItemForPersonalShopAgency( kPacketToDB.m_vecSellItemInfo, kPacketToDB.m_vecUpdatedInventorySlot ) == false )
	{
		START_LOG( cerr, L"대리상인 개설 준비 실패! 위에서 이미 검사했을텐데 왜 실패했지?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacketToDB.m_wstrPersonalShopName )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_45;
		goto end_proc;
	}

	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ, kPacketToDB );		
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	return;

end_proc:
	SendPacket( EGS_REG_PSHOP_AGENCY_ITEM_ACK, kAck );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP	
_IMPL_ON_FUNC( ERM_CHECK_FOR_REG_PSHOP_AGENCY_ITEM_ACK, KEGS_REG_PSHOP_AGENCY_ITEM_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REG_PSHOP_AGENCY_ITEM_ACK );

	KEGS_REG_PSHOP_AGENCY_ITEM_ACK kAck;
	KERM_REG_PERSONAL_SHOP_ITEM_REQ kReq;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kAck.m_iOK = kPacket_.m_iOK;
		goto end_proc;
	}

	__int64 iTotalSellPrice = 0;

	kReq.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kReq.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;

	//등록하려는 물품을 인벤토리에서 체크해본다.
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellPShopItemInfo, kPacket_.m_vecSellItemInfo )
	{
		KInventoryItemInfo kInfo;
		const int iOK = m_kInventory.CheckItemForTrade( kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID, kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity, kInfo );

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"등록하려는 아이템 이상" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( NetError::GetErrStr( iOK ) )
				<< BUILD_LOG( kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< END_LOG;

			kAck.m_iOK = iOK;
			goto end_proc;
		}

		//{{ 2011. 04. 11	최육사	거래 제한
#ifdef SERV_MAX_ED_LIMIT
		if( kSellPShopItemInfo.m_iPricePerOne > SEnum::UI_MAX_PSHOP_ED )
		{
			START_LOG( cerr, L"등록 가능한 판매ED량을 초과하였습니다. 해킹인가?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
				<< BUILD_LOG( kSellPShopItemInfo.m_iPricePerOne )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_23;
			goto end_proc;
		}
#endif SERV_MAX_ED_LIMIT
		//}}

		kReq.m_vecSellItemInfo.push_back( kSellPShopItemInfo );

		//{{ 2009. 4. 28  최육사	최대ED제한
		iTotalSellPrice += ( kSellPShopItemInfo.m_iPricePerOne * kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity );
		//}}
	}

	if( kReq.m_vecSellItemInfo.size() == 0 )
	{
		START_LOG( cerr, L"판매할려는 아이템 종류 & 수량이 잘못되었음.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kReq.m_vecSellItemInfo.size() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_06;
		goto end_proc;
	}

	if( GetPersonalShopUID() == 0  &&  GetPShopAgencyUID() == 0 )
	{
		//08.02.13 hoons : 이런상황이 발생한다면 임시방편으로 개인상점 중지 패킷을 보내야 할것 같음.

		START_LOG( cerr, L"물품을 등록할려는 상점 UID가 잘못되었음.!" )
			<< BUILD_LOG( GetPersonalShopUID() )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_45;
		goto end_proc;
	}

	if( iTotalSellPrice > static_cast<__int64>(SEnum::UI_MAX_ED_CHAR_HAVE) )
	{
		START_LOG( clog, L"최대 판매가능한 금액을 넘어서는 상품등록임." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iTotalSellPrice )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_56;
		goto end_proc;
	}

	if( CheckMaxED( iTotalSellPrice	) == false )
	{
		START_LOG( clog, L"최대 ED를 초과하는 판매금액임. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( iTotalSellPrice )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_56;
		goto end_proc;
	}

	if( GetPShopAgencyUID() == 0 )
	{
		START_LOG( cerr, L"대리 상인 개설도 안되어있는데 아이템 등록하려고 하네? 일어나면 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPShopAgencyUID() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_34;
		goto end_proc;
	}

	if( m_kInventory.DeleteItemForPersonalShopAgency( kPacket_.m_vecSellItemInfo, kReq.m_vecInventorySlotInfo ) == false )
	{
		START_LOG( cerr, L"대리상인 개설 준비 실패! 위에서 이미 검사했을텐데 왜 실패했지?" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_wstrPersonalShopName )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_45;
		goto end_proc;
	}

	{
		KItemQuantityUpdate kUpdateTemp;
		m_kInventory.FlushQuantityChange( kUpdateTemp.m_mapQuantityChange );
		m_kInventory.FlushDeletedItem( kUpdateTemp.m_vecDeleted );
	}

	_SendToLoginRoom( GetPShopAgencyUID(), ERM_REG_PSHOP_AGENCY_ITEM_REQ, kReq );
	return;

end_proc:
	SendPacket( EGS_REG_PSHOP_AGENCY_ITEM_ACK, kAck );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

IMPL_ON_FUNC( DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REG_PSHOP_AGENCY_ITEM_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상인 개설 준비 DB쿼리 실패!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	}

	if( GetPShopAgencyUID() == 0 )
	{
		START_LOG( cerr, L"대리 상인이 열려있지 않은데 어떻게 이쪽으로 왔지? 버그다!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( m_kUserPersonalShopManager.IsOpenPossiblePShopAgency() )
			<< BUILD_LOG( m_kUserPersonalShopManager.GetAgencyExpirationDate() )
			<< END_LOG;

		KEGS_REG_PERSONAL_SHOP_ITEM_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PERSONAL_SHOP_34;
		SendPacket( EGS_REG_PERSONAL_SHOP_ITEM_ACK, kPacketAck );
		return;
	}

	KERM_REG_PERSONAL_SHOP_ITEM_REQ	kReq;
	kReq.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	kReq.m_vecSellItemInfo		= kPacket_.m_vecSellItemInfo;
	kReq.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	_SendToLoginRoom( GetPShopAgencyUID(), ERM_REG_PSHOP_AGENCY_ITEM_REQ, kReq );
}

_IMPL_ON_FUNC( ERM_REG_PSHOP_AGENCY_ITEM_ACK, KERM_REG_PERSONAL_SHOP_ITEM_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REG_PSHOP_AGENCY_ITEM_ACK );

	KEGS_REG_PSHOP_AGENCY_ITEM_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
        kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
        kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
		SendPacket( EGS_REG_PSHOP_AGENCY_ITEM_ACK, kPacket );
		return;
	}

	// 대리상점에 아이템 등록이 성공했다면 이탈처리!
	LIF( GetPersonalShopUID() != 0 );
	SetPersonalShopUID( 0 );

    // 등록 ED 적용하기
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    if ( m_kUserPersonalShopManager.GetPShopType() <= SEnum::AST_NORMAL )
    {
        //m_iED.SetValue( m_iED + kPacket_.m_iTotalSellEDIn );
        int iChargeED = kPacket_.m_vecAddCompleteItemInfo.size() * REG_CHARGE_ED_PER_ONE_ITEM;
        m_iED -= iChargeED;
        //DecreaseED( iChargeED, KUserEDManager::ER_SET_ED_PSHOP_AGENCY_SELL_ITEM );
    }
#endif //SERV_UPGRADE_TRADE_SYSTEM
    //

	kPacket.m_iED = GetED();
	kPacket.m_wstrPersonalShopName = kPacket_.m_wstrPersonalShopName;
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	kPacket.m_vecSellItemInfo = kPacket_.m_vecAddCompleteItemInfo;
#else
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
#endif SERV_PSHOP_AGENCY
	//}}

	SendPacket( EGS_REG_PSHOP_AGENCY_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_JOIN_MY_PSHOP_AGENCY_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_MY_PSHOP_AGENCY_REQ, EGS_JOIN_MY_PSHOP_AGENCY_ACK );

	// 체험ID 기능 제한
	if( IsGuestUser() )
	{
		KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	//{{ 2011. 02. 06	최육사	거래 차단 기능
#ifdef SERV_BLOCK_TRADE
	if( SiKGameSysVal()->IsBlockPersonalShop() == true )
	{
		KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_BLOCK_TRADE_00;
		SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}
#endif SERV_BLOCK_TRADE
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( m_kUserAbuserManager.IsTradeBlockUser() == true )
	{
		KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_00;
		SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( m_kUserAbuserManager.IsTradeBlockUnit() == true )
	{
		KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
		SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
		return;
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT
	
	// 대리상인 개설자만 요청 가능
	if( GetPShopAgencyUID() == 0 )
	{
		if( kPacket_.m_cJoinType == KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_FOR_PICK_UP )
		{
			START_LOG( cerr, L"대리상인이 개설되어있지 않은데 물품받기를 눌렀다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_52;
			SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
			return;
		}
		else
		{
			START_LOG( cerr, L"대리상인 개설자가 아닌데 join pshop을 하려고 하네?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			KEGS_JOIN_MY_PSHOP_AGENCY_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_34;
			SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK, kPacket );
			return;
		}		
	}
	
	KERM_JOIN_PERSONAL_SHOP_REQ kPacketToRoom;
	kPacketToRoom.m_iUserUID = GetUID();
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	kPacketToRoom.m_iUnitUID = GetCharUID();
#endif SERV_INTEGRATION
	//}}
	kPacketToRoom.m_wstrNickName = GetCharName();
	kPacketToRoom.m_bIsSquare = false;
	kPacketToRoom.m_cJoinType = kPacket_.m_cJoinType;
	_SendToLoginRoom( GetPShopAgencyUID(), ERM_JOIN_MY_PSHOP_AGENCY_REQ, kPacketToRoom );

//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
	// 예외처리를 위해 미리 입장체크 해둔다.
	SetPersonalShopUID( GetPShopAgencyUID() );
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}
}

_IMPL_ON_FUNC( ERM_JOIN_MY_PSHOP_AGENCY_ACK, KEGS_JOIN_MY_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_MY_PSHOP_AGENCY_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
		if( GetPShopAgencyUID() == 0 )
		{
			START_LOG( cerr, L"대리상인 개설자가 아니다?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetPersonalShopUID() )
				<< END_LOG;
		}
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}
		SetPersonalShopUID( GetPShopAgencyUID() );

        // 여기 있으면 안되지만 급해서 잠시 묻어가는 코드. 패킷 자체를 분리해야 된다. 
        if ( kPacket_.m_cJoinType == KEGS_JOIN_MY_PSHOP_AGENCY_REQ::JT_FOR_PICK_UP 
#ifdef SERV_UPGRADE_TRADE_SYSTEM
			&& m_kUserPersonalShopManager.GetPShopType() <= SEnum::AST_NORMAL 
#endif //SERV_UPGRADE_TRADE_SYSTEM			
			)
        {

            BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kSellItem, kPacket_.m_vecSellItemInfo )
            {
                kSellItem.m_iTotalSellEDIn = static_cast<int>( kSellItem.m_iTotalSellEDIn * 0.9 );
            }

        }
	}
	else
	{
		SetPersonalShopUID( 0 );
	}

	SendPacket( EGS_JOIN_MY_PSHOP_AGENCY_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_LEAVE_PSHOP_AGENCY_REQ )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( GetPersonalShopUID() == 0  ||  GetPShopAgencyUID() == 0 )
	{
		START_LOG( cerr, L"대리상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( GetPersonalShopUID() )
			<< BUILD_LOG( GetPShopAgencyUID() )
			<< END_LOG;

		KEGS_LEAVE_PSHOP_AGENCY_ACK kAck;
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_12;
		SendPacket( EGS_LEAVE_PSHOP_AGENCY_ACK, kAck );
		return;
	}

	KERM_LEAVE_PERSONAL_SHOP_REQ kPacket;
	kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
	_SendToLoginRoom( GetPersonalShopUID(), ERM_LEAVE_PSHOP_AGENCY_REQ, kPacket );
}

_IMPL_ON_FUNC( ERM_LEAVE_PSHOP_AGENCY_ACK, KEGS_LEAVE_PSHOP_AGENCY_ACK )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetPersonalShopUID( 0 );

		//KSquarePersonalShopInfo kInfo;
		//kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_DATA_CLEAR;
		//kInfo.m_cPersonalShopState		= CXSLSquareUnit::PSS_NONE;
		//kInfo.m_cPersonalShopType = CXSLSquareUnit::PST_NONE;

		//광장에 상점정보를 업데이트한다. 
		//SendToCnRoom( ERM_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT, kInfo );
	}

	SendPacket( EGS_LEAVE_PSHOP_AGENCY_ACK, kPacket_ );
}

IMPL_ON_FUNC( ERM_BREAK_PSHOP_AGENCY_NOT )
{
	if( GetStateID() == KGSFSM::S_INIT ||
		GetStateID() == KGSFSM::S_UID_UPDATED ||
		GetStateID() == KGSFSM::S_LOGINED ||
		GetStateID() == KGSFSM::S_SERVER_SELECT )
	{
		START_LOG( cerr, L"잘못된 상태!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	if( GetPersonalShopUID() != 0 )
	{
		if( GetCharUID() != kPacket_.m_iHostUID )
		{
			KEGS_BREAK_PERSONAL_SHOP_NOT kPacket;
			kPacket.m_iReason = kPacket_.m_iReason;
			SendPacket( EGS_BREAK_PERSONAL_SHOP_NOT, kPacket );
		}

		SetPersonalShopUID( 0 );
	}

	if( GetPShopAgencyUID() != 0 )
	{
		if( GetCharUID() == kPacket_.m_iHostUID )
		{
			m_kUserPersonalShopManager.SetPShopAgencyUID( 0 );
            KEGS_BREAK_PERSONAL_SHOP_NOT kPacket;
            kPacket.m_iReason = kPacket_.m_iReason;
            SendPacket( EGS_BREAK_PERSONAL_SHOP_NOT, kPacket );
		}
	}
}

_IMPL_ON_FUNC( ELG_PSHOP_AGENCY_MESSAGE_NOT, KEGS_PSHOP_AGENCY_MESSAGE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

    SendPacket( EGS_PSHOP_AGENCY_MESSAGE_NOT, kPacket_ );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
_IMPL_ON_FUNC( ERM_BUY_PSHOP_AGENCY_ITEM_COMPLETE_NOT, KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPersonalShopUID() == 0 )
	{
		START_LOG( cerr, L"개인상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	LIF( IsSameServerGroupUID( kPacket_.m_iPriceUnitUID ) );
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	std::vector< KInventoryItemInfo > vecUpdatedSlot;

	//구매자일 경우 처리..
	if( kPacket_.m_iPriceUnitUID == GetCharUID() )
	{
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//구매금액 빼기.
		SetED( GetED() + kPacket_.m_iPriceUnitEDOUT, KUserEDManager::ER_SET_ED_PSHOP_AGENCY_BUY_ITEM );
#else
		const int iBeforeED = m_iED;

		//구매금액 빼기.
		m_iED.SetValue( m_iED + kPacket_.m_iPriceUnitEDOUT );

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
		//#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_OUT_PSHOP_AGENCY, iBeforeED );
		//#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}		

		// 대리상점 ED 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		const int iStatUnitOnlyTradeED = ::abs( kPacket_.m_iPriceUnitEDOUT - kPacket_.m_iPriceCommissionED );		// 수수료 제외
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MPShopAgency, ( iStatUnitOnlyTradeED ) );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MBuyTaxPShopAgency, kPacket_.m_iPriceCommissionED ); // 구매 수수료

		// 일일통계 개인상점 아이템
		KStatisticsKey kKeyPT;
		kKeyPT.m_vecIntKey.push_back( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyPT, KStatistics::SI_ITEM_BUY_PSHOP_AGENCY, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyPT, KStatistics::SI_ITEM_BUY_PSHOP_AGENCY, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}

		KDBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ kPacketReq;
		std::vector<KInventoryItemInfo>	vecInsertDBItem;
		std::map<UidType,KItemInfo>	mapInsertItemInfo;

		if( kPacket_.m_kPriceIntemInfoIN.m_iItemUID > 0 )
		{
			//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
			// 아이템 어뷰저
			m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), 
				KAbuserLogManager::RS_PERSONAL_SHOP,
				kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, 
				kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
#endif SERV_USER_ABUSER_MANAGER
			//}}

			vecInsertDBItem.clear();
			mapInsertItemInfo.clear();

			// 구매한 아이템 넣기
			kPacket_.m_kPriceIntemInfoIN.m_sSlotID = -1;

			vecInsertDBItem.push_back(kPacket_.m_kPriceIntemInfoIN);

			// 판매자 인벤토리 정보 이므로 다지우자
			kPacketReq.m_vecUpdatedInventorySlot.clear();

			// 새로 추가된 아이템 중 중복되는 인벤토리 목록을 정리하자
			m_kInventory.FixUpInventoryItem( vecInsertDBItem, kPacketReq.m_kItemQuantityUpdate , kPacketReq.m_vecUpdatedInventorySlot );

			// 거래로 인하여 추가된 아이템을 먼저 추가 하자
			// 실제로 들어 온 item
			BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertDBItem )
			{
				KItemInfo kInfo = kInsertItemInfo.m_kItemInfo;

				if( kInsertItemInfo.m_iItemUID <= 0 )
					continue;

				if( kInfo.m_iQuantity <= 0 )
					continue;

				mapInsertItemInfo.insert( std::make_pair( kInsertItemInfo.m_iItemUID, kInfo ) );
			}

			std::vector< KInventoryItemInfo >	vecInsertInventoryTemp;

			if( m_kInventory.InsertItem( mapInsertItemInfo, vecInsertInventoryTemp ) == false )
			{
				BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
				{
					START_LOG( cerr, L"추가가 실패했다고?!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( kInsertItemInfo.m_iItemUID )
						<< BUILD_LOG( kInsertItemInfo.m_sSlotID )
						<< BUILD_LOG( kInsertItemInfo.m_kItemInfo.m_iQuantity )
						<< END_LOG;
				}
			}

			// 추가된 아이템을 갱신 목록에 추가한다.
			BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
			{
				BOOST_TEST_FOREACH( KInventoryItemInfo&, kTempInfo, kPacketReq.m_vecUpdatedInventorySlot )
				{
					if( kInsertItemInfo.m_iItemUID <= 0 )
						continue;

					if( kInsertItemInfo.m_kItemInfo.m_iQuantity <= 0 )
						continue;

					if( kInsertItemInfo.m_iItemUID == kTempInfo.m_iItemUID )
					{
						kTempInfo.m_sSlotID = kInsertItemInfo.m_sSlotID;
						break;
					}
				}

				kPacketReq.m_vecUpdatedInventorySlot.push_back( kInsertItemInfo );
			}

			kPacketReq.m_iUnitUID = GetCharUID();
			
			// 중복 갱신,추가 방지
			KItemQuantityUpdate kQuantityUpdate;
			m_kInventory.FlushQuantityChange( kQuantityUpdate.m_mapQuantityChange );
			m_kInventory.FlushDeletedItem( kQuantityUpdate.m_vecDeleted );

			SendToGameDB( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ, kPacketReq );
		}
		else
		{
			KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
			kPacketNot.m_iED = GetED();
			kPacketNot.m_vecInventorySlotInfo = vecUpdatedSlot;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
			START_LOG_WITH_NAME( clog2 )
				<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
		}
		return;
	}

	START_LOG( cerr, L"개인상점 거래를 했는데 판매자도 구매자도 아니다.?" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( kPacket_.m_iPriceUnitUID )
		<< BUILD_LOG( kPacket_.m_iSellUnitUID )
		<< END_LOG;
}
#else
_IMPL_ON_FUNC( ERM_BUY_PSHOP_AGENCY_ITEM_COMPLETE_NOT, KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPersonalShopUID() == 0 )
	{
		START_LOG( cerr, L"개인상점 UID 이상.!" )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	LIF( IsSameServerGroupUID( kPacket_.m_iPriceUnitUID ) );
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	std::vector< KInventoryItemInfo > vecUpdatedSlot;

	//구매자일 경우 처리..
	if( kPacket_.m_iPriceUnitUID == GetCharUID() )
	{
		const int iBeforeED = m_iED;

		//구매금액 빼기.
		m_iED -= kPacket_.m_iPriceUnitEDOUT;

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_OUT_PSHOP_AGENCY, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}

		// 대리상점 ED 통계
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( 0 );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MPShopAgency, ( kPacket_.m_iPriceUnitEDOUT - kPacket_.m_iPriceCommissionED ) );
		KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_MBuyTaxPShopAgency, kPacket_.m_iPriceCommissionED ); // 구매 수수료

		// 일일통계 개인상점 아이템
		KStatisticsKey kKeyPT;
		kKeyPT.m_vecIntKey.push_back( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKeyPT, KStatistics::SI_ITEM_BUY_PSHOP_AGENCY, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKeyPT, KStatistics::SI_ITEM_BUY_PSHOP_AGENCY, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}

		KDBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ kPacketReq;
		if( kPacket_.m_kPriceIntemInfoIN.m_iItemUID > 0 )
		{
			kPacketReq.m_iUnitUID = GetCharUID();
			std::map< int, KItemInfo > mapItem;
			mapItem.insert( std::make_pair( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, kPacket_.m_kPriceIntemInfoIN.m_kItemInfo ) );
			m_kInventory.PrepareInsert( mapItem, kPacketReq.m_mapInsertedItem, kPacketReq.m_vecUpdatedInventorySlot, kPacketReq.m_vecItemInfo );
			kPacketReq.m_vecUpdatedInventorySlot.insert( kPacketReq.m_vecUpdatedInventorySlot.begin(), vecUpdatedSlot.begin(), vecUpdatedSlot.end() );

			if( !kPacketReq.m_vecItemInfo.empty() )
			{
				m_kInventory.FlushQuantityChange( kPacketReq.m_kItemQuantityUpdate.m_mapQuantityChange );
				m_kInventory.FlushDeletedItem( kPacketReq.m_kItemQuantityUpdate.m_vecDeleted );
				SendToGameDB( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ, kPacketReq );
				START_LOG_WITH_NAME( clog2 )
					<< BUILD_LOG( ( int )kPacketReq.m_vecUpdatedInventorySlot.size() );
			}
			else
			{
				KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
				kPacketNot.m_iED = GetED();
				kPacketNot.m_vecInventorySlotInfo = kPacketReq.m_vecUpdatedInventorySlot;
				SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
				START_LOG_WITH_NAME( clog2 )
					<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
			}

			//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
			// 아이템 어뷰저
			m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), 
				KAbuserLogManager::RS_PERSONAL_SHOP,
				kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iItemID, 
				kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.m_iQuantity );
#endif SERV_USER_ABUSER_MANAGER
			//}}
		}
		else
		{
			KEGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT kPacketNot;
			kPacketNot.m_iED = GetED();
			kPacketNot.m_vecInventorySlotInfo = vecUpdatedSlot;
			SendPacket( EGS_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, kPacketNot );
			START_LOG_WITH_NAME( clog2 )
				<< BUILD_LOG( ( int )kPacketNot.m_vecInventorySlotInfo.size() );
		}
		return;
	}

	START_LOG( cerr, L"개인상점 거래를 했는데 판매자도 구매자도 아니다.?" )
		<< BUILD_LOG( GetCharUID() )
		<< BUILD_LOG( kPacket_.m_iPriceUnitUID )
		<< BUILD_LOG( kPacket_.m_iSellUnitUID )
		<< END_LOG;
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

IMPL_ON_FUNC_NOPARAM( EGS_STOP_SALE_PSHOP_AGENCY_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_STOP_SALE_PSHOP_AGENCY_REQ, EGS_STOP_SALE_PSHOP_AGENCY_ACK );

	if( GetPShopAgencyUID() == 0 )
	{
		START_LOG( cerr, L"대리 상인이 개설되어있지 않은데 판매 중지 요청이 들어왔다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_STOP_SALE_PSHOP_AGENCY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_34;
		SendPacket( EGS_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
		return;
	}

	_SendToLoginRoom( GetPShopAgencyUID(), ERM_STOP_SALE_PSHOP_AGENCY_REQ, char() );
}

_IMPL_ON_FUNC( ERM_STOP_SALE_PSHOP_AGENCY_ACK, KEGS_STOP_SALE_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_STOP_SALE_PSHOP_AGENCY_ACK );

    SendPacket( EGS_STOP_SALE_PSHOP_AGENCY_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ, EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kAck;

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
			SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		if( GetPShopAgencyUID() == 0 )
		{
			START_LOG( cerr, L"대리 상인이 개설되어있지 않은데 물품 가져오기 요청이 들어왔다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_vecPickUpItemList.size() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_34;
			SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	SetAgencyShopPickUpItemInfo( kPacket_.m_vecPickUpItemList );

	WriteUnitInfoToDB( false, EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ );

	//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	BingoEventDBWrite();
#endif SERV_EVENT_BINGO
	//}}
#else
	_SendToLoginRoom( GetPShopAgencyUID(), ERM_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacket_ );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
}

IMPL_ON_FUNC( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	// 현재 보유 가능한 ED검사
	const __int64 iCurHaveED = GetED();
	const __int64 iAddED = static_cast<__int64>(kPacket_.m_iTotalAddED);
	const __int64 iCheckTotalED = iCurHaveED + iAddED;

	if( iCheckTotalED < 0  ||  iCheckTotalED > SEnum::UI_MAX_ED_CHAR_HAVE )
	{
		START_LOG( cwarn, L"보유 가능한 ED를 넘어섭니다. 가져오기 실패!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetED() )
			<< BUILD_LOG( iAddED )
			<< BUILD_LOG( iCheckTotalED )
			<< END_LOG;

		KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PERSONAL_SHOP_50;
		SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacketAck );
		return;
	}

	// 현재 인벤토리에 추가 가능한지 검사
	if( m_kInventory.IsEnoughSpaceExist( kPacket_.m_mapPickUpItemInfo ) == false )
	{
		START_LOG( cwarn, L"빈 슬롯이 부족하여 가져오기 실패!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_mapPickUpItemInfo.size() )
			<< END_LOG;

		KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_PERSONAL_SHOP_51;
		SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacketAck );
		return;
	}

	KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacketAck;
	kPacketAck.m_vecPickUpItemList = kPacket_.m_vecPickUpItemList;
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    kPacketAck.m_cShopType = m_kUserPersonalShopManager.GetPShopType();
#endif //SERV_UPGRADE_TRADE_SYSTEM
	_SendToLoginRoom( GetPShopAgencyUID(), ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacketAck );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
IMPL_ON_FUNC( ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacketAck );
		return;
	}

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	// Pick Up성공한 ED를 더하자!
	SetED( GetED() + kPacket_.m_iTotalSellEDIn, KUserEDManager::ER_SET_ED_PSHOP_AGENCY_SELL_ITEM );
#else
	const int iBeforeED = m_iED;

	// Pick Up성공한 ED를 더하자!
	m_iED.SetValue( m_iED + kPacket_.m_iTotalSellEDIn );

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
	//#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_PSHOP_PICK_UP, iBeforeED );
	//#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}		

	// 개인상점 ED 통계
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PPShopPickUp, kPacket_.m_iTotalSellEDIn );

	// Pick Up성공한 아이템들을 인벤토리에 넣자!
	KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketReq;
	std::vector<KInventoryItemInfo>	vecInsertDBItem;
	std::map<UidType,KItemInfo>	mapInsertItemInfo;
	std::map<int,int>	mapInsertItemList;

	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
    kPacketReq.m_bRemainSellItem = kPacket_.m_bRemainSellItem;

	vecInsertDBItem.clear();
	mapInsertItemInfo.clear();

	// 구매한 아이템 넣기
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kPickUpItemInfo, kPacket_.m_vecPickUpSuccessItemList )
	{
		if( kPickUpItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
		{
			continue;
		}

		kPickUpItemInfo.m_kInventoryItemInfo.m_sSlotID = -1;
		vecInsertDBItem.push_back( kPickUpItemInfo.m_kInventoryItemInfo );
		// 통계데이터 용
		mapInsertItemList.insert( std::make_pair( kPickUpItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID, kPickUpItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity ) );
	}

	// 판매자 인벤토리 정보 이므로 다지우자
	kPacketReq.m_vecUpdatedInventorySlot.clear();

	// 새로 추가된 아이템 중 중복되는 인벤토리 목록을 정리하자
	m_kInventory.FixUpInventoryItem( vecInsertDBItem, kPacketReq.m_kItemQuantityUpdate , kPacketReq.m_vecUpdatedInventorySlot );

	// 거래로 인하여 추가된 아이템을 먼저 추가 하자
	// 실제로 들어 온 item
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertDBItem )
	{
		KItemInfo kInfo = kInsertItemInfo.m_kItemInfo;

		if( kInsertItemInfo.m_iItemUID <= 0 )
			continue;

		if( kInfo.m_iQuantity <= 0 )
			continue;

		mapInsertItemInfo.insert( std::make_pair( kInsertItemInfo.m_iItemUID, kInfo ) );
	}

	std::vector< KInventoryItemInfo >	vecInsertInventoryTemp;

	if( m_kInventory.InsertItem( mapInsertItemInfo, vecInsertInventoryTemp ) == false )
	{
		BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
		{
			START_LOG( cerr, L"추가가 실패했다고?!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kInsertItemInfo.m_iItemUID )
				<< BUILD_LOG( kInsertItemInfo.m_sSlotID )
				<< BUILD_LOG( kInsertItemInfo.m_kItemInfo.m_iQuantity )
				<< END_LOG;
		}
	}

	// 추가된 아이템을 갱신 목록에 추가한다.
	BOOST_TEST_FOREACH( const KInventoryItemInfo&, kInsertItemInfo, vecInsertInventoryTemp )
	{
		BOOST_TEST_FOREACH( KInventoryItemInfo&, kTempInfo, kPacketReq.m_vecUpdatedInventorySlot )
		{
			if( kInsertItemInfo.m_iItemUID <= 0 )
				continue;

			if( kInsertItemInfo.m_kItemInfo.m_iQuantity <= 0 )
				continue;

			if( kInsertItemInfo.m_iItemUID == kTempInfo.m_iItemUID )
			{
				kTempInfo.m_sSlotID = kInsertItemInfo.m_sSlotID;
				break;
			}
		}

		kPacketReq.m_vecUpdatedInventorySlot.push_back( kInsertItemInfo );
	}

	// 중복 갱신,추가 방지
	KItemQuantityUpdate kQuantityUpdate;
	m_kInventory.FlushQuantityChange( kQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketReq );


	//////////////////////////////////////////////////////////////////////////
	// 통계 : Character Info, Item
	// 동전일 경우, DB 통계 카운트 에서 제외
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertItemList.begin(); mit != mapInsertItemList.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_PICK_UP_PSHOP, mit->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_PICK_UP_PSHOP, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_PICK_UP_PSHOP, mapInsertItemList );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#else
IMPL_ON_FUNC( ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacketAck );
		return;
	}

	const int iBeforeED = m_iED;

	// Pick Up성공한 ED를 더하자!
    if ( m_kUserPersonalShopManager.GetPShopType() <= SEnum::AST_NORMAL ) 
    {
        m_iED += kPacket_.m_iTotalSellEDIn * 0.9;
    }
    else 
    {
        m_iED += kPacket_.m_iTotalSellEDIn;
    }
	

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_PSHOP_PICK_UP, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
	//}}

	// 개인상점 ED 통계
	KStatisticsKey kKey;
	kKey.m_vecIntKey.push_back( 0 );
	KSIManager.IncreaseCount( KStatistics::SI_ED, kKey, KStatistics::eSIColDB_ED_PPShopPickUp, kPacket_.m_iTotalSellEDIn );

	// Pick Up성공한 아이템들을 인벤토리에 넣자!
	KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacketToDB;
	std::map< int, KItemInfo > mapGetItem;
	std::map< int, int > mapInsertedItem;

	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kPickUpItemInfo, kPacket_.m_vecPickUpSuccessItemList )
	{
		const int iItemID = kPickUpItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID;
		const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
		if( pItemTemplet == NULL )
		{
			START_LOG( cerr, L"아이템 템플릿을 얻지 못함. 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}

		// 수량검사
		if( kPickUpItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity <= 0 )
			continue;

		// ItemID가 중복되는것은 존재하지 않는다!
		std::pair< std::map< int, KItemInfo >::const_iterator, bool > pairResult;
		pairResult = mapGetItem.insert( std::make_pair( iItemID, kPickUpItemInfo.m_kInventoryItemInfo.m_kItemInfo ) );

		if( pairResult.second == false )
		{
			START_LOG( cerr, L"중복되는 ItemID가 존재합니다! 절대로 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
		}
	}

	LIF( m_kInventory.PrepareInsert( mapGetItem, mapInsertedItem, kPacketToDB.m_vecUpdatedInventorySlot, kPacketToDB.m_vecItemInfo ) == true );

	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_mapInsertItem = mapInsertedItem;
	kPacketToDB.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
	m_kInventory.FlushQuantityChange( kPacketToDB.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacketToDB.m_kItemQuantityUpdate.m_vecDeleted );
	SendToGameDB( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacketToDB );

	//////////////////////////////////////////////////////////////////////////
	// 통계 : Character Info, Item
	// 동전일 경우, DB 통계 카운트 에서 제외
	std::map< int, int >::const_iterator mit;
	for ( mit = mapInsertedItem.begin(); mit != mapInsertedItem.end(); ++mit )
	{
		KStatisticsKey kKey;
		kKey.m_vecIntKey.push_back( mit->first );
		KSIManager.IncreaseCount( KStatistics::SI_LOC_ITEM, kKey, KStatistics::SI_ITEM_PICK_UP_PSHOP, mit->second );
		//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
		KSIManager.IncreaseCount( KStatistics::SI_ITEM_DB, kKey, KStatistics::SI_ITEM_PICK_UP_PSHOP, mit->second );
#endif SERV_ITEM_STATISTICS_TO_DB
		//}}
	}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// 아이템 어뷰저
	m_kUserAbuserManager.CheckItemAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::RS_PICK_UP_PSHOP, mapInsertedItem );
#endif SERV_USER_ABUSER_MANAGER
	//}}
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
IMPL_ON_FUNC( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리상인에서 가져온 아이템 인벤에 넣기 실패! 절대로 발생하면 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iED = GetED();
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
    kPacket.m_bRemainSellItem = kPacket_.m_bRemainSellItem;
	SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
}
#else
IMPL_ON_FUNC( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리상인에서 가져온 아이템 인벤에 넣기 실패! 절대로 발생하면 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	KEGS_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iED = GetED();
	kPacket.m_vecSellItemInfo = kPacket_.m_vecSellItemInfo;
	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.InsertItem( kPacket_.m_mapItemInfo, kPacket.m_vecInventorySlotInfo );
	kPacket.m_vecInventorySlotInfo.insert( kPacket.m_vecInventorySlotInfo.begin(), kPacket_.m_vecUpdatedInventorySlot.begin(), kPacket_.m_vecUpdatedInventorySlot.end() );
	SendPacket( EGS_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
IMPL_ON_FUNC( ERM_CHECK_ZOMBIE_USER_REQ )
{
    VERIFY_STATE_WARN( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	switch( CXSLRoom::GetRoomType( kPacket_.m_iRoomUID ) )
	{
	case CXSLRoom::RT_PSHOP_AGENCY:
		{
			// 현재 상점에 입장해 있는 상태가 아니라면?
			if( GetPersonalShopUID() == 0 )
			{
				KERM_CHECK_ZOMBIE_USER_ACK kPacket;
				kPacket.m_cZombieCheckType = KERM_CHECK_ZOMBIE_USER_ACK::ZCT_CONNECT_ZOMBIE;
				_SendToLoginRoom( kPacket_.m_iRoomUID, ERM_CHECK_ZOMBIE_USER_ACK, kPacket );
				break;
			}
			else
			{
				// 상점에 입장은 했는데 다른곳에 입장했다면?
				if( GetPersonalShopUID() != kPacket_.m_iRoomUID )
				{
					KERM_CHECK_ZOMBIE_USER_ACK kPacket;
					kPacket.m_cZombieCheckType = KERM_CHECK_ZOMBIE_USER_ACK::ZCT_JOIN_DIF_ROOM;
					_SendToLoginRoom( kPacket_.m_iRoomUID, ERM_CHECK_ZOMBIE_USER_ACK, kPacket );
					break;
				}
			}
		}
		break;

	default:
		{
            START_LOG( cerr, L"이런 방 타입의 좀비 체크 패킷은 보낸적이 없을텐데?" )
				<< BUILD_LOG( kPacket_.m_iRoomUID )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;
		}
		break;
	}	
}
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


