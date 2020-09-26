#include "GSScriptRefreshThread.h"
#include "GSScriptRefreshManager.h"

#include "KncSend.h"

#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLRandomItemManager.h"
#include "X2Data/XSLManufactureItemManager.h"
#include "RewardTable.h"
#include "X2Data/XSLQuestManager.h"
#include "X2Data/XSLTitleManager.h"
#include "X2Data/XSLDungeonManager.h"
#include "X2Data/XSLMapData.h"
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
//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
#include "AbuserLog/KAbuserLogManager.h"
//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_EVENT_SCRIPT_REFRESH
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
//{{ 2012. 10. 19	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
	#include "X2Data/XSLPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	#include "Temp.h"
#endif SERV_EVENT_BINGO
//}}

#ifdef SERV_SYNTHESIS_AVATAR
#include "X2Data/XSLSynthesisManager.h"
#endif SERV_SYNTHESIS_AVATAR

//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	#include "LogManager.h"
#endif SERV_LOG_SYSTEM_NEW
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

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	#include "X2Data/XSLSkillTree.h"
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    #include "X2Data/XSLEDInventoryExpand.h"
#endif //SERV_GOOD_ELSWORD

#define CASE_REFRESH_SCRIPT( flag, classname )	_CASE_REFRESH_SCRIPT( flag, classname, OpenScriptFile )

