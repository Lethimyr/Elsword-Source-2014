#include "Match.h"
#include "MatchUserManager.h"

#include "Enum/Enum.h"
#include "NetError.h"
#include "MatchUserFSM.h"
#include "CompareLimitList.h"
//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}


//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


FSMclassPtr KMatchUserManager::ms_spUserFSM = KMatchUserFSMPtr( new KMatchUserFSM );


KMatchUserManager::KMatchUserManager() :
m_cMatchType( 0 ),
m_uiTotalPlayerCount( 0 ),
m_uiPlayerCount( 0 )
{
}

KMatchUserManager::~KMatchUserManager()
{
}

void KMatchUserManager::Init( IN const char cMatchType )
{
	m_cMatchType		 = cMatchType;
	m_uiPlayerCount		 = KMatch::GetMatchPlayerCount( static_cast<SEnum::MATCH_TYPE>(cMatchType) );
	m_uiTotalPlayerCount = KMatch::GetMatchTotalPlayerCount( static_cast<SEnum::MATCH_TYPE>(cMatchType) );
}

bool KMatchUserManager::Enter( IN const KMatchUserInfo& kInfo )
{
    if( IsExist( kInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"이미 입장해 있는 유저입니다!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	if( ( m_mapMember.size() + 1 )  > m_uiTotalPlayerCount )
	{
		START_LOG( cerr, L"최대 입장 인원 제한에 걸렸습니다." )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	KMatchUserPtr spMatchUser( new KMatchUser( kInfo ) );
	spMatchUser->SetFSM( ms_spUserFSM );
	m_mapMember.insert( std::make_pair( kInfo.m_iUnitUID, spMatchUser ) );
	return true;
}

bool KMatchUserManager::Enter( IN const std::vector< KMatchUserInfo >& vecMatchUserList )
{
	if( ( m_mapMember.size() + vecMatchUserList.size() )  > m_uiTotalPlayerCount )
	{
		START_LOG( cerr, L"최대 입장 인원 제한에 걸렸습니다." )
			<< BUILD_LOG( m_uiTotalPlayerCount )
			<< BUILD_LOG( m_mapMember.size() )
			<< BUILD_LOG( vecMatchUserList.size() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KMatchUserInfo, kInfo, vecMatchUserList )
	{
		if( Enter( kInfo ) == false )
		{
			START_LOG( cerr, L"매치 입장 처리 실패!" )
				<< END_LOG;
		}
	}

	return true;
}

void KMatchUserManager::Reset()
{
}

KMatchUserPtr KMatchUserManager::GetUser( IN const UidType iUnitUID )
{
    std::map< UidType, KMatchUserPtr >::iterator mit;
    mit = m_mapMember.find( iUnitUID );
    if( mit == m_mapMember.end() )
    {
        START_LOG( cerr, L"존재하지 않는 유저." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return KMatchUserPtr();
    }

    return mit->second;
}

void KMatchUserManager::GetMatchUserList( OUT std::vector< UidType >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		vecUserList.push_back( mit->first );
	}
}

bool KMatchUserManager::DeleteUser( IN const UidType iUnitUID )
{
    std::map< UidType, KMatchUserPtr >::iterator mit;
    mit = m_mapMember.find( iUnitUID );
    if( mit == m_mapMember.end() )
    {
        START_LOG( cerr, L"존재하지 않는 룸유저임." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;
        return true;
    }

    m_mapMember.erase( mit );
    return true;
}

int KMatchUserManager::GetNumMember() const
{
    return static_cast<int>(m_mapMember.size());
}

int	KMatchUserManager::GetRatingAverage() const
{
	int iTotalRating = 0;

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		iTotalRating += spMatchUser->GetRating();
	}

	const int iNumMember = GetNumMember();
	if( iNumMember == 0 )
	{
		START_LOG( cerr, L"멤버가 존재 하지 않는 매치다! 절대 일어나서는 안되는 에러!" )
			<< END_LOG;
		return 0;
	}

	return ( iTotalRating / iNumMember );
}

void KMatchUserManager::GetMatchWaitTimeList( OUT std::map< UidType, int >& mapMatchWaitTime )
{
	mapMatchWaitTime.clear();

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< END_LOG;
			continue;
		}

		mapMatchWaitTime.insert( std::make_pair( spMatchUser->GetCharUID(), spMatchUser->GetMatchWaitTime() ) );
	}
}

bool KMatchUserManager::IsAllPlayersSameRank( IN const CXSLUnit::PVP_RANK eRank )
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->GetPvpRank() != eRank )
		{
			return false;
		}
	}

	return true;
}

