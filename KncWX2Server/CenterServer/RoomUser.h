#pragma once

#include "FSM/support_FSM.h"
#include <boost/timer.hpp>

#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "X2Data/XSLUnit.h"
#include "X2Data/XSLRoom.h"

#define LOADING_TIME_OUT	60.0

#define KRoomUserPtr KRoomUser*
#undef KRoomUserPtr
SmartPointer( KRoomUser );

#pragma pack( push, 1 )

struct KRoomUserInfo;
class KRoomUser
{
    DeclareFSM_MT;

public:
	struct QUEST_ITEM_DROP_INFO
	{
		UidType				m_iGSUID;
		UidType				m_iUnitUID;

		std::vector< int >	m_vecDropQuestItem;

		QUEST_ITEM_DROP_INFO(){};
		QUEST_ITEM_DROP_INFO( const QUEST_ITEM_DROP_INFO& t ) { *this = t; }    // copy constructor
		QUEST_ITEM_DROP_INFO& operator=( const QUEST_ITEM_DROP_INFO& t )       // assign operator
		{
			m_iGSUID = t.m_iGSUID;
			m_iUnitUID = t.m_iUnitUID;
			m_vecDropQuestItem = t.m_vecDropQuestItem;

			return *this;
		}
	};

	//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
	struct BUFF_SKILL_INFO
	{
		UidType								m_iCastingUnitUID;		// 버프 시전자
		int									m_iBuffEnum;			// 버프 종류
		bool								m_bBuffState;			// 버프의 상태(on/off)
		boost::timer						m_tBuffTimer;			// 버프 시간 체크용 타이머
		float								m_fBuffTime;			// 버프 지속시간(남은 버프 유지시간)
		char								m_cBuffLevel;			// 버프 스킬 레벨
	};
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}

public:
    KRoomUser();
    virtual ~KRoomUser();

	void	GetRoomUserInfo( KRoomUserInfo& kInfo )             { kInfo = m_kRoomUserInfo; }
    void	SetRoomUserInfo( const KRoomUserInfo& kInfo )       { m_kRoomUserInfo = kInfo; }

    int		GetTeam()                                           { return m_iTeam; }
    void	SetTeam( int iTeam )                                { m_iTeam = iTeam; }

    UidType	GetGSUID()                                          { return m_kRoomUserInfo.m_iGSUID; }
	UidType GetUserUID()										{ return m_kRoomUserInfo.m_iOwnerUserUID; } 
    UidType	GetCID() const										{ return m_kRoomUserInfo.m_nUnitUID; }
	//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
	short	GetServerGroupIDThisUser() const					{ return m_kRoomUserInfo.m_sServerGroupID; }
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
	//}}

	//{{ 2009. 12. 4  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	int		GetGuildUID()										{ return m_kRoomUserInfo.m_iGuildUID; }
