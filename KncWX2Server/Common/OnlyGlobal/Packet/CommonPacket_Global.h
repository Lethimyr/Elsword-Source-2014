#pragma once


#pragma pack( push, 1 )

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
DECL_DATA( KGlobalCashInfo )
{
	// 여러 종류의 캐시를 서버가 지원해야 할 떄 사용
	
	// 기존 [EBP_BuyItem] SP 전달 파라미터와 Enum값 맞춘 상태라 변경하면 안됨
	enum GLOBAL_CASH_TYPE
	{
		GCT_PUBLISHER_CASH			= 0,		// 일반적인 퍼블리셔 캐시 (한종류의 캐시만 쓰는 경우 이것으로 통일. 실제 캐시 종류가 다르더라도 엘소드 서버가 캐시 구분 할 필요 없을 경우 이것으로 처리함)
		GCT_KOG_ELSWORD_CASH		= 1,		// KOG에서 관리하는 엘소드로 전환된 캐시 (중국 & 동남아시아 사용)
		GCT_KOG_ELSWORD_BONUS_POINT	= 2,		// KOG에서 관리하는 엘소드 추가 보너스 포인트 (중국만 사용)
		GCT_PUBLISHER_ELSWORD_CASH	= 3,		// 퍼블리셔 쪽 엘소드로 전환된 캐시
		GCT_TOTAL_NUM

		// 넣을려면 PUT, GET도 수정해야함
		//GCT_PUBLISHER_BONUS_CASH	= 4,		// 퍼블리셔 쪽 보너스 캐쉬 (현재 사용국가 없어서 주석처리)
	};

	unsigned long				m_ulCash[GCT_TOTAL_NUM];

	KGlobalCashInfo()
	{
		for(int i=0; i < GCT_TOTAL_NUM; i++ )
		{
			m_ulCash[i]	= 0;
		}
	}
};
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


DECL_DATA( KBillProductCategory )
{
	char                m_cCategoryNo;
	std::wstring        m_wstrCategoryName;
	char                m_cParentCategoryNo;
	char                m_cDisplayNo;
};

DECL_DATA( KBillProductInfo )
{
	int                 m_iProductNo;
	int                 m_iProductID;
	bool                m_bSale;
	bool                m_bEvent;
	bool                m_bShow;
	bool                m_bIsCashInvenSkip;
	std::wstring        m_wstrProductName;
	char                m_cCategoryNo;
	int                 m_iSalePrice;
	int                 m_iRealPrice;
	char                m_cPeriod;			// 사용 기간
	char                m_cQuantity;
	bool                m_bEnableGift;
	char                m_cLimitLevel;
	std::wstring        m_wstrEndDate;		// 판매 기간
#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
	std::set< std::pair< __int64, bool > >	m_setSalePeriod;
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
};

DECL_DATA( KBillProductAttribute )
{
	int                 m_iProductNo;
	std::vector< int >  m_vecAttribute;
};

DECL_DATA( KBillPackageInfo )
{
	int                 m_iPackageNo;	// TranNo 임
	std::vector< int >  m_vecProductNo;

#ifdef SERV_GLOBAL_CASH_PACKAGE
	unsigned long       m_ulProductNo;
	int			        m_iProductID;
	std::wstring        m_wstrProductID;
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
	short				m_sProductExpire;
	//}}
	unsigned short      m_usProductPieces;
	std::vector< int >  m_vecSocketOption;
	//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경
	short GetProductExpire() const { return ( m_sProductExpire == -1 ? 0 : m_sProductExpire ); }
	//}}
#endif //SERV_GLOBAL_CASH_PACKAGE
};

DECL_DATA( KBillBuyInfo )
{
	int                 m_iProductNo;
	int                 m_iPoint;
	int                 m_iProductID;
	unsigned short		m_usOrderQuantity;
	int                 m_iCategory;
	std::wstring        m_wstrMemo;


	std::wstring		m_wstrOrderID;


	KBillBuyInfo()
	{
		m_iProductNo		= 0;
		m_iPoint			= 0;
		m_iProductID		= 0;
		m_usOrderQuantity	= 0;
		m_iCategory			= 0;
		m_wstrMemo			= L"";

		m_wstrOrderID		= L"";
	}
};

