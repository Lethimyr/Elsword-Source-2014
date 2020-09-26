#pragma once

#include <map>
#include <vector>
//{{ 2011. 01. 16	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	#include "CacheData.h"
#endif SERV_CHAR_LOG
//}}

typedef std::pair< int, int >	UserStatisticsKey;
typedef std::map< std::pair< int, int >, std::vector< int > >::iterator UserStatisticsIterator;

class KUserStatistics
{
public:

	enum US_Table
	{
		USTable_PvpModeCount = 0,
		USTable_PvpPlayTime,
		USTable_Dungeon,
		USTable_TrainingCenter,
		USTable_EtcData,
#ifdef SERV_USER_STATISTICS_RENEWAL
		USTable_EDData,
#endif // SERV_USER_STATISTICS_RENEWAL
	};

	enum US_Columns_Pvp_ModeCount
	{
        US_Pvp_MC_Win = 0,
		US_Pvp_MC_Lose,
		US_Pvp_MC_Drop,
		US_Pvp_MC_Draw,
		US_Pvp_MC_Intrude,		

		US_Pvp_MC_Max,
	};

	enum US_Columns_Pvp_PlayTime
	{
		US_Pvp_PT_Win = 0,
		US_Pvp_PT_Lose,
		US_Pvp_PT_Drop,
		US_Pvp_PT_Draw,
		US_Pvp_PT_Intrude,		

		US_Pvp_PT_Max,
	};

	enum US_Columns_TrainingCenter
	{
		US_TC_MC_Success = 0,
		US_TC_MC_Fail,
		US_TC_MC_Drop,
		US_TC_PT_Success,
		US_TC_PT_Fail,
		US_TC_PT_Drop,

		US_TC_Max,
	};

	enum US_Columns_Dungeon
	{
		US_D_MC_Win = 0,
		US_D_MC_Lose,
		US_D_MC_Drop,
		US_D_MC_ReStone, // 부활석
		US_D_MC_Party_Win,
		US_D_MC_Party_Lose,
		US_D_MC_Party_Drop,
		US_D_MC_Party_ReStone, // 부활석
		US_D_PT_Win,
		US_D_PT_Lose,
		US_D_PT_Drop,
		US_D_PT_Party_Win,
		US_D_PT_Party_Lose,
		US_D_PT_Party_Drop,

		US_D_Max,
	};

	enum US_Columns_EtcData
	{
		US_Etc_Used_Spirit = 0,
		US_Etc_AllUsed_Spirit_Count,
		US_Etc_Duration_Time,
#ifdef SERV_USER_STATISTICS_RENEWAL
		US_Etc_Dungeon_Play_Time,
		US_Etc_PVP_Play_Time,
#endif //SERV_USER_STATISTICS_RENEWAL
		US_Etc_Pvp_Kill_Count,
		US_Etc_Obtained_Item,
		US_Etc_Shop_Buy_Item,
		US_Etc_Shop_Sell_Item,
		US_Etc_Trade_Buy_Item,
		US_Etc_Trade_Sell_Item,
		US_Etc_Square_Count,
		US_Etc_Square_PlayTime,
		US_Etc_IsPcBang,		
#ifndef SERV_USER_STATISTICS_RENEWAL
		US_Etc_PDungeonED,
		US_Etc_PQuestED,
		US_Etc_PItemSellED,		
		US_Etc_MItemBuyED,
		US_Etc_MManufactureED,
		US_Etc_MItemRepairED,
		US_Etc_MOpenPShopED,
		US_Etc_MEnchantED,
		US_Etc_MSocketED,
#endif //SERV_USER_STATISTICS_RENEWAL
		US_Etc_TradeCount,
		US_Etc_NQuestAccept,
		US_Etc_NQuestGiveUp,
		US_Etc_NQuestComplete,		
		US_Etc_RQuestAccept,
		US_Etc_RQuestGiveUp,
		US_Etc_RQuestComplete,
#ifndef SERV_USER_STATISTICS_RENEWAL	
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
		US_Etc_OpenCubeSpendED,
//#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
		US_Etc_PBattleFieldED,				// 배틀 필드에서 획득한 ED
		US_Etc_MCoboExpressTicketED,		// 마을 이동서를 통해 소모한 ED
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
#endif //SERV_USER_STATISTICS_RENEWAL
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05// 작업날짜: 2013-06-17	// 박세훈
		US_Etc_MItemEvaluate,				// 아이템 감정으로 소모한 ED
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05

