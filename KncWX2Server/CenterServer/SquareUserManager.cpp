#include ".\squareusermanager.h"

KSquareUserManager::KSquareUserManager(void)
:
m_dwMaxUser( 0 ),
m_bIsCheckZU( false )
{
}

KSquareUserManager::~KSquareUserManager(void)
{
	m_mapSquareUser.clear();
}

void KSquareUserManager::Init( int nMaxUser )
{
	m_dwMaxUser	= (DWORD)nMaxUser;
}

void KSquareUserManager::Reset()
{
	m_mapSquareUser.clear();
}

KSquareUserPtr KSquareUserManager::GetUser( UidType nCID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( nCID );

	if( mit == m_mapSquareUser.end() )
	{
		START_LOG( cwarn, L"존재하지 않는 유저." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KSquareUserPtr();
	}

	return mit->second;
}

bool KSquareUserManager::AddUser( KSquareUserPtr spSquareUser )
{
	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저 포인터 이상." )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( spSquareUser->GetCID() );

	if( mit != m_mapSquareUser.end() )
	{
		START_LOG( cerr, L"이미 존재하는 광장 유저임." )
			<< BUILD_LOG( spSquareUser->GetCID() )
			<< END_LOG;

		//m_mapSquareUser.erase( mit );
		return false;
	}

	m_mapSquareUser.insert( std::make_pair( spSquareUser->GetCID(), spSquareUser ) );
	return true;
}

bool KSquareUserManager::DeleteUser( UidType nCID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( nCID );

	if( mit == m_mapSquareUser.end() )
	{
		START_LOG( cerr, L"존재하지 않는 광장 유저임." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return true;
	}

	m_mapSquareUser.erase( mit );
	return true;
}

int KSquareUserManager::DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector<UidType>& vecDelUserUID )
{
	std::map<UidType, KSquareUserPtr>::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
	{
		KSquareUserInfo kInfo;
		mit->second->GetSquareUserInfo( kInfo );

		if( kInfo.m_iGSUID == nGsUID )
		{
			//삭제될 user의 uid를 담는다.
			vecDelUserUID.push_back( kInfo.m_iUnitUID );
		}
	}

	return static_cast<int>(vecDelUserUID.size());
}

bool KSquareUserManager::EnterSquare( const KSquareUserInfo& kInfo )
{
	KSquareUserPtr spSquareUser = MakeUser( kInfo );
	if( !spSquareUser )
	{
		//{{ 2009. 1. 28  최육사	에러로그 줄여보자!
		if( m_mapSquareUser.size() == m_dwMaxUser )
		{
			START_LOG( cwarn, L"광장이 유저가 꽉차서 유저 생성 실패." )
				<< BUILD_LOG( ( int )m_mapSquareUser.size() )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"광장 유저 생성 실패." )
				<< BUILD_LOG( ( int )m_mapSquareUser.size() )
				<< END_LOG;
		}
		//}}

		return false;
	}

	if( AddUser( spSquareUser ) == false )
	{
		START_LOG( cerr, L"광장 유저 추가 실패." )
			<< BUILD_LOG( spSquareUser->GetCID() )
			<< END_LOG;

		return false;
	}

	return true;
}

bool KSquareUserManager::LeaveSquare( UidType nCID )
{
	//만약에 포인터가 안지워지는 경우를 생각해..쩝..
	if( GetPersonalShopState( nCID ) > CXSLSquareUnit::PSS_NONE )
	{
		KSquarePersonalShopInfo kInfo;
		
		kInfo.m_cUpdateType = KSquarePersonalShopInfo::UT_DATA_CLEAR;
		//kInfo.m_cPersonalShopState = CXSLSquareUnit::PSS_NONE;
		SetPersonalShopData( nCID, kInfo );
	}

	return DeleteUser( nCID );
}

bool KSquareUserManager::SetSyncPos( UidType nCID, VECTOR3& vSyncPos )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );
	if( !spSquareUser )
	{
		START_LOG( cwarn, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetSyncPos( vSyncPos );

    return true;
}

void KSquareUserManager::GetUserList( std::map< UidType, std::set< UidType > >& mapUidList_ )
{
    mapUidList_.clear();

    std::map< UidType, KSquareUserPtr >::iterator mit;
    for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"룸 유저 포인터 이상." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            continue;
        }

        // operator[]()의 의도적인 사용. 데이터가 없을 경우 insert되는 것을 인정한다.
        mapUidList_[mit->second->GetGSUID()].insert( mit->second->GetCID() );
    }
}

void KSquareUserManager::GetUserInfoList( std::vector<KSquareUserInfo>& vecUserInfo )
{
	KSquareUserInfo kInfo;

	std::map< UidType, KSquareUserPtr >::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"SQUARE USER 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		mit->second->GetSquareUserInfo( kInfo );
		vecUserInfo.push_back( kInfo );
	}
}

bool KSquareUserManager::GetSortSyncPos( UidType nCID, std::vector<KSquareUserPtr>& vecUserList )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;
	std::vector<KSquareUserPtr> vecSquareUser;

	//map에 있는 리스트를 vector에 담는다.
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); ++mit )
	{
		vecSquareUser.push_back( mit->second );
	}

	KSquareUserPtr spMyUser = GetUser( nCID );
	if( spMyUser == NULL )
	{
		START_LOG( cerr, L"SQUARE USER 포인터 이상.!" )
			<< BUILD_LOG( mit->first )
			<< END_LOG;

		return false;
	}

	//자신의 위치를 기준으로 가까운 순으로 정렬한다.
	KSquareUserPtr spSquareUser;
	for( int nCnt1 = 0; nCnt1 < (int)vecSquareUser.size(); ++nCnt1 )
	{
		bool bSwapped = false;

		for( int nCnt2 = 0; nCnt2 < (int)vecSquareUser.size() - nCnt1 - 1; ++nCnt2 )
		{
			if( GetDistance( spMyUser->GetSyncPos(), vecSquareUser[nCnt2]->GetSyncPos() ) > GetDistance( spMyUser->GetSyncPos(), vecSquareUser[nCnt2+1]->GetSyncPos() ) )
			{
				spSquareUser			= vecSquareUser[nCnt2];
				vecSquareUser[nCnt2]	= vecSquareUser[nCnt2+1];
				vecSquareUser[nCnt2+1]	= spSquareUser;
				bSwapped = true;
			}
		}

		if( bSwapped == false )
		{
			break;
		}
	}

	//거리 1500 미터
	//최대인원 20명
	//조건으로 리스트를 담아 넘겨준다.
	vecUserList.clear();
	for( int i = 0; i < (int)vecSquareUser.size(); ++i )
	{
		if( GetDistance( spMyUser->GetSyncPos(), vecSquareUser[i]->GetSyncPos() ) <= 1500.0f &&
			(int)vecUserList.size() <= 20 )
		{
			vecUserList.push_back( vecSquareUser[i] );
		}
		else
		{
			break;
		}
	}

	return true;
}

bool KSquareUserManager::GetSquareUserGS( UidType nCID, UidType& nGSUID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit;

	mit = m_mapSquareUser.find( nCID );

	if( mit != m_mapSquareUser.end() )
	{
		if( mit->second != NULL )
		{
			nGSUID = mit->second->GetGSUID();

			return true;
		}
		else
		{
			START_LOG( cerr, L"광장 유저 포인터 이상.!" )
				<< BUILD_LOG( nCID )
				<< END_LOG
				;
		}
	}
	else
	{
		START_LOG( cwarn, L"찾는 광장유저가 없음.!" )
			<< BUILD_LOG( nCID )
			<< END_LOG
			;
	}

	return false;
}


KSquareUserPtr KSquareUserManager::MakeUser( const KSquareUserInfo& kInfo )
{
	if( m_mapSquareUser.size() >= m_dwMaxUser )
	{
		START_LOG( cwarn, L"광장 한계인원을 넘었는데 더 생성할려고 하네.?ㅁ?" )
			<< BUILD_LOG( kInfo.m_iGSUID )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( kInfo.m_wstrNickName )
			<< END_LOG;

		return KSquareUserPtr();
	}

	KSquareUserPtr spSquareUser( new KSquareUser );
	if( !spSquareUser )
	{
		return KSquareUserPtr();
	}

	spSquareUser->SetSquareUserInfo( kInfo );
	return spSquareUser;
}

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
bool KSquareUserManager::SetSquareUserInfo( IN const KSquareUserInfo& kInfo )
{
	KSquareUserPtr spSquareUser = GetUser( kInfo.m_iUnitUID );
	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetSquareUserInfo( kInfo );
	return true;
}
//}}

void KSquareUserManager::SetUserEquippedTitle( IN UidType nCID, IN short sEquippedTitleID )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return;
	}

	spSquareUser->SetUserEquippedTitle( sEquippedTitleID );
}

void KSquareUserManager::SetUserEquipItem( IN UidType nCID, IN std::vector< KInventoryItemInfo >& vecInventorySlotInfo )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return;
	}

	spSquareUser->SetUserEquipItem( vecInventorySlotInfo );
}

bool KSquareUserManager::SetUnitPartyInfo( UidType nCID, bool bIsParty )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetUnitPartyInfo( bIsParty );

	return true;
}

bool KSquareUserManager::IsCreatePersonalShop()
{
	int iCountPS = 0;

	std::map< UidType, KSquareUserPtr >::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"SQUARE USER 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		if( mit->second->GetPersonalShopState() > CXSLSquareUnit::PSS_NONE )
			++iCountPS;
	}

    if( iCountPS >= static_cast<int>(GetNumMaxUser()/2) )
	//if( iCountPS >= 33 ) //08.02.12 hoons.현재 광장의 모든 인원을 열어두지 않아 임시로 50%를 잡는다.
		return false;

	return true;
}

