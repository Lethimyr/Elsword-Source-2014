#pragma once

#include "Room.h"
#include "X2Data/XSLDungeon.h"
#include "TimerManager.h"
#include "GameEvent/GameEventScriptManager.h"
//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "BattleFieldUserManager.h"
	#include "BattleFieldMonsterManager.h"
	#include "DungeonSecurityManager.h"
	#include "BattleFieldGameManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	#include "KDropTable.h"
#endif SERV_NEW_HENIR_TEST
//}}


//{{ 2011. 10. 27	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


SmartPointer( KBattleFieldRoom );

class KBattleFieldRoom : public KRoom
{
    NiDeclareRTTI;
    DeclToStringW;

public:
#   undef _ENUM
#   define _ENUM( id ) id,
    enum StatDungeon
    {
#   include "StatDungeon_def.h"
    };

	enum TIMER_ENUM
	{
		TE_MONSTER_RESPAWN			= 0,
		TE_BOSS_MONSTER				= 1,
		TE_DANGEROUS_EVENT			= 2,
		TE_INTRUDE_USER_LOAD		= 3,
		TE_NPC_P2P_SYNC_USER_CHECK	= 4,
		TE_WAIT_TIMER				= 5,
		TE_ZOMBIE_USER_ECHO			= 6,
		//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
		TE_DROP_EVENT_CHECK			= 7,
#endif SERV_ITEM_DROP_EVENT
		//}}
		//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
		TE_EVENT_MONSTER			= 8,
#endif SERV_FIELD_EVENT_MONSTER
		//}}
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		TE_MIDDLE_BOSS_MONSTER		= 9,
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}

		TE_MAX,
	};

	struct ITEM_DATA
	{
		bool						m_bIsGet;			// 획득한 아이템인지 여부
		int							m_iDropItemUID;		// 드롭된 아이템 UID값

		int							m_iItemID;			// 아이템ID

		bool						m_bIsGP;			// ED아이템인지 여부
		int							m_iED;				// ED량
		//{{ 2012. 02. 27	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		char						m_cNpcLevel;		// 해당 아이템을 드롭한 죽은 몬스터 레벨 정보
		int							m_iNpcMaxHP;		// 해당 아이템을 드롭한 죽은 몬스터 HP정보
		std::map< UidType, float >	m_mapDamageByUser;	// 해당 아이템 획득시 각 유저가 몬스터에게 주었던 대미지 정보
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		char						m_cEnchantLevel;
		bool						m_bNotify;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}

		ITEM_DATA()
			: m_bIsGet( false )
			, m_iDropItemUID( 0 )
			, m_iItemID( 0 )
			, m_bIsGP( false )
			, m_iED( 0 )
			//{{ 2012. 02. 27	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
			, m_cNpcLevel( 0 )
			, m_iNpcMaxHP( 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
			, m_cEnchantLevel( 0 )
			, m_bNotify( false )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
		{
		}

		bool IsGetItemPossibleUnit( IN const UidType iUnitUID ) const
		{
			return ( m_mapDamageByUser.find( iUnitUID ) != m_mapDamageByUser.end() );
		}
	};

public:
    KBattleFieldRoom();
    virtual ~KBattleFieldRoom();

    // override.
    virtual void Tick();

protected:
    // Get Set	
    virtual void SetRoomInfo( const KRoomInfo& kInfo );
	virtual void GetRoomInfo( KRoomInfo& kInfo );

    // override.
    virtual void ProcessNativeEvent( const KEventPtr& spEvent );
    virtual void StartGame();
    virtual void StartPlay();
    virtual void EndPlay();
    virtual void EndGame();
    virtual int  DecideWinTeam();
    virtual int  GetPlayMode();
    virtual void HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS );
	virtual void CloseRoom();
	virtual float GetRemainingPlayTime(){ return m_fRemainingPlayTime; }
	virtual void DeleteZombie( std::vector<UidType>& vecZombieUnit );	//추후 pvp/dungeon 방별 개별처리가 필요할경우 재작성을 위해

	DECL_ON_FUNC( ERM_OPEN_BATTLE_FIELD_REQ );
	DECL_ON_FUNC( ERM_JOIN_BATTLE_FIELD_REQ );

   _DECL_ON_FUNC( ERM_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT, KEGS_BATTLE_FIELD_JOIN_USER_LOAD_COMPLETE_NOT );

   _DECL_ON_FUNC( ERM_BATTLE_FIELD_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ );
	
	DECL_ON_FUNC( ERM_LEAVE_ROOM_REQ );
   _DECL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ );
   _DECL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_DUNGEON_GAME_START_REQ, KERM_LEAVE_ROOM_REQ );
   _DECL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_PVP_GAME_START_REQ, KERM_LEAVE_ROOM_REQ );

   _DECL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ );
   _DECL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ );
   _DECL_ON_FUNC( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ );
    DECL_ON_FUNC_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ );
   
   _DECL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ );
	//{{ 2009. 5. 11  최육사	실시간아이템
	DECL_ON_FUNC( ERM_GET_ITEM_COMPLETE_NOT );
	//}}
   _DECL_ON_FUNC( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT ); // 통계

   _DECL_ON_FUNC( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT );

   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );

   _DECL_ON_FUNC( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ );
	//DECL_ON_FUNC_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT );
	
   _DECL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ ); // 실시간 부활석

   //{{ 2011. 01. 25  김민성  월드 트리거 전달(던전만 적용중 - 이후 PVP 확장)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
   _DECL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif SERV_WORLD_TRIGGER_RELOCATION
   //}}

	DECL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_NPC_LOAD_COMPLETE_REQ );
