#pragma once

#include "CommonPacket.h"

#pragma pack( push, 1 )

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_AUTH
DECL_PACKET( ECH_SECURITY_AUTH_REQ )
{
	// 추가 보안 인증 관련은 다 해당 패킷을 이용한다.
	enum SECURITY_AUTH_TYPE
	{
		SAT_NONE,						// 없음(초기값)
		SAT_CHK_SECOND_SECURITY,		// K3R 2차 보안패드 관련 체크
		SAT_MATRIX_CARD,				// Giant MatrixKey 시스템 사용

	};

	int						m_iSecurityAuthType;
	UidType					m_iUserUID;
	std::wstring			m_wstrUserID;
	
#ifdef SERV_PUBLISHER_MATRIXKEY
	std::wstring			m_wstrMatrix;
#endif // SERV_PUBLISHER_MATRIXKEY


	KECH_SECURITY_AUTH_REQ()
	{
		m_iSecurityAuthType = SAT_NONE;
		m_iUserUID			= -1;
		m_wstrUserID		= L"";
#ifdef SERV_PUBLISHER_MATRIXKEY
		m_wstrMatrix			= L"";
#endif // SERV_PUBLISHER_MATRIXKEY
	}
	
};

DECL_PACKET( ECH_SECURITY_AUTH_ACK )
{
	int						m_iOK;
	std::string				m_strSecondSecurityURL;	// K3R 2차 관련 URL

	KECH_SECURITY_AUTH_ACK()
	{
		m_strSecondSecurityURL = "";
	}
};

DECL_PACKET( ECH_SECURITY_INFO_NOT )
{
	enum SECURITY_INFO_TYPE
	{
		SIT_NONE,						// 없음(초기값)
		SIT_MATRIX_CARD,				// Giant MatrixKey 시스템 사용
	};

	int									m_iSecurityInfoType;
	unsigned int						m_uiMatrixKey;
	std::wstring						m_wstrCoordinate;


	KECH_SECURITY_INFO_NOT()
	{
		m_iSecurityInfoType		= SIT_NONE;
		m_uiMatrixKey			= 0;
		m_wstrCoordinate		= L"";
	}
};


#endif // SERV_GLOBAL_AUTH
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING

DECL_PACKET( EGS_EXCHANGE_CASH_REQ )
{
	int									m_uiExchangeValue;
};

DECL_PACKET( EGS_EXCHANGE_CASH_ACK )
{
	int									m_iOK;
	int									m_iPoint;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	KGlobalCashInfo						m_AddBonusCashInfo;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
};

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
DECL_PACKET( EGS_BONUS_CASH_EVENT_NOT )
{
	bool								m_bEventStart;
};
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES


DECL_PACKET( EGS_BILL_PRODUCT_INFO_REQ )
{
	int                                 m_iPage;
};

DECL_PACKET( EGS_BILL_PRODUCT_INFO_ACK )
{
	bool                                m_bFinal;
	std::vector< KBillProductInfo >     m_vecProductInfo;
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
	std::map< int,std::vector< int > >	m_mapKeepShowItem;
	std::map< int, kDisCountItemInfo >	m_DisCountInfoMap;
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#ifdef SERV_WISH_LIST_NO_ITEM
	std::set< int >						m_setWishListNoItemList;
#endif SERV_WISH_LIST_NO_ITEM
};

DECL_PACKET( EGS_BILL_INVENTORY_INQUIRY_REQ )
{
	int                                 m_nItemPerPage;
	int                                 m_iCurrentPage;
};

DECL_PACKET( EGS_BILL_INVENTORY_INQUIRY_ACK )
{
	int                                 m_iOK;
	std::vector< KBillOrderInfo >       m_vecOrderInfo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	std::vector< KBillPackageInfo >     m_vecPackageInfo;
	std::vector< int >					m_vecSubPackageTrans;
	std::vector< int >					m_vecSubPackageInfo;
#endif //SERV_GLOBAL_CASH_PACKAGE

};

DECL_PACKET( EGS_BILL_GET_PURCHASED_CASH_ITEM_REQ )
{
	// 국내 EGS_GET_PURCHASED_CASH_ITEM_REQ 패킷과 맵핑됨

	int                                 m_iTransNo;
	int                                 m_iProductNo;
	std::map< int, int >                m_mapSocketForCash;

	//{{ 2012. 11. 28 캐시 아이템 소켓 옵션 기능 수정 - 김민성
#ifdef SERV_CASH_ITEM_SOCKET_OPTION
	std::map< int, int >                m_mapSocketGroupIDForCash;		// <ItemID,GroupID>
#endif SERV_CASH_ITEM_SOCKET_OPTION
	//}
#ifdef SERV_GLOBAL_CASH_PACKAGE
	BYTE								m_byteProductKind;	// 1 : 패키지 아이템 
#endif //SERV_GLOBAL_CASH_PACKAGE

	// 해당 패킷과 유사한 패키지 아이템 가져오는 EGS_GET_PURCHASED_PACKAGE_CASH_ITEM_REQ는 해외도 그대로 씀
};

