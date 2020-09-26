
#include "XSLStat.h"
#include "XSLUnitManager.h"
#include <d3dx9math.h>

//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
#include <Lottery.h>
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
//}}

class CXSLDungeonSubStage
{
public:
	enum CLEAR_TYPE
	{
		CT_SUB_STAGE	= 0,
		CT_STAGE		= 1,
		CT_STAGE_FORCE	= 2,
		CT_STAGE_SECRET	= 3,
		CT_GAME			= 4,
		CT_FAILURE		= 5,
	};

	enum GO_TYPE
	{
		GT_NONE = 0,
		GT_RIGHT,
		GT_LEFT,
		GT_UP,
		GT_DOWN,
	};	

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	enum SECRET_STAGE_PAD
	{
		SSP_NONE = 0,
		SSP_SECRET,
		SSP_ELITE,
		SSP_NORMAL,
	};

	struct NextStageData
	{
		CLEAR_TYPE m_eClearType;
		int m_iStageIndex;
		int m_iSubStageIndex;
		int m_iRate;

		int m_iDungeonEndingSpeechTableIndex;	// clear type == CT_GAME인 경우에만 유효

		NextStageData()
		{
			ClearData();
		}

		void ClearData()
		{
			m_eClearType = CT_GAME;
			m_iStageIndex = -1;
			m_iSubStageIndex = -1;
			m_iRate = 100;
			m_iDungeonEndingSpeechTableIndex = 0;
		}
	};

	struct ClearCondtionData
	{
		std::vector<NextStageData>	m_vecNextStage;
	};
#endif // SERV_STAGE_CLEAR_IN_SERVER

	struct NPCData
	{
		int								m_UID;
		CXSLUnitManager::NPC_UNIT_ID	m_UnitID;
		int								m_KeyCode;
		bool							m_bFocusCamera;
		std::vector<int>				m_vecStartPos;
		//int								m_StartPos;
		bool							m_bActive;
		bool							m_bShowGage;
		D3DXVECTOR3						m_AddPos;

		int								m_Rate;
		int								m_Level;

		bool							m_bHasBossGage;
		bool							m_bShowBossName;
		bool							m_bShowSubBossName;
		bool							m_bSiegeMode;
		//{{ 2007. 10. 26  최육사  
		bool							m_bIsRight;
		//}}

		bool							m_bSubNPC;
		int								m_iSubNPCIndex;
		int								m_iSubNPCRate;

		bool							m_bNoDrop;
		float							m_fUnitScale;
		//{{ 2009. 12. 18  최육사	던전경험치개편
		CXSLUnitManager::MONSTER_GRADE	m_MonsterGrade;
		//}}
		//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_BOSS_GAUGE_HP_LINES
		u_short							m_BossGaugeHPLines;
#endif SERV_BOSS_GAUGE_HP_LINES
		//}}
		//{{ 2011. 03. 29	손영준	몬스터 팀 변수 추가
#ifdef SERV_ALLY_NPC
		bool							m_bAllyNpc;
#endif SERV_ALLY_NPC
		//}}
#ifdef SERV_MODIFY_DUNGEON_STAGING
		float							m_fShowBossNameDelayTime;
		bool							m_bStopAtStartState;
#endif //SERV_MODIFY_DUNGEON_STAGING
	};

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
	struct NPCDropData
	{
		CXSLUnitManager::NPC_UNIT_ID	m_UnitID;
		int								m_iDropTimes;
	};
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// 적용날짜: 2013-08-13
	struct NPCExpRateData
	{
		CXSLUnitManager::NPC_UNIT_ID	m_UnitID;
		float							m_fExpRate;
	};
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

	struct SubStageData 
	{
		int						m_StartLineSet;
		int						m_EndLineSet;
		int						m_MainLineSet;

		//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
		KLottery								m_kNpcGroupRate;
		std::map< int, std::vector<NPCData*> >	m_mapNPCDataList;
#else
		std::vector<NPCData*>	m_NPCDataList;
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
			//}}

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
		std::vector<NPCDropData>	m_NPCDropDataList;
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// 적용날짜: 2013-08-13
		std::vector<NPCExpRateData>	m_vecNPCExpRateDataList;
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE

		GO_TYPE					m_GoType;
		//{{ 2010. 04. 26  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
		int						m_ExtraNpcStartPos;
		bool					m_bExtraNpcIsLeft;
#endif SERV_SECRET_HELL
		//}}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
		std::vector<ClearCondtionData>	m_vecClearCondData;
#endif // SERV_STAGE_CLEAR_IN_SERVER

		SubStageData()
		{
			m_GoType = GT_NONE;
		}

		~SubStageData()
		{
			//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
			std::map< int, std::vector<NPCData*> >::iterator mit = m_mapNPCDataList.begin();
			for( ; mit != m_mapNPCDataList.end() ; ++mit )
			{
				for( int i = 0; i < (int)mit->second.size(); i++ )
				{
					NPCData* pNPCData = mit->second[i];
					SAFE_DELETE( pNPCData );
				}

				mit->second.clear();
			}
			m_mapNPCDataList.clear();
#else
			for( int i = 0; i < (int)m_NPCDataList.size(); i++ )
			{
				NPCData* pNPCData = m_NPCDataList[i];
				SAFE_DELETE( pNPCData );
			}
			m_NPCDataList.clear();
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
			//}}
		}

		bool LoadData( IN bool bScriptCheck
					 , IN OUT KLuaManager& luaManager
					 );

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
		int GetNpcDropTimes( int iUnitID );
#endif SERV_CREATED_NPC_LIMITED_DROPS
#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// 적용날짜: 2013-08-13
		float GetNpcExpRate( int iUnitID );
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE
		//void CreateNPC();
		//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
		int  GetRandomNpcGruopID();
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
		//}}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
		int GetSecretStageEnteringEvent( IN int iClearConditionIndex );
		bool GetNextStage( OUT CXSLDungeonSubStage::NextStageData& kNextStageData, IN int iClearConditionIndex, IN int iSecretPadIndex );
#endif // SERV_STAGE_CLEAR_IN_SERVER

	private:
#ifdef SERV_CREATED_NPC_LIMITED_DROPS
		bool LoadCreatedNpcDropTimesData( KLuaManager& luaManager );
#endif SERV_CREATED_NPC_LIMITED_DROPS
#ifdef SERV_DUNGEON_NPC_DATA_EXP_RATE		// 적용날짜: 2013-08-13
		bool LoadCreatedNpcExpRateData( KLuaManager& luaManager );
#endif // SERV_DUNGEON_NPC_DATA_EXP_RATE
		bool LoadNPCData( IN bool bScriptCheck, KLuaManager& luaManager );
		bool FetchNPCData( IN bool bScriptCheck, KLuaManager& luaManager, CXSLDungeonSubStage::NPCData* pNPCData );

		//{{ 2012. 12. 21  던전 몬스터 그룹 랜던 배치 - 김민성
#ifdef SERV_DUNGEON_RANDOM_NPC_GROUP
		bool New_LoadNPCData( IN bool bScriptCheck, KLuaManager& luaManager );
#endif SERV_DUNGEON_RANDOM_NPC_GROUP
		//}}

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
		bool LoadClearCondition( KLuaManager& luaManager );
#endif // SERV_STAGE_CLEAR_IN_SERVER
	};

public:
	CXSLDungeonSubStage( int subStageIndex, SubStageData* pSubStageData );
	~CXSLDungeonSubStage(void);

protected:
	SubStageData*		m_pSubStageData;
	int					m_SubStageIndex;

	bool				m_bStart;
	int					m_LoadNPCIndex;
};