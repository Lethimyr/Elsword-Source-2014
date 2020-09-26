#include "Field.h"

#include "FieldFSM.h"
#include "NetError.h"
#include "FieldManager.h"

//{{ 2008. 11. 11  최육사	tcp릴레이
#include "TRServer.h"
//}}
#include "X2Data/XSLRoom.h"

//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
#include "Enum/Enum.h"
#endif SERV_ED_MONITORING_IN_GAME
//}}

#define CLASS_TYPE KField

NiImplementRTTI( KField, KPerformer );
ImplPfID( KField, PI_GS_FIELD );

ImplToStringW( KField )
{
	return START_TOSTRING_PARENTW( KPerformer );
}

KField::KField(void)
:
m_iMapID( 0 )
{
	m_kFieldUserManager.Init( 3000 );
}

KField::~KField(void)
{
}

void KField::Tick()
{
	KPerformer::Tick();

	switch( GetStateID() )
	{
	case KFieldFSM::S_INIT:
		break;

	case KFieldFSM::S_WAIT:
		break;

	case KFieldFSM::S_CLOSE:
		{
			CloseField();
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

void KField::CloseField()
{	
	LIF( GetKFieldManager()->CloseField( GetUID() ) );
	return;
}

void KField::SetFieldInfo( int iMapID, const KTileInfo& kTileInfo, const KMapInfo& kMapInfo )
{
	m_iMapID = iMapID;
	m_kFieldUserManager.SetTileInfo( kTileInfo );
	m_kFieldUserManager.SetMapInfo( kMapInfo );
}

//{{ 2009. 4. 20  최육사	좀비체크
void KField::CheckZombieUser( UidType nCID )
{
	if( m_kFieldUserManager.IsExistUser( nCID ) == false )
	{
		// 좀비 유저 없음!
		return;
	}

	KFieldUserPtr spZombieUser = m_kFieldUserManager.GetUser( nCID );
	if( spZombieUser == NULL )
	{
		START_LOG( cerr, L"존재하는 유저인지 미리 체크했는데 유저포인터 얻기 실패했네?" )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return;
	}

	KFieldUserInfo kZombieUserInfo;
	spZombieUser->GetFieldUserInfo( kZombieUserInfo );

	int	iX = KSectorID::ExtractX( spZombieUser->GetMySectorID() );
	int	iY = KSectorID::ExtractY( spZombieUser->GetMySectorID() );
	int	iZ = KSectorID::ExtractZ( spZombieUser->GetMySectorID() );

	START_LOG( cout, L"필드에 좀비유저가 있다!" )
		<< BUILD_LOG( spZombieUser.use_count() )
		<< BUILD_LOG( kZombieUserInfo.m_iUnitUID )
		<< BUILD_LOG( kZombieUserInfo.m_wstrNickName )
		<< BUILD_LOGc( kZombieUserInfo.m_cUnitClass )
		<< BUILD_LOGc( kZombieUserInfo.m_ucLevel )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		<< BUILD_LOGc( kZombieUserInfo.m_cRank )
#else
		<< BUILD_LOG( kZombieUserInfo.m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		<< BUILD_LOGc( kZombieUserInfo.m_cPVPEmblem )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		<< BUILD_LOG( kZombieUserInfo.m_vecEquippedItem.size() )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		<< BUILD_LOG( kZombieUserInfo.m_iTitleID )
#else
		<< BUILD_LOG( kZombieUserInfo.m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}		
		<< BUILD_LOG( kZombieUserInfo.m_vPos.x )
		<< BUILD_LOG( kZombieUserInfo.m_vPos.y )
		<< BUILD_LOG( kZombieUserInfo.m_vPos.z )
		<< BUILD_LOG( kZombieUserInfo.m_iPartyUID )
		<< BUILD_LOG( iX )
		<< BUILD_LOG( iY )
		<< BUILD_LOG( iZ );

	KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToOtherNot;
	if( !m_kFieldUserManager.LeaveField( nCID, kChangeToOtherNot.m_vecUID ) )
	{
		START_LOG( cerr, L"좀비유저 이탈처리 실패." )
			<< BUILD_LOG( nCID )
			<< END_LOG;
		return;
	}
	
	kChangeToOtherNot.m_vecDelUserList.push_back( nCID );
	if( !kChangeToOtherNot.m_vecUID.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToOtherNot );
	}
}
//}}

void KField::SendToServer( unsigned short usEventID )
{
	SendToServer( usEventID, char() );
}

void KField::SendToCharacter( UidType nTo, unsigned short usEventID )
{
	SendToCharacter( nTo, usEventID, char() );
}

//void KField::_BroadCastGSUser( const std::vector< UidType >& vecUnitList, const KEvent& kEvent_ )
//{
//	KEventPtr spEvent( kEvent_.Clone() );
//	spEvent->m_anTrace[0] = -1; // UnitUID로 목적지 못찾으면 그다음으로 보낼목적지 : 일단 -1로 설정
//	spEvent->m_anTrace[1] = -1;
//	
//	// User 각각에 대한 loop
//	std::vector< UidType >::const_iterator vit;
//	for( vit = vecUnitList.begin(); vit != vecUnitList.end(); ++vit )
//	{
//		LIF( spEvent->m_kDestPerformer.AddUID( *vit ) );
//	}	
//
//	KncSend( PI_GS_FIELD, GetUID(), spEvent );
//}

void KField::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();  // 패킷 처리 소요시간을 구한다.

	switch( spEvent_->m_usEventID )
	{
	CASE_NOPARAM( ERM_OPEN_FIELD_NOT );
	CASE( ERM_JOIN_FIELD_REQ );
	//{{ 2009. 6. 4  최육사		채널이동
   _CASE( ERM_JOIN_FIELD_CHANNEL_CHANGE_REQ, KERM_JOIN_FIELD_REQ );
	//}}

	CASE( ERM_LEAVE_FIELD_REQ );
   _CASE( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT );
	CASE( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT );	

	CASE( ERM_OPTION_UPDATE_NOT );	
	CASE( ERM_UPDATE_PARTY_NOT );

   _CASE( ERM_CHAT_REQ, KEGS_CHAT_REQ );

	//{{ 2008. 12. 13  최육사	개인거래
	CASE( ERM_REQUEST_TRADE_NOT );
	CASE( ERM_REQUEST_TRADE_REPLY_NOT );
	//}}

	//{{ 2008. 12. 31  최육사	
	CASE_NOPARAM( DMP_DUMP_EVENT_NOT );
	//}}
	//{{ 2009. 1. 15  최육사	유저 리스트
   _CASE( ERM_FIELD_USER_LIST_REQ, KEGS_COMMUNITY_USER_LIST_REQ );
	//}}
	//{{ 2009. 4. 14  최육사	친구리스트
	CASE( ERM_UPDATE_FRIEND_NOT );
	//}}
	//{{ 2009. 9. 23  최육사	1차전직캐쉬템
	CASE( ERM_CHANGE_FIELD_USER_INFO_NOT );
	//}}
	//{{ 2010. 7. 26  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	CASE( ERM_SUMMON_PET_NOT );
	CASE( ERM_PET_ACTION_NOT );
	CASE( ERM_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	CASE( ERM_FIELD_USER_LIST_MONITORING_MODE_REQ );
#endif SERV_ED_MONITORING_IN_GAME
	//}}

#ifdef SERV_INVISIBLE_GM
	CASE( ERM_TOGGLE_INVISIBLE_NOT );
#endif SERV_INVISIBLE_GM

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	CASE( ERM_COUPLE_MAKING_SUCCESS_NOT );
	CASE_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT );
	_CASE( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT );
	CASE( ERM_CALL_MY_LOVER_LEAVE_FIELD_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	_CASE( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT );
	_CASE( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM

	default:
		START_LOG( cerr, L"없는 패킷이 필드에 날아온다 ;ㅁ;" )
			<< BUILD_LOG( KEvent::GetIDStr( spEvent_->m_usEventID ) )
			<< END_LOG;
		break;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}
}

IMPL_ON_FUNC_NOPARAM( ERM_OPEN_FIELD_NOT )
{
	if( GetStateID() != KFieldFSM::S_INIT )
	{
		START_LOG( cerr, L"필드가 초기 상태가 아님." )
			<< BUILD_LOG( GetStateID() )
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;
		return;
	}

	StateTransition( KFieldFSM::I_TO_WAIT ); // 초기 상태에서 대기 상태로 변경

	// 생성된 필드 정보를 GSSimLayer에서 참조할 수 있도록 업데이트한다.
	KERM_UPDATE_FIELD_INFO_NOT kPacketNot;
	kPacketNot.m_iMapID	   = m_iMapID;
	kPacketNot.m_iFieldUID = GetUID();
	SendToServer( ERM_UPDATE_FIELD_INFO_NOT, kPacketNot );

	//m_kSquareUserManager.ZU_CheckStart();
}

IMPL_ON_FUNC( ERM_JOIN_FIELD_REQ )
{
	VERIFY_FIELD_STATE( ( 1, KFieldFSM::S_WAIT ), ERM_JOIN_FIELD_ACK, KERM_JOIN_FIELD_ACK );

	// FieldUID 검사
	if( kPacket_.m_iFieldUID != GetUID() )
	{
		START_LOG( cerr, L"있을수 없는 에러! FieldUID가 다르다니!?" )
			<< BUILD_LOG( kPacket_.m_iFieldUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK	= NetError::ERR_FIELD_00;
		SendToCharacter( FIRST_SENDER_UID, ERM_JOIN_FIELD_ACK, kPacket );
		return;
	}

	//{{ 2009. 4. 20  최육사	좀비유저
	CheckZombieUser( FIRST_SENDER_UID );
	//}}

    KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToThisNot;
    KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToOtherNot;
	if( !m_kFieldUserManager.EnterField( kPacket_, kChangeToOtherNot.m_vecUID, kChangeToThisNot.m_vecNewUserList ) )
	{
		kPacket.m_iOK	= NetError::ERR_FIELD_00; // 필드 입장 실패!
		SendToCharacter( FIRST_SENDER_UID, ERM_JOIN_FIELD_ACK, kPacket );
		return;
	}

	kPacket.m_iOK		= NetError::NET_OK;
	kPacket.m_iFieldUID = GetUID();
	SendToCharacter( FIRST_SENDER_UID, ERM_JOIN_FIELD_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
    kChangeToThisNot.m_vecUID.push_back( FIRST_SENDER_UID );
    kChangeToOtherNot.m_vecNewUserList.push_back( kPacket_.m_kFieldUserInfo );
    if( !kChangeToThisNot.m_vecNewUserList.empty() )
    {
        SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToThisNot );
    }
    if( !kChangeToOtherNot.m_vecUID.empty() )
    {
        SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToOtherNot );
    }
	//////////////////////////////////////////////////////////////////////////
}

//{{ 2009. 6. 4  최육사		채널이동
_IMPL_ON_FUNC( ERM_JOIN_FIELD_CHANNEL_CHANGE_REQ, KERM_JOIN_FIELD_REQ )
{
	VERIFY_FIELD_STATE( ( 1, KFieldFSM::S_WAIT ), ERM_JOIN_FIELD_ACK, KERM_JOIN_FIELD_ACK );

	// FieldUID 검사
	if( kPacket_.m_iFieldUID != GetUID() )
	{
		START_LOG( cerr, L"있을수 없는 에러! FieldUID가 다르다니!?" )
			<< BUILD_LOG( kPacket_.m_iFieldUID )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;

		kPacket.m_iOK	= NetError::ERR_FIELD_00;
		SendToCharacter( FIRST_SENDER_UID, ERM_JOIN_FIELD_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}

	//{{ 2009. 4. 20  최육사	좀비유저
	CheckZombieUser( FIRST_SENDER_UID );
	//}}

	KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToThisNot;
	KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToOtherNot;
	if( !m_kFieldUserManager.EnterField( kPacket_, kChangeToOtherNot.m_vecUID, kChangeToThisNot.m_vecNewUserList ) )
	{
		kPacket.m_iOK	= NetError::ERR_FIELD_00; // 필드 입장 실패!
		SendToCharacter( FIRST_SENDER_UID, ERM_JOIN_FIELD_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}

	kPacket.m_iOK		= NetError::NET_OK;
	kPacket.m_iFieldUID = GetUID();
	SendToCharacter( FIRST_SENDER_UID, ERM_JOIN_FIELD_CHANNEL_CHANGE_ACK, kPacket );

	//////////////////////////////////////////////////////////////////////////
	kChangeToThisNot.m_vecUID.push_back( FIRST_SENDER_UID );
	kChangeToOtherNot.m_vecNewUserList.push_back( kPacket_.m_kFieldUserInfo );
	if( !kChangeToThisNot.m_vecNewUserList.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToThisNot );
	}
	if( !kChangeToOtherNot.m_vecUID.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToOtherNot );
	}
	//////////////////////////////////////////////////////////////////////////
}
//}}

IMPL_ON_FUNC( ERM_LEAVE_FIELD_REQ )
{
	VERIFY_FIELD_STATE( ( 1, KFieldFSM::S_WAIT ), ERM_LEAVE_FIELD_ACK, KERM_LEAVE_FIELD_ACK );

	// 해당 유저 FieldUserManager에 Leave처리
	kPacket.m_iOK			  = NetError::NET_OK;
	kPacket.m_iReason		  = kPacket_.m_iReason;	

    KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToOtherNot;
	if( !m_kFieldUserManager.LeaveField( FIRST_SENDER_UID, kChangeToOtherNot.m_vecUID ) )
	{
		START_LOG( cerr, L"필드 나가기 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_FIELD_01;
		SendToCharacter( FIRST_SENDER_UID, ERM_LEAVE_FIELD_ACK, kPacket );
		return;
	}

	if( kPacket_.m_iReason == NetError::NOT_LEAVE_FIELD_REASON_00 )
	{
		kPacket.m_iMapID = kPacket_.m_iMapID;
		SendToCharacter( FIRST_SENDER_UID, ERM_LEAVE_FIELD_ACK, kPacket );
	}

	//////////////////////////////////////////////////////////////////////////
    kChangeToOtherNot.m_vecDelUserList.push_back( FIRST_SENDER_UID );
    if( !kChangeToOtherNot.m_vecUID.empty() )
    {
        SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToOtherNot );
    }
	//////////////////////////////////////////////////////////////////////////
}

_IMPL_ON_FUNC( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

    // 다른 섹터로 이동 체크
    KFieldUserInfo kFieldUserInfoOfThis;
    KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToThisNot;
    KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kNewToOtherNot;
    KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kDelToOtherNot;
	m_kFieldUserManager.MoveUser( FIRST_SENDER_UID, kPacket_.m_vPos, kFieldUserInfoOfThis, kNewToOtherNot.m_vecUID, kDelToOtherNot.m_vecUID, kChangeToThisNot.m_vecNewUserList, kChangeToThisNot.m_vecDelUserList );

	//////////////////////////////////////////////////////////////////////////
    kChangeToThisNot.m_vecUID.push_back( FIRST_SENDER_UID );
    kNewToOtherNot.m_vecNewUserList.push_back( kFieldUserInfoOfThis );
    kDelToOtherNot.m_vecDelUserList.push_back( FIRST_SENDER_UID );
	if( !kChangeToThisNot.m_vecNewUserList.empty()  ||  !kChangeToThisNot.m_vecDelUserList.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToThisNot );
	}
	if( !kNewToOtherNot.m_vecUID.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kNewToOtherNot );
	}
	if( !kDelToOtherNot.m_vecUID.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kDelToOtherNot );
	}
	//////////////////////////////////////////////////////////////////////////

	KETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT kPacketNot;
	kPacketNot.m_UnitUID = FIRST_SENDER_UID;
	kPacketNot.m_vPos = kPacket_.m_vPos;
	kPacketNot.m_vecFieldFrame = kPacket_.m_vecFieldFrame;
	kPacketNot.m_bIsRight = kPacket_.m_bIsRight;	
	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketNot.m_vecUID ) );
    if( !kPacketNot.m_vecUID.empty() )
    {
        SendToTR( ETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT, kPacketNot );
    }
}

