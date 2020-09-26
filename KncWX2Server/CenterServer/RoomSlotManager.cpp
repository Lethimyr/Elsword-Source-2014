#include "RoomSlotManager.h"
#include "NetError.h"

FSMclassPtr KRoomSlotManager::ms_spFSM = KRoomSlotFSMPtr( new KRoomSlotFSM );

KRoomSlotManager::KRoomSlotManager()
{
    m_nRoomSlot = MAX_ROOM_MEMBER;

    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        MakeSlot( i );
    }
}

KRoomSlotManager::~KRoomSlotManager()
{
}

KRoomSlotPtr KRoomSlotManager::Get( int iSlotID ) const
{
	if( iSlotID < 0 || iSlotID >= m_nRoomSlot )
	{
		START_LOG( cerr, L"슬롯 ID 이상." )
			<< BUILD_LOG( iSlotID )
			<< BUILD_LOG( m_nRoomSlot )
			<< END_LOG;

		return KRoomSlotPtr();
	}

	return m_vecRoomSlot[iSlotID];
}

KRoomSlotPtr KRoomSlotManager::Get( int iSlotID )
{
    if( iSlotID < 0 || iSlotID >= m_nRoomSlot )
    {
        START_LOG( cerr, L"슬롯 ID 이상." )
            << BUILD_LOG( iSlotID )
            << BUILD_LOG( m_nRoomSlot )
            << END_LOG;

        return KRoomSlotPtr();
    }

    if( !m_vecRoomSlot[iSlotID] )
    {
        HandleInvalidPointer( iSlotID );
    }

    return m_vecRoomSlot[iSlotID];
}

