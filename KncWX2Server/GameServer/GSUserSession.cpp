#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"
#include "GSSimLayer.h"

#include "BaseServer.h"
#include "UserManager.h"
//{{ 2009. 7. 16  최육사	채널매니저
#include "ChannelManager.h"
//}}

#include "NetLayer.h"
#include "Socket/NetCommon.h"
#include "odbc/Odbc.h"
#include "Enum/Enum.h"

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
	#include "RewardTable.h"
#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
#include "XSLXmlParser.h"
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
	#include "NexonSOAPManager.h"
#endif SERV_CHANNELING_SOAP
//}}

//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}

//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	#include "ServerSNMaker.h"
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
//}}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	#include "X2Data/XSLSquareUnit.h"
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

#ifdef SERV_CHANNELING_USER_MANAGER
#include "ChannelingUserManager.h"
#endif SERV_CHANNELING_USER_MANAGER

//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
	#include "SMSPhoneNumberManager.h"
#endif SERV_POST_COPY_BUG_FIX
//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
	#include "X2Data/XSLBattleFieldManager.h"
#endif // SERV_BATTLE_FIELD_BOSS

//////////////////////////////////////////////////////////////////////////
//#ifdef SERV_GSUSER_CPP
//#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

KMaxProcessTime KGSUser::ms_kMaxLoginTime( 3 );

//{{ 2012. 02. 27	김민성	인벤토리 정렬	- 무거운 기능을 가볍게 변경
#ifdef SERV_SORT_CATEGORY_ITEM_FIX
KMaxProcessTime KGSUser::ms_kMaxInventorySortTime( 3 );
#endif SERV_SORT_CATEGORY_ITEM_FIX
//}}

IMPL_ON_FUNC( DBE_UPDATE_UNIT_INFO_ACK )
{
	// 상태를 막아야 하나?

	//////////////////////////////////////////////////////////////////////////
	// DB업데이트 실패에 대한 롤백처리

	m_kEXP.AdjustInitExp( m_kEXP.GetInitExp() - kPacket_.m_iEXP );
	//{{ 2012. 10. 02	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	m_kEDManager.RollBackDBUpdateInfo( kPacket_.m_iED );
#else
	m_iED.AdjustInitValue( m_iED.GetInitValue() - kPacket_.m_iED );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}	
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	m_kUserPvpManager.CheckRollbackInfo(  kPacket_.m_iOfficialMatchCnt
										, kPacket_.m_iRating
										, kPacket_.m_iMaxRating
										, kPacket_.m_iRPoint
										, kPacket_.m_iAPoint
										, kPacket_.m_iWin
										, kPacket_.m_iLose

										//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
										, kPacket_.m_fKFactor
#endif SERV_2012_PVP_SEASON2
										//}}
										);
#else
	m_iVSPoint.AdjustInitValue( m_iVSPoint.GetInitValue() - kPacket_.m_iVSPoint );
	m_iVSPointMax.AdjustInitValue( m_iVSPointMax.GetInitValue() - kPacket_.m_iVSPointMax );
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	std::vector< int >::const_iterator vitDungeonClear;
	std::map< int, KDungeonClearInfo >::iterator mitDungeonClear;

	for( vitDungeonClear = kPacket_.m_vecDungeonClear.begin(); vitDungeonClear != kPacket_.m_vecDungeonClear.end(); ++vitDungeonClear )
	{
		mitDungeonClear = m_mapDungeonClear.find( *vitDungeonClear );
		if( mitDungeonClear != m_mapDungeonClear.end() )
		{
			mitDungeonClear->second.m_bNew = true;
		}
		else
		{
			START_LOG( cerr, L"던전 클리어 정보가 있어야하는데 없음." )
				<< BUILD_LOG( *vitDungeonClear )
				<< END_LOG;
		}
	}

	std::vector< int >::const_iterator vitTCClear;
	std::map< int, KTCClearInfo >::iterator mitTCClear;
	for( vitTCClear = kPacket_.m_vecTCClear.begin(); vitTCClear != kPacket_.m_vecTCClear.end(); ++vitTCClear )
	{ 
		mitTCClear = m_mapTCClear.find( *vitTCClear );
		if( mitTCClear != m_mapTCClear.end() )
		{
			mitTCClear->second.m_bNew = true;
		}
		else
		{
			START_LOG( cerr, L"훈련소 클리어 정보가 있어야하는데 없음." )
				<< BUILD_LOG( *vitTCClear )
				<< END_LOG;
		}
	}

	m_kInventory.RollBackInitQuantity( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange );
	m_kInventory.RollBackDeletedItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted );
	m_kInventory.RollBackInitEndurance( kPacket_.m_kItemEnduranceUpdate );
	m_kInventory.PositionUpdateComplete( kPacket_.m_kItemPositionUpdate );
	m_iNumResurrectionStone.AdjustInitValue( m_iNumResurrectionStone.GetInitValue() - kPacket_.m_iNumResurrectionStone );
	//{{ 2009. 10. 14  최육사	자동결제 부활석
#ifdef AP_RESTONE
	m_iNumAutoPaymentResStone.AdjustInitValue( m_iNumAutoPaymentResStone.GetInitValue() - kPacket_.m_iNumAutoPaymentResStone );
#endif AP_RESTONE
	//}}

	//{{ 2010. 8. 10	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	m_kUserPetManager.RollBackDBUpdatePetInfo( kPacket_.m_vecPet );
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	m_kUserLocalRankingInfo.AdjustInitValue( kPacket_.m_iChangedLocalRankingSpirit, kPacket_.m_iChangedLocalRankingAP );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	m_kUserRidingPetManager.DBUpdateFailedProcess( kPacket_.m_vecRidingPetUID );
#endif	// SERV_RIDING_PET_SYSTM

	//////////////////////////////////////////////////////////////////////////
	// 후처리

	switch( kPacket_.m_iEventID )
	{
	case 0: break;
	case EGS_STATE_CHANGE_SERVER_SELECT_REQ:
		{
			KEGS_STATE_CHANGE_SERVER_SELECT_ACK kPacket;
			kPacket.m_iOK = NetError::NET_OK;
			SendPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK, kPacket );

			// 유닛리스트에서 삭제
			GetKUserManager()->UnregCharInfo( *this );

			// DB업데이트 했으니 유닛정보 초기화
			ResetUnitInfo();

			StateTransition( KGSFSM::I_TO_SERVER_SELECT );
		}
		break;

	case EGS_DISCONNECT_FOR_SERVER_SELECT_REQ:
		{
			// 유닛리스트에서 삭제
			GetKUserManager()->UnregCharInfo( *this );

			// DB업데이트 했으니 유닛정보 초기화
			ResetUnitInfo();

			// 로그인 서버로 유저정보 삭제하러 가자
			SendDisconnectEvent( EGS_DISCONNECT_FOR_SERVER_SELECT_REQ );
		}
		break;

	case EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ:
		{
			// 유닛리스트에서 삭제
			GetKUserManager()->UnregCharInfo( *this );

			// DB업데이트 했으니 유닛정보 초기화
			ResetUnitInfo();

			// 로그인 서버로 유저정보 삭제하러 가자
			SendDisconnectEvent( EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ );
		}
		break;
		//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
	case ERM_APPROVE_TRADE_REQ:
		{
			_SendToCnRoom( GetTradeUID(), ERM_APPROVE_TRADE_REQ, char() );
		}
		break;
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
		//}}
		//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
	case EGS_BUY_PERSONAL_SHOP_ITEM_REQ:
		{
			KERM_BUY_PERSONAL_SHOP_ITEM_REQ kPacket;
			GetPersonalShopBuyItemInfo( kPacket );
			_SendToPShopRoom( GetPersonalShopUID(), ERM_BUY_PERSONAL_SHOP_ITEM_REQ, kPacket );
		}
		break;
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
		//}}
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	case EGS_CREATE_PSHOP_AGENCY_REQ:
		{
			// 대리 상인 개설하러 갑시당!
			KERM_OPEN_PSHOP_AGENCY_REQ kPacket;
			kPacket.m_iUserUID = GetUID();
			kPacket.m_iED = GetED();
			kPacket.m_wstrNickName = GetCharName();
			kPacket.m_wstrAgencyExpirationDate = m_kUserPersonalShopManager.GetAgencyExpirationDate();
#ifdef SERV_UPGRADE_TRADE_SYSTEM
            kPacket.m_cPersonalShopType = m_kUserPersonalShopManager.GetPShopType();
#endif //SERV_UPGRADE_TRADE_SYSTEM                        
			SendToLoginRoom( ERM_OPEN_PSHOP_AGENCY_REQ, kPacket );
		}break;
	case EGS_PICK_UP_FROM_PSHOP_AGENCY_REQ:
		{
			KEGS_PICK_UP_FROM_PSHOP_AGENCY_REQ kPacket;
			GetAgencyShopPickUpItemInfo( kPacket.m_vecPickUpItemList );
			_SendToLoginRoom( GetPShopAgencyUID(), ERM_PICK_UP_FROM_PSHOP_AGENCY_REQ, kPacket );
		}
		break;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}	
	default:
		START_LOG( cerr, L"정의되지 않은 이벤트가 왔습니다." )
			<< BUILD_LOG( kPacket_.m_iEventID )
			<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_iEventID ) )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( EGS_CONNECT_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_wstrVersion );

	KEGS_CONNECT_ACK kPacket;

	// 최초 게임서버 접속시 버전 비교
	if( OnConnectReq( kPacket_, kPacket ) == false )
	{
		START_LOG( cwarn, L"connect error" )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;
	}

	//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	kPacket.m_bUDPKickOff = GetKGSSimLayer()->GetUDPKickOff();
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

	SendPacket( EGS_CONNECT_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_VERIFY_ACCOUNT_REQ )
{
	//{{ 2010. 9. 2	최육사	중폭 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_INIT ), EGS_VERIFY_ACCOUNT_REQ, EGS_VERIFY_ACCOUNT_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_INIT ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

#ifdef SERV_CHANNELING_USER_MANAGER
	SiKChannelingUserManager()->UpdateChannelingUserList( true, kPacket_.m_iChannelingCode, GetGameServerLoginUser() );
	SetGameServerLoginUser( true );
#endif SERV_CHANNELING_USER_MANAGER

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_bDebugAuth )
		<< BUILD_LOG( kPacket_.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_wstrPasswd );

	//kPacket_.m_bDebugAuth = false; 
	//kPacket_.m_wstrPasswd = L"NGP10:trZUblcTlLAQ3zLkq7XCTTbK7YuMAoeHpFbnV8bTg0B3z6Y4e7Zogt8KTFFE0xvN9EnwoFBcxaSHAPxugwHeR~yMccp8NRozrgqagLsa_x3sNE7fmR8pmggGMKk8_0linaFwxVdiYKIxCk0jn4Th2uAVVrK2bP10u_EcLI_pMOGcgEZp4~DEo3mr8R98tTacbvfELPYKpvZAgwTqm0qZkMiKFA3tUPCGe7~sYzl4VrE1Jo1i";

#ifdef SERV_GLOBAL_AUTH
	kPacket_.m_bDebugAuth = false; // 해외팀에서는 bDebugAuth를 이용한 예외상황 고려안함
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_STEAM
	SetSteamClientUser( kPacket_.m_bSteamClient );
	START_LOG( clog, L"[TEST] 스팀 클라 유저로 등록")
		<< BUILD_LOG( IsSteamClientUser())
		<< END_LOG;
#endif //SERV_STEAM

#ifdef SERV_CHANNELING_AERIA
	SetAeriaClientUser( kPacket_.m_bAeriaClient );
	START_LOG( clog, L"[TEST] 아리아 클라 유저로 등록")
		<< BUILD_LOG( IsAeriaClientUser())
		<< END_LOG;
#endif //SERV_CHANNELING_AERIA

	KELG_NEXON_USER_AUTHENTICATE_REQ kPacket;
	kPacket.m_kAuthenticateReq = kPacket_;
	kPacket.m_kNexonAccountInfo.m_wstrLoginIP = KncUtil::toWideString( KSession::GetIPStr() );

#ifdef SERV_GLOBAL_AUTH
	kPacket.m_kNexonAccountInfo.m_uChannelCode = static_cast<UINT8>(kPacket.m_kAuthenticateReq.m_iChannelingCode);
#endif SERV_GLOBAL_AUTH

#ifdef SERV_STEAM
	START_LOG( clog, L"[TEST] 채널링 코드 출력")
		<< BUILD_LOG( kPacket_.m_iChannelingCode )
		<< END_LOG;
#endif //SERV_STEAM

#ifdef SERV_COUNTRY_TH
	kPacket.m_wstrSocketID = kPacket_.m_wstrSocketID;
	START_LOG( clog, L"UserSession인증 요청." )
		<< BUILD_LOG( kPacket_.m_wstrSocketID )
		<< END_LOG;
#endif // SERV_COUNTRY_TH

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUserID ) )
	{
		KEGS_VERIFY_ACCOUNT_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}

	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )
	{
		if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrPasswd ) )
		{
			KEGS_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_ODBC_00;
			SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );
			return;
		}
	}

	//{{ 2012. 03. 27	김민성		머신 시간을 검사하자(50분 이상 차이나면 모니터링)
#ifdef SERV_CHECK_MACHINE_LOCALTIME
	SYSTEMTIME st;
	GetSystemTime( &st );

	if( CheckMachineLocalTime( st, kPacket_.m_wstrClientTime ) == false )
	{
		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_LOCALTIME;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_LOCALTIME;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
		
// 		KEGS_VERIFY_ACCOUNT_ACK kAck;
// 		kAck.m_iOK = NetError::ERR_CLIENT_QUIT_01;
// 		SendPacket( EGS_VERIFY_ACCOUNT_ACK, kAck );
// 
// 		Kick( NetError::ERR_CLIENT_QUIT_01 );
//		return;
	}
#endif SERV_CHECK_MACHINE_LOCALTIME
	//}}

	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// 오픈 테섭에서는 검사 제외
	{
		if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
		{
			if( m_kUserSecurityManager.CheckValidMachineID( kPacket_.m_strMachineID ) == false )
			{
				START_LOG( cout, L"[알림] 머신ID가 유효하지 않다! 클라이언트로 알림 패킷을 쏘자!" )
					<< BUILD_LOG( kPacket_.m_wstrHWID )
					<< BUILD_LOG( kPacket_.m_wstrUserID )
					<< BUILD_LOG( kPacket_.m_wstrPasswd )
					<< BUILD_LOG( kPacket_.m_bDebugAuth );

				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
				kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
				kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_ID;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				
				//{{ 2011. 09. 06	최육사	머신 ID 기능 업데이트
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
				// 머신ID가 비어있으면 접속 차단!
				Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
#endif SERV_MACHINE_ID_BLOCK_UPDATE
				//}}
			}
		}
	}

	// 머신ID 저장!
	m_kUserSecurityManager.SetMachineID( kPacket_.m_strMachineID );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	SetChannelRandomKey( kPacket_.m_kChannelRandomKey );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}

	//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	if( SiKGameSysVal()->GetCheckServerSN() == true )
	{
		// 클라가 전달해준 sn 이 있다
		if( KServerSNMaker::IsEmptyServerSN( kPacket_.m_charServerSN ) == false )
		{
			// 검사해 볼까?
			// 데이터 유효성 검사
			__int64 iServerSN = 0;
			if( KServerSNMaker::CheckServerSN( kPacket_.m_charServerSN, iServerSN ) == false )
			{
				START_LOG( cerr, L"유효하지 않은 ServerSN을 클라가 줬어!" )
					<< BUILD_LOG( kPacket_.m_wstrUserID )
					<< BUILD_LOG( iServerSN );

				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				// 클라가 파일에서 읽은 12바이트 정보가 변조 되었다.
				KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
				kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#else
				// 클라가 파일에서 읽은 12바이트 정보가 변조 되었다.
				KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
				kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				
				// 머신ID가 비어있으면 접속 차단!
				Kick( KStatistics::eSIColDR_Check_ServerSN_In_GameServer );				
			}

			// DB에 존제하는 ServerSN 인지 확인하자
			KDBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ kCheckSN;
			kCheckSN.m_iServerSN = iServerSN;
			kCheckSN.m_strMachineID = kPacket_.m_strMachineID;
			kCheckSN.m_kAuthenticateReq = kPacket.m_kAuthenticateReq;
			kCheckSN.m_kNexonAccountInfo = kPacket.m_kNexonAccountInfo;
			
			SendToAccountDB( DBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ, kCheckSN );
			return;

		}
		else // 클라가 전달해준 sn 이 없다
		{
			// channel server 를 거쳐서 왔다면 없을리가 없다.
			// 짜르자

			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_EMPTY;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
#else
			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_EMPTY;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#else
			KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
			
			// 머신ID가 비어있으면 접속 차단!
			Kick( KStatistics::eSIColDR_Check_ServerSN_In_GameServer );
		}
	}
	else
	{
		SendToLoginServer( ELG_NEXON_USER_AUTHENTICATE_REQ, kPacket );

		m_kTimer[TM_INIT].restart();
	}
#else
	SendToLoginServer( ELG_NEXON_USER_AUTHENTICATE_REQ, kPacket );

	m_kTimer[TM_INIT].restart();
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
}

