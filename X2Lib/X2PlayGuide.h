#pragma once
class CX2PlayGuide
{
public:
	enum TUTORIAL_QUEST_ID
	{
		TQI_VISIT_RUBEN			= 11000, //수상한 움직임
		TQI_CHASE_THIEF			= 11005, //도적단 추적
		TQI_SAVE_EL				= 11010, //엘을 지켜라		
		TQI_VISIT_RUIN_OF_ELF	= 11030, //윌리엄 주제에 건방지다( 숲속의 폐허 입장)	
	};
	enum PLAY_GUIDE_TYPE
	{
		//가이드 추가 시 우선순위에 맞는 위치에 추가 하여야 합니다.
		PGT_NONE = -1,
		PGT_LEVEL_UP,				//캐릭터의 레벨이 @가 되었습니다.
		PGT_OBTAIN_SKILL_POINT,		//스킬 포인트를 @ 획득 하였습니다.
		PGT_DUNGEON_PLAY,			//'던전 시작' 버튼을 눌러 안내에 따라 던전에 입장하세요.
		PGT_QUEST_COMPLETE,			//@ 버튼으로 완료된 퀘스트를 확인하세요
		PGT_NOVICE_GUIDE_01,		//퀘스트 화살표 방향으로 이동하여 퀘스트를 수행하세요
		PGT_NOVICE_GUIDE_02,		//마르스를 혼내주고 도적단의 목표를 확인하세요.
		PGT_HYPER_MODE,				//@ 버튼으로 각성할 수 있습니다.
		PGT_SPECIAL_ATTACK,			//@,@,@,@ 버튼으로 스킬을 사용할 수 있습니다.
		PGT_GO_NEXT_STAGE,			//문을 파괴하여 다음 스테이지로 이동하세요.
		PGT_OBTAIN_EQUIP_ITEM,		//@' 버튼으로 '인벤토리'를 열어 획득한 장비를 확인할 수 있습니다.
		PGT_OBTAIN_TITLE,			//@' 버튼으로 '캐릭터창'을 열어 획득한 칭호를 장착할 수 있습니다.
		PGT_HAS_SKILL_POINT,		//@' 버튼으로 '스킬창'을 열어 획득한 스킬 포인트를 투자할 수 있습니다.
		PGT_COMPLETE_EPIC_QUEST,	//‘특정마을이름’ 으로 이동하세요.
		PGT_MAX_TYPE,				
	};

	enum GUIDE_PLACE_TYPE
	{
		GPT_NONE = -1,
		GPT_ALL,			//마을, 던전, 필드
		GPT_DUNGEON,		//던전
		GPT_DUNGEON_FIELD,	//던전, 필드
		GPT_VILLAGE_FIELD,	//마을, 필드
	};

	enum NOVICE_GUIDE_STEP
	{
		NGS_STEP_NONE	= -1,

		NGS_STEP_0,		//퀘스트 메뉴
		NGS_STEP_1,		//에픽퀘스트 탭
		NGS_STEP_2,		//에픽퀘스트 완료목록
		NGS_STEP_3,		//퀘스트 완료
		NGS_STEP_4,		//퀘스트 보상 완료
		NGS_STEP_5,		//던전도구
		NGS_STEP_6,		//엘의 나무
		NGS_STEP_7,		//던전 신청
		NGS_STEP_8,		//숲속의 폐허
		NGS_STEP_END,
	};

	enum EPIC_QUEST_GUIDE_STEP
	{
		EQGS_VISIT_NOAS = 0,	//노아스의 호수 입장 가이드
		EQGS_KILL_TOBU,			//토부 킬 가이드
		EQGS_ENTER_EL_TREE,		//엘의 나무 입장 가이드
		EQGS_ENTER_DUNGEON,		//던전 입장 가이드(공용)
		EQGS_KILL_THIEF_PPORU,	//도적 뽀루 킬 가이드
		EQGS_KILL_BORR,			//보르 킬 가이드
		EQGS_QMARK_GUIDE,		//Q마크를 따라 이동하여 퀘스트를 수행하세요
	};

	struct PlayGuideData
	{
		PLAY_GUIDE_TYPE		m_eGuideType;
		GUIDE_PLACE_TYPE	m_ePlaceType;
		int					m_eVillageID; //마을이동 가이드를 위한 값.

		wstring				m_GuideString;  // 기본문자열과 인자를 합친 최종 가이드
		bool				m_bShow;		 
		bool				m_bRepeat;		// 반복 출력 여부
		int					m_iStrID;		// 기본 문자열 
		int					m_iLimitLV;		// 출력 제한 레벨(최대 레벨)	

		PlayGuideData();
		void SetPlayGuide(CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType,	CX2PlayGuide::GUIDE_PLACE_TYPE ePlaceType,
			int iStrID, int iLimitLV, bool bRepeat = true);
	};

	class GuideOrderManager
	{
	public:
		void							InsertGuide(const PLAY_GUIDE_TYPE ePlayGuide_);
		void							EraseGuide(const PLAY_GUIDE_TYPE ePlayGuide_);
		void							SetEndCurrentGuide(){ m_bEndCurrentGuide = true;}
		bool							GetGuideListEmpty()const;
		void							ClearGuideList(){m_setGuideList.clear();}
		void							SetInvalideGuide(){ m_eCurrentGuide = PGT_NONE; }
		CX2PlayGuide::PLAY_GUIDE_TYPE	GetNextGuide();