IMPL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	m_kFieldUserManager.SetUserEquippedTitle( kPacket_.m_iUnitUID, kPacket_.m_iEquippedTitleID );
#else
	m_kFieldUserManager.SetUserEquippedTitle( kPacket_.m_iUnitUID, kPacket_.m_sEquippedTitleID );
#endif SERV_TITLE_DATA_SIZE
	//}}	
	m_kFieldUserManager.SetUserEquipItem( kPacket_.m_iUnitUID, kPacket_.m_vecInventorySlotInfo );

	// 해당 섹터에 브로드캐스팅
	KETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT kPacketNot;
	kPacketNot.m_iUnitUID			  = kPacket_.m_iUnitUID;
	kPacketNot.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	kPacketNot.m_iEquippedTitleID	  = kPacket_.m_iEquippedTitleID;
#else
	kPacketNot.m_sEquippedTitleID	  = kPacket_.m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}	

    LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketNot.m_vecUID ) );
    kPacketNot.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, kPacketNot );
}

//{{ 2008. 12. 9  최육사	인원 제한 옵션
IMPL_ON_FUNC( ERM_OPTION_UPDATE_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	// 필드 옵션 업데이트
	if( !m_kFieldUserManager.SetFieldOption( FIRST_SENDER_UID, kPacket_.m_kFieldOption ) )
	{
        START_LOG( cerr, L"필드 옵션 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ERM_UPDATE_PARTY_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

    KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToThisNot;

    // 파티 정보 업데이트
    if( !m_kFieldUserManager.SetParty( FIRST_SENDER_UID, kPacket_.m_iPartyUID, kPacket_.m_setPartyUIDList, kChangeToThisNot.m_vecNewUserList, kChangeToThisNot.m_vecDelUserList ) )
    {
        START_LOG( cerr, L"파티 정보 업데이트 실패!" )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_setPartyUIDList.size() )
            << END_LOG;
        return;
    }

    kChangeToThisNot.m_vecUID.push_back( FIRST_SENDER_UID );
    if( !kChangeToThisNot.m_vecNewUserList.empty() || !kChangeToThisNot.m_vecDelUserList.empty() )
    {
        SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToThisNot );
    }

	// 주변섹터 유저들에게 파티여부를 알림
	KETR_UPDATE_PARTY_NOT kPacketNot;
	kPacketNot.m_iUnitUID	= FIRST_SENDER_UID;
	kPacketNot.m_iPartyUID	= kPacket_.m_iPartyUID;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketNot.m_vecUID ) );
	kPacketNot.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_UPDATE_PARTY_NOT, kPacketNot );
}
//}}

