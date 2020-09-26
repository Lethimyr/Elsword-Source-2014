#pragma once

class CKTDGUIListControl : public CKTDGUIControl
{
	public:
		CKTDGUIListControl();
		virtual ~CKTDGUIListControl(void);

		virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
		virtual	HRESULT	OnFrameRender();
		virtual	bool	CanHaveFocus(){	return false; }

		void		SetItemControlScript_( const char* pFileName );
		void		SetItemControlScript( const WCHAR* pFileName ){ m_ItemControlScript = pFileName; }

		void		SetItemStartPos_( float x, float y )	{ m_ItemStartPos = D3DXVECTOR2(x,y); }
		void		SetItemStartPos( D3DXVECTOR2 pos )		{ m_ItemStartPos = pos; }
		D3DXVECTOR2	GetItemStartPos()						{ return m_ItemStartPos; }

		void		SetItemWidth( int itemWidth )		{ m_ItemWidth = itemWidth; }
		void		SetItemHeight( int itemHeight )		{ m_ItemHeight = itemHeight; }

		void		SetColumn( int column )	{ m_Column = column; }
		void		SetRow( int row )		{ m_Row = row; }

		void		SetItemBackgroundTex_( const char* pFileName, const char* key );
		void		SetBackgroundTex_( const char* pFileName, const char* key );

		void		SetBackgroundPoint_();

		CKTDGUIControl* GetItem( int index );

		/*
		void AddItem( CKTDGUIControl* pControl );
		void InsertItem( int index, CKTDGUIControl* pControl );
		void RemoveItem( CKTDGUIControl* pControl );
		void RemoveItem( int index );
		void RemoveAllItems();
		CKTDGUIControl* GetItem( int index );
		*/

	protected:
		wstring					m_ItemControlScript;

		D3DXVECTOR2				m_ItemStartPos;
		int						m_ItemWidth;
		int						m_ItemHeight;
		int						m_NowStartItem;

		int						m_Column;
		int						m_Row;

		CKTDGUIControl::UITextureData*	m_pItemBGTex;
		CKTDGUIControl::UITextureData*	m_pBGTex;
		CKTDGUIControl::UIPointData*	m_pBGPoint;
		
		CKTDGUIControl::VERTEX_UI		m_Vertex[4];
		list<CKTDGUIControl*>			m_ItemList;
};
