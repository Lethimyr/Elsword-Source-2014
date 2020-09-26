#pragma once
#ifdef ELSWORD_NEW_BEGINNING
class CX2StateBeginning : public CX2State
{
public:
	friend class CX2StateAutoChanger;
	enum STATE_SERVER_SELECT_UI_CUSTOM_MSG
	{
		SUSUCM_UNIT_BUTTON_OVER = 0,
		SUSUCM_UNIT_BUTTON_UP,

		SUSUCM_BACK_TO_LOGIN,
		SUSUCM_CREATE_UNIT,
		SUSUCM_DELETE_UNIT,
		SUSUCM_SELECT_UNIT,

		SUSUCM_UNIT_ANIMATION_DOWN,
		SUSUCM_UNIT_ANIMATION_UP,
		SUSUCM_UNIT_ANIMATION_OVER,

		SUSUCM_DELETE_UNIT_CHECK,
		SUSUCM_TUTORIAL,

		SUSUCM_RENAME_OK,
		SUSUCM_RENAME_CANCEL,

		SUSUCM_RENAME_CONFIRM_OK,
		SUSUCM_RENAME_CONFIRM_CANCEL,

		SUSUCM_RENAME_SUCCESS_OK,

		SUSUCM_START_TUTORIAL_OK,
		SUSUCM_START_TUTORIAL_CANCEL,

		SUSUCM_START_FIRST_DUNGEON_OK,
		SUSUCM_START_FIRST_DUNGEON_CANCEL,

		SUSUCM_RECOMMEND_OK,
		SUSUCM_RECOMMEND_CANCEL,

		SUSUCM_PREV,
		SUSUCM_NEXT,

		SSSUCM_SERVER_CONNECT0,			// 채널 리스트 버튼 클릭했을 때
		SSSUCM_SELECT_SERVER_GROUP,		// 서버군 버튼을 클릭했을 때
		SSSUCM_SERVER_CONNECT2,				// fix!! remove

		SSSUCM_HACK_USER_PROCESS_CHECK_OK,
		SSSUCM_HACK_USER_PROCESS_CHECK_CANCEL,

		SSSUCM_SERVER_SELECT_EXIT,
		SSSUCM_UNIT_SELECT_EXIT,
		
	};



	struct UnitButton
	{
		int				unitIndex;
		CKTDGUIButton*	pButton;

		UnitButton()
		{
			unitIndex	= 0;
			pButton		= NULL;
		}
	};
	


	//struct LessChannelIndex
	//{
	//	bool operator()( const KChannelInfo& lhs, const KChannelInfo& rhs )
	//	{
	//		if( lhs.m_iChannelID < rhs.m_iChannelID )
	//			return true;

	//		return false;
	//	}
	//};


