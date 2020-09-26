#pragma once

#ifdef ADDED_RELATIONSHIP_SYSTEM
namespace _CONST_CHATBOX_
{
	const int g_iMaxChatMode = 5 + 1;	// 채팅 전환 모드 ( 일반, 귓속말, 길드, 파티, 인연 ) 
										// 인연 추가하며 해당 상수 선언, 
										// 인연 탭이 결혼, 커플로 분리되어 1개 더 선언
}
#endif // ADDED_RELATIONSHIP_SYSTEM


class CX2ChatBox
{

public:
	enum PARTYMSGER_UI_MSG
	{
		PUM_CHAT_ENTER = 900,

		PUM_CHAT_MODE_ALL,
		PUM_CHAT_MODE_PARTY,
		PUM_CHAT_MODE_GUILD,

		PUM_USER_LIST_MODE_SQUARE,
		PUM_USER_LIST_MODE_PARTY,

		PUM_CHAT_WINDOW_MINIMIZE_MAXIMIZE, 
		PUM_CHAT_WINDOW_OPEN_OR_CLOSE,

		PUM_CHAT_WINDOW_SQUARE_USER_LIST,
		PUM_CHAT_WINDOW_PARTY_USER_LIST,
		PUM_CHAT_WINDOW_PARTY_CREATE_ROOM,

		PUM_USER_LIST_PREV_PAGE,
		PUM_USER_LIST_NEXT_PAGE,
		PUM_USER_LIST_REFRESH,		
		PUM_USER_LIST_CLOSE,

		PUM_USER_LIST_POPUP_MENU,

		PUM_PARTY_ACCEPT_INVITE,
		PUM_PARTY_REFUSE_INVITE,
        
		PUM_PARTY_LEAVE,

		PUM_ACCEPT_ROOM_INVITE,
		PUM_REFUSE_ROOM_INVITE,

		PUM_BAN_PARTY_MEMBER,

		PUM_PERSONAL_TRADE_ACCEPT,
		PUM_PERSONAL_TRADE_REFUSE,

		PUM_CHAT_WINDOW_SCROLL_TO_LATEST,

		PUM_CHAT_MODE_PERSONAL,
		PUM_CHAT_HELP,

		PUM_OPEN_PERSONAL_SHOP,


		PUM_POPUP_MENU_USER_INFO,
		PUM_POPUP_MENU_WHISPER,
		PUM_POPUP_MENU_ADD_FRIEND,
		PUM_POPUP_MENU_PARTY_INVITE,
		PUM_POPUP_MENU_TRADE_REQUEST,
		PUM_POPUP_MENU_VIEW_PERSONAL_SHOP,
		PUM_POPUP_MENU_ADD_APPRENTICE,
		PUM_POPUP_MENU_PLAY_TOGETHER,
		PUM_POPUP_MENU_ROOM_INVITE,
		PUM_POPUP_MENU_PARTY_BAN,
		PUM_POPUP_MENU_ROOM_BAN,
		PUM_POPUP_MENU_TUTOR_BAN,

		PUM_USER_LIST_MODE_TUTOR,

		PUM_BAN_TUTOR_UNIT,

		PUM_HELPER_ALL,
		PUM_HELPER_PARTY,
		PUM_HELPER_WHISPER,
		PUM_HELPER_REPLY,
		PUM_HELPER_EMOTION,
		PUM_HELPER_EMOTION_SELECT,
		PUM_HELP,
		PUM_HELPER_GUILD,
		PUM_POPUP_OPTION_MENU_ALL,
		PUM_POPUP_OPTION_MENU_PARTY,
		PUM_POPUP_OPTION_MENU_GUILD,
		PUM_POPUP_OPTION_MENU_WHISPER,
		PUM_POPUP_OPTION_MENU_CLOSE,
		PUM_OPTION_CHECKED_EXP,
		PUM_OPTION_CHECKED_ED,
		PUM_OPTION_CHECKED_SYSTEM,
		PUM_OPTION_CHECKED_NORMAL,
		PUM_OPTION_CHECKED_PARTY,
		PUM_OPTION_CHECKED_GUILD,
		PUM_OPTION_CHECKED_WHISPER,

// #ifdef ADDED_RELATIONSHIP_SYSTEM
		PUM_CHAT_MODE_RELATION,
		PUM_POPUP_OPTION_MENU_RELATION,
		PUM_OPTION_CHECKED_RELATION,
		PUM_HELPER_RELATION,
// #endif // ADDED_RELATIONSHIP_SYSTEM

	};

