#pragma once

#include "Room.h"
#include "X2Data/XSLDungeon.h"
#include "TimerManager.h"
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	#include "EventDataRefresh/EventMonsterRefreshManager.h"
#else
	#include "GameEvent/GameEventScriptManager.h"
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "DungeonMonsterManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	#include "DungeonSecurityManager.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	#include "KDropTable.h"
#endif SERV_NEW_HENIR_TEST
//}}
//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	#include "BadAttitudeManager.h"
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}
//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	#include "ValentineManager.h"
#endif SERV_EVENT_VALENTINE_DUNGEON
//}
//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	#include "DefenceDungeonManager.h"
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

SmartPointer( KDungeonRoom );

class KDungeonRoom : public KRoom
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

	struct ITEM_DATA
	{
		bool	m_bIsGet;
		int		m_iDropItemUID;

		int		m_iItemID;

		bool	m_bIsGP;
		int		m_iED;

		//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		char	m_cEnchantLevel;
		bool	m_bNotify;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}

		ITEM_DATA()
			: m_bIsGet( false )
			, m_iDropItemUID( 0 )
			, m_iItemID( 0 )
			, m_bIsGP( false )
			, m_iED( 0 )
			//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
			, m_cEnchantLevel( 0 )
			, m_bNotify( false )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
			//}}
		{
		}
	};

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	enum DEFFENC_DUNGEON
	{
		MAX_SMALL_CRYSTAL = 4,
	};
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가
	
	//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	typedef std::pair< int, int >	KSubStageKey;

	struct SUB_STAGE_NPC_INFO
	{
		std::set< int > m_setNormalNpcUID;
		std::set< int > m_setMiddleBossNpcUID;
		std::set< int > m_setBossNpcUID;
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

public:
    KDungeonRoom();
    virtual ~KDungeonRoom();

    // override.
    virtual void	Tick();

protected:
    // Get Set
    virtual void	SetRoomInfo( const KRoomInfo& kInfo );

    // override.
    virtual void	ProcessNativeEvent( const KEventPtr& spEvent );
    virtual void	StartGame();
    virtual void	StartPlay();
    virtual void	EndPlay();
    virtual void	EndGame();
    virtual bool	CheckIfPlayEnd();
    virtual int		DecideWinTeam();
    virtual int		GetPlayMode();
    virtual void	HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS );
	virtual void	CloseRoom();
	virtual void	CheckResultSuccess();
	virtual float	GetRemainingPlayTime(){ return static_cast<float>(m_fRemainingPlayTime + m_fLoadingTime); }
	virtual void	GetRoomInfo( OUT KRoomInfo& kInfo );
	
	void			SendDSSGoNextAllNot( KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT& kDSSGoNextAllNot );

	//{{ 2007. 10. 19  최육사  유저 통계를 위한 함수
	void			BroadCastUpdateUnitInfoWithPartyCheck( 
					std::vector< KRoomUserPtr >& vecKRoomUserPtr, 
					unsigned short usEventID, 
					std::vector< KERM_UPDATE_DUNGEON_UNIT_INFO_NOT >& data,
					int iDungeonID
					);
	//}}

	//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
//#ifdef SERV_MAKE_MONSTER_UID_CHANGE
//	int GetDungeonNpcRandomInitUid();
//#endif SERV_MAKE_MONSTER_UID_CHANGE	
	//}}
	//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	virtual	void	LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}	
	//{{ 2012. 11. 9	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
	//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	void			LogToDB_DungeonLog( IN KRoomUserPtr spRoomUser, IN const int iClearValue, IN const KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfo, IN const KDungeonUnitResultInfo& kDungeonUnitInfo, IN const int iLeaveReason = 0 );
#else
	void			LogToDB_DungeonLog( IN KRoomUserPtr spRoomUser, IN const int iClearValue, IN const KERM_UPDATE_DUNGEON_UNIT_INFO_NOT& kUpdateUnitInfo, IN const KDungeonUnitResultInfo& kDungeonUnitInfo );
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	//}}	
//#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

   _DECL_ON_FUNC( ERM_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ );
	DECL_ON_FUNC( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ );
   //{{ 2011. 12. 09	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _DECL_ON_FUNC( ERM_SINGLE_OPEN_DUNGEON_ROOM_REQ, KERM_OPEN_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
   //}}

	DECL_ON_FUNC( ERM_GAME_START_REQ );
	DECL_ON_FUNC( ERM_LEAVE_ROOM_REQ );
	//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
   _DECL_ON_FUNC( ERM_LEAVE_ROOM_FOR_JOIN_BATTLE_FIELD_REQ, KERM_LEAVE_ROOM_REQ );
   _DECL_ON_FUNC( ERM_LEAVE_ROOM_FOR_ENTER_THE_VILLAGE_REQ, KERM_LEAVE_ROOM_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
   _DECL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_REQ, KEGS_DUNGEON_STAGE_LOAD_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_STAGE_LOAD_COMPLETE_REQ, KEGS_DUNGEON_STAGE_LOAD_COMPLETE_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_OPEN_REQ,			KEGS_DUNGEON_SUB_STAGE_OPEN_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_GO_NEXT_REQ,		KEGS_DUNGEON_SUB_STAGE_GO_NEXT_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ,	KEGS_DUNGEON_SUB_STAGE_LOAD_COMPLETE_REQ );

	//{{ 2009. 7. 20  최육사	비밀스테이지
	DECL_ON_FUNC_NOPARAM( ERM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT, KEGS_DUNGEON_SECRET_STAGE_ENTER_SELECT_NOT );
	//}}   

   _DECL_ON_FUNC( ERM_NPC_UNIT_CREATE_REQ, KEGS_NPC_UNIT_CREATE_REQ );
   _DECL_ON_FUNC( ERM_NPC_UNIT_DIE_REQ, KEGS_NPC_UNIT_DIE_REQ );
   _DECL_ON_FUNC( ERM_USER_UNIT_DIE_REQ, KEGS_USER_UNIT_DIE_REQ );
    DECL_ON_FUNC_NOPARAM( ERM_USER_UNIT_DIE_COMPLETE_REQ );
   
   _DECL_ON_FUNC( ERM_GET_ITEM_REQ, KEGS_GET_ITEM_REQ );
	//{{ 2009. 5. 11  최육사	실시간아이템
	DECL_ON_FUNC( ERM_GET_ITEM_COMPLETE_NOT );
	//}}	
	DECL_ON_FUNC( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ );
   _DECL_ON_FUNC( ERM_DUNGEON_PLAY_INFO_TO_SERVER_NOT, KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT ); // 통계

   _DECL_ON_FUNC( ERM_END_GAME_REQ, KEGS_END_GAME_REQ );
   _DECL_ON_FUNC( ERM_USE_QUICK_SLOT_NOT, KEGS_USE_QUICK_SLOT_NOT );    

   _DECL_ON_FUNC( ERM_CHECK_INVALID_USER_NOT, UidType );

   _DECL_ON_FUNC( ERM_RESURRECT_TO_CONTINUE_DUNGEON_REQ, KEGS_RESURRECT_TO_CONTINUE_DUNGEON_REQ );
	DECL_ON_FUNC_NOPARAM( ERM_SKIP_CONTINUE_DUNGEON_NOT );

    DECL_ON_FUNC( ERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT );
	
   _DECL_ON_FUNC( ERM_STOP_DUNGEON_CONTINUE_TIME_REQ, KEGS_STOP_DUNGEON_CONTINUE_TIME_REQ ); // 실시간 부활석
	
   _DECL_ON_FUNC( ERM_FIELD_UNIT_SYNC_DATA_NOT, KEGS_FIELD_UNIT_SYNC_DATA_NOT );			 // 던전 라운지 싱크
	
	DECL_ON_FUNC_NOPARAM( ERM_PARTY_BAN_USER_REQ );											 // 파티유저밴	

	//{{ 2009. 12. 29  최육사	NPCKILLALL해킹방지
	DECL_ON_FUNC_NOPARAM( ERM_DUNGEON_KILLALLNPC_CHECK_REQ );
	//}}
	//{{ 2010. 11. 18	최육사	아이템 판매 차단
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM
   _DECL_ON_FUNC( ERM_CHECK_SELL_ED_ITEM_REQ, KEGS_SELL_ED_ITEM_REQ );
#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
	//}}
#ifdef SERV_CREATE_ELITE_FOR_ADMIN
   _DECL_ON_FUNC( ERM_ADMIN_NPC_UNIT_CREATE_REQ, KEGS_ADMIN_NPC_UNIT_CREATE_REQ );
