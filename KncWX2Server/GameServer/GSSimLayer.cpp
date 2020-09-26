#include "GSSimLayer.h"
#include <iomanip>  // std::setiosflags, std::resetiosflags
#include "DBLayer.h"
#include "GameServer.h"
#include "CnProxy.h"
#include "LoginProxy.h"
#include "DBLayer.h"
#include "ProxyManager.h"
#include "KAutoPath.h"
#include "StatTable.h"

#ifdef SERV_GLOBAL_BILLING
#include "BillManager.h"

#if defined (SERV_COUNTRY_US)
#include "../Common/OnlyGlobal/AuthAndBilling/US/PayletterBillingManager.h"
#elif defined (SERV_COUNTRY_EU)
#include "../Common/OnlyGlobal/AuthAndBilling/EU/GameForgeBillingManager.h"
#elif defined (SERV_COUNTRY_TW)
#include "../Common/OnlyGlobal/AuthAndBilling/TW/GashBillingTCPManager.h"
#elif defined (SERV_COUNTRY_JP)
#include "../Common/OnlyGlobal/AuthAndBilling/JP/PURPLEBillingManager.h"
#elif defined (SERV_COUNTRY_ID)
#include "../Common/OnlyGlobal/AuthAndBilling/ID/CJBillingManager.h"
#elif defined (SERV_COUNTRY_BR)
#include "../Common/OnlyGlobal/AuthAndBilling/BR/BRPayletterBillingManager.h"
#elif defined (SERV_COUNTRY_IN)
#include "../Common/OnlyGlobal/AuthAndBilling/IN/NaunBillingManager.h"
#endif // SERV_COUNTRY_XX

#endif // SERV_GLOBAL_BILLING

#include "NexonBillingTCPManager.h"
#include "ActorManager.h"
#include "ExpTable.h"
#include "SpiritTable.h"
#include "TrainingCenterTable.h"
#include "x2data/XSLQuestManager.h"
#include "x2data/XSLSkillTree.h"
#include "x2data/XSLCheckSumManager.h"
#include "X2Data/XSLRandomItemManager.h"
#include "X2Data/XSLManufactureItemManager.h"
#include "X2Data/XSLResolveItemManager.h"
#include "X2Data/XSLEnchantItemManager.h"
#include "X2Data/XSLSocketItem.h"
#include "X2Data/XSLCashItemManager.h"
#include "X2Data/XSLStringFilter.h"
#include "RewardTable.h"
#include "RecommendUserTable.h"
#include "GameEvent/GameEventManager.h"
#include "GameSysVal/GameSysVal.h"
#include "AbuserLog/KAbuserLogManager.h"
#include "X2Data/XSLAttribEnchantItem.h"
#include "X2Data/XSLTitleManager.h"
//{{ 2008. 11. 24  최육사	랭킹 매니저
#include "GSRankingManager.h"
//}}
//{{ 2008. 11. 24  최육사	방리스트
#include "RoomListManager.h"
//}}
//{{ 2009. 7. 16  최육사	채널매니저
#include "ChannelManager.h"
//}}
//{{ 2009. 4. 23  최육사	대전유저리스트
#include "PVPLobbyManager.h"
//}}
#ifdef SERV_ROBOT_TEST
	#include "RobotTestManager.h"
#endif SERV_ROBOT_TEST

#include "FieldManager.h"
//{{ 2011. 12. 05	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "NewPartyListManager.h"
#else
	#include "PartyListManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2008. 12. 28  최육사	
#include "TutorialManager.h"
//{{ 2009. 10. 27  최육사	길드레벨
#ifdef GUILD_TEST
	#include "X2Data/XSLGuildManager.h"
#endif GUILD_TEST
//}}
//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	#include "X2Data/XSLPetManager.h"
#endif SERV_PET_SYSTEM
//}}
//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	#include "HackingProtectionManager.h"
#endif SERV_HACKING_TOOL_LIST
//}}
//{{ 2010. 05. 27  최육사	클라이언트 해킹 방지
#ifdef SERV_CLIENT_SCRIPT
	#include "ClientScriptManager.h"
#endif SERV_CLIENT_SCRIPT
//}}
//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_PC_BANG_DROP_EVENT
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_PC_BANG_DROP_EVENT
//}}
//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	#include "GSContentManager.h"
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
#include "GlobalProxy.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	#include "NexonSOAPManager.h"
#endif SERV_NEXON_AUTH_SOAP
//}}
#ifdef SERV_NAVER_CHANNELING
    #include "NaverSoapManager.h"
#endif SERV_NAVER_CHANNELING

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
//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	#include "PacketMornitoring/PacketMornitoring.h"
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/ReleaseTickManager.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	#include "X2Data/XSLBuffManager.h"
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	#include "..\Common\GameEvent\DailyGiftBoxManager.h"
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 07. 12	최육사		랜덤 큐브 오픈 딜레이 시스템
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	#include "EventDelaySystemManager.h"
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
//}}

//{{ 2012. 07. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
	#include "SMSPhoneNumberManager.h"
#endif SERV_POST_COPY_BUG_FIX
//}}
//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	#include "CorruptionMonitor.h"
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}
//{{ 2011. 11. 4	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2013. 04. 12	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "X2Data/XSLDefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2013. 05. 20	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	#include "X2Data/XSLCharacterStatTable.h"
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

//{{ 2013. 1. 26	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	#include "Temp.h"
#endif SERV_EVENT_BINGO
//}}

#ifdef SERV_CHANNELING_USER_MANAGER
#include "ChannelingUserManager.h"
#endif SERV_CHANNELING_USER_MANAGER

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
#include "X2Data/XSLSynthesismanager.h"
#endif SERV_SYNTHESIS_AVATAR
//}}

//-- 2010. 12. 13. 지헌 : XTRAP 서버 - CXTrapCSM 포함
#ifdef SERV_USE_XTRAP
#include "XTrapCSM.h"
#endif SERV_USE_XTRAP

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	#include "X2Data/XSLRidingPetManager.h"
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_GOOD_ELSWORD
#include "X2Data/XSLEDInventoryExpand.h"
#endif

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_STRING_FILTER_USING_DB
	#include "StringFilterManager.h"
#endif //SERV_STRING_FILTER_USING_DB

NiImplementRTTI( KGSSimLayer, KSimLayer );

KGSSimLayer::KGSSimLayer(void) 
{
}

KGSSimLayer::~KGSSimLayer(void)
{
}

ImplToStringW( KGSSimLayer )
{
	stm_ << L"--------------------------------- SimLayer Dump ---------------------------------" << std::endl;
    START_TOSTRING_PARENTW( KSimLayer );
	stm_ << L"---------------------------------------------------------------------------------" << std::endl;
    SiKProxyManager()->ToString( stm_ );
	stm_ << L"---------------------------------------------------------------------------------" << std::endl;
    KActorManager::GetKObj()->ToString( stm_ );
	stm_ << L"---------------------------------------------------------------------------------" << std::endl;
    KGSUser::DumpMaxLoginTime();

	//stm_ << L"Repeat Event" << std::endl; -- 지금은 볼일이 없다
	//std::map<u_short, int>::const_iterator mit;
	//for( mit = m_mapRepeatEvent.begin(); mit != m_mapRepeatEvent.end(); ++mit )
	//{
	//	stm_	<< TOSTRINGW( KEvent::GetIDStr( mit->first ) )
	//			<< TOSTRINGW( mit->second )
	//			<< std::endl;
	//}

    return stm_;
}