	enum CHAT_WINDOW_MODE
	{
		CWM_TOTAL,
		CWM_PARTY,
		CWM_GUILD,
		CWM_PERSONAL,

// #ifdef ADDED_RELATIONSHIP_SYSTEM
// GetPicture 부분을 이 enum 에서 불러오기 때문에
// Relation Mode 를 위로 올림, 문제가 될 시
// _CONST_CHATBOX_ namespace 에 상수로 선언
		CWM_RELATION_COUPLE,
		CWM_RELATION_MARRIED,
// #endif // ADDED_RELATIONSHIP_SYSTEM
		CWM_MEGAPHONE,
		CWM_CURRENT,

	};

	enum CHAT_TARGET
	{
		CT_ALL,
		CT_GUILD, 
		CT_PARTY,	
		CT_WHISPER,
// #ifdef ADDED_RELATIONSHIP_SYSTEM
		CT_RELATION,
// #endif // ADDED_RELATIONSHIP_SYSTEM
	};

	enum CHAT_OBTAIN_TYPE
	{
		COT_ED,
		COT_EXP,
	};

	enum CHAT_OPTION_INFO
	{
		COI_NONE = -1,
		COI_TOTAL_EXP,		//전체 탭
		COI_TOTAL_ED,		
		COI_TOTAL_SYSTEM,
		COI_TOTAL_NORMAL,
		COI_TOTAL_PARTY,
		COI_TOTAL_GUILD,
		COI_TOTAL_WHISPER,	
		COI_PARTY_EXP,		//파티 탭
		COI_PARTY_ED,		
		COI_PARTY_SYSTEM,
		COI_PARTY_PARTY,
		COI_PARTY_WHISPER,
		COI_GUILD_EXP,		//길드 탭
		COI_GUILD_ED,
		COI_GUILD_SYSTEM,
		COI_GUILD_GUILD,
		COI_GUILD_WHISPER,	
		COI_WHISPER_EXP,	//귓속말 탭
		COI_WHISPER_ED,
		COI_WHISPER_SYSTEM,
		COI_WHISPER_WHISPER,
		
// #ifdef ADDED_RELATIONSHIP_SYSTEM

		COI_TOTAL_RELATION,
		COI_PARTY_RELATION,
		COI_GUILD_RELATION,
		COI_WHISPER_RELATION,

		COI_RELATION_EXP,
		COI_RELATION_ED,
		COI_RELATION_SYSTEM,
		COI_RELATION_WHISPER,
		COI_RELATION_RELATION,
// #endif // ADDED_RELATIONSHIP_SYSTEM

		COI_END,
	};


//{{ 허상형 : [2009/7/14] // 나소드 메가폰 enum 값 정의
    enum NASOD_SCOPE_UI_MSG
	{
		NSUM_INSERT_MESSAGE_CLOSE = 32000,
		NSUM_INSERT_MESSAGE_OK,
		NSUM_USE_ITEM_CANCEL,
		NSUM_USE_ITEM_OK,
		NSUM_CHAT_MODE_MEGAPHONE,
		NSUM_CHAT_SELECT_MESSAGE,
		NSUM_CHAT_OPEN_MENU,
		NSUM_CHAT_CLOSE_MENU,	
		NSUM_MENU_WHISPER,
		NSUM_MENU_FRIEND,
		NSUM_MENU_CLEAR,
	};
//}} 허상형 : [2009/7/14] // 나소드 메가폰 enum 값 정의



	struct AntiChatSpam // 채팅 도배방지 관련
	{
	public:
		struct ChatLog
		{
			double	m_fTime;
			wstring	m_wstrMsg;

