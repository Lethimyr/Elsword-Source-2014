#pragma once
#include "RoomMonsterManager.h"
#include "X2Data/XSLBattleFieldManager.h"


//{{ 2011. 10. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM


class KBattleFieldMonsterManager : public KRoomMonsterManager
{
public:
	// 몬스터 죽음 카운트
	struct SNpcDieCount
	{
		int		m_iNormalNpcDieCount;
		int		m_iLowEliteNpcDieCount;
		int		m_iHighEliteNpcDieCount;
		int		m_iMiddleBossDieCount;
		int		m_iBossDieCount;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		int		m_iEventBossDieCount;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT

		SNpcDieCount()
		{
			Clear();
		}

		void Clear()
		{
			m_iNormalNpcDieCount	= 0;
			m_iLowEliteNpcDieCount	= 0;
			m_iHighEliteNpcDieCount	= 0;
			m_iMiddleBossDieCount		= 0;
			m_iBossDieCount				= 0;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
			m_iEventBossDieCount		= 0;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
		}
	};

	// 출현 몬스터 타입 정보
	typedef std::pair< int, int >	KSubStageKey;
	struct SUB_STAGE_NPC_INFO
	{
		std::set< int > m_setNormalNpcUID;
		std::set< int > m_setMiddleBossNpcUID;
		std::set< int > m_setBossNpcUID;
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
		std::set< int > m_setEventBossNpcUID;
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
	};

	// 몬스터 리스폰 예약 정보
	struct SMonsterRespawnInfo
	{
		int				m_iMonsterGroupID;
		double			m_fRespawnTime;
		boost::timer	m_tTimer;

		SMonsterRespawnInfo()
		{
			m_iMonsterGroupID = 0;
			m_fRespawnTime = 0.0;
		}

		bool IsRespawnTimeOver() const
		{
            return ( m_tTimer.elapsed() > m_fRespawnTime );
		}
	};

	// 난입자를 위한 현재 생성된 몬스터 리스트
	struct SNpcInfoForClient
	{
		KNPCUnitReq								m_kNpcInfo;
		std::vector< KAttribEnchantNpcInfo >	m_vecAttribNpcInfo;
	};

public:
    KBattleFieldMonsterManager();
    virtual ~KBattleFieldMonsterManager();

	// 게임 시작, 종료 처리
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-14	// 박세훈
	void	StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue, IN const bool bBossField );
	bool	FirstCreateBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID );
	bool	IsAliveBossMonster( IN const int iNpcUID ) const;
#else // SERV_BATTLE_FIELD_BOSS
	void	StartGame( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount, IN const int iDangerousValue );
#endif // SERV_BATTLE_FIELD_BOSS
	void	EndGame();
	void	OnCloseRoom();

	// 몬스터 신규 출현 처리
	bool	FirstCreateMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iPlayerCount );

	// 리스폰 처리
	bool	CheckRespawnMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
								 IN const int iPlayerCount, 
								 IN OUT KDangerousEventInfo& kDangerousEvent,
								 OUT KEGS_NPC_UNIT_CREATE_NOT& kResultNot );

	// 보스 몬스터 처리
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	bool	CheckMiddleBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
									IN const int iDangerousValue,
									IN OUT KDangerousEventInfo& kDangerousEvent,
									OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot );
	void	CreateMiddleBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetMiddleBossMonsterDie( IN const int iNpcUID );
	bool	IsMiddleBossMonster( IN const int iNpcUID ) const;
	bool	IsMiddleBossMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveMiddleBossList.find( iNpcUID ) != m_mapAliveMiddleBossList.end() ); }
	bool	IsRemainMiddleBoss()									{ return (m_mapAliveMiddleBossList.begin() != m_mapAliveMiddleBossList.end()); }
	bool	GetMiddleBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
	void	GetMiddleBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList );
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	bool	CheckEventBossMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
									IN const int iDangerousValue,
									IN OUT KDangerousEventInfo& kDangerousEvent,
									OUT KEGS_NPC_UNIT_CREATE_MIDDLE_BOSS_NOT& kResultNot );
	void	CreateEventBossMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetEventBossMonsterDie( IN const int iNpcUID );
	bool	IsEventBossMonster( IN const int iNpcUID ) const;
	bool	IsEventBossMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveEventBossList.find( iNpcUID ) != m_mapAliveEventBossList.end() ); }
	bool	IsRemainEventBoss()									{ return (m_mapAliveEventBossList.begin() != m_mapAliveEventBossList.end()); }
	bool	GetEventBossNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
	void	GetEventBossMonsterList( OUT std::vector< std::vector<KNPCUnitReq> >& vecNpcDataList );
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT


	void	GetAliveMonsterList( OUT std::vector< KNPCUnitReq >& vecNpcList, OUT std::map< int, KAttribEnchantNpcInfo >& mapAttirbNpcInfo ) const;
	int		GetAtStartedMonsterCount() const			{ return m_iAtStartedMonsterCount; }

	// 몬스터 처리
	bool	CreateMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID );
	bool	SetAttribMonster( IN const int iNpcUID, IN const KAttribEnchantNpcInfo& kInfo );

	// 몬스터 죽음 정보
	void	IncreaseMonsterDieCount( IN const char cMonsterTypeFator );

	bool	IsAttribNpc( IN const int iNpcUID );

	// for lua
	int		GetNormalNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iNormalNpcDieCount; }
	int		GetLowEliteNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iLowEliteNpcDieCount; }
	int		GetHighEliteNpcDieCount_LUA() const			{ return m_kNpcDieCount.m_iHighEliteNpcDieCount; }
	int		GetMiddleBossDieCount_LUA() const			{ return m_kNpcDieCount.m_iMiddleBossDieCount; }
	int		GetBossDieCount_LUA() const					{ return m_kNpcDieCount.m_iBossDieCount; }

	//{{ 배틀 필드에서 몬스터 생성 오류 수정 - 김민성 -- 2012년 11월 15일 패치 예정
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	void	SetNpcOwner( IN const int iNPCUID, IN const UidType iOwnerUnit );
	void	EraseNpcOwner( IN const int iNPCUID );
	void	GetNpcOwnerListByUnitUID( IN const UidType iUnitUID, OUT std::vector< int >& vecNpcUID );
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	//}
	//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	void	ClassifyMonsterEvent( OUT std::vector<int>& vecStartEventID
								, OUT std::vector<int>& vecEndEventID
								);

	void	StartedMonsterEventProcess( IN const std::vector<int>& vecStartEventID
									  , IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
									  , IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
									  );

	void	EndedMonsterEventProcess( IN const std::vector<int>& vecEndEventID
									, OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
									);

	bool	CheckEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
							 , OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
							 , OUT std::vector<KERM_NPC_UNIT_DIE_NOT>& veckPacketNpcDie
							 );

	void	CheckRespawnEventMonster( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID
									, IN OUT KEGS_NPC_UNIT_CREATE_NOT& kNpcCreateNot
									);

	void	CreateEventMonster( IN const KNPCUnitReq& kNpcInfo, OUT int& iCreatedNpcUID );
	bool	SetEventMonsterDie( IN const int iNpcUID, IN const UidType iAttUnitUID );
	bool	GetEventMonsterEventID( IN const int iNpcUID, OUT int& iEventID ) const;

	bool	IsEventMonster( IN const int iNpcUID ) const;
	bool	IsEventMonsterAlive( IN const int iNpcUID ) const	{ return ( m_mapAliveEventNpcList.find( iNpcUID ) != m_mapAliveEventNpcList.end() ); }
	bool	GetEventNpcData( IN const int iNpcUID, OUT NPC_DATA& kNpcData ) const;