#endif SERV_CREATE_ELITE_FOR_ADMIN

   //{{ 2011. 01. 25  김민성  월드 트리거 전달(던전만 적용중 - 이후 PVP 확장)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
   _DECL_ON_FUNC( ECN_WORLD_TRIGGER_RELOCATION_REQ, KEGS_WORLD_TRIGGER_RELOCATION_REQ );
#endif SERV_WORLD_TRIGGER_RELOCATION
   //}}
   //{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
   _DECL_ON_FUNC( ERM_UDP_CHECK_KICK_USER_NOT, KEGS_UDP_CHECK_KICK_USER_NOT );
#endif UDP_CAN_NOT_SEND_USER_KICK
   //}}
	//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC_NOPARAM( ERM_UPDATE_HENIR_REWARD_USER_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	_DECL_ON_FUNC( ERM_BAD_ATTITUDE_USER_CHECK_INFO_NOT, KEGS_BAD_ATTITUDE_USER_CHECK_INFO_NOT );
	_DECL_ON_FUNC( ERM_DEFENCE_DUNGEON_WAVE_ID_NOT, KEGS_DEFENCE_DUNGEON_WAVE_ID_NOT );
	_DECL_ON_FUNC( ERM_FORCED_EXIT_VOTE_REQ, KEGS_FORCED_EXIT_VOTE_REQ );
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	DECL_ON_FUNC_NOPARAM( ERM_SYNC_DUNGEON_TIMER_NOT );
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}

	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	DECL_ON_FUNC( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	_DECL_ON_FUNC( ERM_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT, KEGS_ENTER_DEFENCE_DUNGEON_POPUP_REPLY_NOT );
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}

#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    //{{ 2013.09.03 하형진  던전 룸에 배틀필드 패킷 전송시 무시
	DECL_ON_FUNC_NOPARAM( ERM_BATTLE_FIELD_ZOMBIE_ALERT_NOT );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	_DECL_ON_FUNC( ERM_DUNGEON_SUB_STAGE_CLEAR_REQ, KEGS_DUNGEON_SUB_STAGE_CLEAR_REQ );
	_DECL_ON_FUNC( ERM_SECRET_STAGE_LOAD_REQ, KEGS_SECRET_STAGE_LOAD_REQ );
#endif // SERV_STAGE_CLEAR_IN_SERVER

protected:
	void			SetStageID( IN int iStageID )		{ m_iStageID = iStageID; }
	int				GetStageID()						{ return m_iStageID; }
	void			SetSubStageID( IN int iSubStageID ) { m_iSubStageID = iSubStageID; }
	int				GetSubStageID()						{ return m_iSubStageID; }
	//{{ 2012. 11. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CXSLDungeon::DUNGEON_TYPE GetDungeonType() const	{ return m_eDungeonType; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	int				GetHighestUserLevelAtDungeonStart() const { return m_iHighestUserLevelAtDungeonStart; }
	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	int				GetAverageUserLevelAtDungeonStart() const { return m_iAverageUserLevelAtDungeonStart; }
