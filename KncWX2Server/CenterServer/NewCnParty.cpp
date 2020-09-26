#include "NewCnParty.h"
//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewCnPartyManager.h"
	#include "X2Data/XSLBattleFieldManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "CnPartyFSM.h"
#include "NetError.h"
#include "X2Data/XSLRoom.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLPartyManager.h"
#include "Lottery.h"
//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "Enum/Enum.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
#include "Socket/NetCommon.h"		// 릴레이 서버(현재는 룸 서버) IP
#include "CnNetLayer.h"				// 릴리이 서버(현재는 룸 서버) Port
#endif SERV_CHOOSE_FASTEST_HOST
//}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////

//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}

#define CLASS_TYPE KCnParty
ImplPfID( KCnParty, PI_CN_PARTY );

KCnParty::KCnParty( UidType iUID ) :
m_iUID( iUID )
{
	Reset();
}

KCnParty::~KCnParty()
{
}

void KCnParty::Reset()
{
	//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	for( int iIdx = 0; iIdx < TE_MAX; ++iIdx )
	{
		m_tTimer[iIdx].restart();
	}
	m_bRemakingAutoParty = false;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	SetPublic( false );
	SetItemType( 1 );
	SetDungeonDif( 0 );
	SetDungeonID( 0 );
	//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPartyType = KPartyInfo::PT_NONE;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	m_wstrName.clear();
	SetPlaying( false );
	m_eDungeonMode = CXSLDungeon::DM_INVALID;

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	m_bCheckLowLevel = false;
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPvpGameType = CXSLRoom::PGT_NULL;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	m_kPartyUserManager.Reset();
	m_kGameManager.Reset();
	//{{ 2012. 08. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	m_kFeverManager.Reset();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
}

void KCnParty::SetInfo( IN const KPartyInfo& kInfo )
{
	SetName( kInfo.m_wstrPartyName );
	SetDungeonID( kInfo.m_iDungeonID );
	SetDungeonDif( kInfo.m_DifficultyLevel );
	//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPartyType = kInfo.m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	m_kPartyUserManager.SetMaxMember( kInfo.m_MaxSlot );
	SetPublic( kInfo.m_bPublic );
	SetItemType( kInfo.m_cGetItemType );
	SetDungeonMode( (CXSLDungeon::DUNGEON_MODE) kInfo.m_cDungeonMode );
	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	SetCheckLowLevel( kInfo.m_bCheckLowLevel );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_cPvpGameType = kInfo.m_cPVPGameType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	m_kPartyUserManager.SetComeBackUserInParty( kInfo.m_bComeBackBuff );
	m_kPartyUserManager.SetComeBackBuffEnd( kInfo.m_wsrtComeBackBuffEnd );
#endif SERV_COME_BACK_USER_REWARD
	//}} 
}

