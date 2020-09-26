#include <WinSock2.h>
#include <boost/bind.hpp>
#include <sstream>
#include <iomanip>
#include "ActorManager.h"
#include "ProxyManager.h"
#include "GameServer.h"
#include "GSSimLayer.h"
#include "GSNetLayer.h"
#include "GSAccountDBThread.h"
#include "GSGameDBThread.h"
#include "GSGameDBThread2nd.h"
#include "GSLogDBThread.h"
#include "GSLogDBThread2nd.h"
#include "GSSMSDBThread.h"
#ifdef SERV_ADD_EVENT_DB
#include "GSEventDBThread.h"
#endif //SERV_ADD_EVENT_DB
#ifdef SERV_ADD_SCRIPT_DB
#include "GSScriptDBThread.h"
#endif //SERV_ADD_SCRIPT_DB

#ifdef SERV_COUNTRY_TH
#include "../Common/OnlyGlobal/AuthAndBilling/TH/GSASBillingDBThread.h"	//	AsiaSoft Billing
#endif // SERV_COUNTRY_TH

#ifdef SERV_COUNTRY_TWHK
#include "../Common/OnlyGlobal/AuthAndBilling/TW/GSGASHBillingThread.h"	//	AsiaSoft Billing
#endif //SERV_COUNTRY_TWHK

#ifdef SERV_COUNTRY_JP
#include "../Common/OnlyGlobal/AuthAndBilling/JP/HanBillingForSvr.h"
//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
#include "GSRelayDBThread.h"
#endif //SERV_RELAY_DB_CONNECTION
//}}
#endif //SERV_COUNTRY_TWHK

#ifdef SERV_GLOBAL_BILLING
#include "GSBillingDBThread.h"
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_COUNTRY_ID
#ifdef SERV_ID_NETMARBLE_PCBANG
#include "../Common/OnlyGlobal/NetCafe/ID/GSIDPcbangDBThread.h"	//	2013.07.01 lygan_조성욱 // 인도네시아 퍼블리셔 PC 방 인증 DB
#endif //SERV_ID_NETMARBLE_PCBANG
#endif //SERV_COUNTRY_ID
//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	#include "GSNXWebDBThread.h"
#endif SERV_WEB_POINT_EVENT
//}}
#include "DBLayer.h"
#include "socket/NetCommon.h"
#include "KncSend.h"
#include "UserManager.h"
#include "GSUser.h"
#include "GSFSM.h"
#include "NProtectFunc.h"
#include "HackShield/HSAuth.h"

#include "NetError.h"
#include "RoomListManager.h"
//{{ 2009. 7. 16  최육사	채널매니저
#include "ChannelManager.h"
//}}

#include "X2Data/XSLRandomItemManager.h"
#include "X2Data/XSLManufactureItemManager.h"
#include "X2Data/XSLCashItemManager.h"
#include "X2Data/XSLTitleManager.h"
#include "X2Data/XSLQuestManager.h"
#include "AbuserLog/KAbuserLogManager.h"
#include "RewardTable.h"
#include "X2Data/XSLGuildManager.h"
#include "GameSysVal/GameSysVal.h"
#include "X2Data/XSLResolveItemManager.h"
#include "X2Data/XSLEnchantItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLAttribEnchantItem.h"
#include "RecommendUserTable.h"
#include "ExpTable.h"
//{{ 2010. 9. 11	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	#include "X2Data/XSLPetManager.h"
#endif SERV_PET_SYSTEM
//}}
//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "BattleFieldListManager.h"
	#include "GSAutoPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2012. 10. 19	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
	#include "X2Data/XSLPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

#include "GameEvent/GameEventManager.h"

//081021.hoons.
#include "FieldManager.h"
//081106.hoons.
#include "TRServer.h"

//{{ 2008. 11. 24  최육사	랭킹 매니저
#include "GSRankingManager.h"
//}}

//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 4. 22  최육사	대전유저리스트
#include "PVPLobbyManager.h"
//}}

//{{ 2009. 11. 21  최육사	서버간접속구조개선
#include "NexonBillingTCPManager.h"
//}}

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	#include "HackingProtectionManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}
//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
	#include "GSScriptRefreshManager.h"
#endif SERV_REALTIME_SCRIPT
//}}

#ifdef DEF_TRADE_BOARD
	#include "PersonalShopItemList.h"
#endif DEF_TRADE_BOARD

#ifdef SERV_KOG_STATISTICS
	#include "KOGAgent.h"
#endif SERV_KOG_STATISTICS

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_SCRIPT_REFRESH
//}}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	#include "GameEvent/GameEventDBManager.h"
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

//{{ 2011. 03. 29	최육사	TBB 메모리 관리자
#ifdef SERV_TBB_MALLOC_PROXY_TEST
	#include <tbb/tbbmalloc_proxy.h>
#endif SERV_TBB_MALLOC_PROXY_TEST
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	#include "GSWorldMissionManager.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	#include "GSContentManager.h"
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	#include "CompanyIPTable.h"
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
//}}
//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	#include "ClassChangeTable.h"
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	#include "X2Data\XSLStringFilter.h"
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}

//{{ 2012. 05. 09	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	#include "UdpRelayChecker/UdpRelayChecker.h"
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/ReleaseTickManager.h"
	#include "EventDataRefresh/EventDataRefreshManager.h"
	#include "EventDataRefresh/EventMonsterRefreshManager.h"
	#include "EventDataRefresh/EventNotifyMsgServerInfo.h"
	#include "EventDataRefresh/EventRewardDataRefreshManager.h"
	#include "EventDataRefresh/EventRewardLevelUpRefreshManager.h"
	#include "EventDataRefresh/EventRewardRetainUserItemRefreshManager.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	#include "..\Common\GameEvent\DailyGiftBoxManager.h"
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}
//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	#include "CorruptionMonitor.h"
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}
//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	#include "Temp.h"
#endif SERV_EVENT_BINGO
//}}
//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_CHANNELING_USER_MANAGER
#include "ChannelingUserManager.h"
#endif SERV_CHANNELING_USER_MANAGER

//{{ 2010. 11. 12  조효진	GameForge 채팅 로그 관련 처리 
#ifdef SERV_RECORD_CHAT
#include "GSChatLogDBThread.h"
#endif SERV_RECORD_CHAT
//}}
#ifdef SERV_SYNTHESIS_AVATAR
#include "X2Data/XSLSynthesisManager.h"
#endif SERV_SYNTHESIS_AVATAR

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2013. 3. 11	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	#include "GSLocalRankingManager.h"
#endif SERV_LOCAL_RANKING_SYSTEM
//}}
//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
	#include "X2Data/XSLBuffManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	#include "X2Data/XSLRidingPetManager.h"
#endif	// SERV_RIDING_PET_SYSTM

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHallListManager.h"
	#include "GSWeddingHall.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "GSGlobalMissionManager.h"
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 13	임홍락	글로벌 미션 매니저

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-31	// 박세훈
	#include "X2Data/XSLFieldBossData.h"
	#include "GSFieldBossManager.h"
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    #include "X2Data/XSLEDInventoryExpand.h"
#endif //SERV_GOOD_ELSWORD

#ifdef SERV_STRING_FILTER_USING_DB
	#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

//#include "vld.h"

#ifdef SERV_NPROTECT_CS_AUTH_30
void __stdcall CS3LogCallback( int nLogType, char* szLog )
{
	// 로그가 발생 시 callback 함수로 로그를 전달합니다.
	// callback 함수가 등록되어 있지 않다면 호출되지 않습니다.

	switch(nLogType)
	{
	case LOG_UPDATE:
		// 게임가드 프로토콜 버전, 내부버전의업데이트가발생했을때호출됩니다. (호출빈도 낮음)
		START_LOG( cout, L"nProtect Update Log" )
			<< BUILD_LOG( szLog )
			<< END_LOG;
		break;
	case LOG_NORMAL:
		// 초기화상황, 일반적인로그, 에러발생등의로그가발생했을때호출됩니다. (호출빈도 보통)
		START_LOG( cout, L"nProtect Normal Log" )
			<< BUILD_LOG( szLog )
			<< END_LOG;
		break;
	case LOG_DEBUG:
		// 모든 유저의 패킷 정보가 남게 됩니다. (호출빈도,양 매우 높음)
		START_LOG( clog, L"nProtect Debug Log" )
			<< BUILD_LOG( szLog )
			<< END_LOG;
		break;
	default:
		START_LOG( cerr, L"nProtect Unknown Log" )
			<< BUILD_LOG( nLogType )
			<< BUILD_LOG( szLog )
			<< END_LOG;
	}
}

void __stdcall CS3UpdateInfoCallback( int nUpdateType, int nBefore, int nAfter )
{
	switch(nUpdateType)
	{
	case UPDATE_PROTOCOL:
		START_LOG( cout, L"nProtect Protocol Update Notification" )
			<< BUILD_LOG( nBefore )
			<< BUILD_LOG( nAfter )
			<< END_LOG;
		break;
	case UPDATE_GGVERSION:
		START_LOG( cout, L"nProtect GGVersion Update Notification" )
			<< BUILD_LOG( nBefore )
			<< BUILD_LOG( nAfter )
			<< END_LOG;
		break;
	default:
		START_LOG( cerr, L"nProtect Unknown Update Info" )
			<< BUILD_LOG( nUpdateType )
			<< BUILD_LOG( nBefore )
			<< BUILD_LOG( nAfter )
			<< END_LOG;
	}
}
#endif SERV_NPROTECT_CS_AUTH_30

NiImplementRTTI( KGameServer, KBaseServer );
ImplPfID( KGameServer, PI_GS_SERVER );

ImplToStringW( KGameServer )
{
    return START_TOSTRING_PARENTW( KBaseServer );
}

#define CLASS_TYPE      KGameServer

KGameServer::KGameServer(void)
: KBaseServer()
#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	, m_bActiveLagCheck( false )
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-06-29	// 박세훈
	, m_iTimeEnchantEventUID( 0 )
	, m_iNumOfTimeEnchantEvent( 0 )
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
	, m_iExchangeLimitUID( 0 )
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
	, m_bFirstCashshopOnOffCheck( true )
#endif SERV_CONTENT_MANAGER_INT
{
}

KGameServer::~KGameServer(void)
{
}

#include <lua_tinker.h>
//using namespace lua_tinker;

void KGameServer::RegToLua()
{
    KBaseServer::RegToLua();

    //class_<KGameServer>( "KGameServer" )
    //    .inh<KBaseServer>()
    //    .def( "dump", KGameServer::Dump );

    //decl( "Server", this );
	lua_tinker::class_add<KGameServer>( g_pLua, "KGameServer" );
	lua_tinker::class_inh<KGameServer, KBaseServer>( g_pLua );
	lua_tinker::class_def<KGameServer>( g_pLua, "dump",						&KGameServer::Dump );
	lua_tinker::class_def<KGameServer>( g_pLua, "ver",						&KGameServer::ServerVersion );
	lua_tinker::class_def<KGameServer>( g_pLua, "SetHackShieldHSBFilePath",	&KGameServer::SetHackShieldHSBFilePath );
	//{{ 2010. 10. 19	최육사	SMS테스트
#ifdef SERV_SMS_TEST
	lua_tinker::class_def<KGameServer>( g_pLua, "SMSTest",					&KGameServer::SMSTest_LUA );
#endif SERV_SMS_TEST
	//}}

	lua_tinker::decl( g_pLua, "Server", this );

	// TRServer reg lua
	SiKTRServer()->RegToLua();
}

const wchar_t*  g_szConfigFile = L"config_gs";
KBaseServer*    CreateKNC()                             { return KBaseServer::KeepObject<KGameServer>(); }
KThread*        CreateDBThread( int iDBConnectionInfo, const wchar_t* szDSN, bool bDBConnStr )
{
    switch( iDBConnectionInfo )
    {
    case KDBLayer::DC_ACCOUNT:
        return new KGSAccountDBThread( szDSN, bDBConnStr );
    case KDBLayer::DC_GAME:
        return new KGSGameDBThread( szDSN, bDBConnStr );
    case KDBLayer::DC_LOG:
        return new KGSLogDBThread( szDSN, bDBConnStr );
	case KDBLayer::DC_SMS:
		return new KGSSMSDBThread( szDSN, bDBConnStr );
		//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	case KDBLayer::DC_NX_WEB:
		return new KGSNXWebDBThread( szDSN, bDBConnStr );
#endif SERV_WEB_POINT_EVENT
		//}}
    case KDBLayer::DC_GAME_2ND:
        return new KGSGameDBThread2nd( szDSN, bDBConnStr );
    case KDBLayer::DC_LOG_2ND:
        return new KGSLogDBThread2nd( szDSN, bDBConnStr );
		//{{ 2010. 11. 12  조효진	GameForge 채팅 로그 관련 처리
#ifdef SERV_RECORD_CHAT
	case KDBLayer::DC_CHAT_LOG:
		return new KGSChatLogDBThread( szDSN, bDBConnStr );
#endif SERV_RECORD_CHAT
		//}}
#ifdef SERV_GLOBAL_BILLING
	case KDBLayer::DC_KOG_BILLING:
		return new KGSBillingDBThread( szDSN, bDBConnStr );

#if defined (SERV_COUNTRY_TWHK)
	case KDBLayer::DC_PUBLISHER_BILLING:	//	Gamania Elsword Point Billing
		return new KGSGASHBillingThread( szDSN, bDBConnStr );
#elif defined (SERV_COUNTRY_TH)
	case KDBLayer::DC_PUBLISHER_BILLING:	//	AsiaSoft Billing
		return new KGSASBillingDBThread( szDSN, bDBConnStr );
#endif // SERV_COUNTRY_XX

#endif // SERV_GLOBAL_BILLING

#ifdef SERV_ID_NETMARBLE_PCBANG
	case KDBLayer::DC_ID_PCBANG_AUTH:
		return new KGSIDPcbangDBThread( szDSN, bDBConnStr );
#endif //SERV_ID_NETMARBLE_PCBANG

		//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
#ifdef SERV_RELAY_DB_CONNECTION
	case KDBLayer::DC_JP_RELAY_DB:
		return new KGSRelayDBThread( szDSN, bDBConnStr );
#endif SERV_RELAY_DB_CONNECTION
		//}}
#ifdef SERV_ADD_EVENT_DB
	case KDBLayer::DC_EVENT:
		return new KGSEventDBThread( szDSN, bDBConnStr );
#endif //SERV_ADD_EVENT_DB

#ifdef SERV_ADD_SCRIPT_DB
	case KDBLayer::DC_SCRIPT:
		return new KGSScriptDBThread( szDSN, bDBConnStr );
#endif //SERV_ADD_SCRIPT_DB

    default:
        START_LOG( cerr, L"접속하려는 DB 종류가 이상함." )
            << BUILD_LOG( iDBConnectionInfo )
            << END_LOG;
        return new KGSGameDBThread( szDSN, bDBConnStr );
    }
}