_IMPL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ )
{
	VERIFY_FIELD_STATE_CHAT_ONLY( ( 1, KFieldFSM::S_WAIT ), ERM_CHAT_ACK, KERM_CHAT_ACK );

	if( kPacket_.m_cChatPacketType != KEGS_CHAT_REQ::CPT_TOTAL )
	{
		START_LOG( cerr, L"일반채팅 패킷타입이 아닌데 여기왔네?" )
			<< BUILD_LOGc( kPacket_.m_cChatPacketType )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_24;
		SendToCharacter( FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
		return;
	}

	// 보낸 사람에게 ack를 날린다.
	kPacket.m_iOK				= NetError::NET_OK;
	kPacket.m_cChatPacketType	= kPacket_.m_cChatPacketType;

	// 모든 유저에게 채팅 내용을 날린다.

	if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacket_.m_cRoomChatType == CXSLRoom::RCT_ALL )
	{
		SendToCharacter( FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );

		KETR_CHAT_NOT kPacketTR;
		kPacketTR.m_SenderUnitUID	= FIRST_SENDER_UID;
		kPacketTR.m_RecieverUnitUID	= kPacket_.m_ToUnitUID;
		kPacketTR.m_cChatPacketType = kPacket_.m_cChatPacketType;
		kPacketTR.m_cRoomChatType	= kPacket_.m_cRoomChatType;
		kPacketTR.m_wstrMsg			= kPacket_.m_wstrMsg;

		LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
		kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
		SendToTR( ETR_CHAT_NOT, kPacketTR );
	}
	else if ( (CXSLRoom::ROOM_CHAT_TYPE)kPacket_.m_cRoomChatType == CXSLRoom::RCT_WHISPER )
	{
		KEGS_CHAT_NOT kPacketWhisper;
		kPacketWhisper.m_SenderUnitUID	 = FIRST_SENDER_UID;
		kPacketWhisper.m_RecieverUnitUID = kPacket_.m_ToUnitUID;
		kPacketWhisper.m_cChatPacketType = kPacket_.m_cChatPacketType;
		kPacketWhisper.m_cRoomChatType	 = kPacket_.m_cRoomChatType;
		kPacketWhisper.m_wstrMsg		 = kPacket_.m_wstrMsg;

		if( m_kFieldUserManager.IsExistUser( kPacket_.m_ToUnitUID ) )
		{
			SendToCharacter( FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
			SendToCharacter( FIRST_SENDER_UID, ERM_CHAT_NOT, kPacketWhisper );
			SendToCharacter( kPacket_.m_ToUnitUID, ERM_CHAT_NOT, kPacketWhisper );
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_ROOM_23;
			SendToCharacter( FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
		}		
	}
	else
	{
		START_LOG( cerr, L"이상한 채팅 타입." )
			<< BUILD_LOGc( kPacket_.m_cRoomChatType )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_24;
		SendToCharacter( FIRST_SENDER_UID, ERM_CHAT_ACK, kPacket );
	}
}

//{{ 2008. 12. 13  최육사	개인거래
IMPL_ON_FUNC( ERM_REQUEST_TRADE_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );
	
	if( !m_kFieldUserManager.IsExistUser( kPacket_.m_iReceiverUID ) )
	{
		// [참고] 시간차로 발생할 수 있는 문제
		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( kPacket_.m_iSenderUID )
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< BUILD_LOG( kPacket_.m_wstrSenderNickName )
			<< END_LOG;
		return;
	}

	// 필드에서는 개인상점을 열지 못하므로 예외처리 안함.

	SendToCharacter( kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_NOT, kPacket_ );
}

IMPL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );
	
	if( !m_kFieldUserManager.IsExistUser( kPacket_.m_iReceiverUID ) )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< END_LOG;
		return;
	}

	SendToCharacter( kPacket_.m_iReceiverUID, ERM_REQUEST_TRADE_REPLY_NOT, kPacket_ );
}

