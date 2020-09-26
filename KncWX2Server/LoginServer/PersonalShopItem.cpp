#include ".\PersonalShopItem.h"
#include "X2Data/XSLSquareUnit.h"

//{{ 2011. 04. 25	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY


KPersonalShopItem::KPersonalShopItem( const KSellPersonalShopItemInfo& kInfo )
{
	// 초기화
	Clear();

	// 아이템 정보 업데이트
	SetSellPersonalShopItemInfo( kInfo );
}

KPersonalShopItem::KPersonalShopItem( const KSellPShopItemBackupData& kInfo )
{
	// 초기화
	Clear();

	// DB백업데이터 업데이트
	SetSellPersonalShopItemInfo( kInfo.m_kSellPShopItemInfo );

	// PShopItemUID세팅
	SetPShopAgencyItemUID( kInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID );

	// 현재 아이템 수량이 0개면 다 팔린거다!
	if( GetQuantity() == 0 )
	{
		SetShopItemState( SIS_SOLD_OUT );
	}

	m_iTotalSoldItemQuantity = kInfo.m_iTotalSoldItemQuantity;
	m_iTotalSellEDIn		 = kInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn;
	m_iTotalSellCommissionED = kInfo.m_iTotalSellCommissionED;

	// 대리상점 DB 테이블에서 불러오는 아이템 이므로 대리상점 전용 ItemUID어야만 합니다.
	if( IsPShopAgencyItemUID() == false )
	{
		START_LOG( cerr, L"DB에서 불러왔는데 왜 대리상점 전용 ItemUID가 아니지?" )
			<< BUILD_LOG( GetItemUID() )
			<< END_LOG;
	}
}

KPersonalShopItem::~KPersonalShopItem(void)
{
}

void KPersonalShopItem::Clear()
{
	m_iTotalSoldItemQuantity		 = 0;
	m_iTotalSellEDIn				 = 0;
	m_iTotalSellCommissionED		 = 0;
	m_eItemState					 = SIS_SALE;
	m_iWaitForConfirmQuantity		 = 0;
	m_iWaitForConfirmSellED			 = 0;
	m_iWaitForConfirmSellCommisionED = 0;
}

void KPersonalShopItem::GetSellPersonalShopItemInfo( OUT KSellPersonalShopItemInfo& kInfo ) const
{
    kInfo = m_kSellItemInfo;
	kInfo.m_iTotalSellEDIn = m_iTotalSellEDIn;
	kInfo.m_iTotalSoldItemQuantity = m_iTotalSoldItemQuantity;
}

void KPersonalShopItem::SetPShopAgencyItemUID( IN const UidType iItemUID )
{
	// 대리상점 전용 ItemUID를 받습니다
	m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID = iItemUID;
}