bool KGameServer::Init()
{
    KSimLayer::KeepObject<KGSSimLayer>();
    KNetLayer::KeepObject<KGSNetLayer>();
    KActorManager::KeepObject<KUserManager>();
	//081021.hoons.
	KRoomManager::KeepObject<KFieldManager>();
	//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::KeepObject<KUdpRelayChecker>();
#endif SERV_UDP_RELAY_CHECKER
	//}}

    KNetLayer::GetKObj()->SetActorFactory( IActorFactoryPtr( new KActorFactory<KGSUser, KGSFSM, 32*1024> ) );
	//{{ 2011. 06. 08	최육사	KEvent 출처 등록 기능
#ifdef SERV_KEVENT_FROM
	KNetLayer::GetKObj()->SetSessionType( KSession::SST_CLIENT );
#endif SERV_KEVENT_FROM
	//}}	

    _JIF( KBaseServer::Init(), return false );

	// 현재 버전
	START_LOG( cout, GetMainVersion() );

	//081021.hoons. BaseServer::Init() 에 모든 루아파싱루트가 들어있기때문에 파싱후 init을 해야함.
	_JIF( GetKFieldManager()->Init(), return false );

	//////////////////////////////////////////////////////////////////////////
	// TRServer 시작하기 전에 Profiler 싱글톤 객체 생성하기
    //SiKProfiler()->GetInstance();
	//////////////////////////////////////////////////////////////////////////

	//081106.hoons. TCP RELAY START.
	_JIF( SiKTRServer()->Init(), return false );

	dbg::cout << dbg::lbreak;

	//{{ 2009. 11. 21  최육사	서버간접속구조개선
    SiKProxyManager()->ConnectAll();
	//}}
    WriteServerInfoToDB();

#ifdef SERV_NPROTECT_CS_AUTH_30
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CommonFlag::CF_NPGG ) == true )
	{
		UINT32 uReturn = InitCSAuth3("./nProtect/");
		if( uReturn != ERROR_SUCCESS )
		{
			START_LOG( cerr, L"nProtect CS 인증3 초기화 실패" )
				<< BUILD_LOG( uReturn )
				<< END_LOG;
			return false;
		}

		SetCallbackFunction( CALLBACK_LOG, (PVOID)CS3LogCallback );
		SetCallbackFunction( CALLBACK_UPDATE, (PVOID)CS3UpdateInfoCallback );
	}
#endif SERV_NPROTECT_CS_AUTH_30

#ifdef SERV_COUNTRY_JP
	// 2010. 10  김정협 일본 빌링
	int ret = -1;
	if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_ALPHA )
	{
		ret = HanBillingInit("J_ES_PRE");

		if(ret == 0)
		{
			START_LOG( clog, L"알파버전 빌링 초기화 완료" )				
				<< END_LOG;
		}

	}
	else if(KSimLayer::GetKObj()->GetHanInitFlag() == KSimLayer::HanInitFlag::HIF_REAL )
	{
		ret = HanBillingInit("J_ES");

		if(ret == 0)
		{
			START_LOG( clog, L"리얼버전 빌링 초기화 완료" )				
				<< END_LOG;
		}
	}

	if(ret != 0)
	{
		START_LOG( cerr, L"빌링초기화 에러" )
			<< BUILD_LOG( ret )			
			<< END_LOG;
	}
#endif //SERV_COUNTRY_JP

	// nProtect 한국인증 모듈 사용설정
	//if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CommonFlag::CF_NPGG ) == true )
	//{
	//	KNP_Enable( true );

	//	if( KNP_Init() == false )
	//	{
	//		START_LOG( cerr, L"nProtect 초기화 실패.!" )
	//			<< END_LOG;

	//		return false;
	//	}
	//}
	//{{ 2009. 8. 18  최육사	hack shield
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_HSHIELD ) == true )
	{
		KHSAuth::SetAhnHSServerHandle( _AhnHS_CreateServerObject( KHSAuth::GetHSBFilePath().c_str() ) );

		if( KHSAuth::GetAhnHSServerHandle() == ANTICPX_INVALID_HANDLE_VALUE )
		{
			START_LOG( cerr, L"Hack Shield 초기화 실패! HSB파일을 읽지 못하였습니다." )
				<< BUILD_LOG( KHSAuth::GetHSBFilePath() )
				<< END_LOG;

			return false;
		}
	}
	//}}

#ifdef SERV_USE_XTRAP
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_XTRAP ) == true )
	{
		GetKGSSimLayer()->ReloadXTRAP_MAP();
	}
#endif SERV_USE_XTRAP

	SetServerReady( true );

	//{{ 2009. 8. 27  최육사	서버 초기화 완료
	OnServerReadyComplete();
	//}}

#ifdef DEF_TRADE_BOARD
	SiKPersonalShopItemList();	// 초기생성을 위해.
#endif DEF_TRADE_BOARD

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	RequestRandomQuestInfo();
#endif SERV_RANDOM_DAY_QUEST
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	m_bServerRunningProcessCommunicationOnOff = false;
#endif //SERV_PROCESS_COMMUNICATION_KSMS
	
#ifdef SERV_USE_GM_TOOL_INFO
	if( GetServerRollType() == KServerInfo::SRT_UPDATE_CASH_PRODUCT_LIST )
	{
		KDBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT kPacketNOT;
		kPacketNOT.m_CurrentItemTempletNameMap = SiCXSLItemManager()->GetItemNameContainer();
		SendToGameDB( DBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT, kPacketNOT );
	}
#endif //SERV_USE_GM_TOOL_INFO

    return true;
}

void KGameServer::OnServerReadyComplete()
{
	KBaseServer::OnServerReadyComplete();

	//{{ 2009. 8. 27  최육사	어뷰저 리스트
	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKAbuserLogManager()->UpdateAllAbuserList();
#else
	SiKAbuserLogManager()->SendAbuserListReq();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
	//}}

	//{{ 2009. 7. 6  최육사		랭킹 개편
	SiKGSRankingManager()->Init(); // 랭킹 정보 초기화
	//}}

	//{{ 2009. 11. 21  최육사	서버간접속구조개선
	switch( KSimLayer::GetKObj()->GetBillingFlag() )
	{
	case KSimLayer::BF_NEXON_KOREA_TEST:
	case KSimLayer::BF_NEXON_KOREA:
		{
			if( SiKNexonBillingTCPManager()->IsFirstConnectSucc() )
			{
				START_LOG( cout, L"넥슨 빌링 인증 서버 처음 접속 성공!" );
			}
			else
			{
				START_LOG( cerr, L"넥슨 빌링 인증 서버 접속 실패." )
					<< END_LOG;

				// First Connect Success체크 해야만 접속끊겼을때 에러로그 출력함.
				SiKNexonBillingTCPManager()->FirstConnectSucc();
			}
		}
		break;
	}
	//}}

	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKReleaseTickManager()->Init();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	// 2013.07.18 lygan_조성욱 // SiKGameEventManager 보다 먼저 초기화 되야함
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\GameServerRead"));
		SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\GameServerWrite"));
		SiKProcessCommunicationManager()->Begin();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	// 이벤트 정보 얻기
	SiKGameEventManager()->Init();

#ifdef SERV_STRING_FILTER_USING_DB
	SiKStringFilterManager()->Init();
#endif //SERV_STRING_FILTER_USING_DB

	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKEventNotifyMsgServerInfo()->Init();
#else
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	SiKGameEventScriptManager()->Init();
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager()->Init();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
	SiKGSScriptRefreshManager()->Init();
	SiKGSScriptRefreshManager()->RegToLua( g_pLua );
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
	if( SEnum::CheckFlag( GetServerRollType(), KServerInfo::SRT_UPDATE_CASH_PRODUCT_LIST ) == true )
	{
		KELOG_CASH_PRODUCT_LIST_UPDATE_NOT kPacketToLogDB;
		SiKNexonBillingTCPManager()->GetCashProductList( kPacketToLogDB.m_vecCashProductList );
		SendToLogDB( ELOG_CASH_PRODUCT_LIST_UPDATE_NOT, kPacketToLogDB );

		START_LOG( cout, L"--- 통계를 위한 캐쉬 아이템 리스트 업데이트 ---" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacketToLogDB.m_vecCashProductList.size() );
	}
#endif SERV_CASH_ITEM_LIST
	//}}

	//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	SiKCorruptionMonitor()->Init();
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
	
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	SiKGSWorldMissionManager()->Init();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKGSLocalRankingManager()->Begin();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_KOG_STATISTICS
	SiKOGAgent();
#endif SERV_KOG_STATISTICS
	
	// 서버군 이름 표시
	START_LOG( cout2, SEnum::GetServerGroupName( GetServerGroupID() ) )
		<< BUILD_LOG( GetServerGroupID() )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( GetName() );

#ifdef SERV_TBB_MALLOC_PROXY_TEST
	START_LOG( cout, L"----------------------------------------------" );
	START_LOG( cout, L"----------- TBB 메모리 관리자 사용 -----------" );
	START_LOG( cout, L"----------------------------------------------" );
#endif SERV_TBB_MALLOC_PROXY_TEST

	//{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
	KDBE_CHANNEL_LIST_REQ kPacketToDB;
	kPacketToDB.m_iServerGroupID = SiKChannelManager()->GetServerGroupID();
	SendToLogDB( DBE_CHANNEL_LIST_REQ, kPacketToDB );
#endif SERV_CHAR_CONNECT_LOG
	//}}
#ifdef SERV_ENTRY_POINT
    // 서버군이 2개 국가에서만 동작
    if ( GetKGSSimLayer()->GetServerGroupNum() == KGSSimLayer::DOUBLE_SERVER_GROUP )
    {
        KDBE_CHANNEL_LIST_REQ kPacketToDB2nd;
        kPacketToDB2nd.m_iServerGroupID = ( GetServerGroupID() == SEnum::SGI_SOLES ? SEnum::SGI_GAIA : SEnum::SGI_SOLES );
        SendToLogDB2nd( DBE_CHANNEL_LIST_REQ, kPacketToDB2nd );
    }
#endif SERV_ENTRY_POINT

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	InitTimeControlItemReleaseTick();
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
	InitTimeControlCubeInItemMappingReleaseTick();
#endif //SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->Init();
#endif SERV_UDP_RELAY_CHECKER
	//}}
#ifdef SERV_GLOBAL_MISSION_MANAGER
	SiKGSGlobalMissionManager()->Init();
#endif SERV_GLOBAL_MISSION_MANAGER
}

void KGameServer::ShutDown()
{
	//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	SiKGSLocalRankingManager()->ShutDown();
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if (SiKGameSysVal()->GetProcessCommunication() == true)
	{
		SiKProcessCommunicationManager()->ShutDown();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

	//{{ 2009. 8. 18  최육사	hack shield
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_HSHIELD ) == true )
	{
		_AhnHS_CloseServerHandle( KHSAuth::GetAhnHSServerHandle() );
	}
	//}}
#ifdef SERV_NPROTECT_CS_AUTH_30
	if( KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CF_NPGG ) == true )
	{
		CloseCSAuth3();
	}
#endif SERV_NPROTECT_CS_AUTH_30

	//{{ 2009. 9. 2  최육사		비정상종료통계
#ifdef ABNORMAL_REASON
	GetKGSSimLayer()->SendToDBAbnormalReason( true );
#endif ABNORMAL_REASON
	//}}

    KBaseServer::ShutDown();      // 이곳에서 모든 종료가 끝난다. 각각의 Layer들도 모두 소멸됨.

	//korea nProtect 모듈 정상종료
	//KNP_CleanupGameguardAuth();

	GetKFieldManager()->ShutDown();

	//081106.hoons. tcp relay shutdown
	SiKTRServer()->ShutDown();
	KTRServer::ReleaseInstance();

	//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT	
	SiKGSScriptRefreshManager()->EndThread();
	KGSScriptRefreshManager::ReleaseInstance();
#endif SERV_REALTIME_SCRIPT
	//}}

	//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
	KUdpRelayChecker::GetKObj()->ShutDown();
	KUdpRelayChecker::ReleaseKObj();
#endif SERV_UDP_RELAY_CHECKER
	//}}
}

bool KGameServer::DestroyAllSession()
{
    // NetLayer의 Accepter만 닫힌 시점. 아직 모든 Layer들이 살아있다.

    KActorManager* pkActorManager = KActorManager::GetKObj();

    START_LOG( cout, L"현재 유저 : " << pkActorManager->GetCount() << L" 명" );

	int iIndex = KStatistics::eSIColDR_Server_Shutdown;

	pkActorManager->ForEach( boost::bind( &KActor::SetDisconnectReason, _1, iIndex ) );

    pkActorManager->ForEach( boost::bind( &KActor::ReserveDestroy, _1 ) );

    DWORD dwBeginTick = ::GetTickCount();


    // 안정적인 종료 조건 :
    // 1. 모든 유저 객체가 소멸되어야 하고 
    // 2. 모든 db event가 처리 fetch 되어야 한다. (처리중일수 있음)
    bool bSuccess = pkActorManager->GetCount() == 0 && 
        SiKDBLayer()->GetTotalEventNum() == 0;

    while( !bSuccess && ::GetTickCount() - dwBeginTick < m_dwShutDownWait )   // 하지만 일정 시간이 지나면 그냥 통과.
    {
        ::Sleep( 500 );

        bSuccess = pkActorManager->GetCount() == 0 && 
            SiKDBLayer()->GetTotalEventNum() == 0;
    }

    // proxy manager의 종료 처리는 일단 유저 객체의 소멸처리를 끝낸 다음에 별도 진행한다.
    bool bProxyShutdown = SiKProxyManager()->Shutdown( 60.0f );

    LOG_CONDITION( bSuccess && bProxyShutdown, cout, cerr )
        << BUILD_LOG( m_dwShutDownWait )
        << BUILD_LOG( bSuccess )
        << BUILD_LOG( bProxyShutdown )
        << BUILD_LOG( pkActorManager->GetCount() )
        << BUILD_LOG( SiKDBLayer()->GetTotalEventNum() )
        << BUILD_LOG( ::GetTickCount() - dwBeginTick );

    return bSuccess && bProxyShutdown;
}

