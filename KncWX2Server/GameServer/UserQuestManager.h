#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "x2data/XSLQuestManager.h"

//:::DB CONNECT TIME:::
//(실시간)-UNIT SELECT를 하여 게임시작시 진행중/완료 QUEST 받아오기
//(실시간)-QUEST 완료시 진행중 삭제 => 완료 UPDATE
//(실시간)-게임종료시(UNIT SELECT 포함) 진행중 QUEST UPDATE
//-unit info update 시마다 퀘스트가 있으면 update 하기
//(실시간)-퀘스트 포기시 진행사항 db삭제

SmartPointer(KGSUser);

class KUserQuestManager
{
public:
	enum 
	{
		MAX_ING_QUEST_NUM = 30,
	};

public:
	KUserQuestManager(void);
	~KUserQuestManager(void);

	//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
	void			SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< KCompleteQuestInfo >& vecCompleteQuest, IN KGSUserPtr spUser );
#else
	void			SetUnitQuest( IN OUT std::vector< KQuestInstance >& vecQuest, IN const std::vector< int >& vecCompleteQuest, IN KGSUserPtr spUser );
#endif SERV_DAILY_QUEST
	//}}	

	bool			AddQuest( IN KQuestInstance& kInfo );
	bool			RemoveQuest( IN int iQuestID );	
	
	//{{begin} 2010-06-07.hoons.클라이언트와 인터페이스를 맞추기위해.
	//{{ 2011. 01. 03	최육사	에픽 퀘스트
//#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	bool			Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool IsAfterQuest = true, IN const bool bSendAck = true );
#else
	bool			Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser, IN const bool bSendAck = true );
#endif SERV_REFORM_QUEST
	//}}
//#else
//	bool			Handler_EGS_NEW_QUEST_REQ( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser );
//#endif SERV_EPIC_QUEST_CHECK_UPDATE
	//}}	

	void			Handler_EGS_GIVE_UP_QUEST_REQ( IN KEGS_GIVE_UP_QUEST_REQ& kReq, IN KGSUserPtr spUser );
	//{{ 2011. 08. 31  김민성	비밀던전 헬모드
//#ifdef SERV_RANDOM_DAY_QUEST
	void			Handler_ERM_NPC_UNIT_DIE_NOT( IN const int iDungeonID,
												IN const char cDifficulty,
												IN const int iMonsterID,
												IN KGSUserPtr spUser,
												IN const char cDungeonMode
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
												, IN const bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST
												);
//#else
//	void			Handler_ERM_NPC_UNIT_DIE_NOT( IN int iDungeonID, IN char cDifficulty, IN int iMonsterID, IN KGSUserPtr spUser );
//#endif SERV_RANDOM_DAY_QUEST
	//}}
	void			Handler_EGS_TALK_WITH_NPC_REQ( IN int iNPCID, IN KGSUserPtr spUser );
	
	void			Handler_OnPVPPlay( IN int iGameType
									 , IN KGSUserPtr spUser
									 //{{ 2012. 09. 18   김민성   대전 플레이 퀘스트 조건 변경
#ifdef SERV_PVP_PLAY_QUEST
									 , IN bool bOfficialMatch
#endif SERV_PVP_PLAY_QUEST
									 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
									 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
									 , IN bool bCouplePvp
									 , IN bool bIsDrawn
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
									 , IN const bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST
									 );

	//{{ 2012. 09. 18	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	void			Handler_OnPVPPlay_AccumulatedOfficialMatchCount( IN KGSUserPtr spUser, IN const bool bIncreaseCount );
#endif SERV_2012_PVP_SEASON2
	//}}

	void			Handler_OnPVPKill( IN int iGameType
									 , IN KGSUserPtr spUser
									 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
									 , IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
									 //}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
									 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
									 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
									 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
									 );

	void			Handler_OnPVPWin( IN int iGameType
									, IN KGSUserPtr spUser
									//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
									//{{ 2012. 02. 22	김민성	대전 퀘스트, PVP NPC 관련 퀘스트	오류 수정
#ifdef SERV_NEW_PVP_QUEST_ERROR_MODIFY
#else
									, IN bool bMatchNPC
#endif SERV_NEW_PVP_QUEST_ERROR_MODIFY
									//}}
									, IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
									//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
									, IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
									);

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	void			Handler_OnEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iEnchantLevel );
	void			Handler_OnSocketItem( IN KGSUserPtr spUser, IN const int iItemID, IN const int iSocketUseCount );
	void			Handler_OnAttribEnchantItem( IN KGSUserPtr spUser, IN const int iItemID, IN const KItemAttributeEnchantInfo& kItemAttribEnchantInfo );
	void			Handler_OnResolveItem( IN KGSUserPtr spUser, IN const int iItemID );
	void			Handler_OnUseSkillPoint( IN KGSUserPtr spUser, IN const int iSkillID );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	void			Handler_OnResolveItem( IN KGSUserPtr spUser );