		US_Etc_Max,
	};

#ifdef SERV_USER_STATISTICS_RENEWAL
	enum US_Columns_EDData
	{
		US_ED_PDungeon = 0,
		US_ED_PTraining,
		US_ED_PQuest,
		US_ED_PItemSell,
		US_ED_PItemTrade,		
		US_ED_PPersonalShop,		
		US_ED_PPostLetter,
		US_ED_MItemBuy,
		US_ED_MItemTrade,		
		US_ED_MItemRepair,
		US_ED_MManufacture, 
		US_ED_MDunPenalty,
		US_ED_MPersonalShop,
		US_ED_MOpenPShop,
		US_ED_MEnchant,
		US_ED_MSocket,
		US_ED_MPostLetter,
		US_ED_MSendLetterBasic,
		US_ED_MSendLetterItem,
		US_ED_MAttribEnchant,		
		US_ED_MSellTaxPShop,
		US_ED_MBuyTaxPShop,
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
//#ifdef SERV_CUBE_OPEN_ED_CONDITION
		US_ED_OpenCubeSpendED,
//#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
//#ifdef SERV_SEPARATE_DUNGEON_FIELD_ED_LOG// 작업날짜: 2013-05-08	// 박세훈
		US_ED_PBattleFieldED,		// 배틀 필드에서 획득한 ED
		US_ED_MCoboExpressTicketED,		// 마을 이동서를 통해 소모한 ED
//#endif // SERV_SEPARATE_DUNGEON_FIELD_ED_LOG
//#ifdef SERV_MULTI_RESOLVE
		US_ED_MItemResolve,
//#endif SERV_MULTI_RESOLVE
		US_ED_Max,
	};
#endif //SERV_USER_STATISTICS_RENEWAL


	enum PLAY_TIME
	{
        PT_TOTAL_PLAY_TIME = 0,
		PT_PVP_PLAY_TIME,
		//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
		PT_SPIRIT_PLAY_TIME,
		PT_DUNGEON_PLAY_TIME,
		PT_FIELD_PLAY_TIME,
#else
		PT_DUNGEON_PLAY_TIME,
#endif SERV_FIELD_PLAY_TIME_LOG
		//}}

		PT_MAX,
	};

	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	enum ACCOUNT_GAME_COUNT_TYPE
	{
		AGCT_BUY_CASH_ITEM = 0,			// [0] 캐쉬템 구매 카운트

		AGCT_MAX,
	};
#endif SERV_ACCOUNT_COUNT
	//}}

	//{{ 2011. 01. 16	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	enum CHAR_GAME_COUNT_TYPE
	{
		CGCT_DUNGEON_PLAY_COUNT = 0,	// [0] 던전 플레이 카운트
		CGCT_DUNGEON_PLAY_TIME,			// [1] 던전 플레이 타임
		CGCT_PVP_PLAY_COUNT,			// [2] 대전 플레이 카운트
		CGCT_PVP_PLAY_TIME,				// [3] 대전 플레이 타임
		CGCT_DUNRATION_TIME,			// [4] 캐릭터 선택 누적 시간
		CGCT_USE_SKILL_POINT,			// [5] 스킬 포인트 사용 카운트
		CGCT_SELECT_CHAR,				// [6] 캐릭터 선택 카운트
		CGCT_DESELECT_CHAR,				// [7] 캐릭터 선택 해제 카운트
		CGCT_JOIN_GUILD_SELECT_CHAR,	// [8] 캐릭터 선택 카운트 (길드 가입된 캐릭터)
		//{{ 2011. 02. 09	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG
		CGCT_BUY_CASH_ITEM,				// [9] 캐쉬템 구매 카운트
#endif SERV_BUY_CASH_ITEM_LOG
		//}}
		//{{ 2011. 04. 13	최육사	마지막 플레이 던전 저장
		CGCT_LAST_PLAY_DUNGEON_ID,		// [10] 마지막 플레이 던전 ID
		//}}

		CGCT_MAX,
	};
#endif SERV_CHAR_LOG
	//}}

public:
	KUserStatistics(void);
	~KUserStatistics(void);

