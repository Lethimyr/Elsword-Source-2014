#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "CommonPacket.h"
#include "X2Data/XSLDungeon.h"
//{{ 2011. 11. 23	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
#include "X2Data/XSLBattleField.h"
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
#include "RoomUser.h"
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#include "RoomUserManager.h"
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
//}}

class KDropTable
{
	DeclareRefreshSingleton( KDropTable );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;	

public:
    struct DROP_DATA
    {
		std::vector< int >	m_vecItemID;
        int					m_iExp;
        int					m_iGPItemID;
        int					m_iED;		//GP 량
        int					m_nGPNum;	//GP 개수

		DROP_DATA()
		{
			Clear();
		}

		void Clear()
		{
			m_vecItemID.clear();
			m_iExp        = -1;
			m_iGPItemID   = -1;
			m_iED         = -1;
			m_nGPNum      = -1;
		}
    };

#ifdef DUNGEON_ITEM
	enum PVP_FAILRATE
	{
		PF_WINTEAM = 0,	// 승리팀이 실패할 확률
		PF_LOSETEAM,	// 패배팀이 실패활 확률
		PF_INC_NUM,		// 팀원이 한명씩 많을대 높아지는 실패확률.
		PF_DEC_NUM,		// 팀원이 한명씩 적을대 낮아지는 실패확률.

		PF_END,
	};

#endif DUNGEON_ITEM

	//{{ 2011. 11. 23	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	struct KBattleFieldNpcDropInfo
	{
		int				m_nBattleFieldID;
		int				m_nMonsterID;
		bool			m_bKillCheck;
		int				m_iExp;
		int		        m_iED;
		float	        m_fEDProperty;

		KLottery        m_kLottery;

		KBattleFieldNpcDropInfo()
		{
			m_nBattleFieldID = 0;
			m_nMonsterID = 0;
			m_bKillCheck = false;
			m_iExp = 0;
			m_iED = 0;
			m_fEDProperty = 0.f;
		};

		KBattleFieldNpcDropInfo( const KBattleFieldNpcDropInfo& t ) { *this = t; }    // copy constructor
		KBattleFieldNpcDropInfo& operator=( const KBattleFieldNpcDropInfo& t )       // assign operator
		{
			m_nBattleFieldID = t.m_nBattleFieldID;
			m_nMonsterID = t.m_nMonsterID;
			m_bKillCheck = t.m_bKillCheck;
			m_iExp = t.m_iExp;
			m_iED = t.m_iED;
			m_fEDProperty = t.m_fEDProperty;
			m_kLottery = t.m_kLottery;
			return *this;
		}
	};

	struct KBattleFieldStaticDropInfo
	{
		int				m_nBattleFieldID;
		KLottery        m_kLottery;

		KBattleFieldStaticDropInfo(){};
		KBattleFieldStaticDropInfo( const KBattleFieldStaticDropInfo& t ) { *this = t; }    // copy constructor
		KBattleFieldStaticDropInfo& operator=( const KBattleFieldStaticDropInfo& t )       // assign operator
		{
			m_nBattleFieldID = t.m_nBattleFieldID;
			m_kLottery = t.m_kLottery;
			return *this;
		}
	};
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

protected:

    enum EDropCaseParam {   // head - DCP. 단품 아이템 or 아이템 그룹
        DCP_UNARY_ITEM = 0, // 단품 아이템
        DCP_ITEM_GROUP = 1, // 아이템 그룹
    };

	struct KMonsterDropInfo
	{
        int				m_nDungeonID;
		int				m_nMonsterID;
		bool			m_bKillCheck;
		int				m_iExp;
        int		        m_iED;
        float	        m_fEDProperty;

        KLottery        m_kLottery;

		KMonsterDropInfo(){};
		KMonsterDropInfo( const KMonsterDropInfo& t ) { *this = t; }    // copy constructor
		KMonsterDropInfo& operator=( const KMonsterDropInfo& t )       // assign operator
		{
			m_nDungeonID = t.m_nDungeonID;
			m_nMonsterID = t.m_nMonsterID;
			m_bKillCheck = t.m_bKillCheck;
			m_iExp = t.m_iExp;
			m_iED = t.m_iED;
			m_fEDProperty = t.m_fEDProperty;
			m_kLottery = t.m_kLottery;
			return *this;
		}
	};

	//{{ 2009. 7. 25  최육사	헤니르 시공 NPC드롭
	struct KHenirNpcDropInfo
	{
		int				m_iHenirDungeonMode;
		int				m_nMonsterID;
		bool			m_bKillCheck;
		int				m_iExp;
		int		        m_iED;
		float	        m_fEDProperty;

		KLottery        m_kLottery;

