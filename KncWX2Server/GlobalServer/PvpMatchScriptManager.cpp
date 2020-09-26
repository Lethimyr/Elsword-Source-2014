#include ".\PvpMatchScriptManager.h"
#include "NetError.h"
#include "BaseServer.h"
#include "Lottery.h"


ImplementRefreshSingleton( KPvpMatchScriptManager );

KPvpMatchScriptManager::KPvpMatchScriptManager(void)
{
	m_sDefaultPvpMapID = 0;
}

KPvpMatchScriptManager::~KPvpMatchScriptManager(void)
{
}

ImplToStringW( KPvpMatchScriptManager )
{
	stm_	<< L"----------[ Pvp Match Script Manager ]----------" << std::endl
			<< TOSTRINGW( m_mapPvpMatchInfo.size() )
			<< TOSTRINGW( m_vecPvpMapInfo.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KPvpMatchScriptManager )
{
	lua_tinker::class_add<KPvpMatchScriptManager>( GetLuaState(), "KPvpMatchScriptManager" );	
	lua_tinker::class_def<KPvpMatchScriptManager>( GetLuaState(), "SetCheckWaitUserCount",	&KPvpMatchScriptManager::SetCheckWaitUserCount_LUA );
	lua_tinker::class_def<KPvpMatchScriptManager>( GetLuaState(), "SetGapMinMaxRating",		&KPvpMatchScriptManager::SetGapMinMaxRating_LUA );
	lua_tinker::class_def<KPvpMatchScriptManager>( GetLuaState(), "AddPvpMatchInfo",		&KPvpMatchScriptManager::AddPvpMatchInfo_LUA );
	lua_tinker::class_def<KPvpMatchScriptManager>( GetLuaState(), "AddPvpMapInfo",			&KPvpMatchScriptManager::AddPvpMapInfo_LUA );
	lua_tinker::class_def<KPvpMatchScriptManager>( GetLuaState(), "dump",					&KPvpMatchScriptManager::Dump );

	lua_tinker::decl( GetLuaState(), "PvpMatchScriptManager", this );
}

void KPvpMatchScriptManager::SetCheckWaitUserCount_LUA( int iCheckCount )
{
	if( iCheckCount < 0 )
	{
		START_LOG( cerr, L"매치 정보가 이상합니다." )
			<< BUILD_LOG( iCheckCount )
			<< END_LOG;
		return;
	}

	m_iCheckWaitUserCount = iCheckCount;
}

void KPvpMatchScriptManager::SetGapMinMaxRating_LUA( int iGapMinMaxRating )
{
	if( iGapMinMaxRating < 0 )
	{
		START_LOG( cerr, L"매치 정보가 이상합니다." )
			<< BUILD_LOG( iGapMinMaxRating )
			<< END_LOG;
		return;
	}

	m_iGapMinMaxRating = iGapMinMaxRating;    
}

bool KPvpMatchScriptManager::AddPvpMatchInfo_LUA( int iMatchType, int iKillNum, float fPlayTime )
{
	if( iMatchType < 0  ||  iKillNum < 0  ||  fPlayTime < 0.f )
	{
		START_LOG( cerr, L"매치 정보가 이상합니다." )
			<< BUILD_LOG( iMatchType )
			<< BUILD_LOG( iKillNum )
			<< BUILD_LOG( fPlayTime )
			<< END_LOG;
		return false;
	}

	SPvpMatchInfo kInfo;
	kInfo.m_iKillNum = iKillNum;
	kInfo.m_fPlayTime = fPlayTime;
	m_mapPvpMatchInfo.insert( std::make_pair( iMatchType, kInfo ) );
	return true;
}

bool KPvpMatchScriptManager::AddPvpMapInfo_LUA( int iAddMapID )
{
	if( IsExistMapID( iAddMapID ) == true )
	{
		START_LOG( cerr, L"동일한 맵이 또 있네?" )
			<< BUILD_LOG( iAddMapID )
			<< END_LOG;
		return false;
	}

	// 가장 처음 등록하는 대전맵이 디폴트 맵ID가 된다.
	if( m_sDefaultPvpMapID == 0 )
	{
		m_sDefaultPvpMapID = iAddMapID;
	}
	
	m_vecPvpMapInfo.push_back( iAddMapID );
	m_setPvpMapInfo.insert( iAddMapID );
	return true;
}

bool KPvpMatchScriptManager::GetPvpMatchInfo( IN const int iMatchType, OUT char& cWinKillNum, OUT float& fPlaytime ) const
{
	cWinKillNum = 0;
	fPlaytime = 0;

	std::map< int, SPvpMatchInfo >::const_iterator mit;
	mit = m_mapPvpMatchInfo.find( iMatchType );
	if( mit == m_mapPvpMatchInfo.end() )
	{
        START_LOG( cerr, L"해당 매치 타입에 대한 정보가 없습니다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iMatchType )
			<< END_LOG;
		return false;
	}

	cWinKillNum = static_cast<char>(mit->second.m_iKillNum);
	fPlaytime = mit->second.m_fPlayTime;
	return true;
}

bool KPvpMatchScriptManager::DecisionPvpMap( IN const std::map< short, int >& mapAllPlayersSelectedMap, OUT short& sMapID ) const
{
	sMapID = 0;
	std::map< short, int > mapSelectedMap = mapAllPlayersSelectedMap;
	KLotterySameProb kPvpMapRandom;

	BOOST_TEST_FOREACH( const int, iMapID, m_vecPvpMapInfo )
	{
		// 유저들이 선택한 맵들은 제외한다!
		if( mapSelectedMap.find( iMapID ) != mapSelectedMap.end() )
			continue;

		// 하나씩 넣자!
		kPvpMapRandom.AddCaseSameProb( iMapID );
	}

	// 근데 만약에 비어있다면?
	if( kPvpMapRandom.Empty() == true )
	{
		short sMaxSelectedMapID = 0;
		int iMaxSelectedCount = 0;
		std::map< short, int >::const_iterator mit;
		for( mit = mapSelectedMap.begin(); mit != mapSelectedMap.end(); ++mit )
		{
            if( iMaxSelectedCount < mit->second )
			{
                iMaxSelectedCount = mit->second;
				sMaxSelectedMapID = mit->first;
			}
		}

		// 가장 많은 선택을 받은 맵을 제외한다!
		mapSelectedMap.erase( sMaxSelectedMapID );

		// 남은 맵들로 랜덤 정보를 만들자!
		for( mit = mapSelectedMap.begin(); mit != mapSelectedMap.end(); ++mit )
		{
			if( IsExistMapID( mit->first ) == false )
			{
				START_LOG( cerr, L"어? 존재하지 않는 MapID가 있네? 일어나서는 안되는 에러!" )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			kPvpMapRandom.AddCaseSameProb( mit->first );
		}

		if( kPvpMapRandom.Empty() == true )
		{
			START_LOG( cerr, L"그래도 맵 랜덤 정보가 비어있네? 서버 코드 오류거나 기획상 잘못된걸거야! 일어나서는 안되는 에러다!" )
				<< BUILD_LOG( m_vecPvpMapInfo.size() )
				<< BUILD_LOG( mapSelectedMap.size() )
				<< END_LOG;
			return false;
		}
	}

	// 대전맵을 결정하자!
	sMapID = kPvpMapRandom.DecisionSameProb();
	if( IsExistMapID( sMapID ) == false )
	{
		START_LOG( cerr, L"랜덤으로 결정된 대전맵이 등록된 대전맵이 아니네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( sMapID )
			<< END_LOG;

		sMapID = m_sDefaultPvpMapID;
	}

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout, L"[테스트로그] 대전 맵 결정!" )
		<< BUILD_LOG( sMapID )
		<< BUILD_LOG( mapAllPlayersSelectedMap.size() )
		<< BUILD_LOG( kPvpMapRandom.Size() );
	//////////////////////////////////////////////////////////////////////////	

	return true;
}



