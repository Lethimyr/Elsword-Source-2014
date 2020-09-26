#include "CnScriptRefreshThread.h"
#include "CnScriptRefreshManager.h"

#include "CnRoomManager.h"

// include singleton class header file
#include "X2Data/XSLItemManager.h"
#include "X2Data/XSLQuestManager.h"
#include "X2Data/XSLDungeonManager.h"
#include "KDropTable.h"
#include "GameEvent/GameEventScriptManager.h"
#include "GameSysVal/GameSysVal.h"
#include "KAttribNpcTable.h"
//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	#include "CnHackingCheckManager.h"
#endif SERV_CN_SERVER_HACKING_CHECK
//}}
#include "AbuserLog/KAbuserLogManager.h"
//{{ 2011. 02. 23	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
	#include "ResultProcess.h"
#endif SERV_DUNGEON_RANK_NEW
//}}
//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	#include "PvpMatch/PvpMatchManager.h"
	#include "PvpMatchResultTable.h"
#endif SERV_PVP_NEW_SYSTEM
//}}
//{{ 2011. 08. 24	최육사	시공 개편
#ifdef SERV_NEW_HENIR_TEST
	#include "HenirResultTable.h"
#endif SERV_NEW_HENIR_TEST
//}}
//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "X2Data/XSLBattleFieldManager.h"
	#include "X2Data/XSLPartyManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2012. 10. 9	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	#include "SMSPhoneNumberManager.h"
#endif SERV_SMS_TOTAL_MANAGER
//}}
//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	#include "..\Common\AutoParty\AutoPartyScriptManager.h"
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
//}}
//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	#include "BadAttitudeTable.h"
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}
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

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	#include "X2Data/XSLFieldBossData.h"
#endif // SERV_BATTLE_FIELD_BOSS

#define CASE_REFRESH_SCRIPT( flag, classname )	_CASE_REFRESH_SCRIPT( flag, classname, OpenScriptFile )

#define _CASE_REFRESH_SCRIPT( flag, classname, functionname ) \
case KESR_SCRIPT_REFRESH_ORDER_NOT::flag: \
	START_LOG( cout, L#classname L" 스크립트 갱신을 시작합니다!" ); \
	{ \
		KLocker lock( classname::GetTempCS() ); \
		if( SiTemp##classname()->functionname( SiKCnScriptRefreshManager()->GetRefreshLuaState() ) == false ) \
		{ \
			START_LOG( cerr, L"스크립트 갱신 실패.!" ) \
			<< END_LOG; \
			return; \
		} \
	} \
	break; \


//{{ 2010. 06. 15  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT


#define CLASS_TYPE  KCnScriptRefreshThread

KCnScriptRefreshThread::KCnScriptRefreshThread( void )
{
}

KCnScriptRefreshThread::~KCnScriptRefreshThread( void )
{
	End( 15000 );
}

void KCnScriptRefreshThread::ProcessEvent( const KEventPtr& spEvent_ )
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
void KCnScriptRefreshThread::SendToServer( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, usEventID, data );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

template < class T >
void KCnScriptRefreshThread::SendToCnRoomManager( unsigned short usEventID, const T& data )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_CN_SERVER, NULL, usEventID, data );
	GetKCnRoomManager()->QueueingEventToRoomManager( spEvent );
}

