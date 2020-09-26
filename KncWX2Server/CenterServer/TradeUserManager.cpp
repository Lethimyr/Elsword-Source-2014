#include ".\TradeUserManager.h"

KTradeUserManager::KTradeUserManager(void)
{
}

KTradeUserManager::~KTradeUserManager(void)
{
	Reset();
}

void KTradeUserManager::Init( int nMaxUser )
{
	m_dwMaxUser	= ( DWORD )nMaxUser;
}

void KTradeUserManager::Reset()
{
	m_mapTradeUser.clear();
}

KTradeUserPtr KTradeUserManager::GetUser( UidType nCID )
{
	std::map< UidType, KTradeUserPtr >::iterator mit;
	mit = m_mapTradeUser.find( nCID );

	if( mit == m_mapTradeUser.end() )
	{
		START_LOG( cwarn, L"존재하지 않는 유저." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KTradeUserPtr();
	}

	return mit->second;
}

bool KTradeUserManager::AddUser( KTradeUserPtr spTradeUser )
{
	if( !spTradeUser )
	{
		START_LOG( cerr, L"유저 포인터 이상." )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KTradeUserPtr >::iterator mit;
	mit = m_mapTradeUser.find( spTradeUser->GetCID() );

	if( mit != m_mapTradeUser.end() )
	{
		START_LOG( cerr, L"이미 존재하는 개인 거래 유저임." )
			<< BUILD_LOG( spTradeUser->GetCID() )
			<< END_LOG;

		//m_mapTradeUser.erase( mit );
		return false;
	}

	m_mapTradeUser.insert( std::make_pair( spTradeUser->GetCID(), spTradeUser ) );
	return true;
}

bool KTradeUserManager::DeleteUser( UidType nCID )
{
	std::map< UidType, KTradeUserPtr >::iterator mit;
	mit = m_mapTradeUser.find( nCID );

	if( mit == m_mapTradeUser.end() )
	{
		START_LOG( cerr, L"존재하지 않는 개인 거래 유저임." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return true;
	}

	m_mapTradeUser.erase( mit );
	return true;
}

int KTradeUserManager::DeleteUserByGsUID( IN UidType nGsUID, OUT std::vector< UidType >& vecDelUserUID )
{
	int iCount = 0;

	std::map< UidType, KTradeUserPtr >::iterator mit;
	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); mit++ )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"개인 거래 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}

		if( mit->second->GetGSUID() == nGsUID )
		{
			//삭제될 user의 uid를 담는다.
			vecDelUserUID.push_back( mit->second->GetCID() );
			iCount++;
		}
	}

	return iCount;
}

bool KTradeUserManager::EnterTrade( const KTradeUserInfo& kInfo )
{
	KTradeUserPtr spTradeUser = MakeUser( kInfo );
	if( !spTradeUser )
	{
		START_LOG( cerr, L"개인 거래 유저 생성 실패." )
			<< BUILD_LOG( ( int )m_mapTradeUser.size() )
			<< END_LOG;

		return false;
	}

	if( !AddUser( spTradeUser ) )
	{
		START_LOG( cerr, L"파티 유저 추가 실패." )
			<< BUILD_LOG( spTradeUser->GetCID() )
			<< END_LOG;

		return false;
	}

	return true;
}

