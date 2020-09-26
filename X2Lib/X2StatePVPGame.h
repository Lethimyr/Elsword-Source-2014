#pragma once



class CX2GUUser;
class CX2StatePVPGame : public CX2StateOption
{
public:
	enum PVP_GAME_UI_CUSTOM_MSG
	{
		PGUCM_GAME_LOADING = 0,
		PGUCM_GAME_START,
		PGUCM_GAME_EXIT_OK,
		PGUCM_GAME_EXIT_CANCEL,
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

#ifdef SERV_PVP_NEW_SYSTEM
		LUI_UNKNOWN_COLOR,
		LUI_UNKNOWN_BLACK,
#endif

	};

	struct KillData
	{
#ifdef SERV_PVP_NEW_SYSTEM
		UidType		iUnitUid;
		CX2GameUnit *pGameUnit;
#else
		CX2Room::SlotData* pSlotData; 
		CX2GUUser* pUser;
#endif
		int killNum;
		int mdKillNum;

		KillData()
		{
#ifdef SERV_PVP_NEW_SYSTEM
			iUnitUid = 0;
			pGameUnit = NULL;
#else
			pSlotData = NULL;
			pUser = NULL;
#endif
			killNum = 0;
			mdKillNum = 0;
		}
	};


	struct HigherRank
	{
		CX2PVPRoom::PVP_GAME_TYPE m_ePVPType;

		HigherRank( CX2PVPRoom::PVP_GAME_TYPE ePVPType )
		{
			m_ePVPType = ePVPType;
		}

		bool operator()( const KillData& lhs, const KillData& rhs )
		{
			if( lhs.killNum > rhs.killNum )
				return true;
			else if( lhs.killNum < rhs.killNum )
				return false;

			if( lhs.mdKillNum > rhs.mdKillNum )
				return true;
			else if( lhs.mdKillNum < rhs.mdKillNum )
				return false;

#ifdef SERV_PVP_NEW_SYSTEM
			float fHPRate1 = lhs.pGameUnit->GetNowHp() / lhs.pGameUnit->GetMaxHp();
			float fHPRate2 = rhs.pGameUnit->GetNowHp() / rhs.pGameUnit->GetMaxHp();
#else
			float fHPRate1 = lhs.pUser->GetNowHp() / lhs.pUser->GetMaxHp();
			float fHPRate2 = rhs.pUser->GetNowHp() / rhs.pUser->GetMaxHp();
#endif

			switch( m_ePVPType )
			{
			case CX2PVPRoom::PGT_TEAM_DEATH:
			case CX2PVPRoom::PGT_SURVIVAL:
				{
					if( fHPRate1 <= 0.f )
						fHPRate1 = 1.f;

					if( fHPRate2 <= 0.f )
						fHPRate2 = 1.f;
				} break;
			}


			if( fHPRate1 > fHPRate2 )
				return true;
			else if( fHPRate1 < fHPRate2 )
				return false;

#ifdef SERV_PVP_NEW_SYSTEM
			if( NULL == lhs.pGameUnit || NULL == rhs.pGameUnit )
				return false;

			if( lhs.pGameUnit->GetUnitLevel() < rhs.pGameUnit->GetUnitLevel() )
				return true;
			else if( lhs.pGameUnit->GetUnitLevel() > rhs.pGameUnit->GetUnitLevel() )
				return false;
#else
			if( NULL == lhs.pSlotData->m_pUnit ||
				NULL == rhs.pSlotData->m_pUnit )
				return false;

			if( lhs.pSlotData->m_pUnit->GetUnitData().m_Level < rhs.pSlotData->m_pUnit->GetUnitData().m_Level )
				return true;
			else if( lhs.pSlotData->m_pUnit->GetUnitData().m_Level > rhs.pSlotData->m_pUnit->GetUnitData().m_Level )
				return false;

			if( lhs.pSlotData->m_Index < rhs.pSlotData->m_Index )
				return true;
			else if( lhs.pSlotData->m_Index > rhs.pSlotData->m_Index )
				return false;
#endif
			return false;
		}
	};

public:
	CX2StatePVPGame(void);
	~CX2StatePVPGame(void);

	virtual HRESULT		OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT		OnFrameRender();

	bool				MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual HRESULT		OnResetDevice();
	virtual HRESULT		OnLostDevice();


	virtual void PopTalkBox( UidType iUnitUID_, const WCHAR* pWstrMsg_, 
		const D3DXCOLOR& coTextColor_, const D3DXCOLOR& coOutColor_, const D3DXCOLOR& coBackColor_ );

	CKTDGUIDialogType		GetDlgScore(){return m_pDLGDeathTeamScore;}

#ifdef PLAY_EMOTION_BY_USER_SELECT
	virtual void PlayEmotionByUserSelect();
#endif // PLAY_EMOTION_BY_USER_SELECT

protected:
	virtual bool		UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		UIP2PEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void				UIFrameMove( double fTime, float fElapsedTime );

