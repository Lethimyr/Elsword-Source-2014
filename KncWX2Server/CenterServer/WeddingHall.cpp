#include "WeddingHall.h"
#include "WeddingHallUserManager.h"
#include "NetError.h"
#include "Room/RoomFSM.h"
#include "X2Data/XSLRoom.h"
#include "CnRoomManager.h"

#include "X2Data/XSLUnitManager.h"
#include "x2data/XSLItemManager.h"

#include "GameSysVal/GameSysVal.h"

//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// 릴레이 서버(현재는 룸 서버) IP
#include "CnNetLayer.h"				// 릴리이 서버(현재는 룸 서버) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
NiImplementRTTI( KWeddingHall, KRoom );

#define CLASS_TYPE KWeddingHall

KWeddingHall::KWeddingHall()
{

	m_spUserManager = KWeddingHallUserManagerPtr( new KWeddingHallUserManager );
	SetRoomUserManager( m_spUserManager );


	m_spUserManager->Init( CXSLRoom::RT_WEDDING_HALL, SEnum::WEDDING_HALL_MAX_NUM );

	m_bEndPlay				= false;
	m_cGetItemType			= CXSLRoom::DGIT_RANDOM;
	m_cRoomType				= CXSLRoom::RT_WEDDING_HALL;
}

KWeddingHall::~KWeddingHall()
{
}

ImplToStringW( KWeddingHall )
{
    return START_TOSTRING_PARENTW( KRoom );
}

void KWeddingHall::Tick()
{
    KRoom::Tick();

	if( GetStateID() == KRoomFSM::S_PLAY )
	{
		//////////////////////////////////////////////////////////////////////////
		// 컨티뉴 타이머 체크
        CheckContinueTimer();
		//////////////////////////////////////////////////////////////////////////

	}
	else if( GetStateID() == KRoomFSM::S_WAIT )
	{
        if( m_tTimer[TE_WAIT_TIMER].elapsed() > 10.0 )
		{
			// 결혼식장의 대기 시간이 10초를 넘으면 방을 소멸 시킨다!
			StateTransition( KRoomFSM::I_TO_CLOSE );
		}
	}
}

void KWeddingHall::_BroadCast_RejectUser( int iFlag_, const KEvent& kEvent_ , const UidType iRejectUserUID )
{
	std::map< UidType, std::set<UidType> > mapUserList;
	m_spRoomUserManager->GetUserList( iFlag_, mapUserList );
	m_spRoomUserManager->GetUserList( KRoomUserManager::UL_ALL, mapUserList, KRoomUserManager::UT_OBSERVER );

	// GameServer에 대한 loop
	std::map< UidType, std::set<UidType> >::iterator mit;
	for( mit = mapUserList.begin(); mit != mapUserList.end(); mit++ )
	{
		if( iRejectUserUID == mit->first )
			continue;

		KEventPtr spEvent( kEvent_.Clone() );
		spEvent->m_anTrace[0] = mit->first;
		spEvent->m_anTrace[1] = -1;

		// User 각각에 대한 loop
		std::set<UidType>::iterator sit;
		for( sit = mit->second.begin(); sit != mit->second.end(); sit++ )
		{
			LIF( spEvent->m_kDestPerformer.AddUID( *sit ) );
		}

		KncSend( PI_CN_ROOM, GetUID(), spEvent );
	}
}

