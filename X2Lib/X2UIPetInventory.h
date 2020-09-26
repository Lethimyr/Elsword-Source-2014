
#ifdef SERV_PET_SYSTEM
//{{ oasis907 : 김상윤 [2010.8.31] // 펫 인벤토리

#pragma once

class CX2UIPetInventory : public CX2UIDragable
{
public:

	enum	PET_INVENTORY_UI_CUSTOM_MSG
	{
		PIUCM_EXIT			= 45000,
		PIUCM_CHECK_AUTO_FEED,
		PIUCM_AUTO_FEED_MOUSE_OVER,
		PIUCM_AUTO_FEED_MOUSE_OUT,
	};

	enum	POSITION_STATE
	{
		NORMAL,
		UISOCKET,
		UITEMPINVENTORY,
	};



	
	CX2UIPetInventory( const CKTDXStage* const pNowStage, int iLayer);
	virtual	~CX2UIPetInventory();

	bool				InitUIPetInventory(int iLayer);
	void				DestoryPetInventory();

	virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
	virtual bool		MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const { return m_bShow; }
	void				SetShow( bool bShow );

	void				SetLayer( int layer );
	D3DXVECTOR2			GetDLGSize() const { return m_vDlgSize; }
//{{ robobeg : 2013-11-01
	//CX2Inventory*		GetInventory();								// 후에 const를 붙여보기!!
    const CX2Inventory*		GetMyInventory() const;								// 후에 const를 붙여보기!!
    CX2Inventory*		AccessMyInventory();								// 후에 const를 붙여보기!!
//}} robobeg : 2013-11-01
	void				UpdateSlotList();
	void				UpdateUI();
	void				LostItemSlotList( bool bShow = false, bool bEnable = false );
	
	void				MoveDlgPosition();
	void				MovePosition( D3DXVECTOR2 vec );


	virtual void		DrawSlotMouseOverImage();
	virtual wstring		GetSlotItemDesc();

	virtual bool		MouseDown( D3DXVECTOR2 mousePos );
	virtual bool		MouseUp( D3DXVECTOR2 mousePos );
	virtual bool		OnDropAnyItem( D3DXVECTOR2 mousePos );
	virtual bool		OnRClickedItem( D3DXVECTOR2 mousePos );

	void ShowUIDesc( bool bShow, wstring wstr = L"", D3DXVECTOR2 pos = D3DXVECTOR2(0,0), 
		CX2TalkBoxManagerImp::TALK_BOX_TYPE TBT = CX2TalkBoxManagerImp::TBT_FROM_UP_LEFT,
		D3DXCOLOR coTextColor = D3DXCOLOR(0,0,0,1), D3DXCOLOR coBackColor = D3DXCOLOR(1, 0.96f, 0.6f, 1 ) );

	POSITION_STATE		NowPositionState;


	void				Handler_EGS_SET_AUTO_FEED_PETS_REQ( bool bAutoFeed );
	bool				Handler_EGS_SET_AUTO_FEED_PETS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	CKTDGUIDialogType				m_pDlgPetInventory;
	bool							m_bShow;
	D3DXVECTOR2						m_vDlgSize;

	// ToolTip : TalkBox 이용해서.
	CX2TalkBoxManager*		m_pTalkBoxManager;


};
//}} oasis907 : 김상윤 [2010.8.31] // 펫 인벤토리
#endif SERV_PET_SYSTEM
