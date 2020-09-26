#pragma once

namespace _CONST_COMMUNITY_UI_
{
	const	BYTE	g_byNumOfGuildMemberPerOnePage	= 8;		// 길드탭에서 한 페이지당 보이는 길드멤버수
}


namespace _CONST_RELATIONSHIP_UI_
{
	const int			g_iMaxRelationMemberNumber = 2;		// 한번에 인연 시스템에 들어갈 수 있는 인원은 2명 ( 본인 포함 ) 
	const int			g_iRelationSkillID = 4206; // 5200;
}

//#define WM_CHAT_NOTIFY WM_USER + 300
#ifdef ADDED_RELATIONSHIP_SYSTEM
#define USER_MENU_MAX 18 + 3 + 2		//{{ 허상형 : [2009/9/19] //	길드 메뉴 추가로 인한 MAX 값 변경
									    // 김종훈, 인연 시스템 추가로 인한 MAX 값 변경 ( 고백하기, 소환하기 )
#else // ADDED_RELATIONSHIP_SYSTEM
#define USER_MENU_MAX 18 + 3

#endif // USER_MENU_MAX

#ifndef NEW_MESSENGER
class CX2NexonMsgHandler;
#endif NEW_MESSENGER
class CX2Community
{
#ifndef NEW_MESSENGER
	friend CX2NexonMsgHandler;
#endif NEW_MESSENGER

	public:		

		enum X2_COMMUNITY_TAB
		{
			XMUT_NONE = 0,
			XMUT_FIELD,
			XMUT_FRIEND,
			XMUT_GUILD,
			XMUT_DISCIPLE,
			XMUT_REFEREE,	// 추천인
// #ifdef ADDED_RELATIONSHIP_SYSTEM
			XMUT_RELATION,  // 인연
// #endif // ADDED_RELATIONSHIP_SYSTEM
//#ifdef SERV_RECRUIT_EVENT_BASE
			XMUT_RECRUIT,	// 추천인이벤트
//#endif SERV_RECRUIT_EVENT_BASE
		};

		enum X2_COMMUNITY_CUSTOM_UI_MSG
		{
			XCCUM_CLOSE					= 26000,
			XCCUM_TAB_FIELD,
			XCCUM_TAB_FRIEND,
			XCCUM_TAB_GUILD,
			XCCUM_TAB_DISCIPLE,			
			XCCUM_MOUSE_L_UP,
			XCCUM_MOUSE_R_UP,
			XCCUM_USERMENU_CLOSE,
			XCCUM_USERMENU_DELETE,
			XCCUM_BAN_TUTOR_UNIT,
			XCCUM_USERMENU_CHARINFO,
			XCCUM_USER_LISTBOX_ITEM_LBUTTON,
			XCCUM_USER_LISTBOX_ITEM_RBUTTON,
			XCCUM_USER_LISTBOX_ITEM_LBUTTON_DBLCLK,
			XCCUM_FRIEND_GROUP_SORT,
			XCCUM_FRIEND_STATUS_SORT,
			XCCUM_FRIEND,
			XCCUM_PARTY,
			XCCUM_DISCIPLE,
			XCCUM_ADD_FRIEND_REQ_CONFIRM,
			XCCUM_ADD_FRIEND_REQ_NOT_CONFIRM,
			XCCUM_ADD_FRIEND_OPEN,
			XCCUM_ADD_FRIEND_CANCEL,
			XCCUM_ADD_FRIEND_REQ,
			XCCUM_CREATE_GROUP_OPEN,
			XCCUM_CREATE_GROUP_CLOSE,
			XCCUM_CREATE_GROUP_REQ,
			XCCUM_MGR_GROUP_MOVE_OPEN,
			XCCUM_MGR_GROUP_MOVE_OK,
			XCCUM_MGR_GROUP_MOVE_EXIT,
			XCCUM_UPDATE_FIELD_USER,
			XCCUM_FIELD_LIST_PREV,
			XCCUM_FIELD_LIST_NEXT,
			XCCUM_USERMENU_BLOCKING,
			XCCUM_MGR_CHANGE_GROUP_NAME_OPEN,
			XCCUM_MGR_CHANGE_GROUP_NAME_EXIT,
			XCCUM_MGR_CHANGE_GROUP_NAME_OK,
			XCCUM_USERMENU_WHISPER,
			XCCUM_MGR_CHAT_ENTER,
			XCCUM_DIALOG_CHATBOX_CLOSE,
			XCCUM_CHATBOX_CLOSE,
			XCCUM_CHATBOX_MAX,
			XCCUM_CHATBOX_MINI,
			XCCUM_CHATBOX_SIZE_TOGGLE,
			XCCUM_MGR_CHAT_SEND,
			XCCUM_TRADE,
			XCCUM_MARKET_SHOP,
			XCCUM_MGR_CHAT_FOCUS,
			XCCUM_PARTY_DEMAND,
			XCCUM_DISCIPLE_CUT,

			XCCUM_SEARCH_FRIEND_OPEN,
			XCCUM_SEARCH_FRIEND_CLOSE,
			XCCUM_SEARCH_FRIEND_OK,