void KWeddingHall::SetRoomInfo( const KRoomInfo& kInfo )
{
    m_cRoomType			= kInfo.m_RoomType;
    m_uiRoomListID		= kInfo.m_uiRoomListID;
    m_bPublic			= kInfo.m_bPublic;
    m_bTeamBalance		= false;
    m_wstrPassword		= kInfo.m_wstrPassword;
    m_cGameType			= CXSLRoom::PGT_TEAM;
    m_cWinningNumKill	= 0;
    m_sWorldID			= kInfo.m_WorldID;
    m_cDifficultyLevel	= kInfo.m_DifficultyLevel;
    m_iDungeonID		= kInfo.m_iDungeonID;
	m_cDungeonMode		= kInfo.m_cDungeonMode;

	if( kInfo.m_cGetItemType <= CXSLRoom::DGIT_NONE || kInfo.m_cGetItemType >= CXSLRoom::DGIT_END )
	{
		START_LOG( cwarn, L"던전게임 아이템 습득타입이 잘못되었음.!" )
			<< BUILD_LOGc( m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_cGetItemType )
			<< BUILD_LOGc( kInfo.m_iDungeonID )
			<< BUILD_LOGc( kInfo.m_DifficultyLevel )
			<< END_LOG;

		m_cGetItemType		= CXSLRoom::DGIT_RANDOM;		
	}
	else
	{
		m_cGetItemType		= kInfo.m_cGetItemType;
	}

	SetName( kInfo.m_RoomName );
	SetPlayTimeLimit( kInfo.m_fPlayTime );

	m_spUserManager->SetMaxSlot( SEnum::WEDDING_HALL_MAX_NUM );
}

void KWeddingHall::GetRoomInfo( KRoomInfo& kInfo )
{
	KRoom::GetRoomInfo( kInfo );
  	kInfo.m_MaxSlot = m_spUserManager->GetMaxSlot(); // 해당 결혼식장만의 최대 슬롯 갯수를 가져오자!
}

void KWeddingHall::ProcessNativeEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
	CASE( ERM_OPEN_WEDDING_HALL_REQ );
	CASE( ERM_JOIN_WEDDING_HALL_REQ );
	CASE( ERM_LEAVE_ROOM_REQ );
	_CASE( ERM_LEAVE_WEDDING_HALL_REQ, KEGS_LEAVE_WEDDING_HALL_REQ );
	CASE_NOPARAM( ERM_CHECK_START_WEDDING_REQ );
	_CASE( ERM_START_WEDDING_ASK_NOT, KEGS_START_WEDDING_ASK_ACK );
	CASE( DBE_WEDDING_COMPLETE_ACK );
	_CASE( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ );
	_CASE( ERM_CHECK_INVALID_USER_NOT, UidType );
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음 - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
    }
}

void KWeddingHall::StartGame()
{
    KRoom::StartGame();

	// 배틀필드는 남는시간체크를 하지 않습니다.
	EnableRemainingPlaytime( false );
	
	const int iPlayerCount = m_spUserManager->GetNumMember();
    
    const int iPlayingUserCount = m_spUserManager->GetNumPlaying();
    if( iPlayingUserCount < 1 || iPlayingUserCount > m_spUserManager->GetMaxSlot() )
    {
        START_LOG( cerr, L"플레이 중인 유저 수가 이상함." )
            << BUILD_LOG( iPlayingUserCount )
            << END_LOG;
    }
}

void KWeddingHall::StartPlay()
{
    KRoom::StartPlay();
}

void KWeddingHall::EndPlay()
{
	m_bEndPlay = false;
    KRoom::EndPlay();
}

void KWeddingHall::EndGame()
{
    KRoom::EndGame();
}

int KWeddingHall::DecideWinTeam()
{
	// 배틀필드는 승패결정이 없다! 만약 호출된다고 해도 몬스터가 승리한것으로 처리하자!
	return CXSLRoom::TN_MONSTER;
}

int KWeddingHall::GetPlayMode()
{
    return m_iDungeonID + static_cast<int>(m_cDifficultyLevel);
}

