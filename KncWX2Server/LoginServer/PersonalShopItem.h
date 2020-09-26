#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"

//{{ 2011. 04. 25	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY


// 스마트 포인터 정의
#define KPersonalShopItemPtr KPersonalShopItem*
#undef KPersonalShopItemPtr
SmartPointer( KPersonalShopItem );

//#define PS_OPENING_ED	1000	//임시로 사용될 상점개설 비용.. 추후 개설에 대한 로직이 정해지면 삭제한다. 
#define TD_COMMISSION	0.03	//거래 게시판 수수료

class KPersonalShopItem
{
public:
	enum SHOP_ITEM_STATE
	{
		SIS_SALE = 0,
		SIS_PROCESSING,
		SIS_SOLD_OUT,
		SIS_PICK_UP,
	};

public:
	KPersonalShopItem( const KSellPersonalShopItemInfo& kInfo );
	KPersonalShopItem( const KSellPShopItemBackupData& kInfo );
	virtual ~KPersonalShopItem(void);

	void	Clear();

	void	GetSellPersonalShopItemInfo( OUT KSellPersonalShopItemInfo& kInfo ) const;
	void	SetSellPersonalShopItemInfo( IN const KSellPersonalShopItemInfo& kInfo ) { m_kSellItemInfo = kInfo; }

	void	SetPShopAgencyItemUID( IN const UidType iItemUID );
	bool	IsPShopAgencyItemUID() const				{ return ( m_kSellItemInfo.m_cPShopItemType == KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY ); }

	UidType GetItemUID() const							{ return m_kSellItemInfo.m_kInventoryItemInfo.m_iItemUID; }
	int		GetItemID() const							{ return m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID; }
	char	GetSlotCategory() const						{ return m_kSellItemInfo.m_kInventoryItemInfo.m_cSlotCategory; }
	char	GetUsageType() const						{ return m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType; }
	int		GetQuantity() const							{ return m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity; }
	void	GetItemInfo( OUT KItemInfo& kInfo ) const	{ kInfo = m_kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo; }
	int		GetTotalPricePerOne() const					{ return m_kSellItemInfo.TotalPricePerOne(); }
	SHOP_ITEM_STATE GetShopItemState() const			{ return m_eItemState; }
	void	SetShopItemState( IN const SHOP_ITEM_STATE eState ) { m_eItemState = eState; }
	int		GetTotalSellEDIn() const					{ return m_iTotalSellEDIn; }
	int		GetPricePerOne() const						{ return m_kSellItemInfo.m_iPricePerOne; }

	bool	BuyItem( IN const char cPersonalShopType, 
					 IN const int iBuyQuantity, 
					 OUT int& iSellUnitEDIn,
					 OUT int& iPriceUnitEDOUT, 
					 OUT int& iPriceCommissionED, 
					 OUT KInventoryItemInfo& kPriceIntemInfoIN,
					 OUT KSellPShopItemDBUpdateInfo& kSellItemDBUpdateInfo );

	void	Confirm( IN const bool bSuccess );

private:
	SHOP_ITEM_STATE				m_eItemState;
	KSellPersonalShopItemInfo	m_kSellItemInfo;
	int							m_iTotalSoldItemQuantity;
	int							m_iTotalSellEDIn;
	int							m_iTotalSellCommissionED;
	
	int							m_iWaitForConfirmQuantity;
	int							m_iWaitForConfirmSellED;
	int							m_iWaitForConfirmSellCommisionED;
};


#endif SERV_PSHOP_AGENCY
//}}

