#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( kPacket_.m_iUnitUID > 0 )
	{
		if( IsSameServerGroupUID( kPacket_.m_iUnitUID ) == false )
		{
			KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_04;
			SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket );
			return;
		}
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUnitNickName ) )
	{
		KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket );
		return;
	}

#ifdef SERV_STRING_CHECK_IGNORE_CASE
	if( boost::iequals(GetCharName(),kPacket_.m_wstrUnitNickName) )
#else
	if( GetCharName().compare( kPacket_.m_wstrUnitNickName ) == 0 )
#endif SERV_STRING_CHECK_IGNORE_CASE
	{
		KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_KNM_00;
		SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket );
		return;
	}	

	if( m_kTimer[TM_KNM_NEW_FRIEND_REFRESH].elapsed() < (float)KNM_NEW_FRIEND_REFRESH_TIME_LIMIT )
	{
		KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK kPacket;
		kPacket.m_iOK	= NetError::ERR_KNM_01;
		SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket );

		m_kTimer[TM_KNM_NEW_FRIEND_REFRESH].restart();
		return;
	}

	SendToLoginServer( ELG_KNM_REQUEST_NEW_FRIEND_INFO_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ELG_KNM_REQUEST_NEW_FRIEND_INFO_ACK, KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 체험ID 기능 제한
	if( kPacket_.m_iOK == NetError::ERR_GUEST_USER_03 )
	{
		SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket_ );
		return;
	}

	//찾는 캐릭이 있을경우
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket_ );
	}
	//못찾았다고 오는경우
	else
	{
		SendToGameDB( DBE_KNM_REQUEST_NEW_FRIEND_INFO_REQ, kPacket_.m_wstrUnitNickName );
	}
}

_IMPL_ON_FUNC( DBE_KNM_REQUEST_NEW_FRIEND_INFO_ACK, KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_KNM_INVITE_REQ )
{
	//VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	VERIFY_STATE( (1, KGSFSM::S_ROOM) );

	// 메신져를 이용한 초청
	// : 대전방안에서만 초청이 가능해야 함.

	if( GetRoomUID() != 0 )
	{
		//메신져를 통한 초청이므로 자신이 방안이어야 한다.
		KELG_KNM_INVITE_REQ kPacket;

		kPacket.m_wstrInviteUnitNickName	= kPacket_;
		kPacket.m_wstrMyNickName			= GetCharName();
		kPacket.m_kMyUnitInfo.m_iUnitUID	= GetCharUID();

		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_PVP )
		{
			kPacket.m_kMyUnitInfo.m_iState		= GetUnitState();
			kPacket.m_kMyUnitInfo.m_iDungeonID	= 0;
			kPacket.m_kMyUnitInfo.m_iRoomUID	= GetRoomUID();

			SendToLoginServer( ELG_KNM_INVITE_REQ, kPacket );
			return;
		}
		else
		{
			SET_ERROR( ERR_KNM_03 );
			goto err_proc;
		}
	}
	else
	{
		SET_ERROR( ERR_KNM_03 );
		goto err_proc;
	}

err_proc:
	KEGS_KNM_INVITE_ACK kPacket;
	kPacket.m_iOK	= NetError::GetLastError();

	//단지 같이 하지 못하는 상황이므로 에러가 아님.
	START_LOG( cwarn, L"메신저를 통한 같이하기 실패.!" )
		<< BUILD_LOG( NetError::GetErrStr(kPacket.m_iOK) )
		<< END_LOG;

	SendPacket( EGS_KNM_INVITE_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_KNM_INVITE_ACK, KEGS_KNM_INVITE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_KNM_INVITE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_KNM_INVITE_NOT, KEGS_KNM_INVITE_NOT )
{
	//VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_KNM_INVITE_RESULT_NOT kPacketRet;

	//{{ 2009. 3. 24  최육사	GM제한
	if( GetAuthLevel() == SEnum::UAL_GM )
	{
		kPacketRet.m_iOK = NetError::ERR_GM_CHAR_DENY_00;
		_SendToCharacter( kPacket_.m_kUnitInfo.m_iUnitUID, EGS_KNM_INVITE_RESULT_NOT, kPacketRet );
		return;
	}
	//}}

	//{{ 2008. 12. 2  최육사	메신저 유저 초대 
	// 초대할 수 없는 상태
	if( GetStateID() != KGSFSM::S_FIELD_MAP )
	{
		kPacketRet.m_iOK = NetError::ERR_ROOM_44;
		_SendToCharacter( kPacket_.m_kUnitInfo.m_iUnitUID, EGS_KNM_INVITE_RESULT_NOT, kPacketRet );
		return;
	}

	// 대전 채널 로비에 있는 유저만 초대 가능
	if( GetRoomListID() == 0 )
	{
		kPacketRet.m_iOK = NetError::ERR_ROOM_45;
		_SendToCharacter( kPacket_.m_kUnitInfo.m_iUnitUID, EGS_KNM_INVITE_RESULT_NOT, kPacketRet );
		return;
	}
	//}}

	// 초대 메시지 전송 성공!
	SendPacket( EGS_KNM_INVITE_NOT, kPacket_ );
}