void KGameServer::Tick()
{
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-31	// 박세훈
	CTime tCurrentTime = CTime::GetCurrentTime();
#endif // SERV_BATTLE_FIELD_BOSS

	KBaseServer::Tick();

	//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW
	if( ::GetTickCount() - m_dwServerInfoUpdateTick > 20 * 1000 )
	{
		if( GetServerReady() )
		{
			WriteServerInfoToDB();
		}
	}
#else
	if( ::GetTickCount() - m_dwServerInfoUpdateTick > ms_dwServerInfoUpdateGap )
	{
		if( GetServerReady() )
		{
			WriteServerInfoToDB();
		}
	}
#endif SERV_CCU_NEW
	//}}    

	//nProtect 암호화 모듈 업데이트
	//KNP_Tick();

	// TRServer tick
	SiKTRServer()->Tick();

//#ifdef SERV_KOG_STATISTICS
//	SiKOGAgent()->ConcurrentUser(GetServerGroupID(), SiKChannelManager()->GetChannelID(), KActorManager::GetKObj()->GetCount());
//
//	if( true == SiKOGAgent()->CheckDailyTime() &&	// 보낼시간이고
//		false == SiKOGAgent()->CheckDaily(CTime::GetCurrentTime()) && // 아직 안보낸 날이며
//		SiKChannelManager()->GetChannelID() == 1 // 1번 채널일 경우
//		)
//	{
//		// 보낸날짜 저장
//		SiKOGAgent()->SetDailyTime(CTime::GetCurrentTime());
//
//		//하루통계를 DB에 요청하자
//		SendToLogDB(DBE_CCU_AGENT_REQ, char());
//	}
//#endif SERV_KOG_STATISTICS
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	TickCheckTimeControlItem();
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-31	// 박세훈
	CheckGSFieldBossSystemRequest( tCurrentTime );
	CheckGSFieldBossSystem( tCurrentTime );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING  
	TickCheckTimeControlCubeInItemMapping();
#endif //SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	if ( m_tTimeProcessCommunicationONOFF.elapsed() > 60.0f )
	{
		if (SiKGameSysVal()->GetProcessCommunication() == true && m_bServerRunningProcessCommunicationOnOff == true)
		{
			SiKProcessCommunicationManager()->InitRead(TEXT("\\\\.\\pipe\\GameServerRead"));
			SiKProcessCommunicationManager()->InitWrite(TEXT("\\\\.\\pipe\\GameServerWrite"));
			SiKProcessCommunicationManager()->Begin();
			m_bServerRunningProcessCommunicationOnOff = false;


		}
		else if (SiKGameSysVal()->GetProcessCommunication() == false && m_bServerRunningProcessCommunicationOnOff == false)
		{
			SiKProcessCommunicationManager()->ShutDown();

			m_bServerRunningProcessCommunicationOnOff = true;

		}
		m_tTimeProcessCommunicationONOFF.restart();
	}
#endif //SERV_PROCESS_COMMUNICATION_KSMS

#ifdef SERV_CONTENT_MANAGER_INT
	if( m_tCashshopOnOffCheckTimer.elapsed() > 120.0f )
	{
		KDBE_GET_CASHSHOP_ON_OFF_INFO_REQ kPacket;
		kPacket.m_bFirstCashshopOnOffCheck = m_bFirstCashshopOnOffCheck;
		kPacket.m_iReleaseTick = SiKGSContentManager()->GetReleaseTick();		
		kPacket.m_bEnableCashshop = SiKGSContentManager()->IsEnableCashShop();
		SendToKOGBillingDB( DBE_GET_CASHSHOP_ON_OFF_INFO_REQ, kPacket );

		m_tCashshopOnOffCheckTimer.restart();
		m_bFirstCashshopOnOffCheck = false;
	}
#endif SERV_CONTENT_MANAGER_INT
}

void KGameServer::SetHackShieldHSBFilePath( const char* pFilePath )
{
	KHSAuth::SetHSBFilePath( std::string( pFilePath ) );

	START_LOG( cout, L"Hack Shield HSB File Path" )
		<< BUILD_LOG( pFilePath );
}

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
void KGameServer::SetPCBangIPList( const std::map< std::wstring, int >& mapPCBangIPList )
{
	m_mapPCBangIPList = mapPCBangIPList;

	START_LOG( cout, L"PC Bang IP List size" )
		<< BUILD_LOG( mapPCBangIPList.size() );
	return;
}

int KGameServer::IsPCBangIP( const std::wstring& wstrIP )
{
	START_LOG( clog2, L"PC Bang IP Check" )
		<< BUILD_LOG( wstrIP );

	std::map<std::wstring, int>::const_iterator sit = m_mapPCBangIPList.find(wstrIP);
	if(sit != m_mapPCBangIPList.end())
	{
		return sit->second;
	}
	else
	{
		return 0;
	}

}
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST

void KGameServer::ServerVersion()
{
	std::wcout << L"Server Ver : " << GetMainVersion() << std::endl;
}

//{{ 2010. 10. 19	최육사	SMS테스트
#ifdef SERV_SMS_TEST
void KGameServer::SMSTest_LUA()
{
	// 문자 전송!
	KDBE_SEND_PHONE_MSG_NOT kPacketNot;
	//{{ 2012. 10. 15	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	SiKSMSPhoneNumberManager()->GetPhoneNumberList( KSMSPhoneNumberManager::FS_TEST, kPacketNot.m_vecPhoneNum );
#else
	kPacketNot.m_vecPhoneNum.push_back( std::wstring( L"010-8421-1075" ) );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	kPacketNot.m_wstrSMSMessage = L"SMS 테스트 문자 전송!";
	SendToSMSDB( DBE_SEND_PHONE_MSG_NOT, kPacketNot );

	START_LOG( cout, L"SMS 테스트 문자 전송!" );
}
#endif SERV_SMS_TEST
//}}

//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW
//////////////////////////////////////////////////////////////////////////

void KGameServer::WriteServerInfoToDB()
{
	CTime tCurrentTime = CTime::GetCurrentTime();

	// 로그인서버에서 동접 정보 집계하도록 보낸다.
	KELG_UPDATE_CCU_INFO_NOT kNot;
	kNot.m_kCCUInfo.m_iServerUID		  = GetUID();
	kNot.m_kCCUInfo.m_iConcurrentUser	  = KActorManager::GetKObj()->GetCount();
	kNot.m_kCCUInfo.m_iCurPartyCount	  = SiKPartyListManager()->GetCurrentPartyCount();
	kNot.m_kCCUInfo.m_iPlayGamePartyCount = SiKPartyListManager()->GetPlayGamePartyCount();

#ifdef SERV_CHANNELING_USER_MANAGER
	SiKChannelingUserManager()->GetChannelingUserList( kNot.m_kCCUInfo.m_mapChannelingUserList );
#endif SERV_CHANNELING_USER_MANAGER

	SendToLoginServer( ELG_UPDATE_CCU_INFO_NOT, kNot );

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	SendToGlobalServer( EGB_UPDATE_CCU_INFO_NOT, kNot );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	m_dwServerInfoUpdateTick = ::GetTickCount();
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

void KGameServer::WriteServerInfoToDB()
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	KDBE_UPDATE_SERVER_INFO_REQ kPacket;
	kPacket.m_wstrRegDate = ( CStringW )( tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
	kPacket.m_iServerUID = GetUID();
	kPacket.m_iConcurrentUser = KActorManager::GetKObj()->GetCount();
	//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY
	kPacket.m_iCurPartyCount = SiKPartyListManager()->GetCurrentPartyCount();
	kPacket.m_iPlayGamePartyCount = SiKPartyListManager()->GetPlayGamePartyCount();
#else
	kPacket.m_iCurPartyCount = SiKPartyManager()->GetCurrentPartyCount();
	kPacket.m_iPlayGamePartyCount = SiKPartyManager()->GetPlayGamePartyCount();
#endif SERV_CHANNEL_PARTY
	//}}
	SendToLogDB( DBE_UPDATE_SERVER_INFO_REQ, kPacket );

	m_dwServerInfoUpdateTick = ::GetTickCount();
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_CCU_NEW
//}}

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
void KGameServer::InitTimeControlItemReleaseTick()
{
	m_mapTimeControlItem_StaticDBReleaseTick.clear();
	std::map<int, int>::iterator mit = m_mapTimeControlItem_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_ITEM_CHECK);

	if ( mit != m_mapTimeControlItem_StaticDBReleaseTick.end() )
	{
		mit->second = 0;
	}
	else
	{
		m_mapTimeControlItem_StaticDBReleaseTick.insert(std::make_pair(TCIRTT_TIME_CONTROL_ITEM_CHECK, 0));
	}
}

void KGameServer::SetTimeControlItemReleaseTick(TimeControl_Item_Release_Tick_Type _enum ,std::map<int, int> &mapReleaseTick)
{

	std::map<int, int>::iterator sitstr = mapReleaseTick.find(_enum);
	std::map<int, int>::iterator sitdst = m_mapTimeControlItem_StaticDBReleaseTick.find(_enum);

	if(sitdst != m_mapTimeControlItem_StaticDBReleaseTick.end())
	{
		m_mapTimeControlItem_StaticDBReleaseTick.find(_enum)->second = sitstr->second;
	}
	else
	{
		m_mapTimeControlItem_StaticDBReleaseTick.insert(std::make_pair(sitstr->first, sitstr->second));
	}
}

int KGameServer::GetTimeControlItemReleaseTick(TimeControl_Item_Release_Tick_Type _enum)
{
	std::map<int, int>::const_iterator sit = m_mapTimeControlItem_StaticDBReleaseTick.find(_enum);

	if(sit != m_mapTimeControlItem_StaticDBReleaseTick.end())
	{
		return sit->second;
	}
	else
	{
		return -1;
	}
}

bool KGameServer::TickCheckTimeControlItem()
{
	if( m_tTimeControlItemCheckTimer.elapsed() > 60.0f )
	{
		int iReleaseTickCount = 0;

		iReleaseTickCount = GetTimeControlItemReleaseTick( TCIRTT_TIME_CONTROL_ITEM_CHECK );
		if ( iReleaseTickCount == -1 )
		{
			m_tTimeControlItemCheckTimer.restart();
			return false;
		}

		KDBE_GET_ITEM_ONOFF_NPCSHOP_REQ kPacket;
		kPacket.m_mapTimeControlItem_StaticDBReleaseTick.insert(std::make_pair( TCIRTT_TIME_CONTROL_ITEM_CHECK, iReleaseTickCount ));
		SendToLogDB( DBE_GET_ITEM_ONOFF_NPCSHOP_REQ, kPacket );

		m_tTimeControlItemCheckTimer.restart();
	}
	return true;
}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
void KGameServer::InitTimeControlCubeInItemMappingReleaseTick()
{
	m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.clear();
	std::map<int, int>::iterator mit = m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING);

	if ( mit != m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end() )
	{
		mit->second = 0;
	}
	else
	{
		m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.insert(std::make_pair(TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING, 0));
	}
}

void KGameServer::SetTimeControlCubeInItemMappingReleaseTick(TimeControl_CubeInItemMapping_Release_Tick_Type _enum ,std::map<int, int> &mapReleaseTick)
{
	std::map<int, int>::iterator sitstr = mapReleaseTick.find(_enum);
	std::map<int, int>::iterator sitdst = m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(_enum);

	if(sitdst != m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end())
	{
		m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(_enum)->second = sitstr->second;
	}
	else
	{
		m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.insert(std::make_pair(sitstr->first, sitstr->second));
	}
}

int KGameServer::GetTimeControlCubeInItemMappingReleaseTick(TimeControl_CubeInItemMapping_Release_Tick_Type _enum)
{
	std::map<int, int>::const_iterator sit = m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(_enum);

	if(sit != m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end())
	{
		return sit->second;
	}
	else
	{
		return -1;
	}
}

bool KGameServer::TickCheckTimeControlCubeInItemMapping()
{
	if( m_tTimeControlCubeInItemMappingCheckTimer.elapsed() > 60.0f )
	{
		int iReleaseTickCount = 0;

		iReleaseTickCount = GetTimeControlCubeInItemMappingReleaseTick( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING );
		if ( iReleaseTickCount == -1 )
		{
			m_tTimeControlCubeInItemMappingCheckTimer.restart();
			return false;
		}

		KDBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ kPacket;
		kPacket.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.insert(std::make_pair( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING, iReleaseTickCount ));
		SendToLogDB( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ, kPacket );

		m_tTimeControlCubeInItemMappingCheckTimer.restart();
	}
	return true;
}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 