#endif SERV_FIELD_EVENT_MONSTER
	//}}

	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	int		GetAttribNpcAttribCount( IN const int iNpcUID );
#endif SERV_REFORM_ITEM_DROP
	//}}

protected:
	int										m_iAtStartedMonsterCount;		// 배틀필드 생성직후 소환시킬 몬스터 수 ( 최대 몬스터 수 )
	SNpcDieCount							m_kNpcDieCount;
	std::map< int, SMonsterRespawnInfo >	m_mapReserveMonsterRespawn;		// key : GroupID, value : RespawnInfo
	std::map< int, SNpcInfoForClient >		m_mapNpcInfoForClient;			// 난입자를 위해 보관해두는 몬스터 정보
	
	//{{ 배틀 필드에서 몬스터 생성 오류 수정 - 김민성 -- 2012년 11월 15일 패치 예정
#ifdef SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	std::map< int, UidType >				m_mapNpcOwner;					// 유저가 소환한 몬스터 보관  key : npcuid, value : unituid
#endif SERV_BATTLE_FIELD_NPC_CREATE_ERROR
	//}
	//{{ 2012. 12. 30	박세훈	필드 이벤트 몬스터
#ifdef SERV_FIELD_EVENT_MONSTER
	std::map<int, SMonsterRespawnInfo>		m_mapReserveEventMonsterRespawn;	// key : GroupID, value : RespawnInfo
	std::map< int, std::set<int> >			m_mapEventMonster;					// 현재 진행 중인 몬스터 출현 이벤트의 UID 및 해당 이벤트로 생성된 이벤트 몬스터 UID
	std::map< int, SNpcInfoForClient >		m_mapEventNpcInfoForClient;			// 난입자를 위해 보관해두는 몬스터 정보
	std::map<int, NPC_DATA>	m_mapAliveEventNpcList;		// 현재 생성된 이벤트 몬스터	[key:NpcUID,	 value:NPC_DATA]
	std::map<int, NPC_DATA>	m_mapDieEventNpcList;		// 죽은 이벤트 몬스터 리스트	[key:NpcUID,	 value:NPC_DATA]
#endif SERV_FIELD_EVENT_MONSTER
	//}}
	//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	std::map<int, NPC_DATA>							m_mapAliveMiddleBossList;		// 현재 생성된 중보 몬스터	[key:NpcUID,	 value:NPC_DATA]
	std::vector< std::vector<KNPCUnitReq> >			m_vecMiddleBossListForClient;	// 중보가 생성된 순서대로 그룹별(spawn Group id - npc)
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
	//}
#ifdef SERV_BATTLEFIELD_EVENT_BOSS_INT
	std::map<int, NPC_DATA>							m_mapAliveEventBossList;		// 현재 생성된 이벤트 보스 몬스터	[key:NpcUID,	 value:NPC_DATA]
	std::vector< std::vector<KNPCUnitReq> >			m_vecEventBossListForClient;	// 이보가 생성된 순서대로 그룹별(spawn Group id - npc)
#endif SERV_BATTLEFIELD_EVENT_BOSS_INT
};


#endif SERV_BATTLE_FIELD_SYSTEM
//}}


