#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "Socket/NetCommon.h"

#include "BaseServer.h"
#include "UserManager.h"
#include "GameSysVal/GameSysVal.h"
#include "X2Data/XSLPartyManager.h"

//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

#include "Enum/Enum.h"
#include "odbc/Odbc.h"

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}


#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_INVITE_PARTY_REQ )
{
#ifdef SERV_REMOVE_REPEAT_FILTER_IN_EGS_INVITE_PARTY_REQ
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_REQ, EGS_INVITE_PARTY_ACK );
#else SERV_REMOVE_REPEAT_FILTER_IN_EGS_INVITE_PARTY_REQ
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_REQ, EGS_INVITE_PARTY_ACK );
#endif SERV_REMOVE_REPEAT_FILTER_IN_EGS_INVITE_PARTY_REQ

	KEGS_INVITE_PARTY_ACK kAck;
	kAck.m_iReceiverUID = kPacket_.m_iReceiverUID;
	kAck.m_iOK = NetError::NET_OK;

	// 데이터 검증
	if( kPacket_.m_iReceiverUID <= 0  &&  kPacket_.m_wstrNickName.empty() )
	{
		START_LOG( cerr, L"초대할려는 캐릭터 정보가 이상합니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	// valid한 문자열인지 검사
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) )
	{
		kAck.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	// 파티 초대자가 초대할 수 있는 조건이 되는지 검사
	if( SiKPartyListManager()->CheckInvitePartyConditionForHost( GetThisPtr<KGSUser>() ) == false )
	{
		kAck.m_iOK = NetError::GetLastError();
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 직접 UnitUID가 오는 경우는 같은 필드에서 해당 유닛을 직접 찝었을때 가능한것이다.
	if( kPacket_.m_iReceiverUID > 0 )
	{
		// 같은 서버에 해당 유저가 있는지 확인하자
		KGSUserPtr spReceiver = GetKUserManager()->GetByCID( kPacket_.m_iReceiverUID );
		if( IS_NOT_NULL( spReceiver ) )
		{
			// 파티 초대자가 파티소속이 아니라면?
			if( GetPartyUID() == 0 )
			{
				// 초대를 하면서 파티를 생성하는 동작을 한다!
				if( SiKPartyListManager()->E_CreatePartyInSameField( GetThisPtr<KGSUser>(), spReceiver ) == false )
				{
					START_LOG( cwarn, L"같은 필드에 있는 유저를 함께 파티 생성 실패!" )
						<< BUILD_LOG( GetPartyUID() )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( spReceiver->GetCharName() )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					SendPacket( EGS_INVITE_PARTY_ACK, kAck );
					return;
				}
				else
				{
					// 직접 해당 유저에게 초대장을 날리자!
					KEPM_CREATE_PARTY_BY_INVITE_REQ kPacketNot;
					kPacketNot.m_cAuthLevel = GetAuthLevel();
					kPacketNot.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;
					GetPartyUserInfo( kPacketNot.m_kHostPartyUserInfo );
					spReceiver->GetPartyUserInfo( kPacketNot.m_kReceiverPartyUserInfo );
					SendToCnParty( EPM_CREATE_PARTY_BY_INVITE_REQ, kPacketNot );
					return;
				}
			}
			// 파티 초대자가 이미 파티소속이라면?
			else
			{
				// 그냥 파티 초대 동작을 한다!
				if( SiKPartyListManager()->E_InvitePartyInSameField( GetThisPtr<KGSUser>(), spReceiver ) == false )
				{
					START_LOG( cwarn, L"같은 필드에 있는 유저를 파티 초대 실패!" )
						<< BUILD_LOG( GetPartyUID() )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( spReceiver->GetCharName() )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					kAck.m_iReceiverUID = spReceiver->GetCharUID();
					SendPacket( EGS_INVITE_PARTY_ACK, kAck );
					return;
				}
				else
				{
					// 대상자가 초대 조건에 만족하여 초대처리합니다.
					KEPM_INVITE_PARTY_REQ kPacket;
					kPacket.m_iPartyUID = GetPartyUID();
					spReceiver->GetPartyUserInfo( kPacket.m_kPartyUserInfo );
					SendToCnParty( EPM_INVITE_PARTY_REQ, kPacket );	
					return;
				}			
			}

			return;
		}
	}
	

	//////////////////////////////////////////////////////////////////////////
	// 같은 필드 내에 유저가 존재하지 않는 경우 로그인 서버와 글로벌 서버로 정보를 보내어 파티 초대를 한다.
	{
		bool bInviteSameServerGroup = false;

		//////////////////////////////////////////////////////////////////////////
		// 같은 서버군에 해당되는 닉네임이 왔을경우에는 닉네임에 해당되는 UnitUID를 찾으러 로그인서버로 가자!
#ifdef EXTEND_SERVER_GROUP_MASK
		if( kPacket_.m_iServerGroupID == -1 || kPacket_.m_iServerGroupID == KBaseServer::GetKObj()->GetServerGroupID() )
#else EXTEND_SERVER_GROUP_MASK
		if( kPacket_.m_iServerGroupID == SEnum::SGI_INVALID  ||  kPacket_.m_iServerGroupID == KBaseServer::GetKObj()->GetServerGroupID() )
#endif EXTEND_SERVER_GROUP_MASK
		{
			bInviteSameServerGroup = true;
		}

		// 파티 초대자가 파티소속이 아니라면?
		if( GetPartyUID() == 0 )
		{
			// 로그인 서버로 보내서 체크하고 초대와 함께 파티 생성 동작을 한다.
			if( SiKPartyListManager()->E_CreatePartyFarFieldCheckHost( GetThisPtr<KGSUser>(), kPacket_.m_wstrNickName ) == false )
			{
				START_LOG( cwarn, L"파티 초대자가 파티초대를 할 수 있는 조건에 만족하지 않습니다." )
					<< BUILD_LOG( GetPartyUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kAck.m_iOK = NetError::GetLastError();
				SendPacket( EGS_INVITE_PARTY_ACK, kAck );
				return;
			}
		}
		// 이미 파티 소속이라면?
		else
		{
			// 파티 초대자에 대한 검사를 하자.
			if( SiKPartyListManager()->E_InvitePartyFarFieldCheckHost( GetThisPtr<KGSUser>(), kPacket_.m_wstrNickName ) == false )
			{
				START_LOG( cwarn, L"파티 초대자가 파티초대를 할 수 있는 조건에 만족하지 않습니다." )
					<< BUILD_LOG( GetPartyUID() )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kAck.m_iOK = NetError::GetLastError();
				SendPacket( EGS_INVITE_PARTY_ACK, kAck );
				return;
			}
		}

		KELG_INVITE_PARTY_FIND_RECEIVER_REQ kPacket;
		kPacket.m_iHostUnitUID			= GetCharUID();
		kPacket.m_iReceiverUnitUID		= kPacket_.m_iReceiverUID;		// kPacket.m_iReceiverUnitUID = kPacket.m_iReceiverUnitUID 되어 있던 오류 수정	- 박세훈	-- 2012년 11월 22일 패치 예정
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
		kPacket.m_iServerGroupID		= kPacket_.m_iServerGroupID;
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
		kPacket.m_wstrReceiverNickName	= kPacket_.m_wstrNickName;
		kPacket.m_kCreatePartyInfo		= kPacket_.m_kCreatePartyInfo;
		if( bInviteSameServerGroup )
		{
			SendToLoginServer( ELG_INVITE_PARTY_FIND_RECEIVER_REQ, kPacket );
		}
		else
		{
			SendToGlobalServer( EGB_INVITE_PARTY_FIND_RECEIVER_REQ, kPacket );
		}
	}
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_ACK, KEGS_INVITE_PARTY_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );

	SendPacket( EGS_INVITE_PARTY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_NOT, KEGS_INVITE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 이걸 받는 순간에는 임시 파티 대기자인데 CandidatePartyUID값을 넣어야 하지 않을까?
	SetCandidatePartyUID( kPacket_.m_iPartyUID );	

	SendPacket( EGS_INVITE_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KELG_INVITE_PARTY_FIND_RECEIVER_ACK kAck;
	kAck.m_iHostUnitUID = kPacket_.m_iHostUnitUID;
	kAck.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;

	if( SiKPartyListManager()->E_InvitePartyFarFieldCheckReceiver( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( clog, L"파티 초대 대상자가 초대될수 없는 상태입니다." )
			<< BUILD_LOG( kPacket_.m_wstrHostNickName )
			<< END_LOG;

		kAck.m_iOK = NetError::GetLastError();
		SendToLoginServer( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
	else
	{
		kAck.m_iOK = NetError::NET_OK;
		KPartyUserInfo kPartyUserInfo;
		GetPartyUserInfo( kPartyUserInfo );
		kAck.m_vecReceiverPartyUserInfo.push_back( kPartyUserInfo );
		SendToLoginServer( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
}

_IMPL_ON_FUNC( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KELG_INVITE_PARTY_FIND_RECEIVER_ACK kAck;
	kAck.m_iHostUnitUID = kPacket_.m_iHostUnitUID;
	kAck.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;

	if( SiKPartyListManager()->E_InvitePartyFarFieldCheckReceiver( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( clog, L"파티 초대 대상자가 초대될수 없는 상태입니다." )
			<< BUILD_LOG( kPacket_.m_wstrHostNickName )
			<< END_LOG;

		kAck.m_iOK = NetError::GetLastError();
		SendToGlobalServer( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
	else
	{
		kAck.m_iOK = NetError::NET_OK;
		KPartyUserInfo kPartyUserInfo;
		GetPartyUserInfo( kPartyUserInfo );
		kAck.m_vecReceiverPartyUserInfo.push_back( kPartyUserInfo );
		SendToGlobalServer( EGB_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, kAck );
		return;
	}
}

IMPL_ON_FUNC( ELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 파티 신청 대기 삭제
		m_kUserPartyManager.ClearWaitPartyUser();

		KEGS_INVITE_PARTY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_INVITE_PARTY_ACK, kPacketAck );
		return;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 초대자가 파티소속이라면
	if( GetPartyUID() != 0 )
	{
		KEPM_INVITE_PARTY_REQ kPacket;
		kPacket.m_iPartyUID = GetPartyUID();
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacket.m_kPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_INVITE_PARTY_REQ, kPacket );
	}
	//////////////////////////////////////////////////////////////////////////
	// 초대자가 파티소속이 아니라면
	else
	{
		// 파티 생성과 초대를 동시에 하자!
		KEPM_CREATE_PARTY_BY_INVITE_REQ kPacketNot;
		kPacketNot.m_cAuthLevel = GetAuthLevel();
		kPacketNot.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;
		GetPartyUserInfo( kPacketNot.m_kHostPartyUserInfo );
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacketNot.m_kReceiverPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_CREATE_PARTY_BY_INVITE_REQ, kPacketNot );
	}
}

_IMPL_ON_FUNC( EGB_INVITE_PARTY_FIND_RECEIVER_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 파티 신청 대기 삭제
		m_kUserPartyManager.ClearWaitPartyUser();

		KEGS_INVITE_PARTY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_INVITE_PARTY_ACK, kPacketAck );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 초대자가 파티소속이라면
	if( GetPartyUID() != 0 )
	{
		KEPM_INVITE_PARTY_REQ kPacket;
		kPacket.m_iPartyUID = GetPartyUID();
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacket.m_kPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_INVITE_PARTY_REQ, kPacket );
	}
	//////////////////////////////////////////////////////////////////////////
	// 초대자가 파티소속이 아니라면
	else
	{
		// 파티 생성과 초대를 동시에 하자!
		KEPM_CREATE_PARTY_BY_INVITE_REQ kPacketNot;
		kPacketNot.m_cAuthLevel = GetAuthLevel();
		kPacketNot.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;
		GetPartyUserInfo( kPacketNot.m_kHostPartyUserInfo );
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecReceiverPartyUserInfo )
		{
			kPacketNot.m_kReceiverPartyUserInfo = kInfo;
		}
		SendToCnParty( EPM_CREATE_PARTY_BY_INVITE_REQ, kPacketNot );
	}
}

IMPL_ON_FUNC( EGS_INVITE_PARTY_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( SiKPartyListManager()->E_InvitePartyReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 초대에 의한 입장 실패." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_DENY_NOT, KEGS_INVITE_PARTY_DENY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_INVITE_PARTY_DENY_NOT, kPacket_ );
}

//{{ 2012. 10. 25	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC_NOPARAM( EPM_LEAVE_CANDIDATE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 파티 초대 대상자가 파티초대를 거부했기 때문에 파티는 파괴됩니다. 따라서 임시 등록한 PartyUID값도 0으로 변경해야함.
	SetCandidatePartyUID( 0 );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EGS_LEAVE_PARTY_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		// [참고] 파티이탈하기전에 강퇴당했다면 시간차로 발생가능

		START_LOG( cwarn, L"파티를 탈퇴하기전인데 UID가 없다.?" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_LEAVE_PARTY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_01;
		SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
		return;
	}
	
	if( SiKPartyListManager()->E_LeavePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 나가기 실패.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iReason) )
			<< END_LOG;
	}

	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

IMPL_ON_FUNC( EPM_LEAVE_PARTY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 성공적으로 파티 이탈 되었다면?
	if( kPacket_.m_iOK == NetError::NET_OK  ||
		kPacket_.m_iOK == NetError::ERR_PARTY_01 )
	{
		//{{ 2010. 03. 29  최육사	파티 이탈 예외
		if( GetPartyUID() == 0 )
		{
			START_LOG( clog, L"이탈 요청한 이후에 현재 PartyUID가 0이군.." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< END_LOG;
		}
		else
		{
			if( GetPartyUID() != kPacket_.m_iPartyUID )
			{
				START_LOG( cerr, L"이탈요청한 PartyUID와 현재 PartyUID가 다르다! 절대로 있을수 없는 에러!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetPartyUID() )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< END_LOG;

				//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
				// 여기서 끝낸다.
				return;
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
			}
		}
		//}}

		// 파티 대기자UID도 리셋
		SetCandidatePartyUID( 0 );

		//파티 UID를 리셋하고.
		SetPartyUID( 0 );

		//나간유저 파티정보 업데이트.
		std::set< UidType > setEmpty;
		SendUpdatePartyMemberList( 0, setEmpty );

		//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정
		if( GetStateID() == KGSFSM::S_ROOM  &&  GetRoomUID() > 0 )
		{
			// 현재 방에 있는 상태라면 이탈 처리!
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_DUNGEON:
				{
					// 던전방이라면 이탈처리 합니다.
					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
					SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

					// RoomUID초기화
					SetRoomUID( 0 );

					// field상태로 이동
					StateTransition( KGSFSM::I_TO_FIELD_MAP );
				}
				break;
			case CXSLRoom::RT_SQUARE:
			case CXSLRoom::RT_TRAININGCENTER:
				//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			case CXSLRoom::RT_BATTLE_FIELD:
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				// 훈련소 및 광장에 입장하면서 자동으로 파티 이탈 처리하는거라면 자동 방 이탈 처리하지 않는다.
				break;

			default:
				{
					// 정상적인 상황이 아니므로 에러로그를 남긴다.
					START_LOG( cerr, L"파티 맺고 있었는데 던전이 아닌 방에 들어와있네?" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< END_LOG;

					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
					SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

					// RoomUID초기화
					SetRoomUID( 0 );

					// field상태로 이동
					StateTransition( KGSFSM::I_TO_FIELD_MAP );
				}
				break;
			}
		}
		//}}
	}


	//{{ 2011. 11. 28	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 만약 파티 통합을 위한 이탈이었다면!
	if( kPacket_.m_iReason == NetError::NOT_LEAVE_PARTY_REASON_06 )
	{
		// 클라이언트로 ACK패킷을 보내지 않는다!
		return;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// 클라이언트로 ACK패킷 보냄
	KEGS_LEAVE_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_LEAVE_PARTY_NOT, KEGS_LEAVE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 자기 자신의 이탈에 대한 것이라면 다른 처리를 한다.
	if( GetCharUID() == kPacket_.m_iLeaveUnitUID )
	{
		// 만약 배틀필드에서 파티 가입을 했다면 배틀필드에도 파티 정보 갱신을 해주자!
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
			kPacketToRM.m_iPartyUID = 0; // 이탈처리니까 0값을 넣자
			SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
		}

		//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
		if( m_kComeBackUserInfo.GetIsComeBackUser() == false )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();

			DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );

			if( kReq.m_vecDeActivateBuff.empty() == false )
			{
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif SERV_SERVER_BUFF_SYSTEM
		//}

		return;
	}

	if( GetPartyUID() == 0  ||  GetPartyUID() != kPacket_.m_iPartyUID )
	{
		START_LOG( cwarn, L"다른 파티에 대한 이탈 처리가 왔다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iPartyUID );
		return;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 02. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 만약 배틀필드에서 파티 가입을 했다면 배틀필드에도 파티 정보 갱신을 해주자!
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
		kPacketToRM.m_iPartyUID = GetPartyUID();
		kPacketToRM.m_vecPartyUnitUIDList = kPacket_.m_vecPartyUnitUIDList;
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	if( m_kComeBackUserInfo.GetIsComeBackUser() == false && kPacket_.m_bExistComeBackUser == false )
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();

		DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );

		if( kReq.m_vecDeActivateBuff.empty() == false )
		{
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	SendPacket( EGS_LEAVE_PARTY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_READY_REQ )
{
	VERIFY_STATE( (2, KGSFSM::S_ROOM, KGSFSM::S_FIELD_MAP) );

	KEGS_PARTY_CHANGE_READY_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	////////////////////////////////////////////////////////////////////////// 유저 조건
	// partyuid
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;

	//파티의 던전정보를 찾아서..	
	if( SiKPartyListManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
	{
		START_LOG( cerr, L"파티의 던전정보 찾기 실패.!" )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		//파티 UID 이상과 같은 경우로 본다.
		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	//{{ 2009. 8. 5  최육사		체험ID제한
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"클라이언트에서 체험ID가 레벨20되었을때 유저를 캐릭선택으로 이동시켰을텐데 아직 게임을 하고있네?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUEST_USER_02;
			goto end_proc;
		}

		if( CXSLDungeon::IsHenirDungeon( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_GUEST_USER_00;
			goto end_proc;
		}
	}
	//}}

	//{{ 2010. 12. 28	최육사	하멜 마을 추가
//#ifdef SERV_SHIP_DUNGEON
//	if( CXSLDungeon::IsShipDungeon( iDungeonID ) == false )
//#endif SERV_SHIP_DUNGEON
//	//}}
//	{
//		//던전게이트가 아니면 레디할수 없음.
//		if( CXSLMapData::IsDungeonGate( GetMapID() ) == false )
//		{
//			START_LOG( cwarn, L"던전게이트가 아니면 파티레디를 할수없음." )
//				<< BUILD_LOG( GetCharName() )
//				<< BUILD_LOG( iDungeonID )
//				<< BUILD_LOG( GetMapID() )
//				<< END_LOG;
//
//			kAck.m_iOK = NetError::ERR_PARTY_20;
//			goto end_proc;
//		}
//
//		if( GetMapID() != CXSLMapData::GetMapID( iDungeonID ) )
//		{
//			START_LOG( cwarn, L"레디할수있는 던전게이트가 아님." )
//				<< BUILD_LOG( GetCharName() )
//				<< BUILD_LOG( iDungeonID )
//				<< BUILD_LOG( GetMapID() )
//				<< END_LOG;
//
//			kAck.m_iOK = NetError::ERR_PARTY_29;
//			goto end_proc;
//		}
//	}

	//준비상태로 변경시 해당 던전게임 할수있는지 검사
	if( kPacket_.m_bReady == true )
	{
		//::1. 근성도 검사.
		if( CheckingSpirit( iDungeonID ) == false )
		{
			kAck.m_iOK = NetError::ERR_ROOM_27;
			goto end_proc;
		}

		//::2. 입장필요 아이템검사.
		int iRequireItemID = 0;
		int iRequireQuantity = 0;

		if( CheckRequiredItemToEnterDungeonRoom( iDungeonID + static_cast<int>(cDifficultyLevel), (CXSLDungeon::DUNGEON_MODE) cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
		{
			//{{ 2010. 05. 06  최육사	에러 팝업
			KEGS_ERROR_POPUP_NOT kNot;
			kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			kNot.m_iErrorData1 = iRequireItemID;
			kNot.m_iErrorData2 = iRequireQuantity;
			SendPacket( EGS_ERROR_POPUP_NOT, kNot );
			//}}

			kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			goto end_proc;
		}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		if( CheckLimitedPlayTimes( iDungeonID + static_cast<int>(cDifficultyLevel) ) == false )
		{
			kAck.m_iOK = NetError::ERR_DUNGEON_ENTER_LIMIT_00;
			goto end_proc;
		}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

		//{{ 2010. 12. 30	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) == true )
		{
			if( CXSLDungeon::IsVelderDungeon( iDungeonID ) == true )
			{
				// 해당 map에 들어갈 수 있는지 조건 체크
				//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
#ifdef SERV_ENTER_VILLAGE_ERROR
				int iTempLevel = 0;
				int iTempDungeonID = 0;
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL, iTempLevel, iTempDungeonID ) == false )
#else
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
#endif SERV_ENTER_VILLAGE_ERROR
				//}}
				{
					START_LOG( cwarn, L"마을에 입장할 수 있는 조건이 안됨." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iDungeonID )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					goto end_proc;
				}
			}
		}
#endif SERV_SHIP_DUNGEON
		//}}
	}

	////////////////////////////////////////////////////////////////////////// 처리
	// 레디 변경
	if( SiKPartyListManager()->E_ChangeReadyReq( GetThisPtr<KGSUser>(), kPacket_.m_bReady ) == false )
	{
		START_LOG( cwarn, L"파티 레디 변경 실패." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( static_cast<int>(kPacket_.m_bReady) )
			<< END_LOG;
	}

	return;

end_proc:
	SendPacket( EGS_PARTY_CHANGE_READY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_DUNGEON_REQ )
{	
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_PARTY_CHANGE_DUNGEON_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	////////////////////////////////////////////////////////////////////////// 데이터 검증	
	// PartyUID검증
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	// DungeonID검증
	if( !SiCXSLDungeonManager()->VerifyDungeonID( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) ) )
	{
		START_LOG( cerr, L"존재하지않는 DungeonID입니다. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}

	//{{ 2009. 7. 30  최육사	HenirDungeonMode검증
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"비정상적인 HenirDungeonMode값입니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOG( (int)kPacket_.m_cDungeonMode )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}
	//}}

	////////////////////////////////////////////////////////////////////////// 유저 조건
	//{{ 2009. 8. 5  최육사		체험ID제한
	if( IsGuestUser()  &&  CXSLDungeon::IsHenirDungeon( kPacket_.m_iDungeonID ) )
	{
		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}
	//}}

	//::1. 근성도 검사.
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	//::2. 입장필요 아이템검사.		// 던전 시작 부분에서 체크 하도록 변경
// 	int iRequireItemID = 0;
// 	int iRequireQuantity = 0;
// 
// 	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
// 	{
// 		//{{ 2010. 05. 06  최육사	에러 팝업
// 		KEGS_ERROR_POPUP_NOT kNot;
// 		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
// 		kNot.m_iErrorData1 = iRequireItemID;
// 		kNot.m_iErrorData2 = iRequireQuantity;
// 		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
// 		//}}
// 
// 		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
// 		goto end_proc;
// 	}

	//::3. 선행던전 클리어를 하지 않았음.
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
		if( iRequireDungeonID != 0 )
		{
			std::map<int,KDungeonClearInfo>::iterator mit;
			mit = m_mapDungeonClear.find( iRequireDungeonID );
			if( mit == m_mapDungeonClear.end() )
			{
				kAck.m_iOK	= NetError::ERR_ROOM_06;

				goto end_proc;
			}
		}
	}

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	//::4. 레벨 조건이 충족되지 않는 유저가 있음.
	{
		KGSPartyPtr spParty = SiKPartyListManager()->GetParty( GetPartyUID() );
		if( spParty == NULL )
		{
			START_LOG( cerr, L"파티 UID 이상.!" )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_03;

			goto end_proc;
		}

// 		if( spParty->GetCheckLowLevel() == true )
// 		{
// 			int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) );
// 
// 			if( spParty->CheckPartyMinLevel( iDungeonMinLevel ) == false )
// 			{
// 				kAck.m_iOK = NetError::ERR_CHANGE_DUNGEON_MAP_FAILE;
// 
// 				goto end_proc;
// 			}
// 		}
	}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2012. 03. 03	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	if( CXSLDungeon::IsDefenceDungeon( kPacket_.m_iDungeonID ) == true )
	{
		if( SiKGSWorldMissionManager()->GetIsActive() == true )
		{
			int iWorldCrystalNum = SiKGSWorldMissionManager()->GetWorldCrystalCount();

			if( KGSWorldMissionManager::GATE_OF_THE_DARKNESS_EXPERT <= iWorldCrystalNum )
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_EXPERT;
			}
			else if( KGSWorldMissionManager::GATE_OF_THE_DARKNESS_HARD <= iWorldCrystalNum )
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_HARD;
			}
			else
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_NORMAL;
			}
		}
		else
		{
			START_LOG( cerr, L"패킷 변조 의심 유저입니다.")
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_22;
			goto end_proc;
		}
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}	

	////////////////////////////////////////////////////////////////////////// 처리
	// 던전 변경
	if( SiKPartyListManager()->E_ChangeDungeon( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"던전변경 실패." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;
	}	

	return;

end_proc:
	SendPacket( EGS_PARTY_CHANGE_DUNGEON_ACK, kAck );
}

IMPL_ON_FUNC( EGS_QUICK_START_DUNGEON_GAME_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_REQ, EGS_QUICK_START_DUNGEON_GAME_ACK );

	KEGS_QUICK_START_DUNGEON_GAME_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-09-26	// 박세훈
	// 던전 자동 매칭 중인지 확인!!
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"자동 매칭 중인데 바로 시작 요청이 들어왔다!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_MATCH_MAKING_23;	// 던전 게임 신청 중에는 사용할 수 없는 기능입니다.
		goto end_proc;
	}

	// 공식 대전 매칭 중인지 확인!!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"공식 대전 매칭 중인데 던전 시작 요청이 들어왔다!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_AUTO_PARTY_07;	// 공식 대전 신청 중에는 사용할 수 없는 기능입니다.
		goto end_proc;
	}

	// 룸 리스트 조회 중인지 확인!!
	if( GetRoomListID() != 0 )
	{
		START_LOG( cerr, L"룸 리스트 조회 중에 던전 시작 요청이 들어왔다!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_AUTO_PARTY_08;	// // 룸 리스트 조회 중에는 사용할 수 없는 기능입니다.
		goto end_proc;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	// 정보 초기화
	SetReserveKickUser( false );
	SetDungeonRequiredItemID( 0 );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	// 퀵슬롯 쿨 타임 리셋
	m_kInventory.ResetQuickSlotCoolTime();

	// 1. 파티에 속해 있는지 검사
	if( GetPartyUID() == 0 )
	{
		// 던전 게임을 시작할 조건을 만족하는지 체크
		if( CheckForDungeonGameStart( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, false, true ) == false )
		{
			kAck.m_iOK = NetError::GetLastError();
			kAck.m_iFailUnitUID = GetCharUID();
			kAck.m_wstrFailUserNickName = GetCharName();
			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////
		// 마을에서 던전 게임 시작!
		if( GetRoomUID() == 0 )
		{
			DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, false );
		}
		else
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_BATTLE_FIELD:
				{
					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_28;
					SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_REQ, kPacket );

					// 이탈처리 했으니 여기서 쫑!
					SetRoomUID( 0 );

					//{{ 2012. 10. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
					// RoomUID값을 0으로 바꿨으니 상태도 잠깐 필드 상태로 두자!
					if( GetStateID() == KGSFSM::I_TO_ROOM )
					{
						StateTransition( KGSFSM::I_TO_FIELD_MAP );
					}
#endif SERV_BATTLE_FIELD_SYSTEM
					//}}					

					// 새로 방을 만들면서 던전 게임을 시작하자!
					DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, false );
				}
				break;

			default:
				{
					START_LOG( cerr, L"잘못된 상태값입니다!" )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					kAck.m_iOK = NetError::ERR_BATTLEFIELD_08;
					goto end_proc;
				}
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		// 8. 파티 게임 시작
		if( SiKPartyListManager()->E_GameStart( GetThisPtr<KGSUser>(), kPacket_ ) == false )
		{
			START_LOG( cerr, L"던전게임 시작하기 실패.!" )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( kPacket_.m_iDungeonID )
				<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
				<< BUILD_LOGc( GetPartyUID() )
				<< END_LOG;
		}
	}

	return;

end_proc:
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kAck );
}

IMPL_ON_FUNC( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 소속도 아닌데 파티 던전 게임 시작 체크 패킷이 왔네?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK kPacketAck;
	kPacketAck.m_iOK = NetError::NET_OK;

	// 각 파티원들에게 의견을 물을으러 가기 전에 입장조건을 체크 한다.
	if( CheckForDungeonGameStart( kPacket_.m_kDungeonGameInfo.m_cDungeonMode, kPacket_.m_kDungeonGameInfo.m_iDungeonID, kPacket_.m_kDungeonGameInfo.m_cDifficulty, false, false ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		kPacketAck.m_wstrFailUserNickName = GetCharName(); // 조건 불만족이면 팝업으로 닉네임 띄워야하니깐 닉네임도 담아가자!
	}

	SendToCnParty( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK, kPacketAck );
}

//{{ 2012. 12. 17	박세훈	수동 파티 시 던전 팝업 창 문구 오류 수정
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
_IMPL_ON_FUNC( EPM_PARTY_GAME_START_OPINION_CHECK_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_GAME_START_OPINION_CHECK_NOT, kPacket_ );
}
#else
IMPL_ON_FUNC_NOPARAM( EPM_PARTY_GAME_START_OPINION_CHECK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendID( EGS_PARTY_GAME_START_OPINION_CHECK_NOT );
}
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
//}}

IMPL_ON_FUNC( EGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 게임 시작에 대한 응답을 하네? 시간차 문제인듯" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	SendToCnParty( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_SINGLE_OPEN_DUNGEON_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"던전 혼자 플레이 하기 시작 실패!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// 로그만 찍자
	}

	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacketAck;
	kPacketAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacketAck );
}

_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_ACK, KEGS_LEAVE_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_ACK );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_iOK );

	bool bIsSuccess = false;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		bIsSuccess = true;
	}
	else
	{
		if( GetRoomUID() != 0  &&  kPacket_.m_iOK == NetError::ERR_ROOM_34 )
			bIsSuccess = true;
	}

	if( bIsSuccess == true )
	{
		if( GetTradeUID() != 0 )
		{
			KEGS_BREAK_TRADE_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
			_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
		}
	}
}

