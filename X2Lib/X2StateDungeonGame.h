#pragma once


#ifdef ITEM_VIEW_CHEAT

class ItemViewListDialog
{

public: 
	ItemViewListDialog() 
		: m_pDLGItemViewList( NULL )
		, m_bSortItemViewList( false )
	{
	}

	~ItemViewListDialog()
	{
		SAFE_DELETE_DIALOG( m_pDLGItemViewList );
	}

	bool IsOpen()
	{
		if( m_pDLGItemViewList == NULL )
			return false;

		if( false == m_pDLGItemViewList->GetShow() )
			return false;
		else
			return true;
	}

	void Close()
	{
		if( m_pDLGItemViewList != NULL )
		{
			m_pDLGItemViewList->SetShowEnable( false, false );
		}
	}

	void Open( CKTDXStage* pStage )
	{
		if( m_pDLGItemViewList == NULL )
		{
			m_pDLGItemViewList = new CKTDGUIDialog( pStage, L"DLG_Monster_List_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGItemViewList );

			Update( m_bSortItemViewList );
		}

		if( m_pDLGItemViewList != NULL )
		{
			m_pDLGItemViewList->SetStage( pStage );
			m_pDLGItemViewList->SetShowEnable( true, true );
		}
	}

	void Update( bool bSortAsscending );

	void KeyProcess( CKTDXStage* pStage );
	void OnCommand( const int& ItemID );
	void OnChangeSorting()
	{
		m_bSortItemViewList = !m_bSortItemViewList;
		Update( m_bSortItemViewList );
	}

	CKTDGUIDialogType GetDialog() { return m_pDLGItemViewList; };

protected:
	CKTDGUIDialogType m_pDLGItemViewList;

	bool m_bSortItemViewList;


};

#endif ITEM_VIEW_CHEAT




#ifdef CREATE_MONSTER_LIST_TEST
class CreateMonsterListDialog
{
public: 
	CreateMonsterListDialog() 
		: m_pDLGCreateMonsterList( NULL )
		, m_bSortMonsterList( false )
	{
	}

	~CreateMonsterListDialog()
	{
		SAFE_DELETE_DIALOG( m_pDLGCreateMonsterList );
	}

	bool IsOpen()
	{
		if( m_pDLGCreateMonsterList == NULL )
			return false;

		if( false == m_pDLGCreateMonsterList->GetShow() )
			return false;
		else
			return true;
	}

	void Close()
	{
		if( m_pDLGCreateMonsterList != NULL )
		{
			m_pDLGCreateMonsterList->SetShowEnable( false, false );
		}
	}