typedef     KEGS_GET_PURCHASED_CASH_ITEM_ACK    KEGS_BILL_GET_PURCHASED_CASH_ITEM_ACK;


#ifdef SERV_GOLD_TICKET
DECL_PACKET( EGS_CHARGE_POINT_NOT )
{
	int									m_iOK;
	int									m_iChargedCash;
};
#endif //SERV_GOLD_TICKET

#endif // SERV_GLOBAL_BILLING
//////////////////////////////////////////////////////////////////////////

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
DECL_PACKET( EGS_ADMIN_DELETE_GUILD_ADD_NOT )
{
	int						m_iGuildUID;
};
#endif

//{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
DECL_PACKET( EGS_ADMIN_DELETE_HENIR_RANKING_RANK_NOT )
{
	int						m_iRankingType;
	int						m_iRank;
};

DECL_PACKET( EGS_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT )
{
	int						m_iRankingType;
	std::wstring			m_wstrNickName;
};

DECL_PACKET( EGS_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT )
{
	int						m_iRankingType;
	UidType 				m_iUnitUID;
};
#endif SERV_DELETE_HENIR_RANKING
//}}

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
DECL_PACKET( EGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT )
{
	std::wstring						m_wstrNewCharTBMsg;	
};
#endif SERV_NEW_UNIT_TRADE_LIMIT


// 2011.05.05 lygan_조성욱 // 중국용 커뮤니티탭 유저 정보 보기
#ifdef SERV_USER_WATCH_NEW

DECL_PACKET( EGS_USER_COMMUNITY_SURVEY_NEW_REQ  )
{
	enum COMMUNITY_TAB_TYPE
	{
		NONE = 0,
		USER_WATCH,
		INVITE_GUILD,

	};

	std::wstring						m_wUnitNickName;
	int									m_cCommunity_Tab_Type;
	int									m_iOK;

	KEGS_USER_COMMUNITY_SURVEY_NEW_REQ()
	{
		m_cCommunity_Tab_Type = NONE;
	}
};


DECL_PACKET( EGS_USER_COMMUNITY_SURVEY_NEW_ACK  )
{
	enum COMMUNITY_TAB_TYPE
	{
		NONE = 0,
		USER_WATCH,
		INVITE_GUILD,

	};

	UidType								unitUID;
	int									m_cCommunity_Tab_Type;
	int									m_iOK;

	KEGS_USER_COMMUNITY_SURVEY_NEW_ACK()
	{
		m_cCommunity_Tab_Type = NONE;
	}
};
#endif //SERV_USER_WATCH_NEW

#ifdef SERV_NPROTECT_CS_AUTH_30
DECL_PACKET( EGS_NPROTECT_CSAUTH3_CHECK_NOT )
{
	std::deque< BYTE >					m_deqCSAuth3;
};

typedef KEGS_NPROTECT_CSAUTH3_CHECK_NOT	KEGS_NPROTECT_CSAUTH3_RESPONE_NOT;
typedef KEGS_NPROTECT_CSAUTH3_CHECK_NOT	KEGS_NPROTECT_CSAUTH3_GAMEHACK_REPORT_NOT;
#endif SERV_NPROTECT_CS_AUTH_30


#ifdef SERV_PVP_REMATCH
DECL_PACKET( EGS_PVP_REMATCH_REQ )
{
	bool							m_bAcceptRematch;
};
DECL_PACKET( EGS_PVP_REMATCH_ACK )
{
	int								m_iOK;
};
DECL_PACKET( EGS_PVP_REMATCH_NOT )
{
	UidType							m_iUnitUID;
	bool							m_bAcceptRematch;
};
#endif SERV_PVP_REMATCH

//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
DECL_PACKET( EGS_RESTORE_UNIT_REQ )
{
	UidType                 m_iUnitUID;
	UidType                 m_iUserUID;
};

DECL_PACKET( EGS_RESTORE_UNIT_ACK )
{
	int                     m_iOK;
	UidType                 m_iUnitUID;
	//{{ // 2012.03.06 lygan_조성욱 // 기존 문자열시간을 그대로 받는 부분을 서버에서 숫자로 변경해서 클라로 전달 구조로 변경
	__int64					m_tRestoreAbleDate; //2012.03.05 lygan_조성욱 // 캐릭터 삭제 할때 최종 삭제 가능한 날을 알기 위해 
	//}}
};

DECL_PACKET( EGS_FINAL_DELETE_UNIT_REQ )
{
	UidType                 m_iUnitUID;
	UidType                 m_iUserUID;
};

