#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
#include "NexonBillingTCPManager.h"
#endif SERV_GUILD_CHANGE_NAME
//}}
#include "X2Data/XSLStringFilter.h"

#include "odbc/Odbc.h"
#include "Enum/Enum.h"

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
#include "GSSimLayer.h"
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

#ifdef SERV_STRING_FILTER_USING_DB
#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//////////////////////////////////////////////////////////////////////////
//#ifdef SERV_GSUSER_CPP
//#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

//{{ 2009. 9. 18  최육사	길드
#ifdef GUILD_TEST

IMPL_ON_FUNC( EGS_CREATE_GUILD_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_GUILD_REQ, EGS_CREATE_GUILD_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_CREATE_GUILD_ACK kAck;

		// 체험ID는 길드창단 불가능
		if( IsGuestUser() )
		{
			kAck.m_iOK = NetError::ERR_GUEST_USER_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// 길드 이름 길이 검사
		std::string strGuildName = KncUtil::toNarrowString( kPacket_.m_wstrGuildName );
		if( kPacket_.m_wstrGuildName.empty()  ||  strGuildName.size() > 16 )
		{
			kAck.m_iOK = NetError::ERR_GUILD_45;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// 닉네임과 동일한 조건으로 길드 이름에 대한 필터링 적용
#ifdef SERV_STRING_FILTER_USING_DB
		if( SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#else //SERV_STRING_FILTER_USING_DB
		if( SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
		{
			kAck.m_iOK = NetError::ERR_GUILD_04;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// sql injection 검사
		if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrGuildName ) )
		{
			kAck.m_iOK = NetError::ERR_ODBC_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// sql injection 검사
		if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrGuildMessage ) )
		{
			kAck.m_iOK = NetError::ERR_ODBC_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		// 현재 내가 길드원인 경우에는 길드 생성이 불가능하다!
		if( GetGuildUID() > 0 )
		{
			START_LOG( cerr, L"이미 길드에 가입한 상태라 길드 생성 불가! 일어나서는 안되는 에러" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetGuildUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// 길드 창단 아이템이 사용가능한지 검사
		if( m_kInventory.UseGuildFoundationItemCheckOnly( kPacket_.m_iItemUID ) == false )
		{
			kAck.m_iOK = NetError::GetLastError();
			SendPacket( EGS_CREATE_GUILD_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		// 2010. 07. 26.	박교현		길드명 유일성 인증
		KEGIANT_ROLEREG_CREATE_GUILD_REQ kPacketReq;
		kPacketReq.m_iActorUID = GetCharUID();
		kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacketReq.m_CreateGuildReqPacket = kPacket_;

		SendToGiantRoleReg( EGIANT_ROLEREG_CREATE_GUILD_REQ, kPacketReq );
	}
	else
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	{
		//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
	// Login Server로 길드 생성하러 가자!
	KELG_CREATE_GUILD_REQ kPacketToLG;
	kPacketToLG.m_iUnitUID = GetCharUID();
	kPacketToLG.m_iItemUID = kPacket_.m_iItemUID;
	kPacketToLG.m_wstrGuildName = kPacket_.m_wstrGuildName;
	kPacketToLG.m_wstrGuildMessage = kPacket_.m_wstrGuildMessage;
	SendToLoginServer( ELG_CREATE_GUILD_REQ, kPacketToLG );
#else
	// DB로 길드생성 가능한지 검사하러 가자!
	KDBE_CREATE_GUILD_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iItemUID = kPacket_.m_iItemUID;
	kPacketToDB.m_wstrGuildName = kPacket_.m_wstrGuildName;
	kPacketToDB.m_wstrGuildMessage = kPacket_.m_wstrGuildMessage;
	SendToGameDB( DBE_CREATE_GUILD_REQ, kPacketToDB );
#endif SERV_GUILD_CHANGE_NAME
	}

}
//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
IMPL_ON_FUNC( ELG_CREATE_GUILD_ACK )
#else
IMPL_ON_FUNC( DBE_CREATE_GUILD_ACK )
#endif SERV_GUILD_CHANGE_NAME
//}}
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CREATE_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_CREATE_GUILD_ACK kPacketAck;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		if( kPacket_.m_bReqGiant_DeleteName )
		{
			// 이름 유일성 인증 끝난 후에 에러 날시 다시 자이언트에 삭제 요청해야 함 
			KEGIANT_ROLEREG_DELETE_GUILD_REQ kPacketReq;
			kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
			kPacketReq.m_wstrGuildName = kPacket_.m_wstrGuildName;

			SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_GUILD_REQ, kPacketReq );
		}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CREATE_GUILD_ACK, kPacketAck );
		return;
	}

	// 길드 생성 성공했으니 아이템들을 지우자!
	if( m_kInventory.UseGuildFoundationItem( kPacket_.m_iItemUID, kPacketAck.m_vecInventorySlotInfo, false ) == false )
	{
		START_LOG( cerr, L"길드 생성 아이템 삭제 실패! 검사했는데? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;
	}

	// 생성된 길드의 GuildUID를 저장한다 - 길드 생성했으니 길드 마스터로 등록함
	m_kUserGuildManager.SetGuildInfo( kPacket_.m_kCreatedGuildInfo.m_iGuildUID, kPacket_.m_kCreatedGuildInfo.m_wstrGuildName, SEnum::GUG_MASTER );

	// 클라이언트로 길드 생성에 대한 응답을 보낸다.
	kPacketAck.m_kGuildInfo = kPacket_.m_kCreatedGuildInfo;	
	kPacketAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CREATE_GUILD_ACK, kPacketAck );

	// DB에 생성된 길드 정보를 GuildManager에게 업데이트 하러 가자!
	KELG_CREATE_GUILD_RESULT_REQ kPacketToLg;
	kPacketToLg.m_kGuildInfo = kPacket_.m_kCreatedGuildInfo;
	//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	kPacketToLg.m_kGuildSkillInfo = kPacket_.m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}
	GetGuildMemberInfo( kPacketToLg.m_kLoginGuildMember );
	kPacketToLg.m_vecMemberList.push_back( kPacketToLg.m_kLoginGuildMember );
	SendToLoginServer( ELG_CREATE_GUILD_RESULT_REQ, kPacketToLg );

	//{{ 2009. 11. 2  최육사	길드창단 공지
	{
		KEGS_NOTIFY_MSG_NOT	kPacketNot;
		kPacketNot.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_GUILD_CREATION;
		kPacketNot.m_Count = 1;		

		CStringW cwstrMessage;
		cwstrMessage.Format( L"%s;%s", GetCharName().c_str(), kPacket_.m_kCreatedGuildInfo.m_wstrGuildName.c_str() );
		kPacketNot.m_wstrMSG = static_cast<LPCTSTR>(cwstrMessage);

		SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacketNot );
	}
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 12. 22  최육사	신서버이벤트 "2009-12-23 ~ 2010-01-27"
	//if( KBaseServer::GetKObj()->GetServerGroupID() == SEnum::SGI_GAIA )
	//{
	//	// DB로 보상 정보를 보낸다.
	//	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	//	kPacketToDB.m_iFromUnitUID = GetCharUID();
	//	kPacketToDB.m_iToUnitUID   = GetCharUID();
	//	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT; // 보상 타입
	//	kPacketToDB.m_iRewardID	   = 195;
	//	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	//}
	//}}
	//////////////////////////////////////////////////////////////////////////

#ifdef SERV_CREATE_GUILD_EVENT
	// 이벤트 보상을 주자!
//	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
//	kPacketToDB.m_iFromUnitUID = GetCharUID();
//	kPacketToDB.m_iToUnitUID   = GetCharUID();
//	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_EVENT;
//	kPacketToDB.m_iRewardID	   = 832;// 그노시스의 대축복(15일,5GSP) 큐브
//	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );	
#endif //SERV_CREATE_GUILD_EVENT


	// 길드 생성 성공했으니 길드이름 갱신해주자!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// 필드에 있을경우 유저 정보 갱신하기
		if( GetFieldUID() > 0 )
		{
			KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
			GetFieldUserInfo( kNot.m_kInfo );
			SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
		}
	}		
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				KSquareUserInfo kSquareUserInfo;
				GetSquareUserInfo( kSquareUserInfo );
				SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
			}
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방 타입." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}

