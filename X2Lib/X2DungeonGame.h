#pragma once


#include "./X2TrainingGameUI.h"

/** @class	CX2DungeonGame
	@brief  던전 게임 관련 클래스
	@date
*/
class CX2DungeonGame : public CX2Game
{
	public:
//{{ Public enum, struct Start
		/** @struct	DungeonResultUnitInfo
			@brief  던전 결과 유닛 정보
			@date
		*/

	
		struct DungeonResultUnitInfo  
		{
			UidType	m_UnitUID;			/// 유닛 ID

			bool	m_bIsDie;			/// 생사 확인
			int		m_nTotalScore;		/// 총점
			int		m_nComboScore;		/// 콤보 점수
			int		m_nTechnicalScore;	/// 테크니컬 점수
			int		m_nDamaged;			/// 피격 카운트
			int		m_nNPCKillNum;		/// 죽인 몬스터 수
			int		m_nED;				/// 획득 ED
			int		m_nEXP;				/// 획득 경험치

			//{{ kimhc // 실시간 엘소드 중 실시간 ED 및 경험치 획득
#ifdef	REAL_TIME_ELSWORD
			int		m_nOldED;			// 던전에 들어올때 가지고 있던 ED의 양
			int		m_nOldEXP;			// 던전에 들어올때 가지고 있던 경험치
#endif	REAL_TIME_ELSWORD
			//}} kimhc // 실시간 엘소드 중 실시간 ED 및 경험치 획득


			int		m_nClearBonusEXP;		/// 완료 보너스 경험치
			int		m_nRankBonusEXP;		/// 랭크 보너스 경험치
			int		m_nPartyBonusEXP;		/// 파티 보너스 경험치
			int		m_nMvpBonusEXP;			/// MVP 보너스 경험치
			int		m_nGameBangBonusEXP;	/// PC 방 보너스 경험치
			int		m_nPremiumBonusEXP;		/// 프리미엄 보너스 경험치
            int     m_nEventBonusExp;		/// 이벤트 보너스 경험치
			int		m_nChannelBonusEXP;		/// 채널 보너스 경험치

			float	m_fRankBonus;	/// 랭크 보너스
			float	m_fPartyBonus;	/// 파티 보너스


#ifdef SERV_DUNGEON_MVP
			bool	m_bIsMVPPlayer;			// MVP Player 인가?
#endif // SERV_DUNGEON_MVP

			CX2DungeonRoom::RANK_TYPE	m_cComboRank;		/// 콤보 랭크
			CX2DungeonRoom::RANK_TYPE	m_cTechnicalRank;	/// 테크니컬 랭크
			CX2DungeonRoom::RANK_TYPE	m_cDamagedRank;		/// 데미지 랭크
			CX2DungeonRoom::RANK_TYPE	m_cTimeRank;		/// 시간 랭크
			CX2DungeonRoom::RANK_TYPE	m_cTotalRank;		/// 총 랭크
				

            DungeonResultUnitInfo();		/// 생성자
            int GetTotalExp();				/// 총 경험치 얻기
		};

		/** @struct	DungeonResultInfo
			@brief	던전 결과 정보 
			@date
		*/
		struct DungeonResultInfo
		{
			int									m_Sec;							/// 시간
			bool								m_bWin;							/// 성공 여부
			vector< DungeonResultUnitInfo* >	m_DungeonResultUnitInfoList;	/// 던전 결과 유닛 정보 목록

// 			std::vector<KArcadeRankingInfo>		m_vecTodayRankingInfo;			/// 오늘 랭킹 정보
// 			KArcadeRankingInfo					m_kTopRankingInfo;				/// 최고 랭킹 정보
// 			KArcadeRankingInfo					m_kResultRankingInfo;			/// 결과 랭킹 정보
// 			KArcadeRankingInfo					m_kMyTopRankingInfo;			/// 자신의 최고 랭킹 정보
//			int									m_iRank;						/// 랭크
			//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
#ifdef	SERV_INSERT_GLOBAL_SERVER
			unsigned char						m_ucNumOfProtectedSmallCrystals;	/// 보호된 작은 크리스탈 수
#endif	SERV_INSERT_GLOBAL_SERVER
			//}} kimhc // 2011-04-18 // 국내, 디펜스 던전