//{{ 2009. 12. 15  최육사	서버관리
void KGSSimLayer::DumpToLogFile()
{
	std::wstringstream wstrDump;
	ToString( wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
//}}

#include <lua_tinker.h>
//using namespace lua_tinker;

void KGSSimLayer::RegToLua()
{
    KSimLayer::RegToLua();

	// 081021. hoons.
	GetKFieldManager()->RegToLua();

	lua_tinker::class_add<KGSSimLayer>( g_pLua, "KGSSimLayer" );
	lua_tinker::class_inh<KGSSimLayer, KSimLayer>( g_pLua );
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "dump",				&KGSSimLayer::Dump );
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "SendNotMSG_GRP",	&KGSSimLayer::SendNotMSG_GRP );
	//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "SendNotMSG_ALL",	&KGSSimLayer::SendNotMSG_ALL );
	//}}
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "AddIP",			&KGSSimLayer::AddIP );
	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "TickDump",			&KGSSimLayer::TickDump_LUA );
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}
#ifdef SERV_USE_XTRAP
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "ReloadXTRAP_MAP",	&KGSSimLayer::ReloadXTRAP_MAP );
#endif //SERV_USE_XTRAP
#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "AddCommonFlag_AllGS",		&KGSSimLayer::AddCommonFlag_AllGS_LUA );
	lua_tinker::class_def<KGSSimLayer>( g_pLua, "DeleteCommonFlag_AllGS",	&KGSSimLayer::DeleteCommonFlag_AllGS_LUA );
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH


    lua_tinker::class_def<KGSSimLayer>( g_pLua, "SetServerGroupNum", &KGSSimLayer::SetServerGroupNum );

	lua_tinker::decl( g_pLua, "SimLayer", this );

#ifdef SERV_GLOBAL_BILLING

#if defined(SERV_COUNTRY_US)
	SiKPayletterBillingManager()->RegToLua();
#elif defined(SERV_COUNTRY_EU)
	SiKGameForgeBillingManager()->RegToLua();
#elif defined(SERV_COUNTRY_TW)
	SiKGASHBillingTCPManager()->RegToLua();
#elif defined(SERV_COUNTRY_JP)
	SiKPurpleBillingManager()->RegToLua();
#elif defined(SERV_COUNTRY_ID)
	SiKCJBillingManager()->RegToLua();
#elif defined(SERV_COUNTRY_BR)
	SiKBRPayletterBillingManager()->RegToLua();
#elif defined(SERV_COUNTRY_IN)
	SiKNaunBillingManager()->RegToLua();
#endif //SERV_COUNTRY_XX

#else // SERV_GLOBAL_BILLING
    SiKNexonBillingTCPManager()->RegToLua();
#endif // SERV_GLOBAL_BILLING

	//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	SiKNexonSOAPManager()->RegToLua();
#endif SERV_NEXON_AUTH_SOAP
	//}}
#ifdef SERV_NAVER_CHANNELING
    SiKNaverSOAPManager()->RegToLua();
#endif SERV_NAVER_CHANNELING
}

KActorProxy* CreateProxy( int iProxyType )
{
	if( iProxyType < 0 || iProxyType >= KProxyManager::PT_NUM )
	{
		START_LOG( cerr, L"프록시 타입이 이상함." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;

		return NULL;
	}

	KActorProxy* pkProxy;
	switch( iProxyType )
	{
	case KProxyManager::PT_CENTER:
		pkProxy = new KCnProxy;
		pkProxy->SetSendQueueSize( 512 * 1024 );
		return pkProxy;
	case KProxyManager::PT_LOGIN:
		pkProxy = new KLoginProxy;
		pkProxy->SetSendQueueSize( 512 * 1024 );
		return pkProxy;
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	case KProxyManager::PT_GLOBAL:
		pkProxy = new KGlobalProxy;
		pkProxy->SetSendQueueSize( 512 * 1024 );
		return pkProxy;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
	default:
		START_LOG( cerr, L"프록시 타입이 이상함." )
			<< BUILD_LOG( iProxyType )
			<< END_LOG;
		return NULL;
	}
}

void KGSSimLayer::Init()
{
	KAutoPath kAutoPath;
	std::string	strFile;
	bool bRVal = true;

	{
		LoadingTimer lt( L"Enum.lua" );

		//추후 ↓에서 이루어 지는 데이터 로딩이 실패할경우 서버를 종료 시켜야한다.
		strFile = "Enum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"Enum 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"Enum.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Enum 정보 로드 성공.!" );
		}
	}

	{
		LoadingTimer lt( L"DungeonEnum.lua" );

		//추후 ↓에서 이루어 지는 데이터 로딩이 실패할경우 서버를 종료 시켜야한다.
		strFile = "DungeonEnum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"DungeonEnum 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
			KBaseServer::GetKObj()->AddFailScriptFileName( L"DungeonEnum.lua" );
		}
		else
		{
			START_LOG( cout, L"DungeonEnum 정보 로드 성공.!" );
		}
	}

	//{{ 2009. 12. 19  최육사	서버군 확장
	{
		LoadingTimer lt(L"CashItemData.lua");

		strFile = "CashItemData.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLCashItemManager()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"Cash Item Data 정보 로드 실패.!" )
			<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"CashItemData.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Cash Item Data 정보 로드 성공.!" );
		}
	}
	//}}


#ifdef SERV_GLOBAL_BILLING //-------------------------------------------------
#if defined (SERV_COUNTRY_US)
	m_vecpThreadMgr.push_back( SiKPayletterBillingManager()->GetInstance() );
#elif defined (SERV_COUNTRY_EU)
	m_vecpThreadMgr.push_back( SiKGameForgeBillingManager()->GetInstance() );
#elif defined (SERV_COUNTRY_TW)
	m_vecpThreadMgr.push_back( SiKGASHBillingTCPManager()->GetInstance() );
#elif defined (SERV_COUNTRY_JP)
	m_vecpThreadMgr.push_back( SiKPurpleBillingManager()->GetInstance() );
#elif defined (SERV_COUNTRY_ID)
	m_vecpThreadMgr.push_back( SiKCJBillingManager()->GetInstance() );
#elif defined (SERV_COUNTRY_BR)
	m_vecpThreadMgr.push_back( SiKBRPayletterBillingManager()->GetInstance() );
#elif defined (SERV_COUNTRY_IN)
	m_vecpThreadMgr.push_back( SiKNaunBillingManager()->GetInstance() );
#endif //SERV_COUNTRY_XX

#else // SERV_GLOBAL_BILLING //-------------------------------------------------
			m_vecpThreadMgr.push_back( SiKNexonBillingTCPManager()->GetInstance() );
			//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
			m_vecpThreadMgr.push_back( KNexonSOAPManager::GetInstance() );
#endif SERV_NEXON_AUTH_SOAP
#ifdef SERV_NAVER_CHANNELING
            m_vecpThreadMgr.push_back( KNaverSOAPManager::GetInstance() );
