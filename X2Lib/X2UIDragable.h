#ifdef NEW_VILLAGE_UI

#pragma once

//class CX2StateMenu;
class CX2UIDragable : public CX2ItemSlotManager
{
public:

	CX2UIDragable( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UIDragable(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool MouseDown( D3DXVECTOR2 mousePos );
	virtual bool MouseUp( D3DXVECTOR2 mousePos );
	virtual void MouseMove( D3DXVECTOR2 mousePos );
	virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){ return false; }

	virtual void SetNowDragItemReturn();

	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos ){ return false; }
	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos ){ return false; }

	virtual void DrawSlotMouseOverImage();
	virtual wstring GetSlotItemDesc();

protected:
	
};

#endif