void KWeddingHall::HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS )
{
    if( mapUserCIDNGS.empty() )
    {
        return;
    }

    std::map< UidType, UidType >::iterator mit;
    for( mit = mapUserCIDNGS.begin(); mit != mapUserCIDNGS.end(); ++mit )
    {
		KRoomUserPtr spRoomUser = m_spUserManager->GetUser( mit->first );
		if( spRoomUser == NULL )
		{
			START_LOG( cerr, L"해당 유저가 존재하지 않습니다!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		const UidType iUnitUID = mit->first;
		const UidType iPartyUID = spRoomUser->GetPartyUID();

		if( !m_spUserManager->LeaveRoom( mit->first ) )
		{
			START_LOG( cerr, L"방 나가기 실패." )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
		}

        // 문제 유저에게 패킷을 날린다.
        SendToGSCharacter( mit->second, mit->first, ERM_GAME_LOADING_TIME_OUT_NOT );

		// 방의 모든 유저들에게 슬롯 정보를 날린다.
		KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iUnitUID;
//		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
    }    
	
	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"배틀필드에서는 사용하지 않는 상태입니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( mapUserCIDNGS.size() )
			<< END_LOG;
	}

    // 방에서 모든 유저가 나갔으면 방을 일단 WAIT상태로 이동시킨다!
    if( IsEmpty() )
    {
		StateTransition( KRoomFSM::I_TO_RESULT );
        StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
    }
}

void KWeddingHall::DeleteZombie( std::vector<UidType>& vecZombieUnit )
{
	START_LOG( cout2, L"[알림] 배틀필드 좀비 유저 삭제!" )
		<< BUILD_LOG( vecZombieUnit.size() );

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( iUnitUID );
		if( IS_NOT_NULL( spUser ) )
		{
			START_LOG( cout2, L"[알림] 배틀필드 좀비 유저 삭제!" )
				<< BUILD_LOG( spUser->GetNickName() )
				<< BUILD_LOG( spUser->IsHost() );

			KERM_CHECK_INVALID_ZOMBIE_USER_NOT kPacket;
			kPacket.m_iRoomUID = GetUID();
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_INVALID_ZOMBIE_USER_NOT, kPacket );
		}
		else
		{
			START_LOG( cerr, L"룸유저 포인터 이상.!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
		}

		if( m_spUserManager->LeaveRoom( iUnitUID ) == false )
		{
			START_LOG( cerr, L"좀비 유저 방 나가기 실패." )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( vecZombieUnit.size() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );
	}

	// 방에서 모든 유저가 나갔으면 방을 닫는다.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecZombieUnit )
		{
			// 방의 모든 유저들에게 슬롯 정보를 날린다.
			KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
			kPacketNot.m_iReason = 0;
			kPacketNot.m_cRoomState = GetStateID();
			kPacketNot.m_iUnitUID = iUnitUID;
//			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
		}
	}
}

void KWeddingHall::CloseRoom()
{
	// 방이 닫힐때 정리할것들을 정리하자.
	
	// 배틀필드 방이 닫힐때 좀비체크를 끄자
	m_spUserManager->ZU_CheckEnd();

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	KWeddingHallInfo kHallInfo;
	GetWeddingHallInfo( kHallInfo );
	kHallInfo.m_iRoomUID = 0;
	SetWeddingHallInfo( kHallInfo );
	SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );

	//방생성중 조건이 안되어 생성을 못하는 경우라면
	if( GetStateID() == KRoomFSM::S_INIT )
	{
		StateTransition( KRoomFSM::I_TO_WAIT );
	}

	StateTransition( KRoomFSM::I_TO_CLOSE );

	//{{ 2013. 01. 07	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	// 초기화 하기 전에 검사 한번만~
	if( m_spUserManager->GetNumMember() != 0 )
	{
		START_LOG( cout, L"[경고] 결혼식장을 닫을려고 하는데 삭제되어있어야할 정보가 아직 살아있다! 이래서 버그가.." )
			<< BUILD_LOG( m_spUserManager->GetNumMember() )
			<< END_LOG;
	}

	// 방을 닫기 전에 초기화 한번 해주자!
	m_spUserManager->Reset();
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

	KCnBaseRoom::CloseRoom();
}

void KWeddingHall::CheckLoadingSuccess()
{
	// 아무것도 하지 않습니다. 로딩상태를 쓰지 않기 때문!
}

void KWeddingHall::CheckContinueTimer()
{
    // 컨티뉴 타이머를 체크하여 9초가 지나면 
}

