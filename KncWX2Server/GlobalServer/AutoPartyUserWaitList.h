#pragma once

#include "AutoPartyUser.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


#define KAutoPartyUserWaitListPtr KAutoPartyUserWaitList*
#undef KAutoPartyUserWaitListPtr
SmartPointer( KAutoPartyUserWaitList );


class KAutoPartyUserWaitList
{
public:
    KAutoPartyUserWaitList( IN const UidType iWaitNumber );
    virtual ~KAutoPartyUserWaitList();

	UidType GetWaitNumber() const									{ return m_iWaitNumber; }
	UidType GetPartyUID() const;

	int		GetListType() const										{ return m_iListType; }
	void	SetListType( IN const int iListType )					{ m_iListType = iListType; }

	KAutoPartyUserPtr GetAutoPartyUser( IN const UidType iUnitUID );
	bool	AddAutoPartyUser( KAutoPartyUserPtr spAutoPartyUser );
	void	DelAutoPartyUser( IN const UidType iUnitUID );

	int		GetNumMember() const									{ return static_cast<int>(m_mapMember.size()); }
	void	GetUnitUIDList( OUT std::vector< UidType >& vecUnitUIDList ) const;
	void	GetAutoPartyUserInfoList( OUT std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo ) const;	

	u_char	GetTopUnitLevel() const;
	u_char	GetAverageUnitLevel() const;
	u_char	GetLevelForListType() const;
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	char	GetDifficultyLevel() const								{ return m_kAutoPartyCondition.m_cDifficultyLevel; }
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

	bool	IsExistAutoPartyUser( IN const UidType iUnitUID )		{ return ( m_mapMember.find( iUnitUID ) != m_mapMember.end() ); }
	bool	IsExistUnitType( IN const char cUnitType ) const;

	bool	CheckSameUnitType( IN KAutoPartyUserWaitListPtr spAutoPartyUserWaitList );
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	bool	CheckSameDifficultyLevel( IN KAutoPartyUserWaitListPtr spAutoPartyUserWaitList );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
	double	ElapsedWaitTime() const								{ return m_tWaitTimer.elapsed(); }

	void	SetAutoPartyBonus( IN const bool bAutoPartyBonus );

	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	void	SetAutoPartyCondition( IN const KAutoPartyConditionInfo& kInfo )	{ m_kAutoPartyCondition = kInfo; }
	const KAutoPartyConditionInfo& GetAutoPartyCondition() const				{ return m_kAutoPartyCondition; }
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}

protected:
	UidType									m_iWaitNumber;
	int										m_iListType;
    std::map< UidType, KAutoPartyUserPtr >	m_mapMember;
	std::vector< KAutoPartyUserPtr >		m_vecWaitList;
	boost::timer							m_tWaitTimer;
	//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	KAutoPartyConditionInfo					m_kAutoPartyCondition;
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	//}}
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}