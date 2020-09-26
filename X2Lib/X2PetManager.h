#ifdef SERV_PET_SYSTEM

#pragma once

class CX2PET;

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
struct  CX2PetManager_PetInitTemplate;
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD


class CX2PetManager : public CX2PopupUIBase
{	
public:

	enum PET_UNIT_ID
	{
		PUI_NONE			= 0,
		PUI_TEST,
		PUI_PETTE_PPORU,
		PUI_PETTE_ANCIENT_PPORU,
		PUI_TREE_KNIGHT,
		PUI_PETTE_PPORU_EVENT,
		PUI_PETTE_ANCIENT_PPORU_F,
		PUI_TREE_KNIGHT_F,
		PUI_HOLLO_WITCH,
		PUI_HOLLO_WITCH_F,
		PUI_XMAS2010_PPORU,
		PUI_MYSTIC_NORM,
		PUI_MYSTIC_BLK,
		PUI_MYSTIC_WHT,
		PUI_MYSTIC_RED,
		PUI_HATCHLING_NORM,
		PUI_HATCHLING_DARK,
		PUI_HATCHLING_FIRE,
		PUI_HATCHLING_LIGHT,
		PUI_PETIT_ANCIENT_HALLOW_DARK,
		PUI_PETIT_ANCIENT_HALLOW_FLAME,
		PUI_PETIT_ANCIENT_HALLOW_SPOOK,
		PUI_PETIT_ANCIENT_HALLOW_SPRITE,
		PUI_PETIT_PPORU_BOOMHILL,
		PUI_TREE_KNIGHT_XMAS2011,
		PUI_TREE_KNIGHT_SANTA2011,
		PUI_PETIT_LIGHTBRINGER_UNIEL,
		PUI_PETIT_DARKBRINGER_UNIRING,
		PUI_PET_SILVERFOX_MIHO,			/// 은여우 미호
		PUI_PET_FIREFOX_SHIHO,			/// 불여우 시호
		PUI_PET_DARKFOX_NAHO,			/// 어둠여우 나호
		PUI_PET_BLOOD_EATER,			/// 블러드 이터
		PUI_PET_MANA_EATER,				/// 마나 이터
		PUI_PET_MILLENNIAFOX_EUN,		/// 천년여우 은

		PUI_PETIT_PPORU_HATS_ON,	/// 쁘띠 뽀루 햇츠온
		PUI_STINKO_HATS_ON,			/// 스팅코 햇츠온
		PUI_EBONY_HATS_ON,			/// 에보니 햇츠온

		PUI_PPORU_HOLLOWEEN_EVENT,		/// 할로윈 이벤트 펫 마녀 뽀루

		PUI_GRIM_REAPER_DEATH				= 38, //그림 리퍼-데스(사신 펫)
		PUI_GRIM_REAPER_REBIRTH				= 39, //그림 리퍼-리버스(사신 펫)

		PUI_RAINCOAT_CAT_BLACK_KR           = 40, /// 우비 입은 고양이 블랙
		PUI_RAINCOAT_CAT_YELLOW_KR          = 41, /// 우비 입은 고양이 엘로우

		PUI_PPORU_NAVER_EVENT				= 42, /// 네이버 채널링 이벤트 뽀루