DECL_PACKET( EGS_FINAL_DELETE_UNIT_ACK )
{
	int                     m_iOK;
	UidType                 m_iUnitUID;
	u_int					m_uiKNMSerialNum;
	//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
	int						m_iGuildUID;
#endif GUILD_TEST
	//}}
	//{{ 2011. 02. 23	최육사	캐릭터 로그
#ifdef SERV_CHAR_LOG
	std::wstring			m_wstrNickName;
	u_char					m_ucLevel;
	char					m_cUnitClass;
#endif SERV_CHAR_LOG
	//}}
	__int64					m_tReDelAbleDate;
};
#endif SERV_UNIT_WAIT_DELETE

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
DECL_PACKET( EGS_RESOLVE_ITEM_AVATAR_REQ )
{
	std::map<int, UidType>		m_mapResolveDataAvatar;
	int							m_iAttachFee;
};

DECL_PACKET( EGS_RESOLVE_ITEM_AVATAR_ACK )
{
	int									m_iOK;
	bool								m_bJackpot;
	std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
};
#endif SERV_MULTI_RESOLVE

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
DECL_PACKET( EGS_SYNTHESIS_SOCKET_GROUPID_ACK )
{
	int						m_iOK;
	// <EquipPos, SocketOptionGroupID>
	std::map<int, int>		m_mapAvatarSocketOptionGroupID;
};


DECL_PACKET( EGS_SYNTHESIS_ITEM_REQ )
{
	std::map<int, UidType>		m_mapSynthesisData;
	UidType						m_iKeyData;
	int							m_iSocketNo;
};

DECL_PACKET( EGS_SYNTHESIS_ITEM_ACK )
{
	int									m_iOK;	
	int									m_iRessurectionCount;
	int									m_iRestoreSpirit;
	std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;

	KEGS_SYNTHESIS_ITEM_ACK()
	{		
		m_iRessurectionCount  = 0;
	}
};

#endif SERV_SYNTHESIS_AVATAR
//}}

//{{ 2011. 12. 09	박교현		서버군 리스트
#ifdef SERVER_GROUP_UI_ADVANCED
typedef KPacketOK		KECH_GET_SERVERGROUP_LIST_ACK;

DECL_PACKET( ECH_GET_SERVERGROUP_LIST_NOT )
{
	std::map< int, KServerGroupInfo >	m_mapServerGroupList;
};
#endif SERVER_GROUP_UI_ADVANCED
//}}

//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
DECL_PACKET( EGS_ANTI_ADDICTION_NOT )
{
	enum NOTICE_TYPE
	{
		NT_CHAT		= 0,
		NT_CENTER	= 1,
	};

	int									m_iType;
	int									m_iPlayTime;
};
#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

#ifdef SERV_EPAY_SYSTEM
DECL_PACKET( EGS_EPAY_INFO_NOT )
{
	std::wstring						m_wstrLoginID;
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	std::wstring						m_wstrCharacterName;
	UidType								m_iUnitUID;

};
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
DECL_PACKET( EGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT )
{
	bool									m_bCheckAdult;
};
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

#ifdef SERV_USE_XTRAP
DECL_PACKET( EGS_XTRAP_REQ )
{
	std::vector<unsigned char> m_vecData;
};

DECL_PACKET( EGS_XTRAP_ACK )
{
	std::vector<unsigned char> m_vecData;
};
#endif SERV_USE_XTRAP

#ifdef SERV_INVISIBLE_GM
DECL_PACKET( EGS_TOGGLE_INVISIBLE_NOT )
{
	UidType								m_iUnitUID;
	KFieldUserInfo						m_kFieldUserInfo;
};
#endif SERV_INVISIBLE_GM

#ifdef SERV_SHARING_BANK_TEST

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
DECL_PACKET( EGS_GET_SHARE_BANK_REQ )
{
	std::wstring							m_wstrNickName;
	int										m_iNewUnitTradeBlockDay;
	int										m_iNewUnitTradeBlockUnitClass;

	KEGS_GET_SHARE_BANK_REQ()
	{
		m_wstrNickName		= L"";
		m_iNewUnitTradeBlockDay  = 0;
		m_iNewUnitTradeBlockUnitClass = 0;
	}
};
#else // SERV_NEW_UNIT_TRADE_LIMIT
typedef std::wstring	KEGS_GET_SHARE_BANK_REQ;
#endif // SERV_NEW_UNIT_TRADE_LIMIT

DECL_PACKET( EGS_GET_SHARE_BANK_ACK )
{
	int										m_iOK;

	UidType									m_iUnitUID;
	std::wstring							m_wstrNickName;

	int										m_iBankSize;
	std::map< UidType, KInventoryItemInfo > m_mapItem;
};

DECL_PACKET( EGS_UPDATE_ITEM_POSITION_NOT )
{
	std::vector< KTradeShareItemResult >	m_vecShareItemResult;
};
#endif SERV_SHARING_BANK_TEST

