#pragma once


class CX2PostBox : public CX2UIDragable
{
public:
    enum POSTBOX_MANAGER_UI_MSG
    {
        PBMUM_RECEIVE           = 2100,
        PBMUM_SEND,
        PBMUM_SETTING,
        PBMUM_CLOSE,
    };

    enum POSTBOX_SEND_UI_MSG
    {        
        PBSUM_GET_MYUSER_NAME   = 2110,
        PBSUM_INVENTORY_TAB_CHANGED,
        PBSUM_PREV_PAGE,
        PBSUM_NEXT_PAGE,
        PBSUM_QUANTITY_CANCEL,
        PBSUM_MAIL_SEND,
        PBSUM_ED_CHANGE,
        PBSUM_QUANTITY_OK,
        PBSUM_QUANTITY_CHANGE,
        PBSUM_TITLE_CHANGE,
		PBSUM_RECEIVER_NAME_CHANGE,
		
		//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
		PBSUM_REGISTER_QUANTITY_PLUS	= 2121,
		PBSUM_REGISTER_QUANTITY_MINUS,
		PBSUM_REGISTER_QUANTITY_UPDATE,
		PBSUM_REGISTER_QUANTITY_CONFIRM,
		PBSUM_REGISTER_QUANTITY_CANCEL,
		//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편
    };

    enum POSTBOX_RECEIVE_UI_MSG
    {
        PBRUM_PREV_PAGE         = 2130,
        PBRUM_NEXT_PAGE,
        PBRUM_SYSTEM_MAIL,
        PBRUM_COMMON_MAIL,
        PBRUM_OPEN_MAIL,
        PBRUM_DELETE_MAIL,
        PBRUM_VIEW_MAIL,
// #ifdef SERV_RELATIONSHIP_SYSTEM 
		PBRUM_WEDDING_MAIL,
// #endif // #ifdef SERV_RELATIONSHIP_SYSTEM 
#ifdef ADD_POST_BOX_PAGE_BUTTON //2013.07.15
		PBRUM_FIRST_PAGE,		/// 첫 번째 페이지로
		PBRUM_LAST_PAGE,		/// 마지막 페이지로
#endif //ADD_POST_BOX_PAGE_BUTTON
	};   

    enum POSTBOX_DLG_EVENT
    {
        PBQM_SEND_ACCEPT			= 2140,
        PBQM_SEND_CANCEL,
        PBQM_DELETE_OK,
        PBQM_DELETE_CANCEL,
        PBQM_NEW_MAIL,
		//{{ kimhc // 2009-09-03 // 아이템 봉인 우편으로 전송시 확인 다이얼로그
		PBQM_SEAL_ITEM_SEND_ACCEPT,
		PBQM_SEAL_ITEM_SEND_CANCEL,
		//}} kimhc // 2009-09-03 // 아이템 봉인 우편으로 전송시 확인 다이얼로그
    };

    enum POSTBOX_VIEW_UI_MSG
    {        
        PBVUM_DELETE            = 2150,
        PBVUM_CLOSE,
        PBVUM_GET_ITEM,
        PBVUM_REPLY,        
    };

    enum POSTBOX_OPTION
    {
        PBOUM_CLOSE             = 2160,
        PBOUM_BLIST_ACCEPT,
        PBOUM_BLIST_DELETE,
        PBOUM_BLIST_SELECT,
        PBOUM_ALARM,
        PBOUM_NEW_LETTER,
        PBOUM_BLIST_OK,
        PBOUM_CHECK_ALARM,
        PBOUM_CHECK_NEWLETTER,
    };

// #ifdef SERV_RELATIONSHIP_SYSTEM
	enum POST_LETTER_TYPE		// 우편 타입
	{	
		PLT_SYSTEM = 0,				// 시스템
		PLT_NORMAL,			// 일반
		PLT_WEDDING,			// 청첩장
	};
// #endif // SERV_RELATIONSHIP_SYSTEM