//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchUserManager::IsAllPlayersBeginner( void )
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsBeginner() == false )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::ExistArrangeRankPlayer( void )
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsArrangeRank() == true )
		{
			return true;
		}
	}
	return false;
}

int	KMatchUserManager::GetRatingWeightAverage( void ) const
{
	if( GetNumMember() == 1 )
	{
		// 멤버가 혼자라면 그냥 평균값을 낸다.
		return GetRatingAverage();
	}

	std::map< UidType, int > mapTeamMemberInfo;
	int iMaxRating = 0;

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
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

int KMatchUserManager::GetRatingWeightAverage( std::vector<KRoomUserInfo> vecTeam )
{
	if( vecTeam.empty() == true )
	{
		return 0;
	}

	if( vecTeam.size() == 1 )
	{
		KRoomUserInfo kRoomUserInfo = vecTeam.front();
		// 멤버가 혼자라면 그냥 Rating을 반환한다.
		return kRoomUserInfo.m_iRating;
	}

	std::map< UidType, int > mapTeamMemberInfo;
	int iMaxRating = 0;

	BOOST_TEST_FOREACH( const KRoomUserInfo&, kRoomUserInfo, vecTeam )
	{
		mapTeamMemberInfo.insert( std::make_pair( kRoomUserInfo.m_nUnitUID, kRoomUserInfo.m_iRating ) );

		if( iMaxRating < kRoomUserInfo.m_iRating )
			iMaxRating = kRoomUserInfo.m_iRating;
	}

	const int iRatingWeightAverage = SiKPvpMatchManager()->GetRatingByTeam( mapTeamMemberInfo );
	const int iModifiedMaxRating = static_cast<int>( static_cast<double>( iMaxRating ) * 0.8 );

	return max( iRatingWeightAverage, iModifiedMaxRating );
}
#endif SERV_2012_PVP_SEASON2
//}}

bool KMatchUserManager::Accept( IN const UidType iUnitUID )
{
	KMatchUserPtr spMatchUser = GetUser( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	return spMatchUser->Accept();
}

bool KMatchUserManager::IsAllPlayersAccept()
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsAccepted() == false )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::UpdateSelectedPvpMap( IN const UidType iUnitUID, IN const std::set< short >& setPvpMapInfo )
{
	KMatchUserPtr spMatchUser = GetUser( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spMatchUser->UpdateSelectedPvpMap( setPvpMapInfo );
	return true;
}

void KMatchUserManager::GetAllPlayersSelectedPvpMap( OUT std::map< short, int >& mapPvpMapInfo ) const
{
	mapPvpMapInfo.clear();

	std::map< short, int >::iterator mitSM;
    
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		BOOST_TEST_FOREACH( const short, sMapID, spMatchUser->GetSelectedPvpMap() )
		{
			mitSM = mapPvpMapInfo.find( sMapID );
			if( mitSM == mapPvpMapInfo.end() )
			{
				mapPvpMapInfo.insert( std::make_pair( sMapID, 1 ) );
			}
			else
			{
                ++mitSM->second;
			}
		}
	}    
}

void KMatchUserManager::ClearAllPlayerRoomUserInfo()
{
	std::map< UidType, KMatchUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}
        
		spMatchUser->ClearRoomUserInfo();
	}
}