#endif GUILD_SKILL_TEST
	//}}
	//{{ 2011. 11. 2	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType GetPartyUID() const									{ return m_kRoomUserInfo.m_iPartyUID; }
	void	SetPartyUID( IN const UidType iPartyUID )			{ m_kRoomUserInfo.m_iPartyUID = iPartyUID; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

    bool	IsHost()                                            { return m_bHost; }
    void	SetHost( bool bHost )                               { bHost ? SetReady( true ) : SetReady( false ); m_bHost = bHost; }

    bool	IsReady()                                           { return ( IsHost() || m_bReady ) && !IsInTrade(); }
    bool	SetReady( bool bReady )                             { if( bReady && IsInTrade() ) return false; m_bReady = bReady; return true; }

	bool	IsPitIn()											{ return m_bPitIn; }
	void	SetPitIn( bool bPitIn )								{ m_bPitIn = bPitIn; }

    bool    IsInTrade()                                         { return m_bTrade; }
    void    SetTrade( bool bTrade )                             { if( bTrade ) SetReady( false ); m_bTrade = bTrade; }

    int		GetLoadingProgress()                                { return m_iLoadingProgress; }
    void	SetLoadingProgress( int iLoadingProgress )          { m_iLoadingProgress = iLoadingProgress; }

    bool	IsStageLoaded()                                     { return m_bStageLoaded; }
    void	SetStageLoaded( bool bStageLoaded )                 { m_bStageLoaded = bStageLoaded; }

    int		GetNumKill()                                        { return m_nKill; }
    void	IncreaseNumKill()                                   { m_nKill++; }

	int		GetNumMDKill()                                      { return m_nMDKill; }
	void	IncreaseNumMDKill()                                 { m_nMDKill++; }

    int		GetNumDie()                                         { return m_nDie; }
    void	IncreaseNumDie()                                    { m_nDie++; }

    bool	IsDie()                                             { return m_bDie; }
    void	SetDie( bool bDie )                                 { m_bDie = bDie; }

    float	GetHP()                                             { return m_fHP; }
    void	SetHP( float fHP )                                  { m_fHP = fHP; }

	//{{ 2011. 02. 21	최육사	던전 랭크 개편
//#ifdef SERV_DUNGEON_RANK_NEW
	int		GetBaseHP_LUA()										{ return m_kRoomUserInfo.m_kGameStat.m_iBaseHP; }
	int		GetPercentHP()                                      { return static_cast<int>(( m_fHP * 100.f ) / static_cast<float>(GetBaseHP_LUA())); }
//#else
//	int		GetBaseHP()											{ return m_kRoomUserInfo.m_kGameStat.m_iBaseHP; }
//	int		GetPercentHP()                                      { return static_cast<int>(( m_fHP * 100.f ) / static_cast<float>(GetBaseHP())); }
//#endif SERV_DUNGEON_RANK_NEW
	//}}

    int		GetSlotID()                                         { return m_iSlotID; }
    void	SetSlotID( int iSlotID )                            { m_iSlotID = iSlotID; }

	int		GetStageID()										{ return m_iNowStageID; }
	void	SetStageID( int iStageID )							{ m_iNowStageID = iStageID; }

	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	int		GetSubStageID() const								{ return m_iNowSubStageID; }
	void	SetSubStageID( IN const int iSubStageID )			{ m_iNowSubStageID = iSubStageID; }
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}

    int		GetRebirthPos()                                     { return m_iRebirthPos; }
    void	SetRebirthPos( int iRebirthPos )                    { m_iRebirthPos = iRebirthPos; }

	//{{ 2012. 10. 31	박세훈	Merge 랜선랙 방지 작업-릴레이를 기본적으로 사용한다.
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
	bool	IsOnlyPlaying();
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
	//}}

    bool	IsPlaying();	

	int		GetLevel()											{ return m_kRoomUserInfo.m_ucLevel; }
	char	GetAuthLevel()										{ return m_kRoomUserInfo.m_cAuthLevel; }


//#ifdef PVP_BOSS_COMBAT_TEST
//	bool GetIsBoss() const { return m_bIsBoss; }
//	void SetIsBoss(const bool val) { m_bIsBoss = val; }
//#endif PVP_BOSS_COMBAT_TEST

	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool	IsPrepareForDefence() const							{ return m_bPrepareForDefence; }
	void	SetPrepareForDefence( IN const bool bVal )			{ m_bPrepareForDefence = bVal; }
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	bool	IsRecvEnterPopupReply() const						{ return m_bRecvEnterPopupReply; }
	void	SetRecvEnterPopupReply( IN const bool bVal )		{ m_bRecvEnterPopupReply = bVal; }
	bool	IsEnterDefenceDungeon() const						{ return m_bEnterDefenceDungeon; }
	void	SetEnterDefenceDungeon( IN const bool bVal )		{ m_bEnterDefenceDungeon = bVal; }
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}

	//{{ 2008. 7. 29  최육사	아케이드
	bool	IsGuestUser()										{ return m_kRoomUserInfo.m_bIsGuestUser; }
	//}}
	//080618.hoons.아이템 & 이벤트를 통한 추가경험치 지급..
	//{{ 2010. 05. 11  최육사	이벤트 보상 코드 정리
//#ifdef SERV_ROOMUSERINFO_REFAC
	float	GetBonusRate( KRoomUserInfo::BONUS_TYPE eType )		{ return m_kRoomUserInfo.GetBonusRate( eType ); }
//#endif SERV_ROOMUSERINFO_REFAC
	//}}
	bool	IsRingofpvprebirth()								{ return m_kRoomUserInfo.m_bIsRingofpvprebirth; }

    void	SetUserEquipItem( std::vector< KInventoryItemInfo >& vecChangedInvenSlotInfo );
	void	AddItem( int iItemID, int iQuantity );

	void	GetItemList( OUT std::map< int, int >& mapItemList ) { mapItemList = m_mapGetItemList; }
    int     GetItemCount();

	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	char	GetCurrentRank( void ) const						{ return m_kRoomUserInfo.m_cRank;	}
	char	GetCurrentRankForServer( void ) const				{ return m_kRoomUserInfo.m_cRankForServer;	}
	float	GetKFactor( void ) const							{ return m_kRoomUserInfo.m_fKFactor; }
	//{{ 2012. 06. 21	박세훈	2012 대전 시즌2
	bool	IsPlacementPvpUser( void ) const					{ return GetOfficialMatchCount() < 10;	}
	//{{ 2012. 06. 25	박세훈	2012 대전 시즌2
	bool	IsRedistributionUser( void ) const					{ return m_kRoomUserInfo.m_bIsRedistributionUser;	}
#else
	CXSLUnit::PVP_EMBLEM	GetPVPEmblem() const				{ return (CXSLUnit::PVP_EMBLEM)(CXSLUnit::GetPVPEmblem(m_kRoomUserInfo.m_iRating)); }
#endif SERV_2012_PVP_SEASON2
	//}}
	int		GetOfficialMatchCount() const						{ return m_kRoomUserInfo.m_iOfficialMatchCnt; }
	int		GetRating() const									{ return m_kRoomUserInfo.m_iRating; }
	int		GetMaxRating() const								{ return m_kRoomUserInfo.m_iMaxRating; }

	bool	IsWinBeforeMatch() const							{ return m_kRoomUserInfo.m_bIsWinBeforeMatch; }
	bool	IsPvpNpc() const									{ return m_kRoomUserInfo.m_bIsPvpNpc; }
	void	SetStat( IN const KStat& kStat )					{ m_kRoomUserInfo.m_kGameStat = kStat; }
	int		GetRPoint() const									{ return m_kPvpUnitInfo.m_iRPoint; }
	int		GetAPoint() const									{ return m_kPvpUnitInfo.m_iAPoint; }
	int		GetWinCount() const									{ return m_kPvpUnitInfo.m_iWinCount; }
	bool	IsPlayWithParty() const								{ return m_kPvpUnitInfo.m_bIsPlayWithParty; }
#else
	CXSLUnit::PVP_EMBLEM	GetPVPEmblem(){ return (CXSLUnit::PVP_EMBLEM)(m_kRoomUserInfo.m_iPVPEmblem); }
#endif SERV_PVP_NEW_SYSTEM
	//}}	

	bool	IsSuccessResult()                                   { return m_bIsResult; }
	void	SetSuccessResult( bool bResult )                    { m_bIsResult = bResult; }

	//DUNGEON 게임중 내가죽인 NPC 수를 체크하기위해
	void	AddKillNPC()										{ ++m_nKillNPC; }
	int		GetKillNPC_LUA()									{ if( m_nKillNPC < 1 ) m_nKillNPC = 1; return m_nKillNPC; }	

	//DUNGEON 게임결과를 내기위한 함수..
	//{{ 2010. 05. 12  최육사	서버 코드 정리
	void	SetDungeonUnitInfo( IN const KDungeonPlayResultInfo& kDungeonUnitInfo );
	//}}
	void	SetDungeonPlayInfo( KEGS_DUNGEON_PLAY_INFO_TO_SERVER_NOT& kDungeonPlayInfo ); // 통계

	//{{ 2011. 08. 03	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	void	SetPvpPlayResultInfo( IN const KPvpPlayResultInfo& kPvpUnitInfo );
#endif SERV_PVP_NEW_SYSTEM
	//}}

	int		GetComboScore_LUA()									{ return m_kDungeonUnitInfo.m_ComboScore; }
	int		GetTechnicalScore_LUA()								{ return m_kDungeonUnitInfo.m_TechScore; }
	int		GetDamageNum_LUA()									{ return m_kDungeonUnitInfo.m_nDamageCount; }
	//{{ 2011. 01. 21	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
	int		GetTotalGivenDamamge_LUA() const					{ return m_kDungeonUnitInfo.m_iTotalGivenDamamge; }
	int		GetTotalAttackedDamage_LUA() const					{ return m_kDungeonUnitInfo.m_iTotalAttackedDamage; }	// 전체 피격 대미지	
	int		GetTotalSumNpcHP_LUA()								{ return m_kDungeonUnitInfo.m_iTotalSumNpcHP; }			// 전체 NPC의 HP합
	int		GetTotalRecoveryHP_LUA()							{ return m_kDungeonUnitInfo.m_iTotalRecoveryHP; }		// 소비 아이템으로 회복한 총 HP
	int		GetUsingKindOfSkill_LUA()							{ return m_kDungeonUnitInfo.m_iUsingKindOfSkill; }		// 스킬 사용 종류
	int		GetCommandTechScore_LUA()							{ return m_kDungeonUnitInfo.m_iCommandTechScore; }		// 커맨드 테크니컬 점수
	int		GetSkillTechScore_LUA()								{ return m_kDungeonUnitInfo.m_iSkillTechScore; }		// 스킬 테크니컬 점수
#endif SERV_DUNGEON_RANK_NEW
	//}}
	//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
	int		GetDashCount()										{ return m_kDungeonUnitInfo.m_iDashCount; }				// 대시 사용 횟수