	void Open( CKTDXStage* pStage )
	{
		if( m_pDLGCreateMonsterList == NULL )
		{
			m_pDLGCreateMonsterList = new CKTDGUIDialog( pStage, L"DLG_Monster_List_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGCreateMonsterList );

			Update( m_bSortMonsterList );
		}

		if( m_pDLGCreateMonsterList != NULL )
		{
			m_pDLGCreateMonsterList->SetStage( pStage );
			m_pDLGCreateMonsterList->SetShowEnable( true, true );
		}
	}

	void Update( bool bSortAlphabetical );

	void KeyProcess( CKTDXStage* pStage );
	void OnCommand( const std::wstring& monsterName );
	void OnChangeSorting()
	{
		m_bSortMonsterList = !m_bSortMonsterList;
		Update( m_bSortMonsterList );
	}

	CKTDGUIDialogType GetDialog() { return m_pDLGCreateMonsterList; }

protected:
	CKTDGUIDialogType m_pDLGCreateMonsterList;
	
	bool m_bSortMonsterList;

}; // class CreateMonsterListDialog

#endif CREATE_MONSTER_LIST_TEST




#ifdef MONSTER_STATE_LIST_TEST
// class MonsterStateListDialog
// {
// public: 
// 	MonsterStateListDialog() 
// 		: m_pDLGMonsterStateList( NULL )
// 		, m_NPCID( CX2UnitManager::NUI_NONE )
// 	{
// 		m_vecStateName.reserve( 1024 ); 
// 	}
// 
// 	~MonsterStateListDialog()
// 	{
// 		SAFE_DELETE_DIALOG( m_pDLGMonsterStateList );
// 	}
// 
// 	bool IsOpen()
// 	{
// 		if( m_pDLGMonsterStateList == NULL )
// 			return false;
// 
// 		if( false == m_pDLGMonsterStateList->GetShow() )
// 			return false;
// 		else
// 			return true;
// 	}
// 
// 	void Close()
// 	{
// 		if( m_pDLGMonsterStateList != NULL )
// 		{
// 			m_pDLGMonsterStateList->SetShowEnable( false, false );
// 		}
// 	}
// 
// 	void Open( CKTDXStage* pStage )
// 	{
// 		if( m_pDLGMonsterStateList == NULL )
// 		{
// 			m_pDLGMonsterStateList = new CKTDGUIDialog( pStage, L"DLG_Monster_List_Window.lua" );
// 			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMonsterStateList );
// 		}
// 
// 		if( m_pDLGMonsterStateList != NULL )
// 		{
// 			m_pDLGMonsterStateList->SetStage( pStage );
// 			m_pDLGMonsterStateList->SetShowEnable( true, true );
// 
// 			Update();
// 		}
// 	}
// 
// 	void Update();
// 
// 	void KeyProcess( CKTDXStage* pStage );
// 
// 	void OnCommand( const std::wstring& monsterName );
// 
// 
// 	CKTDGUIDialogType GetDialog() { return m_pDLGMonsterStateList; }
// 
// 
// 
// protected:
// 	CKTDGUIDialogType m_pDLGMonsterStateList;
// 	std::vector< std::wstring > m_vecStateName; 
// 	CX2UnitManager::NPC_UNIT_ID m_NPCID;
// 
// }; // class MonsterStateListDialog
// 
#endif MONSTER_STATE_LIST_TEST



/** @class  BadAttitudeUserWarningDialog  
    @brief  불량 유저에 대한 경고 메시지 다이알로그 

    @desc   Instance 로 제작되어 있다.
			DungeonGame 상태에서만 적용되므로 StateDungeonGame 에 위치해있다.
			DLG 가 항상 등장하는 것이 아니니 차라리 동적으로 할당할 수 있게 제작할까 고민 중이다.
			
    @date   김종훈 on 2013-01-24, 14:32 
*/

#ifndef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
class BadAttitudeUserWarningDialog
{
public: 

	BadAttitudeUserWarningDialog() 
		: m_pDLGWarningState ( NULL )
		, m_fWarningRemainingTime (0.f)
		, m_bIsStartFade ( false )
		, m_bIsEndFade ( false ) 
	{
	}

	~BadAttitudeUserWarningDialog()
	{
		SAFE_DELETE_DIALOG( m_pDLGWarningState );	// Safe Delete Dialog
	}

	bool IsOpen()								// 열려있나?
	{
		if( m_pDLGWarningState == NULL )		// 생성하지 않았음
		{
			return false;
		}
	
		if( false == m_pDLGWarningState->GetShow() )	// 생성되었으나 보여지는 상태는 아님
		{
			return false;
		}
		else								// 생성되어 있다.
		{
			return true;
		}
	}

	void Close()							// 해당 Dlg 를 닫는다.
	{										// 이 객체는 instance 형이라 제거하진 않는다.
		if( m_pDLGWarningState != NULL )
		{
			m_pDLGWarningState->SetShowEnable( false, false );	// 보여지는 상태 및 활성화 상태를 해제
			Init ();											// 데이터를 초기화 한다. ( 존재시간, Fade bool 형 )
		}
	}

	void Init ()												// 데이터를 초기화 한다.
	{															// 그림 데이터는 Instance 생성 시, 저장해둔다.
		m_fWarningRemainingTime = 0.f;						// 존재 시간
		m_bIsStartFade = false;								// Fadein 을 했는가?
		m_bIsEndFade = false;								// FadeOut 을 했는가?
	}


	
	void Open( CKTDXStage* pStage )								// 해당 Dlg 가 없다면 생성하고 다시 보여지게 한다.
	{
		if( m_pDLGWarningState == NULL )						// 아직 생성되지 않았다면
		{
			m_pDLGWarningState = new CKTDGUIDialog( pStage, L"DLG_Bad_Attribute_Player_Warning.lua" );	// Lua 파일을 읽어 생성한다.
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGWarningState );				// DialogManager 에 등록한다.
		}
		else													// 생성했었다면
		{
			m_pDLGWarningState->SetStage( pStage );				// 현재 Stage 를 저장한다.
			m_pDLGWarningState->SetShowEnable( true, true );	// 보여지게 하고 활성화한다.
		}
				
