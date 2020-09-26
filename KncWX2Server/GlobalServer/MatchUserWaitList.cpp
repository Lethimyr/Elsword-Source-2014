#include "MatchUserWaitList.h"

#include "MatchUserFSM.h"
#include "Enum/Enum.h"
#include "NetError.h"
//{{ 2011. 08. 30	최육사	매치 메이킹 개선
#ifdef SERV_PVP_MATCH_UPDATE
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_MATCH_UPDATE
//}}


//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


KMatchUserWaitList::KMatchUserWaitList( IN const int iMatchWaitNumber ) :
m_iMatchWaitNumber( iMatchWaitNumber )
{
}

KMatchUserWaitList::~KMatchUserWaitList()
{
}

UidType KMatchUserWaitList::GetPartyUID() const
{
	UidType iPartyUID = 0;

	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
        if( iPartyUID == 0 )
		{
			iPartyUID = spMatchUser->GetPartyUID();
			continue;
		}

		if( iPartyUID != spMatchUser->GetPartyUID() )
		{
			START_LOG( cerr, L"엇.. 이거 이상한데.. 같은 멤버인데 PartyUID가 틀리네? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iPartyUID )
				<< BUILD_LOG( spMatchUser->GetPartyUID() )
				<< END_LOG;
			continue;
		}
	}

	return iPartyUID;
}

void KMatchUserWaitList::GetUnitUIDList( OUT std::vector< UidType >& vecUnitUIDList ) const
{
	vecUnitUIDList.clear();

	BOOST_TEST_FOREACH( const KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
        if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다!" )
				<< END_LOG;
			continue;
		}

		vecUnitUIDList.push_back( spMatchUser->GetCharUID() );
	}
}

void KMatchUserWaitList::GetMatchUserInfoList( OUT std::vector< KMatchUserInfo >& vecMatchUserInfo ) const
{
	vecMatchUserInfo.clear();

	BOOST_TEST_FOREACH( const KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다!" )
				<< END_LOG;
			continue;
		}

		// 사본을 복사 한다.
		KMatchUserInfo kInfo = spMatchUser->GetMatchUserInfo();
		kInfo.m_iMatchWaitTime = static_cast<int>(spMatchUser->GetMatchCompleteSeconds());
		vecMatchUserInfo.push_back( kInfo );
	}
}

KMatchUserPtr KMatchUserWaitList::GetMatchUser( IN const UidType iUnitUID )
{
	std::map< UidType, KMatchUserPtr >::iterator mit = m_mapMember.find( iUnitUID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cwarn, L"찾는 매치유저가 없습니다.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 유저 포인터 이상.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KMatchUserPtr();
	}

	return mit->second;
}

bool KMatchUserWaitList::AddMatchUser( KMatchUserPtr spMatchUser )
{
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"매치 유저 포인터값이 이상함." )
			<< END_LOG;
		return false;
	}

	if( IsExistMatchUser( spMatchUser->GetCharUID() ) == true )
	{
		START_LOG( cerr, L"이미 매치유저가 등록되어있습니다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( spMatchUser->GetCharUID() )
			<< END_LOG;
		return false;
	}

	m_mapMember.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser ) );
	m_vecWaitList.push_back( spMatchUser );
	return true;
}

void KMatchUserWaitList::DelMatchUser( IN const UidType iUnitUID )
{
	m_mapMember.erase( iUnitUID );

	std::vector< KMatchUserPtr >::iterator vit;
	for( vit = m_vecWaitList.begin(); vit != m_vecWaitList.end(); ++vit )
	{
		if( (*vit) == NULL )
			continue;

		if( (*vit)->GetCharUID() == iUnitUID )
			break;
	}

	if( vit != m_vecWaitList.end() )
	{
		m_vecWaitList.erase( vit );
	}
}

int	KMatchUserWaitList::GetRatingAverage() const
{
	int iTotalRating = 0;

	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
        if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		iTotalRating += spMatchUser->GetRating();
	}

	const int iNumMember = static_cast<int>(m_vecWaitList.size());
	if( iNumMember == 0 )
	{
		START_LOG( cerr, L"유령 대기자 리스트다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetMatchWaitNumber() )
			<< END_LOG;
		return 0;
	}
	
	return ( iTotalRating / iNumMember );
}

//{{ 2011. 08. 30	최육사	매치 메이킹 개선
#ifdef SERV_PVP_MATCH_UPDATE
int	KMatchUserWaitList::GetRatingWeightAverage() const
{
	if( GetNumMember() == 1 )
	{
		// 멤버가 혼자라면 그냥 평균값을 낸다.
		return GetRatingAverage();
	}

	std::map< UidType, int > mapTeamMemberInfo;
	int iMaxRating = 0;

	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		mapTeamMemberInfo.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser->GetRating() ) );

		if( iMaxRating < spMatchUser->GetRating() )
			iMaxRating = spMatchUser->GetRating();
	}

	const int iRatingWeightEverage = SiKPvpMatchManager()->GetRatingByTeam( mapTeamMemberInfo );
	const int iModifiedMaxRating = static_cast<int>( static_cast<double>(iMaxRating) * 0.8 );

	return max( iRatingWeightEverage, iModifiedMaxRating );
}
#endif SERV_PVP_MATCH_UPDATE
//}}

bool KMatchUserWaitList::IsPossibleMatchMakingBegin() const
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		if( spMatchUser->IsPossibleMatchMakingBegin() == false )
			return false;
	}
    
	return true;
}

bool KMatchUserWaitList::IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank )
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( !spMatchUser )
		{
			START_LOG( cerr, L"매치 유저 포인터 이상." )
				<< END_LOG;
			return false;
		}

		if( spMatchUser->GetPvpRank() != eRank )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserWaitList::CheckOptimalMatch( IN KMatchUserWaitListPtr spWaitList, OUT int& iOptimalMatchPoint )
{
	//{{ 2011. 08. 30	최육사	매치 메이킹 개선
#ifdef SERV_PVP_MATCH_UPDATE
	iOptimalMatchPoint = ::abs( GetRatingWeightAverage() - spWaitList->GetRatingWeightAverage() );
#else
	iOptimalMatchPoint = ::abs( GetRatingAverage() - spWaitList->GetRatingAverage() );
#endif SERV_PVP_MATCH_UPDATE
	//}}	
	return true;
}

//{{ 2012. 09. 15	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchUserWaitList::IsAllPlayersArrangeRank( void )
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( !spMatchUser )
		{
			START_LOG( cerr, L"매치 유저 포인터 이상." )
				<< END_LOG;
			return false;
		}

		if( spMatchUser->IsArrangeRank() == false )
		{
			return false;
		}
	}
	return true;
}

bool KMatchUserWaitList::IsAllPlayersBeginner( void )
{
	BOOST_TEST_FOREACH( KMatchUserPtr, spMatchUser, m_vecWaitList )
	{
		if( !spMatchUser )
		{
			START_LOG( cerr, L"매치 유저 포인터 이상." )
				<< END_LOG;
			return false;
		}

		if( spMatchUser->IsBeginner() == false )
		{
			return false;
		}
	}
	return true;
}
#endif SERV_2012_PVP_SEASON2
//}}

#endif SERV_PVP_NEW_SYSTEM
//}}