KRoomSlotPtr KRoomSlotManager::GetByCID( UidType nCID )
{
    std::map< UidType, KRoomSlotPtr >::iterator mit;
    mit = m_mapRoomSlotByCID.find( nCID );
    if( mit == m_mapRoomSlotByCID.end() )
    {
        for( int i = 0; i < m_nRoomSlot; i++ )
        {
            KRoomSlotPtr spRoomSlot = Get( i );
            if( spRoomSlot )
            {
                if( spRoomSlot->IsOccupied() )
                {
                    if( spRoomSlot->GetCID() == nCID )
                    {
                        START_LOG( cerr, L"맵에는 없는데 실제 슬롯에는 있음." )
                            << BUILD_LOG( nCID )
                            << BUILD_LOG( i )
                            << BUILD_LOG( spRoomSlot->GetStateID() )
                            << BUILD_LOG( spRoomSlot->GetStateIDString() )
                            << END_LOG;

                        m_mapRoomSlotByCID.insert( std::make_pair( nCID, spRoomSlot ) );
                        return spRoomSlot;
                    }
                }
            }
        }

        START_LOG( cerr, L"찾고자 하는 CID 가 들어있는 슬롯이 발견되지 않음." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return KRoomSlotPtr();
    }

    if( !mit->second )
    {
        START_LOG( cerr, L"유효하지 않은 포인터." )
            << BUILD_LOG( nCID )
            << END_LOG;

        m_mapRoomSlotByCID.erase( mit );
        return KRoomSlotPtr();
    }

    if( !mit->second->IsOccupied() )
    {
        START_LOG( cerr, L"맵에는 있지만 실제 슬롯에는 없음." )
            << BUILD_LOG( nCID )
            << BUILD_LOG( mit->second->GetStateID() )
            << BUILD_LOG( mit->second->GetStateIDString() )
            << END_LOG;

        m_mapRoomSlotByCID.erase( mit );
        return KRoomSlotPtr();
    }

    return mit->second;
}

bool KRoomSlotManager::EnterSlot( char cSlotEvent, const KRoomUserInfo& kInfo, bool bConsiderTeam )
{
    KRoomSlotPtr spRoomSlot = FindEmptySlot( bConsiderTeam );
    if( !spRoomSlot )
    {
        return false;
    }

    if( !spRoomSlot->EnterSlot( cSlotEvent, kInfo, ( GetNumOccupiedSlot() == 0 ) ) )
    {
        return false;
    }

    std::map< UidType, KRoomSlotPtr >::iterator mit;
    mit = m_mapRoomSlotByCID.find( spRoomSlot->GetCID() );
    if( mit != m_mapRoomSlotByCID.end() )
    {
        START_LOG( cerr, L"CID 존재." )
            << BUILD_LOG( spRoomSlot->GetSlotID() )
            << BUILD_LOG( spRoomSlot->GetStateID() )
            << BUILD_LOG( spRoomSlot->GetStateIDString() )
            << BUILD_LOG( spRoomSlot->GetCID() )
            << END_LOG;

        m_mapRoomSlotByCID.erase( mit );
    }

    m_mapRoomSlotByCID.insert( std::make_pair( spRoomSlot->GetCID(), spRoomSlot ) );
    return true;
}

bool KRoomSlotManager::LeaveSlot( char cSlotEvent, UidType nCID )
{
    return LeaveSlot( cSlotEvent, GetByCID( nCID ) );
}

bool KRoomSlotManager::LeaveSlot( char cSlotEvent, int iSlotID )
{
    return LeaveSlot( cSlotEvent, Get( iSlotID ) );
}

bool KRoomSlotManager::LeaveSlot( char cSlotEvent, KRoomSlotPtr spRoomSlot )
{
    if( !spRoomSlot )
    {
        return false;
    }

    UidType nCID = spRoomSlot->GetCID();

    if( spRoomSlot->IsHost() && GetNumOccupiedSlot() > 1 )
    {
        if( !AppointNewHost() )
        {
            START_LOG( cerr, L"새 방장 임명 실패." )
                << END_LOG;

            return false;
        }
    }

    if( !spRoomSlot->LeaveSlot( cSlotEvent ) )
    {
        return false;
    }

    std::map< UidType, KRoomSlotPtr >::iterator mit;
    mit = m_mapRoomSlotByCID.find( nCID );
    if( mit == m_mapRoomSlotByCID.end() )
    {
        START_LOG( cerr, L"CID 존재 안함." )
            << BUILD_LOG( spRoomSlot->GetSlotID() )
            << END_LOG;
    }
    else
    {
        m_mapRoomSlotByCID.erase( mit );
    }

    return true;
}

bool KRoomSlotManager::ChangeTeam( UidType nCID )
{
    return ChangeTeam( GetByCID( nCID ) );
}

bool KRoomSlotManager::ChangeTeam( int iSlotID )
{
    return ChangeTeam( Get( iSlotID ) );
}

bool KRoomSlotManager::ChangeTeam( KRoomSlotPtr spRoomSlot )
{
    if( !spRoomSlot )
    {
        return false;
    }

    int iCurrentTeam = spRoomSlot->GetTeam();
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlotTo = Get( i );
        if( spRoomSlotTo )
        {
            if( spRoomSlotTo->IsOpened() && !spRoomSlotTo->IsOccupied() && ( spRoomSlotTo->GetTeam() != iCurrentTeam ) )
            {
                spRoomSlotTo->MoveSlot( ( char )CXSLRoom::SE_TEAM_CHANGE, spRoomSlot );
                std::map< UidType, KRoomSlotPtr >::iterator mit;
                mit = m_mapRoomSlotByCID.find( spRoomSlotTo->GetCID() );
                if( mit == m_mapRoomSlotByCID.end() )
                {
                    START_LOG( cerr, L"CID 존재 안함." )
                        << BUILD_LOG( spRoomSlotTo->GetSlotID() )
                        << BUILD_LOG( spRoomSlotTo->GetStateID() )
                        << BUILD_LOG( spRoomSlotTo->GetStateIDString() )
                        << BUILD_LOG( spRoomSlotTo->GetCID() )
                        << END_LOG;
                }
                else
                {
                    m_mapRoomSlotByCID.erase( mit );
                }
                m_mapRoomSlotByCID.insert( std::make_pair( spRoomSlotTo->GetCID(), spRoomSlotTo ) );

                if( spRoomSlot->IsOccupied() )
                {
                    mit = m_mapRoomSlotByCID.find( spRoomSlot->GetCID() );
                    if( mit == m_mapRoomSlotByCID.end() )
                    {
                        START_LOG( cerr, L"CID 존재 안함." )
                            << BUILD_LOG( spRoomSlot->GetSlotID() )
                            << BUILD_LOG( spRoomSlot->GetStateID() )
                            << BUILD_LOG( spRoomSlot->GetStateIDString() )
                            << BUILD_LOG( spRoomSlot->GetCID() )
                            << END_LOG;
                    }
                    else
                    {
                        m_mapRoomSlotByCID.erase( mit );
                    }
                    m_mapRoomSlotByCID.insert( std::make_pair( spRoomSlot->GetCID(), spRoomSlot ) );
                }

                return true;
            }
        }
    }

    return false;
}

bool KRoomSlotManager::ToggleReady( UidType nCID )
{
    return ToggleReady( GetByCID( nCID ) );
}

bool KRoomSlotManager::ToggleReady( int iSlotID )
{
    return ToggleReady( Get( iSlotID ) );
}

