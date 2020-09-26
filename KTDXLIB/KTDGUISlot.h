#pragma once
#ifdef NEW_SKILL_TREE_UI

//Ex,.
namespace _CONST_SLOT_ 
{
	const int g_iMaxData = 100;
}

class CKTDGUISlot;
class CKTDGUISlotData		// Slot 내부에 담길 Data. 상속받아서 쓰자.
{
public:
	enum SLOT_DATA_TYPE
	{
		SDT_NONE,
		SDT_SKILL_TREE,
		SDT_SKILL_EQUIP,
		SDT_USE_KEY,
		SDT_DISUSE_KEY,
		SDT_END,
	};

	virtual void UpdateUI( CKTDGUISlot* pSlot ) = 0;

	bool m_bDataUpdated;
	SLOT_DATA_TYPE	m_eSlotDataType;
//	CKTDGUISlot* m_pSlot;

	CKTDGUISlotData()
	{
		m_bDataUpdated = false;
		m_eSlotDataType = SDT_NONE;
//		m_pSlot = NULL;
	}
	virtual	~CKTDGUISlotData(){};
};

class CKTDGUISlot : public CKTDGUIControl
{
	friend class CKTDGUIDialogManager;
	friend class CKTDGUIDialog;
public:

	
	enum SLOT_CONTROL_STATE
	{
		SCS_NORMAL,
		SCS_DOWN,
		SCS_MOUSEOVER,
		SCS_ONDRAG,
		SCS_DISABLE,

		SCS_END,
	};

// 	enum SCS_EVENT_MSG
// 	{
// 		SCS_BUTTON_MOUSEOVER = 0,
// 		SCS_BUTTON_MOUSEDOWN,
// 		SCS_BUTTON_MOUSEUP,
// 	};



public:
	CKTDGUISlot();
	virtual	~CKTDGUISlot(void);
	
	virtual	HRESULT	OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual	HRESULT	OnFrameRenderEnd();

	HRESULT	CKTDGUISlot::RenderDragSlot();

	virtual bool    HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam );
	//virtual bool    MsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam );

	void LButtonMouseUp();

	virtual void	SetEnable( bool bEnable );
	virtual void	SetShow( bool bShow );
	void SetDragable( bool bDragable ){ m_bDragable = bDragable; }
	bool GetDragable(){ return m_bDragable; }
	D3DXVECTOR2 GetSize();
	virtual D3DXVECTOR2 GetPos() override;

	virtual D3DXVECTOR2 GetGuideDescPos();

	virtual	bool	ContainsPoint( POINT pt );
		
	virtual void	OnFocusOut();
	virtual void	ChangeState( SLOT_CONTROL_STATE state, bool bForce = false );
	SLOT_CONTROL_STATE GetState(){ return m_eControlState; }


// 	void SetEtcPicture( CKTDGUIControl::CPictureData* pPicture ){ SAFE_DELETE(m_pEtcPicture); m_pEtcPicture = pPicture; }
// 	void SetEtcTex( const WCHAR* pFileName, const WCHAR* key );
// 	void SetEtcTex( const WCHAR* pFileName );

/*	void SetColorFactor(D3DXCOLOR cColor);*/

	static void ReturnDraggingItem();
	
	void SetDraggingSlotColor(D3DXCOLOR cColor){ m_cDraggingSlotColor = cColor; }
	void SetDraggedSlotColor(D3DXCOLOR cColor){ m_cDraggedSlotColor = cColor; }
	D3DXCOLOR GetDraggingSlotColor(){ return m_cDraggingSlotColor; }
	D3DXCOLOR GetDraggedSlotColor(){ return m_cDraggedSlotColor; }

	CKTDGUISlot* GetDroppedSlot(){ return m_pDroppedSlot; }
	void AddPicture( int key, CKTDGUIControl::CPictureData* pPicture ){ m_mapAdditionalPictureDataList.insert(std::make_pair(key, pPicture)); }	
	CKTDGUIControl::CPictureData* GetPicture( int index );
	void RemoveAllPicture();
	void DrawEdge( bool bDrawOut, int edgeWidth, D3DXCOLOR edgeColor, CKTDGUIControl::UIPointData* pPoint );

	void SetCustomMsgMouseOut( int msg ) { m_CustomMsgMouseOut = msg; }
	void SetCustomMsgMouseOver( int msg ){ m_CustomMsgMouseOver = msg; }
	void SetCustomMsgMouseDown( int msg ){ m_CustomMsgMouseDown = msg; }
	void SetCustomMsgMouseUp( int msg ){ m_CustomMsgMouseUp = msg; }
	void SetCustomMsgMouseRightUp( int msg ){ m_CustomMsgMouseRightUp = msg; }
	void SetCustomMsgDropped( int msg ){ m_CustomMsgDropped = msg; }

	void SetItemTex( const WCHAR* pFileName );
	void SetItemTex( const WCHAR* pFileName, const WCHAR* key );
	void SetItemTex_LUA( const char* pFileName, const char* key );
	void SetItemTex_LUA2( const char* pFileName );
	void SetMouseOverTex( const WCHAR* pFileName, const WCHAR* key );
	void SetMouseOverTex_LUA( const char* pFileName, const char* key  );
	
	void SetString( CKTDGUIControl::UIStringData* pString ){ SAFE_DELETE(m_pStringData); m_pStringData = pString; }
	CKTDGUIControl::UIStringData* GetString(){ return m_pStringData; }
	void SetString_LUA();

	// 이건 필수고..
	void SetPoint( CKTDGUIControl::UIPointData* pUIPointData );
	void SetPoint_LUA();
	void SetMouseOverPoint( CKTDGUIControl::UIPointData* pUIPointData );
	void SetMouseOverPoint_LUA();
	
	void SetAllPointDatabyNormalPoint();

	// 데이터 관련..
	CKTDGUISlotData* GetSlotData(){ return m_pSlotData; }
	void SetSlotData( CKTDGUISlotData* pSlotData );
	void SwapSlotData( CKTDGUISlot* pSlot );			// 슬롯 두 개의 데이터를 바꾼다

	void SetID(int id){ m_iID = id; }
	int GetID(){ return m_iID; }