//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
_IMPL_ON_FUNC( DBE_CHECK_SERVER_SN_IN_GAMESERVER_ACK, KDBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ )
{
	switch( kPacket_.m_iOK )
	{
	case NetError::NET_OK:
		{
			//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
			if( kPacket_.m_iMonitoringFlag == KUserSecurityManager::SSM_MONITORING )
			{
				m_kUserSecurityManager.SetServerSN_Flag( kPacket_.m_iMonitoringFlag );
			}
			else if( kPacket_.m_iMonitoringFlag == KUserSecurityManager::SSM_BLOCK )
			{
				// 추후 작업
			}
#endif SERV_SERIAL_NUMBER_MORNITORING
			//}}
		}break;
	case NetError::ERR_SERVERSN_01:
		{
			if( SiKGameSysVal()->GetCheckServerSN_ERR_01_1() == true )
			{
				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kNot );
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kNot );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				
				// 머신ID가 비어있으면 접속 차단!
				Kick( KStatistics::eSIColDR_Check_ServerSN_In_GameServer );

				START_LOG( cerr, L"channel server 를 거쳐서 왔다면 틀릴 수가 없다!" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< END_LOG;
			}
		}break;
	case NetError::ERR_SERVERSN_02:
		{
			if( SiKGameSysVal()->GetCheckServerSN_ERR_02() == true )
			{
				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kNot );
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kNot );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				
				// 머신ID가 비어있으면 접속 차단!
				Kick( KStatistics::eSIColDR_Check_ServerSN_In_GameServer );

				START_LOG( cerr, L"channel server 를 거쳐서 왔다면 틀릴 수가 없다!" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< END_LOG;
			}
		}break;
	case NetError::ERR_SERVERSN_05:
		{
			if( SiKGameSysVal()->GetCheckServerSN_ERR_01_2() == true )
			{
				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kNot );
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
				kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kNot );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}
				
				// 머신ID가 비어있으면 접속 차단!
				Kick( KStatistics::eSIColDR_Check_ServerSN_In_GameServer );

				START_LOG( cerr, L"channel server 를 거쳐서 왔다면 틀릴 수가 없다!" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< END_LOG;
			}
		}break;
	case NetError::ERR_SERVERSN_07:
		{
			START_LOG( clog, L"재제된 SN 입니다." )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
				<< BUILD_LOG( kPacket_.m_iServerSN )
				<< BUILD_LOG( kPacket_.m_strMachineID )
				<< END_LOG;

			KEGS_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_SERVERSN_07;
			SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );
			return;
		}break;
	default:
		{
			START_LOG( cerr, L"발생 할 수 없는 경우인데...." )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
				<< BUILD_LOG( kPacket_.m_iServerSN )
				<< BUILD_LOG( kPacket_.m_strMachineID )
				<< END_LOG;

			KEGS_VERIFY_ACCOUNT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_VERIFY_17;
			SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );

			return;
		}break;
	}
	

	/*
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		if( kPacket_.m_iOK != NetError::ERR_SERVERSN_03 )
		{
			// channel server 를 거쳐서 왔다면 없을리가 없다.
			// 짜르자
			KEGS_SERVER_CHECK_HACK_USER_NOT kNot;
			kNot.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHANGE_SERVER_SN_IN_GAMESERVER_SEND_MAIL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kNot );

			// 머신ID가 비어있으면 접속 차단!
			Kick( KStatistics::eSIColDR_Check_ServerSN_In_GameServer );

			START_LOG( cerr, L"channel server 를 거쳐서 왔다면 틀릴 수가 없다!" )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
				<< BUILD_LOG( kPacket_.m_iServerSN )
				<< BUILD_LOG( kPacket_.m_strMachineID )
				<< END_LOG;

			return;
		}
	}
	*/

	//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	m_kUserSecurityManager.SetServerSN( kPacket_.m_iServerSN );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
	//}}
	KELG_NEXON_USER_AUTHENTICATE_REQ kPacket;
	kPacket.m_kAuthenticateReq = kPacket_.m_kAuthenticateReq;
	kPacket.m_kNexonAccountInfo = kPacket_.m_kNexonAccountInfo;

	SendToLoginServer( ELG_NEXON_USER_AUTHENTICATE_REQ, kPacket );

	m_kTimer[TM_INIT].restart();
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
//}}

IMPL_ON_FUNC( ELG_NEXON_USER_AUTHENTICATE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_INIT ) );

	KEGS_VERIFY_ACCOUNT_ACK kPacket;

	//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
	if( kPacket_.m_iOK == NetError::ERR_VERIFY_24 )
	{
		// 인증은 성공 했지만 ATL 2.0 이 아니라면
		START_LOG( cerr, L"통합 맴버십 ATL 등급 확인 결과" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = kPacket_.m_iOK;

		SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
	//}}

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	// -58566 은  채널 서버에서 유저의 id 를 알지 못할 경우 랜덤 키를 발급 할 수 없기 때문에
	// 매직 넘버를 사용한다.
	if( GetChannelRandomKey() != -58566 && kPacket_.m_kAccountInfo.m_iChannelRandomKey != GetChannelRandomKey() )
	{
		kPacket.m_iOK = NetError::ERR_VERIFY_17;

		START_LOG( cerr, L"채널 서버 인증키가 맞지 않습니다." )
			<< BUILD_LOG( kPacket_.m_kAccountInfo.m_wstrID )
			<< BUILD_LOG( kPacket_.m_kAccountInfo.m_iChannelRandomKey )
			<< BUILD_LOG( GetChannelRandomKey() )
			<< END_LOG;

		SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}


	// 넥슨 인증 결과 처리
	if( OnNexonUserAuthenticateAck( kPacket_, kPacket ) == false )
	{
		START_LOG( cerr, L"넥슨 인증 실패!" )
			<< BUILD_LOG( kPacket_.m_kAccountInfo.m_wstrID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;

		SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );
		return;
	}

#ifdef SERV_EPAY_SYSTEM
	// 계정 타입
	m_GiantAccountType.m_uiType = kPacket_.m_uiGiantAccountType;// SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

	m_wstrUserID = kPacket_.m_kAccountInfo.m_wstrID;
	m_usGiantZone = kPacket_.m_usZone;
	m_usGiantGame = kPacket_.m_usGame;
	START_LOG( clog, L"epay관련 로그 & 계정 타입 설정" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_GiantAccountType.m_uiType )
		<< BUILD_LOG( m_wstrUserID )
		<< BUILD_LOG( m_usGiantZone )
		<< BUILD_LOG( m_usGiantGame )
		<< END_LOG;
#endif //SERV_EPAY_SYSTEM

	// 로그인 서버에 유저정보 등록하기
	KELG_REGISTER_USER_REQ kPacketReq;
	kPacketReq.m_wstrUserID = GetName();	
	kPacketReq.m_iChannelID = SiKChannelManager()->GetChannelID();
	kPacketReq.m_uiIP		= GetIP();
	kPacketReq.m_byteCanTry = 0;
	kPacketReq.m_byteGuestUser = m_kNexonAccountInfo.m_byteGuestUser;
	kPacketReq.m_cAuthLevel = GetAuthLevel();
	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	kPacketReq.m_ucChannelCode = kPacket_.m_kNexonAccountInfo.m_uChannelCode;
	if( GetAuthLevel() >= SEnum::UAL_DEVELOPER )
	{
		START_LOG( cout, L"[확인] 채널링 코드 확인" )
			<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_uChannelCode )
			<< BUILD_LOG( m_kNexonAccountInfo.m_uChannelCode )
			<< BUILD_LOG( kPacketReq.m_ucChannelCode );
	}
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	//}}
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	kPacketReq.m_strMachineID = m_kUserSecurityManager.GetStringMachineID();
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	kPacketReq.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

#ifdef SERV_COUNTRY_TH
	kPacketReq.m_wstrSocketID = kPacket_.m_wstrSocketID;
	START_LOG( clog, L"UserSession인증 요청." )
		<< BUILD_LOG( kPacket_.m_wstrSocketID )
		<< END_LOG;
#endif //SERV_COUNTRY_TH

#ifdef SERV_COUNTRY_PH 
	m_usGarenaCyberCafe = kPacket_.m_usGarenaCyberCafe; // 2013.08.01 lygan_조성욱 // 동남아시아 가레나 플랫폼으로 부터 받은 PC 방 정보
#endif //SERV_COUNTRY_PH

	SendToLoginServer( ELG_REGISTER_USER_REQ, kPacketReq );

	//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
 	KEGB_REGISTER_SERVERSN_REQ kReq;
 	kReq.m_iUserUid = GetUID();
 	m_kUserSecurityManager.GetServerSN( kReq.m_iServerSN );
 	SendToGlobalServer( EGB_REGISTER_SERVERSN_REQ, kReq );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
	//}}

#ifdef SERV_NEW_PUNISHMENT
	// 처벌 기록 얻어오기
	SendToAccountDB( DBE_GET_PUNISHMENT_REQ, GetUID() );
#endif SERV_NEW_PUNISHMENT

	// 클라이언트로 결과 통보
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_kAccountInfo = kPacket_.m_kAccountInfo;
	kPacket.m_kAccountInfo.m_bIsGuestUser = IsGuestUser(); // 체험아이디 정보	
	//{{ 2010. 01. 11	김민성	ip추가
#ifdef SERV_CLIENT_PUBLIC_IP
	kPacket.m_wstrClientIP = KncUtil::toWideString( GetIPStr() );
#endif SERV_CLIENT_PUBLIC_IP
		//}}

	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	kPacket.m_uChannelCode = kPacket_.m_kNexonAccountInfo.m_uChannelCode;
	kPacket.m_wstrChannelUserID = kPacket_.m_kNexonAccountInfo.m_wstrChannelUID;
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	//}}

	//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
	if( kPacket_.m_kNexonAccountInfo.m_uChannelCode == KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
	{
		KSOAP_GET_TOONILAND_ID_REQ kSOAPReq;
		kSOAPReq.m_wstrNexonID = kPacket_.m_kNexonAccountInfo.m_wstrID;

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData(PI_NULL, anTrace, SOAP_GET_TOONILAND_ID_REQ, kSOAPReq );

		SiKNexonSOAPManager()->QueueingEvent( spEvent );
	}
#endif SERV_CHANNELING_SOAP
	//}}

	//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
	SetAccountInfo( kPacket_.m_kAccountInfo );
#endif SERV_NEW_CREATE_CHAR_EVENT
		//}}
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	SetEventMoney( kPacket_.m_iEventMoney );
	kPacket.m_iEventMoney = GetEventMoney();
#endif // SERV_EVENT_MONEY
	SendPacket( EGS_VERIFY_ACCOUNT_ACK, kPacket );

	StateTransition( KGSFSM::I_TO_UID_UPDATED );
	ms_kMaxLoginTime.CheckMaxProcessTime( GetName(), ( float )m_kTimer[TM_INIT].elapsed() );

	//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
	if( kPacket_.m_bJoinAccount == true )
	{
		if( SiKAbuserLogManager()->JoinAccountCheckIP( kPacket_.m_kNexonAccountInfo.m_wstrLoginIP ) == true )
		{
			KDBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT kPacketToLog;
			kPacketToLog.m_iUserUID = GetUID();
			kPacketToLog.m_wstrUserID = GetName();
			kPacketToLog.m_wstrLoginIP = kPacket_.m_kNexonAccountInfo.m_wstrLoginIP;
			SendToAccountDB( DBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT, kPacketToLog );
		}
	}
#endif SERV_IP_ACCOUNT_LOG
	//}}

	//080624.hoons.인증패킷 처리를 다해주어야 클라이언트에서 nProtect가 동작함.(해킹유저만)
	//if( m_cHackingUserType == SEnum::HUT_AGREE_HACK_USER )
	//{
	//	// nProtect 한국인증 모듈 사용설정
	//	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CommonFlag::CF_NPGG ) )
	//	{
	//		m_kGGAuth.SetEnable( true );
	//	}
	//	m_kGGAuth.Init();
	//}

	//korea nProtect 초기 인증을 클라이언트에 보낸다.
	//if( m_kGGAuth.FirstAuthQuery( GetThisPtr<KGSUser>() ) == false )
	//{
	//	SetDisconnectReason( KStatistics::eSIColDR_NProtect_Auth_First );
	//	ReserveDestroy();

	//	START_LOG( cerr, L"nProtect 첫인증 실패.!" )
	//		<< BUILD_LOG( GetUID() )
	//		<< BUILD_LOG( GetName() )
	//		<< END_LOG;
	//	return;
	//}

	//{{ 2010. 8. 11  최육사	개발자도 핵쉴드 체크 하도록 수정
	//if( GetAuthLevel() == SEnum::UAL_DEVELOPER )
	//{
	//	// 개발자는 HackShield끕니다.
	//	m_kHSAuth.SetEnable( false );
	//}

	if( m_kHSAuth.FirstHackShieldAuth( GetThisPtr<KGSUser>() ) == false )
	{
		SetDisconnectReason( KStatistics::eSIColDR_NProtect_Auth_First );
		ReserveDestroy();

		START_LOG( cerr, L"hack shield 첫인증 실패.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;
		return;
	}
	//}}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	m_kUserAbuserManager.InitAccountAbuser( GetThisPtr<KGSUser>() );
#endif SERV_USER_ABUSER_MANAGER
	//}}

	//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY

	// 오늘 6 이후 최초 접속이면
	CTime tLastLogin;
	CTime tNow = CTime::GetCurrentTime();
	CTime tRestart = CTime( tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 6, 0, 0 );		// 초기화 기점
	CTimeSpan tOneDay = CTimeSpan( 1, 0, 0, 0 );
	CTime tYesterRestart = tRestart - tOneDay;		// 이전 초기화 기점

	if( KncUtil::ConvertStringToCTime( kPacket_.m_kAccountInfo.m_wstrLastLogin, tLastLogin ) == false )
	{
		START_LOG( cerr, L"시간을 문자열로 변환 실패!!!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_kAccountInfo.m_wstrLastLogin )
			<< END_LOG;
		return;
	}

	if( GetIsUseOTP() == true ) // OTP 사용 유저인가?
	{
		if( tLastLogin >= tYesterRestart )	// 어제 받았다
		{
			if( tLastLogin < tRestart )	// 오늘 받을 조건이 된다
			{
				if( tRestart <= tNow )
				{
					m_kUserUnitSelectRewardManager.AddRewardItemInfo( KRewardTable::ERI_SECOND_SECURITY );
				}
			}
		}
		else
		{
			m_kUserUnitSelectRewardManager.AddRewardItemInfo( KRewardTable::ERI_SECOND_SECURITY );
		}
	}

#endif SERV_SECOND_SECURITY
	//}}

	//{{ 2012. 05. 17	박세훈	키보드 맵핑 정보를 DB에서 읽고 쓰기
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
	SendToAccountDB( DBE_KEYBOARD_MAPPING_INFO_READ_REQ, GetUID() );
#endif SERV_KEYBOARD_MAPPING_INFO_RW
	//}}

	//{{ 2012. 05. 29	박세훈	채팅 옵션 정보를 DB에서 읽고 쓰기
#ifdef SERV_CHAT_OPTION_INFO_RW
	SendToAccountDB( DBE_CHAT_OPTION_INFO_READ_REQ, GetUID() );
#endif SERV_CHAT_OPTION_INFO_RW
	//}}
}

IMPL_ON_FUNC( ELG_REGISTER_USER_ACK )
{
	// 게임서버 접속 시 퍼블인증이 없는 경우에만 타는 Flow 임
	// 게임서버 접속 시 퍼블인증이 없다는 것은 PC방 인증을 우리(KOG)쪽에서 처리한다는 의미임

	VERIFY_STATE( ( 1, KGSFSM::S_UID_UPDATED ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		Kick( kPacket_.m_iOK );
		return;
	}

	if( GetStateID() != KGSFSM::S_SERVER_SELECT )
	{
		StateTransition( KGSFSM::I_TO_LOGINED );
	}

	m_bLoginFail = false;

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	//PC방 인증
	int tempPcBangType = GetKGameServer()->IsPCBangIP( KncUtil::toWideString( KSession::GetIPStr() ) );
	if( tempPcBangType != 0)
	{
		bool bPreStatePCBang = m_kUserPcBangManager.IsPcBang(); // (PC방 통계에 중복 계산 되는거 막기 위해서)

		m_kUserPcBangManager.SetIsPcBang( true );
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(tempPcBangType);
#endif //SERV_PC_BANG_TYPE
		//{{ 2007. 11. 13  최육사  유저 통계 PC방
		if (false == bPreStatePCBang) //(PC방 통계에 중복 계산 되는거 막기 위해서)
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_IsPcBang,	1 );
		//}}

		START_LOG( clog2, L"PC방임..." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( KncUtil::toWideString( KSession::GetIPStr() ) );
	}
	else
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST
	{
		// 해당 플로우는 게임 서버 로그인 시 퍼블 인증을 받지 않는 경우에만 탄다.
		// 즉 PC방 관리를 우리가 하기 때문에 위 SERV_KOG_HAVE_PCBANG_IP_LIST 조건이 아니라면
		// 다 PC방이 아님
		m_kUserPcBangManager.SetIsPcBang( false );
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(-1); // PC방 아닌경우 -1값 0도 피씨방임
#endif //SERV_PC_BANG_TYPE

#ifdef SERV_COUNTRY_PH
		//if (m_usGarenaCyberCafe & 0x0001u)
		if ( (m_usGarenaCyberCafe & ( 1<<8)) >> 8 )
		{
			m_kUserPcBangManager.SetIsPcBang( true );	
#ifdef SERV_PC_BANG_TYPE
			m_kUserPcBangManager.SetPcBangType(0); // PC방 아닌경우 -1값 0도 피씨방임
#endif //SERV_PC_BANG_TYPE
		}
#endif //SERV_COUNTRY_PH

	}


	KENX_USER_LOGIN_NOT kNot;
	kNot.m_bIsGameBang = IsPcBang();
#ifdef SERV_PC_BANG_TYPE
	kNot.m_kPcBangReward = GetPcBangReward();
#endif SERV_PC_BANG_TYPE
	SendPacket( ENX_USER_LOGIN_NOT, kNot );

	// last login
	SendToAccountDB( DBE_UPDATE_LOGIN_TIME_NOT );

	// 모든 로그인 절차가 끝나고 GSUser의 State가 S_LOGINED로 바뀐후에 장바구니를 받아오자!
	KDBE_GET_WISH_LIST_REQ kPacketToDB;
	kPacketToDB.m_iUserUID = GetUID();
	SendToGameDB( DBE_GET_WISH_LIST_REQ, kPacketToDB );
}

#ifdef SERV_GLOBAL_AUTH

IMPL_ON_FUNC( ELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK )
{
	// 채널선택 접속이든 채널이동 접속이든 해외는 둘다 여기 코드 탐

	//////////////////////////////////////////////////////////////////////////
	// 상태 검사
	switch( GetStateID() )
	{
	case KGSFSM::S_UID_UPDATED:
	case KGSFSM::S_LOGINED:
	case KGSFSM::S_SERVER_SELECT:
	case KGSFSM::S_FIELD_MAP:
	case KGSFSM::S_ROOM:
		// 정상적으로 채널이동 중 PC방 인증 완료
		break;

	default:
		{
			START_LOG( cerr, L"이상한 상태. 로그인 처리를 무시합니다." )
				<< BUILD_LOG( GetStateID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;
		}
		return;
	}

#if !defined(SERV_COUNTRY_TW) && !defined(SERV_COUNTRY_HK)
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		Kick( kPacket_.m_iOK );
		return;
	}
#endif //!defined(SERV_COUNTRY_TW) || !defined(SERV_COUNTRY_HK)

	// 채널이동의 경우 KGSFSM::S_FIELD_MAP상태다
	if( GetStateID() != KGSFSM::S_SERVER_SELECT  &&
		GetStateID() != KGSFSM::S_FIELD_MAP  &&  
		GetStateID() != KGSFSM::S_ROOM )
	{
		StateTransition( KGSFSM::I_TO_LOGINED );
	}

#ifdef SERV_COUNTRY_TH
	//	여기서 소켓 ID를 등록한다. (로그 아웃시 사용한다. PC방 유저이면 중요함)
	if( kPacket_.m_wstrSockID != L"" )
		SetSocketID( kPacket_.m_wstrSockID );
#endif //SERV_COUNTRY_TH

	//PC방 인증
#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	int tempPcBangType = GetKGameServer()->IsPCBangIP( KncUtil::toWideString( KSession::GetIPStr() ) );
	if( tempPcBangType != 0)
	{
		bool bPreStatePCBang = m_kUserPcBangManager.IsPcBang(); // (PC방 통계에 중복 계산 되는거 막기 위해서)

		m_kUserPcBangManager.SetIsPcBang( true );
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(tempPcBangType);
#endif //SERV_PC_BANG_TYPE

		//{{ 2007. 11. 13  최육사  유저 통계 PC방
		if (false == bPreStatePCBang)	//(PC방 통계에 중복 계산 되는거 막기 위해서)
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_IsPcBang,	1 );
		//}}

		START_LOG( clog2, L"PC방임...PUBLISHER CHECK" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( KncUtil::toWideString( KSession::GetIPStr() ) );

	}
	else
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST
	{
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		if(kPacket_.m_bIsPcBang)
		{
			bool bPreStatePCBang = m_kUserPcBangManager.IsPcBang(); // (PC방 통계에 중복 계산 되는거 막기 위해서)

			m_kUserPcBangManager.SetIsPcBang( true );
			//{{ 2007. 11. 13  최육사  유저 통계 PC방
			if (false == bPreStatePCBang)	//(PC방 통계에 중복 계산 되는거 막기 위해서)
				m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_IsPcBang,	1 );
			//}}

#ifdef SERV_PC_BANG_TYPE
			m_kUserPcBangManager.SetPcBangType(kPacket_.m_iPCBangType);
#endif //SERV_PC_BANG_TYPE
			START_LOG( clog2, L"PC방맞음...PUBLISHER CHECK" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KncUtil::toWideString( KSession::GetIPStr() ) );
		}
		else
		{
			m_kUserPcBangManager.SetIsPcBang( false );
#ifdef SERV_PC_BANG_TYPE
			m_kUserPcBangManager.SetPcBangType(-1);	// PC방 아닌경우 -1값 0도 피씨방임
#endif //SERV_PC_BANG_TYPE
			START_LOG( clog2, L"PC방아님...PUBLISHER CHECK" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KncUtil::toWideString( KSession::GetIPStr() ) );
		}
//////////////////////////////////////////////////////////////////////////

#else //SERV_CHECK_PCBANG_BY_PUBLISHER
		m_kUserPcBangManager.SetIsPcBang( false );
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(-1);	// PC방 아닌경우 -1값 0도 피씨방임
#endif //SERV_PC_BANG_TYPE
		START_LOG( clog2, L"PC방아님...PUBLISHER CHECK" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( KncUtil::toWideString( KSession::GetIPStr() ) );
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER
	}

#ifdef SERV_COUNTRY_PH
	//if (m_usGarenaCyberCafe & 0x0001u)
	if ( (m_usGarenaCyberCafe & ( 1<<8)) >> 8 )
	{
		m_kUserPcBangManager.SetIsPcBang( true );	
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(0); // PC방 아닌경우 -1값 0도 피씨방임
#endif //SERV_PC_BANG_TYPE
	}
	else
	{
		m_kUserPcBangManager.SetIsPcBang( false );	
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(-1); // PC방 아닌경우 -1값 0도 피씨방임
#endif //SERV_PC_BANG_TYPE
	}
#endif //SERV_COUNTRY_PH


	// 채널이동의 경우 KGSFSM::S_FIELD_MAP상태다
	if( GetStateID() != KGSFSM::S_FIELD_MAP  &&  GetStateID() != KGSFSM::S_SERVER_SELECT )
	{
		StateTransition( KGSFSM::I_TO_LOGINED );
	}
	m_bLoginFail = false;


	KENX_USER_LOGIN_NOT kNot;
	kNot.m_bIsGameBang = IsPcBang();
#ifdef SERV_PC_BANG_TYPE
	kNot.m_kPcBangReward = GetPcBangReward();
#endif SERV_PC_BANG_TYPE
	SendPacket( ENX_USER_LOGIN_NOT, kNot );

	// last login
	SendToAccountDB( DBE_UPDATE_LOGIN_TIME_NOT );

	// 모든 로그인 절차가 끝나고 GSUser의 State가 S_LOGINED로 바뀐후에 장바구니를 받아오자!
	KDBE_GET_WISH_LIST_REQ kPacketToDB;
	kPacketToDB.m_iUserUID = GetUID();
	SendToGameDB( DBE_GET_WISH_LIST_REQ, kPacketToDB );
}
#endif // SERV_GLOBAL_AUTH

//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_PCBANG_AUTH_NEW
//////////////////////////////////////////////////////////////////////////

_IMPL_ON_FUNC( ELG_REGISTER_NEXON_USER_ACK, KENX_AUTH_LOGIN_ACK )
{
	//////////////////////////////////////////////////////////////////////////
	// 상태 검사
	switch( GetStateID() )
	{
	case KGSFSM::S_UID_UPDATED:
		// TIME OUT 되지 않고 정상적으로 PC방 인증 완료
		break;

	case KGSFSM::S_LOGINED:
	case KGSFSM::S_SERVER_SELECT:
	case KGSFSM::S_FIELD_MAP:
	case KGSFSM::S_ROOM:
		// 넥슨 PC방 인증 로그인 처리가 TIME OUT되었음!
		{
			// 만약 PC방 프리미엄을 받을 수 있는 유저라면?
			if( kPacket_.m_byteAuthorizeResult == 1 )
			{
				START_LOG( cout, L"인증 타임 아웃으로 인해 PC방 프리미엄 취소!" )
					<< BUILD_LOG( kPacket_.m_wstrUserID );

				// 인증 타임 아웃으로 인해 PC방 프리미엄 취소하고 일반 유저로 로그인 재 요청!
				KELG_NEXON_USER_LOGIN_TIME_OUT_NOT kNot;
				kNot.m_wstrUserID = GetName();
				SendToLoginServer( ELG_NEXON_USER_LOGIN_TIME_OUT_NOT, kNot );
				return;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"이상한 상태. 로그인 처리를 무시합니다." )
				<< BUILD_LOG( GetStateID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
				<< END_LOG;
		}
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// PC방 인증 결과에 따른 처리

	KEGS_PC_BANG_AUTH_RESULT_NOT kPacketNot;

	switch( kPacket_.m_byteAuthorizeResult )
	{
	case 0: // forbidden, 미가입 PC방
		{
			START_LOG( clog, L"PC방 인증 서버 응답 forbidden" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetIPStr() )
				<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			// 비가맹 PC방이라는 메시지를 클라이언트로 전송
			kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_FORBIDDEN;
			SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );

			// 게임 접속 차단
			SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Fail );
			ReserveDestroy();
		}
		return;

	case 1: // allowed, 가입 PC방
		{
			// PC방 프리미엄 혜택 적용
			m_kUserPcBangManager.SetIsPcBang( true );

			//{{ 2007. 11. 13  최육사  유저 통계 PC방
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_IsPcBang,	1 );
			//}}

			// - 프리미엄 적용과는 관련없는 데이터
			//if( kPacket_.m_byteIsCharged != 1 )
			//{
			//	START_LOG( cwarn, L"인증 결과가 allowed이지만 IsCharged값은 PC방이 아니라고 옴." )
			//		<< BUILD_LOG( GetIPStr() )
			//		<< BUILD_LOG( GetUID() )
			//		<< BUILD_LOG( GetName() )
			//		<< END_LOG;
			//}

			// 가맹 PC방 혜택 알림 메시지 전송
			switch( kPacket_.m_byteOption )
			{
			case 11: // PC방 계약으로 접속하였습니다.
				{
					kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_ALLOWED_1;
					kPacketNot.m_uiData = kPacket_.m_ulArgument;
					SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );
				}
				break;

			case 17: // PC방 정량제 접속입니다.
			case 29: // 바람 PC방 정량제 접속입니다.
				{
					kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_ALLOWED_2;
					SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );

					// 클라이언트에 출력할 메시지 저장
					m_kUserPcBangManager.SetPcBangMessage( kPacket_.m_ulArgument );
				}
				break;

			default:
				{
					START_LOG( cerr, L"정의되지 않은 Option값입니다." )
						<< BUILD_LOGc( kPacket_.m_byteOption )
						<< END_LOG;
				}
				break;
			}
		}
		break;

	case 2: // Trial 일반 가정집, 프리미엄 혜택 없음
		{
			m_kUserPcBangManager.SetCanTry( true );

			// AuthorizeType 확인해서 메시지 띄울거 체크하자!
			switch( kPacket_.m_byteAuthorizeType )
			{
			case 'M': // 정량PC방
			case 'F': // 정액PC방
			case 'B': // 바람정량PC방
				{
					// 프리미엄 서비스 정상화 메시지 전송
					KEGS_PC_BANG_AUTH_RESULT_NOT kPacketNot;
					kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_RECONNECT;
					kPacketNot.m_uiData = 0;
					SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );
				}
				break;
			}
		}
		break;

	case 3: // Terminate - 게임 실행을 차단하고 인증서버로 로그아웃 패킷을 전송한다.
		{
			START_LOG( clog, L"PC방 인증 서버 응답 Terminate" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetIPStr() )
				<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			// 게임 접속 차단
			SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Fail );
			ReserveDestroy();
		}
		break;

	default:
		START_LOG( cerr, L"결과 값 이상." )
			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
			<< END_LOG;
		break;
	}

	// PC방 인증 결과 정보 저장
	m_kUserPcBangManager.InitPcBangAuth( kPacket_.m_byteAuthorizeResult, kPacket_.m_byteAuthorizeType, kPacket_.m_byteChargeType );

	// 채널이동의 경우 KGSFSM::S_FIELD_MAP상태다
	if( GetStateID() != KGSFSM::S_SERVER_SELECT  &&
		GetStateID() != KGSFSM::S_FIELD_MAP  &&  
		GetStateID() != KGSFSM::S_ROOM )
	{
		StateTransition( KGSFSM::I_TO_LOGINED );
	}

	// PC방 인증 결과 통보
	KENX_USER_LOGIN_NOT kNot;
	kNot.m_bIsGameBang = IsPcBang();
	SendPacket( ENX_USER_LOGIN_NOT, kNot );

	// 로그인 성공 처리
	m_bLoginFail = false;

	//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	if( IsRestrictionUser( kPacket_.m_mapProperty, KENX_AUTH_LOGIN_ACK::NPN_SELECTIVE_SHUTDOWN ) == true )
	{
		START_LOG( clog, L"게임 이용 제한 정책에 해당하는 유저" )
			<< END_LOG;
	}