			ChatLog()
			{
				m_fTime = 0.f;
				m_wstrMsg = L"";
			}
			ChatLog( const ChatLog& t ) { *this = t; }    // copy constructor
			ChatLog& operator=( const ChatLog& t )       // assign operator
			{
				m_fTime = t.m_fTime;
				m_wstrMsg = t.m_wstrMsg;

				return *this;
			}
		};

	public:
		int m_iMaxSpamWarningCount;
		int m_iNowSpamWarningCount;
		
		float m_fSpamBlockingTimeLeft;

		float m_fMaxSpamBlockingTime1;
		float m_fMaxSpamBlockingTime2;
		float m_fMaxSpamBlockingTime3;

		std::deque<ChatLog>		m_qChatLog;
		std::map<wstring, int>	m_mapChatLogCount;

		bool m_bSpamMessage;

	public:
		AntiChatSpam();

		void OnFrameMove( double fTime, float fElapsedTime );
		void ClearLastChatLog();
		bool IsThisMessageSpam( wstring& wstrMsg );

		float GetSpamBlockingTimeLeft() const { return m_fSpamBlockingTimeLeft; }
		int GetNowSpamWarningCount() const { return m_iNowSpamWarningCount; }

	};



	struct QueuedChatLog
	{
		wstring		m_wstrMsg;
		wstring		m_wstrColor;
		D3DXCOLOR	m_coTextColor;
		bool		m_bEnableColor;
		bool		m_bIsMyself;
		float		m_fRemainTime;

		KEGS_CHAT_REQ::CHAT_PACKET_TYPE m_eChatPacketType; 

		QueuedChatLog()
		{
			m_wstrMsg = L"";
			m_eChatPacketType = KEGS_CHAT_REQ::CPT_TOTAL;
			m_coTextColor = D3DXCOLOR(1,1,1,1); 
			m_wstrColor = L""; 
			m_bEnableColor = true;
			m_bIsMyself = false;
			m_fRemainTime = 0.f;
		}
		QueuedChatLog( const QueuedChatLog& t ) { *this = t; }    // copy constructor
		QueuedChatLog& operator=( const QueuedChatLog& t )        // assign operator
		{
			m_wstrMsg = t.m_wstrMsg;
			m_wstrColor = t.m_wstrColor;
			m_coTextColor = t.m_coTextColor;
			m_bEnableColor = t.m_bEnableColor;
			m_bIsMyself = t.m_bIsMyself;
			m_fRemainTime = t.m_fRemainTime;
			m_eChatPacketType = t.m_eChatPacketType;

			return *this;
		}
	};
public:
	CX2ChatBox( CX2State* pNowState );
	virtual ~CX2ChatBox();

	void SetStage( CKTDXStage* pStage );

	// Show
	void HideChatWindow();
	void UnHideChatWindow();
	void CloseChatWindow();
	void OpenChatWindow();
	void HideChatWindowFrame();
	void UnHideChatWindowFrame();
	void UnHideChatEditBox();
	void HideChatEditBox();
	void OpenChatWindowButton();
	void CloseChatWindowButton();



	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	HRESULT OnFrameRender();


	// Handler
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool OnOpenWhisperChatMode();
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_CLICKED_PARTY_BUTTON();

	bool Handler_EGS_CHAT_REQ( KEGS_CHAT_REQ::CHAT_PACKET_TYPE eChatPacketType, CX2Room::ROOM_CHAT_TYPE eChatType, 
								UidType iTargetUnitUID, wstring& wstrMsg, const WCHAR* wszTargetUnitName = NULL );
	bool Handler_EGS_CHAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	//블랙리스트
	bool Handler_EGS_NEW_BLACKLIST_USER_REQ( const wstring& wstrNickName );
	bool Handler_EGS_NEW_BLACKLIST_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DEL_BLACKLIST_USER_REQ( UidType unitUID );
	bool Handler_EGS_DEL_BLACKLIST_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	// Implement
	wstring GetChatMessage( bool bClear = true );
	void AddChatLog( const WCHAR* wszMsg, KEGS_CHAT_REQ::CHAT_PACKET_TYPE eChatPacketType,  
						D3DXCOLOR coTextColor = D3DXCOLOR(1,1,1,1), wstring wstrColor = L"", bool bEnableColor = true, 
						bool bIsMyself = false, float fDelayTime = 0.f );
#ifdef CHAT_LOG_TO_TXT
	void AddChatLogToFile( const WCHAR* wszMsg );
#endif // CHAT_LOG_TO_TXT
	void AddChatLogObtain( const WCHAR* wszMsg , CHAT_OBTAIN_TYPE eObtainType );	


