#include "GSUser.h"
#include <dbg/dbg.hpp>
#include "GSFSM.h"
#include "NetError.h"

#include "BaseServer.h"
#include "GameSysVal/GameSysVal.h"

#include "Enum/Enum.h"

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/EventRewardID.h"
#else
//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
	#include ".\rewardtable.h"
#endif SERV_SECOND_SECURITY
//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
	#include "odbc/Odbc.h"
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	#include "ServerSNMaker.h"
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
//}}

//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
#include "Mornitoring/MornitoringManager.h"
#endif SERV_CHECK_X2_EXE_MODULE
//}}

#ifdef SERV_NPROTECT_CS_AUTH_30
#include <boost/foreach.hpp>
#endif SERV_NPROTECT_CS_AUTH_30

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GSUSER_CPP
#pragma NOTE( "GSUserHandler.cpp 파일 컴파일 됩니당!" )
//////////////////////////////////////////////////////////////////////////

#define CLASS_TYPE      KGSUser

IMPL_ON_FUNC( EGS_KNPROTECT_AUTH_CHECK_NOT )
{
	//if( m_kGGAuth.CheckAuthAnswer( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	//{
	//	SetDisconnectReason( KStatistics::eSIColDR_NProtect_Wrong_Ack );
	//	ReserveDestroy();

	//	START_LOG( cerr, L"nProtect 인증 수신 실패.!" )
	//		<< END_LOG;
	//	return;
	//}
}

