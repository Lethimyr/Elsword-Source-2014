#include "ProxyManager.h"
#include "GSScriptRefreshThread.h"
#include "GSScriptRefreshManager.h"
#include "KncSend.h"
#include "KAutoPath.h"


#ifdef SERV_PROCESS_COMMUNICATION_KSMS
#include "..\Common\OnlyGlobal\ProcessCommuniationModule\ProcessCommunicationManager.h"
#endif //SERV_PROCESS_COMMUNICATION_KSMS

//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
//#ifdef SERV_REALTIME_SCRIPT

ImplPfID( KGSScriptRefreshManager, PI_NULL );

ImplementSingleton( KGSScriptRefreshManager );

KGSScriptRefreshManager::KGSScriptRefreshManager() :
m_pRefreshLuaState( NULL )
{
}

KGSScriptRefreshManager::~KGSScriptRefreshManager()
{
	if( m_pRefreshLuaState != NULL )
	{
		lua_close( m_pRefreshLuaState );
		//{{ 2010. 8. 30	최육사	서버 정상 종료 처리		
		m_pRefreshLuaState = NULL;
		//}}
	}
}

void KGSScriptRefreshManager::RegToLua( lua_State* pLuaState )
{
	lua_tinker::class_add<KGSScriptRefreshManager>( pLuaState, "KGSScriptRefreshManager" );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ItemManager",				&KGSScriptRefreshManager::ItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RandomItemManager",			&KGSScriptRefreshManager::RandomItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ManufactureItemManager",	&KGSScriptRefreshManager::ManufactureItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RewardTable",				&KGSScriptRefreshManager::RewardTable_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "QuestManager",				&KGSScriptRefreshManager::QuestManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "TitleManager",				&KGSScriptRefreshManager::TitleManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DungeonManager",			&KGSScriptRefreshManager::DungeonManager_LUA );	
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DropTable",					&KGSScriptRefreshManager::DropTable_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "GameEventScriptManager",	&KGSScriptRefreshManager::GameEventScriptManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "MapData",					&KGSScriptRefreshManager::MapData_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "GuildManager",				&KGSScriptRefreshManager::GuildManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "GameSysVal",				&KGSScriptRefreshManager::GameSysVal_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ResolveItemManager",		&KGSScriptRefreshManager::ResolveItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "EnchantItemManager",		&KGSScriptRefreshManager::EnchantItemManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SocketItem",				&KGSScriptRefreshManager::SocketItem_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AttribEnchantItem",			&KGSScriptRefreshManager::AttribEnchantItem_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RecommendUserTable",		&KGSScriptRefreshManager::RecommendUserTable_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ExpTable",					&KGSScriptRefreshManager::ExpTable_LUA );
	//{{ 2010. 9. 11	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PetManager",				&KGSScriptRefreshManager::PetManager_LUA );
#endif SERV_PET_SYSTEM
	//}}
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AttribNpcTable",			&KGSScriptRefreshManager::AttribNpcTable_LUA );
	//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "CnHackingCheckManager",		&KGSScriptRefreshManager::CnHackingCheckManager_LUA );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "MornitoringManager",		&KGSScriptRefreshManager::MornitoringManager_LUA );
#endif SERV_MORNITORING
	//}}
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AbuserLogManager",			&KGSScriptRefreshManager::AbuserLogManager_LUA );
	//{{ 2011. 02. 23	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ResultProcess",				&KGSScriptRefreshManager::ResultProcess_LUA );
#endif SERV_DUNGEON_RANK_NEW
	//}}
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "WorldMissionManager",		&KGSScriptRefreshManager::WorldMissionManager_LUA );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가
	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PvpMatchManager",			&KGSScriptRefreshManager::PvpMatchManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PvpMatchResultTable",		&KGSScriptRefreshManager::PvpMatchResultTable_LUA );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 24	최육사	시공 개편