	int LineBreak( wstring& wstrText, int iTextWidth, const wstring& wstrTextColor, bool bEnableColor );

	//{{ 허상형 : [2009/7/20] //	메가폰용 LineBreak, 나눈 값들을 바로 전송한다.
	bool MegaLineBreakAdd( wstring wstrText, int iTextWidth, CKTDGUIListBox* pListBox );
	CKTDGUIListBox* GetCurrentChatbox();
	//}} 허상형 : [2009/7/20] //	메가폰용 LineBreak, 나눈 값들을 바로 전송한다.


	bool GetOpenChatWindow() const { return m_bOpenChatWindow; }
	bool GetChatEditBox() const { return m_pDLGChatWindowEditBox->GetShow(); }
	void ChangeChatWindowAndIMEEditBox( CHAT_WINDOW_MODE eMode );

//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	void ChangeChatWindow( CHAT_WINDOW_MODE eMode );				// 채팅 창(리스트, 탭버튼등)만 변경됨
	void ChangeIMEEditBox( CHAT_WINDOW_MODE eIMEEditWindowMode, bool bUseTabKey = false );	// 채팅 입력 창만 변경됨
	CHAT_WINDOW_MODE GetIMEEditWindowMode() const { return m_eIMEEditWindowMode; }
	void SetIMEEditWindowMode( CHAT_WINDOW_MODE eIMEEditWindowMode ) { m_eIMEEditWindowMode = eIMEEditWindowMode; }
	void ProcessChangeIMEEditWindowByTabKey();
	void UnHideChatLog( CHAT_WINDOW_MODE eChatWindowMode );

	void ChangeNativeAndEngKeyTexture();
	bool SetNativeAndEngTexture( bool bNative );
	wstring GetHistory();
	void PushMsgToHistory( const wstring& wstrMsg );
	void ChangeHistoryIndex( bool bUp );
	void SetHistoryIndex( int iIndex ) { m_iNowHistoryIndex = iIndex; }
	int GetHistoryIndex() const { return m_iNowHistoryIndex; }
	void ProcessInputMsgWithUpKey();
	void ProcessInputMsgWithDownKey();
#endif	CHAT_WINDOW_IMPROV
//}} kimhc // 2010.3.10 //	채팅창 개편

	CX2ChatBox::CHAT_WINDOW_MODE GetChatWindowMode() { return m_eChatWindowMode; }
	void SetChatWindowMode( CX2ChatBox::CHAT_WINDOW_MODE val ) { m_eChatWindowMode = val; }
	
	UidType GetWhisperChatTargetUID( const WCHAR* wszNickName, bool bParty = true );
	wstring GetUnitNickName( UidType iUnitUID );
	UidType GetUnitUIDByNickName( const WCHAR* wszNickName );

	void SetFocus();
	bool GetFocus();
	
	bool GetChatEnteredBefore() const { return m_bChatEnteredBefore; }
	void SetChatEnteredBefore(bool val) { m_bChatEnteredBefore = val; }

	bool GetHideChatWindowFrame() const { return m_bHideChatWindowFrame; }
	void SetHideChatWindowFrame(bool val) { m_bHideChatWindowFrame = val; }


	void SetChatDialogPos( float x, float y );
	void HideChatLog();
	void SetScrollBarEndPos();
	void ClearShortCutKeyProcess();
	bool SetChatEditText( const WCHAR* wszText );

	bool GetChatWindowEnlarged() const { return m_bChatWindowEnlarged; }
	void SetChatWindowEnlarged(bool bEnlarged );

