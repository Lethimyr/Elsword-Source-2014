#include "BadAttitudeTable.h"
#include "BaseServer.h"


//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM

ImplementRefreshSingleton( KBadAttitudeTable );

KBadAttitudeTable::KBadAttitudeTable(void)
{
}

KBadAttitudeTable::~KBadAttitudeTable(void)
{
}

ImplToStringW( KBadAttitudeTable )
{
	stm_	<< L"----------[ KBadAttitudeTable ]----------" << std::endl
//			<< TOSTRINGW( m_mapHenirReward.size() )
			;

	return stm_;
}

ImplementLuaScriptParser( KBadAttitudeTable )
{
	lua_tinker::class_add<KBadAttitudeTable>( GetLuaState(), "BadAttitudeTable" );
	lua_tinker::class_def<KBadAttitudeTable>( GetLuaState(), "dump",							&KBadAttitudeTable::Dump );
	lua_tinker::class_def<KBadAttitudeTable>( GetLuaState(), "AddBadAttitudeCutLinePoint",		&KBadAttitudeTable::AddBadAttitudeCutLinePoint_LUA );
	lua_tinker::class_def<KBadAttitudeTable>( GetLuaState(), "AddForceExitPoint",				&KBadAttitudeTable::AddForceExitPoint_LUA );


	lua_tinker::decl( GetLuaState(), "BadAttitudeTable", this );
}

void KBadAttitudeTable::AddBadAttitudeCutLinePoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapBadAttitudeCutLinePoint.find( eDungeonType );
	if( mit != m_mapBadAttitudeCutLinePoint.end() )	// 있다면 중복인데...
	{
		START_LOG( cerr, L"(불량유저)존재하는 던전 타입 입니다. 중복으로 셋팅 하려고 합니다. 데이터를 교체 합니다." )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( mit->second )
			<< BUILD_LOG( iPoint )
			<< END_LOG;

		mit->second = iPoint;
	}
	else	// 없으니 추가한다.
	{
		m_mapBadAttitudeCutLinePoint.insert( std::make_pair( eDungeonType, iPoint ) );

		START_LOG( cout, L"던전 타입별 불량유저 포인트 셋팅 성공!" )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( iPoint )
			<< END_LOG;
	}
}

void KBadAttitudeTable::AddForceExitPoint_LUA( CXSLDungeon::DUNGEON_TYPE eDungeonType, int iPoint )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapForceExitPoint.find( eDungeonType );
	if( mit != m_mapForceExitPoint.end() )	// 있다면 중복인데...
	{
		START_LOG( cerr, L"(강퇴 추가 점수)존재하는 던전 타입 입니다. 중복으로 셋팅 하려고 합니다. 데이터를 교체 합니다." )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( mit->second )
			<< BUILD_LOG( iPoint )
			<< END_LOG;

		mit->second = iPoint;
	}
	else	// 없으니 추가한다.
	{
		m_mapForceExitPoint.insert( std::make_pair( eDungeonType, iPoint ) );

		START_LOG( cout, L"던전 타입별 강퇴 추가 포인트 셋팅 성공!" )
			<< BUILD_LOG( static_cast<int>(eDungeonType) )
			<< BUILD_LOG( iPoint )
			<< END_LOG;
	}
}

int KBadAttitudeTable::GetBadAttitudeCutLinePoint( CXSLDungeon::DUNGEON_TYPE eDungeonType )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapBadAttitudeCutLinePoint.find( eDungeonType );
	if( mit != m_mapBadAttitudeCutLinePoint.end() )
	{
		return mit->second;
	}

	return 1000;
}

int KBadAttitudeTable::GetForceExitPoint( CXSLDungeon::DUNGEON_TYPE eDungeonType )
{
	std::map< CXSLDungeon::DUNGEON_TYPE, int >::iterator mit = m_mapForceExitPoint.find( eDungeonType );
	if( mit != m_mapForceExitPoint.end() )
	{
		return mit->second;
	}

	return 1000;
}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

