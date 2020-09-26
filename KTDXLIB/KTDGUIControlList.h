#pragma once
#ifdef NEW_SKILL_TREE_UI

class CKTDGUIContolList : public CKTDGUIControl
{
	struct ControlInfo
	{
		CKTDGUIControl*	m_pControl;
		bool			m_bUseAbsolutePos;
		ControlInfo()
		{
			m_pControl			= NULL;
			m_bUseAbsolutePos	= false;
		}
	};
	typedef int Row;
	typedef int Col;
	
	typedef vector<ControlInfo> ControlVector;
	typedef std::map<std::pair<Row, Col>, ControlVector > ControlVectorMap;
	

public:
	
// 	enum CONTROL_LIST_EVENT_MSG
// 	{
// 		CLEM_END = 0,
// 	};


public:
	CKTDGUIContolList();
	virtual	~CKTDGUIContolList(void);

	void SetShow(bool bShow, Row row = -1, Col col = -1 );
	void SetEnable(bool bEnable, Row row = -1, Col col = -1 );

	void SetViewSize(int X, int Y, bool bUpdateControl = true );
	int GetViewSizeX(){ return m_iViewSizeX; }
	int GetViewSizeY(){ return m_iViewSizeY; }

	void SetIndex(int X, int Y, bool bUpdateControl = true, bool bSendMessage = true );
	int GetIndexX(){ return m_iTopIndexX; }
	int GetIndexY(){ return m_iTopIndexY; }
	
	void SetScrollOffset(int X, int Y);
	int GetScrollOffsetX(){ return m_iScrollOffsetX; }
	int GetScrollOffsetY(){ return m_iScrollOffsetY; }

	int GetMaxIndexX(){ return m_iMaxIndexX; }
	int GetMaxIndexY(){ return m_iMaxIndexY; }

	void UpdateRenderControlList();
	void UpdateScrollBar();
	
	void SetCustomMsgScroll( int msg ) { m_CustomMsgScroll = msg; }

	virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual	HRESULT	OnFrameRenderEnd();

	virtual bool    HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
	virtual bool    MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual	bool	ContainsPoint( POINT pt );
	virtual void    UpdateRects();

	void			SetPos(D3DXVECTOR2 pos){ m_Pos = pos; }
	void			SetPos_LUA( float x, float y ){ m_Pos.x = x; m_Pos.y = y;}

	// UseAbsolutePosition = true면 받아들이는 control의 좌표를 [0,0]의 가장 왼쪽 위를 기준으로 계산하게 된다.
	// 즉 통채로 완성된 어떤 Control들을 집어 넣을 때 true로 설정해주면 됨. 아직 미구현.
	bool			InsertItem( CKTDGUIControl* pControl, int row, int col, bool bUseAbsolutePosition = false);		
	bool			GetControlVector( int row, int col, ControlVector& vecControl );
	
	bool			AddControlVector( int row, int col, ControlVector& vecControl );

