#pragma once

#include "RefreshSingleton.h"
#include <vector>
#include "CommonPacket.h"

//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include <map>
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}


#define SAME_UNIT_TYPE_WAIT_TIME		30.0
#define DIRECT_GAME_START_WAIT_TIME		300.0
//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
#define SAME_DIFFICULTY_LEVEL_WAIT_TIME	120.0
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}
#define AUTO_PARTY_BONUS_TIME_DEFALUT	120


class KAutoPartyScriptManager
{
	DeclareRefreshSingleton( KAutoPartyScriptManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum EVENT_RESULT
	{
		ER_NONE = 0,
		ER_ACTIVATED,
		ER_DEACTIVATED,
	};

public:
	KAutoPartyScriptManager(void);
	~KAutoPartyScriptManager(void);

	// for lua
	bool	AddAutoPartyDungeonInfo_LUA( int iDungeonMode, int iDungeonIDWithDif, int iBeginLevel, int iEndLevel );
	void	SetSameUnitTypeWaitTime_LUA( double fWaitTime );
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	void	SetSameDifficultyLevelWaitTime_LUA( double fWaitTime );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	void	SetDirectGameStartWaitTime_LUA( double fWaitTime );	
	void	SetAutoPartyBonusTime_LUA( double fBonusTime );
	void	SetAutoPartyMakingBeginCount_LUA( int iAutoPartyMakingBeginCount );
	void	SetCheckWaitUserCount_LUA( int iCheckWaitUserCount );
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	void	SetIsAllMemberCheckSameUnitTypeTime_LUA( bool bVal );
	void	SetIsAllMemberCheckSameDifficultyLevelTime_LUA( bool bVal );
	void	SetIsAllMemberCheckDirectGameStartTime_LUA( bool bVal );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	void	SetAutoPartyMatchingSuccessNum_LUA( IN int iDungeon, IN int iNum );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

	// function
	const std::vector< KAutoPartyMakingProcessCondition >& GetAutoPartyDungeonInfo() const	{ return m_vecAutoPartyDungeonInfo; }	
	double	GetSameUnitTypeWaitTime() const													{ return m_fSameUnitTypeWaitTime; }
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	double	GetSameDifficultyLevelWaitTime() const											{ return m_fSameDifficultyLevelWaitTime; }
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	double	GetDirectGameStartWaitTime() const												{ return m_fDirectGameStartWaitTime; }	
	double	GetAutoPartyBonusTime() const													{ return m_fAutoPartyBonusTime; }
	int		GetAutoPartyMakingBeginCount() const											{ return m_iAutoPartyMakingBeginCount; }
	int		GetCheckWaitUserCount() const													{ return m_iCheckWaitUserCount; }

	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	bool	IsCheckAllMemberSameUnitTypeTime() const										{ return m_bIsAllMemberCheckSameUnitTypeTime; }
	bool	IsCheckAllMemberSameDifficultyLevelTime() const									{ return m_bIsAllMemberCheckSameDifficultyLevelTime; }
	bool	IsCheckAllMemberDirectGameStartTime() const										{ return m_bIsAllMemberCheckDirectGameStartTime; }
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	int		GetAutoPartyMatchingSuccessNum( IN const int iDungeon ) const;
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

private:
	std::vector< KAutoPartyMakingProcessCondition >		m_vecAutoPartyDungeonInfo;
	double												m_fSameUnitTypeWaitTime;
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	double												m_fSameDifficultyLevelWaitTime;
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	double												m_fDirectGameStartWaitTime;	
	int													m_iAutoPartyMakingBeginCount;
	int													m_iCheckWaitUserCount;

	// 자동 파티 보너스
	double												m_fAutoPartyBonusTime;

	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	// 자동 파티 조건 체크 on/off
	bool												m_bIsAllMemberCheckSameUnitTypeTime;
	bool												m_bIsAllMemberCheckSameDifficultyLevelTime;
	bool												m_bIsAllMemberCheckDirectGameStartTime;
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	//{{ 2012. 11. 29	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	std::map<int, int>									m_mapAutoPartyMatchingSuccessNum;
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}
};

DefRefreshSingletonInline( KAutoPartyScriptManager );