DECL_DATA( KBillOrderInfo )
{
	//{{ 허상형 : [2010/2/10/] //	캐시 아이템 지급시 Sell Log 추가
	enum BUY_CASH_ITEM_TYPE
	{
		BCIT_INVALID	= 0,
		BCIT_BUY_ITEM,			//	캐시 아이템 구입
		BCIT_COUPON,			//	쿠폰
		BCIT_GIFT,				//	선물
	};
	//}} 허상형 : [2010/2/10/] //	캐시 아이템 지급시 Sell Log 추가

	UidType             m_iTransNo;
	std::wstring        m_wstrOrderID;
	KBillProductInfo    m_kBillProductInfo;
	std::vector< int >  m_vecSocketOption;

	byte				m_byteType;
	std::wstring		m_wstrSenderNickName;
	std::wstring		m_wstrSenderPresentMessage;
	//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
	UidType				m_iFromUnitUID;
	//}} 허상형 : [2010/8/19/] //	선물 기능 개편
#ifdef SERV_GLOBAL_CASH_PACKAGE
	BYTE                m_byteProductKind;
#endif //SERV_GLOBAL_CASH_PACKAGE

	KBillOrderInfo()
	{
		m_iTransNo = -1;
		m_byteType = BCIT_INVALID;
#ifdef	SERV_GLOBAL_CASH_PACKAGE
		m_byteProductKind = 0;
#endif	SERV_GLOBAL_CASH_PACKAGE
	}
};
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
DECL_DATA( kDisCountItemInfo )
{
	int					m_DisCountProNo;
	int					m_BasicProNo;
	std::vector< int >	m_DisCountKeepItem;
};
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#endif SERV_GLOBAL_BILLING
//////////////////////////////////////////////////////////////////////////


#ifdef SERV_NEW_PVPROOM_PROCESS

DECL_PACKET( EGS_CHANGE_PVP_ROOM_PUBLIC_REQ )
{
	bool									m_bPublic;
	std::wstring							m_wstrPassword;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_PUBLIC_ACK )
{
	bool									m_bPublic;
	int										m_iOK;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_PUBLIC_NOT )
{
	bool									m_bPublic;
};

typedef KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ KERM_CHANGE_PVP_ROOM_PUBLIC_REQ;
typedef KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK KERM_CHANGE_PVP_ROOM_PUBLIC_ACK;

DECL_PACKET( EGS_CHANGE_PVP_ROOM_NAME_REQ )
{
	std::wstring							m_wstrRoomName;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_NAME_ACK )
{
	std::wstring							m_wstrRoomName;
	int										m_iOK;
};

DECL_PACKET( EGS_CHANGE_PVP_ROOM_NAME_NOT )
{
	std::wstring							m_wstrRoomName;
};

typedef KEGS_CHANGE_PVP_ROOM_NAME_REQ KERM_CHANGE_PVP_ROOM_NAME_REQ;
typedef KEGS_CHANGE_PVP_ROOM_NAME_ACK KERM_CHANGE_PVP_ROOM_NAME_ACK;

#endif SERV_NEW_PVPROOM_PROCESS

#ifdef SERV_PC_BANG_TYPE
DECL_DATA( KPcBangReward )
{
	int		m_iPcBangType;
	float	m_fEXPRate;
	float	m_fEDRate;
	float	m_fVSPointRate;
	bool	m_bSpiritFree;
	float	m_fPetEXPRate;
	int		m_iAdditionalHenirReward;
	bool	m_bSecretDungeonFree;
	bool	m_bAvatar;
	bool	m_bTitle;

	KPcBangReward()
	{
		m_iPcBangType				= -1;
		m_fEXPRate					= 0.0f;
		m_fEDRate					= 0.0f;
		m_fVSPointRate				= 0.0f;
		m_bSpiritFree				= false;
		m_fPetEXPRate				= 0.0f;
		m_iAdditionalHenirReward	= 0;
		m_bSecretDungeonFree		= false;
		m_bAvatar					= false;
		m_bTitle					= false;
	}

	COPYCON_ASSIGNOP( KPcBangReward, right )
	{
		m_iPcBangType				= right.m_iPcBangType;
		m_fEXPRate					= right.m_fEXPRate;
		m_fEDRate					= right.m_fEDRate;
		m_fVSPointRate				= right.m_fVSPointRate;
		m_bSpiritFree				= right.m_bSpiritFree;
		m_fPetEXPRate				= right.m_fPetEXPRate;
		m_iAdditionalHenirReward	= right.m_iAdditionalHenirReward;
		m_bSecretDungeonFree		= right.m_bSecretDungeonFree;
		m_bAvatar					= right.m_bAvatar;
		m_bTitle					= right.m_bTitle;

		return *this;
	}
};
#endif SERV_PC_BANG_TYPE