		// 해외팀 pet
		PUI_PANDA_STICK_RED					= 96,
		PUI_PANDA_STICK_BLUE				= 97,
		PUI_PANDA_STICK_RED_F				= 98,
		PUI_PANDA_STICK_BLUE_F				= 99,
		PUI_PETTE_ANCIENT_PPORU_SHADOW		= 100, 
		PUI_GAMANIA_PETTE_PPORU				= 101,
		PUI_PETTE_ANCIENT_PPORU_SHADOW_F	= 102,
		PUI_TREE_KNIGHT_SPRING				= 110,
		PUI_TREE_KNIGHT_FALL				= 111,
		PUI_TREE_KNIGHT_WINTER				= 112,
		PUI_TREE_KNIGHT_SPRING_F			= 113,
		PUI_TREE_KNIGHT_FALL_F				= 114,
		PUI_TREE_KNIGHT_WINTER_F			= 115,
		PUI_HATCHLING_FIRE_F				= 116,
		PUI_HATCHLING_LIGHT_F				= 117,
		PUI_HATCHLING_STAR_F				= 118,
		PUI_HATCHLING_NORM_F				= 119,
		PUI_HATCHLING_DARK_F				= 120,
		PUI_CHINAJOY_PPORU_EVENT			= 121,
		PUI_HATCHLING_BLUE					= 122,
		PUI_DEATH_SCYTHE					= 123,
		PUI_DEATH_SCYTHE_F					= 124,
		PUI_PANDA_ROYAL_RED					= 125,
		PUI_PANDA_ROYAL_WHITE				= 126,
		PUI_MYSTIC_NORM_F					= 127,
		PUI_MYSTIC_BLK_F					= 128,
		PUI_MYSTIC_WHT_F					= 129,
		PUI_MYSTIC_RED_F					= 130,	
		PUI_MYSTIC_BLUE                     = 131,	/// 미스틱 블루
		PUI_MYSTIC_BLUE_F                   = 132,	/// 미스틱 블루(완전체)
		PUI_PETIT_LIGHTBRINGER_UNIEL_F      = 133,	/// 유니엘 완전체 펫
		PUI_SHERIFF_NORM                    = 134,	/// 미국 보안관 펫 ( 갈색 )
		PUI_SHERIFF_WHITE                   = 135,	/// 미국 보안관 펫 ( 흰색 )
		PUI_SHERIFF_NORM_F                  = 136,	/// 미국 보안관 펫 ( 갈색 완전체 )
		PUI_SHERIFF_WHITE_F                 = 137,	/// 미국 보안관 펫 ( 흰색 완전체 )
		PUI_PETIT_DARKBRINGER_UNIRING_F		= 138,	/// 유니링(완전체)
		PUI_PET_BLOOD_EATER_EVENT			= 139,	/// 대만 이벤트용 블러드 이터
		PUI_HATCHLING_BLUE_F                = 140,	///	유럽 영국 CBT기념 야생 해츨링 완전체
		PUI_PETTE_BERTHE					= 141,  /// 베르드 Jr.
		PUI_PETTE_BERTHE_F					= 142,  /// 베르드 Jr. 완전체
		

		PUI_PET_SILVERFOX_MIHO_F            = 30000, /// 은여우 미호 완전체
		PUI_PET_FIREFOX_SHIHO_F             = 30001, /// 불여우 시호 완전체
		PUI_PET_DARKFOX_NAHO_F              = 30002, /// 어둠여우 나호 완전체
		PUI_PET_MANA_EATER_EVENT_INT        = 30003, /// 대만 마나 이터
		PUI_PET_MERCENARY_PPORU_EVENT_INT   = 30004, /// 미국 용병 뽀루
		PUI_RAINCOAT_CAT_BLACK              = 30005, /// 우비 입은 고양이 블랙		PUI_RAINCOAT_CAT_YELLOW             = 30006, /// 우비 입은 고양이 엘로우		PUI_RAINCOAT_CAT_BLACK_F            = 30007, /// 우비 입은 고양이 블랙 완전체		PUI_RAINCOAT_CAT_YELLOW_F           = 30008, /// 우비 입은 고양이 엘로우 완전체
		PUI_EVENT_EAT_FOOD_PPORU_F          = 30009, /// 태국 인도네시아 쁘띠 뽀루
		PUI_HOLLO_WITCH_EVENT_F				= 30010, /// 이벤트용 할로위치 완전체
		PUI_PETTE_PPORU_EVENT_BRAZIL		= 30011, /// 이벤트용 브라질 뽀루
		PUI_HALLOWEEN_PUMPKIN_TANYA			= 30012, /// 할로윈 호박 요정 타냐
		PUI_HALLOWEEN_PUMPKIN_LAEL			= 30013, /// 할로윈 호박 요정 라엘
		PUI_HALLOWEEN_PUMPKIN_LOJETA		= 30014, /// 할로윈 호박 요정 로제타
		PUI_HALLOWEEN_PUMPKIN_NARENEA		= 30015, /// 할로윈 호박 요정 나르네아
		PUI_HALLOWEEN_PUMPKIN_ELDENA		= 30016, /// 할로윈 호박 요정 엘데나 (레어)
		PUI_HALLOWEEN_PUMPKIN_TANYA_F		= 30017, /// 할로윈 호박 요정 타냐 완전체
		PUI_HALLOWEEN_PUMPKIN_LAEL_F		= 30018, /// 할로윈 호박 요정 라엘 완전체
		PUI_HALLOWEEN_PUMPKIN_LOJETA_F		= 30019, /// 할로윈 호박 요정 로제타 완전체
		PUI_HALLOWEEN_PUMPKIN_NARENEA_F		= 30020, /// 할로윈 호박 요정 나르네아 완전체
		PUI_HALLOWEEN_PUMPKIN_ELDENA_F		= 30021, /// 할로윈 호박 요정 엘데나 (레어) 완전체
		PUI_SONOKONG						= 30022, /// 손오공
		PUI_SONOKONG_F						= 30023, /// 손오공 완전체
		PUI_FIREWORK_PPORU					= 30024, /// 연말 이벤트 불꽃 뽀루
		PUI_PET_SNOWBUMP					= 30025, /// 눈사람
		PUI_PET_HUNTER_PENGUIN				= 30026, /// 팽귄
		PUI_TREE_KNIGHT_EVENT_F				= 30027, /// 트리 나이트 완전체 기간제 ( 7일 )
		PUI_HATCHLING_NORM_EVENT_F			= 30028, /// 해츨링 완전체 15일
		PUI_MYSTIC_EVENT_NORM_F				= 30029, /// 미스틱 완전체 15일
	};