#endif SERV_DUNGEON_DASH_LOG
	//}}
	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	int		GetLastUpdatedHP() const							{ return static_cast<int>(m_kDungeonUnitInfo.m_fHP); }
	int		GetInGameAverageFrame() const						{ return m_kDungeonUnitInfo.m_iFrame; }					// 던전 한판동안 평균 프레임
	int		GetStartTimeNumResurrectionStone() const			{ return m_iStartTimeNumResurrectionStone; }			// 시작했을 당시에 부활석 갯수
	int		GetFirstUseResurrectionStoneStageID() const			{ return m_iFirstUseResurrectionStoneStageID; }			// 부활석을 처음 사용한 스테이지ID
	int		GetFirstUseResurrectionStoneSubStageID() const		{ return m_iFirstUseResurrectionStoneSubStageID; }		// 부활석을 처음 사용한 서브 스테이지ID
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	char	GetUnitType()										{ return static_cast<char>( CXSLUnit::GetUnitClassToUnitType( (CXSLUnit::UNIT_CLASS)m_kRoomUserInfo.m_cUnitClass ) ); }
	char	GetUnitClass_LUA()									{ return m_kRoomUserInfo.m_cUnitClass; }
	int		GetTotalScore()										{ return m_kDungeonUnitInfo.m_TotalScore; }	

	void	ResetDungeonUnitInfo()								{ m_bDungeonUnitInfo = false; }
	bool	IsDungeonUnitInfo()									{ return m_bDungeonUnitInfo; }	

	void	SetResultData_LUA( IN double fPercent, IN int nRank ) { m_fPercent = (float)fPercent; m_nRank = nRank; }
	float	GetPercent()										{ return m_fPercent; }
	int		GetRank()											{ return m_nRank; }

	//{{ 2009. 6. 10  최육사	게임 시작할 당시 EXP,ED
	int		GetOldEXP()											{ return m_iOldEXP; }
	int		GetOldED()											{ return m_iOldED; }
	void	UpdateetOldExpED( IN int iOldEXP, IN int iOldED )		{ m_iOldEXP = iOldEXP; m_iOldED = iOldED; }
	//}}
	//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	void	SetGameChannelIP( IN const std::wstring& wstrGameChannelIP )	{ m_wstrGameChannelIP = wstrGameChannelIP; }
	const std::wstring& GetGameChannelIP() const				{ return m_wstrGameChannelIP; }
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
	bool	GetSkillSlotType()										{ return m_kDungeonUnitInfo.m_bSkillSlotType; }				// true = A , flase = B 
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
	//}}
	void	AddRewardEXP( IN int iEXP )							{ m_iRewardEXP += iEXP; }
	void	AddRewardPartyEXP( IN int iPartyEXP )				{ m_iRewardPartyEXP += iPartyEXP; }
	int		GetRewardEXP()										{ return m_iRewardEXP; }
	int		GetRewardPartyEXP()									{ return m_iRewardPartyEXP; }
	void	AddRewardED( IN int ED )							{ m_nRewardED += ED; }
	int		GetRewardED()										{ return m_nRewardED; }
	void	AddRewardVP( IN int VP )							{ m_nRewardVP += VP; }
	int		GetRewardVP()										{ return m_nRewardVP; }

    void	StartGame();
    void	StartPlay();
    void	EndPlay();
    void	EndGame();

    bool	IsLoadingTimeOut();	
	float	GetPlayTime(); // 던전&스테이지 로딩시간을 포함한 플레이 시간입니다.

	//{{ 2007. 11. 5  최육사  부활석 카운트, 지나온 스테이지 카운트
	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	void	IncreaseUsedResurrectionStoneCount();
#else
	void	IncreaseUsedResurrectionStoneCount()				{ ++m_iUsedResurrectionStoneCount; }
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	int		GetUsedRessurectionStoneCount() const				{ return m_iUsedResurrectionStoneCount; }
	void	IncreasePassedStageCount()							{ ++m_iPassedStageCount; m_iPassedSubStageCount = 0; } // 서브 스테이지 카운트는 0으로 초기화
	int		GetPassedStageCount()								{ return m_iPassedStageCount; }
	void	IncreasePassedSubStageCount()						{ ++m_iPassedSubStageCount; }
	int		GetPassedSubStageCount()							{ return m_iPassedSubStageCount; }
	//}}
	//{{ 2007. 12. 17  최육사  난입 상태
	void	SetIsIntrude( bool bIsIntrude )						{ m_bIsIntrude = bIsIntrude; }
	bool	GetIsIntrude()										{ return m_bIsIntrude; }
	//}}

	//{{ 2008. 3. 4  최육사  닉네임 얻기
	const std::wstring& GetNickName()							{ return m_kRoomUserInfo.m_wstrNickName; }
	//}}

	bool	IsGameBang()										{ return m_kRoomUserInfo.m_bIsGameBang; }
