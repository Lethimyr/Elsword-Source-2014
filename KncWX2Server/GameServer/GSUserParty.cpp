#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "UserManager.h"
#include "GameSysVal/GameSysVal.h"

//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY
	#include "PartyListManager.h"
#else
	#include "PartyManager.h"
#endif SERV_CHANNEL_PARTY
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

#ifdef SERV_TEMP_EVENT_MANAGER
#include "X2Data/XSLTempEventManager.h"
#endif 

#include "Enum/Enum.h"
//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
#include "Socket/NetCommon.h"
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
//}}

#ifdef SERV_NEW_EVENT_TYPES
#include "GameEvent/GameEventManager.h"
#endif SERV_NEW_EVENT_TYPES

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
else


#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_PARTY_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_PARTY_LIST_ACK	kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// 1. 던전 게이트에서만 파티 리스트를 줄수있음
	if( !CXSLMapData::IsDungeonGate( GetMapID() ) )
	{
		START_LOG( cerr, L"던전게이트가 아닌데 파티리스트를 요청한다?" )
			<< BUILD_LOG( GetMapID() )
			<< BUILD_LOGc( kPacket_.m_cListType )
			<< BUILD_LOG( kPacket_.m_iID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ROOM_LIST_01;		
		goto end_proc;
	}

	// 2. 다른 마을의 파티 리스트는 볼수 없다
	switch( kPacket_.m_cListType )
	{
		//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
		case KPartyListManager::PLT_DUNGEON:
#else
		case KPartyManager::PLT_DUNGEON:
#endif SERV_CHANNEL_PARTY
			//}}	
			{
				//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
				if( kPacket_.m_iID == SEnum::DI_ELDER_HALLOWEEN_NORMAL || kPacket_.m_iID == SEnum::DI_ELDER_HALLOWEEN_HARD || kPacket_.m_iID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
				{
					// 소속 던전 게이트 검사 안함
				}
				else
#endif SERV_HALLOWEEN_DUNGEON
				//}}
								//## 1. 요청타입이 던전일 경우는 내가서있는 던전게이트 소속 던전이어야 하고
				if( CXSLMapData::GetMapID( kPacket_.m_iID ) != GetMapID() )
				{
					START_LOG( cerr, L"다른 마을의 파티리스트를 요청한다?" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetMapID() )
						<< BUILD_LOGc( kPacket_.m_cListType )
						<< BUILD_LOG( kPacket_.m_iID )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_ROOM_LIST_02;
					goto end_proc;
				}
			}
			break;

			//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
		case KPartyListManager::PLT_GATE_TOTAL:
#else
		case KPartyManager::PLT_GATE_TOTAL:
#endif SERV_CHANNEL_PARTY
			//}}	
			{
				//## 2. 요청타입이 게이트일 경우는 내가 있는 맵아이디와 같아야 한다.
				if( kPacket_.m_iID != GetMapID() )
				{
					START_LOG( cerr, L"다른 마을의 파티리스트를 요청한다?" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetMapID() )
						<< BUILD_LOGc( kPacket_.m_cListType )
						<< BUILD_LOG( kPacket_.m_iID )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_ROOM_LIST_02;
					goto end_proc;
				}
			}
			break;

			//{{ 2010. 04. 29  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
		case KPartyListManager::PLT_SECRET_DUNGEON:
			break;
#endif SERV_SECRET_HELL
			//}}

		default:
			START_LOG( cerr, L"이상한 파티리스트 타입입니다." )
				<< BUILD_LOGc( kPacket_.m_cListType )
				<< BUILD_LOG( kPacket_.m_iID )
				<< BUILD_LOG( kPacket_.m_nViewPage )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ROOM_LIST_00;
			goto end_proc;
	}

	//{{ 2010. 02. 17  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_PartyList( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_PartyList( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_nViewPage )
			<< BUILD_LOGc( kPacket_.m_cListType )
			<< BUILD_LOG( kPacket_.m_iID )
			<< END_LOG;
	}

	return;

end_proc:
	SendPacket( EGS_PARTY_LIST_ACK, kPacket );
}