	enum PET_CUSTOM_UI_MSG
	{
		PCUM_MAKE_NAME_CLOSE	= 43001,
		PCUM_MAKE_NAME_OK,
		PCUM_SUMMON_CANCEL,
		PCUM_BASIC_INFO,
		PCUM_DETAIL,
		PCUM_INVEN,
		FCUM_FOOD,
#ifdef CHANGE_PET_NAME
		PCUM_CHANGE_NAME_OK,
		PCUM_CHANGE_NAME_CLOSE,
#endif CHANGE_PET_NAME
	};

	struct PetStepImage
	{
		wstring m_wstrImageName;
		wstring m_wstrKeyName;
	};

	struct PetSkillInfo
	{
		wstring m_wstrImageName;
		wstring m_wstrKeyName;
		wstring m_wstrSkillDesc;

		PetSkillInfo()
		{
			m_wstrImageName = L"";
			m_wstrKeyName = L"";
			m_wstrSkillDesc = L"";
		}
	};

#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	typedef	CX2PetManager_PetInitTemplate	PetInitTemplet;

	struct	PetInitLuaTemplet
	{
		PetInitTemplet*	m_pInitTemplet;
		KLuaManager*	m_pLuaManager;

		PetInitLuaTemplet()
		{
			m_pInitTemplet = NULL;
			m_pLuaManager = NULL;
		}
	};//

#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	struct PetTemplet
	{
		PET_UNIT_ID				m_PetId;
		wstring					m_wstrCommonName;				// 대표이름
		float					m_caprice;						// 변덕
		float					m_appetite;						// 식사량
		int						m_Evolution_All_Step;			// 진화단계
	
		std::vector<int>		m_vecPetStatus;					// 각 단계별 펫 상태

		std::vector< int >		m_InvenSlot_Width_Size;			// 인벤토리 가로 길이
		std::vector< int >		m_InvenSlot_Height_Size;		// 인벤토리 세로 길이

		std::vector<float>		m_Evolution_Step_Max;			// 각 진화상태 max 친밀도
		std::vector<wstring>	m_Evolution_Step_ScriptName;	// 각 진화상태에 따른 스크립트 이름
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		std::vector<PetInitLuaTemplet>	m_Evolution_Step_InitLuaTemplet;
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		std::vector<PetStepImage> m_Evolution_Step_Image;		// 각 진화상태별 이미지
		std::vector<PetStepImage> m_Evolution_Step_Game_Image;	// 각 진화상태별 게임 이미지
		std::vector<wstring>	m_vecSkillName;					// 각 진화상태별 공격스킬 이름
		std::vector<PetSkillInfo> m_AttackSkill_Step;			// 각 진화상태별 공격스킬 아이콘/설명
		std::vector<PetSkillInfo> m_CheerSkill;					// 격려스킬 아이콘 / 설명		
		std::vector<wstring>	m_SpecialActionCommand;
#ifdef PET_AURA_SKILL
		std::vector<PetSkillInfo> m_AuraSkill;					// 기운스킬 정보
		std::vector<int>		m_vecAuraSkillOption[3];		// 기운스킬 소켓옵션 
#endif
#ifdef SERV_PET_SYSTEM_EX1
		bool					m_bAlwaysMaxSatiety;
#endif //SERV_PET_SYSTEM_EX1

		PetTemplet()
		{
			m_PetId = PUI_NONE;
			m_wstrCommonName = L"";
			m_caprice = 0.f;
			m_appetite = 0.f;
			m_Evolution_All_Step = 0;
			m_vecPetStatus.clear();
			m_Evolution_Step_Max.clear();
			m_Evolution_Step_ScriptName.clear();
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
			m_Evolution_Step_InitLuaTemplet.clear();
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
			m_Evolution_Step_Image.clear();
			m_Evolution_Step_Game_Image.clear();
			m_AttackSkill_Step.clear();			
			m_SpecialActionCommand.clear();
#ifdef PET_AURA_SKILL
			m_AuraSkill.clear();
			m_vecAuraSkillOption[0].clear();
			m_vecAuraSkillOption[1].clear();
			m_vecAuraSkillOption[2].clear();
#endif
#ifdef SERV_PET_SYSTEM_EX1
			m_bAlwaysMaxSatiety = false;
#endif //SERV_PET_SYSTEM_EX1
		}
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
		~PetTemplet();
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	};

