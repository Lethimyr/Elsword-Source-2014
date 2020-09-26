#include "Match.h"

#include "MatchFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "Enum/Enum.h"
#include "X2Data/XSLUnitManager.h"
//{{ 2011. 07. 18	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
#ifdef SERV_GOLBAL_SERVER_EVENT_MANAGER
#include "GameEvent/GameEventManager.h"
#endif //SERV_GOLBAL_SERVER_EVENT_MANAGER


//{{ 2011. 06. 14	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM


#define CLASS_TYPE KMatch
ImplPfID( KMatch, PI_GLOBAL_MATCH_MAKING );


KMatch::KMatch( IN const UidType iMatchUID, IN const SEnum::MATCH_TYPE eMatchType )
{
	SetUID( iMatchUID );
	m_eMatchType = eMatchType;
	m_kMatchUserManager.Init( static_cast<char>(GetMatchType()) );
}

KMatch::~KMatch()
{
}

void KMatch::Tick()
{
	// 이벤트를 먼저 처리한뒤..
	KPerformer::Tick();

	// 각 State에 맞게 동작하자!
	switch( GetStateID() )
	{
	case KMatchFSM::S_INIT:
		break;

	case KMatchFSM::S_CLOSE:
		break;

	case KMatchFSM::S_MATCH_MAKING:
		{
			// 일정 시간동안 매치가 안이루어지면 해당 매치를 해체 시킨다. - 매치가 안되는데 계속 유저 기다리게 하는것도 

			//if( m_tTimer[MTE_MATCH_MAKING].elapsed() > 5.0 )
			//{
			//	KEGS_MATCH_MAKING_SUCCESS_NOT kNot;
			//	kNot.m_iMatchUID = GetUID();
			//	BroadCast( EGB_MATCH_MAKING_SUCCESS_NOT, kNot );

			//	// 체크 상태로 전환!
			//	StateTransition( KMatchFSM::I_TO_CHECK_ACCEPT );
			//}
		}
		break;

	case KMatchFSM::S_CHECK_ACCEPT:
		{
			if( m_tTimer[MTE_CHECK_ACCEPT].elapsed() > 10.0 )
			{
				// 10초가 넘었는데도 모든 유저가 수락을 하지 않았다면 강제로 매치 소멸 하자!
				if( m_kMatchUserManager.IsAllPlayersAccept() == false )
				{
					//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
					std::vector<UidType> vecCancelUserList;
					m_kMatchUserManager.GetCancelUserList( vecCancelUserList );

					CloseMatchByCancel( vecCancelUserList );
#else
					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
#endif SERV_AUTO_MATCH_CANCEL_ERROR
					//}
				}
				else
				{
					// RoomUserInfo 요청하기 전에 미리 비워놓자!
					m_kMatchUserManager.ClearAllPlayerRoomUserInfo();

					// 상태를 플레이 대기 상태로 바꾸자!
					StateTransition( KMatchFSM::I_TO_PREPARE_FOR_PLAY );

					// 플레이 준비 타이머 초기화
					m_tTimer[MTE_PREPARE_FOR_PLAY].restart();

					// 매치를 시작하기 전에 각 유저들의 RoomUserInfo를 얻어온다!
					BroadCast( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ, char() );
				}
			}
		}
		break;

	case KMatchFSM::S_PREPARE_FOR_PLAY:
		{
			//if( m_tTimer[MTE_PREPARE_FOR_PLAY].elapsed() > 60.0 )
			//{
			//	m_tTimer[MTE_PREPARE_FOR_PLAY].restart();

			//	START_LOG( cerr, L"1분동안 플레이 준비상태가 지속되었다. 강제로 매치를 닫자!" )
			//		<< BUILD_LOG( GetMatchUID() )
			//		<< END_LOG;

			//	// 1분이상 상태변화가 없으면 문제가 있는거다!
			//	CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
			//}
		}
		break;

	case KMatchFSM::S_PLAY:
		{
			//if( m_tTimer[MTE_PLAY].elapsed() > 60.0 )
			//{
			//	m_tTimer[MTE_PLAY].restart();

			//	START_LOG( cerr, L"1분동안 플레이 상태가 지속되었다. 강제로 매치를 닫자!" )
			//		<< BUILD_LOG( GetMatchUID() )
			//		<< END_LOG;

			//	// 1분이상 상태변화가 없으면 문제가 있는거다!
			//	CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
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

void KMatch::OnDestroy()
{
	if( GetStateID() != KMatchFSM::S_CLOSE )
	{
		START_LOG( cerr, L"이상한 상태에서 종료처리가 되고 있습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetStateID() )
			<< END_LOG;
	}
}

void KMatch::CloseMatch( IN const int iReason, IN const UidType iCancelUnitUID /*= 0*/ )
{	
	// 매치 상태를 바꾸고
	StateTransition( KMatchFSM::I_TO_CLOSE );

	// 매치 유저들에게 알리자!
	KEGS_MATCH_CLOSE_NOT kPacketNot;
	kPacketNot.m_iReason = iReason;
	kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
	BroadCast( EGB_MATCH_CLOSE_NOT, kPacketNot );
}

void KMatch::CloseMatchByCancel( IN const UidType iCancelUnitUID )
{
	// 매치 상태를 바꾸고
	StateTransition( KMatchFSM::I_TO_CLOSE );

	// 유저들에게 매치 닫기 알림 및 자동 재신청 처리
	{
		KMatchUserPtr spCancelUser = m_kMatchUserManager.GetUser( iCancelUnitUID );
		if( spCancelUser == NULL )
		{
			START_LOG( cerr, L"유저 포인터값이 이상합니다." )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( iCancelUnitUID )
				<< END_LOG;
			return;
		}

		// 취소요청한 같은 파티원은 재신청 안하도록하기
		std::set< UidType > setNoSendPartyUID;
		setNoSendPartyUID.insert( spCancelUser->GetPartyUID() );

		// 억울하게 매칭이 취소된 유저들을 위해 재요청 패킷을 보내자!
		std::vector< UidType > vecUserList;
		m_kMatchUserManager.GetMatchUserList( vecUserList );

		// 매치 재신청할 리스트 
		std::vector< UidType > vecRematchUserList;


		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
		{
			KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"유저 포인터값이 이상합니다." )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			// 취소 요청한 당사자인자 검사
			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_MATCH_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );
				continue;
			}

			// 취소 요청한 당사자와 같은 파티인지 검사
			if( spCancelUser->GetPartyUID() != 0 )
			{
				if( spCancelUser->GetPartyUID() == spMatchUser->GetPartyUID() )
				{
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );
					continue;
				}
			}

			// 나머지 유저들에게는 매치 닫기를 전송하자!
			KEGS_MATCH_CLOSE_NOT kPacketNot;
			kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
			kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
			spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

			// 재신청을 보내지 말아야할 파티인지 체크한다.
			if( spMatchUser->GetPartyUID() != 0 )
			{
				if( setNoSendPartyUID.find( spMatchUser->GetPartyUID() ) != setNoSendPartyUID.end() )
					continue;

				// 이 파티에 대해서 재 신청 메시지를 발송하였으므로 다음 파티원은 발송하지 않도록 하자!
				setNoSendPartyUID.insert( spMatchUser->GetPartyUID() );
			}

			// 매치 재신청할 유저들
			vecRematchUserList.push_back( spMatchUser->GetCharUID() );
		}


		//////////////////////////////////////////////////////////////////////////
		// 매치 재신청!
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRematchUserList )
		{
			KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
			if( spMatchUser == NULL )
			{
				START_LOG( cerr, L"유저 포인터값이 이상합니다." )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOG( iUnitUID )
					<< END_LOG;
				continue;
			}

			KEGB_REMATCH_MAKING_USER_NOT kPacketNot;
			kPacketNot.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
			kPacketNot.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
			spMatchUser->SendToGSCharacter( EGB_REMATCH_MAKING_USER_NOT, kPacketNot );
		}
	}
}

