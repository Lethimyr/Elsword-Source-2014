//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP

#pragma once

namespace		_CONST_UI_ITEM_EXCHANGE_SHOP_
{
	const int	g_iNumOfSlotSet				=	5;		// 한페이지 당 슬롯셋트의 갯수
#ifdef SERV_9TH_NEW_CHARACTER // 해외팀 코드 추가

#ifdef SERV_LIMIT_UNIT
#if defined( SERV_COUNTRY_TWHK ) || defined( SERV_COUNTRY_JP ) || defined( SERV_COUNTRY_EU ) || defined( SERV_COUNTRY_US ) || defined( SERV_COUNTRY_CN )
	const int	g_iNumOfSlotInSlotSet = 8; // 엘리시스
#elif defined( SERV_COUNTRY_TH ) || defined( SERV_COUNTRY_ID ) || defined( SERV_COUNTRY_BR ) || defined( SERV_COUNTRY_PH ) || defined( SERV_COUNTRY_IN )
	const int	g_iNumOfSlotInSlotSet = 7; // 청
#else
	const int	g_iNumOfSlotInSlotSet = 9;
#endif //SERV_COUNTRY_XX
#else //SERV_LIMIT_UNIT
	const int	g_iNumOfSlotInSlotSet		=	9;		// 한 슬롯 셋트 당 교환아이템슷롯의 갯수(유저가 가진 아이템 슬롯 제외)	( 갯수 변경 : 6 -> 7 )
#endif //SERV_LIMIT_UNIT
	
#else
#if defined(SERV_NEW_CHARACTER_EL) && !defined(SERV_NEW_CHARACTER_EL_DISABLE)
	const int	g_iNumOfSlotInSlotSet		=	8;		// 한 슬롯 셋트 당 교환아이템슷롯의 갯수(유저가 가진 아이템 슬롯 제외)	( 갯수 변경 : 6 -> 7 )
#else
	const int	g_iNumOfSlotInSlotSet		=	7;		// 한 슬롯 셋트 당 교환아이템슷롯의 갯수(유저가 가진 아이템 슬롯 제외)	( 갯수 변경 : 6 -> 7 )
#endif SERV_NEW_CHARACTER_EL
#endif SERV_9TH_NEW_CHARACTER
}
class CX2UIItemExchangeShop : public CX2ItemSlotManager
{
public:

	enum	ITEM_EXCHANGE_SHOP_CUSTOM_MSG
	{
		IESCM_EXIT					= 33000,
		IESCM_PUSH_PREV_PAGE,
		IESCM_PUSH_NEXT_PAGE,
		IESCM_FIRST_EXCHANGE_BUTTON,
		IESCM_SECOND_EXCHANGE_BUTTON,
		IESCM_THIRD_EXCHANGE_BUTTON,
		IESCM_FOURTH_EXCHANGE_BUTTON,
		IESCM_FIFTH_EXCHANGE_BUTTON,
		IESCM_OK,
		IESCM_CANCEL,
	};

	struct	SlotSet
	{
		CKTDGUIStatic*			m_pStaticSlotSet;
		CKTDGUIButton*			m_pButtonForExchange;
		bool					m_bUserHave;			// 교환 대상 아이템을 유저가 가지고 있는가?
		int						m_eType;				// invalid, all, random, select 종료
		int						m_iSrcItemID;			// 교환 대상 itemID

		//int						m_iStartIndexForExchangeData;				// 현재 슬롯셋트에 속하는 element의 m_vecItemExchangeData, CX2SlotItem 벡터 내에서의 시작위치
		//int						m_iEndIndexForExchangeData;					// 현재 슬롯셋트에 속하는 element의 m_vecItemExchangeData, CX2SlotItem 벡터 내에서의 마지막위치
		//int						m_iStartIndexForSlotList;					// 현재 슬롯셋트에 속하는 element의 m_vecItemExchangeData, CX2SlotItem 벡터 내에서의 시작위치
		//int						m_iEndIndexForSlotList;						// 현재 슬롯셋트에 속하는 element의 m_vecItemExchangeData, CX2SlotItem 벡터 내에서의 마지막위치