#else
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
	// 셧다운제
	if( CheckShutdownUser( kPacket_.m_mapProperty, KENX_AUTH_LOGIN_ACK::NPN_SHUTDOWN ) == true )
	{
		START_LOG( clog, L"셧다운제 적용 유저" )
			<< END_LOG;
	}

	//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
	// 안심 로그인
	int iPeaceOfMindLogin = 0;
	if( CheckPeaceOfMindLoginUser( kPacket_.m_mapProperty, KENX_AUTH_LOGIN_ACK::NPN_PEACE_OF_MIND_LOGIN, iPeaceOfMindLogin ) == true )
	{
		START_LOG( clog, L"안심 로그인 적용 유저 - 접속 종료" )
			<< END_LOG;

		if( iPeaceOfMindLogin > 0 ) // 이탈 가능성인 가진 유저이다.
		{
			m_kUserInfoByNexon.SetPeaceOfMindLogin( true );
		}
	}
#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
	//}}
#endif SERV_SHUTDOWN_SYSTEM
	//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}
	// last login
	SendToAccountDB( DBE_UPDATE_LOGIN_TIME_NOT );

	// 모든 로그인 절차가 끝나고 GSUser의 State가 S_LOGINED로 바뀐후에 장바구니를 받아오자!
	KDBE_GET_WISH_LIST_REQ kPacketToDB;
	kPacketToDB.m_iUserUID = GetUID();
	SendToGameDB( DBE_GET_WISH_LIST_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( ELG_REGISTER_NEXON_CHANNEL_CHANGE_USER_ACK, KENX_AUTH_LOGIN_ACK )
{
	//////////////////////////////////////////////////////////////////////////
	// 상태 검사
	switch( GetStateID() )
	{
	case KGSFSM::S_UID_UPDATED:
	case KGSFSM::S_LOGINED:
	case KGSFSM::S_SERVER_SELECT:
	case KGSFSM::S_FIELD_MAP:
	case KGSFSM::S_ROOM:
		// 정상적으로 채널이동 중 PC방 인증 완료
		break;

	default:
		{
			START_LOG( cerr, L"이상한 상태. 로그인 처리를 무시합니다." )
				<< BUILD_LOG( GetStateID() )
				<< BUILD_LOG( GetStateIDString() )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
				<< END_LOG;
		}
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// PC방 인증 결과에 따른 처리

	KEGS_PC_BANG_AUTH_RESULT_NOT kPacketNot;

	switch( kPacket_.m_byteAuthorizeResult )
	{
	case 0: // forbidden, 미가입 PC방
		{
			START_LOG( clog, L"PC방 인증 서버 응답 forbidden" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetIPStr() )
				<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			// 비가맹 PC방이라는 메시지를 클라이언트로 전송
			kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_FORBIDDEN;
			SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );

			// 게임 접속 차단
			SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Fail );
			ReserveDestroy();
		}
		return;

	case 1: // allowed, 가입 PC방
		{
			//{{ 2007. 11. 13  최육사  유저 통계 PC방
			m_kUserStatistics.IncreaseCount( KUserStatistics::USTable_EtcData, 0, KUserStatistics::US_Etc_IsPcBang,	1 );
			//}}

			// - 프리미엄 적용과는 관련없는 데이터
			//if( kPacket_.m_byteIsCharged != 1 )
			//{
			//	START_LOG( cerr, L"인증 결과가 allowed이지만 IsCharged값은 PC방이 아니라고 옴." )
			//		<< BUILD_LOG( GetIPStr() )
			//		<< BUILD_LOG( GetUID() )
			//		<< BUILD_LOG( GetName() )
			//		<< END_LOG;
			//}

			// 채널 이동 전후 PC방 프리미엄 정보 비교
			if( IsPcBang() == false )
			{
				START_LOG( cerr, L"채널이동하기 전에는 PC방이 아니었는데 채널 이동 후 PC방이다?" )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetIPStr() )
					<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetUID() )
					<< END_LOG;
			}
		}
		break;

	case 2: // Trial 일반 가정집, 프리미엄 혜택 없음
		{
			m_kUserPcBangManager.SetCanTry( true );

			// 채널 이동 전후 PC방 프리미엄 정보 비교
			if( IsPcBang() == true )
			{
				START_LOG( cerr, L"채널이동하기 전에는 PC방이었는데 채널 이동 후 PC방이 아니다?" )
#ifndef SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetIPStr() )
					<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
					<< BUILD_LOG( GetUID() )
					<< END_LOG;
			}
		}
		break;

	case 3: // Terminate - 게임 실행을 차단하고 인증서버로 로그아웃 패킷을 전송한다.
		{
			START_LOG( clog, L"PC방 인증 서버 응답 Terminate" )
#ifndef SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetIPStr() )
				<< BUILD_LOG( GetName() )
#endif SERV_PRIVACY_AGREEMENT
				<< BUILD_LOG( GetUID() )
				<< END_LOG;

			// 게임 접속 차단
			SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Fail );
			ReserveDestroy();
		}
		break;

	default:
		START_LOG( cerr, L"결과 값 이상." )
			<< BUILD_LOG( kPacket_.m_byteAuthorizeResult )
			<< END_LOG;
		break;
	}

	//{{ 2010. 01. 29  최육사	PC방 상용화
	m_kUserPcBangManager.InitPcBangAuth( kPacket_.m_byteAuthorizeResult, kPacket_.m_byteAuthorizeType, kPacket_.m_byteChargeType );
	//}}

	// PC방 인증 결과 통보
	KENX_USER_LOGIN_NOT kNot;
	kNot.m_bIsGameBang = IsPcBang();
	SendPacket( ENX_USER_LOGIN_NOT, kNot );

	// 로그인 성공 처리
	m_bLoginFail = false;

	//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	if( IsRestrictionUser( kPacket_.m_mapProperty, KENX_AUTH_LOGIN_ACK::NPN_SELECTIVE_SHUTDOWN ) == true )
	{
		START_LOG( clog, L"게임 이용 차단 적용 유저" )
			<< END_LOG;
	}
#else
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
	if( CheckShutdownUser( kPacket_.m_mapProperty, KENX_AUTH_LOGIN_ACK::NPN_SHUTDOWN ) == true )
	{
		START_LOG( clog, L"셧다운제 적용 유저" )
			<< END_LOG;
		// 메시지 전달( 팝업창 )
		// 		KEGS_SHUT_DOWN_MESSAGE_NOT kNot;
		// 		kNot.m_iNetErrorID = NetError::ERR_SHUT_DOWN_00;
		// 
		// 		SendPacket( EGS_SHUT_DOWN_MESSAGE_NOT, kNot );

		// 게임 접속 차단
		// 		SetDisconnectReason( KStatistics::eSIColDR_Policy_Shutdown_Disconnect );
		// 		ReserveDestroy();
		// 		return;
	}
