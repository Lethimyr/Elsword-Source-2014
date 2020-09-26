#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "Socket/NetCommon.h"

#include "BaseServer.h"

#include "Enum/Enum.h"
//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBuffManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


#define CLASS_TYPE      KGSUser


IMPL_ON_FUNC( EGS_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_AUTO_PARTY_DUNGEON_GAME_REQ, EGS_AUTO_PARTY_DUNGEON_GAME_ACK );

	if( GetRoomUID() != 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_BATTLE_FIELD )
	{
		START_LOG( cerr, L"자동 파티 신청을 할수 없는 곳에서 신청 요청을 하였습니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_01;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	// 재집결을 위한 휴식 버프가 걸려있으면 자동 파티 신청을 하지 못한다.
	if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY ) == true )
	{
		START_LOG( clog, L"재집결을 위한 휴식버프로 인해 자동 파티 신청을 할수 없다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_02;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}

	// 자동 매칭 등록을 할수 있는지 조건 체크
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"이미 자동 파티 대기 상태입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyWaitNumber() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_03;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-09-26	// 박세훈
	// 공식 대전 매칭 중인지 확인!!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"공식 대전 매칭 중인데 던전 자동 매칭 요청이 들어왔다!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_07;	// 공식 대전 신청 중에는 사용할 수 없는 기능입니다.
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// 룸 리스트 조회 중인지 확인!!
	if( GetRoomListID() != 0 )
	{
		START_LOG( cerr, L"룸 리스트 조회 중에 던전 자동 매칭 요청이 들어왔다!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_08;	// 룸 리스트 조회 중에는 사용할 수 없는 기능입니다.
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// DungeonID검증
	if( !SiCXSLDungeonManager()->VerifyDungeonID( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) ) )
	{
		START_LOG( cerr, L"존재하지않는 DungeonID입니다. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_04;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// HenirDungeonMode검증
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"비정상적인 HenirDungeonMode값입니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOG( (int)kPacket_.m_cDungeonMode )
			<< END_LOG;

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_04;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// 현재 파티 소속이냐 아니냐에 따라 다르게 처리한다.
	if( GetPartyUID() == 0 )
	{
		////////////////////////////////////////////////////////////////////////// 유저 조건
		if( CheckForDungeonGameStart( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, true, true ) == false )
		{
			KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::GetLastError();
			kPacketAck.m_wstrFailUserNickName = GetCharName();
			SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
			return;
		}

		// 파티에 소속되어 있지 않다면 KAutoPartyUserInfo 를 직접 GSUser로부터 얻어서 매칭 등록을 시도 하자!
		KEGB_REQUEST_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_kAutoPartyCondition.m_iDungeonID = kPacket_.m_iDungeonID;
		kPacketReq.m_kAutoPartyCondition.m_cDifficultyLevel = kPacket_.m_DifficultyLevel;
		kPacketReq.m_kAutoPartyCondition.m_cGetItemType = kPacket_.m_cGetItemType;
		kPacketReq.m_kAutoPartyCondition.m_cDungeonMode = kPacket_.m_cDungeonMode;
		KAutoPartyUserInfo kInfo;
		GetAutoPartyUserInfo( kInfo );
		kPacketReq.m_vecAutoPartyUser.push_back( kInfo );
		SendToAutoParty( EGB_REQUEST_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}
	else
	{
		// 파티에 소속 되어 있다면 직접 파티원들의 정보를 얻으러 가자!
		KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ kPacket;
		kPacket.m_iPartyUID = GetPartyUID();
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ, kPacket );
	}
}

IMPL_ON_FUNC( EPM_CHECK_FOR_AUTO_PARTY_MAKING_REQ )
{
	VERIFY_STATE_WARN( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 파티 소속인지 확인!
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 이런 패킷이 날아왔네?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
		return;
	}

	// 자동 파티를 시작할 조건을 만족하는지 체크하자!
	KEPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	// 재집결을 위한 휴식 버프가 걸려있으면 자동 파티 신청을 하지 못한다.
	if( m_kUserBuffManager.IsBuffActivated( CXSLBuffManager::BTI_DEBUFF_REST_OF_RELLY ) == true )
	{
		START_LOG( clog, L"재집결을 위한 휴식버프로 인해 자동 파티 신청을 할수 없다!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_02;
	}
#endif SERV_SERVER_BUFF_SYSTEM
	//}}

	// 해당 파티원이 던전에 입장할 조건을 만족하는지 체크!
	if( CheckForDungeonGameStart( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonID, kPacket_.m_cDifficulty, true, false ) == false )
	{	
		kPacket.m_iOK = NetError::GetLastError();
		kPacket.m_wstrFailUserNickName = GetCharName();
	}

	SendToCnParty( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK, kPacket );
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_AUTO_PARTY_DUNGEON_GAME_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iOK == NetError::ERR_DUNGEON_REQUIRE_ITEM_00 )
		{
			KEGS_ERROR_POPUP_NOT kNot;
			kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			//{{ 던전 입장권 에러 팝업 변경
#ifdef SERV_CHANGE_ERROR_POPUP
			kNot.m_bPartyMember = true;
#endif SERV_CHANGE_ERROR_POPUP
			//}}
			const int iDungeonID = kPacket_.m_kAutoPartyCondition.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyCondition.m_cDifficultyLevel);
			const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_kAutoPartyCondition.m_cDungeonMode);
			if( SiCXSLDungeonManager()->GetRequireItemIDAndCount( iDungeonID, eDungeonMode, kNot.m_iErrorData1, kNot.m_iErrorData2 ) == true )
			{
				SendPacket( EGS_ERROR_POPUP_NOT, kNot );
			}
			else
			{
				START_LOG( cerr, L"던전 입장권 정보를 찾지 못했다!" )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( iDungeonID )
					<< BUILD_LOG( eDungeonMode )
					<< END_LOG;
			}
		}
		//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		else if( kPacket_.m_iOK != NetError::ERR_PARTY_61 )
#else
		else
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
			//}}
		{
			START_LOG( cerr, L"자동 파티 매칭 정보 얻어오기 실패!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
				<< END_LOG;
		}

		KEGS_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		kPacketAck.m_wstrFailUserNickName = kPacket_.m_wstrFailUserNickName;
		SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// 파티 정보를 가져왔으니 다시 요청하러 가자!
	KEGB_REQUEST_AUTO_PARTY_MAKING_REQ kPacketReq;
	kPacketReq.m_bRemakingAutoParty = kPacket_.m_bRemakingAutoParty; // 재등록여부!
	kPacketReq.m_kAutoPartyCondition = kPacket_.m_kAutoPartyCondition;
	kPacketReq.m_vecAutoPartyUser = kPacket_.m_vecAutoPartyUser;
	SendToAutoParty( EGB_REQUEST_AUTO_PARTY_MAKING_REQ, kPacketReq );
}

_IMPL_ON_FUNC( EGB_REQUEST_AUTO_PARTY_MAKING_ACK, KEGS_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_AUTO_PARTY_DUNGEON_GAME_ACK );

	SendPacket( EGS_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT, KEGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0 )
	{
		START_LOG( cerr, L"이미 자동 파티 대기자인데 또 등록하는게 왔네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyWaitNumber() )
			<< BUILD_LOG( kPacket_.m_iWaitNumber )
			<< BUILD_LOG( kPacket_.m_iEstimatedTime )
			<< END_LOG;

		// 일단은 중복 적용을 피하기 위해 기존에 저장되어있던것은 취소 시키자!
		KEGB_CANCEL_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_iAutoPartyWaitNumber = m_kUserDungeonManager.GetAutoPartyWaitNumber();
		kPacketReq.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01;
		kPacketReq.m_bSendBySystem = true;
		SendToAutoParty( EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}

	// 대기표 번호를 저장하자!
	m_kUserDungeonManager.SetAutoPartyWaitNumber( kPacket_.m_iWaitNumber );

	SendPacket( EGS_REG_AUTO_PARTY_WAIT_LIST_SUCCESS_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_AUTO_PARTY_MAKING_SUCCESS_NOT, KEGS_AUTO_PARTY_MAKING_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 이미 등록되어 있다구?
	if( m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"이미 자동 파티 인데 또 자동파티가 됐네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;

		// 일단은 중복 적용을 피하기 위해 기존에 저장되어있던것은 취소 시키자!
		KEGB_CANCEL_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_iAutoPartyWaitNumber = m_kUserDungeonManager.GetAutoPartyUID();
		kPacketReq.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01;
		kPacketReq.m_bSendBySystem = true;
		SendToAutoParty( EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}

	// 자동 파티 UID값 저장!
	m_kUserDungeonManager.SetAutoPartyUID( kPacket_.m_iAutoPartyUID );

	// 비밀던전 이라면 예외
	if( CXSLDungeon::IsSecretDungeon( kPacket_.m_iDungeonID ) == true )
	{
		kPacket_.m_iDungeonID = ( (kPacket_.m_iDungeonID / 10) * 10 );
	}

	SendPacket( EGS_AUTO_PARTY_MAKING_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0 )
	{
		START_LOG( clog, L"현재 자동 파티에 등록된 상태가 아닌데 응답을 하려고 하네? 시간차 때문인가?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_bGameStartAgree )
			<< END_LOG;
		return;
	}

	SendToAutoParty( EGB_AUTO_PARTY_MAKING_SUCCESS_REPLY_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_CANCEL_AUTO_PARTY_MAKING_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_AUTO_PARTY_MAKING_REQ, EGS_CANCEL_AUTO_PARTY_MAKING_ACK );

	KEGS_CANCEL_AUTO_PARTY_MAKING_ACK kPacket;

	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() == 0  &&  m_kUserDungeonManager.GetAutoPartyUID() == 0 )
	{
		START_LOG( cerr, L"자동 파티를 신청한적이 없는데 취소요청이 왔네?" )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetRoomListID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_AUTO_PARTY_06;
		SendToMatch( EGS_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket );
		return;
	}

	KEGB_CANCEL_AUTO_PARTY_MAKING_REQ kPacketReq;
	kPacketReq.m_iAutoPartyUID = m_kUserDungeonManager.GetAutoPartyUID();
	kPacketReq.m_iAutoPartyWaitNumber = m_kUserDungeonManager.GetAutoPartyWaitNumber();
	kPacketReq.m_iReason = NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01;
	kPacketReq.m_bSendBySystem = false;	
	SendToAutoParty( EGB_CANCEL_AUTO_PARTY_MAKING_REQ, kPacketReq );
}

_IMPL_ON_FUNC( EGB_CANCEL_AUTO_PARTY_MAKING_ACK, KEGS_CANCEL_AUTO_PARTY_MAKING_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_AUTO_PARTY_MAKING_ACK );

	SendPacket( EGS_CANCEL_AUTO_PARTY_MAKING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_UNREG_AUTO_PARTY_WAIT_LIST_NOT, KEGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() == 0 )
	{
		START_LOG( cerr, L"원래부터 자동 파티 신청된 유저가 아닌데 왔다?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// 매치 신청 상태 해제
	m_kUserDungeonManager.SetAutoPartyWaitNumber( 0 );

	if( kPacket_.m_iReason != NetError::NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00 )
	{
		SendPacket( EGS_UNREG_AUTO_PARTY_WAIT_LIST_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( EGB_REMAKING_AUTO_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 자동 매칭 등록을 할수 있는지 조건 체크
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"이미 자동 파티 대기 상태입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyWaitNumber() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< END_LOG;
		return;
	}

	// DungeonID검증
	if( !SiCXSLDungeonManager()->VerifyDungeonID( kPacket_.m_kAutoPartyInfo.m_iDungeonID + static_cast<int>(kPacket_.m_kAutoPartyInfo.m_cDifficultyLevel) ) )
	{
		START_LOG( cerr, L"존재하지않는 DungeonID입니다. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_kAutoPartyInfo.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_kAutoPartyInfo.m_cDifficultyLevel )
			<< END_LOG;
		return;
	}

	// HenirDungeonMode검증
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_kAutoPartyInfo.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"비정상적인 HenirDungeonMode값입니다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_kAutoPartyInfo.m_iDungeonID )
			<< BUILD_LOG( (int)kPacket_.m_kAutoPartyInfo.m_cDungeonMode )
			<< END_LOG;
		return;
	}

	//::1. 근성도 검사.
	if( CheckingSpirit( kPacket_.m_kAutoPartyInfo.m_iDungeonID ) == false )
	{
		return;
	}

	// 자동 매칭 레벨 조건 체크!	


	// 자동 매칭 아이템 레벨 제한 조건 체크!


	// 현재 파티 소속이냐 아니냐에 따라 다르게 처리한다.
	if( GetPartyUID() == 0 )
	{
		// 파티에 소속되어 있지 않다면 KAutoPartyUserInfo 를 직접 GSUser로부터 얻어서 매칭 등록을 시도 하자!
		KEGB_REQUEST_AUTO_PARTY_MAKING_REQ kPacketReq;
		kPacketReq.m_bRemakingAutoParty = true; // 재등록!!
		kPacketReq.m_kAutoPartyCondition = kPacket_.m_kAutoPartyInfo;
		KAutoPartyUserInfo kInfo;
		GetAutoPartyUserInfo( kInfo );
		kPacketReq.m_vecAutoPartyUser.push_back( kInfo );
		SendToAutoParty( EGB_REQUEST_AUTO_PARTY_MAKING_REQ, kPacketReq );
	}
	else
	{
		// 파티에 소속 되어 있다면 직접 파티원들의 정보를 얻으러 가자!
		KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ kPacket;
		kPacket.m_bRemakingAutoParty = true; // 재등록!!
		kPacket.m_iPartyUID = GetPartyUID();
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ, kPacket );
	}
}

_IMPL_ON_FUNC( EGB_AUTO_PARTY_CLOSE_NOT, KEGS_AUTO_PARTY_CLOSE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0 )
	{
		START_LOG( cerr, L"원래부터 자동 파티된 유저가 아닌데 왔다?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// 자동 파티 UID업데이트
	m_kUserDungeonManager.SetAutoPartyUID( 0 );


	// 클라이언트에 알리자!
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_AUTO_PARTY_REASON_00 )
	{
		SendPacket( EGS_AUTO_PARTY_CLOSE_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( EGB_GET_REGROUP_PARTY_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != kPacket_.m_iAutoPartyUID )
	{
		START_LOG( cerr, L"현재 자동 파티에 등록된 상태가 아닌데 파티 재배치에 대한 응답을 하려고 한다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;
		return;
	}

	// 현재 파티에 소속되어 있거나 파티 초대 대기자라면 자동으로 이탈 시킨다!
	if( GetPartyUID() > 0  ||  GetCandidatePartyUID() > 0 )
	{
		// 만약 기존에 파티에 소속되어있었다면 강제 이탈 시킨다!
		SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_06 );

		// 파티 대기자UID도 리셋
		SetCandidatePartyUID( 0 );

		//파티 UID를 리셋하고.
		SetPartyUID( 0 );
	}

	// 수집된 정보를 다시 가져간다.
	KEGB_GET_REGROUP_PARTY_INFO_ACK kPacket;
	GetPartyUserInfo( kPacket.m_kPartyUserInfo );

	// 현재 게임 정보 저장
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kDungeonGameInfo );

	GetRoomUserInfo( kPacket.m_kPartyRoomUserInfo.m_kRoomUserInfo, CXSLRoom::RT_DUNGEON );
	GetTutorUnitUIDList( kPacket.m_kPartyRoomUserInfo.m_vecStudentUnitUID );
	kPacket.m_kPartyRoomUserInfo.m_iCurEXP = GetEXP();
	kPacket.m_kPartyRoomUserInfo.m_iCurED = GetED();
	kPacket.m_kPartyRoomUserInfo.m_wstrChannelIP = NetCommon::GetLocalIPW();
	SendToAutoParty( EGB_GET_REGROUP_PARTY_INFO_ACK, kPacket );	
}

_IMPL_ON_FUNC( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != kPacket_.m_iAutoPartyUID )
	{
		START_LOG( cerr, L"현재 자동 파티에 등록된 상태가 아닌데 파티 재배치에 대한 응답을 하려고 한다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;

		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_06;
		kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
		SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	if( GetPartyUID() > 0  ||  GetCandidatePartyUID() > 0 )
	{
		START_LOG( cerr, L"지금 시점에는 파티 소속이 되면 안된다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;

		KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_06;
		kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
		SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
		return;
	}

	// 만약 파티원이 한명이라면 굳이 파티를 만들어서 게임을 시작할 필요가 없다!
	if( kPacket_.m_vecRegroupPartyUserInfo.size() == 1 )
	{
		// 마을에서 던전 게임 시작!
		if( GetRoomUID() == 0 )
		{
			// 새로 방을 만들면서 던전 게임을 시작하자!
			DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_kCreatePartyInfo.m_cDungeonMode, kPacket_.m_kCreatePartyInfo.m_iDungeonID, kPacket_.m_kCreatePartyInfo.m_DifficultyLevel, true );

			// 싱글 게임 시작 처리되고 나면 바로 AutoParty를 닫으러 가자!
			KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::NET_OK;
			kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
			SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
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

					if( GetStateID() == KGSFSM::S_ROOM )
					{
						StateTransition( KGSFSM::I_TO_FIELD_MAP );
					}

					// 새로 방을 만들면서 던전 게임을 시작하자!
					DungeonGameStart_SingleOpenDungeonRoom( kPacket_.m_kCreatePartyInfo.m_cDungeonMode, kPacket_.m_kCreatePartyInfo.m_iDungeonID, kPacket_.m_kCreatePartyInfo.m_DifficultyLevel, true );

					// 싱글 게임 시작 처리되고 나면 바로 AutoParty를 닫으러 가자!
					KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
					kPacketAck.m_iOK = NetError::NET_OK;
					kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
					SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
				}
				break;

			default:
				{
					START_LOG( cerr, L"잘못된 상태값입니다!" )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK kPacketAck;
					kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_00;
					kPacketAck.m_iAutoPartyUID = kPacket_.m_iAutoPartyUID;
					SendToAutoParty( EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacketAck );
					return;
				}
				break;
			}
		}
	}
	else
	{
		// 이 패킷을 받은 유저는 새로운 파티의 파티장이 될것입니다.
		SendToCnParty( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, kPacket_ );
	}
}

_IMPL_ON_FUNC( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() == 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != kPacket_.m_iAutoPartyUID )
	{
		START_LOG( cerr, L"현재 자동 파티에 등록된 상태가 아닌데 파티 재배치에 대한 응답을 하려고 한다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< BUILD_LOG( kPacket_.m_iAutoPartyUID )
			<< END_LOG;
	}

	_SendToAutoParty( kPacket_.m_iAutoPartyUID, EGB_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_REGROUP_PARTY_NOT, KEGS_REGROUP_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() != 0  &&  GetPartyUID() != kPacket_.m_kPartyInfo.m_iPartyUID )
	{
		START_LOG( cerr, L"다른 파티 소속인데 이 패킷을 이 시점에 받다니.. 뭔가 문제가 있다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iPartyUID )
			<< END_LOG;
	}

	if( GetCandidatePartyUID() != 0  &&  GetCandidatePartyUID() != kPacket_.m_kPartyInfo.m_iPartyUID )
	{
		START_LOG( cerr, L"다른 파티에 가입 신청 상태인데 이 패킷을 이 시점에 받다니.. 뭔가 문제가 있다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetCandidatePartyUID() )
			<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iPartyUID )
			<< END_LOG;
	}

	// 파티 정보 업데이트
	SetPartyUID( kPacket_.m_kPartyInfo.m_iPartyUID );

	//{{ 2012. 04. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 파티의 던전값을 저장하자!
	m_kUserDungeonManager.SetDungeonGameInfo( kPacket_.m_kPartyInfo.m_iDungeonID, kPacket_.m_kPartyInfo.m_DifficultyLevel, kPacket_.m_kPartyInfo.m_cDungeonMode );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 02. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 만약 배틀필드에서 파티 가입을 했다면 배틀필드에도 파티 정보 갱신을 해주자!
	if( GetRoomUID() > 0  &&  CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
	{
		// 기존 데이터 초기화!
		KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT kPacketToRM;
		kPacketToRM.m_iPartyUID = 0;
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );

		// 새로운 파티로 세팅
		kPacketToRM.m_iPartyUID = GetPartyUID();
		BOOST_TEST_FOREACH( const KPartyUserInfo&, kInfo, kPacket_.m_vecPartyUserInfo )
		{
			kPacketToRM.m_vecPartyUnitUIDList.push_back( kInfo.m_iUnitUID );
		}		
		SendToCnRoom( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, kPacketToRM );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	SendPacket( EGS_REGROUP_PARTY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

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
	SendID( EGS_PARTY_BAN_USER_NOT );

	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_PREPARE_FOR_REGROUP_PARTY_AND_CHECK_NEW_HOST_NOT, KEPM_REQUEST_REGROUP_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"현재 자동 파티에 등록된 상태인데 파티 재결합을 시도하려고 한다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( m_kUserDungeonManager.GetAutoPartyUID() )
			<< END_LOG;
		return;
	}

	if( GetPartyUID() > 0  ||  GetCandidatePartyUID() > 0 )
	{
		START_LOG( cerr, L"지금 시점에는 파티 소속이 되면 안된다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( GetCandidatePartyUID() )
			<< END_LOG;
		return;
	}

	SendToCnParty( EPM_REQUEST_REGROUP_PARTY_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_REQUEST_GET_AUTO_PARTY_BONUS_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 자동 파티 보너스 정보를 요청할 던전 리스트 저장!
	KEGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT kPacket;

	if( kPacket_.m_iVillageMapID != 0 )
	{
		m_kUserDungeonManager.GetAutoPartyBonusDungeonList( kPacket_.m_iVillageMapID, kPacket.m_mapDungeonBonusList );
	}

	if( kPacket_.m_vecDungeonList.empty() == false )
	{
		m_kUserDungeonManager.GetAutoPartyBonusDungeonList( kPacket_.m_vecDungeonList, kPacket.m_mapDungeonBonusList );
	}
	
	if( kPacket.m_mapDungeonBonusList.empty() == false )
	{
		SendPacket( EGS_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, kPacket );
	}
}

//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
_IMPL_ON_FUNC( EGB_AUTO_PARTY_WAIT_USER_COUNT_NOT, KEGS_AUTO_PARTY_WAIT_USER_COUNT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_AUTO_PARTY_WAIT_USER_COUNT_NOT, kPacket_ );
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
//}}