    struct MailListSlot
    { 
        bool    m_bRead;        // header
        wstring m_strDate;      // header
        wstring m_strNickName;  // header
        wstring m_strTitle;     // header
        wstring m_strMessage;   // body
        bool    m_bAttachItem;  // header
        int     m_nAttachMoney; // body
        int     m_ItemId;       // header
        int     m_EnchantLevel; // header
        int     m_iKind;        // header
        bool    m_bCache;
        bool    m_bSelected;
        bool    m_bChecked;
        UidType m_postID;
        CX2Item *m_pItem;

    public:
        MailListSlot()
        {
            m_bRead         = false;
            m_strTitle      = L"";
            m_strDate       = L"";
            m_strNickName   = L"";
            m_strMessage    = L"";
            m_bAttachItem   = false;
            m_nAttachMoney  = 0;
            m_ItemId        = 0;
            m_EnchantLevel  = 0;
            m_bCache        = false;
            m_bSelected     = false;
            m_bChecked      = false;
            m_postID        = 0;
            m_pItem         = NULL;
            m_iKind         = 0;
        }
    };

	
public:
    CX2PostBox( CKTDXStage* pNowState );
    virtual ~CX2PostBox();

    void RegisterLuaBind();
    void InitPostBox();
    //void ClearAlarmNewLetter(double fTime, float fElapsedTime);

    HRESULT OnFrameMove( double fTime, float fElapsedTime );
    bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void MouseMove( D3DXVECTOR2 mousePos );
    virtual bool MouseDown( D3DXVECTOR2 mousePos );
    virtual bool MouseUp( D3DXVECTOR2 mousePos );	
    virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );	

    bool GetDialogShow()    { return m_bShow; }
    bool GetEnable()        { return m_bEnable; }

    // 상태 변경        
    void SetNowState( CX2State* pState );
    void SetDialogShow( bool bShow );
    void SetDialogShow( bool bShow, int flag ); // 0:Send, 1:Recieve, 2:Setting
    
    // 인벤토리 정렬    
    bool SetInventorySort();

    CX2Inventory::SORT_TYPE GetInventorySortType() { return m_NowInventorySortType; }

	void DrawSlotMouseOverImage();
    virtual bool DrawSlotMouseOverImageInEquipRect();
	    
    bool OnRClickedItemInInven( D3DXVECTOR2 mousePos );
    int IsInPostItemReact( D3DXVECTOR2 mousePos );
    CX2SlotItem* GetPostSlot();
    void EnablePostQuantityButton( bool bEnable );

    wstring GetSlotItemDesc();
    virtual wstring GetSlotItemDescExtra_RBtn( int itemTID, CX2Item* pItem );

    void SetSlotItem(CX2Item *pkItem);
    void OpenItemQuantityDLG( UidType sellItemUID );

    void InitPostSend();
    void SetSlotShowEnable(bool bShow, int flag);
    void SetTitle(WCHAR *szTitle) { m_strTitle = szTitle; }
    void SetNickName(WCHAR *szNickName) { m_strNickName = szNickName; }
    void CheckEdChange();
    void ChangeTitle();
	void ChangeReceiverName();
    void AttachItem();
    void SetItemNumChange( int iNum );
    void SendMail();
    void SetSystemMail(bool flag) { m_bSystemMail = flag; }
    void RequestList();
    void ChangePage(int step);
#ifdef ADD_POST_BOX_PAGE_BUTTON //2013.07.15
	void MoveFirstPage( IN bool bFirst_ );
