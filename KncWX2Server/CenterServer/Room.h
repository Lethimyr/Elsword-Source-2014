#pragma once

#include "CnBaseRoom.h"

#include "Enum/Enum.h"
#include "Socket/NetCommon.h"

#include "RoomUserManager.h"
#include <boost/timer.hpp>

SmartPointer( KRoom );

#define VERIFY_HOST( eventid ) \
if( !IsHost( FIRST_SENDER_UID ) ) \
{ \
    START_LOG( cerr, L"방장이 아닌데 요청함." ) \
    << BUILD_LOG( FIRST_SENDER_UID ) \
    << END_LOG; \
    kPacket.m_iOK = NetError::ERR_ROOM_08; \
    SendToGSCharacter( LAST_SENDER_UID, FIRST_SENDER_UID, eventid, kPacket ); \
    return; \
} \


class KRoom : public KCnBaseRoom
{
    NiDeclareRTTI;
    DeclToStringW;

public:
	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	enum ROOM_ENUM
	{
		RE_TIME_COUNT_LIMIT = 3,
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 07. 09  최육사	드롭률 이벤트 확장
	enum ROOM_EVENT_ENUM
	{
		REE_DROP_EVENT_PROB_DEFAULT = 1,
	};
	//}}

    enum ENUM_TIMER
    {
        TM_WAIT,
        TM_PLAY_TIME_NOT,				
        TM_PLAY,
		//{{ 2011. 07. 21	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		TM_PLAY_WITHOUT_LOADING,
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		TM_TIME_COUNT,
		TM_RETURN_TO_FIELD,
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
        TM_DELAY_PACKET,
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		TM_CHECK_CHANGE_HOST,
#endif SERV_CHOOSE_FASTEST_HOST
		//}
		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		TM_WAIT_FOR_DEFENCE,
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
        TM_TOTAL_NUM,
    };

	//로딩시간을 체크하기 위한..
	class LOADING_TIMER
	{
	public:
		LOADING_TIMER(){ m_bIsStart = false; }
		~LOADING_TIMER(){}
		void	Start()
		{
			if( m_bIsStart == false )
			{
				m_bIsStart = true;
				m_kTimer.restart();
			}
		}
		double	End()
		{
			if( m_bIsStart == true )
			{
				m_bIsStart = false;
				return m_kTimer.elapsed();
			}
			return 0.0;
		}
		bool	IsStart() { return m_bIsStart; }

	private:
		bool			m_bIsStart;
		boost::timer	m_kTimer;
	};

public:
    KRoom();
    virtual ~KRoom();

    // override.
    virtual void Tick();

    int  GetNumTotalSlot()              { return m_spRoomUserManager->GetNumTotalSlot(); }
    int  GetNumOpenSlot()               { return m_spRoomUserManager->GetNumOpenedSlot(); }
    int  GetNumOccupiedSlot()           { return m_spRoomUserManager->GetNumOccupiedSlot(); }
    int  GetNumMember()                 { return m_spRoomUserManager->GetNumMember(); }

protected:
	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	virtual	int					GetPVPChannelClass( void ) const {	return KPVPChannelInfo::PCC_NONE;	}
	virtual	SEnum::MATCH_TYPE	GetMatchType( void ) const {	return SEnum::MT_NONE; }
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

	virtual void	SetRoomInfo( const KRoomInfo& kInfo ) = 0;
    virtual void	ProcessEvent( const KEventPtr& spEvent );    // derived from KPerformer
    virtual void	ProcessNativeEvent( const KEventPtr& spEvent ) = 0;
    virtual void	StartGame();
    virtual void	StartPlay();
    virtual void	EndPlay();
    virtual void	EndGame();
    virtual bool	CheckIfPlayEnd();
    virtual bool	IsAbleToIntrude();
    virtual int		DecideWinTeam() = 0;
    virtual int		GetPlayMode() = 0;
    virtual void	HandleTimeOutUser( std::map< UidType, UidType >& mapUserCIDNGS ) = 0;
	virtual void	DeleteZombie( std::vector<UidType>& vecZombieUnit );	//추후 pvp/dungeon 방별 개별처리가 필요할경우 재작성을 위해
	virtual void	CheckResultSuccess();
	virtual float	GetRemainingPlayTime(){ return m_fRemainingPlayTime; }	//Ex.pvp게임일경우 게임남은 시간

	virtual void	GetRoomInfo( OUT KRoomInfo& kInfo );
	float	GetPlayTimeLimit()											{ return m_fPlayTimeLimit; }
	void	SetPlayTimeLimit( float fPlayTimeLimit )					{ m_fPlayTimeLimit = fPlayTimeLimit; }
	void	SetRemainingPlayTime( float fRemainingPlayTime )			{ m_fRemainingPlayTime = fRemainingPlayTime; }
	void	GetRoomSlotInfo( std::vector< KRoomSlotInfo >& vecSlot, KRoomUserManager::USERLIST_TYPE eType = KRoomUserManager::UT_GAME )	{ m_spRoomUserManager->GetRoomSlotInfo( vecSlot, eType ); }

	//{{ 2011. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetRoomUserManager( IN KRoomUserManagerPtr spRoomUserManager )	{ m_spRoomUserManager = spRoomUserManager; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	KRoomUserManagerPtr GetRoomUserManager()								{ return m_spRoomUserManager; }

    void	CheckRemainingPlayTime();
    void	CheckGameLoadingTimeOut();
	void	EnableRemainingPlaytime( bool bEnable );
	//{{ 2011. 08. 05	최육사	대전방에 유저 혼자 남게되는 버그 수정
#ifdef SERV_CODE_REFACTORING_20110804
	bool	CheckAllPlayerFinishLoading();
#endif SERV_CODE_REFACTORING_20110804
	//}}

	bool	IsEmpty( KRoomUserManager::USERLIST_TYPE eType = KRoomUserManager::UT_GAME );
    bool	IsFull();
    bool	IsHost( UidType nCID );
	//{{ 2011. 07. 22	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM	
	virtual	bool IsOfficialMatch() const								{ return false; }
#endif SERV_PVP_NEW_SYSTEM
	//}}

    template< class T > void BroadCast( u_short usEventID, const T& data, int iOption = KRoomUserManager::UL_ALL );
    void	BroadCastID( u_short usEventID, int iOption = KRoomUserManager::UL_ALL );
    void	BroadCastRoomSlotInfo();
    void	BroadCastRoomSlotInfo( u_short usEventID );

    virtual	void SendRoomListInfo( IN const int iCode );
	//{{ 2011. 07. 26	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
	virtual void SendPVPUserInfo( UidType iUnitUID, char cState = KCommunityUserInfo::US_NONE ) {}
	virtual void SendAllPVPUserInfo() {}
//#endif SERV_PVP_NEW_SYSTEM
	//}}

	//{{ 2010. 03. 24  최육사	비밀던전 헬모드
	int		GetDungeonID()												{ return m_iDungeonID; }
	char	GetDifficultyLevel()										{ return m_cDifficultyLevel; }
	int		GetDungeonIDAndDif()										{ return ( m_iDungeonID + static_cast<int>(m_cDifficultyLevel) ); }
	//}}

	//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
	void	CheckCaughtBuffSkill();
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}

	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	TimeCountForStartGame( IN const u_short usEventID );
	void	CheckTimeCount();
	virtual void TimeCountFinishProcess( const u_short usEventID ) = 0;
	char	GetCurrentTimeCount() const									{ return m_cCurrentTimeCount; }
	void	SetCurrentTimeCount( IN const char cCurrentTimeCount )		{ m_cCurrentTimeCount = cCurrentTimeCount; }
	void	DecreaseCurrentTimeCount()									{ --m_cCurrentTimeCount; }