#endif SERV_MULTI_RESOLVE
	//}}
	//{{ 2012. 04. 17	최육사	DB쿼리 성공 체크
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	void			Handler_OnEnterTheVillage( IN KGSUserPtr spUser );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
	//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	void			Handler_OnEnterTheBattleField( IN KGSUserPtr spUser );
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	//}}
#ifdef SERV_SUB_QUEST_LEARN_NEW_SKILL
	void			Handler_OnLearnNewSkill( IN KGSUserPtr spUser, IN std::vector<int> vecNowLearnSkill );
#endif SERV_SUB_QUEST_LEARN_NEW_SKILL

	void			Handler_EGS_QUEST_COMPLETE_REQ( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin = false );
	void			Handler_DBE_QUEST_COMPLETE_ACK( IN const KDBE_QUEST_COMPLETE_ACK& kAck, IN KGSUserPtr spUser );
	//{{end} 2010-06-07.hoons.클라이언트와 인터페이스를 맞추기위해.

	bool			IsQuest( const IN int iQuestID );
	bool			IsChangeJobQuest();	//현재 진행 퀘스트중 전직 퀘스트가 있는지..
	//{{ 2009. 8. 8  최육사		은행
	bool			IsCompleteQuest( const IN int iQuestID ) { return ( m_mapCompleteQuest.find( iQuestID ) != m_mapCompleteQuest.end() ); }
	//}}
	//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST
	bool			IsExistEventQuest( IN int iQuestID );
#endif SERV_DAY_QUEST
	//}}

	KQuestInstance*	GetQuestInstance( const IN int iQuestID );
	int				GetQuestCompleteCount( IN const int iQuestID );
#ifdef SERV_DAILY_CONSECUTIVE_QUEST_FIX
	bool			CheckCompletedQuestToday( IN const int iQuestID );
#endif //SERV_DAILY_CONSECUTIVE_QUEST_FIX

	void			GetUpdatedQuestInfo( OUT std::vector< KQuestUpdate >& vecQuestData );

	//{{ 2007. 11. 1  최육사  
	int				GetQuestRewardItemCount();
	//}}

	//현재 진행되는 퀘스트중 퀘스트 아이템을 모으는 퀘스트만 넘겨준다.
	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	void			GetOngoingQuestForRoom( IN KGSUserPtr spUser, OUT std::map< int, KSubQuestInfo >& mapOngoingQuest );
#else
	void			GetDropQuestitembyIngQuest( OUT std::map< int, KSubQuestInfo >& mapDropQuestitembyIngQuest, IN KGSUserPtr spUser );
#endif SERV_QUEST_CLEAR_EXPAND
	//}}	

	//{{ 2010. 02. 06  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST
	void			InitEventQuest( IN KGSUserPtr spUser );
	void			CheckQuestEvent( IN KGSUserPtr spUser );
	void			GetReservedGiveUpQuest( OUT std::vector< int >& vecGiveUpQuest );
#endif SERV_DAY_QUEST
	//}}

	//{{ 2010. 04. 05  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
	KCompleteQuestInfo* GetCompleteQuestInfo( IN int iQuestID );
#endif SERV_DAILY_QUEST
	//}}

#ifdef SERV_EPIC_QUEST
	void			CheckEpicQuest_SuccessStateChange( IN int iMapID, IN KGSUserPtr spUser );

	int				GetQuestingSize(){ return m_mapQuesting.size(); }
	int				GetCompleteQuestSize(){ return m_mapCompleteQuest.size(); }
#endif SERV_EPIC_QUEST

	//{{ 2011. 01. 03	최육사	에픽 퀘스트