_IMPL_ON_FUNC( EPM_PARTY_GAME_START_FAILED_NOT, KEGS_PARTY_GAME_START_FAILED_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 게임 시작 실패 패킷이 오네?" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	SendPacket( EGS_PARTY_GAME_START_FAILED_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_QUICK_START_DUNGEON_GAME_ACK );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetPartyUID() == 0 )
		{
			START_LOG( cerr, L"파티를 통해 던전을 생성햇는데 파티 UID가 없음.!" )
				<< BUILD_LOG( GetCharName() )
				<< END_LOG;
		}
	}

	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacket );
}

IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//////////////////////////////////////////////////////////////////////////
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_LEAVE_ROOM_REQ kPacket;		
		kPacket.m_bSendAck = false;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_28;
		SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

		// 이탈처리 했으니 여기서 쫑!
		SetRoomUID( 0 );

		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if( GetAuthLevel() < SEnum::UAL_GM )
		kPacket_.m_wstrCNIP.clear();

	SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_RoomInfo.m_iDungeonID, kPacket_.m_RoomInfo.m_DifficultyLevel, kPacket_.m_RoomInfo.m_cDungeonMode );
#endif SERV_REFORM_QUEST
	//}}

#ifdef SERV_ADD_TITLE_CONDITION
	UseResurrectionStone( false );
	SetStartedNumMember( kPacket_.m_RoomInfo.m_JoinSlot );
