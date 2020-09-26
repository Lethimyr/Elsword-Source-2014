#pragma once

class CX2Cursor
{
	public:

		enum X2_CURSOR_STATE
		{
			XCS_NORMAL = 0,
			XCS_SELL,
			XCS_REPAIR,
			XCS_DECOMPOSITION,		// 분해
			XCS_ENCHANT,			// 강화 
			XCS_SOCKET,
			XCS_REMOVE_SKILL,
			XCS_ATTRIB_ENCHANT,		// 속성 인챈트
			XCS_ITEM_EXCHANGE,		// 아이템 보상 교환 커서
#ifdef ATTRACTION_ITEM_TEST
			XCS_ATTACH_ITEM,		// 강화권 관련
#endif
#ifdef SERV_ATTRIBUTE_CHARM
			XCS_ATTACH_ATTRIB_ITEM, // 속성 강화권
#endif SERV_ATTRIBUTE_CHARM
#ifdef AUTH_DELETE_ITEM
			XCS_DELETE_ITEM,
#endif

			//{{ kimhc // 2009-08-19 // 아이템 봉인
#ifdef	SEAL_ITEM
			XCS_SEAL_ITEM,
#endif	SEAL_ITEM
			//}} kimhc // 2009-08-19 // 아이템 봉인
#ifdef SERV_SKILL_NOTE
			XCS_REGISTER_MEMO,		// 기술의 노트 메모 등록
#endif
#ifdef EVENT_SCENE
			XCS_EVENT_SCENE,
#endif EVENT_SCENE

#ifdef SERV_PET_SYSTEM
			XCS_PET_FOOD,
#endif
#ifdef CASH_ITEM_REFUND
			XCS_CASH_REFUND,
#endif CASH_ITEM_REFUND
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05//미라클큐브
			XCS_ITEM_EVALUTE,		// 아이템 미감정 상태로 되돌리기
			XCS_EXCHANGE_NEW_ITEM,	// 아이템 교환
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05
//#ifdef FIELD_BOSS_RAID
			XCS_ADD_SOCKET_SLOT,	// 소켓 슬롯 확장
//#endif // FIELD_BOSS_RAID
		};

		CX2Cursor(void);
		virtual ~CX2Cursor(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );
		HRESULT OnFrameRender();

		void	SetPos( D3DXVECTOR2 pos ) { m_Pos = pos; }
		void	SetSize( D3DXVECTOR2 size ) { m_Size = size; }
		void	SetShow( bool bShow );

		bool	GetShowHardwareCursor();

		void	ChangeCursorState( X2_CURSOR_STATE cursorState ); //특별한 공식이 있음 ㅋㅋ
		X2_CURSOR_STATE GetCurorState() { return m_CursorState; }

		HCURSOR GetCursorHandle() { return m_hCursor; }

		

	private:

		struct DRAWFACE_RHW_VERTEX
		{
			float x, y, z, rhw;
			DWORD color;
			float u, v;
		};

	protected:

		void DrawFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color = 0xffffffff );

		HCURSOR				m_hCursor;

//{{ robobeg : 2008-10-13
	    CKTDGStateManager::KStateID m_RenderStateID;
//}} robobeg : 2008-10-13
		CKTDXDeviceTexture*	m_pCursorSellTex;
		CKTDXDeviceTexture* m_pCursorRepairTex;
		CKTDXDeviceTexture*	m_pCursorDraggingTex;
		CKTDXDeviceTexture*	m_pCursorDecompositionTex;
		CKTDXDeviceTexture*	m_pCursorEnchant;
		CKTDXDeviceTexture*	m_pCursorSocket;
		CKTDXDeviceTexture*	m_pCursorRemoveSkill;
		CKTDXDeviceTexture*	m_pCursorAtrrib;
		CKTDXDeviceTexture*	m_pCursorItemExchange;
#ifdef ATTRACTION_ITEM_TEST
		CKTDXDeviceTexture* m_pCursorAttachItem;
#endif
#ifdef AUTH_DELETE_ITEM
		CKTDXDeviceTexture* m_pCursorDeleteItem;
#endif
		
			//{{ kimhc // 2009-08-19 // 아이템 봉인
#ifdef	SEAL_ITEM
		CKTDXDeviceTexture*	m_pCursorSealItem;
#endif	SEAL_ITEM
			//}} kimhc // 2009-08-19 // 아이템 봉인

#ifdef SERV_SKILL_NOTE
		CKTDXDeviceTexture*	m_pCursorRegisterMemo;
#endif
#ifdef CASH_ITEM_REFUND
		CKTDXDeviceTexture* m_pCursorCashRefund;
#endif CASH_ITEM_REFUND
#ifdef EVENT_SCENE
		CKTDXDeviceTexture*	m_pCursorEventScene;
#endif EVENT_SCENE

#ifdef SERV_PET_SYSTEM
		CKTDXDeviceTexture*	m_pCursorPetFood;
#endif
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05//미라클큐브
		CKTDXDeviceTexture* m_pCursorRestoreItemEvalutation;	// 미확인 상태로 변경 커서
		CKTDXDeviceTexture* m_pCursorExchangeNewItem;			// 2013_5 개편 후 아이템으로 변경 커서
#endif // SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef ADD_SOCKET_SLOT // 소켓 슬롯 확장
		CKTDXDeviceTexture* m_pCursorAddSocketSlot;
#endif // ADD_SOCKET_SLOT

		D3DXVECTOR2			m_Pos;
		D3DXVECTOR2			m_Size;

		float				m_fTimeForHide;
		bool				m_bShow;

		X2_CURSOR_STATE		m_CursorState;

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//		LPDIRECT3DVERTEXBUFFER9 m_pVB;
//		D3DXVECTOR2			m_PosVB;
//		D3DXVECTOR2			m_SizeVB;
//		D3DCOLOR            m_d3dColorVB;
//#endif
};

