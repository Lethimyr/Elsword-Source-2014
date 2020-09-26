#ifdef SERV_MULTI_RESOLVE

#pragma once

class CX2UIResolveItem : public CX2UIDragable
{
public:

	enum	UI_RESOLVE_ITEM_CUSTOM_MSG
	{
		URICM_RESOLVE_ITEM_OK					= 49100,
		URICM_RESOLVE_ITEM_CANCLE,
		URICM_RESOLVE_ITEM_RESULT_OK,
		URICM_RESOLVE_ITEM_RESULT_PREV_PAGE,
		URICM_RESOLVE_ITEM_RESULT_NEXT_PAGE,
	};

	struct ResolveItemSlot
	{
		CX2Inventory::SORT_TYPE		m_SORT_TYPE;
		UidType						m_iItemUID;
		CX2SlotItem*				m_pImteSlot;

		ResolveItemSlot()
		{
			m_SORT_TYPE	= CX2Inventory::ST_NONE;
			m_iItemUID	= -1;
			m_pImteSlot = NULL;
		}
	};

	CX2UIResolveItem( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIResolveItem(void);


	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const { return m_bShow; }
	void				SetShow( bool bShow );

	

	void RegisterResolveItem(CX2SlotItem* pItemSlot);
	void SetSlotItem(CX2Item *pkItem);
	bool AttachItem();
	bool DeleteItem();
	bool SetInventorySort();
	bool UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
	virtual void AddEqip(UidType ItemUID);
	virtual void RemoveEqip(UidType ItemUID);

	void OpenResolveResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo, bool bJackpot );

	void OpenMoru();

	void ChangeResolveResultPage( bool bNextPage );	
	void DrawSlotMouseOverImage();

	bool CheckResolveUseItem(UidType Uid);
	void ShowResolveCheckPicture (CX2Inventory::SORT_TYPE sortType );
	void OffResolveCheckPicture();
	void AttachSelectedPicture( CX2SlotItem* pItemSlot );
	void SetShowSelectedPicture( CX2SlotItem* pItemSlot, bool bShow );

	virtual wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );

	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );
	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos );

	virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );

	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetPosition(D3DXVECTOR2 vec, bool bAbsolute = true );

protected:
	
	bool Handler_EGS_RESOLVE_ITEM_REQ();
	bool Handler_EGS_RESOLVE_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void UpdateOpenResolveItemAck();

private:

	int						m_iTotalCostED;
	int						m_DecompositionWindowNowPageNum;

	bool					m_bShow;
	bool					m_bEnable;
	bool					m_bDungeonResolve;
	bool					m_bInvenCheck;
	bool					m_bPlayResolveItem;

	D3DXVECTOR2				m_DLGSize;
	D3DXVECTOR2				m_vItemSlotPos;
	
	float					m_fPlayTime;
	
	CKTDGUIDialogType		m_pDLGResolveItem;
	CKTDGUIDialogType		m_pDLGResolveItemResult;
	CKTDGUIDialogType		m_pDLGOpenMoru;
	
	CX2Inventory::SORT_TYPE		m_NowInventorySortType;
	CX2Item*					m_pItemData;
	std::map<int, UidType>		m_mapItemData;
	std::map< int, int >        m_mapResolveResultInsertedItem;
	std::vector<ResolveItemSlot> m_vecResolveSlot;
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshInstMoru;
};

#endif