	struct PetDisposition
	{
		int iDisposition[5][6][6];

		PetDisposition()
		{
			Init();
		}

		void Init()
		{
			// 펫의 격려 스킬 유지 시간
			int temp0[6][6] = {	{28, 26, 24, 20, 18, 16}, {26, 24, 20, 18, 16, 14}, {22, 20, 18, 16, 14, 12}, 
								{20, 18, 16, 14, 12, 10}, {18, 16, 14, 12, 10, 8},	{16, 14, 12, 10, 8, 6} };
			memcpy( iDisposition[0], temp0, sizeof(int) * 36 );

			// 펫의 공격 스킬 사용 시기
			int temp1[6][6] = {	{7, 8, 9, 10, 11, 12},	{8, 9, 10, 11, 12, 13},	{9, 10, 11, 12, 13, 14}, 
								{3, 4, 5, 6, 7, 8},	{4, 5, 6, 7, 8, 9},	{5, 6, 7, 8, 9, 10} };
			memcpy( iDisposition[1], temp1, sizeof(int) * 36 );

			// 펫의 격려 스킬 사용 시기 (던전)			
			int temp2[6][6] = {	{7, 8, 9, 10, 11, 12},	{8, 9, 10, 11, 12, 13},	{9, 10, 11, 12, 13, 14}, 
								{3, 4, 5, 6, 7, 8},	{4, 5, 6, 7, 8, 9},	{5, 6, 7, 8, 9, 10} };
			memcpy( iDisposition[2], temp2, sizeof(int) * 36 );

			// 펫의 격려 스킬 사용 시기 (대전)
			int temp3[6][6] = {	{2, 3, 4, 2, 2, 3},	{3, 4, 5, 2, 3, 3},	{4, 5, 6, 3, 3, 4}, 
								{2, 3, 4, 2, 2, 3},	{3, 4, 5, 2, 3, 3},	{4, 5, 6, 3, 3, 4} };
			memcpy( iDisposition[3], temp3, sizeof(int) * 36 );

			// 소환자 피격상태에 대한 피드백
			int temp4[6][6] = {	{13, 12, 12, 11, 11, 10},	{12, 12, 11, 11, 10, 10},	{12, 11, 11, 10, 10, 9}, 
								{11, 11, 10, 10, 9, 9},		{11, 10, 10, 9, 9, 8},	{10, 10, 9, 9, 8, 8} };
			memcpy( iDisposition[4], temp4, sizeof(int) * 36 );
		}

		int GetDisposition(int fVal1)
		{
			const int iStep = 4800;
			int iDisPosition = fVal1 + 4800;

			if( iDisPosition < 0 )
				iDisPosition = 0;
			if( iDisPosition > 9600 )
				iDisPosition = 9600;

			iDisPosition /= iStep;
			
			return iDisPosition;
		}

		int GetDispositionVal(int iType, int fVal1, int fVal2)
		{
			if( iType < 0 || iType > 4 )
				return 0;

			const int iStep = 1600;
			int iX = fVal1 + 4800;	// 감성지수
			int iY = fVal2 + 4800;	// 외향지수

			if( iX < 0 )
				iX = 0;
			if( iX > 9600 )
				iX = 9600;
			if( iY < 0 )
				iY = 0;
			if( iY > 9600 )
				iY = 9600;

			iX /= iStep;
			iY /= iStep;

			if( iX > 5 )
				iX = 5;
			if( iY > 5 )
				iY = 5;

			return iDisposition[iType][iY][iX];
		}
	};

public:
	CX2PetManager();
	virtual ~CX2PetManager(void);