#endif SERV_SHUTDOWN_SYSTEM
	//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}
	// last login
	SendToAccountDB( DBE_UPDATE_LOGIN_TIME_NOT );

	// 모든 로그인 절차가 끝나고 GSUser의 State가 S_LOGINED로 바뀐후에 장바구니를 받아오자!
	KDBE_GET_WISH_LIST_REQ kPacketToDB;
	kPacketToDB.m_iUserUID = GetUID();
	SendToGameDB( DBE_GET_WISH_LIST_REQ, kPacketToDB );
}

IMPL_ON_FUNC( ENX_AUTH_TERMINATE_NOT )
{
	// 정량제를 이용하다 시간이 만료된 경우이므로, 적절한 메시지를 보여주고 접속을 끊는다, 인증서버는 로그아웃을 전송해야한다.
	KEGS_PC_BANG_AUTH_RESULT_NOT kPacketNot;

	//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
	if( IsRestrictionUser( kPacket_.m_mapProperty, KENX_AUTH_LOGIN_ACK::NPN_SELECTIVE_SHUTDOWN ) == true )
	{
		START_LOG( clog, L"게임 이용 차단 적용 유저" )
			<< END_LOG;

		switch( m_kUserInfoByNexon.GetRestrictionUserMSG() )
		{
		case KUserInfoByNexon::SHUTDOWN_USER_ERROR: // 3
		case KUserInfoByNexon::SELECTIVE_SHUTDOWN_USER_ERROR: // 4
			{
				kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_SELECTIVE_SHUTDOWN_SYSTEM;
			}break;
		case KUserInfoByNexon::UNKNOWN_ERROR: // 99
		default:
			{
				kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_PEACE_OF_MIND_LOGIN;
			}
		}
		
		SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );

		// 접속 종료 처리
		SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Server );
		ReserveDestroy();
		return;
	}
#else
	//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
	// 안심 로그인
	int iPeaceOfMindLogin = 0;
	if( CheckPeaceOfMindLoginUser( kPacket_.m_mapProperty, KENX_AUTH_LOGIN_ACK::NPN_PEACE_OF_MIND_LOGIN, iPeaceOfMindLogin ) == true )
	{
		START_LOG( clog, L"안심 로그인 적용 유저 - 접속 종료" )
			<< END_LOG;

		if( iPeaceOfMindLogin > 0 ) // 이탈 가능성인 가진 유저이다.
		{
			m_kUserInfoByNexon.SetPeaceOfMindLogin( true );
		}

		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_PEACE_OF_MIND_LOGIN;

		SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );

		// 접속 종료 처리
		SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Server );
		ReserveDestroy();
		return;
	}
#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
	//}}
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
	//}}

	switch( kPacket_.m_byteOption )
	{
	case 3: // 정액 만료
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_1;
		break;

	case 19: // 정량 만료
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_2;
		break;

	case 2: // IP당 접속 수를 초과
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_3;
		break;

	case 27: // IP당 접속 수를 초과
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_3;
		return; // 종료 처리 안함.

	default:
		{
			START_LOG( cerr, L"정의 되지 않은 option값입니다." )
				<< BUILD_LOG( kPacket_.m_byteOption )
				<< BUILD_LOGc( kPacketNot.m_cType )
				<< END_LOG;
		}
	}

	// 클라이언트에게 전달
	SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );

	// 접속 종료 처리
	SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Server );
	ReserveDestroy();
}

IMPL_ON_FUNC( ENX_AUTH_MESSAGE_NOT )
{
	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 

	// 클라이언트에 출력할 메시지 전송
	KEGS_PC_BANG_AUTH_RESULT_NOT kPacketNot;
	kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_MESSAGE;
	kPacketNot.m_uiData = kPacket_.m_ulArgument;
	SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_PCBANG_AUTH_NEW
//////////////////////////////////////////////////////////////////////////
//}}

_IMPL_ON_FUNC( E_KICK_USER_NOT, int )
{
	START_LOG_WITH_NAME( cout )
		<< BUILD_LOG( GetStateIDString() )
		<< BUILD_LOG( NetError::GetErrStr( kPacket_ ) )
		<< BUILD_LOG( kPacket_ );

	Kick( kPacket_ );
}

//{{ 2010. 01. 29  최육사	PC방 상용화
_IMPL_ON_FUNC( ENX_USER_TERMINATE_NOT, KENX_COMMON_ACK_NOT )
{
	// 정량제를 이용하다 시간이 만료된 경우이므로, 적절한 메시지를 보여주고 접속을 끊는다, 인증서버는 로그아웃을 전송해야한다.
	KEGS_PC_BANG_AUTH_RESULT_NOT kPacketNot;

	switch( kPacket_.m_byteOption )
	{
	case 3: // 정액 만료
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_1;
		break;

	case 19: // 정량 만료
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_2;
		break;

	case 2: // IP당 접속 수를 초과
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_3;
		break;

	case 27: // IP당 접속 수를 초과
		kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_TERMINATE_3;
		return; // 종료 처리 안함.

	default:
		{
			START_LOG( cerr, L"정의 되지 않은 option값입니다." )
				<< BUILD_LOG( kPacket_.m_byteOption )
				<< END_LOG;
		}
	}

	// 클라이언트에게 전달
	SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );

	// 접속 종료 처리
	SetDisconnectReason( KStatistics::eSIColDR_PCBang_Auth_Server );
	ReserveDestroy();
}

_IMPL_ON_FUNC( ENX_MESSAGE_NOT, KENX_COMMON_ACK_NOT )
{
	// 클라이언트에 출력할 메시지 전송
	KEGS_PC_BANG_AUTH_RESULT_NOT kPacketNot;
	kPacketNot.m_cType = KEGS_PC_BANG_AUTH_RESULT_NOT::PBRT_MESSAGE;
	kPacketNot.m_uiData = kPacket_.m_uiArgument;
	SendPacket( EGS_PC_BANG_AUTH_RESULT_NOT, kPacketNot );
}
//}}

IMPL_ON_FUNC( EGS_NEW_USER_JOIN_REQ )
{
	//{{ 2010. 10. 07	최육사	중복 패킷 필터 추가
#ifdef SERV_ADD_REPEAT_FILTER
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_INIT ), EGS_NEW_USER_JOIN_REQ, EGS_NEW_USER_JOIN_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_INIT ) );
#endif SERV_ADD_REPEAT_FILTER
	//}}

	// USER ID 길이 검사.
	std::string	multiByteString = KncUtil::toNarrowString( kPacket_.m_wstrID );
	int nSize = ( int )multiByteString.size();

	if( nSize < 4 || nSize > 20 )
	{
		KEGS_NEW_USER_JOIN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NEW_ACCOUNT_03;

		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( nSize )
			<< BUILD_LOG( kPacket_.m_wstrID )
			<< END_LOG;

		SendPacket( EGS_NEW_USER_JOIN_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrID ) )
	{
		KEGS_NEW_USER_JOIN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_NEW_USER_JOIN_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	// USER PASSWORD 길이 검사.
	multiByteString = KncUtil::toNarrowString( kPacket_.m_wstrPassword );
	nSize = ( int )multiByteString.size();

	if( nSize < 4 || nSize > 20 )
	{
		KEGS_NEW_USER_JOIN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NEW_ACCOUNT_04;

		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( nSize )
			<< BUILD_LOG( kPacket_.m_wstrPassword )
			<< END_LOG;

		SendPacket( EGS_NEW_USER_JOIN_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrPassword ) )
	{
		KEGS_NEW_USER_JOIN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_NEW_USER_JOIN_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	// USER NAME 길이 검사.
	multiByteString = KncUtil::toNarrowString( kPacket_.m_wstrName );
	nSize = ( int )multiByteString.size();

#ifdef SERV_INT_ONLY
	if( nSize < 1 || nSize > 25 )
#else //SERV_INT_ONLY
	if( nSize < 1 || nSize > 16 )
#endif //SERV_INT_ONLY
	{
		KEGS_NEW_USER_JOIN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_NEW_ACCOUNT_06;

		START_LOG_WITH_NAME( cwarn )
			<< BUILD_LOG( nSize )
			<< BUILD_LOG( kPacket_.m_wstrName )
			<< END_LOG;

		SendPacket( EGS_NEW_USER_JOIN_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	if( KODBC::IsInvalidMarkInForLetter( kPacket_.m_wstrName ) )
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrName ) )
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	{
		KEGS_NEW_USER_JOIN_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ODBC_00;
		SendPacket( EGS_NEW_USER_JOIN_ACK, kPacket );
		ReserveDestroy();
		return;
	}

	kPacket_.m_byteGuestUser = m_kNexonAccountInfo.m_byteGuestUser;

	SendToLoginAccountDB( DBE_NEW_USER_JOIN_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_NEW_USER_JOIN_ACK, KEGS_NEW_USER_JOIN_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_INIT ) );

	SendPacket( EGS_NEW_USER_JOIN_ACK, kPacket_ );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG_WITH_NAME( cerr )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;
	}

	SetDisconnectReason( KStatistics::eSIColDR_Internal_Account );
	ReserveDestroy();
}

IMPL_ON_FUNC_NOPARAM( EGS_STATE_CHANGE_SERVER_SELECT_REQ )
{
	VERIFY_STATE( ( 3, KGSFSM::S_LOGINED, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	if( GetStateID() == KGSFSM::S_LOGINED )
	{
		KEGS_STATE_CHANGE_SERVER_SELECT_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;

		SendPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK, kPacket );
	}
	//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	else if( GetStateID() == KGSFSM::S_FIELD_MAP  ||  GetStateID() == KGSFSM::S_ROOM )
#else
	else if( GetStateID() == KGSFSM::S_FIELD_MAP )
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
	{
		//{{ 2012. 02. 07	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		if( GetStateID() == KGSFSM::S_ROOM )
		{
			StateTransition( KGSFSM::I_TO_FIELD_MAP );
		}
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//해킹유저인데 동의를 하지 않은상태이면 점속을 끝는다.
		if( m_cHackingUserType == SEnum::HUT_DISAGREE_HACK_USER
#ifdef SERV_DESAGREE_HACK_USER_ON_OFF
			&& SiKGameSysVal()->IsOnDisagreeHackUserFunc() == true 
#endif //SERV_DESAGREE_HACK_USER_ON_OFF
			)
		{
			KEGS_STATE_CHANGE_SERVER_SELECT_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_HACK_USER_02;

			START_LOG( cwarn, L"해킹신고유저인데 동의를 하지않아 종료시킴." )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( m_kNexonAccountInfo.m_wstrID )
				<< END_LOG;

			SendPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK, kPacket );
			//{{ 2013. 2. 14	박세훈	해킹 등록 유저인데 즉시 킥하지 않는 경우 종료 사유 추가
#ifdef SERV_HACK_USER_DISCONNECT_REASON_ADD
			SetDisconnectReason( KStatistics::eSIColDR_Client_Hacking );
#endif SERV_HACK_USER_DISCONNECT_REASON_ADD
			//}}
			ReserveDestroy();
			return;
		}

		// 게임중 캐릭선택으로 오는것을 게임종료처리와 같은 상황으로 본다.
		// 유닛이 선택되어 있다면 해당 유닛정보를 DB업데이트, 로그인서버에서 지우기, 나머지 처리를 한다.
		if( GetCharUID() > 0 )
		{
			//{{ 2010. 8. 4	최육사	코드 리팩토링
			//#ifdef SERV_DESELECT_UNIT_REFAC
			//{{ 2012. 11. 22	최육사		캐릭터 선택 해제 코드 리팩토링
#ifdef SERV_DESELECT_UNIT_CODE_REFACTORING
			OnDeselectUnit( EGS_STATE_CHANGE_SERVER_SELECT_REQ, false );
#else
			OnDeselectUnit( true, EGS_STATE_CHANGE_SERVER_SELECT_REQ );
#endif SERV_DESELECT_UNIT_CODE_REFACTORING
			//}}			
			//#endif SERV_DESELECT_UNIT_REFAC
			//}}
			return;
		}		

		// 유닛 정보 초기화
		ResetUnitInfo();

		KEGS_STATE_CHANGE_SERVER_SELECT_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SendPacket( EGS_STATE_CHANGE_SERVER_SELECT_ACK, kPacket );
	}

	StateTransition( KGSFSM::I_TO_SERVER_SELECT );
}

//////////////////////////////////////////////////////////////////////////
// 안쓰는 패킷 될듯!
IMPL_ON_FUNC_NOPARAM( EGS_GET_SERVER_SET_DATA_REQ )
{
	VERIFY_STATE( ( 2, KGSFSM::S_LOGINED, KGSFSM::S_FIELD_MAP ) );

	KEGS_GET_SERVER_SET_DATA_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	std::map< int, std::vector< KServerInfo > > mapServerList;
	KBaseServer::GetKObj()->GetServerList( mapServerList );

	std::map< int, std::vector< KServerInfo > >::iterator mit;
	for( mit = mapServerList.begin(); mit != mapServerList.end(); mit++ )
	{
		int iMaxCCU = 0;
		int iCCU = 0;
		std::vector< KServerInfo >::iterator vit;
		for( vit = mit->second.begin(); vit != mit->second.end(); vit++ )
		{
			if( vit->m_iServerClass == KBaseServer::ESC_GAME )
			{
				iMaxCCU += vit->m_nMaxUser;
				iCCU += vit->m_nCurrentUser;
			}
		}

		KServerSetData kServerSetData;
		kServerSetData.serverSetID = mit->first;
		kServerSetData.serverSetName = L"솔레스";
		if( iMaxCCU <= 0 )
		{
			START_LOG( cwarn, L"최대 동접 수치가 이상함." )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( iMaxCCU )
				<< END_LOG;

			iMaxCCU = 1;
		}

		if( iCCU < 1500 )
		{
			kServerSetData.userCountLevel = KServerSetData::UCL_FREE;
		}
		else if( iCCU < 3000 )
		{
			kServerSetData.userCountLevel = KServerSetData::UCL_NORMAL;
		}
		else if( iCCU < iMaxCCU )
		{
			kServerSetData.userCountLevel = KServerSetData::UCL_BUSY;
		}
		else
		{
			kServerSetData.userCountLevel = KServerSetData::UCL_FULL;
		}		

		//int iPercent = ( int )( ( float )iCCU * 100.f / ( float )iMaxCCU );
		//if( iPercent < 30 )
		//{
		//    kServerSetData.userCountLevel = KServerSetData::UCL_FREE;
		//}
		//else if( iPercent < 70 )
		//{
		//    kServerSetData.userCountLevel = KServerSetData::UCL_NORMAL;
		//}
		//else if( iPercent < 100 )
		//{
		//    kServerSetData.userCountLevel = KServerSetData::UCL_BUSY;
		//}
		//else
		//{
		//    kServerSetData.userCountLevel = KServerSetData::UCL_FULL;
		//}

		kPacket.m_ServerSetList.push_back( kServerSetData );
	}

	SendPacket( EGS_GET_SERVER_SET_DATA_ACK, kPacket );
}
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( EGS_SELECT_SERVER_SET_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	// 현재는 서버군이 하나다.
	KEGS_SELECT_SERVER_SET_ACK kPacket;

	//블록된 유저면 처리를 해준다.
	switch( GetAuthLevel() )
	{
	case SEnum::UAL_BLOCKED_2:
	case SEnum::UAL_BLOCKED_3:
	case SEnum::UAL_BLOCKED_4:
	case SEnum::UAL_BLOCKED_5:
	case SEnum::UAL_BLOCKED_6:
		//{{ 계정 제재 일수 상태 추가 - 김민성
#ifdef SERV_ADD_BLOCK_DAY
	case SEnum::UAL_BLOCKED_7:
	case SEnum::UAL_BLOCKED_8:
	case SEnum::UAL_BLOCKED_9:
#endif SERV_ADD_BLOCK_DAY
		//}}
		{
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			kPacket.m_iOK = NetError::ERR_ACCOUNT_BLOCK_01;
#else //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			kPacket.m_iOK = NetError::ERR_VERIFY_11;
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
			kPacket.m_kAccountBlockInfo.m_wstrEndTime = m_kAccountBlockInfo.m_wstrEndTime;
			kPacket.m_kAccountBlockInfo.m_wstrBlockReason = m_kAccountBlockInfo.m_wstrBlockReason;

			SendPacket( EGS_SELECT_SERVER_SET_ACK, kPacket );

			SetDisconnectReason( KStatistics::eSIColDR_Account_Block );
			ReserveDestroy();
			return;
		}
		break;
	}

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
	kPacket.m_kAccountBlockInfo.m_byteBlockType	= GetAccountBlockType();
	if( 0 < kPacket.m_kAccountBlockInfo.m_byteBlockType )
	{
		kPacket.m_iOK = NetError::ERR_VERIFY_11;

		KAbuserLogManager::KHackingUserCheckInfo kInfo;
		if( SiKAbuserLogManager()->GetHackingUserCheckInfo( kPacket.m_kAccountBlockInfo.m_byteBlockType, kInfo ) == true )
		{
			switch( kInfo.m_byteAccountBlockNotifyType )
			{
			case KAbuserLogManager::HUCBNT_NONE:
				break;

			case KAbuserLogManager::HUCBNT_RESON:
				GetAccountBlockReson( kPacket.m_kAccountBlockInfo.m_wstrBlockReason2 );
				GetAccountBlockEndDate( kPacket.m_kAccountBlockInfo.m_wstrBlockEndDate );
				SendPacket( EGS_SELECT_SERVER_SET_ACK, kPacket );
				break;

			case KAbuserLogManager::HUCBNT_NOTIFY:
				kPacket.m_kAccountBlockInfo.m_wstrBlockReason2 = kInfo.m_wstrAccountBlockNotify;
				GetAccountBlockEndDate( kPacket.m_kAccountBlockInfo.m_wstrBlockEndDate );
				SendPacket( EGS_SELECT_SERVER_SET_ACK, kPacket );
				break;

			case KAbuserLogManager::HUCBNT_DEFAULT:
				SiKAbuserLogManager()->GetHackingUserCheckDefaultNotify( kPacket.m_kAccountBlockInfo.m_wstrBlockReason2 );
				GetAccountBlockEndDate( kPacket.m_kAccountBlockInfo.m_wstrBlockEndDate );
				SendPacket( EGS_SELECT_SERVER_SET_ACK, kPacket );
				break;
			}
		}

		SetDisconnectReason( KStatistics::eSIColDR_Account_Block );
		ReserveDestroy();
		return;
	}
#endif // SERV_HACKING_USER_CHECK_COUNT

	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_wstrIP = NetCommon::GetLocalIPW();
	kPacket.m_usPort = KNetLayer::GetKObj()->m_usPortMaster;
	SendPacket( EGS_SELECT_SERVER_SET_ACK, kPacket );
}

