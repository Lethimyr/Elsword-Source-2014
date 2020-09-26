#pragma once

class CX2WeddingManager;

#ifdef ADDED_RELATIONSHIP_SYSTEM
class CX2StateWeddingGame : public CX2StateMenu
{
public:
	friend class CX2StateAutoChanger;

	enum STATE_WEDDING_GAME_UI_CUSTOM_MSG
	{
		SWGUCM_LEAVE_WEDDING_GAME = 59000,
		SWGUCM_LEAVE_WEDDING_GAME_CANCEL,
		SWGUCM_WEDDING_START,
		SWGUCM_WEDDING_START_OK,
		SWGUCM_WEDDING_START_CANCEL,
	};
	
public:
	CX2StateWeddingGame(void);
	virtual ~CX2StateWeddingGame(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool ShortCutKeyProcess();

	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

protected:
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_LEAVE_WEDDING_HALL_REQ();
	bool Handler_EGS_LEAVE_WEDDING_HALL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_WEDDING_HALL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_START_WEDDING_REQ();
	bool Handler_EGS_START_WEDDING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 서버에서 보내는 REQ, 서버로 응답하는 ACK
	bool Handler_EGS_START_WEDDING_ASK_REQ();
	void Handler_EGS_START_WEDDING_ASK_ACK( bool bOk_ );

	bool Handler_EGS_START_WEDDING_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_START_WEDDING_AGREE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool ClickMenuBackButton();
	void SetShowStateDLG( bool bShow );

	bool ToggleLobbyMenuWindow();
	void OpenExitDLG();

	bool LastMsgByESC() { OpenExitDLG(); return true; }

	void SetShowCommonBG();

	void CreateWeddingStartButton();
	void DeleteWeddingStartButton();
private:
	CKTDGUIDialogType		m_pDLGExitMsgBox;
	CKTDGUIDialogType		m_pDLGWeddingStart;

//	CKTDGUIDialogType		m_pDLGSquareTitle;	
	
	D3DXVECTOR3			m_vOldEyePt;
	D3DXVECTOR3			m_vOldLookAtPt;

};
#endif //ADDED_RELATIONSHIP_SYSTEM