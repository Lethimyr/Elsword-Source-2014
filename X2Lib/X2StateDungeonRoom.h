#pragma once

class CX2StateDungeonRoom : public CX2StateMenu
{
public:
	friend class CX2StateAutoChanger;

	enum DUNGEON_ROOM_UI_CUSTOM_MSG
	{
		DRUCM_READY,
		DRUCM_START,
		DRUCM_BACK_TO_LOCAL_MAP,	// 현재 사용안함
		DRUCM_CHANGE_ROOM_INFO,
		DRUCM_OPEN_OR_CLOSE_BUTTON,

		DRUCM_DIFFICULTY_LOWER,
		DRUCM_DIFFICULTY_HIGHER,	

		DRUCM_EVENT_SKIP,
		DRUCM_NO_EVENT_SKIP,

		DRUCM_BAN_UNIT,
		DRUCM_BAN_UNIT_OK,

		DRUCM_WHISPER,
		DRUCM_USER_INFO,
//#ifndef NOT_USE_DICE_ROLL
//		DRUCM_CHANGE_GET_ITEM_TYPE,
//#endif  //NOT_USE_DICE_ROLL

		DRUCM_CHALLANGE_NEXT_DIFFICULTY,
		DRUCM_CLOSE_DUNGEON_GUIDE,

		DRUCM_CHANGE_DIFFICULTY_OK,
		DRUCM_BAN_CHANGE_DIFFICULTY_OK,
	};

	enum DUNGEON_ROOM_CHAT_MSG_TYPE
	{
		DRCMT_NONE = 0,
		DRCMT_SECRET,
	};

	struct Dungeon_ROOM_USER_BOX
	{
		UidType			unitUID;
		CKTDGUIDialogType	pDialog;
		CKTDGUIDialogType	pDialogUserBack;

		Dungeon_ROOM_USER_BOX()
		{
			unitUID = 0;
			pDialog = NULL;
			pDialogUserBack = NULL;
		}

		~Dungeon_ROOM_USER_BOX()
		{
			SAFE_DELETE_DIALOG( pDialog );
			SAFE_DELETE_DIALOG( pDialogUserBack );
		}

		void SetShow( bool bShow )
		{
			if ( pDialog != NULL )
				pDialog->SetShowEnable( bShow, bShow );

			if ( pDialogUserBack != NULL )
				pDialogUserBack->SetShowEnable( bShow, bShow );
		}
	};



public:
	CX2StateDungeonRoom(void);
	virtual ~CX2StateDungeonRoom(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool			EnterTutorial();

	CX2DungeonRoom* GetCX2DungeonRoom() { return m_pCX2DungeonRoom; }
	void SetCX2DungeonRoom( CX2DungeonRoom* val ) { m_pCX2DungeonRoom = val; }

	void OnBanUnit( int slotIndex = -1 );
	void OnChatModeOpen();

protected:
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_LEAVE_ROOM_REQ();
	virtual bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	virtual bool Handler_EGS_STATE_CHANGE_LOCAL_MAP_REQ();
	bool Handler_EGS_STATE_CHANGE_LOCAL_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_SLOT_OPEN_REQ( wstring buttonName );
	bool Handler_EGS_CHANGE_SLOT_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_SLOT_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool Handler_EGS_STATE_CHANGE_GAME_START_REQ();
	bool Handler_EGS_STATE_CHANGE_GAME_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool Handler_EGS_STATE_CHANGE_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_READY_REQ();
	bool Handler_EGS_CHANGE_READY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_READY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_PITIN_REQ();
	bool Handler_EGS_CHANGE_PITIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_REQ( CX2Dungeon::DIFFICULTY_LEVEL difficulty );
	bool Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_DUNGEON_DIFFICULTY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool Handler_EGS_CHANGE_MOTION_REQ( KEGS_CHANGE_MOTION_REQ& kEGS_CHANGE_MOTION_REQ );	
	bool Handler_EGS_CHANGE_MOTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );
	bool Handler_EGS_CHANGE_MOTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );

	bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//#ifndef NOT_USE_DICE_ROLL
//	bool Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_REQ( CX2DungeonRoom::DUNGEON_GET_ITEM_TYPE getItemType );	
//	bool Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );
//	bool Handler_EGS_CHANGE_DUNGEON_GET_ITEM_TYPE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );
//#endif //NOT_USE_DICE_ROLL
	bool Handler_EGS_CHANGE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_RESTORE_SPIRIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ 2008. 10. 29  최육사
	bool Handler_EGS_DUNGEON_REQUIRE_ITEM_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//}}
	

	void ChangeMotion();

	void CheckChangeDifficulty( bool bLower );
	bool ChangeOpenOrCloseSlot( wstring buttonName );
	bool ChangeDifficulty( bool bLower );
	bool ChangeEventSkip( bool bEventSkip );
	bool ReadyGame();
	bool StartGame();

	void UnitViewerProcess( CX2UnitViewerUI* pViewer, int slotNum );
	void UISetting();
	void SetUIOXButton();
	virtual void UIFrameMove();
	
	bool SetUserBox( CX2Room::SlotData* pSlotData );

	//bool ClickMenuBackButton();
	virtual void SetShowStateDLG( bool bShow );
	bool ToggleLobbyMenuWindow();
	//bool ToggleMyInfo( CX2CharacterRoom::CHARACTER_ROOM_STATE charRoomState );

	bool LastMsgByESC();
	void LastMsgPopOk();


	bool IsInUnitReact( int iIdx, D3DXVECTOR2 mousePos );
	
	void OpenDungeonHelpPopUp();
	int LineBreak( wstring& wstrText, int iTextWidth, const wstring& wstrTextColor, bool bEnableColor, CKTDGFontManager::CUKFont* pFont );
	
	void ToggleCashShop();

	//virtual void SetSkillUseModeRadioButtonChecked( CX2GameOption::SKILL_USE_MODE eMode );


protected:
	CX2DungeonRoom*		m_pCX2DungeonRoom;	
	CKTDGUIDialogType		m_pDLGDungeonRoomBack;
	CKTDGUIDialogType		m_pDLGDungeonRoomFront;
	CKTDGUIDialogType		m_pDLGLoadingState;

	CKTDGUIDialogType		m_pDLGTalkBox;
	


	CX2TalkBoxManagerUI*	m_pTalkBoxMgr;

	bool					m_bTempEventSkip;

	Dungeon_ROOM_USER_BOX*	m_DungeonRoomUserBoxList[MAX_DUNGEON_ROOM_USER_NUM];


	CKTDGFontManager::CUKFont* m_pFontForIPAndPort;

	float				m_fMotionChangeTime;
	float				m_fMotionChangeRefreshTime;
	bool				m_bEnableMotionChange;

	int					m_iUserPopupSlotIdx;
	bool				m_bBaned;
	CKTDGUIDialogType		m_pDLGMsgBoxForBan;
	UidType				m_UnitUIDToBan;

	float				m_fTimeLeftStartReq;
	float				m_fElapsedIdleTime;

	bool				m_bCheckSendLeaveRoomByRequireLevel;

	CKTDGUIDialogType		m_pDLG_Dungeon_Help;

	bool				m_bReserveCashShopOpen;
	bool				m_bChangeDifficultyLower;
};