			DungeonResultInfo( KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kPacket );
			~DungeonResultInfo();
			
#ifdef	SERV_DUNGEON_MVP
			UidType GetMvpPlayerUID ();
#endif // SERV_DUNGEON_MVP

		};
//}} Public enum, struct End
	
	public:
//{{ Public Function Start
		CX2DungeonGame(void);
		virtual ~CX2DungeonGame(void);
		
		virtual void			Release();
		virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT			OnFrameRender();

		virtual bool			MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		
		virtual void			GameLoading( CX2Room* pRoom );
		virtual void			GameStart();

		virtual void			StageLoading( int stageNum );
		virtual void			StageStart();

		virtual void			SubStageOpen( int subStageNum );
		virtual void			SubStageLoading( int subStageNum );
		virtual void			SubStageStart();

		CX2Dungeon*				GetDungeon() { return m_pDungeon; }		

		virtual bool			DetermineLastKill();	

		int						GetWinTeam() { return m_WinTeam; }
		void					SetWindTeam( CX2Room::TEAM_NUM winTeam ) { m_WinTeam = winTeam; }
        virtual					void ResurrectOtherUser();
		

		virtual bool			Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK );
		virtual bool			Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT );

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_REQ( int iClearConditionIndex );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_CLEAR_ACK( KEGS_DUNGEON_SUB_STAGE_CLEAR_ACK& kEGS_DUNGEON_SUB_STAGE_CLEAR_ACK );
#endif // SERV_STAGE_CLEAR_IN_SERVER

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_OPEN_REQ( int subStageNum );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_OPEN_ACK( KEGS_DUNGEON_SUB_STAGE_OPEN_ACK& kEGS_DUNGEON_SUB_STAGE_OPEN_ACK );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_OPEN_NOT( KEGS_DUNGEON_SUB_STAGE_OPEN_NOT& kEGS_DUNGEON_SUB_STAGE_OPEN_NOT );

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ( int beforeStage, int nextStage );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK& kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ACK );
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT& kEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT );

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ();
		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK( KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK& kEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_ACK );

		virtual bool			Handler_EGS_DUNGEON_SUB_STAGE_START_NOT( KEGS_DUNGEON_SUB_STAGE_START_NOT& kEGS_DUNGEON_SUB_STAGE_START_NOT );

//		bool					StateChangeResultNot();
		bool					Handler_EGS_END_GAME_DUNGEON_RESULT_DATA_NOT( KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kPacket );


		virtual bool			Handler_EGS_STATE_CHANGE_RESULT_NOT( KEGS_STATE_CHANGE_RESULT_NOT& kEGS_STATE_CHANGE_RESULT_NOT );


		//virtual bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ( UidType unitUID, bool bAllowUseCash = false );
		//virtual bool		    Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_ACK );
		virtual	bool			Handler_EGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT( KEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT& kEGS_RESURRECT_TO_CONTINUE_DUNGEON_NOT );

		virtual bool			Handler_EGS_REMAINING_PLAY_TIME_NOT( KEGS_REMAINING_PLAY_TIME_NOT& kEGS_REMAINING_PLAY_TIME_NOT );

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
//{{ oasis907 : 김상윤 [2010.5.6] // 
		bool					Handler_EGS_DUNGEON_EFFECT_REMAINING_TIME_NOT( KEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT& kEGS_DUNGEON_EFFECT_REMAINING_TIME_NOT );
		bool					Handler_EGS_DUNGEON_EFFECT_TIME_OUT_NOT( KEGS_DUNGEON_EFFECT_TIME_OUT_NOT& kEGS_DUNGEON_EFFECT_TIME_OUT_NOT );