void KGameServer::ProcessEvent( const KEventPtr& spEvent_ )
{
    KSerializer ks;
    ks.BeginReading( &spEvent_->m_kbuff );

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
   _CASE( DBE_SERVER_ON_ACK, KDBE_GAME_SERVER_ON_ACK );
   _CASE( DBE_UPDATE_SERVER_INFO_ACK, KServerList );
	//{{ 2009. 2. 13  최육사	채널리스트
	CASE( DBE_CHANNEL_LIST_ACK );
	//}}
    CASE( ERM_ROOM_LIST_INFO_NOT );
	CASE( ERM_SQUARE_LIST_INFO_NOT );

   _CASE( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT );
	CASE( ENX_UPDATE_PRODUCT_LIST_NOT );
	CASE( DBE_EVENT_UPDATE_ACK );

	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	CASE( DBE_RELEASE_TICK_UPDATE_ACK );
	CASE( DBE_EVENT_DATA_UPDATE_ACK );
	CASE( DBE_EVENT_MONSTER_UPDATE_ACK );
	CASE( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK );
	CASE( DBE_EVENT_REWARD_DATA_UPDATE_ACK );
	//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
	CASE( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK );
#endif SERV_CHAR_LEVEL_UP_EVENT
	//}}
	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
	CASE( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
#else
	//CASE( ELG_RANKING_TITLE_REWARD_NOT );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}	
	CASE( DBE_INSERT_TITLE_ACK );
	CASE( DBE_INSERT_REWARD_TO_POST_ACK );
	//{{ 2008. 10. 31  최육사	어뷰저 리스트
	CASE( DBE_ABUSER_LIST_ACK );
	//}}
	//{{ 2008. 11. 5  최육사	광장 개편	
	CASE( ERM_UPDATE_FIELD_INFO_NOT );
	//{{ 2009. 4. 22  최육사	대전유저리스트
	CASE( ECN_NEW_PVP_USER_INFO_NOT );
	CASE( ECN_UPDATE_PVP_USER_INFO_NOT );
   _CASE( ECN_CHECK_INVALID_PVP_USER_NOT, UidType );
	//}}
	//{{ 2009. 6. 1  최육사		채널이동
	CASE( ECN_CHECK_CHANNEL_CHANGE_REQ );
	//}}

	//{{ 2009. 6. 3  최육사		확성기
   _CASE( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT );
	//}}

	//{{ 2009. 7. 6  최육사		랭킹 개편
	CASE( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK );
	CASE( ELG_HENIR_RANKING_REFRESH_NOT );
	CASE( ELG_WEB_RANKING_REFRESH_NOT );
	//}}

	//{{ 2009. 7. 10  최육사	사기 시스템
	CASE( ERM_INCREASE_PARTY_FEVER_NOT );
	//}}
	//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( ERM_MAX_PARTY_FEVER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2009. 10. 30  최육사	길드레벨
#ifdef GUILD_TEST
	CASE( DBE_JOIN_GUILD_ACK );

	CASE( ELG_KICK_RESULT_LETTER_NOT );

	CASE( DBE_EXPAND_GUILD_MAX_MEMBER_ACK );

	CASE( DBE_UPDATE_GUILD_EXP_ACK );

   _CASE( ELG_GUILD_LEVEL_UP_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
#endif GUILD_TEST
	//}}

   //{{ 2010. 02. 01  최육사	길드 게시판
#ifdef SERV_GUILD_AD
   _CASE( ELG_SEND_LETTER_JOIN_GUILD_NOT, KDBE_ACCEPT_JOIN_GUILD_ACK );
#endif SERV_GUILD_AD
   //}}

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	CASE( DBE_CHECK_HACKING_TOOL_LIST_ACK );
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2010. 02. 17  최육사	채널파티통합
#ifdef SERV_CHANNEL_PARTY
	CASE( EPM_OPEN_PARTY_LIST_NOT );
	CASE( EPM_CLOSE_PARTY_LIST_NOT );
	CASE( EPM_ADD_PARTY_LIST_NOT );
	CASE( EPM_DEL_PARTY_LIST_NOT );
	CASE( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT );
	CASE( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT );
	CASE( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT );
	CASE( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT );
#endif SERV_CHANNEL_PARTY
	//}}
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	CASE( DBE_INCREASE_WEB_POINT_ACK );
   _CASE( ELG_WEB_POINT_REWARD_INSERT_TO_POST_NOT, KDBE_GET_WEB_POINT_REWARD_ACK );
#endif SERV_WEB_POINT_EVENT
	//}}

#ifdef DEF_TRADE_BOARD
    CASE( ERM_PERSONAL_SHOP_INFO_NOT );
#endif DEF_TRADE_BOARD

	//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT
   _CASE( ESR_ORDER_TO_GAME_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT );
   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT );
#endif SERV_REALTIME_SCRIPT
	//}}

#ifdef SERV_KOG_STATISTICS
   CASE( DBE_CCU_AGENT_ACK );
#endif SERV_KOG_STATISTICS

	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	CASE( E_DISCONNECT_SERVER_REPORT_NOT );
   _CASE( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT );
#endif SERV_MORNITORING
	//}}

   //{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
   _CASE( EGB_UPDATE_MISSION_INFO_NOT, KEGB_GET_MISSION_INFO_ACK );
	CASE( EGB_GET_MISSION_INFO_ACK );
	CASE( EGB_UPDATE_WORLD_BUFF_NOT );
	CASE( EGB_UPDATE_MISSION_START_TIME_NOT );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	CASE( EGB_TODAY_RANDOM_QUEST_ACK );
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
	_CASE( EGB_SHUT_DOWN_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
#endif SERV_SHUTDOWN_SYSTEM
	//}}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	CASE( DBE_GET_ITEM_ONOFF_NPCSHOP_ACK );
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
	CASE( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	//{{ 2012. 03. 23	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	CASE( EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( ERM_BATTLE_FIELD_ROOM_INFO_NOT );
	CASE( EGB_GET_AUTO_PARTY_BONUS_INFO_ACK );
	CASE( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT );
	CASE( ERM_END_GAME_REGROUP_PARTY_NOT );
	CASE( ERM_COMPLETE_RETURN_TO_FIELD_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	_CASE( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
	//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
	CASE_NOPARAM( EGB_EARTH_QUAKE_NOT );
#endif SERV_EARTH_QUAKE_EFFECT
	//}}
	//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	CASE( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK );
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	_CASE( EGB_JACKPOT_EVENT_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT );
	_CASE( EGB_JACKPOT_EVENT_POST_NOT, KDBE_INSERT_REWARD_TO_POST_REQ );
#endif SERV_EVENT_JACKPOT
	//}}
	//{{ 2013. 3. 11	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, SiKGSLocalRankingManager() );
	CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_RESET_NOT, SiKGSLocalRankingManager() );
	CASE_FOR_REDIRECT( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT, SiKGSLocalRankingManager() );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}
#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
	CASE( DBE_CHECK_EVENT_UPDATE_ACK );
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK	

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
	CASE( ECN_SET_ACTIVE_LAGCHECK_NOT );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	_CASE( ELG_WEDDING_HALL_INFO_UPDATE_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK );
	CASE( ERM_WEDDING_HALL_INFO_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
	CASE( EGB_TIME_ENCHANT_EVENT_INFO_NOT );
	CASE( EGB_TIME_ENCHANT_EVENT_INFO_ACK );
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
	CASE( EGB_EXCHANGE_LIMIT_INFO_NOT );
	CASE( EGB_EXCHANGE_LIMIT_INFO_ACK );
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
	CASE( DBE_GET_CASHSHOP_ON_OFF_INFO_ACK );
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	CASE( DBE_EVENT_DB_SCRIPT_ACK );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
	_CASE( EGB_UPDATE_GLOBAL_MISSION_INFO_NOT, KEGB_GET_GLOBAL_MISSION_INFO_ACK );
	CASE( EGB_GET_GLOBAL_MISSION_INFO_ACK );
	CASE( EGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER
	//}} 2012. 09. 03	임홍락	글로벌 미션 매니저

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-29	// 박세훈
	CASE( ERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT );
	CASE_NOPARAM( EGB_VERIFY_SERVER_CONNECT_NOT );
	CASE_NOPARAM( EGB_VERIFY_SERVER_DISCONNECT_NOT );
	CASE( EGB_BATTLE_FIELD_BOSS_INFO_NOT );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_ENTRY_POINT
    _CASE( DBE_CHANNEL_LIST_2ND_ACK, KDBE_CHANNEL_LIST_ACK );
#endif SERV_ENTRY_POINT

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	_CASE( EGS_ADD_COMMON_FLAG_NOT, KEGS_ADD_COMMON_FLAG_NOT );
	_CASE( EGS_DEL_COMMON_FLAG_NOT, KEGS_DEL_COMMON_FLAG_NOT );
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

#ifdef SERV_STRING_FILTER_USING_DB
	CASE( DBE_CHECK_STRING_FILTER_UPDATE_ACK );
	CASE( DBE_STRING_FILTER_UPDATE_ACK );
#endif //SERV_STRING_FILTER_USING_DB

    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음 - " << spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID );
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }
}

void KGameServer::SendToCnServer( unsigned short usEventID )
{
    SendToCnServer( usEventID, char() );
}

void KGameServer::SendToLoginServer( unsigned short usEventID )
{
	SendToLoginServer( usEventID, char() );
}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
void KGameServer::SendToGlobalServer( unsigned short usEventID )
{
	SendToGlobalServer( usEventID, char() );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

_IMPL_ON_FUNC( DBE_SERVER_ON_ACK, KDBE_GAME_SERVER_ON_ACK )
{
	//스타트에 실패했으면 프로그램을 끈다
	if( kPacket_.m_iOK != NetError::NET_OK)
	{
        ::exit( 0 );
        return;
	}

    SetServerInfo( kPacket_.m_kServerInfo );

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	SetPCBangIPList( kPacket_.m_mapPCBangIPList );
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST

	std::map< int, KNetAddress >::iterator mit;
	for( mit = kPacket_.m_mapCenterList.begin(); mit != kPacket_.m_mapCenterList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_CENTER, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}

	for( mit = kPacket_.m_mapLoginList.begin(); mit != kPacket_.m_mapLoginList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_LOGIN, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	for( mit = kPacket_.m_mapGlobalList.begin(); mit != kPacket_.m_mapGlobalList.end(); mit++ )
	{
		SiKProxyManager()->AddRemoteAddress( KProxyManager::PT_GLOBAL, mit->first, mit->second.m_wstrIP.c_str(), mit->second.m_usPort );
	}
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2009. 5. 5  최육사		채널 정보
	SiKChannelManager()->Init( GetServerGroupID(), kPacket_.m_iChannelID, kPacket_.m_kBonusInfo );
	//}}

    SetEvent( m_spEventAck.get() );
}

_IMPL_ON_FUNC( DBE_UPDATE_SERVER_INFO_ACK, KServerList )
{
    SetServerList( kPacket_ );
}

//{{ 2009. 2. 13  최육사	채널리스트
IMPL_ON_FUNC( DBE_CHANNEL_LIST_ACK )
{
	SiKChannelManager()->UpdateChannelList( kPacket_.m_mapChannelList );

	//{{ 2012. 06. 29	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	SiKChannelManager()->UpdateChannelBonusList(kPacket_.m_mapChannelBonusList);
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
}

#ifdef SERV_ENTRY_POINT
_IMPL_ON_FUNC( DBE_CHANNEL_LIST_2ND_ACK, KDBE_CHANNEL_LIST_ACK )
{
    SiKChannelManager()->UpdateChannelList2nd( kPacket_.m_mapChannelList );

    SiKChannelManager()->UpdateChannelBonusList2nd( kPacket_.m_mapChannelBonusList );
}
#endif SERV_ENTRY_POINT

IMPL_ON_FUNC( ERM_ROOM_LIST_INFO_NOT )
{
    switch( kPacket_.m_iCode )
    {
    case NetError::ERR_ROOM_14:
        // 방 정보 추가.
		if( SiKRoomListManager()->AddRoomInfo( kPacket_.m_kRoomInfo ) )
        {
			int iProxyID = ( int )KncUid::ExtractServerID( kPacket_.m_kRoomInfo.m_RoomUID );
            SiKProxyManager()->SetRoomServerLoad( iProxyID, true );
        }
        break;
    case NetError::ERR_ROOM_15:
        // 방 정보 수정.
		SiKRoomListManager()->ChangeRoomInfo( kPacket_.m_kRoomInfo );
        break;
    case NetError::ERR_ROOM_16:
        // 방 정보 삭제.
		if( SiKRoomListManager()->DelRoomInfo( kPacket_.m_kRoomInfo ) )
        {
			int iProxyID = ( int )KncUid::ExtractServerID( kPacket_.m_kRoomInfo.m_RoomUID );
            SiKProxyManager()->SetRoomServerLoad( iProxyID, false );
        }
        break;
    default:
        START_LOG( cerr, L"룸 정보 변경 코드가 이상함." )
            << BUILD_LOG( kPacket_.m_iCode )
            << END_LOG;

        break;
    }
}

IMPL_ON_FUNC( ERM_SQUARE_LIST_INFO_NOT )
{
#ifndef DEPRECATED_SERVER_GROUP_MASK
	//{{ 2010. 05. 22  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	if( KncUid::ExtractServerGroupID( kPacket_.m_kSquareInfo.m_iSquareUID ) != GetServerGroupID() )
	{
		START_LOG( cerr, L"다른 서버군의 광장 정보입니다. 이쪽으로 오면 안되는데.." )
			<< BUILD_LOG( kPacket_.m_kSquareInfo.m_iSquareUID )
			<< BUILD_LOG( KncUid::ExtractServerGroupID( kPacket_.m_kSquareInfo.m_iSquareUID ) )
			<< BUILD_LOG( GetServerGroupID() )
			<< END_LOG;		
	}
#endif SERV_INTEGRATION
	//}}
#endif DEPRECATED_SERVER_GROUP_MASK

	switch( kPacket_.m_iCode )
	{
	case NetError::ERR_ROOM_14:
		// 광장 정보 추가.
		if( SiKRoomListManager()->AddSquareInfo( kPacket_.m_kSquareInfo ) )
		{
			//SiKProxyManager()->SetLoad( kPacket_.m_kRoomInfo.m_RoomUID, true );
		}
		break;
	case NetError::ERR_ROOM_15:
		// 광장 정보 수정.
		SiKRoomListManager()->ChangeSquareInfo( kPacket_.m_kSquareInfo );
		break;
	case NetError::ERR_ROOM_16:
		// 광장 정보 삭제.
		if( SiKRoomListManager()->DelSquareInfo( kPacket_.m_kSquareInfo ) )
		{
			//SiKProxyManager()->SetLoad( kPacket_.m_kRoomInfo.m_RoomUID, false );
		}
		break;
	default:
		START_LOG( cerr, L"광장 정보 변경 코드가 이상함." )
			<< BUILD_LOG( kPacket_.m_iCode )
			<< END_LOG;

		break;
	}
}

_IMPL_ON_FUNC( ECN_NOTIFY_MSG_NOT, KEGS_NOTIFY_MSG_NOT )
{
    UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ECN_NOTIFY_MSG_NOT, kPacket_ );

	// GM공지일 경우에만 cout으로 남긴다
	if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS )
	{
		START_LOG( cout, L"[ALL SERVERS NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
	else if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_SERVER_GROUP )
	{
		START_LOG( cout, L"[SERVER GROUP NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
	else if( kPacket_.m_cNotifyType == KEGS_NOTIFY_MSG_NOT::NT_SHUT_DOWN_ALL_NOTIFY )
	{
		START_LOG( cout, L"[ALL SERVERS {SHUT_DOWN} NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
#endif SERV_SHUTDOWN_SYSTEM
	//}}
	else 
	{
		START_LOG( clog, L"[NOTIFY MESSAGE]" )
			<< L" ----------------------------- "		<< dbg::endl
			<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
			<< L" ----------------------------- "		<< dbg::endl;
	}
}

IMPL_ON_FUNC( ENX_UPDATE_PRODUCT_LIST_NOT )
{
	SiCXSLCashItemManager()->UpdateProductList( kPacket_.m_vecProductItemID, kPacket_.m_vecCashItemRessolveInfo, kPacket_.m_ulProductNoResStone );

	//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
	if( SEnum::CheckFlag( GetServerRollType(), KServerInfo::SRT_UPDATE_CASH_PRODUCT_LIST ) == true )
	{
		KELOG_CASH_PRODUCT_LIST_UPDATE_NOT kPacketToLogDB;
		kPacketToLogDB.m_vecCashProductList = kPacket_.m_vecCashProductList;
		SendToLogDB( ELOG_CASH_PRODUCT_LIST_UPDATE_NOT, kPacketToLogDB );

		START_LOG( cout, L"--- 통계를 위한 캐쉬 아이템 리스트 업데이트 ---" )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacketToLogDB.m_vecCashProductList.size() );
	}
#endif SERV_CASH_ITEM_LIST
	//}}
}

IMPL_ON_FUNC( DBE_EVENT_UPDATE_ACK )
{
	if( kPacket_.m_vecEventList.size() > 0 )
	{

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		
		if ( SiKGameEventDBManager()->GetMapEventDBData().empty() == true )
		{
			SendToEventDB( DBE_EVENT_DB_SCRIPT_REQ );
		}
		else
		{
			SiKGameEventManager()->SetEvent( kPacket_.m_vecEventList );
		}
#else //SERV_EVENT_DB_CONTROL_SYSTEM
		SiKGameEventManager()->SetEvent( kPacket_.m_vecEventList );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	}
}

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_RELEASE_TICK_UPDATE_ACK )
{
	SiKReleaseTickManager()->SetReleaseTickData( kPacket_.m_mapReleaseTick );
}
IMPL_ON_FUNC( DBE_EVENT_DATA_UPDATE_ACK )
{
	SiKEventDataRefreshManager()->SetEventData( kPacket_.m_mapEventData );
}

IMPL_ON_FUNC( DBE_EVENT_MONSTER_UPDATE_ACK )
{
	SiKEventMonsterRefreshManager()->SetEventMonsterData( kPacket_.m_mapEventMonster );
}

IMPL_ON_FUNC( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK )
{
	SiKEventNotifyMsgServerInfo()->SetEventNotifyMsgServerInfoData( kPacket_.m_vecEventNotifyMsgServerInfo );
}

IMPL_ON_FUNC( DBE_EVENT_REWARD_DATA_UPDATE_ACK )
{
	SiKEventRewardDataRefreshManager()->SetRewardData( kPacket_.m_mapRewardData );
}

//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
IMPL_ON_FUNC( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK )
{
	SiKEventRewardLevelUpRefreshManager()->SetRewardLevelUp( kPacket_.m_mapLevelUpRewardItem );
}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK )
{
	SiKEventRewardRetainUserItemRefreshManager()->SetRewardRetainUserItem( kPacket_.m_mapRewardItem );
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//{{ 2008. 10. 9  최육사	타이틀
//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
#else
//IMPL_ON_FUNC( ELG_RANKING_TITLE_REWARD_NOT )
//{
//	// 특정 서버만 업데이트 하도록 할것!
//	START_LOG( cout, L"로그인 서버로부터 랭킹 칭호 보상 정보가 도착했습니다! 칭호 보상 지급을 시작합니다!" )
//		<< BUILD_LOG( kPacket_.m_mapTitleReward.size() );
//	
//	// GameDB에 타이틀을 insert해준다.
//	std::map< UidType, KRankingTitleReward >::const_iterator mit;
//	for( mit = kPacket_.m_mapTitleReward.begin(); mit != kPacket_.m_mapTitleReward.end(); ++mit )
//	{
//		const KRankingTitleReward& kTitleReward = mit->second;
//		std::wstring wstrMessage;
//
//		std::vector< KTitleReward >::const_iterator vit;
//		for( vit = kTitleReward.m_vecReward.begin(); vit != kTitleReward.m_vecReward.end(); ++vit )
//		{
//			// 타이틀 보상 주기
//			KDBE_INSERT_TITLE_REQ kPacket;
//			kPacket.m_iUnitUID		   = kTitleReward.m_iUnitUID;
//			kPacket.m_iTitleID		   = vit->m_iTitleID;
//			kPacket.m_sPeriod		   = vit->m_sPeriod;
//			kPacket.m_bGameServerEvent = true;
//			SendToGameDB( DBE_INSERT_TITLE_REQ, kPacket );
//
//			const CXSLTitleManager::TitleTemplet* pTitleInfo = SiCXSLTitleManager()->GetTitleTemplet( vit->m_iTitleID );
//			if( pTitleInfo == NULL )
//			{
//				START_LOG( cerr, L"존재하지 않는 칭호입니다!" )
//					<< BUILD_LOG( vit->m_iTitleID )
//					<< END_LOG;
//
//				continue;
//			}
//
//			wstrMessage += pTitleInfo->m_wstrTitleName;
//			wstrMessage += L",\n\n";
//		}
//
//		// 타이틀 보상에 대해 편지로 알리기
//		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
//		kPacketToDB.m_iFromUnitUID = kTitleReward.m_iUnitUID;
//		kPacketToDB.m_iToUnitUID   = kTitleReward.m_iUnitUID;
//		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_RANKING_TITLE;
//		kPacketToDB.m_iRewardID	   = 0; // 보상없음
//		kPacketToDB.m_wstrMessage  = wstrMessage;
//		kPacketToDB.m_bGameServerEvent = true;
//		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
//	}
//}
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

IMPL_ON_FUNC( DBE_INSERT_TITLE_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( ELG_INSERT_TITLE_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_ACK )
{
	// 보상 성공시
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		KELG_FIND_USER_SEND_LETTER_NOT kPacketNot;
		kPacketNot.m_kLetter = kPacket_.m_iRewardLetter;
		SendToLoginServer( ELG_FIND_USER_SEND_LETTER_NOT, kPacketNot );
	}
}
//}}

//{{ 2008. 10. 31  최육사	어뷰저 리스트
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
IMPL_ON_FUNC( DBE_ABUSER_LIST_ACK )
{
	if( kPacket_.m_vecAbuserList.empty() )
		return;

	START_LOG( cout, L"어뷰저 리스트 갱신!" );

	// 어뷰저 리스트 업데이트
	SiKAbuserLogManager()->SetAbuserList( kPacket_.m_iRtt, kPacket_.m_vecAbuserList );
}
#else
IMPL_ON_FUNC( DBE_ABUSER_LIST_ACK )
{
	if( kPacket_.m_mapAbuserList.empty() )
		return;

	START_LOG( cout, L"어뷰저 리스트 갱신!" );

	// 어뷰저 리스트 업데이트
	SiKAbuserLogManager()->UpdateAbuserList( kPacket_.m_mapAbuserList );
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//}}

//{{ 2008. 11. 5  최육사	광장 개편
IMPL_ON_FUNC( ERM_UPDATE_FIELD_INFO_NOT )
{
	// 필드 정보 업데이트
	SiCXSLMapData()->UpdateFieldInfo( kPacket_.m_iMapID, kPacket_.m_iFieldUID );

	START_LOG( cout, L"필드 정보 업데이트!" )
		<< BUILD_LOG( kPacket_.m_iMapID )
		<< BUILD_LOG( kPacket_.m_iFieldUID );
}
//}}

//{{ 2009. 4. 22  최육사	대전유저리스트
IMPL_ON_FUNC( ECN_NEW_PVP_USER_INFO_NOT )
{
	if( !SiKPVPLobbyManager()->AddPVPUser( kPacket_.m_kUserInfo, kPacket_.m_nGSUID ) )
	{
        START_LOG( cerr, L"pvp user 추가 실패!" )
			<< BUILD_LOG( kPacket_.m_nGSUID )
			<< BUILD_LOG( kPacket_.m_kUserInfo.m_iUnitUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ECN_UPDATE_PVP_USER_INFO_NOT )
{
	std::vector< KPVPUserSimpleInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecUserInfo.begin(); vit != kPacket_.m_vecUserInfo.end(); ++vit )
	{
		if( vit->m_cState == KCommunityUserInfo::US_LEAVE_LOBBY )
		{
			if( !SiKPVPLobbyManager()->RemovePVPUser( vit->m_iUnitUID ) )
			{
				START_LOG( cerr, L"pvp user 삭제 실패!" )
					<< BUILD_LOG( vit->m_iUnitUID )
					<< END_LOG;
			}
		}
		else
		{
			if( !SiKPVPLobbyManager()->UpdatePVPUserInfo( vit->m_iUnitUID, vit->m_cState, vit->m_iStateCode, vit->m_iRoomUID ) )
			{
				START_LOG( cwarn, L"pvp user 갱신 실패!" )
					<< BUILD_LOG( vit->m_iUnitUID )
					<< BUILD_LOGc( vit->m_cState )
					<< BUILD_LOG( vit->m_iStateCode )
					<< BUILD_LOG( vit->m_iRoomUID )
					<< END_LOG;
			}
		}
	}	
}

_IMPL_ON_FUNC( ECN_CHECK_INVALID_PVP_USER_NOT, UidType )
{
	// 접속이 끊어진 게임서버의 유저들을 리스트에서 제거하기
	SiKPVPLobbyManager()->RemovePVPUserDisconnectGS( kPacket_ );
}
//}}

//{{ 2009. 6. 1  최육사		채널이동
IMPL_ON_FUNC( ECN_CHECK_CHANNEL_CHANGE_REQ )
{
	KECN_CHECK_CHANNEL_CHANGE_ACK kPacket;
	kPacket.m_iSourGSUID = kPacket_.m_iSourGSUID;
	kPacket.m_iCharUID = kPacket_.m_iCharUID;

	// 현재 동접( 채널이동 예약포함 )
	int iMaxUser = GetMaxUser();
	if( iMaxUser <= 0 )
	{
		// 0나누기 방지
		iMaxUser = 1;
	}

	int iNowUser = KActorManager::GetKObj()->GetCountNoLock() + SiKChannelManager()->GetChannelChangeWaitingUserCount();

	// 들어갈수 있는 공간이 5%가 되어야 채널이동 가능
	if( ( static_cast<float>( iMaxUser - iNowUser ) / static_cast<float>( iMaxUser ) ) < 0.05f )
	{
		START_LOG( clog, L"채널의 동접 여유공간이 부족하여 채널이동할수 없습니다" )
			<< BUILD_LOG( SiKChannelManager()->GetChannelID() )
			<< BUILD_LOG( iMaxUser )
			<< BUILD_LOG( iNowUser )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_08;
		goto end_proc;
	}

	// 채널 입장 예약
	if( SiKChannelManager()->NewChannelChangeUser( kPacket_.m_iCharUID, kPacket_.m_kChangeUserInfo ) == false )
	{
		START_LOG( cerr, L"채널 입장 예약 실패" )
			<< BUILD_LOG( kPacket_.m_iCharUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CHANNEL_CHANGE_05;
		goto end_proc;
	}

	// 등록성공!
	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToCnServer( ECN_CHECK_CHANNEL_CHANGE_ACK, kPacket );
}
//}}

//{{ 2009. 6. 3  최육사		확성기
_IMPL_ON_FUNC( ECN_MEGAPHONE_MESSAGE_NOT, KEGS_MEGAPHONE_MESSAGE_NOT )
{
	// 모든 유저들에게 브로드캐스팅
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ECN_MEGAPHONE_MESSAGE_NOT, kPacket_ );
}
//}}

//{{ 2009. 7. 6  최육사		랭킹 개편
IMPL_ON_FUNC( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK )
{
	// 헤니르의 시공 랭킹 정보 로그인 서버로부터 받아온것 업데이트
	SiKGSRankingManager()->UpdateHenirRanking( kPacket_.m_mapHenirRanking, kPacket_.m_mapLastRank );
}

IMPL_ON_FUNC( ELG_HENIR_RANKING_REFRESH_NOT )
{	
	START_LOG( clog2, L"로그인서버로부터 헤니르 랭킹정보가 도착했다!" )
		<< BUILD_LOG( kPacket_.m_mapHenirRanking.size() );

	// 갱신된 헤니르의 시공 랭킹 정보 업데이트
	SiKGSRankingManager()->UpdateHenirRanking( kPacket_.m_mapHenirRanking );
}

IMPL_ON_FUNC( ELG_WEB_RANKING_REFRESH_NOT )
{
	// 갱신된 대전&던전 랭킹 정보 업데이트
	SiKGSRankingManager()->UpdateDungeonAndPvpRanking( kPacket_ );
}
//}}

//{{ 2009. 7. 10  최육사	사기 시스템
IMPL_ON_FUNC( ERM_INCREASE_PARTY_FEVER_NOT )
{
	// 파티 사기수치 증가!
	//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY
	if( SiKPartyListManager()->E_IncreasePartyFever( kPacket_ ) == false )
#else
	if( SiKPartyManager()->E_IncreasePartyFever( kPacket_.m_iPartyUID ) == false )
#endif SERV_CHANNEL_PARTY
	//}}
	{
        START_LOG( cerr, L"파티 사기 수치 증가 실패!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}
}
//}}

//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( ERM_MAX_PARTY_FEVER_NOT )
{
	// 파티 사기수치 최대 증가
	if( SiKPartyListManager()->E_MaxPartyFever( kPacket_ ) == false )
	{
		START_LOG( cerr, L"파티 사기 수치 증가 실패!" )
			<< BUILD_LOG( kPacket_.m_iPartyUID )
			<< END_LOG;
	}
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 10. 30  최육사	길드레벨
#ifdef GUILD_TEST

IMPL_ON_FUNC( DBE_JOIN_GUILD_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 가입 실패! 절대 일어나서는 안되는 에러!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kJoinGuildMember.m_iUnitUID )
			<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
			<< END_LOG;

		// 실패하더라도 로그인 서버 한번 갔다오자.
	}

	// 길드원 정보 업데이트하러 가자!
	KELG_JOIN_GUILD_REQ kPacketToLg;
	kPacketToLg.m_iOK = kPacket_.m_iOK;
	kPacketToLg.m_iUnitUID = kPacket_.m_kJoinGuildMember.m_iUnitUID;
	kPacketToLg.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacketToLg.m_kLoginGuildMember = kPacket_.m_kJoinGuildMember;
	kPacketToLg.m_kLoginGuildMember.m_ucMemberShipGrade = SEnum::GUG_NEW_USER; // 길드 처음 가입했으니 길드 신입 회원으로 등록함
	SendToLoginServer( ELG_JOIN_GUILD_REQ, kPacketToLg );
}

IMPL_ON_FUNC( ELG_KICK_RESULT_LETTER_NOT )
{
	// 길드 우편으로 강퇴 사실을 알린다.
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = kPacket_.m_iUnitUID;
	kPacketToDB.m_iToUnitUID   = kPacket_.m_iUnitUID;
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_GUILD_KICK; // 길드 강퇴 우편
	kPacketToDB.m_bGameServerEvent = true;
	kPacketToDB.m_wstrMessage = kPacket_.m_wstrGuildName;
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_EXPAND_GUILD_MAX_MEMBER_ACK )
{
	// 길드해산이 성공하면 길드매니저에 업데이트 하러 가자!
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( DLG_UPDATE_EXPAND_GUILD_MAX_MEMBER_NOT, kPacket_ );
	}
}

IMPL_ON_FUNC( DBE_UPDATE_GUILD_EXP_ACK )
{
	// 길드 EXP DB업데이트 성공하면 길드매니저에 업데이트 하러 가자!
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( ELG_UPDATE_GUILD_EXP_NOT, kPacket_ );
	}
}

_IMPL_ON_FUNC( ELG_GUILD_LEVEL_UP_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT )
{
	// 길드 레벨업 전체 공지를 센터서버로 보내자!
	SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket_ );
}

#endif GUILD_TEST
//}}

//{{ 2010. 02. 01  최육사	길드 게시판
#ifdef SERV_GUILD_AD

_IMPL_ON_FUNC( ELG_SEND_LETTER_JOIN_GUILD_NOT, KDBE_ACCEPT_JOIN_GUILD_ACK )
{
	// 길드 우편으로 가입 사실을 알린다.
	KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
	kPacketToDB.m_iFromUnitUID = kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID;
	kPacketToDB.m_iToUnitUID   = kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID;
	kPacketToDB.m_iRewardType  = KPostItemInfo::LT_GUILD_JOIN; // 길드 가입 우편
	kPacketToDB.m_wstrMessage  = kPacket_.m_wstrGuildName;
	kPacketToDB.m_bGameServerEvent = true;	
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
}

#endif SERV_GUILD_AD
//}}

//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST

IMPL_ON_FUNC( DBE_CHECK_HACKING_TOOL_LIST_ACK )
{
	SiKHackingProtectionManager()->UpdateHackingToolList( kPacket_ );
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2010. 02. 17  최육사	채널파티통합
#ifdef SERV_CHANNEL_PARTY

IMPL_ON_FUNC( EPM_OPEN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_OpenPartyList( kPacket_.m_kPartyInfo, kPacket_.m_vecUserInfo );
}

IMPL_ON_FUNC( EPM_CLOSE_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_ClosePartyList( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( EPM_ADD_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_AddPartyList( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( EPM_DEL_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_DelPartyList( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_UpdatePartyInfo( kPacket_.m_kPartyInfo );
}

//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_UpdatePartyFever( kPacket_.m_iPartyUID, kPacket_.m_sPartyFever );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

IMPL_ON_FUNC( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_AddPartyMemberInPartyList( kPacket_.m_iPartyUID, kPacket_.m_kPartyUserInfo );
}

IMPL_ON_FUNC( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	SiKPartyListManager()->E_DelPartyMemberInPartyList( kPacket_.m_iPartyUID, kPacket_.m_iUnitUID );
}

IMPL_ON_FUNC( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	// 먼저 이탈 처리
	std::vector< UidType >::const_iterator vit;
	for( vit = kPacket_.m_vecDeletePartyUser.begin(); vit != kPacket_.m_vecDeletePartyUser.end(); ++vit )
	{
		// 삭제 처리
		SiKPartyListManager()->E_DelPartyMemberInPartyList( kPacket_.m_iPartyUID, *vit );
	}

	// 업데이트
    SiKPartyListManager()->E_UpdatePartyMemberInfo( kPacket_.m_iPartyUID, kPacket_.m_vecUpdatePartyUser );
}

#endif SERV_CHANNEL_PARTY
//}}

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

IMPL_ON_FUNC( DBE_INCREASE_WEB_POINT_ACK )
{
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// 웹 포인트 증가 실패!
		return;
	}

	// 웹 포인트 증가 성공했으면 우리DB에도 로그를 남기자!
	SendToGameDB( DBE_INCREASE_WEB_POINT_LOG_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_WEB_POINT_REWARD_INSERT_TO_POST_NOT, KDBE_GET_WEB_POINT_REWARD_ACK )
{
	std::vector< KWebPointRewardInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecWebPointReward.begin(); vit != kPacket_.m_vecWebPointReward.end(); ++vit )
	{
		if( vit->m_iUnitUID <= 0 )
		{
			START_LOG( cerr, L"Web Point 보상을 하려는데 UnitUID값이 0이다." )
				<< BUILD_LOG( vit->m_iApplySN )
				<< BUILD_LOG( vit->m_wstrUserID )
				<< BUILD_LOG( vit->m_wstrNickName )
				<< END_LOG;
			continue;
		}

		// 우편으로 보상 지급		
		CStringW cwstrItemID;
		cwstrItemID.Format( L"%d", vit->m_iRewardItemID );

		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDB;
		kPacketToDB.m_iFromUnitUID = vit->m_iUnitUID;
		kPacketToDB.m_iToUnitUID   = vit->m_iUnitUID;
		kPacketToDB.m_iRewardType  = KPostItemInfo::LT_WEB_POINT_EVENT;
		kPacketToDB.m_iRewardID	   = vit->m_iRewardItemID;
		kPacketToDB.m_sQuantity	   = 1;
		kPacketToDB.m_wstrMessage  = cwstrItemID.GetBuffer();
		kPacketToDB.m_bGameServerEvent = true;
		SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDB );
	}    
}

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef DEF_TRADE_BOARD
IMPL_ON_FUNC( ERM_PERSONAL_SHOP_INFO_NOT )
{
	switch( kPacket_.m_iCode )
	{
		// 아이템 추가
	case NetError::ERR_PERSONAL_SHOP_26:
		{
			SiKPersonalShopItemList()->AddItemInfo(kPacket_);
		}
		break;
		// 아이템 수정
	case NetError::ERR_PERSONAL_SHOP_27:
		{
			SiKPersonalShopItemList()->ChangeItemInfo(kPacket_);
		}
		break;
		// 아이템 삭제
	case NetError::ERR_PERSONAL_SHOP_28:
		{
			SiKPersonalShopItemList()->DelItemInfo(kPacket_);
		}
		break;

	default:
		START_LOG( cerr, L"개인상점 아이템 리스트 변경코드 오류." )
			<< BUILD_LOG( kPacket_.m_iCode )
			<< END_LOG;
		break;
	}
}
#endif DEF_TRADE_BOARD

//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT

_IMPL_ON_FUNC( ESR_ORDER_TO_GAME_SERVER_NOT, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	// 로그인서버로 가는 명령인지 확인!
	if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SENTINEL < kPacket_.m_iOrderType
		&& KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SENTINEL > kPacket_.m_iOrderType
		)
	{
		if( SEnum::CheckFlag( GetServerRollType(), KServerInfo::SRT_LOGIN_SERVER_SCRIPT_REFRESH ) == true )
		{
			UidType anTrace[2] = { GetUID(), -1 };
			KncSend( PI_GS_SERVER, GetUID(), PI_LOGIN_SERVER, 0, anTrace, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		}
	}
	// 게임서버로 가는 명령인지 확인!
	else if( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SENTINEL < kPacket_.m_iOrderType  && 
			 KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SENTINEL > kPacket_.m_iOrderType )
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, ESR_ORDER_TO_REFRESH_MANAGER_REQ, kPacket_ );
		SiKGSScriptRefreshManager()->QueueingEvent( spEvent );
	}
	else
	{
		START_LOG( cerr, L"이쪽으로 오면 안되는 갱신 요청 타입입니다!" )
			<< BUILD_LOG( kPacket_.m_iOrderType )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_ACK, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	std::map< int, UidType > mapFieldInfo;

	// MapData가 갱신되는 경우드롭 테이블이 갱신 되는 경우
	if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MAP_DATA )
	{
        mapFieldInfo = SiCXSLMapData()->GetFieldInfo();
	}

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	std::map< int, std::vector< KRewardData > > mapRewardScriptData;
	if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_REWARD_TABLE )
	{
		mapRewardScriptData = SiKRewardTable()->GetMapRewardScriptData();
	}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	switch( kPacket_.m_iOrderType )
	{
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ITEM_MANAGER, CXSLItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RANDOM_ITEM_MANAGER, CXSLRandomItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_MANUFACTURE_ITEM_MANAGER, CXSLManufactureItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_REWARD_TABLE, KRewardTable );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_QUEST_MANAGER, CXSLQuestManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_TITLE_MANAGER, CXSLTitleManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_DUNGEON_MANAGER, CXSLDungeonManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_MAP_DATA, CXSLMapData );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_GUILD_MANAGER, CXSLGuildManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_GAME_SYS_VAL, KGameSysVal );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RESOLVE_ITEM_MANAGER, CXSLResolveItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ENCHANT_ITEM_MANAGER, CXSLEnchantItemManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SOCKET_ITEM, CXSLSocketItem );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ATTRIB_ENCHANT_ITEM, CXSLAttribEnchantItem );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RECOMMEND_USER_TABLE, KRecommendUserTable );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_EXP_TABLE, KExpTable );
	//{{ 2010. 9. 11	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_PET_MANAGER, CXSLPetManager );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_MORNITORING_MANAGER, KMornitoringManager );
#endif SERV_MORNITORING
	//}}
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ABUSER_LOG_MANAGER, KAbuserLogManager );
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_GAME_EVENT_SCRIPT_MANAGER, KGameEventScriptManager );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_PVP_MATCH_MANAGER, KPvpMatchManager );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_CONTENT_MANAGER, KGSContentManager );
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}
	//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_COMPANY_IP_TABLE, KCompanyIPTable );
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_CLASS_CHANGE_TABLE, KClassChangeTable );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_STRING_FILTER, CXSLStringFilter );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_DAILY_GIFT_BOX_MANAGER, KDailyGiftBoxManager );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2012. 10. 19	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_PARTY_MANAGER, CXSLPartyManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_BINGO_EVENT_MANAGER, KGSBingoEventInfo );
#endif SERV_EVENT_BINGO
	//}}