//{{ 2012. 11. 28 던전,대전 자동 매칭 취소 되지 않는 오류 수정(예외 상황) - 김민성
#ifdef SERV_AUTO_MATCH_CANCEL_ERROR
void KMatch::CloseMatchByCancel( IN const std::vector<UidType>& vecCancelUnitUID )
{
	// 매치 상태를 바꾸고
	StateTransition( KMatchFSM::I_TO_CLOSE );

	// 취소요청한 같은 파티원은 재신청 안하도록하기
	std::set< UidType > setNoSendPartyUID;

	// 유저들에게 자동 파티 닫기 알림 및 자동 재신청 처리

	// 억울하게 자동 파티가 취소된 유저들을 위해 재요청 패킷을 보내자!
	std::vector< UidType > vecUserList;
	m_kMatchUserManager.GetMatchUserList( vecUserList );

	// 매치 재신청할 리스트 
	std::vector< UidType > vecRematchUserList;

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecUserList )
	{
		KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"유저 포인터값이 이상합니다." )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		bool bContinue = false;

		// 취소 요청한 당사자인자 검사
		BOOST_TEST_FOREACH( UidType, iCancelUnitUID, vecCancelUnitUID )
		{
			KMatchUserPtr spCancelUser = m_kMatchUserManager.GetUser( iCancelUnitUID );
			if( spCancelUser == NULL )
			{
				START_LOG( cerr, L"유저 포인터값이 이상합니다." )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOG( iCancelUnitUID )
					<< END_LOG;
				continue;
			}

			// 취소요청한 같은 파티원은 재신청 안하도록하기
			setNoSendPartyUID.insert( spCancelUser->GetPartyUID() );

			// 취소 요청한 당사자인자 검사
			if( iUnitUID == iCancelUnitUID )
			{
				KEGS_MATCH_CLOSE_NOT kPacketNot;
				kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_02;
				kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
				spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

				bContinue = true;
				continue;
			}

			// 취소 요청한 당사자와 같은 파티인지 검사
			if( spCancelUser->GetPartyUID() != 0 )
			{
				if( spCancelUser->GetPartyUID() == spMatchUser->GetPartyUID() )
				{
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_03;
					kPacketNot.m_iCancelUnitUID = iCancelUnitUID;
					spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

					bContinue = true;
					continue;
				}
			}
		}

		// 파티 닫기를 보낸 유저이므로 넘어가자
		if( bContinue == true )
		{
			continue;
		}

		// 나머지 유저들에게는 매치 닫기를 전송하자!
		KEGS_MATCH_CLOSE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
		kPacketNot.m_iCancelUnitUID = 0;
		spMatchUser->SendToGSCharacter( EGB_MATCH_CLOSE_NOT, kPacketNot );

		// 재신청을 보내지 말아야할 파티인지 체크한다.
		if( spMatchUser->GetPartyUID() != 0 )
		{
			if( setNoSendPartyUID.find( spMatchUser->GetPartyUID() ) != setNoSendPartyUID.end() )
				continue;

			// 이 파티에 대해서 재 신청 메시지를 발송하였으므로 다음 파티원은 발송하지 않도록 하자!
			setNoSendPartyUID.insert( spMatchUser->GetPartyUID() );
		}

		// 매치 재신청할 유저들
		vecRematchUserList.push_back( spMatchUser->GetCharUID() );
	}

	//////////////////////////////////////////////////////////////////////////
	// 매치 재신청!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecRematchUserList )
	{
		KMatchUserPtr spMatchUser = m_kMatchUserManager.GetUser( iUnitUID );
		if( spMatchUser == NULL )
		{
			START_LOG( cerr, L"유저 포인터값이 이상합니다." )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGB_REMATCH_MAKING_USER_NOT kPacketNot;
		kPacketNot.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
		kPacketNot.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
		spMatchUser->SendToGSCharacter( EGB_REMATCH_MAKING_USER_NOT, kPacketNot );
	}
}
#endif SERV_AUTO_MATCH_CANCEL_ERROR
//}