#endif SERV_DUNGEON_NPC_LEVEL
	//}}

	KDropItemData	CreateItemProcess( IN int iItemID, IN bool bIsGP = false, IN int iED = 0 );	
	void			CheckLoadingSuccess();			//한명의 유저가 끈길때 로딩처리를 위해서
	void			CheckDungeonUnitInfoSuccess();	//결과창으로 넘어가기 바로전에 유저가 팅길경우가 발생하여 여러곳에서 사용하기위해
	//{{ 2009. 7. 27  최육사	헤니르 시공
	void			CheckEnterSecretStageSuccess();
	//}}
	//{{ 2010. 8. 16	최육사	서버 코드 정리
	bool			DungeonSubStageStartProcess( IN int iSubStageID );
	//}}

	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
    void			NormalDungeonAttribMonsterDrop( IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
	void			SecretDungeonExtraStageAttribMonsterDrop( IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_SECRET_HELL
	//}}
	//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
#ifdef SERV_ACTIVE_EVENT_MONSTER
	bool			EventDropMonster( IN int iNpcID, IN bool bAllyNpc, IN int iStageID, IN bool bActiveNpc, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#else
	bool			EventDropMonster( IN int iNpcID, IN bool bAllyNpc, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_ACTIVE_EVENT_MONSTER
#else
	bool			EventDropMonster( IN int iNpcID, IN int iStageID, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_ALLY_EVENT_MONSTER
	//}}
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			SecretDungeonExtraStageMonster( IN const int iStageID, IN const short sPartyFever, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#else
	void			SecretDungeonExtraStageMonster( IN int iStageID, IN char cPartyFever, IN OUT KEGS_DUNGEON_STAGE_LOAD_NOT& kNot );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	bool			IsAttribNpc( IN int iNpcUID, OUT int& iNpcLv );

	//{{ 2009. 5. 11  최육사	npc죽은상태
	bool			IsDropDieState( IN char cState );
	//}}
	//{{ 2009. 7. 26  최육사	첫번째 서브 스테이지
	bool			IsFirstSubStage() { return ( m_iStageID == 0  &&  m_iSubStageID == 0 ); }
	//}}
	//{{ 2010. 05. 02  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	bool			IsEnteredExtraStage()	{ return ( m_iExtraStageLoadCount > 0 ); }	
#endif SERV_SECRET_HELL
	//}}
	//{{ 2009. 12. 14  최육사	헤니르임시인벤
	void			CheckTempInventoryClear();
	//}}
	//{{ 2010. 01. 05  최육사	내구도개편
	short			GetSubStageExpNpcNum( IN int iSubStageID ) const;
	//}}
#ifdef SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE// 작업날짜: 2013-09-04	// 박세훈
	void			ComputeTotalPlayTime( void );
#endif // SERV_FIX_DUNGEON_TOTAL_PLAY_TIME_AT_LEAVE
	//{{ 2010. 8. 16	최육사	서버 코드 정리
	double			GetTotalPlayTime() const	{ return m_fTotalPlayTime; }
	double			GetLoadingTime() const		{ return m_fLoadingTime; }
	//}}
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			IncreasePartyFever();
	void			MaxPartyFever();
	bool			IsStartedByAutoParty() const		{ return m_bStartedByAutoParty; }
	virtual void	ReservedRegroupOldParty();
	virtual void	CheckReserveRegroupOldParty();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	int				GetProtectedSmallCrystalCount();
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	void			CheckDeleteHenirDropItem( IN OUT KDropTable::DROP_DATA& sDropDataNpc, 
						  					IN OUT KDropTable::DROP_DATA& sDropDataStatic, 
											IN OUT KDropTable::DROP_DATA& sDropDataEvent, 
											IN OUT KDropTable::DROP_DATA& sDropDataAttribNpc,
											IN OUT KDropTable::DROP_DATA& sDropDataHenir );
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	bool			GetHackingNpcUid()								{ return m_bHackingNpcUid; }
	void			SetHackingNpcUid( IN bool bHacking )			{ m_bHackingNpcUid = bHacking; }
#endif SERV_MAKE_MONSTER_UID_CHANGE

	//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void			OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 07. 04	최육사		대기열 시스템
#ifdef SERV_AUTO_RESURRECTION_BUG_FIX
	void			HenirAutoResurrection( IN const UidType iUnitUID );
#endif SERV_AUTO_RESURRECTION_BUG_FIX
	//}}
	//{{ 2012. 02. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool			AutoPartyBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	bool			PlayWithCharBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}
#ifdef SERV_BUFF_BONUSRATE_HAMEL_EVENT
	void			PlayHamelEventBuff(IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_BUFF_BONUSRATE_HAMEL_EVENT

#ifdef SERV_CRAYON_POP_EVENT_BUFF// 작업날짜: 2013-06-10	// 박세훈
	void			CrayonPopEventBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList ) const;
#endif // SERV_CRAYON_POP_EVENT_BUFF

#ifdef SERV_BONUS_BUFF_SYSTEM
	bool			PlayWithBonusBuff( IN OUT std::map< UidType, std::vector< KBuffInfo > >& mapActivateBuffList );
#endif SERV_BONUS_BUFF_SYSTEM

	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	virtual void	TimeCountFinishProcess( IN const u_short usEventID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	

	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	float			GetDungeonPartyBonusRate();
#endif SERV_REFORM_ITEM_DROP
	//}}
	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	void			PrepareForEnterDefenceDungeon();
	//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	void			EnterDefenceDungeon( IN const std::vector< UidType >& vecLeaveUnitUID );
#else
	//void			EnterDefenceDungeon();
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}	
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

protected:
	KEGS_DUNGEON_SUB_STAGE_GO_NEXT_ALL_NOT	m_kDSSGoNextAllNot;

	//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
	int							m_iLoadingCompleteStageID;
#endif SERV_DUNGEON_STAGE_LOG
	//}}
	int							m_iStageID;
	int							m_iSubStageID;
	double						m_fTotalPlayTime; // 던전&스테이지 로딩 시간을 뺀 실제 플레이 시간입니다.
	double						m_fLoadingTime;
	bool						m_bIsWin;
	
	int							m_nDropItemUID;	// Drop item UID 발급	
	//{{ 2011. 10. 25	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	int							m_nNPCUID;		// NPC Unit UID 발급
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	u_short						m_usLoadingState;	//로딩상태 체크

	//{{ 2010. 07. 16  최육사	이벤트 몬스터 시스템
#ifdef SERV_EVENT_MONSTER
	//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	std::vector< KEventMonsterRefreshManager::KAdventNpcEventInfo > m_vecEventMonster;
#else
	std::vector< KGameEventScriptManager::KAdventNpcEventInfo > m_vecEventMonster;
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
	//}}
#endif SERV_EVENT_MONSTER
	//}}

	std::map< int, ITEM_DATA >	m_mapDropItem;			 // 임시로 발급한 item uid를 습득했는지에 대한 체크	[key:DropUID,	 value:ITEM_DATA]	
	//{{ 2011. 10. 25	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
#else
	//std::map< int, NPC_DATA >	m_mapNPCData;			 // 현재 생성된 NPC
	//std::map< KSubStageKey, SUB_STAGE_NPC_INFO > m_mapSubStageNpcInfo; // 서브스테이지별로 존재하는 npc uid정보
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	std::map< int, int >		m_mapSubStageExpNpcNum;	 // 서브스테이지별 exp npc수						[key:SubStageID, value:NpcCount]
	//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT
	std::map< int, int >		m_mapTimeDropEventMonster; // 특정 시각에 출현하는 이벤트 몬스터			[key:DropUID, value:EventUID]
#endif SERV_TIME_DROP_MONSTER_EVENT
	//}}

	LOADING_TIMER				m_LoadingTimer;

	//{{ 2008. 10. 20  최육사	속성 몬스터 카운트
	std::map< int, int >		m_mapAttribNpcData;		// 1. npcUID    2. npcLv
	int							m_iAttribNpcStageCnt;
	int							m_iAttribNpcDropCnt;
	//}}

	//{{ 2010. 04. 07  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	int							m_iExtraStageLoadCount;
	bool						m_bAdventExtraNpc;	
	float						m_fPoisonTimeLimit;
#endif SERV_SECRET_HELL
	//}}

	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장
#ifdef SERV_PC_BANG_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
	float						m_fItemDropEventProbRate;
#else // SERV_DROP_EVENT_RENEWAL
	int							m_iItemDropEventProbCount;
#endif // SERV_DROP_EVENT_RENEWAL
	bool						m_bWithPlayPcBangEvent;
#endif SERV_PC_BANG_DROP_EVENT
	//}}
	
	int							m_iHighestUserLevelAtDungeonStart;	// 던전 시작시에 유저중에서 최고 레벨
	//{{ 2012. 05. 10	최육사	던전 몬스터 레벨 지정
#ifdef SERV_DUNGEON_NPC_LEVEL
	int							m_iAverageUserLevelAtDungeonStart;	// 던전 시작시에 유저중에서 평균 레벨
#endif SERV_DUNGEON_NPC_LEVEL
	//}}
	CXSLDungeon::DUNGEON_TYPE	m_eDungeonType;						// 헤니르 시공

	UidType						m_iPartyUID;

	//{{ 2010. 06. 17  최육사	오토핵 방지
#ifdef SERV_PROTECT_AUTO_HACK
	int							m_iHackUserCheckCount;
	int							m_iNpcDieHackCount;
	int							m_iGetItemHackCount;
	int							m_iStartedNumMember;
#endif SERV_PROTECT_AUTO_HACK
	//}}	
	//{{ 2010. 7. 28  최육사	오토핵 차단 NPC
#ifdef SERV_AUTO_HACK_CHECK_NPC
	std::set< UidType >			m_setFindedHackUserUnitUID;
#endif SERV_AUTO_HACK_CHECK_NPC
	//}}
	//{{ 2010. 11. 18	최육사	오토 핵 스테이지 로딩 체크
#ifdef SERV_AUTO_HACK_CHECK_STAGE_LOAD
	std::map< int, int >		m_mapStageLoadingCheck;
	UidType						m_iLastLoadingUnitUID;
#endif SERV_AUTO_HACK_CHECK_STAGE_LOAD
	//}}
	//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KDungeonMonsterManager		m_kDungeonMonsterManager;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2011. 10. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KDungeonSecurityManager		m_kDungeonSecurityManager;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
	int							m_iDefenseMainCrystalNpcUID;			// 큰 수정 NPCUID
	std::set< int >				m_setDefenseSmallCrystalNpcUID;			// 작은 수정 NPCUID
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가


	//{{ 2011. 09. 07  김민성	서브 스테이지 DIE NPC 시간 체크 기능
#ifdef SERV_SUB_STAGE_NPC_DIE_CHECK
	std::map< int, int >		m_mapSubStageActiveNpcNum;		// 서브스테이지별 Active npc수						[key:SubStageID, value:NpcCount]
	int							m_iSubStageActiveNpc;			// 현재 서브 스테이지의 액티브 몬스터 수
	int							m_iSubStageKillActiveNpc;		// 서브 스테이지에서 잡은 몬스터 수
	CTime						m_tSubStageFirstKillTime;		// 서브 스테이지에서 처음 몬스터 잡은 시간
#endif SERV_SUB_STAGE_NPC_DIE_CHECK
	//}}
	//{{ 2012. 03. 15	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool						m_bStartedByAutoParty;			// 자동 파티에 의한 시작인지 여부
	std::vector< KERM_END_GAME_REGROUP_PARTY_NOT > m_vecReservedRegroupOldParty;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2012. 02. 06    김민성    던전 몬스터 UID 발급 방식 변경
#ifdef SERV_MAKE_MONSTER_UID_CHANGE
	bool						m_bHackingNpcUid;				// 발급되지 않는 NPC UID 를 클라이언트가 날리는지 검사 - 초기값 false
#endif SERV_MAKE_MONSTER_UID_CHANGE	
	//}}

	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	KBadAttitudeManager			m_kBadAttitudeManager;
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}
	//{{ 2013. 02. 01  이벤트 발렌타인 던전 - 김민성
#ifdef SERV_EVENT_VALENTINE_DUNGEON
	KValentineManager			m_kValentineManager;			// 발렌타인 관련 매니저(이벤트 던전)
#endif SERV_EVENT_VALENTINE_DUNGEON
	//}

	//{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
	float m_fQuestItemDropEventProbRate;;
#endif SERV_QUEST_ITEM_DROP_EVENT
	//}
	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	KDefenceDungeonManager		m_kDefenceDungeonManager;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// 작업날짜: 2013-05-15	// 박세훈
	std::vector<DungonStageLoadLog>		m_vecDungonStageLoadLog;
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_CREATED_NPC_LIMITED_DROPS
	struct NPC_INFO_QUADRUPLE
	{
		bool operator < ( const NPC_INFO_QUADRUPLE& right ) const
		{
			if( iDungeonID == right.iDungeonID )
			{
				if( iStageID == right.iStageID )
				{
					if( iSubStageID == right.iSubStageID )
					{
						if( iNpcID < right.iNpcID )
							return true;
					}
					else if( iSubStageID < right.iSubStageID )
					{
						return true;
					}
				}
				else if( iStageID < right.iStageID )
				{
					return true;
				}
			}
			else if( iDungeonID < right.iDungeonID )
			{
				return true;
			}

			return false;
		}

		int iDungeonID;
		int iStageID;
		int iSubStageID;
		int iNpcID;
	};
	std::map< NPC_INFO_QUADRUPLE, int > m_mapDroppedTimes;
#endif SERV_CREATED_NPC_LIMITED_DROPS

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    int m_iSubStageCount;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST

#ifdef SERV_STAGE_CLEAR_IN_SERVER// 작업날짜: 2013-10-30	// 박세훈
	CXSLDungeonSubStage::NextStageData	m_kNextStageData;
	bool								m_bFirstStage;
	bool								m_bFirstSubStage;
	int									m_iClearConditionIndex;
#endif // SERV_STAGE_CLEAR_IN_SERVER
};


