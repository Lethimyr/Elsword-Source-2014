#include "NewPartyListManager.h"
#include "NetError.h"
#include "GSFSM.h"
#include "X2Data/XSLPartyManager.h"


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

ImplementSingleton( KPartyListManager );

KPartyListManager::KPartyListManager() :
m_iPlayGamePartyCount(0)
{
}

KPartyListManager::~KPartyListManager()
{
}

ImplToStringW( KPartyListManager )
{
	stm_<< L"=== Party List Manager ===" << std::endl
		<< m_mapPartyInfo.size() << std::endl
		;

	return stm_;
}

KGSPartyPtr KPartyListManager::GetParty( IN UidType iPartyUID )
{
	std::map< UidType, KGSPartyPtr >::iterator mit;
	mit = m_mapPartyInfo.find( iPartyUID );
	if( mit == m_mapPartyInfo.end() )
	{
		START_LOG( cwarn, L"찾는 파티가 없습니다.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KGSPartyPtr();
	}

	if( mit->second == NULL )
	{
		START_LOG( cerr, L"파티 포인터 이상.!" )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return KGSPartyPtr();
	}

	return mit->second;
}

bool KPartyListManager::GetDungeonInfo( IN UidType iPartyUID, OUT int& iDungeonID, OUT char& cDifficultyLevel, OUT char& cDungeonMode )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"던전 정보를 찾을 파티가 없음." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return false;
	}

	iDungeonID		 = spParty->GetDungeonID();
	cDifficultyLevel = spParty->GetDungeonDif();
	cDungeonMode	 = spParty->GetDungeonMode();
	return true;
}

bool KPartyListManager::GetPartyUserList( IN UidType iPartyUID, OUT std::vector< KCommunityUserInfo >& vecPartyUserList )
{
	vecPartyUserList.clear();

	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 정보를 찾을 파티가 없음." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return false;
	}
	
	std::vector< KPartyListUserInfo > vecUserInfo;
	spParty->GetListUserInfoList( vecUserInfo );

	BOOST_TEST_FOREACH( const KPartyListUserInfo&, kPartyListUserInfo, vecUserInfo )
	{
		KCommunityUserInfo kInfo;
		kInfo.m_iUnitUID	 = kPartyListUserInfo.m_iUnitUID;
		kInfo.m_wstrNickName = kPartyListUserInfo.m_wstrNickName;
		kInfo.m_cUnitClass	 = kPartyListUserInfo.m_cUnitClass;
		kInfo.m_ucLevel		 = kPartyListUserInfo.m_ucLevel;
		kInfo.m_cState		 = KCommunityUserInfo::US_DUNGEON_LOUNGE;
		kInfo.m_iStateCode	 = iPartyUID;
		kInfo.m_iRoomUID	 = 0;
		vecPartyUserList.push_back( kInfo );
	}
	return true;
}

bool KPartyListManager::IsPartyMember( IN UidType iPartyUID, IN UidType iUnitUID )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 정보를 찾을 파티가 없음." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return false;
	}
    
	return spParty->IsMember( iUnitUID );
}

bool KPartyListManager::GetPartyNumMember( IN UidType iPartyUID, OUT int& iNumMember )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 정보를 찾을 파티가 없음." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return false;
	}

	iNumMember = spParty->GetNumMember();
	return true;
}

bool KPartyListManager::GetPartyFeverExpRate( IN UidType iPartyUID, OUT float& fAddPartyFeverExpRate )
{
	fAddPartyFeverExpRate = 0.0f;

	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"파티 정보를 찾을 파티가 없음." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return false;
	}
    
	fAddPartyFeverExpRate = spParty->GetPartyFeverExpRate();
	return true;
}

bool KPartyListManager::GetPartyFever( IN const UidType iPartyUID, OUT short& sPartyFever )
{
	sPartyFever = 0;

	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"파티 정보를 찾을 파티가 없음." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;

		return false;
	}
    
	sPartyFever = spParty->GetPartyFever();
	return true;
}