bool KTradeUserManager::LeaveTrade( UidType nCID )
{
	KTradeUserPtr spTradeUser = GetUser( nCID );
	if( !spTradeUser )
	{
		START_LOG( cwarn, L"유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return true;
	}

	return DeleteUser( nCID );
}

bool KTradeUserManager::GetTradeUserGS( UidType nCID, UidType& nGSUID )
{
	std::map< UidType, KTradeUserPtr >::iterator mit;

	mit = m_mapTradeUser.find( nCID );

	if( mit != m_mapTradeUser.end() )
	{
		if( mit->second != NULL )
		{
			nGSUID = mit->second->GetGSUID();

			return true;
		}
		else
		{
			START_LOG( cerr, L"개인 거래 유저 포인터 이상.!" )
				<< BUILD_LOG( nCID )
				<< END_LOG;
		}

	}
	else
	{
		START_LOG( cwarn, L"찾는 개인 거래 유저가 없음.!" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
	}

	return false;
}

void KTradeUserManager::GetUserList( OUT std::map< UidType, std::set< UidType > >& mapUidList_ )
{
	mapUidList_.clear();

	std::map< UidType, KTradeUserPtr >::iterator mit;
	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); mit++ )
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

void KTradeUserManager::GetUserInfoList( OUT std::vector< KTradeUserInfo >& vecTradeUserInfo )
{
	std::map< UidType, KTradeUserPtr >::iterator mit;

	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); ++mit )
	{
		KTradeUserInfo kTradeUserInfo;

		if( mit->second != NULL )
		{
			mit->second->GetTradeUserInfo( kTradeUserInfo );

			vecTradeUserInfo.push_back( kTradeUserInfo );
		}
		else
		{
			START_LOG( cerr, L"개인 거래 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			continue;
		}
	}
}

bool KTradeUserManager::GetOtherCID( UidType nCID, UidType& nOtherCID )
{
	if( GetNumMember() != 2 )
	{
		START_LOG( cerr, L"인원 수 이상." )
			<< BUILD_LOG( GetNumMember() )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KTradeUserPtr >::iterator mit = m_mapTradeUser.find( nCID );
	if( mit == m_mapTradeUser.end() )
	{
		return false;
	}

	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); mit++ )
	{
		if( mit->first != nCID )
		{
			nOtherCID = mit->first;
			return true;
		}
	}

	START_LOG( cerr, L"절대 이 곳에는 오지 말아야 한다." )
		<< END_LOG;

	return false;
}

bool KTradeUserManager::GetOtherTradeUserInfo( UidType nCID, KTradeUserInfo& kOther )
{
	if( GetNumMember() != 2 )
	{
		START_LOG( cerr, L"인원 수 이상." )
			<< BUILD_LOG( GetNumMember() )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KTradeUserPtr >::iterator mit = m_mapTradeUser.find( nCID );
	if( mit == m_mapTradeUser.end() )
	{
		return false;
	}

	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); mit++ )
	{
		if( mit->first != nCID )
		{
			if( !mit->second )
			{
				START_LOG( cerr, L"포인터 이상." )
					<< BUILD_LOG( mit->first )
					<< END_LOG;

				continue;
			}

			mit->second->GetTradeUserInfo( kOther );
			return true;
		}
	}

	return false;
}

KTradeUserPtr KTradeUserManager::GetOtherUser( IN UidType nCID )
{
    if( GetNumMember() != 2 )
    {
        START_LOG( cerr, L"인원 수 이상." )
            << BUILD_LOG( GetNumMember() )
            << END_LOG;

        return KTradeUserPtr();
    }

    std::map< UidType, KTradeUserPtr >::iterator mit = m_mapTradeUser.find( nCID );
    if( mit == m_mapTradeUser.end() )
    {
        return KTradeUserPtr();
    }

    for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); mit++ )
    {
        if( mit->first != nCID )
        {
            if( !mit->second )
            {
                START_LOG( cerr, L"포인터 이상." )
                    << BUILD_LOG( mit->first )
                    << END_LOG;

                continue;
            }

            return mit->second;
        }
    }

    return KTradeUserPtr();
}

bool KTradeUserManager::GetTradeItem( IN UidType nCID, IN UidType iItemUID, OUT KInventoryItemInfo& kInfo )
{
	kInfo.m_iItemUID = 0;

	KTradeUserPtr spUser = GetUser( nCID );
	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return spUser->GetTradeItemInfo( iItemUID, kInfo );
}

bool KTradeUserManager::GetTradeItem( IN UidType nCID, OUT std::vector< KInventoryItemInfo >& vecTradeItemInfo )
{
	vecTradeItemInfo.clear();

	KTradeUserPtr spUser = GetUser( nCID );
	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}
	spUser->GetTradeItemInfo( vecTradeItemInfo );

	return true;
}