//{{ 2012. 01. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::SetInfo( IN const KCreatePartyInfo& kInfo )
{
	KPartyInfo kCreatePartyInfo;
	kCreatePartyInfo.m_cPartyType		 = kInfo.m_cPartyType;
	kCreatePartyInfo.m_wstrPartyName	 = L"자동생성파티";
	kCreatePartyInfo.m_MaxSlot			 = 4;
	kCreatePartyInfo.m_iDungeonID		 = kInfo.m_iDungeonID;
	kCreatePartyInfo.m_DifficultyLevel	 = kInfo.m_DifficultyLevel;
	kCreatePartyInfo.m_cGetItemType		 = kInfo.m_cGetItemType;
	kCreatePartyInfo.m_cDungeonMode		 = kInfo.m_cDungeonMode;
	kCreatePartyInfo.m_bCheckLowLevel	 = kInfo.m_bCheckLowLevel;
	kCreatePartyInfo.m_cPVPGameType		 = kInfo.m_cPVPGameType;
	SetInfo( kCreatePartyInfo );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::GetInfo( OUT KPartyInfo& kInfo )
{
	//기본정보
	kInfo.m_bPublic			= IsPublic();
	kInfo.m_cGetItemType	= GetItemType();
	kInfo.m_DifficultyLevel	= GetDungeonDif();
	kInfo.m_iDungeonID		= GetDungeonID();
	//{{ 2011. 06. 28	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kInfo.m_cPartyType		= m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	kInfo.m_iPartyUID		= GetUID();
	kInfo.m_JoinSlot		= m_kPartyUserManager.GetNumMember();
	kInfo.m_MaxSlot			= m_kPartyUserManager.GetMaxMember();
	kInfo.m_wstrPartyName	= GetName();	
	kInfo.m_cDungeonMode	= static_cast<char>( GetDungeonMode() );
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kInfo.m_sPartyFever		= GetFeverPoint();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	kInfo.m_bCheckLowLevel	= GetCheckLowLevel();
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kInfo.m_cPVPGameType = m_cPvpGameType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	CTime tBuffEnd = m_kPartyUserManager.GetComeBackBuffEnd();
	kInfo.m_bComeBackBuff	= m_kPartyUserManager.GetComeBackUserInParty();
	kInfo.m_wsrtComeBackBuffEnd	= tBuffEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#endif SERV_COME_BACK_USER_REWARD
	//}} 
}

void KCnParty::GetInfo( OUT std::vector< KPartyInfo >& vecInfo )
{
	vecInfo.clear();
    KPartyInfo kInfo;
	GetInfo( kInfo );
	vecInfo.push_back( kInfo );
}

//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::GetDungeonGameInfo( OUT KDungeonGameInfo& kInfo ) const
{
	kInfo.m_cDungeonMode = GetDungeonMode();
	kInfo.m_iDungeonID = GetDungeonID();
	kInfo.m_cDifficulty = GetDungeonDif();
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::SetDungeonMode( const CXSLDungeon::DUNGEON_MODE eDungeonMode )
{
	if( CXSLDungeon::IsValidDungeonMode( eDungeonMode ) == false )
	{
		START_LOG( cerr, L"비정상적인 DugneonMode값임." )
			<< BUILD_LOG( eDungeonMode )
			<< END_LOG;

		m_eDungeonMode = CXSLDungeon::DM_INVALID;
		return;
	}

	m_eDungeonMode = eDungeonMode;
}

void KCnParty::Tick()
{
	// 이벤트를 먼저 처리한뒤..
	KPerformer::Tick();

	switch( GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
	case KCnPartyFSM::S_CLOSE:
		break;

	case KCnPartyFSM::S_MAKING_PARTY:
		{
			// 7초동안 첫번째 파티원 초대가 완료되지 않는다면 파티를 닫는다!
			if( m_tTimer[TE_FIRST_INVITE].elapsed() > PARTY_INVITE_TIME_LIMIT )
			{
				CloseParty( NetError::NOT_CLOSE_PARTY_REASON_02 );
			}
		}
		break;

	case KCnPartyFSM::S_WAIT:
		break;

	case KCnPartyFSM::S_CHECK_PLAY_CONDITION:
		{
			if( m_tTimer[TE_CHECK_PLAY_CONDITION].elapsed() > 5.0 )
			{
				START_LOG( cerr, L"파티 던전 게임 시작 조건 체크를 각 파티원들에게 전송하였는데 5초이상 응답이 없었다. 일어나서는 안되는 에러!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
					<< END_LOG;

				// 실패 처리!
				FailProcessPlayCondition( NetError::ERR_PARTY_45 );
			}
		}
		break;

	case KCnPartyFSM::S_CHECK_AUTO_PARTY:
		{
			if( m_tTimer[TE_CHECK_AUTO_PARTY].elapsed() > 5.0 )
			{
				START_LOG( cerr, L"자동 파티 신청을 위한 체크를 보냈는데 5초동안 응답이 없었다. 일어나서는 안되는 에러!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
					<< END_LOG;

				// 실패 처리!
				FailProcessAutoParty( NetError::ERR_UNKNOWN );
			}
		}
		break;

	case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
		{
			if( m_tTimer[TE_ACCEPT_FOR_PLAY].elapsed() > 10.0 )
			{
				// 플레이 수락에 대한 응답 처리가 10초이상 지연되었으므로 파티 게임 시작을 실패 처리 합니다!
				FailProcessAcceptForPlay( NetError::NOT_PARTY_GAME_START_FAILED_01 );
			}
		}
		break;

	case KCnPartyFSM::S_PREPARE_FOR_PLAY:
		{
			if( m_tTimer[TE_PREPARE_FOR_PLAY].elapsed() > 5.0 )
			{
				START_LOG( cerr, L"파티 게임 시작을 위한 정보 수집을 보냈는데 5초동안 응답이 없었다. 일어나서는 안되는 에러!" )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
					<< END_LOG;

				// 게임 시작 정보 수집이 5초 이상 지연되어 게임 시작을 취소 처리 합니다!
				FailProcessPrepareForPlay( NetError::ERR_PARTY_23 );
			}
		}
		break;
	}

	// 신청 대기자 체크
	//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	if( GetStateID() != KCnPartyFSM::S_MAKING_PARTY )
	{
		std::vector< UidType > vecDeleteCadidateUser;
		m_kPartyUserManager.Tick( vecDeleteCadidateUser );
		if( vecDeleteCadidateUser.empty() == false )
		{
			BOOST_TEST_FOREACH( const UidType, iUnitUID, vecDeleteCadidateUser )
			{
				LIF( LeaveCandidateMember( iUnitUID ) );
			}
		}
	}
#else
	m_kPartyUserManager.Tick();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//파티 유저가 1명 이상이고
	//던전게임중이 아닐때 3초마다 유저의 stateid 를 확인하여
	//close 일경우 leave 처리.
}

// 쿨타임 초기화
void KCnParty::AllPartyMembersQuickSlotCoolTimeReset()
{
	// 파티원들에게 퀵슬롯 쿨타임 리셋 패킷 브로드 캐스팅
	BroadCastID( EPM_QUICK_SLOT_COOL_TIME_RESET_NOT );
}

void KCnParty::UpdateAndBroadCastFeverPoint( IN const int iFeverPoint )
{
	// 파티 멤버 수 구하기
	const int iPartyMemberCount = m_kPartyUserManager.GetNumMember();

	// 파티 피버 업데이트!
	int iResultFeverPoint = 0;
	const KCnPartyFeverManager::UPDATE_FEVER_POINT eRet = m_kFeverManager.UpdateFeverPoint( iFeverPoint, iPartyMemberCount, iResultFeverPoint );
	switch( eRet )
	{
	case KCnPartyFeverManager::UFP_ACTIVE_FEVER:
		{
			// fever발동 이벤트 보내기
			KEGS_CHANGE_PARTY_FEVER_STATE_NOT kNot;
			kNot.m_bIsStart = true;
			kNot.m_iPartyFever = iResultFeverPoint;
			BroadCast( EPM_CHANGE_PARTY_FEVER_STATE_NOT, kNot );
		}
		break;

	case KCnPartyFeverManager::UFP_CHANGE_FEVER:
		{
			// 현재 fever수치 보내기
			KEGS_PARTY_FEVER_POINT_NOT kNot;
			kNot.m_iPartyFever = iResultFeverPoint;
			BroadCast( EPM_PARTY_FEVER_POINT_NOT, kNot );
		}
		break;

	default:
		return;
	}

	// 변경된 파티 fever정보를 모든 게임서버의 파티 리스트에 갱신!
	UpdatePartyFeverInPartyList( true );
}

void KCnParty::IncreaseFeverPoint( IN const char cRoomType, IN const short sSubStageNpcCount )
{
	// 파티원수 구하기
	const int iPartyMemberCount = m_kPartyUserManager.GetNumMember();

	// 파티 fever 증가값 구하기
	const int iIncreaseFeverPoint = m_kFeverManager.CalcIncreaseFeverPoint( iPartyMemberCount, cRoomType, sSubStageNpcCount, m_kPartyUserManager.CheckComeBackUserInParty() );
	if( iIncreaseFeverPoint == 0 )
		return;
	
	// 파티 피버 업데이트!
	UpdateAndBroadCastFeverPoint( m_kFeverManager.GetRealFeverPoint() + iIncreaseFeverPoint );
}

void KCnParty::DecreaseFeverPoint( IN const int iDecreaseFeverPoint )
{
	// 피버 포인트 감소!
	UpdateAndBroadCastFeverPoint( m_kFeverManager.GetRealFeverPoint() - iDecreaseFeverPoint );	
}
//}}

void KCnParty::AddPartyInfoInPartyList()
{
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_ADD_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	BroadCastGameServer( EPM_ADD_PARTY_LIST_NOT, kPacket );
}

//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
void KCnParty::DelPartyInfoInPartyList( IN const bool bForce /*= false*/ )
//#else
//void KCnParty::DelPartyInfoInPartyList()
//#endif SERV_PVP_NEW_SYSTEM
//}}
{
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_DEL_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	BroadCastGameServer( EPM_DEL_PARTY_LIST_NOT, kPacket );
}

//{{ 2011. 07. 23	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
void KCnParty::UpdatePartyInfoInPartyList( IN const bool bForce /*= false*/ )
//#else
//void KCnParty::UpdatePartyInfoInPartyList()
//#endif SERV_PVP_NEW_SYSTEM
//}}
{
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_UPDATE_PARTY_IN_PARTY_LIST_NOT kPacket;
	GetInfo( kPacket.m_kPartyInfo );
	BroadCastGameServer( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT, kPacket );
}

//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::UpdatePartyFeverInPartyList( IN const bool bForce /*= false*/ )
{
	//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
//	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
//		return;
//#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	kPacket.m_sPartyFever = GetFeverPoint();
	BroadCastGameServer( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT, kPacket );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::AddPartyMemberInPartyList( IN const KPartyUserInfo& kInfo )
{
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	kPacket.m_kPartyUserInfo = kInfo;
	BroadCastGameServer( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT, kPacket );
}

//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
void KCnParty::DelPartyMemberInPartyList( IN const UidType iUnitUID, IN const bool bForce /*= false*/ )
//#else
//void KCnParty::DelPartyMemberInPartyList( IN UidType iUnitUID )
//#endif SERV_PVP_NEW_SYSTEM
//}}
{
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( GetPartyType() != KPartyInfo::PT_DUNGEON_PARTY  &&  bForce == false )
		return;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KEPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	kPacket.m_iUnitUID = iUnitUID;
	BroadCastGameServer( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT, kPacket );
}

//{{ 2012. 03. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KCnParty::UpdateJoinRoomInfoAndGetOnOffResult( IN const UidType iUnitUID, IN const KPartyUserJoinRoomInfo& kPartyUserJoinRoomInfo )
{
	std::vector< UidType > vecOnMoveButton;
	std::vector< UidType > vecOffMoveButton;

	// 방 입장 정보를 업데이트
	if( m_kPartyUserManager.UpdateJoinRoomInfoAndGetOnOffResult( iUnitUID, kPartyUserJoinRoomInfo, vecOnMoveButton, vecOffMoveButton ) == false )
	{
		START_LOG( cerr, L"파티유저 방 입장 정보를 업데이트 실패하였습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return;
	}

	// 필드에서 필드로, 필드에서 다른곳으로 다른곳에서 필드로 이동한거라면 새로운 갱신을 처리하자!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecOnMoveButton )
	{
		KCnPartyUserPtr spPartyUser = m_kPartyUserManager.GetPartyUser( iUnitUID );
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"파티유저가 존재하지 않습니다!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT kNot;
		kNot.m_bIsOn = true;
		spPartyUser->SendToGSCharacter( EPM_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT, kNot );
	}

	BOOST_TEST_FOREACH( const UidType, iUnitUID, vecOffMoveButton )
	{
		KCnPartyUserPtr spPartyUser = m_kPartyUserManager.GetPartyUser( iUnitUID );
		if( spPartyUser == NULL )
		{
			START_LOG( cerr, L"파티유저가 존재하지 않습니다!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			continue;
		}

		KEGS_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT kNot;
		kNot.m_bIsOn = false;
		spPartyUser->SendToGSCharacter( EPM_UPDATE_MOVE_TO_PARTY_MEMBER_INFO_NOT, kNot );
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
KOldPartyInfo KCnParty::MakeOldPartyInfo() const
{
	KOldPartyInfo kInfo;
	kInfo.m_iOldPartyUID = GetUID();
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	kInfo.m_iOldHostUnitUID = m_kPartyUserManager.GetHostUID();
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	kInfo.m_sOldPartyFever = GetFeverPoint();
	m_kPartyUserManager.GetUnitUIDList( kInfo.m_setOldPartyMember );
	return kInfo;
}

void KCnParty::CheckAllMembersPlayCondition()
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_PLAY_CONDITION )
	{
		START_LOG( cerr, L"잘못된 상태에서 함수가 호출되었습니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// 모든 유저의 체크가 완료되면 다음 단계로 진행!
	if( m_kPartyUserManager.IsAllPartyMembersCheckedPlayCondition() == true )
	{
		// 파티장 찾기!
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( IS_NULL( spHost ) )
		{
			START_LOG( cerr, L"파티장이 존재하지 않습니다! 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
				<< END_LOG;

			// 일어나서는 안되는 에러지만 만약에 일어난다면 어떻게 하면 좋을까..?
			return;
		}

		// 모든 파티원들 ready체크 초기화
		m_kPartyUserManager.ChangeAllReady( false );

		// 모든 파티원들의 PartyRoomUserInfo비우기
		m_kPartyUserManager.ClearDungeonRoomUserInfoList();

#ifdef CORRECT_CHECKING_TICKET_TO_DUNGEON	// 적용날짜: 2013-03-05
		// 기존에 체크 완료 플래그 초기화
		m_kPartyUserManager.ClearCheckedPlayCondition();
#endif // CORRECT_CHECKING_TICKET_TO_DUNGEON
		
		// 수락 타이머 초기화
		m_tTimer[TE_ACCEPT_FOR_PLAY].restart();

		// 게임시작을 위한 모든 파티원들의 수락 체크 상태로 이동!
		StateTransition( KCnPartyFSM::I_TO_ACCEPT_FOR_PLAY );

		// 요청에 대한 응답
		KEPM_PARTY_GAME_START_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_usEventID = m_kGameManager.GetGameStartEventID();
		spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kPacket );

		// 모든 파티원들에게 게임 시작에 대한 의견 묻기! : 파티장 제외
		//{{ 2012. 12. 17	박세훈	수동 파티 시 던전 팝업 창 문구 오류 수정
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
		{
			KEGS_PARTY_GAME_START_OPINION_CHECK_NOT kPacket;
			kPacket.m_iDungeonID		= GetDungeonID();
			kPacket.m_cDifficultyLevel	= GetDungeonDif();
			kPacket.m_cDungeonMode		= GetDungeonMode();
			BroadCast( EPM_PARTY_GAME_START_OPINION_CHECK_NOT, kPacket );
		}
#else
		BroadCastID( EPM_PARTY_GAME_START_OPINION_CHECK_NOT );
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
		//}}
	}
}

void KCnParty::FailProcessPlayCondition( IN const int iNetError, IN const UidType iFailUnitUID /*= 0*/, IN const std::wstring wstrFailUserNickName /*= L""*/ )
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_PLAY_CONDITION )
	{
		START_LOG( cerr, L"잘못된 상태에서 함수가 호출되었습니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// 다시 S_WAIT상태로 이동!
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// 조건 체크 초기화
	m_kPartyUserManager.ClearCheckedPlayCondition();

	// 파티장을 찾자!
	KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
	if( spHost == NULL )
	{
		START_LOG( cerr, L"파티장이 없다! 절대로 있을수 없는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		return;
	}

	KEPM_PARTY_GAME_START_ACK kPacket;
	kPacket.m_iOK = iNetError;
	kPacket.m_iFailUnitUID = iFailUnitUID;
	kPacket.m_wstrFailUserNickName = wstrFailUserNickName;
	kPacket.m_iDungeonMode = GetDungeonMode();
	kPacket.m_iDungeonID = GetDungeonIDAndDif();
	spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kPacket );
}

void KCnParty::CheckAllMembersAutoParty()
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_AUTO_PARTY )
	{
		START_LOG( cerr, L"잘못된 상태에서 함수가 호출되었습니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}
    
	// 모든 파티원이 자동 파티 체크가 완료되었는지 확인!
	if( m_kPartyUserManager.IsAllPartyMembersCheckedForAutoParty() == true )
	{
		// 상태 이동!
		StateTransition( KCnPartyFSM::I_TO_WAIT );

		// 혹시 모르니깐 준비값 초기화하자!
		m_kPartyUserManager.ClearCheckedForAutoParty();

		// 파티장 찾아서!
		KCnPartyUserPtr spPartyHost = m_kPartyUserManager.GetHostPtr();
		if( spPartyHost == NULL )
		{
			START_LOG( cerr, L"파티장이 없네? 있을수 없는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;
			return;
		}

		// 정보 얻기
		KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_bRemakingAutoParty = m_bRemakingAutoParty; // 재등록 여부!
		kPacket.m_kAutoPartyCondition.m_iDungeonID = GetDungeonID();
		kPacket.m_kAutoPartyCondition.m_cDifficultyLevel = GetDungeonDif();
		kPacket.m_kAutoPartyCondition.m_cGetItemType = GetItemType();
		kPacket.m_kAutoPartyCondition.m_cDungeonMode = static_cast<char>(GetDungeonMode());
		m_kPartyUserManager.GetAutoPartyUserInfoList( MakeOldPartyInfo(), kPacket.m_vecAutoPartyUser );
		spPartyHost->SendToGSCharacter( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
	}
}

void KCnParty::FailProcessAutoParty( IN const int iNetError, IN const std::wstring wstrFailUserNickName /*= L""*/ )
{
	if( GetStateID() != KCnPartyFSM::S_CHECK_AUTO_PARTY )
	{
		START_LOG( cerr, L"잘못된 상태에서 함수가 호출되었습니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// 상태 이동!
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// 혹시 모르니깐 준비값 초기화하자!
	m_kPartyUserManager.ClearCheckedForAutoParty();

	// 파티장 찾아서
	KCnPartyUserPtr spPartyHost = m_kPartyUserManager.GetHostPtr();
	if( spPartyHost == NULL )
	{
		START_LOG( cerr, L"파티장이 없네? 있을수 없는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		return;
	}

	// 자동 파티 레디 체크 상태를 해제 하자!
	KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK kAck;
	kAck.m_iOK = iNetError;
	kAck.m_wstrFailUserNickName = wstrFailUserNickName;
	kAck.m_kAutoPartyCondition.m_iDungeonID = GetDungeonID();
	kAck.m_kAutoPartyCondition.m_cDifficultyLevel = GetDungeonDif();
	kAck.m_kAutoPartyCondition.m_cDungeonMode = static_cast<char>(GetDungeonMode());
	spPartyHost->SendToGSCharacter( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kAck );
}

void KCnParty::CheckAllMembersAcceptForPlay()
{
	if( GetStateID() != KCnPartyFSM::S_ACCEPT_FOR_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 함수가 호출되었습니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// 모든 파티원이 준비 되었는지 확인합니다!
	if( m_kPartyUserManager.IsAllReady() == true )
	{
		// 플레이 정보 수집에 대한 타이머 초기화
		m_tTimer[TE_PREPARE_FOR_PLAY].restart();

		// 파티 게임 시작 동의가 완료되었습니다. 파티원 정보 수집을 위해 다시 상태이동!
		StateTransition( KCnPartyFSM::I_TO_PREPARE_FOR_PLAY );

		// 모든 파티원들의 PartyRoomUserInfo비우기 - 아까 비웠지만 혹시나 해서 한번 더 비웁니다.
		m_kPartyUserManager.ClearDungeonRoomUserInfoList();

		//////////////////////////////////////////////////////////////////////////
		// 던전방 만들 준비를 시작.
		KEPM_CHECK_FOR_PARTY_GAME_START_REQ kPacketReq;
		kPacketReq.m_iPartyUID = GetUID();
		kPacketReq.m_usEventID = m_kGameManager.GetGameStartEventID();
		kPacketReq.m_sWorldID  = m_kGameManager.GetGameStartWorldID();
		GetDungeonGameInfo( kPacketReq.m_kDungeonGameInfo );
		BroadCast( EPM_CHECK_FOR_PARTY_GAME_START_REQ, kPacketReq );
	}	
}

void KCnParty::FailProcessAcceptForPlay( IN const int iNetError, IN const UidType iFailUnitUID /*= 0*/ )
{
	if( GetStateID() != KCnPartyFSM::S_ACCEPT_FOR_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 함수가 호출되었습니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// 파티 상태를 Wait로 이동합니다.
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// 모든 파티원에게 파티게임시작 실패 메시지를 날립니다.
	KEGS_PARTY_GAME_START_FAILED_NOT kPacketNot;
	kPacketNot.m_iReason = iNetError;
	kPacketNot.m_iFailUnitUID = iFailUnitUID;
	BroadCast( EPM_PARTY_GAME_START_FAILED_NOT, kPacketNot );
}

void KCnParty::FailProcessPrepareForPlay( IN const int iNetError )
{
	if( GetStateID() != KCnPartyFSM::S_PREPARE_FOR_PLAY )
	{
		START_LOG( cerr, L"잘못된 상태에서 함수가 호출되었습니다!" )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	// 파티 상태를 Wait로 이동합니다.
	StateTransition( KCnPartyFSM::I_TO_WAIT );
    
	// 모든 파티원에게 파티게임시작 실패 메시지를 날립니다.
	KEGS_PARTY_GAME_START_FAILED_NOT kPacketNot;
	kPacketNot.m_iReason = iNetError;
	BroadCast( EPM_PARTY_GAME_START_FAILED_NOT, kPacketNot );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KCnParty::LeaveCandidateMember( IN const UidType iUnitUID )
{
	KCnPartyUserPtr spCandidateUser = m_kPartyUserManager.GetCandidateUser( iUnitUID );
	if( IS_NULL( spCandidateUser ) )
	{
		START_LOG( cerr, L"파티 가입 예약된 유저가 아닙니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	// 파티 멤버 임시 등록 해제를 알리자.
	spCandidateUser->SendToGSCharacter( EPM_LEAVE_CANDIDATE_PARTY_NOT, char() );

	// 임시 멤버 이탈 처리
	LIF( m_kPartyUserManager.LeaveCandidate( iUnitUID ) );
	return true;
}

bool KCnParty::LeaveMember( IN const UidType iUnitUID 
						#ifdef LOG_PARTY_BREAK
							, IN const int iReason	
						#endif // LOG_PARTY_BREAK
							)
{
	SET_ERROR( NET_OK );

	// 이탈하는 유저는 방 입장 정보를 초기화 시켜주자!
	UpdateJoinRoomInfoAndGetOnOffResult( iUnitUID, KPartyUserJoinRoomInfo() );

	// 이탈자의 GSUID값을 얻어오자.
	UidType iLeaveUnitGSUID = 0;
	if( m_kPartyUserManager.GetGSUID( iUnitUID, iLeaveUnitGSUID ) == false )
	{
		//삭제실패.
		SET_ERROR( ERR_PARTY_01 );
		return false;
	}

	//파티에서 나갈유저를 빼주자.
	if( m_kPartyUserManager.LeaveMember( iUnitUID ) == false )
	{
		//삭제실패.
		SET_ERROR( ERR_PARTY_01 );
		return false;
	}
	
	// 파티를 이탈하는 당사자에게도 성공적인 이탈을 알리자!
	{	
		KEGS_LEAVE_PARTY_NOT kNot;
		kNot.m_iLeaveUnitUID	= iUnitUID;
		kNot.m_iPartyUID		= GetUID();
		kNot.m_bExistComeBackUser = false;
#ifdef LOG_PARTY_BREAK
		kNot.m_iReason			= iReason;
#endif // LOG_PARTY_BREAK

		SendToGSCharacter( iLeaveUnitGSUID, iUnitUID, EPM_LEAVE_PARTY_NOT, kNot );
	}

	// 파티에 아무도 없나.?
	if( m_kPartyUserManager.GetNumMember() > PE_CLOSE_PARTY_NUM_MEMBER )
	{	
		// 파티원이 이탈할 경우 Party Fever값 감소
		DecreaseFeverPoint( KCnPartyFeverManager::FEVER_DECREASE_POINT_BY_LEAVE_PARTY );

		// 변경된 파티 정보 파티리스트에 갱신
		UpdatePartyInfoInPartyList();

		// 어떤 파티원이 이탈했는지 다른 파티원들에게 알리자!
		KEGS_LEAVE_PARTY_NOT kNot;
		kNot.m_iLeaveUnitUID	= iUnitUID;
		kNot.m_iNewHostUID		= m_kPartyUserManager.GetHostUID();
		m_kPartyUserManager.GetUnitUIDList( kNot.m_vecPartyUnitUIDList );
		kNot.m_iPartyUID		= GetUID();
		kNot.m_bExistComeBackUser = m_kPartyUserManager.CheckComeBackUserInParty();
#ifdef LOG_PARTY_BREAK
		kNot.m_iReason			= iReason;
#endif // LOG_PARTY_BREAK
		BroadCast( EPM_LEAVE_PARTY_NOT, kNot );

		//파티정보 업데이트.
		m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

		// 새로운 파티장이 정해졌을테니 주변섹터 유저들에게 파티 말풍선을 알려주자!
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost != NULL )
		{
			const bool bLeaveHost = ( iUnitUID == m_kPartyUserManager.GetHostUID() );
			if( bLeaveHost )
			{
				// 모든 게임서버의 파티 리스트에 변경된 정보 갱신
				KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT kNot;
				kNot.m_iPartyUID = GetUID();
				kNot.m_vecUpdatePartyUser.push_back( spHost->GetPartyUserInfo() );	// 새롭게 임명된 방장 정보
				kNot.m_vecDeletePartyUser.push_back( iUnitUID );					// 이탈하는 유저
				BroadCastGameServer( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, kNot );
			}
			else
			{
				// 파티 리스트에서 파티 인원을 삭제!
				DelPartyMemberInPartyList( iUnitUID );
			}
		}
	}
	//{{ 2012. 12. 07	최육사	배틀필드 시스템
#ifdef SERV_CODE_CLEANUP_2012_12_07
	// [2012-12-07][최육사] 파티원이 아무도 없는경우에는 해당 코드를 안타도록 수정.
	else if( m_kPartyUserManager.GetNumMember() == PE_CLOSE_PARTY_NUM_MEMBER )
#else
	else
#endif SERV_CODE_CLEANUP_2012_12_07
	//}}
	{
		// 버프를 지워주기 위해 혼자 남아 있는 사람에게도 전달 해야 한다.
		KEGS_LEAVE_PARTY_NOT kNot;
		kNot.m_iLeaveUnitUID	= iUnitUID;
		kNot.m_iNewHostUID		= m_kPartyUserManager.GetHostUID();
		m_kPartyUserManager.GetUnitUIDList( kNot.m_vecPartyUnitUIDList );
		kNot.m_iPartyUID		= GetUID();
		kNot.m_bExistComeBackUser = m_kPartyUserManager.CheckComeBackUserInParty();
#ifdef LOG_PARTY_BREAK
		kNot.m_iReason			= iReason;
#endif // LOG_PARTY_BREAK
		BroadCast( EPM_LEAVE_PARTY_NOT, kNot );
	}
		
	return true;
}

void KCnParty::CloseParty( IN const int iReason )
{
	// 예비 등록자가 있는지 보고 예비등록자에게도 파티 닫힘을 알리자.
	std::vector< UidType > vecDeleteCadidateUser;
	m_kPartyUserManager.GetCadidateUserList( vecDeleteCadidateUser );

	if( vecDeleteCadidateUser.empty() == false )
	{
		BOOST_TEST_FOREACH( const UidType, iUnitUID, vecDeleteCadidateUser )
		{
			// 7초 이후에 대기자의 상태가 변화가 없었는지 확인, 없었다면 파티 초대를 거부한것이므로 초대 거부 메시지를 초대자에게 날리자.
			KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
			if( IS_NULL( spHost ) )
			{
				START_LOG( cerr, L"파티장 포인터가 잘못되었음.!" )
					<< END_LOG;
			}
			else
			{
				// 초대 받은 유저가 의사 결정을 하지 않고 10초가 지남.
				//## 파티장에게 초대가 거절되었다고 알려줌.
				KEGS_INVITE_PARTY_DENY_NOT kDenyNot;
				kDenyNot.m_iDenyReasonNetError = NetError::ERR_PARTY_19;
				kDenyNot.m_iUnitUID = iUnitUID;
				spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
			}

			LIF( LeaveCandidateMember( iUnitUID ) );
		}
	}

	// 파티가 깨진다는것을 모든 파티원에게 알리자!
	KEGS_CLOSE_PARTY_NOT kPacketNot;
	kPacketNot.m_iPartyUID = GetUID();
	kPacketNot.m_iReason = iReason;
	BroadCast( EPM_CLOSE_PARTY_NOT, kPacketNot );

	// 파티 닫기 처리
    SiKCnPartyManager()->CloseParty( GetThisPtr<KCnParty>() );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

void KCnParty::BroadCastID( IN u_short usEventID, IN const UidType iUnitUID_ /*= 0*/ )
{
	BroadCast( usEventID, char(), iUnitUID_ );
}

void KCnParty::ProcessEvent( const KEventPtr& spEvent_ )
{
	switch( spEvent_->m_usEventID )
	{
		CASE( EPM_CREATE_PARTY_BY_INVITE_REQ );
		CASE( EPM_INVITE_PARTY_REQ );
		CASE( EPM_INVITE_PARTY_REPLY_NOT );
		CASE( EPM_UPDATE_PARTY_USER_INFO_NOT );
		CASE( EPM_LEAVE_PARTY_REQ );
		CASE( EPM_PARTY_CHANGE_READY_REQ );
		CASE( EPM_PARTY_CHANGE_DUNGEON_REQ );
		CASE( EPM_PARTY_GAME_START_REQ );
		//{{ 2012. 01. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		CASE( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK );
	   _CASE( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		CASE( EPM_CHECK_FOR_PARTY_GAME_START_ACK );
		CASE( EPM_END_GAME_NOT );
		CASE( EPM_PARTY_CHANGE_HOST_REQ );
		CASE( EPM_CHECK_PARTY_CHANGE_HOST_ACK );
		CASE( EPM_PARTY_CHANGE_NUM_OF_PER_REQ );
		CASE( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ );
		CASE( EPM_PARTY_BAN_USER_REQ );
		CASE( EPM_CHAT_REQ );
		CASE( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
	   _CASE( EPM_INCREASE_PARTY_FEVER_NOT, KERM_INCREASE_PARTY_FEVER_NOT );
	   //{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	   _CASE( EPM_MAX_PARTY_FEVER_NOT, KERM_MAX_PARTY_FEVER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	   //}}
		CASE( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT );
	   _CASE( EPM_CHECK_INVALID_USER_NOT, UidType );
		//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		CASE( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ );
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		//}}
		//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	   _CASE( EPM_CHANGE_PARTY_TYPE_REQ, KEGS_CHANGE_PARTY_TYPE_REQ );
		CASE( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ );
	   _CASE( EPM_PVP_PARTY_CHANGE_MATCH_INFO_REQ, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ );
#endif SERV_PVP_NEW_SYSTEM
		//}}
	   //{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	   CASE( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ );
	   CASE( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ );
	   CASE( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK );
	   CASE( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ );
	   CASE( EPM_REQUEST_REGROUP_PARTY_NOT );
	   CASE_NOPARAM( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT );
	   CASE( EPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT );
	   CASE_NOPARAM( EPM_END_GAME_REGROUP_PARTY_NOT );

	   _CASE( EPM_UPDATE_GAME_PLAY_STATUS_NOT, KPartyMemberStatus );
#endif SERV_BATTLE_FIELD_SYSTEM
	   //}}
	   //{{ 2012. 05. 30	최육사		공식 대전 버그 수정
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
	   CASE( EPM_UPDATE_PVP_INFO_NOT );
#endif SERV_OFFCIAL_PVP_BUG_FIX
	   //}}

	default:
		START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
	}
}

IMPL_ON_FUNC( EPM_CREATE_PARTY_BY_INVITE_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_INIT ), EPM_CREATE_PARTY_BY_INVITE_ACK, KEPM_CREATE_PARTY_BY_INVITE_ACK );

	KEPM_OPEN_PARTY_LIST_NOT kPacketNot;
	kPacket.m_iOK = NetError::NET_OK;	

	// 새로운 파티 생성 및 파티장 입장
	if( m_kPartyUserManager.Create( kPacket_.m_kHostPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"생성한 파티에 유저 입장실패.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_00;
		goto end_proc;
	}

	// 유저설정을 파티에 셋팅해주고
	SetInfo( kPacket_.m_kCreatePartyInfo );

	// 개발자 로그
	if( kPacket_.m_cAuthLevel >= SEnum::UAL_DEVELOPER )
	{
		START_LOG( cout, L"개발자 파티 생성 성공!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName );
	}

	// 파티리스트에 생성된 파티 추가
	GetInfo( kPacketNot.m_kPartyInfo );
	m_kPartyUserManager.GetUserInfoList( kPacketNot.m_vecUserInfo );
	BroadCastGameServer( EPM_OPEN_PARTY_LIST_NOT, kPacketNot );


	//@@ 방리스트 추가
	//## 1. 공개일경우
	//## 2. 최대인원이 2명 이상일 경우.
	//## 3. 게임중인 상태가 아닌경우.
	//{{ 2011. 07. 04	최육사	대전 개편
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		AddPartyInfoInPartyList();
	}

	//필드정보 업데이트.
	m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

	//대기자 명단에 넣고..
	if( m_kPartyUserManager.JoinCandidate( kPacket_.m_kReceiverPartyUserInfo, true, kPacket_.m_kHostPartyUserInfo.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"파티 대기자 리스트 등록 실패.!" )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kHostPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_07;
		goto end_proc;
	}

	// 파티 생성을 위한 기본 작업이 끝났으니 상태 이동을 하자!
	StateTransition( KCnPartyFSM::I_TO_MAKING_PARTY );

	//파티장에게 초대중이라고 알리고.
	kPacket.m_iOK = NetError::NET_OK;
	GetInfo( kPacket.m_kPartyInfo );
	kPacket.m_kHostPartyUserInfo = kPacket_.m_kHostPartyUserInfo;
	kPacket.m_kReceiverPartyUserInfo = kPacket_.m_kReceiverPartyUserInfo;

	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	kPacket.m_kHostPartyUserInfo.m_wstrIP = NetCommon::GetLocalIPW();
	kPacket.m_kHostPartyUserInfo.m_usPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CREATE_PARTY_BY_INVITE_ACK, kPacket );

	{
		//초대유저에게 not를 알려준다.
		KEGS_INVITE_PARTY_NOT kNot;
		kNot.m_iPartyUID = GetUID();
		kNot.m_iHostUnitUID = kPacket_.m_kHostPartyUserInfo.m_iUnitUID;
		kNot.m_wstrHostNickName = kPacket_.m_kHostPartyUserInfo.m_wstrNickName;
		//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		kNot.m_cPartyType = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		kNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
		kNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
		//}

		SendToGSCharacter( kPacket_.m_kReceiverPartyUserInfo.m_iGSUID, kPacket_.m_kReceiverPartyUserInfo.m_iUnitUID, EPM_INVITE_PARTY_NOT, kNot );
	}

	//////////////////////////////////////////////////////////////////////////	
	//////////////////////////////////////////////////////////////////////////

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CREATE_PARTY_BY_INVITE_ACK, kPacket );

	// 파티 생성 실패! 파티를 닫습니다.
	CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
}

IMPL_ON_FUNC( EPM_INVITE_PARTY_REQ )
{
	// 아직 파티 생성 초대가 완료되지 않은 상태이므로 추가 초대는 불가능합니다.
	if( GetStateID() == KCnPartyFSM::S_MAKING_PARTY )
	{
        KEGS_INVITE_PARTY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	// WAIT상태에서만 추가초대가 가능합니다.
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_INVITE_PARTY_ACK, KEGS_INVITE_PARTY_ACK );

	// 초대받은 유저의 UnitUID저장
	kPacket.m_iReceiverUID = kPacket_.m_kPartyUserInfo.m_iUnitUID;

	// 파티장만 초대 가능하도록 수정
	if( m_kPartyUserManager.GetHostUID() != FIRST_SENDER_UID )
	{
		START_LOG( clog, L"파티장만 초대 가능합니다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	// 파티 소속인지 체크
	KCnPartyUserPtr spHost = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spHost == NULL )
	{
		START_LOG( cerr, L"파티장 유저 정보가 없음." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//이미 파티소속이거나 대기자 거나
	if( m_kPartyUserManager.IsMemberOrCandidate( kPacket_.m_kPartyUserInfo.m_iUnitUID ) == true )
	{
		START_LOG( cwarn, L"이미 해당파티에 소속되어 있음." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_wstrNickName )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_12;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//게임중인지..
	if( IsPlaying() == true )
	{
		START_LOG( cerr, L"파티가 이미 게임중." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( m_kPartyUserManager.GetNumTotal() )
			<< END_LOG;

		//파티장이 요청하는 것이므로 게임중이면 초대 할 수 없는 상태이다
		kPacket.m_iOK = NetError::ERR_PARTY_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//들어갈 자리가 잇는지..
	if( m_kPartyUserManager.GetNumTotal() >= m_kPartyUserManager.GetMaxMember() )
	{
		START_LOG( clog, L"파티에 들어갈 자리가 없음.!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( m_kPartyUserManager.GetNumTotal() )
			<< BUILD_LOG( m_kPartyUserManager.GetMaxMember() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_15;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( GetDungeonMode() == CXSLDungeon::DM_HENIR_CHALLENGE )
	{
		// 입장하려는 유저와 파티멤버들이 모두 동일한 서버군인지 검사
		if( m_kPartyUserManager.IsAllPartyMembersSameServerGroup( kPacket_.m_kPartyUserInfo.m_iUnitUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_05;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	//대기자 명단에 넣고..
	if( m_kPartyUserManager.JoinCandidate( kPacket_.m_kPartyUserInfo, true, FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"파티 대기자 리스트 등록 실패.!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( spHost->GetCharUID() )
#else
			<< BUILD_LOG( spHost->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_07;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );
		return;
	}

	//파티장에게 초대중이라고 알리고.
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_INVITE_PARTY_ACK, kPacket );

	//초대유저에게 not를 알려준다.
	KEGS_INVITE_PARTY_NOT kNot;
	kNot.m_iPartyUID = GetUID();
	kNot.m_iHostUnitUID = spHost->GetCharUID();
	kNot.m_wstrHostNickName = spHost->GetCharName();
	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kNot.m_cPartyType = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	kNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
	kNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
	//}

	SendToGSCharacter( kPacket_.m_kPartyUserInfo.m_iGSUID, kPacket_.m_kPartyUserInfo.m_iUnitUID, EPM_INVITE_PARTY_NOT, kNot );
}

IMPL_ON_FUNC( EPM_INVITE_PARTY_REPLY_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 2, KCnPartyFSM::S_MAKING_PARTY, KCnPartyFSM::S_WAIT ) );

	//대기자에 있는지 검사한다.
	if( m_kPartyUserManager.IsCandidate( kPacket_.m_kPartyUserInfo.m_iUnitUID ) == false )
	{
		if( kPacket_.m_cReplyReason == KEGS_INVITE_PARTY_REPLY_NOT::RR_ACCEPT )
		{
			KEGS_JOIN_PARTY_NOT kJoinNot;
			kJoinNot.m_iOK = NetError::ERR_PARTY_18;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );
		}

		//늦게 답장이 왔다! 거부한것으로 처리!
		goto end_proc;
	}

	//초대원이 파티 초대 거부를 했을경우.
	if( kPacket_.m_cReplyReason != KEGS_INVITE_PARTY_REPLY_NOT::RR_ACCEPT )
	{
		// 임시 멤버 이탈 처리!
		LIF( LeaveCandidateMember( FIRST_SENDER_UID ) );

		// 파티장에게는 초대가 거부되었음을 알리자.
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost != NULL )
		{
			KEGS_INVITE_PARTY_DENY_NOT kDenyNot;			
			kDenyNot.m_iDenyReasonNetError = ( kPacket_.m_cReplyReason == KEGS_INVITE_PARTY_REPLY_NOT::RR_BUSY ? NetError::ERR_PARTY_42 : NetError::ERR_PARTY_19 );
			kDenyNot.m_iUnitUID = kPacket_.m_kPartyUserInfo.m_iUnitUID;
			spHost->SendToGSCharacter( EPM_INVITE_PARTY_DENY_NOT, kDenyNot );
		}
		else
		{
			//파티장에게 거절 여부를 알려주는것인데 없으므로 패킷은 날리지 않는다.
			START_LOG( cwarn, L"초대거절을 알려줄 파티장이 없음.!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_iUnitUID )
				<< END_LOG;
		}

		goto end_proc;
	}
	
	//정회원으로 등록을 시킨다.
	if( m_kPartyUserManager.JoinMember( kPacket_.m_kPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"파티 유저등록 실패.!" )
			<< BUILD_LOG( kPacket_.m_kPartyUserInfo.m_wstrNickName )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		//대기자에게 실패 패킷을 날린다.
		KEGS_JOIN_PARTY_NOT kJoinNot;
		kJoinNot.m_iOK = NetError::ERR_PARTY_18;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );

		//역시 실패한 경우
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	// 처음 가입 입니다!
	if( GetStateID() == KCnPartyFSM::S_MAKING_PARTY )
	{
		KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost == NULL )
		{
			START_LOG( cerr, L"파티장이 존재하지 않는다? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
		}
		else
		{
			// 파티장에게도 날리고!
			KEGS_JOIN_PARTY_NOT kJoinNot;
			GetInfo( kJoinNot.m_vecPartyInfo );
			m_kPartyUserManager.GetUserInfoList( kJoinNot.m_vecPartyUserInfo );
			m_kPartyUserManager.GetUnitUIDList( kJoinNot.m_vecCurPartyUnitUIDList );
			kJoinNot.m_iOK = NetError::NET_OK;
			//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
			kJoinNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
			kJoinNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
			//}

			spHost->SendToGSCharacter( EPM_JOIN_PARTY_NOT, kJoinNot );

			// 파티원에게도 똑같은걸 날립시다!
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );
		}

		// 파티 생성 성공했으니 Wait상태로 이동하자!
		StateTransition( KCnPartyFSM::I_TO_WAIT );
	}
	else
	{
		{
			//새로 파티에 입장하는 유저에게 정보를 보내주고.
			KEGS_JOIN_PARTY_NOT kJoinNot;
			//## 1. 기존 파티인원들의 정보
			m_kPartyUserManager.GetUserInfoList( kJoinNot.m_vecPartyUserInfo );
			m_kPartyUserManager.GetUnitUIDList( kJoinNot.m_vecCurPartyUnitUIDList );
			//## 2. 파티정보
			GetInfo( kJoinNot.m_vecPartyInfo );
			//## 3. 알려주기.
			kJoinNot.m_iOK = NetError::NET_OK;
			//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
			kJoinNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
			kJoinNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
			//}

			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_JOIN_PARTY_NOT, kJoinNot );
		}
		{
			//기존 파티원들에게 새로들어온 파티원 정보를 날려준다.
			//## 1. 새로들어온 유저의 정보를 담아
			KEGS_JOIN_PARTY_NOT kJoinNot;
			GetInfo( kJoinNot.m_vecPartyInfo );
			KPartyUserInfo kPartyUserInfo;
			m_kPartyUserManager.GetUserInfo( kPacket_.m_kPartyUserInfo.m_iUnitUID, kPartyUserInfo );
			m_kPartyUserManager.GetUnitUIDList( kJoinNot.m_vecCurPartyUnitUIDList );
			kJoinNot.m_vecPartyUserInfo.push_back( kPartyUserInfo );
			//## 2. 기존 유저들에게 알려줌
			kJoinNot.m_iOK = NetError::NET_OK;
			//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
			kJoinNot.m_wstrUDPRelayIP = NetCommon::GetLocalIPW();
			kJoinNot.m_usUDPRelayPort = KNetLayer::GetKObj()->GetNCUDPPort();
#endif SERV_CHOOSE_FASTEST_HOST
			//}

			BroadCast( EPM_JOIN_PARTY_NOT, kJoinNot, kPacket_.m_kPartyUserInfo.m_iUnitUID );
		}
	}

	{
		// 파티 리스트의 파티 정보 갱신
		UpdatePartyInfoInPartyList();

		// 파티 리스트에 가입한 파티원 추가
		KPartyUserInfo kPartyUserInfo;
		m_kPartyUserManager.GetUserInfo( kPacket_.m_kPartyUserInfo.m_iUnitUID, kPartyUserInfo );
		AddPartyMemberInPartyList( kPartyUserInfo );

		//필드정보 업데이트.
		m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );
	}
	
	return;

end_proc:
	// 첫번째 파티 초대라면 현재 파티를 깨버린다.
	if( GetStateID() == KCnPartyFSM::S_MAKING_PARTY )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
	}
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_USER_INFO_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	// 파티 유저 정보 갱신
	KCnPartyUserPtr spUser = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spUser == NULL )
	{
		START_LOG( cerr, L"파티 유저 정보를 갱신할 유저가 없습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 업데이트
	spUser->SetUnitClass( kPacket_.m_cUnitClass );
	spUser->SetLevel( kPacket_.m_ucLevel );
	spUser->SetState( kPacket_.m_cState );
	spUser->SetStateCode( kPacket_.m_iStateCode );


	// 파티 멤버들에게 변경된 정보 전송
	{
		KEGS_UPDATE_PARTY_USER_INFO_NOT kNot;
		kNot.m_iUnitUID		= kPacket_.m_iUnitUID;
		kNot.m_cUnitClass	= kPacket_.m_cUnitClass;
		kNot.m_ucLevel		= kPacket_.m_ucLevel;
		kNot.m_cState		= kPacket_.m_cState;
		kNot.m_iStateCode	= kPacket_.m_iStateCode;

		BroadCast( EPM_UPDATE_PARTY_USER_INFO_BROAD_NOT, kNot );
	}

	// 모든 게임서버의 파티 리스트에 변경된 정보 갱신
	{
		KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT kNot;
		kNot.m_iPartyUID = GetUID();
		kNot.m_vecUpdatePartyUser.push_back( spUser->GetPartyUserInfo() );
		BroadCastGameServer( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, kNot );
	}

	if( kPacket_.m_bChangeField == true  &&  kPacket_.m_iUnitUID != m_kPartyUserManager.GetHostUID() )
	{
		if( m_kPartyUserManager.ChangeReady( kPacket_.m_iUnitUID, false ) == false )
		{
			START_LOG( cerr, L"필드 이동시 레뒤 변경 실패." )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( m_kPartyUserManager.GetNumTotal() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;
		}

		// 위에 실패를 할경우라도 강제로 레디를 풀어줌.
		KEGS_PARTY_CHANGE_READY_NOT kNot;
		kNot.m_UnitUID	= kPacket_.m_iUnitUID;
		kNot.m_bReady	= false;
		BroadCast( EPM_PARTY_CHANGE_READY_NOT, kNot );
	}
}

IMPL_ON_FUNC( EPM_LEAVE_PARTY_REQ )
{
	//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	switch( GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
		{
			// 시간차로 발생할수 있는 문제이므로 에러로그 레벨을 cwarn으로 낮춘다.
			START_LOG( cwarn, L"이미 닫혀있는 파티인데 이탈 요청이 왔다! 하지만 시간차로 발생할 수 있는 문제임." )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< END_LOG;
			return;
		}
		break;

	case KCnPartyFSM::S_MAKING_PARTY:
	case KCnPartyFSM::S_WAIT:
	case KCnPartyFSM::S_CHECK_PLAY_CONDITION:
	case KCnPartyFSM::S_CHECK_AUTO_PARTY:
	case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
	case KCnPartyFSM::S_PREPARE_FOR_PLAY:
		break;

	default:
		{
			START_LOG( cerr, L"상태 오류. Name : " << m_strName )
				<< L"    현재 상태 : " << GetStateIDString() << dbg::endl
				<< END_LOG;

			KEPM_LEAVE_PARTY_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_WRONG_STATE_00;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_LEAVE_PARTY_ACK, kPacket );
			return;
		}
		break;
	}
#else
	VERIFY_PARTY_STATE_ACK( ( 4, KCnPartyFSM::S_MAKING_PARTY, KCnPartyFSM::S_WAIT, KCnPartyFSM::S_ACCEPT_FOR_PLAY, KCnPartyFSM::S_PREPARE_FOR_PLAY ), EPM_LEAVE_PARTY_ACK, KEPM_LEAVE_PARTY_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEPM_LEAVE_PARTY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	kPacket.m_iReason = kPacket_.m_iReason;
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	//{{ 2011. 11. 28	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kPacket.m_iNewJoinPartyUID = kPacket_.m_iNewJoinPartyUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	const UidType iLeavePartyUnitUID = FIRST_SENDER_UID;

	// 이탈 유저 정보	
	KPartyUserInfo kLeaveUserInfo;

	// 파티 소속이라면..
	if( m_kPartyUserManager.IsMember( iLeavePartyUnitUID ) == true )
	{
		// 이탈할 유저 정보를 찾는다.
		if( m_kPartyUserManager.GetUserInfo( iLeavePartyUnitUID, kLeaveUserInfo ) == false )
		{
			START_LOG( cerr, L"파티이탈할 유저 정보가 없습니다." )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( iLeavePartyUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_01;
			goto end_proc;
		}
	}
	// 대기자 라면..
	else if( m_kPartyUserManager.IsCandidate( iLeavePartyUnitUID ) == true )
	{
		// 대기자에서 지워 버리자!
		LIF( LeaveCandidateMember( iLeavePartyUnitUID ) );

		//당사자에게 성공했다고 알려주고 종료..
		goto end_proc;
	}
	else
	{
		// 대기자 였었는데 대기시간오버로 대기자에서 자동삭제된 경우 발생할 수 있다.
		START_LOG( cwarn, L"파티이탈할 유저 정보가 없습니다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( iLeavePartyUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_01;
		goto end_proc;
	}

	// 파티 이탈 처리
	if( LeaveMember( iLeavePartyUnitUID
				#ifdef LOG_PARTY_BREAK
					, kPacket.m_iReason
				#endif // LOG_PARTY_BREAK
					) == false )
	{
		//삭제실패.
		kPacket.m_iOK = NetError::GetLastError();
		goto end_proc;
	}

end_proc:

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_PARTY_REASON_05 )//&& kPacket_.m_iReason != NetError::NOT_LEAVE_ROOM_REASON_22)
#else
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_PARTY_REASON_05 )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
		//}}
	{
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_LEAVE_PARTY_ACK, kPacket );
	}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	if( m_kPartyUserManager.GetComeBackUserInParty() == true )			// 파티 생성시 있었는데
	{
		if( m_kPartyUserManager.CheckComeBackUserInParty() == false )	// 나간 유저 처리하고 나니깐 없다.
		{
			CTime tCurr = CTime::GetCurrentTime();

			// 복귀유저가 없으면 버프를 끈다.
			KEPM_CHANGE_COME_BACK_BUFF_NOT kNot;
			kNot.m_bIsBuffFinish = true;
			kNot.m_wstrComeBackBuffEnd = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
			BroadCast( EPM_CHANGE_COME_BACK_BUFF_NOT, kNot );
		}
	}
#endif SERV_COME_BACK_USER_REWARD
	//}}

	// 파티를 유지할 수 있을 만큼 파티원이 충분히 있는가?
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
	}
	else
	{
		switch( GetStateID() )
		{
		case KCnPartyFSM::S_MAKING_PARTY:
		case KCnPartyFSM::S_WAIT:
			break;

		case KCnPartyFSM::S_CHECK_PLAY_CONDITION:
			{
				// 남은 유저들만이라도 플레이 조건 체크 하자!
                CheckAllMembersPlayCondition();
			}
			break;

		case KCnPartyFSM::S_CHECK_AUTO_PARTY:
			{
				// 남은 유저들만이라도 자동 파티 조건 체크 하자!
				CheckAllMembersAutoParty();
			}
			break;

		case KCnPartyFSM::S_ACCEPT_FOR_PLAY:
			{
				// 남은 유저들만이라도 게임 시작 수락 체크 하자!
				CheckAllMembersAcceptForPlay();
			}
			break;

		case KCnPartyFSM::S_PREPARE_FOR_PLAY:
			{
				// 이탈한 파티원을 제외한 다른 파티원들 모두 게임 시작할 준비가 다되어있다면?
				if( m_kPartyUserManager.IsAllCheckedGameStart() )
				{
					// 파티장 찾기
					KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
					if( spHost == NULL )
					{
						START_LOG( cerr, L"파티장 찾기 실패!" )
							<< BUILD_LOG( GetUID() )
							<< END_LOG;
						return;
					}

					// 파티장에게 실패 패킷 전송
					KEGS_PARTY_GAME_START_ACK kAck;
					kAck.m_iOK = NetError::ERR_PARTY_23;
					spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kAck );
				}
			}
			break;
		}		
	}
}


IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_READY_ACK, KEGS_PARTY_CHANGE_READY_ACK );

	if( FIRST_SENDER_UID == m_kPartyUserManager.GetHostUID() )
	{
		// [참고] 파티위임이 되고난직후 ready변경을 시도했을 경우 발생 가능한 오류

		START_LOG( cwarn, L"파티장은 레디변경이 오면안된다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )			
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_32;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_READY_ACK, kPacket );
		return;
	}

	if( m_kPartyUserManager.ChangeReady( FIRST_SENDER_UID, kPacket_.m_bReady ) == false )
	{
		START_LOG( cerr, L"파티원의 레디 변경 실패." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_bReady )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_21;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_READY_ACK, kPacket );
		return;
	}

	//상태변경 완료를 당사자에게 알리고
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_READY_ACK, kPacket );

	// 파티원들에게 레디변경을 알려준다.
	KEGS_PARTY_CHANGE_READY_NOT kNot;
	kNot.m_UnitUID	= FIRST_SENDER_UID;
	kNot.m_bReady	= kPacket_.m_bReady;
	BroadCast( EPM_PARTY_CHANGE_READY_NOT, kNot );
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_DUNGEON_ACK, KEGS_PARTY_CHANGE_DUNGEON_ACK );

	if( m_kPartyUserManager.IsMember( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"파티 멤버가 아닙니다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_03;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );
		return;
	}

	const UidType iHostUID = m_kPartyUserManager.GetHostUID();
	if( FIRST_SENDER_UID != iHostUID )
	{
		START_LOG( cerr, L"파티장이 아닌데 던전정보 변경을 신청함." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( kPacket_.m_cDungeonMode == CXSLDungeon::DM_HENIR_CHALLENGE )
	{
		// 헤니르 시공 도전 모드는 같은 서버군의 유저끼리만 플레이 할 수 있다!
		if( m_kPartyUserManager.IsAllPartyMembersSameServerGroup( iHostUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_05;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// 클라이언트로 결과 전송
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_DUNGEON_ACK, kPacket );

	//## 1. 공개설정일 경우
	//## 2. 최대인원이 2명 이상일 경우
	//## 3. 게임중 상태가 아닌 경우
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		// 파티 리스트에서 먼저 지운다.
		DelPartyInfoInPartyList();
	}	

	//////////////////////////////////////////////////////////////////////////	
	// 바뀐 던전 정보 저장	
	SetDungeonID( kPacket_.m_iDungeonID );
	SetDungeonDif( kPacket_.m_DifficultyLevel );
	SetDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) );

	//바뀐던전 정보를 알려준다.
	KEGS_PARTY_CHANGE_DUNGEON_NOT kNot;
	kNot.m_iDungeonID		= kPacket_.m_iDungeonID;
	kNot.m_DifficultyLevel	= kPacket_.m_DifficultyLevel;
	kNot.m_cDungeonMode		= kPacket_.m_cDungeonMode;
	BroadCast( EPM_PARTY_CHANGE_DUNGEON_NOT, kNot );

	//던전설정이 바뀌었으므로 모든 파티원의 레뒤상태를 풀어준다.
	//ChangeAllReady() 함수안에서 레디 상태를 바꾼후 패킷을 날려준다.
	m_kPartyUserManager.ChangeAllReady( false );

	//@@ 파티리스트 업데이트.
	UpdatePartyInfoInPartyList();

	//## 1. 공개설정일 경우
	//## 2. 최대인원이 2명 이상일 경우
	//## 3. 게임중 상태가 아닌 경우
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		// 다시 추가한다.
		AddPartyInfoInPartyList();
	}
}

IMPL_ON_FUNC( EPM_PARTY_GAME_START_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_GAME_START_ACK, KEPM_PARTY_GAME_START_ACK );
	
	// 파티장인지 검사
	const UidType iHostUID = m_kPartyUserManager.GetHostUID();
	if( FIRST_SENDER_UID != iHostUID )
	{
		START_LOG( cerr, L"파티장이 아닌데 던전게임 시작을 보냄." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	// 파티장을 포함해서 파티원수가 1명뿐인경우 게임시작을 할수 없습니다!
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		START_LOG( cwarn, L"현재 파티원수가 부족하여 파티 게임 시작을 할 수 없습니다!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( iHostUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BATTLEFIELD_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_GAME_START_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( GetDungeonMode() == CXSLDungeon::DM_HENIR_CHALLENGE )
	{
		// 헤니르 시공 도전 모드는 같은 서버군의 유저끼리만 플레이 할 수 있다!
		if( m_kPartyUserManager.IsAllPartyMembersSameServerGroup( iHostUID ) == false )
		{
			START_LOG( cerr, L"헤니르 시공 도전 모드는 같은 서버군유저끼리만 플레이 가능! 이미 체크 했을 텐데.." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( iHostUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_05;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_GAME_START_ACK, kPacket );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// 파티 게임 시작에 필요한 정보를 임시로 저장한다.
	m_kGameManager.SetGameStartInfo( kPacket_.m_usEventID, kPacket_.m_sWorldID );

	// 체크 타이머 초기화
	m_tTimer[TE_CHECK_PLAY_CONDITION].restart();

	// 각 파티원들의 현재 던전 게임 시작 조건을 체크한다!
	StateTransition( KCnPartyFSM::I_TO_CHECK_PLAY_CONDITION );

	// 각 파티원들에게 체크 패킷 전송
	KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ kPacketReq;
	GetDungeonGameInfo( kPacketReq.m_kDungeonGameInfo );
	BroadCast( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ, kPacketReq );
}

//{{ 2012. 01. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_CHECK_PLAY_CONDITION ) );

	KEPM_PARTY_GAME_START_ACK kPacket;

	// 해당 파티원이 플레이 조건 만족을 못하는 경우!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 실패 처리 하자!
		FailProcessPlayCondition( kPacket_.m_iOK, FIRST_SENDER_UID, kPacket_.m_wstrFailUserNickName );
		return;
	}

	// 조건 체크를 만족한 유저는 체크 등록 하자!
	if( m_kPartyUserManager.SetCheckedPlayCondition( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"조건 체크를 등록하려는 유저가 파티원이 아닙니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( GetName() )
			<< END_LOG;
		return;
	}

	// 모든 멤버의 체크가 완료되었는지 확인!
	CheckAllMembersPlayCondition();
}

_IMPL_ON_FUNC( EPM_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT, KEGS_PARTY_GAME_START_OPINION_CHECK_REPLY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_ACCEPT_FOR_PLAY ) );

	// 한명의 파티원이라도 동의하지 않는다면 게임을 시작하지 않습니다.
	if( kPacket_.m_bAccept == false )
	{
		FailProcessAcceptForPlay( NetError::NOT_PARTY_GAME_START_FAILED_00, FIRST_SENDER_UID ); // 동의하지 않은 사람이 있습니다!
        return;
	}

	// 해당 파티원의 Accept정보를 저장합니다.
	if( m_kPartyUserManager.ChangeReady( FIRST_SENDER_UID, true ) == false )
	{
        START_LOG( cerr, L"해당 파티원이 존재하지 않습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 모든 파티원들이 Accept처리가 되었는지 체크하자!
	CheckAllMembersAcceptForPlay();
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EPM_CHECK_FOR_PARTY_GAME_START_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_PREPARE_FOR_PLAY ) );

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	// 소속 파티원인지 검사
	KCnPartyUserPtr spUser = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spUser == NULL )
	{
		START_LOG( cerr, L"게임시작 체크패킷 응답이 왔는데 소속 파티원이 아니다?" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 여기서 끝낸다.
		return;
	}

	// 파티장에게 실패 에러를 보낸다.
	KCnPartyUserPtr spHost = m_kPartyUserManager.GetHostPtr();
	if( spHost == NULL )
	{
		START_LOG( cerr, L"파티장 정보가 없습니다. 있을수 없는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
			<< END_LOG;
		return;
	}

	// NetError가 OK값이 아니라면..
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"여기까지 왔는데 게임을 만족하지 않는 멤버가 존재한다! 오류인가 예외상황인가?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
			<< END_LOG;

		// 게임을 시작할 조건을 만족하지 않는 멤버가 존재한다. 게임 시작을 취소하자!
		FailProcessPrepareForPlay( kPacket_.m_iOK );
		return;
	}

	// 도착한 PartyRoomUserInfo를 검사
	if( kPacket_.m_vecUserInfo.empty() )
	{
		START_LOG( cerr, L"게임시작 체크패킷 응답이 왔는데 PartyRoomUserInfo가 비어있다?" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 게임을 시작할 조건을 만족하지 않는 멤버가 존재한다. 게임 시작을 취소하자!
		FailProcessPrepareForPlay( kPacket_.m_iOK );
		return;
	}

	KPartyRoomUserInfo kPartyRoomUserInfo;
	BOOST_TEST_FOREACH( const KPartyRoomUserInfo&, kInfo, kPacket_.m_vecUserInfo )
	{
		kPartyRoomUserInfo = kInfo;
		break;
	}

	// 도착한 PartyRoomUserInfo를 CnPartyUser에 업데이트 한다.
	spUser->SetPartyRoomUserInfo( kPartyRoomUserInfo );

	// 모든 파티 유저의 PartyRoomUserInfo가 도착해있는지 검사.
	if( m_kPartyUserManager.IsAllCheckedGameStart() == false )
	{
		// 여기서 끝내고 다음 유저가 보내는 패킷에서 다시 체크.
		return;
	}

	// 파티원들의 PartyRoomUserInfo를 얻자
	KERM_PARTY_OPEN_DUNGEON_ROOM_REQ kPacket;
	if( m_kPartyUserManager.GetDungeonRoomUserInfoList( kPacket.m_vecUserInfo ) == false )
	{
		START_LOG( cerr, L"파티를 통한 던전게임 시작준비중 유저정보 수집실패.!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		// 게임을 시작할 조건을 만족하지 않는 멤버가 존재한다. 게임 시작을 취소하자!
		FailProcessPrepareForPlay( kPacket_.m_iOK );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 모든 유저의 PartyRoomUserInfo가 준비되었다면 게임시작!

	// 게임 시작할 준비가 되었으니 상태이동!
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	// 던전방을 만들수 있도록 정보를 수집한다.	
	kPacket.m_sWorldID						= kPacket_.m_sWorldID;
	kPacket.m_kRoomInfo.m_RoomType			= CXSLRoom::RT_DUNGEON;
	kPacket.m_kRoomInfo.m_bPublic			= false;
	kPacket.m_kRoomInfo.m_MaxSlot			= static_cast<char>(m_kPartyUserManager.GetMaxMember());
	kPacket.m_kRoomInfo.m_iDungeonID		= GetDungeonID();
	kPacket.m_kRoomInfo.m_DifficultyLevel	= GetDungeonDif();	
	kPacket.m_kRoomInfo.m_cDungeonMode		= static_cast<char>(GetDungeonMode());	
	kPacket.m_kRoomInfo.m_iPartyUID			= GetUID();

	switch( GetItemType() )
	{
	case CXSLRoom::DGIT_PERSON:
	case CXSLRoom::DGIT_RANDOM:
		kPacket.m_kRoomInfo.m_cGetItemType = GetItemType();
		break;
	default:
		//설정이 잘못되어있으면 항상 랜덤으로.
		kPacket.m_kRoomInfo.m_cGetItemType = CXSLRoom::DGIT_RANDOM;
		break;
	}
	kPacket.m_kRoomInfo.m_fPlayTime = DUNGEON_DEFAULT_LIMIT_PLAY_TIME;

	float fPlayTimeLimit = SiCXSLDungeonManager()->GetPlayTimeLimit( ( kPacket.m_kRoomInfo.m_iDungeonID + kPacket.m_kRoomInfo.m_DifficultyLevel ) );
	if( fPlayTimeLimit > 0.0f )
	{
		kPacket.m_kRoomInfo.m_fPlayTime = fPlayTimeLimit;
	}

	u_short usEventID = 0;
	switch( kPacket_.m_usEventID )
	{
	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ: usEventID = EPM_PARTY_OPEN_DUNGEON_ROOM_NOT; break;
	//case ERM_PARTY_GAME_REPLAY_START_REQ: usEventID = EPM_PARTY_GAME_REPLAY_START_NOT; break;
	default:
		{
			START_LOG( cerr, L"파티를 통한 던전게임 시작준비중 유저정보 수집실패.!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			//kAck.m_iOK = NetError::ERR_PARTY_23;
			//spHost->SendToGSCharacter( EPM_PARTY_GAME_START_ACK, kAck );
		}
		break;
	}

	// 파티장에게 던전방 만들으라고 패킷 전송
	spHost->SendToGSCharacter( usEventID, kPacket );

	//{{ 2009. 6. 10  최육사	퀵슬롯 쿨타임 리셋
	AllPartyMembersQuickSlotCoolTimeReset();
	//}}

	//게임중으로 설정하고
	SetPlaying( true );

	//파티리스트에서 빼준다.
	DelPartyInfoInPartyList();
	//////////////////////////////////////////////////////////////////////////
}

IMPL_ON_FUNC( EPM_END_GAME_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cwarn, L"파티장이 아닌데 E_EndGame을 호출했다?" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;
	}

	// 게임이 종료되었으므로 파티유저들의 ready를 풀어준다.
	m_kPartyUserManager.ChangeAllReady( false );
	SetPlaying( false );
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_HOST_ACK, KEGS_PARTY_CHANGE_HOST_ACK );

	//파티장 확인.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 파티장 변경을 신청함." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	//바뀔 파티장이 있는지 확인.
	KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( kPacket_.m_iNewHostUnitUID );
	if( spNewHost == NULL )
	{
		// 시간차로 발생할 수 있는 오류임.
		START_LOG( cwarn, L"새로 임명할 파티장이 없습니다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_27;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	if( ( GetDungeonID() + static_cast<int>(GetDungeonDif()) ) <= 0 )
	{
		START_LOG( cerr, L"dungeonid값이 이상하다!" )
			<< BUILD_LOG( GetDungeonID() )
			<< BUILD_LOGc( GetDungeonDif() )
			<< END_LOG;
	}
	//////////////////////////////////////////////////////////////////////////

	KEPM_CHECK_PARTY_CHANGE_HOST_REQ kPacketReq;
	kPacketReq.m_iPartyUID = kPacket_.m_iPartyUID;
	//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kPacketReq.m_cPartyType = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	kPacketReq.m_iOldHostUnitUID = FIRST_SENDER_UID;
	kPacketReq.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;
	kPacketReq.m_iDungeonID		 = GetDungeonID();
	kPacketReq.m_cDifficulty	 = GetDungeonDif();
	kPacketReq.m_cDungeonMode	 = GetDungeonMode();
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	kPacketReq.m_bForRegroupParty = false;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	spNewHost->SendToGSCharacter( EPM_CHECK_PARTY_CHANGE_HOST_REQ, kPacketReq );
}

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	KEGS_PARTY_CHANGE_HOST_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;	

	//현재 파티장 정보
	KCnPartyUserPtr spOldHost = m_kPartyUserManager.GetHostPtr();
	if( spOldHost == NULL )
	{
		START_LOG( cerr, L"현재 파티장이 없습니다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< END_LOG;

		// 여기서 끝남.
		return;
	}

	//파티장 확인.
	if( kPacket_.m_iOldHostUnitUID != spOldHost->GetCharUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 파티장 변경을 신청함." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< BUILD_LOG( spOldHost->GetCharUID() )
			<< END_LOG;

		// 여기서 끝남
		return;
	}

	// OK값 체크!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kAck.m_iOK = kPacket_.m_iOK;
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		if( kPacket_.m_bForRegroupParty == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		{
			spOldHost->SendToGSCharacter( EPM_PARTY_CHANGE_HOST_ACK, kAck );
		}
		return;
	}

	//바뀔 파티장이 있는지 확인.
	KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( kPacket_.m_iNewHostUnitUID );
	if( spNewHost == NULL )
	{
		START_LOG( cerr, L"새로 임명할 파티장이 없습니다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_27;
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////	
	//파티장 바꾸기

	if( m_kPartyUserManager.ChangeHost( kPacket_.m_iNewHostUnitUID ) == false )
	{
		START_LOG( cerr, L"파티장 변경실패." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_27;
		goto end_proc;
	}

	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	if( kPacket_.m_bForRegroupParty == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	{
		spOldHost->SendToGSCharacter( EPM_PARTY_CHANGE_HOST_ACK, kAck );
	}

	// 결과 파티원에게 알리기
	{
		KEGS_PARTY_CHANGE_HOST_NOT kNot;
		kNot.m_iBeforeUID	 = kPacket_.m_iOldHostUnitUID;
		kNot.m_iAfterUID	 = m_kPartyUserManager.GetHostUID();
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		kNot.m_bRegroupParty = kPacket_.m_bForRegroupParty;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		BroadCast( EPM_PARTY_CHANGE_HOST_NOT, kNot );
	}

	//파티장이 바뀌는 것이므로 모든 유저의 레디상태를 false로 변경한다.
	m_kPartyUserManager.ChangeAllReady( false );

	// 모든 게임서버의 파티 리스트에 변경된 정보 갱신
	{
		KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT kPacketNot;
		kPacketNot.m_iPartyUID = GetUID();
		kPacketNot.m_vecUpdatePartyUser.push_back( spOldHost->GetPartyUserInfo() ); // 예전 파티장 정보 얻기
		kPacketNot.m_vecUpdatePartyUser.push_back( spNewHost->GetPartyUserInfo() ); // 새로운 파티장 정보 얻기
		BroadCastGameServer( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, kPacketNot );
	}

	return;

end_proc:
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	if( kPacket_.m_bForRegroupParty == false )
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	{
		spOldHost->SendToGSCharacter( EPM_PARTY_CHANGE_HOST_ACK, kAck );
	}
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_NUM_OF_PER_ACK, KEGS_PARTY_CHANGE_NUM_OF_PER_ACK );

	//파티장 확인.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 파티최대인원 변경을 신청함." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );
		return;
	}

	//현재 설정된 최대인원과 틀려야함.
	if( m_kPartyUserManager.GetMaxMember() == kPacket_.m_cNumOfPer )
	{
		START_LOG( cwarn, L"현재 최대설정과 같은 설정값이 옴.")
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_28;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );
		return;
	}

	//현재 인원보다는 같거나 커야한다.
	if( m_kPartyUserManager.GetNumTotal() > kPacket_.m_cNumOfPer )
	{
		// [참고] 시간차로 발생할수 있는 문제이므로 cwarn
		START_LOG( cwarn, L"현재 존재하는 유저보다 작은 설정값이 날아옴." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_28;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket );

	//@@ 방리스트 갱신
	if( IsPlaying() == false )
	{
		//## 1. 공개파티고 설정인원수가 1명일때 리스트 삭제
		if( IsPublic() == true && kPacket_.m_cNumOfPer <= 1 )
		{
			DelPartyInfoInPartyList();
		}
		//## 2. 공개파티고 설정인원수를 1명에서 2명이상으로 바꿀때 리스트 추가.
		else if( IsPublic() == true && m_kPartyUserManager.GetMaxMember() <= 1 && kPacket_.m_cNumOfPer >= 2 )
		{
			AddPartyInfoInPartyList();
		}
	}

	// 방 최대 인원 저장
	m_kPartyUserManager.SetMaxMember( kPacket_.m_cNumOfPer );

	// 다른 파티원들에게도 알림
	KEGS_PARTY_CHANGE_NUM_OF_PER_NOT kPacketNot;
	kPacketNot = kPacket_.m_cNumOfPer;
	BroadCast( EPM_PARTY_CHANGE_NUM_OF_PER_NOT, kPacketNot );

	// 파티 리스트에 있는 파티 정보 업데이트
	UpdatePartyInfoInPartyList();
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK );

	//파티장 확인.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 던전게임 아이템먹는 방식 변경을 신청함." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////	
	// 아이템 획득 타입 변경
	SetItemType( kPacket_.m_cGetItemType );

	// 다른 파티원들에게 알림
	KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT kPacketNot;
	kPacketNot = kPacket_.m_cGetItemType;
	BroadCast( EPM_PARTY_CHANGE_GET_ITEM_TYPE_NOT, kPacketNot );
}

IMPL_ON_FUNC( EPM_PARTY_BAN_USER_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_BAN_USER_ACK, KEGS_PARTY_BAN_USER_ACK );

	kPacket.m_iOK = NetError::NET_OK;

	//파티장 확인.
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 파티강퇴 을 신청함." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_BAN_USER_ACK, kPacket );
		return;
	}

	//강퇴당할 유저가 있는지 확인..
	KCnPartyUserPtr spBanUser = m_kPartyUserManager.GetPartyUser( kPacket_.m_iUnitUID );
	if( spBanUser == NULL )
	{
		// [참고] 시간차로 유저가 없을수 있다
		START_LOG( cwarn, L"파티에서 강퇴할 유저가 없습니다." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_17;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_BAN_USER_ACK, kPacket );
		return;
	}

	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_BAN_USER_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////	
	// 강퇴 패킷 전달
	KEPM_PARTY_BAN_USER_NOT kPacketNot;
	kPacketNot.m_iPartyUID = GetUID();
	spBanUser->SendToGSCharacter( EPM_PARTY_BAN_USER_NOT, kPacketNot );

	//강퇴시킴.
	if( LeaveMember( kPacket_.m_iUnitUID
			#ifdef LOG_PARTY_BREAK
					, NetError::NOT_LEAVE_PARTY_REASON_07
			#endif // LOG_PARTY_BREAK
					) == false )
	{
		START_LOG( cerr, L"강퇴를 위한 파티원 정보 삭제 처리가 실패하였습니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( NetError::GetLastErrMsg() )
			<< END_LOG;
	}
	
	// 파티에 아무도 없나.?
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
	}
}

IMPL_ON_FUNC( EPM_CHAT_REQ )
{
	VERIFY_PARTY_STATE_ACK_WARN( ( 3, KCnPartyFSM::S_WAIT, KCnPartyFSM::S_ACCEPT_FOR_PLAY, KCnPartyFSM::S_PREPARE_FOR_PLAY ), EPM_CHAT_ACK, KEGS_CHAT_ACK );

	KEGS_CHAT_NOT kNot;
	kNot.m_SenderUnitUID	= FIRST_SENDER_UID;
	kNot.m_RecieverUnitUID	= kPacket_.m_ToUnitUID;
	kNot.m_cChatPacketType  = kPacket_.m_cChatPacketType;
	kNot.m_cRoomChatType	= kPacket_.m_cRoomChatType;
	kNot.m_wstrMsg			= kPacket_.m_wstrMsg;

	switch( kPacket_.m_cRoomChatType )
	{
	case CXSLRoom::RCT_ALL:
		{
			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );
			BroadCast( EPM_CHAT_NOT, kNot );
			return;
		}
		break;

	case CXSLRoom::RCT_WHISPER:
		{
			KCnPartyUserPtr spWhisper = m_kPartyUserManager.GetPartyUser( kPacket_.m_ToUnitUID );
			if( spWhisper == NULL )
			{
				// 시간차로 대상자가 파티이탈 할 수 있으므로 발생할 수 있는 오류임.
				START_LOG( cwarn, L"파티 채팅 상대자가 없음." )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( FIRST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_ToUnitUID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_33;
				SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );
				return;
			}

			kPacket.m_iOK = NetError::NET_OK;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );

			// 자기 자신과 귓속말 대상자에게 채팅 메시지 전달
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_NOT, kNot );
			spWhisper->SendToGSCharacter( EPM_CHAT_NOT, kNot );
			return;
		}
		break;

	default:
		{
			START_LOG( cerr, L"이상한 채팅 타입." )
				<< BUILD_LOGc( kPacket_.m_cRoomChatType )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_24;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHAT_ACK, kPacket );
		}
		break;
	}
}

IMPL_ON_FUNC( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK kAck;

	//{{ 2012. 11. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	const float fEnterSecretRate = SiCXSLPartyManager()->GetDungeonSecretStageEnterRate( m_kFeverManager.GetRealFeverPoint(), m_kPartyUserManager.GetNumMember() );
#else
	//// 확률값 얻기	
	//const int iPartyFeverPoint = m_kFeverManager.GetRealFeverPoint();

	////{{ 2011. 10. 04  김민성	   헤니르 밸런스 패치
	////#ifdef SERV_HENIR_BALANCE
	//float fEnterSecretRate = 0.0f;
	//if( m_kPartyUserManager.GetNumMember() > 1 )
	//{
	//	fEnterSecretRate = static_cast<float>( (15 + (iPartyFeverPoint * 0.1)) * ( m_kPartyUserManager.GetNumMember() -1) );
	//}
	//else
	//{
	//	fEnterSecretRate = 0.0f;
	//}
	////#else
	////switch( m_kPartyUserManager.GetNumMember() )
	////{
	////case 1: // 파티원이 1명이하면 비밀 스테이지 입장 불가!
	////	{
	////		START_LOG( cerr, L"파티원이 1명이군요. 클라에서 검사했을텐데?!" )
	////			<< BUILD_LOG( kPacket_.m_iPartyUID )
	////			<< BUILD_LOG( FIRST_SENDER_UID )
	////			<< END_LOG;

	////		kAck.m_bCanEnterSecretStage = false;
	////		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );
	////		return;
	////	}
	////	break;
	////case 2: fEnterSecretRate = static_cast<float>(iPartyFeverPoint) * 0.3f;	break;
	////case 3: fEnterSecretRate = static_cast<float>(iPartyFeverPoint) * 0.6f;	break;
	////case 4: fEnterSecretRate = static_cast<float>(iPartyFeverPoint) * 1.f;	break;
	////default:
	////	{
	////		START_LOG( cerr, L"파티인원수가 이상하다!" )
	////			<< BUILD_LOG( GetNumMember() )
	////			<< END_LOG;

	////		kAck.m_bCanEnterSecretStage = false;
	////		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );
	////		return;
	////	}
	////	break;
	////}
	////#endif SERV_HENIR_BALANCE
	////}}

	//// 랜덤하게 입장 여부 결정!
	//if( fEnterSecretRate > 100.f )
	//	fEnterSecretRate = 100.f;	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	KLottery kLot;
	kLot.AddCase( 1, fEnterSecretRate );
	int iResult = kLot.Decision();
	if( iResult == KLottery::CASE_BLANK )
	{
		// 비밀 스테이지 입장 실패!		
		kAck.m_bCanEnterSecretStage = false;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );

		//{{ 2012. 11. 16	최육사		헤니르 시공 비밀 스테이지 덤프
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
		SiCXSLPartyManager()->SetHenirSecretStageEnterRateDump( m_kFeverManager.GetRealFeverPoint(), m_kPartyUserManager.GetNumMember(), fEnterSecretRate, false );
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
		//}}
		return;
	}

	// 조건 만족되면 마지막 조건체크하러 센터로 이벤트 전송!
	kAck.m_bCanEnterSecretStage = true;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kAck );

	//{{ 2012. 11. 16	최육사		헤니르 시공 비밀 스테이지 덤프
#ifdef SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	SiCXSLPartyManager()->SetHenirSecretStageEnterRateDump( m_kFeverManager.GetRealFeverPoint(), m_kPartyUserManager.GetNumMember(), fEnterSecretRate, true );
#endif SERV_HENIR_SECRET_STAGE_ENTER_RESULT_DUMP
	//}}
}

_IMPL_ON_FUNC( EPM_INCREASE_PARTY_FEVER_NOT, KERM_INCREASE_PARTY_FEVER_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	// Party Fever값 증가
	IncreaseFeverPoint( kPacket_.m_cRoomType, kPacket_.m_sSubStageNpcCount );
}

//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( EPM_MAX_PARTY_FEVER_NOT, KERM_MAX_PARTY_FEVER_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	// Party Fever값 Max
	UpdateAndBroadCastFeverPoint( KCnPartyFeverManager::FEVER_100_PERCENT_POINT );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	// 파티 멤버 리스트를 필드에 업데이트하러가자!
	KEPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT kPacket;
	kPacket.m_iPartyUID = GetUID();
	m_kPartyUserManager.GetUnitUIDList( kPacket.m_setPartyUIDList );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT, kPacket );
}

_IMPL_ON_FUNC( EPM_CHECK_INVALID_USER_NOT, UidType )
{
	const UidType iDisconnectedGSUID = kPacket_;

	// 삭제할 유저 리스트 얻기
	std::vector< UidType > vecDeleteUserList;
	m_kPartyUserManager.GetUnitUIDListByGSUID( iDisconnectedGSUID, vecDeleteUserList );

	BOOST_TEST_FOREACH( const UidType, iDeleteUnitUID, vecDeleteUserList )
	{
		//파티에서 나갈유저를 빼주자.
		if( LeaveMember( iDeleteUnitUID 
			#ifdef LOG_PARTY_BREAK
						, NetError::NOT_LEAVE_PARTY_REASON_09
			#endif // LOG_PARTY_BREAK
						) == false )
			
		{
            START_LOG( cerr, L"유효하지 않은 파티원 정보를 삭제하다가 실패하였습니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( iDeleteUnitUID )
				<< BUILD_LOG( NetError::GetLastErrMsg() )
				<< END_LOG;
		}

		// 파티에 아무도 없나.?
		if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
		{
			CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
		}
	}
}

//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, KEPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK );

	//파티장인지 검사
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 최소 레벨 입장 제한 변경을 신청함." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket );
		return;
	}

	//현재 설정과 같은지 검사.
	if( GetCheckLowLevel() == kPacket_.m_bCheck )
	{
		START_LOG( cwarn, L"현재설정과 같은 최소 레벨 입장 제한 설정값." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetCheckLowLevel() )
			<< BUILD_LOG( kPacket_.m_bCheck )
			<< BUILD_LOG( GetName() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_43;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	// 공개 설정 변경 처리

	//공개설정을 바꾸고.
	SetCheckLowLevel( kPacket_.m_bCheck );

	// 다른 파티원들에게 바뀐 설정 알림
	BroadCast( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT, kPacket_.m_bCheck );

	//@@ 방리스트 추가&삭제
	if( IsPlaying() == false )
	{
		if( IsPublic() == true &&  m_kPartyUserManager.GetMaxMember() >= 2 )
		{
			AddPartyInfoInPartyList();
		}
	}

	// 파티 정보 갱신
	UpdatePartyInfoInPartyList();
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
_IMPL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_REQ, KEGS_CHANGE_PARTY_TYPE_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_CHANGE_PARTY_TYPE_ACK, KEGS_CHANGE_PARTY_TYPE_ACK );

	//파티장인지 검사
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 파티 타입을 변경 하려함." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
		return;
	}

	// 파티 타입 체크
	if( GetPartyType() == kPacket_.m_cPartyType )
	{
		START_LOG( cerr, L"이미 해당 타입인데 또 바꿀려고 하네?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOGc( GetPartyType() )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_05;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
		return;
	}

	// 대전 파티로 변경할때 체크해야하는 조건들..
	if( kPacket_.m_cPartyType == KPartyInfo::PT_PVP_PARTY )
	{
		// 던전파티에서 대전파티로 변경할때 구성인원 수가 4명이면 변경하지 못한다.
		if( m_kPartyUserManager.GetNumMember() >= PE_LIMIT_PARTY_MEBMER )
		{
			START_LOG( cerr, L"파티인원이 4명일때는 대전파티로 변경이 불가능하다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( kPacket_.m_cPartyType )
				<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MATCH_MAKING_06;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
			return;
		}

		// 파티인원중 레벨이 10렙이 안되는 유저가 있는경우 대전파티로 변경이 불가능.
		if( m_kPartyUserManager.IsAllPartyMemberEqualOrOverThisLevel( SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL ) == false )
		{
			START_LOG( cerr, L"파티원중에 10레벨이 안되는 멤버가 있습니다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOGc( kPacket_.m_cPartyType )
				<< BUILD_LOG( m_kPartyUserManager.GetNumMember() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MATCH_MAKING_08;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 그외에 파티 타입 변경시 체크해야할 것들은 여기에서 체크하자!!
	//////////////////////////////////////////////////////////////////////////

	// 파티 타입 변경
	SetInfo( kPacket_ );

	// 결과처리
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );

    // 모든 파티원들에게 알린다!
	KEGS_CHANGE_PARTY_TYPE_NOT kNot;
	GetInfo( kNot.m_kPartyInfo );	
	BroadCast( EPM_CHANGE_PARTY_TYPE_NOT, kNot );

	// 파티 정보 갱신
	UpdatePartyInfoInPartyList( true );

	// 파티 말풍선 추가 삭제
	if( GetPartyType() == KPartyInfo::PT_DUNGEON_PARTY )
	{
		// 파티 리스트에 있는 파티 정보 업데이트
		AddPartyInfoInPartyList();
	}
	else
	{
		// 모든 파티원들의 ready상태를 초기화 한다!
		m_kPartyUserManager.ChangeAllReady( false );

		// 게임서버의 파티 리스트에 파티 정보 삭제
		DelPartyInfoInPartyList( true );
	}
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK );

	// 대전 매칭 재신청인지 검사
	if( kPacket_.m_bRegFrontOfList == false )
	{
		// 재신청이 아니라면 파티장인지 검사하자!
		if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
		{
			START_LOG( cerr, L"파티장이 아닌데 매치 메이킹을 신청하려함." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_26;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, kPacket );
			return;
		}
	}

	// 공식대전은 10레벨 이상만 가능합니다.
	if( m_kPartyUserManager.IsAllPartyMemberEqualOrOverThisLevel( SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL ) == false )
	{
		START_LOG( cerr, L"파티원중에 10레벨이 안되는 멤버가 있습니다!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_09;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_CHANGE_PARTY_TYPE_ACK, kPacket );
		return;
	}

	// 매치 유저 정보 얻어오기!
	m_kPartyUserManager.GetMatchUserInfoList( GetUID(), kPacket.m_vecMatchUserInfo );

	// 매치 신청하러 가자!
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_bRegFrontOfList = kPacket_.m_bRegFrontOfList;
	kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
	kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_REQ, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );

	//파티장인지 검사
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 매치 정보를 수정하려함." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket );
		return;
	}

	// 정상적인 대전 타입인지 체크
	if( kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM  &&
		kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM_DEATH )
	{
		START_LOG( cerr, L"대전 파티에서 세팅 가능한 대전 타입이 아닙니다." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< BUILD_LOGc( kPacket_.m_cPvpGameType )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket );
		return;
	}

	// 게임 타입 저장
	m_cPvpGameType = kPacket_.m_cPvpGameType;

	// 매치 정보 변경!
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket );

	// 모든 파티원들에게 전달
	KEGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT kPacketNot;
	kPacketNot.m_cPvpGameType = kPacket_.m_cPvpGameType;
	BroadCast( EPM_PVP_PARTY_CHANGE_MATCH_INFO_NOT, kPacketNot );
}

#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ )
{
	// 시간차로 파티가 닫혀있는 상태라면 파티 정보 얻지 말고 그대로 돌려보내자!
	switch( GetStateID() )
	{
	case KCnPartyFSM::S_INIT:
	case KCnPartyFSM::S_CLOSE:
		{
			KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, kPacket );
			return;
		}
		break;
	}

	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK );

	// 현재 파티원수를 저장해서 다시 돌려주자!
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kBattleFieldJoinInfo = kPacket_.m_kBattleFieldJoinInfo;

	// 파티원 리스트를 얻자
	m_kPartyUserManager.GetUnitUIDList( kPacket.m_kBattleFieldJoinInfo.m_vecPartyMemberList );
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, kPacket );
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_WAIT ), EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK );

	//파티장인지 검사
	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		START_LOG( cerr, L"파티장이 아닌데 매치 정보를 수정하려함." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_26;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( CXSLDungeon::IsFixedMembers( GetDungeonIDAndDif() ) == true )
	{
		int iFixedMembers = CXSLDungeon::GetFixedMembers( GetDungeonIDAndDif() );
		int iCurrentMembers = m_kPartyUserManager.GetNumMember();

		if( iCurrentMembers == iFixedMembers )
		{
			kPacket.m_iOK = NetError::ERR_PARTY_61;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
			return;
		}
		else if( iCurrentMembers > iFixedMembers )
		{
			// 인원 초과로 실패
			kPacket.m_iOK = NetError::ERR_PARTY_46;
			SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
			return;
		}
	}
	else // 인원 고정 던전이 아닐경우 예전과 같은 수행
#endif SERV_DUNGEON_OPTION_IN_LUA

	//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	if( SiKAutoPartyScriptManager()->GetAutoPartyMatchingSuccessNum( GetDungeonIDAndDif() ) <= m_kPartyUserManager.GetNumMember() )
	{
		kPacket.m_iOK = NetError::ERR_PARTY_61;
		SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

	// 타이머 초기화
	m_tTimer[TE_CHECK_AUTO_PARTY].restart();

	// 자동 파티 생성을 위한 체크 상태로 이동!
	StateTransition( KCnPartyFSM::I_TO_CHECK_AUTO_PARTY );

	// 현재 자동 파티 신청이 재신청 온것인지 저장!
	m_bRemakingAutoParty = kPacket_.m_bRemakingAutoParty;

	// 준비 상태 체크값을 초기화!
	m_kPartyUserManager.ClearCheckedForAutoParty();

	// 각 파티원들에게 현재 준비 상태를 체크 하러 가자!
	KEPM_CHECK_FOR_AUTO_PARTY_MAKING_REQ kPacketReq;
	GetDungeonGameInfo( kPacketReq );
	BroadCast( EPM_CHECK_FOR_AUTO_PARTY_MAKING_REQ, kPacketReq );
}

IMPL_ON_FUNC( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_CHECK_AUTO_PARTY ) );

	// 체크 결과가 실패라면 자동 파티 신청 역시 실패 처리!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 실패 처리!
		FailProcessAutoParty( kPacket_.m_iOK, kPacket_.m_wstrFailUserNickName );
		return;
	}
	
	// 해당 파티원 체크 상태 업데이트!
	if( m_kPartyUserManager.SetCheckedForAutoParty( FIRST_SENDER_UID ) == false )
	{
		START_LOG( cerr, L"파티원 체크 등록 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}

	// 해당 파티원의 체크 상태 업데이트!
	CheckAllMembersAutoParty();
}

IMPL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	VERIFY_PARTY_STATE_ACK( ( 1, KCnPartyFSM::S_INIT ), EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK );
		
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;

	// 재집결하는 파티원수 검사
	if( static_cast<int>(kPacket_.m_vecRegroupPartyUserInfo.size()) > CXSLPartyManager::PE_MAX_PARTY_NUM )
	{
        START_LOG( cerr, L"재 집결하는 파티원수가 4명을 넘었다! 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kPacket_.m_vecRegroupPartyUserInfo.size() )
			<< END_LOG;
	}

	// 새로운 파티 생성 및 모든 새로운 파티원들 입장 처리
	if( m_kPartyUserManager.Create( kPacket_.m_vecRegroupPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"생성한 파티에 유저 리스트 입장실패.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecRegroupPartyUserInfo.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_00;
		goto end_proc;
	}

	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	// 자동 파티 파티장을 설정해주자!
	if( kPacket_.m_iAutoPartyHostUnitUID != 0  &&  m_kPartyUserManager.GetHostUID() != kPacket_.m_iAutoPartyHostUnitUID )
	{
		if( m_kPartyUserManager.ChangeHost( kPacket_.m_iAutoPartyHostUnitUID ) == false )
		{
			START_LOG( cerr, L"파티장 변경실패." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_kPartyUserManager.GetHostUID() )
				<< BUILD_LOG( kPacket_.m_iAutoPartyHostUnitUID )
				<< END_LOG;
			// 에러로그만 찍자
		}
	}
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	// 유저설정을 파티에 셋팅해주고
	SetInfo( kPacket_.m_kCreatePartyInfo );

	// 게임 시작을 할 수 있도록 PartyRoomUserInfo도 저장
	BOOST_TEST_FOREACH( const KRegroupPartyUserInfo&, kRegroupPartyUserInfo, kPacket_.m_vecRegroupPartyUserInfo )
	{
		KCnPartyUserPtr spUser = m_kPartyUserManager.GetPartyUser( kRegroupPartyUserInfo.m_kPartyRoomUserInfo.m_kRoomUserInfo.m_nUnitUID );
		if( spUser == NULL )
		{
			START_LOG( cerr, L"게임시작 체크패킷 응답이 왔는데 소속 파티원이 아니다?" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kRegroupPartyUserInfo.m_kPartyRoomUserInfo.m_kRoomUserInfo.m_nUnitUID )
				<< END_LOG;
			continue;
		}

		// PartyRoomUserInfo저장
        spUser->SetPartyRoomUserInfo( kRegroupPartyUserInfo.m_kPartyRoomUserInfo );

		// OldPartyInfo저장!
		spUser->SetOldPartyInfo( kRegroupPartyUserInfo.m_kOldPartyInfo );
	}

	// 파티리스트에 생성된 파티 추가
	{
		KEPM_OPEN_PARTY_LIST_NOT kPacketNot;
		GetInfo( kPacketNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kPacketNot.m_vecUserInfo );
		BroadCastGameServer( EPM_OPEN_PARTY_LIST_NOT, kPacketNot );
	}

	//@@ 방리스트 추가
	//## 1. 공개일경우
	//## 2. 최대인원이 2명 이상일 경우.
	//## 3. 게임중인 상태가 아닌경우.
	//{{ 2011. 07. 04	최육사	대전 개편
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		AddPartyInfoInPartyList();
	}

	//필드정보 업데이트.
	m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

	// 파티 생성을 위한 기본 작업이 끝났으니 상태 이동을 하자!
	StateTransition( KCnPartyFSM::I_TO_MAKING_PARTY );
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////
	// 파티 가입이 완료되었으니 가입완료된 정보를 각 파티원들에게 알려주고
	{
		KEGS_REGROUP_PARTY_NOT kRegroupNot;
		GetInfo( kRegroupNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kRegroupNot.m_vecPartyUserInfo );
		BroadCast( EPM_REGROUP_PARTY_NOT, kRegroupNot );
	}

	//////////////////////////////////////////////////////////////////////////
	// 바로 게임 시작 처리 하자!

	// 파티원들의 PartyRoomUserInfo를 얻자
	{
		KERM_PARTY_OPEN_DUNGEON_ROOM_REQ kPacketStartGame;
		KCnPartyUserPtr spHost;

		if( m_kPartyUserManager.GetDungeonRoomUserInfoList( kPacketStartGame.m_vecUserInfo ) == false )
		{
			START_LOG( cerr, L"파티를 통한 던전게임 시작준비중 유저정보 수집실패.!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_AUTO_PARTY_00;
			goto end_proc;
		}

		// 파티장에게 실패 에러를 보낸다.
		spHost = m_kPartyUserManager.GetHostPtr();
		if( spHost == NULL )
		{
			START_LOG( cerr, L"파티장 정보가 없습니다. 있을수 없는 에러!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_AUTO_PARTY_00;
			goto end_proc;
		}

		// 던전방을 만들수 있도록 정보를 수집한다.		
		kPacketStartGame.m_sWorldID								= 0;
		kPacketStartGame.m_kRoomInfo.m_RoomType					= CXSLRoom::RT_DUNGEON;
		kPacketStartGame.m_kRoomInfo.m_bPublic					= false;
		kPacketStartGame.m_kRoomInfo.m_MaxSlot					= static_cast<char>(m_kPartyUserManager.GetMaxMember());
		kPacketStartGame.m_kRoomInfo.m_iDungeonID				= GetDungeonID();
		kPacketStartGame.m_kRoomInfo.m_DifficultyLevel			= GetDungeonDif();	
		kPacketStartGame.m_kRoomInfo.m_cDungeonMode				= static_cast<char>(GetDungeonMode());	
		kPacketStartGame.m_kRoomInfo.m_iPartyUID				= GetUID();
		kPacketStartGame.m_kRoomInfo.m_bStartedByAutoParty		= true;		// 자동 파티로 던전 게임 시작!

		switch( GetItemType() )
		{
		case CXSLRoom::DGIT_PERSON:
		case CXSLRoom::DGIT_RANDOM:
			kPacketStartGame.m_kRoomInfo.m_cGetItemType = GetItemType();
			break;
		default:
			//설정이 잘못되어있으면 항상 랜덤으로.
			kPacketStartGame.m_kRoomInfo.m_cGetItemType = CXSLRoom::DGIT_RANDOM;
			break;
		}
		kPacketStartGame.m_kRoomInfo.m_fPlayTime = DUNGEON_DEFAULT_LIMIT_PLAY_TIME;

		float fPlayTimeLimit = SiCXSLDungeonManager()->GetPlayTimeLimit( ( kPacketStartGame.m_kRoomInfo.m_iDungeonID + kPacketStartGame.m_kRoomInfo.m_DifficultyLevel ) );
		if( fPlayTimeLimit > 0.0f )
		{
			kPacketStartGame.m_kRoomInfo.m_fPlayTime = fPlayTimeLimit;
		}

		// 파티장에게 던전방 만들으라고 패킷 전송
		spHost->SendToGSCharacter( EPM_PARTY_OPEN_DUNGEON_ROOM_NOT, kPacketStartGame );
	}

	// 모든 처리가 완료되었으니 ACK를 다시 보내자!
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket );

	//{{ 2009. 6. 10  최육사	퀵슬롯 쿨타임 리셋
	AllPartyMembersQuickSlotCoolTimeReset();
	//}}

	//게임중으로 설정하고
	SetPlaying( true );

	//파티리스트에서 빼준다.
	DelPartyInfoInPartyList();
	//////////////////////////////////////////////////////////////////////////

	return;

end_proc:
	SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket );

	// 파티 생성 실패! 파티를 닫습니다.
	CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
}

IMPL_ON_FUNC( EPM_REQUEST_REGROUP_PARTY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_INIT ) );

	// 새로운 파티 생성 및 모든 새로운 파티원들 입장 처리
	if( m_kPartyUserManager.Create( kPacket_.m_vecRegroupPartyUserInfo ) == false )
	{
		START_LOG( cerr, L"생성한 파티에 유저 리스트 입장실패.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_vecRegroupPartyUserInfo.size() )
			<< END_LOG;
		goto end_proc;
	}

	// 유저설정을 파티에 셋팅해주고
	SetInfo( kPacket_.m_kCreatePartyInfo );

	// 파티 피버 설정!
	UpdateAndBroadCastFeverPoint( kPacket_.m_sPartyFever );

	// 파티리스트에 생성된 파티 추가
	{
		KEPM_OPEN_PARTY_LIST_NOT kPacketNot;
		GetInfo( kPacketNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kPacketNot.m_vecUserInfo );
		BroadCastGameServer( EPM_OPEN_PARTY_LIST_NOT, kPacketNot );
	}

	//@@ 방리스트 추가
	//## 1. 공개일경우
	//## 2. 최대인원이 2명 이상일 경우.
	//## 3. 게임중인 상태가 아닌경우.
	//{{ 2011. 07. 04	최육사	대전 개편
	if( IsPublic() == true  &&
		m_kPartyUserManager.GetMaxMember() >= 2  &&
		IsPlaying() == false )
	{
		AddPartyInfoInPartyList();
	}

	//필드정보 업데이트.
	m_kPartyUserManager.UpdatePartyMemberListToFieldUser( GetUID() );

	// 파티 생성을 위한 기본 작업이 끝났으니 상태 이동을 하자!
	StateTransition( KCnPartyFSM::I_TO_MAKING_PARTY );
	StateTransition( KCnPartyFSM::I_TO_WAIT );

	//////////////////////////////////////////////////////////////////////////
	// 파티 가입이 완료되었으니 가입완료된 정보를 각 파티원들에게 알려주고
	{
		KEGS_REGROUP_PARTY_NOT kRegroupNot;
		GetInfo( kRegroupNot.m_kPartyInfo );
		m_kPartyUserManager.GetUserInfoList( kRegroupNot.m_vecPartyUserInfo );
		BroadCast( EPM_REGROUP_PARTY_NOT, kRegroupNot );
	}
	//////////////////////////////////////////////////////////////////////////

	{
		// 파티 피버를 한번 뿌리자!
		KEGS_PARTY_FEVER_POINT_NOT kNot;
		kNot.m_iPartyFever = GetFeverPoint();
		BroadCast( EPM_PARTY_FEVER_POINT_NOT, kNot );
	}

	{
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		// 파티장을 교체할 필요가 있다면 교체 하자!
		if( kPacket_.m_iOldHostUnitUID != m_kPartyUserManager.GetHostUID() )
		{
			KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( kPacket_.m_iOldHostUnitUID );
			if( IS_NOT_NULL( spNewHost ) )
			{
				KEPM_CHECK_PARTY_CHANGE_HOST_REQ kPacketReq;
				kPacketReq.m_iPartyUID		 = GetUID();
				//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
				kPacketReq.m_cPartyType		 = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
				//}}
				kPacketReq.m_iOldHostUnitUID = m_kPartyUserManager.GetHostUID();
				kPacketReq.m_iNewHostUnitUID = kPacket_.m_iOldHostUnitUID;
				kPacketReq.m_iDungeonID		 = GetDungeonID();
				kPacketReq.m_cDifficulty	 = GetDungeonDif();
				kPacketReq.m_cDungeonMode	 = GetDungeonMode();
				//{{ 2012. 11. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
				kPacketReq.m_bForRegroupParty = true;
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				spNewHost->SendToGSCharacter( EPM_CHECK_PARTY_CHANGE_HOST_REQ, kPacketReq );
			}
			else
			{
				START_LOG( cerr, L"파티유저 포인터값이 존재하지 않는다!" )
					<< BUILD_LOG( kPacket_.m_iOldHostUnitUID )
					<< END_LOG;
			}
		}
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}

	return;

end_proc:
	// 파티 생성 실패! 파티를 닫습니다.
	CloseParty( NetError::NOT_CLOSE_PARTY_REASON_00 );
}

IMPL_ON_FUNC_NOPARAM( ERM_END_DUNGEON_GAME_PARTY_FEVER_MODIFY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	if( FIRST_SENDER_UID != m_kPartyUserManager.GetHostUID() )
	{
		// 파티장이 아니면 아무것도 하지 않는다.
		return;
	}

	// 파티 피버 수정
	if( m_kFeverManager.GetRealFeverPoint() > KCnPartyFeverManager::FEVER_100_PERCENT_POINT )
	{
		UpdateAndBroadCastFeverPoint( KCnPartyFeverManager::FEVER_100_PERCENT_POINT );
	}
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	// 파티원 방 입장 정보 업데이트!
	UpdateJoinRoomInfoAndGetOnOffResult( FIRST_SENDER_UID, kPacket_.m_kJoinRoomInfo );
}

IMPL_ON_FUNC_NOPARAM( EPM_END_GAME_REGROUP_PARTY_NOT )
{
	VERIFY_PARTY_STATE( ( 1, KCnPartyFSM::S_WAIT ) );

	KEPM_REQUEST_REGROUP_PARTY_NOT kRegroupNot;
	std::set< UidType > setLeaveUnitUIDList;

	// 파티 재결합
	m_kPartyUserManager.GetUnitUIDListAboutEndGameRegroupParty( kRegroupNot.m_vecRegroupPartyUserInfo, setLeaveUnitUIDList );
	
	// 이제 이탈시켜야할 유저들을 이탈 시키자!
	BOOST_TEST_FOREACH( const UidType, iUnitUID, setLeaveUnitUIDList )
	{
		KCnPartyUserPtr spLeaveUser = m_kPartyUserManager.GetPartyUser( iUnitUID );
		if( IS_NULL( spLeaveUser ) )
		{	
			START_LOG( cerr, L"파티에서 이탈시킬 유저가 없습니다." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
			return;
		}

		// 해당 유저에게 파티 이탈 처리를 보내자!
		KEPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT kLeavePartyNot;
		kLeavePartyNot.m_iPartyUID = GetUID();
		spLeaveUser->SendToGSCharacter( EPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT, kLeavePartyNot );

		if( LeaveMember( iUnitUID
			#ifdef LOG_PARTY_BREAK
						, NetError::NOT_LEAVE_PARTY_REASON_08
			#endif // LOG_PARTY_BREAK
						) == false )
		{
			START_LOG( cerr, L"파티 재결합을 위한 파티원 이탈 처리 실패!" )
				<< BUILD_LOG( iUnitUID )
				<< END_LOG;
		}
	}

	// 새로운 파티 대상자가 존재한다면 재결합 하러 가자!
	BOOST_TEST_FOREACH( const KRegroupPartyUserInfo&, kInfo, kRegroupNot.m_vecRegroupPartyUserInfo )
	{
		kRegroupNot.m_kCreatePartyInfo.m_cPartyType		 = GetPartyType();
		kRegroupNot.m_kCreatePartyInfo.m_iDungeonID		 = GetDungeonID();
		kRegroupNot.m_kCreatePartyInfo.m_DifficultyLevel = GetDungeonDif();
		kRegroupNot.m_kCreatePartyInfo.m_cGetItemType	 = GetItemType();
		kRegroupNot.m_kCreatePartyInfo.m_cDungeonMode	 = GetDungeonMode();
		kRegroupNot.m_kCreatePartyInfo.m_bCheckLowLevel	 = GetCheckLowLevel();
		kRegroupNot.m_kCreatePartyInfo.m_cPVPGameType	 = m_cPvpGameType;
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		kRegroupNot.m_iOldHostUnitUID					 = kInfo.m_kOldPartyInfo.m_iOldHostUnitUID;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		kRegroupNot.m_sPartyFever						 = kInfo.m_kOldPartyInfo.m_sOldPartyFever;
		SendToGSCharacter( kInfo.m_kPartyUserInfo.m_iGSUID, kInfo.m_kPartyUserInfo.m_iUnitUID, EPM_PREPARE_FOR_REGROUP_PARTY_AND_CHECK_NEW_HOST_NOT, kRegroupNot );
		break;
	}

	// 파티에 아무도 없나.?
	if( m_kPartyUserManager.GetNumMember() <= PE_CLOSE_PARTY_NUM_MEMBER )
	{
		CloseParty( NetError::NOT_CLOSE_PARTY_REASON_01 );
	}
	else
	{
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		// 파티가 남은 경우 원래 파티장이 누구인지 확인후 파티장이 바뀔 필요가 있다면 바꾸자!
		UidType iNewHostUID = 0;
		if( m_kPartyUserManager.ChangeOldPartyHost( iNewHostUID ) == true )
		{
			KCnPartyUserPtr spNewHost = m_kPartyUserManager.GetPartyUser( iNewHostUID );
			if( IS_NOT_NULL( spNewHost ) )
			{
				KEPM_CHECK_PARTY_CHANGE_HOST_REQ kPacketReq;
				kPacketReq.m_iPartyUID		 = GetUID();
				//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
				kPacketReq.m_cPartyType		 = GetPartyType();
#endif SERV_PVP_NEW_SYSTEM
				//}}
				kPacketReq.m_iOldHostUnitUID = m_kPartyUserManager.GetHostUID();
				kPacketReq.m_iNewHostUnitUID = iNewHostUID;
				kPacketReq.m_iDungeonID		 = GetDungeonID();
				kPacketReq.m_cDifficulty	 = GetDungeonDif();
				kPacketReq.m_cDungeonMode	 = GetDungeonMode();
				//{{ 2012. 11. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
				kPacketReq.m_bForRegroupParty = true;
#endif SERV_BATTLE_FIELD_SYSTEM
				//}}
				spNewHost->SendToGSCharacter( EPM_CHECK_PARTY_CHANGE_HOST_REQ, kPacketReq );
			}
			else
			{
				START_LOG( cerr, L"파티유저 포인터값이 존재하지 않는다!" )
					<< BUILD_LOG( iNewHostUID )
					<< END_LOG;
			}
		}
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}

		// 파티가 남은 경우 원래 파티 피버로 되돌리자!
		const short sModifyPartyFever = m_kPartyUserManager.GetOldPartyFeverPoint();
		UpdateAndBroadCastFeverPoint( sModifyPartyFever );		
	}
}

_IMPL_ON_FUNC( EPM_UPDATE_GAME_PLAY_STATUS_NOT, KPartyMemberStatus )
{
	VERIFY_PARTY_STATE_WARN( ( 1, KCnPartyFSM::S_WAIT ) );

	if( m_kPartyUserManager.SetPartyMemberStatus( FIRST_SENDER_UID, kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 유저 플레이 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	KEGS_UPDATE_PARTY_MEMBER_STATUS_NOT kPacketNot;
	kPacketNot.m_iUnitUID = FIRST_SENDER_UID;
	kPacketNot.m_kStatus = kPacket_;
    BroadCast( EPM_BROADCAST_PARTY_MEMBER_STATUS_NOT, kPacketNot );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 30	최육사		공식 대전 버그 수정
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
IMPL_ON_FUNC( EPM_UPDATE_PVP_INFO_NOT )
{
	if( GetStateID() != KCnPartyFSM::S_WAIT )
	{
		// wait상태가 아니라면 무시!
		return;
	}

	KCnPartyUserPtr spPartyUser = m_kPartyUserManager.GetPartyUser( FIRST_SENDER_UID );
	if( spPartyUser == NULL )
	{
		START_LOG( cerr, L"존재하지 않는 파티 멤버입니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
		return;
	}

	// 변경된 대전 정보 업데이트!
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	spPartyUser->SetRank( kPacket_.m_cRank );
	spPartyUser->SetOfficialMatchCnt( kPacket_.m_iOfficialMatchCnt );
#endif SERV_2012_PVP_SEASON2
	//}}
	spPartyUser->SetRating( kPacket_.m_iRating );
	spPartyUser->SetRPoint( kPacket_.m_iRPoint );
}
#endif SERV_OFFCIAL_PVP_BUG_FIX
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}