//{{ 2008. 12. 31  최육사	덤프
IMPL_ON_FUNC_NOPARAM( DMP_DUMP_EVENT_NOT )
{
	dbg::cout << L"MapID  : " << m_iMapID << dbg::endl;
	dbg::cout << L"유저수 : " << m_kFieldUserManager.GetNumMember() << dbg::endl;

	m_kFieldUserManager.Dump();

	dbg::cout << L"-----------------------------------------" << dbg::endl;
}
//}}

//{{ 2009. 1. 15  최육사	유저 리스트
_IMPL_ON_FUNC( ERM_FIELD_USER_LIST_REQ, KEGS_COMMUNITY_USER_LIST_REQ )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	KEGS_COMMUNITY_USER_LIST_ACK kPacket;

	//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
	if( !m_kFieldUserManager.GetFieldUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList, kPacket_.m_bMode ) )
#else
	if( !m_kFieldUserManager.GetFieldUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList ) )
#endif SERV_ED_MONITORING_IN_GAME
	//}}
	{
		START_LOG( cerr, L"필드 유저 리스트 얻기 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_uiViewPage )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_FIELD_04;
		SendToCharacter( FIRST_SENDER_UID, ERM_FIELD_USER_LIST_ACK, kPacket );
		return;
	}

	

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_uiViewPage = kPacket_.m_uiViewPage;
	SendToCharacter( FIRST_SENDER_UID, ERM_FIELD_USER_LIST_ACK, kPacket );
}
//}}