#ifdef SERV_EPIC_QUEST_CHECK_UPDATE
	void			CheckEpicQuest_NewQuestByBeforeQuest( IN KGSUserPtr spUser, IN const bool bSendAck );
	void			CheckEpicQuest_NewQuestByAfterQuest( IN const int iQuestID, IN KGSUserPtr spUser, IN const bool bSendAck, IN const bool bForce );
#endif SERV_EPIC_QUEST_CHECK_UPDATE
	//}}

	//{{ 2012. 12. 21  서브 퀘스트 타입 추가 (적정 레벨 던전 클리어) - 김민성
#ifdef SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	void			Handler_OnDungeonClear(	IN const KGSUserPtr spUser, 
											IN const int iDungeonID, 
											IN const char cDifficulty, 
											IN const char cDungeonMode, 
											IN const char cRank, 
											IN const int iPlayTime, 
											IN const int iDamage, 
											IN const std::set< UidType >& setPartyMembers, 
											IN const std::map< int, KSubQuestInfo >& mapOngoingQuest,
											IN const std::map< UidType, bool >& mapHaveExpInDungeon, 
											IN const std::map< UidType, bool >&	mapSuitableLevelInfo
#ifdef SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT											
											,IN const bool bChar1
											,IN const bool bChar2
#endif SERV_PARTYPLAY_WITH_DUNGEON_CLEAR_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
											,IN const std::vector<UidType> vecRelation
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
											, IN const bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST
#ifdef SERV_THREE_COLOR_EVENT
											,IN const std::set< int >& setEquippedTitle
#endif SERV_THREE_COLOR_EVENT
											);		// 던전 입장 당시 레벨 - 적정 레벨 이었는가?

	bool			IsSuitableLevelUser( IN UidType CharUID, IN const std::map< UidType, bool >& mapSuitableLevelInfo );
#else
	void			Handler_OnDungeonClear(	IN const KGSUserPtr spUser, 
											IN const int iDungeonID, 
											IN const char cDifficulty, 
											IN const char cDungeonMode, 
											IN const char cRank, 
											IN const int iPlayTime, 
											IN const int iDamage, 
											IN const std::set< UidType >& setPartyMembers, 
											IN const std::map< int, KSubQuestInfo >& mapOngoingQuest,
											IN const std::map< UidType, bool >& mapHaveExpInDungeon );
#endif SERV_SUITABLE_LEVEL_DUNGEON_CLEAR_SUB_QUEST
	//}}
	//{{ 2011. 03. 08	김민성 경험치를 획득한 던전의 퀘스트 완료 조건 변경
//#ifdef SERV_EXP_ACQUISITION_CONDITION_CHANGE
	bool			ExistHaveExpInDungeon( IN UidType CharUID, IN const std::map< UidType, bool >& mapHaveExpInDungeon );
//#endif
	//}}

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	void GetUserAllQuest( OUT std::set< int >& mapPaymentQuest );
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	void GetUserGoingQuest( KGSUserPtr spUser, OUT std::set< int >& mapPaymentQuest );
	bool CheckIsGoingComplete( IN const int iQuestID, IN KGSUserPtr spUser );
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

#ifdef SERV_SUB_QUEST_USE_ITEM
	void CountUseItem( IN KGSUserPtr spUser, IN const int iDungeonID, IN const char cDifficulty, IN const int iItemID );
#endif SERV_SUB_QUEST_USE_ITEM
#ifdef SERV_EVENT_LEVEL_UP_QUEST_CLEAR
	bool Handler_OnCheckLevelUpQuest(IN const KGSUserPtr spUser);
#endif SERV_EVENT_LEVEL_UP_QUEST_CLEAR
	//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
	void GetCompleteChangeClassQuest( OUT std::vector< KCompleteQuestInfo >& vecQuest );
	void GetInProgressClassChangeQuest( OUT std::vector< KQuestInstance >& vecQuest );
	void GetCompleteChangeClassQuest( OUT std::vector< int >& vecQuest );
	void GetInProgressClassChangeQuest( OUT std::vector< int >& vecQuest );
	void SetClassChangeQuest( IN UidType iUnitUID, IN std::map< int, int >& mapComplete, IN std::map< int, int >& mapInProgress );
	void SetClassChangeDeleteQuest( IN UidType iUnitUID, IN std::vector< int >& vecDeleteCompleteQuest, IN std::vector< int >& vecDeleteInProgressQuest );
	void GetClassChangeDeleteQuest( IN char cDownUnitClass, OUT std::vector< int >& vecCompleteDelete, OUT std::vector< int >& vecInProgressDelete );
	void GetInProgressQuest( OUT std::vector< int >& vecQuest );
	void GetCompleteQuest( OUT std::vector< int >& vecQuest );
