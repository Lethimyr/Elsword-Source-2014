#include ".\personalshopitemlist.h"

//{{ 2010. 12. 16  조효진	개인 상점 아이템 검색 시 대소문자 무시
#ifdef SERV_STRING_CHECK_IGNORE_CASE
#include <boost/algorithm/string/find.hpp>
#endif SERV_STRING_CHECK_IGNORE_CASE
//}}
#include <boost/test/utils/foreach.hpp>
#include "X2Data/XSLInventory.h"
#include "X2Data/XSLUnit.h"
#include "X2Data/XSLItemManager.h"
#include "NetError.h"

//{{ 2010. 03. 24  최육사	define처리
#ifdef DEF_TRADE_BOARD

ImplementSingleton( KPersonalShopItemList );

KPersonalShopItemList::KPersonalShopItemList(void)
{
}

KPersonalShopItemList::~KPersonalShopItemList(void)
{
}

ImplToStringW( KPersonalShopItemList )
{
	stm_ << L"KPersonalShopItemList dump." << std::endl
		;

	return stm_;
}

bool KPersonalShopItemList::AddItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & _kNot )
{
	// 리스트에 등록될 아이템 수만큼 반복
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo, kAddItemInfo, _kNot.m_vecItemInfo )
	{
		// 아이템 등록형식으로 가공
		KTradeBoardItemInfo kInsertItem;
		kInsertItem.m_iPersonalShopUID = _kNot.m_iPSUID;
		//{{ 2011. 05. 03	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		kInsertItem.m_iHostUnitUID = _kNot.m_iHostUnitUID;
#endif SERV_PSHOP_AGENCY
		//}}
		kInsertItem.m_wstrSellerNickName = _kNot.m_wstrSellerNickName;
		kInsertItem.m_kSellItemInfo = kAddItemInfo;

		// 카테고리 키 검사.
		if( false == CheckSlotCategoryID(kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_cSlotCategory) )
		{
			START_LOG( cerr, L"카테고리 키 이상.!" )
				<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			continue;
		}

		std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
		mit = m_mapItemInfo.find( InvSlotCategoryID(kInsertItem.m_kSellItemInfo) );

		// 카테고리가 비어있으면.
		if( mit == m_mapItemInfo.end() )
		{
			std::list<KTradeBoardItemInfo> kItemList;
			kItemList.push_back(kInsertItem);
			m_mapItemInfo.insert( std::make_pair(InvSlotCategoryID(kInsertItem.m_kSellItemInfo), kItemList) );

			START_LOG( clog, L"카테고리 만들어 삽입." )
				<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOG( kInsertItem.m_wstrSellerNickName );
			continue;
		} // if
		else
		{
			bool bIsInsert = false;

			// 리스트를 검색 정렬하여 집어넣기.
			std::list<KTradeBoardItemInfo>::iterator lit;
			for( lit = mit->second.begin(); lit != mit->second.end(); ++lit )
			{
				bIsInsert = false;

				const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );
				const CXSLItem::ItemTemplet* pInsertItem = SiCXSLItemManager()->GetItemTemplet( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

				// 아이템 아이디 체크.
				if( pListItem == NULL || pInsertItem == NULL )
				{
					START_LOG( cerr, L"잘못된 아이템 아이디.!" )
						<< BUILD_LOG( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						<< END_LOG;
					break;
				}

				//## 키값별 우선순위로 체크하여 정렬 시작

				//# 부위별
				if( InvEqipPosID(pListItem) == InvEqipPosID(pInsertItem) )
				{
					//# 레벨 별
					if( pListItem->m_UseLevel == pInsertItem->m_UseLevel )					
					{
						//# 캐릭터 별
						if( InvUnitTypeID(pListItem) == InvUnitTypeID(pInsertItem) )
						{
                            //# 등급 별
							if( InvItemGradeID(pListItem) == InvItemGradeID(pInsertItem) )
							{
								//# 가격 별
								if( (*lit).m_kSellItemInfo.TotalPricePerOne() > kInsertItem.m_kSellItemInfo.TotalPricePerOne() )
								{
									mit->second.insert( lit, kInsertItem );
									bIsInsert = true;

									START_LOG( clog, L"가격별 아이템 정렬 삽입" )
										<< BUILD_LOG( pInsertItem->m_Name )
										<< BUILD_LOG( pInsertItem->m_ItemID );
									break;
								}
							}
							else if( InvItemGradeID(pListItem) > InvItemGradeID(pInsertItem) )
							{
								mit->second.insert( lit, kInsertItem );
								bIsInsert = true;

								START_LOG( clog, L"등급별 아이템 정렬 삽입" )
									<< BUILD_LOG( pInsertItem->m_Name )
									<< BUILD_LOG( pInsertItem->m_ItemID );
								break;
							}
						}
						else if( InvUnitTypeID(pListItem) > InvUnitTypeID(pInsertItem) )
						{
							mit->second.insert( lit, kInsertItem );
							bIsInsert = true;

							START_LOG( clog, L"캐릭터별 아이템 정렬 삽입" )
								<< BUILD_LOG( pInsertItem->m_Name )
								<< BUILD_LOG( pInsertItem->m_ItemID );
							break;
						}	
					}					
					else if( pListItem->m_UseLevel < pInsertItem->m_UseLevel )
					{
						mit->second.insert( lit, kInsertItem );
						bIsInsert = true;

						START_LOG( clog, L"레벨별 아이템 정렬 삽입" )
							<< BUILD_LOG( pInsertItem->m_Name )
							<< BUILD_LOG( pInsertItem->m_ItemID );
						break;
					}
				}
				else if( InvEqipPosID(pListItem) > InvEqipPosID(pInsertItem) )
				{ 
					mit->second.insert( lit, kInsertItem );
					bIsInsert = true;

					START_LOG( clog, L"부위별 아이템 정렬 삽입" )
						<< BUILD_LOG( pInsertItem->m_Name )
						<< BUILD_LOG( pInsertItem->m_ItemID );
					break;
				}
			} // for

			if( bIsInsert == false )
			{
				mit->second.push_back(kInsertItem);

				START_LOG( clog, L"push_back 아이템 정렬 삽입" )
					<< BUILD_LOG( kInsertItem.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
					<< BUILD_LOG( kInsertItem.m_wstrSellerNickName );
			}
		} // else

	} // boost_test_foreach

	return true;
}

bool KPersonalShopItemList::DelItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & _kNot )
{
	// 삭제할 아이템 수만큼 반복
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo, kDelItemInfo, _kNot.m_vecItemInfo )
	{
		// 카테고리 키 검사.
		if( false == CheckSlotCategoryID(kDelItemInfo.m_kInventoryItemInfo.m_cSlotCategory) )
		{
			START_LOG( cerr, L"카테고리 키 이상.!" )
				<< BUILD_LOG( kDelItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kDelItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kDelItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			continue;
		}

		std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
		mit = m_mapItemInfo.find( InvSlotCategoryID(kDelItemInfo) );

		// 카테고리 리스트가 없다면..
		if( mit == m_mapItemInfo.end() )
		{
			START_LOG( cerr, L"카테로리 리스트가 존재하지 않음." )
				<< BUILD_LOGc( kDelItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< BUILD_LOG( _kNot.m_wstrSellerNickName )
				<< END_LOG;
			continue;
		}
		// 카테고리 리스트가 있다면 순회하여 검색후 삭제.
		else
		{
			std::list<KTradeBoardItemInfo>::iterator lit;
			for( lit = mit->second.begin(); lit != mit->second.end(); ++lit )
			{
				if( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID ==
					kDelItemInfo.m_kInventoryItemInfo.m_iItemUID )
				{
					START_LOG( clog, L"아이템 리스트 삭제성공." )
						<< BUILD_LOG( (*lit).m_wstrSellerNickName )
						<< BUILD_LOG( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						;

					mit->second.erase(lit);
					break;
				}
			} // for
		}
	} // boost_test_foreach
	return true;
}

bool KPersonalShopItemList::DelItemInfo( const UidType & _iCenterUID )
{
	int iDelCnt = 0;
	// 카테고리 별 리스트를 순회..
	std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
	for( mit = m_mapItemInfo.begin(); mit != m_mapItemInfo.end(); ++mit )
	{
		std::list<KTradeBoardItemInfo> & listItemInfo = mit->second;

		// 아이템 정보 리스트를 순회하면서
		// _iCenterUID 서버군에서 생성된 상점 아이템을 삭제.
		std::list<KTradeBoardItemInfo>::iterator lit;
		for( lit = listItemInfo.begin(); lit != listItemInfo.end(); ++lit )
		{
			if( KncUid::ExtractServerID((*lit).m_iPersonalShopUID) == _iCenterUID )
			{
				lit = listItemInfo.erase(lit);
				--lit;

				++iDelCnt;
			}
		}
	}

	START_LOG( clog, L"센터가 셧다운되어 삭제된 아이템 정보 개수." )
		<< BUILD_LOG( iDelCnt )
		;

	return true;
}

bool KPersonalShopItemList::ChangeItemInfo( KERM_PERSONAL_SHOP_INFO_NOT & _kNot )
{
	// 정보를 수정할 아이템 수만큼 반복
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo, kChangeItemInfo, _kNot.m_vecItemInfo )
	{
		// 카테고리 키 검사.
		if( false == CheckSlotCategoryID(kChangeItemInfo.m_kInventoryItemInfo.m_cSlotCategory) )
		{
			START_LOG( cerr, L"카테고리 키 이상.!" )
				<< BUILD_LOG( kChangeItemInfo.m_kInventoryItemInfo.m_iItemUID )
				<< BUILD_LOG( kChangeItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOGc( kChangeItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< END_LOG;

			continue;
		}

		std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
		mit = m_mapItemInfo.find( InvSlotCategoryID(kChangeItemInfo) );

		// 카테고리 리스트가 없다면..
		if( mit == m_mapItemInfo.end() )
		{
			START_LOG( cerr, L"카테고리 리스트가 존재하지 않음." )
				<< BUILD_LOGc( kChangeItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
				<< BUILD_LOG( _kNot.m_wstrSellerNickName )
				<< END_LOG;
			continue;
		}
		// 카테고리 리스트가 있다면 순회하여 검색후 수정..
		else
		{
			std::list<KTradeBoardItemInfo>::iterator lit;
			for( lit = mit->second.begin(); lit != mit->second.end(); ++lit )
			{
				if( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID ==
					kChangeItemInfo.m_kInventoryItemInfo.m_iItemUID )
				{
					START_LOG( clog, L"아이템 리스트 수정성공." )
						<< BUILD_LOG( (*lit).m_wstrSellerNickName )
						<< BUILD_LOG( (*lit).m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
						;

					(*lit).m_kSellItemInfo = kChangeItemInfo;
					break;
				}
			} // for
		}
	} // boost_test_foreach
	return true;
}

void KPersonalShopItemList::GetSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck )
{
	kAck.m_iOK = NetError::NET_OK;

	// 카테고리 키 검사.
	if( false == CheckSlotCategoryID( kReq.m_cSlotCategory ) )
	{
		if( kReq.m_wstrFinder.empty() == true )
		{
			START_LOG( cerr, L"카테고리 키 이상." )
				<< BUILD_LOGc( kReq.m_cSlotCategory )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_29;
		}
		// 스트링만으로 검색하는 경우.
		else
		{
			//BOOST_TEST_FOREACH( std::list<KTradeBoardItemInfo>, TradeBoardItemList, m_mapItemInfo )
			std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
			for( mit = m_mapItemInfo.begin(); mit != m_mapItemInfo.end(); ++mit )
			{
				BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )//TradeBoardItemList )
				{
					const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

					if( NULL == pListItem )
					{
						START_LOG( cerr, L"아이템 아이디가 잘못되었음." )
							<< BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
							<< BUILD_LOG( kItemInfo.m_wstrSellerNickName )
							<< END_LOG;

						continue;
					} // if

					// 스트링 검사
					//{{ 2010. 12. 16  조효진	개인 상점 아이템 검색 시 대소문자 무시
#ifdef SERV_STRING_CHECK_IGNORE_CASE
					if( boost::ifind_first(pListItem->m_Name, kReq.m_wstrFinder) )
#else SERV_STRING_CHECK_IGNORE_CASE
					if( NULL != StrStrW( pListItem->m_Name.c_str(), kReq.m_wstrFinder.c_str() ) )
#endif SERV_STRING_CHECK_IGNORE_CASE
					//}}
					{
#ifdef SERV_FIX_SEARCH_WITH_EMPTY_TAB
						bool bIsInsert = true;
						if( false == kReq.m_vecItemGrade.empty() )
						{
							bIsInsert = false;
							for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
							{
								if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] ) // 순회하면서 찾기
								{
									bIsInsert = true;
									break;
								}
							}
						} // if

						if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
						{
							//최소레벨 전이면 계속 진행
							if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
								pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
								bIsInsert = false;
							////최고레벨 이후면 검색종료
							//else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
							//	break;
						} // if

						if( bIsInsert == true )
#endif SERV_FIX_SEARCH_WITH_EMPTY_TAB
						kAck.m_vecItemInfo.push_back(kItemInfo);

						if( kAck.m_vecItemInfo.size() >= 100 ) // 100 개 제한.
							return;
					}
				}
			}
		}

		// 검색된 아이템 리스트가 없으면 메세지 전송
		// des : 검색할려는 아이템 정보가 없습니다.
		if( true == kAck.m_vecItemInfo.empty() )
			kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;

		return;
	}

	//## 검색할 카테고리 리스트를 찾는다.
	std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
	mit = m_mapItemInfo.find( InvSlotCategoryID(kReq.m_cSlotCategory) );

	//# 카테고리 리스트가 없다면..
	if( mit == m_mapItemInfo.end() )
	{
		START_LOG( clog, L"검색할려는 카테고리 아이템 리스트가 없음." )
			<< BUILD_LOGc( kReq.m_cSlotCategory )
			<< END_LOG;
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
		return;
	}

	//# 검색 시작
	BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )
	{
		bool bIsInsert = true;

		const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

		if( NULL == pListItem )
		{
			START_LOG( cerr, L"아이템 아이디가 잘못되었음." )
				<< BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
				<< BUILD_LOG( kItemInfo.m_wstrSellerNickName )
				<< END_LOG;

			continue;
		} // if

		//# 스트링 검사
		if( false == kReq.m_wstrFinder.empty() )
		{
			//if( (pListItem->m_Name.find(kReq.m_wstrFinder.c_str())) < 0 )
			//int iTemp = pListItem->m_Name.find(kReq.m_wstrFinder.c_str());

			//{{ 2010. 12. 16  조효진	개인 상점 아이템 검색 시 대소문자 무시
#ifdef SERV_STRING_CHECK_IGNORE_CASE
			if( !(boost::ifind_first(pListItem->m_Name, kReq.m_wstrFinder) ) )	// 못 찾았을 때
#else SERV_STRING_CHECK_IGNORE_CASE
			if( StrStrW( pListItem->m_Name.c_str(), kReq.m_wstrFinder.c_str() ) == NULL )
#endif SERV_STRING_CHECK_IGNORE_CASE
			//}}
				bIsInsert = false;
		}

		//# 같은 부위별 인지 체크
		if( -1 != kReq.m_cEqipPosition )
		{
			//아직 이전의 부위면 계속 진행
			if( InvEqipPosID(pListItem) < InvEqipPosID(kReq.m_cEqipPosition) )
				bIsInsert = false;
			//정렬 같은 부위를 넘겼으면 검색종료
			else if( InvEqipPosID(pListItem->m_EqipPosition) > InvEqipPosID(kReq.m_cEqipPosition) )
				break;
		} // if

		//# 캐릭터별 체크
		if( -1 != kReq.m_cUnitType )
		{
			//이전 유닛이면 계속진행
			//if( InvUnitTypeID(pListItem) < InvUnitTypeID(kReq.m_cUnitType) )
			//	bIsInsert = false;
			//같은 유닛이면..
			if( pListItem->m_UnitType == kReq.m_cUnitType )
			{
				//유닛클래스 검사..
				if( -1 != kReq.m_cUnitClass )
				{
					//같은 클래스가 아니면 계속진행
					if( pListItem->m_UnitClass != kReq.m_cUnitClass )
						bIsInsert = false;
				}
			}
			else
				bIsInsert = false;
		} // if

		//# 레벨범위 체크
		if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
		{
			//최소레벨 전이면 계속 진행
			if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
				pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
				bIsInsert = false;
			////최고레벨 이후면 검색종료
			//else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
			//	break;
		} // if

		//# 아이템 등급.
		//if( -1 != kReq.m_cItemGrade )
		if( false == kReq.m_vecItemGrade.empty() )
		{
			////이전 등급이면 계속진행.
			//if( InvItemGradeID(pListItem->m_ItemGrade) < InvItemGradeID(kReq.m_cItemGrade) )
			//	bIsInsert = false;
			////이후 등급이면 검색종료.
			//else if( InvItemGradeID(pListItem->m_ItemGrade) > InvItemGradeID(kReq.m_cItemGrade) )
			//	break;
			bool bIsCheck = false;
			for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
			{
				if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] )
				{
					bIsCheck = true;
					break;
				}
			}

			if( bIsCheck == false )
				bIsInsert = false;
		} // if

		if( kAck.m_vecItemInfo.size() >= 100 )
			break;

		if( true == bIsInsert )
		{
			kAck.m_vecItemInfo.push_back(kItemInfo);
		} // if
	}// BOOST_TEST_FOREACH

	//{{ oasis907 : 김상윤 [2010.3.31] // 
	// 검색된 아이템 리스트가 없으면 메세지 전송
	// des : 검색할려는 아이템 정보가 없습니다.
	if( true == kAck.m_vecItemInfo.empty() )
		kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
	//}}

}