//{{ 2011. 12. 09	박교현		서버군 리스트
#ifdef SERVER_GROUP_UI_ADVANCED
DECL_DATA( KServerGroupInfo )
{
	int							m_iOrder;
	int							m_iServerGroupUID;
	std::wstring				m_wstrServerGroupName;
	std::wstring				m_kServerIP;
	u_short						m_usMasterPort;
	u_short						m_usNCUDPPort;
	bool						m_bEnable;
};
#endif SERVER_GROUP_UI_ADVANCED
//}}

#ifdef SERV_CATCH_HACKUSER_INFO
DECL_PACKET( EGS_CATCH_HACKUSER_INFO_NOT )
{
	UidType				m_iUserUID;
	UidType				m_iUnitUID;
	int					m_iCrashType;
};
#endif SERV_CATCH_HACKUSER_INFO

#ifdef	SERV_SHARING_BANK_TEST
DECL_DATA( KTradeShareItemResult )
{
	UidType								m_iBeforeItemUID;
	UidType								m_iNewItemUID;

	std::wstring						m_wstrRegDate;

	KTradeShareItemResult()
	{
		m_iBeforeItemUID				= 0;
		m_iNewItemUID					= 0;

		m_wstrRegDate				= L"";
	}
};
#endif	SERV_SHARING_BANK_TEST

//{{ 2013. 05. 14	최육사	아이템 개편
#ifdef SERV_BLOCK_LIST
DECL_DATA( KBlockInfo )
{
	enum BLOCK_TYPE
	{
		BT_TRADE_BLOCK = 0,
		BT_ACCOUNT_BLOCK,
		BT_IP_BLOCK,
		BT_DLL_BLOCK,

		BT_MAX,
	};

    UidType							m_iBlockUID;
	char							m_cBlockType;
	UidType							m_iBlockTarget;
	std::wstring					m_wstrBlockTarget;	
	std::wstring					m_wstrStartDate;
	std::wstring					m_wstrEndDate;

	KBlockInfo()
	{
		m_iBlockUID = 0;
		m_cBlockType = 0;
		m_iBlockTarget = 0;
	}
};
#endif SERV_BLOCK_LIST
//}}

#ifdef SERV_ANTI_ADDICTION_SYSTEM
DECL_DATA( KGiantAccountType )
{
	enum TYPE_MASK
	{ 
		NORMAL_USER			= 0,		// 00: 일반 유저
		SPECIAL_USER		= 2,		// 01: 특수 유저
		VIP_USER			= 4,		// 02: VIP 유저
		EVENT_PLAYER		= 8,		// 03: 이벤트 플레이어
		ADULT_PLAYER		= 16,		// 04: 성인 플레이어
		BLACKLIST_USER		= 32,		// 05: 블랙리스트 유저

		DEALER				= 512,		// 10: 딜러?
		WEB_DEALER			= 1024,		// 11: 웹딜러?
	};

	KGiantAccountType()
	{
		m_uiType = 0;
	}

	bool			IsType(TYPE_MASK mask)			{ return ( m_uiType & mask ) ? true : false; }

	unsigned int	m_uiType;
};
#endif SERV_ANTI_ADDICTION_SYSTEM

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
DECL_DATA( KRandomItemResult )
{
	int		m_iItemCount;
	bool	m_bIsSealItem;
	double  m_dRate;
	double  m_dItemTotalCount;
	bool	m_bIsNotifyMsgAttration;

};
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef SERV_ID_NETMARBLE_PCBANG
DECL_PACKET( ECH_PCBANG_IP_AND_MAC_INFO_NOT )
{
	std::wstring							m_wstrPublicIP;
	std::wstring							m_wstrMacInfo;
};