char KSquareUserManager::GetPersonalShopState( UidType nCID )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return CXSLSquareUnit::PSS_NONE;
	}

	return spSquareUser->GetPersonalShopState();
}

UidType KSquareUserManager::GetPersonalShopUID( UidType nCID )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return 0;
	}

	return spSquareUser->GetPersonalShopUID();
}

bool KSquareUserManager::SetPersonalShopData( IN UidType nCID, IN const KSquarePersonalShopInfo& kInfo, OUT KEGS_UPDATE_SQUARE_PERSONAL_SHOP_INFO_NOT& kNot )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );

	if( !spSquareUser )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spSquareUser->SetPersonalShopData( kInfo, kNot );

	return true;
}

//{{ 2010. 8. 2	최육사	광장 싱크 최적화
#ifdef SERV_SQUARE_SYNC

bool KSquareUserManager::CompareSquareSyncPacket( IN UidType nCID, IN const KEGS_SQUARE_UNIT_SYNC_DATA_REQ& kInfo )
{
	KSquareUserPtr spSquareUser = GetUser( nCID );
	if( spSquareUser == NULL )
	{
		START_LOG( cerr, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spSquareUser->CompareSquareSyncPacket( kInfo );
}

#endif SERV_SQUARE_SYNC
//}}

void KSquareUserManager::ZU_CheckStart()
{
	START_LOG( clog, L"광장 유령유저 체크시작.!" );

	//유령유저 체크 시작하기전 시간 초기화
	//std::map< UidType, KSquareUserPtr >::iterator mit;
	//for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
	//{
	//	if( !mit->second )
	//	{
	//		START_LOG( cerr, L"광장 유저 포인터 이상." )
	//			<< BUILD_LOG( mit->first )
	//			<< END_LOG;
	//		continue;
	//	}

	//	mit->second->ZU_Refresh();
	//}

	//유령유저 체크를 시작
	m_bIsCheckZU = true;
}

void KSquareUserManager::ZU_CheckEnd()
{
	START_LOG( clog, L"광장 유령유저 체크종료.!" );

	//유령유저 체크 중지.
	m_bIsCheckZU = false;
}

bool KSquareUserManager::ZU_Tick( std::vector<UidType>& vecZombieUnit )
{
	if( m_bIsCheckZU == false )
	{
		return false;
	}
	//else
	//{
	//	//유저가 없으면 체크를 꺼준다.
	//	if( m_mapSquareUser.empty() == true )
	//	{
	//		m_bIsCheckZU = false;
	//		return false;
	//	}
	//}

	vecZombieUnit.clear();

	std::map< UidType, KSquareUserPtr >::iterator mit;
	for( mit = m_mapSquareUser.begin(); mit != m_mapSquareUser.end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"광장 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		if( mit->second->ZU_Tick() == true )
		{
			vecZombieUnit.push_back( mit->second->GetCID() );
		}
	}

	//유령유저가 없다면..
	if( vecZombieUnit.empty() == true )
		return false;

	return true;
}

void KSquareUserManager::ZU_Refresh( UidType nCID, u_short usEventID )
{
	if( m_bIsCheckZU == false )
		return;

	//유저가 방에 없는데 오는 패킷의 경우 이곳에 추가한다.
	switch( usEventID )
	{
	case ERM_OPEN_SQUARE_NOT:
	case ERM_JOIN_SQUARE_REQ:
	case ERM_CHECK_INVALID_USER_NOT:
	case ERM_SQUARE_LIST_INFO_NOT:
		return;
	}

	START_LOG( clog, L"유령유저 Refresh.!" )
		<< BUILD_LOG( nCID );

	//이벤트가 날아온 유저를 찾는다.
	std::map< UidType, KSquareUserPtr >::iterator mit;
	mit = m_mapSquareUser.find( nCID );

	if( mit != m_mapSquareUser.end() )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"광장유저 포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return;
		}

		//이상없으면 체크 시간을 갱신한다.
		mit->second->ZU_Refresh();
	}
	else
	{
		START_LOG( cerr, L"시간을 갱신할 유저가 없음.!" )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
int KSquareUserManager::GetUserED( IN UidType nCID )
{
	std::map< UidType, KSquareUserPtr >::iterator mit = m_mapSquareUser.find( nCID );
	if( mit == m_mapSquareUser.end() )
	{
		return 0;
	}

	if( !mit->second )
	{
		START_LOG( cerr, L"광장유저 포인터 이상.!" )
			<< BUILD_LOG( mit->first )
			<< END_LOG;

		return 0;
	}

    return mit->second->GetED();
}
#endif SERV_ED_MONITORING_IN_GAME
//}}