#endif SERV_UNIT_CLASS_CHANGE_ITEM
	//}}

	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	void ClearAutoCompleteQuest();
	void SetAutoCompleteQuest( IN OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_REQ& kPacketReq );
	bool GetAndDeleteAutoCompleteQuest( OUT KEGS_QUEST_COMPLETE_REQ& kReq );
	void AddAutoCompleteQuestResult( IN const KEGS_QUEST_COMPLETE_ACK& kAck );
	void GetAutoCompleteQuestResult( OUT KEGS_ALL_COMPLETED_QUEST_COMPLETE_ACK& kPacketAck );
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	void CheckEventQuest( IN KGSUserPtr spUser );
	void CheckResetDayEventQuest_AutoAccept( IN KGSUserPtr spUser );
	void CheckLimitLevelQuest( IN KGSUserPtr spUser );
	void Handler_OnEnterTheDungeon( IN KGSUserPtr spUser );
	void Handler_OnFindNPC( IN KGSUserPtr spUser, std::vector< int >& vecNPCData, bool bDungeon );
	bool CheckCompleteSubQuest_BeforGroup( IN int iQuestID, IN int iSubQuestID, IN KGSUserPtr spUser );
	void CheckEpicQuest_EnterDungeon( IN int iDungeonID, IN KGSUserPtr spUser );
	void CheckEpicQuest_EnterBattleField( IN int iMapID, IN KGSUserPtr spUser );
#endif SERV_REFORM_QUEST
	//}}

#ifdef SERV_POINT_COUNT_SYSTEM
	void			SetUpdateQuestInstance( IN KGSUserPtr spUser);
#endif //SERV_POINT_COUNT_SYSTEM

#ifdef SERV_SKILL_USE_SUBQUEST
	void			Handler_EGS_SKILL_USE_REQ( IN int iSkillID, IN KGSUserPtr spUser );
#endif SERV_SKILL_USE_SUBQUEST

protected:
	void			ClearUnitQuest();
	void			OnPVP( IN CXSLQuestManager::SUB_QUEST_TYPE eSubQuestType
						 , IN int iGameType
						 , IN KGSUserPtr spUser
						 //{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
						 , IN bool bOfficialMatch
#endif SERV_NEW_PVP_QUEST
						 //}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT
						 , IN bool bIsHeroNPC
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_RELATIONSHIP_EVENT_SUBQUEST
						 , IN int iPvpChannelClass = 0
						 , IN bool bCouplePvp = false
						 , IN bool bIsDrawn = false
#endif SERV_RELATIONSHIP_EVENT_SUBQUEST
#ifdef SERV_RECRUIT_EVENT_SUBQUEST
						 , IN bool bHasFriend = false
#endif SERV_RECRUIT_EVENT_SUBQUEST

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
						 , IN char killedUserUnitType
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
						 , IN char killedUserUnitClass
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
						 );

#ifdef SERV_EPIC_QUEST
	int				GetNormalQuestSize();
#endif SERV_EPIC_QUEST

protected:
	std::map< int, KQuestInstance >		m_mapQuesting;				// 진행중인 quest
	//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
	std::map< int, KCompleteQuestInfo > m_mapCompleteQuest;			// 완료한   quest
#else
	std::map< int, int >				m_mapCompleteQuest;			// 완료한   quest
#endif SERV_DAILY_QUEST
	//}}

	//{{ 2010. 02. 06  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST
	std::map< int, KEventQuestInfo >	m_mapEventQuest;			// 현재 수락 가능한 이벤트 퀘스트
	std::vector< int >					m_vecReservedGiveUpQuest;	// 예약된 퀘스트 포기 리스트
#endif SERV_DAY_QUEST
	//}}

	//{{ 2007. 11. 1  최육사  퀘스트 보상 아이템 획득 카운트
	int									m_iRewardItemCount;
	//}}

	//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
	std::map< int, KEGS_QUEST_COMPLETE_REQ >	m_mapAutoQuestCompleteReq;
	std::vector< KEGS_QUEST_COMPLETE_ACK >		m_vecAutoQuestCompleteAck;
#endif SERV_ENTER_FIELD_QUEST_CLEAR
	//}}
};