#ifdef SERV_PC_BANG_TYPE
	int		GetPcBangType()										{ return m_kRoomUserInfo.m_iPcBangType; }	
#endif SERV_PC_BANG_TYPE

	//zombie unit funtion
	//{{ 2012. 09. 08	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool	ZU_Tick( IN const CXSLRoom::ROOM_TYPE eRoomType );
#else
	bool	ZU_Tick();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}	
	void	ZU_Refresh();
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    bool    ZombieAlert_Tick();
    void    ZombieAlert_Refresh();
    bool    ZombieAlert_Get();
    void    ZombieAlert_Set( bool bZombieAlert );
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

    bool	RequestTradeTo( UidType nCID );
    bool	TradeAcceptedBy( UidType nCID );
    bool	TradeRejectedBy( UidType nCID );
    bool	ExpireTradeRequest();

	//{{ 2011. 08. 31  김민성	비밀던전 헬모드
//#ifdef SERV_RANDOM_DAY_QUEST
	bool	GetQuestDropItem( IN const int iDungeonID,
							  IN const char cDifficulty,
							  IN const char cDungeonMode,
							  IN const int iNPCID,
							  OUT QUEST_ITEM_DROP_INFO& sInfo,
							  IN const bool bComeBackUserInRoom /*false*/
							  //{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
							  , IN float fEventDropRate
#endif SERV_QUEST_ITEM_DROP_EVENT
							  //}
#ifdef SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
							  , IN bool bSuitableLevel = false
#endif //SERV_THANKS_GIVING_DAY_DROP_FOR_EQUIPPED_USER
							  );
//#endif SERV_RANDOM_DAY_QUEST
	//}} 
	//{{ 2012. 07. 16	최육사		배틀필드 입장 퀘스트
#ifdef SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	bool	GetQuestDropItemInBattleField( IN const int iBattleFieldID, 
										   IN const int iNpcID, 
										   //{{ 2013. 03. 18	 퀘스트 드롭 확률 증가 이벤트 - 김민성
#ifdef SERV_QUEST_ITEM_DROP_EVENT
										   IN const float fEventRate,
#endif SERV_QUEST_ITEM_DROP_EVENT
										   //}
										   OUT QUEST_ITEM_DROP_INFO& sInfo );
#endif SERV_ENTER_BATTLE_FIELD_QUEST_CLEAR
	//}}

	bool	IsObserver()										{ return m_kRoomUserInfo.m_bIsObserver; }

	bool	SetUnitQuestInvenInfo( std::map<int, KSubQuestInfo>& mapInvenInfo );

	void	SetTutorialUIDList( const std::vector<UidType>& vecTutorUIDList ) { m_vecStudentUnitUID = vecTutorUIDList; }
	void	GetTutorialUIDList( std::vector<UidType>& vecTutorUIDList ) { vecTutorUIDList = m_vecStudentUnitUID; }

	//{{ 2008. 6. 17  최육사  실시간 부활석
	void	SetEndPlay( bool bEndPlay )							{ m_bEndPlay = bEndPlay; }
	void	SetCashContinueReady( bool bCashContinueReady )		{ m_bCashContinueReady = bCashContinueReady; }
	void	ReserveEndPlay();
	bool	CheckEndPlay();
	void	StopDungeonContinueTime( bool bIsStop );
	//}}

	//{{ 2009. 7. 21  최육사	비밀스테이지
	void	SetAgreeEnterSecretStage( CXSLRoom::ENTER_SECRET_STAGE eVal ) { m_eAgreeEnterSecretStage = eVal; }
	CXSLRoom::ENTER_SECRET_STAGE GetAgreeEnterSecretStage() { return m_eAgreeEnterSecretStage; }
	//}}

	//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	bool	HavePet()										{ return !m_kRoomUserInfo.m_vecPet.empty(); }
	void	UpdatePetInfo( IN const std::vector< KPetInfo >& vecPetInfo );
	bool	UpdatePetSatiety( IN UidType iPetUID, IN short sSatiety );
	bool	UpdatePetIntimacy( IN UidType iPetUID, IN int iIntimacy );
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
	int		GetPetID() const;
#else //SERV_PETID_DATA_TYPE_CHANGE
	char	GetPetID() const;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	char	GetPetEvolutionStep() const;
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	void	GetOngoingQuest( IN int iSubQuestType, OUT std::map< int, KSubQuestInfo >& mapOngoingQuest ) const;
	void	CheckItemEquipQuestCondition( IN bool bIsGameStart );
	bool	IsEquippedItem( IN int iItemID ) const;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

	//{{ 2010. 12. 7	최육사	적정 레벨 장비 통계