#endif SERV_NAVER_CHANNELING
#endif // SERV_GLOBAL_BILLING //-------------------------------------------------

    KSimLayer::Init();

    SiKProxyManager()->SetProxyBuilder( CreateProxy );

	{
		//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_PC_BANG_DROP_EVENT
		SiKGameEventManager()->SetServerType( KGameEventManager::ST_GAME );

		KGameEventScriptManager::RegScriptName( "EventData.lua" );
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		KGameEventScriptManager::RegScriptName( "CoboEventData.lua" );
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef  SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		KGameEventScriptManager::RegScriptName( "ValenTineData.lua" );
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		OPEN_SCRIPT_FILE( KGameEventScriptManager );
#endif SERV_PC_BANG_DROP_EVENT
		//}}
	}
	
	{
		LoadingTimer lt( L"DLG_Map_Enum.lua" );

		strFile = "DLG_Map_Enum.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"Map Data Enum 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"DLG_Map_Enum.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Map Data Enum 정보 로드 성공.!" );
		}
	}

	{
		LoadingTimer lt(L"UnitTemplet.lua");

		strFile = "UnitTemplet.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLUnitManager()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"Unit 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"UnitTemplet.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Unit 정보 로드 성공.!" );
		}
	}	

	{
		CXSLDungeonManager::RegScriptName( "DungeonData.lua" );

		if( SiCXSLDungeonManager()->OpenScriptFile_AllDungeonScriptLoad( g_pLua ) == false )
			START_LOG( cerr, L"Dungeon Manager 정보 로드 실패.!" );
		else
			START_LOG( cout, L"Dungeon Manager 정보 로드 성공.!" );

		//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
		if( SiCXSLDungeonManager()->IsLuaPasingSuccess() == false )
		{
			KBaseServer::GetKObj()->AddFailScriptFileName( SiCXSLDungeonManager()->GetParsingLuaFileName().c_str() );
		}
#endif SERV_SCRIPT_PARSING_ERR_REPORT
		//}}
	}

	{
#ifdef SERV_UNITED_SERVER_EU
		std::string strItemLuaName = GetStrItemLuaName();
		CXSLItemManager::RegScriptName( strItemLuaName.c_str() );
#else SERV_UNITED_SERVER_EU
		CXSLItemManager::RegScriptName( "Item.lua" );
#endif SERV_UNITED_SERVER_EU

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
#ifdef SERV_UNITED_SERVER_EU
		std::string strItemTransLuaName = GetStrItemTransLuaName();
		CXSLItemManager::RegScriptName( strItemTransLuaName.c_str() );
#else
		CXSLItemManager::RegScriptName( "ItemTrans.lua" );
#endif
#endif SERV_ITEM_LUA_TRANS_DEVIDE

		CXSLItemManager::RegScriptName( "ItemExchangeTable.lua" );
		//{{ 2010. 9. 1	최육사	기간 초기화 아이템
#ifdef SERV_RESET_PERIOD_EVENT
		CXSLItemManager::RegScriptName( "ExpandPeriodItemData.lua" );
#endif SERV_RESET_PERIOD_EVENT
		//}}
		//{{ 2010. 11. 29	최육사	기간제템 삭제
#ifdef SERV_DELETE_PERIOD_ITEM
		CXSLItemManager::RegScriptName( "DeleteItemData.lua" );
#endif SERV_DELETE_PERIOD_ITEM
		//}}
		//{{ 2011. 06. 18	최육사	ED아이템 구매 체크
#ifdef SERV_CHECK_BUY_ED_ITEM
		CXSLItemManager::RegScriptName( "ShopItem.lua" );
#endif SERV_CHECK_BUY_ED_ITEM
		//}}
		//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		CXSLItemManager::RegScriptName( "CoolTimeGroupItem.lua" );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		CXSLItemManager::RegScriptName( "WeddingItemData.lua" );
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		CXSLItemManager::RegScriptName( "ItemConvertTable.lua" );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		CXSLItemManager::RegScriptName( "KeepItem.lua" );
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_WISH_LIST_NO_ITEM
		CXSLItemManager::RegScriptName( "WishListNoItem.lua" );
#endif SERV_WISH_LIST_NO_ITEM

		OPEN_SCRIPT_FILE( CXSLItemManager );
	}

	{
		CXSLRandomItemManager::RegScriptName( "RandomItemTable.lua" );
		CXSLRandomItemManager::RegScriptName( "RandomItemData.lua" );
#ifdef SERV_CUBE_IN_ITEM_MAPPING
#ifndef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
		CXSLRandomItemManager::RegScriptName( "RandomItemMapping.lua" );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
#endif SERV_CUBE_IN_ITEM_MAPPING
		OPEN_SCRIPT_FILE( CXSLRandomItemManager );
	}

	{
		CXSLManufactureItemManager::RegScriptName( "ManufactureItem.lua" );
		CXSLManufactureItemManager::RegScriptName( "ManufactureResultTable.lua" );
		OPEN_SCRIPT_FILE( CXSLManufactureItemManager );
	}

	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	{
		CXSLSynthesisManager::RegScriptName( "SynthesisTable.lua" );
		OPEN_SCRIPT_FILE( CXSLSynthesisManager );
	}
#endif SERV_SYNTHESIS_AVATAR
	//}}

	{
		CXSLResolveItemManager::RegScriptName( "ResolveTable.lua" );
		OPEN_SCRIPT_FILE( CXSLResolveItemManager );
	}

	{
		CXSLEnchantItemManager::RegScriptName( "EnchantTable.lua" );
		OPEN_SCRIPT_FILE( CXSLEnchantItemManager );
	}

	{
		CXSLSocketItem::RegScriptName( "OptionItemData.lua" );
		CXSLSocketItem::RegScriptName( "SocketItemTable.lua" );
		CXSLSocketItem::RegScriptName( "SetItem.lua" );
		//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
		CXSLSocketItem::RegScriptName( "SocketGroupDataForCashAvatar.lua" );
#endif SERV_CASH_ITEM_SOCKET_OPTION
		//}
		//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		CXSLSocketItem::RegScriptName( "ItemRandomSocket.lua" );
		CXSLSocketItem::RegScriptName( "MiracleCubeTable.lua" );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		OPEN_SCRIPT_FILE( CXSLSocketItem );
	}
	
	{
		LoadingTimer lt(L"PackageItemData.lua");
		
		strFile = "PackageItemData.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLCashItemManager()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"Cash Package Item 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"PackageItemData.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Cash Package Item 정보 로드 성공.!" );
		}
	}

	{
		LoadingTimer lt(L"CashItemPrice.lua");
		
		strFile = "CashItemPrice.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLCashItemManager()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"Cash Item Price 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );

			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"CashItemPrice.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Cash Item Price 정보 로드 성공.!" );
		}
	}

	{
		CXSLAttribEnchantItem::RegScriptName( "AttribEnchantTable.lua" );
		CXSLAttribEnchantItem::RegScriptName( "AttribEnchantRequire.lua" );	
//{{ 2011. 01. 04  김민성	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM
		CXSLAttribEnchantItem::RegScriptName( "AttribAttachTable.lua" );	
#endif SERV_ATTRIBUTE_CHARM
//}}
		OPEN_SCRIPT_FILE( CXSLAttribEnchantItem );
	}

	{
		KRewardTable::RegScriptName( "RewardTable.lua" );
		OPEN_SCRIPT_FILE( KRewardTable );
	}

	{
		KRecommendUserTable::RegScriptName( "RecommendUserTable.lua" );
		OPEN_SCRIPT_FILE( KRecommendUserTable );
	}

	//{{ 2008. 11. 24  최육사	방리스트
	{
		LoadingTimer lt(L"RoomListData.lua");

		strFile = "RoomListData.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiKRoomListManager()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"Room List 생성 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"RoomListData.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Room List 생성 성공.!" );
		}
	}
	//}}

	{
		KExpTable::RegScriptName( "ExpTable.lua" );
		OPEN_SCRIPT_FILE( KExpTable );
	}

	{
		LoadingTimer lt(L"StatTable.lua");

		strFile = "StatTable.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiKStatTable()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"STAT TABLE 정보 로드 실패.!" );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"StatTable.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"STAT TABLE 정보 로드 성공.!" );
		}
	}	

	{
		CXSLQuestManager::RegScriptName( "FieldQuest.lua" );
		CXSLQuestManager::RegScriptName( "FieldSubQuest.lua" );
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		CXSLQuestManager::RegScriptName( "AccountQuest.lua" );
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		CXSLQuestManager::RegScriptName( "AccountQuestDate.lua" );
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}
		OPEN_SCRIPT_FILE( CXSLQuestManager );
	}

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	CXSLSkillTree::RegScriptName( "NewSkillTempletVer2.lua" );
	CXSLSkillTree::RegScriptName( "SkillData.lua" );
	OPEN_SCRIPT_FILE( CXSLSkillTree );
