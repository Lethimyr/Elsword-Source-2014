#pragma once

#include "RefreshSingleton.h"
#include "XSLItem.h"
#include "Lottery.h"
#include "XSLUnitManager.h"



class CXSLItemManager
{
	DeclareRefreshSingleton( CXSLItemManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;

public:
	enum ITEM_EXCHANGE_TYPE
	{
		IET_INVALID = 0,
		IET_ALL,
		IET_RANDOM,
		IET_SELECT,
	};

	//{{ 2012. 10. 09	김민성	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
	enum ITEM_ENCHANT_STANDARD_VALUE
	{
		IESV_PHYSIC_ATTACK	= 120,		// 아이템 1레벨 물공 증가치 (이만큼이 증가해야 1레벨이 오름)
		IESV_MAGIC_ATTACK	= 120,		// 아이템 1레벨 마공 증가치	(이만큼이 증가해야 1레벨이 오름)
		IESV_PHYSIC_DEFENCE	= 30,		// 아이템 1레벨 물방 증가치	(이만큼이 증가해야 1레벨이 오름)
		IESV_MAGIC_DEFENCE	= 30,		// 아이템 1레벨 마방 증가치	(이만큼이 증가해야 1레벨이 오름)
		IESV_BASE_HP		= 3000,		// 아이템 1레벨 HP 증가치	(이만큼이 증가해야 1레벨이 오름)
	};
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
	//}}

	typedef std::pair< int, int > ItemExchangeKey; // key[ HOUSE_ID, SrcItemID ]

	struct ItemExchangeData
	{
		ItemExchangeData()
		{
			m_eExchangeType = IET_INVALID;
		}

		bool AddExchangeData( IN const ITEM_EXCHANGE_TYPE eExchangeType,
			IN const int iDestItemID,
			IN const float fDestItemRate,
			IN const int iDestQuantity
#ifdef SERV_EXCHANGE_PERIOD_ITEM
			, IN const short sPeriod
#endif //SERV_EXCHANGE_PERIOD_ITEM
			);

		ITEM_EXCHANGE_TYPE					m_eExchangeType;
		std::map< int, int >				m_mapDestItemID;
		KLottery							m_kLottery;
#ifdef SERV_EXCHANGE_PERIOD_ITEM
		std::map< int, short >				m_mapDestItemPeriod;
#endif //SERV_EXCHANGE_PERIOD_ITEM
	};

	//{{ 2011. 06. 18	최육사	ED아이템 구매 체크
#ifdef SERV_CHECK_BUY_ED_ITEM
	struct ItemIDList
	{
		std::vector< int >	m_ItemIDList;
	};

	struct NpcItemIDList
	{
		int					m_NpcId;
		std::vector< int >	m_ItemIDList;
	};
#endif SERV_CHECK_BUY_ED_ITEM
	//}}

	//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	struct SItemConvertInfo 
	{
		int						m_iConverType;
		KLotterySameProb< int >	m_kLotResultItemID;
		int						m_iCommission;

		SItemConvertInfo()
		{
			m_iConverType = 0;
			m_iCommission = 0;
		}
	};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	struct tgaDisCountItem
	{
		int					m_DisCountProNo;
		int					m_BasicProNo;
		std::vector<int>	m_DisCountKeepItem;
	};
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

public:
	CXSLItemManager(void);
	~CXSLItemManager(void);

	// for lua
	bool AddItemTemplet_LUA();
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	bool AddItemTempletTrans_LUA();
#endif SERV_ITEM_LUA_TRANS_DEVIDE
	bool AddItemExchangeData_LUA( int iHouseID, ITEM_EXCHANGE_TYPE eExchangeType, int iSrcItemID, int iDestItemID, int iQuantity, float fDestItemRate );
	//{{ 2010. 9. 1	최육사	기간 초기화 아이템
#ifdef SERV_RESET_PERIOD_EVENT
	bool AddItemExpandPeriodData_LUA( int iItemID, short sPeriod );
#endif SERV_RESET_PERIOD_EVENT
	//}}
	//{{ 2010. 11. 29	최육사	기간제템 삭제
#ifdef SERV_DELETE_PERIOD_ITEM
	bool AddDeleteItemData_LUA( int iItemID );
#endif SERV_DELETE_PERIOD_ITEM
	//}}
	//{{ 2011. 06. 18	최육사	ED아이템 구매 체크
#ifdef SERV_CHECK_BUY_ED_ITEM
	bool AddShopItemList_LUA();
	bool AddShopItemGroup_LUA();
#endif SERV_CHECK_BUY_ED_ITEM
	//}}
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool AddCoolTimeGroupItem_LUA( int iCoolTimeGroupEnum, int iItemID );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	void AddWeddingHallItem_LUA( int iWeddinHallType, int iItemID );
	int GetWeddingHallItem( const char cWeddingHall );

	void AddWeddingCompleteRewardItem_LUA( IN int iItemID, IN int iCount );
	void GetWeddingCompleteRewardItem( OUT std::map<int, int>& mapItemList );
	void AddCoupleAnniversaryDayRewardItem_LUA( int iItemID );
	void GetCoupleAnniversaryDayRewardItem( OUT std::vector<int>& vecItemList );
	void AddWeddingAnniversaryDayRewardItem_LUA( int iItemID );
	void GetWeddingAnniversaryDayRewardItem( OUT std::vector<int>& vecItemList );
	void AddWeddingAnniversaryDayRewardTitle_LUA( int iDays, int iItemID );
	int GetWeddingAnniversaryDayRewardTitle( const int iDays );
#endif SERV_RELATIONSHIP_SYSTEM
	//}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	bool AddKeepShowItem_LUA(void);
	bool IsKeepItemShowItem( const int iItemID );
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_WISH_LIST_NO_ITEM
	void AddWishListNoItem_LUA();
	bool IsWishListNoItem( const int iItemID );
#endif //SERV_WISH_LIST_NO_ITEM

	// 아이템 정보
	const CXSLItem::ItemTemplet* GetItemTemplet( const int itemID ) const;
	const std::map< int, CXSLItem::ItemTemplet >& GetItemTempletContainer() const { return m_ItemTempletIDMap; }

#ifdef SERV_USE_GM_TOOL_INFO
	const std::map< int, KItemName >& GetItemNameContainer() const { return m_ItemTempletNameMap; }
#endif //SERV_USE_GM_TOOL_INFO

	// 아이템 교환	
	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	bool GetItemExchangeResult( IN const int iHouseID, 
		IN const int iSrcItemID, 
		IN const int iSrcQuantity, 
		IN const KItemInfo& kSrcItemInfo,
		IN const int iDestItemID, 
		OUT std::map< int ,KItemInfo >& mapResultItem,
		OUT bool& bRandomExchange,
		OUT char& cExchangeType);
#else
	//{{ 2011. 08. 16	최육사	아이템 교환 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
	bool GetItemExchangeResult( IN const int iHouseID, 
		IN const int iSrcItemID, 
		IN const int iSrcQuantity, 
		IN const KItemInfo& kSrcItemInfo,
		IN const int iDestItemID, 
		OUT std::map< int ,KItemInfo >& mapResultItem,
		OUT bool& bRandomExchange);
#else
	bool GetItemExchangeResult( int iHouseID, int iSrcItemID, int iSelectedItemID, std::map< int ,KItemInfo >& mapResultItem );
#endif SERV_ITEM_EXCHANGE_NEW
	//}}	
#endif SERV_EXCHANGE_LOG
	//}

	// 봉인 가능 아이템
	bool IsCanSealForCashItem( int iItemID );
	bool IsCanSealForRandomItem( int iItemID );

	//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	bool IsSkillNoteMemoItem( int iItemID );
#endif SERV_SKILL_NOTE
	//}}

	//{{ 2010. 9. 1	최육사	기간 초기화 아이템
#ifdef SERV_RESET_PERIOD_EVENT
	bool IsEmptyExpandPeriodItemList() const			{ return m_mapExpandPeriodItem.empty(); }
	bool GetExpandPeriodByItemID( IN int iItemID, OUT short& sExpandPeriod ) const;
#endif SERV_RESET_PERIOD_EVENT
	//}}

	//{{ 2010. 11. 29	최육사	기간제템 삭제
#ifdef SERV_DELETE_PERIOD_ITEM
	bool IsForceDeleteItem( int iItemID ) const			{ return ( m_setDeleteItemData.find( iItemID ) != m_setDeleteItemData.end() ); }
#endif SERV_DELETE_PERIOD_ITEM
	//}}

	//{{ 2011. 05. 23  김민성	큐피엘 젤리 친밀도 증감
#ifdef SERV_ADD_KEWPIEL_JELLY
	bool IsFantasticKewpielJelly( const int iItemID );
	bool IsOddKewpielJelly( const int iItemID );
#endif SERV_ADD_KEWPIEL_JELLY
	//}}

	//{{ 2011. 06. 18	최육사	ED아이템 구매 체크
#ifdef SERV_CHECK_BUY_ED_ITEM
	bool IsNpcSellEDItem( IN const int iItemID ) const { return ( m_setNpcSellEDItemList.find( iItemID ) != m_setNpcSellEDItemList.end() ); }
#endif SERV_CHECK_BUY_ED_ITEM
	//}}

	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	bool IsCanSealForSynthesis( int iItemID );
#endif SERV_SYNTHESIS_AVATAR
	//}}

	//{{ 101020 김정협 트레져박스여부 확인
#ifdef SERV_TREASURE_BOX_ITEM
	bool IsTreasureBox( int iBoxID , int iItemID);
#endif SERV_TREASURE_BOX_ITEM
	//}} 101020 김정협 트레져박스여부 확인

	//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	void UpdateSetIDAndMaxLevelMap( const int iSetID, const int iLevel );
	int  GetSetItemMaxLevel( const int iSetID_ );
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 

	//{{ 2011. 10. 18	최육사	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
	//{{ 2013. 05. 15	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	bool GetItemLevel( IN const char cUnitType, IN const char cUnitClass, IN const KItemInfo& kItemInfo, OUT int& iItemLevel );
#else
	bool GetItemLevel( IN const KItemInfo& kItemInfo, OUT int& iItemLevel );
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
	//}}

	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool IsCanUseQuickSlotItemInVillage( IN const int iItemID ) const;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	bool IsBuffItem( IN int iItemID );
#endif SERV_SERVER_BUFF_SYSTEM
	//}}
	//{{ 2013. 05. 20	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	void ItemSealProcess( IN const int iItemID
		, IN const short sPeriod
		, OUT unsigned char& ucSealData
		) const;

	// 아이템 교환
	bool AddItemConvertGroup_LUA( int iGroupID, int iItemID );
	bool AddItemConvertInfo_LUA();
	bool GetResultItemConvert( IN const int iTargetItemID, OUT int& iResultItemID, OUT int& iCommissionED ) const;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013// 작업날짜: 2013-06-25	// 박세훈
	int	GetItemCSPoint( IN const int iItemID );
	int	GetItemCSPointPeriod( IN const int iItemID );
#endif // SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_RESTRICTED_TO_MOVE_TO_BANK
	bool IsInventoryOnly( IN const int iItemID );
#endif SERV_RESTRICTED_TO_MOVE_TO_BANK

protected:
	//{{ 2011. 10. 18	최육사	배틀필드 아이템 레벨 시스템
#ifdef SERV_BATTLE_FIELD_ITEM_LEVEL
	void GetCaculatedItemLevelByEnchantStat( IN const KStat& kEnchantItemStat, OUT int& iItemLevel );	
#endif SERV_BATTLE_FIELD_ITEM_LEVEL
	//}}

private:
	std::map< int, CXSLItem::ItemTemplet >			m_ItemTempletIDMap;		// 아이템 템플릿

#ifdef SERV_USE_GM_TOOL_INFO
	std::map< int, KItemName >						m_ItemTempletNameMap;
#endif //SERV_USE_GM_TOOL_INFO