//{{ 2009. 5. 26  최육사	채널이동
IMPL_ON_FUNC( EGS_CHECK_CHANNEL_CHANGE_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_CHECK_CHANNEL_CHANGE_REQ, EGS_CHECK_CHANNEL_CHANGE_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	// 필드상태가 아니면 이동할수 없다
	if( GetFieldUID() <= 0  ||  GetRoomUID() > 0  ||  GetRoomListID() > 0 )
	{
		START_LOG( cwarn, L"채널 이동 할수 있는 위치가 아닙니다." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetFieldUID() )
			<< BUILD_LOG( GetRoomUID() )
			<< BUILD_LOG( GetRoomListID() )
			<< END_LOG;

		KEGS_CHECK_CHANNEL_CHANGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_09;
		SendPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}

	// 채널이동 쿨타임
	if( m_kTimer[TM_UNIT_DURATION].elapsed() < 5.0 )
	{
		START_LOG( cwarn, L"채널이동 쿨타임." )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		KEGS_CHECK_CHANNEL_CHANGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_10;
		SendPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}

	UidType iGSUID = 0;

	// 같은 채널을 이동하려고 한다면 실패처리!
	if( SiKChannelManager()->GetChannelID() == kPacket_.m_iChannelID )
	{
		START_LOG( cwarn, L"이동 하려는 채널과 현재 채널이 같음." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( SiKChannelManager()->GetChannelID() )
			<< BUILD_LOG( kPacket_.m_iChannelID )
			<< END_LOG;

		KEGS_CHECK_CHANNEL_CHANGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_07;
		SendPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}

	// 이동할수 있는 채널인지 검사	
	if( SiKChannelManager()->GetServerUIDByChannelID( kPacket_.m_iChannelID, iGSUID ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 ChannelID입니다" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iChannelID )
			<< END_LOG;

		KEGS_CHECK_CHANNEL_CHANGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_01;
		SendPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}

	//{{ 2011. 07. 27	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	if( IsRequestMatchMaking() == true )
	{
		KEGS_CHECK_CHANNEL_CHANGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_20;
		SendPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}

	if( GetMatchUID() != 0 )
	{
		KEGS_CHECK_CHANNEL_CHANGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_MATCH_MAKING_20;
		SendPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, kPacket );
		return;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	KECN_CHECK_CHANNEL_CHANGE_REQ kPacketToCn;
	kPacketToCn.m_iSourGSUID = KBaseServer::GetKObj()->GetUID();
	kPacketToCn.m_iDestGSUID = iGSUID;
	kPacketToCn.m_iUserUID = GetUID();
	kPacketToCn.m_iCharUID = GetCharUID();
	//{{ 2009. 11. 10  최육사	채널이동
	GetChannelChangeInfo( kPacketToCn.m_kChangeUserInfo );
	//}}
	SendToCnServer( ECN_CHECK_CHANNEL_CHANGE_REQ, kPacketToCn );
}

IMPL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING
	KEGS_CHECK_CHANNEL_CHANGE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_CHECK_CHANNEL_CHANGE_ACK, kPacket );
}

IMPL_ON_FUNC( EGS_CONNECT_CHANNEL_CHANGE_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_INIT ), EGS_CONNECT_CHANNEL_CHANGE_REQ, EGS_CONNECT_CHANNEL_CHANGE_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_INIT ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_kConnectReq.m_wstrVersion );

	//////////////////////////////////////////////////////////////////////////
	// 버전 비교
	KEGS_CONNECT_CHANNEL_CHANGE_ACK kPacketAck;
	KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ kPacket;	

	// 채널 이동 예약 검사
	if( SiKChannelManager()->IsExistChannelChangeUser( kPacket_.m_kSelectUnitReq.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"채널 이동 예약이 안되어있음!" )
			<< BUILD_LOG( kPacket_.m_kSelectUnitReq.m_iUnitUID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_CHANNEL_CHANGE_06;
		goto end_proc;
	}

	// 해당 유저가 접속할 수 있도록 예약정보 제거
	if( SiKChannelManager()->DelChannelChangeUser( kPacket_.m_kSelectUnitReq.m_iUnitUID, kPacket.m_kChangeUserInfo ) == false )
	{
		START_LOG( cerr, L"채널 이동 예약이 안되어있음!" )
			<< BUILD_LOG( kPacket_.m_kSelectUnitReq.m_iUnitUID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_CHANNEL_CHANGE_06;
		goto end_proc;
	}
	else
	{
		//{{ 2010. 01. 29  최육사	PC방 상용화
		m_kUserPcBangManager.SetIsPcBang( kPacket.m_kChangeUserInfo.m_bIsPcBang );
		//}}
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType( kPacket.m_kChangeUserInfo.m_iPcBangType );
#endif SERV_PC_BANG_TYPE
	}

	if( OnConnectReq( kPacket_.m_kConnectReq, kPacketAck.m_kConnectAck ) == false )
	{
		START_LOG( cwarn, L"connect error" )
			<< BUILD_LOG( NetError::GetErrStr( kPacketAck.m_kConnectAck.m_iOK ) )
			<< END_LOG;

		kPacketAck.m_iOK = kPacketAck.m_kConnectAck.m_iOK;
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_CHANNELING_USER_MANAGER
	SiKChannelingUserManager()->UpdateChannelingUserList( true, kPacket_.m_kVerifyAccountReq.m_iChannelingCode, GetGameServerLoginUser() );
	SetGameServerLoginUser( true );
#endif SERV_CHANNELING_USER_MANAGER

	// 계정인증
	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_kVerifyAccountReq.m_bDebugAuth )
		<< BUILD_LOG( kPacket_.m_kVerifyAccountReq.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_kVerifyAccountReq.m_wstrPasswd );

	kPacket.m_kConnectAck = kPacketAck.m_kConnectAck;
	kPacket.m_kSelectUnitReq = kPacket_.m_kSelectUnitReq;
	kPacket.m_kUserAuthReq.m_kAuthenticateReq = kPacket_.m_kVerifyAccountReq;
	kPacket.m_kUserAuthReq.m_kNexonAccountInfo.m_wstrLoginIP = KncUtil::toWideString( KSession::GetIPStr() );	

#ifdef SERV_GLOBAL_AUTH
	kPacket_.m_kVerifyAccountReq.m_bDebugAuth = false;// 해외팀에서는 bDebugAuth를 이용한 예외상황 고려안함
	kPacket.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth = false; // 해외팀에서는 bDebugAuth를 이용한 예외상황 고려안함
#endif // SERV_GLOBAL_AUTH

#ifdef SERV_JAPAN_CHANNELING
	kPacket.m_kUserAuthReq.m_kNexonAccountInfo.m_uChannelCode = static_cast<UINT8>(kPacket_.m_kVerifyAccountReq.m_iChannelingCode);
#endif SERV_JAPAN_CHANNELING

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	if( KODBC::IsInvalidMarkIn( kPacket_.m_kVerifyAccountReq.m_wstrUserID ) )
	{
		kPacketAck.m_iOK = NetError::ERR_ODBC_00;
		goto end_proc;
	}

	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_INTERNAL )
	{
		if( KODBC::IsInvalidMarkIn( kPacket_.m_kVerifyAccountReq.m_wstrPasswd ) )
		{
			kPacketAck.m_iOK = NetError::ERR_ODBC_00;
			goto end_proc;
		}
	}

	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	if( KSimLayer::GetKObj()->GetVersionFlag() != KSimLayer::VF_OPEN_TEST )		// 오픈 테섭에서는 검사 제외
	{
		if( SiKGameSysVal()->IsMachineIDDuplicateCheck() == true )
		{
			if( m_kUserSecurityManager.CheckValidMachineID( kPacket_.m_kVerifyAccountReq.m_strMachineID ) == false )
			{
				START_LOG( cout, L"[알림] 머신ID가 유효하지 않다! 클라이언트로 알림 패킷을 쏘자!" )
					<< BUILD_LOG( kPacket_.m_kVerifyAccountReq.m_wstrHWID )
					<< BUILD_LOG( kPacket_.m_kVerifyAccountReq.m_wstrUserID )
					<< BUILD_LOG( kPacket_.m_kVerifyAccountReq.m_wstrPasswd )
					<< BUILD_LOG( kPacket_.m_kVerifyAccountReq.m_bDebugAuth );

				//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
				kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#else
				KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
				kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_ID;
				SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
				//}}

				//{{ 2011. 09. 06	최육사	머신 ID 기능 업데이트
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
				// 머신ID가 비어있으면 접속 차단!
				Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
#endif SERV_MACHINE_ID_BLOCK_UPDATE
				//}}
			}
		}
	}

	// 머신ID 저장!
	m_kUserSecurityManager.SetMachineID( kPacket_.m_kVerifyAccountReq.m_strMachineID );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.20 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	 m_bTimeControlItemCheckDungeonPlay = false;
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

	SendToLoginServer( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ, kPacket );

	m_kTimer[TM_INIT].restart();
	return;

end_proc:
	SendPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK, kPacketAck );

	// 채널 이동실패로 인한 종료처리
	SetDisconnectReason( KStatistics::eSIColDR_Channel_Change_Fail );
	ReserveDestroy();
}

IMPL_ON_FUNC( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_INIT ) );

	KEGS_CONNECT_CHANNEL_CHANGE_ACK kPacketAck;
	KELG_CHANNEL_CHANGE_REGISTER_USER_REQ kPacketReq;

	if( OnNexonUserAuthenticateAck( kPacket_.m_kUserAuthAck, kPacketAck.m_kVerifyAccountAck ) == false )
	{
		START_LOG( cerr, L"넥슨 인증 실패!" )
			<< BUILD_LOG( NetError::GetErrStr( kPacketAck.m_kVerifyAccountAck.m_iOK ) )
			<< END_LOG;

		kPacketAck.m_iOK = kPacketAck.m_kVerifyAccountAck.m_iOK;
		goto end_proc;
	}

#ifdef SERV_EPAY_SYSTEM
	// 계정 타입
	m_GiantAccountType.m_uiType = kPacket_.m_uiGiantAccountType;	// SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

	m_wstrUserID = kPacket_.m_kUserAuthAck.m_kAccountInfo.m_wstrID;
	m_usGiantZone = kPacket_.m_usZone;
	m_usGiantGame = kPacket_.m_usGame;
	START_LOG( clog, L"epay관련 로그 & 계정 타입 설정" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_GiantAccountType.m_uiType )
		<< BUILD_LOG( m_wstrUserID )
		<< BUILD_LOG( m_usGiantZone )
		<< BUILD_LOG( m_usGiantGame )
		<< END_LOG;
#endif //SERV_EPAY_SYSTEM

	//////////////////////////////////////////////////////////////////////////
	// 로그인 서버에 등록하기	
	kPacketReq.m_kConnectAck = kPacket_.m_kConnectAck;
	kPacketReq.m_kVerifyAccountAck = kPacketAck.m_kVerifyAccountAck;
	kPacketReq.m_kSelectUnitReq = kPacket_.m_kSelectUnitReq;
	kPacketReq.m_kChangeUserInfo = kPacket_.m_kChangeUserInfo;

	// Verify Account
	kPacketReq.m_kVerifyAccountAck.m_kAccountInfo = kPacket_.m_kUserAuthAck.m_kAccountInfo;
	kPacketReq.m_kVerifyAccountAck.m_kAccountInfo.m_bIsGuestUser = IsGuestUser(); // 체험아이디 정보

	//{{ 2010. 01. 11	김민성	IP추가
#ifdef SERV_CLIENT_PUBLIC_IP
	kPacketReq.m_kVerifyAccountAck.m_wstrClientIP = KncUtil::toWideString( GetIPStr() );
#endif SERV_CLIENT_PUBLIC_IP
	//}}

#ifdef SERV_JAPAN_CHANNELING
	kPacketReq.m_kRegUserReq.m_ucChannelCode = kPacket_.m_kUserAuthAck.m_kNexonAccountInfo.m_uChannelCode;
#endif // SERV_JAPAN_CHANNELING

	// Register User
	kPacketReq.m_kRegUserReq.m_wstrUserID = GetName();	
	kPacketReq.m_kRegUserReq.m_iChannelID = SiKChannelManager()->GetChannelID();
	kPacketReq.m_kRegUserReq.m_uiIP		= GetIP();
	kPacketReq.m_kRegUserReq.m_byteCanTry = 0;
	kPacketReq.m_kRegUserReq.m_byteGuestUser = m_kNexonAccountInfo.m_byteGuestUser;	
	kPacketReq.m_kRegUserReq.m_cAuthLevel = GetAuthLevel();
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	kPacketReq.m_kRegUserReq.m_strMachineID = m_kUserSecurityManager.GetStringMachineID();
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	kPacketReq.m_kRegUserReq.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID

#ifdef SERV_COUNTRY_TH
	kPacketReq.m_kRegUserReq.m_wstrSocketID = kPacket_.m_wstrSocketID;
	START_LOG( clog, L"UserSession인증 요청." )
		<< BUILD_LOG( kPacket_.m_wstrSocketID )
		<< END_LOG;
#endif //SERV_COUNTRY_TH

#ifdef SERV_COUNTRY_PH 
	m_usGarenaCyberCafe = kPacket_.m_usGarenaCyberCafe; // 2013.08.01 lygan_조성욱 // 동남아시아 가레나 플랫폼으로 부터 받은 PC 방 정보
#endif //SERV_COUNTRY_PH

	SendToLoginServer( ELG_CHANNEL_CHANGE_REGISTER_USER_REQ, kPacketReq );

	//////////////////////////////////////////////////////////////////////////
	StateTransition( KGSFSM::I_TO_UID_UPDATED );
	ms_kMaxLoginTime.CheckMaxProcessTime( GetName(), ( float )m_kTimer[TM_INIT].elapsed() );

	//080624.hoons.인증패킷 처리를 다해주어야 클라이언트에서 nProtect가 동작함.(해킹유저만)
	//if( m_cHackingUserType == SEnum::HUT_AGREE_HACK_USER )
	//{
	//	// nProtect 한국인증 모듈 사용설정
	//	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CommonFlag::CF_NPGG ) )
	//	{
	//		m_kGGAuth.SetEnable( true );
	//	}
	//	m_kGGAuth.Init();
	//}

	//korea nProtect 초기 인증을 클라이언트에 보낸다.
	//if( m_kGGAuth.FirstAuthQuery( GetThisPtr<KGSUser>() ) == false )
	//{
	//	SetDisconnectReason( KStatistics::eSIColDR_NProtect_Auth_First );
	//	ReserveDestroy();

	//	START_LOG( cerr, L"nProtect 첫인증 실패.!" )
	//		<< BUILD_LOG( GetUID() )
	//		<< BUILD_LOG( GetName() )
	//		<< END_LOG;
	//	return;
	//}

	//{{ 2009. 8. 18  최육사	hack shield
	if( m_kHSAuth.FirstHackShieldAuth( GetThisPtr<KGSUser>() ) == false )
	{
		SetDisconnectReason( KStatistics::eSIColDR_NProtect_Auth_First );
		ReserveDestroy();

		START_LOG( cerr, L"hack shield 첫인증 실패.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;
		return;
	}
	//}}

	//{{ 2010. 07. 05  최육사	유저 어뷰저 매니저
#ifdef SERV_USER_ABUSER_MANAGER
	m_kUserAbuserManager.InitAccountAbuser( GetThisPtr<KGSUser>() );

#ifdef SERV_MORNITORING_ABUSER_NOTIFY_SMS// 작업날짜: 2013-05-22	// 박세훈
	if( m_kUserAbuserManager.IsMornitoringAbuser() == true )
	{
		KDBE_SEND_PHONE_MSG_NOT kPacketNot;
		kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"모니터링 감시 대상자 접속: UserUID(%d), ServerUID(%d), ServerIP(%s)" ) % GetUID() % KBaseServer::GetKObj()->GetUID() % KBaseServer::GetKObj()->GetPublicIP() );
		SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_MORNITORING_ABUSER_NOTIFY, kPacketNot.m_vecPhoneNum );
		if( kPacketNot.m_vecPhoneNum.empty() == false )
		{
			SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
		}
	}
#endif // SERV_MORNITORING_ABUSER_NOTIFY_SMS

#endif SERV_USER_ABUSER_MANAGER
	//}}

	//{{ 2012. 02. 21	김민성	2차 보안 채널 이동시(미니맵) 오류 수정
#ifdef SERV_SECOND_SECURITY_PW_ERROR_MODIFY
	{
		CTime tLasstAuthDate;
		if( kPacket_.m_bUseSecondPW == true )
		{
			if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastAuthDate, tLasstAuthDate ) == false )
			{
				START_LOG( cerr, L"문자열 시간 변환 실패." )
					<< BUILD_LOG( kPacket_.m_wstrLastAuthDate )
					<< END_LOG;
			}
		}
		SetIsUseSecondPW( kPacket_.m_bUseSecondPW );
		SetSecondPWFailedCount( kPacket_.m_iFailedCount );
		SetSecondPW( kPacket_.m_wstrSecondPW );
		SetLastAuthDate( tLasstAuthDate );
		
		if( kPacket_.m_bUseSecondPW == true )
		{
			SetSuccessSecondSecurity( true );
		}
		else
		{
			SetSuccessSecondSecurity( false );
		}

		KEGS_SECOND_SECURITY_INFO_NOT kNot;
		kNot.m_iOK = kPacketAck.m_kVerifyAccountAck.m_iOK;
		kNot.m_bUseSecondPW = kPacket_.m_bUseSecondPW;
		SendPacket( EGS_SECOND_SECURITY_INFO_NOT, kNot );
	}
#endif SERV_SECOND_SECURITY_PW_ERROR_MODIFY
	//}}

	return;

end_proc:
	SendPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK, kPacketAck );

	// 채널 이동실패로 인한 종료처리
	SetDisconnectReason( KStatistics::eSIColDR_Channel_Change_Fail );
	ReserveDestroy();
}