_IMPL_ON_FUNC( ELG_CREATE_GUILD_RESULT_ACK, KEGS_GUILD_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 로그인서버에 업데이트 해놓고온 결과를 다시 클라이언트에게 전달!
	SendPacket( EGS_GUILD_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_INFO_ACK, KEGS_GUILD_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	m_kUserGuildManager.SetGuildSkill( kPacket_.m_kGuildSkillInfo );
#endif GUILD_SKILL_TEST
	//}}

	SendPacket( EGS_GUILD_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_GUILD_USER_LIST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 내가 길드 소속인지 보자
	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드소속도 아닌데 길드유저리스트 얻을려고 하네?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		KEGS_GET_GUILD_USER_LIST_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_16;
		SendPacket( EGS_GET_GUILD_USER_LIST_ACK, kPacket );
		return;
	}

	// 로그인서버로 보내자
	KELG_GET_GUILD_USER_LIST_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_uiViewPage = kPacket_.m_uiViewPage;
	kPacketToLg.m_cGuildUserListType = kPacket_.m_cGuildUserListType;
	SendToLoginServer( ELG_GET_GUILD_USER_LIST_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_USER_LIST_ACK, KEGS_GET_GUILD_USER_LIST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GET_GUILD_USER_LIST_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GUILD_INFO_NOT, KEGS_GUILD_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_GUILD_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GUILD_MESSAGE_NOT, KEGS_GUILD_MESSAGE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_KICK_MEMBER  ||
		kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_KICK_BY_MASTER )
	{
		// 탈퇴하는 사람이 나라면? 초기화!
		if( GetCharUID() == kPacket_.m_iUnitUID )
		{
			// 길드 정보 초기화
			m_kUserGuildManager.Clear();

			// 길드 이름 갱신
			if( GetStateID() == KGSFSM::S_FIELD_MAP )
			{
				// 필드에 있을경우 유저 정보 갱신하기
				if( GetFieldUID() > 0 )
				{
					KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
					GetFieldUserInfo( kNot.m_kInfo );
					SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
				}
			}		
			else if( GetStateID() == KGSFSM::S_ROOM )
			{
				switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
				{
				case CXSLRoom::RT_PVP:
				case CXSLRoom::RT_DUNGEON:
				case CXSLRoom::RT_TRAININGCENTER:
					{
						KRoomUserInfo kRoomUserInfo;
						GetRoomUserInfo( kRoomUserInfo );
						SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );

						//{{ 2009. 12. 8  최육사	길드 스킬
#ifdef GUILD_SKILL_TEST
						KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
						kPacketNot = kRoomUserInfo;
						SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
#endif GUILD_SKILL_TEST
						//}}
					}
					break;

				case CXSLRoom::RT_SQUARE:
					{
						KSquareUserInfo kSquareUserInfo;
						GetSquareUserInfo( kSquareUserInfo );
						SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
					}
					break;

				default:
					{
						START_LOG( cerr, L"이상한 방 타입." )
							<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
							<< BUILD_LOG( GetCharUID() )
							<< END_LOG;
					}
					break;
				}
			}
		}
	}
	else if( kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_CHANGE_MEMBER_GRADE )
	{
		if( GetCharUID() == kPacket_.m_iUnitUID )
		{
			// 변경 전 등급 얻기
			const u_char ucBeforeMemberShipGrade = m_kUserGuildManager.GetMemberShipGrade();

			// 멤버쉽 등급 변경에 따른 처리
			m_kUserGuildManager.SetMemberShipGrade( static_cast<u_char>(kPacket_.m_iMessageData) );

			// 만약에 방안이라면 방안의 다른 유저들에게 해당 정보를 브로드캐스팅 하자!
			if( ucBeforeMemberShipGrade == SEnum::GUG_NEW_USER  || 
				m_kUserGuildManager.GetMemberShipGrade() == SEnum::GUG_NEW_USER )
			{
				if( GetStateID() == KGSFSM::S_ROOM )
				{
					switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
					{
					case CXSLRoom::RT_PVP:
					case CXSLRoom::RT_DUNGEON:
					case CXSLRoom::RT_TRAININGCENTER:
						{
							KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
							GetRoomUserInfo( kPacketNot );
							SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
						}
						break;
					}
				}
			}			
		}
	}
	else if( kPacket_.m_cMessageType == KEGS_GUILD_MESSAGE_NOT::MT_INSERT_GUILD_CSPOINT )
	{
		// 사용중인 기간제 스킬 포인트를 저장!
		m_kUserGuildManager.SetMaxGuildCSPoint( kPacket_.m_iMessageData2 );
	}

	SendPacket( EGS_GUILD_MESSAGE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EGS_INVITE_GUILD_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrNickName ) == false )
	{
		KEGS_INVITE_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_00;
		SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드에 가입되어있지 않은데 길드초대를 하려함." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		KEGS_INVITE_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_07;
		SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrNickName ) == true )
	{
		KEGS_INVITE_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
		return;
	}

