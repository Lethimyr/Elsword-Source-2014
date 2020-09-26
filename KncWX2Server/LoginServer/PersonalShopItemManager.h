#pragma once

#include "PersonalShopItem.h"


//{{ 2011. 04. 20	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY

class KPersonalShopItemManager
{
public:
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	enum ITEM_LIST_TYPE
	{
		ILT_ALL_ITEMS = 0,
		ILT_ONLY_ON_SALE_ITEMS,
		ILT_ONLY_SOLD_OUT_ITEMS,
	};
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

public:
	KPersonalShopItemManager(void);
	~KPersonalShopItemManager(void);

	void Clear();	
	bool IsEmptySellItemList() const { return m_mapSellItemInfo.empty(); }
	bool IsExistItem( IN const UidType iItemUID ) const;
	u_int GetSellItemCount() const { return m_mapSellItemInfo.size(); }
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	void GetSellPersonalShopItemInfoList( OUT std::vector< KSellPersonalShopItemInfo >& vecItemList, IN const ITEM_LIST_TYPE eType = ILT_ONLY_ON_SALE_ITEMS );
#else
	void GetSellPersonalShopItemInfoList( OUT std::vector< KSellPersonalShopItemInfo >& vecItemList, IN const bool bAllItems = false );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	
	bool GetSellPersonalShopItemInfo( IN const UidType iItemUID, OUT KSellPersonalShopItemInfo& kItemInfo );
	KPersonalShopItemPtr GetPersonalShopItem( IN const UidType iItemUID );

	void UpdateSellItemList( IN const std::vector< KSellPersonalShopItemInfo >& vecSellItemList );
	void UpdateSellItemBackupDataList( IN const std::vector< KSellPShopItemBackupData >& vecSellItemList );

	bool PrepareForBuyItem( IN const UidType iHostUnitUID, IN const char cPersonalShopType, IN const KERM_BUY_PERSONAL_SHOP_ITEM_REQ& kReq, OUT KDBE_BUY_PSHOP_AGENCY_ITEM_REQ& kResult );
	bool Confirm( IN const UidType iItemUID, IN const bool bSuccess );

	bool IsAllItemSoldOut() const;
	//{{ 2011. 05. 24	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	bool IsAllItemOnSale() const;
#endif SERV_PSHOP_AGENCY_NEW
	//}}

	bool PrepareForPickUp( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ& kResult );
	bool PickUp( IN const std::vector< UidType >& vecPickUpItemList, OUT KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ& kResult );
	void UnPickUp( IN const std::vector< UidType >& vecPickUpFailList );
	//{{ 2011. 05. 23	최육사	대리 상인 개편
#ifdef SERV_PSHOP_AGENCY_NEW
	void PickUpComplete( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK& kResult, IN const ITEM_LIST_TYPE eType );
#else
	void PickUpComplete( IN const std::vector< UidType >& vecPickUpItemList, OUT KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK& kResult );
#endif SERV_PSHOP_AGENCY_NEW
	//}}	

protected:
	bool AddSellItemInfo( IN const KSellPersonalShopItemInfo& kInfo );
	bool AddSellItemInfo( IN const KSellPShopItemBackupData& kInfo );
	void DelSellItemInfo( IN const UidType iItemUID );

protected:
	std::vector< KPersonalShopItemPtr >			m_vecSellItemInfo;
	std::map< UidType, KPersonalShopItemPtr >	m_mapSellItemInfo;
	std::map< UidType, KPersonalShopItemPtr >	m_mapSoldPShopAgencyItemInfo;	// 팔린 아이템 정보
};


#endif SERV_PSHOP_AGENCY
//}}