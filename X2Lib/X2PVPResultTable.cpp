#include "StdAfx.h"
#include ".\x2pvpresulttable.h"

CX2PVPResultTable::CX2PVPResultTable(void)
{
	RegisterLuaBind();
	
}

CX2PVPResultTable::~CX2PVPResultTable(void)
{
	for( int i = 0; i < (int)RT_END; ++i )
	{
		m_mapResultData[i].end();
	}
}

void CX2PVPResultTable::RegisterLuaBind()
{
	lua_State* L = g_pKTDXApp->GetLuaBinder()->GetLuaState();

	lua_tinker::class_add<CX2PVPResultTable>( L, "CX2PVPResultTable" );
#ifdef CONVERSION_VS
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetTeamMatchWin_LUA",			&CX2PVPResultTable::SetTeamMatchWin_LUA		);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetTeamMatchLose_LUA",		&CX2PVPResultTable::SetTeamMatchLose_LUA		);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetDeathMatchKill_LUA",		&CX2PVPResultTable::SetDeathMatchKill_LUA	);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetDeathMatchDeath_LUA",		&CX2PVPResultTable::SetDeathMatchDeath_LUA	);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetGameOutPenalty_LUA",		&CX2PVPResultTable::SetGameOutPenalty_LUA	);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetTimeBonus_LUA",			&CX2PVPResultTable::SetTimeBonus_LUA			);
#else CONVERSION_VS
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetTeamMatchWin_LUA",			CX2PVPResultTable::SetTeamMatchWin_LUA		);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetTeamMatchLose_LUA",		CX2PVPResultTable::SetTeamMatchLose_LUA		);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetDeathMatchKill_LUA",		CX2PVPResultTable::SetDeathMatchKill_LUA	);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetDeathMatchDeath_LUA",		CX2PVPResultTable::SetDeathMatchDeath_LUA	);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetGameOutPenalty_LUA",		CX2PVPResultTable::SetGameOutPenalty_LUA	);
	lua_tinker::class_def<CX2PVPResultTable>( L, "SetTimeBonus_LUA",			CX2PVPResultTable::SetTimeBonus_LUA			);
#endif CONVERSION_VS
}

bool CX2PVPResultTable::OpenScriptFile( const WCHAR* wstrFileName )
{
	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "KPVPResultTable", this );

	return g_pKTDXApp->LoadLuaTinker( wstrFileName );
}


void CX2PVPResultTable::SetTeamMatchWin_LUA( int iPVPChannelClass, int VP, int EXP )
{
	SetTable( RT_TEAM_MATCH_WIN, iPVPChannelClass, VP, EXP );
}

void CX2PVPResultTable::SetTeamMatchLose_LUA( int iPVPChannelClass, int VP, int EXP )
{
	SetTable( RT_TEAM_MATCH_LOSE, iPVPChannelClass, VP, EXP );
}

void CX2PVPResultTable::SetDeathMatchKill_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_DEATH_MATCH_KILL, iUnitEmblem, VP, EXP );
}

void CX2PVPResultTable::SetDeathMatchDeath_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_DEATH_MATCH_DEATH, iUnitEmblem, VP, EXP );
}

void CX2PVPResultTable::SetGameOutPenalty_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_OUT_PENALTY, iUnitEmblem, VP, EXP );
}

void CX2PVPResultTable::SetTimeBonus_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_TIME_BONUS, iUnitEmblem, VP, EXP );
}

const CX2PVPResultTable::ResultData* CX2PVPResultTable::GetTeamMatchWin( int iPVPChannelClass )
{
	return GetTable( RT_TEAM_MATCH_WIN, iPVPChannelClass );
}

const CX2PVPResultTable::ResultData* CX2PVPResultTable::GetTeamMatchLose( int iPVPChannelClass )
{
	return GetTable( RT_TEAM_MATCH_LOSE, iPVPChannelClass );
}

const CX2PVPResultTable::ResultData* CX2PVPResultTable::GetDeathMatchKill( CX2PVPEmblem::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_DEATH_MATCH_KILL, (int)eEmblem );
}

const CX2PVPResultTable::ResultData* CX2PVPResultTable::GetDeathMatchDeath( CX2PVPEmblem::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_DEATH_MATCH_DEATH, (int)eEmblem );
}

const CX2PVPResultTable::ResultData* CX2PVPResultTable::GetGameOutPenalty( CX2PVPEmblem::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_OUT_PENALTY, (int)eEmblem );
}

const CX2PVPResultTable::ResultData* CX2PVPResultTable::GetTimeBonus( CX2PVPEmblem::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_TIME_BONUS, (int)eEmblem );
}


void CX2PVPResultTable::SetTable( RESULT_TYPE eResultType, int iKey, int iVP, int iEXP )
{
	if( eResultType >= RT_TEAM_MATCH_WIN && eResultType < RT_END )
	{
		ResultData sResultData;
		sResultData.m_VP	= iVP;
		sResultData.m_EXP	= iEXP;

		m_mapResultData[eResultType].insert( std::make_pair( iKey, sResultData ) );
	}
}

const CX2PVPResultTable::ResultData*  CX2PVPResultTable::GetTable( RESULT_TYPE eResultType, int iKey )
{
	if( eResultType >= RT_TEAM_MATCH_WIN && eResultType < RT_END )
	{
		std::map<int, ResultData>::iterator mit;

		mit = m_mapResultData[eResultType].find( iKey );

		if( mit != m_mapResultData[eResultType].end() )
		{
			return &(mit->second);
		}
	}

	return NULL;
}