bool KMatchUserManager::UpdateRoomUserInfo( IN const KRoomUserInfo& kRoomUserInfo )
{
	KMatchUserPtr spMatchUser = GetUser( kRoomUserInfo.m_nUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( kRoomUserInfo.m_nUnitUID )
			<< END_LOG;
		return false;
	}

	spMatchUser->UpdateRoomUserInfo( kRoomUserInfo );
	return true;
}

bool KMatchUserManager::IsAllPlayersUpdatedRoomUserInfo()
{
	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsEmptyRoomUserInfo() == true )
		{
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::UpdatePvpGameMode( IN const UidType iUnitUID, IN const bool bPvpPowerMode, IN const bool bPvpItemMode )
{
	KMatchUserPtr spMatchUser = GetUser( iUnitUID );
	if( spMatchUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spMatchUser->UpdatePvpGameMode( bPvpPowerMode, bPvpItemMode );
	return true;
}

void KMatchUserManager::DecisionGameMode( OUT bool& bIsItemMode ) const
{
	bIsItemMode = false;
	int iPowerModeCount = 0;
	int iItemModeCount = 0;

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KMatchUserPtr spMatchUser = mit->second;
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spMatchUser->IsPvpPowerMode() == true )
		{
			++iPowerModeCount;
		}

		if( spMatchUser->IsPvpItemMode() == true )
		{
			++iItemModeCount;
		}
	}

	if( iPowerModeCount > iItemModeCount )
	{
		// 파워모드가 더 많이 선택됨!
		bIsItemMode = false;
		return;
	}
	else if( iPowerModeCount < iItemModeCount )
	{
		bIsItemMode = false;
		return;
	}

	// 만약 양쪽의 카운트가 같다면 50%확률로 선택!
	const int iRandomResult = rand() % 2;
	bIsItemMode = ( iRandomResult == 0 );
}

//{{ 2012. 09. 19	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
bool KMatchUserManager::SPartyMembers::operator < ( const SPartyMembers& rhs ) const
{
	if( vecPartyMembers.size() < rhs.vecPartyMembers.size() )		/// 파티원의 수가 많은 것이 앞으로 오도록 정렬
		return false;
	else if( vecPartyMembers.size() == rhs.vecPartyMembers.size() )	/// 파티원의 수가 같으면 레이팅의 합이 큰것이 앞으로 오도록 정렬
		return ( m_iRating < rhs.m_iRating ? false : true );
	else
		return true;
}
#endif SERV_2012_PVP_SEASON2
//}}

