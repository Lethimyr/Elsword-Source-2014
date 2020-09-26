#pragma once

#include "RefreshSingleton.h"
#include "Lottery.h"
#include "XSLItem.h"

#ifdef SERV_CUBE_IN_ITEM_MAPPING
struct RANDOMITEM_MAPPING_DATA
{
	int		 iCubeID;
	int		 iKeyItemID;
	int		 iBeforeGroupID;
	int		 iAfterGroupID;
	bool	 bCheckMapping;

	RANDOMITEM_MAPPING_DATA()
	{
		iCubeID				= 0;
		iKeyItemID			= 0;
		iBeforeGroupID		= 0;
		iAfterGroupID		= 0;
		bCheckMapping		= false;
	}
};

struct RANDOMITEM_MAPPING_TIME
{
	CTime	 tStartTime;
	CTime	 tEndTime;
};
#endif SERV_CUBE_IN_ITEM_MAPPING

class CXSLRandomItemManager
{
	DeclareRefreshSingleton( CXSLRandomItemManager );
	DeclareLuaScriptParser;
	DeclDumpToLogFile;
	DeclDump;
	DeclToStringW;
public:

	enum RANDOM_ITEM_ID
	{
#ifdef SERV_COUNTRY_JP	
        RID_ATTRACTION_ITEM					= 65000031,	// 임규수 일본 변경 108900
		RID_ATTRACTION_ITEM_EVENT			= 60000702, // 임규수 일본 변경 108901
#else //SERV_COUNTRY_JP
        RID_ATTRACTION_ITEM					= 108900,
		RID_ATTRACTION_ITEM_EVENT			= 108901,
#endif //SERV_COUNTRY_JP
		RID_ATTRACTION_ITEM_ARCHANGEL_CHUNG	= 108902,   // 청 아크엔젤 - 봉인된 얼음 조각상
		RID_ATTRACTION_ITEM_SALVATOR_CHUNG	= 160259,   // 청 살바토르 - 봉인된 얼음 조각상
		RID_ATTRACTION_ITEM_OFFICER_CHUNG	= 160286,   // 청 장교 - 봉인된 얼음 조각상(솔레스용)
		RID_ATTRACTION_ITEM_OFFICER			= 160287,   // 장교 - 봉인된 얼음 조각상(가이아용)
		RID_ATTRACTION_ITEM_NASOD_BATTLE_CHUNG	= 160324,   // 청 나소드 배틀 - 봉인된 얼음 조각상(솔레스용)
		RID_ATTRACTION_ITEM_NASOD_BATTLE	= 160325,   // 나소드 배틀 - 봉인된 얼음 조각상(가이아용)
		RID_ATTRACTION_ITEM_ROYAL_SERVENT_CHUNG		= 160377,		// 봉인된 얼음 조각상(청 로열 서번트)
		//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		RID_ATTRACTION_ITEM_GOLD			= 135115,   // 봉인된 얼음 조각상(GOLD)
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}
		//{{ 2012. 09. 04	박세훈	봉인된 얼음 조각상(청 아크데빌)
#ifdef SERV_ATTRACTION_ITEM_ARCHDEVIL
		RID_ATTRACTION_ITEM_ARCHDEVIL_CHUNG	= 160559,   // 청 아크데빌 - 봉인된 얼음 조각상
#endif SERV_ATTRACTION_ITEM_ARCHDEVIL
		//}}
		//{{ 2012. 11. 14	박세훈	봉인된 얼음 조각상(청 벨더 임페리얼 가드)
#ifdef SERV_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD
		RID_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD	= 160597,   // 봉인된 얼음 조각상(청 벨더 임페리얼 가드)
#endif SERV_ATTRACTION_ITEM_CHUNG_VELDER_IMPERIAL_GUARD
		//}}
		// 이 큐브의 경우에는 차가운 크리스탈을 보너스로 지급하지 않는다. ( 기타 큐브의 경우에는 지급하고 있으니 이 점을 고려할 것 )
		RID_ATTRACTION_ITEM_9_10_ENCHANT_ITEM	= 160576,   // 강화의 부적 Lv.9~10큐브
		RID_2012_WINTER_VACATION_LOTTO_CUBE		= 160630,	// 겨울 방학 대박 큐브
		RID_ATTRACTION_ITEM_ARCHANGEL_ARA		= 160643,   // 봉인된 얼음 조각상(아라 아크엔젤)	- 박세훈
		RID_ATTRACTION_ITEM_ELSWORD_5YEAR_CUBE	= 160644,   // 엘소드 5주념 기념 큐브	- 박세훈
		RID_ATTRACTION_ITEM_2013_D4_CUBE		= 160645,   // 2013년 D-4 현물 경품 랜덤 큐브	- 박세훈
		RID_ATTRACTION_ITEM_2013_D2_CUBE		= 160647,   // 2013년 D-2 캐시 액세서리 랜덤 큐브	- 박세훈
		RID_ATTRACTION_ITEM_2012_GOODBYE_CUBE	= 160648,   // 굿바이 2012년 큐브	- 박세훈
		
