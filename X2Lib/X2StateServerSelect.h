#pragma once

class CX2StateServerSelect : public CX2State
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
		SSSUCM_ATL_LIMIT_CHECK_OK,		// 오현빈 // 2012-07-04 // ATL1.0 유저의 게임 접속 제한 확인
		SSSUCM_EVENT_GUIDE_OK,			// 오현빈 // 2012-08-14 // 이벤트 가이드 확인
//#ifdef SERV_RECOMMEND_LIST_EVENT
		SUSUCM_RECOMMEND_NEXT,			//다음에 추천하기
//#endif //SERV_RECOMMEND_LIST_EVENT
		
//#ifdef SHOW_ONLY_ADVERTISEMENT
		SSSUCM_HIDE_UI_MOUSE_OVER,
		SSSUCM_HIDE_UI_MOUSE_OUT,
//#endif //SHOW_ONLY_ADVERTISEMENT

// #ifdef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
		SSSUCM_JUMPING_CHARACTER_OK,	// 점핑 캐릭터 안내 팝업창 OK 버튼 누름
// #endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터

// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		SSSUCM_UNIT_VIEWER_UP,
		SSSUCM_UNIT_VIEWER_DOWN,
		SSSUCM_UNIT_VIEWER_OVER,
		SUSUCM_SELECT_CHANNEL_LIST_STEP,
		SUSUCM_SECURITY_PAD,
		SUSUCM_SELECT_UNIT_LIST_STEP,
		SUSUCM_PREV_CHANNEL_LIST,	
		SUSUCM_NEXT_CHANNEL_LIST,
		SUSUCM_CHANNEL_BUTTON_UP,
		SUSUCM_SECURITY_PAD_CREATE,
// #endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
// #ifdef FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
		SUSUCM_RE_CONNECT_CHANNEL_SERVER_OK,
// #endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차


//#ifdef SERV_UNIT_WAIT_DELETE
		SUSUCM_FINAL_DELETE_UNIT = 48,
		SUSUCM_FINAL_DELETE_UNIT_CHECK = 49,
		SUSUCM_RESTORE_UNIT = 50,
		SUSUCM_RESTORE_UNIT_CHECK = 51,
//#endif SERV_UNIT_WAIT_DELETE

//#ifdef SIMPLE_BUG_FIX
		SSSUCM_DOUBLE_CONNECT = 52,
//#endif SIMPLE_BUG_FIX

//#ifdef NEW_OPTION_SECURITYPAD
		SSSUCM_SECURITYPAD	= 53,
//#endif //NEW_OPTION_SECURITYPAD

	};

#ifdef ADD_PLAY_SOUND //김창한
	enum SOUND_PLAY_TYPE
	{
		SPT_UNIT_SELECT = 0,
		SPT_SERVER_SELECT,
	};
#endif //ADD_PLAY_SOUND

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

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	struct UnitInfoAddData
	{
		double					m_dwLastLoginTime;
		SEnum::SERVER_GROUP_ID  m_eServerGroupID;


#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, 진입 구조 개편, 8차 수정 사항 
		// 유닛 생성 시 정렬하는 Key 값인 마지막 로그인 시간이 분 단위로 갱신됨에 따라 생기는 문제 수정
		// m_bIsCreatedUnit 변수가 True 일 경우 로그인 시간과는 상관 없이 무조건 앞으로 정렬한다.
		bool					m_bIsCreatedUnit;
		UnitInfoAddData( double dwLastLogin, SEnum::SERVER_GROUP_ID eServerGroupID, bool bIsCreatedUnit )
		{
			m_dwLastLoginTime	= dwLastLogin;
			m_eServerGroupID	= eServerGroupID;
			m_bIsCreatedUnit	= bIsCreatedUnit;
		}
		bool operator > ( const UnitInfoAddData & rhs) const
		{
			if ( true == m_bIsCreatedUnit && true == rhs.m_bIsCreatedUnit)
				return m_dwLastLoginTime > rhs.m_dwLastLoginTime;

			else if ( true == m_bIsCreatedUnit )	
				return true;
			
			else if ( true == rhs.m_bIsCreatedUnit )
				return false;

			return m_dwLastLoginTime > rhs.m_dwLastLoginTime;
			
		}

#else  // FIX_REFORM_ENTRY_POINT_8TH				
		UnitInfoAddData( double dwLastLogin, SEnum::SERVER_GROUP_ID eServerGroupID )
		{
			m_dwLastLoginTime	= dwLastLogin;
			m_eServerGroupID	= eServerGroupID;
		}
		bool operator > ( const UnitInfoAddData & rhs) const
		{
			return m_dwLastLoginTime > rhs.m_dwLastLoginTime;
		}
#endif // FIX_REFORM_ENTRY_POINT_8TH				
	};
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