void KWeddingHall::SendRoomListInfo( IN const int iCode )
{
	KERM_WEDDING_HALL_INFO_NOT kPacket;
	kPacket.m_cType = static_cast<char>(iCode);
	GetWeddingHallInfo( kPacket.m_kHallInfo );
	GetUnitList( kPacket.m_vecJoinUnitList );
	BroadCastAllGSServer( ERM_WEDDING_HALL_INFO_NOT, kPacket );
}

void KWeddingHall::OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck )
{
	KRoom::OnLeaveRoom( iLAST_SENDER_UID, iFIRST_SENDER_UID, kPacket_, usEventIDAck );

	//=== 게임중 이탈 결과처리 ===
	START_LOG( clog, L"=== WEDDING HALL : 결혼식장을 이탈한다고 페널티를 주지 않습니다. ===" )
		<< BUILD_LOG( iFIRST_SENDER_UID )
		<< END_LOG;	

	KEGS_LEAVE_ROOM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
	
	// 클라가 준 사유를 넣고 실패 되면 에러가 들어가게 한다.
	kPacket.m_iReason = kPacket_.m_iReason;
	
	KRoomUserPtr spRoomUser = m_spUserManager->GetUser( iFIRST_SENDER_UID );
	if( spRoomUser == NULL )
	{
		START_LOG( cerr, L"유저를 찾을 수 없습니다!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 슬롯 정보 변경.
	if( m_spUserManager->LeaveRoom( iFIRST_SENDER_UID ) == false )
	{
		// 슬롯에서 나가기 실패.
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}
		return;
	}
	else
	{
		// 배틀필드에서 이탈하는게 성공했다면 임시 인벤토리도 날려버리자!
		SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, ERM_CLEAR_TEMP_INVENTORY_NOT );

		if( kPacket_.m_bSendAck )
		{
			SendToGSCharacter( iLAST_SENDER_UID, iFIRST_SENDER_UID, usEventIDAck, kPacket );
		}

		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );
	}

	if( IsEmpty() == false )
	{
		// 방의 모든 유저들에게 슬롯 정보를 날린다.
		KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
		kPacketNot.m_iReason = usEventIDAck; // 전송 되어지는 ACK패킷타입을 넣자! 나중에 구분해서 처리할수 있도록
		kPacketNot.m_cRoomState = GetStateID();
		kPacketNot.m_iUnitUID = iFIRST_SENDER_UID;
//		GetRoomSlotInfo( kPacketNot.m_vecSlot );
		BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
	}

	if( GetStateID() == KRoomFSM::S_LOAD )
	{
		START_LOG( cerr, L"결혼식장에서는 로딩 상태를 쓰지 않습니다!" )
			<< BUILD_LOG( iFIRST_SENDER_UID )
			<< END_LOG;
	}

	// 방에서 모든 유저가 나갔으면 일단 WAIT상태로 이동시킨다!
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();
	}
}

void KWeddingHall::SetWeddingHallInfo( IN const KWeddingHallInfo& kInfo )
{
	m_kHallInfo = kInfo;
}

void KWeddingHall::GetWeddingHallInfo( OUT KWeddingHallInfo& kInfo )
{
	kInfo = m_kHallInfo;
}

void KWeddingHall::GetWeddingHallSlotInfo( std::vector< KRoomSlotInfo >& vecSlot, KRoomUserManager::USERLIST_TYPE eType )
{
	std::vector< KRoomSlotInfo > vecTempSlot;
	GetRoomSlotInfo( vecTempSlot, eType );

	BOOST_TEST_FOREACH( KRoomSlotInfo&, kRoomInfo, vecTempSlot )
	{
		if( kRoomInfo.m_kRoomUserInfo.m_nUnitUID > 0)
		{
			vecSlot.push_back( kRoomInfo );
		}
	}
}

void KWeddingHall::GetUnitList( OUT std::vector< UidType >& vecUidList )
{
	m_spUserManager->GetRoomUserList( vecUidList );
}

