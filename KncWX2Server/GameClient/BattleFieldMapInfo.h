#pragma once

#include <KNCSingleton.h>
#include "lottery.h"

class KBattleFieldMapInfo
{
	DeclareSingleton( KBattleFieldMapInfo );

public:
	KBattleFieldMapInfo(void);
	~KBattleFieldMapInfo(void);

	bool OpenScriptFile( const char* pFileName );
	bool OpenScriptFile_Map( const char* pFileName );
	void AddBattleFieldData_LUA();
	void AddBattleFieldMinLevel_LUA();
	int GetRandomBattleFieldID( int iLevel );

private:
	int							m_iMapCount;
	std::map< int, int >		m_mapBattleFieldList;
	std::map< int, int >		m_mapBattleFieldMinLevel;
};

DefSingletonInline( KBattleFieldMapInfo );