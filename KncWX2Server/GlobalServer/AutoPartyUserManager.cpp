#include "AutoParty.h"
#include "AutoPartyUserManager.h"

#include "Enum/Enum.h"
#include "NetError.h"
#include "AutoPartyUserFSM.h"
#include "CompareLimitList.h"


//{{ 2012. 02. 08	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


FSMclassPtr KAutoPartyUserManager::ms_spUserFSM = KAutoPartyUserFSMPtr( new KAutoPartyUserFSM );


KAutoPartyUserManager::KAutoPartyUserManager()
{
}

KAutoPartyUserManager::~KAutoPartyUserManager()
{
}

void KAutoPartyUserManager::Init( IN const KAutoPartyConditionInfo& kInfo )
{
	m_kConditionInfo	 = kInfo;
}

bool KAutoPartyUserManager::Enter( IN const KAutoPartyUserInfo& kInfo )
{
    if( IsExist( kInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"이미 입장해 있는 유저입니다!" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	if( (int)( m_mapMember.size() + 1 )  > GetLimitPlayerCount() )
	{
		START_LOG( cerr, L"최대 입장 인원 제한에 걸렸습니다." )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( GetLimitPlayerCount() )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;
		return false;
	}

	KAutoPartyUserPtr spAutoPartyUser( new KAutoPartyUser( kInfo ) );
	spAutoPartyUser->SetFSM( ms_spUserFSM );
	m_mapMember.insert( std::make_pair( kInfo.m_iUnitUID, spAutoPartyUser ) );
	return true;
}

bool KAutoPartyUserManager::Enter( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList )
{
	const int iAutoPartyReqMemberCount = static_cast<int>(vecAutoPartyUserList.size());

	if( (int)( m_mapMember.size() + iAutoPartyReqMemberCount )  > GetLimitPlayerCount() )
	{
		START_LOG( cerr, L"최대 입장 인원 제한에 걸렸습니다." )
			<< BUILD_LOG( GetLimitPlayerCount() )
			<< BUILD_LOG( m_mapMember.size() )
			<< BUILD_LOG( iAutoPartyReqMemberCount )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KAutoPartyUserInfo, kInfo, vecAutoPartyUserList )
	{
		if( Enter( kInfo ) == false )
		{
			START_LOG( cerr, L"자동 파티 입장 처리 실패!" )
				<< END_LOG;
		}
	}

	return true;
}

void KAutoPartyUserManager::Reset()
{
}

KAutoPartyUserPtr KAutoPartyUserManager::GetUser( IN const UidType iUnitUID )
{
    std::map< UidType, KAutoPartyUserPtr >::iterator mit;
    mit = m_mapMember.find( iUnitUID );
    if( mit == m_mapMember.end() )
    {
        START_LOG( cerr, L"존재하지 않는 유저." )
            << BUILD_LOG( iUnitUID )
            << END_LOG;

        return KAutoPartyUserPtr();
    }

    return mit->second;
}

void KAutoPartyUserManager::GetAutoPartyUserList( OUT std::vector< UidType >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		vecUserList.push_back( mit->first );
	}
}

bool KAutoPartyUserManager::DeleteUser( IN const UidType iUnitUID )
{
    std::map< UidType, KAutoPartyUserPtr >::iterator mit;
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

int KAutoPartyUserManager::GetNumMember() const
{
    return static_cast<int>(m_mapMember.size());
}

bool KAutoPartyUserManager::Accept( IN const UidType iUnitUID )
{
	KAutoPartyUserPtr spAutoPartyUser = GetUser( iUnitUID );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}
	
	return spAutoPartyUser->Accept();
}

bool KAutoPartyUserManager::IsAllPlayersAccept()
{
	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
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

void KAutoPartyUserManager::ClearAllPlayerRegroupPartyInfo()
{
	std::map< UidType, KAutoPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KAutoPartyUserPtr spAutoPartyUser = mit->second;
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"매치 유저 포인터값이 이상합니다." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}
        
		spAutoPartyUser->ClearPartyUserInfo();
		spAutoPartyUser->ClearPartyRoomUserInfo();
	}
}

bool KAutoPartyUserManager::UpdateRegroupPartyInfo( IN const KPartyUserInfo& kPartyUserInfo, IN const KPartyRoomUserInfo& kPartyRoomUserInfo )
{
	KAutoPartyUserPtr spAutoPartyUser = GetUser( kPartyUserInfo.m_iUnitUID );
	if( spAutoPartyUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 유저입니다!" )
			<< BUILD_LOG( kPartyUserInfo.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	spAutoPartyUser->UpdatePartyUserInfo( kPartyUserInfo );
	spAutoPartyUser->UpdatePartyRoomUserInfo( kPartyRoomUserInfo );
	return true;
}

bool KAutoPartyUserManager::IsAllPlayersUpdatedRegroupPartyInfo()
{
	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( mit->second->IsEmptyPartyRoomUserInfo() == true  ||  mit->second->IsEmptyPartyUserInfo() == true )
		{
			return false;
		}
	}

	return true;
}

//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
bool KAutoPartyUserManager::GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo, OUT UidType& iRegroupPartyHostUnitUID )
#else
bool KAutoPartyUserManager::GetRegroupPartyInfo( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUserInfo )
#endif SERV_REGROUP_OLD_PARTY_HOST
//}}
{
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	iRegroupPartyHostUnitUID = 0;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	vecRegroupPartyUserInfo.clear();

	std::map< UidType, KAutoPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{	
		KAutoPartyUserPtr spAutoPartyUser = mit->second;
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		KRegroupPartyUserInfo kRegroupPartyUserInfo;

		// 파티 재결성을 위한 KPartyUserInfo
		if( spAutoPartyUser->GetPartyUserInfo( kRegroupPartyUserInfo.m_kPartyUserInfo ) == false )
		{
			START_LOG( cerr, L"파티 유저 정보가 없다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}
        
		// 방에 입장하기 위한 KRoomUserInfo
		if( spAutoPartyUser->GetPartyRoomUserInfo( kRegroupPartyUserInfo.m_kPartyRoomUserInfo ) == false )
		{
			START_LOG( cerr, L"룸 유저 정보가 없다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		// 자동 파티 맺기 이전의 파티 정보 보존!
		kRegroupPartyUserInfo.m_kOldPartyInfo = spAutoPartyUser->GetOldPartyInfo();

		// 리스트에 넣자
		vecRegroupPartyUserInfo.push_back( kRegroupPartyUserInfo );

		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		// 해당 유저가 파티장이었으면 리그룹 파티장 후보로 놓자!
		if( iRegroupPartyHostUnitUID == 0 )
		{
			if( kRegroupPartyUserInfo.m_kOldPartyInfo.m_iOldHostUnitUID == spAutoPartyUser->GetCharUID() )
			{
				iRegroupPartyHostUnitUID = spAutoPartyUser->GetCharUID();
			}
		}
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}

	return true;
}

//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KAutoPartyUserManager::GetAutoPartyCancelUserList( OUT std::vector< UidType >& vecCancelUserList )
{
	vecCancelUserList.clear();

	std::map< UidType, KAutoPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( !mit->second )
		{
			START_LOG( cerr, L"룸 유저 포인터 이상." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsAccepted() == false )
		{
			vecCancelUserList.push_back( mit->first );
		}
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