IMPL_ON_FUNC_NOPARAM( EGS_AGREE_HACK_USER_REQ )
{
	//특별한 스테이트 제한이 없다.
	if( m_cHackingUserType == SEnum::HUT_DISAGREE_HACK_USER )
	{
		SendToAccountDB( DBE_AGREE_HACK_USER_REQ, GetUID() );

		START_LOG( clog, L"해킹유저 동의.!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_kNexonAccountInfo.m_wstrID )
			<< END_LOG;

		return;
	}

	KEGS_AGREE_HACK_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_HACK_USER_00;
	SendPacket( EGS_AGREE_HACK_USER_ACK, kPacket );

	START_LOG( cerr, L"해킹유저 리스트에 등록된 유저가 아닌데 패킷이 날아옴.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_kNexonAccountInfo.m_wstrID )
		<< END_LOG;
}

_IMPL_ON_FUNC( DBE_AGREE_HACK_USER_ACK, KEGS_AGREE_HACK_USER_ACK )
{
	//특별한 스테이트 제한이 없다..흠~
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		m_cHackingUserType = SEnum::HUT_AGREE_HACK_USER;
		kPacket_.m_cHackingUserType = m_cHackingUserType;

		SendPacket( EGS_AGREE_HACK_USER_ACK, kPacket_ );
		return;
	}

	START_LOG( cerr, L"해킹유저 동의 디비 업데이트 실패.!" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_kNexonAccountInfo.m_wstrID )
		<< END_LOG;

	SendPacket( EGS_AGREE_HACK_USER_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_REPORT_HACK_USER_NOT )
{
	//특별한 스테이트 제한이 없다. 흠!!
	if( m_cHackingUserType == SEnum::HUT_NORMAL )
	{
		START_LOG( clog, L"해킹 유저 신고.!" )
			<< BUILD_LOG( GetUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( m_cHackingUserType );

		// 동의하지 않은 핵유저로 상태를 바꾸자!
		m_cHackingUserType = SEnum::HUT_DISAGREE_HACK_USER;

		SendToAccountDB( DBE_REPORT_HACK_USER_NOT, GetUID() );		
	}
	else
	{
		START_LOG( cwarn, L"이미 신고된 유저임.!" )
			<< BUILD_LOG( GetUID() )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetCharUID() )
#else
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( m_cHackingUserType )
			<< END_LOG;
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_NPROTECT_HACKING_USER_NOT )
{
	// nProtect 해킹 감지에 따른 통계설정 - 종료작업은 클라이언트에서 처리함.
	SetDisconnectReason( KStatistics::eSIColDR_nProtect_Hacking );
}

//{{ 2009. 8. 18  최육사	hack shield
IMPL_ON_FUNC( EGS_HACKSHIELD_CHECK_ACK )
{
	if( m_kHSAuth.OnHackShieldCheckAck( GetThisPtr<KGSUser>(), kPacket_ ) == false )
	{
		SetDisconnectReason( KStatistics::eSIColDR_HackShield_Wrong_Ack );
		ReserveDestroy();

		START_LOG( cerr, L"hack shield 수신 데이터 인증 실패! UserUID : " << GetUID() );
		return;
	}
}
//}}

//{{ 2009. 12. 29  최육사	NPCKILLALL해킹방지
IMPL_ON_FUNC_NOPARAM( EGS_DUNGEON_KILLALLNPC_CHECK_REQ )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	if( GetRoomUID() <= 0 )
	{
		START_LOG( cerr, L"방이 아닌데 이 패킷을 날렸네?" )
			<< BUILD_LOG( GetCharUID() )
			<< END_LOG;

		return;
	}

	if( CXSLRoom::GetRoomType( GetRoomUID() ) != CXSLRoom::RT_DUNGEON )
	{
		START_LOG( cerr, L"던전방이 아닌데 이 패킷을 날렸네?" )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( CXSLRoom::GetRoomType( GetRoomUID() ) )
			<< END_LOG;

		return;
	}

	// 던전방으로 날리자!
	SendToCnRoom( ERM_DUNGEON_KILLALLNPC_CHECK_REQ );
}

_IMPL_ON_FUNC( ERM_DUNGEON_KILLALLNPC_CHECK_ACK, KEGS_DUNGEON_KILLALLNPC_CHECK_ACK )
{
	VERIFY_STATE( ( 1, KGSFSM::S_ROOM ) );

	SendPacket( EGS_DUNGEON_KILLALLNPC_CHECK_ACK, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGS_REQUEST_HACKSHIELD_CHECK_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	START_LOG( cout, L"클라이언트에서 핵쉴드 체크 패킷을 요청하였습니다." )
#ifdef SERV_PRIVACY_AGREEMENT
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetCharUID() );
#else
		<< BUILD_LOG( GetUserName() )
		<< BUILD_LOG( GetCharName() );
#endif SERV_PRIVACY_AGREEMENT

	// 핵쉴드 요청 패킷 클라로 전송!
	if( m_kHSAuth.Tick( GetThisPtr<KGSUser>(), true ) == false )
	{
		SetDisconnectReason( KStatistics::eSIColDR_HackShield_Auth_Fail );
		ReserveDestroy();

		START_LOG( cerr, L"hack shield 인증 실패.!" )
			<< END_LOG;
		return;
	}
}
//}}

//{{ 2010. 05. 20  최육사	스크립트 파싱
#ifdef SERV_CLIENT_SCRIPT

IMPL_ON_FUNC( EGS_GET_CLIENT_SCRIPT_REQ )
{
	// 상태 굳이 안막아도 될듯..

	KEGS_GET_CLIENT_SCRIPT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;	

	//////////////////////////////////////////////////////////////////////////
	// 스크립트를 불러오자!

	if( SiKClientScriptManager()->GetClientScriptInfo( 0, kPacket.m_wstrScriptString ) == false )
	{
		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	}

	//////////////////////////////////////////////////////////////////////////

	SendPacket( EGS_GET_CLIENT_SCRIPT_ACK, kPacket );
}

#endif SERV_CLIENT_SCRIPT
//}}

//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//{{ 2010. 06. 17  최육사	오토핵 방지
#ifdef SERV_PROTECT_AUTO_HACK
_IMPL_ON_FUNC( ERM_SERVER_CHECK_HACK_USER_NOT, KEGS_SERVER_CHECK_HACK_USER_REQ )
{
	// 이런 패킷은 상태 막지 말자..	

	// 운영자면 해킹 유저로 처리 되지 않는다.
	if( GetAuthLevel() >= SEnum::UAL_GM )
	{
		START_LOG( cout, L"운영자가 해킹유저로 판단되었지만 해킹유저등록 및 계정 차단을 하지는 않음." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
			<< END_LOG;
		return;
	}

	// 체크 항목에 맞게 제재 처리
	switch( kPacket_.m_cHackingCheckType )
	{		
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_NPC_UNIT_DIE:			// 던전에서 몬스터를 죽인 정보가 이상한 경우
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_GET_ITEM:				// 발급되지 않은 아이템과 이상한 아이템을 획득하려는 경우
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_FIND:
		break; // 아무것도 하지 않는다!

	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CANT_KILL_NPC_KICK:	// 보이지 않는 몬스터를 핵유저가 잡았다!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_PLAY_START:			// 이미 플레이 시작 했는데 또 플레이 시작하려는 경우!
		break;
		//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_RESULT:			// 던전 클리어 결과가 비정상적인 경우!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_CN_SERVER_HACKING_CHECK
		//}}
		//{{ 2010. 11. 18	최육사	오토 핵 스테이지 로딩 체크
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_STAGE_LOADING:		// 이미 로딩한 스테이지를 다시 로딩하려는 경우!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
		//}}
		//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_DUNGEON_CLEAR_TIME:			// 던전 클리어 타임이 비정상적인 경우!
		{
//			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
//			HackUserRegTradeBlock( kPacket_.m_cHackingCheckType );
//			//{{ 2011. 09. 08	최육사	오토핵 패킷 모니터링
//#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
//			HackUserRegPacketMornitor();
//#endif SERV_AUTO_HACK_PACKET_MORNITOING
//			//}}
//	
//			m_kUserHackingManager.SetHackingUserSendType( kPacket_.m_cHackingCheckType );
//
//			// 패킷만 날리자!
//			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType
#ifdef SERV_PERIOD_ACCOUNT_BLOCK				
				, SEnum::UAL_BLOCKED_3, L"운영정책에 따라 접속이 제한되었습니다. (홈페이지>고객센터로 문의 주시기 바랍니다.)"
#endif //SERV_PERIOD_ACCOUNT_BLOCK				
				);
			SetRandomTimeKick( KStatistics::eSIColDR_Client_Hacking, 10, 50 );
		}
		break;
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		//}}
		//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_NPC_DIE_TIME:			// 던전 서브 스테이지에서 모든 액티브 몬스터 사냥 시간이 짧은 경우!!(1초)
		{
			//HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );

			m_kUserHackingManager.SetHackingUserSendType( kPacket_.m_cHackingCheckType );

			// 패킷만 날리자!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
		}
		break;
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}
		//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	case KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_VERIFY_DUNGEON_NPC_UID:			// 생성 되지 않은 NPC UID 를 클라이언트가 서버로 보냈음(핵킹툴사용 의심)
		{
			m_kUserHackingManager.SetHackingUserSendType( kPacket_.m_cHackingCheckType );

			// 패킷만 날리자!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket_ );
		}
		break;
#endif SERV_MAKE_MONSTER_UID_CHANGE	
		//}}
	default:
		{
			START_LOG( cerr, L"등록되지 않는 핵유저 체크 타입 입니다!" )
				<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_PROTECT_AUTO_HACK
//}}
#else
//{{ 2010. 06. 17  최육사	오토핵 방지
#ifdef SERV_PROTECT_AUTO_HACK
_IMPL_ON_FUNC( ERM_SERVER_CHECK_HACK_USER_NOT, KEGS_SERVER_CHECK_HACK_USER_NOT )
{
	// 이런 패킷은 상태 막지 말자..	

	// 운영자면 해킹 유저로 처리 되지 않는다.
	if( GetAuthLevel() >= SEnum::UAL_GM )
	{
		START_LOG( cout, L"운영자가 해킹유저로 판단되었지만 해킹유저등록 및 계정 차단을 하지는 않음." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetUserName() )
			<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
			<< END_LOG;
		return;
	}

	// 체크 항목에 맞게 제재 처리
	switch( kPacket_.m_cHackingCheckType )
	{		
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_NPC_UNIT_DIE:			// 던전에서 몬스터를 죽인 정보가 이상한 경우
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_GET_ITEM:				// 발급되지 않은 아이템과 이상한 아이템을 획득하려는 경우
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;

	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CANT_KILL_NPC_FIND:
		break; // 아무것도 하지 않는다!

	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CANT_KILL_NPC_KICK:	// 보이지 않는 몬스터를 핵유저가 잡았다!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_PLAY_START:			// 이미 플레이 시작 했는데 또 플레이 시작하려는 경우!
		break;
		//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CLEAR_RESULT:			// 던전 클리어 결과가 비정상적인 경우!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_CN_SERVER_HACKING_CHECK
		//}}
		//{{ 2010. 11. 18	최육사	오토 핵 스테이지 로딩 체크
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_STAGE_LOADING:		// 이미 로딩한 스테이지를 다시 로딩하려는 경우!
		{
			HackUserRegRejectedUser( kPacket_.m_cHackingCheckType );
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserKick( kPacket_ );

			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
		}
		break;
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
		//}}
		//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_DUNGEON_CLEAR_TIME:			// 던전 클리어 타임이 비정상적인 경우!
		{
			HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );
			HackUserRegTradeBlock( kPacket_.m_cHackingCheckType );
			//{{ 2011. 09. 08	최육사	오토핵 패킷 모니터링
#ifdef SERV_AUTO_HACK_PACKET_MORNITOING
			//HackUserRegPacketMornitor();
#endif SERV_AUTO_HACK_PACKET_MORNITOING
			//}}

			// 패킷만 날리자!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
		}
		break;
#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
		//}}
		//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_NPC_DIE_TIME:			// 던전 서브 스테이지에서 모든 액티브 몬스터 사냥 시간이 짧은 경우!!(1초)
		{
			//HackUserRegMonitorsUser( kPacket_.m_cHackingCheckType );

			// 패킷만 날리자!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
		}
		break;
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
		//}}
		//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	case KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_VERIFY_DUNGEON_NPC_UID:			// 생성 되지 않은 NPC UID 를 클라이언트가 서버로 보냈음(핵킹툴사용 의심)
		{
			// 패킷만 날리자!
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket_ );
		}
		break;
#endif SERV_MAKE_MONSTER_UID_CHANGE	
		//}}
	default:
		{
			START_LOG( cerr, L"등록되지 않는 핵유저 체크 타입 입니다!" )
				<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_PROTECT_AUTO_HACK
//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}

//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER

IMPL_ON_FUNC( EGS_MODULE_INFO_UPDATE_NOT )
{
	// 상태 제한 없음

	//{{ 2013. 06. 12	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST
	SendToGlobalServer( EGB_MODULE_INFO_UPDATE_NOT, kPacket_ );
#else
	SendToLoginServer( ELG_MODULE_INFO_UPDATE_NOT, kPacket_ );
#endif SERV_BLOCK_LIST
	//}}

	//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
	if( SiKMornitoringManager()->CheckModuleList_X2Module() == true )
	{
		// 로그 기록
		KDBE_MODULE_EXIST_LOG_NOT kLog;

		if( CheckModuleList_X2Module( kPacket_.m_vecModuleName, kLog.m_mapModule ) == false )
		{
			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			// x2.exe 가 존재하지 않는다면 문제가 있다.
			KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_X2_EXE_MODULE;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );

			m_kUserHackingManager.SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_X2_EXE_MODULE );

#else
			// x2.exe 가 존재하지 않는다면 문제가 있다.
			KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_X2_EXE_MODULE;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}

			CTime tCurr = CTime::GetCurrentTime();
			m_kUserSecurityManager.GetServerSN( kLog.m_iServerSN );
			kLog.m_iUserUID = GetUID();
			kLog.m_wstrRegDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
			SendToLogDB( DBE_MODULE_EXIST_LOG_NOT, kLog );
		}
	}
#endif SERV_CHECK_X2_EXE_MODULE
	//}}	
}

#endif SERV_DLL_MANAGER
//}}

//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
IMPL_ON_FUNC( ELG_TRADE_BLOCK_NOT )
{
	// 상태 제한 없음.

	m_kUserAbuserManager.SetTradeBlockUser( kPacket_.m_bIsTradeBlock );

	if( kPacket_.m_bIsTradeBlock )
	{
		START_LOG( cout, L"해킹 의심으로 인해 개인 거래가 차단되었습니다!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() );
#else
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() );
#endif SERV_PRIVACY_AGREEMENT
	}
	else
	{
		START_LOG( cout, L"개인 거래 차단이 해제되었습니다!" )
#ifdef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() );
#else
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() );
#endif SERV_PRIVACY_AGREEMENT
	}	
}
#endif SERV_HACK_USER_TRADE_BLOCK
//}}

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
IMPL_ON_FUNC( DBE_GET_SECOND_SECURITY_INFO_ACK )
{
#ifdef SERV_ENTRY_POINT
    VERIFY_STATE_ACK( ( 2, KGSFSM::S_LOGINED, KGSFSM::S_SERVER_SELECT ), EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK );
#endif SERV_ENTRY_POINT
        
	KEGS_SECOND_SECURITY_INFO_NOT kNot;
	kNot.m_iOK = kPacket_.m_iOK;
	kNot.m_bUseSecondPW = kPacket_.m_bUseSecondPW;

	if( kPacket_.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT ) // 비밀 번호 입력 실패가 10회 이상일 때 
	{
		SendPacket( EGS_SECOND_SECURITY_INFO_NOT, kNot );

		// 클라이서 종료 시키기로 하여 주석 됨
		//SetDisconnectReason( KStatistics::eSIColDR_Normal_Disconnect );
		//ReserveDestroy();															// 강제 종료
		return;
	}

	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	if( kPacket_.m_vecFirstSelectUnitReward.empty() == false )
	{
		m_kUserUnitSelectRewardManager.SetFirstSelectUnitRewardInfo( kPacket_.m_vecFirstSelectUnitReward );

		BOOST_TEST_FOREACH( KFirstSelectUnitReward ,kInfo , kPacket_.m_vecFirstSelectUnitReward )
		{
			KEGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT kReward;
			kReward = kInfo.m_cRewardType;
			SendPacket( EGS_EXIST_FIRST_SELECT_UNIT_REWARD_NOT, kReward );
		}
	}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

	// DB 에서 받아온 데이터 저장 (성공 일 때)
	CTime tLasstAuthDate;
	if( kPacket_.m_bUseSecondPW == true )
	{
		if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastAuthDate, tLasstAuthDate ) == false )
		{
			START_LOG( cerr, L"문자열 시간 변환 실패." )
				<< BUILD_LOG( kPacket_.m_wstrLastAuthDate )
				<< END_LOG;
		}

		//{{ 2011. 06. 23	최육사	2차 비번 예외처리
#ifdef SERV_CHECK_SECOND_PW_STATE
		StateTransition( KGSFSM::I_TO_CHECK_SECOND_PW );
#endif SERV_CHECK_SECOND_PW_STATE
		//}}
	}
	SetIsUseSecondPW( kPacket_.m_bUseSecondPW );
	SetSecondPWFailedCount( kPacket_.m_iFailedCount );
	SetSecondPW( kPacket_.m_wstrSecondPW );
	SetLastAuthDate( tLasstAuthDate );

	SendPacket( EGS_SECOND_SECURITY_INFO_NOT, kNot );

	if( m_kUserStatistics.IsFirstUserLoginToday( GetUID(), GetLastLoginDate() ) == true )
	{
		// 보안 사용 정보 로그 남기자!
		SendFirstLoginSecurityInfo();
	}

	//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	SetBF_Team( kPacket_.m_iBF_Team );
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}
	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	m_wstrWinterVacationEventRegDate	= kPacket_.m_wstrWinterVacationEventRegDate;
	m_iWinterVacationEventCount			= kPacket_.m_iWinterVacationEventCount;
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	if( kPacket_.m_bIsComeBackUser == true )	// 휴면 복귀 유저이다
	{
		m_kComeBackUserInfo.SetIsComeBackFirst( kPacket_.m_bIsFirst );
		m_kComeBackUserInfo.SetComeBackRewardLevel( kPacket_.m_iRewardStep );
		m_kComeBackUserInfo.SetComeBackBuffEndTime( kPacket_.m_wstrComeBackBuffEndDate );

		m_kTimer[TM_COME_BACK_USER].restart();		// 버프 시간 동기화 용

		if( kPacket_.m_bIsFirst == true )	// 최초 접속일 때 클라에게 알려주자
		{
			KEGS_COME_BACK_USER_NOT kNot;
			kNot = true;
			SendPacket( EGS_COME_BACK_USER_NOT, kNot );
		}
	}

	m_kComeBackUserInfo.SetIsComeBackUser( kPacket_.m_bIsComeBackUser );	// 휴면 복귀 유저인지 아닌지 셋팅

	//{{ 2012. 05. 16	박세훈	첫 접속 시 가이드 라인 띄워주기