#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 12  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( kPacket_.m_iUnitUID > 0 )
	{
		if( IsSameServerGroupUID( kPacket_.m_iUnitUID ) == false )
		{
			KEGS_INVITE_GUILD_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_DIF_SERVER_GROUP_02;
			SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
			return;
		}
	}	
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	// Guild Manager로 가서 다음 항목을 체크합니다.
	KELG_INVITE_GUILD_REQ kPacket;
	kPacket.m_iSenderUnitUID = GetCharUID();
	kPacket.m_iGuildUID = GetGuildUID();
	kPacket.m_wstrReceiverNickName = kPacket_.m_wstrNickName;
	kPacket.m_iReceiverUnitUID = kPacket_.m_iUnitUID;
	SendToLoginServer( ELG_INVITE_GUILD_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_FIND_USER_REQ )
{
	// 상태를 막을까?

	KELG_INVITE_GUILD_FIND_USER_ACK kPacket;
	kPacket.m_usEventID = kPacket_.m_usEventID;
	kPacket.m_iGuildUID = kPacket_.m_kGuildInfo.m_iGuildUID;
	kPacket.m_iSenderUnitUID = kPacket_.m_iSenderUnitUID;
	kPacket.m_iReceiverUnitUID = kPacket_.m_iReceiverUnitUID;
	kPacket.m_wstrReceiverNickName = GetCharName();

	// 제대로 왔는감?
	if( kPacket_.m_iReceiverUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"초대장이 잘못왔다!? 일어나서는 안되는에러!" )
			<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_07;
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// 이미 길드에 가입되어있는지 체크
	if( GetGuildUID() > 0 )
	{
		// 이미 길드에 가입되어있네?
		kPacket.m_iOK = NetError::ERR_GUILD_15;
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// 초대하려는 길드와 같은 길드원인지 체크
	if( GetGuildUID() == kPacket_.m_kGuildInfo.m_iGuildUID )
	{
		START_LOG( cerr, L"이미 우리 길드원이네? GuildManager에서 검사했을텐데.." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_14;
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// 길드 초대 거부
	if( m_kUserGameOption.IsDenyInviteGuild() )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_17; // 길드 초대 거절
		SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );
		return;
	}

	// 응답을 보내자!
	kPacket.m_iOK = NetError::NET_OK;
	SendToLoginServer( ELG_INVITE_GUILD_FIND_USER_ACK, kPacket );

	// 초대장을 클라이언트로 보내자!
	KEGS_INVITE_GUILD_NOT kPacketNot;
	kPacketNot.m_kGuildInfo = kPacket_.m_kGuildInfo;
	kPacketNot.m_wstrSenderNickName = kPacket_.m_wstrSenderNickName;
	kPacketNot.m_bAutoJoinGuild = kPacket_.m_bAutoJoinGuild;
	SendPacket( EGS_INVITE_GUILD_NOT, kPacketNot );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 접속중인 유저중에서 찾지 못했다면..
	if( kPacket_.m_iOK == NetError::ERR_GUILD_08 )
	{
		// DB로 가서 존재하는 닉네임인지 찾아보자!
		SendToGameDB( DBE_INVITE_GUILD_NICKNAME_CHECK_REQ, kPacket_ );
		return;
	}

	KEGS_INVITE_GUILD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_INVITE_GUILD_NICKNAME_CHECK_ACK, KELG_INVITE_GUILD_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_INVITE_GUILD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_INVITE_GUILD_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_INVITE_GUILD_REPLY_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 길드 가입 초대에 대한 응답을 알리러가자!
	KELG_INVITE_GUILD_REPLY_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacketToLg.m_wstrSenderNickName = kPacket_.m_wstrSenderNickName;
	kPacketToLg.m_iReason = kPacket_.m_iReason;
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_wstrNickName = GetCharName();
	SendToLoginServer( ELG_INVITE_GUILD_REPLY_REQ, kPacketToLg );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_REPLY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 길드 가입 신청 성공일 경우
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KDBE_JOIN_GUILD_REQ kPacketToDB;
		kPacketToDB.m_iGuildUID = kPacket_.m_iGuildUID;		
		kPacketToDB.m_iGuildMasterUnitUID = kPacket_.m_iGuildMasterUnitUID;
		GetGuildMemberInfo( kPacketToDB.m_kJoinGuildMember );
		SendToGameDB( DBE_JOIN_GUILD_REQ, kPacketToDB );
	}

	KEGS_INVITE_GUILD_REPLY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	SendPacket( EGS_INVITE_GUILD_REPLY_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_INVITE_GUILD_REPLY_DENY_NOT, KEGS_INVITE_GUILD_REPLY_DENY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_INVITE_GUILD_REPLY_DENY_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_JOIN_GUILD_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 길드 정보 저장
	m_kUserGuildManager.SetGuildInfo( kPacket_.m_kGuildInfoNot.m_kGuildInfo.m_iGuildUID, kPacket_.m_kGuildInfoNot.m_kGuildInfo.m_wstrGuildName, SEnum::GUG_NEW_USER );
	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	m_kUserGuildManager.SetGuildSkill( kPacket_.m_kGuildInfoNot.m_kGuildSkillInfo );
#endif GUILD_SKILL_TEST
	//}}

	// 로그인서버로부터 길드정보를 받아왔다면! 길드 가입 결과를 알려주자!
	KEGS_JOIN_GUILD_SUCCESS_NOT kPacketNot;
	kPacketNot.m_kGuildInfo = kPacket_.m_kGuildInfoNot.m_kGuildInfo;
	kPacketNot.m_wstrGuildMasterName = kPacket_.m_kGuildInfoNot.m_wstrGuildMasterName;
	kPacketNot.m_vecMemberUnitUID = kPacket_.m_kGuildInfoNot.m_vecMemberUnitUID;
	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	kPacketNot.m_kGuildSkillInfo = kPacket_.m_kGuildInfoNot.m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}
	SendPacket( EGS_JOIN_GUILD_SUCCESS_NOT, kPacketNot );

	// 길드 이름 갱신
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// 필드에 있을경우 유저 정보 갱신하기
		if( GetFieldUID() > 0 )
		{
			KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
			GetFieldUserInfo( kNot.m_kInfo );
			SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
		}
	}		
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				KSquareUserInfo kSquareUserInfo;
				GetSquareUserInfo( kSquareUserInfo );
				SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
			}
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방 타입." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_MEMBER_GRADE_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_GRADE_REQ, EGS_CHANGE_GUILD_MEMBER_GRADE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드등급변경을 하려고 합니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
		return;
	}

	// 자기 자신의 길드 등급은 변경할수 없다
	if( kPacket_.m_iUnitUID == GetCharUID() )
	{
		START_LOG( cerr, L"자기 자신의 등급은 변경불가! 클라에서 체크 안하는가?" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_36;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
		return;
	}	

	// 로그인서버로 가서 길드멤버등급 수정 체크
	KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	SendToLoginServer( ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_GRADE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
		return;
	}

	// DB로 가서 길드 멤버 등급 수정을 요청한다.
	KDBE_CHANGE_GUILD_MEMBER_GRADE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iTargetUnitUID;
	kPacketToDB.m_wstrTargetNickName = kPacket_.m_wstrTargetNickName;
	kPacketToDB.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	kPacketToDB.m_bChangeGuildMaster = kPacket_.m_bChangeGuildMaster;
	SendToGameDB( DBE_CHANGE_GUILD_MEMBER_GRADE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_GRADE_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_GRADE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// 결과 전송
	KEGS_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_wstrTargetNickName = kPacket_.m_wstrTargetNickName;
	kPacket.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	SendPacket( EGS_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_MESSAGE_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MESSAGE_REQ, EGS_CHANGE_GUILD_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrMessage = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrMessage.c_str(), L'♡' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CHANGE_GUILD_MESSAGE_ACK kPacket;

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드메시지를 변경하려고 합니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_MESSAGE_ACK, kPacket );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_GUILD_MESSAGE_ACK, kPacket );
		return;
	}

	// DB로 가서 길드 메시지 변경을 요청한다.
	KDBE_CHANGE_GUILD_MESSAGE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_wstrMessage = kPacket_.m_wstrMessage;
	SendToGameDB( DBE_CHANGE_GUILD_MESSAGE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MESSAGE_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	// 결과 전송
	KEGS_CHANGE_GUILD_MESSAGE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CHANGE_GUILD_MESSAGE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_KICK_GUILD_MEMBER_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_KICK_GUILD_MEMBER_REQ, EGS_KICK_GUILD_MEMBER_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드 탈퇴 또는 길드원 강퇴를 하려고 합니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_KICK_GUILD_MEMBER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_KICK_GUILD_MEMBER_ACK, kPacket );
		return;
	}

	// DB로 가서 길드 멤버 탈퇴를 요청한다.
	KDBE_KICK_GUILD_MEMBER_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iUnitUID;
	SendToGameDB( DBE_KICK_GUILD_MEMBER_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_KICK_GUILD_MEMBER_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_KICK_GUILD_MEMBER_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// 결과 전송
	KEGS_KICK_GUILD_MEMBER_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_bKickBySysop = ( GetCharUID() != kPacket_.m_iTargetUnitUID );
	SendPacket( EGS_KICK_GUILD_MEMBER_ACK, kPacket );


	// 길드원 탈퇴가 성공하면 길드 매니저에 수정된 정보를 업데이트 한다.
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		// 탈퇴하는 사람이 나라면? 초기화!
		if( GetCharUID() == kPacket_.m_iTargetUnitUID )
		{
			m_kUserGuildManager.Clear();
		}
	}
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_MESSAGE_REQ, EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

