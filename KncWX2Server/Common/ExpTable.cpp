#include "ExpTable.h"
#include "BaseServer.h"

#ifdef SERV_MAX_LEVEL_CHECK // ±è¹Î¼º // Àû¿ë³¯Â¥: 2013-07-04
#include "GameSysVal/GameSysVal.h"
#endif // SERV_MAX_LEVEL_CHECK

NiImplementRootRTTI( KExpTable );
ImplementRefreshSingleton( KExpTable );

KExpTable::KExpTable()
{
}

KExpTable::~KExpTable()
{
}

ImplToStringW( KExpTable )
{
	stm_	<< L"----------[ Exp Table ]----------" << std::endl
			<< TOSTRINGW( m_mapExpTable.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KExpTable )
{
	lua_tinker::class_add<KExpTable>( GetLuaState(), "KExpTable" );
	lua_tinker::class_def<KExpTable>( GetLuaState(), "AddExpTable_LUA", &KExpTable::AddExpTable_LUA );

	lua_tinker::decl( GetLuaState(), "g_pkExpTable", this );
}

bool KExpTable::AddExpTable_LUA()
{
	KLuaManager luaManager( GetLuaState() );

	int		nLevel;
	sEXP	sExpData;

	LUA_GET_VALUE( luaManager,	"LEVEL",		nLevel,	0 );
	LUA_GET_VALUE( luaManager,	"NEED_EXP",		sExpData.nNeedExp,	0 );
	LUA_GET_VALUE( luaManager,	"TOTAL_EXP",	sExpData.nTotalExp,	0 );

	if( (nLevel != 1) && 
		(nLevel == 0 || sExpData.nNeedExp == 0 || sExpData.nTotalExp == 0) )
	{
		START_LOG( cerr, L"Level Table read fail" )
			<< BUILD_LOG( nLevel )
			<< BUILD_LOG( sExpData.nNeedExp )
			<< BUILD_LOG( sExpData.nTotalExp )
			<< END_LOG;

		return false;
	}

	m_mapExpTable.insert( std::make_pair( nLevel, sExpData ) );

	return true;
}

int KExpTable::GetRequireNeedExpbyLevel( IN int nLevel )
{
	std::map<int,sEXP>::iterator mit;

	mit = m_mapExpTable.find( nLevel );

	if( mit != m_mapExpTable.end() )
	{
		return mit->second.nNeedExp;
	}

	std::map<int,sEXP>::reverse_iterator mrit;
	mrit = m_mapExpTable.rbegin();

	if( mrit->first < nLevel )
		return mrit->second.nNeedExp;

	return 0;
}

int KExpTable::GetRequireTotalExpbyLevel( IN int nLevel )
{
	std::map<int,sEXP>::iterator mit;

	mit = m_mapExpTable.find( nLevel );

	if( mit != m_mapExpTable.end() )
	{
		return mit->second.nTotalExp;
	}

	std::map<int,sEXP>::reverse_iterator mrit;
	mrit = m_mapExpTable.rbegin();

	if( mrit->first < nLevel )
		return mrit->second.nTotalExp;

	return 0;
}

u_char KExpTable::CheckLevelUp( IN u_char nLevel, IN int nExp )
{
	if( nLevel < 1 )
		nLevel = 1;
	if( nExp < 0 )
		nExp = 0;

#ifdef SERV_MAX_LEVEL_CHECK // ±è¹Î¼º // Àû¿ë³¯Â¥: 2013-07-04
	u_char nResultLevel = nLevel;

	std::map<int,sEXP>::iterator mit;

	mit = m_mapExpTable.find( nLevel );
	for(; mit != m_mapExpTable.end(); ++mit )
	{
		if( nExp < mit->second.nTotalExp )
		{
			if( mit->first == nLevel )
			{
				nResultLevel = nLevel;
				goto end_funtion;
			}
			else
			{
				nResultLevel = mit->first - 1;
				goto end_funtion;
			}
		}
	}

	{
		std::map<int,sEXP>::reverse_iterator mrit;
		mrit = m_mapExpTable.rbegin();

		if( mrit->second.nTotalExp < nExp )
		{
			nResultLevel = mrit->first;
			goto end_funtion;
		}
	}
	
end_funtion:

	if( static_cast<u_char>(SiKGameSysVal()->GetLimitsLevel()) < nResultLevel )
		nResultLevel = static_cast<u_char>(SiKGameSysVal()->GetLimitsLevel());

	return nResultLevel;

#else

	std::map<int,sEXP>::iterator mit;

	mit = m_mapExpTable.find( nLevel );
	for(; mit != m_mapExpTable.end(); ++mit )
	{
		if( nExp < mit->second.nTotalExp )
		{
			if( mit->first == nLevel )
				return nLevel;
			else
				return mit->first - 1;
		}
	}

	std::map<int,sEXP>::reverse_iterator mrit;
	mrit = m_mapExpTable.rbegin();

	if( mrit->second.nTotalExp < nExp )
		return mrit->first;

	return nLevel;

#endif // SERV_MAX_LEVEL_CHECK
}