		CKTDGUIStatic* pStaticWarningDlg = static_cast <CKTDGUIStatic*> ( m_pDLGWarningState->GetControl(L"DLG_Bad_Attribute_Player_Warning") );		// 해당 Static 을 불러와서

		if ( NULL != pStaticWarningDlg )			
		{
			m_pButtonExitWarningDlg = static_cast <CKTDGUIButton*> ( m_pDLGWarningState->GetButton_LUA("_exit") );

			if ( true == m_VecDlgPictureDataPointer.empty ()  )				// 그림 데이터가 비어있다면
			{	
				// 0번 그림 데이터를 가져와서 m_VecDlgPictureDataPointer 에 넣어둔다.
				// 하드 코딩..그림이 추가되면 여기서 push_back 하도록 하자
				m_VecDlgPictureDataPointer.push_back(pStaticWarningDlg->GetPicture(0));		// 다시 불러온다.
			}
		}
		Init ();	// 데이터를 초기화

	}

	void					SetIsFadeIn ( const bool bStartFade_ )			{ m_bIsStartFade = bStartFade_; }	// FadeIn Bool 값을 설정
	bool					GetIsFadeIn () const							{ return m_bIsStartFade; }			// FadeIn Bool 값을 받는다.

	void					SetIsFadeOut ( const bool bEndFade_ )			{ m_bIsEndFade = bEndFade_; }		// FadeOut Bool 값을 설정
	bool					GetIsFadeOut () const							{ return m_bIsEndFade; }			// FadeOut Bool 값을 받는다.

	CKTDGUIDialogType		GetDialog() const { return m_pDLGWarningState; }			// 해당 Dlg 데이터를 받는다.

	void					SetFadeInDlg ( const float fTime_ );					// Dlg 에 Fade in 설정
	void					SetFadeOutDlg ( const float fTime_ );					// Dlg 에 Fade Out 설정

	void					Update ( float fElapsedTime_ );							// 지나온 시간에 맞게 행동		

protected:
	CKTDGUIDialogType						m_pDLGWarningState;				// DLG State, LUA 파일 불러서 갖고 있는다.
	float									m_fWarningRemainingTime;		// DLG 생성 이후 존재했던 시간, 0초 보다 큼 = FadeIn, 5초 보다 큼 = FadeOut, 6초보다 큼 = Destroy
	bool									m_bIsStartFade;					// true 이면 Fade-in 을 시작했었음
	bool									m_bIsEndFade;					// true 이면 Fade-out 을 시작했었음
	CKTDGUIButton*							m_pButtonExitWarningDlg;		// Fade-In, out 에 사용할 버튼 포인터
	vector<CKTDGUIControl::CPictureData*>   m_VecDlgPictureDataPointer;		// Fade-in, out 에 사용할 그림 파일들
};

#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG




class CX2StateDungeonGame : public CX2StateMenu
{
public:

	enum DUNGEON_GAME_UI_CUSTOM_MSG
	{
		DGUCM_GAME_LOADING = 0,
		DGUCM_GAME_START,
		DGUCM_GAME_EXIT_OK,

		DGUCM_GAME_EXIT_CANCEL,

		DGUCM_SUMMON_SELECTED_MONSTER_DEBUG,
		DGUCM_SORT_MONSTER_LIST_DEBUG,

		DGUCM_GOTO_FIRST_DUNGEON_OK,
		DGUCM_GOTO_FIRST_DUNGEON_CANCEL,

		DGUCM_TOTORIAL_EXIT,

		DGUCM_WANT_TO_ENTER_SECRET_STAGE_OK,
		DGUCM_WANT_TO_ENTER_SECRET_STAGE_CANCEL,

