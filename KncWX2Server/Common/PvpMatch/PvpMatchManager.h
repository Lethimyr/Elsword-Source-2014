#pragma once

#include "RefreshSingleton.h"
#include <set>
#include "KncUidType.h"
#include "CommonPacket.h"
#include <Lottery.h>
#include "Enum/Enum.h"


//{{ 2011. 07. 26	ÃÖÀ°»ç	´ëÀü °³Æí
#ifdef SERV_PVP_NEW_SYSTEM


class KPvpMatchManager
{
	DeclareRefreshSingleton( KPvpMatchManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum MATCH_WEIGHT_ENUM
	{
		MWE_LOW_RATING_PLAYER = 0,
		MWE_MIDDLE_RATING_PLAYER,
		MWE_HIGH_RATING_PLAYER,

		MWE_MAX,
	};

	struct SPvpMatchInfo
	{
        int		m_iKillNum;
		float	m_fPlayTime;

		SPvpMatchInfo()
		{
			m_iKillNum = 0;
			m_fPlayTime = 0.f;
		}
	};

	struct SUserRatingInfo
	{
		UidType					m_iUnitUID;
		int						m_iRating;

		SUserRatingInfo()
		{
			m_iUnitUID = 0;
			m_iRating = 0;
		}

		SUserRatingInfo( const UidType iUnitUID, const int iRating )
		{
			m_iUnitUID = iUnitUID;
			m_iRating = iRating;
		}
	};

	struct SPvpNpcInfo
	{
		SEnum::PVP_NPC_TYPE		m_ePvpNpcType;
		char					m_cUnitClass;
		u_char					m_ucLevel;
		std::wstring			m_wstrCharName;
		//{{ 2012. 09. 19	¹Ú¼¼ÈÆ	2012 ´ëÀü ½ÃÁð2
#ifdef SERV_2012_PVP_SEASON2
		char					m_cRank;
#endif SERV_2012_PVP_SEASON2
		//}}
		int						m_Rating;
		KStat					m_GameStat;
		int						m_iRewardTitleID;
		int						m_iRewardTitlePeriod;

		SPvpNpcInfo()
		{
			m_ePvpNpcType = SEnum::PNT_NONE;
			m_cUnitClass = 0;
			m_ucLevel = 0;
			//{{ 2012. 09. 19	¹Ú¼¼ÈÆ	2012 ´ëÀü ½ÃÁð2
#ifdef SERV_2012_PVP_SEASON2
			m_cRank	= 0;
#endif SERV_2012_PVP_SEASON2
			//}}
			m_Rating = 0;
			m_iRewardTitleID = 0;
			m_iRewardTitlePeriod = 0;
		}
	};

public:
	KPvpMatchManager(void);
	~KPvpMatchManager(void);

	// for lua
	void	SetMaxRankingPoint_LUA( int iMaxRankingPoint );
	void	SetMaxArenaPoint_LUA( int iMaxArenaPoint );
	void	SetMatchMakingBeginCount_LUA( int iMatchMakingBeginCount );
	void	SetMatchWeightValue_LUA( int iEnum, int iWeightValue );
	void	SetCheckWaitUserCount_LUA( int iCheckCount );
	void	SetGapMinMaxRating_LUA( int iGapMinMaxRating );
	bool	AddPvpMatchInfo_LUA( int iMatchType, int iKillNum, float fPlayTime );
	bool	AddPvpMapForMatch_LUA( int iAddMapID );
	bool	AddPvpMapForNpcMatch_LUA( int iAddMapID );	
	void	AddPvpNpcInfo_LUA();
	void	AddPvpNpcName_LUA( const char* pName );
	void	SetHeroPvpNpcFactor_LUA( float fFactor );

	// Pvp Match
	int		GetMaxRankingPoint() const				{ return m_iMaxRankingPoint; }
	int		GetMaxArenaPoint() const				{ return m_iMaxArenaPoint; }
	int		GetMatchMakingBeginCount() const		{ return m_iMatchMakingBeginCnt; }
	int		GetCheckWaitUserCount() const			{ return m_iCheckWaitUserCount; }
	int		GetGapMinMaxRating() const				{ return m_iGapMinMaxRating; }	
    bool	GetPvpMatchInfo( IN const int iMatchType, OUT char& cWinKillNum, OUT float& fPlaytime ) const;
	int		GetMatchWeightValue( IN const MATCH_WEIGHT_ENUM eEnum ) const;
	int		GetRatingByTeam( IN const std::map< UidType, int >& mapTeamMemberInfo ) const;

	// Pvp Map
	bool	IsExistMapID( IN const short sMapID ) const	{ return ( m_setPvpMapForMatch.find( sMapID ) != m_setPvpMapForMatch.end() ); }
	bool	DecisionPvpMap( IN const bool bIsNpcPvpMode, IN const std::map< short, int >& mapAllPlayersSelectedMap, OUT short& sMapID ) const;

	// Pvp Npc
	bool	IsPvpNpc( IN const int iNpcID ) const	{ return ( m_mapPvpNpcInfo.find( iNpcID ) != m_mapPvpNpcInfo.end() ); }
	bool	IsExistPvpNpcName( IN const std::wstring& wstrNickName ) const;
	bool	DecisionNpcPvpMode( IN const std::vector< KRoomUserInfo >& vecTeamMember ) const;
	bool	GetPvpNpcInfo( IN const bool bIsAllPlayersBeginner, IN const int iMyTeamRatingWeightAverage, IN const u_int uiPvpPlayerCount, OUT std::vector< KRoomUserInfo >& vecPvpNpcTeam, OUT std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo ) const;
	const SPvpNpcInfo* GetPvpNpcInfo( IN const int iNpcID ) const;
	void	GetPvpNpcNameByRandom( OUT std::wstring& wstrName ) const;
	float	GetHeroPvpNpcFactor() const				{ return m_fHeroPvpNpcFactor; }

private:
	// PVP Match
	int								m_iMaxRankingPoint;
	int								m_iMaxArenaPoint;
	int								m_iMatchMakingBeginCnt;
	int								m_iCheckWaitUserCount;
	int								m_iGapMinMaxRating;
	std::map< int, SPvpMatchInfo >	m_mapPvpMatchInfo;
	int								m_arrMatchWeightValue[MWE_MAX];

	// PVP Map
	std::vector< int >				m_vecPvpMapInfo;
	std::set< int >					m_setPvpMapForMatch;
	KLotterySameProb< int >			m_kLotPvpMapForNpcMatch;
	short							m_sDefaultPvpMapID;

	// PVP Npc
	std::map< int, SPvpNpcInfo >	m_mapPvpNpcInfo;
	KLotterySameProb< int >			m_kLotPvpNpc[SEnum::PNT_MAX];
	KLotterySameProb< int >			m_kLotPvpNpcName;
	std::map< int, std::wstring >	m_mapPvpNpcName;
	std::vector< std::wstring >		m_vecPvpNpcName;
	float							m_fHeroPvpNpcFactor;
};

DefRefreshSingletonInline( KPvpMatchManager );


#endif SERV_PVP_NEW_SYSTEM
//}}