#else // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
	{
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		LoadingTimer lt(L"NewSkillTempletVer2.lua");

		strFile = "NewSkillTempletVer2.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLSkillTree()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"SKILL TEMPLET 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
			KBaseServer::GetKObj()->AddFailScriptFileName( L"NewSkillTempletVer2.lua" );

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		LoadingTimer lt(L"NewSkillTemplet.lua");

		strFile = "NewSkillTemplet.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLSkillTree()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"SKILL TEMPLET 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"NewSkillTemplet.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		}
		else
		{
			START_LOG( cout, L"SKILL TEMPLET 정보 로드 성공.!" );
		}

		strFile = "SkillData.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiCXSLSkillTree()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"SKILL DATA 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"SkillData.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"SKILL DATA 정보 로드 성공.!" );
		}
	}
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

	//{ -- 안쓴다
	//	LoadingTimer lt(L"NewVer.xml");

	//	strFile = "NewVer.xml";
	//	kAutoPath.GetPullPath( strFile );
	//	if ( SiCXSLCheckSumManager()->OpenScriptFile( strFile.c_str() ) == false )
	//		START_LOG( cerr, L"체크섬 정보 로드 실패.!" );
	//	else
	//		START_LOG( cout, L"체크섬 정보 로드 성공.!" );
	//}

	{
		LoadingTimer lt(L"TrainingCenterTemplet");

		strFile = "TrainingCenterTemplet.lua";
		kAutoPath.GetPullPath( strFile );
		if ( SiKTrainingCenterTable()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"훈련소 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"TrainingCenterTemplet" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"훈련소 로드 성공.!" );
		}
	}

	{
		LoadingTimer lt( L"SpiritTable.lua" );
		
		strFile = "SpiritTable.lua";
		kAutoPath.GetPullPath( strFile );
		if ( SiKSpiritTable()->OpenScriptFile( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"레벨별 근성도 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"SpiritTable.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"레벨별 근성도 로드 성공.!" );
		}
	}

	{
		CXSLTitleManager::RegScriptName( "TitleTable.lua" );
		CXSLTitleManager::RegScriptName( "TitleMission.lua" );
		CXSLTitleManager::RegScriptName( "SubTitleMission.lua" );
#ifdef SEPARATION_SUB_TITLE_SCRIPT
		CXSLTitleManager::RegScriptName( "SubTitleOpenMission.lua" );
#endif // SEPARATION_SUB_TITLE_SCRIPT
		OPEN_SCRIPT_FILE( CXSLTitleManager );
	}

	//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
#else
	{
#ifdef SERV_UNITED_SERVER_EU
		strFile = GetStrFilterLuaName();
#else SERV_UNITED_SERVER_EU
		strFile = "StringFilter.lua";
#endif SERV_UNITED_SERVER_EU

		LoadingTimer lt( KncUtil::toWideString( strFile ) );

		//strFile = "StringFilter.lua"; // 위에서 처리. 위치 중요
		kAutoPath.GetPullPath( strFile );
		if ( SiCXSLStringFilter()->OpenScript( strFile.c_str() ) == false )
		{
			START_LOG( cerr, L"스트링 필터 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( KncUtil::toWideString( strFile ).c_str() );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"스트링 필터 로드 성공.!" );
		}
	}
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}
	
	{
		KGameSysVal::RegScriptName( "GameSysValTable.lua" );
		OPEN_SCRIPT_FILE( KGameSysVal );
	}
	
	{
		KAbuserLogManager::RegScriptName( "AbuserList.lua" );
		OPEN_SCRIPT_FILE( KAbuserLogManager );
	}

	{
		CXSLMapData::RegScriptName( "MapData.lua" );
		OPEN_SCRIPT_FILE( CXSLMapData );
	}

	{
		LoadingTimer lt( L"FieldData.lua" );

		strFile = "FieldData.lua";
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( g_pLua, strFile.c_str() ) )
		{
			START_LOG( cerr, L"Field Data 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
			//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
			KBaseServer::GetKObj()->AddFailScriptFileName( L"FieldData.lua" );
#endif SERV_SCRIPT_PARSING_ERR_REPORT
			//}}
		}
		else
		{
			START_LOG( cout, L"Field Data 정보 로드 성공.!" );
		}
	}

	//{{ 2008. 12. 10  최육사	로봇
#ifdef SERV_ROBOT_TEST
	if( SiKRobotTestManager()->InitRobotEquipItemList() == false )
		START_LOG( cerr, L"<<<<< Robot Test Manager 초기화 실패! >>>>>" );
	else
		START_LOG( cout, L"<<<<< Robot Test Manager 초기화 성공! >>>>>" );
#endif SERV_ROBOT_TEST
	//}}

	//{{ 2009. 10. 27  최육사	길드레벨
#ifdef GUILD_TEST
	{
		CXSLGuildManager::RegScriptName( "GuildData.lua" );
		OPEN_SCRIPT_FILE( CXSLGuildManager );
	}
#endif GUILD_TEST
	//}}

	//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	{
		CXSLPetManager::RegScriptName( "PetTemplet.lua" );
		CXSLPetManager::RegScriptName( "PetData.lua" );
		OPEN_SCRIPT_FILE( CXSLPetManager );
	}
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	{
		KPvpMatchManager::RegScriptName( "PvpMatchData.lua" );

#ifdef SERV_UNITED_SERVER_EU
		std::string strPvpNpcData = GetStrPvpNpcDataLua();
		KPvpMatchManager::RegScriptName( strPvpNpcData.c_str() );
#else SERV_UNITED_SERVER_EU
		KPvpMatchManager::RegScriptName( "PvpNpcData.lua" );
#endif SERV_UNITED_SERVER_EU
		
		OPEN_SCRIPT_FILE( KPvpMatchManager );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	{
		KMornitoringManager::RegScriptName( "ServerMornitoringConfig.lua" );
		OPEN_SCRIPT_FILE( KMornitoringManager );
	}
#endif SERV_MORNITORING
	//}}

	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	{
		KGSContentManager::RegScriptName( "ContentData.lua" );
		OPEN_SCRIPT_FILE( KGSContentManager );
	}
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}

	//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	{
		KCompanyIPTable::RegScriptName( "CompanyIPTable.lua" );
		OPEN_SCRIPT_FILE( KCompanyIPTable );
	}
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}

	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	{
		KClassChangeTable::RegScriptName( "ClassChangeTable.lua" );
		KClassChangeTable::RegScriptName( "ClassChangeSkill.lua" );
		KClassChangeTable::RegScriptName( "ClassChangeMemo.lua" );
		KClassChangeTable::RegScriptName( "ClassChangeItem.lua" );
		KClassChangeTable::RegScriptName( "ClassChangeQuest.lua" );
		OPEN_SCRIPT_FILE( KClassChangeTable );
	}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	{
#ifdef SERV_UNITED_SERVER_EU
		std::string strFilter = GetStrFilterLuaName();
		KPvpMatchManager::RegScriptName( strFilter.c_str() );
#else SERV_UNITED_SERVER_EU
		CXSLStringFilter::RegScriptName( "StringFilter.lua" );
#endif SERV_UNITED_SERVER_EU
		OPEN_SCRIPT_FILE( CXSLStringFilter );
	}
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	{
		CXSLBuffManager::RegScriptName( "BuffTemplet.lua" );
		OPEN_SCRIPT_FILE( CXSLBuffManager );
	}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
	{
		KSMSPhoneNumberManager::RegScriptName( "SMSPhoneNumberList.lua" );
		OPEN_SCRIPT_FILE( KSMSPhoneNumberManager );
	}
