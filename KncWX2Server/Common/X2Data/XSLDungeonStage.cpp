#include "XSLDungeonStage.h"

#ifdef _CONVERT_VS_2010
#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))
#endif _CONVERT_VS_2010

CXSLDungeonStage::CXSLDungeonStage( StageData* pStageData )
{
	m_pStageData			= pStageData;
	m_NowSubStageIndex		= 0;
	m_BeforeSubStageIndex	= 0;
}

CXSLDungeonStage::~CXSLDungeonStage()
{

}

bool CXSLDungeonStage::StageData::LoadData( IN bool bScriptCheck, KLuaManager& luaManager )
{
	LUA_GET_VALUE_RETURN(	luaManager, "WORLD_ID",		m_WorldID,		-1,			return false );
	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
	LUA_GET_VALUE_ENUM(		luaManager, "STAGE_TYPE",	m_eStageType,	STAGE_TYPE,	ST_NONE );
	//}}

	////스테이지에서 예약할 NPC 정보를 읽어둔다
	//if( LoadReadyNPC( luaManager ) == false )
	//	return false;

	//서브 스테이지 정보를 읽어온다
	if( LoadSubStageData( bScriptCheck, luaManager ) == false )
		return false;

	return true;
}

int CXSLDungeonStage::StageData::GetNPCRate( int uid )
{
	std::map<int,char>::iterator iter = m_NPCRate.find( uid );
	if( iter != m_NPCRate.end() )
	{
		char rate = iter->second;
		return (int)rate;
	}
	return -1;
}

//bool CXSLDungeonStage::StageData::LoadReadyNPC( KLuaManager& luaManager )
//{
//	if( luaManager.BeginTable( "READY_NPC" ) != E_FAIL )
//	{
//		std::string		strbuffer;
//		std::wstring	npcName;
//		for( int i = 1; luaManager.GetValue( i, strbuffer ) != E_FAIL; ++i )
//		{
//			ConvertCharToWCHAR( npcName, strbuffer.c_str() );
//			m_ReadyNPCList.push_back( npcName );
//		}
//		luaManager.EndTable();
//
//		return true;
//	}
//
//	return false;
//}

bool CXSLDungeonStage::StageData::LoadSubStageData( IN bool bScriptCheck, KLuaManager& luaManager )
{
	bool bRet = true;

	char strTable[MAX_PATH] = "";
	for( int i = 0;; ++i )
	{
#ifdef _CONVERT_VS_2010
		sprintf_s( strTable, ARRAY_SIZE( strTable ), "SUB_STAGE%d", i );
#else
		sprintf( strTable, "SUB_STAGE%d", i );
#endif _CONVERT_VS_2010
		if( luaManager.BeginTable( strTable ) == E_FAIL )
			break;

		CXSLDungeonSubStage::SubStageData* pSubStageData = new CXSLDungeonSubStage::SubStageData();
		const bool bResult = pSubStageData->LoadData( bScriptCheck
			, luaManager
			);

		if( bResult == false )
		{
			bRet = false;
		}

		m_SubStageDataList.push_back( pSubStageData );
		luaManager.EndTable();
	}

	return bRet;
}


