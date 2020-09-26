#include "AutoParty.h"

#include "AutoPartyFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"
#include "X2Data/XSLUnitManager.h"
#include "AutoPartyMakingManager.h"

//{{ 2013. 09. 30	박세훈		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
	#include "X2Data/XSLDungeon.h"
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
//}}



//{{ 2012. 02. 08	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


#define CLASS_TYPE KAutoParty
ImplPfID( KAutoParty, PI_GLOBAL_AUTO_PARTY );


KAutoParty::KAutoParty( IN const UidType iAutoPartyUID, IN const KAutoPartyConditionInfo& kInfo )
{
	SetUID( iAutoPartyUID );
	m_kAutoPartyInfo = kInfo;
	m_kAutoPartyUserManager.Init( kInfo );
}

KAutoParty::~KAutoParty()
{
}

void KAutoParty::Tick()
{
	// 이벤트를 먼저 처리한뒤..
	KPerformer::Tick();

	// 각 State에 맞게 동작하자!
	switch( GetStateID() )
	{
	case KAutoPartyFSM::S_INIT:
		break;

	case KAutoPartyFSM::S_CLOSE:
		break;

	case KAutoPartyFSM::S_AUTO_PARTY_MAKING:
		{
			// 일정 시간동안 매치가 안이루어지면 해당 매치를 해체 시킨다. - 매치가 안되는데 계속 유저 기다리게 하는것도 

			//if( m_tTimer[MTE_MATCH_MAKING].elapsed() > 5.0 )
			//{
			//	KEGS_MATCH_MAKING_SUCCESS_NOT kNot;
			//	kNot.m_iAutoPartyUID = GetUID();
			//	BroadCast( EGB_MATCH_MAKING_SUCCESS_NOT, kNot );

			//	// 체크 상태로 전환!
			//	StateTransition( KAutoPartyFSM::I_TO_CHECK_ACCEPT );
			//}
		}
		break;

	case KAutoPartyFSM::S_CHECK_ACCEPT:
		{
			if( m_tTimer[APTE_CHECK_ACCEPT].elapsed() > 10.0 )
			{
				//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
				// 확인을 누른 유저에게는 NOT_LEAVE_AUTO_PARTY_REASON_01 - 재등록 하게끔
				// 응답이 없는 유저에게는 NOT_LEAVE_AUTO_PARTY_REASON_02 - 재등록 안됨

				std::vector< UidType > vecCancelUserList;
				m_kAutoPartyUserManager.GetAutoPartyCancelUserList( vecCancelUserList );

				CloseAutoPartyByCancel( vecCancelUserList );
#else
				// 10초가 넘었는데도 모든 유저가 수락을 하지 않았다면 강제로 자동 파타ㅣ 소멸 하자!
				//if( m_kAutoPartyUserManager.IsAllPlayersAccept() == false )
				//{
				CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
				//}
				//else
				//{
				//	// RoomUserInfo 요청하기 전에 미리 비워놓자!
				//	m_kAutoPartyUserManager.ClearAllPlayerRoomUserInfo();

				//	// 상태를 플레이 대기 상태로 바꾸자!
				//	StateTransition( KAutoPartyFSM::I_TO_PREPARE_FOR_PLAY );

				//	// 플레이 준비 타이머 초기화
				//	m_tTimer[APTE_PREPARE_FOR_PLAY].restart();

				//	// 매치를 시작하기 전에 각 유저들의 RoomUserInfo를 얻어온다!
				//	BroadCast( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ, char() );
				//}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
				//}
				
			}
		}
		break;

	case KAutoPartyFSM::S_REGROUP_PARTY:
		{
			if( m_tTimer[APTE_REGROUP_PARTY].elapsed() > 60.0 )
			{
				m_tTimer[APTE_REGROUP_PARTY].restart();

				START_LOG( cerr, L"1분동안 플레이 준비상태가 지속되었다. 강제로 자동 파티를 닫자!" )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< END_LOG;

				// 1분이상 상태변화가 없으면 문제가 있는거다!
				CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
			}
		}
		break;

	case KAutoPartyFSM::S_PLAY:
		{
			//if( m_tTimer[MTE_PLAY].elapsed() > 60.0 )
			//{
			//	m_tTimer[MTE_PLAY].restart();

			//	START_LOG( cerr, L"1분동안 플레이 상태가 지속되었다. 강제로 매치를 닫자!" )
			//		<< BUILD_LOG( GetAutoPartyUID() )
			//		<< END_LOG;

			//	// 1분이상 상태변화가 없으면 문제가 있는거다!
			//	CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
			//}
		}
		break;

	default:
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		break;
	}
}

