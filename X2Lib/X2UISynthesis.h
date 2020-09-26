#ifdef SERV_SYNTHESIS_AVATAR

#pragma once

class CX2UISynthesisItem : public CX2UIDragable
{
public:

	enum UI_SYNTHESIS_ITEM_CUSTOM_MSG
	{	
		USICM_SYNTHESIS_ITEM_OK					= 49200,
		USICM_SYNTHESIS_ITEM_CANCLE,			
		USICM_SYNTHESIS_ITEM_RESULT_OK,
		USICM_SYNTHESIS_ITEM_OPTION_CHANGED,
	};

	struct SynthesisItemSlot
	{
		CX2Inventory::SORT_TYPE		m_SORT_TYPE;
		UidType						m_iItemUID;
		CX2SlotItem*				m_pImteSlot;

		SynthesisItemSlot()
		{
			m_SORT_TYPE	= CX2Inventory::ST_NONE;
			m_iItemUID	= -1;
			m_pImteSlot = NULL;
		}
	};

	CX2UISynthesisItem( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UISynthesisItem(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool		GetShow() const { return m_bShow; }
	void		SetShow( bool bShow );

	void RegisterSynthesisItem(CX2SlotItem* pItemSlot);
	bool CheckSynthesisUseItem(UidType Uid);
	bool AttachItem();
	bool DeleteItem();
	bool KeyItem();

	void OpenSynthesisResultWindow( std::map< int, int >& mapInsertedItem, std::vector< KInventoryItemInfo >& vecKInventorySlotInfo);
	
	void OpenEffect();
	void SetSlotItem(CX2Item *pkItem);

	bool SetInventorySort();
	bool UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo);
	virtual void AddEqip(UidType ItemUID);
	virtual void RemoveEqip(UidType ItemUID);
	virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );	
	
	bool CheckUseItem(UidType Uid);
	void ShowSynthesisCheckPicture (CX2Inventory::SORT_TYPE sortType );
	void OffSynthesisCheckPicture();

	void SetLayer(X2_DIALOG_LAYER layer);
	void SetPosition(D3DXVECTOR2 vec);
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

protected:

	bool Handler_EGS_SYNTHESIS_ITEM_REQ();
	bool Handler_EGS_SYNTHESIS_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_SYNTHESIS_SOCKET_GROUPID_REQ();
	bool Handler_EGS_SYNTHESIS_SOCKET_GROUPID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void UpdateOpenSynthesisItemAck();

	wstring GetSlotItemDesc();

	void AttachSelectedPicture( CX2SlotItem* pItemSlot );
	void SetShowSelectedPicture( CX2SlotItem* pItemSlot, bool bShow );

	bool CheckSameUnitEquip( CX2Item* pkItem );
	void AddOption();
	void ChangeItemOption( CKTDGUIDialogType pDialog, int comboBoxIndex ); // 콤보박스에서 옵션 정하자
	void SelectSocketNo();

private:
 
	int							m_iComboBoxIndex;
	int							m_iSocketNo;
	
	bool					m_bPlaySynthesisItem;
 	bool					m_bShow;
	
	D3DXVECTOR2				m_DLGSize;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_MovedPosition;

	float					m_fPlayTime;
 	
	CKTDGUIDialogType			m_pDLGSynthesis;
	CKTDGUIDialogType			m_pDLGSynthesisItemResult;
	CKTDGUIDialogType			m_pDLGOpenEffect;

	CX2Inventory::SORT_TYPE		m_NowInventorySortType;
	
	std::map< int, int >		m_mapAvatarSocketOptionGroupID;

	CX2Item*					m_pItemData;
	std::map<int, UidType>		m_mapItemData;
	std::map< int, int >        m_mapSynthesisResultInsertedItem;
	UidType						m_iKeyData;
	std::vector<SynthesisItemSlot> m_vecSynthesisSlot;
	
	
};

#endif
