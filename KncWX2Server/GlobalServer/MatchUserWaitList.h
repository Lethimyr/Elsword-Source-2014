#pragma once

#include "MatchUser.h"


//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


#define KMatchUserWaitListPtr KMatchUserWaitList*
#undef KMatchUserWaitListPtr
SmartPointer( KMatchUserWaitList );


class KMatchUserWaitList
{
public:
    KMatchUserWaitList( IN const int iMatchWaitNumber );
    virtual ~KMatchUserWaitList();

	int		GetMatchWaitNumber() const								{ return m_iMatchWaitNumber; }
	UidType GetPartyUID() const;

	KMatchUserPtr GetMatchUser( IN const UidType iUnitUID );
	bool	AddMatchUser( KMatchUserPtr spMatchUser );
	void	DelMatchUser( IN const UidType iUnitUID );

	int		GetNumMember() const									{ return static_cast<int>(m_mapMember.size()); }
	void	GetUnitUIDList( OUT std::vector< UidType >& vecUnitUIDList ) const;
	void	GetMatchUserInfoList( OUT std::vector< KMatchUserInfo >& vecMatchUserInfo ) const;
	int		GetRatingAverage() const;
	//{{ 2011. 08. 30	최육사	매치 메이킹 개선
#ifdef SERV_PVP_MATCH_UPDATE
	int		GetRatingWeightAverage() const;
#endif SERV_PVP_MATCH_UPDATE
	//}}

	bool	IsExistMatchUser( IN const UidType iUnitUID )			{ return ( m_mapMember.find( iUnitUID ) != m_mapMember.end() ); }

	bool	IsPossibleMatchMakingBegin() const;
	bool	IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank );

	bool	CheckOptimalMatch( IN KMatchUserWaitListPtr spWaitList, OUT int& iOptimalMatchPoint );
	//{{ 2012. 09. 15	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	bool	IsAllPlayersArrangeRank( void );
	bool	IsAllPlayersBeginner( void );
#endif SERV_2012_PVP_SEASON2
	//}}

protected:
	int									m_iMatchWaitNumber;
    std::map< UidType, KMatchUserPtr >	m_mapMember;
	std::vector< KMatchUserPtr >		m_vecWaitList;
};


#endif SERV_PVP_NEW_SYSTEM
//}}