//#ifndef SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND
//	DECL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_NPC_P2P_SYNC_COMPLETE_NOT );
//#endif  SERV_OPTIMIZE_ROBUST_USER_NPC_PACKET_SEND

	DECL_ON_FUNC( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT );

	//{{ 2011. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( ERM_DUMP_ROOM_MONSTER_NOT );
	DECL_ON_FUNC( ERM_DUMP_BATTLE_FIELD_NOT );
	DECL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZU_ECHO_ACK );


#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
	DECL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

	//{{ 배틀필드 위험도 확인 치트 - 김민성
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
	DECL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_DANGEROUS_VALUE_REQ );
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
	//}}

	//{{ 배틀필드 커뮤니티 리스트 확인 - 김민성
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
	_DECL_ON_FUNC( ERM_COMMUNITY_IN_BATTLE_FIELD_REQ, KEGS_COMMUNITY_USER_LIST_REQ );
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
	//}}
	//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	DECL_ON_FUNC( ERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT );
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
	//}}

	//{{ 2013. 03. 12	최육사	몬스터 유효 검사
#ifdef SERV_CHECK_EXIST_MONSTER_UID
	_DECL_ON_FUNC( ERM_CHECK_EXIST_MONSTER_UID_REQ, KEGS_CHECK_EXIST_MONSTER_UID_REQ );
#endif SERV_CHECK_EXIST_MONSTER_UID
	//}}

	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	_DECL_ON_FUNC( ERM_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ, KEGS_ADMIN_BATTLE_FIELD_INCREASE_DANGER_REQ );
	_DECL_ON_FUNC( ERM_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ, KEGS_BATTLE_FIELD_NPC_DAMAGED_MAP_REQ );
	_DECL_ON_FUNC( ERM_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT, KEGS_SYNC_BATTLE_FIELD_DAMAGED_MAP_NOT );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	DECL_ON_FUNC( ERM_CALL_MY_LOVER_CHECK_ROOM_REQ );
	_DECL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_CALL_MY_LOVER_REQ, KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ );
	DECL_ON_FUNC( ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ );
	DECL_ON_FUNC( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-09	// 박세훈
	_DECL_ON_FUNC( ERM_LEAVE_ROOM_FOR_WORKINGS_BLOCK_REQ, KERM_LEAVE_ROOM_REQ );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-07	// 박세훈
	DECL_ON_FUNC_NOPARAM( ERM_BOSS_FIELD_INTRUDE_RESTRICTION_REQ );
	DECL_ON_FUNC( ERM_BOSS_FIELD_LOG_NOT );
#endif // SERV_BATTLE_FIELD_BOSS

protected:
	KDropItemData	CreateItemProcess( IN const int iItemID, 
									   IN const char cNpcLevel,
									   IN const int iNpcMaxHP,
									   IN const std::map< UidType, float >& mapDamageByUnit,
									   IN const bool bIsGP = false, 
									   IN const int iED = 0 );

	void			CheckLoadingSuccess();			//한명의 유저가 끈길때 로딩처리를 위해서
	void			CheckMonsterRespawn();
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	void			CheckMiddleBossMonster();
	void			BattleFieldMiddleBossDropProcess( IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID, OUT std::vector< KDropItemData >& vecDropItem_ );
	void			BattleFieldNormalDropProcess( const IN KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}

	//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	void			CheckEventMonster( void );
#endif SERV_FIELD_EVENT_MONSTER
	//}}
	void			CheckDangerousEvent();
	void			CheckContinueTimer();
	void			CheckIntrudeUserLoadComplete();
	void			CheckNpcP2PSyncUser( IN const bool bForce = false );
	void			CheckZombieUserEcho();

	//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
	void			CheckDropEventCount();
#endif SERV_ITEM_DROP_EVENT
	//}}

	//{{ 2009. 5. 11  최육사	npc죽은상태
	bool			IsDropDieState( IN char cState );
	//}}
	//{{ 2010. 8. 16	최육사	서버 코드 정리
	double			GetTotalPlayTime() const					{ return m_fTotalPlayTime; }
	//}}
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			IncreasePartyFever( IN const std::map< UidType, float >& mapDamageByUser );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	void			SetBattleFieldID( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID ) { m_eBattleFieldID = eBattleFieldID; }
	SEnum::BATTLE_FIELD_ID GetBattleFieldID() const	{ return m_eBattleFieldID; }

	void			GetBattleFieldRoomInfo( OUT KBattleFieldRoomInfo& kInfo );
	virtual	void	SendRoomListInfo( IN const int iCode );

	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	virtual void	TimeCountFinishProcess( IN const u_short usEventID ) { /*아무것도 안합니다.*/ }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	
	void			OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck );

	//{{ 배틀필드 커뮤니티 리스트 확인 - 김민성
