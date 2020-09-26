#pragma once

#include "RoomUserManager.h"
#include "BattleFieldParty.h"

//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


#define KBattleFieldUserManagerPtr KBattleFieldUserManager*
#undef KBattleFieldUserManagerPtr
SmartPointer( KBattleFieldUserManager );


class KBattleFieldUserManager : public KRoomUserManager
{
public:
    KBattleFieldUserManager();
    virtual ~KBattleFieldUserManager();

	//{{ 2013. 01. 07	최육사	방에서 필드로 돌아오지 못하는 버그 수정
#ifdef SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	virtual void Reset( USERLIST_TYPE eType = UT_GAME );
#endif SERV_NEVER_RETURN_TO_FIELD_BUG_FIX
	//}}

	// 정보 얻기
	void	GetAllPartyMemberListInfo( OUT std::map< UidType, UidType >& mapUnitUIDPartyUID, OUT std::map< UidType, std::set< UidType > >& mapAllPartyMemberList );
	bool	GetPartyMemberListInfo( IN const UidType iPartyUID, OUT std::vector< UidType >& vecPartyMemberList );		
	int		GetNumMemberTotalParty() const;
	int		GetNumTotalParty() const		{ return static_cast<int>(m_mapPartyList.size()); }
	int		GetSamePartyMember( IN const UidType iUnitUID, USERLIST_TYPE eType = UT_GAME );
	int		GetSamePartyLiveMember( IN const UidType iUnitUID, USERLIST_TYPE eType = UT_GAME );
	void	SetMaxSlot( IN const int iMaxSlot ) { m_MaxSlot = iMaxSlot; }
	int		GetMaxSlot() const				{ return m_MaxSlot; }

	// 확인
	bool	IsExistEmptySlot()				{ return ( GetMaxSlot() > GetNumMember() ); }
	bool	IsReservedPartyMember( IN const UidType iUnitUID );	
	bool	IsPartyMembersSameGuild( IN const UidType iUnitUID );

	// 입장 퇴장 처리
	bool	EnterRoom( IN const KRoomUserInfo& kInfo, IN const KBattleFieldJoinInfo& kJoinInfo, IN const bool bConsiderTeam );
	virtual bool LeaveRoom( IN const UidType iUnitUID );

	// 동기화 처리
	bool	RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList );
	bool	RegIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iRegUnitUID );
	bool	UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const std::vector< UidType >& vecUnitUIDList );
	bool	UnregIntrudeUnitLoadUserList( IN const UidType iUnitUID, IN const UidType iUnregUnitUID );
	void	CheckAndGetIntrudeUnitLoadCompleteUserList( OUT std::vector< UidType >& vecNpcLoadUserList );

	// 경험치 처리
	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	bool	GetRoomUserList( std::map< UidType, KRoomUserPtr >& mapRoomUserList, USERLIST_TYPE eType = UT_GAME );
	bool	AddBattleFieldRewardEXP( IN const UidType iUnitUID, IN const float fLevelFactor, IN const int iExp, OUT KERM_NPC_UNIT_DIE_NOT& kInfo );
#else
	bool	AddBattleFieldRewardEXP( IN const char cNpcLevel,
									 IN const char cMonsterTypeFactor,
									 IN const int iNpcMaxHP,
									 IN const std::map< UidType, float > mapDamageByUser, 
									 IN const int iDungeonID, 
									 IN const char cDifficultyLevel, 
									 OUT KERM_NPC_UNIT_DIE_NOT& kInfo );
#endif SERV_REFORM_ITEM_DROP
	//}}

	// ED처리
	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	bool	AddBattleFieldRewardED( IN const char cNpcLevel,
									IN UidType& iUserUID,
									IN float fPartyBonusRate,
									IN const int iED,
									IN const std::map< UidType, float >& mapDamageByUser,
									OUT KEGS_GET_ITEM_NOT& kInf );
#else
	bool	AddBattleFieldRewardED( IN const char cNpcLevel,
									IN const int iNpcMaxHP,
									IN const std::map< UidType, float > mapDamageByUser, 
									IN const int iED,
									OUT KEGS_GET_ITEM_NOT& kInfo );
#endif SERV_REFORM_ITEM_DROP
	//}}
	
	// 아이템 획득 처리
	bool	GetItemWithRandom( IN const std::map< UidType, float > mapDamageByUser, 
							   IN const UidType iGetItemUnitUID, 
							   IN const int iItemID, 
							   IN const int iDungeonID,  
							   OUT UidType& iGetUnitUID, 
							   OUT UidType& iGSUID );

	// 파티 정보 갱신
	void	UpdatePartyInfo( IN const UidType iUnitUID, IN const KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT& kNot );

/*
	//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	bool	GetQuestDropItem( IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo );
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	//}}
*/

	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	bool	GetQuestDropItem( IN const UidType iUnitUID, IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo, IN float fQuestItemDropRate );
#else
	bool	GetQuestDropItem( IN const UidType iUnitUID, IN const int iBattleFieldID, IN const int iNPCID, OUT std::vector<KRoomUser::QUEST_ITEM_DROP_INFO>& vecDropInfo );
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}
	float	GetBattleFieldPartyBonusRate( IN const UidType& iUnitUID );
	bool	IsComeBackUserInParty( IN UidType iUnitUID );
#endif SERV_REFORM_ITEM_DROP
	//}}

	float		GetPartyTotalDamage( IN const UidType iUnitUID, 
								 IN const std::map< UidType, float>& mapDamageByUser, 
								 OUT std::vector<UidType>& vecPartyUserList, 
								 IN const USERLIST_TYPE eType = UT_GAME );

	virtual bool RequestTradeTo( UidType nCID, UidType nCIDTo );
	virtual bool TradeAcceptedBy( UidType nCID, UidType nCIDAcceptor );

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	void CalculatePartyRank( IN const std::map< UidType, float >& mapDamageByUser, OUT std::map< UidType, bool >& mapBonusItem );
	void GetPartyLevelFactor( IN const UidType iUnitUID, IN char cLevel, OUT float& fPartyLevelFactorMin, OUT float& fPartyLevelFactorMax );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    bool ZombieAlert_IsCheck() const                { return m_bIsCheckZombieAlert; }
    void ZombieAlert_CheckStart();
    void ZombieAlert_CheckEnd();
	bool ZombieAlert_Tick();
    bool ZombieAlert_CheckStart( IN const UidType nCID );

#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-15	// 박세훈
	void	GetUnitListGroupdByParty( OUT std::vector< std::set< UidType > >& vecUnitListGroupdByParty ) const;
#endif // SERV_BATTLE_FIELD_BOSS

protected:
	bool	AddRoomUserInBattleFieldPartyList( IN const UidType iUnitUID, IN const UidType iPartyUID, IN const std::vector< UidType >& vecPartyMemberList );
	bool	DelRoomUserInBattleFieldPartyList( IN const UidType iUnitUID );

protected:
	int										m_MaxSlot;
	std::map< UidType, KBattleFieldParty >	m_mapPartyList;		// key : [partyuid], value : [partyinfo]

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	bool								m_bIsCheckZombieAlert;
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}