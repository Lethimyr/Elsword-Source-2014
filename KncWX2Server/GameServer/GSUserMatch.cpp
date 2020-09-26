#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "UserManager.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "Enum/Enum.h"
#include "RoomListManager.h"

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser


//{{ 2011. 06. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
IMPL_ON_FUNC( EGS_CHANGE_PARTY_TYPE_REQ )
{
	//{{ 2012. 04. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_PARTY_TYPE_REQ, EGS_CHANGE_PARTY_TYPE_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_PARTY_TYPE_REQ, EGS_CHANGE_PARTY_TYPE_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrPartyName = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrPartyName.c_str(), L'♡' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CHANGE_PARTY_TYPE_ACK kAck;

	switch( kPacket_.m_cPartyType )
	{
	case KPartyInfo::PT_DUNGEON_PARTY:
		{
			if( CheckDungeonPartyCondition( kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, kPacket_.m_cDungeonMode, kPacket_.m_bCheckLowLevel, true ) == false )
			{
				kAck.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}
		break;

	case KPartyInfo::PT_PVP_PARTY:
		{
			if( CheckPvpPartyCondition( kPacket_.m_cPVPGameType, true ) == false )
			{
				kAck.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}
		break;

	default:
		{
			kAck.m_iOK = NetError::ERR_PARTY_00;
			goto end_proc;
		}
		break;
	}

	if( SiKPartyListManager()->E_ChangePartyType( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 타입 변경 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"파티 타입 변경 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
		<< END_LOG;

	SendPacket( EGS_CHANGE_PARTY_TYPE_ACK, kAck );
}

_IMPL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_ACK, KEGS_CHANGE_PARTY_TYPE_ACK )
{
	//{{ 2012. 04. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_PARTY_TYPE_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_PARTY_TYPE_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_CHANGE_PARTY_TYPE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHANGE_PARTY_TYPE_NOT, KEGS_CHANGE_PARTY_TYPE_NOT )
{
	//{{ 2012. 04. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

    SendPacket( EGS_CHANGE_PARTY_TYPE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ )
{
	//{{ 2012. 04. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ, EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_REQ, EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( GetPartyUID() == 0 )
	{
		START_LOG( clog, L"이건 정상적인 상황임." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK kAck;
		kAck.m_iOK = NetError::NET_OK;
		SendPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kAck );
		return;
	}

	if( SiKPartyListManager()->E_ChangePvpMatchInfo( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 대전 매치 정보 변경 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_ACK, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK )
{
	//{{ 2012. 04. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );	
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK );	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

    SendPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PVP_PARTY_CHANGE_MATCH_INFO_NOT, KEGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT )
{
	//{{ 2012. 04. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );	
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_PVP_PARTY_CHANGE_MATCH_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_REQUEST_MATCH_MAKING_REQ )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REQUEST_MATCH_MAKING_REQ, EGS_REQUEST_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REQUEST_MATCH_MAKING_REQ, EGS_REQUEST_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	KEGS_REQUEST_MATCH_MAKING_ACK kPacketAck;

	// 이미 매치에 등록된 상태인지 확인한다!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"이미 매치에 등록된 상태입니다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< END_LOG;
		
		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_00;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

#ifdef SERV_FIX_JOIN_OFFICIAL_PVP_ROOM// 작업날짜: 2013-09-26	// 박세훈
	// 던전 자동 매칭 중인지 확인!!
	if( m_kUserDungeonManager.GetAutoPartyWaitNumber() != 0  ||  m_kUserDungeonManager.GetAutoPartyUID() != 0 )
	{
		START_LOG( cerr, L"던전 자동 매칭 중인데 공식 대전 매칭 요청이 들어왔다!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_23;	// 던전 게임 신청 중에는 사용할 수 없는 기능입니다.
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

	// 룸 리스트 조회 중인지 확인!!
	if( GetRoomListID() != 0 )
	{
		START_LOG( cerr, L"룸 리스트 조회 중에 공식 대전 매칭 요청이 들어왔다!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_AUTO_PARTY_08;	// 룸 리스트 조회 중에는 사용할 수 없는 기능입니다.
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}
#endif // SERV_FIX_JOIN_OFFICIAL_PVP_ROOM

	// 클라이언트가 보낸 정보를 검증한다.
	switch( kPacket_.m_cPvpPlayerCount )
	{
	case 1:
	case 2:
	case 3:
		break;

	default:
		{
			START_LOG( cerr, L"이상한 대전 인원 신청입니다." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOGc( kPacket_.m_cPvpPlayerCount )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_01;
			SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		}
		return;
	}

	if( kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM  &&
		kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM_DEATH )
	{
		START_LOG( cerr, L"이상한 대전 타입 입니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( kPacket_.m_cPvpGameType )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_02;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

	// 공식 대전은 최소레벨이 10레벨이 되어야 한다.
	if( GetLevel() < SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL )
	{
		START_LOG( cerr, L"공식 대전을 하기에 부족한 캐릭터 레벨입니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( GetLevel() )
			<< END_LOG;
		
		kPacketAck.m_iOK = NetError::ERR_MATCH_MAKING_03;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}
    
	// 파티가 있으면 파티원 정보 다 같이 가지고 가자!
	if( GetPartyUID() == 0 )
	{
		KEGB_REQUEST_MATCH_MAKING_REQ kPacket;
		KMatchUserInfo kInfo;
		GetMatchUserInfo( kInfo );
		kPacket.m_bRegFrontOfList = false;
		kPacket.m_vecMatchUserInfo.push_back( kInfo );
		kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
		SendToMatch( EGB_REQUEST_MATCH_MAKING_REQ, kPacket );
	}
	else
	{
		KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ kPacketReq;
		kPacketReq.m_cPvpGameType = kPacket_.m_cPvpGameType;
		kPacketReq.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ, kPacketReq );
	}
}

IMPL_ON_FUNC( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REQUEST_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REQUEST_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_REQUEST_MATCH_MAKING_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacketAck );
		return;
	}

	KEGB_REQUEST_MATCH_MAKING_REQ kPacket;
	kPacket.m_bRegFrontOfList = kPacket_.m_bRegFrontOfList;
	kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
	kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
	kPacket.m_vecMatchUserInfo = kPacket_.m_vecMatchUserInfo;
	SendToMatch( EGB_REQUEST_MATCH_MAKING_REQ, kPacket );
}

_IMPL_ON_FUNC( EGB_REQUEST_MATCH_MAKING_ACK, KEGS_REQUEST_MATCH_MAKING_ACK )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_REQUEST_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REQUEST_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	SendPacket( EGS_REQUEST_MATCH_MAKING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_REG_MATCH_WAIT_LIST_SUCCESS_NOT, KEGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserPvpManager.IsRequestMatchMaking() == true )
	{
		START_LOG( cerr, L"이미 대전 매치 리스트에 등록되어있는데 또 등록하는게 왔네? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iEstimatedTime )
			<< END_LOG;
	}

	// 매치 메이킹 신청으로 업데이트
	m_kUserPvpManager.SetFlagRequestMatchMaking( true );

	SendPacket( EGS_REG_MATCH_WAIT_LIST_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_CANCEL_MATCH_MAKING_REQ )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_MATCH_MAKING_REQ, EGS_CANCEL_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CANCEL_MATCH_MAKING_REQ, EGS_CANCEL_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	KEGS_CANCEL_MATCH_MAKING_ACK kPacket;

	if( m_kUserPvpManager.IsRequestMatchMaking() == false  &&  GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"대전 매칭을 신청한적이 없는데 취소요청이 왔네?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetRoomListID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_12;
        SendToMatch( EGS_CANCEL_MATCH_MAKING_ACK, kPacket );
		return;
	}

	KEGB_CANCEL_MATCH_MAKING_REQ kPacketReq;
	kPacketReq.m_iMatchUID = GetMatchUID();
	kPacketReq.m_iPartyUID = GetPartyUID();
	kPacketReq.m_iReason = NetError::NOT_LEAVE_MATCH_WAIT_REASON_01;
	kPacketReq.m_bSendBySystem = false;	
    SendToMatch( EGB_CANCEL_MATCH_MAKING_REQ, kPacketReq );
}

_IMPL_ON_FUNC( EGB_CANCEL_MATCH_MAKING_ACK, KEGS_CANCEL_MATCH_MAKING_ACK )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CANCEL_MATCH_MAKING_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CANCEL_MATCH_MAKING_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
	}

	SendPacket( EGS_CANCEL_MATCH_MAKING_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EGB_MATCH_MAKING_SUCCESS_NOT, KEGS_MATCH_MAKING_SUCCESS_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 매치 성공
	m_kUserPvpManager.SetMatchUID( kPacket_.m_iMatchUID );

	SendPacket( EGS_MATCH_MAKING_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_MATCH_MAKING_SUCCESS_REPLY_NOT )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"현재 매치에 등록된 상태가 아닌데 매치 성공에 대한 응답을 하려고 한다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_bGameStartAgree )
			<< END_LOG;
		return;
	}

	// 선택한 비선호 대전맵이 2개 이상인지 체크한다.
	if( kPacket_.m_setSelectedPvpMap.size() > 2 )
	{
        START_LOG( cerr, L"유저가 선택한 비선호 대전맵이 2개 이상이다. 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_setSelectedPvpMap.size() )
			<< END_LOG;

		// 여기서 끊자!
		return;
	}

	SendToMatch( EGB_MATCH_MAKING_SUCCESS_REPLY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EGB_UNREG_MATCH_WAIT_LIST_NOT, KEGS_UNREG_MATCH_WAIT_LIST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( m_kUserPvpManager.IsRequestMatchMaking() == false )
	{
		START_LOG( cerr, L"매치 리스트 등록한적이 없는데 등록취소가 왔네?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// 매치 신청 상태 해제
	m_kUserPvpManager.SetFlagRequestMatchMaking( false );

	if( kPacket_.m_iReason != NetError::NOT_LEAVE_MATCH_WAIT_REASON_00 )
	{
		SendPacket( EGS_UNREG_MATCH_WAIT_LIST_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( EGB_MATCH_CLOSE_NOT, KEGS_MATCH_CLOSE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"원래부터 매치된 유저가 아닌데 매치 취소가 왔다?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;
	}

	// 매치 UID업데이트
	m_kUserPvpManager.SetMatchUID( 0 );


	// 클라이언트에 알리자!
	if( kPacket_.m_iReason != NetError::NOT_LEAVE_MATCH_REASON_00 )
	{
		SendPacket( EGS_MATCH_CLOSE_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( EGB_REMATCH_MAKING_USER_NOT )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	// 이미 매치에 등록된 상태인지 확인한다!
	if( GetMatchUID() != 0 )
	{
		START_LOG( cerr, L"이미 매치에 등록된 상태입니다! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< END_LOG;
		return;
	}

	// 클라이언트가 보낸 정보를 검증한다.
	switch( kPacket_.m_cPvpPlayerCount )
	{
	case 1:
	case 2:
	case 3:
		break;

	default:
		{
			START_LOG( cerr, L"이상한 대전 인원 신청입니다." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetMatchUID() )
				<< BUILD_LOGc( kPacket_.m_cPvpPlayerCount )
				<< END_LOG;
		}
		return;
	}

	if( kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM  &&
		kPacket_.m_cPvpGameType != CXSLRoom::PGT_TEAM_DEATH )
	{
		START_LOG( cerr, L"이상한 대전 타입 입니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( kPacket_.m_cPvpGameType )
			<< END_LOG;
		return;
	}

	// 공식 대전은 최소레벨이 10레벨이 되어야 한다.
	if( GetLevel() < SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL )
	{
		START_LOG( cerr, L"공식 대전을 하기에 부족한 캐릭터 레벨입니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetMatchUID() )
			<< BUILD_LOGc( GetLevel() )
			<< END_LOG;
		return;
	}

	// 파티가 있으면 파티원 정보 다 같이 가지고 가자!
	if( GetPartyUID() == 0 )
	{
		KEGB_REQUEST_MATCH_MAKING_REQ kPacket;
		KMatchUserInfo kInfo;
		GetMatchUserInfo( kInfo );
		kPacket.m_bRegFrontOfList = true; // 대전 매칭 재신청에 따라 리스트 최상위에 등록!
		kPacket.m_vecMatchUserInfo.push_back( kInfo );
		kPacket.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		kPacket.m_cPvpGameType = kPacket_.m_cPvpGameType;
		SendToMatch( EGB_REQUEST_MATCH_MAKING_REQ, kPacket );
	}
	else
	{
		KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ kPacketReq;
		kPacketReq.m_bRegFrontOfList = true; // 대전 매칭 재신청에 따라 리스트 최상위에 등록!
		kPacketReq.m_cPvpGameType = kPacket_.m_cPvpGameType;
		kPacketReq.m_cPvpPlayerCount = kPacket_.m_cPvpPlayerCount;
		SendToCnParty( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ, kPacketReq );
	}    
}

IMPL_ON_FUNC_NOPARAM( EGB_GET_ROOM_USER_INFO_FOR_MATCH_REQ )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( GetMatchUID() == 0 )
	{
		START_LOG( cerr, L"현재 매치에 등록된 상태가 아닌데 매치 성공에 대한 응답을 하려고 한다!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	// 이 패킷이 오면 오피셜 매치 RoomListID를 등록하자!
	SetRoomListID( SiKRoomListManager()->GetPvpOffcialRoomListID() );


    KEGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK kPacket;
	GetRoomUserInfo( kPacket.m_kRoomUserInfo, CXSLRoom::RT_PVP ); // 채널 클래스 설정하는게 문제로군..
	SendToMatch( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK, kPacket );
}

IMPL_ON_FUNC( EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_LEAVE_ROOM_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_32;
			SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, kPacket );

			// 이탈처리 했으니 여기서 쫑!
			SetRoomUID( 0 );

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				StateTransition( KGSFSM::I_TO_FIELD_MAP );
			}
		}		
		else
		{
			START_LOG( cerr, L"이미 방에 입장해있다! 예외 상황이 존재하는것인가?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetRoomUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;

			KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_MATCH_MAKING_13;
			SendToMatch( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket );
			return;
		}
	}
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( GetRoomUID() != 0 )
	{
		START_LOG( cerr, L"이미 방에 입장해있다! 예외 상황이 존재하는것인가?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;

		KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_13;
		SendToMatch( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket );
		return;
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	// 대전방을 생성하러 가자!
	kPacket_.m_kRoomInfo.m_uiRoomListID = SiKRoomListManager()->GetPvpOffcialRoomListID();
	SendToCnRoom( ERM_OPEN_PVP_ROOM_FOR_MATCH_REQ, kPacket_ );
}

//{{ 2012. 06. 05	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_ACK, KEGS_LEAVE_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

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
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

_IMPL_ON_FUNC( ERM_OPEN_PVP_ROOM_FOR_MATCH_ACK, KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대전방 생성 실패!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	// 응답 보내기!
	SendToMatch( EGB_OPEN_PVP_ROOM_FOR_MATCH_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ERM_GAME_START_PVP_MATCH_NOT, KEGS_GAME_START_PVP_MATCH_NOT )
{
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetRoomUID() != 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_BATTLE_FIELD )
		{
			KERM_LEAVE_ROOM_REQ kPacket;
			kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_32;
			SendToCnRoom( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, kPacket );

			// 이탈처리 했으니 여기서 쫑!
			SetRoomUID( 0 );

			if( GetStateID() == KGSFSM::S_ROOM )
			{
				StateTransition( KGSFSM::I_TO_FIELD_MAP );
			}
		}
	}
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	// 방에 입장 처리
	SetRoomUID( kPacket_.m_RoomInfo.m_RoomUID );

	// 방 상태로 이동
	StateTransition( KGSFSM::I_TO_ROOM );
	m_kTimer[TM_ROOM].restart();

	// 필드에서 이탈
	SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

	//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

#ifdef SERV_PVP_REMATCH
	SetRoomListID( SiKRoomListManager()->GetPvpOffcialRoomListID() ); // 2013.05.13 lygan_조성욱 // 대전 개편 관련 Rematch 문제 수정
#endif SERV_PVP_REMATCH

	SendPacket( EGS_GAME_START_PVP_MATCH_NOT, kPacket_ );

	// 서버에 대전 정보 업데이트
	KERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT kPacketNot;
	kPacketNot.m_kPvpUnitInfo.m_iRPoint = GetRPoint();
	kPacketNot.m_kPvpUnitInfo.m_iAPoint = GetAPoint();
	kPacketNot.m_kPvpUnitInfo.m_iWinCount = GetWin();
	kPacketNot.m_kPvpUnitInfo.m_bIsPlayWithParty = ( GetPartyUID() != 0 );
	SendToCnRoom( ERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT, kPacketNot );
}

IMPL_ON_FUNC( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_REQ )
{
	// 여러번 중복으로 올수 있기 때문에 중복 필터 걸지 않는다.
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );
    
	if( GetRoomUID() == 0 )
	{
		START_LOG( cerr, L"방에 없는데 대전 NPC부활패킷을 쏘네?" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_14;
		SendPacket( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, kPacket );
		return;
	}

	SendToCnRoom( ERM_PVP_NPC_UNIT_RE_BIRTH_POS_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, KEGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PVP_NPC_UNIT_RE_BIRTH_POS_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_ROOM ), EGS_PVP_NPC_UNIT_INFO_TO_SERVER_REQ, EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK );

	SendToCnRoom( ERM_PVP_NPC_UNIT_INFO_TO_SERVER_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ERM_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, KEGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_ROOM ), EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK );

	SendPacket( EGS_PVP_NPC_UNIT_INFO_TO_SERVER_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGB_CHECK_INVALID_MATCH_NOT )
{
	if( IsRequestMatchMaking() == true )
	{
		// 매치 신청 상태 해제
		m_kUserPvpManager.SetFlagRequestMatchMaking( false );

		KEGS_UNREG_MATCH_WAIT_LIST_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_WAIT_REASON_01;
		SendPacket( EGS_UNREG_MATCH_WAIT_LIST_NOT, kPacketNot );
	}
	
	if( GetMatchUID() != 0 )
	{
		// 매치 UID업데이트
		m_kUserPvpManager.SetMatchUID( 0 );

		// 클라이언트에 알리자!
		KEGS_MATCH_CLOSE_NOT kPacketNot;
		kPacketNot.m_iReason = NetError::NOT_LEAVE_MATCH_REASON_02;
		SendPacket( EGS_MATCH_CLOSE_NOT, kPacketNot );
	}
}

#endif SERV_PVP_NEW_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