//}}
#endif DUNGEON_TIME_OUT_FRAMEMOVE
		bool					GetShowTutorialUI() { return m_bShowTutorialUI; }
		void					TextOutTutorial_LUA( int nMsg, float fTime=3.f );
		bool					GetIsThereTutorialMessage() { return m_bIsThereTutorialMsg; }		
		void					SetIsThereTutorialMessage(bool val) { m_bIsThereTutorialMsg = val; }
		bool					IsTutorialMessage_LUA( int nMsg )
		{
			if( 0 == m_wstrTutorialMsg.compare( GET_STRING( nMsg ) ) )
				return true;
			else 
				return false;
		}

		void					ShowSubStageOpenGuide();
		/*void					SetIsSendResurrectionReq( bool bCheck ) { m_bIsSendResurrectionReq = bCheck; }*/

#ifdef GET_DUNGEON_ID_IN_LUA
		int						GetDungeonID();
#endif GET_DUNGEON_ID_IN_LUA
		int						GetDungeonType();
		int						GetGameLevel() { return (int)m_pDungeonRoom->GetDifficulty(); }
		void					ClearSubStage( int clearType, int nextStageNum, int nextSubStageNum );
		
		
		// 훈련소 관련 함수
		bool 					GetStartRealTraining() const { return m_bStartRealTraining; }	
		void 					SetStartRealTraining(bool val);
		void					CreateTrainingGameUI( CX2State* pNowState );
		CX2TrainingGameUI*		GetTrainingGameUI() const { return m_pTrainingGameUI; }


		float					GetRemainPlayTime() const { return m_fRemainPlayTime; }
		void					SetRemainPlayTime(float val) { m_fRemainPlayTime = val; }


		bool 					GetEnableRemainTime() const { return m_bEnableRemainTime; }
		void 					SetEnableRemainTime(bool val) { m_bEnableRemainTime = val; }

		void					ShowBossName_LUA( char* pFileName );
		void					ShowBossName( const WCHAR* wszFileName );

		

		//bool					UseQuickSlot();
		void					SetHideDungeonPlayTimeAfterLoading(bool val) { m_bHideDungeonPlayTimeAfterLoading = val; }
		void					ResetUnitPositionAtTrainingGame();

		void					SetClearEffect( float fDelayTime ){ m_bShowClear = true; m_fClearEffDelayTime = fDelayTime; }

		CKTDXTimer*				GetTimerManyPurpose1() { return &m_TimerManyPurpose1; }
		CKTDXTimer*				GetTimerManyPurpose2() { return &m_TimerManyPurpose2; }

		bool					GetKilledBoss() const { return m_bKilledBoss; }

		// UNIT_TYPE 별 게임유저 정보를 찾아주기..
		//bool					GetUserUnitListByUnitType( UNIT_TYPE unitType, std::vector<CX2GUUser*>& vecGUUserList );

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE
		//{{ oasis907 : 김상윤 [2010.4.26] // 
		void					CreateDamageDataForDungeonTimeOut();
		void					DungeonTimeOutFrameMove( double fTime, float fElapsedTime );
		//}}
#endif DUNGEON_TIME_OUT_FRAMEMOVE

		//{{ 허상형 : [2011/3/4/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
		float					GetPlayTimeLimit();
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 허상형 : [2011/3/4/] //	월드 미션
#ifdef COME_BACK_REWARD
		void					CloseDungeonName( float fTime );
#endif

		bool					IsNearPortalLineMap();
#ifdef MODIFY_DUNGEON_STAGING
		void					SetShowMonsterIndicator(bool bVal){ if( NULL != m_pMonsterIndicator) m_pMonsterIndicator->SetShow(bVal);}