_IMPL_ON_FUNC( ESR_ORDER_TO_REFRESH_MANAGER_REQ, KESR_SCRIPT_REFRESH_ORDER_NOT )
{
	switch( kPacket_.m_iOrderType )
	{
	CASE_REFRESH_SCRIPT( OT_CN_ITEM_MANAGER, CXSLItemManager );
	CASE_REFRESH_SCRIPT( OT_CN_QUEST_MANAGER, CXSLQuestManager );
   _CASE_REFRESH_SCRIPT( OT_CN_DUNGEON_MANAGER, CXSLDungeonManager, OpenScriptFile_AllDungeonScriptLoad );
	CASE_REFRESH_SCRIPT( OT_CN_DROP_TABLE, KDropTable );
	CASE_REFRESH_SCRIPT( OT_CN_GAME_EVENT_SCRIPT_MANAGER, KGameEventScriptManager );
	CASE_REFRESH_SCRIPT( OT_CN_GAME_SYS_VAL, KGameSysVal );
	CASE_REFRESH_SCRIPT( OT_CN_ATTRIB_NPC_TABLE, KAttribNpcTable );
	//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	CASE_REFRESH_SCRIPT( OT_CN_HACKING_CHECK_MANAGER, KCnHackingCheckManager );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	CASE_REFRESH_SCRIPT( OT_CN_ABUSER_LOG_MANAGER, KAbuserLogManager );
	//{{ 2011. 02. 23	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
	CASE_REFRESH_SCRIPT( OT_CN_RESULT_PROCESS, KResultProcess );
#endif SERV_DUNGEON_RANK_NEW
	//}}
	//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	CASE_REFRESH_SCRIPT( OT_CN_PVP_MATCH_MANAGER, KPvpMatchManager );
	CASE_REFRESH_SCRIPT( OT_CN_PVP_MATCH_RESULT_TABLE, KPvpMatchResultTable );
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 24	최육사	시공 개편
#ifdef SERV_NEW_HENIR_TEST
	CASE_REFRESH_SCRIPT( OT_CN_HENIR_RESULT_TABLE, KHenirResultTable );
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _CASE_REFRESH_SCRIPT( OT_CN_BATTLE_FIELD_MANAGER, CXSLBattleFieldManager, OpenScriptFile_AllBattleFieldScriptLoad );
	CASE_REFRESH_SCRIPT( OT_CN_PARTY_MANAGER, CXSLPartyManager );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 10. 9	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
	CASE_REFRESH_SCRIPT( OT_CN_SMS_MANAGER, KSMSPhoneNumberManager );
#endif SERV_SMS_TOTAL_MANAGER
	//}}
	//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	CASE_REFRESH_SCRIPT( OT_CN_AUTO_PARTY_SCRIPT_MANAGER, KAutoPartyScriptManager );
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
	//}}
	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	CASE_REFRESH_SCRIPT( OT_CN_BAD_ATTITUDE_TABLE, KBadAttitudeTable );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	CASE_REFRESH_SCRIPT( OT_CN_LOG_MANAGER, KLogManager );
#endif SERV_LOG_SYSTEM_NEW
	//}}
	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	CASE_REFRESH_SCRIPT( OT_CN_DEFENCE_DUNGEON_MANAGER, CXSLDefenceDungeonManager );
	CASE_REFRESH_SCRIPT( OT_CN_BUFF_MANAGER, CXSLBuffManager );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
	CASE_REFRESH_SCRIPT( OT_CN_FIELD_BOSS_DATA, CXSLFieldBossData );
#endif // SERV_BATTLE_FIELD_BOSS

	default:
		{
			START_LOG( cerr, L"이쪽으로 오면 안되는 타입인데?" )
				<< BUILD_LOG( kPacket_.m_iOrderType )
				<< END_LOG;
		}
		return;
	}

	switch( kPacket_.m_iOrderType )
	{
		//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
	case KESR_SCRIPT_REFRESH_ORDER_NOT::OT_CN_LOG_MANAGER:
		{
			// 요건 CnServer로 보내자!
            SendToServer( ESR_ORDER_TO_REFRESH_MANAGER_ACK, kPacket_ );
		}
		return;
#endif SERV_LOG_SYSTEM_NEW
		//}}

	default:
		break;
	}
	

	// 센터서버의 로직 스레드인 CnRoomManager에서 스크립트를 교체하도록 처리!
	SendToCnRoomManager( ESR_ORDER_TO_REFRESH_MANAGER_ACK, kPacket_ );
}


#endif SERV_REALTIME_SCRIPT
//}}


