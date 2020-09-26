#pragma once

#ifdef MONSTER_STATE_LIST_TEST
class MonsterStateListDialog
{
public: 
	MonsterStateListDialog() 
		: m_pDLGMonsterStateList( NULL )
		, m_NPCID( CX2UnitManager::NUI_NONE )
	{
		m_vecStateName.reserve( 1024 ); 
	}

	~MonsterStateListDialog()
	{
		SAFE_DELETE_DIALOG( m_pDLGMonsterStateList );
	}

	bool IsOpen()
	{
		if( m_pDLGMonsterStateList == NULL )
			return false;

		if( false == m_pDLGMonsterStateList->GetShow() )
			return false;
		else
			return true;
	}

	void Close()
	{
		if( m_pDLGMonsterStateList != NULL )
		{
			m_pDLGMonsterStateList->SetShowEnable( false, false );
		}
	}

	void Open( CKTDXStage* pStage )
	{
		if( m_pDLGMonsterStateList == NULL )
		{
			m_pDLGMonsterStateList = new CKTDGUIDialog( pStage, L"DLG_Monster_List_Window.lua" );
			g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGMonsterStateList );
		}

		if( m_pDLGMonsterStateList != NULL )
		{
			m_pDLGMonsterStateList->SetStage( pStage );
			m_pDLGMonsterStateList->SetShowEnable( true, true );

			Update();
		}
	}

	void Update();

	void KeyProcess( CKTDXStage* pStage );

	void OnCommand( const std::string& monsterName );


	CKTDGUIDialogType GetDialog() { return m_pDLGMonsterStateList; }



protected:
	CKTDGUIDialogType m_pDLGMonsterStateList;
	std::vector< std::string > m_vecStateName; 
	CX2UnitManager::NPC_UNIT_ID m_NPCID;

}; // class MonsterStateListDialog

#endif MONSTER_STATE_LIST_TEST


class CX2StateBattleField : public CX2StateMenu
{
public:

	struct DRAWFACE_RHW_VERTEX
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};

	CX2StateBattleField();
	virtual ~CX2StateBattleField(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL
	virtual bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	//bool DungeonStageLoadReq( int stageNum );
	//bool EndDungeonGameReq( CX2Room::TEAM_NUM winTeam );		// 게임을 정상적으로 마친경우

	void OpenExitDLG( const WCHAR* wszMsg );

	virtual bool Handler_EGS_STATE_CHANGE_FIELD_REQ( const UINT uiVillageId_, const USHORT usBattleFieldStartLineIndex_ );
	/*virtual bool Handler_EGS_STATE_CHANGE_FIELD_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );*/
	
	virtual bool Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

  
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    void FlushSendFrameAverage();
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	void MoveFromBattleFieldToOtherBattleField();
	void SetBattleFieldId( UINT uiBattleFieldId_ ) { m_pBattleFieldGame->SetBattleFieldId( uiBattleFieldId_ ); }

	bool GetNowMovingToOtherPlace() const { return m_bNowMovingToOtherPlace; }
	void SetNowMovingToOtherPlace( bool bNowMovingToOtherPlace_ ) { m_bNowMovingToOtherPlace = bNowMovingToOtherPlace_; }

	bool Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_VISIT_CASH_SHOP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef ADDED_RELATIONSHIP_SYSTEM
	/// mauntain // 2013-05-03 // 결혼 시스템 배우자 소환 기능
	virtual bool Handler_EGS_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef PLAY_EMOTION_BY_USER_SELECT
	virtual void PlayEmotionByUserSelect();
#endif // PLAY_EMOTION_BY_USER_SELECT

#ifdef FIELD_BOSS_RAID
	void MoveToBeforePlaceAtRaidField();
	void SwapWorld(){ if( NULL != m_pBattleFieldGame ) m_pBattleFieldGame->SwapWorld(); }
#endif // FIELD_BOSS_RAID

	void Handler_EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ();
protected:

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual void SetShowStateDLG( bool bShow );
	virtual void SetShowCommonBG();

	virtual void UserAndPetListPopUpMenuProcess();

// 	bool Handler_EGS_CHAR_LEVEL_UP_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Handler_EGS_GET_ITEM_REALTIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Handler_EGS_DECREASE_ENDURANCE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Hander_EGS_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Handler_EGS_ITEM_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Handler_EGS_DECREASE_SPIRIT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Handler_EGS_GET_ITEM_TEMP_INVENTORY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool Handler_EGS_GAME_LOADING_REQ( const int iLoadingProgressPercent_ );
	bool Handler_EGS_GAME_LOADING_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_GAME_LOADING_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	bool Handler_EGS_PLAY_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool Handler_EGS_JOIN_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_LEAVE_BATTLE_FIELD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BATTLE_FIELD_NPC_LOAD_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_BATTLE_FIELD_NPC_LOAD_COMPLETE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_BATTLE_FIELD_NPC_P2P_SYNC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_NPC_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_NPC_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef FINALITY_SKILL_SYSTEM //JHKang
	bool Handler_EGS_USE_FINALITY_SKILL_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //FINALITY_SKILL_SYSTEM

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS

	bool Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// 중간 보스 생성 ACK
	bool Handler_EGS_NPC_FIELD_MIDDLE_BOSS_UNIT_CREATE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// 중간 보스 생성 NOT
	bool Handler_EGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		// DamagedMap 요청 ACK


#endif // SERV_BATTLEFIELD_MIDDLE_BOSS



	bool Handler_EGS_BATTLE_FIELD_EVENT_MESSAGE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

#ifdef SERV_CHECK_EXIST_MONSTER_UID
	bool Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_CHECK_EXIST_MONSTER_UID


// 	void Handler_EGS_DUNGEON_INTRUDE_SYNC_REQ() const;
// 	bool Handler_EGS_DUNGEON_INTRUDE_SYNC_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
// 	bool Handler_EGS_DUNGEON_INTRUDE_SYNC_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void CreateGame();
	void GameLoadingStart();
	
	//virtual bool QuitGame();


	virtual bool ShortCutKeyProcess();
#ifdef  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
    FORCEINLINE void MoveToOtherPlace( float fElapsedTime_ );
#else   X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE
	FORCEINLINE void MoveToOtherPlace();
#endif  X2OPTIMIZE_NPC_ADAPTIVE_FRAME_MOVE

	void CreateMovingSmallBar();
	void DrawMovingSmallBar();
	void DestroyMovingSmallBar();

	void DrawFace( const float fX_, const float fY_, const CKTDGUIControl::UITextureData& texData_, D3DCOLOR color_ /* = 0xffffffff */, const float fWidthPercent_ = 1.0f );

	virtual bool ProcessStateChangeUnitSelectWithOptionWindow();
	virtual bool ProcessStateChangeServerSelectWithOptionWindow();



private:
	//CKTDGUIDialogType					m_pDLGExitMsgBox;
	
	CX2BattleFieldGame*					m_pBattleFieldGame;
	D3DXVECTOR3							m_vOldEyePt;
	D3DXVECTOR3							m_vOldLookAtPt;
	bool								m_bNowMovingToOtherPlace;
	CKTDXCheckElapsedTime				m_TimerWaitingPortal;	
	CKTDGUIControl::UITextureData		m_TexDataMovingGageBG;
	CKTDGUIControl::UITextureData		m_TexDataMovingGage;
	CKTDGStateManager::KStateID			m_RenderStateID;	/// 인터페이스 출력을 위한 2D 출력함수
	//CX2Game::CONNECT_TYPE			m_ConnectType;

	//int								m_RenderCount;
#ifdef CAMERA_ZOOM_BY_MOUSE_WHEEL
	SHORT								m_SumDelta;		/// 마우스 휠 저장값
#endif //CAMERA_ZOOM_BY_MOUSE_WHEEL

#ifdef MONSTER_STATE_LIST_TEST
	MonsterStateListDialog m_MonsterStateListDialog;
#endif MONSTER_STATE_LIST_TEST
};

