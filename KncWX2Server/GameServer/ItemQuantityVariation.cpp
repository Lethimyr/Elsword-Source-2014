#include "ItemQuantityVariation.h"
#include "X2Data/XSLItemManager.h"


//{{ 2012. 10. 04	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM


KItemQuantityVariation::KItemQuantityVariation()
{
}

KItemQuantityVariation::~KItemQuantityVariation()
{
}

void KItemQuantityVariation::Clear()
{
	m_mapIncreaseQuantity.clear();
	m_mapDecreaseQuantity.clear();
}

void KItemQuantityVariation::IncreaseQuantity( const int iItemID, const int iQuantity )
{
	std::map< int, int >::iterator mit;
	mit = m_mapIncreaseQuantity.find( iItemID );
	if( mit == m_mapIncreaseQuantity.end() )
	{
		m_mapIncreaseQuantity.insert( std::make_pair( iItemID, iQuantity ) );        
	}
	else
	{
		mit->second += iQuantity;		
	}

	//////////////////////////////////////////////////////////////////////////
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( IS_NOT_NULL( pItemTemplet ) )
	{
		START_LOG( cout, L"[테스트로그] 아이템 획득!" )
			<< BUILD_LOG( pItemTemplet->m_Name )
			<< BUILD_LOG( iQuantity );
	}
	//////////////////////////////////////////////////////////////////////////	
}

void KItemQuantityVariation::DecreaseQuantity( const int iItemID, const int iQuantity )
{
	std::map< int, int >::iterator mit;
	mit = m_mapDecreaseQuantity.find( iItemID );
	if( mit == m_mapDecreaseQuantity.end() )
	{
		m_mapDecreaseQuantity.insert( std::make_pair( iItemID, iQuantity ) );		
	}
	else
	{
		mit->second += iQuantity;		
	}

	//////////////////////////////////////////////////////////////////////////
	const CXSLItem::ItemTemplet* pItemTemplet = SiCXSLItemManager()->GetItemTemplet( iItemID );
	if( IS_NOT_NULL( pItemTemplet ) )
	{
		START_LOG( cout, L"[테스트로그] 아이템 감소!" )
			<< BUILD_LOG( pItemTemplet->m_Name )
			<< BUILD_LOG( iQuantity );
	}
	//////////////////////////////////////////////////////////////////////////
}


#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}