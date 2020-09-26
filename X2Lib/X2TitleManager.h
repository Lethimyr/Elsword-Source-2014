#pragma once

class CX2TitleManager
{
public:

	enum TITLE_TYPE
	{
		TT_NONE = 0,
		TT_RUBEN,
		TT_ELDER,
		TT_BESMA,
		TT_ALTERA,
		TT_ARCADE,
		TT_PVP,
		TT_EVENT,
		TT_VELDER,
		//{{ 2011. 01. 25	최육사	하멜 추가
		TT_HAMEL,
		//}}
		TT_GROW_UP, // 해외. 이름 수정(2013.09.03 김창한)
		//{{ 2012. 01. 02	김태환  샌더 추가
		TT_SANDER,
		//}}
        TT_END,
	};

	enum TITLE_MISSION_CLEAR_TYPE
	{
		TMCT_NONE = 0,
		TMCT_NPC_TALK,
		TMCT_NPC_HUNT,
		TMCT_ITEM_COLLECTION,
		TMCT_QUEST_ITEM_COLLECTION,
		TMCT_DUNGEON_TIME,
		TMCT_DUNGEON_RANK,
		TMCT_DUNGEON_DAMAGE,
		TMCT_DUNGEON_CLEAR_COUNT,
		TMCT_PVP_PLAY,
		TMCT_PVP_WIN,
		TMCT_PVP_KILL,
		TMCT_QUEST,
		TMCT_COLLECT_TITLE,
        TMCT_USE_ITEM,
		//{{ 2010. 05. 17  최육사	대전 던전 서버군 통합
		TMCT_WITH_DIF_SERV_USER,
		//}}
		//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		TMCT_GIVE_PET_FEED,						// 펫 먹이 주기
		TMCT_PLAYER_WITH_DUNGEON_CLEAR,			// 특정 인원↓, 던전클리어
		TMCT_RESURRECTION_STONE,				// 부활석 사용
		TMCT_TOGETHER_MISSION_CLEAR,			// 미션 동시 만족
		TMCT_USER_UNIT_DIE,			// 산소 부족으로 사망
#endif SERV_ADD_TITLE_CONDITION
		//}}
#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
		TMCT_ITEM_SOCKET,
		TMCT_ITEM_ENCHANT_LEVEL,
		TMCT_ITEM_ENCHANT_COUNT,
		TMCT_ITEM_ATTRIB,
		TMCT_ITEM_RESOLVE,
#endif // SERV_ADD_TITLE_CONDITION_2013_08
	};

    enum TITLE_MANAGER_UI_MSG
    {
        TMUI_CLOSE      = 2200,
        TMUI_ATTACH,
        TMUI_TAB,
        TMUI_LIST_SELECT,        
        TMUI_CHECK_TAKE,
    };

	struct TitleInfo
	{
		int						m_iTitleID;
		int						m_iSortNum;

		TITLE_TYPE				m_eTitleType;
		std::wstring			m_wstrTitleName;
		std::wstring			m_wstrDescription;

		std::wstring			m_wstrParticleName;

		CX2Unit::UNIT_TYPE		m_eUnitType;
		CX2Unit::UNIT_CLASS		m_eUnitClass;
		int						m_iOpenLevel;
		bool					m_bIsSecretTitle;

		int						m_iBaseHP;
		int						m_iAtkPhysic;
		int						m_iAtkMagic;
		int						m_iDefPhysic;
		int						m_iDefMagic;

		std::vector< int >	m_vecSocketOption;

		bool					m_bVisible;

#ifdef SERV_GROW_UP_TITLE
		std::map<int, std::wstring>	m_mapGrowUpParticleName;	// <성장 레벨, 표시될 파티클 이름>
#endif SERV_GROW_UP_TITLE
	};

    //////////////////////////////////////////////////////////////////////////
    // 미션 정보

    // Mission을 받기위한 제약사항
    struct MissionCondition
    {
        int						m_iLevel;
        CX2Unit::UNIT_TYPE		m_eUnitType;
        CX2Unit::UNIT_CLASS		m_eUnitClass;		
        std::vector< int >		m_vecClearMissionID;

        MissionCondition()
        {
            m_iLevel				= -1;
            m_eUnitType				= CX2Unit::UT_NONE;
            m_eUnitClass			= CX2Unit::UC_NONE;
        }
    };	

    struct MissionTemplet
    {
        int						 m_iMissionID;
        TITLE_MISSION_CLEAR_TYPE m_eClearType;
        std::wstring			 m_wstrMissionName;        

        MissionCondition		 m_kCondition;
        std::vector< int >		 m_vecSubMission;

        short					 m_sPeriod;
        int						 m_iTitleID;

        MissionTemplet()
        {
            m_iMissionID = 0;
            m_eClearType = CX2TitleManager::TMCT_NONE;            
            m_sPeriod	 = 0;
            m_iTitleID	 = 0;
        }
    };