#ifdef SERV_EVENT_GUIDELINE_POPUP
	CTime tLastConnectDate;
	if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastConnectDate, tLastConnectDate ) == false )
	{
		if( kPacket_.m_wstrLastConnectDate.size() != 0 )
		{
			START_LOG( cerr, L"문자열 시간 변환 실패." )
				<< BUILD_LOG( kPacket_.m_wstrLastConnectDate )
				<< END_LOG;
		}
	}
	else
	{
		if( tLastConnectDate < CTime( 2012, 5, 17, 6, 0, 0 ) )
		{
			m_bPopupTheGuideLine = true;
		}
	}
#endif SERV_EVENT_GUIDELINE_POPUP
	//}}

	SendToGameDB( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, GetName() );
#else
	SendToGameDB( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, GetName() );
#endif SERV_COME_BACK_USER_REWARD
	//}}

	//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	m_mmapDailyGiftBoxList = kPacket_.m_mmapDailyGiftBoxList;
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
}

IMPL_ON_FUNC( EGS_AUTH_SECOND_SECURITY_REQ )
{
	//{{ 2011. 06. 23	최육사	2차 비번 예외처리
#ifdef SERV_CHECK_SECOND_PW_STATE
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_CHECK_SECOND_PW, KGSFSM::S_SERVER_SELECT ), EGS_AUTH_SECOND_SECURITY_ACK );
#else
	VERIFY_STATE_ACK( ( 1, KGSFSM::S_SERVER_SELECT ), EGS_AUTH_SECOND_SECURITY_ACK );
