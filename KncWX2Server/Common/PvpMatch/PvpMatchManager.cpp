#include ".\PvpMatchManager.h"
#include "NetError.h"
#include "BaseServer.h"
#include "Lottery.h"
#include "CompareLimitList.h"
#include "Enum/Enum.h"
#include "X2Data/XSLUnitManager.h"

//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


ImplementRefreshSingleton( KPvpMatchManager );

KPvpMatchManager::KPvpMatchManager(void) :
m_iMaxRankingPoint( 0 ),
m_iMaxArenaPoint( 0 ),
m_iMatchMakingBeginCnt( 0 ),
m_iCheckWaitUserCount( 0 ),
m_iGapMinMaxRating( 0 ),
m_sDefaultPvpMapID( 0 ),
m_fHeroPvpNpcFactor( 0.f )
{
}

KPvpMatchManager::~KPvpMatchManager(void)
{
}

ImplToStringW( KPvpMatchManager )
{
	stm_	<< L"----------[ Pvp Match Script Manager ]----------" << std::endl
			<< TOSTRINGW( m_iMaxRankingPoint )
			<< TOSTRINGW( m_iMaxArenaPoint )
			<< TOSTRINGW( m_iMatchMakingBeginCnt )
			<< TOSTRINGW( m_iCheckWaitUserCount )
			<< TOSTRINGW( m_iGapMinMaxRating )
			<< TOSTRINGW( m_mapPvpMatchInfo.size() )
			<< TOSTRINGW( m_vecPvpMapInfo.size() )
			<< TOSTRINGW( m_setPvpMapForMatch.size() )
			<< TOSTRINGW( m_sDefaultPvpMapID )
			<< TOSTRINGW( m_arrMatchWeightValue[MWE_LOW_RATING_PLAYER] )
			<< TOSTRINGW( m_arrMatchWeightValue[MWE_MIDDLE_RATING_PLAYER] )
			<< TOSTRINGW( m_arrMatchWeightValue[MWE_HIGH_RATING_PLAYER] )
			<< TOSTRINGW( m_mapPvpNpcInfo.size() )
			<< TOSTRINGW( m_mapPvpNpcName.size() )
			<< TOSTRINGW( m_fHeroPvpNpcFactor )
			;

	return stm_;
}

ImplementLuaScriptParser( KPvpMatchManager )
{
	lua_tinker::class_add<KPvpMatchManager>( GetLuaState(), "KPvpMatchManager" );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMaxRankingPoint",		&KPvpMatchManager::SetMaxRankingPoint_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMaxArenaPoint",			&KPvpMatchManager::SetMaxArenaPoint_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMatchMakingBeginCount",	&KPvpMatchManager::SetMatchMakingBeginCount_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetMatchWeightValue",		&KPvpMatchManager::SetMatchWeightValue_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetCheckWaitUserCount",	&KPvpMatchManager::SetCheckWaitUserCount_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetGapMinMaxRating",		&KPvpMatchManager::SetGapMinMaxRating_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpMatchInfo",			&KPvpMatchManager::AddPvpMatchInfo_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpMapForMatch",		&KPvpMatchManager::AddPvpMapForMatch_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpMapForNpcMatch",		&KPvpMatchManager::AddPvpMapForNpcMatch_LUA );	
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpNpcInfo",			&KPvpMatchManager::AddPvpNpcInfo_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "AddPvpNpcName",			&KPvpMatchManager::AddPvpNpcName_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "SetHeroPvpNpcFactor",		&KPvpMatchManager::SetHeroPvpNpcFactor_LUA );
	lua_tinker::class_def<KPvpMatchManager>( GetLuaState(), "dump",						&KPvpMatchManager::Dump );

	lua_tinker::decl( GetLuaState(), "PvpMatchManager", this );
}

void KPvpMatchManager::SetMaxRankingPoint_LUA( int iMaxRankingPoint )
{
	if( iMaxRankingPoint < 0 )
	{
		START_LOG( cerr, L"최대 랭킹 포인트값이 이상합니다." )
			<< BUILD_LOG( iMaxRankingPoint )
			<< END_LOG;
		return;
	}

	m_iMaxRankingPoint = iMaxRankingPoint;
}

void KPvpMatchManager::SetMaxArenaPoint_LUA( int iMaxArenaPoint )
{
	if( iMaxArenaPoint < 0 )
	{
		START_LOG( cerr, L"최대 아레나 포인트값이 이상합니다." )
			<< BUILD_LOG( iMaxArenaPoint )
			<< END_LOG;
		return;
	}

	m_iMaxArenaPoint = iMaxArenaPoint;
}

void KPvpMatchManager::SetMatchMakingBeginCount_LUA( int iMatchMakingBeginCount )
{
	if( iMatchMakingBeginCount < 0 )
	{
		START_LOG( cerr, L"매치 메이킹 시작 카운트가 이상합니다." )
			<< BUILD_LOG( iMatchMakingBeginCount )
			<< END_LOG;
		return;
	}

	m_iMatchMakingBeginCnt = iMatchMakingBeginCount;
}

void KPvpMatchManager::SetMatchWeightValue_LUA( int iEnum, int iWeightValue )
{
	if( iEnum < 0  ||  iEnum >= MWE_MAX )
	{
		START_LOG( cerr, L"대전 매치 가중치 enum이 이상합니다!" )
			<< BUILD_LOG( iEnum )
			<< END_LOG;
		return;
	}

	m_arrMatchWeightValue[iEnum] = iWeightValue;
}

void KPvpMatchManager::SetCheckWaitUserCount_LUA( int iCheckCount )
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

void KPvpMatchManager::SetGapMinMaxRating_LUA( int iGapMinMaxRating )
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

bool KPvpMatchManager::AddPvpMatchInfo_LUA( int iMatchType, int iKillNum, float fPlayTime )
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

bool KPvpMatchManager::AddPvpMapForMatch_LUA( int iAddMapID )
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
	m_setPvpMapForMatch.insert( iAddMapID );
	return true;
}

bool KPvpMatchManager::AddPvpMapForNpcMatch_LUA( int iAddMapID )
{
	if( IsExistMapID( iAddMapID ) == false )
	{
		START_LOG( cerr, L"등록되지 않은 맵이 초보자용 맵으로 등록하려고 하네?" )
			<< BUILD_LOG( iAddMapID )
			<< END_LOG;
		return false;
	}

	m_kLotPvpMapForNpcMatch.AddCaseSameProb( iAddMapID );
	return true;
}

void KPvpMatchManager::AddPvpNpcInfo_LUA()
{
    KLuaManager luaManager( GetLuaState() );

	int iNpcID = 0;
	SPvpNpcInfo sPvpNpcInfo;
	LUA_GET_VALUE_RETURN(		luaManager, L"m_PvpNpcID",			iNpcID,									0,	goto end_proc );

	LUA_GET_VALUE_RETURN_ENUM(	luaManager, L"m_PvpNpcType",		sPvpNpcInfo.m_ePvpNpcType,	SEnum::PVP_NPC_TYPE, 	SEnum::PNT_NONE, goto end_proc );

	int iUnitClass = 0;
	LUA_GET_VALUE_RETURN(		luaManager, L"m_UnitClass",			iUnitClass,								0,	goto end_proc );
	sPvpNpcInfo.m_cUnitClass = static_cast<char>(iUnitClass);

	int iLevel = 0;
	LUA_GET_VALUE(				luaManager, L"m_Level",				iLevel,									0 );
	sPvpNpcInfo.m_ucLevel = static_cast<u_char>(iLevel);

	LUA_GET_VALUE(				luaManager, L"m_CharName",			sPvpNpcInfo.m_wstrCharName,				L"" );

	//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	int iRank;
	LUA_GET_VALUE_RETURN(		luaManager, L"m_Ranking",			iRank,					0,	goto end_proc );
	sPvpNpcInfo.m_cRank = static_cast<char>( iRank );
#endif SERV_2012_PVP_SEASON2
	//}}

	LUA_GET_VALUE_RETURN(		luaManager, L"m_Rating",			sPvpNpcInfo.m_Rating,					0,	goto end_proc );

	LUA_GET_VALUE(				luaManager, L"m_RewardTitleID",		sPvpNpcInfo.m_iRewardTitleID,			0 );
	LUA_GET_VALUE(				luaManager, L"m_RewardTitle",		sPvpNpcInfo.m_iRewardTitlePeriod,		0 );

	// 대전 NPC정보 저장
	m_mapPvpNpcInfo.insert( std::make_pair( iNpcID, sPvpNpcInfo ) );
	
	if( sPvpNpcInfo.m_ePvpNpcType < 0  ||  sPvpNpcInfo.m_ePvpNpcType >= SEnum::PNT_MAX )
		goto end_proc;

	// 랜덤 그룹에도 넣자!
	m_kLotPvpNpc[sPvpNpcInfo.m_ePvpNpcType].AddCaseSameProb( iNpcID );
	return;

end_proc:
	START_LOG( cerr, L"스크립트 파싱 실패!" )
		<< BUILD_LOG( iNpcID )
		<< BUILD_LOG( iUnitClass )
		<< BUILD_LOG( sPvpNpcInfo.m_Rating )
		<< BUILD_LOG( sPvpNpcInfo.m_ePvpNpcType )
		<< END_LOG;
}

void KPvpMatchManager::AddPvpNpcName_LUA( const char* pName )
{
	// 대전 NPC이름
	std::wstring wstrPvpNpcName = KncUtil::toWideString_UTF8( std::string( pName ) );

	// 대전 NPC이름 인덱스
	const int iIndex = static_cast<int>(m_mapPvpNpcName.size());

	// 데이터 저장!
	m_mapPvpNpcName.insert( std::make_pair( iIndex, wstrPvpNpcName ) );
	m_vecPvpNpcName.push_back( wstrPvpNpcName );
	m_kLotPvpNpcName.AddCaseSameProb( iIndex );
}

void KPvpMatchManager::SetHeroPvpNpcFactor_LUA( float fFactor )
{
	if( fFactor < 0.0 )
	{
		START_LOG( cerr, L"상수 정보가 이상합니다." )
			<< BUILD_LOG( fFactor )
			<< END_LOG;
		return;
	}

	m_fHeroPvpNpcFactor = fFactor;
}

bool KPvpMatchManager::GetPvpMatchInfo( IN const int iMatchType, OUT char& cWinKillNum, OUT float& fPlaytime ) const
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

bool KPvpMatchManager::DecisionPvpMap( IN const bool bIsNpcPvpMode, IN const std::map< short, int >& mapAllPlayersSelectedMap, OUT short& sMapID ) const
{
	sMapID = 0;
	std::map< short, int > mapSelectedMap = mapAllPlayersSelectedMap;
	KLotterySameProb< int > kPvpMapRandom;

	if( bIsNpcPvpMode )
	{
		//////////////////////////////////////////////////////////////////////////
		// 초보자 대전맵에 대한 처리

		// 대전맵을 결정하자!
		sMapID = m_kLotPvpMapForNpcMatch.DecisionSameProb();
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 일반 대전맵에 대한 처리

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
			int iMaxSelectedMapID = 0;
			int iMaxSelectedCount = 0;
			std::map< short, int >::const_iterator mit;
			for( mit = mapSelectedMap.begin(); mit != mapSelectedMap.end(); ++mit )
			{
				if( iMaxSelectedCount < mit->second )
				{
					iMaxSelectedCount = mit->second;
					iMaxSelectedMapID = static_cast<int>(mit->first);
				}
			}

			// 가장 많은 선택을 받은 맵을 제외한다!
			mapSelectedMap.erase( iMaxSelectedMapID );

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
		sMapID = static_cast< short >(kPvpMapRandom.DecisionSameProb());
	}	
	
	if( IsExistMapID( sMapID ) == false )
	{
		START_LOG( cerr, L"랜덤으로 결정된 대전맵이 등록된 대전맵이 아니네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( sMapID )
			<< END_LOG;

		sMapID = m_sDefaultPvpMapID;
	}	

	return true;
}

int	KPvpMatchManager::GetMatchWeightValue( IN const MATCH_WEIGHT_ENUM eEnum ) const
{
	if( eEnum < 0  ||  eEnum >= MWE_MAX )
	{
		START_LOG( cerr, L"대전 매치 가중치 enum이 이상합니다!" )
			<< BUILD_LOG( eEnum )
			<< END_LOG;
		return -1;
	}

	return m_arrMatchWeightValue[eEnum];
}

int KPvpMatchManager::GetRatingByTeam( IN const std::map< UidType, int >& mapTeamMemberInfo ) const
{
	if( mapTeamMemberInfo.empty()  ||  mapTeamMemberInfo.size() > SEnum::PE_MAX_MATCH_PLAYER_COUNT )
	{
		START_LOG( cerr, L"팀 레이팅을 계산하려는데 인원수가 이상하다!" )
			<< BUILD_LOG( mapTeamMemberInfo.size() )
			<< END_LOG;
		return -1;
	}

	KCompareLimitList< SUserRatingInfo > cllSortedUserList;

	std::map< UidType, int >::const_iterator mit;
	for( mit = mapTeamMemberInfo.begin(); mit != mapTeamMemberInfo.end(); ++mit )
	{
		const UidType iUnitUID = mit->first;
		const int iRating = mit->second;

		// rating값이 큰순으로 정렬해서 리스트를 만들자!
		cllSortedUserList.IfUpperValuePushBack( iRating, SUserRatingInfo( iUnitUID, iRating ) );
	}

	std::vector< SUserRatingInfo > vecSortedUserList;
	cllSortedUserList.GetValueList( vecSortedUserList );

	int iTotalRatingWithWeightValue = 0;
	int iTotalWeightValue = 0;

	for( u_int uiIndex = 0; uiIndex < vecSortedUserList.size(); ++uiIndex )
	{
		const SUserRatingInfo& sUserInfo = vecSortedUserList[uiIndex];

		// Rating순으로 정해진 가중치 값을 얻는다.
		const int iMatchWeightValue = GetMatchWeightValue( (KPvpMatchManager::MATCH_WEIGHT_ENUM)uiIndex );

		// 해당 유저의 Rating값과 가중치값을 곱한 합을 구한다.
		iTotalRatingWithWeightValue += ( sUserInfo.m_iRating * iMatchWeightValue );

		// 가중치값의 총합을 구한다.
		iTotalWeightValue += iMatchWeightValue;
	}

	// 가중치 총합이 0인 경우는 절대로 일어나서는 
	if( iTotalWeightValue <= 0 )
	{
		START_LOG( cerr, L"가중치 총합이 이상하다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( mapTeamMemberInfo.size() )
			<< BUILD_LOG( iTotalWeightValue )
			<< END_LOG;
		return -1;
	}

	const int iResultTeamRating = iTotalRatingWithWeightValue / iTotalWeightValue;
	return iResultTeamRating;
}

bool KPvpMatchManager::DecisionNpcPvpMode( IN const std::vector< KRoomUserInfo >& vecTeamMember ) const
{
	static int NPC_PVP_MODE = 1;
	std::map< UidType, int > mapTeamMemberRating;

	// 팀 멤버의 Rating을 정리하자!
	BOOST_TEST_FOREACH( const KRoomUserInfo&, kInfo, vecTeamMember )
	{
		mapTeamMemberRating.insert( std::make_pair( kInfo.m_nUnitUID, kInfo.m_iRating ) );
	}

	// 팀 레이팅을 구하자!
	const int TEAM_RATING = GetRatingByTeam( mapTeamMemberRating );
	float fNpcPvpModeRate = static_cast<float>(TEAM_RATING) * 0.001f * 0.01f * GetHeroPvpNpcFactor();
	if( fNpcPvpModeRate > 100.f )
	{
		fNpcPvpModeRate = 100.f;
	}

	//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	if( vecTeamMember.size() == 3 )
	{
		fNpcPvpModeRate *= 0.5f;
	}
#endif SERV_2012_PVP_SEASON2
	//}}

	// 랜덤 돌려서 결정하자!
	KLottery kLot;
	kLot.AddCase( NPC_PVP_MODE, fNpcPvpModeRate );

	const bool bRet = ( kLot.Decision() == NPC_PVP_MODE );
	return bRet;
}

bool KPvpMatchManager::IsExistPvpNpcName( IN const std::wstring& wstrNickName ) const
{
	BOOST_TEST_FOREACH( const std::wstring&, wstrCharName, m_vecPvpNpcName )
	{
        if( wstrCharName == wstrNickName )
			return true;
	}

	return false;
}

//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
bool KPvpMatchManager::GetPvpNpcInfo( IN const bool bIsAllPlayersBeginner, IN const int iMyTeamRatingWeightAverage, IN const u_int uiPvpPlayerCount, OUT std::vector< KRoomUserInfo >& vecPvpNpcTeam, OUT std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo ) const
{
	vecPvpNpcTeam.clear();
	mapPvpNpcInfo.clear();

	// PVP NPC UID값은 -10000부터 시작합니다.
	UidType iPvpNpcUnitUID = -10000;

	std::multimap<int, int> mapNpcListOrderedByRatingGab;
	KLotterySameProb< int > kLotPvpNpc;
	if( bIsAllPlayersBeginner )
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_BEGINNER_NPC];
	}
	else
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_HERO_NPC];
	}

	BOOST_TEST_FOREACH( int, iPvpNpcID, kLotPvpNpc.m_vecCase )
	{
		// 실제 존재하는 녀석인지 체크!
		const SPvpNpcInfo* pPvpNpcInfo = GetPvpNpcInfo( iPvpNpcID );
		if( pPvpNpcInfo == NULL )
		{
			START_LOG( cerr, L"대전 NPC정보가 없습니다!" )
				<< BUILD_LOG( bIsAllPlayersBeginner )
				<< BUILD_LOG( iPvpNpcID )
				<< END_LOG;
			return false;
		}

		// Rating Gab을 구해서 npc 리스트 정리한다.
		mapNpcListOrderedByRatingGab.insert( std::multimap<int, int>::value_type( abs( iMyTeamRatingWeightAverage - pPvpNpcInfo->m_Rating ), iPvpNpcID ) );
	}

	// kLotPvpNpc에 다시 담는다.
	kLotPvpNpc.Clear();
	if( mapNpcListOrderedByRatingGab.empty() != true )
	{
		LIF( mapNpcListOrderedByRatingGab.size() < 100 ); 

		std::multimap<int, int>::iterator it;
		int iMultiple = 32;
		for( it = mapNpcListOrderedByRatingGab.begin(); it != mapNpcListOrderedByRatingGab.end(); ++it )
		{
			int iMultipleCopy = iMultiple;
			// 32부터 시작해서 
			while( 0 <= --iMultipleCopy )
			{
				kLotPvpNpc.AddCaseSameProb( it->second );
			}

			// 2분의 1씩 차감한다.
			if( 1 < iMultiple )
			{
				iMultiple >>= 1;
			}
		}
	}

	for( u_int ui = 0; ui < uiPvpPlayerCount; ++ui )
	{
		// 랜덤하게 등장할 NPC를 정하자!
		const int iPvpNpcID = kLotPvpNpc.DecisionSameProb();

		// 실제 존재하는 녀석인지 체크!
		const SPvpNpcInfo* pPvpNpcInfo = GetPvpNpcInfo( iPvpNpcID );
		if( pPvpNpcInfo == NULL )
		{
			START_LOG( cerr, L"대전 NPC정보가 없습니다!" )
				<< BUILD_LOG( bIsAllPlayersBeginner )
				<< BUILD_LOG( iPvpNpcID )
				<< END_LOG;
			return false;
		}

		//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		// 다음에 나오는 NPC는 중복되지 않도록 삭제해둔다.
		kLotPvpNpc.EraseAll( iPvpNpcID );
#else
		// 다음에 나오는 NPC는 중복되지 않도록 삭제해둔다.
		kLotPvpNpc.Erase( iPvpNpcID );
#endif SERV_2012_PVP_SEASON2
		//}}

		// RoomUserInfo를 만들어서 저장하자!
		KRoomUserInfo kInfo;
		kInfo.m_bIsPvpNpc			= true;
		kInfo.m_iGSUID				= -1;
		kInfo.m_iOwnerUserUID		= -1;
		kInfo.m_nUnitUID			= iPvpNpcUnitUID;
		kInfo.m_cUnitClass			= pPvpNpcInfo->m_cUnitClass;
		if( pPvpNpcInfo->m_wstrCharName.empty() )
		{
			GetPvpNpcNameByRandom( kInfo.m_wstrNickName );
		}
		else
		{
			kInfo.m_wstrNickName = pPvpNpcInfo->m_wstrCharName;
		}
		kInfo.m_ucLevel				= pPvpNpcInfo->m_ucLevel;
		kInfo.m_kGameStat.m_iBaseHP = 30000; // RoomUserManager에서 Enter할때 오류를 안내기 위해 기본HP값을 30000을 넣어둔다.
		//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		kInfo.m_cRank				= pPvpNpcInfo->m_cRank;
#endif SERV_2012_PVP_SEASON2
		//}}
		kInfo.m_iRating				= pPvpNpcInfo->m_Rating;
		kInfo.m_iMaxRating			= kInfo.m_iRating;
		vecPvpNpcTeam.push_back( kInfo );

		// 대전 NPC정보도 저장하자!
		KPvpNpcInfo kNpcInfo;
		kNpcInfo.m_iNpcID = iPvpNpcID;
		mapPvpNpcInfo.insert( std::make_pair( kInfo.m_nUnitUID, kNpcInfo ) );

		// 다음 NpcUID값 발급!
		--iPvpNpcUnitUID;
	}

	return true;
}
#else
bool KPvpMatchManager::GetPvpNpcInfo( IN const bool bIsAllPlayersBeginner, IN const u_int uiPvpPlayerCount, OUT std::vector< KRoomUserInfo >& vecPvpNpcTeam, OUT std::map< UidType, KPvpNpcInfo >& mapPvpNpcInfo ) const
{
	vecPvpNpcTeam.clear();
	mapPvpNpcInfo.clear();

	// PVP NPC UID값은 -10000부터 시작합니다.
	UidType iPvpNpcUnitUID = -10000;

#ifdef SERV_NOT_2012_PVP_SEASON2_BUILD_ERROR_FIX
	KLotterySameProb< int > kLotPvpNpc;
#else // SERV_NOT_2012_PVP_SEASON2_BUILD_ERROR_FIX
	KLotterySameProb kLotPvpNpc;
#endif // SERV_NOT_2012_PVP_SEASON2_BUILD_ERROR_FIX
	if( bIsAllPlayersBeginner )
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_BEGINNER_NPC];
	}
	else
	{
		kLotPvpNpc = m_kLotPvpNpc[SEnum::PNT_HERO_NPC];
	}

	for( u_int ui = 0; ui < uiPvpPlayerCount; ++ui )
	{
		// 랜덤하게 등장할 NPC를 정하자!
		const int iPvpNpcID = kLotPvpNpc.DecisionSameProb();

		// 실제 존재하는 녀석인지 체크!
		const SPvpNpcInfo* pPvpNpcInfo = GetPvpNpcInfo( iPvpNpcID );
		if( pPvpNpcInfo == NULL )
		{
			START_LOG( cerr, L"대전 NPC정보가 없습니다!" )
				<< BUILD_LOG( bIsAllPlayersBeginner )
				<< BUILD_LOG( iPvpNpcID )
				<< END_LOG;
			return false;
		}

		// 다음에 나오는 NPC는 중복되지 않도록 삭제해둔다.
		kLotPvpNpc.Erase( iPvpNpcID );

		// RoomUserInfo를 만들어서 저장하자!
		KRoomUserInfo kInfo;
		kInfo.m_bIsPvpNpc			= true;
		kInfo.m_iGSUID				= -1;
		kInfo.m_iOwnerUserUID		= -1;
		kInfo.m_nUnitUID			= iPvpNpcUnitUID;
		kInfo.m_cUnitClass			= pPvpNpcInfo->m_cUnitClass;
		if( pPvpNpcInfo->m_wstrCharName.empty() )
		{
			GetPvpNpcNameByRandom( kInfo.m_wstrNickName );
		}
		else
		{
            kInfo.m_wstrNickName = pPvpNpcInfo->m_wstrCharName;
		}
		kInfo.m_ucLevel				= pPvpNpcInfo->m_ucLevel;
		kInfo.m_kGameStat.m_iBaseHP = 30000; // RoomUserManager에서 Enter할때 오류를 안내기 위해 기본HP값을 30000을 넣어둔다.
		kInfo.m_iRating				= pPvpNpcInfo->m_Rating;
		kInfo.m_iMaxRating			= kInfo.m_iRating;
		vecPvpNpcTeam.push_back( kInfo );

		// 대전 NPC정보도 저장하자!
		KPvpNpcInfo kNpcInfo;
		kNpcInfo.m_iNpcID = iPvpNpcID;
		mapPvpNpcInfo.insert( std::make_pair( kInfo.m_nUnitUID, kNpcInfo ) );

		// 다음 NpcUID값 발급!
		--iPvpNpcUnitUID;
	}

	return true;
}
#endif SERV_2012_PVP_SEASON2
//}}

const KPvpMatchManager::SPvpNpcInfo* KPvpMatchManager::GetPvpNpcInfo( IN const int iNpcID ) const
{
	std::map< int, SPvpNpcInfo >::const_iterator mit;
	mit = m_mapPvpNpcInfo.find( iNpcID );
	if( mit == m_mapPvpNpcInfo.end() )
	{
		START_LOG( cerr, L"존재 하지 않는 대전 NPC 정보 입니다!" )
			<< BUILD_LOG( iNpcID )
			<< END_LOG;

		return NULL;
	}

	return &mit->second;
}

void KPvpMatchManager::GetPvpNpcNameByRandom( OUT std::wstring& wstrName ) const
{
    const int iNameIndex = m_kLotPvpNpcName.DecisionSameProb();
	std::map< int, std::wstring >::const_iterator mit;
	mit = m_mapPvpNpcName.find( iNameIndex );
	if( mit == m_mapPvpNpcName.end() )
	{
		START_LOG( cerr, L"존재하지 않는 index네? 스크립트 파싱이 잘못되었나?" )
			<< BUILD_LOG( iNameIndex )
			<< END_LOG;
		return;
	}

	wstrName = mit->second;
}
#endif SERV_PVP_NEW_SYSTEM
//}}
