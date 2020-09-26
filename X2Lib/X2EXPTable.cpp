#include "StdAfx.h"
#include ".\x2exptable.h"

CX2EXPTable::CX2EXPTable(void)
{
}

CX2EXPTable::~CX2EXPTable(void)
{
	m_mapExpTable.clear();
}


bool CX2EXPTable::OpenScriptFile( const WCHAR* wstrFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pkExpTable", this );
	return g_pKTDXApp->LoadLuaTinker( wstrFileName );
}

bool CX2EXPTable::AddExpTable_LUA()
{

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	EXPData expData;

	LUA_GET_VALUE( luaManager,	"LEVEL",		expData.m_Level ,		0 );
	LUA_GET_VALUE( luaManager,	"NEED_EXP",		expData.m_nNeedExp,		0 );
	LUA_GET_VALUE( luaManager,	"TOTAL_EXP",	expData.m_nTotalExp,	0 );

	if( (expData.m_Level != 1) && 
		(expData.m_Level == 0 || expData.m_nNeedExp == 0 || expData.m_nTotalExp == 0) )
	{
		return false;
	}

	m_mapExpTable.insert( std::make_pair( expData.m_Level, expData ) );

	return true;
}

CX2EXPTable::EXPData CX2EXPTable::GetEXPData( int level )
{
	EXPData expData;

	std::map<int,EXPData>::iterator iter;
	iter = m_mapExpTable.find( level );
	if( iter != m_mapExpTable.end() )
	{
		expData = iter->second;		
	}
	return expData;
}



int CX2EXPTable::GetLevel( int iNowEXP )
{
	EXPData expData;

	std::map<int,EXPData>::iterator iter;
	for( iter = m_mapExpTable.begin(); iter != m_mapExpTable.end(); iter++ )
	{
		expData = (EXPData) iter->second;
		// warning!! exp table에는 순차적으로 데이터가 들어있다고 가정하자
		if( iNowEXP < expData.m_nTotalExp )
		{
			return iter->first - 1;
		}
	}


	// warning!! 만렙 인 경우??
	std::map<int,EXPData>::reverse_iterator rit;
	rit = m_mapExpTable.rbegin();
	return (int) rit->first;

}