#ifdef SERV_SHARING_BANK_QUEST_CASH
DECL_PACKET( EGS_SHARING_BACK_OPEN_NOT )
{
	int									m_iOK;
	int									m_iOpenType;
};
#endif SERV_SHARING_BANK_QUEST_CASH

#ifdef	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE
typedef	KPacketOK	KEGS_SHARE_BANK_UPDATE_OK_NOT;
#endif	SERV_SHARE_BANK_WAIT_SERVER_RECIEVE

#ifdef SERV_ADVERTISEMENT_EVENT
typedef KEGS_TIME_EVENT_INFO_NOT KEGS_ADVERTISEMENT_EVENT_INFO_NOT;

DECL_PACKET( EGS_GET_ADVERTISEMENT_EVENT_ACK )
{
	std::vector< std::wstring >			m_vecAdvertisementURL;
};
#endif SERV_ADVERTISEMENT_EVENT


#ifdef SERV_ADD_WARP_BUTTON
DECL_PACKET( EGS_WARP_BY_BUTTON_REQ )
{
	int			m_iCurrentVillageWarpIndex;
	bool		m_bFreeWarp;
};

DECL_PACKET( EGS_WARP_BY_BUTTON_ACK )
{
	int			m_iOK;
	int			m_iED;
	int			m_iWarpPointMapID;
};
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_NEW_EVENT_TYPES

DECL_PACKET( EGS_UPDATE_MAX_LEVEL_NOT )
{
	int									m_iMaxLevel;
};

DECL_PACKET( EGS_UPDATE_DUNGEON_STATUS_NOT )
{
	std::map< int, bool >				m_mapEnableDungeon;
};

#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
DECL_PACKET( EGS_UPDATE_CODE_EVENT_NOT )
{
	std::map< int, bool >				m_mapEnableCodeEnum;
};
#endif SERV_CODE_EVENT

#ifdef SERV_ADD_WARP_BUTTON
DECL_PACKET( EGS_UPDATE_WARP_VIP_NOT )
{
	int				m_iOK;
	__int64			m_trWarpVipEndDate;
};
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
DECL_PACKET( EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_REQ )
{
	bool			m_bCheckClinetDirectPcBang;
	int				m_iIsGameBangType;

#ifdef SERV_ID_NETMARBLE_PCBANG
	std::wstring	m_wstrPublicIP;
	std::wstring	m_wstrMacAdress;
#endif //SERV_ID_NETMARBLE_PCBANG

};
DECL_PACKET( EGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK )
{
	bool			m_bCheckClinetDirectPcBang;
	int				m_iIsGameBangType;
#ifdef SERV_PC_BANG_TYPE
	KPcBangReward	m_kPcBangReward;
#endif SERV_PC_BANG_TYPE
};
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
DECL_PACKET( EGS_OPEN_RANDOM_ITEM_DEVELOPER_REQ )
{
	int							m_iItemID;
	int							m_iItemKeyID;
	int							m_iOpenCount;
};

DECL_PACKET( EGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK )
{
	int									m_iOK;
	std::map<int, KRandomItemResult>	m_map_RandomItemResult;
};
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef SERV_CHECK_TIME_QUEST
DECL_PACKET( EGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT )
{
	std::vector<int>					m_vecWarningQuestID;
};
#endif // SERV_CHECK_TIME_QUEST

#ifdef SERV_MOMOTI_EVENT
DECL_PACKET( EGS_MOMOTI_QUIZ_EVENT_REQ )
{
	int										m_iOK;
	int										m_istrReply;									;
};
DECL_PACKET( EGS_MOMOTI_QUIZ_EVENT_ACK )
{
	int										m_iOK;
	int										m_iCheckReward;
};
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
DECL_PACKET( EGS_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT )
{
	int							m_iUDPPortSuccessType;
	int							m_iUseUdpPort;
};
#endif //SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
#ifdef SERV_SKILL_USE_SUBQUEST
DECL_PACKET( EGS_SKILL_USE_REQ )
{
	int m_iSkillID;
	KEGS_SKILL_USE_REQ()
	{
		m_iSkillID = -1;
	}
};
#endif SERV_SKILL_USE_SUBQUEST

#ifdef SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON
typedef KPacketOK KEGS_SHOW_DISCONNECT_REASON_NOT;
#endif SERV_BLOCK_LIST_SHOW_DISCONNECT_REASON

//{{김준환 서버시간 받아오기
#ifdef	SERV_SERVER_TIME_GET
DECL_PACKET( ECH_GET_SERVER_TIME_ACK )
{
	std::wstring                        m_wstrCurrentTime;
};
#endif  SERV_SERVER_TIME_GET

#ifdef SERV_POINT_COUNT_SYSTEM
DECL_PACKET( EGS_QUEST_POINT_COUNT_SYSTEM_NOT )
{
	std::map< int, KQuestInstance >     m_mapQuestInstance;
};
#endif //SERV_POINT_COUNT_SYSTEM