	// 통계 정보 수집 대상 설정
	void SetIsNormalUser( char cAuthLevel );

	// 통계 정보 수집
	void IncreaseCount( int iTableID, int iSubTableID , int iColumn, int iCount );
	void IncreaseCount( std::map< std::pair< int, int >, std::vector< int > >& mapStatistics , UserStatisticsKey& key, std::vector< int >& vecCount );	

	// Character Statistics
	void Clear();
	void GetCharacterStatistics( std::map< std::pair< int, int >, std::vector< int > >& mapPvpStatistics,
				 				 std::map< std::pair< int, int >, std::vector< int > >& mapDungeonStatistics,
								 std::map< std::pair< int, int >, std::vector< int > >& mapTCenterStatistics,
								 std::map< std::pair< int, int >, std::vector< int > >& mapEtcStatistics 
#ifdef SERV_USER_STATISTICS_RENEWAL
								, std::map< std::pair< int, int >, std::vector< int > >& mapEDStatistics 
#endif //SERV_USER_STATISTICS_RENEWAL		 
								 );

	int GetEtcSize()					{ return m_mapEtcStatistics.size(); }

	//{{ 2008. 6. 16  최육사  
	void ResetPlayTime( PLAY_TIME ePT ) { m_iStatisticsPlayTime[ePT] = 0; }	
	void AddPlayTime( PLAY_TIME ePT, int iTotalPlayTime );
	int  GetPlayTime( PLAY_TIME ePT )	{ return m_iStatisticsPlayTime[ePT]; }
	//}}

	//{{ 2010. 06. 30  최육사	근성도 통계
#ifdef SERV_SPIRIT_STATISTICS
	void AddUsedSpirit( int iSpirit )	{ m_iUsedSpirit += iSpirit; }
	int	 GetUsedSpirit()				{ return m_iUsedSpirit; }
#endif SERV_SPIRIT_STATISTICS
	//}}

	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	bool IsAccountCount() const			{ return m_bIsAccountCount; }
	void InitAccountCountInfo( IN const std::wstring& wstrAccountRegDate, IN const std::map< int, int >& mapAccCountInfo );
	void IncreaseAccountCount( IN const ACCOUNT_GAME_COUNT_TYPE eCountType, IN const int iCount = 1 );
	int  GetAccountCount( IN const ACCOUNT_GAME_COUNT_TYPE eCountType ) const;
	const std::wstring& GetUnitCreateDate() const { return m_wstrUnitCreateDate; }
	void GetAccountDBUpdateInfo( OUT std::map< int, int >& mapAccCountInfo );
	bool IsFirstBuy() const				{ return ( GetAccountCount( KUserStatistics::AGCT_BUY_CASH_ITEM ) == 1 ); }
#endif SERV_ACCOUNT_COUNT
	//}}
	//{{ 2011. 01. 16	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	bool IsCharCount() const			{ return m_bIsCharCount; }
	void InitCharacterCountInfo( IN const std::map< int, int >& mapCharCountInfo, IN const std::wstring& wstrUnitCreateDate );
	void IncreaseCharacterCount( IN const CHAR_GAME_COUNT_TYPE eCountType, IN const int iCount = 1 );
	int	 GetCharacterCount( IN const CHAR_GAME_COUNT_TYPE eCountType ) const;
	const std::wstring& GetAccountRegDate() const { return m_wstrAccountRegDate; }
#ifdef SERV_USER_STATISTICS_RENEWAL
	const std::wstring& GetUnitRegDate() const { return m_wstrUnitCreateDate; }
#endif //SERV_USER_STATISTICS_RENEWAL
	void GetDBUpdateInfo( OUT std::map< int, int >& mapCharCountInfo );
	int  GetLastClearDungeonID() const	{ return m_iCharGameCount[CGCT_LAST_PLAY_DUNGEON_ID]; }
	void SetLastClearDungeonID( IN const int iDungeonID );
#endif SERV_CHAR_LOG
	//}}

	//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
	bool IsFirstCharSelectToday( IN const std::wstring& wstrUnitLastLoginDate );
#endif SERV_DAILY_CHAR_FIRST_SELECT
	//}}

