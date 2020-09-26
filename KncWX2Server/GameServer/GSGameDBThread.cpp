#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSGameDBThread.h"
#include "GameServer.h"
#include "NetError.h"
#include "Inventory.h"      // 장착 아이템 얻어올 때 InventoryCategory 필요.
#include "StatTable.h"
#include "ExpTable.h"
#include "X2Data/XSLItem.h"
#include "GSSimLayer.h"
#include "TutorialManager.h"
#include "X2Data/XSLAttribEnchantItem.h"
#include "X2Data/XSLUnit.h"
//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	#include "X2Data/XSLSocketItem.h"
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_CODE_EVENT
#include "ScriptID_Code.h"
#endif SERV_CODE_EVENT

#ifdef SERV_USE_GM_TOOL_INFO
#include <boost/algorithm/string/replace.hpp>
#endif //SERV_USE_GM_TOOL_INFO

//ImplementDBThread( KGSGameDBThread );

#define CLASS_TYPE KGSGameDBThread

IMPL_PROFILER_DUMP( KGSGameDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.P_QueryStats_INS", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.gup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#endif //SERV_ALL_RENEWAL_SP
		continue;

end_proc:
		START_LOG( cerr, vecDump[ui].m_wstrQuery )
			<< BUILD_LOG( vecDump[ui].m_iMinTime )
			<< BUILD_LOG( vecDump[ui].m_iMaxTime )
			<< BUILD_LOG( vecDump[ui].m_iTotalTime )
			<< BUILD_LOG( vecDump[ui].m_iQueryCount )
			<< BUILD_LOG( iAvg )
			<< BUILD_LOG( vecDump[ui].m_iOver1Sec )
			<< BUILD_LOG( vecDump[ui].m_iQueryFail )
			<< END_LOG;
	}
}

KGSGameDBThread::~KGSGameDBThread(void)
{
}

void KGSGameDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
        CASE( DBE_UPDATE_UNIT_INFO_REQ );
		_CASE( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, std::wstring );
	   //{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
	   CASE( DBE_GAME_CREATE_UNIT_REQ );
#else
	   _CASE( DBE_GAME_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_REQ );
#endif SERV_NEW_CREATE_CHAR_EVENT
	   //}}
       _CASE( DBE_GAME_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_REQ );
	   //{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
	   _CASE( DBE_GAME_FINAL_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_REQ );
	   _CASE( DBE_GAME_RESTORE_UNIT_REQ, KEGS_RESTORE_UNIT_REQ );
#endif SERV_UNIT_WAIT_DELETE
	   //}}
       _CASE( DBE_GAME_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_REQ );
		//{{ 2009. 5. 28  최육사	채널이동
		CASE( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ );
		//}}

	   _CASE( DBE_ADMIN_MODIFY_UNIT_LEVEL_REQ, KEGS_ADMIN_MODIFY_UNIT_LEVEL_REQ );
	   _CASE( DBE_ADMIN_CHANGE_ED_REQ, KEGS_ADMIN_CHANGE_ED_REQ );
        CASE( DBE_BUY_ED_ITEM_REQ );
        CASE( DBE_INSERT_ITEM_REQ );
		CASE( DBE_NEW_QUEST_REQ );
		CASE( DBE_GIVE_UP_QUEST_REQ );
		CASE( DBE_QUEST_COMPLETE_REQ );
		CASE( DBE_INSERT_SKILL_REQ );

		//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_FIX_THE_ADMIN_CHEAT
		_CASE( DBE_ADMIN_INIT_SKILL_TREE_REQ, KDBE_INIT_SKILL_TREE_REQ );
#else
		_CASE( DBE_ADMIN_INIT_SKILL_TREE_REQ, UidType );
#endif SERV_FIX_THE_ADMIN_CHEAT
		//}}

       _CASE( DBE_SEARCH_UNIT_REQ, std::wstring );
	   _CASE( DBE_KNM_REQUEST_NEW_FRIEND_INFO_REQ, std::wstring );
	    CASE( DBE_INSERT_TRADE_ITEM_REQ );
		//{{ 2009. 2. 10  최육사	개인거래 버그 예방
		CASE( DBE_INSERT_TRADE_ITEM_BY_SERVER_NOT );
		//}}
       _CASE( DBE_INSERT_PURCHASED_CASH_ITEM_REQ, KDBE_INSERT_ITEM_REQ );
	    CASE( DBE_INSERT_PURCHASED_CASH_PACKAGE_REQ );
		
		CASE( DBE_OPEN_RANDOM_ITEM_REQ );
		CASE( DBE_ITEM_MANUFACTURE_REQ );

	   _CASE( DBE_CHANGE_NICK_NAME_REQ, KEGS_CHANGE_NICK_NAME_REQ );

		CASE( DBE_NEW_BLACKLIST_USER_REQ );
		CASE( DBE_DEL_BLACKLIST_USER_REQ );

		//{{ 2009. 9. 22  최육사	전직캐쉬		
		CASE( DBE_CHANGE_UNIT_CLASS_REQ );
		//}}

		CASE( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ );

        CASE( DBE_ENCHANT_ITEM_REQ );
		//{{ 2008. 12. 21  최육사	강화 복구
		CASE( DBE_RESTORE_ITEM_REQ );
		//}}
		CASE( DBE_RESOLVE_ITEM_REQ );
		CASE( DBE_SOCKET_ITEM_REQ );

		CASE( DBE_INIT_SKILL_TREE_REQ );
		
		CASE( DBE_RECOMMEND_USER_REQ );
		CASE( DBE_INSERT_REWARD_TO_POST_REQ );
		CASE( DBE_PREPARE_INSERT_LETTER_TO_POST_REQ );
		CASE( DBE_INSERT_LETTER_TO_POST_REQ );
		
		//{{ 2008. 9. 18  최육사	우편함
		CASE( DBE_GET_POST_LETTER_LIST_REQ );
		CASE( DBE_READ_LETTER_NOT );
		CASE( DBE_GET_ITEM_FROM_LETTER_REQ );
		CASE( DBE_DELETE_LETTER_NOT );
		//}}		

		CASE( DBE_DEL_TUTORIAL_REQ );
		CASE( DBE_INSERT_TUTORIAL_REQ );

		CASE( DBE_RESET_SKILL_REQ );
        CASE( DBE_EXPAND_INVENTORY_SLOT_REQ );
		CASE( DBE_EXPAND_SKILL_SLOT_REQ );
		//{{ 2008. 12. 14  최육사	캐릭터 슬롯 확장
		CASE( DBE_EXPAND_CHAR_SLOT_REQ );
		//}}
		
		CASE( DBE_GET_WISH_LIST_REQ );
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
		CASE( DBE_UPDATE_EVENT_TIME_REQ );
#endif //SERV_ADD_EVENT_DB
		
		CASE( DBE_UPDATE_INVENTORY_ITEM_POS_NOT );
		CASE( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ );

		//{{ 2008. 9. 3  최육사		속성강화
		CASE( DBE_ATTRIB_ENCHANT_ITEM_REQ );
		CASE( DBE_IDENTIFY_ITEM_REQ );
		//}}

		//{{ 2008. 9. 26  최육사	우체국 블랙리스트
		CASE( DBE_NEW_POST_BLACK_LIST_REQ );
		CASE( DBE_DEL_POST_BLACK_LIST_REQ );
		//}}

		//{{ 2008. 10. 7  최육사	타이틀
		CASE( DBE_INSERT_TITLE_REQ );
		//}}

		//{{ 2008. 12. 25  최육사	부여
		CASE( DBE_ENCHANT_ATTACH_ITEM_REQ );
		//}}

		//{{ 2008. 11. 18  최육사	아이템 교환
		CASE( DBE_ITEM_EXCHANGE_REQ );
		//}}

		//{{ 2009. 4. 8  최육사		닉네임 변경
		CASE( DBE_DELETE_NICK_NAME_REQ );
		//}}

		CASE( DBE_INSERT_CASH_SKILL_POINT_REQ );
		CASE( DBE_EXPIRE_CASH_SKILL_POINT_REQ );

		//{{ 2009. 8. 4  최육사		봉인 스킬
		CASE( DBE_UNSEAL_SKILL_REQ );
		//}}

		//{{ 2009. 5. 11  최육사	실시간 아이템
		CASE( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ );
		CASE( DBE_GET_TEMP_ITEM_REQ );
		//}}

        CASE( DBE_REQUEST_FRIEND_REQ );
        CASE( DBE_ACCEPT_FRIEND_REQ );
        CASE( DBE_DENY_FRIEND_REQ );
        CASE( DBE_BLOCK_FRIEND_REQ );
        CASE( DBE_UNBLOCK_FRIEND_REQ );
        CASE( DBE_DELETE_FRIEND_REQ );
        CASE( DBE_MOVE_FRIEND_REQ );
        CASE( DBE_MAKE_FRIEND_GROUP_REQ );
        CASE( DBE_RENAME_FRIEND_GROUP_REQ );
        CASE( DBE_DELETE_FRIEND_GROUP_REQ );
        CASE( DBE_FRIEND_MESSAGE_NOT );

		//{{ 2009. 7. 29  최육사	item set cheat
		CASE( DBE_ADMIN_GET_ITEM_SET_NOT );
		//}}

		//{{ 2009. 8. 27  최육사	봉인
		CASE( DBE_SEAL_ITEM_REQ );
		CASE( DBE_UNSEAL_ITEM_REQ );
		//}}

		//////////////////////////////////////////////////////////////////////////		
		//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST
		
		//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
#else
		CASE( DBE_CREATE_GUILD_REQ );
#endif SERV_GUILD_CHANGE_NAME
		//}}

	   _CASE( DBE_INVITE_GUILD_NICKNAME_CHECK_REQ, KELG_INVITE_GUILD_ACK );
		CASE( DBE_JOIN_GUILD_REQ );

		CASE( DBE_CHANGE_GUILD_MEMBER_GRADE_REQ );
		CASE( DBE_CHANGE_GUILD_MESSAGE_REQ );
		CASE( DBE_KICK_GUILD_MEMBER_REQ );
		CASE( DBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ );

		CASE( DBE_DISBAND_GUILD_REQ );
		CASE( DBE_EXPAND_GUILD_MAX_MEMBER_REQ );

		//{{ 2009. 10. 27  최육사	길드레벨
		CASE( DBE_UPDATE_GUILD_EXP_REQ );	   
		//}}

#endif GUILD_TEST
		//}}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

		CASE( DBE_RESET_GUILD_SKILL_REQ );
		CASE( DBE_INIT_GUILD_SKILL_REQ );

#endif GUILD_SKILL_TEST
		//}}
		//////////////////////////////////////////////////////////////////////////

		//{{ 2009. 12. 8  최육사	크리스마스이벤트
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
		CASE( DBE_CHECK_TIME_EVENT_COMPLETE_REQ );
#endif //SERV_ADD_EVENT_DB
		//}}

#ifdef SERV_GLOBAL_BILLING
		//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
		CASE( DBE_GET_NICKNAME_BY_UNITUID_REQ );
		//}} 허상형 : [2010/8/19/] //	선물 기능 개편
#endif // SERV_GLOBAL_BILLING

		//{{ 2010. 01. 11  최육사	추천인리스트
		CASE( DBE_GET_RECOMMEND_USER_LIST_REQ );
		//}}

		//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
		CASE( DBE_ATTENDANCE_CHECK_REQ );
	   _CASE( DBE_INCREASE_WEB_POINT_LOG_NOT, KDBE_INCREASE_WEB_POINT_ACK );	   
#endif SERV_WEB_POINT_EVENT
		//}}		
		//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
		CASE( DBE_EXPAND_SKILL_NOTE_PAGE_REQ );
		CASE( DBE_REG_SKILL_NOTE_MEMO_REQ );
#endif SERV_SKILL_NOTE
		//}}
		//{{ 2010. 7. 30 최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		CASE( DBE_CREATE_PET_REQ );
		CASE( DBE_SUMMON_PET_REQ );

		//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
		CASE( DBE_CHANGE_PET_NAME_REQ );
#endif SERV_PET_CHANGE_NAME
		//}}

#endif SERV_PET_SYSTEM
		//}}

		//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT
		CASE( DBE_RESET_PERIOD_ITEM_REQ );
#endif SERV_RESET_PERIOD_EVENT
		//}}
		//{{ 2011. 01. 04	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM
		CASE( DBE_ATTRIB_ATTACH_ITEM_REQ );
#endif SERV_ATTRIBUTE_CHARM
		//}}
		//{{ 2011. 04. 14	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		CASE( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ );
		CASE( DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ );
		CASE( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ );
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 지헌 - 은행 개편 퀘스트, 캐쉬 작업 
#ifdef SERV_SHARING_BANK_QUEST_CASH
		CASE( DBE_SHARING_BACK_OPEN_REQ);
#endif
		//}}
#ifdef SERV_SHARING_BANK_EVENT
		CASE( DBE_SHARING_BANK_EVENT_REQ );
#endif
#ifdef	SERV_SHARING_BANK_TEST
		_CASE( DBE_GET_SHARE_BANK_REQ, KEGS_GET_SHARE_BANK_REQ );
		CASE( DBE_UPDATE_SHARE_ITEM_REQ );
		_CASE( DBE_UPDATE_SHARE_ITEM_FOR_MOVE_SLOT_REQ, KDBE_UPDATE_SHARE_ITEM_REQ );
		_CASE( DBE_UPDATE_SEAL_DATA_NOT, KDBE_SEAL_ITEM_REQ );

		CASE( DBE_CHANGE_INVENTORY_SLOT_ITEM_REQ );
#endif	SERV_SHARING_BANK_TEST

#ifdef GIANT_RESURRECTION_CASHSTONE
		CASE( DBE_REALTIME_RESURRECTION_CASHSTONE_NOT );
		CASE( DBE_RESURRECTION_CASHSTONE_NOT );
#endif //GIANT_RESURRECTION_CASHSTONE

#ifdef SERV_ADVERTISEMENT_EVENT
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
		CASE( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT );
#endif //SERV_ADD_EVENT_DB
#endif SERV_ADVERTISEMENT_EVENT

		//{{ 2011. 08. 03	최육사	대전 강제 종료에 대한 예외처리
#ifdef SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
		CASE( DBE_QUIT_USER_PVP_RESULT_UPDATE_NOT );
#endif SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
		//}}
		//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
		CASE( DBE_RETAINING_SELECT_REWARD_REQ );
#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
		//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
		CASE( DBE_USE_ITEM_IN_INVENTORY_REQ );
#endif SERV_USE_ITEM_DB_UPDATE_FIX
		//}}
		//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
		CASE( DBE_BUY_UNIT_CLASS_CHANGE_REQ );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
		//}}
		//{{ 2012. 04. 30	박세훈	현자의 주문서 접속 이벤트 ( 우편함 중복 체크 )
#ifdef SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
		_CASE( DBE_UPDATE_EVENT_TIME_NOT, KDBE_UPDATE_EVENT_TIME_REQ );
#endif SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
		//}}
		//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
		CASE( DBE_TRADE_COMPLETE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
		//}}
		//{{ 2012. 05. 30	최육사	그노시스 기간 치트
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
		CASE( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ );
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
		//}}
		//{{ 2012. 07. 25	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
		CASE( DBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ );
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
		//}}

		//{{ 우편 ED 거래시 ED 동기화 - 김민성
#ifdef SERV_SEND_LETTER_BEFOR_ED_SYNC
		CASE( DBE_SYNC_ED_REQ );
#endif SERV_SEND_LETTER_BEFOR_ED_SYNC
		//}}

		//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		CASE_NOPARAM( DBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ );
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}

		//{{ 2012. 08. 21	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		CASE( DBE_GET_ITEM_FROM_LETTER_ARRANGE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}

		//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
		CASE( DBE_PVP_WIN_EVENT_CHECK_REQ );
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		_CASE( DBE_NEW_ACCOUNT_QUEST_GAME_DB_REQ, KDBE_NEW_QUEST_REQ );
		_CASE( DBE_ACCOUNT_QUEST_COMPLETE_GAME_DB_REQ, KDBE_ACCOUNT_QUEST_COMPLETE_ACK );
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
		CASE( DBE_PET_AUTO_LOOTING_NOT );
#endif SERV_PET_AUTO_LOOTING
		//}}
		//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
		CASE( DBE_OPEN_SYNTHESIS_ITEM_REQ );
#endif SERV_SYNTHESIS_AVATAR
		//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
		CASE( DBE_CHANGE_PET_ID_REQ );
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

		//{{ 2013. 3. 11	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
		CASE( DBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ );
		CASE( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ );
		CASE( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}
		//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
		CASE( DBE_RECOMMEND_USER_GET_NEXON_SN_REQ );
#endif SERV_RECOMMEND_LIST_EVENT
		//}
#ifdef SERV_ADD_WARP_BUTTON
		CASE( DBE_INSERT_WARP_VIP_REQ );
#endif // SERV_ADD_WARP_BUTTON
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		_CASE( DBE_COUPLE_PROPOSE_USER_FIND_REQ, KEGS_COUPLE_PROPOSE_REQ );
		CASE( DBE_COUPLE_MAKING_SUCCESS_REQ );
		CASE( DBE_WEDDING_PROPOSE_REQ );
		_CASE( DBE_CHECK_NICK_NAME_REQ, KEGS_CHECK_NICK_NAME_REQ );
		_CASE( DBE_WEDDING_ITEM_INFO_REQ, KEGS_WEDDING_ITEM_INFO_REQ );
		CASE( DBE_WEDDING_ITEM_DELETE_REQ );
		CASE( DBE_BREAK_UP_REQ );
		CASE( DBE_ADMIN_CHANGE_COUPLE_DATE_REQ );
		CASE( DBE_CHANGE_LOVE_WORD_REQ );
		CASE( DBE_INSERT_WEDDING_REWARD_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
		//}

#ifdef SERV_MOMOTI_EVENT
		CASE( DBE_MOMOTI_QUIZ_EVENT_REQ );
#endif SERV_MOMOTI_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		CASE( DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ );
#endif // SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_PERIOD_PET
		CASE( DBE_RELEASE_PET_REQ );
#endif SERV_PERIOD_PET

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		CASE( DBE_GET_RIDING_PET_LIST_REQ );
		CASE( DBE_CREATE_RIDING_PET_REQ );
		CASE( DBE_RELEASE_RIDING_PET_REQ );
#endif	// SERV_RIDING_PET_SYSTM

		//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		CASE( DBE_ITEM_EVALUATE_REQ );
		CASE( DBE_RESTORE_ITEM_EVALUATE_REQ );
		CASE( DBE_ITEM_CONVERT_REQ );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// 작업날짜: 2013-05-13	// 박세훈
		CASE( DBE_SEND_LOVE_LETTER_EVENT_REQ );
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		CASE( DBE_ADMIN_CHANGE_UNIT_CLASS_REQ );
		CASE( DBE_ADMIN_AUTO_GET_ALL_SKILL_REQ );
		CASE( DBE_ADMIN_GET_SKILL_REQ );
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈
		CASE( DBE_JUMPING_CHARACTER_UPDATE_REQ );
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_COUPON_EVENT
		CASE( DBE_COUPON_ENTRY_REQ );
#endif SERV_COUPON_EVENT

#ifdef SERV_READY_TO_SOSUN_EVENT
		CASE( DBE_READY_TO_SOSUN_EVENT_REQ );
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		_CASE( DBE_EVENT_PROPOSE_USER_FIND_REQ, KEGS_USE_PROPOSE_ITEM_REQ );
		CASE( DBE_EVENT_MAKING_SUCCESS_REQ );
		CASE( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ );
		CASE( DBE_EVENT_DIVORCE_REQ );
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
		CASE( DBE_USE_RECRUIT_TICKET_REQ );
		CASE( DBE_REGISTER_RECRUITER_REQ );
		CASE( DBE_GET_RECRUIT_RECRUITER_LIST_REQ );
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
		CASE_NOPARAM( DBE_GET_EVENT_INFO_REQ );
		CASE( DBE_SET_EVENT_INFO_NOT );
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_NEW_YEAR_EVENT_2014
		CASE( DBE_2013_EVENT_MISSION_COMPLETE_REQ );
		CASE( DBE_2014_EVENT_MISSION_COMPLETE_REQ );
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_USE_GM_TOOL_INFO
		CASE( DBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT );
#endif //SERV_USE_GM_TOOL_INFO

#ifdef SERV_GLOBAL_MISSION_MANAGER
		CASE( DBE_REGIST_GLOBAL_MISSION_CLEAR_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef SERV_EVENT_CHECK_POWER
		CASE( DBE_START_CHECK_POWER_REQ );
		_CASE( DBE_UPDATE_CHECK_POWER_REQ, KDBE_START_CHECK_POWER_REQ );
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
		CASE( DBE_ITEM_EXTRACT_REQ );
		CASE( DBE_USE_FINALITY_SKILL_REQ );
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_GOOD_ELSWORD
        CASE( DBE_EXPAND_BANK_INVENTORY_REQ );
#endif // SERV_GOOD_ELSWORD

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		CASE( DBE_SOCKET_EXPAND_ITEM_REQ );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_SKILL_PAGE_SYSTEM
		CASE( DBE_EXPAND_SKILL_PAGE_REQ );
		CASE( DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ );
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_ENTRY_POINT 
        _CASE( DBE_CHARACTER_LIST_REQ, std::wstring );
        _CASE( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, KEGS_ENTRY_POINT_CHECK_NICK_NAME_REQ );
        CASE( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ );
#endif SERV_ENTRY_POINT

#ifdef SERV_EVENT_PET_INVENTORY
		CASE( DBE_EVENT_PET_EVENT_FOOD_EAT_REQ );
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		CASE( DBE_EVENT_CHUNG_GIVE_ITEM_REQ );
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		CASE( DBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ );
		CASE( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT );
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		CASE( DBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_REQ );
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

//{{ 2009. 5. 28  최육사	채널이동
//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
bool KGSGameDBThread::Query_SelectUnit( IN const UidType iUserUID, IN const KEGS_SELECT_UNIT_REQ& kReq, OUT KDBE_SELECT_UNIT_ACK& kAck )
#else
bool KGSGameDBThread::Query_SelectUnit( IN const UidType iUserUID, IN const KEGS_SELECT_UNIT_REQ& kReq, OUT KEGS_SELECT_UNIT_ACK& kAck )
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}}
{
	kAck.m_kUnitInfo.Init();
	kAck.m_iOK = NetError::ERR_ODBC_01;
	std::map< UidType, KInventoryItemInfo >::iterator mit;
	//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	std::map< UidType, UidType > mapPetItemList;
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011.10.18     김민성    칭호 중복 진행(완료,수행) 예외 처리
#ifdef SERV_TITLE_DUPLICATE_PROCESS_REVISION
	std::vector< KTitleInfo >::iterator vitTitle;
#endif SERV_TITLE_DUPLICATE_PROCESS_REVISION
	//}}
	//{{ 허상형 : [2010/8/31/] //	부활석 통계 분리 작업
#ifdef SERV_SELECT_UNIT_NEW
	KELOG_UPDATE_STONE_NOT kResNot;
#endif	//	SERV_SELECT_UNIT_NEW
	//}} 허상형 : [2010/8/31/] //	부활석 통계 분리 작업

#ifdef SERV_ADD_WARP_BUTTON
	std::wstring wstrVipEndDate;
	std::wstring wstrVipRegDate;
#endif // SERV_ADD_WARP_BUTTON

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	std::vector< UidType > vecWeddingItem;
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	int iResetHour = 6;
#ifdef SERV_COUNTRY_US
		iResetHour = 3;
#endif //SERV_COUNTRY_US
#endif //SERV_ALL_RENEWAL_SP

	// 케릭터 로그인 정보 체크
	//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL", L"%d, %d, %d", % kReq.m_iUnitUID % iUserUID % iResetHour );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_select_unit_new", L"%d, %d", % kReq.m_iUnitUID % iUserUID );
#endif //SERV_ALL_RENEWAL_SP
#else
	DO_QUERY( L"exec dbo.gup_select_unit", L"%d", % kReq.m_iUnitUID );
#endif SERV_POST_COPY_BUG_FIX
	//}}	
	if( m_kODBC.BeginFetch() )
	{
		//{{ 2011. 01. 18	최육사	캐릭터 카운트 정보
//#ifdef SERV_CHAR_LOG
		//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
#ifdef SERV_SELECT_UNIT_NEW
		FETCH_DATA( iOK
			>> kAck.m_wstrUnitCreateDate
			>> kAck.m_wstrUnitLastLoginDate
			>> kResNot.m_iSupplyCnt
			>> kResNot.m_iQuantity );
#else
		FETCH_DATA( iOK
			>> kAck.m_wstrUnitCreateDate
			>> kAck.m_wstrUnitLastLoginDate );
#endif
#else
		FETCH_DATA( iOK
			>> kAck.m_wstrUnitCreateDate );
#endif SERV_DAILY_CHAR_FIRST_SELECT
		//}}		
//#else
//		FETCH_DATA( iOK );
//#endif SERV_CHAR_LOG
		//}}		
		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

		START_LOG( cerr, L"gup_select_unit 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	if( iOK != NetError::NET_OK )
	{
		//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
		switch( iOK )
		{
		case -10:	
            kAck.m_iOK = NetError::ERR_SELECT_UNIT_07;
            kAck.m_nWrongUnitUID = kReq.m_iUnitUID; // 소속이 잘못된 캐릭터 선택시도
            break;
		default:	kAck.m_iOK = NetError::ERR_SELECT_UNIT_00;	break;
		}
#else
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_00;
#endif SERV_POST_COPY_BUG_FIX
		//}}		

		START_LOG( cerr, L"gup_select_unit 호출 실패!" )
			<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	//{{ 허상형 : [2010/8/31/] //	부활석 통계 분리 작업
#ifdef SERV_SELECT_UNIT_NEW
	// 캐릭터 로그인 정보 체크가 완료되면 Log DB에 부활석 리필 숫자 저장(기준치보다 보유량이 적을 경우만 저장)
	if( kResNot.m_iQuantity < kResNot.m_iSupplyCnt )
	{
		// 지헌 : 수정 - 왜 로그인 서버로 보내고있지? 로그인 디비로 보내도록 수정
		SendToLogDB( ELOG_UPDATE_STONE_NOT, kResNot );
	}
#endif	//	SERV_SELECT_UNIT_NEW
	//}} 허상형 : [2010/8/31/] //	부활석 통계 분리 작업

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 10. 28  최육사	휴면유저 이벤트
#ifdef SERV_COMEBACK_EVENT

	DO_QUERY( L"exec dbo.gup_select_event_unit", L"%d, %d", % iUserUID % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"휴면 유저 이벤트 보상 호출 실패!" )
			<< BUILD_LOG( iUserUID )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		// 호출 실패하더라도 캐릭터 선택은 되게 하자!
	}

#endif SERV_COMEBACK_EVENT
	//}}
	//////////////////////////////////////////////////////////////////////////

	// 캐릭터 정보를 얻는다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUnitUID", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_info_by_unituid", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		//{{ 2012. 06. 11	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		int iMapIDDummy = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		FETCH_DATA( kAck.m_kUnitInfo.m_nUnitUID
			>> kAck.m_kUnitInfo.m_cUnitClass
			>> kAck.m_kUnitInfo.m_iEXP
			>> kAck.m_kUnitInfo.m_ucLevel
			>> kAck.m_kUnitInfo.m_iED
			>> kAck.m_kUnitInfo.m_kStat.m_iBaseHP
			>> kAck.m_kUnitInfo.m_kStat.m_iAtkPhysic
			>> kAck.m_kUnitInfo.m_kStat.m_iAtkMagic
			>> kAck.m_kUnitInfo.m_kStat.m_iDefPhysic
			>> kAck.m_kUnitInfo.m_kStat.m_iDefMagic
			>> kAck.m_kUnitInfo.m_iSPoint	/// kimhc // 2013-12-02 // 사용 안 함
			>> kAck.m_kUnitInfo.m_wstrNickName
			>> iMapIDDummy
			>> kAck.m_kUnitInfo.m_iSpirit
			>> kAck.m_bIsSpiritUpdated
			>> kAck.m_iRecommendUnitUID
			);
#else
		FETCH_DATA( kAck.m_kUnitInfo.m_nUnitUID
			>> kAck.m_kUnitInfo.m_cUnitClass
			>> kAck.m_kUnitInfo.m_iEXP
			>> kAck.m_kUnitInfo.m_ucLevel
			>> kAck.m_kUnitInfo.m_iED
			>> kAck.m_kUnitInfo.m_iVSPoint			
			>> kAck.m_kUnitInfo.m_iVSPointMax
			>> kAck.m_kUnitInfo.m_kStat.m_iBaseHP
			>> kAck.m_kUnitInfo.m_kStat.m_iAtkPhysic
			>> kAck.m_kUnitInfo.m_kStat.m_iAtkMagic
			>> kAck.m_kUnitInfo.m_kStat.m_iDefPhysic
			>> kAck.m_kUnitInfo.m_kStat.m_iDefMagic
			>> kAck.m_kUnitInfo.m_iSPoint
			>> kAck.m_kUnitInfo.m_wstrNickName
			>> kAck.m_kUnitInfo.m_iWin				
			>> kAck.m_kUnitInfo.m_iLose
			>> kAck.m_kUnitInfo.m_nMapID
			>> kAck.m_kUnitInfo.m_iSpirit
			>> kAck.m_bIsSpiritUpdated
			>> kAck.m_iRecommendUnitUID
			);
#endif SERV_PVP_NEW_SYSTEM
		//}}

		m_kODBC.EndFetch();

		kAck.m_iOK = NetError::NET_OK;
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_00;

		START_LOG( cerr, L"gup_get_unit_info_by_unituid 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	DO_QUERY( L"exec dbo.P_GSkill_Expand_SEL", L"%d", % kReq.m_iUnitUID );
	
	/// kimhc // 김현철 // 2013-11-29
	/// 스킬 페이지를 확장하지 않았다면 테이블이 없을 것이고
	/// 전달인자
	/// @iUnitUID
	/// 리턴 인자
	/// SKILLPageCNT - 확장한 페이지 수. 1이면 총 2페이지, 2이면 총 3페이지의 스킬 페이지를 보유
	/// ActivePage - 현재 활성화 되어 있는 스킬 페이지 (1부터 시작)
	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber
			);

		if ( kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber < 1 ||
			kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber > kAck.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable )
		{
			START_LOG( cerr, L"현재 사용 중인 페이지 이상함" )
				<< BUILD_LOG( kAck.m_iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber )
				<< BUILD_LOG( kAck.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable )
				<< END_LOG;

			kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber = 1;
		}
	}
	else
	{
		kAck.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable = 1;
		kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber = 1;
	}
#endif // SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT// 작업날짜: 2013-06-25	// 박세훈
	if( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>( kAck.m_kUnitInfo.m_cUnitClass ) ) == CXSLUnit::UT_ADD )
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_01;
		return false;
	}
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	DO_QUERY( L"exec dbo.P_GEvent_Spirit_SEL", L"%d", % kReq.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA(	kAck.m_kUnitInfo.m_iAccumulationSpirit );
		m_kODBC.EndFetch();
	}
	else
	{
		// 존재하지 않을 수도 있다. 데이터가 없으면 그럴 경우 0으로 대입
		kAck.m_kUnitInfo.m_iAccumulationSpirit = 0;

		//START_LOG( cerr, L"P_GEvent_Spirit_SEL 호출 실패!" )
		//	<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
		//	<< BUILD_LOG( kReq.m_iUnitUID )
		//	<< END_LOG;

		//return false;
	}

	START_LOG( cerr, L"김석근_짐승남근성도확인_SEL" )
		<< BUILD_LOG( kAck.m_kUnitInfo.m_iAccumulationSpirit )
		<< END_LOG;
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

	//////////////////////////////////////////////////////////////////////////
	//{{ 2012. 02. 02	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 캐릭터의 마지막 위치 정보 가져오기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitLastPosition_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitLastPosition_GET", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_kLastPos.m_iMapID
			>> kAck.m_kUnitInfo.m_kLastPos.m_ucLastTouchLineIndex
			>> kAck.m_kUnitInfo.m_kLastPos.m_usLastPosValue
			);
		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

		START_LOG( cerr, L"P_GUnitLastPosition_SEL 호출 실패!" )
			<< BUILD_LOG( NetError::GetErrStr( kAck.m_iOK ) )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	// 캐릭터의 마지막 플레이 상태값 가져오기!
	// 해당 캐릭터의 마지막 플레이 상태값 저장!
	DO_QUERY( L"exec dbo.P_GUnitPlayInfo_SEL", L"%d", % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		KGamePlayStatus kInfo;
		const CXSLUnit::UNIT_TYPE eUnitType = CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>(kAck.m_kUnitInfo.m_cUnitClass) );
		switch( eUnitType )
		{
		case CXSLUnit::UT_ELSWORD:
#ifdef SERV_NEW_CHARACTER_EL
		case CXSLUnit::UT_ELESIS:
#endif // SERV_NEW_CHARACTER_EL
			kInfo.SetCharAbilType( KGamePlayStatus::CAC_WSP );
			break;

		case CXSLUnit::UT_CHUNG:
			kInfo.SetCharAbilType( KGamePlayStatus::CAC_CANNON_BALL_COUNT );
			break;

		case CXSLUnit::UT_ARA:
			kInfo.SetCharAbilType( KGamePlayStatus::CAC_FORCE_POWER );
			break;

#ifdef SERV_9TH_NEW_CHARACTER
		case CXSLUnit::UT_ADD:
			kInfo.SetCharAbilType( KGamePlayStatus::CAC_NP_AND_MUTATION_AND_FORMATION_MODE );
			break;
#endif // SERV_9TH_NEW_CHARACTER

#ifdef ADD_RENA_SYSTEM //김창한
		case CXSLUnit::UT_LIRE:
			kInfo.SetCharAbilType( KGamePlayStatus::CAC_NATURAL_FORCE );
			break;
#endif //ADD_RENA_SYSTEM

		}

		FETCH_DATA( kInfo.m_iMaxHP
			>> kInfo.m_iCurHP
			>> kInfo.m_iMaxMP
			>> kInfo.m_iCurMP
			>> kInfo.m_iCurHyperGage
			>> kInfo.m_iCharAbilCount
			);

		m_kODBC.EndFetch();

		// 저장!
		kAck.m_kGamePlayStatus.Set( kInfo );
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

		START_LOG( cerr, L"P_GUnitPlayInfo_SEL 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	// 쿨 타임 정보 얻기!
	if( kAck.m_kGamePlayStatus.IsEmpty() == false )
	{
		DO_QUERY( L"exec dbo.P_GCoolTime_SEL", L"%d", % kReq.m_iUnitUID );
		while( m_kODBC.Fetch() )
		{
			int iCoolTimeType = 0;
			int iSlotID = 0;
			int iCoolTime = 0;

			FETCH_DATA( iCoolTimeType
				>> iSlotID
				>> iCoolTime );

			switch( iCoolTimeType )
			{
			case KGamePlayStatus::CTT_SKILL_COOL_TIME:
				{
					kAck.m_kGamePlayStatus.AddSkillCoolTime( iSlotID, iCoolTime );
				}
				break;

			case KGamePlayStatus::CTT_QUICK_SLOT_COOL_TIME:
				{
                    kAck.m_kGamePlayStatus.AddQuickSlotCoolTime( iSlotID, iCoolTime );
				}
				break;

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
			case KGamePlayStatus::CTT_RIDING_PET_COOL_TIME:
				{
					kAck.m_kGamePlayStatus.AddRidingPetCoolTime( iSlotID, iCoolTime );
				}
				break;
#endif	// SERV_RIDING_PET_SYSTM

			default:
				{
					START_LOG( cerr, L"정의되지 않은 쿨타임 타입입니다!" )
						<< BUILD_LOG( kReq.m_iUnitUID )
						<< BUILD_LOG( iCoolTimeType )
						<< END_LOG;
				}
				break;
			}
		}
	}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2011. 07. 22	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	// 대전 정보 받아오기!
	
	//{{ 2012. 06. 25	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_GET", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_iOfficialMatchCnt
			>> kAck.m_kUnitInfo.m_iRating
			>> kAck.m_kUnitInfo.m_iMaxRating
			>> kAck.m_kUnitInfo.m_iRPoint
			>> kAck.m_kUnitInfo.m_iAPoint
			>> kAck.m_kUnitInfo.m_bIsWinBeforeMatch
			>> kAck.m_kUnitInfo.m_cRank
			>> kAck.m_kUnitInfo.m_iWin
			>> kAck.m_kUnitInfo.m_iLose
			>> kAck.m_kUnitInfo.m_fKFactor
			>> kAck.m_kUnitInfo.m_bIsRedistributionUser
			>> kAck.m_kUnitInfo.m_iPastSeasonWin
			);
		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

		START_LOG( cerr, L"P_GUnitPVP_Season2_SEL 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}
#else
	DO_QUERY( L"exec dbo.gup_get_unitpvp_info", L"%d", % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_iOfficialMatchCnt
			>> kAck.m_kUnitInfo.m_iRating
			>> kAck.m_kUnitInfo.m_iMaxRating
			>> kAck.m_kUnitInfo.m_iRPoint
			>> kAck.m_kUnitInfo.m_iAPoint
			>> kAck.m_kUnitInfo.m_bIsWinBeforeMatch
			>> kAck.m_kUnitInfo.m_iWin
			>> kAck.m_kUnitInfo.m_iLose
			);
		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

		START_LOG( cerr, L"gup_get_unitpvp_info 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}
#endif SERV_2012_PVP_SEASON2
	//}}
#endif SERV_PVP_NEW_SYSTEM
	//}}

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
	// 정보 확인
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.PX_EventUser_SEL_Check", L"%d, %d", % iUserUID % kAck.m_kUnitInfo.m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EventUser_CHK", L"%d, %d", % iUserUID % kAck.m_kUnitInfo.m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iConnectExperienceAck );
		// 보상대상자 이면 1, 아니면 0, 이벤트 대상자지만 7일 안지났으면 2
		m_kODBC.EndFetch();
	}
	// 다른 이벤트에 사용할 수 있는 예비 정보값 입력
	kAck.m_iReward7DaysItem = 0; 

	START_LOG(clog2, L"7일 후의 기적 확인")
		<< BUILD_LOG( kAck.m_kUnitInfo.m_nUnitUID )
		<< BUILD_LOG( kAck.m_iConnectExperienceAck)
		<< BUILD_LOG( kAck.m_iReward7DaysItem)
		<< END_LOG;
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
	// ED체크 정보 받아오기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_Lastpoint", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_lastpoint", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iRealDataED );
		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

		START_LOG( cerr, L"gup_get_unit_lastpoint 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}
#endif SERV_LOGOUT_ED_CHECK
	//}}
	//////////////////////////////////////////////////////////////////////////	

	// 던전 클리어 정보를 얻는다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDungeonClear_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_dungeon_clear", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KDungeonClearInfo kInfo;
		kInfo.m_bNew = false;

		FETCH_DATA( kInfo.m_iDungeonID
			>> kInfo.m_iMaxScore
			>> kInfo.m_cMaxTotalRank
			>> kInfo.m_wstrClearTime );

		// 캐릭터 UID, 던전 ID가 PK 이므로 한 캐릭터에 대한 던전 ID는 모두 다르게 저장되어 있다.
		// 따라서 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
		kAck.m_kUnitInfo.m_mapDungeonClear.insert( std::make_pair( kInfo.m_iDungeonID, kInfo ) );
	}

	// 훈련소 클리어 정보를 얻는다.	
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GTrainingCenter_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_tc_clear", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KTCClearInfo kInfo;
		kInfo.m_bNew = false;

		FETCH_DATA( kInfo.m_iTCID
			>> kInfo.m_wstrClearTime );

		// 캐릭터 UID, 훈련소 ID가 PK 이므로 한 캐릭터에 대한 던전 ID는 모두 다르게 저장되어 있다.
		// 따라서 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
		kAck.m_kUnitInfo.m_mapTCClear.insert( std::make_pair( kInfo.m_iTCID, kInfo ) );
	}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEventDungeonData_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEventDungeonData_GET", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KDungeonPlayInfo kInfo;
		kInfo.m_bNew = false;

		FETCH_DATA( kInfo.m_iDungeonID
			>> kInfo.m_iPlayTimes
			>> kInfo.m_iClearTimes );

		// 캐릭터 UID, 던전 ID가 PK 이므로 한 캐릭터에 대한 던전 ID는 모두 다르게 저장되어 있다.
		// 따라서 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
		kAck.m_kUnitInfo.m_mapDungeonPlay.insert( std::make_pair( kInfo.m_iDungeonID, kInfo ) );
	}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 9. 29	최육사	아이템 정보 리팩토링
#ifdef SERV_GET_INVENTORY_REFAC
	//////////////////////////////////////////////////////////////////////////	
	if( Query_GetInventory( kReq.m_iUnitUID, kAck.m_mapInventorySlotSize, kAck.m_mapItem, kAck.m_mapPetItem ) == false )
	{
		START_LOG( cerr, L"인벤토리 정보 얻기 실패!" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

		START_LOG( cerr, L"Query_GetInventory() 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////	
#else
	//////////////////////////////////////////////////////////////////////////	
	// 인벤토리 사이즈를 얻는다.
//	DO_QUERY( L"exec dbo.gup_get_inventory_size", L"%d", % kReq.m_iUnitUID );
//	while( m_kODBC.Fetch() )
//	{
//		int iCategoty;
//		int iNumSlot;
//
//		FETCH_DATA( iCategoty
//			>> iNumSlot );
//
//		// 캐릭터 UID, 카테고리 ID가 PK이므로 한 캐릭터에 대한 카테고리 ID는 모두 다르게 저장되어 있다.
//		// 따라서 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
//		kAck.m_mapInventorySlotSize.insert( std::make_pair( iCategoty, iNumSlot ) );
//	}
//
//	//////////////////////////////////////////////////////////////////////////	
//	//{{ 2010. 8. 2	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//	DO_QUERY( L"exec dbo.gup_get_item_list_pet", L"%d", % kReq.m_iUnitUID );
//	while( m_kODBC.Fetch() )
//	{
//		UidType iItemUID = 0;
//		UidType iPetUID = 0;
//
//		FETCH_DATA( iItemUID
//			>> iPetUID );
//
//		std::map< UidType, UidType >::iterator mitPet;
//		mitPet = mapPetItemList.find( iPetUID );
//		if( mitPet == mapPetItemList.end() )
//		{
//			mapPetItemList.insert( std::make_pair( iItemUID, iPetUID ) );
//		}
//		else
//		{
//			START_LOG( cerr, L"중복되는 ItemUID가 있습니다. 있을 수 없는 에러!" )
//				<< BUILD_LOG( iItemUID )
//				<< BUILD_LOG( iPetUID )
//				<< END_LOG;
//		}
//	}
//#endif SERV_PET_SYSTEM
//	//}}
//
//	// 보유 아이템을 얻는다.	
//	DO_QUERY( L"exec dbo.gup_get_item_list", L"%d", % kReq.m_iUnitUID );
//	while( m_kODBC.Fetch() )
//	{
//		int iEnchantLevel = 0;
//		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
//		int arrSocketOption[4] = {0,0,0,0};
//#else
//		short arrSocketOption[4] = {0,0,0,0};
//#endif SERV_ITEM_OPTION_DATA_SIZE
//		//}} 
//		KInventoryItemInfo kInventoryItemInfo;
//
//		FETCH_DATA( kInventoryItemInfo.m_iItemUID
//			>> kInventoryItemInfo.m_kItemInfo.m_iItemID
//			>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
//			>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
//			>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
//			>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
//			>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
//			>> iEnchantLevel
//			>> arrSocketOption[0]
//			>> arrSocketOption[1]
//			>> arrSocketOption[2]
//			>> arrSocketOption[3]
//			>> kInventoryItemInfo.m_cSlotCategory
//				>> kInventoryItemInfo.m_cSlotID );
//
//			//{{ 2008. 2. 20  최육사  강화
//			kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel = static_cast<char>(iEnchantLevel);
//			//}}
//
//			//{{ 2008. 3. 7  최육사  소켓
//			int iCheckIdx;
//			for( iCheckIdx = 3; iCheckIdx >= 0; --iCheckIdx )
//			{
//				if( arrSocketOption[iCheckIdx] != 0 )
//					break;
//			}
//
//			for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
//			{
//				kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );
//			}
//			//}}
//
//			//{{ 2010. 8. 2	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//			// 펫 카테고리이면 따로 담자!
//			if( kInventoryItemInfo.m_cSlotCategory == CXSLInventory::ST_PET )
//			{
//				std::map< UidType, UidType >::const_iterator mitPet;
//				mitPet = mapPetItemList.find( kInventoryItemInfo.m_iItemUID );
//				if( mitPet == mapPetItemList.end() )
//				{
//					START_LOG( cerr, L"펫 아이템 정보에서 해당 아이템을 찾지 못했다! 일어나면 안되는 에러!" )
//						<< BUILD_LOG( kReq.m_iUnitUID )
//						<< BUILD_LOG( kInventoryItemInfo.m_iItemUID )
//						<< END_LOG;
//					continue;
//				}
//
//				const UidType iPetUID = mitPet->second;
//
//				std::map< UidType, std::map< UidType, KInventoryItemInfo > >::iterator mitPetItem;
//				mitPetItem = kAck.m_mapPetItem.find( iPetUID );
//				if( mitPetItem == kAck.m_mapPetItem.end() )
//				{
//					std::map< UidType, KInventoryItemInfo > mapPetItem;
//					mapPetItem.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
//					kAck.m_mapPetItem.insert( std::make_pair( iPetUID, mapPetItem ) );
//				}
//				else
//				{
//					mitPetItem->second.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
//				}
//			}
//			// 펫 카테고리 아닌것들은 일반 인벤토리로!
//			else
//			{
//				// 아이템 UID가 PK이므로 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
//				kAck.m_mapItem.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
//
//				//{{ 2013. 04. 01	 인연 시스템 - 김민성
//#ifdef SERV_RELATIONSHIP_SYSTEM
//				// 청첩장, 결혼 예약권
//				if( kInventoryItemInfo.m_kItemInfo.m_iItemID == CXSLItem::SI_WEDDING_INVITATION_ITEM || kInventoryItemInfo.m_kItemInfo.m_iItemID == CXSLItem::SI_WEDDING_RESERVATION_ITEM )
//				{
//					vecWeddingItem.push_back( kInventoryItemInfo.m_iItemUID );
//				}
//#endif SERV_RELATIONSHIP_SYSTEM
//					//}
//			}
//#else 
//			// 아이템 UID가 PK이므로 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
//			kAck.m_mapItem.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
//#endif SERV_PET_SYSTEM
//			//}}
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// 속성 강화 얻기
//	DO_QUERY( L"exec dbo.gup_get_attribute", L"%d", % kReq.m_iUnitUID );
//	while( m_kODBC.Fetch() )
//	{
//		UidType iItemUID = 0;
//		int iAttribEnchantSlotNo = 0;
//		int iAttribEnchantID = 0;
//
//		FETCH_DATA( iItemUID
//			>> iAttribEnchantSlotNo
//			>> iAttribEnchantID );
//
//		std::map< UidType, KInventoryItemInfo >::iterator mitAtt;
//		mitAtt = kAck.m_mapItem.find( iItemUID );
//		if( mitAtt == kAck.m_mapItem.end() )
//		{
//			//{{ 2010. 8. 2	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//			bool bFindItem = false;
//			std::map< UidType, std::map< UidType, KInventoryItemInfo > >::iterator mitPetItem;
//			for( mitPetItem = kAck.m_mapPetItem.begin(); mitPetItem != kAck.m_mapPetItem.end(); ++mitPetItem )
//			{
//				mitAtt = mitPetItem->second.find( iItemUID );
//				if( mitAtt != mitPetItem->second.end() )
//				{
//					bFindItem = true;
//					break;
//				}
//			}
//
//			if( bFindItem == false )
//			{
//				START_LOG( cerr, L"아이템 리스트는 받았는데 해당 속성은 있으면서 itemuid는 없네?" )
//					<< BUILD_LOG( iItemUID )
//					<< BUILD_LOG( iAttribEnchantSlotNo )
//					<< BUILD_LOG( iAttribEnchantID )
//					<< END_LOG;
//				continue;
//			}
//#else
//			START_LOG( cerr, L"아이템 리스트는 받았는데 해당 속성은 있으면서 itemuid는 없네?" )
//				<< BUILD_LOG( iItemUID )
//				<< BUILD_LOG( iAttribEnchantSlotNo )
//				<< BUILD_LOG( iAttribEnchantID )
//				<< END_LOG;
//			continue;
//#endif SERV_PET_SYSTEM
//			//}}
//		}
//
//		// 속성 강화
//		switch( iAttribEnchantSlotNo )
//		{
//		case CXSLAttribEnchantItem::ESI_SLOT_1:
//			mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>( iAttribEnchantID );
//			break;
//
//		case CXSLAttribEnchantItem::ESI_SLOT_2:
//			mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>( iAttribEnchantID );
//			break;
//
//		case CXSLAttribEnchantItem::ESI_SLOT_3:
//			mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>( iAttribEnchantID );
//			break;
//
//		default:
//			START_LOG( cerr, L"속성 강화 슬롯 넘버가 이상합니다." )
//				<< BUILD_LOG( iItemUID )
//				<< BUILD_LOG( iAttribEnchantSlotNo )
//				<< BUILD_LOG( iAttribEnchantID )
//				<< END_LOG;
//			break;
//		}
//	}
//
//	//////////////////////////////////////////////////////////////////////////
//	// 밀봉 정보 얻기
//	//{{ 2009. 8. 28  최육사	밀봉
//	DO_QUERY( L"exec dbo.gup_get_item_list_seal", L"%d", % kReq.m_iUnitUID );
//	while( m_kODBC.Fetch() )
//	{
//		UidType iItemUID = 0;
//		u_char ucSealData = 0;		
//
//		FETCH_DATA( iItemUID
//			>> ucSealData );
//
//		std::map< UidType, KInventoryItemInfo >::iterator mitSeal;
//		mitSeal = kAck.m_mapItem.find( iItemUID );
//		if( mitSeal == kAck.m_mapItem.end() )
//		{
//			//{{ 2010. 8. 2	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//			bool bFindItem = false;
//			std::map< UidType, std::map< UidType, KInventoryItemInfo > >::iterator mitPetItem;
//			for( mitPetItem = kAck.m_mapPetItem.begin(); mitPetItem != kAck.m_mapPetItem.end(); ++mitPetItem )
//			{
//				mitSeal = mitPetItem->second.find( iItemUID );
//				if( mitSeal != mitPetItem->second.end() )
//				{
//					bFindItem = true;
//					break;
//				}
//			}
//
//			if( bFindItem == false )
//			{
//				START_LOG( cerr, L"아이템 리스트는 받았는데 해당 밀봉정보 있으면서 itemuid는 없네?" )
//					<< BUILD_LOG( iItemUID )
//					<< BUILD_LOGc( ucSealData )
//					<< END_LOG;
//				continue;
//			}
//#else
//			START_LOG( cerr, L"아이템 리스트는 받았는데 해당 밀봉정보 있으면서 itemuid는 없네?" )
//				<< BUILD_LOG( iItemUID )
//				<< BUILD_LOGc( ucSealData )
//				<< END_LOG;
//			continue;
//#endif SERV_PET_SYSTEM
//			//}}			
//		}
//
//		// 밀봉 정보 업데이트
//		mitSeal->second.m_kItemInfo.m_ucSealData = ucSealData;
//	}
	//}}
	//////////////////////////////////////////////////////////////////////////	
#endif SERV_GET_INVENTORY_REFAC
	//////////////////////////////////////////////////////////////////////////	
	//}}	

	//////////////////////////////////////////////////////////////////////////	
	// 장착 아이템 정보를 세팅한다.
	for( mit = kAck.m_mapItem.begin(); mit != kAck.m_mapItem.end(); ++mit )
	{
		if( mit->second.m_cSlotCategory == CXSLInventory::ST_E_EQUIP )
		{
			std::map< int, KInventoryItemInfo >::iterator mit2;
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			mit2 = kAck.m_kUnitInfo.m_mapEquippedItem.find( mit->second.m_sSlotID );
#else
			mit2 = kAck.m_kUnitInfo.m_mapEquippedItem.find( mit->second.m_cSlotID );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
			if( mit2 != kAck.m_kUnitInfo.m_mapEquippedItem.end() )
			{
				START_LOG( cerr, L"장착 슬롯이 중복됨." )
					<< BUILD_LOG( mit->second.m_iItemUID )
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					<< BUILD_LOG( mit->second.m_sSlotID )
#else
					<< BUILD_LOGc( mit->second.m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
					<< BUILD_LOG( mit->second.m_kItemInfo.m_iItemID )
					<< END_LOG;
			}
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			kAck.m_kUnitInfo.m_mapEquippedItem.insert( std::make_pair( mit->second.m_sSlotID, mit->second) );
#else
			kAck.m_kUnitInfo.m_mapEquippedItem.insert( std::make_pair( mit->second.m_cSlotID, mit->second) );
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
		}
	}

	//////////////////////////////////////////////////////////////////////////	
	// 부활석 받아오기.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GResurrectionStone_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_resurrection_stone", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iNumResurrectionStone );

		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_03;

		START_LOG( cerr, L"gup_get_resurrection_stone 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	//{{ 2009. 10. 14  최육사	자동결제 부활석
#ifdef AP_RESTONE
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GResurrectionStone_AutoPay_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_resurrection_stone_autopay", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iNumAutoPaymentResStone
			>> kAck.m_wstrAutoPayResStoneLastDate
			);

		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_iOK = NetError::ERR_SELECT_UNIT_03;

		START_LOG( cerr, L"gup_get_resurrection_stone_autopay 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}

#endif AP_RESTONE
	//}}

#ifdef SERV_NEW_YEAR_EVENT_2014
	// 페치 실패 때를 대비하여 보상을 못 받는 현재 레벨로 셋팅해 둡니다.
	kAck.m_kUnitInfo.m_ucOldYearMissionRewardedLevel = kAck.m_kUnitInfo.m_ucLevel;

	if( kReq.m_setCodeEventScriptID.find( CEI_OLD_YEAR_EVENT_2013 ) != kReq.m_setCodeEventScriptID.end() )
	{
		DO_QUERY( L"exec dbo.P_GEventEndYear_GET", L"%d, %d", % kReq.m_iUnitUID % kAck.m_kUnitInfo.m_ucLevel );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_kUnitInfo.m_ucOldYearMissionRewardedLevel );
			m_kODBC.EndFetch();
		}
	}
	if( kReq.m_setCodeEventScriptID.find( CEI_NEW_YEAR_EVENT_2014 ) != kReq.m_setCodeEventScriptID.end() )
	{
		UidType iNewYearEventUnitUID = 0;
		DO_QUERY( L"exec dbo.P_GEventNewYear_GET", L"%d, %d", % iUserUID % kReq.m_iUnitUID );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iNewYearEventUnitUID
				>> kAck.m_kUnitInfo.m_iNewYearMissionStepID
				);
			m_kODBC.EndFetch();
		}
		// 계정에 미션을 수락한 다른 캐릭터가 있으면 -1로 셋팅
		if( iNewYearEventUnitUID != 0 && iNewYearEventUnitUID != kReq.m_iUnitUID )
			kAck.m_kUnitInfo.m_iNewYearMissionStepID = -1;
	}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_EVENT_CHECK_POWER
	if( kReq.m_setCodeEventScriptID.find( CEI_CHECK_POWER ) != kReq.m_setCodeEventScriptID.end() )
	{
		DO_QUERY( L"exec dbo.P_GEventElesisClassChange_GET", L"%d", % kReq.m_iUnitUID );
		if( m_kODBC.BeginFetch() )
		{
			kAck.m_kUnitInfo.m_bCheckPowerShowPopUp = false;
			std::wstring wstrCheckPowerTime;

			FETCH_DATA( kAck.m_kUnitInfo.m_ucCheckPowerScore
				>> kAck.m_kUnitInfo.m_ucCheckPowerCount
				>> wstrCheckPowerTime
				);
			m_kODBC.EndFetch();

			CTime tCheckPowerTime;
			KncUtil::ConvertStringToCTime( wstrCheckPowerTime, tCheckPowerTime );
			kAck.m_kUnitInfo.m_iCheckPowerTime = tCheckPowerTime.GetTime();

			if( tCheckPowerTime < CTime( 2013, 12, 1, 0, 0, 0 ) )
			{
				kAck.m_kUnitInfo.m_bCheckPowerShowPopUp = true;
			}
		}
	}

	START_LOG( cwarn, L"패킷 오는지 검사")
		<< BUILD_LOG( kAck.m_kUnitInfo.m_ucCheckPowerScore )
		<< BUILD_LOG( kAck.m_kUnitInfo.m_ucCheckPowerCount )
		<< BUILD_LOG( kAck.m_kUnitInfo.m_bCheckPowerShowPopUp )
		<< BUILD_LOG( kAck.m_kUnitInfo.m_iCheckPowerTime )
		<< END_LOG;
#endif SERV_EVENT_CHECK_POWER

	//////////////////////////////////////////////////////////////////////////
	//QUEST 목록 받아오기

	//진행중 퀘스트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GQuests_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_quest_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KQuestInstance		kQuest;
		KSubQuestInstance	kSub[5];
		kQuest.m_OwnorUnitUID = kReq.m_iUnitUID;

		FETCH_DATA( kQuest.m_iID
			>> kSub[0].m_ucClearData
			>> kSub[1].m_ucClearData
			>> kSub[2].m_ucClearData
			>> kSub[3].m_ucClearData
			>> kSub[4].m_ucClearData
			);
		for( int i = 0; i < 5; i++ )
			kQuest.m_vecSubQuestInstance.push_back( kSub[i] );

		kAck.m_vecQuest.push_back( kQuest );
	}

	//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST

	//완료 퀘스트(반복 X)
	{
		CTime tCompleteDate;
		std::wstring wstrCompleteDate;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQuests_Complete_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_quest_complete_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// 완료 날짜 변환
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kAck.m_vecCompletQuest.push_back( kInfo );
		}

		//완료 퀘스트(반복 O)
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQUESTS_REPEAT_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_quest_Repeat_complete_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// 완료 날짜 변환
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kAck.m_vecCompletQuest.push_back( kInfo );
		}
	}

#else

	//완료 퀘스트(반복 X)
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GQuests_Complete_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_quest_complete_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int	iQuestID = 0;

		FETCH_DATA( iQuestID );

		kAck.m_vecCompletQuest.push_back( iQuestID );
	}

	//완료 퀘스트(반복 O)
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GQUESTS_REPEAT_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_quest_Repeat_complete_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int	iQuestID = 0;

		FETCH_DATA( iQuestID );

		kAck.m_vecCompletQuest.push_back( iQuestID );
	}

#endif SERV_DAILY_QUEST
	//}}

	//////////////////////////////////////////////////////////////////////////
	//TITLE MISSION 목록 받아오기

	// 보유중인 타이틀
	bool bResetTitle = false;
	bool bIsEquippedTitle = false;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GTitle_Complete_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_title_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KTitleInfo kTitle;

		FETCH_DATA( kTitle.m_iTitleID
			>> kTitle.m_wstrEndDate
			>> bIsEquippedTitle );

		kAck.m_vecTitle.push_back( kTitle );

		if( bIsEquippedTitle )
		{
			//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
			if( kAck.m_kUnitInfo.m_iTitleID != 0 )
#else
			if( kAck.m_kUnitInfo.m_sTitleID != 0 )
#endif SERV_TITLE_DATA_SIZE
			//}}			
			{
				START_LOG( cerr, L"장착중인 칭호가 또 있네?" )
					<< BUILD_LOG( kReq.m_iUnitUID )
					//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
					<< BUILD_LOG( kAck.m_kUnitInfo.m_iTitleID )
#else
					<< BUILD_LOG( kAck.m_kUnitInfo.m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
					//}}
					<< BUILD_LOG( kTitle.m_iTitleID )
					<< END_LOG;

				bResetTitle = true;
				continue;
			}

			//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
			kAck.m_kUnitInfo.m_iTitleID = kTitle.m_iTitleID;
#else
			kAck.m_kUnitInfo.m_sTitleID = static_cast<short>(kTitle.m_iTitleID);
#endif SERV_TITLE_DATA_SIZE
			//}}			
		}
	}

	// 중복 장착된 타이틀은 장착해제	
	if( bResetTitle )
	{
		// 모두 장착해제
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_UPD_IsHang", L"%d, %d, %d", % kReq.m_iUnitUID % 0 % 0 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_title_hang", L"%d, %d, %d", % kReq.m_iUnitUID % 0 % 0 );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"칭호 장착 해제 업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< END_LOG;
		}

		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_UPD_IsHang", L"%d, %d, %d", % kReq.m_iUnitUID % 0 % kAck.m_kUnitInfo.m_iTitleID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_title_hang", L"%d, %d, %d", % kReq.m_iUnitUID % 0 % kAck.m_kUnitInfo.m_iTitleID );
#endif //SERV_ALL_RENEWAL_SP
#else
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_UPD_IsHang", L"%d, %d, %d", % kReq.m_iUnitUID % 0 % kAck.m_kUnitInfo.m_sTitleID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_title_hang", L"%d, %d, %d", % kReq.m_iUnitUID % 0 % kAck.m_kUnitInfo.m_sTitleID );
#endif //SERV_ALL_RENEWAL_SP
#endif SERV_TITLE_DATA_SIZE
		//}}		
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"칭호 장착 업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
				<< BUILD_LOG( kAck.m_kUnitInfo.m_iTitleID )
#else
				<< BUILD_LOG( kAck.m_kUnitInfo.m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
				//}}				
				<< END_LOG;
		}
	}

	// 진행중 미션
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GTitle_Mission_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_title_mission_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KMissionInstance	kMission;
		KSubMissionInstance	kSub[5];
		kMission.m_vecSubMissionInstance.reserve( 5 ); // 미리 사이즈 주기

		FETCH_DATA( kMission.m_iID
			>> kSub[0].m_sClearData
			>> kSub[1].m_sClearData
			>> kSub[2].m_sClearData
			>> kSub[3].m_sClearData
			>> kSub[4].m_sClearData
			);

		//{{ 2011.10.18     김민성    칭호 중복 진행(완료,수행) 예외 처리
#ifdef SERV_TITLE_DUPLICATE_PROCESS_REVISION
		bool bRet = false;
		KTitleInfo kTitle;
		for( vitTitle = kAck.m_vecTitle.begin() ; vitTitle != kAck.m_vecTitle.end() ; ++vitTitle )
		{
			kTitle = (*vitTitle);

			if( kTitle.m_iTitleID == kMission.m_iID )
			{
				bRet = true;
				break;
			}
		}
		if( bRet == true )
		{
			START_LOG( clog, L"보유중인데 진행 하려는 타이틀 미션이 있다!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kTitle.m_iTitleID )
				<< BUILD_LOG( kMission.m_iID );

			continue;
		}
#endif SERV_TITLE_DUPLICATE_PROCESS_REVISION
		//}}
	
		for( int i = 0; i < 5; ++i )
			kMission.m_vecSubMissionInstance.push_back( kSub[i] );

		kAck.m_vecMission.push_back( kMission );
	}

	//////////////////////////////////////////////////////////////////////////
	// 캐시스킬 포인트 관련 정보 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Cash_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_cash_skill_point_info", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		short iCSPoint = 0;
		short iMaxCSPoint = 0;

		FETCH_DATA( iCSPoint
			>> iMaxCSPoint
			>> kAck.m_kUnitInfo.m_wstrCSPointEndDate );

		kAck.m_kUnitInfo.m_iCSPoint		= iCSPoint;
		kAck.m_kUnitInfo.m_iMaxCSPoint	= iMaxCSPoint;

		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"캐시스킬포인트 정보 얻어오기 실패로 기본값을 넣어줌." )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		kAck.m_kUnitInfo.m_iCSPoint = 0;
		kAck.m_kUnitInfo.m_iMaxCSPoint = 0;
		kAck.m_kUnitInfo.m_wstrCSPointEndDate = L"2000-01-01 00:00:00";
	}

	// 봉인해제된 스킬목록 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Unsealed_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unsealed_skill_info", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		short iSkillID = 0;

		FETCH_DATA( iSkillID );

		kAck.m_vecSkillUnsealed.push_back( iSkillID );
	}

#ifdef SERV_SKILL_PAGE_SYSTEM

	DO_QUERY( L"exec dbo.P_GSkill_New_SEL", L"%d", % kReq.m_iUnitUID );
	while( m_kODBC.Fetch() )
	{
		short iSkillID			= 0;	
		UCHAR cSkillLevel		= 0;
		UCHAR cSkillCSPoint		= 0;
		UCHAR cSkillPageNumber	= 0;


		FETCH_DATA( iSkillID 
			>> cSkillLevel
			>> cSkillCSPoint
			>> cSkillPageNumber
			);

		if ( iSkillID > 0 && cSkillLevel > 0 )
		{
			if ( cSkillPageNumber > kAck.m_vecUserSkillPageData.size() )
			{
				KUserSkillPageData userSkillPageData;
				userSkillPageData.m_vecUserSkillData.push_back( KUserSkillData( iSkillID, cSkillLevel, cSkillCSPoint ) );
				kAck.m_vecUserSkillPageData.push_back( userSkillPageData );
			}
			else if ( cSkillPageNumber > 0 )
			{
				/// 인덱스는 PageNumber - 1
				kAck.m_vecUserSkillPageData[cSkillPageNumber - 1].m_vecUserSkillData.push_back( KUserSkillData( iSkillID, cSkillLevel, cSkillCSPoint ) );
			}
		}
	}

	if ( kAck.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable 
		!= static_cast<unsigned char>( kAck.m_vecUserSkillPageData.size() ) )
	{
		START_LOG( cerr, L"전체 페이지 수가 이상한데..?" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kAck.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable )
			<< BUILD_LOG( kAck.m_vecUserSkillPageData.size() )
			<< END_LOG;

		const int iSkillPageToBeAdded = 
			kAck.m_kUnitInfo.m_UnitSkillData.m_nTheNumberOfSkillPagesAvailable - kAck.m_vecUserSkillPageData.size();

		for ( int i = 0; i < iSkillPageToBeAdded; i++ )
			kAck.m_vecUserSkillPageData.push_back( KUserSkillPageData() );
	}


#else //SERV_SKILL_PAGE_SYSTEM
	//SKILL LIST 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_New_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_skill_list_new", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		short iSkillID = 0;	
		UCHAR cSkillLevel = 0;
		UCHAR cSkillCSPoint = 0;

		FETCH_DATA( iSkillID 
			>> cSkillLevel
			>> cSkillCSPoint
			);

		kAck.m_vecSkillAcquired.push_back( KUserSkillData( iSkillID, cSkillLevel, cSkillCSPoint ) );
	}
#endif // SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_SKILL_PAGE_SYSTEM
	//SKILL SLOT 받아오기
	const int iEQUIPPED_SKILL_SLOT_COUNT = 4;

	DO_QUERY( L"exec dbo.P_GSkillSlot_New_SEL", L"%d", % kReq.m_iUnitUID );

	bool bConductedOnceAtLeast = false;

	while( m_kODBC.Fetch() )
	{
		int aEquippedSkillID[iEQUIPPED_SKILL_SLOT_COUNT] = { 0, };
		int iSkillPageNumber = 0;

		FETCH_DATA( aEquippedSkillID[0]
			>> aEquippedSkillID[1]
			>> aEquippedSkillID[2]
			>> aEquippedSkillID[3]
			>> iSkillPageNumber
			);

		if ( iSkillPageNumber < 1 || 
			 iSkillPageNumber > static_cast<int>( kAck.m_vecUserSkillPageData.size() ) )
		{
			START_LOG( cerr, L"스킬 페이지 번호가 이상함(gup_get_skill_slot_new)" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_vecUserSkillPageData.size() )
				<< BUILD_LOG( iSkillPageNumber )
				<< END_LOG;
		}
		else
		{
			bConductedOnceAtLeast = true;

			for ( UINT i = 0; i < iEQUIPPED_SKILL_SLOT_COUNT; i++ )
			{
				kAck.m_vecUserSkillPageData[iSkillPageNumber - 1].m_aEquippedSkill[i].m_iSkillID
					= aEquippedSkillID[i];			

				if ( iSkillPageNumber == kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber )
					kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[i].m_iSkillID = aEquippedSkillID[i];
			}
		}
	}

	// 한번도 수행 하지 않았다면 insert 하자
	if ( !bConductedOnceAtLeast )
	{
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[0].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[1].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[2].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[3].Init();

		for ( UINT iSkillPageIndex = 0; iSkillPageIndex < kAck.m_vecUserSkillPageData.size(); iSkillPageIndex++ )
		{
			const int iSkillPageNumber = iSkillPageIndex + 1;
			//스킬슬롯이 추가되는 거라서 현재 만들어져 있는 캐릭더들중 슬롯이 없으면 만들어줌
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkillSlot_New_MER", L"%d, %d, %d, %d, %d, %d",
				% kReq.m_iUnitUID
				% (int)0
				% (int)0
				% (int)0
				% (int)0
				% iSkillPageNumber
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkillSlot_New_INS", L"%d, %d, %d, %d, %d, %d",
				% kReq.m_iUnitUID
				% (int)0
				% (int)0
				% (int)0
				% (int)0
				% iSkillPageNumber
				);
#endif //SERV_ALL_RENEWAL_SP
			int iOK = 0;

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != 0 )
			{ 
				START_LOG( cerr, L"기존에 있던 캐릭터의 스킬 슬롯 데이터 생성실패" )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( iSkillPageIndex + 1 )
					<< END_LOG;

				return false;
			}

			START_LOG( clog, L"========= 기존 캐릭터 스킬슬롯이 없어 새로 생성 성공 ===========" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				;
		}		
	}
	
	bConductedOnceAtLeast = false;
	DO_QUERY( L"exec dbo.P_GSkillSlot2_New_SEL", L"%d", % kReq.m_iUnitUID );

	while( m_kODBC.Fetch() )
	{
		int aEquippedSkillID[iEQUIPPED_SKILL_SLOT_COUNT] = { 0, };
		int iSkillPageNumber = 0;

		FETCH_DATA( aEquippedSkillID[0]
		>> aEquippedSkillID[1]
		>> aEquippedSkillID[2]
		>> aEquippedSkillID[3]
		>> iSkillPageNumber
		>> kAck.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate
			);

		if ( iSkillPageNumber < 1 || 
			iSkillPageNumber > static_cast<int>( kAck.m_vecUserSkillPageData.size() ) )
		{
			START_LOG( cerr, L"스킬 페이지 번호가 이상함 (gup_get_skill_slot2_new)" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_vecUserSkillPageData.size() )
				<< BUILD_LOG( iSkillPageNumber )
				<< END_LOG;
		}
		else
		{
			bConductedOnceAtLeast = true;
			for ( UINT i = 0; i < iEQUIPPED_SKILL_SLOT_COUNT; i++ )
			{
				kAck.m_vecUserSkillPageData[iSkillPageNumber - 1].m_aEquippedSkillSlotB[i].m_iSkillID 
					= aEquippedSkillID[i];

				if ( iSkillPageNumber == kAck.m_kUnitInfo.m_UnitSkillData.m_nActiveSkillPagesNumber )
					kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[i].m_iSkillID = aEquippedSkillID[i];
			}
		}
	}

	if ( !bConductedOnceAtLeast )
	{
		START_LOG( cerr, L"스킬 슬롯 B 사용 안함" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[0].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[1].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[2].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[3].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate = L"2000-01-01 00:00:00";
	}
	

#else // SERV_SKILL_PAGE_SYSTEM
	//SKILL SLOT 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkillSlot_New_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_skill_slot_new", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[0].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[1].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[2].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[3].m_iSkillID
			);

		m_kODBC.EndFetch();
	}
	else
	{
		//kPacket.m_iOK = NetError::ERR_SELECT_UNIT_02;
		//goto end_proc;
		//스킬슬롯이 추가되는 거라서 현재 만들어져 있는 캐릭더들중 슬롯이 없으면 만들어줌
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkillSlot_New_MER", L"%d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% (int)0
			% (int)0
			% (int)0
			% (int)0
			% 1
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_skill_slot_new", L"%d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% (int)0
			% (int)0
			% (int)0
			% (int)0
			);
#endif //SERV_ALL_RENEWAL_SP
		int iOK = 0;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != 0 )
		{ 
			START_LOG( cerr, L"기존에 있던 캐릭터의 스킬 슬롯 데이터 생성실패" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< END_LOG;

			return false;
		}

		START_LOG( clog, L"========= 기존 캐릭터 스킬슬롯이 없어 새로 생성 성공 ===========" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			;

		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[0].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[1].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[2].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkill[3].Init();
	}

	//SKILL SLOT B 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkillSlot2_New_SEL", L"%d", % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		int iSkillPageNumber = 0;

		FETCH_DATA( kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[0].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[1].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[2].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[3].m_iSkillID
			>> iSkillPageNumber
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate
			);

		m_kODBC.EndFetch();
	}
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_skill_slot2_new", L"%d", % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[0].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[1].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[2].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[3].m_iSkillID
			>> kAck.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate
			);

		m_kODBC.EndFetch();
	}
#endif //SERV_ALL_RENEWAL_SP
	else
	{
		START_LOG( cerr, L"스킬 슬롯 정보 얻어오기 실패로 기본 초기값 넣어줌." )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[0].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[1].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[2].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_aEquippedSkillSlotB[3].Init();
		kAck.m_kUnitInfo.m_UnitSkillData.m_wstrSkillSlotBEndDate = L"2000-01-01 00:00:00";
	}
#endif // SERV_SKILL_PAGE_SYSTEM


	//{{ 2010. 03. 27  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE

	// 기술의 노트 최대 페이지 수 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GNote_PageCNT_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_notecnt", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_cSkillNoteMaxPageNum );
		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( clog, L"기술의 노트 스킬 슬롯 정보가 없습니다. 업데이트 된적이 없는듯.." )
			<< BUILD_LOG( kReq.m_iUnitUID );

		// 0으로 초기화
		kAck.m_cSkillNoteMaxPageNum = 0;
	}

	// 기술의 노트 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GNote_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_note", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		char cPageNum = 0;
		int iMemoID = 0;

		FETCH_DATA( cPageNum
			>> iMemoID
			);

		kAck.m_mapSkillNote.insert( std::make_pair( cPageNum, iMemoID ) );
	}

#endif SERV_SKILL_NOTE
	//}}

	//COMMUNITY OPTION 받아오기
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	DO_QUERY( L"exec dbo.P_GDenyOption_SEL", L"%d", % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kDenyOptions.m_cDenyFriendShip
			>> kAck.m_kDenyOptions.m_cDenyInviteGuild
			>> kAck.m_kDenyOptions.m_cDenyParty
			>> kAck.m_kDenyOptions.m_cDenyPersonalTrade
			>> kAck.m_kDenyOptions.m_cDenyRequestCouple
            >> kAck.m_kDenyOptions.m_cDenyInvitePracticePVP
			);

		m_kODBC.EndFetch();

		kAck.m_iOK = NetError::NET_OK;
	}
#else
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDenyOption_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_community_opt_unituid", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kDenyOptions.m_cDenyFriendShip
			>> kAck.m_kDenyOptions.m_cDenyInviteGuild
			>> kAck.m_kDenyOptions.m_cDenyParty
			>> kAck.m_kDenyOptions.m_cDenyPersonalTrade
			);

		m_kODBC.EndFetch();

		kAck.m_iOK = NetError::NET_OK;
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	else
	{
		kAck.m_iOK = NetError::ERR_COMMUNITY_OPT_02;

		START_LOG( cerr, L"gup_get_community_opt_unituid 호출 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		return false;
	}

	//{{ 2008. 1. 31  최육사  
	//BLACK LIST 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBlackList_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_select_blacklist", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KChatBlackListUnit kBlackListUnit;

		FETCH_DATA( kBlackListUnit.m_iUnitUID
			>> kBlackListUnit.m_wstrNickName
			);

		kAck.m_vecChatBlackList.push_back( kBlackListUnit );
	}
	//}}

	//080405.hoons. 사제리스트 받아오기.
	int iTutorialFlag = -1;
	if( kAck.m_kUnitInfo.m_ucLevel <= KTutorialManager::UNIT_LEVEL_STUDENT_MAX )
		iTutorialFlag = 0;
	else if( kAck.m_kUnitInfo.m_ucLevel >= KTutorialManager::UNIT_LEVEL_TEACHER_MIN )
		iTutorialFlag = 1;

#ifndef SERV_NO_DISCIPLE
	if( iTutorialFlag != -1 ) // 제자 이거나, 스승의 조건에만 호출하도록...11~19 레벨 캐릭터는 호출 필요성 X // 김민성 // 2013-07-05
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTutor_SEL", L"%d, %d", % iTutorialFlag % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_tutor", L"%d, %d", % iTutorialFlag % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KTutorialDBUnitInfo kInfo;
			FETCH_DATA( kInfo.m_kTutorialUnitInfo.m_iUnitUID
				>> kInfo.m_kTutorialUnitInfo.m_ucLevel
				>> kInfo.m_kTutorialUnitInfo.m_wstrNickName
				>> kInfo.m_wstrLastDate
				);

			kAck.m_vecTutorialDBUnitInfo.push_back( kInfo );
		}
	}
#endif

#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	// 접속 시간 이벤트 정보 얻기 : SP이름은 30min으로 되어있지만 실제로는 접속 시간 이벤트 정보입니다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GIs30min_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_select_30min", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KConnectTimeEventInfo kConnectTimeEvent;

		FETCH_DATA( kConnectTimeEvent.m_iEventUID
			>> kConnectTimeEvent.m_wstrEventTime
			);

		kAck.m_vecConnectTimeEvent.push_back( kConnectTimeEvent );
	}
	//}}
#endif //SERV_ADD_EVENT_DB

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	// 신규 유저냐
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUser_SEL_CombackUserCHK",  L"%d, %d", % iUserUID % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_CombackUser_CHK",  L"%d, %d", % iUserUID % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iNewUnitE );

		//START_LOG(clog2, L"김석근_기존캐릭터 접속 이벤트. m_bNewUnit 값은 신규 유저냐?? 결과!")
		//	<< BUILD_LOG( iUserUID)
		//	<< BUILD_LOG( kReq.m_iUnitUID )
		//	<< BUILD_LOG( kAck.m_iNewUnitE)
		//	<< END_LOG;
		 
		m_kODBC.EndFetch();
	}
#endif SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
#ifdef SERV_ADVERTISEMENT_EVENT
	int iEventUID = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GAdvertisementEvent_SEL", L"%d", % iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GAdvertisementEvent_SEL", L"%d", % iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iEventUID );

		kAck.m_vecAdvertisementEvent.push_back( iEventUID );
	}
#endif SERV_ADVERTISEMENT_EVENT
#endif //SERV_ADD_EVENT_DB
#ifdef SERV_RECRUIT_EVENT_BASE
	DO_QUERY( L"exec dbo.gup_get_recommend_newuid", L"%d", % kReq.m_iUnitUID );
	while( m_kODBC.Fetch() )
	{
		KRecommendUserInfo kInfo;

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_ucLevel
			>> kInfo.m_cUnitClass
			>> kInfo.m_wstrNickName );

		kAck.m_vecRecruiterUnitInfo.push_back( kInfo );

		START_LOG( clog, L"추천인 목록 획득 완료" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( kInfo.m_ucLevel )
			<< BUILD_LOG( kInfo.m_cUnitClass )
			<< BUILD_LOG( kInfo.m_wstrNickName )
			<< END_LOG;
	}

	DO_QUERY( L"exec dbo.gup_get_recommend_olduid", L"%d", % kReq.m_iUnitUID );
	while( m_kODBC.Fetch() )
	{
		KRecommendUserInfo kInfo;

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_ucLevel
			>> kInfo.m_cUnitClass
			>> kInfo.m_wstrNickName );

		kAck.m_vecRecruitUnitInfo.push_back( kInfo );

		START_LOG( clog, L"피추천인 목록 획득 완료" )
			<< BUILD_LOG( kInfo.m_iUnitUID )
			<< BUILD_LOG( kInfo.m_ucLevel )
			<< BUILD_LOG( kInfo.m_cUnitClass )
			<< BUILD_LOG( kInfo.m_wstrNickName )
			<< END_LOG;
	}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_CUSTOM_CONNECT_EVENT
	DO_QUERY( L"exec dbo.P_CustomEvent_CHK", L"%d, %d", % iUserUID % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iCustomEventID );

		m_kODBC.EndFetch();

		START_LOG( clog, L"P_CustomEvent_CHK sp 실행 결과" )
			<< BUILD_LOG( kAck.m_iCustomEventID )
			<< END_LOG;
	}
	else
	{
		START_LOG( cerr, L"P_CustomEvent_CHK sp 호출 실패!" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;
		kAck.m_iCustomEventID = 0;
	}
#endif SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_CHINA_SPIRIT_EVENT
	DO_QUERY( L"exec dbo.P_GEventChinaTicketSpirit_GET", L"%d", % kReq.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA(	kAck.m_kUnitInfo.m_arrChinaSpirit[0]
			>> kAck.m_kUnitInfo.m_arrChinaSpirit[1]
			>> kAck.m_kUnitInfo.m_arrChinaSpirit[2]
			>> kAck.m_kUnitInfo.m_arrChinaSpirit[3]
			>> kAck.m_kUnitInfo.m_arrChinaSpirit[4]
			>> kAck.m_kUnitInfo.m_arrChinaSpirit[5]
			);
		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_kUnitInfo.m_arrChinaSpirit[0] = 0;
		kAck.m_kUnitInfo.m_arrChinaSpirit[1] = 0;
		kAck.m_kUnitInfo.m_arrChinaSpirit[2] = 0;
		kAck.m_kUnitInfo.m_arrChinaSpirit[3] = 0;
		kAck.m_kUnitInfo.m_arrChinaSpirit[4] = 0;
		kAck.m_kUnitInfo.m_arrChinaSpirit[5] = 0;
	}
#endif //SERV_CHINA_SPIRIT_EVENT

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GEvent_Account_Nor_SEL", L"%d", % iUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_select_event_account_nor", L"%d", % iUserUID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KConnectTimeEventInfo kConnectTimeEvent;

			FETCH_DATA( kConnectTimeEvent.m_iEventUID
				>> kConnectTimeEvent.m_wstrEventTime
				);

			kAck.m_vecConnectTimeEvent.push_back( kConnectTimeEvent );
		}
#endif //SERV_ADD_EVENT_DB
#endif SERV_ACC_TIME_EVENT
	//}}

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GRemainTime_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_remaintime", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KCumulativeTimeEventInfo kCumulativeTimeEvent;

		FETCH_DATA( kCumulativeTimeEvent.m_iEventUID
			>> kCumulativeTimeEvent.m_iCumulativeTime
			);

			kAck.m_vecCumulativeTimeEvent.push_back( kCumulativeTimeEvent );
		}
#endif //SERV_ADD_EVENT_DB

	//////////////////////////////////////////////////////////////////////////	
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_Account_Acc_SEL", L"%d", % iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_select_event_account_acc", L"%d", % iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KCumulativeTimeEventInfo kCumulativeTimeEvent;

		FETCH_DATA( kCumulativeTimeEvent.m_iEventUID
			>> kCumulativeTimeEvent.m_iCumulativeTime
			);

		kAck.m_vecCumulativeTimeEvent.push_back( kCumulativeTimeEvent );
	}
#endif SERV_ACC_TIME_EVENT
		//}}
#endif //SERV_ADD_EVENT_DB
	
	//////////////////////////////////////////////////////////////////////////

#endif CUMULATIVE_TIME_EVENT
	//}}
	//////////////////////////////////////////////////////////////////////////	

#ifdef SERV_GLOBAL_FRIEND
	// 친구 목록 얻어 오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_friend_get_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KFriendInfo kFriendInfo;
		FETCH_DATA( kFriendInfo.m_iUnitUID
			>> kFriendInfo.m_wstrNickName
			>> kFriendInfo.m_cGroupID
			>> kFriendInfo.m_cStatus
			);
		kFriendInfo.m_cPosition = KFriendInfo::FP_OFF_LINE;

		kAck.m_kMessengerInfo.m_mapFriendInfo.insert( std::make_pair( kFriendInfo.m_iUnitUID, kFriendInfo ) );
	}

	// 친구 그룹 얻어오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendGroup_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_friend_get_group_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		char cGroup;
		std::wstring wstrGroupName;
		FETCH_DATA( cGroup
			>> wstrGroupName
			);

		kAck.m_kMessengerInfo.m_mapGroup.insert( std::make_pair( cGroup, wstrGroupName ) );
	}

	// 친구 메세지 얻어오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendMessage_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_friend_get_message", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KFriendMessageInfo kFriendMessageInfo;
		FETCH_DATA( kFriendMessageInfo.m_iUnitUID
			>> kFriendMessageInfo.m_wstrNickName
			>> kFriendMessageInfo.m_cMessageType
			>> kFriendMessageInfo.m_wstrMessage
			>> kFriendMessageInfo.m_wstrRegDate
			);

		kAck.m_kMessengerInfo.m_vecFriendMessageInfo.push_back( kFriendMessageInfo );
	}
#endif //SERV_GLOBAL_FRIEND

	//{{ 2009. 7. 7  최육사		랭킹개편
	// 헤니르 랭킹 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Rank_SpaceTime_MyRecord_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_rank_myrecord", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		CTime tRegDate;
		std::wstring wstrRegDate;
		KHenirRankingInfo kInfo;

		FETCH_DATA( kInfo.m_iStageCount
			>> kInfo.m_ulPlayTime
			>> kInfo.m_ucLevel
			>> kInfo.m_cUnitClass
			>> wstrRegDate
			);

		// RegDate 변환
		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
		kInfo.m_tRegDate = tRegDate.GetTime();
		kInfo.m_iUnitUID = kReq.m_iUnitUID;
		kInfo.m_wstrNickName = kAck.m_kUnitInfo.m_wstrNickName;		

		kAck.m_vecHenirRanking.push_back( kInfo );
	}

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
	// 헤니르 영웅 랭킹 정보 읽기
	DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Hero_MyRecord_SEL", L"%d", % kReq.m_iUnitUID );
	while( m_kODBC.Fetch() )
	{
		CTime tRegDate;
		std::wstring wstrRegDate;
		KHenirRankingInfo kInfo;

		FETCH_DATA( kInfo.m_iStageCount
			>> kInfo.m_ulPlayTime
			>> kInfo.m_ucLevel
			>> kInfo.m_cUnitClass
			>> wstrRegDate
			);

		// RegDate 변환
		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
		kInfo.m_tRegDate = tRegDate.GetTime();
		kInfo.m_iUnitUID = kReq.m_iUnitUID;
		kInfo.m_wstrNickName = kAck.m_kUnitInfo.m_wstrNickName;		

		kAck.m_vecHenirHeroRanking.push_back( kInfo );
	}
#endif // SERV_HENIR_RENEWAL_2013
	//}}

	//{{ 2009. 9. 25  최육사	길드
#ifdef GUILD_TEST

	// 길드 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_my_guild_info", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_kUserGuildInfo.m_iGuildUID
			>> kAck.m_kUnitInfo.m_kUserGuildInfo.m_wstrGuildName );

		m_kODBC.EndFetch();

		kAck.m_iOK = NetError::NET_OK;
	}
	
	// 길드원 등급 및 명예 포인트 정보 얻기
	if( kAck.m_kUnitInfo.m_kUserGuildInfo.m_iGuildUID > 0 )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_SEL_Grade", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_guild_member_grade", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_kUnitInfo.m_kUserGuildInfo.m_ucMemberShipGrade
				>> kAck.m_kUnitInfo.m_kUserGuildInfo.m_iHonorPoint );

			m_kODBC.EndFetch();
		}

		if( kAck.m_kUnitInfo.m_kUserGuildInfo.m_ucMemberShipGrade == -1  ||  
			kAck.m_kUnitInfo.m_kUserGuildInfo.m_iHonorPoint == -1 )
		{
			START_LOG( cerr, L"길드원 등급 및 명예 포인트 정보 얻기 실패!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< END_LOG;

			kAck.m_kUnitInfo.m_kUserGuildInfo.m_ucMemberShipGrade = 0;
			kAck.m_kUnitInfo.m_kUserGuildInfo.m_iHonorPoint = 0;
		}
	}

#endif GUILD_TEST
	//}}

	//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

	kAck.m_iSummonedPetUID = 0;

	// 펫 정보 얻기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_pet_list", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		bool bIsSummoned = false;
		KPetInfo kInfo;

#ifdef SERV_PERIOD_PET
		// SERV_PET_AUTO_LOOTING, SERV_PETID_DATA_TYPE_CHANGE 켜진 것 기준으로 하나만 제작
		FETCH_DATA( kInfo.m_iPetUID
			>> kInfo.m_iPetID
			>> kInfo.m_wstrPetName
			>> kInfo.m_cEvolutionStep
			>> kInfo.m_sSatiety	
			>> kInfo.m_iIntimacy
			>> kInfo.m_sExtroversion
			>> kInfo.m_sEmotion
			>> kInfo.m_wstrRegDate
			>> bIsSummoned
			>> kInfo.m_bAutoFeed
			>> kInfo.m_wstrLastFeedDate
			>> kInfo.m_wstrLastSummonDate
			>> kInfo.m_bAutoLooting
			>> kInfo.m_wstrDestroyDate
			);
#else SERV_PERIOD_PET

		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
		FETCH_DATA( kInfo.m_iPetUID
			>> kInfo.m_iPetID
			>> kInfo.m_wstrPetName
			>> kInfo.m_cEvolutionStep
			>> kInfo.m_sSatiety	
			>> kInfo.m_iIntimacy
			>> kInfo.m_sExtroversion
			>> kInfo.m_sEmotion
			>> kInfo.m_wstrRegDate
			>> bIsSummoned
			>> kInfo.m_bAutoFeed
			>> kInfo.m_wstrLastFeedDate
			>> kInfo.m_wstrLastSummonDate
			>> kInfo.m_bAutoLooting
			);
#else //SERV_PETID_DATA_TYPE_CHANGE
		FETCH_DATA( kInfo.m_iPetUID
			>> kInfo.m_cPetID
			>> kInfo.m_wstrPetName
			>> kInfo.m_cEvolutionStep
			>> kInfo.m_sSatiety	
			>> kInfo.m_iIntimacy
			>> kInfo.m_sExtroversion
			>> kInfo.m_sEmotion
			>> kInfo.m_wstrRegDate
			>> bIsSummoned
			>> kInfo.m_bAutoFeed
			>> kInfo.m_wstrLastFeedDate
			>> kInfo.m_wstrLastSummonDate
			>> kInfo.m_bAutoLooting
			);
#endif //SERV_PETID_DATA_TYPE_CHANGE
#else
		FETCH_DATA( kInfo.m_iPetUID
			>> kInfo.m_cPetID
			>> kInfo.m_wstrPetName
			>> kInfo.m_cEvolutionStep
			>> kInfo.m_sSatiety	
			>> kInfo.m_iIntimacy
			>> kInfo.m_sExtroversion
			>> kInfo.m_sEmotion
			>> kInfo.m_wstrRegDate
			>> bIsSummoned
			>> kInfo.m_bAutoFeed
			>> kInfo.m_wstrLastFeedDate
			>> kInfo.m_wstrLastSummonDate
			);
#endif SERV_PET_AUTO_LOOTING
		//}}

#endif SERV_PERIOD_PET

		if( bIsSummoned )
		{
			if( kAck.m_iSummonedPetUID != 0 )
			{
				START_LOG( cerr, L"엇? 소환된 펫이 또있네!? 있을수 없는 에러!" )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( kAck.m_iSummonedPetUID )
					<< BUILD_LOG( kInfo.m_iPetUID )
					<< END_LOG;
			}

#ifdef SERV_PERIOD_PET
			CTime tDestroyDate;
			if( kInfo.m_wstrDestroyDate != L"" &&
				KncUtil::ConvertStringToCTime( kInfo.m_wstrDestroyDate, tDestroyDate ) == true &&
				tDestroyDate <= CTime::GetCurrentTime() )
			{
				// 만료가 된 펫이라면 선택하지 않습니다.
				kAck.m_iSummonedPetUID = 0;
			}
			else
#endif SERV_PERIOD_PET
			kAck.m_iSummonedPetUID = kInfo.m_iPetUID;
		}

		kAck.m_vecPetList.push_back( kInfo );
	}
#ifdef SERV_EVENT_PET_INVENTORY
	//여기서 kInfo에 이벤트 먹이 사용 유무를 얻어 온다.
	{
		bool bIsInfo = false;
		KPetInfo EventPetkInfo;
		DO_QUERY( L"exec dbo.P_GPetEvent_SEL", L"%d", % kReq.m_iUnitUID );
		if( m_kODBC.BeginFetch() )
		{
			int	m_UnitUID = 0;
			bIsInfo = true;
			FETCH_DATA( m_UnitUID
				>> EventPetkInfo.m_iPetUID
				>> EventPetkInfo.m_iPetID					// SERV_PETID_DATA_TYPE_CHANGE
				>> EventPetkInfo.m_wstrPetName
				>> EventPetkInfo.m_bEventFoodEat	
				);
			m_kODBC.EndFetch();
		}
		if( bIsInfo )
		{
			std::vector< KPetInfo >::iterator vecIter;
			for( vecIter = kAck.m_vecPetList.begin(); vecIter != kAck.m_vecPetList.end(); ++vecIter )
			{
				if( vecIter->m_iPetID == EventPetkInfo.m_iPetID )
				{
					vecIter->m_bIsEventPetID = true;
					vecIter->m_bEventFoodEat = EventPetkInfo.m_bEventFoodEat;
				}
			}
		}
	}
#endif SERV_EVENT_PET_INVENTORY
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	// 캐릭터 카운트 정보 얻기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GCharacter_Count_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_Character_Count", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int iLogType = 0;
		int iCountValue = 0;

		FETCH_DATA( iLogType
			>> iCountValue
			);

		kAck.m_mapCharGameCount.insert( std::make_pair( iLogType, iCountValue ) );
	}
#endif SERV_CHAR_LOG
	//}}

#ifdef SERV_UPGRADE_TRADE_SYSTEM
	//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	// 대리 상인 정보 얻기
	DO_QUERY( L"exec dbo.P_GPShopInfo_SEL_ShopCheck", L"%d", % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kPShopAgencyInfo.m_wstrAgencyExpirationDate
			>> kAck.m_kPShopAgencyInfo.m_bIsPShopOpen
            >> kAck.m_kPShopAgencyInfo.m_cShopType
            >> kAck.m_kPShopAgencyInfo.m_wstrAgencyOpenDate
            >> kAck.m_kPShopAgencyInfo.m_wstrPersonalShopName
            >> kAck.m_kPShopAgencyInfo.m_bOnSale
            );

		m_kODBC.EndFetch();
	}
	else
	{
		kAck.m_kPShopAgencyInfo.m_wstrAgencyExpirationDate = L"2000-01-01 00:00:00";
		kAck.m_kPShopAgencyInfo.m_bIsPShopOpen = false;
        kAck.m_kPShopAgencyInfo.m_cShopType = SEnum::AST_NONE;
	}
#endif SERV_PSHOP_AGENCY
	//}}

    // TODO : 여기에 대리상점 아이템 가져오는 부분 추가하기.
    // 상점 판매 물품 얻기
    //#ifdef SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX // 김민성 // 적용날짜: 2013-07-11
    DO_QUERY( L"exec dbo.P_GPShopItem_SEL", L"%d", % kReq.m_iUnitUID );
    //#else	// SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX
    //			DO_QUERY( L"exec dbo.gup_get_PShopItem_Inventory_by_UnitUID", L"%d", % kInfo.m_iUnitUID );
    //#endif // SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX
    while( m_kODBC.Fetch() )
    {
        KSellPShopItemBackupData kItemInfo;
        kItemInfo.m_kSellPShopItemInfo.m_cPShopItemType = KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY;

        //{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
		const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS
		int arrSocketOption[byteArraySize];
		memset( arrSocketOption, 0, sizeof(int) * byteArraySize );
#else
        short arrSocketOption[4] = {0};
#endif SERV_ITEM_OPTION_DATA_SIZE
        //}} 

//#ifdef SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX // 김민성 // 적용날짜: 2013-07-11
        int arrRandomSocketOption[5] = {0};

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> arrSocketOption[4]
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
			>> arrRandomSocketOption[0]
			>> arrRandomSocketOption[1]
			>> arrRandomSocketOption[2]
			>> arrRandomSocketOption[3]
			>> arrRandomSocketOption[4]
			>> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
			>> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
			>> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
			>> kItemInfo.m_iTotalSoldItemQuantity
			>> kItemInfo.m_iTotalSellCommissionED
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory					
			);
#else // SERV_BATTLE_FIELD_BOSS
        FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
            >> arrSocketOption[0]
            >> arrSocketOption[1]
            >> arrSocketOption[2]
            >> arrSocketOption[3]
            >> arrRandomSocketOption[0]
            >> arrRandomSocketOption[1]
            >> arrRandomSocketOption[2]
            >> arrRandomSocketOption[3]
            >> arrRandomSocketOption[4]
            >> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
            >> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
            >> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
            >> kItemInfo.m_iTotalSoldItemQuantity
            >> kItemInfo.m_iTotalSellCommissionED
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory					
            );
#endif // SERV_BATTLE_FIELD_BOSS
        int iCheckRandomIdx;
        for( iCheckRandomIdx = 4; iCheckRandomIdx >= 0; --iCheckRandomIdx )
        {
            if( arrRandomSocketOption[iCheckRandomIdx] != 0 )
                break;
        }

        for( int iIdx = 0; iIdx <= iCheckRandomIdx; ++iIdx )
        {
            kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecRandomSocket.push_back( arrRandomSocketOption[iIdx] );
        }

		int iCheckIdx = byteArraySize;
		while( 0 <= --iCheckIdx )
		{
			if( arrSocketOption[iCheckIdx] != 0 )
				break;
		}

        for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
        {
            kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );
        }

        kAck.m_kPShopAgencyInfo.m_vecSellItemInfo.push_back( kItemInfo );
    }
#else //SERV_UPGRADE_TRADE_SYSTEM

//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
// 대리 상인 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
DO_QUERY( L"exec dbo.P_GPShopInfo_SEL_UnitUID", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
DO_QUERY( L"exec dbo.gup_get_PShop_info_UnitUID", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
if( m_kODBC.BeginFetch() )
{
	FETCH_DATA( kAck.m_kPShopAgencyInfo.m_wstrAgencyExpirationDate
		>> kAck.m_kPShopAgencyInfo.m_bIsPShopOpen
		);

	m_kODBC.EndFetch();
}
else
{
	kAck.m_kPShopAgencyInfo.m_wstrAgencyExpirationDate = L"2000-01-01 00:00:00";
	kAck.m_kPShopAgencyInfo.m_bIsPShopOpen = false;
}
#endif SERV_PSHOP_AGENCY
//}}

#endif //SERV_UPGRADE_TRADE_SYSTEM

#ifdef SERV_ALL_RENEWAL_SP
int iResetHour2 = 6;
#ifdef SERV_COUNTRY_US
	iResetHour2 = 3;
#endif //SERV_COUNTRY_US
#endif //SERV_ALL_RENEWAL_SP

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	// 헤니르 보상 받은 횟수 얻기 (SP 내부에서 1일 횟수 초기화)
	{
		CTime tNow = CTime::GetCurrentTime();
		std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
		DO_QUERY( L"exec dbo.P_GHenirRewardCnt_SEL", L"%d, N\'%s\', %d", % kReq.m_iUnitUID % wstrNow % iResetHour2 );
#else // SERV_HENIR_RENEWAL_2013
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GHenirRewardCnt_SEL", L"%d, N\'%s\', %d", % kReq.m_iUnitUID % wstrNow % iResetHour2 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_henir_reward_cnt", L"%d, N\'%s\'", % kReq.m_iUnitUID % wstrNow );
#endif //SERV_ALL_RENEWAL_SP
#endif // SERV_HENIR_RENEWAL_2013
		if( m_kODBC.BeginFetch() )
		{
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
			FETCH_DATA( iOK
				>> kAck.m_PacketHenirRewardCount.m_iNormal
				>> kAck.m_PacketHenirRewardCount.m_iPremium
				>> kAck.m_PacketHenirRewardCount.m_iEvent
				>> kAck.m_PacketHenirRewardCount.m_iChallengeNormal
				>> kAck.m_PacketHenirRewardCount.m_iChallengePremium
				>> kAck.m_PacketHenirRewardCount.m_iChallengeEvent
				);
#else // SERV_HENIR_RENEWAL_2013
			FETCH_DATA( iOK
				>> kAck.m_PacketHenirRewardCount.m_iNormal
				>> kAck.m_PacketHenirRewardCount.m_iPremium
				>> kAck.m_PacketHenirRewardCount.m_iEvent
				);
#endif // SERV_HENIR_RENEWAL_2013

			m_kODBC.EndFetch();
		}
	}

	switch( iOK )
	{
	case NetError::NET_OK:
		{
			START_LOG( clog, L"헤니르 보상 횟수 얻기 성공" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iPremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iEvent )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengePremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
				;
		}break;
	case -1:
		{
			START_LOG( cerr, L"헤니르 보상 횟수 얻기 실패 - GUnit에 없는 유닛 or 삭제된 유닛" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iPremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iEvent )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengePremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
				;
			kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;
			return false;
		}break;
	case -2:
		{
			START_LOG( cerr, L"헤니르 보상 횟수 초기화 실패 - 초기화 업데이트 실패" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iPremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iEvent )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengePremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
				;
			kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;
			return false;
		}break;
	case -3:
		{
			START_LOG( cerr, L"헤니르 보상 횟수 얻기 실패 - 헤니르 테이블에 데이터가 없거나 여러개" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iPremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iEvent )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengePremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
				;
			kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;
			return false;
		}break;
	default:
		{
			START_LOG( cerr, L"헤니르 보상 횟수 얻기 실패 - 이상한 이유?!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iPremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iEvent )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeNormal )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengePremium )
				<< BUILD_LOG( kAck.m_PacketHenirRewardCount.m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
				;
			kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;
			return false;
		}break;
	}	
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
	if( kAck.m_iOK == NetError::NET_OK )
	{
		DO_QUERY( L"exec dbo.P_GEvent_Unit_GET ", L"%d", % kReq.m_iUnitUID );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iPvpEventIndex
					 >> kAck.m_wstrLastPvpEventDate );

			m_kODBC.EndFetch();
		}
	}
#endif SERV_2012_PVP_SEASON2_EVENT
	//}}
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	//여기서 보상받은 날짜를 받아오자
	DO_QUERY( L"exec dbo.P_GEventChung_GET", L"%d", % kReq.m_iUnitUID );
	{
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_wstrGiveMeTheItemTime_One //아이템 받은 시간
				>> kAck.m_wstrGiveMeTheItemTime_Two 
				>> kAck.m_wstrGiveMeTheItemTime_Tree); 
			//DB에서 받은 정보를 맵에다가 저장 하자.없으면 아무것도 없겠지 뭐
			m_kODBC.EndFetch();
		}
	}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_ADD_WARP_BUTTON
	//////////////////////////////////////////////////////////////////////////
	// 코보 익스프레스 VIP 티켓 구매 정보
	int iPeriod = 0;

	DO_QUERY( L"exec dbo.P_GVIPTicket_SEL", L"%d", % kReq.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( wstrVipEndDate
			>> wstrVipRegDate
			>> iPeriod );

		m_kODBC.EndFetch();

		CTime ctVipEndDate;
		if( KncUtil::ConvertStringToCTime( wstrVipEndDate, ctVipEndDate ) == true )
			kAck.m_kUnitInfo.m_trWarpVipEndData = ctVipEndDate.GetTime();
		else
			kAck.m_kUnitInfo.m_trWarpVipEndData = 0LL;

		START_LOG( clog, L"[TEST] VIP 정보 잘 가져왔는지 체크" )
			<< BUILD_LOG( wstrVipEndDate.c_str() )
			<< BUILD_LOG( wstrVipRegDate.c_str() )
			<< BUILD_LOG( iPeriod )
			<< END_LOG;
	}
	else
	{
		START_LOG( clog, L"코보 익스프레스 VIP 정보 가져오기 실패!! VIP 구매 한 적이 없는 유저다." )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;

		kAck.m_kUnitInfo.m_trWarpVipEndData = 0LL;
	}
#endif // SERV_ADD_WARP_BUTTON

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	if( kAck.m_iOK == NetError::NET_OK )
	{
		std::wstring wstrWeddingDate;
		std::wstring wstrLastRewardDate;

		// 인연 관련 정보를 DB 에서 받아와야 한다.
		DO_QUERY( L"exec dbo.P_GCouple_Info_SEL", L"%d", % kReq.m_iUnitUID );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_kRelationshipInfo.m_iOtherUnitUID
				>> kAck.m_kRelationshipInfo.m_wstrOtherNickName
				>> kAck.m_kRelationshipInfo.m_cRelationshipType
				>> wstrWeddingDate
				>> kAck.m_kRelationshipInfo.m_wstrLoveWord
				>> wstrLastRewardDate
				>> kAck.m_kRelationshipInfo.m_cRewardTitleType	);

			m_kODBC.EndFetch();
		}

		if( kAck.m_kRelationshipInfo.m_cRelationshipType == SEnum::RT_SOLO )
		{
			kAck.m_kRelationshipInfo.m_iOtherUnitUID = 0;
			kAck.m_kRelationshipInfo.m_wstrOtherNickName.clear();
		}
		else
		{
			CTime tWeddingDate;
			KncUtil::ConvertStringToCTime( wstrWeddingDate, tWeddingDate );
			kAck.m_kRelationshipInfo.m_tDate = tWeddingDate.GetTime();

			CTime tLastRewardDate;
			KncUtil::ConvertStringToCTime( wstrLastRewardDate, tLastRewardDate );
			kAck.m_kRelationshipInfo.m_tLastReward = tLastRewardDate.GetTime();
		}
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	//DB에서 버튼을 누른 시간을 받아온다,버튼을 눌렀는지 확인 유무도 받아온다.
	//던전 클리어 횟수랑 필드 몬스터 잡은 횟수도 받아온다.
	DO_QUERY( L"exec dbo.P_GEventCobo_GET", L"%d", % kReq.m_iUnitUID );
	{
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_wstrButtonClickTime_One //버튼 클릭 타임
				>> kAck.m_bItemGive //보상 지급 유무  
				>> kAck.m_iDungeonClearCount //던전 클리어 횟수 
				>> kAck.m_iFieldMonsterKillCount); //필드 몬스터 킬 횟수 
			m_kODBC.EndFetch();
		}
	}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_GROW_UP_SOCKET
	kAck.m_kUnitInfo.m_iEventQuestClearCount = 0;
	kAck.m_kUnitInfo.m_iExchangeCount = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEventTradeCount_SEL", L"%d", % kReq.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEventTradeCount_GET", L"%d", % kReq.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_kUnitInfo.m_iExchangeCount );

		m_kODBC.EndFetch();

		kAck.m_iOK = NetError::NET_OK;
	}
	else
	{
		kAck.m_iOK = NetError::NET_OK;

		START_LOG( clog, L"P_GEventTradeCount_GET 호출 실패! 실패 하면 안되는데??" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;
	}	
#endif SERV_GROW_UP_SOCKET

#ifdef SERV_BLESS_OF_GODDESS_EVENT
	int iMaxLevelUnitInAccount = 0;

	DO_QUERY( L"exec dbo.P_GUnit_MAXLevel_GET", L"%d", % iUserUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iMaxLevelUnitInAccount );

		m_kODBC.EndFetch();
	}

	if( iMaxLevelUnitInAccount >= 45 )
	{
		kAck.m_bMaxLevelUnitInAccount = true;
	}
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	DO_QUERY( L"exec dbo.P_GEvent_GateOfDarkness_SEL", L"%d", % kReq.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iGateOfDarknessSupportEventTime );

		m_kODBC.EndFetch();
	}
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	DO_QUERY( L"exec dbo.P_GEvent_Couple_SEL", L"%d", % kReq.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iRelationTargetUserUid
			>> kAck.m_wstrRelationTargetUserNickname );

		m_kODBC.EndFetch();
	}

	if( kAck.m_iRelationTargetUserUid > 0 )
	{
		kAck.m_bCouple = true;
	}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_ELESIS_UPDATE_EVENT
	DO_QUERY( L"exec dbo.P_GEvent_NoteViewCount_SEL", L"%d", % kReq.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iNoteViewCount );

		m_kODBC.EndFetch();
	}
#endif SERV_ELESIS_UPDATE_EVENT

	// 캐릭터 선택 성공!
	kAck.m_iOK = NetError::NET_OK;
	return true;

end_proc:
	kAck.m_iOK = NetError::ERR_SELECT_UNIT_05;

	START_LOG( cerr, L"케릭터 정보 얻어오는 과정 실패!" )
		<< BUILD_LOG( kAck.m_iOK )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< END_LOG;

	return false;
}
//}}

//{{ 2010. 9. 29	최육사	아이템 정보 리팩토링
#ifdef SERV_GET_INVENTORY_REFAC

bool KGSGameDBThread::Query_GetInventory( IN const UidType iUnitUID, 
										  OUT std::map< int, int >& mapInventorySlotSizeResult,
										  OUT std::map< UidType, KInventoryItemInfo >& mapItemResult,
										  OUT std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItemResult
										  )
{
	mapInventorySlotSizeResult.clear();
	mapItemResult.clear();
	mapPetItemResult.clear();

	typedef std::pair< int, int >	KAttribInfo;

	std::map< UidType, UidType >	mapPetItemList;	 // 펫 아이템 정보
	std::map< UidType, u_char >		mapSealItemList; // 밀봉 아이템 정보	
	std::map< UidType, std::vector< KAttribInfo > > mapAttribItemList; // 속성 아이템 정보
	std::map< UidType, std::vector< KAttribInfo > >::iterator mitAttrib;
	//{{ 2013. 05. 24	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::map< UidType, std::vector< int > > mapRandomSocketList;
	std::map< UidType, std::vector< int > >::iterator mitRS;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//////////////////////////////////////////////////////////////////////////	
	// 인벤토리 사이즈를 얻는다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemInventorySize_SEL", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_inventory_size", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int iCategoty;
		int iNumSlot;

		FETCH_DATA( iCategoty
			>> iNumSlot );

		// 캐릭터 UID, 카테고리 ID가 PK이므로 한 캐릭터에 대한 카테고리 ID는 모두 다르게 저장되어 있다.
		// 따라서 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
		mapInventorySlotSizeResult.insert( std::make_pair( iCategoty, iNumSlot ) );
	}

	//////////////////////////////////////////////////////////////////////////
	// 펫 아이템 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItem_pet_SEL", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_item_list_pet", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		UidType iItemUID = 0;
		UidType iPetUID = 0;

		FETCH_DATA( iItemUID
			>> iPetUID );

		std::map< UidType, UidType >::iterator mitPet;
		//{{ 2012. 12. 17	최육사	iItemUID를 iPetUID로 잘못 넣은것을 수정함.
		mitPet = mapPetItemList.find( iItemUID );
		//}}
		if( mitPet == mapPetItemList.end() )
		{
			mapPetItemList.insert( std::make_pair( iItemUID, iPetUID ) );
		}
		else
		{
			START_LOG( cerr, L"중복되는 ItemUID가 있습니다. 있을 수 없는 에러!" )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iPetUID )
				<< END_LOG;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 속성 강화 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemAttribute_SEL", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_attribute", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		UidType iItemUID = 0;
		int iAttribEnchantSlotNo = 0;
		int iAttribEnchantID = 0;

		FETCH_DATA( iItemUID
			>> iAttribEnchantSlotNo
			>> iAttribEnchantID );

		KAttribInfo kAttribInfo( iAttribEnchantSlotNo, iAttribEnchantID );

		mitAttrib = mapAttribItemList.find( iItemUID );
		if( mitAttrib == mapAttribItemList.end() )
		{
			std::vector< KAttribInfo > vecAttribInfo;
			vecAttribInfo.push_back( kAttribInfo );
			mapAttribItemList.insert( std::make_pair( iItemUID, vecAttribInfo ) );
		}
		else
		{
            mitAttrib->second.push_back( kAttribInfo );
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 밀봉 정보 얻기
	//{{ 2009. 8. 28  최육사	밀봉
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSeal_SEL", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_item_list_seal", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		UidType iItemUID = 0;
		u_char ucSealData = 0;

		FETCH_DATA( iItemUID
			>> ucSealData );

		mapSealItemList.insert( std::make_pair( iItemUID, ucSealData ) );
	}
	//}}

	//{{ 2013. 05. 24	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	// 랜덤 소켓 정보 얻기
	DO_QUERY( L"exec dbo.P_GItemSocket_Random_SEL", L"%d", % iUnitUID );
	while( m_kODBC.Fetch() )
	{
		UidType iItemUID = 0;
		int arrRandomSocketOption[CXSLSocketItem::RSC_MAX] = {0,};

		FETCH_DATA( iItemUID
			>> arrRandomSocketOption[0]
			>> arrRandomSocketOption[1]
			>> arrRandomSocketOption[2]
			>> arrRandomSocketOption[3]
			>> arrRandomSocketOption[4]
			);

		// 2-1. 랜덤 소켓 정보 업데이트
		int iCheckIdx = CXSLSocketItem::RSC_MAX;
		while( 0 <= --iCheckIdx )
		{
			if( arrRandomSocketOption[iCheckIdx] != 0 )
				break;
		}

		std::vector< int > vecRandomSocket;
		for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
		{
			vecRandomSocket.push_back( arrRandomSocketOption[iIdx] );
		}

		// itemuid값은 중복되지 않는것을 db에서 보장해야한다.
		mapRandomSocketList.insert( std::make_pair( iItemUID, vecRandomSocket ) );
	}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

	//////////////////////////////////////////////////////////////////////////	
	// 보유 아이템을 얻는다.	
	//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItem_SEL_ItemList", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItem_ItemList_SEL", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_BATTLE_FIELD_BOSS
	DO_QUERY( L"exec dbo.gup_get_item_list", L"%d", % iUnitUID );
#endif // SERV_BATTLE_FIELD_BOSS
	while( m_kODBC.Fetch() )
	{
		int iEnchantLevel = 0;
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
		const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS
		int arrSocketOption[byteArraySize];
		memset( arrSocketOption, 0, sizeof(int) * byteArraySize );
//#else
//		short arrSocketOption[4] = {0,0,0,0};
//#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		
		KInventoryItemInfo kInventoryItemInfo;

			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		FETCH_DATA( kInventoryItemInfo.m_iItemUID
			>> kInventoryItemInfo.m_kItemInfo.m_iItemID
			>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
			>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
			>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
			>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
			>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
			>> iEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> arrSocketOption[4]
			>> kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
			>> kInventoryItemInfo.m_cSlotCategory
			>> kInventoryItemInfo.m_sSlotID );
#else // SERV_BATTLE_FIELD_BOSS
		FETCH_DATA( kInventoryItemInfo.m_iItemUID
			>> kInventoryItemInfo.m_kItemInfo.m_iItemID
			>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
			>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
			>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
			>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
			>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
			>> iEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> kInventoryItemInfo.m_cSlotCategory
			>> kInventoryItemInfo.m_sSlotID );
#endif // SERV_BATTLE_FIELD_BOSS
//#else
//		FETCH_DATA( kInventoryItemInfo.m_iItemUID
//			>> kInventoryItemInfo.m_kItemInfo.m_iItemID
//			>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
//			>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
//			>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
//			>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
//			>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
//			>> iEnchantLevel
//			>> arrSocketOption[0]
//			>> arrSocketOption[1]
//			>> arrSocketOption[2]
//			>> arrSocketOption[3]
//			>> kInventoryItemInfo.m_cSlotCategory
//			>> kInventoryItemInfo.m_cSlotID );
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}

		// 1. 강화 정보 업데이트
		kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel = static_cast<char>(iEnchantLevel);

		// 2. 소켓 정보 업데이트
		int iCheckIdx = byteArraySize;
		while( 0 <= --iCheckIdx )
		{
			if( arrSocketOption[iCheckIdx] != 0 )
				break;
		}

		for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
		{
			kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );
		}

		//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		// 2-1. 랜덤 소켓 정보 업데이트
		std::map< UidType, std::vector< int > >::const_iterator mitCRS;
		mitCRS = mapRandomSocketList.find( kInventoryItemInfo.m_iItemUID );
		if( mitCRS != mapRandomSocketList.end() )
		{
			kInventoryItemInfo.m_kItemInfo.m_vecRandomSocket = mitCRS->second;
		}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		// 3. 밀봉 정보 업데이트
		std::map< UidType, u_char >::const_iterator mitSeal;
		mitSeal = mapSealItemList.find( kInventoryItemInfo.m_iItemUID );
		if( mitSeal != mapSealItemList.end() )
		{
			kInventoryItemInfo.m_kItemInfo.m_ucSealData = mitSeal->second;
		}

		// 4. 속성 정보 업데이트
		mitAttrib = mapAttribItemList.find( kInventoryItemInfo.m_iItemUID );
		if( mitAttrib != mapAttribItemList.end() )
		{
			BOOST_TEST_FOREACH( const KAttribInfo&, kAttribInfo, mitAttrib->second )
			{
				// 속성 강화
				switch( kAttribInfo.first )
				{
				case CXSLAttribEnchantItem::ESI_SLOT_1:
					kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>( kAttribInfo.second );
					break;

				case CXSLAttribEnchantItem::ESI_SLOT_2:
					kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>( kAttribInfo.second );
					break;

				case CXSLAttribEnchantItem::ESI_SLOT_3:
					kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>( kAttribInfo.second );
					break;

				default:
					START_LOG( cerr, L"속성 강화 슬롯 넘버가 이상합니다." )
						<< BUILD_LOG( kInventoryItemInfo.m_iItemUID )
						<< BUILD_LOG( kAttribInfo.first )
						<< BUILD_LOG( kAttribInfo.second )
						<< END_LOG;
					break;
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////		
		// 펫 카테고리이면 따로 담자!
		if( kInventoryItemInfo.m_cSlotCategory == CXSLInventory::ST_PET )
		{
			std::map< UidType, UidType >::const_iterator mitPet;
			mitPet = mapPetItemList.find( kInventoryItemInfo.m_iItemUID );
			if( mitPet == mapPetItemList.end() )
			{
				START_LOG( cerr, L"펫 아이템 정보에서 해당 아이템을 찾지 못했다! 일어나면 안되는 에러!" )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( kInventoryItemInfo.m_iItemUID )
					<< END_LOG;
				continue;
			}

			const UidType iPetUID = mitPet->second;

			std::map< UidType, std::map< UidType, KInventoryItemInfo > >::iterator mitPetItem;
			mitPetItem = mapPetItemResult.find( iPetUID );
			if( mitPetItem == mapPetItemResult.end() )
			{
				std::map< UidType, KInventoryItemInfo > mapPetItem;
				mapPetItem.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
				mapPetItemResult.insert( std::make_pair( iPetUID, mapPetItem ) );
			}
			else
			{
				mitPetItem->second.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
			}
		}
		//////////////////////////////////////////////////////////////////////////		
		// 펫 카테고리 아닌것들은 일반 인벤토리로!
		else
		{
			// 아이템 UID가 PK이므로 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
			mapItemResult.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
		}
	}

	return true;

end_proc:
	START_LOG( cerr, L"캐릭터 인벤토리 얻기 실패!" )
		<< BUILD_LOG( iUnitUID )
		<< END_LOG;
    return false;
}

#endif SERV_GET_INVENTORY_REFAC
//}}

//{{ 2010. 9. 8	최육사	아이템 획득 사유
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GET_ITEM_REASON
//////////////////////////////////////////////////////////////////////////

bool KGSGameDBThread::Query_InsertItemList( IN const SEnum::GET_ITEM_REASON eGetItemReason, IN const UidType iUnitUID, IN const std::vector< KItemInfo >& vecItemInfo, OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, IN const bool bUnSeal /*= true*/ )
{
	if( SEnum::IsValidGetItemReason( eGetItemReason ) == false )
	{
		START_LOG( cerr, L"유효하지않은 아이템 생성 사유입니다!" )
			<< BUILD_LOG( eGetItemReason )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( vecItemInfo.size() )
			<< END_LOG;
	}

	BOOST_TEST_FOREACH( const KItemInfo&, kNewItemInfo, vecItemInfo )
	{
		KItemInfo kNewItemInfoResult = kNewItemInfo; // 복사본 생성

		//////////////////////////////////////////////////////////////////////////
		// 아이템 생성
		UidType iItemUID = 0;
		int iOK = NetError::ERR_ODBC_01;

		//{{ 2011. 08. 31	최육사	아이템 생성 사유
#ifdef SERV_NEW_ITEM_REASON_DB_LOG
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_INS", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% iUnitUID
			% kNewItemInfo.m_iItemID
			% static_cast<int>(kNewItemInfo.m_cUsageType)
			% kNewItemInfo.m_iQuantity
			% kNewItemInfo.m_sEndurance
			% kNewItemInfo.m_sPeriod
			% static_cast<int>(kNewItemInfo.m_cEnchantLevel)
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
			% static_cast<int>(eGetItemReason)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_item", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% iUnitUID
			% kNewItemInfo.m_iItemID
			% static_cast<int>(kNewItemInfo.m_cUsageType)
			% kNewItemInfo.m_iQuantity
			% kNewItemInfo.m_sEndurance
			% kNewItemInfo.m_sPeriod
			% static_cast<int>(kNewItemInfo.m_cEnchantLevel)
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
			% static_cast<int>(eGetItemReason)
			);
#endif //SERV_ALL_RENEWAL_SP			
#else
		DO_QUERY( L"exec dbo.gup_insert_item", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% iUnitUID
			% kNewItemInfo.m_iItemID
			% static_cast<int>(kNewItemInfo.m_cUsageType)
			% kNewItemInfo.m_iQuantity
			% kNewItemInfo.m_sEndurance
			% kNewItemInfo.m_sPeriod
			% static_cast<int>(kNewItemInfo.m_cEnchantLevel)
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
			% static_cast<int>(kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
			);
#endif SERV_NEW_ITEM_REASON_DB_LOG
		//}}		

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK
				>> iItemUID
				>> kNewItemInfoResult.m_wstrExpirationDate );

			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"아이템 삽입 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iUnitUID )
				<< BUILD_LOG( kNewItemInfo.m_iItemID )
				<< BUILD_LOGc( kNewItemInfo.m_cUsageType )
				<< BUILD_LOG( kNewItemInfo.m_iQuantity )
				<< BUILD_LOG( kNewItemInfo.m_sEndurance )
				<< BUILD_LOG( kNewItemInfo.m_sPeriod )
				<< BUILD_LOGc( kNewItemInfo.m_cEnchantLevel )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
				<< BUILD_LOGc( kNewItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
				<< END_LOG;
			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////		
		//{{ 2009. 8. 27  최육사	밀봉
		if( kNewItemInfo.m_ucSealData > 0 )
		{
			//{{ 2009. 9. 2  최육사		밀봉
			if( kNewItemInfoResult.IsSealedItem()  &&  bUnSeal )
			{
				kNewItemInfoResult.UnsealItem(); // 밀봉 해제
			}
			//}}
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GItemSeal_MER", L"%d, %d", % iItemUID % static_cast<int>(kNewItemInfoResult.m_ucSealData) );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % iItemUID % static_cast<int>(kNewItemInfoResult.m_ucSealData) );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"밀봉 정보 업데이트 실패." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOGc( kNewItemInfoResult.m_ucSealData )
					<< END_LOG;
				goto end_proc;
			}
		}
		//}}

		//////////////////////////////////////////////////////////////////////////		
		// 소켓 정보가 존재한다면 역시 DB에 insert하자!
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-12-11	// 박세훈
		{
			bool bNeedSocketInfoDBUpdate = false;
			
			if( 0 < kNewItemInfo.m_byteExpandedSocketNum )
			{
				bNeedSocketInfoDBUpdate = true;
			}
			else if( kNewItemInfo.m_vecItemSocket.empty() == false )
			{
				for( std::vector<int>::const_iterator it = kNewItemInfo.m_vecItemSocket.begin(); it != kNewItemInfo.m_vecItemSocket.end(); ++it )
				{
					if( 0 < *it )
					{
						bNeedSocketInfoDBUpdate = true;
						break;
					}
				}
			}

			if( bNeedSocketInfoDBUpdate == true )
			{
				const byte byteArraySize = 5;
				int arrSocketInfo[byteArraySize]; // DB 테이블의 소켓테이블 참고
				memset( arrSocketInfo, 0, sizeof(int) * byteArraySize );

				std::vector<int>::const_iterator it = kNewItemInfo.m_vecItemSocket.begin();
				for( int i = 0; i < byteArraySize; ++i )
				{
					if( it == kNewItemInfo.m_vecItemSocket.end() )
						continue;

					arrSocketInfo[i] = *it;
					++it;
				}

#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GItemSocket_MER", L"%d, %d, %d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					% arrSocketInfo[4]			// @iSoket5 smallint
					% kNewItemInfo.m_byteExpandedSocketNum
						);
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GItemSocket_SET", L"%d, %d, %d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					% arrSocketInfo[4]			// @iSoket5 smallint
					% kNewItemInfo.m_byteExpandedSocketNum
					);
#endif //SERV_ALL_RENEWAL_SP

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( iItemUID )
						<< BUILD_LOG( arrSocketInfo[0] )
						<< BUILD_LOG( arrSocketInfo[1] )
						<< BUILD_LOG( arrSocketInfo[2] )
						<< BUILD_LOG( arrSocketInfo[3] )
						<< BUILD_LOG( arrSocketInfo[4] )
						<< END_LOG;
				}
			}
		}
#else // SERV_BATTLE_FIELD_BOSS
		if( kNewItemInfo.m_vecItemSocket.empty() == false )
		{
			bool bSocketExist = false;
			int arrSocketInfo[4] = {0,0,0,0}; // DB 테이블의 소켓테이블 참고
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
			std::vector< int >::const_iterator vitSocket = kNewItemInfo.m_vecItemSocket.begin();
#else
			std::vector< short >::const_iterator vitSocket = kNewItemInfo.m_vecItemSocket.begin();
#endif SERV_ITEM_OPTION_DATA_SIZE
//}} 
			for( int iIdx = 0; iIdx < 4; ++iIdx )
			{
				if( vitSocket == kNewItemInfo.m_vecItemSocket.end() )
					continue;

				if( *vitSocket > 0 )
				{
					bSocketExist = true;
				}
				arrSocketInfo[iIdx] = *vitSocket;
				++vitSocket;
			}

			if( bSocketExist )
			{
				DO_QUERY( L"exec dbo.gup_update_Socket", L"%d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					);

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( iItemUID )
						<< BUILD_LOG( arrSocketInfo[0] )
						<< BUILD_LOG( arrSocketInfo[1] )
						<< BUILD_LOG( arrSocketInfo[2] )
						<< BUILD_LOG( arrSocketInfo[3] )
						<< END_LOG;
				}
			}
		}
#endif // SERV_BATTLE_FIELD_BOSS

		//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		//////////////////////////////////////////////////////////////////////////		
		// 랜덤 소켓 정보가 존재한다면 역시 DB에 insert하자!
		if( kNewItemInfo.m_vecRandomSocket.empty() == false )
		{
			bool bSocketExist = false;
			int arrSocketInfo[CXSLSocketItem::RSC_MAX] = {0,}; // DB 테이블의 소켓테이블 참고
			
			std::vector< int >::const_iterator vitSocket = kNewItemInfo.m_vecRandomSocket.begin();
			for( int iIdx = 0; iIdx < CXSLSocketItem::RSC_MAX; ++iIdx )
			{
				if( vitSocket == kNewItemInfo.m_vecRandomSocket.end() )
					continue;

				if( *vitSocket > 0 )
				{
					bSocketExist = true;
				}
				arrSocketInfo[iIdx] = *vitSocket;
				++vitSocket;
			}

			if( bSocketExist )
			{
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GItemSocket_Random_MER", L"%d, %d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					% arrSocketInfo[4]			// @iSoket5 smallint
					);
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GItemSocket_Random_INT_UPD", L"%d, %d, %d, %d, %d, %d, %d",
					% iUnitUID					// @iUnitUID bigint
					% iItemUID		            // @iItemUID bigint
					% arrSocketInfo[0]			// @iSoket1 smallint
					% arrSocketInfo[1]			// @iSoket2 smallint
					% arrSocketInfo[2]			// @iSoket3 smallint
					% arrSocketInfo[3]			// @iSoket4 smallint
					% arrSocketInfo[4]			// @iSoket5 smallint
					);
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"랜덤 소켓 정보 업데이트 실패." )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( iItemUID )
						<< BUILD_LOG( arrSocketInfo[0] )
						<< BUILD_LOG( arrSocketInfo[1] )
						<< BUILD_LOG( arrSocketInfo[2] )
						<< BUILD_LOG( arrSocketInfo[3] )
						<< BUILD_LOG( arrSocketInfo[4] )
						<< END_LOG;
				}
			}
		}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		// 생성되는 아이템 정보를 모두 DB에 insert하였으므로 컨테이너에도 넣자!
		mapInsertedItemInfo.insert( std::make_pair( iItemUID, kNewItemInfoResult ) );
	}

	return true;

end_proc:
	return false;
}

//////////////////////////////////////////////////////////////////////////
//#else
////////////////////////////////////////////////////////////////////////////
//
//bool KGSGameDBThread::Query_InsertItemList( IN const UidType iUnitUID, IN const std::vector< KItemInfo >& vecItemInfo, OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, IN const bool bUnSeal /*= true*/ )
//{
//	std::vector< KItemInfo >::const_iterator vit;
//	for( vit = vecItemInfo.begin(); vit != vecItemInfo.end(); ++vit )
//	{
//		UidType iItemUID;
//		KItemInfo kItemInfo = *vit;
//		//{{ 2009. 9. 2  최육사		밀봉
//		if( kItemInfo.IsSealedItem()  &&  bUnSeal )
//		{
//			kItemInfo.m_ucSealData -= 100; // 밀봉 해제
//		}
//		//}}
//
//		//////////////////////////////////////////////////////////////////////////
//		// 아이템 생성
//		int iOK = NetError::ERR_ODBC_01;
//
//		DO_QUERY( L"exec dbo.gup_insert_item", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
//			% iUnitUID
//			% vit->m_iItemID
//			% static_cast<int>(vit->m_cUsageType)
//			% vit->m_iQuantity
//			% vit->m_sEndurance
//			% vit->m_sPeriod
//			% static_cast<int>(vit->m_cEnchantLevel) 
//			% static_cast<int>(vit->m_kAttribEnchantInfo.m_cAttribEnchant0 ) 
//			% static_cast<int>(vit->m_kAttribEnchantInfo.m_cAttribEnchant1 )
//			//{{ 2009. 11. 9  최육사	트리플속성
//			% static_cast<int>(vit->m_kAttribEnchantInfo.m_cAttribEnchant2 )
//			//}}
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK
//				>> iItemUID
//				>> kItemInfo.m_wstrExpirationDate );
//
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK == NetError::NET_OK )
//		{
//			// 아이템 UID 는 유일하므로 중복 검사 없이 map에 key로 넣을 수 있다.
//			mapInsertedItemInfo.insert( std::make_pair( iItemUID, kItemInfo ) );
//		}
//		else
//		{
//			START_LOG( cerr, L"아이템 삽입 실패." )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( iUnitUID )
//				<< BUILD_LOG( vit->m_iItemID )
//				<< BUILD_LOGc( vit->m_cUsageType )
//				<< BUILD_LOG( vit->m_iQuantity )
//				<< BUILD_LOG( vit->m_sEndurance )
//				<< BUILD_LOG( vit->m_sPeriod )
//				<< BUILD_LOGc( vit->m_cEnchantLevel )
//				<< BUILD_LOGc( vit->m_kAttribEnchantInfo.m_cAttribEnchant0 )
//				<< BUILD_LOGc( vit->m_kAttribEnchantInfo.m_cAttribEnchant1 )
//				//{{ 2009. 11. 9  최육사	트리플속성
//				<< BUILD_LOGc( vit->m_kAttribEnchantInfo.m_cAttribEnchant2 )
//				//}}
//				<< END_LOG;
//			goto end_proc;
//		}
//
//		//////////////////////////////////////////////////////////////////////////		
//		//{{ 2009. 8. 27  최육사	밀봉
//		if( vit->m_ucSealData > 0 )
//		{
//			DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % iItemUID % static_cast<int>(kItemInfo.m_ucSealData) );
//
//			if( m_kODBC.BeginFetch() )
//			{
//				FETCH_DATA( iOK );
//				m_kODBC.EndFetch();
//			}
//
//			if( iOK != NetError::NET_OK )
//			{
//				START_LOG( cerr, L"밀봉 정보 업데이트 실패." )
//					<< BUILD_LOG( iOK )
//					<< BUILD_LOG( iItemUID )
//					<< BUILD_LOGc( kItemInfo.m_ucSealData )
//					<< END_LOG;
//			}
//		}
//		//}}
//
//		//////////////////////////////////////////////////////////////////////////		
//		// 소켓이 비어있으면 다음 아이템으로..
//		if( vit->m_vecItemSocket.empty() )
//			continue;
//
//		bool bSocketExist = false;
//		int arrSocketInfo[4] = {0,0,0,0}; // DB 테이블의 소켓테이블 참고
//		std::vector< short >::const_iterator vitSocket = vit->m_vecItemSocket.begin();
//		for( int iIdx = 0; iIdx < 4; ++iIdx )
//		{
//			if( vitSocket == vit->m_vecItemSocket.end() )
//				continue;
//
//			if( *vitSocket > 0 )
//			{
//				bSocketExist = true;
//			}
//			arrSocketInfo[iIdx] = *vitSocket;
//			++vitSocket;
//		}
//
//		if( bSocketExist )
//		{
//			DO_QUERY( L"exec dbo.gup_update_Socket", L"%d, %d, %d, %d, %d, %d",
//				% iUnitUID					// @iUnitUID bigint
//				% iItemUID		            // @iItemUID bigint
//				% arrSocketInfo[0]			// @iSoket1 smallint
//				% arrSocketInfo[1]			// @iSoket2 smallint
//				% arrSocketInfo[2]			// @iSoket3 smallint
//				% arrSocketInfo[3]			// @iSoket4 smallint
//				);
//
//			if( m_kODBC.BeginFetch() )
//			{
//				FETCH_DATA( iOK );
//				m_kODBC.EndFetch();
//			}
//
//			if( iOK != NetError::NET_OK )
//			{
//				START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
//					<< BUILD_LOG( iOK )
//					<< BUILD_LOG( iUnitUID )
//					<< BUILD_LOG( iItemUID )
//					<< BUILD_LOG( arrSocketInfo[0] )
//					<< BUILD_LOG( arrSocketInfo[1] )
//					<< BUILD_LOG( arrSocketInfo[2] )
//					<< BUILD_LOG( arrSocketInfo[3] )
//					<< END_LOG;
//			}
//		}
//	}
//
//	return true;
//
//end_proc:
//	return false;
//}

//////////////////////////////////////////////////////////////////////////
#endif SERV_GET_ITEM_REASON
//////////////////////////////////////////////////////////////////////////
//}}

void KGSGameDBThread::Query_UpdateSealItem( IN const std::set< int >& setSealItem, IN OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo )
{
	// 밀봉 아이템 리스트가 없으면 아무 처리 안함
	if( setSealItem.empty() )
		return;

	std::map< UidType, KItemInfo >::iterator mit;
	for( mit = mapInsertedItemInfo.begin(); mit != mapInsertedItemInfo.end(); ++mit )
	{
		// 밀봉 대상인지 검사
		if( setSealItem.find( mit->second.m_iItemID ) == setSealItem.end() )
			continue;

		// 기간제 아이템이면 봉인 안함
		if( mit->second.m_sPeriod > 0 )
			continue;

		// 밀봉처리한다!
		mit->second.m_ucSealData = 100;

		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemSeal_MER", L"%d, %d", % mit->first % static_cast<int>(mit->second.m_ucSealData) );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % mit->first % static_cast<int>(mit->second.m_ucSealData) );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"봉인 DB 업데이트 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOGc( mit->second.m_ucSealData )
				<< END_LOG;

			// 밀봉 DB업데이트 실패했으면 롤백
			mit->second.m_ucSealData = 0;
		}
	}
}

void KGSGameDBThread::Query_UpdateDungeonClear( UidType iUnitUID, std::map< int, KDungeonClearInfo >& mapDungeonClearInfo, std::vector< int >& vecFailed )
{
    vecFailed.clear();
    std::map< int, KDungeonClearInfo >::iterator mit = mapDungeonClearInfo.begin();

    while( mit != mapDungeonClearInfo.end() )
    {
        int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GDungeonClear_MER", L"%d, %d, %d, %d, N\'%s\'",
			% iUnitUID
			% mit->second.m_iDungeonID
			% mit->second.m_iMaxScore
			% (int)mit->second.m_cMaxTotalRank
			% mit->second.m_wstrClearTime
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_dungeon_clear", L"%d, %d, %d, %d, N\'%s\'",
			% iUnitUID
			% mit->second.m_iDungeonID
			% mit->second.m_iMaxScore
			% (int)mit->second.m_cMaxTotalRank
			% mit->second.m_wstrClearTime
			);
#endif //SERV_ALL_RENEWAL_SP
        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( iOK );
            m_kODBC.EndFetch();
        }

        if( iOK != NetError::NET_OK )
        {
            START_LOG( cerr, L"던전 클리어 정보 업데이트 실패." )
                << BUILD_LOG( iOK )
                << END_LOG;

            goto end_proc;
        }

        mit = mapDungeonClearInfo.erase( mit );
        continue;

end_proc:
        mit++;
    }

    for( mit = mapDungeonClearInfo.begin(); mit != mapDungeonClearInfo.end(); mit++ )
    {
        vecFailed.push_back( mit->first );
    }
    return;
}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
void KGSGameDBThread::Query_UpdateDungeonPlay( IN UidType iUnitUID, IN std::map< int, KDungeonPlayInfo >& mapDungeonPlayInfo )
{
	std::map< int, KDungeonPlayInfo >::iterator mitDungeonPlayInfo = mapDungeonPlayInfo.begin();

	while( mitDungeonPlayInfo != mapDungeonPlayInfo.end() )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GEventDungeonData_MER", L"%d, %d, %d, %d",
			% iUnitUID
			% mitDungeonPlayInfo->second.m_iDungeonID
			% mitDungeonPlayInfo->second.m_iPlayTimes
			% mitDungeonPlayInfo->second.m_iClearTimes
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GEventDungeonData_SET", L"%d, %d, %d, %d",
			% iUnitUID
			% mitDungeonPlayInfo->second.m_iDungeonID
			% mitDungeonPlayInfo->second.m_iPlayTimes
			% mitDungeonPlayInfo->second.m_iClearTimes
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"던전 플레이 정보 업데이트 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( mitDungeonPlayInfo->second.m_iDungeonID )
				<< BUILD_LOG( mitDungeonPlayInfo->second.m_iPlayTimes )
				<< BUILD_LOG( mitDungeonPlayInfo->second.m_iClearTimes )
				<< END_LOG;

			goto end_proc;
		}

end_proc:
		mitDungeonPlayInfo++;
	}
}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

void KGSGameDBThread::Query_UpdateTCClear( UidType iUnitUID, std::map< int, KTCClearInfo >& mapTCClearInfo, std::vector< int >& vecFailed )
{
    vecFailed.clear();
	std::map< int, KTCClearInfo >::iterator mit = mapTCClearInfo.begin();

	while( mit != mapTCClearInfo.end() )
	{
        int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTrainingCenter_INS", L"%d, %d, N\'%s\'",
			% iUnitUID
			% mit->second.m_iTCID
			% mit->second.m_wstrClearTime
			);	
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_tc_clear", L"%d, %d, N\'%s\'",
			% iUnitUID
			% mit->second.m_iTCID
			% mit->second.m_wstrClearTime
			);	
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"훈련소 클리어 정보 업데이트 실패." )
				<< BUILD_LOG( iOK )
				<< END_LOG;

			goto end_proc;
		}

		mit = mapTCClearInfo.erase( mit );
		continue;

end_proc:
		mit++;
	}

    for( mit = mapTCClearInfo.begin(); mit != mapTCClearInfo.end(); mit++ )
    {
        vecFailed.push_back( mit->first );
    }
	return;
}

bool KGSGameDBThread::Query_UpdateItemQuantity( UidType iUnitUID, std::map< UidType, int >& mapUpdated, std::map< UidType, int >& mapFailed, bool& bUpdateFailed )
{
	//{{ 2009. 11. 17  최육사	DB쿼리실패	
	bool bQuerySuccess = true;
	//}}
	bUpdateFailed = false;
    mapFailed.clear();

    std::map< UidType, int >::iterator mit = mapUpdated.begin();
    while( mit != mapUpdated.end() )
    {
        int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD", L"%d, %d, %d, %d", 
			% iUnitUID 
			% mit->first 
			% CXSLItem::PT_QUANTITY 
			% mit->second
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item", L"%d, %d, %d, %d", 
			% iUnitUID 
			% mit->first 
			% CXSLItem::PT_QUANTITY 
			% mit->second
			);
#endif //SERV_ALL_RENEWAL_SP
// 		START_LOG( cerr, L"업데이트 내용 확인하자" )
// 			<< BUILD_LOG( iUnitUID )
// 			<< BUILD_LOG( mit->first  )
// 			<< BUILD_LOG( mit->second  )
// 			<< END_LOG;

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( iOK );
            m_kODBC.EndFetch();

            if( iOK != NetError::NET_OK )
            {
                START_LOG( cerr, L"아이템 정보 업데이트 실패." )
                    << BUILD_LOG( iOK )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
                    << END_LOG;

				//{{ 2008. 10. 23  최육사	증분값 버그한번 잡아보자~!
				if( iOK == -5 )
				{
					START_LOG( cout, L"증분값 업데이트 버그 발생!" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( iUnitUID )
						<< BUILD_LOG( mit->first )
						<< BUILD_LOG( mit->second );

					bUpdateFailed = true;
				}
				//}}
            }
        }
        else
        {
            START_LOG( cerr, L"아이템 수량 업데이트중 BeginFetch 실패." );
        }

end_proc:
        if( iOK != NetError::NET_OK  &&  iOK != -5 )
        {
			//{{ 2009. 11. 17  최육사	DB쿼리실패
			bQuerySuccess = false;
			//}}

			mapFailed.insert( std::make_pair( mit->first, mit->second ) );
        }
        mit++;
    }

	return bQuerySuccess;
}

bool KGSGameDBThread::Query_UpdateItemEndurance( IN UidType iUnitUID, IN std::map< UidType, int >& mapUpdated, OUT std::map< UidType, int >& mapFailed )
{
	//{{ 2009. 11. 17  최육사	DB쿼리실패
	bool bQuerySuccess = true;
	//}}

	mapFailed.clear();

	std::map< UidType, int >::iterator mit = mapUpdated.begin();
	while( mit != mapUpdated.end() )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD", L"%d, %d, %d, %d", 
			% iUnitUID
			% mit->first 
			% CXSLItem::PT_ENDURANCE 
			% mit->second 
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item", L"%d, %d, %d, %d", 
			% iUnitUID
			% mit->first 
			% CXSLItem::PT_ENDURANCE 
			% mit->second 
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 정보 업데이트 실패." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"아이템 내구도 업데이트중 BeginFetch 실패." );
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			//{{ 2009. 11. 17  최육사	DB쿼리실패
			bQuerySuccess = false;
			//}}

			mapFailed.insert( std::make_pair( mit->first, mit->second ) );
		}
		mit++;
	}

	return bQuerySuccess;
}

//void KGSGameDBThread::UpdateItemPosition( IN std::map< UidType, std::pair< int, int > >& mapUpdated, OUT std::map< UidType, std::pair< int, int > >& mapFailed, IN bool bFinal )
//{
//	mapFailed.clear();
//
//	std::map< UidType, std::pair< int, int > >::iterator mit = mapUpdated.begin();
//	while( mit != mapUpdated.end() )
//	{
//		int iOK = NetError::ERR_UNKNOWN;
//		DO_QUERY( ( L"exec dbo.gup_update_item_position %d, %d, %d" ) % mit->first % mit->second.first % mit->second.second );
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//
//			if( iOK != NetError::NET_OK )
//			{
//				START_LOG( cerr, L"아이템 위치 업데이트 실패." )
//					<< BUILD_LOG( iOK )
//					<< END_LOG;
//			}
//		}
//		else
//		{
//			START_LOG( cerr, L"아이템 위치 업데이트중 BeginFetch 실패." );
//		}
//
//end_proc:
//		if( iOK != NetError::NET_OK )
//		{
//			mapFailed.insert( std::make_pair( mit->first, mit->second ) );
//
//			//{{ 2008. 7. 8  최육사  인벤 카테고리 버그 찾기
//			START_LOG( cerr, L"인벤 버그일까?!" )
//				<< BUILD_LOG( bFinal );
//			//}}	
//		}
//		mit++;
//	}
//}

//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

void KGSGameDBThread::Query_UpdateItemPosition( IN UidType iUnitUID, IN std::map< UidType, KItemPosition >& mapUpdated, OUT std::map< UidType, KItemPosition >& mapFailed )
{
	mapFailed.clear();

	std::map< UidType, KItemPosition >::iterator mit = mapUpdated.begin();
	while( mit != mapUpdated.end() )
	{
		int iOK = NetError::ERR_ODBC_01;

		// 펫 인벤토리와 관련이 있는 변경인지 체크!
		if( mit->second.m_iPetUID == KItemPosition::IPE_INVALID_PET_UID )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GItem_UPD_Postion", L"%d, %d, %d",
				% mit->first
				% mit->second.m_iSlotCategory
				% mit->second.m_iSlotID
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_item_position", L"%d, %d, %d",
				% mit->first
				% mit->second.m_iSlotCategory
				% mit->second.m_iSlotID
				);
#endif //SERV_ALL_RENEWAL_SP
		}
		else
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GItem_Pet_MER", L"%d, %d, %d, %d, %d", 
				% iUnitUID
				% mit->first
				% mit->second.m_iPetUID
				% mit->second.m_iSlotCategory
				% mit->second.m_iSlotID
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_item_position_pet", L"%d, %d, %d, %d, %d", 
				% iUnitUID
				% mit->first
				% mit->second.m_iPetUID
				% mit->second.m_iSlotCategory
				% mit->second.m_iSlotID
				);
#endif //SERV_ALL_RENEWAL_SP
		}

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 위치 업데이트 실패." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second.m_iPetUID )
					<< BUILD_LOG( mit->second.m_iSlotCategory )
					<< BUILD_LOG( mit->second.m_iSlotID )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"아이템 위치 업데이트중 BeginFetch 실패." );
		}

end_proc:
		if( iOK == NetError::NET_OK )
		{
			mapFailed.insert( std::make_pair( mit->first, mit->second ) );
		}
		mit++;
	}
}

#else

void KGSGameDBThread::Query_UpdateItemPosition( IN UidType iUnitUID, IN std::map< UidType, std::pair< int, int > >& mapUpdated, OUT std::map< UidType, std::pair< int, int > >& mapFailed )
{
	mapFailed.clear();

	std::map< UidType, std::pair< int, int > >::iterator mit = mapUpdated.begin();
	while( mit != mapUpdated.end() )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_Postion", L"%d, %d, %d", % mit->first % mit->second.first % mit->second.second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item_position", L"%d, %d, %d", % mit->first % mit->second.first % mit->second.second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 위치 업데이트 실패." )
					<< BUILD_LOG( iOK )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"아이템 위치 업데이트중 BeginFetch 실패." );
		}

end_proc:
		if( iOK == NetError::NET_OK )
		{
			mapFailed.insert( std::make_pair( mit->first, mit->second ) );
		}
		mit++;
	}
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2009. 12. 15  최육사	아이템 삭제사유
void KGSGameDBThread::Query_DeleteItem( IN std::vector< KDeletedItemInfo >& vecDeleted, OUT std::vector< KDeletedItemInfo >& vecFailed )
{
    vecFailed.clear();

    std::vector< KDeletedItemInfo >::iterator vit = vecDeleted.begin();
    while( vit != vecDeleted.end() )
    {
        int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_DEL", L"%d, %d", % vit->m_iItemUID % (int)vit->m_ucDeleteReason );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_item", L"%d, %d", % vit->m_iItemUID % (int)vit->m_ucDeleteReason );
#endif //SERV_ALL_RENEWAL_SP
// 		START_LOG( cerr, L"삭제 내용 확인하자" )
// 			<< BUILD_LOG( vit->m_iItemUID )
// 			<< END_LOG;

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( iOK );
            m_kODBC.EndFetch();

            if( iOK != NetError::NET_OK )
            {
                START_LOG( cerr, L"아이템 삭제 실패." )
                    << BUILD_LOG( iOK )
					<< BUILD_LOG( vit->m_iItemUID )
					<< BUILD_LOGc( vit->m_ucDeleteReason )
                    << END_LOG;
            }
        }
        else
        {
            START_LOG( cerr, L"아이템 삭제 중 BeginFetch 실패." );
        }
end_proc:
        if( iOK != NetError::NET_OK )
        {
            vecFailed.push_back( *vit );
        }
        ++vit;
    }
}
//}}

//{{ 2009. 10. 28  최육사	길드레벨
bool KGSGameDBThread::Query_GetGuildMemberGrade( IN UidType iUnitUID, OUT u_char& ucGuildMemberGrade )
{
	ucGuildMemberGrade = 0;
	int iHonorPointDummy = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_SEL_Grade", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_member_grade", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( ucGuildMemberGrade
			>> iHonorPointDummy );

		m_kODBC.EndFetch();
	}

	if( ucGuildMemberGrade == -1 )
	{
		START_LOG( cwarn, L"길드원 등급 정보가 존재하지 않음." )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;

		goto end_proc;
	}

	return true;

end_proc:
	return false;
}
//}}

#ifdef GIANT_RESURRECTION_CASHSTONE
//{{ //2011.12.19 lygan_조성욱 // 중국에 캐쉬샵에서 구입되는 부활석 잔존관련 작업(재무 심사용)
int KGSGameDBThread::Query_UpdateResurrection_stone( IN const UidType iUnitUID, IN const int iNumResurrectionStone, OUT int& iACKNumResurrectionStone)
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GResurrectionStone_MER", L"%d, %d", % iUnitUID % iNumResurrectionStone );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_resurrection_stone", L"%d, %d", % iUnitUID % iNumResurrectionStone );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK == NetError::NET_OK )
	{
		iACKNumResurrectionStone = 0;
	}
	else
	{
		START_LOG( cerr, L"부활석 업데이트 실패." )
			<< BUILD_LOG( iOK )
			<< END_LOG;
		goto end_proc;
	}

	return iOK;

end_proc:
	return iOK;
}
//}}
#endif //GIANT_RESURRECTION_CASHSTONE

//{{ 2012. 10. 23	최육사		DB업데이트 코드 리팩토링
#ifdef SERV_DB_UPDATE_UNIT_INFO_REFACTORING
bool KGSGameDBThread::Query_UpdateUnitInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck )
{
	int iOK = NetError::ERR_ODBC_01;

	int iDummpMapID = 0;

	//{{ 2011. 10. 26	최육사	DB해킹 트랩
#ifdef SERV_DB_HACKING_ED_UPDATE_TRAP

#ifdef SERV_SKILL_PAGE_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_Basic", L"%d, %d, %d, %d, %d, %d, %d",

		% kReq.m_iUnitUID
		% kReq.m_iEXP
		% kReq.m_iLevel
		% kReq.m_iED
		% iDummpMapID
		% kReq.m_iSpirit
		% kReq.m_bIsSpiritUpdated
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_UI", L"%d, %d, %d, %d, %d, %d, %d",
		
		% kReq.m_iUnitUID
		% kReq.m_iEXP
		% kReq.m_iLevel
		% kReq.m_iED
		% iDummpMapID
		% kReq.m_iSpirit
		% kReq.m_bIsSpiritUpdated
		);
#endif //SERV_ALL_RENEWAL_SP

#else // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_iEXP
		% kReq.m_iLevel
		% kReq.m_iED
		% kReq.m_iSPoint
		% iDummpMapID
		% kReq.m_iSpirit
		% kReq.m_bIsSpiritUpdated
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_ui_5", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_iEXP
		% kReq.m_iLevel
		% kReq.m_iED
		% kReq.m_iSPoint
		
		% iDummpMapID
		% kReq.m_iSpirit
		% kReq.m_bIsSpiritUpdated
		);
#endif //SERV_ALL_RENEWAL_SP
#endif // SERV_SKILL_PAGE_SYSTEM

#else //SERV_DB_HACKING_ED_UPDATE_TRAP

	//{{ 2011. 07. 22	최육사	대전 개편
	//#ifdef SERV_PVP_NEW_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_UnitInfo", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_iEXP
		% kReq.m_iLevel
		% kReq.m_iED
		% kReq.m_iSPoint		
		% kReq.m_iMapID
		% kReq.m_iSpirit
		% kReq.m_bIsSpiritUpdated
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_unit_info", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_iEXP
		% kReq.m_iLevel
		% kReq.m_iED
		% kReq.m_iSPoint		
		% kReq.m_iMapID
		% kReq.m_iSpirit
		% kReq.m_bIsSpiritUpdated
		);
#endif //SERV_ALL_RENEWAL_SP		
#endif SERV_DB_HACKING_ED_UPDATE_TRAP
	//}}	

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

#ifdef SERV_CHINA_SPIRIT_EVENT
	if( iOK == NetError::NET_OK )
	{
		DO_QUERY( L"exec dbo.P_GEventChinaTicketSpirit_SET", L"%d, %d, %d, %d, %d, %d, %d", 
			% kReq.m_iUnitUID
			% kReq.m_arrChinaSpirit[0]
			% kReq.m_arrChinaSpirit[1]
			% kReq.m_arrChinaSpirit[2]
			% kReq.m_arrChinaSpirit[3]
			% kReq.m_arrChinaSpirit[4]
			% kReq.m_arrChinaSpirit[5]
			);
	}
#endif //SERV_CHINA_SPIRIT_EVENT

end_proc:
	if( iOK == NetError::NET_OK )
	{
		kAck.m_iEXP = 0;
		kAck.m_iED = 0;
		//{{ 2011. 07. 22	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
#else
		kAck.m_iVSPoint = 0;
		kAck.m_iVSPointMax = 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		return true;
	}
	else
	{
		START_LOG( cerr, L"캐릭터 기본 정보 업데이트 실패." )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_iEXP )
			<< BUILD_LOG( kReq.m_iLevel )
			<< BUILD_LOG( kReq.m_iED )

#ifdef SERV_SKILL_PAGE_SYSTEM
#else // SERV_SKILL_PAGE_SYSTEM
			<< BUILD_LOG( kReq.m_iSPoint )
#endif // SERV_SKILL_PAGE_SYSTEM


			<< BUILD_LOG( iDummpMapID )
			<< BUILD_LOG( kReq.m_iSpirit )
			<< BUILD_LOG( kReq.m_bIsSpiritUpdated )
			<< END_LOG;
		return false;
	}
}

bool KGSGameDBThread::Query_UpdatePvpInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck )
{
	int iOK = NetError::ERR_ODBC_01;

	//{{ 2011. 07. 22	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_UPD",
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_iOfficialMatchCnt
		% kReq.m_iRating
		% kReq.m_iMaxRating
		% kReq.m_iRPoint
		% kReq.m_iAPoint
		% kReq.m_bIsWinBeforeMatch
		% (int)kReq.m_cEmblemEnum
		% kReq.m_iWin
		% kReq.m_iLose
		% kReq.m_fKFactor
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_UPT",
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_iOfficialMatchCnt
		% kReq.m_iRating
		% kReq.m_iMaxRating
		% kReq.m_iRPoint
		% kReq.m_iAPoint
		% kReq.m_bIsWinBeforeMatch
		% (int)kReq.m_cEmblemEnum
		% kReq.m_iWin
		% kReq.m_iLose
		% kReq.m_fKFactor
		);
#endif //SERV_ALL_RENEWAL_SP
#else
	DO_QUERY( L"exec dbo.gup_update_unitpvp"
		, L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_iOfficialMatchCnt
		% kReq.m_iRating
		% kReq.m_iMaxRating
		% kReq.m_iRPoint
		% kReq.m_iAPoint
		% kReq.m_bIsWinBeforeMatch
		% (int)kReq.m_cEmblemEnum
		% kReq.m_iWin
		% kReq.m_iLose
		);
#endif SERV_2012_PVP_SEASON2
	//}}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK == NetError::NET_OK )
	{
		kAck.m_iOfficialMatchCnt = 0;
		kAck.m_iRating = 0;
		kAck.m_iMaxRating = 0;
		kAck.m_iRPoint = 0;
		kAck.m_iAPoint = 0;
		kAck.m_iWin = 0;
		kAck.m_iLose = 0;
		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		kAck.m_fKFactor = 0.f;
#endif SERV_2012_PVP_SEASON2
		//}}
		return true;
	}
	else
	{
		START_LOG( cerr, L"유닛 대전 정보 업데이트 실패." )
			<< BUILD_LOG( iOK )
			//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_iOfficialMatchCnt )
			<< BUILD_LOG( kReq.m_iRating )
			<< BUILD_LOG( kReq.m_iMaxRating )
			<< BUILD_LOG( kReq.m_iRPoint )
			<< BUILD_LOG( kReq.m_iAPoint )
			<< BUILD_LOG( kReq.m_bIsWinBeforeMatch )
			<< BUILD_LOGc( kReq.m_cEmblemEnum )
			<< BUILD_LOG( kReq.m_iWin )
			<< BUILD_LOG( kReq.m_iLose )
			<< BUILD_LOG( kReq.m_fKFactor )
#endif SERV_2012_PVP_SEASON2
			//}}
			<< END_LOG;
		return false;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

bool KGSGameDBThread::Query_UpdatePvpInfo( IN const KDBE_QUIT_USER_PVP_RESULT_UPDATE_NOT& kNot )
{
	int iOK = NetError::ERR_ODBC_01;

	//{{ 2011. 07. 22	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_UPD",
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kNot.m_iUnitUID
		% kNot.m_iOfficialMatchCnt
		% kNot.m_iRating
		% kNot.m_iMaxRating
		% kNot.m_iRPoint
		% kNot.m_iAPoint
		% kNot.m_bIsWinBeforeMatch
		% (int)kNot.m_cEmblemEnum
		% kNot.m_iWin
		% kNot.m_iLose
		% kNot.m_fKFactor
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_UPT",
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kNot.m_iUnitUID
		% kNot.m_iOfficialMatchCnt
		% kNot.m_iRating
		% kNot.m_iMaxRating
		% kNot.m_iRPoint
		% kNot.m_iAPoint
		% kNot.m_bIsWinBeforeMatch
		% (int)kNot.m_cEmblemEnum
		% kNot.m_iWin
		% kNot.m_iLose
		% kNot.m_fKFactor
		);
#endif //SERV_ALL_RENEWAL_SP
#else
	DO_QUERY( L"exec dbo.gup_update_unitpvp"
		, L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kNot.m_iUnitUID
		% kNot.m_iOfficialMatchCnt
		% kNot.m_iRating
		% kNot.m_iMaxRating
		% kNot.m_iRPoint
		% kNot.m_iAPoint
		% kNot.m_bIsWinBeforeMatch
		% (int)kNot.m_cEmblemEnum
		% kNot.m_iWin
		% kNot.m_iLose
		);
#endif SERV_2012_PVP_SEASON2
	//}}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK == NetError::NET_OK )
	{
		return true;
	}
	else
	{
		START_LOG( cerr, L"유닛 대전 정보 업데이트 실패." )
			<< BUILD_LOG( iOK )
			//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
			<< BUILD_LOG( kNot.m_iUnitUID )
			<< BUILD_LOG( kNot.m_iOfficialMatchCnt )
			<< BUILD_LOG( kNot.m_iRating )
			<< BUILD_LOG( kNot.m_iMaxRating )
			<< BUILD_LOG( kNot.m_iRPoint )
			<< BUILD_LOG( kNot.m_iAPoint )
			<< BUILD_LOG( kNot.m_bIsWinBeforeMatch )
			<< BUILD_LOGc( kNot.m_cEmblemEnum )
			<< BUILD_LOG( kNot.m_iWin )
			<< BUILD_LOG( kNot.m_iLose )
			<< BUILD_LOG( kNot.m_fKFactor )
#endif SERV_2012_PVP_SEASON2
			//}}
			<< END_LOG;
		return false;
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}
}

bool KGSGameDBThread::Query_UpdateLastGamePoint( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	int iOK = NetError::ERR_ODBC_01;
	
	// 해당 캐릭터의 마지막 위치 정보 저장!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_LastPoint_UPD", L"%d, %d", % kReq.m_iUnitUID % kReq.m_iRealDataED );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_unit_lastpoint", L"%d, %d", % kReq.m_iUnitUID % kReq.m_iRealDataED );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"로그 오프 ED체크 정보 업데이트 실패." )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_iRealDataED )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KGSGameDBThread::Query_UpdateResurrectionStone( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck )
{
	// 중국의 경우 부활석 변화량이 없어도 프로시저 호출함
#ifndef GIANT_RESURRECTION_CASHSTONE
	// 부활석 변화량이 없으면 프로시저 호출안함. (중국제외)
	if( kReq.m_iNumResurrectionStone != 0 )
#endif //GIANT_RESURRECTION_CASHSTONE
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GResurrectionStone_MER", L"%d, %d", % kReq.m_iUnitUID % kReq.m_iNumResurrectionStone );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_resurrection_stone", L"%d, %d", % kReq.m_iUnitUID % kReq.m_iNumResurrectionStone );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK == NetError::NET_OK )
		{
			kAck.m_iNumResurrectionStone = 0;
		}
		else
		{
			START_LOG( cerr, L"부활석 업데이트 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_iNumResurrectionStone )
				<< END_LOG;
			goto end_proc;
		}
	}

	//{{ 2009. 10. 14  최육사	자동결제 부활석
#ifdef AP_RESTONE
	if( kReq.m_iNumAutoPaymentResStone != 0 )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GResurrectionStone_AutoPay_MER", L"%d, %d", % kReq.m_iUnitUID % kReq.m_iNumAutoPaymentResStone );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_resurrection_stone_autopay", L"%d, %d", % kReq.m_iUnitUID % kReq.m_iNumAutoPaymentResStone );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK == NetError::NET_OK )
		{
			kAck.m_iNumAutoPaymentResStone = 0;
		}
		else
		{
			START_LOG( cerr, L"자동결제 부활석 업데이트 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_iNumAutoPaymentResStone )
				<< END_LOG;
			goto end_proc;
		}
	}
#endif AP_RESTONE
	//}}

	return true;

end_proc:
	START_LOG( cerr, L"부활석 정보 업데이트 실패!" )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< BUILD_LOG( kReq.m_iNumResurrectionStone )
		<< BUILD_LOG( kReq.m_iNumAutoPaymentResStone )
		<< END_LOG;
	return false;
}

bool KGSGameDBThread::Query_UpdateQuestInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	//QUEST UPDATE
	BOOST_TEST_FOREACH( const KQuestUpdate&, kQuestInfo, kReq.m_vecQuestData )
	{
		if( kQuestInfo.m_vecClearData.size() != 5 )
		{
			START_LOG( cerr, L"퀘스트 진행 정보가 5개가 아니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kQuestInfo.m_iQuestID )
				<< BUILD_LOG( kQuestInfo.m_vecClearData.size() )
				<< END_LOG;
			continue;
		}

		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQuests_UPD", L"%d, %d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kQuestInfo.m_iQuestID
			% (int)kQuestInfo.m_vecClearData[0]
			% (int)kQuestInfo.m_vecClearData[1]
			% (int)kQuestInfo.m_vecClearData[2]
			% (int)kQuestInfo.m_vecClearData[3]
			% (int)kQuestInfo.m_vecClearData[4]
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_quest", L"%d, %d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kQuestInfo.m_iQuestID
			% (int)kQuestInfo.m_vecClearData[0]
			% (int)kQuestInfo.m_vecClearData[1]
			% (int)kQuestInfo.m_vecClearData[2]
			% (int)kQuestInfo.m_vecClearData[3]
			% (int)kQuestInfo.m_vecClearData[4]
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"퀘스트 진행 정보 업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kQuestInfo.m_iQuestID )
				<< BUILD_LOGc( kQuestInfo.m_vecClearData[0] )
				<< BUILD_LOGc( kQuestInfo.m_vecClearData[1] )
				<< BUILD_LOGc( kQuestInfo.m_vecClearData[2] )
				<< BUILD_LOGc( kQuestInfo.m_vecClearData[3] )
				<< BUILD_LOGc( kQuestInfo.m_vecClearData[4] )
				<< END_LOG;
		}
	}

	//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST

	// 예약된 포기 퀘스트
	BOOST_TEST_FOREACH( const int, iGiveUpQuestID, kReq.m_vecGiveUpQuestList )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQuests_DEL", L"%d, %d", % kReq.m_iUnitUID % iGiveUpQuestID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_quest", L"%d, %d", % kReq.m_iUnitUID % iGiveUpQuestID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"퀘스트 포기 정보 업데이트 실패!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( iGiveUpQuestID )
				<< END_LOG;
		}
	}

#endif SERV_DAY_QUEST
	//}}
	return true;

end_proc:
	START_LOG( cerr, L"퀘스트 정보 업데이트 실패!" )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< END_LOG;
	return false;
}

bool KGSGameDBThread::Query_UpdateTitleInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{	
	//TITLE MISSION UPDATE
	BOOST_TEST_FOREACH( const KMissionUpdate&, kMission, kReq.m_vecMissionData )
	{
		if( kMission.m_vecClearData.size() != 5 )
		{
			START_LOG( cerr, L"칭호 클리어 정보가 5개가 아니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kMission.m_iTitleID )
				<< BUILD_LOG( kMission.m_vecClearData.size() )
				<< END_LOG;
			continue;
		}

		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Mission_MER", L"%d, %d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kMission.m_iTitleID
			% kMission.m_vecClearData[0]
			% kMission.m_vecClearData[1]
			% kMission.m_vecClearData[2]
			% kMission.m_vecClearData[3]
			% kMission.m_vecClearData[4]
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_title_mission", L"%d, %d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kMission.m_iTitleID
			% kMission.m_vecClearData[0]
			% kMission.m_vecClearData[1]
			% kMission.m_vecClearData[2]
			% kMission.m_vecClearData[3]
			% kMission.m_vecClearData[4]
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"미션 정보 업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kMission.m_iTitleID )
				<< BUILD_LOGc( kMission.m_vecClearData[0] )
				<< BUILD_LOGc( kMission.m_vecClearData[1] )
				<< BUILD_LOGc( kMission.m_vecClearData[2] )
				<< BUILD_LOGc( kMission.m_vecClearData[3] )
				<< BUILD_LOGc( kMission.m_vecClearData[4] )
				<< END_LOG;
			goto end_proc;
		}
	}

	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
//#ifdef SERV_TITLE_DATA_SIZE

	// 장착중인 타이틀 업데이트
	if( kReq.m_iInitTitleID != kReq.m_iEquippedTitleID )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_UPD_IsHang", L"%d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_iInitTitleID
			% kReq.m_iEquippedTitleID
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_title_hang", L"%d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_iInitTitleID
			% kReq.m_iEquippedTitleID
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"칭호 장착 정보 업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_iInitTitleID )
				<< BUILD_LOG( kReq.m_iEquippedTitleID )
				<< END_LOG;
			goto end_proc;
		}
	}

//#else
//
//	// 장착중인 타이틀 업데이트
//	if( kPacket_.m_sInitTitleID != kPacket_.m_sEquippedTitleID )
//	{
//		DO_QUERY( L"exec dbo.gup_update_title_hang", L"%d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kPacket_.m_sInitTitleID
//			% kPacket_.m_sEquippedTitleID
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			START_LOG( cerr, L"칭호 장착 정보 업데이트 실패!" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( kPacket_.m_iUnitUID )
//				<< BUILD_LOG( kPacket_.m_sInitTitleID )
//				<< BUILD_LOG( kPacket_.m_sEquippedTitleID )
//				<< END_LOG;
//
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//#endif SERV_TITLE_DATA_SIZE
	//}}
	return true;

end_proc:
	START_LOG( cerr, L"칭호 정보 업데이트 실패!" )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< END_LOG;
    return false;
}

bool KGSGameDBThread::Query_UpdateSkillSlotInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
#ifdef SERV_SKILL_PAGE_SYSTEM

	std::map< int, std::vector<int> >::const_iterator mIterSkillSlotVector
		= kReq.m_mapSkillSlotVector.begin();

	for ( ; mIterSkillSlotVector != kReq.m_mapSkillSlotVector.end(); ++mIterSkillSlotVector )
	{
		const int iSkillPagesNumber = mIterSkillSlotVector->first;
		const std::vector<int>& vecSkillIDInSlots = mIterSkillSlotVector->second;

		if ( vecSkillIDInSlots.size() < 4 )
		{
			START_LOG( cerr, L"스킬 슬롯 정보가 이상합니다!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( vecSkillIDInSlots.size() )
				<< END_LOG;
			return false;
		}

		//SKILL SLOT UPDATE
		{
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkillSlot_New_MER", L"%d, %d, %d, %d, %d, %d",
				% kReq.m_iUnitUID
				% vecSkillIDInSlots[0]
				% vecSkillIDInSlots[1]
				% vecSkillIDInSlots[2]
				% vecSkillIDInSlots[3]
				% iSkillPagesNumber
					);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkillSlot_New_INS", L"%d, %d, %d, %d, %d, %d",
				% kReq.m_iUnitUID
				% vecSkillIDInSlots[0]
			% vecSkillIDInSlots[1]
			% vecSkillIDInSlots[2]
			% vecSkillIDInSlots[3]
			% iSkillPagesNumber
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"스킬 슬롯 정보 업데이트 실패!" )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( vecSkillIDInSlots[0] )
					<< BUILD_LOG( vecSkillIDInSlots[1] )
					<< BUILD_LOG( vecSkillIDInSlots[2] )
					<< BUILD_LOG( vecSkillIDInSlots[3] )
					<< BUILD_LOG( iSkillPagesNumber )
					<< END_LOG;
				goto end_proc;
			}
		}

		//SKILL SLOT B UPDATE
		if( vecSkillIDInSlots.size() >= 8 )
		{
			int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_SKILL_PAGE_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkillSlot2_New_MER", L"%d, %d, %d, %d, %d, %d, N\'%s\'",
				% kReq.m_iUnitUID
				% vecSkillIDInSlots[4]
				% vecSkillIDInSlots[5]
				% vecSkillIDInSlots[6]
				% vecSkillIDInSlots[7]
				% iSkillPagesNumber
				% kReq.m_wstrSkillSlotBEndDate
					);
#else //SERV_ALL_RENEWAL_SP
			// [벡터조심]
			DO_QUERY( L"exec dbo.P_GSkillSlot2_New_INS", L"%d, %d, %d, %d, %d, %d, N\'%s\'",
				% kReq.m_iUnitUID
				% vecSkillIDInSlots[4]
			% vecSkillIDInSlots[5]
			% vecSkillIDInSlots[6]
			% vecSkillIDInSlots[7]
			% iSkillPagesNumber
			% kReq.m_wstrSkillSlotBEndDate
				);
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_SKILL_PAGE_SYSTEM
			// [벡터조심]
			DO_QUERY( L"exec dbo.P_GSkillSlot2_New_INS", L"%d, %d, %d, %d, %d, %d",
				% kReq.m_iUnitUID
				% vecSkillIDInSlots[4]
			% vecSkillIDInSlots[5]
			% vecSkillIDInSlots[6]
			% vecSkillIDInSlots[7]
			% iSkillPagesNumber
				);
#endif // SERV_SKILL_PAGE_SYSTEM


			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"스킬 슬롯B 정보 업데이트 실패!" )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( vecSkillIDInSlots[4] )
					<< BUILD_LOG( vecSkillIDInSlots[5] )
					<< BUILD_LOG( vecSkillIDInSlots[6] )
					<< BUILD_LOG( vecSkillIDInSlots[7] )
					<< BUILD_LOG( iSkillPagesNumber )
					<< END_LOG;
				goto end_proc;
			}
		}
	}

#else // SERV_SKILL_PAGE_SYSTEM
	if( kReq.m_vecSkillSlot.size() < 4 )
	{
		START_LOG( cerr, L"스킬 슬롯 정보가 이상합니다!" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_vecSkillSlot.size() )
			<< END_LOG;
		return false;
	}

	//SKILL SLOT UPDATE
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkillSlot_New_MER", L"%d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_vecSkillSlot[0]
			% kReq.m_vecSkillSlot[1]
			% kReq.m_vecSkillSlot[2]
			% kReq.m_vecSkillSlot[3]
			% 1
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_skill_slot_new", L"%d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_vecSkillSlot[0]
			% kReq.m_vecSkillSlot[1]
			% kReq.m_vecSkillSlot[2]
			% kReq.m_vecSkillSlot[3]
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"스킬 슬롯 정보 업데이트 실패!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_vecSkillSlot[0] )
				<< BUILD_LOG( kReq.m_vecSkillSlot[1] )
				<< BUILD_LOG( kReq.m_vecSkillSlot[2] )
				<< BUILD_LOG( kReq.m_vecSkillSlot[3] )
				<< END_LOG;
			goto end_proc;
		}
	}
	
	//SKILL SLOT B UPDATE
	if( kReq.m_vecSkillSlot.size() >= 8 )
	{
		int iOK = NetError::ERR_ODBC_01;

		// [벡터 조심]
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkillSlot2_New_MER", L"%d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_vecSkillSlot[4]
			% kReq.m_vecSkillSlot[5]
			% kReq.m_vecSkillSlot[6]
			% kReq.m_vecSkillSlot[7]
			% 1
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_skill_slot2_new", L"%d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_vecSkillSlot[4]
			% kReq.m_vecSkillSlot[5]
			% kReq.m_vecSkillSlot[6]
			% kReq.m_vecSkillSlot[7]
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"스킬 슬롯B 정보 업데이트 실패!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_vecSkillSlot[4] )
				<< BUILD_LOG( kReq.m_vecSkillSlot[5] )
				<< BUILD_LOG( kReq.m_vecSkillSlot[6] )
				<< BUILD_LOG( kReq.m_vecSkillSlot[7] )
				<< END_LOG;
			goto end_proc;
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	
	return true;

end_proc:
	START_LOG( cerr, L"스킬 슬롯 정보 업데이트 실패!" )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< END_LOG;
	return false;
}

bool KGSGameDBThread::Query_UpdateCommunityOption( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	int iOK = NetError::ERR_ODBC_01;

	//COMMUNITY OPTION UPDATE
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDenyOption_MER", L"%d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% (int)kReq.m_kDenyOptions.m_cDenyFriendShip
		% (int)kReq.m_kDenyOptions.m_cDenyInviteGuild
		% (int)kReq.m_kDenyOptions.m_cDenyParty
		% (int)kReq.m_kDenyOptions.m_cDenyPersonalTrade
		% (int)kReq.m_kDenyOptions.m_cDenyRequestCouple
		% (int)kReq.m_kDenyOptions.m_cDenyInvitePracticePVP
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDenyOption_UPD", L"%d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% (int)kReq.m_kDenyOptions.m_cDenyFriendShip
		% (int)kReq.m_kDenyOptions.m_cDenyInviteGuild
		% (int)kReq.m_kDenyOptions.m_cDenyParty
		% (int)kReq.m_kDenyOptions.m_cDenyPersonalTrade
		% (int)kReq.m_kDenyOptions.m_cDenyRequestCouple
        % (int)kReq.m_kDenyOptions.m_cDenyInvitePracticePVP
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
#else
// 해외팀 SP 이름 수정
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDenyOption_MER", L"%d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% (int)kReq.m_kDenyOptions.m_cDenyFriendShip
		% (int)kReq.m_kDenyOptions.m_cDenyInviteGuild
		% (int)kReq.m_kDenyOptions.m_cDenyParty
		% (int)kReq.m_kDenyOptions.m_cDenyPersonalTrade
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_community_opt", L"%d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% (int)kReq.m_kDenyOptions.m_cDenyFriendShip
		% (int)kReq.m_kDenyOptions.m_cDenyInviteGuild
		% (int)kReq.m_kDenyOptions.m_cDenyParty
		% (int)kReq.m_kDenyOptions.m_cDenyPersonalTrade
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"커뮤니티 옵션 정보 업데이트 실패!" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOGc( kReq.m_kDenyOptions.m_cDenyFriendShip )
			<< BUILD_LOGc( kReq.m_kDenyOptions.m_cDenyInviteGuild )
			<< BUILD_LOGc( kReq.m_kDenyOptions.m_cDenyParty )
			<< BUILD_LOGc( kReq.m_kDenyOptions.m_cDenyPersonalTrade )
			//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
			<< BUILD_LOGc( kReq.m_kDenyOptions.m_cDenyRequestCouple )
#endif SERV_RELATIONSHIP_SYSTEM
			//}
			<< END_LOG;
		return false;
	}

	return true;
}

bool KGSGameDBThread::Query_UpdateWishList( IN const UidType iUserUID, IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{	
	std::map< int, int >::const_iterator mitWishList;
	for( mitWishList = kReq.m_mapWishList.begin(); mitWishList != kReq.m_mapWishList.end(); ++mitWishList )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemWishList_MER", L"%d, %d, %d", % iUserUID % mitWishList->first % mitWishList->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_WishList", L"%d, %d, %d", % iUserUID % mitWishList->first % mitWishList->second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"장바구니 DB업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( iUserUID )
				<< BUILD_LOG( mitWishList->first )
				<< BUILD_LOG( mitWishList->second )
				<< END_LOG;
		}
	}

	return true;
}

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
bool KGSGameDBThread::Query_UpdateRankingInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	// 헤니르 랭킹 정보 기록
	BOOST_TEST_FOREACH( const KHenirRankingInfo&, kRankingInfo, kReq.m_vecHenirRanking )
	{
		// RegDate 변환
		CTime tRegDate = CTime( kRankingInfo.m_tRegDate );
		std::wstring wstrRegDate = ( CStringW )( tRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_Rank_SpaceTime_MyRecord_MER", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kReq.m_iUnitUID
			% kRankingInfo.m_iStageCount
			% kRankingInfo.m_ulPlayTime
			% (int)kRankingInfo.m_ucLevel
			% (int)kRankingInfo.m_cUnitClass
			% wstrRegDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_rank_myrecord", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kReq.m_iUnitUID
			% kRankingInfo.m_iStageCount
			% kRankingInfo.m_ulPlayTime
			% (int)kRankingInfo.m_ucLevel
			% (int)kRankingInfo.m_cUnitClass
			% wstrRegDate
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"헤니르 랭킹 정보 DB업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kRankingInfo.m_iStageCount )
				<< BUILD_LOG( kRankingInfo.m_ulPlayTime )
				<< BUILD_LOG( (int)kRankingInfo.m_ucLevel )
				<< BUILD_LOG( (int)kRankingInfo.m_cUnitClass )
				<< BUILD_LOG( wstrRegDate )
				<< END_LOG;
			return false;
		}

		// 랭킹정보는 하나밖에 안들어있다.
		break;
	}

	// 헤니르 영웅 랭킹 정보 기록
	BOOST_TEST_FOREACH( const KHenirRankingInfo&, kRankingInfo, kReq.m_vecHenirHeroRanking )
	{
		// RegDate 변환
		CTime tRegDate = CTime( kRankingInfo.m_tRegDate );
		std::wstring wstrRegDate = ( CStringW )( tRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Hero_MyRecord_MER", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kReq.m_iUnitUID
			% kRankingInfo.m_iStageCount
			% kRankingInfo.m_ulPlayTime
			% (int)kRankingInfo.m_ucLevel
			% (int)kRankingInfo.m_cUnitClass
			% wstrRegDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_Rank_SpaceTime_Hero_MyRecord_INT", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kReq.m_iUnitUID
			% kRankingInfo.m_iStageCount
			% kRankingInfo.m_ulPlayTime
			% (int)kRankingInfo.m_ucLevel
			% (int)kRankingInfo.m_cUnitClass
			% wstrRegDate
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"헤니르 영웅 랭킹 정보 DB업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kRankingInfo.m_iStageCount )
				<< BUILD_LOG( kRankingInfo.m_ulPlayTime )
				<< BUILD_LOG( (int)kRankingInfo.m_ucLevel )
				<< BUILD_LOG( (int)kRankingInfo.m_cUnitClass )
				<< BUILD_LOG( wstrRegDate )
				<< END_LOG;
			return false;
		}

		// 랭킹정보는 하나밖에 안들어있다.
		break;
	}

end_proc:
	return true;
}
#else // SERV_HENIR_RENEWAL_2013
bool KGSGameDBThread::Query_UpdateRankingInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	BOOST_TEST_FOREACH( const KHenirRankingInfo&, kRankingInfo, kReq.m_vecHenirRanking )
	{
		// RegDate 변환
		CTime tRegDate = CTime( kRankingInfo.m_tRegDate );
		std::wstring wstrRegDate = ( CStringW )( tRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_Rank_SpaceTime_MyRecord_MER", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kReq.m_iUnitUID
			% kRankingInfo.m_iStageCount
			% kRankingInfo.m_ulPlayTime
			% (int)kRankingInfo.m_ucLevel
			% (int)kRankingInfo.m_cUnitClass
			% wstrRegDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_rank_myrecord", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kReq.m_iUnitUID
			% kRankingInfo.m_iStageCount
			% kRankingInfo.m_ulPlayTime
			% (int)kRankingInfo.m_ucLevel
			% (int)kRankingInfo.m_cUnitClass
			% wstrRegDate
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"헤니르 랭킹 정보 DB업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kRankingInfo.m_iStageCount )
				<< BUILD_LOG( kRankingInfo.m_ulPlayTime )
				<< BUILD_LOG( (int)kRankingInfo.m_ucLevel )
				<< BUILD_LOG( (int)kRankingInfo.m_cUnitClass )
				<< BUILD_LOG( wstrRegDate )
				<< END_LOG;
			return false;
		}

		// 랭킹정보는 하나밖에 안들어있다.
		break;
	}

	return true;
}
#endif // SERV_HENIR_RENEWAL_2013

bool KGSGameDBThread::Query_UpdateGuildInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	//{{ 2009. 10. 7  최육사	길드
//#ifdef GUILD_TEST

	// 길드 정보 업데이트
	if( kReq.m_iGuildUID == 0 )
		return true;

	// 마지막 업데이트라면 로그아웃 처리
	if( kReq.m_bFinal )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_UPD_lastdate", L"%d, %d",
			% kReq.m_iUnitUID
			% kReq.m_iGuildUID
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_lastdate", L"%d, %d",
			% kReq.m_iUnitUID
			% kReq.m_iGuildUID
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드 로그아웃 처리 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_iGuildUID )
				<< END_LOG;
		}
	}

	{
		int iOK = NetError::ERR_ODBC_01;

		// 길드 명예 포인트 업데이트
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_UPD_EXP", L"%d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_iGuildUID
			% kReq.m_iHonorPoint
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_member_exp", L"%d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_iGuildUID
			% kReq.m_iHonorPoint
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드원 명예 포인트 업데이트 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kReq.m_iGuildUID )
				<< BUILD_LOG( kReq.m_iHonorPoint )
				<< END_LOG;
		}
	}

//#endif GUILD_TEST
	//}}

	return true;

end_proc:
	START_LOG( cerr, L"길드 정보 업데이트 실패." )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< BUILD_LOG( kReq.m_iGuildUID )
		<< END_LOG;
	return false;
}

#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
bool KGSGameDBThread::Query_UpdateConnectTimeEventInfo( IN const UidType iUserUID, IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{	
	BOOST_TEST_FOREACH( const KCumulativeTimeEventInfo&, kEventInfo, kReq.m_vecUpdateEventTime )
	{
		//{{ 2010. 06. 15  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		if( kEventInfo.m_bAccountEvent )
		{
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GEvent_Account_Acc_MER", L"%d, %d, %d", % iUserUID % kEventInfo.m_iEventUID % kEventInfo.m_iCumulativeTime );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_remaintime_account", L"%d, %d, %d", % iUserUID % kEventInfo.m_iEventUID % kEventInfo.m_iCumulativeTime );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( iUserUID )
					<< BUILD_LOG( kEventInfo.m_iEventUID )
					<< BUILD_LOG( kEventInfo.m_iCumulativeTime )
					<< END_LOG;
			}
		}
		else
#endif SERV_ACC_TIME_EVENT
		//}}
		{
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GRemainTime_MER", L"%d, %d, %d", % kReq.m_iUnitUID % kEventInfo.m_iEventUID % kEventInfo.m_iCumulativeTime );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_remaintime", L"%d, %d, %d", % kReq.m_iUnitUID % kEventInfo.m_iEventUID % kEventInfo.m_iCumulativeTime );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( kEventInfo.m_iEventUID )
					<< BUILD_LOG( kEventInfo.m_iCumulativeTime )
					<< END_LOG;
			}
		}
	}

	return true;

end_proc:
	START_LOG( cerr, L"접속 시간 이벤트 DB업데이트 실패" )
		<< BUILD_LOG( iUserUID )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< END_LOG;
	return false;
}
#endif //SERV_ADD_EVENT_DB

bool KGSGameDBThread::Query_UpdatePetInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck )
{	
	BOOST_TEST_FOREACH( const KPetInfo&, kPet, kReq.m_vecPet )
	{
		int iOK = NetError::ERR_ODBC_01;

		// 펫 정보 업데이트
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPet_Info_UPD_Info", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\', N\'%s\'", 
			% kPet.m_iPetUID
			% (int)kPet.m_cEvolutionStep
			% kPet.m_sSatiety
			% kPet.m_iIntimacy
			% kPet.m_sExtroversion
			% kPet.m_sEmotion
			% kPet.m_bAutoFeed
			% kPet.m_wstrLastFeedDate
			% kPet.m_wstrLastSummonDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_pet_info", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\', N\'%s\'", 
			% kPet.m_iPetUID
			% (int)kPet.m_cEvolutionStep
			% kPet.m_sSatiety
			% kPet.m_iIntimacy
			% kPet.m_sExtroversion
			% kPet.m_sEmotion
			% kPet.m_bAutoFeed
			% kPet.m_wstrLastFeedDate
			% kPet.m_wstrLastSummonDate
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"펫 정보 업데이트 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPet.m_iPetUID )
				<< BUILD_LOGc( kPet.m_cEvolutionStep )
				<< BUILD_LOG( kPet.m_sSatiety )
				<< BUILD_LOG( kPet.m_iIntimacy )
				<< BUILD_LOG( kPet.m_sExtroversion )
				<< BUILD_LOG( kPet.m_sEmotion )
				<< BUILD_LOG( kPet.m_bAutoFeed )
				<< BUILD_LOG( kPet.m_wstrLastFeedDate )
				<< BUILD_LOG( kPet.m_wstrLastSummonDate )
				<< END_LOG;

			// DB업데이트가 실패 한다면 롤백 데이터를 넣자!
			kAck.m_vecPet.push_back( kPet );
		}
	}

	return true;
}

bool KGSGameDBThread::Query_UpdateGameCountInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	std::map< int, int >::const_iterator mitCGC;
	for( mitCGC = kReq.m_mapCharGameCount.begin(); mitCGC != kReq.m_mapCharGameCount.end(); ++mitCGC )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCharacter_Count_MER", L"%d, %d, %d", 
			% kReq.m_iUnitUID
			% mitCGC->first
			% mitCGC->second
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_Character_Count", L"%d, %d, %d", 
			% kReq.m_iUnitUID
			% mitCGC->first
			% mitCGC->second
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"캐릭터 게임 카운트 정보 업데이트!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( mitCGC->first )
				<< BUILD_LOG( mitCGC->second )
				<< END_LOG;
		}
	}

	return true;
}

bool KGSGameDBThread::Query_UpdateHenirRewardCount( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	DO_QUERY( L"exec dbo.P_GHenirRewardCnt_UPD", L"%d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_kHenirRewardCnt.m_iNormal
		% kReq.m_kHenirRewardCnt.m_iPremium
		% kReq.m_kHenirRewardCnt.m_iEvent
		% kReq.m_kHenirRewardCnt.m_iChallengeNormal
		% kReq.m_kHenirRewardCnt.m_iChallengePremium
		% kReq.m_kHenirRewardCnt.m_iChallengeEvent
		);
#else // SERV_HENIR_RENEWAL_2013 // 해외팀 SP 이름 변경
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GHenirRewardCnt_UPD", L"%d, %d, %d, %d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_kHenirRewardCnt.m_iNormal
		% kReq.m_kHenirRewardCnt.m_iPremium
		% kReq.m_kHenirRewardCnt.m_iEvent
		% 0
		% 0
		% 0
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_henir_reward_cnt", L"%d, %d, %d, %d", 
		% kReq.m_iUnitUID
		% kReq.m_kHenirRewardCnt.m_iNormal
		% kReq.m_kHenirRewardCnt.m_iPremium
		% kReq.m_kHenirRewardCnt.m_iEvent
		);
#endif //SERV_ALL_RENEWAL_SP
#endif // SERV_HENIR_RENEWAL_2013

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 게임 카운트 정보 업데이트!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_kHenirRewardCnt.m_iNormal )
			<< BUILD_LOG( kReq.m_kHenirRewardCnt.m_iPremium )
			<< BUILD_LOG( kReq.m_kHenirRewardCnt.m_iEvent )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
			<< BUILD_LOG( kReq.m_kHenirRewardCnt.m_iChallengeNormal )
			<< BUILD_LOG( kReq.m_kHenirRewardCnt.m_iChallengePremium )
			<< BUILD_LOG( kReq.m_kHenirRewardCnt.m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
			<< END_LOG;
		return false;
	}

	return true;
}

//{{ 2012. 10. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
bool KGSGameDBThread::Query_UpdateLastPosition( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	int iOK = NetError::ERR_ODBC_01;
	
	// 마지막 위치 저장
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitLastPosition_UPD", L"%d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_kLastPos.m_iMapID
		% (int)kReq.m_kLastPos.m_ucLastTouchLineIndex
		% kReq.m_kLastPos.m_usLastPosValue
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitLastPosition_UPT", L"%d, %d, %d, %d",
		% kReq.m_iUnitUID
		% kReq.m_kLastPos.m_iMapID
		% (int)kReq.m_kLastPos.m_ucLastTouchLineIndex
		% kReq.m_kLastPos.m_usLastPosValue
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"유닛 마을 위치 정보 업데이트 실패." )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_kLastPos.m_iMapID )
			<< BUILD_LOGc( kReq.m_kLastPos.m_ucLastTouchLineIndex )
			<< BUILD_LOG( kReq.m_kLastPos.m_usLastPosValue )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KGSGameDBThread::Query_UpdateGamePlayStatus( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{	
	if( kReq.m_kGamePlayStatus.IsEmpty() )
		return true;

    KGamePlayStatus kPlayStatus;
	if( kReq.m_kGamePlayStatus.Get( kPlayStatus ) == false )
	{
		START_LOG( cerr, L"게임 플레이 상태 정보가 없다? GSUser에서 무조건 받아오지 않나?" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< END_LOG;
		return false;
	}

	{
		int iOK = NetError::ERR_ODBC_01;

		// 캐릭터 기본 정보 저장
		DO_QUERY( L"exec dbo.P_GUnitPlayInfo_INS", L"%d, %d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% kPlayStatus.m_iMaxHP
			% kPlayStatus.m_iCurHP
			% kPlayStatus.m_iMaxMP
			% kPlayStatus.m_iCurMP
			% kPlayStatus.m_iCurHyperGage
			% kPlayStatus.m_iCharAbilCount
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kPlayStatus.m_iMaxHP )
				<< BUILD_LOG( kPlayStatus.m_iCurHP )
				<< BUILD_LOG( kPlayStatus.m_iMaxMP )
				<< BUILD_LOG( kPlayStatus.m_iCurMP )
				<< BUILD_LOG( kPlayStatus.m_iCurHyperGage )
				<< BUILD_LOG( kPlayStatus.m_iCharAbilCount )
				<< END_LOG;
		}
	}

	// 스킬 쿨타임 데이터 저장!
#ifdef SERV_CHANGE_SKILL_COOL_TIME_DB_SP 	// 적용날짜: 2013-05-02
	{
		int arrySlotID[8] = {-1,};
		int arryCoolTime[8] = {0,};
        for( int index = 0 ; index < 8 ; ++index )
		{
			arrySlotID[index] = -1;
			arryCoolTime[index] = 0;
		}

		std::map< int, int >::const_iterator mit = kPlayStatus.m_mapSkillCoolTime.begin();
		for( int index = 0 ; mit != kPlayStatus.m_mapSkillCoolTime.end(); ++mit, ++index )
		{
			if( index >= 8 )
			{
				// 배열 사이즈 오버 이므로 확인 필요
				START_LOG( cerr, L"스킬 배열 index 가 오버 되었습니다." )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( index )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;
				continue;;
			}

			arrySlotID[index] = mit->first;
			arryCoolTime[index] = mit->second;
		}

		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCoolTime_MER_New", 
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% KGamePlayStatus::CTT_SKILL_COOL_TIME
			% arrySlotID[0]
			% arryCoolTime[0]
			% arrySlotID[1]
			% arryCoolTime[1]
			% arrySlotID[2]
			% arryCoolTime[2]
			% arrySlotID[3]
			% arryCoolTime[3]
			% arrySlotID[4]
			% arryCoolTime[4]
			% arrySlotID[5]
			% arryCoolTime[5]
			% arrySlotID[6]
			% arryCoolTime[6]
			% arrySlotID[7]
			% arryCoolTime[7]
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCoolTime_INS_New", 
								L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kReq.m_iUnitUID
			% KGamePlayStatus::CTT_SKILL_COOL_TIME
			% arrySlotID[0]
			% arryCoolTime[0]
			% arrySlotID[1]
			% arryCoolTime[1]
			% arrySlotID[2]
			% arryCoolTime[2]
			% arrySlotID[3]
			% arryCoolTime[3]
			% arrySlotID[4]
			% arryCoolTime[4]
			% arrySlotID[5]
			% arryCoolTime[5]
			% arrySlotID[6]
			% arryCoolTime[6]
			% arrySlotID[7]
			% arryCoolTime[7]
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"캐릭터 스킬 쿨타임 정보 저장 실패!" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( arrySlotID[0] )
				<< BUILD_LOG( arryCoolTime[0] )
				<< BUILD_LOG( arrySlotID[1] )
				<< BUILD_LOG( arryCoolTime[1] )
				<< BUILD_LOG( arrySlotID[2] )
				<< BUILD_LOG( arryCoolTime[2] )
				<< BUILD_LOG( arrySlotID[3] )
				<< BUILD_LOG( arryCoolTime[3] )
				<< BUILD_LOG( arrySlotID[4] )
				<< BUILD_LOG( arryCoolTime[4] )
				<< BUILD_LOG( arrySlotID[5] )
				<< BUILD_LOG( arryCoolTime[5] )
				<< BUILD_LOG( arrySlotID[6] )
				<< BUILD_LOG( arryCoolTime[6] )
				<< BUILD_LOG( arrySlotID[7] )
				<< BUILD_LOG( arryCoolTime[7] )
				<< END_LOG;
		}
	}

#else		// SERV_CHANGE_SKILL_COOL_TIME_DB_SP

	// 스킬 쿨타임 데이터 저장!
	{
		std::map< int, int >::const_iterator mit;
		for( mit = kPlayStatus.m_mapSkillCoolTime.begin(); mit != kPlayStatus.m_mapSkillCoolTime.end(); ++mit )
		{
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_MER", L"%d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_SKILL_COOL_TIME
				% mit->first
				% mit->second
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_INS", L"%d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_SKILL_COOL_TIME
				% mit->first
				% mit->second
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;
			}
		}
	}
#endif	// SERV_CHANGE_SKILL_COOL_TIME_DB_SP

#ifdef	SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP 	// 적용날짜: 2013-06-20
	// 퀵슬롯 쿨타임 데이터 저장!
	{
		int arrySlotID[CXSLItem::CIG_MAX] = {-1,};
		int arryCoolTime[CXSLItem::CIG_MAX] = {0,};
		for( int index = 0 ; index < CXSLItem::CIG_MAX ; ++index )
		{
			std::map< int, int >::iterator mit;
			mit = kPlayStatus.m_mapQuickSlotCoolTime.find( index );
			if( mit != kPlayStatus.m_mapQuickSlotCoolTime.end() )
			{
				arrySlotID[index] = mit->first;
				arryCoolTime[index] = mit->second;
			}
			else
			{
				arrySlotID[index] = -1;
				arryCoolTime[index] = 0;
			}
		}

		// 10 개씩 끊어서 호출!
		for( int inum = 0 ; inum < CXSLItem::CIG_MAX ; inum += 10 )
		{
			int arryTempSlotID[10] = {-1,};
			int arryTempCoolTime[10] = {0,};

			arryTempSlotID[0]		= ( inum < CXSLItem::CIG_MAX )		? arrySlotID[inum]		: -1;
			arryTempCoolTime[0]		= ( inum < CXSLItem::CIG_MAX )		? arryCoolTime[inum]	: 0; 
			arryTempSlotID[1]		= ( inum+1 < CXSLItem::CIG_MAX )	? arrySlotID[inum+1]	: -1;
			arryTempCoolTime[1]		= ( inum+1 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+1]	: 0; 
			arryTempSlotID[2]		= ( inum+2 < CXSLItem::CIG_MAX )	? arrySlotID[inum+2]	: -1;
			arryTempCoolTime[2]		= ( inum+2 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+2]	: 0; 
			arryTempSlotID[3]		= ( inum+3 < CXSLItem::CIG_MAX )	? arrySlotID[inum+3]	: -1;
			arryTempCoolTime[3]		= ( inum+3 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+3]	: 0; 
			arryTempSlotID[4]		= ( inum+4 < CXSLItem::CIG_MAX )	? arrySlotID[inum+4]	: -1;
			arryTempCoolTime[4]		= ( inum+4 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+4]	: 0; 
			arryTempSlotID[5]		= ( inum+5 < CXSLItem::CIG_MAX )	? arrySlotID[inum+5]	: -1;
			arryTempCoolTime[5]		= ( inum+5 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+5]	: 0; 
			arryTempSlotID[6]		= ( inum+6 < CXSLItem::CIG_MAX )	? arrySlotID[inum+6]	: -1;
			arryTempCoolTime[6]		= ( inum+6 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+6]	: 0; 
			arryTempSlotID[7]		= ( inum+7 < CXSLItem::CIG_MAX )	? arrySlotID[inum+7]	: -1;
			arryTempCoolTime[7]		= ( inum+7 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+7]	: 0; 
			arryTempSlotID[8]		= ( inum+8 < CXSLItem::CIG_MAX )	? arrySlotID[inum+8]	: -1;
			arryTempCoolTime[8]		= ( inum+8 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+8]	: 0; 
			arryTempSlotID[9]		= ( inum+9 < CXSLItem::CIG_MAX )	? arrySlotID[inum+9]	: -1;
			arryTempCoolTime[9]		= ( inum+9 < CXSLItem::CIG_MAX )	? arryCoolTime[inum+9]	: 0; 

			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_MER_Quick", 
				L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_QUICK_SLOT_COOL_TIME
				% arryTempSlotID[0]
				% arryTempCoolTime[0]
				% arryTempSlotID[1]
				% arryTempCoolTime[1]
				% arryTempSlotID[2]
				% arryTempCoolTime[2]
				% arryTempSlotID[3]
				% arryTempCoolTime[3]
				% arryTempSlotID[4]
				% arryTempCoolTime[4]
				% arryTempSlotID[5]
				% arryTempCoolTime[5]
				% arryTempSlotID[6]
				% arryTempCoolTime[6]
				% arryTempSlotID[7]
				% arryTempCoolTime[7]
				% arryTempSlotID[8]
				% arryTempCoolTime[8]
				% arryTempSlotID[9]
				% arryTempCoolTime[9]
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_INS_Quick", 
				L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_QUICK_SLOT_COOL_TIME
				% arryTempSlotID[0]
				% arryTempCoolTime[0]
				% arryTempSlotID[1]
				% arryTempCoolTime[1]
				% arryTempSlotID[2]
				% arryTempCoolTime[2]
				% arryTempSlotID[3]
				% arryTempCoolTime[3]
				% arryTempSlotID[4]
				% arryTempCoolTime[4]
				% arryTempSlotID[5]
				% arryTempCoolTime[5]
				% arryTempSlotID[6]
				% arryTempCoolTime[6]
				% arryTempSlotID[7]
				% arryTempCoolTime[7]
				% arryTempSlotID[8]
				% arryTempCoolTime[8]
				% arryTempSlotID[9]
				% arryTempCoolTime[9]
				);
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"캐릭터 퀵 슬롯 쿨타임 정보 저장 실패!" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( kReq.m_iUnitUID )
						<< BUILD_LOG( arryTempSlotID[0] )
						<< BUILD_LOG( arryTempCoolTime[0] )
						<< BUILD_LOG( arryTempSlotID[1] )
						<< BUILD_LOG( arryTempCoolTime[1] )
						<< BUILD_LOG( arryTempSlotID[2] )
						<< BUILD_LOG( arryTempCoolTime[2] )
						<< BUILD_LOG( arryTempSlotID[3] )
						<< BUILD_LOG( arryTempCoolTime[3] )
						<< BUILD_LOG( arryTempSlotID[4] )
						<< BUILD_LOG( arryTempCoolTime[4] )
						<< BUILD_LOG( arryTempSlotID[5] )
						<< BUILD_LOG( arryTempCoolTime[5] )
						<< BUILD_LOG( arryTempSlotID[6] )
						<< BUILD_LOG( arryTempCoolTime[6] )
						<< BUILD_LOG( arryTempSlotID[7] )
						<< BUILD_LOG( arryTempCoolTime[7] )
						<< BUILD_LOG( arryTempSlotID[8] )
						<< BUILD_LOG( arryTempCoolTime[8] )
						<< BUILD_LOG( arryTempSlotID[9] )
						<< BUILD_LOG( arryTempCoolTime[9] )
						<< END_LOG;
				}
		}

	}
#else	// SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP

// 퀵슬롯 쿨타임 데이터 저장!
	{
		std::map< int, int >::const_iterator mit;
		for( mit = kPlayStatus.m_mapQuickSlotCoolTime.begin(); mit != kPlayStatus.m_mapQuickSlotCoolTime.end(); ++mit )
		{
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_MER", L"%d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_QUICK_SLOT_COOL_TIME
				% mit->first
				% mit->second
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_INS", L"%d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_QUICK_SLOT_COOL_TIME
				% mit->first
				% mit->second
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second )
					<< END_LOG;
			}
		}
	}

#endif	// SERV_CHANGE_QUICK_SLOT_COOL_TIME_DB_SP


#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	// 퀵슬롯 쿨타임 데이터 저장!
	{
		std::map< int, int >::const_iterator it;
		for( it = kPlayStatus.m_mapRidingPetCoolTime.begin(); it != kPlayStatus.m_mapRidingPetCoolTime.end(); ++it )
		{
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_MER", L"%d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_RIDING_PET_COOL_TIME
				% it->first
				% it->second
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCoolTime_INS", L"%d, %d, %d, %d",
				% kReq.m_iUnitUID
				% KGamePlayStatus::CTT_RIDING_PET_COOL_TIME
				% it->first
				% it->second
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kReq.m_iUnitUID )
					<< BUILD_LOG( it->first )
					<< BUILD_LOG( it->second )
					<< END_LOG;
			}
		}
	}
#endif	// SERV_RIDING_PET_SYSTM

	return true;

end_proc:
	START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
		<< BUILD_LOG( kReq.m_iUnitUID )
		<< END_LOG;
	return false;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
bool KGSGameDBThread::Query_UpdateBuffEffect( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{	
	BOOST_TEST_FOREACH( const KRecordBuffInfo&, kRecordBuffInfo, kReq.m_vecRecordBuffInfo )
	{
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_GBuffeffect_UPD", L"%d, %d, N\'%s\'",
			% kReq.m_iUnitUID
			% kRecordBuffInfo.m_iBuffID
			% kRecordBuffInfo.m_wstrStartTime
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"버프 정보 업데이트 실패!" )
				<< BUILD_LOG( kReq.m_iUnitUID )
				<< BUILD_LOG( kRecordBuffInfo.m_iBuffID )
				<< BUILD_LOG( kRecordBuffInfo.m_wstrStartTime )
				<< END_LOG;
		}
	}

	return true;
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
bool KGSGameDBThread::Query_UpdateLocalRankingInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck )
{	
	int iOK = NetError::ERR_ODBC_01;

	if( ( 0 < kReq.m_iChangedLocalRankingSpirit ) || ( 0 < kReq.m_iChangedLocalRankingAP ) )
	{
		DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_INS", L"%d, %d, %d",
			% kReq.m_iUnitUID
			% kReq.m_iChangedLocalRankingSpirit
			% kReq.m_iChangedLocalRankingAP
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

end_proc:
	if( iOK == NetError::NET_OK )
	{
		kAck.m_iChangedLocalRankingSpirit	= 0;
		kAck.m_iChangedLocalRankingAP		= 0;
	}

	return true;
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
bool KGSGameDBThread::Query_UpdateRidingPetInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck )
{
	BOOST_TEST_FOREACH( const KRidingPetInfo&, kInfo, kReq.m_vecRidingPetList )
	{
		int iOK = NetError::ERR_ODBC_01;

		USHORT usStamina = static_cast<USHORT>( kInfo.m_fStamina );
		CTime tLastUnSummonDate( kInfo.m_iLastUnSummonDate );
		std::wstring wstrLastUnSummonDate = tLastUnSummonDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

		DO_QUERY( L"exec dbo.P_GRiding_UPD", L"%d, %d, %d, N\'%s\'",
			% kInfo.m_iRidingPetUID
			% kReq.m_iUnitUID
			% usStamina
			% wstrLastUnSummonDate
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			kAck.m_vecRidingPetUID.push_back( kInfo.m_iRidingPetUID );
		}
	}

end_proc:
	return true;
}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
bool KGSGameDBThread::Query_UpdateAccumulationSpirit( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_Spirit_MER", L"%d, %d", 
		% kReq.m_iUnitUID
		% kReq.m_iAccumultionSpirit
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_Spirit_SET", L"%d, %d", 
		% kReq.m_iUnitUID
		% kReq.m_iAccumultionSpirit
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	START_LOG( cerr, L"김석근_짐승남근성도확인_SET" )
		<< BUILD_LOG( kReq.m_iAccumultionSpirit )
		<< END_LOG;

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"P_GEvent_Spirit_SET 이벤트 누적 근성도 정보 업데이트 실패!" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_iAccumultionSpirit )
			<< END_LOG;
	}

	return true;
}
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
bool KGSGameDBThread::Query_UpdateGateOfDarknessSupportEventTime( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_GEvent_GateOfDarkness_SET", L"%d, %d", 
		% kReq.m_iUnitUID
		% kReq.m_iGateOfDarknessSupportEventTime
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"어둠의 문 개편 이벤트 남은 시간 갱신 실패!" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_iGateOfDarknessSupportEventTime )
			<< END_LOG;
	}

	return true;
}
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
//////////////////////////////////////////////////////////////////////////

#ifdef SERV_ELESIS_UPDATE_EVENT
bool KGSGameDBThread::Query_UpdateEventNoteViewCount( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_GEvent_NoteViewCount_SET", L"%d, %d", 
		% kReq.m_iUnitUID
		% kReq.m_iNoteViewCount
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"엘리시스 기술 전수 이벤트 값 갱신 실패!" )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( kReq.m_iNoteViewCount )
			<< END_LOG;
	}

	return true;
}
#endif SERV_ELESIS_UPDATE_EVENT

#endif SERV_DB_UPDATE_UNIT_INFO_REFACTORING
//////////////////////////////////////////////////////////////////////////
//}}


//{{ 2013. 07. 08	최육사	공유 은행
#ifdef SERV_SHARING_BANK_TEST
bool KGSGameDBThread::Query_UpdateShareItem( IN const KDBE_UPDATE_SHARE_ITEM_REQ& kReq, OUT KDBE_UPDATE_SHARE_ITEM_ACK& kAck )
{
	kAck.m_iOK					= NetError::NET_OK;
	kAck.m_bReload				= kReq.m_bReload;
	kAck.m_wstrReloadNickname	= kReq.m_wstrReloadNickname;
	kAck.m_vecInventorySlotItem = kReq.m_vecInventorySlotItem;

	std::map< UidType, KUpdateShareItemInfo >::const_iterator mit;	//	내구도, 수량 업데이트
	KItemPositionUpdate::const_iterator mit2;						//	슬롯 위치변경
	std::map< UidType, KTradeShareItemInfo >::const_iterator mit3;	//	In&Out	

	//	1. 내구도 업데이트
	for( mit = kReq.m_mapUpdateEndurance.begin(); mit != kReq.m_mapUpdateEndurance.end(); ++mit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD", L"%d, %d, %d, %d", 
			% mit->second.m_iUnitUID
			% mit->first 
			% mit->second.m_iUsageType
			% mit->second.m_iValue
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item", L"%d, %d, %d, %d", 
			% mit->second.m_iUnitUID
			% mit->first 
			% mit->second.m_iUsageType
			% mit->second.m_iValue
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );
			m_kODBC.EndFetch();

			if( kAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"은행 공유 : 아이템 정보 업데이트 실패." )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( mit->second.m_iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second.m_iValue )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 아이템 내구도 업데이트중 BeginFetch 실패." );
		}
	}

	//	2. 수량 업데이트
	for( mit = kReq.m_mapUpdateQuantity.begin(); mit != kReq.m_mapUpdateQuantity.end(); ++mit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD", L"%d, %d, %d, %d", 
			% mit->second.m_iUnitUID
			% mit->first 
			% mit->second.m_iUsageType
			% mit->second.m_iValue
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item", L"%d, %d, %d, %d", 
			% mit->second.m_iUnitUID
			% mit->first 
			% mit->second.m_iUsageType
			% mit->second.m_iValue
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );
			m_kODBC.EndFetch();

			if( kAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"은행 공유 : 아이템 정보 업데이트 실패." )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( mit->second.m_iUnitUID )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( mit->second.m_iValue )
					<< END_LOG;

				//{{ 2008. 10. 23  최육사	증분값 버그한번 잡아보자~!
				if( kAck.m_iOK == -5 )
				{
					START_LOG( cout, L"은행 공유 : 증분값 업데이트 버그 발생!" )
						<< BUILD_LOG( kAck.m_iOK )
						<< BUILD_LOG( mit->second.m_iUnitUID )
						<< BUILD_LOG( mit->first )
						<< BUILD_LOG( mit->second.m_iValue )
						<< END_LOG;

				}
				//}}
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 아이템 수량 업데이트중 BeginFetch 실패." );
		}
	}

	//	3. 슬롯 위치 변경
	for( mit2 = kReq.m_mapUpdatePosition.begin(); mit2 != kReq.m_mapUpdatePosition.end(); ++mit2 )
	{
		const UidType iItemUID	= mit2->first;
		const int iCategory		= mit2->second.m_iSlotCategory;
		const int iSlotID		= mit2->second.m_iSlotID;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_Postion", L"%d, %d, %d",
			% iItemUID
			% iCategory
			% iSlotID
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item_position", L"%d, %d, %d",
			% iItemUID
			% iCategory
			% iSlotID
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );
			m_kODBC.EndFetch();

			if( kAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"은행 공유 : 아이템 위치 업데이트 실패." )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOG( iCategory )
					<< BUILD_LOG( iSlotID )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 아이템 위치 업데이트 BeginFetch 실패." );
		}
	}

	//	4. 인벤토리 -> 공유은행
	for( mit3 = kReq.m_mapInItem.begin(); mit3 != kReq.m_mapInItem.end(); ++mit3 )
	{
		KTradeShareItemInfo kShareInfo = mit3->second;

#ifdef SERV_SHARE_BANK_ITEM_EVALUATE_FIX
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_TradeBankNew", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_Trade_BankItem_New", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_SHARE_BANK_ITEM_EVALUATE_FIX
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_TradeBank", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_Trade_BankItem", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#endif //SERV_ALL_RENEWAL_SP
#endif //SERV_SHARE_BANK_ITEM_EVALUATE_FIX

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK
				>> kShareInfo.m_iNewItemUID
				>> kShareInfo.m_wstrRegDate );

			m_kODBC.EndFetch();

			if( kAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"은행 공유 : 공유은행으로 아이템 넣기 실패" )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( kShareInfo.m_iFromUnitUID )
					<< BUILD_LOG( kShareInfo.m_iToUnitUID )
					<< BUILD_LOG( kShareInfo.m_iItemUID )
					<< BUILD_LOG( kShareInfo.m_iItemID )
					<< BUILD_LOGc( kShareInfo.m_cInventoryCategory)
					<< BUILD_LOG( kShareInfo.m_sSlotID )
					<< BUILD_LOG( kShareInfo.m_ucSealCnt )
					<< BUILD_LOG( kShareInfo.m_ucDeleteReason )
					<< END_LOG;
			}
			else
			{
				kAck.m_vecTradeShareItemResult.push_back( kShareInfo );
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 공유은행으로 아이템 넣기 BeginFetch 실패." );
		}
	}

	//	5. 공유은행 -> 인벤토리
	for( mit3 = kReq.m_mapOutItem.begin(); mit3 != kReq.m_mapOutItem.end(); ++mit3 )
	{
		KTradeShareItemInfo kShareInfo = mit3->second;

#ifdef SERV_SHARE_BANK_ITEM_EVALUATE_FIX
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_TradeBankNew", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_Trade_BankItem_New", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_SHARE_BANK_ITEM_EVALUATE_FIX
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_TradeBank", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_Trade_BankItem", L"%d, %d, %d, %d, %d, %d, %d",
			% kShareInfo.m_iFromUnitUID
			% kShareInfo.m_iToUnitUID
			% kShareInfo.m_iItemUID
			% static_cast<int>(kShareInfo.m_cInventoryCategory)
			% static_cast<int>(kShareInfo.m_sSlotID)
			% static_cast<int>(kShareInfo.m_ucSealCnt)
			% static_cast<int>(kShareInfo.m_ucDeleteReason)
			);
#endif //SERV_ALL_RENEWAL_SP
#endif //SERV_SHARE_BANK_ITEM_EVALUATE_FIX

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK
				>> kShareInfo.m_iNewItemUID
				>> kShareInfo.m_wstrRegDate );

			m_kODBC.EndFetch();

			if( kAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"은행 공유 : 공유은행에서 아이템 꺼내기 실패" )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( kShareInfo.m_iFromUnitUID )
					<< BUILD_LOG( kShareInfo.m_iToUnitUID )
					<< BUILD_LOG( kShareInfo.m_iItemUID )
					<< BUILD_LOG( kShareInfo.m_iItemID )
					<< BUILD_LOGc( kShareInfo.m_cInventoryCategory)
					<< BUILD_LOG( kShareInfo.m_sSlotID )
					<< BUILD_LOG( kShareInfo.m_ucSealCnt )
					<< BUILD_LOG( kShareInfo.m_ucDeleteReason )
					<< END_LOG;
			}
			else
			{
				kAck.m_vecTradeShareItemResult.push_back( kShareInfo );
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 공유은행에서 아이템 꺼내기 BeginFetch 실패." );
		}
	}

	//	6. 삭제될 아이템 업데이트
	BOOST_TEST_FOREACH( const UidType, iItemUID, kReq.m_vecDeletedItem )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_DEL", L"%d, %d", % iItemUID % static_cast<int>(KDeletedItemInfo::DR_ZERO_QUANTITY_SHARE_BANK) );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_item", L"%d, %d", % iItemUID % static_cast<int>(KDeletedItemInfo::DR_ZERO_QUANTITY_SHARE_BANK) );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );
			m_kODBC.EndFetch();

			if( kAck.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"은행 공유 : 아이템 삭제 실패." )
					<< BUILD_LOG( kAck.m_iOK )
					<< BUILD_LOG( iItemUID )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 공유은행에서 아이템 꺼내기 BeginFetch 실패." );
		}
	}

	return true;

end_proc:
	return false;
}
#endif SERV_SHARING_BANK_TEST

void KGSGameDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSGameDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

//{{ 2012. 10. 23	최육사		DB업데이트 코드 리팩토링
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_DB_UPDATE_UNIT_INFO_REFACTORING
//////////////////////////////////////////////////////////////////////////

IMPL_ON_FUNC( DBE_UPDATE_UNIT_INFO_REQ )
{
	KDBE_UPDATE_UNIT_INFO_ACK kPacket;
	kPacket.m_iEventID				= kPacket_.m_iEventID;
	kPacket.m_iEXP				    = kPacket_.m_iEXP;
	kPacket.m_iED				    = kPacket_.m_iED;
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kPacket.m_iOfficialMatchCnt		= kPacket_.m_iOfficialMatchCnt;
	kPacket.m_iRating				= kPacket_.m_iRating;
	kPacket.m_iMaxRating			= kPacket_.m_iMaxRating;
	kPacket.m_iRPoint				= kPacket_.m_iRPoint;
	kPacket.m_iAPoint				= kPacket_.m_iAPoint;
	kPacket.m_iWin					= kPacket_.m_iWin;
	kPacket.m_iLose					= kPacket_.m_iLose;

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	kPacket.m_fKFactor				= kPacket_.m_fKFactor;
#endif SERV_2012_PVP_SEASON2
	//}}

#else
	kPacket.m_iVSPoint			    = kPacket_.m_iVSPoint;
	kPacket.m_iVSPointMax           = kPacket_.m_iVSPointMax;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	std::map< int, KDungeonClearInfo >::iterator mitDungeonClear;
	for( mitDungeonClear = kPacket_.m_mapDungeonClear.begin(); mitDungeonClear != kPacket_.m_mapDungeonClear.end(); mitDungeonClear++ )
	{
		kPacket.m_vecDungeonClear.push_back( mitDungeonClear->first );
	}
	std::map< int, KTCClearInfo >::iterator mitTCClear;
	for( mitTCClear = kPacket_.m_mapTCClear.begin(); mitTCClear != kPacket_.m_mapTCClear.end(); mitTCClear++ )
	{
		kPacket.m_vecTCClear.push_back( mitTCClear->first );
	}
	kPacket.m_kItemQuantityUpdate = kPacket_.m_kItemQuantityUpdate;
	kPacket.m_kItemEnduranceUpdate = kPacket_.m_kItemEnduranceUpdate;
	kPacket.m_kItemPositionUpdate = kPacket_.m_kItemPositionUpdate;
	kPacket.m_iNumResurrectionStone = kPacket_.m_iNumResurrectionStone;
	//{{ 2009. 10. 14  최육사	자동결제 부활석
#ifdef AP_RESTONE
	kPacket.m_iNumAutoPaymentResStone = kPacket_.m_iNumAutoPaymentResStone;
#endif AP_RESTONE
	//}}
	//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	kPacket.m_iChangedLocalRankingSpirit	= kPacket_.m_iChangedLocalRankingSpirit;
	kPacket.m_iChangedLocalRankingAP		= kPacket_.m_iChangedLocalRankingAP;
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

	// 캐릭터 기본 정보 업데이트
	LIF( Query_UpdateUnitInfo( kPacket_, kPacket ) );

	// 캐릭터 대전 정보 업데이트
	LIF( Query_UpdatePvpInfo( kPacket_, kPacket ) );

	//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
	LIF( Query_UpdateLastGamePoint( kPacket_ ) );
#endif SERV_LOGOUT_ED_CHECK
	//}}

	//{{ 2012. 10. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	// 마지막 위치 정보 저장
	LIF( Query_UpdateLastPosition( kPacket_ ) );

	// 캐릭터 플레이 정보 저장
	LIF( Query_UpdateGamePlayStatus( kPacket_ ) );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	LIF( Query_UpdateLastLogOffDate( kPacket_ ) );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

	bool bUpdateFailed = false;
	Query_UpdateDungeonClear( kPacket_.m_iUnitUID, kPacket_.m_mapDungeonClear, kPacket.m_vecDungeonClear );
	Query_UpdateTCClear( kPacket_.m_iUnitUID, kPacket_.m_mapTCClear, kPacket.m_vecTCClear );
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	Query_UpdateDungeonPlay( kPacket_.m_iUnitUID, kPacket_.m_mapDungeonPlay );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
	Query_UpdateItemPosition( kPacket_.m_iUnitUID, kPacket_.m_kItemPositionUpdate, kPacket.m_kItemPositionUpdate );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_bFinal );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	// 부활석 정보 업데이트
	LIF( Query_UpdateResurrectionStone( kPacket_, kPacket ) );

	// 퀘스트 정보 업데이트
	LIF( Query_UpdateQuestInfo( kPacket_ ) );

	// 칭호 정보 업데이트
	LIF( Query_UpdateTitleInfo( kPacket_ ) );

	// 스킬 슬롯 업데이트
	LIF( Query_UpdateSkillSlotInfo( kPacket_ ) );

	// 커뮤니티 옵션 정보 업데이트
	LIF( Query_UpdateCommunityOption( kPacket_ ) );

	// 찜 리스트 업데이트
	LIF( Query_UpdateWishList( FIRST_SENDER_UID, kPacket_ ) );

	// 헤니르 시공 랭킹 정보 업데이트
	LIF( Query_UpdateRankingInfo( kPacket_ ) );
	
	// 길드 정보 업데이트
	LIF( Query_UpdateGuildInfo( kPacket_ ) );

	//{{ 2009. 12. 8  최육사	이벤트개편
#ifdef CUMULATIVE_TIME_EVENT
	// 이벤트 정보 업데이트
#ifdef SERV_ADD_EVENT_DB
	SendToEventDB( DBE_UPDATE_UNIT_CONNECT_TIME_EVENT_INFO_NOT, kPacket_ );
#else //SERV_ADD_EVENT_DB
	LIF( Query_UpdateConnectTimeEventInfo( FIRST_SENDER_UID, kPacket_ ) );
#endif //SERV_ADD_EVENT_DB
#endif CUMULATIVE_TIME_EVENT
	//}}
	
	//{{ 2010. 8. 4	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	// 펫 정보 업데이트
	LIF( Query_UpdatePetInfo( kPacket_, kPacket ) );
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	// 게임 카운트 업데이트
	LIF( Query_UpdateGameCountInfo( kPacket_ ) );
#endif SERV_CHAR_LOG
	//}}

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	LIF( Query_UpdateHenirRewardCount( kPacket_ ) );
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	LIF( Query_UpdateBuffEffect( kPacket_ ) );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	LIF( Query_UpdateLocalRankingInfo( kPacket_, kPacket ) );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	LIF( Query_UpdateRidingPetInfo( kPacket_, kPacket ) );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	LIF( Query_UpdateAccumulationSpirit( kPacket_ ) );
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	LIF( Query_UpdateGateOfDarknessSupportEventTime( kPacket_ ) );
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
	LIF( Query_UpdateEventNoteViewCount( kPacket_ ) );
#endif SERV_ELESIS_UPDATE_EVENT

	if( kPacket_.m_bFinal  &&  kPacket_.m_iEventID == 0 )
		return;

	SendToUser( LAST_SENDER_UID, DBE_UPDATE_UNIT_INFO_ACK, kPacket );
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

//IMPL_ON_FUNC( DBE_UPDATE_UNIT_INFO_REQ )
//{
//	KDBE_UPDATE_UNIT_INFO_ACK kPacket;
//	//{{ 2009. 3. 17  최육사	채널이동
//	kPacket.m_iEventID				= kPacket_.m_iEventID;
//	//}}
//	kPacket.m_iEXP				    = kPacket_.m_iEXP;
//	kPacket.m_iED				    = kPacket_.m_iED;
//	//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
//	kPacket.m_iOfficialMatchCnt		= kPacket_.m_iOfficialMatchCnt;
//	kPacket.m_iRating				= kPacket_.m_iRating;
//	kPacket.m_iMaxRating			= kPacket_.m_iMaxRating;
//	kPacket.m_iRPoint				= kPacket_.m_iRPoint;
//	kPacket.m_iAPoint				= kPacket_.m_iAPoint;
//	kPacket.m_iWin					= kPacket_.m_iWin;
//	kPacket.m_iLose					= kPacket_.m_iLose;
//
//	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
//#ifdef SERV_2012_PVP_SEASON2
//	kPacket.m_fKFactor				= kPacket_.m_fKFactor;
//#endif SERV_2012_PVP_SEASON2
//	//}}
//
//#else
//	kPacket.m_iVSPoint			    = kPacket_.m_iVSPoint;
//	kPacket.m_iVSPointMax           = kPacket_.m_iVSPointMax;
//#endif SERV_PVP_NEW_SYSTEM
//	//}}
//	std::map< int, KDungeonClearInfo >::iterator mitDungeonClear;
//	for( mitDungeonClear = kPacket_.m_mapDungeonClear.begin(); mitDungeonClear != kPacket_.m_mapDungeonClear.end(); mitDungeonClear++ )
//	{
//		kPacket.m_vecDungeonClear.push_back( mitDungeonClear->first );
//	}
//	std::map< int, KTCClearInfo >::iterator mitTCClear;
//	for( mitTCClear = kPacket_.m_mapTCClear.begin(); mitTCClear != kPacket_.m_mapTCClear.end(); mitTCClear++ )
//	{
//		kPacket.m_vecTCClear.push_back( mitTCClear->first );
//	}
//	kPacket.m_kItemQuantityUpdate = kPacket_.m_kItemQuantityUpdate;
//	kPacket.m_kItemEnduranceUpdate = kPacket_.m_kItemEnduranceUpdate;
//	kPacket.m_kItemPositionUpdate = kPacket_.m_kItemPositionUpdate;
//	kPacket.m_iNumResurrectionStone = kPacket_.m_iNumResurrectionStone;
//	//{{ 2009. 10. 14  최육사	자동결제 부활석
//#ifdef AP_RESTONE
//	kPacket.m_iNumAutoPaymentResStone = kPacket_.m_iNumAutoPaymentResStone;
//#endif AP_RESTONE
//	//}}	
//
//	int iOK = NetError::ERR_ODBC_01;
//	//{{ 2012. 06. 11	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//	int iMapIDDummy = 0;
//#endif SERV_BATTLE_FIELD_SYSTEM
//	//}}
//
//	//{{ 2011. 10. 26	최육사	DB해킹 트랩
//#ifdef SERV_DB_HACKING_ED_UPDATE_TRAP
//	DO_QUERY( L"exec dbo.gup_update_ui_5", L"%d, %d, %d, %d, %d, %d, %d, %d",
//		% kPacket_.m_iUnitUID
//		% kPacket_.m_iEXP
//		% kPacket_.m_iLevel
//		% kPacket_.m_iED
//		% kPacket_.m_iSPoint		
//		% iMapIDDummy
//		% kPacket_.m_iSpirit
//		% kPacket_.m_bIsSpiritUpdated
//		);
//#else
//	//{{ 2011. 07. 22	최육사	대전 개편
//	//#ifdef SERV_PVP_NEW_SYSTEM
//	DO_QUERY( L"exec dbo.gup_update_unit_info", L"%d, %d, %d, %d, %d, %d, %d, %d",
//		% kPacket_.m_iUnitUID
//		% kPacket_.m_iEXP
//		% kPacket_.m_iLevel
//		% kPacket_.m_iED
//		% kPacket_.m_iSPoint		
//		% kPacket_.m_iMapID
//		% kPacket_.m_iSpirit
//		% kPacket_.m_bIsSpiritUpdated
//		);
//	//#else
//	//	DO_QUERY( L"exec dbo.gup_update_unit_info", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
//	//		% kPacket_.m_iUnitUID
//	//		% kPacket_.m_iEXP
//	//		% kPacket_.m_iLevel
//	//		% kPacket_.m_iED
//	//		% kPacket_.m_iVSPoint
//	//		% kPacket_.m_iVSPointMax
//	//		% kPacket_.m_iSPoint
//	//		% kPacket_.m_iWin
//	//		% kPacket_.m_iLose
//	//		% kPacket_.m_iMapID
//	//		% kPacket_.m_iSpirit
//	//		% kPacket_.m_bIsSpiritUpdated
//	//		);
//	//#endif SERV_PVP_NEW_SYSTEM
//	//}}
//#endif SERV_DB_HACKING_ED_UPDATE_TRAP
//	//}}	
//
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	if( iOK == NetError::NET_OK )
//	{
//		kPacket.m_iEXP = 0;
//		kPacket.m_iED = 0;
//		//{{ 2011. 07. 22	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
//#else
//		kPacket.m_iVSPoint = 0;
//		kPacket.m_iVSPointMax = 0;
//#endif SERV_PVP_NEW_SYSTEM
//		//}}
//    }
//    else
//    {
//        START_LOG( cerr, L"유닛 정보 업데이트 실패." )
//            << BUILD_LOG( iOK )
//			<< BUILD_LOG( kPacket_.m_iUnitUID )
//			<< BUILD_LOG( kPacket_.m_iEXP )
//			<< BUILD_LOG( kPacket_.m_iLevel )
//			<< BUILD_LOG( kPacket_.m_iED )
//			<< BUILD_LOG( kPacket_.m_iSPoint )
//			<< BUILD_LOG( kPacket_.m_iSpirit )
//			<< BUILD_LOG( kPacket_.m_bIsSpiritUpdated )
//            << END_LOG;
//
//		kPacket.m_iOK = iOK;
//    }
//
//	//////////////////////////////////////////////////////////////////////////
//	//{{ 2012. 02. 02	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
//	DO_QUERY( L"exec dbo.P_GUnitLastPosition_UPT", L"%d, %d, %d, %d",
//		% kPacket_.m_iUnitUID
//		% kPacket_.m_kLastPos.m_iMapID
//		% (int)kPacket_.m_kLastPos.m_ucLastTouchLineIndex
//		% kPacket_.m_kLastPos.m_usLastPosValue
//		);
//
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	if( iOK != NetError::NET_OK )
//	{
//		START_LOG( cerr, L"유닛 마을 위치 정보 업데이트 실패." )
//			<< BUILD_LOG( iOK )
//			<< BUILD_LOG( kPacket_.m_iUnitUID )
//			<< BUILD_LOG( kPacket_.m_kLastPos.m_iMapID )
//			<< BUILD_LOGc( kPacket_.m_kLastPos.m_ucLastTouchLineIndex )
//			<< BUILD_LOG( kPacket_.m_kLastPos.m_usLastPosValue )
//			<< END_LOG;
//
//		kPacket.m_iOK = iOK;
//	}
//#endif SERV_BATTLE_FIELD_SYSTEM
//	//}}
//	//////////////////////////////////////////////////////////////////////////	
//
//	//{{ 2011. 07. 22	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
//
//	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
//#ifdef SERV_2012_PVP_SEASON2
//	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_UPT",
//		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
//		% kPacket_.m_iUnitUID
//		% kPacket_.m_iOfficialMatchCnt
//		% kPacket_.m_iRating
//		% kPacket_.m_iMaxRating
//		% kPacket_.m_iRPoint
//		% kPacket_.m_iAPoint
//		% kPacket_.m_bIsWinBeforeMatch
//		% (int)kPacket_.m_cEmblemEnum
//		% kPacket_.m_iWin
//		% kPacket_.m_iLose
//		% kPacket_.m_fKFactor
//		);
//#else
//	DO_QUERY( L"exec dbo.gup_update_unitpvp"
//		, L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
//		% kPacket_.m_iUnitUID
//		% kPacket_.m_iOfficialMatchCnt
//		% kPacket_.m_iRating
//		% kPacket_.m_iMaxRating
//		% kPacket_.m_iRPoint
//		% kPacket_.m_iAPoint
//		% kPacket_.m_bIsWinBeforeMatch
//		% (int)kPacket_.m_cEmblemEnum
//		% kPacket_.m_iWin
//		% kPacket_.m_iLose
//		);
//#endif SERV_2012_PVP_SEASON2
//	//}}
//
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	if( iOK == NetError::NET_OK )
//	{
//		kPacket.m_iOfficialMatchCnt = 0;
//		kPacket.m_iRating = 0;
//		kPacket.m_iMaxRating = 0;
//		kPacket.m_iRPoint = 0;
//		kPacket.m_iAPoint = 0;
//		kPacket.m_iWin = 0;
//		kPacket.m_iLose = 0;
//		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
//#ifdef SERV_2012_PVP_SEASON2
//		kPacket.m_fKFactor = 0.f;
//#endif SERV_2012_PVP_SEASON2
//		//}}
//	}
//	else
//	{
//		START_LOG( cerr, L"유닛 대전 정보 업데이트 실패." )
//			<< BUILD_LOG( iOK )
//			//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
//#ifdef SERV_2012_PVP_SEASON2
//			<< BUILD_LOG( kPacket_.m_iUnitUID )
//			<< BUILD_LOG( kPacket_.m_iOfficialMatchCnt )
//			<< BUILD_LOG( kPacket_.m_iRating )
//			<< BUILD_LOG( kPacket_.m_iMaxRating )
//			<< BUILD_LOG( kPacket_.m_iRPoint )
//			<< BUILD_LOG( kPacket_.m_iAPoint )
//			<< BUILD_LOG( kPacket_.m_bIsWinBeforeMatch )
//			<< BUILD_LOGc( kPacket_.m_cEmblemEnum )
//			<< BUILD_LOG( kPacket_.m_iWin )
//			<< BUILD_LOG( kPacket_.m_iLose )
//			<< BUILD_LOG( kPacket_.m_fKFactor )
//#endif SERV_2012_PVP_SEASON2
//			//}}
//			<< END_LOG;
//
//		kPacket.m_iOK = iOK;
//	}
//#endif SERV_PVP_NEW_SYSTEM
//	//}}
//
//	//////////////////////////////////////////////////////////////////////////	
//	//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
//#ifdef SERV_LOGOUT_ED_CHECK
//	// 해당 캐릭터의 마지막 위치 정보 저장!
//	DO_QUERY( L"exec dbo.gup_update_unit_lastpoint", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iRealDataED );
//
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	if( iOK != NetError::NET_OK )
//	{
//		START_LOG( cerr, L"로그 오프 ED체크 정보 업데이트 실패." )
//			<< BUILD_LOG( iOK )
//			<< BUILD_LOG( kPacket_.m_iUnitUID )
//			<< BUILD_LOG( kPacket_.m_iRealDataED )
//			<< END_LOG;
//
//		kPacket.m_iOK = iOK;
//	}
//
//	// 해당 캐릭터의 마지막 플레이 상태값 저장!
//	if( kPacket_.m_kGamePlayStatus.IsEmpty() == false )
//	{
//		KGamePlayStatus kPlayStatus;
//		LIF( kPacket_.m_kGamePlayStatus.Get( kPlayStatus ) );
//
//		// 캐릭터 기본 정보 저장
//		DO_QUERY( L"exec dbo.P_GUnitPlayInfo_INS", L"%d, %d, %d, %d, %d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kPlayStatus.m_iMaxHP
//			% kPlayStatus.m_iCurHP
//			% kPlayStatus.m_iMaxMP
//			% kPlayStatus.m_iCurMP
//			% kPlayStatus.m_iCurHyperGage
//			% kPlayStatus.m_iCharAbilCount
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( kPacket_.m_iUnitUID )
//				<< BUILD_LOG( kPlayStatus.m_iMaxHP )
//				<< BUILD_LOG( kPlayStatus.m_iCurHP )
//				<< BUILD_LOG( kPlayStatus.m_iMaxMP )
//				<< BUILD_LOG( kPlayStatus.m_iCurMP )
//				<< BUILD_LOG( kPlayStatus.m_iCurHyperGage )
//				<< BUILD_LOG( kPlayStatus.m_iCharAbilCount )
//				<< END_LOG;
//
//			kPacket.m_iOK = iOK;
//		}
//
//		// 스킬 쿨타임 데이터 저장!
//		{
//			std::map< int, int >::const_iterator mit;
//			for( mit = kPlayStatus.m_mapSkillCoolTime.begin(); mit != kPlayStatus.m_mapSkillCoolTime.end(); ++mit )
//			{
//				DO_QUERY( L"exec dbo.P_GCoolTime_INS", L"%d, %d, %d, %d",
//					% kPacket_.m_iUnitUID
//					% KGamePlayStatus::CTT_SKILL_COOL_TIME
//					% mit->first
//					% mit->second
//					);
//
//				if( m_kODBC.BeginFetch() )
//				{
//					FETCH_DATA( iOK );
//					m_kODBC.EndFetch();
//				}
//
//				if( iOK != NetError::NET_OK )
//				{
//					START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
//						<< BUILD_LOG( iOK )
//						<< BUILD_LOG( kPacket_.m_iUnitUID )
//						<< BUILD_LOG( mit->first )
//						<< BUILD_LOG( mit->second )
//						<< END_LOG;
//
//					kPacket.m_iOK = iOK;
//				}
//			}
//		}
//
//		// 퀵슬롯 쿨타임 데이터 저장!
//		{
//			std::map< int, int >::const_iterator mit;
//			for( mit = kPlayStatus.m_mapQuickSlotCoolTime.begin(); mit != kPlayStatus.m_mapQuickSlotCoolTime.end(); ++mit )
//			{
//				DO_QUERY( L"exec dbo.P_GCoolTime_INS", L"%d, %d, %d, %d",
//					% kPacket_.m_iUnitUID
//					% KGamePlayStatus::CTT_QUICK_SLOT_COOL_TIME
//					% mit->first
//					% mit->second
//					);
//
//				if( m_kODBC.BeginFetch() )
//				{
//					FETCH_DATA( iOK );
//					m_kODBC.EndFetch();
//				}
//
//				if( iOK != NetError::NET_OK )
//				{
//					START_LOG( cerr, L"캐릭터 플레이 정보 저장 실패!" )
//						<< BUILD_LOG( iOK )
//						<< BUILD_LOG( kPacket_.m_iUnitUID )
//						<< BUILD_LOG( mit->first )
//						<< BUILD_LOG( mit->second )
//						<< END_LOG;
//
//					kPacket.m_iOK = iOK;
//				}
//			}
//		}
//	}
//
//#endif SERV_LOGOUT_ED_CHECK
//	//}}
//	//////////////////////////////////////////////////////////////////////////	
//
//	bool bUpdateFailed = false;
//	Query_UpdateDungeonClear( kPacket_.m_iUnitUID, kPacket_.m_mapDungeonClear, kPacket.m_vecDungeonClear );
//	Query_UpdateTCClear( kPacket_.m_iUnitUID, kPacket_.m_mapTCClear, kPacket.m_vecTCClear );
//	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
//	LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
//	Query_UpdateItemPosition( kPacket_.m_iUnitUID, kPacket_.m_kItemPositionUpdate, kPacket.m_kItemPositionUpdate );
//	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );
//
//	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
//	if( bUpdateFailed )
//	{
//		START_LOG( cout, L"증분값 업데이트 실패!" )
//			<< BUILD_LOG( kPacket_.m_iUnitUID )
//			<< BUILD_LOG( kPacket_.m_bFinal );
//
//		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
//		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
//		{
//			START_LOG( cout, L"아이템 정보" )
//				<< BUILD_LOG( mitQC->first )
//				<< BUILD_LOG( mitQC->second );
//		}
//	}
//	//}}
//
//	//////////////////////////////////////////////////////////////////////////	
//	// 부활석 변화량이 없으면 프로시저 호출안함.
//	if( kPacket_.m_iNumResurrectionStone != 0 )
//	{
//		DO_QUERY( L"exec dbo.gup_update_resurrection_stone", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iNumResurrectionStone );
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK == NetError::NET_OK )
//		{
//			kPacket.m_iNumResurrectionStone = 0;
//		}
//		else
//		{
//			START_LOG( cerr, L"부활석 업데이트 실패." )
//				<< BUILD_LOG( iOK )
//				<< END_LOG;
//
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//	//{{ 2009. 10. 14  최육사	자동결제 부활석
//#ifdef AP_RESTONE
//	if( kPacket_.m_iNumAutoPaymentResStone != 0 )
//	{
//		DO_QUERY( L"exec dbo.gup_update_resurrection_stone_autopay", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iNumAutoPaymentResStone );
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK == NetError::NET_OK )
//		{
//			kPacket.m_iNumAutoPaymentResStone = 0;
//		}
//		else
//		{
//			START_LOG( cerr, L"자동결제 부활석 업데이트 실패." )
//				<< BUILD_LOG( iOK )
//				<< END_LOG;
//
//			kPacket.m_iOK = iOK;
//		}
//	}
//#endif AP_RESTONE
//	//}}
//
//	//QUEST UPDATE
//	for( int i = 0; i < (int)kPacket_.m_vecQuestData.size(); i++ )
//	{
//		KQuestUpdate kQuest = kPacket_.m_vecQuestData[i];
//
//		DO_QUERY( L"exec dbo.gup_update_quest", L"%d, %d, %d, %d, %d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kQuest.m_iQuestID
//			% (int)kQuest.m_vecClearData[0]
//			% (int)kQuest.m_vecClearData[1]
//			% (int)kQuest.m_vecClearData[2]
//			% (int)kQuest.m_vecClearData[3]
//			% (int)kQuest.m_vecClearData[4]
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//	//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
//#ifdef SERV_DAY_QUEST
//
//	// 예약된 포기 퀘스트
//	for( u_int ui = 0; ui < kPacket_.m_vecGiveUpQuestList.size(); ++ui )
//	{
//		const int& iGiveUpQuestID = kPacket_.m_vecGiveUpQuestList[ui];
//
//		DO_QUERY( L"exec dbo.gup_delete_quest", L"%d, %d", % kPacket_.m_iUnitUID % iGiveUpQuestID );
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//#endif SERV_DAY_QUEST
//	//}}
//
//	////{{ 2008. 10. 7  최육사	타이틀
//	//TITLE MISSION UPDATE
//	for( u_int i = 0; i < kPacket_.m_vecMissionData.size(); i++ )
//	{
//		KMissionUpdate kMission = kPacket_.m_vecMissionData[i];
//
//		DO_QUERY( L"exec dbo.gup_update_title_mission", L"%d, %d, %d, %d, %d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kMission.m_iTitleID
//			% kMission.m_vecClearData[0]
//			% kMission.m_vecClearData[1]
//			% kMission.m_vecClearData[2]
//			% kMission.m_vecClearData[3]
//			% kMission.m_vecClearData[4]
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			START_LOG( cerr, L"미션 정보 업데이트 실패!" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( kPacket_.m_iUnitUID )
//				<< BUILD_LOG( kMission.m_iTitleID )
//				<< END_LOG;
//
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
//#ifdef SERV_TITLE_DATA_SIZE
//
//	// 장착중인 타이틀 업데이트
//	if( kPacket_.m_iInitTitleID != kPacket_.m_iEquippedTitleID )
//	{
//		DO_QUERY( L"exec dbo.gup_update_title_hang", L"%d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kPacket_.m_iInitTitleID
//			% kPacket_.m_iEquippedTitleID
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			START_LOG( cerr, L"칭호 장착 정보 업데이트 실패!" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( kPacket_.m_iUnitUID )
//				<< BUILD_LOG( kPacket_.m_iInitTitleID )
//				<< BUILD_LOG( kPacket_.m_iEquippedTitleID )
//				<< END_LOG;
//
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//#else
//
//	// 장착중인 타이틀 업데이트
//	if( kPacket_.m_sInitTitleID != kPacket_.m_sEquippedTitleID )
//	{
//		DO_QUERY( L"exec dbo.gup_update_title_hang", L"%d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kPacket_.m_sInitTitleID
//			% kPacket_.m_sEquippedTitleID
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			START_LOG( cerr, L"칭호 장착 정보 업데이트 실패!" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( kPacket_.m_iUnitUID )
//				<< BUILD_LOG( kPacket_.m_sInitTitleID )
//				<< BUILD_LOG( kPacket_.m_sEquippedTitleID )
//				<< END_LOG;
//
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//#endif SERV_TITLE_DATA_SIZE
//	//}}
//
//
//	//}}
//
//	//SKILL SLOT UPDATE
//	DO_QUERY( L"exec dbo.gup_insert_skill_slot_new", L"%d, %d, %d, %d, %d",
//		% kPacket_.m_iUnitUID
//		% kPacket_.m_vecSkillSlot[0]
//		% kPacket_.m_vecSkillSlot[1]
//		% kPacket_.m_vecSkillSlot[2]
//		% kPacket_.m_vecSkillSlot[3]
//		);
//
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	if( iOK != NetError::NET_OK )
//	{
//		kPacket.m_iOK = iOK;
//	}
//
//	//SKILL SLOT B UPDATE
//	if( kPacket_.m_vecSkillSlot.size() >= 8 )
//	{
//		// [벡터 조심]
//		DO_QUERY( L"exec dbo.gup_insert_skill_slot2_new", L"%d, %d, %d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kPacket_.m_vecSkillSlot[4]
//			% kPacket_.m_vecSkillSlot[5]
//			% kPacket_.m_vecSkillSlot[6]
//			% kPacket_.m_vecSkillSlot[7]
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			kPacket.m_iOK = iOK;
//		}
//	}
//
//	//COMMUNITY OPTION UPDATE
//	DO_QUERY( L"exec dbo.gup_update_community_opt", L"%d, %d, %d, %d, %d",
//		% kPacket_.m_iUnitUID
//		% (int)kPacket_.m_kDenyOptions.m_cDenyFriendShip
//		% (int)kPacket_.m_kDenyOptions.m_cDenyInviteGuild
//		% (int)kPacket_.m_kDenyOptions.m_cDenyParty
//		% (int)kPacket_.m_kDenyOptions.m_cDenyPersonalTrade
//		);
//
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	if( iOK != NetError::NET_OK )
//	{
//		kPacket.m_iOK = iOK;
//	}
//
//	//{{ 2008. 5. 5  최육사  장바구니 업데이트
//	{
//		std::map< int, int >::const_iterator mitWishList;
//		for( mitWishList = kPacket_.m_mapWishList.begin(); mitWishList != kPacket_.m_mapWishList.end(); ++mitWishList )
//		{
//			DO_QUERY( L"exec dbo.gup_update_WishList", L"%d, %d, %d", % FIRST_SENDER_UID % mitWishList->first % mitWishList->second );
//
//			if( m_kODBC.BeginFetch() )
//			{
//				FETCH_DATA( iOK );
//				m_kODBC.EndFetch();
//			}
//
//			if( iOK != NetError::NET_OK )
//			{
//				START_LOG( cerr, L"장바구니 DB업데이트 실패!" )
//					<< BUILD_LOG( iOK )
//					<< BUILD_LOG( FIRST_SENDER_UID )
//					<< BUILD_LOG( mitWishList->first )
//					<< BUILD_LOG( mitWishList->second )
//					<< END_LOG;
//
//				// [참고] 실패하더라도 굳이 롤백할 필요는 없음. 중요도가 낮기 때문..
//				kPacket.m_iOK = iOK;
//			}
//		}
//	}
//	//}}
//
//	//{{ 2009. 7. 7  최육사		랭킹 개편
//	{
//		std::vector< KHenirRankingInfo >::const_iterator vit = kPacket_.m_vecHenirRanking.begin();
//		if( vit != kPacket_.m_vecHenirRanking.end() )
//		{
//			const KHenirRankingInfo& kRankingInfo = *vit;
//
//			// RegDate 변환
//			CTime tRegDate = CTime( kRankingInfo.m_tRegDate );
//			std::wstring wstrRegDate = ( CStringW )( tRegDate.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );
//
//			DO_QUERY( L"exec dbo.gup_insert_rank_myrecord", L"%d, %d, %d, %d, %d, N\'%s\'",
//				% kPacket_.m_iUnitUID
//				% kRankingInfo.m_iStageCount
//				% kRankingInfo.m_ulPlayTime
//				% (int)kRankingInfo.m_ucLevel
//				% (int)kRankingInfo.m_cUnitClass
//				% wstrRegDate
//				);
//
//			if( m_kODBC.BeginFetch() )
//			{
//				FETCH_DATA( iOK );
//				m_kODBC.EndFetch();
//			}
//
//			if( iOK != NetError::NET_OK )
//			{
//				START_LOG( cerr, L"아케이드 랭킹 DB업데이트 실패!" )
//					<< BUILD_LOG( iOK )
//					<< BUILD_LOG( kPacket_.m_iUnitUID )
//					<< BUILD_LOG( kRankingInfo.m_iStageCount )
//					<< BUILD_LOG( kRankingInfo.m_ulPlayTime )
//					<< BUILD_LOG( (int)kRankingInfo.m_ucLevel )
//					<< BUILD_LOG( (int)kRankingInfo.m_cUnitClass )
//					<< BUILD_LOG( wstrRegDate )
//					<< END_LOG;
//
//				// 실패했을때에 대한 예외처리는?
//				kPacket.m_iOK = iOK;
//			}
//		}
//	}
//	//}}
//
//	//{{ 2009. 10. 7  최육사	길드
//#ifdef GUILD_TEST
//
//	// 길드 정보 업데이트
//	if( kPacket_.m_iGuildUID > 0 )
//	{
//		// 마지막 업데이트라면 로그아웃 처리
//		if( kPacket_.m_bFinal )
//		{
//			DO_QUERY( L"exec dbo.gup_update_guild_lastdate", L"%d, %d",
//				% kPacket_.m_iUnitUID
//				% kPacket_.m_iGuildUID
//				);
//
//			if( m_kODBC.BeginFetch() )
//			{
//				FETCH_DATA( iOK );
//				m_kODBC.EndFetch();
//			}
//
//			if( iOK != NetError::NET_OK )
//			{
//				kPacket.m_iOK = iOK;
//
//				START_LOG( cerr, L"길드 로그아웃 처리 실패!" )
//					<< BUILD_LOG( iOK )
//					<< BUILD_LOG( kPacket_.m_iUnitUID )
//					<< BUILD_LOG( kPacket_.m_iGuildUID )
//					<< END_LOG;
//			}
//		}
//
//		//////////////////////////////////////////////////////////////////////////
//		//{{ 2009. 10. 28  최육사	길드레벨
//		DO_QUERY( L"exec dbo.gup_update_guild_member_exp", L"%d, %d, %d",
//			% kPacket_.m_iUnitUID
//			% kPacket_.m_iGuildUID
//			% kPacket_.m_iHonorPoint
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			kPacket.m_iOK = iOK;
//
//			START_LOG( cerr, L"길드원 명예 포인트 업데이트 실패." )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( kPacket_.m_iUnitUID )
//				<< BUILD_LOG( kPacket_.m_iGuildUID )
//				<< BUILD_LOG( kPacket_.m_iHonorPoint )
//				<< END_LOG;
//		}
//		//}}
//		//////////////////////////////////////////////////////////////////////////		
//	}
//
//#endif GUILD_TEST
//	//}}
//
//	//{{ 2009. 12. 8  최육사	이벤트개편
//#ifdef CUMULATIVE_TIME_EVENT
//
//	{
//		std::vector< KCumulativeTimeEventInfo >::const_iterator vitCT;
//		for( vitCT = kPacket_.m_vecUpdateEventTime.begin(); vitCT != kPacket_.m_vecUpdateEventTime.end(); ++vitCT )
//		{
//			//{{ 2010. 06. 15  최육사	계정단위 접속시간 이벤트
//#ifdef SERV_ACC_TIME_EVENT
//			if( vitCT->m_bAccountEvent )
//			{
//				DO_QUERY( L"exec dbo.gup_update_remaintime_account", L"%d, %d, %d", % LAST_SENDER_UID % vitCT->m_iEventUID % vitCT->m_iCumulativeTime );
//
//				if( m_kODBC.BeginFetch() )
//				{
//					FETCH_DATA( iOK );
//					m_kODBC.EndFetch();
//				}
//
//				if( iOK != NetError::NET_OK )
//				{
//					kPacket.m_iOK = iOK;
//
//					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
//						<< BUILD_LOG( iOK )
//						<< BUILD_LOG( LAST_SENDER_UID )
//						<< BUILD_LOG( vitCT->m_iEventUID )
//						<< BUILD_LOG( vitCT->m_iCumulativeTime )
//						<< END_LOG;
//				}
//			}
//			else
//#endif SERV_ACC_TIME_EVENT
//			//}}
//			{
//				DO_QUERY( L"exec dbo.gup_update_remaintime", L"%d, %d, %d", % kPacket_.m_iUnitUID % vitCT->m_iEventUID % vitCT->m_iCumulativeTime );
//
//				if( m_kODBC.BeginFetch() )
//				{
//					FETCH_DATA( iOK );
//					m_kODBC.EndFetch();
//				}
//
//				if( iOK != NetError::NET_OK )
//				{
//					kPacket.m_iOK = iOK;
//
//					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
//						<< BUILD_LOG( iOK )
//						<< BUILD_LOG( kPacket_.m_iUnitUID )
//						<< BUILD_LOG( vitCT->m_iEventUID )
//						<< BUILD_LOG( vitCT->m_iCumulativeTime )
//						<< END_LOG;
//				}
//			}
//		}
//	}	
//
//#endif CUMULATIVE_TIME_EVENT
//	//}}
//
//	//{{ 2010. 8. 4	최육사	펫 시스템
//#ifdef SERV_PET_SYSTEM
//
//	// 펫 정보 업데이트	
//	BOOST_TEST_FOREACH( const KPetInfo&, kPet, kPacket_.m_vecPet )
//	{
//		DO_QUERY( L"exec dbo.gup_update_pet_info", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\', N\'%s\'", 
//			% kPet.m_iPetUID
//			% (int)kPet.m_cEvolutionStep
//			% kPet.m_sSatiety
//			% kPet.m_iIntimacy
//			% kPet.m_sExtroversion
//			% kPet.m_sEmotion
//			% kPet.m_bAutoFeed
//			% kPet.m_wstrLastFeedDate
//			% kPet.m_wstrLastSummonDate
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			kPacket.m_iOK = iOK;
//
//			START_LOG( cerr, L"펫 정보 업데이트 실패!" )
//				<< BUILD_LOG( kPacket.m_iOK )
//				<< BUILD_LOG( kPet.m_iPetUID )
//				<< BUILD_LOG( kPet.m_sSatiety )
//				<< BUILD_LOG( kPet.m_iIntimacy )
//				<< BUILD_LOG( kPet.m_sExtroversion )
//				<< BUILD_LOG( kPet.m_sEmotion )
//				<< BUILD_LOG( kPet.m_bAutoFeed )
//				<< BUILD_LOG( kPet.m_wstrLastFeedDate )
//				<< BUILD_LOG( kPet.m_wstrLastSummonDate )
//				<< END_LOG;
//
//			// DB업데이트가 실패 한다면 롤백 데이터를 넣자!
//			kPacket.m_vecPet.push_back( kPet );
//		}
//	}
//
//#endif SERV_PET_SYSTEM
//	//}}
//
//	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
//#ifdef SERV_CHAR_LOG
//	{
//		std::map< int, int >::const_iterator mitCGC;
//		for( mitCGC = kPacket_.m_mapCharGameCount.begin(); mitCGC != kPacket_.m_mapCharGameCount.end(); ++mitCGC )
//		{
//			DO_QUERY( L"exec dbo.gup_update_Character_Count", L"%d, %d, %d", 
//				% kPacket_.m_iUnitUID
//				% mitCGC->first
//				% mitCGC->second
//				);
//
//			if( m_kODBC.BeginFetch() )
//			{
//				FETCH_DATA( iOK );
//				m_kODBC.EndFetch();
//			}
//
//			if( iOK != NetError::NET_OK )
//			{
//				kPacket.m_iOK = iOK;
//
//				START_LOG( cerr, L"캐릭터 게임 카운트 정보 업데이트!" )
//					<< BUILD_LOG( kPacket.m_iOK )
//					<< BUILD_LOG( kPacket_.m_iUnitUID )
//					<< BUILD_LOG( mitCGC->first )
//					<< BUILD_LOG( mitCGC->second )
//					<< END_LOG;
//			}
//		}
//	}
//#endif SERV_CHAR_LOG
//	//}}
//	//{{ 2011. 08. 12   김민성      헤니르 개편 
//#ifdef SERV_NEW_HENIR_TEST
//	DO_QUERY( L"exec dbo.P_GHenirRewardCnt_UPD", L"%d, %d, %d, %d", 
//		% kPacket_.m_iUnitUID 
//		% kPacket_.m_kHenirRewardCnt.m_iNormal 
//		% kPacket_.m_kHenirRewardCnt.m_iPremium 
//		% kPacket_.m_kHenirRewardCnt.m_iEvent 
//		);
//
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	if( iOK != NetError::NET_OK )
//	{
//		kPacket.m_iOK = iOK;
//
//		START_LOG( cerr, L"캐릭터 게임 카운트 정보 업데이트!" )
//			<< BUILD_LOG( kPacket.m_iOK )
//			<< BUILD_LOG( kPacket_.m_iUnitUID )
//			<< BUILD_LOG( kPacket_.m_kHenirRewardCnt.m_iNormal )
//			<< BUILD_LOG( kPacket_.m_kHenirRewardCnt.m_iPremium )
//			<< BUILD_LOG( kPacket_.m_kHenirRewardCnt.m_iEvent )
//			<< END_LOG;
//	}
//#endif SERV_NEW_HENIR_TEST
//	//}}
//
//	//{{ 2012. 05. 6	박세훈	어둠의 문 개편
//#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
//	BOOST_TEST_FOREACH( const KRecordBuffInfo&, kRecordBuffInfo, kPacket_.m_vecRecordBuffInfo )
//	{
//		DO_QUERY( L"exec dbo.P_GBuffeffect_UPD", L"%d, %d, N\'%s\'",
//			% kPacket_.m_iUnitUID
//			% kRecordBuffInfo.m_iBuffID
//			% kRecordBuffInfo.m_wstrStartTime
//			);
//
//		if( m_kODBC.BeginFetch() )
//		{
//			FETCH_DATA( iOK );
//			m_kODBC.EndFetch();
//		}
//
//		if( iOK != NetError::NET_OK )
//		{
//			START_LOG( cerr, L"버프 정보 업데이트 실패!" )
//				<< BUILD_LOG( kPacket_.m_iUnitUID )
//				<< BUILD_LOG( kRecordBuffInfo.m_iBuffID )
//				<< BUILD_LOG( kRecordBuffInfo.m_wstrStartTime )
//				<< END_LOG;
//		}
//	}
//#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//	//}}
//
//end_proc:
//
//    if( kPacket_.m_bFinal  &&  kPacket_.m_iEventID == 0 )
//		return;
//
//	SendToUser( LAST_SENDER_UID, DBE_UPDATE_UNIT_INFO_ACK, kPacket );
//}

//////////////////////////////////////////////////////////////////////////
#endif SERV_DB_UPDATE_UNIT_INFO_REFACTORING
//////////////////////////////////////////////////////////////////////////
//}}

_IMPL_ON_FUNC( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, std::wstring )
{
	KEGS_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_UNIT_WAIT_DELETE //2012.03.07 lygan_조성욱 // 캐릭터 삭제와 복구일을 문자열이 아닌 숫자로 변환하기 위해
	std::wstring wstrDelAbleDate  = L"";
	std::wstring wstrRestoreAbleDate  = L"";
	CTime tDelAbleDateTime;
	CTime tRestoreAbleDateTime;
#endif SERV_UNIT_WAIT_DELETE

#ifdef SERV_ALL_RENEWAL_SP
	int iDefaultUnitSlotCount = 10;	// 기본 국내
#if defined( SERV_COUNTRY_TWHK )
	iDefaultUnitSlotCount = 5;
#elif defined( SERV_COUNTRY_JP )
	iDefaultUnitSlotCount = 11;
#elif defined( SERV_COUNTRY_EU )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_US )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_CN )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_TH )
	iDefaultUnitSlotCount = 4;
#elif defined( SERV_COUNTRY_ID )
	iDefaultUnitSlotCount = 4;	
#elif defined( SERV_COUNTRY_BR )
	iDefaultUnitSlotCount = 5;
#elif defined( SERV_COUNTRY_PH )
	iDefaultUnitSlotCount = 3;
#elif defined( SERV_COUNTRY_IN )
	iDefaultUnitSlotCount = 3;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

#ifdef SERV_4TH_ANNIVERSARY_EVENT
	// 4주년 정보 받아온다. 첫 접속, 캐릭터 삭제일 등등
	DO_QUERY( L"exec dbo.P_GEvent_UserRecord_SEL", L"%d", % LAST_SENDER_UID );
	if( m_kODBC.BeginFetch() )
	{
		std::wstring wstrTime[4];
		CTime tTIme[4];

		FETCH_DATA( wstrTime[0]
		>> wstrTime[1]
		>> wstrTime[2]
		>> wstrTime[3]
		>> kPacket.m_4ThAnnivEventInfo.m_iLongestConnectTime
			>> kPacket.m_4ThAnnivEventInfo.m_iCountQuestComplete
			>> kPacket.m_4ThAnnivEventInfo.m_iCountQuestComplete
			>> kPacket.m_4ThAnnivEventInfo.m_iCountReceivedPost
			>> kPacket.m_4ThAnnivEventInfo.m_iDayTotalConnect
			>> kPacket.m_4ThAnnivEventInfo.m_iCountPvpLose
			>> kPacket.m_4ThAnnivEventInfo.m_iCountResurrect
			>> kPacket.m_4ThAnnivEventInfo.m_iCountDungeonClear );

		for(int i = 0; i < 4; ++i)
		{
			if( KncUtil::ConvertStringToCTime( wstrTime[i], tTIme[i] ) == false )
			{
				START_LOG( cerr, L"시간 변환 실패" )
					<< BUILD_LOG( wstrTime[i] )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< END_LOG;
			}
		}

		kPacket.m_4ThAnnivEventInfo.m_tTimeFirstPlay = tTIme[0].GetTime();
		kPacket.m_4ThAnnivEventInfo.m_tTimeFirstPet = tTIme[1].GetTime();
		kPacket.m_4ThAnnivEventInfo.m_tTimeFirstHenir = tTIme[2].GetTime();
		kPacket.m_4ThAnnivEventInfo.m_tTimeFirstDeleteChar = tTIme[3].GetTime();

		START_LOG(clog, L"[4주년] DB에서 4주년에 사용될 계정 정보 가져 왔다")
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( wstrTime[0] )
			<< BUILD_LOG( kPacket.m_4ThAnnivEventInfo.m_iItemIDFirstBuy )
			<< BUILD_LOG( kPacket.m_4ThAnnivEventInfo.m_iCountDungeonClear )
			<< END_LOG;

		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"P_GEvent_UserRecord_SEL 호출 실패" )
			<< END_LOG;
	}
#endif // SERV_4TH_ANNIVERSARY_EVENT

	std::vector< KUnitInfo >::iterator vit;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUser_MER", L"%d, N\'%s\', %d", % LAST_SENDER_UID % kPacket_.c_str() % iDefaultUnitSlotCount );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_user_info", L"%d, N\'%s\'", % LAST_SENDER_UID % kPacket_.c_str() );
#endif //SERV_ALL_RENEWAL_SP	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_nUnitSlot );

		m_kODBC.EndFetch();
	}

	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUserUID", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_Userinfo_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
#else
	DO_QUERY( L"exec dbo.gup_get_unit_info_by_useruid", L"%d", % LAST_SENDER_UID );
#endif SERV_2012_PVP_SEASON2
	//}}
    while( m_kODBC.Fetch() )
    {
        KUnitInfo kUnitInfo;
		//{{ 2012. 06. 11	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		int iMapIDDummy = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2		
		
		//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kUnitInfo.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kUnitInfo.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_iOfficialMatchCnt
			>> kUnitInfo.m_cRank
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kUnitInfo.m_wstrNickName
			>> iMapIDDummy
			>> kUnitInfo.m_wstrLastDate
			>> kUnitInfo.m_bDeleted
			>> wstrDelAbleDate
			>> wstrRestoreAbleDate
			);
#else SERV_UNIT_WAIT_DELETE
		FETCH_DATA( kUnitInfo.m_nUnitUID
		>> kUnitInfo.m_cUnitClass
		>> kUnitInfo.m_iEXP
		>> kUnitInfo.m_ucLevel
		>> kUnitInfo.m_iED
		>> kUnitInfo.m_iOfficialMatchCnt
		>> kUnitInfo.m_cRank
		>> kUnitInfo.m_kStat.m_iBaseHP
		>> kUnitInfo.m_kStat.m_iAtkPhysic
		>> kUnitInfo.m_kStat.m_iAtkMagic
		>> kUnitInfo.m_kStat.m_iDefPhysic
		>> kUnitInfo.m_kStat.m_iDefMagic
		>> kUnitInfo.m_iSPoint
		>> kUnitInfo.m_wstrNickName
		>> iMapIDDummy
		);
#endif SERV_UNIT_WAIT_DELETE
		//}}		
		
		// GetPvpRankForClient()
		// 캐릭터 리스트를 읽어 갈적엔 유저의 대전 정보가 채워져 있지 않고 DB에서 바로 랭크 정보를 읽어가기 때문에
		// 배치 랭커에 대한 변환 절차를 이곳에서 바로 수행하였다. ( 읽어간 랭크 정보는 서버에 기록되지 않는다 )
		if( kUnitInfo.m_iOfficialMatchCnt < 10 )
		{
			kUnitInfo.m_cRank = static_cast<char>( CXSLUnit::PVPRANK_RANK_ARRANGE );
		}
#else
		//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kUnitInfo.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kUnitInfo.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_iRating
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kUnitInfo.m_wstrNickName
			>> iMapIDDummy
			>> kUnitInfo.m_wstrLastDate
			>> kUnitInfo.m_bDeleted
			>> wstrDelAbleDate
			>> wstrRestoreAbleDate
			);
#else SERV_UNIT_WAIT_DELETE
		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kUnitInfo.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kUnitInfo.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_iRating
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kUnitInfo.m_wstrNickName
			>> iMapIDDummy
			);
#endif SERV_UNIT_WAIT_DELETE
		//}}
#endif SERV_2012_PVP_SEASON2
//}}
#else
		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kUnitInfo.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kUnitInfo.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_iVSPoint
			>> kUnitInfo.m_iVSPointMax
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kUnitInfo.m_wstrNickName
			>> kUnitInfo.m_iWin
			>> kUnitInfo.m_iLose
			>> kUnitInfo.m_nMapID
			);
#endif SERV_PVP_NEW_SYSTEM
		//}}        

#ifdef SERV_UNIT_WAIT_DELETE
		//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
		if( KncUtil::ConvertStringToCTime( wstrDelAbleDate, tDelAbleDateTime ) == true )
			kUnitInfo.m_trDelAbleDate = tDelAbleDateTime.GetTime();
		else
			kUnitInfo.m_trDelAbleDate = 0LL;

		//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
		if( KncUtil::ConvertStringToCTime( wstrRestoreAbleDate, tRestoreAbleDateTime ) == true )
			kUnitInfo.m_trRestoreAbleDate = tRestoreAbleDateTime.GetTime();
		else
			kUnitInfo.m_trRestoreAbleDate = 0LL;
		//}}
#endif SERV_UNIT_WAIT_DELETE

		//////////////////////////////////////////////////////////////////////////
		if( CXSLUnit::IsValidUnitClass( static_cast<CXSLUnit::UNIT_CLASS>(kUnitInfo.m_cUnitClass) ) == false )
		{
			START_LOG( cout, L"[알림] 등록되지 않은 유닛 클래스입니다!" )
				<< BUILD_LOG( kUnitInfo.m_nUnitUID )
				<< BUILD_LOG( kUnitInfo.m_wstrNickName )
				<< BUILD_LOGc( kUnitInfo.m_cUnitClass );
			continue;
		}
		
		//////////////////////////////////////////////////////////////////////////		

		kPacket.m_vecUnitInfo.push_back( kUnitInfo );
	}

	// 장착 아이템 정보
	for( vit = kPacket.m_vecUnitInfo.begin(); vit != kPacket.m_vecUnitInfo.end(); ++vit )
	{
		std::map< UidType, int > mapItemUIDSlot;
		mapItemUIDSlot.clear();

		//{{ 2013. 05. 24	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		// 랜덤 소켓 정보 얻기
		std::map< UidType, std::vector< int > > mapRandomSocketList;

		DO_QUERY( L"exec dbo.P_GItemSocket_Random_SEL", L"%d", % vit->m_nUnitUID );
		while( m_kODBC.Fetch() )
		{
			UidType iItemUID = 0;
			int arrRandomSocketOption[CXSLSocketItem::RSC_MAX] = {0,};

			FETCH_DATA( iItemUID
				>> arrRandomSocketOption[0]
				>> arrRandomSocketOption[1]
				>> arrRandomSocketOption[2]
				>> arrRandomSocketOption[3]
				>> arrRandomSocketOption[4]
				);

				// 2-1. 랜덤 소켓 정보 업데이트
				int iCheckIdx = CXSLSocketItem::RSC_MAX;
				while( 0 <= --iCheckIdx )
				{
					if( arrRandomSocketOption[iCheckIdx] != 0 )
						break;
				}

				std::vector< int > vecRandomSocket;
				for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
				{
					vecRandomSocket.push_back( arrRandomSocketOption[iIdx] );
				}

				// itemuid값은 중복되지 않는것을 db에서 보장해야한다.
				mapRandomSocketList.insert( std::make_pair( iItemUID, vecRandomSocket ) );
		}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		// 보유 아이템을 얻는다.
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_SEL_EquippedItemList", L"%d", % vit->m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_Equipped_Item_SEL", L"%d", % vit->m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_BATTLE_FIELD_BOSS
		DO_QUERY( L"exec dbo.gup_get_equipped_item_list", L"%d", % vit->m_nUnitUID );
#endif // SERV_BATTLE_FIELD_BOSS
		while( m_kODBC.Fetch() )
		{
			int iEnchantLevel = 0;
			//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
			const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS
			int arrSocketOption[byteArraySize];
			memset( arrSocketOption, 0, sizeof(int) * byteArraySize );
//#else
//			short arrSocketOption[4] = {0,0,0,0};
//#endif SERV_ITEM_OPTION_DATA_SIZE
			//}} 
			KInventoryItemInfo kInventoryItemInfo;

			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			FETCH_DATA( kInventoryItemInfo.m_iItemUID
				>> kInventoryItemInfo.m_kItemInfo.m_iItemID
				>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
				>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
				>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
				>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
				>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
				>> iEnchantLevel
				>> arrSocketOption[0]
				>> arrSocketOption[1]
				>> arrSocketOption[2]
				>> arrSocketOption[3]
				>> arrSocketOption[4]
				>> kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
				>> kInventoryItemInfo.m_cSlotCategory
				>> kInventoryItemInfo.m_sSlotID
				);
#else // SERV_BATTLE_FIELD_BOSS
			FETCH_DATA( kInventoryItemInfo.m_iItemUID
				>> kInventoryItemInfo.m_kItemInfo.m_iItemID
				>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
				>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
				>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
				>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
				>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
				>> iEnchantLevel
				>> arrSocketOption[0]
				>> arrSocketOption[1]
				>> arrSocketOption[2]
				>> arrSocketOption[3]
				>> kInventoryItemInfo.m_cSlotCategory
				>> kInventoryItemInfo.m_sSlotID );
#endif // SERV_BATTLE_FIELD_BOSS
//#else
//			FETCH_DATA( kInventoryItemInfo.m_iItemUID
//				>> kInventoryItemInfo.m_kItemInfo.m_iItemID
//				>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
//				>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
//				>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
//				>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
//				>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
//				>> iEnchantLevel
//				>> arrSocketOption[0]
//				>> arrSocketOption[1]
//				>> arrSocketOption[2]
//				>> arrSocketOption[3]
//				>> kInventoryItemInfo.m_cSlotCategory
//				>> kInventoryItemInfo.m_cSlotID );
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}

			//{{ 2008. 2. 20  최육사  강화
			kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel = static_cast<char>(iEnchantLevel);
			//}}

			//{{ 2008. 3. 7  최육사  소켓
			int iCheckIdx = byteArraySize;
			while( 0 <= --iCheckIdx )
			{
				if( arrSocketOption[iCheckIdx] != 0 )
					break;
			}

			for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
			{
				kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );				
			}
			//}}

			//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			// 2-1. 랜덤 소켓 정보 업데이트
			std::map< UidType, std::vector< int > >::const_iterator mitCRS;
			mitCRS = mapRandomSocketList.find( kInventoryItemInfo.m_iItemUID );
			if( mitCRS != mapRandomSocketList.end() )
			{
				kInventoryItemInfo.m_kItemInfo.m_vecRandomSocket = mitCRS->second;
			}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}

            std::map< int, KInventoryItemInfo >::iterator mit;
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			mit = vit->m_mapEquippedItem.find( kInventoryItemInfo.m_sSlotID );
//#else
//			mit = vit->m_mapEquippedItem.find( kInventoryItemInfo.m_cSlotID );
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
//			//}}
            if( mit != vit->m_mapEquippedItem.end() )
            {
                START_LOG( cerr, L"장착 아이템 정보가 중복됨." )
                    << BUILD_LOG( kInventoryItemInfo.m_iItemUID )
					//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
					<< BUILD_LOG( kInventoryItemInfo.m_sSlotID )
//#else
//					<< BUILD_LOGc( kInventoryItemInfo.m_cSlotID )
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
					//}}
                    << BUILD_LOG( kInventoryItemInfo.m_kItemInfo.m_iItemID )
                    << END_LOG;
            }
			//{{ 2008. 9. 26  최육사	속성강화 캐릭터 리스트 표시
			//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
//#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
			vit->m_mapEquippedItem.insert( std::make_pair( kInventoryItemInfo.m_sSlotID, kInventoryItemInfo ) );

			mapItemUIDSlot.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo.m_sSlotID ) );
//#else
//			vit->m_mapEquippedItem.insert( std::make_pair( kInventoryItemInfo.m_cSlotID, kInventoryItemInfo ) );
//
//			mapItemUIDSlot.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo.m_cSlotID ) );
//#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
			//}}
        }

		//////////////////////////////////////////////////////////////////////////
		// 속성 강화 얻기
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemAttribute_SEL", L"%d", % vit->m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_attribute", L"%d", % vit->m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			UidType iItemUID = 0;
			int iAttribEnchantSlotNo = 0;
			int iAttribEnchantID = 0;

			FETCH_DATA( iItemUID
				>> iAttribEnchantSlotNo
				>> iAttribEnchantID );

			std::map< UidType, int >::const_iterator mitSlot;
			mitSlot = mapItemUIDSlot.find( iItemUID );
			if( mitSlot == mapItemUIDSlot.end() )
				continue;            

			std::map< int, KInventoryItemInfo >::iterator mitAtt;
			mitAtt = vit->m_mapEquippedItem.find( mitSlot->second );
			if( mitAtt == vit->m_mapEquippedItem.end() )
				continue;

			// 속성 강화
			switch( iAttribEnchantSlotNo )
			{
			case CXSLAttribEnchantItem::ESI_SLOT_1:
				mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>( iAttribEnchantID );
				break;

			case CXSLAttribEnchantItem::ESI_SLOT_2:
				mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>( iAttribEnchantID );
				break;

			case CXSLAttribEnchantItem::ESI_SLOT_3:
				mitAtt->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>( iAttribEnchantID );
				break;

			default:
				START_LOG( cerr, L"속성 강화 슬롯 넘버가 이상합니다." )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOG( iAttribEnchantSlotNo )
					<< BUILD_LOG( iAttribEnchantID )
					<< END_LOG;
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//{{ 2009. 9. 25  최육사	길드
#ifdef GUILD_TEST

		// 길드 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_SEL", L"%d", % vit->m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_my_guild_info", L"%d", % vit->m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( vit->m_kUserGuildInfo.m_iGuildUID
				>> vit->m_kUserGuildInfo.m_wstrGuildName );

			m_kODBC.EndFetch();
		}

#endif GUILD_TEST
		//}}
    }

#ifdef SERV_SHARING_BANK_QUEST_CASH
	// 지헌 : 캐쉬템 안보이도록 하기 위해서, 은행 공유 했는지 체크한다.

	//1. 은행 공유 확인 SP  호출
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBankShare_SEL", L"%d",
		% LAST_SENDER_UID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBankShare_CHK", L"%d",
		% LAST_SENDER_UID
		);
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
	}

	//2. 은행 공유 유무를 패킷에 넣자.
	if(kPacket.m_iOK != NetError::NET_OK)
	{
		START_LOG(clog2, L"지헌로그:은행공유가 활성화 되어 있지 않다.")
			<< BUILD_LOG(LAST_SENDER_UID)
			<< BUILD_LOG(kPacket.m_iOK)
			<< END_LOG;

		kPacket.m_bSharingBank = false;
	}
	else
	{
		kPacket.m_bSharingBank = true;
	}
#endif SERV_SHARING_BANK_QUEST_CASH

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-12	// 박세훈
	{
		KDBE_JUMPING_CHARACTER_INFO_NOT kPacket;

		DO_QUERY( L"exec dbo.P_GEvent_Jumping_SEL", L"%d", % LAST_SENDER_UID );

		while( m_kODBC.Fetch() )
		{
			UidType			iUnitUID;
			std::wstring	wstrRegDate;

			FETCH_DATA( iUnitUID
				>> wstrRegDate
				);

			kPacket.m_mapJumpingInfo.insert( std::map< UidType, std::wstring >::value_type( iUnitUID, wstrRegDate ) );
		}

		SendToUser( LAST_SENDER_UID, DBE_JUMPING_CHARACTER_INFO_NOT, kPacket );
	}
#endif // SERV_JUMPING_CHARACTER

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
        << BUILD_LOG( LAST_SENDER_UID );

    SendToUser( LAST_SENDER_UID, DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_ACK, kPacket );
}

//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
IMPL_ON_FUNC( DBE_GAME_CREATE_UNIT_REQ )
#else
_IMPL_ON_FUNC( DBE_GAME_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_REQ )
#endif SERV_NEW_CREATE_CHAR_EVENT
//}}
{
    KEGS_CREATE_UNIT_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;

    START_LOG( clog, L"유닛 생성" )
        << BUILD_LOG( kPacket_.m_wstrNickName )
        << BUILD_LOG( kPacket_.m_iClass );

#ifdef SERV_NICK_NAME_DOUBLE_CHECK
	for ( int i = 0; i < (int)kPacket_.m_wstrNickName.size(); i++ )
	{
		WCHAR tempChar = kPacket_.m_wstrNickName[i];

		if ( !((tempChar >= 'a' && tempChar <= 'z') || (tempChar >= 'A' && tempChar <= 'Z') || (tempChar >= '0' && tempChar <= '9')))
		{
			START_LOG( cerr, L"[ERROR] 닉네임에 잘못된 문자 포함!" )
				<< BUILD_LOG( kPacket_.m_wstrNickName )
				<< END_LOG;
			
			kPacket.m_iOK = NetError::ERR_CREATE_UNIT_08;
			kPacket.m_kUnitInfo.m_wstrNickName = kPacket_.m_wstrNickName;
			kPacket.m_kUnitInfo.m_cUnitClass = kPacket_.m_iClass;
			goto end_proc;
		}
	}
#endif //SERV_NICK_NAME_DOUBLE_CHECK

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_INS", L"%d, N\'%s\', %d",
		% LAST_SENDER_UID
		% kPacket_.m_wstrNickName
		% kPacket_.m_iClass
		// 변하지 않는 값을 읽기만 하므로 이 쓰레드에서 참조해도 괜찮을듯..
		//% GetKGameServer()->GetServerGroupID()
		);
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_create_unit", L"%d, N\'%s\', %d",
				% LAST_SENDER_UID
				% kPacket_.m_wstrNickName
				% kPacket_.m_iClass
				// 변하지 않는 값을 읽기만 하므로 이 쓰레드에서 참조해도 괜찮을듯..
				//% GetKGameServer()->GetServerGroupID()
				);
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK
                 >> kPacket.m_kUnitInfo.m_nUnitUID
				 //{{ 2008. 4. 8  최육사  삭제된 닉네임 사용 가능 날짜
				 >> kPacket.m_wstrEnableDate
				 //}}
				 );

        m_kODBC.EndFetch();
    }	

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        switch( kPacket.m_iOK )
        {
        case -1:    kPacket.m_iOK = NetError::ERR_CREATE_UNIT_00;   break;
        case -2:    kPacket.m_iOK = NetError::ERR_CREATE_UNIT_01;   break;
        case -3:    kPacket.m_iOK = NetError::ERR_CREATE_UNIT_05;   break;
        case -10:   kPacket.m_iOK = NetError::ERR_CREATE_UNIT_02;   break;
		case -222:  kPacket.m_iOK = NetError::ERR_CREATE_UNIT_06;	break;
		case -23:   kPacket.m_iOK = NetError::ERR_CREATE_UNIT_09;	break;
#ifdef SERV_LIMIT_TO_CREATE_NEW_CHARACTER_EVENT
		case -99:   kPacket.m_iOK = NetError::NOT_CREATE_CHARACTER_BY_COUNT;	break;
		case -900:   kPacket.m_iOK = NetError::NOT_CREATE_CHARACTER_BY_COUNT;	break;
		case -901:   kPacket.m_iOK = NetError::NOT_CREATE_CHARACTER_BY_COUNT;	break;
#endif //SERV_LIMIT_TO_CREATE_NEW_CHARACTER_EVENT
		default:
			START_LOG( cerr, L"gup_create_unit: 정의되지 않은 결과값입니다." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;
			break;
        }

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		kPacket.m_bReqGiant_DeleteName = true;
		kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

        goto end_proc;
    }

    kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUnitUID", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_get_unit_info_by_unituid", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
		bool bIsSpiritUpdated = false;
		UidType iRecommendUnitUID = 0;
		//{{ 2012. 06. 11	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		int iMapIDDummy = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		FETCH_DATA( kPacket.m_kUnitInfo.m_nUnitUID
			>> kPacket.m_kUnitInfo.m_cUnitClass
			>> kPacket.m_kUnitInfo.m_iEXP
			>> kPacket.m_kUnitInfo.m_ucLevel
			>> kPacket.m_kUnitInfo.m_iED
			>> kPacket.m_kUnitInfo.m_kStat.m_iBaseHP
			>> kPacket.m_kUnitInfo.m_kStat.m_iAtkPhysic
			>> kPacket.m_kUnitInfo.m_kStat.m_iAtkMagic
			>> kPacket.m_kUnitInfo.m_kStat.m_iDefPhysic
			>> kPacket.m_kUnitInfo.m_kStat.m_iDefMagic
			>> kPacket.m_kUnitInfo.m_iSPoint
			>> kPacket.m_kUnitInfo.m_wstrNickName
			>> iMapIDDummy
			>> kPacket.m_kUnitInfo.m_iSpirit
			>> bIsSpiritUpdated
			>> iRecommendUnitUID
			);
#else
		FETCH_DATA( kPacket.m_kUnitInfo.m_nUnitUID
			>> kPacket.m_kUnitInfo.m_cUnitClass
			>> kPacket.m_kUnitInfo.m_iEXP
			>> kPacket.m_kUnitInfo.m_ucLevel
			>> kPacket.m_kUnitInfo.m_iED
			>> kPacket.m_kUnitInfo.m_iVSPoint
			>> kPacket.m_kUnitInfo.m_iVSPointMax
			>> kPacket.m_kUnitInfo.m_kStat.m_iBaseHP
			>> kPacket.m_kUnitInfo.m_kStat.m_iAtkPhysic
			>> kPacket.m_kUnitInfo.m_kStat.m_iAtkMagic
			>> kPacket.m_kUnitInfo.m_kStat.m_iDefPhysic
			>> kPacket.m_kUnitInfo.m_kStat.m_iDefMagic
			>> kPacket.m_kUnitInfo.m_iSPoint
			>> kPacket.m_kUnitInfo.m_wstrNickName
			>> kPacket.m_kUnitInfo.m_iWin
			>> kPacket.m_kUnitInfo.m_iLose
			>> kPacket.m_kUnitInfo.m_nMapID
			);
#endif SERV_PVP_NEW_SYSTEM
		//}}        

        m_kODBC.EndFetch();

		kPacket.m_iOK = NetError::NET_OK;
    }
	else
	{
		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;

		goto end_proc;
	}

#ifdef SERV_2012_PVP_SEASON2// 작업날짜: 2013-07-01	// 박세훈
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_SEL", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitPVP_Season2_GET", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_kUnitInfo.m_iOfficialMatchCnt
			>> kPacket.m_kUnitInfo.m_iRating
			>> kPacket.m_kUnitInfo.m_iMaxRating
			>> kPacket.m_kUnitInfo.m_iRPoint
			>> kPacket.m_kUnitInfo.m_iAPoint
			>> kPacket.m_kUnitInfo.m_bIsWinBeforeMatch
			>> kPacket.m_kUnitInfo.m_cRank
			>> kPacket.m_kUnitInfo.m_iWin
			>> kPacket.m_kUnitInfo.m_iLose
			>> kPacket.m_kUnitInfo.m_fKFactor
			>> kPacket.m_kUnitInfo.m_bIsRedistributionUser
			>> kPacket.m_kUnitInfo.m_iPastSeasonWin
			);
		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"P_GUnitPVP_Season2_SEL 호출 실패!" )
			<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
		goto end_proc;
	}
#endif // SERV_2012_PVP_SEASON2

	//{{ 2009. 3. 31  최육사	캐릭터 생성시 프로모션 입히기
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemPeriod_INS", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_unit_set_promotion", L"%d", % kPacket.m_kUnitInfo.m_nUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK	);
		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"캐릭터 생성시 옷입히기 쿼리 실패!" )
			<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
		goto end_proc;
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 생성시 옷입히기 쿼리 실패!" )
			<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
			<< BUILD_LOG( iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CREATE_UNIT_03;
		goto end_proc;
	}
	//}}

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
//#ifdef SERV_NEW_CREATE_CHAR_EVENT
//	DO_QUERY( L"exec dbo.gup_insert_event_unit", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % kPacket.m_kUnitInfo.m_nUnitUID % kPacket_.m_wstrRegDate );
//	if( m_kODBC.BeginFetch() )
//	{
//		FETCH_DATA( iOK );
//		m_kODBC.EndFetch();
//	}
//
//	switch( iOK )
//	{
//	case 0:
//		{
//			START_LOG( clog, L"캐릭터 생성시 아이템 지급 성공!!" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
//				<< BUILD_LOG( kPacket_.m_wstrRegDate )
//				<< END_LOG;
//
//		}break;
//	case -1:
//		{
//			START_LOG( cerr, L"계정 생성일을 체크했는데 신규가 아닐 때(롤백아님/그냥 종료)" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
//				<< BUILD_LOG( kPacket_.m_wstrRegDate )
//				<< END_LOG;
//		}break;
//	case -2:
//		{
//			START_LOG( cerr, L"큐브 우편으로 안보내졌을때" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
//				<< BUILD_LOG( kPacket_.m_wstrRegDate )
//				<< END_LOG;
//		}break;
//	case -3:
//		{
//			START_LOG( cerr, L"칭호 장착 안됐을때" )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
//				<< BUILD_LOG( kPacket_.m_wstrRegDate )
//				<< END_LOG;
//		}break;
//	case -4:
//		{
//			START_LOG( cerr, L"큐브 칭호 받은 캐릭 기록 실패했을때 " )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
//				<< BUILD_LOG( kPacket_.m_wstrRegDate )
//				<< END_LOG;
//		}break;
//	default:
//		{
//			START_LOG( cerr, L"없는 실패 값입니다." )
//				<< BUILD_LOG( iOK )
//				<< BUILD_LOG( LAST_SENDER_UID )
//				<< BUILD_LOG( kPacket.m_kUnitInfo.m_nUnitUID )
//				<< BUILD_LOG( kPacket_.m_wstrRegDate )
//				<< END_LOG;
//		}break;
//	}
//#endif SERV_NEW_CREATE_CHAR_EVENT
	//}}
	//////////////////////////////////////////////////////////////////////////

end_proc:

    SendToUser( LAST_SENDER_UID, DBE_GAME_CREATE_UNIT_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_GAME_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_REQ )
{
    KEGS_DELETE_UNIT_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
#ifdef GUILD_TEST
	kPacket.m_iGuildUID = 0;	
	std::wstring wstrGuildName;
#endif GUILD_TEST

	//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
	//{{	//2012.03.05 lygan_조성욱 // 캐릭터 삭제 할때 최종 삭제가 가능할 날 알아 오기 위해
	std::wstring wstrDelDate  = L"";
	std::wstring wstrDelAbleDate  = L"";
	std::wstring wstrRestoreAbleDate  = L"";
	std::wstring wstrReDelAbleDate = L"";
	CTime tReDelAbleDateTime;
	CTime tDelAbleDateTime;

	//}}//2012.03.05 lygan_조성욱 // 캐릭터 삭제 할때 최종 삭제가 가능할 날 알아 오기 위해
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_SEL_Check", L"%d, %d",	% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_ReDelAbleDate_CHK", L"%d, %d",	% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK 
			>> wstrReDelAbleDate
			);

		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"캐릭터 삭제 조건 사전 체크 결과 값이 왜 없지??" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_DELETE_UNIT_00;

		goto end_proc;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 삭제 조건 사전 체크 에러!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		if ( kPacket.m_iOK == -1 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_00;
		}
		else if( kPacket.m_iOK == -22 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_02;

			//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
			if( KncUtil::ConvertStringToCTime( wstrReDelAbleDate, tReDelAbleDateTime ) == true )
				kPacket.m_tReDelAbleDate = tReDelAbleDateTime.GetTime();
			else
				kPacket.m_tReDelAbleDate = 0LL;
			//}}
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}

		goto end_proc;
	}
#endif SERV_UNIT_WAIT_DELETE
	//}}

#ifdef SERV_RECRUIT_EVENT_BASE
	DO_QUERY( L"exec dbo.gup_delete_recommend", L"%d", % kPacket_.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"추천인 테이블에서 캐릭터 삭제 실패!" )
			<< BUILD_LOG( NetError::ERR_RECOMMEND_USER_06 )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
#endif SERV_RECRUIT_EVENT_BASE

	//{{ 2011. 02. 23	최육사	캐릭터 로그
#ifdef SERV_CHAR_LOG
	// 캐릭터 정보를 얻는다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUnitUID", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_info_by_unituid", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		KUnitInfo kUnitInfo;
		bool bIsSpiritUpdated = false;
		UidType iRecommendUnitUID = 0;
		//{{ 2012. 06. 11	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		int iMapIDDummy = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kPacket.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kPacket.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kPacket.m_wstrNickName
			>> iMapIDDummy
			>> kUnitInfo.m_iSpirit
			>> bIsSpiritUpdated
			>> iRecommendUnitUID
			);
#else
		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kPacket.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kPacket.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_iVSPoint			
			>> kUnitInfo.m_iVSPointMax
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kPacket.m_wstrNickName
			>> kUnitInfo.m_iWin
			>> kUnitInfo.m_iLose
			>> kUnitInfo.m_nMapID
			>> kUnitInfo.m_iSpirit
			>> bIsSpiritUpdated
			>> iRecommendUnitUID
			);
#endif SERV_PVP_NEW_SYSTEM
		//}}

		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_00;
		goto end_proc;
	}
#endif SERV_CHAR_LOG
	//}}

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT
	// 삭제도 못하도록 추가했습니다. by 박진웅
	if( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>( kPacket.m_cUnitClass ) ) == CXSLUnit::UT_ELESIS )
	{
		kPacket.m_iOK = NetError::ERR_DELETE_UNIT_07;
		goto end_proc;
	}
#endif SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef SERV_UNIT_WAIT_DELETE //2012.06.08 lygan_조성욱 // 닉네임 변경 대기 중인 캐릭터는 지우지 못하게 처리 하기 위해
	if(kPacket.m_wstrNickName.length() != 0)
	{
		if(kPacket.m_wstrNickName.find(L"_") != -1)
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_05;
			goto end_proc;
		}
	}
#endif //SERV_UNIT_WAIT_DELETE

	//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
	// 길드 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_SEL", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_my_guild_info", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iGuildUID
			>> wstrGuildName );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iGuildUID > 0 )
	{
		//////////////////////////////////////////////////////////////////////////
		// 길드 마스터 등급 검사
		u_char ucGuildMemberShipGrade = 0;
		
		if( Query_GetGuildMemberGrade( kPacket_.m_iUnitUID, ucGuildMemberShipGrade ) == false )
		{
			START_LOG( cerr, L"길드원 등급 정보 얻기 실패. GuildUID는 유효한데 DB에는 등급정보가 없다? 있을수 없는에러!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
			goto end_proc;
		}

		// 길드 마스터는 캐릭터 삭제를 할수 없다!
		if( ucGuildMemberShipGrade == SEnum::GUG_MASTER )
		{
			kPacket.m_iOK = NetError::ERR_GUILD_35;
			goto end_proc;
		}

		//////////////////////////////////////////////////////////////////////////
		// 길드 탈퇴 처리
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_DEL", L"%d, %d", 
			% kPacket_.m_iUnitUID
			% kPacket.m_iGuildUID
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_guild_member", L"%d, %d", 
			% kPacket_.m_iUnitUID
			% kPacket.m_iGuildUID
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드 탈퇴 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket.m_iGuildUID )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // 해당 길드에 속한 유저가 아닙니다.
			case -2: kPacket.m_iOK = NetError::ERR_GUILD_26; break; // 트랜젝션 에러
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // 길드 탈퇴 실패
			}

			goto end_proc;
		}
	}
#endif GUILD_TEST
	//}}

#ifdef SERV_ALL_RENEWAL_SP
	int iDelableDay = 1;	// 기본 국내
	int iRestoreableDay = 1;
#if defined( SERV_COUNTRY_TWHK )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_JP )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_EU )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_US )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_CN )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_TH )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_ID )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_BR )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_PH )
		iDelableDay = 1;
		iRestoreableDay = 1;
#elif defined( SERV_COUNTRY_IN )
		iDelableDay = 1;
		iRestoreableDay = 1;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

	//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitWaitDelete_INS", L"%d, %d, %d, %d", % kPacket_.m_iUnitUID 
															% kPacket_.m_iUserUID
															% iDelableDay
															% iRestoreableDay );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitWaitDelete_INT", L"%d, %d", % kPacket_.m_iUnitUID 
															% kPacket_.m_iUserUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}


	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 삭제 대기 처리 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		if( kPacket.m_iOK == -1 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_01;
		}
		else if ( kPacket.m_iOK == -11 || kPacket.m_iOK == -12 )
		{
			// -11 : 캐릭터 정보 갱신 실패, -12 : 사제 정보 제거 실패
			// 유저 입장에서는 DB에러로 봐야함
			kPacket.m_iOK = NetError::ERR_ODBC_01;
		} 
		else if ( kPacket.m_iOK == -22 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_02;
		}
	}


	//{{ //2012.03.05 lygan_조성욱 // 캐릭터 삭제 할때 최종 삭제가 가능할 날 알아 오기 위해
	DO_QUERY( L"exec dbo.P_GUnitWaitDelete_SEL", L"%d",	% kPacket_.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( wstrDelDate 
			>> wstrDelAbleDate
			>> wstrRestoreAbleDate
			);

		m_kODBC.EndFetch();
	}

	//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
	if( KncUtil::ConvertStringToCTime( wstrDelAbleDate, tDelAbleDateTime ) == true )
		kPacket.m_tDelAbleDate = tDelAbleDateTime.GetTime();
	else
		kPacket.m_tDelAbleDate = 0LL;
	//}}

	//}} //2012.03.05 lygan_조성욱 // 캐릭터 삭제 할때 최종 삭제가 가능할 날 알아 오기 위해
#else SERV_UNIT_WAIT_DELETE
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2012. 02. 21	김민성	캐릭터 삭제 및 길드 탈퇴 예외처리 수정
#ifdef SERV_UNIT_DELETE_EXCEPTION_MODIFY
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_Delete", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_unit", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_UNIT_DELETE_EXCEPTION_MODIFY
	DO_QUERY( L"exec dbo.gup_delete_unit", L"%d", % kPacket_.m_iUnitUID );
#endif SERV_UNIT_DELETE_EXCEPTION_MODIFY
	//}}

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );

        m_kODBC.EndFetch();
    }

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 삭제 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOG( m_kODBC.GetLastQuery() )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_DELETE_UNIT_00; break; // 해당 캐릭터를 찾을 수 없습니다.
			//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		case -2: kPacket.m_iOK = NetError::ERR_DELETE_UNIT_06; break; // 인연 상태에서는 삭제 할 수 없습니다.
#endif SERV_RELATIONSHIP_SYSTEM
			//}
		default: kPacket.m_iOK = NetError::ERR_DELETE_UNIT_05; break; // 캐릭터 삭제 실패
		}
	}
#endif SERV_UNIT_WAIT_DELETE
	//}}

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_GAME_DELETE_UNIT_ACK, kPacket );
}

//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_GAME_FINAL_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_REQ )
{
	KEGS_FINAL_DELETE_UNIT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	//{{ 2012. 06. 11	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int iMapIDDummy = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 02. 23	최육사	캐릭터 로그
#ifdef SERV_CHAR_LOG
	// 캐릭터 정보를 얻는다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_ByUnitUID", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_info_by_unituid", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		KUnitInfo kUnitInfo;
		bool bIsSpiritUpdated = false;
		UidType iRecommendUnitUID = 0;

		FETCH_DATA( kUnitInfo.m_nUnitUID
			>> kPacket.m_cUnitClass
			>> kUnitInfo.m_iEXP
			>> kPacket.m_ucLevel
			>> kUnitInfo.m_iED
			>> kUnitInfo.m_kStat.m_iBaseHP
			>> kUnitInfo.m_kStat.m_iAtkPhysic
			>> kUnitInfo.m_kStat.m_iAtkMagic
			>> kUnitInfo.m_kStat.m_iDefPhysic
			>> kUnitInfo.m_kStat.m_iDefMagic
			>> kUnitInfo.m_iSPoint
			>> kPacket.m_wstrNickName
			>> iMapIDDummy
			>> kUnitInfo.m_iSpirit				
			>> bIsSpiritUpdated
			>> iRecommendUnitUID			
			);

		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_SELECT_UNIT_00;
		goto end_proc;
	}
#endif SERV_CHAR_LOG
	//}}

	// 길드 정보는 이미 삭제 되었다고 봄 //
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_Delete_DelSystem", L"%d, %d", % kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_unit", L"%d, %d", % kPacket_.m_iUserUID
		% kPacket_.m_iUnitUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 최종 삭제 처리 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		if( kPacket.m_iOK == -21 || kPacket.m_iOK == - 23)
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_04;
		}

		else if ( kPacket.m_iOK == -22 )
		{
			kPacket.m_iOK = NetError::ERR_DELETE_UNIT_03;

		}
		else if (	kPacket.m_iOK == -24 || kPacket.m_iOK == -11 || 
			kPacket.m_iOK == -13 || kPacket.m_iOK == -15 )
		{
			kPacket.m_iOK = NetError::ERR_ODBC_01;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GAME_FINAL_DELETE_UNIT_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_GAME_RESTORE_UNIT_REQ, KEGS_RESTORE_UNIT_REQ )
{
	KEGS_RESTORE_UNIT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	std::wstring wstrRestoreAbleDate  = L"";
	CTime tRestoreAbleDateTime;

	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_INS", L"%d, %d",% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitRestore_SET", L"%d, %d",% kPacket_.m_iUnitUID
		% kPacket_.m_iUserUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK 
			>> wstrRestoreAbleDate
			);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 복구 처리 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< END_LOG;

		if( kPacket.m_iOK == -31 || kPacket.m_iOK == -33)
		{
			kPacket.m_iOK = NetError::ERR_RESTORE_UNIT_01;
		}

		else if ( kPacket.m_iOK == -32 )
		{
			kPacket.m_iOK = NetError::ERR_RESTORE_UNIT_02;
			//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
			if( KncUtil::ConvertStringToCTime( wstrRestoreAbleDate, tRestoreAbleDateTime ) == true )
				kPacket.m_tRestoreAbleDate = tRestoreAbleDateTime.GetTime();
			else
				kPacket.m_tRestoreAbleDate = 0LL;
			//}}

		}
		else if ( kPacket.m_iOK == -34 || kPacket.m_iOK == -11 )
		{
			kPacket.m_iOK = NetError::ERR_ODBC_01;
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GAME_RESTORE_UNIT_ACK, kPacket );
}
#endif SERV_UNIT_WAIT_DELETE
//}}

_IMPL_ON_FUNC( DBE_GAME_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_REQ )
{
	//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
	KDBE_SELECT_UNIT_ACK kPacket;
#else
	KEGS_SELECT_UNIT_ACK kPacket;
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	//}}

	// 캐릭터 선택
	Query_SelectUnit( LAST_SENDER_UID, kPacket_, kPacket );

#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT// 작업날짜: 2013-06-25	// 박세훈
	if( ( kPacket.m_iOK != NetError::ERR_SELECT_UNIT_01 )
		|| ( CXSLUnit::GetUnitClassToUnitType( static_cast<CXSLUnit::UNIT_CLASS>( kPacket.m_kUnitInfo.m_cUnitClass ) ) != CXSLUnit::UT_ADD )
		)
	{
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
        << BUILD_LOG( LAST_SENDER_UID )
        << BUILD_LOG( kPacket_.m_iUnitUID );
#ifdef SERV_ELISIS_PREVIOUS_SIS_EVENT// 작업날짜: 2013-06-25	// 박세훈
	}
#endif // SERV_ELISIS_PREVIOUS_SIS_EVENT

#ifdef SERV_ADD_EVENT_DB
	KDBE_GAME_SELECT_UNIT_EVENT_DATA_NOT kPacketNot;
	kPacketNot.m_kSelectUnitAck = kPacket;
	kPacketNot.m_iUserUID = LAST_SENDER_UID;
#ifdef SERV_GLOBAL_EVENT_TABLE
	kPacketNot.m_mapGlobalEventData = kPacket_.m_mapGlobalEventData;
#endif //SERV_GLOBAL_EVENT_TABLE

	SendToEventDB( DBE_GAME_SELECT_UNIT_EVENT_DATA_NOT, kPacketNot );
#else //SERV_ADD_EVENT_DB
    SendToUser( LAST_SENDER_UID, DBE_GAME_SELECT_UNIT_ACK, kPacket );
#endif //SERV_ADD_EVENT_DB
}

//{{ 2009. 5. 28  최육사	채널이동
IMPL_ON_FUNC( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ )
{
	KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK kPacket;
	kPacket.m_kConnectAck = kPacket_.m_kConnectAck;
	kPacket.m_kVerifyAccountAck = kPacket_.m_kVerifyAccountAck;
	kPacket.m_kChangeUserInfo = kPacket_.m_kChangeUserInfo;

	// 캐릭터 선택
	Query_SelectUnit( LAST_SENDER_UID, kPacket_.m_kSelectUnitReq, kPacket.m_kSelectUnitAck );

	LOG_SUCCESS( kPacket.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr( kPacket.m_kSelectUnitAck.m_iOK ) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_kSelectUnitReq.m_iUnitUID );

#ifdef SERV_ADD_EVENT_DB
	KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT kPacketNot;
	kPacketNot.m_kSelectUnitAck = kPacket;
	kPacketNot.m_iUserUID = LAST_SENDER_UID;

	SendToEventDB( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT, kPacketNot );
#else //SERV_ADD_EVENT_DB
	SendToUser( LAST_SENDER_UID, DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK, kPacket );
#endif //SERV_ADD_EVENT_DB
}
//}}

_IMPL_ON_FUNC( DBE_ADMIN_MODIFY_UNIT_LEVEL_REQ, KEGS_ADMIN_MODIFY_UNIT_LEVEL_REQ )
{
    KEGS_ADMIN_MODIFY_UNIT_LEVEL_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_00;
	kPacket.m_kUnitInfo.m_ucLevel = (UCHAR)kPacket_.m_iLevel;
	kPacket.m_kUnitInfo.m_iEXP = kPacket_.m_iEXP;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_.m_wstrUnitNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kPacket_.m_wstrUnitNickName );
#endif //SERV_ALL_RENEWAL_SP
	UidType	unitUID;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( unitUID );
		m_kODBC.EndFetch();

		kPacket.m_kUnitInfo.m_nUnitUID = unitUID;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_01;
		goto end_proc;
	}

	//{{ 2011. 07. 22	최육사	모든 쿼리 SP사용
#ifdef SERV_ALL_DB_QUERY_USE_SP
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_SEL_UnitClass", L"%d", % unitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unitclass_by_unituid", L"%d", % unitUID );
#endif //SERV_ALL_RENEWAL_SP
#else
	//DO_QUERY_NO_PROFILE( L"SELECT UnitClass FROM dbo.GUnit( NOLOCK )", L"WHERE UnitUID = %d", % unitUID );
#endif SERV_ALL_DB_QUERY_USE_SP
	//}}    

    int iClass;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iClass );
        m_kODBC.EndFetch();
    }
    else
    {
        kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_01;
        goto end_proc;
    }
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_Exp_Admin", L"%d, %d, %d",
		% unitUID
		% kPacket_.m_iEXP
		% kPacket_.m_iLevel
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_admin_update_unit_info", L"%d, %d, %d",
        % unitUID
        % kPacket_.m_iEXP
        % kPacket_.m_iLevel
        );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_ADMIN_MODIFY_UNIT_LEVEL_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_ED_REQ, KEGS_ADMIN_CHANGE_ED_REQ )
{
	KEGS_ADMIN_CHANGE_ED_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_.m_wstrUnitNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kPacket_.m_wstrUnitNickName );
#endif //SERV_ALL_RENEWAL_SP
	UidType	unitUID;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( unitUID );
		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;
		goto end_proc;
	}
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_GamePointAdmin", L"%d, %d",
		% unitUID
		% kPacket_.m_nED
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_admin_update_unit_gamepoint", L"%d, %d",
		% unitUID
		% kPacket_.m_nED
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_CHANGE_ED_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BUY_ED_ITEM_REQ )
{
    KDBE_BUY_ED_ITEM_ACK kPacket;
    kPacket.m_iED = 0;
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	kPacket.m_iAPoint = 0;
#else
	kPacket.m_iVSPoint = 0;
#endif SERV_PVP_NEW_SYSTEM
	//}}    
    kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
    LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
    LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
    Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	// DB에 구매한 ED아이템을 넣을 준비를 하자!
	std::vector< KItemInfo > vecInsertItemList;

	// 새로 마련한 컨테이너에 옮겨 담자!
	BOOST_TEST_FOREACH( const KBuyGPItemInfo&, kBuyGPItemInfo, kPacket_.m_vecBuyGPItemInfo )
    {
		KItemInfo kItemInfo;
		kItemInfo.m_iItemID		= kBuyGPItemInfo.m_iItemID;
		kItemInfo.m_cUsageType	= kBuyGPItemInfo.m_cPeriodType;
		kItemInfo.m_iQuantity	= kBuyGPItemInfo.m_iQuantity;
		kItemInfo.m_sEndurance	= kBuyGPItemInfo.m_sEndurance;
		kItemInfo.m_sPeriod		= kBuyGPItemInfo.m_sPeriod;
		vecInsertItemList.push_back( kItemInfo );
    }

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
#ifdef SERV_GET_ITEM_REASON_BY_CHEAT
	int iGetItemReason = ( true == kPacket_.m_bCheat ) ? SEnum::GIR_GET_ITEM_BY_CHEAT : SEnum::GIR_BUY_ED_ITEM;

	if( Query_InsertItemList( static_cast<SEnum::GET_ITEM_REASON>( iGetItemReason ), kPacket_.m_iUnitUID, vecInsertItemList, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( SEnum::GIR_BUY_ED_ITEM, kPacket_.m_iUnitUID, vecInsertItemList, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON_BY_CHEAT
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, vecInsertItemList, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
		//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

    SendToUser( LAST_SENDER_UID, DBE_BUY_ED_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_ITEM_REQ )
{
    KDBE_INSERT_ITEM_ACK kPacket;
	kPacket.m_bOutRoom = kPacket_.m_bOutRoom;
    kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
    kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	kPacket.m_mapGetItem = kPacket_.m_mapGetItem;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( static_cast<SEnum::GET_ITEM_REASON>(kPacket_.m_cGetItemReason), kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}	
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

    SendToUser( LAST_SENDER_UID, DBE_INSERT_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_NEW_QUEST_REQ )
{
	KDBE_NEW_QUEST_ACK kPacket;
	kPacket.m_iOK		= NetError::ERR_ODBC_01;
	kPacket.m_iQuestID	= kPacket_.m_iQuestID;
	//{{ 2010. 10. 27	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	//////////////////////////////////////////////////////////////////////////
	// 아이템 보상
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_UnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_UnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_QUEST_REWARD, kPacket_.m_UnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_UnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
		//}}
	{
		START_LOG( cerr, L"퀘스트 수락 보상 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		goto end_proc;
	}
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 퀘스트 수락
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GQuests_INS", L"%d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_quest", L"%d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"퀘스트 DB 생성 실패." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket.m_iQuestID )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_NEW_QUEST_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_GIVE_UP_QUEST_REQ )
{
	KDBE_GIVE_UP_QUEST_ACK kPacket;
	kPacket.m_iOK		= NetError::ERR_ODBC_01;
	kPacket.m_iQuestID	= kPacket_.m_iQuestID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GQuests_DEL", L"%d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_quest", L"%d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"퀘스트 DB 삭제 실패." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket.m_iQuestID )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GIVE_UP_QUEST_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_QUEST_COMPLETE_REQ )
{
	KDBE_QUEST_COMPLETE_ACK kPacket;
	std::vector< KItemInfo >::iterator vit;
	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	kPacket.m_bAutoComplete	= kPacket_.m_bAutoComplete;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	kPacket.m_bIsNew		= kPacket_.m_bIsNew;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
    kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
    kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_bIsChangeJob = kPacket_.m_bIsChangeJob;
	kPacket.m_cChangeUnitClass = kPacket_.m_cChangeUnitClass;	

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	kPacket.m_iNewDefaultSkill1 = kPacket_.m_iNewDefaultSkill1;
	kPacket.m_iNewDefaultSkill2 = kPacket_.m_iNewDefaultSkill2;	
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
	
	kPacket.m_kCompleteQuestInfo.m_iQuestID = kPacket_.m_iQuestID;
	std::wstring wstrCompleteDate;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GQuests_Complete_INS", L"%d, %d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID % kPacket_.m_bIsRepeat );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_complete_quest", L"%d, %d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID % kPacket_.m_bIsRepeat );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> wstrCompleteDate
			);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"퀘스트 완료 DB 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< BUILD_LOG( kPacket_.m_bIsRepeat )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}
	else
	{
		CTime tCompleteDate;
		LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );

		// 퀘스트 완료 업데이트 성공!
		kPacket.m_kCompleteQuestInfo.m_iQuestID = kPacket_.m_iQuestID;
		kPacket.m_kCompleteQuestInfo.m_iCompleteCount = 1;
		kPacket.m_kCompleteQuestInfo.m_tCompleteDate = tCompleteDate.GetTime();
	}

#else

	kPacket.m_iQuestID = kPacket_.m_iQuestID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GQuests_Complete_INS", L"%d, %d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID % kPacket_.m_bIsRepeat );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_complete_quest", L"%d, %d, %d", % kPacket_.m_UnitUID % kPacket_.m_iQuestID % kPacket_.m_bIsRepeat );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"퀘스트 완료 DB 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< BUILD_LOG( kPacket_.m_bIsRepeat )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}	

#endif SERV_DAILY_QUEST
	//}}

	//item uid 받아오기
	if( kPacket_.m_bIsNew == true )
	{
		bool bUpdateFailed = false;
		LIF( Query_UpdateItemQuantity( kPacket_.m_UnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
		Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

		//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
		if( bUpdateFailed )
		{
			START_LOG( cout, L"증분값 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_UnitUID );

			std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
			for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
			{
				START_LOG( cout, L"아이템 정보" )
					<< BUILD_LOG( mitQC->first )
					<< BUILD_LOG( mitQC->second );
			}
		}
		//}}

		//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
		if( Query_InsertItemList( SEnum::GIR_QUEST_REWARD, kPacket_.m_UnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
		if( Query_InsertItemList( kPacket_.m_UnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
		//}}
		{
			START_LOG( cerr, L"퀘스트 보상 아이템 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_UnitUID )
				<< END_LOG;
			goto end_proc;
		}
	}

	//전직퀘스트 였을 경우처리..
	if( kPacket_.m_bIsChangeJob == true )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		for ( int iSkillPagesNumber = 1; iSkillPagesNumber <= kPacket_.m_iTheNumberOfSkillPagesAvailable; iSkillPagesNumber++ )
		{
			DO_QUERY( L"exec P_GUnit_UPD_UnitClass_20131212", L"%d, %d, %d, %d, %d", 
				% kPacket_.m_UnitUID 
				% (int)kPacket_.m_cChangeUnitClass 
				% kPacket_.m_iNewDefaultSkill1 
				% kPacket_.m_iNewDefaultSkill2 
				% iSkillPagesNumber );

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}
			else
			{
				kPacket.m_iOK = NetError::ERR_QUEST_10;

				START_LOG( cerr, L"DB 처리 실패로 인한 유닛 퀘스트완료 전직실패.!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
					<< END_LOG;

				goto end_proc;
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				kPacket.m_iOK = NetError::ERR_QUEST_10;

				START_LOG( cerr, L"유닛 퀘스트완료 전직실패.!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
					<< END_LOG;

				goto end_proc;
			}
		}

#else // SERV_SKILL_PAGE_SYSTEM
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec P_GUnit_UPD_UnitClass_New", L"%d, %d, %d, %d", % kPacket_.m_UnitUID % (int)kPacket_.m_cChangeUnitClass % kPacket_.m_iNewDefaultSkill1 % kPacket_.m_iNewDefaultSkill2 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec P_GUnit_UPD_UnitClass", L"%d, %d, %d, %d", % kPacket_.m_UnitUID % (int)kPacket_.m_cChangeUnitClass % kPacket_.m_iNewDefaultSkill1 % kPacket_.m_iNewDefaultSkill2 );
#endif //SERV_ALL_RENEWAL_SP

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		DO_QUERY( L"exec gup_update_unit_class", L"%d, %d", % kPacket_.m_UnitUID % (int)kPacket_.m_cChangeUnitClass );
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_QUEST_10;

			START_LOG( cerr, L"DB 처리 실패로 인한 유닛 퀘스트완료 전직실패.!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
				<< END_LOG;

			goto end_proc;
		}

        if( kPacket.m_iOK != NetError::NET_OK )
		{
			kPacket.m_iOK = NetError::ERR_QUEST_10;

			START_LOG( cerr, L"유닛 퀘스트완료 전직실패.!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
				<< END_LOG;

			goto end_proc;
		}
#endif // SERV_SKILL_PAGE_SYSTEM

	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_QUEST_COMPLETE_ACK, kPacket );
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
IMPL_ON_FUNC( DBE_INSERT_SKILL_REQ )
{
	KDBE_INSERT_SKILL_ACK kAck;
	kAck.m_iOK						= NetError::ERR_ODBC_01;
	kAck.m_mapSkillList				= kPacket_.m_mapSkillList;
	kAck.m_iCSPoint					= kPacket_.m_iCSPoint;
	kAck.m_iTotalSpendSkillPoint	= kPacket_.m_iTotalSpendSkillPoint;
	kAck.m_iBeforeCSPoint			= kPacket_.m_iBeforeCSPoint;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kAck.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
    
	// kimhc // 김현철 // 2013-12-03 // 스킬 페이지 작업 시 이부분을 페이지 별로
	// sp를 호출 하는 것에 대해서 DBA와 얘기했을때
	// DB에 저장되는 CSP가 유효한 정보가 아닌 것으로 테스트 됨
	// 일단 삭제 or 변경 하지 않고 그냥 두기로 함

	// 캐시 스킬 포인트 정보를 갱신
	if( kPacket_.m_iCSPoint >= 0 )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_Cash_UPD_PointInfo", L"%d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_cash_skill_point_info", L"%d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );
			m_kODBC.EndFetch();
		}	

		if( kAck.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to update cash skill point when insert skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iCSPoint )
				<< BUILD_LOG( kAck.m_iOK )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_SKILL_24;

			goto end_proc;	
		}
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	// 스킬 획득 정보를 갱신
	{
		std::map< int, KGetSkillInfo >::iterator mit = kPacket_.m_mapSkillList.begin();
		for( ; mit != kPacket_.m_mapSkillList.end() ; ++mit )
		{
			int iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
				% kPacket_.m_iUnitUID 
				% mit->second.m_iSkillID
				% mit->second.m_iSkillLevel
				% mit->second.m_iSpendSkillCSPoint
				% kPacket_.m_iActiveSkillPageNumber );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_New_UPD", L"%d, %d, %d, %d, %d",
				% kPacket_.m_iUnitUID 
				% mit->second.m_iSkillID
				% mit->second.m_iSkillLevel
				% mit->second.m_iSpendSkillCSPoint
				% kPacket_.m_iActiveSkillPageNumber );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"failed to insert skill!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( mit->second.m_iSkillID )
					<< BUILD_LOG( mit->second.m_iSkillLevel )
					<< BUILD_LOG( mit->second.m_iSpendSkillCSPoint )
					<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
					<< BUILD_LOG( iOK )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_SKILL_00;

				goto end_proc;
			}
		}
	}
#else // SERV_SKILL_PAGE_SYSTEM
	// 스킬 획득 정보를 갱신
	{
		std::map< int, KGetSkillInfo >::iterator mit = kPacket_.m_mapSkillList.begin();
		for( ; mit != kPacket_.m_mapSkillList.end() ; ++mit )
		{
			int iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
				% kPacket_.m_iUnitUID 
				% mit->second.m_iSkillID
				% mit->second.m_iSkillLevel
				% mit->second.m_iSpendSkillCSPoint
				% 0 );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
				% kPacket_.m_iUnitUID 
				% mit->second.m_iSkillID
				% mit->second.m_iSkillLevel
				% mit->second.m_iSpendSkillCSPoint );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"failed to insert skill!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( mit->second.m_iSkillID )
					<< BUILD_LOG( mit->second.m_iSkillLevel )
					<< BUILD_LOG( mit->second.m_iSpendSkillCSPoint )
					<< BUILD_LOG( iOK )
					<< END_LOG;

				kAck.m_iOK = NetError::ERR_SKILL_00;

				goto end_proc;
			}
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM

	kAck.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_SKILL_ACK, kAck );
}

IMPL_ON_FUNC( DBE_INIT_SKILL_TREE_REQ )
{
	KDBE_INIT_SKILL_TREE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	kPacket.m_iItemUID			= kPacket_.m_iItemUID;
	kPacket.m_iSPoint 			= kPacket_.m_iSPoint;
	kPacket.m_iCSPoint 			= kPacket_.m_iCSPoint;
	kPacket.m_iDefaultSkillID1	= kPacket_.m_iDefaultSkillID1;
	kPacket.m_iDefaultSkillID2	= kPacket_.m_iDefaultSkillID2;
	kPacket.m_iDefaultSkillID3	= kPacket_.m_iDefaultSkillID3;
	kPacket.m_iDefaultSkillID4	= kPacket_.m_iDefaultSkillID4;
	kPacket.m_iDefaultSkillID5	= kPacket_.m_iDefaultSkillID5;
	kPacket.m_iDefaultSkillID6	= kPacket_.m_iDefaultSkillID6;
	kPacket.m_iBeforeSPoint		= kPacket_.m_iBeforeSPoint;
	kPacket.m_iBeforeCSPoint	= kPacket_.m_iBeforeCSPoint;


#ifdef SERV_SKILL_PAGE_SYSTEM
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_UPD_Reset", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iSPoint
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6
			% kPacket_.m_iActiveSkillPageNumber
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_All_DEL_20131212", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6
			% kPacket_.m_iActiveSkillPageNumber
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"DB 스킬초기화 실패.!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( kPacket_.m_iCSPoint )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID1 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID2 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID3 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID4 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID5 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID6 )
				<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_12;

			goto end_proc;
		}
	}
#else // SERV_SKILL_PAGE_SYSTEM
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_UPD_Reset", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iSPoint
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_All_DEL", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iSPoint
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"DB 스킬초기화 실패.!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOG( kPacket_.m_iSPoint )
				<< BUILD_LOG( kPacket_.m_iCSPoint )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID1 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID2 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID3 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID4 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID5 )
				<< BUILD_LOG( kPacket_.m_iDefaultSkillID6 )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_12;

			goto end_proc;
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM


	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INIT_SKILL_TREE_ACK, kPacket );
}

//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
_IMPL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_REQ, KDBE_INIT_SKILL_TREE_REQ )
{
	KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iSPoint = kPacket_.m_iSPoint;
	kPacket.m_iCSPoint = kPacket_.m_iCSPoint;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_SKILL_PAGE_SYSTEM
	{
		//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_UPD_Reset", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iSPoint
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6
			% kPacket_.m_iActiveSkillPageNumber
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_All_DEL_20131212", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6
			% kPacket_.m_iActiveSkillPageNumber
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}
	}
#else // SERV_SKILL_PAGE_SYSTEM
	{
		//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
		DO_QUERY( L"exec dbo.P_GSkill_New_All_DEL", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iSPoint
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6 );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_SKILL_PAGE_SYSTEM
	//kimhc // 김현철 // 위으 if에 의해 건너뛰어도 되도록
	kPacket.m_iOK = NetError::NET_OK;
#endif // SERV_SKILL_PAGE_SYSTEM

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ADMIN_CHANGE_UNIT_CLASS_REQ )
{
	KDBE_ADMIN_CHANGE_UNIT_CLASS_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_cUnitClass = kPacket_.m_cUnitClass;

	//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_UnitClassAdmin", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_cUnitClass)
		% kPacket_.m_iNewDefaultSkill1
		% kPacket_.m_iNewDefaultSkill2
		% kPacket_.m_iNewDefaultSkill3
		% kPacket_.m_iNewDefaultSkill4
		% kPacket_.m_iNewDefaultSkill5
		% kPacket_.m_iNewDefaultSkill6 );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_UnitClass_Admin", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% static_cast<int>(kPacket_.m_cUnitClass)
		% kPacket_.m_iNewDefaultSkill1
		% kPacket_.m_iNewDefaultSkill2
		% kPacket_.m_iNewDefaultSkill3
		% kPacket_.m_iNewDefaultSkill4
		% kPacket_.m_iNewDefaultSkill5
		% kPacket_.m_iNewDefaultSkill6 );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_CHANGE_UNIT_CLASS_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ADMIN_AUTO_GET_ALL_SKILL_REQ )
{
	KDBE_ADMIN_AUTO_GET_ALL_SKILL_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_cUnitClass = kPacket_.m_cUnitClass;
	kPacket.m_iSPoint = kPacket_.m_iSPoint;
	kPacket.m_iCSPoint = kPacket_.m_iCSPoint;
	kPacket.m_mapGetSkillList = kPacket_.m_mapGetSkillList;
	kPacket.m_vecUnsealedSkillID = kPacket_.m_vecUnsealedSkillID;
	kPacket.m_iNewDefaultSkill1 = kPacket_.m_iNewDefaultSkill1;
	kPacket.m_iNewDefaultSkill2 = kPacket_.m_iNewDefaultSkill2;
	kPacket.m_iNewDefaultSkill3 = kPacket_.m_iNewDefaultSkill3;
	kPacket.m_iNewDefaultSkill4 = kPacket_.m_iNewDefaultSkill4;
	kPacket.m_iNewDefaultSkill5 = kPacket_.m_iNewDefaultSkill5;
	kPacket.m_iNewDefaultSkill6 = kPacket_.m_iNewDefaultSkill6;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	std::map< int, int >::const_iterator mit;
	{
		//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnit_UPD_UnitClassAdmin", L"%d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% static_cast<int>(kPacket_.m_cUnitClass)
			% kPacket_.m_iNewDefaultSkill1
			% kPacket_.m_iNewDefaultSkill2
			% kPacket_.m_iNewDefaultSkill3
			% kPacket_.m_iNewDefaultSkill4
			% kPacket_.m_iNewDefaultSkill5
			% kPacket_.m_iNewDefaultSkill6 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnit_UPD_UnitClass_Admin", L"%d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% static_cast<int>(kPacket_.m_cUnitClass)
			% kPacket_.m_iNewDefaultSkill1
			% kPacket_.m_iNewDefaultSkill2
			% kPacket_.m_iNewDefaultSkill3
			% kPacket_.m_iNewDefaultSkill4
			% kPacket_.m_iNewDefaultSkill5
			% kPacket_.m_iNewDefaultSkill6 );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"치트키 스킬 초기화 실패" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_00;
			goto end_proc;
		}
	}

#ifdef SERV_SKILL_PAGE_SYSTEM

	mit = kPacket_.m_mapGetSkillList.begin();
	for( ; mit != kPacket_.m_mapGetSkillList.end() ; ++mit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% mit->first
			% mit->second
			% 0
			% kPacket_.m_iActiveSkillPageNumber );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_UPD", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% mit->first
			% mit->second
			% 0
			% kPacket_.m_iActiveSkillPageNumber );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to insert skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
				<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_00;

			goto end_proc;
		}
	}

#else // SERV_SKILL_PAGE_SYSTEM
	//std::map< int, int >::iterator 
    mit = kPacket_.m_mapGetSkillList.begin();
	for( ; mit != kPacket_.m_mapGetSkillList.end() ; ++mit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% mit->first
			% mit->second
			% 0
			% 0 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% mit->first
			% mit->second
			% 0 );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to insert skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_00;

			goto end_proc;
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM



	BOOST_TEST_FOREACH( short, iSkillID, kPacket_.m_vecUnsealedSkillID )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_Unsealed_INS", L"%d, %d", % kPacket_.m_iUnitUID % (int)iSkillID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_unsealed_skill", L"%d, %d", % kPacket_.m_iUnitUID % (int)iSkillID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to insert unsealed skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( iSkillID )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_00;

			goto end_proc;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_AUTO_GET_ALL_SKILL_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ADMIN_GET_SKILL_REQ )
{
	KDBE_ADMIN_GET_SKILL_ACK kPacket;
	kPacket.m_iOK						= NetError::ERR_ODBC_00;
	kPacket.m_iSkillID					= kPacket_.m_iSkillID;
	kPacket.m_iSkillLevel				= kPacket_.m_iSkillLevel;
	kPacket.m_iCSPoint					= kPacket_.m_iCSPoint;
	kPacket.m_bUnsealed					= kPacket_.m_bUnsealed;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	// 스킬 획득 정보를 갱신
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d %d",
				% kPacket_.m_iUnitUID 
				% kPacket_.m_iSkillID
				% kPacket_.m_iSkillLevel
				% kPacket_.m_iCSPoint
				% kPacket_.m_iActiveSkillPageNumber );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_New_UPD", L"%d, %d, %d, %d %d",
				% kPacket_.m_iUnitUID 
				% kPacket_.m_iSkillID
				% kPacket_.m_iSkillLevel
				% kPacket_.m_iCSPoint
				% kPacket_.m_iActiveSkillPageNumber );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"failed to insert skill!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSkillID )
					<< BUILD_LOG( kPacket.m_iActiveSkillPageNumber )
					<< BUILD_LOG( kPacket.m_iOK )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_SKILL_00;

				goto end_proc;
			}
		}
#else // SERV_SKILL_PAGE_SYSTEM
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d %d",
				% kPacket_.m_iUnitUID 
				% kPacket_.m_iSkillID
				% kPacket_.m_iSkillLevel
				% kPacket_.m_iCSPoint
				% 0 );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
				% kPacket_.m_iUnitUID 
				% kPacket_.m_iSkillID
				% kPacket_.m_iSkillLevel
				% kPacket_.m_iCSPoint );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"failed to insert skill!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSkillID )
					<< BUILD_LOG( kPacket.m_iOK )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_SKILL_00;

				goto end_proc;
			}
		}
#endif // SERV_SKILL_PAGE_SYSTEM


		if( kPacket_.m_bUnsealed == true )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_Unsealed_INS", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iSkillID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_insert_unsealed_skill", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iSkillID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"failed to insert unsealed skill!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSkillID )
					<< BUILD_LOG( kPacket.m_iOK )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_SKILL_00;

				goto end_proc;
			}
		}
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	//kimhc // 김현철 // 위으 if에 의해 건너뛰어도 되도록
	kPacket.m_iOK = NetError::NET_OK;
#endif // SERV_SKILL_PAGE_SYSTEM

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_GET_SKILL_ACK, kPacket );
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC( DBE_INSERT_SKILL_REQ )
{
	KDBE_INSERT_SKILL_ACK kPacket;
	kPacket.m_iOK				= NetError::ERR_ODBC_01;
	kPacket.m_iSkillID			= kPacket_.m_iSkillID;
	kPacket.m_iSkillLevel		= kPacket_.m_iSkillLevel;
	kPacket.m_iSkillCSPoint		= kPacket_.m_iSkillCSPoint;
	kPacket.m_iCSPoint			= kPacket_.m_iCSPoint;

	// 캐시 스킬 포인트 정보를 갱신
	if( kPacket_.m_iCSPoint >= 0 )
	{
		DO_QUERY( L"exec dbo.gup_update_cash_skill_point_info", L"%d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}	

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to update cash skill point when insert skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iCSPoint )
				<< BUILD_LOG( kPacket_.m_iSkillID )
				<< BUILD_LOG( kPacket_.m_iSkillLevel )
				<< BUILD_LOG( kPacket_.m_iSkillCSPoint )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_24;

			goto end_proc;	
		}
	}

	// 스킬 획득 정보를 갱신
	DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iSkillID
		% kPacket_.m_iSkillLevel
		% kPacket_.m_iSkillCSPoint );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}	

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"failed to insert skill!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< BUILD_LOG( kPacket_.m_iSkillLevel )
			<< BUILD_LOG( kPacket_.m_iSkillCSPoint )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_00;

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_SKILL_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INIT_SKILL_TREE_REQ )
{
	KDBE_INIT_SKILL_TREE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iItemUID			= kPacket_.m_iItemUID;
	kPacket.m_iSPoint 			= kPacket_.m_iSPoint;
	kPacket.m_iCSPoint 			= kPacket_.m_iCSPoint;
	kPacket.m_iDefaultSkillID	= kPacket_.m_iDefaultSkillID;
	kPacket.m_iDefaultSkillID2	= kPacket_.m_iDefaultSkillID2;

	DO_QUERY( L"exec dbo.gup_delete_all_skill_new", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSPoint
		% kPacket_.m_iCSPoint
		% kPacket_.m_iDefaultSkillID
		% kPacket_.m_iDefaultSkillID2
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"DB 스킬초기화 실패.!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iSPoint )
			<< BUILD_LOG( kPacket_.m_iCSPoint )
			<< BUILD_LOG( kPacket_.m_iDefaultSkillID )
			<< BUILD_LOG( kPacket_.m_iDefaultSkillID2 )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_12;

		goto end_proc;
	}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INIT_SKILL_TREE_ACK, kPacket );
}

//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_FIX_THE_ADMIN_CHEAT
	_IMPL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_REQ, KDBE_INIT_SKILL_TREE_REQ )
#else
	_IMPL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_REQ, UidType )
#endif SERV_FIX_THE_ADMIN_CHEAT
//}}
{
	KEGS_ADMIN_INIT_SKILL_TREE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_FIX_THE_ADMIN_CHEAT
	DO_QUERY( L"exec dbo.gup_delete_all_skill_new", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSPoint
		% kPacket_.m_iCSPoint
		% kPacket_.m_iDefaultSkillID
		% kPacket_.m_iDefaultSkillID2 );
#else
	DO_QUERY( L"exec dbo.gup_delete_all_skill_new", L"%d, %d, %d, %d, %d",
		% kPacket_
		%1000
		%0
		%0
		%0 );
#endif SERV_FIX_THE_ADMIN_CHEAT
	//}}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_INIT_SKILL_TREE_ACK, kPacket );
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013


_IMPL_ON_FUNC( DBE_SEARCH_UNIT_REQ, std::wstring )
{
    KPacketOK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
    UidType iUnitUID = 0;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iUnitUID );
        m_kODBC.EndFetch();
    }

    if( iUnitUID != 0 )
    {
        kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_03;
    }
    else
    {
        kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_04;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_SEARCH_UNIT_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_KNM_REQUEST_NEW_FRIEND_INFO_REQ, std::wstring )
{	
	KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK kPacket;
	kPacket.m_iOK				= NetError::ERR_ODBC_01;
	kPacket.m_wstrUnitNickName	= kPacket_;

	KDenyOptions kDeny;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	UidType iUnitUID = 0;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iUnitUID );
		m_kODBC.EndFetch();
	}

	if( iUnitUID != 0 )
	{
		kPacket.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_KNM_02;

		goto end_proc;
	}

	//{{ 2009. 4. 1  최육사		친구 초대 차단 옵션을 찾아보자	
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	DO_QUERY( L"exec dbo.P_GDenyOption_SEL", L"%d", % iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kDeny.m_cDenyFriendShip
			>> kDeny.m_cDenyInviteGuild
			>> kDeny.m_cDenyParty
			>> kDeny.m_cDenyPersonalTrade
			>> kDeny.m_cDenyRequestCouple
            >> kDeny.m_cDenyInvitePracticePVP
			);

		m_kODBC.EndFetch();

		// 친구 초대 거부 옵션
		if( kDeny.m_cDenyFriendShip == KDenyOptions::DOS_ON )
		{
			kPacket.m_iOK = NetError::ERR_KNM_06;
			goto end_proc;
		}

		kPacket.m_iOK = NetError::NET_OK;
	}
#else
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDenyOption_SEL", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_community_opt_unituid", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kDeny.m_cDenyFriendShip
			>> kDeny.m_cDenyInviteGuild
			>> kDeny.m_cDenyParty
			>> kDeny.m_cDenyPersonalTrade
			);

		m_kODBC.EndFetch();

		// 친구 초대 거부 옵션
		if( kDeny.m_cDenyFriendShip == KDenyOptions::DOS_ON )
		{
			kPacket.m_iOK = NetError::ERR_KNM_06;
			goto end_proc;
		}

		kPacket.m_iOK = NetError::NET_OK;
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	else
	{
		kPacket.m_iOK = NetError::ERR_COMMUNITY_OPT_02;
		goto end_proc;
	}
	//}}

	// 넥슨 메신저용 CharacterSN.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MessengerSN_SEL_ByUnitUID", L"%d", % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_MessengerSN", L"%d", % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		int iTemp = 0;
		FETCH_DATA( iTemp );

		kPacket.m_uiKNMSerialNum = static_cast<u_int>(iTemp);

		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_KNM_02;
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket );
}


IMPL_ON_FUNC( DBE_INSERT_TRADE_ITEM_REQ )
{
	KDBE_INSERT_TRADE_ITEM_ACK kPacket;
	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_TRADE, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;		
	}

	SendToUser( LAST_SENDER_UID, DBE_INSERT_TRADE_ITEM_ACK, kPacket );
}

//{{ 2009. 2. 10  최육사	개인거래 버그 예방
IMPL_ON_FUNC( DBE_INSERT_TRADE_ITEM_BY_SERVER_NOT )
{
	std::map< UidType, KItemInfo > mapDummy;
	KItemQuantityUpdate kDummy;
	std::map< UidType, int >::const_iterator mit;
	std::vector< KDeletedItemInfo >::const_iterator vit;
	std::vector< KItemInfo >::const_iterator vitIT;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kDummy.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kDummy.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_TRADE_EXCEPTION, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, mapDummy ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, mapDummy ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_GamePointAdmin", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iED );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_admin_update_unit_gamepoint", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iED );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"개인거래중 종료로 인한 서버 강제 ED업데이트 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iED )
			<< END_LOG;
	}

	//////////////////////////////////////////////////////////////////////////
	START_LOG( cout, L"개인거래 또는 개인상점중 종료로 인한 서버 강제 거래아이템 업데이트!" )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iED );

	mit = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
	for( ; mit != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mit )
	{
		START_LOG( cout, L"수량 변동" )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second );
	}

	vit = kPacket_.m_kItemQuantityUpdate.m_vecDeleted.begin();
	for( ; vit != kPacket_.m_kItemQuantityUpdate.m_vecDeleted.end(); ++vit )
	{
		START_LOG( cout, L"아이템 삭제" )
			<< BUILD_LOG( vit->m_iItemUID );
	}
    
	vitIT = kPacket_.m_vecItemInfo.begin();
	for( ; vitIT != kPacket_.m_vecItemInfo.end(); ++vitIT )
	{
		START_LOG( cout, L"아이템 생성" )
			<< BUILD_LOG( vitIT->m_iItemID )
			<< BUILD_LOGc( vitIT->m_cUsageType )
			<< BUILD_LOG( vitIT->m_iQuantity )
			<< BUILD_LOG( vitIT->m_sEndurance )
			<< BUILD_LOGc( vitIT->m_cEnchantLevel )
			<< BUILD_LOGc( vitIT->m_kAttribEnchantInfo.m_cAttribEnchant0 )
			<< BUILD_LOGc( vitIT->m_kAttribEnchantInfo.m_cAttribEnchant1 )
			<< BUILD_LOG( vitIT->m_sPeriod )
			<< BUILD_LOG( vitIT->m_wstrExpirationDate );

		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::vector< int >::const_iterator vitS = vitIT->m_vecItemSocket.begin();
#else
		std::vector< short >::const_iterator vitS = vitIT->m_vecItemSocket.begin();
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		for( ; vitS != vitIT->m_vecItemSocket.begin(); ++vitS )
		{
			START_LOG( cout, L"아이템 생성 : 소켓" )
				<< BUILD_LOG( *vitS );
		}
	}
	//////////////////////////////////////////////////////////////////////////

end_proc:
	return;
}
//}}

_IMPL_ON_FUNC( DBE_INSERT_PURCHASED_CASH_ITEM_REQ, KDBE_INSERT_ITEM_REQ )
{
    KDBE_INSERT_ITEM_ACK kPacket;
    kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
    kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
    LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
    LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
    Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_BUY_CASH_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	//{{ 2009. 9. 2  최육사		밀봉
	Query_UpdateSealItem( kPacket_.m_setSealCashItem, kPacket.m_mapItemInfo );
	//}}

    SendToUser( LAST_SENDER_UID, DBE_INSERT_PURCHASED_CASH_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_PURCHASED_CASH_PACKAGE_REQ )
{
	KDBE_INSERT_PURCHASED_CASH_PACKAGE_ACK kPacket;
	kPacket.m_ulProductNo = kPacket_.m_ulProductNo;
	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_BUY_CASH_PACKAGE, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	//{{ 2009. 9. 2  최육사		밀봉
	Query_UpdateSealItem( kPacket_.m_setSealCashItem, kPacket.m_mapItemInfo );
	//}}

	SendToUser( LAST_SENDER_UID, DBE_INSERT_PURCHASED_CASH_PACKAGE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_OPEN_RANDOM_ITEM_REQ )
{
	KDBE_OPEN_RANDOM_ITEM_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05	
	// 큐브 열고 나온 결과물이 밀봉 되도록 수정
	if( Query_InsertItemList( SEnum::GIR_RANDOM_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo, false ) == false )
#else
	if( Query_InsertItemList( SEnum::GIR_RANDOM_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}	
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"랜덤 큐브 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RANDOM_ITEM_06;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;

		//{{ 2010. 7. 26  최육사	밀봉 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
		Query_UpdateSealItem( kPacket_.m_setSealRandomItem, kPacket.m_mapItemInfo );
#endif SERV_SEALED_RANDOM_ITEM
		//}}
	}

	//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	kPacket.m_iNumResurrectionStone	= kPacket_.m_iNumResurrectionStone;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	//}}

	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	kPacket.m_iSpendED = kPacket_.m_iSpendED;
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}

	SendToUser( LAST_SENDER_UID, DBE_OPEN_RANDOM_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ITEM_MANUFACTURE_REQ )
{
	KDBE_ITEM_MANUFACTURE_ACK kPacket;
    kPacket.m_iOK                       = NetError::ERR_ODBC_01;
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
    LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
    Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_MANUFACTURE, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_MANUFACTURE_11;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
		//{{ 2012. 02. 07	박세훈	아이템 툴 작업 ( g_pManufactureItemManager:AddManufactureResultGroupWithRate 에 초기 봉인 상태 인자 추가 )
#ifdef SERV_ADD_SEALED_ITEM_SIGN

		std::set< int > setSealItem;

		for( size_t i=0; i < kPacket_.m_vecItemInfo.size(); ++i )
		{
			if( kPacket_.m_vecItemInfo[i].IsSealedItem() == true )   // 봉인 아이템이라는 것
			{
				setSealItem.insert( kPacket_.m_vecItemInfo[i].m_iItemID );
			}
		}
		Query_UpdateSealItem( setSealItem, kPacket.m_mapItemInfo );

#endif SERV_ADD_SEALED_ITEM_SIGN
		//}}
	}

	SendToUser( LAST_SENDER_UID, DBE_ITEM_MANUFACTURE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_RESOLVE_ITEM_REQ )
{
	KDBE_RESOLVE_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_bJackpot					= kPacket_.m_bJackpot;
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_RESOLVE_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RESOLVE_ITEM_05;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	SendToUser( LAST_SENDER_UID, DBE_RESOLVE_ITEM_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_CHANGE_NICK_NAME_REQ, KEGS_CHANGE_NICK_NAME_REQ )
{
    KEGS_CHANGE_NICK_NAME_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_bCheckOnly = kPacket_.m_bCheckOnly;


#ifdef SERV_ALL_RENEWAL_SP
	int iNickKeepDay = 14;	// 기본 국내
#if defined( SERV_COUNTRY_TWHK )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_JP )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_EU )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_US )
	iNickKeepDay = 7;
#elif defined( SERV_COUNTRY_CN )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_TH )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_ID )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_BR )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_PH )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_IN )
	iNickKeepDay = 14;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

	// 사용가능한 닉네임인지 체크만 하기
	if( kPacket_.m_bCheckOnly )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GDeletedNickNameHistory_SEL", L"N\'%s\', %d", % kPacket_.m_wstrNickName % iNickKeepDay );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_check_nickname", L"N\'%s\'", % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
			goto end_proc;
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			switch( kPacket.m_iOK )
			{
			case -1:
				kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_02;
				break;
			case -2:
				kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_03;
				break;			

			default:
				{
					START_LOG( cerr, L"정의되지 않은 에러! sp가 바꼈나?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_wstrNickName )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
				}
			}

			goto end_proc;
		}
	}
	// 실제로 닉네임 변경하기
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnitNickName_UPD_Restore", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrNickName );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_restore_nickname", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}
		else
		{
			kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
			goto end_proc;
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			switch( kPacket.m_iOK )
			{
			case -1:
				kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_00;
				break;
			case -2:
				kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_01;
				break;
			case -3:
				kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_02;
				break;
			case -4:
				kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_03;
				break;
			case -5:
				kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
				break;

			default:
				{
					START_LOG( cerr, L"정의되지 않은 에러! sp가 바꼈나?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_wstrNickName )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_04;
				}
			}

			goto end_proc;
		}
		else
		{
			kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
		}
	}    

end_proc:
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
        << BUILD_LOG( kPacket.m_iOK )
        << BUILD_LOG( m_kODBC.GetLastQuery() )
        << END_LOG;

    SendToUser( LAST_SENDER_UID, DBE_CHANGE_NICK_NAME_ACK, kPacket );
}

//{{ 2007. 8. 9  최육사  블랙리스트 패킷 핸들러 - GSGameDBThread
IMPL_ON_FUNC( DBE_NEW_BLACKLIST_USER_REQ )
{
	KEGS_NEW_BLACKLIST_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kChatBlackListUnit.m_iUnitUID = kPacket_.m_iBlackListUnitUID;

	// 블랙리스트 유저 저장
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBlackList_INS", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iBlackListUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_blacklist", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iBlackListUnitUID );
#endif //SERV_ALL_RENEWAL_SP	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
				 >> kPacket.m_kChatBlackListUnit.m_wstrNickName;
			);
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_BLACKLIST_03;   break;
		case -2:    kPacket.m_iOK = NetError::ERR_BLACKLIST_01;   break;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_NEW_BLACKLIST_USER_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DEL_BLACKLIST_USER_REQ )
{
	KDBE_DEL_BLACKLIST_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iBlackListUnitUID = kPacket_.m_iBlackListUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBlackList_DEL", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iBlackListUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_blacklist", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iBlackListUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_BLACKLIST_02;   break;
		case -2:    kPacket.m_iOK = NetError::ERR_BLACKLIST_04;   break;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_DEL_BLACKLIST_USER_ACK, kPacket );
}
//}}

//{{ 2009. 9. 22  최육사	전직캐쉬
IMPL_ON_FUNC( DBE_CHANGE_UNIT_CLASS_REQ )
{
	KDBE_CHANGE_UNIT_CLASS_ACK kPacket;
	kPacket.m_iOK		 = NetError::ERR_ODBC_01;
	kPacket.m_usEventID  = kPacket_.m_usEventID;
	kPacket.m_cUnitClass = kPacket_.m_cUnitClass;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iNewDefaultSkill1					= kPacket_.m_iNewDefaultSkill1;
	kPacket.m_iNewDefaultSkill2					= kPacket_.m_iNewDefaultSkill2;
	kPacket.m_iTheNumberOfSkillPagesAvailable	= kPacket_.m_iTheNumberOfSkillPagesAvailable;

	for ( int iSkillPagesNumber = 1; iSkillPagesNumber <= kPacket_.m_iTheNumberOfSkillPagesAvailable; iSkillPagesNumber++ )
	{
		DO_QUERY( L"exec P_GUnit_UPD_UnitClass_20131212", L"%d, %d, %d, %d, %d", 
			% kPacket_.m_iUnitUID 
			% (int)kPacket_.m_cUnitClass 
			% kPacket_.m_iNewDefaultSkill1 
			% kPacket_.m_iNewDefaultSkill2 
			% iSkillPagesNumber );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK )
		{
			switch( kPacket.m_iOK )
			{
			case -1:    kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;   break;
			case -2:    kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_05;   break;
			}
		}
	}
	
#else // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	kPacket.m_iNewDefaultSkill1 = kPacket_.m_iNewDefaultSkill1;
	kPacket.m_iNewDefaultSkill2 = kPacket_.m_iNewDefaultSkill2;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec P_GUnit_UPD_UnitClass_New", L"%d, %d, %d, %d", % kPacket_.m_iUnitUID % (int)kPacket_.m_cUnitClass % kPacket_.m_iNewDefaultSkill1 % kPacket_.m_iNewDefaultSkill2 );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec P_GUnit_UPD_UnitClass", L"%d, %d, %d, %d", % kPacket_.m_iUnitUID % (int)kPacket_.m_cUnitClass % kPacket_.m_iNewDefaultSkill1 % kPacket_.m_iNewDefaultSkill2 );
#endif //SERV_ALL_RENEWAL_SP
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	DO_QUERY( L"exec gup_update_unit_class", L"%d, %d", % kPacket_.m_iUnitUID % (int)kPacket_.m_cUnitClass );
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;   break;
		case -2:    kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_05;   break;
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM


end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_UNIT_CLASS_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	KDBE_INSERT_BUY_PERSONAL_SHOP_ITEM_ACK kPacket;
	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );

// 	START_LOG( cerr, L"삭제 누가 호출 하나" )
// 		<< BUILD_LOG( kPacket_.m_iUnitUID )
// 		<< END_LOG;

	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_PERSONAL_SHOP, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ENCHANT_ITEM_REQ )
{
    KDBE_ENCHANT_ITEM_ACK kPacket;
    kPacket.m_iEnchantResult		  = kPacket_.m_iEnchantResult;
    kPacket.m_iItemUID				  = kPacket_.m_iItemUID;
	kPacket.m_iItemID				  = kPacket_.m_iItemID;
    kPacket.m_iLevelAfterEnchant	  = kPacket_.m_iLevelAfterEnchant;
    kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec P_GItemEnchant_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iItemUID % kPacket_.m_iLevelAfterEnchant );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec gup_update_Enchant", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iItemUID % kPacket_.m_iLevelAfterEnchant );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		std::wstring wstrEnchantResult = NetError::GetErrStr( kPacket_.m_iEnchantResult );

		START_LOG( cerr, L"강화 레벨 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iLevelAfterEnchant )
			<< BUILD_LOG( wstrEnchantResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ENCHANT_ITEM_13;
		goto end_proc;
	}

	bool bUpdateFailed = false;
    LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_ENCHANT_ITEM_ACK, kPacket );
}

//{{ 2008. 12. 21  최육사	강화 복구
IMPL_ON_FUNC( DBE_RESTORE_ITEM_REQ )
{
	KDBE_RESTORE_ITEM_ACK kPacket;
	kPacket.m_iItemUID				  = kPacket_.m_iItemUID;
	kPacket.m_iLevelAfterEnchant	  = kPacket_.m_iLevelAfterEnchant;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec P_GItemEnchant_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iItemUID % kPacket_.m_iLevelAfterEnchant );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec gup_update_Enchant", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iItemUID % kPacket_.m_iLevelAfterEnchant );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"사용불가 아이템 복구 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iLevelAfterEnchant )	
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ENCHANT_ITEM_13;
		goto end_proc;
	}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RESTORE_ITEM_ACK, kPacket );
}
//}}

//{{ 2008. 3. 6  최육사  소켓
IMPL_ON_FUNC( DBE_SOCKET_ITEM_REQ )
{
	KDBE_SOCKET_ITEM_ACK kPacket;
	kPacket.m_vecSocketInfo = kPacket_.m_vecSocketInfo;
	//{{ 2010. 04. 15  최육사	소켓 개편
#ifdef SERV_SOCKET_NEW
#else
	kPacket.m_iSocketResult = kPacket_.m_iSocketResult;
	kPacket.m_iItemID		= kPacket_.m_iItemID;
#endif SERV_SOCKET_NEW
	//}}

#ifdef SERV_ADD_TITLE_CONDITION_2013_08		// 적용날짜: 2013-08-13
	kPacket.m_iItemID		= kPacket_.m_iItemID;
#endif // SERV_ADD_TITLE_CONDITION_2013_08

	kPacket.m_iItemUID		= kPacket_.m_iItemUID;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	kPacket.m_iSocketUseCount = kPacket_.m_iSocketUseCount;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	const byte byteArraySize = 5;
	int arrSocketInfo[byteArraySize]; // DB 테이블의 소켓테이블 참고
	memset( arrSocketInfo, 0, sizeof(int) * byteArraySize );

	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit = kPacket.m_vecSocketInfo.begin();
#else
	std::vector< short >::const_iterator vit = kPacket.m_vecSocketInfo.begin();
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	for( int iIdx = 0; iIdx < byteArraySize; ++iIdx )
	{		
		if( vit == kPacket.m_vecSocketInfo.end() )
			continue;

		arrSocketInfo[iIdx] = *vit;
		++vit;
	}
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_MER", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID		// @iUnitUID bigint
		% kPacket_.m_iItemUID		// @iItemUID bigint
		% arrSocketInfo[0]			// @iSoket1 smallint
		% arrSocketInfo[1]			// @iSoket2 smallint
		% arrSocketInfo[2]			// @iSoket3 smallint
		% arrSocketInfo[3]			// @iSoket4 smallint
		% arrSocketInfo[4]			// @iSoket5 smallint
		% kPacket_.m_byteExpandedSocketNum
			);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_SET", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID		// @iUnitUID bigint
		% kPacket_.m_iItemUID		// @iItemUID bigint
		% arrSocketInfo[0]			// @iSoket1 smallint
		% arrSocketInfo[1]			// @iSoket2 smallint
		% arrSocketInfo[2]			// @iSoket3 smallint
		% arrSocketInfo[3]			// @iSoket4 smallint
		% arrSocketInfo[4]			// @iSoket5 smallint
		% kPacket_.m_byteExpandedSocketNum
		);
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_BATTLE_FIELD_BOSS
	int arrSocketInfo[4] = {0,0,0,0}; // DB 테이블의 소켓테이블 참고
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit = kPacket.m_vecSocketInfo.begin();
#else
	std::vector< short >::const_iterator vit = kPacket.m_vecSocketInfo.begin();
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	for( int iIdx = 0; iIdx < 4; iIdx++ )
	{		
		if( vit == kPacket.m_vecSocketInfo.end() )
			continue;

		arrSocketInfo[iIdx] = *vit;
		++vit;
	}

	DO_QUERY( L"exec dbo.gup_update_Socket", L"%d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID		// @iUnitUID bigint
		% kPacket_.m_iItemUID		// @iItemUID bigint
		% arrSocketInfo[0]			// @iSoket1 smallint
		% arrSocketInfo[1]			// @iSoket2 smallint
		% arrSocketInfo[2]			// @iSoket3 smallint
		% arrSocketInfo[3]			// @iSoket4 smallint
		);
#endif // SERV_BATTLE_FIELD_BOSS

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( arrSocketInfo[0] )
			<< BUILD_LOG( arrSocketInfo[1] )
			<< BUILD_LOG( arrSocketInfo[2] )
			<< BUILD_LOG( arrSocketInfo[3] )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
			<< BUILD_LOG( arrSocketInfo[4] )
#endif // SERV_BATTLE_FIELD_BOSS
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SOCKET_ITEM_10;
	}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( LAST_SENDER_UID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_SOCKET_ITEM_ACK, kPacket );	
}
//}}

//{{ 2008. 3. 28  최육사  추천 / 사제 보상
IMPL_ON_FUNC( DBE_RECOMMEND_USER_REQ )
{
	KDBE_RECOMMEND_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iRecommendedUnitUID = 0;

	//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
	// 보상
	DO_QUERY( L"exec dbo.P_GRecommend_INS", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iRecommendedUnitUID );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_00;   break;
		case -2:    kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_01;   break;
		case -3:	kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_07;   break;		// 더 이상 추천 할 수 없는 유저입니다.
			//}}
		default:
			{
				START_LOG( cerr, L"추천인 등록이 실패하였습니다!" )
					<< BUILD_LOG( kPacket.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_ODBC_01;
			}
		}
	}
#else
	// 보상
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GRecommend_INS", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_recommend", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iRecommendedUnitUID );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_00;   break;
			//{{ 2008. 5. 16  최육사  체험ID 제한
		case -3:	kPacket.m_iOK = NetError::ERR_GUEST_USER_01;	   break;
			//}}
		default:
			{
				START_LOG( cerr, L"추천인 등록이 실패하였습니다!" )
					<< BUILD_LOG( kPacket.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_ODBC_01;
			}
		}
	}
#endif SERV_RECOMMEND_LIST_EVENT
	//}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RECOMMEND_USER_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_REQ )
{
	KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
	kPacket.m_iOK					  	   = NetError::ERR_ODBC_01;
	kPacket.m_iRewardLetter.m_iFromUnitUID = kPacket_.m_iFromUnitUID;
	kPacket.m_iRewardLetter.m_iToUnitUID   = kPacket_.m_iToUnitUID;
	kPacket.m_iRewardLetter.m_cScriptType  = kPacket_.m_iRewardType;
	kPacket.m_iRewardLetter.m_iScriptIndex = kPacket_.m_iRewardID;
	kPacket.m_iRewardLetter.m_iQuantity	   = kPacket_.m_sQuantity;
	kPacket.m_iRewardLetter.m_wstrMessage  = kPacket_.m_wstrMessage;

	// 보상
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_sQuantity
		% kPacket_.m_iRewardType
		% kPacket_.m_iRewardID
		% kPacket_.m_wstrMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_sQuantity
		% kPacket_.m_iRewardType
		% kPacket_.m_iRewardID
		% kPacket_.m_wstrMessage
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iRewardLetter.m_iPostNo
			>> kPacket.m_iRewardLetter.m_wstrRegDate );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_REWARD_TO_POST_00;   break;
		}

		goto end_proc;
	}	

	//{{ 2010. 02. 24  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

	// 웹 포인트 이벤트 보상 우편 로그
	if( kPacket_.m_iRewardType == KPostItemInfo::LT_WEB_POINT_EVENT )
	{
		int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GEvent_WebPost_INS", L"%d, %d", % kPacket_.m_iToUnitUID % kPacket.m_iRewardLetter.m_iPostNo );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_event_webpost", L"%d, %d", % kPacket_.m_iToUnitUID % kPacket.m_iRewardLetter.m_iPostNo );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"웹 포인트 이벤트 보상 우편 로그 쿼리 실패" )				
				<< BUILD_LOG( kPacket_.m_iToUnitUID )
				<< BUILD_LOG( kPacket.m_iRewardLetter.m_iPostNo )
				<< END_LOG;
		}
	}

#endif SERV_WEB_POINT_EVENT
	//}}

	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	kPacket.m_iDBIndex = kPacket_.m_iDBIndex;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

end_proc:
	if( kPacket_.m_bGameServerEvent )
	{
		SendToServer( DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
	}
	else
	{
		SendToUser( LAST_SENDER_UID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
	}
}
//}}

//{{ 2008. 9. 18  최육사	편지 쓰기
IMPL_ON_FUNC( DBE_PREPARE_INSERT_LETTER_TO_POST_REQ )
{
	KDBE_PREPARE_INSERT_LETTER_TO_POST_ACK kPacket;
	kPacket.m_iOK			  = NetError::ERR_ODBC_01;

	//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	// ED 동기화
	int iOK = NetError::ERR_ODBC_00;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_GamePointPost", L"%d, %d", % kPacket_.m_iFromUnitUID % kPacket_.m_iIncrementED );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPT_POST_GP", L"%d, %d", % kPacket_.m_iFromUnitUID % kPacket_.m_iIncrementED );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ED 동기화 실패!!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iFromUnitUID )
			<< BUILD_LOG( kPacket_.m_iIncrementED )
			<< BUILD_LOG( iOK )
			<< END_LOG;

		// 실패 했다면 ED를 원래대로 돌려놔야 한다.
		kPacket.m_iIncrementED = kPacket_.m_iIncrementED;
	}

	// 아이템 동기화
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iFromUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iFromUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}

	if( ( kPacket.m_iIncrementED != 0 ) ||
		( kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.empty() != true ) ||
		( kPacket.m_kItemQuantityUpdate.m_vecDeleted.empty() != true ) )
	{
		// 작업 전 동기화 수행 실패!!
		kPacket.m_iOK = NetError::ERR_POST_LETTER_03;
		goto end_proc;
	}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}

	kPacket.m_iItemUID		  = kPacket_.m_iItemUID;
	kPacket.m_wstrToNickName  = kPacket_.m_wstrToNickName;
	kPacket.m_kSendLetter	  = kPacket_.m_kSendLetter;
	kPacket.m_iSendLetterCost = kPacket_.m_iSendLetterCost;

	//////////////////////////////////////////////////////////////////////////
	// 우편에 첨부 넣기 전에 수량 체크	
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_SEL_Check", L"%d, N\'%s\'", % kPacket_.m_iFromUnitUID % kPacket_.m_wstrToNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_check_insert_post_item", L"%d, N\'%s\'", % kPacket_.m_iFromUnitUID % kPacket_.m_wstrToNickName );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -99: kPacket.m_iOK = NetError::ERR_POST_LETTER_10; break;
		case -98: kPacket.m_iOK = NetError::ERR_POST_LETTER_13; break;
		case -97: kPacket.m_iOK = NetError::ERR_POST_LETTER_15; break;
		case -96: kPacket.m_iOK = NetError::ERR_POST_LETTER_17; break;
			//{{ 2012. 09. 26	최육사		우편 복사 버그 수정
#ifdef SERV_POST_COPY_BUG_FIX
		case -100: kPacket.m_iOK = NetError::ERR_POST_LETTER_20; break;
#endif SERV_POST_COPY_BUG_FIX
			//}}

		default:
			START_LOG( cerr, L"정의되지 않은 에러~!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iFromUnitUID )
				<< BUILD_LOG( kPacket_.m_wstrToNickName )
				<< END_LOG;
			break;
		}
	}
#ifdef	SERV_SHARING_BANK_TEST
	else
	{
		if( kPacket_.m_iItemUID > 0 )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GItem_SEL_Check", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_iFromUnitUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GItem_Check", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_iFromUnitUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK  );
				m_kODBC.EndFetch();
			}
		}

		if(kPacket.m_iOK == -1)
			kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
	}
#endif	SERV_SHARING_BANK_TEST

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_PREPARE_INSERT_LETTER_TO_POST_ACK, kPacket );
}

//{{ 2012. 08. 20	박세훈	우편 로직 변경
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( DBE_INSERT_LETTER_TO_POST_REQ )
{
	KDBE_INSERT_LETTER_TO_POST_ACK kPacket;
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit;
#else
	std::vector< short >::const_iterator vit;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	kPacket.m_iOK					  = NetError::ERR_ODBC_01;
	kPacket.m_wstrToNickName		  = kPacket_.m_wstrToNickName;	
	kPacket.m_kSendLetter			  = kPacket_.m_kSendLetter;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iSendLetterCost		  = kPacket_.m_iSendLetterCost;
	
	if( kPacket_.m_kSendLetter.IsSystemLetter() != true )
	{
		const byte	iTradeType		= 0;	// 우편 보내기
		int			iED				= -kPacket_.m_iSendLetterCost;
		UidType		iItemUID		= 0;
		int			iQuantity		= 0;
		byte		iPostItemType	= 3;	// 첨부 없음
		byte		iUsageType		= static_cast<byte>( CXSLItem::PT_INFINITY );

		if( 0 < kPacket_.m_kSendLetter.m_iScriptIndex )
		{
			if( kPacket_.m_kSendLetter.m_iScriptIndex == CXSLItem::EDI_BRONZE_ED )
			{
				iPostItemType	= 2;	// ED 첨부
				iED				-= kPacket_.m_kSendLetter.m_iQuantity;
				iQuantity		= kPacket_.m_kSendLetter.m_iQuantity;
			}
			else
			{
				iPostItemType = 1;	// 아이템 첨부
				iItemUID  = kPacket_.m_kSendLetter.m_iItemUID;
				iQuantity = kPacket_.m_kSendLetter.m_iQuantity;
				iUsageType= kPacket_.m_kSendLetter.m_iUsageType;
			}
		}
		DO_QUERY( L"exec dbo.P_GItem_Total_Post",
			L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, N\'%s\', N\'%s\', %d",
			% iTradeType
			% kPacket_.m_iFromUnitUID
			% kPacket_.m_wstrToNickName
			% iED
			% iItemUID
			% iQuantity
			% iUsageType
			% iPostItemType
			% kPacket_.m_kSendLetter.m_wstrTitle
			% kPacket_.m_kSendLetter.m_wstrMessage
			% kPacket_.m_kSendLetter.m_iPostNo
			);

		UidType iGarbageValue;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_kSendLetter.m_iPostNo
				>> kPacket.m_kSendLetter.m_iToUnitUID
				>> kPacket.m_kSendLetter.m_wstrRegDate
				>> iGarbageValue
				);
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"편지 쓰기 실패!! 절대 일어나서는 안됨!!!!! 복구 이슈 발생!")
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( iTradeType )
				<< BUILD_LOG( kPacket_.m_iFromUnitUID )
				<< BUILD_LOG( kPacket_.m_wstrToNickName )
				<< BUILD_LOG( iED )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iQuantity )
				<< BUILD_LOG( iUsageType )
				<< BUILD_LOG( iPostItemType )
				<< BUILD_LOG( kPacket_.m_kSendLetter.m_wstrTitle )
				<< BUILD_LOG( kPacket_.m_kSendLetter.m_wstrMessage )
				<< BUILD_LOG( kPacket_.m_kSendLetter.m_iPostNo )
				<< END_LOG;

			kPacket.m_kItemQuantityUpdate = kPacket_.m_kItemQuantityUpdate;
			goto end_proc;
		}
	}
	else
	{
		// 1. 아이템들을 지운다..
		bool bUpdateFailed = false;
		LIF( Query_UpdateItemQuantity( kPacket_.m_iFromUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
		Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );	

		//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
		if( bUpdateFailed )
		{
			START_LOG( cout, L"증분값 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_iFromUnitUID );

			std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
			for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
			{
				START_LOG( cout, L"아이템 정보" )
					<< BUILD_LOG( mitQC->first )
					<< BUILD_LOG( mitQC->second );
			}
		}
		//}}

		int arrSocketInfo[4] = {0,0,0,0}; // DB 테이블의 소켓테이블 참고
		vit = kPacket_.m_kSendLetter.m_vecItemSocket.begin();
		for( int iIdx = 0; iIdx < 4; iIdx++ )
		{		
			if( vit == kPacket_.m_kSendLetter.m_vecItemSocket.end() )
				continue;

			arrSocketInfo[iIdx] = *vit;
			++vit;
		}
		
		//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05

		int arrRandomSocketInfo[CXSLSocketItem::RSC_MAX] = {0,}; // DB 테이블의 소켓테이블 참고

		std::vector< int >::const_iterator vitSocket = kPacket_.m_kSendLetter.m_vecRandomSocket.begin();
		for( int iIdx = 0; iIdx < CXSLSocketItem::RSC_MAX; ++iIdx )
		{
			if( vitSocket == kPacket_.m_kSendLetter.m_vecRandomSocket.end() )
				continue;

			arrRandomSocketInfo[iIdx] = *vitSocket;
			++vitSocket;
		}
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_INS_ItemNew",
			L"%d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iFromUnitUID					// @FromUnitUID bigint
			% kPacket_.m_wstrToNickName					// @ToUnitNickName nvarchar(16)
			% kPacket_.m_kSendLetter.m_iQuantity		// @Quantity  int
			% static_cast<int>(kPacket_.m_kSendLetter.m_cScriptType) // @ScType   tinyint
			% kPacket_.m_kSendLetter.m_iScriptIndex		// @ScIndex  int
			% kPacket_.m_kSendLetter.m_wstrTitle		// @Title   nvarchar(38)
			% kPacket_.m_kSendLetter.m_wstrMessage		// @Message  nvarchar(600)
			% static_cast<int>(kPacket_.m_kSendLetter.m_cEnchantLevel)	// @ELevel   tinyint
			% arrSocketInfo[0]							// @Socket1  smallint
			% arrSocketInfo[1]							// @Socket2  smallint
			% arrSocketInfo[2]							// @Socket3  smallint
			% arrSocketInfo[3]							// @Socket4  smallint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )	// @Attribute1  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )	// @Attribute2  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )	// @Attribute3  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_ucSealData)
			% arrRandomSocketInfo[0]					// @Socket1R
			% arrRandomSocketInfo[1]					// @Socket2R
			% arrRandomSocketInfo[2]					// @Socket3R
			% arrRandomSocketInfo[3]					// @Socket4R
			% arrRandomSocketInfo[4]					// @Socket5R
			);
#else //SERV_ALL_RENEWAL_SP
		// 2. 우체국에 편지를 넣는다..
		DO_QUERY( L"exec dbo.P_GPost_INS",
			L"%d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iFromUnitUID					// @FromUnitUID bigint
			% kPacket_.m_wstrToNickName					// @ToUnitNickName nvarchar(16)
			% kPacket_.m_kSendLetter.m_iQuantity		// @Quantity  int
			% static_cast<int>(kPacket_.m_kSendLetter.m_cScriptType) // @ScType   tinyint
			% kPacket_.m_kSendLetter.m_iScriptIndex		// @ScIndex  int
			% kPacket_.m_kSendLetter.m_wstrTitle		// @Title   nvarchar(38)
			% kPacket_.m_kSendLetter.m_wstrMessage		// @Message  nvarchar(600)
			% static_cast<int>(kPacket_.m_kSendLetter.m_cEnchantLevel)	// @ELevel   tinyint
			% arrSocketInfo[0]							// @Socket1  smallint
			% arrSocketInfo[1]							// @Socket2  smallint
			% arrSocketInfo[2]							// @Socket3  smallint
			% arrSocketInfo[3]							// @Socket4  smallint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )	// @Attribute1  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )	// @Attribute2  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )	// @Attribute3  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_ucSealData)
			% arrRandomSocketInfo[0]					// @Socket1R
			% arrRandomSocketInfo[1]					// @Socket2R
			% arrRandomSocketInfo[2]					// @Socket3R
			% arrRandomSocketInfo[3]					// @Socket4R
			% arrRandomSocketInfo[4]					// @Socket5R
			);
#endif //SERV_ALL_RENEWAL_SP			
#else
		DO_QUERY( L"exec dbo.gup_insert_post_item_new",
			L"%d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d",
			% kPacket_.m_iFromUnitUID					// @FromUnitUID bigint
			% kPacket_.m_wstrToNickName					// @ToUnitNickName nvarchar(16)
			% kPacket_.m_kSendLetter.m_iQuantity		// @Quantity  int
			% static_cast<int>(kPacket_.m_kSendLetter.m_cScriptType) // @ScType   tinyint
			% kPacket_.m_kSendLetter.m_iScriptIndex		// @ScIndex  int
			% kPacket_.m_kSendLetter.m_wstrTitle		// @Title   nvarchar(38)
			% kPacket_.m_kSendLetter.m_wstrMessage		// @Message  nvarchar(600)
			% static_cast<int>(kPacket_.m_kSendLetter.m_cEnchantLevel)	// @ELevel   tinyint
			% arrSocketInfo[0]							// @Socket1  smallint
			% arrSocketInfo[1]							// @Socket2  smallint
			% arrSocketInfo[2]							// @Socket3  smallint
			% arrSocketInfo[3]							// @Socket4  smallint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )	// @Attribute1  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )	// @Attribute2  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )	// @Attribute3  tinyint
			% static_cast<int>(kPacket_.m_kSendLetter.m_ucSealData)
			);
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}		

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_kSendLetter.m_iPostNo
				>> kPacket.m_kSendLetter.m_iToUnitUID
				>> kPacket.m_kSendLetter.m_wstrRegDate );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{	
			START_LOG( cerr, L"편지 쓰기 DB isnert실패! 일어나면 안되는 에러!!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iFromUnitUID )
				<< BUILD_LOG( kPacket_.m_wstrToNickName	)
				<< BUILD_LOG( kPacket_.m_kSendLetter.m_iQuantity )
				<< BUILD_LOGc( kPacket_.m_kSendLetter.m_cScriptType )
				<< BUILD_LOG( kPacket_.m_kSendLetter.m_iScriptIndex )
				<< BUILD_LOG( kPacket_.m_kSendLetter.m_wstrTitle )
				<< BUILD_LOG( kPacket_.m_kSendLetter.m_wstrMessage )
				<< BUILD_LOGc( kPacket_.m_kSendLetter.m_cEnchantLevel )
				<< BUILD_LOG( arrSocketInfo[0] )
				<< BUILD_LOG( arrSocketInfo[1] )
				<< BUILD_LOG( arrSocketInfo[2] )
				<< BUILD_LOG( arrSocketInfo[3] )
				<< BUILD_LOGc( kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )
				<< BUILD_LOGc( kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )
				//{{ 2009. 11. 9  최육사	트리플속성
				<< BUILD_LOGc( kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )
				//}}
				//{{ 2009. 8. 27  최육사	밀봉
				<< BUILD_LOGc( kPacket_.m_kSendLetter.m_ucSealData )
				//}}
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_POST_LETTER_03;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_LETTER_TO_POST_ACK, kPacket );
}
//}}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////
IMPL_ON_FUNC( DBE_INSERT_LETTER_TO_POST_REQ )
{
	KDBE_INSERT_LETTER_TO_POST_ACK kPacket;
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >::const_iterator vit;
#else
	std::vector< short >::const_iterator vit;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	kPacket.m_iOK					  = NetError::ERR_ODBC_01;
	kPacket.m_wstrToNickName		  = kPacket_.m_wstrToNickName;	
	kPacket.m_kSendLetter			  = kPacket_.m_kSendLetter;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iSendLetterCost		  = kPacket_.m_iSendLetterCost;

	// 1. 아이템들을 지운다..
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iFromUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );	

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iFromUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}
	
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
	const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
	const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS

	int arrSocketInfo[byteArraySize]; // DB 테이블의 소켓테이블 참고
	memset( arrSocketInfo, 0, sizeof(int) * byteArraySize );

	vit = kPacket_.m_kSendLetter.m_vecItemSocket.begin();
	for( int iIdx = 0; iIdx < byteArraySize; iIdx++ )
	{		
		if( vit == kPacket_.m_kSendLetter.m_vecItemSocket.end() )
			continue;

		arrSocketInfo[iIdx] = *vit;
		++vit;
	}
	
	//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05

	int arrRandomSocketInfo[CXSLSocketItem::RSC_MAX] = {0,}; // DB 테이블의 소켓테이블 참고

	std::vector< int >::const_iterator vitSocket = kPacket_.m_kSendLetter.m_vecRandomSocket.begin();
	for( int iIdx = 0; iIdx < CXSLSocketItem::RSC_MAX; ++iIdx )
	{
		if( vitSocket == kPacket_.m_kSendLetter.m_vecRandomSocket.end() )
			continue;

		arrRandomSocketInfo[iIdx] = *vitSocket;
		++vitSocket;
	}

	// 2. 우체국에 편지를 넣는다..
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_INS_ItemNew",
		L"%d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d",
		% kPacket_.m_iFromUnitUID					// @FromUnitUID bigint
		% kPacket_.m_wstrToNickName					// @ToUnitNickName nvarchar(16)
		% kPacket_.m_kSendLetter.m_iQuantity		// @Quantity  int
		% static_cast<int>(kPacket_.m_kSendLetter.m_cScriptType) // @ScType   tinyint
		% kPacket_.m_kSendLetter.m_iScriptIndex		// @ScIndex  int
		% kPacket_.m_kSendLetter.m_wstrTitle		// @Title   nvarchar(38)
		% kPacket_.m_kSendLetter.m_wstrMessage		// @Message  nvarchar(600)
		% static_cast<int>(kPacket_.m_kSendLetter.m_cEnchantLevel)	// @ELevel   tinyint
		% arrSocketInfo[0]							// @Socket1  smallint
		% arrSocketInfo[1]							// @Socket2  smallint
		% arrSocketInfo[2]							// @Socket3  smallint
		% arrSocketInfo[3]							// @Socket4  smallint
		% arrSocketInfo[4]							// @Socket5  smallint
		% kPacket_.m_kSendLetter.m_byteExpandedSocketNum
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )	// @Attribute1  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )	// @Attribute2  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )	// @Attribute3  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_ucSealData)
		% arrRandomSocketInfo[0]					// @Socket1R
		% arrRandomSocketInfo[1]					// @Socket2R
		% arrRandomSocketInfo[2]					// @Socket3R
		% arrRandomSocketInfo[3]					// @Socket4R
		% arrRandomSocketInfo[4]					// @Socket5R
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_INS",
		L"%d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		L"%d, %d, %d",
		% kPacket_.m_iFromUnitUID					// @FromUnitUID bigint
		% kPacket_.m_wstrToNickName					// @ToUnitNickName nvarchar(16)
		% kPacket_.m_kSendLetter.m_iQuantity		// @Quantity  int
		% static_cast<int>(kPacket_.m_kSendLetter.m_cScriptType) // @ScType   tinyint
		% kPacket_.m_kSendLetter.m_iScriptIndex		// @ScIndex  int
		% kPacket_.m_kSendLetter.m_wstrTitle		// @Title   nvarchar(38)
		% kPacket_.m_kSendLetter.m_wstrMessage		// @Message  nvarchar(600)
		% static_cast<int>(kPacket_.m_kSendLetter.m_cEnchantLevel)	// @ELevel   tinyint
		% arrSocketInfo[0]							// @Socket1  smallint
		% arrSocketInfo[1]							// @Socket2  smallint
		% arrSocketInfo[2]							// @Socket3  smallint
		% arrSocketInfo[3]							// @Socket4  smallint
		% arrSocketInfo[4]							// @Socket5  smallint
		% kPacket_.m_kSendLetter.m_byteExpandedSocketNum
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )	// @Attribute1  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )	// @Attribute2  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )	// @Attribute3  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_ucSealData)
		% arrRandomSocketInfo[0]					// @Socket1R
		% arrRandomSocketInfo[1]					// @Socket2R
		% arrRandomSocketInfo[2]					// @Socket3R
		% arrRandomSocketInfo[3]					// @Socket4R
		% arrRandomSocketInfo[4]					// @Socket5R
		);
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_BATTLE_FIELD_BOSS
	DO_QUERY( L"exec dbo.P_GPost_INS",
		L"%d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iFromUnitUID					// @FromUnitUID bigint
		% kPacket_.m_wstrToNickName					// @ToUnitNickName nvarchar(16)
		% kPacket_.m_kSendLetter.m_iQuantity		// @Quantity  int
		% static_cast<int>(kPacket_.m_kSendLetter.m_cScriptType) // @ScType   tinyint
		% kPacket_.m_kSendLetter.m_iScriptIndex		// @ScIndex  int
		% kPacket_.m_kSendLetter.m_wstrTitle		// @Title   nvarchar(38)
		% kPacket_.m_kSendLetter.m_wstrMessage		// @Message  nvarchar(600)
		% static_cast<int>(kPacket_.m_kSendLetter.m_cEnchantLevel)	// @ELevel   tinyint
		% arrSocketInfo[0]							// @Socket1  smallint
		% arrSocketInfo[1]							// @Socket2  smallint
		% arrSocketInfo[2]							// @Socket3  smallint
		% arrSocketInfo[3]							// @Socket4  smallint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )	// @Attribute1  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )	// @Attribute2  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )	// @Attribute3  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_ucSealData)
		% arrRandomSocketInfo[0]					// @Socket1R
		% arrRandomSocketInfo[1]					// @Socket2R
		% arrRandomSocketInfo[2]					// @Socket3R
		% arrRandomSocketInfo[3]					// @Socket4R
		% arrRandomSocketInfo[4]					// @Socket5R
		);
#endif // SERV_BATTLE_FIELD_BOSS
#else
	DO_QUERY( L"exec dbo.gup_insert_post_item_new",
		L"%d, N\'%s\', %d, %d, %d, N\'%s\', N\'%s\', %d, %d, %d, "
		L"%d, %d, %d, %d, %d, %d",
		% kPacket_.m_iFromUnitUID					// @FromUnitUID bigint
		% kPacket_.m_wstrToNickName					// @ToUnitNickName nvarchar(16)
		% kPacket_.m_kSendLetter.m_iQuantity		// @Quantity  int
		% static_cast<int>(kPacket_.m_kSendLetter.m_cScriptType) // @ScType   tinyint
		% kPacket_.m_kSendLetter.m_iScriptIndex		// @ScIndex  int
		% kPacket_.m_kSendLetter.m_wstrTitle		// @Title   nvarchar(38)
		% kPacket_.m_kSendLetter.m_wstrMessage		// @Message  nvarchar(600)
		% static_cast<int>(kPacket_.m_kSendLetter.m_cEnchantLevel)	// @ELevel   tinyint
		% arrSocketInfo[0]							// @Socket1  smallint
		% arrSocketInfo[1]							// @Socket2  smallint
		% arrSocketInfo[2]							// @Socket3  smallint
		% arrSocketInfo[3]							// @Socket4  smallint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )	// @Attribute1  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )	// @Attribute2  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )	// @Attribute3  tinyint
		% static_cast<int>(kPacket_.m_kSendLetter.m_ucSealData)
		);
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}	

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kSendLetter.m_iPostNo
			>> kPacket.m_kSendLetter.m_iToUnitUID
			>> kPacket.m_kSendLetter.m_wstrRegDate );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{	
		START_LOG( cerr, L"편지 쓰기 DB isnert실패! 일어나면 안되는 에러!!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iFromUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrToNickName	)
			<< BUILD_LOG( kPacket_.m_kSendLetter.m_iQuantity )
			<< BUILD_LOGc( kPacket_.m_kSendLetter.m_cScriptType )
			<< BUILD_LOG( kPacket_.m_kSendLetter.m_iScriptIndex )
			<< BUILD_LOG( kPacket_.m_kSendLetter.m_wstrTitle )
			<< BUILD_LOG( kPacket_.m_kSendLetter.m_wstrMessage )
			<< BUILD_LOGc( kPacket_.m_kSendLetter.m_cEnchantLevel )
			<< BUILD_LOG( arrSocketInfo[0] )
			<< BUILD_LOG( arrSocketInfo[1] )
			<< BUILD_LOG( arrSocketInfo[2] )
			<< BUILD_LOG( arrSocketInfo[3] )
			<< BUILD_LOGc( kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant0 )
			<< BUILD_LOGc( kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant1 )
			//{{ 2009. 11. 9  최육사	트리플속성
			<< BUILD_LOGc( kPacket_.m_kSendLetter.m_kAttribEnchantInfo.m_cAttribEnchant2 )
			//}}
			//{{ 2009. 8. 27  최육사	밀봉
			<< BUILD_LOGc( kPacket_.m_kSendLetter.m_ucSealData )
			//}}
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_POST_LETTER_03;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_LETTER_TO_POST_ACK, kPacket );
}
//////////////////////////////////////////////////////////////////////////
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

//{{ 2008. 9. 18  최육사	우편함
IMPL_ON_FUNC( DBE_GET_POST_LETTER_LIST_REQ )
{
	KDBE_GET_POST_LETTER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	// 보상
	//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_SEL_ItemNew", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Gpost_SEL", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else
	DO_QUERY( L"exec dbo.gup_get_post_item_new", L"%d", % kPacket_.m_iUnitUID );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}	

	while( m_kODBC.Fetch() )
	{
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
//#ifdef SERV_ITEM_OPTION_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
		const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS
		int arrSocketOption[byteArraySize];
		memset( arrSocketOption, 0, sizeof(int) * byteArraySize );
//#else
//		short arrSocketOption[4] = {0,0,0,0};
//#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		int arrRandomSocketOption[CXSLSocketItem::RSC_MAX] = {0,};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}

		KPostItemInfo kPostItemInfo;
		bool bReceived = false;

		//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		FETCH_DATA( kPostItemInfo.m_iPostNo
			>> kPostItemInfo.m_wstrFromNickName			
			>> kPostItemInfo.m_iToUnitUID
			>> kPostItemInfo.m_iQuantity
			>> kPostItemInfo.m_cScriptType
			>> kPostItemInfo.m_iScriptIndex
			>> kPostItemInfo.m_wstrRegDate
			>> bReceived
			>> kPostItemInfo.m_bRead
			>> kPostItemInfo.m_wstrTitle
			>> kPostItemInfo.m_wstrMessage
			>> kPostItemInfo.m_cEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> arrSocketOption[4]
			>> kPostItemInfo.m_byteExpandedSocketNum
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
			>> arrRandomSocketOption[0]
			>> arrRandomSocketOption[1]
			>> arrRandomSocketOption[2]
			>> arrRandomSocketOption[3]
			>> arrRandomSocketOption[4]
			>> kPostItemInfo.m_ucSealData
			>> kPostItemInfo.m_iFromUnitUID
			);
#else // SERV_BATTLE_FIELD_BOSS
		FETCH_DATA( kPostItemInfo.m_iPostNo
			>> kPostItemInfo.m_wstrFromNickName			
			>> kPostItemInfo.m_iToUnitUID
			>> kPostItemInfo.m_iQuantity
			>> kPostItemInfo.m_cScriptType
			>> kPostItemInfo.m_iScriptIndex
			>> kPostItemInfo.m_wstrRegDate
			>> bReceived
			>> kPostItemInfo.m_bRead
			>> kPostItemInfo.m_wstrTitle
			>> kPostItemInfo.m_wstrMessage
			>> kPostItemInfo.m_cEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
			>> arrRandomSocketOption[0]
			>> arrRandomSocketOption[1]
			>> arrRandomSocketOption[2]
			>> arrRandomSocketOption[3]
			>> arrRandomSocketOption[4]
			>> kPostItemInfo.m_ucSealData
			>> kPostItemInfo.m_iFromUnitUID
			);
#endif // SERV_BATTLE_FIELD_BOSS
#else
		FETCH_DATA( kPostItemInfo.m_iPostNo
			>> kPostItemInfo.m_wstrFromNickName
			>> kPostItemInfo.m_iToUnitUID
			>> kPostItemInfo.m_iQuantity
			>> kPostItemInfo.m_cScriptType
			>> kPostItemInfo.m_iScriptIndex
			>> kPostItemInfo.m_wstrRegDate
			>> bReceived
			>> kPostItemInfo.m_bRead
			>> kPostItemInfo.m_wstrTitle
			>> kPostItemInfo.m_wstrMessage
			>> kPostItemInfo.m_cEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
			>> kPostItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
			>> kPostItemInfo.m_ucSealData
			>> kPostItemInfo.m_iFromUnitUID
			);
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}	

		// 첨부를 이미 받았다면 초기화 처리
		if( bReceived )
		{
			kPostItemInfo.m_iScriptIndex = 0;
			kPostItemInfo.m_iQuantity	 = 0;
		}
		else
		{
			int iCheckIdx = byteArraySize;
			while( 0 <= --iCheckIdx )
			{
				if( arrSocketOption[iCheckIdx] != 0 )
					break;
			}

			for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
			{
				kPostItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );
			}

			//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
			// 2-1. 랜덤 소켓 정보 업데이트
			iCheckIdx = CXSLSocketItem::RSC_MAX;
			while( 0 <= --iCheckIdx )
			{
				if( arrRandomSocketOption[iCheckIdx] != 0 )
					break;
			}

			for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
			{
				kPostItemInfo.m_vecRandomSocket.push_back( arrRandomSocketOption[iIdx] );
			}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
			//}}
		}

		// 우체국 정보 넣기
		kPacket.m_vecPostItem.push_back( kPostItemInfo );
	}

	// 블랙리스트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_vGUnitNickName_SEL", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_post_blacklist", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		std::wstring wstrNickName;
		FETCH_DATA( wstrNickName );

		// 우체국 정보 넣기
		kPacket.m_vecBlackList.push_back( wstrNickName );
	}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_GET_POST_LETTER_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_READ_LETTER_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_UPD_IsRead", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_read_post_letter", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"편지 읽기 DB업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< BUILD_LOG( iOK )
			<< END_LOG;
	}

end_proc:
	return;
}

//{{ 2012. 08. 20	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
IMPL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_REQ )
{
	KDBE_GET_ITEM_FROM_LETTER_ACK kPacket;
	kPacket.m_iOK			  = NetError::ERR_ODBC_01;
	kPacket.m_iPostNo		  = kPacket_.m_iPostNo;
	kPacket.m_bSystemLetter	  = kPacket_.m_bSystemLetter;
	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iED			  = kPacket_.m_iED;

	// ED 동기화
	int iOK = NetError::ERR_ODBC_00;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_GamePointPost", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iIncrementED );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPT_POST_GP", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iIncrementED );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ED 동기화 실패!!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iIncrementED )
			<< BUILD_LOG( iOK )
			<< END_LOG;

		// 실패 했다면 ED를 원래대로 돌려놔야 한다.
		kPacket.m_iIncrementED = kPacket_.m_iIncrementED;
	}

	// 아이템 동기화
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}

	if( ( kPacket.m_iIncrementED != 0 ) ||
		( kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.empty() != true ) ||
		( kPacket.m_kItemQuantityUpdate.m_vecDeleted.empty() != true ) )
	{
		// 작업 전 동기화 수행 실패!!
		kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
		goto end_proc;
	}

	if( kPacket_.m_bSystemLetter != true )
	{
		kPacket.m_vecUpdatedInventorySlot.clear();
		if( 0 < kPacket_.m_iED )
		{
			const byte	iTradeType		= 1;				// 우편 받기
			int			iED				= 0;
			UidType		iItemUID		= 0;
			int			iQuantity		= 0;
			byte		iPostItemType	= 3;				// 첨부 없음
			byte		iUsageType		= static_cast<byte>( CXSLItem::PT_INFINITY );
			DO_QUERY( L"exec dbo.P_GItem_Total_Post",
				L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, N\'%s\', N\'%s\', %d",
				% iTradeType
				% kPacket_.m_iUnitUID
				% L""
				% iED
				% iItemUID
				% iQuantity
				% iUsageType
				% iPostItemType
				% L""
				% L""
				% kPacket_.m_iPostNo
				);

			int				iGarbagePostNo;
			UidType			iGarbageUnitUID;
			std::wstring	wstrGarbageRegDate;

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK
					>> iGarbagePostNo
					>> iGarbageUnitUID
					>> wstrGarbageRegDate
					>> iItemUID
					);
				m_kODBC.EndFetch();
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 가져오기 실패!!")
					<< BUILD_LOG( kPacket.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iPostNo )
					<< END_LOG;
				goto end_proc;
			}
		}
		else
		{
			if( kPacket_.m_vecItemInfo.empty() == true )
			{
                START_LOG( cerr, L"여기서 원소 갯수가 0개일리가 없음. 일어나서는 안되는 에러!" )
					<< BUILD_LOG( kPacket_.m_vecItemInfo.size() )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_UNKNOWN;
				goto end_proc;
			}

			BOOST_TEST_FOREACH( const KItemInfo&, kItem, kPacket_.m_vecItemInfo )
			{
				const byte	iTradeType		= 1;				// 우편 받기
				int			iED				= 0;
				UidType		iItemUID		= 0;
				int			iQuantity		= 0;
				byte		iPostItemType	= 3;				// 첨부 없음
				byte		iUsageType		= static_cast<byte>( kItem.m_cUsageType );
#ifndef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GItem_Total_Post",
					L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, N\'%s\', N\'%s\', %d",
					% iTradeType
					% kPacket_.m_iUnitUID
					% L""
					% iED
					% iItemUID
					% iQuantity
					% iUsageType
					% iPostItemType
					% L""
					% L""
					% kPacket_.m_iPostNo
					);

				int				iGarbagePostNo;
				UidType			iGarbageUnitUID;
				std::wstring	wstrGarbageRegDate;

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK
						>> iGarbagePostNo
						>> iGarbageUnitUID
						>> wstrGarbageRegDate
						>> iItemUID
						);
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"아이템 가져오기 실패!!")
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iPostNo )
						<< END_LOG;
					goto end_proc;
				}

				KInventoryItemInfo kInventoryItemInfo;
				kInventoryItemInfo.m_iItemUID	= iItemUID;
				kInventoryItemInfo.m_kItemInfo	= kItem;
				kPacket.m_vecUpdatedInventorySlot.push_back( kInventoryItemInfo );
				break;
#endif //SERV_ALL_RENEWAL_SP
			}
		}
	}
	else
	{
		// 시스템 편지라면 지우고..
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_UPD_DeleteSystem", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_DEL_System", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"편지 첨부물 받아가기 DB업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_iPostNo )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
			goto end_proc;
		}

		//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
		if( Query_InsertItemList( SEnum::GIR_FROM_LETTER, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
		if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
			//}}
		{
			START_LOG( cerr, L"아이템 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iPostNo )
				<< END_LOG;
		}
	}

end_proc:
SendToUser( LAST_SENDER_UID, DBE_GET_ITEM_FROM_LETTER_ACK, kPacket );
}

#else	// SERV_TRADE_LOGIC_CHANGE_LETTER

IMPL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_REQ )
{
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	KDBE_GET_ITEM_FROM_LETTER_ACK kPacket;
	kPacket.m_iOK			  = NetError::ERR_ODBC_01;
	kPacket.m_iPostNo		  = kPacket_.m_iPostNo;
	kPacket.m_cLetterType	  = kPacket_.m_cLetterType;
	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iED			  = kPacket_.m_iED;

	CTime tCurr = CTime::GetCurrentTime();
	CTime tWedding;
	std::wstring wstrWeddingDate;

	switch( kPacket_.m_cLetterType )
	{
	case KPostItemInfo::LT_POST_OFFICE:
		{
			// 유저 편지라면 첨부체크만 한다..
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_UPD", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_get_post_item", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}
		}
		break;
	case KPostItemInfo::LT_WEDDING_INVITATION:
	case KPostItemInfo::LT_WEDDING_RESERVE:
		{
			bool bExistWeddingUID = true;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_UPD_DeleteWedding", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo % bExistWeddingUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_DEL_Wedding", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo % bExistWeddingUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK
					     >> kPacket.m_iWeddingUID
						 >> wstrWeddingDate );
				m_kODBC.EndFetch();
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				kPacket.m_iWeddingUID = 0;
				START_LOG( cerr, L"웨딩 관련 우편인데...결혼식장UID 를 얻지 못했다" )
					<< BUILD_LOG( kPacket_.m_iPostNo )
					<< BUILD_LOG( kPacket.m_iOK )
					<< END_LOG;
			}
			else
			{
				KncUtil::ConvertStringToCTime( wstrWeddingDate, tWedding );
			}
		}
		break;
	case KPostItemInfo::LT_WEDDING_REWARD:
		{
			bool bExistWeddingUID = false;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_UPD_DeleteWedding", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo % bExistWeddingUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_DEL_Wedding", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo % bExistWeddingUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK
					>> kPacket.m_iWeddingUID
					>> wstrWeddingDate );
				m_kODBC.EndFetch();
			}

			// 없으므로 강제로 초기화
			kPacket.m_iWeddingUID = 0;

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				kPacket.m_iWeddingUID = 0;
				START_LOG( cerr, L"웨딩 관련 우편인데..." )
					<< BUILD_LOG( kPacket_.m_iPostNo )
					<< BUILD_LOG( kPacket.m_iOK )
					<< END_LOG;
			}
		}
		break;
	default:
		{
			// 시스템 편지라면 지우고..
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_UPD_DeleteSystem", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_DEL_System", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}
		}
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"편지 첨부물 받아가기 DB업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
		goto end_proc;
	}

	if( kPacket_.m_cLetterType == KPostItemInfo::LT_WEDDING_INVITATION || kPacket_.m_cLetterType == KPostItemInfo::LT_WEDDING_RESERVE )
	{
		std::vector< KItemInfo >::iterator vit = kPacket_.m_vecItemInfo.begin();
		if( vit != kPacket_.m_vecItemInfo.end() )
		{
			CTimeSpan tSpan(tWedding.GetTime() - tCurr.GetTime());
			vit->m_sPeriod	=(short)(tSpan.GetTotalHours() / 24 ) + 1;
		}
	}
	
	if( Query_InsertItemList( SEnum::GIR_FROM_LETTER, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< END_LOG;
	}
	
	if( kPacket_.m_cLetterType == KPostItemInfo::LT_WEDDING_INVITATION || kPacket_.m_cLetterType == KPostItemInfo::LT_WEDDING_RESERVE )
	{
		std::map< UidType, KItemInfo >::iterator mit = kPacket.m_mapItemInfo.begin();
		if( mit != kPacket.m_mapItemInfo.end() )
		{
			kPacket.m_iWeddingItemUID = mit->first;
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCouple_WeddingInvitation_INS", L"%d, %d", % mit->first % kPacket.m_iWeddingUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GCouple_WeddingInvitation_Wedding_INT", L"%d, %d", % mit->first % kPacket.m_iWeddingUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"청첩장 아이템 기록 실패!" )
					<< BUILD_LOG( mit->first )
					<< BUILD_LOG( kPacket.m_iWeddingUID )
					<< END_LOG;

				kPacket.m_iWeddingItemUID = 0;
			}
		}	
	}

#else //  SERV_RELATIONSHIP_SYSTEM
	KDBE_GET_ITEM_FROM_LETTER_ACK kPacket;
	kPacket.m_iOK			  = NetError::ERR_ODBC_01;
	kPacket.m_iPostNo		  = kPacket_.m_iPostNo;
	kPacket.m_bSystemLetter	  = kPacket_.m_bSystemLetter;
	kPacket.m_mapInsertedItem = kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iED			  = kPacket_.m_iED;

	if( kPacket_.m_bSystemLetter )
	{
		// 시스템 편지라면 지우고..
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_UPD_DeleteSystem", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_DEL_System", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#endif //SERV_ALL_RENEWAL_SP
	}
	else
	{
		// 유저 편지라면 첨부체크만 한다..
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_UPD", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_get_post_item", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPostNo );
#endif //SERV_ALL_RENEWAL_SP
	}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"편지 첨부물 받아가기 DB업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_POST_LETTER_06;
		goto end_proc;
	}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_FROM_LETTER, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
		//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPostNo )
			<< END_LOG;
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_ITEM_FROM_LETTER_ACK, kPacket );
}

#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}
IMPL_ON_FUNC( DBE_DELETE_LETTER_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	std::vector< UidType >::const_iterator vit;
	for( vit = kPacket_.m_vecPostNo.begin(); vit != kPacket_.m_vecPostNo.end(); ++vit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_UPD_Deldate", L"%d, %d", % kPacket_.m_iUnitUID % *vit );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_post_item_by_PostNo", L"%d, %d", % kPacket_.m_iUnitUID % *vit );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"편지 삭제 DB업데이트 실패!" )
				<< BUILD_LOG( *vit )
				<< BUILD_LOG( iOK )
				<< END_LOG;
		}
	}
}
//}}

IMPL_ON_FUNC( DBE_DEL_TUTORIAL_REQ )
{
	KDBE_DEL_TUTORIAL_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iTeacherUID = kPacket_.m_iTeacherUID;
	kPacket.m_iStudentUID = kPacket_.m_iStudentUID;
	kPacket.m_cReason = kPacket_.m_cReason;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GTutor_UPD_Delete", L"%d, %d",
		% kPacket_.m_iTeacherUID
		% kPacket_.m_iStudentUID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_Tutor", L"%d, %d",
				% kPacket_.m_iTeacherUID
				% kPacket_.m_iStudentUID
				);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"사제리스트 디비삭제 실패.!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iTeacherUID )
				<< BUILD_LOG( kPacket_.m_iStudentUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_TUTORIAL_00;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_DEL_TUTORIAL_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_TUTORIAL_REQ )
{
	KDBE_INSERT_TUTORIAL_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iTeacherUID = kPacket_.m_iTeacherUID;
	kPacket.m_kStudentUnitInfo = kPacket_.m_kStudentUnitInfo;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GTutor_INS", L"%d, %d",
		% kPacket_.m_iTeacherUID
		% kPacket_.m_kStudentUnitInfo.m_iUnitUID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_tutor", L"%d, %d",
		% kPacket_.m_iTeacherUID
		% kPacket_.m_kStudentUnitInfo.m_iUnitUID
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			switch( kPacket.m_iOK )
			{
			case -2:
				kPacket.m_iOK = NetError::ERR_TUTORIAL_04;
				break;
			case -3:
				kPacket.m_iOK = NetError::ERR_TUTORIAL_05;
				break;
			}

			START_LOG( cerr, L"사제관계 맺기 DB처리 실패.!" )
				<< BUILD_LOG( kPacket.m_iTeacherUID )
				<< BUILD_LOG( kPacket.m_kStudentUnitInfo.m_iUnitUID )
				<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
				<< END_LOG;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_TUTORIAL_ACK, kPacket );
}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27

IMPL_ON_FUNC( DBE_RESET_SKILL_REQ )
{
	KDBE_RESET_SKILL_ACK kPacket;	
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iDelSkillID				= kPacket_.m_iDelSkillID;
	kPacket.m_iDelSkillLevel			= kPacket_.m_iDelSkillLevel;
	kPacket.m_iCSPoint					= kPacket_.m_iCSPoint;
	kPacket.m_iBeforeSPoint				= kPacket_.m_iBeforeSPoint;
	kPacket.m_iBeforeCSPoint			= kPacket_.m_iBeforeCSPoint;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber	= kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( !kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.empty() || !kPacket.m_kItemQuantityUpdate.m_vecDeleted.empty() )
	{
		START_LOG( cerr, L"스킬 초기화 아이템 DB에서 수량감소 혹은 삭제 실패." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		kPacket.m_iOK = NetError::ERR_RESET_SKILL_02;
		goto end_proc;
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	{
		// 스킬 초기화
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% kPacket_.m_iDelSkillID 
			% kPacket_.m_iDelSkillLevel
			% 0
			% kPacket_.m_iActiveSkillPageNumber
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_UPD", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% kPacket_.m_iDelSkillID 
			% kPacket_.m_iDelSkillLevel
			% 0
			% kPacket_.m_iActiveSkillPageNumber
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );

			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"스킬 초기화 DB쿼리 실패 반환." )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iDelSkillID )
				<< BUILD_LOG( kPacket_.m_iActiveSkillPageNumber )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_RESET_SKILL_03;
			goto end_proc;
		}
	}
#else // SERV_SKILL_PAGE_SYSTEM
	{
		// 스킬 초기화
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% kPacket_.m_iDelSkillID 
			% kPacket_.m_iDelSkillLevel
			% 0
			% 0
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% kPacket_.m_iDelSkillID 
			% kPacket_.m_iDelSkillLevel
			% 0
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );

			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"스킬 초기화 DB쿼리 실패 반환." )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iDelSkillID )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_RESET_SKILL_03;
			goto end_proc;
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM
	
	{
		// 캐시 스킬 포인트 정보 갱신
		if( kPacket_.m_iCSPoint >= 0 )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_Cash_UPD_PointInfo", L"%d, %d",
				% kPacket_.m_iUnitUID
				% kPacket_.m_iCSPoint
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_cash_skill_point_info", L"%d, %d",
				% kPacket_.m_iUnitUID
				% kPacket_.m_iCSPoint
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}	

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"failed to update cash skill point when reset skill!" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
					<< BUILD_LOG( kPacket_.m_iDelSkillID )
					<< BUILD_LOG( kPacket_.m_iDelSkillLevel )
					<< BUILD_LOG( kPacket.m_iOK )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_SKILL_24;

				goto end_proc;	
			}
		}
	}

end_proc:
	LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iDelSkillID )
		<< BUILD_LOG( kPacket_.m_iDelSkillLevel )
		<< BUILD_LOG( kPacket_.m_iCSPoint )
		<< BUILD_LOG( kPacket.m_iOK );

	SendToUser( LAST_SENDER_UID, DBE_RESET_SKILL_ACK, kPacket );
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
IMPL_ON_FUNC( DBE_RESET_SKILL_REQ )
{
	KDBE_RESET_SKILL_ACK kPacket;	
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iSkillID					= kPacket_.m_iSkillID;
	kPacket.m_iSkillLevel				= kPacket_.m_iSkillLevel;
	kPacket.m_iSkillCSPoint				= kPacket_.m_iSkillCSPoint;
	kPacket.m_iCSPoint					= kPacket_.m_iCSPoint;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( !kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.empty() || !kPacket.m_kItemQuantityUpdate.m_vecDeleted.empty() )
	{
		START_LOG( cerr, L"스킬 초기화 아이템 DB에서 수량감소 혹은 삭제 실패." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
		kPacket.m_iOK = NetError::ERR_RESET_SKILL_02;
		goto end_proc;
	}

	// 스킬 초기화
	DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iSkillID 
		% kPacket_.m_iSkillLevel
		% kPacket_.m_iSkillCSPoint
		);


	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"스킬 초기화 DB쿼리 실패 반환." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RESET_SKILL_03;
		goto end_proc;
	}

	// 캐시 스킬 포인트 정보 갱신
	if( kPacket_.m_iCSPoint >= 0 )
	{
		DO_QUERY( L"exec dbo.gup_update_cash_skill_point_info", L"%d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}	

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to update cash skill point when reset skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iCSPoint )
				<< BUILD_LOG( kPacket_.m_iSkillID )
				<< BUILD_LOG( kPacket_.m_iSkillLevel )
				<< BUILD_LOG( kPacket_.m_iSkillCSPoint )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_24;

			goto end_proc;	
		}
	}



end_proc:
	LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iSkillID )
		<< BUILD_LOG( kPacket_.m_iSkillLevel )
		<< BUILD_LOG( kPacket_.m_iSkillCSPoint )
		<< BUILD_LOG( kPacket_.m_iCSPoint )
		//<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
		//<< BUILD_LOG( kPacket_.m_iRetrievedCSPoint )
		<< BUILD_LOG( kPacket.m_iOK );

	SendToUser( LAST_SENDER_UID, DBE_RESET_SKILL_ACK, kPacket );
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

IMPL_ON_FUNC( DBE_EXPAND_INVENTORY_SLOT_REQ )
{
    KDBE_EXPAND_INVENTORY_SLOT_ACK kPacket;
	kPacket.m_usEventID = kPacket_.m_usEventID;

    std::map< int, int >::const_iterator mit;
    for( mit = kPacket_.m_mapExpandedSlot.begin(); mit != kPacket_.m_mapExpandedSlot.end(); ++mit )
    {
//#ifdef SERV_REFORM_INVENTORY_INT	// 해외팀 주석 처리
		//{{ 2012. 12. 26	박세훈	인벤토리 개편 테스트	- 허상형 ( Merged by 박세훈 )
//#ifdef SERV_REFORM_INVENTORY_TEST
//		if( mit->first == CXSLInventory::ST_BANK )
//		{
//			DO_QUERY( L"exec dbo.gup_insert_inventory_size", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
//		}
//		else
//		{
//			DO_QUERY( L"exec dbo.P_GItemInventorySize_INS", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
//		}
//#else
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemInventorySize_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#else //SERV_ALL_RENEWAL_SP
        DO_QUERY( L"exec dbo.gup_insert_inventory_size", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#endif //SERV_ALL_RENEWAL_SP
//#endif SERV_REFORM_INVENTORY_TEST
		//}}
//#endif SERV_REFORM_INVENTORY_INT

		int iOK = NetError::ERR_ODBC_01;

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( iOK );
            m_kODBC.EndFetch();
        }

        if( iOK == NetError::NET_OK )
        {
            kPacket.m_mapExpandedSlot.insert( std::make_pair( mit->first, mit->second ) );
        }
        else
        {
            START_LOG( cerr, L"인벤토리 확장 실패." )
                << BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
                << END_LOG;
        }
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_EXPAND_INVENTORY_SLOT_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_EXPAND_SKILL_SLOT_REQ )
{
	KDBE_EXPAND_SKILL_SLOT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iPeriodExpire = kPacket_.m_iPeriodExpire;
	//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
	kPacket.m_usEventID = kPacket_.m_usEventID;
#endif SERV_ADD_PACKAGE_PRODUCT
	//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
	for ( int iSkillPageNumber = 1; iSkillPageNumber <= static_cast<int>( kPacket_.m_usTheNumberOfSkillPagesAvailable ); iSkillPageNumber++ )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkillSlot2_New_MER_Period", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPeriodExpire % kPacket_.m_usTheNumberOfSkillPagesAvailable );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkillSlot2_New_BUY", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPeriodExpire % kPacket_.m_usTheNumberOfSkillPagesAvailable );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK 
				>> kPacket.m_wstrSkillSlotBEndDate );

			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"스킬슬롯B 확장 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iPeriodExpire )			
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_14;
			goto end_proc;
		}
	}
#else // SERV_SKILL_PAGE_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkillSlot2_New_MER_Period", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPeriodExpire % 1 );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_skill_slotB_new", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPeriodExpire );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK 
			>> kPacket.m_wstrSkillSlotBEndDate );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"스킬슬롯B 확장 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPeriodExpire )			
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_14;
	}
#endif // SERV_SKILL_PAGE_SYSTEM


end_proc:
	SendToUser( LAST_SENDER_UID, DBE_EXPAND_SKILL_SLOT_ACK, kPacket );
}

//{{ 2008. 12. 14  최육사	캐릭터 슬롯 확장
IMPL_ON_FUNC( DBE_EXPAND_CHAR_SLOT_REQ )
{
	KDBE_EXPAND_CHAR_SLOT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUser_UPD_UsSize", L"%d, %d, %d", % LAST_SENDER_UID % kPacket_.m_iExpandSlotSize % kPacket_.m_iCharSlotMax );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_ussize", L"%d, %d, %d", % LAST_SENDER_UID % kPacket_.m_iExpandSlotSize % kPacket_.m_iCharSlotMax );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK 
			>> kPacket.m_iCharSlotSize );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 슬롯 확장 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( LAST_SENDER_UID )			
			<< BUILD_LOG( kPacket_.m_iExpandSlotSize )
			<< BUILD_LOG( kPacket_.m_iCharSlotMax )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_31;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_EXPAND_CHAR_SLOT_ACK, kPacket );
}
//}}

//{{ 2008. 5. 5  최육사  장바구니 얻기
IMPL_ON_FUNC( DBE_GET_WISH_LIST_REQ )
{
	KDBE_GET_WISH_LIST_ACK kPacket;
	int iSlotID = 0;
	int iItemID = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemWishList_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_WishList", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( iSlotID
				 >> iItemID
				 );
		
		kPacket.m_mapWishList.insert( std::make_pair( iSlotID, iItemID ) );
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_WISH_LIST_ACK, kPacket );
}
//}}

#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
//{{ 2008. 5. 19  최육사  이벤트 타임 업데이트
IMPL_ON_FUNC( DBE_UPDATE_EVENT_TIME_REQ )
{
	KDBE_UPDATE_EVENT_TIME_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	{
		//////////////////////////////////////////////////////////////////////////
		// 접속 시간 이벤트

		std::vector< KConnectTimeEventInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecConnectTimeEvent.begin(); vit != kPacket_.m_vecConnectTimeEvent.end(); ++vit )
		{
			//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
			if( vit->m_bAccountEvent )
			{
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GEvent_Account_Nor_MER", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vit->m_iEventUID % vit->m_wstrEventTime );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_event_account_nor", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vit->m_iEventUID % vit->m_wstrEventTime );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setConnectTimeEvent.insert( vit->m_iEventUID );
					
					//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
					if( vit->m_iScriptID == 573 )
					{
						CTime tCurrentTime = CTime::GetCurrentTime();
						std::wstring wstrCurrentTime = tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
						DO_QUERY( L"exec dbo.P_GEvent_AngelCoin_INT", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % kPacket_.m_iUnitUID % wstrCurrentTime );

						int iResult = -1;
						if( m_kODBC.BeginFetch() )
						{
							FETCH_DATA( iResult );
							m_kODBC.EndFetch();
						}
						
						if( iResult != 0 )
						{
							START_LOG( cerr, L"대천사의 주화 이벤트 Insert BeginFetch() 실패")
								<< BUILD_LOG( LAST_SENDER_UID )
								<< BUILD_LOG( kPacket_.m_iUnitUID )
								<< BUILD_LOG( wstrCurrentTime )
								<< END_LOG;
						}
					}
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
					//}}
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vit->m_iEventUID )
						<< BUILD_LOG( vit->m_wstrEventTime )
						<< END_LOG;
				}
			}
			else
#endif SERV_ACC_TIME_EVENT
			//}}
			{
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GIs30Min_MER", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vit->m_iEventUID % vit->m_wstrEventTime );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_30min", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vit->m_iEventUID % vit->m_wstrEventTime );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setConnectTimeEvent.insert( vit->m_iEventUID );
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vit->m_iEventUID )
						<< BUILD_LOG( vit->m_wstrEventTime )
						<< END_LOG;
				}
			}			
		}

		//////////////////////////////////////////////////////////////////////////		
	}
	

	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT

	{
		//////////////////////////////////////////////////////////////////////////
		// 누적 시간 이벤트

		CTime tUpdateTime = CTime::GetCurrentTime();
		tUpdateTime += CTimeSpan( 18250, 0, 0, 0 ); // 한번만 줘야 하므로 50년을 더한다.
		std::wstring wstrUpdateDate = ( CStringW )( tUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

		std::vector< KCumulativeTimeEventInfo >::const_iterator vitCT;
		for( vitCT = kPacket_.m_vecCumulativeTimeEvent.begin(); vitCT != kPacket_.m_vecCumulativeTimeEvent.end(); ++vitCT )
		{
			//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
			if( vitCT->m_bAccountEvent )
			{
				// 완료 정보를 업데이트 하자!
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GEvent_Account_Nor_MER", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vitCT->m_iEventUID % wstrUpdateDate );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_event_account_nor", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vitCT->m_iEventUID % wstrUpdateDate );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setCumulativeTimeEvent.insert( vitCT->m_iEventUID );
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vitCT->m_iEventUID )
						<< BUILD_LOG( wstrUpdateDate )
						<< END_LOG;
				}
			}
			else
#endif SERV_ACC_TIME_EVENT
			//}}
			{
				// 완료 정보를 업데이트 하자!
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GIs30Min_MER", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vitCT->m_iEventUID % wstrUpdateDate );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_30min", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vitCT->m_iEventUID % wstrUpdateDate );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( kPacket.m_iOK );
					m_kODBC.EndFetch();
				}

				if( kPacket.m_iOK == NetError::NET_OK )
				{
					kPacket.m_setCumulativeTimeEvent.insert( vitCT->m_iEventUID );
				}
				else
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( kPacket.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vitCT->m_iEventUID )
						<< BUILD_LOG( wstrUpdateDate )
						<< END_LOG;
				}
			}			
		}
	}	

#endif CUMULATIVE_TIME_EVENT
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_UPDATE_EVENT_TIME_ACK, kPacket );
}
//}}
#endif //SERV_ADD_EVENT_DB

//{{ 2008. 5. 28  최육사  잘못된 인벤 카테고리
IMPL_ON_FUNC( DBE_UPDATE_INVENTORY_ITEM_POS_NOT )
{
	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	std::map< UidType, KItemPosition > mapFailedPos;
#else
	std::map< UidType, std::pair< int, int > > mapFailedPos;
#endif SERV_PET_SYSTEM
	//}}
	
	Query_UpdateItemPosition( kPacket_.m_iUnitUID, kPacket_.m_kItemPositionUpdate, mapFailedPos );
}
//}}

//{{ 2008. 6. 20  최육사  선물하기
IMPL_ON_FUNC( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ )
{
	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK kPacket;
	kPacket.m_iOK				 = NetError::ERR_ODBC_01;
	kPacket.m_wstrMessage		 = kPacket_.m_wstrMessage;
	kPacket.m_vecPurchaseReqInfo = kPacket_.m_vecPurchaseReqInfo;

#ifdef SERV_GLOBAL_BILLING
	kPacket.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	kPacket.m_iUseCashType		 = kPacket_.m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	kPacket.m_bUseCoupon		= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	// SERV_GLOBAL_BILLING - SP 수정 해야 함
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUser_SEL_NickName", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_userid_by_nickname", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrUserID
			>> kPacket.m_iReceiverUserUID	// SERV_GLOBAL_BILLING
			>> kPacket.m_iReceiverUnitUID	// SERV_GLOBAL_BILLING
			);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_BUY_CASH_ITEM_22;   break;
		}
	}
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnit_SEL_Level", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_unitclass_level", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			int iOK = -1;
			FETCH_DATA( iOK
				>> kPacket.m_iReceiverUnitClass
				>> kPacket.m_cReceiverLevel
				);
			m_kODBC.EndFetch();
		}
		else
		{
			kPacket.m_cReceiverLevel = -1;
			kPacket.m_iReceiverUnitClass = -1;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK, kPacket );
}
//}}

//{{ 2008. 9. 3  최육사		속성강화
IMPL_ON_FUNC( DBE_ATTRIB_ENCHANT_ITEM_REQ )
{
	KDBE_ATTRIB_ENCHANT_ITEM_ACK kPacket;
	kPacket.m_iItemUID = kPacket_.m_iItemUID;
	kPacket.m_iItemID = kPacket_.m_iItemID;
	kPacket.m_cAttribEnchantSlotNo = kPacket_.m_cAttribEnchantSlotNo;
	kPacket.m_cAttribEnchantID = kPacket_.m_cAttribEnchantID;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemAttribute_MER", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% static_cast<int>(kPacket_.m_cAttribEnchantSlotNo) 
		% static_cast<int>(kPacket_.m_cAttribEnchantID)
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_Attribute", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% static_cast<int>(kPacket_.m_cAttribEnchantSlotNo) 
		% static_cast<int>(kPacket_.m_cAttribEnchantID)
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"속성 강화 정보 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOGc( kPacket_.m_cAttribEnchantSlotNo )
			<< BUILD_LOGc( kPacket_.m_cAttribEnchantID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ATTRIB_ENCHANT_01;
	}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ATTRIB_ENCHANT_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_IDENTIFY_ITEM_REQ )
{
	KDBE_IDENTIFY_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_IDENTIFY_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}	
	{
		START_LOG( cerr, L"감정된 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_IDENTIFY_04;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	SendToUser( LAST_SENDER_UID, DBE_IDENTIFY_ITEM_ACK, kPacket );
}
//}}

//{{ 2008. 9. 26  최육사	우체국 블랙리스트
IMPL_ON_FUNC( DBE_NEW_POST_BLACK_LIST_REQ )
{
	KDBE_NEW_POST_BLACK_LIST_ACK kPacket;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_BlackList_INS", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_post_blacklist", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_00; break;
		case -2: kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_01; break;
		
		default: kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_02; break;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_NEW_POST_BLACK_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DEL_POST_BLACK_LIST_REQ )
{
	KDBE_DEL_POST_BLACK_LIST_ACK kPacket;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;	
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_BlackList_DEL", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_post_blacklist", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_00; break;
		case -2: kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_03; break;

		default: kPacket.m_iOK = NetError::ERR_POST_BLACK_LIST_04; break;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_DEL_POST_BLACK_LIST_ACK, kPacket );
}
//}}

//{{ 2008. 10. 7  최육사	타이틀
IMPL_ON_FUNC( DBE_INSERT_TITLE_REQ )
{
	KDBE_INSERT_TITLE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iTitleID = kPacket_.m_iTitleID;

	//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
	if( kPacket_.m_bExpandPeriod )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_UPD_EndDate", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_title", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
	}
	else
#endif SERV_TITLE_ITEM_NEW
	//}}
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_MER", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_Title", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
	}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrEndDate );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
        START_LOG( cerr, L"칭호 업데이트가 실패했다? 일어날수 없는 에러." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iTitleID )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TITLE_04;
	}
	
end_proc:
	if( kPacket_.m_bGameServerEvent )
	{
		SendToServer( DBE_INSERT_TITLE_ACK, kPacket );
	}
	else
	{
		SendToUser( LAST_SENDER_UID, DBE_INSERT_TITLE_ACK, kPacket );
	}
}
//}}

//{{ 2008. 12. 25  최육사	부여
IMPL_ON_FUNC( DBE_ENCHANT_ATTACH_ITEM_REQ )
{
	KDBE_ENCHANT_ATTACH_ITEM_ACK kPacket;
	kPacket.m_iItemUID				  = kPacket_.m_iItemUID;
	kPacket.m_iLevelAfterEnchant	  = kPacket_.m_iLevelAfterEnchant;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec P_GItemEnchant_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iItemUID % kPacket_.m_iLevelAfterEnchant );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec gup_update_Enchant", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iItemUID % kPacket_.m_iLevelAfterEnchant );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"강화 레벨 부여 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iLevelAfterEnchant )	
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ATTACH_ITEM_03;
		goto end_proc;
	}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ENCHANT_ATTACH_ITEM_ACK, kPacket );
}
//}}

//{{ 2008. 11. 18  최육사	아이템 교환
IMPL_ON_FUNC( DBE_ITEM_EXCHANGE_REQ )
{
	KDBE_ITEM_EXCHANGE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;	
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;	

	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	kPacket.m_iSourceItemID			= kPacket_.m_iSourceItemID;
	kPacket.m_iSourceItemQuantity	= kPacket_.m_iSourceItemQuantity;
	kPacket.m_cExchangeType			= kPacket_.m_cExchangeType;
#endif SERV_EXCHANGE_LOG
	//}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_EXCHANGE_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"아이템 교환 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_EXCHANGE_04;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
	kPacket.m_mapResultItem = kPacket_.m_mapResultItem;
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
	//}}

#ifdef SERV_2013_JUNGCHU_TITLE
	kPacket.m_b12TimesRewarded = false;
#endif SERV_2013_JUNGCHU_TITLE

#if defined( SERV_GROW_UP_SOCKET ) || defined( SERV_2013_JUNGCHU_TITLE )
	if( ( kPacket.m_iOK == NetError::NET_OK ) && kPacket_.m_iSourceItemID == EXCHANGE_ITEM_ID_FOR_GROW_UP )
	{
		DO_QUERY( L"exec dbo.P_GEventTradeCount_UPD", L"%d", % kPacket_.m_iUnitUID );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iExchangeCount );
			m_kODBC.EndFetch();

#ifdef SERV_2013_JUNGCHU_TITLE
#ifdef SERV_2013_SILVER_WEEK_TITLE
	// 일본은 보상주지 않는다.
#else //SERV_2013_SILVER_WEEK_TITLE
			if( kPacket.m_iExchangeCount == 12 )
				kPacket.m_b12TimesRewarded = true;
#endif //SERV_2013_SILVER_WEEK_TITLE
#endif SERV_2013_JUNGCHU_TITLE
		}
		else
		{
			START_LOG( clog, L"P_GEventTradeCount_UPD 호출 실패! 실패 하면 안되는데??" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;
		}
	}
end_proc:
#endif SERV_GROW_UP_SOCKET

	//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	if( ( kPacket.m_iOK == NetError::NET_OK ) && ( kPacket_.m_vecDestItem.empty() == false ) )
	{
		bool bChecker = false;
		if( 1 < kPacket_.m_vecDestItem.size() )
		{
			START_LOG( cerr, L"대천사의 주화 교환 결과 아이템의 종류가 여러개입니다.")
				<< END_LOG;
			bChecker = true;
		}

		CTime tCurrentTime = CTime::GetCurrentTime();
		std::wstring wstrCurrentTime = tCurrentTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

		BOOST_TEST_FOREACH( const int, iItemID, kPacket_.m_vecDestItem )
		{
			if( bChecker == true )
			{
				START_LOG( cerr, iItemID )
					<< END_LOG;
			}

			DO_QUERY( L"exec dbo.P_GEvent_AngelCoin_Log_INT", L"%d, %d, %d, N\'%s\'", % LAST_SENDER_UID % kPacket_.m_iUnitUID % iItemID % wstrCurrentTime );

			int iResult = -2;
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iResult );
				m_kODBC.EndFetch();
			}

			if( iResult != NetError::NET_OK )
			{
				START_LOG( cerr, L"대천사의 주화 교환 로그 삽입 실패")
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( iItemID )
					<< BUILD_LOG( wstrCurrentTime )
					<< END_LOG;
			}
		}
	}

end_proc:
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	//}}

	SendToUser( LAST_SENDER_UID, DBE_ITEM_EXCHANGE_ACK, kPacket );
}
//}}

//{{ 2009. 4. 8  최육사		닉네임 변경
IMPL_ON_FUNC( DBE_DELETE_NICK_NAME_REQ )
{
	KDBE_DELETE_NICK_NAME_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_UPD", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_change_nickname", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_CHANGE_NICK_NAME_00;
		goto end_proc;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
		case -2:
			{
				START_LOG( cerr, L"닉네임 변경 실패!" )
					<< BUILD_LOG( kPacket.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_CHANGE_NICK_NAME_00;
			}
			break;

		default:
			{
				START_LOG( cerr, L"정의되지 않은 에러! sp가 바꼈나?" )
					<< BUILD_LOG( kPacket.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_wstrNickName )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_CHANGE_NICK_NAME_00;
			}
		}
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_DELETE_NICK_NAME_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( DBE_INSERT_CASH_SKILL_POINT_REQ )
{
	KDBE_INSERT_CASH_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK					= NetError::ERR_ODBC_01;
	kPacket.m_iCSPoint				= kPacket_.m_iCSPoint;
	kPacket.m_iPeriod				= kPacket_.m_iPeriod;
	kPacket.m_bUpdateEndDateOnly	= kPacket_.m_bUpdateEndDateOnly;
	//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
	kPacket.m_iSkillPointItemID		= kPacket_.m_iSkillPointItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
	//}}


	if( true == kPacket_.m_bUpdateEndDateOnly )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_Cash_UPD_Period", L"%d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_cash_skill_period", L"%d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
	}
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_Cash_MER", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			% kPacket_.m_iPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_cash_skill_point_info", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iCSPoint
			% kPacket_.m_iPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
	}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrEndDate );
		m_kODBC.EndFetch();
	}	

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"failed to insert cash skill point!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iCSPoint )
			<< BUILD_LOG( kPacket_.m_iPeriod )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_23;

		goto end_proc;	
	}


end_proc:
	LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_INSERT_CASH_SKILL_POINT_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_EXPIRE_CASH_SKILL_POINT_REQ )
{
	KDBE_EXPIRE_CASH_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK					= NetError::ERR_ODBC_01;
	
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	
	for ( UINT i = 0; i < kPacket_.m_vecRetrievedSkillPageData.size(); i++ )
		kPacket.m_vecRetrievedSPoint.push_back( kPacket_.m_vecRetrievedSkillPageData[i].m_iRetrievedSPoint );

#else // SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iRetrievedSPoint		= kPacket_.m_iRetrievedSPoint;
#endif // SERV_SKILL_PAGE_SYSTEM


	// expire 시키기 전에 현재 배운 스킬트리의 복사본을 log로 남긴다, 그리고 MAX_CSP를 0로 바꾼다.

#ifdef SERV_SKILL_PAGE_SYSTEM
	DO_QUERY( L"exec dbo.P_GSkill_BeforeList_INS", L"%d", % kPacket_.m_iUnitUID );
#else // SERV_SKILL_PAGE_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_BeforeList_INS", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_BeforeSkillList", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#endif // SERV_SKILL_PAGE_SYSTEM


	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // 김현철 // kPacket 에 kPacket_ 의 정보를 대입했으므로 아래와 같이 처리함
		int iSumCSPoint = 0;
		for ( UINT i = 0; i < kPacket.m_vecRetrievedSPoint.size(); i++ )
			iSumCSPoint += kPacket.m_vecRetrievedSPoint[i];

		START_LOG( cerr, L"failed to log current skill tree before reset cash skill point!!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( iSumCSPoint )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
#else // SERV_SKILL_PAGE_SYSTEM
		START_LOG( cerr, L"failed to log current skill tree before reset cash skill point!!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
#endif // SERV_SKILL_PAGE_SYSTEM

		kPacket.m_iOK = NetError::ERR_SKILL_25;
		goto end_proc;	
	}

	// 로그를 남겼다면 실제로 cash skill point 정보를 초기화 한다
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Cash_UPD_PointInfo", L"%d, %d", % kPacket_.m_iUnitUID % 0 ); // Cash skill point
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_cash_skill_point_info", L"%d, %d", % kPacket_.m_iUnitUID % 0 ); // Cash skill point
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{			
#ifdef SERV_SKILL_PAGE_SYSTEM
		// kimhc // 김현철 // kPacket 에 kPacket_ 의 정보를 대입했으므로 아래와 같이 처리함
		int iSumSPoint = 0;
		for ( UINT i = 0; i < kPacket.m_vecRetrievedSPoint.size(); i++ )
			iSumSPoint += kPacket.m_vecRetrievedSPoint[i];

		START_LOG( cerr, L"failed to reset cash skill point!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( iSumSPoint )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
#else // SERV_SKILL_PAGE_SYSTEM
		START_LOG( cerr, L"failed to reset cash skill point!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
#endif // SERV_SKILL_PAGE_SYSTEM

		kPacket.m_iOK = NetError::ERR_SKILL_25;
		goto end_proc;	
	}

	// note!! 300개 이상 skill정보가 바뀌는 경우가 있을까? 로그만 남기기.
	// 과도한 쿼리 호출을 걱정해서 남긴 로그?
#ifdef SERV_SKILL_PAGE_SYSTEM
	LogAboutCheckingTooManySkillDatas( kPacket_ );
#else // SERV_SKILL_PAGE_SYSTEM
	if( kPacket_.m_vecUserSkillData.size() > 300 )
	{
		START_LOG( cerr, L"too many skill update on cash skill point expiration!" )
			<< BUILD_LOG( (int) kPacket_.m_vecUserSkillData.size() )
			<< END_LOG;
	}
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_SKILL_PAGE_SYSTEM
	for ( UINT iPageIndex = 0; iPageIndex < kPacket_.m_vecRetrievedSkillPageData.size(); iPageIndex++ )
	{
		const KRetrievedSkillPageData& retrievedSkillPageData = kPacket_.m_vecRetrievedSkillPageData[iPageIndex];
		for ( UINT iSkillIndex = 0; iSkillIndex < retrievedSkillPageData.m_vecUserSkillData.size(); iSkillIndex++ )
		{
			const KUserSkillData& userSkillData = retrievedSkillPageData.m_vecUserSkillData[iSkillIndex];

			{
				const int iSkillPageNumber = iPageIndex + 1;
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d %d",
					% kPacket_.m_iUnitUID 
					% userSkillData.m_iSkillID
					% userSkillData.m_cSkillLevel
					% userSkillData.m_cSkillCSPoint
					% iSkillPageNumber);	/// 넘겨줄 때는 Index가 아닌 PageNubmer를 넘겨줘야 하므로 1부터 시작하는 번호로 전달
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GSkill_New_UPD", L"%d, %d, %d, %d %d",
					% kPacket_.m_iUnitUID 
					% userSkillData.m_iSkillID
					% userSkillData.m_cSkillLevel
					% userSkillData.m_cSkillCSPoint
					% iSkillPageNumber);	/// 넘겨줄 때는 Index가 아닌 PageNubmer를 넘겨줘야 하므로 1부터 시작하는 번호로 전달
#endif //SERV_ALL_RENEWAL_SP
				int iResult = NetError::ERR_UNKNOWN;
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iResult );
					m_kODBC.EndFetch();
				}

				if( iResult != NetError::NET_OK )
				{
					START_LOG( cerr, L"failed to update skill on cash skill point expire!" )
						<< BUILD_LOG( LAST_SENDER_UID )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( retrievedSkillPageData.m_iRetrievedSPoint )
						<< BUILD_LOG( userSkillData.m_iSkillID )
						<< BUILD_LOG( userSkillData.m_cSkillLevel )
						<< BUILD_LOG( userSkillData.m_cSkillCSPoint )
						<< BUILD_LOG( iSkillPageNumber )
						<< BUILD_LOG( kPacket.m_iOK )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_SKILL_22;
				}
			}

		}
	}
#else // SERV_SKILL_PAGE_SYSTEM
	for( UINT i = 0; i < kPacket_.m_vecUserSkillData.size(); ++i )
	{
		const KUserSkillData& userSkillData = kPacket_.m_vecUserSkillData[i];
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d %d",
			% kPacket_.m_iUnitUID 
			% userSkillData.m_iSkillID
			% userSkillData.m_cSkillLevel
			% userSkillData.m_cSkillCSPoint
			% 1);	/// 넘겨줄 때는 Index가 아닌 PageNubmer를 넘겨줘야 하므로 1부터 시작하는 번호로 전달
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% userSkillData.m_iSkillID
			% userSkillData.m_cSkillLevel
			% userSkillData.m_cSkillCSPoint );
#endif //SERV_ALL_RENEWAL_SP
		int iResult = NetError::ERR_UNKNOWN;
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iResult );
			m_kODBC.EndFetch();
		}

		if( iResult != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to update skill on cash skill point expire!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
				<< BUILD_LOG( userSkillData.m_iSkillID )
				<< BUILD_LOG( userSkillData.m_cSkillLevel )
				<< BUILD_LOG( userSkillData.m_cSkillCSPoint )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_SKILL_22;
		}
	}
#endif // SERV_SKILL_PAGE_SYSTEM


end_proc:
	LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_EXPIRE_CASH_SKILL_POINT_ACK, kPacket );
}

//{{ 2009. 8. 4  최육사		봉인 스킬
IMPL_ON_FUNC( DBE_UNSEAL_SKILL_REQ )
{
	KDBE_UNSEAL_SKILL_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iSkillID = kPacket_.m_iSkillID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Unsealed_INS", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iSkillID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_unsealed_skill", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iSkillID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"봉인스킬 봉인해제 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iSkillID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_27;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_UNSEAL_SKILL_ACK, kPacket );
}
//}}

//{{ 2009. 5. 11  최육사	실시간 아이템
IMPL_ON_FUNC( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ )
{
	KDBE_GET_ITEM_INSERT_TO_INVENTORY_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kBroadInfo = kPacket_.m_kBroadInfo;
	kPacket.m_mapInsertItem = kPacket_.m_mapInsertItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_DUNGEON_DROP, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}	
	{
		START_LOG( cerr, L"실시간 아이템아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_10;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;

		//{{ 2009. 11. 18  최육사	특정시각드롭이벤트
		std::set< int > setSealItem;
		std::vector< KItemInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecItemInfo.begin(); vit != kPacket_.m_vecItemInfo.end(); ++vit )
		{
			if( vit->IsSealedItem() )
			{
				setSealItem.insert( vit->m_iItemID );
			}
		}

		Query_UpdateSealItem( setSealItem, kPacket.m_mapItemInfo );
		//}}
	}

	SendToUser( LAST_SENDER_UID, DBE_GET_ITEM_INSERT_TO_INVENTORY_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_GET_TEMP_ITEM_REQ )
{
	KDBE_GET_TEMP_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iTempItemUID = kPacket_.m_iTempItemUID;
	kPacket.m_mapInsertItem = kPacket_.m_mapInsertItem;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_TEMP_INVENTORY, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"실시간 아이템아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_10;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;

		//{{ 2009. 11. 18  최육사	특정시각드롭이벤트
		std::set< int > setSealItem;
		std::vector< KItemInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecItemInfo.begin(); vit != kPacket_.m_vecItemInfo.end(); ++vit )
		{
            if( vit->IsSealedItem() )
			{
				setSealItem.insert( vit->m_iItemID );
			}
		}

		Query_UpdateSealItem( setSealItem, kPacket.m_mapItemInfo );
		//}}
	}

	SendToUser( LAST_SENDER_UID, DBE_GET_TEMP_ITEM_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( DBE_REQUEST_FRIEND_REQ )
{
    KDBE_REQUEST_FRIEND_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
    kPacket.m_wstrMessage = kPacket_.m_wstrMessage;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_INS", L"%d, %d, N\'%s\', %d, %d",
		% LAST_SENDER_UID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% KFriendInfo::FS_REQUESTED
		% KFriendInfo::FS_WAITING
		);
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_request", L"%d, %d, N\'%s\', %d, %d",
		% LAST_SENDER_UID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickName
		% KFriendInfo::FS_REQUESTED
		% KFriendInfo::FS_WAITING
		);
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK
            >> kPacket.m_iUnitUID );

        m_kODBC.EndFetch();
    }

	// 2014.03.03 이지헌 sp 반환값을 정확히 알기 위해서 로그 위치를 위로 올렸습니다.
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"친구 요청 실패" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket.m_iUnitUID )
			<< END_LOG;
	}

    switch( kPacket.m_iOK )
    {
    case NetError::NET_OK:
        break;
#ifdef SERV_REQUEST_FRIEND_NO_NICKNAME_ERROR_FIX
	case -1:	// 임규수 일본 추가 닉네임 존재하지 않을 시 
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_04;
		break;
#endif SERV_REQUEST_FRIEND_NO_NICKNAME_ERROR_FIX
    case -5:
        kPacket.m_iOK = NetError::ERR_MESSENGER_13;
        break;
    case -6:
        kPacket.m_iOK = NetError::ERR_MESSENGER_12;
        break;
    case -7:
        kPacket.m_iOK = NetError::ERR_MESSENGER_14;
        break;
    case -8:
        kPacket.m_iOK = NetError::ERR_MESSENGER_15;
        break;
    default:
        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
        break;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_REQUEST_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ACCEPT_FRIEND_REQ )
{
    KDBE_ACCEPT_FRIEND_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iFriendUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_UPD_Accept", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iFriendUnitUID
		% KFriendInfo::FS_WAITING
		% KFriendInfo::FS_REQUESTED
		% KFriendInfo::FS_NORMAL
		);
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_accept", L"%d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iFriendUnitUID
		% KFriendInfo::FS_WAITING
		% KFriendInfo::FS_REQUESTED
		% KFriendInfo::FS_NORMAL
		);
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 수락 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOG( kPacket_.m_iFriendUnitUID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_ACCEPT_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DENY_FRIEND_REQ )
{
    KDBE_DENY_FRIEND_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iFriendUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_DEL", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iFriendUnitUID );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_delete", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iFriendUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 거절 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOG( kPacket_.m_iFriendUnitUID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_DENY_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BLOCK_FRIEND_REQ )
{
    KDBE_BLOCK_FRIEND_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iFriendUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_UPD_Unblock", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iFriendUnitUID
		% KFriendInfo::FS_NORMAL
		% KFriendInfo::FS_BLOCKED
		);
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_block_unblock", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iFriendUnitUID
		% KFriendInfo::FS_NORMAL
		% KFriendInfo::FS_BLOCKED
		);
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 차단 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOG( kPacket_.m_iFriendUnitUID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_BLOCK_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UNBLOCK_FRIEND_REQ )
{
    KDBE_UNBLOCK_FRIEND_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iFriendUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_UPD_Unblock", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iFriendUnitUID
		% KFriendInfo::FS_BLOCKED
		% KFriendInfo::FS_NORMAL
		);
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_block_unblock", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iFriendUnitUID
		% KFriendInfo::FS_BLOCKED
		% KFriendInfo::FS_NORMAL
		);
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 차단 해제 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOG( kPacket_.m_iFriendUnitUID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_UNBLOCK_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DELETE_FRIEND_REQ )
{
    KDBE_DELETE_FRIEND_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iFriendUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_DEL", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iFriendUnitUID );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_delete", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iFriendUnitUID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 삭제 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOG( kPacket_.m_iFriendUnitUID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_DELETE_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_MOVE_FRIEND_REQ )
{
    KDBE_MOVE_FRIEND_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iUnitUID = kPacket_.m_iFriendUnitUID;
    kPacket.m_cTargetGroupID = kPacket_.m_cTargetGroupID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_UPD_Move", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iFriendUnitUID % ( int )kPacket_.m_cTargetGroupID );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_move", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iFriendUnitUID % ( int )kPacket_.m_cTargetGroupID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 그룹 이동 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOG( kPacket_.m_iFriendUnitUID )
            << BUILD_LOGc( kPacket_.m_cTargetGroupID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_MOVE_FRIEND_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_MAKE_FRIEND_GROUP_REQ )
{
    KDBE_MAKE_FRIEND_GROUP_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_cGroupID = kPacket_.m_cGroupID;
    kPacket.m_wstrGroupName = kPacket_.m_wstrGroupName;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendGroup_INS", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % ( int )kPacket_.m_cGroupID % kPacket_.m_wstrGroupName );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_group_make", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % ( int )kPacket_.m_cGroupID % kPacket_.m_wstrGroupName );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 그룹 만들기 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOGc( kPacket_.m_cGroupID )
            << BUILD_LOG( kPacket_.m_wstrGroupName )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_MAKE_FRIEND_GROUP_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_RENAME_FRIEND_GROUP_REQ )
{
    KDBE_MAKE_FRIEND_GROUP_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_cGroupID = kPacket_.m_cGroupID;
    kPacket.m_wstrGroupName = kPacket_.m_wstrGroupName;
#ifdef SERV_ALL_RENEWAL_SP
	 DO_QUERY( L"exec dbo.P_GFriendGroup_UPD_GroupName", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % ( int )kPacket_.m_cGroupID % kPacket_.m_wstrGroupName );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_group_rename", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % ( int )kPacket_.m_cGroupID % kPacket_.m_wstrGroupName );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 그룹 이름 변경 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOGc( kPacket_.m_cGroupID )
            << BUILD_LOG( kPacket_.m_wstrGroupName )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_RENAME_FRIEND_GROUP_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DELETE_FRIEND_GROUP_REQ )
{
    KDBE_DELETE_FRIEND_GROUP_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_cGroupID = kPacket_.m_cGroupID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriend_UPD_GroupDelete", L"%d, %d", % kPacket_.m_iUnitUID % ( int )kPacket_.m_cGroupID );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_group_delete", L"%d, %d", % kPacket_.m_iUnitUID % ( int )kPacket_.m_cGroupID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 그룹 삭제 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( LAST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_iUnitUID )
            << BUILD_LOGc( kPacket_.m_cGroupID )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_MESSENGER_10;
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_DELETE_FRIEND_GROUP_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_FRIEND_MESSAGE_NOT )
{
    int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendMessage_INS", L"%d, N\'%s\', %d, N\'%s\'", % kPacket_.m_iSenderUnitUID % kPacket_.m_wstrReceiverNickName % ( int )kPacket_.m_cMessageType % kPacket_.m_wstrMessage );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.gup_friend_send_message", L"%d, N\'%s\', %d, N\'%s\'", % kPacket_.m_iSenderUnitUID % kPacket_.m_wstrReceiverNickName % ( int )kPacket_.m_cMessageType % kPacket_.m_wstrMessage );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();
    }

    if( iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"친구 메세지 남기기 실패." )
            << BUILD_LOG( iOK )
            << BUILD_LOG( kPacket_.m_iSenderUnitUID )
            << BUILD_LOG( kPacket_.m_wstrReceiverNickName )
            << BUILD_LOGc( kPacket_.m_cMessageType )
            << BUILD_LOG( kPacket_.m_wstrMessage )
            << END_LOG;
    }

end_proc:
    return;
}

//{{ 2009. 7. 29  최육사	item set cheat
IMPL_ON_FUNC( DBE_ADMIN_GET_ITEM_SET_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.gup_insert_patchitem", L"N\'%s\'", % kPacket_.m_wstrNickName );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"아이템 세트 치트 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_wstrNickName )
			<< END_LOG;
	}

end_proc:
	return;
}
//}}

//{{ 2009. 8. 27  최육사	봉인
IMPL_ON_FUNC( DBE_SEAL_ITEM_REQ )
{
	KDBE_SEAL_ITEM_ACK kPacket;
	kPacket.m_ucSealResult			  = kPacket_.m_ucSealResult;	
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSeal_MER", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_ucSealResult );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_ucSealResult );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"봉인 DB 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOGc( kPacket_.m_ucSealResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SEAL_ITEM_16;
		goto end_proc;
	}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iItemUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_SEAL_ITEM_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UNSEAL_ITEM_REQ )
{
	KDBE_UNSEAL_ITEM_ACK kPacket;
	kPacket.m_ucSealResult			  = kPacket_.m_ucSealResult;	
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSeal_MER", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_ucSealResult );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_ucSealResult );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"봉인해제 DB 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOGc( kPacket_.m_ucSealResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SEAL_ITEM_16;
		goto end_proc;
	}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iItemUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_UNSEAL_ITEM_ACK, kPacket );
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
#else
IMPL_ON_FUNC( DBE_CREATE_GUILD_REQ )
{
	KDBE_CREATE_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iItemUID = kPacket_.m_iItemUID;

	// 1. 길드 창단 가능한지 체크
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuildMember_SEL_Check", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrGuildName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild_check", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrGuildName );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( clog, L"길드 생성할 수 없는것으로 체크됨!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrGuildName );

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_00; break; // 이미 나는 길드에 가입중
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_01; break; // 길드 이름 중복
			//{{ 2009. 10. 26  최육사	길드
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_42; break; // 이 길드 이름은 7일간 사용할 수 없습니다.
			//}}
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		kPacket.m_bReqGiant_DeleteName = true; 
		kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		goto end_proc;
	}

	// 2. 길드 생성
#ifdef SERV_CREATE_GUILD_EVENT
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_INS", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 30
		% kPacket_.m_wstrGuildMessage
		); // 초기인원 30명으로
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 30
		% kPacket_.m_wstrGuildMessage
		); // 초기인원 30명으로
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_CREATE_GUILD_EVENT
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_INS", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 20
		% kPacket_.m_wstrGuildMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 20
		% kPacket_.m_wstrGuildMessage
		);
#endif //SERV_ALL_RENEWAL_SP
#endif //SERV_CREATE_GUILD_EVENT

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kCreatedGuildInfo.m_iGuildUID
			>> kPacket.m_kCreatedGuildInfo.m_wstrFoundingDay );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 생성 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrGuildName )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_04; break; // 길드 창단 에러
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_05; break; // 생성자가 길드원 가입 중 에러			
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		kPacket.m_bReqGiant_DeleteName = true; 
		kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		goto end_proc;
	}
	else
	{
		// 창단 성공하면 초기 길드 정보 세팅
		kPacket.m_kCreatedGuildInfo.m_wstrGuildName		= kPacket_.m_wstrGuildName;
#ifdef SERV_CREATE_GUILD_EVENT
		kPacket.m_kCreatedGuildInfo.m_usMaxNumMember	= 30;
#else //SERV_CREATE_GUILD_EVENT
		kPacket.m_kCreatedGuildInfo.m_usMaxNumMember	= 20;
#endif //SERV_CREATE_GUILD_EVENT
		kPacket.m_kCreatedGuildInfo.m_ucGuildLevel		= 1;
		kPacket.m_kCreatedGuildInfo.m_iGuildEXP			= 0;
		kPacket.m_kCreatedGuildInfo.m_wstrGuildMessage	= kPacket_.m_wstrGuildMessage;
	}

	//{{ 2009. 12. 3  최육사	길드스킬
	// 3. 길드 스킬 포인트
#ifdef GUILD_SKILL_TEST

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER", L"%d, %d",
		% kPacket.m_kCreatedGuildInfo.m_iGuildUID
		% 1
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_skill_point", L"%d, %d",
		% kPacket.m_kCreatedGuildInfo.m_iGuildUID
		% 1
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 생성 직후에 스킬 포인트 1포인트 insert가 실패하였다. 있을수 없는에러!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket.m_kCreatedGuildInfo.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}
	else
	{
		// 길드 스킬 포인트 초기값은 1포인트다.
		kPacket.m_kGuildSkillInfo.m_iGuildSPoint = 1;
	}

#endif GUILD_SKILL_TEST
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CREATE_GUILD_ACK, kPacket );
}

#endif SERV_GUILD_CHANGE_NAME
//}}
_IMPL_ON_FUNC( DBE_INVITE_GUILD_NICKNAME_CHECK_REQ, KELG_INVITE_GUILD_ACK )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#endif //SERV_ALL_RENEWAL_SP
	UidType iUnitUID = 0;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iUnitUID );
		m_kODBC.EndFetch();
	}

	if( iUnitUID != 0 )
	{
		kPacket_.m_iOK = NetError::ERR_GUILD_08; // 현재 오프라인인 유저입니다.
	}
	else
	{
		kPacket_.m_iOK = NetError::ERR_SEARCH_UNIT_04; // 존재하지 않는 닉네임입니다.
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INVITE_GUILD_NICKNAME_CHECK_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_JOIN_GUILD_REQ )
{
	KDBE_JOIN_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_kJoinGuildMember = kPacket_.m_kJoinGuildMember;	

	//////////////////////////////////////////////////////////////////////////
	// 길드 가입
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuildMember_INS", L"%d, %d, N\'%s\'", 
		% kPacket_.m_kJoinGuildMember.m_iUnitUID
		% kPacket_.m_iGuildUID
		% L""
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild_member", L"%d, %d, N\'%s\'", 
		% kPacket_.m_kJoinGuildMember.m_iUnitUID
		% kPacket_.m_iGuildUID
		% L""
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
	
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( kPacket.m_iOK == -3 )
		{
			START_LOG( cwarn, L"길드 최대 가입 인원 부족." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_kJoinGuildMember.m_iUnitUID )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"길드 가입 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_kJoinGuildMember.m_iUnitUID )
				<< END_LOG;
		}

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_14; break; // 이미 길드에 가입되어있음
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_18; break; // 길드 가입 실패
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_18; break; // 길드 가입 최대 인원 제한
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}
	}

end_proc:	
	SendToServer( DBE_JOIN_GUILD_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_GRADE_REQ )
{
	KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_iTargetUnitUID = kPacket_.m_iTargetUnitUID;
	kPacket.m_wstrTargetNickName = kPacket_.m_wstrTargetNickName;
	kPacket.m_ucMemberShipGrade = kPacket_.m_ucMemberShipGrade;	
	kPacket.m_bChangeGuildMaster = kPacket_.m_bChangeGuildMaster; // 반드시 넘겨줘야하는 값!
	
	//////////////////////////////////////////////////////////////////////////
	// 등급 변경 요청!
	
	if( kPacket_.m_bChangeGuildMaster )
	{
		// 길드 마스터 위임
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_UPD_Master", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTargetUnitUID 
			% (int)SEnum::GUG_NORMAL_USER
			% (int)SEnum::GUG_MASTER
			% kPacket_.m_iGuildUID
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_master", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTargetUnitUID 
			% (int)SEnum::GUG_NORMAL_USER
			% (int)SEnum::GUG_MASTER
			% kPacket_.m_iGuildUID
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드마스터 위임 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // 해당 길드에 속한 유저가 아닙니다.
			case -2: kPacket.m_iOK = NetError::ERR_GUILD_21; break; // 과거 길마 상태변환 실패
			case -3: kPacket.m_iOK = NetError::ERR_GUILD_21; break; // 새 길마 상태변환 실패
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // 길드원 등급 변경 실패
			}
		}
	}
	else
	{
		// 길드 멤버 등급 변경
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Member_UPD_Grade", L"%d, %d, %d",
			% kPacket_.m_iTargetUnitUID 
			% kPacket_.m_iGuildUID
			% kPacket_.m_ucMemberShipGrade 
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_member_grade", L"%d, %d, %d",
			% kPacket_.m_iTargetUnitUID 
			% kPacket_.m_iGuildUID
			% kPacket_.m_ucMemberShipGrade 
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드원 등급 변경 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
				<< BUILD_LOGc( kPacket_.m_ucMemberShipGrade )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // 해당 길드에 속한 유저가 아닙니다.
			case -2: kPacket.m_iOK = NetError::ERR_GUILD_21; break; // 트랜젝션 에러
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // 길드원 등급 변경 실패
			}
		}
	}
	
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 등급 수정이 성공하면 길드 매니저에 수정된 정보를 업데이트 한다.
		SendToLoginServer( ELG_UPDATE_CHANGE_GUILD_MEMBER_GRADE_NOT, kPacket );
	}
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MESSAGE_REQ )
{
	KDBE_CHANGE_GUILD_MESSAGE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_wstrMessage = kPacket_.m_wstrMessage;

	//////////////////////////////////////////////////////////////////////////
	// 1. 길드 메시지 변경을 요청한 유저가 변경권한이 있는지 알아보자!
	u_char ucGuildMemberShipGrade = 0;	

	if( Query_GetGuildMemberGrade( kPacket_.m_iUnitUID, ucGuildMemberShipGrade ) == false )
	{
		START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )			
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
		goto end_proc;
	}

	// 길드 마스터와 길드 관리자만 길드 메시지 변경권한이 있습니다.
	if( ucGuildMemberShipGrade != SEnum::GUG_MASTER  &&
		ucGuildMemberShipGrade != SEnum::GUG_SYSOP )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_22; // 길드 메시지 변경 권한이 없습니다.
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	// 2. 길드 메시지 변경 요청!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Message", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_wstrMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_message", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_wstrMessage
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 메시지 변경 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_wstrMessage )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // 해당 길드에 속한 유저가 아닙니다.
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_23; break; // 트랜젝션 에러
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // 길드 메시지 변경 실패
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_GUILD_MESSAGE_ACK, kPacket );
	
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 길드 메시지 변경이 성공하면 길드 매니저에 수정된 정보를 업데이트 한다.
		SendToLoginServer( ELG_UPDATE_CHANGE_GUILD_MESSAGE_NOT, kPacket );
	}
}

IMPL_ON_FUNC( DBE_KICK_GUILD_MEMBER_REQ )
{
	KDBE_KICK_GUILD_MEMBER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_iTargetUnitUID = kPacket_.m_iTargetUnitUID;

	//////////////////////////////////////////////////////////////////////////
	// 1. 길드관리자가 나를 강퇴하는거라면 강퇴를 요청한 유저가 권한이 있는지 알아보자!
	u_char ucGuildMemberShipGrade = 0;
	
	if( Query_GetGuildMemberGrade( kPacket_.m_iUnitUID, ucGuildMemberShipGrade ) == false )
	{
		START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
		goto end_proc;
	}
	
	// 누군가를 강퇴하는거라면?
	if( kPacket_.m_iUnitUID != kPacket_.m_iTargetUnitUID )
	{
		u_char ucTargetMemberShipGrade = 0;

		if( Query_GetGuildMemberGrade( kPacket_.m_iTargetUnitUID, ucTargetMemberShipGrade ) == false )
		{
			START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )
				<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
			goto end_proc;
		}

		// 길드 마스터를 강퇴 시키려는 거라면?
		if( ucTargetMemberShipGrade == SEnum::GUG_MASTER )
		{
			kPacket.m_iOK = NetError::ERR_GUILD_25; // 길드 마스터는 강퇴 당할 수 없습니다!
			goto end_proc;
		}
			
		// 나에게 강퇴 권한이 있는가?
		switch( ucGuildMemberShipGrade )
		{
		case SEnum::GUG_MASTER:			
			break;

		case SEnum::GUG_SYSOP:
			{
				// 관리자가 다른 관리자를 강퇴 시킬순 없다.
				if( ucTargetMemberShipGrade == SEnum::GUG_SYSOP )
				{
					kPacket.m_iOK = NetError::ERR_GUILD_24; // 길드원 강퇴 권한이 없습니다.
					goto end_proc;
				}
			}
			break;

		default:
			{
				kPacket.m_iOK = NetError::ERR_GUILD_24; // 길드원 강퇴 권한이 없습니다.
				goto end_proc;
			}
			break;
		}
	}
	else
	{
		// 길드 마스터가 탈퇴하려는 거라면?
		if( ucGuildMemberShipGrade == SEnum::GUG_MASTER )
		{
			kPacket.m_iOK = NetError::ERR_GUILD_25; // 길드 마스터는 탈퇴기능을 사용할수 없습니다!
			goto end_proc;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 2. 탈퇴자의 명예 포인트 초기화
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_UPD_EXP", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% 0
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_member_exp", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% 0
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드원 명예 포인트 초기화 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 길드원이 아니거나, 잘못된 길드넘버
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_29; break; // 트랜젝션 에러
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////	
	// 3. 길드원 강퇴 또는 길드탈퇴 요청!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_DEL", L"%d, %d", 
		% kPacket_.m_iTargetUnitUID
		% kPacket_.m_iGuildUID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_guild_member", L"%d, %d", 
		% kPacket_.m_iTargetUnitUID
		% kPacket_.m_iGuildUID
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 탈퇴 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // 해당 길드에 속한 유저가 아닙니다.
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_26; break; // 트랜젝션 에러
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // 길드 탈퇴 실패
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_KICK_GUILD_MEMBER_ACK, kPacket );
	
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 길드원 탈퇴가 성공하면 길드 매니저에 수정된 정보를 업데이트 한다.
		SendToLoginServer( ELG_UPDATE_KICK_GUILD_MEMBER_NOT, kPacket );
	}
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ )
{
	KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iTargetUnitUID = kPacket_.m_iTargetUnitUID;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_wstrMessage = kPacket_.m_wstrMessage;

	// 다른 길드원의 메시지를 변경하는것이라면!
	if( kPacket_.m_iUnitUID != kPacket_.m_iTargetUnitUID )
	{
		// 요청자의 등급을 얻자!
		u_char ucGuildMemberShipGrade = 0;

		if( Query_GetGuildMemberGrade( kPacket_.m_iUnitUID, ucGuildMemberShipGrade ) == false )
		{
			START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )				
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
			goto end_proc;
		}

		// 대상자의 등급을 얻자!
		u_char ucTargerGrade = 0;

		if( Query_GetGuildMemberGrade( kPacket_.m_iTargetUnitUID, ucTargerGrade ) == false )
		{
			START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )				
				<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
			goto end_proc;
		}

		// 등급별 동작 제한
		switch( ucTargerGrade )
		{
		case SEnum::GUG_MASTER:
			{
				START_LOG( cerr, L"다른 누군가가 길드마스터의 인사말을 절대변경할수없다!" )
					<< BUILD_LOG( kPacket_.m_iGuildUID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
					<< END_LOG;

                kPacket.m_iOK = NetError::ERR_GUILD_28;
				goto end_proc;
			}
			break;

		case SEnum::GUG_SYSOP:
			{
				if( ucGuildMemberShipGrade != SEnum::GUG_MASTER )
				{
					kPacket.m_iOK = NetError::ERR_GUILD_28; // 길드 관리자의 인사말은 길드마스터만 수정가능합니다.
					goto end_proc;
				}
			}
			break;

		case SEnum::GUG_OLD_USER:
		case SEnum::GUG_VIP_USER:
		case SEnum::GUG_NORMAL_USER:
		case SEnum::GUG_NEW_USER:
			{
				if( ucGuildMemberShipGrade != SEnum::GUG_MASTER  &&
					ucGuildMemberShipGrade != SEnum::GUG_SYSOP )
				{
					kPacket.m_iOK = NetError::ERR_GUILD_28; // 일반 멤버의 인사말은 길드마스터와 길드관리자만 수정가능합니다.
					goto end_proc;
				}
			}
			break;

		default:
			{
				START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )
					<< BUILD_LOG( kPacket.m_iOK )
					<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
				goto end_proc;
			}
		}
	}

	// 길드원 인사말 변경
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_UPD_Message", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iTargetUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_wstrMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_member_message", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iTargetUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_wstrMessage
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 탈퇴 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_wstrMessage )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // 해당 길드에 속한 유저가 아닙니다.
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_29; break; // 트랜젝션 에러
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // 길드원 인사말 변경 실패
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK, kPacket );
	
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 길드원 인사말 변경이 성공하면 길드 매니저에 수정된 정보를 업데이트 한다.
		SendToLoginServer( ELG_UPDATE_CHANGE_GUILD_MEMBER_MESSAGE_NOT, kPacket );
	}
}

IMPL_ON_FUNC( DBE_DISBAND_GUILD_REQ )
{
	KDBE_DISBAND_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;

	//////////////////////////////////////////////////////////////////////////
	// 1. 길드관리자가 나를 강퇴하는거라면 강퇴를 요청한 유저가 권한이 있는지 알아보자!
	u_char ucGuildMemberShipGrade = 0;

	if( Query_GetGuildMemberGrade( kPacket_.m_iUnitUID, ucGuildMemberShipGrade ) == false )
	{
		START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )			
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
		goto end_proc;
	}

	// 길드 마스터만 길드해산을 할수있다
	if( ucGuildMemberShipGrade != SEnum::GUG_MASTER )
	{
		START_LOG( cerr, L"길드 마스터가 아닌데 길드해산을 하려함!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( ucGuildMemberShipGrade )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_30; // 길드마스터가 아닙니다.
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////	
	// 2. 길드 해산 요청

	//길드 해산
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_UPD_Delete", L"%d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_guild", L"%d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 탈퇴 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )	
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_19; break; // 해당 유닛은 해당 길드원이 아님
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_31; break; // 길드의 길드원이 1명 초과
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_32; break; // 길드원 삭제 실패
		case -4: kPacket.m_iOK = NetError::ERR_GUILD_32; break; // 길드 삭제로 업데이트 실패
		case -5: kPacket.m_iOK = NetError::ERR_GUILD_32; break; // 길드 삭제 로그 남기기 실패
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break; // 길드 해산 실패
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_DISBAND_GUILD_ACK, kPacket );
	
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 길드해산이 성공하면 길드매니저에 해당 길드를 지우러 가자!
		SendToLoginServer( ELG_UPDATE_DISBAND_GUILD_NOT, kPacket );
	}	
}

IMPL_ON_FUNC( DBE_EXPAND_GUILD_MAX_MEMBER_REQ )
{
	KGuildInfo kGuildInfo;
	KDBE_EXPAND_GUILD_MAX_MEMBER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;

	//////////////////////////////////////////////////////////////////////////
	// 길드 마스터인지 확인
	u_char ucGuildMemberShipGrade = 0;

	if( Query_GetGuildMemberGrade( kPacket_.m_iUnitUID, ucGuildMemberShipGrade ) == false )
	{
		START_LOG( cerr, L"길드원 등급 정보 얻기 실패." )			
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_19; // 길드에 속한 유저가 아닙니다.
		goto end_proc;
	}

	// 길드 관리자, 길드 마스터만 길드인원 확장을 할 수 있다.
	if( ucGuildMemberShipGrade != SEnum::GUG_MASTER  &&
		ucGuildMemberShipGrade != SEnum::GUG_SYSOP )
	{
		START_LOG( cerr, L"길드 마스터 또는 길드 관리자가 아닌데 길드 인원 확장을 하려함!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( ucGuildMemberShipGrade )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_40;
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	// 길드 확장 가능한 인원인지 확인
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_info", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kGuildInfo.m_wstrGuildName
			>> kGuildInfo.m_usMaxNumMember
			>> kGuildInfo.m_ucGuildLevel
			>> kGuildInfo.m_iGuildEXP
			>> kGuildInfo.m_wstrGuildMessage
			>> kGuildInfo.m_wstrFoundingDay );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 정보 얻기 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_06;
		goto end_proc;
	}
	
	if( kGuildInfo.m_usMaxNumMember >= KGuildInfo::GUILD_MAX_MEMBER_LIMIT )
	{
		START_LOG( cerr, L"길드 인원을 더이상 확장할 수 없습니다." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_41;
		goto end_proc;
	}
	else
	{
		// 인원 확장!
		kPacket.m_usMaxNumMember = kGuildInfo.m_usMaxNumMember + 10;
		if( kPacket.m_usMaxNumMember > KGuildInfo::GUILD_MAX_MEMBER_LIMIT )
			kPacket.m_usMaxNumMember = KGuildInfo::GUILD_MAX_MEMBER_LIMIT;
	}

	//////////////////////////////////////////////////////////////////////////
	// 길드 인원 확장
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_UPD_Maxno", L"%d, %d", % kPacket_.m_iGuildUID % kPacket.m_usMaxNumMember );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_maxno", L"%d, %d", % kPacket_.m_iGuildUID % kPacket.m_usMaxNumMember );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 인원 확장 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket.m_usMaxNumMember )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_39;
	}

end_proc:
	SendToServer( DBE_EXPAND_GUILD_MAX_MEMBER_ACK, kPacket );
}

//{{ 2009. 10. 27  최육사	길드레벨
IMPL_ON_FUNC( DBE_UPDATE_GUILD_EXP_REQ )
{
	KDBE_UPDATE_GUILD_EXP_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;

	KGuildInfo kGuildInfo;

	//////////////////////////////////////////////////////////////////////////
	// 명예 포인트 업데이트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_UPD_EXP", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_iHonorPoint
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_member_exp", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_iHonorPoint
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드원 명예 포인트 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iHonorPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 길드원이 아니거나, 잘못된 길드넘버
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_29; break; // 트랜젝션 에러
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	// 길드 EXP 얻기	
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_info", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kGuildInfo.m_wstrGuildName
			>> kGuildInfo.m_usMaxNumMember
			>> kGuildInfo.m_ucGuildLevel
			>> kGuildInfo.m_iGuildEXP
			>> kGuildInfo.m_wstrGuildMessage
			>> kGuildInfo.m_wstrFoundingDay );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 정보 얻기 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_06;
		goto end_proc;
	}

	//////////////////////////////////////////////////////////////////////////
	// 길드 제한 레벨 체크
	if( kGuildInfo.m_iGuildEXP >= kPacket_.m_iGuildLimitEXP )
	{
		START_LOG( clog, L"더이상 길드 경험치를 획득할 수 없습니다." )
			<< BUILD_LOG( kGuildInfo.m_iGuildEXP )
			<< BUILD_LOG( kPacket_.m_iGuildLimitEXP );
		
		kPacket.m_iOK = NetError::ERR_GUILD_43;
		goto end_proc;
	}
	else
	{
		// 증가한 경험치
		kPacket.m_iGuildEXP = kGuildInfo.m_iGuildEXP + kPacket_.m_iGuildEXP;
		
		if( kPacket.m_iGuildEXP > kPacket_.m_iGuildLimitEXP )
		{
			kPacket.m_iGuildEXP = kPacket_.m_iGuildLimitEXP;

			// 증분값 다시 얻기
			kPacket_.m_iGuildEXP = kPacket_.m_iGuildLimitEXP - kGuildInfo.m_iGuildEXP;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 길드 EXP 업데이트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_UPD_EXP", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildEXP
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_exp", L"%d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildEXP
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 EXP 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket.m_iGuildEXP )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 길드원이 아니거나, 잘못된 길드넘버
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_29; break; // 트랜젝션 에러
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

end_proc:
	SendToServer( DBE_UPDATE_GUILD_EXP_ACK, kPacket );
}
//}}

#endif GUILD_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( DBE_RESET_GUILD_SKILL_REQ )
{
	KDBE_RESET_GUILD_SKILL_ACK kPacket;
	kPacket.m_iOK						= NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID					= kPacket_.m_iGuildUID;
	kPacket.m_iGuildSkillID				= kPacket_.m_iGuildSkillID;
	kPacket.m_iGuildSkillLevel			= kPacket_.m_iGuildSkillLevel;
	kPacket.m_iGuildSkillCSPoint		= kPacket_.m_iGuildSkillCSPoint;
	kPacket.m_iGuildSPoint				= kPacket_.m_iGuildSPoint;
	kPacket.m_iGuildCSPoint				= kPacket_.m_iGuildCSPoint;		
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( !kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.empty()  ||  !kPacket.m_kItemQuantityUpdate.m_vecDeleted.empty() )
	{
		START_LOG( cerr, L"길드 스킬 초기화 아이템 DB에서 수량감소 혹은 삭제 실패." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_SKILL_08;
		goto end_proc;
	}

	// 스킬 초기화
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Skill_MER", L"%d, %d, %d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildSkillID 
		% kPacket_.m_iGuildSkillLevel
		% kPacket_.m_iGuildSkillCSPoint
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_skill_info_new", L"%d, %d, %d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildSkillID 
		% kPacket_.m_iGuildSkillLevel
		% kPacket_.m_iGuildSkillCSPoint
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 스킬 되돌리기 DB쿼리 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iGuildSkillID )
			<< BUILD_LOG( kPacket_.m_iGuildSkillLevel )
			<< BUILD_LOG( kPacket_.m_iGuildSkillCSPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06;		break; // 존재하지않는 길드
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_10;	break;
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_10;	break;
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN;			break;
		}
		
		goto end_proc;
	}

	// 길드 캐시 스킬 포인트 정보 갱신
	if( kPacket_.m_iGuildCSPoint >= 0 )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_UPD", L"%d, %d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildCSPoint
			% kPacket_.m_iMaxGuildCSPoint
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_cash_skill_point_info", L"%d, %d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildCSPoint
			% kPacket_.m_iMaxGuildCSPoint
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}	

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드 스킬 되돌리기한 이후에 기간제 스킬 포인트 DB업데이트 하다 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iGuildCSPoint )
				<< BUILD_LOG( kPacket_.m_iMaxGuildCSPoint )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
			}

			goto end_proc;
		}
	}
	// 길드 스킬 포인트 정보 갱신
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER", L"%d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildSPoint
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_skill_point", L"%d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildSPoint
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}	

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드 스킬 되돌리기한 이후에 시스킬 포인트 DB업데이트 하다 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iGuildSPoint )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
			}

			goto end_proc;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RESET_GUILD_SKILL_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( ELG_RESET_GUILD_SKILL_DEL_ITEM_ACK, kPacket );
	}
}

IMPL_ON_FUNC( DBE_INIT_GUILD_SKILL_REQ )
{
	KDBE_INIT_GUILD_SKILL_ACK kPacket;
	kPacket.m_iOK				= NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID			= kPacket_.m_iGuildUID;
	kPacket.m_iGuildSPoint		= kPacket_.m_iGuildSPoint;
	kPacket.m_iGuildCSPoint		= kPacket_.m_iGuildCSPoint;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( !kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.empty()  ||  !kPacket.m_kItemQuantityUpdate.m_vecDeleted.empty() )
	{
		START_LOG( cerr, L"길드 스킬 초기화 아이템 DB에서 수량감소 혹은 삭제 실패." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_SKILL_12;
		goto end_proc;
	}
#ifdef SERV_ALL_RENEWAL_SP
	// 길드 스킬 초기화
	DO_QUERY( L"exec dbo.P_GGuild_Skill_DEL", L"%d, %d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildSPoint
		% kPacket_.m_iGuildCSPoint
		);
#else //SERV_ALL_RENEWAL_SP
	// 길드 스킬 초기화
	DO_QUERY( L"exec dbo.gup_delete_all_guild_skill", L"%d, %d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildSPoint
		% kPacket_.m_iGuildCSPoint
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 스킬 초기화 DB쿼리 실패.!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_iGuildSPoint )
			<< BUILD_LOG( kPacket_.m_iGuildCSPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_14; break;
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_14; break;
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INIT_GUILD_SKILL_ACK, kPacket );

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		SendToLoginServer( ELG_INIT_GUILD_SKILL_DEL_ITEM_ACK, kPacket );
	}
}

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 12. 8  최육사	크리스마스이벤트
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
IMPL_ON_FUNC( DBE_CHECK_TIME_EVENT_COMPLETE_REQ )
{
	KDBE_CHECK_TIME_EVENT_COMPLETE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	//////////////////////////////////////////////////////////////////////////	
	// 일단 보상부터 주자!
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_XMAS_EVENT, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"이벤트 보상 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_02;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	CTime tUpdateTime = CTime::GetCurrentTime();
	tUpdateTime += CTimeSpan( 18250, 0, 0, 0 ); // 한번만 줘야 하므로 50년을 더한다.
	std::wstring wstrUpdateDate = ( CStringW )( tUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

	std::vector< KCumulativeTimeEventInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecCompletedEvent.begin(); vit != kPacket_.m_vecCompletedEvent.end(); ++vit )
	{
		//////////////////////////////////////////////////////////////////////////
		// 다시 이벤트 시작할수 있으면 완료 업데이트 하지 말고 이벤트 다시 시작 못하면 완료 업데이트 하자!
		bool bRestartEvent = false;
		int iOK = NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.gup_insert_xmas_event", L"%d, %d", % kPacket_.m_iUnitUID % vit->m_iEventUID );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK <= 0 )
		{
			START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( vit->m_iEventUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_02;
			continue;
		}
		else
		{
			kPacket.m_iRewardCount = iOK;
			if( kPacket.m_iRewardCount >= 5 )
			{
				bRestartEvent = false;
			}
			else
			{
				bRestartEvent = true;
			}

			kPacket.m_iOK = NetError::NET_OK;
		}

		//////////////////////////////////////////////////////////////////////////
		// 이벤트 보상 로그를 기록하자!

		if( bRestartEvent == false )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GIs30Min_MER", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vit->m_iEventUID % wstrUpdateDate );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_30min", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vit->m_iEventUID % wstrUpdateDate );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_iOK );
				m_kODBC.EndFetch();
			}

			if( kPacket.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
					<< BUILD_LOG( kPacket.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( vit->m_iEventUID )
					<< BUILD_LOG( wstrUpdateDate )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_02;
				continue;
			}
		}
		else
		{
			// 이벤트 경과 시간 초기화
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GRemainTime_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % vit->m_iEventUID % 0 );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_remaintime", L"%d, %d, %d", % kPacket_.m_iUnitUID % vit->m_iEventUID % 0 );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( vit->m_iEventUID )					
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_CUMULATIVE_TIME_EVENT_02;
				continue;
			}
			else
			{
				// 재시작할 이벤트를 넣자!
				kPacket.m_vecRestartEvent.push_back( vit->m_iEventUID );
			}
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHECK_TIME_EVENT_COMPLETE_ACK, kPacket );
}
//}}
#endif //SERV_ADD_EVENT_DB

#ifdef SERV_GLOBAL_BILLING
//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
IMPL_ON_FUNC( DBE_GET_NICKNAME_BY_UNITUID_REQ )
{
	KDBE_GET_NICKNAME_BY_UNITUID_ACK kPacket;
	kPacket.m_vecBillOrderInfo = kPacket_.m_vecBillOrderInfo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	kPacket.m_vecPackageInfo = kPacket_.m_vecPackageInfo;
	kPacket.m_vecSubPackageInfo = kPacket_.m_vecSubPackageInfo;
	kPacket.m_vecSubPackageTrans = kPacket_.m_vecSubPackageTrans;
#endif //SERV_GLOBAL_CASH_PACKAGE

	std::vector< UidType >::iterator vit;
	for( vit = kPacket_.m_vecUID.begin(); vit != kPacket_.m_vecUID.end(); vit++ )
	{
		std::wstring wstrNickName;
		UidType	fromUnitUID = *vit;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"%d", % fromUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_nickname", L"%d", % fromUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( wstrNickName );
			m_kODBC.EndFetch();
		}

		START_LOG( clog2, L"[테스트로그]")
			<< BUILD_LOG( fromUnitUID )
			<< BUILD_LOG( wstrNickName )
			<< END_LOG;

		kPacket.m_mapNickName.insert( std::make_pair< UidType, std::wstring >( fromUnitUID, wstrNickName ) );
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_NICKNAME_BY_UNITUID_ACK, kPacket );
}
//}} 허상형 : [2010/8/19/] //	선물 기능 개편
#endif // SERV_GLOBAL_BILLING


//{{ 2010. 01. 11  최육사	추천인리스트
IMPL_ON_FUNC( DBE_GET_RECOMMEND_USER_LIST_REQ )
{
	KDBE_GET_RECOMMEND_USER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	// 추천인 리스트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GRecommend_SEL", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_recommend_list_me", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KRecommendUserInfo kInfo;
		char cUnitClass = 0;

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_ucLevel
			>> cUnitClass
			>> kInfo.m_wstrNickName
			>> kInfo.m_wstrRecommendDate );
		
		kPacket.m_vecRecommendUserList.push_back( kInfo );
	}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_RECOMMEND_USER_LIST_ACK, kPacket );
}
//}}

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

IMPL_ON_FUNC( DBE_ATTENDANCE_CHECK_REQ )
{
	KDBE_ATTENDANCE_CHECK_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	// 출석 체크
	DO_QUERY( L"exec dbo.gup_insert_event_attendance", L"%d", % kPacket_.m_iUnitUID );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( kPacket.m_iOK != -1 )
		{
			START_LOG( cerr, L"출석 체크 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;
		}

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_WEB_POINT_EVENT_00; break;
		case -2: kPacket.m_iOK = NetError::ERR_WEB_POINT_EVENT_02; break;
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ATTENDANCE_CHECK_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_INCREASE_WEB_POINT_LOG_NOT, KDBE_INCREASE_WEB_POINT_ACK )
{	
	int iOK = NetError::ERR_ODBC_01;

	// 웹 포인트 획득 로그
	DO_QUERY( L"exec dbo.gup_insert_event_webpoint", L"%d, %d, %d", % kPacket_.m_iUnitUID % (int)kPacket_.m_cPointType % kPacket_.m_iIncreasePoint );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"웹 포인트 획득 로그 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

end_proc:
	return;
}

#endif SERV_WEB_POINT_EVENT
//}}

//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE

IMPL_ON_FUNC( DBE_EXPAND_SKILL_NOTE_PAGE_REQ )
{
	KDBE_EXPAND_SKILL_NOTE_PAGE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_cExpandedMaxPageNum = kPacket_.m_cExpandedMaxPageNum;

	// 기술의 노트 페이지 확장
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GNote_PageCNT_MER", L"%d, %d", % kPacket_.m_iUnitUID % (int)kPacket_.m_cExpandedMaxPageNum );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_notecnt", L"%d, %d", % kPacket_.m_iUnitUID % (int)kPacket_.m_cExpandedMaxPageNum );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
	
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"기술의 노트 페이지 확장 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_cExpandedMaxPageNum )
			<< END_LOG;
	
		kPacket.m_iOK = NetError::ERR_SKILL_NOTE_05;
	}
	
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_EXPAND_SKILL_NOTE_PAGE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_REG_SKILL_NOTE_MEMO_REQ )
{
	KEGS_REG_SKILL_NOTE_MEMO_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_cSkillNotePageNum = kPacket_.m_cPageNum;
	kPacket.m_iMemoID = kPacket_.m_iMemoID;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;

	// 기술의 노트 메모 등록
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GNote_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iMemoID % (int)kPacket_.m_cPageNum );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_note", L"%d, %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iMemoID % (int)kPacket_.m_cPageNum );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"기술의 노트 메모 등록 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iMemoID )
			<< BUILD_LOGc( kPacket_.m_cPageNum )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SKILL_NOTE_03;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_REG_SKILL_NOTE_MEMO_ACK, kPacket );
}

#endif SERV_SKILL_NOTE
//}}

//{{ 2010. 7. 30 최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( DBE_CREATE_PET_REQ )
{
	KDBE_CREATE_PET_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_kPetInfo = kPacket_.m_kPetInfo;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		BOOST_MAP_CONST_FOREACH( UidType, int, kItem, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( kItem.first )
				<< BUILD_LOG( kItem.second );
		}
	}
	//}}

	// 펫 생성
#ifdef SERV_PERIOD_PET
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_INS", L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_kPetInfo.m_iPetID					// SERV_PETID_DATA_TYPE_CHANGE
		% kPacket_.m_kPetInfo.m_wstrPetName
		% (int)kPacket_.m_kPetInfo.m_cEvolutionStep
		% kPacket_.m_kPetInfo.m_sSatiety
		% kPacket_.m_kPetInfo.m_iIntimacy
		% kPacket_.m_kPetInfo.m_sExtroversion
		% kPacket_.m_kPetInfo.m_sEmotion
		% kPacket_.m_sPeriod
		);
#else //SERV_ALL_RENEWAL_SP
	// SERV_PET_AUTO_LOOTING, SERV_PETID_DATA_TYPE_CHANGE 켜진 것 기준으로 하나만 제작
	DO_QUERY( L"exec dbo.gup_create_pet", L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_kPetInfo.m_iPetID					// SERV_PETID_DATA_TYPE_CHANGE
		% kPacket_.m_kPetInfo.m_wstrPetName
		% (int)kPacket_.m_kPetInfo.m_cEvolutionStep
		% kPacket_.m_kPetInfo.m_sSatiety
		% kPacket_.m_kPetInfo.m_iIntimacy
		% kPacket_.m_kPetInfo.m_sExtroversion
		% kPacket_.m_kPetInfo.m_sEmotion
		% kPacket_.m_sPeriod
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_PERIOD_PET

#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_INS", L"%d, %d, N\'%s\', %d, %d, %d, %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_kPetInfo.m_iPetID					// SERV_PETID_DATA_TYPE_CHANGE
		% kPacket_.m_kPetInfo.m_wstrPetName
		% (int)kPacket_.m_kPetInfo.m_cEvolutionStep
		% kPacket_.m_kPetInfo.m_sSatiety
		% kPacket_.m_kPetInfo.m_iIntimacy
		% kPacket_.m_kPetInfo.m_sExtroversion
		% kPacket_.m_kPetInfo.m_sEmotion
		% 0
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_pet", L"%d, %d, N\'%s\', %d, %d, %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_kPetInfo.m_iPetID					// SERV_PETID_DATA_TYPE_CHANGE
		% kPacket_.m_kPetInfo.m_wstrPetName
		% (int)kPacket_.m_kPetInfo.m_cEvolutionStep
		% kPacket_.m_kPetInfo.m_sSatiety
		% kPacket_.m_kPetInfo.m_iIntimacy
		% kPacket_.m_kPetInfo.m_sExtroversion
		% kPacket_.m_kPetInfo.m_sEmotion
		);	
#endif //SERV_ALL_RENEWAL_SP		
#else //SERV_PETID_DATA_TYPE_CHANGE
	DO_QUERY( L"exec dbo.gup_create_pet", L"%d, %d, N\'%s\', %d, %d, %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% (int)kPacket_.m_kPetInfo.m_cPetID
		% kPacket_.m_kPetInfo.m_wstrPetName
		% (int)kPacket_.m_kPetInfo.m_cEvolutionStep
		% kPacket_.m_kPetInfo.m_sSatiety
		% kPacket_.m_kPetInfo.m_iIntimacy
		% kPacket_.m_kPetInfo.m_sExtroversion
		% kPacket_.m_kPetInfo.m_sEmotion
		);
#endif //SERV_PETID_DATA_TYPE_CHANGE

#endif SERV_PERIOD_PET

	if( m_kODBC.BeginFetch() )
	{
#ifdef SERV_PERIOD_PET
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kPetInfo.m_iPetUID
			>> kPacket.m_kPetInfo.m_wstrDestroyDate
			);
#else SERV_PERIOD_PET
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kPetInfo.m_iPetUID
			);
#endif SERV_PERIOD_PET

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"펫 생성 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
#ifndef SERV_PRIVACY_AGREEMENT
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_wstrPetName )
#endif SERV_PRIVACY_AGREEMENT
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cEvolutionStep )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_sSatiety )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_iIntimacy )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_sExtroversion )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_sEmotion )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_00;
		goto end_proc;
	}
#ifdef SERV_EVENT_PET_INVENTORY
	//여기서 이벤트 펫 먹이 정보 등록 
	//이벤트 용이기 떄문에 기본 컨텐츠 정보에 넣을 필요는 없다 
	//여기는 이벤트 펫만 등록을 하면 된다.
	if( kPacket_.m_kPetInfo.m_bIsEventPetID == true )
	{
		DO_QUERY( L"exec dbo.P_GPetEvent_INT", L"%d, %d, N\'%s\', %d, %d", 
			% kPacket_.m_iUnitUID
			% kPacket.m_kPetInfo.m_iPetUID
			% kPacket_.m_kPetInfo.m_iPetID					// SERV_PETID_DATA_TYPE_CHANGE
			% kPacket_.m_kPetInfo.m_wstrPetName
			% kPacket_.m_kPetInfo.m_bEventFoodEat
			);
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK);
			m_kODBC.EndFetch();
		}
		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"이벤트용 펫 DB정보 등록 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_kPetInfo.m_iPetID )
				<< BUILD_LOG( kPacket_.m_kPetInfo.m_wstrPetName )
				<< BUILD_LOG( kPacket_.m_kPetInfo.m_iPetUID )
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_PET_00;
			goto end_proc;
		}
	}
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_FREE_AUTO_LOOTING
	if( kPacket.m_kPetInfo.m_bFreeAutoLooting == true )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPet_Info_UPD_PickUp", L"%d", % kPacket.m_kPetInfo.m_iPetUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPet_Info_PickUP_UPT", L"%d", % kPacket.m_kPetInfo.m_iPetUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"펫의 오토 루팅 기능 DB 업데이트에 실패 하였습니다." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket.m_kPetInfo.m_iPetUID )
				<< END_LOG;
			kPacket.m_kPetInfo.m_bFreeAutoLooting = false;
			goto end_proc;
		}
	}
#endif SERV_FREE_AUTO_LOOTING

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CREATE_PET_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_SUMMON_PET_REQ )
{
	KDBE_SUMMON_PET_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iBeforeSummonPetUID = kPacket_.m_iBeforeSummonPetUID;
	kPacket.m_iSummonPetUID = kPacket_.m_iSummonPetUID;

	//////////////////////////////////////////////////////////////////////////
	// 인벤토리 변경 데이터 업데이트 ( 소환 해제되는 펫의 인벤 정보도 포함 된것 ) 

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );	
	LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
	Query_UpdateItemPosition( kPacket_.m_iUnitUID, kPacket_.m_kItemPositionUpdate, kPacket.m_kItemPositionUpdate );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		BOOST_MAP_CONST_FOREACH( UidType, int, kItem, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( kItem.first )
				<< BUILD_LOG( kItem.second );
		}
	}
	//}}
    
	//////////////////////////////////////////////////////////////////////////
	// 소환 해제!
	if( kPacket_.m_iBeforeSummonPetUID > 0 )
	{
		// 펫 소환 해제
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPet_Info_UPD_Call", L"%d, %d", % kPacket_.m_iBeforeSummonPetUID % 0 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_pet_call", L"%d, %d", % kPacket_.m_iBeforeSummonPetUID % 0 );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"펫 소환 해제 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iBeforeSummonPetUID )
				<< END_LOG;

			// 펫 소환 해제 실패하면 여기서 돌아가자!
			kPacket.m_iOK = NetError::ERR_PET_08;
			goto end_proc;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 펫 소환!
	if( kPacket_.m_iSummonPetUID > 0 )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPet_Info_UPD_Call", L"%d, %d", % kPacket_.m_iSummonPetUID % 1 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_pet_call", L"%d, %d", % kPacket_.m_iSummonPetUID % 1 );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"펫 소환 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iSummonPetUID )
				<< END_LOG;

			// 펫 소환 실패하면 여기서 돌아가자!
			kPacket.m_iOK = NetError::ERR_PET_04;
			goto end_proc;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_SUMMON_PET_ACK, kPacket );
}

//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
IMPL_ON_FUNC( DBE_CHANGE_PET_NAME_REQ )
{
	KDBE_CHANGE_PET_NAME_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iItemUID = kPacket_.m_iItemUID;
	kPacket.m_kPetInfo = kPacket_.m_kPetInfo;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		BOOST_MAP_CONST_FOREACH( UidType, int, kItem, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( kItem.first )
				<< BUILD_LOG( kItem.second );
		}
	}
	//}}

	if( kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.find( kPacket_.m_iItemUID ) != kPacket.m_kItemQuantityUpdate.m_mapQuantityChange.end() )
	{
		START_LOG( cerr, L"DBE_CHANGE_GUILD_NAME_REQ: 수량 감소 실패" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_18;
		goto end_proc;
	}
	else
	{
		BOOST_TEST_FOREACH( const KDeletedItemInfo&, kDeletedItemInfo, kPacket.m_kItemQuantityUpdate.m_vecDeleted )
		{
			if( kDeletedItemInfo.m_iItemUID == kPacket_.m_iItemUID )
			{
				START_LOG( cerr, L"DBE_CHANGE_GUILD_NAME_REQ: 아이템 삭제 실패" )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iItemUID )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_ITEM_18;
				goto end_proc;
			}
		}
	}

	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	// 펫이름 변경
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_UPD_NickName", L"%d, N\'%s\'", 
		% kPacket_.m_kPetInfo.m_iPetUID
		% kPacket_.m_kPetInfo.m_wstrPetName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_UPT", L"%d, N\'%s\'", 
		% kPacket_.m_kPetInfo.m_iPetUID
		% kPacket_.m_kPetInfo.m_wstrPetName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"펫 이름 변경 실패! ( 아이템 복구 이슈 발생 )" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE //2013.07.02
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_iPetID )
#else //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_wstrPetName )
			<< BUILD_LOGc( kPacket_.m_kPetInfo.m_cEvolutionStep )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_sSatiety )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_iIntimacy )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_sExtroversion )
			<< BUILD_LOG( kPacket_.m_kPetInfo.m_sEmotion )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PET_19;
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_PET_NAME_ACK, kPacket );
}
#endif SERV_PET_CHANGE_NAME
//}}

#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

IMPL_ON_FUNC( DBE_RESET_PERIOD_ITEM_REQ )
{
	KDBE_RESET_PERIOD_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	std::map< UidType, short >::const_iterator mit;
	for( mit = kPacket_.m_mapExpandPeriodItemList.begin(); mit != kPacket_.m_mapExpandPeriodItemList.end(); ++mit )
	{
		KItemPeriodInfo kInfo;

		DO_QUERY( L"exec dbo.gup_update_item_period", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kInfo.m_wstrExpirationDate );

			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK == NetError::NET_OK )
		{
			kInfo.m_iItemUID = mit->first;
			kInfo.m_sPeriod = mit->second;
			kPacket.m_vecItemPeriodInfo.push_back( kInfo );
		}
		else
		{
			START_LOG( cerr, L"기간 초기화 아이템 DB업데이트 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second )
				<< END_LOG;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RESET_PERIOD_ITEM_ACK, kPacket );
}

#endif SERV_RESET_PERIOD_EVENT
//}}

//{{ 2011. 01. 04	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM

IMPL_ON_FUNC( DBE_ATTRIB_ATTACH_ITEM_REQ )
{
	KDBE_ATTRIB_ATTACH_ITEM_ACK kPacket;
	kPacket.m_iItemUID					= kPacket_.m_iItemUID;
	kPacket.m_kAttribEnchantInfo		= kPacket_.m_kAttribEnchantInfo;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	
	{
		// 첫번째 슬롯
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemAttribute_MER", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iItemUID
			% CXSLAttribEnchantItem::ESI_SLOT_1
			% static_cast<int>(kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant0)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_Attribute", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iItemUID
			% CXSLAttribEnchantItem::ESI_SLOT_1
			% static_cast<int>(kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant0)
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"0번 슬롯 속성 부여 업데이트 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant0 )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant1 )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant2 )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}
	}

	{
		// 두번째 슬롯
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemAttribute_MER", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iItemUID
			% CXSLAttribEnchantItem::ESI_SLOT_2
			% static_cast<int>(kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant1)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_Attribute", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iItemUID
			% CXSLAttribEnchantItem::ESI_SLOT_2
			% static_cast<int>(kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant1)
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"1번 슬롯 속성 부여 업데이트 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant0 )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant1 )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant2 )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}
	}

	{
		// 세번째 슬롯
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItemAttribute_MER", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iItemUID
			% CXSLAttribEnchantItem::ESI_SLOT_3
			% static_cast<int>(kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant2)
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_Attribute", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iItemUID
			% CXSLAttribEnchantItem::ESI_SLOT_3
			% static_cast<int>(kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant2)
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"2번 슬롯 속성 부여 업데이트 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemUID )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant0 )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant1 )
				<< BUILD_LOGc( kPacket_.m_kAttribEnchantInfo.m_cAttribEnchant2 )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
		}
	}

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ATTRIB_ATTACH_ITEM_ACK, kPacket );
}

#endif SERV_ATTRIBUTE_CHARM
//}}

//{{ 2011. 04. 14	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY

IMPL_ON_FUNC( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ )
{
	KDBE_INSERT_PERIOD_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_usEventID = kPacket_.m_usEventID;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_cShopType = kPacket_.m_cShopType;

	// 대리 상인 생성
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPshopInfo_MER", L"%d, %d, %d, %d", 
		% LAST_SENDER_UID
		% kPacket_.m_iUnitUID
		% kPacket_.m_sAgencyPeriod
		% static_cast<int>(kPacket_.m_cShopType)
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPshopInfo_INT", L"%d, %d, %d, %d", 
		% LAST_SENDER_UID
		% kPacket_.m_iUnitUID
		% kPacket_.m_sAgencyPeriod
        % static_cast<int>(kPacket_.m_cShopType)
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrAgencyExpirationDate
			);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상인 기간 설정 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_sAgencyPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_43;
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_PERIOD_PSHOP_AGENCY_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ )
{
	KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_wstrPersonalShopName		= kPacket_.m_wstrPersonalShopName;
	kPacket.m_vecSellItemInfo			= kPacket_.m_vecSellItemInfo;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	SendToUser( LAST_SENDER_UID, DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_ACK, kPacket );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
IMPL_ON_FUNC( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_mapInsertItem				= kPacket_.m_mapInsertItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_vecSellItemInfo			= kPacket_.m_vecSellItemInfo;
    kPacket.m_bRemainSellItem           = kPacket_.m_bRemainSellItem;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );

	// 	START_LOG( cerr, L"삭제 누가 호출 하나" )
	// 		<< BUILD_LOG( kPacket_.m_iUnitUID )
	// 		<< END_LOG;

	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_PICK_UP_PSHOP, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
		//}}
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
	//}}

	SendToUser( LAST_SENDER_UID, DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
}
#else
IMPL_ON_FUNC( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_mapInsertItem				= kPacket_.m_mapInsertItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_vecSellItemInfo			= kPacket_.m_vecSellItemInfo;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_PICK_UP_PSHOP, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo, false ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo, false ) == false )
#endif SERV_GET_ITEM_REASON
	//}}
	{
		START_LOG( cerr, L"대리 상인 Pick Up 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_47;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	SendToUser( LAST_SENDER_UID, DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

#endif SERV_PSHOP_AGENCY
//}}

#ifdef	SERV_SHARING_BANK_TEST
_IMPL_ON_FUNC( DBE_GET_SHARE_BANK_REQ, KEGS_GET_SHARE_BANK_REQ )
{
	KEGS_GET_SHARE_BANK_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
			kAck.m_wstrNickName = kPacket_.m_wstrNickName;
#else // SERV_NEW_UNIT_TRADE_LIMIT
		kAck.m_wstrNickName = kPacket_;
#endif // SERV_NEW_UNIT_TRADE_LIMIT

	//	UnitUID 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kAck.m_wstrNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kAck.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iUnitUID );
		m_kODBC.EndFetch();
	}

	if( kAck.m_iUnitUID != 0 )
	{
		kAck.m_iOK = NetError::NET_OK;
	}
	else
	{
		kAck.m_iOK = NetError::ERR_KNM_02;
		goto end_proc;
	}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	if( kPacket_.m_iNewUnitTradeBlockDay != 0 )
	{
		std::wstring strUnitCreateDate;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnit_SEL_RegDate", L"N\'%d\'", % kAck.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_select_unit", L"N\'%d\'", % kAck.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK 
				>> strUnitCreateDate
				);
			m_kODBC.EndFetch();
		}

		if( kAck.m_iOK != NetError::NET_OK )
		{
			kAck.m_iOK = NetError::ERR_KNM_02;
			goto end_proc;
		}

		CTime tReleaseTradeBlockTime;
		LIF( KncUtil::ConvertStringToCTime( strUnitCreateDate, tReleaseTradeBlockTime ) );
		tReleaseTradeBlockTime += CTimeSpan( kPacket_.m_iNewUnitTradeBlockDay, 0, 0, 0 );
		CTime tCurTime	 = CTime::GetCurrentTime();

		if( tCurTime < tReleaseTradeBlockTime  )
		{
			kAck.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
			goto end_proc;
		}
	}

	if( kPacket_.m_iNewUnitTradeBlockUnitClass != 0 )
	{
		int iClass;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnit_SEL_UnitClass", L"%d", % kAck.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_unitclass_by_unituid", L"%d", % kAck.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iClass );
			m_kODBC.EndFetch();
		}
		else
		{
			kAck.m_iOK = NetError::ERR_ADMIN_COMMAND_01;
			goto end_proc;
		}

		if( iClass < kPacket_.m_iNewUnitTradeBlockUnitClass )
		{
			kAck.m_iOK = NetError::ERR_TRADE_BLOCK_UNIT_02;
			goto end_proc;
		}
	}
#endif SERV_NEW_UNIT_TRADE_LIMIT

	//	은행 크기 얻기
	DO_QUERY( L"exec dbo.P_GItemInventorySize_SEL_Bank", L"%d", % kAck.m_iUnitUID );

	int iBankSize = 0;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iBankSize );
		m_kODBC.EndFetch();
	}

	kAck.m_iBankSize = iBankSize + CXSLInventory::MPB_NORMAL;

	if( kAck.m_iBankSize <= 0 )
	{
		kAck.m_iOK = NetError::ERR_TRADE_09;
		goto end_proc;
	}

	//	은행아이템 얻어오기
#ifdef SERV_SHARE_BANK_ITEM_EVALUATE_FIX // 김석근 // 적용날짜 : 2013-08-28
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItem_SEL_BankNew", L"%d", % kAck.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItem_GET_BankItem_New", L"%d", % kAck.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_SHARE_BANK_ITEM_EVALUATE_FIX
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItem_SEL_Bank", L"%d", % kAck.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItem_GET_BankItem", L"%d", % kAck.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#endif ////SERV_SHARE_BANK_ITEM_EVALUATE_FIX
	while( m_kODBC.Fetch() )
	{
		int iEnchantLevel = 0;
#ifdef	SERV_ITEM_OPTION_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS
		int arrSocketOption[5] = {0,0,0,0,0};
#else // SERV_BATTLE_FIELD_BOSS
		int arrSocketOption[4] = {0,0,0,0};
#endif // SERV_BATTLE_FIELD_BOSS
#else	
		short arrSocketOption[4] = {0,0,0,0};
#endif	SERV_ITEM_OPTION_DATA_SIZE
		KInventoryItemInfo kInventoryItemInfo;

#ifdef SERV_SHARE_BANK_ITEM_EVALUATE_FIX // 김석근 // 적용날짜 : 2013-08-28
		int arrRandomSocketOption[5] = {0};

#ifdef SERV_BATTLE_FIELD_BOSS
		FETCH_DATA( kInventoryItemInfo.m_iItemUID
			>> kInventoryItemInfo.m_kItemInfo.m_iItemID
			>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
			>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
			>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
			>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
			>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
			>> iEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> arrSocketOption[4]
			>> kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
			>> arrRandomSocketOption[0]
			>> arrRandomSocketOption[1]
			>> arrRandomSocketOption[2]
			>> arrRandomSocketOption[3]
			>> arrRandomSocketOption[4]
			>> kInventoryItemInfo.m_cSlotCategory
				>> kInventoryItemInfo.m_sSlotID );
#else // SERV_BATTLE_FIELD_BOSS
		FETCH_DATA( kInventoryItemInfo.m_iItemUID
			>> kInventoryItemInfo.m_kItemInfo.m_iItemID
			>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
			>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
			>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
			>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
			>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
			>> iEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> arrRandomSocketOption[0]
			>> arrRandomSocketOption[1]
			>> arrRandomSocketOption[2]
			>> arrRandomSocketOption[3]
			>> arrRandomSocketOption[4]
			>> kInventoryItemInfo.m_cSlotCategory
			>> kInventoryItemInfo.m_sSlotID );
#endif // SERV_BATTLE_FIELD_BOSS

			int iCheckRandomIdx;
			for( iCheckRandomIdx = 4; iCheckRandomIdx >= 0; --iCheckRandomIdx )
			{
				if( arrRandomSocketOption[iCheckRandomIdx] != 0 )
					break;
			}

			for( int iIdx = 0; iIdx <= iCheckRandomIdx; ++iIdx )
			{
				kInventoryItemInfo.m_kItemInfo.m_vecRandomSocket.push_back( arrRandomSocketOption[iIdx] );
			}

#else //SERV_SHARE_BANK_ITEM_EVALUATE_FIX

		FETCH_DATA( kInventoryItemInfo.m_iItemUID
			>> kInventoryItemInfo.m_kItemInfo.m_iItemID
			>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
			>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
			>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
			>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
			>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
			>> iEnchantLevel
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> kInventoryItemInfo.m_cSlotCategory
			>> kInventoryItemInfo.m_sSlotID );

			/*
			START_LOG(cout2, L"지헌로그:타입 값 확인 해봅시다.")
				<< BUILD_LOG(kInventoryItemInfo.m_iItemUID)
				<< BUILD_LOG(kInventoryItemInfo.m_kItemInfo.m_iItemID)
				<< BUILD_LOG(kInventoryItemInfo.m_kItemInfo.m_iQuantity)
				<< BUILD_LOG(kInventoryItemInfo.m_kItemInfo.m_sEndurance)
				<< BUILD_LOG(kInventoryItemInfo.m_kItemInfo.m_sPeriod)
				<< BUILD_LOGc( kInventoryItemInfo.m_kItemInfo.m_cUsageType )
				<< END_LOG;
				*/
#endif //SERV_SHARE_BANK_ITEM_EVALUATE_FIX

			// 1. 강화 정보 업데이트
			kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel = static_cast<char>(iEnchantLevel);

			// 2. 소켓 정보 업데이트
			int iCheckIdx;
			for( iCheckIdx = 3; iCheckIdx >= 0; --iCheckIdx )
			{
				if( arrSocketOption[iCheckIdx] != 0 )
					break;
			}

			for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
			{
				kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );
			}

			if( kInventoryItemInfo.m_cSlotCategory != CXSLInventory::ST_BANK )
			{
				START_LOG( cerr, L"은행 공유 : 은행 카테고리가 아닌데 들어옴" )
					<< BUILD_LOG( kAck.m_iUnitUID )
					<< BUILD_LOG( kInventoryItemInfo.m_iItemUID )
					<< BUILD_LOG( kInventoryItemInfo.m_cSlotCategory )
					<< END_LOG;

				continue;
			}
			// Type을 공유은행으로 바꿔준다
			kInventoryItemInfo.m_cSlotCategory = CXSLInventory::ST_SHARE_BANK;

			// 아이템 UID가 PK이므로 map에 insert 할 때 키 중복 여부 검사를 할 필요가 없다.
			kAck.m_mapItem.insert( std::make_pair( kInventoryItemInfo.m_iItemUID, kInventoryItemInfo ) );
	}

	//	Attribute 정보 얻어오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemAttribute_SEL_Bank", L"%d", % kAck.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemAttribute_SEL_BankItem", L"%d", % kAck.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		UidType iItemUID = 0;
		int iAttribEnchantSlotNo = 0;
		int iAttribEnchantID = 0;
		std::map< UidType, KInventoryItemInfo >::iterator mitItem;

		FETCH_DATA( iItemUID
			>> iAttribEnchantSlotNo
			>> iAttribEnchantID );


		mitItem = kAck.m_mapItem.find( iItemUID );
		if( mitItem != kAck.m_mapItem.end() )
		{
			switch( iAttribEnchantSlotNo )
			{
			case CXSLAttribEnchantItem::ESI_SLOT_1:
				mitItem->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 = static_cast<char>( iAttribEnchantID );
				break;

			case CXSLAttribEnchantItem::ESI_SLOT_2:
				mitItem->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 = static_cast<char>( iAttribEnchantID );
				break;

			case CXSLAttribEnchantItem::ESI_SLOT_3:
				mitItem->second.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 = static_cast<char>( iAttribEnchantID );
				break;

			default:
				START_LOG( cerr, L"은행 공유 : 속성 강화 슬롯 넘버가 이상합니다." )
					<< BUILD_LOG( kAck.m_iUnitUID )
					<< BUILD_LOG( iItemUID )
					<< BUILD_LOG( iAttribEnchantSlotNo )
					<< BUILD_LOG( iAttribEnchantID )
					<< END_LOG;
				break;
			}
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 속성정보는 있는데 아이템은 없다?" )
				<< BUILD_LOG( kAck.m_iUnitUID )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iAttribEnchantSlotNo )
				<< BUILD_LOG( iAttribEnchantID )
				<< END_LOG;
		}
	}

	//	봉인 정보 얻어오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSeal_SEL_Bank", L"%d", % kAck.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSeal_SEL_BankItem", L"%d", % kAck.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		UidType iItemUID = 0;
		int iSealCnt = 0;
		std::map< UidType, KInventoryItemInfo >::iterator mitItem;

		FETCH_DATA( iItemUID
			>> iSealCnt );

		mitItem = kAck.m_mapItem.find( iItemUID );
		if( mitItem != kAck.m_mapItem.end() )
		{
			mitItem->second.m_kItemInfo.m_ucSealData = iSealCnt;
		}
		else
		{
			START_LOG( cerr, L"은행 공유 : 봉인정보는 있는데 아이템은 없다?" )
				<< BUILD_LOG( kAck.m_iUnitUID )
				<< BUILD_LOG( iItemUID )
				<< BUILD_LOG( iSealCnt )
				<< END_LOG;
		}
	}

end_proc:

	SendToUser( LAST_SENDER_UID, DBE_GET_SHARE_BANK_ACK, kAck );
}

IMPL_ON_FUNC( DBE_UPDATE_SHARE_ITEM_REQ )
{
	KDBE_UPDATE_SHARE_ITEM_ACK kAck;

	if( Query_UpdateShareItem( kPacket_, kAck ) == false )
	{
		START_LOG( cerr, L"공유 은행 아이템 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}
	
	SendToUser( LAST_SENDER_UID, DBE_UPDATE_SHARE_ITEM_ACK, kAck );
}

_IMPL_ON_FUNC( DBE_UPDATE_SHARE_ITEM_FOR_MOVE_SLOT_REQ, KDBE_UPDATE_SHARE_ITEM_REQ )
{
	KDBE_UPDATE_SHARE_ITEM_ACK kAck;

	if( Query_UpdateShareItem( kPacket_, kAck ) == false )
	{
		START_LOG( cerr, L"공유 은행 아이템 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_UPDATE_SHARE_ITEM_FOR_MOVE_SLOT_ACK, kAck );
}

_IMPL_ON_FUNC( DBE_UPDATE_SEAL_DATA_NOT, KDBE_SEAL_ITEM_REQ )
{
	KDBE_SEAL_ITEM_ACK kPacket;
	kPacket.m_ucSealResult			  = kPacket_.m_ucSealResult;	
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSeal_MER", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_ucSealResult );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_item_seal", L"%d, %d", % kPacket_.m_iItemUID % kPacket_.m_ucSealResult );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"봉인 DB 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOGc( kPacket_.m_ucSealResult )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_SEAL_ITEM_16;
		goto end_proc;
	}

end_proc:
	return;
}

IMPL_ON_FUNC( DBE_CHANGE_INVENTORY_SLOT_ITEM_REQ )
{
	KDBE_CHANGE_INVENTORY_SLOT_ITEM_ACK kPacket;
	kPacket.m_cFromSlotType = kPacket_.m_cFromSlotType;
	kPacket.m_iFromSlotID = kPacket_.m_iFromSlotID;
	kPacket.m_cToSlotType = kPacket_.m_cToSlotType;
	kPacket.m_iToSlotID = kPacket_.m_iToSlotID;
	kPacket.m_kItemQuantityUpdate = kPacket_.m_kItemQuantityUpdate;
	kPacket.m_kItemEnduranceUpdate = kPacket_.m_kItemEnduranceUpdate;
	kPacket.m_kItemPositionUpdate = kPacket_.m_kItemPositionUpdate;

	//////////////////////////////////////////////////////////////////////////
	//START_LOG( cout, L"[테스트로그] 공유 은행 슬롯 동작하기 전에 미리 DB업데이트 정보를 처리하자!" )		
	//	<< BUILD_LOG( kPacket_.m_iUnitUID )
	//	<< BUILD_LOG( kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.size() )
	//	<< BUILD_LOG( kPacket_.m_kItemEnduranceUpdate.size() )
	//	<< BUILD_LOG( kPacket_.m_kItemPositionUpdate.size() )
	//	<< BUILD_LOG( kPacket_.m_kItemQuantityUpdate.m_vecDeleted.size() );
	//////////////////////////////////////////////////////////////////////////

	// 공유 은행 슬롯 이동을 하기 전에 미리 인벤토리의 모든 변경 정보를 DB와 맞추자!
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	LIF( Query_UpdateItemEndurance( kPacket_.m_iUnitUID, kPacket_.m_kItemEnduranceUpdate, kPacket.m_kItemEnduranceUpdate ) );
	Query_UpdateItemPosition( kPacket_.m_iUnitUID, kPacket_.m_kItemPositionUpdate, kPacket.m_kItemPositionUpdate );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	SendToUser( LAST_SENDER_UID, DBE_CHANGE_INVENTORY_SLOT_ITEM_ACK, kPacket );
}

#endif	SERV_SHARING_BANK_TEST

#ifdef GIANT_RESURRECTION_CASHSTONE
IMPL_ON_FUNC( DBE_REALTIME_RESURRECTION_CASHSTONE_NOT )
{

	int idumyData;
	Query_UpdateResurrection_stone(kPacket_.m_iUnitUID, kPacket_.m_iNumResurrection_CashStone, idumyData );


	//end_proc:
	return;
}
IMPL_ON_FUNC( DBE_RESURRECTION_CASHSTONE_NOT )
{


	int iRet = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.gup_update_resurrection_CashStone", L"%d, %d", 
											% kPacket_.m_iUnitUID 
											% kPacket_.m_iNumResurrection_CashStone );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iRet );

		m_kODBC.EndFetch();
	}
	else
	{
		goto end_proc;
	}


end_proc:
	LOG_SUCCESS( iRet == NetError::NET_OK )
		<< BUILD_LOG( iRet )
		<< BUILD_LOG( kPacket_.m_iUnitUID  )
		<< BUILD_LOG( kPacket_.m_iNumResurrection_CashStone  )
		<< END_LOG;

	return;

}
#endif //GIANT_RESURRECTION_CASHSTONE

#ifdef SERV_SHARING_BANK_EVENT
IMPL_ON_FUNC( DBE_SHARING_BANK_EVENT_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
	// sp를 호출해서 이벤트 보상을 주도록 합니다~
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEventBankShare_INS", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iItemID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBankShare_GET_Event", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iItemID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( clog2, L"계정은행 이벤트 SP 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< END_LOG;
	}
	else
	{
		START_LOG( clog2, L"지헌로그:계정은행 이벤트 SP 성공!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< END_LOG;

		SendToUser(LAST_SENDER_UID, DBE_SHARING_BANK_EVENT_ACK);
	}

end_proc:
	return;
}
#endif

//{{ 지헌 - 은행 개편 퀘스트, 캐쉬 작업 
#ifdef SERV_SHARING_BANK_QUEST_CASH
IMPL_ON_FUNC( DBE_SHARING_BACK_OPEN_REQ )
{
	KDBE_SHARING_BACK_OPEN_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	// 계정 은행 오픈 했다고 디비에 알리자
	// 해당 sp를 호출
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBankShare_INS", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iOpenType);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GBankShare_INS", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iOpenType);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"계정은행 뚫기 SP 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( static_cast<int>(kPacket_.m_iOpenType) )
			<< END_LOG;
	}

	kPacket.m_iOpenType = kPacket_.m_iOpenType;
	SendToUser( LAST_SENDER_UID, DBE_SHARING_BACK_OPEN_ACK, kPacket );
end_proc:
	return;
}
#endif SERV_SHARING_BANK_QUEST_CASH
//}}

#ifdef SERV_ADVERTISEMENT_EVENT
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
IMPL_ON_FUNC( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	BOOST_TEST_FOREACH( const int&, iEventUID, kPacket_.m_vecCompletedEvent )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GAdvertisementEvent_INS", L"%d, %d",
			% kPacket_.m_iUserUID
			% iEventUID
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GAdvertisementEvent_INT", L"%d, %d",
			% kPacket_.m_iUserUID
			% iEventUID
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"실시간 광고 이벤트 정보 기록 실패" )
				<< BUILD_LOG( iOK )
				<< END_LOG;
		}
	}
end_proc:
	return;
}
#endif //SERV_ADD_EVENT_DB
#endif SERV_ADVERTISEMENT_EVENT

//{{ 2011. 08. 03	최육사	대전 강제 종료에 대한 예외처리
#ifdef SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
IMPL_ON_FUNC( DBE_QUIT_USER_PVP_RESULT_UPDATE_NOT )
{
	LIF( Query_UpdatePvpInfo( kPacket_ ) );
}
#endif SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC( DBE_RETAINING_SELECT_REWARD_REQ )
{
	KDBE_RETAINING_SELECT_REWARD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;	
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;	

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	
	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cerr, L"이탈방지 보상 증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( Query_InsertItemList( SEnum::GIR_RETAINING_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
	{
		START_LOG( cerr, L"이탈방지 보상 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_ITEM_06;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	SendToUser( LAST_SENDER_UID, DBE_RETAINING_SELECT_REWARD_ACK, kPacket );
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
IMPL_ON_FUNC( DBE_USE_ITEM_IN_INVENTORY_REQ )
{
	KDBE_USE_ITEM_IN_INVENTORY_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUsedItemID = kPacket_.m_iUsedItemID;
#ifdef SERV_GOLD_TICKET
	kPacket.m_iItemUID = kPacket_.m_iItemUID;
#endif //SERV_GOLD_TICKET
	kPacket.m_iWarpPointMapID = kPacket_.m_iWarpPointMapID;
	//{{ 2012. 10. 31	박세훈	코보 익스프레스 티켓 추가
#ifdef SERV_ADD_COBO_EXPRESS_TICKET
	kPacket.m_iED	= kPacket_.m_iED;
#endif SERV_ADD_COBO_EXPRESS_TICKET
	//}}
	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
	kPacket.m_iTempCode	= kPacket_.m_iTempCode;
//#endif SERV_PET_AUTO_LOOTING
	//}}

	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	SendToUser( LAST_SENDER_UID, DBE_USE_ITEM_IN_INVENTORY_ACK, kPacket );
}
#endif SERV_USE_ITEM_DB_UPDATE_FIX
//}}

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
IMPL_ON_FUNC( DBE_BUY_UNIT_CLASS_CHANGE_REQ )
{
	kPacket_.m_iOK = NetError::ERR_ODBC_00;

	std::map< int, int >::const_iterator mitSkill = kPacket_.m_mapChangeSkill.begin();
#ifdef SERV_REFORM_SKILL_NOTE
#else // SERV_REFORM_SKILL_NOTE
	std::map< int, int >::const_iterator mitMemo = kPacket_.m_mapChangeMemo.begin();
#endif // SERV_REFORM_SKILL_NOTE

	std::map< UidType, int >::const_iterator mitItem = kPacket_.m_mapChangeItem.begin();
	std::map< int, int >::const_iterator mitCompleteQuest = kPacket_.m_mapChangeCompleteQuest.begin();
	std::map< int, int >::const_iterator mitInProgressQuest = kPacket_.m_mapChangeInProgressQuest.begin();

	// 클래스 변경
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_UnitClassChange", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iNewUnitClass ); // 단순 UnitClass 만 변경
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPT_UnitClassChange", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iNewUnitClass ); // 단순 UnitClass 만 변경
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		switch( kPacket_.m_iOK )
		{
		case -1:
			{
				START_LOG( cerr, L"전직 변경 아이템 실패! UnitClass 변경 실패 - 해당 유닛이 없는 경우" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iNewUnitClass )
					<< END_LOG;
			}break;
		case -2:
			{
				START_LOG( cerr, L"전직 변경 아이템 실패! UnitClass 변경 실패 - 업데이트 실패" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iNewUnitClass )
					<< END_LOG;
			}break;
		default:
			{
				START_LOG( cerr, L"전직 변경 아이템 실패! UnitClass 변경 실패" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iNewUnitClass )
					<< END_LOG;
			}break;
		}
	}

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	// 무제한 변경권을 사용시 로그 남긴다.
	if( kPacket_.m_iOK == NetError::NET_OK && kPacket_.m_bUnlimitedSecondJobItem == true )
	{
		KDBE_UNLIMITED_SECOND_CHANGE_JOB_NOT	kUSCjobNot;
		kUSCjobNot.m_iUnitUID			=	kPacket_.m_iUnitUID;
		kUSCjobNot.m_iOldUnitClass		=	kPacket_.m_iOldUnitClass;
		kUSCjobNot.m_iNewUnitClass		=	kPacket_.m_iNewUnitClass;

		SendToLogDB( DBE_UNLIMITED_SECOND_CHANGE_JOB_NOT, kUSCjobNot );
	}
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

	// 스킬 초기화
#ifdef SERV_SKILL_PAGE_SYSTEM

	for ( int iSkillPagesNumber = 1; 
		iSkillPagesNumber <= kPacket_.m_iTheNumberOfSkillPagesAvailable; iSkillPagesNumber++ )
	{
		DO_QUERY( L"exec dbo.P_GSkill_New_All_DEL_UnitClassChange_20131212", L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iSPoint
			% kPacket_.m_iCSPoint
			% kPacket_.m_iDefaultSkillID1
			% kPacket_.m_iDefaultSkillID2
			% kPacket_.m_iDefaultSkillID3
			% kPacket_.m_iDefaultSkillID4
			% kPacket_.m_iDefaultSkillID5
			% kPacket_.m_iDefaultSkillID6
			% iSkillPagesNumber
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"DB 스킬초기화 실패.! - 습득 스킬 지우기 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iSPoint )
						<< BUILD_LOG( kPacket_.m_iCSPoint )
						<< BUILD_LOG( kPacket_.m_iDefaultSkillID1 )
						<< BUILD_LOG( kPacket_.m_iDefaultSkillID2 )
						<< BUILD_LOG( kPacket_.m_iDefaultSkillID3 )
						<< BUILD_LOG( kPacket_.m_iDefaultSkillID4 )
						<< BUILD_LOG( kPacket_.m_iDefaultSkillID5 )
						<< BUILD_LOG( kPacket_.m_iDefaultSkillID6 )
						<< BUILD_LOG( iSkillPagesNumber )

						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"DB 스킬초기화 실패.!- 스킬 포인트 보상 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iSPoint )
						<< BUILD_LOG( kPacket_.m_iCSPoint )
						<< BUILD_LOG( iSkillPagesNumber )
						<< END_LOG;
				}break;
			case -3:
				{
					START_LOG( cerr, L"DB 스킬초기화 실패.!- 캐시 스킬 포인트 보상 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iSPoint )
						<< BUILD_LOG( kPacket_.m_iCSPoint )
						<< BUILD_LOG( iSkillPagesNumber )
						<< END_LOG;
				}break;
			case -4:
				{
					START_LOG( cerr, L"DB 스킬초기화 실패.!- 스킬 슬롯A 초기화 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iSPoint )
						<< BUILD_LOG( kPacket_.m_iCSPoint )
						<< BUILD_LOG( iSkillPagesNumber )
						<< END_LOG;
				}break;
			case -5:
				{
					START_LOG( cerr, L"DB 스킬초기화 실패.!- 스킬 슬롯B 초기화 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iSPoint )
						<< BUILD_LOG( kPacket_.m_iCSPoint )
						<< END_LOG;
				}break;
			case -6:
				{
					START_LOG( cerr, L"DB 스킬초기화 실패.! - 기본 스킬 레벨 1로 변경" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iSPoint )
						<< BUILD_LOG( kPacket_.m_iCSPoint )
						<< BUILD_LOG( iSkillPagesNumber )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"DB 스킬초기화 실패.!" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iSPoint )
						<< BUILD_LOG( kPacket_.m_iCSPoint )
						<< BUILD_LOG( iSkillPagesNumber )
						<< END_LOG;
				}break;
			}
		}
	}

#else // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_New_UPD_AllResetClassChange", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSPoint
		% kPacket_.m_iCSPoint
		% kPacket_.m_iDefaultSkillID1
		% kPacket_.m_iDefaultSkillID2
		% kPacket_.m_iDefaultSkillID3
		% kPacket_.m_iDefaultSkillID4
		% kPacket_.m_iDefaultSkillID5
		% kPacket_.m_iDefaultSkillID6
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_New_All_DEL_UnitClassChange", L"%d, %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSPoint
		% kPacket_.m_iCSPoint
		% kPacket_.m_iDefaultSkillID1
		% kPacket_.m_iDefaultSkillID2
		% kPacket_.m_iDefaultSkillID3
		% kPacket_.m_iDefaultSkillID4
		% kPacket_.m_iDefaultSkillID5
		% kPacket_.m_iDefaultSkillID6
		);
#endif //SERV_ALL_RENEWAL_SP
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	DO_QUERY( L"exec dbo.P_GSkill_New_DEL_UnitClassChange", L"%d, %d, %d, %d, %d",
	% kPacket_.m_iUnitUID
	% kPacket_.m_iSPoint
	% kPacket_.m_iCSPoint
	% kPacket_.m_iDefaultSkillID
	% kPacket_.m_iDefaultSkillID2
	);
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		switch( kPacket_.m_iOK )
		{
		case -1:
			{
				START_LOG( cerr, L"DB 스킬초기화 실패.! - 습득 스킬 지우기 실패" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSPoint )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID1 )
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID2 )
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID3 )
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID4 )
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID5 )
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID6 )
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID )
					<< BUILD_LOG( kPacket_.m_iDefaultSkillID2 )
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
					<< END_LOG;
			}break;
		case -2:
			{
				START_LOG( cerr, L"DB 스킬초기화 실패.!- 스킬 포인트 보상 실패" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSPoint )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
					<< END_LOG;
			}break;
		case -3:
			{
				START_LOG( cerr, L"DB 스킬초기화 실패.!- 캐시 스킬 포인트 보상 실패" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSPoint )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
					<< END_LOG;
			}break;
		case -4:
			{
				START_LOG( cerr, L"DB 스킬초기화 실패.!- 스킬 슬롯A 초기화 실패" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSPoint )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
					<< END_LOG;
			}break;
		case -5:
			{
				START_LOG( cerr, L"DB 스킬초기화 실패.!- 스킬 슬롯B 초기화 실패" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSPoint )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
					<< END_LOG;
			}break;
		case -6:
			{
				START_LOG( cerr, L"DB 스킬초기화 실패.! - 기본 스킬 레벨 1로 변경" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSPoint )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
					<< END_LOG;
			}break;
		default:
			{
				START_LOG( cerr, L"DB 스킬초기화 실패.!" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iSPoint )
					<< BUILD_LOG( kPacket_.m_iCSPoint )
					<< END_LOG;
			}break;
		}
	}

#endif // SERV_SKILL_PAGE_SYSTEM



	// 스킬 변경 적용
	for( ; mitSkill != kPacket_.m_mapChangeSkill.end() ; ++mitSkill )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_Unsealed_UPD_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitSkill->first % mitSkill->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_Unsealed_UPT_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitSkill->first % mitSkill->second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Skill 변경 실패 - 해당 유닛이 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitSkill->first )
						<< BUILD_LOG( mitSkill->second )
						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Skill 변경 실패 - 해당 캐시 스킬이 봉인해제되지 않은 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitSkill->first )
						<< BUILD_LOG( mitSkill->second )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Skill 변경 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitSkill->first )
						<< BUILD_LOG( mitSkill->second )
						<< END_LOG;
				}break;
			}
		}
	}
	
#ifdef SERV_REFORM_SKILL_NOTE
#else // SERV_REFORM_SKILL_NOTE
	// 장착한 메모 변경 적용
	for( ; mitMemo != kPacket_.m_mapChangeMemo.end() ; ++mitMemo )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GNote_UPD_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitMemo->first % mitMemo->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GNote_UPT_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitMemo->first % mitMemo->second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Memo 변경 실패 - 해당 유닛이 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitMemo->first )
						<< BUILD_LOG( mitMemo->second )
						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Memo 변경 실패 - 해당 메모가 기술의 노트에 기록되지 않은 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitMemo->first )
						<< BUILD_LOG( mitMemo->second )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Memo 변경 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitMemo->first )
						<< BUILD_LOG( mitMemo->second )
						<< END_LOG;
				}break;
			}
		}
	}
#endif // SERV_REFORM_SKILL_NOTE


	// 인벤토리 캐쉬 & 퀘스트 아이템 변경 적용
	for( ; mitItem != kPacket_.m_mapChangeItem.end() ; ++mitItem )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitItem->first % mitItem->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPT_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitItem->first % mitItem->second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Item 변경 실패 - 해당 유닛이 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitItem->first )
						<< BUILD_LOG( mitItem->second )
						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Item 변경 실패 - 해당 아이템이 인벤토리에 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitItem->first )
						<< BUILD_LOG( mitItem->second )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! Item 변경 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitItem->first )
						<< BUILD_LOG( mitItem->second )
						<< END_LOG;
				}break;
			}
		}
	}

	// 퀘스트 삭제
	for( u_short unSize = 0 ; unSize < kPacket_.m_vecDeleteCompleteQuest.size() ; ++unSize )
	{
		DO_QUERY( L"exec dbo.P_GQuests_Complete_DEL_UnitClassChange", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_vecDeleteCompleteQuest[unSize]  );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"전직 변경 Quest 삭제 실패 - 해당 유닛이 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_vecDeleteCompleteQuest[unSize] )
						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"전직 변경 Quest 삭제 실패 - 퀘스트가 완료되지 않은 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_vecDeleteCompleteQuest[unSize] )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"전직 변경 Quest 삭제 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_vecDeleteCompleteQuest[unSize] )
						<< END_LOG;
				}break;
			}
		}
	}

	// 퀘스트 삭제
	for( u_short unSize = 0 ; unSize < kPacket_.m_vecDeleteInProgressQuest.size() ; ++unSize )
	{
		DO_QUERY( L"exec dbo.P_GQuests_DEL_UnitClassChange", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_vecDeleteInProgressQuest[unSize]  );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"전직 변경 진행 중 Quest 삭제 실패 - 해당 유닛이 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_vecDeleteInProgressQuest[unSize] )
						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"전직 변경 진행 중 Quest 삭제 실패 - 퀘스트가 완료되지 않은 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_vecDeleteInProgressQuest[unSize] )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"전직 변경 진행 중 Quest 삭제 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_vecDeleteInProgressQuest[unSize] )
						<< END_LOG;
				}break;
			}
		}
	}

	// 퀘스트 변경 적용
	for( ; mitCompleteQuest != kPacket_.m_mapChangeCompleteQuest.end() ; ++mitCompleteQuest )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQuests_Complete_UPD_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitCompleteQuest->first % mitCompleteQuest->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQuests_Complete_UPT_UnitClassChange", L"%d, %d, %d", % kPacket_.m_iUnitUID % mitCompleteQuest->first % mitCompleteQuest->second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! 완료 Quest 변경 실패 - 해당 유닛이 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitCompleteQuest->first )
						<< BUILD_LOG( mitCompleteQuest->second )
						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! 완료 Quest 변경 실패 - 해당 퀘스트가 완료되지 않은 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitCompleteQuest->first )
						<< BUILD_LOG( mitCompleteQuest->second )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! 완료 Quest 변경 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitCompleteQuest->first )
						<< BUILD_LOG( mitCompleteQuest->second )
						<< END_LOG;
				}break;
			}
		}
	}

	for( ; mitInProgressQuest != kPacket_.m_mapChangeInProgressQuest.end() ; ++mitInProgressQuest )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQuests_UPD_UnitClassChange", L"%d, %d, %d", 
			% kPacket_.m_iUnitUID 
			% mitInProgressQuest->first 
			% mitInProgressQuest->second
			); //완료로 변경 및 새로 수락
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GQuests_UPT_UnitClassChange", L"%d, %d, %d", 
			% kPacket_.m_iUnitUID 
			% mitInProgressQuest->first 
			% mitInProgressQuest->second
			); //완료로 변경 및 새로 수락
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			switch( kPacket_.m_iOK )
			{
			case -1:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! 진행 Quest 변경 실패 - 해당 유닛이 없는 경우" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitInProgressQuest->first )
						<< BUILD_LOG( mitInProgressQuest->second )
						<< END_LOG;
				}break;
			case -2:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! 진행 Quest 변경 실패 - 해당 퀘스트가 진행중이지 않은 경우 " )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitInProgressQuest->first )
						<< BUILD_LOG( mitInProgressQuest->second )
						<< END_LOG;
				}break;
			default:
				{
					START_LOG( cerr, L"전직 변경 아이템 실패! 진행 Quest 변경 실패" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( mitInProgressQuest->first )
						<< BUILD_LOG( mitInProgressQuest->second )
						<< END_LOG;
				}break;
			}
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_BUY_UNIT_CLASS_CHANGE_ACK, kPacket_ );
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}


//{{ 2012. 04. 30	박세훈	현자의 주문서 접속 이벤트 ( 우편함 중복 체크 )
#ifdef SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
_IMPL_ON_FUNC( DBE_UPDATE_EVENT_TIME_NOT, KDBE_UPDATE_EVENT_TIME_REQ )
{
	int iOK = NetError::ERR_ODBC_01;

	{
		//////////////////////////////////////////////////////////////////////////
		// 접속 시간 이벤트

		std::vector< KConnectTimeEventInfo >::const_iterator vit;
		for( vit = kPacket_.m_vecConnectTimeEvent.begin(); vit != kPacket_.m_vecConnectTimeEvent.end(); ++vit )
		{
			//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
			if( vit->m_bAccountEvent )
			{
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GEvent_Account_Nor_MER", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vit->m_iEventUID % vit->m_wstrEventTime );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_event_account_nor", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vit->m_iEventUID % vit->m_wstrEventTime );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vit->m_iEventUID )
						<< BUILD_LOG( vit->m_wstrEventTime )
						<< END_LOG;
				}
			}
			else
#endif SERV_ACC_TIME_EVENT
				//}}
			{
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GIs30Min_MER", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vit->m_iEventUID % vit->m_wstrEventTime );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_30min", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vit->m_iEventUID % vit->m_wstrEventTime );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vit->m_iEventUID )
						<< BUILD_LOG( vit->m_wstrEventTime )
						<< END_LOG;
				}
			}			
		}

		//////////////////////////////////////////////////////////////////////////		
	}


	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT

	{
		//////////////////////////////////////////////////////////////////////////
		// 누적 시간 이벤트

		CTime tUpdateTime = CTime::GetCurrentTime();
		tUpdateTime += CTimeSpan( 18250, 0, 0, 0 ); // 한번만 줘야 하므로 50년을 더한다.
		std::wstring wstrUpdateDate = ( CStringW )( tUpdateTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) ) );

		std::vector< KCumulativeTimeEventInfo >::const_iterator vitCT;
		for( vitCT = kPacket_.m_vecCumulativeTimeEvent.begin(); vitCT != kPacket_.m_vecCumulativeTimeEvent.end(); ++vitCT )
		{
			//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
			if( vitCT->m_bAccountEvent )
			{
				// 완료 정보를 업데이트 하자!
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GEvent_Account_Nor_MER", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vitCT->m_iEventUID % wstrUpdateDate );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_event_account_nor", L"%d, %d, N\'%s\'", % LAST_SENDER_UID % vitCT->m_iEventUID % wstrUpdateDate );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vitCT->m_iEventUID )
						<< BUILD_LOG( wstrUpdateDate )
						<< END_LOG;
				}
			}
			else
#endif SERV_ACC_TIME_EVENT
				//}}
			{
				// 완료 정보를 업데이트 하자!
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GIs30Min_MER", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vitCT->m_iEventUID % wstrUpdateDate );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_update_30min", L"%d, %d, N\'%s\'", % kPacket_.m_iUnitUID % vitCT->m_iEventUID % wstrUpdateDate );
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"이벤트 타임 DB업데이트 실패?" )
						<< BUILD_LOG( iOK )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( vitCT->m_iEventUID )
						<< BUILD_LOG( wstrUpdateDate )
						<< END_LOG;
				}
			}
		}
	}

#endif CUMULATIVE_TIME_EVENT
	//}}

end_proc:
	return;
}
#endif SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
//}}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
IMPL_ON_FUNC( DBE_TRADE_COMPLETE_REQ )
{
	KDBE_TRADE_COMPLETE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iED = kPacket_.m_iED;
	kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;

	bool bUpdateFailed = false;

	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	SendToUser( LAST_SENDER_UID, DBE_TRADE_COMPLETE_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
IMPL_ON_FUNC( DBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ )
{
	KDBE_ADMIN_CHEAT_GET_ALL_SKILL_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// 봉인된 스킬 해제
	if( kPacket_.m_vecUnSealedSkill.empty() == false )
	{
		BOOST_TEST_FOREACH( short, iSkillID, kPacket_.m_vecUnSealedSkill )
		{
			int iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_Unsealed_INS", L"%d, %d", % kPacket_.m_iUnitUID % iSkillID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_insert_unsealed_skill", L"%d, %d", % kPacket_.m_iUnitUID % iSkillID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			// 해제에 실패하면 해당 스킬은 찍지 않는다.
			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"봉인스킬 봉인해제 실패!" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( iSkillID )
					<< END_LOG;
				
				kPacket_.m_mapSkillInfo.erase( iSkillID );
				kPacket.m_iOK = NetError::ERR_ODBC_01;
				continue;
			}
			kPacket.m_vecUnSealedSkill.push_back( iSkillID );
		}
	}

#ifdef SERV_SKILL_PAGE_SYSTEM
	// 남은 스킬 전부 다 찍기
	for( std::map<int, KAdminCheatSkill>::iterator it=kPacket_.m_mapSkillInfo.begin(); it != kPacket_.m_mapSkillInfo.end(); ++it )
	{
		int iOK = NetError::NET_OK;
		const int iSkillID		= it->first;
		const int iSkillLevel	= it->second.m_iSkillLevel;
		const int iSkillCSPoint	= it->second.m_iSkillCSPoint;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% iSkillID
			% iSkillLevel
			% iSkillCSPoint
			% kPacket_.m_iActiveSkillPageNumber );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_UPD", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% iSkillID
			% iSkillLevel
			% iSkillCSPoint
			% kPacket_.m_iActiveSkillPageNumber );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to insert skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( iSkillID )
				<< BUILD_LOG( iSkillLevel )
				<< BUILD_LOG( iSkillCSPoint )
				<< BUILD_LOG( iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ODBC_01;
			continue;
		}

		kPacket.m_mapSkillInfo.insert( std::map<int, KAdminCheatSkill>::value_type( it->first, it->second ) );
	}
#else // SERV_SKILL_PAGE_SYSTEM
	// 남은 스킬 전부 다 찍기
	for( std::map<int, KAdminCheatSkill>::iterator it=kPacket_.m_mapSkillInfo.begin(); it != kPacket_.m_mapSkillInfo.end(); ++it )
	{
		int iOK = NetError::NET_OK;
		const int iSkillID		= it->first;
		const int iSkillLevel	= it->second.m_iSkillLevel;
		const int iSkillCSPoint	= it->second.m_iSkillCSPoint;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% iSkillID
			% iSkillLevel
			% iSkillCSPoint
			% 0 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
			% kPacket_.m_iUnitUID 
			% iSkillID
			% iSkillLevel
			% iSkillCSPoint );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to insert skill!" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( iSkillID )
				<< BUILD_LOG( iSkillLevel )
				<< BUILD_LOG( iSkillCSPoint )
				<< BUILD_LOG( iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_ODBC_01;
			continue;
		}

		kPacket.m_mapSkillInfo.insert( std::map<int, KAdminCheatSkill>::value_type( it->first, it->second ) );
	}
#endif // SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_SKILL_PAGE_SYSTEM
	kPacket.m_iActiveSkillPageNumber = kPacket_.m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_CHEAT_GET_ALL_SKILL_ACK, kPacket );
}
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}

//{{ 2012. 05. 30	최육사	그노시스 기간 치트
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
IMPL_ON_FUNC( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ )
{
	KEGS_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_00;

	// 기간 변경 치트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Cash_UPD_EndDate", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrModifyDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Cash_UPD_C", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrModifyDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
	
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"치트 쿼리 호출 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrModifyDate )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
	}
	else
	{
		kPacket.m_wstrCspEndDate = kPacket_.m_wstrModifyDate;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_ACK, kPacket );
}
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
//}}

//{{ 우편 ED 거래시 ED 동기화 - 김민성
#ifdef SERV_SEND_LETTER_BEFOR_ED_SYNC
IMPL_ON_FUNC( DBE_SYNC_ED_REQ )
{
	KDBE_SYNC_ED_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_00;
	kPacket.m_kInfo = kPacket_;

	int iTotalChangeED = kPacket.m_kInfo.m_iChangeED - kPacket.m_kInfo.m_iED - kPacket.m_kInfo.m_iSendLetterCost;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPD_GamePointPost", L"%d, %d", % kPacket.m_kInfo.m_iFromUnitUID % iTotalChangeED );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnit_UPT_POST_GP", L"%d, %d", % kPacket.m_kInfo.m_iFromUnitUID % iTotalChangeED );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	// 해제에 실패하면 해당 스킬은 찍지 않는다.
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"ED 동기화 실패!!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket.m_kInfo.m_iFromUnitUID )
			<< BUILD_LOG( iTotalChangeED )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_SYNC_ED_ACK, kPacket );
}
#endif SERV_SEND_LETTER_BEFOR_ED_SYNC
//}}

//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
IMPL_ON_FUNC_NOPARAM( DBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ )
{
	KDBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK kPacket;

	CTime tCurrentTime = CTime::GetCurrentTime();
	CTime tMore( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0 );
	CTime tUnder( tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay() + 1, 0, 0, 0 );

	std::wstring wstrMore = tMore.Format( _T("%Y-%m-%d %H:%M:%S") );
	std::wstring wstrUnder = tUnder.Format( _T("%Y-%m-%d %H:%M:%S") );
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_AngelCoin_SEL", L"%d, N\'%s\', N\'%s\'", % FIRST_SENDER_UID % wstrMore % wstrUnder );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_AngelCoin_GET", L"%d, N\'%s\', N\'%s\'", % FIRST_SENDER_UID % wstrMore % wstrUnder );
#endif //SERV_ALL_RENEWAL_SP
	// 로그 남기기
	int iResult = -1;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iResult );
		m_kODBC.EndFetch();
	}

	if( iResult == 0 )
	{
		kPacket.m_bReceiveTheLetter = true;
	}
	else
	{
		kPacket.m_bReceiveTheLetter = false;
		if( iResult == -1 )	
		{
			START_LOG( cerr, L"대천사의 주화 이벤트 BeginFetch() 실패")
				<< END_LOG;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK, kPacket );
}
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
//}}

//{{ 2012. 08. 21	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
IMPL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_ARRANGE_REQ )
{
	bool bUpdateFailed = false;
	KItemQuantityUpdate kFailed;

	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kFailed.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kFailed.m_vecDeleted );

	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}

	kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange	= kFailed.m_mapQuantityChange;
	kPacket_.m_kItemQuantityUpdate.m_vecDeleted			= kFailed.m_vecDeleted;

	BOOST_TEST_FOREACH( KInventoryItemInfo, kItem, kPacket_.m_vecMoveItemSlotInfo )
	{
		int iOK;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_UPD_Postion", L"%d, %d, %d", % kItem.m_iItemUID % static_cast<byte>( kItem.m_cSlotCategory ) % kItem.m_sSlotID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_item_position", L"%d, %d, %d", % kItem.m_iItemUID % static_cast<byte>( kItem.m_cSlotCategory ) % kItem.m_sSlotID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 위치 업데이트 실패." )
					<< BUILD_LOG( iOK )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"아이템 위치 업데이트중 BeginFetch 실패." );
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_ITEM_FROM_LETTER_ARRANGE_ACK, kPacket_ );
}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
IMPL_ON_FUNC( DBE_PVP_WIN_EVENT_CHECK_REQ )
{
	KDBE_PVP_WIN_EVENT_CHECK_ACK kPacket;
	kPacket.m_iOK					  	 = NetError::ERR_ODBC_01;
	kPacket.m_kRewardAck.m_iOK			 = NetError::ERR_ODBC_01;
	kPacket.m_kRewardAck.m_iRewardLetter.m_iFromUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_kRewardAck.m_iRewardLetter.m_iToUnitUID   = kPacket_.m_iUnitUID;
	kPacket.m_kRewardAck.m_iRewardLetter.m_cScriptType  = KPostItemInfo::LT_EVENT;
	kPacket.m_kRewardAck.m_iRewardLetter.m_iScriptIndex = 10407;
	kPacket.m_iIndex = kPacket_.m_iIndex;
	kPacket.m_wstrRegDate = kPacket_.m_wstrRegDate;

	int iPvpEventIndex = 0;
	std::wstring wstrLastPvpEventDate;

	// 받을 수 있는 유저인지 확인한다.
	DO_QUERY( L"exec dbo.P_GEvent_Unit_GET ", L"%d", % kPacket_.m_iUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iPvpEventIndex
			>> wstrLastPvpEventDate );

		m_kODBC.EndFetch();
	}

	// 받은 적이 있다.
	if( iPvpEventIndex != 0 )
	{
		// 같은 날짜인데 같은 인덱스라면 이번 이벤트때 벌서 받은 사람이다.
		CTime tCurr;
		CTime tDBRegDate;
		KncUtil::ConvertStringToCTime( kPacket_.m_wstrRegDate, tCurr );
		KncUtil::ConvertStringToCTime( wstrLastPvpEventDate, tDBRegDate );

		if( tCurr.GetDay() == tDBRegDate.GetDay() && kPacket_.m_iIndex == iPvpEventIndex )
		{
			START_LOG( cerr, L"대전 이벤트 아이템 지급 대상자가 아닙니다." )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_wstrRegDate )
				<< BUILD_LOG( iPvpEventIndex )
				<< BUILD_LOG( wstrLastPvpEventDate )
				<< END_LOG;

			goto end_proc;
		}
	}
	
	// 보상
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUnitUID
		% 0
		% KPostItemInfo::LT_EVENT
		% 10407
		% kPacket.m_kRewardAck.m_iRewardLetter.m_wstrMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iUnitUID
		% 0
		% KPostItemInfo::LT_EVENT
		% 10407
		% kPacket.m_kRewardAck.m_iRewardLetter.m_wstrMessage
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_kRewardAck.m_iOK
				 >> kPacket.m_kRewardAck.m_iRewardLetter.m_iPostNo
				 >> kPacket.m_kRewardAck.m_iRewardLetter.m_wstrRegDate );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_kRewardAck.m_iOK == NetError::NET_OK )
	{
		DO_QUERY( L"exec dbo.P_GEvent_Unit_INT", L"%d, %d, N\'%s\'",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iIndex
			% kPacket_.m_wstrRegDate
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"대전 이벤트 아이템 지급 이후 로그 기록 실패" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;
		}
	}	

end_proc:

	// 로그 기록 성공
	SendToUser( LAST_SENDER_UID, DBE_PVP_WIN_EVENT_CHECK_ACK, kPacket );

	// 우편 발송 성공
	if( kPacket.m_kRewardAck.m_iOK == NetError::NET_OK )
	{
		SendToUser( LAST_SENDER_UID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket.m_kRewardAck );
	}
}
#endif SERV_2012_PVP_SEASON2_EVENT
//}}

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
_IMPL_ON_FUNC( DBE_NEW_ACCOUNT_QUEST_GAME_DB_REQ, KDBE_NEW_QUEST_REQ )
{
	KDBE_NEW_QUEST_ACK kPacket;
	kPacket.m_iOK		= NetError::ERR_ODBC_01;
	kPacket.m_iQuestID	= kPacket_.m_iQuestID;
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	//////////////////////////////////////////////////////////////////////////
	// 아이템 보상
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_UnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_UnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
	if( Query_InsertItemList( SEnum::GIR_QUEST_REWARD, kPacket_.m_UnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
	{
		START_LOG( cerr, L"퀘스트 수락 보상 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		goto end_proc;
	}
	//////////////////////////////////////////////////////////////////////////

	kPacket.m_iOK		= NetError::NET_OK;
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_NEW_ACCOUNT_QUEST_GAME_DB_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ACCOUNT_QUEST_COMPLETE_GAME_DB_REQ, KDBE_ACCOUNT_QUEST_COMPLETE_ACK )
{
	KDBE_QUEST_COMPLETE_ACK kPacket;

	kPacket.m_bAutoComplete				= kPacket_.m_kQuestReq.m_bAutoComplete;
	kPacket.m_bIsNew					= kPacket_.m_kQuestReq.m_bIsNew;
	kPacket.m_iOK						= NetError::ERR_ODBC_01;
	kPacket.m_mapInsertedItem			= kPacket_.m_kQuestReq.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_kQuestReq.m_vecUpdatedInventorySlot;
	kPacket.m_bIsChangeJob				= kPacket_.m_kQuestReq.m_bIsChangeJob;
	kPacket.m_cChangeUnitClass			= kPacket_.m_kQuestReq.m_cChangeUnitClass;	
	kPacket.m_kCompleteQuestInfo.m_iQuestID = kPacket_.m_kQuestReq.m_iQuestID;
	kPacket.m_kCompleteQuestInfo.m_iCompleteCount = 1;
	kPacket.m_kCompleteQuestInfo.m_tCompleteDate = kPacket_.m_tCompleteTime;

	//item uid 받아오기
	if( kPacket_.m_kQuestReq.m_bIsNew == true )
	{
		bool bUpdateFailed = false;
		LIF( Query_UpdateItemQuantity( kPacket_.m_kQuestReq.m_UnitUID, kPacket_.m_kQuestReq.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
		Query_DeleteItem( kPacket_.m_kQuestReq.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

		//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
		if( bUpdateFailed )
		{
			START_LOG( cout, L"증분값 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_kQuestReq.m_UnitUID );

			std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kQuestReq.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
			for( ; mitQC != kPacket_.m_kQuestReq.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
			{
				START_LOG( cout, L"아이템 정보" )
					<< BUILD_LOG( mitQC->first )
					<< BUILD_LOG( mitQC->second );
			}
		}
		//}}

		if( Query_InsertItemList( SEnum::GIR_QUEST_REWARD, kPacket_.m_kQuestReq.m_UnitUID, kPacket_.m_kQuestReq.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
		{
			START_LOG( cerr, L"퀘스트 보상 아이템 업데이트 실패!" )
				<< BUILD_LOG( kPacket_.m_kQuestReq.m_UnitUID )
				<< END_LOG;
			goto end_proc;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_QUEST_COMPLETE_GAME_DB_ACK, kPacket );
};
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}

//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
IMPL_ON_FUNC( DBE_PET_AUTO_LOOTING_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_UPD_PickUp", L"%d", % kPacket_.m_iPetUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_PickUP_UPT", L"%d", % kPacket_.m_iPetUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"펫의 오토 루팅 기능 DB 업데이트에 실패 하였습니다." )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< END_LOG;
	}
end_proc:
	return;
}
#endif SERV_PET_AUTO_LOOTING
//}}

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
IMPL_ON_FUNC( DBE_OPEN_SYNTHESIS_ITEM_REQ )
{
	KDBE_OPEN_SYNTHESIS_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_mapSynthesisData			= kPacket_.m_mapSynthesisData;
	kPacket.m_iKeyData					= kPacket_.m_iKeyData;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( Query_InsertItemList( SEnum::GIR_SYNTHESIS_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
	{
		START_LOG( cerr, L"아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RANDOM_ITEM_06;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	//{{ 2009. 9. 2  최육사		밀봉
	Query_UpdateSealItem( kPacket_.m_setSealCashItem, kPacket.m_mapItemInfo );
	//}}

	SendToUser( LAST_SENDER_UID, DBE_OPEN_SYNTHESIS_ITEM_ACK, kPacket );
}
#endif SERV_SYNTHESIS_AVATAR
//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
IMPL_ON_FUNC( DBE_CHANGE_PET_ID_REQ )
{
	KDBE_CHANGE_PET_ID_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	// 펫 ID 변경
	DO_QUERY( L"exec dbo.P_GPet_Info_UPD_ChangePet", L"%d, %d", % kPacket_.m_iPetUID % kPacket_.m_iAfterPetID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"변신 물약 허용 가능한 펫의 DB 업데이트에 실패 하였습니다." )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< END_LOG;
	}

	kPacket.m_iPetUID = kPacket_.m_iPetUID;
	kPacket.m_iBeforePetID = kPacket_.m_iBeforePetID;
	kPacket.m_iAfterPetID = kPacket_.m_iAfterPetID;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_PET_ID_ACK, kPacket );
	return;
}
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_ADD_WARP_BUTTON
IMPL_ON_FUNC( DBE_INSERT_WARP_VIP_REQ )
{
	KDBE_INSERT_WARP_VIP_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_wstrEndDate = L"2000-01-01 00:00:00";
	
	// VIP 기간 갱신시켜 줍니다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GVIPTicket_MER", L" %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPeriod );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GVIPTicket_SET", L" %d, %d", % kPacket_.m_iUnitUID % kPacket_.m_iPeriod );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iOK
			>> kAck.m_wstrEndDate );

		m_kODBC.EndFetch();
	}
	
	START_LOG( clog, L"[TEST] VIP 기간 DB에 업데이트 했습니다." )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iPeriod )
		<< BUILD_LOG( kAck.m_wstrEndDate.c_str() )
		<< END_LOG;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_WARP_VIP_ACK, kAck );
}
#endif // SERV_ADD_WARP_BUTTON

//{{ 2013. 3. 11	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
IMPL_ON_FUNC( DBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ )
{
	KDBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK kPacket;

	// 유저 정보
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_SEL", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_GET", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iUID
			>> kPacket.m_kUnitInfo.m_wstrNickName
			>> kPacket.m_kUnitInfo.m_ucLevel
			>> kPacket.m_kUnitInfo.m_cUnitClass
			>> kPacket.m_kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT]
			>> kPacket.m_kUnitInfo.m_iPoint[SEnum::LRMTC_AP]
			);
		m_kODBC.EndFetch();

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket );
			return;
		}

		kPacket.m_kUserInfo.m_iUserUID	= kPacket_.m_iUserUID;
		kPacket.m_kUnitInfo.m_iUnitUID	= kPacket_.m_iUnitUID;
		kPacket.m_bFiltered				= kPacket_.m_bFiltered;
	}

end_proc:
	KncSend( PI_GS_GAME_DB, 0, PI_ACCOUNT_DB, 0, anTrace_, DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_REQ, kPacket );
}

IMPL_ON_FUNC( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ )
{
	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ kPacket;

	// 유저 정보
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_SEL", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_GET", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iUID
			>> kPacket.m_kUnitInfo.m_wstrNickName
			>> kPacket.m_kUnitInfo.m_ucLevel
			>> kPacket.m_kUnitInfo.m_cUnitClass
			>> kPacket.m_kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT]
			>> kPacket.m_kUnitInfo.m_iPoint[SEnum::LRMTC_AP]
			);
		m_kODBC.EndFetch();

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK, kPacket );
			return;
		}

		kPacket.m_kUserInfo.m_iUserUID	= kPacket_.m_iUserUID;
		kPacket.m_kUnitInfo.m_iUnitUID	= kPacket_.m_iUnitUID;

		kPacket.m_nViewPage				= kPacket_.m_nViewPage;
		kPacket.m_cMainTabIndex			= kPacket_.m_cMainTabIndex;
		kPacket.m_cSubTabIndex			= kPacket_.m_cSubTabIndex;
		kPacket.m_byteFilter			= kPacket_.m_byteFilter;
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		kPacket.m_bRankingButtonClick	= kPacket_.m_bRankingButtonClick;
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
	}

end_proc:
	KncSend( PI_GS_GAME_DB, 0, PI_ACCOUNT_DB, 0, anTrace_, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ, kPacket );
}

IMPL_ON_FUNC( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ )
{
	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK kPacket;

	// 유저 정보
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_SEL", L"%d", % kPacket_.m_iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_GET", L"%d", % kPacket_.m_iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iUID
			>> kPacket.m_kUnitInfo.m_wstrNickName
			>> kPacket.m_kUnitInfo.m_ucLevel
			>> kPacket.m_kUnitInfo.m_cUnitClass
			>> kPacket.m_kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT]
			>> kPacket.m_kUnitInfo.m_iPoint[SEnum::LRMTC_AP]
			);
		m_kODBC.EndFetch();

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK, kPacket );
			return;
		}

		kPacket.m_kUserInfo.m_iUserUID	= kPacket_.m_iUserUID;
		kPacket.m_kUnitInfo.m_iUnitUID	= kPacket_.m_iUnitUID;

		kPacket.m_iIncreasedSpirit	= kPacket_.m_iIncreasedSpirit;
		kPacket.m_iIncreasedAP		= kPacket_.m_iIncreasedAP;
		kPacket.m_bForce			= kPacket_.m_bForce;
	}

end_proc:
	KncSend( PI_GS_GAME_DB, 0, PI_ACCOUNT_DB, 0, anTrace_, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ, kPacket );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
IMPL_ON_FUNC( DBE_RECOMMEND_USER_GET_NEXON_SN_REQ )
{
	KDBE_RECOMMEND_USER_GET_NEXON_SN_ACK kAck;
	kAck.m_iRecommendUserNexonSN = kPacket_.m_iRecommendUserNexonSN;
	kAck.m_iRecommendUnitUID = kPacket_.m_iRecommendUnitUID;	
	kAck.m_wstrRecommendedUserNickName = kPacket_.m_wstrRecommendedUserNickName;

	DO_QUERY( L"exec dbo.P_GUnitNickName_MUser_OID_GET", L" N\'%s\'", % kPacket_.m_wstrRecommendedUserNickName );

	if( m_kODBC.BeginFetch() )
	{
#ifdef SERV_RECOMMEND_LIST_EVENT_2013_07	// 적용날짜: 2013-07-04
		FETCH_DATA( kAck.m_iRecommendedUserNexonSN 
			>> kAck.m_iRecommendedUserChannelCode  );
#else	// SERV_RECOMMEND_LIST_EVENT_2013_07
		FETCH_DATA( kAck.m_iRecommendedUserNexonSN );
#endif SERV_RECOMMEND_LIST_EVENT_2013_07

		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RECOMMEND_USER_GET_NEXON_SN_ACK, kAck );
}
#endif SERV_RECOMMEND_LIST_EVENT
//}
#ifdef SERV_MOMOTI_EVENT
IMPL_ON_FUNC( DBE_MOMOTI_QUIZ_EVENT_REQ )
{
	KDBE_MOMOTI_QUIZ_EVENT_ACK kPacketDBEMomotiQuizEventAck;
	kPacketDBEMomotiQuizEventAck.m_iOK = NetError::NET_OK;
	
	
	/// 보상여부 체크 하자
	int iCheckReward = 0; // 기본 세팅은 정답 처리
	int iStrReply = 0;
	iStrReply = kPacket_.m_istrReply;

	int iCheckRewardID = 0;
	switch(iStrReply)
	{
	case 1:
		iCheckRewardID = _CONST_MOMOTI_EVENT_::iMomotiReward1;
		break;
	case 2:
		iCheckRewardID = _CONST_MOMOTI_EVENT_::iMomotiReward2;
		break;
	case 3:
		iCheckRewardID = _CONST_MOMOTI_EVENT_::iMomotiReward3;
		break;
	}
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.PX_EventUser_SEL_Check", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % iCheckRewardID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EventUser_CHK", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % iCheckRewardID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iCheckReward );
		// 보상 대상자이면 0, 이미 보상 받은 사람이면 1
		m_kODBC.EndFetch();

	}
	START_LOG( cwarn, L"KimSukGeun_퀴즈이벤트 보상여부 체크" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( iCheckReward )
		<< BUILD_LOG( iStrReply )
		<< END_LOG;


	/// 오답여부 체크하자( iStrReply == 0 이면, 오답)
	if(iCheckReward == 0 && iStrReply == 0) // 미보상상태에서 오답이라면,
	{
		iCheckReward = 2; // 오답이면 2 처리
	}

#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
	// 대상 캐릭터가 아니면 리턴값이 100로 리턴됨.
	if( iStrReply == 100 )
	{
		iCheckReward = 3;
	}
#endif //SERV_MOMOTI_EVENT_ADDQUIZ
	/// 보상 대상자 구분처리
	if( iCheckReward == 0) // 미보상 상태
	{
		// 보상 안받았으면 보상 주자
		// 이벤트 보상을 주자!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDBPVPEVENT;
		kPacketToDBPVPEVENT.m_iFromUnitUID = kPacket_.m_iUnitUID;
		kPacketToDBPVPEVENT.m_iToUnitUID   = kPacket_.m_iUnitUID;
		kPacketToDBPVPEVENT.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDBPVPEVENT.m_iRewardID	   = iCheckRewardID; 

		// DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDBPVPEVENT); // 이부분 삽입
		KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
		kPacket.m_iOK					  	   = NetError::ERR_ODBC_01;
		kPacket.m_iRewardLetter.m_iFromUnitUID = kPacketToDBPVPEVENT.m_iFromUnitUID;
		kPacket.m_iRewardLetter.m_iToUnitUID   = kPacketToDBPVPEVENT.m_iToUnitUID;
		kPacket.m_iRewardLetter.m_cScriptType  = kPacketToDBPVPEVENT.m_iRewardType;
		kPacket.m_iRewardLetter.m_iScriptIndex = kPacketToDBPVPEVENT.m_iRewardID;
		kPacket.m_iRewardLetter.m_iQuantity	   = kPacketToDBPVPEVENT.m_sQuantity;
		kPacket.m_iRewardLetter.m_wstrMessage  = kPacketToDBPVPEVENT.m_wstrMessage;

		// 보상
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kPacketToDBPVPEVENT.m_iFromUnitUID
			% kPacketToDBPVPEVENT.m_iToUnitUID
			% kPacketToDBPVPEVENT.m_sQuantity
			% kPacketToDBPVPEVENT.m_iRewardType
			% kPacketToDBPVPEVENT.m_iRewardID
			% kPacketToDBPVPEVENT.m_wstrMessage
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kPacketToDBPVPEVENT.m_iFromUnitUID
			% kPacketToDBPVPEVENT.m_iToUnitUID
			% kPacketToDBPVPEVENT.m_sQuantity
			% kPacketToDBPVPEVENT.m_iRewardType
			% kPacketToDBPVPEVENT.m_iRewardID
			% kPacketToDBPVPEVENT.m_wstrMessage
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_iRewardLetter.m_iPostNo
				>> kPacket.m_iRewardLetter.m_wstrRegDate );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			switch( kPacket.m_iOK )
			{
			default:
				kPacket.m_iOK = NetError::ERR_REWARD_TO_POST_00;
				break;
			}
			iCheckRewardID = 0;
		}	

		if( kPacketToDBPVPEVENT.m_bGameServerEvent )
		{
			SendToServer( DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
		}
		else
		{
			SendToUser( LAST_SENDER_UID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
		}

	// 보상 받았으면 업데이트 하자.
		// 보상정보 업데이트
		if(iCheckRewardID != 0)
		{
			int iOK = NetError::ERR_ODBC_01;
			DO_QUERY( L"exec dbo.P_EventUser_UPD", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % iCheckRewardID);

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA(iOK);			
				m_kODBC.EndFetch();
			}

			if(iOK != NetError::NET_OK)
			{
				START_LOG( cerr, L"KimSukGeun_모모치보상업데이트 실패" )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( iCheckRewardID )
					<< END_LOG;
			}
			else
			{
				kPacketDBEMomotiQuizEventAck.m_iCheckReward = 0;

				START_LOG( clog2, L"KimSukGeun_모모치보상정보 업데이트 성공" )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< END_LOG;
			}
		}
	}
	else if( iCheckReward == 1) // 이미 보상받은 상태
	{
		kPacketDBEMomotiQuizEventAck.m_iCheckReward = 1;
	}
	else if( iCheckReward == 2) // 오답 상태
	{
		kPacketDBEMomotiQuizEventAck.m_iCheckReward = 2;
	}
#ifdef SERV_MOMOTI_EVENT_ADDQUIZ
	else if( iCheckReward == 3) // 대상 캐릭터가 아님.
	{
		kPacketDBEMomotiQuizEventAck.m_iCheckReward = 3;
	}
#endif SERV_MOMOTI_EVENT_ADDQUIZ

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_MOMOTI_QUIZ_EVENT_ACK, kPacketDBEMomotiQuizEventAck );

	START_LOG( cwarn, L"KimSukGeun_모모치 상황 파악하기" )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacketDBEMomotiQuizEventAck.m_iCheckReward )
		<< BUILD_LOG( iCheckRewardID )
		<< BUILD_LOG( iCheckReward )
		<< END_LOG;

}
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
IMPL_ON_FUNC( DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ )
{
	KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_ACK kPacketEventPvpAck;
	kPacketEventPvpAck.m_iOK = NetError::NET_OK;
	kPacketEventPvpAck.m_iRewardBonusItem = kPacket_.m_iRewardBonusItem;

	// 데이터 초기화
	int icheckPost = 0;
	int	iCheckRewardID = 0;
	int iRewardBonusItem = 0;
	int iGetConnectExperience = 0;
	int iCharLevel = 0;

	// 보상관련 정보 확인
	iCharLevel = kPacket_.m_iUnitLevel;
	iGetConnectExperience = kPacket_.m_iGetConnectExperience;
	iRewardBonusItem = kPacket_.m_iRewardBonusItem;

	// 보상조건 확인 접속경험이 있으면1
	if( iGetConnectExperience == 1)
	{
		if(iCharLevel >= 9 && iCharLevel <= 18) //Level. 9~18
		{
			icheckPost = 843; //마법의 목걸이 15일권 큐브 
		}
		else if(iCharLevel >= 19 && iCharLevel <= 30) //Level. 19~30
		{
			icheckPost = 844; //기술의 반지 4종 15일권 큐브
		}
		else if(iCharLevel >= 31 && iCharLevel <= 37) //Level. 31~37
		{
			icheckPost = 845; //아리엘의 강화부적 Lv5
		}
		else if(iCharLevel >= 38 && iCharLevel <= 44) //Level. 38~44
		{
			icheckPost = 846; //그노시스의 축복 10SP 15일권
		}
		else if(iCharLevel >= 45 && iCharLevel <= 51) //Level. 45~51
		{
			icheckPost = 847; //경형치 50% 보너스 메달(3일권)
		}
		else if(iCharLevel >= 52 && iCharLevel <= 63) //Level. 52~60
		{
			icheckPost = 848; //아리엘의 플루오르스톤 LV5 2개
		}

	}


	// 특정상황에서만 우편을 날리자
	if(icheckPost != 0)
	{
		// 이벤트 보상을 주자!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDBPVPEVENT;
		kPacketToDBPVPEVENT.m_iFromUnitUID = kPacket_.m_iUnitUID;
		kPacketToDBPVPEVENT.m_iToUnitUID   = kPacket_.m_iUnitUID;
		kPacketToDBPVPEVENT.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDBPVPEVENT.m_iRewardID	   = icheckPost; 
		iCheckRewardID = kPacketToDBPVPEVENT.m_iRewardID;


		// DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDBPVPEVENT); // 이부분 삽입
		KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
		kPacket.m_iOK					  	   = NetError::ERR_ODBC_01;
		kPacket.m_iRewardLetter.m_iFromUnitUID = kPacketToDBPVPEVENT.m_iFromUnitUID;
		kPacket.m_iRewardLetter.m_iToUnitUID   = kPacketToDBPVPEVENT.m_iToUnitUID;
		kPacket.m_iRewardLetter.m_cScriptType  = kPacketToDBPVPEVENT.m_iRewardType;
		kPacket.m_iRewardLetter.m_iScriptIndex = kPacketToDBPVPEVENT.m_iRewardID;
		kPacket.m_iRewardLetter.m_iQuantity	   = kPacketToDBPVPEVENT.m_sQuantity;
		kPacket.m_iRewardLetter.m_wstrMessage  = kPacketToDBPVPEVENT.m_wstrMessage;

		// 보상
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kPacketToDBPVPEVENT.m_iFromUnitUID
			% kPacketToDBPVPEVENT.m_iToUnitUID
			% kPacketToDBPVPEVENT.m_sQuantity
			% kPacketToDBPVPEVENT.m_iRewardType
			% kPacketToDBPVPEVENT.m_iRewardID
			% kPacketToDBPVPEVENT.m_wstrMessage
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kPacketToDBPVPEVENT.m_iFromUnitUID
			% kPacketToDBPVPEVENT.m_iToUnitUID
			% kPacketToDBPVPEVENT.m_sQuantity
			% kPacketToDBPVPEVENT.m_iRewardType
			% kPacketToDBPVPEVENT.m_iRewardID
			% kPacketToDBPVPEVENT.m_wstrMessage
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_iRewardLetter.m_iPostNo
				>> kPacket.m_iRewardLetter.m_wstrRegDate );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			switch( kPacket.m_iOK )
			{
			default:
				kPacket.m_iOK = NetError::ERR_REWARD_TO_POST_00;
				break;
			}
			iCheckRewardID = 0;
		}	

		if( kPacketToDBPVPEVENT.m_bGameServerEvent )
		{
			SendToServer( DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
		}
		else
		{
			SendToUser( LAST_SENDER_UID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
		}
		
	}

	// 보상정보 업데이트
	if(iCheckRewardID != 0)
	{
		int iOK = NetError::ERR_ODBC_01;
		DO_QUERY( L"exec dbo.P_EventUser_UPD", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA(iOK);			
			m_kODBC.EndFetch();
		}

		if(iOK != NetError::NET_OK)
		{
			START_LOG( cerr, L"7일후의 기적 업데이트 실패" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( iCheckRewardID )
				<< END_LOG;
		}
		else
		{
			//GSUser정보세팅해야함.
			kPacketEventPvpAck.m_iRewardBonusItem = 1;

			START_LOG( clog2, L"7일 후의 기적 보상정보 업데이트 성공" )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< END_LOG;
		}
	}


end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_ACK, kPacketEventPvpAck );

	START_LOG( cwarn, L"7일간의 기전 우편 상황 파악하기" )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( iGetConnectExperience )
		<< BUILD_LOG( iCheckRewardID )
		<< BUILD_LOG( iCharLevel )
		<< BUILD_LOG( kPacketEventPvpAck.m_iRewardBonusItem )
		<< END_LOG;
}
#endif // SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
IMPL_ON_FUNC( DBE_GET_RIDING_PET_LIST_REQ )
{
	KDBE_GET_RIDING_PET_LIST_ACK kPacket;
	kPacket.m_iOK					= NetError::ERR_ODBC_01;
	kPacket.m_iAfterWorkStorageKey	= kPacket_.m_iAfterWorkStorageKey;

	DO_QUERY( L"exec dbo.P_GRiding_SEL", L"%d", % kPacket_.m_iUnitUID );

	while( m_kODBC.Fetch() == true )
	{
		CTime tLastUnSummonDate;
		std::wstring wstrLastUnSummonDate;

		KRidingPetInfo	kInfo;
		USHORT			usStamina;

		FETCH_DATA( kInfo.m_iRidingPetUID
			>> kInfo.m_usRindingPetID
			>> usStamina
			>> kInfo.m_wstrDestroyDate
			>> wstrLastUnSummonDate
			);

		wstrLastUnSummonDate		= wstrLastUnSummonDate.substr( 0, 19 );
		kInfo.m_fStamina			= usStamina;
		kInfo.m_iLastUnSummonDate	= ( KncUtil::ConvertStringToCTime( wstrLastUnSummonDate, tLastUnSummonDate ) == true ) ? tLastUnSummonDate.GetTime() : 0;
		kPacket.m_vecRidingPetList.push_back( kInfo );
	}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_RIDING_PET_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_CREATE_RIDING_PET_REQ )
{
	KDBE_CREATE_RIDING_PET_ACK kPacket;
	kPacket.m_iOK								= NetError::ERR_ODBC_01;
	kPacket.m_kCreatedPetInfo.m_usRindingPetID	= kPacket_.m_usCreatePetID;
	kPacket.m_vecUpdatedInventorySlot			= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		BOOST_MAP_CONST_FOREACH( UidType, int, kItem, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( kItem.first )
				<< BUILD_LOG( kItem.second );
		}
	}
	//}}

	// 라이딩 펫 생성
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GRiding_INS", L"%d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_usCreatePetID
		% kPacket_.m_sPeriod
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GRiding_INT", L"%d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_usCreatePetID
		% kPacket_.m_sPeriod
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kCreatedPetInfo.m_iRidingPetUID
			>> kPacket.m_kCreatedPetInfo.m_wstrDestroyDate
			);

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"라이딩 펫 생성 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_usCreatePetID )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RIDING_PET_05;
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CREATE_RIDING_PET_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_RELEASE_RIDING_PET_REQ )
{
	KEGS_RELEASE_RIDING_PET_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_iRidingPetUID	= kPacket_.m_iRidingPetUID;
	
	DO_QUERY( L"exec dbo.P_GRiding_UPD_Deleted", L"%d, %d",
		% kPacket_.m_iRidingPetUID
		% kPacket_.m_iUnitUID
		);

	if( m_kODBC.BeginFetch() == true )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"라이딩 펫 놓아주기 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket.m_iRidingPetUID )
			<< END_LOG;
		
		kPacket.m_iOK = NetError::ERR_RIDING_PET_11;	// 라이딩 펫 놓아주기에 실패하였습니다.
		goto end_proc;
	}

#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
	// 현재 퀘스트를 보유중이면
	if(kPacket_.m_bEventQuest == true)
	{
		// 이벤트 보상을 주자!
		KDBE_INSERT_REWARD_TO_POST_REQ kPacketToDBPVPEVENT;
		kPacketToDBPVPEVENT.m_iFromUnitUID = kPacket_.m_iUnitUID;
		kPacketToDBPVPEVENT.m_iToUnitUID   = kPacket_.m_iUnitUID;
		kPacketToDBPVPEVENT.m_iRewardType  = KPostItemInfo::LT_EVENT;
		kPacketToDBPVPEVENT.m_iRewardID	   = _CONST_AEVENT_RIDING_WITH_SUB_QUEST::iHasEventQuestRewardID; 

		// DBE_INSERT_REWARD_TO_POST_REQ, kPacketToDBPVPEVENT); // 이부분 삽입
		KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
		kPacket.m_iOK					  	   = NetError::ERR_ODBC_01;
		kPacket.m_iRewardLetter.m_iFromUnitUID = kPacketToDBPVPEVENT.m_iFromUnitUID;
		kPacket.m_iRewardLetter.m_iToUnitUID   = kPacketToDBPVPEVENT.m_iToUnitUID;
		kPacket.m_iRewardLetter.m_cScriptType  = kPacketToDBPVPEVENT.m_iRewardType;
		kPacket.m_iRewardLetter.m_iScriptIndex = kPacketToDBPVPEVENT.m_iRewardID;
		kPacket.m_iRewardLetter.m_iQuantity	   = kPacketToDBPVPEVENT.m_sQuantity;
		kPacket.m_iRewardLetter.m_wstrMessage  = kPacketToDBPVPEVENT.m_wstrMessage;

		// 보상
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kPacketToDBPVPEVENT.m_iFromUnitUID
			% kPacketToDBPVPEVENT.m_iToUnitUID
			% kPacketToDBPVPEVENT.m_sQuantity
			% kPacketToDBPVPEVENT.m_iRewardType
			% kPacketToDBPVPEVENT.m_iRewardID
			% kPacketToDBPVPEVENT.m_wstrMessage
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kPacketToDBPVPEVENT.m_iFromUnitUID
			% kPacketToDBPVPEVENT.m_iToUnitUID
			% kPacketToDBPVPEVENT.m_sQuantity
			% kPacketToDBPVPEVENT.m_iRewardType
			% kPacketToDBPVPEVENT.m_iRewardID
			% kPacketToDBPVPEVENT.m_wstrMessage
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_iRewardLetter.m_iPostNo
				>> kPacket.m_iRewardLetter.m_wstrRegDate );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			switch( kPacket.m_iOK )
			{
			default:
				kPacket.m_iOK = NetError::ERR_REWARD_TO_POST_00;
				break;
			}
		}	

		if( kPacketToDBPVPEVENT.m_bGameServerEvent )
		{
			SendToServer( DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
		}
		else
		{
			SendToUser( LAST_SENDER_UID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
		}		
	}
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RELEASE_RIDING_PET_ACK, kPacket );
}
#endif	// SERV_RIDING_PET_SYSTM

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
_IMPL_ON_FUNC( DBE_COUPLE_PROPOSE_USER_FIND_REQ, KEGS_COUPLE_PROPOSE_REQ )
{
	KDBE_COUPLE_PROPOSE_USER_FIND_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_wstrOtherNickName = kPacket_.m_wstrUnitName;

	DO_QUERY( L"exec dbo.P_GCouple_Info_SEL_Check", L"N\'%s\'", % kPacket_.m_wstrUnitName );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iOK
				 >> kAck.m_iOtherUserUID
				 >> kAck.m_iOtherUnitUID
				 >> kAck.m_cUnitClass
				 >> kAck.m_cRelationshipType
				 );

		m_kODBC.EndFetch();
	}

	// 존재 하지 않는 닉네임
	if( kAck.m_iOK == -1 )
	{
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_COUPLE_PROPOSE_USER_FIND_ACK, kAck );
};

IMPL_ON_FUNC( DBE_COUPLE_MAKING_SUCCESS_REQ )
{
	KDBE_COUPLE_MAKING_SUCCESS_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kAck.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kAck.m_iAcceptUserUID = kPacket_.m_iAcceptUserUID;
	kAck.m_ucAcceptUnitLevel = kPacket_.m_ucAcceptUnitLevel;
	kAck.m_cAcceptUnitClass = kPacket_.m_cAcceptUnitClass;
	kAck.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
	kAck.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	kAck.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	Query_DeleteItem( kPacket_.m_vecDeleted, kAck.m_kItemQuantityUpdate.m_vecDeleted );

	// 현재 시간 기준
	CTime tNow = CTime::GetCurrentTime();

	// 삭제가 모두 성공해야지만 인연 정보 기록
	if( kAck.m_kItemQuantityUpdate.m_vecDeleted.empty() == true )
	{		
		std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		// 애칭은 초기값 - 캐시를 사용하지 않으면 비어있음
		std::wstring wstrLoveWord;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCouple_Info_MER", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
			% 1
			% kAck.m_iRequestUnitUID
			% kAck.m_iAcceptUnitUID
			% wstrNow
			% wstrLoveWord
			% 0 );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCouple_Info_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
				% 1
				% kAck.m_iRequestUnitUID
				% kAck.m_iAcceptUnitUID
				% wstrNow
				% wstrLoveWord
				% 0 );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );

			m_kODBC.EndFetch();
		}
	}
	else
	{
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_11;
	}

	if( kAck.m_iOK == NetError::NET_OK )
	{
		kAck.m_tDate = tNow.GetTime();

		START_LOG( clog, L"커플 맺기 성공!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kAck.m_wstrRequestUnitName )
			<< BUILD_LOG( kAck.m_wstrAcceptUnitName )
			<< BUILD_LOG( static_cast<int>(kAck.m_kItemQuantityUpdate.m_vecDeleted.size()) )
			<< BUILD_LOG( static_cast<int>(kPacket_.m_vecDeleted.size()) )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_COUPLE_MAKING_SUCCESS_ACK, kAck );
}

IMPL_ON_FUNC( DBE_WEDDING_PROPOSE_REQ )
{
	KDBE_WEDDING_PROPOSE_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	
	kAck.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;
	kAck.m_wstrWeddingDate = kPacket_.m_wstrWeddingDate;;
	kAck.m_iManUID = kPacket_.m_iManUID;;
	kAck.m_iGirlUID = kPacket_.m_iGirlUID;;
	kAck.m_cWeddingHallType = kPacket_.m_cWeddingHallType;;
	kAck.m_cOfficiantNPC = kPacket_.m_cOfficiantNPC;;
	kAck.m_wstrWeddingMsg = kPacket_.m_wstrWeddingMsg;;

	// 아이템 삭제
	bool bUpdateFailed = false;
	Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kAck.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kAck.m_kItemQuantityUpdate.m_vecDeleted );

	// 증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}

	// 삭제가 모두 성공해야지만 인연 정보 기록
	if( kAck.m_kItemQuantityUpdate.m_vecDeleted.empty() == true && bUpdateFailed == false )
	{
		// 현재 시간 기준
		CTime tNow = CTime::GetCurrentTime();
		std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );	
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCouple_WeddingInfo_INS", L"%d, %d, %d, %d, N\'%s\', N\'%s\', N\'%s\'", 
			% kPacket_.m_iManUID
			% kPacket_.m_iGirlUID
			% static_cast<int>(kPacket_.m_cWeddingHallType)
			% static_cast<int>(kPacket_.m_cOfficiantNPC)
			% wstrNow
			% kPacket_.m_wstrWeddingDate
			% kPacket_.m_wstrWeddingMsg );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCouple_WeddingInfo_INT", L"%d, %d, %d, %d, N\'%s\', N\'%s\', N\'%s\'", 
			% kPacket_.m_iManUID
			% kPacket_.m_iGirlUID
			% static_cast<int>(kPacket_.m_cWeddingHallType)
			% static_cast<int>(kPacket_.m_cOfficiantNPC)
			% wstrNow
			% kPacket_.m_wstrWeddingDate
			% kPacket_.m_wstrWeddingMsg );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK
					 >> kAck.m_iWeddingUID );

			m_kODBC.EndFetch();
		}

		kAck.m_iOK = NetError::NET_OK;


	}
	else
	{
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_20;
	}

	if( kAck.m_iOK == NetError::NET_OK )
	{
		if( Quety_WeddingLetter( LAST_SENDER_UID, kPacket_, kAck ) == false )
		{
			// 뭔가 실패 했다!
			START_LOG( cerr, L"결혼 관련 우편 발송 실패!!" )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_RELATIONSHIP_20;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_WEDDING_PROPOSE_ACK, kAck );
}

bool KGSGameDBThread::Quety_WeddingLetter( IN UidType iSendUserUID, IN KDBE_WEDDING_PROPOSE_REQ& kPacket_, IN OUT KDBE_WEDDING_PROPOSE_ACK& kAck )
{
	//////////////////////////////////////////////////////////////////////////
	// 우편 발송
	int iOK = NetError::ERR_ODBC_01;

	std::wstring wstReserveTemp;

	char cPostTypeInvitation = KPostItemInfo::LT_WEDDING_INVITATION;
	int iPostInvitationItem = CXSLItem::SI_WEDDING_INVITATION_ITEM;

	int	iPostNo = 0;
	//////////////////////////////////////////////////////////////////////////
	// 신랑
	{
		KPostItemInfo kInfo;
		kInfo.m_iFromUnitUID = kPacket_.m_iManUID;
		kInfo.m_iToUnitUID   = kPacket_.m_iManUID;
		kInfo.m_cScriptType  = KPostItemInfo::LT_WEDDING_RESERVE;
		kInfo.m_iScriptIndex = CXSLItem::SI_WEDDING_RESERVATION_ITEM;
		kInfo.m_iQuantity	   = 1;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kInfo.m_iFromUnitUID
			% kInfo.m_iToUnitUID
			% kInfo.m_iQuantity
			% static_cast<int>(kInfo.m_cScriptType)
			% kInfo.m_iScriptIndex
			% kInfo.m_wstrMessage
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kInfo.m_iFromUnitUID
			% kInfo.m_iToUnitUID
			% kInfo.m_iQuantity
			% static_cast<int>(kInfo.m_cScriptType)
			% kInfo.m_iScriptIndex
			% kInfo.m_wstrMessage
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK
				>> kInfo.m_iPostNo
				>> kInfo.m_wstrRegDate );
			m_kODBC.EndFetch();
		}

		if( iOK == NetError::NET_OK )
		{
			KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
			kPacket.m_iOK = iOK;
			kPacket.m_iRewardLetter = kInfo;
			SendToUser( iSendUserUID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );

			iOK = NetError::ERR_ODBC_01;

			// Gpost_Wedding 에 정보 추가
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_Wedding_INS", L"%d, %d",
				% kInfo.m_iPostNo
				% kAck.m_iWeddingUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_Gpost_Wedding_INT", L"%d, %d",
				% kInfo.m_iPostNo
				% kAck.m_iWeddingUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK == NetError::NET_OK )
			{
				START_LOG( cerr, L"우편의 Wedding 정보 insert 성공" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kInfo.m_iPostNo )
					<< BUILD_LOG( kAck.m_iWeddingUID )
					<< END_LOG;

				kInfo.m_iFromUnitUID = kPacket_.m_iManUID;
				kInfo.m_iToUnitUID   = kPacket_.m_iManUID;
				kInfo.m_cScriptType  = KPostItemInfo::LT_WEDDING_REWARD;
				kInfo.m_iScriptIndex = CXSLItem::SI_WEDDING_DRESS_ITEM;
				kInfo.m_iQuantity	   = 1;
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
					% kInfo.m_iFromUnitUID
					% kInfo.m_iToUnitUID
					% kInfo.m_iQuantity
					% static_cast<int>(kInfo.m_cScriptType)
					% kInfo.m_iScriptIndex
					% kInfo.m_wstrMessage
					);
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
					% kInfo.m_iFromUnitUID
					% kInfo.m_iToUnitUID
					% kInfo.m_iQuantity
					% static_cast<int>(kInfo.m_cScriptType)
					% kInfo.m_iScriptIndex
					% kInfo.m_wstrMessage
					);
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK
						>> kInfo.m_iPostNo
						>> kInfo.m_wstrRegDate );
					m_kODBC.EndFetch();
				}

				if( iOK == NetError::NET_OK )
				{
					KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
					kPacket.m_iOK = iOK;
					kPacket.m_iRewardLetter = kInfo;
					SendToUser( iSendUserUID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
				}
			}
		}
		else
		{
			START_LOG( cerr, L"결혼식 예약 우편 발송 실패" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kInfo.m_iFromUnitUID )
				<< BUILD_LOG( kInfo.m_iToUnitUID )
				<< BUILD_LOG( kInfo.m_iQuantity )
				<< BUILD_LOGc( kInfo.m_cScriptType )
				<< BUILD_LOG( kInfo.m_iScriptIndex )
				<< END_LOG;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 신부
	{
		KPostItemInfo kInfo;
		kInfo.m_iFromUnitUID = kPacket_.m_iGirlUID;
		kInfo.m_iToUnitUID   = kPacket_.m_iGirlUID;
		kInfo.m_cScriptType  = KPostItemInfo::LT_WEDDING_RESERVE;
		kInfo.m_iScriptIndex = CXSLItem::SI_WEDDING_RESERVATION_ITEM;
		kInfo.m_iQuantity	   = 1;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kInfo.m_iFromUnitUID
			% kInfo.m_iToUnitUID
			% kInfo.m_iQuantity
			% static_cast<int>(kInfo.m_cScriptType)
			% kInfo.m_iScriptIndex
			% kInfo.m_wstrMessage
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kInfo.m_iFromUnitUID
			% kInfo.m_iToUnitUID
			% kInfo.m_iQuantity
			% static_cast<int>(kInfo.m_cScriptType)
			% kInfo.m_iScriptIndex
			% kInfo.m_wstrMessage
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK
				>> kInfo.m_iPostNo
				>> kInfo.m_wstrRegDate );
			m_kODBC.EndFetch();
		}

		if( iOK == NetError::NET_OK )
		{
			KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
			kPacket.m_iOK = iOK;
			kPacket.m_iRewardLetter = kInfo;
			SendToUser( iSendUserUID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );

			iOK = NetError::ERR_ODBC_01;

			// Gpost_Wedding 에 정보 추가
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_Wedding_INS", L"%d, %d",
				% kInfo.m_iPostNo
				% kAck.m_iWeddingUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_Gpost_Wedding_INT", L"%d, %d",
				% kInfo.m_iPostNo
				% kAck.m_iWeddingUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK == NetError::NET_OK )
			{
				START_LOG( cerr, L"우편의 Wedding 정보 insert 성공" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kInfo.m_iPostNo )
					<< BUILD_LOG( kAck.m_iWeddingUID )
					<< END_LOG;

				kInfo.m_iFromUnitUID = kPacket_.m_iGirlUID;
				kInfo.m_iToUnitUID   = kPacket_.m_iGirlUID;
				kInfo.m_cScriptType  = KPostItemInfo::LT_WEDDING_REWARD;
				kInfo.m_iScriptIndex = CXSLItem::SI_WEDDING_DRESS_ITEM;
				kInfo.m_iQuantity	   = 1;
#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
					% kInfo.m_iFromUnitUID
					% kInfo.m_iToUnitUID
					% kInfo.m_iQuantity
					% static_cast<int>(kInfo.m_cScriptType)
					% kInfo.m_iScriptIndex
					% kInfo.m_wstrMessage
					);
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
					% kInfo.m_iFromUnitUID
					% kInfo.m_iToUnitUID
					% kInfo.m_iQuantity
					% static_cast<int>(kInfo.m_cScriptType)
					% kInfo.m_iScriptIndex
					% kInfo.m_wstrMessage
					);
#endif //SERV_ALL_RENEWAL_SP
				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK
						>> kInfo.m_iPostNo
						>> kInfo.m_wstrRegDate );
					m_kODBC.EndFetch();
				}

				if( iOK == NetError::NET_OK )
				{
					KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
					kPacket.m_iOK = iOK;
					kPacket.m_iRewardLetter = kInfo;
					SendToUser( iSendUserUID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
				}
			}
		}
		else
		{
			START_LOG( cerr, L"결혼식 예약 우편 발송 실패" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kInfo.m_iFromUnitUID )
				<< BUILD_LOG( kInfo.m_iToUnitUID )
				<< BUILD_LOG( kInfo.m_iQuantity )
				<< BUILD_LOGc( kInfo.m_cScriptType )
				<< BUILD_LOG( kInfo.m_iScriptIndex )
				<< END_LOG;
		}
	}
	// 하객
	BOOST_TEST_FOREACH( UidType, iRecvUID, kPacket_.m_vecInviteUnitList )
	{
		KPostItemInfo kInfo;
		kInfo.m_iFromUnitUID = kPacket_.m_iManUID;
		kInfo.m_iToUnitUID   = iRecvUID;
		kInfo.m_cScriptType  = KPostItemInfo::LT_WEDDING_INVITATION;
		kInfo.m_iScriptIndex = CXSLItem::SI_WEDDING_INVITATION_ITEM;
		kInfo.m_iQuantity	   = 1;
		kInfo.m_wstrFromNickName = kPacket_.m_wstrWeddingNickName;
		kInfo.m_wstrMessage = kPacket_.m_wstrWeddingMsg;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kInfo.m_iFromUnitUID
			% kInfo.m_iToUnitUID
			% kInfo.m_iQuantity
			% static_cast<int>(kInfo.m_cScriptType)
			% kInfo.m_iScriptIndex
			% kInfo.m_wstrMessage
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
			% kInfo.m_iFromUnitUID
			% kInfo.m_iToUnitUID
			% kInfo.m_iQuantity
			% static_cast<int>(kInfo.m_cScriptType)
			% kInfo.m_iScriptIndex
			% kInfo.m_wstrMessage
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK
				>> kInfo.m_iPostNo
				>> kInfo.m_wstrRegDate );
			m_kODBC.EndFetch();
		}

		if( iOK == NetError::NET_OK )
		{
			KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
			kPacket.m_iOK = iOK;
			kPacket.m_iRewardLetter = kInfo;
			SendToUser( iSendUserUID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );

			iOK = NetError::ERR_ODBC_01;

			// Gpost_Wedding 에 정보 추가
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPost_Wedding_INS", L"%d, %d",
				% kInfo.m_iPostNo
				% kAck.m_iWeddingUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_Gpost_Wedding_INT", L"%d, %d",
				% kInfo.m_iPostNo
				% kAck.m_iWeddingUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK == NetError::NET_OK )
			{
				START_LOG( cerr, L"우편의 Wedding 정보 insert 성공" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kInfo.m_iPostNo )
					<< BUILD_LOG( kAck.m_iWeddingUID )
					<< END_LOG;
			}
		}
		else
		{
			START_LOG( cerr, L"결혼식 예약 우편 발송 실패" )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kInfo.m_iFromUnitUID )
				<< BUILD_LOG( kInfo.m_iToUnitUID )
				<< BUILD_LOG( kInfo.m_iQuantity )
				<< BUILD_LOGc( kInfo.m_cScriptType )
				<< BUILD_LOG( kInfo.m_iScriptIndex )
				<< END_LOG;
		}
	}

	return true;

end_proc:
	return false;
}

bool KGSGameDBThread::Query_UpdateLastLogOffDate( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq )
{
	if( kReq.m_cUnitRelationshipType == SEnum::RT_SOLO )
	{
		return true;
	}

	int iOK = NetError::ERR_ODBC_01;
	CTime tCurr = CTime::GetCurrentTime();
	std::wstring wstrLastDate;
	wstrLastDate = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	// 마지막 위치 저장
	DO_QUERY( L"exec dbo.P_GCouple_Info_UPD_LastLogOffDate", L"%d, N\'%s\'",
		% kReq.m_iUnitUID
		% wstrLastDate
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"유닛 마지막 로그오프 시간 업데이트 실패." )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kReq.m_iUnitUID )
			<< BUILD_LOG( wstrLastDate )
			<< END_LOG;
		return false;
	}

	return true;
}

#ifdef SERV_SKILL_PAGE_SYSTEM

void KGSGameDBThread::LogAboutCheckingTooManySkillDatas( IN const KDBE_EXPIRE_CASH_SKILL_POINT_REQ& kPacket_ ) const
{
	UINT uiSizeOfEverySkillData = 0;
	
	for ( UINT i = 0; i < kPacket_.m_vecRetrievedSkillPageData.size(); i++ )
		uiSizeOfEverySkillData += kPacket_.m_vecRetrievedSkillPageData[i].m_vecUserSkillData.size();

	if ( uiSizeOfEverySkillData > 300L )
	{
		START_LOG( cerr, L"too many skill update on cash skill point expiration!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( (int) uiSizeOfEverySkillData )
			<< END_LOG;
	}
}

// void KGSGameDBThread::DoQueryUpdatingSkillInfo( OUT int& iOk_, IN const UidType lastSenderUID_,
// 	IN const UidType iUnitUID_,	IN const KRetrievedSkillPageData& retrievedSkillPageData_ )
// {
// 	for ( UINT i = 0; i < retrievedSkillPageData_.m_vecUserSkillData.size(); ++i )
// 	{
// 		const KUserSkillData& userSkillData = retrievedSkillPageData_.m_vecUserSkillData[i];
// 
// 		DO_QUERY( L"exec dbo.gup_update_skill_info_new", L"%d, %d, %d, %d",
// 			% iUnitUID_ 
// 			% userSkillData.m_iSkillID
// 			% userSkillData.m_cSkillLevel
// 			% userSkillData.m_cSkillCSPoint );
// 
// 		int iResult = NetError::ERR_UNKNOWN;
// 		if( m_kODBC.BeginFetch() )
// 		{
// 			FETCH_DATA( iResult );
// 			m_kODBC.EndFetch();
// 		}
// 
// 		if( iResult != NetError::NET_OK )
// 		{
// 			START_LOG( cerr, L"failed to update skill on cash skill point expire!" )
// 				<< BUILD_LOG( lastSenderUID_ )
// 				<< BUILD_LOG( iUnitUID_ )
// 				<< BUILD_LOG( retrievedSkillPageData_.m_iRetrievedSPoint )
// 				<< BUILD_LOG( userSkillData.m_iSkillID )
// 				<< BUILD_LOG( userSkillData.m_cSkillLevel )
// 				<< BUILD_LOG( userSkillData.m_cSkillCSPoint )
// 				<< BUILD_LOG( iOk_ )
// 				<< END_LOG;
// 
// 			iOk_ = NetError::ERR_SKILL_22;
// 		}
// 	}
// }


#endif // SERV_SKILL_PAGE_SYSTEM

_IMPL_ON_FUNC( DBE_CHECK_NICK_NAME_REQ, KEGS_CHECK_NICK_NAME_REQ )
{
	KEGS_CHECK_NICK_NAME_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_.m_wstrNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_name", L"N\'%s\'", % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iUnitUID );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iUnitUID > 0 )
	{
		kPacket.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHECK_NICK_NAME_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_WEDDING_ITEM_INFO_REQ, KEGS_WEDDING_ITEM_INFO_REQ )
{
	KEGS_WEDDING_ITEM_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	bool bAllFind = true;

	BOOST_TEST_FOREACH( UidType, iItemUID, kPacket_.m_vecWeddingItemList )
	{
		int iWeddingUID = 0;
		DO_QUERY( L"exec dbo.P_GCouple_WeddingInvitation_SEL", L"%d", % iItemUID );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iWeddingUID );
			m_kODBC.EndFetch();
		}

		if( iWeddingUID > 0 )
		{
			KWeddingItemInfo kInfo;
			kInfo.m_iItemUID = iItemUID;
			kInfo.m_iWeddingUID = iWeddingUID;
			kPacket.m_mapWeddingItemInfo.insert( std::make_pair( kInfo.m_iItemUID, kInfo ) );
		}
		else
		{
			START_LOG( cerr, L"아이템에 매칭된 웨딩 정보가 없습니다." )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			bAllFind = false;
			kPacket.m_iOK = NetError::ERR_RELATIONSHIP_24;
			break;
		}
	}

	if( bAllFind == true )
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_WEDDING_ITEM_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_WEDDING_ITEM_DELETE_REQ )
{
	KDBE_WEDDING_ITEM_DELETE_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_mapWeddingItemInfo = kPacket_.m_mapWeddingItemInfo;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	Query_DeleteItem( kPacket_.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	SendToUser( LAST_SENDER_UID, DBE_WEDDING_ITEM_DELETE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BREAK_UP_REQ )
{
	KDBE_BREAK_UP_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iCost = kPacket_.m_iCost;
	kPacket.m_iMyUnitUID = kPacket_.m_iMyUnitUID;
	kPacket.m_iLoverUnitUID = kPacket_.m_iLoverUnitUID;

	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	std::wstring wstrTemp;
	// 타입이 3 이면 이별
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GCouple_Info_MER", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
		% 3	
		% kPacket.m_iMyUnitUID
		% kPacket.m_iLoverUnitUID
		% wstrNow
		% wstrTemp
		% 0	);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GCouple_Info_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
		% 3	
		% kPacket.m_iMyUnitUID
		% kPacket.m_iLoverUnitUID
		% wstrNow
		% wstrTemp
		% 0	);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"헤어지기 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iMyUnitUID )
			<< BUILD_LOG( kPacket_.m_iLoverUnitUID )
			<< BUILD_LOG( kPacket_.m_iCost )
			<< END_LOG;

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_BREAK_UP_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ADMIN_CHANGE_COUPLE_DATE_REQ )
{
	KEGS_ADMIN_CHANGE_COUPLE_DATE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	
	DO_QUERY( L"exec dbo.P_GCouple_Info_UPD_RegDate", L"%d, %d, %f", % kPacket_.m_iUnitUID % kPacket_.m_iLoverUnitUID % kPacket_.m_fAddRate );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"커플 날짜 변경 오류 치트키 오류" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iLoverUnitUID )
			<< BUILD_LOG( kPacket_.m_fAddRate )
			<< END_LOG;
	
		goto end_proc;
	}

	kPacket.m_iOK = NetError::NET_OK;

	START_LOG( cout, L"커플 날짜 변경 성공!" )
		<< BUILD_LOG( kPacket.m_iOK )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_iLoverUnitUID )
		<< BUILD_LOG( kPacket_.m_fAddRate )
		<< END_LOG;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_CHANGE_COUPLE_DATE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_CHANGE_LOVE_WORD_REQ )
{
	KDBE_CHANGE_LOVE_WORD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUseUnitUID = kPacket_.m_iUseUnitUID;
    kPacket.m_iLoverUnitUID = kPacket_.m_iLoverUnitUID;
	kPacket.m_wstrNewLoveWord = kPacket_.m_wstrNewLoveWord;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	// 아이템 삭제
	bool bUpdateFailed = false;
	Query_UpdateItemQuantity( kPacket_.m_iUseUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	// 증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUseUnitUID )
			<< END_LOG;

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}


	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	std::wstring wstrTemp;
	// 타입이 3 이면 이별
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GCouple_Info_MER", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
		% 4	
		% kPacket_.m_iUseUnitUID
		% kPacket_.m_iLoverUnitUID
		% wstrNow
		% kPacket_.m_wstrNewLoveWord
		% 0	);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GCouple_Info_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
		% 4	
		% kPacket_.m_iUseUnitUID
		% kPacket_.m_iLoverUnitUID
		% wstrNow
		% kPacket_.m_wstrNewLoveWord
		% 0	);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"애칭 변경 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUseUnitUID )
			<< BUILD_LOG( kPacket_.m_iLoverUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrNewLoveWord )
			<< END_LOG;

		goto end_proc;
	}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_LOVE_WORD_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_WEDDING_REWARD_REQ )
{
	KDBE_INSERT_WEDDING_REWARD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_bTitleReward = kPacket_.m_bTitleReward;
	kPacket.m_iTitleRewardStep = kPacket_.m_iTitleRewardStep;

	// 우편 발송 sp
	KPostItemInfo kInfo;
	kInfo.m_iFromUnitUID = kPacket_.kPostReq.m_iFromUnitUID;
	kInfo.m_iToUnitUID   = kPacket_.kPostReq.m_iToUnitUID;
	kInfo.m_cScriptType  = kPacket_.kPostReq.m_iRewardType;
	kInfo.m_iScriptIndex = kPacket_.kPostReq.m_iRewardID;
	kInfo.m_iQuantity	   = 1;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kInfo.m_iFromUnitUID
		% kInfo.m_iToUnitUID
		% kInfo.m_iQuantity
		% static_cast<int>(kInfo.m_cScriptType)
		% kInfo.m_iScriptIndex
		% kInfo.m_wstrMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kInfo.m_iFromUnitUID
		% kInfo.m_iToUnitUID
		% kInfo.m_iQuantity
		% static_cast<int>(kInfo.m_cScriptType)
		% kInfo.m_iScriptIndex
		% kInfo.m_wstrMessage
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kInfo.m_iPostNo
			>> kInfo.m_wstrRegDate );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK == NetError::NET_OK )
	{
		// 우편 발송 결과 전송
		KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_iRewardLetter = kInfo;
		SendToUser( LAST_SENDER_UID, DBE_INSERT_REWARD_TO_POST_ACK, kPacket );

		// DB 인연 정보 갱신
		int iType = 0;
		if( kPacket_.m_bTitleReward == true )
		{
			iType = 6;	// 타이틀 보상 정보 업데이트
		}
		else
		{
			iType = 5;	// 일반 보상
		}

		CTime tCurr = CTime::GetCurrentTime();
		std::wstring wstrNow = tCurr.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
		std::wstring wstrLoveWord;

		kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCouple_Info_MER", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
			% iType
			% kPacket_.m_iUnitUID
			% kPacket_.m_iLoverUnitUID
			% wstrNow
			% wstrLoveWord
			% kPacket_.m_iTitleRewardStep );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GCouple_Info_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\', %d", 
			% iType
			% kPacket_.m_iUnitUID
			% kPacket_.m_iLoverUnitUID
			% wstrNow
			% wstrLoveWord
			% kPacket_.m_iTitleRewardStep );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );

			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"인연 정보 갱신 오류!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iLoverUnitUID )
			<< BUILD_LOG( iType )
			<< BUILD_LOG( kPacket_.m_iTitleRewardStep )
			<< END_LOG;
		}
	}
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_INSERT_WEDDING_REWARD_ACK, kPacket );
}

#endif SERV_RELATIONSHIP_SYSTEM
//}

//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
IMPL_ON_FUNC( DBE_ITEM_EVALUATE_REQ )
{
	KDBE_ITEM_EVALUATE_ACK kPacket;
	kPacket.m_vecRandomSocket = kPacket_.m_vecRandomSocket;
	kPacket.m_iItemUID = kPacket_.m_iItemUID;
	kPacket.m_iCost = kPacket_.m_iCost;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	int arrSocketInfo[CXSLSocketItem::RSC_MAX] = {0,}; // DB 테이블의 소켓테이블 참고	

	std::vector< int >::const_iterator vit = kPacket.m_vecRandomSocket.begin();
	for( int iIdx = 0; iIdx < CXSLSocketItem::RSC_MAX; ++iIdx )
	{		
		if( vit == kPacket.m_vecRandomSocket.end() )
			continue;

		arrSocketInfo[iIdx] = *vit;
		++vit;
	}

	// 랜덤 소켓 업데이트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_Random_MER", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID		// @iUnitUID bigint
		% kPacket_.m_iItemUID		// @iItemUID bigint
		% arrSocketInfo[0]			// @iSoket1 smallint
		% arrSocketInfo[1]			// @iSoket2 smallint
		% arrSocketInfo[2]			// @iSoket3 smallint
		% arrSocketInfo[3]			// @iSoket4 smallint
		% arrSocketInfo[4]			// @iSoket4 smallint
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_Random_INT_UPD", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID		// @iUnitUID bigint
		% kPacket_.m_iItemUID		// @iItemUID bigint
		% arrSocketInfo[0]			// @iSoket1 smallint
		% arrSocketInfo[1]			// @iSoket2 smallint
		% arrSocketInfo[2]			// @iSoket3 smallint
		% arrSocketInfo[3]			// @iSoket4 smallint
		% arrSocketInfo[4]			// @iSoket4 smallint
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iItemUID )
			<< BUILD_LOG( arrSocketInfo[0] )
			<< BUILD_LOG( arrSocketInfo[1] )
			<< BUILD_LOG( arrSocketInfo[2] )
			<< BUILD_LOG( arrSocketInfo[3] )
			<< BUILD_LOG( arrSocketInfo[4] )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_EVALUATE_ITEM_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ITEM_EVALUATE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_RESTORE_ITEM_EVALUATE_REQ )
{
	KDBE_RESTORE_ITEM_EVALUATE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iTargetItemUID = kPacket_.m_iTargetItemUID;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	int arrSocketInfo[CXSLSocketItem::RSC_MAX] = {0,}; // DB 테이블의 소켓테이블 참고	

	// 랜덤 소켓 업데이트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_Random_MER", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID		// @iUnitUID bigint
		% kPacket_.m_iTargetItemUID	// @iItemUID bigint
		% arrSocketInfo[0]			// @iSoket1 smallint
		% arrSocketInfo[1]			// @iSoket2 smallint
		% arrSocketInfo[2]			// @iSoket3 smallint
		% arrSocketInfo[3]			// @iSoket4 smallint
		% arrSocketInfo[4]			// @iSoket4 smallint
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_Random_INT_UPD", L"%d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID		// @iUnitUID bigint
		% kPacket_.m_iTargetItemUID	// @iItemUID bigint
		% arrSocketInfo[0]			// @iSoket1 smallint
		% arrSocketInfo[1]			// @iSoket2 smallint
		% arrSocketInfo[2]			// @iSoket3 smallint
		% arrSocketInfo[3]			// @iSoket4 smallint
		% arrSocketInfo[4]			// @iSoket4 smallint
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"아이템 소켓 정보 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iTargetItemUID )
			<< BUILD_LOG( arrSocketInfo[0] )
			<< BUILD_LOG( arrSocketInfo[1] )
			<< BUILD_LOG( arrSocketInfo[2] )
			<< BUILD_LOG( arrSocketInfo[3] )
			<< BUILD_LOG( arrSocketInfo[4] )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_EVALUATE_ITEM_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RESTORE_ITEM_EVALUATE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ITEM_CONVERT_REQ )
{
	KDBE_ITEM_CONVERT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iCommissionED = kPacket_.m_iCommissionED;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( Query_InsertItemList( SEnum::GIR_RANDOM_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo, false ) == false )
	{
		START_LOG( cerr, L"아이템 교환시 아이템 DB 추가 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RANDOM_ITEM_06;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	SendToUser( LAST_SENDER_UID, DBE_ITEM_CONVERT_ACK, kPacket );
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// 작업날짜: 2013-05-13	// 박세훈
IMPL_ON_FUNC( DBE_SEND_LOVE_LETTER_EVENT_REQ )
{
	KDBE_SEND_LOVE_LETTER_EVENT_ACK kPacket;
	kPacket.m_iOK						= NetError::ERR_ODBC_01;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

	// 아이템 삭제
	bool bUpdateFailed = false;
	Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	// 증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}

	// 삭제가 성공해야지만 우편 정보 기록
	if( ( kPacket.m_kItemQuantityUpdate.m_vecDeleted.empty() == true ) &&
		( bUpdateFailed == false )
		)
	{
		DO_QUERY( L"exec dbo.P_GEvent_Letter_INT", L"%d, N\'%s\'",
			% kPacket_.m_iUnitUID
			% kPacket_.m_wstrMessage
			);

		if( m_kODBC.BeginFetch() )
		{
			bool bIsFirstSend;
			FETCH_DATA( kPacket.m_iOK
				>> bIsFirstSend
				);
			m_kODBC.EndFetch();

			if( kPacket.m_iOK == NetError::NET_OK )
			{
				if( bIsFirstSend == true )
				{
					kPacket.m_iOK = NetError::ERR_REQUEST_OF_LOVE_01;
				}
			}
			else
			{
				START_LOG( cerr, L"뽀루의 러브 레터 DB 저장 실패" )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< END_LOG;
			}
		}
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_REQUEST_OF_LOVE_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_SEND_LOVE_LETTER_EVENT_ACK, kPacket );
}
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈
IMPL_ON_FUNC( DBE_JUMPING_CHARACTER_UPDATE_REQ )
{
	// 점핑 전용 SP를 만들어야 한다.

	KDBE_JUMPING_CHARACTER_UPDATE_ACK kPacket;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	// 점핑 기록
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_Jumping_INS", L"%d, %d, N\'%s\'",
		% LAST_SENDER_UID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrRegDate
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GEvent_Jumping_INT", L"%d, %d, N\'%s\'",
		% LAST_SENDER_UID
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrRegDate
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		goto end_proc;
	}

	kPacket.m_iOK					= NetError::NET_OK;
	kPacket.m_iUnitUID				= kPacket_.m_iUnitUID;
	kPacket.m_iLevel				= kPacket_.m_iLevel;
	kPacket.m_cExpandedMaxPageNum	= -1;
	kPacket.m_iClass				= -1;

	// 1. 기술의 노트 페이지 처리
	if( 0 <= kPacket_.m_cExpandedMaxPageNum )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GNote_PageCNT_MER", L"%d, %d",
			% kPacket_.m_iUnitUID
			% static_cast<int>( kPacket_.m_cExpandedMaxPageNum )
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_notecnt", L"%d, %d",
			% kPacket_.m_iUnitUID
			% static_cast<int>( kPacket_.m_cExpandedMaxPageNum )
			);
#endif //SERV_ALL_RENEWAL_SP
		int iOK = NetError::ERR_ODBC_01;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK == NetError::NET_OK )
		{
			kPacket.m_cExpandedMaxPageNum = kPacket_.m_cExpandedMaxPageNum;

			// 로그
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GJumpingLog_INS", L"%d, %d, %d, N\'%s\'",
				% LAST_SENDER_UID
				% kPacket_.m_iUnitUID
				% 1
				% kPacket_.m_wstrRegDate
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GJumpingLog_INT", L"%d, %d, %d, N\'%s\'",
				% LAST_SENDER_UID
				% kPacket_.m_iUnitUID
				% 1
				% kPacket_.m_wstrRegDate
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}
		}
	}

	// 2. Class 및 기본 스킬 처리
	{
#ifdef SERV_SKILL_PAGE_SYSTEM
		int iOK = NetError::NET_OK;
		const int iMax = static_cast<int>( kPacket_.m_vecNewDefaultSkill.size() / 2 );
		
		for ( int iSkillPagesNumber = 1; iSkillPagesNumber <= kPacket_.m_iTheNumberOfSkillPagesAvailable; iSkillPagesNumber++ )
		{
			for( int i = 0; i < iMax; ++i )
			{
				const int iNewDefaultSkill1 = kPacket_.m_vecNewDefaultSkill[i * 2];
				const int iNewDefaultSkill2 = kPacket_.m_vecNewDefaultSkill[i * 2 + 1];

				DO_QUERY( L"exec P_GUnit_UPD_UnitClass_20131212", L"%d, %d, %d, %d, %d",
					% kPacket_.m_iUnitUID
					% kPacket_.m_iClass
					% iNewDefaultSkill1
					% iNewDefaultSkill2
					% iSkillPagesNumber
					);

				int iTemp = NetError::ERR_ODBC_01;

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iTemp );
					m_kODBC.EndFetch();
				}

				if( iTemp == NetError::NET_OK )
				{
					kPacket.m_iClass = kPacket_.m_iClass;
					kPacket.m_vecNewDefaultSkill.push_back( iNewDefaultSkill1 );
					kPacket.m_vecNewDefaultSkill.push_back( iNewDefaultSkill2 );
				}
				else
				{
					iOK = iTemp;
				}
			}
		}
#else // SERV_SKILL_PAGE_SYSTEM
		int iOK = NetError::NET_OK;
		const int iMax = static_cast<int>( kPacket_.m_vecNewDefaultSkill.size() / 2 );
		for( int i = 0; i < iMax; ++i )
		{
			const int iNewDefaultSkill1 = kPacket_.m_vecNewDefaultSkill[i * 2];
			const int iNewDefaultSkill2 = kPacket_.m_vecNewDefaultSkill[i * 2 + 1];
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec P_GUnit_UPD_UnitClass_New", L"%d, %d, %d, %d",
				% kPacket_.m_iUnitUID
				% kPacket_.m_iClass
				% iNewDefaultSkill1
				% iNewDefaultSkill2
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec P_GUnit_UPD_UnitClass", L"%d, %d, %d, %d",
				% kPacket_.m_iUnitUID
				% kPacket_.m_iClass
				% iNewDefaultSkill1
				% iNewDefaultSkill2
				);
#endif //SERV_ALL_RENEWAL_SP
			int iTemp = NetError::ERR_ODBC_01;

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iTemp );
				m_kODBC.EndFetch();
			}

			if( iTemp == NetError::NET_OK )
			{
				kPacket.m_iClass = kPacket_.m_iClass;
				kPacket.m_vecNewDefaultSkill.push_back( iNewDefaultSkill1 );
				kPacket.m_vecNewDefaultSkill.push_back( iNewDefaultSkill2 );
			}
			else
			{
				iOK = iTemp;
			}
		}
#endif // SERV_SKILL_PAGE_SYSTEM

		if( iOK == NetError::NET_OK )
		{
			// 로그
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GJumpingLog_INS", L"%d, %d, %d, N\'%s\'",
				% LAST_SENDER_UID
				% kPacket_.m_iUnitUID
				% 2
				% kPacket_.m_wstrRegDate
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GJumpingLog_INT", L"%d, %d, %d, N\'%s\'",
				% LAST_SENDER_UID
				% kPacket_.m_iUnitUID
				% 2
				% kPacket_.m_wstrRegDate
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}
		}
	}

	// 3. 봉인 스킬 처리
	{
		int iOK = NetError::NET_OK;
		BOOST_TEST_FOREACH( short, iSkillID, kPacket_.m_vecUnSealedSkill )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_Unsealed_INS", L"%d, %d",
				% kPacket_.m_iUnitUID
				% iSkillID
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_insert_unsealed_skill", L"%d, %d",
				% kPacket_.m_iUnitUID
				% iSkillID
				);
#endif //SERV_ALL_RENEWAL_SP
			int iTemp = NetError::ERR_ODBC_01;
			
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iTemp );
				m_kODBC.EndFetch();
			}

			if( iTemp == NetError::NET_OK )
			{
				kPacket.m_vecUnSealedSkill.push_back( iSkillID );
			}
			else
			{
				iOK = iTemp;
			}
		}

		if( iOK == NetError::NET_OK )
		{
			// 로그
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GJumpingLog_INS", L"%d, %d, %d, N\'%s\'",
				% LAST_SENDER_UID
				% kPacket_.m_iUnitUID
				% 3
				% kPacket_.m_wstrRegDate
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GJumpingLog_INT", L"%d, %d, %d, N\'%s\'",
				% LAST_SENDER_UID
				% kPacket_.m_iUnitUID
				% 3
				% kPacket_.m_wstrRegDate
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_JUMPING_CHARACTER_UPDATE_ACK, kPacket );
}
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_COUPON_EVENT
IMPL_ON_FUNC( DBE_COUPON_ENTRY_REQ )
{
	KDBE_COUPON_ENTRY_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}

	if( bUpdateFailed == false && kPacket.m_kItemQuantityUpdate.m_vecDeleted.size() == 0 )
	{
		KELOG_COUPON_EVENT_NOT kPacketNot;
		kPacketNot.m_iUserUID = kPacket_.m_iUserUID;
		kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
		kPacketNot.m_iCouponType = kPacket_.m_iCouponType;
		SendToLogDB( ELOG_COUPON_EVENT_NOT, kPacketNot );

		kPacket.m_iOK = NetError::NET_OK;
	}
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_COUPON_ENTRY_ACK, kPacket );
}
#endif SERV_COUPON_EVENT

#ifdef SERV_READY_TO_SOSUN_EVENT
IMPL_ON_FUNC( DBE_READY_TO_SOSUN_EVENT_REQ )
{
	KDBE_READY_TO_SOSUN_EVENT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_mapInsertedItem			= kPacket_.m_mapInsertedItem;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_iFirstUnitClass			= kPacket_.m_iFirstUnitClass;
	
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	if( Query_InsertItemList( SEnum::GIR_ENCHANTMENT_EXTRACTION_ITEM, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#else
	if( Query_InsertItemList( kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo ) == false )
#endif SERV_GET_ITEM_REASON
		//}}
	{
		START_LOG( cerr, L"아라 1차 전직 이름 맞추기 이벤트 보상 아이템 획득 실패" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RANDOM_ITEM_06;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	SendToUser( LAST_SENDER_UID, DBE_READY_TO_SOSUN_EVENT_ACK, kPacket );
}
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
_IMPL_ON_FUNC( DBE_EVENT_PROPOSE_USER_FIND_REQ, KEGS_USE_PROPOSE_ITEM_REQ )
{
	KDBE_EVENT_PROPOSE_USER_FIND_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_wstrOtherNickName = kPacket_.m_wstrNickName;

	DO_QUERY( L"exec dbo.P_GEvent_Couple_CHK", L"N\'%s\'", % kPacket_.m_wstrNickName );

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iOK
			>> kAck.m_iOtherUnitUID
			>> kAck.m_iOtherUserUID
			>> kAck.m_cUnitClass
			>> kAck.m_bCouple
			);

		m_kODBC.EndFetch();
	}

	// 존재 하지 않는 닉네임
	if( kAck.m_iOK == -1 )
	{
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_EVENT_PROPOSE_USER_FIND_ACK, kAck );
}

IMPL_ON_FUNC( DBE_EVENT_MAKING_SUCCESS_REQ )
{
	KDBE_EVENT_MAKING_SUCCESS_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kAck.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kAck.m_iAcceptUserUID = kPacket_.m_iAcceptUserUID;
	kAck.m_ucAcceptUnitLevel = kPacket_.m_ucAcceptUnitLevel;
	kAck.m_cAcceptUnitClass = kPacket_.m_cAcceptUnitClass;
	kAck.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;
	kAck.m_wstrAcceptUnitName = kPacket_.m_wstrAcceptUnitName;
	kAck.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	Query_DeleteItem( kPacket_.m_vecDeleted, kAck.m_kItemQuantityUpdate.m_vecDeleted );

	// 현재 시간 기준
	CTime tNow = CTime::GetCurrentTime();

	// 삭제가 모두 성공해야지만 인연 정보 기록
	if( kAck.m_kItemQuantityUpdate.m_vecDeleted.empty() == true )
	{		
		DO_QUERY( L"exec dbo.P_GEvent_Couple_INS", L"%d, %d", 
			% kAck.m_iRequestUnitUID
			% kAck.m_iAcceptUnitUID
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );

			m_kODBC.EndFetch();
		}
	}
	else
	{
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_01;
	}

	if( kAck.m_iOK == NetError::NET_OK )
	{
		kAck.m_tDate = tNow.GetTime();

		START_LOG( clog, L"가상 결혼 성공!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kAck.m_wstrRequestUnitName )
			<< BUILD_LOG( kAck.m_wstrAcceptUnitName )
			<< BUILD_LOG( static_cast<int>(kAck.m_kItemQuantityUpdate.m_vecDeleted.size()) )
			<< BUILD_LOG( static_cast<int>(kPacket_.m_vecDeleted.size()) )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_EVENT_MAKING_SUCCESS_ACK, kAck );
}

IMPL_ON_FUNC( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ )
{
	KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;

	Query_DeleteItem( kPacket_.m_vecDeleted, kAck.m_kItemQuantityUpdate.m_vecDeleted );

	if( kAck.m_kItemQuantityUpdate.m_vecDeleted.empty() == true )
	{
		kAck.m_iOK = NetError::NET_OK;
	}
	else
	{
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_01;
	}

	if( kAck.m_iOK == NetError::NET_OK )
	{
		START_LOG( clog, L"수락자 아이템 삭제 성공" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( static_cast<int>(kAck.m_kItemQuantityUpdate.m_vecDeleted.size()) )
			<< BUILD_LOG( static_cast<int>(kPacket_.m_vecDeleted.size()) )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_ACK, kAck );
}

IMPL_ON_FUNC( DBE_EVENT_DIVORCE_REQ )
{
	KDBE_EVENT_DIVORCE_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_iRelationTargetUserUid = kPacket_.m_iRelationTargetUserUid;
	kAck.m_vecUpdatedInventorySlot = kPacket_.m_vecUpdatedInventorySlot;	

	Query_DeleteItem( kPacket_.m_vecDeleted, kAck.m_kItemQuantityUpdate.m_vecDeleted );

	if( kAck.m_kItemQuantityUpdate.m_vecDeleted.empty() == true )
	{
		DO_QUERY( L"exec dbo.P_GEvent_Couple_DEL", L"%d, %d", 
			% kPacket_.m_iUnitUID
			% kPacket_.m_iRelationTargetUserUid
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );

			m_kODBC.EndFetch();
		}
	}
	else
	{
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_EVENT_04;
	}
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_EVENT_DIVORCE_ACK, kAck );
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_PERIOD_PET
IMPL_ON_FUNC( DBE_RELEASE_PET_REQ )
{
	KEGS_RELEASE_PET_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_iPetUID		= kPacket_.m_iPetUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_UPD_Delete", L"%d",
		% kPacket_.m_iPetUID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPet_Info_DEL", L"%d",
		% kPacket_.m_iPetUID
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() == true )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"펫 놓아주기 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RIDING_PET_11;	// 펫 놓아주기에 실패하였습니다.
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_RELEASE_PET_ACK, kPacket );
}
#endif SERV_PERIOD_PET

#ifdef SERV_RECRUIT_EVENT_BASE
IMPL_ON_FUNC( DBE_USE_RECRUIT_TICKET_REQ )
{
	KDBE_USE_RECRUIT_TICKET_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	CTime tRegDate;
	std::wstring wstrRegDate;
	CTime tEventDate;
	KncUtil::ConvertStringToCTime( STRING_RECRUIT_EVENT_DATE, tEventDate );

	DO_QUERY( L"exec dbo.gup_get_recommend_regdate", L"%d",
		% kPacket_.m_iUnitUID
		);
	if( m_kODBC.BeginFetch() )
	{
		kPacket.m_iOK = NetError::NET_OK;

		FETCH_DATA( wstrRegDate );
		m_kODBC.EndFetch();

		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
	}

	if( kPacket.m_iOK != NetError::NET_OK ||
		tRegDate < tEventDate )
	{
		START_LOG( cerr, L"추천받을 대상이 아닙니다." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_02;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_USE_RECRUIT_TICKET_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_REGISTER_RECRUITER_REQ )
{
	KDBE_REGISTER_RECRUITER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	CTime tRegDate;
	std::wstring wstrRegDate;
	CTime tEventDate;
	KncUtil::ConvertStringToCTime( STRING_RECRUIT_EVENT_DATE, tEventDate );

	// 추천인 날짜 검사
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	DO_QUERY( L"exec dbo.gup_get_recommend_nickname", L"N\'%s\'",
		% kPacket_.m_wstrNickname
		);
	if( m_kODBC.BeginFetch() )
	{
		kPacket.m_iOK = NetError::NET_OK;

		FETCH_DATA( wstrRegDate );
		m_kODBC.EndFetch();

		KncUtil::ConvertStringToCTime( wstrRegDate, tRegDate );
	}
	if( kPacket.m_iOK != NetError::NET_OK ||
		tRegDate > tEventDate )
	{
		START_LOG( cerr, L"추천 가능한 유저가 아닙니다." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_04;
		goto end_proc;
	}

	// 보상
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GRecommend_INS_New", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickname
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_recommend_new", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrNickname
		);
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		// 나중에 목록 불러오는 sp를 쓰므로 iRecruiterUnitUID 는 여기선 굳이 받아올 필요가 없는 값이긴 했다.
		UidType iRecruiterUnitUID;
		FETCH_DATA( kPacket.m_iOK
			>> iRecruiterUnitUID
			);
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_00;
			break;
		case -3:
			kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_03;
			break;
		case -4:
			kPacket.m_iOK = NetError::ERR_RECOMMEND_USER_01;
			break;
		default:
			kPacket.m_iOK = NetError::ERR_ODBC_01;
			break;
		}

		goto end_proc;
	}

	// 피추천인의 추천인 목록에 추가를 해 주기 위한 부분
	DO_QUERY( L"exec dbo.gup_get_recommend_newuid", L"%d", % kPacket_.m_iUnitUID );
	while( m_kODBC.Fetch() )
	{
		FETCH_DATA( kPacket.m_kRecruiterUnitInfo.m_iUnitUID
			>> kPacket.m_kRecruiterUnitInfo.m_ucLevel
			>> kPacket.m_kRecruiterUnitInfo.m_cUnitClass
			>> kPacket.m_kRecruiterUnitInfo.m_wstrNickName );
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_REGISTER_RECRUITER_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_GET_RECRUIT_RECRUITER_LIST_REQ )
{
	KEGS_GET_RECRUIT_RECRUITER_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.gup_get_recommend_newuid", L"%d", % kPacket_.m_iUnitUID );
	while( m_kODBC.Fetch() )
	{
		KRecommendUserInfo kInfo;
		kInfo.m_wstrRecommendDate = L"";

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_ucLevel
			>> kInfo.m_cUnitClass
			>> kInfo.m_wstrNickName );

		kPacket.m_vecRecruiterUnitInfo.push_back( kInfo );
	}

	DO_QUERY( L"exec dbo.gup_get_recommend_olduid", L"%d", % kPacket_.m_iUnitUID );
	while( m_kODBC.Fetch() )
	{
		KRecommendUserInfo kInfo;
		kInfo.m_wstrRecommendDate = L"";

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_ucLevel
			>> kInfo.m_cUnitClass
			>> kInfo.m_wstrNickName );

		kPacket.m_vecRecruitUnitInfo.push_back( kInfo );
	}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_RECRUIT_RECRUITER_LIST_ACK, kPacket );
}


#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING

IMPL_ON_FUNC_NOPARAM( DBE_GET_EVENT_INFO_REQ )
{
	KEGS_GET_EVENT_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	// 던전
	DO_QUERY( L"exec dbo.P_GEvent_UnitClass_SEL", L"%d", % 74710 );
	while( m_kODBC.Fetch() )
	{
		int iUnitType = 0;
		int iTempCount = 0;

		FETCH_DATA( iUnitType
			>> iTempCount );

		if( iUnitType == 2 )
			kPacket.m_kEventCharacterRaking.m_iDungeonCharacter1 = iTempCount;
		else if( iUnitType == 3 )
			kPacket.m_kEventCharacterRaking.m_iDungeonCharacter2 = iTempCount;
		else if( iUnitType == 5 )
			kPacket.m_kEventCharacterRaking.m_iDungeonCharacter3 = iTempCount;
		else if( iUnitType == 7 )
			kPacket.m_kEventCharacterRaking.m_iDungeonCharacter4 = iTempCount;
	}


	// 대전
	DO_QUERY( L"exec dbo.P_GEvent_UnitClass_SEL", L"%d", % 74720 );
	while( m_kODBC.Fetch() )
	{
		int iUnitType = 0;
		int iTempCount = 0;

		FETCH_DATA( iUnitType
			>> iTempCount );

		if( iUnitType == 2 )
			kPacket.m_kEventCharacterRaking.m_iPVPCharacter1 = iTempCount;
		else if( iUnitType == 3 )
			kPacket.m_kEventCharacterRaking.m_iPVPCharacter2 = iTempCount;
		else if( iUnitType == 5 )
			kPacket.m_kEventCharacterRaking.m_iPVPCharacter3 = iTempCount;
		else if( iUnitType == 7 )
			kPacket.m_kEventCharacterRaking.m_iPVPCharacter4 = iTempCount;
	}

	// 필드
	DO_QUERY( L"exec dbo.P_GEvent_UnitClass_SEL", L"%d", % 74730 );
	while( m_kODBC.Fetch() )
	{
		int iUnitType = 0;
		int iTempCount = 0;

		FETCH_DATA( iUnitType
			>> iTempCount );

		if( iUnitType == 2 )
			kPacket.m_kEventCharacterRaking.m_iFieldCharacter1 = iTempCount;
		else if( iUnitType == 3 )
			kPacket.m_kEventCharacterRaking.m_iFieldCharacter2 = iTempCount;
		else if( iUnitType == 5 )
			kPacket.m_kEventCharacterRaking.m_iFieldCharacter3 = iTempCount;
		else if( iUnitType == 7 )
			kPacket.m_kEventCharacterRaking.m_iFieldCharacter4 = iTempCount;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_EVENT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_SET_EVENT_INFO_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_GEvent_UnitClass_SET", L"%d, %d",
		% kPacket_.m_iUnitType
		% kPacket_.m_iQuestID
		);

	if( m_kODBC.BeginFetch() == true )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 퀘스트 랭킹 카운트 DB 쓰기 실패" )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< BUILD_LOG( kPacket_.m_iUnitType )
			<< END_LOG;
	}

end_proc:
	return;
}
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_NEW_YEAR_EVENT_2014
IMPL_ON_FUNC( DBE_2013_EVENT_MISSION_COMPLETE_REQ )
{
	KDBE_2013_EVENT_MISSION_COMPLETE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_ucLevel = kPacket_.m_ucLevel;

	DO_QUERY( L"exec dbo.P_GEventEndYear_UPD", L"%d, %d", % kPacket_.m_iUnitUID % kPacket_.m_ucLevel );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
		kPacket.m_iOK = NetError::ERR_QUEST_09;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_2013_EVENT_MISSION_COMPLETE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_2014_EVENT_MISSION_COMPLETE_REQ )
{
	KEGS_2014_EVENT_MISSION_COMPLETE_ACK kPacket;

	DO_QUERY( L"exec dbo.P_GEventNewYear_SET", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iNewYearMissionStepID );
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
		kPacket.m_iOK = NetError::ERR_QUEST_09;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_2014_EVENT_MISSION_COMPLETE_ACK, kPacket );
}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_USE_GM_TOOL_INFO
IMPL_ON_FUNC( DBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT )
{
	std::map< int, KItemName >			PrevItemTempletNameMap;
	std::map< int, KItemName >			UpdateItemTempletNameMap;
	std::map< int, KItemName >			InsertItemTempletNameMap;
	std::map< int, KItemName >::iterator mitPrevItemName;
	std::map< int, KItemName >::iterator mitCurrentItemName;

	DO_QUERY_NO_ARG( L"exec dbo.P_GItem_Def_GET" );
	while( m_kODBC.Fetch() )
	{
		int iItemID = 0;
		KItemName kItemName;
		
		FETCH_DATA( iItemID
			>> kItemName.m_wstrKRItemName
			>> kItemName.m_wstrTransItemName );

		mitPrevItemName = PrevItemTempletNameMap.find( iItemID );

		if( mitPrevItemName == PrevItemTempletNameMap.end() )
		{
			PrevItemTempletNameMap.insert( std::make_pair( iItemID, kItemName ) );			
		}
		else
		{
			START_LOG( cerr, L"DB 아이템 리스트가 중복되는 게 있다고???" )
				<< BUILD_LOG( iItemID )
				<< END_LOG;
			continue;
		}
	}

	if( kPacket_.m_CurrentItemTempletNameMap.size() != PrevItemTempletNameMap.size() )
	{
		for( mitCurrentItemName = kPacket_.m_CurrentItemTempletNameMap.begin(); mitCurrentItemName != kPacket_.m_CurrentItemTempletNameMap.end(); ++mitCurrentItemName )
		{
			mitPrevItemName = PrevItemTempletNameMap.find( mitCurrentItemName->first );
			if( mitPrevItemName != PrevItemTempletNameMap.end() )
			{
				if(  mitCurrentItemName->second.m_wstrKRItemName.compare( mitPrevItemName->second.m_wstrKRItemName ) != 0
					|| mitCurrentItemName->second.m_wstrTransItemName.compare( mitPrevItemName->second.m_wstrTransItemName ) != 0 )
				{

					boost::replace_all( mitCurrentItemName->second.m_wstrKRItemName, L"'",L"''");
					boost::replace_all( mitCurrentItemName->second.m_wstrTransItemName, L"'",L"''");

					UpdateItemTempletNameMap.insert( std::make_pair(  mitCurrentItemName->first, mitCurrentItemName->second ) );
				}
			}
			else
			{
				boost::replace_all( mitCurrentItemName->second.m_wstrKRItemName, L"'",L"''");
				boost::replace_all( mitCurrentItemName->second.m_wstrTransItemName, L"'",L"''");

				InsertItemTempletNameMap.insert( std::make_pair(  mitCurrentItemName->first, mitCurrentItemName->second ) );
			}
		}

		for( mitCurrentItemName = UpdateItemTempletNameMap.begin(); mitCurrentItemName != UpdateItemTempletNameMap.end(); ++mitCurrentItemName )
		{
			DO_QUERY( L"exec dbo.P_GItem_Def_UPD", L"%d, N\'%s\', N\'%s\'", % mitCurrentItemName->first % mitCurrentItemName->second.m_wstrKRItemName % mitCurrentItemName->second.m_wstrTransItemName );
		}
		for( mitCurrentItemName = InsertItemTempletNameMap.begin(); mitCurrentItemName != InsertItemTempletNameMap.end(); ++mitCurrentItemName )
		{
			DO_QUERY( L"exec dbo.P_GItem_Def_INS", L"%d, N\'%s\', N\'%s\'", % mitCurrentItemName->first % mitCurrentItemName->second.m_wstrKRItemName % mitCurrentItemName->second.m_wstrTransItemName );
		}
	}

end_proc:
	return;
}

#endif //SERV_USE_GM_TOOL_INFO

#ifdef SERV_GLOBAL_MISSION_MANAGER
IMPL_ON_FUNC( DBE_REGIST_GLOBAL_MISSION_CLEAR_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	DO_QUERY( L"exec dbo.P_GEvent_GlobalMission_INS", L"N\'%s\', %d",
		% kPacket_.m_wstrGlobalMissionDay 
		% kPacket_.m_iUnitUID
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	return;
}
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef SERV_EVENT_CHECK_POWER
IMPL_ON_FUNC( DBE_START_CHECK_POWER_REQ )
{
	std::wstring wstrCheckPowerTime = CTime( kPacket_.m_iCheckPowerTime ).Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	START_LOG( cwarn, L"패킷 오는지 검사")
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_ucCheckPowerCount )
		<< BUILD_LOG( kPacket_.m_iCheckPowerTime )
		<< BUILD_LOG( kPacket_.m_ucCheckPowerScore )
		<< BUILD_LOG( wstrCheckPowerTime )
		<< END_LOG;

	DO_QUERY( L"exec dbo.P_GEventElesisClassChange_SET", L"%d, %d, %d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_ucCheckPowerScore
		% kPacket_.m_ucCheckPowerCount
		% wstrCheckPowerTime
		);

	KDBE_START_CHECK_POWER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_START_CHECK_POWER_ACK, kPacket );
}
_IMPL_ON_FUNC( DBE_UPDATE_CHECK_POWER_REQ, KDBE_START_CHECK_POWER_REQ )
{
	std::wstring wstrCheckPowerTime = CTime( kPacket_.m_iCheckPowerTime ).Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	START_LOG( cwarn, L"패킷 오는지 검사")
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_ucCheckPowerCount )
		<< BUILD_LOG( kPacket_.m_iCheckPowerTime )
		<< BUILD_LOG( kPacket_.m_ucCheckPowerScore )
		<< BUILD_LOG( wstrCheckPowerTime )
		<< END_LOG;

	DO_QUERY( L"exec dbo.P_GEventElesisClassChange_SET", L"%d, %d, %d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_ucCheckPowerScore
		% kPacket_.m_ucCheckPowerCount
		% wstrCheckPowerTime
		);

	KDBE_UPDATE_CHECK_POWER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_UPDATE_CHECK_POWER_ACK, kPacket );
}
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
IMPL_ON_FUNC( DBE_ITEM_EXTRACT_REQ )
{
	KDBE_ITEM_EXTRACT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;	
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;
	kPacket.m_mapResultItem	= kPacket_.m_mapResultItem;

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	if( Query_InsertItemList( SEnum::GIR_ITEM_EXTRACT, kPacket_.m_iUnitUID, kPacket_.m_vecItemInfo, kPacket.m_mapItemInfo, false ) == false )
	{
		START_LOG( cerr, L"추출 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_EXTRACT_06;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}
	
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"추출 아이템 업데이트 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_ITEM_EXTRACT_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_USE_FINALITY_SKILL_REQ )
{
	KDBE_USE_FINALITY_SKILL_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;	
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;	

	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	//{{ 2008. 10. 23  최육사	증분값 업데이트 실패
	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}
	//}}

	kPacket.m_iOK = NetError::NET_OK;

	SendToUser( LAST_SENDER_UID, DBE_USE_FINALITY_SKILL_ACK, kPacket );
}
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_GOOD_ELSWORD
IMPL_ON_FUNC( DBE_EXPAND_BANK_INVENTORY_REQ )
{
    KDBE_EXPAND_BANK_INVENTORY_ACK kPacket;
    kPacket.m_usEventID = kPacket_.m_usEventID;
    kPacket.m_iED = kPacket_.m_iED;

    std::map< int, int >::const_iterator mit;
    for( mit = kPacket_.m_mapExpandedSlot.begin(); mit != kPacket_.m_mapExpandedSlot.end(); ++mit )
    {
        //{{ 2012. 12. 26	박세훈	인벤토리 개편 테스트	- 허상형 ( Merged by 박세훈 )
#ifdef SERV_REFORM_INVENTORY_TEST
        if( mit->first == CXSLInventory::ST_BANK )
        {
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GItemInventorySize_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#else //SERV_ALL_RENEWAL_SP
            DO_QUERY( L"exec dbo.gup_insert_inventory_size", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#endif //SERV_ALL_RENEWAL_SP
        }
        else
        {
#ifdef SERV_ALL_RENEWAL_SP
            DO_QUERY( L"exec dbo.P_GItemInventorySize_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#else //SERV_ALL_RENEWAL_SP
            DO_QUERY( L"exec dbo.P_GItemInventorySize_INS", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#endif //SERV_ALL_RENEWAL_SP
        }
#else
#ifdef SERV_ALL_RENEWAL_SP
		 DO_QUERY( L"exec dbo.P_GItemInventorySize_MER", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#else //SERV_ALL_RENEWAL_SP
        DO_QUERY( L"exec dbo.gup_insert_inventory_size", L"%d, %d, %d", % kPacket_.m_iUnitUID % mit->first % mit->second );
#endif //SERV_ALL_RENEWAL_SP
#endif SERV_REFORM_INVENTORY_TEST
        //}}DBE_EXPAND_INVENTORY_SLOT_REQ

        int iOK = NetError::ERR_ODBC_01;

        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( iOK );
            m_kODBC.EndFetch();
        }

        if( iOK == NetError::NET_OK )
        {
            kPacket.m_mapExpandedSlot.insert( std::make_pair( mit->first, mit->second ) );
            kPacket.m_iOK = iOK;
        }
        else
        {
            START_LOG( cerr, L"ED를 통한 은행 확장 실패." )
                << BUILD_LOG( iOK )
                << BUILD_LOG( kPacket_.m_iUnitUID )
                << BUILD_LOG( mit->first )
                << BUILD_LOG( mit->second )
                << END_LOG;
        }
    }

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_EXPAND_BANK_INVENTORY_ACK, kPacket );
}
#endif // SERV_GOOD_ELSWORD

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
IMPL_ON_FUNC( DBE_SOCKET_EXPAND_ITEM_REQ )
{
	KDBE_SOCKET_EXPAND_ITEM_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	const byte byteArraySize = 5;
	int iArrSocketInfo[byteArraySize];
	memset( iArrSocketInfo, 0, sizeof(int) * byteArraySize );

	for( int i = 0, size = min( byteArraySize, static_cast<int>( kPacket_.m_vecItemSocket.size() ) ); i < size; ++i )
	{
		iArrSocketInfo[i] = kPacket_.m_vecItemSocket[i];
	}

	// 아이템 소켓 정보에 확장 소켓 수량을 1회 증가 시키자
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_MER", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% iArrSocketInfo[0]					// @iSoket1 smallint
		% iArrSocketInfo[1]					// @iSoket2 smallint
		% iArrSocketInfo[2]					// @iSoket3 smallint
		% iArrSocketInfo[3]					// @iSoket4 smallint
		% iArrSocketInfo[4]					// @iSoket5 smallint
		% kPacket_.m_byteExpandedSocketNum
			);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GItemSocket_SET", L"%d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iItemUID
		% iArrSocketInfo[0]					// @iSoket1 smallint
		% iArrSocketInfo[1]					// @iSoket2 smallint
		% iArrSocketInfo[2]					// @iSoket3 smallint
		% iArrSocketInfo[3]					// @iSoket4 smallint
		% iArrSocketInfo[4]					// @iSoket5 smallint
		% kPacket_.m_byteExpandedSocketNum
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		goto end_proc;
	}

	// Update SP가 성공했다면, 수량 변화나 삭제되어야 할 아이템 정보도 Update 하자
	bool bUpdateFailed = false;
	LIF( Query_UpdateItemQuantity( kPacket_.m_iUnitUID, kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange, kPacket.m_kItemQuantityUpdate.m_mapQuantityChange, bUpdateFailed ) );
	Query_DeleteItem( kPacket_.m_kItemQuantityUpdate.m_vecDeleted, kPacket.m_kItemQuantityUpdate.m_vecDeleted );

	if( bUpdateFailed )
	{
		START_LOG( cout, L"증분값 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID );

		std::map< UidType, int >::const_iterator mitQC = kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.begin();
		for( ; mitQC != kPacket_.m_kItemQuantityUpdate.m_mapQuantityChange.end(); ++mitQC )
		{
			START_LOG( cout, L"아이템 정보" )
				<< BUILD_LOG( mitQC->first )
				<< BUILD_LOG( mitQC->second );
		}
	}

	kPacket.m_iItemUID					= kPacket_.m_iItemUID;
	kPacket.m_byteExpandedSocketNum		= kPacket_.m_byteExpandedSocketNum;
	kPacket.m_vecUpdatedInventorySlot	= kPacket_.m_vecUpdatedInventorySlot;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_SOCKET_EXPAND_ITEM_ACK, kPacket );
}
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_ENTRY_POINT
_IMPL_ON_FUNC( DBE_CHARACTER_LIST_REQ, std::wstring )
{
    KEGS_CHARACTER_LIST_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;

    std::vector< KUnitInfo >::iterator vit;

    UidType nUserUID = FIRST_SENDER_UID; // FIRST_SENDER_UID, LAST_SENDER_UID 의미 파악이 어렵다. nUserUID 로 alias 한 이유

    int nServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
    if ( nUserUID != -1 ) 
    {
        int nUnitSlot = m_kSP.GetUnitSlotNum( nUserUID, kPacket_ ); // 솔레스 캐릭터 최대 슬롯 수
        // 현재 서버군을 아래 map 의 키로 넣어야 한다. 
        kPacket.m_mapServerGroupUnitSlot.insert( std::make_pair( nServerGroupID, nUnitSlot ) );

        std::vector<KUnitInfo> vecUnitInfo;
        m_kSP.GetUnitInfo( nUserUID, vecUnitInfo );
        m_kSP.GetEquipItemList( vecUnitInfo );
        m_kSP.GetLastPosition( vecUnitInfo );
        kPacket.m_mapServerGroupUnitInfo.insert( std::map< int, std::vector<KUnitInfo> >::value_type( nServerGroupID, vecUnitInfo ) );
    }
    else 
    {
        START_LOG( cerr, L"캐릭터 리스트 요청시 UserUID 이상 " )
            << BUILD_LOG( nUserUID )
            << END_LOG;
    }

    kPacket.m_iOK = NetError::NET_OK;
    kPacket.m_strUserID = kPacket_;

    SendToUser( LAST_SENDER_UID, EGS_CHARACTER_LIST_1ST_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, KEGS_ENTRY_POINT_CHECK_NICK_NAME_REQ )
{
    KEGS_CHECK_NICK_NAME_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_wstrNickName = kPacket_.m_wstrNickName;

#ifdef SERV_ALL_RENEWAL_SP
	int iNickKeepDay = 14;	// 기본 국내
#if defined( SERV_COUNTRY_TWHK )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_JP )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_EU )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_US )
	iNickKeepDay = 7;
#elif defined( SERV_COUNTRY_CN )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_TH )
	iNickKeepDay = 0;
#elif defined( SERV_COUNTRY_ID )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_BR )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_PH )
	iNickKeepDay = 14;
#elif defined( SERV_COUNTRY_IN )
	iNickKeepDay = 14;
#endif //SERV_COUNTRY_XX
#endif //SERV_ALL_RENEWAL_SP

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GDeletedNickNameHistory_SEL", L"N\'%s\', %d", % kPacket_.m_wstrNickName % iNickKeepDay );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_check_nickname", L"N\'%s\'", % kPacket_.m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK );
        m_kODBC.EndFetch();
    }

    switch ( kPacket.m_iOK )
    {
    case -1:
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_02;
        break;
    case -2: 
        kPacket.m_iOK = NetError::ERR_RESTORE_NICK_NAME_03;
        break;
    default:
        ;
    }

end_proc:

    SendToUser( LAST_SENDER_UID, EGS_ENTRY_POINT_CHECK_NICK_NAME_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ )
{
    KEGS_GET_CREATE_UNIT_TODAY_COUNT_ACK kPacket;
    kPacket.m_iUserUID = kPacket_.m_iUserUID;

    int nServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();
    int nCreateUnitCountToday = 0;

    DO_QUERY( L"exec dbo.P_GUnit_CreateCNT_GET", L"%d", % kPacket_.m_iUserUID );
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( nCreateUnitCountToday );
        m_kODBC.EndFetch();
    }

    kPacket.m_mapCreateCharCountToday.insert( std::make_pair( nServerGroupID, nCreateUnitCountToday ) );

end_proc:

    SendToUser( LAST_SENDER_UID, EGS_GET_CREATE_UNIT_TODAY_COUNT_1ST_ACK, kPacket );
}
#endif // SERV_ENTRY_POINT


#ifdef SERV_SKILL_PAGE_SYSTEM
IMPL_ON_FUNC( DBE_EXPAND_SKILL_PAGE_REQ )
{
	KDBE_EXPAND_SKILL_PAGE_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_usEventID		= kPacket_.m_usEventID;
	
	
	BOOST_TEST_FOREACH( const int&, iSkillPagesNumberToBeAdded, kPacket_.m_vecSkillPageNumberToBeAdded )
	{
		{
			int iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_Expand_MER", L"%d, %d",
				% kPacket_.m_iUnitUID
				% 3 );	/// 최대록 확장 가능한 페이지 수
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GSkill_Expand_INS", L"%d, %d",
				% kPacket_.m_iUnitUID
				% 3 );	/// 최대로 확장 가능한 페이지 수
#endif //SERV_ALL_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"스킬 페이지 확장 실패 (P_GSkill_Expand_INS)" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( iSkillPagesNumberToBeAdded )
					<< BUILD_LOG( iOK )
					<< END_LOG;

				kPacket.m_iOK = NetError::ERR_SKILL_24;
				goto end_proc;	
			}
		}


		// 스킬 획득 정보를 갱신
		const int iLevel = 1;
		const int iSpendCSPoint = 0;
		for ( int i = 0; i < THE_NUMBER_OF_DEFAULT_SKILLS; i++ )
		{
			if ( kPacket_.m_iDefaultSkill[i] > 0 )
			{
				int iOK = NetError::NET_OK;

#ifdef SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GSkill_New_MER", L"%d, %d, %d, %d, %d",
					% kPacket_.m_iUnitUID 
					% kPacket_.m_iDefaultSkill[i]
					% iLevel
					% iSpendCSPoint
					% iSkillPagesNumberToBeAdded );
#else //SERV_ALL_RENEWAL_SP
				DO_QUERY( L"exec dbo.P_GSkill_New_UPD", L"%d, %d, %d, %d, %d",
					% kPacket_.m_iUnitUID 
					% kPacket_.m_iDefaultSkill[i]
					% iLevel
					% iSpendCSPoint
					% iSkillPagesNumberToBeAdded );
#endif //SERV_ALL_RENEWAL_SP

				if( m_kODBC.BeginFetch() )
				{
					FETCH_DATA( iOK );
					m_kODBC.EndFetch();
				}

				if( iOK != NetError::NET_OK )
				{
					START_LOG( cerr, L"추가된 페이지에 기본을 스킬 등록 하지 못 했습니다." )
						<< BUILD_LOG( LAST_SENDER_UID )
						<< BUILD_LOG( kPacket_.m_iUnitUID )
						<< BUILD_LOG( kPacket_.m_iDefaultSkill[i] )
						<< BUILD_LOG( iLevel )
						<< BUILD_LOG( iSpendCSPoint )
						<< BUILD_LOG( iSkillPagesNumberToBeAdded )
						<< BUILD_LOG( iOK )
						<< END_LOG;

					kPacket.m_iOK = NetError::ERR_SKILL_PAGE_05;

					goto end_proc;
				}
			}
		}
	}		

	kPacket.m_iOK						= NetError::NET_OK;
	kPacket.m_cUnitClass				= kPacket_.m_cUnitClass;
	kPacket.m_iCSPoint					= kPacket_.m_iCSPoint;
	kPacket.m_iSPoint					= kPacket_.m_iSPoint;
	kPacket.m_iUnitUID					= kPacket_.m_iUnitUID;

	const int& iSkillPagesNumberToBeAddedRef = kPacket_.m_vecSkillPageNumberToBeAdded.back();
	kPacket.m_iSkillPageNumberToBeAdded	= iSkillPagesNumberToBeAddedRef;

	for ( int i = 0; i < THE_NUMBER_OF_DEFAULT_SKILLS; i++ )
		kPacket.m_iDefaultSkill[i] = kPacket_.m_iDefaultSkill[i];
		

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_EXPAND_SKILL_PAGE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ )
{
	KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;

	int iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Expand_UPD_Active", L"%d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSkillPagesNumberToBeActive );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GSkill_Expand_UPD", L"%d, %d",
		% kPacket_.m_iUnitUID
		% kPacket_.m_iSkillPagesNumberToBeActive );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	switch ( iOK )
	{
	case NetError::NET_OK:
		{
			kAck.m_iOK = iOK;
		} break;

	case -1:		// 확장 자체를 한 적이 없음
		{
			START_LOG( cerr, L"확장 자체를 한 적이 없음 (P_GSkill_Expand_UPD_Active)" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iSkillPagesNumberToBeActive )
				<< BUILD_LOG( iOK )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_SKILL_PAGE_06;
			goto end_proc;	
		} break;

	case -2:		// Update 실패
		{
			START_LOG( cerr, L"Update 실패 (P_GSkill_Expand_UPD_Active)" )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iSkillPagesNumberToBeActive )
				<< BUILD_LOG( iOK )
				<< END_LOG;

			kAck.m_iOK = NetError::ERR_SKILL_PAGE_07;
			goto end_proc;	
		} break;
	}

	kAck.m_iSkillPagesNumberToBeActive	= kPacket_.m_iSkillPagesNumberToBeActive;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK, kAck );
}
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_EVENT_PET_INVENTORY
IMPL_ON_FUNC( DBE_EVENT_PET_EVENT_FOOD_EAT_REQ )
{
	int iOK = 0;
	DO_QUERY( L"exec dbo.P_GPetEvent_INT", L"%d, %d, N\'%s\', %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_iPetUID
		% kPacket_.m_iPetID					// SERV_PETID_DATA_TYPE_CHANGE
		% kPacket_.m_wstrPetName
		% kPacket_.m_bEventFoodEat
		);
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
end_proc:
	KDBE_EVENT_PET_EVENT_FOOD_EAT_ACK kPacket;
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"이벤트용 펫 DB정보 등록 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPetID )
			<< BUILD_LOG( kPacket_.m_wstrPetName )
			<< BUILD_LOG( kPacket_.m_iPetUID )
			<< BUILD_LOG( kPacket_.m_bEventFoodEat )
			<< END_LOG;
		kPacket.m_iOK = NetError::ERR_PET_11;
		SendToUser( LAST_SENDER_UID, DBE_EVENT_PET_EVENT_FOOD_EAT_ACK, kPacket );
	}
	else
	{
		kPacket.m_iOK = iOK;
		kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
		kPacket.m_iPetID = kPacket_.m_iPetID;
		kPacket.m_wstrPetName = kPacket_.m_wstrPetName;
		kPacket.m_iPetUID = kPacket_.m_iPetUID;
		kPacket.m_bEventFoodEat = kPacket_.m_bEventFoodEat;
		kPacket.m_vecInventorySlotInfo = kPacket_.m_vecInventorySlotInfo;
		SendToUser( LAST_SENDER_UID, DBE_EVENT_PET_EVENT_FOOD_EAT_ACK, kPacket );
	}
}
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
IMPL_ON_FUNC( DBE_EVENT_CHUNG_GIVE_ITEM_REQ )
{
	int iOK = 0;
	DO_QUERY( L"exec dbo.P_GEventChung_SET", L"%d, N\'%s\', N\'%s\',N\'%s\'", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrGiveItemTime_One
		% kPacket_.m_wstrGiveItemTime_Two
		% kPacket_.m_wstrGiveItemTime_Tree				// SERV_PETID_DATA_TYPE_CHANGE
		);
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	KDBE_EVENT_CHUNG_GIVE_ITEM_ACK	kPacket;
	kPacket.m_iOK = iOK;
	kPacket.m_bTwoGiveItem = kPacket_.m_bTwoGiveItem;
	kPacket.m_iChoice = kPacket_.m_iChoice;
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"청 아이템 등록 시간 저장 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrGiveItemTime_One )
			<< BUILD_LOG( kPacket_.m_wstrGiveItemTime_Two )
			<< BUILD_LOG( kPacket_.m_wstrGiveItemTime_Tree )
			<< END_LOG;
	}
	else
	{
		START_LOG( clog, L"청 아이템 등록 시간 저장 성공" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrGiveItemTime_One )
			<< BUILD_LOG( kPacket_.m_wstrGiveItemTime_Two )
			<< BUILD_LOG( kPacket_.m_wstrGiveItemTime_Tree )
			<< END_LOG;
	}
	SendToUser( LAST_SENDER_UID, DBE_EVENT_CHUNG_GIVE_ITEM_ACK, kPacket );
}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
IMPL_ON_FUNC( DBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
	DO_QUERY( L"exec dbo.P_GEventCobo_SET", L"%d, N\'%s\', %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrButtonClickTime_One
		% kPacket_.m_bItemGive
		% kPacket_.m_iDungeonClearCount		
		% kPacket_.m_iFieldMonsterKillCount	
		);
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
end_proc:
	KDBE_EVENT_COBO_DUNGEON_AND_FIELD_ACK kPacket;
	KEGS_EVENT_COBO_DUNGEON_FIELD_ACK   kStartPacket;
	if( kPacket_.m_bStartButton == false ) //이건 보상 줄때다
	{
		kPacket.m_iOk = iOK;
		kPacket.m_bWeekEndItem = kPacket_.m_WeekEndItem;
		kPacket.m_NowDay = kPacket_.m_Nowday;
		kPacket.m_iDungeonClearCount = kPacket_.m_iDungeonClearCount;
		kPacket.m_iFieldMonsterKillCount = kPacket_.m_iFieldMonsterKillCount;
		kPacket.m_wstrButtonClickTime_One = kPacket_.m_wstrButtonClickTime_One;
	}
	else
	{
		kStartPacket.m_bStartUI = false; //버튼 눌렀으니까 버튼 비활성화
		kStartPacket.m_DungeonCount = kPacket_.m_iDungeonClearCount; //처음 시작이니까 0이다
		kStartPacket.m_FieldMonsterKillCount = kPacket_.m_iFieldMonsterKillCount; //처음 시작이니까 0이다
		kStartPacket.m_iOK = iOK; //정보 저장이 잘됫는지 확인
		kStartPacket.m_wstrPushTime = kPacket_.m_wstrButtonClickTime_One; //시간에 따라 열어줘야 하는 UI가 다르니까 누른 시간 담아오자
	}
	///이벤트 버튼 눌러서 시작이면
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"코보사 정보 저장 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrButtonClickTime_One )
			<< BUILD_LOG( kPacket_.m_bItemGive )
			<< BUILD_LOG( kPacket_.m_iDungeonClearCount )
			<< BUILD_LOG( kPacket_.m_iFieldMonsterKillCount )
			<< END_LOG;
	}
	if( kPacket_.m_bStartButton == false)
	{
		SendToUser( LAST_SENDER_UID, DBE_EVENT_COBO_DUNGEON_AND_FIELD_ACK, kPacket );
	}
	else
	{
		SendToUser( LAST_SENDER_UID, EGS_EVENT_COBO_DUNGEON_FIELD_ACK, kStartPacket );
	}
}
IMPL_ON_FUNC( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT )
{
	int iOK = NetError::ERR_ODBC_01;;
	DO_QUERY( L"exec dbo.P_GEventCobo_SET", L"%d, N\'%s\', %d, %d, %d", 
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrButtonClickTime_One
		% kPacket_.m_bItemGive
		% kPacket_.m_iDungeonClearCount		
		% kPacket_.m_iFieldMonsterKillCount	
		);
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"던전 카운트 몬스터킬 카운트 저장 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrButtonClickTime_One )
			<< BUILD_LOG( kPacket_.m_bItemGive )
			<< BUILD_LOG( kPacket_.m_iDungeonClearCount )
			<< BUILD_LOG( kPacket_.m_iFieldMonsterKillCount )
			<< END_LOG;
	}
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
IMPL_ON_FUNC( DBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_REQ )
{
	//정보를 저장 하자
	int iOK = NetError::ERR_ODBC_01;
	int iDungeonID = (int)SEnum::DI_EVENT_VALENTINE_DUNGEON_INT;
	DO_QUERY( L"exec dbo.P_GEventDungeonData_SET", L"%d, %d, %d, %d",
		% kPacket_.m_iUnitUID
		% iDungeonID
		% 0
		% kPacket_.m_iValenTineItemCount
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	KDBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_ACK	kPacket;
	kPacket.m_iOk = NetError::NET_OK;
	kPacket.m_iOk = iOK;
	kPacket.m_iValenTineItemCount = kPacket_.m_iValenTineItemCount;
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"발렌타인 아이템 보상 횟수 기록 실패" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iValenTineItemCount )
			<< END_LOG;
	}
	SendToUser( LAST_SENDER_UID, DBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_ACK, kPacket );
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM