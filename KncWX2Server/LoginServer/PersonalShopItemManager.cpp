#include "LoginServer.h"
#include ".\PersonalShopItemManager.h"
#include "X2Data/XSLInventory.h"
#include "X2Data/XSLItem.h"
#include "X2Data/XSLSquareUnit.h"
#include "Enum/Enum.h"

#include "NetError.h"


//{{ 2011. 04. 20	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY


KPersonalShopItemManager::KPersonalShopItemManager(void)
{
}

KPersonalShopItemManager::~KPersonalShopItemManager(void)
{
	Clear();
}

void KPersonalShopItemManager::Clear()
{
	m_vecSellItemInfo.clear();
	m_mapSellItemInfo.clear();
	m_mapSoldPShopAgencyItemInfo.clear();
}

bool KPersonalShopItemManager::IsExistItem( IN const UidType iItemUID ) const
{
    return ( m_mapSellItemInfo.find( iItemUID ) != m_mapSellItemInfo.end() );
}

//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
//////////////////////////////////////////////////////////////////////////
void KPersonalShopItemManager::GetSellPersonalShopItemInfoList( OUT std::vector< KSellPersonalShopItemInfo >& vecItemList, IN const ITEM_LIST_TYPE eType /*= ITEM_LIST_TYPE::ILT_ONLY_ON_SALE_ITEMS*/ )
{
	vecItemList.clear();

	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템 정보가 이상합니다!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"대리상인 전용 ItemUID가 아닙니다! 절대 일어나서는 안되는 오류!" )
				<< BUILD_LOG( spItemInfo->GetItemUID() )
				<< END_LOG;
			continue;
		}

		switch( eType )
		{
		case ILT_ALL_ITEMS:
			break;

		case ILT_ONLY_ON_SALE_ITEMS:
			{
				// 다 팔린것은 리스트에 담지 않는다!
				if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SOLD_OUT  ||
					spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
					continue;
			}
			break;

		case ILT_ONLY_SOLD_OUT_ITEMS:
			{
				// 덜 팔린것은 리스트에 담지 않는다!
				if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SALE  ||
					spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PROCESSING )
					continue;
			}
			break;

		default:
			{
				START_LOG( cerr, L"물품 리스트 요청 타입이 이상합니다!" )
					<< BUILD_LOG( eType )
					<< END_LOG;
			}
			return;
		}

		KSellPersonalShopItemInfo kInfo;
		spItemInfo->GetSellPersonalShopItemInfo( kInfo );
		vecItemList.push_back( kInfo );
	}
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
void KPersonalShopItemManager::GetSellPersonalShopItemInfoList( OUT std::vector< KSellPersonalShopItemInfo >& vecItemList, IN const bool bAllItems /*= false*/ )
{
	vecItemList.clear();

	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템 정보가 이상합니다!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"대리상인 전용 ItemUID가 아닙니다! 절대 일어나서는 안되는 오류!" )
				<< BUILD_LOG( spItemInfo->GetItemUID() )
				<< END_LOG;
			continue;
		}

		if( bAllItems == false )
		{
			// 다 팔린것은 리스트에 담지 않는다!
			if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SOLD_OUT )
				continue;
		}

		KSellPersonalShopItemInfo kInfo;
		spItemInfo->GetSellPersonalShopItemInfo( kInfo );
		vecItemList.push_back( kInfo );
	}
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_PSHOP_AGENCY_NEW
//}}

bool KPersonalShopItemManager::GetSellPersonalShopItemInfo( IN const UidType iItemUID, OUT KSellPersonalShopItemInfo& kItemInfo )
{
	KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"아이템이 존재하지 않습니다!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}

	spItemInfo->GetSellPersonalShopItemInfo( kItemInfo );
	return true;
}