	void				FrameMoveInPlayMode( double fTime, float fElapsedTime );
	void				RankBoxFrameMove( double fTime, float fElapsedTime );
	void				SortByKillCount( CX2PVPRoom* pPVPRoom, vector<KillData>& vecSlotSorted );


	
	//방 정보 변경 관련
	bool				Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_LEAVE_ROOM_REQ( int leaveRoomReason = NetError::NOT_LEAVE_ROOM_REASON_00 );
	bool				Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_GAME_LOADING_TIME_OUT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_LEAVE_GAME_REQ();
	bool				Handler_EGS_LEAVE_GAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_LEAVE_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_CHANGE_TEAM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_STATE_CHANGE_RESULT_REQ();
	bool				Handler_EGS_STATE_CHANGE_RESULT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_STATE_CHANGE_RESULT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_STATE_CHANGE_UNIT_SELECT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	//게임 내부와 관련
	bool				GameLoadingReq( int percent );
	bool				GameLoadingAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				GameLoadingNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				GameLoadingAllUnitOkNot();

	bool				Handler_EGS_PLAY_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_STATE_CHANGE_GAME_INTRUDE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_INTRUDE_START_REQ();
	bool				Handler_EGS_INTRUDE_START_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_INTRUDE_START_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool				CurrentKillScoreNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_CURRENT_TEAM_KILL_SCORE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_REMAINING_PLAY_TIME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				PlayTimeOutNot(void);

	bool				MyUserUnitInfoToServerAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );

	bool				UserUnitRebirthPosReq( int startPosIndex );
	bool				UserUnitRebirthPosAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				UserUnitRebirthReq( UidType unitUID );
	bool				UserUnitRebirthAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				UserUnitRebirthNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );//살려준다는 메시지

	virtual bool		Handler_EGS_USER_UNIT_DIE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); //몇초후에 살아날것인가에 대해 받고, 게임 객체에서 자기 유닛이 죽었다는거 체크 후 몇초동안 서버에서 살려준다는 메시지가 날아올 때까지 기다림
	virtual bool		Handler_EGS_USER_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool		Handler_EGS_USER_UNIT_DIE_COMPLETE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool		Handler_EGS_END_GAME_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool				Handler_EGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//bool				Handler_EGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	//bool				Handler_EGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_END_GAME_PVP_RESULT_DATA_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_UPDATE_UNIT_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//bool				Handler_EGS_NOTIFY_MSG_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_ADD_ON_STAT_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool				Handler_EGS_ADD_ON_STAT_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool				Handler_EGS_CHANGE_PITIN_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ 허상형 : [2009/8/20] //	신학기 이벤트 대전방 패킷 관련
#ifdef NEW_TERM_EVENT
	bool				Handler_EGS_PVP_KILL_DIE_INFO_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif	//	NEW_TERM_EVENT
	//}} 허상형 : [2009/8/20] //	신학기 이벤트 대전방 패킷 관련

	//기타 컨트롤 함수
	void				OpenExitDLG();
	void				LoadUI();
	void				CreateGame();
	void				GameLoadingStart();

	virtual bool		QuitGame();

	virtual bool		ShortCutKeyProcess();
	
#ifdef DUNGEON_ITEM
	virtual bool Handler_EGS_GET_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CREATE_PVP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CREATE_PVP_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_PVP_ITEM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGE_PVP_ITEM_POS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_CHANGE_PVP_ITEM_POS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

#ifdef SERV_PVP_NEW_SYSTEM
	bool Handler_EGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ( UidType iUid, int iStartPos );
	bool Handler_EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ();
	bool Handler_EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif

	bool Handler_EGS_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	
	CKTDGUIDialogType					m_pDLGExitMsgBox;
	CKTDGUIDialogType					m_pDLGLoadingState;
	CKTDGUIDialogType					m_pDLGRankBox;	
	CKTDGUIDialogType					m_pDLGDeathTeamScore;
	bool							m_bIsOpenExitDLG;

	CKTDGUIDialogType					m_pDLGF1Help;



	CX2PVPGame*						m_pPVPGame;	
	bool							m_bLoadingOK;

	CX2GameLoadingTip*				m_pCX2GameLoadingTip;

	CX2Game::CONNECT_TYPE			m_ConnectType;
	int								m_LoadingPercent;
	int								m_LoadingRenderCount;

	bool							m_bResultStateReq;
	bool							m_bGameEnd;
	float							m_fGameEndWaitTime;

	vector< CKTDGUIDialogType >		m_DLGLoadingStateUnitInfoList;

	bool							m_bShowRankBox;
	bool							m_bStateChangingToResult;	


	int								m_iMyLastKillCount; // kill count sub quest 화면 표시하기 위해서
};