void KPersonalShopItemList::GetStrictSearchList( IN KEGS_SEARCH_TRADE_BOARD_REQ & kReq, OUT KEGS_SEARCH_TRADE_BOARD_ACK & kAck )
{
    kAck.m_iOK = NetError::NET_OK;

    // 카테고리 키 검사.
    if( false == CheckSlotCategoryID( kReq.m_cSlotCategory ) )
    {
        if( kReq.m_wstrFinder.empty() == true )
        {
            START_LOG( cerr, L"카테고리 키 이상." )
                << BUILD_LOGc( kReq.m_cSlotCategory )
                << END_LOG;

            kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_29;
        }
        // 스트링만으로 검색하는 경우.
        else
        {
            //BOOST_TEST_FOREACH( std::list<KTradeBoardItemInfo>, TradeBoardItemList, m_mapItemInfo )
            std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
            for( mit = m_mapItemInfo.begin(); mit != m_mapItemInfo.end(); ++mit )
            {
                BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )//TradeBoardItemList )
                {
                    const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

                    if( NULL == pListItem )
                    {
                        START_LOG( cerr, L"아이템 아이디가 잘못되었음." )
                            << BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
                            << BUILD_LOG( kItemInfo.m_wstrSellerNickName )
                            << END_LOG;

                        continue;
                    } // if

                    // 스트링 검사
                    if( pListItem->m_Name == kReq.m_wstrFinder ) // str1에서 str2 를 검색하여 가장 먼저 나타나는 곳의 위치를 리턴. 없으면 널 포인터 리턴
                    {
#ifdef SERV_FIX_SEARCH_WITH_EMPTY_TAB
                        bool bIsInsert = true;
                        if( false == kReq.m_vecItemGrade.empty() )
                        {
                            bIsInsert = false;
                            for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
                            {
                                if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] ) // 순회하면서 찾기
                                {
                                    bIsInsert = true;
                                    break;
                                }
                            }
                        } // if

                        if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
                        {
                            //최소레벨 전이면 계속 진행
                            if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
                                pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
                                bIsInsert = false;
                            ////최고레벨 이후면 검색종료
                            //else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
                            //	break;
                        } // if

                        if( bIsInsert == true )
#endif SERV_FIX_SEARCH_WITH_EMPTY_TAB
                            kAck.m_vecItemInfo.push_back(kItemInfo);

                        if( kAck.m_vecItemInfo.size() >= 100 ) // 100 개 제한.
                            return;
                    }
                }
            }
        }

        // 검색된 아이템 리스트가 없으면 메세지 전송
        // des : 검색할려는 아이템 정보가 없습니다.
        if( true == kAck.m_vecItemInfo.empty() )
            kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;

        return;
    }

    //## 검색할 카테고리 리스트를 찾는다.
    std::map< char, std::list<KTradeBoardItemInfo> >::iterator mit;
    mit = m_mapItemInfo.find( InvSlotCategoryID(kReq.m_cSlotCategory) );

    //# 카테고리 리스트가 없다면..
    if( mit == m_mapItemInfo.end() )
    {
        START_LOG( clog, L"검색할려는 카테고리 아이템 리스트가 없음." )
            << BUILD_LOGc( kReq.m_cSlotCategory )
            << END_LOG;
        kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
        return;
    }

    //# 검색 시작
    BOOST_TEST_FOREACH( KTradeBoardItemInfo, kItemInfo, mit->second )
    {
        bool bIsInsert = true;

        const CXSLItem::ItemTemplet* pListItem = SiCXSLItemManager()->GetItemTemplet( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID );

        if( NULL == pListItem )
        {
            START_LOG( cerr, L"아이템 아이디가 잘못되었음." )
                << BUILD_LOG( kItemInfo.m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
                << BUILD_LOG( kItemInfo.m_wstrSellerNickName )
                << END_LOG;

            continue;
        } // if

        //# 스트링 검사
        if( false == kReq.m_wstrFinder.empty() )
        {
            //if( (pListItem->m_Name.find(kReq.m_wstrFinder.c_str())) < 0 )
            //int iTemp = pListItem->m_Name.find(kReq.m_wstrFinder.c_str());

            if( StrStrW( pListItem->m_Name.c_str(), kReq.m_wstrFinder.c_str() ) == NULL )
                bIsInsert = false;
        }

        //# 같은 부위별 인지 체크
        if( -1 != kReq.m_cEqipPosition )
        {
            //아직 이전의 부위면 계속 진행
            if( InvEqipPosID(pListItem) < InvEqipPosID(kReq.m_cEqipPosition) )
                bIsInsert = false;
            //정렬 같은 부위를 넘겼으면 검색종료
            else if( InvEqipPosID(pListItem->m_EqipPosition) > InvEqipPosID(kReq.m_cEqipPosition) )
                break;
        } // if

        //# 캐릭터별 체크
        if( -1 != kReq.m_cUnitType )
        {
            //이전 유닛이면 계속진행
            //if( InvUnitTypeID(pListItem) < InvUnitTypeID(kReq.m_cUnitType) )
            //	bIsInsert = false;
            //같은 유닛이면..
            if( pListItem->m_UnitType == kReq.m_cUnitType )
            {
                //유닛클래스 검사..
                if( -1 != kReq.m_cUnitClass )
                {
                    //같은 클래스가 아니면 계속진행
                    if( pListItem->m_UnitClass != kReq.m_cUnitClass )
                        bIsInsert = false;
                }
            }
            else
                bIsInsert = false;
        } // if

        //# 레벨범위 체크
        if( -1 != kReq.m_cMinLevel && -1 != kReq.m_cMaxLevel )
        {
            //최소레벨 전이면 계속 진행
            if( pListItem->m_UseLevel < static_cast<int>(kReq.m_cMinLevel) ||
                pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
                bIsInsert = false;
            ////최고레벨 이후면 검색종료
            //else if( pListItem->m_UseLevel > static_cast<int>(kReq.m_cMaxLevel) )
            //	break;
        } // if

        //# 아이템 등급.
        //if( -1 != kReq.m_cItemGrade )
        if( false == kReq.m_vecItemGrade.empty() )
        {
            ////이전 등급이면 계속진행.
            //if( InvItemGradeID(pListItem->m_ItemGrade) < InvItemGradeID(kReq.m_cItemGrade) )
            //	bIsInsert = false;
            ////이후 등급이면 검색종료.
            //else if( InvItemGradeID(pListItem->m_ItemGrade) > InvItemGradeID(kReq.m_cItemGrade) )
            //	break;
            bool bIsCheck = false;
            for( int i = 0; i < static_cast<int>(kReq.m_vecItemGrade.size()); ++i )
            {
                if( pListItem->m_ItemGrade == kReq.m_vecItemGrade[i] )
                {
                    bIsCheck = true;
                    break;
                }
            }

            if( bIsCheck == false )
                bIsInsert = false;
        } // if

        if( kAck.m_vecItemInfo.size() >= 100 )
            break;

        if( true == bIsInsert )
        {
            kAck.m_vecItemInfo.push_back(kItemInfo);
        } // if
    }// BOOST_TEST_FOREACH

    //{{ oasis907 : 김상윤 [2010.3.31] // 
    // 검색된 아이템 리스트가 없으면 메세지 전송
    // des : 검색할려는 아이템 정보가 없습니다.
    if( true == kAck.m_vecItemInfo.empty() )
        kAck.m_iOK = NetError::ERR_PERSONAL_SHOP_30;
    //}}

}
char KPersonalShopItemList::InvSlotCategoryID( KSellPersonalShopItemInfo & _kInfo )
{
	return InvSlotCategoryID(_kInfo.m_kInventoryItemInfo.m_cSlotCategory);
}

char KPersonalShopItemList::InvSlotCategoryID( char _cID )
{
	char cRet = 7;	// 현재는 퀘스트가 제일 하위 키값임.

	switch( _cID )
	{
	case CXSLInventory::ST_AVARTA: cRet = 1; break;
	case CXSLInventory::ST_EQUIP: cRet = 2; break;
	case CXSLInventory::ST_ACCESSORY: cRet = 3; break;
	case CXSLInventory::ST_QUICK_SLOT: cRet = 4; break;
	case CXSLInventory::ST_SPECIAL: cRet = 5; break;
	case CXSLInventory::ST_MATERIAL: cRet = 6; break;
	case CXSLInventory::ST_QUEST: cRet = 7; break;
	}

	return cRet;
}

char KPersonalShopItemList::InvEqipPosID( const CXSLItem::ItemTemplet* _pInfo )
{
	if( _pInfo == NULL )
		return 15;

	return InvEqipPosID(_pInfo->m_EqipPosition);
}

char KPersonalShopItemList::InvEqipPosID( char _cID )
{
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
	char cRet = 16;
#else //SERV_NEW_ONE_PIECE_AVATAR_SLOT
	char cRet = 15;		// 현재는 무기 악세사리가 하위 키값임.
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

	switch( _cID )
	{
	case CXSLUnit::EP_WEAPON_HAND: cRet = 1; break;
	case CXSLUnit::EP_DEFENCE_BODY: cRet = 2; break;
	case CXSLUnit::EP_DEFENCE_LEG: cRet = 3; break;
	case CXSLUnit::EP_DEFENCE_HAND: cRet = 4; break;
	case CXSLUnit::EP_DEFENCE_FOOT: cRet = 5; break;
	case CXSLUnit::EP_DEFENCE_HAIR: cRet = 6; break;
	case CXSLUnit::EP_AC_FACE1: cRet = 7; break;
	case CXSLUnit::EP_AC_FACE2: cRet = 8; break;
	case CXSLUnit::EP_AC_FACE3: cRet = 9; break;
	case CXSLUnit::EP_AC_BODY: cRet = 10; break;
	case CXSLUnit::EP_AC_LEG: cRet = 11; break;
	case CXSLUnit::EP_AC_ARM: cRet = 12; break;
	case CXSLUnit::EP_AC_RING: cRet = 13; break;
	case CXSLUnit::EP_AC_NECKLESS: cRet = 14; break;
	case CXSLUnit::EP_AC_WEAPON: cRet = 15; break;
#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
	case CXSLUnit::EP_ONEPIECE_FASHION: cRet = 16; break;
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT
	}

	return cRet;
}

char KPersonalShopItemList::InvUnitTypeID( const CXSLItem::ItemTemplet* _pInfo )
{
	if( _pInfo == NULL )
		return 5;

	return InvUnitTypeID(_pInfo->m_UnitType);
}

char KPersonalShopItemList::InvUnitTypeID( char _cID )
{
	char cRet = UTSI_CHUNG;

	switch( _cID )
	{
	case CXSLUnit::UT_ELSWORD:	cRet = UTSI_ELSWORD;	break;
	case CXSLUnit::UT_LIRE:		cRet = UTSI_LENA;		break;
	case CXSLUnit::UT_ARME:		cRet = UTSI_AISHA;	break;
	case CXSLUnit::UT_RAVEN:	cRet = UTSI_RAVEN;	break;
	case CXSLUnit::UT_EVE:		cRet = UTSI_EVE;		break;
	// kimhc // 2010-12-23 에 추가될 신캐릭터 청 (일단 엘소드와 같게 함)
	case CXSLUnit::UT_CHUNG:	cRet = UTSI_CHUNG;	break;
	case CXSLUnit::UT_ARA:		cRet = UTSI_ARA;	break;
#ifdef SERV_NEW_CHARACTER_EL
	case CXSLUnit::UT_ELESIS:	cRet = UTSI_ELESIS;	break;
#endif // SERV_NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CXSLUnit::UT_ADD:	cRet = UTSI_NEW_CHARACTER;	break;
#endif //SERV_9TH_NEW_CHARACTER
	}

	return cRet;
}

char KPersonalShopItemList::InvItemGradeID( const CXSLItem::ItemTemplet* _pInfo )
{
	if( _pInfo == NULL )
		return 5;

	return InvItemGradeID(_pInfo->m_ItemGrade);
}

char KPersonalShopItemList::InvItemGradeID( char _cID )
{
	char cRet = 5;

	switch( _cID )
	{
	case CXSLItem::IG_UNIQUE: cRet = 1; break;
	case CXSLItem::IG_ELITE: cRet = 2; break;
	case CXSLItem::IG_RARE: cRet = 3; break;
	case CXSLItem::IG_NORMAL: cRet = 4; break;
	case CXSLItem::IG_LOW: cRet = 5; break;
	}

	return cRet;
}

bool KPersonalShopItemList::CheckSlotCategoryID( char _cID )
{
	switch( _cID )
	{
	case CXSLInventory::ST_AVARTA: break;
	case CXSLInventory::ST_EQUIP: break;
	case CXSLInventory::ST_ACCESSORY: break;
	case CXSLInventory::ST_QUICK_SLOT: break;
	case CXSLInventory::ST_SPECIAL: break;
	case CXSLInventory::ST_MATERIAL: break;
	case CXSLInventory::ST_QUEST: break;

	default:
		return false;
	}
	return true;
}

#endif DEF_TRADE_BOARD
//}}
