#ifdef SERV_COMMUNITY_IN_BATTLE_FIELD
	enum
	{
		PAGE_PER_BATTLE_FIELD_USER = 8,
	};

	bool			GetPVPUserList( IN UINT& uiViewPage, IN UINT& uiTotalPage, IN OUT std::vector< KCommunityUserInfo >& vecUserList );
#endif SERV_COMMUNITY_IN_BATTLE_FIELD
	//}}
	void			MakePacket_BattleFieldLeaveLog( IN const UidType iUnitUID, IN const int iReason, OUT KELOG_BATTLE_FIELD_LEAVE_LOG_NOT& kPacket );

	//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	virtual	void	LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
	void	IncreaseDangerousValue( IN const int iIncreaseValue, IN const UidType iGSUID );
	
	void	OnNpcUnitDie( IN const int iPlayerCount
						, IN const bool bIsAttribNpc
						, IN const char cDifficultyLevel
						, IN const char cMonsterGrade
						, IN const bool bIncreaseDanger
						, IN const UidType iGSUID
						);

	void	CheckFieldBossSystem( void );

	void	InitBossFieldCreateInfo( IN const KBossFieldCreateInfo& kBossFieldCreateInfo );

	void	BattleFieldBossDropProcess( const IN KEGS_NPC_UNIT_DIE_REQ& kPacket_, IN KRoomMonsterManager::NPC_DATA& kDieNpcInfo, IN const UidType& iSendUnitUID );
	void	_BossRewardProcess( IN const UidType iUnitUID, IN const std::multimap< float, std::set< UidType > > mmapPartyContribution, OUT std::vector<int>& vecItemID ) const;
	int		EventMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade );
	int		MiddleBossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade );
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	int		EventBossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	int		MonsterDiePrcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket, OUT char& cMonsterGrade );
	int		BossMonsterDieProcess( IN const UidType iUnitUID, IN const KEGS_NPC_UNIT_DIE_REQ& kPacket_, OUT KERM_NPC_UNIT_DIE_ACK& kPacket );
	void	_MakePartyContributionData( IN const std::map<UidType, float>& mapUserContribution, OUT std::multimap< float, std::set< UidType > >& mmapPartyContribution ) const;

	void	_BossFieldCloseProcess( const bool bTimeOut );

	bool	EnterRoom( IN const KRoomUserInfo& kInfo, IN const KBattleFieldJoinInfo& kJoinInfo, IN const bool bConsiderTeam );
#endif // SERV_BATTLE_FIELD_BOSS
private:
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-12-03	// 박세훈
	// 로그 수집
	void	_MakeBossFieldUserLogAtLeave( IN const UidType iUnitUID );
	void	_MakeBossFieldUserLog( IN const UidType iUnitUID, IN const byte byteContributionRank );
#endif // SERV_BATTLE_FIELD_BOSS

protected:
	SEnum::BATTLE_FIELD_ID				m_eBattleFieldID;			// 배틀필드ID

	double								m_fTotalPlayTime;			// 던전&스테이지 로딩 시간을 뺀 실제 플레이 시간입니다.
	
	int									m_nDropItemUID;				// Drop item UID 발급
	std::map< int, ITEM_DATA >			m_mapDropItem;				// 임시로 발급한 item uid를 습득했는지에 대한 체크	[key:DropUID,	 value:ITEM_DATA]

	KBattleFieldGameManager				m_kGameManager;
	KBattleFieldUserManagerPtr			m_spUserManager;
	KBattleFieldMonsterManager			m_kMonsterManager;
	KDungeonSecurityManager				m_kSecurityManager;

	boost::timer						m_tTimer[TE_MAX];

	static DWORD						m_dwMonsterRespawnCheckTick;

	//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	float								m_fItemDropEventProbRate;
#else // SERV_DROP_EVENT_RENEWAL
	int									m_iItemDropEventProbCount;
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
	//}}
	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	float								m_fQuestItemDropEventProbRate;
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	byte								m_byteBossFieldState;
	__time64_t							m_tFieldHoldingTime;
	__time64_t							m_tFieldOpenTime;
	__time64_t							m_tFieldCloseTime;

	KDBE_BOSS_FIELD_LOG_NOT				m_kBossFieldLog;
#endif // SERV_BATTLE_FIELD_BOSS
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