	void SetPickedUser( UidType iPickedUserUID, wstring strPickedUserNickName );
	
	bool GetMouseOverChatBox();

	void SetChatBoxLayerUp(bool val);	
	bool GetOpenChatBoxButton() { return m_pDLGChatWindowButton->GetShow(); }

	void SetFocusChatEditBox(bool val) { m_bFocusChatEditBox = val; }
	bool GetFocusChatEditBox() { return m_bFocusChatEditBox; }
	
	void CX2ChatBox::ClearAllChatLog();	// kimhc // 채팅창에 있는 모든 글을 지움 // 2009-06-16

	void PopTalkBox( const KEGS_CHAT_NOT& kEvent_,	const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_ = D3DXCOLOR(0,0,0,1), const D3DXCOLOR& coOutColor_ = (DWORD)0x00000000, const D3DXCOLOR& coBackColor_ = (DWORD)0xffffffff );

#ifdef UNIT_EMOTION
	bool IsEmotionID( const wstring &wstrChatMsg ) const;
	CX2Unit::EMOTION_TYPE GetEmotionID( const wstring &wstrChatMsg ) const;
	wstring GetEmotionName( const wstring &wstrChatMsg ) const;
	bool GetEmotionIdAndName(CX2Unit::EMOTION_TYPE &eId, wstring &wstrName, const wstring &wstrChatMsg);

	void SendEmotionId( wstring wstrEmotionName);	// 이모션리스트에서 선택된 이모션을 보낸다.
	wstring GetEmotionSound(CX2Unit::EMOTION_TYPE eId) const;
	bool PlayEmotionSound( CX2Unit::UNIT_TYPE eUnitType, CX2Unit::EMOTION_TYPE eEmotionID, const D3DXVECTOR3& vPos, bool b3DSound );



#endif

	//{{ 허상형 : [2009/7/14] // 나소드 메가폰 관련 함수
	bool ShowNasodMessageDlg( bool bEnable );	//	나소드 메가폰 메시지 입력 Dialog 생성

	void SetLastUsedMegaphone(int itemTID) { m_iLastUsedMegaphoneTID = itemTID; }

	void SetItemSlot( CX2SlotItem* pItemSlot ) { m_pSelectedItemSlot = pItemSlot; }

	//	핸들러
	bool Handler_EGS_USE_MEGAPHONE_REQ();
	bool Handler_EGS_USE_MEGAPHONE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_MEGAPHONE_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//}} 허상형 : [2009/7/14] //

	bool Handler_EGS_CHAT_OPTION_INFO_WRITE_REQ(byte cIndex, bool bVal);
	bool Handler_EGS_CHAT_OPTION_INFO_WRITE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
	bool Handler_EGS_ANTI_ADDICTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_ANTI_ADDICTION_SYSTEM
	//}} 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템

	void ResetChatTabPopup(CX2ChatBox::CHAT_WINDOW_MODE eChatWinMode); //채팅 우클릭 팝업 메뉴 리셋
	void SetCheckBox( CX2ChatBox::CHAT_WINDOW_MODE eChatWinMode ) ;
	void SetCheckBoxData( CHAT_OPTION_INFO eChatOptionInfo ); 


	const bool GetDisplayObtainingED(CHAT_WINDOW_MODE eChatWindowMode);
	const bool GetDisplayObtainingEXP(CHAT_WINDOW_MODE eChatWindowMode);
	const bool GetDisplaySystem(CHAT_WINDOW_MODE eChatWindowMode);
	const bool GetDisplayNomal(CHAT_WINDOW_MODE eChatWindowMode);
	const bool GetDisplayParty(CHAT_WINDOW_MODE eChatWindowMode);
	const bool GetDisplayGuild(CHAT_WINDOW_MODE eChatWindowMode);
	const bool GetDisplayWhisper(CHAT_WINDOW_MODE eChatWindowMode);

#ifdef ADDED_RELATIONSHIP_SYSTEM
	const bool GetDisplayRelation(CHAT_WINDOW_MODE eChatWindowMode);
#endif // ADDED_RELATIONSHIP_SYSTEM