	HRESULT				OnFrameMove( double fTime, float fElapsedTime );
	HRESULT				OnFrameRender();

	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void CreatePet(UidType uMasterUid, CX2PET::PetInfo& petInfo, bool bUpgrade = false, D3DXVECTOR3 vInitPos = D3DXVECTOR3(0.f, -1000.f, 0.f));
	CX2PETPtr CreateGamePet(UidType uMasterUid, CX2PET::PetInfo& petInfo);

	void AddPet(CX2PETPtr pPet);	
	map<UidType, CX2PETPtr>::iterator RemovePet(UidType uMasterUid);
	void ChangePet(CX2PETPtr pPet);
	void AllRemovePet();
	void RemovePetList();
	bool IsValidPet(CX2PET *pPet);

	void NotifyShowObjectChanged(UidType uMasterUid, bool bVal);

	void SetAllShow(bool bVal);
	bool GetAllShow() { return m_bShow; }
	void SetShow(UidType uMasterUid, bool bVal);
	bool GetShow(UidType uMasterUid);
	
	CX2PET *GetMyPet();
	CX2PET *GetPet(UidType uMasterUid);
	CX2PET *GetPetInx(int iInx);
	CX2PET *GetPetByUid( UidType iPetUid );
	UINT	GetPetNum() { return m_mapPet.size(); }
	
	char GetPetStatus( int petId, int iLv );
	char GetPetStatus( CX2PET::PetInfo& petInfo );

	void InitPos();

	CX2PET::PET_ACTION_COMMAND GetPetActionCommand( wstring &wstrMsg );
	void SendPetActionCommand( CX2PET::PET_ACTION_COMMAND ePetAction );
	void PlayEmotion(UidType userid, CX2PET::PET_ACTION_COMMAND ePetAction);

	bool OpenScriptFile( const WCHAR* pFileName );
#ifdef	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD
	void LoadInitPetData();
#endif	X2OPTIMIZE_GAME_PET_BACKGROUND_LOAD

	bool AddPetTemplet_LUA();
	PetTemplet *GetPetTemplet(PET_UNIT_ID pid);

	int	GetPetInventorySlotSize( PET_UNIT_ID pid, char cEvolutionStep );
	int	GetPetInventorySlotWidthSize( PET_UNIT_ID pid, char cEvolutionStep );
	int	GetPetInventorySlotHeightSize( PET_UNIT_ID pid, char cEvolutionStep );

	int GetDisposition(int iVal) { return m_PetDisposition.GetDisposition(iVal); }
	int GetDispositionVal(int iType, int iVal1, int iVal2) { return m_PetDisposition.GetDispositionVal(iType, iVal1, iVal2); }

	void RenderName();

	CX2TalkBoxManager*			GetTalkBoxManager() { return m_pTalkBoxMgr; }

	bool ForceUpgradePet();

	void ShowMakePetName(bool bVal, UidType iMakePetId = -1);
	//{{ 2012. 02. 22  오현빈    펫 이름 변경권
#ifdef CHANGE_PET_NAME
	void ShowChangePetName(bool bVal, UidType iChangeItemId = -1);
#endif CHANGE_PET_NAME
	//}} 
	bool CanMyPetEvolution();
	bool GetNerverSummonPet();
	void ShowPetList( bool bVal, int iPage = 0 );
	void AddMyPetList( std::vector< UidType > &vecPetList )
	{
		m_vecMyPetEvolutionList.clear();
		m_vecMyPetEvolutionList = vecPetList;
	}
	void AddNeverSummonPetList( std::vector< UidType > &vecPetList, UidType iPetUid )
	{
		m_vecNeverSummonPetList.clear();
		m_vecNeverSummonPetList = vecPetList;

		if( iPetUid > 0 )
		{
			for(UINT i=0; i<m_vecNeverSummonPetList.size(); ++i)
			{
				if( iPetUid == m_vecNeverSummonPetList[i] )
				{
					m_vecNeverSummonPetList.erase( m_vecNeverSummonPetList.begin() + i );
					break;
				}
			}
		}		
	}
	float GetIntimacyPercent( int petId, char lv, float fIntimacy );

	bool ChangeSatiety(UidType iUnitUID, UidType iPetUID, short sCurrentSatiety );
	bool ChangeIntimacy(UidType iUnitUID, UidType iPetUID, int iCurrentIntimacy );


