#pragma once

class CX2SlotItem;

class CX2SlotManager
{
public:

	class CX2Slot
	{
	public:

		enum SLOT_TYPE
		{
			ST_NONE,
			ST_EQUIPPED,
			ST_INVENTORY,
			ST_BANK,			//kimhc // 2009-08-04 ST_REPOSITORY -> ST_BANK로 변경 
			ST_SHOP_BUY,
			ST_SHOP_PREVIEW,
			ST_RECIPE,
			ST_SUPPLEMENT, //보조제
			ST_MATERIAL,
			ST_CASH_SHOP_MAIN_NEW,
			ST_CASH_SHOP_MAIN_HOT,
			ST_CASH_SHOP_MAIN_RECOMMEND,
			ST_CASH_SHOP_SUB,
			ST_CASH_SHOP_SUB_SPECIAL,
			ST_CASH_SHOP_DEPOSIT,	//캐시 아이템 보관함 
			ST_CASH_SHOP_CHOICED,	//찜
            ST_POST_SEND,
            ST_POST_RECEIVE,
			ST_UI_WATCH,			// 엿보기창 슬롯 추가 : 2008.11.19 김태완
			ST_PET,					// oasis907 : 김상윤 [2010.9.2] // 펫 인벤토리
			ST_RESOLVE,				// #ifdef SERV_SYNTHESIS_AVATAR
			ST_SYNTHESIS,			// #ifdef SERV_MULTI_RESOLVE
			ST_END,
		};

	public:

		CX2Slot(void);
		virtual ~CX2Slot(void);

		void SetSlotManager( CX2SlotManager* pSlotManager );
		CX2SlotManager* GetSlotManager() { return m_pSlotManager; }

		virtual void SetEnable( bool bEnable, bool bEffectItemImage = true );
		virtual void SetShow( bool bShow, bool bEffectItemImage = true );

		virtual bool GetClickable() const;
		virtual void SetClickable(bool val);

		bool GetEnable() { return m_bEnable; }

		SLOT_TYPE GetSlotType() { return m_SlotType; }
		int	GetSlotID() { return m_SlotID; }

		D3DXVECTOR2 GetPos();
		void SetPos( D3DXVECTOR2 pos );
		D3DXVECTOR2 GetSize() { return m_Size; }
#ifdef SERV_NEW_DEFENCE_DUNGEON
		void SetSize(D3DXVECTOR2 val) { m_Size = val; }
#endif //SERV_NEW_DEFENCE_DUNGEON

		virtual void SetSlotData_LUA();

		virtual bool IsInMousePoint( D3DXVECTOR2 mousePos );

		virtual	void SetDialog( CKTDGUIDialogType pDLGSlot );
		virtual CKTDGUIDialogType GetDialog() { return m_pDLGSlot; }

		virtual CKTDGUIDialogType GetDLGSlotFixBack() { return m_pDLGSlotFixBack; }

		virtual void ResetPos();
#ifdef NEW_VILLAGE_UI
		// 2008.11.13 김태완
		bool	IsItem();
#endif
		//{{ 09.03.18 김태완
		X2_DIALOG_LAYER			GetNormalLayer(){ return m_NormalLayer; }
		void					SetNormalLayer( X2_DIALOG_LAYER layer ){ m_NormalLayer = layer; }

		//}}

	protected:

		virtual void LuaGetValue( KLuaManager& luaManager );
		//virtual void LuaTinkerCall();

		int						m_SlotID;

		CX2SlotManager*			m_pSlotManager;

		bool					m_bEnable;
		bool					m_bShow;
		bool					m_bClickable;

		SLOT_TYPE				m_SlotType;
		D3DXVECTOR2				m_Pos;
		D3DXVECTOR2				m_Size;

		CKTDGUIDialogType			m_pDLGSlotFixBack;
		CKTDGUIDialogType			m_pDLGSlot;

		//{{ 09.03.18 김태완
		X2_DIALOG_LAYER			m_NormalLayer;
		//}}
	};

public:

