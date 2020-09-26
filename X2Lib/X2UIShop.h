#ifdef NEW_VILLAGE_UI

#pragma once

//class CX2StateMenu;

class CX2UIShop : public CX2ItemSlotManager
{
public:

	struct BuySlot
	{
		CKTDGUIButton*			m_pBuyButton;
		CKTDGUIStatic*			m_pEDStatic;
		CKTDGUIStatic*			m_pEDVPStatic;
        const CX2Item::ItemTemplet*    m_pItemTemplet;

		BuySlot()
		{
			m_pBuyButton = NULL;
			m_pEDStatic = NULL;
			m_pEDVPStatic = NULL;
			m_pItemTemplet = NULL;
		}
		~BuySlot()
		{
			m_pItemTemplet = NULL;
		}
	};

	enum UI_SHOP_CUSTOM_MSG
	{
		USCM_EXIT				= 20000,

		USCM_CATEGORY_WEAPON,
		USCM_CATEGORY_COAT,
		USCM_CATEGORY_PANTS,
		USCM_CATEGORY_GLOVE,
		USCM_CATEGORY_SHOES,
		USCM_CATEGORY_SPECIAL,
		USCM_CATEGORY_ACCESSORY,
		USCM_CATEGORY_QUICKSLOT,
        
		USCM_SHOW_OTHER_CHARACTER_EQUIP_ON,
		USCM_SHOW_OTHER_CHARACTER_EQUIP_OFF,

		USCM_PREV_PAGE,
		USCM_NEXT_PAGE,

		//////////////////////////////////////////////////////////////////////////

		USCM_BUY,

		USCM_BUY_ITEM_CONFIRM_OK,
		USCM_BUY_ITEM_CONFIRM_CANCLE,

		USCM_BUY_ITEM_QUANTITY_CONFIRM,
		USCM_BUY_ITEM_QUANTITY_CANCLE,
		USCM_BUY_ITEM_QUANTITY_PLUS,
		USCM_BUY_ITEM_QUANTITY_MINUS,
		USCM_BUY_ITEM_QUANTITY_UPDATE,

		//////////////////////////////////////////////////////////////////////////

		USCM_SELL_ITEM_CONFIRM_OK,
		USCM_SELL_ITEM_CONFIRM_CANCLE,

		USCM_SELL_ITEM_QUANTITY_CONFIRM,
		USCM_SELL_ITEM_QUANTITY_CANCLE,
		USCM_SELL_ITEM_QUANTITY_PLUS,
		USCM_SELL_ITEM_QUANTITY_MINUS,
		USCM_SELL_ITEM_QUANTITY_UPDATE,

		//////////////////////////////////////////////////////////////////////////

		USCM_REPAIR_ITEM_CONFIRM_OK,

		USCM_REPAIR_ITEM_ALL,
		USCM_REPAIR_ITEM_ALL_CONFIRM_OK,
		USCM_REPAIR_ITEM_ALL_CONFIRM_CANCLE,

		//////////////////////////////////////////////////////////////////////////

		USCM_ENCHANT_ITEM_EXIT,
		USCM_ENCHANT_ITEM_OLD_OK,
		USCM_ENCHANT_ITEM_NEW_OK,

		USCM_ENCHANT_ITEM_RESULT_OK,

		USCM_ENCHANT_ADJUVANT_CHECKBOX,
		
		//////////////////////////////////////////////////////////////////////////

		USCM_ENCHANT_ATTRIBUTE_ITEM_EXIT,
		USCM_ENCHANT_ATTRIBUTE_ITEM_SELECT,
		USCM_ENCHANT_ATTRIBUTE_ITEM_REMOVE,
		USCM_ENCHANT_ATTRIBUTE_ITEM_CONFIRM,
		USCM_ENCHANT_ATTRIBUTE_ITEM_REMOVE_CONFIRM,
		//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가 
//#ifdef SERV_DESTROY_GUARD_ITEM
		USCM_ENCHANT_PLUS_CHECKBOX,
//#endif // SERV_DESTROY_GUARD_ITEM
		//}}
		//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
//#ifdef SERV_DESTROY_GUARD_ITEM
		USCM_ENCHANT_DESTROY_GUARD_CHECKBOX,
//#endif // SERV_DESTROY_GUARD_ITEM
		//}}
//#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
		USCM_EVENT_EXCHANGE_ITEM,
//#endif ALWAYS_EVENT_ADAMS_UI_SHOP
	};


	enum BUY_ITEM_CATEGORY
	{
		BIC_WEAPON,
		BIC_COAT,
		BIC_TROUSERS,
		BIC_HAND,
		BIC_FOOTWEAR,
		BIC_SPECIAL,
		BIC_ACCESSORY,
		BIC_QUICK_SLOT,
		BIC_COUNT,
	};



public:

	CX2UIShop( CKTDXStage* pNowStage, const WCHAR* pFileName );
	~CX2UIShop();
	
	// 기본적으로 해 줘야 하는 것
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//슬롯 관련
	virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );
	// 
	// 	void DrawSlotMouseOverImage();
	// 	wstring GetSlotItemDesc();


	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow( bool val );
	
	void SetLayer(X2_DIALOG_LAYER layer);
	void SetPosition(D3DXVECTOR2 vec);

	void SetHouseID(int HouseID){ m_HouseID = HouseID; }

	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

	// 진입점들
	void SellItem( UidType sellItemUID, D3DXVECTOR2 pos = D3DXVECTOR2(300, 300) );
	void RepairItem( UidType RepairItemUID );
	void RepairAllEquippedItem();
	void EnchantItem( UidType EnchantItemUID, D3DXVECTOR2 pos = D3DXVECTOR2(300, 300) );
	void EnchantAttributeItem( UidType AttributeEnchantItemUID, D3DXVECTOR2 pos = D3DXVECTOR2(300, 300) );
	
#ifdef AUTH_AUTO_SELL_ITEM
	void AuthAutoSellItem( CX2Inventory::SORT_TYPE SortType );
#endif

protected:

	// 판매
	void OpenItemSellNumDLG( D3DXVECTOR2 pos );
	void UpdateItemSellNumDLG( bool bReadIME, bool bTextSelected = false );
	void OpenSellItemConfirmDLG();
	bool Handler_EGS_SELL_ITEM_REQ();
	bool Handler_EGS_SELL_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 수리
	bool Handler_EGS_REPAIR_ITEM_REQ( UidType ItemUID );
	bool Handler_EGS_REPAIR_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void AllEquippingItemRepairREQ();


	// 구매
	void ResetBuySlotList( int nPage );
	void BuyUISetting();
	void OpenItemBuyNumDLG( D3DXVECTOR2 pos );
	void UpdateItemBuyNumDLG( bool bReadIME, bool bTextSelected = false );
	void OpenBuyItemConfirmDLG();
	virtual bool Handler_EGS_BUY_ED_ITEM_REQ();
	virtual bool Handler_EGS_BUY_ED_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	int PreCheckShopBuyItem();

	//강화
	void OpenEnchantDLG( D3DXVECTOR2 pos);
	void UpdateEnchantWindow();
	int GetEnchantStoneLevel( int iItemLevel );
	bool Handler_EGS_ENCHANT_ITEM_REQ( bool bSpecialEnchant );
	bool Handler_EGS_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OpenEnchantResultWindow( KEGS_ENCHANT_ITEM_ACK& kEGS_ENCHANT_ITEM_ACK );
	

#ifdef FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING	
	void SplitEnchantDLGStringSet ( CKTDGUIStatic*  pStatic, wstring strItemName );
#endif // FIXED_DIALOG_UPGRADE_ITEM_SPLIT_STRING	

	//강화보조
	void GetRequiredEnchantAdjuvantItemID( IN const int iItemLevel_, OUT vector<int>& vecAdjuvantIdList_ );
	int GetNumOfRequiredEnchantAdjuvantItem( IN const vector<int>& vecAdjuvantIdList_ );
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	void GetRequiredEnchantPlusItemID( const int iItemLevel, OUT vector<int>& vecPlusIdList_ );
	int GetNumOfRequiredEnchantPlusItem( IN const vector<int>& vecPlusIdList_ );
#endif // SERV_ENCHANT_PLUS_ITEM
	//}}
	//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가 
#ifdef SERV_DESTROY_GUARD_ITEM
	void GetRequiredDestroyGuardItemID( const int iItemLevel, OUT vector<int>& vecGuardIdList_ );
	int GetNumOfRequiredDestroyGuardItem( IN const vector<int>& vecGuardIdList_ );
#endif // SERV_DESTROY_GUARD_ITEM
	//}}
	
#ifdef SERV_CATCH_HACKUSER_INFO
	bool Handler_EGS_CATCH_HACKUSER_INFO_NOT( KEGS_CATCH_HACKUSER_INFO_NOT& kEGS_CATCH_HACKUSER_INFO_NOT );