#endif SERV_CHECK_SECOND_PW_STATE
	//}}

	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 암호화 하여 DB에 저장
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	kPacket_.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	if( GetSecondPWFailedCount() < 10 && IsSameSecondSecurityPW( kPacket_.m_wstrSecondPW ) == true )
	{
		KDBE_SEUCCESS_SECOND_SECURITY_REQ kReq;
		CTime tCurr = CTime::GetCurrentTime();
		SetLastAuthDate( tCurr );

		kReq.m_iUserID = GetUID();
		kReq.m_wstrCurrTime = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

		START_LOG( clog, L"2차 보안 인증 성공!!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
		SendToAccountDB( DBE_SEUCCESS_SECOND_SECURITY_REQ, kReq );
	}
	else
	{
		START_LOG( clog, L"2차 보안 인증 실패!!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( m_iFailedCount )
			<< END_LOG;
		SendToAccountDB( DBE_FAILED_SECOND_SECURITY_REQ, GetUID() );
	}
}

IMPL_ON_FUNC( DBE_SEUCCESS_SECOND_SECURITY_ACK )
{
	//{{ 2011. 06. 23	최육사	2차 비번 예외처리
#ifdef SERV_CHECK_SECOND_PW_STATE
	VERIFY_STATE_ACK( ( 2, KGSFSM::S_CHECK_SECOND_PW, KGSFSM::S_SERVER_SELECT ), EGS_AUTH_SECOND_SECURITY_ACK );
#endif SERV_CHECK_SECOND_PW_STATE
	//}}

	if( kPacket_.m_iOK == NetError::NET_OK )	// 인증 성공 기록이 완료 되었다
	{
		//{{ 2011. 06. 23	최육사	2차 비번 예외처리
#ifdef SERV_CHECK_SECOND_PW_STATE
		StateTransition( KGSFSM::I_TO_SERVER_SELECT );
#endif SERV_CHECK_SECOND_PW_STATE
		//}}

		// 인증 성공을 하게 되면 값들은 초기화 된다.
		SetSuccessSecondSecurity( true );
		SetSecondPWFailedCount( 0 );

		KEGS_AUTH_SECOND_SECURITY_ACK kAck;

		kAck.m_iOK = NetError::NET_OK;
		kAck.m_iFailedCount = 0;

		SendPacket( EGS_AUTH_SECOND_SECURITY_ACK, kAck );

#ifdef SERV_SECOND_SECURITY_REWARD_ID
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		if( ERI_SECOND_SECURITY == 0 )
#else
		if( KRewardTable::ERI_SECOND_SECURITY == 0 )
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
		return;
#endif // SERV_SECOND_SECURITY_REWARD_ID

		// 오늘 6 이후 최초 접속이면
		CTime tLastLogin;
		CTime tNow = CTime::GetCurrentTime();
		CTime tRestart = CTime( tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 6, 0, 0 );		// 초기화 기점
		CTimeSpan tOneDay = CTimeSpan( 1, 0, 0, 0 );
		CTime tYesterRestart = tRestart - tOneDay;		// 이전 초기화 기점

		if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrCurrTime, tLastLogin ) == false )
		{
			START_LOG( cerr, L"시간을 문자열로 변환 실패!!!" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( kAck.m_iOK )
				<< BUILD_LOG( kPacket_.m_wstrCurrTime )
				<< END_LOG;
			return;
		}
		
		if( tLastLogin >= tYesterRestart )	// 어제 받았다
		{
			if( tLastLogin < tRestart )	// 오늘 받을 조건이 된다
			{
				if( tRestart <= tNow )
				{
					//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					m_kUserUnitSelectRewardManager.AddRewardItemInfo( ERI_SECOND_SECURITY );
#else
					m_kUserUnitSelectRewardManager.AddRewardItemInfo( KRewardTable::ERI_SECOND_SECURITY );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
					//}}
				}
			}
		}
		else
		{
			//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
			m_kUserUnitSelectRewardManager.AddRewardItemInfo( ERI_SECOND_SECURITY );
#else
			m_kUserUnitSelectRewardManager.AddRewardItemInfo( KRewardTable::ERI_SECOND_SECURITY );
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
			//}}
		}
	}
}