IMPL_ON_FUNC( ELG_CHANNEL_CHANGE_REGISTER_USER_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_UID_UPDATED ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		KEGS_CONNECT_CHANNEL_CHANGE_ACK kPacketAck;
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK, kPacketAck );

		// 채널 이동실패로 인한 종료처리
		SetDisconnectReason( KStatistics::eSIColDR_Channel_Change_Fail );
		ReserveDestroy();
		return;
	}

	//{{ 2012. 06. 14	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	//////////////////////////////////////////////////////////////////////////
	m_mmapDailyGiftBoxList = kPacket_.m_kChangeUserInfo.m_mmapDailyGiftBoxList;
	//////////////////////////////////////////////////////////////////////////
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	// 캐릭터 선택하러 가기위해 서버 선택 상태까지 바로 이동!
	StateTransition( KGSFSM::I_TO_LOGINED );
	StateTransition( KGSFSM::I_TO_SERVER_SELECT );

	//////////////////////////////////////////////////////////////////////////
	// 캐릭터 선택
	KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ kPacketToDB;
	kPacketToDB.m_kConnectAck = kPacket_.m_kConnectAck;
	kPacketToDB.m_kVerifyAccountAck = kPacket_.m_kVerifyAccountAck;
	kPacketToDB.m_kSelectUnitReq = kPacket_.m_kSelectUnitReq;
	kPacketToDB.m_kChangeUserInfo = kPacket_.m_kChangeUserInfo;

#ifdef SERV_NEW_YEAR_EVENT_2014
	IF_EVENT_ENABLED( CEI_OLD_YEAR_EVENT_2013 )
	{
		kPacketToDB.m_kSelectUnitReq.m_setCodeEventScriptID.insert( CEI_OLD_YEAR_EVENT_2013 );
	}
	IF_EVENT_ENABLED( CEI_NEW_YEAR_EVENT_2014 )
	{
		kPacketToDB.m_kSelectUnitReq.m_setCodeEventScriptID.insert( CEI_NEW_YEAR_EVENT_2014 );
	}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	IF_EVENT_ENABLED( CEI_GATE_OF_DARKNESS_SUPPORT_EVENT )
	{
		kPacketToDB.m_kSelectUnitReq.m_setCodeEventScriptID.insert( CEI_GATE_OF_DARKNESS_SUPPORT_EVENT );
	}
#endif //SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_EVENT_CHECK_POWER
	IF_EVENT_ENABLED( CEI_CHECK_POWER )
	{
		kPacketToDB.m_kSelectUnitReq.m_setCodeEventScriptID.insert( CEI_CHECK_POWER );
	}
#endif SERV_EVENT_CHECK_POWER

	SendToGameDB( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	KEGS_CONNECT_CHANNEL_CHANGE_ACK kPacketAck;

	if( kPacket_.m_kSelectUnitAck.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
		if( kPacket_.m_kSelectUnitAck.m_iOK == NetError::ERR_SELECT_UNIT_07 )
		{
			START_LOG( cout, L"UserUID소속이 아닌 UnitUID로 캐릭터를 선택한 유저가 나타났다!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetUserID() )
				<< BUILD_LOG( GetCharUID() )
				<< BUILD_LOG( GetCharName() );

			KDBE_SEND_PHONE_MSG_NOT kPacketNot;
			kPacketNot.m_wstrSMSMessage = boost::str( boost::wformat( L"계정소속이 아닌 캐릭터로 선택시도한 유저 발견! : UserUID(%d)" ) % GetUID() );
			//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
			SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_INCORRECT_USER_UNIT, kPacketNot.m_vecPhoneNum );
#else
			SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::PNLT_ELSWORD_DEV_TEAM, kPacketNot.m_vecPhoneNum );
#endif SERV_SMS_TOTAL_MANAGER
			//}}
			if( kPacketNot.m_vecPhoneNum.empty() == false )
			{
				SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );
			}

			// 자동으로 어뷰저 등록
			m_kUserAbuserManager.RegEDAbuser( GetThisPtr<KGSUser>() );
			m_kUserAbuserManager.RegItemAbuser( GetThisPtr<KGSUser>() );
			//HackUserRegPacketMornitor();
		}
#endif SERV_POST_COPY_BUG_FIX
		//}}

		kPacketAck.m_iOK = kPacket_.m_kSelectUnitAck.m_iOK;
		SendPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK, kPacketAck );

		// 채널 이동실패로 인한 종료처리
		SetDisconnectReason( KStatistics::eSIColDR_Channel_Change_Fail );
		ReserveDestroy();
		return;
	}

	// 2007.12. 28. jseop. 닉네임이 잘못되어 변경해야 하는 경우.
	if( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_wstrNickName.compare( L"__DELETED__" ) == 0 )
	{		
		kPacketAck.m_iOK = NetError::ERR_SELECT_UNIT_06;
		SendPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK, kPacketAck );
		return;
	}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
	// 보스 필드라면 마을로 이동 시키자
	if( SiCXSLBattleFieldManager()->IsBossFieldID( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_kLastPos.m_iMapID ) == true )
	{
		kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_kLastPos.m_iMapID = SiCXSLMapData()->GetPossibleEnterVillageMapID( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_ucLevel, kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_mapDungeonClear );
		kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_kLastPos.m_bIgnoreLastTouch = true;
	}
	// 최초 선택이나, 채널 변경에서 동일하게 처리되는 구문은
	// 함수로 처리해서 공용으로 처리되도록 하는게 좋을 듯 하다...
#endif // SERV_BATTLE_FIELD_BOSS

	SendToAccountDB( DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_REQ, kPacket_ );
}


_IMPL_ON_FUNC( DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_ACK, KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_SERVER_SELECT ) );

	KEGS_CONNECT_CHANNEL_CHANGE_ACK kPacketAck;

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		CTime tReleaseTradeBlockTime;
		LIF( KncUtil::ConvertStringToCTime( kPacket_.m_kSelectUnitAck.m_wstrUnitCreateDate, tReleaseTradeBlockTime ) );
		tReleaseTradeBlockTime += CTimeSpan( SiKGameSysVal()->GetNewUnitTradeBlockDay(), 0, 0, 0 );
		CTime tCurTime = CTime::GetCurrentTime();

		if( SiKGameSysVal()->IsNewUnitTradeBlock() == true
			&& ( ( SiKGameSysVal()->GetNewUnitTradeBlockDay() != 0 && tCurTime < tReleaseTradeBlockTime )
			|| ( SiKGameSysVal()->GetNewUnitTradeBlockUnitClass() != 0 && GetUnitClass() < SiKGameSysVal()->GetNewUnitTradeBlockUnitClass() ) )
			)
		{
			m_tReleaseTradeBlockTime = tReleaseTradeBlockTime;

			m_kUserAbuserManager.SetTradeBlockUnit( true );
		}
		else
		{
			m_kUserAbuserManager.SetTradeBlockUnit( false );
		}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	if( kPacket_.m_kSelectUnitAck.m_iOK != NetError::NET_OK )
	{
		kPacketAck.m_iOK = kPacket_.m_kSelectUnitAck.m_iOK;
		goto end_proc;
	}
	else
	{
		//{{ 2009. 5. 13  최육사	P2P포트 체크 - 채널이동에서는 
		//if( GetP2PIP().empty() )
		//{
		//	START_LOG( cerr, L"P2P포트 체크 실패!" )
		//		<< BUILD_LOG( GetUID() )
		//		<< END_LOG;
		//}
		//}}

		//////////////////////////////////////////////////////////////////////////
		// 캐릭터 선택에 관련된 모든 처리는 이 함수 안에서 하도록 한다!
		if( OnAccountSelectUnitAck( kPacket_.m_kSelectUnitAck, true ) == false )
		{
			START_LOG( cerr, L"select unit error" )
				<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_kSelectUnitAck.m_iOK ) )
				<< END_LOG;

			goto end_proc;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// 채널 이동 하면서 임시 보관 했던 데이터는 여기서 처리하자!
		SetAccountInfo( kPacket_.m_kVerifyAccountAck.m_kAccountInfo );
		m_kUserEventManager.SetEventTimerInfo( kPacket_.m_kChangeUserInfo.m_mapEventDurationTime );
		//}}		
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		m_kUserTitleManager.EquipTitle( kPacket_.m_kChangeUserInfo.m_iEquippedTitleID );
#else
		m_kUserTitleManager.EquipTitle( kPacket_.m_kChangeUserInfo.m_sEquippedTitleID );
#endif SERV_TITLE_DATA_SIZE
		//}}
		//}}
		//{{ 2010. 02. 08  최육사	PC방 채널 이동 오류 수정
		std::vector< KInventoryItemInfo > vecMoveItemInfo;
		m_kInventory.SetChannelChangeInventoryInfo( kPacket_.m_kChangeUserInfo.m_vecPcBangItemInfo, vecMoveItemInfo );
		//}}
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		m_wstrWinterVacationEventRegDate	= kPacket_.m_kChangeUserInfo.m_wstrWinterVacationEventRegDate;
		m_iWinterVacationEventCount			= kPacket_.m_kChangeUserInfo.m_iWinterVacationEventCount;
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		//{{ 2012. 1. 2		Merge 박세훈	( 2013.1.2 지헌 : 미니맵 채널 이동 시 누락됬었던 정보 추가 )
#ifdef SERV_CHANNEL_CHANGE_BUG_FIX
		m_nUnitSlot = kPacket_.m_kChangeUserInfo.m_nUnitSlot;
#endif SERV_CHANNEL_CHANGE_BUG_FIX
		//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-30
		if( 0 < kPacket_.m_kChangeUserInfo.m_iSummonedRidingPetUID )
		{
			KEGS_SUMMON_RIDING_PET_REQ kPacket;
			kPacket.m_iRidingPetUID = kPacket_.m_kChangeUserInfo.m_iSummonedRidingPetUID;
			kPacket.m_bNeedAck		= false;

			KEventPtr spEvent( new KEvent );
			spEvent->SetData( PI_GS_USER, NULL, EGS_SUMMON_RIDING_PET_REQ, kPacket );
			LIF( spEvent->m_kDestPerformer.AddUID( GetUID() ) );
			QueueingEvent( spEvent );
		}
#endif	// SERV_RIDING_PET_SYSTM

		// 장착 정보가 바뀌었으므로 다른 유저들에게도 알린다.
		SendUpdateChangeEquippedItem( vecMoveItemInfo );

		//////////////////////////////////////////////////////////////////////////
		// 채널이동
		kPacketAck.m_kConnectAck = kPacket_.m_kConnectAck;
		kPacketAck.m_kVerifyAccountAck = kPacket_.m_kVerifyAccountAck;
		kPacketAck.m_kChangeUserInfo = kPacket_.m_kChangeUserInfo;
		kPacketAck.m_iOK = NetError::NET_OK;
		//////////////////////////////////////////////////////////////////////////
	}