#ifdef SERV_STRING_FILTER_USING_DB
	kPacket_.m_wstrMessage = SiKStringFilterManager()->FilteringChatString( kPacket_.m_wstrMessage.c_str(), L'♡' );
#endif //SERV_STRING_FILTER_USING_DB

	KEGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK kPacket;

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드원 인사말 변경을 하려고 합니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
		return;
	}

	// 길드원 인사말 글자 길이 제한
	std::string strMessage = KncUtil::toNarrowString( kPacket_.m_wstrMessage );
	if( strMessage.size() > 60 )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_27;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{		
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
		return;
	}

	// DB로 가서 길드 멤버 등급 수정을 요청한다.
	KDBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iTargetUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iGuildUID = GetGuildUID();
	kPacketToDB.m_wstrMessage = kPacket_.m_wstrMessage;
	SendToGameDB( DBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// 결과 전송
	KEGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_bDelete = kPacket_.m_wstrMessage.empty(); // 메시지가 비어있으면 delete다!
	SendPacket( EGS_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( EGS_DISBAND_GUILD_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DISBAND_GUILD_REQ, EGS_DISBAND_GUILD_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드해산을 하려고 합니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_DISBAND_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_DISBAND_GUILD_ACK, kPacket );
		return;
	}

	// DB로 가서 길드해산 요청을 한다.
	KDBE_DISBAND_GUILD_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	SendToGameDB( DBE_DISBAND_GUILD_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_DISBAND_GUILD_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_DISBAND_GUILD_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// 결과 전송
	KEGS_DISBAND_GUILD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_DISBAND_GUILD_ACK, kPacket );


	// 길드해산이 성공하면 길드 정보 초기화
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
		{
			KEGIANT_ROLEREG_DELETE_GUILD_REQ kPacketReq;
			kPacketReq.m_uiGiantUID = m_kNexonAccountInfo.m_uiNexonSN;
			kPacketReq.m_wstrGuildName = m_kUserGuildManager.GetGuildName();

			SendToGiantRoleReg( EGIANT_ROLEREG_DELETE_GUILD_REQ, kPacketReq );
		}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		// 길드정보 초기화!
		m_kUserGuildManager.Clear();

		// 길드 이름 갱신
		if( GetStateID() == KGSFSM::S_FIELD_MAP )
		{
			// 필드에 있을경우 유저 정보 갱신하기
			if( GetFieldUID() > 0 )
			{
				KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
				GetFieldUserInfo( kNot.m_kInfo );
				SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
			}
		}
		else if( GetStateID() == KGSFSM::S_ROOM )
		{
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_PVP:
			case CXSLRoom::RT_DUNGEON:
			case CXSLRoom::RT_TRAININGCENTER:
				{
					KRoomUserInfo kRoomUserInfo;
					GetRoomUserInfo( kRoomUserInfo );
					SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
				}
				break;

			case CXSLRoom::RT_SQUARE:
				{
					KSquareUserInfo kSquareUserInfo;
					GetSquareUserInfo( kSquareUserInfo );
					SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
				}
				break;

			default:
				{
					START_LOG( cerr, L"이상한 방 타입." )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;
				}
				break;
			}
		}
	}
}

IMPL_ON_FUNC( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_USE_ITEM_IN_INVENTORY_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
		return;
	}

	if( m_kInventory.UseItemInInventory( kPacket_.m_iItemUID, kPacket.m_vecKInventorySlotInfo ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	//{{ 2012. 11. 15	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	kPacket.m_iED = GetED();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	SendPacket( EGS_USE_ITEM_IN_INVENTORY_ACK, kPacket );

	// 길드 인원 확장하러 가자!
	KDBE_EXPAND_GUILD_MAX_MEMBER_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID = GetCharUID();
	kPacketToDB.m_iGuildUID = GetGuildUID();
	SendToGameDB( DBE_EXPAND_GUILD_MAX_MEMBER_REQ, kPacketToDB );
}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
IMPL_ON_FUNC( EGS_CHANGE_GUILD_NAME_CHECK_REQ )
{
	/*
	
	목적: EGS_CHANGE_GUILD_NAME_REQ는 반드시 EGS_CHANGE_GUILD_NAME_CHECK를 정상적으로 통과한 뒤에만 작동 하게끔 한다.

	1. EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_CHECK_REQ 를 중복 필터를 건다.
	2. 중복 검사가 끝난 후,
		m_iOK != NetError::NET_OK 이면
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );를 통해	EGS_CHANGE_GUILD_NAME_REQ를 해제하고,
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );를 통해			EGS_CHANGE_GUILD_NAME_CHECK_REQ를 해제한다.

		m_iOK == NetError::NET_OK 이면
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );를 통해			EGS_CHANGE_GUILD_NAME_CHECK_REQ만 해제한다.

	3. 클라이언트에서 EGS_CHANGE_GUILD_NAME_REQ를 보내왔을 때,
	EGS_CHANGE_GUILD_NAME_CHECK_REQ는 걸려 있어선 안되며, 동시에 EGS_CHANGE_GUILD_NAME_REQ는 걸려 있어야만 한다.

	- EGS_CHANGE_GUILD_NAME_CHECK 없이 EGS_CHANGE_GUILD_NAME_REQ를 조작하여 보내는 경우를 막았다.
	- EGS_CHANGE_GUILD_NAME_CHECK_ACK를 받기 전에 조작된 EGS_CHANGE_GUILD_NAME_REQ를 보내는 경우를 막았다.

	*/

	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_CHECK_REQ, EGS_CHANGE_GUILD_NAME_CHECK_ACK );

	KEGS_CHANGE_GUILD_NAME_CHECK_ACK kPacket;

	// 1. 길드에 소속되어있지 않다면, 길드 이름을 바꿀 수 없다.
	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드에 소속되어있지 않은데 길드 이름을 변경하려고 합니다!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	const u_char ucMemberShipGrade = m_kUserGuildManager.GetMemberShipGrade();
	// 2. 현재 내가 길드마스터가 아닌 경우에는 길드 이름 변경이 불가능하다!
	if( ucMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"길드 마스터가 아닌데 길드 이름을 변경 하려함!" )
			<< BUILD_LOG( m_nUnitUID )
			<< BUILD_LOG( ucMemberShipGrade )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_46; // 길드마스터가 아닙니다.
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	const std::string strGuildName = KncUtil::toNarrowString( kPacket_.m_wstrGuildName );
	// 3. 길드 이름 길이 검사
	if( kPacket_.m_wstrGuildName.empty()  ||  strGuildName.size() > 16 )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_45;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	// 4. 닉네임과 동일한 조건으로 길드 이름에 대한 필터링 적용
#ifdef SERV_STRING_FILTER_USING_DB
	if( SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#else //SERV_STRING_FILTER_USING_DB
	if( SiCXSLStringFilter()->CheckIsValidString( CXSLStringFilter::FT_NICKNAME, kPacket_.m_wstrGuildName ) == false )
#endif //SERV_STRING_FILTER_USING_DB
	{
		kPacket.m_iOK = NetError::ERR_GUILD_04;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	// 5. sql injection 검사
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrGuildName ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		return;
	}

	// 6. LoginUser로 보낼 패킷 구성
	KELG_CHANGE_GUILD_NAME_CHECK_REQ kPacketToLG;
	kPacketToLG.m_iUnitUID = GetCharUID();
	kPacketToLG.m_wstrNewGuildName = kPacket_.m_wstrGuildName;

	// LoginUser로 가자
	m_kUserGuildManager.SetGuildNameChanging();
	SendToLoginServer( ELG_CHANGE_GUILD_NAME_CHECK_REQ, kPacketToLG );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_CHECK_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		m_kUserGuildManager.ResetGuildNameChanging();
	}
	SendPacket( EGS_CHANGE_GUILD_NAME_CHECK_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_CHANGE_GUILD_NAME_REQ )
{
	JUST_VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_CHECK_REQ, EGS_CHANGE_GUILD_NAME_ACK );
	NEGATIVE_VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KELG_CHANGE_GUILD_NAME_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_52;
		kPacket.m_iUnitUID = GetCharUID();
		SendToLoginServer( DBE_CHANGE_GUILD_NAME_ACK, kPacket );
		return;
	}

	KELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_ulOrderNo = kPacket_.m_ulOrderNo;
	kPacket.m_ulProductNo = kPacket_.m_ulProductNo;
	kPacket.m_iQuantity = kPacket_.m_iQuantity;
	SendToLoginServer( ELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_TIME_CHECK_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_CHANGE_GUILD_NAME_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CHANGE_GUILD_NAME_ACK, kPacket );
		m_kUserGuildManager.ResetGuildNameChanging();
		return;
	}