//{{ 2012. 10. 25	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KPartyListManager::CheckInvitePartyConditionForHost( IN KGSUserPtr spHost )
{
	SET_ERROR( NET_OK );

	// 요청자가 dungeon 이나 pvp 플레이 중이라면 실패
	if( spHost->GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( spHost->GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				SET_ERROR( ERR_PARTY_48 );
				return false;
			}
			break;

		case CXSLRoom::RT_PVP:
			{
				SET_ERROR( ERR_PARTY_49 );
				return false;
			}
			break;

		case CXSLRoom::RT_TRAININGCENTER:
			{
				SET_ERROR( ERR_PARTY_56 );
				return false;
			}
			break;
		}
	}

	//{{ 2011. 06. 21	최육사	대전 개편
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
	//////////////////////////////////////////////////////////////////////////

	// 파티초대 대상자가 현재 매칭 신청 했다면 초대 불가능 
	if( spHost->IsRequestMatchMaking() == true )
	{
		SET_ERROR( ERR_MATCH_MAKING_21 );
		return false;
	}

	// 파티 초대 대상자가 매칭 완료상태면 초대 불가능
	if( spHost->GetMatchUID() != 0 )
	{
		SET_ERROR( ERR_MATCH_MAKING_22 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
	//////////////////////////////////////////////////////////////////////////
	
	// 대전 로비에 있는 유저는 초대를 할 수 없다.
	if( spHost->GetRoomListID() != 0 )
	{
		SET_ERROR( ERR_PARTY_57 );
		return false;
	}

	// 자동 파티 신청 대기중일때는 초대 불가능
	if( spHost->GetAutoPartyWaitNumber() != 0 )
	{
		SET_ERROR( ERR_PARTY_58 );
		return false;
	}

	// 자동 파티 매칭이 완료된 상태일때는 초대 불가능
	if( spHost->GetAutoPartyUID() != 0 )
	{
		SET_ERROR( ERR_PARTY_59 );
		return false;
	}

	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//////////////////////////////////////////////////////////////////////////
// 파티 초대가 가능한 유저인지 체크하는 함수
bool KPartyListManager::CheckInvitePartyConditionForReceiver( IN KGSUserPtr spReceiver )
{
	SET_ERROR( NET_OK );

	// GM은 파티에 초대될수 없습니다.
	if( spReceiver->GetAuthLevel() == SEnum::UAL_GM )
	{
		SET_ERROR( ERR_GM_CHAR_DENY_00 );
		return false;
	}

	// 파티 초대 거부 옵션이 켜져있는 유저입니다.
	if( spReceiver->IsDenyInviteParty() )
	{
		SET_ERROR( ERR_PARTY_19 );
		return false;
	}

	//{{ 2011. 06. 21	최육사	대전 개편
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
	//////////////////////////////////////////////////////////////////////////

	// 파티초대 대상자가 현재 매칭 신청 했다면 초대 불가능 
	if( spReceiver->IsRequestMatchMaking() == true )
	{
		SET_ERROR( ERR_MATCH_MAKING_16 );
		return false;
	}

	// 파티 초대 대상자가 매칭 완료상태면 초대 불가능
	if( spReceiver->GetMatchUID() != 0 )
	{
		SET_ERROR( ERR_MATCH_MAKING_17 );
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
	//////////////////////////////////////////////////////////////////////////
	
	// 파티 가입 예약 상태인지 검사 ( 이 부분을 주석 처리하면 중복 초대가 가능해집니다. )
	if( spReceiver->GetCandidatePartyUID() != 0 )
	{
		SET_ERROR( ERR_PARTY_60 );
		return false;
	}

	// 초대할 유저가 다른파티 소속인지 체크..
	if( spReceiver->GetPartyUID() != 0 )
	{
		SET_ERROR( ERR_PARTY_54 );
		return false;
	}

	//{{ 2012. 10. 25	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 대전 로비에 있는 유저는 초대 될수 없다.
	if( spReceiver->GetRoomListID() != 0 )
	{
		SET_ERROR( ERR_PARTY_51 );
		return false;
	}

	// 대전방 또는 던전방에 입장한 유저라면 파티에 초대 될수 없다.
	if( spReceiver->GetRoomUID() > 0 )
	{
		switch( CXSLRoom::GetRoomType( spReceiver->GetRoomUID() ) )
		{
		case CXSLRoom::RT_DUNGEON:
			{
				SET_ERROR( ERR_PARTY_50 );
				return false;
			}
			break;

		case CXSLRoom::RT_PVP:
			{
				SET_ERROR( ERR_PARTY_51 );
				return false;
			}
			break;

		case CXSLRoom::RT_TRAININGCENTER:
			{
				SET_ERROR( ERR_PARTY_07 );
				return false;
			}
			break;
		}
	}

	// 초대 대상자가 다른 파티가입자를 기다리는 상황이라면 안된다!
	if( spReceiver->IsEmptyWaitPartyUser() == false )
	{
		SET_ERROR( ERR_BATTLEFIELD_11 );
		return false;
	}

	// 자동 파티 신청 대기중일때는 초대 불가능
	if( spReceiver->GetAutoPartyWaitNumber() != 0 )
	{
		SET_ERROR( ERR_PARTY_55 );
		return false;
	}

	// 자동 파티 매칭이 완료된 상태일때는 초대 불가능
	if( spReceiver->GetAutoPartyUID() != 0 )
	{
		SET_ERROR( ERR_PARTY_55 );
		return false;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 같은 필드에서 파티 초대하면서 파티 생성할때
bool KPartyListManager::E_CreatePartyInSameField( IN KGSUserPtr spHost, IN KGSUserPtr spReceiver )
{
	SET_ERROR( NET_OK );

	if( spHost->GetPartyUID() != 0 )
	{
		START_LOG( cerr, L"파티 정보가 존재하면 안됩니다!" )
			<< BUILD_LOG( spHost->GetPartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_10 );
		return false;
	}

	// 파티 가입 예약 상태인지 검사 ( 이 부분을 주석 처리하면 중복 초대가 가능해집니다. )
	if( spHost->GetCandidatePartyUID() != 0 )
	{
		START_LOG( cerr, L"파티 예약 정보가 존재하면 안됩니다!" )
			<< BUILD_LOG( spHost->GetCandidatePartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_PARTY_60 );
		return false;
	}

	// 파티 초대 가능한지 조건 검사
	if( CheckInvitePartyConditionForReceiver( spReceiver ) == false )
	{
		//NetError::GetLastError();
		return false;
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 이미 파티는 생성되어 있는 상태고 같은 필드에 있는 유저를 파티 초대할때
bool KPartyListManager::E_InvitePartyInSameField( IN KGSUserPtr spHost, IN KGSUserPtr spReceiver )
{
	SET_ERROR( NET_OK );

	if( spHost->GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 정보가 존재하지 않습니다." )
			<< BUILD_LOG( spHost->GetPartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_12 );
		return false;
	}

	// 파티 가입 예약 상태인지 검사 ( 이 부분을 주석 처리하면 중복 초대가 가능해집니다. )
	if( spHost->GetCandidatePartyUID() != 0 )
	{
		START_LOG( cerr, L"파티 예약 정보가 존재하면 안됩니다!" )
			<< BUILD_LOG( spHost->GetCandidatePartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_PARTY_60 );
		return false;
	}

	KGSPartyPtr spParty = GetParty( spHost->GetPartyUID() );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 정보가 존재하지 않습니다." )
			<< BUILD_LOG( spHost->GetPartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_12 );
		return false;
	}

	// 파티 입장 가능 인원 수 확인
	if( spParty->GetNumMember() == CXSLPartyManager::PE_MAX_PARTY_NUM )
	{
		SET_ERROR( ERR_PARTY_52 );
		return false;
	}

	// 파티 초대 가능한지 조건 검사
	if( CheckInvitePartyConditionForReceiver( spReceiver ) == false )
	{
		//NetError::GetLastError()
		return false;
	}	

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 다른 필드에서 초대와 함께 파티 생성 및 파티 초대자 체크
bool KPartyListManager::E_CreatePartyFarFieldCheckHost(	IN KGSUserPtr spHost, IN const std::wstring& wstrReceiverNickName )
{
	SET_ERROR( NET_OK );

	if( spHost->GetPartyUID() != 0 )
	{
		START_LOG( cerr, L"파티 정보가 존재하면 안됩니다!" )
			<< BUILD_LOG( spHost->GetPartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_10 );
		return false;
	}

	// 파티 가입 예약 상태인지 검사 ( 이 부분을 주석 처리하면 중복 초대가 가능해집니다. )
	if( spHost->GetCandidatePartyUID() != 0 )
	{
		START_LOG( cerr, L"파티 대기자 정보가 존재하면 안됩니다!" )
			<< BUILD_LOG( spHost->GetCandidatePartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_PARTY_60 );
		return false;
	}

	// 파티 초대장을 날렸는지 체크!
	//if( spHost->IsEmptyWaitPartyUser() == false )
	//{
	//	kAck.m_iOK = NetError::ERR_UNKNOWN;
	//	spHost->SendPacket( EGS_INVITE_PARTY_ACK, kAck );
	//	return false;
	//}

	// 파티 초대 타이머 가동
	//LIF( spHost->RegWaitPartyUser( spHost->GetCharUID() ) );

	// 파티 매니저로 가자!
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 다른 필드에서 자신의 파티에 초대 및 파티 초대자 체크
bool KPartyListManager::E_InvitePartyFarFieldCheckHost( IN KGSUserPtr spHost, IN const std::wstring& wstrReceiverNickName )
{
	SET_ERROR( NET_OK );

	if( spHost->GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 정보가 없는데 초대하려고 했습니다." )
			<< BUILD_LOG( spHost->GetPartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_10 );
		return false;
	}

	// 파티 가입 예약 상태인지 검사 ( 이 부분을 주석 처리하면 중복 초대가 가능해집니다. )
	if( spHost->GetCandidatePartyUID() != 0 )
	{
		START_LOG( cerr, L"파티 대기자 정보가 존재하면 안됩니다!" )
			<< BUILD_LOG( spHost->GetCandidatePartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_PARTY_60 );
		return false;
	}

	KGSPartyPtr spParty = GetParty( spHost->GetPartyUID() );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 정보가 존재하지 않습니다." )
			<< BUILD_LOG( spHost->GetPartyUID() )
			<< END_LOG;

		SET_ERROR( ERR_BATTLEFIELD_12 );
		return false;
	}

	// 파티 입장 가능 인원 수 확인
	if( spParty->GetNumMember() == CXSLPartyManager::PE_MAX_PARTY_NUM )
	{
		SET_ERROR( ERR_PARTY_52 );
		return false;
	}

	// 파티 초대장을 날렸는지 체크!
	//if( spHost->IsEmptyWaitPartyUser() == false )
	//{
	//	kAck.m_iOK = NetError::ERR_UNKNOWN;
	//	spHost->SendPacket( EGS_INVITE_PARTY_ACK, kAck );
	//	return false;
	//}

	//// 파티 초대 타이머 가동
	//LIF( spHost->RegWaitPartyUser( spHost->GetCharUID() ) );
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 닉네임으로 상대방을 초대하려고 할때
bool KPartyListManager::E_InvitePartyFarFieldCheckReceiver( IN KGSUserPtr spReceiver, IN const KELG_INVITE_PARTY_FIND_RECEIVER_REQ& kReq )
{
	SET_ERROR( NET_OK );

	// 파티 초대 가능한지 조건 검사
	if( CheckInvitePartyConditionForReceiver( spReceiver ) == false )
	{
		//NetError::GetLastError();
		return false;
	}

	// 정상적으로 초대장을 날렸다!
	return true;
}

bool KPartyListManager::E_InvitePartyReply( IN KGSUserPtr spUser, IN const KEGS_INVITE_PARTY_REPLY_NOT& kNot )
{
	KEGS_JOIN_PARTY_NOT kJoinNot;
	kJoinNot.m_iOK = NetError::NET_OK;

	KEPM_INVITE_PARTY_REPLY_NOT kPacket;
	kPacket.m_iPartyUID = kNot.m_iPartyUID;
	kPacket.m_cReplyReason = kNot.m_cReplyReason;
	spUser->GetPartyUserInfo( kPacket.m_kPartyUserInfo );
	

	// 응답을 보낸 당사자가 이미 다른 파티에 가입이 된 상태라면 자동으로 초대 거부를 한다!
	if( spUser->GetPartyUID() > 0  &&  kNot.m_cReplyReason == KEGS_INVITE_PARTY_REPLY_NOT::RR_ACCEPT )
	{	
		kJoinNot.m_iOK = NetError::ERR_PARTY_12;
		kPacket.m_cReplyReason = KEGS_INVITE_PARTY_REPLY_NOT::RR_NOT_ACCEPT;
		goto end_proc;
	}

	// 이미 초대장을 받은 상태라면?
	//if( spUser->IsEmptyWaitPartyUser() == false )
	//{
	//	// 그 초대장의 주인이 맞다면?
	//	if( spUser->IsWaitPartyUser( kNot.m_iHostUnitUID ) == false )
	//	{
	//		KEGS_JOIN_PARTY_NOT kJoinNot;
	//		kJoinNot.m_iOK = NetError::ERR_PARTY_12;
	//		spUser->SendPacket( EGS_JOIN_PARTY_NOT, kJoinNot );
	//		return true;
	//	}
	//}

	//{{ 2011. 07. 27	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	// 공식 대전 신청을 해놓은 상태라면 자동으로 초대 거부한다.
	if( spUser->IsRequestMatchMaking() == true )
	{
		kJoinNot.m_iOK = NetError::ERR_MATCH_MAKING_18;
		kPacket.m_cReplyReason = KEGS_INVITE_PARTY_REPLY_NOT::RR_NOT_ACCEPT;
		goto end_proc;
	}

	// 공식 대전 매칭이 된 상태라면 자동으로 초대 거부한다.
	if( spUser->GetMatchUID() != 0 )
	{
		kJoinNot.m_iOK = NetError::ERR_MATCH_MAKING_19;
		kPacket.m_cReplyReason = KEGS_INVITE_PARTY_REPLY_NOT::RR_NOT_ACCEPT;
		goto end_proc;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2013. 01. 07	최육사	던전에 5명이상 입장한것처럼 보이는 버그 수정
#ifdef SERV_ABNORMAL_DUNGEON_PARTY_MEMBER_COUNT_BUG_FIX
	// 초대 대상자가 다른 파티가입자를 기다리는 상황이라면 안된다!
	if( spUser->IsEmptyWaitPartyUser() == false )
	{
		kJoinNot.m_iOK = NetError::ERR_BATTLEFIELD_11;
		kPacket.m_cReplyReason = KEGS_INVITE_PARTY_REPLY_NOT::RR_NOT_ACCEPT;
		goto end_proc;
	}

	// 자동 파티 신청 대기중일때는 초대 불가능
	if( spUser->GetAutoPartyWaitNumber() != 0 )
	{
		kJoinNot.m_iOK = NetError::ERR_PARTY_55;
		kPacket.m_cReplyReason = KEGS_INVITE_PARTY_REPLY_NOT::RR_NOT_ACCEPT;
		goto end_proc;
	}

	// 자동 파티 매칭이 완료된 상태일때는 초대 불가능
	if( spUser->GetAutoPartyUID() != 0 )
	{
		kJoinNot.m_iOK = NetError::ERR_PARTY_55;
		kPacket.m_cReplyReason = KEGS_INVITE_PARTY_REPLY_NOT::RR_NOT_ACCEPT;
		goto end_proc;
	}
#endif SERV_ABNORMAL_DUNGEON_PARTY_MEMBER_COUNT_BUG_FIX
	//}}

end_proc:
	// 에러인 경우 보낸 응답을 보낸 당사자에게도 알린다.
	if( kJoinNot.m_iOK != NetError::NET_OK )
	{
		spUser->SendPacket( EGS_JOIN_PARTY_NOT, kJoinNot );
	}

	// 초대를 받은 파티로 보고하기
	spUser->_SendToCnParty( kNot.m_iPartyUID, EPM_INVITE_PARTY_REPLY_NOT, kPacket );
	return true;
}

bool KPartyListManager::E_LeavePartyReq( IN KGSUserPtr spUser, IN const KEGS_LEAVE_PARTY_REQ& kReq )
{
	UidType iLeavePartyUID = 0;

	// 파티 소속인지..?
	if( spUser->GetPartyUID() > 0 )
	{
		// 파티 가입 예약인지..?
		if( spUser->GetCandidatePartyUID() > 0  &&  spUser->GetPartyUID() != spUser->GetCandidatePartyUID() )
		{
			START_LOG( cwarn, L"파티 소속인데다 예약도 되있네? 일어나서는 안되는 에러!" )
				<< BUILD_LOG( spUser->GetPartyUID() )
				<< BUILD_LOG( spUser->GetCandidatePartyUID() )
				<< END_LOG;

			// 파티 자동 이탈 처리
			KEPM_LEAVE_PARTY_REQ kPacketReq;
			kPacketReq.m_iPartyUID = spUser->GetCandidatePartyUID();
			kPacketReq.m_iReason = NetError::NOT_LEAVE_PARTY_REASON_05;
			spUser->_SendToCnParty( spUser->GetCandidatePartyUID(), EPM_LEAVE_PARTY_REQ, kPacketReq );
		}

		iLeavePartyUID = spUser->GetPartyUID();
	}
	else
	{
		// 파티 예약자인지..?
		if( spUser->GetCandidatePartyUID() > 0 )
		{
			iLeavePartyUID = spUser->GetCandidatePartyUID();
		}
		else
		{
			START_LOG( cerr, L"파티 이탈 하려고하는데 PartyUID가 없다?" )
				<< BUILD_LOG( spUser->GetPartyUID() )
				<< BUILD_LOG( spUser->GetCandidatePartyUID() )
				<< END_LOG;

			return false;
		}
	}

	KEPM_LEAVE_PARTY_REQ kPacket;
	kPacket.m_iPartyUID = iLeavePartyUID;
	kPacket.m_iReason = kReq.m_iReason;
	spUser->_SendToCnParty( iLeavePartyUID, EPM_LEAVE_PARTY_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_UpdatePartyUserInfo( IN KGSUserPtr spUser, IN bool bChangeField )
{
	KEPM_UPDATE_PARTY_USER_INFO_NOT kNot;
	kNot.m_iPartyUID	= spUser->GetPartyUID();
	kNot.m_bChangeField = bChangeField;
	kNot.m_iUnitUID		= spUser->GetCharUID();
	kNot.m_cUnitClass	= spUser->GetUnitClass();
	kNot.m_ucLevel		= spUser->GetLevel();
	kNot.m_cState		= spUser->GetUnitState();	
	if( kNot.m_cState == CXSLUnit::CUS_FIELD_MAP )
	{
		kNot.m_iStateCode = spUser->GetMapID();
	}
	else
	{
		kNot.m_iStateCode = 0;
	}

	spUser->SendToCnParty( EPM_UPDATE_PARTY_USER_INFO_NOT, kNot );
	return true;
}

bool KPartyListManager::E_ChangeReadyReq( IN KGSUserPtr spUser, IN bool bReady )
{
	KEPM_PARTY_CHANGE_READY_REQ kPacket;
	kPacket.m_iPartyUID = spUser->GetPartyUID();
	kPacket.m_bReady = bReady;
    spUser->SendToCnParty( EPM_PARTY_CHANGE_READY_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_ChangeDungeon( IN KGSUserPtr spUser, IN const KEGS_PARTY_CHANGE_DUNGEON_REQ& kReq )
{
	KEPM_PARTY_CHANGE_DUNGEON_REQ kPacket;
	kPacket.m_iPartyUID		  = spUser->GetPartyUID();
	kPacket.m_iDungeonID	  = kReq.m_iDungeonID;
	kPacket.m_DifficultyLevel = kReq.m_DifficultyLevel;
	kPacket.m_cDungeonMode	  = kReq.m_cDungeonMode;
	spUser->SendToCnParty( EPM_PARTY_CHANGE_DUNGEON_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_GameStart( IN KGSUserPtr spHost, IN const KEGS_QUICK_START_DUNGEON_GAME_REQ& kReq )
{
	KEPM_PARTY_GAME_START_REQ kPacket;
	kPacket.m_iPartyUID = spHost->GetPartyUID();
	//kPacket.m_sWorldID = kReq.m_sWorldID;
	kPacket.m_usEventID = ERM_PARTY_OPEN_DUNGEON_ROOM_REQ;
	spHost->SendToCnParty( EPM_PARTY_GAME_START_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_EndGame( IN KGSUserPtr spHost )
{
	KEPM_END_GAME_NOT kPacket;
	kPacket.m_iPartyUID = spHost->GetPartyUID();
	spHost->SendToCnParty( EPM_END_GAME_NOT, kPacket );
	return true;
}

bool KPartyListManager::E_ChangeHost( IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_HOST_REQ& kReq )
{
	KEPM_PARTY_CHANGE_HOST_REQ kPacket;
	kPacket.m_iPartyUID = spHost->GetPartyUID();
	kPacket.m_iNewHostUnitUID = kReq;
	spHost->SendToCnParty( EPM_PARTY_CHANGE_HOST_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_ChangeNumOfPer( IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_NUM_OF_PER_REQ& kReq )
{
	KEPM_PARTY_CHANGE_NUM_OF_PER_REQ kPacket;
	kPacket.m_iPartyUID = spHost->GetPartyUID();
	kPacket.m_cNumOfPer = kReq;
	spHost->SendToCnParty( EPM_PARTY_CHANGE_NUM_OF_PER_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_ChangeGetItemtype( IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ& kReq )
{
	KEPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ kPacket;
	kPacket.m_iPartyUID = spHost->GetPartyUID();
	kPacket.m_cGetItemType = kReq;
	spHost->SendToCnParty( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_BanUser( IN KGSUserPtr spHost, IN const KEGS_PARTY_BAN_USER_REQ& kReq )
{
	KEPM_PARTY_BAN_USER_REQ kPacket;
	kPacket.m_iPartyUID = spHost->GetPartyUID();
	kPacket.m_iUnitUID = kReq;
	spHost->SendToCnParty( EPM_PARTY_BAN_USER_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_Chat(	IN KGSUserPtr spUser, IN const KEGS_CHAT_REQ& kReq )
{
	KEPM_CHAT_REQ kPacket;
	kPacket.m_iPartyUID = spUser->GetPartyUID();
	kPacket.m_cChatPacketType = kReq.m_cChatPacketType;
	kPacket.m_cRoomChatType = kReq.m_cRoomChatType;
	kPacket.m_ToUnitUID = kReq.m_ToUnitUID;
	kPacket.m_wstrRecieverUnitNickName = kReq.m_wstrRecieverUnitNickName;
	kPacket.m_wstrMsg = kReq.m_wstrMsg;
	spUser->SendToCnParty( EPM_CHAT_REQ, kPacket );
	return true;
}

bool KPartyListManager::E_IncreasePartyFever( IN const KERM_INCREASE_PARTY_FEVER_NOT& kNot )
{
	SendToCnParty( kNot.m_iPartyUID, EPM_INCREASE_PARTY_FEVER_NOT, kNot );
	return true;
}

//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KPartyListManager::E_MaxPartyFever( IN const KERM_MAX_PARTY_FEVER_NOT& kNot )
{
	SendToCnParty( kNot.m_iPartyUID, EPM_MAX_PARTY_FEVER_NOT, kNot );
	return true;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

bool KPartyListManager::E_DungeonSecretStageEnterCheck( IN KGSUserPtr spUser )
{
    KEPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ kPacket;
	kPacket.m_iPartyUID = spUser->GetPartyUID();
	spUser->SendToCnParty( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ, kPacket );
	return true;
}
//}}

void KPartyListManager::E_OpenPartyList( IN const KPartyInfo& kPartyInfo, IN const std::vector< KPartyUserInfo >& vecPartyUser )
{
	// 이미 존재하는 파티인지 검사
	if( m_mapPartyInfo.find( kPartyInfo.m_iPartyUID ) != m_mapPartyInfo.end() )
	{
		START_LOG( cerr, L"이미 파티리스트에 해당 파티정보가 있습니다! 유령파티인가?" )
			<< BUILD_LOG( kPartyInfo.m_iPartyUID )
			<< END_LOG;
        
		// 파티 정보 삭제
		E_ClosePartyList( kPartyInfo.m_iPartyUID );
	}

	// 파티 정보 생성
	KGSPartyPtr spParty( new KGSParty( kPartyInfo ) );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 정보 생성 실패! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kPartyInfo.m_wstrPartyName )
			<< BUILD_LOG( vecPartyUser.size() )
			<< END_LOG;
		return;
	}

	// 파티 멤버 등록
	std::vector< KPartyUserInfo >::const_iterator vitNP;
	for( vitNP = vecPartyUser.begin(); vitNP != vecPartyUser.end(); ++vitNP )
	{
		spParty->AddMember( *vitNP );
	}


	// 파티 정보 컨테이너에 추가
	m_mapPartyInfo.insert( std::make_pair( spParty->GetPartyUID(), spParty ) );
}

void KPartyListManager::E_ClosePartyList( IN const UidType iPartyUID )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"삭제하려는 파티가 없습니다." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return;
	}

    // 파티 리스트에서 삭제한다.
	E_DelPartyList( iPartyUID );

	// map컨테이너에서 삭제
	m_mapPartyInfo.erase( iPartyUID );
}

bool KPartyListManager::E_AddPartyList( IN const UidType iPartyUID )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"파티 리스트에 추가하려는 파티가 없습니다." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return false;
	}

	const int iDungeonID = spParty->GetDungeonID();

	//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	if( CXSLDungeon::IsValentineDungeon( iDungeonID ) == true )
	{
		return true;
	}
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	if( CXSLDungeon::IsValentineDungeonInt( iDungeonID ) == true )
	{
		return true;
	}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / 강정훈
	if( CXSLDungeon::IsHalloweenDungeon( iDungeonID ) == true )
	{
		return true;
	}
#endif //SERV_HALLOWEEN_EVENT_2013

	const int iMapID = CXSLMapData::GetMapID( iDungeonID );
	if( iMapID == SEnum::VMI_INVALID )
	{
		START_LOG( cerr, L"파티리스트 [추가]할려니 던전아이디가 이상함." )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
		return false;
	}

	//{{ 2010. 03. 26  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	// 일반 던전과 비밀던전 구분하여 리스트 추가
	if( CXSLDungeon::IsSecretDungeon( iDungeonID ) == true )
	{
        return AddSecretDungeonPartyList( spParty, iDungeonID, iMapID );
	}
	else
#endif SERV_SECRET_HELL
	//}}
	{
        return AddNormalDungeonPartyList( spParty, iDungeonID, iMapID );
	}
}

bool KPartyListManager::E_DelPartyList( IN const UidType iPartyUID )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"삭제하려는 파티가 없습니다." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return false;
	}

	const int iDungeonID = spParty->GetDungeonID();

	//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	if( CXSLDungeon::IsValentineDungeon( iDungeonID ) == true )
	{
		return true;
	}
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}
#ifdef SERV_EVENT_VALENTINE_DUNGEON_INT
	if( CXSLDungeon::IsValentineDungeonInt( iDungeonID ) == true )
	{
		return true;
	}
#endif SERV_EVENT_VALENTINE_DUNGEON_INT

#ifdef SERV_HALLOWEEN_EVENT_2013 // 2013.10.14 / 강정훈
	if( CXSLDungeon::IsHalloweenDungeon( iDungeonID ) == true )
	{
		return true;
	}
#endif //SERV_HALLOWEEN_EVENT_2013

	const int iMapID = CXSLMapData::GetMapID( iDungeonID );

	if( iMapID == SEnum::VMI_INVALID )
	{
		START_LOG( cerr, L"파티리스트 [삭제]할려니 던전아이디가 이상함." )
			<< BUILD_LOG( spParty->GetPartyName() )
			<< BUILD_LOG( spParty->GetNumMember() )
			<< BUILD_LOG( iDungeonID )
			<< END_LOG;
		return false;
	}

	//{{ 2010. 03. 26  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	// 일반 던전과 비밀던전 구분하여 리스트 삭제
	if( CXSLDungeon::IsSecretDungeon( iDungeonID ) == true )
	{
		return DelSecretDungeonPartyList( spParty, iDungeonID, iMapID );
	}
	else
#endif SERV_SECRET_HELL
	//}}
	{
		return DelNormalDungeonPartyList( spParty, iDungeonID, iMapID );
	}	
}

bool KPartyListManager::AddNormalDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID )
{	
	bool bAddRet = false; // 추가 성공 여부 확인

	std::map< int, std::vector< KGSPartyPtr > >::iterator mit;

	//## 1. 던전별 리스트 먼저 처리.
	mit = m_mapPartyList[PLT_DUNGEON].find( iDungeonID );

	//없을경우..
	if( mit == m_mapPartyList[PLT_DUNGEON].end() )
	{
		m_mapPartyList[PLT_DUNGEON][iDungeonID].push_back( spParty );
		bAddRet = true;
	}
	//있을경우.
	else
	{
		//안에 데이터가 없을경우만 넣는다.
		if( std::find( m_mapPartyList[PLT_DUNGEON][iDungeonID].begin(), m_mapPartyList[PLT_DUNGEON][iDungeonID].end(), spParty ) == m_mapPartyList[PLT_DUNGEON][iDungeonID].end() )
		{
			m_mapPartyList[PLT_DUNGEON][iDungeonID].insert( m_mapPartyList[PLT_DUNGEON][iDungeonID].begin(), spParty );
			bAddRet = true;
		}
	}

	//## 2. 던전게이트별 리스트 처리.
	mit = m_mapPartyList[PLT_GATE_TOTAL].find( iMapID );

	//없을경우..
	if( mit == m_mapPartyList[PLT_GATE_TOTAL].end() )
	{
		m_mapPartyList[PLT_GATE_TOTAL][iMapID].push_back(spParty);
		bAddRet = true;
	}
	//있을경우.
	else
	{
		//안에 데이터가 없을경우만 넣는다.
		if( std::find( m_mapPartyList[PLT_GATE_TOTAL][iMapID].begin(), m_mapPartyList[PLT_GATE_TOTAL][iMapID].end(), spParty ) == m_mapPartyList[PLT_GATE_TOTAL][iMapID].end() )
		{
			m_mapPartyList[PLT_GATE_TOTAL][iMapID].insert( m_mapPartyList[PLT_GATE_TOTAL][iMapID].begin(), spParty );
			bAddRet = true;
		}
	}

	return bAddRet;
}

//{{ 2010. 03. 26  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL

bool KPartyListManager::AddSecretDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID )
{
	bool bAddRet = false; // 추가 성공 여부 확인

	std::map< int, std::vector< KGSPartyPtr > >::iterator mit;

	//## 1. 던전별 리스트 먼저 처리.
	mit = m_mapPartyList[PLT_SECRET_DUNGEON].find( iDungeonID );

	//없을경우..
	if( mit == m_mapPartyList[PLT_SECRET_DUNGEON].end() )
	{
		m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].push_back( spParty );
		bAddRet = true;
	}
	//있을경우.
	else
	{
		//안에 데이터가 없을경우만 넣는다.
		if( std::find( m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].begin(), m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].end(), spParty ) == m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].end() )
		{
			m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].insert( m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].begin(), spParty );
			bAddRet = true;
		}
	}

	//## 2. 던전게이트별 리스트 처리.
	mit = m_mapPartyList[PLT_GATE_TOTAL].find( iMapID );

	//없을경우..
	if( mit == m_mapPartyList[PLT_GATE_TOTAL].end() )
	{
		m_mapPartyList[PLT_GATE_TOTAL][iMapID].push_back(spParty);
		bAddRet = true;
	}
	//있을경우.
	else
	{
		//안에 데이터가 없을경우만 넣는다.
		if( std::find( m_mapPartyList[PLT_GATE_TOTAL][iMapID].begin(), m_mapPartyList[PLT_GATE_TOTAL][iMapID].end(), spParty ) == m_mapPartyList[PLT_GATE_TOTAL][iMapID].end() )
		{
			m_mapPartyList[PLT_GATE_TOTAL][iMapID].insert( m_mapPartyList[PLT_GATE_TOTAL][iMapID].begin(), spParty );
			bAddRet = true;
		}
	}

	return bAddRet;
}

bool KPartyListManager::DelSecretDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID )
{
	bool bDelRet = false; // 삭제 성공 여부 확인

	std::map< int, std::vector< KGSPartyPtr > >::iterator mit;

	//## 1. 던전별 리스트 먼저 처리.
	mit = m_mapPartyList[PLT_SECRET_DUNGEON].find( iDungeonID );

	if( mit != m_mapPartyList[PLT_SECRET_DUNGEON].end() )
	{
		std::vector< KGSPartyPtr >::iterator vit = std::find( m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].begin(), m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].end(), spParty );
		if( vit != m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].end() )
		{
			m_mapPartyList[PLT_SECRET_DUNGEON][iDungeonID].erase(vit);
			bDelRet = true;
		}
	}

	//## 2. 던전게이트별 리스트 처리.
	mit = m_mapPartyList[PLT_GATE_TOTAL].find( iMapID );

	if( mit != m_mapPartyList[PLT_GATE_TOTAL].end() )
	{
		std::vector< KGSPartyPtr >::iterator vit = std::find( m_mapPartyList[PLT_GATE_TOTAL][iMapID].begin(), m_mapPartyList[PLT_GATE_TOTAL][iMapID].end(), spParty );
		if( vit != m_mapPartyList[PLT_GATE_TOTAL][iMapID].end() )
		{
			m_mapPartyList[PLT_GATE_TOTAL][iMapID].erase(vit);
			bDelRet = true;
		}
	}

	return bDelRet;
}

#endif SERV_SECRET_HELL
//{{ 2010. 03. 26  최육사	비밀던전 헬모드

bool KPartyListManager::DelNormalDungeonPartyList( IN KGSPartyPtr spParty, IN const int iDungeonID, IN const int iMapID )
{
	bool bDelRet = false; // 삭제 성공 여부 확인

	std::map< int, std::vector< KGSPartyPtr > >::iterator mit;

	//## 1. 던전별 리스트 먼저 처리.
	mit = m_mapPartyList[PLT_DUNGEON].find( iDungeonID );

	if( mit != m_mapPartyList[PLT_DUNGEON].end() )
	{
		std::vector< KGSPartyPtr >::iterator vit = std::find( m_mapPartyList[PLT_DUNGEON][iDungeonID].begin(), m_mapPartyList[PLT_DUNGEON][iDungeonID].end(), spParty );
		if( vit != m_mapPartyList[PLT_DUNGEON][iDungeonID].end() )
		{
			m_mapPartyList[PLT_DUNGEON][iDungeonID].erase(vit);
			bDelRet = true;
		}
	}

	//## 2. 던전게이트별 리스트 처리.
	mit = m_mapPartyList[PLT_GATE_TOTAL].find( iMapID );

	if( mit != m_mapPartyList[PLT_GATE_TOTAL].end() )
	{
		std::vector< KGSPartyPtr >::iterator vit = std::find( m_mapPartyList[PLT_GATE_TOTAL][iMapID].begin(), m_mapPartyList[PLT_GATE_TOTAL][iMapID].end(), spParty );
		if( vit != m_mapPartyList[PLT_GATE_TOTAL][iMapID].end() )
		{
			m_mapPartyList[PLT_GATE_TOTAL][iMapID].erase(vit);
			bDelRet = true;
		}
	}

	return bDelRet;
}

void KPartyListManager::E_UpdatePartyInfo( IN const KPartyInfo& kPartyInfo )
{
	KGSPartyPtr spParty = GetParty( kPartyInfo.m_iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"업데이트 하려는 파티가 없습니다." )
			<< BUILD_LOG( kPartyInfo.m_iPartyUID )
			<< END_LOG;
		return;
	}

	spParty->SetInfo( kPartyInfo );
}

//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KPartyListManager::E_UpdatePartyFever(	IN const UidType iPartyUID, IN const short sPartyFever )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"업데이트 하려는 파티가 없습니다." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return;
	}

	spParty->SetPartyFever( sPartyFever );
}

void KPartyListManager::E_EndGamePartyRegroup( IN const UidType iPartyUID )
{
	SendToCnParty( iPartyUID, EPM_END_GAME_REGROUP_PARTY_NOT, char() );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KPartyListManager::E_AddPartyMemberInPartyList( IN UidType iPartyUID, IN const KPartyUserInfo& kInfo )
{
    KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"파티원을 등록할 파티정보가 없습니다." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return;
	}

	// 멤버 정보 등록
	spParty->AddMember( kInfo );
}

void KPartyListManager::E_DelPartyMemberInPartyList( IN UidType iPartyUID, IN UidType iUnitUID )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"파티원을 삭제할 파티정보가 없습니다." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return;
	}

	// 멤버 정보 삭제
	spParty->DeleteMember( iUnitUID );
}

