#pragma once

class CKTDGUIListBox : public CKTDGUIControl
{
	public:

		enum LIST_BOX_EVENT_MSG
		{
			LBEM_ITEM_DBLCLK = 0,
			LBEM_SELECTION,
			LBEM_SELECTION_END,
			LBEM_SELECTION_BY_RBUTTON,
		};

		enum LIST_BOX_ITEM_TYPE
		{
			LBIT_DEFAULT = 0,
			LBIT_NOT_ENABLE,
			LBIT_GROUP_PLUS,
			LBIT_GROUP_MINUS,
		};

		struct ListBoxItem
		{
			WCHAR strText[256];
			void*  pData;

			RECT  rcActive;
			bool  bSelected;
			CKTDGUIControl* pControl;
			LIST_BOX_ITEM_TYPE itemType;
			int	itemDepth;			//그룹 속에 그룹이 있을 수도 있는 경우를 대비함..


			bool	bSpread;
			float	fSpreadTimeGap;

			int		iSpreadCount;			
			float	fNowSpreadTime;	

			bool	m_bShow;
			float	m_fTimeLeftShow;

            D3DXCOLOR m_color;
            bool    m_bColor;

			//{{ 허상형 : [2009/7/20] //	메가폰 체크 확인 변수
            bool	m_bMegaSelect;
			bool	m_bMegaOver;
			bool	m_bMyMessage;
			wstring	m_wstrData;
			int		m_iMessageID;
			//}} 허상형 : [2009/7/20] //	메가폰 체크 확인 변수

			ListBoxItem()
			{
				pData = NULL;
				pControl = NULL;
				itemType = LBIT_DEFAULT;
				itemDepth = 0;

				bSpread = false;
				iSpreadCount = 0;
				fNowSpreadTime = 0.f;
				m_fTimeLeftShow = -1.f;
				m_bShow = true;

                m_color = D3DXCOLOR(0xffffffff);
                m_bColor = false;

				//{{ 허상형 : [2009/7/20] //	메가폰 체크 확인 변수 초기화
				m_bMegaSelect = false;
				m_bMegaOver = false;
				m_bMyMessage = false;
				m_wstrData = L"";
				m_iMessageID = -1;
				//}} 허상형 : [2009/7/20] //	메가폰 체크 확인 변수 초기화
			}

			~ListBoxItem()
			{
				SAFE_DELETE(pControl);
			}

			void	SetSpread( bool bVal )			{ bSpread = bVal; }
			void	SetSpreadCount( int iCount )	{ iSpreadCount = iCount; }

			//{{ 허상형 : [2009/7/20] //	메가폰 함수들
			void	SetMegaSelect( bool bSelect )	{ m_bMegaSelect = bSelect; }
			void	SetMyMessage( bool bMine )		{ m_bMyMessage = bMine; }

			int		GetMessageID()					{ return m_iMessageID; }
			wstring *GetSenderName()				{ return &m_wstrData; }
			//}} 허상형 : [2009/7/20] //	메가폰 함수들

			void OnFrameMove( double fTime, float fElapsedTime )
			{
				if( m_fTimeLeftShow > 0.f )
				{
					m_fTimeLeftShow -= fElapsedTime;
					if( m_fTimeLeftShow < 0.f )
					{
						m_fTimeLeftShow = 0.f;
						m_bShow = false;
					}
				}

				if( true == bSpread )
				{
					if( iSpreadCount < (int) wcslen( strText ) )
					{
						fNowSpreadTime += fElapsedTime;
						if( fNowSpreadTime > fSpreadTimeGap )
						{
							fNowSpreadTime = 0.f;
							iSpreadCount++;
						}
					}
					else
					{
						bSpread = false;
					}
				}
			}
		};

		enum STYLE { MULTISELECTION = 1 };

	public:
		CKTDGUIListBox();
		virtual ~CKTDGUIListBox();

		CKTDGUIScrollBar* InitScrollBar_LUA();

		virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
		virtual bool    MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual void    UpdateRects();

		virtual void	SetDialog( CKTDGUIDialogType pDialog );

		void SetShowTextTex( bool bCheck ) { m_bShowTextTex = bCheck; }
		void InvalidSelectedItem() { m_nSelected = -1; }

		DWORD GetStyle() const { return m_dwStyle; }
		int GetSize() const { return m_Items.GetSize(); }
		void SetStyle( DWORD dwStyle ) { m_dwStyle = dwStyle; }
		int  GetScrollBarWidth() const { return m_nSBWidth; }
		void SetScrollBarWidth( int nWidth ) { m_nSBWidth = nWidth; UpdateRects(); }

		void SetScrollBarPos( int x, int y ) { m_pScrollBar->SetLocation( x, y ); }
		void SetScrollBarSize( int width, int height ) { m_pScrollBar->SetSize( width, height ); }

		void SetBorder( int nBorder, int nMargin ) { m_nBorder = nBorder; m_nMargin = nMargin; }
		
