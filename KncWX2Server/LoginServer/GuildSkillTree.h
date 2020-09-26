#pragma once

#include "x2data/XSLSkillTree.h"


#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27

class KGuildSkillTree
{
public:
	struct GuildSkill
	{
		int m_iSkillLevel;
		int m_iSkillCSPoint;

		GuildSkill()
		{
			m_iSkillLevel = 0;
			m_iSkillCSPoint = 0;
		}

		GuildSkill( int iLevel, int iSkillCSPoint )
		{
			m_iSkillLevel	= iLevel;
			m_iSkillCSPoint = iSkillCSPoint;
		}
	};

public:
	KGuildSkillTree(void);
	~KGuildSkillTree(void);

	void UpdateGuildSkillTree( IN const KGuildSkillInfo& kInfo );
	void ResetGuildSkillTree();
	void GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo ) const;

	int	 GetSkillLevel( int iSkillID ) const;

	int  GetGuildSPoint() const			{ return m_iGuildSPoint; }
	void SetGuildSPoint( int iVal ) 	{ m_iGuildSPoint = iVal; }
	int	 GetGuildCSPoint() const		{ return m_iGuildCSPoint; }
	void SetGuildCSPoint( int iVal )	{ m_iGuildCSPoint = iVal; }
	int	 GetMaxGuildCSPoint() const		{ return m_iMaxGuildCSPoint; }
	void SetMaxGuildCSPoint( int iVal ) { m_iMaxGuildCSPoint = iVal; }
	CTime GetGuildCSPointEndDate() const { return m_tGuildCSPointEndDate; }

	bool SetGuildCSPointEndDate( IN const std::wstring& wstrEndDate );
	bool SetSkillLevelAndCSP( int iSkillID, int iSkillLevel, int iSkillCSPoint );				// 스킬이 존재하면 스킬레벨을 지정된 레벨로, 없으면 추가하고 스킬레벨을 지정된 레벨로

	bool IsMasterSkillLevel( IN int iSkillID );													// 스킬을 최고레벨까지 찍었는지
	bool IsAllPrecedingSkillMasterLevel( int iSkillID );											// 선행스킬을 모두 최고레벨까지 배웠는지
	bool IsAllFollowingSkillLevelZero( int iSkillID );											// leaf node인지
	bool IsTierOpened( int iSkillID );
	void CalcCumulativeUsedSPointOnEachTier( OUT std::vector< int >& vecTierSPoint );

	bool IsCashSkillPointExpired();
	void CalcExpireCashSkillPoint( OUT int& iGuildSPoint, OUT int& iRetrievedSPoint, OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData );
	void ExpireCashSkillPoint( OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData );

	bool GetSkillLevelAndCSP( IN int iSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint );	// 획득한 스킬이 존재하는지 체크, 있다면 스킬레벨을 return, 없으면 -1 return

	void CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint );

private:
	typedef std::map< int, GuildSkill >	GuildSkillDataMap;
	GuildSkillDataMap					m_mapSkillTree;

	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
	int									m_iMaxGuildCSPoint;
	std::wstring						m_wstrGuildCSPointEndDate;
	CTime								m_tGuildCSPointEndDate;
};

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
#ifdef GUILD_SKILL_TEST

class KGuildSkillTree
{
public:
struct GuildSkill
{
int m_iSkillLevel;
int m_iSkillCSPoint;

GuildSkill()
{
m_iSkillLevel = 0;
m_iSkillCSPoint = 0;
}

GuildSkill( int iLevel, int iSkillCSPoint )
{
m_iSkillLevel	= iLevel;
m_iSkillCSPoint = iSkillCSPoint;
}
};

public:
KGuildSkillTree(void);
~KGuildSkillTree(void);

void UpdateGuildSkillTree( IN const KGuildSkillInfo& kInfo );
void ResetGuildSkillTree();
void GetGuildSkillInfo( OUT KGuildSkillInfo& kInfo ) const;

int	 GetSkillLevel( int iSkillID ) const;

int  GetGuildSPoint() const			{ return m_iGuildSPoint; }
void SetGuildSPoint( int iVal ) 	{ m_iGuildSPoint = iVal; }
int	 GetGuildCSPoint() const		{ return m_iGuildCSPoint; }
void SetGuildCSPoint( int iVal )	{ m_iGuildCSPoint = iVal; }
int	 GetMaxGuildCSPoint() const		{ return m_iMaxGuildCSPoint; }
void SetMaxGuildCSPoint( int iVal ) { m_iMaxGuildCSPoint = iVal; }
CTime GetGuildCSPointEndDate() const { return m_tGuildCSPointEndDate; }

bool SetGuildCSPointEndDate( IN const std::wstring& wstrEndDate );
bool SetSkillLevelAndCSP( int iSkillID, int iSkillLevel, int iSkillCSPoint );				// 스킬이 존재하면 스킬레벨을 지정된 레벨로, 없으면 추가하고 스킬레벨을 지정된 레벨로

bool IsMaxSkillLevel( IN int iSkillID );													// 스킬을 최고레벨까지 찍었는지
bool IsAllPrecedingSkillMaxLevel( int iSkillID );											// 선행스킬을 모두 최고레벨까지 배웠는지
bool IsAllFollowingSkillLevelZero( int iSkillID );											// leaf node인지
bool IsTierOpened( int iSkillID );
void CalcCumulativeUsedSPointOnEachTier( OUT std::vector< int >& vecTierSPoint );

bool IsCashSkillPointExpired();
void CalcExpireCashSkillPoint( OUT int& iGuildSPoint, OUT int& iRetrievedSPoint, OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData );
void ExpireCashSkillPoint( OUT std::vector< KGuildSkillData >& vecModifiedGuildSkillData );

bool GetSkillLevelAndCSP( IN int iSkillID, OUT int& iSkillLevel, OUT int& iSkillCSPoint );	// 획득한 스킬이 존재하는지 체크, 있다면 스킬레벨을 return, 없으면 -1 return

void CalcUsedSPointAndCSPoint( OUT int& iSPoint, OUT int& iCSPoint );

private:
typedef std::map< int, GuildSkill >	GuildSkillDataMap;
GuildSkillDataMap					m_mapSkillTree;

int									m_iGuildSPoint;
int									m_iGuildCSPoint;
int									m_iMaxGuildCSPoint;
std::wstring						m_wstrGuildCSPointEndDate;
CTime								m_tGuildCSPointEndDate;
};

#endif GUILD_SKILL_TEST
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013