	//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
	bool IsFirstUserLoginToday( IN const UidType& UserUID, IN const std::wstring& wstrUserLastLoginDate );
#endif SERV_SECOND_SECURITY
	//}}

	//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
	void SetCharName( std::wstring& wstrUnitID )						{ m_wstrLastUnitID = wstrUnitID; }
	std::wstring& GetCharName()											{ return m_wstrLastUnitID; }
#endif SERV_PCBANG_INCORRECT_BILLING
	//}

private:
	std::map< std::pair< int, int >, std::vector< int > >	m_mapPvpStatistics;
	std::map< std::pair< int, int >, std::vector< int > >	m_mapDungeonStatistics;
	std::map< std::pair< int, int >, std::vector< int > >	m_mapTCenterStatistics;
	std::map< std::pair< int, int >, std::vector< int > >	m_mapEtcStatistics;
#ifdef SERV_USER_STATISTICS_RENEWAL
	std::map< std::pair< int, int >, std::vector< int > >	m_mapEDStatistics;
#endif //SERV_USER_STATISTICS_RENEWAL

	bool													m_bIsNormalUser;

	//{{ 2007. 11. 19  최육사  유저 플레이 타임 통계
	int														m_iStatisticsPlayTime[PT_MAX];
	//}}
	//{{ 2010. 06. 30  최육사	근성도 통계
#ifdef SERV_SPIRIT_STATISTICS
	int														m_iUsedSpirit;
#endif SERV_SPIRIT_STATISTICS
	//}}

	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	KCacheData< int >										m_iAccountGameCount[AGCT_MAX];
	std::wstring											m_wstrAccountRegDate;
	bool													m_bIsAccountCount;
#endif SERV_ACCOUNT_COUNT
	//}}
	//{{ 2011. 01. 16	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG	
	KCacheData< int >										m_iCharGameCount[CGCT_MAX];
	std::wstring											m_wstrUnitCreateDate;
	bool													m_bIsCharCount;
#endif SERV_CHAR_LOG
	//}}

	//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
	std::wstring											m_wstrLastUnitID;
#endif SERV_PCBANG_INCORRECT_BILLING
	//}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
public:
	void SetSystemInfoStatistics( const KEGS_SYSTEM_INFO& kPacket ) { m_kSystemInfo = kPacket; }
	const KEGS_SYSTEM_INFO& GetSystemInfoStatistics() { return m_kSystemInfo; }

	void SetKOGGamePerformanceStatistics_Village( const KEGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG& kPacket ) { m_kKOGGamePerformance_Village = kPacket; }
	const KEGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG& GetKOGGamePerformanceStatistics_Village() { return m_kKOGGamePerformance_Village; }

	void SetKOGGamePerformanceStatistics_Field( const KEGS_KOG_GAME_PERFORMANCE_FIELD_LOG& kPacket ) { m_kKOGGamePerformance_Field = kPacket; }
	const KEGS_KOG_GAME_PERFORMANCE_FIELD_LOG& GetKOGGamePerformanceStatistics_Field() { return m_kKOGGamePerformance_Field; }

	void SetKOGGamePerformanceStatistics_Dungeon( const KEGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG& kPacket ) { m_kKOGGamePerformance_Dungeon = kPacket; }
	const KEGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG& GetKOGGamePerformanceStatistics_Dungeon() { return m_kKOGGamePerformance_Dungeon; }

	void SetKOGGamePerformanceStatistics_PvP( const KEGS_KOG_GAME_PERFORMANCE_PVP_LOG& kPacket ) { m_kKOGGamePerformance_PvP = kPacket; }
	const KEGS_KOG_GAME_PERFORMANCE_PVP_LOG& GetKOGGamePerformanceStatistics_PvP() { return m_kKOGGamePerformance_PvP; }

private:
	KEGS_SYSTEM_INFO m_kSystemInfo;
	KEGS_KOG_GAME_PERFORMANCE_VILLAGE_LOG m_kKOGGamePerformance_Village;
	KEGS_KOG_GAME_PERFORMANCE_FIELD_LOG m_kKOGGamePerformance_Field;
	KEGS_KOG_GAME_PERFORMANCE_DUNGEON_LOG m_kKOGGamePerformance_Dungeon;
	KEGS_KOG_GAME_PERFORMANCE_PVP_LOG m_kKOGGamePerformance_PvP;
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
};