    //////////////////////////////////////////////////////////////////////////

    //Sub quest type 조건별 확인할 데이터
    struct ClearCondition
    {
        bool                            m_bSuccess;
        int                             m_nCount;

        //::공용 데이터
        //::1. Dungeon ID
        SEnum::DUNGEON_ID			m_eDungeonID;
        //::2. Dungeon Difficulty
        char							m_cDifficulty;
		//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
		CX2Dungeon::DUNGEON_MODE		m_eDungeonMode;
		//:: Upper Difficulty
		bool							m_bUpperDifficulty;
#endif SERV_HELL_MODE_TITLE
		//}}
        
		//::3. NPC ID & Monster ID 2가지 기능.

		//{{ 2012. 1. 11	Merge 박세훈	2012.12.26 임규수 타이틀 미션 클리어 NPC 복수 적용
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
		std::set< CX2UnitManager::NPC_UNIT_ID > m_setKillNPCID;
#else
        CX2UnitManager::NPC_UNIT_ID		m_eKillNPCID;
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
		//}}

        //::4. PVP TYPE
        CX2PVPRoom::PVP_GAME_TYPE		m_ePVPType;
        //::5. Item ID
        int								m_iCollectionItemID;

        //::조건별 데이터
        //::1. Monster kill count..
        int								m_iKillNum;
        //::2. Item collection count
        int								m_iCollectionItemNum;
        //::3. clear dungeon rank
        CX2DungeonRoom::RANK_TYPE		m_eDungeonRank;
        //::4. clear dungeon time
        int								m_iDungeonClearTime;
        //::5. talk npc
        CX2UnitManager::NPC_UNIT_ID	    m_eTalkNPCID;
        bool							m_bTalkNPC;	
        //::6. pvp play count
        int								m_iPVPPlay;
        //::7. pvp win count
        int								m_iPVPWin;
        //::8. pvp kill count
        int								m_iPVPKill;
        //::9. quest item drop rate
        float							m_fQuestItemDropRate;
        //::10. damage
        int								m_iDungeonDamage;
        //::11. Dungeon clear count
        int								m_iDungeonClearCount;
        //::12. Quest Clear
        int								m_iQuestID;
        //::13. Title Collect
        int								m_iTitleID;
        //::14. Use Item
        int								m_iItemID;
		//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
		//::15. Pet Feed
		CX2Item::ITEM_GRADE			m_eItemGrade;
		int								m_iPetFeedCount;
		//::16. Player With Dungeon Clear
		int								m_iPlayerCount;
		//::17. Use Resurrection Stone
		bool							m_bCheckResurrectionStone;
		//::18. User Unit Die Reason
		KEGS_USER_UNIT_DIE_REQ::USER_UNIT_DIE_REASON	m_eDieReason;
#endif SERV_ADD_TITLE_CONDITION
		//}}

        ClearCondition()
        {
            m_bSuccess              = false;
            m_nCount                = 0;

			//{{ 2012. 1. 11	Merge 박세훈	2012.12.26 임규수 타이틀 미션 클리어 NPC 복수 적용
#ifdef SERV_SUB_TITLE_MISSION_PLURAL_NPC
#else
            m_eKillNPCID			= CX2UnitManager::NUI_NONE;
#endif SERV_SUB_TITLE_MISSION_PLURAL_NPC
			//}}
            m_iKillNum				= 0;
            m_iCollectionItemID		= -1;
            m_iCollectionItemNum	= 0;
            m_eDungeonID			= SEnum::DI_NONE;
            m_cDifficulty			= -1;
			//{{ 2010. 08. 23  최육사	비밀던전 헬모드
#ifdef SERV_HELL_MODE_TITLE
			m_eDungeonMode			= CX2Dungeon::DM_INVALID;
			m_bUpperDifficulty		= false;
#endif SERV_HELL_MODE_TITLE
			//}}
            m_eDungeonRank			= CX2DungeonRoom::RT_NONE;
            m_iDungeonClearTime		= 0;
            m_eTalkNPCID			= CX2UnitManager::NUI_NONE;
            m_bTalkNPC				= false;
            m_ePVPType				= CX2PVPRoom::PGT_TEAM;
            m_iPVPPlay				= 0;
            m_iPVPWin				= 0;
            m_iPVPKill				= 0;
            m_fQuestItemDropRate	= 0.0f;
            m_iDungeonDamage		= 0;
            m_iDungeonClearCount	= 0;
            m_iQuestID				= 0;
            m_iTitleID				= 0;
			m_iItemID				= 0;
			//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
			m_eItemGrade			= CX2Item::IG_NONE;
			m_iPetFeedCount			= 0;
			m_iPlayerCount			= 0;
			m_bCheckResurrectionStone = false;
			m_eDieReason			= KEGS_USER_UNIT_DIE_REQ::UUDR_UNKNOWN;
#endif SERV_ADD_TITLE_CONDITION
			//}}
        }
    };