			XCCUM_SEARCH_FRIEND_RESULT_CLOSE,
			XCCUM_SEARCH_FRIEND_RESULT_ADD_FRIEND,
			XCCUM_SEARCH_FRIEND_RESULT_PARTY_INVITE,
			XCCUM_SEARCH_FRIEND_RESULT_PARTY_REQUEST,
			XCCUM_SEARCH_FRIEND_RESULT_CHANNEL_MOVE,
			XCCUM_SEARCH_FRIEND_RESULT_PVP_INVITE,
			XCCUM_SEARCH_FRIEND_RESULT_PVP_PLAYTOGETHER,

			XCCUM_PVP_INVITE_ACCEPT,
			XCCUM_PVP_INVITE_REJECT,

			XCCUM_PVP_INVITE,
			XCCUM_PVP_TOGETHER,

			XCCUM_CHANGE_CHANNEL_ACCEPT,
			XCCUM_CHANGE_CHANNEL_CANCEL,

			//{{ kimhc // 2009-09-18 // 길드 MSG
			// 길드 탭 UI MSG
			XCCUM_PUSH_BUTTON_VIEW_LOACATION,			// 위치보기 버튼
			XCCUM_PUSH_BUTTON_VIEW_GUILD_MEMBER_GRADE,	// 등급보기 버튼
			XCCUM_PUSH_BUTTON_ADD_GUILD_MEMBER,
			XCCUM_PUSH_BUTTON_GUILD_INFO,
			XCCUM_GUILD_PREV_PAGE,
			XCCUM_GUILD_NEXT_PAGE,
			XCCUM_MOUSE_R_UP_ON_GUILD_MEMBER_GRADE,
			
			// 커뮤니티 UI 전체에서 쓰이는 MSG
			XCCUM_INVITE_GUILD_MEMBER,		// 길드 초대
			XCCUM_KICK_GUILD_MEMBER,		// 길드 추방

			XCCUM_KICK_GUILD_MEMBER_OK,		// 길드 추방 확인
			XCCUM_KICK_GUILD_MEMBER_CANCEL,	// 길드 추방 취소
			//}} kimhc // 2009-09-18 // 길드 MSG

			XCCUM_TAB_REFEREE,
			XCCUM_REFEREE_PREV_PAGE,
			XCCUM_REFEREE_NEXT_PAGE,

			XCCUM_USERMENU_WATCH,

			XCCUM_USERMENU_IMAGE,
			XCCUM_USERMENU_USER_BLOCK,
			XCCUM_USERMENU_MACHINE_BLOCK,
// #ifdef ADDED_RELATIONSHIP_SYSTEM
			XCCUM_TAB_RELATION,
			XCCUM_INVITE_COUPLE,
			XCCUM_SUMMON_COUPLE,
// #endif // ADDED_RELATIONSHIP_SYSTEM

//#ifdef SERV_RECRUIT_EVENT_BASE
			XCCUM_TAB_RECRUIT,
//#endif SERV_RECRUIT_EVENT_BASE
		};
		enum FRIEND_STATUS
		{
			FS_ONLINE = 0,
			FS_OFFLINE,
			FS_ETC,
			FS_GAME_CONNECTED,
		};

		//{{ kimhc // 2009-09-17 // 길드 UI
#ifdef	GUILD_MANAGEMENT
		enum X2_GUILD_VIEW_INFO
		{
			GVI_LOCATION		= 0,
			GVI_MEMBER_GRADE,
			GVI_GUILD_MEMBER_INFO,
		};
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-17 // 길드 UI

#ifdef NEW_MESSENGER
		struct MessengerUser
		{
			enum FRIEND_STATUS
			{
				FS_NORMAL,
				FS_REQUESTED,		// 누군가에게 친구 신청을 했을 때 이 상태로 임시등록 된다.
				FS_WAITING,			// 다른 사람에게서 자신에게 친구 신청이 왔을때 이 상태로 임시등록 된다.
				FS_BLOCKED,

				FS_NUM,
			};

			enum FRIEND_POSITION
			{
				FP_OFF_LINE,
				FP_ON_LINE,
			};

			UidType                             m_UnitUID;
			std::wstring                        m_NickName;				// 기존 코드와 변수명 통일
			int		                            m_uCategoryCode;		// 기존 코드와 이름을 통일해 주려고 일부러 m_u로 네이밍.
			FRIEND_STATUS                       m_eStatus;
			FRIEND_POSITION						m_ePosition;

//			int			m_Level;

			MessengerUser()
			{
				m_UnitUID = 0;
				m_NickName = L"";
				m_uCategoryCode = 0;
				m_eStatus = FS_NORMAL;
				m_ePosition = FP_OFF_LINE;
			}
			void Update( const KFriendInfo& friendInfo, bool &bRefresh );
			inline bool IsOnline(){	return m_ePosition != FP_OFF_LINE; }	// 온라인인가요?
			inline bool IsRequested(){ return m_eStatus == FS_REQUESTED; }	// 내가 친구신청을 했고 응답을 기다리고 있나요?
			inline bool IsWaiting(){ return m_eStatus == FS_WAITING; }		// 상대가 친구신청을 해 왔고 내가 응답을 해 줘야 하나요?
			inline bool IsBlocked(){ return m_eStatus == FS_BLOCKED; }		// 차단 먹였나요?
			inline bool IsNormal(){ return m_eStatus == FS_NORMAL; }
			inline bool IsBlockable(){ return m_eStatus == FS_NORMAL || m_eStatus == FS_BLOCKED; }	// 차단, 차단해제를 할 수 있는 상태인가요?
		};
		