_IMPL_ON_FUNC( DBE_FAILED_SECOND_SECURITY_ACK, int )
{
	// 인증 실패 기록이 완료 되었다  // 실패 카운트가 담겨 온다
	KEGS_AUTH_SECOND_SECURITY_ACK kAck;

	SetSuccessSecondSecurity( false );
	SetSecondPWFailedCount( kPacket_ );
	kAck.m_iFailedCount = kPacket_;

	if( kPacket_ < 10 )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED;
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT;
	}

	SendPacket( EGS_AUTH_SECOND_SECURITY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_CREATE_SECOND_SECURITY_REQ )
{
	KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

#ifdef SERV_DISABLE_SECOND_SECURITY
	START_LOG( clog, L"2차 보안패드를 막았는데 클라해킹으로 들어왔나 봄" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrSecondPW )
		<< END_LOG;

	SendPacket( EGS_CREATE_SECOND_SECURITY_ACK, kAck );
	return;
#endif SERV_DISABLE_SECOND_SECURITY

	// 문자열 무결성 검사( 숫자로만, 4~6자리 인가? )
	if( CheckIntegritySecondSecurityPW( kPacket_.m_wstrSecondPW, kAck.m_iOK ) == false )
	{
		START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

		SendPacket( EGS_CREATE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	KDBE_CREATE_SECOND_SECURITY_REQ kReq;
	kReq.m_iUserID = GetUID();
	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 암호화 하여 DB에 저장
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strPW )
		<< END_LOG;
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strChangePW )
		<< END_LOG;
	kReq.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kReq.m_wstrSecondPW )
		<< END_LOG;
#else
	kReq.m_wstrSecondPW = kPacket_.m_wstrSecondPW;
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	SendToAccountDB( DBE_CREATE_SECOND_SECURITY_REQ, kReq );
}

IMPL_ON_FUNC( DBE_CREATE_SECOND_SECURITY_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetIsUseSecondPW( true );
		SetSecondPWFailedCount( 0 );
		SetSecondPW( kPacket_.m_wstrSecondPW );
		SetSuccessSecondSecurity( true );

		START_LOG( clog, L"2차 보안 생성 성공!!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;
	}

	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 알려주면 안되니깐 지우자
	kPacket_.m_wstrSecondPW = L"";
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	SendPacket( EGS_CREATE_SECOND_SECURITY_ACK, kPacket_ );
}

IMPL_ON_FUNC( EGS_DELETE_SECOND_SECURITY_REQ )
{
	KEGS_DELETE_SECOND_SECURITY_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	if( kPacket_.m_wstrSecondPW.empty() == true )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DEFECT_PW;
		SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	// 현재 비밀번호와 비교
	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 암호화
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strPW )
		<< END_LOG;
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strChangePW )
		<< END_LOG;
	kPacket_.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrSecondPW )
		<< END_LOG;
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	if( IsSameSecondSecurityPW( kPacket_.m_wstrSecondPW ) == false )
	{
		START_LOG( clog, L"2차 보안 비밀번호의 값이 틀렸습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
	}

	SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_DELETE_SECOND_SECURITY_VERIFY_REQ )
{
	KEGS_DELETE_SECOND_SECURITY_VERIFY_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	if( kPacket_.m_wstrSecondPW.empty() == true )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DEFECT_PW;
		SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	// 현재 비밀번호와 비교
	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 암호화
	std::string strPW = KncUtil::toNarrowString( kPacket_.m_wstrSecondPW );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strPW )
		<< END_LOG;
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( strChangePW )
		<< END_LOG;
	kPacket_.m_wstrSecondPW = KncUtil::toWideString( strChangePW.c_str() );
	START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrSecondPW )
		<< END_LOG;
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	if( IsSameSecondSecurityPW( kPacket_.m_wstrSecondPW ) == false )
	{
		START_LOG( clog, L"2차 보안 비밀번호의 값이 틀렸습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
		SendPacket( EGS_DELETE_SECOND_SECURITY_ACK, kAck );
		return;
	}

	if( kPacket_.m_bDelete )
	{
		KDBE_DELETE_SECOND_SECURITY_REQ kReq;
		kReq.m_iUserID = GetUID();
		GetSecondPW( kReq.m_wstrSecondPW );

		SendToAccountDB( DBE_DELETE_SECOND_SECURITY_REQ, kReq );
		return;
	}

	kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DELETE_FAILE;
	SendPacket( EGS_DELETE_SECOND_SECURITY_VERIFY_ACK, kAck );
}

IMPL_ON_FUNC( DBE_DELETE_SECOND_SECURITY_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetIsUseSecondPW( false );
		SetSecondPWFailedCount( 0 );
		SetSuccessSecondSecurity( false );

		START_LOG( clog, L"2차 보안 해지 성공!!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
	}

	KEGS_DELETE_SECOND_SECURITY_VERIFY_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;
	SendPacket( EGS_DELETE_SECOND_SECURITY_VERIFY_ACK, kAck );
}

