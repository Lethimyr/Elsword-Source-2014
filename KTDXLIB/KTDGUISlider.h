#pragma once

class CKTDGUISlider : public CKTDGUIControl
{
	public:

		enum SlIDER_EVENT_MSG
		{
			SLIDER_VALUE_CHANGED = 0,
		};

	public:
		CKTDGUISlider();
		virtual ~CKTDGUISlider();
		
		virtual HRESULT	OnFrameMove(double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();
	
		virtual bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );

		//virtual BOOL ContainsPoint( POINT pt ); 
		//virtual bool CanHaveFocus() { return (m_bVisible && m_bEnabled); }
		virtual void UpdateRects(); 

		void SetValue( int nValue ) { SetValueInternal( nValue, false ); }
		int  GetValue() const { return m_nValue; };

		void GetRange( int &nMin, int &nMax ) const { nMin = m_nMin; nMax = m_nMax; }
		void SetRange( int nMin, int nMax );

		void	SetBGTex_LUA( const char* pFileName, const char* key  );
		void	SetButtonTex_LUA( const char* pFileName, const char* key );

		void	SetPos(int x, int y) { m_x = x; m_y = y; UpdateRects(); }
		void	SetSize(int width, int height) { m_width = width; m_height = height; UpdateRects(); }
		void	SetButtonSize( int width, int height ) { m_iButtonWidth = width, m_iButtonHeight = height; UpdateRects(); }
		void	SetMin(int _min) { m_nMin = _min; }
		void	SetMax(int _max) { m_nMax = _max; }

		void SetCustomMsgValueChanged( int msg ){ m_CustomMsgValueChanged = msg; }

	protected:
		void SetValueInternal( int nValue, bool bFromInput );
		int  ValueFromPos( int x ); 
		void FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point );

#ifdef DLL_BUILD
		virtual bool IsSelectByEditGui( POINT pt ) override;
		virtual void MoveControl( float fx, float fy ) override;
		virtual void SetEditGUI( bool bEdit ) override;		// GUI 에디트 모드 설정
		virtual D3DXVECTOR2 GetPos() override;		/// 컨트롤의 위치
		virtual vector<D3DXVECTOR2> GetPosList() override;		// 컨트롤 내에 pictures의 위치 정보
		void DrawEditEdge();		// UITool에서 편집용으로 사용된다.

protected:
	bool m_bEditEdge;
	CKTDXDeviceTexture * m_pCheckedEdgeTexture;
#endif

protected:
		int m_x, m_y;
		int m_width, m_height;					// slider control 전체 영역 크기
		int m_iButtonWidth, m_iButtonHeight;	// slider button 크기

		CKTDGUIControl::UIPointData m_TempPoint;

		CKTDGUIControl::UITextureData* m_pBGTex;
		CKTDGUIControl::UITextureData* m_pButtonTex;

		int	m_CustomMsgValueChanged;

		int m_nValue;

		int m_nMin;
		int m_nMax;

		int m_nDragX;      // Mouse position at start of drag
		int m_nDragOffset; // Drag offset from the center of the button
		int m_nButtonX;

		bool m_bPressed;
		RECT m_rcButton;
		RECT m_rcBoundingBox;

		CKTDGUIControl::VERTEX_UI		m_Vertex[4];
};