	CX2SlotManager( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2SlotManager(void);

	virtual void SetSlotManagerLayer( int slotManagerLayer );
	virtual int	GetSlotManagerLayer() { return m_SlotManagerLayer; }
	// 2008.12.16 김태완 : 유틸리티 함수 추가. 
	virtual void SetSlotManagerChangeSequence( bool bFront ); // 해당 레이어에서 가장 뒤로 보낼지 앞으로 보낼지
	////

	//////////////////////////////////////////////////////////////////////////
	//상속받은 클래스에서 꼭 불러줘야하는 함수들

	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//////////////////////////////////////////////////////////////////////////
	

	//kimhc // 2009-08-04 // 루아에서 slot 생성 시 쓸수 있는 Dummy 값
	void	SetDummyValue( int iDummyValue ) { m_iDummyValue = iDummyValue; }
	int		GetDummyValue_LUA() const { return m_iDummyValue; }
	virtual void AddSlot( CX2Slot* pSlot );
	
	CX2SlotItem* CreateItemSlot();

	virtual void SetEnable( CX2Slot::SLOT_TYPE slotType, bool bEnable );
	virtual void SetShow( CX2Slot::SLOT_TYPE slottype, bool bShow );

	virtual void SetEnable( bool bEnable );
	virtual void SetShow( bool bShow );

	virtual void SetPos( const D3DXVECTOR2& pos ) { m_Pos = pos; }
	virtual D3DXVECTOR2 GetPos() { return m_Pos; }

	virtual CX2Slot* GetSlot( int index );
	virtual CX2Slot* GetSlot( CX2Slot::SLOT_TYPE slotType, int slotID );
	virtual int GetSlotMaxNum() { return (int)m_SlotList.size(); }
#ifdef NEW_VILLAGE_UI
	static CX2Slot* GetSlotBeforeDragging() { return *m_pSlotBeforeDragging; }
	bool GetIsNowDraggingItem() { return (*m_pSlotBeforeDragging != NULL); }
#else
	static CX2Slot* GetSlotBeforeDragging() { return m_pSlotBeforeDragging; }
	bool GetIsNowDraggingItem() { return (m_pSlotBeforeDragging != NULL); }
#endif


	virtual void SetNowDragItemReturn();

	virtual wstring GetSlotItemDesc() = NULL;
	virtual void DrawSlotDesc() = NULL;

	void SetNowOverItemSlotAndDesc( CX2SlotItem* pSlotItem ) 
	{ 
		KTDXPROFILE();

		m_pNowOverItemSlot = pSlotItem;
		
		if( NULL != pSlotItem )
		{
			DrawSlotDesc();
		}
		else
		{
			if ( m_pDLGItemDesc != NULL )
				m_pDLGItemDesc->SetShow( false );

			if ( m_pDLGItemDescCompare != NULL )
				m_pDLGItemDescCompare->SetShow( false );

#ifdef FIX_TOOLTIP
			if ( m_pDLGItemDescAdd != NULL )
				m_pDLGItemDescAdd->SetShow( false );

			if ( m_pDLGItemDescCompareAdd != NULL )
				m_pDLGItemDescCompareAdd->SetShow( false );
#endif FIX_TOOLTIP

		}
	}
	
	//{{ mauntain : 김태환 [2012.07.16] 인벤토리 및 슬롯을 고려한 아이템 구입 갯수 연산
#ifdef SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	int GetUsedSlotMaxNum();	/// 현재 사용 가능한 퀵슬롯 갯수
	int GetUsedSlotNum();		/// 현재 사용중인 퀵슬롯 갯수
#endif SERV_AUTOMATICALLY_REGISTER_FOR_CONSUMABLE_ITEM
	//}}

#ifdef SERV_UPGRADE_TRADE_SYSTEM // 김태환

	CX2SlotItem* GetSlotByItemUID( IN UidType uidItemUID_ );		/// ItemUID를 통해 SlotItem 객체를 반환하는 함수

#endif //SERV_UPGRADE_TRADE_SYSTEM

protected:

	//////////////////////////////////////////////////////////////////////////
	//클래스 이름에 맞게 상콤한 네이밍으로 오버라이딩해서 사용하세요.
	virtual void RegisterLuaBind();
	//////////////////////////////////////////////////////////////////////////
	

	virtual bool MouseDown(  D3DXVECTOR2 mousePos );
	virtual bool MouseUp(  D3DXVECTOR2 mousePos );
	virtual void MouseMove( D3DXVECTOR2 mousePos );

	virtual CX2Slot* GetSlotInMousePos( D3DXVECTOR2 mousePos );

	bool OpenScriptFile( const WCHAR* pFilename );

protected:

	CX2State*				m_pNowState;

	vector< CX2Slot* >		m_SlotList;

	D3DXVECTOR2				m_Pos;
#ifdef NEW_VILLAGE_UI
	//2008.11.13 김태완 : Global 통하도록 수정합니다
	static CX2Slot**		m_pSlotBeforeDragging;	
#else
	static CX2Slot*			m_pSlotBeforeDragging;	
#endif
	CX2SlotItem*			m_pNowOverItemSlot;

	int						m_SlotManagerLayer;
	
	int						m_iDummyValue;				//kimhc // 2009-08-04 // 루아에서 slot 생성 시 쓸수 있는 Dummy 값


	CKTDGUIDialogType		m_pDLGItemDesc;
	CKTDGUIDialogType		m_pDLGItemDescCompare;

#ifdef FIX_TOOLTIP
	CKTDGUIDialogType		m_pDLGItemDescAdd;
	CKTDGUIDialogType		m_pDLGItemDescCompareAdd;
#endif FIX_TOOLTIP
};
