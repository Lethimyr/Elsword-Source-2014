#include "AutoPartyUserWaitList.h"

#include "AutoPartyUserFSM.h"
#include "Enum/Enum.h"
#include "NetError.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KAutoPartyUserWaitList::KAutoPartyUserWaitList( IN const UidType iWaitNumber ) :
m_iWaitNumber( iWaitNumber ),
m_iListType( 0 )
{
}

KAutoPartyUserWaitList::~KAutoPartyUserWaitList()
{
}

UidType KAutoPartyUserWaitList::GetPartyUID() const
{
	UidType iPartyUID = 0;

	BOOST_TEST_FOREACH( KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
        if( iPartyUID == 0 )
		{
			iPartyUID = spAutoPartyUser->GetOldPartyUID();
			continue;
		}

		if( iPartyUID != spAutoPartyUser->GetOldPartyUID() )
		{
			START_LOG( cerr, L"엇.. 이거 이상한데.. 같은 멤버인데 PartyUID가 틀리네? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( iPartyUID )
				<< BUILD_LOG( spAutoPartyUser->GetOldPartyUID() )
				<< END_LOG;
			continue;
		}
	}

	return iPartyUID;
}

void KAutoPartyUserWaitList::GetUnitUIDList( OUT std::vector< UidType >& vecUnitUIDList ) const
{
	vecUnitUIDList.clear();

	BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
        if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다!" )
				<< END_LOG;
			continue;
		}

		vecUnitUIDList.push_back( spAutoPartyUser->GetCharUID() );
	}
}

void KAutoPartyUserWaitList::GetAutoPartyUserInfoList( OUT std::vector< KAutoPartyUserInfo >& vecAutoPartyUserInfo ) const
{
	vecAutoPartyUserInfo.clear();

	BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다!" )
				<< END_LOG;
			continue;
		}

		// 사본을 복사 한다.
		KAutoPartyUserInfo kInfo = spAutoPartyUser->GetAutoPartyUserInfo();
		kInfo.m_iAutoPartyWaitTime = static_cast<int>(spAutoPartyUser->GetAutoPartyCompleteSeconds());
		vecAutoPartyUserInfo.push_back( kInfo );
	}
}

u_char KAutoPartyUserWaitList::GetTopUnitLevel() const
{
	u_char ucTopLevel = 0;

	BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 유저 포인터값이 이상합니다" )
				<< END_LOG;
			return 0;
		}

		if( ucTopLevel < spAutoPartyUser->GetLevel() )
			ucTopLevel = spAutoPartyUser->GetLevel();
	}

	return ucTopLevel;
}

u_char KAutoPartyUserWaitList::GetAverageUnitLevel() const
{
	int iTotalLevel = 0;

    BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 유저 포인터값이 이상합니다" )
				<< END_LOG;
			return 0;
		}

		iTotalLevel += spAutoPartyUser->GetLevel();
	}

	if( GetNumMember() == 0 )
	{
		START_LOG( cerr, L"평균값을 구할려고 하는데 현재 멤버 수가 0입니다. 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetWaitNumber() )
			<< END_LOG;
		return 0;
	}

	const int iAverageLevel = iTotalLevel / GetNumMember();
	return static_cast<u_char>( iAverageLevel );
}

u_char KAutoPartyUserWaitList::GetLevelForListType() const
{
	const int iNotIncludeAverageLevel = GetTopUnitLevel() - 5;
	
	int iTotalLevel = 0;
	int iAverageIncludeUserCount = 0;

	BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
		if( IS_NULL( spAutoPartyUser ) )
		{
			START_LOG( cerr, L"자동 파티 유저 포인터값이 이상합니다" )
				<< END_LOG;
			return 0;
		}

		if( iNotIncludeAverageLevel > static_cast<int>(spAutoPartyUser->GetLevel()) )
			continue;

        iTotalLevel += spAutoPartyUser->GetLevel();
		++iAverageIncludeUserCount;
	}

    if( iAverageIncludeUserCount == 0 )
	{
		START_LOG( cerr, L"캐릭터 평균 레벨값을 구할 대상이 한명도 없습니다. 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( iAverageIncludeUserCount )
			<< END_LOG;
		return 0;
	}

	return static_cast<u_char>( iTotalLevel / iAverageIncludeUserCount );
}

bool KAutoPartyUserWaitList::IsExistUnitType( IN const char cUnitType ) const
{
	BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 유저 포인터값이 이상합니다" )
				<< END_LOG;
			return 0;
		}

		if( spAutoPartyUser->GetUnitType() == cUnitType )
			return true;
	}

    return false;
}

bool KAutoPartyUserWaitList::CheckSameUnitType( IN KAutoPartyUserWaitListPtr spAutoPartyUserWaitList )
{
	BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 유저 포인터값이 이상합니다" )
				<< END_LOG;
			return 0;
		}

		if( spAutoPartyUserWaitList->IsExistUnitType( spAutoPartyUser->GetUnitType() ) == true )
			return true;
	}
	
	return false;
}

//{{ 2012. 11. 14	최육사		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
bool KAutoPartyUserWaitList::CheckSameDifficultyLevel( IN KAutoPartyUserWaitListPtr spAutoPartyUserWaitList )
{
	if( IS_NULL( spAutoPartyUserWaitList ) )
	{
		START_LOG( cerr, L"대기 리스트 포인터값이 이상합니다" )
			<< END_LOG;
		return false;
	}

    return ( GetDifficultyLevel() == spAutoPartyUserWaitList->GetDifficultyLevel() );
}
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}

KAutoPartyUserPtr KAutoPartyUserWaitList::GetAutoPartyUser( IN const UidType iUnitUID )
{
	std::map< UidType, KAutoPartyUserPtr >::iterator mit = m_mapMember.find( iUnitUID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cwarn, L"찾는 매치유저가 없습니다.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"매치 유저 포인터 이상.!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return KAutoPartyUserPtr();
	}

	return mit->second;
}

bool KAutoPartyUserWaitList::AddAutoPartyUser( KAutoPartyUserPtr spAutoPartyUser )
{
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"매치 유저 포인터값이 이상함." )
			<< END_LOG;
		return false;
	}

	if( IsExistAutoPartyUser( spAutoPartyUser->GetCharUID() ) == true )
	{
		START_LOG( cerr, L"이미 매치유저가 등록되어있습니다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( spAutoPartyUser->GetCharUID() )
			<< END_LOG;
		return false;
	}

	m_mapMember.insert( std::make_pair( spAutoPartyUser->GetCharUID(), spAutoPartyUser ) );
	m_vecWaitList.push_back( spAutoPartyUser );
	return true;
}

void KAutoPartyUserWaitList::DelAutoPartyUser( IN const UidType iUnitUID )
{
	m_mapMember.erase( iUnitUID );

	std::vector< KAutoPartyUserPtr >::iterator vit;
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

void KAutoPartyUserWaitList::SetAutoPartyBonus( IN const bool bAutoPartyBonus )
{
	BOOST_TEST_FOREACH( const KAutoPartyUserPtr, spAutoPartyUser, m_vecWaitList )
	{
		if( IS_NULL( spAutoPartyUser ) )
		{
			START_LOG( cerr, L"자동 파티 유저 포인터값이 이상합니다" )
				<< END_LOG;
			continue;
		}

		spAutoPartyUser->SetAutoPartyBonus( bAutoPartyBonus );
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