		SlotSet()
		{
			m_pStaticSlotSet						= NULL;
			m_pButtonForExchange					= NULL;
			m_bUserHave								= false;
			m_eType									= CX2ItemManager::IET_INVALID;
			m_iSrcItemID							= -1;
			/*m_iStartIndexForExchangeData			= 0;
			m_iEndIndexForExchangeData				= 0;
			m_iStartIndexForSlotList				= 0;
			m_iEndIndexForSlotList					= 0;*/
		}
	};

public:
	CX2UIItemExchangeShop( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	virtual ~CX2UIItemExchangeShop();

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	bool				MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const;
	void				SetShow( bool bShow );
	D3DXVECTOR2			GetDLGSize() const;
	void				SetPosition( D3DXVECTOR2 vec );
	void				SetLayer( X2_DIALOG_LAYER layer );
	void				SetHouseID( int iHouseID );
	virtual		bool	MouseDown( D3DXVECTOR2 mousePos );
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
	void				InvalidLastSrcItem();
	void				InvalidLastSelectExchange();
	void				PlayWarningSelectExchange();
	void				StopWarningSelectExchange();
	bool				SelectExchangeSrcItem( UidType iSrcItemUID );
	bool				CheckExchangeUseItem( int itemID );
	bool				Handler_EGS_ITEM_EXCHANGE_REQ( UidType iSrcItemUID, int iSrcItemID, int iSrcQuantity, int iSelectedItemID = 0);
#ifdef EXCHANGE_OPEN_IMAGE
	bool				Handler_EGS_ITEM_EXCHANGE_REQ() { return Handler_EGS_ITEM_EXCHANGE_REQ( m_iSrcItemUID, m_iSrcItemID, m_iSrcQuantity, m_iDestItemID ); }
#endif EXCHANGE_OPEN_IMAGE
#endif SERV_ITEM_EXCHANGE_NEW

private:
	// 핸들러
	bool				Handler_EGS_ITEM_EXCHANGE_REQ( int iSrcItemID, int iSelectedItemID = 0);
	bool				Handler_EGS_ITEM_EXCHANGE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


private:
	// 멤버함수
	bool				InitItemExchangeData();
	bool				InitNowPageAndMaxPage();
	bool				UpdatePageString();
	bool				UpdatePageUI();
	bool				UpdateSlotSetUI( CKTDGUIStatic* pStaticSlotSet, CX2ItemManager::ITEM_EXCHANGE_TYPE eType );
	bool				UpdateSlotList();
	void				LostItemSlotList();
	//void				DestoryUIItemExchangeShop();
	int					SearchItemInUserInventory( int iItemID );	// 인벤토리에 해당 아이템이 없는 경우 -1 리턴
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
	bool				PushedExchangeButton( int iCustomMsg, bool bPopupCenter = false );
#else
	bool				PushedExchangeButton( int iCustomMsg );
#endif SERV_ITEM_EXCHANGE_NEW

    const wchar_t*      GetItemFullName_( int iItemID );
	//	bool				CompareVector( std::vector< CX2ItemManager::ItemExchangeData >& vFirst, std::vector< CX2ItemManager::ItemExchangeData >& vSecond );

private:
	// 멤버변수
	bool								m_bShow;
	const CKTDXStage* const				m_pNowStage;
	int									m_iHouseID;				// X2LocationManager::HOUSE_ID
	CKTDGUIDialogType						m_pDlgBackground;		// 아이템 교환 다이얼로그
	//CKTDGUIDialogType						m_pDlgSelectedSlot;		// m_pDlgSelectedItem은 mousemove에 사용되고, 이것은 mousedown에 사용됨
	D3DXVECTOR2							m_vSizeDlg;
	D3DXVECTOR2							m_vPosDlg;
	UINT								m_uiNowPage;			// 현재 보여지는 페이지 수
	UINT								m_uiMaxPage;			// 현재 보여줄 수 있는 최대 페이지 수
	UINT								m_uiSelectedSlotID;		// 선택된 슬롯의 ID
#ifdef SERV_ITEM_EXCHANGE_NEW // 디파인 잘 못 두른 것 해외팀 수정
	int									m_iLastCustomMsg;
	int									m_iLastSrcItemID;
	UidType								m_iLastSelectSrcItemUID;
	CKTDGUIDialogType					m_pDLGWarningSelectExchange;
#endif SERV_ITEM_EXCHANGE_NEW
	CKTDGUIDialogType						m_pDlgExchangeConfirm;	// 교환 확인 다이얼로그

	SlotSet								m_SlotSet[_CONST_UI_ITEM_EXCHANGE_SHOP_::g_iNumOfSlotSet];
	
	//std::vector< CX2ItemManager::ItemExchangeData > m_vecTempItemExchangeData;
	std::vector< std::vector< CX2ItemManager::ItemExchangeData > > m_vecItemExchangeData;

#ifdef SERV_EVENT_MONEY
	// EGS_UPDATE_EVENT_MONEY_NOT가 EGS_ITEM_EXCHANGE_ACK전에 오도록 패킷순서 보장을 받을 수 없기 때문에
	// EGS_UPDATE_EVENT_MONEY_NOT를 받기 전에 EGS_ITEM_EXCHANGE_ACK가 먼저 오는 경우
	// 깃털 값을 클라에서 적용 시켜 주기 위한 깃털 개수를 기록하는 변수
	int									m_iFeatherResultQuantity; 
#endif //SERV_EVENT_MONEY

#ifdef EXCHANGE_OPEN_IMAGE
	UidType		m_iSrcItemUID;
	int			m_iSrcItemID;
	int			m_iSrcQuantity;
	int			m_iDestItemID;
#endif EXCHANGE_OPEN_IMAGE
};

#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업