//{{ 2009. 4. 14  최육사	친구리스트
IMPL_ON_FUNC( ERM_UPDATE_FRIEND_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToThisNot;

	// 친구 정보 업데이트	
	if( !m_kFieldUserManager.SetFriend( FIRST_SENDER_UID, kPacket_.m_setFriendUIDList, kChangeToThisNot.m_vecNewUserList, kChangeToThisNot.m_vecDelUserList ) )
	{
		START_LOG( cerr, L"친구 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_setFriendUIDList.size() )
			<< END_LOG;
		return;
	}

	kChangeToThisNot.m_vecUID.push_back( FIRST_SENDER_UID );
	if( !kChangeToThisNot.m_vecNewUserList.empty() || !kChangeToThisNot.m_vecDelUserList.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToThisNot );
	}
}
//}}

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
IMPL_ON_FUNC( ERM_CHANGE_FIELD_USER_INFO_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	// 캐릭터 정보 업데이트	
	if( !m_kFieldUserManager.SetFieldUserInfo( kPacket_.m_kInfo ) )
	{
		START_LOG( cerr, L"캐릭터 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )			
			<< END_LOG;
		return;
	}

	KETR_CHANGE_FIELD_USER_INFO_NOT kPacketTR;
	kPacketTR.m_iUnitUID		= FIRST_SENDER_UID;
	kPacketTR.m_cUnitClass		= kPacket_.m_kInfo.m_cUnitClass;
	//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
	kPacketTR.m_wstrGuildName	= kPacket_.m_kInfo.m_wstrGuildName;
#endif GUILD_TEST
	//}}

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_CHANGE_FIELD_USER_INFO_NOT, kPacketTR );
}
//}}