#ifdef SERV_GLOBAL_BILLING
	// 해외용 빌링에 전달
	KEBILL_PICK_UP_REQ kPacket;
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_iTransNo		  = kPacket_.m_ulOrderNo;
	kPacket.m_iProductNo	  = kPacket_.m_ulProductNo;
	kPacket.m_byteProductKind = 0;
	SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacket );
#else //SERV_GLOBAL_BILLING
	// 넥슨 빌링 서버에 전달
	KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ kPacket;
	kPacket.m_ulPacketNo	  = SiKNexonBillingTCPManager()->GetNextPacketNo();
	kPacket.m_bytePacketType  = KNexonBillingTCPPacket::NISMS_INVENTORY_PICK_UP_ONCE;
	kPacket.m_ulOrderNo		  = kPacket_.m_ulOrderNo;
	kPacket.m_ulProductNo	  = kPacket_.m_ulProductNo;
	kPacket.m_usOrderQuantity = kPacket_.m_iQuantity;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_NX_BILLING_TCP, anTrace, ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ, kPacket );
	SiKNexonBillingTCPManager()->QueueingEvent( spEvent );
#endif //SERV_GLOBAL_BILLING
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_ACK )
{
	m_kUserGuildManager.ResetGuildNameChanging();

	KEGS_CHANGE_GUILD_NAME_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iOK == NetError::ERR_GUILD_52 )
		{
			REMOVE_VERIFY_STATE_REPEAT_FILTER( EGS_CHANGE_GUILD_NAME_REQ, EGS_CHANGE_GUILD_NAME_ACK );
		}
		else
		{
			SendPacket( EGS_CHANGE_GUILD_NAME_ACK, kPacket );
		}
		return;
	}

	// GameServer의 Guild Name 갱신
	m_kUserGuildManager.SetGuildName( kPacket_.m_kGuildInfo.m_wstrGuildName );

	// 혹시나 해서 검사해봄
	if( m_kUserGuildManager.GetGuildUID() != kPacket_.m_kGuildInfo.m_iGuildUID )
	{
		START_LOG( cerr, L"GuildUID 정보가 일치하지 않습니다. ( 조작 가능성 있음 )" )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< BUILD_LOG( m_kUserGuildManager.GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_wstrGuildName )
			<< END_LOG;

		// 그냥 넘어 가자!
	}

	// 클라이언트로 길드 이름 변경에 대한 응답을 보낸다.
	kPacket.m_kGuildInfo = kPacket_.m_kGuildInfo;
	SendPacket( EGS_CHANGE_GUILD_NAME_ACK, kPacket );

	// DB에 변경된 길드 정보를 GuildManager에게 업데이트 하러 가자!
	KELG_CHANGE_GUILD_NAME_RESULT_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = kPacket_.m_kGuildInfo.m_iGuildUID;
	kPacketToLg.m_wstrGuildName = kPacket_.m_kGuildInfo.m_wstrGuildName;

	SendToLoginServer( ELG_CHANGE_GUILD_NAME_RESULT_REQ, kPacketToLg );

	// 길드명 변경 공지
	KEGS_NOTIFY_MSG_NOT	kPacketNot;
	kPacketNot.m_cNotifyType = KEGS_NOTIFY_MSG_NOT::NT_GUILD_NAME_CHANGED;
	kPacketNot.m_Count = 1;		

	CStringW cwstrMessage;
	cwstrMessage.Format( L"%s;%s", kPacket_.m_wstrOldGuildName.c_str(), kPacket_.m_kGuildInfo.m_wstrGuildName.c_str() );
	kPacketNot.m_wstrMSG = static_cast<LPCTSTR>(cwstrMessage);

	SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacketNot );

	// 길드 이름 변경 성공했으니 길드 이름 갱신해주자!
	if( GetStateID() == KGSFSM::S_FIELD_MAP )
	{
		// 필드에 있을경우 유저 정보 갱신하기
		if( GetFieldUID() > 0 )
		{
			KERM_CHANGE_FIELD_USER_INFO_NOT kNot;
			GetFieldUserInfo( kNot.m_kInfo );
			SendToGSField( ERM_CHANGE_FIELD_USER_INFO_NOT, kNot );
		}
	}		
	else if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KRoomUserInfo kRoomUserInfo;
				GetRoomUserInfo( kRoomUserInfo );
				SendToCnRoom( ERM_CHANGE_ROOM_USER_INFO_NOT, kRoomUserInfo );
			}
			break;

		case CXSLRoom::RT_SQUARE:
			{
				KSquareUserInfo kSquareUserInfo;
				GetSquareUserInfo( kSquareUserInfo );
				SendToCnRoom( ERM_CHANGE_SQUARE_USER_INFO_NOT, kSquareUserInfo );
			}
			break;

		default:
			{
				START_LOG( cerr, L"이상한 방 타입." )
					<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;
			}
			break;
		}
	}
}
#endif SERV_GUILD_CHANGE_NAME
//}}