	virtual void ReservedRegroupOldParty()								{ /*Nothing*/ }
	virtual void CheckReserveRegroupOldParty()							{ /*Nothing*/ }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	void	CheckPingScore();
#endif SERV_CHOOSE_FASTEST_HOST
	//}
#ifdef SERV_PVP_REMATCH
	virtual void	Rematch() {}
	virtual bool	CanRematch() { return false; }
#endif SERV_PVP_REMATCH
	
	//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	virtual	void	LogToDB_GamePlayNetWorkInfo( IN const UidType iUnitUID, IN const KGamePlayNetworkInfoPackage& kInfo, IN const bool bLeaveRoom ) = 0;
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

	//{{ 2013. 2. 7	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	bool	IsLanBugCheckUser( const UidType iUnitUID ) const	{	return m_setRanBugCheckList.find( iUnitUID ) != m_setRanBugCheckList.end();	}
	void	AddLanBugCheckUser( const UidType iUnitUID ){	m_setRanBugCheckList.insert( iUnitUID );	}
	void	RemoveLanBugCheckUser( const UidType iUnitUID ){	m_setRanBugCheckList.erase( iUnitUID );	}
	void	ClearLanBugCheckUser( void ){	m_setRanBugCheckList.clear();	}

	bool	IsLanBugVerifyUser( const UidType iUnitUID ) const	{	return m_setRanBugVerifyList.find( iUnitUID ) != m_setRanBugVerifyList.end();	}
	void	AddLanBugVerifyUser( const UidType iUnitUID ){	m_setRanBugVerifyList.insert( iUnitUID );	}
	void	RemoveLanBugVerifyUser( const UidType iUnitUID ){	m_setRanBugVerifyList.erase( iUnitUID );	}
	void	ClearLanBugVerifyUser( void ){	m_setRanBugVerifyList.clear();	}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

	//{{ 2013. 2. 7	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	virtual	void	OnLeaveRoom( IN const UidType iLAST_SENDER_UID, IN const UidType iFIRST_SENDER_UID, IN const KERM_LEAVE_ROOM_REQ& kPacket_, IN const u_short usEventIDAck );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// 적용날짜: 2013-05-03
	bool	HostCheck( void ) const;
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ


	template < class T > void BroadCastUpdateUnitInfo( std::vector<KRoomUserPtr>& vecKRoomUserPtr, unsigned short usEventID, const T& data );

    // event handler
    DECL_ON_FUNC( ERM_JOIN_ROOM_REQ );
   _DECL_ON_FUNC( ERM_BAN_USER_REQ, KEGS_BAN_USER_REQ );
   _DECL_ON_FUNC( ERM_CHANGE_READY_REQ,	KEGS_CHANGE_READY_REQ );
   _DECL_ON_FUNC( ERM_CHANGE_PITIN_REQ,	KEGS_CHANGE_PITIN_REQ );
   _DECL_ON_FUNC( ERM_CHANGE_SLOT_OPEN_REQ, KEGS_CHANGE_SLOT_OPEN_REQ );
   _DECL_ON_FUNC( ERM_GAME_LOADING_REQ, KEGS_GAME_LOADING_REQ );
   _DECL_ON_FUNC( ERM_STATE_CHANGE_RESULT_REQ, KEGS_STATE_CHANGE_RESULT_REQ );
   _DECL_ON_FUNC( ERM_CHAT_REQ, KEGS_CHAT_REQ );
   _DECL_ON_FUNC( ERM_CHANGE_MOTION_REQ, KEGS_CHANGE_MOTION_REQ );
   _DECL_ON_FUNC( ERM_CHANGE_PLAY_TIME_LIMIT_REQ, KEGS_CHANGE_PLAY_TIME_LIMIT_REQ );

   _DECL_ON_FUNC( ERM_SET_UNIT_INFO_NOT, KRoomUserInfo );
    DECL_ON_FUNC_NOPARAM( ERM_REFRESH_ROOM_REQ );
    DECL_ON_FUNC( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT );
	//{{ 2009. 5. 21  최육사	내구도 소모 개편
	DECL_ON_FUNC( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT );
	//}}
	//{{ 2012. 10. 17	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO   	
   _DECL_ON_FUNC( ERM_RESULT_SUCCESS_REQ, KEGS_RESULT_SUCCESS_REQ );
#else
	DECL_ON_FUNC_NOPARAM( ERM_RESULT_SUCCESS_REQ );
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}

   _DECL_ON_FUNC( ERM_ADD_ON_STAT_REQ, KEGS_ADD_ON_STAT_REQ );

   _DECL_ON_FUNC( ERM_REPAIR_ITEM_NOT, KEGS_REPAIR_ITEM_NOT );

    DECL_ON_FUNC( ERM_REQUEST_TRADE_NOT );
    DECL_ON_FUNC( ERM_REQUEST_TRADE_REPLY_NOT );

    DECL_ON_FUNC_NOPARAM( ERM_TRADE_START_NOT );
    DECL_ON_FUNC_NOPARAM( ERM_TRADE_FINISH_NOT );

	DECL_ON_FUNC( ERM_SET_QUEST_ITEM_INFO_NOT );

	//{{ 2008. 4. 2  최육사  근성도 회복 아이템
   _DECL_ON_FUNC( ERM_RESTORE_SPIRIT_NOT, KEGS_RESTORE_SPIRIT_NOT );
	//}}

	//{{ 2009. 5. 8  최육사		실시간 레벨업
	DECL_ON_FUNC( ERM_CHAR_LEVEL_UP_NOT );
	//}}
	//{{ 2009. 9. 23  최육사	1차전직캐쉬템
   _DECL_ON_FUNC( ERM_CHANGE_ROOM_USER_INFO_NOT, KRoomUserInfo );
	//}}

	//{{ 2009. 11. 30  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	DECL_ON_FUNC( ERM_CHANGE_SKILL_INFO_IN_ROOM_NOT );
