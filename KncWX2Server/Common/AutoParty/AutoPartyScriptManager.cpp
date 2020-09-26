#include ".\AutoPartyScriptManager.h"
#include "NetError.h"
#include "BaseServer.h"
#include "Lottery.h"

//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "Enum/Enum.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}


ImplementRefreshSingleton( KAutoPartyScriptManager );

KAutoPartyScriptManager::KAutoPartyScriptManager(void) :
m_fSameUnitTypeWaitTime( SAME_UNIT_TYPE_WAIT_TIME ),
m_fDirectGameStartWaitTime( DIRECT_GAME_START_WAIT_TIME ),
//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
m_fSameDifficultyLevelWaitTime( SAME_DIFFICULTY_LEVEL_WAIT_TIME ),
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}
m_fAutoPartyBonusTime( AUTO_PARTY_BONUS_TIME_DEFALUT ),
m_iAutoPartyMakingBeginCount( 1 ),
//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
m_bIsAllMemberCheckSameUnitTypeTime( true ),
m_bIsAllMemberCheckSameDifficultyLevelTime( true ),
m_bIsAllMemberCheckDirectGameStartTime( true ),
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}
m_iCheckWaitUserCount( 10 )
{
}

KAutoPartyScriptManager::~KAutoPartyScriptManager(void)
{
}

ImplToStringW( KAutoPartyScriptManager )
{
	stm_	<< L"----------[ Auto Party Script Manager ]----------" << std::endl
			<< TOSTRINGW( m_vecAutoPartyDungeonInfo.size() )
		;

	return stm_;
}

ImplementLuaScriptParser( KAutoPartyScriptManager )
{
	lua_tinker::class_add<KAutoPartyScriptManager>( GetLuaState(), "KAutoPartyScriptManager" );	
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "AddAutoPartyDungeonInfo",		&KAutoPartyScriptManager::AddAutoPartyDungeonInfo_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetSameUnitTypeWaitTime",		&KAutoPartyScriptManager::SetSameUnitTypeWaitTime_LUA );
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetSameDifficultyLevelWaitTime",&KAutoPartyScriptManager::SetSameDifficultyLevelWaitTime_LUA );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetDirectGameStartWaitTime",	&KAutoPartyScriptManager::SetDirectGameStartWaitTime_LUA );	
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetAutoPartyBonusTime",			&KAutoPartyScriptManager::SetAutoPartyBonusTime_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetAutoPartyMakingBeginCount",	&KAutoPartyScriptManager::SetAutoPartyMakingBeginCount_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetCheckWaitUserCount",			&KAutoPartyScriptManager::SetCheckWaitUserCount_LUA );
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetIsAllMemberCheckSameUnitTypeTime",			&KAutoPartyScriptManager::SetIsAllMemberCheckSameUnitTypeTime_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetIsAllMemberCheckSameDifficultyLevelTime",	&KAutoPartyScriptManager::SetIsAllMemberCheckSameDifficultyLevelTime_LUA );
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetIsAllMemberCheckDirectGameStartTime",		&KAutoPartyScriptManager::SetIsAllMemberCheckDirectGameStartTime_LUA );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "SetAutoPartyMatchingSuccessNum",				&KAutoPartyScriptManager::SetAutoPartyMatchingSuccessNum_LUA );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

	lua_tinker::class_def<KAutoPartyScriptManager>( GetLuaState(), "dump",							&KAutoPartyScriptManager::Dump );

	lua_tinker::decl( GetLuaState(), "AutoPartyScriptManager", this );
}

bool KAutoPartyScriptManager::AddAutoPartyDungeonInfo_LUA( int iDungeonMode, int iDungeonIDWithDif, int iBeginLevel, int iEndLevel )
{
	if( iDungeonMode < 0  ||  iDungeonIDWithDif < 0  ||  iBeginLevel < 0  ||  iEndLevel < 0 )
	{
		START_LOG( cerr, L"매치 정보가 이상합니다." )
			<< BUILD_LOG( iDungeonMode )
			<< BUILD_LOG( iDungeonIDWithDif )
			<< BUILD_LOG( iBeginLevel )
			<< BUILD_LOG( iEndLevel )
			<< END_LOG;
		return false;
	}

	KAutoPartyMakingProcessCondition kAutoPartyDungeonInfo;
	kAutoPartyDungeonInfo.m_iDungeonMode = iDungeonMode;
	kAutoPartyDungeonInfo.m_iDungeonIDWithDif = iDungeonIDWithDif;
	kAutoPartyDungeonInfo.m_iBeginLevel = iBeginLevel;
	kAutoPartyDungeonInfo.m_iEndLevel = iEndLevel;
	m_vecAutoPartyDungeonInfo.push_back( kAutoPartyDungeonInfo );
	return true;
}

