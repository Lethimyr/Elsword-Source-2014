#pragma once

class CX2StateSquareGame : public CX2StateMenu
{
public:
	friend class CX2StateAutoChanger;

	enum STATE_SQUARE_GAME_UI_CUSTOM_MSG
	{
		SQGUCM_LEAVE_SQUARE_GAME = 0,
		SQGUCM_LEAVE_SQUARE_GAME_CANCEL,

        SQGUCM_ACCEPT_QUEST_MOON,
        SQGUCM_CANCEL_QUEST_MOON,
	};
	
public:
	CX2StateSquareGame(void);
	virtual ~CX2StateSquareGame(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	void LeaveSquareGame() { Handler_EGS_LEAVE_SQUARE_REQ(); }

	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

protected:
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ( UINT nMapID );
	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_SQUARE_UNIT_SYNC_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_JOIN_SQUARE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_LEAVE_SQUARE_REQ( int leaveRoomReason = NetError::NOT_LEAVE_ROOM_REASON_00 );
	bool Handler_EGS_LEAVE_SQUARE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_SQUARE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool ClickMenuBackButton();
	void SetShowStateDLG( bool bShow );

	bool ToggleLobbyMenuWindow();
	void OpenExitDLG();

	bool LastMsgByESC() { OpenExitDLG(); return true; }

	void SetShowCommonBG();

	void SetShowMenuByTrade();
	void SetShowMenuByPShop();
	void SetShowMenuByDefault();

private:
	CKTDGUIDialogType		m_pDLGExitMsgBox;
	CKTDGUIDialogType		m_pDLGFront;
	CKTDGUIDialogType		m_pDLGBack;

//	CKTDGUIDialogType		m_pDLGSquareTitle;	
	
	D3DXVECTOR3			m_vOldEyePt;
	D3DXVECTOR3			m_vOldLookAtPt;
};
