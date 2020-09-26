#include ".\PersonalShopUser.h"

KPersonalShopUser::KPersonalShopUser()
{
	//m_kTradeItemInfo.m_iItemUID = 0;
	m_bIshost = false;

	m_kItemQuantityUpdateTemp.m_mapQuantityChange.clear();
	m_kItemQuantityUpdateTemp.m_vecDeleted.clear();
	m_vecItemInfoTemp.clear();
	m_iEDTemp = 0;

#ifdef DEF_TRADE_BOARD
	// 기본셋팅은 광장으로 설정..
	m_kPersonalShopUserInfo.m_bIsSquare = true;
#endif DEF_TRADE_BOARD
}

KPersonalShopUser::~KPersonalShopUser(void)
{
}

//{{ 2009. 2. 10  최육사	개인거래 버그 예방
void KPersonalShopUser::SetUpdateTemp( const KItemQuantityUpdate& kUpdateTemp, const std::vector< KItemInfo >& vecItemInfoTemp, int iEDTemp )
{
	m_kItemQuantityUpdateTemp.m_mapQuantityChange.clear();
	m_kItemQuantityUpdateTemp.m_vecDeleted.clear();
	m_vecItemInfoTemp.clear();
	m_iEDTemp = 0;

	m_kItemQuantityUpdateTemp = kUpdateTemp;
	m_vecItemInfoTemp = vecItemInfoTemp;
	m_iEDTemp = iEDTemp;
}

void KPersonalShopUser::GetUpdateTemp( KItemQuantityUpdate& kUpdateTemp, std::vector< KItemInfo >& vecItemInfoTemp, int& iEDTemp )
{
	kUpdateTemp = m_kItemQuantityUpdateTemp;
	vecItemInfoTemp = m_vecItemInfoTemp;
	iEDTemp = m_iEDTemp;

	m_kItemQuantityUpdateTemp.m_mapQuantityChange.clear();
	m_kItemQuantityUpdateTemp.m_vecDeleted.clear();
	m_vecItemInfoTemp.clear();
	m_iEDTemp = 0;
}
//}}

//bool KPersonalShopUser::SetTradeItemInfo( const KInventoryItemInfo& kInfo )
//{
//	if( m_bOffer || m_bApproval || m_bConfirm )
//	{
//		START_LOG( cerr, L"교환 아이템을 변경할 수 없는 상태임." )
//			<< BUILD_LOG( m_bOffer )
//			<< BUILD_LOG( m_bApproval )
//			<< BUILD_LOG( m_bConfirm )
//			<< END_LOG;
//
//		return false;
//	}
//
//	m_kTradeItemInfo = kInfo;
//	return true;
//}
//
//bool KPersonalShopUser::SetTradeED( int iTradeED )
//{
//	if( m_bOffer || m_bApproval || m_bConfirm )
//	{
//		START_LOG( cerr, L"교환 ED를 변경할 수 없는 상태임." )
//			<< BUILD_LOG( m_bOffer )
//			<< BUILD_LOG( m_bApproval )
//			<< BUILD_LOG( m_bConfirm )
//			<< END_LOG;
//
//		return false;
//	}
//
//	m_iTradeED = iTradeED;
//	return true;
//}