public:
	CX2StateServerSelect(void);
	virtual ~CX2StateServerSelect(void);

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

#ifdef SERV_SECOND_SECURITY
	void SetEnableUnitSelect(bool _bVal){ m_bEnableUnitSelectBySecondSecurity = _bVal; }
#endif SERV_SECOND_SECURITY

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	bool Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ();
	bool Handler_EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE

#ifdef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, 진입 구조 개편, 7차 수정 사항 ( 보안 큐브 및 복귀 유저 보상 받을 수 있도록 수정 )
	bool Handler_EGS_SELECT_UNIT_REQ( UidType unitUID = -1, int iUnitLevel = -1 );
#endif  // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, 진입 구조 개편, 7차 수정 사항 ( 보안 큐브 및 복귀 유저 보상 받을 수 있도록 수정 )

protected:
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void CreateUnitButton();

#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
	bool ClearUnitButton( bool bIsClearUnitViewer_ = true );
	void CreateUnitButtonNew();
	void ChangeUnitButtonInfo ();
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
	bool ClearUnitButton();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편

	void ClearBeforeUnit( CX2Unit* pUnit );
	void UnitButtonUp( CX2Unit* pUnit );

	bool IsChannelSelectMode();

#ifndef FIX_REFORM_ENTRY_POINT_7TH		// kimjh, 진입 구조 개편, 7차 수정 사항 ( 보안 큐브 및 복귀 유저 보상 받을 수 있도록 수정 )
	bool Handler_EGS_SELECT_UNIT_REQ( UidType unitUID, int iUnitLevel );
#endif // FIX_REFORM_ENTRY_POINT_7TH	// kimjh, 진입 구조 개편, 7차 수정 사항 ( 보안 큐브 및 복귀 유저 보상 받을 수 있도록 수정 )

#ifdef SERV_SELECT_UNIT_PACKET_DIVISION		/// 페킷의 크기가 최대 허용 크기를 넘겨버려서, 분할
	bool Handler_EGS_SELECT_UNIT_1_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// 유닛의 정보, 인벤토리
	bool Handler_EGS_SELECT_UNIT_2_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// 퀘스트
	bool Handler_EGS_SELECT_UNIT_3_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// 타이틀, 스킬, 펫
	bool Handler_EGS_SELECT_UNIT_4_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// 접속 이벤트, 차단, 사제, 랭킹
	bool Handler_EGS_SELECT_UNIT_5_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// ETC, 1회성 이벤트
#endif SERV_SELECT_UNIT_PACKET_DIVISION
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

#ifdef SERV_UNIT_WAIT_DELETE
	bool Handler_EGS_FINAL_DELETE_UNIT_REQ();
	bool Handler_EGS_FINAL_DELETE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_RESTORE_UNIT_REQ();
	bool Handler_EGS_RESTORE_UNIT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_UNIT_WAIT_DELETE
	
#ifdef SERVER_GROUP_UI_ADVANCED
	bool Handler_ECH_GET_SERVERGROUP_LIST_REQ();
	bool Handler_ECH_GET_SERVERGROUP_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_ECH_GET_SERVERGROUP_LIST_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void UpdateServerGroupListUI();
	void CreateServerGroupButton();
	void ClearServerGroupButton();
