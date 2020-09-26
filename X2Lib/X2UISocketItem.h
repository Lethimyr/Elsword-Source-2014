#ifdef NEW_VILLAGE_UI

#pragma once
//{{ oasis907 : 김상윤 [2010.4.5] // 소켓 강화 UI 개편
#ifdef SERV_SOCKET_NEW


namespace		_CONST_UISOCKETITEM_INFO_
{
	const int	g_iMaxNumSocketInItem = MAX_SOCKET_SLOT_NUM;
	const float	g_fSocketEquipEffectFlickDelay = 0.08f;
	

	// 소켓 강화 수수료 상수
	const double	g_dSocketCostFactor = 0.01;

	// 마법석 등급에 따른 수수료 상수 
	const double	g_dNormalStoneCostFactor = 1.0;
	const double	g_dRareStoneCostFactor = 1.5;
	const double	g_dEliteStoneCostFactor = 2.0;
	const double	g_dUniqueStoneCostFactor = 2.5;

	const int	g_iArrayMagicStoneID[] = 
	{ 
		109945, 109955, 132110, 132111, 132112, 132113, 132114, 132115, 
		111701, 111702, 111703, 111704, 111705, 135184,
		98004, 98005, 98006, 98007, 98008, 98009, 98010, 98011, 98012,
		98013, 98014, 98015, 98016, 98017, 98018, 98019,
		// 여기서 부터는 고정 옵션
		132193,	132194, 132195, 132196, 132197, 132198, 132199,	
		132200, 132201, 132202, 132203, 132204, 132205, 72510, 72520,
		98028, 98029, 98030, 98031, 98032, 98033,
		//#ifdef SERV_FISHING_HOLE_MAGIC_STONE
		60004121, 60004122, 60004123, 60004124, 60004125,
		//#endif SERV_FISHING_HOLE_MAGIC_STONE
		//#ifdef SERV_MISS_ELIOS_STONE_ITEM
		67003750,
		//#endif SERV_MISS_ELIOS_STONE_ITEM
		//#ifdef SERV_BC_EVENT
		60005473,
		//#endif SERV_BC_EVENT
		//#ifdef SERV_LURIEL_MAGIC_STONE
		85003840, 85003841, 85003842, 85003843, 85003844, 85003845, 85003846, 85003847, 85003848, 85003849, 85003850,
		152000699, 152000700, 152000701, 152000702, 152000703
		//#endif SERV_LURIEL_MAGIC_STONE
	};

	const int	g_iArrayMagicStoneStringID[] = 
	{ 
		STR_ID_5073,	STR_ID_5073,	STR_ID_5074,	STR_ID_5075,	STR_ID_5076,	STR_ID_5077,	STR_ID_5078,	STR_ID_5079, 
		STR_ID_13900,	STR_ID_13901,	STR_ID_13902,	STR_ID_13903,	STR_ID_13904,	STR_ID_5073,	STR_ID_5094,	STR_ID_5097,
		STR_ID_5100,	STR_ID_5095,	STR_ID_5096,	STR_ID_5091,	STR_ID_5089,	STR_ID_5093,	STR_ID_5092,	STR_ID_13618,
		STR_ID_13619,	STR_ID_13901,	STR_ID_13900,	STR_ID_13903,	STR_ID_13902,	STR_ID_13904, 
		// 여기서 부터는 고정 옵션
		STR_ID_5085,	STR_ID_5086,	STR_ID_5087,	STR_ID_5088,	STR_ID_5089,	STR_ID_5090,	STR_ID_5091,	STR_ID_5092,
		STR_ID_5093,	STR_ID_5094,	STR_ID_5095,	STR_ID_5096,	STR_ID_5097,	STR_ID_13618,	STR_ID_13619,	STR_ID_5073,
		STR_ID_5073,	STR_ID_5073,	STR_ID_5073,	STR_ID_5073,	STR_ID_5073,
		//#ifdef SERV_FISHING_HOLE_MAGIC_STONE
		STR_ID_13900,	STR_ID_13901,	STR_ID_13902,	STR_ID_13903,	STR_ID_13904,
		//#endif SERV_FISHING_HOLE_MAGIC_STONE
		//#ifdef SERV_MISS_ELIOS_STONE_ITEM
		STR_ID_5073,
		//#endif SERV_MISS_ELIOS_STONE_ITEM
		//#ifdef SERV_BC_EVENT
		STR_ID_5073,
		//#endif SERV_BC_EVENT
		//#ifdef SERV_LURIEL_MAGIC_STONE
		STR_ID_13618, 	STR_ID_13619,	STR_ID_5089,	STR_ID_5090,	STR_ID_5091,	STR_ID_5092,	STR_ID_5093,	STR_ID_5094,	STR_ID_5095,	STR_ID_5096,	STR_ID_5097,
		STR_ID_13900,	STR_ID_13901,	STR_ID_13902,	STR_ID_13903,	STR_ID_13904,
		//#endif SERV_LURIEL_MAGIC_STONE
	};
}



