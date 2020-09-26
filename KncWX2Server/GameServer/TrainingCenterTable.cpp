#include ".\trainingcentertable.h"

#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"

ImplementSingleton( KTrainingCenterTable );

KTrainingCenterTable::KTrainingCenterTable(void)
{
	//lua_tinker::class_<KTrainingCenterTable>("KTrainingCenterTable")
	//	.def("AddTCTemplet",						KTrainingCenterTable::AddTCTemplet_LUA)
	//	.def("SetTrainingListByUnitClass_LUA",		KTrainingCenterTable::SetTrainingListByUnitClass_LUA)
	//	;

	//lua_tinker::decl( "KTrainingCenterTable", this );
	lua_tinker::class_add<KTrainingCenterTable>( g_pLua, "KTrainingCenterTable" );
	lua_tinker::class_def<KTrainingCenterTable>( g_pLua, "AddTCTemplet",					 &KTrainingCenterTable::AddTCTemplet_LUA );
	lua_tinker::class_def<KTrainingCenterTable>( g_pLua, "SetTrainingListByUnitClass_LUA",	 &KTrainingCenterTable::SetTrainingListByUnitClass_LUA );

	lua_tinker::decl( g_pLua, "KTrainingCenterTable", this );
}

KTrainingCenterTable::~KTrainingCenterTable(void)
{
	m_mapTCInfo.clear();
}

ImplToStringW( KTrainingCenterTable )
{
	return stm_;
}

bool KTrainingCenterTable::OpenScriptFile( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}

bool KTrainingCenterTable::AddTCTemplet_LUA()
{
	KLuaManager luaManager( g_pLua );

	TC_TABLE_INFO	kInfo;

	LUA_GET_VALUE_RETURN(	luaManager, L"m_iID",			kInfo.m_iID,			0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_iDungeonID",	kInfo.m_iDungeonID,		0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_iMinLevel",		kInfo.m_iMinLevel,		0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_iBeforeID",		kInfo.m_iBeforeID,		0,		goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_fPlayTime",		kInfo.m_fPlayTime,		0.f,	goto LoadFail; );
	LUA_GET_VALUE_RETURN(	luaManager, L"m_iDifficulty",	kInfo.m_iDifficulty,	0,		goto LoadFail; );

	//LUA_GET_VALUE_RETURN(	luaManager, L"m_wstrTitle",		kInfo.m_wstrTitle,		L"",	goto LoadFail; );
	//LUA_GET_VALUE_RETURN(	luaManager, L"m_wstrDesc",		kInfo.m_wstrDesc,		L"",	goto LoadFail; );


	if( luaManager.BeginTable( L"UNIT_CLASS" ) == S_OK )
	{
		int index	=  1; 
		int buf		= -1;
		while( luaManager.GetValue( index, buf ) == S_OK )
		{
			if( buf >= 0 )
			{
				kInfo.m_vecUnitClass.push_back( buf );
			}
			index++;
		}

		if( index == 1 && buf == -1 )
		{
			goto LoadFail;
		}

		luaManager.EndTable();
	}

	if( luaManager.BeginTable( L"REWARD" ) == S_OK )
	{
		LUA_GET_VALUE_RETURN(	luaManager, L"m_iRewardED",		kInfo.m_iRewardED,		0,		goto LoadFail; );
		LUA_GET_VALUE_RETURN(	luaManager, L"m_iRewardEXP",	kInfo.m_iRewardEXP,		0,		goto LoadFail; );

		luaManager.EndTable();
	}

	m_mapTCInfo.insert( std::make_pair( kInfo.m_iID, kInfo ) );

	return true;


LoadFail:
	START_LOG( cerr, L"TRAINING CENTER LOAD FAIL ::" )
		<< BUILD_LOG( kInfo.m_iID )
		<< BUILD_LOG( kInfo.m_iDungeonID )
		<< BUILD_LOG( kInfo.m_iMinLevel )
		<< BUILD_LOG( kInfo.m_iBeforeID )
		<< BUILD_LOG( kInfo.m_fPlayTime )
		<< BUILD_LOG( kInfo.m_iDifficulty )
		<< BUILD_LOG( kInfo.m_vecUnitClass.size() )
		<< BUILD_LOG( kInfo.m_iRewardED )
		<< BUILD_LOG( kInfo.m_iRewardEXP )
		<< END_LOG;

	return false;
}

bool KTrainingCenterTable::CheckIfEnter( IN std::map< int, KTCClearInfo >& mapTCClear, IN int iTCID )
{
	//진행하면서 제약을 체크한다.
	std::map< int , TC_TABLE_INFO >::iterator mit;
	std::map< int , KTCClearInfo >::iterator mitBefore;
	mit = m_mapTCInfo.find( iTCID );

	if( mit != m_mapTCInfo.end() )
	{
		if( mit->second.m_iBeforeID == 0 )
		{
			return true;
		}

		if( mapTCClear.size() > 0 )
		{
			mitBefore = mapTCClear.find( mit->second.m_iBeforeID );

			if( mitBefore != mapTCClear.end() )
			{
				return true;
			}
		}
	}

	return false;
}

int KTrainingCenterTable::GetDungeonID( IN int iTCID )
{
	std::map<int, TC_TABLE_INFO>::iterator mit;

	mit = m_mapTCInfo.find( iTCID );

	if( mit != m_mapTCInfo.end() )
	{
		return mit->second.m_iDungeonID;
	}

	return -1; 
}

float KTrainingCenterTable::GetPlayTime( IN int iTCID )
{
	std::map<int, TC_TABLE_INFO>::iterator mit;

	mit = m_mapTCInfo.find( iTCID );

	if( mit != m_mapTCInfo.end() )
	{
		return mit->second.m_fPlayTime;
	}

	return -1.f; 
}

bool KTrainingCenterTable::GetReward( IN int iTCID, OUT int& iRewardED, OUT int& iRewardEXP )
{
	iRewardED	= 0;
	iRewardEXP	= 0; 

	std::map<int, TC_TABLE_INFO>::iterator mit;

	mit = m_mapTCInfo.find( iTCID );

	if( mit != m_mapTCInfo.end() )
	{
		iRewardED		= mit->second.m_iRewardED;
		iRewardEXP		= mit->second.m_iRewardEXP;

		return true;
	}

	return false;
}

#ifdef DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM

bool KTrainingCenterTable::IsTrainingCenter( IN int iDungeonID )
{
	std::map<int, TC_TABLE_INFO>::iterator mit
		= m_mapTCInfo.begin();

	while ( mit != m_mapTCInfo.end() )
	{
		if ( mit->second.m_iDungeonID == iDungeonID )
			return true;
		else
			mit++;
	}

	return false;
}

#endif // DO_NOT_COUNT_DEAD_NPC_RELATED_TO_TITLE_MISSION_IN_TRAINING_ROOM