		struct MessengerUserGroup
		{
			wstring		m_GroupName;
			int			m_uCategoryCode;		// 기존 코드와 이름을 통일해 주려고 일부러 i대신 u를 사용.
			bool		m_bIsOpen;
			vector< MessengerUser* >	m_vecUser;
		};
#else NEW_MESSENGER
		struct MessengerUser
		{

			NMFriendKey m_NMFriendKey;

			_UInt32_	m_uCategoryCode; // Update함수에서 리프레시 못해주고 있으므로 따로 업데이트 해줘야함

			UidType		m_UnitUID;
			wstring		m_NickName;
			BOOL		m_bLogin;
			wstring		m_Position;
			int			m_Level;

			_UInt32_			m_uFlag;
			NMUSERSTATUS		m_uStatus;

			bool		m_bBlocked;

			MessengerUser() : m_NickName(), m_Position()
			{
				ZeroMemory( &m_NMFriendKey, sizeof(NMFriendKey) );
				m_uCategoryCode = 0;
				m_UnitUID = 0;
				m_bLogin = FALSE;
				m_Level = 0;
				m_uFlag = 0;
				m_uStatus = kStatus_Unknown;
				m_bBlocked = false;
			}

			void Update( const CNMFriendInfo& friendInfo, bool &bRefresh );
		};

		struct MessengerUserGroup
		{
			wstring		m_GroupName;
			_UInt32_	m_uCategoryCode;
			bool		m_bIsOpen;
			vector< MessengerUser* >	m_vecUser;
		};
#endif NEW_MESSENGER

		struct MessengerUserList
		{
			vector< MessengerUserGroup* > m_vecUserGroup;
			vector< MessengerUser* > m_vecDefaultGroupUser;
		};

		struct FriendReqMsg
		{
#ifdef NEW_MESSENGER
			wstring		m_FromNickName;
			wstring		m_Msg;
			UidType		m_FromUnitUid;
#else
			_UInt32_	m_ToUnitNMSN;		//NexonMessengerSerialNum
			wstring		m_FromNickName;
			wstring		m_ToNickName;
			wstring		m_Msg;
			_UInt32_	m_uSerialNo;
#endif

			bool		m_bPopUpCheck;
			CKTDGUIDialogType	m_pDialog;

			FriendReqMsg()
			{
				m_bPopUpCheck = false;
				m_pDialog = NULL;
#ifndef NEW_MESSENGER	// 빌드 오류로 해외팀 추가
				m_ToUnitNMSN = 0;
				m_uSerialNo = 0;
#endif // NEW_MESSENGER
			};

			~FriendReqMsg()
			{
				if ( m_pDialog != NULL )
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDialog, NULL, false );
				m_pDialog = NULL;
			}
		};

		struct ChatSession
		{
			bool				m_bSettingComplete;
#ifdef NEW_MESSENGER
			UidType				m_UnitUid;
			wstring				m_wstrTargetNickName;
#else
			CNMCSChatSession*	m_pCNMCSChatSession;
#endif 
			CKTDGUIDialogType		m_pDialog;
			vector< wstring >	m_vecTalk;
			bool				m_bAlarmReceiveMessage;
			bool				m_bMaxWindow;
			bool				m_bHideWindow;
			bool				m_bShowWindow;
			
			ChatSession()
			{
#ifdef NEW_MESSENGER
				m_UnitUid	= -1;
#else
				m_pCNMCSChatSession = NULL;
#endif 
				m_pDialog = NULL;
				m_bSettingComplete = false;
				m_bAlarmReceiveMessage = false;
				m_bMaxWindow = true;
				m_bHideWindow = false;
				m_bShowWindow = false;						
			}

			~ChatSession()
			{
#ifndef NEW_MESSENGER
				if ( m_pCNMCSChatSession != NULL )
					m_pCNMCSChatSession->WantClose();

				SAFE_DELETE( m_pCNMCSChatSession );
#endif // #ifndef NEW_MESSENGER
				if ( m_pDialog != NULL )
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDialog, NULL, false );
				m_pDialog = NULL;
			}			
		};
		
		struct ChatHistory
		{
			wstring m_wstrNickName;
			wstring m_wstrOppositeNickName;

			vector<wstring> m_vecTalkHistory;
		};
		

		//{{ kimhc // 2009-09-18 // 길드 탭 UI의 유저리스트 와 관련된 구조체
#ifdef	GUILD_MANAGEMENT
		struct GuildUserListUIControls
		{
			CKTDGUIStatic*	m_pStaticNickName;
			CKTDGUIStatic*	m_pStaticLocationOrGradeOrTimeSpan;
			CKTDGUIStatic*	m_pStaticChannel;
			CKTDGUIButton*	m_pButtonNickName;
			CKTDGUIButton*	m_pButtonGrade;
#ifdef ELLIPSE_GLOBAL
			CKTDGUIButton*	m_pButtonToolTip;
#endif ELLIPSE_GLOBAL

			GuildUserListUIControls() :
			m_pStaticNickName( NULL ),
			m_pStaticChannel( NULL ),
			m_pStaticLocationOrGradeOrTimeSpan( NULL ),
			m_pButtonNickName( NULL ),
			m_pButtonGrade( NULL )
#ifdef ELLIPSE_GLOBAL
			, m_pButtonToolTip( NULL )
#endif ELLIPSE_GLOBAL
			{}
		};
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-18 // 길드 탭 UI의 유저리스트 control 구조체

		CX2Community(void);
		virtual ~CX2Community(void);

		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void Reset();

		void SetOpen( bool bCheck );
		bool GetOpen() { return m_bOpen; }  

		bool ChangeUserListTab( const WCHAR* pUserListTabName );

		void SetStage( CKTDXStage* pStage );

		void OnFrameMove( double fTime, float fElapsedTime );
		void OnFrameRender();

		//친구 관련
#ifndef NEW_MESSENGER
		void ResetUserList();
#endif
		bool OpenAddFriendWindow( bool bCheck );
		
		bool OpenSearchFriendWindow( bool bOpen );
		bool SearchFriendReq();
		bool OpenSearchFriendResultWindow( KEGS_SEARCH_UNIT_ACK kEvent );
		bool CloseSearchFriendResultWindow();


		bool FriendReq();
		void CheckFriendReq();
		bool ProcessFriendReqMsg( bool bConfirm );
#ifndef NEW_MESSENGER
		bool OnRequestNewFriendEvent( CNMRequestNewFriendEvent* pEvent );
		void OnFriendInfoChangedEvent( CNMFriendInfoChangedEvent* pEvent );	//제대로 처리 해줘야함 ㅡ,ㅡ;
		bool Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ( const WCHAR* pNickName );
		bool Handler_EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_KNM_TOGETHER_REQ( wstring nickName );
		bool Handler_EGS_KNM_TOGETHER_ACK( KEGS_KNM_TOGETHER_ACK& kEvent );
#endif
		void SetOpenFriendReqMsgBox( bool bOpen );

		//그룹 관련
		bool OpenCreateGroupWindow( bool bCheck );
		bool CreateGroup();
		bool ChangeGroupName();
		bool OpenGroupMoveWindow( bool bOpen );
		bool OpenGroupNameChangeWindow( bool bOpen );
		void MoveFriendGroup();

		//유저 리스트에서 마우스 왼쪽이나 오른쪽 버튼 눌렀을 때
		bool SelectUserListItem( CKTDGUIListBox::LIST_BOX_EVENT_MSG listBoxEventMsg );
		wstring GetSelectedUnitName();
		int GetSelectedUnitLevel();

		bool ProcessUserMenuCommand( const WCHAR* pCommandName );	//오른쪽 마우스 버튼 눌렀을 때 뜨는 메뉴에 있는 버튼 클릭 시
		bool ProcessGroupMenuCommand( const WCHAR* pCommandName );

		//채팅 세션 관련
		bool SetOpenChatSession( CKTDGUIDialogType pDialog, bool bOpen );
#ifdef NEW_MESSENGER
		bool SendChatMessage( const wstring& ChatMsg );					// 이 함수들은 NEW_MESSENGER에서 구현이 다르므로 이렇게 표시
		ChatSession* FindChatSession( UidType UnitUID );
		int	GetChatSessionIndex( UidType UnitUID );

		bool ReceiveChatMessage( UidType FromUnitUid, const wstring& ChatMsg, const wstring& wstrFromUnitName );
		bool SetOpenChatSession( UidType FromUnitUid, const wstring& wstrFromUnitName );
#else NEW_MESSENGER
		bool SendChatMessage( const WCHAR* pChatMsg );
		ChatSession* FindChatSession( _UInt32_ uSerialKey );
		int	GetChatSessionIndex( _UInt32_ uSerialKey );

		void OnGSSessionCreatedEvent( CNMGSSessionCreatedEvent* pEvent );
		void OnGSSessionInfoChangedEvent( CNMGSSessionInfoChangedEvent* pEvent );
	 	void OnGSSessionEstablishedEvent( CNMGSSessionEstablishedEvent* pEvent );
		void OnCSChatMessageReceivedEvent( CNMCSChatMessageReceivedEvent* pEvent );
		void OnGSSessionClosedEvent( CNMGSSessionClosedEvent* pEvent );
		void OnGSSessionFailedEvent( CNMGSSessionFailedEvent* pEvent );
#endif NEW_MESSENGER
		void ClearChatSessionList();
		void SetChatBoxShow(CX2Community::ChatSession* pChatSession, bool bShow);

		//기타 다른곳에서 필요한 헬퍼 함수
		void GetFriendNameList( vector< wstring >& vecFriendNameList );

#ifdef FIXED_RELATIONSHIP_SYSTEM
		void GetFriendNameListAndUID ( map <wstring, UidType> & mapFriendInfoList );
#endif // FIXED_RELATIONSHIP_SYSTEM

		static wstring ConvertOrgName( const WCHAR* pFullName );
		//수정해야함 나중에 서버군 이름 넣어줘야하는 식으로.. 아직은 솔레스 밖에 없으므로 일단 요로코롬 작업
		static wstring ConvertFullName( const WCHAR* pNickName );

		void UpdateTutorListWindow() { UpdateUserList(1); }
		