class CX2UISocketItem : public CX2UIDragable
{
public:
	enum UI_SOCKET_ITEM_CUSTOM_MSG
	{
		USICM_EXIT							= 40000,
		USICM_SOCKET_EQUIP,
		USICM_VERIFY_EQUIP_OK,
		USICM_ENTER_CODE,
		USICM_SOCKET_SECRET,
		USICM_SOCKET_EQUIP_DEV,
	};

	enum SOCKET_SLOT_STATE
	{
		SSS_NOT_AVAILABLE,
		SSS_EMPTY,
		SSS_READY_TO_EQUIP,
		SSS_EQUIPPED,
	};

	struct SOCKET_SLOT_INFO
	{
		SOCKET_SLOT_STATE m_StateSlotState;
		UidType m_StoneUID;
		int		m_iQuantityStoneRemain;
		int		m_iEquippedSocketOption;
		
		SOCKET_SLOT_INFO()
		{
			m_StateSlotState = SSS_NOT_AVAILABLE;
			m_StoneUID =-1;
			m_iQuantityStoneRemain = -1;
			m_iEquippedSocketOption = 0;
		}

	};

	CX2UISocketItem( CKTDXStage* pNowStage, const WCHAR* pFileName );
	~CX2UISocketItem(void);

 	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
 	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	
	void SetShow(bool bShow);

	void ResetSocketWindow( UidType Uid );

	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }

	void SetPosition(D3DXVECTOR2 vec);
	void SetLayer(X2_DIALOG_LAYER layer);
	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos );
	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );
	virtual void DrawSlotMouseOverImage();


	bool Handler_EGS_SOCKET_ITEM_REQ( bool bDev = false);
	bool Handler_EGS_SOCKET_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	wstring GetSlotItemDesc();

#ifdef FIX_SOCKET_ICON_TEXTURE
	// 추가된 마법석 아이콘의 텍스쳐 파일명 식별 하기 위해 수정
	void SetSocketIconTextureBySocketOption(int iSoketIndex, int iSocketOption);
#else
	wstring GetSocketIconTextureKeyBySocketOption(int iSocketOption);
#endif //FIX_SOCKET_ICON_TEXTURE
	wstring GetSocketIconTextureNameBySocketTID(int TID);

	int CalculateNewSocketCost(UidType UID);
	int CalculateReplaceSocketCost(UidType UID);
	void UpdateUICostED();

	bool IsStoneItem(UidType Uid);

	void ShowSocketDescExpected(int iSlotIndex);

	void UpdateButtonEquip();
	void SetEnableButtonEquip(bool bEnable);

	void SocketEquipEffect();

	void SocketEquipReadyEffectStart();
	void SocketEquipReadyEffectEnd();
	
	bool CheckSocketUseItem(UidType Uid);

	UidType GetSocketItemUID(){ return m_SocketItemUID;}

protected:
#ifdef ADD_SOCKET_SLOT
	int GetSocketSlotNum( const CX2Item* pItem_ );
#endif // ADD_SOCKET_SLOT
private:
	
	bool					m_bFashion;
	int						m_iTotalCostED;
	int						m_iSocketCount;

	int						m_ItemPrice;

	bool					m_bShow;
	int						m_PickedSocketIndex;
	UidType					m_SocketItemUID;
	D3DXVECTOR2				m_MovedPosition;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSocketSeq1[_CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem];

	CKTDGUIDialogType		m_pDlgSocketItem;
#ifdef UI_SOCKET_NEW_CHEAT
	CKTDGUIDialogType		m_pDlgSocketItemCheat;

	CKTDGUIIMEEditBox*		m_pIMEEditSocketOptionCode[_CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem];

	int						m_iSocketOptionCode[_CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem];;

#endif UI_SOCKET_NEW_CHEAT
	X2_DIALOG_LAYER			m_Layer;

	SOCKET_SLOT_INFO		m_vecSocketSlotInfo[_CONST_UISOCKETITEM_INFO_::g_iMaxNumSocketInItem];


	bool					m_bMouseOverSoundCondition_1;
	bool					m_bMouseOverSoundCondition_2;


};
#endif SERV_SOCKET_NEW
//}} oasis907 : 김상윤 [2010.4.5] //  소켓 강화 UI 개편
#endif