        HRESULT AddString(const WCHAR* wszText, D3DXCOLOR aColor, void* pData, CKTDGUIControl* pControl = NULL);
		HRESULT AddItem( const WCHAR *wszText, void *pData, int itemDepth = 0, LIST_BOX_ITEM_TYPE itemType = LBIT_DEFAULT, bool bSpread = false, float fSpreadTime = 0.f );

		//{{ 허상형 : [2009/7/20] //	메가폰 형식으로 Item 추가할수 있게 함수 재정의
		HRESULT AddItem( const WCHAR *wszText, void *pData, int iMessageID, wstring wstrName, bool bIsMine = false );
		void	ClearSelect() ;
		//}} 허상형 : [2009/7/20] //	메가폰 형식으로 Item 추가할수 있게 함수 재정의

		void	AddItem_LUA( const char* pName );


		HRESULT InsertItem( int nIndex, const WCHAR *wszText, void *pData );
		void RemoveItem( int nIndex );
		int  RemoveItemByText( WCHAR *wszText );
		void RemoveItemByData( void *pData );
		void RemoveAllItems();

		ListBoxItem* GetLastItem();

		ListBoxItem* GetItem( int nIndex );
		int GetSelectedIndex( int nPreviousSelected = -1 );
		ListBoxItem* GetSelectedItem( int nPreviousSelected = -1 ) { return GetItem( GetSelectedIndex( nPreviousSelected ) ); }

#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
		void SelectItem( IN int nNewIndex, IN bool bSendMessage = true );
#else // UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
		void SelectItem( int nNewIndex );
#endif // UPGRADE_TRADE_SYSTEM_ADD_FUNCTION

		void SetShowAllItems( bool bShow = true );
		void SetShowTimeAllItems( float fTimeLeft );


		void SetString( CKTDGUIControl::UIStringData* pString );
		void SetString_LUA()
		{
			CKTDGUIControl::UIStringData* pString = SetStringData_();
			SetString( pString );
		}

		void SetGroupString( CKTDGUIControl::UIStringData* pString );
		void SetGroupString_LUA()
		{
			CKTDGUIControl::UIStringData* pString = SetStringData_();
			SetGroupString( pString );
		}

		CKTDGUIControl::UIStringData* GetString() { return m_pStringData; }
		CKTDGUIControl::UIStringData* GetGroupString() { return m_pGroupStringData; }

		void SetMultiSelection_LUA() { m_dwStyle = MULTISELECTION; } 

		void	SetTextTex_LUA( const char* pFileName, const char* key );
		void	SetBoundingBoxTex_LUA( const char* pFileName, const char* key );

		//스크롤바 텍스쳐 초기화
		void	SetScollBarUpButtonTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarDownButtonTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarTrackTex_LUA( const char* pFileName, const char* key );
		void	SetScollBarThumbTex_LUA( const char* pFileName, const char* key );

		//{{ 허상형 : [2009/7/21] //	마우스오버 텍스쳐 루아 함수
		void	SetButtonMouseOverTex_LUA( const char* pFileName, const char* key );
		//}} 허상형 : [2009/7/21] //	마우스오버 텍스쳐 루아 함수

		void SetPos_LUA( int x, int y )
		{
			m_x = x;
			m_y = y;

			UpdateRects();
		}

		void SetSize_LUA(int width, int height )
		{
			m_width = width;
			m_height = height;

			UpdateRects();
		}

		void SetStaticHeight( int height ) { m_bIsStaticListBox = true; m_nTextHeight = height; }

		void SetScrollBarTrackRange( int nStart, int nEnd );
		void SetScrollBarTrackPos( int nPosition );
		void SetScrollBarEndPos();

		void SetCustomMsgItemDoubleClick( int msg ){ m_CustomMsgItemDoubleClick = msg; }
		void SetCustomMsgSelection( int msg ){ m_CustomMsgSelection = msg; }
		void SetCustomMsgSelection_End( int msg ){ m_CustomMsgSelection_End = msg; }
		void SetCustomMsgSelectionByRButton( int msg ){ m_CustomMsgSelectionByRButton = msg; }

		//void SetItemScriptName_LUA( const char* fileName );
		//bool OpenItemScript( const WCHAR* pFileName );
		HRESULT AddControl(  CKTDGUIItem* pItem );

		virtual D3DXVECTOR2 GetPos() { return D3DXVECTOR2( (float)m_x, (float)m_y ); }
		const int GetWidth() const { return m_width; }
		const int GetHeight() const { return m_height; }
		const CKTDGUIControl::UITextureData* GetBoundingBoxTexData() const				{ return m_pBoudingBoxTex; }
		const CKTDGUIControl::UITextureData* GetTextTexData() const						{ return m_pTextTex; }
		const CKTDGUIControl::UITextureData* GetScrollBarUpButtonTexData() const		{ return m_pScollBarUpButtonTex; }
		const CKTDGUIControl::UITextureData* GetScrollBarUpButtonDownTexData() const	{ return m_pScollBarUpButtonDownTex; }
		const CKTDGUIControl::UITextureData* GetScrollBarDownButtonTexData() const		{ return m_pScollBarDownButtonTex; }
		const CKTDGUIControl::UITextureData* GetScrollBarDownButtonDownTexData() const	{ return m_pScollBarDownButtonDownTex; }
		const CKTDGUIControl::UITextureData* GetScrollBarThumbTexData() const			{ return m_pScollBarThumbTex; }
		const CKTDGUIControl::UITextureData* GetScrollBarTrackTexData() const			{ return m_pScollBarTrackTex; }
		