		KHenirNpcDropInfo(){};
		KHenirNpcDropInfo( const KHenirNpcDropInfo& t ) { *this = t; }    // copy constructor
		KHenirNpcDropInfo& operator=( const KHenirNpcDropInfo& t )       // assign operator
		{
			m_iHenirDungeonMode = t.m_iHenirDungeonMode;
			m_nMonsterID = t.m_nMonsterID;
			m_bKillCheck = t.m_bKillCheck;
			m_iExp = t.m_iExp;
			m_iED = t.m_iED;
			m_fEDProperty = t.m_fEDProperty;
			m_kLottery = t.m_kLottery;
			return *this;
		}
	};
	//}}

	//{{ 2010. 05. 02  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	struct KExtraStageNpcDropInfo
	{
		int				m_nMonsterID;
		bool			m_bKillCheck;
		int				m_iExp;
		int		        m_iED;
		float	        m_fEDProperty;

		KLottery        m_kLottery;

		KExtraStageNpcDropInfo(){};
		KExtraStageNpcDropInfo( const KExtraStageNpcDropInfo& t ) { *this = t; }    // copy constructor
		KExtraStageNpcDropInfo& operator=( const KExtraStageNpcDropInfo& t )       // assign operator
		{
			m_nMonsterID = t.m_nMonsterID;
			m_bKillCheck = t.m_bKillCheck;
			m_iExp = t.m_iExp;
			m_iED = t.m_iED;
			m_fEDProperty = t.m_fEDProperty;
			m_kLottery = t.m_kLottery;
			return *this;
		}
	};
#endif SERV_SECRET_HELL
	//}}

	//{{ 2009. 4. 21  최육사	전체드롭테이블
	struct KStaticDropInfo
	{
		int				m_nDungeonID;
		KLottery        m_kLottery;

		KStaticDropInfo(){};
		KStaticDropInfo( const KStaticDropInfo& t ) { *this = t; }    // copy constructor
		KStaticDropInfo& operator=( const KStaticDropInfo& t )       // assign operator
		{
			m_nDungeonID = t.m_nDungeonID;
			m_kLottery = t.m_kLottery;
			return *this;
		}
	};
	//}}
	
	//{{ 2009. 5. 23  최육사	시간 드롭
	struct KTimeDropItemInfo
	{
		int				m_iTimeDropID;
		int				m_iItemID;
		std::wstring	m_wstrItemName;
		bool			m_bIsDrop;
		CTime			m_kDropTime;
		bool			m_bNoticeMsg;

		KTimeDropItemInfo()
		{
			m_iTimeDropID = 0;
			m_iItemID	  = 0;
			m_bIsDrop	  = false;
			m_bNoticeMsg  = false;
		}
	};
	//}}

	//{{ 2011. 03. 28	최육사	서버 상태 체크
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
	struct KManyEDDropNpcInfo
	{
		float			m_fMultiplyDropED;
		float			m_fMultiplyDropEDCount;

		KManyEDDropNpcInfo()
		{
			m_fMultiplyDropED = 0.f;
			m_fMultiplyDropEDCount = 0.f;
		}
	};
#endif SERV_MANY_ED_DROP_EVENT_NPC
	//}}

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//{{ 2012. 10. 17	박세훈	PaymentItem 금지 조건 추가
#ifdef SERV_UNPAYMENT_CONDITION
	struct DUNGEON_CLEAR_REWARD
	{
		int		m_iDungeonID;
		int		m_iItemID;
		float	m_fRate;
		int		m_iQuestID;
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
		int		m_iGoingQuestID;
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
		int		m_iMinLv;
		int		m_iMaxLv;
		int		m_iQuantity;
		char    m_cUnitClass;
		float	m_fPcBangUserRate;

#ifdef	SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		int		m_iTitleID;
		int		m_iTitleID2;
		CXSLUnit::UNIT_TYPE		m_eUnitType;
		bool	m_bPartyPay;
		bool	m_bComeback;
		int		m_iEquippedItemID1;
		int		m_iEquippedItemID2;
		int		m_iUseItemID;
		bool	m_bMustParty;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		int		m_iConsumingItemID;
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
		int		m_iAllyNPCID;
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

		enum TITRATION_LEVEL
		{
			TL_MIN_LEVEL = 2,
			TL_MAX_LEVEL = 2,
		};

		//{{ 2012. 11. 05	박세훈	PaymentItem 기간 조건 추가
#ifdef SERV_PAYMENT_TIME_CONDITION
		CTime	m_tBeginDate;
		CTime	m_tEndDate;
#endif SERV_PAYMENT_TIME_CONDITION
		//}}

