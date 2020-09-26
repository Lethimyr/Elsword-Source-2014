#ifdef NEW_VILLAGE_UI

#pragma once

class CX2UIWatch : public CX2ItemSlotManager
{
public:
	enum UI_WATCH_CUSTOM_MSG
	{
		UWCM_EXIT = 13000,
	};

	CX2UIWatch( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIWatch(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }

	void SetShow(bool val);
	bool GetShow(){ return m_bShow; }

	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

	bool OpenDialog(UidType uid);
	void ResetEquipUI();

	void SetPosition(D3DXVECTOR2 vec);
	void SetLayer(X2_DIALOG_LAYER layer);

	// À¯´Öºä¾î °ü·Ã
	CX2UnitViewerUI* GetUnitViewerUI() { return m_pUnitViewerUI; }
	void ResetUnitViewerUI();
	bool IsInUnitClickReact( D3DXVECTOR2 mousePos );
	void UnitHandling( double fTime, float fElapsedTime );

// 	void DrawSlotMouseOverImage();
 	wstring GetSlotItemDesc();
	virtual wstring GetSlotItemDescByTID( CX2Item* pItem, int itemTID, bool bItemDescExtra = true, bool bMyUnitItem_ = false );

	CX2Unit* GetUnitByUid( UidType uid );
	UidType GetTargetUnitUID() const { return m_TargetUnitUID; }

private:
	bool					m_bShow;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGUIDialogType			m_pDLGWatch;   

	CX2UnitViewerUI*		m_pUnitViewerUI;

	// À¯´Öºä¾î°ü·Ã
	float					m_fRemainRotY;
	float					m_fMouseSensitivity;
	bool					m_bRotateReturn;
	D3DXVECTOR2				m_UnitClickPos;
	D3DXVECTOR2				m_UnitClickSize;
	bool					m_bUnitClicked;

	CX2Unit*				m_pTargetUnit;
	UidType					m_TargetUnitUID;



	D3DXVECTOR2				m_MovedPosition;

	std::set<wstring>		m_setNoOpenNickNameList;

};

#endif