#define _CASE_REFRESH_SCRIPT( flag, classname, functionname ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	START_LOG( cout, L#classname L" 스크립트 갱신을 시작합니다!" ); \
	{ \
		KLocker lock( classname::GetTempCS() ); \
		if( SiTemp##classname()->functionname( SiKGSScriptRefreshManager()->GetRefreshLuaState() ) == false ) \
		{ \
			START_LOG( cerr, L"스크립트 갱신 실패.!" ) \
			<< END_LOG; \
			return; \
		} \
	} \
	break; \


//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT


#define CLASS_TYPE  KGSScriptRefreshThread

KGSScriptRefreshThread::KGSScriptRefreshThread( void )
{
}

KGSScriptRefreshThread::~KGSScriptRefreshThread( void )
{
	End( 15000 );
}

void KGSScriptRefreshThread::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
   _CASE( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT );

	default:
		START_LOG( cerr, L"핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	return;
}

template < class T >
void KGSScriptRefreshThread::SendToServer( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_SERVER, NULL, usEventID, data );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
	CASE_REFRESH_SCRIPT( OT_GS_ITEM_MANAGER, CXSLItemManager );
	CASE_REFRESH_SCRIPT( OT_GS_RANDOM_ITEM_MANAGER, CXSLRandomItemManager );
	CASE_REFRESH_SCRIPT( OT_GS_MANUFACTURE_ITEM_MANAGER, CXSLManufactureItemManager );
	CASE_REFRESH_SCRIPT( OT_GS_REWARD_TABLE, KRewardTable );
	CASE_REFRESH_SCRIPT( OT_GS_QUEST_MANAGER, CXSLQuestManager );
	CASE_REFRESH_SCRIPT( OT_GS_TITLE_MANAGER, CXSLTitleManager );
   _CASE_REFRESH_SCRIPT( OT_GS_DUNGEON_MANAGER, CXSLDungeonManager, OpenScriptFile_AllDungeonScriptLoad );
	CASE_REFRESH_SCRIPT( OT_GS_MAP_DATA, CXSLMapData );
	CASE_REFRESH_SCRIPT( OT_GS_GUILD_MANAGER, CXSLGuildManager );
	CASE_REFRESH_SCRIPT( OT_GS_GAME_SYS_VAL, KGameSysVal );
	CASE_REFRESH_SCRIPT( OT_GS_RESOLVE_ITEM_MANAGER, CXSLResolveItemManager );
	CASE_REFRESH_SCRIPT( OT_GS_ENCHANT_ITEM_MANAGER, CXSLEnchantItemManager );
	CASE_REFRESH_SCRIPT( OT_GS_SOCKET_ITEM, CXSLSocketItem );
	CASE_REFRESH_SCRIPT( OT_GS_ATTRIB_ENCHANT_ITEM, CXSLAttribEnchantItem );
	CASE_REFRESH_SCRIPT( OT_GS_RECOMMEND_USER_TABLE, KRecommendUserTable );
	CASE_REFRESH_SCRIPT( OT_GS_EXP_TABLE, KExpTable );
	//{{ 2010. 9. 11	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	CASE_REFRESH_SCRIPT( OT_GS_PET_MANAGER, CXSLPetManager );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	CASE_REFRESH_SCRIPT( OT_GS_MORNITORING_MANAGER, KMornitoringManager );
#endif SERV_MORNITORING
	//}}
	CASE_REFRESH_SCRIPT( OT_GS_ABUSER_LOG_MANAGER, KAbuserLogManager );
	//{{ 2010. 11. 15	최육사	이벤트 스크립트 실시간 패치
#ifdef SERV_EVENT_SCRIPT_REFRESH
	CASE_REFRESH_SCRIPT( OT_GS_GAME_EVENT_SCRIPT_MANAGER, KGameEventScriptManager );
#endif SERV_EVENT_SCRIPT_REFRESH
	//}}
	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	CASE_REFRESH_SCRIPT( OT_GS_PVP_MATCH_MANAGER, KPvpMatchManager );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
#ifndef SERV_CONTENT_MANAGER_INT
	CASE_REFRESH_SCRIPT( OT_GS_CONTENT_MANAGER, KGSContentManager );
#endif SERV_CONTENT_MANAGER_INT
#endif SERV_CONTENT_MANAGER
	//}}
	//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
	CASE_REFRESH_SCRIPT( OT_GS_COMPANY_IP_TABLE, KCompanyIPTable );
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
	//}}
	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	CASE_REFRESH_SCRIPT( OT_GS_CLASS_CHANGE_TABLE, KClassChangeTable );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	CASE_REFRESH_SCRIPT( OT_GS_STRING_FILTER, CXSLStringFilter );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}

	//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	CASE_REFRESH_SCRIPT( OT_GS_DAILY_GIFT_BOX_MANAGER, KDailyGiftBoxManager );
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}

	//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_REFRESH_SCRIPT( OT_GS_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2012. 10. 19	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _CASE_REFRESH_SCRIPT( OT_GS_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager, OpenScriptFile_AllBattleFieldScriptLoad );
	CASE_REFRESH_SCRIPT( OT_GS_PARTY_MANAGER, CXSLPartyManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	CASE_REFRESH_SCRIPT( OT_GS_BINGO_EVENT_MANAGER, KGSBingoEventInfo );
#endif SERV_EVENT_BINGO
	//}}

#ifdef SERV_SYNTHESIS_AVATAR
	CASE_REFRESH_SCRIPT( OT_GS_SYNTHESIS_TABLE, CXSLSynthesisManager );
#endif SERV_SYNTHESIS_AVATAR

	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_REFRESH_SCRIPT( OT_GS_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CASE_REFRESH_SCRIPT( OT_GS_DEFENCE_DUNGEON_MANAGER, CXSLDefenceDungeonManager );
	CASE_REFRESH_SCRIPT( OT_GS_BUFF_MANAGER, CXSLBuffManager );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	CASE_REFRESH_SCRIPT( OT_GS_RIDING_PET_MANAGER, CXSLRidingPetManager );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
	CASE_REFRESH_SCRIPT( OT_GS_SKILL_TREE, CXSLSkillTree );
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	CASE_REFRESH_SCRIPT( OT_GS_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
    CASE_REFRESH_SCRIPT( OT_GS_ED_INVENTORY_EXPAND, CXSLEDInventoryExpand );
#endif //SERV_GOOD_ELSWORD
	default:
		{
			START_LOG( cerr, L"이쪽으로 오면 안되는 타입인데?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		return;
	}
	
	// 로직 스레드에서 교체하도록 처리!
	SendToServer( ESR_ORDER_TO_REFRESH_MANAGER_ACK, kPacket_ );
}

#endif SERV_REALTIME_SCRIPT
//}}