void KAutoPartyScriptManager::SetSameUnitTypeWaitTime_LUA( double fWaitTime )
{
	m_fSameUnitTypeWaitTime = fWaitTime;

	START_LOG( cout, L"[자동 파티 설정] 같은 종류 캐릭터 매칭 대기 시간 : " << m_fSameUnitTypeWaitTime );
}

void KAutoPartyScriptManager::SetDirectGameStartWaitTime_LUA( double fWaitTime )
{
	m_fDirectGameStartWaitTime = fWaitTime;

	START_LOG( cout, L"[자동 파티 설정] 바로 게임 시작하는 매칭 대기 시간 : " << m_fDirectGameStartWaitTime );
}

//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
void KAutoPartyScriptManager::SetSameDifficultyLevelWaitTime_LUA( double fWaitTime )
{
	m_fSameDifficultyLevelWaitTime = fWaitTime;

	START_LOG( cout, L"[자동 파티 설정] 같은 난이도 매칭 대기 시간 : " << m_fSameDifficultyLevelWaitTime );
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

void KAutoPartyScriptManager::SetAutoPartyBonusTime_LUA( double fBonusTime )
{
	m_fAutoPartyBonusTime = fBonusTime;

	START_LOG( cout, L"[자동 파티 설정] 자동 파티 보너스 적용 시간!" )
		<< BUILD_LOG( GetAutoPartyBonusTime() );
}

void KAutoPartyScriptManager::SetAutoPartyMakingBeginCount_LUA( int iAutoPartyMakingBeginCount )
{
	m_iAutoPartyMakingBeginCount = iAutoPartyMakingBeginCount;

	START_LOG( cout, L"[자동 파티 설정] 자동 파티 매칭 시작 대기자 인원 수" )
		<< BUILD_LOG( GetAutoPartyMakingBeginCount() );
}

void KAutoPartyScriptManager::SetCheckWaitUserCount_LUA( int iCheckWaitUserCount )
{
	m_iCheckWaitUserCount = iCheckWaitUserCount;

	START_LOG( cout, L"[자동 파티 설정] 자동 파티 대기자 리스트 세팅 카운트" )
		<< BUILD_LOG( GetCheckWaitUserCount() );
}

//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
void KAutoPartyScriptManager::SetAutoPartyMatchingSuccessNum_LUA( IN int iDungeon, IN int iNum )
{
	if( ( iDungeon < 0 ) || ( iNum < 1 ) )
	{
		START_LOG( cerr, L"잘못된 인자값입니다.")
			<< BUILD_LOG( iDungeon )
			<< BUILD_LOG( iNum )
			<< END_LOG;
	}

	std::pair< std::map<int, int>::iterator, bool > pairResult = m_mapAutoPartyMatchingSuccessNum.insert( std::map<int, int>::value_type( iDungeon, iNum ) );
	if( pairResult.second == false )
	{
		START_LOG( cerr, L"이미 정의되어 있는 iDungeon 입니다.")
			<< BUILD_LOG( iDungeon )
			<< END_LOG;
	}
}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}

//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
void KAutoPartyScriptManager::SetIsAllMemberCheckSameUnitTypeTime_LUA( bool bVal )
{
	m_bIsAllMemberCheckSameUnitTypeTime = bVal;

	START_LOG( cout, L"[자동 파티 설정] 자동 파티 매칭시 같은 유닛인지 체크 여부" )
		<< BUILD_LOG( IsCheckAllMemberSameUnitTypeTime() );
}

void KAutoPartyScriptManager::SetIsAllMemberCheckSameDifficultyLevelTime_LUA( bool bVal )
{
	m_bIsAllMemberCheckSameDifficultyLevelTime = bVal;

	START_LOG( cout, L"[자동 파티 설정] 자동 파티 매칭시 다른 난이도 인지 체크 여부" )
		<< BUILD_LOG( IsCheckAllMemberSameDifficultyLevelTime() );
}

void KAutoPartyScriptManager::SetIsAllMemberCheckDirectGameStartTime_LUA( bool bVal )
{
	m_bIsAllMemberCheckDirectGameStartTime = bVal;

	START_LOG( cout, L"[자동 파티 설정] 자동 파티 매칭시 인원 수 부족해도 바로 시작 여부" )
		<< BUILD_LOG( IsCheckAllMemberDirectGameStartTime() );
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
int KAutoPartyScriptManager::GetAutoPartyMatchingSuccessNum( IN const int iDungeon ) const
{
	std::map<int, int>::const_iterator it = m_mapAutoPartyMatchingSuccessNum.find( iDungeon );
	if( it == m_mapAutoPartyMatchingSuccessNum.end() )
	{
		return SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	}

	return it->second;
}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