bool KMatch::Enter( IN const std::vector< KMatchUserInfo >& vecMatchUserList )
{
	return m_kMatchUserManager.Enter( vecMatchUserList );
}

bool KMatch::CheckOptimalMatch( IN KMatchUserWaitListPtr spWaitList, OUT int& iOptimalMatchPoint )
{
	//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	iOptimalMatchPoint = ::abs( m_kMatchUserManager.GetRatingWeightAverage() - spWaitList->GetRatingWeightAverage() );
#else
	iOptimalMatchPoint = ::abs( m_kMatchUserManager.GetRatingAverage() - spWaitList->GetRatingAverage() );
#endif SERV_2012_PVP_SEASON2
	//}}
	return true;
}

bool KMatch::CheckMatchMakingSuccess()
{
	switch( GetStateID() )
	{
	case KMatchFSM::S_INIT:
	case KMatchFSM::S_MATCH_MAKING:
		break;

	default:
		{
			START_LOG( cerr, L"현재 상태가 이상합니다!" )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOG( GetMatchType() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
		}
		return false;
	}

	// 인원수 최종 확인!
	if( KMatch::GetMatchTotalPlayerCount( GetMatchType() ) != GetNumMember() )
	{
		START_LOG( clog, L"대전 매칭에 필요한 인원수가 충족되지 않았습니다." )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOG( GetMatchType() )
			<< BUILD_LOG( KMatch::GetMatchTotalPlayerCount( GetMatchType() ) )
			<< BUILD_LOG( GetNumMember() )
			<< END_LOG;

		// 계속 매치 메이킹 진행..
		if( GetStateID() != KMatchFSM::S_MATCH_MAKING )
		{
			StateTransition( KMatchFSM::I_TO_MATCH_MAKING );
		}
		return false;
	}

	// 체크 상태로 전환!
	StateTransition( KMatchFSM::I_TO_CHECK_ACCEPT );

	// 체크 타이머 초기화
	m_tTimer[MTE_CHECK_ACCEPT].restart();

	KEGS_MATCH_MAKING_SUCCESS_NOT kNot;
	kNot.m_iMatchUID = GetUID();
	BroadCast( EGB_MATCH_MAKING_SUCCESS_NOT, kNot );
	return true;
}

CXSLRoom::PVP_GAME_TYPE KMatch::GetMatchPvpGameType( IN const SEnum::MATCH_TYPE eMatchType )
{
	switch( eMatchType )
	{
	case SEnum::MT_TEAM_1_ON_1:
	case SEnum::MT_TEAM_2_ON_2:
	case SEnum::MT_TEAM_3_ON_3:
		return CXSLRoom::PGT_TEAM;

	case SEnum::MT_TEAM_DEATH_1_ON_1:
	case SEnum::MT_TEAM_DEATH_2_ON_2:
	case SEnum::MT_TEAM_DEATH_3_ON_3:
		return CXSLRoom::PGT_TEAM_DEATH;
	}

	return CXSLRoom::PGT_NULL;
}

int KMatch::GetMatchPlayerCount( IN const SEnum::MATCH_TYPE eMatchType )
{
	switch( eMatchType )
	{
	case SEnum::MT_TEAM_1_ON_1:
	case SEnum::MT_TEAM_DEATH_1_ON_1:
		return 1;

	case SEnum::MT_TEAM_2_ON_2:
	case SEnum::MT_TEAM_DEATH_2_ON_2:
		return 2;

	case SEnum::MT_TEAM_3_ON_3:
	case SEnum::MT_TEAM_DEATH_3_ON_3:
		return 3;
	}

	return 0;
}

int KMatch::GetMatchTotalPlayerCount( IN const SEnum::MATCH_TYPE eMatchType )
{
    return ( GetMatchPlayerCount( eMatchType ) * 2 );
}

bool KMatch::GetRoomInfoForPvpGame( IN const bool bIsNpcPvpMode, OUT KRoomInfo& kInfo ) const
{
	kInfo.m_RoomType		= CXSLRoom::RT_PVP;
	kInfo.m_RoomName		= L"Match Making Battle!";
	kInfo.m_RoomState; // 이건 머지?
	kInfo.m_bPublic			= false;
	kInfo.m_bTeamBalance; // 이건 머지?
	kInfo.m_MaxSlot			= KMatch::GetMatchTotalPlayerCount( GetMatchType() );
	kInfo.m_JoinSlot		= m_kMatchUserManager.GetNumMember();
	kInfo.m_PVPGameType		= KMatch::GetMatchPvpGameType( GetMatchType() );
#ifdef DUNGEON_ITEM
	//m_kMatchUserManager.DecisionGameMode( kInfo.m_bIsItemMode ); // 차후에 도입할 시스템
	kInfo.m_bIsItemMode		= true; // 공식대전은 아이템전!
#endif DUNGEON_ITEM
	kInfo.m_iPVPChannelClass = KPVPChannelInfo::PCC_OFFICIAL;
	kInfo.m_bCanIntrude		= false;	

	// 대전 정보 얻기
	if( SiKPvpMatchManager()->GetPvpMatchInfo( static_cast<int>(GetMatchType()), kInfo.m_WinKillNum, kInfo.m_fPlayTime ) == false )
	{
		START_LOG( cerr, L"대전 정보 얻기 실패!" )
			<< BUILD_LOG( GetMatchType() )
			<< BUILD_LOG( GetMatchUID() )
			<< END_LOG;
		return false;
	}

	// 각 멤버들이 선택한 고르고 싶지 않은 mapID 리스트를 정리하자!
	std::map< short, int > mapAllPlayersSelectedMap;
	m_kMatchUserManager.GetAllPlayersSelectedPvpMap( mapAllPlayersSelectedMap );

	// 대전 맵을 결정하자!
	if( SiKPvpMatchManager()->DecisionPvpMap( bIsNpcPvpMode, mapAllPlayersSelectedMap, kInfo.m_WorldID ) == false )
	{
		START_LOG( cerr, L"대전 맵 결정 실패!" )
			<< BUILD_LOG( GetMatchType() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOG( bIsNpcPvpMode )
			<< BUILD_LOG( mapAllPlayersSelectedMap.size() )
			<< END_LOG;
		return false;
	}
	else
	{
		kInfo.m_ShowPvpMapWorldID = kInfo.m_WorldID;
	}	

#ifdef SERV_PVP_REMATCH
	// 나중에도 이용하기 위해
	kInfo.m_mapAllPlayersSelectedMap = mapAllPlayersSelectedMap;
#endif SERV_PVP_REMATCH

	return true;
}

bool KMatch::GetMatchTypeByMatchInfo( IN const char cPvpGameType, IN const char cPvpPlayerCount, OUT SEnum::MATCH_TYPE& eMatchType )
{
    switch( cPvpGameType )
	{
	case CXSLRoom::PGT_TEAM:
		{
			switch( cPvpPlayerCount )
			{
			case 1: eMatchType = SEnum::MT_TEAM_1_ON_1; return true;
			case 2: eMatchType = SEnum::MT_TEAM_2_ON_2; return true;
			case 3: eMatchType = SEnum::MT_TEAM_3_ON_3; return true;
			}
		}
		break;

	case CXSLRoom::PGT_TEAM_DEATH:
		{
			switch( cPvpPlayerCount )
			{
			case 1: eMatchType = SEnum::MT_TEAM_DEATH_1_ON_1; return true;
			case 2: eMatchType = SEnum::MT_TEAM_DEATH_2_ON_2; return true;
			case 3: eMatchType = SEnum::MT_TEAM_DEATH_3_ON_3; return true;
			}
		}
		break;
	}

	START_LOG( cerr, L"잘못된 대전 게임 타입 정보입니다!" )
		<< BUILD_LOGc( cPvpGameType )
		<< BUILD_LOGc( cPvpPlayerCount )
		<< END_LOG;
	return false;	
}

void KMatch::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE( EGB_CANCEL_MATCH_MAKING_REQ );
	   _CASE( EGB_MATCH_MAKING_SUCCESS_REPLY_NOT, KEGS_MATCH_MAKING_SUCCESS_REPLY_NOT );
		CASE( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK );
		CASE( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK );

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_REQ )
{
	switch( GetStateID() )
	{
	case KMatchFSM::S_MATCH_MAKING:
	case KMatchFSM::S_CHECK_ACCEPT:
	case KMatchFSM::S_PREPARE_FOR_PLAY:
	case KMatchFSM::S_PLAY:
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
	if( m_kMatchUserManager.IsExist( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"매치에 포함된 유저도 아닌데 매치 취소요청이 왔네? 먼가 문제가 있군.." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iMatchUID )
			<< BUILD_LOG( kPacket_.m_bSendBySystem )
			<< END_LOG;
		return;
	}

	// 시스템이 보낸것이 아니라면 ACK를 전송하자!
	if( kPacket_.m_bSendBySystem == false )
	{
        KEGS_CANCEL_MATCH_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_CANCEL_MATCH_MAKING_ACK, kPacket );
	}

	// 매치를 닫자!
	CloseMatchByCancel( FIRST_SENDER_UID );
}

_IMPL_ON_FUNC( EGB_MATCH_MAKING_SUCCESS_REPLY_NOT, KEGS_MATCH_MAKING_SUCCESS_REPLY_NOT )
{
	VERIFY_MATCH_STATE( ( 1, KMatchFSM::S_CHECK_ACCEPT ) );

	// 수락한 유저라면 수락 체크한다!
	if( kPacket_.m_bGameStartAgree == true )
	{
		if( m_kMatchUserManager.Accept( FIRST_SENDER_UID ) == false )
		{
			START_LOG( cerr, L"수락을 실패하였습니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}

		if( m_kMatchUserManager.UpdateSelectedPvpMap( FIRST_SENDER_UID, kPacket_.m_setSelectedPvpMap ) == false )
		{
			START_LOG( cerr, L"매치 유저 정보에 대전 맵 선택 업데이트 실패!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_setSelectedPvpMap.size() )
				<< END_LOG;
			return;
		}

		if( m_kMatchUserManager.UpdatePvpGameMode( FIRST_SENDER_UID, kPacket_.m_bPvpPowerMode, kPacket_.m_bPvpItemMode ) == false )
		{
			START_LOG( cerr, L"매치 유저 정보에 대전 모드 선택 업데이트 실패!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_bPvpPowerMode )
				<< BUILD_LOG( kPacket_.m_bPvpItemMode )
				<< END_LOG;
			return;
		}
	}
	else
	{
		// 매치 취소
		CloseMatchByCancel( FIRST_SENDER_UID );
		return;
	}

	// 모든 유저가 수락이 완료되었다면?
	if( m_kMatchUserManager.IsAllPlayersAccept() == true )
	{
		// RoomUserInfo 요청하기 전에 미리 비워놓자!
		m_kMatchUserManager.ClearAllPlayerRoomUserInfo();

		// 상태를 플레이 대기 상태로 바꾸자!
		StateTransition( KMatchFSM::I_TO_PREPARE_FOR_PLAY );

		// 플레이 준비 타이머 초기화
		m_tTimer[MTE_PREPARE_FOR_PLAY].restart();

        // 매치를 시작하기 전에 각 유저들의 RoomUserInfo를 얻어온다!
		BroadCast( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ, char() );
	}
}

IMPL_ON_FUNC( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK )
{
	VERIFY_MATCH_STATE( ( 1, KMatchFSM::S_PREPARE_FOR_PLAY ) );

	// roomuserinfo를 업데이트 하자!
	if( m_kMatchUserManager.UpdateRoomUserInfo( kPacket_.m_kRoomUserInfo ) == false )
	{
		START_LOG( cerr, L"해당 매치 유저의 RoomUserInfo 업데이트가 실패하였다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< END_LOG;

		// 이 시점에 와서 이게 실패하면 그냥 매치를 닫을수 밖에 없다!
		CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
		return;
	}

	// 모든 매치 유저들의 RoomUserInfo가 수집되었는지 확인!
	if( m_kMatchUserManager.IsAllPlayersUpdatedRoomUserInfo() == true )
	{
		//////////////////////////////////////////////////////////////////////////
		// 1. 모든 유저의 RoomUserInfo가 수집되었으면 이제 편을 나누자!
		std::vector< KRoomUserInfo > vecRedTeam;
		std::vector< KRoomUserInfo > vecBlueTeam;
		if( m_kMatchUserManager.TeamMemberSelection( vecRedTeam, vecBlueTeam ) == false )
		{
            START_LOG( cerr, L"팀 선정에 실패하였다. 절대로 일어나서는 안되는 에러! 매치를 종료 시키자!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			// 이 시점에 와서 이게 실패하면 그냥 매치를 닫을수 밖에 없다!
			CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
			return;
		}

		//////////////////////////////////////////////////////////////////////////		
		// 2. NPC대전을 할지 말지 결정!
		SEnum::PVP_NPC_TYPE eRedTeamPvpNpcMode = SEnum::PNT_NONE;
		SEnum::PVP_NPC_TYPE eBlueTeamPvpNpcMode = SEnum::PNT_NONE;

		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		const bool bIsAllPlayersBeginner = m_kMatchUserManager.IsAllPlayersBeginner();
		const int iRedTeamRatingWeightAverage = KMatchUserManager::GetRatingWeightAverage( vecRedTeam );
		const int iBlueTeamRatingWeightAverage = KMatchUserManager::GetRatingWeightAverage( vecBlueTeam );

		if( bIsAllPlayersBeginner == true )
		{
			eRedTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
			eBlueTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
		}
		else if( m_kMatchUserManager.ExistArrangeRankPlayer() == false )
		{
			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecRedTeam ) == true )
			{
				eRedTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}

			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecBlueTeam ) == true )
			{
				eBlueTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}
		}
#else
		// 모든 유저가 초보 유저인지 체크한다!
		const bool bIsAllPlayersBeginner = m_kMatchUserManager.IsAllPlayersSameRank( CXSLUnit::PVPRANK_RANK_F );

		if( bIsAllPlayersBeginner )
		{
			eRedTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
			eBlueTeamPvpNpcMode = SEnum::PNT_BEGINNER_NPC;
		}
		else
		{
			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecRedTeam ) == true )
			{
				eRedTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}

			if( SiKPvpMatchManager()->DecisionNpcPvpMode( vecBlueTeam ) == true )
			{
				eBlueTeamPvpNpcMode = SEnum::PNT_HERO_NPC;
			}
		}
#endif SERV_2012_PVP_SEASON2
		//}}

		//////////////////////////////////////////////////////////////////////////		
		// 3. 일반 대전이다!
		if( eRedTeamPvpNpcMode == SEnum::PNT_NONE  &&  eBlueTeamPvpNpcMode == SEnum::PNT_NONE )
		{
			KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ kPacketReq;

			// 대전 방 정보 얻기
			if( GetRoomInfoForPvpGame( false, kPacketReq.m_kRoomInfo ) == false )
			{
				START_LOG( cerr, L"대전방 정보를 얻는데 실패하였다. 절대로 일어나서는 안되는 에러! 매치를 종료 시키자!" )
					<< BUILD_LOG( GetUID() )
					<< END_LOG;

				CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
				return;
			}

			kPacketReq.m_cMatchType = static_cast<char>(GetMatchType());
			kPacketReq.m_cPvpNpcType = static_cast<char>(SEnum::PNT_NONE);
			kPacketReq.m_iMatchUID = GetUID();
			kPacketReq.m_vecRedTeam = vecRedTeam;
			kPacketReq.m_vecBlueTeam = vecBlueTeam;
			m_kMatchUserManager.GetMatchWaitTimeList( kPacketReq.m_mapMatchWaitTime );
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacketReq );
		}

		if( eRedTeamPvpNpcMode != SEnum::PNT_NONE )
		{
			BOOST_TEST_FOREACH( const KRoomUserInfo&, kRedTeamUser, vecRedTeam )
			{
				KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ kPacketReq;

				// 대전방 정보 얻기
				if( GetRoomInfoForPvpGame( true, kPacketReq.m_kRoomInfo ) == false )
				{
					START_LOG( cerr, L"대전방 정보를 얻는데 실패하였다. 절대로 일어나서는 안되는 에러! 매치를 종료 시키자!" )
						<< BUILD_LOG( GetUID() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				// 대전 NPC정보 불러오기!
				//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, iRedTeamRatingWeightAverage, vecRedTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#else
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, vecRedTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#endif SERV_2012_PVP_SEASON2
					//}}
				{
					START_LOG( cerr, L"대전 NPC 정보를 얻는데 실패하였다. 절대로 일어나서는 안되는 에러! 매치를 종료 시키자!" )
						<< BUILD_LOG( GetUID() )
						<< BUILD_LOG( bIsAllPlayersBeginner )
						<< BUILD_LOG( vecRedTeam.size() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				kPacketReq.m_cMatchType = static_cast<char>(GetMatchType());
				kPacketReq.m_cPvpNpcType = static_cast<char>(eRedTeamPvpNpcMode);
				kPacketReq.m_iMatchUID = GetUID();
				kPacketReq.m_vecRedTeam = vecRedTeam; // 레드팀 유저는 그대로 레드팀!
				m_kMatchUserManager.GetMatchWaitTimeList( kPacketReq.m_mapMatchWaitTime );
				SendToGSCharacter( kRedTeamUser.m_iGSUID, kRedTeamUser.m_nUnitUID, EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacketReq );
				break;
			}

			if( eBlueTeamPvpNpcMode == SEnum::PNT_NONE )
			{
				BOOST_TEST_FOREACH( const KRoomUserInfo&, kRoomUserInfo, vecBlueTeam )
				{
					// 매치 유저들에게 알리자!
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_MATCH_CLOSE_NOT, kPacketNot );

					// 매치에서 이탈 시키자!
					LIF( m_kMatchUserManager.DeleteUser( kRoomUserInfo.m_nUnitUID ) );

#ifdef SERV_FIX_PVP_NPC_AUTO_REMATCH// 작업날짜: 2013-05-24	// 박세훈
					// 매치 재신청!
					KEGB_REMATCH_MAKING_USER_NOT kPacketNotForRematch;
					kPacketNotForRematch.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
					kPacketNotForRematch.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_REMATCH_MAKING_USER_NOT, kPacketNotForRematch );
#endif // SERV_FIX_PVP_NPC_AUTO_REMATCH
				}
			}
		}

		if( eBlueTeamPvpNpcMode != SEnum::PNT_NONE )
		{
			BOOST_TEST_FOREACH( const KRoomUserInfo&, kBlueTeamUser, vecBlueTeam )
			{
				KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ kPacketReq;

				// 대전 방 정보 얻기
				if( GetRoomInfoForPvpGame( true, kPacketReq.m_kRoomInfo ) == false )
				{
					START_LOG( cerr, L"대전방 정보를 얻는데 실패하였다. 절대로 일어나서는 안되는 에러! 매치를 종료 시키자!" )
						<< BUILD_LOG( GetUID() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				// 대전 NPC정보 불러오기!
				//{{ 2012. 09. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, iBlueTeamRatingWeightAverage, vecBlueTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#else
				if( SiKPvpMatchManager()->GetPvpNpcInfo( bIsAllPlayersBeginner, vecBlueTeam.size(), kPacketReq.m_vecBlueTeam, kPacketReq.m_mapPvpNpcInfo ) == false )
#endif SERV_2012_PVP_SEASON2
					//}}
				{
					START_LOG( cerr, L"대전 NPC 정보를 얻는데 실패하였다. 절대로 일어나서는 안되는 에러! 매치를 종료 시키자!" )
						<< BUILD_LOG( GetUID() )
						<< BUILD_LOG( bIsAllPlayersBeginner )
						<< BUILD_LOG( vecRedTeam.size() )
						<< END_LOG;

					CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
					return;
				}

				kPacketReq.m_cMatchType = static_cast<char>(GetMatchType());
				kPacketReq.m_cPvpNpcType = static_cast<char>(eBlueTeamPvpNpcMode);
				kPacketReq.m_iMatchUID = GetUID();
				kPacketReq.m_vecRedTeam = vecBlueTeam; // 블루팀 유저들을 레드팀으로 설정하자!
				m_kMatchUserManager.GetMatchWaitTimeList( kPacketReq.m_mapMatchWaitTime );
				SendToGSCharacter( kBlueTeamUser.m_iGSUID, kBlueTeamUser.m_nUnitUID, EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacketReq );
				break;
			}

			if( eRedTeamPvpNpcMode == SEnum::PNT_NONE )
			{
				BOOST_TEST_FOREACH( const KRoomUserInfo&, kRoomUserInfo, vecRedTeam )
				{
					// 매치 유저들에게 알리자!
					KEGS_MATCH_CLOSE_NOT kPacketNot;
					kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_01;
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_MATCH_CLOSE_NOT, kPacketNot );

					// 매치에서 이탈 시키자!
					LIF( m_kMatchUserManager.DeleteUser( kRoomUserInfo.m_nUnitUID ) );

#ifdef SERV_FIX_PVP_NPC_AUTO_REMATCH// 작업날짜: 2013-05-24	// 박세훈
					// 매치 재신청!
					KEGB_REMATCH_MAKING_USER_NOT kPacketNotForRematch;
					kPacketNotForRematch.m_cPvpGameType = KMatch::GetMatchPvpGameType( GetMatchType() );
					kPacketNotForRematch.m_cPvpPlayerCount = KMatch::GetMatchPlayerCount( GetMatchType() );
					SendToGSCharacter( kRoomUserInfo.m_iGSUID, kRoomUserInfo.m_nUnitUID, EGB_REMATCH_MAKING_USER_NOT, kPacketNotForRematch );
#endif // SERV_FIX_PVP_NPC_AUTO_REMATCH
				}
			}
		}		

		//////////////////////////////////////////////////////////////////////////		

		// 대전 게임을 시작했으니 상태를 바꾸자!
		StateTransition( KMatchFSM::I_TO_PLAY );

		// 플레이 타이머 초기화
		m_tTimer[MTE_PLAY].restart();
	}
}

IMPL_ON_FUNC( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK )
{
	// 이미 닫기고 있는 상황에 이 패킷이 올수도 있다. 이럴땐 그냥 씹자!
	if( GetStateID() == KMatchFSM::S_CLOSE )
		return;

	VERIFY_MATCH_STATE( ( 1, KMatchFSM::S_PLAY ) );

    // 매치 대전방 생성 실패!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
        START_LOG( cerr, L"매치 대전방 생성 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// 대전 게임 시작 실패! 매치 종료하자!
		CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_02 );
	}
	else
	{
		// 매칭을 종료하자!
		CloseMatch( NetError::NOT_LEAVE_MATCH_REASON_00 );
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}