bool KMatchUserManager::TeamMemberSelection( OUT std::vector< KRoomUserInfo >& vecRedTeam, OUT std::vector< KRoomUserInfo >& vecBlueTeam )
{
	vecRedTeam.clear();
	vecBlueTeam.clear();


	//////////////////////////////////////////////////////////////////////////	
	// 1. 팀 구성하기전에 인원수 맞는지부터 확인!
	if( m_uiTotalPlayerCount != m_mapMember.size() )
	{
		START_LOG( cerr, L"이제 팀 구성하려는데 인원수가 안맞네? 절대 일어나서는 안되는에러!" )
			<< BUILD_LOG( m_uiTotalPlayerCount )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;
		return false;
	}


	//////////////////////////////////////////////////////////////////////////	
	// 2. 파티원끼리 묶자!
	std::map< UidType, std::vector< UidType > > mapPartyMembers;

	{
		std::map< UidType, std::vector< UidType > >::iterator mitPM;
		std::map< UidType, KMatchUserPtr >::const_iterator mit;
		for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
		{
			KMatchUserPtr spMatchUser = mit->second;
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"룸 유저 포인터 이상." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				return false;
			}

			// 파티 정보가 있으면 파티원끼리 묶자!
			mitPM = mapPartyMembers.find( spMatchUser->GetPartyUID() );
			if( mitPM == mapPartyMembers.end() )
			{
				std::vector< UidType > vecMemberList;
				vecMemberList.push_back( spMatchUser->GetCharUID() );
				mapPartyMembers.insert( std::make_pair( spMatchUser->GetPartyUID(), vecMemberList ) );
			}
			else
			{
				mitPM->second.push_back( spMatchUser->GetCharUID() );
			}
		}
	}	


	//////////////////////////////////////////////////////////////////////////	
	// 3. 파티가 한팀이 되어 나올수 있는지 보자!
	std::vector< UidType > vecRedTeamUnitUIDList;
	std::vector< UidType > vecBlueTeamUnitUIDList;

	//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	// 인원수가 많고, 레이팅이 큰 순으로 정렬한다.
	std::multiset<SPartyMembers> msetPartyMembers;
	std::vector<KRoomUserInfo> vecTempTeam;
	for( std::map< UidType, std::vector< UidType > >::iterator it=mapPartyMembers.begin(); it != mapPartyMembers.end(); ++it )
	{
		const UidType iPartyUID = it->first;
		const std::vector<UidType>& vecUnitUID = it->second;

		// party uid가 0인 경우는 파티를 만들지 않고 혼자서 매칭 신청한 경우다.
		// 파티에 소속된 것이 아니기 때문에 개개인으로 다루어야 한다.
		if( iPartyUID == 0 )
		{
			BOOST_TEST_FOREACH( const UidType&, iUnitUID, vecUnitUID )
			{
				std::vector<UidType> vecTemp;
				vecTemp.push_back( iUnitUID );
				GetRoomUserInfo( vecTemp, vecTempTeam );
				msetPartyMembers.insert( SPartyMembers( GetRatingWeightAverage( vecTempTeam ), vecTemp ) );
			}
			continue;
		}
		GetRoomUserInfo( vecUnitUID, vecTempTeam );
		msetPartyMembers.insert( SPartyMembers( GetRatingWeightAverage( vecTempTeam ), vecUnitUID ) );
	}

	int iTotalRatingRed		= 0;
	int iTotalRatingBlue	= 0;
	for( std::multiset<SPartyMembers>::iterator it=msetPartyMembers.begin(); it != msetPartyMembers.end(); ++it )
	{
		const SPartyMembers& sPartyMembers = *it;

		// 레드팀 레이팅이 더 작고 들어갈 공간이 있다면, 레드팀에 삽입
		if( ( iTotalRatingRed < iTotalRatingBlue ) && ( ( vecRedTeamUnitUIDList.size() + sPartyMembers.vecPartyMembers.size() ) <= m_uiPlayerCount ) )
		{
			vecRedTeamUnitUIDList.insert( vecRedTeamUnitUIDList.end(), sPartyMembers.vecPartyMembers.begin(), sPartyMembers.vecPartyMembers.end() );
			iTotalRatingRed += sPartyMembers.m_iRating;
		}
		else
		{
			// 그렇지 않을 경우
			if( ( vecBlueTeamUnitUIDList.size() + sPartyMembers.vecPartyMembers.size() ) <= m_uiPlayerCount )
			{
				// 블루 팀에 공간이 있다면, 블루팀에 넣고
				vecBlueTeamUnitUIDList.insert( vecBlueTeamUnitUIDList.end(), sPartyMembers.vecPartyMembers.begin(), sPartyMembers.vecPartyMembers.end() );
				iTotalRatingBlue += sPartyMembers.m_iRating;
			}
			else
			{
				// 블루팀에 공간이 없다면, 레드팀에 넣는다.
				vecRedTeamUnitUIDList.insert( vecRedTeamUnitUIDList.end(), sPartyMembers.vecPartyMembers.begin(), sPartyMembers.vecPartyMembers.end() );
				iTotalRatingRed += sPartyMembers.m_iRating;
			}
		}
	}