	//{{ oasis907 : 김상윤 [2010.11.15] // 
	enum BEGINNING_GAME_STATE
	{
		BG_STATE_SERVER_SELECT,
		BG_STATE_CHANNEL_SELECT,
		BG_STATE_UNIT_SELECT,
		BG_STATE_UNIT_CREATE,
	};
	//}}


	

public:
	CX2StateBeginning(void);
	virtual ~CX2StateBeginning(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	


	void OnFrameMove_GameServerConnect( double fTime, float fElapsedTime );



	bool EnterTutorial();

	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();

	bool Handler_EGS_STATE_CHANGE_FIELD_REQ(bool bForceMove = false);
	bool Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//bool Handler_EGS_NEW_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_USER_LIST_IN_SECTOR_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void ClearUnitViewerList();
	CX2UnitViewerUI* GetUnitViewer( CX2Unit* pUnit );



protected:
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef ELSWORD_NEW_BEGINNING
	void CreateUnitButton( bool bLoadingAnim = false);
	bool SetShowUnitButton( bool bShow );
#else
	void CreateUnitButton();
#endif ELSWORD_NEW_BEGINNING
	bool ClearUnitButton();

	void ClearBeforeUnit( CX2Unit* pUnit );
	void UnitButtonUp( CX2Unit* pUnit );

	bool Handler_EGS_SELECT_UNIT_REQ( UidType unitUID, int iUnitLevel );
	bool Handler_EGS_SELECT_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할
#ifdef	PC_BANG_WORK
	bool Handler_EGS_SELECT_UNIT_INVENTORY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_GET_MY_INVENTORY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	PC_BANG_WORK
	//}} kimhc // 2010-01-04 // 기존의 EGS_SELECT_UNIT_ACK에 인벤정보가 포함되어 오던 것 분할

	bool Handler_EGS_GET_MY_MESSENGER_SN_REQ( UidType unitUID );
	bool Handler_EGS_GET_MY_MESSENGER_SN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DELETE_UNIT_REQ();
	bool Handler_EGS_DELETE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	

	bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	


	// 이름 바꾸기
	bool Handler_EGS_CHANGE_NICK_NAME_REQ( UidType uid, const WCHAR* wszNickName, bool bCheckOnly );
	bool Handler_EGS_CHANGE_NICK_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void OpenRenameMsgBox( bool bOpen );
	void SetRenameMsgBoxError( const wstring& wstrErrorMessage );


	bool Handler_EGS_RECOMMEND_USER_REQ( const WCHAR* wszNickName, bool bCancel = false );
	bool Handler_EGS_RECOMMEND_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void OpenRecommendMsgBox( bool bOpen );

	//bool Handler_EGS_GET_POST_LETTER_LIST_REQ();
	//bool Handler_EGS_GET_POST_LETTER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );




	bool Handler_EGS_SELECT_SERVER_SET_REQ();
	bool Handler_EGS_SELECT_SERVER_SET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ();
	bool Handler_EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_AGREE_HACK_USER_REQ();
	bool Handler_EGS_AGREE_HACK_USER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_CREATE_TUTORIAL_ROOM_REQ();
	bool Handler_EGS_CREATE_TUTORIAL_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	//{{ dmlee 2009.3.5 statelogin에 있던 것 이동
	bool Handler_EGS_CONNECT_REQ( const wstring& wstrGameServerIP, const int iGameServerPort );
	bool Handler_EGS_CONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_VERIFY_ACCOUNT_REQ();
	bool Handler_EGS_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_ENX_USER_LOGIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_KXPT_PORT_CHECK_REQ();
	bool Handler_KXPT_PORT_CHECK_ACK( const KXPT_PORT_CHECK_ACK& kXPT_PORT_CHECK_ACK );


	bool Handler_EGS_CURRENT_TIME_REQ();
	bool Handler_EGS_CURRENT_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool LoginErrorCheck();
	//bool LoginNexonKorea();

	//}} dmlee 2009.3.5 statelogin에 있던 것 이동



	bool Handler_ECH_GET_CHANNEL_LIST_REQ();
	bool Handler_ECH_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_ECH_GET_CHANNEL_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_REQ();
	bool Handler_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_ECH_DISCONNECT_REQ();
	bool Handler_ECH_DISCONNECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_ECH_VERIFY_ACCOUNT_REQ();
	bool Handler_ECH_VERIFY_ACCOUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	void UpdateChannelListUI();
	bool ConnectToChannelServer();
	void UnitSelectExit();




protected:

	void UnitHandling( double fTime, float fElapsedTime );

	void DownAnimFrameMove( double fTime, float fElapsedTime );
	void PlayDownAnim();



	virtual bool LastMsgByESC();
	virtual void LastMsgPopOk();

public:
	//{{ kimhc // 2009-12-15 // 서버군 추가
#ifdef	ADD_SERVER_GROUP
	//bool OpenScriptServerGroupFile();			// 이전에 플레이 했던 서버군 읽기
	void UpdateServerSelectButton( SERVER_GROUP_ID eID );
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // 서버군 추가

#ifdef ELSWORD_NEW_BEGINNING	
	//{{ oasis907 : 김상윤 [2010.11.11] // 
	void CreateGame();
	void SetShowEnableServerSelectControl(bool bEnable);
	void ReInitUnitSelect();
	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//}} oasis907 : 김상윤 [2010.11.11] // 
#endif ELSWORD_NEW_BEGINNING

	
public:
	static bool				m_sbSelectedServerSet;				// 이 변수를 기준으로 서버선택화면인지, 캐릭터 선택화면인지 구분할 수 있다.
	
protected:
	CKTDGUIDialogType			m_pDLGServerSelectBack;

	//{{ kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef	ADD_SERVER_GROUP
	CKTDGUIDialogType			m_pDLGChannelSelectBack;
	int							m_iChannelNum;				// 채널 갯수
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리

	CKTDGUIDialogType			m_pDLGCheckHack;

	vector<CKTDGUIDialogType>	m_vecUnitSlot;
#ifndef ELSWORD_NEW_BEGINNING
	CKTDGUIDialogType			m_pDLGUnitSelectBack;
#endif ELSWORD_NEW_BEGINNING
	CKTDGUIDialogType			m_pDLGUnitSelectFront;

	CKTDGUIDialogType			m_pDLGDeleteUnitCheck;
	CKTDGUIDialogType			m_pDLGLoadingState;

	CKTDGUIDialogType			m_pDLGRenameMsgBox;
	CKTDGUIDialogType			m_pDLGRenameConfirmMsgBox;
	CKTDGUIDialogType			m_pDLGRenameSuccessMsgBox;

	CKTDGUIDialogType			m_pDLGTutorialMsgBox;
	CKTDGUIDialogType			m_pDLGGoToFirstDungeon;

	CKTDGUIDialogType			m_pDLGRecommend;
	CKTDGUIDialogType			m_pDLGRecommendSuccess;



	vector<UnitButton*>		m_UnitButtonList;
	vector<CX2UnitViewerUI*> m_UnitViewerUIList;

	CX2Unit*				m_pSelectUnit;
	CKTDGUIButton*			m_pSelectButton;

	bool					m_bUnitClicked;
	float					m_fMouseSensitivity;

	float					m_fRemainRotY;
	bool					m_bRotateReturn;
	float					m_fRotSpeed;

	CKTDGUIButton*			m_pOveredButton;
	CKTDGUIStatic*			m_pOveredName;


	bool					m_bOneTimeInit;
	bool					m_bStateChangingToUnitCreate;

	bool					m_bPlayDownAnim;
	float					m_fDownAnimTime;

	int						m_MaxUnitNum;

	bool					m_bReserveEnterTutorial;

	bool					m_bCheckRecommend;
	bool					m_bCheckRecommendSuccess;

	int						m_NowPage;
	int						m_MaxPage;





	bool			m_bPortCheckWait;
	float			m_fPortCheckWaitTime;
	int				m_PortCheckRetryTime;

	bool			m_bENX_USER_LOGIN_NOT;
	bool			m_bEGS_VERIFY_ACCOUNT_ACK;
	bool			m_bIsGameBang;
	bool			m_bPortCheckReq;

	bool			m_bEGS_VERIFY_ACCOUNT_REQ;
	float			m_fEGS_VERIFY_ACCOUNT_ACK_WaitTime;

	bool			m_bReceivedChannelList;

	bool			m_bSentEGS_CONNECT_REQ;
	bool			m_bReceivedEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK;		

	bool			m_bReservedConnectionToChannelServer;					// 다른 채널 서버에 연결을 끊은 후에 새로운 채널 서버에 접속할지

	bool			m_bWaiting_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK;

#ifndef CHANGE_CHANNEL
	std::vector< KChannelInfo > m_vecChannelInfo;
#endif
	int				m_iTryConnectChannelID;				// 현재 접속시도중인 채널 ID



	bool			m_bJustChangedNickName;			// 닉네임을 바꾼후 EGS_SELECT_UNIT_ACK를 받으면 넥슨 메신저쪽에 닉네임이 바뀌었다는 이벤트를 보낸다



	UidType			m_NickNameChangeUnitUID;		// 닉네임 변경할 유닛 UID
	wstring			m_NickNameChangeUnitNickName;	// 닉네임 변경할 유닛의 새로운 닉네임


	bool			m_bRemoveUnitAfterReceiveNMSerialNum;	// 캐릭터 삭제시 넥슨 메신저 시리얼 넘버를 받은 후에 캐릭터 삭제 시도	

#ifdef ELSWORD_NEW_BEGINNING

	BEGINNING_GAME_STATE	m_eNowState;
	bool					m_bServerSelectDlgFadeIn;
	float					m_fServerSelectDlgFadeInElapsedTime;

	bool					m_bServerSelectWhiteIn;

	bool					m_bChannelSelectWhiteOut;
	float					m_fChannelSelectWhiteOutElapsedTime;


#endif ELSWORD_NEW_BEGINNING
};
#endif ELSWORD_NEW_BEGINNING