#endif SERV_POST_COPY_BUG_FIX
	//}}

	//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	{
		KCorruptionMonitor::RegScriptName( "CorruptionMonitorConfig.lua" );
		OPEN_SCRIPT_FILE( KCorruptionMonitor );
	}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}

	//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	{
		KDailyGiftBoxManager::RegScriptName( "DailyGiftBoxPackage.lua" );
		OPEN_SCRIPT_FILE( KDailyGiftBoxManager );
	}
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	//{{ 2012. 07. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	{
		CXSLPartyManager::RegScriptName( "PartyServerData.lua" );
		OPEN_SCRIPT_FILE( CXSLPartyManager );
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 11. 4	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	{
		CXSLBattleFieldManager::RegScriptName( "BattleFieldServerData.lua" );
		CXSLBattleFieldManager::RegScriptName( "BattleFieldData.lua" );

		if( SiCXSLBattleFieldManager()->OpenScriptFile_AllBattleFieldScriptLoad( g_pLua ) == false )
			START_LOG( cerr, L"Battle Field Manager 정보 로드 실패.!" );
		else
			START_LOG( cout, L"Battle Field Manager 정보 로드 성공.!" );

		//{{ 2011. 02. 07	최육사	스크립트 파싱 오류 리포트
#ifdef SERV_SCRIPT_PARSING_ERR_REPORT
		if( SiCXSLBattleFieldManager()->IsLuaPasingSuccess() == false )
		{
			KBaseServer::GetKObj()->AddFailScriptFileName( SiCXSLBattleFieldManager()->GetParsingLuaFileName().c_str() );
		}
#endif SERV_SCRIPT_PARSING_ERR_REPORT
		//}}		
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2013. 04. 12	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	{
		CXSLDefenceDungeonManager::RegScriptName( "DefenceDungeonData.lua" );
		OPEN_SCRIPT_FILE( CXSLDefenceDungeonManager );
	}
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	//{{ 2013. 1. 26	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	KGSBingoEventInfo::RegScriptName( "BingoEventInfo.lua" );
	OPEN_SCRIPT_FILE( KGSBingoEventInfo );
#endif SERV_EVENT_BINGO
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	{
		CXSLRidingPetManager::RegScriptName( "RidingPetTemplet.lua" );
		CXSLRidingPetManager::RegScriptName( "RidingPetData.lua" );
		OPEN_SCRIPT_FILE( CXSLRidingPetManager );
	}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_GOOD_ELSWORD
    {
        CXSLEDInventoryExpand::RegScriptName( "EDInventoryExpand.lua" );
        OPEN_SCRIPT_FILE( CXSLEDInventoryExpand );
    }
#endif //SERV_GOOD_ELSWORD

	//{{ 2013. 05. 20	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	{
		if( SiCXSLCharacterStatTable()->OpenScriptFile_CharacterStatTable() == false )
			START_LOG( cerr, L"Character Stat Table 정보 로드 실패.!" );
		else
			START_LOG( cout, L"Character Stat Table 정보 로드 성공.!" );
	}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//{{ 2010. 05. 27  최육사	클라이언트 해킹 방지
#ifdef SERV_CLIENT_SCRIPT
	{
		LoadingTimer It( L"KFLOAT_FUNC.lua" );

		strFile = "KFLOAT_FUNC.lua";
		kAutoPath.GetPullPath( strFile );
		if( SiKClientScriptManager()->LoadClientScript( strFile.c_str(), 0 ) == false )
			START_LOG( cerr, L"Client Script 정보 로드 실패.!" )
				<< BUILD_LOG( KncUtil::toWideString( strFile ) );
		else
			START_LOG( cout, L"Client Script 정보 로드 성공.!" );
	}
#endif SERV_CLIENT_SCRIPT
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	{
		CXSLFieldBossData::RegScriptName( "FieldBossData.lua" );
		OPEN_SCRIPT_FILE( CXSLFieldBossData );
	}
#endif // SERV_BATTLE_FIELD_BOSS

	//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY
	SiKPartyListManager();
#else
	SiKPartyManager();
#endif SERV_CHANNEL_PARTY
	//}}

	//{{ 2009. 4. 23  최육사	대전유저리스트
	SiKPVPLobbyManager();
	//}}

	//{{ 2009. 7. 16  최육사	채널매니저
	SiKChannelManager();
	//}}

	//{{ 2009. 11. 9  최육사	GS랭킹매니저
	SiKGSRankingManager();
	//}}

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager();
#endif SERV_HACKING_TOOL_LIST
	//}}

#ifdef SERV_CHANNELING_USER_MANAGER
	SiKChannelingUserManager();
#endif SERV_CHANNELING_USER_MANAGER

	//{{ 2011. 04. 04  김민성   LFH
#ifdef SERV_LFH
	//Enable_LFH();
	Dump_Blocks_In_All_Heaps();
#endif SERV_LFH
	//}
	//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
	m_bUDPKickUserOff = false;
#endif UDP_CAN_NOT_SEND_USER_KICK
	//}}
	// 해외팀 주석 처리
    //m_iServerGroupNum = DOUBLE_SERVER_GROUP;
}

void KGSSimLayer::Tick()
{
    KSimLayer::Tick();

#ifdef SERV_GLOBAL_BILLING
	SiKBillManager()->Tick();	//SiKGameForgeBillingManager와 착각하지 말기

#if defined(SERV_COUNTRY_TW)
	SiKGASHBillingTCPManager()->Tick();
#elif defined(SERV_COUNTRY_IN)
	SiKNaunBillingManager()->Tick();
#else

#endif //SERV_COUNTRY_XX

#endif // SERV_GLOBAL_BILLING

#ifdef SERV_USE_XTRAP
#ifdef SERV_XTRAP_MAP_LOADING_AUTOMATION
	if( m_tXTrapReloadTimer.elapsed() > 120.0 )
	{
		m_tXTrapReloadTimer.restart();

		ReloadXTRAP_MAP();
	}
#endif SERV_XTRAP_MAP_LOADING_AUTOMATION
#endif SERV_USE_XTRAP

    SiKProxyManager()->Tick();

	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	SiKReleaseTickManager()->Tick();
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}

	// 게임 이벤트
	SiKGameEventManager()->Tick();

	// 어뷰저 리스트 갱신
	SiKAbuserLogManager()->Tick();

#ifdef SERV_STRING_FILTER_USING_DB
	SiKStringFilterManager()->Tick();
#endif //SERV_STRING_FILTER_USING_DB

	//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY	
#else
	SiKPartyManager()->Tick();
#endif SERV_CHANNEL_PARTY
	//}}

	//{{ 2009. 7. 16  최육사	채널매니저
	SiKChannelManager()->Tick();
	//}}

	//{{ 2009. 9. 1  최육사		IOCP종료사유
#ifdef ABNORMAL_REASON
	if( m_tAbnormalReasonTimer.elapsed() > 3600.0 )
	{
		m_tAbnormalReasonTimer.restart();

		SendToDBAbnormalReason( false );
	}
#endif ABNORMAL_REASON
	//}}

	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	SiKHackingProtectionManager()->Tick();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING	
	if( GetKGameServer()->IsConnectionMornitoringEnable() == true )
	{
		SiKMornitoringManager()->Tick_GSSimLayer();
	}