#endif //MODIFY_DUNGEON_STAGING

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		
		virtual bool			Send_EGS_DEFENCE_DUNGEON_WAVE_ID_NOT( const int iDungeonWaveID_ );	
		bool					Send_EGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT ( const int iStageIndex_, const int iSubStageIndex_ );		// 유저의 해당 서브 스테이지 콤보, 테크닉 점수를 서버에 전송한다.
		bool					Handler_EGS_BAD_ATTITUDE_USER_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// 불량 유저의 UID 를 전송한다.		
#endif // SERV_DUNGEON_FORCED_EXIT_SYSTEM

#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
		virtual void AbuserUserCheck( const CX2GUUser* pCheckUser_ ) {}	/// 던전에서는 작동 안함
#endif	SERV_FIX_SYNC_PACKET_USING_RELAY

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		void CreateAllyEventMonster();
		void ReCreateAllyEventMonster( UidType EventOwnUnitUID );
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

	protected:
//{{ Protected Function Start
		void					DungeonLoading(bool bIsNpcLoad = false);

		
		void					ShowClearEffect();
		virtual void			UpdateUnitPointer();


		void					CountTimeSubStageFrameMove( double fTime, float fElapsedTime );
		void					TutorialFrameMove( double fTime, float fElapsedTime );
		
		bool					GetResurrectionOperationCond();

		bool					DetermineShowDungeonPlayTime();

#ifdef EXTRACT_ALL_DUNGEON_NPC_INFO
		void					ExtractAllDungeonNPCInfo_DevOnly( const char* szFileName );
#endif EXTRACT_ALL_DUNGEON_NPC_INFO
//}} Protected Function End
		
#ifdef X2TOOL
	public:
		void ChangeStage(int iNextStage, int iNextSubStage = 0);
#endif



	protected:
//{{ Protected Variable Start
		CX2DungeonRoom*             m_pDungeonRoom;	/// 던전 ID와 Difficulty만 얻는것 빼고는 X2Game의 Room을 사용 하면 되기 때문에 없어도 될 듯...
		CX2Dungeon*					m_pDungeon;	/// 그냥 인스턴스로 해도 될지...

		CX2Room::TEAM_NUM			m_WinTeam;	/// X2PVPGame에도 존재

		bool						m_bGoNextPacketSended;	/// EGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ 를 전송할 타이밍을 체크할 용도로 사용
		
		bool						m_bShowClear;		/// Clear 이펙트를 보여줄 것인가?
		float						m_fClearEffDelayTime;	/// 보여준다면 몇초 후에 보여줄 것인가?

#ifdef DUNGEON_TIME_OUT_FRAMEMOVE	// oasis907 : 김상윤 [2010.4.26] // 
		bool							m_bShowDungeonClearLimitTimeOut; /// 시간 제한 던전에서 타이머를 표시할지 // FieldFix: 초기화가 안되어 있음
		float							m_fDungeonClearLimitTimeOut;	/// 남은시간 출력용
		bool							m_bDungeonClearLimitTimeOut; /// 시간 제한 던전에서 제한 시간을 넘어섰는지
		CX2DamageManager::DamageData*	m_pDamageDataForDungeonTimeOut;	/// 시간 제한을 넘겼을 때 나오는 독 데미지
