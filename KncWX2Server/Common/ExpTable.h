#pragma once

#include <windows.h>
#include <RTTI.h>
#include <map>
#include "RefreshSingleton.h"


class KExpTable
{
	NiDeclareRootRTTI( KExpTable );
	DeclareRefreshSingleton( KExpTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	struct sEXP
	{
		int	nNeedExp;
		int	nTotalExp;
	};

public:
	KExpTable();
	virtual ~KExpTable();

	// for lua
	bool	AddExpTable_LUA();

	// result
	int		GetRequireNeedExpbyLevel( IN int nLevel );
	int		GetRequireTotalExpbyLevel( IN int nLevel );
	u_char	CheckLevelUp( IN u_char nLevel, IN int nExp );

protected:
	std::map< int, sEXP >	m_mapExpTable;
};

DefRefreshSingletonInline( KExpTable );

