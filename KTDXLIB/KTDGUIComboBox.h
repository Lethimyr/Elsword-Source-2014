#pragma once

class CKTDGUIComboBox : public CKTDGUIControl
{
	public:

		enum COMBOBOX_EVENT_MSG
		{
			CBEM_SELECTION_CHANGED = 0,
		};

		struct ComboBoxItem
		{
			WCHAR strText[256];
			void*  pData;

			RECT  rcActive;
			bool  bVisible;

			CKTDGUIControl* pControl;

			ComboBoxItem()
			{
				memset( strText, 0, sizeof(WCHAR)*256 );
				pControl = NULL;
				pData = NULL;
				bVisible = false;
				rcActive = RECT();
			}
			~ComboBoxItem()
			{
				SAFE_DELETE(pControl);			
			}
		};

	public:

		CKTDGUIComboBox(void);
		virtual ~CKTDGUIComboBox(void);

		void InitScrollBar_LUA();

		virtual HRESULT OnFrameRender();
		virtual HRESULT OnFrameRenderEnd();
		virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );

		virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
		virtual void OnHotkey();

		virtual void OnFocusOut();
		virtual void UpdateRects(); 

		HRESULT AddItem( const WCHAR* strText, void* pData, bool bSendChangeMsg = true );
		void	AddItem_LUA( const char* pName );
#ifdef COMBOBOX_ADD_ITEM_STR_ID
		void	AddItemStrID_LUA( int iIndex );
#endif COMBOBOX_ADD_ITEM_STR_ID
		HRESULT	AddControl( CKTDGUIControl* pControl );

		void    RemoveAllItems();
		void    RemoveItem( UINT index );
		bool    ContainsItem( const WCHAR* strText, UINT iStart=0 );
		int     FindItem( const WCHAR* strText, UINT iStart=0 );
		void*   GetItemData( const WCHAR* strText );
		void*   GetItemData( int nIndex );
		void    SetDropHeight( UINT nHeight ) { m_nDropHeight = nHeight; UpdateRects(); }
		int     GetScrollBarWidth() const { return m_nSBWidth; }
		void    SetScrollBarWidth( int nWidth ) { m_nSBWidth = nWidth; UpdateRects(); }

		void*   GetSelectedData();
		ComboBoxItem* GetSelectedItem();
		int		GetSelectedItemIndex() { return m_iSelected; }

		UINT    GetNumItems() { return m_Items.GetSize(); }
		ComboBoxItem* GetItem( UINT index ) { return m_Items.GetAt( index ); }

		HRESULT SetSelectedByIndex( UINT index );
		HRESULT SetSelectedByText( const WCHAR* strText, bool bFireMsg = true );
		HRESULT SetSelectedByData( void* pData );  

		void	SetUnSelected();

		void SetString( CKTDGUIControl::UIStringData* pString ){ m_pStringData = pString; }
		void SetString_LUA()
		{
			CKTDGUIControl::UIStringData* pString = SetStringData_();
			SetString( pString );
		}

		void SetTextStringOffsetPos( int x, int y ) { m_TextStringOffsetPosX = x; m_TextStringOffsetPosY = y; }
		void SetReverseDropdown( bool bCheck ) { m_bIsReverseDropDown = bCheck; }


		//자동으로 만들 때
		void SetPos_LUA( int x, int y );
		void SetSize_LUA(int width, int height );
		void SetButtonAddSize( int addWidth, int addHeight );

		//수동으로 만들 때
		void SetTextPos_LUA( int x, int y );
		void SetTextSize_LUA( int width, int height );
		void SetButtonPos_LUA( int x, int y );
		void SetButtonSize_LUA( int width, int height );

        void SetSelectTextShow(bool val) { m_bSelectTextShow = val; }
		
		//콤보 박스 텍스쳐 초기화
		void	SetTextTex_LUA( const char* pFileName, const char* key );
		void	SetButtonTex_LUA( const char* pFileName, const char* key );
		void	SetButtonMouseOverTex_LUA( const char* pFileName, const char* key );
		void	SetButtonMouseDownTex_LUA( const char* pFileName, const char* key );
		void	SetDropdownTex_LUA( const char* pFileName, const char* key );
		void	SetDropdownTextTex_LUA( const char* pFileName, const char* key );

		//스크롤바 텍스쳐 초기화
		void	SetScollBarUpButtonTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarDownButtonTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarTrackTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarThumbTex_LUA( const char* pFileName, const char* key );

		void	SetStaticHeight( int height ) { m_bIsStaticComboBox = true; m_nStaticHeight = height; }
		//void	SetItemScriptName( const char* pFileName );
		//bool	OpenScript( const WCHAR* pFileName );

		void	SetCustomMsgSelectionChanged(int msg) { m_CustomMsgSelectionChanged = msg; }
		void	SetCustomMsgOpenDropdown(int msg) { m_CustomMsgOpenDropdown = msg; } 
		void	SetCustomMsgCloseDropdown(int msg) { m_CustomMsgCloseDropdown = msg; } 
		
		void	SetDrawTextLimit(int nLen) { m_iTextLimit = nLen; }

		bool	GetOpenedDrowdown() { return m_bOpened; }


