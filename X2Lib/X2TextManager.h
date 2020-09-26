#pragma once

class CX2TextManager
{
	public:

		class CX2Text
		{
			friend CX2TextManager;

			public:
				struct TextMoveItem
				{
					wstring		m_Msg;
					D3DXVECTOR2 m_Pos;
					D3DXCOLOR	m_Color;
					D3DXCOLOR	m_OutLineColor;
					float		m_fChangeTime;
					bool		m_bAutoDelete;

					TextMoveItem()
					{
						m_Msg			= L"";
						INIT_VECTOR2(m_Pos, 0, 0);
						m_Color			= D3DXCOLOR(0xffffffff);
						m_OutLineColor	= D3DXCOLOR(0xffffffff); 
						m_fChangeTime	= 0.0f;
						m_bAutoDelete	= false;
					}

					TextMoveItem& TextMoveItem::operator=(const TextMoveItem& rhs) 
					{ 
						if ( this == &rhs ) 
						{ 
							return *this; 
						}

						m_Msg			= rhs.m_Msg;
						m_Pos			= rhs.m_Pos;
						m_Color			= rhs.m_Color;
						m_OutLineColor	= rhs.m_OutLineColor;
						m_fChangeTime	= rhs.m_fChangeTime;
						m_bAutoDelete	= rhs.m_bAutoDelete;

						return *this; 
					} 
				};

			protected:
				CX2Text( int fontIndex, D3DXVECTOR2 pos, const WCHAR* pTextMsg, 
						D3DXCOLOR color, D3DXCOLOR outlineColor, 
						DWORD dtType = DT_LEFT, float fLineSpace = 1.0f  );

				virtual ~CX2Text();

			public:

				bool	GetShow() { return m_bShow; }
				bool	GetEnable() { return m_bEnable; }
				void	SetShow( bool bShow ) { m_bShow = bShow; }
				void	SetEnable( bool bEnable ) { m_bEnable = bEnable; }

				void	Move( D3DXVECTOR2 pos, const WCHAR* pMsg, D3DXCOLOR color, D3DXCOLOR outlineColor, 
							float fChangeTime, bool bDirect = false, bool bAutoDelete = false );

				void	Move_LUA();

				void	SetDTType( DWORD dtType ) { m_DTType = dtType; }
				void	SetLineSpace( float fLineSpace ) { m_fLineSpace = fLineSpace; }

				int		GetTextWidth( const WCHAR* pMsg );
				int		GetNowTextWidth();

				bool	GetIsMoving() { return m_fAnimateTime != 0.0f; }
				
				D3DXVECTOR2 GetPos() { return m_Pos; }
				void SetPos( D3DXVECTOR2 vPos ) { m_Pos = vPos; }

				const TextMoveItem* GetTextMoveItemFront()
				{
					if( m_queMoveItems.size() > 0 )
					{
						return &m_queMoveItems.front();
					}
					else
					{
						return NULL;
					}
				}




				int GetFontIndex() const { return m_FontIndex; }
				wstring GetTextMsg() const { return m_TextMsg; }
				D3DXVECTOR2 GetGoalPos() const { return m_GoalPos; }
				D3DXCOLOR GetGoalColor() const { return m_GoalColor; }
				D3DXCOLOR GetGoalOutlineColor() const { return m_GoalOutlineColor; }
				float GetAnimateTime() const { return m_fAnimateTime; }
				float GetChangeTime() const { return m_fChangeTime; }

				CKTDGFontManager::CUKFont* GetFont() const { return m_pFont; }
				int GetLineCount() const { return m_iLineCount; }

			protected:

				bool	GetIsAutoDeleteEnd() { return m_bCheckAutoDeleteEnd; }

				HRESULT OnFrameMove( double fTime, float fElapsedTime );
				HRESULT OnFrameRender();

			protected:

				bool				m_bShow;
				bool				m_bEnable;

				int					m_FontIndex;
			
				wstring				m_TextMsg;			
				
				DWORD				m_DTType;
				float				m_fLineSpace;

				D3DXVECTOR2			m_Pos;
				D3DXCOLOR			m_Color;
				D3DXCOLOR			m_OutlineColor;
				float				m_fAnimateTime;
				
				D3DXVECTOR2			m_GoalPos;
				D3DXCOLOR			m_GoalColor;
				D3DXCOLOR			m_GoalOutlineColor;
				
				float				m_fChangeTime;
				
				bool				m_bAutoDelete; //Move가 끝난 다음에 불릴 Delete
				bool				m_bCheckAutoDeleteEnd;

				int					m_iLineCount;	// 몇줄인지
				
				
				CKTDGFontManager::CUKFont*	m_pFont;				
				std::queue< TextMoveItem >	m_queMoveItems;
		};

	public:

		CX2TextManager(void);
		virtual ~CX2TextManager(void);

		CX2Text*	AddText( int fontIndex, D3DXVECTOR2 pos, const WCHAR* pTextMsg, 
							D3DXCOLOR color, D3DXCOLOR outlineColor,
							DWORD dtType = DT_LEFT, float fLineSpace = 1.0f );
/*
		CX2Text*	CreateText( int fontIndex, D3DXVECTOR2 pos, const WCHAR* pTextMsg, 
			D3DXCOLOR color, D3DXCOLOR outlineColor,
			DWORD dtType = DT_LEFT, float fLineSpace = 1.0f );

		CX2Text*	AddText( CX2Text* pText );
*/
		CX2Text*	AddText_LUA( int fontIndex, D3DXVECTOR2 pos, const char* pTextMsg, 
								D3DXCOLOR color, D3DXCOLOR outlineColor );

		bool		CheckText( CX2Text* pTextForCheck );
		bool		DeleteText( CX2Text* pTextForDelete );

		HRESULT		OnFrameMove( double fTime, float fElapsedTime );
		HRESULT		OnFrameRender();

	protected:

		vector< CX2TextManager::CX2Text* > m_vecTextList;
};
