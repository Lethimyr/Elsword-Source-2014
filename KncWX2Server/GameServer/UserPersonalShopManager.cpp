#include ".\userpersonalshopmanager.h"


//{{ 2011. 04. 19	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY


KUserPersonalShopManager::KUserPersonalShopManager(void)
{
	Clear();
}

KUserPersonalShopManager::~KUserPersonalShopManager(void)
{
}

void KUserPersonalShopManager::Clear()
{
	m_iPShopAgencyUID		= 0;
	m_tAgencyExpirationDate = CTime( 2000, 1, 1, 0, 0, 0 ); // 초기값
	//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	m_iPersonalShopBuyItemUID = 0;
	m_iPersonalShopBuyItemID = 0;
	m_iPersonalShopBuyQuantity = 0;
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	//}}
}

void KUserPersonalShopManager::Init( IN OUT KUserPShopAgencyInfo& kUserPShopAgency )
{
	// 대리 상인 만료일 세팅
	SetPShopAgencyExpirationDate( kUserPShopAgency.m_wstrAgencyExpirationDate );
#ifdef SERV_UPGRADE_TRADE_SYSTEM
    SetPShopType( kUserPShopAgency.m_cShopType );
#endif //SERV_UPGRADE_TRADE_SYSTEM
	CTime tCurTime = CTime::GetCurrentTime();
	if( tCurTime > m_tAgencyExpirationDate )
	{
		// 이미 만료일이 지났다면 문자열을 비우자!
		kUserPShopAgency.m_wstrAgencyExpirationDate.clear();
	}
}

void KUserPersonalShopManager::SetPShopAgencyExpirationDate( IN const std::wstring& wstrAgencyExpirationDate )
{
	m_wstrAgencyExpirationDate = wstrAgencyExpirationDate;

	if( KncUtil::ConvertStringToCTime( wstrAgencyExpirationDate, m_tAgencyExpirationDate ) == false )
	{
		START_LOG( cerr, L"문자열 시간 변환 실패." )
			<< BUILD_LOG( wstrAgencyExpirationDate )
			<< END_LOG;
		return;
	}
}

bool KUserPersonalShopManager::IsOpenPossiblePShopAgency() const
{
	return ( CTime::GetCurrentTime() < m_tAgencyExpirationDate );
}

#ifdef SERV_UPGRADE_TRADE_SYSTEM
void KUserPersonalShopManager::SetPShopType( IN const char cShopType_ )
{
    m_cShopType = cShopType_;
}
#endif //SERV_UPGRADE_TRADE_SYSTEM
#endif SERV_PSHOP_AGENCY
//}}