//{{ 2010. 7. 26  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( ERM_SUMMON_PET_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );
    
	if( m_kFieldUserManager.SetPetInfo( FIRST_SENDER_UID, kPacket_.m_vecPetInfo ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_vecPetInfo.size() )
			<< END_LOG;

		return;
	}

	// 해당 섹터에 브로드캐스팅
	KETR_SUMMON_PET_NOT kPacketTR;
	kPacketTR.m_iUnitUID		= FIRST_SENDER_UID;	
	kPacketTR.m_vecPetInfo		= kPacket_.m_vecPetInfo;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_SUMMON_PET_NOT, kPacketTR );
}

IMPL_ON_FUNC( ERM_PET_ACTION_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	KETR_PET_ACTION_NOT kPacketTR;
	kPacketTR.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacketTR.m_cActionType = kPacket_.m_cActionType;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_PET_ACTION_NOT, kPacketTR );
}

IMPL_ON_FUNC( ERM_PET_EVOLUTION_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	// petinfo 업데이트
	if( m_kFieldUserManager.SetPetInfo( FIRST_SENDER_UID, kPacket_.m_kEvolutionPetInfo ) == false )
	{
		START_LOG( cerr, L"펫 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kEvolutionPetInfo.m_iPetUID )
			<< END_LOG;

		return;
	}

	// 해당 섹터에 브로드캐스팅
	KETR_PET_EVOLUTION_NOT kPacketTR;
	kPacketTR.m_iUnitUID		= FIRST_SENDER_UID;	
	kPacketTR.m_kEvolutionPetInfo = kPacket_.m_kEvolutionPetInfo;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_PET_EVOLUTION_NOT, kPacketTR );
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
IMPL_ON_FUNC( ERM_FIELD_USER_LIST_MONITORING_MODE_REQ )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	KEGS_COMMUNITY_USER_LIST_ACK kPacket;

	m_kFieldUserManager.RefreshUser_ED( kPacket_.m_mapUnit_ED );

	if( !m_kFieldUserManager.GetFieldUserList( kPacket_.m_uiViewPage, kPacket.m_uiTotalPage, kPacket.m_vecUserList, true ) )
	{
		START_LOG( cerr, L"필드 유저 리스트 얻기 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_uiViewPage )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_FIELD_04;
		SendToCharacter( FIRST_SENDER_UID, ERM_FIELD_USER_LIST_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_uiViewPage = kPacket_.m_uiViewPage;
	SendToCharacter( FIRST_SENDER_UID, ERM_FIELD_USER_LIST_ACK, kPacket );
}
#endif SERV_ED_MONITORING_IN_GAME
//}}

#ifdef SERV_INVISIBLE_GM
IMPL_ON_FUNC( ERM_TOGGLE_INVISIBLE_NOT )
{
	KFieldUserPtr spFieldUser = m_kFieldUserManager.GetUser( kPacket_.m_iUnitUID );
	if( !spFieldUser )
	{
		START_LOG( cerr, L"유저 검색 실패." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	// 필드유저도 미리 갱신하고
	spFieldUser->SetFieldUserInfo( kPacket_.m_kFieldUserInfo );

	KETR_TOGGLE_INVISIBLE_NOT kPacketNot;
	kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacketNot.m_kFieldUserInfo = kPacket_.m_kFieldUserInfo;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketNot.m_vecUID ) );
	kPacketNot.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_TOGGLE_INVISIBLE_NOT, kPacketNot );
}
#endif SERV_INVISIBLE_GM

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
_IMPL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	if( m_kFieldUserManager.SetRidingPetInfo( kPacket_.m_iUnitUID, kPacket_.m_iRidingPetUID, kPacket_.m_usRidingPetID ) == false )
	{
		START_LOG( cerr, L"라이딩 펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_usRidingPetID )
			<< END_LOG;
		return;
	}

	// 해당 섹터에 브로드캐스팅
	KETR_SUMMON_RIDING_PET_NOT kPacketTR;
	kPacketTR.m_iUnitUID		= kPacket_.m_iUnitUID;
	kPacketTR.m_iRidingPetUID	= kPacket_.m_iRidingPetUID;
	kPacketTR.m_usRidingPetID	= kPacket_.m_usRidingPetID;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_SUMMON_RIDING_PET_NOT, kPacketTR );
}

_IMPL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT )
{
	VERIFY_STATE( ( 1, KFieldFSM::S_WAIT ) );

	if( m_kFieldUserManager.SetRidingPetInfo( kPacket_.m_iUnitUID, 0, 0 ) == false )
	{
		START_LOG( cerr, L"라이딩 펫 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		return;
	}

	// 해당 섹터에 브로드캐스팅
	KETR_UNSUMMON_RIDING_PET_NOT kPacketTR;
	kPacketTR.m_iUnitUID		= kPacket_.m_iUnitUID;	

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_UNSUMMON_RIDING_PET_NOT, kPacketTR );
}
#endif	// SERV_RIDING_PET_SYSTM

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( ERM_COUPLE_MAKING_SUCCESS_NOT )
{
	KFieldUserPtr spUser = m_kFieldUserManager.GetUser( FIRST_SENDER_UID );
	if( spUser != NULL )
	{
		spUser->SetLoverUnitUID( ( FIRST_SENDER_UID == kPacket_.m_iRequestUnitUID ) ? kPacket_.m_iAcceptUnitUID : kPacket_.m_iRequestUnitUID );
	}

	// 해당 섹터에 브로드캐스팅
	KETR_COUPLE_MAKING_SUCCESS_NOT kPacketTR;
	kPacketTR.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacketTR.m_wstrAcceptUnitName= kPacket_.m_wstrAcceptUnitName;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_COUPLE_MAKING_SUCCESS_NOT, kPacketTR );
}