IMPL_ON_FUNC( EGS_CHANGE_SECOND_SECURITY_PW_REQ )
{
	KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	// 문자열 무결성 검사( 숫자로만, 4~6자리 인가? )
	if( CheckIntegritySecondSecurityPW( kPacket_.m_wstrNewSecondPW, kAck.m_iOK ) == false || 
		CheckIntegritySecondSecurityPW( kPacket_.m_wstrOldSecondPW, kAck.m_iOK ) == false )
	{
		START_LOG( clog, L"2차 보안 비밀번호의 값이 이상합니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW )
			<< END_LOG;

		SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
		return;
	}

	// 현재 비밀번호와 비교
	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 암호화
	std::string strOldPW = KncUtil::toNarrowString( kPacket_.m_wstrOldSecondPW );
	std::string strChangeOldPW = KServerSNMaker::GetCRCFromString( strOldPW );
	kPacket_.m_wstrOldSecondPW = KncUtil::toWideString( strChangeOldPW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	if( IsSameSecondSecurityPW( kPacket_.m_wstrOldSecondPW ) == false )
	{
		START_LOG( clog, L"2차 보안 비밀번호의 값이 틀렸습니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW )
			<< END_LOG;

		KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;

		SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
		return;
	}

	// 현재 비밀번호와 동일한 비번으로 셋팅 하려 할 때
	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 암호화
	std::string strNewPW = KncUtil::toNarrowString( kPacket_.m_wstrNewSecondPW );
	std::string strChangeNewPW = KServerSNMaker::GetCRCFromString( strNewPW );
	kPacket_.m_wstrNewSecondPW = KncUtil::toWideString( strChangeNewPW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}
	if( IsSameSecondSecurityPW( kPacket_.m_wstrNewSecondPW ) == true )
	{
		START_LOG( clog, L"2차 보안의 새비밀번호가 이전 비밀번호와 동일합니다." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW )
			<< END_LOG;

		KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DEFECT_PW_3;

		SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
		return;
	}

	KDBE_CHANGE_SECOND_SECURITY_PW_REQ kReq;
	kReq.m_iUserID = GetUID();
	kReq.m_wstrOldSecondPW = kPacket_.m_wstrOldSecondPW;
	kReq.m_wstrNewSecondPW = kPacket_.m_wstrNewSecondPW;

	SendToAccountDB( DBE_CHANGE_SECOND_SECURITY_PW_REQ, kReq );
}

IMPL_ON_FUNC( DBE_CHANGE_SECOND_SECURITY_PW_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SetSecondPW( kPacket_.m_wstrSecondPW );
		SetSecondPWFailedCount( 0 );

		START_LOG( clog, L"2차 보안 비밀번호 변경 성공!!" )
			<< BUILD_LOG( GetUID() )
			<< END_LOG;
	}

	KEGS_CHANGE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = kPacket_.m_iOK;

	SendPacket( EGS_CHANGE_SECOND_SECURITY_PW_ACK, kAck );
}

IMPL_ON_FUNC( EGS_COMPARE_SECOND_SECURITY_PW_REQ )
{
	KEGS_COMPARE_SECOND_SECURITY_PW_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;

	//{{  2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
#ifdef SERV_SECOND_SECURITY_PW_ENCRYPTION
	// 암호화
	std::string strPW = KncUtil::toNarrowString( kPacket_ );
	std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
	kPacket_ = KncUtil::toWideString( strChangePW.c_str() );
#endif SERV_SECOND_SECURITY_PW_ENCRYPTION
	//}}

	// 2차 보안 패드 사용 중이고 비밀번호가 동일 할 때
	if( GetIsUseSecondPW() == false )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_NO_USE;
	}
	else if( IsSameSecondSecurityPW( kPacket_ ) == false )
	{
		kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
	}

	SendPacket( EGS_COMPARE_SECOND_SECURITY_PW_ACK, kAck );
}

IMPL_ON_FUNC( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_REQ )
{
    KEGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK kAck;
    kAck.m_iOK = NetError::NET_OK;

    // 2011.12.20     김민성    2차 보안 패드 비밀번호 암호화
    std::string strPW = KncUtil::toNarrowString( kPacket_ );
    std::string strChangePW = KServerSNMaker::GetCRCFromString( strPW );
    kPacket_ = KncUtil::toWideString( strChangePW.c_str() );

    // 2차 보안 패드 사용 중이고 비밀번호가 동일 할 때 통과
    if( GetIsUseSecondPW() == false )
    {
        kAck.m_iOK = NetError::ERR_SECOND_SECURITY_NO_USE;
    }
    else if( IsSameSecondSecurityPW( kPacket_ ) == false )
    {
        kAck.m_iOK = NetError::ERR_SECOND_SECURITY_DIFFER_PW;
    }
    else 
    {
#ifdef SERV_CHECK_SECOND_PW_STATE
        StateTransition( KGSFSM::I_TO_SERVER_SELECT );
#endif SERV_CHECK_SECOND_PW_STATE
    }

    SendPacket( EGS_CHECK_SECOND_SECURITY_PW_CHRACTER_LIST_ACK, kAck );
}

#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
IMPL_ON_FUNC( EGS_CHECK_MACHINE_ID_REQ )
{
	// 상태 제한은 없습니다!

	// 차단 기능 동작 여부
	if( SiKGameSysVal()->IsMachineIDBlock() == false )
	{
		KEGS_CHECK_MACHINE_ID_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_strMachineID = kPacket_.m_strMachineID;
        SendPacket( EGS_CHECK_MACHINE_ID_ACK, kPacketAck );
		return;
	}

	// 문자열 길이 체크!
	if( kPacket_.m_strMachineID.empty()  ||  kPacket_.m_strMachineID.size() > 24 )
	{
		START_LOG( cerr, L"머신ID값이 이상합니다! 접속 차단!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< END_LOG;

		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#else
		//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}

		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}

	// 쿼리에 직접 들어갈 문자열이므로 injection 대비 검사를 한다.
	std::wstring wstrMachineID = KncUtil::toWideString( kPacket_.m_strMachineID );
	if( KODBC::IsInvalidMarkInForMachineID( wstrMachineID ) == true )
	{
		START_LOG( cerr, L"머신ID값이 이상합니다! 접속 차단!" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< END_LOG;

		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#else
		//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}

		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}

	//{{ 2011. 09. 07	최육사	머신 ID 기능 업데이트
#ifdef SERV_MACHINE_ID_BLOCK_UPDATE
	if( m_kUserSecurityManager.CheckValidMachineID( kPacket_.m_strMachineID ) == false )
	{
		START_LOG( cout, L"[알림] 머신ID가 유효하지 않다! 접속 차단 시키자!" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_strMachineID );

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
		
		// 머신ID가 비어있으면 접속 차단!
		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}
#endif SERV_MACHINE_ID_BLOCK_UPDATE
	//}}

	SendToLoginServer( ELG_CHECK_MACHINE_ID_REQ, kPacket_ );
}

_IMPL_ON_FUNC( ELG_CHECK_MACHINE_ID_ACK, KEGS_CHECK_MACHINE_ID_ACK )
{
	// 상태 제한은 없습니다!

	// 일단 저장해두자!
	m_kUserSecurityManager.SetMachineID( kPacket_.m_strMachineID );

	// 인증 실패했다면 접속 차단!
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_BLOCK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#else
		//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_BLOCK_MACHINE_ID;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
		//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}

		Kick( NetError::ERR_CHECK_MACHINE_ID_00 );
		return;
	}

	SendPacket( EGS_CHECK_MACHINE_ID_ACK, kPacket_ );
}
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
IMPL_ON_FUNC( ELG_CHECK_MODULE_INFO_UPDATE_NOT )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 클라이언트에 메일을 보내자
		//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );

		// 해킹 의심유저로 등록한다.
		HackUserRegMonitorsUser( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL );
#else
		KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
		kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_DLL;
		SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );

		// 해킹 의심유저로 등록한다.
		HackUserRegMonitorsUser( KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_DLL );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
		//}}
	
		// 로그 기록
		bool bWrite = false;
		KDBE_CHECK_MODULE_INFO_LOG_NOT kLog;
		
		std::map< std::wstring, KModuleInfo >::iterator mit = kPacket_.m_mapModuleName.begin();
		for(  ; mit != kPacket_.m_mapModuleName.end() ; ++mit )
		{
			if( mit->second.m_iAllow == KModuleInfo::MIA_MONITORING )
			{
				kLog.m_vecMonitoringDLL.push_back( mit->second );
			}
			else if( mit->second.m_iAllow == KModuleInfo::MIA_BLACK_LIST )
			{
				kLog.m_vecBlackListDLL.push_back( mit->second );
			}	
		}

		if( kLog.m_vecMonitoringDLL.empty() == false )  // 모니터링이 우선순위이다.
		{
			// TradeBlock에 등록하기!
			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			HackUserRegTradeBlock( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL );

			m_kUserHackingManager.SetHackingUserSendType( KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_DLL );
#else
			HackUserRegTradeBlock( KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_DLL );
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}

			bWrite = true;

			START_LOG( clog, L"DLL 체크에서 걸렸구나~거래 막자" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KModuleInfo::MIA_MONITORING )
				<< END_LOG;
		}
		else if( kLog.m_vecBlackListDLL.empty() == false )
		{
			//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			KEGS_SERVER_CHECK_HACK_USER_REQ kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_CHECK_CLIENT_BLOCK_DLL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_REQ, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
#else
			//{{ 2012. 03. 20	김민성		Game Server 에서 종료 사유별 해킹 정보 체크
#ifdef SERV_CHECK_KICK_USER_IN_GAMESERVER
			KEGS_SERVER_CHECK_HACK_USER_NOT kPacket;
			kPacket.m_cHackingCheckType = KEGS_SERVER_CHECK_HACK_USER_NOT::HCT_CHECK_CLIENT_BLOCK_DLL;
			SendPacket( EGS_SERVER_CHECK_HACK_USER_NOT, kPacket );
#endif SERV_CHECK_KICK_USER_IN_GAMESERVER
			//}}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
			//}}
			
			SetDisconnectReason( KStatistics::eSIColDR_Client_Hacking );
			ReserveDestroy();

			bWrite = true;

			START_LOG( clog, L"DLL 체크에서 걸렸구나~킥 하자" )
				<< BUILD_LOG( GetUID() )
				<< BUILD_LOG( KModuleInfo::MIA_BLACK_LIST )
				<< END_LOG;
		}

		if( bWrite == true )
		{
			// 로그 기록하자
			kLog.m_iUserUID = GetUID();
			m_kUserSecurityManager.GetServerSN( kLog.m_iServerSN );

			SendToLogDB( DBE_CHECK_MODULE_INFO_LOG_NOT, kLog );

			START_LOG( clog, L"로그 기록하러 가자" )
				<< BUILD_LOG( kLog.m_iUserUID )
				<< BUILD_LOG( kLog.m_iServerSN )
				<< END_LOG;
		}
	}
}
#endif SERV_HACKING_DLL_CHECK
//}}