		DUNGEON_CLEAR_REWARD()
		{
			m_iDungeonID		= -1;
			m_iItemID			= 0;
			m_fRate				= 0.0f;
			m_iQuestID			= 0;
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
			m_iGoingQuestID		= 0;
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
			m_iMinLv			= 0;
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			m_iMaxLv			= 99;
			m_iTitleID			= 0;
			m_iTitleID2			= 0;
			m_eUnitType			= CXSLUnit::UT_NONE;
			m_bPartyPay			= false;
			m_bComeback			= false;
			m_iEquippedItemID1	= 0;
			m_iEquippedItemID2	= 0;
			m_iUseItemID		= 0;
			m_bMustParty		= false;
#else
			m_iMaxLv			= 0;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			m_iQuantity			= 1;
			m_cUnitClass		= CXSLUnit::UC_NONE;
			m_fPcBangUserRate	= 0.0f;

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
			m_iConsumingItemID	= 0;
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
			m_iAllyNPCID	= 0;
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC
		}

		DUNGEON_CLEAR_REWARD(	IN const int		iDungeonID,
								IN const int		iItemID,
								IN const float		fRate,
								IN const int		iQuestID,
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
								IN const int		iGoingQuestID,
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
								IN const int		iMinLv,
								IN const int		iMaxLv
								, IN const int		iQuantity
								, IN const char		cUnitClass
								, IN const float	fPcBangUserRate
#ifdef	SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
								, IN int		iTitleID
								, IN int		iTitleID2
								, IN CXSLUnit::UNIT_TYPE		eUnitType
								, IN bool	bPartyPay
								, IN bool	bComeback
								, IN int		iEquippedItemID1
								, IN int		iEquippedItemID2
								, IN int		iUseItemID
								, IN bool	bMustParty
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
								, IN bool	iConsumingItemID
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
								, IN bool	iAllyNPCID
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC
								)
								: m_iDungeonID( iDungeonID ),
								m_iItemID( iItemID ),
								m_fRate( fRate ),
								m_iQuestID( iQuestID ),
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
								m_iGoingQuestID( iGoingQuestID ),
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
								m_iMinLv( iMinLv ),
								m_iMaxLv( iMaxLv )
								, m_iQuantity( iQuantity )
								, m_cUnitClass( cUnitClass )
								, m_fPcBangUserRate( fPcBangUserRate )
#ifdef	SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
								, m_iTitleID( iTitleID )
								, m_iTitleID2( iTitleID2 )
								, m_eUnitType( eUnitType )
								, m_bPartyPay( bPartyPay )
								, m_bComeback( bComeback )
								, m_iEquippedItemID1( iEquippedItemID1 )
								, m_iEquippedItemID2( iEquippedItemID2 )
								, m_iUseItemID( iUseItemID )
								, m_bMustParty( bMustParty )
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
								, m_iConsumingItemID( iConsumingItemID )
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
								, m_iAllyNPCID( iAllyNPCID )
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC
		{
		}

		bool operator < ( IN const DUNGEON_CLEAR_REWARD& right ) const
		{
			if( m_iDungeonID < right.m_iDungeonID )			return true;
			else if( right.m_iDungeonID < m_iDungeonID )	return false;

			if( m_iItemID < right.m_iItemID )		return true;
			else if( right.m_iItemID < m_iItemID )	return false;

			if( m_fRate < right.m_fRate )		return true;
			else if( right.m_fRate < m_fRate )	return false;

			if( m_iQuestID < right.m_iQuestID )			return true;
			else if( right.m_iQuestID < m_iQuestID )	return false;

			if( m_iMinLv < right.m_iMinLv )			return true;
			else if( right.m_iMinLv < m_iMinLv )	return false;

			if( m_iMaxLv < right.m_iMaxLv )			return true;
			else if( right.m_iMaxLv < m_iMaxLv )	return false;

			if( m_iQuantity < right.m_iQuantity )		return true;
			else if( right.m_iQuantity < m_iQuantity )	return false;

			if( m_cUnitClass < right.m_cUnitClass )			return true;
			else if( right.m_cUnitClass < m_cUnitClass )	return false;

			if( m_fPcBangUserRate < right.m_fPcBangUserRate )		return true;
			else if( right.m_fPcBangUserRate < m_fPcBangUserRate )	return false;

#ifdef	SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
			if( m_iTitleID < right.m_iTitleID )		return true;
			else if( right.m_iTitleID < m_iTitleID )	return false;
			
			if( m_iTitleID2 < right.m_iTitleID2 )		return true;
			else if( right.m_iTitleID2 < m_iTitleID2 )	return false;
				
			if( m_eUnitType < right.m_eUnitType )		return true;
			else if( right.m_eUnitType < m_eUnitType )	return false;
				
			if( m_iEquippedItemID1 < right.m_iEquippedItemID1 )		return true;
			else if( right.m_iEquippedItemID1 < m_iEquippedItemID1 )	return false;
	
			if( m_iEquippedItemID2 < right.m_iEquippedItemID2 )		return true;
			else if( right.m_iEquippedItemID2 < m_iEquippedItemID2 )	return false;
			
			if( m_iUseItemID < right.m_iUseItemID )		return true;
			else if( right.m_iUseItemID < m_iUseItemID )	return false;
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
			if( m_iConsumingItemID < right.m_iConsumingItemID )		return true;
			else if( right.m_iConsumingItemID < m_iConsumingItemID )	return false;
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM

#ifdef SERV_PAYMENT_ITEM_WITH_ALLY_NPC
			if( m_iAllyNPCID < right.m_iAllyNPCID )		return true;
			else if( right.m_iAllyNPCID < m_iAllyNPCID )	return false;
#endif SERV_PAYMENT_ITEM_WITH_ALLY_NPC

			return false;
		}
	};
#else
	struct DUNGEON_CLEAR_REWARD
	{
		int		m_iItemID;
		float	m_fRate;
		int		m_iQuestID;
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
		int		m_iGoingQuestID;
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
		int		m_iMinLv;
		int		m_iMaxLv;
		int		m_iQuantity;
		char    m_cUnitClass;
		float	m_fPcBangUserRate;