	void ListBoxAddItem(wstring ControlName, wstring chatContent, int addRow );

	void SetOptionData(std::map<byte,bool>mapChatOptionInfo);
#ifdef ADDED_RELATIONSHIP_SYSTEM
	bool ShowInviteCoupleDlg( bool bEnable );	//	초대 메시지 DLG 생성 및 보이기
#endif // ADDED_RELATIONSHIP_SYSTEM

private:
	void ClearSelectChatListBox();
	void PopupNasodScopeUserMenu(bool bPopup_);


protected:
	CKTDXStage*				m_pStage;

	// Dialogs
	CKTDGUIDialogType			m_pDLGChatWindowButton;
	CKTDGUIDialogType			m_pDLGChatWindow;
	CKTDGUIDialogType			m_pDLGChatWindowEditBox;
	CKTDGUIDialogType			m_pDLGChatWindowHelper;
	CKTDGUIDialogType			m_pDLGEmotionList;
	
	//CHAT_TARGET 			m_eChatTarget;	
	
	CHAT_WINDOW_MODE		m_eChatWindowMode;		

	//{{ kimhc // 2010.3.10 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
	CHAT_WINDOW_MODE		m_eIMEEditWindowMode;		// 채팅 입력창 모드
	bool					m_bPrevNativeLanguage;		// 이전의 한영키상태를 나타냄 // true 면 모국어
	std::deque<wstring>		m_deqHistoryForMsg;			// 이전에 입력했던 채팅을 기억하는 벡터
	int						m_iNowHistoryIndex;
	bool					m_bIsFirstOpenEditWindow;	// 처음으로 Enter를 눌러서 채팅입력창을 연 경우
#endif	CHAT_WINDOW_IMPROV
	//}} kimhc // 2010.3.10 //	채팅창 개편
	
	bool					m_bOpenChatWindow;
	bool					m_bChatWindowEnlarged;

	wstring					m_wstrPickedUserNickName;
	UidType					m_iPickedUserUID;

	float					m_fRefreshTime;
	bool					m_bChatEnteredBefore;

	AntiChatSpam			m_AntiChatSpam;

	wstring					m_wstrLastWhiperNickName;
	std::vector<wstring>	m_vecChatHelpString;

	bool					m_bIsJustOpenChatEditBox;
	bool					m_bHideChatWindowFrame;

	vector<QueuedChatLog>	m_vecQueuedChatLog;
	
	bool					m_bOpenChatWindowButton;
	bool					m_bFocusChatEditBox;	

	//{{ 허상형 : [2009/7/14] //	나소드 메가폰 관련 변수

    CKTDGUIDialogType			m_pDLGNasodScopeMessage;	//	나소드 메가폰 메시지 입력 Dialog
	CKTDGUIDialogType			m_pDLGNasodScopeMessageYesNo;	//	나소드 메가폰 전송 여부 확인 Dialog
	CKTDGUIDialogType			m_pDLGNasodScopeUserMenu;	//	나소드 메가폰 UI 메뉴
	CX2SlotItem*			m_pSelectedItemSlot;		//	선택된 나소드 아이템
	wstring					m_wstrSenderName;			//	메시지 보낸 케릭터 이름
	//{{ oasis907 : 김상윤 [2010.3.29] // 
	int						m_iLastUsedMegaphoneTID;
	//}}
	int						m_iMegaID;					//	메가폰 ID
	//}} 허상형 : [2009/7/14] //	나소드 메가폰 관련 변수

	vector<CKTDXDeviceSound*> m_vecDeviceSound;


	CKTDGUIDialogType			m_pDLGTabMenu;	//	채팅탭 우클릭 메뉴

	std::map<byte, bool>		m_mapChatOptionInfo;	
	CHAT_WINDOW_MODE			m_ePopupWindowMode;
#ifdef PLAY_EMOTION_BY_USER_SELECT
	float						m_fSendEmotionIdDelay;
#endif // PLAY_EMOTION_BY_USER_SELECT

};

