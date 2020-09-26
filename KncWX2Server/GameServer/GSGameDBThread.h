#pragma once

#include "SubThread.h"
#include "odbc/Odbc.h"
//{{ 2009. 8. 13  최육사	sp profiler
#include "odbc/DBUtil.h"
//}}
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

//{{ 2009. 10. 6  최육사	길드
#include "Enum/Enum.h"
//}}
#include "..\Common\ODBC\StoredProcedure.h"

class KGSGameDBThread : public KSubThread
{
    DeclareDBThreadWithSP( KGSGameDBThread );
	DeclareSPProfiler;

public:
    KGSGameDBThread();
    virtual ~KGSGameDBThread(void);

protected:
    // derived from KSubThread
    virtual inline void ProcessEvent( const KEventPtr& spEvent );

protected:
    KStoredProcedure m_kSP;

    // DB query function
	//{{ 2009. 5. 28  최육사	채널이동
	//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
	bool Query_SelectUnit( IN const UidType iUserUID, IN const KEGS_SELECT_UNIT_REQ& kReq, OUT KDBE_SELECT_UNIT_ACK& kAck );
#else
	bool Query_SelectUnit( IN const UidType iUserUID, IN const KEGS_SELECT_UNIT_REQ& kReq, OUT KEGS_SELECT_UNIT_ACK& kAck );
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	//}}
	//}}
	//{{ 2010. 9. 29	최육사	아이템 정보 리팩토링
#ifdef SERV_GET_INVENTORY_REFAC
	bool Query_GetInventory( IN const UidType iUnitUID, 
							 OUT std::map< int, int >& mapInventorySlotSizeResult,
							 OUT std::map< UidType, KInventoryItemInfo >& mapItemResult,
							 OUT std::map< UidType, std::map< UidType, KInventoryItemInfo > >& mapPetItemResult
							 );
#endif SERV_GET_INVENTORY_REFAC
	//}}
	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	bool Query_InsertItemList( IN const SEnum::GET_ITEM_REASON eGetItemReason, IN const UidType iUnitUID, IN const std::vector< KItemInfo >& vecItemInfo, OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, IN const bool bUnSeal = true );
//#else
//	bool Query_InsertItemList( IN const UidType iUnitUID, IN const std::vector< KItemInfo >& vecItemInfo, OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo, IN const bool bUnSeal = true );
#endif SERV_GET_ITEM_REASON
	//}}
	void Query_UpdateSealItem( IN const std::set< int >& setSealItem, IN OUT std::map< UidType, KItemInfo >& mapInsertedItemInfo );
    void Query_UpdateDungeonClear( UidType iUnitUID, std::map< int, KDungeonClearInfo >& mapDungeonClearInfo, std::vector< int >& vecFailed );
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
	void Query_UpdateDungeonPlay( IN UidType iUnitUID, IN std::map< int, KDungeonPlayInfo >& mapDungeonPlayInfo );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
    void Query_UpdateTCClear( UidType iUnitUID, std::map< int, KTCClearInfo >& mapTCClearInfo, std::vector< int >& vecFailed );
    
    bool Query_UpdateItemQuantity( IN UidType iUnitUID, IN std::map< UidType, int >& mapUpdated, OUT std::map< UidType, int >& mapFailed, OUT bool& bUpdateFailed );
	bool Query_UpdateItemEndurance( IN UidType iUnitUID, IN std::map< UidType, int >& mapUpdated, OUT std::map< UidType, int >& mapFailed );
	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	void Query_UpdateItemPosition( IN UidType iUnitUID, IN std::map< UidType, KItemPosition >& mapUpdated, OUT std::map< UidType, KItemPosition >& mapFailed );
#else
	void Query_UpdateItemPosition( IN UidType iUnitUID, IN std::map< UidType, std::pair< int, int > >& mapUpdated, OUT std::map< UidType, std::pair< int, int > >& mapFailed );
#endif SERV_PET_SYSTEM
	//}}    
	//{{ 2009. 12. 15  최육사	아이템 삭제사유
    void Query_DeleteItem( IN std::vector< KDeletedItemInfo >& vecDeleted, OUT std::vector< KDeletedItemInfo >& vecFailed );
	//}}