bool KWeddingHall::CheckWeddingHallInfo( KWeddingHallInfo& kWeddingInfo )
{
	if( m_kHallInfo.m_iWeddingUID != kWeddingInfo.m_iWeddingUID )
	{
		 START_LOG( cerr, L"결혼식장 UID 값이 다릅니다." )
			 << BUILD_LOG( m_kHallInfo.m_iWeddingUID )
			 << BUILD_LOG( kWeddingInfo.m_iWeddingUID )
			 << END_LOG;

		 return false;
	}

	if( m_kHallInfo.m_bSuccess == true )
	{
		START_LOG( cerr, L"결혼식이 완료된 방입니다." )
			<< BUILD_LOG( m_kHallInfo.m_iWeddingUID )
			<< BUILD_LOG( m_kHallInfo.m_bSuccess )
			<< END_LOG;

		return false;
	}

	return true;
}

bool KWeddingHall::IsGroomOrBride( IN UidType iUnitUID )
{
	if( m_kHallInfo.m_iGroom == iUnitUID )
	{
		return true;
	}

	if( m_kHallInfo.m_iBride == iUnitUID )
	{
		return true;
	}

	return false;
}

IMPL_ON_FUNC( ERM_OPEN_WEDDING_HALL_REQ )
{
    VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_WEDDING_HALL_ACK, KERM_OPEN_WEDDING_HALL_ACK );

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
	SendRoomCountInfo( CXSLRoom::RT_WEDDING_HALL, true );

    // 결혼식장 옵션 정보 세팅.
	kPacket_.m_kWeddingInfo.m_iRoomUID = GetUID();
	SetWeddingHallInfo( kPacket_.m_kWeddingInfo );

    // 방 생성이므로 슬롯 정보를 초기화 한다.
    m_spUserManager->Reset();
    m_spUserManager->AssignTeam( -1 );

    // 슬롯에 생성 요청자를 넣는다.
    if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) == false )
    {
        START_LOG( cerr, L"방 입장 실패. 배틀필드를 생성하는것인데 여기서 실패가 났다! 절대 일어나서는 안되는 문제!" )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
            << END_LOG;

        kPacket.m_iOK = NetError::GetLastError();
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_WEDDING_HALL_ACK, kPacket );
        return;
    }

    // 방 상태를 wait 로 바꾼다.
    StateTransition( KRoomFSM::I_TO_WAIT );
	m_tTimer[TE_WAIT_TIMER].restart();
    m_kTimer[TM_WAIT].restart();

	//모든유저상태를 레디로 바꾼다.
	m_spUserManager->SetAllReady( true );

	//gm 이상이면 센터서버의 ip를 넣어주자.
	if( m_spUserManager->CheckAuthLevel( FIRST_SENDER_UID ) == true )
	{
		kPacket.m_wstrCNIP = NetCommon::GetLocalIPW();
	}

	// 게임 초기화.
	StartGame();

	// 기존의 방 로딩 과정 없이 결혼식장에서는 바로 게임을 시작한다!
	StartPlay();

    // 방 생성 요청자에게 ack를 날린다.
    kPacket.m_iOK = NetError::NET_OK;
	GetRoomInfo( kPacket.m_RoomInfo );
	GetWeddingHallSlotInfo( kPacket.m_vecSlot );
	GetWeddingHallInfo( kPacket.m_kWeddingInfo );

	//{{ 핑 상태체크 호스트 변경 - 김민성
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();

    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_WEDDING_HALL_ACK, kPacket );

	//방생성후 좀비유저 체크시작.
	m_spUserManager->ZU_CheckStart();

	// 방 정보 전송
	SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [주의!!!] 반드시 방리스트를 먼저 보낸 이후에 이 패킷을 보내야 한다! 유저의 복귀를 위해 패킷을 다시 쏜다!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_JOIN_WEDDING_HALL_REQ )
{
	KERM_JOIN_WEDDING_HALL_ACK kPacket;
	
	if( GetStateID() != KRoomFSM::S_PLAY  &&  GetStateID() != KRoomFSM::S_WAIT )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// 제대로 찾아 왔는지 보자!
	if( CheckWeddingHallInfo( kPacket_.m_kWeddingInfo ) == false )
	{
		START_LOG( cerr, L"결혼식장에 입장했다! 헐 버그다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetWeddingHallUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// 배틀필드에 입장 처리 한다!
	if( m_spUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) == false )
	{
		START_LOG( cerr, L"결혼식장 입장 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	if( m_spUserManager->SetReady( FIRST_SENDER_UID, true ) == false )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	if( m_spUserManager->StartGame( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// 해당 유저는 바로 게임 시작 처리 하자!
	if( m_spUserManager->StartPlay( kPacket_.m_kRoomUserInfo.m_nUnitUID ) == false )
	{
		START_LOG( cerr, L"게임 시작 처리 실패!" )
			<< BUILD_LOG( kPacket_.m_kRoomUserInfo.m_nUnitUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_37;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );
		return;
	}

	// 방 상태 변화.
	if( GetStateID() == KRoomFSM::S_WAIT )
	{
		StateTransition( KRoomFSM::I_TO_LOAD );
		StateTransition( KRoomFSM::I_TO_PLAY );
	}

	// 입장한 유저에게 ack를 날린다.
	kPacket.m_iOK = NetError::NET_OK;
	GetRoomInfo( kPacket.m_RoomInfo );
	GetWeddingHallInfo( kPacket.m_kWeddingInfo );

	KRoomSlotInfo kInfoMy;
	if( m_spRoomUserManager->GetRoomSlotInfo( FIRST_SENDER_UID, kInfoMy ) == true )
	{
		kPacket.m_vecSlot.push_back( kInfoMy );
	}

	//{{ 핑 상태체크 호스트 변경 - 김민성
	kPacket.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kPacket.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_ACK, kPacket );

	// 입장한 유저에게 결혼식장에 있는 모든 유저의 정보를 하나씩 보내자
	std::vector< KRoomSlotInfo > vecTempSlot;
	GetWeddingHallSlotInfo( vecTempSlot );

	BOOST_TEST_FOREACH( KRoomSlotInfo&, kInfo, vecTempSlot )
	{
		if( kInfo.m_kRoomUserInfo.m_nUnitUID > 0 &&  kInfo.m_kRoomUserInfo.m_nUnitUID != FIRST_SENDER_UID )
		{
			KEGS_JOIN_WEDDING_HALL_NOT kPacketList;
			kPacketList.m_StartPosIndex = 0;
			kPacketList.m_JoinSlot = kInfo;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_JOIN_WEDDING_HALL_NOT, kPacketList );
		}
	}

	// 방의 모든 유저에게 NOT를 날린다.
	KEGS_JOIN_WEDDING_HALL_NOT kPacketNot;
	//080425.hoons.옵저버 상태별 유저정보를 셋팅해서 보내준다.
	m_spUserManager->KRoomUserManager::GetRoomSlotInfo( FIRST_SENDER_UID, kPacketNot.m_JoinSlot, KRoomUserManager::UT_GAME );
	kPacketNot.m_StartPosIndex = 0;
	BroadCast_RejectUser( ERM_JOIN_WEDDING_HALL_NOT, kPacketNot, FIRST_SENDER_UID );

	// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
	SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );

	//////////////////////////////////////////////////////////////////////////	
	// [주의!!!] 반드시 방리스트를 먼저 보낸 이후에 이 패킷을 보내야 한다! 유저의 복귀를 위해 패킷을 다시 쏜다!
	if( kPacket_.m_kReturnToFieldInfo.m_iRoomUID != 0 )
	{
		KERM_COMPLETE_RETURN_TO_FIELD_NOT kPacketToGS;
		kPacketToGS.m_kReturnToFieldInfo = kPacket_.m_kReturnToFieldInfo;
		SendToGSServer( LAST_SENDER_UID, ERM_COMPLETE_RETURN_TO_FIELD_NOT, kPacketToGS );
	}
}