#ifdef SERV_NEW_HENIR_TEST
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "HenirResultTable",			&KGSScriptRefreshManager::HenirResultTable_LUA );
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ContentManager",			&KGSScriptRefreshManager::ContentManager_LUA );
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}

	//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "CompanyIPTable",			&KGSScriptRefreshManager::CompanyIPTable_LUA );
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ClassChangeTable",			&KGSScriptRefreshManager::ClassChangeTable_LUA );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "StringFilter",				&KGSScriptRefreshManager::StringFilter_LUA );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 04. 25	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "IncreaseTheProtectedCrystalCount",	&KGSScriptRefreshManager::WorldMissionManager_IncreaseCrystalCount_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "ResetTheProtectedCrystalCount",	&KGSScriptRefreshManager::WorldMissionManager_ResetIncreaseCrystalCount_LUA );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DailyGiftBoxManager",	&KGSScriptRefreshManager::DailyGiftBoxManager_Lua );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BattleFieldManager",		&KGSScriptRefreshManager::BattleFieldManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "AutoPartyScriptManager",	&KGSScriptRefreshManager::AutoPartyScriptManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "PartyManager",				&KGSScriptRefreshManager::PartyManager_LUA );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SmsManager",	&KGSScriptRefreshManager::SmsManager_Lua );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BingoEventManager",	&KGSScriptRefreshManager::BingoEventManager_Lua );
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BadAttitudeTable",				&KGSScriptRefreshManager::BadAttitudeTable_LUA );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

#ifdef SERV_SYNTHESIS_AVATAR
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SynthesisTable",			&KGSScriptRefreshManager::SynthesisTable_LUA );
#endif SERV_SYNTHESIS_AVATAR

	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "LogManager",				&KGSScriptRefreshManager::LogManager_LUA );
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "DefenceDungeonManager",		&KGSScriptRefreshManager::DefenceDungeonManager_LUA );
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "BuffManager",				&KGSScriptRefreshManager::BuffManager_LUA );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "RidingPetManager",			&KGSScriptRefreshManager::RidingPetManager_LUA );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "SkillTree",			&KGSScriptRefreshManager::SkillTree_LUA );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "FieldBossData",		&KGSScriptRefreshManager::FieldBossData_LUA );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    lua_tinker::class_def<KGSScriptRefreshManager>( pLuaState, "EDInventory", &KGSScriptRefreshManager::EDInventoryExpand_LUA );
#endif // SERV_GOOD_ELSWORD

	lua_tinker::decl( pLuaState, "ScriptRefresh", this );
}