#endif SERV_ADD_TITLE_CONDITION

	//{{ 2013. 02. 05  칭호 획득 조건 추가(부활석 사용 횟수, 샌더 마을) - 김민성
#ifdef SERV_ADD_TITLE_CONDITION_SANDER
	ClearUseResurrectionStoneCount();
#endif SERV_ADD_TITLE_CONDITION_SANDER
	//}

	StateTransition( KGSFSM::I_TO_ROOM );
	m_kTimer[TM_ROOM].restart();

	// 필드에서 이탈
	SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

	//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			int iRequireDungeonID = kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>(kPacket_.m_RoomInfo.m_DifficultyLevel);

			CXSLDungeon::DUNGEON_MODE eDungeonMode = (CXSLDungeon::DUNGEON_MODE) kPacket_.m_RoomInfo.m_cDungeonMode;

			//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
			// 헤니르 던전이라면 보상 받은 횟수를 증가시킨다.
			if( CXSLDungeon::IsHenirDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) == true )
			{
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
				if( m_kUserDungeonManager.IncreaseHenirRewardCount( kPacket_.m_RoomInfo.m_cDungeonMode ) == false )
#else // SERV_HENIR_RENEWAL_2013
				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
#endif // SERV_HENIR_RENEWAL_2013
				{
					START_LOG( clog, L"헤니르 보상 획득 횟수 증가 실패!! 더이상 증갈 할 수 없으니 보상 받을 수 없다." )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
						<< END_LOG;
				}
				//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
				else
				{
					// 헤니르 시공 보상 대상자임을 센터서버에도 알리러 가자!
					SendToCnRoom( ERM_UPDATE_HENIR_REWARD_USER_NOT );
				}
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
			}
