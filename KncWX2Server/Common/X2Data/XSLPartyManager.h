#pragma once


#include "RefreshSingleton.h"
#include "XSLRoom.h"


#define PARTY_INVITE_TIME_LIMIT		7.0


class CXSLPartyManager
{
	DeclareRefreshSingleton( CXSLPartyManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
public:
	enum PARTY_ENUM
	{
		PE_MAX_PARTY_NUM = 4,

		PE_PARTY_COUNT_TYPE = CXSLRoom::RT_TOTAL_NUM,
	};

	struct SIncreasePartyFeverRange
	{
		int		m_iMin;
		int		m_iMax;
		int		m_iMinMaxGap;

		SIncreasePartyFeverRange()
		{
			m_iMin = 0;
			m_iMax = 0;
			m_iMinMaxGap = 0;
		}
	};

	//{{ 2012. 11. 16	최육사		헤니르 시공 비밀 스테이지 덤프
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	struct SHenirSecretStageResultDump
	{
		int		m_iFeverPoint;
		int		m_iNumMember;
		float	m_fEnterSecretRate;
		bool	m_bEnterSuccess;

		SHenirSecretStageResultDump()
		{
			m_iFeverPoint = 0;
			m_iNumMember = 0;
			m_fEnterSecretRate = 0.f;
			m_bEnterSuccess = false;	
		}
	};
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	//}}

public:
	CXSLPartyManager(void);
	~CXSLPartyManager(void);

	// for lua
	void	SetActivateFeverPoint_LUA( int iFever );
	bool	SetMaxFeverPointByNumMember_LUA( int iNumMember, int iMaxFever );
	bool	SetIncreasePartyFeverByNumMember_LUA( int iNumMember, int iMin, int iMax );
	//{{ 2012. 11. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetDungeonSecretStageEnterFactor_LUA( float fBasePercent, float fPartyFeverFactor );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	// function
	int		GetActivateFeverPoint() const				{ return m_iActivateFeverPoint; }
	short	GetMaxFeverPointByNumMember( IN const short sPartyFever, IN const int iPartyNumMember );
	int		GetIncreasePartyFever( IN const int iNumMember );
	float	GetDungeonSecretStageEnterRate( IN const int iPartyFeverPoint, IN const int iPartyNumMember );

	//{{ 2012. 11. 16	최육사		헤니르 시공 비밀 스테이지 덤프
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	void	SetHenirSecretStageEnterRateDump( IN const int iFeverPoint, IN const int iNumMember, IN const float fRate, IN const bool bEnterSuccess );
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	//}}

private:
	int											m_iActivateFeverPoint;
	std::map< int, int >						m_mapMaxFeverPoint;
	std::map< int, SIncreasePartyFeverRange >	m_mapIncreasePartyFever;
	//{{ 2012. 11. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	float										m_fBasePercent;
	float										m_fPartyFeverFactor;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 11. 16	최육사		헤니르 시공 비밀 스테이지 덤프
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	SHenirSecretStageResultDump					m_kHenirSecretEnterMinRate;
	SHenirSecretStageResultDump					m_kHenirSecretEnterMaxRate;
	int											m_iEnterSuccessCount;
	int											m_iEnterFailCount;
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	//}}
};

DefRefreshSingletonInline( CXSLPartyManager );