#endif SERVER_GROUP_UI_ADVANCED	

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

	virtual bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_REQ();
	virtual bool Handler_EGS_STATE_CHANGE_SERVER_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool Handler_EGS_GET_SERVER_SET_DATA_REQ();
	//bool Handler_EGS_GET_SERVER_SET_DATA_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

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

#ifdef SERV_SECOND_SECURITY
	bool Handler_EGS_SECOND_SECURITY_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_SECOND_SECURITY

	bool Handler_EGS_KEYBOARD_MAPPING_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void SettingUserKeyboardMappingInfo( const KKeyboardMappingInfo& kKeyboardMappingInfo_ );
#ifdef SERV_KEY_MAPPING_INT
	void SettingUserGamePadMappingInfo( const KKeyboardMappingInfo& kKeyboardMappingInfo_ );
#endif //SERV_KEY_MAPPING_INT

	bool Handler_EGS_CHAT_OPTION_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	bool Handler_EGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif //SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
	bool Handler_EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
	bool Handler_EGS_JUMPING_CHARACTER_GUIDE_NOT ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// 점핑 캐릭터 팝업창 띄우라고 서버에서 주는 not
#endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터


	void UpdateChannelListUI();
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
	bool ConnectToChannelServer( bool bIsReversingConnect = false );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
	bool ConnectToChannelServer();
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
	
	void UnitSelectExit();

#ifdef MODIFY_ADVERTISEMENT // 적용날짜: 2013-03-28
	void AddMeshAdvertisement_Back();
	void AddMeshAdvertisement_Front();
#endif // MODIFY_ADVERTISEMENT


#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void HideAllUnitViewerUI ();
	void CreateServerSelectUnitViewerUI ( CX2Unit * pUnit_ );
	void SetFocusUnitButtonByIndex ( const int iIndex_ );
	void SetRightSideFlagColor ( D3DXCOLOR colorFlag );
	void SetShowServerStateButtonSet ( bool bIsShowUnitListState, bool bIsShowChannelListState ); 
	void SetBackgroundImageBySelectUnitType ( CX2Unit::UNIT_TYPE eUnitType_ );
	void SetShowSelectUnitInfo( CX2Unit * pUnit );
	void CreateChannelButton( bool bSelectTopChannelButton = false );
	bool Handler_EGS_CHARACTER_LIST_REQ ();
	bool Handler_EGS_CHARACTER_LIST_ACK ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_REQ ();
	bool Handler_EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ ();
	bool Handler_EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ENTRY_POINT_CHANGE_NICK_NAME_REQ ( UidType uid, const WCHAR* wszNickName, bool bCheckOnly, SEnum::SERVER_GROUP_ID eGroupID );

#ifdef ADD_PLAY_SOUND //김창한
	void PlaySoundSelect( CX2Unit::UNIT_TYPE eUnitType, SOUND_PLAY_TYPE eSoundPlayType );
#endif //ADD_PLAY_SOUND

	#ifdef FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG
	void InitChannelInfo();
	#endif // FIX_2ND_CHANNEL_TO_FIRST_ALIGN_BUG
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
#ifdef FIX_REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	void FindAndConnectLowerUserCountGameServer ( const vector< KChannelInfo > & vecChannelInfo );
#endif // FIX_REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// 김종훈, 진입구조 개편, 재접속 관련 알고리즘 수정
	int FindAndConnectMatchingLevelGameServer ( int iMatchingUnitLevel, const map<int, KChannelBonusInfo > & mapChannelBonusInfo, const vector< KChannelInfo > & vecChannelInfo );
#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// 김종훈, 진입구조 개편, 재접속 관련 알고리즘 수정

protected:

	void UnitHandling( double fTime, float fElapsedTime );

	void DownAnimFrameMove( double fTime, float fElapsedTime );
	void PlayDownAnim();