		DGUCM_CLOSE_BAD_ATTITUDE_WARNING_DIALOG,

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // 적용날짜: 2013-04-22
		DGUCM_AGREE_ENTER_GATE_OF_DARKNESS,		/// 어둠의 문 입장 동의
		DGUCM_NOT_AGREE_ENTER_GATE_OF_DARKNESS,	/// 어둠의 문 입장 거부
#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

	};
// 스크립트와 인덱스 맞춰야 함. DLG_PVP_GAME_LOADING_STAT_USER.lua
	enum LOADING_USER_INFO
	{
		LUI_ELSWORD_COLOR = 0,
		LUI_ARME_COLOR,
		LUI_LIRE_COLOR,
		LUI_RAVEN_COLOR,
		LUI_EVE_COLOR,
		LUI_CHUNG_COLOR,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_COLOR,
#endif


		LUI_ELSWORD_BLACK,
		LUI_ARME_BLACK,
		LUI_LIRE_BLACK,
		LUI_RAVEN_BLACK,
		LUI_EVE_BLACK,
		LUI_CHUNG_BLACK,
#ifdef ARA_CHARACTER_BASE
		LUI_ARA_BLACK,
#endif
//#ifdef NEW_CHARACTER_EL 
		LUI_EL_COLOR,
		LUI_EL_BLACK,
//#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
		LUI_ADD_COLOR,
		LUI_ADD_BLACK,
#endif //SERV_9TH_NEW_CHARACTER
		LUI_EMPTY,
	};

	struct DungeonMapUI
	{
	public:
		struct SubStageNode
		{
			vector< std::pair<int, int> >		m_vecNextSubStageIndex;
		};

	public:
        DungeonMapUI( CX2Dungeon* pDungeon, CX2State* pNowState )
		{
			m_bShow = false;

			m_pDungeon = pDungeon;
			CreatStageGraph( pDungeon );

			m_pNowState = pNowState;
			m_pDLGDungeonMap		= NULL;
			m_pDLGDungeonMap		= new CKTDGUIDialog( (CKTDXStage*) m_pNowState, L"DLG_DungeonMap_UI.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDungeonMap );
			m_vPos = m_pDLGDungeonMap->GetPos();

			SetShow( m_bShow );
		}

		~DungeonMapUI()
		{
            SAFE_DELETE_DIALOG( m_pDLGDungeonMap );
		}

		bool GetShow() const { return m_bShow; }
		void SetShow( bool bShow ) 
		{ 
			m_bShow = bShow; 

			if( m_pDLGDungeonMap != NULL )
			{
				m_pDLGDungeonMap->SetShowEnable( m_bShow, m_bShow );
			}
		}

		void CreatStageGraph( CX2Dungeon* pDungeon );
		void CreateDungeonMapUI();
		void ResizeBackground();
		void OnFirstSubStage();

		void SetToOriginalPos()
		{
			if( m_pDLGDungeonMap == NULL )
				return;

			m_pDLGDungeonMap->Move( m_vPos, D3DXCOLOR(1, 1, 1, 1), 0.01f, true );
		}

	private:
		CKTDGUIDialogType			m_pDLGDungeonMap;
		CX2State*				m_pNowState;		
		CX2Dungeon*				m_pDungeon;

		map< std::pair<int, int>, SubStageNode> m_mapStageGraph;

		bool					m_bShow;

		D3DXVECTOR2				m_vPos;
	};


	struct DungeonEndingEvent
	{
		DungeonEndingEvent();
		~DungeonEndingEvent();

		void OnFrameMove( double fTime, float fElapsedTime );

		//{{ kimhc // 2010.12.13 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		void SetDungeonClearTexture( CKTDGUIStatic* pStaticClearTexture_ );
#endif	NEW_CHARACTER_CHUNG
		//}} kimhc // 2010.12.13 //  2010-12-23 New Character CHUNG

		void Start()
		{
			m_bStarted = true;
			m_fElapsedTime = 0.f;
			m_bEnd = false;
		}

		void StartChapter2()
		{
			m_bStartChapter2 = true;			
		}

	public:
		CKTDGUIDialogType m_pDLGSpeechBalloon;
		CKTDGUIDialogType m_pDLGCharacterPicture;