#ifdef SERV_ED_MONITORING_IN_GAME
		bool Handler_EGS_COMMUNITY_USER_LIST_MONITORING_MODE_REQ(int iPage);
#endif
		bool Handler_EGS_COMMUNITY_USER_LIST_REQ(int iPage);
		bool Handler_EGS_COMMUNITY_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		void DeleteFriendTab();
		void CutOnOffFriend();
		void SetFieldUserIcon(int i, bool bShow);
		void ClearUserIcon(int i);
		bool GetOverDialog();
		void SetHideWindow(bool val);
		bool GetHideWindow() { return m_bHideWindow; }

		bool GetOpenChatBox();

		// 레이어 처리
		void SetLayer(X2_DIALOG_LAYER layer);

		void AlarmReceiveMessage( const WCHAR* wszMessage );
		void ResetUpdateList() { m_bUpdateFieldUser = false; }

		void SetFriendTab(bool val);
//커뮤니티에서 길드를 분리하기 위해, 외부에서 탭을 수정할 수 있도록 함수 추가
		void SetFieldTab(bool val);
		void SetGuildTab(bool val);
		X2_DIALOG_LAYER	GetUserLayer() { return	m_eNowLayer; }

		bool GetNickNameByUID(wstring &wstrNickName, UidType iUid);
		void ResetHistory() { m_vecChatHistory.clear(); }

		void SetRefresh() { m_bRefresh = true; }
#ifndef NEW_MESSENGER
		void SyncFriendList();
#endif

		// 09.04.21 태완 : 대전장 초대 관련
		bool Handler_EGS_INVITE_PVP_ROOM_REQ( UidType UnitUid );
#ifdef SERV_COMMAND_LINE_INVITE_PVP_ROOM
		bool Handler_EGS_INVITE_PVP_ROOM_REQ( wstring wszNickName );
#endif  //SERV_COMMAND_LINE_INVITE_PVP_ROOM
		bool Handler_EGS_INVITE_PVP_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_INVITE_PVP_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifndef NEW_MESSENGER
		// 친구정보를 서버로 보낸다.
		void Handler_EGS_UPDATE_FRIEND_INFO_REQ(wstring &wstrNickName, bool bDelete = true);
		void Handler_EGS_UPDATE_FRIEND_INFO_REQ(CNMFriendInfo &friendInfo, bool bFirstSync = false);
		bool Handler_EGS_UPDATE_FRIEND_INFO_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif
#ifdef NEW_MESSENGER
		void SetFriendList( KMessengerInfo& refMessengerInfo );
		bool DeleteFriend( UidType UnitUid );

		// 친구 요청
		bool Handler_EGS_REQUEST_FRIEND_REQ( wstring wstrNickName, wstring wstrMessage );		
		bool Handler_EGS_REQUEST_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
		bool Handler_EGS_REQUEST_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 

		// 수락 대기 중인 캐릭터에 대해 친구 수락
		bool Handler_EGS_ACCEPT_FRIEND_REQ( UidType AcceptID );
		bool Handler_EGS_ACCEPT_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_ACCEPT_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		// 수락 대기 중인 캐릭터에 대해 친구 거절
		bool Handler_EGS_DENY_FRIEND_REQ( UidType DenyID );			
		bool Handler_EGS_DENY_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_DENY_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		// 친구 차단 요청
		bool Handler_EGS_BLOCK_FRIEND_REQ( UidType BlockID );
		bool Handler_EGS_BLOCK_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_BLOCK_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		// 친구 차단 해제
		bool Handler_EGS_UNBLOCK_FRIEND_REQ( UidType UnblockID );
		bool Handler_EGS_UNBLOCK_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_UNBLOCK_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		// 친구 삭제 요청
		bool Handler_EGS_DELETE_FRIEND_REQ( UidType DeleteID );
		bool Handler_EGS_DELETE_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_DELETE_FRIEND_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		// 친구 그룹 이동 요청
		bool Handler_EGS_MOVE_FRIEND_REQ( UidType UnitUid, int TargetGroupID );
		bool Handler_EGS_MOVE_FRIEND_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		// 그룹 생성 요청
		bool Handler_EGS_MAKE_FRIEND_GROUP_REQ( wstring wstrGroupName );
		bool Handler_EGS_MAKE_FRIEND_GROUP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		// 그룹 이름 바꾸기 요청
		bool Handler_EGS_RENAME_FRIEND_GROUP_REQ( int GroupID, wstring wstrGroupName );
		bool Handler_EGS_RENAME_FRIEND_GROUP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		// 그룹 삭제 요청
		bool Handler_EGS_DELETE_FRIEND_GROUP_REQ( int GroupId );
		bool Handler_EGS_DELETE_FRIEND_GROUP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		// 친구 업데이트
		bool Handler_EGS_FRIEND_POSITION_UPDATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

		//{{ kimhc // 2009-09-18 // 길드 탭에 출력할 정보를 얻어오기 위한 핸들러
