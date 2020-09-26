#include "Enum\Enum.h"
#include ".\TradeUser.h"

KTradeUser::KTradeUser()
{
	m_bOffer = false;
	m_bApproval = false;
	m_bConfirm = false;	
	m_iTradeED = 0;

	//m_kSuccessTradeReq.m_bIsSuccess = false;
	//m_bIsSuccess = false;

	m_kItemQuantityUpdateTemp.m_mapQuantityChange.clear();
	m_kItemQuantityUpdateTemp.m_vecDeleted.clear();
	m_vecItemInfoTemp.clear();
	m_iEDTemp = 0;
}

KTradeUser::~KTradeUser(void)
{
}

bool KTradeUser::GetTradeItemInfo( UidType iItemUID, KInventoryItemInfo& kInfo )
{
	std::vector< KInventoryItemInfo >::const_iterator vit;
	for( vit = m_vecTradeItemInfo.begin(); vit != m_vecTradeItemInfo.end(); ++vit )
	{
		if( vit->m_iItemUID == iItemUID )
		{
            kInfo = *vit;
			return true;
		}
	}

	return false;
}

bool KTradeUser::InsertTradeItemInfo( const KInventoryItemInfo& kInfo )
{
    if( m_bOffer || m_bApproval || m_bConfirm )
    {
        START_LOG( cerr, L"교환 아이템을 변경할 수 없는 상태임." )
            << BUILD_LOG( m_bOffer )
            << BUILD_LOG( m_bApproval )
            << BUILD_LOG( m_bConfirm )
            << END_LOG;

        return false;
    }

	m_vecTradeItemInfo.push_back( kInfo );
    return true;
}

bool KTradeUser::RemoveTradeItemInfo( UidType iItemUID )
{
	std::vector< KInventoryItemInfo >::iterator vit;
	for( vit = m_vecTradeItemInfo.begin(); vit != m_vecTradeItemInfo.end(); ++vit )
	{
		if( vit->m_iItemUID == iItemUID )
			break;
	}

	if( vit == m_vecTradeItemInfo.end() )
		return false;

	m_vecTradeItemInfo.erase( vit );
	return true;
}

bool KTradeUser::SetTradeED( int iTradeED )
{
    if( m_bOffer || m_bApproval || m_bConfirm )
    {
        START_LOG( cerr, L"교환 ED를 변경할 수 없는 상태임." )
            << BUILD_LOG( m_bOffer )
            << BUILD_LOG( m_bApproval )
            << BUILD_LOG( m_bConfirm )
            << END_LOG;

        return false;
    }

    m_iTradeED = iTradeED;
    return true;
}

//{{ 2009. 2. 10  최육사	개인거래 버그 예방
void KTradeUser::Confirm( const KERM_CONFIRM_TRADE_ACK& kInfo )
{
	m_bConfirm = true;

	m_kItemQuantityUpdateTemp.m_mapQuantityChange.clear();
	m_kItemQuantityUpdateTemp.m_vecDeleted.clear();
	m_vecItemInfoTemp.clear();
	m_iEDTemp = 0;
	
	m_kItemQuantityUpdateTemp = kInfo.m_kItemQuantityUpdateTemp;
	m_vecItemInfoTemp = kInfo.m_vecItemInfoTemp;
	m_iEDTemp = kInfo.m_iEDTemp;
}

void KTradeUser::GetUpdateTemp( KERM_CONFIRM_TRADE_NOT& kInfo )
{
	kInfo.m_kItemQuantityUpdateTemp = m_kItemQuantityUpdateTemp;
	kInfo.m_vecItemInfoTemp = m_vecItemInfoTemp;
	kInfo.m_iEDTemp = m_iEDTemp;

	m_kItemQuantityUpdateTemp.m_mapQuantityChange.clear();
	m_kItemQuantityUpdateTemp.m_vecDeleted.clear();
	m_vecItemInfoTemp.clear();
	m_iEDTemp = 0;
}

//{{ 2009. 4. 28  최육사	최대ED제한
bool KTradeUser::CheckMaxED( __int64 iAddED )
{
	iAddED += m_kTradeUserInfo.m_iED;

	return ( iAddED <= static_cast<__int64>(SEnum::UI_MAX_ED_CHAR_HAVE) );
}
//}}

//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
void KTradeUser::SetDBUpdateInfo( IN const KERM_DB_UPDATE_TRADE_RESULT_NOT& kInfo )
{
	m_vecDBUpdateInfo.clear();
	m_vecDBUpdateInfo.push_back( kInfo );
}

bool KTradeUser::GetDBUpdateInfo( IN OUT std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >& vecDBUpdateInfo ) const
{
	if( m_vecDBUpdateInfo.empty() )
		return false;

	vecDBUpdateInfo.insert( vecDBUpdateInfo.begin(), m_vecDBUpdateInfo.begin(), m_vecDBUpdateInfo.end() );
	return true;
}
#endif SERV_TRADE_DDOS_DEFENCE
//}}