bool KTradeUserManager::GetTradeED( UidType nCID, int& iED )
{
	iED = 0;

	KTradeUserPtr spUser = GetUser( nCID );
	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}
	iED = spUser->GetTradeED();

	return true;
}

bool KTradeUserManager::UpdateTradeItem( UidType nCID, KInventoryItemInfo& kInfo )
{
	KTradeUserPtr spUser = GetUser( nCID );

	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

    KTradeUserPtr spOtherUser = GetOtherUser( nCID );
    if( !spOtherUser )
    {
        START_LOG( cerr, L"상대방 유저를 찾을 수 없음." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

	if( spUser->InsertTradeItemInfo( kInfo ) )
    {
        spOtherUser->CancelOffer();
        return true;
    }
    else
    {
        return false;
    }
}

bool KTradeUserManager::UpdateTradeED( UidType nCID, int iED )
{
	KTradeUserPtr spUser = GetUser( nCID );

	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

    KTradeUserPtr spOtherUser = GetOtherUser( nCID );
    if( !spOtherUser )
    {
        START_LOG( cerr, L"상대방 유저를 찾을 수 없음." )
            << BUILD_LOG( nCID )
            << END_LOG;

        return false;
    }

    if( spUser->SetTradeED( iED ) )
    {
        spOtherUser->CancelOffer();
        return true;
    }
    else
    {
        return false;
    }
}

bool KTradeUserManager::RemoveTradeItem( IN UidType nCID, IN UidType iItemUID )
{
	KTradeUserPtr spUser = GetUser( nCID );
	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	//{{ 2009. 4. 20  최육사	빼도 캔슬되게
	KTradeUserPtr spOtherUser = GetOtherUser( nCID );
	if( !spOtherUser )
	{
		START_LOG( cerr, L"상대방 유저를 찾을 수 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	if( spUser->RemoveTradeItemInfo( iItemUID ) )
	{
		spOtherUser->CancelOffer();
		return true;
	}
	else
	{
		return false;
	}
	//}}
}

bool KTradeUserManager::Offer( UidType nCID )
{
	KTradeUserPtr spUser = GetUser( nCID );

	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spUser->Offer();
	return true;
}

bool KTradeUserManager::Approve( UidType nCID )
{
	KTradeUserPtr spUser = GetUser( nCID );

	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spUser->Approve();
	return true;
}

bool KTradeUserManager::Confirm( IN UidType nCID, IN const KERM_CONFIRM_TRADE_ACK& kInfo )
{
	KTradeUserPtr spUser = GetUser( nCID );

	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spUser->Confirm( kInfo );
	return true;
}

bool KTradeUserManager::IsOffered()
{
	std::map< UidType, KTradeUserPtr >::iterator mit;

	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"개인 거래 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return false;
		}
		else
		{
			if( !mit->second->IsOffered() )
			{
				return false;
			}
		}
	}

	return true;
}

bool KTradeUserManager::IsApproved()
{
	std::map< UidType, KTradeUserPtr >::iterator mit;

	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"개인 거래 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return false;
		}
		else
		{
			if( !mit->second->IsApproved() )
			{
				return false;
			}
		}
	}

	return true;
}

bool KTradeUserManager::IsConfirmed()
{
	std::map< UidType, KTradeUserPtr >::iterator mit;

	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"개인 거래 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return false;
		}
		else
		{
			if( !mit->second->IsConfirmed() )
			{
				return false;
			}
		}
	}

	return true;
}

void KTradeUserManager::GetOfferState( OUT std::map< UidType, bool >& mapOfferState )
{
    mapOfferState.clear();

    std::map< UidType, KTradeUserPtr >::iterator mit;

    for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); mit++ )
    {
        if( mit->second == NULL )
        {
            START_LOG( cerr, L"개인 거래 유저 포인터 이상." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            continue;
        }
        else
        {
            // 키 중복 검사를 할 필요가 없다.
            mapOfferState.insert( std::make_pair( mit->first, mit->second->IsOffered() ) );
        }
    }
}