#ifdef SERV_SYNTHESIS_AVATAR
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SYNTHESIS_TABLE, CXSLSynthesisManager );
#endif SERV_SYNTHESIS_AVATAR

	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_DEFENCE_DUNGEON_MANAGER, CXSLDefenceDungeonManager );
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_BUFF_MANAGER, CXSLBuffManager );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_RIDING_PET_MANAGER, CXSLRidingPetManager );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_SKILL_TREE, CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    CASE_SCRIPT_REFRESH_SWAP_INSTANCE( OT_GS_ED_INVENTORY_EXPAND, CXSLEDInventoryExpand );
#endif
	default:
		{
			START_LOG( cerr, L"이쪽으로 오면 안되는 타입인데?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		break;
	}

	// MapData가 갱신되는 경우드롭 테이블이 갱신 되는 경우
	if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MAP_DATA )
	{
		BOOST_MAP_CONST_FOREACH( int, UidType, kFieldInfo, mapFieldInfo )
		{
			SiCXSLMapData()->UpdateFieldInfo( kFieldInfo.first, kFieldInfo.second );
		}

		SiCXSLMapData()->DumpToLogFile();
	}
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GAME_EVENT_SCRIPT_MANAGER )
	{
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
		const std::map< int, EVENT_DATA >	mapEventScriptData = SiKGameEventScriptManager()->GetMapEventScriptData();
		const std::map< int, EVENT_DATA >	mapEventDBData = SiKGameEventDBManager()->GetMapEventDBData();
		
		SiKGameEventManager()->SetTotalEventData( mapEventScriptData, mapEventDBData );

#endif //SERV_EVENT_DB_CONTROL_SYSTEM
		SiKGameEventManager()->RefreshEventScript();
	}
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_CONTENT_MANAGER )
	{
		// 캐쉬샵 상점을 열지 말지를 클라이언트에 알린다.
		KEGS_CASH_SHOP_OPEN_NOT kNot;
		kNot.m_bCashShopOpen = SiKGSContentManager()->IsEnableCashShop();

		{
			KEvent kEvent;
			kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_SHOP_OPEN_NOT, kNot );
			KActorManager::GetKObj()->SendToAll( kEvent );
		}
		
		{
			// 클라이언트에 상품 정보를 다시 다 받으라고 알린다.
			KEvent kEvent;
			kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_PRODUCT_INFO_CHANGED_NOT );
			KActorManager::GetKObj()->SendToAll( kEvent );
		}

		START_LOG( cout, L"[컨텐츠 관리자] 캐쉬샵 상점의 온오프 정보를 클라이언트로 브로드캐스팅 하였습니다!" )
			<< BUILD_LOG( kNot.m_bCashShopOpen );
	}
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}
	//{{ 2011. 11. 04	김민성 일일 랜덤 퀘스트 실시간 패치 
