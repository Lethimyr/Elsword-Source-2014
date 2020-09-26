#pragma once

class CX2PartyUI
: public CX2ItemSlotManager
{
public:
	enum PARTY_UI_CUSTOM_MSG
	{
		// 파티 리스트 UI
		PUCM_PARTY_LIST_DUNGEON_SELECT = 14000,		//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_LIST_PREV_PAGE,					//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_LIST_NEXT_PAGE,					//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_LIST_JOIN_PARTY,					//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_LIST_CLOSE_WINDOW,				//현재 사용하지 않는 기능//2012-10-23


		// 파티 리스트에 파티 슬롯들
		PUCM_PARTY_LIST_SLOT_CLICKED,				//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_LIST_SLOT_DOUBLE_CLICKED,		//현재 사용하지 않는 기능//2012-10-23


		// 파티 만들기 UI
		PUCM_PARTY_NAME,
		PUCM_PARTY_DIFFICULTY_LEFT,					//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_DIFFICULTY_RIGHT,				//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_MAX_SLOT_LEFT,					//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_MAX_SLOT_RIGHT,					//현재 사용하지 않는 기능//2012-10-23
		PUCM_PARTY_QUICK_PARTY,						// 던전 신청(매칭)
		PUCM_DUNGEON_PARTY_MATCH_CANCEL,			/// 던전파티 매칭 중인 것 취소,
		PUCM_PARTY_VIEW_PARTY_LIST,
		PUCM_PARTY_GAME_START,						/// 현재 인원으로 시작하기
		PUCM_PARTY_GAME_READY,						/// 현재 사용하지 않는 기능 //2012-10-23
		PUCM_PARTY_CLOSE_WINDOW,
		PUCM_PARTY_PUBLIC,
		PUCM_PARTY_LEAVE_PARTY,
		PUCM_PARTY_OPEN_LOCAL_MAP,

		PUCM_PARTY_LEAVE_DUNGEON_LOUNGE,
		PUCM_PARTY_GAME_RESTART,					// fix!! 사용하지 않음


		PUCM_PARTY_ITEM_GET_RANDOM,
		PUCM_PARTY_ITEM_GET_EACH,


		PUCM_PARTY_LEADER_ACCEPT_JOIN_REQUEST,
		PUCM_PARTY_LEADER_REJECT_JOIN_REQUEST,

		PUCM_ACCEPT_INVITE_REQUEST,					// 누군가의 파티 초대에 수락
		PUCM_REJECT_INVITE_REQUEST,					// 누군가의 파티 초대에 거절


		PUCM_PARTY_MEMBER_CLICKED,

		PUCM_PARTY_PLAY_SOLO,

		// 파티 리스트에 파티 슬롯들 우클릭
		PUCM_PARTY_LIST_SLOT_RCLICKED,

		PUCM_PARTY_LEVEL_LIMIT,
		PUCM_CREATE_LEVEL_UNLIMIT_PARTY_OK,
		PUCM_CREATE_LEVEL_UNLIMIT_PARTY_CANCEL,

		// 대전매칭 및 파티창 개편
		PUCM_SELECT_DUNGEON_MENU,
		PUCM_SELECT_PVP_MENU,
		PUCM_PVP_SETTING,
		PUCM_PVP_SETTING_CLOSE,
		PUCM_PVP_MAP_SETTING,
		PUCM_PVP_ETC_SETTING,
		PUCM_PVP_SETTING_SAVE_CLOSE,
		PUCM_PVP_MAP_SELECT,
		PUCM_PVP_MAP_CHECK,		
		PUCM_PVP_SETTING_1,
		PUCM_PVP_SETTING_2,
		PUCM_PVP_CREATE_PARTY,
		PUCM_PVP_START_MATCH,
		PUCM_PVP_CANCEL_MATCH,
		PUCM_PVP_CHECK_START_OK,						// 대전 매칭 완료 확인
		PUCM_PVP_CHECK_START_CANCEL,					// 대전 매칭 완료 취소
		PUCM_PVP_LEAVE_PARTY,
		PUCM_PVP_LEAVE_PARTY_OK,
		PUCM_PVP_LEAVE_PARTY_CANCEL,
		PUCM_PVP_CANCEL_MATCH_OK,
		PUCM_PVP_CANCEL_MATCH_CANCEL,

		// 필드 던전 파티 시작
		PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_OK,		/// 던전 시작 할 준비가 되었음
		PUCM_PARTY_READY_TO_DUNGEON_PLAY_REPLY_CANCEL,	/// 던전 시작 할 준비가 되지 않았음
		PUCM_REWARD_ITEM_MOUSE_OVER,					// 던전 보상 아이템 툴팁
		PUCM_REWARD_ITEM_MOUSE_OUT,						// 던전 보상 아이템 툴팁
		PUCM_DUNGEON_DIFFICULTY_CHECKED,				// 던전 난이도 변경
		PUCM_DUNGEON_DIFFICULTY_UNCHECKED,				// 던전 난이도 변경
	};

	enum GAME_TOOL_BUTTON_STATE
	{
		BUTTON_START,
		BUTTON_WAIT,
		BUTTON_CANCEL
	};
#ifdef SERV_PVP_NEW_SYSTEM
	struct PVP_MAP_INFO
	{
		int					m_WorldID;

		wstring				m_MapName;
		wstring				m_MapSize;
		wstring				m_MapDesc;

		wstring				m_MapTexName;
		wstring				m_MapTexKeyName;
		

		PVP_MAP_INFO()
		{
			m_WorldID	= 0;
		}
	};
#endif

public:
	CX2PartyUI( CKTDXStage* pNowStage );
	virtual ~CX2PartyUI(void);

public:
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	
	//2008.12.14 김태완 : UIManager 연결 작업
    void SetLayer(X2_DIALOG_LAYER layer);
	bool GetShow();


	void OnChangeState();	 
	void OnJoinParty();
	void UpdatePartyMenu();

	void OpenPartyDLG( bool bOpen );
	void UpdatePartyDLG( bool bRenamePartyTitle = false );
	void CreatePartyDlg();

	void OpenLocalMapDlg();


	bool IsGameStartable( CX2LocationManager::LOCAL_MAP_ID eCurrLocalMapID, const CX2Dungeon::DungeonData* pDungeonData, bool bHaveParty, bool bPartyLeader );

	//void OpenPartyMemberDLG( bool bOpen );
	//void UpdatePartyMemberDLG();

	void OpenPartyListMemberPreviewDLG( bool bOpen, int iPartyIndex = 0 );
	void UpdatePartyListMemberPreviewDLG( int iPartyIndex );


#ifdef HENIR_TEST
	void OpenPartyFeverDLG( bool bOpen );
	void UpdatePartyFeverDLG();
#endif HENIR_TEST



	// 09.03.06 김태완
	void EnableDialog(bool bEnable);

	void SetStage(CKTDXStage* val);

	void SetPartyListPageNumber( int iCurr, int iMax )
	{
		m_iPartyListPageNumber = iCurr;
		m_iMaxPartyListPageNumber = iMax;
	}

#ifndef REFORM_NOVICE_GUIDE
	void SetNoviceGuideStep(int iStep);
#endif //REFORM_NOVICE_GUIDE
	bool GetShowLocalMap();

	// 09.04.01 태완
	void PlayWarningAllReady();
	void StopWarningAllReady();
	bool IsReadyPossible();			// 현재 상황에 레디를 해도 되는지
	
#ifdef SERV_PVP_NEW_SYSTEM
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void InitPvpSetting();
	void OpenPartyMenu(bool bOpen);
	void OpenPvpSetting(bool bOpen, bool bMap = true);	
	bool GetShowPartyMenu() 
	{ 
		if( m_pDLGPartyMenu != NULL )
			return m_pDLGPartyMenu->GetShow(); 
		return false;
	}

	bool IsMouseOverPartyUI()
	{
		if( m_pDLGParty != NULL && m_pDLGParty->GetShow() == true )
			return m_pDLGParty->GetIsMouseOver();
		else if( m_pDLGPartyPVP != NULL && m_pDLGPartyPVP->GetShow() == true )
			return m_pDLGPartyPVP->GetIsMouseOver();
		else if( m_pDlgPartyPvpMapSetting != NULL && m_pDlgPartyPvpMapSetting->GetShow() == true )
			return m_pDlgPartyPvpMapSetting->GetIsMouseOver();

		return false;
	}

	void ResetePvpMapSetting();
	void SavePvpMapSetting();
	void LoadPvpMap();
	void LoadPvpMapUI();
	void AddPvpMapList(CKTDGUIContolList *pControlList, int row, int pvpMapInfoIndex);

	bool GetDungeonPartyMenu() { return m_bPartyDungeon; }
	bool GetPvpPartyMenu() { return m_bPartyPVP; }
	void SetDungeonPartyMenu(bool bVal) { m_bPartyDungeon = bVal; m_bPartyPVP = !bVal; }
	void SetPvpPartyMenu(bool bVal) { m_bPartyPVP = bVal; m_bPartyDungeon = !bVal; }
		
	void UpdateMapInfo(int iMapIndex);
	void UpdatePvpPartyDlg();
		
	void CheckPvpStart();
	void GetSelectedPvpMap(std::set<short> &selectedMap);

	char GetPvpMatchMode();	
	char GetPvpMatchPlayerNum();	
	void SetPvpMatchPlayerNum(int iVal);
	void ShowPvpModeDesc();

	void SetEstimatedTime(int iVal) { m_iEstimatedTime = iVal; m_fWaitPvpTime = 0.f; }
#endif


#ifdef FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	CKTDGUIDialogType CreateRewardItemInfoDlg (  const WCHAR * wDlgName );										// RewardItemDlg 를 생성
	void ChangeRewardItemInfoDlgLayer ( bool bOpen, int iLayerToChange );	// 해당 Dlg 의 Layer 를 변경
	void SetShowEnableRewardItemInfoDlg ( bool bOpenMajor, bool bOpenMinor, bool bEnableFlag );			// NullCheck 이후, bEnableFlag 가 true 이면 SetShowEnable 호출, false 면 SetShow 호출
#endif	// FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION


	void StartToWaitForDungeonPartyMatching();
	void CancelToWaitForDungeonPartyMatching();
	void StartToWaitForPvpMatching();
	void CancelToWaitForPvpMatching();
	void OpenRewardItemInfoPopup( bool bOpen, D3DXVECTOR2 vPos, int iItemID);



	void SetBossDropItemTexture( int iIndex_, int iItemID_ = -1, bool bShow_ = false);

	//{던전, 파티 버튼 동작 방식 변경
	void SetPVPButton(GAME_TOOL_BUTTON_STATE eButtonState_ = BUTTON_START, bool bChecked_ = true, bool bForceDisable = false);
	void SetDungeonButton(GAME_TOOL_BUTTON_STATE eButtonState_ = BUTTON_START, bool bChecked_ = true, bool bForceDisable = false);


	//{던전, 파티 버튼 동작 방식 변경}

	bool GetShowDungeonMenu()
	{ 
		if( NULL != m_pDLGParty) 
			return m_pDLGParty->GetShow(); 
		else
			return false; 
	}
	bool GetShowPVPMenu()
	{ 
		if( NULL != m_pDLGParty) 
			return m_pDLGPartyPVP->GetShow(); 
		else
			return false; 
	}
	 
	CX2LocalMapUI* GetLocalMapUI(){return m_pLocalMapUI; }
	void	UpdateDungeonRequestButton();

#ifdef NEW_DEFENCE_DUNGEON
	void SetChangePartyUI(bool bChangePartyUI) { m_bChangePartyUI = bChangePartyUI; } //어둠의 문 난이도가 변경 되었을 때, EGS_PARTY_CHANGE_DUNGEON_REQ 를 호출하도록 주는 신호
#endif NEW_DEFENCE_DUNGEON

	void UpdateDifficultyUI(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_, 
						CX2Dungeon::DUNGEON_TYPE eDungeonType_ = CX2Dungeon::DT_NORMAL, 
						bool pNormalOnly = false ); //던전도구창 UI 설정
	void UpdateDifficultyButton(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_); //보통, 어려움, 매우어려움 버튼 설정
	void UpdateDifficultyButtonForSpecialDungeon(CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_, CX2Dungeon::DUNGEON_TYPE eDungeonType_); // 헬, 도전 버튼 설정(시공,비던)
	void UpdateDungeonTypeButton(CX2Dungeon::DUNGEON_TYPE eDungeonType_); //시공, 비던, 일반던전의 난이도 버튼 종류 설정
	void UpdateAdequateMemberCount(CX2Dungeon::DUNGEON_TYPE eDungeonType_, CX2Dungeon::DIFFICULTY_LEVEL eDifficultyLevel_); //적정 인원수 설정
	void UpdateNormalOnly(CX2Dungeon::DUNGEON_TYPE eDungeonType_, bool bNormalOnly_); //보통버튼만 있는 경우에 대한 설정
	void UpdateAdequateItemLevel(int iItemLevel_);
	void UpdateDungeonThumbnailQuestMark();


	void UpdateBossItemList(const CX2Dungeon::DungeonData::mapBossDropItem& BossDropItemList_, bool bIsHenirChallenge = false );

	void SetCurrentWaitUserCount( int iWaitUserCount_, bool bShow_ = true );
	void GameStartCurrentMember();
	
#ifdef SERV_NEW_EVENT_TYPES
	void ClearLocalMapDLGs();
#endif SERV_NEW_EVENT_TYPES		
private:
	void ShortCutKeyProcess();
	void UpdateMatchingUI(float fElapsedTime_);
#ifdef QUEST_GUIDE
	void UpdateQuestGuide();
#endif //QUEST_GUIDE
#ifdef REFORM_NOVICE_GUIDE
	void UpdateNoviceGuide();
#endif //REFORM_NOVICE_GUIDE

	bool SelectDungeonDifficulty( const int iSelectDungeonDifficulty_);
	static const int					MAX_PARTY_COUNT_PER_PAGE	= 12;	// 파티리스트는 한번에 12개만 보임
	static const double 				PARTY_LIST_REFRESH_INTERVAL;

	CKTDXStage*							m_pCurrStage;

	CKTDGUIDialogType						m_pDLGPartyList;			// 파티리스트
	CKTDGUIDialogType						m_pDLGParty;				// 파티 기본 기능, 파티만들기, 시작하기 등등의 버튼이 있는
	//CKTDGUIDialogType						m_pDLGPartyMember;			// 내 파티의 멤버들 정보
	CKTDGUIDialogType						m_pDLGPartyListMeberPreview;	// 파티리스트에서 파티원 정보 미리보기
	CKTDGUIDialogType						m_pDLGWarningAllReady;		// (파티장 전용) 모든 파티원이 레디가 됐음을 알리는 경고
	CKTDGUIDialogType						m_pDLGPartyFever;

	CKTDGUIDialogType						m_pDLGQuestRewardItemInfo;
#ifdef 	FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION
	CKTDGUIDialogType						m_pDLGQuestRewardItemInfoAdd;
#endif // FIXED_DIALOG_REWARD_ITEM_SPLIT_DESCRIPTION

#ifdef SERV_PVP_NEW_SYSTEM
	std::vector<PVP_MAP_INFO>				m_vecPvpMapInfo;
	std::vector<short>						m_vecDefaultMap;
	SHORT									m_SumDelta;
	bool									m_bPartyDungeon;			// 현재 선택된 파티가 던전인지 여부
	bool									m_bPartyPVP;				// 현재 선택된 파티가 대전인지 여부
	CKTDGUIDialogType						m_pDLGPartyMenu;			// 던전/대전 파티 선택 버튼메뉴
	CKTDGUIDialogType						m_pDLGPartyPVP;
	CKTDGUIDialogType						m_pDLGPartyPVPSetting;
	CKTDGUIDialogType						m_pDLGPartyPVPSettingButton;
	CKTDGUIDialogType						m_pDlgPartyPvpMapSetting;
	//CKTDGUIDialogType						m_pDlgPartyPvpEtcSetting;
	CKTDGUIDialogType						m_pDlgPvpConfirm;	

	int										m_iEstimatedTime;
	float									m_fWaitPvpTime;	
#endif
	
	CX2LocationManager::LOCAL_MAP_ID	m_ePartyListLocalMapID;		// 파티리스트 창에 보일 파티리스트가 속한 지역 ID 
	int									m_iPartyListDungeonID;		// 파티리스트에서 선택된 던전 ID	

	int									m_iPartyListPageNumber;		// [1,x]
	int									m_iMaxPartyListPageNumber;	//

	int									m_iPickedPartyListSlotIndex;	// [0, MAX_PARTY_COUNT_PER_PAGE]

	CX2PartyManager::PartyData			m_aPartyData[ MAX_PARTY_COUNT_PER_PAGE ];		// 파티리스트에 나올 파티 data

	CKTDXTimer							m_timerPartyListRefresh;

	CX2LocalMapUI*						m_pLocalMapUI;
	int									m_iNoviceGuideStep;

	float								m_fElapsedTime;				// 파티갱신 쿨타임

	std::map<wstring, int>				m_mapBossItemList;
	bool								m_bShowQuestMark;				// 대전 시작 버튼 // 던전시작 버튼 위의 Q마크 노출 여부
	// 오현빈 // 2012-10-12 // 사용하는 곳이 많아져서 멤버 변수로 가지고 있도록 변경
	CKTDGUICheckBox*					m_pDungeonCheckBox;		// 던전 시작 버튼
	CKTDGUICheckBox*					m_pPVPCheckBox;	

#ifdef NEW_DEFENCE_DUNGEON
	bool								m_bChangePartyUI;			//어둠의 문 난이도가 변경 되었을 때, EGS_PARTY_CHANGE_DUNGEON_REQ 를 호출하도록 주는 신호
#endif NEW_DEFENCE_DUNGEON

	GAME_TOOL_BUTTON_STATE				m_eDungeonButton;
	GAME_TOOL_BUTTON_STATE				m_ePVPButton;

#ifdef FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN
	LPARAM								m_lParam;	// 현재 포커싱 중인 보상 아이템 버튼을 담아둠
#endif // FIXED_DIALOG_REWARD_ITEM_NOT_DRAWN

#ifdef SERV_EVENT_VALENTINE_DUNGEON
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hValentineHeart1;		/// 발렌타인 던전 안내 이펙트 1
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hValentineHeart2;		/// 발렌타인 던전 안내 이펙트 2
#endif SERV_EVENT_VALENTINE_DUNGEON
};