#ifdef	GUILD_MANAGEMENT
		bool Handler_EGS_GET_GUILD_USER_LIST_REQ();
		bool Handler_EGS_GET_GUILD_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		UidType GetSelectedGuildMemberUID() {	return m_SelectedGuildMemberUID; }
		std::wstring GetGuildMemberNickName( UidType unitUID ) const;
		BYTE	GetGuildMemberGradeByUIDInThisList( UidType unitUID ) const;

		void SetTabByShortCutKey( X2_COMMUNITY_TAB eCommunityTab ); // 단축키 용도
		void OnGuildTabUIButton();
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-18 // 길드 탭에 출력할 정보를 얻어오기 위한 핸들러

#ifdef SERV_RECRUIT_EVENT_BASE
		void SetRecruitUI( bool bShow );
		void UpdateRecruitList();
		bool Handler_EGS_GET_RECRUIT_RECRUITER_LIST_REQ();
		bool Handler_EGS_GET_RECRUIT_RECRUITER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_RECRUIT_RECRUITER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_RECRUIT_EVENT_BASE

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
		void SetRefereeUI( bool bShow );
		void UpdateRefereeList();
		bool Handler_EGS_GET_RECOMMEND_USER_LIST_REQ( UINT uiNowPage );
		bool Handler_EGS_GET_RECOMMEND_USER_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_RECOMMEND_USER_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

		// 2011.05.05 lygan_조성욱 // 중국용 커뮤니티탭 유저 정보 보기
#ifdef SERV_USER_WATCH_NEW
		bool Handler_EGS_USER_WATCH_NEW_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_USER_WATCH_NEW

		// 해당 uid가 내친구인지 검사한다.
		bool IsMyFriend(UidType iUid);

		UidType GetFriendUID(wstring &nickName);
		UidType	GetPickedUserUID() {	return m_iPickedUserUID;	}
		X2_COMMUNITY_TAB GetOpenTab() const { return m_MessengerUserTab; }


#ifdef FIX_SEND_LIST_AT_LEAVE_GUILD // 적용날짜: 2013-03-28
		void RemoveGuildListUI()		/// 길드 UI 초기화 하는 함수 ( CX2GuildManager에서 사용하기 위해 )
		{
			m_vecUserUID.resize( 0 );
			m_vecGuildMemberInfo.resize( 0 );

			UpdateGuildTabList();
			UpdateGuildTabUI();
		}
#endif // FIX_SEND_LIST_AT_LEAVE_GUILD

#ifdef ADDED_RELATIONSHIP_SYSTEM
		void			SetRelationshipUI ( SEnum::RELATIONSHIP_TYPE eType_ );
		void			InitRelationUI();
		void			UpdateCoupleStateAndDurationDay ();
		void			UpdateCoupleNameAndChannel ();
		void			ResetRelationshipTabUI();
		bool			IsRelatioshipSkillID ( int iSkillID );
		bool			Handler_EGS_RELATIONSHIP_INFO_ACK  ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		void			ResetRelationDummyUnit ();
		void			AllocateRelationDummyUnit ( wstring wstrTargetNickName, char cClassType );
		void			UpdateRelationshipList ();
		void			SetRelationshipMemberIcon( int i, bool bShow, int iUnitClass, int iUnitLv  );
		void			SetRelationTab( const bool bOpen_ );
		const WCHAR*	GetRelationTabName( const SEnum::RELATIONSHIP_TYPE eType_ ) const;
		void			ClearUnitViewerUIInRelationshipTab ( void );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef ADDED_RELATIONSHIP_SYSTEM
		void InitUI();
#endif // ADDED_RELATIONSHIP_SYSTEM

	protected:
#ifndef ADDED_RELATIONSHIP_SYSTEM
		void InitUI();
#endif // ADDED_RELATIONSHIP_SYSTEM
		//void SetMode( X2_MESSENGER_MODE messengerMode );
		void SetTab( X2_COMMUNITY_TAB messengerTab );

		void ClearUserList();

		void ResetUserListUI();

		MessengerUser* GetMessengerUser( const WCHAR* pNickName );
#ifdef NEW_MESSENGER
		MessengerUser* GetMessengerUser( UidType UnitUID );
#else
		MessengerUser* GetMessengerUser( NMIDCode uIDCode );
#endif NEW_MESSENGER
		MessengerUserGroup* GetMgrUserGroup( const WCHAR* pGroupName );
		MessengerUserGroup* GetMgrUserGroup( int iGroupCode );
				
		void SetShowChatSessionList( bool bShow );
		bool AddChatMsg( ChatSession* pChatSession, const WCHAR* pChatMsg, bool bApplyColor = false, const WCHAR* chatColor  = L"#CFFFFFF" );
#ifndef NEW_MESSENGER
		bool DisconnectChatSession( _UInt32_ uSerialKey );
#endif
		
		wstring GetSelectedGroupName();
		wstring GetGroupName( const WCHAR* pName );
		bool FilteringGroupName( const WCHAR* pName );
#ifndef NEW_MESSENGER
		LPCTSTR GetStatusString( CNMFriendInfo* pFriendInfo );
#endif
		wstring GetStatusString( MessengerUser* pMessengerUser );
		FRIEND_STATUS GetStatusEnum( MessengerUser *PMessengerUser );

#ifndef NEW_MESSENGER
		void RefreshChatInfo( _UInt32_ uSerialKey );