bool KRoomSlotManager::ToggleReady( KRoomSlotPtr spRoomSlot )
{
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->ToggleReady( ( char )CXSLRoom::SE_READY );
}

void KRoomSlotManager::ForceReady( char cSlotEvent )
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            spRoomSlot->ForceReady( cSlotEvent );
        }
    }
}

void KRoomSlotManager::ForceNotReady( char cSlotEvent )
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            spRoomSlot->ForceNotReady( cSlotEvent );
        }
    }
}

bool KRoomSlotManager::ChangeMotion( char cSlotEvent, UidType nCID, int iMotion )
{
    return ChangeMotion( cSlotEvent, GetByCID( nCID ), iMotion );
}

bool KRoomSlotManager::ChangeMotion( char cSlotEvent, int iSlotID, int iMotion )
{
    return ChangeMotion( cSlotEvent, Get( iSlotID ), iMotion );
}

bool KRoomSlotManager::ChangeMotion( char cSlotEvent, KRoomSlotPtr spRoomSlot, int iMotion )
{
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->ChangeMotion( cSlotEvent, iMotion );
}

bool KRoomSlotManager::OpenSlot( int iSlotID )
{
    return OpenSlot( Get( iSlotID ) );
}

bool KRoomSlotManager::OpenSlot( KRoomSlotPtr spRoomSlot )
{
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->OpenSlot( ( char )CXSLRoom::SE_OPEN_CLOSE );
}

bool KRoomSlotManager::CloseSlot( int iSlotID )
{
    return CloseSlot( Get( iSlotID ) );
}

bool KRoomSlotManager::CloseSlot( KRoomSlotPtr spRoomSlot )
{
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->CloseSlot( ( char )CXSLRoom::SE_OPEN_CLOSE );
}

bool KRoomSlotManager::ToggleOpenClose( int iSlotID )
{
    return ToggleOpenClose( Get( iSlotID ) );
}

bool KRoomSlotManager::ToggleOpenClose( KRoomSlotPtr spRoomSlot )
{
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->ToggleOpenClose( ( char )CXSLRoom::SE_OPEN_CLOSE );
}

bool KRoomSlotManager::SetLoadingProgress( UidType nCID, int iLoadingProgress )
{
    KRoomSlotPtr spRoomSlot = GetByCID( nCID );
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->SetLoadingProgress( iLoadingProgress );
}

void KRoomSlotManager::ResetLoadingProgress()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            spRoomSlot->ResetLoadingProgress();
        }
    }
}

bool KRoomSlotManager::SetP2PConnected( UidType nCID, bool bP2PConnected )
{
    KRoomSlotPtr spRoomSlot = GetByCID( nCID );
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->SetP2PConnected( bP2PConnected );
}

void KRoomSlotManager::ResetP2PConnected()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            spRoomSlot->ResetP2PConnected();
        }
    }
}

bool KRoomSlotManager::SetLoading( UidType nCID, bool bLoading )
{
	KRoomSlotPtr spRoomSlot = GetByCID( nCID );
	if( !spRoomSlot )
	{
		return false;
	}

	return spRoomSlot->SetLoading( bLoading );
}

void KRoomSlotManager::ResetLoading()
{
	for( int i = 0; i < m_nRoomSlot; i++ )
	{
		KRoomSlotPtr spRoomSlot = Get( i );
		if( spRoomSlot )
		{
			spRoomSlot->ResetLoading();
		}
	}
}

bool KRoomSlotManager::SetDieUnit(UidType nCID, UidType& uiMurdererUnit )
{
	KRoomSlotPtr spRoomSlot = GetByCID( nCID );
	if( !spRoomSlot )
	{
		return false;
	}

	return spRoomSlot->SetDieUnit( uiMurdererUnit );
}

bool KRoomSlotManager::GetDieUnit(UidType nCID, KEGS_USER_UNIT_DIE_NOT& kInfo )
{
	KRoomSlotPtr spRoomSlot = GetByCID( nCID );
	if( !spRoomSlot )
	{
		return false;
	}

	return spRoomSlot->GetDieUnit( kInfo );
}

void KRoomSlotManager::ResetDieUnit()
{
	for( int i = 0; i < m_nRoomSlot; i++ )
	{
		KRoomSlotPtr spRoomSlot = Get( i );
		if( spRoomSlot )
		{
			spRoomSlot->ResetDieUnit();
		}
	}
}

bool KRoomSlotManager::GetRoomUserInfo( UidType nCID, KRoomUserInfo& kInfo )
{
    KRoomSlotPtr spRoomSlot = GetByCID( nCID );
    if( !spRoomSlot )
    {
        return false;
    }

    spRoomSlot->GetRoomUserInfo( kInfo );
    return true;
}

