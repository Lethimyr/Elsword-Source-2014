#pragma once

class CX2StatePVPRoom : public CX2StateMenu
{
	public:
		friend class CX2StateAutoChanger;

		enum PVP_ROOM_UI_CUSTOM_MSG
		{
			PRUCM_RED_TEAM = 0,
			PRUCM_BLUE_TEAM,
			PRUCM_CHANGE_MAP,
			PRUCM_READY,
			PRUCM_START,
			PRUCM_INTRUDE,
			PRUCM_BACK_TO_PVP_LOBBY,
			PRUCM_CHANGE_ROOM_INFO,
			PRUCM_OPEN_OR_CLOSE_BUTTON,

			PRUCM_CHANGE_MAP_OK,
			PRUCM_CHANGE_MAP_CANCEL,
			PRUCM_CHANGE_MAP_SELECT,

			PRUCM_NEXT_KILL_NUM,
			PRUCM_PREV_KILL_NUM,

			PRUCM_NEXT_PLAY_TIME,
			PRUCM_PREV_PLAY_TIME,

			PRUCM_BAN_UNIT,
			PRUCM_BAN_UNIT_OK,
			PRUCM_WHISPER,
			PRUCM_USER_INFO,

			PRUCM_CHANGE_MAP_NEXT_TITLE,
			PRUCM_CHANGE_MAP_PREV_TITLE,

			PRUCM_CHANGE_MAP_SELECT_OVER,
			PRUCM_CHANGE_MAP_SELECT_OUT,
            
#ifdef DUNGEON_ITEM
			PRUCM_CHANGE_PVP_ITEMMODE,
#endif


			PRUCM_CHANGE_MAP_SELECT_FOR_DEBUG,		// 사내 테스트 용~

			PRUCM_CHECK_BASE_DEFENCE_MODE,			// 사내 테스트 용~
			PRUCM_CHECK_BOSS_COMBAT_MODE,			// 사내 테스트 용~

//#ifdef SERV_NEW_PVPROOM_PROCESS
			PRUCM_CHANGE_PVP_ROOM_PUBLIC,
			PRUCM_CHANGE_PVP_ROOM_PRIVATE,
			PRUCM_SET_PVP_ROOM_PASSWORD_OK,
			PRUCM_SET_PVP_ROOM_PASSWORD_CANCLE,
			PRUCM_SET_PVP_ROOM_PASSWORD_EXIT,
			PRUCM_CHANGE_PVP_ROOM_NAME_OK,
//#endif SERV_NEW_PVPROOM_PROCESS

		};

		enum PVP_ROOM_CHAT_MSG_TYPE
		{
			PRCMT_NONE = 0,
			PRCMT_SECRET,
		};

		struct PVP_ROOM_USER_BOX
		{
			UidType			unitUID;
			CKTDGUIDialogType	pDialog;
			CKTDGUIDialogType	pDialogUserBack;


			PVP_ROOM_USER_BOX()
			{
				unitUID = 0;
				pDialog = NULL;
				pDialogUserBack = NULL;
			}

			~PVP_ROOM_USER_BOX()
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

		struct MAP_INFO
		{
			int					m_WorldID;

			bool				m_bHot;
			bool				m_bNew;
			
			wstring				m_MapName;
			wstring				m_MapTexName;
			wstring				m_MapTexKeyName;

			wstring				m_MapNameTexName;
			wstring				m_MapNameTexKeyName;

			MAP_INFO()
			{
				m_WorldID	= 0;
				m_bHot		= false;
				m_bNew		= false;
			}
		};

		class CX2MapSelectWindow
		{
			public:

				CX2MapSelectWindow();
				~CX2MapSelectWindow();
				
				void OnFrameMove( double fTime, float fElapsedTime );
			
				void Open( int selectedMapWorldID );
				void Close();

				bool GetOpen();
				int	GetSelectedMapWorldID();
				void SetSelectMapByWorldID( int worldID );
				void SetSelectedMapWorldIDForce(const int val) { m_SelectedMapWorldID = val; }

				void AddMapInfo_LUA();
				MAP_INFO* GetMapInfo( int worldID );