IMPL_ON_FUNC( ERM_LEAVE_ROOM_REQ )
{
	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket_, ERM_LEAVE_ROOM_ACK );
}

_IMPL_ON_FUNC( ERM_LEAVE_WEDDING_HALL_REQ, KEGS_LEAVE_WEDDING_HALL_REQ )
{
	KERM_LEAVE_ROOM_REQ kPacket;
	kPacket.m_iReason = kPacket_.m_iReason;
#ifndef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	kPacket.m_kBattleFieldJoinInfo.m_iBattleFieldID = kPacket_.m_iBattleFieldID;
	kPacket.m_kGamePlayNetworkInfo = kPacket_.m_kGamePlayNetworkInfo;
	kPacket.m_kResultInfo = kPacket_.m_kResultInfo;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	OnLeaveRoom( LAST_SENDER_UID, FIRST_SENDER_UID, kPacket, ERM_LEAVE_WEDDING_HALL_ACK );
}

IMPL_ON_FUNC_NOPARAM( ERM_CHECK_START_WEDDING_REQ )
{
	KERM_CHECK_START_WEDDING_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	if( IsGroomOrBride( FIRST_SENDER_UID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_41;
	}

	// 물어보러가자
	bool bSendSucess = false;
	if( IsGroom( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_START_WEDDING_NOT, char() );
			bSendSucess = true;
		}
	}

	if( IsBride( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_CHECK_START_WEDDING_NOT, char() );
			bSendSucess = true;
		}
	}

	if( bSendSucess == false )
	{
		// 상대방에게 요청을 보내지 못함
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_42;
	}

	// 요청자에게 보내기
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_CHECK_START_WEDDING_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_START_WEDDING_ASK_NOT, KEGS_START_WEDDING_ASK_ACK )
{
	KEGS_START_WEDDING_AGREE_NOT kPacket;
	kPacket.m_bAgree = kPacket_.m_bAgree;

	if( IsGroom( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_START_WEDDING_AGREE_NOT, kPacket );
		}
	}
	else if( IsBride( FIRST_SENDER_UID ) == true )
	{
		KRoomUserPtr spUser = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spUser != NULL )
		{
			SendToGSCharacter( spUser->GetGSUID(), spUser->GetCID(), ERM_START_WEDDING_AGREE_NOT, kPacket );
		}
	}

	// 시작을 반대 했다면
	if( kPacket_.m_bAgree == false )
	{
		return;	
	}
	
	// 시작과 동시에 DB 기록이 되어야 한다
	KDBE_WEDDING_COMPLETE_REQ kPacketDB;
	kPacketDB.m_iWeddingUID = m_kHallInfo.m_iWeddingUID;
	kPacketDB.m_iGroom = m_kHallInfo.m_iGroom;
	kPacketDB.m_iBride = m_kHallInfo.m_iBride;
	CTime tCurr = CTime::GetCurrentTime();
	kPacketDB.m_wstrWeddingDate	 = (const wchar_t*)tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	SendToGameDB( DBE_WEDDING_COMPLETE_REQ, kPacketDB );
}