		enum TITRATION_LEVEL
		{
			TL_MIN_LEVEL = 2,
			TL_MAX_LEVEL = 2,
		};

		DUNGEON_CLEAR_REWARD()
		{
			m_iItemID			= 0;
			m_fRate				= 0.0f;
			m_iQuestID			= 0;
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
			m_iGoingQuestID		= 0;
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
			m_iMinLv			= 0;
			m_iMaxLv			= 0;
			m_iQuantity			= 1;
			m_cUnitClass		= CXSLUnit::UC_NONE;
			m_fPcBangUserRate	= 0.0f;
		}
	};
#endif SERV_UNPAYMENT_CONDITION
	//}}
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

	//{{ 2011. 08. 23	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
	struct SHenirSpecialNpcDropInfo
	{
		std::map< int, int >	m_mapDropInfo;
	};
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	struct EIDESettingInfo	// EnchantItemDropEventSettingInfo
	{
		short			m_iMinLevel;
		short			m_iMaxLevel;
		KLottery		m_kLottery;
		std::set<char>	m_setNotify;

		EIDESettingInfo()
			: m_iMinLevel( 0 )
			, m_iMaxLevel( 0 )
		{
		}
	};

	struct EnchantItemDropEventInfo
	{
		CTime							m_tBeginDate;
		CTime							m_tEndDate;
		std::vector<EIDESettingInfo>	m_vecEIDESettingInfo;

		void	clear( void ){	m_vecEIDESettingInfo.clear();			}
		bool	empty( void ){	return m_vecEIDESettingInfo.empty();	}
	};
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

public:
	KDropTable(void);
	~KDropTable(void);

	//////////////////////////////////////////////////////////////////////////	
	// for lua
	void AddToGroup_LUA( int nGroupID, int nItemID, float fProbability );
	void AddMonsterDropInfo_LUA();	
	void AddHenirMonsterDropInfo_LUA(); // 헤니르 시공
	//{{ 2010. 05. 02  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	void AddExtraStageMonsterDropInfo_LUA();
#endif SERV_SECRET_HELL
	//}}
	//{{ 2009. 4. 21  최육사	전체드롭테이블
	void AddStaticDropInfo_LUA();
	//}}
	//{{ 2011. 11. 23	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void AddBattleFieldNpcDropInfo_LUA();
	void AddBattleFieldStaticDropInfo_LUA();
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2009. 6. 29  최육사	획득 공지
	bool AddNotifyDropItemInfo_LUA( int iItemID );
	void SetNotifyDropItem_LUA( bool bVal ) { m_bNotifyDropItem = bVal; }
	//}}
	//{{ 2009. 12. 18  최육사	던전경험치개편
#ifdef DUNGEON_EXP_NEW
	bool AddNpcExpTable_LUA( int iNpcLv, int iEXP );
#endif DUNGEON_EXP_NEW
	//}}
	//{{ 2011. 03. 28	최육사	서버 상태 체크
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
	void SetManyEDDropNpcEvent_LUA( bool bVal );
	bool AddManyEDDropNpc_LUA( int iNpcID, float fMultiplyDropED, float fMultiplyDropEDCount );
#endif SERV_MANY_ED_DROP_EVENT_NPC
	//}}