#ifdef SHOW_ONLY_ADVERTISEMENT
	void SetShowServerSelectUI( bool bVal_ );
#endif // SHOW_ONLY_ADVERTISEMENT


	virtual bool LastMsgByESC();
	virtual void LastMsgPopOk();

public:
	//{{ kimhc // 2009-12-15 // 서버군 추가
#ifdef	ADD_SERVER_GROUP
	bool OpenScriptServerGroupFile();			// 이전에 플레이 했던 서버군 읽기
#ifdef SERVER_GROUP_UI_ADVANCED
	void UpdateServerSelectButton( int iID );
#else
	void UpdateServerSelectButton( SERVER_GROUP_ID eID );
#endif SERVER_GROUP_UI_ADVANCED
#endif	ADD_SERVER_GROUP
	//}} kimhc // 2009-12-15 // 서버군 추가

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	void ClearChannelButton();
	void ChangeSelectUnitStandMeshInstance ( CX2Unit::UNIT_TYPE );
	void MoveToCharacterSelectUI();
	bool ChannelButtonUp( CKTDGUIButton * pButton );
	#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
	void ConnectToLowerUserCountGameServer ( bool bIsConnectAnotherServerGroup = false );
	#else  // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
	void ConnectToLowerUserCountGameServer ();
	#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
	
#ifdef FIX_REFORM_ENTRY_POINT_5TH // 김종훈, 진입 구조 개편 수정 5차
	void ProcessIntroMovieEnd ();		// 인트로 무비 ( 애드 추가 동영상 등 ) 이 끝났을 때의 처리
#endif // FIX_REFORM_ENTRY_POINT_5TH // 김종훈, 진입 구조 개편 수정 5차

#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, 진입 구조 개편, 8차 수정 사항 
										// ( 유닛 생성 시 정렬하는 Key 값인 마지막 로그인 시간이 분 단위로 갱신됨에 따라 생기는 문제 수정 )
	UidType GetUidCreatedUnit() const		{ return m_uidCreatedUnit; }
	void	SetUidCreatedUnit(UidType val)	{ m_uidCreatedUnit = val; }
#endif // FIX_REFORM_ENTRY_POINT_8TH				


public:
	static bool				m_sbSelectedServerSet;				// 이 변수를 기준으로 서버선택화면인지, 캐릭터 선택화면인지 구분할 수 있다.
#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	static bool				m_bCanNotConenctToBusyServer;				// 이 변수를 기준으로 서버선택화면인지, 캐릭터 선택화면인지 구분할 수 있다.
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh
	
protected:
	CKTDGUIDialogType			m_pDLGServerSelectBack;

	//{{ kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리
#ifdef	ADD_SERVER_GROUP
	CKTDGUIDialogType			m_pDLGChannelSelectBack;
	int							m_iChannelNum;				// 채널 갯수
	//{{ 허상형 : [2011/6/20] //        채널 선택 불편사항 수정
	bool                                               m_bSelectChannel;
	//}} 허상형 : [2011/6/20] //        채널 선택 불편사항 수정
#endif	ADD_SERVER_GROUP
	//}}  kimhc // 2009-12-15 // 서버군 추가 작업에 따른 채널 다이얼로그 분리

	CKTDGUIDialogType			m_pDLGCheckHack;

	vector<CKTDGUIDialogType>	m_vecUnitSlot;

	CKTDGUIDialogType			m_pDLGUnitSelectBack;
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

#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
	CKTDGUIDialogType			m_pDLGATLLimitCheckMsgBox;
#endif //SERV_CHANGE_NEXON_AUTH_ATL_LEVEL

#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	CKTDGUIDialogType			m_pDLGEventGuide;
#endif //SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE


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

#ifdef SERVER_GROUP_UI_ADVANCED
	CKTDGUIDialogType			m_pDLGServerGroupSelectBack;
	bool			m_bReceivedServerGroupList;
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_UNIT_WAIT_DELETE //2012.02.28 lygan_조성욱 // 버튼에 unitUID를 설정하여 캐릭터 삭제 할때 사용할 용도
	UidType			m_iSelectUnitUID;
	bool			m_bMaintainNowUnitButton;								// 삭제 신청 및 복구시 첫 번째 캐릭터 슬롯으로 포커스 잡는 것을 막는 용도