void KRoomSlotManager::GetRoomUserInfo( std::map< UidType, KRoomUserInfo >& mapRoomUserInfo )
{
    mapRoomUserInfo.clear();

    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOccupied() )
            {
                KRoomUserInfo kInfo;
                spRoomSlot->GetRoomUserInfo( kInfo );

                mapRoomUserInfo.insert( std::make_pair( spRoomSlot->GetCID(), kInfo ) );
            }
        }
    }
}

bool KRoomSlotManager::GetRoomSlotInfo( int i, KRoomSlotInfo& kInfo )
{
    return GetRoomSlotInfo( Get( i ), kInfo );
}

bool KRoomSlotManager::GetRoomSlotInfo(  KRoomSlotPtr spRoomSlot, KRoomSlotInfo& kInfo  )
{
    if( !spRoomSlot )
    {
        return false;
    }

    spRoomSlot->GetRoomSlotInfo( kInfo );
    return true;
}

void KRoomSlotManager::GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecRoomSlotInfo, bool bResetSlotEvent )
{
    vecRoomSlotInfo.clear();

    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotInfo kInfo;
        GetRoomSlotInfo( i, kInfo);

        vecRoomSlotInfo.push_back( kInfo );
    }

    if( bResetSlotEvent )
    {
        ResetSlotEvent();
    }
}

int KRoomSlotManager::GetNumTotalSlot()
{
    return m_nRoomSlot;
}

int KRoomSlotManager::GetNumOpenedSlot()
{
    int iCount = 0;

    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOpened() )
            {
                iCount++;
            }
        }
    }

    return iCount;
}

int KRoomSlotManager::GetNumOccupiedSlot()
{
    int iCount = 0;

    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOccupied() )
            {
                iCount++;
            }
        }
    }

    return iCount;
}

bool KRoomSlotManager::GetSlotID( UidType nCID, int& iSlotID )
{
    KRoomSlotPtr spRoomSlot = GetByCID( nCID );

    if( !spRoomSlot )
    {
        return false;
    }

    iSlotID = spRoomSlot->GetSlotID();
    return true;
}

bool KRoomSlotManager::GetCID( int iSlotID, UidType& nCID )
{
    KRoomSlotPtr spRoomSlot = Get( iSlotID );

    if( !spRoomSlot )
    {
        return false;
    }

    if( !spRoomSlot->IsOccupied() )
    {
        return false;
    }

    nCID = spRoomSlot->GetCID();
    return true;
}

bool KRoomSlotManager::IsHost( UidType nCID )
{
    return IsHost( GetByCID( nCID ) );
}

bool KRoomSlotManager::IsHost( int iSlotID )
{
    return IsHost( Get( iSlotID ) );
}
bool KRoomSlotManager::IsHost( KRoomSlotPtr spRoomSlot )
{
    if( !spRoomSlot )
    {
        return false;
    }

    return spRoomSlot->IsHost();
}

bool KRoomSlotManager::IsAllPlayerReady()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOccupied() && !spRoomSlot->IsReady() )
            {
                return false;
            }
        }
    }

    return true;
}

bool KRoomSlotManager::IsAllPlayerFinishLoading()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOccupied() && !spRoomSlot->IsFinishLoading() )
            {
                return false;
            }
        }
    }

    return true;
}

bool KRoomSlotManager::IsAllPlayerP2PConnected()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOccupied() && !spRoomSlot->IsP2PConnected() )
            {
                return false;
            }
        }
    }

    return true;
}

bool KRoomSlotManager::IsAllPlayerLoading()
{
	for( int i = 0; i < m_nRoomSlot; i++ )
	{
		KRoomSlotPtr spRoomSlot = Get( i );
		if( spRoomSlot )
		{
			if( spRoomSlot->IsOccupied() && !spRoomSlot->IsLoading() )
			{
				return false;
			}
		}
	}

	return true;
}

void KRoomSlotManager::MakeSlot( int i )
{
    if( i < 0 )
    {
        START_LOG( cerr, L"슬롯 ID 이상." )
            << BUILD_LOG( i )
            << END_LOG;

        return;
    }

    KRoomSlotPtr spRoomSlot( new KRoomSlot( i ) );
    spRoomSlot->SetFSM( ms_spFSM );

    if( ( int )m_vecRoomSlot.size() == i )
    {
        m_vecRoomSlot.push_back( spRoomSlot );
    }
    else if( ( int )m_vecRoomSlot.size() > i )
    {
        START_LOG( cwarn, L"이미 있는 슬롯을 다시 생성." )
            << BUILD_LOG( i )
            << END_LOG;

        m_vecRoomSlot[i].reset();
        m_vecRoomSlot[i] = spRoomSlot;
    }
    else
    {
        START_LOG( cerr, L"슬롯 ID 이상." )
            << BUILD_LOG( i )
            << END_LOG;
    }
}

