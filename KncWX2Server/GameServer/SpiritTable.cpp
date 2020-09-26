#include ".\spirittable.h"

#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"

ImplementSingleton( KSpiritTable );

KSpiritTable::KSpiritTable(void)
{
	//lua_tinker::class_<KSpiritTable>("KSpiritTable")		
	//	.def("SetMaxSpirit",				KSpiritTable::SetMaxSpirit_LUA)
	//	.def("AddSpiritTable_LUA",			KSpiritTable::AddSpiritTable_LUA);

	//lua_tinker::decl( "g_pkSpiritTable", this );
	lua_tinker::class_add<KSpiritTable>( g_pLua, "KSpiritTable" );
	lua_tinker::class_def<KSpiritTable>( g_pLua, "SetMaxSpirit",		 &KSpiritTable::SetMaxSpirit_LUA );
	lua_tinker::class_def<KSpiritTable>( g_pLua, "AddSpiritTable_LUA",	 &KSpiritTable::AddSpiritTable_LUA );

	lua_tinker::decl( g_pLua, "g_pkSpiritTable", this );
}

KSpiritTable::~KSpiritTable(void)
{
	m_mapSpiritTable.clear();
}

bool KSpiritTable::OpenScriptFile( const char* pFileName )
{	
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}

bool KSpiritTable::SetMaxSpirit_LUA()
{
	KLuaManager luaManager( g_pLua );

	LUA_GET_VALUE_RETURN( luaManager,	"MAX_SPIRIT",		m_iMaxSpirit,		0, return false; );

	return true;
}

bool KSpiritTable::AddSpiritTable_LUA()
{
	KLuaManager luaManager( g_pLua );

	int	nLevel;
	int	nSpirit;

	LUA_GET_VALUE( luaManager,	"LEVEL",		nLevel,		0 );
	LUA_GET_VALUE( luaManager,	"SPIRIT",		nSpirit,	0 );

	if( nLevel <= 0  ||  nSpirit < 0 )
	{
		START_LOG( cerr, L"Spirit Table read fail" )
			<< BUILD_LOG( nLevel )
			<< BUILD_LOG( nSpirit )
			<< END_LOG;

		return false;
	}

	m_mapSpiritTable.insert( std::make_pair( nLevel, nSpirit ) );

	return true;
}

bool KSpiritTable::GetSpiritByLevel( IN int nLevel, OUT int& nSpirit )
{
	std::map< int, int >::iterator mit = m_mapSpiritTable.find( nLevel );
	if( mit == m_mapSpiritTable.end() )
	{
		return false;
	}

	nSpirit = mit->second;
	return true;
}