#endif GUILD_SKILL_TEST
   //}}

#ifdef DUNGEON_ITEM
	DECL_ON_FUNC_NOPARAM( ERM_CHANGE_PVP_ITEMMODE_REQ );
#endif DUNGEON_ITEM

	//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
   _DECL_ON_FUNC( ERM_SEARCH_UNIT_REQ, KELG_SEARCH_UNIT_REQ );
   _DECL_ON_FUNC( ERM_GET_UNIT_INFO_ACK, KELG_GET_UNIT_INFO_ACK );
#endif SERV_INTEGRATION
	//}}

	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	DECL_ON_FUNC( ERM_SUMMON_PET_NOT );
	DECL_ON_FUNC( ERM_PET_ACTION_NOT );
   _DECL_ON_FUNC( ERM_CHANGED_PET_SATIETY_NOT, KEGS_CHANGED_PET_SATIETY_NOT );
   _DECL_ON_FUNC( ERM_CHANGED_PET_INTIMACY_NOT, KEGS_CHANGED_PET_INTIMACY_NOT );
   _DECL_ON_FUNC( ERM_CHANGED_PET_SATIETY_INTIMACY_NOT, KEGS_CHANGED_PET_SATIETY_INTIMACY_NOT );
	DECL_ON_FUNC( ERM_PET_EVOLUTION_NOT );
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
	_DECL_ON_FUNC( ERM_BUFF_SKILL_INFO_CONVEY_REQ, KEGS_BUFF_SKILL_INFO_CONVEY_REQ );
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}

	//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	_DECL_ON_FUNC( ERM_UPDATE_BUFF_INFO_IN_ROOM_NOT, KEGS_UPDATE_BUFF_INFO_IN_ROOM_NOT );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}

	//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	_DECL_ON_FUNC( ERM_USE_SKILL_BUFF_IN_PLAY_NOT, KEGS_USE_SKILL_BUFF_IN_PLAY_NOT );
	_DECL_ON_FUNC( ERM_NEXT_RETURN_TO_FIELD_NOT, KERM_COMPLETE_RETURN_TO_FIELD_NOT );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	_DECL_ON_FUNC( ERM_UPDATE_USER_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_USER_UNIT_BUFF_INFO_NOT );
	_DECL_ON_FUNC( ERM_UPDATE_NPC_UNIT_BUFF_INFO_NOT, KEGS_UPDATE_NPC_UNIT_BUFF_INFO_NOT );
#endif SERV_SERVER_BUFF_SYSTEM
	//}}
	//{{ 2012. 07. 27	최육사		사제시스템 EXP 보너스 버그 수정
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
	DECL_ON_FUNC( ERM_UPDATE_TUTORIAL_INFO_NOT );
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
	//}}
	//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
	_DECL_ON_FUNC( ERM_VISIT_CASH_SHOP_NOT, KEGS_VISIT_CASH_SHOP_NOT );
#endif SERV_VISIT_CASH_SHOP
	//}}
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	_DECL_ON_FUNC( ERM_UDP_PING_SCORE_NOT, KEGS_UDP_PING_SCORE_NOT );
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
	_DECL_ON_FUNC( ERM_FIELD_USER_FRAME_INFO_NOT, int );
#endif SERV_FIELD_FRAME_LOG
	//}}

	//{{ 2013. 04. 09	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	_DECL_ON_FUNC( ERM_CHANGE_GAME_STAT_INFO_IN_ROOM_NOT, KRoomUserInfo );
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-08
	DECL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_REQ );
	DECL_ON_FUNC( ERM_LOCAL_RANKING_WATCH_UNIT_ACK );