		//{{ 2012. 12. 28  미니 얼음 조각상 가열기 - 김민성
#ifdef SERV_SERV_MINI_RANDOM_CUBE_REWARD
		RID_MINI_ATTRACTION_ITEM				= 160656,	// 미니 봉인된 얼음 조각상
#endif SERV_SERV_MINI_RANDOM_CUBE_REWARD
		//}}
		//{{ 2012. 10. 08  할로윈 가열기
#ifdef SERV_HALLOWEEN_ICE_HEATER
		RID_HALLOWEEN_ICE_HEATER_ITEM			= 85003498,	// 할로윈 몬스터의 영혼
		RID_SHINE_HALLOWEEN_ICE_HEATER_ITEM		= 85003500,	// 빛나는 할로윈 몬스터의 영혼
#endif SERV_HALLOWEEN_ICE_HEATER
		//}}
		RID_ATTRACTION_ITEM_EVELUN_ARA			= 160666,   // 봉인된 얼음 조각상(아라 살바토르 이벨른)	- 박세훈

		//{{ 2013. 02. 04  특정시간 가열기 확률 변경 - 김민성
#ifdef SERV_ATTRACTION_ITEM_TIME_EVENT
		RID_ATTRACTION_ITEM_GLACIAL_EVENT			= 160683,   // 봉인된 얼음 조각상(글레이셜 2배용)
		RID_ATTRACTION_ITEM_EVELUN_ARA_EVENT		= 160684,   // 봉인된 얼음 조각상(아라 살바토르 이벨른 2배용)
#endif SERV_ATTRACTION_ITEM_TIME_EVENT
		//}
		//{{ 2013. 2. 21	박세훈	2013년 02월 21일 패치
#ifdef SERV_ATTRACTION_ITEM_GOLD_CONDOR
		RID_ATTRACTION_ITEM_GOLD_CONDOR		= 160710,   // 봉인된 얼음 조각상( 황금 콘돌 아바타 )
#endif SERV_ATTRACTION_ITEM_GOLD_CONDOR
		//}}

		RID_ATTRANTION_BONUS					= 91610,
		
#ifdef SERV_ATTRACTION_ITEM_ARC_DEVIL
		RID_ATTRACTION_ITEM_ARC_DEVIL			= 70005100,	// 아크데빌 봉인 조각상 ( 가열기 아님! )
#endif
#ifdef SERV_ATTRACTION_ITEM_EU
		RID_ATTRACTION_ITEM_OFFICER_EU			= 85001530, // 봉인된 얼음 조각상(엘소드, 레나, 아이샤, 레이븐, 이브, 청 엘 수색대 장교)
		RID_ATTRACTION_ITEM_OFFICER_DE			= 85001531, // 봉인된 얼음 조각상(레이븐, 이브, 청 엘 수색대 장교)
		RID_ATTRACTION_ITEM_NASOD_EU			= 85002450, //얼음 조각상 가열기(나소드 배틀 슈트)
		RID_ATTRACTION_ITEM_NASOD_DE			= 85002440, //얼음 조각상 가열기(레이븐, 이브, 청 나소드 배틀 슈트)
		RID_ATTRACTION_ITEM_ARCANGEL_EU			= 85002870, // 봉인된 얼음 조각상(아크엔젤)
		RID_ATTRACTION_ITEM_ARCANGEL_DE			= 85002871, // 봉인된 얼음 조각상(이브, 청 아크엔젤)
		RID_ATTRACTION_ITEM_VELDER_EU			= 85002872, // 봉인된 얼음 조각상(벨더 임페리얼 가드)
		RID_ATTRACTION_ITEM_CHUNG_VELDER_EU		= 85002873, // 봉인된 얼음 조각상(청 벨더 임페리얼 가드)
		RID_ATTRACTION_ITEM_SOLES_EU			= 85002874, // 봉인된 얼음 조각상(살바토르 솔레스)
		RID_ATTRACTION_ITEM_CHUNG_SOLES_EU		= 85002875, // 봉인된 얼음 조각상(청 살바토르 솔레스)
		RID_ATTRACTION_ITEM_CHUNG_ARCDEVIL		= 85002876, // 봉인된 얼음 조각상(청 아크데빌)
		RID_ATTRACTION_ITEM_ARCDEVIL			= 85002877, // 봉인된 얼음 조각상(아크데빌)
#endif //SERV_ATTRACTION_ITEM_EU	
#ifdef SERV_MINI_ATTRACTION_ITEM_JP
		RID_MINI_ATTRACTION_ITEM_JP1			= 65001062, // 미니 얼음 조각상 가열기
#endif SERV_MINI_ATTRACTION_ITEM_JP
#ifdef SERV_BATTLEFIELD_COOKIE_PIECE
		RID_BATTLEFIELD_COOKIE					= 85002460,	// 필드 쿠키
		RID_BATTLEFIELD_COOKIE_PIECE			= 85002461,	// 필드 쿠키 조각
		RID_BATTLEFIELD_GOLD_COOKIE				= 85002462, // 필드 황금 쿠키
		RID_BATTLEFIELD_GOLD_COOKIE_PIECE		= 85002463, // 필드 황금 쿠키 조각
#endif SERV_BATTLEFIELD_COOKIE_PIECE
#ifdef SERV_ONE_THIRD_MINI_ATTRACTION_ITEM
		RID_ONE_THIRD_MINI_ATTRACTION_ITEM		= 140000110,		// 2013.04.23 lygan_조성욱 // 1/3 확률의 미니 가열기 조각상 // 해당 가열기는 차가운 크리스탈이 추가 지급 되지 않는다.
#endif //SERV_ONE_THIRD_MINI_ATTRACTION_ITEM		
		
#ifdef	SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD// 적용날짜: 2013-04-25
		RID_ALL_IN_ONE_ATTRACTION_ITEM					= 160772,	// 봉인된 올인원 얼음 조각상 (시즌1)
#endif	// SERV_ALL_IN_ONE_RANDOM_CUBE_REWARD

#ifdef SERV_2013_CHINA_EVENT
		RID_2013_CHINA_EVENT_CUBE				= 84001860,
#endif SERV_2013_CHINA_EVENT
	};

	struct RandomUnitData
	{
		char		m_cUnitClass;
		int			m_iItemGroupID;
	};

	//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
	typedef std::pair< int, int >	RandomItemKey;
	//}}

	struct RandomItemData
	{
		int			m_ItemID;
		bool		m_bGiveAll;
		//{{ 2012. 10. 16	박세훈	RandomBoxTable 구조 변경
#ifdef SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		byte		m_byteUseCondition;
#else
		bool		m_bUnitSelect;
		bool		m_bGiveCharacter;
#endif SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		//}}
		int			m_KeyItemID;
		int			m_RequiredKeyCount;
		int			m_iRessurectionCount;
		int			m_iRestoreSpirit;

		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		int			m_iRequiredED;
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}

		std::vector< RandomUnitData > m_vecUnitData;

		//{{ 2012. 10. 16	박세훈	RandomBoxTable 구조 변경
