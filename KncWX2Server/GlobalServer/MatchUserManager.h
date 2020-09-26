#pragma once

#include "MatchUser.h"


//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


SmartPointer( KMatchUserFSM );


class KMatchUserManager
{
public:
	enum RATING_ENUM
	{
		RE_HIGH = 0,
		RE_MIDDLE,
		RE_LOW,

		RE_MAX,
	};

	struct SPvpTeamSet
	{
		std::vector< UidType > m_vecRedTeam;
		std::vector< UidType > m_vecBlueTeam;
	};

public:
    KMatchUserManager();
    virtual ~KMatchUserManager();

	void	Init( IN const char cMatchType );

	// 초기화 및 삭제
	bool	Enter( IN const KMatchUserInfo& kInfo );
	bool	Enter( IN const std::vector< KMatchUserInfo >& vecMatchUserList );
    void	Reset();
	bool	DeleteUser( IN const UidType iUnitUID );

	// 정보 얻기
    KMatchUserPtr GetUser( IN const UidType iUnitUID );
	void	GetMatchUserList( OUT std::vector< UidType >& vecUserList );
    int		GetNumMember() const;
	bool	IsExist( IN const UidType iUnitUID )	{ return ( m_mapMember.find( iUnitUID ) != m_mapMember.end() ); }
	int		GetRatingAverage() const;
	void	GetMatchWaitTimeList( OUT std::map< UidType, int >& mapMatchWaitTime );

	// 랭크
	bool	IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank );
	
	//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	struct SPartyMembers
	{
		int						m_iRating;
		std::vector<UidType>	vecPartyMembers;
		SPartyMembers( const int iRating, const std::vector<UidType>& vecPartyMembers )
		{
			m_iRating = iRating;
			this->vecPartyMembers = vecPartyMembers;
		}
		bool operator < ( const SPartyMembers& rhs ) const;
	};
	bool	IsAllPlayersBeginner( void );
	bool	ExistArrangeRankPlayer( void );
	int		GetRatingWeightAverage( void ) const;
	static int GetRatingWeightAverage( std::vector<KRoomUserInfo> vecTeam );
#endif SERV_2012_PVP_SEASON2
	//}}
 
	// 수락
	bool	Accept( IN const UidType iUnitUID );
	bool	IsAllPlayersAccept();

	// 대전맵
	bool	UpdateSelectedPvpMap( IN const UidType iUnitUID, IN const std::set< short >& setPvpMapInfo );
	void	GetAllPlayersSelectedPvpMap( OUT std::map< short, int >& mapPvpMapInfo ) const;

	// RoomUserInfo
	void	ClearAllPlayerRoomUserInfo();
	bool	UpdateRoomUserInfo( IN const KRoomUserInfo& kRoomUserInfo );
	bool	IsAllPlayersUpdatedRoomUserInfo();

	// 게임모드 선정
	bool	UpdatePvpGameMode( IN const UidType iUnitUID, IN const bool bPvpPowerMode, IN const bool bPvpItemMode );
	void	DecisionGameMode( OUT bool& bIsItemMode ) const;

	// 팀 선정
	bool	TeamMemberSelection( OUT std::vector< KRoomUserInfo >& vecRedTeam, OUT std::vector< KRoomUserInfo >& vecBlueTeam );
	bool	GetRoomUserInfo( IN const std::vector< UidType >& vecUnitUIDList, OUT std::vector< KRoomUserInfo >& vecRoomUserInfo );
	
	//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
	void	GetCancelUserList( OUT std::vector< UidType >& vecCancelUserList );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
	//}

protected:
	bool	MakeTeamOptimalMatch( OUT std::vector< UidType >& vecRedTeam, OUT std::vector< UidType >& vecBlueTeam );
	bool	GetTeamRatingGapByRatingLevel( IN const std::vector< UidType >& vecSortedUserList, 
											IN const std::vector< int >& vecHighRating, 
											IN const std::vector< int >& vecMiddleRating, 
											IN const std::vector< int >& vecLowRating,
											OUT int& iGepTeamRating,
											OUT std::vector< UidType >& vecRedTeamResult,
											OUT std::vector< UidType >& vecBlueTeamResult );

protected:
	char								m_cMatchType;
	u_int								m_uiTotalPlayerCount;
	u_int								m_uiPlayerCount;
    std::map< UidType, KMatchUserPtr >	m_mapMember;
	static FSMclassPtr                  ms_spUserFSM;
};


#endif SERV_PVP_NEW_SYSTEM
//}}