#include "KncRobotManager.h"
#include "battlefieldmapinfo.h"
#include "../Common/Enum/Enum.h"

ImplementSingleton( KBattleFieldMapInfo );

KBattleFieldMapInfo::KBattleFieldMapInfo(void)
{	
	m_iMapCount = 1;
}

KBattleFieldMapInfo::~KBattleFieldMapInfo(void)
{
}


bool KBattleFieldMapInfo::OpenScriptFile( const char* pFileName )
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();

	lua_tinker::class_add<KBattleFieldMapInfo>( spLua.get(), "BattleFieldManager" );
	lua_tinker::class_def<KBattleFieldMapInfo>( spLua.get(), "AddBattleFieldData",		&KBattleFieldMapInfo::AddBattleFieldData_LUA );
	lua_tinker::decl( spLua.get(), "g_pBattleFieldManager", this );
	lua_tinker::decl( spLua.get(), "MapData", this );

	if( 0 != luaL_dofile( spLua.get(), pFileName ) ) return false;

	return true;
}
bool KBattleFieldMapInfo::OpenScriptFile_Map( const char* pFileName )
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();

	lua_tinker::class_add<KBattleFieldMapInfo>( spLua.get(), "BattleFieldManager" );
	lua_tinker::class_def<KBattleFieldMapInfo>( spLua.get(), "AddMapData",				&KBattleFieldMapInfo::AddBattleFieldMinLevel_LUA );
	lua_tinker::decl( spLua.get(), "MapData", this );

	if( 0 != luaL_dofile( spLua.get(), pFileName ) ) return false;

	return true;
}

void KBattleFieldMapInfo::AddBattleFieldData_LUA()
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();
	KLuaManager kLuamanager( spLua.get() );

	int iMapID = -1;

	LUA_GET_VALUE( kLuamanager,	L"BATTLE_FIELD_ID",	iMapID, -1 );

	if( iMapID < 0 )
	{
		START_LOG( cerr, L"FIELD ID FAIL .!" )
			<< BUILD_LOG( iMapID )
			<< END_LOG;
		return;
	}

	std::map< int, int>::iterator mit = m_mapBattleFieldList.begin();
	for( ; mit != m_mapBattleFieldList.end() ; ++mit )
	{
		if( mit->second == iMapID )
		{
			START_LOG( cerr, L"FIELD ID 중복 !" )
				<< BUILD_LOG( iMapID )
				<< END_LOG;
			return;
		}
	}

	m_mapBattleFieldList.insert( std::make_pair( m_iMapCount++, iMapID ) );
	
}

void KBattleFieldMapInfo::AddBattleFieldMinLevel_LUA()
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();
	KLuaManager kLuamanager( spLua.get() );

	int iMapID = -1;
	int iMinLevel = -1;

	LUA_GET_VALUE( kLuamanager,	L"ID",	iMapID, -1 );

	if( iMapID < 0 )
	{
		START_LOG( cerr, L"FIELD ID FAIL .!" )
			<< BUILD_LOG( iMapID )
			<< END_LOG;
		return;
	}

	if( kLuamanager.BeginTable( L"REQUIRE_LEVEL" ) == S_OK )
	{
		LUA_GET_VALUE( kLuamanager,		"MIN",				iMinLevel,				-1		);

		kLuamanager.EndTable();
	}

	if( iMinLevel < 0 )
	{
		START_LOG( cerr, L"FIELD ID FAIL .!" )
			<< BUILD_LOG( iMapID )
			<< BUILD_LOG( iMinLevel )
			<< END_LOG;
		return;
	}
    	
	std::map< int, int>::iterator mit = m_mapBattleFieldMinLevel.find( iMapID );
	if( mit != m_mapBattleFieldMinLevel.end() )
	{
		START_LOG( cerr, L"FIELD ID 중복 !" )
			<< BUILD_LOG( iMapID )
			<< BUILD_LOG( iMinLevel )
			<< END_LOG;
		return;
	}

	m_mapBattleFieldMinLevel.insert( std::make_pair( iMinLevel, iMapID ) );

}

int KBattleFieldMapInfo::GetRandomBattleFieldID( int iLevel )
{
	return 40000;

	// [테스트]
	int iRandom = rand() % m_mapBattleFieldList.size();

	int iMapId = 0;

	if( m_mapBattleFieldList[ iRandom ] != NULL )
		iMapId = m_mapBattleFieldList[ iRandom ];

	std::map<int, int>::iterator mit = m_mapBattleFieldMinLevel.find( iLevel );
	if( mit->second < iMapId )
	{
		iMapId = m_mapBattleFieldList[ 0 ];
	}

	return iMapId;
}