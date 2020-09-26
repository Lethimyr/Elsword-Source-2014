#pragma once

#include "RefreshSingleton.h"
#include <set>
#include <map>
#include <KncUtil.h>
#include "Lottery.h"
#include "CommonPacket.h"
#include "X2Data/XSLDungeon.h"

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM


class KBadAttitudeTable
{
	DeclareRefreshSingleton( KBadAttitudeTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclToStringW;
	DeclDump;

public:
	KBadAttitudeTable(void);
	~KBadAttitudeTable(void);

	// 루아
	void AddBadAttitudeCutLinePoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint );
	void AddForceExitPoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint );

	// 던전 타입의 불량 유저 판단 점수
	int GetBadAttitudeCutLinePoint( CXSLDungeon::DUNGEON_TYPE eDungeonType );

	// 던전 타입의 강퇴 되는 추가 점수
	int GetForceExitPoint( CXSLDungeon::DUNGEON_TYPE eDungeonType );

private:

	std::map< CXSLDungeon::DUNGEON_TYPE, int >			m_mapBadAttitudeCutLinePoint;		// 던전 타입별 , 불량 유저로 판단되는 점수
	std::map< CXSLDungeon::DUNGEON_TYPE, int >			m_mapForceExitPoint;				// 던전 타입별 , 강퇴 되는 추가 점수
};

DefRefreshSingletonInline( KBadAttitudeTable );



#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}