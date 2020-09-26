#pragma once

#include "XSLDungeonSubStage.h"

class CXSLDungeonStage
{
public:
	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
	enum STAGE_TYPE
	{
		ST_NONE = 0,
		ST_EXTRA_NPC,
		ST_EXTRA,
	};
	//}}

	struct ReadyNPCData
	{
		int						m_UnitID;
	};

	struct StageData	
	{
	public:
		int												m_WorldID;
		//{{ 2010. 03. 24  최육사	비밀던전 헬모드
		STAGE_TYPE										m_eStageType;
		//}}
		std::vector<CXSLUnitManager::NPC_UNIT_ID>		m_ReadyNPCList;
		std::vector<CXSLDungeonSubStage::SubStageData*>	m_SubStageDataList;
		std::map<int,char>								m_NPCRate;

	public:
		~StageData()
		{
			m_ReadyNPCList.clear();

			for( int i = 0; i < (int)m_SubStageDataList.size(); i++ )
			{
				CXSLDungeonSubStage::SubStageData* pSubStageData = m_SubStageDataList[i];
				SAFE_DELETE( pSubStageData );
			}
			m_SubStageDataList.clear();

			//SAFE_DELETE( m_pWorld );
			m_NPCRate.clear();
		}

		bool LoadData( IN bool bScriptCheck, KLuaManager& luaManager );
		void SetNPCRate( std::map<int,char>& NPCRate ){ m_NPCRate = NPCRate; }
		int  GetNPCRate( int uid );

	private:
		//bool LoadReadyNPC( KLuaManager& luaManager );
		bool LoadSubStageData( IN bool bScriptCheck, KLuaManager& luaManager );
	};

public:

public:
	CXSLDungeonStage( StageData* pStageData );
	~CXSLDungeonStage();

	StageData*					m_pStageData;

	int							m_NowSubStageIndex;
	int							m_BeforeSubStageIndex;
};