#endif
		void SetOpenedChatSessionHistory();

		CKTDGUIListBox* GetListBox();

		void UpdateUserList(int iPage);

		void CheckButton(CKTDGUIButton* pButton);
		bool OpenUserMenu(int idummy = 0);		//유저 리스트에서 마우스 오른쪽 버튼 눌렀을 때 뜨는 메뉴
		bool OpenGroupMenu();
		
		bool Handler_EGS_KNM_INVITE_REQ( wstring nickName );
		
		bool TryTalk( CX2Community::MessengerUser* pMessengerUser );

		bool CheckSendMsgByChatbox();
		bool IsOnlySpaceString( const wstring wstrText );

		void UpdateFieldUserList(int iPage);
		void UpdateMarketUserList(int iPage);
		void UpdateDiscipleUserList();

		int FindChatHistory(wstring &wstrNickName, wstring &wstrOppositeNickName);
		void CreateChatHistory(wstring &wstrNickName, wstring &wstrOppositeNickName);
		void SetChatHistory(CX2Community::ChatSession* pChatSession);
		void GetChatHistory(CX2Community::ChatSession* pChatSession);


		//{{ kimhc // 2009-09-17 // 길드 UI
#ifdef	GUILD_MANAGEMENT
		void			InitGuildUI();
		void			ResetGuildTabUI();
		void			InitGuildTabListUI();
		void			UpdateGuildName();
		void			UpdateInfoTypeString();
		void			UpdateGuildPositionInfo();
		void			UpdateGuildGradeInfo();
		void			UpdateGuildTabList();
		void			UpdateGuildTabPageUI();
		void			UpdateNumOfGuildMembers();
		void			UpdateGuildTabUI();
		void			SetGuildMemberNickNameColor( const KGuildMemberInfo& kGuildMemberInfo, CKTDGUIStatic* pStaticNickName );
#ifdef EXTEND_SERVER_GROUP_MASK
		D3DXCOLOR		GetChannelColor( int iChannelID, int ServerID = 0 ) const;
#else // EXTEND_SERVER_GROUP_MASK
		D3DXCOLOR		GetChannelColor( int iChannelID, SERVER_GROUP_ID ServerID = SGI_SOLES ) const;
#endif EXTEND_SERVER_GROUP_MASK
		std::wstring	GetWStrAboutTimeFromLogOutToNow( const CTime& ctLogOutTime );
		//void			SetGuildMemberIcon( int i, bool bShow, const KGuildMemberPositionInfo& kGuildMemberPositionInfo );
		void			SetGuildMemberIcon( int i, bool bShow, int iUnitClass, int iUnitLv );
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-17 // 길드 UI

#ifdef SERV_ED_MONITORING_IN_GAME
		void			Handler_EGS_ED_MONITORING_BLOCK_REQ(UidType unituid, unsigned char ucType);
		bool			Handler_EGS_ED_MONITORING_BLOCK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

		CKTDXStage*				m_pStage;

		CKTDGUIDialogType			m_pDlgBack;
		CKTDGUIDialogType			m_pDlgField;
		CKTDGUIDialogType			m_pDlgFriend;
		CKTDGUIDialogType			m_pDlgGuild;
		CKTDGUIDialogType			m_pDlgDisciple;

#ifdef ADDED_RELATIONSHIP_SYSTEM
		CKTDGUIDialogType			m_pDlgRelation;
		CKTDGUIDialogType			m_pDlgRelationOnlyNotCouple;
		CKTDGUIDialogType			m_pDlgRelationOnlyPartnerShowing;


#endif // ADDED_RELATIONSHIP_SYSTEM

		//{{ kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능
#ifdef	VIEW_REFEREE_LIST
		CKTDGUIDialogType			m_pDlgReferee; // 추천인 관련 다이얼로그
		UINT						m_uiNowRefereePage;
		UINT						m_uiMaxRefereePage;
		std::vector< KRecommendUserInfo > m_vecRecommendUserInfo;
#endif	VIEW_REFEREE_LIST
		//}} kimhc // 2009-01-06 // 가이아 서버에만 추천인 리스트 보이는 기능

#ifdef	SERV_RECRUIT_EVENT_BASE
		CKTDGUIDialogType					m_pDlgRecruit;
		std::vector< KRecommendUserInfo >	m_vecRecruitUnitInfo;
		std::vector< KRecommendUserInfo >	m_vecRecruiterUnitInfo;
#endif	SERV_RECRUIT_EVENT_BASE

		CKTDGUIDialogType			m_pDLGCreateGroup;
		CKTDGUIDialogType			m_pDLGGroupMove;
		CKTDGUIDialogType			m_pDLGAddFriend;
		CKTDGUIDialogType			m_pDLGFriendReqWait;
		CKTDGUIDialogType			m_pDLGUserMenu;
		CKTDGUIDialogType			m_pDLGChangeGroupName;

		CKTDGUIDialogType			m_pDLGSearchFriend;
		CKTDGUIDialogType			m_pDLGSearchFriendResult;
		CKTDGUIDialogType			m_pDLGSearchFriendResultCharIcon;

		
		CKTDGUIDialogType			m_pFieldUserIcon[8];

		//CKTDGUIDialogType			m_pDLGMgr;
		//CKTDGUIDialogType			m_pDLGAddFriend;
		//CKTDGUIDialogType			m_pDLGCreateGroup;
		//CKTDGUIDialogType			m_pDLGUserMenu;
		//CKTDGUIDialogType			m_pDLGGroupMove;
		//CKTDGUIDialogType			m_pDLGGroupMenu;
		
		//CKTDGUIDialogType			m_pDLGFriendReqWait;

		bool					m_bOpen;

		//X2_MESSENGER_MODE		m_MessengerMode;
		X2_COMMUNITY_TAB		m_MessengerUserTab;

		MessengerUserList*		m_pMessengerUserList;

		FriendReqMsg*			m_pFriendReqMsg;

		set< wstring >			m_setOpenedGroupName;

		vector< ChatSession* >	m_vecChatSession;
		ChatSession*			m_pOpendChatSession;

		bool					m_bRequestChat;