//{{ 2008. 12. 2  최육사	메신저 유저 초대
IMPL_ON_FUNC( EGS_KNM_INVITE_RESULT_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_KNM_INVITE_RESULT_NOT, kPacket_ );
}
//}}

IMPL_ON_FUNC( EGS_KNM_TOGETHER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendToLoginServer( ELG_KNM_TOGETHER_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ELG_KNM_TOGETHER_ACK, KEGS_KNM_TOGETHER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_KNM_TOGETHER_ACK, kPacket_ );
}

IMPL_ON_FUNC( ELG_KNM_TOGETHER_BY_UNIT_INFO_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SET_ERROR( NET_OK );

	KELG_KNM_TOGETHER_BY_UNIT_INFO_ACK kPacket;
	kPacket.m_iDemandGSUID		= kPacket_.m_iDemandGSUID;
	kPacket.m_iDemandUserUID	= kPacket_.m_iDemandUserUID;

	//{{ 2009. 3. 24  최육사	GM제한
	if( GetAuthLevel() == SEnum::UAL_GM )
	{
		SET_ERROR( ERR_GM_CHAR_DENY_00 );
		goto end_proc;
	}
	//}}

	if( GetRoomUID() != 0 )
	{
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_PVP )
		{
			kPacket.m_kUnitInfo.m_iState		= GetUnitState();
			kPacket.m_kUnitInfo.m_iDungeonID	= 0;
			kPacket.m_kUnitInfo.m_iUnitUID		= GetCharUID();
			kPacket.m_kUnitInfo.m_iRoomUID		= GetRoomUID();
		}
		else
		{
			SET_ERROR( ERR_KNM_05 );
			goto end_proc;
		}
	}
	else
	{
		SET_ERROR( ERR_KNM_05 );
		goto end_proc;
	}

end_proc:
	kPacket.m_iOK	= NetError::GetLastError();
	SendToLoginServer( ELG_KNM_TOGETHER_BY_UNIT_INFO_ACK, kPacket );

	START_LOG( clog, L"KNM 같이할 유닛정보 찾기" )
		<< BUILD_LOG( NetError::GetErrStr(kPacket.m_iOK) )
		<< BUILD_LOG( kPacket.m_iDemandGSUID )
		<< BUILD_LOG( kPacket.m_iDemandUserUID )
		;
}