IMPL_ON_FUNC_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT )
{
	KFieldUserPtr spUser = m_kFieldUserManager.GetUser( FIRST_SENDER_UID );
	if( spUser != NULL )
	{
		spUser->SetLoverUnitUID( 0 );
	}
}

_IMPL_ON_FUNC( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT )
{
	// 해당 섹터에 브로드캐스팅
	KETR_RELATIONSHIP_EFFECT_NOT kPacketTR;
	kPacketTR.m_iManUID			= kPacket_.m_iManUID;
	kPacketTR.m_iGirlUID		= kPacket_.m_iGirlUID;
	kPacketTR.m_wstrLoveWord	= kPacket_.m_wstrLoveWord;
	kPacketTR.m_iEffectIndex	= kPacket_.m_iEffectIndex;

	LIF( m_kFieldUserManager.GetBroadcastDestination( FIRST_SENDER_UID, kPacketTR.m_vecUID ) );
	kPacketTR.m_vecUID.push_back( FIRST_SENDER_UID );
	SendToTR( ETR_RELATIONSHIP_EFFECT_NOT, kPacketTR );
}

IMPL_ON_FUNC( ERM_CALL_MY_LOVER_LEAVE_FIELD_REQ )
{
	VERIFY_FIELD_STATE( ( 1, KFieldFSM::S_WAIT ), ERM_CALL_MY_LOVER_LEAVE_FIELD_ACK, KERM_CALL_MY_LOVER_LEAVE_FIELD_ACK );

	// 해당 유저 FieldUserManager에 Leave처리
	kPacket.m_iOK					= NetError::NET_OK;
	kPacket.m_iReason				= kPacket_.m_iReason;
	kPacket.m_iMapID				= kPacket_.m_iMapID;
	kPacket.m_vPos					= kPacket_.m_vPos;	
	kPacket.m_LastTouchLineIndex	= kPacket_.m_LastTouchLineIndex;
	kPacket.m_iLimitLevel			= kPacket_.m_iLimitLevel;	
	kPacket.m_iRequireDungeonID		= kPacket_.m_iRequireDungeonID;	

	KETR_CHANGE_USER_LIST_IN_SECTOR_NOT kChangeToOtherNot;
	if( !m_kFieldUserManager.LeaveField( FIRST_SENDER_UID, kChangeToOtherNot.m_vecUID ) )
	{
		START_LOG( cerr, L"필드 나가기 실패." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iReason ) )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_FIELD_01;
		SendToCharacter( FIRST_SENDER_UID, ERM_CALL_MY_LOVER_LEAVE_FIELD_ACK, kPacket );
		return;
	}

	if( kPacket_.m_iReason == NetError::NOT_LEAVE_FIELD_REASON_00 )
	{
		SendToCharacter( FIRST_SENDER_UID, ERM_CALL_MY_LOVER_LEAVE_FIELD_ACK, kPacket );
	}

	//////////////////////////////////////////////////////////////////////////
	kChangeToOtherNot.m_vecDelUserList.push_back( FIRST_SENDER_UID );
	if( !kChangeToOtherNot.m_vecUID.empty() )
	{
		SendToTR( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT, kChangeToOtherNot );
	}
	//////////////////////////////////////////////////////////////////////////
}

#endif SERV_RELATIONSHIP_SYSTEM
//}
