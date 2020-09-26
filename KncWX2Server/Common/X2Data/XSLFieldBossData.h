#pragma once

#include "ServerDefine.h"

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-14	// 박세훈
#include "RefreshSingleton.h"
#include <set>

class CXSLFieldBossData
{
	DeclareRefreshSingleton( CXSLFieldBossData );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

	typedef	std::map< float, std::map< int, byte > >	TYPE_REWARD_DATA;

public:
	void	SetData_LUA( void );

	bool	DoesFieldAffectTotalDangerousValue( IN int iVillageMapID ) const	{	return m_setPortalLocation.find( iVillageMapID ) != m_setPortalLocation.end();	}
	bool	GetRandomPortalLocationID( OUT int& iPortalLocationID ) const;
	
	byte	GetTotalDangerousValue( void ) const	{	return m_byteTotalDangerousValue;	}
	int		GetFieldBossCoolTime_M( void ) const	{	return m_iFieldBossCoolTime_M;	}
	int		GetFieldBossPortalHoldingTime_M( void ) const	{	return m_iPortalHoldingTime_M;	}
	int		GetFieldBossFieldHoldingTime_M( void ) const	{	return m_iFieldHoldingTime_M;	}
	byte	GetFieldBossEndGameFieldHoldingTime_S( void ) const	{	return m_byteEndGameFieldHoldingTime_S;	}
	USHORT	GetFieldBossEndGameFieldHoldingTimeForce_S( void ) const	{	return ( m_byteEndGameFieldHoldingTime_S + m_byteEndGameFieldHoldingTimeForce_S );	}
	byte	GetIntrudeRestrictionTime_S( void ) const	{	return m_byteIntrudeRestrictionTime_S;	}
	byte	GetMaximumExpandedSocketCount( void ) const	{	return m_byteMaximumExpandedSocketCount;	}
	float	GetLimiteContribution( void ) const	{	return m_fLimiteContribution;	}
	float	GetMaximumContribution( void ) const	{	return m_fMaximumContribution;	}

	void	GetPrecedenceRewardData( IN const size_t iRank, IN const size_t iWholeSize, OUT std::map<int, byte>& mapRewardData ) const;

	bool	IsSocketExpandItemForWeapon( IN const int iItemID ) const	{	return m_setSocketExpandItemForWeapon.find( iItemID ) != m_setSocketExpandItemForWeapon.end();	}
	bool	IsSocketExpandItemForDefence( IN const int iItemID ) const	{	return m_setSocketExpandItemForDefence.find( iItemID ) != m_setSocketExpandItemForDefence.end();	}

private:
	CXSLFieldBossData( void );
	~CXSLFieldBossData( void );

	template<typename T1, typename T2>	bool	_SetData( OUT T1& typeValue, IN const T2 typeMinValue, IN const T2 typeSetValue, IN const T2 typeMaxValue ) const;
	void	_CheckData( void ) const;
	bool	_PrecedenceAndRewardDataParsing( IN KLuaManager& luaManager );
	bool	_RewardParsing( IN KLuaManager& luaManager, OUT std::map<int, byte>& mapReward ) const;

private:
	std::set<int>		m_setSocketExpandItemForWeapon;
	std::set<int>		m_setSocketExpandItemForDefence;
	std::set<int>		m_setPortalLocation;		// 보스 필드로 이동하기 위한 포탈 생성 위치 리스트 ( VillageMapID )
	TYPE_REWARD_DATA	m_mapRewardData;
	
	int		m_iFieldBossCoolTime_M;					// 필드 보스 재 출현 제한 시간
	int		m_iPortalHoldingTime_M;					// 보스 필드로 이동하기 위한 포탈의 유지 시간
	int		m_iFieldHoldingTime_M;					// 보스 필드 방의 생성 후 유지 시간

	float	m_fLimiteContribution;
	float	m_fMaximumContribution;
	
	byte	m_byteTotalDangerousValue;
	byte	m_byteEndGameFieldHoldingTime_S;		// 보스 게임 종료 후 보스 필드 유지 시간
	byte	m_byteEndGameFieldHoldingTimeForce_S;	// 보스 필드 유지 시간 이후 강제 종료까지의 유예 시간
	byte	m_byteIntrudeRestrictionTime_S;			// 남은 시간이 이 시간 미만일 경우 난입 불가가 된다.

	byte	m_byteMaximumExpandedSocketCount;		// 최대 확장 가능한 소켓 수
};

template<typename T1, typename T2>
bool CXSLFieldBossData::_SetData( OUT T1& typeValue, IN const T2 typeMinValue, IN const T2 typeSetValue, IN const T2 typeMaxValue ) const
{
	if( ( typeSetValue < typeMinValue )
		|| ( typeMaxValue < typeSetValue )
		)
	{
		START_LOG( cerr, L"설정할 수 있는 범위의 값이 아닙니다.")
			<< BUILD_LOG( typeMinValue )
			<< BUILD_LOG( typeSetValue )
			<< BUILD_LOG( typeMaxValue )
			<< END_LOG;
		return false;
	}

	typeValue = static_cast<T1>( typeSetValue );

	return true;
}

DefRefreshSingletonInline( CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS
