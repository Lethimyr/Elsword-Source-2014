#include ".\userwishlist.h"
#include <dbg/dbg.hpp>
#include "NetError.h"
#include "../common/X2Data/XSLCashItemManager.h"

#ifdef SERV_WISH_LIST_NO_ITEM
#include "X2Data/XSLItemManager.h"
#endif //SERV_WISH_LIST_NO_ITEM		

#ifdef SERV_WISH_LIST_BUG_FIX
#include "BillManager.h"
#endif //SERV_WISH_LIST_BUG_FIX

KUserWishList::KUserWishList(void)
{
	m_bBuyCashItemInWishList = false;
}

KUserWishList::~KUserWishList(void)
{	
}

void KUserWishList::InitWishList( const std::map< int, int >& mapWishList )
{
	std::vector< int > vecNoSaleProduct; // 판매중지된 상품 리스트
	std::map< int, int >::const_iterator mit;
	for( mit = mapWishList.begin(); mit != mapWishList.end(); ++mit )
	{
		if( mit->first < 0  ||  mit->first >= SLOT_MAX )
			continue;

		m_arrWishList[mit->first].SetValue( mit->second );

		// 판매중지된 상품 리스트 검사
		if( !SiCXSLCashItemManager()->IsBuyAvailable( mit->second ) )
		{
			vecNoSaleProduct.push_back( mit->second );
		}
#ifdef SERV_WISH_LIST_NO_ITEM
		// 찜리스트불가 아이템이면 찜리스트에서 삭제한다.
		if( SiCXSLItemManager()->IsWishListNoItem( mit->second ) )
		{
			vecNoSaleProduct.push_back( mit->second );
		}
#endif //SERV_WISH_LIST_NO_ITEM
	}

	//{{ 2008. 5. 15  최육사  판매중인 상품이 아닐경우 찜에서 빼는 작업
	std::vector< int >::const_iterator vit;
	for( vit = vecNoSaleProduct.begin(); vit != vecNoSaleProduct.end(); ++vit )
	{
		if( !DelWish( *vit ) )
		{
			START_LOG( cerr, L"판매중지된 상품을 찜리스트에서 제거하기 실패!" )
				<< BUILD_LOG( *vit )
				<< END_LOG;
		}
	}
	//}}
}

void KUserWishList::GetWishList( std::map< int, int >& mapWishList )
{
#ifdef SERV_WISH_LIST_BUG_FIX
	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() <= 0 )
			continue;

		int nProductID = m_arrWishList[iIdx].GetSlotData();

		std::set<int> setSellCashItemList = SiKBillManager()->GetSellCashItemList();	// 현재 팔고 있는 캐쉬아이템 리스트를 가져옴.

		if( setSellCashItemList.find( nProductID ) == setSellCashItemList.end()
#ifdef SERV_WISH_LIST_NO_ITEM
			|| SiCXSLItemManager()->IsWishListNoItem( nProductID )
#endif //SERV_WISH_LIST_NO_ITEM			
			)
		{
			if( !DelWish( nProductID ) )												// 주의! DelWish()하고 나면 m_arrWishList[iIdx].GetSlotData()의 값이 바뀜.
			{																			// 예) m_arrWishList[1]의 데이터가 m_arrWishList[0]으로 이동함.
				START_LOG( cerr, L"판매중지된 상품을 찜리스트에서 제거하기 실패!" )
					<< BUILD_LOG( nProductID )
					<< END_LOG;
			}

			--iIdx;																		// 찜리스트가 각각 앞당겨졌으므로 for 루프 인덱스도 앞당김.
		}
	}

	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		mapWishList.insert( std::make_pair( iIdx, m_arrWishList[iIdx].GetSlotData() ) );
	}
#else //SERV_WISH_LIST_BUG_FIX
	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() <= 0 )
			continue;

		mapWishList.insert( std::make_pair( iIdx, m_arrWishList[iIdx].GetSlotData() ) );
	}
#endif //SERV_WISH_LIST_BUG_FIX
}

void KUserWishList::FlushWishListChange( std::map< int, int >& mapWishList )
{
	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].IsChanged() )
		{
			int iTemp = m_arrWishList[iIdx].GetSlotData();
			
			m_arrWishList[iIdx].SetValue( iTemp );
			
			mapWishList.insert( std::make_pair( iIdx, iTemp ) );
		}
	}
}

bool KUserWishList::AddWish( int iItemID )
{
	SET_ERROR( NET_OK );

	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() == iItemID )
		{
			START_LOG( cerr, L"이미 있는걸 등록하려고 하네? 일단은 통과시키자." )
				<< BUILD_LOG( iIdx )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			return true;
		}

		if( m_arrWishList[iIdx].GetSlotData() == 0 )
		{
			m_arrWishList[iIdx] = iItemID;
			return true;
		}	
	}

	SET_ERROR( ERR_WISH_LIST_02 );
	return false;
}

bool KUserWishList::DelWish( int iItemID )
{
	SET_ERROR( NET_OK );

	for( int iIdx = 0; iIdx < SLOT_MAX; ++iIdx )
	{
		if( m_arrWishList[iIdx].GetSlotData() == iItemID )
		{
			for( int iCnt = iIdx; iCnt < SLOT_MAX; ++iCnt )
			{
				// 슬롯전체의 마지막 인덱스라면..
				if( iCnt == (SLOT_MAX - 1) )
				{
					if( m_arrWishList[iCnt].GetSlotData() == 0 )
						break;

					m_arrWishList[iCnt] = 0;
					break;
				}

				// 등록된 상품의 마지막 인덱스..
				if( m_arrWishList[iCnt].GetSlotData() == 0 )
					break;

				// 한칸씩 밀자!				
				m_arrWishList[iCnt] = m_arrWishList[iCnt + 1].GetSlotData();
			}
			return true;
		}
	}

	START_LOG( cerr, L"등록되지도 않은걸 삭제하려고 하네.. 일단은 통과시키자." ) 
		<< BUILD_LOG( iItemID )
		<< END_LOG;
	return false;
}