#endif SERV_MORNITORING
	//}}

	//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	SiKPacketMornitoring()->Tick();
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
	//}}

	//{{ 2012. 07. 12	최육사		랜덤 큐브 오픈 딜레이 시스템
#ifdef SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	SiKEventDelaySystemManager()->Tick();
#endif SERV_OPEN_RANDOM_CUBE_DELAY_SYSTEM
	//}}
}

void KGSSimLayer::ShutDown()
{
    KSimLayer::ShutDown();

#ifdef SERV_GLOBAL_BILLING
#if defined (SERV_COUNTRY_US)
	SiKPayletterBillingManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_EU)
	SiKGameForgeBillingManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_TW)
	SiKGASHBillingTCPManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_JP)
	SiKPurpleBillingManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_ID)
	SiKCJBillingManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_BR)
	SiKBRPayletterBillingManager()->ReleaseInstance();
#elif defined (SERV_COUNTRY_IN)
	SiKNaunBillingManager()->ReleaseInstance();
#endif // SERV_COUNTRY_XX
#else // SERV_GLOBAL_BILLING
    SiKNexonBillingTCPManager()->ReleaseInstance();	
#endif // SERV_GLOBAL_BILLING
	
	KGameEventManager::ReleaseInstance();
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	KGameEventScriptManager::ReleaseInstance();
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	KTrainingCenterTable::ReleaseInstance();
	CXSLSkillTree::ReleaseInstance();
    CXSLQuestManager::ReleaseInstance();
	CXSLCheckSumManager::ReleaseInstance();
	KStatTable::ReleaseInstance();
	KExpTable::ReleaseInstance();
	KRoomListManager::ReleaseInstance();
	CXSLDungeonManager::ReleaseInstance();
	CXSLUnitManager::ReleaseInstance();
	CXSLRandomItemManager::ReleaseInstance();
	CXSLManufactureItemManager::ReleaseInstance();	
	CXSLResolveItemManager::ReleaseInstance();
	CXSLEnchantItemManager::ReleaseInstance();
	CXSLSocketItem::ReleaseInstance();
	CXSLCashItemManager::ReleaseInstance();	
	CXSLAttribEnchantItem::ReleaseInstance();
	KRewardTable::ReleaseInstance();
	KRecommendUserTable::ReleaseInstance();	
	CXSLItemManager::ReleaseInstance();
	CXSLMapData::ReleaseInstance();
	KSpiritTable::ReleaseInstance();	
	CXSLTitleManager::ReleaseInstance();
    //KCheckIP::ReleaseInstance();
    KProxyManager::ReleaseInstance();
	CXSLStringFilter::ReleaseInstance();	
	KAbuserLogManager::ReleaseInstance();
	KGameSysVal::ReleaseInstance();
	//{{ 2008. 11. 24  최육사	랭킹 매니저
	KGSRankingManager::ReleaseInstance();
	//}}
	//{{ 2010. 01. 28  최육사	통합파티
#ifdef SERV_CHANNEL_PARTY
	KPartyListManager::ReleaseInstance();
#else
	KPartyManager::ReleaseInstance();
#endif SERV_CHANNEL_PARTY
	//}}
	//{{ 2008. 12. 28  최육사	객체 해제
	KTutorialManager::ReleaseInstance();	
	//KRobotTestManager::ReleaseInstance();
	//}}
	//{{ 2009. 4. 23  최육사	대전유저리스트
	KPVPLobbyManager::ReleaseInstance();
	//}}
	//{{ 2009. 7. 16  최육사	채널매니저
	KChannelManager::ReleaseInstance();
	//}}
	//{{ 2009. 10. 27  최육사	길드레벨
#ifdef GUILD_TEST
	CXSLGuildManager::ReleaseInstance();
#endif GUILD_TEST
	//}}
	//{{ 2010. 8. 6	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	CXSLPetManager::ReleaseInstance();
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	KPvpMatchManager::ReleaseInstance();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST
	KHackingProtectionManager::ReleaseInstance();
#endif SERV_HACKING_TOOL_LIST
	//}}

	//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
	SiKNexonSOAPManager()->ReleaseInstance();
#endif SERV_NEXON_AUTH_SOAP
	//}}
#ifdef SERV_NAVER_CHANNELING
    SiKNaverSOAPManager()->ReleaseInstance();
#endif SERV_NAVER_CHANNELING
	//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	KCompanyIPTable::ReleaseInstance();
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}

	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	KClassChangeTable::ReleaseInstance();
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}
	//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
	KSMSPhoneNumberManager::ReleaseInstance();
#endif SERV_POST_COPY_BUG_FIX
	//}}
	//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	KCorruptionMonitor::ReleaseInstance();
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
	//}}
	//{{ 2012. 03. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CXSLBuffManager::ReleaseInstance();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 07. 30	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CXSLPartyManager::ReleaseInstance();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2011. 11. 4	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CXSLBattleFieldManager::ReleaseInstance();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 04. 12	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CXSLDefenceDungeonManager::ReleaseInstance();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2012. 06. 05	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	KDailyGiftBoxManager::ReleaseInstance();
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	//{{ 2013. 1. 26	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	KGSBingoEventInfo::ReleaseInstance();
#endif SERV_EVENT_BINGO
	//}}
#ifdef SERV_CHANNELING_USER_MANAGER
	KChannelingUserManager::ReleaseInstance();
#endif SERV_CHANNELING_USER_MANAGER

	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	CXSLSynthesisManager::ReleaseInstance();
#endif SERV_SYNTHESIS_AVATAR
	//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	CXSLRidingPetManager::ReleaseInstance();
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	CXSLFieldBossData::ReleaseInstance();
#endif // SERV_BATTLE_FIELD_BOSS
}

//{{ 임홍락 [2012.05.16] 서버 단일화 // SERV_USE_NATION_FLAG 필수 포함임
#ifdef SERV_UNITED_SERVER_EU
std::string KGSSimLayer::GetStrItemLuaName()
{
	std::string strFile = "Item.lua";

	switch(static_cast<int>(GetDwNationFlag()))
	{
	case NF_DE:		strFile = "Item_DE.lua";	break;
	case NF_FR:		strFile = "Item_FR.lua";	break;
	case NF_IT:		strFile = "Item_IT.lua";	break;
	case NF_PL:		strFile = "Item_PL.lua";	break;
	case NF_ES:		strFile = "Item_ES.lua";	break;
	case NF_TR:		strFile = "Item_TR.lua";	break;
	case NF_EN:		strFile = "Item_EN.lua";	break;
	default:
		strFile = "Item.lua";
		break;
	}		

	return strFile;
}

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
std::string KGSSimLayer::GetStrItemTransLuaName()
{
	std::string strFile = "Item.lua";

	switch(static_cast<int>(GetDwNationFlag()))
	{
	case NF_DE:		strFile = "ItemTrans_DE.lua";	break;
	case NF_FR:		strFile = "ItemTrans_FR.lua";	break;
	case NF_IT:		strFile = "ItemTrans_IT.lua";	break;
	case NF_PL:		strFile = "ItemTrans_PL.lua";	break;
	case NF_ES:		strFile = "ItemTrans_ES.lua";	break;
	case NF_TR:		strFile = "ItemTrans_TR.lua";	break;
	case NF_EN:		strFile = "ItemTrans_EN.lua";	break;
	default:
		strFile = "ItemTrans.lua";
		break;
	}

	return strFile;
}
#endif // SERV_ITEM_LUA_TRANS_DEVIDE