		GuideOrderManager():m_eCurrentGuide(PGT_NONE),m_bEndCurrentGuide(false),m_iShowCount(0)
		{}

	private:
		std::set<PLAY_GUIDE_TYPE>	m_setGuideList;
		PLAY_GUIDE_TYPE				m_eCurrentGuide;
		bool						m_bEndCurrentGuide;
		int							m_iShowCount;
	};
public:
	CKTDGUIDialogType	GetDlgPlayGuide() {return m_pDLGPlayGuide;}	
	PLAY_GUIDE_TYPE		GetCurrentPlayGuide(); //현재 활성화 된 가이드 얻기
	bool				GetShowGuideByPlaceType( GUIDE_PLACE_TYPE ePlaceType_);
	void SetPlayGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow, int iVal = -1);
	void SetPlayGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow, wstring str1, wstring str2 = L"", wstring str3 = L"", wstring str4 = L"" );
	void SetVillageID(int eVillageID);
	void SetHide(bool bVal){ m_bHide = bVal; }
	int GetVillageID(){return m_PlayGuide[PGT_COMPLETE_EPIC_QUEST].m_eVillageID;}
	void OnFrameMove( double fTime, float fElapsedTime );
	void ClearPlayGuide();

//튜토리얼
	void SetMouseClickIndex(int val);
	void ResetNoviceGuide();
	UINT GetGuideStep()const {return m_uiCurrentGuideStep;}
	bool GetActiveNoviceGuide()const { return (false == m_bCompleteTutorial) ? m_bActiveNoviceGuide : false;  }
	bool GetShowDungeonMenu()const { return (true == m_bCompleteTutorial) ? true : m_bShowDungeonMenu;}
	void SetCompleteTutorial(bool bVal){m_bCompleteTutorial = bVal;}
	void SetShowDungeonMenu(bool bVal){m_bShowDungeonMenu = bVal;}


	void DeleteQuestGuideParticle();
	void DeleteInputGuideParticle();

	void StartGoEffect();
	void StopGoEffect();
#ifdef GUIDE_FIRST_EPIC_QUEST
	void SetFirstEpicQuestGuide( EPIC_QUEST_GUIDE_STEP GuideStep_);
	void SetShowFirstEpicQuestGuide(bool bShow_);
#endif //GUIDE_FIRST_EPIC_QUEST
	void SetShowPlayGuide(bool bVal_);

	CX2PlayGuide(void);
	~CX2PlayGuide(void);

private:
	void SetShowGuide( CX2PlayGuide::PLAY_GUIDE_TYPE eGuideType, bool bShow);
	bool SetGuideString(PLAY_GUIDE_TYPE eGuideType, int iVal);
	bool SetGuideString(PLAY_GUIDE_TYPE eGuideType, wstring str);
	bool SetGuideString(PLAY_GUIDE_TYPE eGuideType, wstring str1, wstring str2, wstring str3, wstring str4);

	bool IsShowGuide();

	void OnFrameMove_SetAlpha( float fElapsedTime );

#ifdef GUIDE_FIRST_EPIC_QUEST
	void OnFrameMove_FirstEpicGuide(float fElapsedTime);
#endif //GUIDE_FIRST_EPIC_QUEST

	//{{튜토리얼 던전에서 사용되는 퀘스트 가이드 
	void OnFrameMove_Tutorial();
	void CreateQuestGuideParticle();
	void SetShowGuideParticle(bool bVal);
	void CreateInputGuide();
	void SetPostionInputGuide(const D3DXVECTOR3& vPos);
	//}}튜토리얼 던전에서 사용되는 퀘스트 가이드 


	
	CKTDGUIDialogType	m_pDLGPlayGuide;			// 가이드 다이얼로그 객체
	PlayGuideData		m_PlayGuide[PGT_MAX_TYPE];  // 각각의 가이드에대한 정보
	float				m_fOnTime;					// 깜빡임 용 출력 시간
	bool				m_bHide;					
	GuideOrderManager	m_GuideOrderManager;		// 다음 출력해야할 가이드 관리자
	PLAY_GUIDE_TYPE		m_eOldPlayGuide;			// 이전 출력된 가이드


	UINT				m_uiGuideStep;
	UINT				m_uiCurrentGuideStep;
	bool				m_bStart;
	bool				m_bActiveNoviceGuide;		// 초보자 가이드 활성화 여부
	bool				m_bCompleteTutorial;		// 튜토리얼 완료 여부
	bool				m_bShowDungeonMenu;			// 던전도구 활성화 여부(11010퀘스트를 완료하기 전엔 비활성화)


	//{{튜토리얼 던전에서 사용되는 퀘스트 가이드 
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hIndicator;
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hIndicatorMark;
	CKTDGUIDialogType									m_pDLGParticle;
	bool												m_bShowGuideParticle;


	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hInputGuide;
	//}}튜토리얼 던전에서 사용되는 퀘스트 가이드 


	CKTDGParticleSystem::CParticleEventSequenceHandle	m_hParticleGo;//오른쪽 이동 진행 가이드


#ifdef GUIDE_FIRST_EPIC_QUEST
	//도적단 추적, 엘을 지켜라 퀘스트에 대한 초보자 가이드 용 
	CKTDGUIDialogType									m_pDLGUIFirstEpicQuestGuide;
	bool												m_bShowFirstEpicQuestGuide;
	float												m_fEpicGuideDelayTime;
	float												m_fEpicGuideAlphaTime;
#endif //GUIDE_FIRST_EPIC_QUEST

};