bool KTradeUserManager::IsThereNothingToTrade()
{
    std::map< UidType, KTradeUserPtr >::iterator mit;

    for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); mit++ )
    {
        if( !mit->second )
        {
            START_LOG( cerr, L"거래 유저 포인터 이상." )
                << BUILD_LOG( mit->first )
                << END_LOG;

            continue;
        }        
        
        if( !mit->second->IsTradeItemEmpty() )
        {
            return false;
        }

        if( mit->second->GetTradeED() > 0 )
        {
            return false;
        }
    }

    return true;
}

//{{ 2009. 2. 10  최육사	개인거래 버그예방
bool KTradeUserManager::GetUpdateTemp( IN UidType nCID, OUT KERM_CONFIRM_TRADE_NOT& kInfo )
{
	KTradeUserPtr spUser = GetUser( nCID );

	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spUser->GetUpdateTemp( kInfo );
	return true;
}
//}}

KTradeUserPtr KTradeUserManager::MakeUser( const KTradeUserInfo& kInfo )
{
	if( m_mapTradeUser.size() >= m_dwMaxUser )
	{
		START_LOG( cwarn, L"개인 거래 룸이 가득 찼음." )
			<< BUILD_LOG( kInfo.m_iGSUID )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( kInfo.m_wstrNickName )
			<< BUILD_LOG( m_dwMaxUser )
			<< END_LOG;

		return KTradeUserPtr();
	}

	KTradeUserPtr spTradeUser( new KTradeUser() );
	if( !spTradeUser )
	{
		return KTradeUserPtr();
	}

	spTradeUser->SetTradeUserInfo( kInfo );
	return spTradeUser;
}

bool KTradeUserManager::IsCanInsertTradeItem( IN UidType nCID )
{
	KTradeUserPtr spUser = GetUser( nCID );

	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}
	
	return spUser->IsCanInsertTradeItem();
}

//{{ 2009. 4. 7  최육사		아이템업데이트 가능 상태인지 검사
bool KTradeUserManager::CheckStateUpdateTradeItem( IN UidType nCID )
{
	KTradeUserPtr spUser = GetUser( nCID );
	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	if( spUser->IsOffered() )
	{
		START_LOG( cerr, L"offered 상태인데 아이템 등록 하려한다?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	if( spUser->IsApproved() )
	{
		START_LOG( cerr, L"approved 상태인데 아이템 등록 하려한다?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	return true;
}
//}}

//{{ 2010. 05. 22  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION

bool KTradeUserManager::IsAllTradeUsersSameServerGroup( IN UidType iUnitUID )
{
	const UidType iServerGroupID = KncUid::ExtractServerGroupID( iUnitUID );

	std::map< UidType, KTradeUserPtr >::iterator mit;
	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"거래 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// 체크하려는 유저의 UnitUID와 다른 서버군 유저가 있는지 보자!
		if( KncUid::ExtractServerGroupID( mit->second->GetCID() ) != iServerGroupID )
		{
			return false;
		}
	}

	return true;
}

#endif SERV_INTEGRATION
//}}

//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
bool KTradeUserManager::IsDBUpdateReady() const
{
	std::map< UidType, KTradeUserPtr >::const_iterator mit;
	for( mit = m_mapTradeUser.begin(); mit != m_mapTradeUser.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"개인 거래 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			return false;
		}
		else
		{
			if( !mit->second->IsDBUpdateReady() )
			{
				return false;
			}
		}
	}

	return true;    
}

bool KTradeUserManager::SetDBUpdateInfo( IN const UidType iUnitUID, IN const KERM_DB_UPDATE_TRADE_RESULT_NOT& kInfo )
{
	KTradeUserPtr spUser = GetUser( iUnitUID );
	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spUser->SetDBUpdateInfo( kInfo );
	return true;
}

bool KTradeUserManager::GetDBUpdateInfo( IN const UidType iUnitUID, IN OUT std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >& vecDBUpdateInfo )
{
	KTradeUserPtr spUser = GetUser( iUnitUID );
	if( !spUser )
	{
		START_LOG( cerr, L"유저가 존재하지 않음." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	return spUser->GetDBUpdateInfo( vecDBUpdateInfo );
}
#endif SERV_TRADE_DDOS_DEFENCE
//}}


