#pragma once

//#include <Serializer/SerializeMacro.h>
//#include "KncUidType.h"
#include "CommonPacket.h"

#pragma pack( push, 1 )

DECL_DATA( KServerInfo )
{
	//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
	enum SERVER_ROLL_TYPE
	{
		SRT_NONE						= 0,
		SRT_UPDATE_CASH_PRODUCT_LIST	= ( 1 << 0 ),
		SRT_LOGIN_SERVER_SCRIPT_REFRESH	= ( 1 << 1 ),
	};
#endif SERV_SERVER_ROLL_TYPE
	//}}

    int                         m_iUID;
    std::wstring                m_wstrName;
    int                         m_iServerGroupID;
    int                         m_iServerClass;
	//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
	std::wstring                m_wstrPublicIP;
	std::wstring                m_wstrPrivateIP;
#else
	std::wstring                m_wstrIP;
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
	//}}
    u_short                     m_usMasterPort;
    u_short                     m_usNCUDPPort;
    int                         m_nMaxUser;
    int                         m_nCurrentUser;
    bool                        m_bOn;
	//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
	char						m_cServerRollType;
#endif SERV_SERVER_ROLL_TYPE
	//}}

	KServerInfo()
	{
		m_iUID				= 0;
		m_iServerGroupID	= 0;
		m_iServerClass		= 0;		
		m_usMasterPort		= 0;
		m_usNCUDPPort		= 0;
		m_nMaxUser			= 0;
		m_nCurrentUser		= 0;
		m_bOn				= false;
		//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
		m_cServerRollType	= SRT_NONE;
#endif SERV_SERVER_ROLL_TYPE
		//}}
	}

	COPYCON_ASSIGNOP( KServerInfo, right )
	{
		m_iUID = right.m_iUID;
		m_wstrName = right.m_wstrName;
		m_iServerGroupID = right.m_iServerGroupID;
		m_iServerClass = right.m_iServerClass;
		//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
		m_wstrPublicIP = right.m_wstrPublicIP;
		m_wstrPrivateIP = right.m_wstrPrivateIP;
#else
		m_wstrIP = right.m_wstrIP;
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
		//}}
		m_usMasterPort = right.m_usMasterPort;
		m_usNCUDPPort = right.m_usNCUDPPort;
		m_nMaxUser = right.m_nMaxUser;
		m_nCurrentUser = right.m_nCurrentUser;
		m_bOn = right.m_bOn;
		//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
		m_cServerRollType = right.m_cServerRollType;
#endif SERV_SERVER_ROLL_TYPE
		//}}

		return *this;
	}
};

//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
DECL_PACKET( ECN_VERIFY_SERVER_CONNECT_REQ )
{
    KNetAddress					m_kNetAddress;
	int							m_iServerGroupID;
};
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

DECL_PACKET( DBE_UPDATE_SERVER_INFO_REQ )
{
    std::wstring                m_wstrRegDate;
    UidType                     m_iServerUID;
    int                         m_iConcurrentUser;
	//{{ 2009. 4. 1  최육사		파티정보
	int							m_iCurPartyCount;
	int							m_iPlayGamePartyCount;
	//}}
};

// 해외는 Nexon은 아니지만 이름 수정 시 건들려야 하는 곳이 많아서 그냥 Nexon으로 둠
DECL_PACKET( ELG_NEXON_USER_AUTHENTICATE_REQ )
{
    KUserAuthenticateReq        m_kAuthenticateReq;
    KNexonAccountInfo           m_kNexonAccountInfo;

#ifdef SERV_KOG_OTP_VERIFY
	bool						m_bServerUseKogOTP;
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
	unsigned short				m_usZone;
	unsigned short				m_usGame;
#endif //SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
	std::wstring				m_wstrSocketID;
#endif // SERV_COUNTRY_TH

	KELG_NEXON_USER_AUTHENTICATE_REQ()
	{
#ifdef SERV_KOG_OTP_VERIFY
		m_bServerUseKogOTP	= false; 
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
		m_usZone			= 0;
		m_usGame			= 0;
#endif // SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		m_wstrSocketID		= L"";
#endif // SERV_COUNTRY_TH
	}
};

// 해외는 Nexon은 아니지만 이름 수정 시 건들려야 하는 곳이 많아서 그냥 Nexon으로 둠
DECL_PACKET( ELG_NEXON_USER_AUTHENTICATE_ACK )
{
    int                         m_iOK;
    KAccountInfo                m_kAccountInfo;
    KNexonAccountInfo           m_kNexonAccountInfo;

	char						m_cHackingUserType;
	//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
	bool						m_bJoinAccount;
#endif SERV_IP_ACCOUNT_LOG
	//}}
	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	std::map< int, int >		m_mapAccCountInfo;
#endif SERV_ACCOUNT_COUNT
	//}}
#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
	unsigned int				m_uiGiantAccountType; //2011.11.01 lygan_조성욱 // 채널이동시에 자이언트 계정 타입을 알기위해 삽입. 
#endif SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
#ifdef SERV_EPAY_SYSTEM
	unsigned short				m_usZone;
	unsigned short				m_usGame;
#endif //SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
	std::wstring				m_wstrSocketID;
#endif // SERV_COUNTRY_TH
#ifdef SERV_COUNTRY_PH
	unsigned short				m_usGarenaCyberCafe;
#endif //SERV_COUNTRY_PH
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	int                         m_iEventMoney;

	KELG_NEXON_USER_AUTHENTICATE_ACK()
	{
		m_iEventMoney = 0;
	}
#endif // SERV_EVENT_MONEY
};

//{{ 2010. 06. 29  최육사	게임 PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW

DECL_PACKET( ELG_NEXON_USER_LOGIN_TIME_OUT_NOT )
{
	std::wstring				m_wstrUserID;
};

#endif SERV_PCBANG_AUTH_NEW
//}}

//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
DECL_PACKET( DBE_SELECT_UNIT_ACK )
{
	int                                     m_iOK;
	KUnitInfo                               m_kUnitInfo;
	//{{ 2011. 01. 18	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	std::wstring							m_wstrUnitCreateDate;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
	int										m_iRealDataED;
#endif SERV_LOGOUT_ED_CHECK
	//}}
	//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
	std::wstring							m_wstrUnitLastLoginDate;
#endif SERV_DAILY_CHAR_FIRST_SELECT
	//}}

	// 카테고리, 슬롯 갯수
	std::map< int, int >                    m_mapInventorySlotSize;
	std::map< UidType, KInventoryItemInfo > m_mapItem;
	//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	std::map< UidType, std::map< UidType, KInventoryItemInfo > > m_mapPetItem; // 서버 전용
#endif SERV_PET_SYSTEM
	//}}

	int										m_iNumResurrectionStone; // 부활석
	//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
	int										m_iNumAutoPaymentResStone;
	std::wstring							m_wstrAutoPayResStoneLastDate;
#endif AP_RESTONE
	//}}

	std::vector< KQuestInstance >			m_vecQuest;
	//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
	std::vector< KCompleteQuestInfo >		m_vecCompletQuest;
#else
	std::vector< int >						m_vecCompletQuest;
#endif SERV_DAILY_QUEST
	//}}	

	//{{ 2008. 10. 6  최육사	타이틀
	std::vector< KMissionInstance >			m_vecMission;
	std::vector< KTitleInfo >				m_vecTitle;
	//}}
	
#ifdef SERV_SKILL_PAGE_SYSTEM
 	std::vector<KUserSkillPageData>			m_vecUserSkillPageData;		// 획득한 스킬 리스트
#else // SERV_SKILL_PAGE_SYSTEM
	std::vector< KUserSkillData >			m_vecSkillAcquired;			// 획득한 스킬 리스트
#endif // SERV_SKILL_PAGE_SYSTEM

	std::vector< short >					m_vecSkillUnsealed;			// 봉인해제된 스킬 리스트

	//{{ 2010. 03. 13  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
	char									m_cSkillNoteMaxPageNum;
	std::map< char, int >					m_mapSkillNote;				// [key:SlotID, value:MemoID]
#endif SERV_SKILL_NOTE
	//}}

	std::vector< KChatBlackListUnit >		m_vecChatBlackList;
	KDenyOptions							m_kDenyOptions;

	//{{ 2008. 3. 31  최육사  추천인
	bool									m_bIsRecommend;
	UidType									m_iRecommendUnitUID;
	//}}

	//080405. hoons. 사제리스트 데이터
	std::vector< KTutorialDBUnitInfo >		m_vecTutorialDBUnitInfo;

	//{{ 2008. 5. 7  최육사  통계
	bool									m_bIsSpiritUpdated;
	//}}

	//{{ 2008. 5. 19  최육사  접속 시간 이벤트
	std::vector< KConnectTimeEventInfo >	m_vecConnectTimeEvent;
	//}}
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	std::vector< KCumulativeTimeEventInfo > m_vecCumulativeTimeEvent;
#endif CUMULATIVE_TIME_EVENT
	//}}

	//{{ 2009. 7. 7  최육사		랭킹개편
	std::vector< KHenirRankingInfo >		m_vecHenirRanking;
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
	std::vector< KHenirRankingInfo >		m_vecHenirHeroRanking;
#endif // SERV_HENIR_RENEWAL_2013
	//}}

	KMessengerInfo                          m_kMessengerInfo;

	//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	UidType									m_iSummonedPetUID;
	std::vector< KPetInfo >					m_vecPetList;
	std::vector< UidType >					m_vecCanEvolutionPetList;
	std::vector< UidType >					m_vecNeverSummonPetList;
#endif SERV_PET_SYSTEM
	//}}

	//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
	std::map< int, int >					m_mapCharGameCount;
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	KUserPShopAgencyInfo					m_kPShopAgencyInfo;
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
	KEGS_HENIR_REWARD_COUNT_NOT				m_PacketHenirRewardCount;
#endif SERV_NEW_HENIR_TEST
	//}}

	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
	std::map< int, int >					m_mapRandomQuestList;
#endif SERV_RANDOM_DAY_QUEST
	//}}
	//{{ 2011. 10. 13	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
	bool									m_bCashShopOpen;
#endif SERV_CONTENT_MANAGER
	//}}
	//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	KGamePlayStatusContainer				m_kGamePlayStatus;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	int										m_iEventMoney;
#endif // SERV_EVENT_MONEY

	//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
	int										m_iPvpEventIndex;
	std::wstring							m_wstrLastPvpEventDate;
#endif SERV_2012_PVP_SEASON2_EVENT
	//}}

	//{{ 2012. 10. 10	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	KDBE_BINGO_EVENT_INFO_READ_ACK			m_kBingoEvent;
#endif SERV_EVENT_BINGO
	//}}

	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	std::vector< KQuestInstance >			m_vecAccountQuesting;		// 진행 중인 계정 단위 퀘스트
	std::vector< KCompleteQuestInfo >		m_vecAccountCompleteQuest;  // 완료한 계정 단위 퀘스트
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}
	//{{ 2012. 12. 20	최육사	아라 첫 선택 튜토리얼
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
	bool									m_bFirstSelect;
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
	//}}
#ifdef SERV_ADVERTISEMENT_EVENT
	std::vector< int > m_vecAdvertisementEvent;
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_CUSTOM_CONNECT_EVENT
	int										m_iCustomEventID;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
	int m_iConnectExperienceAck;
	int m_iReward7DaysItem;
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	int m_iNewUnitE;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
	KLocalRankingUserInfo					m_kLocalRankingUserInfo;
	std::wstring							m_wstrLocalRankingUserInfoUpdated;
#endif	// SERV_LOCAL_RANKING_SYSTEM

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	KRelationshipInfo						m_kRelationshipInfo;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
    UidType                         	    m_nWrongUnitUID;
#ifdef SERV_BLESS_OF_GODDESS_EVENT
	bool									m_bMaxLevelUnitInAccount;
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
	int										m_iGateOfDarknessSupportEventTime;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
	bool									m_bCouple;
	UidType									m_iRelationTargetUserUid;
	std::wstring							m_wstrRelationTargetUserNickname;
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
	std::vector< KRecommendUserInfo >		m_vecRecruiterUnitInfo;
	std::vector< KRecommendUserInfo >		m_vecRecruitUnitInfo;
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_ELESIS_UPDATE_EVENT
	int										m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
	std::wstring							m_wstrGiveMeTheItemTime_One;
	std::wstring							m_wstrGiveMeTheItemTime_Two;
	std::wstring							m_wstrGiveMeTheItemTime_Tree;
#endif SERV_EVENT_CHUNG_GIVE_ITEM 
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
	std::wstring						   m_wstrButtonClickTime_One; //1주차 버튼 클릭 시간 저장 타임
	bool								   m_bItemGive; //2주차 버튼 클릭 시간 저장 타임
	int									   m_iDungeonClearCount;
	int									   m_iFieldMonsterKillCount;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
	int									   m_iValentineGiveItemCount;
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_GLOBAL_EVENT_TABLE
	std::map< int, KGlobalEventTableData >	m_mapGlobalEventData;
#endif //SERV_GLOBAL_EVENT_TABLE
	KDBE_SELECT_UNIT_ACK():m_nWrongUnitUID(0)
	{
		m_iOK = 0;
		//{{ 2012. 12. 20	최육사	아라 첫 선택 튜토리얼
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
		m_bFirstSelect = false;
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
		//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
		m_iCustomEventID = 0;
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		m_iConnectExperienceAck = 0;
		m_iReward7DaysItem = 0;
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_BLESS_OF_GODDESS_EVENT
		m_bMaxLevelUnitInAccount = false;
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		m_iNewUnitE = -1;
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		m_iGateOfDarknessSupportEventTime = 0;
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
#ifdef SERV_RELATIONSHIP_EVENT_INT
		m_bCouple = false;
		m_iRelationTargetUserUid = 0;
		m_wstrRelationTargetUserNickname = L"";
#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_ELESIS_UPDATE_EVENT
		m_iNoteViewCount = 0;
#endif SERV_ELESIS_UPDATE_EVENT
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		m_wstrGiveMeTheItemTime_One	= L"1900-01-01 00:00:00";
		m_wstrGiveMeTheItemTime_Two	= L"1900-01-01 00:00:00";
		m_wstrGiveMeTheItemTime_Tree	= L"1900-01-01 00:00:00";
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		m_wstrButtonClickTime_One		= L"1900-01-01 00:00:00";
		m_bItemGive					= false;
		m_iDungeonClearCount		= 0;
		m_iFieldMonsterKillCount	= 0;
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		m_iValentineGiveItemCount   = -1; //발렌타인 던전 보상 카운트
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_GLOBAL_EVENT_TABLE
		m_mapGlobalEventData.clear();
#endif //SERV_GLOBAL_EVENT_TABLE
	}
};
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}

//{{ 2009. 5. 26  최육사	채널이동
DECL_PACKET( ECN_CHECK_CHANNEL_CHANGE_REQ )
{
	UidType						m_iSourGSUID;
	UidType						m_iDestGSUID;
    UidType						m_iUserUID;
	UidType						m_iCharUID;
	KChannelChangeInfo			m_kChangeUserInfo;
};

DECL_PACKET( ECN_CHECK_CHANNEL_CHANGE_ACK )
{
	int							m_iOK;
	UidType						m_iSourGSUID;
	UidType						m_iCharUID;
};

DECL_PACKET( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ )
{
	KEGS_CONNECT_ACK					m_kConnectAck;
	KELG_NEXON_USER_AUTHENTICATE_REQ	m_kUserAuthReq;
	KEGS_SELECT_UNIT_REQ				m_kSelectUnitReq;
	KChannelChangeInfo					m_kChangeUserInfo;
#ifdef SERV_KOG_OTP_VERIFY
	bool								m_bServerUseKogOTP;
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
	unsigned short						m_usZone;
	unsigned short						m_usGame;
	unsigned int						m_uiGiantAccountType;
#endif //SERV_EPAY_SYSTEM

	KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ()
	{
#ifdef SERV_KOG_OTP_VERIFY
		m_bServerUseKogOTP	= false;
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
		m_usZone			= 0;
		m_usGame			= 0;
		m_uiGiantAccountType = 0;
#endif //SERV_EPAY_SYSTEM
	}
};

DECL_PACKET( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK )
{
	KEGS_CONNECT_ACK					m_kConnectAck;
	KELG_NEXON_USER_AUTHENTICATE_ACK	m_kUserAuthAck;
	KEGS_SELECT_UNIT_REQ				m_kSelectUnitReq;
	KChannelChangeInfo					m_kChangeUserInfo;
	//{{ 2012. 02. 21	김민성	2차 보안 채널 이동시(미니맵) 오류 수정
#ifdef SERV_SECOND_SECURITY_PW_ERROR_MODIFY
	bool					m_bUseSecondPW;
	std::wstring			m_wstrSecondPW;
	int						m_iFailedCount;
	std::wstring			m_wstrLastAuthDate;
#endif SERV_SECOND_SECURITY_PW_ERROR_MODIFY
	//}}
#ifdef SERV_EPAY_SYSTEM
	unsigned short			m_usZone;
	unsigned short			m_usGame;
	unsigned int			m_uiGiantAccountType;
#endif //SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
	std::wstring						m_wstrSocketID;
#endif // SERV_COUNTRY_TH

#ifdef SERV_COUNTRY_PH
	unsigned short				m_usGarenaCyberCafe;
#endif //SERV_COUNTRY_PH

	KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK()
	{
#ifdef SERV_SECOND_SECURITY_PW_ERROR_MODIFY
		m_bUseSecondPW = false;
		m_iFailedCount = 0;
#endif // SERV_SECOND_SECURITY_PW_ERROR_MODIFY

#ifdef SERV_EPAY_SYSTEM
		m_usZone		= 0;
		m_usGame		= 0;
		m_uiGiantAccountType = 0;
#endif // SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		m_wstrSocketID		= L"";
#endif // SERV_COUNTRY_TH

#ifdef SERV_COUNTRY_PH
		m_usGarenaCyberCafe	= 0;
#endif //SERV_COUNTRY_PH
	}

};

DECL_PACKET( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ )
{
	KEGS_CONNECT_ACK					m_kConnectAck;
	KEGS_VERIFY_ACCOUNT_ACK				m_kVerifyAccountAck;
    KEGS_SELECT_UNIT_REQ				m_kSelectUnitReq;
	KChannelChangeInfo					m_kChangeUserInfo;
};

DECL_PACKET( DBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK )
{
	KEGS_CONNECT_ACK					m_kConnectAck;
	KEGS_VERIFY_ACCOUNT_ACK				m_kVerifyAccountAck;

	// [참고] 캐릭터 정보 크기가 굉장히 크지만 서버 내부에서만 사용하는 이벤트이기 때문에 이벤트 크기로 인한 문제는 없음
	//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
	KDBE_SELECT_UNIT_ACK				m_kSelectUnitAck;
#else
	KEGS_SELECT_UNIT_ACK				m_kSelectUnitAck;
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	//}}
	
	KChannelChangeInfo					m_kChangeUserInfo;
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
	int									m_iNewUnitCh;

	KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK()
	{
		m_iNewUnitCh = -1;
	};
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

};
//}}

typedef std::map< int, std::vector< KServerInfo > > KServerList;

DECL_DATA( KSquareCreateInfo )
{
	int				m_iSquareType;
	//{{ 2010. 11. 1	최육사	광장 리스트 개선
#ifdef SERV_SQUARE_LIST_SERV_UID
	char			m_cSortIndex;
#endif SERV_SQUARE_LIST_SERV_UID
	//}}
	std::wstring	m_wstrSquareName;

	CON_COPYCON_ASSIGNOP( KSquareCreateInfo, right )
	{
		m_iSquareType		= right.m_iSquareType;
		//{{ 2010. 11. 1	최육사	광장 리스트 개선
#ifdef SERV_SQUARE_LIST_SERV_UID
		m_cSortIndex		= right.m_cSortIndex;
#endif SERV_SQUARE_LIST_SERV_UID
		//}}
		m_wstrSquareName	= right.m_wstrSquareName;

		return *this;
	}
};

//{{ 2008. 11. 4  최육사	광장 개편
typedef KSquareCreateInfo		KFieldCreateInfo;
//}}

typedef KSquareUserInfo KERM_JOIN_SQUARE_REQ;

DECL_DATA( KDBE_CENTER_SERVER_ON_ACK )
{
	int                                 m_iOK;
	KServerInfo                         m_kServerInfo;
	int									m_iProxyID;
	std::vector<KSquareCreateInfo>		m_vecSquareList;
};

DECL_DATA( KDBE_LOGIN_SERVER_ON_ACK )
{
	int                                 m_iOK;
	int									m_iProxyID;
	KServerInfo                         m_kServerInfo;
};

DECL_DATA( KDBE_GAME_SERVER_ON_ACK )
{
	int                                 m_iOK;
	KServerInfo                         m_kServerInfo;

	std::map< int, KNetAddress >		m_mapCenterList;
	std::map< int, KNetAddress >		m_mapLoginList;
	//{{ 2009. 5. 5  최육사		채널id
	int									m_iChannelID;

	//{{ 2012. 05. 16	박세훈	채널 리스트 정보 통합
#ifdef SERV_INTEGRATE_THE_CHANNEL_INFO
#else
	KChannelBonusInfo					m_kBonusInfo;
#endif SERV_INTEGRATE_THE_CHANNEL_INFO
	//}}

	//}}

#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
	std::map< std::wstring, int >		m_mapPCBangIPList;	// IP, PCBangLevel
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST 

	//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
	std::map< int, KNetAddress >		m_mapGlobalList;
//#endif SERV_INSERT_GLOBAL_SERVER
	//}} 2011. 04. 13  김민성  글로벌 서버 추가
};

#ifdef	SERV_FROM_CHANNEL_TO_LOGIN_PROXY
DECL_DATA( KDBE_CHANNEL_SERVER_ON_ACK )
{
	int                                 m_iOK;
	int									m_iProxyID;
	KServerInfo                         m_kServerInfo;
	std::map< int, KNetAddress >		m_mapLoginList;
};
#else // SERV_FROM_CHANNEL_TO_LOGIN_PROXY
//{{ 2009. 1. 13  최육사	채널 서버
typedef KDBE_LOGIN_SERVER_ON_ACK		KDBE_CHANNEL_SERVER_ON_ACK;
//}}
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY

DECL_PACKET( ERM_END_GAME_NOT )
{
    int                                 m_iWinTeam;
};

DECL_PACKET( ERM_GET_ITEM_ACK )
{
    int                                 m_iOK;
    KItemInfo                           m_kItemInfo;
};

//{{ 2009. 5. 11  최육사	실시간아이템
DECL_PACKET( DBE_GET_ITEM_INSERT_TO_INVENTORY_REQ )
{
	UidType								m_iUnitUID;
	std::map< int, int >				m_mapInsertItem;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >			m_vecItemInfo;

	KEGS_GET_ITEM_NOT					m_kBroadInfo;
};

DECL_PACKET( DBE_GET_ITEM_INSERT_TO_INVENTORY_ACK )
{
	int                                 m_iOK;
	std::map< int, int >				m_mapInsertItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;

	KEGS_GET_ITEM_NOT					m_kBroadInfo;
};

DECL_PACKET( ERM_GET_ITEM_COMPLETE_NOT )
{
	UidType								m_iUnitUID;
	std::map< int, int >				m_mapGetItem;
};

DECL_PACKET( DBE_GET_TEMP_ITEM_REQ )
{
	UidType								m_iUnitUID;
	int									m_iTempItemUID;
	std::map< int, int >				m_mapInsertItem;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >			m_vecItemInfo;
};

DECL_PACKET( DBE_GET_TEMP_ITEM_ACK )
{
	int                                 m_iOK;
	int									m_iTempItemUID;
	std::map< int, int >				m_mapInsertItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
};
//}}

DECL_PACKET( ELOG_STAT_PLAY_TIME )
{
    UidType                             m_iUserUID;
    std::wstring                        m_wstrIP;
    std::wstring                        m_wstrLoginTime;
    std::wstring                        m_wstrLogoutTime;	
    bool                                m_bLoginFail;
    bool                                m_bLogoutFail;
	int									m_iTotalPlayTime;
	int									m_iPvpPlayTime;
	//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
	int									m_iDungeonPlayTime;
	int									m_iFieldPlayTime;
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}
	int									m_iDisconnectReason;
	int									m_iDisconnectUserFSM;
	//{{ 2009. 12. 22  최육사	서버군확장
	int									m_iServerGroupID;
	//}}
	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
	std::wstring						m_wstrMachineID;
#endif SERV_MACHINE_ID_BLOCK
	//}}
	//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
	std::wstring						m_wstrUnitID;
	//bool                                m_bIsPcbang;
	UidType								m_iUnitUID;
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}
#ifdef SERV_CONNECT_LOG_CHANNELING
	unsigned char						m_ucChannelCode;
#endif SERV_CONNECT_LOG_CHANNELING

	bool                                m_bIsPcbang;	// 해외도 필요해서 밖으로 뺌

#ifdef SERV_STEAM
	bool								m_bSteamClient;	// 스팀 클라
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
	bool								m_bAeriaClient;
#endif //SERV_CHANNELING_AERIA

	KELOG_STAT_PLAY_TIME()
		: m_iUserUID( 0 )
		, m_iTotalPlayTime( 0 )
		, m_iPvpPlayTime( 0 )
	//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
		, m_iDungeonPlayTime( 0 )
		, m_iFieldPlayTime( 0 )
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}
#ifdef SERV_CONNECT_LOG_CHANNELING
		, m_ucChannelCode( 0 )
#endif SERV_CONNECT_LOG_CHANNELING
		, m_bIsPcbang( false )
#ifdef SERV_STEAM
		, m_bSteamClient( false )
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		, m_bAeriaClient( false )
#endif //SERV_CHANNELING_AERIA
	{
	}
};

DECL_PACKET( DBE_BUY_ED_ITEM_REQ )
{
    UidType                             m_iUnitUID;
    KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::map< UidType, int >			m_kItemEnduranceUpdate;	
    std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
    std::vector< KBuyGPItemInfo >       m_vecBuyGPItemInfo;
#ifdef SERV_GET_ITEM_REASON_BY_CHEAT
	bool								m_bCheat;
#endif SERV_GET_ITEM_REASON_BY_CHEAT
};

DECL_PACKET( DBE_BUY_ED_ITEM_ACK )
{
    int                                 m_iED;
	//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	int                                 m_iAPoint;
#else
	int                                 m_iVSPoint;
#endif SERV_PVP_NEW_SYSTEM
	//}}
    KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::map< UidType, int >			m_kItemEnduranceUpdate;	
    std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
    std::map< UidType, KItemInfo >      m_mapItemInfo;
};

DECL_PACKET( DBE_INSERT_ITEM_REQ )
{
    UidType                             m_iUnitUID;
	//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
	char								m_cGetItemReason;
#endif SERV_GET_ITEM_REASON
	//}}
    std::map< int, int >                m_mapInsertedItem;
    KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::map< UidType, int >			m_kItemEnduranceUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
    std::vector< KItemInfo >            m_vecItemInfo;
	bool								m_bOutRoom;		//게임종료시 이패킷을 이용한는 경우에 서버업데이트 유무를 가리기 위해.
	//{{ 2009. 9. 2  최육사		밀봉
	std::set< int >						m_setSealCashItem;
	//}}

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	std::map< int, int >				m_mapGetItem;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

	KDBE_INSERT_ITEM_REQ()
	{
		m_iUnitUID = 0;
		m_bOutRoom = false;
		//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
		m_cGetItemReason = 0;
#endif SERV_GET_ITEM_REASON
		//}}
	}
};

DECL_PACKET( DBE_INSERT_ITEM_ACK )
{
    std::map< int, int >                m_mapInsertedItem;
    KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::map< UidType, int >			m_kItemEnduranceUpdate;	
    std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
    std::map< UidType, KItemInfo >      m_mapItemInfo;
	bool								m_bOutRoom;		//게임종료시 이패킷을 이용한는 경우에 서버업데이트 유무를 가리기 위해.

	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	std::map< int, int >                m_mapGetItem;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}

	KDBE_INSERT_ITEM_ACK()
	{
		m_bOutRoom = false;
	}
};

//{{ 2008. 7. 29  최육사	캐쉬 아이템
DECL_PACKET( DBE_INSERT_PURCHASED_CASH_PACKAGE_REQ )
{
	UidType                             m_iUnitUID;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::map< UidType, int >			m_kItemEnduranceUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
	unsigned long						m_ulProductNo;
	//{{ 2009. 9. 2  최육사		밀봉
	std::set< int >						m_setSealCashItem;
	//}}

	KDBE_INSERT_PURCHASED_CASH_PACKAGE_REQ()
	{
		m_iUnitUID = 0;
	}
};

DECL_PACKET( DBE_INSERT_PURCHASED_CASH_PACKAGE_ACK )
{
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::map< UidType, int >			m_kItemEnduranceUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	unsigned long						m_ulProductNo;
};
//}}

DECL_PACKET( ELOG_CHAT_LOG_NOT )
{
    UidType         m_nRoomUID;
    UidType         m_nCharUID;
    UidType         m_nUserUID;
    char            m_cChatType;
    std::wstring    m_strMsg;
};

typedef UidType KDBE_UPDATE_LOGIN_TIME_NOT;

//DECL_PACKET( ERM_GAME_PLAY_TIME_NOT )
//{
//    char        m_cRoomType;        // 대전 / 던전 여부.
//    int         m_nPlayTime;        // minute.
//};

DECL_PACKET( DBE_CHARACTER_LEVELUP_NOT )
{
    UidType         m_nCharUID; 
    unsigned char   m_ucLevelBefore;
    unsigned char   m_ucLevelAfter;
};

//////////////////////////////////////////////////////////////////////////
// Party
DECL_PACKET( ERM_PARTY_OPEN_DUNGEON_ROOM_REQ )
{
	KRoomInfo							m_kRoomInfo;
	short								m_sWorldID;
	std::vector< KPartyRoomUserInfo >	m_vecUserInfo;	

	KERM_PARTY_OPEN_DUNGEON_ROOM_REQ()
	{
		m_sWorldID = 0;
	}
};

typedef KPacketOK	KERM_PARTY_OPEN_DUNGEON_ROOM_ACK;

//{{ 2009. 3. 18  최육사	파티유저밴
typedef KPacketOK	KERM_PARTY_BAN_USER_ACK;
//}}

// Party
//////////////////////////////////////////////////////////////////////////

//{{ 2007. 8. 9  최육사  블랙리스트 패킷구조체
DECL_PACKET( DBE_NEW_BLACKLIST_USER_REQ )
{
	UidType				m_iUnitUID;
	UidType				m_iBlackListUnitUID;
};

DECL_PACKET( DBE_NEW_BLACKLIST_USER_ACK )
{
	int					m_iOK;
	KChatBlackListUnit	m_kChatBlackListUnit;
};

DECL_PACKET( DBE_DEL_BLACKLIST_USER_REQ )
{
	UidType				m_iUnitUID;
	UidType				m_iBlackListUnitUID;
};

DECL_PACKET( DBE_DEL_BLACKLIST_USER_ACK )
{
	int					m_iOK;
	UidType				m_iBlackListUnitUID;
};
//}}

//{{ 2007. 8. 20  최육사  채팅 통합 : ERM_CHAT_ACK 새로 만듬
DECL_PACKET( ERM_CHAT_ACK )
{
	int					m_iOK;
	char				m_cChatPacketType;
};
//}}

typedef KSquareCreateInfo	KERM_OPEN_SQUARE_NOT;

DECL_PACKET( ERM_REQUEST_TRADE_NOT )
{
	UidType				m_iSenderUID;
	std::wstring		m_wstrSenderNickName;
	UidType				m_iReceiverUID;
};

DECL_PACKET( ERM_REQUEST_TRADE_REPLY_NOT )
{
	UidType				m_iReceiverUID;
	int					m_iOK;

	UidType				m_iMemberUID;
	UidType				m_iMemberGSUID;
};

DECL_PACKET( ERM_OPEN_TRADE_REQ )
{
	UidType				m_iUserUID;
	std::wstring		m_wstrSenderNickName;
	UidType				m_iMemberUID;
	UidType				m_iMemberGSUID;
	//{{ 2009. 4. 28  최육사	최대ED제한
	int					m_iED;
	//}}
};

DECL_PACKET( ERM_OPEN_TRADE_ACK )
{
	int					m_iOK;
	UidType				m_iTradeUID;
};

typedef UidType				KERM_ACCEPT_JOIN_TRADE_NOT;

DECL_PACKET( ERM_JOIN_TRADE_REQ )
{
	KTradeUserInfo		m_kMemberInfo;
};

DECL_PACKET( ERM_JOIN_TRADE_ACK )
{
	int									m_iOK;
	UidType								m_iTradeUID;
	std::vector< KTradeUserInfo >		m_vecTradeUserInfo;
};

DECL_PACKET( ERM_UPDATE_TRADE_ITEM_REQ )
{
	KInventoryItemInfo					m_kInventoryItemInfo;
	//{{ 2009. 1. 7  최육사		거래템 10개 확장
	bool								m_bDelete;
	//}}

	KERM_UPDATE_TRADE_ITEM_REQ()
	{
		m_bDelete = false;
	}
};

typedef int		KERM_UPDATE_TRADE_ED_REQ;

DECL_PACKET( ERM_CONFIRM_TRADE_REQ )
{
	std::vector< KInventoryItemInfo	>	m_vecItemIn;
	std::vector< KInventoryItemInfo	>	m_vecItemOut;
	int									m_iEDIn;
	int									m_iEDOut;

	KERM_CONFIRM_TRADE_REQ()
	{
		m_iEDIn = 0;
		m_iEDOut = 0;
	}
};

DECL_PACKET( ERM_CONFIRM_TRADE_ACK )
{
	int									m_iOK;

	// GSUser객체 소멸시 직접 DB업데이트용 정보
	KItemQuantityUpdate                 m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >            m_vecItemInfoTemp;
	int									m_iEDTemp;
};

DECL_PACKET( ERM_CONFIRM_TRADE_NOT )
{
	UidType								m_iTradeUID;
	std::vector< KInventoryItemInfo	>	m_vecItemIn;
	std::vector< KInventoryItemInfo	>	m_vecItemOut;
	int									m_iEDIn;
	int									m_iEDOut;

	// GSUser객체 소멸시 직접 DB업데이트용 정보
	KItemQuantityUpdate                 m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >            m_vecItemInfoTemp;
	int									m_iEDTemp;
	//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
	UidType								m_iOtherCID;		// 거래 상대
#endif SERV_MONITORING_LETTER_AND_TRADE
	//}}
};

DECL_DATA( KSuccessTradeData )
{
	bool								m_bIsSuccess;

	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	KERM_CONFIRM_TRADE_NOT				m_kConfirmTradeNot;
};

typedef KSuccessTradeData	KERM_SUCCESS_TRADE_REQ;
typedef	KSuccessTradeData	KERM_SUCCESS_TRADE_ACK;

DECL_PACKET( DBE_INSERT_TRADE_ITEM_REQ )
{
	UidType                             m_iUnitUID;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
};

DECL_PACKET( DBE_INSERT_TRADE_ITEM_ACK )
{
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
};

//{{ 2009. 2. 10  최육사	개인거래 버그 예방
DECL_PACKET( DBE_INSERT_TRADE_ITEM_BY_SERVER_NOT )
{
	UidType                             m_iUnitUID;
	int									m_iED;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KItemInfo >            m_vecItemInfo;
};
//}}

DECL_PACKET( ELOG_INSERT_TRADE_LOG_NOT )
{
	UidType								m_iFromUserUID;
	UidType								m_iFromUnitUID;
	UidType								m_iToUserUID;
	UidType								m_iToUnitUID;
	UidType								m_iItemUID;
	int									m_iItemID;	
	int									m_iQuantity;
	std::wstring						m_wstrRegDate;
};

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
DECL_PACKET( ERM_DB_UPDATE_TRADE_RESULT_NOT )
{
	UidType                             m_iUnitUID;
	int									m_iChangedED;

	KItemQuantityUpdate                 m_kItemQuantityUpdate_OUT;

	std::vector< KInventoryItemInfo >			m_vecUpdated;				// 삭제 혹은 수량 변경 해야 하는 아이템(거래로 나가는 아이템)
	std::vector< KInventoryItemInfo >			m_vecUpdatedInventorySlot;  // 클라이언트 갱신용
	std::vector< KInventoryItemInfo >			m_vecInsertDBItem; // insert 해야 하는 아이템(거래로 들어오는 아이템을 모두 insert 해야한다.)
	
	KERM_DB_UPDATE_TRADE_RESULT_NOT()
	{
		m_iUnitUID = 0;
		m_iChangedED = 0;
	}
};

DECL_PACKET( DBE_DB_UPDATE_TRADE_RESULT_REQ )
{
	std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >		m_vecDBUpdate;
};

DECL_PACKET( DBE_DB_UPDATE_TRADE_RESULT_ACK )
{
	int													m_iOK;
	std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >		m_vecDBUpdate;

	KDBE_DB_UPDATE_TRADE_RESULT_ACK()
	{
		m_iOK = 0;
	}
};
#else

//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
DECL_PACKET( ERM_DB_UPDATE_TRADE_RESULT_NOT )
{
	UidType                             m_iUnitUID;
	int									m_iChangedED;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;

	KERM_DB_UPDATE_TRADE_RESULT_NOT()
	{
		m_iUnitUID = 0;
		m_iChangedED = 0;
	}
};

DECL_PACKET( ERM_DB_UPDATE_TRADE_COMPLETE_NOT )
{
	UidType								m_iUnitUID;
	int									m_iChangedED;
	int									m_iChangedEDFailed;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;

	KERM_DB_UPDATE_TRADE_COMPLETE_NOT()
	{
		m_iUnitUID = 0;
		m_iChangedED = 0;
		m_iChangedEDFailed = 0;
	}
};

DECL_PACKET( DBE_DB_UPDATE_TRADE_RESULT_REQ )
{
	std::vector< KERM_DB_UPDATE_TRADE_RESULT_NOT >		m_vecDBUpdate;
};

DECL_PACKET( DBE_DB_UPDATE_TRADE_RESULT_ACK )
{
	int													m_iOK;
	std::vector< KERM_DB_UPDATE_TRADE_COMPLETE_NOT >	m_vecDBUpdate;

	KDBE_DB_UPDATE_TRADE_RESULT_ACK()
	{
		m_iOK = 0;
	}
};
#endif SERV_TRADE_DDOS_DEFENCE
//}}

#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2010. 06. 04  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
//////////////////////////////////////////////////////////////////////////

DECL_PACKET( ENX_AUTH_INITIALIZE_REQ )
{
	BYTE								m_bytePacketType;
	BYTE								m_byteInitializeType;
	BYTE								m_byteGameSN;
	BYTE								m_byteDomainSN;
	std::wstring						m_wstrDomainName;
	BYTE								m_byteSynchronizeType;
	unsigned long						m_ulSynchronizeCount;
};

DECL_PACKET( ENX_AUTH_INITIALIZE_ACK )
{
	BYTE								m_bytePacketType;
	BYTE								m_byteInitializeType;
	BYTE								m_byteResult;
	BYTE								m_byteDomainSN;
	std::wstring						m_wstrMessage;
};

DECL_PACKET( ENX_AUTH_LOGIN_REQ )
{
	BYTE										m_bytePacketType;
	BYTE										m_byteLoginType;
	std::wstring								m_wstrUserID;
	unsigned int								m_uiIP;
	std::map< unsigned short, KPropertyValue >	m_mapProperty;
};

DECL_PACKET( ENX_AUTH_LOGIN_ACK )
{
	BYTE										m_bytePacketType;
	BYTE										m_byteLoginType;
	__int64										m_iSessionNo;
	std::wstring								m_wstrUserID;
	BYTE										m_byteAuthorizeResult;
	BYTE										m_byteAuthorizeType; // 구버전의 addressDesc와 동일
	BYTE										m_byteChargeType; // 구버전의 accountDesc와 동일
	BYTE										m_byteOption;
	unsigned long								m_ulArgument;
	BYTE										m_byteIsCharged; // 구버전의 IsGameBangBillType과 동일
	std::map< unsigned short, KPropertyValue >	m_mapProperty;

	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
	enum NX_PROPERTY_NO
	{
		NPN_OUT_USER_RETAINING = 4,       // 이탈방지		
		NPN_SHUTDOWN = 5,				  // 셧다운

		//{{ 2012. 02. 14    김민성   넥슨 안심 로그인 - 목표 위치기반 로그인
#ifdef SERV_NEXON_PEACE_OF_MIND_LOGIN
		NPN_PEACE_OF_MIND_LOGIN = 7,		  // 안심 로그인
#endif SERV_NEXON_PEACE_OF_MIND_LOGIN
		//}}
		//{{ 2012. 06. 13	김민성       선택적 셧다운제
#ifdef SERV_SELECTIVE_SHUTDOWN_SYSTEM
		NPN_SELECTIVE_SHUTDOWN = 10,		  // 게임 이용 차단 정책
#endif SERV_SELECTIVE_SHUTDOWN_SYSTEM
		//}}
	};
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
};

DECL_PACKET( ENX_AUTH_LOGOUT_NOT )
{
	BYTE										m_bytePacketType;
	BYTE										m_byteLogoutType;
	std::wstring								m_wstrUserID;
	__int64										m_iSessionNo;
};

DECL_PACKET( ENX_AUTH_TERMINATE_NOT )
{
	BYTE										m_bytePacketType;
	BYTE										m_byteTerminateType;
	__int64										m_iSessionNo;
	std::wstring								m_wstrUserID;
	std::wstring								m_wstrCharacterName;
	BYTE										m_byteOption;
	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
	BYTE										m_bytePropertyCount;
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
	std::map< unsigned short, KPropertyValue >	m_mapProperty;
#endif SERV_SHUTDOWN_SYSTEM
	//}}
};

DECL_PACKET( ENX_AUTH_MESSAGE_NOT )
{
	BYTE										m_bytePacketType;
	BYTE										m_byteMessageType;
	__int64										m_iSessionNo;
	std::wstring								m_wstrUserID;
	std::wstring								m_wstrCharacterName;
	BYTE										m_byteOption;
	unsigned long								m_ulArgument;
	unsigned long								m_ulSessionCount;
};

DECL_PACKET( ENX_AUTH_SYNCHRONIZE_REQ )
{
	BYTE										m_bytePacketType;
	BYTE										m_byteIsMonitoring;
	std::map< __int64, std::wstring >			m_mapSynchronizeReq;
};

DECL_PACKET( ENX_AUTH_SYNCHRONIZE_ACK )
{
	BYTE										m_bytePacketType;
	BYTE										m_byteIsMonitoring;
	std::map< __int64, BYTE >					m_mapSynchronizeAck;
};

DECL_PACKET( ENX_AUTH_ALIVE_NOT )
{
	BYTE										m_bytePacketType;
};


//////////////////////////////////////////////////////////////////////////
#endif SERV_PCBANG_AUTH_NEW
//}}

DECL_PACKET( ENX_INITIALIZE_BILLING_AUTH_NOT )
{
	BYTE								m_bytePacketType;
	BYTE								m_byteGameSN;
	BYTE								m_byteDomainSN;
	std::wstring						m_wstrDomainName;
};

DECL_DATA( KENX_COMMON_REQ )
{
	BYTE								m_bytePacketType;
	BYTE								m_byteLogin;
	std::wstring						m_wstrUserID;
	std::wstring						m_wstrUnitNickName;
	unsigned int						m_uiIP;
	BYTE								m_byteCanTry;
	BYTE								m_byteGuestUser;
};

DECL_DATA( KENX_COMMON_ACK_NOT )
{
	BYTE								m_bytePacketType;
	std::wstring						m_wstrUserID;
	std::wstring						m_wstrUnitNickName;
	BYTE								m_byteResult;
	BYTE								m_byteAddressDesc;
	BYTE								m_byteAccountDesc;
	BYTE								m_byteOption;
	unsigned int						m_uiArgument;
	BYTE								m_byteIsGameBangBillType;
};

DECL_PACKET( ENX_ALIVE_NOT )
{
	BYTE								m_bytePacketType;
};

typedef KENX_COMMON_REQ					KENX_USER_LOGIN_REQ;

typedef KENX_COMMON_ACK_NOT             KENX_USER_LOGIN_ACK;

typedef KENX_COMMON_REQ					KENX_USER_LOGOUT_REQ;

DECL_PACKET( ENX_ADM_SEARCH_REQ )
{
    DWORD                               m_dwTransactionID;
    std::wstring                        m_wstrUserID;
};

DECL_PACKET( ENX_ADM_SEARCH_ACK )
{
    DWORD                               m_dwTransactionID;
    BYTE                                m_byteResult;
};

DECL_PACKET( ENX_CHAR_SEARCH_REQ )
{
    DWORD                               m_dwTransactionID;
    std::wstring                        m_wstrUnitNickName;
};

DECL_PACKET( ENX_CHAR_SEARCH_ACK )
{
    DWORD                               m_dwTransactionID;
    BYTE                                m_byteResult;
};

DECL_PACKET( ENX_NEW_SHOP_ITEM_NOT )
{
    DWORD                               m_dwTransactionID;
    std::wstring                        m_wstrUnitNickName;
};

DECL_PACKET( ENX_NEW_SHOP_GIFT_NOT )
{
    DWORD                               m_dwTransactionID;
    std::wstring                        m_wstrUnitNickName;
};

DECL_PACKET( ERM_END_TC_GAME_ACK )
{
	int			m_iOK;

	int			m_iTCID;
	bool		m_bIsSuccess;
};

DECL_PACKET( ELG_REGISTER_USER_REQ )
{
    std::wstring                        m_wstrUserID;
	//{{ 2009. 4. 17  최육사	채널id
	int									m_iChannelID;
	//}}
    unsigned int						m_uiIP;
    BYTE								m_byteCanTry;
	BYTE								m_byteGuestUser;	
	char								m_cAuthLevel;
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
	unsigned int						m_uiPublisherUID;
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	unsigned char						m_ucChannelCode;
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
	//}}
	//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	std::string							m_strMachineID;
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
	//}}
#ifdef SERV_COUNTRY_TH
	std::wstring						m_wstrSocketID;
#endif // SERV_COUNTRY_TH
};

DECL_PACKET( ELG_REGISTER_USER_ACK )
{
	int			m_iOK;
};

//{{ 2009. 5. 28  최육사	채널이동
DECL_PACKET( ELG_CHANNEL_CHANGE_REGISTER_USER_REQ )
{
	KEGS_CONNECT_ACK					m_kConnectAck;
	KEGS_VERIFY_ACCOUNT_ACK				m_kVerifyAccountAck;
	KEGS_SELECT_UNIT_REQ				m_kSelectUnitReq;
	KChannelChangeInfo					m_kChangeUserInfo;

	// Reg Global Server
	KELG_REGISTER_USER_REQ				m_kRegUserReq;
};

DECL_PACKET( ELG_CHANNEL_CHANGE_REGISTER_USER_ACK )
{
	int									m_iOK;
	KEGS_CONNECT_ACK					m_kConnectAck;
	KEGS_VERIFY_ACCOUNT_ACK				m_kVerifyAccountAck;
	KEGS_SELECT_UNIT_REQ				m_kSelectUnitReq;
	KChannelChangeInfo					m_kChangeUserInfo;
};
//}}

//{{ 2009. 3. 17  최육사	유저 접속종료 알림
DECL_PACKET( ELG_USER_DISCONNECT_REQ )
{
	u_short								m_usEventID;
	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	std::map< int, int >				m_mapAccCountInfo;
#endif SERV_ACCOUNT_COUNT
	//}}
};
//}}

//{{ 2008. 5. 27  최육사  랜덤맵
DECL_PACKET( ERM_GAME_START_REQ )
{
	short		m_sWorldID;	
};
//}}
typedef KPacketOK						KERM_GAME_START_ACK;

//{{ 2008. 10. 29  최육사	아이템 검사
DECL_PACKET( ERM_CHANGE_READY_ACK )
{
	int			m_iOK;
	int			m_iDungeonID;
	char		m_cDungeonMode;
};
//}}

DECL_PACKET( DBE_OPEN_RANDOM_ITEM_REQ )
{
	UidType                             m_iUnitUID;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
	//{{ 2010. 7. 26  최육사	밀봉 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
	std::set< int >						m_setSealRandomItem;
#endif SERV_SEALED_RANDOM_ITEM
	//}}
	//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	int									m_iNumResurrectionStone;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	//}}
	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	int									m_iSpendED;
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}
};

DECL_PACKET( DBE_OPEN_RANDOM_ITEM_ACK )
{
    int                                 m_iOK;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	int									m_iNumResurrectionStone;
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
	//}}
	//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
	int									m_iSpendED;
#endif SERV_CUBE_OPEN_ED_CONDITION
	//}
};

//{{ 2007. 10. 10  최육사  제조
DECL_PACKET( DBE_ITEM_MANUFACTURE_REQ )
{
	UidType                             m_iUnitUID;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
};

DECL_PACKET( DBE_ITEM_MANUFACTURE_ACK )
{
	int                                 m_iOK;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
};
//}}

typedef KPacketOK			KERM_STATE_CHANGE_GAME_INTRUDE_ACK;

//{{ 2007. 10. 16  최육사  유저 통계
DECL_PACKET( ELOG_USER_STATISTICS )
{
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	std::map< std::pair< int, int >, std::vector< int > > m_mapPvpStatistics;	
	std::map< std::pair< int, int >, std::vector< int > > m_mapDungeonStatistics;
	std::map< std::pair< int, int >, std::vector< int > > m_mapTCenterStatistics;
	std::map< std::pair< int, int >, std::vector< int > > m_mapEtcStatistics;
#ifdef SERV_USER_STATISTICS_RENEWAL
	std::map< std::pair< int, int >, std::vector< int > > m_mapEDStatistics;
	int									m_iAuthLevel;
	char								m_cUnitClass;
	u_char								m_ucLevel;
	int									m_iED;
	int									m_iVSPoint;
	int									m_iVSPointMax;
	std::wstring                        m_wstrLoginTime;
	std::wstring                        m_wstrLogoutTime;
	std::wstring						m_wstrNickName;
	UINT32								m_uiGiantUID;
	std::wstring						m_wstrAccountCreateDate;
	int									m_iEXP;
	std::wstring						m_wstrUnitCreateDate;
	int									m_iiDurationTime;
#endif //SERV_USER_STATISTICS_RENEWAL
};
//}}

DECL_DATA( KPurchaseCoupon )
{
    UINT32                  m_uiNexonSN;
    std::wstring            m_wstrUserID;
    std::wstring            m_wstrUserName;
    std::wstring            m_wstrCouponSerial;
    int                     m_iGameType;
    int                     m_iServerDomain;
    std::wstring            m_wstrUnitNickName;
    int                     m_iReason;
    std::wstring            m_wstrCafeNo;
};

DECL_PACKET( ELG_KNM_INVITE_REQ )
{
	std::wstring		m_wstrInviteUnitNickName;

	std::wstring		m_wstrMyNickName;
	KNMUnitInfo			m_kMyUnitInfo;
};

DECL_PACKET( ERM_NPC_UNIT_DIE_NOT )
{
	int								m_iDungeonID;
	char							m_cDifficulty;

	int								m_nDieNPCUID;
	int								m_iNPCID;
	char							m_cAttUnitType;
	UidType							m_uiAttUnit;
	std::map< UidType, KEXPData >	m_EXPList;

	short							m_sSubStageExpNpcNum;
	char							m_cUserCount;
	bool							m_bNoDrop;
	bool							m_bDecreaseEndurance;

	//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	bool							m_bIsPvpNpc;
	int								m_iGameType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	//{{ 2011. 08. 31  김민성	비밀던전 헬모드
#ifdef SERV_RANDOM_DAY_QUEST
	char							m_cDungeonMode;
#endif SERV_RANDOM_DAY_QUEST
	//}}
	bool							m_bQuestComplete;
#ifdef PVP_QUEST_HERO_KILL_COUNT
	bool							m_bHeroNPC;
#endif //PVP_QUEST_HERO_KILL_COUNT

	KERM_NPC_UNIT_DIE_NOT()
	{
		m_iDungeonID		 = 0;
		m_cDifficulty		 = 0;
		m_nDieNPCUID		 = 0;
		m_iNPCID			 = 0;
		m_cAttUnitType		 = 0;
		m_uiAttUnit			 = 0;
		m_sSubStageExpNpcNum = 0;
		m_cUserCount		 = 0;
		m_bNoDrop			 = true;
		m_bDecreaseEndurance = false;
		//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		m_bIsPvpNpc			 = false;
		m_iGameType			 = -1;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 08. 31  김민성	비밀던전 헬모드
#ifdef SERV_RANDOM_DAY_QUEST
		m_cDungeonMode		= 0;
#endif SERV_RANDOM_DAY_QUEST
		//}}
		m_bQuestComplete	= false;
#ifdef PVP_QUEST_HERO_KILL_COUNT
		m_bHeroNPC			= false;
#endif //PVP_QUEST_HERO_KILL_COUNT
	}
};

DECL_DATA( KDBE_PURCHASED_CASH_ITEM_LIST_REQ )
{
    int                     m_nItemPerPage;
    int                     m_iCurrentPage;
    std::wstring            m_wstrUnitNickName;
};

//{{ 2007. 11. 12  최육사  치트 로그
DECL_PACKET( ELOG_CHEAT_LOG_NOT )
{
	UidType                 m_iUserUID;
	UidType                 m_iUnitUID;
	char					m_cAuthLevel;
	std::wstring			m_wsCheatCode;
	std::wstring			m_wsCheatContents;
};
//}}

DECL_PACKET( ERM_GAME_START_NOT )
{
	char                            m_cRoomState;
	int								m_iGameType;		
	std::vector< KRoomSlotInfo >    m_vecSlot;

	//튜토리얼 & 훈련소에서는 근성도를 깍지 않기 때문에 확인용으로 들어가며
	//dungeoID + difficulty 로 전해준다.
	int								m_iDungeonID;
	//{{ 2008. 5. 27  최육사  랜덤맵
	short							m_sWorldID;
	//}}
};

DECL_PACKET( ERM_USER_UNIT_DIE_NOT )
{
	int			m_iGameType;	
	int			m_KillerNPCUID;
	UidType		m_KillerUserUnitUID;			//죽인 Unit
	UidType		m_MaxDamageKillerUserUnitUID;	//죽인 Unit
	UidType		m_KilledUserUnitUID;			//죽은 Unit
	//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
	bool		m_bOfficialMatch;				// 대전 매칭을 이용한 것인가?
#endif SERV_NEW_PVP_QUEST
	//}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT
	bool		m_bHeroNPC;						// 대전 에픽 NPC에게 죽은것인가
#endif //PVP_QUEST_HERO_KILL_COUNT

#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
	char		m_killedUserUnitType;
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL

#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
	char       m_killedUserUnitClass;
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL

	KERM_USER_UNIT_DIE_NOT()
	{
		m_iGameType						= -1;		
		m_KillerNPCUID					= -1;
		m_KillerUserUnitUID				= -1;
		m_MaxDamageKillerUserUnitUID	= -1;
		m_KilledUserUnitUID				= -1;
		//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		m_bOfficialMatch				= false;
#endif SERV_NEW_PVP_QUEST
		//}}
#ifdef PVP_QUEST_HERO_KILL_COUNT
		m_bHeroNPC						= false;
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		m_killedUserUnitType			= 0;
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		m_killedUserUnitClass			= 0;
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
	}
};

DECL_PACKET( DBE_GET_PURCHASED_CASH_ITEM_REQ )
{
    int                                 m_iOrderNo;
    int                                 m_iProductNo;
    int                                 m_iPackageProductNo;
    int									m_iQuantity;	
    int                                 m_iItemID;
};

DECL_PACKET( DBE_GET_PURCHASED_CASH_ITEM_ACK )
{
    int                                 m_iOK;
    int                                 m_iItemID;
	int									m_iQuantity;
};

DECL_PACKET( DBE_ROLLBACK_PURCHASED_CASH_ITEM_NOT )
{
	int                                 m_iOrderNo;
	int                                 m_iProductNo;
	std::wstring                        m_wstrMsg;
};

//{{ 2007. 12. 17  최육사  센터 서버 유저 통계
DECL_PACKET( ERM_PVP_USER_STATISTICS_NOT )
{
	enum US_GAME_RESULT
	{
		UGR_WIN = 0,
		UGR_LOSE,
		UGR_DRAW,
		UGR_DROP,		
	};
	
	char		m_cGameType;
	char		m_cGameResult;
	short		m_sWorldID;
	bool		m_bIntrude;	
	int			m_iGamePlayTime;	
	int			m_iKillNum;

	KERM_PVP_USER_STATISTICS_NOT()
	{
		m_cGameType		= 0;
		m_cGameResult	= 0;
		m_sWorldID		= 0;
		m_bIntrude		= false;
		m_iGamePlayTime = 0;
		m_iKillNum		= 0;
	}
};

DECL_PACKET( ERM_DUNGEON_USER_STATISTICS_NOT )
{
	enum US_GAME_RESULT
	{
		UGR_WIN = 0,
		UGR_LOSE,
		UGR_DROP,
	};

	int			m_iDungeonID;
	char		m_cGameResult;
	bool		m_bIsPartyPlay;
	int			m_iGamePlayTime;
	int			m_iUsedResurrectionStone;

	KERM_DUNGEON_USER_STATISTICS_NOT()
	{
		m_iDungeonID			 = 0;
		m_cGameResult			 = 0;
		m_bIsPartyPlay			 = false;
		m_iGamePlayTime 		 = 0;
		m_iUsedResurrectionStone = 0;
	}
};

DECL_PACKET( ERM_TC_USER_STATISTICS_NOT )
{
	enum US_GAME_RESULT
	{
		UGR_SUCCESS = 0,
		UGR_FAIL,		
		UGR_DROP,		
	};

	int			m_iTCID;
	char		m_cGameResult;
	int			m_iGamePlayTime;

	KERM_TC_USER_STATISTICS_NOT()
	{
		m_iTCID			= 0;
		m_cGameResult	= 0;		
		m_iGamePlayTime = 0;
	}
};

//{{ 2009. 3. 31  최육사	TR 사용
DECL_PACKET( ETR_CHAT_NOT )
{
	std::vector< UidType >	m_vecUID;
	char					m_cChatPacketType;
	UidType					m_SenderUnitUID;
	std::wstring			m_wstrSenderUnitNickName;
	std::wstring			m_wstrRecieverUnitNickName;
	UidType					m_RecieverUnitUID;
	char					m_cRoomChatType;
	std::wstring			m_wstrMsg;
};
//}}

DECL_PACKET( ELG_CHAT_REQ )
{
	char			m_cChatPacketType;
	//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
	int				m_iChatData;
#endif GUILD_TEST
	//}}
	//{{ 2008. 2. 1  최육사  
	UidType			m_iSenderUnitUID;
	//}}
	std::wstring	m_wstrSenderUnitNickName;
	std::wstring	m_wstrRecieverUnitNickName;
	std::wstring	m_wstrMsg;
};

typedef KPacketOK	KELG_CHAT_ACK;

DECL_PACKET( ELG_CHAT_NOT )
{
	char			m_cChatPacketType;
	//{{ 2008. 2. 1  최육사  
	UidType			m_iSenderUnitUID;
	//}}
	std::wstring	m_wstrSenderUnitNickName;
    UidType         m_iReceiverUnitUID;
	std::wstring	m_wstrRecieverUnitNickName;
	std::wstring	m_wstrMsg;
};

DECL_PACKET( ERM_CHECK_INVALID_ZOMBIE_USER_NOT )
{
	UidType		m_iRoomUID;
};

DECL_PACKET( ENX_BT_INITIALIZE_REQ )
{
    unsigned long   m_ulPacketNo;
    BYTE            m_bytePacketType;
    std::wstring    m_wstrServiceCode;
    BYTE            m_byteDomain;
};

DECL_PACKET( ENX_BT_INITIALIZE_ACK )
{
    unsigned long   m_ulPacketNo;
    BYTE            m_bytePacketType;
    std::wstring    m_wstrServiceCode;
    BYTE            m_byteDomain;
    unsigned long   m_ulResult;
};

DECL_PACKET( ENX_BT_HEART_BEAT_REQ )
{
    unsigned long   m_ulPacketNo;
    BYTE            m_bytePacketType;
    __int64         m_iReleaseTick;
};

DECL_PACKET( ENX_BT_HEART_BEAT_ACK )
{
    unsigned long   m_ulPacketNo;
    BYTE            m_bytePacketType;
    unsigned long   m_ulResult;
};

DECL_PACKET( ENX_BT_CATEGORY_INQUIRY_REQ )
{
    unsigned long   m_ulPacketNo;
    BYTE            m_bytePacketType;
};

DECL_PACKET( ENX_BT_CATEGORY_INQUIRY_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    std::vector< KNXBTCategoryInfo >    m_vecNXBTCategoryInfo;
};

DECL_PACKET( ENX_BT_PRODUCT_INQUIRY_XML_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulPageIndex;
    unsigned long                       m_ulRowPerPage;
};

DECL_PACKET( ENX_BT_PRODUCT_INQUIRY_XML_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    __int64                             m_iReleaseTick;
    unsigned long                       m_ulTotalProductCount;
    std::wstring                        m_wstrResultXML;
};

DECL_PACKET( ENX_BT_PRODUCT_INQUIRY_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulPageIndex;
    unsigned long                       m_ulRowPerPage;
};

DECL_PACKET( ENX_BT_PRODUCT_INQUIRY_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    __int64                             m_iReleaseTick;
    unsigned long                       m_ulTotalProductCount;
    std::vector< KNXBTProductInfo >     m_vecNXBTProductInfo;
};

DECL_PACKET( ENX_BT_CHECK_BALANCE_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
	//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
	unsigned long						m_ulPaymentType;
#endif SERV_TOONILAND_CHANNELING
	//}}
    std::wstring                        m_wstrUserID;
};

DECL_PACKET( ENX_BT_CHECK_BALANCE_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulBalance;
};

DECL_PACKET( ENX_BT_PURCHASE_ITEM_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulRemoteIP;
    BYTE                                m_byteReason;
    std::wstring                        m_wstrGameID;
    std::wstring                        m_wstrUserID;
    unsigned long                       m_ulUserOID;
    std::wstring                        m_wstrUserName;
    BYTE                                m_byteUserAge;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
	byte								m_byteIsCafe;
#endif // SERV_NEXON_COUPON_SYSTEM
    std::wstring                        m_wstrOrderID;
	unsigned long						m_ulPaymentType;
    unsigned long                       m_ulTotalAmount;
    std::vector< KNXBTPurchaseReqInfo > m_vecNXBTPurchaseInfo;

	KENX_BT_PURCHASE_ITEM_REQ()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulRemoteIP( 0 )
		, m_byteReason( 0 )
		, m_ulUserOID( 0 )
		, m_byteUserAge( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
		, m_byteIsCafe( 0 )
#endif // SERV_NEXON_COUPON_SYSTEM
		, m_ulPaymentType( 0 )
		, m_ulTotalAmount( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_PURCHASE_ITEM_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    std::wstring                        m_wstrOrderID;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulOrderNo;
    std::vector< KNXBTPurchaseAckInfo > m_vecNXBTPurchaseInfo;
};

DECL_PACKET( ENX_BT_PURCHASE_GIFT_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulRemoteIP;
    BYTE                                m_byteReason;
    std::wstring                        m_wstrSenderGameID;
    std::wstring                        m_wstrSenderUserID;
    unsigned long                       m_ulSenderUserOID;
    std::wstring                        m_wstrSenderUserName;
    BYTE                                m_byteSenderUserAge;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	byte								m_byteIsCafe;
#endif // SERV_NEXON_COUPON_SYSTEM
    BYTE                                m_byteReceiverServerNo;
    std::wstring                        m_wstrReceiverGameID;
    std::wstring                        m_wstrMessage;
    std::wstring                        m_wstrOrderID;
	unsigned long						m_ulPaymentType;
    unsigned long                       m_ulTotalAmount;
    std::vector< KNXBTPurchaseReqInfo > m_vecNXBTPurchaseInfo;

	KENX_BT_PURCHASE_GIFT_REQ()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulRemoteIP( 0 )
		, m_byteReason( 0 )
		, m_ulSenderUserOID( 0 )
		, m_byteSenderUserAge( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		, m_byteIsCafe( 0 )
#endif // SERV_NEXON_COUPON_SYSTEM
		, m_byteReceiverServerNo( 0 )
		, m_ulPaymentType( 0 )
		, m_ulTotalAmount( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_PURCHASE_GIFT_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    std::wstring                        m_wstrOrderID;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulOrderNo;
    std::vector< KNXBTPurchaseAckInfo > m_vecNXBTPurchaseInfo;
};

//{{ 2009. 10. 14  최육사	자동결제
DECL_PACKET( ENX_BT_CHECK_AUTO_PAYMENT_USER_REQ )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	std::wstring                        m_wstrGameID;
	std::wstring                        m_wstrUserID;
	unsigned long						m_ulProductNo;
};

DECL_PACKET( ENX_BT_CHECK_AUTO_PAYMENT_USER_ACK )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
};
//}}

DECL_PACKET( ENX_BT_COUPON_INQUIRY_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    BYTE                                m_byteIsCafe;
    std::wstring                        m_wstrCouponString;
};

DECL_PACKET( ENX_BT_COUPON_INQUIRY_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
	std::vector< KNXBTCouponProductInfo > m_vecProductInfo;
};

//{{ 2009. 3. 24  최육사	쿠폰사용여부 조회
DECL_PACKET( ENX_BT_COUPON_INQUIRY_NEW_REQ )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	unsigned long						m_ulUserIP;
	std::wstring						m_wstrGameID;
	BYTE                                m_byteIsCafe;
	std::wstring						m_wstrUserID;
	unsigned long						m_ulUserOID;
	std::wstring                        m_wstrCouponString;
};

DECL_PACKET( ENX_BT_COUPON_INQUIRY_NEW_ACK )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	unsigned long                       m_ulResult;
	std::vector< KNXBTCouponProductInfo > m_vecProductInfo;
};
//}}

DECL_PACKET( ENX_BT_COUPON_USING_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulRemoteIP;
    BYTE                                m_byteReason;
    std::wstring                        m_wstrGameID;
    BYTE                                m_byteIsCafe;
    std::wstring                        m_wstrUserID;
    unsigned long                       m_ulUserOID;
    std::wstring                        m_wstrUserName;
    std::wstring                        m_wstrCouponString;
};

DECL_PACKET( ENX_BT_COUPON_USING_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    std::wstring                        m_wstrCouponTXID;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulOrderNo;
    std::vector< KNXBTCouponProductInfo > m_vecCouponProductInfo;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_INQUIRY_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    std::wstring                        m_wstrGameID;
    BYTE                                m_byteShowInventory;
    unsigned long                       m_ulPageIndex;
    unsigned long                       m_ulRowPerPage;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_INQUIRY_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulTotalCount;
    std::vector< KNXBTOrderInfo >       m_vecNXBTOrderInfo;
    std::vector< KNXBTPackageInfo >     m_vecNXBTPackageInfo;
    std::vector< KNXBTLotteryInfo >     m_vecNXBTLotteryInfo;
};

//{{ 2008. 6. 20  최육사  선물하기
DECL_PACKET( ENX_BT_NISMS_INVENTORY_CHECK_REQ )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	std::wstring                        m_wstrGameID;
	BYTE                                m_byteShowInventory;
	BYTE								m_byteIsPresent;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_CHECK_ACK )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	unsigned long                       m_ulResult;
	BYTE								m_byteOrderType;
	unsigned long                       m_ulTotalCount;
};
//}}

DECL_PACKET( ENX_BT_NISMS_INVENTORY_READ_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_READ_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
};

//{{ 2008. 6. 26  최육사  선물하기
DECL_PACKET( ENX_BT_NISMS_INVENTORY_READABLE_REQ )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	std::wstring                        m_wstrGameID;
	BYTE                                m_byteShowInventory;
	BYTE								m_byteIsPresent;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_READABLE_ACK )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	unsigned long                       m_ulResult;
	BYTE								m_byteOrderType;
	unsigned long                       m_ulTotalCount;
};
//}}

DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
	unsigned short						m_usOrderQuantity;
	std::wstring						m_wstrExtendValue;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
	unsigned short						m_usOrderQuantity;
	std::wstring						m_wstrExtendValue;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
	std::wstring						m_wstrExtendValue;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
	std::wstring						m_wstrExtendValue;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ )
{
    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
    unsigned short                      m_usOrderQuantity;
    std::wstring                        m_wstrExtendValue;
};

DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK )
{
	// 해외에서는 EBILL_PICK_UP_ACK 이거 사용

    unsigned long                       m_ulPacketNo;
    BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
    unsigned long                       m_ulOrderNo;
    unsigned long                       m_ulProductNo;
    BYTE                                m_byteProductKind;
    std::wstring                        m_wstrProductName;
    std::wstring                        m_wstrProductID;
    unsigned short                      m_usProductExpire;
    unsigned short                      m_usProductPieces;
    unsigned short                      m_usOrderQuantity;
    std::wstring                        m_wstrProductAttribute0;
    std::wstring                        m_wstrProductAttribute1;
    std::wstring                        m_wstrProductAttribute2;
    std::wstring                        m_wstrProductAttribute3;
    std::wstring                        m_wstrProductAttribute4;
    std::wstring                        m_wstrExtendValue;
    std::vector< KNXBTSubProductInfo >  m_vecNXBTSubProductInfo;
	//{{ 2008. 7. 10  최육사  추가 변수
	std::vector< KNXBTBonusProductInfoPickUp > m_vecKNXBTBonusProductInfo;
	//}}
};

//{{ 2008. 6. 16  최육사  넥슨 빌링 패키지 픽업
DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	unsigned long                       m_ulOrderNo;
	unsigned long				        m_ulPackageProductNo;
	unsigned long                       m_ulProductNo;
};

// SERV_GLOBAL_BILLING 해외팀도 사용함
DECL_PACKET( ENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
	unsigned long                       m_ulResult;
	unsigned long                       m_ulOrderNo;
	unsigned long				        m_ulPackageProductNo;
	unsigned long                       m_ulProductNo;
#ifdef SERV_GLOBAL_CASH_PACKAGE
	unsigned long                       m_ulSubProductNo;
#endif //SERV_GLOBAL_CASH_PACKAGE
};
//}}

//{{ 2010. 11. 17	최육사	넥슨 빌링 청약 철회
#ifdef SERV_NX_BILLING_REFUND

DECL_PACKET( ENX_BT_REFUND_REQ )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
    unsigned long                       m_ulRemoteIP;
	std::wstring                        m_wstrRequestID;
	std::wstring                        m_wstrGameID;
	unsigned long                       m_ulOrderNo;
	unsigned long                       m_ulProductNo;
	unsigned short                      m_usQuantity;

	KENX_BT_REFUND_REQ()
	{
		m_ulPacketNo = 0;
		m_bytePacketType = 0;
		m_ulRemoteIP = 0;
		m_ulOrderNo = 0;
		m_ulProductNo = 0;
		m_usQuantity = 0;
	}
};

DECL_PACKET( ENX_BT_REFUND_ACK )
{
	unsigned long                       m_ulPacketNo;
	BYTE                                m_bytePacketType;
    unsigned long                       m_ulResult;
};

#endif SERV_NX_BILLING_REFUND
//}}

//{{ 2008. 2. 4  최육사  블랙리스트
DECL_PACKET( ELG_SEARCH_BLACK_LIST_UNIT_REQ )
{
	std::wstring						m_wstrNickName;
};

DECL_PACKET( ELG_SEARCH_BLACK_LIST_UNIT_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
};
//}}

DECL_PACKET( ERM_OPEN_PERSONAL_SHOP_REQ )
{
	UidType				m_iUserUID;
	int					m_iED;
	std::wstring		m_wstrNickName;
	char				m_cPersonalShopType;

	KERM_OPEN_PERSONAL_SHOP_REQ()
	{
		m_iUserUID = 0;
		m_iED = 0;
		m_cPersonalShopType = 0;		
	}
};

DECL_PACKET( ERM_OPEN_PERSONAL_SHOP_ACK )
{
	int					m_iOK;
	UidType				m_iPersonalShopUID;
	char				m_cPersonalShopType;
	//{{ 2011. 04. 12	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	UidType				m_iGSUID;
	UidType				m_iUserUID;
	UidType				m_iUnitUID;
	std::wstring		m_wstrAgencyOpenDate;
#endif SERV_PSHOP_AGENCY
	//}}

	KERM_OPEN_PERSONAL_SHOP_ACK()
	{
		m_iOK = 0;
		m_iPersonalShopUID = 0;
		m_cPersonalShopType = 0;
		//{{ 2011. 04. 12	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		m_iGSUID = 0;
		m_iUserUID = 0;
		m_iUnitUID = 0;
#endif SERV_PSHOP_AGENCY
		//}}
	}
};

typedef KPacketOK KERM_CHECK_OPEN_PERSONAL_SHOP_ACK;

DECL_PACKET( ERM_REG_PERSONAL_SHOP_ITEM_REQ )
{
	std::wstring								m_wstrPersonalShopName;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;

	//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	std::vector< KInventoryItemInfo >			m_vecInventorySlotInfo;
#endif SERV_PSHOP_AGENCY
	//}}
};

DECL_PACKET( ERM_REG_PERSONAL_SHOP_ITEM_ACK )
{
	int											m_iOK;
	//{{ 2011. 04. 14	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	UidType										m_iGSUID;
	UidType										m_iUnitUID;
	std::vector< KInventoryItemInfo >			m_vecInventorySlotInfo;
#endif SERV_PSHOP_AGENCY
	//}}
	int											m_iOpeningED;
	std::wstring								m_wstrPersonalShopName;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	std::vector< KSellPersonalShopItemInfo >	m_vecAddCompleteItemInfo;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
};

DECL_PACKET( ERM_CHECK_JOIN_PERSONAL_SHOP_ACK )
{
	int						m_iOK;
	UidType					m_iPersonalShopUID;
};

DECL_PACKET( ERM_JOIN_PERSONAL_SHOP_REQ )
{
	UidType				m_iUserUID;
	//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
	UidType				m_iUnitUID;
#endif SERV_INTEGRATION
	//}}
	std::wstring		m_wstrNickName;

	bool				m_bIsSquare;
	//{{ 2011. 05. 06	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	char				m_cJoinType;
#endif SERV_PSHOP_AGENCY
	//}}

#ifdef SERV_UPGRADE_TRADE_SYSTEM
  	int					m_iUsedItemID; // 상점 업그레이드 아이템(보증서)
#endif SERV_UPGRADE_TRADE_SYSTEM

	KERM_JOIN_PERSONAL_SHOP_REQ()
	{
		m_iUserUID = 0;
		//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		m_iUnitUID = 0;
#endif SERV_INTEGRATION
		//}}
		m_bIsSquare = false;
		//{{ 2011. 05. 06	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		m_cJoinType = 0;
#endif SERV_PSHOP_AGENCY
		//}}

#ifdef SERV_UPGRADE_TRADE_SYSTEM
		m_iUsedItemID = 0;
#endif SERV_UPGRADE_TRADE_SYSTEM

	}
};

DECL_PACKET( ERM_JOIN_PERSONAL_SHOP_ACK )
{
	int				m_iOK;
	//{{ 2011. 07. 19	최육사	대리상점 구매 수수료 없앤 이벤트
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	bool			m_bIsPShopAgency;
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
	//}}
	UidType			m_iPersonalShopUID;
	std::wstring	m_wstrPersonalShopName;
	std::wstring	m_wstrNickName;
	std::vector<KSellPersonalShopItemInfo> m_vecSellItemInfo;
};

DECL_PACKET( ERM_LEAVE_PERSONAL_SHOP_REQ )
{
	int			m_iReason;
};

DECL_PACKET( ERM_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	UidType						m_iBuyItemUID;
	int							m_iBuyItemID;
	int							m_iBuyQuantity;
	int							m_iBuyUnitNowED;

	// GSUser객체 소멸시 직접 DB업데이트용 정보
	KItemQuantityUpdate			m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >	m_vecItemInfoTemp;
	int							m_iEDTemp;
};

DECL_PACKET( ERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT )
{
	UidType				m_iSellUnitUID;
	int					m_iSellUnitEDIN;
	int					m_iSellCommissionED;
	KInventoryItemInfo	m_kSellIntemInfoOUT;

	UidType				m_iPriceUnitUID;
	int					m_iPriceUnitEDOUT;
	//{{ 2010. 03. 31  최육사	거래게시판
	int					m_iPriceCommissionED;
	//}}
	KInventoryItemInfo	m_kPriceIntemInfoIN;

	//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD
	bool				m_bIsSquare;
#endif DEF_TRADE_BOARD
	//}}	

	// GSUser객체 소멸시 직접 DB업데이트용 정보
	KItemQuantityUpdate                 m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >            m_vecItemInfoTemp;
	int									m_iEDTemp;
};

DECL_PACKET( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ )
{
	KInventoryItemInfo		m_kSellItemInfo;

	int						m_iBuyQuantity;
	UidType					m_iBuyUnitUID;
	int						m_iTotalPrice;
};

DECL_PACKET( ERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK )
{
	int						m_iOK;
	UidType					m_iBuyUnitUID;
	UidType					m_iBuyItemUID;
	int						m_iBuyQuantity;

	// GSUser객체 소멸시 직접 DB업데이트용 정보
	KItemQuantityUpdate                 m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >            m_vecItemInfoTemp;
	int									m_iEDTemp;
};

DECL_PACKET( ERM_BREAK_PERSONAL_SHOP_NOT )
{
	int				m_iReason;
	UidType			m_iHostUID;
};

typedef KDBE_INSERT_TRADE_ITEM_REQ KDBE_INSERT_BUY_PERSONAL_SHOP_ITEM_REQ;
typedef KDBE_INSERT_TRADE_ITEM_ACK KDBE_INSERT_BUY_PERSONAL_SHOP_ITEM_ACK;

//{{ 2009. 9. 22  최육사	전직캐쉬
DECL_PACKET( DBE_CHANGE_UNIT_CLASS_REQ )
{
	u_short					m_usEventID;
	UidType					m_iUnitUID;
	char        			m_cUnitClass;

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	int						m_iNewDefaultSkill1;
	int						m_iNewDefaultSkill2;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iTheNumberOfSkillPagesAvailable;
#endif // SERV_SKILL_PAGE_SYSTEM


	KDBE_CHANGE_UNIT_CLASS_REQ()
	{
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iTheNumberOfSkillPagesAvailable = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
	};
};

DECL_PACKET( DBE_CHANGE_UNIT_CLASS_ACK )
{
	int						m_iOK;
	u_short					m_usEventID;	
	char        			m_cUnitClass;
	
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	int						m_iNewDefaultSkill1;
	int						m_iNewDefaultSkill2;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iTheNumberOfSkillPagesAvailable;
#endif // SERV_SKILL_PAGE_SYSTEM

	KDBE_CHANGE_UNIT_CLASS_ACK()
	{
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
	};

};
//}}

//{{ 2008. 2. 18  최육사  
DECL_PACKET( DBE_RESOLVE_ITEM_REQ )
{
	UidType                             m_iUnitUID;
	bool								m_bJackpot;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
};

DECL_PACKET( DBE_RESOLVE_ITEM_ACK )
{
	int                                 m_iOK;
	bool								m_bJackpot;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
};
//}}

DECL_PACKET( DBE_ENCHANT_ITEM_REQ )
{
    UidType                             m_iUnitUID;
    int                                 m_iEnchantResult;
    UidType                             m_iItemUID;
	int									m_iItemID;
    int                                 m_iLevelAfterEnchant;
    KItemQuantityUpdate                 m_kItemQuantityUpdate;
    std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_ENCHANT_ITEM_ACK )
{
	int									m_iOK;
	int                                 m_iEnchantResult;
	UidType                             m_iItemUID;
	int									m_iItemID;
	int                                 m_iLevelAfterEnchant;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

//{{ 2008. 12. 18  최육사	보조제 로그
DECL_PACKET( ELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT )
{
	UidType								m_iItemUID;
	char								m_cBeforeLevel;
	char								m_cAfterLevel;
	//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가 - 로그 남기기
	// 파괴 방지까지 합쳐서 한번에 적용. 
#ifdef SERV_ENCHANT_PLUS_ITEM
	bool								m_bUseSupport;	// 플루오르 스톤 썻냐?
	bool								m_bUsePlus;		// 강화 확률 증가 썻냐?
	bool								m_bUseDestroyGuard;	// 파괴 방지 썼냐?
#endif SERV_ENCHANT_PLUS_ITEM
	//}}
};
//}}

DECL_DATA( KEventInfo )
{
	int				m_iEventUID;
	int				m_iEventType;
	std::wstring	m_wstrEventName;
	std::wstring	m_wstrBeginDate;
	std::wstring	m_wstrEndDate;
	int				m_iScriptID;
	bool			m_bEnable;
#ifdef SERV_ALLOW_EVENT_ERASE
	bool			m_bOn;
#endif SERV_ALLOW_EVENT_ERASE

	CON_COPYCON_ASSIGNOP( KEventInfo, right )
	{
		m_iEventUID		= right.m_iEventUID;
		m_iEventType	= right.m_iEventType;
		m_wstrEventName = right.m_wstrEventName;
		m_wstrBeginDate = right.m_wstrBeginDate;
		m_wstrEndDate	= right.m_wstrEndDate;
		m_iScriptID		= right.m_iScriptID;
		m_bEnable		= right.m_bEnable;
#ifdef SERV_ALLOW_EVENT_ERASE
		m_bOn			= right.m_bOn;
#endif SERV_ALLOW_EVENT_ERASE
		return *this;
	}
};

DECL_PACKET( DBE_EVENT_UPDATE_ACK )
{
	std::vector< KEventInfo >			m_vecEventList;
};

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
DECL_PACKET( DBE_RELEASE_TICK_UPDATE_ACK )
{
	std::map< int, int >	m_mapReleaseTick;
};

DECL_PACKET( DBE_EVENT_DATA_UPDATE_ACK )
{
	std::map< int, EVENT_DATA >					m_mapEventData;
};

DECL_PACKET( DBE_EVENT_MONSTER_UPDATE_ACK )
{
	std::map< int, EVENT_MONSTER_DATA >			m_mapEventMonster;
};

DECL_PACKET( DBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK )
{
	std::vector< std::pair< int, int > >		m_vecEventNotifyMsgServerInfo;
};

DECL_PACKET( DBE_EVENT_REWARD_DATA_UPDATE_ACK )
{
	std::map< int, std::vector< KRewardData > > m_mapRewardData;
};

//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
DECL_PACKET( DBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK )
{
	std::map< u_char, std::vector< int > >		m_mapLevelUpRewardItem;
};
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
DECL_PACKET( DBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK )
{
	std::map< int, KRewardItem >				m_mapRewardItem;
};
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2008. 2. 26  최육사  개인상점 로그
DECL_PACKET( ELOG_INSERT_PERSONAL_SHOP_LOG_NOT )
{
	bool								m_bIsSquare;
	//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	char								m_cPShopType;
#endif SERV_PSHOP_AGENCY
	//}}
	UidType								m_iFromUnitUID;
	UidType								m_iToUnitUID;
	UidType								m_iItemUID;
	int									m_iItemID;	
	int									m_iQuantity;
	int									m_iED;
};
//}}

DECL_PACKET( DBE_UPDATE_UNIT_CLASS_NOT )
{
    UidType                             m_iUnitUID;
    char                                m_cUnitClass;
};

//{{ 2008. 3. 4  최육사  어뷰저 로그
DECL_PACKET( ELOG_INSERT_ABUSER_LOG_NOT )
{
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrIP;
	int									m_iGameType;
	float								m_fPlayTimeLimit;
	bool					            m_bPublic;
	int									m_iPlayWithMember;
	std::wstring						m_wstrEndGameTime;
	int									m_iPlayTime;
	int									m_iRewardVP;
	int									m_iRewardEXP;
	int									m_iKillCount;
	int									m_iDeathCount;
};
//}}

//{{ 2008. 3. 5  최육사  소켓
//{{ 2010. 04. 15  최육사	소켓 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////

DECL_PACKET( DBE_SOCKET_ITEM_REQ )
{
	UidType								m_iUnitUID;
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >					m_vecSocketInfo;
#else
	std::vector< short >				m_vecSocketInfo;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}}
	UidType                             m_iItemUID;
	int									m_iItemID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	int									m_iSocketUseCount;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
	byte								m_byteExpandedSocketNum;
#endif // SERV_BATTLE_FIELD_BOSS

	KDBE_SOCKET_ITEM_REQ()
		: m_iUnitUID( 0 )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		, m_byteExpandedSocketNum( 0 )
#endif // SERV_BATTLE_FIELD_BOSS
	{
	}
};

DECL_PACKET( DBE_SOCKET_ITEM_ACK )
{
	int									m_iOK;
	//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
	std::vector< int >					m_vecSocketInfo;
#else
	std::vector< short >				m_vecSocketInfo;
#endif SERV_ITEM_OPTION_DATA_SIZE
	//}} 
	UidType                             m_iItemUID;
	int									m_iItemID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
	int									m_iSocketUseCount;
#endif SERV_QUEST_CLEAR_EXPAND
	//}}
};

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

DECL_PACKET( DBE_SOCKET_ITEM_REQ )
{
	UidType								m_iUnitUID;
	std::vector< short >				m_vecSocketInfo;
	int                                 m_iSocketResult;
	UidType                             m_iItemUID;
	int									m_iItemID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_SOCKET_ITEM_ACK )
{
	std::vector< short >				m_vecSocketInfo;
	int                                 m_iSocketResult;
	UidType                             m_iItemUID;
	int									m_iItemID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

//////////////////////////////////////////////////////////////////////////
#endif SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////
//}}
//}}

DECL_PACKET( ERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT )
{
    UidType                             m_UnitUID;
    std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iEquippedTitleID;
#else
	short								m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
};

DECL_PACKET( ERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT )
{
	//{{ 2010. 12. 23	최육사	장비 변경 패킷 타입
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
	char								m_cChangeEquippedPacketType;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
	//}}
    UidType                             m_UnitUID;
    std::vector< KInventoryItemInfo>    m_vecInventorySlotInfo;
    KRoomUserInfo                       m_kRoomUserInfo;
	
	KERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT()
	{
		//{{ 2010. 12. 23	최육사	장비 변경 패킷 타입
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
		m_cChangeEquippedPacketType = 0;
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
		//}}
		m_UnitUID = 0;
	}
};

//{{ 2008. 11. 30  최육사	
DECL_PACKET( ERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT )
{
	UidType								m_iUnitUID;
	std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iEquippedTitleID;
#else
	short								m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
};

DECL_PACKET( ETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iUnitUID;
	std::vector< KInventoryItemInfo >   m_vecInventorySlotInfo;
	//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
	int									m_iEquippedTitleID;
#else
	short								m_sEquippedTitleID;
#endif SERV_TITLE_DATA_SIZE
	//}}
};
//}}

DECL_PACKET( ELOG_RESURRECTION_STONE_LOG_NOT )
{
	UidType								m_UnitUID;
	int									m_iDungeonID;
	std::wstring						m_wstrRegDate;
};

DECL_PACKET( ELOG_RESOLVE_JACKPOT_LOG_NOT )
{
	UidType								m_UnitUID;
	int									m_iResolveItemID;
	int									m_iElapsedTime;
	std::wstring						m_wstrRegDate;
};

DECL_PACKET( DBE_INIT_SKILL_TREE_REQ )
{
	UidType			m_iUnitUID;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	UidType			m_iItemUID;
	int				m_iSPoint;
	int				m_iCSPoint;
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	int				m_iDefaultSkillID1;
	int				m_iDefaultSkillID2;
	int				m_iDefaultSkillID3;
	int				m_iDefaultSkillID4;
	int				m_iDefaultSkillID5;
	int				m_iDefaultSkillID6;
	int				m_iBeforeSPoint;
	int				m_iBeforeCSPoint;

	KDBE_INIT_SKILL_TREE_REQ()
	{
		m_iUnitUID	= 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iItemUID	= 0;
		m_iSPoint	= 0;
		m_iCSPoint	= 0;
		m_iDefaultSkillID1	= 0;
		m_iDefaultSkillID2	= 0;
		m_iDefaultSkillID3	= 0;
		m_iDefaultSkillID4	= 0;
		m_iDefaultSkillID5	= 0;
		m_iDefaultSkillID6	= 0;
		m_iBeforeSPoint = 0;
		m_iBeforeCSPoint = 0;

	}
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	int				m_iDefaultSkillID;
	int				m_iDefaultSkillID2;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
};

DECL_PACKET( DBE_INIT_SKILL_TREE_ACK )
{
	int				m_iOK;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	UidType			m_iItemUID;
	int				m_iSPoint;
	int				m_iCSPoint;
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	int				m_iDefaultSkillID1;
	int				m_iDefaultSkillID2;
	int				m_iDefaultSkillID3;
	int				m_iDefaultSkillID4;
	int				m_iDefaultSkillID5;
	int				m_iDefaultSkillID6;
	int				m_iBeforeSPoint;
	int				m_iBeforeCSPoint;

	KDBE_INIT_SKILL_TREE_ACK()
	{
		m_iOK	= 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iItemUID	= 0;
		m_iSPoint	= 0;
		m_iCSPoint	= 0;
		m_iDefaultSkillID1	= 0;
		m_iDefaultSkillID2	= 0;
		m_iDefaultSkillID3	= 0;
		m_iDefaultSkillID4	= 0;
		m_iDefaultSkillID5	= 0;
		m_iDefaultSkillID6	= 0;
		m_iBeforeSPoint = 0;
		m_iBeforeCSPoint = 0;

	}
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	int				m_iDefaultSkillID;
	int				m_iDefaultSkillID2;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

};

//{{ 2008. 3. 28  최육사  보상
DECL_PACKET( DBE_RECOMMEND_USER_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
};

DECL_PACKET( DBE_RECOMMEND_USER_ACK )
{
	int									m_iOK;
    UidType								m_iRecommendedUnitUID;
};

DECL_PACKET( DBE_INSERT_REWARD_TO_POST_REQ )
{
	UidType								m_iFromUnitUID;
	UidType								m_iToUnitUID;
	int									m_iRewardType;
	int									m_iRewardID;
	short								m_sQuantity;	
	//{{ 2008. 9. 26  최육사	우체국 확장
	std::wstring						m_wstrMessage;
	//}}
	bool								m_bGameServerEvent;

	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	int									m_iDBIndex;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

	KDBE_INSERT_REWARD_TO_POST_REQ()
	{
		m_iFromUnitUID = 0;
		m_iToUnitUID = 0;
		m_iRewardType = 0;
		m_iRewardID = 0;
		m_sQuantity = 0;
		m_bGameServerEvent = false;
		//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		m_iDBIndex = 0;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}
	}
};

DECL_PACKET( DBE_INSERT_REWARD_TO_POST_ACK )
{
	int									m_iOK;
	//{{ 2008. 9. 30  최육사	우체국 확장
	KPostItemInfo						m_iRewardLetter;
	//}}
	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	int									m_iDBIndex;
	
	KDBE_INSERT_REWARD_TO_POST_ACK()
	{
		m_iDBIndex				= 0;
	}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}    
};
//}}

//{{ 2008. 3. 30  최육사  우체국
DECL_PACKET( DBE_GET_POST_LETTER_LIST_REQ )
{
	UidType	m_iUnitUID;

	KDBE_GET_POST_LETTER_LIST_REQ()
		: m_iUnitUID( 0 )
	{
	}
};

DECL_PACKET( DBE_GET_POST_LETTER_LIST_ACK )
{
	int									m_iOK;
	std::vector< KPostItemInfo >		m_vecPostItem;
	std::vector< std::wstring >			m_vecBlackList;

	KDBE_GET_POST_LETTER_LIST_ACK()
		: m_iOK( 0 )
	{
	}
};

//{{ 2008. 9. 18  최육사	편지 보내기
DECL_PACKET( DBE_PREPARE_INSERT_LETTER_TO_POST_REQ )
{	
	UidType								m_iFromUnitUID;
	UidType								m_iItemUID;
	std::wstring						m_wstrToNickName;
	KPostItemInfo						m_kSendLetter;
	int									m_iSendLetterCost;

	//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	int									m_iIncrementED;			// ED 증분 값
	KItemQuantityUpdate					m_kItemQuantityUpdate;	// Item Update

	KDBE_PREPARE_INSERT_LETTER_TO_POST_REQ()
	{
		m_iFromUnitUID		= 0;
		m_iItemUID			= 0;
		m_iSendLetterCost	= 0;
		m_iIncrementED		= 0;
	}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}
};

DECL_PACKET( DBE_PREPARE_INSERT_LETTER_TO_POST_ACK )
{
	int									m_iOK;
	UidType								m_iItemUID;
	std::wstring						m_wstrToNickName;
	KPostItemInfo						m_kSendLetter;
	int									m_iSendLetterCost;

	//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	int									m_iIncrementED;			// ED 증분 값
	KItemQuantityUpdate					m_kItemQuantityUpdate;	// Item Update

	KDBE_PREPARE_INSERT_LETTER_TO_POST_ACK()
	{
		m_iItemUID			= 0;
		m_iSendLetterCost	= 0;
		m_iIncrementED		= 0;
	}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}
};

DECL_PACKET( DBE_INSERT_LETTER_TO_POST_REQ )
{
	UidType								m_iFromUnitUID;
	std::wstring						m_wstrToNickName;
	KPostItemInfo						m_kSendLetter;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	int									m_iSendLetterCost;
};

DECL_PACKET( DBE_INSERT_LETTER_TO_POST_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrToNickName;
	KPostItemInfo						m_kSendLetter;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	int									m_iSendLetterCost;
};

DECL_PACKET( ELG_FIND_USER_SEND_LETTER_NOT )
{
	KPostItemInfo						m_kLetter;
};
//}}

//{{ 2008. 9. 18  최육사	우편함
DECL_PACKET( DBE_READ_LETTER_NOT )
{
	UidType								m_iUnitUID;
	UidType								m_iPostNo;
};

DECL_PACKET( DBE_GET_ITEM_FROM_LETTER_REQ )
{
	UidType                             m_iUnitUID;
	UidType								m_iPostNo;
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	char								m_cLetterType;
#else
	bool								m_bSystemLetter;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
	int									m_iED;

	//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	int									m_iIncrementED;			// ED 증분 값
	KItemQuantityUpdate					m_kItemQuantityUpdate;	// Item Update
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}

	KDBE_GET_ITEM_FROM_LETTER_REQ()
	{
		m_iUnitUID		= 0;
		m_iPostNo		= 0;
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		m_cLetterType	= 0;
#else
		m_bSystemLetter	= false;
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		m_iED			= 0;
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		m_iIncrementED	= 0;
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
	}

};

DECL_PACKET( DBE_GET_ITEM_FROM_LETTER_ACK )
{
	int									m_iOK;
	UidType								m_iPostNo;
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	char								m_cLetterType;
	int									m_iWeddingUID;
	UidType								m_iWeddingItemUID;
#else
	bool								m_bSystemLetter;
#endif SERV_RELATIONSHIP_SYSTEM
	//}
	std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	int									m_iED;

	//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
	int									m_iIncrementED;			// ED 증분 값
	KItemQuantityUpdate					m_kItemQuantityUpdate;	// Item Update

	KDBE_GET_ITEM_FROM_LETTER_ACK()
	{
		m_iPostNo		= 0;
		m_bSystemLetter	= false;
		m_iED			= 0;
		m_iIncrementED	= 0;
	}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
	//}}
};

DECL_PACKET( DBE_DELETE_LETTER_NOT )
{
	UidType								m_iUnitUID;
	std::vector< UidType >				m_vecPostNo;
};
//}}

DECL_PACKET( DBE_INSERT_POST_ITEM_TO_INVENTORY_REQ )
{
	UidType                             m_iUnitUID;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
	std::vector< KPostItemInfo >		m_vecPostItemInfo;
};

DECL_PACKET( DBE_INSERT_POST_ITEM_TO_INVENTORY_ACK )
{
	int                                 m_iOK;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	std::vector< KPostItemInfo >		m_vecPostItemInfo;
};
//}}

DECL_PACKET( ERM_SET_QUEST_ITEM_INFO_NOT )
{
	std::map<int, KSubQuestInfo>		m_mapDropQuestItembyIngQuest;
	//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	std::set< int >						m_setQuestInfo;
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
	std::set< int >						m_setGoingQuestInfo;
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
};

DECL_PACKET( DBE_DEL_TUTORIAL_REQ )
{
	UidType		m_iTeacherUID;
	UidType		m_iStudentUID;
	char		m_cReason;
};

DECL_PACKET( DBE_DEL_TUTORIAL_ACK )
{
	int			m_iOK;
	UidType		m_iTeacherUID;
	UidType		m_iStudentUID;
	char		m_cReason;
};

DECL_PACKET( ELG_DEL_TUTORIAL_NOT )
{
	UidType		m_iReceiverUnitUID;
	UidType		m_iDeleteUnitUID;
	//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	int				m_iReason;	
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	std::wstring	m_wstrReason;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}
};

DECL_PACKET( ELG_REQUEST_TUTORIAL_NOT )
{
	UidType				m_iReceiverUnitUID;
	KTutorialUnitInfo	m_kTutorialUnitInfo;
};

//DECL_PACKET( ELG_REQUEST_TUTORIAL_REPLY_NOT )
//{
//	UidType				m_iReceiverUnitUID;
//	KTutorialUnitInfo	m_kTutorialUnitInfo;
//	bool				m_bAccept;
//};

DECL_PACKET( DBE_INSERT_TUTORIAL_REQ )
{
	UidType			m_iTeacherUID;

	KTutorialUnitInfo	m_kStudentUnitInfo;
};

DECL_PACKET( DBE_INSERT_TUTORIAL_ACK )
{
	int				m_iOK;
	UidType			m_iTeacherUID;

	KTutorialUnitInfo	m_kStudentUnitInfo;
};

DECL_PACKET( ELOG_GET_PURCHASED_CASH_ITEM_NOT )
{
    UidType                             m_iUnitUID;
    int                                 m_iItemID;
    short                               m_sPeriod;
    int									m_iQuantity;
	u_char								m_ucLevel;
	char								m_cUnitClass;
};

DECL_PACKET( DBE_EXPAND_INVENTORY_SLOT_REQ )
{
    UidType                             m_iUnitUID;
	unsigned short						m_usEventID;
    std::map< int, int >                m_mapExpandedSlot; // first = type ( SORT_TYPE ), second : number of slot
    int                                 m_iED; // ED 로 확장하는 경우 비용
};

DECL_PACKET( DBE_EXPAND_INVENTORY_SLOT_ACK )
{
	unsigned short						m_usEventID;
    std::map< int, int >                m_mapExpandedSlot;
    int                                 m_iED;
    int                                 m_iOK;
};

typedef KDBE_EXPAND_INVENTORY_SLOT_REQ    KDBE_EXPAND_BANK_INVENTORY_REQ;
typedef KDBE_EXPAND_INVENTORY_SLOT_ACK    KDBE_EXPAND_BANK_INVENTORY_ACK;

//{{ 2008. 4. 21  최육사  스킬슬롯B
DECL_PACKET( DBE_EXPAND_SKILL_SLOT_REQ )
{
	UidType								m_iUnitUID;
	int									m_iPeriodExpire;		// 0 이면 영구 
	
#ifdef SERV_SKILL_PAGE_SYSTEM
	USHORT								m_usTheNumberOfSkillPagesAvailable;
#endif // SERV_SKILL_PAGE_SYSTEM


	//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
	u_short								m_usEventID;
#endif SERV_ADD_PACKAGE_PRODUCT
	//}}

	KDBE_EXPAND_SKILL_SLOT_REQ()
	{
		m_iUnitUID = 0;
		m_iPeriodExpire = 0;

#ifdef SERV_SKILL_PAGE_SYSTEM
		m_usTheNumberOfSkillPagesAvailable = 1;
#endif // SERV_SKILL_PAGE_SYSTEM

		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
		m_usEventID = 0;
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
	}
};

DECL_PACKET( DBE_EXPAND_SKILL_SLOT_ACK )
{
	int									m_iOK;
	int									m_iPeriodExpire;
	std::wstring						m_wstrSkillSlotBEndDate;
	//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
	u_short								m_usEventID;
#endif SERV_ADD_PACKAGE_PRODUCT
	//}}

	KDBE_EXPAND_SKILL_SLOT_ACK()
	{
		m_iOK = 0;
		m_iPeriodExpire = 0;
		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
		m_usEventID = 0;
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
	}
};
//}}



//{{ 2008. 12. 14  최육사	캐릭터 슬롯 확장
DECL_PACKET( DBE_EXPAND_CHAR_SLOT_REQ )
{
    int									m_iExpandSlotSize;
	int									m_iCharSlotMax;
};

DECL_PACKET( DBE_EXPAND_CHAR_SLOT_ACK )
{
	int									m_iOK;
	int									m_iCharSlotSize;
};
//}}

//{{ 2008. 5. 5  최육사  장바구니 얻어오기
DECL_PACKET( DBE_GET_WISH_LIST_REQ )
{
	UidType								m_iUserUID;
};

DECL_PACKET( DBE_GET_WISH_LIST_ACK )
{
	std::map< int, int >				m_mapWishList;
};
//}}

DECL_PACKET( DBE_UPDATE_IS_LOGIN_NOT )
{
	UidType								m_iUserUID;
	bool								m_bIsLogin;
	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	std::map< int, int >				m_mapAccCountInfo;
#endif SERV_ACCOUNT_COUNT
	//}}
};

//{{ 2008. 5. 19  최육사  
DECL_PACKET( ENX_UPDATE_PRODUCT_LIST_NOT )
{
	std::vector< int >					m_vecProductItemID;
	std::vector< KCashItemResolveInfo > m_vecCashItemRessolveInfo;
	//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
	std::vector< KCashPruductInfo >		m_vecCashProductList;
#endif SERV_CASH_ITEM_LIST
	//}}
	unsigned long						m_ulProductNoResStone;
};
//}}

//{{ 2008. 5. 19  최육사  체험ID 이벤트
DECL_PACKET( DBE_UPDATE_EVENT_TIME_REQ )
{
	UidType									m_iUnitUID;
	std::vector< KConnectTimeEventInfo >	m_vecConnectTimeEvent;
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	std::vector< KCumulativeTimeEventInfo > m_vecCumulativeTimeEvent;
#endif CUMULATIVE_TIME_EVENT
	//}}
};

DECL_PACKET( DBE_UPDATE_EVENT_TIME_ACK )
{
	int									m_iOK;
	std::set< int >						m_setConnectTimeEvent;
	//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
	std::set< int >						m_setCumulativeTimeEvent;
#endif CUMULATIVE_TIME_EVENT
	//}}
};
//}}

//{{ 2008. 5. 21  최육사  아이템 어뷰저 로그
DECL_PACKET( ELOG_ITEM_ABUSER_LOG_NOT )
{
	UidType								m_iUnitUID;
	std::vector< KItemAbuserLogInfo >	m_vecItemAbuserLog;
};

DECL_PACKET( ELOG_ENCHANT_ABUSER_LOG_NOT )
{
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	int									m_iItemID;
	char								m_cEnchantLevel;
	int									m_iEnchantResult;	
};

DECL_PACKET( ELOG_ED_ABUSER_LOG_NOT )
{
	UidType								m_iUnitUID;
	int									m_iState;
	int                                 m_iBeforeED;
	int									m_iAfterED;
	int									m_iED;    
};
//}}

DECL_PACKET( ERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT )
{
	std::vector<UidType>				m_vecStudentUnitUID;	//사제시스템중 제자리스트
};

//{{ 2008. 5. 28  최육사  인벤 아이템 슬롯 정보 업데이트
DECL_PACKET( DBE_UPDATE_INVENTORY_ITEM_POS_NOT )
{
	UidType								m_iUnitUID;
	KItemPositionUpdate                 m_kItemPositionUpdate;	
};
//}}

//{{ 2008. 6. 20  최육사  선물하기
DECL_PACKET( ELG_PRESENT_CASH_ITEM_REQ )
{
	UINT32								m_uiPwdHash;
	std::wstring						m_wstrPassword;
	std::wstring                        m_wstrReceiverNickName;
	std::wstring                        m_wstrMessage;
	std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KELG_PRESENT_CASH_ITEM_REQ()
		: m_uiPwdHash( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};

DECL_PACKET( ELG_PRESENT_CASH_ITEM_ACK )
{
	int                                 m_iOK;
	std::wstring                        m_wstrReceiverNickName;
	std::wstring                        m_wstrMessage;
	std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KELG_PRESENT_CASH_ITEM_ACK()
		: m_iOK( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};

DECL_PACKET( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ )
{
	std::wstring                        m_wstrReceiverNickName;
	std::wstring                        m_wstrMessage;
	std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ()
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		: m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
		m_wstrReceiverNickName		=	L"";
		m_wstrMessage				=	L"";

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		m_iUseCashType				=	-1;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
	}
};

DECL_PACKET( DBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK )
{
	int                                 m_iOK;
	std::wstring                        m_wstrUserID;
	std::wstring                        m_wstrMessage;
	std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;

#ifdef SERV_GLOBAL_BILLING
	UidType								m_iReceiverUserUID;
	UidType								m_iReceiverUnitUID;
	std::wstring						m_wstrReceiverNickName;

	char								m_cReceiverLevel;
	int									m_iReceiverUnitClass;
#endif // SERV_GLOBAL_BILLING

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
	int									m_iUseCashType;
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK()
		: m_iOK( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};
//}}

//{{ 2009. 7. 6  최육사		랭킹 개편
DECL_PACKET( DBE_DELETE_RANKING_INFO_NOT )
{
    int									m_iRankingType;
};

DECL_PACKET( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ )
{
	UidType								m_iServerUID;
};

DECL_PACKET( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK )
{
	std::map< int, std::vector< KHenirRankingInfo > >	m_mapHenirRanking;
	std::map< int, u_int >								m_mapLastRank;
};

DECL_PACKET( ELG_HENIR_RANKING_REFRESH_NOT )
{
	std::map< int, std::vector< KHenirRankingInfo > >	m_mapHenirRanking;
};

typedef KELG_HENIR_RANKING_REFRESH_NOT		KDBE_BACKUP_RANKING_INFO_NOT;
typedef KELG_HENIR_RANKING_REFRESH_NOT		KDBE_GET_HENIR_RANKING_INFO_ACK;

DECL_PACKET( DBE_GET_WEB_RANKING_INFO_ACK )
{
	std::vector< KDungeonRankingInfo >					m_vecDungeonRanking;
	std::vector< KPvpRankingInfo >						m_vecPvpRanking;
};

//{{ 2009. 11. 9  최육사	로그인서버GameDB
DECL_PACKET( ELG_GET_WEB_RANKING_REFRESH_NOT )
{
	UidType												m_iServerUID;
	char												m_cRankingRefreshType;
    u_int												m_uiLastIndex;
};

DECL_PACKET( ELG_WEB_RANKING_REFRESH_NOT )
{
	enum RANKING_REFRESH_TYPE
	{
        DUNGEON_VECTOR = 0,
		DUNGEON_MAP,
		PVP_VECTOR,
		PVP_MAP,
	};

	bool												m_bInit;
	char												m_cRankingRefreshType;
	u_int												m_uiTotalSize;
	std::vector< KDungeonRankingInfo >					m_vecDungeonRanking;
	std::map< UidType, KDungeonRankingInfo >			m_mapDungeonRanking;
	std::vector< KPvpRankingInfo >						m_vecPvpRanking;
	std::map< UidType, KPvpRankingInfo >				m_mapPvpRanking;

	KELG_WEB_RANKING_REFRESH_NOT()
	{
		m_bInit = false;
		m_cRankingRefreshType = -1;
		m_uiTotalSize = 0;
	}
};
//}}

DECL_PACKET( ELG_NEW_RECORD_HENIR_RANKING_NOT )
{
	KHenirRankingInfo					m_kNewRecordRanking;
};

//{{ 2009. 7. 31  최육사	랭킹 추가 기능
DECL_PACKET( ELG_ADMIN_NEW_RECORD_TEST_NOT )
{
	std::vector< KHenirRankingInfo >	m_vecNewRecordTest;
};

DECL_PACKET( ELG_ADMIN_NEW_RECORD_NOT )
{
	int									m_iRankingType;
	KHenirRankingInfo					m_kNewRecordRanking;
};

DECL_PACKET( ELG_ADMIN_DELETE_RECORD_NOT )
{
	int									m_iRankingType;
	std::wstring						m_wstrNickName;
};
//}}

//{{ 2008. 9. 3  최육사		속성강화
DECL_PACKET( DBE_ATTRIB_ENCHANT_ITEM_REQ )
{
	UidType                             m_iUnitUID;	
	UidType                             m_iItemUID;
	int									m_iItemID;
	char                                m_cAttribEnchantSlotNo;
	char                                m_cAttribEnchantID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_ATTRIB_ENCHANT_ITEM_ACK )
{
	int									m_iOK;
	UidType                             m_iItemUID;
	int									m_iItemID;
	char                                m_cAttribEnchantSlotNo;
	char                                m_cAttribEnchantID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

typedef KDBE_OPEN_RANDOM_ITEM_REQ		KDBE_IDENTIFY_ITEM_REQ;
typedef KDBE_OPEN_RANDOM_ITEM_ACK		KDBE_IDENTIFY_ITEM_ACK;
//}}

//{{ 2008. 9. 26  최육사	우체국 블랙리스트
DECL_PACKET( DBE_NEW_POST_BLACK_LIST_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
};

DECL_PACKET( DBE_NEW_POST_BLACK_LIST_ACK )
{
	int									m_iOK;
    std::wstring						m_wstrNickName;
};

typedef KDBE_NEW_POST_BLACK_LIST_REQ		KDBE_DEL_POST_BLACK_LIST_REQ;
typedef KDBE_NEW_POST_BLACK_LIST_ACK		KDBE_DEL_POST_BLACK_LIST_ACK;
//}}

//{{ 2008. 10. 7  최육사	타이틀
DECL_PACKET( DBE_INSERT_TITLE_REQ )
{
	UidType								m_iUnitUID;
    int									m_iTitleID;
	short								m_sPeriod;

	bool								m_bGameServerEvent;
	//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
	bool								m_bExpandPeriod;
#endif SERV_TITLE_ITEM_NEW
	//}}

	KDBE_INSERT_TITLE_REQ()
	{
		m_bGameServerEvent = false;
		//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
		m_bExpandPeriod = false;
#endif SERV_TITLE_ITEM_NEW
		//}}
	}
};

DECL_PACKET( DBE_INSERT_TITLE_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	int									m_iTitleID;
	std::wstring						m_wstrEndDate;
};

DECL_PACKET( ELG_RANKING_TITLE_REWARD_NOT )
{
	std::map< UidType, KRankingTitleReward > m_mapTitleReward;
};
//}}

//{{ 2008. 10. 22  최육사	캐쉬 분해 로그
DECL_PACKET( ELOG_CASH_ITEM_RESOLVE_LOG_NOT )
{
    UidType								m_iUnitUID;
	UidType								m_iItemUID;
	int									m_iResultQuantity;
	//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
	int									m_iAfterItemID;
	int								    m_iAfterItemQuantity;
	int									m_iLevel;
#endif SERV_MULTI_RESOLVE
	//}}
};
//}}

//{{ 2008. 11. 10  최육사	아이템 강화시 파괴로그
DECL_PACKET( ELOG_ENCHANT_ITEM_BREAK_LOG_NOT )
{
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	int									m_iItemID;
	char								m_cEnchantLevel;
};
//}}

//{{ 2008. 10. 30  최육사	어뷰저 리스트
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
DECL_PACKET( DBE_ABUSER_LIST_REQ )
{
	int m_iRtt;
};
#else
DECL_PACKET( DBE_ABUSER_LIST_REQ )
{
	std::map< int, int >				m_mapReleaseTick;
};
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
DECL_PACKET( DBE_ABUSER_LIST_ACK )
{	
	int							m_iRtt;
	std::vector< UidType >		m_vecAbuserList;
};
#else
DECL_PACKET( DBE_ABUSER_LIST_ACK )
{	
	std::map< int, KAbuserList >		m_mapAbuserList;
};
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//}}

//{{ 2008. 11. 10  최육사	sms
DECL_PACKET( DBE_SEND_PHONE_MSG_NOT )
{
	std::vector< std::wstring >			m_vecPhoneNum;
	std::wstring						m_wstrSMSMessage;
};
//}}

//{{ 2008. 12. 21  최육사	아이템 복구
DECL_PACKET( DBE_RESTORE_ITEM_REQ )
{
	UidType								m_iUnitUID;
	UidType                             m_iItemUID;	
	int                                 m_iLevelAfterEnchant;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_RESTORE_ITEM_ACK )
{
	int									m_iOK;
	UidType                             m_iItemUID;
	int                                 m_iLevelAfterEnchant;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( ELOG_RESTORE_ITEM_LOG_NOT )
{
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	int									m_iBeforeEnchantLv;
	int									m_iAfterEnchantLv;
};
//}}

//{{ 2008. 12. 25  최육사	부여
DECL_PACKET( DBE_ENCHANT_ATTACH_ITEM_REQ )
{
	UidType								m_iUnitUID;
	UidType                             m_iItemUID;	
	int                                 m_iLevelAfterEnchant;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_ENCHANT_ATTACH_ITEM_ACK )
{
	int									m_iOK;
	UidType                             m_iItemUID;
	int                                 m_iLevelAfterEnchant;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( ELOG_ATTACH_ITEM_ENCHANT_LOG_NOT )
{
	UidType								m_iUnitUID;
	int									m_iAttachItemID;
	UidType								m_iDestItemUID;
	int									m_iBeforeEnchantLv;
	int									m_iAfterEnchantLv;
};
//}}

//{{ 2008. 11. 18  최육사	아이템 교환
DECL_PACKET( DBE_ITEM_EXCHANGE_REQ )
{
	UidType                             m_iUnitUID;	
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;	
	//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
	std::map< int, int >				m_mapResultItem;
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
	//}}

	//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	std::vector<int>					m_vecDestItem;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
	//}}

	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	int									m_iSourceItemID;		// 재료 아이템 ID
	int									m_iSourceItemQuantity;	// 재료 아이템 수량
	char								m_cExchangeType;
#endif SERV_EXCHANGE_LOG
	//}
	
};

DECL_PACKET( DBE_ITEM_EXCHANGE_ACK )
{
	int                                 m_iOK;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
	std::map< int, int >				m_mapResultItem;
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
	//}}
	//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
	int									m_iSourceItemID;		// 재료 아이템 ID
	int									m_iSourceItemQuantity;	// 재료 아이템 수량
	char								m_cExchangeType;
#endif SERV_EXCHANGE_LOG
	//}

#ifdef SERV_GROW_UP_SOCKET
	int									m_iExchangeCount;
#endif SERV_GROW_UP_SOCKET

#ifdef SERV_2013_JUNGCHU_TITLE
	bool								m_b12TimesRewarded;
#endif SERV_2013_JUNGCHU_TITLE
};

DECL_PACKET( ELOG_ITEM_EXCHANGE_LOG_NOT )
{
	UidType								m_iUnitUID;
	UidType								m_iDeletedItemUID;
	int									m_iDeletedItemID;
	UidType								m_iChangedItemUID;
	int									m_iChangedItemID;
};
//}}

//{{ 2008. 11. 19  최육사	어뷰저 자동 등록
DECL_PACKET( DBE_REG_ABUSER_AUTOMATIC_NOT )
{
	std::map< int, UidType >			m_mapRegAbuserInfo;

	void AddAbuserInfo( const int iType, const UidType iUID )
	{
		m_mapRegAbuserInfo.insert( std::make_pair( iType, iUID ) );
	}
};
//}}

//{{ 2008. 12. 30  최육사	매력아이템 로그
DECL_PACKET( ELOG_CHARM_ITEM_LOG_NOT )
{
	UidType								m_iUnitUID;
	int									m_iCharmItemID;
	int									m_iResultItemID;	
	int									m_iQuantity;
};
//}}

//{{ 2008. 11. 4  최육사	광장 개편
DECL_PACKET( ERM_UPDATE_FIELD_INFO_NOT )
{
    int									m_iMapID;
	UidType								m_iFieldUID;
};

DECL_PACKET( ERM_JOIN_FIELD_REQ )
{
	UidType								m_iFieldUID;
	KFieldUserInfo						m_kFieldUserInfo;
	//{{ 2008. 12. 9  최육사	친구, 파티 리스트
	std::set< UidType >					m_setPartyUIDList;
	std::set< UidType >					m_setFriendUIDList;
	KFieldOption						m_kFieldOption;
	//}}
};

DECL_PACKET( ERM_JOIN_FIELD_ACK )
{
	int									m_iOK;
    UidType								m_iFieldUID;
};

DECL_PACKET( ERM_LEAVE_FIELD_REQ )
{
	int									m_iReason;
	int									m_iMapID;
};

DECL_PACKET( ERM_LEAVE_FIELD_ACK )
{
	int									m_iOK;
	int									m_iReason;
	int									m_iMapID;
};

DECL_PACKET( ETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_UnitUID;
	VECTOR3								m_vPos;	
	std::vector< KFieldFrameInfo >		m_vecFieldFrame;
	bool								m_bIsRight;	
};
//}}

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( ETR_CHANGE_USER_LIST_IN_SECTOR_NOT )
{
	std::vector< UidType >				m_vecUID;
	std::vector< KFieldUserInfo >		m_vecNewUserList;
    std::vector< UidType >				m_vecDelUserList;
};

//////////////////////////////////////////////////////////////////////////

DECL_PACKET( ERM_OPTION_UPDATE_NOT )
{
	KFieldOption						m_kFieldOption;
};

DECL_PACKET( ERM_UPDATE_PARTY_NOT )
{
	UidType								m_iPartyUID;
	std::set< UidType >					m_setPartyUIDList;
};

DECL_PACKET( ETR_UPDATE_PARTY_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iUnitUID;	
	UidType								m_iPartyUID;
};

//{{ 2009. 1. 16  최육사	채널리스트
DECL_PACKET( DBE_CHANNEL_LIST_REQ )
{
	int									m_iServerGroupID;
};

DECL_PACKET( DBE_CHANNEL_LIST_ACK )
{	
	std::map< int, KChannelInfo >		m_mapChannelList;

	//{{ 2012. 06. 29	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
	std::map< int, KChannelBonusInfo >	m_mapChannelBonusList;
#endif SERV_CHANNEL_LIST_RENEWAL
	//}}
};
//}}

//{{ 2009. 2. 13  최육사	채널 이동
//DECL_PACKET( ELG_UPDATE_USER_MIGRATION_INFO_REQ )
//{
//	UidType								m_iServerUID;
//	UidType								m_iAccountAuthKey;
//    KUserMigrationInfo					m_kUserInfo;
//};
//
//DECL_PACKET( ELG_UPDATE_USER_MIGRATION_INFO_ACK )
//{
//	int									m_iOK;
//	UidType								m_iServerUID;
//	UidType								m_iAccountAuthKey;
//	UidType								m_iUnitUID;
//};
//
//DECL_PACKET( ELG_GET_USER_MIGRATION_INFO_REQ )
//{
//	UidType								m_iServerUID;
//	UidType								m_iAccountAuthKey;
//	UidType								m_iUnitUID;
//};
//
//DECL_PACKET( ELG_GET_USER_MIGRATION_INFO_ACK )
//{
//	int									m_iOK;
//	KUserMigrationInfo					m_kUserInfo;
//};
//}}

//{{ 2009. 4. 1  최육사		친구차단옵션
DECL_PACKET( ELG_UPDATE_DENY_FRIEND_SHIP_NOT )
{
	UidType								m_iUnitUID;
    bool								m_bDenyFriendShip;
};
//}}

//{{ 2009. 4. 7  최육사		닉네임 변경
DECL_PACKET( DBE_DELETE_NICK_NAME_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
};

typedef KPacketOK					KDBE_DELETE_NICK_NAME_ACK;
typedef KDBE_DELETE_NICK_NAME_REQ	KELOG_DELETE_NICK_NAME_NOT;
typedef KDBE_DELETE_NICK_NAME_REQ	KELOG_CHANGE_NICK_NAME_NOT;
//}}

//{{ 2009. 4. 14  최육사	친구리스트
DECL_PACKET( ERM_UPDATE_FRIEND_NOT )
{
	std::set< UidType >					m_setFriendUIDList;
};
//}}

//{{ 2009. 4. 17  최육사	유저찾기
DECL_PACKET( ELG_SEARCH_UNIT_REQ )
{
	unsigned short						m_usEventID;
	std::wstring						m_wstrNickName;
	UidType								m_iUnitUID;

	KELG_SEARCH_UNIT_REQ()
	{
		m_usEventID = 0;
		m_iUnitUID = 0;
	}
};

DECL_PACKET( ELG_SEARCH_UNIT_ACK )
{
	int					m_iOK;
	unsigned short		m_usEventID;
	KConnectionUnitInfo	m_kCUnitInfo;
};
//}}

DECL_PACKET( ERM_OPEN_TC_ROOM_REQ )
{
	KTCInfo			m_kTCInfo;
	KRoomUserInfo	m_kRoomUserInfo;
};

DECL_PACKET( ERM_JOIN_ROOM_REQ )
{
	bool                                m_bQuickJoin;
	std::wstring                        m_wstrPassword;
	KRoomUserInfo                       m_kRoomUserInfo;

	std::vector< UidType >				m_vecStudentUnitUID;	//사제시스템중 제자리스트	
#ifdef SERV_NEW_PVPROOM_PROCESS
	bool								m_bInvitation;
#endif SERV_NEW_PVPROOM_PROCESS
};

//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
//#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
DECL_PACKET( ERM_LEAVE_ROOM_REQ )
{
	int                                 m_iReason;
	int									m_iItemID;
	//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	bool								m_bSendAck;
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	KDungeonPlayResultInfo				m_kResultInfo;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	KGamePlayNetworkInfoPackage			m_kGamePlayNetworkInfo;
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
	//}}
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM

#endif SERV_RELATIONSHIP_SYSTEM
	//}

	KERM_LEAVE_ROOM_REQ()
	{
		m_iReason = 0;
		m_iItemID = 0;
		m_bSendAck = true;
	}
};
//#else
//DECL_PACKET( ERM_LEAVE_ROOM_REQ )
//{
//	int                                 m_iReason;
//};
//#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}


DECL_PACKET( ERM_ROOM_LIST_INFO_NOT )
{
	int                                 m_iCode;
	KRoomInfo                           m_kRoomInfo;
};

// KEGS_STATE_CHANGE_RESULT_REQ -> send id

// KEGS_STATE_CHANGE_RESULT_ACK -> KPacketOK

// KEGS_STATE_CHANGE_RESULT_NOT -> std::vector< KRoomSlotInfo >

//{{ 2007. 10. 23  최육사  
typedef KERM_LEAVE_ROOM_REQ				KERM_LEAVE_TC_ROOM_REQ;
//}}

//{{ 2009. 4. 20  최육사	대전초대
DECL_PACKET( ELG_INVITE_PVP_ROOM_REQ )
{
	std::wstring	m_wstrFromNickName;
	UidType			m_iToUnitUID;
	KRoomInfo		m_kRoomInfo;
};

DECL_PACKET( ELG_INVITE_PVP_ROOM_ACK )
{
	int				m_iOK;
	std::wstring	m_wstrNickName;
};
//}}

//{{ 2009. 4. 22  최육사	대전유저리스트
DECL_PACKET( ECN_NEW_PVP_USER_INFO_NOT )
{
	UidType								m_nGSUID;
    KCommunityUserInfo					m_kUserInfo;
};

DECL_PACKET( ECN_UPDATE_PVP_USER_INFO_NOT )
{
	std::vector< KPVPUserSimpleInfo >	m_vecUserInfo;
};
//}}

//{{ 2009. 5. 8  최육사		실시간 레벨업
DECL_PACKET( ERM_CHAR_LEVEL_UP_NOT )
{
	KStat								m_kBaseStat;
	KRoomUserInfo                       m_kRoomUserInfo;
#ifdef SERV_ELESIS_UPDATE_EVENT
	int									m_iNoteViewCount;
#endif SERV_ELESIS_UPDATE_EVENT
};
//}}

DECL_PACKET( DBE_INSERT_CASH_SKILL_POINT_REQ )
{
	UidType			m_iUnitUID;
	int				m_iCSPoint;
	int				m_iPeriod;
	bool			m_bUpdateEndDateOnly;
	//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
	int				m_iSkillPointItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
	//}}
};


DECL_PACKET( DBE_INSERT_CASH_SKILL_POINT_ACK )
{
	int				m_iOK;
	int				m_iCSPoint;
	int				m_iPeriod;
	std::wstring	m_wstrEndDate;
	bool			m_bUpdateEndDateOnly;
	//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
	int				m_iSkillPointItemID;
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
	//}}
};

#ifdef SERV_SKILL_PAGE_SYSTEM

DECL_PACKET( DBE_EXPIRE_CASH_SKILL_POINT_REQ )
{
	UidType									m_iUnitUID;
	std::vector<KRetrievedSkillPageData>	m_vecRetrievedSkillPageData;

	KDBE_EXPIRE_CASH_SKILL_POINT_REQ()
		: m_iUnitUID(0)
	{
		m_vecRetrievedSkillPageData.clear();
	}
};


DECL_PACKET( DBE_EXPIRE_CASH_SKILL_POINT_ACK )
{
	int								m_iOK;
	std::vector<int>				m_vecRetrievedSPoint;

	KDBE_EXPIRE_CASH_SKILL_POINT_ACK()
		: m_iOK(0)
	{
		m_vecRetrievedSPoint.clear();
	}
};

#else // SERV_SKILL_PAGE_SYSTEM

DECL_PACKET( DBE_EXPIRE_CASH_SKILL_POINT_REQ )
{
	UidType							m_iUnitUID;
	int								m_iRetrievedSPoint;
	std::vector< KUserSkillData >	m_vecUserSkillData;		// DB에 갱신해야할 스킬 정보
};


DECL_PACKET( DBE_EXPIRE_CASH_SKILL_POINT_ACK )
{
	int								m_iOK;
	int								m_iRetrievedSPoint;
};

#endif // SERV_SKILL_PAGE_SYSTEM


//{{ 2009. 8. 4  최육사		봉인 스킬
DECL_PACKET( DBE_UNSEAL_SKILL_REQ )
{
    UidType							m_iUnitUID;
	int								m_iSkillID;	
};

DECL_PACKET( DBE_UNSEAL_SKILL_ACK )
{
    int								m_iOK;
	int								m_iSkillID;
};
//}}

DECL_PACKET( ELOG_INIT_SKILL_TREE_NOT )
{
	UidType							m_iUnitUID;
	int								m_iSPoint;
	int								m_iCSPoint;
	UidType							m_iItemUID;
};



DECL_PACKET( ELOG_INSERT_CASH_SKILL_POINT_NOT )
{
	UidType			m_iUnitUID;
	int				m_iUnitLevel;
	int				m_iMaxCSPoint;
	int				m_iPeriod;
	bool			m_bUpdateEndDateOnly;
};

//{{ 2009. 5. 14  최육사	내구도소모개편
DECL_PACKET( ERM_DECREASE_ENDURANCE_DUNGEON_NOT )
{
	char							m_cPartyNumMember;
	int								m_iEnduranceDamage;

	KERM_DECREASE_ENDURANCE_DUNGEON_NOT()
	{
		m_iEnduranceDamage	= 0;
		m_cPartyNumMember	= 0;
	}
};

DECL_PACKET( ERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT )
{
	UidType								m_UnitUID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	KRoomUserInfo						m_kRoomUserInfo;
};
//}}

//{{ 2009. 5. 23  최육사	시간드롭
DECL_PACKET( DBE_GET_TIME_DROP_INFO_ACK )
{
	std::vector< KTimeDropInfo >		m_vecTimeDropInfo;
};

DECL_PACKET( DBE_CHECK_TIME_DROP_RESULT_REQ )
{
	UidType								m_iGSUID;
	UidType								m_iUserUID;
    UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	int									m_iTimeDropID;
	int									m_iRewardItemID;
	std::wstring						m_wstrRewardItemName;
	int									m_iDropItemUID;
};

DECL_PACKET( DBE_CHECK_TIME_DROP_RESULT_ACK )
{
	int									m_iOK;
	UidType								m_iGSUID;
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	int									m_iTimeDropID;
	int									m_iRewardItemID;
	std::wstring						m_wstrRewardItemName;
	int									m_iDropItemUID;
};

//{{ 2009. 7. 22  최육사	메가폰 로그
DECL_PACKET( ELOG_MEGAPHONE_LOG_NOT )
{
    UidType								m_iUnitUID;
	int									m_iItemID;
};
//}}

//{{ 2009. 7. 10  최육사	사기 시스템
DECL_PACKET( ERM_INCREASE_PARTY_FEVER_NOT )
{
	UidType								m_iPartyUID;
	//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	short								m_sSubStageNpcCount;
	char								m_cRoomType;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KERM_INCREASE_PARTY_FEVER_NOT()
	{
		m_iPartyUID = 0;
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_sSubStageNpcCount = 0;
		m_cRoomType = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};
//}}

//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_PACKET( ERM_MAX_PARTY_FEVER_NOT )
{
	UidType								m_iPartyUID;

	KERM_MAX_PARTY_FEVER_NOT()
	{
		m_iPartyUID = 0;
	}
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 7. 27  최육사	헤니르 클리어 로그
DECL_PACKET( ELOG_HENIR_DUNGEON_CLEAR_NOT )
{
    UidType								m_iUserUID;
	UidType								m_iUnitUID;
	int									m_iPlayTime;
};
//}}

DECL_PACKET( DBE_REQUEST_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
    std::wstring                        m_wstrMessage;
};

DECL_PACKET( DBE_REQUEST_FRIEND_ACK )
{
    int                                 m_iOK;
    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
    std::wstring                        m_wstrMessage;
};

DECL_PACKET( ELG_REQUEST_FRIEND_NOT )
{
    UidType                             m_iDestUnitUID;
    std::wstring                        m_wstrDestNickName;
    UidType                             m_iUnitUID;
    std::wstring                        m_wstrNickName;
    std::wstring                        m_wstrMessage;
};

DECL_PACKET( DBE_ACCEPT_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    UidType                             m_iFriendUnitUID;
};

typedef		        KEGS_ACCEPT_FRIEND_ACK		    KDBE_ACCEPT_FRIEND_ACK;

DECL_PACKET( ELG_ACCEPT_FRIEND_NOT )
{
    UidType                             m_iDestUnitUID;
    UidType                             m_iUnitUID;
};

DECL_PACKET( DBE_DENY_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    UidType                             m_iFriendUnitUID;
};

typedef		        KEGS_DENY_FRIEND_ACK		    KDBE_DENY_FRIEND_ACK;

DECL_PACKET( ELG_DENY_FRIEND_NOT )
{
    UidType                             m_iDestUnitUID;
    UidType                             m_iUnitUID;
};

DECL_PACKET( DBE_BLOCK_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    UidType                             m_iFriendUnitUID;
};

typedef		        KEGS_BLOCK_FRIEND_ACK		    KDBE_BLOCK_FRIEND_ACK;

DECL_PACKET( ELG_BLOCK_FRIEND_NOT )
{
    UidType                             m_iDestUnitUID;
    UidType                             m_iUnitUID;
};

DECL_PACKET( DBE_UNBLOCK_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    UidType                             m_iFriendUnitUID;
};

typedef		        KEGS_UNBLOCK_FRIEND_ACK		    KDBE_UNBLOCK_FRIEND_ACK;

DECL_PACKET( ELG_UNBLOCK_FRIEND_NOT )
{
    UidType                             m_iDestUnitUID;
    UidType                             m_iUnitUID;
};

DECL_PACKET( DBE_DELETE_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    UidType                             m_iFriendUnitUID;
};

typedef		        KEGS_DELETE_FRIEND_ACK		    KDBE_DELETE_FRIEND_ACK;

DECL_PACKET( ELG_DELETE_FRIEND_NOT )
{
    UidType                             m_iDestUnitUID;
    UidType                             m_iUnitUID;
};

DECL_PACKET( DBE_MOVE_FRIEND_REQ )
{
    UidType                             m_iUnitUID;
    UidType                             m_iFriendUnitUID;
    char                                m_cTargetGroupID;
};

typedef		        KEGS_MOVE_FRIEND_ACK		    KDBE_MOVE_FRIEND_ACK;

DECL_PACKET( DBE_MAKE_FRIEND_GROUP_REQ )
{
    UidType                             m_iUnitUID;
    char                                m_cGroupID;
    std::wstring                        m_wstrGroupName;
};

typedef	            KEGS_MAKE_FRIEND_GROUP_ACK      KDBE_MAKE_FRIEND_GROUP_ACK;

DECL_PACKET( DBE_RENAME_FRIEND_GROUP_REQ )
{
    UidType                             m_iUnitUID;
    char                                m_cGroupID;
    std::wstring                        m_wstrGroupName;
};

typedef		        KEGS_RENAME_FRIEND_GROUP_ACK    KDBE_RENAME_FRIEND_GROUP_ACK;

DECL_PACKET( DBE_DELETE_FRIEND_GROUP_REQ )
{
    UidType                             m_iUnitUID;
    char                                m_cGroupID;
};

typedef		        KEGS_DELETE_FRIEND_GROUP_ACK    KDBE_DELETE_FRIEND_GROUP_ACK;

DECL_PACKET( DBE_FRIEND_MESSAGE_NOT )
{
    UidType                             m_iSenderUnitUID;
    std::wstring                        m_wstrReceiverNickName;
    char                                m_cMessageType;
    std::wstring                        m_wstrMessage;
};

DECL_PACKET( ELG_UNIT_DISCONNECT_NOT )
{
    UidType                             m_iUnitUID;
    std::set< UidType >                 m_setFriendUID;
	//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
	int									m_iGuildUID;
#endif GUILD_TEST
	//}}
};

DECL_PACKET( ELG_FRIEND_POSITION_UPDATE_NOT )
{
    UidType                             m_iUnitUID;
    std::set< UidType >                 m_setFriendUID;
    char                                m_cPosition;
};

//{{ 2009. 7. 29  최육사	item set cheat
DECL_PACKET( DBE_ADMIN_GET_ITEM_SET_NOT )
{
	std::wstring						m_wstrNickName;
};
//}}

//{{ 2009. 8. 27  최육사	밀봉
DECL_PACKET( DBE_SEAL_ITEM_REQ )
{
	UidType								m_iUnitUID;
	UidType                             m_iItemUID;
	u_char								m_ucSealResult;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_SEAL_ITEM_ACK )
{
	int									m_iOK;
	u_char								m_ucSealResult;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;    
};

typedef KDBE_SEAL_ITEM_REQ		KDBE_UNSEAL_ITEM_REQ;
typedef KDBE_SEAL_ITEM_ACK		KDBE_UNSEAL_ITEM_ACK;
//}}

//////////////////////////////////////////////////////////////////////////
/* 해외용 패킷쪽으로 위치 이동 시킴
//09. 08. 06 김정협 Gash인증을 위하여 패킷 구조체 생성

#define MAX_PACKET_SIZE_OF_GASH_AUTH 1024

DECL_PACKET( EGASH_AUTHENTICATION_REQ )
{
    UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;
	std::string m_strServicePassword;
	std::string m_strLockFlag;
	std::string m_strIPAddress;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_AUTHENTICATION
	{
		GRPAC_SERVICE_CODE,
		GRPAC_SERVICE_REGION,
		GRPAC_SERVICE_ACCOUNTID,
		GRPAC_SERVICE_ACCOUNTPASSWORD,
		GRPAC_LOCKFLAG,
		GRPAC_IPADDRESS,
		GRPAC_PACKETID,

		GRPAC_NUM,
	};

    void GetGashPacket( std::string& strPacket )
    {
        strPacket.clear();

        strPacket = m_strServiceCode;
        strPacket += "\'";
        strPacket += m_strServiceRegion;
        strPacket += "\'";
        strPacket += m_strServiceAccountID;
        strPacket += "\'";
        strPacket += m_strServicePassword;
        strPacket += "\'";
        strPacket += m_strLockFlag;
        strPacket += "\'";
        strPacket += m_strIPAddress;
        strPacket += "\'";
        strPacket += m_strPacketID;
        strPacket += "\'\r\n";

        return;
    }

};


DECL_PACKET( EGASH_AUTHORIZATION_REQ )
{
    UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;
	std::string m_strServicePassword;
	std::string m_strAuthenticationFlag;
	std::string m_strType;
	std::string m_strDuplicateCheck;
	std::string m_strIPAddress;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_AUTHORIZATION
	{
		GRPAZ_SERVICE_CODE,
		GRPAZ_SERVICE_REGION,
		GRPAZ_SERVICE_ACCOUNTID,
		GRPAZ_SERVICE_ACCOUNTPASSWORD,
		GRPAZ_AUTHENTICATIONFLAG,
		GRPAZ_TYPE,
		GRPAZ_DUPLICATECHECK,
		GRPAZ_IPADDRESS,
		GRPAZ_PACKETID,

		GRPAZ_NUM,
	};

    void GetGashPacket( std::string& strPacket )
    {
        strPacket.clear();

        strPacket = m_strServiceCode;
        strPacket += "\'";
        strPacket += m_strServiceRegion;
        strPacket += "\'";
        strPacket += m_strServiceAccountID;
        strPacket += "\'";
        strPacket += m_strServicePassword;
        strPacket += "\'";
        strPacket += m_strAuthenticationFlag;
        strPacket += "\'";
        strPacket += m_strType;
        strPacket += "\'";
        strPacket += m_strDuplicateCheck;
        strPacket += "\'";
        strPacket += m_strIPAddress;
        strPacket += "\'";
        strPacket += m_strPacketID;
        strPacket += "\'\r\n";

        return;
    }
};


DECL_PACKET( EGASH_ACCOUNTING_REQ )
{
    UidType m_iActorUID;
	std::string m_strServiceCode;
	std::string m_strServiceRegion;
	std::string m_strServiceAccountID;	
	std::string m_strType;
	std::string m_strLoginTime;
	std::string m_strLogoutTime;
	std::string m_strIPAddress;
	std::string m_strPacketID;

	enum GASH_RECEIVE_PACKET_ACCOUNTING
	{
		GRPAT_SERVICE_CODE,
		GRPAT_SERVICE_REGION,
		GRPAT_SERVICE_ACCOUNTID,
		GRPAT_TYPE,		
		GRPAT_LOGINTIME,		
		GRPAT_LOGOUTTIME,
		GRPAT_IPADDRESS,
		GRPAT_PACKETID,

		GRPAT_NUM,
	};

    void GetGashPacket( std::string& strPacket )
    {
        strPacket.clear();

        strPacket = m_strServiceCode;
        strPacket += "\'";
        strPacket += m_strServiceRegion;
        strPacket += "\'";
        strPacket += m_strServiceAccountID;
        strPacket += "\'";
        strPacket += m_strType;
        strPacket += "\'";
        strPacket += m_strLoginTime;
        strPacket += "\'";
        strPacket += m_strLogoutTime;
        strPacket += "\'";
        strPacket += m_strIPAddress;
        strPacket += "\'";
        strPacket += m_strPacketID;
        strPacket += "\'\r\n";

        return;
    }
};

DECL_DATA( KGashResponseData )
{
	std::string m_strResultCode;
	std::string m_strOutString;
	std::string m_strPacketID;
	enum GASH_RESPONSE
	{
		GR_RESULT_CODE,
		GR_OUTSTRING,
		GR_PACKETID,

		GR_NUM,
	};

    bool ExtractFromGASHPacket( std::string& strPacket )
    {
        char cPacket[MAX_PACKET_SIZE_OF_GASH_AUTH];
        
        if( strPacket.size() > MAX_PACKET_SIZE_OF_GASH_AUTH )
        {
            return false;
        }

        strPacket.copy(cPacket, strPacket.size());
        int iEnd = 0;
        for( ; iEnd < ( int )strPacket.size() - 2; iEnd++ )
        {
            if( cPacket[iEnd] == '\r' && cPacket[iEnd + 1] == '\n' )
            {
                break;
            }

            if( cPacket[iEnd] == ';' )
            {
                iEnd++;
                break;
            }

            m_strResultCode.push_back( cPacket[iEnd] );
        }
        m_strResultCode.push_back( '\0' );

        for( ; iEnd < ( int )strPacket.size() - 2; iEnd++ )
        {
            if( cPacket[iEnd] == '\r' && cPacket[iEnd + 1] == '\n' )
            {
                break;
            }

            if( cPacket[iEnd] == ';' )
            {
                iEnd++;
                break;
            }

            m_strOutString.push_back( cPacket[iEnd] );
        }
        m_strOutString.push_back( '\0' );

        for( ; iEnd < ( int )strPacket.size() - 2; iEnd++ )
        {
            if( cPacket[iEnd] == '\r' && cPacket[iEnd + 1] == '\n' )
            {
                break;
            }

            if( cPacket[iEnd] == ';' )
            {
                iEnd++;
                break;
            }

            m_strPacketID.push_back( cPacket[iEnd] );
        }
        m_strPacketID.push_back( '\0' );

        return true;
    }
};

typedef KGashResponseData	KEGASH_AUTHENTICATION_ACK;
typedef	KGashResponseData	KEGASH_AUTHORIZATION_ACK;
typedef	KGashResponseData	KEGASH_ACCOUNTING_ACK;


DECL_PACKET( DBE_GASH_USER_LOGIN_REQ )
{
	std::wstring    m_wstrServiceAccountID;
    std::wstring    m_wstrOTP;
};

DECL_PACKET( DBE_GASH_USER_LOGIN_ACK )
{
    int m_iOK;
    UidType m_iUserUID;
    std::wstring m_wstrOTP;
	
	KDBE_GASH_USER_LOGIN_ACK()
	{
		m_iOK			= 0;
		m_iUserUID		= 0;
	};
};
*/

//{{ 2009. 9. 1  최육사		비정상종료통계
DECL_PACKET( ELOG_ABNORMAL_DISCONNECT )
{
	UidType						m_iServerUID;
	std::map< DWORD, int >		m_mapAbnormalDisconnect;
};
//}}

//{{ 2009. 9. 9  최육사		밀봉
DECL_PACKET( ELOG_USE_SEAL_ITEM_NOT )
{
    UidType						m_iUnitUID;
	UidType						m_iDestItemUID;
	UidType						m_iSealItemUID;
	u_char						m_ucSealCount;
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 9. 17  최육사	길드
#ifdef GUILD_TEST
//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
#else
DECL_PACKET( DBE_CREATE_GUILD_REQ )
{
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	std::wstring						m_wstrGuildName;
	std::wstring						m_wstrGuildMessage;
};

DECL_PACKET( DBE_CREATE_GUILD_ACK )
{
	int									m_iOK;
	UidType								m_iItemUID;
	KGuildInfo							m_kCreatedGuildInfo;
	//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	KGuildSkillInfo						m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	//{{ 2012.02.21 조효진  고유 이름 관련 프로세스 애매한거 정리 
	bool								m_bReqGiant_DeleteName;
	std::wstring						m_wstrGuildName;

	KDBE_CREATE_GUILD_ACK()
	{
		m_bReqGiant_DeleteName	= false;
		m_wstrGuildName			= L"";
	};
	//}}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
};
#endif SERV_GUILD_CHANGE_NAME
//}}
DECL_PACKET( ELG_CREATE_GUILD_RESULT_REQ )
{
	KGuildInfo							m_kGuildInfo;
	std::vector< KGuildMemberInfo >		m_vecMemberList;
	KGuildMemberInfo					m_kLoginGuildMember;
	//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	KGuildSkillInfo						m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}
};

DECL_PACKET( ELG_GET_GUILD_INFO_REQ )
{	
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	KGuildMemberInfo					m_kLoginGuildMember;
};

typedef KELG_GET_GUILD_INFO_REQ		KDBE_GET_GUILD_INFO_REQ;

DECL_PACKET( DBE_GET_GUILD_INFO_ACK )
{
	int									m_iOK;
	KGuildInfo							m_kGuildInfo;
	std::vector< KGuildMemberInfo >		m_vecMemberList;
	//{{ 2009. 11. 19  최육사	길드구조개선	
	KGuildMemberInfo					m_kLoginGuildMember;
	//}}
	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	KGuildSkillInfo						m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}
#ifdef SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	std::map<UidType, KLocalRankingUnitInfo>	m_mapRankerUnitInfo;
	std::map<UidType, UidType>					m_mapRankerUIDInfo;
	std::map<UidType, KLocalRankingUserInfo>	m_mapRankerUserInfo;
#endif // SERV_LOCAL_RANKING_SYSTEM
};

DECL_PACKET( ELG_GET_GUILD_USER_LIST_REQ )
{
	int									m_iGuildUID;
	UINT								m_uiViewPage;
	char								m_cGuildUserListType;
};

DECL_PACKET( ELG_UPDATE_GUILD_MEMBER_INFO_NOT )
{
	int									m_iGuildUID;
	KGuildMemberInfo					m_kMemberInfo;
};

DECL_PACKET( ELG_INVITE_GUILD_REQ )
{
	int									m_iGuildUID;
	UidType								m_iSenderUnitUID;
	std::wstring						m_wstrReceiverNickName;
	UidType								m_iReceiverUnitUID;
};

DECL_PACKET( ELG_INVITE_GUILD_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrReceiverNickName;
};

DECL_PACKET( ELG_INVITE_GUILD_FIND_USER_REQ )
{
	u_short								m_usEventID;
	KGuildInfo							m_kGuildInfo;
	UidType								m_iSenderUnitUID;
	std::wstring						m_wstrSenderNickName;
	UidType								m_iReceiverUnitUID;
	//{{ 2010. 01. 18  최육사	길드게시판
	bool								m_bAutoJoinGuild;
	//}}

	KELG_INVITE_GUILD_FIND_USER_REQ()
	{
		m_usEventID			= 0;
		m_iSenderUnitUID	= 0;
		m_iReceiverUnitUID	= 0;
		m_bAutoJoinGuild	= false;
	}
};

DECL_PACKET( ELG_INVITE_GUILD_FIND_USER_ACK )
{
	int									m_iOK;
	u_short								m_usEventID;
	int									m_iGuildUID;
	UidType								m_iSenderUnitUID;
	UidType								m_iReceiverUnitUID;
	std::wstring						m_wstrReceiverNickName;
};

DECL_PACKET( ELG_INVITE_GUILD_REPLY_REQ )
{
	int									m_iGuildUID;
	std::wstring						m_wstrSenderNickName;
	int									m_iReason;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
};

DECL_PACKET( ELG_INVITE_GUILD_REPLY_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iGuildMasterUnitUID;
};

DECL_PACKET( DBE_JOIN_GUILD_REQ )
{
	int									m_iGuildUID;
	KGuildMemberInfo					m_kJoinGuildMember;
	UidType								m_iGuildMasterUnitUID;
};

DECL_PACKET( DBE_JOIN_GUILD_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	KGuildMemberInfo					m_kJoinGuildMember;
};

DECL_PACKET( ELG_JOIN_GUILD_REQ )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	KGuildMemberInfo					m_kLoginGuildMember;
};

DECL_PACKET( ELG_JOIN_GUILD_ACK )
{
	int									m_iOK;
	KEGS_GUILD_INFO_NOT					m_kGuildInfoNot;
};

DECL_PACKET( DBE_CHANGE_GUILD_MEMBER_GRADE_REQ )
{
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	UidType								m_iTargetUnitUID;
	std::wstring						m_wstrTargetNickName;
	u_char								m_ucMemberShipGrade;
	bool								m_bChangeGuildMaster;
};

DECL_PACKET( DBE_CHANGE_GUILD_MEMBER_GRADE_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iTargetUnitUID;
	std::wstring						m_wstrTargetNickName;
	u_char								m_ucMemberShipGrade;
	bool								m_bChangeGuildMaster;
};

typedef KDBE_CHANGE_GUILD_MEMBER_GRADE_REQ		KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ;
typedef KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK		KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK;


DECL_PACKET( DBE_CHANGE_GUILD_MESSAGE_REQ )
{
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	std::wstring						m_wstrMessage;
};

DECL_PACKET( DBE_CHANGE_GUILD_MESSAGE_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	std::wstring						m_wstrMessage;
};

DECL_PACKET( DBE_KICK_GUILD_MEMBER_REQ )
{
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	UidType								m_iTargetUnitUID;
};

DECL_PACKET( DBE_KICK_GUILD_MEMBER_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	UidType								m_iTargetUnitUID;	
};

DECL_PACKET( ELG_KICK_RESULT_LETTER_NOT )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrGuildName;
};

DECL_PACKET( DBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ )
{
	UidType								m_iUnitUID;
	UidType								m_iTargetUnitUID;
	int									m_iGuildUID;
	std::wstring						m_wstrMessage;
};

DECL_PACKET( DBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	UidType								m_iTargetUnitUID;
	int									m_iGuildUID;
	std::wstring						m_wstrMessage;
};

DECL_PACKET( DBE_DISBAND_GUILD_REQ )
{
	UidType								m_iUnitUID;
	int									m_iGuildUID;
};

DECL_PACKET( DBE_DISBAND_GUILD_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
};

DECL_PACKET( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ )
{
    UidType								m_iUnitUID;
	int									m_iGuildUID;
	UidType								m_iItemUID;
};

DECL_PACKET( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_ACK )
{
    int									m_iOK;
	UidType								m_iItemUID;
};

DECL_PACKET( DBE_EXPAND_GUILD_MAX_MEMBER_REQ )
{
    UidType								m_iUnitUID;
	int									m_iGuildUID;
};

DECL_PACKET( DBE_EXPAND_GUILD_MAX_MEMBER_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	u_short								m_usMaxNumMember;
};

//{{ 2009. 10. 27  최육사	길드레벨
DECL_PACKET( DBE_UPDATE_GUILD_EXP_REQ )
{
    UidType								m_iUnitUID;
	int									m_iGuildUID;
	int									m_iHonorPoint;
	int									m_iGuildEXP;
	int									m_iGuildLimitEXP;
};

DECL_PACKET( DBE_UPDATE_GUILD_EXP_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	int									m_iGuildUID;	
	int									m_iGuildEXP;
};

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
DECL_DATA( KCreateGuildREQ )
{
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	std::wstring						m_wstrGuildName;
	std::wstring						m_wstrGuildMessage;
};

DECL_DATA( KCreateGuildACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	KGuildInfo							m_kCreatedGuildInfo;
	//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	KGuildSkillInfo						m_kGuildSkillInfo;
#endif GUILD_SKILL_TEST
	//}}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
	//{{ 2012.02.21 조효진  고유 이름 관련 프로세스 애매한거 정리 
	bool								m_bReqGiant_DeleteName;
	std::wstring						m_wstrGuildName;

	KCreateGuildACK()
	{
		m_bReqGiant_DeleteName	= false;
		m_wstrGuildName			= L"";
	};
	//}}
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
};

DECL_DATA( KChangeGuildNameCheckREQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrNewGuildName;

	KChangeGuildNameCheckREQ()
	{
		m_iUnitUID = 0;
	}
};

DECL_DATA( KChangeGuildNameREQ )
{
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	std::wstring						m_wstrOldGuildName;
	std::wstring						m_wstrNewGuildName;

	KChangeGuildNameREQ()
	{
		m_iUnitUID	= 0;
		m_iGuildUID	= 0;
	}
};

DECL_DATA( KChangeGuildNameACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	KGuildInfo							m_kGuildInfo;
	std::wstring						m_wstrOldGuildName;

	KChangeGuildNameACK()
	{
		m_iUnitUID = 0;
	}
};

typedef KCreateGuildREQ KELG_CREATE_GUILD_REQ;
typedef KCreateGuildACK KELG_CREATE_GUILD_ACK;
typedef KCreateGuildREQ KDBE_CREATE_GUILD_REQ;
typedef KCreateGuildACK KDBE_CREATE_GUILD_ACK;

typedef KChangeGuildNameCheckREQ KELG_CHANGE_GUILD_NAME_CHECK_REQ;
typedef KChangeGuildNameCheckREQ KDBE_CHANGE_GUILD_NAME_CHECK_REQ;

typedef KPacketOK KELG_CHANGE_GUILD_NAME_CHECK_ACK;

typedef KChangeGuildNameREQ KELG_CHANGE_GUILD_NAME_REQ;
typedef KChangeGuildNameACK KELG_CHANGE_GUILD_NAME_ACK;
typedef KChangeGuildNameREQ KDBE_CHANGE_GUILD_NAME_REQ;
typedef KChangeGuildNameACK KDBE_CHANGE_GUILD_NAME_ACK;

DECL_PACKET( DBE_CHANGE_GUILD_NAME_CHECK_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;

	KDBE_CHANGE_GUILD_NAME_CHECK_ACK()
	{
		m_iUnitUID = 0;
	}
};

DECL_PACKET( ELG_CHANGE_GUILD_NAME_RESULT_REQ )
{
	int									m_iGuildUID;
	std::wstring						m_wstrGuildName;
};

DECL_PACKET( ELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ )
{
	UidType								m_iUnitUID;
	unsigned long                       m_ulOrderNo;
	unsigned long                       m_ulProductNo;
	int									m_iQuantity;
};

DECL_PACKET( ELG_CHANGE_GUILD_NAME_TIME_CHECK_ACK )
{
	int									m_iOK;
	unsigned long                       m_ulOrderNo;
	unsigned long                       m_ulProductNo;
	int									m_iQuantity;
};

#endif SERV_GUILD_CHANGE_NAME
//}}

DECL_PACKET( DBE_GUILD_LEVEL_UP_REQ )
{
    int									m_iGuildUID;
	std::wstring						m_wstrGuildName;
	u_char								m_ucGuildLevel;
	//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	int									m_iGuildSPoint;
#endif GUILD_SKILL_TEST
	//}}
};

DECL_PACKET( DBE_GUILD_LEVEL_UP_ACK )
{
    int									m_iOK;
	int									m_iGuildUID;
	std::wstring						m_wstrGuildName;
	u_char								m_ucGuildLevel;
	//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	int									m_iGuildSPoint;
#endif GUILD_SKILL_TEST
	//}}
};
//}}

#endif GUILD_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
DECL_PACKET( ERM_CHANGE_FIELD_USER_INFO_NOT )
{
	KFieldUserInfo						m_kInfo;
};

DECL_PACKET( ETR_CHANGE_FIELD_USER_INFO_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iUnitUID;
	char								m_cUnitClass;
	//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
	std::wstring						m_wstrGuildName;
#endif GUILD_TEST
	//}}
};
//}}

//{{ 2009. 10. 26  최육사	트래픽어택
DECL_PACKET( ELOG_TRAFFIC_ATTACKER_LOG_NOT )
{
	std::wstring						m_wstrUserID;
	int									m_iChannelID;
};
//}}

//{{ 2009. 10. 27  최육사	랭킹
DECL_PACKET( ELG_FIND_DELETE_CHAR_IN_RANKING_NOT )
{
	UidType								m_iUnitUID;
	std::vector< int >					m_vecRankingType;
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

DECL_PACKET( ELG_GET_GUILD_SKILL_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
    int									m_iGuildSkillID;
};

typedef KELG_GET_GUILD_SKILL_REQ		KELG_RESET_GUILD_SKILL_REQ;

DECL_PACKET( DBE_GET_GUILD_SKILL_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	int									m_iGuildSkillID;
	int									m_iGuildSkillLevel;	
	int									m_iGuildSkillCSPoint;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
	int									m_iMaxGuildCSPoint;
};

DECL_PACKET( DBE_GET_GUILD_SKILL_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	int									m_iGuildSkillID;
	int									m_iGuildSkillLevel;
	int									m_iGuildSkillCSPoint;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
};

DECL_PACKET( DBE_RESET_GUILD_SKILL_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	int									m_iGuildSkillID;
	int									m_iGuildSkillLevel;
	int									m_iGuildSkillCSPoint;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
	int									m_iMaxGuildCSPoint;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_RESET_GUILD_SKILL_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	int									m_iGuildSkillID;
	int									m_iGuildSkillLevel;
	int									m_iGuildSkillCSPoint;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( ELG_INIT_GUILD_SKILL_REQ )
{
    int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
};

DECL_PACKET( ELG_INIT_GUILD_SKILL_NOT )
{
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
};

DECL_PACKET( DBE_INIT_GUILD_SKILL_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_INIT_GUILD_SKILL_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	int									m_iGuildSPoint;
	int									m_iGuildCSPoint;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( ELG_INSERT_GUILD_CASH_SKILL_POINT_NOT )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	int									m_iGuildCSPoint;
	int									m_iPeriod;
};

DECL_PACKET( DBE_INSERT_GUILD_CASH_SKILL_POINT_REQ )
{
	int									m_iGuildUID;
	int									m_iGuildCSPoint;
	int									m_iPeriod;
	bool								m_bUpdateEndDateOnly;
};

DECL_PACKET( DBE_INSERT_GUILD_CASH_SKILL_POINT_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	int									m_iGuildCSPoint;
	int									m_iPeriod;
	std::wstring						m_wstrEndDate;
	bool								m_bUpdateEndDateOnly;
};

DECL_PACKET( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ )
{
	int									m_iGuildUID;
	int									m_iRetrievedSPoint;
	int									m_iGuildSPoint;
	std::vector< KGuildSkillData >		m_vecGuildSkillData;		// DB에 갱신해야할 스킬 정보
};

DECL_PACKET( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	int									m_iRetrievedSPoint;
};

typedef KRoomUserInfo		KERM_CHANGE_SKILL_INFO_IN_ROOM_NOT;

DECL_PACKET( ELG_ADMIN_GET_GUILD_SKILL_POINT_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
    int									m_iGuildSPoint;
};

DECL_PACKET( ELG_ADMIN_GET_GUILD_SKILL_POINT_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iUnitUID;
    int									m_iGuildSPoint;
};

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 12. 8  최육사	크리스마스이벤트
DECL_PACKET( DBE_CHECK_TIME_EVENT_COMPLETE_REQ )
{
	UidType								m_iUnitUID;
	std::vector< KCumulativeTimeEventInfo > m_vecCompletedEvent;
	std::map< int, int >                m_mapInsertedItem;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;
};

DECL_PACKET( DBE_CHECK_TIME_EVENT_COMPLETE_ACK )
{
	int									m_iOK;
	int									m_iRewardCount;
	std::vector< int >					m_vecRestartEvent;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
};
//}}

//{{ 2009. 12. 16  최육사	동접툴
DECL_PACKET( DBE_GET_CONCURRENT_USER_INFO_REQ )
{
	char								m_cCCUType;
	UidType								m_iServerUID;
	std::wstring						m_wstrDate;
};

DECL_PACKET( DBE_GET_CONCURRENT_USER_INFO_ACK )
{
	int									m_iOK;
	char								m_cCCUType;
	UidType								m_iServerUID;
	std::wstring						m_wstrDate;
	std::vector< KCCUInfo >				m_vecCCUInfo;
};
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 01. 04  최육사	길드 광고
#ifdef SERV_GUILD_AD

DECL_PACKET( DBE_GET_GUILD_BOARD_INFO_ACK )
{
	std::vector< KGuildAdInfo >			m_vecGuildAdList;
	std::vector< KApplyJoinGuildInfo >	m_vecApplyJoinGuildList;
#ifdef SERV_GUILD_FIND_AD
	std::vector< KGuildFindAdInfo >		m_vecGuildFindAdList;
	std::vector< KGuildInviteMsgInfo >	m_vecGuildInviteMsgList;
#endif SERV_GUILD_FIND_AD
};

DECL_PACKET( ELG_GET_GUILD_AD_LIST_REQ )
{
    int									m_iGuildUID;
	UidType								m_iUnitUID;
	char								m_cSortType;
	u_int								m_uiViewPage;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( ELG_GET_GUILD_SKILL_IN_BOARD_REQ )
{
    int									m_iGuildUID;
	UidType								m_iUnitUID;
};

DECL_PACKET( DBE_GET_GUILD_SKILL_IN_BOARD_ACK )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	KGuildSkillInfo						m_kGuildSkillInfo;
};

#ifdef SERV_GUILD_FIND_AD
typedef KELG_GET_GUILD_SKILL_IN_BOARD_REQ KELG_GET_GUILD_SKILL_IN_INVITE_GUILD_LIST_REQ;
typedef KDBE_GET_GUILD_SKILL_IN_BOARD_ACK KDBE_GET_GUILD_SKILL_IN_INVITE_GUILD_LIST_ACK;
#endif SERV_GUILD_FIND_AD

//////////////////////////////////////////////////////////////////////////
DECL_PACKET( ELG_REGISTRATION_GUILD_AD_REQ )
{
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	short								m_sPeriod;
	std::wstring						m_wstrAdMessage;
	int									m_iCost;
};

DECL_PACKET( ELG_REGISTRATION_GUILD_AD_ACK )
{
    int									m_iOK;
	short								m_sPeriod;
	std::wstring						m_wstrAdMessage;
	int									m_iCost;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( DBE_REGISTRATION_GUILD_AD_REQ )
{
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	short								m_sPeriod;
	std::wstring						m_wstrAdMessage;
	int									m_iCost;
	bool								m_bExistExpiredAd;
};

DECL_PACKET( DBE_REGISTRATION_GUILD_AD_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	int									m_iGuildUID;
	short								m_sPeriod;
	std::wstring						m_wstrAdRegDate;
	std::wstring						m_wstrAdEndDate;
	std::wstring						m_wstrAdMessage;
	int									m_iCost;
	bool								m_bExistExpiredAd;
};


//////////////////////////////////////////////////////////////////////////
typedef KELG_REGISTRATION_GUILD_AD_REQ		KELG_MODIFY_REG_GUILD_AD_REQ;
typedef KELG_REGISTRATION_GUILD_AD_ACK		KELG_MODIFY_REG_GUILD_AD_ACK;

typedef KDBE_REGISTRATION_GUILD_AD_ACK		KDBE_MODIFY_REG_GUILD_AD_ACK;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( ELG_APPLY_JOIN_GUILD_REQ )
{
	KApplyJoinGuildInfo					m_kApplyJoinGuildInfo;
	bool								m_bOldApplyDelete;
};

DECL_PACKET( DBE_APPLY_JOIN_GUILD_REQ )
{
	KApplyJoinGuildInfo					m_kApplyJoinGuildInfo;
	bool								m_bOldApplyDelete;
	std::vector< KApplyDeleteInfo >		m_vecDeleteApply;
};

DECL_PACKET( DBE_APPLY_JOIN_GUILD_ACK )
{
	int									m_iOK;
    KApplyJoinGuildInfo					m_kApplyJoinGuildInfo;
};

DECL_PACKET( ELG_GET_APPLY_JOIN_GUILD_LIST_REQ )
{
    int									m_iGuildUID;
	UidType								m_iUnitUID;
	u_int								m_uiViewPage;
};

DECL_PACKET( ELG_ACCEPT_JOIN_GUILD_REQ )
{
    int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iApplyJoinUnitUID;
};

DECL_PACKET( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iApplyJoinUnitUID;
};

DECL_PACKET( DBE_ACCEPT_JOIN_GUILD_REQ )
{
	int									m_iGuildUID;
	KGuildMemberInfo					m_kJoinGuildMemberInfo;
};

DECL_PACKET( DBE_ACCEPT_JOIN_GUILD_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	KGuildMemberInfo					m_kJoinGuildMemberInfo;
	std::wstring						m_wstrGuildName;
};

DECL_PACKET( ELG_DELETE_APPLY_JOIN_GUILD_REQ )
{
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iDeletedUnitUID;
	bool								m_bDeleteChar;
};

DECL_PACKET( DBE_DELETE_APPLY_JOIN_GUILD_ACK )
{
	int									m_iOK;
	int									m_iGuildUID;
	UidType								m_iUnitUID;
	UidType								m_iDeletedUnitUID;
	bool								m_bDeleteChar;
};

DECL_PACKET( DBE_DELETE_GUILD_AD_LIST_NOT )
{
	std::vector< int >					m_vecDeleteGuildAd;
	std::vector< KApplyDeleteInfo >		m_vecDeleteApply;
#ifdef SERV_GUILD_FIND_AD
	std::vector< UidType >				m_vecDeleteGuildFindAd;
	std::vector< KInviteDeleteInfo >	m_vecDeleteInvite;
#endif SERV_GUILD_FIND_AD
};

#endif SERV_GUILD_AD
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 01. 11  최육사	추천인리스트
DECL_PACKET( DBE_GET_RECOMMEND_USER_LIST_REQ )
{
    UidType								m_iUnitUID;
	
	KDBE_GET_RECOMMEND_USER_LIST_REQ()
	{
		m_iUnitUID						= 0;
	}
};

DECL_PACKET( DBE_GET_RECOMMEND_USER_LIST_ACK )
{
	int									m_iOK;
	std::vector< KRecommendUserInfo >	m_vecRecommendUserList;
	//{{ 2012. 01. 17  김민성	추천인 리스트 최대 전송 패킷 크기 조절
#ifdef SERV_RECOMMEND_LIST_OVER_SIZE
	u_int								m_uiViewPage;
	u_int								m_uiTotalPage;
#endif SERV_RECOMMEND_LIST_OVER_SIZE
	//}}

	KDBE_GET_RECOMMEND_USER_LIST_ACK()
	{
		m_iOK						= 0;
		//{{ 2012. 01. 17  김민성	추천인 리스트 최대 전송 패킷 크기 조절
#ifdef SERV_RECOMMEND_LIST_OVER_SIZE
		m_uiViewPage				= 1;
		m_uiTotalPage				= 1;
#endif SERV_RECOMMEND_LIST_OVER_SIZE
		//}}
	}
};

DECL_PACKET( ELG_RECOMMEND_USER_INFO_NOT )
{
	UidType									m_iRecommendedUnitUID;
	UidType									m_iRecommendUnitUID;
	char									m_cType;
	int										m_iData;

	KELG_RECOMMEND_USER_INFO_NOT()
	{
		m_iRecommendedUnitUID	= 0;
		m_iRecommendUnitUID		= 0;
		m_cType					= 0;
		m_iData					= 0;
	}
};
//}}


//{{ 2010. 01. 27  최육사	방 정보
#ifdef SERV_ROOM_COUNT

//{{ 2010. 01. 27  최육사	방 정보
DECL_PACKET( ERM_ROOM_COUNT_INFO_NOT )
{
	char								m_cRoomType;
	bool								m_bIsOpen;
};
//}}

#endif SERV_ROOM_COUNT
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST

DECL_PACKET( DBE_CHECK_HACKING_TOOL_LIST_REQ )
{
	int									m_iReleaseTick;
};

DECL_PACKET( DBE_CHECK_HACKING_TOOL_LIST_ACK )
{
	int									m_iReleaseTick;
	std::vector< KHackingToolInfo >		m_vecHackingToolList;
};

#endif SERV_HACKING_TOOL_LIST
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM

DECL_PACKET( ELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	UidType							m_iPartyUID;
	UidType							m_iHostUnitUID;
	std::wstring					m_wstrHostNickName;
	UidType							m_iReceiverUnitUID;
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
	int								m_iServerGroupID;
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
    std::wstring					m_wstrReceiverNickName;
	KCreatePartyInfo				m_kCreatePartyInfo;

	KELG_INVITE_PARTY_FIND_RECEIVER_REQ()
		: m_iPartyUID( 0 )
		, m_iHostUnitUID( 0 )
		, m_iReceiverUnitUID( 0 )
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
		, m_iServerGroupID( -1 )
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
	{
	}
};

DECL_PACKET( ELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	int								m_iOK;
    UidType							m_iHostUnitUID;
	std::vector< KPartyUserInfo >	m_vecReceiverPartyUserInfo;
	KCreatePartyInfo				m_kCreatePartyInfo;

	KELG_INVITE_PARTY_FIND_RECEIVER_ACK()
	{
		m_iOK = 0;
		m_iHostUnitUID = 0;
	}
};

DECL_PACKET( EPM_LEAVE_PARTY_REQ )
{
	UidType								m_iPartyUID;
	int									m_iReason;	
	//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType								m_iNewJoinPartyUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEPM_LEAVE_PARTY_REQ()
	{
		m_iPartyUID = 0;
		m_iReason = 0;		
		//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iNewJoinPartyUID = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};

DECL_PACKET( EPM_LEAVE_PARTY_ACK )
{
	int									m_iOK;
	//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
	int									m_iReason;
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
	//}}
	UidType								m_iPartyUID;	
	//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	UidType								m_iNewJoinPartyUID;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEPM_LEAVE_PARTY_ACK()
	{
		m_iOK = 0;
		//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
		m_iReason = 0;
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
		//}}
		m_iPartyUID = 0;		
		//{{ 2012. 11. 7	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iNewJoinPartyUID = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_INVITE_PARTY_REQ )
{
	UidType								m_iPartyUID;
	KPartyUserInfo						m_kPartyUserInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_INVITE_PARTY_REPLY_NOT )
{
	UidType								m_iPartyUID;
	char								m_cReplyReason;
	KPartyUserInfo						m_kPartyUserInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( ELG_INVITE_PARTY_REPLY_TO_HOST_NOT )
{
	UidType								m_iHostUnitUID;
	char								m_cReplyReason;
	KPartyUserInfo						m_kPartyUserInfo;

	KELG_INVITE_PARTY_REPLY_TO_HOST_NOT()
	{
		m_iHostUnitUID = 0;
		m_cReplyReason = 0;
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_CREATE_PARTY_BY_INVITE_REQ )
{
	char								m_cAuthLevel; // 개발자 로그를 남기기위한 정보
	KCreatePartyInfo					m_kCreatePartyInfo;
	KPartyUserInfo						m_kHostPartyUserInfo;
	KPartyUserInfo						m_kReceiverPartyUserInfo;
};

DECL_PACKET( EPM_CREATE_PARTY_BY_INVITE_ACK )
{
	int									m_iOK;
	KPartyInfo							m_kPartyInfo;
	KPartyUserInfo						m_kHostPartyUserInfo;
	KPartyUserInfo						m_kReceiverPartyUserInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_UPDATE_PARTY_USER_INFO_NOT )
{
	UidType								m_iPartyUID;
	bool								m_bChangeField;
	UidType								m_iUnitUID;
	UCHAR								m_cState;
	int									m_iStateCode;
	char						    	m_cUnitClass;
	UCHAR								m_ucLevel;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_CHANGE_READY_REQ )
{
	UidType								m_iPartyUID;
	bool								m_bReady;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_CHANGE_DUNGEON_REQ )
{
	UidType								m_iPartyUID;
	int									m_iDungeonID;
	char								m_DifficultyLevel;
	char								m_cDungeonMode;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_GAME_START_REQ )
{
	UidType								m_iPartyUID;
	short								m_sWorldID;
	unsigned short						m_usEventID;

	KEPM_PARTY_GAME_START_REQ()
	{
		m_iPartyUID = 0;
		m_sWorldID = 0;
		m_usEventID = 0;
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_GAME_START_ACK )
{
	int									m_iOK;
	unsigned short						m_usEventID;
	//{{ 2012. 05. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int									m_iDungeonMode;
	int									m_iDungeonID;
	UidType								m_iFailUnitUID;
	std::wstring						m_wstrFailUserNickName;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

	KEPM_PARTY_GAME_START_ACK()
	{
		m_iOK = 0;
		m_usEventID = 0;
		//{{ 2012. 05. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_iDungeonMode = 0;
		m_iDungeonID = 0;
		m_iFailUnitUID = 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_CHECK_FOR_PARTY_GAME_START_REQ )
{
	UidType				m_iPartyUID;
	unsigned short		m_usEventID;
	short				m_sWorldID;
	KDungeonGameInfo	m_kDungeonGameInfo;

	KEPM_CHECK_FOR_PARTY_GAME_START_REQ()
		: m_iPartyUID( 0 )
		, m_usEventID( 0 )
		, m_sWorldID( 0 )
	{
	}
};

DECL_PACKET( EPM_CHECK_FOR_PARTY_GAME_START_ACK )
{
	int									m_iOK;
	UidType								m_iPartyUID;
	unsigned short						m_usEventID;
	short								m_sWorldID;
	std::vector< KPartyRoomUserInfo >	m_vecUserInfo;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_END_GAME_NOT )
{
	UidType								m_iPartyUID;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_CHANGE_HOST_REQ )
{
	UidType								m_iPartyUID;
	UidType								m_iNewHostUnitUID;
};


DECL_PACKET( EPM_CHECK_PARTY_CHANGE_HOST_REQ )
{
	UidType								m_iPartyUID;
	//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	char								m_cPartyType;
#endif SERV_PVP_NEW_SYSTEM
	//}}
	UidType								m_iOldHostUnitUID;
	UidType								m_iNewHostUnitUID;
	int									m_iDungeonID;
	char								m_cDifficulty;
	char								m_cDungeonMode;
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	bool								m_bForRegroupParty;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	KEPM_CHECK_PARTY_CHANGE_HOST_REQ()
	{
		m_iPartyUID			= 0;
		//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		m_cPartyType		= 0;
#endif SERV_PVP_NEW_SYSTEM
		//}}
		m_iOldHostUnitUID	= 0;
		m_iNewHostUnitUID	= 0;
		m_iDungeonID		= 0;
		m_cDifficulty		= 0;
		m_cDungeonMode		= 0;
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		m_bForRegroupParty	= false;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}
};

DECL_PACKET( EPM_CHECK_PARTY_CHANGE_HOST_ACK )
{
	int									m_iOK;
	UidType								m_iPartyUID;
	UidType								m_iOldHostUnitUID;
	UidType								m_iNewHostUnitUID;
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	bool								m_bForRegroupParty;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}

	KEPM_CHECK_PARTY_CHANGE_HOST_ACK()
	{
		m_iOK				= 0;
		m_iPartyUID			= 0;
		m_iOldHostUnitUID	= 0;
		m_iNewHostUnitUID	= 0;
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		m_bForRegroupParty	= 0;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_CHANGE_NUM_OF_PER_REQ )
{
	UidType								m_iPartyUID;
	char								m_cNumOfPer;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ )
{
    UidType								m_iPartyUID;
	char								m_cGetItemType;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_CHAT_REQ )
{
	UidType								m_iPartyUID;
	char								m_cChatPacketType;
	char								m_cRoomChatType;
	UidType								m_ToUnitUID;
	std::wstring						m_wstrRecieverUnitNickName;
	std::wstring						m_wstrMsg;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_BAN_USER_REQ )
{
	UidType								m_iPartyUID;
    UidType								m_iUnitUID;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_PARTY_BAN_USER_NOT )
{
	UidType								m_iPartyUID;    
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ )
{
	UidType								m_iPartyUID;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT )
{
	UidType								m_iPartyUID;
};

DECL_PACKET( EPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT )
{
	UidType								m_iPartyUID;
	std::set< UidType >					m_setPartyUIDList;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_UPDATE_PARTY_IN_PARTY_LIST_NOT )
{
	KPartyInfo							m_kPartyInfo;
};

//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_PACKET( EPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT )
{
    UidType								m_iPartyUID;
	short								m_sPartyFever;

	KEPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT()
	{
		m_iPartyUID = 0;
		m_sPartyFever = 0;
	}
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

DECL_PACKET( EPM_OPEN_PARTY_LIST_NOT )
{
	KPartyInfo							m_kPartyInfo;
	std::vector< KPartyUserInfo >		m_vecUserInfo;
};

DECL_PACKET( EPM_CLOSE_PARTY_LIST_NOT )
{
	UidType								m_iPartyUID;
};

typedef KEPM_CLOSE_PARTY_LIST_NOT		KEPM_ADD_PARTY_LIST_NOT;
typedef KEPM_CLOSE_PARTY_LIST_NOT		KEPM_DEL_PARTY_LIST_NOT;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( EPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	UidType								m_iPartyUID;
	std::vector< KPartyUserInfo >		m_vecUpdatePartyUser;
	std::vector< UidType >				m_vecDeletePartyUser;
};

DECL_PACKET( EPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	UidType								m_iPartyUID;
	KPartyUserInfo						m_kPartyUserInfo;
};

DECL_PACKET( EPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT )
{
	UidType								m_iPartyUID;
	UidType								m_iUnitUID;
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

DECL_PACKET( DBE_ATTENDANCE_CHECK_REQ )
{
    UidType								m_iUnitUID;
};

typedef KPacketOK		KDBE_ATTENDANCE_CHECK_ACK;


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( DBE_INCREASE_WEB_POINT_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrUserID;
	char								m_cPointType;
	int									m_iIncreasePoint;
};

DECL_PACKET( DBE_INCREASE_WEB_POINT_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	std::wstring						m_wstrUserID;
	char								m_cPointType;
	int									m_iIncreasePoint;
};


//////////////////////////////////////////////////////////////////////////
DECL_PACKET( DBE_GET_WEB_POINT_REWARD_ACK )
{
	std::vector< KWebPointRewardInfo >	m_vecWebPointReward;
};

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef DEF_TRADE_BOARD
DECL_PACKET( ERM_PERSONAL_SHOP_INFO_NOT )
{
	int			m_iCode;

	UidType			m_iPSUID;
	std::wstring	m_wstrSellerNickName;
	//{{ 2011. 05. 03	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	UidType			m_iHostUnitUID;
#endif SERV_PSHOP_AGENCY
	//}}

	std::vector<KSellPersonalShopItemInfo> m_vecItemInfo;
};
#endif DEF_TRADE_BOARD


//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE

DECL_PACKET( DBE_EXPAND_SKILL_NOTE_PAGE_REQ )
{
	UidType								m_iUnitUID;
    char								m_cExpandedMaxPageNum;
	char								m_cRollBackMaxPageNum;
};

DECL_PACKET( DBE_EXPAND_SKILL_NOTE_PAGE_ACK )
{
    int									m_iOK;
	char								m_cExpandedMaxPageNum;
	char								m_cRollBackMaxPageNum;
};

DECL_PACKET( DBE_REG_SKILL_NOTE_MEMO_REQ )
{
	UidType								m_iUnitUID;
    char								m_cPageNum;
	int									m_iMemoID;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
};

#endif SERV_SKILL_NOTE
//}}


//{{ 2010. 05. 12  최육사	서버 코드 정리
DECL_PACKET( ERM_MY_USER_UNIT_INFO_TO_SERVER_REQ )
{
	KRoomUserInfo						m_kRoomUserInfo;
	KDungeonPlayResultInfo				m_kDungeonUnitInfo;	
};
//}}


//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW

DECL_PACKET( ELG_UPDATE_CCU_INFO_NOT )
{
	KUpdateCCUInfo						m_kCCUInfo;
};

DECL_PACKET( DBE_UPDATE_CCU_INFO_NOT )
{
	std::wstring						m_wstrRegDate;
	std::vector< KUpdateCCUInfo >		m_vecCCUInfo;
};

#endif SERV_CCU_NEW
//}}


//{{ 2010. 06. 16  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT

DECL_PACKET( ESR_SCRIPT_REFRESH_ORDER_NOT )
{
	enum ORDER_TYPE
	{
		OT_NONE = 0,

		//////////////////////////////////////////////////////////////////////////
		// CenterServer
		OT_CN_ITEM_MANAGER,
		OT_CN_QUEST_MANAGER,
		OT_CN_DUNGEON_MANAGER,
		OT_CN_DROP_TABLE,
		OT_CN_GAME_EVENT_SCRIPT_MANAGER,
		OT_CN_GAME_SYS_VAL,
		OT_CN_ATTRIB_NPC_TABLE,
		OT_CN_HACKING_CHECK_MANAGER,
		OT_CN_ABUSER_LOG_MANAGER,
		OT_CN_RESULT_PROCESS,
		//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		OT_CN_PVP_MATCH_MANAGER,
		OT_CN_PVP_MATCH_RESULT_TABLE,
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 08. 24	최육사	시공 개편
#ifdef SERV_NEW_HENIR_TEST
		OT_CN_HENIR_RESULT_TABLE,
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		OT_CN_BATTLE_FIELD_MANAGER,
		OT_CN_PARTY_MANAGER,
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
		OT_CN_SMS_MANAGER,
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2012. 11. 30	박세훈	던전별 자동 파티 매칭 성공 인원수 설정
#ifdef SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		OT_CN_AUTO_PARTY_SCRIPT_MANAGER,
#endif SERV_AUTO_PARTY_MATCHING_SUCCESS_NUM
		//}}
		//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
		OT_CN_BAD_ATTITUDE_TABLE,
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
		//}
		//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		OT_CN_LOG_MANAGER,
#endif SERV_LOG_SYSTEM_NEW
		//}}
		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		OT_CN_DEFENCE_DUNGEON_MANAGER,
		OT_CN_BUFF_MANAGER,
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
		OT_CN_FIELD_BOSS_DATA,
#endif // SERV_BATTLE_FIELD_BOSS

		OT_CN_SENTINEL,

		//////////////////////////////////////////////////////////////////////////
		// LoginServer
		OT_LG_GUILD_MANAGER,
		OT_LG_MORNITORING_MANAGER,
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER				// 지워야 함
		OT_LG_WORLD_MISSION_MANAGER,
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		OT_LG_GAME_SYS_VAL,
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
		OT_LG_TITLE_MANAGER,
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
		//}}

		//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
		OT_LG_SMS_MANAGER,
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		OT_LG_LOG_MANAGER,
#endif SERV_LOG_SYSTEM_NEW
		//}}

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
		OT_LG_SKILL_TREE,
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2
				
		OT_LG_SENTINEL,

		//////////////////////////////////////////////////////////////////////////		
		// GameServer
		OT_GS_ITEM_MANAGER,
		OT_GS_RANDOM_ITEM_MANAGER,
		OT_GS_MANUFACTURE_ITEM_MANAGER,
		OT_GS_REWARD_TABLE,
		OT_GS_QUEST_MANAGER,
		OT_GS_TITLE_MANAGER,
		OT_GS_DUNGEON_MANAGER,
		OT_GS_MAP_DATA,
		OT_GS_GUILD_MANAGER,
		OT_GS_GAME_SYS_VAL,
		OT_GS_RESOLVE_ITEM_MANAGER,
		OT_GS_ENCHANT_ITEM_MANAGER,
		OT_GS_SOCKET_ITEM,
		OT_GS_ATTRIB_ENCHANT_ITEM,
		OT_GS_RECOMMEND_USER_TABLE,
		OT_GS_EXP_TABLE,
		OT_GS_PET_MANAGER,
		OT_GS_MORNITORING_MANAGER,
		OT_GS_ABUSER_LOG_MANAGER,
		OT_GS_GAME_EVENT_SCRIPT_MANAGER,
		//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		OT_GS_PVP_MATCH_MANAGER,
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 10. 12	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
		OT_GS_CONTENT_MANAGER,
#endif SERV_CONTENT_MANAGER
		//}}
		//{{ 2011.10.14     김민성    운영자 기능은 특정 IP 에서만 사용 가능
#ifdef SERV_USE_GM_CHEAT_RESTRICTED_IP
		OT_GS_COMPANY_IP_TABLE,
#endif SERV_USE_GM_CHEAT_RESTRICTED_IP
		//}}
		//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
		OT_GS_CLASS_CHANGE_TABLE,
#endif SERV_UNIT_CLASS_CHANGE_ITEM
		//}}

		//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
		OT_GS_STRING_FILTER,
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
		//}}
		//{{ 2011. 12. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		OT_GS_BATTLE_FIELD_MANAGER,
		OT_GS_PARTY_MANAGER,
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}

		//{{ 2012. 06. 06	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		OT_GS_DAILY_GIFT_BOX_MANAGER,
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}

		//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
		OT_GS_SMS_MANAGER,
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2013. 1. 28	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
		OT_GS_BINGO_EVENT_MANAGER,
#endif SERV_EVENT_BINGO
		//}}

#ifdef SERV_SYNTHESIS_AVATAR
		OT_GS_SYNTHESIS_TABLE,
#endif SERV_SYNTHESIS_AVATAR

		//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		OT_GS_LOG_MANAGER,
#endif SERV_LOG_SYSTEM_NEW
		//}}
		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		OT_GS_DEFENCE_DUNGEON_MANAGER,
		OT_GS_BUFF_MANAGER,
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		OT_GS_RIDING_PET_MANAGER,
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2// 작업날짜: 2013-08-12	// 박세훈
		OT_GS_SKILL_TREE,
#endif // SERV_REALTIME_SCRIPT_NEWSKILLTEMPLETVER2

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
		OT_GS_FIELD_BOSS_DATA,
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD // 2013-11-11 우상혁
        OT_GS_ED_INVENTORY_EXPAND,
#endif // SERV_GOOD_ELSWORD
		OT_GS_SENTINEL,

		//////////////////////////////////////////////////////////////////////////
		// Global Server
		OT_GB_MORNITORING_MANAGER,
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		// GlobalServer
		OT_GB_WORLD_MISSION_MANAGER,
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		//{{ 2011. 07. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		OT_GB_PVP_MATCH_MANAGER,
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
		OT_GB_QUEST_MANAGER,
#endif SERV_RANDOM_DAY_QUEST
		//}}
		//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
		OT_GB_SMS_MANAGER,
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2012. 11. 9	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		OT_GB_AUTO_PARTY_SCRIPT_MANAGER,
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		OT_GB_LOG_MANAGER,
#endif SERV_LOG_SYSTEM_NEW
		//}}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-11	// 박세훈
		OT_GB_BATTLE_FIELD_MANAGER,
		OT_GB_FIELD_BOSS_DATA,
#endif // SERV_BATTLE_FIELD_BOSS
		
		OT_GB_SENTINEL,

		//////////////////////////////////////////////////////////////////////////
		// ChannelServer
		//{{ 2012. 10. 8	박세훈	SMS 전화번호 통합 관리
#ifdef SERV_SMS_TOTAL_MANAGER
		OT_CH_SMS_MANAGER,
#endif SERV_SMS_TOTAL_MANAGER
		//}}
		//{{ 2013. 02. 19	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
		OT_CH_LOG_MANAGER,
#endif SERV_LOG_SYSTEM_NEW
		//}}

		OT_CH_SENTINEL,


	};

	int									m_iOrderType;

	KESR_SCRIPT_REFRESH_ORDER_NOT()
	{
		m_iOrderType = OT_NONE;
	}
};

#endif SERV_REALTIME_SCRIPT
//}}


//{{ 2010. 06. 20  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD

DECL_PACKET( E_STATISTICS_INFO_NOT )
{
	std::vector< KStatisticsInfo >		m_vecStatisticsInfo;

	void AddStatisticsInfo( int	iStatisticsID, const KStatisticsKey& kKey, int iIndex, int iCount )
	{
		KStatisticsInfo kInfo;
		kInfo.m_iStatisticsID = iStatisticsID;
		kInfo.m_kKey = kKey;
		kInfo.m_iIndex = iIndex;
		kInfo.m_iCount = iCount;
        m_vecStatisticsInfo.push_back( kInfo );
	}
};

DECL_PACKET( E_HENIR_RANKING_LOG_NOT )
{
    int									m_iRankingType;
};

DECL_PACKET( E_LOCAL_LOG_PVP_NOT )
{
    int									m_iPVPChannelClass;
	UidType								m_iOwnerUserUID;
	std::wstring						m_wstrIP;
	UidType								m_nUnitUID;
	std::wstring						m_wstrNickName;
	int									m_iUnitClass;
	UidType								m_iRoomUID;
	int									m_iPlayMode;
	bool								m_bIsItemMode;
	bool								m_bPublic;
	int									m_iNumMember;
	float								m_fPlayTimeLimit;
	std::wstring						m_wstrEndGameTime;
	int									m_iPlayTime;
	int									m_iVSPoint;
	int									m_iEXP;
	int									m_iNumMDKill;
	int									m_iNumDie;
	int									m_iResult;
};

DECL_PACKET( E_LOCAL_LOG_PVP_ROOM_NOT )
{
	int									m_iPlayMode;
	bool								m_bIsItemMode;
	short								m_sWorldID;
	int									m_iNumMember;
	int									m_iPlayTime;
	std::wstring						m_wstrEndGameTime;
};

DECL_PACKET( E_LOCAL_LOG_DUNGEON_ROOM_NOT )
{
	int									m_iDungeonID;
	int									m_iDifficultyLevel;
	int									m_iIsChallenge;
	int									m_iWin;
	int									m_iLose;
	int									m_iStartNumMember;
	int									m_iEndNumMember;
	int									m_iTotalPlayTime;
	std::wstring						m_wstrEndGameTime;
};

DECL_PACKET( E_LOCAL_LOG_TITLE_NOT )
{
	int									m_iTitleID;
	std::wstring						m_wstrCharName;
	u_char								m_ucLevel;
};

DECL_PACKET( E_LOCAL_LOG_DUNGEON_NOT )
{
	//{{ 2010. 12. 6	최육사	던전 로그
	enum DUNGEON_CLEAR_TYPE
	{
		DCT_FAILED = 0,					// [0] : 실패
		DCT_SUCCESS,					// [1] : 클리어		
		DCT_UNIT_DIE_LEAVE_ROOM,		// [2] : 죽고 이탈
		DCT_UNIT_ALIVE_LEAVE_ROOM,		// [3] : 안죽고 이탈
		DCT_CLIENT_CRASH_LEAVE_ROOM,	// [4] : 크래쉬로 인한 이탈
		DCT_CLIENT_HACKING_LEAVE_ROOM,	// [5] : 해킹으로 인한 이탈
	};
	//}}

	int									m_iDungeonID;				// 던전 ID
	UidType								m_nUnitUID;					// 캐릭터UID
	std::wstring						m_wstrNickName;				// 캐릭터 닉네임
	int									m_iDifficultyLevel;			// 던전 난이도
	int									m_iIsChallenge;				// 헤니르 시공 도전모드 여부
	int									m_iUnitClass;				// 캐릭터 직업
	int									m_iClear;					// 던전 종료시 상태 ( DUNGEON_CLEAR_TYPE )
	int									m_iStartNumMember;			// 던전 시작할 당시 파티 멤버 수
	int									m_iEndNumMember;			// 던전 종료될때 파티 멤버 수
	u_char								m_ucLevel;					// 캐릭터 레벨
	int									m_iPlayTime;				// 플레이 시간
	int									m_iEXP;						// 획득 경험치
	int									m_iED;						// 획득 ED
	char								m_cComboRank;				// 콤보 랭크
	int									m_iComboScore;				// 콤보 스코어
	char								m_cTechnicalRank;			// 테크니컬 랭크
	int									m_iTechnicalScore;			// 테크니컬 스코어
	char								m_cTimeRank;				// 시간 랭크
	char								m_cDamagedRank;				// 대미지 랭크
	int									m_iDamageNum;				// 대미지 카운트
	char								m_cTotalRank;				// 전체 랭크
	int									m_iRessurectionStoneCount;	// 부활석 사용 횟수
	int									m_iPassedStageCount;		// 진행한 스테이지 수
	int									m_iPassedSubStageCount;		// 진행한 서브 스테이지 수
	//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	bool								m_bIsWithPet;				// 펫 소환 여부
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 11. 8	최육사	던전 로그 DB
#ifdef SERV_DUNGEON_LOG_DB
	std::wstring						m_wstrRegDate;				// 기록 날짜
#endif SERV_DUNGEON_LOG_DB
	//}}
	//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
	short								m_sNpcDieCount;				// 몬스터 죽인 횟수
	__int64								m_iGivenDamage;				// 몬스터에게 준 대미지
	__int64								m_iAttackDamage;			// 몬스터로부터 받은 대미지
	short								m_sUsingSkillKind;			// 사용한 스킬 종류 수
	int									m_iTotalEXP;				// 전체 획득 경험치
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
	int									m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	char								m_cPetID;					// 소환한 펫 ID
#endif //SERV_PETID_DATA_TYPE_CHANGE
	char								m_cPetEvoStep;				// 소환한 펫의 진화 단계
#endif SERV_ADD_DUNGEON_LOG_COLUMN
	//}}
	//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
	int									m_iDashCount;				// 대쉬 사용 횟수
#endif SERV_DUNGEON_DASH_LOG
	//}}
	//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	int									m_iBaseHP;					// 기본HP
    int									m_iEndHP;					// 게임종료시 HP
	int									m_iRecovHP;					// 회복한 HP
    int									m_iContinue;				// 부활석 사용 갯수
	int									m_iStartContinue;			// 던전 시작할 당시 소유한 부활석 갯수
    int									m_iContinueUseStage;		// 첫 부활석 사용 스테이지
	int									m_iContinueUseSub;			// 첫 부활석 사용 서브 스테이지
    int									m_iFrame;					// 던전 한판동안 평균 프레임
	std::wstring						m_wstrChannelIP;			// 해당 유저가 접속한 채널IP
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
	//}}
	//{{ 2012. 07. 11 김민성
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
	UidType								m_iUserUID;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
	//}}
	//{{ 2012. 11. 6	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	char								m_cAutoPartyPlay;			// 자동 파티 플레이 여부
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	int									m_iAutoPartyWaitTime;		// 자동 매칭까지 대기 시간
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
	//}
	//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	int									m_iLeaveReason;				// 방을 이탈하게된 자세한 사유
	char								m_cRoomState;				// 현재 방 상태
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
	//}}
	//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
	bool								m_bSkillSlotType;			// true = A , flase = B 
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
	//}}

	KE_LOCAL_LOG_DUNGEON_NOT()
	{
		m_iDungeonID				= 0;
		m_nUnitUID					= 0;
		m_iDifficultyLevel			= 0;
		m_iIsChallenge				= 0;
		m_iUnitClass				= 0;
		m_iClear					= 0;
		m_iStartNumMember			= 0;
		m_iEndNumMember				= 0;
		m_ucLevel					= 0;
		m_iPlayTime					= 0;
		m_iEXP						= 0;
		m_iED						= 0;
		m_cComboRank				= 0;
		m_iComboScore				= 0;
		m_cTechnicalRank			= 0;
		m_iTechnicalScore			= 0;
		m_cTimeRank					= 0;
		m_cDamagedRank				= 0;
		m_iDamageNum				= 0;
		m_cTotalRank				= 0;
		m_iRessurectionStoneCount	= 0;
		m_iPassedStageCount			= 0;
		m_iPassedSubStageCount		= 0;
		//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		m_bIsWithPet				= false;
#endif SERV_PET_SYSTEM
		//}}		
		//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
		m_sNpcDieCount				= 0;
		m_iGivenDamage				= 0;
		m_iAttackDamage				= 0;
		m_sUsingSkillKind			= 0;
		m_iTotalEXP					= 0;
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		m_iPetID					= 0;
#else //SERV_PETID_DATA_TYPE_CHANGE
		m_cPetID					= 0;
#endif //SERV_PETID_DATA_TYPE_CHANGE
		m_cPetEvoStep				= 0;
#endif SERV_ADD_DUNGEON_LOG_COLUMN
		//}}
		//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
		m_iDashCount				= 0;
#endif SERV_DUNGEON_DASH_LOG
		//}}
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		m_iBaseHP					= 0;
		m_iEndHP					= 0;
		m_iRecovHP					= 0;
		m_iContinue					= 0;
		m_iStartContinue			= 0;
		m_iContinueUseStage			= 0;
		m_iContinueUseSub			= 0;
		m_iFrame					= 0;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 07. 11 김민성
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
		m_iUserUID					= 0;
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
		//}}
		//{{ 2012. 11. 6	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		m_cAutoPartyPlay			= 0;
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		m_iAutoPartyWaitTime		= 0;
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
		//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		m_iLeaveReason				= 0;
		m_cRoomState				= 0;
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		//}}
		//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
		m_bSkillSlotType			= true;			// true = A , flase = B 
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
		//}}
	}
};

DECL_PACKET( E_LOCAL_LOG_HENIR_RANKING_NOT )
{
	int									m_iRankingType;
	std::vector< KHenirRankingInfo >	m_vecRankingInfo;
};

//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

DECL_PACKET( E_LOCAL_LOG_PET_SUMMON_NOT )
{
	std::wstring						m_wstrNickName;	
	u_char								m_ucLevel;
	UidType								m_iPetUID;
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
	int									m_iPetID;
#else //SERV_PETID_DATA_TYPE_CHANGE
	char								m_cPetID;
#endif //SERV_PETID_DATA_TYPE_CHANGE
	char								m_cEvolutionStep;
	int									m_iIntimacy;
	short								m_sExtroversion;
	short								m_sEmotion;
	std::wstring						m_wstrRegDate;
};

#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG

DECL_PACKET( E_LOCAL_LOG_SERVER_DISCONNECT_NOT )
{
	enum SERVER_DISCONNECT_LOG_TYPE
	{
		SDLT_CONNECT = 0,
		SDLT_DISCONNECT,
	};

    char								m_cLogType;
	std::wstring						m_wstrSourceName;
	std::wstring						m_wstrDestinationName;
	std::wstring						m_wstrDestinationIP;
	std::wstring						m_wstrReason;
	std::wstring						m_wstrRegDate;
};

#endif SERV_SERVER_DISCONNECT_LOG
//}}

#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
DECL_PACKET( E_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT )
{
	int	m_iQuestID;
	std::wstring m_wstrRegDate;
};
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG

DECL_PACKET( E_LOCAL_LOG_BILLING_PACKET_NOT )
{
	enum BILLING_PACKET_LOG_TYPE
	{
		BPLT_SEND = 0,
		BPLT_RECV,
	};

	char								m_cLogType;
	UidType								m_iUserUID;
	unsigned long						m_ulOrderNo;
	std::wstring						m_wstrRegDate;
	u_short								m_usEventID;
};

#endif SERV_BILLING_PACKET_LOG
//}}

#endif SERV_STATISTICS_THREAD
//}}


//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK

DECL_PACKET( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ )
{
	std::map< int, int >				m_mapReleaseTick;
};

DECL_PACKET( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK )
{
	std::map< int, int >					m_mapReleaseTick;
	std::map< int, std::set< UidType > >	m_mapAccountBlockList;
	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
	std::vector< std::string >				m_vecMachineIDBlockList;
#endif SERV_MACHINE_ID_BLOCK
	//}}
};

#endif SERV_ACCOUNT_BLOCK
//}}

//{{2010. 06. 30  최육사	근성도 통계
#ifdef SERV_SPIRIT_STATISTICS

DECL_PACKET( DBE_SPIRIT_STATISTICS_NOT )
{
	UidType								m_iUnitUID;
	//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
	int									m_iSpiritPlayTime;
#else
	int									m_iDungeonPlayTime;
#endif SERV_FIELD_PLAY_TIME_LOG
	//}}
	int									m_iDurationTime;
	int									m_iUsedSpirit;
};

#endif SERV_SPIRIT_STATISTICS
//}}


//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

DECL_PACKET( DBE_CREATE_PET_REQ )
{
	UidType								m_iUnitUID;
	KPetInfo							m_kPetInfo;
#ifdef SERV_PERIOD_PET
	short								m_sPeriod;
#endif SERV_PERIOD_PET
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_CREATE_PET_ACK )
{
	int									m_iOK;
	KPetInfo							m_kPetInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_SUMMON_PET_REQ )
{
	UidType								m_iUnitUID;
	UidType								m_iBeforeSummonPetUID;
    UidType								m_iSummonPetUID;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	KItemEnduranceUpdate                m_kItemEnduranceUpdate;
	KItemPositionUpdate                 m_kItemPositionUpdate;
};

DECL_PACKET( DBE_SUMMON_PET_ACK )
{
	int									m_iOK;
	UidType								m_iBeforeSummonPetUID;
	UidType								m_iSummonPetUID;
	KItemQuantityUpdate					m_kItemQuantityUpdate;
	KItemEnduranceUpdate                m_kItemEnduranceUpdate;
	KItemPositionUpdate                 m_kItemPositionUpdate;
};

DECL_PACKET( ERM_SUMMON_PET_NOT )
{
    UidType								m_iUnitUID;
	std::vector< KPetInfo >				m_vecPetInfo;
};

DECL_PACKET( ETR_SUMMON_PET_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iUnitUID;
	std::vector< KPetInfo >				m_vecPetInfo;
};

DECL_PACKET( ERM_PET_ACTION_NOT )
{
	UidType								m_iUnitUID;
	char								m_cActionType;
};

DECL_PACKET( ETR_PET_ACTION_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iUnitUID;
	char								m_cActionType;
};

DECL_PACKET( ERM_DECREASE_PET_SATIETY_NOT )
{
	char								m_cTeamNumMember;
};

DECL_PACKET( ERM_PET_EVOLUTION_NOT )
{
	UidType								m_iUnitUID;
	KPetInfo							m_kEvolutionPetInfo;
};

DECL_PACKET( ETR_PET_EVOLUTION_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iUnitUID;
	KPetInfo							m_kEvolutionPetInfo;
};

//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
DECL_PACKET( DBE_CHANGE_PET_NAME_REQ )
{
	UidType								m_iItemUID;
	UidType								m_iUnitUID;
	KPetInfo							m_kPetInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_CHANGE_PET_NAME_ACK )
{
	UidType								m_iItemUID;
	int									m_iOK;
	KPetInfo							m_kPetInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};
#endif SERV_PET_CHANGE_NAME
//}}
#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

DECL_PACKET( DBE_RESET_PERIOD_ITEM_REQ )
{
	UidType								m_iUnitUID;
	std::map< UidType, short >			m_mapExpandPeriodItemList;
};

DECL_PACKET( DBE_RESET_PERIOD_ITEM_ACK )
{
	int									m_iOK;
	std::vector< KItemPeriodInfo >		m_vecItemPeriodInfo;
};

#endif SERV_RESET_PERIOD_EVENT
//}}

DECL_PACKET( DBE_CCU_AGENT_ACK )
{
	int			m_iOK;
	int			m_iDailyUniqueUser;
	int			m_iDailyResisterredUser;
};

//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING

DECL_PACKET( ELOG_INSERT_TRAFFIC_ABUSER_LOG_NOT )
{
	UidType								m_iUnitUID;
	std::map< unsigned short, int >		m_mapTrafficEventCount;
};

#endif SERV_TRAFFIC_USER_MONITORING
//}}

//{{ 2010. 9. 29	최육사	ED 모니터링 치트
#ifdef SERV_ADMIN_SHOW_ED

DECL_PACKET( ELG_ADMIN_SHOW_ED_CHEAT_REQ )
{
	UidType								m_iSenderUserUID;
};

DECL_PACKET( ELG_ADMIN_SHOW_ED_CHEAT_ACK )
{
	int									m_iOK;
	UidType								m_iSenderUserUID;
	std::wstring						m_wstrNickName;
	int									m_iED;
};

#endif SERV_ADMIN_SHOW_ED
//}}


//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
DECL_PACKET( ELG_REG_REJECTED_USER_NOT )
{
	UidType								m_iUserUID;
	char								m_cRejectedReason;
#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
	char								m_cPeriodUAL;		// 기간제 블럭 종류
	std::wstring						m_wstrBlockReason;	// 기간제 블럭 사유
#endif // SERV_PERIOD_ACCOUNT_BLOCK

	KELG_REG_REJECTED_USER_NOT()
		: m_iUserUID( 0 )
		, m_cRejectedReason( 0 )
#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
		, m_cPeriodUAL( 0 )
#endif // SERV_PERIOD_ACCOUNT_BLOCK
	{
	}
};
#endif SERV_CN_SERVER_HACKING_CHECK
//}}

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
DECL_PACKET( E_DISCONNECT_SERVER_REPORT_NOT )
{
    char								m_cType;
};
#endif SERV_MORNITORING
//}}

//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER

DECL_PACKET( DBE_GET_MODULE_INFO_LIST_ACK )
{
	std::vector< KModuleInfo >			m_vecModuleList;

	//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
	std::map< int, int >				m_mapReleaseTick;
#endif SERV_HACKING_DLL_CHECK
	//}}
};

#endif SERV_DLL_MANAGER
//}}

//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG

DECL_PACKET( DBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT )
{
	UidType								m_iUserUID;
	std::wstring						m_wstrUserID;
	std::wstring						m_wstrLoginIP;
};

#endif SERV_IP_ACCOUNT_LOG
//}}

//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM

DECL_PACKET( E_LOCAL_LOG_ABUSER_MORNITORING_NOT )
{
	std::vector< KAbuserEventLog >		m_vecAbuserEventLog;
};

#endif SERV_AUTO_HACK_CHECK_GET_ITEM
//}}

//{{ 2010. 11. 18	최육사	아이템 판매 차단
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM

DECL_PACKET( ERM_CHECK_SELL_ED_ITEM_ACK )
{
	int									m_iOK;
	UidType                             m_iItemUID;	
	int									m_iQuantity;
};

#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
//}}


//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT

DECL_PACKET( DBE_GET_DUNGEON_EVENT_INFO_ACK )
{
	std::vector< KDungeonDropEventNpcInfo >		m_vecEventInfo;
};

DECL_PACKET( DBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT )
{
	int								m_iEventUID;
	UidType							m_iKillUnitUID;
	__int64							m_tNpcDieDate;

	KDBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT()
	{
		m_iEventUID		= 0;
		m_iKillUnitUID	= 0;
		m_tNpcDieDate	= 0;
	}
};

#endif SERV_TIME_DROP_MONSTER_EVENT
//}}

//{{ 2010. 12. 7	최육사	메일 전송 시스템
#ifdef SERV_MAIL_SYSTEM

DECL_PACKET( EMAIL_SEND_EMAIL_NOT )
{
	std::vector< std::wstring >		m_vecRecvMailAddrList;
	std::wstring					m_wstrTitle;
	std::wstring					m_wstrDesc;
	std::vector< std::wstring >		m_vecAttachFileNameList;
	std::vector< std::wstring >		m_vecAttachFileFullPathList;
};

#endif SERV_MAIL_SYSTEM
//}}


//{{ 2010. 12. 8	최육사	적정 레벨 장비 통계
#ifdef SERV_APPROPRIATE_LEVEL_STAT

DECL_PACKET( ELOG_APPROPRIATE_LEVEL_STATISTICS_NOT )
{
	__int64										m_tDungeonStartTime;
	int											m_iDungeonID;
	int											m_cDifficultyLevel;
	std::vector< KUserAppropriateLevelInfo >	m_vecUserAppropriateLevel;    
};

#endif SERV_APPROPRIATE_LEVEL_STAT
//}}


//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT

DECL_PACKET( DBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK )
{
	std::vector< KTimeOpenRandomItemEventInfo >	m_vecTimeOpenRandomItemInfo;
	std::set< UidType >							m_setGetRewardUserUIDList;
};

DECL_PACKET( ELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ )
{
    UidType										m_iItemUID;
	UidType										m_iUserUID;
	UidType										m_iUnitUID;
	//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
	int											m_iRandomItemID;
#endif SERV_RANDOM_CUBE_GOLD_REWARD
	//}}
};

DECL_PACKET( ELG_CHECK_TIME_RANDOM_ITEM_EVENT_ACK )
{
	UidType										m_iItemUID;
	int											m_iEventItemID;

	KELG_CHECK_TIME_RANDOM_ITEM_EVENT_ACK()
	{
		m_iItemUID = 0;
		m_iEventItemID = 0;
	}
};

DECL_PACKET( DBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT )
{
    KTimeOpenRandomItemEventDBUPdate			m_kDBUpdateInfo;
};

#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}


//{{ 2011. 01. 04	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM

DECL_PACKET( DBE_ATTRIB_ATTACH_ITEM_REQ )
{
	UidType								m_iUnitUID;
	UidType                             m_iItemUID;
	KItemAttributeEnchantInfo			m_kAttribEnchantInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_ATTRIB_ATTACH_ITEM_ACK )
{
	int									m_iOK;
	UidType                             m_iItemUID;
	KItemAttributeEnchantInfo			m_kAttribEnchantInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;    
};

DECL_PACKET( ELOG_ATTRIB_ATTACH_ITEM_LOG_NOT )
{
	UidType								m_iUnitUID;
	int									m_iAttachItemID;
	UidType								m_iDestItemUID;
	KItemAttributeEnchantInfo			m_kBeforeAttribEnchantInfo;
	KItemAttributeEnchantInfo			m_kAfterAttribEnchantInfo;
};

#endif SERV_ATTRIBUTE_CHARM
//}}

//{{ 2011. 01. 13 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
DECL_PACKET( DBE_GET_CONCURRENT_USER_LOGIN_REQ )
{
	std::wstring						m_wstrID;
	std::wstring						m_wstrPW;
};

DECL_PACKET( DBE_GET_CONCURRENT_USER_LOGIN_ACK )
{
	int									m_iOK;
	int									m_iAuthLevel;
};

DECL_PACKET( DBE_GET_GAME_SERVER_LIST_ACK )
{
	int									m_iServerCount;
	std::vector< KToolServerList >		m_vecServerList;
};
#endif SERV_CCU_MONITORING_TOOL
//}}

//{{ 2011. 01. 13	최육사	캐릭터 로그
#ifdef SERV_CHAR_LOG

DECL_PACKET( ELOG_USER_CHARACTER_LOG_NOT )
{
	enum LOG_TYPE
	{
		LT_NONE = 0,
		LT_CREATE_CHARACTER,		// [1]  캐릭터 생성
		LT_FIRST_QUEST_COMPLETE,	// [2]  첫번째 퀘스트 완료				- 퀘스트 완료 컨테이너를 뒤져서 첫 번째 완료인지 체크하자!
		LT_FIRST_DUNGEON_PLAY,		// [3]  첫 던전 플레이					- 던전 클리어 정보를 뒤져서 첫 번째 완료인지 체크하자! ???? 이거 클리어 체크임?
		LT_DUNGEON_PLAY_COMPLETE,	// [4]  모든 던전별 난이도별 첫 클리어	- 던전 클리어 정보를 뒤져서 첫 번째 완료인지 체크하자!
		LT_FIRST_PVP,				// [5]  첫 대전 시작					- 누적 VP를 체크하여
		LT_FIRST_USE_SKILL_POINT,	// [6]  첫 스킬포인트 사용				- 처음 사용한건지 아닌지 체크가 어려울듯..
		LT_FIRST_DESELECT,			// [7]  첫번째 선택해제
		LT_SECOND_SELECT,			// [8]  두번째 선택
		LT_SECOND_DESELECT,			// [9] 두번째 선택해제
		LT_THIRD_SELECT,			// [10] 세번째 선택
		LT_FIRST_JOB_CHANGE,		// [11] 1차 전직
		LT_SECOND_JOB_CHANGE,		// [12] 2차 전직
		LT_FIRST_USE_CASH,			// [13] 첫 캐시 사용
		LT_LEVEL_UP,				// [14] 캐릭터 레벨업
		LT_FULL_LEVEL,				// [15] 만렙 달성
		LT_DELETE_CHARACTER,		// [16] 캐릭터 삭제
		LT_JOIN_GUILD_SELECT_CHAR,	// [17] 길드 가입한 상태에서 첫번째 캐릭터 선택
		//{{ 2011. 02. 09	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG
		LT_FIRST_BUY_CASH_ITEM,		// [18] 해당 캐릭터의 최초 캐쉬템 구매
#endif SERV_BUY_CASH_ITEM_LOG
		//}}
		//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
		LT_DAILY_CHAR_FIRST_SELECT,	// [19] 매일 캐릭터 첫 접속 로그
#endif SERV_DAILY_CHAR_FIRST_SELECT
		//}}

		LT_MAX,
	};

	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	bool								m_bOnlyOneUnit;
	UidType								m_iServerUID;
	short								m_sLogType;
	u_char								m_ucLevel;
	char								m_cUnitClass;
	int									m_iDungeonID;
	int									m_iProductNo;
	int									m_iTotalDungeonPlayTime;
	int									m_iTotalPvpPlayTime;
	int									m_iDurationTime;
	std::wstring						m_wstrAccountCreateDate;
	std::wstring						m_wstrUnitCreateDate;
	std::wstring						m_wstrRegDate;

	KELOG_USER_CHARACTER_LOG_NOT()
	{
		m_iUserUID				= 0;
		m_iUnitUID				= 0;
		m_bOnlyOneUnit			= false;
		m_iServerUID			= 0;
		m_sLogType				= LT_NONE;
		m_ucLevel				= 0;
		m_cUnitClass			= 0;
		m_iDungeonID			= 0;
		m_iProductNo			= 0;
		m_iTotalDungeonPlayTime = 0;
		m_iTotalPvpPlayTime		= 0;
		m_iDurationTime			= 0;
	}

	static bool IsValidLogType( LOG_TYPE eLogType )
	{
		if( eLogType < LT_NONE  ||  eLogType > LT_MAX )
		{
			return false;
		}

		return true;
	}
};

#endif SERV_CHAR_LOG
//}}

//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK

DECL_PACKET( DBE_REG_DUNGEON_CLEAR_TIME_HACK_USER_NOT )
{
    UidType								m_iUserUID;
	std::wstring						m_wstrRegDate;
};

#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
//}}

//{{ 2011. 02. 08	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG

DECL_PACKET( ELOG_BUY_CASH_ITEM_LOG_NOT )
{
	enum CASH_ITEM_BUY_TYPE
	{
		CIBT_BUY = 0,
		CIBT_PRESENT,
	};

    UidType								m_iUserUID;
	int									m_iServerGroupID;
	UidType								m_iUnitUID;
	char								m_cUnitClass;
	u_char								m_ucLevel;
	bool								m_bIsFirstBuy;
	char								m_cBuyType;
	int									m_iProductNo;
	int									m_iBuyQuantity;
	int									m_iTotalSalePrice;
	std::wstring						m_wstrRegDate;
	//{{ 2011. 10. 11    김민성    캐쉬 아이템 선물 할 때 받는 유저의 정보 DB에 기록하기
#ifdef SERV_PRESENT_CASH_ITEM_ADD_LOG
	std::wstring						m_wstrReceiverUserID;
#endif SERV_PRESENT_CASH_ITEM_ADD_LOG
	//}} 

	KELOG_BUY_CASH_ITEM_LOG_NOT()
	{
		m_iUserUID			= 0;
		m_iServerGroupID	= 0;
		m_iUnitUID			= 0;
		m_cUnitClass		= 0;
		m_ucLevel			= 0;
		m_bIsFirstBuy		= 0;
		m_cBuyType			= 0;
		m_iProductNo		= 0;
		m_iBuyQuantity		= 0;
		m_iTotalSalePrice	= 0;
	}
};

#endif SERV_BUY_CASH_ITEM_LOG
//}}


//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
DECL_PACKET( ELOG_CASH_PRODUCT_LIST_UPDATE_NOT )
{
	std::vector< KCashPruductInfo >		m_vecCashProductList;
};
#endif SERV_CASH_ITEM_LIST
//}}


//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK


DECL_PACKET( ELG_TRADE_BLOCK_NOT )
{
	bool							m_bIsTradeBlock;
};

#endif SERV_HACK_USER_TRADE_BLOCK
//}}

//{{ 2011. 03. 17	김민성	던전 플레이 로그 요청사항 
#ifdef SERV_DUNGEON_TEMP_LOG_DB
DECL_PACKET( DBE_DB_TEMP_LOG_DUNGEON_NOT )
{
	int						m_iDungeonID;
	UidType					m_iUnitUID;
	std::wstring			m_wstrNickName;
	UidType					m_iUserUID;
	int						m_iDifficulty;
	bool					m_bIsChallenge;
	int						m_iUnitClass;
	bool					m_bIsClear;
	int						m_iStartMember;
	int						m_iEndMember;
	u_char					m_ucLv;
	int						m_iPlayTime;
	char					m_cComboRank;
	int						m_iComboScore;
	char					m_cTechnicalRank;
	int						m_iTechnScore;
	char					m_cTimeRank;
	char					m_cDamageRank;
	char					m_cTotalRank;
	int						m_iPassStage;
	int						m_iPassSub;
	int						m_iNormKill;
	int						m_iLoEliteKill;
	int						m_iHiEliteKill;
	int						m_iMidBossKill;
	int						m_iBossKill;
	int						m_iAttkedDamage;
	int						m_iRecovHP;
	int						m_iBaseHP;
	int						m_iGivenDamage;
	int						m_iPartyDamage;
	int						m_iSkillUsedCount;
	std::wstring			m_wstrRegDate;
	bool					m_bComeBackParty;

};
#endif SERV_DUNGEON_TEMP_LOG_DB
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 04. 12	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY

DECL_PACKET( ERM_LOAD_PSHOP_BOARD_INFO_NOT )
{
	UidType										m_iGSUID;

	KERM_LOAD_PSHOP_BOARD_INFO_NOT()
	{
		m_iGSUID = 0;
	}
};

DECL_PACKET( ERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT )
{
	UidType										m_iUserUID;
	UidType										m_iUnitUID;
	std::wstring								m_wstrNickName;
	char										m_cPersonalShopType;
	std::wstring								m_wstrAgencyExpirationDate;
	std::wstring								m_wstrAgencyOpenDate;
	std::wstring								m_wstrPersonalShopName;
	std::vector< KSellPShopItemBackupData >		m_vecSellItemInfo;
	bool										m_bOnSale;

	KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT()
	{
		m_iUserUID = 0;
		m_iUnitUID = 0;
		m_cPersonalShopType = 0;
		m_bOnSale = false;
	}
};

DECL_PACKET( DBE_LOAD_PSHOP_AGENCY_ACK )
{
	std::vector< KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT >	m_vecOpenPShopAgency;
};

DECL_PACKET( DBE_INSERT_PERIOD_PSHOP_AGENCY_REQ )
{
	UidType					m_iUnitUID;
    short					m_sAgencyPeriod;
	u_short					m_usEventID;
    char                    m_cShopType;
};

DECL_PACKET( DBE_INSERT_PERIOD_PSHOP_AGENCY_ACK )
{
	int						m_iOK;	
	std::wstring			m_wstrAgencyExpirationDate;
	u_short					m_usEventID;
    char                    m_cShopType;
};

DECL_PACKET( ERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT )
{
	std::wstring			m_wstrAgencyExpirationDate;
};

DECL_PACKET( ERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ )
{
    UidType					m_iUserUID;
    UidType					m_iUnitUID;
    std::wstring			m_wstrNickName;
    char					m_cPersonalShopType;
    std::wstring			m_wstrAgencyExpirationDate;
    std::wstring			m_wstrAgencyOpenDate;
    std::wstring			m_wstrPersonalShopName;
    std::vector< KSellPShopItemBackupData >		m_vecSellItemInfo;
    bool										m_bOnSale;


	KERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ()
	{
		m_iUnitUID = 0;
        m_vecSellItemInfo.clear();
	}
};

DECL_PACKET( ERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK )
{
    UidType					m_iPShopAgencyUID;

	KERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK()
	{
		m_iPShopAgencyUID = 0;
	}
};

DECL_PACKET( ERM_OPEN_PSHOP_AGENCY_REQ )
{	
	UidType				m_iUserUID;	
	int					m_iED;
	std::wstring		m_wstrNickName;
	char				m_cPersonalShopType;
	std::wstring		m_wstrAgencyExpirationDate;

	KERM_OPEN_PSHOP_AGENCY_REQ()
	{
		m_iUserUID = 0;
		m_iED = 0;
		m_cPersonalShopType = 0;
	}
};

typedef KPacketOK		KERM_CHECK_OPEN_PSHOP_AGENCY_ACK;

DECL_PACKET( DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ )
{
	UidType										m_iUnitUID;
	KItemQuantityUpdate							m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >			m_vecUpdatedInventorySlot;
	std::wstring								m_wstrPersonalShopName;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
};

DECL_PACKET( DBE_PREPARE_REG_PSHOP_AGENCY_ITEM_ACK )
{
	int											m_iOK;
	KItemQuantityUpdate							m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >			m_vecUpdatedInventorySlot;
	std::wstring								m_wstrPersonalShopName;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
};

DECL_PACKET( DBE_STOP_SALE_PSHOP_AGENCY_REQ )
{
    UidType					m_iGSUID;
	UidType					m_iUnitUID;
	int						m_iReason;
	
	KDBE_STOP_SALE_PSHOP_AGENCY_REQ()
	{
		m_iGSUID = 0;
		m_iUnitUID = 0;
		m_iReason = 0;
	}
};

DECL_PACKET( DBE_STOP_SALE_PSHOP_AGENCY_ACK )
{
	int						m_iOK;
	UidType					m_iGSUID;
	UidType					m_iUnitUID;
	int						m_iReason;

	KDBE_STOP_SALE_PSHOP_AGENCY_ACK()
	{
		m_iOK = 0;
		m_iGSUID = 0;
		m_iUnitUID = 0;
		m_iReason = 0;
	}
};

DECL_PACKET( ERM_BREAK_PSHOP_AGENCY_NOT )
{
	int				m_iReason;
	UidType			m_iHostGSUID;
	UidType			m_iHostUID;

	KERM_BREAK_PSHOP_AGENCY_NOT()
	{
		m_iReason = 0;
		m_iHostGSUID = 0;
		m_iHostUID = 0;
	}
};

DECL_PACKET( DBE_BUY_PSHOP_AGENCY_ITEM_REQ )
{
	int									m_iOK;

	// 판매자 정보
	UidType								m_iHostUnitUID;
	int									m_iSellUnitEDIn;

	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	KInventoryItemInfo					m_kSellItemDBUpdate;
#else
	KSellPShopItemDBUpdateInfo			m_kSellItemDBUpdate;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}

	// 구매자 정보
	UidType								m_iPriceGSUID;
	UidType								m_iPriceUnitUID;
	int									m_iPriceUnitEDOUT;
	int									m_iPriceCommissionED;
	KInventoryItemInfo					m_kPriceIntemInfoIN;

	// GSUser객체 소멸시 직접 DB업데이트용 정보
	KItemQuantityUpdate                 m_kItemQuantityUpdateTemp;
	std::vector< KItemInfo >            m_vecItemInfoTemp;
	int									m_iEDTemp;

	KDBE_BUY_PSHOP_AGENCY_ITEM_REQ()
	{
		m_iOK				 = 0;
		m_iHostUnitUID		 = 0;
		m_iSellUnitEDIn		 = 0;
		m_iPriceGSUID		 = 0;
		m_iPriceUnitUID		 = 0;
		m_iPriceUnitEDOUT	 = 0;
		m_iPriceCommissionED = 0;
		m_iEDTemp			 = 0;
	}
};

DECL_PACKET( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	std::vector< UidType >				m_vecPickUpItemList;
	std::map< int, int >				m_mapPickUpItemInfo;
	int									m_iTotalAddED;

	KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ()
	{
		m_iTotalAddED = 0;
	}
};

DECL_PACKET( ERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
    char                    m_cShopType;
	std::vector< UidType >				m_vecPickUpItemList;
};


DECL_PACKET( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	UidType								m_iGSUID;
	UidType								m_iUnitUID;
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	std::vector< KSellPersonalShopItemInfo >	m_vecPickUpItemList;
	int									m_iSellItemTotalED;

	KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ()
	{
		m_iSellItemTotalED = 0;
	}
#else
	std::vector< UidType >				m_vecPickUpItemList;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
};

DECL_PACKET( DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	int									m_iOK;
	UidType								m_iGSUID;
	UidType								m_iUnitUID;
	//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	std::vector< KSellPersonalShopItemInfo >	m_vecPickUpItemList;
	int									m_iSellItemTotalED;
#else
	std::vector< UidType >				m_vecPickUpItemList;
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
	//}}
	std::vector< UidType >				m_vecPickUpFailList;
};

DECL_PACKET( ERM_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
    int											m_iOK;
	int											m_iTotalSellEDIn;
	std::vector< KSellPersonalShopItemInfo >	m_vecPickUpSuccessItemList;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
    bool                                        m_bRemainSellItem;
};

DECL_PACKET( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	UidType										m_iUnitUID;
	std::map< int, int >						m_mapInsertItem;
	KItemQuantityUpdate							m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >			m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >					m_vecItemInfo;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
    bool                                        m_bRemainSellItem;

};

DECL_PACKET( DBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK )
{
	int											m_iOK;
	std::map< int, int >						m_mapInsertItem;
	KItemQuantityUpdate							m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >			m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >				m_mapItemInfo;
	std::vector< KSellPersonalShopItemInfo >	m_vecSellItemInfo;
    bool                                        m_bRemainSellItem;

};

#endif SERV_PSHOP_AGENCY
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
DECL_PACKET( EGB_GET_MISSION_INFO_REQ )
{
	UidType										m_iServerUID;
};

DECL_PACKET( EGB_GET_MISSION_INFO_ACK )
{
	enum FLAG_TYPE
	{
		FT_NONE = 0,
		FT_INIT,
		FT_UPDATE,
		FT_BEGIN,

		FT_SUCCESS,
		FT_FAIL,
	};

	bool										m_bActive;
	int											m_iFlag;
	int											m_iWorldCrystalCount;
	//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	int											m_iWorldMaxCrystalCount;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}
	int											m_iMissionTime;
	int											m_iDungeonPlayTime;

	//{{ 2012. 04. 24	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
	std::map< int, std::vector< KDefenseOpenTime > >	m_mapDefenseOpenTime;	// 요일별 오픈 시간
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
	//}}
	//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
	u_short										m_usWorldBuffDurationTime;
#endif SERV_NEW_DEFENCE_DUNGEON
	//}}

	KEGB_GET_MISSION_INFO_ACK()
		: m_bActive( false )
		, m_iFlag( 0 )
		, m_iWorldCrystalCount( 0 )
		//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		, m_iWorldMaxCrystalCount( 0 )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
		, m_iMissionTime( 0 )
		, m_iDungeonPlayTime( 0 )
		//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		, m_usWorldBuffDurationTime( 0 )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
	{
	}
};

typedef KEGB_GET_MISSION_INFO_ACK		KEGB_UPDATE_MISSION_INFO_NOT;

typedef int								KEGB_CLEAR_MISSION_DUNGEON_NOT;

DECL_PACKET( EGB_UPDATE_WORLD_BUFF_NOT )
{
	bool										m_bActive;
	u_short										m_usSuccessWorldBuff;
	u_short										m_usWorldBuffDurationTime;

	KEGB_UPDATE_WORLD_BUFF_NOT()
		: m_bActive( false )
		, m_usSuccessWorldBuff( 0 )
		, m_usWorldBuffDurationTime( 0 )
	{
	}
};

DECL_PACKET( DBE_INCREASE_DEFENCE_CRYSTAL_REQ )
{
	std::wstring								m_wsrtStartTime;
	int											m_iProtectedCrystalCount;
	int											m_iTargetCrystal;
};

typedef KDBE_INCREASE_DEFENCE_CRYSTAL_REQ		KDBE_GET_DEFENCE_CRYSTAL_REQ;
typedef KDBE_INCREASE_DEFENCE_CRYSTAL_REQ		KDBE_GET_DEFENCE_CRYSTAL_ACK;

DECL_PACKET( EGB_UPDATE_MISSION_START_TIME_NOT )
{
	std::wstring								m_wsrtStartTime;
};

#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY

DECL_PACKET( DBE_GET_SECOND_SECURITY_INFO_REQ )
{
	UidType					m_iUserUID;
	std::map< int, int >	m_mapComeBackRewardCondition; // [주의] 여기에는 싱글톤 객체에 담겨진 정보를 넣는 변수임. 용량 커지면 안됨!
	//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	std::wstring			m_wstrID;
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}
};

DECL_PACKET( DBE_GET_SECOND_SECURITY_INFO_ACK )
{
	int						m_iOK;
	bool					m_bUseSecondPW;
	std::wstring			m_wstrSecondPW;
	int						m_iFailedCount;
	std::wstring			m_wstrLastAuthDate;
	
	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	bool					m_bIsFirst;						// 처음 확인하는 것인가?
	bool					m_bIsComeBackUser;
	int						m_iRewardStep;					// 복귀 유저 몇 단계
	std::wstring			m_wstrLastConnectDate;
	std::wstring			m_wstrComeBackBuffEndDate;		// 복귀 버프 종료 시간
#endif SERV_COME_BACK_USER_REWARD
	//}}
	//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	int						m_iBF_Team;					// 선택한 특공대 종류
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//}}

	//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
	std::multimap<int /*iItemID*/, KDailyGiftBoxInfo>	m_mmapDailyGiftBoxList;
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	std::wstring			m_wstrWinterVacationEventRegDate;
	int						m_iWinterVacationEventCount;
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}
	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	std::vector<KFirstSelectUnitReward>			m_vecFirstSelectUnitReward;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}

	KDBE_GET_SECOND_SECURITY_INFO_ACK()
		: m_iOK( 0 )
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		, m_iWinterVacationEventCount( -1 )
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
	{
	}
};

typedef KDBE_GET_SECOND_SECURITY_INFO_REQ KDBE_GET_PURE_SECOND_SECURITY_INFO_REQ;
typedef KDBE_GET_SECOND_SECURITY_INFO_ACK KDBE_GET_PURE_SECOND_SECURITY_INFO_ACK;

DECL_PACKET( DBE_CREATE_SECOND_SECURITY_REQ )
{
	UidType                 m_iUserID;
	std::wstring			m_wstrSecondPW;
};

DECL_PACKET( DBE_CREATE_SECOND_SECURITY_ACK )
{
	int		                m_iOK;
	std::wstring			m_wstrSecondPW;
};

DECL_PACKET( DBE_SEUCCESS_SECOND_SECURITY_REQ )
{
	UidType                 m_iUserID;
	std::wstring			m_wstrCurrTime;
};

DECL_PACKET( DBE_SEUCCESS_SECOND_SECURITY_ACK )
{
	int						m_iOK;
	std::wstring			m_wstrCurrTime;
};

typedef KDBE_CREATE_SECOND_SECURITY_REQ			KDBE_DELETE_SECOND_SECURITY_REQ;

typedef KPacketOK			KDBE_DELETE_SECOND_SECURITY_ACK;

DECL_PACKET( DBE_CHANGE_SECOND_SECURITY_PW_REQ )
{
	UidType                 m_iUserID;
	std::wstring			m_wstrOldSecondPW;
	std::wstring			m_wstrNewSecondPW;
};

typedef KDBE_CREATE_SECOND_SECURITY_ACK			KDBE_CHANGE_SECOND_SECURITY_PW_ACK;

DECL_PACKET( DBE_UPDATE_SECURITY_TYPE_NOT )
{
	UidType                 m_iUserUID;
	int						m_iSecurityType;
};

DECL_PACKET( DBE_FIRST_WRITE_SECURITY_TYPE_NOT )
{
	UidType                 m_iUserUID;
	int						m_iAge;
	int						m_iSecurityType;
	std::wstring			m_wstrCurrTime;
};

#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
DECL_PACKET( ERM_CHECK_ZOMBIE_USER_REQ )
{
	UidType					m_iRoomUID;

	KERM_CHECK_ZOMBIE_USER_REQ()
	{
		m_iRoomUID = 0;
	}
};

DECL_PACKET( ERM_CHECK_ZOMBIE_USER_ACK )
{
	enum ZOMBIE_CHECK_TYPE
	{
		ZCT_CONNECT_NORMAL = 0,
		ZCT_CONNECT_ZOMBIE,
		ZCT_JOIN_DIF_ROOM,
		ZCT_DISCONNECT_ZOMBIE,
	};

	char					m_cZombieCheckType;
	
	KERM_CHECK_ZOMBIE_USER_ACK()
	{
		m_cZombieCheckType = ZCT_CONNECT_NORMAL;
	}
};
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}


//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
DECL_PACKET( EPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ )
{
	UidType                 m_iPartyUID;
	bool					m_bCheck;
};

typedef KPacketOK		  KEPM_PARTY_CHANGE_CHECK_MIN_LEVEL_ACK;
typedef	bool			  KEPM_PARTY_CHANGE_CHECK_MIN_LEVEL_NOT;

#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
DECL_PACKET( DBE_WRITE_COME_BACK_REWARD_NOT )
{
	UidType					m_iUserUID;
	int						m_iRewardStep;					// 복귀 유저 몇 단계
	std::wstring			m_wstrComeBackBuffEndDate;
};

DECL_PACKET( EPM_CHANGE_COME_BACK_BUFF_NOT )
{
	bool					m_bIsBuffFinish;
	std::wstring			m_wstrComeBackBuffEnd;
};

DECL_PACKET( DBE_FIRST_WRITE_COME_BACK_INFO_NOT )
{
	UidType					m_iUserUID;
	int						m_iAge;
	UidType					m_iUnitUID;
	int						m_iUnitClass;
	int						m_iUnitLevel;
	bool					m_bTodayFirst;
	int						m_iRewardLevel;
	std::wstring			m_wsrtRegDate;
};

typedef UidType				KDBE_WRITE_COME_BACK_END_NOT;

DECL_PACKET( DBE_WRITE_COME_BACK_LOGOUT_NOT )
{
	UidType					m_iUserUID;
	std::wstring			m_wsrtLogOutDate;
};
#endif SERV_COME_BACK_USER_REWARD
//}} 

//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
DECL_PACKET( ELOG_DUNGEON_STAGE_LOG_NOT )
{
	int						m_iDungeonID;
	int						m_iStageID;
	UidType					m_iUnitUID;
	char					m_cLevel;
	char					m_cDifficulty;
	bool					m_bIsChallenge;
	char					m_cUnitClass;
	int						m_iStagePlayTime;
	int						m_iStageDamage;
	int						m_iResurrectionCount;
	std::wstring			m_wstrRegDate;

	KELOG_DUNGEON_STAGE_LOG_NOT()
	{
		m_iDungeonID		 = 0;
		m_iStageID			 = 0;
		m_iUnitUID			 = 0;
		m_cLevel			 = 0;
		m_cDifficulty		 = 0;
		m_bIsChallenge		 = false;
		m_cUnitClass		 = 0;
		m_iStagePlayTime	 = 0;
		m_iStageDamage		 = 0;
		m_iResurrectionCount = 0;
	}
};
#endif SERV_DUNGEON_STAGE_LOG
//}}

//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM

DECL_PACKET( EGB_REQUEST_MATCH_MAKING_REQ )
{
	bool							m_bRegFrontOfList;
	char							m_cPvpPlayerCount;
	char							m_cPvpGameType;
	std::vector< KMatchUserInfo >	m_vecMatchUserInfo;

	KEGB_REQUEST_MATCH_MAKING_REQ()
	{
		m_bRegFrontOfList = false;
		m_cPvpPlayerCount = 0;
		m_cPvpGameType = 0;
	}
};

DECL_PACKET( EGB_CANCEL_MATCH_MAKING_REQ )
{
    UidType							m_iMatchUID;
	UidType							m_iPartyUID;
	int								m_iReason;
	bool							m_bSendBySystem;
	
	KEGB_CANCEL_MATCH_MAKING_REQ()
	{
		m_iMatchUID = 0;
		m_iPartyUID = 0;
		m_iReason = 0;
		m_bSendBySystem = false;
	}
};

DECL_PACKET( EGB_REMATCH_MAKING_USER_NOT )
{
	char							m_cPvpPlayerCount;
	char							m_cPvpGameType;

	KEGB_REMATCH_MAKING_USER_NOT()
	{
		m_cPvpPlayerCount	= 0;
		m_cPvpGameType		= 0;
	}
};

DECL_PACKET( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ )
{
	bool							m_bRegFrontOfList;
	char							m_cPvpPlayerCount;
	char							m_cPvpGameType;

	KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ()
	{
		m_bRegFrontOfList	= false;
		m_cPvpPlayerCount	= 0;
		m_cPvpGameType		= 0;
	}
};

DECL_PACKET( EPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK )
{
    int								m_iOK;
	bool							m_bRegFrontOfList;
	char							m_cPvpPlayerCount;
	char							m_cPvpGameType;
	std::vector< KMatchUserInfo >	m_vecMatchUserInfo;

	KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK()
	{
		m_iOK = 0;
		m_bRegFrontOfList = false;
		m_cPvpPlayerCount = 0;
		m_cPvpGameType = 0;
	}
};

DECL_PACKET( EGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK )
{
	KRoomUserInfo					m_kRoomUserInfo;
};

DECL_PACKET( EGB_OPEN_PVP_ROOM_FOR_MATCH_REQ )
{
	UidType								m_iMatchUID;
	char								m_cMatchType;
	char								m_cPvpNpcType;
	KRoomInfo							m_kRoomInfo;
	std::vector< KRoomUserInfo >		m_vecRedTeam;
	std::vector< KRoomUserInfo >		m_vecBlueTeam;
	std::map< UidType, int >			m_mapMatchWaitTime;
	std::map< UidType, KPvpNpcInfo >	m_mapPvpNpcInfo;

	KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ()
	{
		m_cMatchType	= 0;
		m_cPvpNpcType	= 0;
		m_iMatchUID		= 0;
	}
};

typedef KPacketOK		KEGB_OPEN_PVP_ROOM_FOR_MATCH_ACK;

DECL_PACKET( ERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT )
{
    KPvpPlayResultInfo		m_kPvpUnitInfo;
};

DECL_PACKET( DBE_PVP_MATCH_RESULT_LOG_NOT )
{
	char					m_cMatchType;
	char					m_cNpcPvpType;
	std::wstring			m_wstrRegDate;
	int						m_iPlayTime;
	std::vector< KPvpMatchUserResultLog > m_vecPvpMatchUserLog;

	KDBE_PVP_MATCH_RESULT_LOG_NOT()
	{
		m_cMatchType	= 0;
		m_cNpcPvpType	= 0;
		m_iPlayTime		= 0;
	}
};

DECL_PACKET( EGB_MATCH_MAKING_ADMIN_COMMAND_NOT )
{
	enum COMMAND_TYPE
	{
		CT_DUMP = 0,
		CT_CLEAR,
	};

	char					m_cType;
	
	KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT()
	{
		m_cType = CT_DUMP;        
	}
};

#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 08. 03	최육사	대전 강제 종료에 대한 예외처리
#ifdef SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
DECL_PACKET( DBE_QUIT_USER_PVP_RESULT_UPDATE_NOT )
{
    UidType					m_iUnitUID;
	int						m_iOfficialMatchCnt;
	int						m_iRating;
	int						m_iMaxRating;
	int						m_iRPoint;
	int						m_iAPoint;
	bool					m_bIsWinBeforeMatch;
	int						m_cEmblemEnum;

	//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	double					m_fKFactor;
#endif SERV_2012_PVP_SEASON2
	//}}

	int						m_iWin;
	int						m_iLose;
	
	KDBE_QUIT_USER_PVP_RESULT_UPDATE_NOT()
	{
		m_iUnitUID			= 0;
		m_iOfficialMatchCnt	= 0;
		m_iRating			= 0;
		m_iMaxRating		= 0;
		m_iRPoint			= 0;
		m_iAPoint			= 0;
		m_bIsWinBeforeMatch	= false;
		m_cEmblemEnum		= 0;

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		m_fKFactor			= 0.0;
#endif SERV_2012_PVP_SEASON2
		//}}

		m_iWin				= 0;
		m_iLose				= 0;
	}
};
#endif SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
DECL_PACKET( DBE_RETAINING_SELECT_REWARD_REQ )
{
	UidType                             m_iUnitUID;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;	
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
};

DECL_PACKET( DBE_RETAINING_SELECT_REWARD_ACK )
{
	int                                 m_iOK;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
};

DECL_PACKET( DBE_INSERT_RETAINING_USER_REQ )
{
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
	int									m_iItemID;
	std::wstring						m_wstrRegDate;
};

DECL_PACKET( DBE_CHECK_RETAINING_USER_REQ )
{
	UidType								m_iUserUID;
	UidType								m_iUnitUID;
};

DECL_PACKET( DBE_CHECK_RETAINING_USER_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	std::wstring						m_wstrDate;
};

typedef KDBE_INSERT_RETAINING_USER_REQ		KDBE_INSERT_LOG_RETAINING_USER_NOT;
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
DECL_PACKET( DBE_GAME_CREATE_UNIT_REQ )
{
	std::wstring            m_wstrNickName;
	int                     m_iClass;
	std::wstring            m_wstrRegDate;
};
#endif SERV_NEW_CREATE_CHAR_EVENT
//}}

//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
DECL_PACKET( SOAP_GET_TOONILAND_ID_REQ )
{
	std::wstring            m_wstrNexonID;
};

DECL_PACKET( SOAP_GET_TOONILAND_ID_ACK )
{
	int			            m_iOK;
	std::wstring            m_wstrChannelingUID;
	std::wstring            m_wstrToonilandID;
};
#endif SERV_CHANNELING_SOAP
//}}

//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
DECL_PACKET( EGB_TODAY_RANDOM_QUEST_ACK )
{
	std::map< int, int >			m_mapTodayRandomQuest;
};
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
DECL_PACKET( DBE_UPDATE_CHANNEL_RANDOMKEY_REQ )
{
	std::wstring					m_wstrUserID;
};

DECL_PACKET( DBE_UPDATE_CHANNEL_RANDOMKEY_ACK )
{
	int						m_iOK;
	std::wstring			m_wstrUserID;
	int						m_iRandomKey;
};
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
DECL_PACKET( ERM_USER_UNIT_DIE_ACK )
{
	int			m_iOK;
	float		m_fReBirthTime;
	
	short		m_VP;
	short		m_EXP;

	unsigned char				m_ucDieReason;

	KERM_USER_UNIT_DIE_ACK()
	{
		m_iOK				= 0;
		m_fReBirthTime		= 0.f;
		m_VP				= 0;
		m_EXP				= 0;
		m_ucDieReason		= 0;
	}
};
#endif SERV_ADD_TITLE_CONDITION
//}}

//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
DECL_PACKET( ERM_FIELD_USER_LIST_MONITORING_MODE_REQ )
{
	UINT								m_uiViewPage;
	std::map< UidType, int >			m_mapUnit_ED;

	KERM_FIELD_USER_LIST_MONITORING_MODE_REQ()
	{
		m_uiViewPage = 0;
	};
};

DECL_PACKET( ERM_SQUARE_UNIT_REFRESH_ED_NOT )
{
	UidType								m_iUnitUID;
	int									m_iED;

	KERM_SQUARE_UNIT_REFRESH_ED_NOT()
	{
		m_iUnitUID = 0;
		m_iED = 0;
	};
};
#endif SERV_ED_MONITORING_IN_GAME
//}}

//{{ 2011. 10. 13	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
DECL_PACKET( DBE_USE_ITEM_IN_INVENTORY_REQ )
{
	UidType                             m_iUnitUID;
	int									m_iUsedItemID;
#ifdef SERV_GOLD_TICKET
	UidType								m_iItemUID;
#endif //SERV_GOLD_TICKET

	int									m_iWarpPointMapID;
	//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int									m_iED;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
	__int64								m_iTempCode;
//#endif SERV_PET_AUTO_LOOTING
	//}}

	KDBE_USE_ITEM_IN_INVENTORY_REQ()
		: m_iUnitUID( 0 )
#ifdef SERV_GOLD_TICKET
		, m_iItemUID( 0 )
#endif //SERV_GOLD_TICKET
		//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		, m_iED( 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
		, m_iTempCode( 0 )
//#endif SERV_PET_AUTO_LOOTING
		//}}
	{
	}
};

DECL_PACKET( DBE_USE_ITEM_IN_INVENTORY_ACK )
{
	int                                 m_iOK;
	int									m_iUsedItemID;
#ifdef SERV_GOLD_TICKET
	UidType								m_iItemUID;
#endif //SERV_GOLD_TICKET
	int									m_iWarpPointMapID;
	//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	int									m_iED;
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
	__int64								m_iTempCode;
//#endif SERV_PET_AUTO_LOOTING
	//}}

	KDBE_USE_ITEM_IN_INVENTORY_ACK()
		: m_iOK( 0 )
#ifdef SERV_GOLD_TICKET
		, m_iItemUID( 0 )
#endif //SERV_GOLD_TICKET
		//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		, m_iED( 0 )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
		, m_iTempCode( 0 )
//#endif SERV_PET_AUTO_LOOTING
		//}}
	{
	}
};
#endif SERV_USE_ITEM_DB_UPDATE_FIX
//}}

//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
DECL_PACKET( DBE_CHECK_SERVER_SN_REQ )
{
	__int64								m_iServerSN;
	std::string							m_strMachineID;
	std::wstring						m_wstrID;
};

DECL_PACKET( DBE_CREATE_SERVER_SN_REQ )
{
	std::string							m_strMachineID;
};

DECL_PACKET( DBE_CREATE_SERVER_SN_ACK )
{
	int									m_iOK;
	__int64								m_iServerSN;
};

typedef KDBE_CREATE_SERVER_SN_ACK						KDBE_CHECK_SERVER_SN_ACK;
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}

//{{  2011.11.08     김민성    강화 로그
#ifdef SERV_ENCHANT_ITEM_LOG
DECL_PACKET( ELOG_HIGH_ENCHANT_ITEM_LOG_NOT )
{
	enum ENCHANT_ITEM_RESULT
	{
		EIS_START_LOG_LEVEL = 7,			// 로그 남기기 시작하는 강화 레벨

		EIS_NONE			= 0,			// ??
		EIS_SUCCESS_ENCHANT = 1,			// 인첸트 성공
		EIS_BREAK_ITEM		= 2,			// 아이템 파손
		EIS_NO_CHANGE		= 3,			// 변화없음
		EIS_DOWN_ENCHANT	= 4,			// 강화 레벨 하락
		EIS_INIT_ITEM		= 5,			// 아이템 초기화
	};

	UidType								m_iUnitUID;
	UidType								m_iItemUID;
	int									m_iItemID;
	int									m_iBeforeLevel;
	int									m_iAfterLevel;
	bool								m_bSupportMaterial;
	int									m_iEnchantResult;
	std::wstring						m_wstrTime;

	KELOG_HIGH_ENCHANT_ITEM_LOG_NOT()
	{
		m_iUnitUID						= 0;
		m_iItemUID						= 0;
		m_iItemID						= 0;
		m_iBeforeLevel					= 0;
		m_iAfterLevel					= 0;
		m_bSupportMaterial				= false;
		m_iEnchantResult				= KELOG_HIGH_ENCHANT_ITEM_LOG_NOT::EIS_NONE;
	};
};
#endif SERV_ENCHANT_ITEM_LOG
//}}

//{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
DECL_PACKET( DBE_UNIT_CONNECT_LOG_NOT )
{
	UidType								m_iServerUID;
	UidType								m_iUnitUID;
	int									m_iUnitClass;
	int									m_iExp;
	int									m_iLevel;
	int									m_iPlayTime;
	bool								m_bPcBang;
	char								m_cAuthLevel;
	int									m_iLoginLevel;
	std::wstring						m_wstrName;
	std::wstring						m_wstrLogin;
	std::wstring						m_wstrLogout;

	KDBE_UNIT_CONNECT_LOG_NOT()
	{
		m_iServerUID			= 0;
		m_iUnitUID				= 0;
		m_iUnitClass			= 0;
		m_iExp					= 0;
		m_iLevel				= 0;
		m_iPlayTime				= 0;
		m_bPcBang				= false;
		m_cAuthLevel			= 0;
		m_iLoginLevel			= 0;
	};
};
#endif SERV_CHAR_CONNECT_LOG
//}}

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
DECL_PACKET( DBE_BUY_UNIT_CLASS_CHANGE_REQ )
{
	int										m_iOK;
	UidType									m_iUnitUID;
	int										m_iNewUnitClass;

	// kimhc // 김현철 // 2013-11-19 // m_iSPoint, m_iCSPoint가 스킬 페이지 수와 관계없이
	// 한개씩인 이유는 전직 변경에 의해 초기화 되면 모든 페이지의 SPoint와 CSPoint가
	// 똑같이 초기화 될 것이기 때문이다.
	int										m_iSPoint;			
	int										m_iCSPoint;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int										m_iTheNumberOfSkillPagesAvailable;
#endif // SERV_SKILL_PAGE_SYSTEM


#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	int										m_iDefaultSkillID1;
	int										m_iDefaultSkillID2;
	int										m_iDefaultSkillID3;
	int										m_iDefaultSkillID4;
	int										m_iDefaultSkillID5;
	int										m_iDefaultSkillID6;
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
	int										m_iDefaultSkillID;
	int										m_iDefaultSkillID2;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
	std::map< int, int >					m_mapChangeSkill; // <원본스킬, 바뀐스킬> 봉인해제한 스킬 변경 용
	
#ifdef SERV_REFORM_SKILL_NOTE
#else // SERV_REFORM_SKILL_NOTE
	std::map< int, int >					m_mapChangeMemo;  // <원본MemoIDt, 바뀐MemoID>
#endif // SERV_REFORM_SKILL_NOTE

	std::map< UidType, int >				m_mapChangeItem;  // <원본ItemUID, 바뀐ItemIID>
	std::map< int, int >					m_mapChangeCompleteQuest; // <원본퀘스트, 바뀔Quest>  완료된 퀘스트
	std::map< int, int >					m_mapChangeInProgressQuest; // <진행중퀘스트, 바뀔QuestInstance> 진행 중인 퀘스트
	std::vector< int >						m_vecDeleteCompleteQuest;		// 삭제를 해야 하는 완료 퀘스트
	std::vector< int >						m_vecDeleteInProgressQuest;		// 삭제를 해야 하는 진행 퀘스트

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
	bool									m_bUnlimitedSecondJobItem;			//무제한 전직 아이템으로 전직 했는가?
	int										m_iOldUnitClass;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

	KDBE_BUY_UNIT_CLASS_CHANGE_REQ()
	{
		m_iOK					= 0;
		m_iUnitUID				= 0;
		m_iNewUnitClass			= 0;
		m_iSPoint				= 0;
		m_iCSPoint				= 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iTheNumberOfSkillPagesAvailable = 1;
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		m_iDefaultSkillID1		= 0;
		m_iDefaultSkillID2		= 0;
		m_iDefaultSkillID3		= 0;
		m_iDefaultSkillID4		= 0;
		m_iDefaultSkillID5		= 0;
		m_iDefaultSkillID6		= 0;
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		m_iDefaultSkillID		= 0;
		m_iDefaultSkillID2		= 0;
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		m_bUnlimitedSecondJobItem	= false;
		m_iOldUnitClass			= 0;
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
	};
};
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2011. 12. 02  김민성	AP 구매 로그 수치
#ifdef SERV_BUY_AP_ITEM_LOG
DECL_PACKET( DBE_BUY_AP_ITEM_LOG_NOT )
{
	UidType									m_iUnitUID;
	int										m_iItemID;
	int										m_iQuantity;
	int										m_iBeforAP;
	int										m_iAfterAP;
	std::wstring							m_wstrBuyDate;

	KDBE_BUY_AP_ITEM_LOG_NOT()
	{
		m_iUnitUID			= 0;
		m_iItemID			= 0;
		m_iQuantity			= 0;
		m_iBeforAP			= 0;
		m_iAfterAP			= 0;
	};
};
#endif SERV_BUY_AP_ITEM_LOG
//}}

//{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
DECL_PACKET( DBE_UPDATE_PAKCET_MORNITORING_LOG_NOT )
{
	std::vector< KPacketProcessingLatencyInfo >		m_vecGSUserPacketLatencyInfo;
	std::vector< KPacketProcessingCountInfo >		m_vecLoginProxyPacketCountInfo;
	std::vector< KPacketProcessingCountInfo >		m_vecCnProxyPacketCountInfo;
	std::vector< KPacketProcessingCountInfo >		m_vecGlobalProxyPacketCountInfo;
};
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}

//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
DECL_PACKET( DBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ )
{
	int									m_iOK;
	KUserAuthenticateReq				m_kAuthenticateReq;
	KNexonAccountInfo					m_kNexonAccountInfo;
	__int64								m_iServerSN;
	std::string							m_strMachineID;

	//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	int									m_iMonitoringFlag;		// 1 : 모니터링 , 2 : 접속종료
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}

	KDBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ()
	{
		m_iOK							= 0;
		m_iServerSN						= 0;	
		//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
		m_iMonitoringFlag				= 0;
#endif SERV_SERIAL_NUMBER_MORNITORING
		//}}
	};
};
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
//}}

//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
DECL_PACKET( ELG_CHECK_MODULE_INFO_UPDATE_NOT )
{
	int											m_iOK;
	std::map< std::wstring, KModuleInfo >		m_mapModuleName;

	KELG_CHECK_MODULE_INFO_UPDATE_NOT()
	{
		m_iOK						= 0;
	};
};

DECL_PACKET( DBE_CHECK_MODULE_INFO_LOG_NOT )
{
	UidType										m_iUserUID;
	__int64										m_iServerSN;
	std::vector< KModuleInfo >					m_vecMonitoringDLL;
	std::vector< KModuleInfo >					m_vecBlackListDLL;

	KDBE_CHECK_MODULE_INFO_LOG_NOT()
	{
		m_iUserUID						= 0;
		m_iServerSN						= 0;
	};
};

DECL_PACKET( DBE_GET_HACKING_MODULE_LIST_REQ )
{
	int								m_iReleaseTick;

	KDBE_GET_HACKING_MODULE_LIST_REQ()
	{
		m_iReleaseTick						= 0;
	};
};

DECL_PACKET( DBE_GET_HACKING_MODULE_LIST_ACK )
{
	int										m_iOK;
	int										m_iReleaseTick;
	std::vector< KModuleInfo >				m_vecModuleList;
	bool									m_bRequest;

	KDBE_GET_HACKING_MODULE_LIST_ACK()
	{
		m_iOK							= 0;
		m_iReleaseTick					= 0;
		m_bRequest						= 0;
	};
};
#endif SERV_HACKING_DLL_CHECK
//}}

//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM

DECL_PACKET( EGB_GET_AUTO_PARTY_BONUS_INFO_ACK )
{
	std::vector< KAutoPartyBonusInfo >	m_vecAutoPartyBonusInfo;
};

DECL_PACKET( EGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT )
{
    char								m_cDungeonMode;
	int									m_iDungeonIDWithDif;
    bool								m_bActivatedBonus;

	KEGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT()
	{
		m_cDungeonMode = 0;
		m_iDungeonIDWithDif = 0;
		m_bActivatedBonus = false;
	}
};

DECL_PACKET( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ )
{
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
};

DECL_PACKET( EPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK )
{
	int									m_iOK;
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;

	KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( ERM_OPEN_BATTLE_FIELD_REQ )
{
	KRoomInfo                           m_kRoomInfo;
	KRoomUserInfo                       m_kRoomUserInfo;
	std::vector< UidType >				m_vecStudentUnitUID;	//사제시스템중 제자리스트	
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
	KReturnToFieldInfo					m_kReturnToFieldInfo;	// 필드로 복귀에 필요한 정보
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	KBossFieldCreateInfo				m_KBossFieldCreateInfo;
	KBossFieldJoinInfo					m_kBossFieldJoinInfo;
#endif // SERV_BATTLE_FIELD_BOSS
};

DECL_PACKET( ERM_OPEN_BATTLE_FIELD_ACK )
{
	int                                 m_iOK;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::wstring						m_wstrCNIP;
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
    KBattleFieldJoinInfo                m_kBattleFieldJoinInfo;
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	int									m_StartPosIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	std::wstring						m_wstrUDPRelayIP;
	USHORT								m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	__time64_t							m_tRemainFieldHoldingTime;
	KBossFieldJoinInfo					m_kBossFieldJoinInfo;
	bool								m_bBossField;
#endif // SERV_BATTLE_FIELD_BOSS

	KERM_OPEN_BATTLE_FIELD_ACK()
		: m_iOK( 0 )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		, m_tRemainFieldHoldingTime( 0 )
		, m_bBossField( false )
#endif // SERV_BATTLE_FIELD_BOSS
	{
	}
};

DECL_PACKET( ERM_JOIN_BATTLE_FIELD_REQ )
{
	KRoomUserInfo                       m_kRoomUserInfo;
	std::vector< UidType >				m_vecStudentUnitUID;	//사제시스템중 제자리스트	
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
	KReturnToFieldInfo					m_kReturnToFieldInfo;	// 필드로 복귀에 필요한 정보
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
	KBossFieldJoinInfo					m_kBossFieldJoinInfo;
#endif // SERV_BATTLE_FIELD_BOSS
};

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
DECL_PACKET( ERM_JOIN_BATTLE_FIELD_ACK )
{
	int                                 m_iOK;
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;

	std::wstring						m_wstrCNIP;
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	int									m_iLastTouchIndex;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

	int									m_iRequireLevel;
	int									m_iRequireDungeonID;

	//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
	std::wstring						m_wstrUDPRelayIP;
	USHORT								m_usUDPRelayPort;
#endif SERV_CHOOSE_FASTEST_HOST
	//}
	//{{ 2012. 11. 21	최육사		난입자에게 캐쉬샵 유저 정보 보내기
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	std::vector< UidType >				m_vecEnterCashShopUser;
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
	//}}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
	__time64_t							m_tRemainFieldHoldingTime;	// 보스 필드가 닫힐 때까지 남은 시간
	KBossFieldJoinInfo					m_kBossFieldJoinInfo;
	bool								m_bBossField;
#endif // SERV_BATTLE_FIELD_BOSS

	KERM_JOIN_BATTLE_FIELD_ACK()
	{
		Initialize();
	}

	void Initialize()
	{
		m_iOK = 0;
		m_kBattleFieldJoinInfo.Initialize();
		m_RoomInfo.Initialize();
		m_vecSlot.clear();
		m_wstrCNIP.clear();
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_iLastTouchIndex = 0;
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		m_iRequireLevel = 0;
		m_iRequireDungeonID = 0;

		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		m_wstrUDPRelayIP.clear();
		m_usUDPRelayPort = 0;
#endif SERV_CHOOSE_FASTEST_HOST

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		m_tRemainFieldHoldingTime = 0;
		m_bBossField = false;
#endif // SERV_BATTLE_FIELD_BOSS
	}
};
#else // SERV_BATTLE_FIELD_BOSS
typedef KPacketOK		KERM_JOIN_BATTLE_FIELD_ACK;
#endif // SERV_BATTLE_FIELD_BOSS

DECL_PACKET( ERM_BATTLE_FIELD_ROOM_INFO_NOT )
{
	enum INFO_TYPE
	{
		IT_UPDATE = 0,
        IT_DELETE,
	};

	char								m_cType;
	KBattleFieldRoomInfo				m_kBFRoomInfo;

	KERM_BATTLE_FIELD_ROOM_INFO_NOT()
	{
		m_cType = IT_UPDATE;
	}
};

DECL_PACKET( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ )
{
	KDungeonGameInfo					m_kDungeonGameInfo;
};

DECL_PACKET( EPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrFailUserNickName;

	KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK()
	{
		m_iOK = 0;
	}
};

typedef KPacketOK		KERM_SINGLE_OPEN_DUNGEON_ROOM_ACK;

// 자동 파티 매칭
DECL_PACKET( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ )
{
	bool								m_bRemakingAutoParty;
	UidType								m_iPartyUID;

	KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ()
	{
		m_bRemakingAutoParty = false;
		m_iPartyUID = 0;
	}
};

DECL_PACKET( EPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrFailUserNickName;
	bool								m_bRemakingAutoParty;
	KAutoPartyConditionInfo				m_kAutoPartyCondition;
	std::vector< KAutoPartyUserInfo >	m_vecAutoPartyUser;	

	KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK()
	{
		m_bRemakingAutoParty = false;
		m_iOK = 0;
	}
};

typedef KDungeonGameInfo	KEPM_CHECK_FOR_AUTO_PARTY_MAKING_REQ;

DECL_PACKET( EPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrFailUserNickName;

	KEPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( EGB_REQUEST_AUTO_PARTY_MAKING_REQ )
{
	bool								m_bRemakingAutoParty;
	KAutoPartyConditionInfo				m_kAutoPartyCondition;
	std::vector< KAutoPartyUserInfo >	m_vecAutoPartyUser;

	KEGB_REQUEST_AUTO_PARTY_MAKING_REQ()
	{
		m_bRemakingAutoParty = false;
	}
};

typedef KPacketOK		KEGB_REQUEST_AUTO_PARTY_MAKING_ACK;

DECL_PACKET( EGB_CANCEL_AUTO_PARTY_MAKING_REQ )
{
	UidType							m_iAutoPartyUID;
	UidType							m_iAutoPartyWaitNumber;
	int								m_iReason;
	bool							m_bSendBySystem;

	KEGB_CANCEL_AUTO_PARTY_MAKING_REQ()
	{
		m_iAutoPartyUID = 0;
		m_iAutoPartyWaitNumber = 0;
		m_iReason = 0;
		m_bSendBySystem = false;
	}
};

DECL_PACKET( EGB_REMAKING_AUTO_PARTY_NOT )
{
    KAutoPartyConditionInfo			m_kAutoPartyInfo;
};

DECL_PACKET( EGB_GET_REGROUP_PARTY_INFO_REQ )
{
	UidType							m_iAutoPartyUID;
	KDungeonGameInfo				m_kDungeonGameInfo;

	KEGB_GET_REGROUP_PARTY_INFO_REQ()
	{
		m_iAutoPartyUID = 0;
	}
};

DECL_PACKET( EGB_GET_REGROUP_PARTY_INFO_ACK )
{
	KPartyUserInfo					m_kPartyUserInfo;
	KPartyRoomUserInfo				m_kPartyRoomUserInfo;
};

DECL_PACKET( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ )
{
	UidType									m_iAutoPartyUID;
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	UidType									m_iAutoPartyHostUnitUID;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	KCreatePartyInfo						m_kCreatePartyInfo;
	std::vector< KRegroupPartyUserInfo >	m_vecRegroupPartyUserInfo;

	KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ()
	{
		m_iAutoPartyUID = 0;
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		m_iAutoPartyHostUnitUID = 0;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
	}
};

DECL_PACKET( EPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK )
{
	int										m_iOK;
	UidType									m_iAutoPartyUID;
	
	KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK()
	{
		m_iOK = 0;
		m_iAutoPartyUID = 0;
	}
};

DECL_PACKET( EPM_REQUEST_REGROUP_PARTY_NOT )
{
	KCreatePartyInfo						m_kCreatePartyInfo;
	//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
	UidType									m_iOldHostUnitUID;
#endif SERV_REGROUP_OLD_PARTY_HOST
	//}}
	short									m_sPartyFever;
	std::vector< KRegroupPartyUserInfo >	m_vecRegroupPartyUserInfo;

	KEPM_REQUEST_REGROUP_PARTY_NOT()
	{
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		m_iOldHostUnitUID = 0;
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		m_sPartyFever = 0;
	}
};

DECL_PACKET( EGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT )
{
	enum COMMAND_TYPE
	{
		CT_DUMP = 0,
		CT_CLEAR,
	};

	char					m_cType;

	KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT()
	{
		m_cType = CT_DUMP;
	}
};

DECL_PACKET( ERM_BATTLE_FIELD_ZU_ECHO_REQ )
{
    UidType								m_iUnitUID;
	//{{ 2012. 11. 20	최육사		배틀필드 좀비 유저 체크 강화
#ifdef SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
	UidType								m_iRoomUID;
#endif SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
	//}}

	KERM_BATTLE_FIELD_ZU_ECHO_REQ()
	{
		m_iUnitUID = 0;
		//{{ 2012. 11. 20	최육사		배틀필드 좀비 유저 체크 강화
#ifdef SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		m_iRoomUID = 0;
#endif SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		//}}
	}
};

DECL_PACKET( ERM_DUMP_ROOM_MONSTER_NOT )
{
	std::wstring						m_wstrNickName;
	int									m_iNpcUID;

	KERM_DUMP_ROOM_MONSTER_NOT()
	{
		m_iNpcUID = -1;
	}
};

DECL_PACKET( ERM_DUMP_BATTLE_FIELD_NOT )
{
	std::wstring						m_wstrNickName;
};

DECL_PACKET( ERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT )
{
	UidType								m_iPartyUID;
	std::vector< UidType >				m_vecPartyUnitUIDList; // UnitUID

	KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT()
	{
		m_iPartyUID = 0;
	}
};

DECL_PACKET( EPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT )
{
	KPartyUserJoinRoomInfo				m_kJoinRoomInfo;  
};

DECL_PACKET( ERM_END_GAME_REGROUP_PARTY_NOT )
{
	UidType								m_iGSUID;
	UidType								m_iPartyUID;

	KERM_END_GAME_REGROUP_PARTY_NOT()
	{
		m_iGSUID = 0;
		m_iPartyUID = 0;
	}
};

DECL_PACKET( EPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT )
{
	UidType								m_iPartyUID;

	KEPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT()
	{
		m_iPartyUID = 0;
	}
};

DECL_PACKET( ERM_COMPLETE_RETURN_TO_FIELD_NOT )
{
    KReturnToFieldInfo					m_kReturnToFieldInfo;
};

DECL_PACKET( ELOG_BATTLE_FIELD_LEAVE_LOG_NOT )
{
	enum LEAVE_TYPE
	{
		LT_NONE = 0,

		LT_DISCONNECT,			// 1=접속종료
		LT_CHANNEL_CHANGE,		// 2=채널이동
		LT_WALK_TO_DIF_FIELD,	// 3=걸어서(포탈)이동
		LT_USE_WARP_ITEM,		// 4=귀환서로이동
		LT_MOVE_PARTY_FIELD,	// 5=파티필드로이동
		LT_ENTER_THE_DUNGEON,	// 6=던전 입장
		LT_ENTER_THE_PVP,		// 7=대전입장
		LT_ABNORMAL_DISCONNECT,	// 8=비정상종료
		LT_CLIENT_CRASH,		// 9=크래시
	};

	std::wstring						m_wstrServerIP;
	int									m_iBattleFieldID;
	UidType								m_iRoomUID;
	char								m_cLeaveType;	// 이탈 종류
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;
	char								m_cUnitClass;
	u_char								m_ucLevel;
	int									m_iPlayTime;
	int									m_iGetEXP;
	int									m_iGetED;
	std::wstring						m_wstrRegDate;
	//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
	int									m_iFieldFrame;
#endif SERV_FIELD_FRAME_LOG
	//}}
#ifdef SERV_ADD_FIELD_LOG_COLUMN
	short								m_sNpcDieCount;
#endif SERV_ADD_FIELD_LOG_COLUMN

	KELOG_BATTLE_FIELD_LEAVE_LOG_NOT()
		: m_iBattleFieldID( 0 ),
		m_iRoomUID( 0 ),
		m_cLeaveType( LT_NONE ),
		m_iUnitUID( 0 ),
		m_cUnitClass( 0 ),
		m_ucLevel( 0 ),
		m_iPlayTime( 0 ),
		m_iGetEXP( 0 ),
		m_iGetED( 0 )
		//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
		, m_iFieldFrame( 0 )
#endif SERV_FIELD_FRAME_LOG
		//}}
#ifdef SERV_ADD_FIELD_LOG_COLUMN
		, m_sNpcDieCount( 0 )
#endif SERV_ADD_FIELD_LOG_COLUMN
	{		
	}
};

#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
DECL_PACKET( DBE_GET_CENTER_SERVER_LIST_ACK )
{
	std::vector< KServerInfo >			m_vecCenterServerList;
};

DECL_PACKET( DBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT )
{
	std::vector< KUdpRelayCheckLog >	m_vecUdpRelayCheckLog;
};
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 03. 25	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
DECL_PACKET( EGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT )
{
	int		m_iTheGateOfDarknessIndividualBuffLevel;

	KEGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT()
		: m_iTheGateOfDarknessIndividualBuffLevel( 0 )
	{
	}
};
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
DECL_PACKET( EGB_REGISTER_SERVERSN_REQ )
{
	UidType					m_iUserUid;
	__int64					m_iServerSN;

	KEGB_REGISTER_SERVERSN_REQ()
	{
		m_iUserUid = 0;
		m_iServerSN = 0;
	}
};

DECL_PACKET( EGB_REGISTER_SERVERSN_ACK )
{
	int						m_iOK;
	UidType					m_iUserUid;

	KEGB_REGISTER_SERVERSN_ACK()
	{
		m_iOK = 0;
		m_iUserUid = 0;
	}
};

typedef KEGB_REGISTER_SERVERSN_ACK	KEGB_REGISTER_SERVERSN_NOT;

DECL_PACKET( EGB_UNREGISTER_SERVERSN_NOT )
{
	__int64					m_iServerSN;
	UidType					m_iUserUid;

	KEGB_UNREGISTER_SERVERSN_NOT()
	{
		m_iServerSN = 0;
		m_iUserUid  = 0;
	}
};
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}

//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
DECL_PACKET( DBE_MODULE_EXIST_LOG_NOT )
{
	UidType							m_iUserUID;
	__int64							m_iServerSN;
	std::map<std::wstring, bool>	m_mapModule;
	std::wstring					m_wstrRegDate;

	KDBE_MODULE_EXIST_LOG_NOT()
	{
		m_iUserUID		 = 0;
		m_iServerSN		 = 0;
	}
};
#endif SERV_CHECK_X2_EXE_MODULE
//}}

//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
DECL_PACKET( SOAP_CHECK_NEXON_ID_REQ )
{
	int									m_iServiceCode;
	int									m_iGameCode;
	__int64								m_iNexonSN;
	std::wstring						m_wstrLoginID;
	std::string							m_strPassword;
	std::wstring                        m_wstrReceiverNickName;
	std::wstring                        m_wstrMessage;
	std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KSOAP_CHECK_NEXON_ID_REQ()
		: m_iServiceCode( 0 )
		, m_iGameCode( 0 )
		, m_iNexonSN( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};

DECL_PACKET( SOAP_CHECK_NEXON_ID_ACK )
{
	int									m_iOK;
	std::wstring						m_wstrLoginID;
	std::wstring                        m_wstrReceiverNickName;
	std::wstring                        m_wstrMessage;
	std::vector< KNXBTPurchaseReqInfo > m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	bool								m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM

	KSOAP_CHECK_NEXON_ID_ACK()
		: m_iOK( 0 )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		, m_bUseCoupon( false )
#endif // SERV_NEXON_COUPON_SYSTEM
	{
	}
};
#endif SERV_NEXON_AUTH_SOAP
//}}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
DECL_PACKET( DBE_TRADE_COMPLETE_REQ )
{
	UidType								m_iUnitUID;
	int									m_iED;
	KItemQuantityUpdate					m_kQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;

	KDBE_TRADE_COMPLETE_REQ()
	{
		m_iUnitUID = 0;
		m_iED = 0;
	}
};

DECL_PACKET( DBE_TRADE_COMPLETE_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	int									m_iED;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	std::map< UidType, int >			m_mapUpdateFailed;

	KDBE_TRADE_COMPLETE_ACK()
	{
		m_iOK = 0;
		m_iUnitUID = 0;
		m_iED = 0;
	}
};
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
DECL_PACKET( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ )
{
	UidType								m_iSellUnitUID;
	UidType								m_iSellUnitGSUID;
	int									m_iSellUnitEDIN;
	int									m_iSellCommissionED;
	//KItemQuantityUpdate					m_kSellItemQuantityUpdateTemp;
	//std::vector< KItemInfo >			m_vecSellItemInfoTemp;

	UidType								m_iBuyUnitUID;
	UidType								m_iBuyUnitGSUID;
	int									m_iBuyUnitEDOUT;
	int									m_iBuyCommissionED;
	bool								m_bBuyUnitIsSquare;
	//KItemQuantityUpdate					m_kBuyItemQuantityUpdateTemp;
	//std::vector< KItemInfo >			m_vecBuyItemInfoTemp;

	KInventoryItemInfo					m_kSellItemInfo;
	KInventoryItemInfo					m_kBuyItemInfo;
			
	KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ()
	{
		m_iSellUnitUID				= 0;
		m_iSellUnitGSUID				= 0;
		m_iSellUnitEDIN				= 0;
		m_iSellCommissionED			= 0;

		m_iBuyUnitUID				= 0;
		m_iBuyUnitGSUID					= 0;
		m_iBuyUnitEDOUT				= 0;
		m_iBuyCommissionED			= 0;
	}
};

DECL_PACKET( DBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK )
{
	int											m_iOK;
	KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ	m_kResult;	

	KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK()
	{
		m_iOK = 0;

	}
};
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}


//{{ 2012. 05. 30	최육사		공식 대전 버그 수정
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
DECL_PACKET( EPM_UPDATE_PVP_INFO_NOT )
{
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	char								m_cRank;
	int									m_iOfficialMatchCnt;
#endif SERV_2012_PVP_SEASON2
	//}}
	int									m_iRating;
	int									m_iRPoint;

	KEPM_UPDATE_PVP_INFO_NOT()
	{
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		m_cRank   = 0;
		m_iOfficialMatchCnt	= 0;
#endif SERV_2012_PVP_SEASON2
		//}}
		m_iRating = 0;
		m_iRPoint = 0;
	}
};
#endif SERV_OFFCIAL_PVP_BUG_FIX
//}}


//{{ 2012. 05. 17	박세훈	키보드 맵핑 정보를 DB에서 읽고 쓰기
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
DECL_PACKET( DBE_KEYBOARD_MAPPING_INFO_WRITE_REQ )
{
	UidType								m_iUserUID;
	KKeyboardMappingInfo				m_kKeyboardMappingInfo;
};
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}

//{{ 2012. 05. 29	박세훈	채팅 옵션 정보를 DB에서 읽고 쓰기
#ifdef SERV_CHAT_OPTION_INFO_RW
DECL_PACKET( DBE_CHAT_OPTION_INFO_WRITE_REQ )
{
	UidType								m_iUserUID;
	byte								m_cIndex;
	bool								m_bValue;
};
#endif SERV_CHAT_OPTION_INFO_RW
//}}

//{{ 2012. 05. 30	최육사	그노시스 기간 치트
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
DECL_PACKET( DBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ )
{
	UidType								m_iUnitUID;
	std::wstring						m_wstrModifyDate;

	KDBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ()
	{
		m_iUnitUID = 0;
	}
};
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
//}}

//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
DECL_PACKET( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT )
{
	UidType							m_iUserUID;
	UidType							m_iUnitUID;
	int								m_iItemID;
	byte							m_iState;
	std::wstring					m_wstrRegDate;
};
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
DECL_PACKET( DBE_UPDATE_EVENT_MONEY_REQ )
{
	UidType							m_iUserUID;
	UidType							m_iUnitUID;
	int								m_iOldQuantity;
	int								m_iNewQuantity;
	int								m_iRewardItemID;
	std::wstring					m_wstrRegDate;

	KDBE_UPDATE_EVENT_MONEY_REQ()
	{
		m_iUserUID = 0;
		m_iUnitUID = 0;
		m_iOldQuantity = 0;
		m_iNewQuantity = 0;
		m_iRewardItemID = 0;		
	}
};

typedef int					KDBE_UPDATE_EVENT_MONEY_ACK;
#endif // SERV_EVENT_MONEY

//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
DECL_PACKET( DBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK )
{
	int								m_iTotalCCU;

	KDBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK()
	{
		m_iTotalCCU = 0;
	}
};
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
DECL_PACKET( DBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ )
{
	UidType							m_iUnitUID;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int								m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	std::vector<short>				m_vecUnSealedSkill;
	std::map<int, KAdminCheatSkill>	m_mapSkillInfo;	// m_iSkillID, KAdminCheatSkill

#ifdef SERV_SKILL_PAGE_SYSTEM
	KDBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ()
		: m_iUnitUID(0), m_iActiveSkillPageNumber(0)
	{
		m_vecUnSealedSkill.clear();
		m_mapSkillInfo.clear();
	}
#endif // SERV_SKILL_PAGE_SYSTEM
};

typedef KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK KDBE_ADMIN_CHEAT_GET_ALL_SKILL_ACK;
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}

//{{ 2012. 07. 27	최육사		사제시스템 EXP 보너스 버그 수정
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
DECL_PACKET( ERM_UPDATE_TUTORIAL_INFO_NOT )
{
	std::vector< UidType >			m_vecStudentUnitUID;
};
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
//}}

//{{ 우편 ED 거래시 ED 동기화 - 김민성
#ifdef SERV_SEND_LETTER_BEFOR_ED_SYNC
DECL_PACKET( DBE_SYNC_ED_REQ )
{
	UidType								m_iFromUnitUID;
	bool								m_bIsGmMessage;
	std::wstring						m_wstrToNickName;
	std::wstring						m_wstrTitle;
	std::wstring						m_wstrMessage;
	int									m_iED;								// 우편 첨부  ED
	UidType								m_iItemUID;	
	int									m_iQuantity;
	int									m_iChangeED;						// 우편 보내기 전 동기화용 ED
	int									m_iSendLetterCost;					// 우편 보내기 수수료
	
	KDBE_SYNC_ED_REQ()
	{
		m_iFromUnitUID = 0;
		m_iItemUID = 0;
		m_iQuantity = 0;
		m_iED = 0;
		m_iChangeED = 0;
		m_iSendLetterCost = 0;
	}
};

DECL_PACKET( DBE_SYNC_ED_ACK )
{
	int									m_iOK;
	KDBE_SYNC_ED_REQ					m_kInfo;

	KDBE_SYNC_ED_ACK()
	{
		m_iOK = 0;
	}
};
#endif SERV_SEND_LETTER_BEFOR_ED_SYNC
//}}

//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
typedef KDBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK KEGS_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK;
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
//}}

//{{ 2012. 08. 21	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
typedef KEGS_GET_ITEM_FROM_LETTER_ACK	KDBE_GET_ITEM_FROM_LETTER_ARRANGE_REQ;
typedef KEGS_GET_ITEM_FROM_LETTER_ACK	KDBE_GET_ITEM_FROM_LETTER_ARRANGE_ACK;
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
DECL_PACKET( DBE_PVP_WIN_EVENT_CHECK_REQ )
{
	UidType								m_iUnitUID;
	int									m_iIndex;
	std::wstring						m_wstrRegDate;

	KDBE_PVP_WIN_EVENT_CHECK_REQ()
	{
		m_iUnitUID = 0;
		m_iIndex = 0;
	}
};

DECL_PACKET( DBE_PVP_WIN_EVENT_CHECK_ACK )
{
	int									m_iOK;
	int									m_iIndex;
	std::wstring						m_wstrRegDate;
	KDBE_INSERT_REWARD_TO_POST_ACK		m_kRewardAck;

	KDBE_PVP_WIN_EVENT_CHECK_ACK()
	{
		m_iOK = 0;
		m_iIndex = 0;
	}
};

#endif SERV_2012_PVP_SEASON2_EVENT
//}}

//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
enum BingoSetValue
{
	BSV_NUM_MAX						= 100,
	BSV_BINGOBOARD_SLOT_NUM			= 25,
	BSV_PRESENT_NUM					= 12,
	BSV_BINGOBOARD_SLOT_NUM_CHECK	= BSV_BINGOBOARD_SLOT_NUM + 1,
	BSV_PRESENT_NUM_CHECK			= BSV_PRESENT_NUM + 1,
};

DECL_DATA( KBingoEventLog )
{
	UidType			iUnitUID;
	byte			iActionType;
	int				iOpenNum;
	int				iPos;
	int				iItemID;
	std::wstring	wstrRegDate;

	KBingoEventLog()
	{
		iUnitUID	= 0;
		iActionType	= 0;
		iOpenNum	= -1;
		iPos		= -1;
		iItemID		= 0;
	}
};

DECL_PACKET( DBE_BINGO_EVENT_INFO_WRITE_REQ )		// 빙고 이벤트 DB 쓰기 정보
{
	unsigned short							m_usAckEventID;			// 처리할 ACK 패킷
	bool									m_bChanceChanged;		// 기회 정보 변경 여부
	int										m_iBalance;				// 잔액 정보
	int										m_iLotteryChance;		// 뽑기 찬스 횟수
	int										m_iLotteryRecord;		// 뽑은 횟수
	int										m_iMixChance;			// 뒤섞기 찬스 횟수
	int										m_iResetChance;			// 초기화 찬스 횟수
	std::map<byte, byte>					m_mapBingoBoard;		// 빙고 숫자판
	std::map<byte, std::pair<int, bool> >	m_mapPresentInfo;		// 선물 정보
	std::vector<KBingoEventLog>				m_vecLog;				// 로그 정보

	KDBE_BINGO_EVENT_INFO_WRITE_REQ()
	{
		clear();
	}

	void clear( void )
	{
		m_usAckEventID		= 0;
		m_bChanceChanged	= false;
		m_iBalance			= 0;
		m_iLotteryChance	= 0;
		m_iLotteryRecord	= 0;
		m_iMixChance		= 0;
		m_iResetChance		= 0;
		m_mapBingoBoard.clear();
		m_mapPresentInfo.clear();
		m_vecLog.clear();
	}
};

typedef KDBE_BINGO_EVENT_INFO_WRITE_REQ	KDBE_BINGO_EVENT_INFO_WRITE_ACK;
typedef KDBE_BINGO_EVENT_INFO_WRITE_REQ	KDBE_BINGO_EVENT_LOTTERY_ACK;
typedef KDBE_BINGO_EVENT_INFO_WRITE_REQ	KDBE_BINGO_EVENT_MIX_ACK;
typedef KDBE_BINGO_EVENT_INFO_WRITE_REQ	KDBE_BINGO_EVENT_RESET_ACK;
typedef KDBE_BINGO_EVENT_INFO_WRITE_REQ	KDBE_BINGO_EVENT_INIT_ACK;
#endif SERV_EVENT_BINGO
//}}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
DECL_PACKET( ERM_UPDATE_QUEST_INFO_NOT )
{
	std::vector< int >	m_vecNPC;
	bool				m_isDungeon;
};
#endif SERV_REFORM_QUEST
//}}

//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
DECL_DATA( KEDMornitoringInfo )
{
	__int64		m_iMaxED;
	__int64		m_iAverageED;
	
	KEDMornitoringInfo()
	{
		m_iMaxED = 0;
		m_iAverageED = 0;
	}
};

DECL_DATA( KItemMornitoringInfo )
{
	int			m_iMaxItemCount;
	int			m_iAverageItemCount;

	KItemMornitoringInfo()
	{
		m_iMaxItemCount = 0;
		m_iAverageItemCount = 0;
	}
};

DECL_PACKET( DBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK )
{
	std::map< int, KEDMornitoringInfo >		m_mapEDMornitoringInfo;
	std::map< int, KItemMornitoringInfo >	m_mapItemMornitoringInfo;
};
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}

//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
DECL_DATA( KDBUpdateGamePlayNetworkInfo )
{
	int								m_iGameType;
	int								m_iHolePunchingTryCount;	// 홀 펀칭 시도 횟수
	int								m_iP2PPlayTime;				// P2P 네트워크로 플레이한 시간 ( ms단위 )
	int								m_iRelayPlayTime;			// 릴레이 서버를 통해 플레이한 시간 ( ms단위 )
	std::wstring					m_wstrRelayServerIP;
	std::wstring					m_wstrRegDate;

	KDBUpdateGamePlayNetworkInfo()
	{
		m_iGameType = 0;
		m_iHolePunchingTryCount = 0;
		m_iP2PPlayTime = 0;
		m_iRelayPlayTime = 0;
	}
};

DECL_PACKET( ELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT )
{
	std::vector< KDBUpdateGamePlayNetworkInfo >	m_vecDBUpdateInfo;
};
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
DECL_PACKET( ERM_UPDATE_VISIT_CASH_SHOP_NOT )
{
	bool						m_bEnterCashShop;
	UidType						m_iUnitUID;

	KERM_UPDATE_VISIT_CASH_SHOP_NOT()
	{
		m_bEnterCashShop = false;
		m_iUnitUID = 0;
	}
};
#endif SERV_VISIT_CASH_SHOP
//}}

//{{ 배틀필드 위험도 확인 치트 - 김민성
#ifdef SERV_BATTLE_FIELD_DANGEROUS_CHEAT
typedef int	KERM_BATTLE_FIELD_DANGEROUS_VALUE_ACK;
#endif SERV_BATTLE_FIELD_DANGEROUS_CHEAT
//}}

//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
DECL_PACKET( EGB_REGISTER_UNIT_NOT )
{
	KRegUnitInfo		m_kUnitInfo;
};

DECL_PACKET( EGB_UNIT_DISCONNECT_NOT )
{
	UidType				m_iUnitUID;
	//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
	UidType				m_iUserUID;
#endif SERV_EVENT_JACKPOT
	//}}

	KEGB_UNIT_DISCONNECT_NOT()
		: m_iUnitUID( 0 )
		//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
		, m_iUserUID( 0 )
#endif SERV_EVENT_JACKPOT
		//}}
	{
	}
};
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 10. 7	박세훈	랜선렉 로그 추가
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
DECL_DATA( KLanBugRecord )
{
	UidType		m_iMyUnitUID;
	UidType		m_iAccusationUnitUID;
	byte		m_byteNum;
	bool		m_bNowKickOut;
	int			m_iMyUnitWin;

	KLanBugRecord()
	{
		m_iMyUnitUID			= 0;
		m_iAccusationUnitUID	= 0;
		m_byteNum				= 0;
		m_bNowKickOut			= false;
		m_iMyUnitWin			= 0;
	};

	KLanBugRecord( UidType	iMyUnitUID,
				   UidType	iAccusationUnitUID,
				   byte		byteNum,
				   bool		bNowKickOut,
				   int		iMyUnitWin )
	{
		m_iMyUnitUID			= iMyUnitUID;
		m_iAccusationUnitUID	= iAccusationUnitUID;
		m_byteNum				= byteNum;
		m_bNowKickOut			= bNowKickOut;
		m_iMyUnitWin			= iMyUnitWin;
	};
};

DECL_PACKET( DBE_UDP_CHECK_KICK_USER_NOT )
{
	UidType						m_iRoomUID;
	int							m_iPVPChannelClass;
	int							m_iMatchType;
	std::vector<KLanBugRecord>	m_vecLanBugRecord;

	KDBE_UDP_CHECK_KICK_USER_NOT()
	{
		m_iRoomUID			= 0;
		m_iPVPChannelClass	= -1;
		m_iMatchType		= -1;
	}
};
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
//}}

//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
DECL_PACKET( EGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT )
{
    UidType						m_iAutoPartyWaitNumber;

	KEGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT()
	{
		m_iAutoPartyWaitNumber = 0;
	}
};
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
//}}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
DECL_PACKET( ECN_PARTY_BUFF_UPDATE_NOT )
{
	std::map<int, KBuffInfo>	m_mapActivateBuffList;		// Activate Buff 저장소
	std::set<int>				m_setDeactivateBuffList;	// Deactivate Buff 저장소
};

DECL_PACKET( ERM_ACTIVATE_BUFF_NOT )
{
	std::vector< KBuffInfo >			m_vecActivateBuffList;
};

DECL_PACKET( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ )
{
	UidType							m_iUnitUID;
	std::vector< KBuffInfo >		m_vecActivateBuff;
	std::vector< int >				m_vecDeActivateBuff;
	bool							m_bSendAck;

	KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ()
	{
		m_iUnitUID = 0;
		m_bSendAck = true;
	}
};

DECL_PACKET( ELG_UPDATE_USER_UNIT_BUFF_INFO_ACK )
{
	KUnitBuffInfo					m_kUnitBuffInfo;
};

DECL_PACKET( ERM_PARTY_OPEN_DUNGEON_ROOM_NOT )
{
	short								m_sWorldID;
	KRoomInfo						    m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::vector< KRoomSlotInfo >		m_vecObserverSlot; // 던전게임인데 옵저버가 필요있나?

	std::wstring						m_wstrCNIP;

	//{{ 2009. 2. 4  최육사		던전 입장권
	std::vector< KInventoryItemInfo >	m_vecInventorySlotInfo;
	//}}

	std::map< UidType, std::vector< KBuffInfo > >		m_mapActivateBuff;
};
#endif SERV_SERVER_BUFF_SYSTEM
//}}

//{{ 2012. 12. 18	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
DECL_PACKET( ERM_DEACTIVATE_BUFF_NOT )
{
	std::vector< int >					m_vecDeactivateBuffID;
};
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
//}}

//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
DECL_PACKET( ERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT )
{
	std::wstring						m_wstrNickName;
};
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
//}}

//{{ 2012. 11. 15	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
DECL_PACKET( ERM_FIELD_USER_STATISTICS_NOT )
{
	int			m_iGamePlayTime;

	KERM_FIELD_USER_STATISTICS_NOT()
		: m_iGamePlayTime( 0 )
	{
	}
};
#endif SERV_FIELD_PLAY_TIME_LOG
//}}

//{{ 2012. 12. 12	최육사	결과창 이탈 유저 로그
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
DECL_PACKET( DBE_DUNGEON_RESULT_LEAVE_LOG_NOT )
{
    UidType								m_iUnitUID;
	std::wstring						m_wstrLeaveReason;
	std::wstring						m_wstrRoomState;
	std::wstring						m_wstrRegDate;

	KDBE_DUNGEON_RESULT_LEAVE_LOG_NOT()
	{
		m_iUnitUID = 0;
	}
};
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
//}}

//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
DECL_DATA( KJackPotEventInitInfo )
{
	std::wstring	m_wstrCuponNumber;
	std::wstring	m_wstrBeginDate;
	std::wstring	m_wstrEndDate;
	UidType			m_iUserUID;
	UidType			m_iUnitUID;
	int				m_iState;

	KJackPotEventInitInfo()
		: m_iUserUID( 0 )
		, m_iUnitUID( 0 )
		, m_iState( 0 )
	{
	}
};

DECL_DATA( KJackPotEventWinInfo )
{
	std::wstring	m_wstrCuponNumber;
	UidType			m_iUserUID;
	UidType			m_iUnitUID;
	int				m_iState;

	KJackPotEventWinInfo()
		: m_iUserUID( 0 )
		, m_iUnitUID( 0 )
		, m_iState( 0 )
	{
	}
};

DECL_PACKET( DBE_JACKPOT_EVENT_INIT_ACK )
{
	std::vector<KJackPotEventInitInfo>	m_vecJackpotEventInitInfo;
};

DECL_PACKET( DBE_JACKPOT_EVENT_UPDATE_ACK )
{
	int				m_iOK;
	UidType			m_iUnitUID;
	std::wstring	m_wstrCuponNumber;

	KDBE_JACKPOT_EVENT_UPDATE_ACK()
		: m_iOK( 0 )
		, m_iUnitUID( 0 )
	{
	}
};
#endif SERV_EVENT_JACKPOT
//}}

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
DECL_PACKET( DBE_NEW_ACCOUNT_QUEST_ACK )
{
	int									m_iOK;
	KDBE_NEW_QUEST_REQ					m_kQuestReq;

	KDBE_NEW_ACCOUNT_QUEST_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( DBE_ACCOUNT_QUEST_COMPLETE_ACK )
{
	int									m_iOK;
	KDBE_QUEST_COMPLETE_REQ				m_kQuestReq;
	__int64								m_tCompleteTime;

	KDBE_ACCOUNT_QUEST_COMPLETE_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( DBE_ACCOUNT_QUEST_UPDATE_NOT )
{
	UidType								m_iUserUID;
	std::vector< KQuestUpdate >			m_vecAccountQuesting;

	KDBE_ACCOUNT_QUEST_UPDATE_NOT()
	{
		m_iUserUID = 0;
	}
};
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}

//{{ 2012. 12. 19  교환(한정 수량) - 김민성
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
DECL_PACKET( DBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ )
{
	KEGS_ITEM_EXCHANGE_REQ				m_kReq;
	KDBE_ITEM_EXCHANGE_REQ				m_kToDB;
};

DECL_PACKET( DBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK )
{
	int								m_iOK;
	KDBE_ITEM_EXCHANGE_REQ			m_kDB;

	KDBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK()
	{
		m_iOK				 = 0;
	}
};
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
//}}

//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
DECL_PACKET( DBE_PET_AUTO_LOOTING_NOT )
{
	UidType		m_iPetUID;
	
	KDBE_PET_AUTO_LOOTING_NOT()
		: m_iPetUID( 0 )
	{
	}
};
#endif SERV_PET_AUTO_LOOTING
//}}

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
DECL_PACKET( ERM_BAD_ATTITUDE_USER_LOG_NOT )
{
	UidType				m_iUnitUID;
	int					m_iDungeonIDAndDif;
	short               m_sStartMember;
	short               m_sNowMember;
	short               m_sVoteCount;
	short               m_sExitStage;
	short               m_sExitSubStage;
	short               m_sBadAttitudePoint;
	std::wstring		m_wstrRegDate;
	bool				m_bExitResult;


	KERM_BAD_ATTITUDE_USER_LOG_NOT()
	{
		m_iUnitUID					= 0;
		m_iDungeonIDAndDif			= 0;
		m_sStartMember				= 0;
		m_sNowMember				= 0;
		m_sVoteCount				= 0;
		m_sExitStage				= 0;
		m_sExitSubStage				= 0;
		m_sBadAttitudePoint			= 0;
		m_bExitResult				= false;
	}
};
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 02. 	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
DECL_PACKET( DBE_TICK_PERFORMANCE_LOG_NOT )
{
	UidType				m_iServerUID;
	int					m_iServerType;
    DWORD				m_dwMaxTickCount;
	std::wstring		m_wstrMaxTickRegDate;
	std::wstring		m_wstrRegDate;

	KDBE_TICK_PERFORMANCE_LOG_NOT()
	{
		m_iServerUID = 0;
		m_iServerType = 0;
		m_dwMaxTickCount = 0;
	}
};
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
//}}

//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
typedef KFirstSelectUnitReward	KDBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT;
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
DECL_PACKET( DBE_ITEM_EXCHANGE_LOG_NOT )
{
	UidType						m_iUserUID;
	UidType						m_iUnitUID;
	char						m_cExchangeType;
	int							m_iSourceItemID;
	int							m_iSourceItemQuantity;
	std::map< int, int >		m_mapResultItem;
	std::wstring				m_wstrRegDate;

	KDBE_ITEM_EXCHANGE_LOG_NOT()
	{
		m_iUserUID				= 0;
		m_iUnitUID				= 0;
		m_cExchangeType			= 0;
		m_iSourceItemID			= 0;
		m_iSourceItemQuantity	= 0;
	}
};
#endif SERV_EXCHANGE_LOG
//}

//{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
DECL_PACKET( DBE_LOG_STATISTICS_INFO_NOT )
{
	std::vector< KLogStatInfo >	m_vecLogStat;
};
#endif SERV_LOG_SYSTEM_NEW
//}}

//{{ 2013. 2. 26	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
DECL_PACKET( DBE_UDP_CHECK_KICK_USER_NOT )
{
	UidType		m_iRoomUID;
	int			m_iPVPChannelClass;
	int			m_iMatchType;
	UidType		m_iUnitUID;

	KDBE_UDP_CHECK_KICK_USER_NOT()
		: m_iRoomUID( 0 )
		, m_iPVPChannelClass( 0 )
		, m_iMatchType( 0 )
		, m_iUnitUID( 0 )
	{
	}
};
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
DECL_PACKET( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK )
{
	int											m_iOK;
	int											m_iUID;
	std::wstring								m_wstrResetTime;
	std::map<UidType, KLocalRankingUnitInfo>	m_mapRankerUnitInfo;
	std::map<UidType, UidType>					m_mapRankerUIDInfo;
	std::map<UidType, KLocalRankingUserInfo>	m_mapRankerUserInfo;

	KDBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK()
		: m_iOK( 0 )
		, m_iUID( 0 )
	{
	}
};

typedef	KDBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK	KDBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_REQ;

DECL_PACKET( DBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ )
{
	UidType		m_iUserUID;
	UidType		m_iUnitUID;
	bool		m_bFiltered;

	KDBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ()
		: m_iUserUID( 0 )
		, m_iUnitUID( 0 )
		, m_bFiltered( true )
	{
	}
};

DECL_PACKET( DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK )
{
	int						m_iOK;
	int						m_iUID;
	KLocalRankingUserInfo	m_kUserInfo;
	std::wstring			m_wstrUpdated;
	KLocalRankingUnitInfo	m_kUnitInfo;
	bool					m_bFiltered;

	KDBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK()
		: m_iOK( 0 )
		, m_iUID( 0 )
		, m_bFiltered( true )
	{
	}
};

typedef	KDBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK	KDBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_REQ;

DECL_PACKET( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ )
{
	UidType		m_iUserUID;
	UidType		m_iUnitUID;

	UINT		m_nViewPage;
	char		m_cMainTabIndex;
	char		m_cSubTabIndex;
	byte		m_byteFilter;
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
	bool		m_bRankingButtonClick;
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

	KDBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ()
		: m_iUserUID( 0 )
		, m_iUnitUID( 0 )
		, m_nViewPage( 0 )
		, m_cMainTabIndex( -1 )
		, m_cSubTabIndex( -1 )
		, m_byteFilter( 0 )
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		, m_bRankingButtonClick( false )
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
	{
	}
};

DECL_PACKET( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK )
{
	int						m_iOK;
	int						m_iUID;
	KLocalRankingUserInfo	m_kUserInfo;
	std::wstring			m_wstrUpdated;
	KLocalRankingUnitInfo	m_kUnitInfo;

	UINT					m_nViewPage;
	char					m_cMainTabIndex;
	char					m_cSubTabIndex;
	byte					m_byteFilter;
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
	bool					m_bRankingButtonClick;
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK()
		: m_iOK( 0 )
		, m_iUID( 0 )
		, m_nViewPage( 0 )
		, m_cMainTabIndex( -1 )
		, m_cSubTabIndex( -1 )
		, m_byteFilter( 0 )
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		, m_bRankingButtonClick( false )
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
	{
	}
};

typedef	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ;

DECL_PACKET( DBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ )
{
	UidType		m_iUserUID;
	UidType		m_iUnitUID;

	int			m_iIncreasedSpirit;
	int			m_iIncreasedAP;
	bool		m_bForce;

	KDBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ()
		: m_iUserUID( 0 )
		, m_iUnitUID( 0 )
		, m_iIncreasedSpirit( 0 )
		, m_iIncreasedAP( 0 )
		, m_bForce( false )
	{
	}
};

DECL_PACKET( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK )
{
	int						m_iOK;
	int						m_iUID;
	KLocalRankingUserInfo	m_kUserInfo;
	std::wstring			m_wstrUpdated;
	KLocalRankingUnitInfo	m_kUnitInfo;

	int						m_iIncreasedSpirit;
	int						m_iIncreasedAP;
	bool					m_bForce;

	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK()
		: m_iOK( 0 )
		, m_iUID( 0 )
		, m_iIncreasedSpirit( 0 )
		, m_iIncreasedAP( 0 )
		, m_bForce( false )
	{
	}
};

typedef	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK	KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ;


typedef	KLocalRankingUserInfo	KDBE_LOCAL_RANKING_USER_INFO_WRITE_REQ;

DECL_PACKET( DBE_LOCAL_RANKING_USER_INFO_WRITE_ACK )
{
	int						m_iOK;
	KLocalRankingUserInfo	m_kInfo;
	std::wstring			m_wstrUpdated;

	KDBE_LOCAL_RANKING_USER_INFO_WRITE_ACK()
		: m_iOK( 0 )
	{
	}
};

DECL_PACKET( ELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT )
{
	int						m_iUID;
	KLocalRankingUserInfo	m_kUserInfo;
	KLocalRankingUnitInfo	m_kUnitInfo;

	KELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT()
		: m_iUID( 0 )
	{
	}
};

typedef	KELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT	KELG_LOCAL_RANKING_CHANGED_RANKER_INFO_CHECK_NOT;

DECL_PACKET( DBE_LOCAL_RANKING_RESET_ACK )
{
	int				m_iOK;
	int				m_iUID;
	std::wstring	m_wstrUpdated;

	KDBE_LOCAL_RANKING_RESET_ACK()
		: m_iOK( 0 )
		, m_iUID( 0 )
	{
	}
};

typedef	KDBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK	KELG_LOCAL_RANKING_RESET_NOT;
DECL_PACKET( DBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT )
{
	std::map<UidType, bool>	m_mapChangeInfo;

	KDBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT()
	{
	}
};

typedef	KEGS_LOCAL_RANKING_RANK_UP_NOT	KELG_LOCAL_RANKING_RANK_UP_NOT;

typedef	KDBE_GET_GUILD_INFO_ACK	KDBE_ACCOUNT_GET_GUILD_INFO_REQ;
typedef	KELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT	KELG_LOCAL_RANKING_GUILD_CHANGE_INFO_NOT;

DECL_PACKET( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ )
{
	__int64			m_iNexonSN;
	std::string		m_strPassword;

	KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ()
		: m_iNexonSN( 0 )
	{
	}
};

typedef	KPacketOK	KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_ACK;

DECL_PACKET( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ )
{
	__int64					m_iNexonSN;
	std::string				m_strPassword;
	KLocalRankingUserInfo	m_kInfo;

	KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ()
		: m_iNexonSN( 0 )
	{
	}
};

DECL_PACKET( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK )
{
	int						m_iOK;
	KLocalRankingUserInfo	m_kInfo;

	KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK()
		: m_iOK( 0 )
	{
	}
};
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
DECL_PACKET( DBE_RECOMMEND_USER_GET_NEXON_SN_REQ )
{
	UINT32					m_iRecommendUserNexonSN;
	UidType					m_iRecommendUnitUID;
	std::wstring			m_wstrRecommendedUserNickName;

	KDBE_RECOMMEND_USER_GET_NEXON_SN_REQ()
	{
		m_iRecommendUserNexonSN = 0;
		m_iRecommendUnitUID		= 0;
	}
};

DECL_PACKET( DBE_RECOMMEND_USER_GET_NEXON_SN_ACK )
{
	UINT32					m_iRecommendUserNexonSN;		// 추천하는 사람
	UidType					m_iRecommendUnitUID;
	UINT32					m_iRecommendedUserNexonSN;		// 추천받는 사람
	std::wstring			m_wstrRecommendedUserNickName;
#ifdef SERV_RECOMMEND_LIST_EVENT_2013_07	// 적용날짜: 2013-07-04
	int						m_iRecommendedUserChannelCode;	// 추천받는 사람
#endif SERV_RECOMMEND_LIST_EVENT_2013_07

	KDBE_RECOMMEND_USER_GET_NEXON_SN_ACK()
	{
		m_iRecommendUserNexonSN		= 0;
		m_iRecommendUnitUID			= 0;
		m_iRecommendedUserNexonSN	= 0;

#ifdef SERV_RECOMMEND_LIST_EVENT_2013_07	// 적용날짜: 2013-07-04
		m_iRecommendedUserChannelCode	= KNexonAccountInfo::CE_NONE;
#endif SERV_RECOMMEND_LIST_EVENT_2013_07
	}
};

DECL_PACKET( SOAP_CHECK_SAME_USER_REQ )
{
	int									m_iServiceCode;
	__int64								m_iNexonSN_A;
	__int64								m_iNexonSN_B;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;

	KSOAP_CHECK_SAME_USER_REQ()
	{
		m_iServiceCode	= 0;
		m_iNexonSN_A	= 0;
		m_iNexonSN_B	= 0;
		m_iUnitUID		= 0;
	}
};

DECL_PACKET( SOAP_CHECK_SAME_USER_ACK )
{
	int									m_iOK;
	UidType								m_iUnitUID;
	std::wstring						m_wstrNickName;

	KSOAP_CHECK_SAME_USER_ACK()
	{
		m_iOK		= 0;
		m_iUnitUID	= 0;
	}
};
#endif SERV_RECOMMEND_LIST_EVENT
//}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
DECL_PACKET( EGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT )
{
	UidType			m_iServerGroupID;
	std::wstring	m_wstrNickName;
	bool			m_bConnect;

	KEGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT()
		: m_iServerGroupID( -1 )
		, m_bConnect( false )
	{
	}
};
#endif	// SERV_HERO_PVP_MANAGE_LIST

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-05
DECL_PACKET( ELG_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	UidType	m_iRequestGSUID;
	UidType	m_iRequestUnitUID;
	UidType m_iTargetUnitUID;

	KELG_LOCAL_RANKING_WATCH_UNIT_REQ()
		: m_iRequestGSUID( 0 )
		, m_iRequestUnitUID( 0 )
		, m_iTargetUnitUID( 0 )
	{
	}
};

DECL_PACKET( ELG_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	int						m_iOK;
	KLocalRankingUserInfo	m_kUserInfo;
	KLocalRankingUnitInfo	m_kUnitInfo;

	UidType	m_iRequestGSUID;
	UidType	m_iRequestUnitUID;

	KELG_LOCAL_RANKING_WATCH_UNIT_ACK()
		: m_iOK( -1 )
		, m_iRequestGSUID( 0 )
		, m_iRequestUnitUID( 0 )
	{
	}
};

typedef	KELG_LOCAL_RANKING_WATCH_UNIT_ACK			KDBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_ACK;
typedef	KDBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_ACK	KDBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_REQ;

typedef	KELG_LOCAL_RANKING_WATCH_UNIT_REQ	KERM_LOCAL_RANKING_WATCH_UNIT_REQ;
typedef	KELG_LOCAL_RANKING_WATCH_UNIT_ACK	KERM_LOCAL_RANKING_WATCH_UNIT_ACK;

DECL_PACKET( ELG_LOCAL_RANKING_SYSTEM_CHECK_NOT )
{
	int	m_iMainTabIndex;
	int m_iSubTabIndex;

	KELG_LOCAL_RANKING_SYSTEM_CHECK_NOT()
		: m_iMainTabIndex( 0 )
		, m_iSubTabIndex( 0 )
	{
	}
};
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
DECL_PACKET( ERM_PREPARE_FOR_DEFENCE_DUNGEON_REQ )
{
	KDungeonGameInfo						m_kDefenceDungeonInfo;
};

DECL_PACKET( ERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK )
{
	KRoomUserInfo							m_kRoomUserInfo;
};

DECL_PACKET( ERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT )
{
	int										m_iDefenceDungeonPhase;

	KERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT()
	{
		m_iDefenceDungeonPhase = 0;
	}
};
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
DECL_PACKET( DBE_LOAD_WEDDING_HALL_INFO_ACK )
{
	int										m_iOK;
	std::map<int, KWeddingHallInfo>			m_mapWeddingInfo;

	KDBE_LOAD_WEDDING_HALL_INFO_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( DBE_WEDDING_HALL_INFO_UPDATE_NOT )
{
	std::map<int, KWeddingHallInfo>			m_mapWeddingInfo;

	KDBE_WEDDING_HALL_INFO_UPDATE_NOT()
	{
	}
};

DECL_PACKET( DBE_COUPLE_PROPOSE_USER_FIND_ACK )
{
	int										m_iOK;
	std::wstring							m_wstrOtherNickName;
	UidType									m_iOtherUserUID;
	UidType									m_iOtherUnitUID;
	char									m_cUnitClass;
	char									m_cRelationshipType;
	
	KDBE_COUPLE_PROPOSE_USER_FIND_ACK()
	{
		m_iOK = 0;
		m_iOtherUserUID = 0;
		m_iOtherUnitUID = 0;
		m_cUnitClass = 0;
		m_cRelationshipType = 0;
	}
};

DECL_PACKET( ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ )
{
	UidType					m_iOtherUserUID;
	UidType					m_iOtherUnitUID;
	std::wstring			m_wstrRequestUnitName;
	UidType					m_iRequestUserUID;
	UidType					m_iRequestUnitUID;
	std::wstring			m_wstrAcceptUnitName;

	KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ()
	{
		m_iOtherUserUID = 0;
		m_iOtherUnitUID = 0;
		m_iRequestUserUID = 0;
	}
};

DECL_PACKET( ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK )
{
	int						m_iOK;
	std::wstring			m_wstrNickName;

	KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( ELG_COUPLE_PROPOSE_NOT )
{
	std::wstring			m_wstrRequestUnitName;
	UidType					m_iRequestUnitUID;

	KELG_COUPLE_PROPOSE_NOT()
	{
		m_iRequestUnitUID = 0;
	}
};

DECL_PACKET( ERM_COUPLE_PROPOSE_RESULT_FAIL_NOT )
{
	int							m_iOK;
	UidType						m_iRequestUnitUID;
	std::wstring				m_wstrRequestUnitName;
	UidType						m_iAcceptUnitUID;
	std::wstring				m_wstrAcceptUnitName;

	KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT()
	{
		m_iOK = 0;
		m_iRequestUnitUID = 0;
		m_iAcceptUnitUID = 0;
	}
};

DECL_PACKET( ERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT )
{
	int							m_iOK;
	UidType						m_iRequestUnitUID;
	std::wstring				m_wstrRequestUnitName;
	UidType						m_iAcceptUnitUID;
	std::wstring				m_wstrAcceptUnitName;
	__int64						m_tDate;

	KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT()
	{
		m_iOK = 0;
		m_iRequestUnitUID = 0;
		m_iAcceptUnitUID = 0;
		m_tDate = 0;
	}
};

DECL_PACKET( DBE_COUPLE_MAKING_SUCCESS_REQ )
{
	UidType								m_iRequestUnitUID;
	std::wstring						m_wstrRequestUnitName;

	UidType								m_iAcceptUserUID;
	u_char								m_ucAcceptUnitLevel;
	char								m_cAcceptUnitClass;
	UidType								m_iAcceptUnitUID;
	std::wstring						m_wstrAcceptUnitName;

	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KDeletedItemInfo >		m_vecDeleted;

	KDBE_COUPLE_MAKING_SUCCESS_REQ()
	{
		m_iAcceptUserUID = 0;
		m_ucAcceptUnitLevel = 0;
		m_cAcceptUnitClass = 0;
		m_iRequestUnitUID = 0;
		m_iAcceptUnitUID = 0;
	}
};

DECL_PACKET( DBE_COUPLE_MAKING_SUCCESS_ACK )
{
	int									m_iOK;
	UidType								m_iRequestUnitUID;
	std::wstring						m_wstrRequestUnitName;
	
	UidType								m_iAcceptUserUID;
	u_char								m_ucAcceptUnitLevel;
	char								m_cAcceptUnitClass;
	UidType								m_iAcceptUnitUID;
	std::wstring						m_wstrAcceptUnitName;

	__int64								m_tDate;

	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_COUPLE_MAKING_SUCCESS_ACK()
	{
		m_iOK = 0;
		m_iRequestUnitUID = 0;
		m_iAcceptUserUID = 0;
		m_ucAcceptUnitLevel = 0;
		m_cAcceptUnitClass = 0;
		m_iAcceptUnitUID = 0;
		m_tDate = 0;
	}
};

DECL_PACKET( ERM_COUPLE_MAKING_SUCCESS_NOT )
{
	UidType								m_iRequestUnitUID;
	std::wstring						m_wstrRequestUnitName;
	UidType								m_iAcceptUnitUID;
	std::wstring						m_wstrAcceptUnitName;

	KERM_COUPLE_MAKING_SUCCESS_NOT()
	{
		m_iRequestUnitUID = 0;
		m_iAcceptUnitUID = 0;
	}
};

DECL_PACKET( ETR_COUPLE_MAKING_SUCCESS_NOT )
{
	std::vector< UidType >				m_vecUID;

	std::wstring						m_wstrRequestUnitName;
	std::wstring						m_wstrAcceptUnitName;

	KETR_COUPLE_MAKING_SUCCESS_NOT()
	{
	}
};

DECL_PACKET( ELG_RELATIONSHIP_INFO_REQ )
{
	UidType								m_iRequestGSUID;
	UidType								m_iRequestUnitUID;
	UidType								m_iAcceptUnitUID;

	KELG_RELATIONSHIP_INFO_REQ()
	{
		m_iRequestGSUID = 0;
		m_iRequestUnitUID = 0;
		m_iAcceptUnitUID = 0;
	}
};

DECL_PACKET( ETR_RELATIONSHIP_EFFECT_NOT )
{
	std::vector< UidType >				m_vecUID;
	UidType								m_iManUID;					// 남
	UidType								m_iGirlUID;					// 여
	std::wstring						m_wstrLoveWord;	// 애칭
	int									m_iEffectIndex;
	KETR_RELATIONSHIP_EFFECT_NOT()
	{
		m_iManUID = 0;
		m_iGirlUID = 0;
		m_iEffectIndex = 0;
	}
};

DECL_PACKET( DBE_WEDDING_PROPOSE_REQ )
{
	UidType								m_iUnitUID;					// 캐시 아이템 사용자
	UidType								m_iManUID;					// 남
	UidType								m_iGirlUID;					// 여
	char								m_cWeddingHallType;			// 결혼식장 타입
	char								m_cOfficiantNPC;			// 주례npc 타입
	std::wstring						m_wstrWeddingDate;			// 결혼 일시
	std::wstring						m_wstrWeddingMsg;			// 모시는 글
	std::wstring						m_wstrWeddingNickName;			// Nickname
	std::vector<UidType>				m_vecInviteUnitList;

	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_WEDDING_PROPOSE_REQ()
	{
		m_iUnitUID = 0;
		m_iManUID = 0;
		m_iGirlUID = 0;
		m_cWeddingHallType = 0;
		m_cOfficiantNPC = 0;
	}
};

DECL_PACKET( DBE_WEDDING_PROPOSE_ACK )
{
	int									m_iOK;

	int									m_iWeddingUID;
	UidType								m_iManUID;					// 남
	UidType								m_iGirlUID;					// 여
	char								m_cWeddingHallType;			// 결혼식장 타입
	char								m_cOfficiantNPC;			// 주례npc 타입
	std::wstring						m_wstrWeddingDate;			// 결혼 일시
	std::wstring						m_wstrWeddingMsg;			// 모시는 글

	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_WEDDING_PROPOSE_ACK()
	{
		m_iOK = 0;
		m_iWeddingUID = 0;
		m_iManUID = 0;
		m_iGirlUID = 0;
		m_cWeddingHallType = 0;
		m_cOfficiantNPC = 0;
	}
};

DECL_PACKET( ELG_ADD_WEDDING_HALL_INFO_NOT )
{
	KWeddingHallInfo					m_kWeddingHallInfo;

	KELG_ADD_WEDDING_HALL_INFO_NOT()
	{
	}
};

DECL_PACKET( ELG_WEDDING_ITEM_FIND_INFO_REQ )
{
	int											m_iOK;
	UidType										m_iUnitUID;
	std::map< UidType, KWeddingItemInfo >		m_mapWeddingItemInfo;

	KELG_WEDDING_ITEM_FIND_INFO_REQ()
	{
		m_iUnitUID = 0;
		m_iOK = 0;
	}
};

DECL_PACKET( DBE_WEDDING_ITEM_DELETE_REQ )
{
	UidType								m_iUnitUID;
	std::map< UidType, KWeddingItemInfo >	m_mapWeddingItemInfo;

	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KDeletedItemInfo >		m_vecDeleted;

	KDBE_WEDDING_ITEM_DELETE_REQ()
	{
		m_iUnitUID = 0;
	}
};

DECL_PACKET( DBE_WEDDING_ITEM_DELETE_ACK )
{
	int										m_iOK;
	std::map< UidType, KWeddingItemInfo >	m_mapWeddingItemInfo;

	std::vector< KInventoryItemInfo >		m_vecUpdatedInventorySlot;
	KItemQuantityUpdate						m_kItemQuantityUpdate;

	KDBE_WEDDING_ITEM_DELETE_ACK()
	{
		m_iOK = 0;
	}
};


DECL_PACKET( ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ )
{
	int								m_iMapID;
	VECTOR3							m_vPos;
	unsigned char					m_LastTouchLineIndex;

	UidType							m_iCalledUserUID;
	UidType							m_iCalledGSUID;
	int								m_iChannelID;

	UidType							m_iLoverUnitUID;
	char							m_cCallType;

	KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ()
	{
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
		m_iChannelID = 0;
		m_iLoverUnitUID = 0;
		m_cCallType = 0;
	}
};

typedef			KPacketOK			KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK;

DECL_PACKET( ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT )
{
	bool						m_bAgree;
	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;
	UidType						m_iCalledUserUID;
	UidType						m_iCalledGSUID;
	UidType						m_iMyUserUID;
	UidType						m_iMyGSUID;

	KELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT()
	{
		m_bAgree = false;
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
		m_iMyUserUID = 0;
		m_iMyGSUID = 0;
	}
};

DECL_PACKET( ELG_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT )
{
	bool						m_bUseWarpItem;
	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	KELG_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT()
	{
		m_bUseWarpItem = false;
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
	}
};

DECL_PACKET( ERM_CALL_MY_LOVER_LEAVE_FIELD_REQ )
{
	int							m_iReason;
	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;
	int							m_iLimitLevel;
	int							m_iRequireDungeonID;

	KERM_CALL_MY_LOVER_LEAVE_FIELD_REQ()
	{
		m_iReason = 0;
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
		m_iLimitLevel = 0;
		m_iRequireDungeonID = 0;
	}
};

DECL_PACKET( ERM_CALL_MY_LOVER_LEAVE_FIELD_ACK )
{
	int							m_iOK;
	int							m_iReason;
	int							m_iMapID;
	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;
	int							m_iLimitLevel;
	int							m_iRequireDungeonID;

	KERM_CALL_MY_LOVER_LEAVE_FIELD_ACK()
	{
		m_iOK = 0;
		m_iReason = 0;
		m_iMapID = 0;
		m_LastTouchLineIndex = 0;
		m_iLimitLevel = 0;
		m_iRequireDungeonID = 0;
	}
};

DECL_PACKET( ERM_CALL_MY_LOVER_CHECK_ROOM_REQ )
{
	VECTOR3							m_vPos;
	unsigned char					m_LastTouchLineIndex;

	UidType							m_iLoverUnitUID;
	char							m_cCallType;

	int								m_iChannelID;
	UidType							m_iCalledUserUID;
	UidType							m_iCalledGSUID;
    	
	KERM_CALL_MY_LOVER_CHECK_ROOM_REQ()
	{
		m_LastTouchLineIndex = 0;
		m_iLoverUnitUID = 0;
		m_cCallType = 0;
		m_iChannelID = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
	}
};

DECL_PACKET( ERM_CALL_MY_LOVER_CHECK_ROOM_ACK )
{
	int							m_iOK;

	KBattleFieldRoomInfo		m_kBTInfo;

	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	UidType						m_iLoverUnitUID;
	char						m_cCallType;

	KERM_CALL_MY_LOVER_CHECK_ROOM_ACK()
	{
		m_iOK = 0;
		m_LastTouchLineIndex = 0;
		m_iLoverUnitUID = 0;
		m_cCallType = 0;
	}
};

DECL_PACKET( ELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ )
{
	int							m_iOK;

	KBattleFieldRoomInfo		m_kBTInfo;

	VECTOR3						m_vPos;
	unsigned char				m_LastTouchLineIndex;

	UidType						m_iLoverUnitUID;
	char						m_cCallType;

	int							m_iChannelID;
	UidType						m_iCalledUserUID;
	UidType						m_iCalledGSUID;

	KELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ()
	{
		m_iOK = 0;
		m_LastTouchLineIndex = 0;
		m_iLoverUnitUID = 0;
		m_cCallType = 0;
		m_iChannelID = 0;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
	}
};

typedef			KPacketOK			KELG_CALL_MY_LOVER_FIND_USER_ROOM_ACK;

DECL_PACKET( ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT )
{
	bool								m_bAgree;
	UidType								m_iCalledUserUID;
	UidType								m_iCalledGSUID;

	UidType								m_iMyUserUID;
	UidType								m_iMyGSUID;
	int									m_iBattleFieldID;
	UidType								m_RoomUID;

	VECTOR3								m_vPos;
	unsigned char						m_LastTouchLineIndex;

	KELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT()
	{
		m_bAgree = false;
		m_iCalledUserUID = 0;
		m_iCalledGSUID = 0;
		m_iMyUserUID = 0;
		m_iMyGSUID = 0;
		m_iBattleFieldID = 0;
		m_RoomUID = 0;
		m_LastTouchLineIndex = 0;
	}
};

DECL_PACKET( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT )
{
	UidType								m_iCalledUserUID;
	UidType								m_iCalledUnitUID;
	UidType								m_iCalledGSUID;

	int									m_iBattleFieldID;
	UidType								m_RoomUID;

	VECTOR3								m_vPos;
	unsigned char						m_LastTouchLineIndex;

	KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT()
	{
		m_iCalledUserUID = 0;
		m_iCalledUnitUID = 0;
		m_iCalledGSUID = 0;
		m_iBattleFieldID = 0;
		m_RoomUID = 0;
		m_LastTouchLineIndex = 0;
	}
};

DECL_PACKET( ERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ )
{
	UidType								m_iCalledUserUID;
	UidType								m_iCalledUnitUID;
	UidType								m_iCalledGSUID;

	UidType								m_RoomUID;

	VECTOR3								m_vPos;

	KRoomUserInfo                       m_kRoomUserInfo;
	std::vector< UidType >				m_vecStudentUnitUID;	//사제시스템중 제자리스트	
	KBattleFieldJoinInfo				m_kBattleFieldJoinInfo;
	KReturnToFieldInfo					m_kReturnToFieldInfo;	// 필드로 복귀에 필요한 정보

	KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ()
	{
		m_iCalledUserUID = 0;
		m_iCalledUnitUID = 0;
		m_iCalledGSUID = 0;
		m_RoomUID = 0;
	}
};

DECL_PACKET( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT )
{
	int									m_iOK;
	UidType								m_iCalledUserUID;
	UidType								m_iCalledGSUID;

	KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT()
	{
		m_iOK = 0;
		m_iCalledUserUID = 0;		
		m_iCalledGSUID = 0;
	}
};

DECL_PACKET( ERM_WEDDING_HALL_INFO_NOT )
{
	enum INFO_TYPE
	{
		IT_INSERT = 1,
		IT_UPDATE,
		IT_DELETE,
	};

	char								m_cType;
	KWeddingHallInfo					m_kHallInfo;
	std::vector< UidType >				m_vecJoinUnitList;

	KERM_WEDDING_HALL_INFO_NOT()
	{
		m_cType = 0;
	}
};

DECL_PACKET( ELG_CHECK_WEDDING_HALL_INFO_REQ )
{
	UidType								m_iGSUID;
	UidType								m_iGSUserUID;

	int									m_iWeddingUID;
	int									m_StartPosIndex;
	char								m_cUnitWeddingStatus;

	KELG_CHECK_WEDDING_HALL_INFO_REQ()
	{
		m_iGSUID = 0;
		m_iGSUserUID = 0;
		m_iWeddingUID = 0;
		m_cUnitWeddingStatus = 0;
	}
};

DECL_PACKET( ELG_CHECK_WEDDING_HALL_INFO_ACK )
{
	int										m_iOK;
	KWeddingHallInfo						m_kWeddingInfo;
	char									m_cUnitWeddingStatus;

	KELG_CHECK_WEDDING_HALL_INFO_ACK()
	{
		m_iOK = 0;
		m_cUnitWeddingStatus = 0;
	}
};


DECL_PACKET( ERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ )
{
	KWeddingHallInfo						m_kWeddingInfo;
	char									m_cUnitWeddingStatus;

	KERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ()
	{
		m_cUnitWeddingStatus = 0;
	}
};


DECL_PACKET( ERM_OPEN_WEDDING_HALL_REQ )
{
	KWeddingHallInfo						m_kWeddingInfo;
	KRoomUserInfo							m_kRoomUserInfo;
	KReturnToFieldInfo						m_kReturnToFieldInfo;

	KERM_OPEN_WEDDING_HALL_REQ()
	{
	}
};


DECL_PACKET( ERM_OPEN_WEDDING_HALL_ACK )
{
	int                                 m_iOK;

	KWeddingHallInfo					m_kWeddingInfo;
	KRoomInfo							m_RoomInfo;
	std::vector< KRoomSlotInfo >        m_vecSlot;
	std::wstring						m_wstrCNIP;

	//{{ 핑 상태체크 호스트 변경 - 김민성
	std::wstring						m_wstrUDPRelayIP;
	USHORT								m_usUDPRelayPort;

	KERM_OPEN_WEDDING_HALL_ACK()
	{
		m_iOK = 0;
		m_usUDPRelayPort = 0;
	}
};

typedef KERM_OPEN_WEDDING_HALL_REQ	KERM_JOIN_WEDDING_HALL_REQ;

typedef KERM_OPEN_WEDDING_HALL_ACK	KERM_JOIN_WEDDING_HALL_ACK;


DECL_PACKET( DBE_BREAK_UP_REQ )
{
	UidType								m_iMyUnitUID;
	UidType								m_iLoverUnitUID;
	int									m_iCost;

	KDBE_BREAK_UP_REQ()
	{
		m_iMyUnitUID = 0;
		m_iLoverUnitUID = 0;
		m_iCost = 0;
	}
};

DECL_PACKET( DBE_BREAK_UP_ACK )
{
	int									m_iOK;
	UidType								m_iMyUnitUID;
	UidType								m_iLoverUnitUID;
	int									m_iCost;

	KDBE_BREAK_UP_ACK()
	{
		m_iOK = 0;
		m_iMyUnitUID = 0;
		m_iLoverUnitUID = 0;
		m_iCost = 0;
	}
};

DECL_PACKET( ELG_BREAK_UP_NOT )
{
	UidType								m_iLoverUnitUID;

	KELG_BREAK_UP_NOT()
	{
		m_iLoverUnitUID = 0;
	}
};

typedef KPacketOK	KERM_CHECK_START_WEDDING_ACK;

DECL_PACKET( DBE_WEDDING_COMPLETE_REQ )
{
	int							m_iWeddingUID;
	UidType						m_iGroom;
	UidType						m_iBride;
	std::wstring				m_wstrWeddingDate;
	
	KDBE_WEDDING_COMPLETE_REQ()
	{
		m_iWeddingUID = 0;
		m_iGroom = 0;
		m_iBride = 0;
	}
};

DECL_PACKET( DBE_WEDDING_COMPLETE_ACK )
{
	int							m_iOK;
	std::wstring				m_wstrWeddingDate;

	KDBE_WEDDING_COMPLETE_ACK()
	{
		m_iOK = 0;
	}
};

DECL_PACKET( ERM_START_WEDDING_NOT )
{
	int							m_iOK;
	UidType						m_iGroom;
	UidType						m_iBride;
	std::map< int, int >		m_mapRewardItem;

	KERM_START_WEDDING_NOT()
	{
		m_iOK = 0;
		m_iGroom = 0;
		m_iBride = 0;
	}
};

DECL_PACKET( DBE_CHANGE_LOVE_WORD_REQ )
{
	UidType						m_iUseUnitUID;
	UidType						m_iLoverUnitUID;
	std::wstring				m_wstrNewLoveWord;

	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;

	KDBE_CHANGE_LOVE_WORD_REQ()
	{
		m_iUseUnitUID = 0;
		m_iLoverUnitUID = 0;
	}
};

DECL_PACKET( DBE_CHANGE_LOVE_WORD_ACK )
{
	int							m_iOK;
	UidType						m_iUseUnitUID;
	UidType						m_iLoverUnitUID;
	std::wstring				m_wstrNewLoveWord;

	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_CHANGE_LOVE_WORD_ACK()
	{
		m_iOK = 0;
		m_iUseUnitUID = 0;
		m_iLoverUnitUID = 0;
	}
};

DECL_PACKET( ELG_CHANGE_LOVE_WORD_NOT )
{
	UidType						m_iUseUnitUID;
	UidType						m_iLoverUnitUID;
	std::wstring				m_wstrNewLoveWord;

	KELG_CHANGE_LOVE_WORD_NOT()
	{
		m_iUseUnitUID = 0;
		m_iLoverUnitUID = 0;
	}
};

DECL_PACKET( DBE_INSERT_WEDDING_REWARD_REQ )
{
	UidType								m_iUnitUID;
	UidType								m_iLoverUnitUID;
	bool								m_bTitleReward;
	int									m_iTitleRewardStep;

	KDBE_INSERT_REWARD_TO_POST_REQ		kPostReq;

	KDBE_INSERT_WEDDING_REWARD_REQ()
	{
		m_iUnitUID = 0;
		m_iLoverUnitUID = 0;
		m_bTitleReward = false;
		m_iTitleRewardStep = 0;
	}
};

DECL_PACKET( DBE_INSERT_WEDDING_REWARD_ACK )
{
	int									m_iOK;
	bool								m_bTitleReward;
	int									m_iTitleRewardStep;

	KDBE_INSERT_WEDDING_REWARD_ACK()
	{
		m_iOK = 0;
		m_bTitleReward = false;
		m_iTitleRewardStep = 0;
	}
};

DECL_PACKET( DBE_ADMIN_CHANGE_COUPLE_DATE_REQ )
{
	float						m_fAddRate;
	UidType						m_iUnitUID;
	UidType						m_iLoverUnitUID;

	KDBE_ADMIN_CHANGE_COUPLE_DATE_REQ()
	{
		m_fAddRate = 0.f;
		m_iUnitUID = 0;
		m_iLoverUnitUID = 0;
	}
};

DECL_PACKET( DBE_WRITE_RELATIONSHIP_INFO_LOG_REQ )
{
	UidType						m_iUserUID;
	UidType						m_iUnitUID;
	u_char						m_ucLevel;
	char						m_cUnitClass;
	char						m_cCoupleType;
	std::wstring				m_wstrRegDate;

	KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ()
	{
		m_iUnitUID = 0;
		m_ucLevel = 0;
		m_cUnitClass = 0;
		m_cCoupleType = 0;
	}
};

DECL_PACKET( DBE_WRITE_RELATIONSHIP_INFO_LOG_ACK )
{
	int				m_iOK;

	KDBE_WRITE_RELATIONSHIP_INFO_LOG_ACK()
	{
		m_iOK = 0;
	}
};
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
DECL_PACKET( ECN_SET_ACTIVE_LAGCHECK_NOT )
{
	bool m_bActiveLagCheck;

	KECN_SET_ACTIVE_LAGCHECK_NOT()
	{
		m_bActiveLagCheck = false;
	}
};
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
DECL_PACKET( DBE_GET_RIDING_PET_LIST_REQ )
{
	UidType		m_iUnitUID;
	int			m_iAfterWorkStorageKey;

	KDBE_GET_RIDING_PET_LIST_REQ()
		: m_iUnitUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
	{
	}
};

DECL_PACKET( DBE_GET_RIDING_PET_LIST_ACK )
{
	int								m_iOK;
	int								m_iAfterWorkStorageKey;
	std::vector<KRidingPetInfo>		m_vecRidingPetList;

	KDBE_GET_RIDING_PET_LIST_ACK()
		: m_iOK( 0 )
		, m_iAfterWorkStorageKey( 0 )
	{
	}
};

DECL_PACKET( DBE_CREATE_RIDING_PET_REQ )
{
	UidType								m_iUnitUID;
	USHORT								m_usCreatePetID;
	short								m_sPeriod;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_CREATE_RIDING_PET_REQ()
		: m_iUnitUID( 0 )
		, m_usCreatePetID( 0 )
		, m_sPeriod( 0 )
	{
	}
};

DECL_PACKET( DBE_CREATE_RIDING_PET_ACK )
{
	int									m_iOK;
	KRidingPetInfo						m_kCreatedPetInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_CREATE_RIDING_PET_ACK()
		: m_iOK( 0 )
	{
	}
};

DECL_PACKET( DBE_RELEASE_RIDING_PET_REQ )
{
	UidType		m_iRidingPetUID;
	UidType		m_iUnitUID;
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
	bool		m_bEventQuest;
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST

	KDBE_RELEASE_RIDING_PET_REQ()
		: m_iRidingPetUID( 0 )
		, m_iUnitUID( 0 )
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
		,m_bEventQuest( false )
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST
	{
	}
};

DECL_PACKET( ETR_SUMMON_RIDING_PET_NOT )
{
	std::vector<UidType>	m_vecUID;
	UidType					m_iUnitUID;
	UidType					m_iRidingPetUID;
	USHORT					m_usRidingPetID;

	KETR_SUMMON_RIDING_PET_NOT()
		: m_iUnitUID( 0 )
		, m_iRidingPetUID( 0 )
		, m_usRidingPetID( 0 )
	{
	}
};

DECL_PACKET( ETR_UNSUMMON_RIDING_PET_NOT )
{
	std::vector<UidType>				m_vecUID;
	UidType								m_iUnitUID;

	KETR_UNSUMMON_RIDING_PET_NOT()
		: m_iUnitUID( 0 )
	{
	}
};
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// 작업날짜: 2013-05-10	// 박세훈
DECL_PACKET( DBE_SEND_LOVE_LETTER_EVENT_REQ )
{
	UidType                             m_iUnitUID;
	std::wstring						m_wstrMessage;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_SEND_LOVE_LETTER_EVENT_REQ()
		: m_iUnitUID( 0 )
	{
	}
};

DECL_PACKET( DBE_SEND_LOVE_LETTER_EVENT_ACK )
{
	int									m_iOK;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_SEND_LOVE_LETTER_EVENT_ACK()
		: m_iOK( 0 )
	{
	}
};
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

//{{ 2013. 05. 10	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
DECL_PACKET( DBE_ITEM_EVALUATE_REQ )
{
	UidType                             m_iUnitUID;
	UidType								m_iItemUID;
	int									m_iCost;
	std::vector< int >					m_vecRandomSocket;

	KDBE_ITEM_EVALUATE_REQ()
	{
		m_iUnitUID = 0;
		m_iItemUID = 0;
		m_iCost = 0;
	}
};

DECL_PACKET( DBE_ITEM_EVALUATE_ACK )
{
	int                                 m_iOK;
	UidType								m_iItemUID;
	int									m_iCost;
	std::vector< int >					m_vecRandomSocket;

	KDBE_ITEM_EVALUATE_ACK()
	{
		m_iItemUID = 0;
		m_iCost = 0;
	}
};

DECL_PACKET( DBE_RESTORE_ITEM_EVALUATE_REQ )
{
	UidType                             m_iUnitUID;
	UidType								m_iTargetItemUID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_RESTORE_ITEM_EVALUATE_REQ()
	{
		m_iUnitUID = 0;
		m_iTargetItemUID = 0;
	}
};

DECL_PACKET( DBE_RESTORE_ITEM_EVALUATE_ACK )
{
	int                                 m_iOK;
	UidType								m_iTargetItemUID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_RESTORE_ITEM_EVALUATE_ACK()
	{
		m_iOK = 0;
		m_iTargetItemUID = 0;
	}
};

DECL_PACKET( DBE_ITEM_CONVERT_REQ )
{
	UidType                             m_iUnitUID;
	int									m_iCommissionED;
	std::vector< KItemInfo >            m_vecItemInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_ITEM_CONVERT_REQ()
	{
		m_iUnitUID = 0;
		m_iCommissionED = 0;
	}
};

DECL_PACKET( DBE_ITEM_CONVERT_ACK )
{
	int                                 m_iOK;
	int									m_iCommissionED;
	std::map< UidType, KItemInfo >      m_mapItemInfo;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_ITEM_CONVERT_ACK()
	{
		m_iOK = 0;
		m_iCommissionED = 0;
	}
};
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
DECL_PACKET( DBE_LOCAL_RANKING_SYSTEM_STATISTICS_LOG_NOT )
{
	UidType			m_iUserUID;
	UidType			m_iUnitUID;
	unsigned char	m_ucLevel;
	bool			m_bLocalRankingUser;
	unsigned short	m_usLocalRankingClickCount;

	KDBE_LOCAL_RANKING_SYSTEM_STATISTICS_LOG_NOT()
		: m_iUserUID( 0 )
		, m_iUnitUID( 0 )
		, m_ucLevel( 0 )
		, m_bLocalRankingUser( false )
		, m_usLocalRankingClickCount( 0 )
	{
	}
};
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// 작업날짜: 2013-05-15	// 박세훈
DECL_DATA( DungonStageLoadLog )
{
	UidType	m_iUnitUID;
	char	m_cNormalStage;
	char	m_cSecretStage;
	char	m_cEliteStage;
	char	m_cClearConditionCheckCount;
	int		m_iDungeonID;
	int		m_iNowStageID;
	int		m_iReqStageID;
	bool	m_bHost;
	char	m_cMemberNum;

	DungonStageLoadLog()
		: m_iUnitUID( 0 )
		, m_cNormalStage( 0 )
		, m_cSecretStage( 0 )
		, m_cEliteStage( 0 )
		, m_cClearConditionCheckCount( 0 )
		, m_iDungeonID( 0 )
		, m_iNowStageID( 0 )
		, m_iReqStageID( 0 )
		, m_bHost( false )
		, m_cMemberNum( 0 )
	{
	}
};

DECL_PACKET( DBE_DUNGEON_STAGE_LOAD_LOG_NOT )
{
	UidType								m_iRoomUID;
	std::vector<DungonStageLoadLog>		m_vecDungonStageLoadLog;

	KDBE_DUNGEON_STAGE_LOAD_LOG_NOT()
		: m_iRoomUID( 0 )
	{
	}
};
#endif // SERV_DUNGEON_STAGE_LOAD_LOG

#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG// 작업날짜: 2013-05-15	// 박세훈
DECL_PACKET( DBE_FIELD_WORKINGS_BLOCK_LOG_NOT )
{
	UidType	m_iUnitUID;
	char	m_cX2State;
	int		m_iPhysicalMemory;
	int		m_iVirtualMemory;
	char	m_cProcessTerm;
	char	m_cFieldUserNum;
	char	m_cFieldNpcNum;

	KDBE_FIELD_WORKINGS_BLOCK_LOG_NOT()
		: m_iUnitUID( 0 )
		, m_cX2State( 0 )
		, m_iPhysicalMemory( 0 )
		, m_iVirtualMemory( 0 )
		, m_cProcessTerm( 0 )
		, m_cFieldUserNum( 0 )
		, m_cFieldNpcNum( 0 )
	{
	}
};
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-22	// 박세훈
DECL_PACKET( DBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT )
{
	byte			m_byteBlockCheckCount;
	std::wstring	m_wstrBlockCheckResetDate;

	KDBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT()
		: m_byteBlockCheckCount( 0 )
	{
	}
};

typedef	KDBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT	KDBE_BLOCK_COUNT_CHECK_INFO_READ_ACK;
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
DECL_DATA( KTimeEnchantEventInfo )
{
	int			m_iEventUID;
	__time64_t	m_tBeginDate;
	__time64_t	m_tEndDate;
	char		m_cEnchantLevel;

	KTimeEnchantEventInfo()
		: m_iEventUID( 0 )
		, m_tBeginDate( 0 )
		, m_tEndDate( 0 )
		, m_cEnchantLevel( 0 )
	{
	}
};

DECL_PACKET( DBE_TIME_ENCHANT_EVENT_INFO_ACK )
{
	std::vector<KTimeEnchantEventInfo>	m_vecTimeEnchantEventInfo;	// <m_iEventUID, KTimeEnchantEventInfo>
};

DECL_PACKET( DBE_TIME_ENCHANT_EVENT_INFO_NOT )
{
	int						m_iTimeEnchantEventUID;
	KTimeEnchantEventInfo	m_kTimeEnchantEventInfo;

	KDBE_TIME_ENCHANT_EVENT_INFO_NOT()
		: m_iTimeEnchantEventUID( 0 )
	{
	}
};

DECL_PACKET( EGB_TIME_ENCHANT_EVENT_INFO_NOT )
{
	int	m_iTimeEnchantEventUID;
	int	m_iNumOfTimeEnchantEvent;

	KEGB_TIME_ENCHANT_EVENT_INFO_NOT()
		: m_iTimeEnchantEventUID( 0 )
		, m_iNumOfTimeEnchantEvent( 0 )
	{
	}
};

typedef	KEGB_TIME_ENCHANT_EVENT_INFO_NOT	KEGB_TIME_ENCHANT_EVENT_INFO_ACK;

DECL_DATA( KOpenRandomItemStorage )
{
	int									m_iRessurectionCount;
	int									m_iRestoreSpirit;
	int									m_iRandomItemID;
	bool								m_bIsCharmItem;
	int									m_iSpendED;
	std::map< int, int >                m_mapInsertedItem;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;

	KOpenRandomItemStorage()
		: m_iRessurectionCount( 0 )
		, m_iRestoreSpirit( 0 )
		, m_iRandomItemID( 0 )
		, m_bIsCharmItem( false )
		, m_iSpendED( 0 )
	{
	}
};

DECL_PACKET( EGB_TIME_ENCHANT_EVENT_NOT )
{
	UidType			m_iGSUID;
	UidType			m_iUnitUID;
	int				m_iAfterWorkStorageKey;
	unsigned short	m_usEventID;

	KEGB_TIME_ENCHANT_EVENT_NOT()
		: m_iGSUID( 0 )
		, m_iUnitUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
		, m_usEventID( 0 )
	{
	}
};

DECL_DATA( KTimeEnchantEventResult )
{
	int		m_iAfterWorkStorageKey;
	char	m_cTimeEnchantEventLevel;

#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
	bool	m_bNotifyGetItem;
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF

	KTimeEnchantEventResult()
		: m_iAfterWorkStorageKey( 0 )
		, m_cTimeEnchantEventLevel( 0 )
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
		, m_bNotifyGetItem( true )
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
	{
	}
};

typedef	KTimeEnchantEventResult	KEGB_TIME_ENCHANT_EVENT_CUBE_NOT;
typedef	KTimeEnchantEventResult	KEGB_TIME_ENCHANT_EVENT_DROP_NOT;

DECL_PACKET( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ )
{
	UidType					m_iGSUID;
	UidType					m_iUnitUID;
	int						m_iTimeEnchantEventUID;
	KTimeEnchantEventInfo	m_kTimeEnchantEventInfo;
	int						m_iAfterWorkStorageKey;
	unsigned short			m_usEventID;

	KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ()
		: m_iGSUID( 0 )
		, m_iUnitUID( 0 )
		, m_iTimeEnchantEventUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
		, m_usEventID( 0 )
	{
	}
};

DECL_PACKET( DBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK )
{
	UidType					m_iGSUID;
	UidType					m_iUnitUID;
	int						m_iAfterWorkStorageKey;
	unsigned short			m_usEventID;
	char					m_cEnchantLevel;

	KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK()
		: m_iGSUID( 0 )
		, m_iUnitUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
		, m_usEventID( 0 )
		, m_cEnchantLevel( 0 )
	{
	}
};
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
DECL_PACKET( DBE_HACKING_USER_CHECK_COUNT_INFO_REQ )
{
	UidType			m_iUserUID;
	byte			m_byteType;
	std::wstring	m_wstrReason;

	KDBE_HACKING_USER_CHECK_COUNT_INFO_REQ()
		: m_iUserUID( 0 )
		, m_byteType( 0 )
	{
	}
};

DECL_PACKET( DBE_HACKING_USER_CHECK_COUNT_INFO_ACK )
{
	int				m_iOK;
	byte			m_byteType;
	byte			m_byteBlockCheckCount;
	std::wstring	m_wstrReason;
	std::wstring	m_wstrLastResetDate;

	KDBE_HACKING_USER_CHECK_COUNT_INFO_ACK()
		: m_iOK( 0 )
		, m_byteType( 0 )
		, m_byteBlockCheckCount( 0 )
	{
	}
};

DECL_DATA( KHackingUserCheckCountDB )
{
	byte			m_byteBlockCheckCount;
	std::wstring	m_wstrLastResetDate;

	KHackingUserCheckCountDB()
		: m_byteBlockCheckCount( 0 )
	{
	}
};

DECL_PACKET( DBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT )
{
	UidType										m_iUserUID;
	std::map<byte, KHackingUserCheckCountDB>	m_mapHackingUserCheckCountDB;

	KDBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT()
		: m_iUserUID( 0 )
	{
	}
};

DECL_PACKET( DBE_ACCOUNT_BLOCK_NOT )
{
	UidType			m_iUserUID;
	byte			m_byteType;
	std::wstring	m_wstrReason;
	std::wstring	m_wstrEndDate;

	KDBE_ACCOUNT_BLOCK_NOT()
		: m_iUserUID( 0 )
		, m_byteType( 0 )
	{
	}
};
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
DECL_PACKET( DBE_INSERT_SKILL_REQ )
{
	UidType							m_iUnitUID;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int								m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	std::map< int, KGetSkillInfo >	m_mapSkillList;
	int								m_iCSPoint;
	int								m_iTotalSpendSkillPoint;
	int								m_iBeforeCSPoint;

	KDBE_INSERT_SKILL_REQ()
	{
		m_iUnitUID = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iCSPoint = -1;
		m_iTotalSpendSkillPoint = 0;
		m_iBeforeCSPoint = 0;
	}
};

DECL_PACKET( DBE_INSERT_SKILL_ACK )
{
	int								m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int								m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	std::map< int, KGetSkillInfo >	m_mapSkillList;
	int								m_iCSPoint;
	int								m_iTotalSpendSkillPoint;
	int								m_iBeforeCSPoint;

	KDBE_INSERT_SKILL_ACK()
	{
		m_iOK = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iCSPoint = -1;
		m_iTotalSpendSkillPoint = 0;
		m_iBeforeCSPoint = 0;
	}
};

DECL_PACKET( DBE_RESET_SKILL_REQ )
{
	UidType								m_iUnitUID;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int									m_iDelSkillID;
	int									m_iDelSkillLevel;

	int									m_iCSPoint;					// unit이 가지고 있는 CSPoint 

	// 롤백용
	int									m_iBeforeSPoint;
	int									m_iBeforeCSPoint;

	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_RESET_SKILL_REQ()
	{
		m_iUnitUID = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iDelSkillID = 0;
		m_iDelSkillLevel = 0;
		m_iCSPoint = 0;

		m_iBeforeSPoint = 0;
		m_iBeforeCSPoint = 0;
	}
};

DECL_PACKET( DBE_RESET_SKILL_ACK )
{
	int									m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int									m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int									m_iDelSkillID;
	int									m_iDelSkillLevel;

	int									m_iCSPoint;

	// 롤백용
	int									m_iBeforeSPoint;
	int									m_iBeforeCSPoint;

	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_RESET_SKILL_ACK()
	{
		m_iOK = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
		m_vecUpdatedInventorySlot.clear();
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iDelSkillID = 0;
		m_iDelSkillLevel = 0;
		m_iCSPoint = 0;
		m_iBeforeSPoint = 0;
		m_iBeforeCSPoint = 0;
	}
};

DECL_PACKET( DBE_ADMIN_CHANGE_UNIT_CLASS_REQ )
{
	UidType					m_iUnitUID;
	char        			m_cUnitClass;

	int						m_iNewDefaultSkill1;
	int						m_iNewDefaultSkill2;
	int						m_iNewDefaultSkill3;
	int						m_iNewDefaultSkill4;
	int						m_iNewDefaultSkill5;
	int						m_iNewDefaultSkill6;

	KDBE_ADMIN_CHANGE_UNIT_CLASS_REQ()
	{
		m_iUnitUID = 0;
		m_cUnitClass = 0;

		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
		m_iNewDefaultSkill3 = 0;
		m_iNewDefaultSkill4 = 0;
		m_iNewDefaultSkill5 = 0;
		m_iNewDefaultSkill6 = 0;
	}
};

DECL_PACKET( DBE_ADMIN_CHANGE_UNIT_CLASS_ACK )
{
	int						m_iOK;
	UidType					m_iUnitUID;
	char        			m_cUnitClass;

	int						m_iNewDefaultSkill1;
	int						m_iNewDefaultSkill2;
	int						m_iNewDefaultSkill3;
	int						m_iNewDefaultSkill4;
	int						m_iNewDefaultSkill5;
	int						m_iNewDefaultSkill6;

	KDBE_ADMIN_CHANGE_UNIT_CLASS_ACK()
	{
		m_iOK = 0;
		m_iUnitUID = 0;
		m_cUnitClass = 0;

		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
		m_iNewDefaultSkill3 = 0;
		m_iNewDefaultSkill4 = 0;
		m_iNewDefaultSkill5 = 0;
		m_iNewDefaultSkill6 = 0;
	}
};

DECL_PACKET( DBE_ADMIN_AUTO_GET_ALL_SKILL_REQ )
{
	UidType					m_iUnitUID;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM

	char        			m_cUnitClass;
	int						m_iSPoint;
	int						m_iCSPoint;
	std::map<int, int>		m_mapGetSkillList;
	std::vector< short >	m_vecUnsealedSkillID;
	int						m_iNewDefaultSkill1;
	int						m_iNewDefaultSkill2;
	int						m_iNewDefaultSkill3;
	int						m_iNewDefaultSkill4;
	int						m_iNewDefaultSkill5;
	int						m_iNewDefaultSkill6;

	KDBE_ADMIN_AUTO_GET_ALL_SKILL_REQ()
	{
		m_iUnitUID = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_cUnitClass = 0;
		m_iSPoint = 0;
		m_iCSPoint = 0;
		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
		m_iNewDefaultSkill3 = 0;
		m_iNewDefaultSkill4 = 0;
		m_iNewDefaultSkill5 = 0;
		m_iNewDefaultSkill6 = 0;
	}
};

DECL_PACKET( DBE_ADMIN_AUTO_GET_ALL_SKILL_ACK )
{
	int						m_iOK;
	UidType					m_iUnitUID;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	char        			m_cUnitClass;
	int						m_iSPoint;
	int						m_iCSPoint;
	std::map<int, int>		m_mapGetSkillList;
	std::vector< short >	m_vecUnsealedSkillID;
	int						m_iNewDefaultSkill1;
	int						m_iNewDefaultSkill2;
	int						m_iNewDefaultSkill3;
	int						m_iNewDefaultSkill4;
	int						m_iNewDefaultSkill5;
	int						m_iNewDefaultSkill6;

	KDBE_ADMIN_AUTO_GET_ALL_SKILL_ACK()
	{
		m_iOK = 0;
		m_iUnitUID = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_cUnitClass = 0;
		m_iSPoint = 0;
		m_iCSPoint = 0;
		m_iNewDefaultSkill1 = 0;
		m_iNewDefaultSkill2 = 0;
		m_iNewDefaultSkill3 = 0;
		m_iNewDefaultSkill4 = 0;
		m_iNewDefaultSkill5 = 0;
		m_iNewDefaultSkill6 = 0;
	}
};

DECL_PACKET( DBE_ADMIN_GET_SKILL_REQ )
{
	UidType					m_iUnitUID;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int						m_iSkillID;
	int						m_iSkillLevel;
	int						m_iCSPoint;
	bool					m_bUnsealed;
	
	KDBE_ADMIN_GET_SKILL_REQ()
	{
		m_iUnitUID = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iSkillID = 0;
		m_iSkillLevel = 0;
		m_iCSPoint = 0;
		m_bUnsealed = false;
	}
};

DECL_PACKET( DBE_ADMIN_GET_SKILL_ACK )
{
	int						m_iOK;
#ifdef SERV_SKILL_PAGE_SYSTEM
	int						m_iActiveSkillPageNumber;
#endif // SERV_SKILL_PAGE_SYSTEM
	int						m_iSkillID;
	int						m_iSkillLevel;
	int						m_iCSPoint;
	bool					m_bUnsealed;

	KDBE_ADMIN_GET_SKILL_ACK()
	{
		m_iOK = 0;
#ifdef SERV_SKILL_PAGE_SYSTEM
		m_iActiveSkillPageNumber = 0;
#endif // SERV_SKILL_PAGE_SYSTEM
		m_iSkillID = 0;
		m_iSkillLevel = 0;
		m_iCSPoint = 0;
		m_bUnsealed = false;
	}
};

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
DECL_PACKET( DBE_RESET_SKILL_REQ )
{
	UidType								m_iUnitUID;
	int									m_iSkillID;
	int									m_iSkillLevel;
	int									m_iSkillCSPoint;
	int									m_iCSPoint;					// unit이 가지고 있는 CSPoint 
	//int									m_iRetrievedSPoint;
	//int									m_iRetrievedCSPoint;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};

DECL_PACKET( DBE_RESET_SKILL_ACK )
{
	int									m_iOK;
	int									m_iSkillID;
	int									m_iSkillLevel;
	int									m_iSkillCSPoint;
	int									m_iCSPoint;
	//int									m_iRetrievedSPoint;
	//int									m_iRetrievedCSPoint;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;	
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
};
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-17	// 박세훈
DECL_PACKET( SOAP_COUPON_LIST_REQ )
{
	bool			m_bUsed;
	std::string		m_strNexonID;
	short			m_sCouponBoxType;
	int				m_iStartRowIndex;

	KSOAP_COUPON_LIST_REQ()
		: m_bUsed( false )
		, m_sCouponBoxType( 0 )
		, m_iStartRowIndex( 0 )
	{
	}
};

DECL_PACKET( SOAP_COUPON_LIST_ACK )
{
	int								m_iOK;
	bool							m_bUsed;
	short							m_sCouponBoxType;
	int								m_iStartRowIndex;
	int								m_iCouponCount;
	std::vector<KClientCouponBox>	m_vecCouponList;

	KSOAP_COUPON_LIST_ACK()
		: m_iOK( 0 )
		, m_bUsed( false )
		, m_sCouponBoxType( 0 )
		, m_iStartRowIndex( 0 )
		, m_iCouponCount( 0 )
	{
	}
};

DECL_PACKET( SOAP_REGIST_COUPON_REQ )
{
	std::string	m_strCouponPin;
	std::string	m_strNexonID;
};

DECL_PACKET( SOAP_COUPON_TARGET_ITEM_REQ )
{
	int	m_iCouponCardNo;
	int	m_iStartRowIndex;

	KSOAP_COUPON_TARGET_ITEM_REQ()
		: m_iCouponCardNo( 0 )
		, m_iStartRowIndex( 0 )
	{
	}
};

DECL_PACKET( SOAP_COUPON_TARGET_ITEM_ACK )
{
	int									m_iOK;
	int									m_iCouponCardNo;
	int									m_iStartRowIndex;
	int									m_iTargetItemCount;
	std::vector<KCouponBoxTargetItem>	m_vecTargetItem;

	KSOAP_COUPON_TARGET_ITEM_ACK()
		: m_iOK( 0 )
		, m_iCouponCardNo( 0 )
		, m_iStartRowIndex( 0 )
		, m_iTargetItemCount( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_NISMS_ITEM_COUPON_USING_REQ )
{
	unsigned long	m_ulPacketNo;
	byte			m_bytePacketType;
	unsigned long	m_ulRemoteIP;
	byte			m_byteReason;
	std::wstring	m_wstrGameID;
	byte			m_byteIsCafe;
	std::wstring	m_wstrUserID;
	unsigned long	m_ulUserOID;
	std::wstring	m_wstrUserName;
	int				m_iCouponCardNo;

	KENX_BT_NISMS_ITEM_COUPON_USING_REQ()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulRemoteIP( 0 )
		, m_byteReason( 0 )
		, m_byteIsCafe( 0 )
		, m_ulUserOID( 0 )
		, m_iCouponCardNo( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_NISMS_ITEM_COUPON_USING_ACK )
{
	unsigned long						m_ulPacketNo;
	BYTE								m_bytePacketType;
	std::wstring						m_wstrCouponTXID;
	unsigned long						m_ulResult;
	unsigned long						m_ulOrderNo;
	std::vector<KNXBTCouponProductInfo>	m_vecCouponProductInfo;

	KENX_BT_NISMS_ITEM_COUPON_USING_ACK()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulResult( 0 )
		, m_ulOrderNo( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ )
{
	unsigned long	m_ulPacketNo;
	byte			m_bytePacketType;
	unsigned long	m_ulRemoteIP;
	std::wstring	m_wstrUserID;
	unsigned long	m_ulUserOID;
	unsigned long   m_ulProductNo;
	unsigned short	m_usOrderQuantity;

	KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulRemoteIP( 0 )
		, m_ulUserOID( 0 )
		, m_ulProductNo( 0 )
		, m_usOrderQuantity( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK )
{
	unsigned long					m_ulPacketNo;
	byte							m_bytePacketType;
	unsigned long					m_ulResult;
	std::vector<KDiscountCoupon>	m_vecDiscountCoupon;

	KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulResult( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ )
{
	unsigned long							m_ulPacketNo;
	byte									m_bytePacketType;
	unsigned long							m_ulRemoteIP;
	std::wstring							m_wstrUserID;
	unsigned long							m_ulUserOID;
	std::vector<KDiscountCouponInquriyInfo>	m_vecDiscountCouponInquiryInfo;

	KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulRemoteIP( 0 )
		, m_ulUserOID( 0 )
	{
	}
};

DECL_PACKET( ENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK )
{
	unsigned long					m_ulPacketNo;
	byte							m_bytePacketType;
	unsigned long					m_ulResult;
	std::vector<KDiscountCoupon>	m_vecDiscountCoupon;

	KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK()
		: m_ulPacketNo( 0 )
		, m_bytePacketType( 0 )
		, m_ulResult( 0 )
	{
	}
};

#endif // SERV_NEXON_COUPON_SYSTEM

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
DECL_DATA( KExchangeLimitInfo )
{
	int				m_iLimitUID;
	__time64_t		m_tBeginDate;
	__time64_t		m_tEndDate;
	std::set<int>	m_setDestItemID;
	int				m_iLimitQuantity;

	KExchangeLimitInfo()
		: m_iLimitUID( 0 )
		, m_tBeginDate( 0 )
		, m_tEndDate( 0 )
		, m_iLimitQuantity( 0 )
	{
	}
};

DECL_PACKET( DBE_EXCHANGE_LIMIT_INFO_ACK )
{
	std::list< KExchangeLimitInfo >	m_listExchangeLimitInfo;
};

DECL_PACKET( DBE_EXCHANGE_LIMIT_INFO_NOT )
{
	int		m_iExchangeLimitUID;
	int		m_iLimitUID;

	KDBE_EXCHANGE_LIMIT_INFO_NOT()
		: m_iExchangeLimitUID( 0 )
		, m_iLimitUID( 0 )
	{
	}
};

DECL_PACKET( EGB_EXCHANGE_LIMIT_INFO_NOT )
{
	// 교환 목록 중에 수량이 제한되어 있는 것이 상대적으로 적다고 가정하고 작업함.
	int						m_iExchangeLimitUID;
	std::map< int, int >	m_mapExchangeLimitInfo;	// <m_iDestItemID, m_iQuantity>

	KEGB_EXCHANGE_LIMIT_INFO_NOT()
		: m_iExchangeLimitUID( 0 )
	{
	}
};

typedef	KEGB_EXCHANGE_LIMIT_INFO_NOT	KEGB_EXCHANGE_LIMIT_INFO_ACK;

DECL_PACKET( EGB_EXCHANGE_LIMIT_REQ )
{
	UidType	m_iGSUID;
	UidType	m_iUnitUID;
	int		m_iAfterWorkStorageKey;
	int		m_iDestItemID;

	KEGB_EXCHANGE_LIMIT_REQ()
		: m_iGSUID( 0 )
		, m_iUnitUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
		, m_iDestItemID( 0 )
	{
	}
};

DECL_PACKET( EGB_EXCHANGE_LIMIT_ACK )
{
	int					m_iOK;
	int					m_iExchangeLimitUID;
	int					m_iLimitUID;
	int					m_iRollBackUID;

	int					m_iAfterWorkStorageKey;

	KEGB_EXCHANGE_LIMIT_ACK()
		: m_iOK( 0 )
		, m_iExchangeLimitUID( 0 )
		, m_iLimitUID( 0 )
		, m_iRollBackUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
	{
	}
};

DECL_PACKET( DBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ )
{
	int					m_iExchangeLimitUID;
	int					m_iLimitUID;
	int					m_iDestItemID;
	std::wstring		m_wstrRegDate;

	UidType				m_iGSUID;
	UidType				m_iUnitUID;
	int					m_iAfterWorkStorageKey;

	KDBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ()
		: m_iExchangeLimitUID( 0 )
		, m_iLimitUID( 0 )
		, m_iDestItemID( 0 )
		, m_iGSUID( 0 )
		, m_iUnitUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
	{
	}
};

DECL_PACKET( DBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK )
{
	int					m_iOK;
	int					m_iExchangeLimitUID;
	int					m_iLimitUID;
	int					m_iRollBackUID;
	
	UidType				m_iGSUID;
	UidType				m_iUnitUID;
	int					m_iAfterWorkStorageKey;

	KDBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK()
		: m_iOK( 0 )
		, m_iExchangeLimitUID( 0 )
		, m_iLimitUID( 0 )
		, m_iRollBackUID( 0 )
		, m_iGSUID( 0 )
		, m_iUnitUID( 0 )
		, m_iAfterWorkStorageKey( 0 )
	{
	}
};

DECL_PACKET( EGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT )
{
	UidType				m_iUnitUID;
	int					m_iExchangeLimitUID;
	int					m_iLimitUID;
	int					m_iRollBackUID;

	KEGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT()
		: m_iUnitUID( 0 )
		, m_iExchangeLimitUID( 0 )
		, m_iLimitUID( 0 )
		, m_iRollBackUID( 0 )
	{
	}
};

DECL_PACKET( DBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT )
{
	int		m_iRollBackUID;

	KDBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT()
		: m_iRollBackUID( 0 )
	{
	}
};
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈
DECL_PACKET( DBE_JUMPING_CHARACTER_INFO_NOT )
{
	std::map< UidType, std::wstring >	m_mapJumpingInfo;
};

DECL_PACKET( DBE_JUMPING_CHARACTER_UPDATE_REQ )
{
	UidType				m_iUnitUID;
	// 절대값
	int					m_iLevel;
	char				m_cExpandedMaxPageNum;
	//
	int					m_iClass;
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	std::vector<int>	m_vecNewDefaultSkill;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
	std::vector<short>	m_vecUnSealedSkill;
	std::wstring		m_wstrRegDate;

#ifdef SERV_SKILL_PAGE_SYSTEM
	int					m_iTheNumberOfSkillPagesAvailable;
#endif // SERV_SKILL_PAGE_SYSTEM

	KDBE_JUMPING_CHARACTER_UPDATE_REQ()
		: m_iUnitUID( 0 )
		, m_iLevel( 0 )
		, m_cExpandedMaxPageNum( 0 )
		, m_iClass( 0 )
#ifdef SERV_SKILL_PAGE_SYSTEM
		, m_iTheNumberOfSkillPagesAvailable(0)
#endif // SERV_SKILL_PAGE_SYSTEM
	{
	}
};

DECL_PACKET( DBE_JUMPING_CHARACTER_UPDATE_ACK )
{
	int					m_iOK;
	UidType				m_iUnitUID;
	// 절대값
	int					m_iLevel;
	char				m_cExpandedMaxPageNum;
	//
	int					m_iClass;
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
	std::vector<int>	m_vecNewDefaultSkill;
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
	std::vector<short>	m_vecUnSealedSkill;

	KDBE_JUMPING_CHARACTER_UPDATE_ACK()
		: m_iOK( 0 )
		, m_iUnitUID( 0 )
		, m_iLevel( 0 )
		, m_cExpandedMaxPageNum( 0 )
		, m_iClass( 0 )
	{
	}
};
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
DECL_PACKET( DBE_ITEM_EXTRACT_REQ )
{
	UidType                             m_iUnitUID;	
	KItemQuantityUpdate                 m_kItemQuantityUpdate;

	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;	

	std::map< int, int >				m_mapResultItem;

	KDBE_ITEM_EXTRACT_REQ()
	{
		m_iUnitUID			= 0;
	}
};

DECL_PACKET( DBE_ITEM_EXTRACT_ACK )
{
	int                                 m_iOK;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::map< UidType, KItemInfo >      m_mapItemInfo;

	std::map< int, int >				m_mapResultItem;

	KDBE_ITEM_EXTRACT_ACK()
	{
		m_iOK			= 0;
	}
};

DECL_PACKET( DBE_USE_FINALITY_SKILL_REQ )
{
	UidType								m_iUnitUID;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;

	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;
	std::vector< KItemInfo >            m_vecItemInfo;

	KDBE_USE_FINALITY_SKILL_REQ()
	{
		m_iUnitUID			= 0;
	}
};


DECL_PACKET( DBE_USE_FINALITY_SKILL_ACK )
{
	int                                 m_iOK;
	KItemQuantityUpdate                 m_kItemQuantityUpdate;
	std::vector< KInventoryItemInfo >   m_vecUpdatedInventorySlot;

	KDBE_USE_FINALITY_SKILL_ACK()
	{
		m_iOK			= 0;
	}
};
#endif // SERV_FINALITY_SKILL_SYSTEM


#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
DECL_DATA( KHenirHackUserData )
{
    UidType								m_iUserUID;
    UidType								m_iUnitUID;
    int									m_iPlayTime;
    int                                 m_iMemberNum;
};

DECL_PACKET( ELOG_HENIR_HACKING_LOG_NOT )
{
    std::vector< KHenirHackUserData > m_vecHackUser;
};
#endif //SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
DECL_PACKET( EGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT )
{
	int		m_iBattleFieldID;
	byte	m_byteIncreasedValue;

	KEGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT()
		: m_iBattleFieldID( 0 )
		, m_byteIncreasedValue( 0 )
	{
	}
};

typedef	KEGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT	KERM_UPDATE_TOTAL_DANGEROUS_VALUE_NOT;

DECL_PACKET( EGB_BATTLE_FIELD_BOSS_INFO_NOT )
{	
	__time64_t	m_tRemainPortalTime;	// 남은 포탈 유지 시간
	__time64_t	m_tRemainCoolTime;		// 남은 쿨 타임 시간
	int			m_iPortalAppearanceMap;	// 게이트가 생성될 배틀 필드 ID
	int			m_iPortalDestination;	// 게이트가 연결된 보스 필드 ID
	bool		m_bPortalOpen;
	bool		m_bCoolTime;

	KEGB_BATTLE_FIELD_BOSS_INFO_NOT()
		: m_tRemainPortalTime( 0 )
		, m_tRemainCoolTime( 0 )
		, m_iPortalAppearanceMap( 0 )
		, m_iPortalDestination( 0 )
		, m_bPortalOpen( false )
		, m_bCoolTime( false )
	{
	}
};

typedef	KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ			KEGB_ADMIN_BOSS_FIELD_GATE_OPEN_REQ;
typedef	KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_ACK			KEGB_ADMIN_BOSS_FIELD_GATE_OPEN_ACK;
typedef	KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ	KEGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ;
typedef	KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK	KEGB_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK;
typedef	KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ	KEGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ;
typedef	KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK	KEGB_ADMIN_SET_TOTAL_DANGEROUS_VALUE_ACK;

typedef	KEGS_BOSS_FIELD_INTRUDE_RESTRICTION_ACK		KERM_BOSS_FIELD_INTRUDE_RESTRICTION_ACK;

DECL_PACKET( DBE_SOCKET_EXPAND_ITEM_REQ )
{
	UidType							m_iUnitUID;
	UidType							m_iItemUID;
	std::vector<int>				m_vecItemSocket;
	byte							m_byteExpandedSocketNum;
	KItemQuantityUpdate				m_kItemQuantityUpdate;
	std::vector<KInventoryItemInfo>	m_vecUpdatedInventorySlot;

	KDBE_SOCKET_EXPAND_ITEM_REQ()
		: m_iUnitUID( 0 )
		, m_iItemUID( 0 )
		, m_byteExpandedSocketNum( 0 )
	{
	}
};

DECL_PACKET( DBE_SOCKET_EXPAND_ITEM_ACK )
{
	int								m_iOK;
	UidType							m_iItemUID;
	byte							m_byteExpandedSocketNum;
	KItemQuantityUpdate				m_kItemQuantityUpdate;
	std::vector<KInventoryItemInfo>	m_vecUpdatedInventorySlot;

	KDBE_SOCKET_EXPAND_ITEM_ACK()
		: m_iOK( 0 )
		, m_iItemUID( 0 )
		, m_byteExpandedSocketNum( 0 )
	{
	}
};

typedef	KEGS_BOSS_FIELD_LOG_NOT	KERM_BOSS_FIELD_LOG_NOT;

DECL_PACKET( DBE_BOSS_FIELD_LOG_NOT )
{
	typedef	std::map< UidType, std::vector< KBossFieldUserLog > >	TYPE_BOSS_FIELD_USER_LOG;
	
	KBossFieldLog				m_kFieldLog;
	TYPE_BOSS_FIELD_USER_LOG	m_mapUserLog;	// 입장 할 때마다 추가되며, 가장 마지막 정보에 갱신을 한다.

	void	clear( void )
	{
		m_kFieldLog.clear();
		m_mapUserLog.clear();
	}
};
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
DECL_PACKET( ELOG_EXPAND_BANK_INVENTORY_NOT )
{
    UidType	m_iUserUID;
    UidType	m_iUnitUID;
    int	m_iUnitLevel;
    int m_iBeforeED;
    int m_iUseED;
    int m_iCategory;
    int m_iNumSlot;

    KELOG_EXPAND_BANK_INVENTORY_NOT()
        :m_iUserUID(0),
        m_iUnitUID(0),
        m_iUnitLevel(0),
        m_iBeforeED(0),
        m_iUseED(0),
        m_iCategory(0),
        m_iNumSlot(0)
    {}
};
#endif //SERV_GOOD_ELSWORD

DECL_DATA( KFieldBossGenKillStat )
{
    int m_nFieldID;
    //int m_nNumGenBoss;
    //int m_nNumDieBoss;
    //int m_nNumDropItem;
    int m_nNumUser;
    //int m_nDieNPCUID;
    int m_nSpawnID;
    //std::vector< KDropItemData > m_vecDropItem;

    KFieldBossGenKillStat()
        :m_nFieldID(0),
        //m_nNumGenBoss(0),
        //m_nNumDieBoss(0),
        //m_nNumDropItem(0),
        m_nNumUser(0),
        //m_nDieNPCUID(0),
        m_nSpawnID(0)
    {}
};

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
DECL_PACKET( ELOG_KOM_FILE_CHECK_LOG_NOT )
{
	std::wstring		m_wstrInvaildKomName;
	UidType				m_iUserUID;
};
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#ifdef SERV_SKILL_PAGE_SYSTEM

#define THE_NUMBER_OF_DEFAULT_SKILLS 6

DECL_PACKET( DBE_EXPAND_SKILL_PAGE_REQ )
{
	UidType					m_iUnitUID;
	std::vector<int>		m_vecSkillPageNumberToBeAdded;	/// 2번째 페이지 추가를 요청하면 값은 2임
	int						m_iSPoint;
	int						m_iCSPoint;
	int						m_iDefaultSkill[THE_NUMBER_OF_DEFAULT_SKILLS];
	char        			m_cUnitClass;
	unsigned short			m_usEventID;

	KDBE_EXPAND_SKILL_PAGE_REQ()
		: m_iUnitUID(0),
		m_iSPoint(0), m_iCSPoint(0), m_cUnitClass(0), m_usEventID(0)
	{
		m_vecSkillPageNumberToBeAdded.clear();
		for ( int i = 0; i < THE_NUMBER_OF_DEFAULT_SKILLS; i++ )
			m_iDefaultSkill[i] = 0;
	}
};

DECL_PACKET( DBE_EXPAND_SKILL_PAGE_ACK )
{
	int						m_iOK;
	UidType					m_iUnitUID;
	int						m_iSkillPageNumberToBeAdded;	/// 2번째 페이지 추가를 요청하면 값은 2임
	int						m_iSPoint;
	int						m_iCSPoint;
	int						m_iDefaultSkill[THE_NUMBER_OF_DEFAULT_SKILLS];
	char        			m_cUnitClass;
	unsigned short			m_usEventID;

	KDBE_EXPAND_SKILL_PAGE_ACK()
		: m_iOK(0),
		m_iUnitUID(0), m_iSkillPageNumberToBeAdded(0),
		m_iSPoint(0), m_iCSPoint(0), 
		m_cUnitClass(0), m_usEventID(0)
	{
		for ( int i = 0; i < THE_NUMBER_OF_DEFAULT_SKILLS; i++ )
			m_iDefaultSkill[i] = 0;
	}
};

DECL_PACKET( DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ )
{
	UidType					m_iUnitUID;
	int						m_iSkillPagesNumberToBeActive;

	KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ()
		: m_iUnitUID(0), m_iSkillPagesNumberToBeActive(1)
	{}

	KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ( const UidType iUnitUID_, const int iSkillPagesNumberToBeActive_ )
		: m_iUnitUID( iUnitUID_ ), m_iSkillPagesNumberToBeActive( iSkillPagesNumberToBeActive_ )
	{}
};

DECL_PACKET( DBE_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK )
{
	int						m_iOK;
	int						m_iSkillPagesNumberToBeActive;

	KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK()
		: m_iOK(0), m_iSkillPagesNumberToBeActive(1)
	{}
};

DECL_PACKET( ELOG_EXPAND_SKILL_PAGE_NOT )
{
	UidType					m_iUnitUID;
	UCHAR					m_ucLevel;
	UCHAR        			m_ucUnitClass;
	UCHAR					m_ucBeforeTheNumberOfSkillPagesAvailable;
	UCHAR					m_ucPaymentType;	// 0: ED, 1: Cash
	std::wstring			m_wstrSenderNickName;

	KELOG_EXPAND_SKILL_PAGE_NOT()
		: m_iUnitUID(0),
		m_ucLevel(0),
		m_ucUnitClass(0),
		m_ucBeforeTheNumberOfSkillPagesAvailable(1),
		m_ucPaymentType(0),
		m_wstrSenderNickName()
	{}
};

#endif // SERV_SKILL_PAGE_SYSTEM

DECL_PACKET( DBE_GET_CREATE_UNIT_TODAY_COUNT_REQ )
{
    UidType	m_iUserUID;

    KDBE_GET_CREATE_UNIT_TODAY_COUNT_REQ():
    m_iUserUID(0)
    {}
};

#ifdef SERV_NAVER_CHANNELING
DECL_PACKET( NAVER_SOAP_GET_TOKEN_REQ )
{
    LONG64 m_iNaverSN;
    int m_iGameCode;

    KNAVER_SOAP_GET_TOKEN_REQ():
    m_iNaverSN(0),
    m_iGameCode(0)
    {}
};

//typedef std::string KNAVER_SOAP_GET_TOKEN_ACK;
DECL_PACKET( NAVER_SOAP_GET_TOKEN_ACK )
{
    int         m_iOK;
    std::string m_strAccessToken;

    KNAVER_SOAP_GET_TOKEN_ACK():m_iOK(0),
        m_strAccessToken(std::string())
    {}
};

#endif SERV_NAVER_CHANNELING

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
DECL_PACKET( EGS_DEL_COMMON_FLAG_NOT )
{
	DWORD dwFlag;

	KEGS_DEL_COMMON_FLAG_NOT()
	{
		dwFlag = 0;
	}
};
DECL_PACKET( EGS_ADD_COMMON_FLAG_NOT )
{
	DWORD dwFlag;

	KEGS_ADD_COMMON_FLAG_NOT()
	{
		dwFlag = 0;
	}
};
DECL_PACKET( ECN_DEL_COMMON_FLAG_NOT )
{
	DWORD dwFlag;

	KECN_DEL_COMMON_FLAG_NOT()
	{
		dwFlag = 0;
	}
};
DECL_PACKET( ECN_ADD_COMMON_FLAG_NOT )
{
	DWORD dwFlag;

	KECN_ADD_COMMON_FLAG_NOT()
	{
		dwFlag = 0;
	}
};

#endif // SERV_MODFIY_FLAG_REALTIME_PATCH

#pragma pack( pop )

//{{ 2013.02.27 조효진	해외 기본 구조 작업 (SERV_GLOBAL_BASE)
#   include "OnlyGlobal/Packet/ServerPacket_Global.h"
//}}