#ifdef SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		RandomItemData::RandomItemData()
			: m_byteUseCondition( 0 )
		{
		}
#endif SERV_CHANGE_THE_RANDOM_BOX_TABLE_STRUCTURE
		//}}
	};

	struct ItemGroupData
	{
		int			m_iItemID;
		float		m_fRate;
		int			m_iPeriod;
		int			m_iQuantity;

		ItemGroupData()
		{
			m_iItemID	= 0;
			m_fRate		= 0.0f;
			m_iPeriod	= 0;
			m_iQuantity	= 0;
		}
	};

	struct ItemGroupDataList
	{
		int			m_iGroupID;
		KLottery	m_kLottery;
		std::vector< ItemGroupData > m_vecItemData;

		ItemGroupDataList(){};
		ItemGroupDataList( const ItemGroupDataList& t ) { *this = t; }    // copy constructor
		ItemGroupDataList& operator=( const ItemGroupDataList& t )        // assign operator
		{
			m_iGroupID = t.m_iGroupID;
			m_kLottery = t.m_kLottery;
			m_vecItemData = t.m_vecItemData;
			return *this;
		}
	};

	//{{ 2009. 6. 22  최육사	매력아이템리스트
	struct KAttractionItemInfo
	{
		bool	m_bNotifyMsg;
		int		m_iMsgNum;

		KAttractionItemInfo()
		{
			m_bNotifyMsg = false;
			m_iMsgNum = 0;
		}
	};
	//}}

	//{{ 2013. 3. 16	박세훈	매력 아이템 스크립트 처리
#ifdef SERV_CHARM_ITEM_SCRIPT
	struct CharmItemInfo
	{
		int m_iBonusItemID;

		CharmItemInfo()
			: m_iBonusItemID( 0 )
		{
		}
	};
#endif SERV_CHARM_ITEM_SCRIPT
	//}}

public:
	CXSLRandomItemManager(void);
	~CXSLRandomItemManager(void);

	// for lua
	bool	AddRandomItemTemplet_LUA();
	bool	AddRandomItemGroup_LUA( int iGroupID, int iItemID, float fRate, int iPeriod, int iQuantity );

#ifdef SERV_CUBE_IN_ITEM_MAPPING
	bool	AddRandomItemMappingData_LUA( int nIndex, int iCubeID, int iKeyItemID, int iBeforeGroupID, int iAfterGroupID );
#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	bool    AddRandomItemMappingTime_LUA( int nIndex, std::wstring wstrScriptStartDate, std::wstring wstrScriptEndDate );
#else SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
	bool	AddRandomItemMappingTime_LUA( int nIndex, const char* szStartDate, const char* szEndDate );
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

	bool	ModifyMapItemGroup(RANDOMITEM_MAPPING_DATA randItemMappingData);
	bool	RestoreMapItemGroup(RANDOMITEM_MAPPING_DATA randItemMappingData);

	std::map<int, RANDOMITEM_MAPPING_DATA>		m_mapRandomItemMappingData;
	std::map<int, RANDOMITEM_MAPPING_TIME>		m_mapRandomItemMappingTime;
#endif SERV_CUBE_IN_ITEM_MAPPING

	//{{ 2009. 6. 22  최육사	매력아이템 리스트
	void	SetNotifyMsg_LUA( bool bVal ) { m_bIsNotifyMsg = bVal; }
#ifdef SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	bool	AddAttractionItemInfo_LUA( int iItemID, bool bNotify, bool bSeal = true );
	// 다른 함수랑 구분 위해 _Global 붙임
	bool	IsSealedItem_Global( IN const int iItemID ) const { return ( m_setSealedItemList_Global.find( iItemID ) != m_setSealedItemList_Global.end() ); }
#else //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	bool	AddAttractionItemInfo_LUA( int iItemID, bool bNotify );
#endif //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	//}}
	//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
	bool	AddSealRandomItemInfo_LUA( int iItemID );
	bool	AddNotifyResultItemInfo_LUA( int iItemID );
