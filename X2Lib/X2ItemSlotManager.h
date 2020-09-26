#pragma once

class CX2ItemSlotManager : public CX2SlotManager
{

public:
	CX2ItemSlotManager( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2ItemSlotManager(void);

	virtual void SetSlotManagerLayer( int slotManagerLayer );
	

	//////////////////////////////////////////////////////////////////////////
	//상속받은 클래스에서 꼭 불러줘야하는 함수들

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	
	//////////////////////////////////////////////////////////////////////////
	

	virtual void MouseMove( D3DXVECTOR2 mousePos );
	virtual bool MouseDown( D3DXVECTOR2 mousePos );

	virtual bool DrawSlotMouseOverImageInEquipRect() { return false; }
	virtual void DrawSlotMouseOverImage();
	virtual void DrawSlotDesc();
	virtual void InvalidSlotDesc();
	virtual void InvalidSelectedItem();

    // 슬롯 관련
	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescByUID( UidType itemUID, bool bItemDescExtra = true );
	virtual wstring GetSlotItemDescByTID( CX2Item* pItem, int itemTID, bool bItemDescExtra = true, bool bMyUnitItem_ = false);
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );

	virtual wstring GetManufactureMaterialDesc( CX2ItemManager::ManufactureData* pManufactureData );//, CX2Item::ItemTemplet* pItemTemplet );
	//{{ JHKang // 강정훈 // 2010. 10. 6 // 자동 줄나누기 두 번 실행 방지
#ifdef FIX_TOOLTIP
	virtual wstring GetEnchantDesc( CX2Item* pItem, bool bTotal );
#else
	virtual wstring GetEnchantDesc( CX2Item* pItem );
#endif FIX_TOOLTIP
	//}} JHKang // 강정훈 // 2010. 10. 6 // 자동 줄나누기 두 번 실행 방지

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	#ifdef SERV_NEW_ITEM_SYSTEM_2013_05 // 오현빈
		virtual wstring GetSocketDesc( const vector<int>& vecSocketOption, const wstring& wstrTitle, const int iSocketLevel_,  CX2Unit::UNIT_CLASS eUnitClass_ = CX2Unit::UC_NONE );
	#else
		virtual wstring GetSocketDesc( const vector<int>& vecSocketOption, const wstring& wstrTitle, const int iSocketLevel_ );
	#endif // SERV_NEW_ITEM_SYSTEM_2013_05
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업	

	virtual wstring GetExpirationDateDesc( CX2Item* pItem );
	virtual wstring GetSlotCashItemPeriodDesc( int itemTID ) { return L""; }
	virtual wstring GetSlotCashItemDesc( int itemTID ) { return L""; }

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	virtual wstring GetSetItemDesc( CX2Item* pItem, int itemTID, const int iUserLevel_ );
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	
    const CX2Item::ItemTemplet* GetNowOverSlotItemTemplet();

	//잘못된 슬롯 찾는 시스템이다 이거 쓰면 안됨 이거 고쳐야함
	CX2SlotItem* GetSlotByID( int iSlotID );
	CX2SlotItem* GetSlotFindRes( int iIdx );

	void FindSlotBySlotType( CX2SlotManager::CX2Slot::SLOT_TYPE eSlotType );
	int GetSlotFindResCount();

	virtual bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID );

	virtual void SetNowDragItemReturn();

	virtual void SetShowRBButtonDownMessage( bool bShow ) { m_bShowRBDownMessage = bShow; }

	// 유틸 함수
public:
	virtual bool GetCheckOperationCondition();
	bool IsInEquippedItemSlotReact( D3DXVECTOR2 mousePos );
#ifdef SERV_CUBE_IN_ITEM_MAPPING
	void SetToolTipUseOpenRandomID(std::map< int,std::map<int, kRandomItemMappingToInfoClient> > mapToolTip) {m_mapToolTipUseOpenRandomID = mapToolTip;}
#endif //SERV_CUBE_IN_ITEM_MAPPING

protected:

	virtual wstring GetSpecialAbilityDesc( int itemID );

#ifdef BUFF_TEMPLET_SYSTEM
	virtual wstring GetSocketOptionItemDesc( int itemID );
#endif BUFF_TEMPLET_SYSTEM

	virtual int	GetItemLayer() { return m_SlotManagerLayer; }
	virtual int GetSelectedItemLayer() { return m_SlotManagerLayer + 1; }
	
	//virtual int GetDraggingItemLayer() { return m_SlotManagerLayer + 2; } 
	virtual int GetDraggingItemLayer() { return (int)XDL_POP_UP; } 
	
	virtual int GetItemDescLayer() { return m_SlotManagerLayer + 3; }

	virtual CX2Item* GetCompareItem( IN int itemID, IN UidType itemUID, OUT bool& bCheckIsEquipping );
    virtual CX2Item* GetCompareItem( IN const CX2Item::ItemTemplet* pkItemTemplet, IN CX2Item* pItem, OUT bool& bCheckIsEquipping );

	virtual bool GetIsShowCompareItemDescBySlotType( CX2Slot::SLOT_TYPE slotType );

#ifdef FIX_TOOLTIP
	void SplitDescHeight( wstring* pOrgStr, wstring* pTempStr, OUT int &pHeight, OUT int &pFontHeight, int fontIndex, bool bSplit );
#endif FIX_TOOLTIP

#ifdef	PVP_SEASON2
	void WriteStringForRestrictionPurchaseToDesc( const CX2PVPEmblem::PVP_RANK ePvpRankConditionForPurchase_, const CX2PVPEmblem::PVP_RANK eMyPvpRank_, wstring& wstrSlotItemDesc_ );
#endif	PVP_SEASON2
	//CKTDGUIDialogType		m_pDLGItemDesc;
	//CKTDGUIDialogType		m_pDLGItemDescCompare;
	CKTDGUIDialogType		m_pDLGSelectedItem;

	D3DXVECTOR2				m_MyInfoSlotItemReactLeftTop;
	D3DXVECTOR2				m_MyInfoSlotItemReactRightBottom;

	// 드래그 관련
#ifdef NEW_VILLAGE_UI
	UidType*				m_DraggingItemUID;
#else
	UidType					m_DraggingItemUID;
#endif
	int						m_DraggingItemTID;
	D3DXVECTOR2				m_NowMousePos;	


	// 검색 결과 리스트
	vector< CX2SlotItem* >	m_FindedSlotList;

	bool					m_bShowRBDownMessage;

#ifdef USE_FREE_TYPE
	static const int		m_iDescFontHeightOffset = 2;
#endif // USE_FREE_TYPE
#ifdef SERV_CUBE_IN_ITEM_MAPPING
	std::map< int,std::map<int, kRandomItemMappingToInfoClient> >		m_mapToolTipUseOpenRandomID;		// 마우스 오른 클릭으로 열었던 아이템 큐브 아이템과 그룹ID저장
#endif //SERV_CUBE_IN_ITEM_MAPPING
};