#endif SERV_NEW_HENIR_TEST
			//}}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
			std::map< int, KDungeonPlayInfo >::iterator mitDungeonPlay = m_mapDungeonPlay.find( kPacket_.m_RoomInfo.m_iDungeonID );
			if( mitDungeonPlay == m_mapDungeonPlay.end() )
			{
				KDungeonPlayInfo kDungeonPlayInfo;
				kDungeonPlayInfo.m_iDungeonID = kPacket_.m_RoomInfo.m_iDungeonID;
				kDungeonPlayInfo.m_iPlayTimes = 1;
				kDungeonPlayInfo.m_iClearTimes = 0;
				kDungeonPlayInfo.m_bNew = true;

				m_mapDungeonPlay.insert( std::make_pair( kPacket_.m_RoomInfo.m_iDungeonID, kDungeonPlayInfo ) );
			}
			else
			{
				mitDungeonPlay->second.m_iPlayTimes += 1;
				mitDungeonPlay->second.m_bNew = true;
			}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

			//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
			int iItemID = 0;
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo, iItemID ) )
#else
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo ) )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
			//}}
			{
				// 우선 게임시작시 아이템이 없으면 오류니 로그를 남겨본다..
				START_LOG( cerr, L"던전게임 시작시 필요아이템이 없음.!" )
					<< BUILD_LOG( GetCharName() )
					<< BUILD_LOG( iRequireDungeonID )
					<< BUILD_LOG( (int)eDungeonMode )
					<< END_LOG;

//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
				// 강제퇴장 예약 설정
				SetReserveKickUser( true );
				SetDungeonRequiredItemID( iItemID );

// 				KERM_RESERVE_LEAVE_USER_NOT kPacketReq;
// 				kPacketReq.m_iUnitUID = GetCharUID();
// 				kPacketReq.m_iItemID = iItemID;
// 				SendToCnRoom( ERM_RESERVE_LEAVE_USER_NOT, kPacketReq );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"일어날수 없는 에러!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
	}	

	KEGS_PARTY_GAME_START_NOT kNot;
	kNot.m_RoomInfo = kPacket_.m_RoomInfo;
	kNot.m_sWorldID = kPacket_.m_sWorldID;
	kNot.m_vecSlot = kPacket_.m_vecSlot;
	kNot.m_vecObserverSlot = kPacket_.m_vecObserverSlot;
	kNot.m_wstrCNIP = kPacket_.m_wstrCNIP;
	kNot.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;

	SendPacket( EGS_PARTY_GAME_START_NOT, kNot );

	//{{ 2009. 5. 30  최육사	임시 인벤
	ClearTempInventory();
	//}}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	std::map< UidType, std::vector< KBuffInfo > >::iterator mit = kPacket_.m_mapActivateBuff.find( GetCharUID() );
	if( mit != kPacket_.m_mapActivateBuff.end() )
	{
		// 추가 된 버프가 있으면 보내자.
		std::vector< KBuffInfo > vecActivateBuff;
		BOOST_TEST_FOREACH( KBuffInfo&, kInfo, mit->second )
		{
			ActivateBuff( kInfo.m_kFactorInfo.m_BuffIdentity.m_eBuffTempletID, vecActivateBuff );
		}

		if( vecActivateBuff.empty() == false )
		{
			// 로그인 서버 버프 매니저에 업데이트
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();
			kReq.m_vecActivateBuff = vecActivateBuff;
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}	
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}

