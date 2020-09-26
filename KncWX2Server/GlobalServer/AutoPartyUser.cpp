#include "AutoPartyUser.h"
#include "Enum/Enum.h"


//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


KAutoPartyUser::KAutoPartyUser( IN const KAutoPartyUserInfo& kInfo )
{
	m_kAutoPartyUserInfo = kInfo;
}

KAutoPartyUser::~KAutoPartyUser()
{
}

char KAutoPartyUser::GetUnitType() const
{
	const CXSLUnit::UNIT_CLASS eUnitClass = static_cast<CXSLUnit::UNIT_CLASS>( GetUnitClass() );
	return static_cast<char>( CXSLUnit::GetUnitClassToUnitType( eUnitClass ) );
}

bool KAutoPartyUser::Accept()
{
	if( IsAccepted() == true )
	{
		START_LOG( cerr, L"이미 동의한 상태이다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return false;
	}

	StateTransition( KAutoPartyUserFSM::I_TO_ACCEPTED );
	return true;
}

bool KAutoPartyUser::IsAccepted()
{
	return ( GetStateID() == KAutoPartyUserFSM::S_ACCEPTED );
}

void KAutoPartyUser::UpdatePartyUserInfo( IN const KPartyUserInfo& kPartyUserInfo )
{
	if( IsEmptyPartyUserInfo() == false )
	{
		START_LOG( cerr, L"이미 PartyUserInfo가 있는데 또 업데이트 하러 들어왔네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyUserInfo.size() )
			<< END_LOG;
	}

	// PartyUserInfo 초기화
	ClearPartyUserInfo();

	// 가져온 PartyUserInfo를 저장하자!
	m_vecPartyUserInfo.push_back( kPartyUserInfo );
}

bool KAutoPartyUser::GetPartyUserInfo( OUT KPartyUserInfo& kPartyUserInfo )
{
	if( IsEmptyPartyUserInfo() == true )
	{
		START_LOG( cerr, L"PartyUserInfo가 비어있다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyUserInfo.size() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, m_vecPartyUserInfo )
	{
		kPartyUserInfo = kInfo;
		return true;
	}

	return false;
}

void KAutoPartyUser::UpdatePartyRoomUserInfo( IN const KPartyRoomUserInfo& kPartyRoomUserInfo )
{
	if( IsEmptyPartyRoomUserInfo() == false )
	{
		START_LOG( cerr, L"이미 PartyRoomUserInfo가 있는데 또 업데이트 하러 들어왔네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyRoomUserInfo.size() )
			<< END_LOG;
	}

	// PartyRoomUserInfo 초기화
	ClearPartyRoomUserInfo();

	// 가져온 RoomUserInfo를 저장하자!
	m_vecPartyRoomUserInfo.push_back( kPartyRoomUserInfo );
}

bool KAutoPartyUser::GetPartyRoomUserInfo( OUT KPartyRoomUserInfo& kPartyRoomUserInfo )
{
	if( IsEmptyPartyRoomUserInfo() == true )
	{
		START_LOG( cerr, L"PartyRoomUserInfo가 비어있다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_vecPartyRoomUserInfo.size() )
			<< END_LOG;
		return false;
	}

	BOOST_TEST_FOREACH( const KPartyRoomUserInfo&, kInfo, m_vecPartyRoomUserInfo )
	{
		kPartyRoomUserInfo = kInfo;
		kPartyRoomUserInfo.m_bAutoPartyBonus = m_kAutoPartyUserInfo.m_bAutoPartyBonus;	// 자동 파티 보너스
		kPartyRoomUserInfo.m_kOldPartyInfo = m_kAutoPartyUserInfo.m_kOldPartyInfo;		// 수동 파티 정보
		//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		kPartyRoomUserInfo.m_iAutoPartyWaitTime = m_kAutoPartyUserInfo.m_iAutoPartyWaitTime;
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
		return true;
	}

	return false;
}

void KAutoPartyUser::SendToGSCharacter( unsigned short usEventID )
{
    SendToGSCharacter( usEventID, char() );
}

bool KAutoPartyUser::IsHost( const UidType iUnitUID )
{
	// host unit uid 가 0 이면 파티가 아니기 때문에 재신청 하도록 해야함
	if( m_kAutoPartyUserInfo.m_kOldPartyInfo.m_iOldHostUnitUID == 0 )
		return true;

	return m_kAutoPartyUserInfo.m_kOldPartyInfo.m_iOldHostUnitUID == iUnitUID;
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}