bool KPersonalShopItem::BuyItem( IN const char cPersonalShopType, 
								 IN const int iBuyQuantity, 
								 OUT int& iSellUnitEDIn,
								 OUT int& iPriceUnitEDOUT, 
								 OUT int& iPriceCommissionED, 
								 OUT KInventoryItemInfo& kPriceIntemInfoIN,
								 OUT KSellPShopItemDBUpdateInfo& kSellItemDBUpdateInfo )
{
	if( IsPShopAgencyItemUID() == false )
	{
		START_LOG( cerr, L"이 아이템은 아직 대리상점 전용 ItemUID가 발급되지 않았습니다. 절대로 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetItemUID() )
			<< END_LOG;
		return false;
	}

	if( GetShopItemState() != SIS_SALE )
	{
		START_LOG( cerr, L"해당 아이템은 현재 구매가능한 상태가 아닙니다!" )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( GetShopItemState() )
			<< END_LOG;
		return false;
	}

	if( iBuyQuantity > GetQuantity() )
	{
		START_LOG( cerr, L"구매하려는 수량만큼 아이템 수량이 존재하지 않습니다! 일어날수 없는 에러!" )
			<< BUILD_LOG( GetItemUID() )
			<< BUILD_LOG( iBuyQuantity )
			<< BUILD_LOG( GetQuantity() )
			<< END_LOG;
		return false;
	}

	// 1. 판매자를 위한 해당 물품의 판매 금액을 구한다
	int iResultSellUnitEDIN = GetPricePerOne() * iBuyQuantity;			// 판매자가 받을 금액
	int iiSellCommissionED = 0;

	// 판매자가 받을 금액 저장
	iSellUnitEDIn = iResultSellUnitEDIN;

	// 2. 구매자를 위한 구매 정보
	iPriceUnitEDOUT = m_kSellItemInfo.TotalPricePerOne() * iBuyQuantity;		// 구매자가 지불해야할 금액
#ifdef DEF_TRADE_BOARD	
	//{{ 2011. 07. 19	최육사	대리상점 구매 수수료 없앤 이벤트
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	// 이벤트 기간 동안에는 수수료가 없다!
	iPriceCommissionED = 0;
#else
	//iPriceCommissionED = static_cast<int>(iPriceUnitEDOUT * TD_COMMISSION);		// 거래게시판 수수료
	//iPriceUnitEDOUT += iPriceCommissionED;										// 지불 금액에 수수료 포함하기
    iPriceCommissionED = 0;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}	
#endif DEF_TRADE_BOARD
	kPriceIntemInfoIN = m_kSellItemInfo.m_kInventoryItemInfo;					// 구매자에게 보낼 구매 아이템 정보
	kPriceIntemInfoIN.m_kItemInfo.m_iQuantity = iBuyQuantity;

	//////////////////////////////////////////////////////////////////////////
	// 판매아이템 정보처리 & 상점참여 유저들에게 판매상품 정보 업데이트 처리.

	// 컨펌 되기 전에 따로 보관해둔다.
	m_iWaitForConfirmQuantity = iBuyQuantity;
	m_iWaitForConfirmSellED = iResultSellUnitEDIN;
	m_iWaitForConfirmSellCommisionED = iiSellCommissionED;

	// 판매되고 있는 아이템 수량을 깎는다!
	const int iBeforeQuantity = GetQuantity();
	m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity -= iBuyQuantity;
	LIF( m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity >= 0 );

	// 판매자가 받을 금액을 저장한다.
	m_iTotalSoldItemQuantity += iBuyQuantity;
	m_iTotalSellEDIn += iResultSellUnitEDIN;
	m_iTotalSellCommissionED += iiSellCommissionED;

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
#else
	//////////////////////////////////////////////////////////////////////////	
	kSellItemDBUpdateInfo.m_iItemUID = GetItemUID();
	kSellItemDBUpdateInfo.m_iQuantity = GetQuantity() - iBeforeQuantity; // 증분값!
	kSellItemDBUpdateInfo.m_iTotalSellEDIn = m_iTotalSellEDIn;
	kSellItemDBUpdateInfo.m_iTotalSoldItemQuantity = m_iTotalSoldItemQuantity;
	kSellItemDBUpdateInfo.m_iTotalSellCommissionED = m_iTotalSellCommissionED;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
    
	// 판매 처리 중 상태로 변경!
	SetShopItemState( SIS_PROCESSING );
	return true;
}

void KPersonalShopItem::Confirm( IN const bool bSuccess )
{
	LIF( IsPShopAgencyItemUID() );

	if( bSuccess == false )
	{
		// 컨펌이 실패하면 롤백처리!
		m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity += m_iWaitForConfirmQuantity;
		m_iTotalSoldItemQuantity -= m_iWaitForConfirmQuantity;
		m_iTotalSellEDIn -= m_iWaitForConfirmSellED;
		m_iTotalSellCommissionED -= m_iWaitForConfirmSellCommisionED;
	}

	// 컨펌 백업 데이터는 초기화!
	m_iWaitForConfirmQuantity = 0;
	m_iWaitForConfirmSellED = 0;
	m_iWaitForConfirmSellCommisionED = 0;

	// 다시 판매 상태로 변경!
	if( GetQuantity() > 0 )
	{
		SetShopItemState( SIS_SALE );
	}
	else
	{
		SetShopItemState( SIS_SOLD_OUT );
	}
}


#endif SERV_PSHOP_AGENCY
//}}

