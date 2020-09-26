#include "CnPartyUserManager.h"
#include "NetError.h"
#include "X2Data/XSLPartyManager.h"


const float KCnPartyUserManager::m_scfCandidateTimeLimit = 7.f;

KCnPartyUserManager::KCnPartyUserManager()
{
	Reset();
}

KCnPartyUserManager::~KCnPartyUserManager()
{
}

void KCnPartyUserManager::Reset()
{
    SetMaxMember( 0 );
	m_mapMember.clear();
	m_mapCandidate.clear();
}

//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnPartyUserManager::Tick( OUT std::vector< UidType >& vecDeleteCadidateUser )
{
	vecDeleteCadidateUser.clear();

	// 신청 대기자 리스트 체크
	if( m_mapCandidate.empty() )
		return;

	std::vector< UidType > vecLeaveList;
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	for( mit = m_mapCandidate.begin(); mit != m_mapCandidate.end(); ++mit )
	{
		const KWaitingUserInfo& kWaitingUserInfo = mit->second;

		// 초대 받은 유저가 의사 결정을 하지 않고 7초가 지남.
		if( kWaitingUserInfo.m_kTimer.elapsed() <= PARTY_INVITE_TIME_LIMIT )
			continue;

		if( IS_NULL( kWaitingUserInfo.m_spUser ) )
			continue;

		if( kWaitingUserInfo.m_bInvited )
		{	
			// 초대장을 날린 유저에게 초대가 거절되었다고 알려줌.
			KCnPartyUserPtr spSendInviteUser = GetPartyUser( kWaitingUserInfo.m_iSendInviteUnitUID );
			if( IS_NOT_NULL( spSendInviteUser ) )
			{
				KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
				kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
				kDenyNot.m_iUnitUID = kWaitingUserInfo.m_spUser->GetCharUID();
				spSendInviteUser->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			}
			//else -- 굳이 파티장에게까지 날릴필요가 있을까? 일단 주석처리
			//{
			//	// 시간차로인해 초대장을 날린 유저가 없다면 파티장에게 날리자
			//	KCnPartyUserPtr spHost = GetHostPtr();
			//	if( IS_NOT_NULL( spHost ) )
			//	{
			//		KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
			//		kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
			//		kDenyNot.m_iUnitUID = kWaitingUserInfo.m_spUser->GetCharUID();
			//		spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			//	}
			//	else
			//	{
			//		START_LOG( cerr, L"파티장 포인터가 잘못되었음.!" )
			//			<< END_LOG;
			//	}
			//}
		}
		else
		{
			// 파티 가입 요청 받은 파티장이 의사 결정 하지 않고 10초가 지남.
			//## 당사자에게 파티거절되었다고 알려줌.
			if( mit->second.m_spUser != NULL )
			{
				KEGS_JOIN_PARTY_NOT kNot;
				kNot.m_iOK = NetError::ERR_PARTY_18;
				mit->second.m_spUser->SendToGSCharacter( EPM_JOIN_PARTY_NOT, kNot );
			}
			else
			{
				START_LOG( cerr, L"대기자 대기시간이 10초가 넘었는데 포인터가 잘못되었음." )
					<< END_LOG;
			}
		}

		//처리후 대기자에서 지워준다.
		vecDeleteCadidateUser.push_back( kWaitingUserInfo.m_spUser->GetCharUID() );
	}
}
#else
void KCnPartyUserManager::Tick()
{
	// 신청 대기자 리스트 체크
	if( m_mapCandidate.empty() )
		return;

	std::vector< UidType > vecLeaveList;
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	for( mit = m_mapCandidate.begin(); mit != m_mapCandidate.end(); ++mit )
	{
		const KWaitingUserInfo& kWaitingUserInfo = mit->second;

		if( kWaitingUserInfo.m_kTimer.elapsed() <= m_scfCandidateTimeLimit )
			continue;

		if( kWaitingUserInfo.m_bInvited )
		{
			// 초대 받은 유저가 의사 결정을 하지 않고 10초가 지남.
			//## 파티장에게 초대가 거절되었다고 알려줌.

			KCnPartyUserPtr spHost = GetHostPtr();
			if( spHost != NULL )
			{
				KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
				kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
				kDenyNot.m_iUnitUID = kWaitingUserInfo.m_spUser->GetCharUID();
				spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			}
			else
			{
				START_LOG( cerr, L"파티장 포인터가 잘못되었음.!" )
					<< END_LOG;
			}
		}
		else
		{
			// 파티 가입 요청 받은 파티장이 의사 결정 하지 않고 10초가 지남.
			//## 당사자에게 파티거절되었다고 알려줌.
			if( mit->second.m_spUser != NULL )
			{
				KEGS_JOIN_PARTY_NOT kNot;
				kNot.m_iOK = NetError::ERR_PARTY_18;
				mit->second.m_spUser->SendToGSCharacter( EPM_JOIN_PARTY_NOT, kNot );
			}
			else
			{
				START_LOG( cerr, L"대기자 대기시간이 10초가 넘었는데 포인터가 잘못되었음." )
					<< END_LOG;
			}
		}

		//처리후 대기자에서 지워준다.
		mit = m_mapCandidate.erase( mit );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

bool KCnPartyUserManager::GetUserInfo( IN UidType nCID, OUT KPartyUserInfo& kInfo )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( nCID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"정보를 요청한 유저가 없음." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	spPartyUser->GetPartyUserInfo( kInfo );
	kInfo.m_bHost	= spPartyUser->IsHost();
	kInfo.m_bReady	= spPartyUser->IsReady();
	return true;
}

void KCnPartyUserManager::GetUserInfoList( OUT std::vector< KPartyUserInfo >& kInfoList )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KPartyUserInfo kInfo;
		mit->second->GetPartyUserInfo( kInfo );
		kInfo.m_bHost	= mit->second->IsHost();
		kInfo.m_bReady	= mit->second->IsReady();
		kInfoList.push_back( kInfo );
	}
}