#endif DUNGEON_TIME_OUT_FRAMEMOVE
		bool						m_bCountTimeLeftAfterSubStageOpen;	/// 서브스테이지 진행 경고 남은 시간을 보여줄 것인지 여부
		float						m_fTimeLeftAfterSubStageOpen;		/// 서브스테이지 진행 경고 남은 시간 (보통 20초)
		CKTDGFontManager::CUKFont*	m_pFontForTime;		// 다음 스테이지, 던전 타임 아웃등 시간 표시용
		//CKTDGFontManager::CUKFont*	m_pFontForQuest;

		bool						m_bIsSendSkipContinueNot;	/// 사용안함
		
		// 튜토리얼
		bool						m_bIsThereTutorialMsg;		/// 튜토리얼 메시지가 있는지 여부
		CKTDGFontManager::CUKFont*	m_pFontForTutorialMsg;		/// 튜토리얼 메시지를 나타내는데 사용
		wstring						m_wstrTutorialMsg;			/// 튜토리얼 메시지
		float						m_fTimeLeftForTutorialMessage;	/// 튜토리얼 메시지 유지 시간 (-1이면 계속 보이게 되는 듯...)
		D3DXCOLOR					m_coTutorialMsg;			/// 튜토리얼 메시지의 칼라
		D3DXCOLOR					m_coTutorialMsgIncrement;	/// 튜토리얼 메시지 칼라의 증감 값
		int							m_iTutorialMsgFlashCount;	/// 튜토리얼 메시지를 여러번 flash 하는것처럼 count라고 되어있으나 실제로는 한번밖에 안하는 듯... (fade out->in)
		bool						m_bShowTutorialUI;	// 튜토리얼 UI를 보여줄 것인지 여부

		bool						m_bEnableRemainTime;	/// true면 m_fRemainPlayTime이 점점 감소 된다. (TrainingGame에서만 사용되는 듯)
		float						m_fRemainPlayTime;		/// 제한시간이 있는 던전의 남은시간..불이익을 주는데 사용되지는 않는듯, TrainingGame과 DefenceDungeon에서 사용되는듯(DungeonRoom을 사용하지 않으려면 변경되지 않을 값도 필요할 듯..)

		/// 훈련소 관련 (지금은 훈련소가 없는데...), 자유훈련도 TrainingGame을 사용하고 있지만 잘 구별해야 할 듯
		bool						m_bStartRealTraining;	/// 훈련소 시작 REQ를 날라기위한 플래그
		CX2TrainingGameUI*			m_pTrainingGameUI;		/// 훈련소 UI

		bool						m_bShowDungeonRemainTime;	/// m_fRemainPlayTime을 표시할 것인지.. (시간 제한이 있는 던전의 경우 표
		bool						m_bShowDungeonPlayTimeForQuest;		/// 헤니르 또는 던전 시간 제한 퀘스트를 가지고 있는 경우 던전 진행 시간을 표시
		bool						m_bHideDungeonPlayTimeAfterLoading; /// 스테이지 로딩후 잠깐 동안은 던전 플레이 시간 표시하지 않도록
#ifdef SHOW_REMAIN_TIME_IN_CLEAR_CONDITION
		bool						m_bShowStageReaminTime;
#endif SHOW_REMAIN_TIME_IN_CLEAR_CONDITION

		CKTDXTimer					m_TimerManyPurpose1;	/// 사용 안함(아케이드에서만 사용했었음)
		CKTDXTimer					m_TimerManyPurpose2;	/// 사용 안함(아케이드에서만 사용했었음)

		CX2MonsterIndicator*		m_pMonsterIndicator;	/// 던전에서만 사용하기로 함

		bool						m_bKilledBoss;		/// 던전의 보스를 죽였는지
		
#ifdef CHECK_SUB_STAGE_GO_NEXT
		KProtectedType<bool>		m_bCheckSubStageGoNext;	/// 해킹 체크용
#endif

#ifdef COME_BACK_REWARD
		CKTDGUIDialogType			m_pDLGTitle;	/// 휴면복귀, 혈맹의 끈등에서 사용하고 있으나... 이 구조를 바꾸어야 할듯..
		float						m_fDungeonNameTime;	/// m_pDLGTitle 제거용...
#endif
	
		CKTDGParticleSystem::CParticleEventSequenceHandle m_hBossName;

#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		int							m_iDefenceDungeonWaveID;		// 어둠의 문 서브스테이지( Wave )
#endif //SERV_DUNGEON_FORCED_EXIT_SYSTEM




//}} Protected Variable End		

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
	public:
		bool IsDungeonEscape() { return m_bEscape; }

	private:
		bool m_bEscape;
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK
};