#endif SERV_UNIT_WAIT_DELETE

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
#ifdef SERV_SECOND_SECURITY
	bool			m_bEnableUnitSelectBySecondSecurity;
#endif SERV_SECOND_SECURITY

	bool			m_bRecivedKeyMap;
	
#ifdef MODIFY_ADVERTISEMENT // 적용날짜: 2013-03-28
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshAddAdvertisement_Back;
	CKTDGXMeshPlayer::CXMeshInstanceHandle	m_hMeshAddAdvertisement_Front;
#endif // MODIFY_ADVERTISEMENT

#ifdef SHOW_ONLY_ADVERTISEMENT
	CKTDGUIDialogType			m_pDLGShowOnlyAdvertise;
	bool						m_bHideServerSelectUI;
#endif // SHOW_ONLY_ADVERTISEMENT

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
	CKTDGUIDialogType			m_pDLGInfoCreateJumpingChar;		// 점핑 캐릭터 DLG
#endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터

#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	// 서버군, 최대 가능 갯수, 현재 갖고 있는 갯수

	// 게임 서버로 자동 접속해야 할 때 EGS_DISCONNECT_FOR_SERVER_SELECT_ACK 패킷을 보냈다는 플래그
	// 기본 값 false 이며 현재 게임 서버를 끊고 다른 게임 서버로 붙을 때 true 로 켜짐
#ifdef FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// 김종훈, 진입구조 개편, 재접속 관련 알고리즘 수정
	static bool						m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection;
#else  // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW		// 김종훈, 진입구조 개편, 재접속 관련 알고리즘 수정
	bool						m_bSend_EGS_DISCONNECT_FOR_SERVER_SELECT_ACK_For_Auto_Connection;
#endif // FIX_REFORM_ENTRY_POINT_RE_CONNECT_FLOW	// 김종훈, 진입구조 개편, 재접속 관련 알고리즘 수정
		
	// 게임 서버로 자동 접속해야 할 때 EGS_SELECT_UNIT_REQ 패킷을 보냈다는 플래그
	// 기본 값 false 이며 자동 접속 중 게임으로 접속할 때 true 로 켜짐
	bool						m_bSend_EGS_SELECT_UNIT_REQ_For_Auto_Connection;						// 


	// 유닛 뷰어, 진입 구조 개편으로 하나만 존재
	CX2UnitViewerUI*			m_pUnitViewerUI;	

	// 채널 페이지 추가로 인해 현재 페이지, 최대 페이지 추가
	int							m_iChannelNowPage;
	int							m_iChannelMaxPage;
	
	// EGS_GET_CREATE_UNIT_TODAY_COUNT_ACK 를 받았을 때, 생성 가능한 유닛을 설정한다.
	// 해당 숫자가 하루 생성 가능한 숫자를 넘어섰다면 더 이상 만들 수 없다 ( 캐릭터 생성 불가 )
	int							m_iCreateUnitTodayCount;		
	
	vector<CKTDGUIDialogType>	m_vecChannelSlot;			 // 채널 슬롯 DLG
	CKTDGUIDialogType			m_pDLGShowBeforeConnect;	//  광고 창을 보여주는 DLG
	std::vector<CKTDGXMeshPlayer::CXMeshInstanceHandle>		m_vecStandMeshInst;	// 캐릭터 밑 스탠드
	
	CX2EffectSet::Handle		m_hStandEffect;								// 캐릭터 이펙트
	float						m_fFadeTitleControlValue;					// 상단 알림말 Fade 변수, 0 ~ 2 로 Fade 되며 0 ~ 1 은 value = 투명도, 1 ~ 2 는 2 - value = 투명도 
	int							m_iReservedEntryPointServerChannelIndex;	// 게임 서버와의 접속을 끊었을 때, 다시 접속해야 할 게임 서버의 Index
	vector< KChannelInfo >		m_vecGaiaChannelInfo;						// 가이아 서버의 채널 정보
	vector< KChannelInfo >		m_vecSolesChannelInfo;						// 솔레스 서버의 채널 정보
	map< int, KChannelBonusInfo > m_mapGaiaChannelBonusInfo;				// 가이아 서버의 보너스 채널 정보 ( 레벨 제한 등.. )
	map< int, KChannelBonusInfo > m_mapSolesChannelBonusInfo;				// 솔레스 서버의 보너스 채널 정보 ( 레벨 제한 등.. )
	CKTDGUIButton*				m_pSelectChannelButton;						// 현재 선택한 채널 버튼
	bool						m_bIsAbledStateCreateUnitButton;			// 현재 캐릭터 생성 버튼을 클릭할 수 있는 상태인가?
#endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

#ifdef ADD_PLAY_SOUND //김창한
	CKTDXDeviceSound*			m_pSound;
#endif //ADD_PLAY_SOUND

#ifdef FIX_REFORM_ENTRY_POINT_2ND // 김종훈, 진입 구조 개편 수정 2차
	CKTDGUIDialogType			m_pDLGReConnectChannelServer;				// 광고 창을 보여주는 DLG
	bool						m_bReservedSecurityPad;						// 보안 패드창을 동영상 이후 띄워줄 수 있도록 한다.
	bool						m_bReservedHackingUserAgreeDlg;				// 해킹 유저 안내창을 동영상 이후 띄워줄 수 있도록 한다.
#endif // FIX_REFORM_ENTRY_POINT_2TH // 김종훈, 진입 구조 개편 수정 2차

#ifdef FIX_REFORM_ENTRY_POINT_4TH				// 김종훈, 진입 구조 개편 4차 ( 무한 대기 ) 수정
	bool						m_bWaitingEGS_CONNECT_REQ_ChannelButtonUp;			// 채널 버튼이 눌려서 게임 서버에 접속을 시도 할 때, 게임 서버에 접속 못했을 경우 예외 처리를 진행한다.
#endif // #ifdef FIX_REFORM_ENTRY_POINT_4TH		// 김종훈, 진입 구조 개편 4차 ( 무한 대기 ) 수정

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
private:
	void _SendSystemInfo();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef PLAY_PROMOTION_MOVIE //JHKang
	bool	m_bOrgSound;	/// 기본 효과음 설정 임시 저장
	bool	m_bOrgMusic;	/// 기본 배경음 설정 임시 저장
#endif //PLAY_PROMOTION_MOVIE

#ifdef FIX_REFORM_ENTRY_POINT_8TH		// kimjh, 진입 구조 개편, 8차 수정 사항 
										// ( 유닛 생성 시 정렬하는 Key 값인 마지막 로그인 시간이 분 단위로 갱신됨에 따라 생기는 문제 수정 )
	UidType	m_uidCreatedUnit;			// X2StateCreateUnit 에서 생성한 유닛
#endif // FIX_REFORM_ENTRY_POINT_8TH				

#ifdef FIX_REFORM_ENTRY_POINT_10TH		// kimjh, 캐릭터 리스트 못받으면 재접속 유도
	float				m_fElapsedTime_WaitingCharacterList;		// 캐릭터 정보를 받아오는대기 중인 시간 ( m_pDLGWaitingChannelList 가 존재 시, 시간 증가 )
	CKTDGUIDialogType	m_pDLGWaitingCharacterList;					// 캐릭터 정보를 받아오는 중 입니다.
#endif // FIX_REFORM_ENTRY_POINT_10TH	// kimjh, 캐릭터 리스트 못받으면 재접속 유도

#ifdef CLIENT_PORT_CHANGE_REQUEST
	int				m_iUDPRequestCount;
#endif //CLIENT_PORT_CHANGE_REQUEST
};