typedef KECH_PCBANG_IP_AND_MAC_INFO_NOT			 KDBE_PCBANG_IP_AND_MAC_INFO_NOT;
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_COUNTRY_PH
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef std::vector<uint8> dataBuffer_t;
typedef std::string string_t;
DECL_DATA( kGarenaUserTokInfo_t ) // 2013.07.31 lygan_조성욱 // 동남아시아 퍼블리셔에서 인증 관련으로 제공해준 구조체
{
	uint32				m_Uid;
	std::string			m_Username;
	uint32				m_Timestamp;
	uint16				m_GcaType;
	uint8				m_GoldMemberType;
	uint8				m_IpBonus;
	std::string			m_Reserved;
};
#endif //SERV_COUNTRY_PH

//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
//#ifdef SERV_RELAY_DB_CONNECTION
DECL_DATA( KJPEventRewardInfo )
{
	__int64								m_iRID;
	std::wstring						m_wstrUserID;
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	int									m_iRewardID;

	KJPEventRewardInfo()
	{
		m_iRID = 0;
		m_iUserUID = 0;
		m_iUnitUID = 0;
		m_iRewardID = 0;
	}
};
//#endif SERV_RELAY_DB_CONNECTION
//}}

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
DECL_DATA( KEventCharacterRanking )
{
	// 남자일때 1은 엘소드 , 2는 레이븐 , 3은 청
	// 여자일때 1은 아이샤 , 2는 레나 , 3은 이브 , 4는 아라
	int								m_iDungeonCharacter1;
	int								m_iDungeonCharacter2;
	int								m_iDungeonCharacter3;
	int								m_iDungeonCharacter4;
	int								m_iPVPCharacter1;
	int								m_iPVPCharacter2;
	int								m_iPVPCharacter3;
	int								m_iPVPCharacter4;
	int								m_iFieldCharacter1;
	int								m_iFieldCharacter2;
	int								m_iFieldCharacter3;
	int								m_iFieldCharacter4;

	KEventCharacterRanking()
	{
		m_iDungeonCharacter1 = 0;
		m_iDungeonCharacter2 = 0;
		m_iDungeonCharacter3 = 0;
		m_iDungeonCharacter4 = 0;
		m_iPVPCharacter1 = 0;
		m_iPVPCharacter2 = 0;
		m_iPVPCharacter3 = 0;
		m_iPVPCharacter4 = 0;
		m_iFieldCharacter1 = 0;
		m_iFieldCharacter2 = 0;
		m_iFieldCharacter3 = 0;
		m_iFieldCharacter4 = 0;
	}
};
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING


#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
DECL_DATA( EVENT_DATA ) // 2013.10.31 darkstarbt_조성욱 // 국내에서 예전에 CommonPacket.h 에 만들어 놓은거에 최신화 안하고 따로 여기 만들고, 국내 들어가면 여기꺼 그대로들어가면됨
{
	float	m_fEXPRate;
	float	m_fVPRate;
	float	m_fEDRate;

	float	m_fRankingPointRate;


	int		m_iDungeonID;
	int		m_iDungeonMode;
	int		m_iUnitLevel;		
	char	m_cUnitType;
	char    m_cUnitClass;
	int		m_iPartyNum;

	// 아이템 이벤트
	float	m_fEventTime;
	int		m_iEventReward;
	std::map< std::pair< int, int >, int >	m_mapMinMaxReward;

	bool	m_bGiveOnce;
	bool	m_bDelEndEvent;
	bool	m_bPcBangEvent;
	bool	m_bDirectReward;
	bool	m_bAccountEvent;
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		float	m_fDropRate;
#else // SERV_DROP_EVENT_RENEWAL	
	int		m_iDropCount;
#endif // SERV_DROP_EVENT_RENEWAL
	bool	m_bWithPlayPcBang;

	int		m_iChannelCode;
	int		m_iBF_Team;

	int		m_iHenirRewardEventCount;
	bool	m_bHenirRewardUnLimited;


	int		m_iServerGroup;

	bool	m_bAccumulateEXP;

	//{{ 2012. 03. 28	박세훈	드롭률 설정 이벤트 개편 Merge 작업 ( 2012.03.20 lygan_조성욱 )
#ifdef DROPEVENT_RENEWAL
	int		m_iStartLevel;
	int		m_iEndLevel;
#endif DROPEVENT_RENEWAL
	//}}

	bool	m_bRepeatEvent;

	bool	m_bRepeatRewardEvent;
	int		m_iBeforeEventID;

	//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	bool	m_bPlayWithCharEXP;
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
	//}}

	int		m_iEventGroupID;

	float	m_fQuestItemDropRate;

#ifdef SERV_ADVERTISEMENT_EVENT
	std::wstring	m_wstrAdvertisementURL;
#endif SERV_ADVERTISEMENT_EVENT


	int		m_iEventItemID;

	int		m_iCustomEventID;

#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
	float	m_fDefenseDungeonOpenRate;
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT

#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
	byte	m_byteCriterionUserType;
	byte	m_byteCriterionPresentUserType;
	CTime	m_tCriterionDateBegin;
	CTime	m_tCriterionDateEnd;
#endif	// SERV_CRITERION_DATE_EVENT

#ifdef SERV_STEAM_USER_CONNECT_EVENT
	bool			m_bOnlySteamUser;
	bool			m_bOnlyNotSteamUser;
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	bool	m_bNewUnitEvent;
	bool	m_bNewUnitEvent2;
	bool	m_bCurrentUnitEvent;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		int		m_iUnitClassLevel;
#endif SERV_UNIT_CLASS_LEVEL_EVENT

#ifdef SERV_HENIR_REWARD_EVENT// 작업날짜: 2013-09-09	// 박세훈
		bool	m_bAccumulate;
#endif // SERV_HENIR_REWARD_EVENT

	EVENT_DATA()
		: m_fEXPRate(0.f)
		, m_fVPRate(0.f)
		, m_fEDRate(0.f)
		, m_fRankingPointRate(0.f)
		, m_iDungeonID(0)
		, m_iDungeonMode(0)
		, m_iUnitLevel(0)
		, m_cUnitType(0)
		, m_cUnitClass(0)
		, m_iPartyNum(0)
		, m_fEventTime(0.f)
		, m_iEventReward(0)
		, m_bGiveOnce(true)
		, m_bDelEndEvent(false)
		, m_bPcBangEvent(false)
		, m_bDirectReward(false)
		, m_bAccountEvent(false)
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		, m_fDropRate(0.0f)
#else // SERV_DROP_EVENT_RENEWAL
		, m_iDropCount(0)
#endif // SERV_DROP_EVENT_RENEWAL
		, m_bWithPlayPcBang(false)
		, m_iChannelCode(0)
		, m_iBF_Team(0)
		, m_iHenirRewardEventCount(0)
		, m_bHenirRewardUnLimited(false)
		, m_iServerGroup(-1)
		, m_bAccumulateEXP(false)
		//{{ 2012. 03. 28	박세훈	드롭률 설정 이벤트 개편 Merge 작업 ( 2012.03.20 lygan_조성욱 )
#ifdef DROPEVENT_RENEWAL
		, m_iStartLevel(0)
		, m_iEndLevel(0)
#endif DROPEVENT_RENEWAL
		//}}
		, m_bRepeatEvent(false)
		, m_bRepeatRewardEvent(false)
		, m_iBeforeEventID(-1)
		//{{ 2012. 12. 17	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		, m_bPlayWithCharEXP(false)
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		//}}
		, m_iEventGroupID(-1)
		, m_fQuestItemDropRate(-1.f)
		, m_iEventItemID(0)
		, m_iCustomEventID(0)
#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT// 적용날짜: 2013-04-11
		, m_fDefenseDungeonOpenRate(1.0f)
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT// 적용날짜: 2013-04-11
		, m_byteCriterionUserType(0)
		, m_byteCriterionPresentUserType(0)
#endif	// SERV_CRITERION_DATE_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		, m_bOnlySteamUser(false)
		, m_bOnlyNotSteamUser(false)
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		, m_bNewUnitEvent(false)
		, m_bNewUnitEvent2(false)
		, m_bCurrentUnitEvent(false)
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		, m_iUnitClassLevel(-1)
#endif SERV_UNIT_CLASS_LEVEL_EVENT
#ifdef SERV_HENIR_REWARD_EVENT// 작업날짜: 2013-09-09	// 박세훈
		, m_bAccumulate(false)
#endif // SERV_HENIR_REWARD_EVENT
	{
	}

};

DECL_DATA( KRewardData )
{	
	int				m_iRewardItemID;
	int				m_iQuantity;
	short			m_sPeriod;

	KRewardData(){};
	KRewardData( const KRewardData& t ) { *this = t; }    // copy constructor
	KRewardData& operator=( const KRewardData& t )       // assign operator
	{			
		m_iRewardItemID	= t.m_iRewardItemID;
		m_iQuantity		= t.m_iQuantity;
		m_sPeriod		= t.m_sPeriod;

		return *this;
	}
};