//{{ 2012. 06. 04	김민성       서버 해킹 유저 체크 요청 응답으로 변경
#ifdef SERV_SERVER_CHECK_HACK_USER_REQ_ACK
IMPL_ON_FUNC( EGS_SERVER_CHECK_HACK_USER_ACK )
{
	char cLastSendType = KEGS_SERVER_CHECK_HACK_USER_REQ::HCT_NONE;
	if( m_kUserHackingManager.SetHackingUserReciveType( kPacket_.m_cHackingCheckType, cLastSendType ) == false )
	{
		START_LOG( cerr, L"해킹 유저 패킷을 보냈는데 잘못된 응답이다." )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOGc( cLastSendType )
			<< BUILD_LOGc( kPacket_.m_cHackingCheckType )
			<<END_LOG;

		// 클라에게 패킷을 쏘자
		SendID( EGS_CHECK_HACK_USER_PACKET_REQUEST_NOT );

		// 킥하자
		if( SiKGameSysVal()->GetHackingUserKick() == true )
		{
			SetDisconnectReason( KStatistics::eSIColDR_Client_Hacking );
			ReserveDestroy();
		}
	}
}
#endif SERV_SERVER_CHECK_HACK_USER_REQ_ACK
//}}

#ifdef SERV_NPROTECT_CS_AUTH_30
IMPL_ON_FUNC( EGS_NPROTECT_CSAUTH3_RESPONE_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	static const size_t MaxBufferSize = 4096;

	size_t ReceivedSize = min(kPacket_.m_deqCSAuth3.size(), MaxBufferSize);
	size_t index = 0;
	BOOST_FOREACH(BYTE b, kPacket_.m_deqCSAuth3)
	{
		m_CSAuth3.packet[index] = b;
		++index;
		if(index >= ReceivedSize)
			break;
	}

	UINT32 uReturn = m_CSAuth3.Check( ReceivedSize );

	if( uReturn >= 3000 )
	{
		START_LOG( cerr, L"nProtect CS 인증3 응답 실패" )
			<< BUILD_LOG( GetName() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( uReturn )
			<< END_LOG;
		SetDisconnectReason( KStatistics::eSIColDR_nProtect_Error );
		ReserveDestroy();

		return;
	}
}

IMPL_ON_FUNC( EGS_NPROTECT_CSAUTH3_GAMEHACK_REPORT_NOT )
{
	VERIFY_STATE( ( 2, KGSFSM::S_FIELD_MAP, KGSFSM::S_ROOM ) );

	// 게임핵이 발견되었지만 정상 동작합니다. 
	// 게임클라이언트는 전달받은 데이터를 서버에 전송하고 정상동작 합니다.

	const size_t MaxBufferSize = 4096;

	// Hack report 가 클라이언트에서 전달된 경우
	size_t ReceivedSize = max(kPacket_.m_deqCSAuth3.size(), MaxBufferSize);
	size_t index = 0;
	BOOST_FOREACH(BYTE b, kPacket_.m_deqCSAuth3)
	{
		m_CSAuth3.packet[index] = b;
		++index;
		if(index >= ReceivedSize)
			break;
	}

	// TODO: 테섭에서 유저키를 바꿀수 있도록 컨피그로 빼내기

	// 해외팀 각국가 전용 디파인
#if defined( SERV_COUNTRY_JP )
	UINT32 uReturn = DecryptHackData( "ElswordJP", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_CN )
	UINT32 uReturn = DecryptHackData( "ElswordCN", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_TH )
	UINT32 uReturn = DecryptHackData( "ElswordTH", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_ID )
	UINT32 uReturn = DecryptHackData( "ElswordID", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_BR )
	UINT32 uReturn = DecryptHackData( "ElswordBR", m_CSAuth3.packet, ReceivedSize );
#elif defined( SERV_COUNTRY_PH )
	UINT32 uReturn = DecryptHackData( "ElswordPH", m_CSAuth3.packet, ReceivedSize );
#endif

	if( uReturn == -1 )
	{
		// 실패	
		START_LOG( cerr, L"nProtect 게임핵 리포트 패킷 복호화 실패." )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( GetName() )
			<< END_LOG;
	}

	START_LOG( cout, L"nProtect 게임핵 리포트." )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() )
		<< BUILD_LOG( uReturn )
		<< END_LOG;
}