	//{{ 2011. 08. 23	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
	bool AddHenirSpecialNpcDropInfo_LUA( int iNpcID, int iItemID, int iQuantity );
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	bool AddPaymentDungeon_LUA( IN int iDungeonID );

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
	
	bool CheckDungeonClearPaymentItem( IN int iDungeonID, IN char cDifficulty, IN bool bExp, KRoomUserPtr spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		KRoomUserManagerPtr spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		OUT std::map< int, KItemInfo >& DungeonClearPaymentItem, IN bool bAraEvent = false );

#else //SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
	
	bool CheckDungeonClearPaymentItem( IN int iDungeonID, IN char cDifficulty, IN bool bExp, KRoomUserPtr spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		KRoomUserManagerPtr spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		OUT std::map< int, KItemInfo >& DungeonClearPaymentItem );

#endif //SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT

	bool GetPaymentItemInfo( const IN DUNGEON_CLEAR_REWARD& sData, IN bool bIsPcBang, OUT KItemInfo& kResultItemInfo  ) const;	
	void ProcessPaymentItem( IN const std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >::iterator& mit,
		//{{ 2012. 10. 17	박세훈	PaymentItem 금지 조건 추가
#ifdef SERV_UNPAYMENT_CONDITION
		IN const int iDungeonID,
		IN const char cDifficulty,
#endif SERV_UNPAYMENT_CONDITION
		//}}
		IN const KRoomUserPtr spRoomUser,
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		IN const KRoomUserManagerPtr spRoomUserManager,
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		OUT std::map< int, KItemInfo >& DungeonClearPaymentItem
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
		, IN const bool bAraEvent = false
#endif SERV_DUNGEON_CLEAR_PAYMENT_FOR_ARA_EVENT
		) const;

	//{{ 2012. 10. 17	박세훈	PaymentItem 금지 조건 추가
#ifdef SERV_UNPAYMENT_CONDITION
	bool AddUnPaymentCondition_LUA( IN int iDungeonID );
	bool CheckUnPaymentCondition( IN const DUNGEON_CLEAR_REWARD& kCheckValue ) const	{	return m_setUnPaymentCondition.find( kCheckValue ) != m_setUnPaymentCondition.end();	}
#endif SERV_UNPAYMENT_CONDITION
	//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	// 사용 아이템 체크 확인용
	bool CheckPaymentUseItem(IN const int iItemID );
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX

#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	bool GetComeBackBuffDropCount( IN const int& iRoomMember );
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	void	SetEnchantItemDropEvent_LUA( void );
	void	EnchantItemDropEventProcess( IN const int iItemID, OUT char& cEnchantLevel, OUT bool& bNotify );
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}

private:
	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	bool NpcDropItem( IN const int iNpcID, 
		IN const int iDropEXP, 
		IN const float fEDProperty, 
		IN const int iDropED, 
		IN const KLottery& kLot,
		IN const float fLevelFactor,
		IN const float fDungeonFactor,
		IN const float fContribution,
		IN const int iPartyUserCount_,	// kimhc // 김현철 // 2013-12-16 // 기여도 변경 작업
		IN const float fPartyDropBonus,
		//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
		IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
		//}}
		OUT DROP_DATA& sDropData,
		IN const bool bIsBoss );
#else
	bool NpcDropItem( IN const int iNpcID, IN const int iDropEXP, IN const float fEDProperty, IN const int iDropED, IN const KLottery& kLot, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum, IN const bool bIsBoss );
#endif SERV_REFORM_ITEM_DROP
	//}}

#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
	bool _CheckTitlePass( IN KRoomUserInfo& sRoomUserInfo, IN KRoomUserManagerPtr spRoomUserManager, IN const DUNGEON_CLEAR_REWARD& dcr ) const;
	bool _CheckEquippedItemPass( IN KRoomUserInfo& sRoomUserInfo, IN KRoomUserManagerPtr spRoomUserManager, IN const DUNGEON_CLEAR_REWARD& dcr ) const;
	bool _CheckEquippedItem(IN KRoomUserInfo& sRoomUserInfo, IN const int iItemID) const;
	bool _CheckPaymentCondition(IN const DUNGEON_CLEAR_REWARD& dcr, KRoomUserPtr spRoomUser, IN KRoomUserManagerPtr spRoomUserManager ) const;
	bool _CheckPaymentConditionDetail(IN const DUNGEON_CLEAR_REWARD& dcr, IN KRoomUserInfo& sRoomUserInfo, IN KRoomUserManagerPtr spRoomUserManager ) const;
	bool _CheckPaymentUseItemDetail( std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >::iterator& mit, IN const int iItemID ) const;
#endif	
	