#ifdef SERV_APPROPRIATE_LEVEL_STAT
	void	GetAppropriateLevelInfo( OUT KUserAppropriateLevelInfo& kInfo );
#endif SERV_APPROPRIATE_LEVEL_STAT
	//}}

	//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
	bool	AddCaughtBuffSkill( IN const int iBuffEnum, IN const float fBuffTime, IN const bool bBuffState, IN const UidType iCastingUnitUID, IN const char cBuffLevel );	// 걸린 버프 스킬 추가
	bool	DeleteCaughtBuffSkill( IN const int iBuffEnum );																			// 걸린 버프 스킬 삭제
	bool	GetCaughtSpecificBuffSkill( IN const int iBuffEnum, OUT BUFF_SKILL_INFO& BuffInfo );										// 걸린 특정버프 정보 얻기
	bool	GetFinishBuffSkill( OUT std::vector< KBuffSkillInfo >& vecBuff );													// 걸린 모든 버프 정보 얻기
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	void	SetUnitAllQuestInfo( std::set< int >& setQuestInfo )  { m_kRoomUserInfo.m_setQuestInfo = setQuestInfo; }
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
	bool	HasSpecialItem( IN int iItemID ) const;
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	void	SetUnitGoingQuestInfo( std::set< int >& setQuestInfo )  { m_kRoomUserInfo.m_setGoingQuestInfo = setQuestInfo; }
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST

#ifdef SERV_PVP_REMATCH
	bool	IsAcceptRematch()						{ return m_bAcceptRematch; }
	void	SetRematch( bool bAcceptRematch )		{ m_bAcceptRematch = bAcceptRematch; }
#endif SERV_PVP_REMATCH

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
	bool	AddAllyNPC( IN const KNPCUnitReq& kNPCUnit );
	bool	IsAllyNPC( IN const int iNPCID );
	void	DeleteAllyNPC( IN const UidType iNPCUID );
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

	//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
	double	GetStagePlayTime();
	int		GetStageDamage() const	{ return m_iStageDamage; }
	void	SetStageDamage( IN const int iStageDamage ) { m_iStageDamage = iStageDamage; }
#endif SERV_DUNGEON_STAGE_LOG
	//}}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	bool	IsComeBackUser()									{ return m_kRoomUserInfo.m_bComeBackUser; }
#endif SERV_COME_BACK_USER_REWARD
	//}}
	//{{ 2011. 07. 25	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	void	SetPvpNpcID( IN const int iNpcID )		{ m_iPvpNpcID = iNpcID; }
	int		GetPvpNpcID() const						{ return m_iPvpNpcID; }
	void	SetMatchWaitTime( IN const int iMatchWaitTime )		{ m_iMatchWaitTime = iMatchWaitTime; }
	int		GetMatchWaitTime() const				{ return m_iMatchWaitTime; }
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	void	SetHenirRewardUser( IN const bool bHenirRewardUser )	{ m_bHenirReward = bHenirRewardUser; }
	bool	IsHenirRewardUser() const							{ return m_bHenirReward; }
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	SetActivatedAutoPartyBonus( IN const bool bVal )	{ m_bActivatedAutoPartyBonus = bVal; }
	bool	IsActivatedAutoPartyBonus() const					{ return m_bActivatedAutoPartyBonus; }
	void	SetBattleFieldNpcLoad( IN const bool bVal )			{ m_bBattleFieldNpcLoad = bVal; }
	bool	IsBattleFieldNpcLoad() const						{ return m_bBattleFieldNpcLoad; }
	void	SetBattleFieldNpcSyncSubjects( IN const bool bVal )	{ m_bBattleFieldNpcSyncSubjects = bVal; }
	bool	IsBattleFieldNpcSyncSubjects() const				{ return m_bBattleFieldNpcSyncSubjects; }
	void	RegIntrudeUnitLoadUser( IN const UidType iUnitUID ) { m_setIntrudeUnitLoadUserList.insert( iUnitUID ); }
	void	UnregIntrudeUnitLoadUser( IN const UidType iUnitUID ) { m_setIntrudeUnitLoadUserList.erase( iUnitUID ); }
	bool	IsEmptyIntrudeUnitLoadUserList() const				{ return m_setIntrudeUnitLoadUserList.empty(); }
	const std::set< UidType >& GetOldPartyMember() const		{ return m_kOldPartyInfo.m_setOldPartyMember; }
	int		GetOldPartyMemberCount() const						{ return static_cast<int>(m_kOldPartyInfo.m_setOldPartyMember.size()); }
	void	SetOldPartyInfo( IN const KOldPartyInfo& kInfo )	{ m_kOldPartyInfo = kInfo; }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	void	UpdateGameStat( IN const KStat& kGameStat );
	void	UpdateBuffInfo( IN const std::vector< KBuffInfo >& vecActivatedBuff );
	bool	ExistActiveBuff( IN const int iBuffID );
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void	AddUseSkillBuffInPlay( IN const int iSkillID )		{ m_kRoomUserInfo.m_setUseSkillBuffInPlay.insert( iSkillID ); }
	void	DelUseSkillBuffInPlay( IN const int iSkillID )		{ m_kRoomUserInfo.m_setUseSkillBuffInPlay.erase( iSkillID ); }
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
	bool CheckCompleteSubQuest_BeforGroup( IN int iQuestID, IN int iSubQuestID, OUT std::vector<int>& vecUnderGroupSubQuest );
	bool CheckFindNPCQuest( IN int iMapID, IN std::vector< int >& vecNpc, IN bool IsDungeon );