#ifdef SERV_RANDOM_DAY_QUEST
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_QUEST_MANAGER )
	{
		// Random Quest 를 받아 오자
		SendToGlobalServer( EGB_TODAY_RANDOM_QUEST_REQ, KBaseServer::GetKObj()->GetUID() );
	}
#endif SERV_RANDOM_DAY_QUEST
		//}}
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
	else if( kPacket_.m_iOrderType == KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_REWARD_TABLE )
	{
		const std::map< int, std::vector< KRewardData > >	mapRewardDBData = SiKRewardTable()->GetMapRewardDBData();

		SiKRewardTable()->SetMapTotalRewardData(mapRewardScriptData, mapRewardDBData);

		KESG_REWARD_DB_DATA_NOT kPacket;

		kPacket.m_mapTotalRewardData = SiKRewardTable()->GetMapTotalRewardData();
		// 모든 유저들에게 브로드캐스팅
		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ESG_REWARD_DB_DATA_NOT, kPacket );
	}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GameServer 스크립트 실시간 패치 완료"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

#endif SERV_REALTIME_SCRIPT
//}}

#ifdef SERV_KOG_STATISTICS
IMPL_ON_FUNC( DBE_CCU_AGENT_ACK )
{
	if( NetError::NET_OK == kPacket_.m_iOK )
	{
		SiKOGAgent()->DailyInfobyUnitandUser(GetServerGroupID(), SiKChannelManager()->GetChannelID(), kPacket_.m_iDailyUniqueUser, kPacket_.m_iDailyResisterredUser );
	}
	else
	{
		START_LOG(cerr, L"KOG STATISTICS - DAILY INFO SEND FAIL.!")
			<< BUILD_LOG(NetError::GetErrStr(kPacket_.m_iOK))
			<< END_LOG;
	}
}
#endif SERV_KOG_STATISTICS

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
IMPL_ON_FUNC( E_DISCONNECT_SERVER_REPORT_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KMornitoringManager::ET_DISCONNECT_GAME_LOGIN_PROXY:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_LOGIN_PROXY );

			// 로그인 서버와의 접속이 끊어진 상태이므로 로그인서버로 보고 하지 않는다!
		}
		break;

	case KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY );

			// 로그인 서버로 집계하기
			SendToLoginServer( ELG_DISCONNECT_SERVER_REPORT_NOT, kPacket_ );
		}
		break;

	case KMornitoringManager::ET_DISCONNECT_GAME_BILLING:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_BILLING );

			// 로그인 서버로 집계하기
			SendToLoginServer( ELG_DISCONNECT_SERVER_REPORT_NOT, kPacket_ );
		}
		break;

		//{{ 2012. 01. 27  김민성	로그인 인증서버 연결 끊김 자동 sms 전송
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
	case KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH );

			// 로그인 서버로 집계하기
			SendToLoginServer( ELG_DISCONNECT_SERVER_REPORT_NOT, kPacket_ );
		}
		break;