#ifdef SERV_COUNTRY_PH
DECL_PACKET( EGS_EXCHANGE_CASH_CLIENT_NOT ) // 2013.08.06 lygan_조성욱 // 동남아시아의 경우 전환할때 EGS_EXCHANGE_CASH_REQ 패킷을 못날린다. 그래서 ACK를 받을 수 없기 때문에 NOT로 대신한다.
{
	int									m_iOK;
	int									m_iPoint;
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	KGlobalCashInfo						m_AddBonusCashInfo;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
};
#endif //SERV_COUNTRY_PH

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
DECL_PACKET( EGS_CHANGE_PET_ID_NOT )
{
	int									m_iOK;
	UidType								m_iPetUID;

	KEGS_CHANGE_PET_ID_NOT()
		: m_iOK( 0 ), m_iPetUID( 0 )
	{
	}
};
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_COUPON_EVENT
DECL_PACKET( EGS_COUPON_ITEM_CHECK_REQ )
{
	int				m_iButtonType;
};

DECL_PACKET( EGS_COUPON_ITEM_CHECK_ACK )
{
	int				m_iOK;
	int				m_iCouponType;
	int				m_iItemID;
};

DECL_PACKET( EGS_COUPON_ENTRY_REQ )
{
	int				m_iCouponType;
};

DECL_PACKET( EGS_COUPON_ENTRY_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecUpdatedInventorySlot;
};
#endif //SERV_COUPON_EVENT

#ifdef SERV_GUILD_FIND_AD
typedef KEGS_GET_GUILD_SKILL_IN_BOARD_REQ KEGS_GET_GUILD_SKILL_IN_INVITE_GUILD_LIST_REQ;
typedef KEGS_GET_GUILD_SKILL_IN_BOARD_ACK KEGS_GET_GUILD_SKILL_IN_INVITE_GUILD_LIST_ACK;

DECL_PACKET( EGS_GET_GUILD_FIND_AD_LIST_REQ )
{
	enum FIND_AD_SORT_TYPE
	{
		FAST_REG_DATE = 0,
		FAST_UNIT_LEVEL,

		FAST_MAX,
	};

	char								m_cSortType;
	u_int								m_uiViewPage;
};

DECL_PACKET( EGS_GET_GUILD_FIND_AD_LIST_ACK )
{	
	char									m_cSortType;
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KGuildFindAdInfo >			m_vecGuildFindAdList;
	bool									m_bCanShowInviteUserList;
	bool									m_bCanShowInviteGuildList;
	bool									m_bRegMyGuildFindAd;
};

DECL_PACKET( EGS_REGISTRATION_GUILD_INVITE_MSG_REQ )
{	
	bool									m_bOldInviteMsgDelete;
	UidType									m_iUnitUID;
	char									m_cUnitClass;
	int										m_iUnitLevel;
	std::wstring							m_wstrNickName;
	std::wstring							m_wstrMessage;
};

DECL_PACKET( EGS_REGISTRATION_GUILD_INVITE_MSG_ACK )
{
	int										m_iOK;
	int										m_iED;
	short									m_sPeriod;
	std::wstring							m_wstrNickName;
	std::wstring							m_wstrMessage;
};

DECL_PACKET( EGS_REGISTRATION_GUILD_FIND_AD_REQ )
{
	short									m_sPeriod;
	std::wstring							m_wstrFindAdMessage;
};

DECL_PACKET( EGS_REGISTRATION_GUILD_FIND_AD_ACK )
{
	int										m_iOK;
	int										m_iED;
	short									m_sPeriod;
	std::wstring							m_wstrFindAdMessage;
};

typedef KEGS_REGISTRATION_GUILD_INVITE_MSG_REQ KEGS_MODIFY_REG_GUILD_INVITE_MSG_REQ;
typedef KEGS_REGISTRATION_GUILD_INVITE_MSG_ACK KEGS_MODIFY_REG_GUILD_INVITE_MSG_ACK;
typedef KEGS_REGISTRATION_GUILD_FIND_AD_REQ KEGS_MODIFY_REG_GUILD_FIND_AD_REQ;
typedef KEGS_REGISTRATION_GUILD_FIND_AD_ACK KEGS_MODIFY_REG_GUILD_FIND_AD_ACK;

DECL_PACKET( EGS_GET_GUILD_INVITE_USER_LIST_REQ )
{
	UINT									m_uiViewPage;
};

DECL_PACKET( EGS_GET_GUILD_INVITE_USER_LIST_ACK )
{
	int										m_iOK;
	std::wstring							m_wstrGuildName;
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KGuildInviteMsgInfo >		m_vecGuildInviteUserList;
};

DECL_PACKET( EGS_GET_GUILD_INVITE_GUILD_LIST_REQ )
{
	UINT									m_uiViewPage;
};