void KPartyListManager::E_UpdatePartyMemberInfo( IN UidType iPartyUID, IN const std::vector< KPartyUserInfo >& vecPartyUserInfo )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		START_LOG( cwarn, L"파티원 정보를 업데이트할 파티정보가 없습니다." )
			<< BUILD_LOG( iPartyUID )
			<< END_LOG;
		return;
	}
    
	// 파티원 정보 업데이트
	std::vector< KPartyUserInfo >::const_iterator vit;
	for( vit = vecPartyUserInfo.begin(); vit != vecPartyUserInfo.end(); ++vit )
	{
		spParty->UpdatePartyUserInfo( *vit );
	}
}

int KPartyListManager::DelPartyInfo( IN UidType iCnServerUID )
{
	std::vector< UidType > vecDelPartyList;

	std::map< UidType, KGSPartyPtr >::iterator	mit;
	for( mit = m_mapPartyInfo.begin(); mit != m_mapPartyInfo.end(); ++mit )
	{
		if( KncUid::ExtractServerID( mit->first ) == iCnServerUID )	// 해당 센터에서 만들어졌던 파티인 경우
		{
			vecDelPartyList.push_back( mit->first );
		}
	}

	std::vector< UidType >::const_iterator vit;
	for( vit = vecDelPartyList.begin(); vit != vecDelPartyList.end(); ++vit )
	{
        E_ClosePartyList( *vit );
	}

	return static_cast<int>(vecDelPartyList.size());
}