end_proc:
	SendPacket( EGS_CONNECT_CHANNEL_CHANGE_ACK, kPacketAck );

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	SetDungeonCount(kPacket_.m_kChangeUserInfo.m_DungeonCount);
	SetFieldMosterKillCount(kPacket_.m_kChangeUserInfo.m_FieldMonsterKillCount);
	SetDungeonClearUI(kPacket_.m_kChangeUserInfo.m_DungeonClearUI);
	SetFieldCountUI(kPacket_.m_kChangeUserInfo.m_FieldCountUI);
	CTime tTempTime;
	if(KncUtil::ConvertStringToCTime( kPacket_.m_kChangeUserInfo.m_wstrButtonClickTime, tTempTime ) == false)
	{
		START_LOG( cerr, L"채널 이동 후 Ctime으로 시간변환 실패" )
			<< BUILD_LOG( kPacket_.m_kChangeUserInfo.m_wstrButtonClickTime )
			<< END_LOG;
		tTempTime = CTime::GetCurrentTime();
	}
	SetButtonClickTime(tTempTime);
	SetRemaindTime(kPacket_.m_kChangeUserInfo.m_RemaindTime);
	SetStartButtonPush(kPacket_.m_kChangeUserInfo.m_ButtonStartUI);
	SetCoboItemGive(kPacket_.m_kChangeUserInfo.m_bCoboItemGive);

	kPacket_.m_kSelectUnitAck.m_wstrButtonClickTime_One = kPacket_.m_kChangeUserInfo.m_wstrButtonClickTime;
	kPacket_.m_kSelectUnitAck.m_bItemGive = kPacket_.m_kChangeUserInfo.m_bCoboItemGive;
	kPacket_.m_kSelectUnitAck.m_iDungeonClearCount = kPacket_.m_kChangeUserInfo.m_DungeonCount;
	kPacket_.m_kSelectUnitAck.m_iFieldMonsterKillCount = kPacket_.m_kChangeUserInfo.m_FieldMonsterKillCount;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

	
	if( kPacket_.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
	{
		//////////////////////////////////////////////////////////////////////////
		// 캐릭터 선택 성공한 이후에 처리하는 것들은 모두 이 함수 안에서 처리하도록 한다!
		OnSelectUnitSuccess( kPacket_.m_kSelectUnitAck, true );
		//////////////////////////////////////////////////////////////////////////

#ifdef SERV_GLOBAL_EVENT_TABLE
		m_mapGlobalEventData = kPacket_.m_kChangeUserInfo.m_mapGlobalEventData;
		GetGlobalEventTableData(m_mapGlobalEventData);
#endif //SERV_GLOBAL_EVENT_TABLE

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		SetEventMoney( kPacket_.m_kSelectUnitAck.m_iEventMoney );

		KEGS_UPDATE_EVENT_MONEY_NOT kNot;
		kNot.m_iEventMoney = GetEventMoney();
		SendPacket( EGS_UPDATE_EVENT_MONEY_NOT, kNot );
#endif // SERV_EVENT_MONEY

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		IF_EVENT_ENABLED( CEI_EVENT_COBO_DUNGEON_AND_FIELD )
		{
			if(  GetLevel() >= 10 )
			{
				KEGS_EVENT_COBO_DUNGEON_FIELD_NOT kPacketCoboNot;
				//현재 시간을 받아온다.
				CTime tChangeEventTime = SiKGameEventScriptManager()->GetCoboEventData()[0];
				CTime tCurTime_ = CTime::GetCurrentTime();
				//현재 시간과 바뀌어야 하는 이벤트 시간을 비교한다.
				std::wstring GameEventTime = tChangeEventTime.Format(L"%Y-%m-%d %H:%M:%S");

				if( tCurTime_ < tChangeEventTime)
				{
					///현재 시간이 바뀌어야 하는 기준 시간보다 작으면 던전 클리어를 열어 주어야 한다.
					///단 주말일 경우에는 필드 카운트도 열어주어야 하니까 주말인지 체크를 해야한다.
					CTime	tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[1];
					CTime   tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[2];
					GameEventTime = tWeekEndTimeStart.Format(L"%Y-%m-%d %H:%M:%S");
					START_LOG( clog, L"던전 주말 타임 체크시작" )
						<< BUILD_LOG( GameEventTime )
						<< END_LOG;

					GameEventTime = tWeekEndTimeEnd.Format(L"%Y-%m-%d %H:%M:%S");
					START_LOG( clog, L"던전 주말 타임 체크끝" )
						<< BUILD_LOG( GameEventTime )
						<< END_LOG;
					//클릭 한적이 있다.
					CTime tClickTime;
					if( KncUtil::ConvertStringToCTime( kPacket_.m_kChangeUserInfo.m_wstrButtonClickTime, tClickTime ) == false )
					{

						tClickTime = CTime::GetCurrentTime();  //실패 했을 경우 그냥 false처리 하자
					}
					SetButtonClickTime(tClickTime);
					if(kPacket_.m_kChangeUserInfo.m_bCoboItemGive == true)
					{
						if( tClickTime.GetDay() < tCurTime_.GetDay()) //하루가 지났냐
						{
							///하루가 지났으면 보상 기록 초기화 하고 UI활성화 시켜주면 된다.
							kPacketCoboNot.m_StartButtonUI = true;
							kPacketCoboNot.m_DungeonCountUI = true;
							kPacketCoboNot.m_DungeonCount = 0;
							kPacketCoboNot.m_iRemaindTime = -1;
							if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //이러면 주말이다
							{
								//주말이면 필드카운트 UI도 활성화 
								kPacketCoboNot.m_FieldCountUI = true;
								kPacketCoboNot.m_FieldMonsterKillCount = 0;
							}
							kPacketCoboNot.m_tPushTime = tClickTime.GetTime();
							SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
							START_LOG( clog, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
								<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
								<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
								<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
								<< END_LOG;
							///DB에 초기화 하러 가자
							//여기서 몬스터 킬수랑 던전 클리어수 보관해야 하니까 저장 하자
							KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
							kPacketToDB.m_iUnitUID = GetCharUID();
							kPacketToDB.m_bItemGive = 0;
							kPacketToDB.m_wstrButtonClickTime_One = tClickTime.Format(L"%Y-%m-%d %H:%M:%S");
							kPacketToDB.m_iDungeonClearCount = 0;
							kPacketToDB.m_iFieldMonsterKillCount = 0;
							kPacketCoboNot.m_tPushTime = tClickTime.GetTime();
							SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
							SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
							SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
							SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
							SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
							SetDungeonCount(kPacketCoboNot.m_DungeonCount);
							SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
							SetCoboItemGive(false);

						}
						else
						{
							///하루가 안지났으면 UI그냥 다 끄면 된다.
							kPacketCoboNot.m_StartButtonUI = false;
							kPacketCoboNot.m_DungeonCountUI = false;
							kPacketCoboNot.m_DungeonCount = 0;
							kPacketCoboNot.m_iRemaindTime = -1;
							if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //이러면 주말이다
							{
								//주말이면 필드카운트 UI도 활성화 
								kPacketCoboNot.m_FieldCountUI = false;
								kPacketCoboNot.m_FieldMonsterKillCount = 0;
							}
							kPacketCoboNot.m_tPushTime = tClickTime.GetTime();
							SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
							START_LOG( clog, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
								<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
								<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
								<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
								<< END_LOG;
							SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
							SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
							SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
							SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
							SetDungeonCount(kPacketCoboNot.m_DungeonCount);
							SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
							SetCoboItemGive(true);
						}
					}
					else
					{
						///채널 이동을 고려해야지

						///보상 안받았으면 UI다시 활성화 시켜주면 된다.
						kPacketCoboNot.m_StartButtonUI = kPacket_.m_kChangeUserInfo.m_ButtonStartUI;
						kPacketCoboNot.m_DungeonCountUI = kPacket_.m_kChangeUserInfo.m_DungeonClearUI;
						kPacketCoboNot.m_DungeonCount = kPacket_.m_kChangeUserInfo.m_DungeonCount;
						kPacketCoboNot.m_iRemaindTime = kPacket_.m_kChangeUserInfo.m_RemaindTime;
						if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //이러면 주말이다
						{
							//주말이면 필드카운트 UI도 활성화 
							kPacketCoboNot.m_FieldCountUI = true;
							kPacketCoboNot.m_FieldMonsterKillCount = kPacket_.m_kChangeUserInfo.m_FieldMonsterKillCount;
						}
						kPacketCoboNot.m_tPushTime = tClickTime.GetTime();
						SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
						START_LOG( clog, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
							<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
							<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
							<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
							<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
							<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
							<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
							<< END_LOG;
						SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
						SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
						SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
						SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
						SetDungeonCount(kPacketCoboNot.m_DungeonCount);
						SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
						SetCoboItemGive(false);
					}
				}			
				else
				{
					///현재 시간이 바뀌어야 하는 기준 시간보다 작으면 던전 클리어를 열어 주어야 한다.
					///단 주말일 경우에는 필드 카운트도 열어주어야 하니까 주말인지 체크를 해야한다.
					CTime	tWeekEndTimeStart = SiKGameEventScriptManager()->GetCoboEventData()[3];
					CTime tWeekEndTimeEnd = SiKGameEventScriptManager()->GetCoboEventData()[4];
					CTime TempTime = SiKGameEventScriptManager()->GetCoboEventData()[0];
					GameEventTime = tWeekEndTimeStart.Format(L"%Y-%m-%d %H:%M:%S");
					START_LOG( clog, L"필드 주말 타임 체크시작" )
						<< BUILD_LOG( GameEventTime )
						<< END_LOG;
					GameEventTime = tWeekEndTimeEnd.Format(L"%Y-%m-%d %H:%M:%S");
					START_LOG( clog, L"필드 주말 타임 체크끝" )
						<< BUILD_LOG( GameEventTime )
						<< END_LOG;
					CTime tClickTime;
					if( KncUtil::ConvertStringToCTime( kPacket_.m_kChangeUserInfo.m_wstrButtonClickTime, tClickTime ) == false )
					{

						tClickTime = CTime::GetCurrentTime();  //실패 했을 경우 그냥 false처리 하자
					}
					SetButtonClickTime(tClickTime);
					if(kPacket_.m_kChangeUserInfo.m_bCoboItemGive == true)
					{
						if( tClickTime.GetDay() < tCurTime_.GetDay()) //하루가 지났냐
						{
							///하루가 지났으면 보상 기록 초기화 하고 UI활성화 시켜주면 된다.
							kPacketCoboNot.m_StartButtonUI = true;
							kPacketCoboNot.m_FieldCountUI = true;
							kPacketCoboNot.m_FieldMonsterKillCount = 0;
							kPacketCoboNot.m_DungeonCount = 0;
							kPacketCoboNot.m_iRemaindTime = -1;
							if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //이러면 주말이다
							{
								//주말이면 필드카운트 UI도 활성화 
								kPacketCoboNot.m_DungeonCountUI = true;
							}
							kPacketCoboNot.m_tPushTime = tClickTime.GetTime();
							SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
							START_LOG( clog, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
								<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
								<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
								<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
								<< END_LOG;
							///DB에 초기화 하러 가자
							KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT kPacketToDB;
							kPacketToDB.m_iUnitUID = GetCharUID();
							kPacketToDB.m_bItemGive = 0;
							kPacketToDB.m_wstrButtonClickTime_One = tClickTime.Format(L"%Y-%m-%d %H:%M:%S");
							kPacketToDB.m_iDungeonClearCount = 0;
							kPacketToDB.m_iFieldMonsterKillCount = 0;
							SendToGameDB( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, kPacketToDB );
							SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
							SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
							SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
							SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
							SetDungeonCount(kPacketCoboNot.m_DungeonCount);
							SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
							SetCoboItemGive(false);
						}
						else
						{
							///하루가 안지났으면 UI그냥 다 끄면 된다.
							kPacketCoboNot.m_StartButtonUI = false;
							kPacketCoboNot.m_DungeonCountUI = false;
							kPacketCoboNot.m_DungeonCount = 0;
							kPacketCoboNot.m_iRemaindTime = -1;
							//주말이면 필드카운트 UI도 활성화 
							kPacketCoboNot.m_FieldCountUI = false;
							kPacketCoboNot.m_FieldMonsterKillCount = 0;
							kPacketCoboNot.m_tPushTime = tClickTime.GetTime();
							SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
							START_LOG( clog, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
								<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
								<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
								<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
								<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
								<< END_LOG;
							SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
							SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
							SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
							SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
							SetDungeonCount(kPacketCoboNot.m_DungeonCount);
							SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
							SetCoboItemGive(true);
						}
					}
					else
					{
						///보상 안받았으면 UI다시 활성화 시켜주면 된다.
						kPacketCoboNot.m_StartButtonUI = kPacket_.m_kChangeUserInfo.m_ButtonStartUI;
						kPacketCoboNot.m_DungeonCount = kPacket_.m_kChangeUserInfo.m_DungeonCount;
						kPacketCoboNot.m_iRemaindTime = kPacket_.m_kChangeUserInfo.m_RemaindTime;
						//주말이면 필드카운트 UI도 활성화 
						kPacketCoboNot.m_FieldCountUI = true;
						kPacketCoboNot.m_FieldMonsterKillCount = kPacket_.m_kChangeUserInfo.m_FieldMonsterKillCount;
						if(tCurTime_ > tWeekEndTimeStart && tCurTime_ < tWeekEndTimeEnd) //이러면 주말이다
						{
							kPacketCoboNot.m_DungeonCountUI = true;
						}
						kPacketCoboNot.m_tPushTime = tClickTime.GetTime();
						SendPacket(EGS_EVENT_COBO_DUNGEON_FIELD_NOT,kPacketCoboNot);
						START_LOG( clog, L"클릭한적 있을떄 던전 보상 지급받았을때 하루 지났을때" )
							<< BUILD_LOG( kPacketCoboNot.m_StartButtonUI )
							<< BUILD_LOG( kPacketCoboNot.m_DungeonCountUI )
							<< BUILD_LOG( kPacketCoboNot.m_DungeonCount )
							<< BUILD_LOG( kPacketCoboNot.m_iRemaindTime )
							<< BUILD_LOG( kPacketCoboNot.m_FieldCountUI )
							<< BUILD_LOG( kPacketCoboNot.m_FieldMonsterKillCount )
							<< END_LOG;
						SetRemaindTime(kPacketCoboNot.m_iRemaindTime);
						SetStartButtonPush(kPacketCoboNot.m_StartButtonUI);
						SetDungeonClearUI(kPacketCoboNot.m_DungeonCountUI);
						SetFieldCountUI(kPacketCoboNot.m_FieldCountUI);
						SetDungeonCount(kPacketCoboNot.m_DungeonCount);
						SetFieldMosterKillCount(kPacketCoboNot.m_FieldMonsterKillCount);
						SetCoboItemGive(false);
					}

				}
			}
		}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
	}
	else
	{
		// 채널 이동실패로 인한 종료처리
		SetDisconnectReason( KStatistics::eSIColDR_Channel_Change_Fail );
		ReserveDestroy();
	}
}

IMPL_ON_FUNC( EGS_JOIN_FIELD_CHANNEL_CHANGE_REQ )
{
	//{{ 2010. 9. 7	최육사	중복 패킷 필터 개선
#ifdef SERV_REPEAT_FILTER_REFAC
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_FIELD_MAP ), EGS_JOIN_FIELD_CHANNEL_CHANGE_REQ, EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK );
#else
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
#endif SERV_REPEAT_FILTER_REFAC
	//}}

	KEGS_JOIN_FIELD_CHANNEL_CHANGE_ACK kPacketAck;
	KERM_JOIN_FIELD_REQ kPacket;

	// 채널이동전에 저장했던 mapid와 진입요청 mapid가 일치하는지 검사
	if( GetMapID() != kPacket_.m_iMapID )
	{
		START_LOG( cerr, L"채널이동 한건데 mapid가 틀리다? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( GetMapID() )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map정보가 이상합니다.
		goto end_proc;
	}

	// 해당 map에 들어갈 수 있는지 조건 체크
	//{{ 2012. 09. 21   김민성   마을 입장 오류 메시지 구분
#ifdef SERV_ENTER_VILLAGE_ERROR
	int iTempLevel = 0;
	int iTempDungeonID = 0;
	if( !CheckEnterTheVillage( kPacket_.m_iMapID, iTempLevel, iTempDungeonID ) )
#else
	if( !CheckEnterTheVillage( kPacket_.m_iMapID ) )
#endif SERV_ENTER_VILLAGE_ERROR
	//}}
	{
		START_LOG( cwarn, L"마을에 입장할 수 있는 조건이 안됨." )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::GetLastError();
		goto end_proc;
	}	

	if( GetFieldUID() > 0 )
	{
		START_LOG( cerr, L"채널이동 한건데 필드에 들어와있다? 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map정보가 이상합니다.
		goto end_proc;
	}

	if( !SiCXSLMapData()->VerifyMapID( kPacket_.m_iMapID ) )
	{
		START_LOG( cerr, L"필드에 입장하려는데 MapID가 이상합니다." )
			<< BUILD_LOG( kPacket_.m_iMapID )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map정보가 이상합니다.
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	// FieldUId를 얻는다.
	if( !SiCXSLMapData()->GetFieldUID( GetMapID(), kPacket.m_iFieldUID ) )
	{
		START_LOG( cerr, L"잘못된 맵 정보" )
			<< BUILD_LOG( GetMapID() )
			<< END_LOG;

		kPacketAck.m_iOK = NetError::ERR_FIELD_00; // map정보가 이상합니다.
		goto end_proc;
	}

	// 아직 필드에 입장해있지 않다!
	GetFieldUserInfo( kPacket.m_kFieldUserInfo );
	kPacket.m_kFieldUserInfo.m_vPos = kPacket_.m_vPos;
	kPacket.m_kFieldOption		    = m_kFieldOption;
	if( GetPartyUID() > 0 )
	{
		//{{ 2010. 03. 02  최육사	채널 통합 파티
#ifdef SERV_CHANNEL_PARTY
		KEPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT kPacketToPM;
		kPacketToPM.m_iPartyUID = GetPartyUID();
		SendToCnParty( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT, kPacketToPM );
#else
#pragma ELSE_MESSAGE
		//if( SiKPartyManager()->GetUserUIDList( GetPartyUID(), kPacket.m_setPartyUIDList ) == false )
		//{
		//	START_LOG( cerr, L"PartyUID는 있는데 파티리스트 정보를 얻을 수 없다?" )
		//		<< BUILD_LOG( GetCharUID() )
		//		<< BUILD_LOG( GetCharName() )
		//		<< BUILD_LOG( GetPartyUID() )
		//		<< END_LOG;
		//}
#endif SERV_CHANNEL_PARTY
		//}}		
	}

	//{{ 2009. 4. 9  최육사		친구리스트	
	for( std::map< std::wstring, UidType >::const_iterator mitF = m_mapFriendList.begin(); mitF != m_mapFriendList.end(); ++mitF )
	{
		kPacket.m_setFriendUIDList.insert( mitF->second );
	}
	//}}

	// 입장패킷 보내기전에 미리 FieldUID를 저장한다.
	SetFieldUID( kPacket.m_iFieldUID );
	LIF( GetCharUID() != 0 );
	_SendToGSField( GetFieldUID(), ERM_JOIN_FIELD_CHANNEL_CHANGE_REQ, kPacket );
	return;

end_proc:
	SendPacket( EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK, kPacketAck );
}

_IMPL_ON_FUNC( ERM_JOIN_FIELD_CHANNEL_CHANGE_ACK, KERM_JOIN_FIELD_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );
	// 상태는 어떻게 되나?

	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		LIF( GetFieldUID() == kPacket_.m_iFieldUID );

		// 필드 진입 성공
		SetFieldUID( kPacket_.m_iFieldUID );

		//파티가 있을경우 필드이동 성공시 파티에 업데이트 함.
		SendUpdatePartyUnitInfo( true );

		//{{ 2009. 5. 13  최육사	P2P포트 체크
		if( GetP2PIP().empty() )
		{
			START_LOG( cerr, L"채널이동후 P2P포트 체크 실패!" )
				<< BUILD_LOG( GetUID() )
				<< END_LOG;
		}
		//}}
#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
		if(m_bIdentityConfirmCheck == false)
		{
			if(m_GiantAccountType.IsType(KGiantAccountType::ADULT_PLAYER) == false)
			{
				KEGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT kPacket;
				kPacket.m_bCheckAdult = false;
				SendPacket( EGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT, kPacket );
			}
			m_bIdentityConfirmCheck = true;
		}
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	}

	//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	KEGS_UDP_KICK_GAMEEDIT_NOT kPacketUDP;
	kPacketUDP.m_bUDPSetKick = GetKGSSimLayer()->GetUDPKickOff();
	SendPacket( EGS_UDP_KICK_GAMEEDIT_NOT, kPacketUDP );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}

	KEGS_JOIN_FIELD_CHANNEL_CHANGE_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_JOIN_FIELD_CHANNEL_CHANGE_ACK, kPacket );
}
//}}

//{{ 2009. 2. 13  최육사	채널리스트
IMPL_ON_FUNC_NOPARAM( EGS_GET_CHANNEL_LIST_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	KEGS_GET_CHANNEL_LIST_ACK kPacketAck;
	kPacketAck.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
#ifdef SERV_CHANNEL_LIST_RENEWAL
	kPacketAck.m_vecChannelInfo = SiKChannelManager()->GetChannelInfo();
	kPacketAck.m_mapChannelBonusInfo = SiKChannelManager()->GetChannelBonusInfo();
#else //SERV_CHANNEL_LIST_RENEWAL
	kPacketAck.m_vecChannelList = SiKChannelManager()->GetChannelList();
#endif //SERV_CHANNEL_LIST_RENEWAL
	SendPacket( EGS_GET_CHANNEL_LIST_ACK, kPacketAck );
}
//}}

IMPL_ON_FUNC_NOPARAM( EGS_ENTRY_POINT_GET_CHANNEL_LIST_REQ )
{
    KEGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK kPacketAck;
    //kPacketAck.m_iServerGroupID = ( KBaseServer::GetKObj()->GetServerGroupID() == SEnum::SGI_SOLES ? SEnum::SGI_GAIA : SEnum::SGI_SOLES );

    switch( KBaseServer::GetKObj()->GetServerGroupID() )
    {
    case SEnum::SGI_SOLES:
        kPacketAck.m_mapSolesChannelList = SiKChannelManager()->GetChannelMap();
        kPacketAck.m_mapGaiaChannelList = SiKChannelManager()->GetChannelMap2nd();
        kPacketAck.m_mapSolesChannelBonusList = SiKChannelManager()->GetChannelBonusList();
        kPacketAck.m_mapGaiaChannelBonusList = SiKChannelManager()->GetChannelBonusList2nd();
        break;
    case SEnum::SGI_GAIA:
        kPacketAck.m_mapSolesChannelList = SiKChannelManager()->GetChannelMap2nd();
        kPacketAck.m_mapGaiaChannelList = SiKChannelManager()->GetChannelMap();
        kPacketAck.m_mapSolesChannelBonusList = SiKChannelManager()->GetChannelBonusList2nd();
        kPacketAck.m_mapGaiaChannelBonusList = SiKChannelManager()->GetChannelBonusList();
        break;
    default:
        kPacketAck.m_mapSolesChannelList = SiKChannelManager()->GetChannelMap();
        kPacketAck.m_mapGaiaChannelList = SiKChannelManager()->GetChannelMap2nd();
        kPacketAck.m_mapSolesChannelBonusList = SiKChannelManager()->GetChannelBonusList();
        kPacketAck.m_mapGaiaChannelBonusList = SiKChannelManager()->GetChannelBonusList2nd();
    }

    SendPacket( EGS_ENTRY_POINT_GET_CHANNEL_LIST_ACK, kPacketAck );

}

IMPL_ON_FUNC_NOPARAM( EGS_DISCONNECT_FOR_SERVER_SELECT_REQ )
{
	// 채널 선택창과 필드, 보안패드 상태에서만 호출 가능
	VERIFY_STATE( ( 3, KGSFSM::S_SERVER_SELECT, KGSFSM::S_FIELD_MAP, KGSFSM::S_CHECK_SECOND_PW ) );

	// 게임중 캐릭선택으로 오는것을 게임종료처리와 같은 상황으로 본다.
	// 유닛이 선택되어 있다면 해당 유닛정보를 DB업데이트, 로그인서버에서 지우기, 나머지 처리를 한다.
	if( GetCharUID() > 0 )
	{
		//{{ 2010. 8. 4	최육사	코드 리팩토링
		//#ifdef SERV_DESELECT_UNIT_REFAC
		//{{ 2012. 11. 22	최육사		캐릭터 선택 해제 코드 리팩토링
#ifdef SERV_DESELECT_UNIT_CODE_REFACTORING
		OnDeselectUnit( EGS_DISCONNECT_FOR_SERVER_SELECT_REQ, false );
#else
		OnDeselectUnit( true, EGS_DISCONNECT_FOR_SERVER_SELECT_REQ );
#endif SERV_DESELECT_UNIT_CODE_REFACTORING
		//}}		
		//#endif SERV_DESELECT_UNIT_REFAC
		//}}
		return;
	}

	// 로그인서버로 유저정보 빼내러 가자! - 이중접속 방지
	SendDisconnectEvent( EGS_DISCONNECT_FOR_SERVER_SELECT_REQ );
}

IMPL_ON_FUNC_NOPARAM( EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ )
{
	// 필드에서만 호출 가능
	VERIFY_STATE( ( 1, KGSFSM::S_FIELD_MAP ) );

	// 게임중 캐릭선택으로 오는것을 게임종료처리와 같은 상황으로 본다.
	// 유닛이 선택되어 있다면 해당 유닛정보를 DB업데이트, 로그인서버에서 지우기, 나머지 처리를 한다.
	if( GetCharUID() > 0 )
	{
		//{{ 2010. 8. 4	최육사	코드 리팩토링
		//#ifdef SERV_DESELECT_UNIT_REFAC
		//{{ 2012. 11. 22	최육사		캐릭터 선택 해제 코드 리팩토링
#ifdef SERV_DESELECT_UNIT_CODE_REFACTORING
		OnDeselectUnit( EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ, false );
#else
		OnDeselectUnit( true, EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ );
#endif SERV_DESELECT_UNIT_CODE_REFACTORING
		//}}		
		//#endif SERV_DESELECT_UNIT_REFAC
		//}}
		return;
	}

	// 로그인서버로 유저정보 빼내러 가자! - 이중접속 방지
	SendDisconnectEvent( EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ );
}

_IMPL_ON_FUNC( ELG_USER_DISCONNECT_ACK, KELG_USER_DISCONNECT_REQ )
{
	// 상태제한?

	// 채널이동을 위한 종료!
	m_bLogoutFail = false;
	m_bReportDisconnectToLogin = false; // 로그인서버에 유저정보 지우고 왔으므로 OnDestroy()에서 다시 빼내러 가면 안된다!

	// 정상적으로 종료처리 되었음을 알려줌
	u_short usEventIDAck = 0;

	switch( kPacket_.m_usEventID )
	{
	case EGS_DISCONNECT_FOR_SERVER_SELECT_REQ:  usEventIDAck = EGS_DISCONNECT_FOR_SERVER_SELECT_ACK; break;
	case EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ: usEventIDAck = EGS_DISCONNECT_FOR_CHANNEL_CHANGE_ACK; break;
	default:
		START_LOG( cerr, L"잘못된 req이벤트로 disconnect되었습니다." )
			<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
			<< END_LOG;
	}

	SendID( usEventIDAck );

	// 채널이동으로 인한 접속종료처리
	SetDisconnectReason( KStatistics::eSIColDR_Channel_Change );
	ReserveDestroy();
}
//}}

IMPL_ON_FUNC( EGS_CLIENT_QUIT_REQ )
{
	// 로그인 실패 flag
	m_bLogoutFail = false;

	SendID( EGS_CLIENT_QUIT_ACK );

	// 종료 사유 설정
	int iIndex = KStatistics::eSIColDR_Unknown;

	switch( kPacket_.m_iReason )
	{
	case NetError::ERR_CLIENT_QUIT_00: // 정상종료
		iIndex = KStatistics::eSIColDR_Normal_Disconnect;
		break;

	case NetError::ERR_CLIENT_QUIT_01: // 해킹 감지
		iIndex = KStatistics::eSIColDR_Client_Hacking;
		break;

	case NetError::ERR_CLIENT_QUIT_02: // 클라이언트 크래쉬
		iIndex = KStatistics::eSIColDR_Client_Crash;
		break;

#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT
	case NetError::ERR_CLIENT_QUIT_03: // 클라이언트 CheckKom.xml 파싱 오류
		iIndex = KStatistics::eSIColDR_InvlideCheckKomScript;
		break;
#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
	case NetError::ERR_CLIENT_QUIT_04: // 클라이언트 Kom 파일 변조 감지
		iIndex = KStatistics::eSIColDR_InvaildKomDetect;
		break;
#endif // SERV_KOM_FILE_CHECK_ADVANCED

	default:
		START_LOG( cerr, L"잘못된 disconnect reason 통계 인덱스임." )
			<< BUILD_LOG( kPacket_.m_iReason )
			<< END_LOG;
		break;
	}

	SetDisconnectReason( iIndex );
	ReserveDestroy();
}

IMPL_ON_FUNC_NOPARAM( E_DISABLE_HB_CHECK_REQ )
{
	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_bCheckHBTick );

	m_bCheckHBTick = false;
}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
	m_bDestroyReserved = true;
}