public:

	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	bool NormalNpcDropItem( IN const int nDungeonID, 
		IN const int iNpcID,
		IN const char cNpcLevel,
		IN const float fLevelFactor,
		IN const float fDungeonFactor, 
		IN const float fContribution,
		IN const float fPartyDropBonus,
		IN const int iPartyUserCount,
		//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
		IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
		//}}
		OUT bool& bDecreaseEndurance,
		OUT DROP_DATA& sDropData, 
		IN const bool bIsBoss = false );

	bool HenirNpcDropItem( IN const CXSLDungeon::DUNGEON_MODE eDungeonMode, 
		IN const int iNpcID,
		IN const char cNpcLevel,
		IN const float fDungeonFactor, 
		IN const float fContribution,
		IN const float fPartyDropBonus,
		IN const int iPartyUserCount,
		//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
		IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
		//}}
		OUT bool& bDecreaseEndurance,
		OUT DROP_DATA& sDropData,  
		IN const bool bIsBoss = false );

	bool ExtraStageNpcDropItem( IN const int iNpcID, 
		IN const char cNpcLevel,
		IN const float fLevelFactor,
		IN const float fDungeonFactor, 
		IN const float fContribution,
		IN const float fPartyDropBonus,
		IN const int iPartyUserCount,
		//{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		IN const float fDropRate,
#else // SERV_DROP_EVENT_RENEWAL
		IN const int iDropCount,
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
		//}}
		OUT bool& bDecreaseEndurance,
		OUT DROP_DATA& sDropData,
		IN const bool bIsBoss = false );
#else
	bool NormalNpcDropItem( IN const int nDungeonID, IN const int iNpcID, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum = 1, IN const bool bIsBoss = false );
	bool HenirNpcDropItem( IN const CXSLDungeon::DUNGEON_MODE eDungeonMode, IN const int iNpcID, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN int const iItemDropNum, IN const bool bIsBoss );
	//{{ 2010. 05. 02  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	bool ExtraStageNpcDropItem( IN const int iNpcID, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum = 1, IN const bool bIsBoss = false );
#endif SERV_SECRET_HELL
	//}}
#endif SERV_REFORM_ITEM_DROP
	//}}
	//{{ 2012. 11. 12	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	void NpcSpecialDropItem( IN const int iNpcID, IN const char cMonsterGrade, IN const char cNpcDieState, IN OUT std::vector< int >& vecItemID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 2009. 5. 13  최육사	서브스테이지별 경험치 주는 npc수
    bool IsExpDropNpc( IN int nDungeonID, IN int nMonsterID );
	//}}
	//{{ 2010. 01. 05  최육사	내구도개편
	bool IsHenirExpDropNpc( IN CXSLDungeon::DUNGEON_MODE eDungeonMode, IN int nMonsterID );
	//}}

	//{{ 2009. 4. 22  최육사	전체드롭
	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	bool StaticDropItem( IN int nDungeonID, OUT DROP_DATA& sDropData, IN float fUserContribution );
#else
	bool StaticDropItem( IN int nDungeonID, OUT DROP_DATA& sDropData, IN int iItemDropNum = 1 );
#endif SERV_REFORM_ITEM_DROP
	//}}

	//}}
	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	bool EventDropItem( IN float fUserContribution, OUT DROP_DATA& sDropData );
#else
	bool EventDropItem( IN int iItemDropNum, OUT DROP_DATA& sDropData );
#endif SERV_REFORM_ITEM_DROP
	//}}
	bool DropGroupItem( IN int iDropGroupID, OUT int& iDropItemID );
	//{{ 2009. 6. 17  최육사	대전드롭
	bool PvpDropItem( IN int iItemDropNum, OUT DROP_DATA& sDropData );
	//}}

	//{{ 2009. 5. 23  최육사	시간 드롭
	void SetTimeDropInfo( IN const std::vector< KTimeDropInfo >& vecTimeDropInfo, IN const std::wstring& wstrServerIP );
	bool CheckTimeDrop( OUT int& iTimeDropID, OUT int& iDropItemID );
	bool RollbackTimeDrop( IN int iTimeDropID );
	//}}

	//{{ 2009. 6. 29  최육사	드롭 획득 공지
	bool IsNotifyDropItem( IN int iItemID );
	//}}
	//{{ 2011. 11. 22	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	//{{ 필드 드롭 개편 - 김민성