//{{ 2009. 2. 4  최육사	던전 리플레이
//_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_ACK, KERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//
//	if( kPacket_.m_iOK == NetError::NET_OK )
//	{
//		if( GetPartyUID() == 0 )
//		{
//			START_LOG( cerr, L"파티를 통해 던전 리플레이를 했는데 파티 UID가 없음.!" )
//				<< BUILD_LOG( GetCharName() )
//				<< END_LOG;
//		}
//	}
//
//	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacket;
//	kPacket.m_iOK = kPacket_.m_iOK;
//	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacket );
//}
//
//_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_NOT, KEGS_PARTY_GAME_START_NOT )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
//
//	if( GetAuthLevel() < SEnum::UAL_GM )
//		kPacket_.m_wstrCNIP.clear();
//
//	if( GetRoomUID() != kPacket_.m_RoomInfo.m_RoomUID )
//	{
//		START_LOG( cerr, L"던전 리플레이인데 RoomUID가 다르면 안되는데?" )
//			<< BUILD_LOG( GetCharUID() )
//			<< BUILD_LOG( GetRoomUID() )
//			<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomUID )
//			<< END_LOG;
//
//		// roomuid를 일단 넣어준다. 만약에 roomuid가 0이라면.. 이건 문제가 된다
//		SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );
//	}
//
//	m_kTimer[TM_ROOM].restart();
//
//	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
//	{
//	case CXSLRoom::RT_DUNGEON:
//		{
//			int iRequireDungeonID = kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>(kPacket_.m_RoomInfo.m_DifficultyLevel);
//
//			CXSLDungeon::DUNGEON_MODE eDungeonMode = (CXSLDungeon::DUNGEON_MODE) kPacket_.m_RoomInfo.m_cDungeonMode;
//			
//			//{{ 2011. 08. 12   김민성      헤니르 개편 
//#ifdef SERV_NEW_HENIR_TEST
//			// 헤니르 던전이라면 보상 받은 횟수를 증가시킨다.
//			if( CXSLDungeon::IsHenirDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) == true )
//			{
//				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
//				{
//					START_LOG( cerr, L"헤니르 보상 획득 횟수 증가 실패!!" )
//						<< BUILD_LOG( GetCharName() )
//						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
//						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
//						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
//						<< END_LOG;
//				}
//			}
//#endif SERV_NEW_HENIR_TEST
//			//}}
//
//			//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
//			int iItemID = 0;
//			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo, iItemID ) )
//#else
//			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo ) )
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//				//}}
//			{
//				// 우선 게임시작시 아이템이 없으면 오류니 로그를 남겨본다..
//				START_LOG( cerr, L"던전게임 시작시 필요아이템이 없음.!" )
//					<< BUILD_LOG( GetCharName() )
//					<< BUILD_LOG( iRequireDungeonID )
//					<< BUILD_LOG( (int)eDungeonMode )
//					<< END_LOG;
//
//				//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
//				// 강제퇴장 예약 설정
//				SetReserveKickUser( true );
//				SetDungeonRequiredItemID( iItemID );
//
//// 				KERM_RESERVE_LEAVE_USER_NOT kPacketReq;
//// 				kPacketReq.m_iUnitUID = GetCharUID();
//// 				kPacketReq.m_iItemID = iItemID;
//// 				SendToCnRoom( ERM_RESERVE_LEAVE_USER_NOT, kPacketReq );
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//				//}}
//				// 방 입장 실패 처리 - 센터서버로 ERM_LEAVE_ROOM_REQ를 날려서 방이탈 처리 함
//				// 돌아오는 ACK는 KGSFSM에 걸려서 클라이언트로 전송되지 않는다.
//				//KERM_LEAVE_ROOM_REQ kPacketReq;
//				//kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
//				//SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );
//				//return;
//			}
//		}
//		break;
//
//	default:
//		{
//			START_LOG( cerr, L"일어날수 없는 에러!" )
//				<< BUILD_LOG( GetCharUID() )
//				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
//				<< END_LOG;
//		}
//	}	
//
//	SendPacket( EGS_PARTY_GAME_START_NOT, kPacket_ );
//
//	//{{ 2009. 5. 30  최육사	임시 인벤
//	ClearTempInventory();
//	//}}
//}
//}}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_HOST_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_HOST_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 26  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeHost( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeHost( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cwarn, L"파티장 바꾸기 실패.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_BAN_USER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_BAN_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_BAN_USER_ACK, kAck );
		return;
	}

	//자기자신을 강퇴 시킬수 없다.
	if( kPacket_ == GetCharUID() )
	{
		KEGS_PARTY_BAN_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_ROOM_26;
		SendPacket( EGS_PARTY_BAN_USER_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 26  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_BanUser( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_BanUser( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티강퇴 실패.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//{{ 2009. 3. 18  최육사	파티유저밴
IMPL_ON_FUNC( ERM_PARTY_BAN_USER_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 실패했어도 not는 날려주자!
	}

	//{{ 2010. 8. 18	최육사	서버 코드 정리
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		ClearTempInventory();
	}
	//}}

	// 방 이탈처리
	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendID( EGS_PARTY_BAN_USER_NOT );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}
}
//}}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ )
{	
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeGetItemtype( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeGetItemtype( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"던전게임 아이템 습득방식 변경실패." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_NUM_OF_PER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_NUM_OF_PER_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeNumOfPer( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeNumOfPer( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티 인원병경 실패.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//{{ 2009. 4. 1  최육사		파티렙제
IMPL_ON_FUNC( EGS_PARTY_LIMIT_LEVEL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_LIMIT_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_LIMIT_LEVEL_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
#else
	if( SiKPartyManager()->E_LimitLevel( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티 렙제 옵션 변경 실패.!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}
//}}

IMPL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INVITE_PARTY_ACK );
    
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"파티 생성 실패!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}
	else
	{
		// 실제로 파티가 생성된 상태이므로 파티 소속으로 넣자!
		SetPartyUID( kPacket_.m_kPartyInfo.m_iPartyUID );
	}

	KEGS_INVITE_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iReceiverUID = kPacket_.m_kReceiverPartyUserInfo.m_iUnitUID;
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_wstrUDPRelayIP = kPacket_.m_kHostPartyUserInfo.m_wstrIP;
	kPacket.m_usUDPRelayPort = kPacket_.m_kHostPartyUserInfo.m_usPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	SendPacket( EGS_INVITE_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_JOIN_PARTY_NOT, KEGS_JOIN_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		BOOST_TEST_FOREACH( const KPartyInfo&, kInfo, kPacket_.m_vecPartyInfo )
		{
			//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정
			if( GetPartyUID() > 0 )
			{
				if( GetPartyUID() != kInfo.m_iPartyUID )
				{
					START_LOG( cerr, L"이미 파티 소속인데 또 파티 가입처리가 되었군.. 기존파티는 이탈 시키자!" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( GetCharName() )
						<< BUILD_LOG( GetPartyUID() )
						<< BUILD_LOG( kInfo.m_iPartyUID )
						<< END_LOG;

					// 기존 파티 이탈 처리
					//SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_05 );

					// 새로운 파티 생성에 대해서는 자동 이탈 처리한다.
					KEPM_LEAVE_PARTY_REQ kPacketReq;
					kPacketReq.m_iPartyUID = kInfo.m_iPartyUID;
					kPacketReq.m_iReason = NetError::NOT_LEAVE_PARTY_REASON_05;
					_SendToCnParty( kInfo.m_iPartyUID, EPM_LEAVE_PARTY_REQ, kPacketReq );
					return;
				}
				//else
				//{
				//	START_LOG( cerr, L"이미 PartyUID가 저장되어있는데 똑같은 PartyUID를 또 저장하려고 하네? 절대 일어나서는 안되는 에러!" )
				//		<< BUILD_LOG( GetCharUID() )
				//		<< BUILD_LOG( GetCharName() )
				//		<< BUILD_LOG( GetPartyUID() )
				//		<< END_LOG;

				//	// 이건 이탈처리를 할 필요 없는 상황.
				//}
			}

			// 예약된 partyuid삭제
			SetCandidatePartyUID( 0 );
			//}}

			// partyuid값 저장
			SetPartyUID( kInfo.m_iPartyUID );

			//{{ 2012. 04. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			// 파티의 던전값을 저장하자!
			m_kUserDungeonManager.SetDungeonGameInfo( kInfo.m_iDungeonID, kInfo.m_DifficultyLevel, kInfo.m_cDungeonMode );
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			break;
		}

		if( GetPartyUID() <= 0 )
		{
			START_LOG( cerr, L"파티UID값이 이상합니다. 절대 일어나서는 안되는에러!" )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
				<< BUILD_LOG( kPacket_.m_vecPartyInfo.size() )
				<< BUILD_LOG( kPacket_.m_vecPartyUserInfo.size() )
				<< END_LOG;
		}

		//{{ 2012. 02. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		// 만약 배틀필드에서 파티 가입을 했다면 배틀필드에도 파티 정보 갱신을 해주자!
		if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
			kPacketToRM.m_iPartyUID = GetPartyUID();
			kPacketToRM.m_vecPartyUnitUIDList = kPacket_.m_vecCurPartyUnitUIDList;
			SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
		}
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
		bool bExistComeBackUser = false;
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecPartyUserInfo )
		{
			if( kInfo.m_bComeBack == true )
			{
				bExistComeBackUser = true;
				break;
			}
		}

		if( bExistComeBackUser == true )
		{
			KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
			kReq.m_iUnitUID = GetCharUID();

			ActivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecActivateBuff );
			
			if( kReq.m_vecActivateBuff.empty() == false )
			{
				SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
			}
		}
#endif SERV_SERVER_BUFF_SYSTEM
		//}
	}
	//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정
	else if( kPacket_.m_iOK == NetError::ERR_PARTY_18 )
	{
		// 예약된 partyuid삭제
		SetCandidatePartyUID( 0 );
	}
	//}}

	SendPacket( EGS_JOIN_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CLOSE_PARTY_NOT, KEGS_CLOSE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2012. 02. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 임시 파티 멤버인 경우 역시 이탈 처리!
	if( GetCandidatePartyUID() != 0  &&  GetCandidatePartyUID() == kPacket_.m_iPartyUID )
	{
		SetCandidatePartyUID( 0 );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	if( GetPartyUID() == 0  ||  GetPartyUID() != kPacket_.m_iPartyUID )
	{
		START_LOG( cerr, L"관련 없는 파티인데 파티 닫기 처리가 왔다!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
		return;
	}

	// 파티 이탈 처리!
	SetPartyUID( 0 );

	//{{ 2012. 02. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 만약 배틀필드에서 파티 가입을 했다면 배틀필드에도 파티 정보 갱신을 해주자!
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
		kPacketToRM.m_iPartyUID = 0; // 이탈처리니까 0값을 넣자
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	if( m_kComeBackUserInfo.GetIsComeBackUser() == false )
	{
		KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
		kReq.m_iUnitUID = GetCharUID();

		DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );

		if( kReq.m_vecDeActivateBuff.empty() == false )
		{
			SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
		}
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}

	SendPacket( EGS_CLOSE_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_UPDATE_PARTY_USER_INFO_BROAD_NOT, KEGS_UPDATE_PARTY_USER_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_PARTY_USER_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_ACK, KEGS_PARTY_CHANGE_READY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_READY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_NOT, KEGS_PARTY_CHANGE_READY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_READY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_ACK, KEGS_PARTY_CHANGE_DUNGEON_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_DUNGEON_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_NOT, KEGS_PARTY_CHANGE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2012. 04. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, kPacket_.m_cDungeonMode );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	SendPacket( EGS_PARTY_CHANGE_DUNGEON_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_GAME_START_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 입장 아이템이 부족한 경우는 따로 에러 팝업을 띄운다.
	//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( kPacket_.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00 )
	{
		KEGS_ERROR_POPUP_NOT kNot;
		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		//{{ 던전 입장권 에러 팝업 변경
#ifdef SERV_CHANGE_ERROR_POPUP
		kNot.m_bPartyMember = true;
#endif SERV_CHANGE_ERROR_POPUP
		//}}
		if( SiCXSLDungeonManager()->GetRequireItemIDAndCount( kPacket_.m_iDungeonID, static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_iDungeonMode), kNot.m_iErrorData1, kNot.m_iErrorData2 ) == true )
		{
			SendPacket( EGS_ERROR_POPUP_NOT, kNot );
		}
		else
		{
			START_LOG( cerr, L"던전 입장권 정보를 찾지 못했다!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iDungeonID )
				<< BUILD_LOG( kPacket_.m_iDungeonMode )
				<< END_LOG;
		}
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEGS_QUICK_START_DUNGEON_GAME_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iFailUnitUID = kPacket_.m_iFailUnitUID;
	kPacket.m_wstrFailUserNickName = kPacket_.m_wstrFailUserNickName;
	SendPacket( EGS_QUICK_START_DUNGEON_GAME_ACK, kPacket );
}

IMPL_ON_FUNC( EPM_CHECK_FOR_PARTY_GAME_START_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_FOR_PARTY_GAME_START_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// 파티 소속인지 검사
	if( GetPartyUID() <= 0 )
	{
		START_LOG( cerr, L"파티원이 아닌데 파티장이 게임 시작하자는 이벤트가 왔다?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	// 다른 파티원인지 검사
	if( GetPartyUID() != kPacket_.m_iPartyUID )
	{
		START_LOG( cerr, L"해당 소속 파티원이 아닌데 다른 파티장이 게임 시작하자는 이벤트가 왔다?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	switch( kPacket_.m_usEventID )
	{
	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ:
		{
			if( GetStateID() != KGSFSM::S_FIELD_MAP )
			{
				if( GetStateID() == KGSFSM::S_ROOM  &&  CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
				{
					START_LOG( cwarn, L"파티장과 같은 상태가 아니라서 게임을 시작할수 없음." )
						<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
						<< BUILD_LOG( GetStateIDString() )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_UNKNOWN;
					SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
					return;
				}
			}
		}
		break;

	//case ERM_PARTY_GAME_REPLAY_START_REQ:
	//	{
	//		if( GetStateID() != KGSFSM::S_ROOM )
	//		{
	//			START_LOG( cwarn, L"파티장과 같은 상태가 아니라서 게임을 시작할수 없음." )
	//				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
	//				<< BUILD_LOG( GetStateIDString() )
	//				<< END_LOG;

	//			kPacket.m_iOK = NetError::ERR_UNKNOWN;
	//			SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
	//			return;
	//		}
	//	}
	//	break;

	default:
		{
			START_LOG( cerr, L"잘못된 이벤트 정보 입니다. 절대 일어날수없는에러!" )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
			return;
		}
		break;
	}

	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kDungeonGameInfo );

	// PartyRoomUserInfo얻기
	KPartyRoomUserInfo kPartyRoomUserInfo;
	GetTutorUnitUIDList( kPartyRoomUserInfo.m_vecStudentUnitUID );
	GetRoomUserInfo( kPartyRoomUserInfo.m_kRoomUserInfo, CXSLRoom::RT_DUNGEON );
	//{{ 2009. 6. 10  최육사	게임 시작할 당시 EXP,ED얻기
	kPartyRoomUserInfo.m_iCurEXP = GetEXP();
	kPartyRoomUserInfo.m_iCurED = GetED();
	//}}
	kPartyRoomUserInfo.m_wstrChannelIP = NetCommon::GetLocalIPW();

	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_sWorldID = kPacket_.m_sWorldID;
	kPacket.m_usEventID = kPacket_.m_usEventID;
	kPacket.m_vecUserInfo.push_back( kPartyRoomUserInfo );
	SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_PARTY_OPEN_DUNGEON_ROOM_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	kPacket_.m_kRoomInfo.m_bDefenceDungeonOpen = SiKGSWorldMissionManager()->GetIsActive();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	SendToCnRoom( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ, kPacket_ );
}

//_IMPL_ON_FUNC( EPM_PARTY_GAME_REPLAY_START_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
//{
//	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
//
//	SendToCnRoom( ERM_PARTY_GAME_REPLAY_START_REQ, kPacket_ );
//}

//{{ 2011. 07. 19	최육사	대전 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_PARTY_CHANGE_HOST_ACK kPacket;	
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_iOldHostUnitUID = kPacket_.m_iOldHostUnitUID;
	kPacket.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	kPacket.m_bForRegroupParty = kPacket_.m_bForRegroupParty;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	// 해당 파티가 맞는지 검사
	if( kPacket_.m_iPartyUID != GetPartyUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	// 해당 유저가 맞는지 검사
	if( kPacket_.m_iNewHostUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	switch( kPacket_.m_cPartyType )
	{
	case KPartyInfo::PT_DUNGEON_PARTY:
		{
			//바뀔파티장이 현재 설정된 던전을 할수있는지 확인.
			if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
			{
				START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_31;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}

			const int iDungeonID = kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty);
			const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode);

			if( CheckRequiredItemToEnterDungeonRoom( iDungeonID, eDungeonMode ) == false )
			{
				START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_31;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
			if( CheckLimitedPlayTimes( iDungeonID ) == false )
			{
				START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_DUNGEON_ENTER_LIMIT_00;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

			const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
			if( iRequireDungeonID != 0 )
			{
				std::map< int, KDungeonClearInfo >::iterator mit;
				mit = m_mapDungeonClear.find( iRequireDungeonID );
				if( mit == m_mapDungeonClear.end() )
				{
					START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
						<< BUILD_LOG( kPacket_.m_iPartyUID )
						<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_PARTY_31;
					SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
					return;
				}
			}
		}
		break;

	case KPartyInfo::PT_PVP_PARTY:
		{
			// 대전 파티 제한 레벨
			if( GetLevel() < SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL )
			{
				START_LOG( cwarn, L"공식 대전을 하기에 부족한 캐릭터 레벨입니다." )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOGc( GetLevel() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MATCH_MAKING_11;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}			
		}
		break;

	default:
		{
			START_LOG( cerr, L"파티 타입이 이상합니다!" )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( kPacket_.m_cPartyType )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_27;
			SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
			return;
		}
		break;
	}

	// 모든 조건을 만족했다면 OK값을 보내자!
	kPacket.m_iOK = NetError::NET_OK;
	SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_PARTY_CHANGE_HOST_ACK kPacket;	
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_iOldHostUnitUID = kPacket_.m_iOldHostUnitUID;
	kPacket.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;

	// 해당 파티가 맞는지 검사
	if( kPacket_.m_iPartyUID != GetPartyUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	// 해당 유저가 맞는지 검사
	if( kPacket_.m_iNewHostUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	//바뀔파티장이 현재 설정된 던전을 할수있는지 확인.
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_31;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	const int iDungeonID = kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty);
	const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode);

	if( CheckRequiredItemToEnterDungeonRoom( iDungeonID, eDungeonMode ) == false )
	{
		START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_31;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
	if( iRequireDungeonID != 0 )
	{
		std::map< int, KDungeonClearInfo >::iterator mit;
		mit = m_mapDungeonClear.find( iRequireDungeonID );
		if( mit == m_mapDungeonClear.end() )
		{
			START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_31;
			SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
			return;
		}
	}

	// 모든 조건을 만족했다면 OK값을 보내자!
	kPacket.m_iOK = NetError::NET_OK;
	SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_ACK, KEGS_PARTY_CHANGE_HOST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_HOST_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_NOT, KEGS_PARTY_CHANGE_HOST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_HOST_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_BAN_USER_ACK, KEGS_PARTY_BAN_USER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_BAN_USER_ACK, kPacket_ );

	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_ACK, KEGS_PARTY_CHANGE_NUM_OF_PER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_NOT, KEGS_PARTY_CHANGE_NUM_OF_PER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_NOT, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_BAN_USER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2010. 02. 09  최육사	빠른파티 기능개선
#ifdef SERV_QUICK_PARTY_UPGRADE
	AddKickedPartyUID( GetPartyUID() );
#endif SERV_QUICK_PARTY_UPGRADE
	//}}

	//{{ 2010. 03. 29  최육사	파티 이탈 예외
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"이탈 요청한 이후에 현재 PartyUID가 0이다! 절대로 있을수 없는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}
	else
	{
		if( GetPartyUID() != kPacket_.m_iPartyUID )
		{
			START_LOG( cerr, L"이탈요청한 PartyUID와 현재 PartyUID가 다르다! 절대로 있을수 없는 에러!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< END_LOG;
		}			
	}
	//}}

	// PartyUID를 0으로 변경
	SetPartyUID( 0 );

	//나간유저 파티정보 업데이트.
	std::set< UidType > setEmpty;
	SendUpdatePartyMemberList( GetPartyUID(), setEmpty );

	//당사자에게 강퇴패킷을 날려준다.
	//{{ 2009. 3. 18  최육사	던전으로 가서 유저를 이탈시키자!
	if( GetRoomUID() > 0 )
	{
		//{{ 2010. 03. 17  최육사	상태 오류 수정
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
		{
			SendToCnRoom( ERM_PARTY_BAN_USER_REQ );
		}
		else if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			SendID( EGS_PARTY_BAN_USER_NOT );
		}
		else
		{
			START_LOG( cerr, L"던전방이 아닌데 파티를 맺고 있었네?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
		//}}
	}
	else
	{
		SendID( EGS_PARTY_BAN_USER_NOT );
	}

	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_bCanEnterSecretStage == false )
	{
		SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket_ );
		return;
	}

	// 방으로 비밀스테이지 체크하러 가자!
	SendToCnRoom( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
}

_IMPL_ON_FUNC( EPM_CHAT_ACK, KEGS_CHAT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHAT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHAT_NOT, KEGS_CHAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHAT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHANGE_PARTY_FEVER_STATE_NOT, KEGS_CHANGE_PARTY_FEVER_STATE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PARTY_FEVER_STATE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_FEVER_POINT_NOT, KEGS_PARTY_FEVER_POINT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_FEVER_POINT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 필드에 파티원 리스트를 업데이트 하러 가자!
	SendUpdatePartyMemberList( kPacket_.m_iPartyUID, kPacket_.m_setPartyUIDList );
}

IMPL_ON_FUNC_NOPARAM( EPM_QUICK_SLOT_COOL_TIME_RESET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 퀵슬롯 쿨 타임 리셋
	m_kInventory.ResetQuickSlotCoolTime();
}

//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
IMPL_ON_FUNC( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	// 현재 던전의 레벨에 부합 하지 않는 유저가 있는지 검사
	KGSPartyPtr spParty = SiKPartyListManager()->GetParty( GetPartyUID() );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( spParty->GetDungeonID() + static_cast<int>(spParty->GetDungeonDif()) );

	if( spParty->CheckPartyMinLevel( iDungeonMinLevel ) == false )
	{
		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_CHANGE_PARTY_MIN_LEVEL_CHECK_FAILE;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  최육사	채널 통합 파티

	if( SiKPartyListManager()->E_ChangeCheckMinLevel( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 최소 레벨 입장 제한 설정 실패!" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT, kPacket_ );
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
IMPL_ON_FUNC( EPM_CHANGE_COME_BACK_BUFF_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ kReq;
	kReq.m_iUnitUID = GetCharUID();

	if( kPacket_.m_bIsBuffFinish == false )
	{
		ActivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecActivateBuff );
	}
	else
	{
		DeactivateBuff( CXSLBuffManager::BTI_BUFF_RETURN_OF_HERO, kReq.m_vecDeActivateBuff );
	}

	if( kReq.m_vecActivateBuff.empty() == false || kReq.m_vecDeActivateBuff.empty() == false )
	{
		SendToLoginServer( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, kReq );
	}
#else
	CTime tComeBackBuffEnd;

	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrComeBackBuffEnd, tComeBackBuffEnd ) == false )
	{
		tComeBackBuffEnd = CTime( 0 );
		START_LOG( cerr, L"시간 변환 실패!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_wstrComeBackBuffEnd )
			<< END_LOG;
	}

	KEGS_COME_BACK_BUFF_NOT kNot;
	kNot.m_bIsFinish = kPacket_.m_bIsBuffFinish;
	kNot.m_iComeBackBuffEnd = tComeBackBuffEnd.GetTime();
	SendPacket( EGS_COME_BACK_BUFF_NOT, kNot );
#endif SERV_SERVER_BUFF_SYSTEM
	//}
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_GAME_START_TIME_COUNT_NOT, KEGS_GAME_START_TIME_COUNT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GAME_START_TIME_COUNT_NOT, kPacket_ );

	//{{ 2013. 01. 01	최육사	던전에서 필드로 돌아오지 못하는 오류 수정
#ifdef SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
	// 스킬 체인지 쿨 타임 체크를 위해 타이머를 초기화 한다.
	m_kTimer[TM_SKILL_CHANGE_COOL_TIME_AT_START_COUNT].restart();
#endif SERV_SKILL_CHANGE_COOL_TIME_AT_START_COUNT
	//}}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}