protected:
		
	//void ShowGuideDesc( bool bOpen );
	void UpdateDraggingVertex( VERTEX_UI& vertexLT, VERTEX_UI& vertexRT, VERTEX_UI& vertexLB, VERTEX_UI& vertexRB, const UIPointData& pointData  );

#ifdef DLL_BUILD
	virtual bool IsSelectByEditGui( POINT pt ) override
	{
		return ContainsPoint(pt);
	}

	virtual void MoveControl( float fx, float fy ) override;
	virtual void MoveSubControl( float fx, float fy, wstring subControlName ) override;

	virtual void SetEditGUI( bool bEdit ) override;		// GUI 에디트 모드 설정

	virtual void ShowSubView( wstring name, bool bView ) override;
	virtual vector<D3DXVECTOR2> GetPosList() override;		// 컨트롤 내에 pictures의 위치 정보
	virtual D3DXVECTOR2 GetPos(wstring name) override;

	void DrawEditEdge();		// UITool에서 편집용으로 사용된다.

protected:
	bool m_bEditEdge;
	CKTDXDeviceTexture * m_pCheckedEdgeTexture;
#endif


private:
	CKTDGUIControl::UITextureData*	m_pItemTex;				// 아이템 그림
	//CKTDGUIControl::UITextureData*	m_pDraggingItemTex;		// 드래그 중인 아이템 그림
 	CKTDGUIControl::UITextureData*	m_pMouseOverTex;		// 마우스 올리면 표시될 그림

	CKTDGUIControl::UIStringData*		m_pStringData;

	CKTDGUIControl::UIPointData*		m_pNormalPoint;
	CKTDGUIControl::UIPointData*		m_pMouseOverPoint;
	CKTDGUIControl::UIPointData*		m_pDraggingPoint;
	
	CKTDGUIControl::VERTEX_UI			m_Vertex[4];
	CKTDGUIControl::VERTEX_UI			m_DragVertex[4];

	static CKTDGUISlot*				s_pDragSlot;					// static. 현재 드래그 되고 있는 슬롯

	bool							m_bDragable;
	D3DXCOLOR						m_cDraggingSlotColor;			// 드래깅 중일 때 마우스 위에 뜨는 그림의 색깔
	D3DXCOLOR						m_cDraggedSlotColor;			// 드래깅 중일 때 원래 위치에 남은 그림의 색깔
	D3DXCOLOR						m_cNormalSlotColor;				// 보통때 슬롯 컬러
	D3DXCOLOR						m_cMouseOverColor;				// 마우스 오버 그림의 컬러

	std::map<int, CKTDGUIControl::CPictureData*>			m_mapAdditionalPictureDataList;		// 추가적으로 집어넣는 Picture. 여러가지 용도의 그림이 슬롯 위에 올 것을 대비해서 map으로 만들어서 key-picture로 사용할 수 있게 함.


	CKTDGUISlot*					m_pDroppedSlot;					// 자신에게 Drop된 Slot. Drop과 동시에 s_pDragSlot을 NULL로 만들기 때문에 필요. 해당 메시지를 받았을 때 외에는 참조하지 않도록 하자!
	SLOT_CONTROL_STATE				m_eControlState;

	CKTDXDeviceSound*					m_pSndMouseOver;
	CKTDXDeviceSound*					m_pSndMouseUp;
	CKTDXDeviceSound*					m_pSndMouseDown;

	int									m_CustomMsgMouseOut;
	int									m_CustomMsgMouseOver;
	int									m_CustomMsgMouseDown;
	int									m_CustomMsgMouseUp;
	int									m_CustomMsgMouseRightUp;
	int									m_CustomMsgDropped;				// 다른 어떤 Slot이 내 머리위로 Drop 되었을 때 날릴 메시지
	//int								m_CustomMsgMouseDblClk;

	bool								m_bMouseOut;	// 마우스가 over된 상태에서 normal 상태로 바뀌는지

	CKTDGUISlotData*					m_pSlotData;
	int									m_iID;

	CKTDXDeviceTexture*					m_pEdgeTexture;

};

#endif