DECL_PACKET( EGS_GET_GUILD_INVITE_GUILD_LIST_ACK )
{
	int										m_iOK;
	u_int									m_uiTotalPage;
	u_int									m_uiViewPage;
	std::vector< KGuildInviteMsgInfo >		m_vecGuildInviteGuildList;
};

DECL_PACKET( EGS_ACCEPT_INVITE_REQ )
{
	int										m_iGuildUID;
};

typedef KPacketOK KEGS_ACCEPT_INVITE_ACK;

DECL_PACKET( EGS_CANCEL_INVITE_MSG_REQ )
{
	int										m_iGuildUID;
	UidType									m_iDeletedUnitUID;
};

DECL_PACKET( EGS_CANCEL_INVITE_MSG_ACK )
{
	int										m_iOK;
	bool									m_bCancel;
};

typedef KPacketOK KEGS_DELETE_GUILD_FIND_AD_ACK;
#endif SERV_GUILD_FIND_AD

#ifdef SERV_READY_TO_SOSUN_EVENT
DECL_PACKET( EGS_READY_TO_SOSUN_EVENT_ACK )
{
	int										m_iOK;
	std::vector< KInventoryItemInfo >		m_vecKInventorySlotInfo;
	int										m_iFirstUnitClass;
};
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
DECL_PACKET( EGS_USE_PROPOSE_ITEM_REQ )
{
	UidType									m_iUsedItemUID;
	std::wstring							m_wstrNickName;
};

DECL_PACKET( EGS_USE_PROPOSE_ITEM_ACK )
{
	int										m_iOK;
	std::wstring							m_wstrNickName;
	std::vector< KInventoryItemInfo >		m_vecKInventorySlotInfo;
};

typedef KEGS_COUPLE_PROPOSE_NOT KEGS_EVENT_PROPOSE_NOT;
typedef KEGS_COUPLE_PROPOSE_ACK KEGS_EVENT_PROPOSE_ACK;
typedef KEGS_COUPLE_PROPOSE_AGREE_NOT KEGS_EVENT_PROPOSE_AGREE_NOT;
typedef KEGS_COUPLE_PROPOSE_RESULT_NOT KEGS_EVENT_PROPOSE_RESULT_NOT;

DECL_PACKET( EGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT )
{
	std::vector< KInventoryItemInfo >		m_vecUpdatedInventorySlot;
};

DECL_PACKET( EGS_USE_DIVORCE_ITEM_ACK )
{
	int										m_iOK;
	std::vector< KInventoryItemInfo >		m_vecUpdatedInventorySlot;
};

typedef KEGS_BREAK_UP_NOT KEGS_DIVORCE_NOT;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_EVENT_VC
DECL_PACKET( EGS_USE_INTIMACY_UP_ITEM_REQ )
{
	UidType								m_iItemUID;
};

DECL_PACKET( EGS_USE_INTIMACY_UP_ITEM_ACK )
{
	int m_iOK;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	int m_iUpPercent;
};
#endif //SERV_EVENT_VC

#ifdef SERV_CHINA_SPIRIT_EVENT
DECL_PACKET( EGS_USE_SPIRIT_REWARD_REQ )
{
	int									m_iLocationIndex;
};
DECL_PACKET( EGS_USE_SPIRIT_REWARD_ACK )
{
	int									m_iOK;
	int									m_arrChinaSpirit[6];
	int									m_iLocationIndex;

	KEGS_USE_SPIRIT_REWARD_ACK()
	{
		m_iOK = 0;
		m_arrChinaSpirit[0] = 0;
		m_arrChinaSpirit[1] = 0;
		m_arrChinaSpirit[2] = 0;
		m_arrChinaSpirit[3] = 0;
		m_arrChinaSpirit[4] = 0;
		m_arrChinaSpirit[5] = 0;
		m_iLocationIndex = 0;
	}
};
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_PERIOD_PET
DECL_PACKET( EGS_RELEASE_PET_REQ )
{
	UidType		m_iPetUID;

	KEGS_RELEASE_PET_REQ()
		: m_iPetUID( 0 )
	{
	}
};
DECL_PACKET( EGS_RELEASE_PET_ACK )
{
	UidType		m_iOK;
	UidType		m_iPetUID;

	KEGS_RELEASE_PET_ACK()
		: m_iOK( 0 )
		, m_iPetUID( 0 )
	{
	}
};
DECL_PACKET( EGS_RELEASE_PET_NOT )
{
	UidType		m_iPetUID;
	UidType		m_iUnitUID;
	int			m_iPetID;

	KEGS_RELEASE_PET_NOT()
		: m_iPetUID( 0 )
		, m_iUnitUID( 0 )
		, m_iPetID( 0 )
	{
	}
};
#endif SERV_PERIOD_PET

