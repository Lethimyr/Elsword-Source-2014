#pragma once

#include "Event.h"
#include "CommonPacket.h"
#include "ClientPacket.h"
#include "x2data/XSLQuestManager.h"

SmartPointer(KGSUser);

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
class KUserAccountQuestManager
{

public:
	KUserAccountQuestManager(void);
	~KUserAccountQuestManager(void);

	int				GetQuestRewardItemCount();
	KQuestInstance* GetAccountQuestInstance( const IN int iQuestID );
	void			ClearAccountQuest();
	bool			AddAccountQuest( IN KQuestInstance& kInfo );
	bool			RemoveAccountQuest( IN int iAccountQuestID );
	void			SetAccountQuest( IN std::vector< KQuestInstance >& vecAccountQuest, IN std::vector< KCompleteQuestInfo >& vecAccountCompleteQuest, IN OUT std::vector< KQuestInstance >& vecQuest, IN OUT std::vector< KCompleteQuestInfo >& vecCompleteQuest, IN KGSUserPtr spUser );
	void			CheckAccountQuest_NewQuestByAfterQuest( IN const int iAccountQuestID, IN KGSUserPtr spUser, IN const bool bForce );
	void			CheckAccountQuest_NewQuestByBeforeQuest( IN KGSUserPtr spUser );
	bool			IsAccountQuesting( IN int iAccountQuestID );	// 진행 중인 계정 퀘스트 인가?
	bool			IsAccountCompleteQuest( IN int iAccountQuestID );	// 완료 된 계정 퀘스트 인가?
	void			GetAccountQuesting( OUT std::vector< KQuestUpdate >& vecQuestData );
	void			CheckAccountQuest( IN KGSUserPtr spUser );
	int				GetAccountQuestCompleteCount( IN const int iQuestID );
	bool			Handler_EGS_NEW_QUEST_REQ_Account( IN const KEGS_NEW_QUEST_REQ& kReq, IN KGSUserPtr spUser );
	void			Handler_EGS_QUEST_COMPLETE_REQ_Account( IN const KEGS_QUEST_COMPLETE_REQ& kReq, IN KGSUserPtr spUser, IN const bool bIsAdmin = false );
	void			Handler_DBE_QUEST_COMPLETE_ACK_Account( IN const KDBE_QUEST_COMPLETE_ACK& kAck, IN KGSUserPtr spUser );
	void			Handler_CharaterLevelUp_Account( IN KGSUserPtr spUser );
	bool			CheckCompleteSubQuest_BeforGroup_AccountQuest( IN int iQuestID, IN int iSubQuestID, IN KGSUserPtr spUser );

	//{{ 2013. 03. 21	 계정 퀘스트 - 적정 레벨 던전 클리어 조건 추가 - 김민성
#ifdef SERV_ACCOUNT_QUEST_ADD_CONDITION
	void			Handler_OnDungeonClear( IN const KGSUserPtr spUser, 
											IN const int iDungeonID, 
											IN const char cDifficulty, 
											IN const char cDungeonMode, 
											IN const std::set< UidType >& setPartyMembers, 
											IN const std::map< UidType, bool >& mapHaveExpInDungeon,
											IN const std::map< UidType, bool >& mapSuitableLevelInfo );
	bool			IsSuitableLevelUser( IN UidType CharUID, IN const std::map< UidType, bool >& mapSuitableLevelInfo );
#endif SERV_ACCOUNT_QUEST_ADD_CONDITION
	//}

	//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	void				CheckResetDayAccountQuest_AutoAccept( IN KGSUserPtr spUser );
	KCompleteQuestInfo*	GetAccountCompleteQuest( IN int iAccountQuestID );	// 완료 된 계정 퀘스트
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}

protected:

	std::map< int, KQuestInstance >		m_mapAccountQuesting;				// 진행중인 계정 quest
	std::map< int, KCompleteQuestInfo > m_mapCompletedAccountQuest;			// 완료한   계정 quest

	//{{ 2007. 11. 1  최육사  퀘스트 보상 아이템 획득 카운트
	int									m_iRewardItemCount;
	//}}
};
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}