#ifndef NEW_MESSENGER
		NMFriendKey				m_RequestedNMFriendKey;
#endif

		int						m_CurrentTrackPos;

		CX2TalkBoxManager*		m_pTalkBoxManager;

		wstring					m_MessageSenderNickName;
		bool					m_bReceiveMessage;

		int						m_MaxChatSessionByPage;
		int						m_NowPageNum;

		D3DXVECTOR2				m_ChatSessionOffsetPos;
		int						m_ChatSessionHeight;

		CKTDXDeviceSound*		m_pSoundAlarm;
		
		vector<UidType>			m_vecUserUID;
		vector<int>				m_vecUserLv;
		UidType					m_iPickedUserUID;
		int						m_iPickedUserIndex;

		CKTDGUIDialogType			m_pDLGQuery;

		int						m_iMaxFieldPage;
		int						m_iNowFieldPage;
		vector< KCommunityUserInfo > m_vecFieldUser;

		bool					m_bSortGroup;
		bool					m_bUpdateFieldUser;
		bool					m_bOpenGroupMenu;

		float					m_fFieldUserTimer;
		bool					m_bHideWindow;		

		X2_DIALOG_LAYER			m_eNowLayer;

		vector<ChatHistory>		m_vecChatHistory;

		bool					m_bRefresh;

		std::map<wstring, UidType> m_mapFriendUid;

		KConnectionUnitInfo		m_SearchUnitUnitInfo;
		bool					m_bSearchUnitUnitInfoValid;

		int						m_iChangeChannelId;

#ifdef NEW_MESSENGER
		std::vector< KFriendMessageInfo > m_vecFriendMessageInfo;
#endif

		//{{ kimhc // 2009-09-17 // 길드 UI
#ifdef	GUILD_MANAGEMENT
		X2_GUILD_VIEW_INFO		m_eNowGuildViewInfo;			// 현재 보는 정보가 위치인가? 등급인가?
		UINT					m_uiNowGuildPage;				// 현재 보고있는 길드 페이지
		UINT					m_uiMaxGuildPage;				
		UINT					m_uiNowNumberOfGuildMembersPlayingGame;	// 접속중인 길드원의 인원
		UINT					m_uiAllNumberOfGuildMembers;	// 길드원 전체의 인원
		UidType					m_SelectedGuildMemberUID;		// 선택된 길드 멤버에 대한 UID

		// 길드탭의 리스트와 관련된 컨트롤들
		GuildUserListUIControls	m_GuildUserListUIConstrols[_CONST_COMMUNITY_UI_::g_byNumOfGuildMemberPerOnePage];
		std::vector< KGuildMemberInfo >	m_vecGuildMemberInfo;
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-09-17 // 길드 UI

#ifdef FIX_INVITE_PVP_PLAYER /// 김태환
		vector<UidType>			m_vecInviteRoomUID;				/// 초대한 룸 아이디 
		float					m_fInviteCoolTime;				///	초대 룸 리스트 삭제 쿨타임
#endif // FIX_INVITE_PVP_PLAYER
		

#ifdef ADDED_RELATIONSHIP_SYSTEM

		struct RelationSkillUIControls
		{
			CKTDGUISlot*	m_pRelationSkillSlot;
			CKTDGUIStatic*	m_pStaticLevelString;
			CKTDGUIStatic*	m_pStaticActiveMark;
			CKTDGUIStatic*	m_pStaticLock;				// 봉인스킬 표시 자물쇠
			
			
			RelationSkillUIControls()
			{
				m_pRelationSkillSlot = NULL;
				m_pStaticLevelString = NULL;
				m_pStaticActiveMark = NULL;
				m_pStaticLock = NULL;
			}
		};


		struct RelationUIControls
		{
			CKTDGUIStatic*	m_pStaticNickName;
			CKTDGUIStatic*	m_pStaticLocationOrGradeOrTimeSpan;
			CKTDGUIStatic*	m_pStaticChannel;
			CKTDGUIButton*	m_pButtonNickName;
			
			RelationUIControls() :
				m_pStaticNickName( NULL ),
				m_pStaticChannel( NULL ),
				m_pStaticLocationOrGradeOrTimeSpan( NULL ),
				m_pButtonNickName( NULL )		
			{}
		};



		vector <CX2UnitViewerUI*>	m_vecUnitViewerUI;
		CX2Unit *					m_pRelationUnit;
		RelationUIControls			m_arrRelationUIControls[_CONST_RELATIONSHIP_UI_::g_iMaxRelationMemberNumber];
		
#endif // ADDED_RELATIONSHIP_SYSTEM
};