KPersonalShopItemPtr KPersonalShopItemManager::GetPersonalShopItem( IN const UidType iItemUID )
{
	std::map< UidType, KPersonalShopItemPtr >::iterator mit;
	mit = m_mapSellItemInfo.find( iItemUID );
	if( mit == m_mapSellItemInfo.end() )
	{
		START_LOG( cwarn, L"아이템 정보가 존재하지 않습니다!" )
			<< BUILD_LOG( m_mapSellItemInfo.size() );

		return KPersonalShopItemPtr();
	}

	return mit->second;
}

bool KPersonalShopItemManager::AddSellItemInfo( IN const KSellPersonalShopItemInfo& kInfo )
{
	if( IsExistItem( kInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
	{
		START_LOG( cerr, L"이미 존재하는 아이템 입니다!" )
			<< BUILD_LOG( kInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;
	}

	KPersonalShopItemPtr spItemInfo = KPersonalShopItemPtr( new KPersonalShopItem( kInfo ) );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"아이템 정보 생성을 실패하였습니다." )
			<< BUILD_LOG( kInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;        
	}

	m_mapSellItemInfo.insert( std::make_pair( kInfo.m_kInventoryItemInfo.m_iItemUID, spItemInfo ) );
	m_vecSellItemInfo.push_back( spItemInfo );
	return true;
}

bool KPersonalShopItemManager::AddSellItemInfo( IN const KSellPShopItemBackupData& kInfo )
{
	if( IsExistItem( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
	{
		START_LOG( cerr, L"이미 존재하는 아이템 입니다!" )
			<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;
	}

	LIF( kInfo.m_kSellPShopItemInfo.m_cPShopItemType == KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY );

	KPersonalShopItemPtr spItemInfo = KPersonalShopItemPtr( new KPersonalShopItem( kInfo ) );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"아이템 정보 생성을 실패하였습니다." )
			<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
			<< END_LOG;
		return false;
	}

	m_mapSellItemInfo.insert( std::make_pair( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID, spItemInfo ) );
	m_vecSellItemInfo.push_back( spItemInfo );
	return true;
}

void KPersonalShopItemManager::DelSellItemInfo( IN const UidType iItemUID )
{
	LIF( IsExistItem( iItemUID ) );
    
	// vector컨테이너에서 삭제
	std::vector< KPersonalShopItemPtr >::iterator vit;
	for( vit = m_vecSellItemInfo.begin(); vit != m_vecSellItemInfo.end(); ++vit )
	{
		if( (*vit) == NULL )
		{
			START_LOG( cerr, L"아이템 포인터 값이 이상합니다!" )
				<< END_LOG;
			continue;
		}

		if( (*vit)->GetItemUID() == iItemUID )
			break;
	}

	if( vit != m_vecSellItemInfo.end() )
	{
		m_vecSellItemInfo.erase( vit );
	}

	// map컨테이너에서 삭제
    m_mapSellItemInfo.erase( iItemUID );
}

void KPersonalShopItemManager::UpdateSellItemList( IN const std::vector< KSellPersonalShopItemInfo >& vecSellItemList )
{
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kInfo, vecSellItemList )
	{
		if( IsExistItem( kInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
		{
			START_LOG( cerr, L"중복된 아이템이 있습니다. 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kInfo.m_kInventoryItemInfo.m_iItemUID )
				<< END_LOG;			
			continue;
		}

		AddSellItemInfo( kInfo );
	}
}

void KPersonalShopItemManager::UpdateSellItemBackupDataList( IN const std::vector< KSellPShopItemBackupData >& vecSellItemList )
{
	BOOST_TEST_FOREACH( const KSellPShopItemBackupData&, kInfo, vecSellItemList )
	{
		if( IsExistItem( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID ) == true )
		{
			START_LOG( cerr, L"DB로부터 받은 백업 아이템 데이터를 넣는데 중복된게 있다? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< END_LOG;
			continue;
		}

		if( kInfo.m_kSellPShopItemInfo.m_cPShopItemType != KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY )
		{
			START_LOG( cerr, L"대리상인 전용 ItemUID타입이 아닙니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOGc( kInfo.m_kSellPShopItemInfo.m_cPShopItemType )
				<< END_LOG;
            continue;
		}

		AddSellItemInfo( kInfo );
	}
}

bool KPersonalShopItemManager::PrepareForBuyItem( IN const UidType iHostUnitUID, IN const char cPersonalShopType, IN const KERM_BUY_PERSONAL_SHOP_ITEM_REQ& kReq, OUT KDBE_BUY_PSHOP_AGENCY_ITEM_REQ& kResult )
{
	SET_ERROR( NET_OK );

	// 살려는 아이템을 찾는다.
	KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( kReq.m_iBuyItemUID );
	if( spItemInfo == NULL )
	{
		// 시간차로 발생할 수 있는 오류임!
		START_LOG( cwarn, L"아이템이 존재하지 않습니다!" )
			<< BUILD_LOG( kReq.m_iBuyItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_15 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// 구매 준비

	// 1. 대리상점 전용 ItemUID가 맞는지 검사
	if( spItemInfo->IsPShopAgencyItemUID() == false )
	{
		START_LOG( cerr, L"대리상점 전용 ItemUID가 아닙니다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOGc( spItemInfo->GetItemUID() )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_15 );
		return false;
	}

	// 1. 해당 아이템이 현재 구매 가능한 상태인지 검사
	if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_SALE )
	{
		START_LOG( clog, L"현재 해당 아이템은 구매 가능한 상태가 아닙니다!" )
			<< BUILD_LOGc( spItemInfo->GetShopItemState() )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_25 );
		return false;
	}

	// 1. 거래 가능한 카테고리의 아이템인지?
	if( CXSLInventory::IsTradePossibleCategory( spItemInfo->GetSlotCategory() ) == false )
	{
		START_LOG( cerr, L"판매중인 아이템이 거래가능한 카테고리가 아니다." )
			<< BUILD_LOGc( spItemInfo->GetSlotCategory() )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_17 );
		return false;
	}

	// 2. 구매 수량 요청 검사
	if( spItemInfo->GetUsageType() != CXSLItem::PT_QUANTITY )
	{
		if( kReq.m_iBuyQuantity > 1 )
		{
			START_LOG( cerr, L"수량성 아이템이 아닌데 사는 개수가 1이상임. 버그인가? 해킹인가?" )
				<< BUILD_LOGc( spItemInfo->GetUsageType() )
				<< BUILD_LOG( kReq.m_iBuyQuantity )
				<< END_LOG;
		}
	}

	// 3. 수량 검사
	if( spItemInfo->GetQuantity() < kReq.m_iBuyQuantity )
	{
		START_LOG( cerr, L"판매물품이 부족함." )
			<< BUILD_LOG( spItemInfo->GetQuantity() )
			<< BUILD_LOG( kReq.m_iBuyQuantity )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_14 );
		return false;
	}

	// 4. 구매 액수의 한계치 검사
	__int64 iPricePerOne  = static_cast<__int64>(spItemInfo->GetTotalPricePerOne());
	__int64 iItemQuantity = static_cast<__int64>(kReq.m_iBuyQuantity);
	__int64 iCheckTotalPrice = iPricePerOne * iItemQuantity;

	if( iCheckTotalPrice < 0  ||  iCheckTotalPrice > SEnum::UI_MAX_ED_CHAR_HAVE )
	{
		START_LOG( cout, L"구매 액수값이 한계치를 넘어섰다. 버그유저로 의심됨.." )
			<< BUILD_LOG( iPricePerOne )
			<< BUILD_LOG( iItemQuantity )
			<< BUILD_LOG( iCheckTotalPrice );

		SET_ERROR( ERR_PERSONAL_SHOP_24 );
		return false;
	}

	// 5. 가격 검사..
	int iTotalPrice = (spItemInfo->GetTotalPricePerOne() * kReq.m_iBuyQuantity);
#ifdef DEF_TRADE_BOARD	
	//{{ 2011. 07. 19	최육사	대리상점 구매 수수료 없앤 이벤트
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	// 이벤트 기간 동안에는 수수료가 없다!
#else
	//iTotalPrice += static_cast<int>(iTotalPrice * TD_COMMISSION); // 거래게시판을 통해서 왔으면 구매 수수료 3%를 붙인다.
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}	
#endif DEF_TRADE_BOARD

	// 6. 구매자가 물품살 ED가 있는지 검사.
	if( iTotalPrice > kReq.m_iBuyUnitNowED )
	{
		START_LOG( clog, L"ED가 부족하여 물품을 구매할수 없음.!" )
			<< BUILD_LOG( iTotalPrice )
			<< BUILD_LOG( kReq.m_iBuyUnitNowED );

		SET_ERROR( ERR_PERSONAL_SHOP_16 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Temp정보 정리
	{
		//#3. GSUser종료시 예외 처리 정보
		kResult.m_kItemQuantityUpdateTemp = kReq.m_kItemQuantityUpdateTemp;

		// 7. 구매성공시의 ED값을 구하여 임시 저장
		kResult.m_iEDTemp = kReq.m_iBuyUnitNowED - iTotalPrice;

		// 8. 한번에 구매가능한 아이템은 최대 1종류뿐
		kResult.m_vecItemInfoTemp = kReq.m_vecItemInfoTemp;

		if( kResult.m_vecItemInfoTemp.size() > 1 )
		{
			START_LOG( cerr, L"한번에 구매가능한 아이템 종류는 한종류뿐인데? 일어날수 없는 에러!" )
				<< BUILD_LOG( kResult.m_vecItemInfoTemp.size() )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_05 );
			return false;
		}
		else if( kResult.m_vecItemInfoTemp.size() == 1 )
		{
			//{{ 2009. 2. 12  최육사	개인상점 ED버그 방지
			bool bIsBuyItemExist = false;
			BOOST_TEST_FOREACH( KItemInfo&, kBuyItemInfoTemp, kResult.m_vecItemInfoTemp )
			{
				if( kBuyItemInfoTemp.m_iItemID == spItemInfo->GetItemID() )
				{
					// GSUser소멸시 강제 업데이트기능으로 DB에 들어갈 아이템 정보 복사
					spItemInfo->GetItemInfo( kBuyItemInfoTemp );

					// 구매한 수량
					kBuyItemInfoTemp.m_iQuantity = kReq.m_iBuyQuantity;

					bIsBuyItemExist = true;
					break;
				}
			}

			if( bIsBuyItemExist == false )
			{
				START_LOG( cerr, L"아이템이 존재하지 않음." )
					<< BUILD_LOG( kReq.m_iBuyQuantity )
					<< BUILD_LOG( kReq.m_iBuyItemUID )
					<< END_LOG;

				SET_ERROR( ERR_PERSONAL_SHOP_05 );
				return false;
			}
			//}}
		}
	}

	// 아이템 구매 처리
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	KSellPShopItemDBUpdateInfo kSellItemTemp;
	if( spItemInfo->BuyItem( cPersonalShopType, 
		kReq.m_iBuyQuantity, 
		kResult.m_iSellUnitEDIn, 
		kResult.m_iPriceUnitEDOUT, 
		kResult.m_iPriceCommissionED, 
		kResult.m_kPriceIntemInfoIN,
		kSellItemTemp ) == false )
#else
	if( spItemInfo->BuyItem( cPersonalShopType, 
		kReq.m_iBuyQuantity, 
		kResult.m_iSellUnitEDIn, 
		kResult.m_iPriceUnitEDOUT, 
		kResult.m_iPriceCommissionED, 
		kResult.m_kPriceIntemInfoIN,
		kResult.m_kSellItemDBUpdate ) == false )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	{
		START_LOG( cerr, L"아이템이 존재하지 않음." )
			<< BUILD_LOG( kReq.m_iBuyQuantity )
			<< BUILD_LOG( kReq.m_iBuyItemUID )
			<< END_LOG;

		SET_ERROR( ERR_PERSONAL_SHOP_05 );
		return false;
	}

	return true;
}

bool KPersonalShopItemManager::Confirm( IN const UidType iItemUID, IN const bool bSuccess )
{
	// 살려는 아이템을 찾는다.
	KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
	if( spItemInfo == NULL )
	{
		START_LOG( cerr, L"아이템이 존재하지 않습니다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iItemUID )
			<< END_LOG;

		return false;
	}
	
	spItemInfo->Confirm( bSuccess );
    return true;
}

bool KPersonalShopItemManager::IsAllItemSoldOut() const
{
	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템 정보가 이상합니다!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_SOLD_OUT )
            return false;
	}

	return true;
}

//{{ 2011. 05. 24	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
bool KPersonalShopItemManager::IsAllItemOnSale() const
{
	BOOST_TEST_FOREACH( KPersonalShopItemPtr, spItemInfo, m_vecSellItemInfo )
	{
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템 정보가 이상합니다!" )
				<< END_LOG;
			continue;
		}

		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_SOLD_OUT  ||
			spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
			return false;
	}

	return true;
}
#endif SERV_PSHOP_AGENCY_NEW
//}}

bool KPersonalShopItemManager::PrepareForPickUp( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ& kResult )
{
	SET_ERROR( NET_OK );

	kResult.m_vecPickUpItemList = vecPickUpItemList;
	kResult.m_mapPickUpItemInfo.clear();
	kResult.m_iTotalAddED = 0;

	// 각 아이템 정보들을 모으자!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템이 존재하지 않습니다!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_52 );
			return false;
		}

		// 대리상점 전용 ItemUID가 아니라면 PickUp할수 없다!
		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"대리상점 전용 ItemUID가 아닙니다! 절대 일어날수 없는 에러!" ) 
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_47 );
			return false;
		}

		// 현재 판매가 진행중인 아이템이라면 가져올수 없다!
		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PROCESSING )
		{
            SET_ERROR( ERR_PERSONAL_SHOP_48 );
			return false;
		}

		// 그외에 가져올수 없는 상태
		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_49 );
			return false;
		}

		// 아이템 정보 얻기
		const int iItemID = spItemInfo->GetItemID();
		const int iQuantity = spItemInfo->GetQuantity();

		std::pair< std::map< int, int >::const_iterator, bool > pairResult;
		pairResult = kResult.m_mapPickUpItemInfo.insert( std::make_pair( iItemID, iQuantity ) );
		if( pairResult.second == false )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_53 );
			return false;
		}

		kResult.m_iTotalAddED += spItemInfo->GetTotalSellEDIn();
	}

	return true;
}