	//스크롤바 관련
	void	SetXScollBarUpButtonTex_LUA( const char* pFileName, const char* key );
	void	SetXScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key );
	void	SetXScollBarDownButtonTex_LUA( const char* pFileName, const char* key );
	void	SetXScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key );
	void	SetXScollBarTrackTex_LUA( const char* pFileName, const char* key );
	void	SetXScollBarThumbTex_LUA( const char* pFileName, const char* key );

	void	SetYScollBarUpButtonTex_LUA( const char* pFileName, const char* key );
	void	SetYScollBarUpButtonDownTex_LUA( const char* pFileName, const char* key );
	void	SetYScollBarDownButtonTex_LUA( const char* pFileName, const char* key );
	void	SetYScollBarDownButtonDownTex_LUA( const char* pFileName, const char* key );
	void	SetYScollBarTrackTex_LUA( const char* pFileName, const char* key );
	void	SetYScollBarThumbTex_LUA( const char* pFileName, const char* key );

	void SetXScrollBarPos( int x, int y ) { m_pScrollBarX->SetLocation( x, y ); }
	void SetXScrollBarSize( int width, int height ) { m_pScrollBarX->SetSize( width, height ); }
	void SetYScrollBarPos( int x, int y ) { m_pScrollBarY->SetLocation( x, y ); }
	void SetYScrollBarSize( int width, int height ) { m_pScrollBarY->SetSize( width, height ); }

	void SetShowEnableXScroolBar(bool bShow, bool bEnable){ m_pScrollBarX->SetShowEnable(bShow, bEnable); }
	void SetShowEnableYScroolBar(bool bShow, bool bEnable){ m_pScrollBarY->SetShowEnable(bShow, bEnable); }

	CKTDGUIScrollBar* InitXScrollBar_LUA();
	CKTDGUIScrollBar* InitYScrollBar_LUA();
	CKTDGUIScrollBar* GetXScroolBar(){ return m_pScrollBarX; }
	CKTDGUIScrollBar* GetYScroolBar(){ return m_pScrollBarY; }


protected:
	void FromRectToPoint( RECT& rect, CKTDGUIControl::UIPointData& point );

#ifdef DLL_BUILD
	virtual bool IsSelectByEditGui( POINT pt ) override { return ContainsPoint(pt); }

	virtual void SetColor( D3DXCOLOR color );
	virtual void MoveControl( float fx, float fy ) override;
	virtual void MoveSubControl( float fx, float fy, wstring subControlName ) override;

	virtual void SetEditGUI( bool bEdit ) override;		// GUI 에디트 모드 설정

	virtual void ShowSubView( wstring name, bool bView ) override;	

	virtual vector<D3DXVECTOR2> GetPosList() override;		// 컨트롤 내에 pictures의 위치 정보

	virtual D3DXVECTOR2 GetPos() override;		/// 컨트롤의 위치
	virtual D3DXVECTOR2 GetPos(wstring name) override;


	void DrawEditEdge( CKTDGUIControl::UIPointData*	m_pEditEdgePoint );		// UITool에서 편집용으로 사용된다.

protected:
		bool m_bEditEdge;
		CKTDXDeviceTexture * m_pCheckedEdgeTexture;
#endif

private:

	ControlVectorMap	m_mapControls;
	vector<CKTDGUIControl*>		m_vecNowControls;

	CKTDGUIControl::UITextureData* m_pXScollBarUpButtonTex;
	CKTDGUIControl::UITextureData* m_pXScollBarUpButtonDownTex;
	CKTDGUIControl::UITextureData* m_pXScollBarDownButtonTex;
	CKTDGUIControl::UITextureData* m_pXScollBarDownButtonDownTex;
	CKTDGUIControl::UITextureData* m_pXScollBarTrackTex;
	CKTDGUIControl::UITextureData* m_pXScollBarThumbTex;

	CKTDGUIControl::UITextureData* m_pYScollBarUpButtonTex;
	CKTDGUIControl::UITextureData* m_pYScollBarUpButtonDownTex;
	CKTDGUIControl::UITextureData* m_pYScollBarDownButtonTex;
	CKTDGUIControl::UITextureData* m_pYScollBarDownButtonDownTex;
	CKTDGUIControl::UITextureData* m_pYScollBarTrackTex;
	CKTDGUIControl::UITextureData* m_pYScollBarThumbTex;
	
	int m_iTopIndexX, m_iTopIndexY;
	int m_iMaxIndexX, m_iMaxIndexY;
	int m_iViewSizeX, m_iViewSizeY;
	int m_iScrollOffsetX, m_iScrollOffsetY;

	int	m_CustomMsgScroll;		// Scrool이 될 때 발생하는 MSG

	D3DXVECTOR2	m_Pos;

	RECT m_rcBoundingBox;

	CKTDGUIScrollBar* m_pScrollBarX;
	CKTDGUIScrollBar* m_pScrollBarY;

};

#endif