				void ChangeSelectedMapDLGFirstSeq();

				int GetAllRandomMap();

			private:
				
				bool OpenScript( const WCHAR* pFileName );

				void ResetSelectedMapButton();

#ifdef FREE_SELECT_PVP_MAP

				void OpenPVPMapSelectWindowForDebug( bool bOpen );
				void UpdatePVPMapSelectWindowForDebug();

#endif FREE_SELECT_PVP_MAP


				vector< MAP_INFO* >			m_vecpMapInfo;

				CKTDGUIDialogType				m_pDLGBack;
				vector< CKTDGUIDialogType >	m_vecpDLGMap;
#ifdef FREE_SELECT_PVP_MAP
				CKTDGUIDialogType m_pDLGPVPMapSelectionWindowForDebug;
#endif FREE_SELECT_PVP_MAP


				CKTDGUIDialogType				m_pDLGSelected;

				bool	m_bOpen;
				int		m_SelectedMapWorldID;
		};

	public:
		CX2StatePVPRoom(void);
		virtual ~CX2StatePVPRoom(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


        bool EnterTutorial();


		bool Handler_EGS_CHANGE_SLOT_OPEN_REQ( int slotIndex );

		void OnBanUnit( int slotIndex = -1 );
		void OnChatModeOpen();

		virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
			const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

		void OpenMapSelectWindow( bool bOpen );

#ifdef SERVER_PVP_BASE_DEFENCE_TEST

		bool IsPVPGameOptionOpened();
		void OpenPVPGameOption( bool bOpen );
		void UpdatePVPGameOption();
#endif SERVER_PVP_BASE_DEFENCE_TEST

#ifdef FIX_INTRUSION_SKILL_BUG
		bool GetDisableSkillUIOpen(){return m_bDisableSkillUIOpen;}
#endif //FIX_INTRUSION_SKILL_BUG		

	protected:		
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//bool ClickMenuBackButton();
		void SetShowStateDLG( bool bShow );
		bool ToggleLobbyMenuWindow();
		//bool ToggleMyInfo( CX2CharacterRoom::CHARACTER_ROOM_STATE charRoomState );

		bool LastMsgByESC();
		void LastMsgPopOk();

		bool Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason = NetError::NOT_LEAVE_ROOM_REASON_00 );
		bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_LEAVE_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CHANGE_TEAM_REQ( CX2Room::TEAM_NUM teamNum );
		bool Handler_EGS_CHANGE_TEAM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_TEAM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CHANGE_READY_REQ();
		bool Handler_EGS_CHANGE_READY_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_READY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CHANGE_PITIN_REQ();
		bool Handler_EGS_CHANGE_PITIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CHANGE_MAP_REQ( int worldID );
		bool Handler_EGS_CHANGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_MAP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


		bool Handler_EGS_CHANGE_SLOT_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_SLOT_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_STATE_CHANGE_GAME_START_REQ();
		bool Handler_EGS_STATE_CHANGE_GAME_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_STATE_CHANGE_GAME_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_STATE_CHANGE_GAME_INTRUDE_REQ();
		bool Handler_EGS_STATE_CHANGE_GAME_INTRUDE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_PLAY_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_INTRUDE_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		//bool Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CHANGE_PLAY_TIME_LIMIT_REQ( float fPlayTime );
		bool Handler_EGS_CHANGE_PLAY_TIME_LIMIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_PLAY_TIME_LIMIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CHANGE_WINNING_NUM_KILL_REQ( int winKillNum );
		bool Handler_EGS_CHANGE_WINNING_NUM_KILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_WINNING_NUM_KILL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



		bool Handler_EGS_CHANGE_MOTION_REQ( KEGS_CHANGE_MOTION_REQ& kEGS_CHANGE_MOTION_REQ );	
		bool Handler_EGS_CHANGE_MOTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );
		bool Handler_EGS_CHANGE_MOTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );

		bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_TRADE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef DUNGEON_ITEM
		void Handler_EGS_CHANGE_PVP_ITEMMODE_REQ();
		bool Handler_EGS_CHANGE_PVP_ITEMMODE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_PVP_ITEMMODE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
		