#endif //ADD_POST_BOX_PAGE_BUTTON
    void OpenMail(int inx);
    void CheckDeleteMail(int inx = 0);   
    bool GetCheckMail(int inx);
    void SetMailView(int inx);
    void SetAttachItem();
    void BlackListReq();
    void ShowBlackList();

    void ReadPostOption();
    void SavePostOption();   

    bool GetCheckPitIn() { return m_bCheckPitIn; }
    void SetCheckPitIn(bool val) { m_bCheckPitIn = val; }

    void IncUnReadMail() { ++m_nUnReadMail; }
    int GetUnReadMail() { return m_nUnReadMail; }
    void IncTotalMail() { ++m_nTotalMail; }
    int GetTotalMail() { return m_nTotalMail; }

    void AddNewLetter( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    void ClearNewLetter();
    int GetNewLetter() { return m_iNewLetter; }

    void SetSwitchState(bool val) { m_bSwitch = val; }
    bool GetSwitchState() { return m_bSwitch; }
    
	void RegisterItem(CX2SlotItem* pItemSlot);

	//{{ 11.09.26 임규수 우편 수량성 아이템 첨부 취소 오류 수정
#ifdef POSTBOX_CANCEL_QUANTITY_ITEM_FIX
	void UnRegisterItem();
#endif
	//}}
	
	bool GetOpendSendWindow() 
	{ 
		return m_pDLGMailSend->GetShow(); 
	}
    
	bool GetShow() { return m_bShow; }
	void SetShow(bool val){ m_bShow = val; }

	bool OnRClickedItem( D3DXVECTOR2 mousePos );
	bool OnDropAnyItem( D3DXVECTOR2 mousePos );

	void UpdateUnReadMailNotice();

	//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
#ifdef POST_QUANTITY_NEW
	void OpenRegisterQuantityDLG( D3DXVECTOR2 pos );
	void UpdateRegisterQuantityDLG( bool bReadIME );
#endif	//	POST_QUANTITY_NEW
	//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편

protected:
    int AddMultiLine();

    // 슬롯 관련
    bool UpdateInventorySlotList( std::vector< KInventoryItemInfo >& vecInventorySlotInfo );
    virtual void RemoveEqip(UidType ItemUID);
    virtual void AddEqip(UidType ItemUID);

    void ResetInvenPage( int nowPage, int maxPage );
    void ResetInvenPageUI();
    

    void ChangeAlarm();
    void ChangeNewNotify();

    // 리스트 슬롯 관련
    void ClearMailList();
    void ClearListSlot();
    void SetListSlot(int inx);
    void SetListPage();

    bool SetMailHeader(int inx, KPostItemTitleInfo mailHeader);
    int SetMailBody( KPostItemInfo& mailBody );

    void GetItemLetter(int inx);
    void ReplyMail(int inx);

    void AddUser(wstring userName);
    void DeleteUser(wstring userName);
    wstring GetUserName(int inx);
	//{{ 2009. 6. 24  최육사	편지스트링조합
	wstring GetSelectedUserName();
	//}}

	// 우편 제목, 보낸이 길이 제한
	void LimitPostStringLength( OUT wstring& wstrTitle_, OUT wstring& wstrFromNickName_ );

#ifdef SERV_FREE_MAIL_TICKET
	int GetPostFee( IN int iED, IN const CX2Item* pItemData );
#endif //SERV_FREE_MAIL_TICKET

public:
    // 메일 전송
    
    bool Handler_EGS_SEND_LETTER_REQ();
    bool Handler_EGS_SEND_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 메일 삭제
    bool Handler_EGS_DELETE_LETTER_REQ();
    bool Handler_EGS_DELETE_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 메일 열기
    bool Handler_EGS_READ_LETTER_REQ(UidType postId);
    bool Handler_EGS_READ_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 메일 리스트
    bool Handler_EGS_GET_POST_LETTER_LIST_REQ();
    bool Handler_EGS_GET_POST_LETTER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 아이템 가져오기
    bool Handler_EGS_GET_ITEM_FROM_LETTER_REQ(UidType postId);
    bool Handler_EGS_GET_ITEM_FROM_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 안읽은 메일수 
    bool Handler_EGS_NEW_POST_LETTER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 새로운 편지 받음 알림
    bool Handler_EGS_RECEIVE_LETTER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 블랙리스트 등록
    bool Handler_EGS_NEW_POST_BLACK_LIST_REQ();
    bool Handler_EGS_NEW_POST_BLACK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 블랙리스트 삭제
    bool Handler_EGS_DEL_POST_BLACK_LIST_REQ();
    bool Handler_EGS_DEL_POST_BLACK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    // 블랙리스트 목록 가져오기
    bool Handler_EGS_GET_POST_BLACK_LIST_REQ();
    bool Handler_EGS_GET_POST_BLACK_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	// 편지알림 갱신
	bool Handler_EGS_RENEWAL_LETTER_REQ();
	bool Handler_EGS_RENEWAL_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:
    CKTDGUIDialogType		    m_pDLGQuery;
    CKTDGUIDialogType          m_pProcessDlg;

    wstring                 m_strNickName;
    wstring                 m_strTitle;
    CX2SlotItem*			m_pPostSlot;
    int                     m_nQuantity;
    int                     m_nED;
    int                     m_nFee; // 수수료

    CX2Inventory::SORT_TYPE m_NowInventorySortType;
    int						m_NowInvenSortTypePageNum;
    int						m_NowInvenSortTypePageMaxNum;

    CX2State*				m_pNowState;

    //CKTDGUIDialogType          m_pDlgPostBox;
    CKTDXTimer			    m_TimerPostBox;

    CKTDGUIDialogType			m_pDLGMailSend;
    CKTDGUIDialogType          m_pDlgMailReceive;
    CKTDGUIDialogType          m_pDlgMailListSlot;
    CKTDGUIDialogType          m_pDlgMailSetting;
    CKTDGUIDialogType          m_pDlgMailView;    
	CKTDGUIDialogType          m_pDlgMailViewCommon;	
	CKTDGUIDialogType          m_pDlgMailViewSystem;
#ifdef SERV_RELATIONSHIP_SYSTEM
	CKTDGUIDialogType          m_pDlgMailViewWedding;
#endif // SERV_RELATIONSHIP_SYSTEM




    CKTDGUIDialogType			m_pDLGMyTradeWindow;
    CKTDGUIDialogType          m_pDLGAddBlackList;

    CX2TalkBoxManager*		m_pTalkBoxManager;

    bool					m_bRegisteredTrade;
    bool                    m_bProcess;
    bool                    m_bMouseDown;
    
	//{{ 허상형 : [2010/8/25/] //	우편 수량 UI 개편
#ifdef POST_QUANTITY_NEW
	CKTDGUIDialogType			m_pDLGItemEnroll;
	UidType						m_EnrollItemUid;			// 첨부 아이템의 UidType
#endif	//	POST_QUANTITY_NEW
	//}} 허상형 : [2010/8/25/] //	우편 수량 UI 개편


private:
    bool m_bPostBox;
    bool m_bShow;
    bool m_bEnable;
    bool m_bAttachItem;
    int m_nTabIndex;
		
	UidType m_iItemUid;

    int m_nMaxPage;
    int m_nCurrentPage;
    int m_nMaxSlot;    
    MailListSlot m_listSlot[6];

	bool m_bSystemMail;
    bool m_bNewLetterisSystem;
    int m_CurrentState;

    std::vector<wstring> m_vecBList;
    bool m_bBuffBList;

    bool m_bAlarm;
    bool m_bNewLetterNotify;

    bool m_bCheckPitIn;
    int m_nUnReadDeleteMail;
    int m_nUnReadMail;
    int m_nTotalMail;

    int m_iNewLetter;   

    int m_nUnReadCommon;
    int m_nUnReadSystem;
    D3DXCOLOR m_nowColor;
    float  m_fBlinkTime;

    bool m_bSwitch;
    bool m_bEditTitle;    
	
	bool m_bReNewalLetter;
	bool m_bInit;

	//{{ 김상훈 : 2010.10.04  우편함 전체선택 UI 추가 
#ifdef POST_SELECT_ALL_MAILS
	bool m_bSelectedAll;
#endif POST_SELECT_ALL_MAILS
	//}} 김상훈 : 2010.10.04  우편함 전체선택 UI 추가 

#ifdef SERV_RELATIONSHIP_SYSTEM
	char m_cLetterListType;
	int m_nUnReadWedding;
	bool m_bNewLetterisWedding;

	bool IsSystemMail () { return ( m_cLetterListType == PLT_SYSTEM ) ? true : false; }
	bool IsCommonMail () { return ( m_cLetterListType == PLT_NORMAL ) ? true : false; }
	bool IsWeddingMail () { return ( m_cLetterListType == PLT_WEDDING ) ? true : false; }
#endif // SERV_RELATIONSHIP_SYSTEM

	//std::vector<wstring> m_vecMyUserName;
	std::vector<KPostSmallTitleInfo> m_vecUnReadMailTitle;
};