void KGSScriptRefreshManager::ItemManager_LUA()
{
	START_LOG( cout, L"ItemManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 ItemManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ITEM_MANAGER );

	// 센터서버의 ItemManager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ItemManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::RandomItemManager_LUA()
{
	START_LOG( cout, L"RandomItemManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 RandomItemManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RANDOM_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"RandomItemManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::ManufactureItemManager_LUA()
{
	START_LOG( cout, L"ManufactureItemManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 ManufactureItemManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MANUFACTURE_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ManufactureItemManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::RewardTable_LUA()
{
	START_LOG( cout, L"RewardTable 클래스 스크립트 실시간 패치!" );

	// 게임서버의 RewardTable 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_REWARD_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"RewardTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::QuestManager_LUA()
{
	START_LOG( cout, L"QuestManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 QuestManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_QUEST_MANAGER );

	// 센터서버의 QuestManager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_QUEST_MANAGER );

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	// 글로벌서버의 QuestManager 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_QUEST_MANAGER );
#endif SERV_RANDOM_DAY_QUEST
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"QuestManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::TitleManager_LUA()
{
	START_LOG( cout, L"TitleManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 TitleManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_TITLE_MANAGER );

	//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_TITLE_MANAGER );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"TitleManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::DungeonManager_LUA()
{
	START_LOG( cout, L"DungeonManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 DungeonManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_DUNGEON_MANAGER );

	// 센터서버의 DungeonManager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_DUNGEON_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"DungeonManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::DropTable_LUA()
{
	START_LOG( cout, L"DropTable 클래스 스크립트 실시간 패치!" );

	// 센터서버의 DropTable 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_DROP_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"DropTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::GameEventScriptManager_LUA()
{
	START_LOG( cout, L"GameEventScriptManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 GameEventScriptManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GAME_EVENT_SCRIPT_MANAGER );

	// 센터서버의 GameEventScriptManager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_GAME_EVENT_SCRIPT_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GameEventScriptManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::MapData_LUA()
{
	START_LOG( cout, L"MapData 클래스 스크립트 실시간 패치!" );

	// 게임서버의 MapData 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MAP_DATA );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"MapData 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::GuildManager_LUA()
{
	START_LOG( cout, L"GuildManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 GuildManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GUILD_MANAGER );

	// 로그인서버의 GuildManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_GUILD_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GuildManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}

void KGSScriptRefreshManager::GameSysVal_LUA()
{
	START_LOG( cout, L"GameSysVal 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Game Sys Val 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_GAME_SYS_VAL );

	// 센터서버의 Game Sys Val 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_GAME_SYS_VAL );

	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_GAME_SYS_VAL );
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"GameSysVal 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::ResolveItemManager_LUA()
{
	START_LOG( cout, L"ResolveItemManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Resolve Item Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RESOLVE_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ResolveItemManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::EnchantItemManager_LUA()
{
	START_LOG( cout, L"EnchantItemManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Enchant Item Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ENCHANT_ITEM_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"EnchantItemManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::SocketItem_LUA()
{
	START_LOG( cout, L"SocketItem 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Socket Item 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SOCKET_ITEM );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"SocketItem 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::AttribEnchantItem_LUA()
{
	START_LOG( cout, L"AttribEnchantItem 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Attrib Enchant Item 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ATTRIB_ENCHANT_ITEM );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AttribEnchantItem 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::RecommendUserTable_LUA()
{
	START_LOG( cout, L"RecommendUserTable 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Recommend User Table 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RECOMMEND_USER_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"RecommendUserTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::ExpTable_LUA()
{
	START_LOG( cout, L"ExpTable 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Exp Table 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_EXP_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ExpTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

//{{ 2010. 9. 11	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
void KGSScriptRefreshManager::PetManager_LUA()
{
	START_LOG( cout, L"PetManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Pet Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_PET_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PetManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS


}
#endif SERV_PET_SYSTEM
//}}

void KGSScriptRefreshManager::AttribNpcTable_LUA()
{
	START_LOG( cout, L"AttribNpcTable 클래스 스크립트 실시간 패치!" );

	// 센터서버의 Attrib Npc Table 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_ATTRIB_NPC_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AttribNpcTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
void KGSScriptRefreshManager::CnHackingCheckManager_LUA()
{
	START_LOG( cout, L"CnHackingCheckManager 클래스 스크립트 실시간 패치!" );

	// 센터서버의 Cn Hacking Check Manager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_HACKING_CHECK_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"CnHackingCheckManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_CN_SERVER_HACKING_CHECK
//}}

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
void KGSScriptRefreshManager::MornitoringManager_LUA()
{
	START_LOG( cout, L"MornitoringManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 MornitoringManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_MORNITORING_MANAGER );

	// 로그인서버의 MornitoringManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_MORNITORING_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"MornitoringManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_MORNITORING
//}}

void KGSScriptRefreshManager::AbuserLogManager_LUA()
{
	START_LOG( cout, L"AbuserLogManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Abuser Log Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ABUSER_LOG_MANAGER );

	// 센터서버의 Abuser Log Manager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_ABUSER_LOG_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AbuserLogManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

//{{ 2011. 02. 23	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
void KGSScriptRefreshManager::ResultProcess_LUA()
{
	START_LOG( cout, L"ResultProcess 클래스 스크립트 실시간 패치!" );

	// 센터서버의 ResultProcess 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_RESULT_PROCESS );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ResultProcess 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_DUNGEON_RANK_NEW
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
void KGSScriptRefreshManager::WorldMissionManager_LUA()
{
	START_LOG( cout, L"WorldMissionManager 클래스 스크립트 실시간 패치!" );

	// 글로벌서버의 WorldMissionManager 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_WORLD_MISSION_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"WorldMissionManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
void KGSScriptRefreshManager::PvpMatchManager_LUA()
{
	START_LOG( cout, L"PvpMatchManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Pvp Match Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_PVP_MATCH_MANAGER );

	// 센터서버의 Pvp Match Manager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_PVP_MATCH_MANAGER );

	// 글로벌서버의 Pvp Match Manager 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_PVP_MATCH_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PvpMatchManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::PvpMatchResultTable_LUA()
{
	START_LOG( cout, L"PvpMatchResultTable 클래스 스크립트 실시간 패치!" );

	// 센터서버의 Pvp Match Result Table 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_PVP_MATCH_RESULT_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PvpMatchResultTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 08. 24	최육사	시공 개편
#ifdef SERV_NEW_HENIR_TEST
void KGSScriptRefreshManager::HenirResultTable_LUA()
{
	START_LOG( cout, L"HenirResultTable 클래스 스크립트 실시간 패치!" );
    
	// 센터서버의 Henir Result Table 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_HENIR_RESULT_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"HenirResultTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_NEW_HENIR_TEST
//}}


//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
void KGSScriptRefreshManager::ContentManager_LUA()
{
	START_LOG( cout, L"ContentManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 ContentManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_CONTENT_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ContentManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
//}}

//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
void KGSScriptRefreshManager::BattleFieldManager_LUA()
{
	START_LOG( cout, L"BattleFieldManager 클래스 스크립트 실시간 패치!" );

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	// 글로벌서버의 Battle Field Manager 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_BATTLE_FIELD_MANAGER );
#endif // SERV_BATTLE_FIELD_BOSS

	// 센터서버의 Battle Field Manager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_BATTLE_FIELD_MANAGER );

	// 게임서버의 Battle Field Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_BATTLE_FIELD_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"BattleFieldManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::AutoPartyScriptManager_LUA()
{
	START_LOG( cout, L"AutoPartyScriptManager 클래스 스크립트 실시간 패치!");

	// 글로벌서버의 AutoPartyScriptManager 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_AUTO_PARTY_SCRIPT_MANAGER );

	//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	// 센터서버의 AutoPartyScriptManager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_AUTO_PARTY_SCRIPT_MANAGER );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"AutoPartyScriptManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::PartyManager_LUA()
{
	START_LOG( cout, L"PartyManager 클래스 스크립트 실시간 패치!" );
    
	// 센터서버의 Party Manager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_PARTY_MANAGER );

	// 게임서버의 Party Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_PARTY_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"PartyManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
void KGSScriptRefreshManager::BadAttitudeTable_LUA()
{
	START_LOG( cout, L"BadAttitudeTable 클래스 스크립트 실시간 패치!" );

	// 센터서버의 BadAttitudeTable 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_BAD_ATTITUDE_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"BadAttitudeTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
void KGSScriptRefreshManager::DefenceDungeonManager_LUA()
{
	START_LOG( cout, L"Defence Dungeon Manager 클래스 스크립트 실시간 패치!" );

	// 센터서버의 Defence Dungeon Manager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_DEFENCE_DUNGEON_MANAGER );

	// 게임서버의 Defence Dungeon Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_DEFENCE_DUNGEON_MANAGER );
}

void KGSScriptRefreshManager::BuffManager_LUA()
{
	START_LOG( cout, L"Buff Manager 클래스 스크립트 실시간 패치!" );

	// 센터서버의 Buff Manager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_BUFF_MANAGER );

	// 게임서버의 Buff Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_BUFF_MANAGER );
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

void KGSScriptRefreshManager::Init()
{
	// 스크립트 갱신 전용 lua_state 생성!
	m_pRefreshLuaState = lua_open();

	_JIF( m_pRefreshLuaState != NULL, return );

	luaL_openlibs( m_pRefreshLuaState );
	lua_settop( m_pRefreshLuaState, 0 );

	{
		// enum.lua 파싱
		std::string strFile = "Enum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"GSScriptRefreshManager : Enum 정보 로드 실패.!" );
			return;
		}
	}

	{
		// enum.lua 파싱
		std::string strFile = "DungeonEnum.lua";
		KAutoPath kAutoPath;
		kAutoPath.GetPullPath( strFile );
		if( 0 != LUA_DOFILE( m_pRefreshLuaState, strFile.c_str() ) )
		{
			START_LOG( cerr, L"GSScriptRefreshManager : DungeonEnum 정보 로드 실패.!" );
			return;
		}
	}

	// 스레드는 무조건 1개만 생성!
	KThreadManager::Init( 1 );

	// 스레드 시작!
	KThreadManager::BeginThread();

	START_LOG( cout, L"스크립트 리프레쉬 스레드 시작!" );
}

KThread* KGSScriptRefreshManager::CreateThread()
{
	return new KGSScriptRefreshThread;
}

void KGSScriptRefreshManager::RefreshOrderToCnServer( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;
	KncSend( PI_GS_SERVER, GetUID(), PI_CN_SERVER, 0, NULL, ESR_ORDER_TO_CENTER_SERVER_NOT, kNot );
}

void KGSScriptRefreshManager::RefreshOrderToCnServerBroadCast( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, ESR_ORDER_TO_CENTER_SERVER_NOT, kNot );

	SiKProxyManager()->BroadCastToCnServer( *spEvent );
}

void KGSScriptRefreshManager::RefreshOrderToCnServerEachServerGroup( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, ESR_ORDER_TO_CENTER_SERVER_NOT, kNot );

	SiKProxyManager()->BroadCastToCnServerEachServerGroup( *spEvent );
}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
void KGSScriptRefreshManager::RefreshOrderToGBServer( IN int iOrderType )
{
	KESR_SCRIPT_REFRESH_ORDER_NOT kNot;
	kNot.m_iOrderType = iOrderType;
	KncSend( PI_GS_SERVER, GetUID(), PI_GLOBAL_SERVER, 0, NULL, ESR_ORDER_TO_GLOBAL_SERVER_NOT, kNot );
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
void KGSScriptRefreshManager::CompanyIPTable_LUA()
{
	START_LOG( cout, L"CompanyIPTable 클래스 스크립트 실시간 패치!" );

	// 게임서버의 TitleManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_COMPANY_IP_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"CompanyIPTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
//}}

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
void KGSScriptRefreshManager::ClassChangeTable_LUA()
{
	START_LOG( cout, L"ClassChangeTable 클래스 스크립트 실시간 패치!" );

	// 게임서버의 TitleManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_CLASS_CHANGE_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"ClassChangeTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
void KGSScriptRefreshManager::StringFilter_LUA()
{
	START_LOG( cout, L"StringFitler 클래스 스크립트 실시간 패치!" );

	// 게임서버의 String Fitler 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_STRING_FILTER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"StringFitler 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}

//{{ 2012. 04. 25	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
void KGSScriptRefreshManager::WorldMissionManager_IncreaseCrystalCount_LUA( int iIncreaseCrystalNum )
{
	START_LOG( cout, L"WorldMissionManager 보호된 수정의 개수 증가를 요청합니다.");

	KncSend( PI_GS_SERVER, GetUID(), PI_GLOBAL_SERVER, 0, NULL, EGB_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT, iIncreaseCrystalNum );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"WorldMissionManager 보호된 수정의 개수 증가를 요청합니다."));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}

void KGSScriptRefreshManager::WorldMissionManager_ResetIncreaseCrystalCount_LUA( void )
{
	START_LOG( cout, L"WorldMissionManager 보호된 수정의 개수 초기화를 요청합니다.");

	KncSend( PI_GS_SERVER, GetUID(), PI_GLOBAL_SERVER, 0, NULL, EGB_RESET_INCREASE_THE_PROTECTED_CRYSTAL_COUNT_NOT, char() );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"WorldMissionManager 보호된 수정의 개수 초기화를 요청합니다."));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
void KGSScriptRefreshManager::DailyGiftBoxManager_Lua( void )
{
	START_LOG( cout, L"DailyGiftBoxManager 클래스 스크립트 실시간 패치!");
	// 게임서버의 DailyGiftBoxPackage 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_DAILY_GIFT_BOX_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"DailyGiftBoxManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS

}
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
void KGSScriptRefreshManager::SmsManager_Lua( void )
{
	START_LOG( cout, L"KSMSPhoneNumberManager 클래스 스크립트 실시간 패치!");
	
	// 게임서버의 SMSPhoneNumberManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SMS_MANAGER );

	// 센터서버의 SMSPhoneNumberManager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_SMS_MANAGER );

	// 로그인서버의 SMSPhoneNumberManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SMS_MANAGER );

	// 글로벌서버의 SMSPhoneNumberManager 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_SMS_MANAGER );
}
#endif SERV_SMS_TOTAL_MANAGER
//}}

//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
void KGSScriptRefreshManager::BingoEventManager_Lua( void )
{
	START_LOG( cout, L"KGSBingoEventInfo 클래스 스크립트 실시간 패치!");

	// 게임서버의 SMSPhoneNumberManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_BINGO_EVENT_MANAGER );
}
#endif SERV_EVENT_BINGO
//}}

//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
void KGSScriptRefreshManager::LogManager_LUA()
{
	START_LOG( cout, L"KLogManager 클래스 스크립트 실시간 패치!");

	// 게임서버의 KLogManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_LOG_MANAGER );

	// 센터서버의 KLogManager 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_LOG_MANAGER );

	// 로그인서버의 KLogManager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_LOG_MANAGER );

	// 글로벌서버의 KLogManager 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_LOG_MANAGER );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"KLogManager 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}
#endif SERV_LOG_SYSTEM_NEW
//}}

#ifdef SERV_SYNTHESIS_AVATAR
void KGSScriptRefreshManager::SynthesisTable_LUA()
{
	START_LOG( cout, L"SynthesisTable 클래스 스크립트 실시간 패치!" );

	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SYNTHESIS_TABLE );

#ifdef SERV_PROCESS_COMMUNICATION_KSMS
	SiKProcessCommunicationManager()->QueueingProcessWrite(boost::str(boost::wformat(L"%1%_%2%") % 0 %L"SynthesisTable 클래스 스크립트 실시간 패치!"));
#endif //SERV_PROCESS_COMMUNICATION_KSMS
}
#endif SERV_SYNTHESIS_AVATAR

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
void KGSScriptRefreshManager::RidingPetManager_LUA()
{
	START_LOG( cout, L"RidingPetManager 클래스 스크립트 실시간 패치!" );

	// 게임서버의 Riding Pet Manager 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_RIDING_PET_MANAGER );
}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
void KGSScriptRefreshManager::SkillTree_LUA( void )
{
	START_LOG( cout, L"SkillTree 클래스 스크립트 실시간 패치!" );

	// 게임서버의 SkillTree 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_SKILL_TREE );

	// 로그인서버의 SkillTree 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_LG_SKILL_TREE );
}
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
void KGSScriptRefreshManager::FieldBossData_LUA( void )
{
	START_LOG( cout, L"CXSLFieldBossData 클래스 스크립트 실시간 패치!" );

	// 게임서버 갱신 요청!
	RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_FIELD_BOSS_DATA );

	// 센터서버 갱신 요청!
	RefreshOrderToCnServerBroadCast( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_FIELD_BOSS_DATA );

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	// 글로벌서버 갱신 요청!
	RefreshOrderToGBServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GB_FIELD_BOSS_DATA );
#endif SERV_RANDOM_DAY_QUEST
	//}}
}
#endif // SERV_BATTLE_FIELD_BOSS

//#endif SERV_REALTIME_SCRIPT
//}}

#ifdef SERV_GOOD_ELSWORD
void KGSScriptRefreshManager::EDInventoryExpand_LUA()
{
    START_LOG( cout, L"InventoryExpand 클래스 스크립트 실시간 패치!" );
    // 게임서버의 InventoryExpand 갱신 요청!
    RefreshOrderToCnServer( KESR_SCRIPT_REFRESH_ORDER_NOT::OT_GS_ED_INVENTORY_EXPAND );
}
#endif // SERV_GOOD_ELSWORD