		float		m_fElapsedTime;
		bool		m_bStarted;
		bool		m_bEnd;

		bool		m_bEndChapter1;
		bool		m_bStartChapter2;
		float		m_fSpeechElapsedTime;

		int			m_iIndexSpeech1;
		int			m_iIndexSpeech2;


		float		m_fElapsedTime_ShowClearEffect;
		float		m_fElapsedTime_WinEffect;

		std::vector< wstring > m_vecSpeech1;
		std::vector< wstring > m_vecSpeech2;

	};



#ifdef HENIR_TEST

	struct SecretStageEnteringEvent
	{
		SecretStageEnteringEvent();
		~SecretStageEnteringEvent();

		void OnFrameMove( double fTime, float fElapsedTime );

		void Start()
		{
			SAFE_DELETE_DIALOG( m_pDLGSpeechBalloon );
			SAFE_DELETE_DIALOG( m_pDLGCharacterPicture );

			m_bStarted = true;
			m_fElapsedTime = 0.f;
			m_bEnd = false;
			m_bCreatedChallengeParticle = false;
		}

		void End()
		{
			m_fElapsedTime = 0.f;
			m_bStarted = false;
			m_bEnd = true;

			if( m_pDLGCharacterPicture != NULL )
				m_pDLGCharacterPicture->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
			if( m_pDLGSpeechBalloon != NULL )
				m_pDLGSpeechBalloon->Move( D3DXVECTOR2(0, 0), D3DXCOLOR(1, 1, 1, 0), 0.25f, false );
		}
		
	public:
		CKTDGUIDialogType m_pDLGSpeechBalloon;
		CKTDGUIDialogType m_pDLGCharacterPicture;

		float		m_fElapsedTime;
		bool		m_bStarted;
		bool		m_bEnd;

		float		m_fSpeechElapsedTime;
		int			m_iIndexSpeech1;

		float		m_fElapsedTime_ShowClearEffect;
        
		std::vector< wstring > m_vecSpeech1;
		CX2UnitManager::NPC_UNIT_ID m_eBossNPCID; 

		bool m_bCreatedChallengeParticle;
	};


#endif HENIR_TEST
	struct DRAWFACE_RHW_VERTEX
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};

#ifdef REFORM_ENTRY_POINT
	struct DUNGEON_LOADING_DATA
	{
		int iLoadingPercent;
		UidType uidUnitUID;

