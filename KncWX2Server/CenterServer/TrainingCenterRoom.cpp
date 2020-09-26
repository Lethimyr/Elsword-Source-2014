#include ".\trainingCenterRoom.h"
#include "ResultProcess.h"

#include "Room/RoomFSM.h"
#include "NetError.h"


NiImplementRTTI( KTrainingCenterRoom, KDungeonRoom );

#define CLASS_TYPE KTrainingCenterRoom

KTrainingCenterRoom::KTrainingCenterRoom()
{
	m_cDifficultyLevel	= 0;			//훈련소에서는 사용되지 않기 때문에 초기화를 시켜둔다.
}

KTrainingCenterRoom::~KTrainingCenterRoom()
{
}

ImplToStringW( KTrainingCenterRoom )
{
	return START_TOSTRING_PARENTW( KDungeonRoom );
}

//void KTrainingCenterRoom::Tick()
//{
//	KRoom::Tick();
//
//	if( GetStateID() == KRoomFSM::S_PLAY )
//	{
//
//		CheckEndPlay();
//	}
//}

void KTrainingCenterRoom::SetRoomInfo( const KTCInfo& kInfo )
{
	m_iTCID				= kInfo.m_iTCID;
	m_cRoomType			= kInfo.m_cRoomType;
	//m_uiChannelID		= kInfo.m_uiChannelID;
	m_iDungeonID		= kInfo.m_iDungeonID;
	SetPlayTimeLimit( kInfo.m_fPlayTime );
}

void KTrainingCenterRoom::ProcessNativeEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
	CASE( ERM_OPEN_TC_ROOM_REQ );
   _CASE( ERM_END_TC_GAME_REQ, KEGS_END_TC_GAME_REQ );
   _CASE( ERM_SET_TC_REMAINING_TIME_REQ, KEGS_SET_TC_REMAINING_TIME_REQ );
	//{{ 2007. 10. 23  최육사  
   _CASE( ERM_LEAVE_TC_ROOM_REQ, KERM_LEAVE_ROOM_REQ );
	//}}    

	default:
		KDungeonRoom::ProcessNativeEvent( spEvent_ );
	}
}

void KTrainingCenterRoom::StartGame()
{
	EnableRemainingPlaytime( false );	//훈련소에서는 나레이션이 들어가기때문에 클라이언트가 게임시간을 진행시키게 한다.

	KDungeonRoom::StartGame();
}

//void KTrainingCenterRoom::StartPlay()
//{
//	KDungeonRoom::StartPlay();
//}

//void KTrainingCenterRoom::EndPlay()
//{
//	KDungeonRoom::EndPlay();
//}

//void KTrainingCenterRoom::EndGame()
//{
//	return KDungeonRoom::EndGame();
//}

//bool KTrainingCenterRoom::CheckIfPlayEnd()
//{
//	return KDungeonRoom::CheckIfPlayEnd();
//}

//int KTrainingCenterRoom::DecideWinTeam()
//{
//	return KDungeonRoom::DecideWinTeam();
//}

//int KTrainingCenterRoom::GetPlayMode()
//{
//	return KDungeonRoom::GetPlayMode();
//}

//void KTrainingCenterRoom::HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS )
//{
//	KDungeonRoom::HandleTimeOutUser( mapUserCIDNGS );
//}

IMPL_ON_FUNC( ERM_OPEN_TC_ROOM_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_INIT ), ERM_OPEN_TC_ROOM_ACK, KEGS_CREATE_TC_ROOM_ACK );

	//{{ 2010. 03. 14  최육사	방 분배 기능 개선
#ifdef SERV_ROOM_COUNT
	SendRoomCountInfo( CXSLRoom::RT_TRAININGCENTER, true );
#endif SERV_ROOM_COUNT
	//}}

	SetRoomInfo( kPacket_.m_kTCInfo );

	// 방 생성이므로 슬롯 정보를 초기화 한다.
	m_spRoomUserManager->Reset();
	m_spRoomUserManager->AssignTeam( -1 );

	// 슬롯에 생성 요청자를 넣는다.
	if( !m_spRoomUserManager->EnterRoom( kPacket_.m_kRoomUserInfo, false ) )
    {
        START_LOG( cerr, L"방 입장 실패." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( GetUID() )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_ROOM_30;
        SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_TC_ROOM_ACK, kPacket );
        return;
    }

	// 방 상태를 wait->load 로 바꾼다.
	StateTransition( KRoomFSM::I_TO_WAIT );
	m_kTimer[TM_WAIT].restart();

	StartGame();

	// 방 생성 요청자에게 ack를 날린다.
	kPacket.m_iOK				= NetError::NET_OK;

	//훈련소 정보 셋팅(추가로 계속 사용할경우가 있으면 함수로 뺀다.)
	kPacket.m_kTCInfo.m_iTCID		= m_iTCID;
	kPacket.m_kTCInfo.m_RoomUID		= GetUID();
	//kPacket.m_kTCInfo.m_uiChannelID	= m_uiChannelID;
	kPacket.m_kTCInfo.m_iDungeonID	= m_iDungeonID;
	kPacket.m_kTCInfo.m_usUDPRelayPort	= m_usUDPRelayPort;
	kPacket.m_kTCInfo.m_wstrUDPRelayIP	= m_wstrUDPRelayIP;
	kPacket.m_kTCInfo.m_fPlayTime		= GetPlayTimeLimit();
    kPacket.m_kRoomUserInfo             = kPacket_.m_kRoomUserInfo;

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_OPEN_TC_ROOM_ACK, kPacket );
}