#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
		//}}

	default:
		START_LOG( cerr, L"잘못된 타입입니다!" )
			<< BUILD_LOGc( kPacket_.m_cType )
			<< END_LOG;
		return;
	}
}

_IMPL_ON_FUNC( DBE_SEND_PHONE_MSG_FOR_MORNITORING_ACK, KDBE_SEND_PHONE_MSG_NOT )
{
	SiKMornitoringManager()->ReserveSMSMessage( kPacket_ );
}

#endif SERV_MORNITORING
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
_IMPL_ON_FUNC( EGB_UPDATE_MISSION_INFO_NOT, KEGB_GET_MISSION_INFO_ACK )
{
	SiKGSWorldMissionManager()->UpdateMissionInfo( kPacket_ );
}
IMPL_ON_FUNC( EGB_GET_MISSION_INFO_ACK )
{	
	SiKGSWorldMissionManager()->UpdateMissionInfo( kPacket_ );
}

IMPL_ON_FUNC( EGB_UPDATE_WORLD_BUFF_NOT )
{	
	if( SiKGSWorldMissionManager()->UpdateWorldBuffInfo( kPacket_ ) == true )
	{
		KEGS_UPDATE_WORLD_BUFF_NOT kNot;
		
		SiKGSWorldMissionManager()->GetWorldBuff( kNot );

		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, EGB_UPDATE_WORLD_BUFF_NOT, kNot );
	}
}

IMPL_ON_FUNC( EGB_UPDATE_MISSION_START_TIME_NOT )
{
	SiKGSWorldMissionManager()->UpdateMissionStartTime( kPacket_.m_wsrtStartTime );

	if( kPacket_.m_wsrtStartTime.empty() == true)
	{
		START_LOG( cerr, L"월드 버프 시간이 비었다고 한다." )
			<< BUILD_LOG( kPacket_.m_wsrtStartTime )
			<< END_LOG;
	}	
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
void KGameServer::RequestRandomQuestInfo()
{
	SendToGlobalServer( EGB_TODAY_RANDOM_QUEST_REQ, KBaseServer::GetKObj()->GetUID() );
}

IMPL_ON_FUNC( EGB_TODAY_RANDOM_QUEST_ACK )
{
	SiCXSLQuestManager()->SetRandomQuestGroupList( kPacket_.m_mapTodayRandomQuest );

	// 접속해 있는 모든 유저에게 알려준다
	KEGS_CHANGE_RANDOM_QUEST_NOT kNot;
	kNot.m_mapRandomQuestList = kPacket_.m_mapTodayRandomQuest;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_SERVER, NULL, EGS_CHANGE_RANDOM_QUEST_NOT, kNot );
	KActorManager::GetKObj()->SendToAll( *spEvent );
}

#endif SERV_RANDOM_DAY_QUEST
//}}


//{{ 2011. 12. 16	최육사	동접 최대 인원 실시간 적용
#ifdef SERV_REALTIME_MAX_USER
int KGameServer::GetMaxUser() const
{
	// 채널매니저에 저장된 최대인원을 먼저 찾는다.
	int iChannelMaxUser = SiKChannelManager()->GetMaxUser();
	if( iChannelMaxUser == 0 )
	{
        iChannelMaxUser = GetDefaultMaxUser();
	}

	return iChannelMaxUser;
}
#endif SERV_REALTIME_MAX_USER
//}}

//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
_IMPL_ON_FUNC( EGB_SHUT_DOWN_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT )
{
	SendToCnServer( ECN_NOTIFY_MSG_NOT, kPacket_ );
	
	START_LOG( cout, L"[ALL SERVERS {SHUT_DOWN} NOTIFY MESSAGE]" )
		<< L" ----------------------------- "		<< dbg::endl
		<< L"MSG : " << kPacket_.m_wstrMSG.c_str()	<< dbg::endl
		<< L" ----------------------------- "		<< dbg::endl;

}
#endif SERV_SHUTDOWN_SYSTEM
//}}

//{{ 2012. 03. 23	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
IMPL_ON_FUNC( EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT )
{
	//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	SiKGSWorldMissionManager()->UpdateIndividualBuffInfo( kPacket_ );
#else
	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_GLOBAL_SERVER, 0, PI_GS_USER, 0 , anTrace, EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT, kPacket_ );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
_IMPL_ON_FUNC( ECN_UDP_KICK_GAMEEDIT_NOT, KEGS_UDP_KICK_GAMEEDIT_NOT )
{
	GetKGSSimLayer()->SetUDPKickOff(kPacket_.m_bUDPSetKick);

	UidType anTrace[2] = { 0, -1 };
	KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ECN_UDP_KICK_GAMEEDIT_NOT, kPacket_ );
}
#endif UDP_CAN_NOT_SEND_USER_KICK
//}}

//{{ 공존의 축제 - 지진 효과 - 김민성
#ifdef SERV_EARTH_QUAKE_EFFECT
IMPL_ON_FUNC_NOPARAM( EGB_EARTH_QUAKE_NOT )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_SERVER, NULL, EGS_EARTH_QUAKE_NOT );
	KActorManager::GetKObj()->SendToAll( *spEvent );
}
#endif SERV_EARTH_QUAKE_EFFECT
//}}

//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( ERM_BATTLE_FIELD_ROOM_INFO_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_UPDATE:
		{
			// 변경된 배틀필드 정보를 넣는다!
			SiKBattleFieldListManager()->UpdateBattleFieldRoomInfo( kPacket_.m_kBFRoomInfo );
		}
		break;

	case KERM_BATTLE_FIELD_ROOM_INFO_NOT::IT_DELETE:
		{
			// 변경된 배틀필드 정보를 넣는다!
			SiKBattleFieldListManager()->DeleteBattleFieldRoomInfo( kPacket_.m_kBFRoomInfo );
		}
		break;

	default:
		{
			START_LOG( cerr, L"배틀필드 방 정보 업데이트" )
				<< BUILD_LOGc( kPacket_.m_cType )
				<< END_LOG;
		}
		break;
	}
}

IMPL_ON_FUNC( EGB_GET_AUTO_PARTY_BONUS_INFO_ACK )
{
	BOOST_TEST_FOREACH( const KAutoPartyBonusInfo&, kInfo, kPacket_.m_vecAutoPartyBonusInfo )
	{
        SiKGSAutoPartyManager()->UpdateAutoPartyBonusInfo( kInfo.m_cDungeonMode, kInfo.m_iDungeonIDWithDif, kInfo.m_bActivatedBonus );
	}
}

IMPL_ON_FUNC( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT )
{
	SiKGSAutoPartyManager()->UpdateAutoPartyBonusInfo( kPacket_.m_cDungeonMode, kPacket_.m_iDungeonIDWithDif, kPacket_.m_bActivatedBonus );
}

IMPL_ON_FUNC( ERM_END_GAME_REGROUP_PARTY_NOT )
{    
	// 파티 재결합 요청
	SiKPartyListManager()->E_EndGamePartyRegroup( kPacket_.m_iPartyUID );
}

IMPL_ON_FUNC( ERM_COMPLETE_RETURN_TO_FIELD_NOT )
{
    KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CN_ROOM, kPacket_.m_kReturnToFieldInfo.m_iRoomUID, NULL, ERM_NEXT_RETURN_TO_FIELD_NOT, kPacket_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
IMPL_ON_FUNC( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK )
{
    SiKCorruptionMonitor()->Update( kPacket_ );
}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}

//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
_IMPL_ON_FUNC( EGB_JACKPOT_EVENT_NOTIFY_NOT, KEGS_NOTIFY_MSG_NOT )
{
	KncSend( PI_GS_SERVER, GetUID(), PI_CN_SERVER, 0, NULL, ECN_NOTIFY_MSG_NOT, kPacket_ );

	START_LOG( clog, L"[NOTIFY MESSAGE] => SEND CENTER SERVER " )
		<< L" ----------------------------- "	<< dbg::endl
		<< L"MSG : " << kPacket_.m_wstrMSG	    << dbg::endl
		<< L" ----------------------------- "	<< dbg::endl;
}

_IMPL_ON_FUNC( EGB_JACKPOT_EVENT_POST_NOT, KDBE_INSERT_REWARD_TO_POST_REQ )
{
	// 이벤트 보상을 주자!
	SendToGameDB( DBE_INSERT_REWARD_TO_POST_REQ, kPacket_ );
}
#endif SERV_EVENT_JACKPOT
//}}
#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING // 2012.12.11 lygan_조성욱 // 석근이 작업 리뉴얼 ( DB에서 실시간 값 반영, 교환, 제조 쪽도 적용 )
IMPL_ON_FUNC( DBE_GET_ITEM_ONOFF_NPCSHOP_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapTimeControlItem_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_ITEM_CHECK);

	if ( cmit !=  kPacket_.m_mapTimeControlItem_StaticDBReleaseTick.end() )
	{
		if ( GetTimeControlItemReleaseTick( TCIRTT_TIME_CONTROL_ITEM_CHECK )  != cmit->second )
		{

			if ( kPacket_.m_mapGetItemOnOff.empty() == false )
			{
				GetKGSSimLayer()->SetTimeControlItem_Info(kPacket_.m_mapGetItemOnOff);

				KEGS_GET_TIME_CONTROL_ITME_LIST_NOT kPacket;
				kPacket.m_iOK = NetError::NET_OK;

				UidType anTrace[2] = { 0, -1 };
				KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, DBE_GET_TIME_CONTROL_ITME_LIST_NOT, kPacket );
			}
			
			SetTimeControlItemReleaseTick( TCIRTT_TIME_CONTROL_ITEM_CHECK, kPacket_.m_mapTimeControlItem_StaticDBReleaseTick );
		}
	}
}
#endif //SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
IMPL_ON_FUNC( DBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.find(TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING);

	if ( cmit !=  kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick.end() )
	{
		if ( GetTimeControlCubeInItemMappingReleaseTick( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING )  != cmit->second )
		{

			//if ( kPacket_.m_mapGetCubeInItemMappingOnOff.empty() == false )
			//{
				GetKGSSimLayer()->SetTimeControlCubeInItemMapping_Info(kPacket_.m_mapGetCubeInItemMappingOnOff);

				//KEGS_GET_TIME_CONTROL_ITME_LIST_NOT kPacket;
				//kPacket.m_iOK = NetError::NET_OK;

				//UidType anTrace[2] = { 0, -1 };
				//KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, DBE_GET_TIME_CONTROL_ITME_LIST_NOT, kPacket );
			//}
			
			SetTimeControlCubeInItemMappingReleaseTick( TCIRTT_TIME_CONTROL_CUBE_IN_ITEM_MAPPING, kPacket_.m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick );
		}
	}
}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
IMPL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK )
{
	std::map<int , int>::const_iterator cmit;

	cmit = kPacket_.m_mapReleaseTick.find( KGameEventManager::ERTT_EVENT_DB_SCRIPT_CHECK );

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKGameEventManager()->GetEventDBScriptReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L" 이벤트 DB 스크립트 데이터 변경 된 것을 새로 받아 옵니다." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

			SendToEventDB( DBE_EVENT_DB_SCRIPT_REQ );

			SiKGameEventManager()->SetEventDBScriptReleaseTick( iNewReleaseTick );
		}
	}

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
	 cmit = kPacket_.m_mapReleaseTick.find( KGameEventManager::ERTT_EVENT_CHECK );

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKGameEventManager()->GetEventReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L"이벤트 바뀐 것을 확인하였으므로 새로 받아옵니다." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

			SendToEventDB( DBE_EVENT_UPDATE_REQ );

			SiKGameEventManager()->SetEventReleaseTick( iNewReleaseTick );
		}
	}
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK
}

IMPL_ON_FUNC( DBE_EVENT_DB_SCRIPT_ACK )
{
	if ( kPacket_.m_mapDBRewardData.size() > 0 )
	{
		const std::map< int, std::vector< KRewardData > >	mapRewardScriptData = SiKRewardTable()->GetMapRewardScriptData();
		SiKRewardTable()->SetMapRewardDBData(kPacket_.m_mapDBRewardData);
		const std::map< int, std::vector< KRewardData > >	mapRewardDBData = SiKRewardTable()->GetMapRewardDBData();

		SiKRewardTable()->SetMapTotalRewardData(mapRewardScriptData, mapRewardDBData);

		KESG_REWARD_DB_DATA_NOT kPacket;

		kPacket.m_mapTotalRewardData = SiKRewardTable()->GetMapTotalRewardData();
		// 모든 유저들에게 브로드캐스팅
		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, ESG_REWARD_DB_DATA_NOT, kPacket );
	}

	if( kPacket_.m_mapEventDBData.size() > 0 )
	{
		SiKGameEventDBManager()->SetEventDBData(kPacket_.m_mapEventDBData);

		const std::map< int, EVENT_DATA >	mapEventScriptData = SiKGameEventScriptManager()->GetMapEventScriptData();
		const std::map< int, EVENT_DATA >	mapEventDBData = SiKGameEventDBManager()->GetMapEventDBData();

		SiKGameEventManager()->SetTotalEventData( mapEventScriptData, mapEventDBData );

		if ( SiKGameEventManager()->CheckMapIngEventDataEmpty() == true)
		{
#ifdef SERV_ADD_EVENT_DB
			SendToEventDB( DBE_EVENT_UPDATE_REQ );
#else //SERV_ADD_EVENT_DB
			SendToLogDB( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
		}
		else
		{
			SiKGameEventManager()->RefreshEventScript();
		}
	}
}