		DUNGEON_LOADING_DATA()
		{
			iLoadingPercent = 0;
			uidUnitUID = -1;
		}
	};
#endif //REFORM_ENTRY_POINT

public:
	CX2StateDungeonGame(void);
	~CX2StateDungeonGame(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	virtual void SetShowStateDLG( bool bShow ) {};


	bool DungeonStageLoadReq( int stageNum );
	bool EndDungeonGameReq( CX2Room::TEAM_NUM winTeam );		// 게임을 정상적으로 마친경우


	void OpenExitDLG( const WCHAR* wszMsg );

	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

	DungeonMapUI* GetDungeonMapUI() const { return m_pDungeonMapUI; }

	CX2StateDungeonGame::DungeonEndingEvent* GetDungeonEndingEvent() { return &m_DungeonEndingEvent; }



#ifdef HENIR_TEST
	CX2StateDungeonGame::SecretStageEnteringEvent* GetSecretStageEnteringEvent() { return &m_SecretStageEnteringEvent; }
#endif HENIR_TEST

#ifdef OLD_COMBO_LIST_UI
	void CreateComboListForTutorial();
#endif OLD_COMBO_LIST_UI


	// 09.03.23 태완 : public으로 이동
	void ToggleDungeonMapUI();

#ifdef HENIR_TEST

	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ();
	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Send_EGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT( bool bWantToEnterSecretStage );
	bool Handler_EGS_DUNGEON_SECRET_STAGE_ENTER_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#endif HENIR_TEST

#ifdef NEW_HENIR_TEST
	int GetStartSecretStageEnteringEvent(){ return m_iStartSecretStageEnteringEvent; } 
	void InitStartSecretStageEnteringEvent(){ m_iStartSecretStageEnteringEvent = 0; }
	bool SecretDungeonStageLoadReq(int iVal);
#endif NEW_HENIR_TEST

	bool Handler_EGS_DUNGEON_KILLALLNPC_CHECK_REQ();	
	bool Handler_EGS_DUNGEON_KILLALLNPC_CHECK_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


#ifdef SERV_EVENT_VALENTINE_DUNGEON
	void NotfiyCreateValentineCupCake();
	void Send_EGS_SYNC_DUNGEON_TIMER_NOT();
	bool Handler_EGS_SYNC_DUNGEON_TIMER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	void				AllocateFaultyPlayerWarningDlg ();							// DLG 할당
	void				DeallocateFaultyPlayerWarningDlg ();						// DLG 삭제
	void				ReservingWarningDlgDelete ();								// DLG 삭제 예약
#endif	// FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
	bool Handler_EGS_DECISION_ENTER_DEFENCE_DUNGEON_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_UPDATE_INVENTORY_SLOT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // 적용날짜: 2013-04-22
	bool Handler_LEAVE_ROOM_BEFORE_DEFENCE_DUNGEON_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void Sned_Handler_EGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT( bool bAgree );
#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

	/// 어둠의 문 시작까지의 대기 시간
	float GetWaitDefenceDungeonStartTime() const { return m_fWaitDefenceDungeonStartTime; }
	void SetWaitDefenceDungeonStartTime(float val) { m_fWaitDefenceDungeonStartTime = val; }

	void WaitDefenceDungeonStart( const float fElapsedTime );		/// 특정 시간 이후 어둠의 문 스테이트 변경을 처리하는 함수

#endif // SERV_NEW_DEFENCE_DUNGEON
#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES // 오현빈
	bool Handler_EGS_DUNGEON_KILLALLNPC_CHECK_NOT();
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES

#ifdef PLAY_EMOTION_BY_USER_SELECT
	virtual void PlayEmotionByUserSelect();
#endif // PLAY_EMOTION_BY_USER_SELECT

protected:

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	// 09.03.30 태완
	virtual void SetShowCommonBG(){};

	//방 정보 변경 관련
	bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_GAME_LOADING_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_STATE_CHANGE_RESULT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_STATE_CHANGE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool GameLoadingReq( int percent );
	bool GameLoadingAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool GameLoadingNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GameLoadingAllUnitOkNot();

	bool PlayStartNot();

	bool BroadCastDungeonRoomSlotInfoNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_END_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	virtual bool StateChangeFieldReq();
	virtual bool StateChangeLocalMapReq();
	bool StateChangeLocalMapAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool StateChangeResultReq();
	bool StateChangeResultAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool StateChangeResultNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool LoadNextStageOrSubStageReq( string nextStageID = "" );
	bool LoadNextStageOrSubStageAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool LoadNextStageOrSubStageNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool StageLoadingReq();
	bool StageLoadingAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool StageLoadingCompleteAllUnitNot();

	bool NPCUnitCreateAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool NPCUnitCreateNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool NPCUnitDieReq();
	bool NPCUnitDieAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool NPCUnitDieNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool DungeonStageLoadAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool DungeonStageLoadNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool DungeonStageLoadCompleteReq( bool bIsSecretStage );
	bool DungeonStageLoadCompleteAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool DungeonStageLoadCompleteNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool DungeonStageStartNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_REMAINING_PLAY_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_MY_USER_UNIT_INFO_TO_SERVER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool Handler_EGS_NOTIFY_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	

	bool Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
	bool Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif DUNGEON_TIME_OUT_FRAMEMOVE

	bool Handler_EGS_PLAY_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트
#ifdef	SERV_TOONILAND_CHANNELING_EVENT
	bool Handler_EGS_USER_GAME_STAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	SERV_TOONILAND_CHANNELING_EVENT
	//}} kimhc // 2011-08-08 // 투니 랜드 채널링 이벤트

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	bool	Handler_EGS_BAD_ATTITUDE_USER_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	bool Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_STAGE_CLEAR_IN_SERVER
#ifdef SERV_CATCH_HACKUSER_INFO
	bool Handler_EGS_CATCH_HACKUSER_INFO_NOT( KEGS_CATCH_HACKUSER_INFO_NOT& kEGS_CATCH_HACKUSER_INFO_NOT );
#endif SERV_CATCH_HACKUSER_INFO

	void LoadUI();
	void CreateGame();
	void GameLoadingStart();


	virtual bool QuitGame();


	// tutorial 관련
	void ClearTutorialInputTreeParticle();
	HRESULT TutorialFrameMove( double fTime, float fElapsedTime );

	void TextOutSkillList( const char unitStateID );
	void CreateSkillSuccessEffect() { m_bCreateCommandSuccessSplash = true; }

	virtual bool ShortCutKeyProcess();

	virtual void ProcessShowMiniMap( const bool bHide_ );



#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    void					MoveToRubenVillage( float fElapsedTime );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void					MoveToRubenVillage();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	void					DrawMovingSmallBar();
	void					CreateMovingSmallBar();
	void					DestroyMovingSmallBar();
	void					DrawFace( const float fX_, const float fY_, 
									const CKTDGUIControl::UITextureData& texData_, 
									D3DCOLOR color_ /* = 0xffffffff */, const float fWidthPercent_ = 1.0f );

#ifdef SERV_EVENT_VALENTINE_DUNGEON
	void ValentineDungeonTimer_OnFrameMove( float fElapsedTime_ );
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_NEW_DEFENCE_DUNGEON // 적용날짜: 2013-04-09
	/// 어둠의 문 입장 처리 여부
	bool GetEnterDefenceDungeon() const { return m_bEnterDefenceDungeon; }
	void SetEnterDefenceDungeon(bool val) { m_bEnterDefenceDungeon = val; }
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef REFORM_ENTRY_POINT
	void SetLoadingGageBar( int iLoadingPercent );
#endif //REFORM_ENTRY_POINT

private:
	CKTDGUIDialogType					m_pDLGExitMsgBox;
	CKTDGUIDialogType					m_pDLGLoadingState;
	CKTDGUIDialogType					m_pDLGTutorialInputControl;

	CKTDGUIDialogType					m_pDLGGoToFirstDungeon;
	CKTDGUIDialogType					m_pDLGInputUI;
	CKTDGUIDialogType					m_pDLGTutorialExitButton;

	//CKTDGUIDialogType					m_pDLGF1Help;		
	CKTDGUIDialogType					m_pDLGF1Help2;

	CKTDGUIDialogType					m_pDLGArcadeFrame;




	CX2DungeonGame*					m_pDungeonGame;
	bool							m_bLoadingOK;
	CX2GameLoadingTip*				m_pCX2GameLoadingTip;

	wstring							m_HostIP;
	int								m_HostPort;


	bool							m_bSubStageLoading;
	int								m_SubStageLoadingRenderCount;

	bool							m_bResultStateReq;
	bool							m_bGameEnd;
	float							m_fGameEndWaitTime;



	CKTDXDeviceTexture*				m_pLoadingOutLineTex;
	CKTDXDeviceTexture*				m_pLoadingBasicTex;


	int								m_LoadingPercent;
	int								m_LoadingRenderCount;
	CX2Game::CONNECT_TYPE			m_ConnectType;

	int								m_NextStageNum;
	int								m_RenderCount;

#ifdef REFORM_ENTRY_POINT
	CKTDGUIDialogType				m_pDLGLoadingFront;
#else //REFORM_ENTRY_POINT
	vector< CKTDGUIDialogType >		m_DLGLoadingStateUnitInfoList;
#endif //REFORM_ENTRY_POINT

	// tutorial 관련
	char							m_TutorialBeforeUserStateID;
	vector< CKTDGParticleSystem::CParticleEventSequenceHandle  >	m_vecTutorialInputTreeParticle;
	D3DXVECTOR3						m_vTutorialParticlePos;
	bool							m_bShowSkilList;
	CKTDGFontManager::CUKFont*		m_pFontForSkillList;


	char							m_HighLightSkillID; 
	float							m_fTimeLeftForHighlight;
	D3DXCOLOR						m_coHighlight;
	D3DXCOLOR						m_coHighLightIncrement;
	int								m_iTutorialMsgFlashCount;

	bool							m_bCreateCommandSuccessSplash;

	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hTutorialMissionSplashParticle;

	map< CX2Unit::UNIT_CLASS, map<char, wstring> > m_mapUnitCommandList;


	DungeonMapUI*					m_pDungeonMapUI;

	bool							m_bWasQuestUIMinimized;
	bool							m_bWasMiniMapMinimized;




	bool							m_bSend_KEGS_END_GAME_REQ;

	bool							m_bStateChangeLocalMap;

	bool							m_bSend_KEGS_LEAVE_ROOM_REQ;
	bool							m_bReceive_KEGS_LEAVE_ROOM_ACK;
	bool							m_bReceive_KEGS_UNIT_INFO_UPDATE;

	bool							m_bLeaveRoomAtTutorial;

	DungeonEndingEvent				m_DungeonEndingEvent;
#ifdef HENIR_TEST
	SecretStageEnteringEvent		m_SecretStageEnteringEvent;
#endif HENIR_TEST
#ifdef NEW_HENIR_TEST
	int								m_iStartSecretStageEnteringEvent;
#endif NEW_HENIR_TEST
	CKTDGUIDialogType					m_pDLGArcadeMission;
	float							m_fTimeShowArcadeMission;
	bool							m_bAutoShowOffArcadeMission;


#ifdef CREATE_MONSTER_LIST_TEST
	CreateMonsterListDialog	m_CreateMonsterListDialog;
#endif CREATE_MONSTER_LIST_TEST

#ifdef MONSTER_STATE_LIST_TEST
	MonsterStateListDialog m_MonsterStateListDialog;
#endif MONSTER_STATE_LIST_TEST

#ifndef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	BadAttitudeUserWarningDialog m_BadAttitudeUserWarningDialog;
#endif FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

	CKTDXCheckElapsedTime				m_TimerWaitingPortal;
	CKTDGUIControl::UITextureData		m_TexDataMovingGageBG;
	CKTDGUIControl::UITextureData		m_TexDataMovingGage;
	CKTDGStateManager::KStateID			m_RenderStateID;	/// 인터페이스 출력을 위한 2D 출력함수

#ifdef FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG
	CKTDGUIDialogType						m_pDLGFaultyPlayerWarning;
	bool									m_bFaultyPlayerWarningDlgExitButtonPressed;	// DLG 가 삭제 되어야 하는가?
#endif // FIXED_DIALOG_FAULTY_PLAYER_WARNING_DLG

#ifdef SERV_EVENT_VALENTINE_DUNGEON
	CKTDGUIDialogType					m_pDLGValentineTimer;	/// 발렌타인 던전 타이머 UI
	float								m_fValentineDungeonRemainTime;	/// 발렌타인 던전 타이머 UI
	int									m_iValentineDungeonStage;		/// 발렌타인 던전 스테이지
	bool								m_bIsValentineDungeon;
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hStageStartEffect1;		/// 스테이지 변경시 발생되는 안내 문구 이펙트 1
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hStageStartEffect2;		/// 스테이지 변경시 발생되는 안내 문구 이펙트 2
#endif //SERV_EVENT_VALENTINE_DUNGEON

#ifdef SERV_NEW_DEFENCE_DUNGEON	// 적용날짜: 2013-04-09
	bool								m_bEnterDefenceDungeon;				/// 어둠의 문 입장 처리 여부
	float								m_fWaitDefenceDungeonStartTime;		/// 어둠의 문 시작까지의 대기 시간
#endif // SERV_NEW_DEFENCE_DUNGEON

#ifdef DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL
	SHORT								m_SumDelta;		/// 마우스 휠 저장값
#endif //DUNGEON_CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef REFORM_ENTRY_POINT
	vector<DUNGEON_LOADING_DATA>		m_vecDungeonLoadingData; //던전 로딩 데이터. 던전 로딩형태가 게이지바 하나로 변화되며 
																 //그 퍼센트 기준점이 제일 로딩이 느린 유저가 되야해서 추가한 구조체 벡터. (서버 작업 없이)
#endif //REFORM_ENTRY_POINT

};