bool KCnPartyUserManager::GetDungeonRoomUserInfoList( OUT std::vector< KPartyRoomUserInfo >& vecInfoList )
{
	vecInfoList.clear();

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KPartyRoomUserInfo kInfo;
		if( mit->second->GetPartyRoomUserInfo( kInfo ) == false )
		{
			return false;
		}

		vecInfoList.push_back( kInfo );
	}

	return true;
}

void KCnPartyUserManager::GetListUserInfoList( OUT std::vector< KPartyListUserInfo >& vecUserInfo )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;

	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KPartyListUserInfo kInfo;
		kInfo.m_iUnitUID		= mit->second->GetCharUID();
		kInfo.m_wstrNickName	= mit->second->GetCharName();
		kInfo.m_cUnitClass		= mit->second->GetUnitClass();
		kInfo.m_ucLevel			= mit->second->GetLevel();
		kInfo.m_bHost			= mit->second->IsHost();
		vecUserInfo.push_back( kInfo );
	}
}

//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
void KCnPartyUserManager::GetMatchUserInfoList( IN const UidType iPartyUID, OUT std::vector< KMatchUserInfo >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;			
		}

		// 매치 유저 정보 얻어오기!
		KMatchUserInfo kUserInfo;
		spPartyUser->GetMatchUserInfo( iPartyUID, kUserInfo );
		vecUserList.push_back( kUserInfo );
	}
}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnPartyUserManager::GetAutoPartyUserInfoList( IN const KOldPartyInfo& kOldPartyInfo, OUT std::vector< KAutoPartyUserInfo >& vecUserList )
{
	vecUserList.clear();

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;			
		}

		// 매치 유저 정보 얻어오기!
		KAutoPartyUserInfo kUserInfo;
		spPartyUser->GetAutoPartyUserInfo( kUserInfo );
		kUserInfo.m_kOldPartyInfo = kOldPartyInfo;
		vecUserList.push_back( kUserInfo );
	}
}

bool KCnPartyUserManager::GetGSUID( IN const UidType nCID, OUT UidType& iGSUID )
{
	iGSUID = 0;

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	mit = m_mapMember.find( nCID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cerr, L"파티 멤버 정보를 찾을 수 없습니다." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return false;
	}

	if( IS_NULL( mit->second ) )
	{
		START_LOG( cerr, L"파티 멤버 포인터가 이상합니다." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	iGSUID = mit->second->GetGSUID();
	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnPartyUserManager::GetUnitUIDList( OUT std::set< UidType >& setUidList ) const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;			
		}

		setUidList.insert( mit->second->GetCharUID() );
	}
}

void KCnPartyUserManager::GetUnitUIDList( OUT std::vector< UidType >& vecUidList ) const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		vecUidList.push_back( mit->second->GetCharUID() );
	}
}

void KCnPartyUserManager::GetUnitUIDListByGSUID( IN UidType iGSUID, OUT std::vector< UidType >& vecUserList ) const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->GetGSUID() != iGSUID )
			continue;

		vecUserList.push_back( mit->second->GetCharUID() );
	}
}

void KCnPartyUserManager::ClearDungeonRoomUserInfoList()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		mit->second->ClearPartyRoomUserInfo();
	}
}

bool KCnPartyUserManager::IsAllCheckedGameStart()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
		{
			START_LOG( cerr, L"유저포인터 이상.!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( mit->second->IsExistPartyRoomUserInfo() == false )
		{
			return false;
		}
	}

	return true;
}



KCnPartyUserPtr KCnPartyUserManager::GetHostPtr() const
{
	//{{ 2012. 12. 07	최육사	배틀필드 시스템
#ifdef SERV_CODE_CLEANUP_2012_12_07
	// [2012-12-07][최육사] 파티원이 아예 없을때 호출되는 경우도 있는것 같다. 이경우에 대한 예외처리.
	if( m_mapMember.empty() == true )
	{
		START_LOG( cerr, L"파티 멤버가 한명도 없는데 이 함수가 호출되었다!" )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;

		return KCnPartyUserPtr();
	}
#endif SERV_CODE_CLEANUP_2012_12_07
	//}}

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->IsHost() == true )
		{
			return mit->second;
		}
	}

	START_LOG( cerr, L"파티장이 없다.? ㅠㅂㅠ 제발 날 힘들게 하지 말아줘~~" )
		<< END_LOG;

	return KCnPartyUserPtr();
}

UidType KCnPartyUserManager::GetHostUID() const
{
	KCnPartyUserPtr spHost = GetHostPtr();
	if( spHost == NULL )
	{
		START_LOG( cerr, L"파티장이 없음." )
			<< END_LOG;

		return 0;
	}

	return spHost->GetCharUID();
}