#endif SERV_SEALED_RANDOM_ITEM
	//}}
	//{{ 2013. 3. 16	박세훈	매력 아이템 스크립트 처리
#ifdef SERV_CHARM_ITEM_SCRIPT
	void	AddCharmItem_LUA( IN int iItemID, IN int iBonusItemID );
	bool	IsCharmItem( IN const int iItemID ) const;
	int		GetBonusCharmItem( IN const int iItemID ) const;
#endif SERV_CHARM_ITEM_SCRIPT
	//}}

	// function
	//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
	const std::map< int, int >* GetRandomItemKeyList( IN int iRandomItemID ) const;
	//}}
	const RandomItemData* GetRandomItemData( IN int iRandomItemID, IN int iKeyItemID ) const;
	//{{ 2009. 6. 22  최육사	매력아이템 리스트
	bool	IsAttrationItem( IN const int iItemID ) const { return ( m_mapAttractionItem.find( iItemID ) != m_mapAttractionItem.end() ); }
	bool	MakeNotifyMsgAttrationItem( IN const int iRewardItemID, IN const std::wstring& wstrNickName, OUT std::wstring& wstrNotifyMsg );	
	//}}
	//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
	bool	MakeNotifyMsgRandomResultItem( IN const int iRewardItemID, IN const std::wstring& wstrNickName, OUT std::wstring& wstrNotifyMsg );
	bool	IsSealRandomItemCube( IN int iItemID )	{ return ( m_setSealRandomItem.find( iItemID ) != m_setSealRandomItem.end() ); }
	bool	IsNotifyResultItem( IN int iItemID )	{ return ( m_setNotifyResultItem.find( iItemID ) != m_setNotifyResultItem.end() ); }
#endif SERV_SEALED_RANDOM_ITEM
	//}}

	//{{ 2011. 05. 12  김민성	랜덤 큐브 오픈시 인벤토리 검사 수정
#ifdef SERV_RANDOM_ITEM_CHECK_INVEN
	bool	GetResultItem( IN char cUnitClass, IN const RandomItemData* pRandomItemData, OUT std::map< int, KItemInfo >& mapResultItem, OUT std::map< int, int >& mapCheckEmpty, OUT bool& bIsGiveAll ) const;
#else
	bool	GetResultItem( IN char cUnitClass, IN const RandomItemData* pRandomItemData, OUT std::map< int, KItemInfo >& mapResultItem, OUT std::map< int, int >& mapCheckEmpty ) const;
#endif SERV_RANDOM_ITEM_CHECK_INVEN
	//}}

private:
	//{{ 2009. 12. 5  최육사	랜덤큐브시스템개편
	std::map< int, std::map< int, int > >		m_mapRandomItemKeyTable;
	//}}
	std::map< RandomItemKey, RandomItemData >	m_mapRandomItem;
	std::map< int, ItemGroupDataList >			m_mapItemGroup;
	//{{ 2009. 6. 22  최육사	매력아이템리스트
	std::map< int, KAttractionItemInfo >		m_mapAttractionItem;
	bool										m_bIsNotifyMsg;
	//}}
	//{{ 2010. 7. 26  최육사	봉인 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
	std::set< int >								m_setSealRandomItem;
	std::set< int >								m_setNotifyResultItem;
#endif SERV_SEALED_RANDOM_ITEM
	//}}

#ifdef SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL
	std::set< int >								m_setSealedItemList_Global;
#endif //SERV_RANDOM_CUBE_SPECIAL_ATTRACTION_NOT_SEAL

	//{{ 2013. 3. 16	박세훈	매력 아이템 스크립트 처리
#ifdef SERV_CHARM_ITEM_SCRIPT
	std::map<int, CharmItemInfo>				m_mapCharItemInfo;
#endif SERV_CHARM_ITEM_SCRIPT
	//}}
};

DefRefreshSingletonInline( CXSLRandomItemManager );



