#pragma once

#include "RefreshSingleton.h"
#include <set>


class KPvpMatchScriptManager
{
	DeclareRefreshSingleton( KPvpMatchScriptManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
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

public:
	KPvpMatchScriptManager(void);
	~KPvpMatchScriptManager(void);

	// for lua
	void	SetCheckWaitUserCount_LUA( int iCheckCount );
	void	SetGapMinMaxRating_LUA( int iGapMinMaxRating );
	bool	AddPvpMatchInfo_LUA( int iMatchType, int iKillNum, float fPlayTime );
	bool	AddPvpMapInfo_LUA( int iAddMapID );

	// function
	int		GetCheckWaitUserCount() const	{ return m_iCheckWaitUserCount; }
	int		GetGapMinMaxRating() const		{ return m_iGapMinMaxRating; }
	bool	IsExistMapID( IN const short sMapID ) const	{ return ( m_setPvpMapInfo.find( sMapID ) != m_setPvpMapInfo.end() ); }
    bool	GetPvpMatchInfo( IN const int iMatchType, OUT char& cWinKillNum, OUT float& fPlaytime ) const;
	bool	DecisionPvpMap( IN const std::map< short, int >& mapAllPlayersSelectedMap, OUT short& sMapID ) const;

private:
	int								m_iCheckWaitUserCount;
	int								m_iGapMinMaxRating;
	std::map< int, SPvpMatchInfo >	m_mapPvpMatchInfo;
	std::vector< int >				m_vecPvpMapInfo;
	std::set< int >					m_setPvpMapInfo;
	short							m_sDefaultPvpMapID;
};

DefRefreshSingletonInline( KPvpMatchScriptManager );