    struct SubMissionTemplet
    {
        int						 m_iSubMissionID;

        std::wstring			 m_wstrDescription;
        TITLE_MISSION_CLEAR_TYPE m_eClearType;
        ClearCondition			 m_ClearCondition;
        bool					 m_bAutomaticDescription;

        SubMissionTemplet()
        {
            m_iSubMissionID = -1;
            m_eClearType	= CX2TitleManager::TMCT_NONE;
            m_bAutomaticDescription = true;
        }
    };


public:
	CX2TitleManager(void);
	~CX2TitleManager(void);

    HRESULT OnFrameMove(double fTime, float fElapsedTime);
    void CheckNewTitle();

	bool OpenScriptFile( const WCHAR* pFileName );
	bool AddTitleInfo_LUA();
    bool AddTitleMissionInfo_LUA();
    bool AddSubTitleMissionInfo_LUA();
#ifdef LUA_TRANS_DEVIDE
	bool AddTitleInfoTrans_LUA();
	bool AddTitleMissionInfoTrans_LUA();
	bool AddSubTitleMissionInfoTrans_LUA();
#endif LUA_TRANS_DEVIDE
    bool LoadClearCondition( KLuaManager& luaManager, SubMissionTemplet& kSubMissionTemplet );

	const TitleInfo* GetTitleInfo( int iTitleID );

    bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    bool GetNewTitle() { return m_bNewTitle; }
    bool GetShow();
    void OpenTitle();
    void CloseTitle();
    void ChangeTitleGroup(int iGroup);

    void InitRecord();

    bool SelectTitle(int val);
    bool AttachTitle(int val);
    void DetachTitle();
    wstring GetTitleModel(int val);
#ifdef SERV_GROW_UP_TITLE
	wstring GetTitleModel( int val_, int iLevel_ );
#endif SERV_GROW_UP_TITLE
    wstring GetTitleName() { return m_strDesc; }
	wstring GetTitleImgName() { return m_strImgName; }
    wstring GetMIssionDesc(int missionId);
    wstring GetSubMissionDesc(int subMissionId);
    wstring GetTitleAbilityDesc(int titleId);
    CX2Stat::Stat CX2TitleManager::GetSocketStat();
    bool GetUnitType(int titleid);
    bool GetUnitClass(int titleid);

    const CX2TitleManager::MissionTemplet* GetMissionInfo( int iMissionID );
    const CX2TitleManager::SubMissionTemplet* GetSubMissionInfo( int iSubMissionID );

    void TakeTitle(std::vector<KTitleInfo> vecTitleInfo);
    bool UpdateTitle(KTitleInfo ktitleInfo, int iState);
    bool UpdateTitle(int iTitleId, int iState, bool bInfinity = true);
    bool UpdateMission(std::vector< KMissionInstance > vecMisstionInfo);
    void UpdateSubMission(int iMissionId, std::vector< KSubMissionInstance > vecSubMission);
    void UpdateClearCondition(KSubMissionInstance kSubMission);

    bool GetTakeTitle(int titleid);
	void EqipTitle(int titleid);
	//{{ 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력
#ifdef PRINT_INGAMEINFO_TO_EXCEL
	void PrintTitleInfo_ToExcel(void);
#endif PRINT_INGAMEINFO_TO_EXCEL
	//}} 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력

private:
	std::map< int, TitleInfo* >		m_mapTitleInfo;
    std::map< int, MissionTemplet >	    m_mapTitleMission;
    std::map< int, SubMissionTemplet >	m_mapSubMission;

    CX2TitleSystem                      *m_pDlgTitle;    
    bool m_bInit;
    bool m_bProcess;
    bool m_bNewTitle;
    bool m_bShowNewTitle;
    bool m_bNewOpenMission;

    int m_iSelTitleId;
    wstring m_strDesc;
	wstring m_strImgName;

	int									m_nString_Index;
#ifdef SHOW_NEW_TITLE_NAME
	wstring m_strNewTitle;
#endif //SHOW_NEW_TITLE_NAME

//#if 0 // 칭호 프리뷰
//    CKTDGParticleSystem::CParticleEventSequenceHandle	m_hSeqEmblem;
//    CKTDGParticleSystem::CParticle*						m_pPart_Emblem_200;    
//#endif

private:
    bool Handler_EGS_EQUIP_TITLE_REQ(int titleId);
    bool Handler_EGS_EQUIP_TITLE_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ); 
    bool Handler_EGS_NEW_MISSION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    bool Handler_EGS_REWARD_TITLE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    bool Handler_EGS_UPDATE_MISSION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );    
    bool Handler_EGS_TITLE_EXPIRATION_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    
};



