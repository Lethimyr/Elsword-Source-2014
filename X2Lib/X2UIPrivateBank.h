//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK

#pragma once

namespace		_CONST_UI_PRIVATE_BANK_
{
	const	int		g_iBankQuestID		= 7035;		// 은행을 확장 시켜주는 퀘스트 ID
	const	int		g_iBankQuestItemID	= 99380;	// 은행을 확장 시켯주는 퀘스트 ItemID
}

class CX2UIPrivateBank : public CX2UIDragable
{
public:

	enum	UI_PRIVATE_BANK_CUSTOM_MSG
	{
		UPBCM_EXIT			= 34000,
		UPBCM_CASHSHOP,
//#ifdef GOOD_ELSWORD //JHKang	// 해외팀 주석
		UPBCM_ED,
		UPBCM_BUY_CONFIRM,
		UPBCM_BUY_CANCEL,
//#endif //GOOD_ELSWORD
//#ifdef SERV_SHARING_BANK_TEST
		UPBCM_USER_CHANGE,
		UPBCM_USER_SELECT,
		UPBCM_CHANGE_INVENTORY_SLOT_ITEM_OK,
		UPBCM_WANT_BUY_SHARING_CASH_ITEM,		// 지헌 : 은행 안 뚫었을 경우, 구매 하시겠습니까? 물음에 OK 누르면 호출
//#endif SERV_SHARING_BANK_TEST
	};
	
	CX2UIPrivateBank( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	virtual	~CX2UIPrivateBank();

	bool				InitUIPrivateBank( const CKTDXStage* const pNowStage, const WCHAR* const pFileName );
	void				DestoryPrivateBank();

	virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
	virtual bool		MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				GetShow() const { return m_bShow; }
	void				SetShow( bool bShow );

	void				SetLayer( X2_DIALOG_LAYER layer );
	D3DXVECTOR2			GetDLGSize() const { return m_vDlgSize; }
	void				SetPosition( D3DXVECTOR2 vec, bool bAbsolute = true );
    //const CX2Inventory*		GetInventory() const;								// 후에 const를 붙여보기!!
	const CX2Inventory*		GetMyInventory() const;								// 후에 const를 붙여보기!!
    CX2Inventory*		AccessMyInventory();								// 후에 const를 붙여보기!!
	std::wstring		GetStrMembership( CX2Inventory::MEMBERSHIP_PRIVATE_BANK membershipGrade ) const;	// membership에 따른 회원등급 string
#ifdef GOOD_ELSWORD //JHKang
	UINT				GetGradeSize( IN CX2Inventory::MEMBERSHIP_PRIVATE_BANK membershipGrade_ ) const;		// 현재 등급에 따른 다음 등급 사이즈 얻기
#endif //GOOD_ELSWORD
	void				UpdateUICashShopButton();					// 캐시샵으로 이동할 수 있는 버튼 이미지 업데이트
	void				UpdateUIStrMembership();					// (일반 회원), (골드 회원) 등의 string 출력
	void				UpdateUIUsedAndMaxNumOfSlot();				// 사용중인 슬롯의 갯수와 현재 등급에서 최대 사용 할 수 있는 슬롯의 갯수
	void				UpdateSlotList();
	void				UpdateUI();
	void				LostItemSlotList( bool bShow = false, bool bEnable = false );
	
#ifdef	SERV_SHARING_BANK_TEST
	void				ResetPrivateBank();

	void				SetShareNickName( wstring wstrNickName ) { m_wstrShareNickName = wstrNickName; }
	void				SetIsShareBank( bool bEnable ) { m_bIsOpenedShareBank = bEnable; }
	void				SetShareBankSize( int iSize ) { m_iBankSize = iSize; }
	void				SetShareUnitUID( UidType iShareUnitUID ) { m_ShareUnitUID = iShareUnitUID; }

	std::wstring		GetShareNickName() { return m_wstrShareNickName; }
	bool				GetIsShareBank() { return m_bIsOpenedShareBank; }
	int					GetShareBankSize() { return m_iBankSize; }
	UidType				GetShareUnitUID() { return m_ShareUnitUID; }

	bool				Handler_EGS_GET_SHARE_BANK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_SHARING_BANK_TEST

#ifdef	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
	bool				Handler_EGS_SHARE_BANK_UPDATE_OK_NOT();
#endif	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE

	virtual void		DrawSlotMouseOverImage();
	virtual wstring		GetSlotItemDesc();

	virtual bool		MouseDown( D3DXVECTOR2 mousePos );
	virtual bool		MouseUp( D3DXVECTOR2 mousePos );
	virtual bool		OnDropAnyItem( D3DXVECTOR2 mousePos );
	virtual bool		OnRClickedItem( D3DXVECTOR2 mousePos );

	//void				ToggleCashShop();
	
protected:
	bool				Handler_EGS_GET_MY_BANK_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef GOOD_ELSWORD //JHKang
#ifndef NO_GOOD_ELSWORD_INT
	bool	Handler_EGS_GET_NEXT_BANK_ED_REQ();
	bool	Handler_EGS_GET_NEXT_BANK_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool	Handler_EGS_EXPAND_BANK_SLOT_ED_REQ();
	bool	Handler_EGS_EXPAND_BANK_SLOT_ED_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif NO_GOOD_ELSWORD_INT
#endif //GOOD_ELSWORD

private:
	std::wstring					m_wstrDlgFileName;
	CKTDGUIDialogType				m_pDlgPrivateBank;
	bool							m_bShow;
	D3DXVECTOR2						m_vDlgSize;
	int								m_iMaxNumOfSlot;			// 현재 등급에서 사용할 수 있는 최대 슬롯의 갯수
	int								m_iUsedNumOfSlot;			// 사용중인 슬롯의 갯수
	//CX2CashShop*					m_pCashShop;

	//std::wstring					m_wstrNickName;				// 캐릭터 닉네임

#ifdef GOOD_ELSWORD //JHKang
	int					m_iConfirmED;
	CKTDGUIDialogType	m_pDLGBuyConfirm;
#endif //GOOD_ELSWORD

#ifdef SERV_SHARING_BANK_TEST
	bool							m_bIsOpenedShareBank;		// 은행 공유 열려 있는지
	int								m_iBankSize;				// 은행 크기
	std::wstring					m_wstrShareNickName;		// 현재 보고있는 캐릭터 닉네임
	UidType							m_ShareUnitUID;
	bool							m_bReserveResetDialog;		// true 일 경우 UI리셋
	CKTDGUIDialogType				m_pDlgWantBuySharingBank;	// 은행 공유 살꺼냐? 메시지 띄울 DLG
#endif SERV_SHARING_BANK_TEST

#ifdef SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
	CKTDGUIDialogType				m_pDLGWaitingMsgBox;
#endif SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
};

#endif	PRIVATE_BANK
		//}} kimhc // 2009-08-04 // 캐릭터별 은행