#else

	{
		std::set< UidType > setInsertedPartyUID;
		std::map< UidType, std::vector< UidType > >::const_iterator mitPM;
		for( mitPM = mapPartyMembers.begin(); mitPM != mapPartyMembers.end(); ++mitPM )
		{
			const UidType iPartyUID = mitPM->first;
			const std::vector< UidType >& vecMemberList = mitPM->second;
			
			// 파티가 아니면 건너뛰자!
			if( iPartyUID == 0 )
				continue;

			// 솔로 플레이어는 건너뛰자!
			if( vecMemberList.size() == 1 )
				continue;

			// 파티 멤버수와 한팀 인원수가 같은 경우!
            if( vecMemberList.size() != m_uiPlayerCount )
			{
				START_LOG( cerr, L"파티인데 솔로도 아니면서 인원수가 안맞네.. 일어나서는 안되는 에러!" )
					<< BUILD_LOG( iPartyUID )
					<< BUILD_LOG( vecMemberList.size() )
					<< BUILD_LOG( m_uiPlayerCount )
					<< END_LOG;
				return false;
			}

			// 레드팀이 비어있으면 레드팀으로 선정! 레드팀이 이미 결정되어있으면 블루팀으로 선정!
			if( vecRedTeamUnitUIDList.empty() )
			{
				vecRedTeamUnitUIDList = vecMemberList;
				setInsertedPartyUID.insert( iPartyUID );
				continue;
			}
			else if( vecBlueTeamUnitUIDList.empty() )
			{
                vecBlueTeamUnitUIDList = vecMemberList;
				setInsertedPartyUID.insert( iPartyUID );
				continue;
			}
			else
			{
				START_LOG( cerr, L"이미 모든팀이 선정되었는데 또 인원이 남았나? 절대 일어나서는 안되는 에러!" )
					<< BUILD_LOG( vecRedTeamUnitUIDList.size() )
					<< BUILD_LOG( vecBlueTeamUnitUIDList.size() )
					<< BUILD_LOG( vecMemberList.size() )
					<< END_LOG;
                return false;
			}
		}

		// 팀 구성이 완료된 파티는 리스트에서 지운다!
		BOOST_TEST_FOREACH( const UidType, iPartyUID, setInsertedPartyUID )
		{
            mapPartyMembers.erase( iPartyUID );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 4. 레드팀만 구성이 완료된 경우!
	if( vecRedTeamUnitUIDList.size() == m_uiPlayerCount  &&  vecBlueTeamUnitUIDList.empty() == true )
	{
		std::map< UidType, std::vector< UidType > >::const_iterator mitPM;
		for( mitPM = mapPartyMembers.begin(); mitPM != mapPartyMembers.end(); ++mitPM )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, mitPM->second )
			{
				vecBlueTeamUnitUIDList.push_back( iUnitUID );
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 4. 블루팀만 구성이 완료된 경우!
	else if( vecBlueTeamUnitUIDList.size() == m_uiPlayerCount  &&  vecRedTeamUnitUIDList.empty() == true )
	{
		std::map< UidType, std::vector< UidType > >::const_iterator mitPM;
		for( mitPM = mapPartyMembers.begin(); mitPM != mapPartyMembers.end(); ++mitPM )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, mitPM->second )
			{
				vecRedTeamUnitUIDList.push_back( iUnitUID );
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 5. 모든 팀 구성이 완료되지 않은 경우!
	else if( vecRedTeamUnitUIDList.empty() == true  &&  vecBlueTeamUnitUIDList.empty() == true )
	{
        if( MakeTeamOptimalMatch( vecRedTeamUnitUIDList, vecBlueTeamUnitUIDList ) == false )
		{
			START_LOG( cerr, L"최적의 팀 구성에 실패하였습니다!" )
				<< BUILD_LOG( m_uiPlayerCount )
				<< BUILD_LOG( GetNumMember() )
				<< END_LOG;
			return false;
		}
	}
#endif SERV_2012_PVP_SEASON2
	//}}


	//////////////////////////////////////////////////////////////////////////
	// 6. 여기까지 왔는데 팀 구성이 완료되지 않았다면 문제가 있는거다!
	if( vecRedTeamUnitUIDList.size() != m_uiPlayerCount  ||  vecBlueTeamUnitUIDList.size() != m_uiPlayerCount )
	{
		START_LOG( cerr, L"매칭 팀 구성을 실패하였습니다!" )
			<< BUILD_LOG( vecRedTeamUnitUIDList.size() )
			<< BUILD_LOG( vecBlueTeamUnitUIDList.size() )
			<< END_LOG;
		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	// 7. RoomUserInfo를 얻자!
	{
		if( GetRoomUserInfo( vecRedTeamUnitUIDList, vecRedTeam ) == false )
		{
			START_LOG( cerr, L"RoomUserInfo 얻기가 실패하였습니다." )
				<< BUILD_LOG( vecRedTeamUnitUIDList.size() )
				<< END_LOG;
			return false;
		}

		if( GetRoomUserInfo( vecBlueTeamUnitUIDList, vecBlueTeam ) == false )
		{
			START_LOG( cerr, L"RoomUserInfo 얻기가 실패하였습니다." )
				<< BUILD_LOG( vecBlueTeamUnitUIDList.size() )
				<< END_LOG;
			return false;
		}
	}

	return true;
}

bool KMatchUserManager::GetRoomUserInfo( IN const std::vector< UidType >& vecUnitUIDList, OUT std::vector< KRoomUserInfo >& vecRoomUserInfo )
{
	vecRoomUserInfo.clear();

    BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUnitUIDList )
	{
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터 이상." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			return false;
		}
        
		KRoomUserInfo kInfo;
		if( spMatchUser->GetRoomUserInfo( kInfo ) == false )
		{
			START_LOG( cerr, L"룸 유저 정보가 없다!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			return false;
		}

		vecRoomUserInfo.push_back( kInfo );
	}

	return true;
}

bool KMatchUserManager::MakeTeamOptimalMatch( OUT std::vector< UidType >& vecRedTeam, OUT std::vector< UidType >& vecBlueTeam )
{
	vecRedTeam.clear();
	vecBlueTeam.clear();

	std::vector< UidType > vecSortedUserList;

	//////////////////////////////////////////////////////////////////////////	
	// 1, 높은 레이팅 순으로 정렬한다!
	{
		KCompareLimitList<UidType,6> listSortedUserList;

		std::map< UidType, KMatchUserPtr >::const_iterator mit;
		for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
		{
			KMatchUserPtr spMatchUser = mit->second;
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"룸 유저 포인터 이상." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				return false;
			}

			listSortedUserList.IfLowerValuePushBack( spMatchUser->GetRating(), spMatchUser->GetCharUID() );
		}

		listSortedUserList.GetValueList( vecSortedUserList );
	}
	

	//////////////////////////////////////////////////////////////////////////	
	// (2) 차례로 끊어서 2단계 또는 3단계를 만든다.
	std::vector< int > vecIndex[RE_MAX];

	{
		// 정렬된 유저 수 만큼 구분해서 넣습니다!
		for( u_int ui = 0; ui < vecSortedUserList.size(); ++ui )
		{
			const bool bIsRed = ( ( ui % 2 ) == 0 );
			const int iIndex = ( ui / 2 );
			if( iIndex < 0  ||  iIndex >= RE_MAX )
			{
				START_LOG( cerr, L"이상한 인덱스 값이 나왔다! 일어나면 안되는 에러!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( ui )
					<< BUILD_LOG( iIndex )
					<< END_LOG;
				return false;
			}

			if( bIsRed )
			{
				vecIndex[iIndex].push_back( ui );
			}
			else
			{
				vecIndex[iIndex].push_back( ui );
			}
		}
	}


	//////////////////////////////////////////////////////////////////////////	
	// (3) 각 경우의 수 별로 최적의 레이팅을 찾아내어 팀으로 구성한다!
	std::vector< UidType > vecRedTeamTemp;
	std::vector< UidType > vecBlueTeamTemp;

	{		
		int iBestOptimalMatchRating = INT_MAX;
		
		{
			// 첫번째 경우의 수
			int iGapTeamRating = 0;
			if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
			{
				START_LOG( cerr, L"팀 레이팅 차이값 구하기 실패!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( vecIndex[RE_HIGH].size() )
					<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
					<< BUILD_LOG( vecIndex[RE_LOW].size() )
					<< END_LOG;
				return false;
			}

			// 최적의 매치 구성 인가?
			if( iGapTeamRating < iBestOptimalMatchRating )
			{
				iBestOptimalMatchRating = iGapTeamRating;
				vecRedTeam = vecRedTeamTemp;
				vecBlueTeam = vecBlueTeamTemp;
			}
		}

		while( next_permutation( vecIndex[RE_LOW].begin(), vecIndex[RE_LOW].end() ) )
		{
			// 그 다음 경우의 수
			int iGapTeamRating = 0;
			std::vector< int > vecIndexResult;
			if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
			{
				START_LOG( cerr, L"팀 레이팅 차이값 구하기 실패!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( vecIndex[RE_HIGH].size() )
					<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
					<< BUILD_LOG( vecIndex[RE_LOW].size() )
					<< END_LOG;
				return false;
			}

			// 최적의 매치 구성 인가?
			if( iGapTeamRating < iBestOptimalMatchRating )
			{
				iBestOptimalMatchRating = iGapTeamRating;
				vecRedTeam = vecRedTeamTemp;
				vecBlueTeam = vecBlueTeamTemp;
			}
		}

		while( next_permutation( vecIndex[RE_MIDDLE].begin(), vecIndex[RE_MIDDLE].end() ) )
		{
			// 그 다음 경우의 수
			int iGapTeamRating = 0;
			std::vector< int > vecIndexResult;
			if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
			{
				START_LOG( cerr, L"팀 레이팅 차이값 구하기 실패!" )
					<< BUILD_LOG( vecSortedUserList.size() )
					<< BUILD_LOG( vecIndex[RE_HIGH].size() )
					<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
					<< BUILD_LOG( vecIndex[RE_LOW].size() )
					<< END_LOG;
				return false;
			}

			// 최적의 매치 구성 인가?
			if( iGapTeamRating < iBestOptimalMatchRating )
			{
				iBestOptimalMatchRating = iGapTeamRating;
				vecRedTeam = vecRedTeamTemp;
				vecBlueTeam = vecBlueTeamTemp;
			}

			while( next_permutation( vecIndex[RE_LOW].begin(), vecIndex[RE_LOW].end() ) )
			{
				// 그 다음 경우의 수
				int iGapTeamRating = 0;
				std::vector< int > vecIndexResult;
				if( GetTeamRatingGapByRatingLevel( vecSortedUserList, vecIndex[RE_HIGH], vecIndex[RE_MIDDLE], vecIndex[RE_LOW], iGapTeamRating, vecRedTeamTemp, vecBlueTeamTemp ) == false )
				{
					START_LOG( cerr, L"팀 레이팅 차이값 구하기 실패!" )
						<< BUILD_LOG( vecSortedUserList.size() )
						<< BUILD_LOG( vecIndex[RE_HIGH].size() )
						<< BUILD_LOG( vecIndex[RE_MIDDLE].size() )
						<< BUILD_LOG( vecIndex[RE_LOW].size() )
						<< END_LOG;
					return false;
				}

				// 최적의 매치 구성 인가?
				if( iGapTeamRating < iBestOptimalMatchRating )
				{
					iBestOptimalMatchRating = iGapTeamRating;
					vecRedTeam = vecRedTeamTemp;
					vecBlueTeam = vecBlueTeamTemp;
				}
			}
		}
	}

	return true;
}

bool KMatchUserManager::GetTeamRatingGapByRatingLevel( IN const std::vector< UidType >& vecSortedUserList, 
														IN const std::vector< int >& vecHighRating, 
														IN const std::vector< int >& vecMiddleRating, 
														IN const std::vector< int >& vecLowRating,
														OUT int& iGepTeamRating,
														OUT std::vector< UidType >& vecRedTeamResult,
														OUT std::vector< UidType >& vecBlueTeamResult )
{
	iGepTeamRating = 0;
	vecRedTeamResult.clear();
	vecBlueTeamResult.clear();

	std::map< UidType, int > mapRedTeamRating;
	std::map< UidType, int > mapBlueTeamRating;	

	//////////////////////////////////////////////////////////////////////////	
	// high rating 유저 등록
	for( u_int ui = 0; ui < vecHighRating.size(); ++ui )
	{
		const u_int iUserIndex = vecHighRating[ui];
		if( iUserIndex < 0  ||  iUserIndex >= vecSortedUserList.size() )
		{
			START_LOG( cerr, L"정리된 유저 리스트에 존재하지 않는 index값이다! 이건 있을수 없는 일이야!" )
				<< BUILD_LOG( vecSortedUserList.size() )
				<< BUILD_LOG( iUserIndex )
				<< END_LOG;
			return false;
		}

		const UidType iUnitUID = vecSortedUserList[iUserIndex];
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// 팀 레이팅을 구하기 위해 정리
		const bool bIsRed = ( ( ui % 2 ) == 0 );
		if( bIsRed )
		{
			mapRedTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecRedTeamResult.push_back( iUnitUID );
		}
		else
		{
			mapBlueTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecBlueTeamResult.push_back( iUnitUID );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// middle rating 유저 등록
	for( u_int ui = 0; ui < vecMiddleRating.size(); ++ui )
	{
		const u_int iUserIndex = vecMiddleRating[ui];
		if( iUserIndex < 0  ||  iUserIndex >= vecSortedUserList.size() )
		{
			START_LOG( cerr, L"정리된 유저 리스트에 존재하지 않는 index값이다! 이건 있을수 없는 일이야!" )
				<< BUILD_LOG( vecSortedUserList.size() )
				<< BUILD_LOG( iUserIndex )
				<< END_LOG;
			return false;
		}

		const UidType iUnitUID = vecSortedUserList[iUserIndex];
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// 팀 레이팅을 구하기 위해 정리
		const bool bIsRed = ( ( ui % 2 ) == 0 );
		if( bIsRed )
		{
			mapRedTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecRedTeamResult.push_back( iUnitUID );
		}
		else
		{
			mapBlueTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecBlueTeamResult.push_back( iUnitUID );
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// low rating 유저 등록
	for( u_int ui = 0; ui < vecLowRating.size(); ++ui )
	{
		const u_int iUserIndex = vecLowRating[ui];
		if( iUserIndex < 0  ||  iUserIndex >= vecSortedUserList.size() )
		{
			START_LOG( cerr, L"정리된 유저 리스트에 존재하지 않는 index값이다! 이건 있을수 없는 일이야!" )
				<< BUILD_LOG( vecSortedUserList.size() )
				<< BUILD_LOG( iUserIndex )
				<< END_LOG;
			return false;
		}

		const UidType iUnitUID = vecSortedUserList[iUserIndex];
		KMatchUserPtr spMatchUser = GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// 팀 레이팅을 구하기 위해 정리
		const bool bIsRed = ( ( ui % 2 ) == 0 );
		if( bIsRed )
		{
			mapRedTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecRedTeamResult.push_back( iUnitUID );
		}
		else
		{
			mapBlueTeamRating.insert( std::make_pair( iUnitUID, spMatchUser->GetRating() ) );
			vecBlueTeamResult.push_back( iUnitUID );
		}
	}

	// 각 팀별 레이팅을 구하자!
	const int iRedTeamRating = SiKPvpMatchManager()->GetRatingByTeam( mapRedTeamRating );
	const int iBlueTeamRating = SiKPvpMatchManager()->GetRatingByTeam( mapBlueTeamRating );

	// 팀 레이팅 차의 절대값!
	iGepTeamRating = abs( iRedTeamRating - iBlueTeamRating );
	return true;
}

//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KMatchUserManager::GetCancelUserList( OUT std::vector< UidType >& vecCancelUserList )
{
	vecCancelUserList.clear();

	std::map< UidType, KMatchUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;;
		}

		if( mit->second->IsAccepted() == false )
		{
			vecCancelUserList.push_back( mit->first );
		}
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}

#endif SERV_PVP_NEW_SYSTEM
//}}