IMPL_ON_FUNC( DBE_WEDDING_COMPLETE_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KERM_START_WEDDING_NOT kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		KRoomUserPtr spBride = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spBride != NULL )
		{
			SendToGSCharacter( spBride->GetGSUID(), spBride->GetCID(), ERM_START_WEDDING_NOT, kPacket );
		}

		KRoomUserPtr spGroom = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spGroom != NULL )
		{
			SendToGSCharacter( spGroom->GetGSUID(), spGroom->GetCID(), ERM_START_WEDDING_NOT, kPacket );
		}

		return;
	}

	// 모든 유저에게 시작을 알리자 - 동시에 보상도 담아서 보내자
	{
		KERM_START_WEDDING_NOT kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		kPacket.m_iGroom = m_kHallInfo.m_iGroom;
		kPacket.m_iBride = m_kHallInfo.m_iBride;
		SiCXSLItemManager()->GetWeddingCompleteRewardItem( kPacket.m_mapRewardItem );
		BroadCast( ERM_START_WEDDING_NOT, kPacket );
	}
	
	// 해당 결혼식이 완료 되었다고 알려준다
	{
		m_kHallInfo.m_bSuccess = true;

		// 모든 게임 서버에 방 리스트를 갱신하라고 날린다.
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_DELETE );