	//{{ 2011. 08. 08	최육사	아이템 교환 시스템 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
	typedef std::map< ItemExchangeKey, ItemExchangeData >	ItemExchangeTable;
	std::map< int, ItemExchangeTable >				m_mapItemExchangeTable;	// 교환 아이템
#else	
	std::map< ItemExchangeKey, ItemExchangeData >	m_mapItemExchangeData;	// 교환 아이템
#endif SERV_ITEM_EXCHANGE_NEW
	//}}	
	//{{ 2010. 9. 1	최육사	기간 초기화 아이템
#ifdef SERV_RESET_PERIOD_EVENT
	std::map< int, short >							m_mapExpandPeriodItem;	// 기간 초기화 아이템
#endif SERV_RESET_PERIOD_EVENT
	//}}
	//{{ 2010. 11. 29	최육사	기간제템 삭제
#ifdef SERV_DELETE_PERIOD_ITEM
	std::set< int >									m_setDeleteItemData;
#endif SERV_DELETE_PERIOD_ITEM
	//}}
	//{{ 2011. 06. 18	최육사	ED아이템 구매 체크
#ifdef SERV_CHECK_BUY_ED_ITEM
	std::set< int >									m_setNpcSellEDItemList; // NPC가 판매하는 ED아이템 리스트
#endif SERV_CHECK_BUY_ED_ITEM
	//}}
	//{{ 2011. 07. 08    김민성    옵션 수치화
#ifdef SERV_USE_PERCENT_IN_OPTION_DATA
	std::map<int, int>								m_mapSetIDAndMaxLevel;	// 셋트의 최고 레벨
#endif SERV_USE_PERCENT_IN_OPTION_DATA
	//}} 
	//{{ 2012. 04. 04	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	std::map< int, CXSLItem::COOLTIME_ITEM_GROUP >	m_mapCoolTimeItemGroup;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	std::map<char, int>								m_mapWeddingHallTypeItem;	// [타입,itemid]
	std::map< int, int >							m_mapWeddingCompleteRewardItem;
	std::vector< int >								m_vecCoupleAnniversaryDayRewardItem;
	std::vector< int >								m_vecWeddingAnniversaryDayRewardItem;
	std::map< int, int >							m_mapWeddingAnniversaryDayRewardTitle;	// [days, itemid]
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	//{{ 2013. 05. 21	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	std::map< int, std::vector< int > >				m_mapItemConvertGroup;
	std::map< int, SItemConvertInfo >				m_mapItemConvertTable;
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	public:
		int							m_InfoType;
		int							m_ShowItem;
		std::vector< int >			m_vecKeepItem_Key;
		std::map< int,std::vector< int > >	m_mapKeepShowItem;
		std::map< int, kDisCountItemInfo > m_DisCountInfoMap;
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP

#ifdef SERV_WISH_LIST_NO_ITEM
		std::set< int >									m_setWishListNoItemList; 
#endif SERV_WISH_LIST_NO_ITEM
};

DefRefreshSingletonInline( CXSLItemManager );