IMPL_ON_FUNC( EGS_GET_MY_MESSENGER_SN_REQ )
{
	// 우선은 제약이 없다.

	SendToAccountDB( DBE_GET_MY_MESSENGER_SN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_GET_MY_MESSENGER_SN_ACK, KEGS_GET_MY_MESSENGER_SN_ACK )
{
	// m_iOK에 대한 오류 로그는 Account DB Thread 에서 출력하기 때문에 바로 클라에게 날려준다.

	SendPacket( EGS_GET_MY_MESSENGER_SN_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_REQUEST_FRIEND_REQ )
{
	KEGS_REQUEST_FRIEND_ACK kPacket;

	// 닉네임 검사
	if( !CheckValidNickName( kPacket_.m_wstrNickName ) )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_REQUEST_FRIEND_ACK, kPacket );
		return;
	}

	// 메세지 검사
	if( kPacket_.m_wstrMessage.size() > 256 )
	{
		kPacket.m_iOK = NetError::ERR_MESSENGER_05;
		SendPacket( EGS_REQUEST_FRIEND_ACK, kPacket );
		return;
	}

	//if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrMessage ) )
	//{
	//    kPacket.m_iOK = NetError::ERR_ODBC_00;
	//    SendPacket( EGS_REQUEST_FRIEND_ACK, kPacket );
	//    return;
	//}

	// 이미 친구인 캐릭터를 초대했는지 검사
	std::map< UidType, KFriendInfo >::iterator mit;
	for( mit = m_kMessengerInfo.m_mapFriendInfo.begin(); mit != m_kMessengerInfo.m_mapFriendInfo.end(); mit++ )
	{
		if( kPacket_.m_wstrNickName.compare( mit->second.m_wstrNickName ) == 0 )
		{
			START_LOG( cerr, L"친구 요청한 캐릭터가 이미 친구 목록에 있음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_00;
			SendPacket( EGS_REQUEST_FRIEND_ACK, kPacket );
			return;
		}
	}

	// 친구 수 제한
	if( m_kMessengerInfo.m_mapFriendInfo.size() >= 100 )
	{
		kPacket.m_iOK = NetError::ERR_MESSENGER_12;
		SendPacket( EGS_REQUEST_FRIEND_ACK, kPacket );
		return;
	}

	KDBE_REQUEST_FRIEND_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_wstrNickName = kPacket_.m_wstrNickName;
	kPacketReq.m_wstrMessage = kPacket_.m_wstrMessage;
	SendToGameDB( DBE_REQUEST_FRIEND_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_REQUEST_FRIEND_ACK )
{
	KEGS_REQUEST_FRIEND_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		KFriendInfo kFriendInfo;
		kFriendInfo.m_iUnitUID = kPacket_.m_iUnitUID;
		kFriendInfo.m_wstrNickName = kPacket_.m_wstrNickName;
		kFriendInfo.m_cGroupID = 0;
		kFriendInfo.m_cStatus = KFriendInfo::FS_REQUESTED;

		std::pair< std::map< UidType, KFriendInfo >::iterator, bool > pairResult;
		pairResult = m_kMessengerInfo.m_mapFriendInfo.insert( std::make_pair( kFriendInfo.m_iUnitUID, kFriendInfo ) );
		if( !pairResult.second )
		{
			START_LOG( cerr, L"친구 요청한 캐릭터가 이미 친구 목록에 있음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kFriendInfo.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_00;
		}
	}

	SendPacket( EGS_REQUEST_FRIEND_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		KELG_REQUEST_FRIEND_NOT kPacketNot;
		kPacketNot.m_iDestUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_wstrDestNickName = kPacket_.m_wstrNickName;
		kPacketNot.m_iUnitUID = GetCharUID();
		kPacketNot.m_wstrNickName = GetCharName();
		kPacketNot.m_wstrMessage = kPacket_.m_wstrMessage;
		SendToLoginServer( ELG_REQUEST_FRIEND_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGS_REQUEST_FRIEND_NOT )
{
	KFriendInfo kFriendInfo;
	kFriendInfo.m_iUnitUID = kPacket_.m_iUnitUID;
	kFriendInfo.m_wstrNickName = kPacket_.m_wstrNickName;
	kFriendInfo.m_cGroupID = 0;
	kFriendInfo.m_cStatus = KFriendInfo::FS_WAITING;

	std::pair< std::map< UidType, KFriendInfo >::iterator, bool > pairResult;
	pairResult = m_kMessengerInfo.m_mapFriendInfo.insert( std::make_pair( kFriendInfo.m_iUnitUID, kFriendInfo ) );
	if( !pairResult.second )
	{
		START_LOG( cerr, L"친구 요청 대기 캐릭터가 이미 친구 목록에 있음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kFriendInfo.m_iUnitUID )
			<< END_LOG;

		return;
	}

	SendPacket( EGS_REQUEST_FRIEND_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_ACCEPT_FRIEND_REQ )
{
	// 친구 목록에 없는 캐릭터에 대해 친구 수락 했는지 검사
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_ );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"친구 수락한 캐릭터가 친구 목록에 없음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;

		KEGS_ACCEPT_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_02;
		SendPacket( EGS_ACCEPT_FRIEND_ACK, kPacket );
		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_WAITING )
	{
		START_LOG( cerr, L"친구 수락한 캐릭터가 상태 이상." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		KEGS_ACCEPT_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_01;
		SendPacket( EGS_ACCEPT_FRIEND_ACK, kPacket );
		return;
	}

	KDBE_ACCEPT_FRIEND_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iFriendUnitUID = kPacket_;
	SendToGameDB( DBE_ACCEPT_FRIEND_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_ACCEPT_FRIEND_ACK )
{
	KEGS_ACCEPT_FRIEND_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
		if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
		{
			START_LOG( cerr, L"친구 수락한 캐릭터가 친구 목록에 없었음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_02;
		}
		else
		{
			if( mit->second.m_cStatus != KFriendInfo::FS_WAITING )
			{
				START_LOG( cerr, L"친구 수락한 캐릭터가 대기 상태가 아니었음." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOGc( mit->second.m_cStatus )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MESSENGER_01;
			}
			mit->second.m_cStatus = KFriendInfo::FS_NORMAL;
		}
	}

	SendPacket( EGS_ACCEPT_FRIEND_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		KELG_ACCEPT_FRIEND_NOT kPacketNot;
		kPacketNot.m_iDestUnitUID = kPacket.m_iUnitUID;
		kPacketNot.m_iUnitUID = GetCharUID();
		SendToLoginServer( ELG_ACCEPT_FRIEND_NOT, kPacketNot );

		KELG_FRIEND_POSITION_UPDATE_NOT kPacketPosNot;
		kPacketPosNot.m_iUnitUID = GetCharUID();
		kPacketPosNot.m_setFriendUID.insert( kPacket_.m_iUnitUID );
		kPacketPosNot.m_cPosition = KFriendInfo::FP_ON_LINE;
		SendToLoginServer( ELG_FRIEND_POSITION_UPDATE_NOT, kPacketPosNot );
	}
}

IMPL_ON_FUNC( EGS_ACCEPT_FRIEND_NOT )
{
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"친구 수락해 준 캐릭터가 친구 목록에 없었음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_REQUESTED )
	{
		START_LOG( cerr, L"친구 수락해 준 캐릭터가 상태 이상" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		return;
	}

	mit->second.m_cStatus = KFriendInfo::FS_NORMAL;

	SendPacket( EGS_ACCEPT_FRIEND_NOT, kPacket_ );

	KELG_FRIEND_POSITION_UPDATE_NOT kPacketPosNot;
	kPacketPosNot.m_iUnitUID = GetCharUID();
	kPacketPosNot.m_setFriendUID.insert( kPacket_.m_iUnitUID );
	kPacketPosNot.m_cPosition = KFriendInfo::FP_ON_LINE;
	SendToLoginServer( ELG_FRIEND_POSITION_UPDATE_NOT, kPacketPosNot );
}

IMPL_ON_FUNC( EGS_DENY_FRIEND_REQ )
{
	// 친구 목록에 없는 캐릭터에 대해 친구 거부 했는지 검사
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_ );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"친구 거부한 캐릭터가 친구 목록에 없음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;

		KEGS_DENY_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_02;
		SendPacket( EGS_DENY_FRIEND_ACK, kPacket );
		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_WAITING )
	{
		START_LOG( cerr, L"친구 거부한 캐릭터가 상태 이상." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		KEGS_DENY_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_01;
		SendPacket( EGS_DENY_FRIEND_ACK, kPacket );
		return;
	}

	KDBE_DENY_FRIEND_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iFriendUnitUID = kPacket_;
	SendToGameDB( DBE_DENY_FRIEND_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_DENY_FRIEND_ACK )
{
	KEGS_DENY_FRIEND_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
		if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
		{
			START_LOG( cerr, L"친구 거부한 캐릭터가 친구 목록에 없었음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_02;
		}
		else
		{
			if( mit->second.m_cStatus != KFriendInfo::FS_WAITING )
			{
				START_LOG( cerr, L"친구 거부한 캐릭터가 상태 이상." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOGc( mit->second.m_cStatus )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MESSENGER_01;
			}
			m_kMessengerInfo.m_mapFriendInfo.erase( mit );
		}
	}

	SendPacket( EGS_DENY_FRIEND_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		KELG_DENY_FRIEND_NOT kPacketNot;
		kPacketNot.m_iDestUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_iUnitUID = GetCharUID();
		SendToLoginServer( ELG_DENY_FRIEND_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGS_DENY_FRIEND_NOT )
{
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"나를 친구 거부한 캐릭터가 친구 목록에 없었음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_REQUESTED )
	{
		START_LOG( cerr, L"나를 친구 거부한 캐릭터가 상태 이상" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		return;
	}

	m_kMessengerInfo.m_mapFriendInfo.erase( mit );

	SendPacket( EGS_DENY_FRIEND_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_BLOCK_FRIEND_REQ )
{
	// 친구 목록에 없는 캐릭터에 대해 차단했는지 검사
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_ );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"친구 차단한 캐릭터가 친구 목록에 없음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;

		KEGS_BLOCK_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		SendPacket( EGS_BLOCK_FRIEND_ACK, kPacket );
		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_NORMAL )
	{
		START_LOG( cerr, L"친구 차단한 캐릭터가 상태 이상." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		KEGS_BLOCK_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_01;
		SendPacket( EGS_BLOCK_FRIEND_ACK, kPacket );
		return;
	}

	KDBE_BLOCK_FRIEND_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iFriendUnitUID = kPacket_;
	SendToGameDB( DBE_BLOCK_FRIEND_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_BLOCK_FRIEND_ACK )
{
	KEGS_BLOCK_FRIEND_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
		if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
		{
			START_LOG( cerr, L"차단한 캐릭터가 친구 목록에 없었음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		}
		else
		{
			if( mit->second.m_cStatus != KFriendInfo::FS_NORMAL )
			{
				START_LOG( cerr, L"친구 차단한 캐릭터가 상태 이상." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOGc( mit->second.m_cStatus )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MESSENGER_01;
			}
			mit->second.m_cStatus = KFriendInfo::FS_BLOCKED;
		}
	}

	SendPacket( EGS_BLOCK_FRIEND_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		KELG_BLOCK_FRIEND_NOT kPacketNot;
		kPacketNot.m_iDestUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_iUnitUID = GetCharUID();
		SendToLoginServer( ELG_BLOCK_FRIEND_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGS_BLOCK_FRIEND_NOT )
{
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"나를 차단한 캐릭터가 친구 목록에 없었음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_NORMAL )
	{
		START_LOG( cerr, L"나를 친구 차단한 캐릭터가 상태 이상" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		return;
	}

	SendPacket( EGS_BLOCK_FRIEND_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_UNBLOCK_FRIEND_REQ )
{
	// 친구 목록에 없는 캐릭터에 대해 차단 해제했는지 검사
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_ );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"친구 차단 해제한 캐릭터가 친구 목록에 없음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;

		KEGS_UNBLOCK_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		SendPacket( EGS_UNBLOCK_FRIEND_ACK, kPacket );
		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_BLOCKED )
	{
		START_LOG( cerr, L"친구 차단 해제한 캐릭터가 상태 이상." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		KEGS_UNBLOCK_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_01;
		SendPacket( EGS_UNBLOCK_FRIEND_ACK, kPacket );
		return;
	}

	KDBE_UNBLOCK_FRIEND_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iFriendUnitUID = kPacket_;
	SendToGameDB( DBE_UNBLOCK_FRIEND_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_UNBLOCK_FRIEND_ACK )
{
	KEGS_UNBLOCK_FRIEND_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
		if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
		{
			START_LOG( cerr, L"차단 해제한 캐릭터가 친구 목록에 없었음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		}
		else
		{
			if( mit->second.m_cStatus != KFriendInfo::FS_BLOCKED )
			{
				START_LOG( cerr, L"친구 차단 해제한 캐릭터가 상태 이상." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOGc( mit->second.m_cStatus )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MESSENGER_01;
			}
			mit->second.m_cStatus = KFriendInfo::FS_NORMAL;
		}
	}

	SendPacket( EGS_UNBLOCK_FRIEND_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		KELG_UNBLOCK_FRIEND_NOT kPacketNot;
		kPacketNot.m_iDestUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_iUnitUID = GetCharUID();
		SendToLoginServer( ELG_UNBLOCK_FRIEND_NOT, kPacketNot );

		KELG_FRIEND_POSITION_UPDATE_NOT kPacketPosNot;
		kPacketPosNot.m_iUnitUID = GetCharUID();
		kPacketPosNot.m_setFriendUID.insert( kPacket_.m_iUnitUID );
		kPacketPosNot.m_cPosition = KFriendInfo::FP_ON_LINE;
		SendToLoginServer( ELG_FRIEND_POSITION_UPDATE_NOT, kPacketPosNot );
	}
}

IMPL_ON_FUNC( EGS_UNBLOCK_FRIEND_NOT )
{
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"나를 차단 해제한 캐릭터가 친구 목록에 없었음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		return;
	}

	if( mit->second.m_cStatus != KFriendInfo::FS_NORMAL )
	{
		START_LOG( cerr, L"나를 차단 해제한 캐릭터가 상태 이상" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( mit->second.m_cStatus )
			<< END_LOG;

		return;
	}

	SendPacket( EGS_UNBLOCK_FRIEND_NOT, kPacket_ );

	KELG_FRIEND_POSITION_UPDATE_NOT kPacketPosNot;
	kPacketPosNot.m_iUnitUID = GetCharUID();
	kPacketPosNot.m_setFriendUID.insert( kPacket_.m_iUnitUID );
	kPacketPosNot.m_cPosition = KFriendInfo::FP_ON_LINE;
	SendToLoginServer( ELG_FRIEND_POSITION_UPDATE_NOT, kPacketPosNot );
}

IMPL_ON_FUNC( EGS_DELETE_FRIEND_REQ )
{
	// 친구 목록에 없는 캐릭터를 삭제했는지 검사
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_ );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"친구 삭제한 캐릭터가 친구 목록에 없음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;

		KEGS_DELETE_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		SendPacket( EGS_UNBLOCK_FRIEND_ACK, kPacket );
		return;
	}

	KDBE_DELETE_FRIEND_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iFriendUnitUID = kPacket_;
	SendToGameDB( DBE_DELETE_FRIEND_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_DELETE_FRIEND_ACK )
{
	KEGS_DELETE_FRIEND_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
		if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
		{
			START_LOG( cerr, L"삭제한 캐릭터가 친구 목록에 없었음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		}
		else
		{
			m_kMessengerInfo.m_mapFriendInfo.erase( mit );
		}
	}

	SendPacket( EGS_DELETE_FRIEND_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		KELG_DELETE_FRIEND_NOT kPacketNot;
		kPacketNot.m_iDestUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_iUnitUID = GetCharUID();
		SendToLoginServer( ELG_DELETE_FRIEND_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC( EGS_DELETE_FRIEND_NOT )
{
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"나를 삭제한 캐릭터가 친구 목록에 없었음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		return;
	}
	else
	{
		m_kMessengerInfo.m_mapFriendInfo.erase( mit );
	}

	SendPacket( EGS_DELETE_FRIEND_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_MOVE_FRIEND_REQ )
{
	// 친구 목록에 없는 캐릭터를 그룹 이동했는지 검사
	std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
	if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
	{
		START_LOG( cerr, L"그룹 이동하려는 캐릭터가 친구 목록에 없음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		KEGS_MOVE_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		SendPacket( EGS_MOVE_FRIEND_ACK, kPacket );
		return;
	}

	std::map< char, std::wstring >::iterator mitGroup = m_kMessengerInfo.m_mapGroup.find( kPacket_.m_cTargetGroupID );
	if( mitGroup == m_kMessengerInfo.m_mapGroup.end() )
	{
		START_LOG( cerr, L"그룹이 존재하지 않음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( mit->second.m_cGroupID )
			<< BUILD_LOGc( kPacket_.m_cTargetGroupID )
			<< END_LOG;

		KEGS_MOVE_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_04;
		SendPacket( EGS_MOVE_FRIEND_ACK, kPacket );
		return;
	}

	if( mit->second.m_cGroupID == kPacket_.m_cTargetGroupID )
	{
		START_LOG( cerr, L"그룹이 같음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( mit->second.m_cGroupID )
			<< BUILD_LOGc( kPacket_.m_cTargetGroupID )
			<< END_LOG;

		KEGS_MOVE_FRIEND_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_06;
		SendPacket( EGS_MOVE_FRIEND_ACK, kPacket );
		return;
	}

	KDBE_MOVE_FRIEND_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_iFriendUnitUID = kPacket_.m_iUnitUID;
	kPacketReq.m_cTargetGroupID = kPacket_.m_cTargetGroupID;
	SendToGameDB( DBE_MOVE_FRIEND_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_MOVE_FRIEND_ACK )
{
	KEGS_MOVE_FRIEND_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< UidType, KFriendInfo >::iterator mit = m_kMessengerInfo.m_mapFriendInfo.find( kPacket_.m_iUnitUID );
		if( mit == m_kMessengerInfo.m_mapFriendInfo.end() )
		{
			START_LOG( cerr, L"그룹 이동한 캐릭터가 친구 목록에 없었음." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_03;
		}
		else
		{
			std::map< char, std::wstring >::iterator mitGroup = m_kMessengerInfo.m_mapGroup.find( kPacket_.m_cTargetGroupID );
			if( mitGroup == m_kMessengerInfo.m_mapGroup.end() )
			{
				START_LOG( cerr, L"그룹이 존재하지 않음." )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_cTargetGroupID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MESSENGER_04;
			}
			else
			{
				mit->second.m_cGroupID = kPacket_.m_cTargetGroupID;
			}
		}
	}

	SendPacket( EGS_MOVE_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_MAKE_FRIEND_GROUP_REQ )
{
	int iMaxNumGroup = 100;
	if( ( int )m_kMessengerInfo.m_mapGroup.size() >= iMaxNumGroup )
	{
		START_LOG( cerr, L"그룹 수가 너무 많음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_kMessengerInfo.m_mapGroup.size() )
			<< END_LOG;

		KEGS_MAKE_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_07;
		SendPacket( EGS_MAKE_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	char cCandidate = 1;
	std::map< char, std::wstring >::iterator mitGroup;
	for( ; ( int )cCandidate <= iMaxNumGroup; cCandidate++ )
	{
		mitGroup = m_kMessengerInfo.m_mapGroup.find( cCandidate );
		if( mitGroup == m_kMessengerInfo.m_mapGroup.end() )
		{
			break;
		}
	}

	if( ( int )cCandidate > iMaxNumGroup )
	{
		START_LOG( cerr, L"새 그룹 ID를 찾지 못함." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( m_kMessengerInfo.m_mapGroup.size() )
			<< END_LOG;

		KEGS_MAKE_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_07;
		SendPacket( EGS_MAKE_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	// 문자열 검사
	if( kPacket_.m_wstrGroupName.size() > 30 )
	{
		KEGS_MAKE_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_11;
		SendPacket( EGS_MAKE_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrGroupName ) )
	{
		KEGS_MAKE_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_MAKE_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	KDBE_MAKE_FRIEND_GROUP_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_cGroupID = cCandidate;
	kPacketReq.m_wstrGroupName = kPacket_.m_wstrGroupName;
	SendToGameDB( DBE_MAKE_FRIEND_GROUP_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_MAKE_FRIEND_GROUP_ACK )
{
	KEGS_MAKE_FRIEND_GROUP_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< char, std::wstring >::iterator mitGroup = m_kMessengerInfo.m_mapGroup.find( kPacket_.m_cGroupID );
		if( mitGroup != m_kMessengerInfo.m_mapGroup.end() )
		{
			START_LOG( cerr, L"그룹 ID 중복" )
				<< BUILD_LOG( kPacket_.m_cGroupID )
				<< BUILD_LOG( mitGroup->first )
				<< BUILD_LOG( mitGroup->second )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_08;
		}
		else
		{
			m_kMessengerInfo.m_mapGroup.insert( std::make_pair( kPacket_.m_cGroupID, kPacket_.m_wstrGroupName ) );
		}
	}

	SendPacket( EGS_MAKE_FRIEND_GROUP_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_RENAME_FRIEND_GROUP_REQ )
{
	std::map< char, std::wstring >::iterator mitGroup;
	mitGroup = m_kMessengerInfo.m_mapGroup.find( kPacket_.m_cGroupID );
	if( mitGroup == m_kMessengerInfo.m_mapGroup.end() )
	{
		START_LOG( cerr, L"그룹이 존재하지 않음" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_cGroupID )
			<< END_LOG;

		KEGS_RENAME_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_04;
		SendPacket( EGS_RENAME_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	// 문자열 검사
	if( kPacket_.m_wstrGroupName.size() > 30 )
	{
		KEGS_RENAME_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_11;
		SendPacket( EGS_RENAME_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrGroupName ) )
	{
		KEGS_RENAME_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_RENAME_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	if( kPacket_.m_wstrGroupName.compare( mitGroup->second ) == 0 )
	{
		START_LOG( cerr, L"그룹 이름이 같음" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_cGroupID )
			<< END_LOG;

		KEGS_RENAME_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_09;
		SendPacket( EGS_RENAME_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	KDBE_RENAME_FRIEND_GROUP_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_cGroupID = kPacket_.m_cGroupID;
	kPacketReq.m_wstrGroupName = kPacket_.m_wstrGroupName;
	SendToGameDB( DBE_RENAME_FRIEND_GROUP_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_RENAME_FRIEND_GROUP_ACK )
{
	KEGS_RENAME_FRIEND_GROUP_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< char, std::wstring >::iterator mitGroup = m_kMessengerInfo.m_mapGroup.find( kPacket_.m_cGroupID );
		if( mitGroup == m_kMessengerInfo.m_mapGroup.end() )
		{
			START_LOG( cerr, L"그룹이 존재하지 않음" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_cGroupID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_04;
		}
		else
		{
			mitGroup->second = kPacket_.m_wstrGroupName;
		}
	}

	SendPacket( EGS_RENAME_FRIEND_GROUP_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_DELETE_FRIEND_GROUP_REQ )
{
	std::map< char, std::wstring >::iterator mitGroup;
	mitGroup = m_kMessengerInfo.m_mapGroup.find( kPacket_.m_cGroupID );
	if( mitGroup == m_kMessengerInfo.m_mapGroup.end() )
	{
		START_LOG( cerr, L"그룹이 존재하지 않음" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_cGroupID )
			<< END_LOG;

		KEGS_DELETE_FRIEND_GROUP_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MESSENGER_04;
		SendPacket( EGS_DELETE_FRIEND_GROUP_ACK, kPacket );
		return;
	}

	KDBE_DELETE_FRIEND_GROUP_REQ kPacketReq;
	kPacketReq.m_iUnitUID = GetCharUID();
	kPacketReq.m_cGroupID = kPacket_.m_cGroupID;
	SendToGameDB( DBE_DELETE_FRIEND_GROUP_REQ, kPacketReq );
}

IMPL_ON_FUNC( DBE_DELETE_FRIEND_GROUP_ACK )
{
	KEGS_DELETE_FRIEND_GROUP_ACK kPacket = kPacket_;

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		std::map< char, std::wstring >::iterator mitGroup = m_kMessengerInfo.m_mapGroup.find( kPacket_.m_cGroupID );
		if( mitGroup == m_kMessengerInfo.m_mapGroup.end() )
		{
			START_LOG( cerr, L"그룹이 존재하지 않음" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_cGroupID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_MESSENGER_04;
		}
		else
		{
			std::map< UidType, KFriendInfo >::iterator mit;
			for( mit = m_kMessengerInfo.m_mapFriendInfo.begin(); mit != m_kMessengerInfo.m_mapFriendInfo.end(); mit++ )
			{
				if( mit->second.m_cGroupID == kPacket_.m_cGroupID )
				{
					mit->second.m_cGroupID = 0;
				}
			}
		}
	}

	SendPacket( EGS_DELETE_FRIEND_GROUP_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_FRIEND_POSITION_UPDATE_NOT )
{
	std::map< UidType, char >::iterator mitPosition;
	for( mitPosition = kPacket_.m_mapFriendPosition.begin(); mitPosition != kPacket_.m_mapFriendPosition.end(); mitPosition++ )
	{
		std::map< UidType, KFriendInfo >::iterator mitFriend;
		mitFriend = m_kMessengerInfo.m_mapFriendInfo.find( mitPosition->first );
		if( mitFriend == m_kMessengerInfo.m_mapFriendInfo.end() )
		{
			START_LOG( cerr, L"친구 리스트에 없는 유닛임." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( mitPosition->first )
				<< END_LOG;

			continue;
		}

		mitFriend->second.m_cPosition = mitPosition->second;
	}

	SendPacket( EGS_FRIEND_POSITION_UPDATE_NOT, kPacket_ );
}


//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


