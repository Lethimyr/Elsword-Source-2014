#include ".\kpvpresulttable.h"

#include "Enum/Enum.h"


//{{ 2011. 07. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
#else

ImplementSingleton( KPVPResultTable );

KPVPResultTable::KPVPResultTable(void)
{
	RegisterLuaBind();
}

KPVPResultTable::~KPVPResultTable(void)
{
	for( int i = 0; i < (int)RT_END; ++i )
	{
		m_mapResultData[i].end();
	}
}

void KPVPResultTable::RegisterLuaBind()
{
	//lua_tinker::class_<KPVPResultTable>("KStatTable")
	//	.def("SetTeamMatchWin_LUA",			KPVPResultTable::SetTeamMatchWin_LUA		)
	//	.def("SetTeamMatchLose_LUA",		KPVPResultTable::SetTeamMatchLose_LUA		)
	//	.def("SetDeathMatchKill_LUA",		KPVPResultTable::SetDeathMatchKill_LUA		)
	//	.def("SetDeathMatchDeath_LUA",		KPVPResultTable::SetDeathMatchDeath_LUA		)
	//	.def("SetGameOutPenalty_LUA",		KPVPResultTable::SetGameOutPenalty_LUA		)
	//	.def("SetTimeBonus_LUA",			KPVPResultTable::SetTimeBonus_LUA			)
	//	.def("dump",						KPVPResultTable::Dump						)
	//	;

	//lua_tinker::decl( "KPVPResultTable", this );
	lua_tinker::class_add<KPVPResultTable>( g_pLua, "KPVPResultTable" );
	lua_tinker::class_def<KPVPResultTable>( g_pLua, "SetTeamMatchWin_LUA",		&KPVPResultTable::SetTeamMatchWin_LUA );
	lua_tinker::class_def<KPVPResultTable>( g_pLua, "SetTeamMatchLose_LUA",		&KPVPResultTable::SetTeamMatchLose_LUA );
	lua_tinker::class_def<KPVPResultTable>( g_pLua, "SetDeathMatchKill_LUA",	&KPVPResultTable::SetDeathMatchKill_LUA );
	lua_tinker::class_def<KPVPResultTable>( g_pLua, "SetDeathMatchDeath_LUA",	&KPVPResultTable::SetDeathMatchDeath_LUA );
	lua_tinker::class_def<KPVPResultTable>( g_pLua, "SetGameOutPenalty_LUA",	&KPVPResultTable::SetGameOutPenalty_LUA );
	lua_tinker::class_def<KPVPResultTable>( g_pLua, "SetTimeBonus_LUA",			&KPVPResultTable::SetTimeBonus_LUA );
	lua_tinker::class_def<KPVPResultTable>( g_pLua, "dump",						&KPVPResultTable::Dump );

	lua_tinker::decl( g_pLua, "KPVPResultTable", this );
}

bool KPVPResultTable::OpenScriptFile( const char* strFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, strFileName ), return false );

	return true;
}

ImplToStringW( KPVPResultTable )
{
	/*
	for( int i = 0; i < CXSLUnit::UT_END; i++ )
	{
		stm_ << L"Character Class : " << i << std::endl;

		std::vector<KUnitStatInfo>::const_iterator vit;
		for( vit = m_vecStat[i].begin(); vit != m_vecStat[i].end(); vit++ )
		{
			stm_ << vit - m_vecStat[i].begin() << L" : "
				<< L"AtkPhysic = " << vit->m_fAtkPhysic
				<< L", AtkMagic = " << vit->m_fAtkMagic
				<< L", DefPhysic = " << vit->m_fDefPhysic
				<< L", DefMagic = " << vit->m_fDefMagic
				<< L", HP = " << vit->m_fHP
				<< std::endl;
		}

		stm_ << std::endl;
	}
	*/

	return stm_;
}


void KPVPResultTable::SetTeamMatchWin_LUA( int iPVPChannelClass, int VP, int EXP )
{
	SetTable( RT_TEAM_MATCH_WIN, iPVPChannelClass, VP, EXP );
}

