#pragma once

/** @class CX2StateDungeonResult
	@brief Doxygen 주석 추가
	@date 2011/02/09
*/
class CX2StateDungeonResult : public CX2StateCommonBG
{
	public:
//{{ Public Enum, Struct Start
		/// 던전 결과 UI 사용자 지정 메시지
		enum DUNGEON_RESULT_UI_CUSTOM_MSG
		{
			DRUCM_CHOICE_ITEM_BOX = 0,			/// 
			DRUCM_GO_TRAINING_SCHOOL_OK,		/// 
			DRUCM_GO_TRAINING_SCHOOL_CANCEL,	/// 

			DRUCM_ESCAPE_DUNGEON_RESULT_OK,		/// 던전 결과창 이탈 팝업 확인
			DRUCM_ESCAPE_DUNGEON_RESULT_CANCEL,	/// 던전 결과창 이탈 팝업 캔슬
		};

		/// 던전 결과 애니메이션 목록
		enum DUNGEON_RESULT_ANIMATE_ORDER
		{
			DRAO_NONE = 0,						/// 없음
			DRAO_DUNGEON_PICTURE_BG,			/// 던전 그림 배경

			//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전 // #define SERV_INSERT_GLOBAL_SERVER 
			DRA0_DUNGEON_DEFENCE_RESULT,		/// 디펜스 던전에서 지켜진 엘의 수정 갯수
			//}} kimhc // 2011-04-18 // 국내, 디펜스 던전 // #define SERV_INSERT_GLOBAL_SERVER 

			DRAO_TOTAL_SCORE_BG,				/// 총점 배경
			DRAO_TIME_AND_DAMAGED_SCORE_BG,		/// 시간, 피해 점수 배경
			DRAO_COMBO_SCORE,					/// 콤보 점수
			DRAO_TECHNICAL_SCORE,				/// 기술 점수
			DRAO_TIME,							/// 시간
			DRAO_DAMAGED,						/// 피해
			DRAO_TOTAL_SCORE,					/// 총점
			DRAO_TOTAL_SCORE_RANK,				/// 총점 등급
			DRAO_OTHERS_RANK,					/// 타인 등급
			DRAO_WAIT_INFO_FADE_OUT,			/// 
			DRAO_FIRST_RESULT_INFO_FADE_OUT,	/// 
			DRAO_CHOICE_REWARD,					/// 보상 선택
			DRAO_ANIMATE_BOX,					/// 애니메이트 박스?
			DRAO_ED_BG,							/// ED(엘소드 돈) 배경
			DRAO_ITEM_BG,						/// 아이템 배경
			DRAO_EXP_BG,						/// 경험치 배경
			DRAO_ED,							/// ED(델소드 돈)
			DRAO_ITEM,							/// 아이템
			DRAO_EXP,							/// 경험치
			DRAO_BONUS_EXP,						/// 도장까지
			DRAO_EXP_BAR,						/// 경험치 막대기
// #ifdef SERV_DUNGEON_MVP
			DRAO_PICKED_MVP_PLAYER,				/// MVP Player 로 선정된 유저
// #endif // SERV_DUNGEON_MVP

			DRAO_END,
		};

		/// 등급 점수
		enum RANK_SCORE
		{
			RS_A = 0,	/// A
			RS_B,		/// B
			RS_C,		/// C
			RS_A_PLUS,	/// D
			RS_B_PLUS,	/// E
			RS_C_PLUS,	/// F
			RS_D,		/// D
			RS_E,		/// E
			RS_F,		/// F
			RS_S,		/// S
			RS_SS,		/// SS
			RS_DEAD,	/// DEAD
			RS_END,
		};

		/// 보너스 EXP 형태
		enum BonusEXPType
		{
			BET_CLEAR = 0,		/// 완료?
			BET_RANK,			/// 등급
			BET_PARTY,			/// 파티 플레이
			BET_MVP,			/// MVP
			BET_GAMEBANG,		/// PC 방
			BET_PREMIUM,		/// 프리미엄?
            BET_EVENT,			/// 이벤트
			BET_CHANNEL_BONUS,	/// 채널 보너스
		};

		/** @struct BonusEXPData
			@brief 보너스 경험치 형태와 경험치를 가짐
			@date 2011/02/09
		*/
		struct BonusEXPData
		{
			BonusEXPType bonusExpType;
			int	bonusExp;
		};
//{{ Public Enum, Struct End