std::string KGSSimLayer::GetStrFilterLuaName()
{
	std::string strFile = "StringFilter.lua";
	switch(static_cast<int>(GetDwNationFlag()))
	{
	case NF_DE:		strFile = "StringFilter_DE.lua";	break;
	case NF_FR:		strFile = "StringFilter_FR.lua";	break;
	case NF_IT:		strFile = "StringFilter_IT.lua";	break;
	case NF_PL:		strFile = "StringFilter_PL.lua";	break;
	case NF_ES:		strFile = "StringFilter_ES.lua";	break;
	case NF_TR:		strFile = "StringFilter_TR.lua";	break;
	case NF_EN:		strFile = "StringFilter_EN.lua";	break;
	default:
		strFile = "StringFilter.lua";
		break;
	}	
	return strFile;
}

std::string KGSSimLayer::GetStrPvpNpcDataLua()
{
	std::string strFile = "PvpNpcData.lua";
	switch(static_cast<int>(m_dwNationFlag))
	{
	case NF_DE:		strFile = "PvpNpcData_DE.lua";	break;
	case NF_FR:		strFile = "PvpNpcData_FR.lua";	break;
	case NF_IT:		strFile = "PvpNpcData_IT.lua";	break;
	case NF_PL:		strFile = "PvpNpcData_PL.lua";	break;
	case NF_ES:		strFile = "PvpNpcData_ES.lua";	break;
	case NF_TR:		strFile = "PvpNpcData_TR.lua";	break;
	case NF_EN:		strFile = "PvpNpcData_EN.lua";	break;
	default:
		strFile = "PvpNpcData.lua";
		break;
	}	
	
	return strFile;
}
#endif SERV_UNITED_SERVER_EU
//}}

void KGSSimLayer::SendNotMSG( IN char cNotifyType, IN const std::wstring& wstrNotMSG, IN int iCount )
{
	KEGS_NOTIFY_MSG_NOT	kPacket;
	kPacket.m_cNotifyType = cNotifyType;
	kPacket.m_Count = iCount;
	kPacket.m_wstrMSG = wstrNotMSG;

	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CN_SERVER, 0, NULL, ECN_NOTIFY_MSG_NOT, kPacket );

	START_LOG( clog, L"[NOTIFY MESSAGE] => SEND CENTER SERVER " )
		<< L" ----------------------------- "	<< dbg::endl
		<< L"MSG : " << kPacket.m_wstrMSG	    << dbg::endl
		<< L" ----------------------------- "	<< dbg::endl;
}

void KGSSimLayer::SendNotMSG_GRP( const char* szNotMSG, int iCount /*= 1 */)
{
	std::wstring wstrMsg = KncUtil::toWideString( std::string( szNotMSG ) );

	SendNotMSG( KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_SERVER_GROUP, wstrMsg, iCount );
}

//{{ 2010. 05. 10  최육사	대전던전 서버군 통합
void KGSSimLayer::SendNotMSG_ALL( const char* szNotMSG, int iCount /*= 1*/ )
{
	std::wstring wstrMsg = KncUtil::toWideString( std::string( szNotMSG ) );

	SendNotMSG( KEGS_NOTIFY_MSG_NOT::NT_GM_NOTIFY_ALL_SERVERS, wstrMsg, iCount );
}
//}}

void KGSSimLayer::AddIP( const char* szAddIP )
{
    if( szAddIP == NULL )
    {
        START_LOG( cerr, L"IP 스트링 이상." )
            << END_LOG;

        return;
    }

    std::string strIP = szAddIP;
    if( strIP.size() < 1 || strIP.size() > 15 )
    {
        START_LOG( cerr, L"추가하려는 IP 이상." )
            << BUILD_LOG( strIP.size() )
            << END_LOG;

        return;
    }

	START_LOG( cout, L"=== ADD IP ===" )
		<< BUILD_LOG( strIP.c_str() )
		<< BUILD_LOG( strIP.size() )
		<< L"================"
		<< dbg::endl;

	m_vecIP.push_back( strIP );
}

bool KGSSimLayer::CheckIP( const char* szIP )
{
    if( !CheckCommonFlag( KSimLayer::CF_CHECK_IP ) )
    {
        return true;
    }

	if( m_vecIP.empty() )
    {
        return true;
    }

    if( szIP == NULL )
    {
        START_LOG( cerr, L"비교하려는 IP 이상." )
            << END_LOG;

        return false;
    }

    std::string strIP = szIP;
    if( strIP.size() < 1 || strIP.size() > 15 )
    {
        START_LOG( cerr, L"비교하려는 IP 길이 이상." )
            << BUILD_LOG( strIP.size() )
            << END_LOG;

        return false;
    }

    std::vector< std::string >::iterator vit;
	for( vit = m_vecIP.begin(); vit != m_vecIP.end(); vit++ )
	{
        if( vit->size() < 1 || vit->size() > 15 )
        {
            START_LOG( cerr, L"비교하려는 IP 이상." )
                << BUILD_LOG( vit->size() )
                << END_LOG;

            return false;
        }

        if( vit->compare( 0, vit->size(), strIP, 0, vit->size() ) == 0 )
        {
            return true;
        }
	}

	return false;
}

void KGSSimLayer::AddRepeatEvent( u_short usEventID, int iCnt )
{
	std::map<u_short, int>::iterator mit;

	//반복 이벤트가 있는지 체크..
	mit = m_mapRepeatEvent.find( usEventID );

	//있으면..
	if( mit != m_mapRepeatEvent.end() )
	{
		//체크하여 반복획수가 더 많으면 새로 갱신.
		if( mit->second < iCnt )
			mit->second = iCnt;
	}
	//없다면..
	else
	{
		//새로 넣어줌.
		m_mapRepeatEvent.insert( std::make_pair( usEventID, iCnt ) );
	}
}

//{{ 2009. 9. 1  최육사		IOCP종료사유
#ifdef ABNORMAL_REASON
void KGSSimLayer::AddAbnormalDisconnectReason( DWORD dwReason )
{
	std::map< DWORD, int >::iterator mit = m_mapAbnormalDisconnectReason.find( dwReason );
	if( mit == m_mapAbnormalDisconnectReason.end() )
	{
		m_mapAbnormalDisconnectReason.insert( std::make_pair( dwReason, 1 ) );
	}
	else
	{
		mit->second += 1;
	}
}

void KGSSimLayer::SendToDBAbnormalReason( bool bFinal )
{
	if( !m_mapAbnormalDisconnectReason.empty() )
	{
		KELOG_ABNORMAL_DISCONNECT kPacket;
		kPacket.m_iServerUID = KBaseServer::GetKObj()->GetUID();
		kPacket.m_mapAbnormalDisconnect = m_mapAbnormalDisconnectReason;
		KncSend( PC_SERVER, 0, PC_LOG_DB, 0, NULL, ELOG_ABNORMAL_DISCONNECT, kPacket );

		m_mapAbnormalDisconnectReason.clear();

		if( bFinal )
		{
			START_LOG( cout, L"비정상 종료 세부 사유 통계 DB 업데이트!" );
		}
	}
}
#endif ABNORMAL_REASON
//}}

