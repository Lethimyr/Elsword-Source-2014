#pragma once

#include "RefreshSingleton.h"
#include "X2Data/XSLUnit.h"
#include <vector>
#include "Enum/Enum.h"


//{{ 2011. 07. 19	ÃÖÀ°»ç	´ëÀü °³Æí
#ifdef SERV_PVP_NEW_SYSTEM


class KPvpMatchResultTable
{
	DeclareRefreshSingleton( KPvpMatchResultTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum RESULT_TYPE
	{
		RT_TEAM_MATCH_WIN		= 0,
		RT_TEAM_MATCH_LOSE,
		RT_DEATH_MATCH_KILL,
		RT_DEATH_MATCH_DEATH,
		RT_TIME_BONUS,
		RT_OUT_PENALTY,	

		RT_END,
	};

public:
	KPvpMatchResultTable(void);
	~KPvpMatchResultTable(void);

	// for lua
	void	AddPvpExpRewardInfo_LUA( int iLevel, int iExp );

	// function
	//double	GetKFactor( IN const int iPvpPlayCount, IN const bool bIsWin, IN const bool bIsWinBeforeMatch, IN const int iUserMaxRating ) const;
	//double	GetExpectOfVictory( IN const int iMyTeamRating, IN const int iEnemyTeamRating ) const;

	//{{ 2012. 06. 21	¹Ú¼¼ÈÆ	2012 ´ëÀü ½ÃÁð2
#ifdef SERV_2012_PVP_SEASON2
	float	GetKFactor(			IN const int iPvpPlayCount,
								IN const bool bIsWinBeforeMatch, 
								IN const bool bIsWin,
								IN const int iUserRating,
								IN float fKFactor,
								IN const bool bIsRedistributionUser );

	int		GetRatingPvpResult( IN const int iMyTeamRating, 
								IN const int iEnemyTeamRating, 
								IN const int iUserRating, 
								IN const bool bIsWin, 
								IN const double& fKFactor,
								IN const bool bArrangeTeamsMatch
								);
#else
    int		GetRatingPvpResult( IN const int iMyTeamRating, 
								IN const int iEnemyTeamRating, 
								IN const int iUserRating, 
								IN const int iUserMaxRating, 
								IN const int iPvpPlayCount, 
								IN const bool bIsWin, 
								IN const bool bIsWinBeforeMatch, 
								IN const SEnum::MATCH_TYPE eMatchType,
								IN const SEnum::PVP_NPC_TYPE ePvpNpcType );
#endif SERV_2012_PVP_SEASON2
	//}}

	int		GetRPointPvpResult( IN const int iMyTeamRating, 
								IN const int iEnemyTeamRating, 
								IN const bool bIsWin, 
								IN const int iKillNum, 
								IN const int iMDKillNum, 
								IN const int iDeathNum,
								IN const SEnum::MATCH_TYPE eMatchType,
								IN const SEnum::PVP_NPC_TYPE ePvpNpcType );

	int		GetAPointPvpResult( IN const int iUserRating, 
								IN const bool bIsWin, 
								IN const int iKillNum, 
								IN const int iMDKillNum,
								IN const SEnum::MATCH_TYPE eMatchType,
								IN const SEnum::PVP_NPC_TYPE ePvpNpcType );

	int		GetExpPvpResult( IN const int iLevel, 
								IN const bool bIsWin, 
								IN const int iKillNum, 
								IN const int iMDKillNum,
								IN const SEnum::MATCH_TYPE eMatchType,
								IN const SEnum::PVP_NPC_TYPE ePvpNpcType );

	// penalty
	int		GetRPointPvpPenalty();
	int		GetAPointPvpPenalty( IN const int iUserRating, IN const SEnum::MATCH_TYPE eMatchType, IN const SEnum::PVP_NPC_TYPE ePvpNpcType );

protected:
	int		GetExpByLevel( IN const int iLevel ) const;

protected:
	std::map< int, int >	m_mapPvpExpReward;
};

DefRefreshSingletonInline( KPvpMatchResultTable );


#endif SERV_PVP_NEW_SYSTEM
//}}