#endif SERV_REFORM_QUEST
	//}}
	//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
	bool	IsEnterCashShopUser() const							{ return m_bEnterCashShopInBattleField; }
	void	UpdateUserCashShopInfo( IN bool bEnter );
#endif SERV_VISIT_CASH_SHOP
	//}}
	
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	DWORD	GetPingScore();
	void	SetPingScore( DWORD dwPingScore );
	void	ClearPingScore();
#endif SERV_CHOOSE_FASTEST_HOST
	//}


	//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	int		GetAutoPartyWaitTime()								{ return m_iAutoPartyWaitTime; }
	void	SetAutoPartyWaitTime( int iAutoPartyWaitTime )		{ m_iAutoPartyWaitTime = iAutoPartyWaitTime; }
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}

	//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
	int		GetFieldFrame( void ) const				{	return m_iFieldFrame;			}
	void	SetFieldFrame( IN int iFieldFrame )		{	m_iFieldFrame = iFieldFrame;	}
#endif SERV_FIELD_FRAME_LOG
	//}}
	//{{ 2012. 12. 18	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	void	SetBonusRate( IN const std::map< char, float >& mapBonusRate )	{ m_kRoomUserInfo.m_mapBonusRate = mapBonusRate; }
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

	//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
	UidType		GetOldPartyUID()					{ return m_kOldPartyInfo.m_iOldPartyUID; }
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
	//}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
	void	SetRidingPetInfo( IN const UidType iRidingPetUID, IN const USHORT usRidingPetID )
	{
		m_kRoomUserInfo.m_iRidingPetUID = iRidingPetUID;
		m_kRoomUserInfo.m_usRidingPetID = usRidingPetID;
	}
#endif	// SERV_RIDING_PET_SYSTM

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	void	SetLoverUnitUID( IN UidType iLoverUnitUID )			{ m_kRoomUserInfo.m_iLoverUnitUID = iLoverUnitUID; }
	char	GetWeddingStatus()									{ return m_kRoomUserInfo.m_cWeddingStatus; }
#endif SERV_RELATIONSHIP_SYSTEM
	//}

protected:
    KRoomUserInfo                   m_kRoomUserInfo;
    int                             m_iTeam;
    bool                            m_bHost;
    bool                            m_bReady;
	bool							m_bPitIn;
    bool                            m_bTrade;
    int                             m_iLoadingProgress;
    bool                            m_bStageLoaded;
    int                             m_nKill;
	int                             m_nMDKill;
    int                             m_nDie;
    bool                            m_bDie;
    float                           m_fHP;
    int                             m_iSlotID;
	int							    m_iNowStageID;
	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	int								m_iNowSubStageID;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
    int                             m_iRebirthPos;
	int								m_nKillNPC;
	//{{ 2009. 6. 10  최육사	게임 시작할 당시 EXP,ED
	int								m_iOldEXP;
	int								m_iOldED;
	//}}
	//게임중 죽었을 경우 결과처리를 하기위해 임시저장
	int								m_iRewardEXP;
	int								m_iRewardPartyEXP;
	int								m_nRewardED;
	int								m_nRewardVP;
	bool							m_bIsResult;	// result 화면 처리가 끝난는가..?

	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	int								m_iStartTimeNumResurrectionStone;
	int								m_iFirstUseResurrectionStoneStageID;
	int								m_iFirstUseResurrectionStoneSubStageID;
	std::wstring					m_wstrGameChannelIP;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2011. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool							m_bActivatedAutoPartyBonus;
	bool							m_bBattleFieldNpcLoad;
	bool							m_bBattleFieldNpcSyncSubjects;
	std::set< UidType >				m_setIntrudeUnitLoadUserList;
	KOldPartyInfo					m_kOldPartyInfo;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool							m_bHenirReward;		// KRoomUserInfo의 멤버변수였던것을 이쪽으로 옮겼다. 굳이 GetRoomUserInfo로 계속 가져올 필요가 없기 때문 - 육사
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2010. 05. 12  최육사	서버 코드 정리
	KDungeonPlayResultInfo			m_kDungeonUnitInfo; // 결과처리를 위해 클라이언트에서 보내는 유저 게임데이터
	//}}
	bool							m_bDungeonUnitInfo;

	//{{ 2011. 08. 03	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	KPvpPlayResultInfo				m_kPvpUnitInfo;
	int								m_iMatchWaitTime;