	public:
//{{ Public Function Start
		CX2StateDungeonResult(void);
		virtual ~CX2StateDungeonResult(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual HRESULT OnFrameRender();

		//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
#ifdef	SERV_INSERT_GLOBAL_SERVER
		bool GetShowNumOfElCrystalsProtected() const { return m_bShowNumOfElCrystalsProtected; }
		void SetShowNumOfElCrystalsProtected( bool bShowNumOfElCrystalsProtected_ ) { m_bShowNumOfElCrystalsProtected = bShowNumOfElCrystalsProtected_; }
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-18 // 국내, 디펜스 던전

//{{ Public Function End

	protected:
//{{ Protected Function Start
		virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		virtual bool LastMsgByESC();

		//bool Handler_EGS_REFRESH_ROOM_REQ();
		//bool Handler_EGS_REFRESH_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_RESULT_SUCCESS_REQ();
		bool Handler_EGS_RESULT_SUCCESS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		virtual bool Handler_EGS_RESULT_ALL_UNIT_SUCCESS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_START_REWARD_BOX_SELECT_REQ();
		bool Handler_EGS_START_REWARD_BOX_SELECT_ACK();
		bool Handler_EGS_START_REWARD_BOX_SELECT_NOT();

		bool Handler_EGS_SELECT_REWARD_BOX_REQ( int boxNum );
		bool Handler_EGS_SELECT_REWARD_BOX_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_SELECT_REWARD_BOX_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool Handler_EGS_LEAVE_ROOM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_LEAVE_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		bool Handler_EGS_JOIN_ROOM_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

		bool InitUI();

		void UnitViewerProcess( CX2UnitViewerUI* pViewer );

		RANK_SCORE Get_UI_RANK_SCORE( CX2DungeonRoom::RANK_TYPE rankType );

		void ShowBonusEXPEvent( CX2StateDungeonResult::BonusEXPType bonusEXPType, int bonusEXP );

		virtual void PicCharFrameMove( double fTime, float fElapsedTime );
		void PicCharDrawText();

		bool GetPressEnterKeyOrZKeyForSkip() const { return m_bPressEnterKeyOrZKeyForSkip; }
		void SetPressEnterKeyOrZKeyForSkip(bool val) { m_bPressEnterKeyOrZKeyForSkip = val; }
		//}} Protected Function End

	protected:
//{{ Protected Variable Start
		CKTDGUIDialogType		m_pDLGBack;		/// 다이얼로그 배경
		CKTDGUIDialogType		m_pDLGFront;	/// 다이얼로그 전경
		CKTDGUIDialogType		m_pDLGMsgBox;	/// 다이얼로그 메시지 박스

		float				m_fPlayBGM;		/// 

		bool				m_bStartTimer;			/// 시작 타임머
		float				m_fRemainTime;			/// 지난 시간
		bool				m_bSendRefreshRoomReq;	/// 룸 갱신

		float				m_fTotalScore;			/// 총점

		float				m_fComboScore;			/// 콤보 점수
		float				m_fTechnicalScore;		/// 테크니컬 점수

		float				m_fHour;				/// 시간
		float				m_fMinute;				/// 분
		float				m_fSec;					/// 초

		float				m_fDamaged;				/// 피격 데미지


		DUNGEON_RESULT_ANIMATE_ORDER m_DungeonResultAniOrder;	/// 결과 애니메이트 목록

		bool				m_bInitUIFail;			/// UI 초기화 실패 관련


		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hParticleElMark;	/// ?
		CKTDGParticleSystem::CParticleEventSequenceHandle  m_hParticleRank;		/// 등급 파티클?

		CKTDXDeviceSound*					m_pSoundElMark;

	
		CKTDGPicChar*						m_pPicCharScore;				/// 점수 그림 문자
		CKTDGPicChar*						m_pPicCharEXP;					/// 경험치
		CKTDGPicChar*						m_pPicCharBonusEXP;				/// 보너스 경험치
		CKTDGPicChar*						m_pPicCharComboAndTechScore;	/// 콤보 및 테크니컬 점수
		CKTDGPicChar*						m_pPicCharED;					/// ED(엘소드 돈)

		bool								m_bAnimateScoreBG;				/// 애니메이트 점수 배경

		float								m_fStartWaitRemainTime;			/// 시작대기지난 시간
		bool								m_bUIAnimStart;					/// UI 애니메이션 시작

		float								m_fMyTotalRankWaitRemainTime;	/// 
		bool								m_bUIMyTotalRankStart;			/// 

		float								m_fWaitForFadeOutRemainTime;	/// 
		bool								m_bWaitForFadeOut;				/// 

		int									m_ChoiceItemID;					/// 선택 아이템 ID
		float								m_fED;							/// ED
		float								m_fEXP;							/// 경험치
		float								m_fBonusEXP;					/// 보너스 경험치
		float								m_fEXPSize;						/// 경험치 크기

		float								m_fAnimatBoxTime;				/// 애니메이트 박스 시간?

		bool								m_bSendStartRewardReq;			/// 
		bool								m_bStartRewardBoxSelectNot;		/// 

		vector< CKTDGXMeshPlayer::CXMeshInstanceHandle > m_vecRewardXMeshInst;
		vector< CKTDGParticleSystem::CParticleEventSequenceHandle  > m_vecBonusStamp;

		vector< BonusEXPData >				m_vecBonusExpType;
		float								m_fTimeBonusEXPShowEvent;
		bool								m_bCheckBonusEXP;

		bool								m_bSendedRegHack;
		//{{ kimhc // 2011-04-18 // 국내, 디펜스 던전
#ifdef	SERV_INSERT_GLOBAL_SERVER
		bool								m_bShowNumOfElCrystalsProtected;	/// 보호된 엘의 갯수를 출력 할 것인가? (디펜스 던전에서 승리한 경우만 출력)
#endif	SERV_INSERT_GLOBAL_SERVER
		//}} kimhc // 2011-04-18 // 국내, 디펜스 던전

		bool								m_bPressEnterKeyOrZKeyForSkip;		/// 스킵을 위해 Enter 또는 Z 키를 눌렀는지 여부

		//}} Protected Variable End
};