//{{ 2011. 06. 20	최육사	대전 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EGS_CREATE_PARTY_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_CREATE_PARTY_ACK kAck;

	switch( kPacket_.m_cPartyType )
	{
	case KPartyInfo::PT_DUNGEON_PARTY:
		{
			if( CheckDungeonPartyCondition( kPacket_.m_iDungeonID, kPacket_.m_DifficultyLevel, kPacket_.m_cDungeonMode, kPacket_.m_bCheckLowLevel, false ) == false )
			{
				kAck.m_iOK = NetError::GetLastError();
				goto end_proc;
			}
		}
		break;

	case KPartyInfo::PT_PVP_PARTY:
		{
			if( CheckPvpPartyCondition( kPacket_.m_cPVPGameType, false ) == false )
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

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	{
		CTime tComeBackBuffEnd	= m_kComeBackUserInfo.GetComeBackBuffEnd();
		kPacket_.m_wsrtComeBackBuffEnd = tComeBackBuffEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		kPacket_.m_bComeBackBuff = m_kComeBackUserInfo.GetIsComeBackUser();
	}
#endif SERV_COME_BACK_USER_REWARD
	//}} 

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( CXSLDungeon::IsFixedMembers( kPacket_.m_iDungeonID ) == true )
	{
		kPacket_.m_maxSlot = CXSLDungeon::GetFixedMembers( kPacket_.m_iDungeonID );
	}
#endif SERV_DUNGEON_OPTION_IN_LUA

	////////////////////////////////////////////////////////////////////////// 처리
	// 파티 생성
	//{{ 2010. 02. 17  최육사	채널파티통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_CreatePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_CreateParty( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"파티생성 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"파티 생성실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr(kAck.m_iOK) )
		<< END_LOG;

	SendPacket( EGS_CREATE_PARTY_ACK, kAck );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
//IMPL_ON_FUNC( EGS_CREATE_PARTY_REQ )
//{
//	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
//
//	KEGS_CREATE_PARTY_ACK kAck;
//
//	////////////////////////////////////////////////////////////////////////// 데이터 검증
//	// DungeonID검사
//	if( ( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) ) <= 0 )
//	{
//		START_LOG( cerr, L"dungeonid값이 이상하다!" )
//			<< BUILD_LOG( kPacket_.m_iDungeonID )
//			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
//			<< END_LOG;
//	}
//
//	//던전 아이디가 정상인지 체크한다.
//	//{{ 2009. 10. 16  최육사	벨더
//	if( false == CXSLDungeon::IsRubenDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsElderDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsBesmaDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsAlteraDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsPeitaDungeon( kPacket_.m_iDungeonID )  &&		
//		//{{ 2010. 12. 29	최육사	하멜 마을 추가
//#ifdef SERV_SHIP_DUNGEON
//		false == CXSLDungeon::IsVelderDungeon( kPacket_.m_iDungeonID )  &&
//		false == CXSLDungeon::IsHamelDungeon( kPacket_.m_iDungeonID )
//#else
//		false == CXSLDungeon::IsVelderDungeon( kPacket_.m_iDungeonID )
//#endif SERV_SHIP_DUNGEON
//		//}}
//		)
//	{
//		kAck.m_iOK = NetError::ERR_PARTY_22;
//		goto end_proc;
//	}
//	//}}	
//
//	//헤니르던전모드가 정상인지 체크한다.
//	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
//	{
//		START_LOG( cerr, L"던전 모드값이 이상합니다!" )
//			<< BUILD_LOGc( kPacket_.m_cDungeonMode )
//			<< END_LOG;
//
//		kAck.m_iOK = NetError::ERR_PARTY_22;
//		goto end_proc;
//	}
//
//	////////////////////////////////////////////////////////////////////////// 유저 조건
//	//{{ 2009. 8. 5  최육사		체험ID제한
//	if( IsGuestUser()  &&  CXSLDungeon::IsHenirDungeon( kPacket_.m_iDungeonID ) )
//	{
//		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
//		goto end_proc;
//	}
//	//}}
//
//	//이미 파티가 있는경우..
//	if( GetPartyUID() != 0 )
//	{
//		kAck.m_iOK = NetError::ERR_PARTY_12;
//		goto end_proc;
//	}
//
//	//대전 로비에 있는경우.
//	if( GetUnitState() == CXSLUnit::CUS_PVP_LOBBY )
//	{
//		kAck.m_iOK = NetError::ERR_PARTY_13;
//		goto end_proc;
//	}
//
//	//::1. 근성도 검사.
//	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
//	{
//		kAck.m_iOK = NetError::ERR_ROOM_27;
//		goto end_proc;
//	}
//
//	//::2. 입장필요 아이템검사.
//	int iRequireItemID = 0;
//	int iRequireQuantity = 0;
//
//	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
//	{
//		//{{ 2010. 05. 06  최육사	에러 팝업
//		KEGS_ERROR_POPUP_NOT kNot;
//		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
//		kNot.m_iErrorData1 = iRequireItemID;
//		kNot.m_iErrorData2 = iRequireQuantity;
//		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
//		//}}
//
//		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
//		goto end_proc;
//	}
//
//	//운영자 이상이 아니면 체크한다.
//	if( GetAuthLevel() < SEnum::UAL_GM )
//	{
//		//::3. 선행던전 클리어를 하지 않았음.
//		const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
//		if( iRequireDungeonID != 0 )
//		{
//			std::map<int,KDungeonClearInfo>::iterator mit;
//			mit = m_mapDungeonClear.find( iRequireDungeonID );
//			if( mit == m_mapDungeonClear.end() )
//			{
//				kAck.m_iOK	= NetError::ERR_ROOM_06;
//				goto end_proc;
//			}
//		}
//
//		//{{ 2010. 12. 30	최육사	하멜 마을 추가
//#ifdef SERV_SHIP_DUNGEON
//		if( CXSLDungeon::IsShipDungeon( kPacket_.m_iDungeonID ) == true )
//		{
//			if( CXSLDungeon::IsVelderDungeon( kPacket_.m_iDungeonID ) == true )
//			{
//				// 해당 map에 들어갈 수 있는지 조건 체크
//				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
//				{
//					START_LOG( cwarn, L"마을에 입장할 수 있는 조건이 안됨." )
//						<< BUILD_LOG( GetCharUID() )
//						<< BUILD_LOG( kPacket_.m_iDungeonID )
//						<< END_LOG;
//
//					kAck.m_iOK = NetError::GetLastError();
//					goto end_proc;
//				}
//			}
//		}
//#endif SERV_SHIP_DUNGEON
//		//}}
//	}
//	else
//	{
//		START_LOG( cout, L"운영자 파티 생성 시도!" )
//			<< BUILD_LOG( GetCharName() )
//			<< BUILD_LOG( kPacket_.m_iDungeonID );
//	}
//
//	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
//#ifdef SERV_COME_BACK_USER_REWARD
//	{
//		// 파티 생성자가 휴면 유저이라면 휴면 파티
//		CTime tComeBackBuffEnd = m_kComeBackUserInfo.GetComeBackBuffEnd();
//		kPacket_.m_bComeBackBuff = m_kComeBackUserInfo.GetIsComeBackUser();
//		kPacket_.m_wsrtComeBackBuffEnd = tComeBackBuffEnd.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
//	}
//#endif SERV_COME_BACK_USER_REWARD
//	//}} 
//
//	//{{ 2011. 03. 21  김민성	던전 난이도 보정
//#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
//	if( kPacket_.m_bCheckLowLevel == true )	// 레벨 제한이 설정 되어 있다면
//	{
//		int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) );
//
//		if( static_cast<int>(GetLevel()) < iDungeonMinLevel - 2 )
//		{
//			kAck.m_iOK = NetError::ERR_CREATE_PARTY_FAILE;
//			goto end_proc;
//		}
//	}
//#endif SERV_DUNGEON_DIFFICULTY_REVISION
//	//}}
//
//	////////////////////////////////////////////////////////////////////////// 처리
//	// 파티 생성
//	//{{ 2010. 02. 17  최육사	채널파티통합
//#ifdef SERV_CHANNEL_PARTY
//	if( SiKPartyListManager()->E_CreatePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
//#else
//	if( SiKPartyManager()->E_CreateParty( GetThisPtr<KGSUser>(), kPacket_ ) == false )
//#endif SERV_CHANNEL_PARTY
//		//}}	
//	{
//		START_LOG( cerr, L"파티생성 실패.!" )
//			<< BUILD_LOG( GetCharName() )
//			<< END_LOG;
//	}
//
//	return;
//
//end_proc:
//	START_LOG( cwarn, L"파티 생성실패.!" )
//		<< BUILD_LOG( GetCharName() )
//		<< BUILD_LOG( NetError::GetErrStr(kAck.m_iOK) )
//		<< END_LOG;
//
//	SendPacket( EGS_CREATE_PARTY_ACK, kAck );
//}
//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

IMPL_ON_FUNC( EGS_JOIN_PARTY_REQ )
{
	//파티 조인은 마을/던전게이트 일때만 가능함.
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_JOIN_PARTY_ACK kAck;	

	////////////////////////////////////////////////////////////////////////// 데이터 검증
	// partyuid
	if( kPacket_.m_iPartyUID <= 0 )
	{
		START_LOG( cwarn, L"파티uid 정보가 이상합니다." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_17;
		goto end_proc;
	}

	////////////////////////////////////////////////////////////////////////// 유저 조건	
	// 이미 가입된 파티가 있는지..?
	if( GetPartyUID() != 0 )
	{
		START_LOG( cwarn, L"이미 가입된 파티가 있음.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_12;
		goto end_proc;
	}

	// 대전로비에 잇는지..
	if( GetUnitState() == CXSLUnit::CUS_PVP_LOBBY )
	{
		START_LOG( cerr, L"대전로비에서는 파티에 가입할수 없습니다." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_14;
		goto end_proc;
	}

	//{{ 2011. 07. 27	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	// 파티 신청자가 현재 매칭 신청 했다면 신청 불가능 
	if( IsRequestMatchMaking() == true )
	{
		kAck.m_iOK = NetError::ERR_MATCH_MAKING_18;
		goto end_proc;
	}

	// 파티 신청자가 현재 매칭 중이라면 신청 불가능 
	if( GetMatchUID() != 0 )
	{
		kAck.m_iOK = NetError::ERR_MATCH_MAKING_19;
		goto end_proc;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;
	//파티의 던전정보를 찾아서..
	//{{ 2010. 02. 20  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->GetDungeonInfo( kPacket_.m_iPartyUID, iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#else
	if( SiKPartyManager()->GetDungeonInfo( kPacket_.m_iPartyUID, iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티의 던전정보 찾기 실패.!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		// 시간차로 파티정보를 못찾을수 있다
		kAck.m_iOK = NetError::ERR_PARTY_17;
		goto end_proc;
	}

	//{{ 2009. 8. 5  최육사		체험ID제한
	if( IsGuestUser()  &&  CXSLDungeon::IsHenirDungeon( iDungeonID ) )
	{
		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}
	//}}

	//::1. 근성도 검사.
	if( CheckingSpirit( iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	//::2. 입장필요 아이템검사.
	int iRequireItemID = 0;
	int iRequireQuantity = 0;

	if( CheckRequiredItemToEnterDungeonRoom( iDungeonID + static_cast<int>(cDifficultyLevel), (CXSLDungeon::DUNGEON_MODE)cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
	{
		//{{ 2010. 05. 06  최육사	에러 팝업
		KEGS_ERROR_POPUP_NOT kNot;
		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		kNot.m_iErrorData1 = iRequireItemID;
		kNot.m_iErrorData2 = iRequireQuantity;
		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
		//}}

		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		goto end_proc;
	}

	//운영자 이상이 아니면 체크한다.
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		//::3. 선행던전 검사.
		//int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonID + static_cast<int>(cDifficultyLevel) );

		////선행던전이 있어야 한다.
		//if( iRequireDungeonID != 0 )
		//{
		//	std::map< int, KDungeonClearInfo >::const_iterator mit;
		//	mit = m_mapDungeonClear.find( iRequireDungeonID );
		//	if( mit == m_mapDungeonClear.end() )
		//	{
		//		kAck.m_iOK = NetError::ERR_ROOM_06;
		//		goto end_proc;
		//	}
		//}
	}
	else
	{
		START_LOG( cout, L"운영자 파티 참여.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( iDungeonID )
			;
	}	
	//}}

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	bool isPartyNULL = false;
	if( SiKPartyListManager()->IsPossibleLowLevel( GetThisPtr<KGSUser>(), kPacket_.m_iPartyUID, isPartyNULL ) == false )
	{
		if( isPartyNULL == true )
		{
			kAck.m_iOK = NetError::ERR_PARTY_17;
			goto end_proc;
		}
		kAck.m_iOK = NetError::ERR_MIN_LEVEL_JOIN_PARTY_FAILE;
		goto end_proc;
	}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	////////////////////////////////////////////////////////////////////////// 처리	
	// 파티에 입장할수 있는 상황이면..
	//{{ 2010. 02. 17  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_JoinPartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_JoinParty( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cwarn, L"파티 가입신청 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"파티 입장실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
		<< END_LOG;

	SendPacket( EGS_JOIN_PARTY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_JOIN_PARTY_INVITE_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( GetPartyUID() == 0 )
	{
		// [참고] 시간차로 발생가능

		START_LOG( cwarn, L"파티장인데 Party UID 가 없다.?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		return;
	}

	//{{ 2010. 02. 17  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_JoinPartyInviteReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_JoinPartyInviteReply( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"파티가입 승인처리 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_LEAVE_PARTY_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		// [참고] 파티이탈하기전에 강퇴당했다면 시간차로 발생가능

		START_LOG( cwarn, L"파티를 탈퇴하기전인데 UID가 없다.?" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_LEAVE_PARTY_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_PARTY_01;
		SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
		return;
	}

	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_LeavePartyReq( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_LeaveParty( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"파티 나가기 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iReason) )
			<< END_LOG;
	}

	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

IMPL_ON_FUNC( EGS_INVITE_PARTY_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_INVITE_PARTY_ACK kAck;
	kAck.m_iReceiverUID = kPacket_.m_iReceiverUID;
	kAck.m_iOK = NetError::NET_OK;	

	// 같은 채널일 경우만 초대가 가능합니다.
	KGSUserPtr spReceiver = GetKUserManager()->GetByCID( kPacket_.m_iReceiverUID );
	if( spReceiver == NULL )
	{
		// 시간차로 유저가 존재하지 않을 수 있다.
		START_LOG( cwarn, L"초대할려는 유저가 없네.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_07;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	if( GetPartyUID() == 0 )
	{
		kAck.m_iOK = NetError::ERR_PARTY_09;
		SendPacket( EGS_INVITE_PARTY_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_InvitePartyReq( GetThisPtr<KGSUser>(), spReceiver ) == false )
#else
	if( SiKPartyManager()->E_InviteParty( GetPartyUID(), GetThisPtr<KGSUser>(), spReceiver ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티장이 초대한 유저 초대실패.!" )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( spReceiver->GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( spReceiver->GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_INVITE_PARTY_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	////파티장 초대에 응하는 경우..
	//KERM_INVITE_PARTY_REPLY_NOT kNot;
	//kNot.m_bAccept = kPacket_.m_bAccept;
	//kNot.m_iReason = 0;	// 별다른 이유가 없을경우.
	//GetPartyUserInfo( kNot.m_kPartyUserInfo );

	//_SendToCnRoom( kPacket_.m_iPartyUID, ERM_INVITE_PARTY_REPLY_NOT, kNot );

	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_InvitePartyReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_InvitePartyReply( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"파티 초대에 의한 입장 실패." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_READY_REQ )
{
	VERIFY_STATE( (2, KGSFSM::S_ROOM, KGSFSM::S_FIELD_MAP) );

	KEGS_PARTY_CHANGE_READY_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	////////////////////////////////////////////////////////////////////////// 유저 조건
	// partyuid
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;
	//파티의 던전정보를 찾아서..

	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#else
	if( SiKPartyManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cerr, L"파티의 던전정보 찾기 실패.!" )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		//파티 UID 이상과 같은 경우로 본다.
		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	//{{ 2009. 8. 5  최육사		체험ID제한
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"클라이언트에서 체험ID가 레벨20되었을때 유저를 캐릭선택으로 이동시켰을텐데 아직 게임을 하고있네?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUEST_USER_02;
			goto end_proc;
		}

		if( CXSLDungeon::IsHenirDungeon( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_GUEST_USER_00;
			goto end_proc;
		}
	}
	//}}

	//{{ 2010. 12. 28	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
	if( CXSLDungeon::IsShipDungeon( iDungeonID ) == false )
#endif SERV_SHIP_DUNGEON
	//}}
	{
		//던전게이트가 아니면 레디할수 없음.
		if( CXSLMapData::IsDungeonGate( GetMapID() ) == false )
		{
			START_LOG( cwarn, L"던전게이트가 아니면 파티레디를 할수없음." )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( iDungeonID )
				<< BUILD_LOG( GetMapID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_20;
			goto end_proc;
		}

			//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
		if( iDungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_HARD || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
		{
			// 소속 던전 게이트 검사 안함
		}
		else
#endif SERV_HALLOWEEN_DUNGEON
			//}}
		if( GetMapID() != CXSLMapData::GetMapID( iDungeonID ) )
		{
			START_LOG( cwarn, L"레디할수있는 던전게이트가 아님." )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( iDungeonID )
				<< BUILD_LOG( GetMapID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_29;
			goto end_proc;
		}
	}

	//준비상태로 변경시 해당 던전게임 할수있는지 검사
	if( kPacket_.m_bReady == true )
	{
		//::1. 근성도 검사.
		if( CheckingSpirit( iDungeonID ) == false )
		{
			kAck.m_iOK = NetError::ERR_ROOM_27;
			goto end_proc;
		}

		//::2. 입장필요 아이템검사.
		int iRequireItemID = 0;
		int iRequireQuantity = 0;

		if( CheckRequiredItemToEnterDungeonRoom( iDungeonID + static_cast<int>(cDifficultyLevel), (CXSLDungeon::DUNGEON_MODE) cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
		{
			//{{ 2010. 05. 06  최육사	에러 팝업
			KEGS_ERROR_POPUP_NOT kNot;
			kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			kNot.m_iErrorData1 = iRequireItemID;
			kNot.m_iErrorData2 = iRequireQuantity;
			SendPacket( EGS_ERROR_POPUP_NOT, kNot );
			//}}

			kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
			goto end_proc;
		}

		//{{ 2010. 12. 30	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) == true )
		{
			if( CXSLDungeon::IsVelderDungeon( iDungeonID ) == true )
			{
				// 해당 map에 들어갈 수 있는지 조건 체크
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
				{
					START_LOG( cwarn, L"마을에 입장할 수 있는 조건이 안됨." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iDungeonID )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					goto end_proc;
				}
			}
		}
#endif SERV_SHIP_DUNGEON
		//}}
	}

	////////////////////////////////////////////////////////////////////////// 처리
	// 레디 변경
	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeReadyReq( GetThisPtr<KGSUser>(), kPacket_.m_bReady ) == false )
#else
	if( SiKPartyManager()->E_ChangeReady( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_.m_bReady ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티 레디 변경 실패." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( static_cast<int>(kPacket_.m_bReady) )
			<< END_LOG;
	}

	return;

end_proc:
	SendPacket( EGS_PARTY_CHANGE_READY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_DUNGEON_REQ )
{	
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEGS_PARTY_CHANGE_DUNGEON_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	////////////////////////////////////////////////////////////////////////// 데이터 검증	
	// PartyUID검증
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

	// DungeonID검증
	if( !SiCXSLDungeonManager()->VerifyDungeonID( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) ) )
	{
		START_LOG( cerr, L"존재하지않는 DungeonID입니다. 클라에서 검사했을텐데.." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}

#ifdef SERV_NEW_EVENT_TYPES
	bool bIsEventDungeon = SiCXSLDungeonManager()->IsSwitchingWithEventInfo( kPacket_.m_iDungeonID );
	if( !SiKGameEventManager()->IsEnableDungeon( kPacket_.m_iDungeonID, bIsEventDungeon ) )
	{
		START_LOG( cerr, L"꺼져있는 이벤트 DungeonID입니다. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}
#endif SERV_NEW_EVENT_TYPES

	//{{ 2009. 7. 30  최육사	HenirDungeonMode검증
	if( CXSLDungeon::IsValidDungeonMode( static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode) ) == false )
	{
		START_LOG( cerr, L"비정상적인 HenirDungeonMode값입니다." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOG( (int)kPacket_.m_cDungeonMode )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}
	//}}

	////////////////////////////////////////////////////////////////////////// 유저 조건
	//{{ 2009. 8. 5  최육사		체험ID제한
	if( IsGuestUser()  &&  CXSLDungeon::IsHenirDungeon( kPacket_.m_iDungeonID ) )
	{
		kAck.m_iOK = NetError::ERR_GUEST_USER_00;
		goto end_proc;
	}
	//}}

	//::1. 근성도 검사.
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	//::2. 입장필요 아이템검사.
	int iRequireItemID = 0;
	int iRequireQuantity = 0;

	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
	{
		//{{ 2010. 05. 06  최육사	에러 팝업
		KEGS_ERROR_POPUP_NOT kNot;
		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		kNot.m_iErrorData1 = iRequireItemID;
		kNot.m_iErrorData2 = iRequireQuantity;
		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
		//}}

		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		goto end_proc;
	}

	//::3. 선행던전 클리어를 하지 않았음.
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
		if( iRequireDungeonID != 0 )
		{
			std::map<int,KDungeonClearInfo>::iterator mit;
			mit = m_mapDungeonClear.find( iRequireDungeonID );
			if( mit == m_mapDungeonClear.end() )
			{
				kAck.m_iOK	= NetError::ERR_ROOM_06;

				goto end_proc;
			}
		}
	}

	//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
	//::4. 레벨 조건이 충족되지 않는 유저가 있음.
	{
		KGSPartyPtr spParty = SiKPartyListManager()->GetParty( GetPartyUID() );
		if( spParty == NULL )
		{
			START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_03;

			goto end_proc;
		}

		if( spParty->GetCheckLowLevel() == true )
		{
			int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_DifficultyLevel) );

			if( spParty->CheckPartyMinLevel( iDungeonMinLevel ) == false )
			{
				kAck.m_iOK = NetError::ERR_CHANGE_DUNGEON_MAP_FAILE;

				goto end_proc;
			}
		}
	}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
	//}}

	//{{ 2012. 03. 03	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	if( CXSLDungeon::IsDefenceDungeon( kPacket_.m_iDungeonID ) == true )
	{
		if( SiKGSWorldMissionManager()->GetIsActive() == true )
		{
			int iWorldCrystalNum = SiKGSWorldMissionManager()->GetWorldCrystalCount();

			if( KGSWorldMissionManager::GATE_OF_THE_DARKNESS_EXPERT <= iWorldCrystalNum )
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_EXPERT;
			}
			else if( KGSWorldMissionManager::GATE_OF_THE_DARKNESS_HARD <= iWorldCrystalNum )
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_HARD;
			}
			else
			{
				kPacket_.m_DifficultyLevel = CXSLDungeon::DL_NORMAL;
			}
		}
		else
		{
			START_LOG( cerr, L"패킷 변조 의심 유저입니다.")
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_22;
			goto end_proc;
		}
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	

	////////////////////////////////////////////////////////////////////////// 처리
	// 던전 변경
	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeDungeon( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeDungeon( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"던전변경 실패." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_DifficultyLevel )
			<< END_LOG;
	}	

	return;

end_proc:
	SendPacket( EGS_PARTY_CHANGE_DUNGEON_ACK, kAck );
}

IMPL_ON_FUNC( EGS_PARTY_GAME_START_REQ )
{
	//{{ 2010. 9. 6	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ), EGS_PARTY_GAME_START_REQ, EGS_PARTY_GAME_START_ACK );
#else
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	// 정보 초기화
	SetReserveKickUser( false );
	SetDungeonRequiredItemID( 0 );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}

	////////////////////////////////////////////////////////////////////////// 유저 검사
	// 1. 파티에 속해 있는지 검사
	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetStateIDString() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}
	
	int iDungeonID = 0;
	char cDifficultyLevel = 0;
	char cDungeonMode = 0;
	// 2. 파티의 던전정보를 찾아서..
	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#else
	if( SiKPartyManager()->GetDungeonInfo( GetPartyUID(), iDungeonID, cDifficultyLevel, cDungeonMode ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cerr, L"파티의 던전정보 찾기 실패.!" )
			<< BUILD_LOG( GetPartyUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;
		//파티 UID 이상과 같은 경우로 본다.
		kAck.m_iOK = NetError::ERR_PARTY_03;
		goto end_proc;
	}

#ifdef SERV_DUNGEON_OPTION_IN_LUA
	if( CXSLDungeon::IsFixedMembers( iDungeonID ) == true )
	{
		int iNumMember = 0;
		if( SiKPartyListManager()->GetPartyNumMember( GetPartyUID(), iNumMember ) == false )
		{
			START_LOG( cerr, L"파티 맴버 이상.!" )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( GetCharName() )
				<< BUILD_LOG( iNumMember )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_PARTY_46;
			goto end_proc;
		}

		if( iNumMember != CXSLDungeon::GetFixedMembers( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_PARTY_46;
			goto end_proc;
		}
	}
#endif SERV_DUNGEON_OPTION_IN_LUA

#ifdef SERV_NEW_EVENT_TYPES
	bool bIsEventDungeon = SiCXSLDungeonManager()->IsSwitchingWithEventInfo( iDungeonID );
	if( !SiKGameEventManager()->IsEnableDungeon( iDungeonID, bIsEventDungeon ) )
	{
		START_LOG( cerr, L"꺼져있는 이벤트 DungeonID입니다. 클라에서 검사했을텐데.." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( iDungeonID )
			<< BUILD_LOGc( (int)cDifficultyLevel )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_22;
		goto end_proc;
	}
#endif SERV_NEW_EVENT_TYPES

	//{{ 2009. 8. 5  최육사		체험ID제한
	if( IsGuestUser() )
	{
		if( GetLevel() > SiKGameSysVal()->GetGuestUserLimitLevel() )
		{
			START_LOG( cerr, L"클라이언트에서 체험ID가 레벨20되었을때 유저를 캐릭선택으로 이동시켰을텐데 아직 게임을 하고있네?" )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_GUEST_USER_02;
			goto end_proc;
		}

		if( CXSLDungeon::IsHenirDungeon( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_GUEST_USER_00;
			goto end_proc;
		}
	}
	//}}

	//{{ 2010. 12. 28	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
	if( CXSLDungeon::IsShipDungeon( iDungeonID ) == false )
#endif SERV_SHIP_DUNGEON
	//}}
	{
		// 3. 던전게이트인지 검사.
		if( CXSLMapData::IsDungeonGate( GetMapID() ) == false )
		{
			kAck.m_iOK = NetError::ERR_PARTY_20;
			goto end_proc;
		}

		//{{ 2011. 10.10	할로윈 던전 하드코딩
#ifdef SERV_HALLOWEEN_DUNGEON
		if( iDungeonID == SEnum::DI_ELDER_HALLOWEEN_NORMAL || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_HARD || iDungeonID == SEnum::DI_ELDER_HALLOWEEN_EXPERT )
		{
			// 소속 던전 게이트 검사 안함
		}
		else
#endif SERV_HALLOWEEN_DUNGEON
			//}}
		// 4. 해당 던전에 맞는 던전게이트인지 검사	
		if( GetMapID() != CXSLMapData::GetMapID( iDungeonID ) )
		{
			kAck.m_iOK = NetError::ERR_PARTY_29;
			goto end_proc;
		}
	}

	// 5. 파티장의 근성도 검사
	if( CheckingSpirit( iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	// 6. 입장필요 아이템검사.
	int iRequireItemID = 0;
	int iRequireQuantity = 0;

	if( CheckRequiredItemToEnterDungeonRoom( iDungeonID + static_cast<int>(cDifficultyLevel), (CXSLDungeon::DUNGEON_MODE) cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
	{
		//{{ 2010. 05. 06  최육사	에러 팝업
		KEGS_ERROR_POPUP_NOT kNot;
		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		kNot.m_iErrorData1 = iRequireItemID;
		kNot.m_iErrorData2 = iRequireQuantity;
		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
		//}}

		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		goto end_proc;
	}

	// 7. 선행던전 클리어를 검사
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonID + static_cast<int>(cDifficultyLevel) );
		if( iRequireDungeonID != 0 )
		{
			std::map<int,KDungeonClearInfo>::iterator mit;
			mit = m_mapDungeonClear.find( iRequireDungeonID );
			if( mit == m_mapDungeonClear.end() )
			{
				kAck.m_iOK	= NetError::ERR_ROOM_06;
				goto end_proc;
			}
		}

		//{{ 2010. 12. 30	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
		if( CXSLDungeon::IsShipDungeon( iDungeonID ) == true )
		{
			if( CXSLDungeon::IsVelderDungeon( iDungeonID ) == true )
			{
				// 해당 map에 들어갈 수 있는지 조건 체크
				if( CheckEnterTheVillage( CXSLMapData::MI_HAMEL ) == false )
				{
					START_LOG( cwarn, L"마을에 입장할 수 있는 조건이 안됨." )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( iDungeonID )
						<< END_LOG;

					kAck.m_iOK = NetError::GetLastError();
					goto end_proc;
				}
			}
		}
#endif SERV_SHIP_DUNGEON
		//}}
	}
	
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	//	디펜스던전 검사
	if( CXSLDungeon::IsDefenceDungeon( iDungeonID ) == true )
	{
		//	던전 오픈 검사
		if( false == SiKGSWorldMissionManager()->GetIsActive() )
		{
			kAck.m_iOK = NetError::ERR_WORLD_MISSION_02;
			goto end_proc;
		}
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	// 8. 파티 게임 시작
	//{{ 2010. 02. 22  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_GameStart( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_GameStart( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"던전게임 시작하기 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( iDungeonID )
			<< BUILD_LOGc( cDifficultyLevel )
			<< BUILD_LOGc( GetPartyUID() )
			<< END_LOG;
	}

	return;

end_proc:
	SendPacket( EGS_PARTY_GAME_START_ACK, kAck );
}

IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
{
	// 파티장에게 알려주기 위해.
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetPartyUID() == 0 )
		{
			START_LOG( cerr, L"파티를 통해 던전을 생성햇는데 파티 UID가 없음.!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;
		}
	}

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_PARTY_GAME_START_ACK, kAck );
}

_IMPL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_NOT, KEGS_PARTY_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
		kPacket_.m_wstrCNIP.clear();

	m_iRoomUID = kPacket_.m_RoomInfo.m_RoomUID;

	StateTransition( KGSFSM::I_TO_ROOM );
	m_kTimer[TM_ROOM].restart();

	// 필드에서 이탈
	SendLeaveField( NetError::NOT_LEAVE_FIELD_REASON_01 );

	//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
	SendUpdateGuildMemberInfo();
#endif GUILD_TEST
	//}}

	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			int iRequireDungeonID = kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>(kPacket_.m_RoomInfo.m_DifficultyLevel);

			CXSLDungeon::DUNGEON_MODE eDungeonMode = (CXSLDungeon::DUNGEON_MODE) kPacket_.m_RoomInfo.m_cDungeonMode;

			//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
			// 헤니르 던전이라면 보상 받은 횟수를 증가시킨다.
			if( CXSLDungeon::IsHenirDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) == true )
			{
				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
				{
					START_LOG( clog, L"헤니르 보상 획득 횟수 증가 실패!!" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
						<< END_LOG;
				}
			}
#endif SERV_NEW_HENIR_TEST
			//}}

			//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
			int iItemID = 0;
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo, iItemID ) )
#else
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo ) )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
			//}}
			{
				// 우선 게임시작시 아이템이 없으면 오류니 로그를 남겨본다..
				START_LOG( cerr, L"던전게임 시작시 필요아이템이 없음.!" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( iRequireDungeonID )
					<< BUILD_LOG( (int)eDungeonMode )
					<< END_LOG;

//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
				// 강제퇴장 예약 설정
				SetReserveKickUser( true );
				SetDungeonRequiredItemID( iItemID );

// 				KERM_RESERVE_LEAVE_USER_NOT kPacketReq;
// 				kPacketReq.m_iUnitUID = GetCharUID();
// 				kPacketReq.m_iItemID = iItemID;
// 				SendToCnRoom( ERM_RESERVE_LEAVE_USER_NOT, kPacketReq );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"일어날수 없는 에러!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
	}	

	SendPacket( EGS_PARTY_GAME_START_NOT, kPacket_ );

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	// 던전 시작하면 초기화
	m_kInventory.SetUseItemNeedPayment( false );
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

	//{{ 2009. 5. 30  최육사	임시 인벤
	ClearTempInventory();
	//}}
}

//{{ 2009. 2. 4  최육사	던전 리플레이
_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_ACK, KERM_PARTY_OPEN_DUNGEON_ROOM_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( GetPartyUID() == 0 )
		{
			START_LOG( cerr, L"파티를 통해 던전 리플레이를 했는데 파티 UID가 없음.!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< END_LOG;
		}
	}

	KEGS_PARTY_GAME_START_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_PARTY_GAME_START_ACK, kAck );
}

_IMPL_ON_FUNC( ERM_PARTY_GAME_REPLAY_START_NOT, KEGS_PARTY_GAME_START_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetAuthLevel() < SEnum::UAL_GM )
		kPacket_.m_wstrCNIP.clear();

	if( GetRoomUID() != kPacket_.m_RoomInfo.m_RoomUID )
	{
		START_LOG( cerr, L"던전 리플레이인데 RoomUID가 다르면 안되는데?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( kPacket_.m_RoomInfo.m_RoomUID )
			<< END_LOG;

		// roomuid를 일단 넣어준다. 만약에 roomuid가 0이라면.. 이건 문제가 된다
		m_iRoomUID = kPacket_.m_RoomInfo.m_RoomUID;
	}

	m_kTimer[TM_ROOM].restart();

	switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
	{
	case CXSLRoom::RT_DUNGEON:
		{
			int iRequireDungeonID = kPacket_.m_RoomInfo.m_iDungeonID + static_cast<int>(kPacket_.m_RoomInfo.m_DifficultyLevel);

			CXSLDungeon::DUNGEON_MODE eDungeonMode = (CXSLDungeon::DUNGEON_MODE) kPacket_.m_RoomInfo.m_cDungeonMode;
			
			//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
			// 헤니르 던전이라면 보상 받은 횟수를 증가시킨다.
			if( CXSLDungeon::IsHenirDungeon( kPacket_.m_RoomInfo.m_iDungeonID ) == true )
			{
				if( m_kUserDungeonManager.IncreaseHenirRewardCount() == false )
				{
					START_LOG( cerr, L"헤니르 보상 획득 횟수 증가 실패!!" )
#ifdef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharUID() )
#else
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardNormalCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardPremiumCount() )
						<< BUILD_LOG( m_kUserDungeonManager.GetPossibleHenirRewardEventCount() )
						<< END_LOG;
				}
			}
#endif SERV_NEW_HENIR_TEST
			//}}

			//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
			int iItemID = 0;
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo, iItemID ) )
#else
			if( !DeleteRequiredItemToEnterDungeonRoom( iRequireDungeonID, eDungeonMode, kPacket_.m_vecInventorySlotInfo ) )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
				//}}
			{
				// 우선 게임시작시 아이템이 없으면 오류니 로그를 남겨본다..
				START_LOG( cerr, L"던전게임 시작시 필요아이템이 없음.!" )
#ifdef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetCharUID() )
#else
					<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( iRequireDungeonID )
					<< BUILD_LOG( (int)eDungeonMode )
					<< END_LOG;

				//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
				// 강제퇴장 예약 설정
				SetReserveKickUser( true );
				SetDungeonRequiredItemID( iItemID );

// 				KERM_RESERVE_LEAVE_USER_NOT kPacketReq;
// 				kPacketReq.m_iUnitUID = GetCharUID();
// 				kPacketReq.m_iItemID = iItemID;
// 				SendToCnRoom( ERM_RESERVE_LEAVE_USER_NOT, kPacketReq );
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
				//}}
				// 방 입장 실패 처리 - 센터서버로 ERM_LEAVE_ROOM_REQ를 날려서 방이탈 처리 함
				// 돌아오는 ACK는 KGSFSM에 걸려서 클라이언트로 전송되지 않는다.
				//KERM_LEAVE_ROOM_REQ kPacketReq;
				//kPacketReq.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_22;
				//SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacketReq );
				//return;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"일어날수 없는 에러!" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
	}

	SendPacket( EGS_PARTY_GAME_START_NOT, kPacket_ );

	//{{ 2009. 5. 30  최육사	임시 인벤
	ClearTempInventory();
	//}}
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 28	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON

IMPL_ON_FUNC( EGS_PARTY_TO_FIELD_MAP_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_PARTY_TO_FIELD_MAP_ACK kAck;

	CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	if( eRoomType != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"파티 던전을 종료하는데 UID가 던전 또는 아케이드가 아님.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( static_cast<int>(eRoomType) )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_25;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}
	
	if( SiKPartyListManager()->E_ToFieldMap( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"던전게임 끝내기 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC_NOPARAM( EGS_PARTY_TO_FIELD_MAP_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	KEGS_PARTY_TO_FIELD_MAP_ACK kAck;

	CXSLRoom::ROOM_TYPE eRoomType = CXSLRoom::GetRoomType( GetRoomUID() );

	if( eRoomType != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"파티 던전을 종료하는데 UID가 던전이 아님.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( static_cast<int>(eRoomType) )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_25;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ToFieldMap( GetThisPtr<KGSUser>() ) == false )
#else
	if( SiKPartyManager()->E_ToFieldMap( GetPartyUID(), GetThisPtr<KGSUser>() ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"던전게임 끝내기 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_SHIP_DUNGEON
//}}

_IMPL_ON_FUNC( ERM_PARTY_TO_FIELD_MAP_ACK, KEGS_PARTY_TO_FIELD_MAP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK  ||  GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 던전방 종료 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kPacket_ );
}

//{{ 2010. 12. 29	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON

_IMPL_ON_FUNC( ERM_PARTY_TO_FIELD_MAP_NOT, KEGS_PARTY_TO_FIELD_MAP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}
	
	SendPacket( EGS_PARTY_TO_FIELD_MAP_NOT, kPacket_ );
}

#else

IMPL_ON_FUNC_NOPARAM( ERM_PARTY_TO_FIELD_MAP_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}

	SendID( EGS_PARTY_TO_FIELD_MAP_NOT );
}

#endif SERV_SHIP_DUNGEON
//}}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_NAME_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );	

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_NAME_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_NAME_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeName( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeName( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"파티 공개설정 변경 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}	
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_PUBLIC_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_PUBLIC_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_PUBLIC_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangePublic( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangePublic( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"파티 공개설정 변경 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_HOST_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_HOST_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 26  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeHost( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeHost( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}
	{
		START_LOG( cwarn, L"파티장 바꾸기 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_BAN_USER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_BAN_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_BAN_USER_ACK, kAck );
		return;
	}

	//자기자신을 강퇴 시킬수 없다.
	if( kPacket_ == GetCharUID() )
	{
		KEGS_PARTY_BAN_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_ROOM_26;
		SendPacket( EGS_PARTY_BAN_USER_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 26  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_BanUser( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_BanUser( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티강퇴 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//{{ 2009. 3. 18  최육사	파티유저밴
IMPL_ON_FUNC( ERM_PARTY_BAN_USER_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 실패했어도 not는 날려주자!
	}

	//{{ 2010. 8. 18	최육사	서버 코드 정리
	if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
	{
		ClearTempInventory();
	}
	//}}

	// 방 이탈처리
	SetRoomUID( 0 );

	StateTransition( KGSFSM::I_TO_FIELD_MAP );

	SendID( EGS_PARTY_BAN_USER_NOT );

	if( GetTradeUID() != 0 )
	{
		KEGS_BREAK_TRADE_REQ kPacket;
		kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_16;
		_SendToCnRoom( GetTradeUID(), ERM_BREAK_TRADE_REQ, kPacket );
	}
}
//}}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_GET_ITEM_TYPE_REQ )
{	
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeGetItemtype( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeGetItemtype( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cerr, L"던전게임 아이템 습득방식 변경실패." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_PARTY_CHANGE_NUM_OF_PER_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_NUM_OF_PER_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_ChangeNumOfPer( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_ChangeNumOfPer( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티 인원병경 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

//{{ 2009. 4. 1  최육사		파티렙제
IMPL_ON_FUNC( EGS_PARTY_LIMIT_LEVEL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_LIMIT_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_LIMIT_LEVEL_ACK, kAck );
		return;
	}

	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
#else
	if( SiKPartyManager()->E_LimitLevel( GetPartyUID(), GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티 렙제 옵션 변경 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}
//}}

//{{ 2009. 5. 1  최육사		파티 퀵조인
IMPL_ON_FUNC( EGS_PARTY_QUICK_JOIN_REQ )
{
	// 파티 퀵조인은 마을/던전게이트 일때만 가능함.
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_PARTY_QUICK_JOIN_ACK kAck;

	// 이미 가입된 파티가 있는지..?
	if( GetPartyUID() != 0 )
	{
		START_LOG( cwarn, L"이미 가입된 파티가 있음.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_12;
		goto end_proc;
	}

	// 대전로비에 잇는지..
	if( GetUnitState() == CXSLUnit::CUS_PVP_LOBBY )
	{
		START_LOG( cerr, L"대전로비에서는 파티에 가입할수 없습니다." )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_PARTY_14;
		goto end_proc;
	}

	//::1. 근성도 검사.				
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		kAck.m_iOK = NetError::ERR_ROOM_27;
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	if( ( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty) ) <= 0 )
	{
		START_LOG( cerr, L"dungeonid값이 이상하다!" )
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_cDifficulty )
			<< END_LOG;
	}
	//////////////////////////////////////////////////////////////////////////

	//::2. 입장필요 아이템검사.
	int iRequireItemID = 0;
	int iRequireQuantity = 0;

	if( CheckRequiredItemToEnterDungeonRoom( kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty), (CXSLDungeon::DUNGEON_MODE) kPacket_.m_cDungeonMode, iRequireItemID, iRequireQuantity ) == false )
	{
		//{{ 2010. 05. 06  최육사	에러 팝업
		KEGS_ERROR_POPUP_NOT kNot;
		kNot.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		kNot.m_iErrorData1 = iRequireItemID;
		kNot.m_iErrorData2 = iRequireQuantity;
		SendPacket( EGS_ERROR_POPUP_NOT, kNot );
		//}}

		kAck.m_iOK = NetError::ERR_DUNGEON_REQUIRE_ITEM_00;
		goto end_proc;
	}

	//운영자 이상이 아니면 체크한다.
	if( GetAuthLevel() < SEnum::UAL_GM )
	{
		//::3. 선행던전 검사.
		//int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( iDungeonID + static_cast<int>(cDifficultyLevel) );

		////선행던전이 있어야 한다.
		//if( iRequireDungeonID != 0 )
		//{
		//	std::map< int, KDungeonClearInfo >::const_iterator mit;
		//	mit = m_mapDungeonClear.find( iRequireDungeonID );
		//	if( mit == m_mapDungeonClear.end() )
		//	{
		//		kAck.m_iOK = NetError::ERR_ROOM_06;
		//		goto end_proc;
		//	}
		//}
	}
	else
	{
		START_LOG( cout, L"운영자 퀵파티!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_cDungeonMode )
			<< BUILD_LOGc( kPacket_.m_cDifficulty );
	}
	//}}

	// 파티에 입장할수 있는 상황이면..
	//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY	
	if( SiKPartyListManager()->E_QuickJoin( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_QuickJoin( GetThisPtr<KGSUser>(), kPacket_ ) == false )
#endif SERV_CHANNEL_PARTY
		//}}	
	{
		START_LOG( cwarn, L"파티 가입신청 실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_iDungeonID )
			<< BUILD_LOGc( kPacket_.m_cDifficulty )
			<< END_LOG;
	}

	return;

end_proc:
	START_LOG( cwarn, L"파티 입장실패.!" )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetCharUID() )
#else
		<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( NetError::GetErrStr(kAck.m_iOK) )
		<< END_LOG;

	SendPacket( EGS_PARTY_QUICK_JOIN_ACK, kAck );	
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 02. 17  최육사	채널 파티 통합
#ifdef SERV_CHANNEL_PARTY

IMPL_ON_FUNC( EPM_CREATE_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );	

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//개발자 이상인지 체크한다.
		if( GetAuthLevel() >= SEnum::UAL_DEVELOPER )
		{
			START_LOG( cout, L"개발자 파티 생성 실패!" )
#ifdef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharUID() )
#else
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) );
		}

		KEGS_CREATE_PARTY_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CREATE_PARTY_ACK, kPacketAck );
		return;
	}

	//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정	
	// 이미 파티 가입된 상태인지 확인한다.
	if( GetPartyUID() > 0 )
	{
		if( GetPartyUID() != kPacket_.m_kPartyInfo.m_iPartyUID )
		{
			START_LOG( cerr, L"이미 PartyUID가 저장되어있는데 또 PartyUID를 저장하려고 하네? 새로생성된 파티 자동이탈처리!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iPartyUID )
				<< END_LOG;

			// 기존에 가입되어 있던 파티는 이탈 처리한다.
			//SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_05 );

			// 새로운 파티 생성에 대해서는 자동 이탈 처리한다.
			KEPM_LEAVE_PARTY_REQ kPacketReq;
			kPacketReq.m_iPartyUID = kPacket_.m_kPartyInfo.m_iPartyUID;
			kPacketReq.m_iReason = NetError::NOT_LEAVE_PARTY_REASON_05;
			_SendToCnParty( kPacket_.m_kPartyInfo.m_iPartyUID, EPM_LEAVE_PARTY_REQ, kPacketReq );

			// 파티 생성 실패 처리
			KEGS_CREATE_PARTY_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_PARTY_00;
			SendPacket( EGS_CREATE_PARTY_ACK, kPacketAck );
			return;
		}
		else
		{
			START_LOG( cerr, L"이미 PartyUID가 저장되어있는데 똑같은 PartyUID를 또 저장하려고 하네? 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetPartyUID() )
				<< END_LOG;

			// 이건 이탈처리를 할 필요 없는 상황.
		}
	}
	//}}

	//파티를 개설한 유저에게 UID를 셋팅해준다.
	SetPartyUID( kPacket_.m_kPartyInfo.m_iPartyUID );

	KEGS_CREATE_PARTY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kPartyInfo = kPacket_.m_kPartyInfo;
	kPacket.m_kPartyUserInfo = kPacket_.m_kPartyUserInfo;
	SendPacket( EGS_CREATE_PARTY_ACK, kPacket );

	//개발자 이상인지 체크한다.
	if( GetAuthLevel() >= SEnum::UAL_DEVELOPER )
	{
		START_LOG( cout, L"개발자 파티 생성 성공!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_kPartyInfo.m_iDungeonID );
	}
}

IMPL_ON_FUNC( EPM_JOIN_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_JOIN_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_JOIN_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_JOIN_PARTY_NOT, KEGS_JOIN_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		for( u_int ui = 0; ui < kPacket_.m_vecPartyInfo.size(); ++ui )
		{
			const KPartyInfo& kInfo = kPacket_.m_vecPartyInfo[ui];

			//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정
			if( GetPartyUID() > 0 )
			{
				if( GetPartyUID() != kInfo.m_iPartyUID )
				{
					START_LOG( cerr, L"이미 파티 소속인데 또 파티 가입처리가 되었군.. 기존파티는 이탈 시키자!" )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetPartyUID() )
						<< BUILD_LOG( kInfo.m_iPartyUID )
						<< END_LOG;

					// 기존 파티 이탈 처리
					//SendLeaveParty( NetError::NOT_LEAVE_PARTY_REASON_05 );

					// 새로운 파티 생성에 대해서는 자동 이탈 처리한다.
					KEPM_LEAVE_PARTY_REQ kPacketReq;
					kPacketReq.m_iPartyUID = kInfo.m_iPartyUID;
					kPacketReq.m_iReason = NetError::NOT_LEAVE_PARTY_REASON_05;
					_SendToCnParty( kInfo.m_iPartyUID, EPM_LEAVE_PARTY_REQ, kPacketReq );
					return;
				}
				else
				{
					START_LOG( cerr, L"이미 PartyUID가 저장되어있는데 똑같은 PartyUID를 또 저장하려고 하네? 절대 일어나서는 안되는 에러!" )
						<< BUILD_LOG( GetCharUID() )
#ifndef SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
						<< BUILD_LOG( GetPartyUID() )
						<< END_LOG;

					// 이건 이탈처리를 할 필요 없는 상황.
				}
			}

			// 예약된 partyuid삭제
			SetCandidatePartyUID( 0 );
			//}}

			// partyuid값 저장
			SetPartyUID( kInfo.m_iPartyUID );
			break;
		}

		if( GetPartyUID() <= 0 )
		{
			START_LOG( cerr, L"파티UID값이 이상합니다. 절대 일어나서는 안되는에러!" )
				<< BUILD_LOG( GetPartyUID() )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
				<< BUILD_LOG( kPacket_.m_vecPartyInfo.size() )
				<< BUILD_LOG( kPacket_.m_vecPartyUserInfo.size() )
				<< END_LOG;
		}
	}
	//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정
	else if( kPacket_.m_iOK == NetError::ERR_PARTY_18 )
	{
		// 예약된 partyuid삭제
		SetCandidatePartyUID( 0 );
	}
	//}}

	SendPacket( EGS_JOIN_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_JOIN_PARTY_INVITE_NOT, KEGS_JOIN_PARTY_INVITE_NOT )
{
	//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정	
	// 마을 및 던전게이트에서만 파티 가입 수락을 받아야함..
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
	//}}

	SendPacket( EGS_JOIN_PARTY_INVITE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_JOIN_PARTY_INVITE_DENY_NOT, KEGS_JOIN_PARTY_INVITE_DENY_NOT )
{
	//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정	
	// 마을 및 던전게이트에서만 파티 가입 수락을 받아야함..
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
	//}}

	SendPacket( EGS_JOIN_PARTY_INVITE_DENY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_DENY_NOT, KEGS_INVITE_PARTY_DENY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_INVITE_PARTY_DENY_NOT, kPacket_ );
}

//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정
IMPL_ON_FUNC( EPM_CHECK_JOIN_PARTY_USER_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEPM_CHECK_JOIN_PARTY_USER_ACK kPacket;

	// 이미 파티에 소속된 유저인지 체크
	if( GetPartyUID() > 0 )
	{
		kPacket.m_iOK = NetError::ERR_PARTY_12;
		_SendToCnParty( kPacket_.m_iPartyUID, EPM_CHECK_JOIN_PARTY_USER_ACK, kPacket );
		return;
	}

	// 파티 가입 예약
	SetCandidatePartyUID( kPacket_.m_iPartyUID );

	// 파티에 가입할 정보를 얻자
	kPacket.m_iOK = NetError::NET_OK;
	GetPartyUserInfo( kPacket.m_kPartyUserInfo );
	_SendToCnParty( kPacket_.m_iPartyUID, EPM_CHECK_JOIN_PARTY_USER_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( EPM_LEAVE_PARTY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 성공적으로 파티 이탈 되었다면?
	if( kPacket_.m_iOK == NetError::NET_OK  ||
		kPacket_.m_iOK == NetError::ERR_PARTY_01 )
	{
		//{{ 2010. 03. 29  최육사	파티 이탈 예외
		if( GetPartyUID() == 0 )
		{
			START_LOG( clog, L"이탈 요청한 이후에 현재 PartyUID가 0이군.." )
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

		// 파티 대기자UID도 리셋
		SetCandidatePartyUID( 0 );

		//파티 UID를 리셋하고.
		SetPartyUID( 0 );

		//나간유저 파티정보 업데이트.
		std::set< UidType > setEmpty;
		SendUpdatePartyMemberList( 0, setEmpty );

		//{{ 2010. 03. 12  최육사	채널 통합 파티 버그 수정
		if( GetStateID() == KGSFSM::S_ROOM  &&  GetRoomUID() > 0 )
		{
			// 현재 방에 있는 상태라면 이탈 처리!
			switch( CXSLRoom::GetRoomType( GetRoomUID() ) )
			{
			case CXSLRoom::RT_DUNGEON:
				{
					// 던전방이라면 이탈처리 합니다.
					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
					SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

					// RoomUID초기화
					SetRoomUID( 0 );

					// field상태로 이동
					StateTransition( KGSFSM::I_TO_FIELD_MAP );
				}
				break;
			case CXSLRoom::RT_SQUARE:
			case CXSLRoom::RT_TRAININGCENTER:
				// 훈련소 및 광장에 입장하면서 자동으로 파티 이탈 처리하는거라면 자동 방 이탈 처리하지 않는다.
				break;

			default:
				{
					// 정상적인 상황이 아니므로 에러로그를 남긴다.
					START_LOG( cerr, L"파티 맺고 있었는데 던전이 아닌 방에 들어와있네?" )
						<< BUILD_LOG( GetCharUID() )
						<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
						<< END_LOG;

					KERM_LEAVE_ROOM_REQ kPacket;
					kPacket.m_iReason = NetError::NOT_LEAVE_ROOM_REASON_00;
					SendToCnRoom( ERM_LEAVE_ROOM_REQ, kPacket );

					// RoomUID초기화
					SetRoomUID( 0 );

					// field상태로 이동
					StateTransition( KGSFSM::I_TO_FIELD_MAP );
				}
				break;
			}
		}
		//}}
	}

	// 클라이언트로 ACK패킷 보냄
	KEGS_LEAVE_PARTY_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_LEAVE_PARTY_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_LEAVE_PARTY_NOT, KEGS_LEAVE_PARTY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_LEAVE_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_ACK, KEGS_INVITE_PARTY_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_INVITE_PARTY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_INVITE_PARTY_NOT, KEGS_INVITE_PARTY_NOT )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	SendPacket( EGS_INVITE_PARTY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_UPDATE_PARTY_USER_INFO_BROAD_NOT, KEGS_UPDATE_PARTY_USER_INFO_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_UPDATE_PARTY_USER_INFO_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_ACK, KEGS_PARTY_CHANGE_READY_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_READY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_READY_NOT, KEGS_PARTY_CHANGE_READY_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_READY_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_ACK, KEGS_PARTY_CHANGE_DUNGEON_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_DUNGEON_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_DUNGEON_NOT, KEGS_PARTY_CHANGE_DUNGEON_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_DUNGEON_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_GAME_START_ACK, KEGS_PARTY_GAME_START_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_GAME_START_ACK, kPacket_ );
}

IMPL_ON_FUNC( EPM_CHECK_FOR_PARTY_GAME_START_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_FOR_PARTY_GAME_START_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// 파티 소속인지 검사
	if( GetPartyUID() <= 0 )
	{
		START_LOG( cerr, L"파티원이 아닌데 파티장이 게임 시작하자는 이벤트가 왔다?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	// 다른 파티원인지 검사
	if( GetPartyUID() != kPacket_.m_iPartyUID )
	{
		START_LOG( cerr, L"해당 소속 파티원이 아닌데 다른 파티장이 게임 시작하자는 이벤트가 왔다?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
		return;
	}

	switch( kPacket_.m_usEventID )
	{
	case ERM_PARTY_OPEN_DUNGEON_ROOM_REQ:
		{
			if( GetStateID() != KGSFSM::S_FIELD_MAP )
			{
				START_LOG( cwarn, L"파티장과 같은 상태가 아니라서 게임을 시작할수 없음." )
					<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
					<< BUILD_LOG( GetStateIDString() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
				return;
			}
		}
		break;

	case ERM_PARTY_GAME_REPLAY_START_REQ:
		{
			if( GetStateID() != KGSFSM::S_ROOM )
			{
				START_LOG( cwarn, L"파티장과 같은 상태가 아니라서 게임을 시작할수 없음." )
					<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
					<< BUILD_LOG( GetStateIDString() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
				return;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"잘못된 이벤트 정보 입니다. 절대 일어날수없는에러!" )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
			return;
		}
		break;
	}

	//{{ 2011. 05. 16  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
	UseResurrectionStone( false );

	int ipartynum = 0;
	SiKPartyListManager()->GetPartyNumMember( GetPartyUID(), ipartynum );
	SetStartedNumMember( ipartynum );
#endif SERV_ADD_TITLE_CONDITION
	//}}

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	m_kUserDungeonManager.SetPossibleHenirReward();
#endif SERV_NEW_HENIR_TEST
	//}}

	// PartyRoomUserInfo얻기
	KPartyRoomUserInfo kPartyRoomUserInfo;
	GetTutorUnitUIDList( kPartyRoomUserInfo.m_vecStudentUnitUID );
	GetRoomUserInfo( kPartyRoomUserInfo.m_kRoomUserInfo, CXSLRoom::RT_DUNGEON );
	//{{ 2009. 6. 10  최육사	게임 시작할 당시 EXP,ED얻기
	kPartyRoomUserInfo.m_iCurEXP = GetEXP();
	kPartyRoomUserInfo.m_iCurED = GetED();
	//}}
	//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	kPartyRoomUserInfo.m_wstrChannelIP = NetCommon::GetLocalIPW();
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_sWorldID = kPacket_.m_sWorldID;
	kPacket.m_usEventID = kPacket_.m_usEventID;
	kPacket.m_vecUserInfo.push_back( kPartyRoomUserInfo );
	SendToCnParty( EPM_CHECK_FOR_PARTY_GAME_START_ACK, kPacket );
}

_IMPL_ON_FUNC( EPM_PARTY_OPEN_DUNGEON_ROOM_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_GAME_REPLAY_START_NOT, KERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendToCnRoom( ERM_PARTY_GAME_REPLAY_START_REQ, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_TO_FIELD_MAP_ACK, KEGS_PARTY_TO_FIELD_MAP_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		SendPacket( EGS_PARTY_TO_FIELD_MAP_ACK, kPacket_ );
		return;
	}

	// 던전방으로 가서 필드로 이탈 처리 하자!
	//{{ 2010. 12. 28	최육사	하멜 마을 추가
#ifdef SERV_SHIP_DUNGEON
	KEGS_PARTY_TO_FIELD_MAP_REQ kPacket;
	kPacket.m_cPartyToFieldType = kPacket_.m_cPartyToFieldType;
	kPacket.m_iFieldStartPos = kPacket_.m_iFieldStartPos;
	SendToCnRoom( ERM_PARTY_TO_FIELD_MAP_REQ, kPacket );
#else
	SendToCnRoom( ERM_PARTY_TO_FIELD_MAP_REQ );
#endif SERV_SHIP_DUNGEON
	//}}	
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NAME_ACK, KEGS_PARTY_CHANGE_NAME_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NAME_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NAME_NOT, KEGS_PARTY_CHANGE_NAME_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NAME_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_PUBLIC_ACK, KEGS_PARTY_CHANGE_PUBLIC_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_PUBLIC_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_PUBLIC_NOT, KEGS_PARTY_CHANGE_PUBLIC_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_PUBLIC_NOT, kPacket_ );
}

//{{ 2011. 07. 19	최육사	대전 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_PARTY_CHANGE_HOST_ACK kPacket;	
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_iOldHostUnitUID = kPacket_.m_iOldHostUnitUID;
	kPacket.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;

	// 해당 파티가 맞는지 검사
	if( kPacket_.m_iPartyUID != GetPartyUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	// 해당 유저가 맞는지 검사
	if( kPacket_.m_iNewHostUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	switch( kPacket_.m_cPartyType )
	{
	case KPartyInfo::PT_DUNGEON_PARTY:
		{
			//바뀔파티장이 현재 설정된 던전을 할수있는지 확인.
			if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
			{
				START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_31;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}

			const int iDungeonID = kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty);
			const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode);

			if( CheckRequiredItemToEnterDungeonRoom( iDungeonID, eDungeonMode ) == false )
			{
				START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
					<< BUILD_LOG( kPacket_.m_iPartyUID )
					<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
					<< BUILD_LOG( GetCharUID() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_PARTY_31;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}

			const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
			if( iRequireDungeonID != 0 )
			{
				std::map< int, KDungeonClearInfo >::iterator mit;
				mit = m_mapDungeonClear.find( iRequireDungeonID );
				if( mit == m_mapDungeonClear.end() )
				{
					START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
						<< BUILD_LOG( kPacket_.m_iPartyUID )
						<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
						<< BUILD_LOG( GetCharUID() )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_PARTY_31;
					SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
					return;
				}
			}
		}
		break;

	case KPartyInfo::PT_PVP_PARTY:
		{
			// 대전 파티 제한 레벨
			if( GetLevel() < SEnum::PE_PVP_OFFICIAL_LIMIT_LEVEL )
			{
				START_LOG( cwarn, L"공식 대전을 하기에 부족한 캐릭터 레벨입니다." )
					<< BUILD_LOG( GetUID() )
					<< BUILD_LOG( GetCharUID() )
					<< BUILD_LOG( GetMatchUID() )
					<< BUILD_LOGc( GetLevel() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_MATCH_MAKING_11;
				SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
				return;
			}			
		}
		break;

	default:
		{
			START_LOG( cerr, L"파티 타입이 이상합니다!" )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOGc( kPacket_.m_cPartyType )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_27;
			SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
			return;
		}
		break;
	}

	// 모든 조건을 만족했다면 OK값을 보내자!
	kPacket.m_iOK = NetError::NET_OK;
	SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EPM_CHECK_PARTY_CHANGE_HOST_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	KEPM_CHECK_PARTY_CHANGE_HOST_ACK kPacket;	
	kPacket.m_iPartyUID = kPacket_.m_iPartyUID;
	kPacket.m_iOldHostUnitUID = kPacket_.m_iOldHostUnitUID;
	kPacket.m_iNewHostUnitUID = kPacket_.m_iNewHostUnitUID;

	// 해당 파티가 맞는지 검사
	if( kPacket_.m_iPartyUID != GetPartyUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	// 해당 유저가 맞는지 검사
	if( kPacket_.m_iNewHostUnitUID != GetCharUID() )
	{
		START_LOG( cerr, L"파티 소속이 아닌데 파티장 변경 요청이 들어왔군." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	//바뀔파티장이 현재 설정된 던전을 할수있는지 확인.
	if( CheckingSpirit( kPacket_.m_iDungeonID ) == false )
	{
		START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_31;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	const int iDungeonID = kPacket_.m_iDungeonID + static_cast<int>(kPacket_.m_cDifficulty);
	const CXSLDungeon::DUNGEON_MODE eDungeonMode = static_cast<CXSLDungeon::DUNGEON_MODE>(kPacket_.m_cDungeonMode);

	if( CheckRequiredItemToEnterDungeonRoom( iDungeonID, eDungeonMode ) == false )
	{
		START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PARTY_31;
		SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
		return;
	}

	const int iRequireDungeonID = SiCXSLDungeonManager()->GetRequireDungeonID( kPacket_.m_iDungeonID );
	if( iRequireDungeonID != 0 )
	{
		std::map< int, KDungeonClearInfo >::iterator mit;
		mit = m_mapDungeonClear.find( iRequireDungeonID );
		if( mit == m_mapDungeonClear.end() )
		{
			START_LOG( cwarn, L"새로 임명될 유저가 현재 설정된 던전을 수행할수없음." )
				<< BUILD_LOG( kPacket_.m_iPartyUID )
				<< BUILD_LOG( kPacket_.m_iNewHostUnitUID )
				<< BUILD_LOG( GetCharUID() )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_PARTY_31;
			SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
			return;
		}
	}

	// 모든 조건을 만족했다면 OK값을 보내자!
	kPacket.m_iOK = NetError::NET_OK;
	SendToCnParty( EPM_CHECK_PARTY_CHANGE_HOST_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PVP_NEW_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_ACK, KEGS_PARTY_CHANGE_HOST_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_HOST_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_HOST_NOT, KEGS_PARTY_CHANGE_HOST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_HOST_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_BAN_USER_ACK, KEGS_PARTY_BAN_USER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_BAN_USER_ACK, kPacket_ );

	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_ACK, KEGS_PARTY_CHANGE_NUM_OF_PER_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_NUM_OF_PER_NOT, KEGS_PARTY_CHANGE_NUM_OF_PER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_NUM_OF_PER_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_ACK, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_CHANGE_GET_ITEM_TYPE_NOT, KEGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_GET_ITEM_TYPE_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_BAN_USER_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	//{{ 2010. 02. 09  최육사	빠른파티 기능개선
#ifdef SERV_QUICK_PARTY_UPGRADE
	AddKickedPartyUID( GetPartyUID() );
#endif SERV_QUICK_PARTY_UPGRADE
	//}}

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
	//{{ 2009. 3. 18  최육사	던전으로 가서 유저를 이탈시키자!
	if( GetRoomUID() > 0 )
	{
		//{{ 2010. 03. 17  최육사	상태 오류 수정
		if( CXSLRoom::GetRoomType( GetRoomUID() ) == CXSLRoom::RT_DUNGEON )
		{
			SendToCnRoom( ERM_PARTY_BAN_USER_REQ );
		}
		else
		{
			START_LOG( cerr, L"던전방이 아닌데 파티를 맺고 있었네?" )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
				<< END_LOG;
		}
		//}}
	}
	else
	{
		SendID( EGS_PARTY_BAN_USER_NOT );
	}

	//{{ 2011. 07. 08	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	SendLeavePVPMatch( NetError::NOT_LEAVE_MATCH_WAIT_REASON_02 );
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

_IMPL_ON_FUNC( EPM_PARTY_QUICK_JOIN_ACK, KEGS_PARTY_QUICK_JOIN_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 파티 퀵 조인 실패!
		SetQuickJoinPartyUID( 0 );
		kPacket_.m_iPartyUID = 0;
	}

	SendPacket( EGS_PARTY_QUICK_JOIN_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, KEGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( kPacket_.m_bCanEnterSecretStage == false )
	{
		SendPacket( EGS_DUNGEON_SECRET_STAGE_ENTER_CHECK_ACK, kPacket_ );
		return;
	}

	// 방으로 비밀스테이지 체크하러 가자!
	SendToCnRoom( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
}

_IMPL_ON_FUNC( EPM_CHAT_ACK, KEGS_CHAT_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHAT_ACK, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHAT_NOT, KEGS_CHAT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHAT_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_CHANGE_PARTY_FEVER_STATE_NOT, KEGS_CHANGE_PARTY_FEVER_STATE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_CHANGE_PARTY_FEVER_STATE_NOT, kPacket_ );
}

_IMPL_ON_FUNC( EPM_PARTY_FEVER_POINT_NOT, KEGS_PARTY_FEVER_POINT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_FEVER_POINT_NOT, kPacket_ );
}

IMPL_ON_FUNC( EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 필드에 파티원 리스트를 업데이트 하러 가자!
	SendUpdatePartyMemberList( kPacket_.m_iPartyUID, kPacket_.m_setPartyUIDList );
}

IMPL_ON_FUNC( EPM_PARTY_TALKBOX_INFO_UPDATE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 현재 필드 상태인지 검사
	if( GetFieldUID() > 0 )
	{
		// 필드로 업데이트
		KERM_PARTY_TALKBOX_INFO_UPDATE_NOT kPacketNot;
		kPacketNot.m_bIsDelete = kPacket_.m_bIsDelete;
		kPacketNot.m_vecPartyInfo = kPacket_.m_vecPartyInfo;
		SendToGSField( ERM_PARTY_TALKBOX_INFO_UPDATE_NOT, kPacketNot );
	}
	else
	{
		// 방으로 바로 전송
		KEGS_PARTY_TALKBOX_INFO_NOT kPacketNot;
		kPacketNot.m_bIsDelete = kPacket_.m_bIsDelete;
		kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_vecPartyInfo = kPacket_.m_vecPartyInfo;
		SendPacket( EGS_PARTY_TALKBOX_INFO_NOT, kPacketNot );
	}
}

IMPL_ON_FUNC_NOPARAM( EPM_QUICK_SLOT_COOL_TIME_RESET_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 퀵슬롯 쿨 타임 리셋
	m_kInventory.ResetQuickSlotCoolTime();
}

#endif SERV_CHANNEL_PARTY

//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
IMPL_ON_FUNC( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetPartyUID() == 0 )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	// 현재 던전의 레벨에 부합 하지 않는 유저가 있는지 검사
	KGSPartyPtr spParty = SiKPartyListManager()->GetParty( GetPartyUID() );
	if( spParty == NULL )
	{
		START_LOG( cerr, L"파티 UID 이상.!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< END_LOG;

		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_PARTY_03;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	int iDungeonMinLevel = SiCXSLDungeonManager()->GetDungeonMinLevel( spParty->GetDungeonID() + static_cast<int>(spParty->GetDungeonDif()) );

	if( spParty->CheckPartyMinLevel( iDungeonMinLevel ) == false )
	{
		KEGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK kAck;
		kAck.m_iOK = NetError::ERR_CHANGE_PARTY_MIN_LEVEL_CHECK_FAILE;
		SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kAck );
		return;
	}

	//{{ 2010. 02. 23  최육사	채널 통합 파티

	if( SiKPartyListManager()->E_ChangeCheckMinLevel( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 최소 레벨 입장 제한 설정 실패!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetPartyUID() )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK, kPacket_ );
}

IMPL_ON_FUNC( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	SendPacket( EGS_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT, kPacket_ );
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
IMPL_ON_FUNC( EPM_CHANGE_COME_BACK_BUFF_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );
	
	CTime tComeBackBuffEnd;

	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrComeBackBuffEnd, tComeBackBuffEnd ) == false )
	{
		tComeBackBuffEnd = CTime( 0 );
		START_LOG( cerr, L"시간 변환 실패!!" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetPartyUID() )
			<< BUILD_LOG( kPacket_.m_wstrComeBackBuffEnd )
			<< END_LOG;
	}

	KEGS_COME_BACK_BUFF_NOT kNot;
	kNot.m_bIsFinish = kPacket_.m_bIsBuffFinish;
	kNot.m_iComeBackBuffEnd = tComeBackBuffEnd.GetTime();
	SendPacket( EGS_COME_BACK_BUFF_NOT, kNot );
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//////////////////////////////////////////////////////////////////////////
#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