//{{ 2011. 04. 04  김민성   LFH
#ifdef SERV_LFH
void KGSSimLayer::Enable_LFH()
{
	ULONG  HeapFragValue = 2;

	DWORD NumberOfHeaps = GetProcessHeaps( 0, NULL );

	if ( NumberOfHeaps == 0 ) {
		START_LOG( cerr, L"GetProcessHeaps 실패" );
	}

	HANDLE Heaps[HEAP_SIZE];

	if ( NumberOfHeaps > HEAP_SIZE ) { 
		START_LOG( cerr, L"힙의 개수가 힙 핸들 배열 크기 초과");
		return;
	}

	NumberOfHeaps = GetProcessHeaps( NumberOfHeaps, Heaps );

	for ( DWORD HeapsIndex = 0 ; HeapsIndex < NumberOfHeaps ; ++HeapsIndex ) 
	{

		if ( HeapSetInformation( Heaps[HeapsIndex], HeapCompatibilityInformation, &HeapFragValue, sizeof(HeapFragValue) ) ) 
		{
			START_LOG( cout, L"[LFH]가 활성화 되었습니다." << Heaps[HeapsIndex] );
		}
		else 
		{
			START_LOG( cout, L"[LFH] 활성화 실패. index : " << HeapsIndex );
		}
	}
}
void KGSSimLayer::Dump_Blocks_In_All_Heaps()
{
	//get all the heaps in the process
	HANDLE heaps [100];
	DWORD c = ::GetProcessHeaps (100, heaps);
	START_LOG(cout, L"The process has " << c << L" heaps.\n");

	//get the default heap and the CRT heap (both are among 
	//those retrieved above)

	const HANDLE default_heap = ::GetProcessHeap ();
	const HANDLE crt_heap = (HANDLE) _get_heap_handle ();

	for (unsigned int i = 0; i < c; i++)
	{
		//query the heap attributes
		ULONG heap_info = 0;
		SIZE_T ret_size = 0;
		if (::HeapQueryInformation (heaps [i], 
			HeapCompatibilityInformation, 
			&heap_info, 
			sizeof (heap_info), 
			&ret_size))
		{
			//show the heap attributes
			switch (heap_info)
			{
			case 0:
				START_LOG(cout, L"Heap " << i + 1 << L" is a regular heap.");
				break;
			case 1:
				START_LOG(cout, L"Heap " << i + 1 << L" is a heap with look-asides (fast heap).");
				break;
			case 2:
				START_LOG(cout, L"Heap " << i + 1 << L" is a LFH (low-fragmentation) heap.");
				break;
			default:
				START_LOG(cout, L"Heap " << i + 1 << L" is of unknown type.");
				break;
			}
			if (heaps [i] == default_heap)
			{
				START_LOG(cout, L"This the DEFAULT process heap.");
			}
			if (heaps [i] == crt_heap)
			{
				START_LOG(cout, L"This the heap used by the CRT.");   
			}

//walk the heap and show each allocated block inside it 
//(the attributes of each entry will differ between 
//DEBUG and RELEASE builds)

// 			PROCESS_HEAP_ENTRY entry;
// 			memset (&entry, 0, sizeof (entry));
// 			int count = 0;
// 			while (::HeapWalk (heaps [i], &entry))
// 			{
// 				if (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY)
// 				{
// 					printf (" Allocated entry %d: size: %d, overhead: %d.\n", ++count, entry.cbData, entry.cbOverhead);
// 				}
// 			}
		}
	}
}


#endif SERV_LFH
//}}

//{{ 2011. 12. 14	최육사	SimLayer Tick퍼포먼스 체크
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
void KGSSimLayer::TickDump_LUA()
{
	std::wstringstream wstrDump;
	KSimLayer::GetKObj()->TickDump( "KSimLayer", wstrDump );
	KActorManager::GetKObj()->TickDump( "KActorManager", wstrDump );
	SiKProxyManager()->TickDump( "KProxyManager", wstrDump );
	KBaseServer::GetKObj()->TickDump( "KBaseServer", wstrDump );
	SiKTRServer()->TickDump( "KTRServer", wstrDump );
	SiKGameEventManager()->TickDump( "KGameEventManager", wstrDump );
	SiKAbuserLogManager()->TickDump( "KAbuserLogManager", wstrDump );
	SiKChannelManager()->TickDump( "KChannelManager", wstrDump );
	dbg::cout << wstrDump.str() << dbg::endl;
}
#endif SERV_SIMLAYER_TICK_PERFORMANCE_CHECK
//}}

#ifdef SERV_USE_XTRAP
void KGSSimLayer::ReloadXTRAP_MAP()
{
	KLocker lock( m_csXTrapReload );

	if( false == KSimLayer::GetKObj()->CheckCommonFlag( KSimLayer::CommonFlag::CF_XTRAP ) )
		return;

	bool bModified = false;

	FILE *fi;

	// 바뀐 맵파일이 있는지 검사
	for( int iCSFileIndex = 1; iCSFileIndex <= CSFILE_NUM; ++iCSFileIndex )
	{
		std::stringstream strstmFileName;
		strstmFileName << "map" << iCSFileIndex << ".cs3";

		fi = fopen( strstmFileName.str().c_str(), "rb" );	// MAP 파일 읽는 부분
		if( fi == NULL )
		{
			START_LOG( cerr, L"맵파일 로드 실패!" )
				<< BUILD_LOG( strstmFileName.str() )
				<< END_LOG;
			continue;
		}
		std::vector< char > vecXTrapMap;
		vecXTrapMap.resize( XTRAP_CS4_BUFSIZE_MAP );

		fread( &vecXTrapMap[0], XTRAP_CS4_BUFSIZE_MAP, 1, fi );
		fclose( fi );

		if( m_setLoadedMaps.find( vecXTrapMap ) == m_setLoadedMaps.end() )
		{
			// 아직 없던 맵이므로 추가합니다.
			bModified = true;
			m_setLoadedMaps.insert( vecXTrapMap );

			// 실제 xtrap에서 사용하는 버퍼를 채웁니다.
			m_vecUsingMap.resize( m_setLoadedMaps.size() * XTRAP_CS4_BUFSIZE_MAP );
			int iOffset = XTRAP_CS4_BUFSIZE_MAP * m_setLoadedMaps.size() - XTRAP_CS4_BUFSIZE_MAP;
			memcpy( &m_vecUsingMap[iOffset], &vecXTrapMap[0], XTRAP_CS4_BUFSIZE_MAP );

			START_LOG( clog2, L"다음 맵파일이 변경되었으므로 추가합니다." )
				<< BUILD_LOG( strstmFileName.str() )
				<< END_LOG;
		}
	}

	// 바뀐파일이 있을 경우 전체 로드합니다.
	if( bModified )
	{
		unsigned int result = XTrap_S_LoadDll();	// XTrap4Server.dll 로드

		if(result == 0)
			START_LOG( cout, L"XTrap Server DLL 로드 성공!");
		else
			START_LOG( cerr, L"XTrap Server DLL 로드 실패!");

		START_LOG( clog2, L"맵파일이 추가되어 현재 개수는 다음과 같습니다." )
			<< BUILD_LOG( m_setLoadedMaps.size() )
			<< END_LOG;

		XTrap_S_Start( 600, m_setLoadedMaps.size(), &m_vecUsingMap[0], NULL );	// XTrap server 모듈 시작
	}
}
#endif //SERV_USE_XTRAP

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH // 단일 센터 서버로 보내기
void KGSSimLayer::DeleteCommonFlag_AllGS_LUA( DWORD dwFlag )
{
	KECN_DEL_COMMON_FLAG_NOT kNot;
	kNot.dwFlag = dwFlag;
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CN_SERVER, 0, NULL, ECN_DEL_COMMON_FLAG_NOT, kNot );
}
void KGSSimLayer::AddCommonFlag_AllGS_LUA( DWORD dwFlag )
{
	KECN_ADD_COMMON_FLAG_NOT kNot;
	kNot.dwFlag = dwFlag;
	KncSend( PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), PI_CN_SERVER, 0, NULL, ECN_ADD_COMMON_FLAG_NOT, kNot );
}
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

void KGSSimLayer::SetServerGroupNum( int iGroupNum_ )
{
    m_iServerGroupNum = iGroupNum_;
}

int KGSSimLayer::GetServerGroupNum()
{
    return m_iServerGroupNum;
}