DECL_PACKET( ESG_REWARD_DB_DATA_NOT )
{
	std::map< int, std::vector< KRewardData > > m_mapTotalRewardData;
};


#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_CUBE_IN_ITEM_MAPPING
DECL_DATA( kRandomItemMappingToInfoClient )
{
	int		 m_iBeforeGroupID;
	int		 m_iAfterGroupID;
	bool	 m_bCheckMapping;
	std::wstring	 m_tStartTime;
	std::wstring	 m_tEndTime;

	kRandomItemMappingToInfoClient()
	{
		m_iBeforeGroupID = 0;
		m_iAfterGroupID = 0;
		m_bCheckMapping = false;
	}
};

#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
DECL_DATA( KRandomItemMappingToInfoServer )
{
	int		 m_iIndex;
	int		 m_iCubeID;
	int		 m_iKeyItemID;
	int		 m_iBeforeGroupID;
	int		 m_iAfterGroupID;
	std::wstring	 m_tStartTime;
	std::wstring	 m_tEndTime;

	KRandomItemMappingToInfoServer()
	{
		m_iIndex			= 0;
		m_iCubeID			= 0;
		m_iKeyItemID		= 0;
		m_iBeforeGroupID	= 0;
		m_iAfterGroupID		= 0;
		m_tStartTime		= L"";
		m_tEndTime			= L"";
	}
};
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
#endif //SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_USE_GM_TOOL_INFO
DECL_DATA( KItemName )
{
	std::wstring				m_wstrKRItemName;
	std::wstring				m_wstrTransItemName;
};
#endif //SERV_USE_GM_TOOL_INFO

//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
DECL_DATA( KGlobalMissionReward )
{
	int									m_iRewardType;					// 보상 타입(버프, 접속이벤트 등)
	int									m_iRewardDurationTime;			// 보상 유지 시간(분)
	int									m_iScriptID;					// 보상 스크립트 ID

	KGlobalMissionReward()
	{
		m_iRewardType = 0;
		m_iRewardDurationTime = 0;
		m_iScriptID = 0;
	}
};

DECL_DATA( KGlobalMissionInfo )
{
	bool									m_bActive;						// 진행 중인지 여부
	std::wstring							m_wstrMissionStartTime;			// 실제 글로벌 미션 시작 시간
	int										m_iDurationMinute;				// 글로벌 미션 유지 시간(분)
	int										m_iRemainSecond;				// 글로벌 미션 남은 시간(초)

	int										m_iMissionType;					// 글로벌 미션 타입
	int										m_iClearCondition;				// 글로벌 미션 클리어 조건
	int										m_iClearCount;					// 목표 클리어 수치
	int										m_iNowCount;					// 현재 클리어 수치


	KGlobalMissionInfo()
	{
		m_bActive = false;
		m_wstrMissionStartTime = L"";
		m_iDurationMinute = 0;
		m_iRemainSecond = 0;

		m_iMissionType = 0;
		m_iClearCondition = 0;
		m_iClearCount = 0;
		m_iNowCount = 0;
	}
};
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	임홍락	글로벌 미션 매니저

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
DECL_DATA( KPacketGetItemOnOff )
{
	int						m_iType;
	int						m_iItemID;
	std::wstring			m_wstrStartTime;
	std::wstring			m_wstrEndTime;

	KPacketGetItemOnOff()
	{
		m_iType = 0;
		m_iItemID = 0;
		m_wstrStartTime = L"";
		m_wstrEndTime = L"";
	}
};

typedef KPacketOK								KDBE_GET_TIME_CONTROL_ITME_LIST_NOT;
typedef KPacketOK								KEGS_GET_TIME_CONTROL_ITME_LIST_NOT;
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING


#ifdef SERV_STRING_FILTER_USING_DB
DECL_DATA( KStringFilterInfo )
{
	short			m_sStringFilterType;
	std::wstring	m_wstrFilter;

	KStringFilterInfo()
	{
		m_sStringFilterType = 0;
		m_wstrFilter = L"";
	}
};
typedef std::vector< KStringFilterInfo > StringFilterVector;
#endif //SERV_STRING_FILTER_USING_DB

#pragma pack( pop )