void KRoomSlotManager::HandleInvalidPointer( int i )
{
    START_LOG( cerr, L"포인터가 유효하지 않음." )
        << BUILD_LOG( i )
        << END_LOG;

    MakeSlot( i );
}

void KRoomSlotManager::ResetSlot()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            spRoomSlot->ResetSlot();
        }
    }

    m_mapRoomSlotByCID.clear();
}

void KRoomSlotManager::ResetSlotEvent()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            spRoomSlot->ResetSlotEvent();
        }
    }
}

KRoomSlotPtr KRoomSlotManager::FindEmptySlot( bool bConsiderTeam )
{
    if( bConsiderTeam )
    {
        std::map< int, int > mapTeamAndNumEmpty;
        std::map< int, KRoomSlotPtr > mapTeamAndSlotPtr;
        for( int i = 0; i < m_nRoomSlot; i++ )
        {
            KRoomSlotPtr spRoomSlot = Get( i );
            if( !spRoomSlot )
            {
                START_LOG( cerr, L"이상한 포인터." )
                    << BUILD_LOG( i )
                    << END_LOG;

                continue;
            }
            if( spRoomSlot->IsOpened() && !spRoomSlot->IsOccupied() )
            {
                if( mapTeamAndNumEmpty.find( spRoomSlot->GetTeam() ) == mapTeamAndNumEmpty.end() )
                {
                    mapTeamAndNumEmpty.insert( std::make_pair( spRoomSlot->GetTeam(), 1 ) );
                }
                else
                {
                    mapTeamAndNumEmpty[spRoomSlot->GetTeam()]++;
                }

                if( mapTeamAndSlotPtr.find( spRoomSlot->GetTeam() ) == mapTeamAndSlotPtr.end() )
                {
                    mapTeamAndSlotPtr[spRoomSlot->GetTeam()] = spRoomSlot;
                }
            }
        }

        std::map< int, int >::iterator mit1;
        std::map< int, KRoomSlotPtr >::iterator mit2;
        int iCandidateTeam;
        int iMaxEmptySlot = 0;
        for( mit1 = mapTeamAndNumEmpty.begin(); mit1 != mapTeamAndNumEmpty.end(); mit1++ )
        {
            if( mit1->second > iMaxEmptySlot )
            {
                iCandidateTeam = mit1->first;
                iMaxEmptySlot = mit1->second;
            }
        }

        if( iMaxEmptySlot > 0 )
        {
            mit2 = mapTeamAndSlotPtr.find( iCandidateTeam );
            if( mit2 == mapTeamAndSlotPtr.end() )
            {
                START_LOG( cerr, L"발생하면 안되는 상황임." )
                    << BUILD_LOG( iCandidateTeam )
                    << BUILD_LOG( iMaxEmptySlot )
                    << END_LOG;

                return FindEmptySlot( false );
            }

            return mapTeamAndSlotPtr[iCandidateTeam];
        }

        return KRoomSlotPtr();
    }
    else
    {
        for( int i = 0; i < m_nRoomSlot; i++ )
        {
            KRoomSlotPtr spRoomSlot = Get( i );
            if( !spRoomSlot )
            {
                START_LOG( cerr, L"이상한 포인터." )
                    << BUILD_LOG( i )
                    << END_LOG;

                continue;
            }
            if( spRoomSlot->IsOpened() && !spRoomSlot->IsOccupied() )
            {
                //{{ 임시 땜빵.
                spRoomSlot->SetTeam( CXSLRoom::TN_RED );
                //}}
                return spRoomSlot;
            }
        }

        return KRoomSlotPtr();
    }
}

UidType KRoomSlotManager::AppointNewHost()
{
    for( int i = 0; i < m_nRoomSlot; i++ )
    {
        KRoomSlotPtr spRoomSlot = Get( i );
        if( spRoomSlot )
        {
            if( spRoomSlot->IsOccupied() && !spRoomSlot->IsHost() )
            {
                if( spRoomSlot->SetAsHost( ( char )CXSLRoom::SE_CHANGE ) )
                {
                    return spRoomSlot->GetCID();
                }
            }
        }
    }

    START_LOG( cerr, L"호스트 변경 실패." );

    return 0;
}