#endif	// SERV_LOCAL_RANKING_SYSTEM
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	_DECL_ON_FUNC( ERM_COUPLE_MAKING_SUCCESS_NOT, KERM_COUPLE_MAKING_SUCCESS_NOT );
	DECL_ON_FUNC_NOPARAM( ERM_BREAK_UP_SUCCESS_NOT );
	_DECL_ON_FUNC( ERM_RELATIONSHIP_EFFECT_NOT, KEGS_RELATIONSHIP_EFFECT_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
	//}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	_DECL_ON_FUNC( ERM_SUMMON_RIDING_PET_NOT, KEGS_SUMMON_RIDING_PET_NOT );
	_DECL_ON_FUNC( ERM_UNSUMMON_RIDING_PET_NOT, KEGS_UNSUMMON_RIDING_PET_NOT );
#endif	// SERV_RIDING_PET_SYSTM

	//{{ 2013. 2. 1	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	_DECL_ON_FUNC( ERM_FORCE_RELAY_NOT, KEGS_FORCE_RELAY_NOT );
	_DECL_ON_FUNC( ERM_ADD_UDP_CHECK_KICK_USER_NOT, UidType );
	_DECL_ON_FUNC( ERM_REMOVE_LANBUG_CHECK_USER_NOT, UidType );
	_DECL_ON_FUNC( ERM_REMOVE_LANBUG_VERIFY_USER_NOT, UidType );
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

private:

    // template이 너무 길어지는 것을 막는다. (현재 template 남용이 심각한 편 ㅜ.ㅜ..)
    void _BroadCast( int iFlag, const KEvent& kEvent );	

protected:
    char					            m_cRoomType;
    UINT								m_uiRoomListID;


    bool					            m_bPublic;
    bool								m_bTeamBalance;
	bool								m_bCanIntrude;
    std::wstring                        m_wstrPassword;

    char					            m_cGameType;
    char					            m_cWinningNumKill;
    float                               m_fPlayTimeLimit;
    short					            m_sWorldID;
	//{{ 2008. 5. 27  최육사  랜덤맵
	short								m_sShowPvpMapWorldID;
	//}}

    char				                m_cDifficultyLevel;
    int					                m_iDungeonID;
    
    float                               m_fRemainingPlayTime;
    static float                        ms_fPlayTimeNotifyGap;
	bool								m_bEnableRemainingPlayTime;

    boost::timer                        m_kTimer[TM_TOTAL_NUM];
	//{{ 2011. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KRoomUserManagerPtr					m_spRoomUserManager;
#else
	KRoomUserManager                    m_kRoomUserManager;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}    
    bool								m_bEndPlay;

	//아이템 습득방식(random & person)
	char								m_cGetItemType;
	char 								m_cDungeonMode;			// CXSLDungeon::DUNGEON_MODE

#ifdef DUNGEON_ITEM
	bool								m_bIsItemMode;
#endif DUNGEON_ITEM

	//{{ 2012. 05. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	u_short								m_usEventIDForTimeCount;
	char								m_cCurrentTimeCount;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#ifdef SERV_PVP_REMATCH
	std::map< short, int >				m_mapAllPlayersSelectedMap;
#endif SERV_PVP_REMATCH

	//{{ 2013. 2. 7	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	std::set<UidType>	m_setRanBugCheckList;
	std::set<UidType>	m_setRanBugVerifyList;
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

#ifdef  SERV_CHOOSE_FASTEST_HOST
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
    bool    m_bCheckPingScoreExecuted;
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
#endif  SERV_CHOOSE_FASTEST_HOST


#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
private:
	//bool m_bForceHost;
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

private:
#ifdef	SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ// 적용날짜: 2013-05-03
	boost::timer						m_kHostCheckTimer;
#endif	// SERV_CHECK_HOST_FOR_ERM_NPC_UNIT_DIE_REQ


};

template< class T >
void KRoom::BroadCast( u_short usEventID, const T& data, int iOption /*= BC_ALL */)
{
    KEvent kEvent;  // _BroadCast에서 clone할 것이므로 여기선 heap 아니어도 됨.
    kEvent.m_kDestPerformer.m_dwPerformerID = PI_GS_CHARACTER;
    kEvent.m_usEventID = usEventID;

    KSerializer ks;
    kEvent.m_kbuff.Clear();

    // serialize - only data
    ks.BeginWriting( &kEvent.m_kbuff );
    ks.Put( data );
    ks.EndWriting();

    _BroadCast( iOption, kEvent );
}

template< class T >
void KRoom::BroadCastUpdateUnitInfo( std::vector< KRoomUserPtr >& vecRoomUser, unsigned short usEventID, const T& data )
{
	for( u_int ui = 0; ui < vecRoomUser.size(); ++ui )
	{
		SendToGSCharacter( vecRoomUser[ui]->GetGSUID(), vecRoomUser[ui]->GetCID(), usEventID, data[ui] );
	}
}