		const CKTDGUIControl::UIStringData* GetStringData() const { return m_pStringData; }
		const CKTDGUIControl::UIStringData* GetGroupStringData() const { return m_pGroupStringData; }

		// Minimize / Maximize
		virtual void SetMaximize( bool val );
		virtual bool ContainsPoint( POINT pt );


		void SetIgnoreListClick( bool bCheck ) { m_bIgnoreListClick = bCheck; }
		CKTDGUIScrollBar* GetScrollBar() const { return m_pScrollBar; }
	
		void SetUseItemDisappear(bool val) { m_bUseItemDisappear = val; }
		void SetUpdateScrollBarOnUpdate(bool val) { m_bUpdateScrollBarOnUpdate = val; }
		void SetScrollToEndOnUpdateRects(bool val) { m_bScrollToEndOnUpdateRects = val; }

#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
		void SetSelectItemAtMouseMove( bool bIsSelectItemAtMouseMove_ ) { m_bIsSelectItemAtMouseMove = bIsSelectItemAtMouseMove_; }
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION

	protected:

		void FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point );

#ifdef DLL_BUILD
		virtual bool IsSelectByEditGui( POINT pt ) override
		{
			return ContainsPoint(pt);
		}

		virtual void MoveControl( float fx, float fy ) override;

		virtual void SetEditGUI( bool bEdit ) override;		// GUI 에디트 모드 설정


		virtual vector<D3DXVECTOR2> GetPosList() override;		// 컨트롤 내에 pictures의 위치 정보
		void DrawEditEdge();		// UITool에서 편집용으로 사용된다.

protected:
	bool m_bEditEdge;
	CKTDXDeviceTexture * m_pCheckedEdgeTexture;
#endif

protected:
		RECT m_rcText;      // Text rendering bound
		RECT m_rcSelection; // Selection box bound

		CKTDGUIControl::UITextureData* m_pBoudingBoxTex;
		CKTDGUIControl::UITextureData* m_pTextTex;

		CKTDGUIControl::UITextureData* m_pButtonMouseOverTex;
		CKTDGUIControl::UITextureData* m_pScollBarUpButtonTex;
		CKTDGUIControl::UITextureData* m_pScollBarUpButtonDownTex;
		CKTDGUIControl::UITextureData* m_pScollBarDownButtonTex;
		CKTDGUIControl::UITextureData* m_pScollBarDownButtonDownTex;
		CKTDGUIControl::UITextureData* m_pScollBarTrackTex;
		CKTDGUIControl::UITextureData* m_pScollBarThumbTex;


		CKTDGUIControl::UIStringData* m_pStringData;
		CKTDGUIControl::UIStringData* m_pGroupStringData;

		CKTDGUIScrollBar*	m_pScrollBar;

		int m_nSBWidth;
		int m_nBorder;
		int m_nMargin;
		int m_nTextHeight;  // Height of a single line of text
		DWORD m_dwStyle;    // List box style
		int m_nSelected;    // Index of the selected item for single selection list box
		int m_nSelStart;    // Index of the item where selection starts (for handling multi-selection)
		bool m_bDrag;       // Whether the user is dragging the mouse to select

		bool m_bShowTextTex;

		CGrowableArray< ListBoxItem* > m_Items;

		CKTDGUIControl::UIPointData m_TempPoint;
		CKTDGUIControl::VERTEX_UI		m_Vertex[4];

		RECT m_rcBoundingBox;

		int m_x, m_y;
		int m_width, m_height;

		int						m_CustomMsgItemDoubleClick;
		int						m_CustomMsgSelection;
		int						m_CustomMsgSelection_End;
		int						m_CustomMsgSelectionByRButton;

		bool					m_bIsStaticListBox;
		wstring					m_ItemScriptName;

		CKTDXDeviceSound*		m_pSndMouseDown;

		CKTDGFontManager::CUKFont* m_pUKFont;
		CKTDGFontManager::CUKFont* m_pGroupUKFont;

		bool					m_bIgnoreListClick;

		int						m_BlankWidthByDepth;		//깊이에 따른 스트링 x좌표 이동 길이
		int						m_BlankWidthToGroupMark;	//그룹 표시 마크에의한 x좌표 이동 길이

		CKTDGUIStatic*			m_pGroupMark;

		D3DXVECTOR2				m_GroupMarkOffsetPos;
		D3DXVECTOR2				m_GroupMarkAddSize;

		bool					m_bUseItemDisappear;		

		bool					m_bUpdateScrollBarOnUpdate;
		bool					m_bScrollToEndOnUpdateRects;
		
		
#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
		bool					m_bIsSelectItemAtMouseMove;			/// 마우스 오버를 통해 아이템 선택 가능하게 할 지 여부
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
};