bool KPersonalShopItemManager::PickUp( IN const std::vector< UidType >& vecPickUpItemList, OUT KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ& kResult )
{
	SET_ERROR( NET_OK );

	// 각 아이템 정보들을 모으자!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템이 존재하지 않습니다!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_52 );
			return false;
		}

		// 대리상점 전용 ItemUID가 아니라면 가져올수 없다!
		if( spItemInfo->IsPShopAgencyItemUID() == false  )
		{
			START_LOG( cerr, L"대리상점 전용 ItemUID가 아닌데 가져오기를 하려고 합니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			SET_ERROR( ERR_PERSONAL_SHOP_47 );
			return false;
		}

		// 현재 판매가 진행중인 아이템이라면 가져올수 없다!		
		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PROCESSING )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_48 );
			return false;
		}

		if( spItemInfo->GetShopItemState() == KPersonalShopItem::SIS_PICK_UP )
		{
			SET_ERROR( ERR_PERSONAL_SHOP_49 );
			return false;
		}
	}

	// pick up 상태로 바꾸자!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템이 존재하지 않습니다!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}
		
		spItemInfo->SetShopItemState( KPersonalShopItem::SIS_PICK_UP );

		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		KSellPersonalShopItemInfo kItem;
		spItemInfo->GetSellPersonalShopItemInfo( kItem );
		kResult.m_vecPickUpItemList.push_back( kItem );
