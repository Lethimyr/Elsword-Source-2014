#include ".\pvpmatchresulttable.h"
#include "Enum/Enum.h"
#include <math.h>


//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


ImplementRefreshSingleton( KPvpMatchResultTable );

KPvpMatchResultTable::KPvpMatchResultTable(void)
{
}

KPvpMatchResultTable::~KPvpMatchResultTable(void)
{
}

ImplToStringW( KPvpMatchResultTable )
{
	stm_	<< L"----------[ Pvp Match Result Table ]----------" << std::endl
			<< TOSTRINGW( m_mapPvpExpReward.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( KPvpMatchResultTable )
{
	lua_tinker::class_add<KPvpMatchResultTable>( GetLuaState(), "KPvpMatchResultTable" );	
	lua_tinker::class_def<KPvpMatchResultTable>( GetLuaState(), "AddPvpExpRewardInfo",		&KPvpMatchResultTable::AddPvpExpRewardInfo_LUA );
	lua_tinker::class_def<KPvpMatchResultTable>( GetLuaState(), "dump",						&KPvpMatchResultTable::Dump );

	lua_tinker::decl( GetLuaState(), "KPvpMatchResultTable", this );
}

void KPvpMatchResultTable::AddPvpExpRewardInfo_LUA( int iLevel, int iExp )
{
    if( iLevel < 0  ||  iExp < 0 )
	{
		START_LOG( cerr, L"경험치 보상 테이블 값이 이상합니다." )
			<< BUILD_LOG( iLevel )
			<< BUILD_LOG( iExp )
			<< END_LOG;
		return;
	}
	
	m_mapPvpExpReward.insert( std::make_pair( iLevel, iExp ) );
}

//double	KPvpMatchResultTable::GetKFactor( IN const int iPvpPlayCount, IN const bool bIsWin, IN const bool bIsWinBeforeMatch, IN const int iUserMaxRating ) const
//{
//	// 최초 진입자
//    if( iPvpPlayCount <= 5 )
//	{
//		// 연승!
//		if( bIsWin == bIsWinBeforeMatch )
//		{
//            return 300.0;
//		}
//		else
//		{
//			return static_cast<double>(300.0 * 0.65);
//		}
//	}
//	else
//	{
//		if( iPvpPlayCount > 30  &&  iUserMaxRating > 2400 )
//		{
//            return 24.0;
//		}
//	}
//
//	return 32.0;
//}

//double KPvpMatchResultTable::GetExpectOfVictory( IN const int iMyTeamRating, IN const int iEnemyTeamRating ) const
//{
//	const double fTeamRatingGap = static_cast<double>(iEnemyTeamRating) - static_cast<double>(iMyTeamRating);
//	const double fMiddleResult = pow( 10.0, ( fTeamRatingGap / 400.0 ) );
//	const double fResult = ( 1.0 / ( 1.0 + fMiddleResult ) );
//
//	return fResult;
//}

//{{ 2012. 06. 21	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
float	KPvpMatchResultTable::GetKFactor(	IN const int iPvpPlayCount, 
											IN const bool bIsWinBeforeMatch, 
											IN const bool bIsWin, 
											IN const int iUserRating, 
											IN float fKFactor, 
											IN const bool bIsRedistributionUser )
{
	return lua_tinker::call<float>( GetLuaState(), "GET_K_FACTOR", iPvpPlayCount, bIsWinBeforeMatch, bIsWin, iUserRating, fKFactor, bIsRedistributionUser );
}

int KPvpMatchResultTable::GetRatingPvpResult( IN const int iMyTeamRating, 
											 IN const int iEnemyTeamRating, 
											 IN const int iUserRating, 
											 IN const bool bIsWin, 
											 IN const double& fKFactor,
											 IN const bool bArrangeTeamsMatch
											 )
{
	return lua_tinker::call<int>( GetLuaState(), "GET_RATING_PVP_RESULT", iMyTeamRating, iEnemyTeamRating, iUserRating, bIsWin, fKFactor, bArrangeTeamsMatch );
}
#else
int KPvpMatchResultTable::GetRatingPvpResult( IN const int iMyTeamRating, 
											  IN const int iEnemyTeamRating, 
											  IN const int iUserRating, 
											  IN const int iUserMaxRating, 
											  IN const int iPvpPlayCount, 
											  IN const bool bIsWin, 
											  IN const bool bIsWinBeforeMatch, 
											  IN const SEnum::MATCH_TYPE eMatchType, 
											  IN const SEnum::PVP_NPC_TYPE ePvpNpcType )
{
	// K상수값을 구하자!
	//const double fKFactor = GetKFactor( iPvpPlayCount, bIsWin, bIsWinBeforeMatch, iUserMaxRating );
	//const double fKFactor = lua_tinker::call<double>( GetLuaState(), "GET_K_FACTOR", iPvpPlayCount, bIsWin, bIsWinBeforeMatch, iUserMaxRating );
	//LIF( fKFactor == fKFactorS );

	// 기대승률을 구하자!
	//const double fExpectOfVictory = GetExpectOfVictory( iMyTeamRating, iEnemyTeamRating );
	//const double fExpectOfVictory = lua_tinker::call<double>( GetLuaState(), "GET_EXPECT_OF_VICTORY", iMyTeamRating, iEnemyTeamRating );
	//LIF( fExpectOfVictory == fExpectOfVictoryS );

	// 승패값을 구하자!
	//const double fMyTeamResultFactor = ( bIsWin ? 1.0 : 0.0 );

	//const double fRatingResult = static_cast<int>( (double)iMyTeamRating + ( fKFactor * ( fMyTeamResultFactor - fExpectOfVictory ) ) );
	//const int iResultForUser = static_cast<int>( fRatingResult - (double)iUserRating );
	const int iResultForUser = lua_tinker::call<int>( GetLuaState(), "GET_RATING_PVP_RESULT", iMyTeamRating, iEnemyTeamRating, iUserRating, iUserMaxRating, iPvpPlayCount, bIsWin, bIsWinBeforeMatch, eMatchType, ePvpNpcType );
    return iResultForUser;
}
#endif SERV_2012_PVP_SEASON2
//}}

int	KPvpMatchResultTable::GetRPointPvpResult( IN const int iMyTeamRating, 
											  IN const int iEnemyTeamRating, 
											  IN const bool bIsWin, 
											  IN const int iKillNum, 
											  IN const int iMDKillNum, 
											  IN const int iDeathNum, 
											  IN const SEnum::MATCH_TYPE eMatchType, 
											  IN const SEnum::PVP_NPC_TYPE ePvpNpcType )
{
	// 기대승률을 구하자!
	//const double fExpectOfVictory = GetExpectOfVictory( iMyTeamRating, iEnemyTeamRating );
	//const double fExpectOfVictory = lua_tinker::call<double>( GetLuaState(), "GET_EXPECT_OF_VICTORY", iMyTeamRating, iEnemyTeamRating );
	//LIF( fExpectOfVictory == fExpectOfVictoryS );

	// 승패 RPoint를 구하자!
	//int iWinLoseRPoint = 0;
	//if( bIsWin )
	//{
 //       iWinLoseRPoint = static_cast<int>( 12.0 * ( 1.0 - fExpectOfVictory ) );
	//}
	//else
	//{
	//	iWinLoseRPoint = static_cast<int>( -12.0 * fExpectOfVictory );
	//}

    // 킬데스 RPoint를 구하자!
	//const int iKillDeathRPoint = min( 12, ( iKillNum + iMDKillNum - iDeathNum ) );
	//
	//const int iRPointResult = iWinLoseRPoint + iKillDeathRPoint;
	const int iRPointResult = lua_tinker::call<int>( GetLuaState(), "GET_RANKING_POINT_PVP_RESULT", iMyTeamRating, iEnemyTeamRating, bIsWin, iKillNum, iMDKillNum, iDeathNum, eMatchType, ePvpNpcType );
	return iRPointResult;
}

int	KPvpMatchResultTable::GetAPointPvpResult( IN const int iUserRating, 
											  IN const bool bIsWin, 
											  IN const int iKillNum, 
											  IN const int iMDKillNum, 
											  IN const SEnum::MATCH_TYPE eMatchType, 
											  IN const SEnum::PVP_NPC_TYPE ePvpNpcType )
{
	// 승패 APoint구하자!
	//int iWinLoseAPoint = 0;
	//if( bIsWin )
	//{
	//	iWinLoseAPoint = 4;
	//}
	//else
	//{
	//	iWinLoseAPoint = 0;
	//}

	// 킬데스 APoint구하자!
	//const int iKillDeathAPoint = max( 0, min( 5, iKillNum + iMDKillNum - iDeathNum ) );

	// Rating으로 APoint구하자!
	//const double fRatingAPoint = static_cast<double>(iUserRating) * 0.03;
    
	//const int iAPointResult = static_cast<int>( static_cast<double>( iWinLoseAPoint + iKillDeathAPoint ) * fRatingAPoint );
	const int iAPointResult = lua_tinker::call<int>( GetLuaState(), "GET_ARENA_POINT_PVP_RESULT", iUserRating, bIsWin, iKillNum, iMDKillNum, eMatchType, ePvpNpcType );
	return iAPointResult;
}

int	KPvpMatchResultTable::GetExpPvpResult( IN const int iLevel, 
										   IN const bool bIsWin, 
										   IN const int iKillNum, 
										   IN const int iMDKillNum, 
										   IN const SEnum::MATCH_TYPE eMatchType, 
										   IN const SEnum::PVP_NPC_TYPE ePvpNpcType )
{
	// 레벨별 경험치 얻기
	const double fExpByLevel = static_cast<double>( GetExpByLevel( iLevel ) );
	
	// 승패 상수
	//const double fWinLoseFactor = ( bIsWin ? 1.0 : 0.2 );

	// 킬데스 값
	//const double fKillDeathExp = ( 0.2 * static_cast<double>( min( ( iKillNum + iMDKillNum ), 5 ) ) );

	//const int iExpResult = static_cast<int>( fExpByLevel * ( fWinLoseFactor + fKillDeathExp ) );
	const int iExpResult = lua_tinker::call<int>( GetLuaState(), "GET_EXP_PVP_RESULT", fExpByLevel, bIsWin, iKillNum, iMDKillNum, eMatchType, ePvpNpcType );
    return iExpResult;
}

int	KPvpMatchResultTable::GetExpByLevel( IN const int iLevel ) const
{
	std::map< int, int >::const_iterator mit;
	mit = m_mapPvpExpReward.find( iLevel );
	if( mit == m_mapPvpExpReward.end() )
	{
		START_LOG( cerr, L"존재하지 않는 레벨별 경험치 정보입니다!" )
			<< BUILD_LOGc( iLevel )
			<< END_LOG;
        return 0;
	}

	return mit->second;
}

int	KPvpMatchResultTable::GetRPointPvpPenalty()
{
	const int iResult = lua_tinker::call<int>( GetLuaState(), "GET_RANKING_POINT_PVP_PENALTY" );
    return iResult;
}

int	KPvpMatchResultTable::GetAPointPvpPenalty( IN const int iUserRating, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType )
{
	//const int iResult = -( GetAPointPvpResult( iUserRating, true, 0, 0, ePvpNpcType ) * 5 );
	const int iResult = lua_tinker::call<int>( GetLuaState(), "GET_ARENA_POINT_PVP_PENALTY", iUserRating, eMatchType, ePvpNpcType );
	return iResult;
}

#endif SERV_PVP_NEW_SYSTEM
//}}