_IMPL_ON_FUNC( ERM_END_TC_GAME_REQ, KEGS_END_TC_GAME_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_END_TC_GAME_ACK, KERM_END_TC_GAME_ACK );

	kPacket.m_iOK			= NetError::NET_OK;
	kPacket.m_bIsSuccess	= kPacket_.m_bIsSuccess;
	kPacket.m_iTCID			= m_iTCID;

	//이후 방의 게임을 끝내고 삭제한다.
	if( m_spRoomUserManager->LeaveRoom( FIRST_SENDER_UID ) == false )
	{
		// 슬롯에서 나가기 실패.
		kPacket.m_iOK = NetError::ERR_SLOT_04;
	}

	KDungeonRoom::EndPlay();
	KDungeonRoom::EndGame();

	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		START_LOG( cerr, L"훈련소 방삭제 실패.! 아직 인원이 남아있다." )
			<< BUILD_LOG( m_spRoomUserManager->GetNumMember() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG
			;
	}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_END_TC_GAME_ACK, kPacket );

	//{{ 2007. 12. 18  최육사  유저 통계 [수정]
	KERM_TC_USER_STATISTICS_NOT kPacketNot;
	kPacketNot.m_iTCID		   = m_iTCID;
	kPacketNot.m_cGameResult   = ( kPacket_.m_bIsSuccess == true ? KERM_TC_USER_STATISTICS_NOT::UGR_SUCCESS : KERM_TC_USER_STATISTICS_NOT::UGR_FAIL );
	kPacketNot.m_iGamePlayTime = static_cast<int>( GetTotalPlayTime() + 0.5 );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_TC_USER_STATISTICS_NOT, kPacketNot );
	//}}
}

_IMPL_ON_FUNC( ERM_SET_TC_REMAINING_TIME_REQ, KEGS_SET_TC_REMAINING_TIME_REQ )
{
	VERIFY_ROOM_STATE( ( 1, KRoomFSM::S_PLAY ), ERM_SET_TC_REMAINING_TIME_ACK, KEGS_SET_TC_REMAINING_TIME_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	EnableRemainingPlaytime( kPacket_.m_bEnable );

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_SET_TC_REMAINING_TIME_ACK, kPacket );
}

//{{ 2007. 10. 23  최육사  
_IMPL_ON_FUNC( ERM_LEAVE_TC_ROOM_REQ, KERM_LEAVE_ROOM_REQ )
{
	// 의도적인 Leave Room 직후 접속이 끊겼을 때 한 번 더 Leave Room 하게 되는 경우가 있다.
	KEGS_LEAVE_TC_ROOM_ACK kPacket;
	if( GetStateID() == KRoomFSM::S_INIT || GetStateID() == KRoomFSM::S_CLOSE )
	{
		kPacket.m_iOK = NetError::ERR_ROOM_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_TC_ROOM_ACK, kPacket );
		return;
	}	
	
	if( GetStateID() == KRoomFSM::S_LOAD || GetStateID() == KRoomFSM::S_PLAY )
	{
		//=== 게임중 이탈 결과처리 ===
		START_LOG( clog, L"=== TRAINING GAME : 게임중 이탈시 결과처리 ===" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	// 방 나가기
	kPacket.m_iOK = NetError::NET_OK;
	if( !m_spRoomUserManager->LeaveRoom( FIRST_SENDER_UID ) )
	{
		// 방에서 나가기 실패
		kPacket.m_iOK = NetError::ERR_SLOT_04;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_TC_ROOM_ACK, kPacket );
		return;
	}
	else
	{
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_LEAVE_TC_ROOM_ACK, kPacket );

		//{{ 2007. 12. 18  최육사  유저 통계 [수정]
		KERM_TC_USER_STATISTICS_NOT kPacketNot;
		kPacketNot.m_iTCID		   = m_iTCID;
		kPacketNot.m_cGameResult   = KERM_TC_USER_STATISTICS_NOT::UGR_DROP;
		kPacketNot.m_iGamePlayTime = ( int )( m_kTimer[TM_PLAY].elapsed() + 0.5 );
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, ERM_TC_USER_STATISTICS_NOT, kPacketNot );
		//}}
	}

	// 훈련소는 무조건 EndPlay()호출
	KDungeonRoom::EndPlay();

	if( IsEmpty() == true )
	{
		StateTransition( KRoomFSM::I_TO_CLOSE );
	}
	else
	{
		START_LOG( cerr, L"훈련소 방삭제 실패.! 아직 인원이 남아있다." )
			<< BUILD_LOG( m_spRoomUserManager->GetNumMember() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG
			;
	}
}
//}}