#ifdef SERV_CUBE_IN_ITEM_MAPPING
DECL_PACKET( EGS_CUBE_IN_ITEM_MAPPING_NOT )
{
	std::map< int, std::map<int, kRandomItemMappingToInfoClient> >	m_mapCubeInItemMapping;

	KEGS_CUBE_IN_ITEM_MAPPING_NOT()
	{
		m_mapCubeInItemMapping.clear();
	}
};
#endif SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_RECRUIT_EVENT_BASE
DECL_PACKET( EGS_USE_RECRUIT_TICKET_REQ )
{
	UidType					m_iItemUID;
};
DECL_PACKET( EGS_USE_RECRUIT_TICKET_ACK )
{
	int						m_iOK;
};

DECL_PACKET( EGS_REGISTER_RECRUITER_REQ )
{
	std::wstring			m_wstrNickname;
};
DECL_PACKET( EGS_REGISTER_RECRUITER_ACK )
{
	int									m_iOK;
	std::vector< KInventoryItemInfo >	m_vecKInventorySlotInfo;
	int									m_iUsedItemID;
};
DECL_PACKET( EGS_REGISTER_RECRUITER_NOT )
{
	UidType					m_iRecruiterUnitUID;
	KRecommendUserInfo		m_kRecruitUnitInfo;
};
DECL_PACKET( EGS_GET_RECRUIT_RECRUITER_LIST_ACK )
{
	int									m_iOK;
	std::vector< KRecommendUserInfo >	m_vecRecruitUnitInfo;
	std::vector< KRecommendUserInfo >	m_vecRecruiterUnitInfo;
};
DECL_PACKET( EGS_RECRUIT_RECRUITER_INFO_NOT )
{
	UidType					m_iSenderUnitUID;
	char					m_cType;
	int						m_iData;
};

#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
DECL_PACKET( EGS_GET_EVENT_INFO_ACK )
{
	int							m_iOK;
	KEventCharacterRanking		m_kEventCharacterRaking;
};
#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_ELESIS_UPDATE_EVENT
DECL_PACKET( EGS_EVENT_NOTE_VIEW_ACK )
{
	int							m_iOK;
	int							m_iTitleID;
	int							m_iNoteViewCount;
};
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_NEW_YEAR_EVENT_2014
DECL_PACKET( EGS_2013_EVENT_MISSION_COMPLETE_REQ )
{
	bool								m_bLevelUpEvent;
};
DECL_PACKET( EGS_2013_EVENT_MISSION_COMPLETE_ACK )
{
	int									m_iOK;
	int									m_iRewardedLevel;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};
typedef KPacketOK		KEGS_2014_EVENT_MISSION_COMPLETE_ACK;
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_GLOBAL_MISSION_MANAGER
DECL_PACKET( EGS_GLOBAL_MISSION_UPDATE_NOT )
{
	std::map< std::wstring, KGlobalMissionInfo > m_mapGlobalMissionInfo;
};
#endif SERV_GLOBAL_MISSION_MANAGER
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
DECL_PACKET( EGS_UNLIMITED_SECOND_CHANGE_JOB_NOT )
{
	int									m_iUnlimitedSecondChangeJob;
};
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_EVENT_CHECK_POWER
DECL_PACKET( EGS_START_CHECK_POWER_REQ )
{
	bool				m_bStart;
};
typedef KPacketOK		KEGS_START_CHECK_POWER_ACK;
DECL_PACKET( EGS_UPDATE_CHECK_POWER_NOT )
{
	unsigned char						m_ucCheckPowerCount;
	__int64								m_iCheckPowerTime;
	unsigned char						m_ucCheckPowerScore;
};
DECL_PACKET( EGS_SET_MULTIPLYER )
{
	float	fM;
};
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
DECL_PACKET( EGS_BUY_UI_SETTING_REQ )
{
	int									m_iTimeControlItemType;
	int									m_iHouseID;
};

DECL_PACKET( EGS_BUY_UI_SETTING_ACK )
{
	int									m_iOK;
	std::set<int>						m_setGetItemOnOff;
	int									m_iHouseID;
	int									m_iTimeControlItemType;

};

DECL_PACKET( EGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK )
{
	std::map<int , std::vector<KPacketGetItemOnOff> > m_mapGetItemOnOff;

};


typedef KPacketOK								KEGS_GET_TIME_CONTROL_ITME_TALK_LIST_REQ;