#endif SERV_CATCH_HACKUSER_INFO	

	//속성 강화 관련
	void ResetAtrribEnchantWindow();
	bool Handler_EGS_ATTRIB_ENCHANT_ITEM_REQ( int slotNum, int enchantID, bool bDebug );
	bool Handler_EGS_ATTRIB_ENCHANT_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 내부 유틸리티 함수
	CKTDGUIRadioButton* GetRadioButtonNameByCategory( BUY_ITEM_CATEGORY buyItemCategory );
    bool IsPossibleUsedByMyCharacter( const CX2Item::ItemTemplet* pItemTemplet );

	//{{ kimhc // 2009-09-16 // 길드 창단 허가서 or 길드 확장권을 구입을 할 수 있는가?
#ifdef	GUILD_MANAGEMENT
	bool CanBuyItemToCreateGuild() const;
	bool CanBuyItemToExpandGuild() const;
#endif	GUILD_MANAGEMENT
	//}} kimhc // 2009-09-16 // 길드 창단 허가서 or 길드 확장권을 구입을 할 수 있는가?
	
	//{{ JHKang / 강정훈 / 2010/10/12
#ifdef MODIFY_SHOP_BUY_ITEM
	CX2Inventory::SORT_TYPE GetCurrInventoryType( CX2Item::ITEM_TYPE type );
#endif MODIFY_SHOP_BUY_ITEM
	//}} JHKang / 강정훈 / 2010/10/12

#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
	bool Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_REQ( );
	bool Handler_EGS_SUPPORT_MATERIAL_EVENT_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
#ifdef ALWAYS_EVENT_ADAMS_UI_SHOP
	bool Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif ALWAYS_EVENT_ADAMS_UI_SHOP
private:

	// 공용
	bool					m_bShow;
	D3DXVECTOR2				m_MovedPosition;
	int						m_HouseID;			// X2LocationManager::HOUSE_ID
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	// 다이얼로그
	CKTDGUIDialogType			m_pDLGShop;

	CKTDGUIDialogType			m_pDLGSellItemNum;
	CKTDGUIDialogType			m_pDLGSellItemConfirm;

	CKTDGUIDialogType			m_pDLGRepairItemConfirm;
	
	CKTDGUIDialogType			m_pDLGBuyItemNum;
	CKTDGUIDialogType			m_pDLGBuyItemConfirm;

	CKTDGUIDialogType			m_pDLGEnchantItem;
	CKTDGUIDialogType			m_pDLGEnchantItemResult;

	CKTDGUIDialogType			m_pDLGAttribEnchantItem;
	CKTDGUIDialogType			m_pDLGAttribEnchantItemConfirm;


	// - - - - - - - - - - - - - - - - - - - - - - - - -
	// 구매 아이템 관련

	// ( 화면에 8개씩 보이는 ) 아이템 리스트
	// 화면에 보이는 아이템 갯수는 LUA에 있는 슬롯 수로 판단한다 -> m_SlotList.size()
	vector<BuySlot*>		m_vecBuySlot;
	int						m_nNowPage;
	int						m_nMaxPage;
	bool					m_bShowAllCharItem;

	// 구매
	int						m_BuyItemNum;
	int						m_BuyItemSlotNum;
	
	// 살 수 있는 아이템 전체 리스트, 종류별로 
    vector<const CX2Item::ItemTemplet*> m_vecItemList[BIC_COUNT];
	BUY_ITEM_CATEGORY		m_NowBuyItemCategory;

	// 판매
	UidType					m_SellItemUID;
	int						m_SellItemNum;
	bool					m_bQuickSell;

	// 수리
	UidType					m_RepairItemUID;
	vector< UidType >		m_vecAllEquippingItem;

	// 강화
	UidType					m_EnchantItemUID;
	bool					m_bUseEnchantAdjuvant;
	int						m_EnchantLevelBefore;
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가
#ifdef SERV_ENCHANT_PLUS_ITEM
	bool					m_bUseEnchantPlus;
#endif // SERV_ENCHANT_PLUS_ITEM
		//}}
		//{{ 2011.5.23 지헌 : 강화시 파괴 방지 아이템 추가
#ifdef SERV_DESTROY_GUARD_ITEM
	bool					m_bUseDestroyGuard;
#endif // SERV_DESTROY_GUARD_ITEM
		//}}
	
	// 속성
	UidType					m_AttributeEnchantItemUID;
	int						m_AttribEnchantID;
	int						m_AttribEnchantSlotID;

	// 마우스 휠 관련
	SHORT					m_SumDelta;

#ifdef SERV_SUPPORT_MATERIAL_ENCHANT_EVENT	
	int						m_iAddEnchantLevel;	/// MAGIC_ENCHANT_LEVEL_LIMIT 기본 강화 제한 레벨의 추가 값(이벤트 용)
#endif //SERV_SUPPORT_MATERIAL_ENCHANT_EVENT
};

#endif
