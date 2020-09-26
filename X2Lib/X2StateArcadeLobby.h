#pragma once

//class CX2StateArcadeLobby : public CX2StateMenu
//{
//	public:
//
//		enum ARCADE_LOBBY_UI_CUSTOM_MSG
//		{
//			ALUCM_OPEN_CREATE_ROOM_DLG = 0,
//			ALUCM_OPEN_CREATE_ROOM_DLG_OK,
//			ALUCM_OPEN_CREATE_ROOM_DLG_CANCEL,
//
//			ALUCM_ROOM_BACK_PAGE,
//			ALUCM_ROOM_NEXT_PAGE,
//
//			ALUCM_QUICK_JOIN,
//			
//			ALUCM_CHANGE_CHANNEL,
//
//			ALUCM_ROOM_PASSWORD_CHECK_OK,
//			ALUCM_ROOM_PASSWORD_CHECK_CANCEL,
//
//			ALUCM_JOIN_ROOM,
//		};
//
//		struct SimpleRoom
//		{
//			UidType roomUid;
//			CKTDGUIDialogType pDialog;
//
//			SimpleRoom()
//			{
//				roomUid = 0;
//				pDialog = NULL;
//			}
//			~SimpleRoom()
//			{
//				roomUid = 0;
//				SAFE_DELETE_DIALOG( pDialog );
//			}
//		};
//
//	public:
//
//		CX2StateArcadeLobby(void);
//		virtual ~CX2StateArcadeLobby(void);
//
//		HRESULT OnFrameMove(double fTime, float fElapsedTime );
//
//
//		void OpenDLGJoinPasswordRoom( UidType roomUID );
//
//		bool Handler_EGS_CHANGE_ARCADE_ROOM_LIST_REQ( UINT channelID );
//		bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_REQ();
//		bool Handler_EGS_JOIN_ROOM_REQ( UidType roomUid, bool bIsObserver = false, wstring roomPassword = L"" );
//
//	protected:
//
//		bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//		bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool ClickMenuBackButton();
//		void SetShowStateDLG( bool bShow );
//
//		bool LastMsgByESC();
//		void LastMsgPopOk();
//
//		void InitArcadeLobbyUI();
//		void OpenCreateRoomDLG();
//		void CloseCreateRoomDLG();
//
//		bool Handler_EGS_ROOM_LIST_REQ( int pageNum );
//		bool Handler_EGS_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//		void ResetRoomListUI();
//
//		
//		bool Handler_EGS_JOIN_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_QUICK_JOIN_REQ();
//		bool Handler_EGS_QUICK_JOIN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_CREATE_ROOM_REQ();
//		bool Handler_EGS_CREATE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		
//		bool Handler_EGS_CHANGE_ARCADE_ROOM_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//		void ResetChannelRadioButtonUI( UINT selectedChannelID );
//
//		void CreateRoomPacket( KEGS_CREATE_ROOM_REQ& kEGS_CREATE_ROOM_REQ );
//
//		bool Handler_EGS_STATE_CHANGE_VILLAGE_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		bool Handler_EGS_GET_RANKING_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//		void ResetRankingUI();
//		void DrawPicChar( float fElapsedTime );
//		wstring GetRecordTime( int iPlayTime );
//		wstring GetRecordTimeEx( int iPlayTime );
//
//	protected:
//
//		CKTDGUIDialogType									m_pDLGArcadeLobbyBack;
//
//		CKTDGUIDialogType									m_pDLGArcadeCreateRoom;
//		bool											m_bCreateRoomObserver;
//
//		float											m_fRefreshTime;
//
//		bool											m_bEnableResetDLGRoomList;
//		int												m_iMaxNumOfRoom;
//		bool											m_bEnableRefreshRoomList;
//		vector<SimpleRoom*>								m_SimpleRoomList;
//		vector<CX2DungeonRoom::DungeonRoomSimpleInfo*>	m_RoomList;
//
//		int												m_NowPage;
//		int												m_EndPage;
//
//		UINT											m_TempChannelID;
//
//		CKTDGUIDialogType									m_pDLGJoinPasswordRoom;
//		UidType											m_JoinRoomUid;
//
//		CKTDGPicChar*									m_pPicCharTodayRankStage;
//		CKTDGPicChar*									m_pPicCharTodayRankTime;
//
//		CKTDGPicChar*									m_pPicCharTodayRankStageSmall;
//		CKTDGPicChar*									m_pPicCharTodayRankTimeSmall;
//
//};