#endif GUILD_TEST
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( EGS_GET_GUILD_SKILL_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_GUILD_SKILL_REQ, EGS_GET_GUILD_SKILL_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	KEGS_GET_GUILD_SKILL_ACK kPacketAck;

	// 길드 소속인지 검사
	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드 소속이 아닌데 길드스킬을 배우려고한다." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iGuildSkillID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_GET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// 길드 마스터 등급인지 검사
	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"길드마스터 등급이 아닌데 길드스킬을 배우려고한다." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_GUILD_SKILL_00;
		SendPacket( EGS_GET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	KELG_GET_GUILD_SKILL_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iGuildSkillID = kPacket_.m_iGuildSkillID;
	SendToLoginServer( ELG_GET_GUILD_SKILL_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_ACK, KEGS_GET_GUILD_SKILL_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_GET_GUILD_SKILL_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_GET_GUILD_SKILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_NOT, KEGS_GET_GUILD_SKILL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 배운 길드 스킬을 UserGuildManager에 등록하자!
	m_kUserGuildManager.UpdateGuildSkill( kPacket_ );

	// 클라이언트에 획득 스킬 정보 전송
	SendPacket( EGS_GET_GUILD_SKILL_NOT, kPacket_ );

	// 만약에 방안이라면 방안의 다른 유저들에게 해당 정보를 브로드캐스팅 하자!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_RESET_GUILD_SKILL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESET_GUILD_SKILL_REQ, EGS_RESET_GUILD_SKILL_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_RESET_GUILD_SKILL_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// 길드 소속인지 검사
		if( GetGuildUID() <= 0 )
		{
			START_LOG( cerr, L"길드 소속이 아닌데 길드스킬 되돌리기를 하려고 한다." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iGuildSkillID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_19;
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}

		// 길드 마스터 등급인지 검사
		if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
		{
			START_LOG( cerr, L"길드마스터 등급이 아닌데 길드스킬 되돌리기를 하려고 한다." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_07;
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}

		// 인벤토리에 길드 스킬 되돌리기 메달이 있는지 검사		
		if( m_kInventory.ResetGuildSkillItemCheckOnly() == false )
		{
			START_LOG( cerr, L"길드 스킬 초기화 아이템이 인벤토리에 없음." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iGuildSkillID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_08;
			SendPacket( EGS_RESET_GUILD_SKILL_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KELG_RESET_GUILD_SKILL_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iGuildSkillID = kPacket_.m_iGuildSkillID;
	SendToLoginServer( ELG_RESET_GUILD_SKILL_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_ACK, KEGS_RESET_GUILD_SKILL_ACK )
{
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_RESET_GUILD_SKILL_ACK );

	SendPacket( EGS_RESET_GUILD_SKILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_DEL_ITEM_REQ, KDBE_RESET_GUILD_SKILL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 길드 스킬 되돌리기 메달을 삭제하자!
	if( m_kInventory.ResetGuildSkillItem( kPacket_.m_vecUpdatedInventorySlot ) == false )
	{
		START_LOG( cerr, L"길드 스킬 초기화 아이템이 인벤토리에 없음. 미리 검사했는데.." )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( GetCharUID() )			
			<< BUILD_LOG( kPacket_.m_iGuildSkillID )
			<< END_LOG;

		KEGS_RESET_GUILD_SKILL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GUILD_SKILL_08;
		SendPacket( EGS_RESET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// 삭제 성공하면 DB로가서 삭제된 아이템정보를 업데이트하고 길드 스킬 초기화 정보도 업데이트하자!
	m_kInventory.FlushQuantityChange( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_RESET_GUILD_SKILL_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_RESET_GUILD_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	}

	KEGS_RESET_GUILD_SKILL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_RESET_GUILD_SKILL_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_NOT, KEGS_RESET_GUILD_SKILL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 유저 길드 매니저에 적용
	if( m_kUserGuildManager.ResetGuildSkill( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 스킬 삭제 실패!" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;
		return;
	}

	SendPacket( EGS_RESET_GUILD_SKILL_NOT, kPacket_ );

	// 만약에 방안이라면 방안의 다른 유저들에게 해당 정보를 브로드캐스팅 하자!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( EGS_INIT_GUILD_SKILL_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INIT_GUILD_SKILL_REQ, EGS_INIT_GUILD_SKILL_ACK );

	//////////////////////////////////////////////////////////////////////////
	// 예외처리
	{
		KEGS_INIT_GUILD_SKILL_ACK kAck;

		//{{ 2012. 03. 29	최육사	Inventory Lock 기능
#ifdef SERV_INVENTORY_LOCK
		if( m_kInventory.IsLocked() == true )
		{
			START_LOG( cout, L"인벤토리 락이 걸려있는 상태입니다!" )
#ifndef SERV_PRIVACY_AGREEMENT			
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() );				

			kAck.m_iOK = NetError::ERR_INVENTORY_LOCK_00;
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}
#endif SERV_INVENTORY_LOCK
		//}}

		// 길드 소속인지 검사
		if( GetGuildUID() <= 0 )
		{
			START_LOG( cerr, L"길드 소속이 아닌데 길드스킬 초기화를 하려고 한다." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )			
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_19;
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}

		// 길드 마스터 등급인지 검사
		if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER )
		{
			START_LOG( cerr, L"길드마스터 등급이 아닌데 길드스킬 초기화를 하려고 한다." )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_11;
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}

		// 인벤토리에 길드 스킬 초기화 아이템이 있는지 검사
		if( m_kInventory.InitGuildSkillItemCheckOnly( kPacket_.m_iItemUID ) == false )
		{
			START_LOG( cerr, L"스킬초기화 아이템이 없는데 패킷 날아옴.!" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUILD_SKILL_12;
			SendPacket( EGS_INIT_GUILD_SKILL_ACK, kAck );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	KELG_INIT_GUILD_SKILL_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iItemUID = kPacket_.m_iItemUID;
	SendToLoginServer( ELG_INIT_GUILD_SKILL_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_ACK, KEGS_INIT_GUILD_SKILL_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_INIT_GUILD_SKILL_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_INIT_GUILD_SKILL_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_DEL_ITEM_REQ, KDBE_INIT_GUILD_SKILL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 길드 스킬 초기화 아이템을 삭제하자!
	if( m_kInventory.InitGuildSkillItem( kPacket_.m_iItemUID, kPacket_.m_vecUpdatedInventorySlot ) == false )
	{
		KEGS_INIT_GUILD_SKILL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GUILD_SKILL_12;
		SendPacket( EGS_INIT_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// 삭제 성공하면 DB로가서 삭제된 아이템정보를 업데이트하고 길드 스킬 초기화 정보도 업데이트하자!
	m_kInventory.FlushQuantityChange( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.FlushDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );

	SendToGameDB( DBE_INIT_GUILD_SKILL_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_INIT_GUILD_SKILL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
		m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	}

	KEGS_INIT_GUILD_SKILL_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecUpdatedInventorySlot;
	SendPacket( EGS_INIT_GUILD_SKILL_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_NOT, KEGS_INIT_GUILD_SKILL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 유저 길드 매니저에 적용
	m_kUserGuildManager.InitGuildSkill();

	// 클라이언트에 결과 전송
	SendPacket( EGS_INIT_GUILD_SKILL_NOT, kPacket_ );

	// 만약에 방안이라면 방안의 다른 유저들에게 해당 정보를 브로드캐스팅 하자!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

_IMPL_ON_FUNC( ELG_EXPIRE_GUILD_CASH_SKILL_POINT_NOT, KEGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 유저 길드 매니저에 적용
	m_kUserGuildManager.ExpireGuildSkill( kPacket_ );

	// 클라이언트에 결과 전송
	SendPacket( EGS_EXPIRE_GUILD_CASH_SKILL_POINT_NOT, kPacket_ );

	// 만약에 방안이라면 방안의 다른 유저들에게 해당 정보를 브로드캐스팅 하자!
	if( GetStateID() == KGSFSM::S_ROOM )
	{
		switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
		{
		case CXSLRoom::RT_PVP:
		case CXSLRoom::RT_DUNGEON:
		case CXSLRoom::RT_TRAININGCENTER:
			{
				KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacketNot;
				GetRoomUserInfo( kPacketNot );
				SendToCnRoom( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacketNot );
			}
			break;
		}
	}
}

IMPL_ON_FUNC( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_CHANGE_SKILL_INFO_IN_ROOM_NOT kPacket;
	kPacket.m_iUnitUID = kPacket_.m_nUnitUID;
	kPacket.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;
	kPacket.m_kGameStat = kPacket_.m_kGameStat;
	kPacket.m_UnitSkillData = kPacket_.m_UnitSkillData;
	SendPacket( EGS_CHANGE_SKILL_INFO_IN_ROOM_NOT, kPacket );
}

#endif GUILD_SKILL_TEST

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 31  최육사	길드 광고
#ifdef SERV_GUILD_AD

IMPL_ON_FUNC( EGS_GET_GUILD_AD_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KELG_GET_GUILD_AD_LIST_REQ kPacket;
	kPacket.m_iGuildUID = GetGuildUID();
	kPacket.m_iUnitUID = GetCharUID();
	kPacket.m_cSortType = kPacket_.m_cSortType;
	kPacket.m_uiViewPage = kPacket_.m_uiViewPage;    
	SendToLoginServer( ELG_GET_GUILD_AD_LIST_REQ, kPacket );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_AD_LIST_ACK, KEGS_GET_GUILD_AD_LIST_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_GET_GUILD_AD_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_GUILD_SKILL_IN_BOARD_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// 체험ID 검사
	if( IsGuestUser() )
	{
		KEGS_GET_GUILD_SKILL_IN_BOARD_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_GET_GUILD_SKILL_IN_BOARD_ACK, kPacketAck );
		return;
	}

	KELG_GET_GUILD_SKILL_IN_BOARD_REQ kPacket;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_iUnitUID = GetCharUID();
	SendToLoginServer( ELG_GET_GUILD_SKILL_IN_BOARD_REQ, kPacket );
}

_IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_IN_BOARD_ACK, KEGS_GET_GUILD_SKILL_IN_BOARD_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_GET_GUILD_SKILL_IN_BOARD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_REGISTRATION_GUILD_AD_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REGISTRATION_GUILD_AD_REQ, EGS_REGISTRATION_GUILD_AD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_REGISTRATION_GUILD_AD_ACK kPacket;

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrAdMessage ) == false )
	{
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_04;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	// 체험ID 검사
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// 문자열 검사
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrAdMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드 광고를 신청하려고 합니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
		m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"길드 광고를 등록할 수 없는 길드원등급입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// 광고 문자열 길이 검사
	std::string strAdMessage = KncUtil::toNarrowString( kPacket_.m_wstrAdMessage );
	if( strAdMessage.size() > 80 )
	{
		START_LOG( cerr, L"광고 글자 길이가 너무 깁니다. 클라에서 체크했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_wstrAdMessage )
			<< BUILD_LOG( strAdMessage.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_02;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// 요청 가능한 기간인지 검사 ( 광고 등록 기간 1일 ~ 5일 가능 )
	if( kPacket_.m_sPeriod < 0  ||  kPacket_.m_sPeriod > 5 )
	{
		START_LOG( cerr, L"요청 가능한 광고 등록 기간이 아닙니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_03;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// 수수료가 충분히 있는지 검사
	const int iCost = kPacket_.m_sPeriod * 3000; // 하루에 3000ED
	if( iCost > GetED() )
	{
		START_LOG( clog, L"수수료가 부족해서 광고 등록을 할 수 없습니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< BUILD_LOG( iCost )
			<< BUILD_LOG( GetED() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_04;
		SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iCost, KUserEDManager::ER_OUT_ED_REG_GUILD_AD_COMMISSION );
#else
	const int iBeforeED = m_iED;

	m_iED -= iCost;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	
	// 유저 통계
	//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );

	// 로그인서버로 가서 길드 광고 등록 신청
	KELG_REGISTRATION_GUILD_AD_REQ kPacetToLg;
	kPacetToLg.m_iUnitUID = GetCharUID();
	kPacetToLg.m_iGuildUID = GetGuildUID();
	kPacetToLg.m_sPeriod = kPacket_.m_sPeriod;
	kPacetToLg.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	kPacetToLg.m_iCost = iCost;
	SendToLoginServer( ELG_REGISTRATION_GUILD_AD_REQ, kPacetToLg );
}

IMPL_ON_FUNC( ELG_REGISTRATION_GUILD_AD_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_REGISTRATION_GUILD_AD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		// 수수료 롤백이므로 증가시켜야한다.
		IncreaseED( kPacket_.m_iCost, KUserEDManager::ER_IN_ED_REG_GUILD_AD_COMMISSION_ROLLBACK );
#else
		const int iBeforeED = m_iED;

		m_iED += kPacket_.m_iCost;

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST_ROLLBACK, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}

		// 유저 통계
		//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );
		//}}

	}

	KEGS_REGISTRATION_GUILD_AD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iED = GetED();
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	SendPacket( EGS_REGISTRATION_GUILD_AD_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_MODIFY_REG_GUILD_AD_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_MODIFY_REG_GUILD_AD_REQ, EGS_MODIFY_REG_GUILD_AD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_MODIFY_REG_GUILD_AD_ACK kPacket;

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrAdMessage ) == false )
	{
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_04;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	// 문자열 검사
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrAdMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드 광고를 신청하려고 합니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
		m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"길드 광고를 등록할 수 없는 길드원등급입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// 광고 문자열 길이 검사
	std::string strAdMessage = KncUtil::toNarrowString( kPacket_.m_wstrAdMessage );
	if( strAdMessage.size() > 80 )
	{
		START_LOG( cerr, L"광고 글자 길이가 너무 깁니다. 클라에서 체크했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_wstrAdMessage )
			<< BUILD_LOG( strAdMessage.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_02;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// 요청 가능한 기간인지 검사 ( 광고 등록 기간 1일 ~ 5일 가능 )
	if( kPacket_.m_sPeriod < 0  ||  kPacket_.m_sPeriod > 5 )
	{
		START_LOG( cerr, L"요청 가능한 광고 등록 기간이 아닙니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_03;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// 수수료가 충분히 있는지 검사
	const int iCost = kPacket_.m_sPeriod * 3000; // 하루에 3000ED
	if( iCost > GetED() )
	{
		START_LOG( clog, L"수수료가 부족해서 광고 등록을 할 수 없습니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< BUILD_LOG( iCost )
			<< BUILD_LOG( GetED() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_04;
		SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	DecreaseED( iCost, KUserEDManager::ER_OUT_ED_MODIFY_GUILD_AD_COMMISSION );
#else
	const int iBeforeED = m_iED;

	m_iED -= iCost;

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	// ED 어뷰저
	m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
	//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	
	// 유저 통계
	//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );

	// 로그인서버로 가서 길드 광고 등록 신청
	KELG_MODIFY_REG_GUILD_AD_REQ kPacetToLg;
	kPacetToLg.m_iUnitUID = GetCharUID();
	kPacetToLg.m_iGuildUID = GetGuildUID();
	kPacetToLg.m_sPeriod = kPacket_.m_sPeriod;
	kPacetToLg.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	kPacetToLg.m_iCost = iCost;
	SendToLoginServer( ELG_MODIFY_REG_GUILD_AD_REQ, kPacetToLg );
}

IMPL_ON_FUNC( ELG_MODIFY_REG_GUILD_AD_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_MODIFY_REG_GUILD_AD_ACK );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		// 수수료 비용 롤백이므로 증가 시켜야한다.
		IncreaseED( kPacket_.m_iCost, KUserEDManager::ER_IN_ED_MODIFY_GUILD_AD_COMMISSION_ROLLBACK );
#else
		const int iBeforeED = m_iED;

		m_iED += kPacket_.m_iCost;

		//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
		// ED 어뷰저
		m_kUserAbuserManager.CheckEDAbuser( GetThisPtr<KGSUser>(), KAbuserLogManager::ELS_GUILD_AD_COST_ROLLBACK, iBeforeED );
#endif SERV_USER_ABUSER_MANAGER
		//}}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
		//}}
		// 유저 통계
		//m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_MManufactureED, iED );
		// ED통계 수수료
	}

	KEGS_MODIFY_REG_GUILD_AD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_iED = GetED();
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	SendPacket( EGS_MODIFY_REG_GUILD_AD_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_APPLY_JOIN_GUILD_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_APPLY_JOIN_GUILD_REQ, EGS_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_APPLY_JOIN_GUILD_ACK kPacket;

#ifdef SERV_STRING_FILTER_USING_DB
	if( GetAuthLevel() < SEnum::UAL_GM && SiKStringFilterManager()->CheckIsValidString( CXSLStringFilter::FT_CHAT, kPacket_.m_wstrMessage ) == false )
	{
		kPacket.m_iOK = NetError::ERR_STRING_FILTER_04;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}
#endif //SERV_STRING_FILTER_USING_DB

	// 체험ID 검사
	if( IsGuestUser() )
	{
		kPacket.m_iOK = NetError::ERR_GUEST_USER_00;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// 문자열 검사
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrMessage ) )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// 가입 메시지 길이 검사
	std::string strMessage = KncUtil::toNarrowString( kPacket_.m_wstrMessage );
	if( strMessage.size() > 80 )
	{
		START_LOG( cerr, L"가입 메시지 글자 길이가 너무 깁니다. 클라에서 체크했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOG( kPacket_.m_wstrMessage )
			<< BUILD_LOG( strMessage.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_02;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// 이미 길드에 가입된 유저인지 체크	
	if( GetGuildUID() > 0 )
	{
		START_LOG( cerr, L"이미 길드에 가입된 유저인데 길드 가입 신청을 하려고 했음. 클라에서 검사 했을텐데.." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_15;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// 길드 가입 신청 거부 체크 했는지 검사
	if( m_kUserGameOption.IsDenyInviteGuild() == true )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_15;
		SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	KELG_APPLY_JOIN_GUILD_REQ kPacketToLg;
	GetApplyJoinGuildInfo( kPacketToLg.m_kApplyJoinGuildInfo );
	kPacketToLg.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID = kPacket_.m_iGuildUID;
	kPacketToLg.m_kApplyJoinGuildInfo.m_wstrMessage = kPacket_.m_wstrMessage;
	kPacketToLg.m_bOldApplyDelete = kPacket_.m_bOldApplyDelete;
	SendToLoginServer( ELG_APPLY_JOIN_GUILD_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_APPLY_JOIN_GUILD_ACK, KEGS_APPLY_JOIN_GUILD_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}	

	SendPacket( EGS_APPLY_JOIN_GUILD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_GET_APPLY_JOIN_GUILD_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KELG_GET_APPLY_JOIN_GUILD_LIST_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_uiViewPage = kPacket_.m_uiViewPage;
	SendToLoginServer( ELG_GET_APPLY_JOIN_GUILD_LIST_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_GET_APPLY_JOIN_GUILD_LIST_ACK, KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_GET_APPLY_JOIN_GUILD_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_ACCEPT_JOIN_GUILD_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_ACCEPT_JOIN_GUILD_REQ, EGS_ACCEPT_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_ACCEPT_JOIN_GUILD_ACK kPacket;

	if( GetGuildUID() <= 0 )
	{
		START_LOG( cerr, L"길드원이 아닌데 길드 가입 수락을 하려고 한다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19;
		SendPacket( EGS_ACCEPT_JOIN_GUILD_ACK, kPacket );
		return;
	}

	if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
		m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"길드 가입 수락을 할 수 없는 길드원등급입니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetGuildUID() )
			<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
		SendPacket( EGS_ACCEPT_JOIN_GUILD_ACK, kPacket );
		return;
	}

	KELG_ACCEPT_JOIN_GUILD_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iApplyJoinUnitUID = kPacket_.m_iUnitUID;
	SendToLoginServer( ELG_ACCEPT_JOIN_GUILD_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_ACCEPT_JOIN_GUILD_ACK, KEGS_ACCEPT_JOIN_GUILD_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_ACCEPT_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_ACCEPT_JOIN_GUILD_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_DELETE_APPLY_JOIN_GUILD_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_DELETE_APPLY_JOIN_GUILD_REQ, EGS_DELETE_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	KEGS_DELETE_APPLY_JOIN_GUILD_ACK kPacket;

	if( GetGuildUID() > 0 )
	{
		if( m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_MASTER  &&
			m_kUserGuildManager.GetMemberShipGrade() != SEnum::GUG_SYSOP )
		{
			START_LOG( cwarn, L"길드 가입 신청 삭제를 할 수 없는 길드원등급입니다." )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetGuildUID() )
				<< BUILD_LOGc( m_kUserGuildManager.GetMemberShipGrade() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
			SendPacket( EGS_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
			return;
		}
	}

	KELG_DELETE_APPLY_JOIN_GUILD_REQ kPacketToLg;
	kPacketToLg.m_iGuildUID = GetGuildUID();
	kPacketToLg.m_iUnitUID = GetCharUID();
	kPacketToLg.m_iDeletedUnitUID = kPacket_.m_iUnitUID;
	kPacketToLg.m_bDeleteChar = false;
	SendToLoginServer( ELG_DELETE_APPLY_JOIN_GUILD_REQ, kPacketToLg );
}

_IMPL_ON_FUNC( ELG_DELETE_APPLY_JOIN_GUILD_ACK, KEGS_DELETE_APPLY_JOIN_GUILD_ACK )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_FIELD_MAP ), EGS_DELETE_APPLY_JOIN_GUILD_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	SendPacket( EGS_DELETE_APPLY_JOIN_GUILD_ACK, kPacket_ );
}

#endif SERV_GUILD_AD
//}}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