void KAutoParty::OnDestroy()
{
	if( GetStateID() != KAutoPartyFSM::S_CLOSE )
	{
		START_LOG( cerr, L"이상한 상태에서 종료처리가 되고 있습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateID() )
			<< END_LOG;
	}
}

void KAutoParty::CloseAutoParty( IN const int iReason, IN const UidType iCancelUnitUID /*= 0*/ )
{
	// 매치 상태를 바꾸고
	StateTransition( KAutoPartyFSM::I_TO_CLOSE );

	// 매치 유저들에게 알리자!
	KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
	BroadCast( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );
}

void KAutoParty::CloseAutoPartyByCancel( IN const UidType iCancelUnitUID )
{
	// 자동 파티 상태를 바꾸고
	StateTransition( KAutoPartyFSM::I_TO_CLOSE );

	// 유저들에게 자동 파티 닫기 알림 및 자동 재신청 처리
	{
		KAutoPartyUserPtr spCancelUser = m_kAutoPartyUserManager.GetUser( iCancelUnitUID );
		if( spCancelUser == NULL )
		{
			START_LOG( cerr, L"유저 포인터값이 이상합니다." )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( iCancelUnitUID )
				<< END_LOG;
			return;
		}

		// 취소요청한 같은 파티원은 재신청 안하도록하기
		std::set< UidType > setNoSendPartyUID;
		setNoSendPartyUID.insert( spCancelUser->GetOldPartyUID() );

		// 억울하게 자동 파티가 취소된 유저들을 위해 재요청 패킷을 보내자!
		std::vector< UidType > vecUserList;
		m_kAutoPartyUserManager.GetAutoPartyUserList( vecUserList );

		// 자동 파티 재신청할 리스트 
		std::vector< UidType > vecReAutoPartyUserList;


		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
		{
			KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
			if( spAutoPartyUser == NULL )
			{
				START_LOG( cerr, L"유저 포인터값이 이상합니다." )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			// 취소 요청한 당사자인자 검사
			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );
				continue;
			}

			// 취소 요청한 당사자와 같은 파티인지 검사
			if( spCancelUser->GetOldPartyUID() != 0 )
			{
				if( spCancelUser->GetOldPartyUID() == spAutoPartyUser->GetOldPartyUID() )
				{
					KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );
					continue;
				}
			}

			// 나머지 유저들에게는 매치 닫기를 전송하자!
			KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
			kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_01;
			kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
			spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

			// 재신청을 보내지 말아야할 파티인지 체크한다.
			if( spAutoPartyUser->GetOldPartyUID() != 0 )
			{
				if( setNoSendPartyUID.find( spAutoPartyUser->GetOldPartyUID() ) != setNoSendPartyUID.end() )
					continue;

//				// 이 파티에 대해서 재 신청 메시지를 발송하였으므로 다음 파티원은 발송하지 않도록 하자!
//				setNoSendPartyUID.insert( spAutoPartyUser->GetOldPartyUID() );
			}

			if( spAutoPartyUser->IsHost( spAutoPartyUser->GetCharUID() ) == true )
			{
				// 매치 재신청할 유저들
				vecReAutoPartyUserList.push_back( spAutoPartyUser->GetCharUID() );

				// 이 파티에 대해서 재 신청 메시지를 발송하였으므로 다음 파티원은 발송하지 않도록 하자!
				setNoSendPartyUID.insert( spAutoPartyUser->GetOldPartyUID() );
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// 자동 파티 재신청!
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecReAutoPartyUserList )
		{
			KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
			if( spAutoPartyUser == NULL )
			{
				START_LOG( cerr, L"유저 포인터값이 이상합니다." )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			KEGB_REMAKING_AUTO_PARTY_NOT kPacketNot;
			kPacketNot.m_kAutoPartyInfo = m_kAutoPartyInfo;
			spAutoPartyUser->SendToGSCharacter( EGB_REMAKING_AUTO_PARTY_NOT, kPacketNot );
		}
	}
}

//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KAutoParty::CloseAutoPartyByCancel( IN const std::vector<UidType>& vecCancelUnitUID )
{
	// 자동 파티 상태를 바꾸고
	StateTransition( KAutoPartyFSM::I_TO_CLOSE );

	// 취소요청한 같은 파티원은 재신청 안하도록하기
	std::set< UidType > setNoSendPartyUID;

	// 유저들에게 자동 파티 닫기 알림 및 자동 재신청 처리

	// 억울하게 자동 파티가 취소된 유저들을 위해 재요청 패킷을 보내자!
	std::vector< UidType > vecUserList;
	m_kAutoPartyUserManager.GetAutoPartyUserList( vecUserList );

	// 자동 파티 재신청할 리스트 
	std::vector< UidType > vecReAutoPartyUserList;

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
	{
		KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"유저 포인터값이 이상합니다." )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		bool bContinue = false;

		// 취소 요청한 당사자인자 검사
		BOOST_TEST_FOREACH( UidType, iCancelUnitUID, vecCancelUnitUID )
		{
			KAutoPartyUserPtr spCancelUser = m_kAutoPartyUserManager.GetUser( iCancelUnitUID );
			if( spCancelUser == NULL )
			{
				START_LOG( cerr, L"유저 포인터값이 이상합니다." )
					<< BUILD_LOG( GetAutoPartyUID() )
					<< BUILD_LOG( iCancelUnitUID )
					<< END_LOG;
				continue;
			}

			// 취소요청한 같은 파티원은 재신청 안하도록하기
			setNoSendPartyUID.insert( spCancelUser->GetOldPartyUID() );

			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

				bContinue = true;
				break;
			}

			// 취소 요청한 당사자와 같은 파티인지 검사
			if( spCancelUser->GetOldPartyUID() != 0 )
			{
				if( spCancelUser->GetOldPartyUID() == spAutoPartyUser->GetOldPartyUID() )
				{
					KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

					bContinue = true;
					break;
				}
			}
		}
		
		// 파티 닫기를 보낸 유저이므로 넘어가자
		if( bContinue == true )
		{
			continue;
		}

		// 나머지 유저들에게는 매치 닫기를 전송하자!
		KEGS_AUTO_PARTY_CLOSE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_REASON_01;
		kPacketNot.m_iCancelUnitUID = 0;
		spAutoPartyUser->SendToGSCharacter( EGB_AUTO_PARTY_CLOSE_NOT, kPacketNot );

		// 재신청을 보내지 말아야할 파티인지 체크한다.
		if( spAutoPartyUser->GetOldPartyUID() != 0 )
		{
			if( setNoSendPartyUID.find( spAutoPartyUser->GetOldPartyUID() ) != setNoSendPartyUID.end() )
				continue;

			// 이 파티에 대해서 재 신청 메시지를 발송하였으므로 다음 파티원은 발송하지 않도록 하자!
			setNoSendPartyUID.insert( spAutoPartyUser->GetOldPartyUID() );
		}

		// 매치 재신청할 유저들
		vecReAutoPartyUserList.push_back( spAutoPartyUser->GetCharUID() );
	}


	//////////////////////////////////////////////////////////////////////////
	// 자동 파티 재신청!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecReAutoPartyUserList )
	{
		KAutoPartyUserPtr spAutoPartyUser = m_kAutoPartyUserManager.GetUser( iUnitUID );
		if( spAutoPartyUser == NULL )
		{
			START_LOG( cerr, L"유저 포인터값이 이상합니다." )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGB_REMAKING_AUTO_PARTY_NOT kPacketNot;
		kPacketNot.m_kAutoPartyInfo = m_kAutoPartyInfo;
		spAutoPartyUser->SendToGSCharacter( EGB_REMAKING_AUTO_PARTY_NOT, kPacketNot );
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}

bool KAutoParty::Enter( IN const std::vector< KAutoPartyUserInfo >& vecAutoPartyUserList )
{
	return m_kAutoPartyUserManager.Enter( vecAutoPartyUserList );
}

bool KAutoParty::CheckAutoPartyMakingSuccess()
{
	switch( GetStateID() )
	{
	case KAutoPartyFSM::S_INIT:
	case KAutoPartyFSM::S_AUTO_PARTY_MAKING:
		break;

	default:
		{
			START_LOG( cerr, L"현재 상태가 이상합니다!" )
				<< BUILD_LOG( GetAutoPartyUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		return false;
	}

	// 체크 상태로 전환!
	StateTransition( KAutoPartyFSM::I_TO_CHECK_ACCEPT );

	// 체크 타이머 초기화
	m_tTimer[APTE_CHECK_ACCEPT].restart();

	KEGS_AUTO_PARTY_MAKING_SUCCESS_NOT kNot;
	kNot.m_iAutoPartyUID = GetUID();
	kNot.m_iMemberCount = GetNumMember();
	kNot.m_iDungeonID = m_kAutoPartyInfo.m_iDungeonID;
	kNot.m_cDifficultyLevel = m_kAutoPartyInfo.m_cDifficultyLevel;
	//{{ 2012. 12. 17	박세훈	수동 파티 시 던전 팝업 창 문구 오류 수정
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	kNot.m_cDungeonMode		= m_kAutoPartyInfo.m_cDungeonMode;
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
	//}}
	BroadCast( EGB_AUTO_PARTY_MAKING_SUCCESS_NOT, kNot );
	return true;
}

bool KAutoParty::GetCreatePartyInfo( OUT KCreatePartyInfo& kInfo ) const
{
	kInfo.m_cPartyType		= KPartyInfo::PT_DUNGEON_PARTY;		
	kInfo.m_iDungeonID		= m_kAutoPartyInfo.m_iDungeonID;
	kInfo.m_DifficultyLevel = m_kAutoPartyInfo.m_cDifficultyLevel;
	kInfo.m_cGetItemType	= m_kAutoPartyInfo.m_cGetItemType;
	kInfo.m_cDungeonMode	= m_kAutoPartyInfo.m_cDungeonMode;
	kInfo.m_bCheckLowLevel	= false;

	//kInfo.m_RoomType		= CXSLRoom::RT_DUNGEON;
	//kInfo.m_RoomName		= L"AutoParty Making Dungeon Play!";
	//kInfo.m_RoomState; // 이건 머지?
	//kInfo.m_bPublic			= false;
	//kInfo.m_bTeamBalance; // 이건 머지?
	//kInfo.m_MaxSlot			= SEnum::AUTO_PARTY_TOTAL_PLAYER_COUNT;
	//kInfo.m_JoinSlot		= m_kAutoPartyUserManager.GetNumMember();
	//kInfo.m_cDungeonMode	= m_kAutoPartyInfo.m_cDungeonMode;
	//kInfo.m_iDungeonID		= m_kAutoPartyInfo.m_iDungeonID;
	//kInfo.m_DifficultyLevel = m_kAutoPartyInfo.m_cDifficultyLevel;
	//kInfo.m_cGetItemType	= 0;
	//kInfo.m_iPartyUID		= 0;
	//kInfo.m_bCanIntrude		= false;
	return true;
}

void KAutoParty::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE( EGB_CANCEL_AUTO_PARTY_MAKING_REQ );
	   _CASE( EGB_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, KEGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT );
		CASE( EGB_GET_REGROUP_PARTY_INFO_ACK );
	   _CASE( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK );

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_REQ )
{
	switch( GetStateID() )
	{
	case KAutoPartyFSM::S_AUTO_PARTY_MAKING:
	case KAutoPartyFSM::S_CHECK_ACCEPT:
	case KAutoPartyFSM::S_REGROUP_PARTY:
	case KAutoPartyFSM::S_PLAY:
		break;

	default:
		{
            START_LOG( cerr, L"이상한 상태에서 패킷이 도착하였습니다." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		break;
	}
    
	// 해당 유저가 존재하는지 검사!
	if( m_kAutoPartyUserManager.IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"매치에 포함된 유저도 아닌데 매치 취소요청이 왔네? 먼가 문제가 있군.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< BUILD_LOG( kPacket_.m_bSendBySystem )
			<< END_LOG;
		return;
	}

	// 시스템이 보낸것이 아니라면 ACK를 전송하자!
	if( kPacket_.m_bSendBySystem == false )
	{
        KEGS_CANCEL_AUTO_PARTY_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket );
	}

	// 매치를 닫자!
	CloseAutoPartyByCancel( FIRST_SENDER_UID );
}

_IMPL_ON_FUNC( EGB_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, KEGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT )
{
	VERIFY_AUTO_PARTY_STATE( ( 1, KAutoPartyFSM::S_CHECK_ACCEPT ) );

	// 수락한 유저라면 수락 체크한다!
	if( kPacket_.m_bGameStartAgree == true )
	{
		if( m_kAutoPartyUserManager.Accept( FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"수락을 실패하였습니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}
	}
	else
	{
		// 자동 파티 취소
		CloseAutoPartyByCancel( FIRST_SENDER_UID );
		return;
	}

	// 모든 유저가 수락이 완료되었다면?
	if( m_kAutoPartyUserManager.IsAllPlayersAccept() == true )
	{
		// PartyUserInfo와 RoomUserInfo요청하기 전에 미리 비워놓자!
		m_kAutoPartyUserManager.ClearAllPlayerRegroupPartyInfo();

		// 상태를 플레이 대기 상태로 바꾸자!
		StateTransition( KAutoPartyFSM::I_TO_REGROUP_PARTY );

		// 플레이 준비 타이머 초기화
		m_tTimer[APTE_REGROUP_PARTY].restart();

        // 자동 파티를 시작하기 전에 각 유저들의 RoomUserInfo를 얻어온다!
		KEGB_GET_REGROUP_PARTY_INFO_REQ kPacket;
		kPacket.m_iAutoPartyUID = GetUID();
		kPacket.m_kDungeonGameInfo.m_cDungeonMode = m_kAutoPartyInfo.m_cDungeonMode;
		kPacket.m_kDungeonGameInfo.m_iDungeonID = m_kAutoPartyInfo.m_iDungeonID;
		kPacket.m_kDungeonGameInfo.m_cDifficulty = m_kAutoPartyInfo.m_cDifficultyLevel;
		BroadCast( EGB_GET_REGROUP_PARTY_INFO_REQ, kPacket );
	}
}

IMPL_ON_FUNC( EGB_GET_REGROUP_PARTY_INFO_ACK )
{
	VERIFY_AUTO_PARTY_STATE( ( 1, KAutoPartyFSM::S_REGROUP_PARTY ) );

	// 파티 재배치를 위한 정보 업데이트!
	if( m_kAutoPartyUserManager.UpdateRegroupPartyInfo( kPacket_.m_kPartyUserInfo, kPacket_.m_kPartyRoomUserInfo ) == false )
	{
		START_LOG( cerr, L"해당 매치 유저의 RegroupPartyInfo 업데이트가 실패하였다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_iUnitUID )
			<< END_LOG;

		// 이 시점에 와서 이게 실패하면 그냥 매치를 닫을수 밖에 없다!
		CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
		return;
	}

	// 모든 매치 유저들의 PartyUserInfo와 RoomUserInfo가 수집되었는지 확인!
	if( m_kAutoPartyUserManager.IsAllPlayersUpdatedRegroupPartyInfo() == true )
	{
		// 파티 정보 새로 만들기
		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ kPacketReq;
		kPacketReq.m_iAutoPartyUID = GetUID();
		GetCreatePartyInfo( kPacketReq.m_kCreatePartyInfo );

		// 파티 regroup정보 얻기
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		if( m_kAutoPartyUserManager.GetRegroupPartyInfo( kPacketReq.m_vecRegroupPartyUserInfo, kPacketReq.m_iAutoPartyHostUnitUID ) == false )
#else
		if( m_kAutoPartyUserManager.GetRegroupPartyInfo( kPacketReq.m_vecRegroupPartyUserInfo ) == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		{
			START_LOG( cerr, L"해당 매치 유저의 RegroupPartyInfo 정보를 불러오기가 실패하였다." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;

			// 이 시점에 와서 이게 실패하면 그냥 자동파티를 닫을수 밖에 없다!
			CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
			return;
		}

		// 파티 재결성하는 인원수를 체크!
		if( kPacketReq.m_vecRegroupPartyUserInfo.size() > 4 )
		{
			START_LOG( cerr, L"자동 파티 매칭 인원수가 4명을 넘었다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kPacketReq.m_vecRegroupPartyUserInfo.size() )
				<< END_LOG;
		}
		
		// 파티를 재결성하러 가자!
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, kPacketReq );

		//////////////////////////////////////////////////////////////////////////		

		// 던전 게임을 시작했으니 상태를 바꾸자!
		StateTransition( KAutoPartyFSM::I_TO_PLAY );

		// 플레이 타이머 초기화
		m_tTimer[APTE_PLAY].restart();

		//////////////////////////////////////////////////////////////////////////
		// 마지막으로 자동 파티가 성공했던 시간 정보를 각 자동파티 메이킹 프로세스에 처리하자!
		
		//{{ 2013. 09. 30	박세훈		자동파티 대기열 난이도 통합
#ifdef SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		int iDungeonIDWithDif = m_kAutoPartyInfo.m_iDungeonID;
		switch( m_kAutoPartyInfo.m_cDungeonMode )
		{
		case CXSLDungeon::DM_HENIR_PRACTICE:
		case CXSLDungeon::DM_HENIR_CHALLENGE:
		case CXSLDungeon::DM_SECRET_NORMAL:
		case CXSLDungeon::DM_SECRET_HELL:
			iDungeonIDWithDif += m_kAutoPartyInfo.m_cDifficultyLevel;	// 난이도 값 추가
			break;

		default:
			break;
		}

		SiKAutoPartyMakingManager()->RefreshTimerAutoPartyGameStart( m_kAutoPartyInfo.m_cDungeonMode, iDungeonIDWithDif );
#else
		SiKAutoPartyMakingManager()->RefreshTimerAutoPartyGameStart( m_kAutoPartyInfo.m_cDungeonMode, m_kAutoPartyInfo.m_iDungeonID );
#endif SERV_AUTO_PARTY_DIFFICULTY_INTEGRATION
		//}}
	}
}

_IMPL_ON_FUNC( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	// 이미 닫기고 있는 상황에 이 패킷이 올수도 있다. 이럴땐 그냥 씹자!
	if( GetStateID() == KAutoPartyFSM::S_CLOSE )
		return;

	VERIFY_AUTO_PARTY_STATE( ( 1, KAutoPartyFSM::S_PLAY ) );

    // 자동 파티 재편성 실패!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
        START_LOG( cerr, L"자동 파티 재편성 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// 던전 게임 시작 실패! 자동 파티를 종료하자!
		CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_02 );
	}
	else
	{
		// 자동 파티를 종료하자!
		CloseAutoParty( NetError::NOT_LEAVE_AUTO_PARTY_REASON_00 );
	}
}

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