KCnPartyUserPtr KCnPartyUserManager::GetPartyUser( IN const UidType nCID )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	mit = m_mapMember.find( nCID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cwarn, L"파티 멤버 정보를 찾을 수 없습니다." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"파티 멤버 포인터가 이상합니다." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	return mit->second;
}

KCnPartyUserPtr KCnPartyUserManager::GetCandidateUser( IN const UidType nCID )
{
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	mit = m_mapCandidate.find( nCID );
	if( mit == m_mapCandidate.end() )
	{
		START_LOG( cwarn, L"파티 대기자 정보를 찾을 수 없습니다." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	if( mit->second.m_spUser == NULL )
	{
		START_LOG( cerr, L"파티 대기자 포인터가 이상합니다." )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return KCnPartyUserPtr();
	}

	return mit->second.m_spUser;
}

bool KCnPartyUserManager::AddCandidate( IN const KPartyUserInfo& kUserInfo, IN const bool bInvited, IN const UidType iSendInviteUnitUID )
{ 
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"캐릭터 UID 이상" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( IsCandidate( kUserInfo.m_iUnitUID ) == true )
	{
		START_LOG( cerr, L"이미 후보 회원임" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	KWaitingUserInfo kWaitUser;
	kWaitUser.m_spUser		= KCnPartyUserPtr( new KCnPartyUser( kUserInfo ) );
	kWaitUser.m_bInvited	= bInvited;
	//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kWaitUser.m_iSendInviteUnitUID = iSendInviteUnitUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	kWaitUser.m_kTimer.restart();

	m_mapCandidate.insert( std::make_pair( kUserInfo.m_iUnitUID, kWaitUser ) );
	return true;
}

bool KCnPartyUserManager::AddMember( IN const KPartyUserInfo& kUserInfo )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"캐릭터 UID 이상" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	std::map< UidType, KCnPartyUserPtr >::iterator mit = m_mapMember.find( kUserInfo.m_iUnitUID );
	if( mit != m_mapMember.end() )
	{
		START_LOG( cerr, L"이미 정회원임" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	// 파티원 생성
	KCnPartyUserPtr spPartyUser( new KCnPartyUser( kUserInfo ) );

	//파티장 임명
	if( m_mapMember.size() == 0 )
	{
		spPartyUser->SetHost( true );
	}

	m_mapMember.insert( std::make_pair( kUserInfo.m_iUnitUID, spPartyUser ) );

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	if( kUserInfo.m_bComeBack == true )
	{
		if( CheckComeBackUserInParty() == true ) // 기존의 파티원 중 복귀 유저가 있을 때
		{
			CTime tUserBuffEnd;
			if( KncUtil::ConvertStringToCTime( kUserInfo.m_wsrtComeBackEnd, tUserBuffEnd ) == false )
			{
				START_LOG( cerr, L"문자열 시간조립 실패.!" )
					<< BUILD_LOG( kUserInfo.m_iGSUID )
					<< BUILD_LOG( kUserInfo.m_wsrtComeBackEnd )
					<< END_LOG;

				tUserBuffEnd = CTime( 0 );
			}

			if( m_tComeBackBuffEnd < tUserBuffEnd )
			{
				m_tComeBackBuffEnd = tUserBuffEnd;
			}
			SetComeBackUserInParty( true );
		}
		else // 기존의 파티원 중 복귀 유저가 없을 때
		{
			SetComeBackUserInParty( true );
			SetComeBackBuffEnd( kUserInfo.m_wsrtComeBackEnd );
		}
	}
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	// 파티장 체크
	if( GetHostUID() == 0 )
	{
		START_LOG( cerr, L"파티장 설정이 잘못되었다!" )
			<< BUILD_LOG( kUserInfo.m_wstrNickName )
			<< END_LOG;
	}

	return true;
}

bool KCnPartyUserManager::DeleteCandidate( IN UidType nCID )
{
	std::map< UidType, KWaitingUserInfo >::iterator mit = m_mapCandidate.find( nCID );
	if( mit == m_mapCandidate.end() )
	{
		START_LOG( cerr, L"후보 회원에 존재하지 않음" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	m_mapCandidate.erase( mit );
	return true;
}

bool KCnPartyUserManager::DeleteMember( IN UidType nCID )
{
	// 파티장이었을 경우 새로운 파티장 임명.

	std::map< UidType, KCnPartyUserPtr >::iterator mit = m_mapMember.find( nCID );
	if( mit == m_mapMember.end() )
	{
		START_LOG( cerr, L"정회원에 존재하지 않음" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	// 파티장이었는지 저장한다음 삭제
	const bool bIsHost = mit->second->IsHost();

	// 삭제
	m_mapMember.erase( mit );

	//파티장이었는데 파티원이 남아있는경우.
	if( bIsHost == true  &&  m_mapMember.size() > 0 )
	{
		mit = m_mapMember.begin();
		mit->second->SetHost( true );

		// 파티장 체크
		if( GetHostUID() == 0 )
		{
			START_LOG( cerr, L"파티장 설정이 잘못되었다!" )
				<< BUILD_LOG( nCID )
				<< END_LOG;
		}
	}	

	return true;
}

bool KCnPartyUserManager::Create( IN const std::vector< KPartyUserInfo >& vecUserInfo )
{
	bool bRet = false;

	BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, vecUserInfo )
	{
		if( GetNumMember() == 0 )
		{
			if( Create( kInfo ) == true )
			{
				bRet = true;
			}
		}
		else
		{
			if( AddMember( kInfo ) == true )
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool KCnPartyUserManager::Create( IN const std::vector< KRegroupPartyUserInfo >& vecUserInfo )
{
	bool bRet = false;

	BOOST_TEST_FOREACH( const KRegroupPartyUserInfo&, kInfo, vecUserInfo )
	{
		if( GetNumMember() == 0 )
		{
			if( Create( kInfo.m_kPartyUserInfo ) == true )
			{
				bRet = true;
			}
		}
		else
		{
			if( AddMember( kInfo.m_kPartyUserInfo ) == true )
			{
				bRet = true;
			}
		}
	}

	return bRet;
}

bool KCnPartyUserManager::Create( IN const KPartyUserInfo& kUserInfo )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"캐릭터 UID 이상" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( GetNumTotal() > 0 )
	{
		START_LOG( cerr, L"빈 파티가 아님" )
			<< BUILD_LOG( GetNumTotal() )
			<< END_LOG;

		return false;
	}

	LIF( AddMember( kUserInfo ) );
	return true;
}

bool KCnPartyUserManager::JoinCandidate( IN const KPartyUserInfo& kUserInfo, IN const bool bInvited, IN const UidType iSendInviteUnitUID )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"캐릭터 UID 이상" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( GetNumTotal() < 1 )
	{
		START_LOG( cerr, L"파티원이 없는 파티임" )
			<< BUILD_LOG( GetNumMember() )
			<< BUILD_LOG( GetNumCandidate() )
			<< BUILD_LOG( GetNumTotal() )
			<< BUILD_LOG( GetMaxMember() )
			<< END_LOG;

		return false;
	}

	if( GetNumTotal() >= GetMaxMember() )
	{
		// 시간차로 발생 가능한 오류임.
		// 예외1 : 파티리스트의 갱신간격은 3초임, 현재 파티인원 정보가 갱신되기전에 유저가 파티 가입 시도하면 발생가능함.

		START_LOG( cwarn, L"파티 수용 인원이 가득 참." )
			<< BUILD_LOG( GetNumMember() )
			<< BUILD_LOG( GetNumCandidate() )
			<< BUILD_LOG( GetNumTotal() )
			<< BUILD_LOG( GetMaxMember() )
			<< END_LOG;

		return false;
	}

	if( IsMemberOrCandidate( kUserInfo.m_iUnitUID ) )
	{
		START_LOG( cerr, L"이미 파티에 존재하는 유저임" )
			<< BUILD_LOG( GetNumTotal() )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( AddCandidate( kUserInfo, bInvited, iSendInviteUnitUID ) );
	return true;
}

bool KCnPartyUserManager::JoinMember( IN const KPartyUserInfo& kUserInfo )
{
	if( kUserInfo.m_iUnitUID <= 0 )
	{
		START_LOG( cerr, L"캐릭터 UID 이상" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( !IsCandidate( kUserInfo.m_iUnitUID ) )
	{
		START_LOG( cerr, L"후보 회원이 아님" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( IsMember( kUserInfo.m_iUnitUID ) )
	{
		START_LOG( cerr, L"이미 정회원임" )
			<< BUILD_LOG( kUserInfo.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( DeleteCandidate( kUserInfo.m_iUnitUID ) );
	LIF( AddMember( kUserInfo ) );

	return true;
}

bool KCnPartyUserManager::LeaveCandidate( IN UidType iUnitUID )
{
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"캐릭터 UID 이상" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	if( !IsCandidate( iUnitUID ) )
	{
		START_LOG( cerr, L"후보 회원이 아님" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( DeleteCandidate( iUnitUID ) );
	return true;
}

bool KCnPartyUserManager::LeaveMember( IN UidType iUnitUID )
{	
	if( iUnitUID <= 0 )
	{
		START_LOG( cerr, L"캐릭터 UID 이상" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	if( !IsMember( iUnitUID ) )
	{
		START_LOG( cerr, L"정회원이 아님." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		return false;
	}

	LIF( DeleteMember( iUnitUID ) );
	return true;
}

bool KCnPartyUserManager::IsHost( IN UidType nCID )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( nCID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"파티장 체크할 유저가 없음.!" )
			<< BUILD_LOG( nCID )
			<< BUILD_LOG( m_mapMember.size() )
			<< END_LOG;
		return false;
	}

	return spPartyUser->IsHost();
}

bool KCnPartyUserManager::IsAllReady()
{
	//대기자가 있으면 체크를 해주지 않는다.
	if( m_mapCandidate.empty() == false )
		return false;

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->IsHost() == false  &&  mit->second->IsReady() == false )
			return false;
	}

	return true;
}

//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION

bool KCnPartyUserManager::IsAllPartyMembersSameServerGroup( IN UidType iUnitUID )
{
	const UidType iServerGroupID = KncUid::ExtractServerGroupID( iUnitUID );

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		// 체크하려는 유저의 UnitUID와 다른 서버군 유저가 있는지 보자!
		if( KncUid::ExtractServerGroupID( mit->second->GetCharUID() ) != iServerGroupID )
		{
			return false;			
		}
	}

	return true;
}

#endif SERV_INTEGRATION
//}}

//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
bool KCnPartyUserManager::IsAllPartyMemberEqualOrOverThisLevel( IN const u_char ucLevel )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"파티원 포인터값이 이상합니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			return false;
		}

		if( spPartyUser->GetLevel() < ucLevel )
			return false;
	}

	return true;
}
#endif SERV_PVP_NEW_SYSTEM
//}}

bool KCnPartyUserManager::ChangeReady( IN UidType nCID, IN bool bIsReady )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( nCID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"정회원에 존재하지 않음" )
			<< BUILD_LOG( nCID )
			<< END_LOG;

		return false;
	}

	spPartyUser->SetReady( bIsReady );
	return true;
}

void KCnPartyUserManager::ChangeAllReady( IN bool bIsReady )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;

	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		if( mit->second->IsHost() == false )
		{
			mit->second->SetReady( bIsReady );

			KEGS_PARTY_CHANGE_READY_NOT kNot;
			kNot.m_UnitUID	= mit->second->GetCharUID();
			kNot.m_bReady	= bIsReady;
			BroadCastToGSCharacter( EPM_PARTY_CHANGE_READY_NOT, kNot );
		}
	}
}

bool KCnPartyUserManager::ChangeHost( IN UidType iNewHostUID )
{	
	KCnPartyUserPtr spOldHostUser = GetHostPtr();
	if( spOldHostUser == NULL )
	{
		START_LOG( cerr, L"파티장을 찾을수 없음.!" )
			<< END_LOG;
		return false;
	}

	KCnPartyUserPtr spNewHostUser = GetPartyUser( iNewHostUID );
	if( spNewHostUser == NULL )
	{
		START_LOG( cerr, L"변경될 파티장을 찾을수 없음.!" )
			<< BUILD_LOG( spOldHostUser->GetCharUID() )
			<< BUILD_LOG( iNewHostUID )
			<< END_LOG;
		return false;
	}

	spOldHostUser->SetHost( false );
	spNewHostUser->SetHost( true );

	// 파티장 체크
	if( GetHostUID() == 0 )
	{
		START_LOG( cerr, L"파티장 설정이 잘못되었다!" )
			<< BUILD_LOG( iNewHostUID )
			<< END_LOG;
	}
	return true;
}

void KCnPartyUserManager::UpdatePartyMemberListToFieldUser( IN const UidType iPartyUID )
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		if( mit->second == NULL )
			continue;

		// 클라이언트로 파티 리스트 업데이트를 보낸다.
		KEPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT kPacket;
		kPacket.m_iPartyUID = iPartyUID;
		GetUnitUIDList( kPacket.m_setPartyUIDList );
		mit->second->SendToGSCharacter( EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT, kPacket );
	}
}


//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
bool KCnPartyUserManager::CheckComeBackUserInParty()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit = m_mapMember.begin();
	for( ; mit != m_mapMember.end() ; ++mit )
	{
		KCnPartyUserPtr pCnPartyUser = mit->second;
		if( pCnPartyUser == NULL)
		{
			START_LOG( cerr, L"CnPartyUser 포인터가 이상하다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// 복귀 유저인지 판별하자!
		// 한명이라도 복귀 유저이면 true
		if( pCnPartyUser->GetComeBackUser() == true )
		{
			SetComeBackUserInParty( true );
			return true;
		}
	}
	SetComeBackUserInParty( false );
	return false;
}

void KCnPartyUserManager::SetComeBackBuffEnd( IN const std::wstring& wstrBuffEnd )
{
	if( GetComeBackUserInParty() == true )
	{
		if( KncUtil::ConvertStringToCTime( wstrBuffEnd, m_tComeBackBuffEnd ) == false )
		{
			START_LOG( cerr, L"문자열 시간조립 실패.!" )
				<< BUILD_LOG( wstrBuffEnd )
				<< END_LOG;

			m_tComeBackBuffEnd = CTime( 0 );
		}
	}
	else
	{
		m_tComeBackBuffEnd = CTime( 0 );
	}
}
#endif SERV_COME_BACK_USER_REWARD
//}} 

//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnPartyUserManager::GetCadidateUserList( OUT std::vector< UidType >& vecCadidateUserList )
{
	vecCadidateUserList.clear();

	// 신청 대기자 리스트 체크
	if( m_mapCandidate.empty() )
		return;

	std::vector< UidType > vecLeaveList;
	std::map< UidType, KWaitingUserInfo >::iterator mit;
	for( mit = m_mapCandidate.begin(); mit != m_mapCandidate.end(); ++mit )
	{
		const KWaitingUserInfo& kWaitingUserInfo = mit->second;

		if( IS_NULL( kWaitingUserInfo.m_spUser ) )
			continue;

		// 지워질 대기자 리스트에 추가한다.
		vecCadidateUserList.push_back( kWaitingUserInfo.m_spUser->GetCharUID() );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 03. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KCnPartyUserManager::SetPartyMemberStatus( IN const UidType iUnitUID, IN const KPartyMemberStatus& kInfo )
{
	KCnPartyUserPtr spCnPartyUser = GetPartyUser( iUnitUID );
	if( IS_NULL( spCnPartyUser ) )
	{
		START_LOG( cerr, L"존재 하지 않는 유저입니다." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	spCnPartyUser->SetPartyMemberStatus( kInfo );
	return true;
}

short KCnPartyUserManager::GetOldPartyFeverPoint() const
{
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit = m_mapMember.begin();
	for( ; mit != m_mapMember.end() ; ++mit )
	{
		KCnPartyUserPtr spCnPartyUser = mit->second;
		if( IS_NULL( spCnPartyUser ) )
		{
			START_LOG( cerr, L"CnPartyUser 포인터가 이상하다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// 원래 파티 정보가 있었는지 확인!
		KOldPartyInfo kInfo;
		if( spCnPartyUser->GetOldPartyInfo( kInfo ) == false )
			break;

		return kInfo.m_sOldPartyFever;
	}

	return 0;
}

bool KCnPartyUserManager::UpdateJoinRoomInfoAndGetOnOffResult( IN const UidType iUnitUID, 
															   IN const KPartyUserJoinRoomInfo& kJoinRoomInfo, 
															   OUT std::vector< UidType >& vecOnMoveButton,
															   OUT std::vector< UidType >& vecOffMoveButton )
{
	vecOnMoveButton.clear();
	vecOffMoveButton.clear();

	KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"방 입장 정보를 저장할 유저를 찾지 못하였습니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	const KPartyUserJoinRoomInfo& kBeforeJoinRoomInfo = spPartyUser->GetJoinRoomInfo();
	spPartyUser->UpdateJoinRoomInfo( kJoinRoomInfo );

	// 이동한 곳이 배틀필드와 관련이 있는 경우만 처리
	if( CXSLRoom::GetRoomType( kBeforeJoinRoomInfo.m_iRoomUID ) == CXSLRoom::RT_BATTLE_FIELD  ||  
		CXSLRoom::GetRoomType( kJoinRoomInfo.m_iRoomUID ) == CXSLRoom::RT_BATTLE_FIELD )
	{		
		std::map< int, UidType > mapCheckSameBattleField; // 같은 필드에 있는 유저를 찾기 위한 컨테이너		

        // 같은 필드에 있는 유저가 있는지 확인하기!
		std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
		for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
		{
			KCnPartyUserPtr spCheckPartyUser = mit->second;
			if( spCheckPartyUser == NULL )
			{
				START_LOG( cerr, L"방 입장 정보를 저장할 유저를 찾지 못하였습니다!" )
					<< BUILD_LOG( mit->first )
					<< END_LOG;
				continue;
			}

			// 방에 입장한 상태가 아닌데..
			if( spCheckPartyUser->GetJoinRoomUID() == 0 )
			{
				// 버튼이 켜져있다면.. 끄자!
				if( spCheckPartyUser->IsOnGoToPartyButton() == true )
				{
					spCheckPartyUser->SetOnGoToPartyButton( false );
					vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );					
				}

				// 다음유저로..
				continue;
			}

			// 현재 위치가 배틀필드가 아닌데..
			if( spCheckPartyUser->GetJoinRoomType() != CXSLRoom::RT_BATTLE_FIELD )
			{
				// 버튼이 켜져있다면.. 끄자!
				if( spCheckPartyUser->IsOnGoToPartyButton() == true )
				{
					spCheckPartyUser->SetOnGoToPartyButton( false );
                    vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );
				}

				// 다음 유저로..
				continue;
			}
			// 현재 위치가 배틀필드인데..
			else
			{
				// 같은 필드유저로 체크된적이 없다면!
				std::map< int, UidType >::const_iterator mitFB = mapCheckSameBattleField.find( spCheckPartyUser->GetJoinBattleFieldID() );
                if( mitFB == mapCheckSameBattleField.end() )
				{
					// 등록하자! 널 기준으로 모이는거야!!
					mapCheckSameBattleField.insert( std::make_pair( spCheckPartyUser->GetJoinBattleFieldID(), spCheckPartyUser->GetJoinRoomUID() ) );

					// 등록된 사람은 버튼을 켜지 말자!
					if( spCheckPartyUser->IsOnGoToPartyButton() == true )
					{
						spCheckPartyUser->SetOnGoToPartyButton( false );
						vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );
					}
				}
				// 이미 등록된 유저가 있다면!
				else
				{
					// 이미 같은곳에 있는거라면..
					if( mitFB->second == spCheckPartyUser->GetJoinRoomUID() )
					{
						// 버튼을 켜지 말자!
						if( spCheckPartyUser->IsOnGoToPartyButton() == true )
						{
							spCheckPartyUser->SetOnGoToPartyButton( false );
							vecOffMoveButton.push_back( spCheckPartyUser->GetCharUID() );							
						}

						// 다음 유저로..
						continue;
					}
					// 다른 필드에 있는 사람은..
					else
					{
						// 버튼을 켜주자!
						if( spCheckPartyUser->IsOnGoToPartyButton() == false )
						{
							spCheckPartyUser->SetOnGoToPartyButton( true );
							vecOnMoveButton.push_back( spCheckPartyUser->GetCharUID() );
						}

						// 다음 유저로..
						continue;
					}
				}
			}
		}
	}

	return true;
}

void KCnPartyUserManager::GetUnitUIDListAboutEndGameRegroupParty( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	// 1. 원래 수동 파티가 3명이었는데 현재 총인원이 4명인 경우! ( 1명만 강퇴 시키면 된다! )
	// 2. 원래 수동 파티가 2명이었는데 현재 총인원이 3명인경우! ( 1명만 강퇴 시키면 된다! )
	// 3. 원래 수동 파티가 한명도 없었을 경우! ( 모두다 파티 탈퇴 처리 하면 된다! )

	// 재결합 해야 하는 경우
	// 1. 원래 수동 파티가 2명:2명 이었고 현재 총인원이 4명인 경우! ( 2명은 자동 강퇴 처리, 이후에 재결합 조치! )

	switch( GetNumMember() ) 
	{
	case 1:
		CheckRegroup1Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	case 2:
		CheckRegroup2Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	case 3:
		CheckRegroup3Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	case 4:
		CheckRegroup4Member( vecRegroupPartyUser, setLeaveUnitUIDList );
		break;

	default:
		{
            START_LOG( cerr, L"현재 파티원 수가 이상합니다!" )
				<< BUILD_LOG( GetNumMember() )
				<< END_LOG;
		}
		break;
	}
}

void KCnPartyUserManager::GetCheckedSameOldParty( OUT std::map< UidType, std::set< UidType > >& mapCheckedSameOldParty, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	mapCheckedSameOldParty.clear();

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"방 입장 정보를 저장할 유저를 찾지 못하였습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KOldPartyInfo kInfo;
		if( spPartyUser->GetOldPartyInfo( kInfo ) == false )
		{
			// 옛날 파티 정보가 없다면 그냥 파티 이탈!
			setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
			continue;
		}

		// 시작할때 파티 상태가 아니었다면 파티 이탈!
		if( kInfo.m_iOldPartyUID == 0 )
		{
			setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
			continue;
		}

		// 시작할때 같은 파티원이 없었다면 역시 파티 이탈!
		if( kInfo.m_setOldPartyMember.empty() == true )
		{
			setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
			continue;
		}

		// 현재 멤버중에 같은 파티원이 있는지 체크하자!		
		BOOST_TEST_FOREACH( const UidType, iUnitUID, kInfo.m_setOldPartyMember )
		{
			bool bInsertOldPartyMember = false;

			if( iUnitUID == spPartyUser->GetCharUID() )
			{
				bInsertOldPartyMember = true;
			}
			else
			{
				if( IsMember( iUnitUID ) == true )
				{
					bInsertOldPartyMember = true;
				}
			}

			// 현재 멤버중에 원래 같은 파티원이 있다면 체크!!
			if( bInsertOldPartyMember )
			{
				std::map< UidType, std::set< UidType > >::iterator mitOP;
				mitOP = mapCheckedSameOldParty.find( kInfo.m_iOldPartyUID );
				if( mitOP == mapCheckedSameOldParty.end() )
				{
					std::set< UidType > setOldPartyMember;
					setOldPartyMember.insert( iUnitUID );
					mapCheckedSameOldParty.insert( std::make_pair( kInfo.m_iOldPartyUID, setOldPartyMember ) );
				}
				else
				{
					mitOP->second.insert( iUnitUID );
				}
			}
		}
	}
}

void KCnPartyUserManager::CheckRegroup1Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	_JIF( GetNumMember() == 1, return; );

	//////////////////////////////////////////////////////////////////////////
	// 현재 파티원수가 1명인 경우엔 당연히 자동으로 파티 해체를 시켜야 한다!
	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"방 입장 정보를 저장할 유저를 찾지 못하였습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		setLeaveUnitUIDList.insert( spPartyUser->GetCharUID() );
	}
}

void KCnPartyUserManager::CheckRegroup2Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	//////////////////////////////////////////////////////////////////////////
	// 현재 파티원수가 2명인 경우엔
	// 1. 둘다 원래 같은 소속 수동파티원이라면 파티 유지!
	// 2. 그렇지 않다면 무조건 파티 이탈 처리!

	const int iNumMember = GetNumMember();
	_JIF( iNumMember == 2, return; );

	std::map< UidType, std::set< UidType > > mapCheckedSameOldParty;
	GetCheckedSameOldParty( mapCheckedSameOldParty, setLeaveUnitUIDList );

	// 파티 소속이 1명 이하인 경우 이탈 처리 합니다!
	std::map< UidType, std::set< UidType > >::const_iterator mit;
	for( mit = mapCheckedSameOldParty.begin(); mit != mapCheckedSameOldParty.end(); ++mit )
	{
		const std::set< UidType >& setOldPartyMember = mit->second;
		if( setOldPartyMember.size() == 1 )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
			{
				setLeaveUnitUIDList.insert( iUnitUID );
			}
		}
	}
}

void KCnPartyUserManager::CheckRegroup3Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	//////////////////////////////////////////////////////////////////////////
	// 현재 파티원수가 3명인 경우엔
	// 1. 세명 다 원래 같은 소속 수동파티원이라면 파티 유지!
	// 2. 세명중 두명만 같은 소속 수동파티원이라면 그 두명만 유지하고 남은 한명은 이탈 처리!
	// 3. 세명 모두 같은 소속 수동파티원이 아니라면 모두 이탈 처리!

	const int iNumMember = GetNumMember();
	_JIF( iNumMember == 3, return; );

	std::map< UidType, std::set< UidType > > mapCheckedSameOldParty;
	GetCheckedSameOldParty( mapCheckedSameOldParty, setLeaveUnitUIDList );

	// 파티 소속이 1명 이하인 경우 이탈 처리 합니다!
	std::map< UidType, std::set< UidType > >::const_iterator mit;
	for( mit = mapCheckedSameOldParty.begin(); mit != mapCheckedSameOldParty.end(); ++mit )
	{
		const std::set< UidType >& setOldPartyMember = mit->second;
		if( setOldPartyMember.size() == 1 )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
			{
				setLeaveUnitUIDList.insert( iUnitUID );
			}
		}
	}
}

void KCnPartyUserManager::CheckRegroup4Member( OUT std::vector< KRegroupPartyUserInfo >& vecRegroupPartyUser, OUT std::set< UidType >& setLeaveUnitUIDList )
{
	vecRegroupPartyUser.clear();
	setLeaveUnitUIDList.clear();

	const int iNumMember = GetNumMember();
	_JIF( iNumMember == 4, return; );

	std::map< UidType, std::set< UidType > > mapCheckedSameOldParty;
	GetCheckedSameOldParty( mapCheckedSameOldParty, setLeaveUnitUIDList );

	// 파티 소속이 1명 이하인 경우 이탈 처리 합니다!
	bool bCheckedDoubleParty = false;

	std::map< UidType, std::set< UidType > >::const_iterator mit;
	for( mit = mapCheckedSameOldParty.begin(); mit != mapCheckedSameOldParty.end(); ++mit )
	{
		const std::set< UidType >& setOldPartyMember = mit->second;

		// 수동 파티 멤버값이 1일 경우. 바로 이탈 처리.
		if( setOldPartyMember.size() == 1 )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
			{
				setLeaveUnitUIDList.insert( iUnitUID );
			}
			continue;
		}

		if( setOldPartyMember.size() != 2 )
			continue;

		// 수동 파티 멤버값이 2일 경우 처음 멤버들은 현재 파티에 담게 한다. ( 굳이 파티를 이탈시킬 필요가 없음 )
		if( bCheckedDoubleParty == false )
		{
			bCheckedDoubleParty = true;
			continue;
		}
		
		// 두번째로 체크되는 파티는 재결합 해야하는 파티이다!
		BOOST_TEST_FOREACH( const UidType, iUnitUID, setOldPartyMember )
		{
			KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
			if( spPartyUser == NULL )
			{
				START_LOG( cerr, L"방 입장 정보를 저장할 유저를 찾지 못하였습니다!" )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			KRegroupPartyUserInfo kInfo;
			spPartyUser->GetPartyUserInfo( kInfo.m_kPartyUserInfo );
			spPartyUser->GetOldPartyInfo( kInfo.m_kOldPartyInfo );
			vecRegroupPartyUser.push_back( kInfo );

			// 재결합되는 파티원들은 현재 파티에서 이탈처리한다.
			setLeaveUnitUIDList.insert( iUnitUID );
		}
	}
}

//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
bool KCnPartyUserManager::ChangeOldPartyHost( OUT UidType& iNewHostUID )
{
	iNewHostUID = 0;

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 체크값 초기화할 유저를 찾지 못하였습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		KOldPartyInfo kInfo;
		if( spPartyUser->GetOldPartyInfo( kInfo ) == false )
			continue;

		if( iNewHostUID == 0 )
		{
			iNewHostUID = kInfo.m_iOldHostUnitUID;
			continue;
		}
		
		if( iNewHostUID != kInfo.m_iOldHostUnitUID )
		{
			START_LOG( cerr, L"옜날에 같은 파티원이었는데 파티장은 달랐다? 이상한 일이네.." )
				<< BUILD_LOG( iNewHostUID )
				<< BUILD_LOG( kInfo.m_iOldHostUnitUID )
				<< END_LOG;
		}
	}

	// 선택된 옛날 파티장이 없다면 교체하지 말자!
	if( iNewHostUID == 0 )
		return false;

	// 현재 파티장과 다르다면 파티장을 교체하자!
	return ( iNewHostUID != GetHostUID() );
}
#endif SERV_REGROUP_OLD_PARTY_HOST
//}}

void KCnPartyUserManager::ClearCheckedForAutoParty()
{
	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 체크값 초기화할 유저를 찾지 못하였습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		spPartyUser->SetCheckedForAutoParty( false );
	}
}

bool KCnPartyUserManager::SetCheckedForAutoParty( IN const UidType iUnitUID )
{
    KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"파티 유저가 존재하지 않습니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;	
		return false;
	}

    spPartyUser->SetCheckedForAutoParty( true );
	return true;
}

bool KCnPartyUserManager::IsAllPartyMembersCheckedForAutoParty() const
{
	if( m_mapMember.empty() )
		return false;

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 체크값 초기화할 유저를 찾지 못하였습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spPartyUser->IsCheckedForAutoParty() == false )
			return false;
	}

	return true;
}

void KCnPartyUserManager::ClearCheckedPlayCondition()
{

	std::map< UidType, KCnPartyUserPtr >::iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 체크값 초기화할 유저를 찾지 못하였습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		spPartyUser->SetCheckedPlayCondition( false );
	}
}

bool KCnPartyUserManager::SetCheckedPlayCondition( IN const UidType iUnitUID )
{
	KCnPartyUserPtr spPartyUser = GetPartyUser( iUnitUID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"파티 유저가 존재하지 않습니다!" )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;	
		return false;
	}

	spPartyUser->SetCheckedPlayCondition( true );
	return true;
}

bool KCnPartyUserManager::IsAllPartyMembersCheckedPlayCondition() const
{
	if( m_mapMember.empty() )
		return false;

	std::map< UidType, KCnPartyUserPtr >::const_iterator mit;
	for( mit = m_mapMember.begin(); mit != m_mapMember.end(); ++mit )
	{
		KCnPartyUserPtr spPartyUser = mit->second;
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"자동 파티 체크값 초기화할 유저를 찾지 못하였습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		if( spPartyUser->IsCheckedPlayCondition() == false )
			return false;
	}

	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}