//#ifdef SERV_REFORM_ITEM_DROP
	bool GetBattleFieldNpcDropInfo( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
									IN const int iNpcID, 
									OUT KBattleFieldNpcDropInfo& kDropInfo );

	bool BattleFieldNpcDropItem( IN const KBattleFieldNpcDropInfo& kDropInfo
		//{{ 2013. 03. 26	 필드 ED, EXP 팩터 추가 - 김민성
#ifdef SERV_FIELD_ED_EXP_FACTOR
							   , IN const float fFieldEDFactor
							   , IN const float fFieldEXPFactor
#else
							   , IN const float fFieldFactor
#endif SERV_FIELD_ED_EXP_FACTOR
		//}
							   , IN const float fLevelFactor
							   , IN const float fUserContribution
							   , IN const int iPartyCount_	// kimhc // 김현철 // 2013-12-16 // 기여도 변경 작업
							   , IN const float fPartyDropBonus
								 //{{ 2012. 12. 16  드롭 이벤트 - 김민성
#ifdef SERV_ITEM_DROP_EVENT
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
							   , IN const float fDropRate
#else // SERV_DROP_EVENT_RENEWAL
							   , IN const int iDropCount
#endif // SERV_DROP_EVENT_RENEWAL
#endif SERV_ITEM_DROP_EVENT
								 //}}
							   , IN const bool bIsBoss
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-14	// 박세훈
							   , IN const float fBattleFieldBossFactor
#endif // SERV_BATTLE_FIELD_BOSS
							   , OUT DROP_DATA& sDropData
							   );

	bool BattleFieldStaticDropItem( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, 
									OUT DROP_DATA& sDropData, 
									IN const float fUserContribution );
//#else
//	bool BattleFieldNpcDropItem( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, IN const int iNpcID, IN const char cNpcLevel, IN const char cMonsterTypeFactor, OUT DROP_DATA& sDropData, IN const int iItemDropNum = 1, IN const bool bIsBoss = false );
//	bool BattleFieldStaticDropItem( IN const SEnum::BATTLE_FIELD_ID eBattleFieldID, OUT DROP_DATA& sDropData, IN int iItemDropNum = 1 );
//#endif SERV_REFORM_ITEM_DROP
	//}}
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

public:
    // load된 데이터 확인용 함수.
    void	DumpGroup();
    void	DumpMonsterDrop();

#ifdef DUNGEON_ITEM
public:
	void	SetPVPFailRate_LUA( IN int iEnumKey, IN float fRate );
	void	SetPVPItemChangeTime_LUA( float fTime );
	void	AddPVPDropInfo_LUA( IN int iGroup, IN int iItemID, IN float fDropRate );
	bool	DecisionbyAllItem( OUT int& iGroup, OUT int& iItemID );
	bool	DecisionbyGroupItem( IN int iGroup, OUT int& iItemID );
	float	GetPVPDropFailRate( PVP_FAILRATE eKey );
	double  GetPVPItemChangeTime(){ return m_dfPVPItemChangeTime; }
	bool	CheckPVPItem( int iItemID );

	//{{ 2009. 12. 18  최육사	던전사용아이템
	bool	AddAttribNpcSpecialDrop_LUA( IN char cDifficulty, IN int iItemID, IN float fDropRate );
	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	bool	AttribNpcSpecialDrop( IN char cDifficulty, IN float fUserContribution ,OUT std::vector< int >& vecDropItem );
#else
	bool	AttribNpcSpecialDrop( IN char cDifficulty, OUT std::vector< int >& vecDropItem );
#endif SERV_REFORM_ITEM_DROP
	//}}
	//}}
#endif DUNGEON_ITEM

	//{{ 2009. 12. 18  최육사	던전경험치개편
#ifdef DUNGEON_EXP_NEW
	int		GetNpcExp( IN char cNpcLv );
#endif DUNGEON_EXP_NEW
	//}}

	//{{ 2011. 08. 23	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
	//{{ 필드 드롭 개편 - 김민성
#ifdef SERV_REFORM_ITEM_DROP
	void	GetHenirSpecialNpcDropInfo( IN const int iNpcID,IN float fTotalDropRate, OUT DROP_DATA& sDropDataHenir );
#else
	void	GetHenirSpecialNpcDropInfo( IN const int iNpcID, OUT DROP_DATA& sDropDataHenir );
#endif SERV_REFORM_ITEM_DROP
	//}}
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2011. 03. 28	최육사	서버 상태 체크
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
	bool	IsManyEDDropNpcEvent() const { return m_bIsDropManyEDEvent; }
	bool	GetManyEDDropNpcInfo( IN const int iNpcID, OUT KManyEDDropNpcInfo& kInfo );
#endif SERV_MANY_ED_DROP_EVENT_NPC
	//}}

	//{{ 2012. 07. 17	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2_2
	float CalculateRebirthTime( IN const int& iMyTeamKillNum
							  , IN const int& iEnemyTeamKillNum
							  , IN const bool& bDidIHaveARebirthRing );

	bool	SuccessfullyGetTheItem( IN const CXSLRoom::PVP_GAME_TYPE& ePvpGameType, IN const int& iMyTeamKillNum, IN const int& iEnemyTeamKillNum );