#ifdef DLL_BUILD
		virtual bool IsSelectByEditGui( POINT pt ) override;

		virtual void MoveControl( float fx, float fy ) override;

		virtual void SetEditGUI( bool bEdit ) override;		// GUI 에디트 모드 설정


		virtual D3DXVECTOR2 GetPos() override;		/// 컨트롤의 위치
		virtual vector<D3DXVECTOR2> GetPosList() override;		// 컨트롤 내에 pictures의 위치 정보

		void DrawEditEdge( D3DXCOLOR edgeColor );		// UITool에서 편집용으로 사용된다.

protected:
		bool m_bEditEdge;
		D3DXCOLOR m_colorEdge;
		CKTDXDeviceTexture * m_pCheckedEdgeTexture;
#endif
		
	protected:

		void FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point );

		int     m_iSelected;
		int     m_iFocused;
		int     m_nDropHeight;
		int		m_nStaticHeight;

		bool	m_bIsStaticComboBox;
		wstring	m_ItemScriptName;

		int     m_nSBWidth;

		bool    m_bOpened;
		bool	m_bPressed;
		bool	m_bMouseOverCheck;

		RECT	m_rcText;
		RECT	m_rcButton;

		RECT	m_rcDropdown;
		RECT	m_rcDropdownText;

		int m_x, m_y;
		int m_width, m_height;

		int m_TextX, m_TextY;						//콤보박스에 스트링말고 텍스쳐 넣을 때.. 텍스쳐 크기..
		int m_TextWidth, m_TextHeight;

		int m_TextStringOffsetPosX;
		int m_TextStringOffsetPosY;

		int m_ButtonX, m_ButtonY;
		int m_ButtonWidth, m_ButtonHeight;

		int m_ButtonOffsetX;
		int m_ButtonOffsetY;

		int m_ButtonAddWidth;
		int m_ButtonAddHeight;

		CKTDGUIControl::UITextureData* m_pTextTex;
		CKTDGUIControl::UITextureData* m_pButtonTex;
		CKTDGUIControl::UITextureData* m_pButtonMouseOverTex;
		CKTDGUIControl::UITextureData* m_pButtonMouseDownTex;
		CKTDGUIControl::UITextureData* m_pDropdownTex;
		CKTDGUIControl::UITextureData* m_pDropdownTextTex;

		CKTDGUIControl::UITextureData* m_pScollBarUpButtonTex;
		CKTDGUIControl::UITextureData* m_pScollBarUpButtonDownTex;
		CKTDGUIControl::UITextureData* m_pScollBarDownButtonTex;
		CKTDGUIControl::UITextureData* m_pScollBarDownButtonDownTex;
		CKTDGUIControl::UITextureData* m_pScollBarTrackTex;
		CKTDGUIControl::UITextureData* m_pScollBarThumbTex;

		CKTDGUIControl::UIPointData m_TempPoint;
		CKTDGUIControl::UIStringData*	m_pStringData;

		CKTDGUIScrollBar*		m_pScrollBar;

		RECT m_rcBoundingBox;	//콤보 박스를 열고 닫는 범위
		RECT m_rcBoundingBox2;	//마우스 핸들 값이 인지하는 컨트롤의 범위

		CKTDGUIControl::VERTEX_UI		m_Vertex[4];	

		CGrowableArray< ComboBoxItem* > m_Items;
	
		int								m_CustomMsgSelectionChanged;
		int								m_CustomMsgOpenDropdown;
		int								m_CustomMsgCloseDropdown;

		bool							m_bMousePointAdjust;			//마우스 포인터 조정하기 위해..
		bool							m_bIsReverseDropDown;

		CKTDXDeviceSound*				m_pSndButtonMouseDown;
		CKTDXDeviceSound*				m_pSndButtonMouseOver;

        bool                            m_bSelectTextShow;
		int								m_iTextLimit;					// 출력할 글자수 제한걸기
};