// [개선할사항]
// 		KRoomUserPtr spHost = m_spUserManager->GetHostUser();
// 		if( spHost != NULL )
// 		{
// 			SendToGSCharacter( spHost->GetGSUID(), spHost->GetCID(), ERM_START_WEDDING_NOT, kPacket );
// 		}
	}

	// 결혼 로그 기록
	{
		KRoomUserPtr spBride = m_spUserManager->GetUser( m_kHallInfo.m_iBride );
		if( spBride != NULL )
		{
			// 로그 기록(나)
			KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
			kLog.m_iUserUID = spBride->GetUserUID();
			kLog.m_iUnitUID = spBride->GetCID();
			kLog.m_ucLevel = spBride->GetLevel();
			kLog.m_cUnitClass = spBride->GetUnitClass_LUA();
			kLog.m_cCoupleType = 2;
			kLog.m_wstrRegDate = kPacket_.m_wstrWeddingDate;
			SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );

		}

		KRoomUserPtr spGroom = m_spUserManager->GetUser( m_kHallInfo.m_iGroom );
		if( spGroom != NULL )
		{
			// 로그 기록(나)
			KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ kLog;
			kLog.m_iUserUID = spGroom->GetUserUID();
			kLog.m_iUnitUID = spGroom->GetCID();
			kLog.m_ucLevel = spGroom->GetLevel();
			kLog.m_cUnitClass = spGroom->GetUnitClass_LUA();
			kLog.m_cCoupleType = 2;
			kLog.m_wstrRegDate = kPacket_.m_wstrWeddingDate;
			SendToLogDB( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, kLog );
		}
	}
}

_IMPL_ON_FUNC( ERM_SQUARE_UNIT_SYNC_DATA_REQ, KEGS_SQUARE_UNIT_SYNC_DATA_REQ )
{
	// 아무것도 하진 않음
}

_IMPL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType )
{
	std::vector<KRoomUserManager::KLeaveUserInfo>	vecDelUserUID;
	const int iCount = m_spUserManager->DeleteUserByGsUID( kPacket_, vecDelUserUID );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_ )
		<< BUILD_LOG( iCount )
		<< BUILD_LOG( m_spUserManager->GetNumMember() );

	//현재방에서 삭제된 유저가 없으면 종료.
	if( iCount == 0 )
		return;

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		m_spUserManager->LeaveRoom( kInfo.m_iUnitUID );
	}

	BOOST_TEST_FOREACH( const KRoomUserManager::KLeaveUserInfo&, kInfo, vecDelUserUID )
	{
		if( IsEmpty() == false )
		{
			// 방의 모든 유저들에게 슬롯 정보를 날린다.
			KEGS_LEAVE_WEDDING_HALL_NOT kPacketNot;
			kPacketNot.m_cRoomState		= GetStateID();
			kPacketNot.m_iUnitUID		= kInfo.m_iUnitUID;
//			GetRoomSlotInfo( kPacketNot.m_vecSlot );
			BroadCast( ERM_LEAVE_WEDDING_HALL_NOT, kPacketNot );
		}
	}

	if( GetStateID() == KRoomFSM::S_LOAD  ||  GetStateID() == KRoomFSM::S_RESULT )
	{
		START_LOG( cerr, L"결혼에서는 사용하지 않는 상태입니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< BUILD_LOG( iCount )
			<< END_LOG;
	}

	// 방에서 모든 유저가 나갔으면 일단 WAIT상태로 바꾼다.
	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_RESULT );
		StateTransition( KRoomFSM::I_TO_WAIT );
		m_tTimer[TE_WAIT_TIMER].restart();

		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_DELETE );
	}
	else
	{
		// 배틀필드 이탈이 성공했다면 방 정보를 한번 갱신하자
		SendRoomListInfo( KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE );
	}
}
#endif SERV_RELATIONSHIP_SYSTEM
//}