void KPVPResultTable::SetTeamMatchLose_LUA( int iPVPChannelClass, int VP, int EXP )
{
	SetTable( RT_TEAM_MATCH_LOSE, iPVPChannelClass, VP, EXP );
}

void KPVPResultTable::SetDeathMatchKill_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_DEATH_MATCH_KILL, iUnitEmblem, VP, EXP );
}

void KPVPResultTable::SetDeathMatchDeath_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_DEATH_MATCH_DEATH, iUnitEmblem, VP, EXP );
}

void KPVPResultTable::SetGameOutPenalty_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_OUT_PENALTY, iUnitEmblem, VP, EXP );
}

void KPVPResultTable::SetTimeBonus_LUA( int iUnitEmblem, int VP, int EXP )
{
	SetTable( RT_TIME_BONUS, iUnitEmblem, VP, EXP );
}



const KPVPResultTable::ResultData* KPVPResultTable::GetTeamMatchWin( int iPVPChannelClass )
{
	return GetTable( RT_TEAM_MATCH_WIN, iPVPChannelClass );
}

const KPVPResultTable::ResultData* KPVPResultTable::GetTeamMatchLose( int iPVPChannelClass )
{
	return GetTable( RT_TEAM_MATCH_LOSE, iPVPChannelClass );
}

const KPVPResultTable::ResultData* KPVPResultTable::GetDeathMatchKill( CXSLUnit::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_DEATH_MATCH_KILL, (int)eEmblem );
}

const KPVPResultTable::ResultData* KPVPResultTable::GetDeathMatchDeath( CXSLUnit::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_DEATH_MATCH_DEATH, (int)eEmblem );
}

const KPVPResultTable::ResultData* KPVPResultTable::GetGameOutPenalty( CXSLUnit::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_OUT_PENALTY, (int)eEmblem );
}

const KPVPResultTable::ResultData* KPVPResultTable::GetTimeBonus( CXSLUnit::PVP_EMBLEM eEmblem )
{
	return GetTable( RT_TIME_BONUS, (int)eEmblem );
}


void KPVPResultTable::SetTable( RESULT_TYPE eResultType, int iKey, int iVP, int iEXP )
{
	if( eResultType >= RT_TEAM_MATCH_WIN && eResultType < RT_END )
	{
		ResultData sResultData;
		sResultData.m_VP	= iVP;
		sResultData.m_EXP	= iEXP;

		switch( eResultType )
		{
		case RT_TEAM_MATCH_WIN:
		case RT_TEAM_MATCH_LOSE:
			{
				//채널클래스 키값 체크
				if( KPVPChannelInfo::PCC_NONE >= iKey || KPVPChannelInfo::PCC_END <= iKey )
				{
					goto end_proc;
				}
			}
			break;

		case RT_DEATH_MATCH_KILL:
		case RT_DEATH_MATCH_DEATH:
		case RT_TIME_BONUS:
		case RT_OUT_PENALTY:
			{
				switch( iKey )
				{
				case CXSLUnit::PE_RECRUIT:
				case CXSLUnit::PE_GUARDS:
				case CXSLUnit::PE_MERCENARY:
				case CXSLUnit::PE_KNIGHTS:
				case CXSLUnit::PE_COMMANDER:
				case CXSLUnit::PE_EMPEROR:
				case CXSLUnit::PE_HERO:
				case CXSLUnit::PE_GOTOWAR:
					break;

				default:
					{
						goto end_proc;
					}
					break;
				}
			}
			break;
		}

		m_mapResultData[eResultType].insert( std::make_pair( iKey, sResultData ) );

		return;
	}

end_proc:
	START_LOG( cerr, L"대전결과 테이블 파싱 실패.!" )
		<< BUILD_LOG( eResultType )
		<< BUILD_LOG( iKey )
		<< END_LOG;
}

const KPVPResultTable::ResultData*  KPVPResultTable::GetTable( RESULT_TYPE eResultType, int iKey )
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

	START_LOG( cerr, L"대전결과 테이블 찾기 실패.!" )
		<< BUILD_LOG( eResultType )
		<< BUILD_LOG( iKey )
		<< END_LOG;

	return NULL;
}


#endif SERV_PVP_NEW_SYSTEM
//}}