#else //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
IMPL_ON_FUNC( DBE_CHECK_EVENT_UPDATE_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapReleaseTick.find( KGameEventManager::ERTT_EVENT_CHECK );

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKGameEventManager()->GetEventReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L"이벤트 바뀐 것을 확인하였으므로 새로 받아옵니다." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

#ifdef SERV_ADD_EVENT_DB
			SendToEventDB( DBE_EVENT_UPDATE_REQ );
#else //SERV_ADD_EVENT_DB
			SendToLogDB( DBE_EVENT_UPDATE_REQ );
#endif //SERV_ADD_EVENT_DB
		
			SiKGameEventManager()->SetEventReleaseTick( iNewReleaseTick );
		}
	}
}
#endif //SERV_REFRESH_EVENT_USING_RELEASE_TICK
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
IMPL_ON_FUNC( ECN_SET_ACTIVE_LAGCHECK_NOT )
{
	m_bActiveLagCheck = kPacket_.m_bActiveLagCheck;
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
_IMPL_ON_FUNC( ELG_WEDDING_HALL_INFO_UPDATE_NOT, KDBE_LOAD_WEDDING_HALL_INFO_ACK )
{
	std::map<int, KWeddingHallInfo>::iterator mit = kPacket_.m_mapWeddingInfo.begin();
	for(  ; mit != kPacket_.m_mapWeddingInfo.end() ; ++mit )
	{
		SiKWeddingHallListManager()->AddWeddingHallInfo( mit->second );
	}
}

IMPL_ON_FUNC( ERM_WEDDING_HALL_INFO_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KERM_WEDDING_HALL_INFO_NOT::IT_INSERT:
		{
			// 새로 생성된 결혼식장 정보
			SiKWeddingHallListManager()->AddWeddingHallInfo( kPacket_.m_kHallInfo );
		}
		break;
	case KERM_WEDDING_HALL_INFO_NOT::IT_UPDATE:
		{
			// 새로 생성된 결혼식장 정보
			SiKWeddingHallListManager()->UpdateWeddingHallInfo( kPacket_.m_kHallInfo, kPacket_.m_vecJoinUnitList );
		}
		break;
	case KERM_WEDDING_HALL_INFO_NOT::IT_DELETE:
		{
			// 소멸된 결혼식장 정보
			SiKWeddingHallListManager()->DeleteWeddingHallInfo( kPacket_.m_kHallInfo.m_iWeddingUID );
		}
		break;

	default:
		{
			START_LOG( cerr, L"결혼식장 정보 업데이트" )
				<< BUILD_LOGc( kPacket_.m_cType )
				<< END_LOG;
		}
		break;
	}
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
bool KGameServer::PopFirstTimeEnchantEvent( void )
{
	if( m_iNumOfTimeEnchantEvent <= 0 )
	{
		return false;
	}

	--m_iNumOfTimeEnchantEvent;
	return true;
}

IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_INFO_NOT )
{
	m_iTimeEnchantEventUID		= kPacket_.m_iTimeEnchantEventUID;
	m_iNumOfTimeEnchantEvent	= kPacket_.m_iNumOfTimeEnchantEvent;
}

IMPL_ON_FUNC( EGB_TIME_ENCHANT_EVENT_INFO_ACK )
{
	m_iTimeEnchantEventUID		= kPacket_.m_iTimeEnchantEventUID;
	m_iNumOfTimeEnchantEvent	= kPacket_.m_iNumOfTimeEnchantEvent;
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-30	// 박세훈
int	KGameServer::DecreaseExchangeLimit( IN const int iDestItemID )
{
	if( m_iExchangeLimitUID == 0 )
	{
		return ELR_NOT_INITED;
	}

	std::map< int, int >::iterator it = m_mapExchangeLimitInfo.find( iDestItemID );
	if( it == m_mapExchangeLimitInfo.end() )
	{
		// 목록 자체에 없다면, 제한 대상이 아니다.
		return ELR_PASS;
	}

	if( it->second <= 0 )
	{
		// 목록엔 있지만, 수량이 소진되었다.
		return ELR_EXHAUSTED;
	}

	// 수량을 감소 시키고, 체크하러 가자.
	--it->second;

	return ELR_CHECK;
}

IMPL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_NOT )
{
	m_iExchangeLimitUID		= kPacket_.m_iExchangeLimitUID;
	m_mapExchangeLimitInfo	= kPacket_.m_mapExchangeLimitInfo;
}

IMPL_ON_FUNC( EGB_EXCHANGE_LIMIT_INFO_ACK )
{
	m_iExchangeLimitUID		= kPacket_.m_iExchangeLimitUID;
	m_mapExchangeLimitInfo	= kPacket_.m_mapExchangeLimitInfo;
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_CONTENT_MANAGER_INT
IMPL_ON_FUNC( DBE_GET_CASHSHOP_ON_OFF_INFO_ACK )
{
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if (SiKGSContentManager()->GetReleaseTick() != kPacket_.m_iReleaseTick )
		{
			SiKGSContentManager()->SetReleaseTick( kPacket_.m_iReleaseTick );
			SiKGSContentManager()->SetEnableCashShop( kPacket_.m_bEnableCashshop );

			// 캐쉬샵 상점을 열지 말지를 클라이언트에 알린다.
			KEGS_CASH_SHOP_OPEN_NOT kNot;
			kNot.m_bCashShopOpen = kPacket_.m_bEnableCashshop;

			{
				KEvent kEvent;
				kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_SHOP_OPEN_NOT, kNot );
				KActorManager::GetKObj()->SendToAll( kEvent );
			}

			if ( kNot.m_bCashShopOpen == true)
			{
				// 클라이언트에 상품 정보를 다시 다 받으라고 알린다.
				KEvent kEvent;
				kEvent.SetData( PI_GS_PROXY, NULL, EGS_CASH_PRODUCT_INFO_CHANGED_NOT );
				KActorManager::GetKObj()->SendToAll( kEvent );
			}

			START_LOG( cout, L"[컨텐츠 관리자] 캐쉬샵 상점의 온오프 정보를 클라이언트로 브로드캐스팅 하였습니다!" )
				<< BUILD_LOG( kNot.m_bCashShopOpen );
		}
	}	
}
#endif SERV_CONTENT_MANAGER_INT

//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
_IMPL_ON_FUNC( EGB_UPDATE_GLOBAL_MISSION_INFO_NOT, KEGB_GET_GLOBAL_MISSION_INFO_ACK )	// 게임 서버에 GlobalMissionInfo 데이터 브로드 캐스팅
{
	SiKGSGlobalMissionManager()->UpdateGlobalMissionInfo( kPacket_ );
}

IMPL_ON_FUNC( EGB_GET_GLOBAL_MISSION_INFO_ACK )
{	
	SiKGSGlobalMissionManager()->UpdateGlobalMissionInfo( kPacket_ );
}

IMPL_ON_FUNC( EGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT )
{
	//SiKGSGlobalMissionManager()->UpdateGlobalMissionInfo( kPacket_.m_wstrStartTime );
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	임홍락	글로벌 미션 매니저

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-29	// 박세훈
void KGameServer::CheckGSFieldBossSystemRequest( IN const CTime tCurrentTime )
{
	const __time64_t tReservedTimeForInitRequest = SiKGSFieldBossManager()->GetReservedTimeForInitRequest();
	
	if( tReservedTimeForInitRequest == 0 )
		return;

	if( tCurrentTime.GetTime() < tReservedTimeForInitRequest )
		return;

	// 실패를 대비하여 1분 뒤에 또 시도하도록 예약하자
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( tCurrentTime.GetTime() + 60 );
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GLOBAL_USER, 0, NULL, EGB_BATTLE_FIELD_BOSS_INFO_NOT, char() );
}

void KGameServer::CheckGSFieldBossSystem( IN const CTime tCurrentTime )
{
	if( SiKGSFieldBossManager()->CheckTerm( tCurrentTime.GetTime() ) == false )
		return;

	// 글로벌 서버와 연결되어 있다면, 자체적으로 처리할 필요 없다.
	if( SiKGSFieldBossManager()->IsConnected() == true )
		return;

	const bool bPortalOpen	= SiKGSFieldBossManager()->IsPortalOpen();
	const bool bCoolTime	= SiKGSFieldBossManager()->IsCoolTime();

	const __time64_t tPortalAppearanceTime = SiKGSFieldBossManager()->GetPortalAppearanceTime();

	// 포탈이 열려 있는지 확인
	if( bPortalOpen == true )
	{
		const __time64_t tPortalHoldingTime = tPortalAppearanceTime + SiKGSFieldBossManager()->GetRemainPortalTime();
		if( tPortalHoldingTime <= tCurrentTime.GetTime() )
		{
			// 포탈 닫힘 처리
			SiKGSFieldBossManager()->ClosePortal();

			// 모든 유저들에게 브로드캐스팅
			KEGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT kPacket;
			kPacket.m_iBattleFieldID	= SiKGSFieldBossManager()->GetPortalAppearanceMap();

			UidType anTrace[2] = { 0, -1 };
			KncSend( PI_CN_SERVER, 0, PI_GS_USER, 0 , anTrace, EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT, kPacket );
		}
	}

	// 쿨 타임 체크
	if( bCoolTime == true )
	{
		const __time64_t tFieldBossCoolTime = tPortalAppearanceTime + SiKGSFieldBossManager()->GetRemainCoolTime();
		if( tFieldBossCoolTime <= tCurrentTime.GetTime() )
		{
			SiKGSFieldBossManager()->SetCoolTimeState( false );
		}
	}
}

IMPL_ON_FUNC( ERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT )
{
	SendToGlobalServer( EGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT, kPacket_ );
}

IMPL_ON_FUNC_NOPARAM( EGB_VERIFY_SERVER_CONNECT_NOT )
{
	// 바로 요청을 보내도록 하자
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( 1 );
	SiKGSFieldBossManager()->SetConnected( true );
}

IMPL_ON_FUNC_NOPARAM( EGB_VERIFY_SERVER_DISCONNECT_NOT )
{
	// 연결이 끊어져있으면 정보 요청 패킷을 보내지 말자
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( 0 );
	SiKGSFieldBossManager()->SetConnected( false );
}

IMPL_ON_FUNC( EGB_BATTLE_FIELD_BOSS_INFO_NOT )
{
	// 초기화 정보를 얻었으니 계속해서 정보 요청 패킷을 보낼 필요가 없다.
	SiKGSFieldBossManager()->SetReservedTimeForInitRequest( 0 );

	const bool bPortalOpen	= SiKGSFieldBossManager()->IsPortalOpen();

	SiKGSFieldBossManager()->SetInfo( CTime::GetCurrentTime().GetTime()
									, kPacket_.m_tRemainPortalTime
									, kPacket_.m_tRemainCoolTime
									, kPacket_.m_iPortalAppearanceMap
									, kPacket_.m_iPortalDestination
									, kPacket_.m_bPortalOpen
									, kPacket_.m_bCoolTime
									);

	// 포탈 Open/Close에 변화가 없다면 루틴 종료
	if( bPortalOpen == kPacket_.m_bPortalOpen )
		return;

	// BroadCasting
	KEvent kEvent;

	if( kPacket_.m_bPortalOpen == true )
	{
		KEGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT kPacket;
		kPacket.m_iBattleFieldID	= kPacket_.m_iPortalAppearanceMap;
		kPacket.m_iBossFieldID		= kPacket_.m_iPortalDestination;
		
		// 모든 유저들에게 브로드캐스팅
		kEvent.SetData( PI_GS_PROXY, NULL, EGS_BATTLE_FIELD_BOSS_GATE_OPEN_NOT, kPacket );
	}
	else
	{
		KEGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT kPacket;
		kPacket.m_iBattleFieldID	= kPacket_.m_iPortalAppearanceMap;

		// 모든 유저들에게 브로드캐스팅
		kEvent.SetData( PI_GS_PROXY, NULL, EGS_BATTLE_FIELD_BOSS_GATE_CLOSE_NOT, kPacket );
	}

	KActorManager::GetKObj()->SendToAll( kEvent );
}
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
_IMPL_ON_FUNC( EGS_ADD_COMMON_FLAG_NOT, KEGS_ADD_COMMON_FLAG_NOT )
{
	START_LOG( cout, L"EGS_ADD_COMMON_FLAG_NOT Recieve UID: " << GetUID() );
	KSimLayer::GetKObj()->AddCommonFlag( kPacket_.dwFlag );	
}
_IMPL_ON_FUNC( EGS_DEL_COMMON_FLAG_NOT, KEGS_DEL_COMMON_FLAG_NOT )
{
	START_LOG( cout, L"EGS_DEL_COMMON_FLAG_NOT Recieve UID: " << GetUID() );
	KSimLayer::GetKObj()->DeleteCommonFlag( kPacket_.dwFlag );	
}
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH


#ifdef SERV_STRING_FILTER_USING_DB
IMPL_ON_FUNC( DBE_CHECK_STRING_FILTER_UPDATE_ACK )
{
	std::map<int , int>::const_iterator cmit = kPacket_.m_mapReleaseTick.find( KStringFilterManager::iReleaseTickType );

	START_LOG( clog, L"스트링 필터 받아왔나" )
		<< BUILD_LOG( SiKStringFilterManager()->GetReleaseTick() )
		<< BUILD_LOG( kPacket_.m_mapReleaseTick.size() )
		<< END_LOG;

	if( cmit !=  kPacket_.m_mapReleaseTick.end() )
	{
		int iOldReleaseTick = SiKStringFilterManager()->GetReleaseTick();
		int iNewReleaseTick = cmit->second;
		if( iOldReleaseTick != iNewReleaseTick )
		{
			START_LOG( cout2, L"스트링 필터 바뀐 것을 확인하였으므로 새로 받아옵니다." )
				<< BUILD_LOG( iOldReleaseTick )
				<< BUILD_LOG( iNewReleaseTick )
				<< END_LOG;

			SendToScriptDB( DBE_STRING_FILTER_UPDATE_REQ );

			SiKStringFilterManager()->SetReleaseTick( iNewReleaseTick );
		}
	}
}
IMPL_ON_FUNC( DBE_STRING_FILTER_UPDATE_ACK )
{
	START_LOG( clog, L"스트링 필터 목록 받아왔다" )
		<< BUILD_LOG( kPacket_.m_vecStringFilterList.size() )
		<< END_LOG;

	if( kPacket_.m_vecStringFilterList.size() > 0 )
	{
		SiKStringFilterManager()->SetStringFilter( kPacket_.m_vecStringFilterList );
	}
}
#endif //SERV_STRING_FILTER_USING_DB