#else
		kResult.m_vecPickUpItemList.push_back( iItemUID );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
	}

	return true;
}

void KPersonalShopItemManager::UnPickUp( IN const std::vector< UidType >& vecPickUpFailList )
{
	// 각 아이템 정보들을 모으자!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpFailList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템이 존재하지 않습니다! 이미 검사 했는데? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// 대리상점 전용 ItemUID가 아니라면 PickUp할수 없다!
		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"대리상점 전용 ItemUID가 아닙니다! 절대 일어날수 없는 에러!" ) 
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
		}

		// 현재 판매가 진행중인 아이템이라면 가져올수 없다!
		if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_PICK_UP )
		{
			START_LOG( cerr, L"Pick Up 상태가 아닌데 Pick Up취소를 요청하네?" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// 현재 아이템 상태에 맞게 수정하자!
		if( spItemInfo->GetQuantity() > 0 )
		{
			spItemInfo->SetShopItemState( KPersonalShopItem::SIS_SALE );
		}
		else
		{
			spItemInfo->SetShopItemState( KPersonalShopItem::SIS_SOLD_OUT );
		}
	}
}

//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
void KPersonalShopItemManager::PickUpComplete( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK& kResult, IN const ITEM_LIST_TYPE eType )
#else
void KPersonalShopItemManager::PickUpComplete( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK& kResult )
#endif SERV_PSHOP_AGENCY_NEW
//}}
{
	kResult.m_iTotalSellEDIn = 0;
	kResult.m_vecPickUpSuccessItemList.clear();
	kResult.m_vecSellItemInfo.clear();

	// 각 아이템 정보들을 모으자!
	BOOST_TEST_FOREACH( const UidType, iItemUID, vecPickUpItemList )
	{
		KPersonalShopItemPtr spItemInfo = GetPersonalShopItem( iItemUID );
		if( spItemInfo == NULL )
		{
			START_LOG( cerr, L"아이템이 존재하지 않습니다! 이미 검사 했는데? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// 대리상점 전용 ItemUID가 아니라면 PickUp할수 없다!
		if( spItemInfo->IsPShopAgencyItemUID() == false )
		{
			START_LOG( cerr, L"대리상점 전용 ItemUID가 아닙니다! 절대 일어날수 없는 에러!" ) 
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
		}

		// 현재 판매가 진행중인 아이템이라면 가져올수 없다!
		if( spItemInfo->GetShopItemState() != KPersonalShopItem::SIS_PICK_UP )
		{
			START_LOG( cerr, L"Pick Up 상태가 아닌데 Pick Up취소를 요청하네? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;
			continue;
		}

		// 판매대금을 가져오자!
		kResult.m_iTotalSellEDIn += spItemInfo->GetTotalSellEDIn();

		// 아이템 정보를 얻자!
		KSellPersonalShopItemInfo kInfo;
		spItemInfo->GetSellPersonalShopItemInfo( kInfo );
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		DelSellItemInfo( iItemUID );
#else
		kResult.m_vecPickUpSuccessItemList.push_back( kInfo );
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
	}

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
#else
	// 정보를 얻는 아이템들을 삭제하자!
	BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kSellItemInfo, kResult.m_vecPickUpSuccessItemList )
	{
		DelSellItemInfo( kSellItemInfo.m_kInventoryItemInfo.m_iItemUID );
	}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	// 현재 판매중인 아이템 정보도 얻자!
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW	
	GetSellPersonalShopItemInfoList( kResult.m_vecSellItemInfo, eType );
#else
	GetSellPersonalShopItemInfoList( kResult.m_vecSellItemInfo, true );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	
}

#endif SERV_PSHOP_AGENCY
//}}