	//{{ 2009. 10. 28  최육사	길드레벨
	bool Query_GetGuildMemberGrade( IN UidType iUnitUID, OUT u_char& ucGuildMemberGrade );
	//}}

#ifdef GIANT_RESURRECTION_CASHSTONE
	//{{ //2011.12.19 lygan_조성욱 // 중국에 캐쉬샵에서 구입되는 부활석 잔존관련 작업(재무 심사용)
	int Query_UpdateResurrection_stone( IN const UidType iUnitUID, IN const int iNumResurrectionStone, OUT int& iACKNumResurrectionStone);
	//}}
#endif //GIANT_RESURRECTION_CASHSTONE


	//{{ 2012. 10. 23	최육사		DB업데이트 코드 리팩토링
#ifdef SERV_DB_UPDATE_UNIT_INFO_REFACTORING
	bool Query_UpdateUnitInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck );
	bool Query_UpdatePvpInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck );
	bool Query_UpdatePvpInfo( IN const KDBE_QUIT_USER_PVP_RESULT_UPDATE_NOT& kNot );
	bool Query_UpdateLastGamePoint( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateResurrectionStone( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck );
	bool Query_UpdateQuestInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateTitleInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateSkillSlotInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateCommunityOption( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateWishList( IN const UidType iUserUID, IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateRankingInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateGuildInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	bool Query_UpdateConnectTimeEventInfo( IN const UidType iUserUID, IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#endif //SERV_ADD_EVENT_DB
	bool Query_UpdatePetInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck );
	bool Query_UpdateGameCountInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateHenirRewardCount( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	//{{ 2012. 10. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool Query_UpdateLastPosition( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
	bool Query_UpdateGamePlayStatus( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	bool Query_UpdateBuffEffect( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	bool Query_UpdateLocalRankingInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	bool Query_UpdateRidingPetInfo( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq, OUT KDBE_UPDATE_UNIT_INFO_ACK& kAck );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
	bool Query_UpdateAccumulationSpirit( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	bool Query_UpdateGateOfDarknessSupportEventTime( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
	bool Query_UpdateEventNoteViewCount( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#endif SERV_ELESIS_UPDATE_EVENT

#endif SERV_DB_UPDATE_UNIT_INFO_REFACTORING
	//}}
	//{{ 2013. 07. 08	최육사	공유 은행
#ifdef SERV_SHARING_BANK_TEST
	bool Query_UpdateShareItem( IN const KDBE_UPDATE_SHARE_ITEM_REQ& kReq, OUT KDBE_UPDATE_SHARE_ITEM_ACK& kAck );
#endif SERV_SHARING_BANK_TEST
	//}}	
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	bool Quety_WeddingLetter( IN UidType iSendUserUID, IN KDBE_WEDDING_PROPOSE_REQ& kPacket_, IN OUT KDBE_WEDDING_PROPOSE_ACK& kAck );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	bool Query_UpdateLastLogOffDate( IN const KDBE_UPDATE_UNIT_INFO_REQ& kReq );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_SKILL_PAGE_SYSTEM
	void LogAboutCheckingTooManySkillDatas( IN const KDBE_EXPIRE_CASH_SKILL_POINT_REQ& kPacket_ ) const;
// 	void DoQueryUpdatingSkillInfo( OUT int& iOk_, IN const UidType lastSenderUID_, IN const UidType iUnitUID_,
// 		IN const KRetrievedSkillPageData& retrievedSkillPageData_ );
#endif // SERV_SKILL_PAGE_SYSTEM

protected:
    // packet send function
    template < class T > void SendToServer( unsigned short usEventID, const T& data );
    template < class T > void SendToUser( UidType nTo, unsigned short usEventID, const T& data );
    void SendToServer( unsigned short usEventID );
    void SendToUser( UidType nTo, unsigned short usEventID );

	//{{ 2009. 10. 30  최육사	길드레벨
	template < class T > void SendToLoginServer( unsigned short usEventID, const T& data );	
	//}}

	//{{ 2010. 11. 25	조효진	추가 수정 임규수 일본 추가
//#ifdef SERV_SELECT_UNIT_NEW
	// 2014.02.28 이지헌 SendToLogDB 는 SERV_SELECT_UNIT_NEW 없어도 사용하는 곳이 있으므로 디파인 제거 했습니다.
	template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
	void SendToLogDB( unsigned short usEventID );
//#endif SERV_SELECT_UNIT_NEW
	//}}

#ifdef SERV_ADD_EVENT_DB
	template < class T > void SendToEventDB( unsigned short usEventID, const T& data );
	void SendToEventDB( unsigned short usEventID );
#endif //SERV_ADD_EVENT_DB

protected:
    // event handler
    DECL_ON_FUNC( DBE_UPDATE_UNIT_INFO_REQ );

	_DECL_ON_FUNC( DBE_MY_UNIT_AND_INVENTORY_INFO_LIST_REQ, std::wstring );

   //{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
   DECL_ON_FUNC( DBE_GAME_CREATE_UNIT_REQ );
#else
   _DECL_ON_FUNC( DBE_GAME_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_REQ );
#endif SERV_NEW_CREATE_CHAR_EVENT
   //}}
   _DECL_ON_FUNC( DBE_GAME_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_REQ );
   //{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
   _DECL_ON_FUNC( DBE_GAME_FINAL_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_REQ );
   _DECL_ON_FUNC( DBE_GAME_RESTORE_UNIT_REQ, KEGS_RESTORE_UNIT_REQ );
#endif SERV_UNIT_WAIT_DELETE
   //}}

   _DECL_ON_FUNC( DBE_GAME_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_REQ );
	//{{ 2009. 5. 28  최육사	채널이동
	DECL_ON_FUNC( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ );
	//}}

   _DECL_ON_FUNC( DBE_ADMIN_MODIFY_UNIT_LEVEL_REQ, KEGS_ADMIN_MODIFY_UNIT_LEVEL_REQ );
   _DECL_ON_FUNC( DBE_ADMIN_CHANGE_ED_REQ, KEGS_ADMIN_CHANGE_ED_REQ );

    DECL_ON_FUNC( DBE_BUY_ED_ITEM_REQ );
    DECL_ON_FUNC( DBE_INSERT_ITEM_REQ );
	DECL_ON_FUNC( DBE_NEW_QUEST_REQ );
	DECL_ON_FUNC( DBE_GIVE_UP_QUEST_REQ );
	DECL_ON_FUNC( DBE_QUEST_COMPLETE_REQ );
	DECL_ON_FUNC( DBE_INSERT_SKILL_REQ );

	//{{ 2012. 03. 23	박세훈	관리자용 치트키 오류 수정
#ifdef SERV_FIX_THE_ADMIN_CHEAT
	_DECL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_REQ, KDBE_INIT_SKILL_TREE_REQ );
#else
	_DECL_ON_FUNC( DBE_ADMIN_INIT_SKILL_TREE_REQ, UidType );
#endif SERV_FIX_THE_ADMIN_CHEAT
	//}}

   _DECL_ON_FUNC( DBE_SEARCH_UNIT_REQ, std::wstring );
   _DECL_ON_FUNC( DBE_KNM_REQUEST_NEW_FRIEND_INFO_REQ, std::wstring );
    DECL_ON_FUNC( DBE_INSERT_TRADE_ITEM_REQ );
	//{{ 2009. 2. 10  최육사	개인거래 버그 예방
	DECL_ON_FUNC( DBE_INSERT_TRADE_ITEM_BY_SERVER_NOT );
	//}}

   _DECL_ON_FUNC( DBE_INSERT_PURCHASED_CASH_ITEM_REQ, KDBE_INSERT_ITEM_REQ );
    DECL_ON_FUNC( DBE_INSERT_PURCHASED_CASH_PACKAGE_REQ );
	
	DECL_ON_FUNC( DBE_OPEN_RANDOM_ITEM_REQ );	
	DECL_ON_FUNC( DBE_ITEM_MANUFACTURE_REQ );

   _DECL_ON_FUNC( DBE_CHANGE_NICK_NAME_REQ, KEGS_CHANGE_NICK_NAME_REQ );
    
	DECL_ON_FUNC( DBE_NEW_BLACKLIST_USER_REQ );
	DECL_ON_FUNC( DBE_DEL_BLACKLIST_USER_REQ );

	//{{ 2009. 9. 22  최육사	전직캐쉬	
	DECL_ON_FUNC( DBE_CHANGE_UNIT_CLASS_REQ );
	//}}

	DECL_ON_FUNC( DBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ );

    DECL_ON_FUNC( DBE_ENCHANT_ITEM_REQ );
	//{{ 2008. 12. 21  최육사	강화 복구
	DECL_ON_FUNC( DBE_RESTORE_ITEM_REQ );
	//}}
	DECL_ON_FUNC( DBE_RESOLVE_ITEM_REQ );
	DECL_ON_FUNC( DBE_SOCKET_ITEM_REQ );

	DECL_ON_FUNC( DBE_INIT_SKILL_TREE_REQ );
	
	DECL_ON_FUNC( DBE_RECOMMEND_USER_REQ );
	DECL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_REQ );
	DECL_ON_FUNC( DBE_PREPARE_INSERT_LETTER_TO_POST_REQ );
	DECL_ON_FUNC( DBE_INSERT_LETTER_TO_POST_REQ );
	
	//{{ 2008. 9. 18  최육사	우편함
	DECL_ON_FUNC( DBE_GET_POST_LETTER_LIST_REQ );
	DECL_ON_FUNC( DBE_READ_LETTER_NOT );
	DECL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_REQ );
	DECL_ON_FUNC( DBE_DELETE_LETTER_NOT );
	//}}	

	//080407.hoons. 사제관계 지우기
	DECL_ON_FUNC( DBE_DEL_TUTORIAL_REQ );
	DECL_ON_FUNC( DBE_INSERT_TUTORIAL_REQ );

	DECL_ON_FUNC( DBE_RESET_SKILL_REQ );
	DECL_ON_FUNC( DBE_EXPAND_SKILL_SLOT_REQ );
    DECL_ON_FUNC( DBE_EXPAND_INVENTORY_SLOT_REQ );
	//{{ 2008. 12. 14  최육사	캐릭터 슬롯 확장
	DECL_ON_FUNC( DBE_EXPAND_CHAR_SLOT_REQ );
	//}}

    DECL_ON_FUNC( DBE_GET_WISH_LIST_REQ );

#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	DECL_ON_FUNC( DBE_UPDATE_EVENT_TIME_REQ );
#endif //SERV_ADD_EVENT_DB
	

	DECL_ON_FUNC( DBE_UPDATE_INVENTORY_ITEM_POS_NOT );

	DECL_ON_FUNC( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ );

	//{{ 2008. 9. 3  최육사		속성강화
	DECL_ON_FUNC( DBE_ATTRIB_ENCHANT_ITEM_REQ );
	DECL_ON_FUNC( DBE_IDENTIFY_ITEM_REQ );
	//}}

	//{{ 2008. 9. 26  최육사	우체국 블랙리스트
	DECL_ON_FUNC( DBE_NEW_POST_BLACK_LIST_REQ );
	DECL_ON_FUNC( DBE_DEL_POST_BLACK_LIST_REQ );
	//}}

	//{{ 2008. 10. 7  최육사	타이틀
	DECL_ON_FUNC( DBE_INSERT_TITLE_REQ );
	//}}

	//{{ 2008. 12. 25  최육사	부여
	DECL_ON_FUNC( DBE_ENCHANT_ATTACH_ITEM_REQ );
	//}}

	//{{ 2008. 11. 18  최육사	아이템 교환
	DECL_ON_FUNC( DBE_ITEM_EXCHANGE_REQ );
	//}}

	//{{ 2009. 4. 8  최육사		닉네임 변경
	DECL_ON_FUNC( DBE_DELETE_NICK_NAME_REQ );
	//}}

	DECL_ON_FUNC( DBE_INSERT_CASH_SKILL_POINT_REQ );
	DECL_ON_FUNC( DBE_EXPIRE_CASH_SKILL_POINT_REQ );

	//{{ 2009. 8. 4  최육사		봉인 스킬
	DECL_ON_FUNC( DBE_UNSEAL_SKILL_REQ );
	//}}

	//{{ 2009. 5. 11  최육사	실시간 아이템
	DECL_ON_FUNC( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ );
	DECL_ON_FUNC( DBE_GET_TEMP_ITEM_REQ );
	//}}

    DECL_ON_FUNC( DBE_REQUEST_FRIEND_REQ );
    DECL_ON_FUNC( DBE_ACCEPT_FRIEND_REQ );
    DECL_ON_FUNC( DBE_DENY_FRIEND_REQ );
    DECL_ON_FUNC( DBE_BLOCK_FRIEND_REQ );
    DECL_ON_FUNC( DBE_UNBLOCK_FRIEND_REQ );
    DECL_ON_FUNC( DBE_DELETE_FRIEND_REQ );
    DECL_ON_FUNC( DBE_MOVE_FRIEND_REQ );
    DECL_ON_FUNC( DBE_MAKE_FRIEND_GROUP_REQ );
    DECL_ON_FUNC( DBE_RENAME_FRIEND_GROUP_REQ );
    DECL_ON_FUNC( DBE_DELETE_FRIEND_GROUP_REQ );
    DECL_ON_FUNC( DBE_FRIEND_MESSAGE_NOT );

	//{{ 2009. 7. 29  최육사	item set cheat
	DECL_ON_FUNC( DBE_ADMIN_GET_ITEM_SET_NOT );
	//}}

	//{{ 2009. 8. 27  최육사	봉인
	DECL_ON_FUNC( DBE_SEAL_ITEM_REQ );
	DECL_ON_FUNC( DBE_UNSEAL_ITEM_REQ );
	//}}

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST
	
	//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
#else
	DECL_ON_FUNC( DBE_CREATE_GUILD_REQ );
#endif SERV_GUILD_CHANGE_NAME
	//}}

   _DECL_ON_FUNC( DBE_INVITE_GUILD_NICKNAME_CHECK_REQ, KELG_INVITE_GUILD_ACK );
	DECL_ON_FUNC( DBE_JOIN_GUILD_REQ );

	DECL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_GRADE_REQ );
	DECL_ON_FUNC( DBE_CHANGE_GUILD_MESSAGE_REQ );
	DECL_ON_FUNC( DBE_KICK_GUILD_MEMBER_REQ );
	DECL_ON_FUNC( DBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ );

	DECL_ON_FUNC( DBE_DISBAND_GUILD_REQ );
	DECL_ON_FUNC( DBE_EXPAND_GUILD_MAX_MEMBER_REQ );

	//{{ 2009. 10. 27  최육사	길드레벨
	DECL_ON_FUNC( DBE_UPDATE_GUILD_EXP_REQ );   
	//}}

#endif GUILD_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	DECL_ON_FUNC( DBE_RESET_GUILD_SKILL_REQ );
	DECL_ON_FUNC( DBE_INIT_GUILD_SKILL_REQ );

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 12. 8  최육사	크리스마스이벤트
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	DECL_ON_FUNC( DBE_CHECK_TIME_EVENT_COMPLETE_REQ );
#endif //SERV_ADD_EVENT_DB
	//}}
#ifdef SERV_GLOBAL_BILLING
	//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
	DECL_ON_FUNC( DBE_GET_NICKNAME_BY_UNITUID_REQ );
	//}} 허상형 : [2010/8/19/] //	선물 기능 개편
#endif // SERV_GLOBAL_BILLING

// 현재 사용하는 곳 없음. 주석처리
// #ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
// 	DECL_ON_FUNC( DBE_INSERT_GIANT_DELETE_UNIT_REQ );
// #endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

	//{{ 2010. 01. 11  최육사	추천인리스트
	DECL_ON_FUNC( DBE_GET_RECOMMEND_USER_LIST_REQ );
	//}}

	//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT
	DECL_ON_FUNC( DBE_ATTENDANCE_CHECK_REQ );
   _DECL_ON_FUNC( DBE_INCREASE_WEB_POINT_LOG_NOT, KDBE_INCREASE_WEB_POINT_ACK );
#endif SERV_WEB_POINT_EVENT
	//}}

	//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	DECL_ON_FUNC( DBE_EXPAND_SKILL_NOTE_PAGE_REQ );
	DECL_ON_FUNC( DBE_REG_SKILL_NOTE_MEMO_REQ );
#endif SERV_SKILL_NOTE
	//}}

	//{{ 2010. 7. 30 최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	DECL_ON_FUNC( DBE_CREATE_PET_REQ );
	DECL_ON_FUNC( DBE_SUMMON_PET_REQ );

	//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
	DECL_ON_FUNC( DBE_CHANGE_PET_NAME_REQ );
#endif SERV_PET_CHANGE_NAME
	//}}

#endif SERV_PET_SYSTEM
	//}}

	//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT
	DECL_ON_FUNC( DBE_RESET_PERIOD_ITEM_REQ );
#endif SERV_RESET_PERIOD_EVENT
	//}}
	//{{ 2011. 01. 04	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM
	DECL_ON_FUNC( DBE_ATTRIB_ATTACH_ITEM_REQ );
#endif SERV_ATTRIBUTE_CHARM
	//}}
	//{{ 2011. 04. 14	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	DECL_ON_FUNC( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ );
	DECL_ON_FUNC( DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ );
	DECL_ON_FUNC( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ );
#endif SERV_PSHOP_AGENCY
	//}}
	//{{ 지헌 - 은행 개편 퀘스트, 캐쉬 작업 
#ifdef SERV_SHARING_BANK_QUEST_CASH
	DECL_ON_FUNC( DBE_SHARING_BACK_OPEN_REQ);
#endif
	//}}
	//{{ 지헌 - 은행 개편 기념 이벤트용 임시 코드(특정 아이템 은행 공유 이용 체크)			
#ifdef SERV_SHARING_BANK_EVENT
	DECL_ON_FUNC( DBE_SHARING_BANK_EVENT_REQ );
#endif

#ifdef	SERV_SHARING_BANK_TEST
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
	_DECL_ON_FUNC( DBE_GET_SHARE_BANK_REQ, KEGS_GET_SHARE_BANK_REQ  );
#else // SERV_NEW_UNIT_TRADE_LIMIT
	_DECL_ON_FUNC( DBE_GET_SHARE_BANK_REQ, std::wstring );
#endif // SERV_NEW_UNIT_TRADE_LIMIT
	DECL_ON_FUNC( DBE_UPDATE_SHARE_ITEM_REQ );
	_DECL_ON_FUNC( DBE_UPDATE_SHARE_ITEM_FOR_MOVE_SLOT_REQ, KDBE_UPDATE_SHARE_ITEM_REQ );
	_DECL_ON_FUNC( DBE_UPDATE_SEAL_DATA_NOT, KDBE_SEAL_ITEM_REQ );

	DECL_ON_FUNC( DBE_CHANGE_INVENTORY_SLOT_ITEM_REQ );
#endif	SERV_SHARING_BANK_TEST

#ifdef GIANT_RESURRECTION_CASHSTONE
	DECL_ON_FUNC( DBE_REALTIME_RESURRECTION_CASHSTONE_NOT );
	DECL_ON_FUNC( DBE_RESURRECTION_CASHSTONE_NOT );
#endif //GIANT_RESURRECTION_CASHSTONE

#ifdef SERV_ADVERTISEMENT_EVENT
#ifdef SERV_ADD_EVENT_DB
#else //SERV_ADD_EVENT_DB
	DECL_ON_FUNC( DBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT );
#endif //SERV_ADD_EVENT_DB
#endif SERV_ADVERTISEMENT_EVENT

	//{{ 2011. 08. 03	최육사	대전 강제 종료에 대한 예외처리
#ifdef SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
	DECL_ON_FUNC( DBE_QUIT_USER_PVP_RESULT_UPDATE_NOT );
#endif SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
	//}}
	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
	DECL_ON_FUNC( DBE_RETAINING_SELECT_REWARD_REQ );
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	//{{ 2011. 10. 14	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
	DECL_ON_FUNC( DBE_USE_ITEM_IN_INVENTORY_REQ );
#endif SERV_USE_ITEM_DB_UPDATE_FIX
	//}}

	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	DECL_ON_FUNC( DBE_BUY_UNIT_CLASS_CHANGE_REQ );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 04. 30	박세훈	현자의 주문서 접속 이벤트 ( 우편함 중복 체크 )
#ifdef SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
	_DECL_ON_FUNC( DBE_UPDATE_EVENT_TIME_NOT, KDBE_UPDATE_EVENT_TIME_REQ );
#endif SERV_SCROLL_OF_SAGE_CHECK_THE_LETTER_BOX
	//}}
	//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
	DECL_ON_FUNC( DBE_TRADE_COMPLETE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
	//}}
	//{{ 2012. 05. 30	최육사	그노시스 기간 치트
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
	DECL_ON_FUNC( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ );
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
	//}}
	//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
	DECL_ON_FUNC( DBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ );
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
	//}}

	//{{ 우편 ED 거래시 ED 동기화 - 김민성
#ifdef SERV_SEND_LETTER_BEFOR_ED_SYNC
	DECL_ON_FUNC( DBE_SYNC_ED_REQ );
#endif SERV_SEND_LETTER_BEFOR_ED_SYNC
	//}}

//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
	DECL_ON_FUNC_NOPARAM( DBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_REQ );
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
//}}

//{{ 2012. 08. 21	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	DECL_ON_FUNC( DBE_GET_ITEM_FROM_LETTER_ARRANGE_REQ );
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}
//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
	DECL_ON_FUNC( DBE_PVP_WIN_EVENT_CHECK_REQ );
#endif SERV_2012_PVP_SEASON2_EVENT
//}}

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	_DECL_ON_FUNC( DBE_NEW_ACCOUNT_QUEST_GAME_DB_REQ, KDBE_NEW_QUEST_REQ );
	_DECL_ON_FUNC( DBE_ACCOUNT_QUEST_COMPLETE_GAME_DB_REQ, KDBE_ACCOUNT_QUEST_COMPLETE_ACK );
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}

//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
	DECL_ON_FUNC( DBE_PET_AUTO_LOOTING_NOT );
#endif SERV_PET_AUTO_LOOTING
//}}

	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	DECL_ON_FUNC( DBE_OPEN_SYNTHESIS_ITEM_REQ );
#endif SERV_SYNTHESIS_AVATAR
	//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
	DECL_ON_FUNC( DBE_CHANGE_PET_ID_REQ );
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

	//{{ 2013. 3. 11	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	DECL_ON_FUNC( DBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ );
	DECL_ON_FUNC( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ );
	DECL_ON_FUNC( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
	//}}

	//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
	DECL_ON_FUNC( DBE_RECOMMEND_USER_GET_NEXON_SN_REQ );
#endif SERV_RECOMMEND_LIST_EVENT
	//}
#ifdef SERV_ADD_WARP_BUTTON
	DECL_ON_FUNC( DBE_INSERT_WARP_VIP_REQ );
#endif // SERV_ADD_WARP_BUTTON	
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	_DECL_ON_FUNC( DBE_COUPLE_PROPOSE_USER_FIND_REQ, KEGS_COUPLE_PROPOSE_REQ );
	DECL_ON_FUNC( DBE_COUPLE_MAKING_SUCCESS_REQ );
	DECL_ON_FUNC( DBE_WEDDING_PROPOSE_REQ );
	_DECL_ON_FUNC( DBE_CHECK_NICK_NAME_REQ, KEGS_CHECK_NICK_NAME_REQ );
	_DECL_ON_FUNC( DBE_WEDDING_ITEM_INFO_REQ, KEGS_WEDDING_ITEM_INFO_REQ );
	DECL_ON_FUNC( DBE_WEDDING_ITEM_DELETE_REQ );
	DECL_ON_FUNC( DBE_BREAK_UP_REQ );
	DECL_ON_FUNC( DBE_ADMIN_CHANGE_COUPLE_DATE_REQ );
	DECL_ON_FUNC( DBE_CHANGE_LOVE_WORD_REQ );
	DECL_ON_FUNC( DBE_INSERT_WEDDING_REWARD_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	DECL_ON_FUNC( DBE_GET_RIDING_PET_LIST_REQ );
	DECL_ON_FUNC( DBE_CREATE_RIDING_PET_REQ );
	DECL_ON_FUNC( DBE_RELEASE_RIDING_PET_REQ );
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_MOMOTI_EVENT
	DECL_ON_FUNC( DBE_MOMOTI_QUIZ_EVENT_REQ );
#endif SERV_MOMOTI_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
	DECL_ON_FUNC( DBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ );
#endif // SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	DECL_ON_FUNC( DBE_ITEM_EVALUATE_REQ );
	DECL_ON_FUNC( DBE_RESTORE_ITEM_EVALUATE_REQ );
	DECL_ON_FUNC( DBE_ITEM_CONVERT_REQ );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// 작업날짜: 2013-05-13	// 박세훈
	DECL_ON_FUNC( DBE_SEND_LOVE_LETTER_EVENT_REQ );
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	DECL_ON_FUNC( DBE_ADMIN_CHANGE_UNIT_CLASS_REQ );
	DECL_ON_FUNC( DBE_ADMIN_AUTO_GET_ALL_SKILL_REQ );
	DECL_ON_FUNC( DBE_ADMIN_GET_SKILL_REQ );
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈
	DECL_ON_FUNC( DBE_JUMPING_CHARACTER_UPDATE_REQ );
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_COUPON_EVENT
	DECL_ON_FUNC( DBE_COUPON_ENTRY_REQ );
#endif SERV_COUPON_EVENT

#ifdef SERV_READY_TO_SOSUN_EVENT
	DECL_ON_FUNC( DBE_READY_TO_SOSUN_EVENT_REQ );
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	_DECL_ON_FUNC( DBE_EVENT_PROPOSE_USER_FIND_REQ, KEGS_USE_PROPOSE_ITEM_REQ );
	DECL_ON_FUNC( DBE_EVENT_MAKING_SUCCESS_REQ );
	DECL_ON_FUNC( DBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ );
	DECL_ON_FUNC( DBE_EVENT_DIVORCE_REQ );
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_PERIOD_PET
	DECL_ON_FUNC( DBE_RELEASE_PET_REQ );
#endif SERV_PERIOD_PET

#ifdef SERV_RECRUIT_EVENT_BASE
	DECL_ON_FUNC( DBE_USE_RECRUIT_TICKET_REQ );
	DECL_ON_FUNC( DBE_REGISTER_RECRUITER_REQ );
	DECL_ON_FUNC( DBE_GET_RECRUIT_RECRUITER_LIST_REQ );
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
	DECL_ON_FUNC_NOPARAM( DBE_GET_EVENT_INFO_REQ );
	DECL_ON_FUNC( DBE_SET_EVENT_INFO_NOT );
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_NEW_YEAR_EVENT_2014
	DECL_ON_FUNC( DBE_2013_EVENT_MISSION_COMPLETE_REQ );
	DECL_ON_FUNC( DBE_2014_EVENT_MISSION_COMPLETE_REQ );
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_USE_GM_TOOL_INFO
	DECL_ON_FUNC( DBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT );
#endif //SERV_USE_GM_TOOL_INFO

#ifdef SERV_GLOBAL_MISSION_MANAGER
	DECL_ON_FUNC( DBE_REGIST_GLOBAL_MISSION_CLEAR_NOT );
#endif SERV_GLOBAL_MISSION_MANAGER

#ifdef SERV_EVENT_CHECK_POWER
	DECL_ON_FUNC( DBE_START_CHECK_POWER_REQ );
	_DECL_ON_FUNC( DBE_UPDATE_CHECK_POWER_REQ, KDBE_START_CHECK_POWER_REQ );
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
	DECL_ON_FUNC( DBE_ITEM_EXTRACT_REQ );
	DECL_ON_FUNC( DBE_USE_FINALITY_SKILL_REQ );
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_GOOD_ELSWORD
    DECL_ON_FUNC( DBE_EXPAND_BANK_INVENTORY_REQ );
#endif // SERV_GOOD_ELSWORD

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	DECL_ON_FUNC( DBE_SOCKET_EXPAND_ITEM_REQ );
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_SKILL_PAGE_SYSTEM
	DECL_ON_FUNC( DBE_EXPAND_SKILL_PAGE_REQ );
	DECL_ON_FUNC( DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ );
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_ENTRY_POINT
    _DECL_ON_FUNC( DBE_CHARACTER_LIST_REQ, std::wstring );
    _DECL_ON_FUNC( DBE_ENTRY_POINT_CHECK_NICK_NAME_REQ, KEGS_ENTRY_POINT_CHECK_NICK_NAME_REQ );
    DECL_ON_FUNC( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ );
#endif //SERV_ENTRY_POINT

#ifdef SERV_EVENT_PET_INVENTORY
	DECL_ON_FUNC( DBE_EVENT_PET_EVENT_FOOD_EAT_REQ );
#endif SERV_EVENT_PET_INVENTORY

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	DECL_ON_FUNC( DBE_EVENT_CHUNG_GIVE_ITEM_REQ );
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	DECL_ON_FUNC( DBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ );
	DECL_ON_FUNC( DBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT );
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	DECL_ON_FUNC( DBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_REQ );
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
};

template < class T >
void KGSGameDBThread::SendToServer( unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_GAME_DB, 0, PI_GS_SERVER, KBaseServer::GetKObj()->GetUID(), NULL, usEventID, data );
}

template < class T >
void KGSGameDBThread::SendToUser( UidType nTo, unsigned short usEventID, const T& data )
{
    KncSend( PI_GS_GAME_DB, 0, PI_GS_USER, nTo, NULL, usEventID, data );
}

//{{ 2009. 10. 30  최육사	길드레벨
template < class T >
void KGSGameDBThread::SendToLoginServer( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_GAME_DB, 0, PI_LOGIN_USER, 0, NULL, usEventID, data );
}
//}}

//{{ 2010. 11. 25	조효진	추가 수정 임규수 일본 추가
//#ifdef SERV_SELECT_UNIT_NEW
template < class T >
void KGSGameDBThread::SendToLogDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_GAME_DB, 0, PI_LOG_DB, 0, NULL, usEventID, data );
}
//#endif SERV_SELECT_UNIT_NEW
//}}

#ifdef SERV_ADD_EVENT_DB
template < class T >
void KGSGameDBThread::SendToEventDB( unsigned short usEventID, const T& data )
{
	KncSend( PI_GS_GAME_DB, 0, PI_EVENT_DB, 0, NULL, usEventID, data );
}
#endif //SERV_ADD_EVENT_DB