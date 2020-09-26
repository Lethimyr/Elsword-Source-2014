#include "FieldUserManager.h"

#include "Profiler/Profiler.h"

KFieldUserManager::KFieldUserManager()
:
m_nMaxUser( 0 )
{
}

KFieldUserManager::~KFieldUserManager()
{
}

//{{ 2009. 3. 12  최육사	덤프
void KFieldUserManager::Dump()
{
	dbg::cout << L"mapUserList Size : " << m_mapUserList.size() << dbg::endl;
	dbg::cout << L"vecUserList Size : " << m_vecUserList.size() << dbg::endl;
		
	std::map< SectorID, FieldUserListPtr >::const_iterator mit;
	for( mit = m_mapSectorUser.begin(); mit != m_mapSectorUser.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		int iX = KSectorID::ExtractX( mit->first );
		int	iY = KSectorID::ExtractY( mit->first );
		int	iZ = KSectorID::ExtractZ( mit->first );

		dbg::cout << L"[" << iX << L":" << iY << L":" << iZ << L"] sector user list size : " << (*mit->second).size() << dbg::endl;
	}
}
//}}

void KFieldUserManager::Init( int nMaxUser )
{
	m_nMaxUser = nMaxUser;
}

KFieldUserPtr KFieldUserManager::GetUser( UidType nCID )
{
	std::map< UidType, KFieldUserPtr >::iterator mit;
	mit = m_mapUserList.find( nCID );

	if( mit == m_mapUserList.end() )
	{
		START_LOG( cwarn, L"존재하지 않는 유저." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KFieldUserPtr();
	}

	return mit->second;
}

bool KFieldUserManager::GetSectorID( UidType nCID, SectorID& iSectorID )
{
    iSectorID = 0;

	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저 찾기 실패" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	iSectorID = spFieldUser->GetMySectorID();
	return true;
}

bool KFieldUserManager::AddUser( KFieldUserPtr spFieldUser,
                                 const VECTOR3& vPos,
                                 std::vector< UidType >& vecNearbyUnitAddedThis,
                                 std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis )
{
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저 포인터 이상." )
			<< END_LOG;

		return false;
	}

    KFieldUserInfo kFieldUserInfoOfThis;
    vecNearbyUnitAddedThis.clear();
    std::vector< UidType > vecNearbyUnitRemovedThis;
    vecNearbyUnitAddedToThis.clear();
    std::vector< UidType > vecNearbyUnitRemovedFromThis;

    std::pair< std::map< UidType, KFieldUserPtr >::iterator, bool > pairResult;
    pairResult = m_mapUserList.insert( std::make_pair( spFieldUser->GetCID(), spFieldUser ) );

    if( !pairResult.second )
    {
        START_LOG( cerr, L"이미 존재하는 필드 유저임." )
            << BUILD_LOG( spFieldUser->GetCID() )
            << END_LOG;

        return false;
	}

	//{{ 2009. 1. 15  최육사	유저 리스트
	if( !AddUserList( spFieldUser ) )
	{
		START_LOG( cerr, L"유저 리스트에 이미 해당 유저가 존재하고 있다." )
			<< BUILD_LOG( spFieldUser->GetCID() )
			<< END_LOG;

		// 기존의 유저를 리스트에서 지운다.
		RemoveUserList( spFieldUser->GetCID() );

		// 다시 리스트에 추가
		if( !AddUserList( spFieldUser ) )
		{
			START_LOG( cerr, L"리스트에 중복유저 지우고 추가되어야 하는데?" )
				<< BUILD_LOG( spFieldUser->GetCID() )
				<< END_LOG;
			return false;
		}
	}
	//}}

    LIF( MoveUser( spFieldUser, vPos, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) );
    LIF( vecNearbyUnitRemovedThis.size() == 0 );
    LIF( vecNearbyUnitRemovedFromThis.size() == 0 );

    return true;
}

bool KFieldUserManager::RemoveUser( UidType nCID,
                                    std::vector< UidType >& vecNearbyUnitRemovedThis )
{
    KFieldUserInfo kFieldUserInfoOfThis;
    std::vector< UidType > vecNearbyUnitAddedThis;
    vecNearbyUnitRemovedThis.clear();
    std::vector< KFieldUserInfo > vecNearbyUnitAddedToThis;
    std::vector< UidType > vecNearbyUnitRemovedFromThis;

	//{{ 2009. 1. 15  최육사	유저 리스트
	if( !RemoveUserList( nCID ) )
	{
		START_LOG( cerr, L"유저 리스트에서 삭제하려는데 유저가 없다?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
	}
	//}}

	std::map< UidType, KFieldUserPtr >::iterator mit;
	mit = m_mapUserList.find( nCID );
	if( mit == m_mapUserList.end() )
	{
		START_LOG( cerr, L"존재하지 않는 필드 유저임." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

    if( !mit->second )
    {
        START_LOG( cerr, L"필드 유저 포인터 이상" )
            << BUILD_LOG( nCID )
            << END_LOG;

        m_mapUserList.erase( mit );
        return false;
    }

    LIF( MoveUser( mit->second, INVALID_SECTOR, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) );
    LIF( vecNearbyUnitAddedThis.size() == 0 );
    LIF( vecNearbyUnitAddedToThis.size() == 0 );

    m_mapUserList.erase( mit );
	return true;
}

bool KFieldUserManager::MoveUser( KFieldUserPtr spFieldUser,
                                  SectorID iDestSectorID,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis )
{
    if( !spFieldUser )
    {
        START_LOG( cerr, L"유저 포인터 이상" )
            << BUILD_LOG( iDestSectorID )
            << END_LOG;

        return false;
    }

    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();

    UidType iUnitUID = spFieldUser->GetCID();

    // 섹터가 달라지지 않았으면 이동 완료.
    SectorID iSrcSectorID = spFieldUser->GetMySectorID();

    if( iDestSectorID == iSrcSectorID )
    {
        spFieldUser->GetFieldUserInfo( kFieldUserInfoOfThis );
        return true;
    }

    SectorsPtr spSrcSectors = GetAdjacentSectors( iSrcSectorID ); _JIF( spSrcSectors, return false );
    SectorsPtr spDestSectors = GetAdjacentSectors( iDestSectorID ); _JIF( spDestSectors, return false );

    // 원래 있었던 섹터에서 나가자
    if( !RemoveUserFromSector( iSrcSectorID, iUnitUID ) )
    {
        START_LOG( cerr, L"섹터에서 나가기 실패." )
            << BUILD_LOG( iUnitUID )
            << BUILD_LOG( iDestSectorID )
            << END_LOG;

        return false;
    }

    // 해당 섹터에 들어가자
    if( !AddUserToSector( iDestSectorID, spFieldUser ) )
    {
        START_LOG( cerr, L"섹터에 유저 추가 실패" )
            << BUILD_LOG( iDestSectorID )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }
    spFieldUser->SetMySectorID( iDestSectorID );

    // 주변 인접 섹터 설정
    spFieldUser->SetAdjacentSectors( spDestSectors );

    // 내 필드유저 정보 얻기
    spFieldUser->GetFieldUserInfo( kFieldUserInfoOfThis );

	// 변화된 인접 섹터를 계산한다
    std::vector< SectorID > vecSrcRemainder;
    std::vector< SectorID > vecDestRemainder;
    KSectorID::FindRemainder( *spSrcSectors, *spDestSectors, vecSrcRemainder, vecDestRemainder );

	// 주변섹터중에서 유저의 이탈을 알려야할 유저들의 recv user list를 검사하여 이탈처리 한다.
	std::map< UidType, KFieldUserPtr > mapSrcRemainderUnit;
    std::map< UidType, KFieldUserPtr > mapDestRemainderUnit;
    std::map< UidType, KFieldUserPtr > mapCandidateOther;
    std::map< UidType, KFieldUserPtr >::iterator mitOtherUnit;
    std::map< UidType, KFieldUserPtr >::iterator mitCandidateOther;

    GetSectorUnitList( vecSrcRemainder, mapSrcRemainderUnit );

	//////////////////////////////////////////////////////////////////////////
	// 이탈 섹터의 유저에 대한 처리

	for( mitOtherUnit = mapSrcRemainderUnit.begin(); mitOtherUnit != mapSrcRemainderUnit.end(); ++mitOtherUnit )
	{
        if( !mitOtherUnit->second )
        {
            START_LOG( cerr, L"포인터 이상" )
                << BUILD_LOG( mitOtherUnit->first )
                << END_LOG;

            continue;
        }

		// 내가 이 유저의 리스트에 있으면..
		bool bDeletedFromOther = mitOtherUnit->second->DelEventRecvUser( iUnitUID );
		if( bDeletedFromOther )
		{
			// 나의 send 리스트에서 이 유저를 뺀다
			if( !spFieldUser->DelEventSendUser( mitOtherUnit->first ) )
			{
				START_LOG( cerr, L"센드 리스트 이상" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
			}
		}

        // 내가 이 유저의 리스트에 있었거나 파티원일 때
        if( bDeletedFromOther || mitOtherUnit->second->IsParty( iUnitUID ) || mitOtherUnit->second->IsFriend( iUnitUID ) )
        {
			// 이 유저에게 나를 delete 하도록 알려줘야한다.
			vecNearbyUnitRemovedThis.push_back( mitOtherUnit->first );
        }

        // 이 유저가 내 리스트에 있으면..
		bool bDeletedFromThis = spFieldUser->DelEventRecvUser( mitOtherUnit->first );
		if( bDeletedFromThis )
		{
			// 이 유저의 send 리스트에서 날 뺀다.
			if( !mitOtherUnit->second->DelEventSendUser( iUnitUID ) )
			{
				START_LOG( cerr, L"센드 리스트 이상" )
					<< BUILD_LOG( mitOtherUnit->first )
					<< END_LOG;
			}
		}

		// 이 유저가 내 리스트에 있었거나 파티원일 때
        if( bDeletedFromThis || spFieldUser->IsParty( mitOtherUnit->first ) || spFieldUser->IsFriend( mitOtherUnit->first ) )
        {
            // 나에게 이 유저를 delete 하도록 알려줘야한다.
			vecNearbyUnitRemovedFromThis.push_back( mitOtherUnit->first );
        }
	}

	// 주변섹터중에서 유저의 진입을 알려야할 유저들의 recv user list를 검사하여 입장처리 한다
    GetSectorUnitList( vecDestRemainder, mapDestRemainderUnit );

	//////////////////////////////////////////////////////////////////////////
	// 진입 섹터의 유저에 대한 처리

	for( mitOtherUnit = mapDestRemainderUnit.begin(); mitOtherUnit != mapDestRemainderUnit.end(); ++mitOtherUnit )
	{
        if( !mitOtherUnit->second )
        {
            START_LOG( cerr, L"유닛 포인터 이상." )
                << BUILD_LOG( mitOtherUnit->first )
                << END_LOG;

            continue;
        }

		// 이 유저의 리스트에 내가 들어있으면..
		bool bAddFromOther = mitOtherUnit->second->AddEventRecvUser( iUnitUID );
		if( bAddFromOther )
		{
			// 나의 send list에 이 유저를 추가해야 한다.
			if( !spFieldUser->AddEventSendUser( mitOtherUnit->first ) )
			{
				START_LOG( cerr, L"센드 리스트 이상" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
			}
		}

        // 이 유저의 리스트에 내가 들어가거나 파티원일 경우
        if( bAddFromOther || mitOtherUnit->second->IsParty( iUnitUID ) || mitOtherUnit->second->IsFriend( iUnitUID ) )
        {
			// 나를 추가하라고 알린다.
			vecNearbyUnitAddedThis.push_back( mitOtherUnit->first );            
        }

		// 내 리스트에 이 유저가 들어있을경우..
		bool bAddFromThis = spFieldUser->AddEventRecvUser( mitOtherUnit->first );
		if( bAddFromThis )
		{
			// 이 유저의 send 리스트에 날 추가한다.
			if( !mitOtherUnit->second->AddEventSendUser( iUnitUID ) )
			{
				START_LOG( cerr, L"센드 리스트 이상" )
					<< BUILD_LOG( mitOtherUnit->first )
					<< END_LOG;
			}
		}

        // 내 리스트에 이 유저가 들어가거나 파티원일 경우의 처리.
        if( bAddFromThis || spFieldUser->IsParty( mitOtherUnit->first ) || spFieldUser->IsFriend( mitOtherUnit->first ) )
        {
            // 나에게 이 유저를 추가 하도록 알려줘야한다.
            KFieldUserInfo kFieldUserInfo;
            mitOtherUnit->second->GetFieldUserInfo( kFieldUserInfo );
            vecNearbyUnitAddedToThis.push_back( kFieldUserInfo );            
        }
    }

	return true;
}

bool KFieldUserManager::MoveUser( UidType iUnitUID,
                                  SectorID iDestSectorID,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis )
{
    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();

    KFieldUserPtr spFieldUser = GetUser( iUnitUID );
    if( !spFieldUser )
    {
        START_LOG( cerr, L"유저 검색 실패." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    return MoveUser( spFieldUser, iDestSectorID, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis );
}

bool KFieldUserManager::MoveUser( KFieldUserPtr spFieldUser,
                                  const VECTOR3& vPos,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis)
{
    if( !spFieldUser )
    {
        START_LOG( cerr, L"유저 포인터 이상" )
            << END_LOG;

        return false;
    }

    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();

    if( !MoveUser( spFieldUser, GetDestSectorID( vPos ), kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) )
    {
        START_LOG( cerr, L"유저 이동 실패" )
            << BUILD_LOG( spFieldUser->GetCID() )
            << END_LOG;

        return false;
    }

    spFieldUser->UpdateUserPos( vPos );
    return true;
}

bool KFieldUserManager::MoveUser( UidType iUnitUID,
                                  const VECTOR3& vPos,
                                  KFieldUserInfo& kFieldUserInfoOfThis,
                                  std::vector< UidType >& vecNearbyUnitAddedThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedThis,
                                  std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis,
                                  std::vector< UidType >& vecNearbyUnitRemovedFromThis )
{
    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitRemovedThis.clear();
    vecNearbyUnitAddedToThis.clear();
    vecNearbyUnitRemovedFromThis.clear();
    
    SectorID iSectorID = GetDestSectorID( vPos );

    if( !MoveUser( iUnitUID, iSectorID, kFieldUserInfoOfThis, vecNearbyUnitAddedThis, vecNearbyUnitRemovedThis, vecNearbyUnitAddedToThis, vecNearbyUnitRemovedFromThis ) )
    {
        START_LOG( cerr, L"유저 이동 실패" )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    if( !UpdateUserPos( iUnitUID, vPos ) )
    {
        START_LOG( cerr, L"유저 위치 업데이트 실패." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return false;
    }

    return true;
}

bool KFieldUserManager::EnterField( const KERM_JOIN_FIELD_REQ& kJoinUser,
                                    std::vector< UidType >& vecNearbyUnitAddedThis,
                                    std::vector< KFieldUserInfo >& vecNearbyUnitAddedToThis )
{
    vecNearbyUnitAddedThis.clear();
    vecNearbyUnitAddedToThis.clear();

	KFieldUserPtr spFieldUser = MakeUser( kJoinUser.m_kFieldUserInfo, kJoinUser.m_setPartyUIDList, kJoinUser.m_setFriendUIDList, kJoinUser.m_kFieldOption );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"필드 유저 생성 실패." )
			<< BUILD_LOG( ( int )GetNumMember() )
			<< END_LOG;

		return false;
	}

	if( !AddUser( spFieldUser, kJoinUser.m_kFieldUserInfo.m_vPos, vecNearbyUnitAddedThis, vecNearbyUnitAddedToThis ) )
	{
		START_LOG( cerr, L"필드 유저 추가 실패." )
			<< BUILD_LOG( spFieldUser->GetCID() )
			<< END_LOG;

		return false;
	}

	return true;
}

bool KFieldUserManager::LeaveField( UidType nCID, std::vector< UidType >& vecNearbyUnitRemovedThis )
{
    vecNearbyUnitRemovedThis.clear();

	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"필드 나가기중 유저 찾기 실패!" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	if( !RemoveUser( nCID, vecNearbyUnitRemovedThis ) )
    {
        START_LOG( cerr, L"필드 나가기 실패" )
            << BUILD_LOG( nCID )
            << END_LOG;
    
        return false;
    }

    return true;
}

//{{ 2009. 1. 15  최육사	유저 리스트
bool KFieldUserManager::AddUserList( KFieldUserPtr spFieldUser )
{	
	std::vector< KFieldUserPtr >::const_iterator vit;
	for( vit = m_vecUserList.begin(); vit != m_vecUserList.end(); ++vit )
	{
		KFieldUserPtr spCheckFieldUser = *vit;
		if( spCheckFieldUser == NULL )
		{
			START_LOG( cerr, L"필드유저 포인터가 이상합니다." )
				<< BUILD_LOG( spFieldUser->GetCID() )
				<< END_LOG;
			continue;
		}

		if( spCheckFieldUser->GetCID() == spFieldUser->GetCID() )
		{
			START_LOG( cerr, L"이미 존재하는 필드 유저임." )
				<< BUILD_LOG( spFieldUser->GetCID() )
				<< END_LOG;
			return false;
		}
	}

	m_vecUserList.push_back( spFieldUser );
	return true;
}

bool KFieldUserManager::RemoveUserList( UidType nCID )
{
	std::vector< KFieldUserPtr >::iterator vit;
	for( vit = m_vecUserList.begin(); vit != m_vecUserList.end(); ++vit )
	{
		KFieldUserPtr spCheckFieldUser = *vit;
		if( spCheckFieldUser == NULL )
		{
			START_LOG( cerr, L"필드유저 포인터가 이상합니다." )
				<< BUILD_LOG( nCID )
				<< END_LOG;
			continue;
		}

		if( spCheckFieldUser->GetCID() == nCID )
			break;
	}

	if( m_vecUserList.end() == vit )
	{
		START_LOG( cerr, L"유저리스트에서 유저를 삭제하려는데 유저가 없습니다." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	m_vecUserList.erase( vit );
	return true;
}
//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
bool KFieldUserManager::GetFieldUserList( UINT& uiViewPage, UINT& uiTotalPage, std::vector< KCommunityUserInfo >& vecUserList, bool bEdView /*false*/ )
#else
bool KFieldUserManager::GetFieldUserList( UINT& uiViewPage, UINT& uiTotalPage, std::vector< KCommunityUserInfo >& vecUserList )
#endif SERV_ED_MONITORING_IN_GAME
//}}
{
	vecUserList.clear();

	_JIF( uiViewPage > 0, return false; );

	if( m_vecUserList.empty() )
	{
		uiViewPage = 1;
		uiTotalPage = 1;
		return true;
	}
	
	uiTotalPage = m_vecUserList.size() / PAGE_PER_USER_COUNT;
	if( ( m_vecUserList.size() % PAGE_PER_USER_COUNT ) > 0 )
	{
		++uiTotalPage;
	}

	u_int uiBeginIndex = ( uiViewPage * PAGE_PER_USER_COUNT ) - PAGE_PER_USER_COUNT;
	if( m_vecUserList.size() <= uiBeginIndex )
	{
        uiViewPage = uiTotalPage; // 마지막 페이지
		uiBeginIndex = ( uiViewPage * PAGE_PER_USER_COUNT ) - PAGE_PER_USER_COUNT;

		if( m_vecUserList.size() <= uiBeginIndex )
		{
			START_LOG( cerr, L"일어날 수 없는 에러!" )
				<< BUILD_LOG( uiViewPage )
				<< BUILD_LOG( uiTotalPage )
				<< BUILD_LOG( uiBeginIndex )
				<< BUILD_LOG( m_vecUserList.size() )
				<< END_LOG;
			return false;
		}
	}
	
	std::vector< KFieldUserPtr >::reverse_iterator vit = m_vecUserList.rbegin();
	vit += uiBeginIndex;

	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	KCommunityUserInfo kTemp;
#endif SERV_ED_MONITORING_IN_GAME
	//}}

	while( vit != m_vecUserList.rend() )
	{
		KFieldUserPtr spFieldUser = *vit;
		if( spFieldUser == NULL )
		{
			START_LOG( cerr, L"필드 유저 포인터가 이상합니다." )
				<< END_LOG;			
		}
		else
		{
			//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
			kTemp = spFieldUser->GetCommunityUserInfo();
			if( bEdView == false )
			{
				kTemp.m_iED = 0;
			}
            vecUserList.push_back( kTemp );
#else
			vecUserList.push_back( spFieldUser->GetCommunityUserInfo() );
#endif SERV_ED_MONITORING_IN_GAME
			//}}
			
			if( vecUserList.size() >= PAGE_PER_USER_COUNT )
				break;
		}

		++vit;
	}

	return true;
}
//}}

//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE

bool KFieldUserManager::SetUserEquippedTitle( IN UidType nCID, IN int iEquippedTitleID )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetUserEquippedTitle( iEquippedTitleID );
	return true;
}

#else

bool KFieldUserManager::SetUserEquippedTitle( IN UidType nCID, IN short sEquippedTitleID )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetUserEquippedTitle( sEquippedTitleID );
	return true;
}

#endif SERV_TITLE_DATA_SIZE
//}}

bool KFieldUserManager::SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetUserEquipItem( vecInventorySlotInfo );
    return true;
}

bool KFieldUserManager::SetFieldOption( IN UidType nCID, IN const KFieldOption& kFieldOption )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"필드 옵션을 업데이트 하려고 하는데 필드유저가 없다?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->SetFieldOption( kFieldOption );
	return true;
}

bool KFieldUserManager::SetParty( IN UidType nCID, IN UidType iPartyUID, IN const std::set< UidType >& setParty, OUT std::vector< KFieldUserInfo >& vecAdded, OUT std::vector< UidType >& vecRemoved )
{
    vecAdded.clear();
    vecRemoved.clear();

    KFieldUserPtr spFieldUser = GetUser( nCID );
    if( !spFieldUser )
    {
        START_LOG( cerr, L"유저 찾기 실패" )
            << BUILD_LOG( nCID )
			<< BUILD_LOG( setParty.size() )
            << END_LOG;
        return false;
    }

	//////////////////////////////////////////////////////////////////////////	
    std::set< UidType > setPartyRev = setParty;

	// 파티 유저 체크
	if( !setPartyRev.empty() )
	{
        if( setPartyRev.find( nCID ) == setPartyRev.end() )
		{
			START_LOG( cerr, L"파티 리스트를 업데이트 하려고 하는데 내가 파티리스트에 없다?" )
				<< BUILD_LOG( nCID )
				<< BUILD_LOG( setPartyRev.size() )
				<< END_LOG;
		}
	}

	// 자기 자신은 지우자.
	setPartyRev.erase( nCID );

    std::set< UidType > setCurrentParty;
    spFieldUser->GetParty( setCurrentParty );

    std::set< UidType >::iterator sitParty = setPartyRev.begin();
    std::set< UidType >::iterator sitCurrentParty = setCurrentParty.begin();

    while( sitParty != setPartyRev.end() || sitCurrentParty != setCurrentParty.end() )
    {
        if( sitParty == setPartyRev.end() )
        {
            if( !spFieldUser->IsEventRecvUser( *sitCurrentParty )  &&  !spFieldUser->IsFriend( *sitCurrentParty ) )
            {
                vecRemoved.push_back( *sitCurrentParty );
            }

            sitCurrentParty++;
            continue;
        }
        
        if( sitCurrentParty == setCurrentParty.end() )
        {
            if( !spFieldUser->IsEventRecvUser( *sitParty )  &&  !spFieldUser->IsFriend( *sitParty ) )
            {
                KFieldUserPtr spPartyUser = GetUser( *sitParty );
                if( !spPartyUser )
                {
					// [참고] 유저를 못찾는 경우가 있음 : 파티를 맺는 순간 서로 다른 필드에 파티신청자와 파티장이 위치하고있으면 유저를 못찾는다!

					// 다른 필드에 있으므로 필드유저정보를 보낼필요가 없음, 따라서 로그는 cwarn처리함.

                    START_LOG( cwarn, L"유저 찾기 실패" )
                        << BUILD_LOG( *sitParty )
                        << END_LOG;

                    sitParty++;
                    continue;
                }
                KFieldUserInfo kFieldUserInfo;
                spPartyUser->GetFieldUserInfo( kFieldUserInfo );
                vecAdded.push_back( kFieldUserInfo );
            }

            sitParty++;
            continue;
        }

        if( *sitParty < *sitCurrentParty )
        {
            if( !spFieldUser->IsEventRecvUser( *sitParty )  &&  !spFieldUser->IsFriend( *sitParty ) )
            {
                KFieldUserPtr spPartyUser = GetUser( *sitParty );
                if( !spPartyUser )
                {
					// [참고] 유저를 못찾는 경우가 있음 : 파티를 맺는 순간 서로 다른 필드에 파티신청자와 파티장이 위치하고있으면 유저를 못찾는다!

					// 다른 필드에 있으므로 필드유저정보를 보낼필요가 없음, 따라서 로그는 cwarn처리함.

                    START_LOG( cwarn, L"유저 찾기 실패" )
                        << BUILD_LOG( *sitParty )
                        << END_LOG;

                    sitParty++;
                    continue;
                }
                KFieldUserInfo kFieldUserInfo;
                spPartyUser->GetFieldUserInfo( kFieldUserInfo );
                vecAdded.push_back( kFieldUserInfo );
            }

            sitParty++;
            continue;
        }
        else if( *sitParty > *sitCurrentParty )
        {
            if( !spFieldUser->IsEventRecvUser( *sitCurrentParty )  &&  !spFieldUser->IsFriend( *sitCurrentParty ) )
            {
                vecRemoved.push_back( *sitCurrentParty );
            }

            sitCurrentParty++;
            continue;
        }
        else
        {
            sitParty++;
            sitCurrentParty++;
            continue;
        }
    }

    spFieldUser->SetParty( setPartyRev );

	//{{ 2009. 4. 1  최육사		파티UID 업데이트
	spFieldUser->SetPartyUID( iPartyUID );
	//}}

    return true;
}

//{{ 2009. 4. 9  최육사		친구리스트
bool KFieldUserManager::SetFriend( IN UidType nCID, IN const std::set< UidType >& setFriend, OUT std::vector< KFieldUserInfo >& vecAdded, OUT std::vector< UidType >& vecRemoved )
{
	vecAdded.clear();
	vecRemoved.clear();

	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저 찾기 실패" )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( setFriend.size() )
			<< END_LOG;
		return false;
	}

	//////////////////////////////////////////////////////////////////////////	
	std::set< UidType > setFriendRev = setFriend;

	// 친구 유저 체크
	if( !setFriendRev.empty() )
	{
		if( setFriendRev.find( nCID ) != setFriendRev.end() )
		{
			START_LOG( cerr, L"친구 리스트를 업데이트 하려고 하는데 내가 친구리스트에 있다?" )
				<< BUILD_LOG( nCID )
				<< BUILD_LOG( setFriendRev.size() )
				<< END_LOG;
		}
	}

	// 자기 자신은 지우자.
	setFriendRev.erase( nCID );

	std::set< UidType > setCurrentFriend;
	spFieldUser->GetFriend( setCurrentFriend );

	std::set< UidType >::iterator sitFriend = setFriendRev.begin();
	std::set< UidType >::iterator sitCurrentFriend = setCurrentFriend.begin();

	while( sitFriend != setFriendRev.end() || sitCurrentFriend != setCurrentFriend.end() )
	{
		if( sitFriend == setFriendRev.end() )
		{
			if( !spFieldUser->IsEventRecvUser( *sitCurrentFriend )  &&  !spFieldUser->IsParty( *sitCurrentFriend ) )
			{
				vecRemoved.push_back( *sitCurrentFriend );
			}

			sitCurrentFriend++;
			continue;
		}

		if( sitCurrentFriend == setCurrentFriend.end() )
		{
			if( !spFieldUser->IsEventRecvUser( *sitFriend )  &&  !spFieldUser->IsParty( *sitFriend ) )
			{
				KFieldUserPtr spPartyUser = GetUser( *sitFriend );
				if( !spPartyUser )
				{
					// [참고] 유저를 못찾는 경우가 있음 : 파티를 맺는 순간 서로 다른 필드에 파티신청자와 파티장이 위치하고있으면 유저를 못찾는다!

					// 다른 필드에 있으므로 필드유저정보를 보낼필요가 없음, 따라서 로그는 cwarn처리함.

					START_LOG( cwarn, L"유저 찾기 실패" )
						<< BUILD_LOG( *sitFriend )
						<< END_LOG;

					sitFriend++;
					continue;
				}
				KFieldUserInfo kFieldUserInfo;
				spPartyUser->GetFieldUserInfo( kFieldUserInfo );
				vecAdded.push_back( kFieldUserInfo );
			}

			sitFriend++;
			continue;
		}

		if( *sitFriend < *sitCurrentFriend )
		{
			if( !spFieldUser->IsEventRecvUser( *sitFriend )  &&  !spFieldUser->IsParty( *sitFriend ) )
			{
				KFieldUserPtr spPartyUser = GetUser( *sitFriend );
				if( !spPartyUser )
				{
					// [참고] 유저를 못찾는 경우가 있음 : 파티를 맺는 순간 서로 다른 필드에 파티신청자와 파티장이 위치하고있으면 유저를 못찾는다!

					// 다른 필드에 있으므로 필드유저정보를 보낼필요가 없음, 따라서 로그는 cwarn처리함.

					START_LOG( cwarn, L"유저 찾기 실패" )
						<< BUILD_LOG( *sitFriend )
						<< END_LOG;

					sitFriend++;
					continue;
				}
				KFieldUserInfo kFieldUserInfo;
				spPartyUser->GetFieldUserInfo( kFieldUserInfo );
				vecAdded.push_back( kFieldUserInfo );
			}

			sitFriend++;
			continue;
		}
		else if( *sitFriend > *sitCurrentFriend )
		{
			if( !spFieldUser->IsEventRecvUser( *sitCurrentFriend )  &&  !spFieldUser->IsParty( *sitCurrentFriend ) )
			{
				vecRemoved.push_back( *sitCurrentFriend );
			}

			sitCurrentFriend++;
			continue;
		}
		else
		{
			sitFriend++;
			sitCurrentFriend++;
			continue;
		}
	}

	spFieldUser->SetFriend( setFriendRev );
	return true;
}
//}}

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
bool KFieldUserManager::SetFieldUserInfo( IN const KFieldUserInfo& kInfo )
{
	KFieldUserPtr spFieldUser = GetUser( kInfo.m_iUnitUID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"변경된 유저 정보를 업데이트 하려고 하는데 필드유저가 없다?" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	spFieldUser->SetFieldUserInfo( kInfo );
	return true;
}
//}}

//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

bool KFieldUserManager::SetPetInfo( IN UidType nCID, IN const std::vector< KPetInfo >& vecPetInfo )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"현재 위치 정보를 업데이트 하려고 하는데 필드유저가 없다?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->SetPetInfo( vecPetInfo );
	return true;
}

bool KFieldUserManager::SetPetInfo( IN UidType nCID, IN const KPetInfo& kPetInfo )
{
	std::vector< KPetInfo > vecPetInfo;
	vecPetInfo.push_back( kPetInfo );
    return SetPetInfo( nCID, vecPetInfo );
}

#endif SERV_PET_SYSTEM
//}}

bool KFieldUserManager::UpdateUserPos( UidType nCID, const VECTOR3& UserPos )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"현재 위치 정보를 업데이트 하려고 하는데 필드유저가 없다?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->UpdateUserPos( UserPos );
	return true;
}

bool KFieldUserManager::AddUserToSector( SectorID iSectorID, KFieldUserPtr spUser )
{
    if( iSectorID == INVALID_SECTOR )
    {
        return true;
    }

    if( !spUser )
    {
        START_LOG( cerr, L"필드 유저 포인터 이상." )
            << BUILD_LOG( iSectorID )
            << END_LOG;

        return false;
    }

    UidType nCID = spUser->GetCID();
    if( nCID <= 0 )
    {
        START_LOG( cerr, L"캐릭터 UID이상." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    std::map< SectorID, FieldUserListPtr >::iterator mitSector = m_mapSectorUser.find( iSectorID );
    if( mitSector == m_mapSectorUser.end() )
    {
        // 섹터가 없으면 집어넣자
        FieldUserListPtr spFieldUserList( new std::map< UidType, KFieldUserPtr > ); _JIF( spFieldUserList, return false );
        spFieldUserList->insert( std::make_pair( nCID, spUser ) );
        m_mapSectorUser.insert( std::make_pair( iSectorID, spFieldUserList ) );
    }
    else
    {
        if( !mitSector->second )
        {
            START_LOG( cerr, L"유저 리스트 포인터 이상." )
                << BUILD_LOG( iSectorID )
                << END_LOG;

            return false;
        }

        std::pair< std::map< UidType, KFieldUserPtr >::iterator, bool > pairResult;
        pairResult = mitSector->second->insert( std::make_pair( nCID, spUser ) );

        if( !pairResult.second )
        {
            START_LOG( cerr, L"이미 존재하는 유저임." )
                << BUILD_LOG( nCID )
                << END_LOG;

            return false;
        }
    }

    return true;
}

bool KFieldUserManager::RemoveUserFromSector( SectorID iSectorID, UidType nCID )
{
    if( iSectorID == INVALID_SECTOR )
    {
        // 소속 섹터가 없었음
        return true;
    }

    std::map< SectorID, FieldUserListPtr >::iterator mitSector = m_mapSectorUser.find( iSectorID );
    if( mitSector == m_mapSectorUser.end() )
    {
        START_LOG( cerr, L"없는 섹터가 있네..?" )
            << BUILD_LOG( iSectorID )
            << END_LOG;
        return false;
    }

    if( !mitSector->second )
    {
        START_LOG( cerr, L"유저 리스트 맵 포인터 이상." )
            << BUILD_LOG( iSectorID )
            << END_LOG;

        return false;
    }

    // 해당 섹터에서 유저 삭제
    std::map< UidType, KFieldUserPtr >::iterator mitUser = mitSector->second->find( nCID );
    if( mitUser == mitSector->second->end() )
    {
        START_LOG( cerr, L"유저 검색 실패." )
            << BUILD_LOG( iSectorID )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    mitSector->second->erase( mitUser );
    return true;
}

void KFieldUserManager::GetSectorUnitList( const std::vector< SectorID >& vecSectorList, std::map< UidType, KFieldUserPtr >& mapUnitList )
{
    mapUnitList.clear();

    std::vector< SectorID >::const_iterator vit;
    for( vit = vecSectorList.begin(); vit != vecSectorList.end(); ++vit )
    {
        GetAddedSectorUnitList( *vit, mapUnitList );
	}
}

SectorsPtr KFieldUserManager::GetAdjacentSectors( SectorID iSector )
{
    std::map< SectorID, SectorsPtr >::iterator mit = m_mapAdjacentSector.find( iSector );
    if( mit == m_mapAdjacentSector.end() )
    {
        SectorsPtr spVector( new std::vector< SectorID > ); _JIF( spVector, return SectorsPtr() );
        KSectorID::ExtractAdjacentSector( iSector, m_kTileInfo.m_NumX, m_kTileInfo.m_NumY, m_kTileInfo.m_NumZ, *spVector );
        m_mapAdjacentSector.insert( std::make_pair( iSector, spVector ) );
        return spVector;
    }
    else
    {
        return mit->second;
    }
}

bool KFieldUserManager::GetBroadcastDestination( UidType nCID, std::vector< UidType >& vecDestination )
{
	KFieldUserPtr spFieldUser = GetUser( nCID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저 찾기 실패" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spFieldUser->GetBroadcastDestination( vecDestination );
	return true;
}

KFieldUserPtr KFieldUserManager::MakeUser( const KFieldUserInfo& kFieldUserInfo, const std::set< UidType >& setPartyList, const std::set< UidType >& setFriendList, const KFieldOption& kFieldOption )
{
	if( GetNumMember() >= GetNumMaxUser() )
	{
		START_LOG( cwarn, L"필드 한계인원을 넘었는데 더 생성할려고 하네.?ㅁ?" )
			<< BUILD_LOG( kFieldUserInfo.m_iUnitUID )
			<< BUILD_LOG( kFieldUserInfo.m_wstrNickName )
			<< END_LOG;

		return KFieldUserPtr();
	}

	KFieldUserPtr spFieldUser( new KFieldUser );
	if( !spFieldUser )
	{
		return KFieldUserPtr();
	}

	// 1. 필드유저정보 등록
	spFieldUser->SetFieldUserInfo( kFieldUserInfo );
	
	// 2. 파티유저와 친구유저인지 검사
    std::set< UidType > setPartyRev = setPartyList;
	std::set< UidType > setFriendRev = setFriendList;

	// 3. 파티유저와 친구유저 체크
	if( !setPartyRev.empty() )
	{
		if( setPartyRev.find( kFieldUserInfo.m_iUnitUID ) == setPartyRev.end() )
		{
			START_LOG( cerr, L"파티 리스트를 업데이트 하려고 하는데 내가 파티리스트에 없다?" )
				<< BUILD_LOG( kFieldUserInfo.m_iUnitUID )
				<< BUILD_LOG( setPartyRev.size() )
				<< END_LOG;
		}
	}

	if( !setFriendRev.empty() )
	{
		if( setFriendRev.find( kFieldUserInfo.m_iUnitUID ) != setFriendRev.end() )
		{
			START_LOG( cerr, L"친구리스트에 나 자신이 포함되어있네?" )
				<< BUILD_LOG( kFieldUserInfo.m_iUnitUID )
				<< BUILD_LOG( setFriendRev.size() )
				<< END_LOG;
		}
	}

	// 4. 자기 자신은 지우자.
	setPartyRev.erase( kFieldUserInfo.m_iUnitUID );
	setFriendRev.erase( kFieldUserInfo.m_iUnitUID );
		
	// 5. 파티리스트와 친구리스트 등록
    spFieldUser->SetParty( setPartyRev );
	spFieldUser->SetFriend( setFriendRev );

	// 6. 필드 옵션 등록
    spFieldUser->SetFieldOption( kFieldOption );

	return spFieldUser;
}

void KFieldUserManager::GetAddedSectorUnitList( SectorID iSectorID, std::map< UidType, KFieldUserPtr >& mapUnitList  )
{
    std::map< SectorID, FieldUserListPtr >::iterator mitSector = m_mapSectorUser.find( iSectorID );
    if( mitSector == m_mapSectorUser.end() )
    {
		return;
	}

    if( !mitSector->second )
    {
        START_LOG( cerr, L"유저 리스트 포인터 이상" )
            << BUILD_LOG( iSectorID )
            << END_LOG;

        return;
    }

    // 혹시 루프 안돌리고 한 번에 insert 하는 방법은 없을까??
    std::map< UidType, KFieldUserPtr >::iterator mitUser;
    for( mitUser = mitSector->second->begin(); mitUser != mitSector->second->end(); mitUser++ )
    {
        std::pair< std::map< UidType, KFieldUserPtr >::iterator, bool > pairResult;
        pairResult = mapUnitList.insert( std::make_pair( mitUser->first, mitUser->second ) );
        LIF( pairResult.second );
    }
}

SectorID KFieldUserManager::GetDestSectorID( const VECTOR3& vPos )
{	
	int iIndexX = 0;
	if( m_kTileInfo.m_SizeX > 0 )
	{
		iIndexX = ( static_cast< short >( vPos.x ) - m_kMapInfo.m_MinX ) / m_kTileInfo.m_SizeX;
		if( iIndexX < 0 ) iIndexX = 0;
		else if( iIndexX >= m_kTileInfo.m_NumX ) iIndexX = m_kTileInfo.m_NumX - 1;
	}

	int iIndexY = 0;
	if( m_kTileInfo.m_SizeY > 0 ) 
	{
		iIndexY = ( static_cast< short >( vPos.y ) - m_kMapInfo.m_MinY ) / m_kTileInfo.m_SizeY;
		if( iIndexY < 0 ) iIndexY = 0;
		else if( iIndexY >= m_kTileInfo.m_NumY ) iIndexY = m_kTileInfo.m_NumY - 1;
	}

	int iIndexZ = 0;
	if( m_kTileInfo.m_SizeZ > 0 )
	{
		iIndexZ = ( static_cast< short >( vPos.z ) - m_kMapInfo.m_MinZ ) / m_kTileInfo.m_SizeZ;
		if( iIndexZ < 0 ) iIndexZ = 0;
		else if( iIndexZ >= m_kTileInfo.m_NumZ ) iIndexZ = m_kTileInfo.m_NumZ - 1;
	}
	
	SectorID iDestSectorID;
	KSectorID::SetIndex( iDestSectorID, iIndexX, iIndexY, iIndexZ );

	return iDestSectorID;
}

//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
void KFieldUserManager::RefreshUser_ED( std::map< UidType, int >& mapUnit_ED )
{
	std::map< UidType, int >::iterator mit;

	std::vector< KFieldUserPtr >::const_iterator vit;
	for( vit = m_vecUserList.begin(); vit != m_vecUserList.end(); ++vit )
	{
		KFieldUserPtr spCheckFieldUser = *vit;
		if( spCheckFieldUser == NULL )
		{
			START_LOG( cerr, L"필드유저 포인터가 이상합니다." )
				<< BUILD_LOG( spCheckFieldUser->GetCID() )
				<< END_LOG;
			continue;
		}
		
		mit = mapUnit_ED.find( spCheckFieldUser->GetCID() );
		if( mit != mapUnit_ED.end() )
		{
			if( mit->second >= 0 )
				spCheckFieldUser->UpdateUserED( mit->second );
		}
	}
}
#endif SERV_ED_MONITORING_IN_GAME
//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
bool KFieldUserManager::SetRidingPetInfo( IN const UidType iUnitUID, IN const UidType iRidingPetUID, IN const USHORT usRidingPetID )
{
	KFieldUserPtr spFieldUser = GetUser( iUnitUID );
	if( spFieldUser == NULL )
	{
		START_LOG( cerr, L"현재 라이딩 펫 정보를 업데이트 하려고 하는데 필드 유저가 없다?" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spFieldUser->SetRidingPetInfo( iRidingPetUID, usRidingPetID );
	return true;
}
#endif	// SERV_RIDING_PET_SYSTM