template < class T >
void KPartyListManager::SendToCnParty( UidType iPartyUID, unsigned short usEventID )
{
	SendToCnParty( iPartyUID, usEventID, char() );
}

//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
bool KPartyListManager::IsPossibleLowLevel( IN KGSUserPtr spUser, IN KGSPartyPtr spParty )
{
	if( spParty->GetCheckLowLevel() == true )
	{
		int iDungeonID = spParty->GetDungeonID();
		int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( iDungeonID + static_cast<int>(spParty->GetDungeonDif()) );

		if( spUser->GetLevel() < (iDungeonMinLevel - 2) )
		{
			return false;
		}
	}

	return true;
}

bool KPartyListManager::IsPossibleLowLevel( IN KGSUserPtr spUser, IN UidType iPartyUID, OUT bool isPartyNULL )
{
	KGSPartyPtr spParty = GetParty( iPartyUID );
	if( spParty == NULL )
	{
		isPartyNULL = true;
		return false;
	}

	return IsPossibleLowLevel( spUser, spParty );
}

bool KPartyListManager::E_ChangeCheckMinLevel( IN KGSUserPtr spHost, IN const KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ& kReq )
{
	KEPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ kPacket;
	kPacket.m_iPartyUID = spHost->GetPartyUID();
	kPacket.m_bCheck = kReq;
	spHost->SendToCnParty( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ, kPacket );
	return true;
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 06. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
bool KPartyListManager::E_ChangePartyType( IN KGSUserPtr spHost, IN const KEGS_CHANGE_PARTY_TYPE_REQ& kReq )
{
	spHost->SendToCnParty( EPM_CHANGE_PARTY_TYPE_REQ, kReq );
    return true;
}

bool KPartyListManager::E_ChangePvpMatchInfo( IN KGSUserPtr spHost, IN const KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ& kReq )
{
	spHost->SendToCnParty( EPM_PVP_PARTY_CHANGE_MATCH_INFO_REQ, kReq );
	return true;
}
#endif SERV_PVP_NEW_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