#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
DECL_PACKET( EGS_EVENT_CHUNG_GIVE_ITEM_NOT )
{
	bool m_bGiveItemGet;
	std::wstring m_wstrToolTipTime;
	char m_cGetUnitClass;
	bool m_bTwoGiveItem;
	KEGS_EVENT_CHUNG_GIVE_ITEM_NOT()
	{
		m_bGiveItemGet = false;
		m_wstrToolTipTime = L"";
		m_bTwoGiveItem = false;
	}
};
DECL_PACKET( EGS_EVENT_CHUNG_GIVE_ITEM_REQ )
{
	int iChoice;
	bool bTwoGiveItem;
	KEGS_EVENT_CHUNG_GIVE_ITEM_REQ()
	{
		iChoice = 0;
		bTwoGiveItem = false;
	}
};
DECL_PACKET( EGS_EVENT_CHUNG_GIVE_ITEM_ACK )
{
	int m_iOK;
	std::wstring  m_wstrGetItemTime;
	KEGS_EVENT_CHUNG_GIVE_ITEM_ACK()
	{
		m_iOK = 0;
		m_wstrGetItemTime = L" ";
	}
};
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
DECL_PACKET( EGS_EVENT_COBO_DUNGEON_FIELD_NOT )
{
	bool								m_StartButtonUI;
	bool								m_DungeonCountUI;
	bool								m_FieldCountUI;
	int									m_DungeonCount;
	int									m_FieldMonsterKillCount;
	int									m_iOk;
	int									m_iRemaindTime;
	__time64_t							m_tPushTime;
	KEGS_EVENT_COBO_DUNGEON_FIELD_NOT()
	{
		m_StartButtonUI = false;
		m_DungeonCountUI = false;
		m_FieldCountUI	=	false;
		m_DungeonCount	= 0;
		m_FieldMonsterKillCount = 0;
		m_iOk = 0;
		m_iRemaindTime = -1;
		m_tPushTime = 0;
	}
};
DECL_PACKET( EGS_EVENT_COBO_DUNGEON_FIELD_REQ )
{
	int								m_iOK;
	bool							m_EventStart;
	KEGS_EVENT_COBO_DUNGEON_FIELD_REQ()
	{
		m_iOK = 0;
		m_EventStart = false;
	}
};
DECL_PACKET( EGS_EVENT_COBO_DUNGEON_FIELD_ACK )
{
	bool								m_bStartUI;
	bool								m_DungeonCountUI;
	bool								m_FieldCountUI;
	int									m_DungeonCount;
	int									m_FieldMonsterKillCount;
	int									m_iOK;
	int									m_iRemaindTime;
	std::wstring						m_wstrPushTime;
	__time64_t							m_tPushTime;
	KEGS_EVENT_COBO_DUNGEON_FIELD_ACK()
	{
		m_bStartUI = false;
		m_DungeonCountUI = false;
		m_FieldCountUI = false;
		m_DungeonCount = 0;
		m_FieldMonsterKillCount = 0;
		m_iOK = 0;
		m_iRemaindTime = -1;
		m_wstrPushTime = L"";
		m_tPushTime = 0;
	}
};
DECL_PACKET( EGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT )
{
	int						m_iDungeonClearCount;

	KEGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT()
	{
		m_iDungeonClearCount = 0;
	}

};
DECL_PACKET( EGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT )
{
	int					   m_iFieldMonsterKillCount;

	KEGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT()
	{
		m_iFieldMonsterKillCount = 0;
	}
};
DECL_PACKET( EGS_EVENT_COBO_ITEM_GIVE_CHEAT_NOT )
{
	bool					   m_CoboEventITemGet;
	bool					   m_bNextDay;

	KEGS_EVENT_COBO_ITEM_GIVE_CHEAT_NOT()
	{
		m_CoboEventITemGet = false;
		m_bNextDay		   = false;
	}
};
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
DECL_PACKET( EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT )
{
	int					   m_iValentineItemCount;
	KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT()
	{
		m_iValentineItemCount = -1;
	}
};
DECL_PACKET( EGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_CHEAT_NOT )
{
	int					   m_iValentineItemCount;
	KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_CHEAT_NOT()
	{
		m_iValentineItemCount = -1;
	}
};
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
DECL_PACKET( EGS_CASH_DIRECT_CHARGE_CN_REQ )
{
	UidType				m_iUserUID;
};

DECL_PACKET( EGS_CASH_DIRECT_CHARGE_CN_ACK )
{
	int					m_iOK;
	std::wstring		m_wstrToken;
};

DECL_PACKET( EGS_CASH_DIRECT_CHARGE_CN_NOT )
{
	int					m_iOK;
};
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#ifdef SERV_MANUFACTURE_PERIOD_FIX
DECL_PACKET( EGS_MANUFACTURE_PERIOD_SETTING_REQ )
{
	int									m_iHouseID;
};
DECL_PACKET( EGS_MANUFACTURE_PERIOD_SETTING_ACK )
{
	int									m_iOK;
	std::map<int , int>					m_mapPeriodGroup;
	int									m_iHouseID;
};
#endif //SERV_MANUFACTURE_PERIOD_FIX

#ifdef SERV_4TH_ANNIVERSARY_EVENT
DECL_PACKET( EGS_4TH_ANNIV_EVENT_REWARD_REQ )
{
	int					m_iSeletedIndex;	// 선택한 사진 번호
};

DECL_PACKET( EGS_4TH_ANNIV_EVENT_REWARD_ACK )
{
	int					m_iOK;
	int					m_iSeletedIndex;
};
#endif // SERV_4TH_ANNIVERSARY_EVENT

#pragma pack( pop )
