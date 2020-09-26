#pragma once

/*
class CKTDGUIScrollBar : public CKTDGUIControl
{
	public:

		CKTDGUIScrollBar(void);

		virtual ~CKTDGUIScrollBar(void);

		virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
		virtual bool    MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual	HRESULT	OnFrameRender();
		virtual void    UpdateRects();

		//virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );

		void SetTrackRange( int nStart, int nEnd );
		int GetTrackPos() { return m_nPosition; }
		void SetTrackPos( int nPosition ) { m_nPosition = nPosition; Cap(); UpdateThumbRect(); }
		int GetPageSize() { return m_nPageSize; }
		void SetPageSize( int nPageSize ) { m_nPageSize = nPageSize; Cap(); UpdateThumbRect(); }

		void Scroll( int nDelta );    // Scroll by nDelta items (plus or minus)
		void ShowItem( int nIndex );  // Ensure that item nIndex is displayed, scroll if necessary

		void SetLocation( int x, int y ) { m_x = x; m_y = y; UpdateRects(); }
		void SetSize( int width, int height ) { m_width = width; m_height = height; UpdateRects(); }

		void	SetUpButtonTex_();
		void	SetDownButtonTex_();
		void	SetThumbTex_();
		void	SetTrackTex_();

	protected:

		// ARROWSTATE indicates the state of the arrow buttons.
		// CLEAR            No arrow is down.
		// CLICKED_UP       Up arrow is clicked.
		// CLICKED_DOWN     Down arrow is clicked.
		// HELD_UP          Up arrow is held down for sustained period.
		// HELD_DOWN        Down arrow is held down for sustained period.
		enum ARROWSTATE { CLEAR, CLICKED_UP, CLICKED_DOWN, HELD_UP, HELD_DOWN };

		void UpdateThumbRect();
		void Cap();  // Clips position at boundaries. Ensures it stays within legal range.

		bool m_bShowThumb;
		bool m_bDrag;
		
//		RECT m_rcUpButton;
//		RECT m_rcDownButton;
//		RECT m_rcTrack;
//		RECT m_rcThumb;

		CKTDGUIControl::UIPointData* m_pUpButtonPoint;
		CKTDGUIControl::UIPointData* m_pDownButtonPoint;
		CKTDGUIControl::UIPointData* m_pTrackPoint;
		CKTDGUIControl::UIPointData* m_pThumbPoint;

		CKTDGUIControl::UITextureData* m_pUpButtonTex;
		CKTDGUIControl::UITextureData* m_pDownButtonTex;
		CKTDGUIControl::UITextureData* m_pTrackTex;
		CKTDGUIControl::UITextureData* m_pThumbTex;

		int m_nPosition;  // Position of the first displayed item
		int m_nPageSize;  // How many items are displayable in one page
		int m_nStart;     // First item
		int m_nEnd;       // The index after the last item
		POINT m_LastMouse;// Last mouse position
		ARROWSTATE m_Arrow; // State of the arrows
		double m_dArrowTS;  // Timestamp of last arrow event.

		//RECT m_rcBoundingBox;

		CKTDGUIControl::UIPointData* m_pBoundingBoxPoint;

		int m_x, m_y;
		int m_width, m_height;

		CKTDGUIControl::VERTEX_UI			m_Vertex[4];

};


*/

class CKTDGUIScrollBar : public CKTDGUIControl
{
	public:
		CKTDGUIScrollBar();
		virtual ~CKTDGUIScrollBar();

		virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
		virtual bool    MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		//virtual void    Render( IDirect3DDevice9* pd3dDevice, float fElapsedTime );
		virtual void    UpdateRects();

		virtual	HRESULT	OnFrameRender();

		virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );

		void SetTrackRange( int nStart, int nEnd );
		int GetTrackPos() { return m_nPosition; }
		void SetTrackPos( int nPosition );
		void SetTrankEndPos() { m_nPosition = m_nEnd; Cap(); UpdateThumbRect(); }
		int GetTrackEndPos() { return m_nEnd; }
		int GetPageSize() { return m_nPageSize; }
		void SetPageSize( int nPageSize ) { m_nPageSize = nPageSize; Cap(); UpdateThumbRect(); }

		void SetLocation( int x, int y ) { m_x = x; m_y = y; UpdateRects(); }
		void SetSize( int width, int height ) { m_width = width; m_height = height; UpdateRects(); }

		int GetWidth() { return m_width; }

		void Scroll( int nDelta );    // Scroll by nDelta items (plus or minus)
		void ShowItem( int nIndex );  // Ensure that item nIndex is displayed, scroll if necessary

		void	SetUpButtonTex(	CKTDGUIControl::UITextureData* pUpButtonTex );
		void	SetUpButtonDownTex( CKTDGUIControl::UITextureData* pUpButtonDownTex );
		void	SetDownButtonTex( CKTDGUIControl::UITextureData* pDownButtonTex );
		void	SetDownButtonDownTex( CKTDGUIControl::UITextureData* pDownButtonDownTex );
		void	SetThumbTex( CKTDGUIControl::UITextureData* pTrackTex );
		void	SetTrackTex( CKTDGUIControl::UITextureData* pThumbTex );

		bool GetEnableThumb() const { return m_bEnableThumb; }
		void SetEnableThumb(bool val) { m_bEnableThumb = val; }
		bool GetEnableTrack() const { return m_bEnableTrack; }
		void SetEnableTrack(bool val) { m_bEnableTrack = val; }

		RECT GetBoundingBox() const { return m_rcBoundingBox; }

	protected:
		// ARROWSTATE indicates the state of the arrow buttons.
		// CLEAR            No arrow is down.
		// CLICKED_UP       Up arrow is clicked.
		// CLICKED_DOWN     Down arrow is clicked.
		// HELD_UP          Up arrow is held down for sustained period.
		// HELD_DOWN        Down arrow is held down for sustained period.
		enum ARROWSTATE { CLEAR, CLICKED_UP, CLICKED_DOWN, HELD_UP, HELD_DOWN };

		void UpdateThumbRect();
		void Cap();  // Clips position at boundaries. Ensures it stays within legal range.

		void FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point );

#ifdef DLL_BUILD
public:
		virtual void MoveControl( float fx, float fy ) override;

		virtual void SetEditGUI( bool bEdit ) override;

		virtual D3DXVECTOR2 GetPos() override;

		void ShowEdge( bool bShow );
protected:
		
		void DrawEditEdge();

protected:
		bool m_bEditEdge;
		CKTDXDeviceTexture * m_pCheckedEdgeTexture;

#endif
protected:
		bool m_bShowThumb;
		bool m_bDrag;

		bool m_bIsUpButtonClicked;
		bool m_bIsDownButtonClicked;

		RECT m_rcUpButton;
		RECT m_rcDownButton;
		RECT m_rcTrack;
		RECT m_rcThumb;

		CKTDGUIControl::UIPointData m_TempPoint;

		CKTDGUIControl::UITextureData* m_pUpButtonTex;
		CKTDGUIControl::UITextureData* m_pDownButtonTex;
		CKTDGUIControl::UITextureData* m_pUpButtonDownTex;
		CKTDGUIControl::UITextureData* m_pDownButtonDownTex;
		CKTDGUIControl::UITextureData* m_pTrackTex;
		CKTDGUIControl::UITextureData* m_pThumbTex;

		CKTDGUIControl::VERTEX_UI			m_Vertex[4];

		int m_nPosition;  // Position of the first displayed item
		int m_nPageSize;  // How many items are displayable in one page
		int m_nStart;     // First item
		int m_nEnd;       // The index after the last item

		POINT m_LastMouse;// Last mouse position
		ARROWSTATE m_Arrow; // State of the arrows
		double m_dArrowTS;  // Timestamp of last arrow event.

		// Size, scale, and positioning members
		int m_x, m_y;
		int m_width, m_height;

		RECT m_rcBoundingBox;
		
		
		CKTDXDeviceSound*				m_pSndMouseDown;

		bool			m_bEnableThumb; 
		bool			m_bEnableTrack;

};