//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
IMPL_ON_FUNC( SOAP_GET_TOONILAND_ID_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetToonilandID( kPacket_.m_wstrToonilandID );

		KEGS_TOONILAND_USER_ID_NOT kNot;
		kNot.m_iTooniLandID = kPacket_.m_wstrToonilandID;

		SendPacket( EGS_TOONILAND_USER_ID_NOT, kNot );
	}
}
#endif SERV_CHANNELING_SOAP
//}}


IMPL_ON_FUNC_NOPARAM( E_CHECK_SEQUENCE_COUNT_NOT )
{
	//{{ 2012. 03. 14	김민성		패킷 시퀀스 카운트 체크
#ifdef SERV_CHECK_SEQUENCE_COUNT
	if( m_bCheckSequence == false )
	{
		m_bCheckSequence = true;

		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_SEQUENCE_COUNT;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );

		m_kUserHackingManager.SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_SEQUENCE_COUNT );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_SEQUENCE_COUNT;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
	}
#endif SERV_CHECK_SEQUENCE_COUNT
	//}}
}

//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
IMPL_ON_FUNC( EGB_REGISTER_SERVERSN_ACK )
{
 	if( kPacket_.m_iOK != NetError::NET_OK )
 	{
		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_SERVER_SN_OVERLAP_CONNECT;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_SERVER_SN_OVERLAP_CONNECT;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
 	}
}

_IMPL_ON_FUNC( EGB_REGISTER_SERVERSN_NOT, KEGB_REGISTER_SERVERSN_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_SERVER_SN_OVERLAP_CONNECT;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_SERVER_SN_OVERLAP_CONNECT;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
	}
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}

//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
IMPL_ON_FUNC_NOPARAM( EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ )
{
	VERIFY_STATE_REPEAT_FILTER( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ, EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK );
	SendToGameDB( DBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ );
}

IMPL_ON_FUNC( DBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK )
{
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK );
	SendPacket( EGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK, kPacket_ );
}
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
//}}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
IMPL_ON_FUNC( EGS_SYSTEM_INFO )
{
	//유저 PC 사양 정보 저장
	m_kUserStatistics.SetSystemInfoStatistics( kPacket_ );
}

IMPL_ON_FUNC( EGS_TCP_PING )
{
	SendPacket( EGS_TCP_PING, kPacket_ );
}//IMPL_ON_FUNC()

IMPL_ON_FUNC( EGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG )
{
	//마을에서 측정된 게임 성능 정보 저장
	m_kUserStatistics.SetKOGGamePerformanceStatistics_Village( kPacket_ );
}

IMPL_ON_FUNC( EGS_KOG_GAME_PERFORMANCE_FIELD_LOG )
{
	//필드에서 측정된 게임 성능 정보 저장
	m_kUserStatistics.SetKOGGamePerformanceStatistics_Field( kPacket_ );
}

IMPL_ON_FUNC( EGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG )
{
	//던전에서 측정된 게임 성능 정보 저장
	m_kUserStatistics.SetKOGGamePerformanceStatistics_Dungeon( kPacket_ );
}

IMPL_ON_FUNC( EGS_KOG_GAME_PERFORMANCE_PVP_LOG )
{
	//대전에서 측정된 게임 성능 정보 저장
	m_kUserStatistics.SetKOGGamePerformanceStatistics_PvP( kPacket_ );
}

#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
IMPL_ON_FUNC( EGS_GET_ACTIVE_LAGCHECK_REQ )
{
	KEGS_GET_ACTIVE_LAGCHECK_ACK kPacket;
	kPacket.m_bActiveLagCheck = GetKGameServer()->GetActiveLagCheck();

	SendPacket( EGS_GET_ACTIVE_LAGCHECK_ACK, kPacket );
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
IMPL_ON_FUNC( EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_REQ )
{
#ifdef SERV_ID_NETMARBLE_PCBANG
	KDBE_ID_PCBANG_CEHCK_AUTH_REQ kPacketDB;

	if ( kPacket_.m_iIsGameBangType <= -1 || kPacket_.m_bCheckClinetDirectPcBang != true)
	{
		m_kUserPcBangManager.SetIsPcBang(false);
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(-1);
#endif //SERV_PC_BANG_TYPE
	}
	else
	{
		m_kUserPcBangManager.SetIsPcBang( kPacket_.m_bCheckClinetDirectPcBang);
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(kPacket_.m_iIsGameBangType);
#endif //SERV_PC_BANG_TYPE
	}

	kPacketDB.m_bCheckClinetDirectPcBang = m_kUserPcBangManager.IsPcBang();
	kPacketDB.m_iIsGameBangType = m_kUserPcBangManager.GetPcBangType();

	kPacketDB.m_wstrMacAdress = kPacket_.m_wstrMacAdress;
	kPacketDB.m_wstrPublicIP = kPacket_.m_wstrPublicIP;

	//kPacketDB.m_wstrMacAdress = L"000115503111"; // 테스트용 나중에 삭제 해야 함
	//kPacketDB.m_wstrPublicIP = L"14.45.79.161";


	SendToIDPcbangDB( DBE_ID_PCBANG_CEHCK_AUTH_REQ, kPacketDB );

#else //SERV_ID_NETMARBLE_PCBANG

	KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK kPacket;

	if ( kPacket_.m_iIsGameBangType <= -1 || kPacket_.m_bCheckClinetDirectPcBang != true)
	{
		m_kUserPcBangManager.SetIsPcBang(false);
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(-1);
#endif //SERV_PC_BANG_TYPE
	}
	else
	{
		m_kUserPcBangManager.SetIsPcBang( kPacket_.m_bCheckClinetDirectPcBang);
#ifdef SERV_PC_BANG_TYPE
		m_kUserPcBangManager.SetPcBangType(kPacket_.m_iIsGameBangType);
#endif //SERV_PC_BANG_TYPE
	}

	kPacket.m_bCheckClinetDirectPcBang = m_kUserPcBangManager.IsPcBang();
	kPacket.m_iIsGameBangType = m_kUserPcBangManager.GetPcBangType();
	kPacket.m_kPcBangReward = GetPcBangReward();

	SendPacket( EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK, kPacket );

#endif //SERV_ID_NETMARBLE_PCBANG	
}
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef SERV_ID_NETMARBLE_PCBANG
IMPL_ON_FUNC( DBE_ID_PCBANG_CEHCK_AUTH_ACK )
{

	
	KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK kPacket;
	
	if (kPacket_.m_iIsNetmarbleGameBangType == 1) // 2013.07.02 lygan_조성욱 // 넷마블에서 우선 숫자 1이 인증된 PC 방이라고 알려주지만 차후 해당 숫자 번호로 PC 방 등급 나눌수도 있다.
	{
		m_kUserPcBangManager.SetIsPcBang( true );

		m_kUserPcBangManager.SetPcBangType(0);
	}

	if ( kPacket_.m_iIsNetmarbleGameBangType == 1 && kPacket_.m_bCheckClinetDirectPcBang == true ) // 2013.07.01 lygan_조성욱 // pika와 넷마블 둘다 PC방 가입되어 있을때
	{
		m_kUserPcBangManager.SetIsPcBang( true );

		m_kUserPcBangManager.SetPcBangType(0);
	}
	

	kPacket.m_bCheckClinetDirectPcBang = m_kUserPcBangManager.IsPcBang();
	kPacket.m_iIsGameBangType = m_kUserPcBangManager.GetPcBangType();
	kPacket.m_kPcBangReward = GetPcBangReward();

	
	SendPacket( EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK, kPacket );
}

#endif //SERV_ID_NETMARBLE_PCBANG
//{{ 2013. 05. 29	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
IMPL_ON_FUNC( EGB_UPDATE_BLOCK_RESULT_NOT )
{
	VERIFY_STATE( ( 4, KGSFSM::S_UID_UPDATED, KGSFSM::S_LOGINED, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	std::map< int, bool >::const_iterator mit;
	for( mit = kPacket_.m_mapBlockResult.begin(); mit != kPacket_.m_mapBlockResult.end(); ++mit )
	{
		switch( mit->first )
		{
		case KBlockInfo::BT_TRADE_BLOCK:
			{
				m_kUserAbuserManager.SetTradeBlockUser( mit->second );
			}
			break;

		case KBlockInfo::BT_ACCOUNT_BLOCK:
			{
#ifdef SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON
				KEGS_SHOW_DISCONNECT_REASON_NOT kPacket;
#ifdef SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
				kPacket.m_iOK = NetError::ERR_ACCOUNT_BLOCK_03;
#else //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
				kPacket.m_iOK = NetError::ERR_VERIFY_11;
#endif //SERV_ACCOUNT_BLOCK_MESSAGE_RENEWAL
				
				SendPacket( EGS_SHOW_DISCONNECT_REASON_NOT, kPacket );
#endif SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON

				// 접속 종료 처리
				SetDisconnectReason( KStatistics::eSIColDR_Account_Block );
				ReserveDestroy();
			}
			break;

		case KBlockInfo::BT_IP_BLOCK:
			{	
#ifdef SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON
				KEGS_SHOW_DISCONNECT_REASON_NOT kPacket;
				kPacket.m_iOK = NetError::ERR_IP_BLOCK_00;
				SendPacket( EGS_SHOW_DISCONNECT_REASON_NOT, kPacket );
#endif SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON

				// 접속 종료 처리
				SetDisconnectReason( KStatistics::eSIColDR_IP_Block );
				ReserveDestroy();
			}
			break;

		case KBlockInfo::BT_DLL_BLOCK:
			{
				// DLL체크
			}
			break;

		default:
			{
				START_LOG( cerr, L"정의 되지 않은 타입 입니다." )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;
			}
			break;
		}
	}
}
#endif SERV_BLOCK_LIST
//}}

//{{ 2013. 09. 24	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
IMPL_ON_FUNC( DBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_ACK )
{
	switch( GetStateID() )
	{
	case KGSFSM::S_INIT:
	case KGSFSM::S_EXIT:
	case KGSFSM::S_UID_UPDATED:
		{
			START_LOG( cerr, L"이 상태에서는 보상처리가 와도 아무것도 처리 하지 않습니다!" )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
			break;
		}

	default:
		break;
	}

    // 중계DB로부터 보상처리를 받았으니 해당 보상을 처리하자!
	BOOST_TEST_FOREACH( const KJPEventRewardInfo&, kInfo, kPacket_.m_vecRewardInfo )
	{
		// 보상처리는 쿠폰지급으로 하자.
		KEBILL_REWARD_COUPON_JP_EVENT_REQ kPacketReq;
		kPacketReq.m_kRewardInfo = kInfo;
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_iUserUID = GetUID();
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_wstrUserName = GetName();
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_iUnitUID = GetCharUID();
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_wstrUnitNickName = GetCharName();
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_ucLevel = GetLevel();
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_wstrIP = KncUtil::toWideString( GetIPStr() );
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_uiPublisherUID = m_kNexonAccountInfo.m_uiNexonSN;
		kPacketReq.m_kUseCouponPacket.m_PurchaserInfo.m_iChannelingCode = GetChannelCode();
		kPacketReq.m_kUseCouponPacket.m_wstrSerialCode = boost::str( boost::wformat( L"%d" ) % kInfo.m_iRID );

		// 퍼블 체크가 아니기에 바로 사용 요청
		SendToKOGBillingDB( EBILL_REWARD_COUPON_JP_EVENT_REQ, kPacketReq );
	}
}

IMPL_ON_FUNC( EBILL_REWARD_COUPON_JP_EVENT_ACK )
{
	switch( GetStateID() )
	{
	case KGSFSM::S_INIT:
	case KGSFSM::S_EXIT:
	case KGSFSM::S_UID_UPDATED:
		{
			START_LOG( cerr, L"이 상태에서는 보상처리가 와도 아무것도 처리 하지 않습니다!" )
				<< BUILD_LOG( GetName() )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( GetStateIDString() )
				<< END_LOG;
			break;
		}

	default:
		break;
	}

	if( kPacket_.m_kUseCouponPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"should be NET_OK" )
			<< BUILD_LOG( kPacket_.m_kUseCouponPacket.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_kUseCouponPacket.m_iOK ) )
			<< END_LOG;
		return;
	}

	// 아래 DirectPickup 수정 시 EBILL_USE_COUPON_RESULT_ACK에도 수정해줘야 함
//	std::vector< KBillCouponItemInfo >::iterator vit;
//	for( vit = kPacket_.m_vecBillCouponItemInfo.begin(); vit != kPacket_.m_vecBillCouponItemInfo.end(); vit++ )
//	{
//		KBillCouponItemInfo& refCouponItemInfo = *vit;
//
//		int iProductNo = refCouponItemInfo.m_iProductNO;
//		KBillProductInfo kInfo;
//		if( !SiKBillManager()->GetProductInfo( iProductNo, kInfo ) )
//		{
//			START_LOG( cerr, L"coupon error" )
//				<< BUILD_LOG( iProductNo )
//				<< BUILD_LOG( GetUID() )
//				<< END_LOG;
//
//			continue;
//		}
//
//		int iItemID = kInfo.m_iProductID;
//		if( iItemID <= 0 )
//		{
//			START_LOG( cerr, L"아이템 ID 변환 실패." )
//				<< BUILD_LOG( iItemID )
//				<< END_LOG;
//
//			continue;
//		}
//
//		//{{ 바로 PickUp 해야 하는 아이템들 PickUp 요청함
//		if ( IsDirectPickUpItem(iItemID) )
//		{
//			KEBILL_PICK_UP_REQ kPacketReq;
//			kPacketReq.m_iTransNo = refCouponItemInfo.m_iTranNo;
//			kPacketReq.m_iProductNo = refCouponItemInfo.m_iProductNO;
//			kPacketReq.m_iUnitUID = GetCharUID();
//#ifdef SERV_GLOBAL_CASH_PACKAGE
//			// 구조상의 이유로 쿠폰의 Direct Pickup은 패키지 상품은 되지 않는다.
//			// 유저가 PickUp 해야함
//			kPacketReq.m_byteProductKind = 0;
//#endif //SERV_GLOBAL_CASH_PACKAGE
//			SendToKOGBillingDB( EBILL_PICK_UP_REQ, kPacketReq );
//		}
//		//}}
//	}

	// 성공적으로 보상처리가 됐으면 이제 보상 결과를 중계DB에 업데이트 하러 가자!
	KDBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT kPacketNot;
	kPacketNot.m_iRID = kPacket_.m_kRewardInfo.m_iRID;
	SendToRelayDB( DBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT, kPacketNot );
}
#endif SERV_RELAY_DB_CONNECTION
//}}

//////////////////////////////////////////////////////////////////////////
//#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