#endif SERV_NPROTECT_CS_AUTH_30

//-- 지헌 : XTRAP 서버 - 클라 응답 패킷 처리부분(Step3)
#ifdef SERV_USE_XTRAP
IMPL_ON_FUNC( EGS_XTRAP_ACK )
{
	if(m_cXTrapCSM.CSMStep3( &kPacket_ ) == false)
	{
		// 스텝3에선 끊지 않는게 정상입니다.
		//SetDisconnectReason( KStatistics::eSIColDR_nProtect_Hacking );
		//ReserveDestroy();

		START_LOG( cerr, L"XTRAP 해킹 클라이언트  UserUID : " << GetUID() );
		return;
	}
}
#endif SERV_USE_XTRAP

#ifdef SERV_FIX_NONE_NPC_DUNGEON_LINES// 작업날짜: 2013-05-22	// 박세훈
IMPL_ON_FUNC_NOPARAM( ERM_DUNGEON_KILLALLNPC_CHECK_NOT )
{
	SendID( EGS_DUNGEON_KILLALLNPC_CHECK_NOT );
}
#endif // SERV_FIX_NONE_NPC_DUNGEON_LINES

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
IMPL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_INFO_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"해킹 유저 체크 카운트를 초기화 할 수 없습니다." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< BUILD_LOG( kPacket_.m_byteType )
			<< BUILD_LOG( kPacket_.m_wstrReason )
			<< END_LOG;
		return;
	}
	
	m_kUserAbuserManager.InitCheckCount( kPacket_.m_byteType, kPacket_.m_byteBlockCheckCount, kPacket_.m_wstrLastResetDate );
	IncreaseCheckCount( kPacket_.m_byteType, kPacket_.m_wstrReason, true );
}
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef SERV_HACKING_USER_PROCESS_NAME_RECORD// 작업날짜: 2013-06-03	// 박세훈
IMPL_ON_FUNC( EGS_HACKING_USER_PROCESS_NAME_RECORD_NOT )
{
	switch( kPacket_.m_byteType )
	{
	case SEnum::HUCT_CLIENT_HACKING_PROCESS:
	case SEnum::HUCT_CLIENT_HACKING_TEXT:
	case SEnum::HUCT_CLIENT_HACKING_MODULE:
	case SEnum::HUCT_CLIENT_HACKING_DLL_MANAGER:
	case SEnum::HUCT_CLIENT_HACKING_ZOMBIE_PROCESS:
	case SEnum::HUCT_CLIENT_HACKING_THREAD_TITLE:
		IncreaseCheckCount( kPacket_.m_byteType, kPacket_.m_wstrReason );
		break;

	default:
		START_LOG( cerr, L"사용할 수 없는 클라이언트 해킹 타입이 들어왔습니다." )
			<< BUILD_LOG( kPacket_.m_byteType )
			<< BUILD_LOG( GetCharUID() )
			<< BUILD_LOG( GetCharName() )
			<< END_LOG;
	}
}
#endif // SERV_HACKING_USER_PROCESS_NAME_RECORD

#ifdef SERV_ENTRY_POINT

IMPL_ON_FUNC_NOPARAM( EGS_GET_SECOND_SECURITY_INFO_REQ )
{
    KDBE_GET_PURE_SECOND_SECURITY_INFO_REQ kReq;
    kReq.m_iUserUID = GetUID();
    //SiKGameSysVal()->GetComeBackRewardCondition( kReq.m_mapComeBackRewardCondition );
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
    kReq.m_wstrID = GetName();
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT

    SendToAccountDB( DBE_GET_PURE_SECOND_SECURITY_INFO_REQ, kReq );	// AccountDB에 최근 접속 종료 정보 얻기
}

IMPL_ON_FUNC( DBE_GET_PURE_SECOND_SECURITY_INFO_ACK )
{
    KEGS_SECOND_SECURITY_INFO_NOT kNot;
    kNot.m_iOK = kPacket_.m_iOK;
    kNot.m_bUseSecondPW = kPacket_.m_bUseSecondPW;

    if( kPacket_.m_iOK == NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT ) // 비밀 번호 입력 실패가 10회 이상일 때 
    {
        SendPacket( EGS_GET_SECOND_SECURITY_INFO_ACK, kNot );

        // 클라이서 종료 시키기로 하여 주석 됨
        //SetDisconnectReason( KStatistics::eSIColDR_Normal_Disconnect );
        //ReserveDestroy();															// 강제 종료
        return;
    }

    // DB 에서 받아온 데이터 저장 (성공 일 때)
    CTime tLastAuthDate;
    if( kPacket_.m_bUseSecondPW == true )
    {
        if( KncUtil::ConvertStringToCTime( kPacket_.m_wstrLastAuthDate, tLastAuthDate ) == false )
        {
            START_LOG( cerr, L"문자열 시간 변환 실패." )
                << BUILD_LOG( kPacket_.m_wstrLastAuthDate )
                << END_LOG;
        }

        //{{ 2011. 06. 23	최육사	2차 비번 예외처리
#ifdef SERV_CHECK_SECOND_PW_STATE
        //StateTransition( KGSFSM::I_TO_CHECK_SECOND_PW );
#endif SERV_CHECK_SECOND_PW_STATE
        //}}
    }
    SetIsUseSecondPW( kPacket_.m_bUseSecondPW );
    SetSecondPWFailedCount( kPacket_.m_iFailedCount );
    SetSecondPW( kPacket_.m_wstrSecondPW );
    SetLastAuthDate( tLastAuthDate );

    SendPacket( EGS_GET_SECOND_SECURITY_INFO_ACK, kNot );

    if( m_kUserStatistics.IsFirstUserLoginToday( GetUID(), GetLastLoginDate() ) == true )
    {  // 보안 사용 정보 로그 남기자!
        SendFirstLoginSecurityInfo();
    }
}

#endif SERV_ENTRY_POINT

//////////////////////////////////////////////////////////////////////////
#endif SERV_GSUSER_CPP
//////////////////////////////////////////////////////////////////////////


