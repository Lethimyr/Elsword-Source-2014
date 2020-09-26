#pragma once

class CX2StatePvPLobby : public CX2StateMenu
{
public:
	friend class CX2StateAutoChanger;

	struct SimpleRoom
	{
		UidType roomUid;
		CKTDGUIButton* pButton;
		//CKTDGUIButton* pObserverButton;
		CKTDGUIStatic* pStatic;

		SimpleRoom()
		{
			roomUid = 0;
			pButton = NULL;
			pStatic = NULL;
		}
		~SimpleRoom()
		{
			roomUid = 0;
			pButton = NULL;
			pStatic = NULL;
		}
	};

	enum UI_PVP_LOBBY_CUSTOM_MSG
	{
		UPLCM_EXIT = 12000,
		UPLCM_SELECT_CHANNEL,
		UPLCM_GAMETYPE_ALL,
		UPLCM_GAMETYPE_TEAM,
		UPLCM_GAMETYPE_TEAM_DEATH,
		UPLCM_GAMETYPE_SURVIVAL,
		UPLCM_QUICKSTART_TEAM,
		UPLCM_QUICKSTART_TEAM_DEATH,
		UPLCM_QUICKSTART_SURVIVAL,
		UPLCM_CREATE_ROOM,
		UPLCM_JOIN_ROOM,
		UPLCM_ROOMLIST_PREV_PAGE,
		UPLCM_ROOMLIST_NEXT_PAGE,

		UPLCM_CREATEROOMDLG_EXIT = 12100,
		UPLCM_CREATEROOMDLG_GAMETYPE_TEAM,
		UPLCM_CREATEROOMDLG_GAMETYPE_TEAM_DEATH,
		UPLCM_CREATEROOMDLG_GAMETYPE_SURVIVAL,
		UPLCM_CREATEROOMDLG_KILL_PLUS,
		UPLCM_CREATEROOMDLG_KILL_MINUS,
		UPLCM_CREATEROOMDLG_TIME_PLUS,
		UPLCM_CREATEROOMDLG_TIME_MINUS,
		UPLCM_CREATEROOMDLG_PRIVATE_CHECKED,
		UPLCM_CREATEROOMDLG_PRIVATE_UNCHECKED,
		
		//{{ kimhc // 2009-05-04, 17:10
		// SpecialUser 이상의 등급일 경우에 생기는 Observer 모드의 체크박스 Msg Enum 값
		UPLCM_CREATEROOMDLG_OBSERVER_CHECKED,
		UPLCM_CREATEROOMDLG_OBSERVER_UNCHECKED,
		//}} kimhc // 2009-05-04, 17:10

		UPLCM_CREATEROOMDLG_CREATE_CONFIRM,		

		UPLCM_PASSWORD_CANCLE,
		UPLCM_PASSWORD_OK,

		UPLCM_CREATEROOMDLG_ITEMMODE_PLUS,
		UPLCM_CREATEROOMDLG_ITEMMODE_MINUS,

		UPLCM_QUICKSTART_TEAM_ITEMMODE,
		UPLCM_QUICKSTART_TEAM_DEATH_ITEMMODE,
	};

	public:
		CX2StatePvPLobby(void);
		virtual ~CX2StatePvPLobby(void);

		HRESULT OnFrameMove(double fTime, float fElapsedTime );
		HRESULT OnFrameRender();

		bool EnterTutorial();

	protected:		
		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//bool ClickMenuBackButton();
		void SetShowStateDLG( bool bShow );

		bool Handler_EGS_ROOM_LIST_REQ( int pageNum );
		bool Handler_EGS_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_CREATE_ROOM_REQ();
		bool Handler_EGS_CREATE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_JOIN_ROOM_REQ( UidType roomUid, bool bIsObserver = false, wstring roomPassword = L"" );
		bool Handler_EGS_JOIN_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		

		bool Handler_EGS_CHANGE_PVP_ROOM_LIST_REQ( UINT channelID );
		bool Handler_EGS_CHANGE_PVP_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		//bool AutoJoinRoom( CX2PVPRoom::PVP_GAME_TYPE gameType );
		bool Handler_EGS_QUICK_JOIN_REQ( CX2PVPRoom::PVP_GAME_TYPE gameType, bool bItemMode = false );
		bool Handler_EGS_QUICK_JOIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef PVP_NO_PASSWORD_ERROR
		bool CreatePVPRoomPacket( KEGS_CREATE_ROOM_REQ& kEGS_CREATE_ROOM_REQ );
#else
		void CreatePVPRoomPacket( KEGS_CREATE_ROOM_REQ& kEGS_CREATE_ROOM_REQ );
#endif PVP_NO_PASSWORD_ERROR

        
	private:
		void ResetRoomListUI();
		//void SetPingLevel( int iSimpleRoomIndex, int iLevel );
		void OpenCreateRoomDLG();
		void SelectChannel( UINT channelID );

		void OpenDLGJoinPasswordRoom( UidType roomUID );
		void SelectGametypeOnCreateRoomDLG(CX2PVPRoom::PVP_GAME_TYPE GameType);

		void OpenDialog();

		bool LastMsgByESC();
		void LastMsgPopOk();

		bool CheckUnitLevelByChannelList();

		void SettingChannelButton();
		int	GetChannelID( KPVPChannelInfo::PVP_CHANNEL_CLASS ePVP_CHANNEL_CLASS );
		float GetPlayTimeForCreateRoom( CX2PVPRoom::PVP_GAME_TYPE eGameType );
		int GetKillCountForCreateRoom( CX2PVPRoom::PVP_GAME_TYPE eGameType );
		
		CKTDGUIDialogType							m_pDLGPVPLobbyBack;
		CKTDGUIDialogType							m_pDLGPvPLobby; 
		CKTDGUIDialogType							m_pDLGCreateRoom;

//		CKTDGUIDialogType							m_pDLGPVPLobbyFront;
//		CKTDGUIDialogType							m_pDLGCreatePVPRoom;		

		CKTDGUIDialogType							m_pDLGJoinPasswordRoom;
		CKTDGUIDialogType							m_pDLGLoadingState;

		float									m_fRefreshTime;
		float									m_fTime;

		int										m_MaxChannelID;

		int										m_NowPage;
		int										m_EndPage;
		bool									m_bEnableResetDLGRoomList;
		bool									m_bEnableRefreshRoomList;

		int										m_iMaxNumOfRoom;			// const, 화면에 보이는 최대 room list 수
		int										m_iRoomListGameType;		// room list의 game type

		vector<SimpleRoom*>						m_SimpleRoomList;
		vector<CX2PVPRoom::PVPRoomSimpleInfo*>	m_RoomList;


		UidType									m_JoinRoomUid;
		UINT									m_TempChannelID;

		// createroom 관련
		int										m_CreatePVPRoomType;
		bool									m_bIsPublicRoom;

		int										m_KillCountForCreateRoomIndex;
		int										m_PlayTimeForCreateRoomIndex;


		bool									m_bCreateRoomObserver;

		CX2PVPRoom::PVP_GAME_TYPE				m_QuickJoinPVPGameType;

		bool									m_bStateChangingVillageByForce;

		CX2UIUnit*								m_pUINPC;
		CX2TFieldNpcShop*						m_pDlgNpcMessage;

#ifdef DUNGEON_ITEM
		bool									m_bItemMode;
		bool									m_QuickJoinPVPItemMode;
#endif
};