	//{{ 김상훈 : 2010.10.07  캐릭터 우클리 팝업메뉴 UI 개선
#ifndef CHARACTER_MOUSE_RBUTTON
	bool GetMyPetPick();
#else
	bool GetMyPetPick( bool bOpenPopup);
#endif

#ifdef PRINT_INGAMEINFO_TO_EXCEL
	void PrintPetInfo_ToExcel(void);
#endif PRINT_INGAMEINFO_TO_EXCEL
	

	//}} 김상훈 : 2010.10.07  캐릭터 우클리 팝업메뉴 UI 개선
	void ClosePetPopupMenu();
	void OpenPetPopupMenu();

	float GetSummonedPetMp() const { return m_fSummonedPetMp; }
	void SetSummonedPetMp( const float fSummonedPetMp_ ) { m_fSummonedPetMp = fSummonedPetMp_; }

	void UpdateMasterUserSocketAndEnchantData( CX2GUUser* pMasterUser_ );

	//void OpenPetGagePopupMenu();
#ifdef SERV_EVENT_VC
	void UseIntimacyUpItem( UidType itemUid_ );
#endif //SERV_EVENT_VC

	bool Handler_EGS_COMMANDS_FOR_PETS_REQ( CX2PET::PET_ACTION_COMMAND ePetAction );
	bool Handler_EGS_COMMANDS_FOR_PETS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PET_ACTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_SUMMON_PET_REQ( UidType petUid );
	bool Handler_EGS_SUMMON_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_SUMMON_PET_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_PET_EVOLUTION_REQ( UidType petUid );
	bool Handler_EGS_PET_EVOLUTION_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_PET_EVOLUTION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_CHANGED_PET_SATIETY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		// 포만도 변경
	bool Handler_EGS_CHANGED_PET_INTIMACY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// 친밀도 변경
	bool Handler_EGS_CHANGED_PET_SATIETY_INTIMACY_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// 포만도&친밀도 변경

	void Handler_EGS_CREATE_PET_REQ( UidType itemUid, wstring wstrPetName );
	bool Handler_EGS_CREATE_PET_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void Handler_EGS_FEED_PETS_REQ( UidType iItemUid );
	bool Handler_EGS_FEED_PETS_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool Handler_EGS_AUTO_FEED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool Handler_EGS_FIRST_FEED_PETS_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	bool Handler_EGS_EAT_SPECIAL_FEED_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_ADD_KEWPIEL_JELLY
	//{{ 2012. 02. 22  오현빈    펫 이름 변경권
#ifdef CHANGE_PET_NAME
	void Handler_EGS_CHANGE_PET_NAME_REQ( UidType itemUid, wstring wstrPetName );
	bool Handler_EGS_CHANGE_PET_NAME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif CHANGE_PET_NAME
	//}} 
#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	bool Handler_EGS_CHANGE_PET_ID_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_PERIOD_PET
	bool Handler_EGS_RELEASE_PET_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif SERV_PERIOD_PET

#ifdef SERV_EVENT_VC
	void Handler_EGS_USE_INTIMACY_UP_ITEM_REQ( UidType itemUid_ );
	bool Handler_EGS_USE_INTIMACY_UP_ITEM_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif //SERV_EVENT_VC

public:
	static const float MAX_OF_SATIETY;	/// 최대 포만감
	static const float SATIETY_RATIO_TO_BE_ABLE_TO_APPLY_AURA;	/// 기운이 적용될 수 있는 포만도 비율

private:
	bool				m_bShow;		

	std::map<UidType, CX2PETPtr>		m_mapPet;
	std::map<PET_UNIT_ID, PetTemplet*>	m_PetTemplet;

	PetDisposition			m_PetDisposition;

	CX2TalkBoxManager*			m_pTalkBoxMgr;

	bool				m_bCreatePet;

	UidType					m_iMakePetId;
	CKTDGUIDialogType		m_pDlgMakePetName;	
	CKTDGUIDialogType		m_pDlgPetPopupMenu;	
#ifdef CHANGE_PET_NAME
	UidType					m_iChangeItemUID;
	CKTDGUIDialogType		m_pDlgChangePetName;	
#endif CHANGE_PET_NAME

	std::vector< UidType > m_vecMyPetEvolutionList;
	std::vector< UidType > m_vecNeverSummonPetList;

	int m_iEnabledPetAi;
	std::vector<CX2PETPtr>	m_vecRemovePet;

	float					m_fSummonedPetMp;	/// 소환된 펫의 MP를 저장 (마을<->필드<->던전 등간의 이동에서 소환된 펫의 MP를 유지하기 위한 용도)
};

#endif