#endif SERV_PVP_NEW_SYSTEM
	//}}

	std::map< int, int >			m_mapGetItemList;		// 던전 게임중 내가 먹은 아이템	
	
	float							m_fPercent;
	int								m_nRank;

    boost::timer                    m_kLoadingTimer;
    boost::timer                    m_kPlayTimer;
	float							m_fPlayTime;
	//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
	boost::timer					m_kStagePlayTimer;
	double							m_fLastStagePlayTime;

	int								m_iStageDamage;
#endif SERV_DUNGEON_STAGE_LOG
	//}}

	// 부활석 사용 카운트, 지나온 스테이지 카운트
	int								m_iUsedResurrectionStoneCount;
	int								m_iPassedStageCount;
	int								m_iPassedSubStageCount;
	
	//{{ 2007. 12. 17  최육사  난입 상태 [for statistics]
	bool							m_bIsIntrude;
	//}}

	//Zombie user 체크를 위한 변수
	boost::timer					m_TimerZU;
#ifdef  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST
    boost::timer                    m_TimerZombieAlert;
    bool                            m_bZombieAlert;
#endif  SERV_OPTIMIZE_DETECT_ZOMBIE_HOST

    std::map< UidType, boost::timer >   m_mapTradeRequest;

	//080408.hoons. 사제시스템 던전게임 보상처리를 위해 사용될 데이터
	std::vector< UidType >			m_vecStudentUnitUID;

	// 실시간 부활석
	bool							m_bEndPlay;
	bool							m_bCashContinueReady;
	double							m_fDelayPacketTime;
	boost::timer					m_tDelayPacketTimer;
	boost::timer					m_tCashContinueTimer;

//
//#ifdef PVP_BOSS_COMBAT_TEST
//
//	bool							m_bIsBoss;
//#endif PVP_BOSS_COMBAT_TEST

	//{{ 2009. 7. 21  최육사	비밀스테이지
	CXSLRoom::ENTER_SECRET_STAGE	m_eAgreeEnterSecretStage;
	//}}

	//{{ 2010. 12. 7	최육사	적정 레벨 장비 통계
#ifdef SERV_APPROPRIATE_LEVEL_STAT
	KUserAppropriateLevelInfo		m_kAppropriateLevelInfo;
#endif SERV_APPROPRIATE_LEVEL_STAT
	//}}

	//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
	std::map< int, BUFF_SKILL_INFO >	m_mapBuff;		// 유저가 걸린 버프
#endif SERV_ADD_BUFF_SKILL_INFO
	//}}
	//{{ 2011. 07. 25	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	int								m_iPvpNpcID;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
	bool							m_bEnterCashShopInBattleField;
#endif SERV_VISIT_CASH_SHOP
	//}}
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	bool							m_bRecivedPingScore;
	std::vector< DWORD >			m_vecPingScore;
#endif SERV_CHOOSE_FASTEST_HOST

	//}
	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	float							m_fAddQuestDropBouns;
#endif SERV_SERVER_BUFF_SYSTEM
	//}
	//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	int								m_iAutoPartyWaitTime;
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}
	//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
	int								m_iFieldFrame;
#endif SERV_FIELD_FRAME_LOG
	//}}
#ifdef SERV_PVP_REMATCH
	bool							m_bAcceptRematch;
#endif SERV_PVP_REMATCH

	//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	bool							m_bPrepareForDefence;	
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	//{{ 2013. 04. 22	최육사	어둠의 문 입장 로직 변경
#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	bool							m_bRecvEnterPopupReply;
	bool							m_bEnterDefenceDungeon;
#endif SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC
	//}}

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
	std::map< UidType, KNPCUnitReq >	m_mapAllyNPCUnit;
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
public:
	void SetPingScoreForForceHost( DWORD dwPingScore );
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
};

#pragma pack( pop )


