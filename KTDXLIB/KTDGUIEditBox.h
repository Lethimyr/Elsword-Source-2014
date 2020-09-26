#pragma once

class CKTDGUIEditBox : public CKTDGUIControl
{
	public:
		enum EDITBOX_EVENT_MSG
		{
			EEM_CHANGE = 0,
			EEM_ENTER,
		};

	public:
		CKTDGUIEditBox(void);
		virtual	~CKTDGUIEditBox(void);

		virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
		virtual	HRESULT	OnFrameRender();

		virtual	bool	HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual	bool	HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
		virtual	bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef LOGIN_KEYBOARD_SECURITY
		void	ProcessBackspace();
#endif LOGIN_KEYBOARD_SECURITY
		void	SetNormalTex_LUA( const char* pFileName, const char* key );
		void	SetDisableTex_LUA( const char* pFileName, const char* key );

		void	SetNormalPoint_LUA();
		void	SetDisablePoint_LUA();
		void	SetTextPoint_LUA();

		void	SetFont_LUA( int nFontIndex );

		void	SetCustomMsgChange( int msg )
		{
			m_CustomMsgEditBoxChange = msg;
		}
		void	SetCustomMsgEnter( int msg )
		{
			m_CustomMsgEditBoxEnter = msg;
		}
		void	SetCustomMsgFocusOut(int val) 
		{
			m_CustomMsgEditBoxFocusOut = val; 
		}
		void OnFocusOut();
		void SetEnableInput(bool bEnable)
		{
			m_bEnableInput = bEnable;
		}

		void	SetText( LPCWSTR wszText, bool bSelected = false );
		LPCWSTR	GetText()
		{
			return m_Buffer.GetBuffer();
		}
		int		GetTextLength()
		{
			return m_Buffer.GetTextSize();
		}  // Returns text length in chars excluding NULL.
		HRESULT	GetTextCopy( LPWSTR strDest, UINT bufferCount );
		void	ClearText();

		void	SetTextColor( D3DCOLOR Color )
		{
			m_TextColor = Color; 
		}	// Text color
		void	SetSelectedTextColor( D3DCOLOR Color )
		{
			m_SelTextColor = Color;
		}	// Selected text color
		void	SetSelectedBackColor( D3DCOLOR Color )
		{
			m_SelBkColor = Color;
		}	// Selected background color
		void	SetCaretColor( D3DCOLOR Color )
		{
			m_CaretColor = Color;
		}	// Caret color
		void	SetOutlineColor( bool bCheck, D3DCOLOR Color = 0xff000000 )
		{
			m_bOutlineCheck = bCheck;
			m_OutlineColor = Color;
		}

		void	SetTextColor_LUA( float colorR, float colorG, float colorB, float colorA )
		{
			m_TextColor = D3DCOLOR_COLORVALUE(colorR,colorG,colorB,colorA);
		}
		void	SetSelectedTextColor_LUA( float colorR, float colorG, float colorB, float colorA )
		{
			m_SelTextColor = D3DCOLOR_COLORVALUE(colorR,colorG,colorB,colorA);
		}
		void	SetSelectedBackColor_LUA( float colorR, float colorG, float colorB, float colorA )
		{
			m_SelBkColor = D3DCOLOR_COLORVALUE(colorR,colorG,colorB,colorA);
		}
		void	SetCaretColor_LUA( float colorR, float colorG, float colorB, float colorA )
		{
			m_CaretColor = D3DCOLOR_COLORVALUE(colorR,colorG,colorB,colorA);
		}
		void	SetOutlineColor_LUA( float colorR, float colorG, float colorB, float colorA )
		{
			m_OutlineColor = D3DCOLOR_COLORVALUE(colorR,colorG,colorB,colorA); m_bOutlineCheck = true;
		}

#ifdef BYTE_LIMIT_IN_EDIT_BOX
		void	SetByteLimit_LUA( int val ) { m_Buffer.SetByteLimit( val ); }
#endif //BYTE_LIMIT_IN_EDIT_BOX

		void	SetHideChar_( const char* hideChar );
		void	SetHideChar( WCHAR hideChar );

#ifdef DLL_BUILD
		virtual void	MoveControl( float fx, float fy );

		virtual D3DXVECTOR2 GetPos() override;
		virtual vector<D3DXVECTOR2> GetPosList() override;

		virtual void SetEditGUI( bool bEdit ) override;
		virtual bool IsSelectByEditGui( POINT pt ) override;

		void DrawEditEdge();

protected:
	bool m_bEditEdge;
	CKTDXDeviceTexture * m_pCheckedEdgeTexture;
#endif

	protected:
		void	PlaceCaret( int nCP );
		void	DeleteSelectionText();
		void	ResetCaretBlink();
		void	CopyToClipboard();
		void	PasteFromClipboard();

		int		RtWidth( RECT &rc )		{ return ( (rc).right - (rc).left ); }
		HRESULT	DrawText( LPCWSTR strText, RECT* prcDest, int nCount = -1, DWORD format = DT_LEFT | DT_VCENTER );
		HRESULT	DrawRect( RECT* prcDest, D3DCOLOR color );

		bool	ContainPTForEditBox(POINT pt);
		
		//LPDIRECT3DDEVICE9				m_pd3dDevice;
#ifdef  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		CKTDGFontManager::CUKFont*	    m_pFont;
#else
		CKTDGFontManager::CKTDGFont*	m_pFont;
#endif
		D3DXCOLOR						m_NowFontColor;
		D3DXCOLOR						m_OutlineColor;
		bool							m_bOutlineCheck;
		bool							m_bBGCheck;
		CKTDGUIControl::UITextureData*	m_pNormalTex;
		CKTDGUIControl::UITextureData*	m_pDisableTex;
		CKTDGUIControl::UIPointData*	m_pNormalPoint;
		CKTDGUIControl::UIPointData*	m_pDisablePoint;
		CKTDGUIControl::UIPointData*	m_pTextPoint;
		int								m_CustomMsgEditBoxChange;
		int								m_CustomMsgEditBoxEnter;
		int								m_CustomMsgEditBoxFocusOut;
		bool							m_bEnableInput;

		CKTDGUIUniBuffer	m_Buffer;		// Buffer to hold text
		RECT				m_rcText;		// Bounding rectangle for the text
		double				m_dfBlink;		// Caret blink time in milliseconds
		double				m_dfLastBlink;	// Last timestamp of caret blink
		bool				m_bCaretOn;		// Flag to indicate whether caret is currently visible
		int					m_nCaret;		// Caret position, in characters
		bool				m_bInsertMode;	// If true, control is in insert mode. Else, overwrite mode.
		int					m_nSelStart;	// Starting position of the selection. The caret marks the end.
		int					m_nFirstVisible;// First visible character in the edit control
		D3DCOLOR			m_TextColor;	// Text color
		D3DCOLOR			m_SelTextColor;	// Selected text color
		D3DCOLOR			m_SelBkColor;	// Selected background color
		D3DCOLOR			m_CaretColor;	// Caret color

		bool				m_bPasswordEditBox;
		WCHAR				m_HideChar;

		// Mouse-specific
		bool				m_bMouseDrag;	// True to indicate drag in progress

		// Static
		static	bool		s_bHideCaret;	// If true, we don't render the caret.

		CKTDGUIControl::VERTEX_UI			m_Vertex[4];
		CKTDXDeviceSound*					m_pSndKeyDown;
};