#ifdef SERVER_PVP_BASE_DEFENCE_TEST
		bool Handler_EGS_CHANGE_PVP_GAME_OPTION_REQ( bool bBaseDefence, bool bBossCombat );
		bool Handler_EGS_CHANGE_PVP_GAME_OPTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_PVP_GAME_OPTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#endif SERVER_PVP_BASE_DEFENCE_TEST

#ifdef SERV_NEW_PVPROOM_PROCESS
		void Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_REQ( bool bPublic, std::wstring wstrPassword );
		bool Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_PVP_ROOM_PUBLIC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		void Handler_EGS_CHANGE_PVP_ROOM_NAME_REQ( std::wstring wstrRoomName );
		bool Handler_EGS_CHANGE_PVP_ROOM_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_CHANGE_PVP_ROOM_NAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_NEW_PVPROOM_PROCESS

		void ChangeMotion();

		bool UISetting();

		void UIFrameMove();
		void SetUIOXButton();
		void UnitViewerProcess( CX2UnitViewerUI* pViewer, int slotNum );

		bool SetRoomSlotInfo( std::vector<KRoomSlotInfo>& kPacket );
		bool SetUserBox( CX2Room::SlotData* pSlotData );




		virtual bool RegisterLuaBind();

		bool CheckUnitLevelByChannelList();


		void ToggleCashShop();

		void ResetNowMap();


		void InitPvpRoomUI();


		//virtual void SetSkillUseModeRadioButtonChecked( CX2GameOption::SKILL_USE_MODE eMode );
		
#ifdef SERV_NEW_PVPROOM_PROCESS
		void OpenSetRoomPasswordDLG();
#endif SERV_NEW_PVPROOM_PROCESS



	protected:
		CX2PVPRoom*				m_pCX2PVPRoom;		
		CKTDGUIDialogType		m_pDLGPVPRoomBack;
		CKTDGUIDialogType		m_pDLGPVPRoomFront;

//		map< UidType, CKTDGUIDialogType > m_mapPVPRoomUserBox;
		PVP_ROOM_USER_BOX*	m_PVPRoomUserBoxList[MAX_PVP_ROOM_USER_NUM];

		CKTDGUIDialogType		m_pDLGTalkBox;
		
	
		CKTDGUIDialogType		m_pDLGLoadingState;

		



		CX2TalkBoxManagerUI*	m_pTalkBoxMgr;
		
		

		D3DXVECTOR2			m_TimeLimit;	//x 가 최소 y가 최대
		D3DXVECTOR2			m_KillNumLimit;



		bool				m_bWaitTeamChangeAck;

		bool				m_bInitUIFail;

		CKTDGFontManager::CUKFont* m_pFontForIPAndPort;

		float				m_fMotionChangeTime;
		float				m_fMotionChangeRefreshTime;
		bool				m_bEnableMotionChange;

		int					m_iUserPopupSlotIdx;
		bool				m_bBaned;
		CKTDGUIDialogType		m_pDLGMsgBoxForBan;
		UidType				m_UnitUIDToBan;

		bool				m_bSendLeaveRoomReqByForce;

		float				m_fTimeLeftStartReq;
		float				m_fElapsedIdleTime;

		bool				m_bReserveCashShopOpen;

		CX2MapSelectWindow* m_pCX2MapSelectWindow;



		
#ifdef SERVER_PVP_BASE_DEFENCE_TEST
		CKTDGUIDialogType	m_pDLGPVPGameOption;

#endif SERVER_PVP_BASE_DEFENCE_TEST


		CKTDXTimer m_Timer_EGS_STATE_CHANGE_GAME_START_NOT;
		CKTDXTimer m_Timer_EGS_STATE_CHANGE_GAME_INTRUDE_NOT;

#ifdef FIX_INTRUSION_SKILL_BUG
		bool				m_bDisableSkillUIOpen;
#endif //FIX_INTRUSION_SKILL_BUG		

#ifdef SERV_NEW_PVPROOM_PROCESS
		CKTDGUIDialogType	m_pDLGSetPasswordRoom;
#endif SERV_NEW_PVPROOM_PROCESS
};