#endif SERV_2012_PVP_SEASON2_2
	//}}

protected:	
    int		GetEDItemID( int nED ) const; // ED양에 맞는 ItemID를 반환한다.

protected:
	std::map< std::pair< int, int >, KMonsterDropInfo >	m_mapMonsterDrop;   //Key(1:DungeonID,2:MonsterID)
    std::map< int, KLottery >							m_mapDropGroup;
	//{{ 2009. 4. 21  최육사	전체드롭테이블
	std::map< int, KStaticDropInfo >					m_mapStaticDrop;
	//}}

	//{{ 2009. 7. 25  최육사	헤니르 시공
	std::map< std::pair< int, int >, KHenirNpcDropInfo > m_mapHenirNpcDrop;
	//}}

	//{{ 2010. 05. 02  최육사	비밀던전 헬모드
#ifdef SERV_SECRET_HELL
	std::map< int, KExtraStageNpcDropInfo >				m_mapExtraStageNpcDrop;
#endif SERV_SECRET_HELL
	//}}

	//{{ 2009. 5. 23  최육사	시간 드롭	
	std::vector< KTimeDropItemInfo >					m_vecTimeDrop;
	std::map< int, KTimeDropItemInfo >					m_mapTimeDrop;
	//}}

	//{{ 2009. 6. 17  최육사	대전 드롭
	KLottery											m_kPvpDrop;
	//}}

	//{{ 2009. 6. 29  최육사	드롭 획득 공지
	std::set< int >										m_setNotifyDropItem;
	bool												m_bNotifyDropItem;
	//}}

    friend void TestItemDrop(); // for unit testing.

#ifdef DUNGEON_ITEM
	std::vector< int >									m_vecPVPDropItem;	// 대전에서 드롭되는 모든 아이템 아이디(식별용)
	std::map< int , KLottery >							m_mapPVPDropItem;	// 대전 랜던 드롭을 위해.
	float	m_fPVPFailRate[PF_END];
	double	m_dfPVPItemChangeTime;
	//{{ 2009. 12. 18  최육사	던전사용아이템
	std::map< char, KLottery >							m_mapAttribNpcSPDrop; // 속성 몬스터 스페셜 드롭
	//}}
#endif DUNGEON_ITEM

	//{{ 2009. 12. 18  최육사	던전경험치개편
#ifdef DUNGEON_EXP_NEW
	std::map< int, int >								m_mapNpcExpTable;	// npc경험치 테이블
#endif DUNGEON_EXP_NEW
	//}}
	//{{ 2011. 03. 28	최육사	서버 상태 체크
#ifdef SERV_MANY_ED_DROP_EVENT_NPC
	bool												m_bIsDropManyEDEvent;
	std::map< int, KManyEDDropNpcInfo >					m_mapManyEDDropTable;
#endif SERV_MANY_ED_DROP_EVENT_NPC
	//}}

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	std::map< int, std::vector< DUNGEON_CLEAR_REWARD > >	m_mapPaymentItemTable;   // < 던전 , <퀘스트 , 아이템목록 >>

	//{{ 2012. 10. 17	박세훈	PaymentItem 금지 조건 추가
#ifdef SERV_UNPAYMENT_CONDITION
	std::set<DUNGEON_CLEAR_REWARD>							m_setUnPaymentCondition;
#endif SERV_UNPAYMENT_CONDITION
	//}}

#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	std::map< int, float >								m_mapComeBackBuffDropRate;   // < 파티 인원 , 드롭 확률>
#endif SERV_COME_BACK_USER_REWARD
	//}}
	//{{ 2011. 08. 23	최육사	헤니르 시공 개편
#ifdef SERV_NEW_HENIR_TEST
	std::map< int, SHenirSpecialNpcDropInfo >			m_mapHenirSpecialNpcDrop;	// 헤니르시공 NPC 특수 드롭
#endif SERV_NEW_HENIR_TEST
	//}}
	//{{ 2011. 11. 23	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::map< std::pair< int, int >, KBattleFieldNpcDropInfo >	m_mapBattleFieldNpcDrop;   //Key(1:BattleFieldID,2:MonsterID)
	std::map< int, KBattleFieldStaticDropInfo >					m_mapBattleFieldStaticDrop;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
	EnchantItemDropEventInfo							m_kEnchantItemDropEventInfo;
#endif SERV_ENCHANT_ITEM_DROP_EVENT
	//}}
};

DefRefreshSingletonInline( KDropTable );


