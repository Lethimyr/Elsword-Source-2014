#pragma once

#include <KNCSingleton.h>
#include "ThreadManager.h"
#include <KncLua.h>
#include <lua_tinker.h>
using namespace lua_tinker;


#define CASE_SCRIPT_REFRESH_SWAP_INSTANCE( flag, className ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	className::SwapInstance( g_pLua ); \
	START_LOG( cout, L#className L" 스크립트 갱신을 완료하였습니다!" ); \
	Si##className()->DumpToLogFile(); \
	break; \


//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
//#ifdef SERV_REALTIME_SCRIPT


class KGSScriptRefreshManager : public KThreadManager
{
	DeclareSingleton( KGSScriptRefreshManager );
	DeclPfID;

public:
	KGSScriptRefreshManager();
	virtual ~KGSScriptRefreshManager();

	void RegToLua( lua_State* pLuaState );

	// 스크립트 갱신 요청 lua함수
    void ItemManager_LUA();
	void RandomItemManager_LUA();
	void ManufactureItemManager_LUA();
	void RewardTable_LUA();
	void QuestManager_LUA();
	void TitleManager_LUA();
	void DungeonManager_LUA();
	void DropTable_LUA();
	void GameEventScriptManager_LUA();
	void MapData_LUA();
	void GuildManager_LUA();
	void GameSysVal_LUA();
	void ResolveItemManager_LUA();
	void EnchantItemManager_LUA();
	void SocketItem_LUA();
	void AttribEnchantItem_LUA();
	void RecommendUserTable_LUA();
	void ExpTable_LUA();	

	//{{ 2010. 9. 11	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	void PetManager_LUA();
#endif SERV_PET_SYSTEM
	//}}
	void AttribNpcTable_LUA();
	//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	void CnHackingCheckManager_LUA();
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	void MornitoringManager_LUA();
#endif SERV_MORNITORING
	//}}
	void AbuserLogManager_LUA();
	//{{ 2011. 02. 23	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
	void ResultProcess_LUA();
#endif SERV_DUNGEON_RANK_NEW
	//}}

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	void WorldMissionManager_LUA();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가
	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	void PvpMatchManager_LUA();
	void PvpMatchResultTable_LUA();
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 24	최육사	시공 개편
#ifdef SERV_NEW_HENIR_TEST
	void HenirResultTable_LUA();
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	void ContentManager_LUA();
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}

	//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	void CompanyIPTable_LUA();
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	void ClassChangeTable_LUA();
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	void StringFilter_LUA();
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 04. 25	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	void WorldMissionManager_IncreaseCrystalCount_LUA( int iIncreaseCrystalNum );
	void WorldMissionManager_ResetIncreaseCrystalCount_LUA( void );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	void DailyGiftBoxManager_Lua( void );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void BattleFieldManager_LUA();
	void AutoPartyScriptManager_LUA();
	void PartyManager_LUA();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	void SmsManager_Lua( void );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	void	BingoEventManager_Lua( void );
#endif SERV_EVENT_BINGO
	//}}
	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	void BadAttitudeTable_LUA();
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

#ifdef SERV_SYNTHESIS_AVATAR
	void SynthesisTable_LUA();
#endif SERV_SYNTHESIS_AVATAR

	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	void LogManager_LUA();
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	void DefenceDungeonManager_LUA();
	void BuffManager_LUA();
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	void RidingPetManager_LUA( void );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	void SkillTree_LUA( void );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	void	FieldBossData_LUA( void );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    void EDInventoryExpand_LUA();
#endif 
	// derived from KThreadManager
	virtual void Init();
	virtual KThread* CreateThread();

	lua_State* GetRefreshLuaState() { return m_pRefreshLuaState; }

protected:
	void RefreshOrderToCnServer( IN int iOrderType );
	void RefreshOrderToCnServerBroadCast( IN int iOrderType );
	void RefreshOrderToCnServerEachServerGroup( IN int iOrderType );
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	void RefreshOrderToGBServer( IN int iOrderType );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가


protected:
	lua_State* m_pRefreshLuaState;
};

DefSingletonInline( KGSScriptRefreshManager );


//#endif SERV_REALTIME_SCRIPT
//}}