#include <winsock2.h>
#include "ServerPacket.h"


SERIALIZE_DEFINE_PUT( KServerInfo, obj, ks )
{
	return PUT( m_iUID ) 
		&& PUT( m_wstrName )
		&& PUT( m_iServerGroupID )
		&& PUT( m_iServerClass )
		//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
		&& PUT( m_wstrPublicIP )
		&& PUT( m_wstrPrivateIP )
#else
		&& PUT( m_wstrIP )
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
		//}}
		&& PUT( m_usMasterPort )
		&& PUT( m_usNCUDPPort )
		&& PUT( m_nMaxUser )
		&& PUT( m_nCurrentUser )
		&& PUT( m_bOn )
		//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
		&& PUT( m_cServerRollType )
#endif SERV_SERVER_ROLL_TYPE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KServerInfo, obj, ks )
{
    return GET( m_iUID ) 
        && GET( m_wstrName )
        && GET( m_iServerGroupID )
        && GET( m_iServerClass )
		//{{ 2013. 01. 17	최육사	서버간 통신 사설IP 적용
#ifdef SERV_PRIVATE_IP_SERVER_NETWORKING
		&& GET( m_wstrPublicIP )
		&& GET( m_wstrPrivateIP )
#else
		&& GET( m_wstrIP )
#endif SERV_PRIVATE_IP_SERVER_NETWORKING
		//}}
        && GET( m_usMasterPort )
        && GET( m_usNCUDPPort )
        && GET( m_nMaxUser )
        && GET( m_nCurrentUser )
        && GET( m_bOn )
		//{{ 2011. 02. 23	최육사	서버별 역할 타입
#ifdef SERV_SERVER_ROLL_TYPE
		&& GET( m_cServerRollType )
#endif SERV_SERVER_ROLL_TYPE
		//}}
        ;
}

//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
SERIALIZE_DEFINE_PUT( KECN_VERIFY_SERVER_CONNECT_REQ, obj, ks )
{
	return PUT( m_kNetAddress )
		&& PUT( m_iServerGroupID )
		;
}

SERIALIZE_DEFINE_GET( KECN_VERIFY_SERVER_CONNECT_REQ, obj, ks )
{
	return GET( m_kNetAddress )
		&& GET( m_iServerGroupID )
		;
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_SERVER_INFO_REQ, obj,ks )
{
    return PUT( m_wstrRegDate )
		&& PUT( m_iServerUID )
		&& PUT( m_iConcurrentUser )
        //{{ 2009. 4. 1  최육사		파티정보
		&& PUT( m_iCurPartyCount )
		&& PUT( m_iPlayGamePartyCount )
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_SERVER_INFO_REQ, obj,ks )
{
    return GET( m_wstrRegDate )
		&& GET( m_iServerUID )
		&& GET( m_iConcurrentUser )
		//{{ 2009. 4. 1  최육사		파티정보
		&& GET( m_iCurPartyCount )
		&& GET( m_iPlayGamePartyCount )
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KELG_NEXON_USER_AUTHENTICATE_REQ, obj, ks )
{
    return  PUT( m_kAuthenticateReq ) 
		&&	PUT( m_kNexonAccountInfo )
#ifdef SERV_KOG_OTP_VERIFY
		&&	PUT( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
		&&	PUT( m_usZone )
		&&	PUT( m_usGame )
#endif // SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		&&	PUT( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_GET( KELG_NEXON_USER_AUTHENTICATE_REQ, obj, ks )
{
    return  GET( m_kAuthenticateReq ) 
		&&	GET( m_kNexonAccountInfo )
#ifdef SERV_KOG_OTP_VERIFY
		&&	GET( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
		&&	GET( m_usZone )
		&&	GET( m_usGame )
#endif // SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		&&	GET( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_PUT( KELG_NEXON_USER_AUTHENTICATE_ACK, obj, ks )
{
    return PUT( m_iOK )
		&& PUT( m_kAccountInfo )
		&& PUT( m_kNexonAccountInfo )
		&& PUT( m_cHackingUserType )
		//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
		&& PUT( m_bJoinAccount )
#endif SERV_IP_ACCOUNT_LOG
		//}}
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& PUT( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
		&& PUT( m_uiGiantAccountType )
#endif SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
#ifdef SERV_EPAY_SYSTEM
		&& PUT( m_usZone )
		&& PUT( m_usGame )
#endif // SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
#ifdef SERV_COUNTRY_PH
		&& PUT( m_usGarenaCyberCafe )
#endif // SERV_COUNTRY_PH
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		&& PUT( m_iEventMoney )
#endif // SERV_EVENT_MONEY
        ;
}

SERIALIZE_DEFINE_GET( KELG_NEXON_USER_AUTHENTICATE_ACK, obj, ks )
{
    return GET( m_iOK )
		&& GET( m_kAccountInfo )
		&& GET( m_kNexonAccountInfo )
		&& GET( m_cHackingUserType )
		//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
		&& GET( m_bJoinAccount )
#endif SERV_IP_ACCOUNT_LOG
		//}}
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& GET( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
		&& GET( m_uiGiantAccountType )
#endif SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
#ifdef SERV_EPAY_SYSTEM
		&& GET( m_usZone )
		&& GET( m_usGame )
#endif // SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
#ifdef SERV_COUNTRY_PH
		&& GET( m_usGarenaCyberCafe )
#endif // SERV_COUNTRY_PH
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		&& GET( m_iEventMoney )
#endif // SERV_EVENT_MONEY
		;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 06. 29  최육사	게임 PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW

SERIALIZE_DEFINE_PUT( KELG_NEXON_USER_LOGIN_TIME_OUT_NOT, obj, ks )
{
	return PUT( m_wstrUserID )
		;
}

SERIALIZE_DEFINE_GET( KELG_NEXON_USER_LOGIN_TIME_OUT_NOT, obj, ks )
{
	return GET( m_wstrUserID )
		;
}

#endif SERV_PCBANG_AUTH_NEW
//}}

//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
SERIALIZE_DEFINE_PUT( KDBE_SELECT_UNIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kUnitInfo )
		//{{ 2011. 01. 18	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&& PUT( m_wstrUnitCreateDate )
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
		&& PUT( m_iRealDataED )
#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
		&& PUT( m_wstrUnitLastLoginDate )
#endif SERV_DAILY_CHAR_FIRST_SELECT
		//}}
		&& PUT( m_mapInventorySlotSize )
		&& PUT( m_mapItem )
		//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_mapPetItem )
#endif SERV_PET_SYSTEM
		//}}
		&& PUT( m_iNumResurrectionStone )
		//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&& PUT( m_iNumAutoPaymentResStone )
		&& PUT( m_wstrAutoPayResStoneLastDate )
#endif AP_RESTONE
		//}}
		&& PUT( m_vecQuest )
		&& PUT( m_vecCompletQuest )
		&& PUT( m_vecMission )
		&& PUT( m_vecTitle )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_vecUserSkillPageData )
#else //SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_vecSkillAcquired )
#endif //SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_vecSkillUnsealed )
		//{{ 2010. 03. 13  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
		&& PUT( m_cSkillNoteMaxPageNum )
		&& PUT( m_mapSkillNote )
#endif SERV_SKILL_NOTE
		//}}
		&& PUT( m_vecChatBlackList )
		&& PUT( m_kDenyOptions )
		&& PUT( m_bIsRecommend )
		&& PUT( m_iRecommendUnitUID )
		&& PUT( m_vecTutorialDBUnitInfo )
		&& PUT( m_bIsSpiritUpdated )
		&& PUT( m_vecConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& PUT( m_vecCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT
		//}}
		//{{ 2009. 7. 7  최육사		랭킹개편
		&& PUT( m_vecHenirRanking )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
		&& PUT( m_vecHenirHeroRanking )
#endif // SERV_HENIR_RENEWAL_2013
		//}}
		&& PUT( m_kMessengerInfo )
		//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_iSummonedPetUID )
		&& PUT( m_vecPetList )
		&& PUT( m_vecCanEvolutionPetList )
		&& PUT( m_vecNeverSummonPetList )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&& PUT( m_mapCharGameCount )
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_kPShopAgencyInfo )
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		&& PUT( m_PacketHenirRewardCount )
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
		&& PUT( m_mapRandomQuestList )
#endif SERV_RANDOM_DAY_QUEST
		//}}
		//{{ 2011. 10. 13	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
		&& PUT( m_bCashShopOpen )
#endif SERV_CONTENT_MANAGER
		//}}
		//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		&& PUT( m_iEventMoney )
#endif // SERV_EVENT_MONEY
		//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
		&& PUT( m_iPvpEventIndex )
		&& PUT( m_wstrLastPvpEventDate )
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}
		//{{ 2012. 10. 10	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
		&& PUT( m_kBingoEvent )
#endif SERV_EVENT_BINGO
		//}}
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		&& PUT( m_vecAccountQuesting )
		&& PUT( m_vecAccountCompleteQuest )
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		//{{ 2012. 12. 20	최육사	아라 첫 선택 튜토리얼
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
		&& PUT( m_bFirstSelect )
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
		//}}
#ifdef SERV_ADVERTISEMENT_EVENT
		&& PUT( m_vecAdvertisementEvent )
#endif SERV_ADVERTISEMENT_EVENT
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& PUT( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		&& PUT( m_iConnectExperienceAck )
		&& PUT( m_iReward7DaysItem )
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& PUT( m_iNewUnitE )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
		&& PUT( m_kLocalRankingUserInfo )
		&& PUT( m_wstrLocalRankingUserInfoUpdated )
#endif	// SERV_LOCAL_RANKING_SYSTEM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& PUT( m_kRelationshipInfo )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
        && PUT( m_nWrongUnitUID )
#ifdef SERV_BLESS_OF_GODDESS_EVENT
		&& PUT( m_bMaxLevelUnitInAccount )
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& PUT( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		&& PUT( m_bCouple )
		&& PUT( m_iRelationTargetUserUid )
		&& PUT( m_wstrRelationTargetUserNickname )
#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_RECRUIT_EVENT_BASE
		&& PUT( m_vecRecruiterUnitInfo )
		&& PUT( m_vecRecruitUnitInfo )
#endif SERV_RECRUIT_EVENT_BASE
#ifdef SERV_ELESIS_UPDATE_EVENT
		&& PUT( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		&& PUT( m_wstrGiveMeTheItemTime_One )
		&& PUT( m_wstrGiveMeTheItemTime_Two )
		&& PUT( m_wstrGiveMeTheItemTime_Tree )
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		&& PUT( m_wstrButtonClickTime_One )
		&& PUT( m_bItemGive )
		&& PUT( m_iDungeonClearCount )
		&& PUT( m_iFieldMonsterKillCount )
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		&& PUT( m_iValentineGiveItemCount )
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& PUT( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}

SERIALIZE_DEFINE_GET( KDBE_SELECT_UNIT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kUnitInfo )
		//{{ 2011. 01. 18	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&& GET( m_wstrUnitCreateDate )
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
		&& GET( m_iRealDataED )
#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
		&& GET( m_wstrUnitLastLoginDate )
#endif SERV_DAILY_CHAR_FIRST_SELECT
		//}}
		&& GET( m_mapInventorySlotSize )
		&& GET( m_mapItem )
		//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_mapPetItem )
#endif SERV_PET_SYSTEM
		//}}
		&& GET( m_iNumResurrectionStone )
		//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&& GET( m_iNumAutoPaymentResStone )
		&& GET( m_wstrAutoPayResStoneLastDate )
#endif AP_RESTONE
		//}}
		&& GET( m_vecQuest )
		&& GET( m_vecCompletQuest )		
		&& GET( m_vecMission )
		&& GET( m_vecTitle )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_vecUserSkillPageData )
#else //SERV_SKILL_PAGE_SYSTEM
		&& GET( m_vecSkillAcquired )
#endif //SERV_SKILL_PAGE_SYSTEM
		&& GET( m_vecSkillUnsealed )
		//{{ 2010. 03. 13  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
		&& GET( m_cSkillNoteMaxPageNum )
		&& GET( m_mapSkillNote )
#endif SERV_SKILL_NOTE
		//}}
		&& GET( m_vecChatBlackList )
		&& GET( m_kDenyOptions )
		&& GET( m_bIsRecommend )
		&& GET( m_iRecommendUnitUID )
		&& GET( m_vecTutorialDBUnitInfo )
		&& GET( m_bIsSpiritUpdated )
		&& GET( m_vecConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& GET( m_vecCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT
		//}}
		//{{ 2009. 7. 7  최육사		랭킹개편
		&& GET( m_vecHenirRanking )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
		&& GET( m_vecHenirHeroRanking )
#endif // SERV_HENIR_RENEWAL_2013
		//}}
		&& GET( m_kMessengerInfo )
		//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_iSummonedPetUID )
		&& GET( m_vecPetList )
		&& GET( m_vecCanEvolutionPetList )
		&& GET( m_vecNeverSummonPetList )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&& GET( m_mapCharGameCount )
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_kPShopAgencyInfo )
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		&& GET( m_PacketHenirRewardCount )
#endif SERV_NEW_HENIR_TEST
		//}}//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
		&& GET( m_mapRandomQuestList )
#endif SERV_RANDOM_DAY_QUEST
		//}}
		//{{ 2011. 10. 13	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
		&& GET( m_bCashShopOpen )
#endif SERV_CONTENT_MANAGER
		//}}
		//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		&& GET( m_iEventMoney )
#endif // SERV_EVENT_MONEY
		//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
		&& GET( m_iPvpEventIndex )
		&& GET( m_wstrLastPvpEventDate )
#endif SERV_2012_PVP_SEASON2_EVENT
		//}}
		//{{ 2012. 10. 10	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
		&& GET( m_kBingoEvent )
#endif SERV_EVENT_BINGO
		//}}
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		&& GET( m_vecAccountQuesting )
		&& GET( m_vecAccountCompleteQuest )
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		//{{ 2012. 12. 20	최육사	아라 첫 선택 튜토리얼
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
		&& GET( m_bFirstSelect )
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
		//}}
#ifdef SERV_ADVERTISEMENT_EVENT
		&& GET( m_vecAdvertisementEvent )
#endif SERV_ADVERTISEMENT_EVENT
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& GET( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		&& GET( m_iConnectExperienceAck )
		&& GET( m_iReward7DaysItem )
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& GET( m_iNewUnitE )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
		&& GET( m_kLocalRankingUserInfo )
		&& GET( m_wstrLocalRankingUserInfoUpdated )
#endif	// SERV_LOCAL_RANKING_SYSTEM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& GET( m_kRelationshipInfo )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
        && GET( m_nWrongUnitUID )
#ifdef SERV_BLESS_OF_GODDESS_EVENT
		&& GET( m_bMaxLevelUnitInAccount )
#endif SERV_BLESS_OF_GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& GET( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		&& GET( m_bCouple )
		&& GET( m_iRelationTargetUserUid )
		&& GET( m_wstrRelationTargetUserNickname )
#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_RECRUIT_EVENT_BASE
		&& GET( m_vecRecruiterUnitInfo )
		&& GET( m_vecRecruitUnitInfo )
#endif SERV_RECRUIT_EVENT_BASE
#ifdef SERV_ELESIS_UPDATE_EVENT
		&& GET( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		&& GET( m_wstrGiveMeTheItemTime_One )
		&& GET( m_wstrGiveMeTheItemTime_Two )
		&& GET( m_wstrGiveMeTheItemTime_Tree )
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		&& GET( m_wstrButtonClickTime_One )
		&& GET( m_bItemGive )
		&& GET( m_iDungeonClearCount )
		&& GET( m_iFieldMonsterKillCount )
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		&& GET( m_iValentineGiveItemCount )
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& GET( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 26  최육사	채널이동
SERIALIZE_DEFINE_PUT( KECN_CHECK_CHANNEL_CHANGE_REQ, obj, ks )
{
	return PUT( m_iSourGSUID )
		&& PUT( m_iDestGSUID )
		&& PUT( m_iUserUID )
		&& PUT( m_iCharUID )
		&& PUT( m_kChangeUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KECN_CHECK_CHANNEL_CHANGE_REQ, obj, ks )
{
	return GET( m_iSourGSUID )
		&& GET( m_iDestGSUID )
		&& GET( m_iUserUID )
		&& GET( m_iCharUID )
		&& GET( m_kChangeUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KECN_CHECK_CHANNEL_CHANGE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iSourGSUID )
		&& PUT( m_iCharUID )
		;
}

SERIALIZE_DEFINE_GET( KECN_CHECK_CHANNEL_CHANGE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iSourGSUID )
		&& GET( m_iCharUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ, obj, ks )
{
	return PUT( m_kConnectAck )
		&& PUT( m_kUserAuthReq )
		&& PUT( m_kSelectUnitReq )
		&& PUT( m_kChangeUserInfo )
#ifdef SERV_KOG_OTP_VERIFY
		&& PUT( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
		&& PUT( m_usZone )
		&& PUT( m_usGame )
		&& PUT( m_uiGiantAccountType )
#endif //SERV_EPAY_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ, obj, ks )
{
	return GET( m_kConnectAck )
		&& GET( m_kUserAuthReq )
		&& GET( m_kSelectUnitReq )
		&& GET( m_kChangeUserInfo )
#ifdef SERV_KOG_OTP_VERIFY
		&& GET( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
		&& GET( m_usZone )
		&& GET( m_usGame )
		&& GET( m_uiGiantAccountType )
#endif //SERV_EPAY_SYSTEM
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK, obj, ks )
{
	return PUT( m_kConnectAck )
		&& PUT( m_kUserAuthAck )
		&& PUT( m_kSelectUnitReq )
		&& PUT( m_kChangeUserInfo )
		//{{ 2012. 02. 21	김민성	2차 보안 채널 이동시(미니맵) 오류 수정
#ifdef SERV_SECOND_SECURITY_PW_ERROR_MODIFY
		&& PUT( m_bUseSecondPW )
		&& PUT( m_wstrSecondPW )
		&& PUT( m_iFailedCount )
		&& PUT( m_wstrLastAuthDate )
#endif SERV_SECOND_SECURITY_PW_ERROR_MODIFY
		//}}
#ifdef SERV_EPAY_SYSTEM
		&& PUT( m_usZone )
		&& PUT( m_usGame )
		&& PUT( m_uiGiantAccountType )
#endif //SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSocketID )
#endif //SERV_COUNTRY_TH
#ifdef SERV_COUNTRY_PH
		&& PUT( m_usGarenaCyberCafe )
#endif //SERV_COUNTRY_PH
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK, obj, ks )
{
	return GET( m_kConnectAck )
		&& GET( m_kUserAuthAck )
		&& GET( m_kSelectUnitReq )
		&& GET( m_kChangeUserInfo )
		//{{ 2012. 02. 21	김민성	2차 보안 채널 이동시(미니맵) 오류 수정
#ifdef SERV_SECOND_SECURITY_PW_ERROR_MODIFY
		&& GET( m_bUseSecondPW )
		&& GET( m_wstrSecondPW )
		&& GET( m_iFailedCount )
		&& GET( m_wstrLastAuthDate )
#endif SERV_SECOND_SECURITY_PW_ERROR_MODIFY
		//}}
#ifdef SERV_EPAY_SYSTEM
		&& GET( m_usZone )
		&& GET( m_usGame )
		&& GET( m_uiGiantAccountType )
#endif //SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSocketID )
#endif //SERV_COUNTRY_TH
#ifdef SERV_COUNTRY_PH
		&& GET( m_usGarenaCyberCafe )
#endif //SERV_COUNTRY_PH
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ, obj, ks )
{
	return PUT( m_kConnectAck )
		&& PUT( m_kVerifyAccountAck )
		&& PUT( m_kSelectUnitReq )
		&& PUT( m_kChangeUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_REQ, obj, ks )
{
	return GET( m_kConnectAck )
		&& GET( m_kVerifyAccountAck )
		&& GET( m_kSelectUnitReq )
		&& GET( m_kChangeUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK, obj, ks )
{
	return PUT( m_kConnectAck )
		&& PUT( m_kVerifyAccountAck )
		&& PUT( m_kSelectUnitAck )
		&& PUT( m_kChangeUserInfo )
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& PUT( m_iNewUnitCh )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK, obj, ks )
{
	return GET( m_kConnectAck )
		&& GET( m_kVerifyAccountAck )
		&& GET( m_kSelectUnitAck )
		&& GET( m_kChangeUserInfo )
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& GET( m_iNewUnitCh )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		;
}


//}}

SERIALIZE_DEFINE_PUT( KSquareCreateInfo, obj, ks )
{
	return PUT( m_iSquareType )
		//{{ 2010. 11. 1	최육사	광장 리스트 개선
#ifdef SERV_SQUARE_LIST_SERV_UID
		&& PUT( m_cSortIndex )
#endif SERV_SQUARE_LIST_SERV_UID
		//}}
		&& PUT( m_wstrSquareName )
		;
}

SERIALIZE_DEFINE_GET( KSquareCreateInfo, obj, ks )
{
	return GET( m_iSquareType ) 
		//{{ 2010. 11. 1	최육사	광장 리스트 개선
#ifdef SERV_SQUARE_LIST_SERV_UID
		&& GET( m_cSortIndex )
#endif SERV_SQUARE_LIST_SERV_UID
		//}}
		&& GET( m_wstrSquareName )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CENTER_SERVER_ON_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		&&	PUT( m_kServerInfo )
		&&	PUT( m_iProxyID )
		&&	PUT( m_vecSquareList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CENTER_SERVER_ON_ACK, obj, ks )
{
	return  GET( m_iOK ) 
		&&	GET( m_kServerInfo )
		&&	GET( m_iProxyID )
		&&	GET( m_vecSquareList )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_LOGIN_SERVER_ON_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		&&	PUT( m_iProxyID )
		&&	PUT( m_kServerInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOGIN_SERVER_ON_ACK, obj, ks )
{
	return  GET( m_iOK ) 
		&&	GET( m_iProxyID )
		&&	GET( m_kServerInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GAME_SERVER_ON_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_kServerInfo )
		&& PUT( m_mapCenterList )
		&& PUT( m_mapLoginList )
		//{{ 2009. 5. 5  최육사		채널id
		&& PUT( m_iChannelID )
		
		//{{ 2012. 05. 16	박세훈	채널 리스트 정보 통합
#ifdef SERV_INTEGRATE_THE_CHANNEL_INFO
#else
		&& PUT( m_kBonusInfo )
#endif SERV_INTEGRATE_THE_CHANNEL_INFO
		//}}

		//}}
#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
		&& PUT( m_mapPCBangIPList )
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
		&& PUT( m_mapGlobalList )
//#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		;
}

SERIALIZE_DEFINE_GET( KDBE_GAME_SERVER_ON_ACK, obj, ks )
{
	return GET( m_iOK ) 
		&& GET( m_kServerInfo )
		&& GET( m_mapCenterList )
		&& GET( m_mapLoginList )
		//{{ 2009. 5. 5  최육사		채널id
		&& GET( m_iChannelID )

		//{{ 2012. 05. 16	박세훈	채널 리스트 정보 통합
#ifdef SERV_INTEGRATE_THE_CHANNEL_INFO
#else
		&& GET( m_kBonusInfo )
#endif SERV_INTEGRATE_THE_CHANNEL_INFO
		//}}

		//}}
#ifdef SERV_KOG_HAVE_PCBANG_IP_LIST
		&& GET( m_mapPCBangIPList )
#endif //SERV_KOG_HAVE_PCBANG_IP_LIST
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
//#ifdef SERV_INSERT_GLOBAL_SERVER
		&& GET( m_mapGlobalList )
//#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		;
}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
SERIALIZE_DEFINE_PUT( KDBE_CHANNEL_SERVER_ON_ACK, obj, ks )
{
	return	PUT( m_iOK )
		&&	PUT( m_iProxyID )
		&&	PUT( m_kServerInfo )
		&&	PUT( m_mapLoginList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANNEL_SERVER_ON_ACK, obj, ks )
{
	return	GET( m_iOK )
		&&	GET( m_iProxyID )
		&&	GET( m_kServerInfo )
		&&	GET( m_mapLoginList )
		;
}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

SERIALIZE_DEFINE_PUT( KERM_END_GAME_NOT, obj, ks )
{
    return PUT( m_iWinTeam )
		;
}

SERIALIZE_DEFINE_GET( KERM_END_GAME_NOT, obj, ks )
{
    return GET( m_iWinTeam )
		;
}

SERIALIZE_DEFINE_PUT( KERM_GET_ITEM_ACK, obj, ks )
{
    return PUT( m_iOK )
		&& PUT( m_kItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_GET_ITEM_ACK, obj, ks )
{
    return GET( m_iOK )
		&& GET( m_kItemInfo )
		;
}

//{{ 2009. 5. 11  최육사	실시간아이템
SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_INSERT_TO_INVENTORY_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_kBroadInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_INSERT_TO_INVENTORY_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_kBroadInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_INSERT_TO_INVENTORY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapInsertItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_kBroadInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_INSERT_TO_INVENTORY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapInsertItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_kBroadInfo )
		;
}

SERIALIZE_DEFINE_PUT( KERM_GET_ITEM_COMPLETE_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapGetItem )
		;
}

SERIALIZE_DEFINE_GET( KERM_GET_ITEM_COMPLETE_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapGetItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_TEMP_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iTempItemUID )
		&& PUT( m_mapInsertItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_TEMP_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iTempItemUID )
		&& GET( m_mapInsertItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_TEMP_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iTempItemUID )
		&& PUT( m_mapInsertItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_TEMP_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iTempItemUID )
		&& GET( m_mapInsertItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KELOG_STAT_PLAY_TIME, obj, ks )
{
    return PUT( m_iUserUID )
		&& PUT( m_wstrIP )
		&& PUT( m_wstrLoginTime )
		&& PUT( m_wstrLogoutTime )
		&& PUT( m_bLoginFail )
		&& PUT( m_bLogoutFail )
		&& PUT( m_iTotalPlayTime )
		&& PUT( m_iPvpPlayTime )
		//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
		&& PUT( m_iDungeonPlayTime )
		&& PUT( m_iFieldPlayTime )
#endif SERV_FIELD_PLAY_TIME_LOG
		//}}
		&& PUT( m_iDisconnectReason )
		&& PUT( m_iDisconnectUserFSM )
		//{{ 2009. 12. 22  최육사	서버군확장
		&& PUT( m_iServerGroupID )
		//}}
		//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
		&& PUT( m_wstrMachineID )
#endif SERV_MACHINE_ID_BLOCK
		//}}
		//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
		&& PUT( m_wstrUnitID )
		//&& PUT( m_bIsPcbang )
		&& PUT( m_iUnitUID )
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}
#ifdef SERV_CONNECT_LOG_CHANNELING
		&& PUT( m_ucChannelCode )
#endif SERV_CONNECT_LOG_CHANNELING
		&& PUT( m_bIsPcbang ) // 해외도 필요해서 밖으로 뺌
#ifdef SERV_STEAM
		&& PUT( m_bSteamClient )
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		&& PUT( m_bAeriaClient )
#endif //SERV_CHANNELING_AERIA
        ;
}

SERIALIZE_DEFINE_GET( KELOG_STAT_PLAY_TIME, obj, ks )
{	
	return GET( m_iUserUID )
		&& GET( m_wstrIP )
		&& GET( m_wstrLoginTime )
		&& GET( m_wstrLogoutTime )
		&& GET( m_bLoginFail )
		&& GET( m_bLogoutFail )
		&& GET( m_iTotalPlayTime )
		&& GET( m_iPvpPlayTime )
		//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
		&& GET( m_iDungeonPlayTime )
		&& GET( m_iFieldPlayTime )
#endif SERV_FIELD_PLAY_TIME_LOG
		//}}
		&& GET( m_iDisconnectReason )
		&& GET( m_iDisconnectUserFSM )
		//{{ 2009. 12. 22  최육사	서버군확장
		&& GET( m_iServerGroupID )
		//}}
		//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
		&& GET( m_wstrMachineID )
#endif SERV_MACHINE_ID_BLOCK
		//}}
		//{{ 2011. 09. 27	김민성	PC 방 오과금 방지
#ifdef SERV_PCBANG_INCORRECT_BILLING
		&& GET( m_wstrUnitID )
		//&& GET( m_bIsPcbang )
		&& GET( m_iUnitUID )
#endif SERV_PCBANG_INCORRECT_BILLING
	//}}
#ifdef SERV_CONNECT_LOG_CHANNELING
		&& GET( m_ucChannelCode )
#endif SERV_CONNECT_LOG_CHANNELING
		&& GET( m_bIsPcbang ) // 해외도 필요해서 밖으로 뺌
#ifdef SERV_STEAM
		&& GET( m_bSteamClient )
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		&& GET( m_bAeriaClient )
#endif //SERV_CHANNELING_AERIA
		;
}

SERIALIZE_DEFINE_PUT( KDBE_BUY_ED_ITEM_REQ, obj, ks )
{
    return PUT( m_iUnitUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecBuyGPItemInfo )
#ifdef SERV_GET_ITEM_REASON_BY_CHEAT
		&& PUT( m_bCheat )
#endif SERV_GET_ITEM_REASON_BY_CHEAT
		;
}

SERIALIZE_DEFINE_GET( KDBE_BUY_ED_ITEM_REQ, obj, ks )
{
    return GET( m_iUnitUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecBuyGPItemInfo )
#ifdef SERV_GET_ITEM_REASON_BY_CHEAT
		&& GET( m_bCheat )
#endif SERV_GET_ITEM_REASON_BY_CHEAT
		;
}

SERIALIZE_DEFINE_PUT( KDBE_BUY_ED_ITEM_ACK, obj, ks )
{
    return PUT( m_iED )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_iAPoint )
#else
		&& PUT( m_iVSPoint )
#endif SERV_PVP_NEW_SYSTEM
		//}}
        && PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BUY_ED_ITEM_ACK, obj, ks )
{
	return GET( m_iED )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_iAPoint )
#else
		&& GET( m_iVSPoint )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_ITEM_REQ, obj, ks )
{
    return PUT( m_iUnitUID )
		//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
		&& PUT( m_cGetItemReason )
#endif SERV_GET_ITEM_REASON
		//}}
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_bOutRoom )		
		&& PUT( m_setSealCashItem )
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& PUT( m_mapGetItem )
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_ITEM_REQ, obj, ks )
{
    return GET( m_iUnitUID )
		//{{ 2010. 9. 8	최육사	아이템 획득 사유
#ifdef SERV_GET_ITEM_REASON
		&& GET( m_cGetItemReason )
#endif SERV_GET_ITEM_REASON
		//}}
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_bOutRoom )        
		&& GET( m_setSealCashItem )
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& GET( m_mapGetItem )
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_ITEM_ACK, obj, ks )
{
    return PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_bOutRoom )
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& PUT( m_mapGetItem )
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_ITEM_ACK, obj, ks )
{
    return GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_bOutRoom )
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& GET( m_mapGetItem )
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
        ;
}

//{{ 2008. 7. 29  최육사	캐쉬 아이템
SERIALIZE_DEFINE_PUT( KDBE_INSERT_PURCHASED_CASH_PACKAGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_ulProductNo )		
		&& PUT( m_setSealCashItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_PURCHASED_CASH_PACKAGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_ulProductNo )		
		&& GET( m_setSealCashItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_PURCHASED_CASH_PACKAGE_ACK, obj, ks )
{
	return PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_ulProductNo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_PURCHASED_CASH_PACKAGE_ACK, obj, ks )
{
	return GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_ulProductNo )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELOG_CHAT_LOG_NOT, obj, ks )
{
    return PUT( m_nRoomUID )
		&& PUT( m_nCharUID )
		&& PUT( m_nUserUID )
		&& PUT( m_cChatType )
		&& PUT( m_strMsg )
		;
}

SERIALIZE_DEFINE_GET( KELOG_CHAT_LOG_NOT, obj, ks )
{
    return GET( m_nRoomUID )
		&& GET( m_nCharUID )
		&& GET( m_nUserUID )
		&& GET( m_cChatType )
		&& GET( m_strMsg )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHARACTER_LEVELUP_NOT, obj, ks )
{
    return PUT( m_nCharUID )
		&& PUT( m_ucLevelBefore )
		&& PUT( m_ucLevelAfter )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHARACTER_LEVELUP_NOT, obj, ks )
{
    return GET( m_nCharUID )
		&& GET( m_ucLevelBefore )
		&& GET( m_ucLevelAfter )
		;
}

//////////////////////////////////////////////////////////////////////////
// Party
SERIALIZE_DEFINE_PUT( KERM_PARTY_OPEN_DUNGEON_ROOM_REQ, obj, ks )
{
	return PUT( m_kRoomInfo )
		&& PUT( m_sWorldID )
		&& PUT( m_vecUserInfo )
        ;
}

SERIALIZE_DEFINE_GET( KERM_PARTY_OPEN_DUNGEON_ROOM_REQ, obj, ks )
{
	return GET( m_kRoomInfo )
		&& GET( m_sWorldID )
		&& GET( m_vecUserInfo )
        ;
}

// Party
//////////////////////////////////////////////////////////////////////////

//{{ 2007. 8. 9  최육사  블랙리스트 패킷구조체
SERIALIZE_DEFINE_PUT( KDBE_NEW_BLACKLIST_USER_REQ, obj, ks )
{
	return  PUT( m_iUnitUID ) &&
			PUT( m_iBlackListUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_NEW_BLACKLIST_USER_REQ, obj, ks )
{
	return  GET( m_iUnitUID ) &&
			GET( m_iBlackListUnitUID );
}

SERIALIZE_DEFINE_PUT( KDBE_NEW_BLACKLIST_USER_ACK, obj, ks )
{
	return  PUT( m_iOK ) &&
			PUT( m_kChatBlackListUnit );
}

SERIALIZE_DEFINE_GET( KDBE_NEW_BLACKLIST_USER_ACK, obj, ks )
{
	return  GET( m_iOK ) &&
			GET( m_kChatBlackListUnit );
}

SERIALIZE_DEFINE_PUT( KDBE_DEL_BLACKLIST_USER_REQ, obj, ks )
{
	return  PUT( m_iUnitUID ) &&
			PUT( m_iBlackListUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_DEL_BLACKLIST_USER_REQ, obj, ks )
{
	return  GET( m_iUnitUID ) &&
			GET( m_iBlackListUnitUID );
}

SERIALIZE_DEFINE_PUT( KDBE_DEL_BLACKLIST_USER_ACK, obj, ks )
{
	return  PUT( m_iOK ) &&
			PUT( m_iBlackListUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_DEL_BLACKLIST_USER_ACK, obj, ks )
{
	return  GET( m_iOK ) &&
			GET( m_iBlackListUnitUID );
}
//}}

//{{ 2007. 8. 20  최육사  채팅 통합
SERIALIZE_DEFINE_PUT( KERM_CHAT_ACK, obj, ks )
{
	return  PUT( m_iOK ) &&
			PUT( m_cChatPacketType );
}

SERIALIZE_DEFINE_GET( KERM_CHAT_ACK, obj, ks )
{
	return  GET( m_iOK ) &&
			GET( m_cChatPacketType );
}
//}}

SERIALIZE_DEFINE_PUT( KERM_REQUEST_TRADE_NOT, obj, ks )
{
	return	PUT( m_iSenderUID ) &&
			PUT( m_wstrSenderNickName ) &&
			PUT( m_iReceiverUID );
}

SERIALIZE_DEFINE_GET( KERM_REQUEST_TRADE_NOT, obj, ks )
{
	return	GET( m_iSenderUID ) &&
			GET( m_wstrSenderNickName ) &&
			GET( m_iReceiverUID );
}

SERIALIZE_DEFINE_PUT( KERM_REQUEST_TRADE_REPLY_NOT, obj, ks )
{
	return	PUT( m_iReceiverUID ) &&
			PUT( m_iOK ) &&
			PUT( m_iMemberUID ) &&
			PUT( m_iMemberGSUID );
}

SERIALIZE_DEFINE_GET( KERM_REQUEST_TRADE_REPLY_NOT, obj, ks )
{
	return	GET( m_iReceiverUID ) &&
			GET( m_iOK ) &&
			GET( m_iMemberUID ) &&
			GET( m_iMemberGSUID );
}

SERIALIZE_DEFINE_PUT( KERM_OPEN_TRADE_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_wstrSenderNickName )
		&& PUT( m_iMemberUID )
		&& PUT( m_iMemberGSUID )
		//{{ 2009. 4. 28  최육사	최대ED제한
		&& PUT( m_iED )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_TRADE_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_wstrSenderNickName )
		&& GET( m_iMemberUID )
		&& GET( m_iMemberGSUID )
		//{{ 2009. 4. 28  최육사	최대ED제한
		&& GET( m_iED )
		//}}
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_OPEN_TRADE_ACK, obj, ks )
{
	return	PUT( m_iOK ) &&
		PUT( m_iTradeUID );
}

SERIALIZE_DEFINE_GET( KERM_OPEN_TRADE_ACK, obj, ks )
{
	return	GET( m_iOK ) &&
		GET( m_iTradeUID );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_JOIN_TRADE_REQ, obj, ks )
{
	return	PUT( m_kMemberInfo );
};

SERIALIZE_DEFINE_GET( KERM_JOIN_TRADE_REQ, obj, ks )
{
	return	GET( m_kMemberInfo );
};

SERIALIZE_DEFINE_PUT( KERM_JOIN_TRADE_ACK, obj, ks )
{
	return	PUT( m_iOK ) &&
			PUT( m_iTradeUID ) &&
			PUT( m_vecTradeUserInfo );
};

SERIALIZE_DEFINE_GET( KERM_JOIN_TRADE_ACK, obj, ks )
{
	return	GET( m_iOK ) &&
			GET( m_iTradeUID ) &&
			GET( m_vecTradeUserInfo );
};

SERIALIZE_DEFINE_PUT( KERM_UPDATE_TRADE_ITEM_REQ, obj, ks )
{
	return PUT( m_kInventoryItemInfo )
		&& PUT( m_bDelete )
		;
};

SERIALIZE_DEFINE_GET( KERM_UPDATE_TRADE_ITEM_REQ, obj, ks )
{
	return GET( m_kInventoryItemInfo )
		&& GET( m_bDelete )
		;
};

SERIALIZE_DEFINE_PUT( KERM_CONFIRM_TRADE_REQ, obj, ks )
{
	return PUT( m_vecItemIn )
		&& PUT( m_vecItemOut )
		&& PUT( m_iEDIn )
		&& PUT( m_iEDOut )
		;
};

SERIALIZE_DEFINE_GET( KERM_CONFIRM_TRADE_REQ, obj, ks )
{
	return GET( m_vecItemIn )
		&& GET( m_vecItemOut )
		&& GET( m_iEDIn )
		&& GET( m_iEDOut )
		;
};

//{{ 2009. 2. 10  최육사	개인거래 버그 예방
SERIALIZE_DEFINE_PUT( KERM_CONFIRM_TRADE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdateTemp )
		&& PUT( m_vecItemInfoTemp )
		&& PUT( m_iEDTemp )
		;
}

SERIALIZE_DEFINE_GET( KERM_CONFIRM_TRADE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdateTemp )
		&& GET( m_vecItemInfoTemp )
		&& GET( m_iEDTemp )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KERM_CONFIRM_TRADE_NOT, obj, ks )
{
	return PUT( m_iTradeUID )
		&& PUT( m_vecItemIn )
		&& PUT( m_vecItemOut )
		&& PUT( m_iEDIn )
		&& PUT( m_iEDOut )
		&& PUT( m_kItemQuantityUpdateTemp )
		&& PUT( m_vecItemInfoTemp )
		&& PUT( m_iEDTemp )
		//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
		&& PUT( m_iOtherCID )
#endif SERV_MONITORING_LETTER_AND_TRADE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_CONFIRM_TRADE_NOT, obj, ks )
{
	return GET( m_iTradeUID )
		&& GET( m_vecItemIn )
		&& GET( m_vecItemOut )
		&& GET( m_iEDIn )
		&& GET( m_iEDOut )
        && GET( m_kItemQuantityUpdateTemp )
		&& GET( m_vecItemInfoTemp )
		&& GET( m_iEDTemp )
		//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
		&& GET( m_iOtherCID )
#endif SERV_MONITORING_LETTER_AND_TRADE
		//}}
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KSuccessTradeData, obj, ks )
{
	return	PUT( m_bIsSuccess ) &&
			PUT( m_vecInventorySlotInfo ) &&
			PUT( m_kConfirmTradeNot );
};

SERIALIZE_DEFINE_GET( KSuccessTradeData, obj, ks )
{
	return	GET( m_bIsSuccess ) &&
			GET( m_vecInventorySlotInfo ) &&
			GET( m_kConfirmTradeNot );
};
//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KDBE_INSERT_TRADE_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )		
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TRADE_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )		
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_TRADE_ITEM_ACK, obj, ks )
{
	return PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )		
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TRADE_ITEM_ACK, obj, ks )
{
	return GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )		
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}

//{{ 2009. 2. 10  최육사	개인거래 버그 예방
SERIALIZE_DEFINE_PUT( KDBE_INSERT_TRADE_ITEM_BY_SERVER_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iED )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TRADE_ITEM_BY_SERVER_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iED )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecItemInfo )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KELOG_INSERT_TRADE_LOG_NOT, obj, ks )
{
	return PUT( m_iFromUserUID )
		&& PUT( m_iFromUnitUID )
		&& PUT( m_iToUserUID )
		&& PUT( m_iToUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_iQuantity )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KELOG_INSERT_TRADE_LOG_NOT, obj, ks )
{
	return GET( m_iFromUserUID )
		&& GET( m_iFromUnitUID )
		&& GET( m_iToUserUID )
		&& GET( m_iToUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_iQuantity )
		&& GET( m_wstrRegDate )
		;
}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
SERIALIZE_DEFINE_PUT( KERM_DB_UPDATE_TRADE_RESULT_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iChangedED )
		&& PUT( m_vecUpdated )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_kItemQuantityUpdate_OUT )
		&& PUT( m_vecInsertDBItem )
		;
}

SERIALIZE_DEFINE_GET( KERM_DB_UPDATE_TRADE_RESULT_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iChangedED )
		&& GET( m_vecUpdated )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_kItemQuantityUpdate_OUT )
		&& GET( m_vecInsertDBItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DB_UPDATE_TRADE_RESULT_REQ, obj, ks )
{
	return PUT( m_vecDBUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DB_UPDATE_TRADE_RESULT_REQ, obj, ks )
{
	return GET( m_vecDBUpdate )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_DB_UPDATE_TRADE_RESULT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecDBUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DB_UPDATE_TRADE_RESULT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecDBUpdate )
		;
}
#else
//{{ 2012. 03. 28	최육사	개인거래 DDOS 방어
#ifdef SERV_TRADE_DDOS_DEFENCE
SERIALIZE_DEFINE_PUT( KERM_DB_UPDATE_TRADE_RESULT_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iChangedED )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_DB_UPDATE_TRADE_RESULT_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iChangedED )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}


SERIALIZE_DEFINE_PUT( KERM_DB_UPDATE_TRADE_COMPLETE_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iChangedED )
		&& PUT( m_iChangedEDFailed )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_DB_UPDATE_TRADE_COMPLETE_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iChangedED )
		&& GET( m_iChangedEDFailed )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_DB_UPDATE_TRADE_RESULT_REQ, obj, ks )
{
	return PUT( m_vecDBUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DB_UPDATE_TRADE_RESULT_REQ, obj, ks )
{
	return GET( m_vecDBUpdate )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_DB_UPDATE_TRADE_RESULT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecDBUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DB_UPDATE_TRADE_RESULT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecDBUpdate )
		;
}
#endif SERV_TRADE_DDOS_DEFENCE
//}}

#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2010. 06. 04  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KENX_AUTH_INITIALIZE_REQ, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteInitializeType )
		&& PUT( m_byteGameSN )
		&& PUT( m_byteDomainSN )
		&& PUT( m_wstrDomainName )
		&& PUT( m_byteSynchronizeType )
		&& PUT( m_ulSynchronizeCount )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_INITIALIZE_REQ, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteInitializeType )
		&& GET( m_byteGameSN )
		&& GET( m_byteDomainSN )
		&& GET( m_wstrDomainName )
		&& GET( m_byteSynchronizeType )
		&& GET( m_ulSynchronizeCount )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_INITIALIZE_ACK, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteInitializeType )
		&& PUT( m_byteResult )
		&& PUT( m_byteDomainSN )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_INITIALIZE_ACK, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteInitializeType )
		&& GET( m_byteResult )
		&& GET( m_byteDomainSN )
		&& GET( m_wstrMessage )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_LOGIN_REQ, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteLoginType )
		&& PUT( m_wstrUserID )
		&& PUT( m_uiIP )		
		&& PUT( m_mapProperty )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_LOGIN_REQ, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteLoginType )
		&& GET( m_wstrUserID )
		&& GET( m_uiIP )		
		&& GET( m_mapProperty )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_LOGIN_ACK, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteLoginType )
		&& PUT( m_iSessionNo )
		&& PUT( m_wstrUserID )
		&& PUT( m_byteAuthorizeResult )
		&& PUT( m_byteAuthorizeType )
		&& PUT( m_byteChargeType )
		&& PUT( m_byteOption )
		&& PUT( m_ulArgument )
		&& PUT( m_byteIsCharged )
		&& PUT( m_mapProperty )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_LOGIN_ACK, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteLoginType )
		&& GET( m_iSessionNo )
		&& GET( m_wstrUserID )
		&& GET( m_byteAuthorizeResult )
		&& GET( m_byteAuthorizeType )
		&& GET( m_byteChargeType )
		&& GET( m_byteOption )
		&& GET( m_ulArgument )
		&& GET( m_byteIsCharged )
		&& GET( m_mapProperty )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_LOGOUT_NOT, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteLogoutType )
		&& PUT( m_wstrUserID )
		&& PUT( m_iSessionNo )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_LOGOUT_NOT, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteLogoutType )
		&& GET( m_wstrUserID )
		&& GET( m_iSessionNo )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_TERMINATE_NOT, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteTerminateType )
		&& PUT( m_iSessionNo )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrCharacterName )
		&& PUT( m_byteOption )
	//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
		&& PUT( m_bytePropertyCount )
#endif SERV_NEXON_SESSION_PROTOCOL
	//}} 
	//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
		&& PUT( m_mapProperty )
#endif SERV_SHUTDOWN_SYSTEM
	//}}
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_TERMINATE_NOT, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteTerminateType )
		&& GET( m_iSessionNo )
		&& GET( m_wstrUserID )
		&& GET( m_wstrCharacterName )
		&& GET( m_byteOption )
		//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
		&& GET( m_bytePropertyCount )
#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
		//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
		&& GET( m_mapProperty )
#endif SERV_SHUTDOWN_SYSTEM
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_MESSAGE_NOT, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteMessageType )
		&& PUT( m_iSessionNo )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrCharacterName )
		&& PUT( m_byteOption )
		&& PUT( m_ulArgument )
		&& PUT( m_ulSessionCount )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_MESSAGE_NOT, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteMessageType )
		&& GET( m_iSessionNo )
		&& GET( m_wstrUserID )
		&& GET( m_wstrCharacterName )
		&& GET( m_byteOption )
		&& GET( m_ulArgument )
		&& GET( m_ulSessionCount )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_SYNCHRONIZE_REQ, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteIsMonitoring )
		&& PUT( m_mapSynchronizeReq )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_SYNCHRONIZE_REQ, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteIsMonitoring )
		&& GET( m_mapSynchronizeReq )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_SYNCHRONIZE_ACK, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteIsMonitoring )
		&& PUT( m_mapSynchronizeAck )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_SYNCHRONIZE_ACK, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteIsMonitoring )
		&& GET( m_mapSynchronizeAck )
		;
}


SERIALIZE_DEFINE_PUT( KENX_AUTH_ALIVE_NOT, obj, ks )
{
	return PUT( m_bytePacketType )
		;
}

SERIALIZE_DEFINE_GET( KENX_AUTH_ALIVE_NOT, obj, ks )
{
	return GET( m_bytePacketType )
		;
}


//////////////////////////////////////////////////////////////////////////
#endif SERV_PCBANG_AUTH_NEW
//}}

SERIALIZE_DEFINE_PUT( KENX_INITIALIZE_BILLING_AUTH_NOT, obj, ks )
{
	return PUT( m_bytePacketType )
		&& PUT( m_byteGameSN )
		&& PUT( m_byteDomainSN )
		&& PUT( m_wstrDomainName )
		;
}

SERIALIZE_DEFINE_GET( KENX_INITIALIZE_BILLING_AUTH_NOT, obj, ks )
{
	return GET( m_bytePacketType )
		&& GET( m_byteGameSN )
		&& GET( m_byteDomainSN )
		&& GET( m_wstrDomainName )
		;
}


SERIALIZE_DEFINE_PUT( KENX_COMMON_REQ, obj, ks )
{
	return	PUT( m_bytePacketType ) &&
			PUT( m_byteLogin ) &&
			PUT( m_wstrUserID ) &&
			PUT( m_wstrUnitNickName ) &&
			PUT( m_uiIP ) &&
			PUT( m_byteCanTry ) &&
			PUT( m_byteGuestUser )
			;
}

SERIALIZE_DEFINE_GET( KENX_COMMON_REQ, obj, ks )
{
	return	GET( m_bytePacketType ) &&
			GET( m_byteLogin ) &&
			GET( m_wstrUserID ) &&
			GET( m_wstrUnitNickName ) &&
			GET( m_uiIP ) &&
			GET( m_byteCanTry ) &&
			GET( m_byteGuestUser )
			;
}

SERIALIZE_DEFINE_PUT( KENX_COMMON_ACK_NOT, obj, ks )
{
	return	PUT( m_bytePacketType ) &&
			PUT( m_wstrUserID ) &&
			PUT( m_wstrUnitNickName ) &&
			PUT( m_byteResult ) &&
			PUT( m_byteAddressDesc ) &&
			PUT( m_byteAccountDesc ) &&
			PUT( m_byteOption ) &&
			PUT( m_uiArgument ) &&
			PUT( m_byteIsGameBangBillType );
}

SERIALIZE_DEFINE_GET( KENX_COMMON_ACK_NOT, obj, ks )
{
	return	GET( m_bytePacketType ) &&
			GET( m_wstrUserID ) &&
			GET( m_wstrUnitNickName ) &&
			GET( m_byteResult ) &&
			GET( m_byteAddressDesc ) &&
			GET( m_byteAccountDesc ) &&
			GET( m_byteOption ) &&
			GET( m_uiArgument ) &&
			GET( m_byteIsGameBangBillType );
}

SERIALIZE_DEFINE_PUT( KENX_ALIVE_NOT, obj, ks )
{
	return	PUT( m_bytePacketType );
}

SERIALIZE_DEFINE_GET( KENX_ALIVE_NOT, obj, ks )
{
	return	GET( m_bytePacketType );
}

SERIALIZE_DEFINE_PUT( KENX_ADM_SEARCH_REQ, obj, ks )
{
    return  PUT( m_dwTransactionID ) &&
            PUT( m_wstrUserID );
}

SERIALIZE_DEFINE_GET( KENX_ADM_SEARCH_REQ, obj, ks )
{
    return  GET( m_dwTransactionID ) &&
            GET( m_wstrUserID );
}

SERIALIZE_DEFINE_PUT( KENX_ADM_SEARCH_ACK, obj, ks )
{
    return  PUT( m_dwTransactionID ) &&
            PUT( m_byteResult );
}

SERIALIZE_DEFINE_GET( KENX_ADM_SEARCH_ACK, obj, ks )
{
    return  GET( m_dwTransactionID ) &&
            GET( m_byteResult );
}

SERIALIZE_DEFINE_PUT( KENX_CHAR_SEARCH_REQ, obj, ks )
{
    return  PUT( m_dwTransactionID ) &&
            PUT( m_wstrUnitNickName );
}

SERIALIZE_DEFINE_GET( KENX_CHAR_SEARCH_REQ, obj, ks )
{
    return  GET( m_dwTransactionID ) &&
            GET( m_wstrUnitNickName );
}

SERIALIZE_DEFINE_PUT( KENX_CHAR_SEARCH_ACK, obj, ks )
{
    return  PUT( m_dwTransactionID ) &&
            PUT( m_byteResult );
}

SERIALIZE_DEFINE_GET( KENX_CHAR_SEARCH_ACK, obj, ks )
{
    return  GET( m_dwTransactionID ) &&
            GET( m_byteResult );
}

SERIALIZE_DEFINE_PUT( KENX_NEW_SHOP_ITEM_NOT, obj, ks )
{
    return  PUT( m_dwTransactionID ) &&
            PUT( m_wstrUnitNickName );
}

SERIALIZE_DEFINE_GET( KENX_NEW_SHOP_ITEM_NOT, obj, ks )
{
    return  GET( m_dwTransactionID ) &&
            GET( m_wstrUnitNickName );
}

SERIALIZE_DEFINE_PUT( KENX_NEW_SHOP_GIFT_NOT, obj, ks )
{
    return  PUT( m_dwTransactionID ) &&
            PUT( m_wstrUnitNickName );
}

SERIALIZE_DEFINE_GET( KENX_NEW_SHOP_GIFT_NOT, obj, ks )
{
    return  GET( m_dwTransactionID ) &&
            GET( m_wstrUnitNickName );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_END_TC_GAME_ACK, obj, ks )
{
	return	PUT( m_iOK ) &&
			PUT( m_iTCID ) &&
			PUT( m_bIsSuccess )
		;
}

SERIALIZE_DEFINE_GET( KERM_END_TC_GAME_ACK, obj, ks )
{
	return	GET( m_iOK ) &&
			GET( m_iTCID ) &&
			GET( m_bIsSuccess )
		;
}

SERIALIZE_DEFINE_PUT( KELG_REGISTER_USER_REQ, obj, ks )
{
    return PUT( m_wstrUserID )
		//{{ 2009. 4. 17  최육사	채널id
		&& PUT( m_iChannelID )
		//}}
		&& PUT( m_uiIP )
		&& PUT( m_byteCanTry )
		&& PUT( m_byteGuestUser )		
		&& PUT( m_cAuthLevel )
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
		&& PUT( m_uiPublisherUID )
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		&& PUT( m_ucChannelCode )
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		&& PUT( m_strMachineID )
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KELG_REGISTER_USER_REQ, obj, ks )
{
    return GET( m_wstrUserID )
		//{{ 2009. 4. 17  최육사	채널id
		&& GET( m_iChannelID )
		//}}
		&& GET( m_uiIP )
		&& GET( m_byteCanTry )
		&& GET( m_byteGuestUser )        
		&& GET( m_cAuthLevel )
#ifdef SERV_GET_USER_INFO_BY_PUBLISHER_UID
		&& GET( m_uiPublisherUID )
#endif SERV_GET_USER_INFO_BY_PUBLISHER_UID
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		&& GET( m_ucChannelCode )
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		&& GET( m_strMachineID )
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_PUT( KELG_REGISTER_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KELG_REGISTER_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CHANNEL_CHANGE_REGISTER_USER_REQ, obj, ks )
{
	return PUT( m_kConnectAck )
		&& PUT( m_kVerifyAccountAck )
		&& PUT( m_kSelectUnitReq )
		&& PUT( m_kChangeUserInfo )
		&& PUT( m_kRegUserReq )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANNEL_CHANGE_REGISTER_USER_REQ, obj, ks )
{
	return GET( m_kConnectAck )
		&& GET( m_kVerifyAccountAck )
		&& GET( m_kSelectUnitReq )
		&& GET( m_kChangeUserInfo )
		&& GET( m_kRegUserReq )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CHANNEL_CHANGE_REGISTER_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kConnectAck )
		&& PUT( m_kVerifyAccountAck )
		&& PUT( m_kSelectUnitReq )
		&& PUT( m_kChangeUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANNEL_CHANGE_REGISTER_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kConnectAck )
		&& GET( m_kVerifyAccountAck )
		&& GET( m_kSelectUnitReq )
		&& GET( m_kChangeUserInfo )
		;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 3. 17  최육사	유저 접속종료 알림
SERIALIZE_DEFINE_PUT( KELG_USER_DISCONNECT_REQ, obj, ks )
{
	return PUT( m_usEventID )
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& PUT( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KELG_USER_DISCONNECT_REQ, obj, ks )
{
	return GET( m_usEventID )
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& GET( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2008. 5. 27  최육사  랜덤맵
SERIALIZE_DEFINE_PUT( KERM_GAME_START_REQ, obj, ks )
{
	return PUT( m_sWorldID )
		;
}

SERIALIZE_DEFINE_GET( KERM_GAME_START_REQ, obj, ks )
{
	return GET( m_sWorldID )
		;
}
//}}

//{{ 2008. 10. 29  최육사	아이템 검사
SERIALIZE_DEFINE_PUT( KERM_CHANGE_READY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iDungeonID )
		&& PUT( m_cDungeonMode )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHANGE_READY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iDungeonID )
		&& GET( m_cDungeonMode )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_OPEN_RANDOM_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		//{{ 2010. 7. 26  최육사	밀봉 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
		&& PUT( m_setSealRandomItem )
#endif SERV_SEALED_RANDOM_ITEM
		//}}
		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		&& PUT( m_iNumResurrectionStone )
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		&& PUT( m_iSpendED )
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		;
}

SERIALIZE_DEFINE_GET( KDBE_OPEN_RANDOM_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		//{{ 2010. 7. 26  최육사	밀봉 랜덤 큐브
#ifdef SERV_SEALED_RANDOM_ITEM
		&& GET( m_setSealRandomItem )
#endif SERV_SEALED_RANDOM_ITEM
		//}}
		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		&& GET( m_iNumResurrectionStone )
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		&& GET( m_iSpendED )
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_OPEN_RANDOM_ITEM_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_mapInsertedItem )
		&&	PUT( m_kItemQuantityUpdate )		
		&&	PUT( m_vecUpdatedInventorySlot )
		&&	PUT( m_mapItemInfo )
		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		&& PUT( m_iNumResurrectionStone )
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		&& PUT( m_iSpendED )
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		;
}

SERIALIZE_DEFINE_GET( KDBE_OPEN_RANDOM_ITEM_ACK, obj, ks )
{
	return  GET( m_iOK )
		&&	GET( m_mapInsertedItem )
		&&	GET( m_kItemQuantityUpdate )		
		&&	GET( m_vecUpdatedInventorySlot )
		&&	GET( m_mapItemInfo )
		//{{ 2011.12.28 김민성   랜덤 큐브 오픈시 부활석 표기 오류 수정
#ifdef SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		&& GET( m_iNumResurrectionStone )
#endif SERV_OPEN_RANDOM_CUBE_VIEW_ERROR
		//}}
		//{{ 2012. 11. 26 큐브 ED 오픈 조건 기능 추가 - 김민성
#ifdef SERV_CUBE_OPEN_ED_CONDITION
		&& GET( m_iSpendED )
#endif SERV_CUBE_OPEN_ED_CONDITION
		//}
		;
}
//}}

//{{ 2007. 10. 10  최육사  제조
SERIALIZE_DEFINE_PUT( KDBE_ITEM_MANUFACTURE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_MANUFACTURE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ITEM_MANUFACTURE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_MANUFACTURE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}
//}}

//{{ 2007. 10. 16  최육사  유저 통계
SERIALIZE_DEFINE_PUT( KELOG_USER_STATISTICS, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_mapPvpStatistics )		
		&& PUT( m_mapDungeonStatistics )
		&& PUT( m_mapTCenterStatistics )
		&& PUT( m_mapEtcStatistics )
#ifdef SERV_USER_STATISTICS_RENEWAL
		&& PUT( m_mapEDStatistics )
		&& PUT( m_iAuthLevel )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_iED )
		&& PUT( m_iVSPoint )
		&& PUT( m_iVSPointMax )
		&& PUT( m_wstrLoginTime )
		&& PUT( m_wstrLogoutTime )
		&& PUT( m_wstrNickName )
		&& PUT( m_uiGiantUID )
		&& PUT( m_wstrAccountCreateDate )
		&& PUT( m_iEXP )
		&& PUT( m_wstrUnitCreateDate )
		&& PUT( m_iiDurationTime )
#endif //SERV_USER_STATISTICS_RENEWAL
		;
}

SERIALIZE_DEFINE_GET( KELOG_USER_STATISTICS, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_mapPvpStatistics )		
		&& GET( m_mapDungeonStatistics )
		&& GET( m_mapTCenterStatistics )
		&& GET( m_mapEtcStatistics )
#ifdef SERV_USER_STATISTICS_RENEWAL
		&& GET( m_mapEDStatistics )
		&& GET( m_iAuthLevel )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_iED )
		&& GET( m_iVSPoint )
		&& GET( m_iVSPointMax )
		&& GET( m_wstrLoginTime )
		&& GET( m_wstrLogoutTime )
		&& GET( m_wstrNickName )
		&& GET( m_uiGiantUID )
		&& GET( m_wstrAccountCreateDate )
		&& GET( m_iEXP )
		&& GET( m_wstrUnitCreateDate )
		&& GET( m_iiDurationTime )
#endif //SERV_USER_STATISTICS_RENEWAL
		;
}
//}}

SERIALIZE_DEFINE_PUT( KPurchaseCoupon, obj, ks )
{
    return  PUT( m_uiNexonSN ) &&
            PUT( m_wstrUserID ) &&
            PUT( m_wstrUserName ) &&
            PUT( m_wstrCouponSerial ) &&
            PUT( m_iGameType ) &&
            PUT( m_iServerDomain ) &&
            PUT( m_wstrUnitNickName ) &&
            PUT( m_iReason ) &&
            PUT( m_wstrCafeNo );
}

SERIALIZE_DEFINE_GET( KPurchaseCoupon, obj, ks )
{
    return  GET( m_uiNexonSN ) &&
            GET( m_wstrUserID ) &&
            GET( m_wstrUserName ) &&
            GET( m_wstrCouponSerial ) &&
            GET( m_iGameType ) &&
            GET( m_iServerDomain ) &&
            GET( m_wstrUnitNickName ) &&
            GET( m_iReason ) &&
            GET( m_wstrCafeNo );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_KNM_INVITE_REQ, obj, ks )
{
	return  PUT( m_wstrInviteUnitNickName ) &&
			PUT( m_wstrMyNickName ) &&
			PUT( m_kMyUnitInfo );
}

SERIALIZE_DEFINE_GET( KELG_KNM_INVITE_REQ, obj, ks )
{
	return  GET( m_wstrInviteUnitNickName ) &&
			GET( m_wstrMyNickName ) &&
			GET( m_kMyUnitInfo );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_NPC_UNIT_DIE_NOT, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_cDifficulty )
		&& PUT( m_nDieNPCUID )
		&& PUT( m_iNPCID )
		&& PUT( m_cAttUnitType )
		&& PUT( m_uiAttUnit )
		&& PUT( m_EXPList )
        //{{ 2009. 5. 13  최육사	내구도소모개편
		&& PUT( m_sSubStageExpNpcNum )
		&& PUT( m_cUserCount )
		&& PUT( m_bNoDrop )
		&& PUT( m_bDecreaseEndurance )
		//}}
		//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_bIsPvpNpc )
		&& PUT( m_iGameType )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 08. 31  김민성	비밀던전 헬모드
#ifdef SERV_RANDOM_DAY_QUEST
		&& PUT( m_cDungeonMode )
#endif SERV_RANDOM_DAY_QUEST
		//}}
		&& PUT( m_bQuestComplete )
#ifdef PVP_QUEST_HERO_KILL_COUNT
		&& PUT( m_bHeroNPC )
#endif //PVP_QUEST_HERO_KILL_COUNT
		;
}

SERIALIZE_DEFINE_GET( KERM_NPC_UNIT_DIE_NOT, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_cDifficulty )
		&& GET( m_nDieNPCUID )
		&& GET( m_iNPCID )
		&& GET( m_cAttUnitType )
		&& GET( m_uiAttUnit )
		&& GET( m_EXPList )
		//{{ 2009. 5. 13  최육사	내구도소모개편
		&& GET( m_sSubStageExpNpcNum )
		&& GET( m_cUserCount )
		&& GET( m_bNoDrop )
		&& GET( m_bDecreaseEndurance )
		//}}
		//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_bIsPvpNpc )
		&& GET( m_iGameType )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 08. 31  김민성	비밀던전 헬모드
#ifdef SERV_RANDOM_DAY_QUEST
		&& GET( m_cDungeonMode )
#endif SERV_RANDOM_DAY_QUEST
		//}}
		&& GET( m_bQuestComplete )
#ifdef PVP_QUEST_HERO_KILL_COUNT
		&& GET( m_bHeroNPC )
#endif //PVP_QUEST_HERO_KILL_COUNT
        ;
}

SERIALIZE_DEFINE_PUT( KDBE_PURCHASED_CASH_ITEM_LIST_REQ, obj, ks )
{
    return  PUT( m_nItemPerPage ) &&
            PUT( m_iCurrentPage ) &&
            PUT( m_wstrUnitNickName );
}

SERIALIZE_DEFINE_GET( KDBE_PURCHASED_CASH_ITEM_LIST_REQ, obj, ks )
{
    return  GET( m_nItemPerPage ) &&
            GET( m_iCurrentPage ) &&
            GET( m_wstrUnitNickName );
}

//{{ 2007. 11. 12  최육사  치트 로그
SERIALIZE_DEFINE_PUT( KELOG_CHEAT_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cAuthLevel )
		&& PUT( m_wsCheatCode )
		&& PUT( m_wsCheatContents )
		;
}

SERIALIZE_DEFINE_GET( KELOG_CHEAT_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_cAuthLevel )
		&& GET( m_wsCheatCode )
		&& GET( m_wsCheatContents )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_GAME_START_NOT, obj, ks )
{
	return PUT( m_cRoomState )
		&& PUT( m_iGameType )
		&& PUT( m_vecSlot )
		&& PUT( m_iDungeonID )
        //{{ 2008. 5. 27  최육사  랜덤맵
        && PUT( m_sWorldID )
        //}}
        ;
}

SERIALIZE_DEFINE_GET( KERM_GAME_START_NOT, obj, ks )
{
	return GET( m_cRoomState )
		&& GET( m_iGameType )
		&& GET( m_vecSlot )
		&& GET( m_iDungeonID )
        //{{ 2008. 5. 27  최육사  랜덤맵
        && GET( m_sWorldID )
        //}}
        ;
}

SERIALIZE_DEFINE_PUT( KERM_USER_UNIT_DIE_NOT, obj, ks )
{
	return PUT( m_iGameType )
		&& PUT( m_KillerNPCUID )
		&& PUT( m_KillerUserUnitUID )
		&& PUT( m_MaxDamageKillerUserUnitUID )
		&& PUT( m_KilledUserUnitUID )
		//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		&& PUT( m_bOfficialMatch )
#endif SERV_NEW_PVP_QUEST
		//}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT
		&& PUT( m_bHeroNPC )
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		&& PUT( m_killedUserUnitType )
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		&& PUT( m_killedUserUnitClass )
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		;
}

SERIALIZE_DEFINE_GET( KERM_USER_UNIT_DIE_NOT, obj, ks )
{
	return GET( m_iGameType )
		&& GET( m_KillerNPCUID )
		&& GET( m_KillerUserUnitUID )
		&& GET( m_MaxDamageKillerUserUnitUID )
		&& GET( m_KilledUserUnitUID )
		//{{ 2011. 07. 25    김민성    대전 퀘스트 조건 추가
#ifdef SERV_NEW_PVP_QUEST
		&& GET( m_bOfficialMatch )
#endif SERV_NEW_PVP_QUEST
		//}} 
#ifdef PVP_QUEST_HERO_KILL_COUNT
		&& GET( m_bHeroNPC )
#endif //PVP_QUEST_HERO_KILL_COUNT
#ifdef SERV_PVP_QUEST_OF_CHARCTER_KILL
		&& GET( m_killedUserUnitType )
#endif //SERV_PVP_QUEST_OF_CHARCTER_KILL
#ifdef SERV_EVENT_QUEST_CHUNG_PVP_KILL
		&& GET( m_killedUserUnitClass )
#endif SERV_EVENT_QUEST_CHUNG_PVP_KILL
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_PURCHASED_CASH_ITEM_REQ, obj, ks )
{
    return  PUT( m_iOrderNo ) &&
            PUT( m_iProductNo ) &&
            PUT( m_iPackageProductNo ) &&
            PUT( m_iQuantity ) &&
            PUT( m_iItemID );
}

SERIALIZE_DEFINE_GET( KDBE_GET_PURCHASED_CASH_ITEM_REQ, obj, ks )
{
    return  GET( m_iOrderNo ) &&
            GET( m_iProductNo ) &&
            GET( m_iPackageProductNo ) &&
            GET( m_iQuantity ) &&
            GET( m_iItemID );
}

SERIALIZE_DEFINE_PUT( KDBE_GET_PURCHASED_CASH_ITEM_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iItemID ) &&
            PUT( m_iQuantity );
}

SERIALIZE_DEFINE_GET( KDBE_GET_PURCHASED_CASH_ITEM_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iItemID ) &&
            GET( m_iQuantity );
}

SERIALIZE_DEFINE_PUT( KDBE_ROLLBACK_PURCHASED_CASH_ITEM_NOT, obj, ks )
{
    return  PUT( m_iOrderNo ) &&
            PUT( m_iProductNo ) &&
            PUT( m_wstrMsg );
}

SERIALIZE_DEFINE_GET( KDBE_ROLLBACK_PURCHASED_CASH_ITEM_NOT, obj, ks )
{
    return  GET( m_iOrderNo ) &&
            GET( m_iProductNo ) &&
            GET( m_wstrMsg );
}

//{{ 2007. 12. 17  최육사  센터 서버 유저 통계
SERIALIZE_DEFINE_PUT( KERM_PVP_USER_STATISTICS_NOT, obj, ks )
{
	return PUT( m_cGameType )
		&& PUT( m_cGameResult )
		&& PUT( m_sWorldID )
		&& PUT( m_bIntrude )
		&& PUT( m_iGamePlayTime )
		&& PUT( m_iKillNum )
		;
}

SERIALIZE_DEFINE_GET( KERM_PVP_USER_STATISTICS_NOT, obj, ks )
{
	return GET( m_cGameType )
		&& GET( m_cGameResult )
		&& GET( m_sWorldID )
		&& GET( m_bIntrude )
		&& GET( m_iGamePlayTime )
		&& GET( m_iKillNum )
		;
}

SERIALIZE_DEFINE_PUT( KERM_DUNGEON_USER_STATISTICS_NOT, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_cGameResult )
		&& PUT( m_bIsPartyPlay )
		&& PUT( m_iGamePlayTime )		
		&& PUT( m_iUsedResurrectionStone )
		;
}

SERIALIZE_DEFINE_GET( KERM_DUNGEON_USER_STATISTICS_NOT, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_cGameResult )
		&& GET( m_bIsPartyPlay )
		&& GET( m_iGamePlayTime )
		&& GET( m_iUsedResurrectionStone )
		;
}

SERIALIZE_DEFINE_PUT( KERM_TC_USER_STATISTICS_NOT, obj, ks )
{
	return PUT( m_iTCID )
		&& PUT( m_cGameResult )
		&& PUT( m_iGamePlayTime )
		;
}

SERIALIZE_DEFINE_GET( KERM_TC_USER_STATISTICS_NOT, obj, ks )
{
	return GET( m_iTCID )
		&& GET( m_cGameResult )
		&& GET( m_iGamePlayTime )
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 3. 31  최육사	TR 사용
SERIALIZE_DEFINE_PUT( KETR_CHAT_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_cChatPacketType )
		&& PUT( m_SenderUnitUID )
		&& PUT( m_wstrSenderUnitNickName )
		&& PUT( m_wstrRecieverUnitNickName )
		&& PUT( m_RecieverUnitUID )
		&& PUT( m_cRoomChatType )
		&& PUT( m_wstrMsg )
		;
}

SERIALIZE_DEFINE_GET( KETR_CHAT_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_cChatPacketType )
		&& GET( m_SenderUnitUID )
		&& GET( m_wstrSenderUnitNickName )
		&& GET( m_wstrRecieverUnitNickName )
		&& GET( m_RecieverUnitUID )
		&& GET( m_cRoomChatType )
		&& GET( m_wstrMsg )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CHAT_REQ, obj, ks )
{
	return PUT( m_cChatPacketType )
		//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_iChatData )
#endif GUILD_TEST
		//}}
		//{{ 2008. 2. 1  최육사  
		&& PUT( m_iSenderUnitUID )
		//}}
		&& PUT( m_wstrSenderUnitNickName )
		&& PUT( m_wstrRecieverUnitNickName )
		&& PUT( m_wstrMsg )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHAT_REQ, obj, ks )
{
	return GET( m_cChatPacketType )
		//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_iChatData )
#endif GUILD_TEST
		//}}
		//{{ 2008. 2. 1  최육사  
		&& GET( m_iSenderUnitUID )
		//}}
		&& GET( m_wstrSenderUnitNickName )
		&& GET( m_wstrRecieverUnitNickName )
		&& GET( m_wstrMsg )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CHAT_NOT, obj, ks )
{
	return PUT( m_cChatPacketType )
		//{{ 2008. 2. 1  최육사  
		&& PUT( m_iSenderUnitUID )
		//}}
		&& PUT( m_wstrSenderUnitNickName )
        && PUT( m_iReceiverUnitUID )
		&& PUT( m_wstrRecieverUnitNickName )
		&& PUT( m_wstrMsg )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHAT_NOT, obj, ks )
{
	return GET( m_cChatPacketType )
		//{{ 2008. 2. 1  최육사  
		&& GET( m_iSenderUnitUID )
		//}}
		&& GET( m_wstrSenderUnitNickName )
        && GET( m_iReceiverUnitUID )
		&& GET( m_wstrRecieverUnitNickName )
		&& GET( m_wstrMsg )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_CHECK_INVALID_ZOMBIE_USER_NOT, obj, ks )
{
	return PUT( m_iRoomUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHECK_INVALID_ZOMBIE_USER_NOT, obj, ks )
{
	return GET( m_iRoomUID )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_INITIALIZE_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_wstrServiceCode ) &&
            PUT( m_byteDomain );
}

SERIALIZE_DEFINE_GET( KENX_BT_INITIALIZE_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_wstrServiceCode ) &&
            GET( m_byteDomain );
}

SERIALIZE_DEFINE_PUT( KENX_BT_INITIALIZE_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_wstrServiceCode ) &&
            PUT( m_byteDomain ) &&
            PUT( m_ulResult );
}

SERIALIZE_DEFINE_GET( KENX_BT_INITIALIZE_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_wstrServiceCode ) &&
            GET( m_byteDomain ) &&
            GET( m_ulResult );
}

SERIALIZE_DEFINE_PUT( KENX_BT_HEART_BEAT_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_iReleaseTick );
}

SERIALIZE_DEFINE_GET( KENX_BT_HEART_BEAT_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_iReleaseTick );
}

SERIALIZE_DEFINE_PUT( KENX_BT_HEART_BEAT_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult );
}

SERIALIZE_DEFINE_GET( KENX_BT_HEART_BEAT_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult );
}

SERIALIZE_DEFINE_PUT( KENX_BT_CATEGORY_INQUIRY_REQ, obj,ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType );
}

SERIALIZE_DEFINE_GET( KENX_BT_CATEGORY_INQUIRY_REQ, obj,ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType );
}

SERIALIZE_DEFINE_PUT( KENX_BT_CATEGORY_INQUIRY_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_vecNXBTCategoryInfo );
}

SERIALIZE_DEFINE_GET( KENX_BT_CATEGORY_INQUIRY_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_vecNXBTCategoryInfo );
}

SERIALIZE_DEFINE_PUT( KENX_BT_PRODUCT_INQUIRY_XML_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulPageIndex ) &&
            PUT( m_ulRowPerPage );
}

SERIALIZE_DEFINE_GET( KENX_BT_PRODUCT_INQUIRY_XML_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulPageIndex ) &&
            GET( m_ulRowPerPage );
}

SERIALIZE_DEFINE_PUT( KENX_BT_PRODUCT_INQUIRY_XML_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_iReleaseTick ) &&
            PUT( m_ulTotalProductCount ) &&
            PUT( m_wstrResultXML );
}

SERIALIZE_DEFINE_GET( KENX_BT_PRODUCT_INQUIRY_XML_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_iReleaseTick ) &&
            GET( m_ulTotalProductCount ) &&
            GET( m_wstrResultXML );
}

SERIALIZE_DEFINE_PUT( KENX_BT_PRODUCT_INQUIRY_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulPageIndex ) &&
            PUT( m_ulRowPerPage );
}

SERIALIZE_DEFINE_GET( KENX_BT_PRODUCT_INQUIRY_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulPageIndex ) &&
            GET( m_ulRowPerPage );
}

SERIALIZE_DEFINE_PUT( KENX_BT_PRODUCT_INQUIRY_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_iReleaseTick ) &&
            PUT( m_ulTotalProductCount ) &&
            PUT( m_vecNXBTProductInfo );
}

SERIALIZE_DEFINE_GET( KENX_BT_PRODUCT_INQUIRY_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_iReleaseTick ) &&
            GET( m_ulTotalProductCount ) &&
            GET( m_vecNXBTProductInfo );
}

SERIALIZE_DEFINE_PUT( KENX_BT_CHECK_BALANCE_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
			//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
			PUT( m_ulPaymentType ) &&
#endif SERV_TOONILAND_CHANNELING
			//}}
            PUT( m_wstrUserID );
}

SERIALIZE_DEFINE_GET( KENX_BT_CHECK_BALANCE_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
			//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
			GET( m_ulPaymentType ) &&
#endif SERV_TOONILAND_CHANNELING
			//}}
            GET( m_wstrUserID );
}

SERIALIZE_DEFINE_PUT( KENX_BT_CHECK_BALANCE_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_ulBalance );
}

SERIALIZE_DEFINE_GET( KENX_BT_CHECK_BALANCE_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_ulBalance );
}

SERIALIZE_DEFINE_PUT( KENX_BT_PURCHASE_ITEM_REQ, obj, ks )
{
    return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulRemoteIP )
		&& PUT( m_byteReason )
		&& PUT( m_wstrGameID )
		&& PUT( m_wstrUserID )
		&& PUT( m_ulUserOID )
		&& PUT( m_wstrUserName )
		&& PUT( m_byteUserAge )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
		&& PUT( m_byteIsCafe )
#endif // SERV_NEXON_COUPON_SYSTEM
		&& PUT( m_wstrOrderID )
		&& PUT( m_ulPaymentType )
		&& PUT( m_ulTotalAmount )
		&& PUT( m_vecNXBTPurchaseInfo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_PURCHASE_ITEM_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulRemoteIP )
		&& GET( m_byteReason )
		&& GET( m_wstrGameID )
		&& GET( m_wstrUserID )
		&& GET( m_ulUserOID )
		&& GET( m_wstrUserName )
		&& GET( m_byteUserAge )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
		&& GET( m_byteIsCafe )
#endif // SERV_NEXON_COUPON_SYSTEM
		&& GET( m_wstrOrderID )
		&& GET( m_ulPaymentType )
		&& GET( m_ulTotalAmount )
		&& GET( m_vecNXBTPurchaseInfo )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_PURCHASE_ITEM_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_wstrOrderID ) &&
            PUT( m_ulResult ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_vecNXBTPurchaseInfo );
}

SERIALIZE_DEFINE_GET( KENX_BT_PURCHASE_ITEM_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_wstrOrderID ) &&
            GET( m_ulResult ) &&
            GET( m_ulOrderNo ) &&
            GET( m_vecNXBTPurchaseInfo );
}

SERIALIZE_DEFINE_PUT( KENX_BT_PURCHASE_GIFT_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulRemoteIP )
		&& PUT( m_byteReason )
		&& PUT( m_wstrSenderGameID )
		&& PUT( m_wstrSenderUserID )
		&& PUT( m_ulSenderUserOID )
		&& PUT( m_wstrSenderUserName )
		&& PUT( m_byteSenderUserAge )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& PUT( m_byteIsCafe )
#endif // SERV_NEXON_COUPON_SYSTEM
		&& PUT( m_byteReceiverServerNo )
		&& PUT( m_wstrReceiverGameID )
		&& PUT( m_wstrMessage )
		&& PUT( m_wstrOrderID )
		&& PUT( m_ulPaymentType )
		&& PUT( m_ulTotalAmount )
		&& PUT( m_vecNXBTPurchaseInfo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_PURCHASE_GIFT_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulRemoteIP )
		&& GET( m_byteReason )
		&& GET( m_wstrSenderGameID )
		&& GET( m_wstrSenderUserID )
		&& GET( m_ulSenderUserOID )
		&& GET( m_wstrSenderUserName )
		&& GET( m_byteSenderUserAge )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& GET( m_byteIsCafe )
#endif // SERV_NEXON_COUPON_SYSTEM
		&& GET( m_byteReceiverServerNo )
		&& GET( m_wstrReceiverGameID )
		&& GET( m_wstrMessage )
		&& GET( m_wstrOrderID )
		&& GET( m_ulPaymentType )
		&& GET( m_ulTotalAmount )
		&& GET( m_vecNXBTPurchaseInfo )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_PURCHASE_GIFT_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_wstrOrderID ) &&
            PUT( m_ulResult ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_vecNXBTPurchaseInfo );
}

SERIALIZE_DEFINE_GET( KENX_BT_PURCHASE_GIFT_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_wstrOrderID ) &&
            GET( m_ulResult ) &&
            GET( m_ulOrderNo ) &&
            GET( m_vecNXBTPurchaseInfo );
}

//{{ 2009. 10. 14  최육사	자동결제
SERIALIZE_DEFINE_PUT( KENX_BT_CHECK_AUTO_PAYMENT_USER_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_wstrGameID )
		&& PUT( m_wstrUserID )
		&& PUT( m_ulProductNo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_CHECK_AUTO_PAYMENT_USER_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_wstrGameID )
		&& GET( m_wstrUserID )
		&& GET( m_ulProductNo )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_CHECK_AUTO_PAYMENT_USER_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_CHECK_AUTO_PAYMENT_USER_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KENX_BT_COUPON_INQUIRY_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_byteIsCafe ) &&
            PUT( m_wstrCouponString );
}

SERIALIZE_DEFINE_GET( KENX_BT_COUPON_INQUIRY_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_byteIsCafe ) &&
            GET( m_wstrCouponString );
}

SERIALIZE_DEFINE_PUT( KENX_BT_COUPON_INQUIRY_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_vecProductInfo );
}

SERIALIZE_DEFINE_GET( KENX_BT_COUPON_INQUIRY_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_vecProductInfo );
}


//{{ 2009. 3. 24  최육사	쿠폰사용여부 조회
SERIALIZE_DEFINE_PUT( KENX_BT_COUPON_INQUIRY_NEW_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulUserIP )
		&& PUT( m_wstrGameID )
		&& PUT( m_byteIsCafe )
		&& PUT( m_wstrUserID )
		&& PUT( m_ulUserOID )
		&& PUT( m_wstrCouponString )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_COUPON_INQUIRY_NEW_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulUserIP )
		&& GET( m_wstrGameID )
		&& GET( m_byteIsCafe )
		&& GET( m_wstrUserID )
		&& GET( m_ulUserOID )
		&& GET( m_wstrCouponString )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_COUPON_INQUIRY_NEW_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		&& PUT( m_vecProductInfo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_COUPON_INQUIRY_NEW_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		&& GET( m_vecProductInfo )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KENX_BT_COUPON_USING_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulRemoteIP ) &&
            PUT( m_byteReason ) &&
            PUT( m_wstrGameID ) &&
            PUT( m_byteIsCafe ) &&
            PUT( m_wstrUserID ) &&
            PUT( m_ulUserOID ) &&
            PUT( m_wstrUserName ) &&
            PUT( m_wstrCouponString );
}

SERIALIZE_DEFINE_GET( KENX_BT_COUPON_USING_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulRemoteIP ) &&
            GET( m_byteReason ) &&
            GET( m_wstrGameID ) &&
            GET( m_byteIsCafe ) &&
            GET( m_wstrUserID ) &&
            GET( m_ulUserOID ) &&
            GET( m_wstrUserName ) &&
            GET( m_wstrCouponString );
}

SERIALIZE_DEFINE_PUT( KENX_BT_COUPON_USING_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_wstrCouponTXID ) &&
            PUT( m_ulResult ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_vecCouponProductInfo );
}

SERIALIZE_DEFINE_GET( KENX_BT_COUPON_USING_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_wstrCouponTXID ) &&
            GET( m_ulResult ) &&
            GET( m_ulOrderNo ) &&
            GET( m_vecCouponProductInfo );
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_INQUIRY_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_wstrGameID ) &&
            PUT( m_byteShowInventory ) &&
            PUT( m_ulPageIndex ) &&
            PUT( m_ulRowPerPage );
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_INQUIRY_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_wstrGameID ) &&
            GET( m_byteShowInventory ) &&
            GET( m_ulPageIndex ) &&
            GET( m_ulRowPerPage );
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_INQUIRY_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_ulTotalCount ) &&
            PUT( m_vecNXBTOrderInfo ) &&
            PUT( m_vecNXBTPackageInfo ) &&
            PUT( m_vecNXBTLotteryInfo );
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_INQUIRY_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_ulTotalCount ) &&
            GET( m_vecNXBTOrderInfo ) &&
            GET( m_vecNXBTPackageInfo ) &&
            GET( m_vecNXBTLotteryInfo );
}

//{{ 2008. 6. 20  최육사  선물하기
SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_CHECK_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_wstrGameID )
		&& PUT( m_byteShowInventory )
		&& PUT( m_byteIsPresent )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_CHECK_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_wstrGameID )
		&& GET( m_byteShowInventory )
		&& GET( m_byteIsPresent )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_CHECK_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		&& PUT( m_byteOrderType )
		&& PUT( m_ulTotalCount )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_CHECK_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		&& GET( m_byteOrderType )
		&& GET( m_ulTotalCount )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_READ_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_ulProductNo );
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_READ_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulOrderNo ) &&
            GET( m_ulProductNo );
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_READ_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult );
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_READ_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult );
}

//{{ 2008. 6. 26  최육사  선물하기
SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_READABLE_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_wstrGameID )
		&& PUT( m_byteShowInventory )
		&& PUT( m_byteIsPresent )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_READABLE_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_wstrGameID )
		&& GET( m_byteShowInventory )
		&& GET( m_byteIsPresent )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_READABLE_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		&& PUT( m_byteOrderType )
		&& PUT( m_ulTotalCount )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_READABLE_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		&& GET( m_byteOrderType )
		&& GET( m_ulTotalCount )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
			PUT( m_usOrderQuantity ) &&
			PUT( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulOrderNo ) &&
            GET( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
			GET( m_usOrderQuantity ) &&
			GET( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
			PUT( m_usOrderQuantity ) &&
			PUT( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_ulOrderNo ) &&
            GET( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  BYTE -> unsigned short  및 변수 추가
			GET( m_usOrderQuantity ) &&
			GET( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  변수 추가
			PUT( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulOrderNo ) &&
            GET( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  변수 추가
			GET( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulResult ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  변수 추가
			PUT( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_ROLLBACK_ACK, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulResult ) &&
            GET( m_ulOrderNo ) &&
			GET( m_ulProductNo ) &&
			//{{ 2008. 1. 29  최육사  변수 추가
			GET( m_wstrExtendValue );
			//}}
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ, obj, ks )
{
    return  PUT( m_ulPacketNo ) &&
            PUT( m_bytePacketType ) &&
            PUT( m_ulOrderNo ) &&
            PUT( m_ulProductNo ) &&
            PUT( m_usOrderQuantity ) &&
            PUT( m_wstrExtendValue );
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_REQ, obj, ks )
{
    return  GET( m_ulPacketNo ) &&
            GET( m_bytePacketType ) &&
            GET( m_ulOrderNo ) &&
            GET( m_ulProductNo ) &&
            GET( m_usOrderQuantity ) &&
            GET( m_wstrExtendValue );
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK, obj, ks )
{
    return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		&& PUT( m_ulOrderNo )
		&& PUT( m_ulProductNo )
		&& PUT( m_byteProductKind )
		&& PUT( m_wstrProductName )
		&& PUT( m_wstrProductID )
		&& PUT( m_usProductExpire )
		&& PUT( m_usProductPieces )
		&& PUT( m_usOrderQuantity )
		&& PUT( m_wstrProductAttribute0 )
		&& PUT( m_wstrProductAttribute1 )
		&& PUT( m_wstrProductAttribute2 )
		&& PUT( m_wstrProductAttribute3 )
		&& PUT( m_wstrProductAttribute4 )
		&& PUT( m_wstrExtendValue )		
		&& PUT( m_vecNXBTSubProductInfo )
		//{{ 2008. 7. 10  최육사  추가 변수
		&& PUT( m_vecKNXBTBonusProductInfo )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_ONCE_ACK, obj, ks )
{
    return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		&& GET( m_ulOrderNo )
		&& GET( m_ulProductNo )
		&& GET( m_byteProductKind )
		&& GET( m_wstrProductName )
		&& GET( m_wstrProductID )
		&& GET( m_usProductExpire )
		&& GET( m_usProductPieces )
		&& GET( m_usOrderQuantity )
		&& GET( m_wstrProductAttribute0 )
		&& GET( m_wstrProductAttribute1 )
		&& GET( m_wstrProductAttribute2 )
		&& GET( m_wstrProductAttribute3 )
		&& GET( m_wstrProductAttribute4 )
		&& GET( m_wstrExtendValue )
		&& GET( m_vecNXBTSubProductInfo )
		//{{ 2008. 7. 10  최육사  추가 변수
		&& GET( m_vecKNXBTBonusProductInfo )
		//}}
		;
}

//{{ 2008. 6. 16  최육사  넥슨 빌링 패키지 픽업
SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulOrderNo )
		&& PUT( m_ulPackageProductNo )
		&& PUT( m_ulProductNo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulOrderNo )
		&& GET( m_ulPackageProductNo )
		&& GET( m_ulProductNo )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		&& PUT( m_ulOrderNo )
		&& PUT( m_ulPackageProductNo )
		&& PUT( m_ulProductNo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& PUT( m_ulSubProductNo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_INVENTORY_PICK_UP_FOR_PACKAGE_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		&& GET( m_ulOrderNo )
		&& GET( m_ulPackageProductNo )
		&& GET( m_ulProductNo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& GET( m_ulSubProductNo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}
//}}


//{{ 2010. 11. 17	최육사	넥슨 빌링 청약 철회
#ifdef SERV_NX_BILLING_REFUND

SERIALIZE_DEFINE_PUT( KENX_BT_REFUND_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulRemoteIP )
		&& PUT( m_wstrRequestID )
		&& PUT( m_wstrGameID )
		&& PUT( m_ulOrderNo )
		&& PUT( m_ulProductNo )
		&& PUT( m_usQuantity )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_REFUND_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulRemoteIP )
		&& GET( m_wstrRequestID )
		&& GET( m_wstrGameID )
		&& GET( m_ulOrderNo )
		&& GET( m_ulProductNo )
		&& GET( m_usQuantity )
		;
}


SERIALIZE_DEFINE_PUT( KENX_BT_REFUND_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_REFUND_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		;
}

#endif SERV_NX_BILLING_REFUND
//}}


//{{ 2008. 2. 4  최육사  블랙리스트
SERIALIZE_DEFINE_PUT( KELG_SEARCH_BLACK_LIST_UNIT_REQ, obj, ks )
{
	return PUT( m_wstrNickName );
}

SERIALIZE_DEFINE_GET( KELG_SEARCH_BLACK_LIST_UNIT_REQ, obj, ks )
{
	return GET( m_wstrNickName );
}

SERIALIZE_DEFINE_PUT( KELG_SEARCH_BLACK_LIST_UNIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_SEARCH_BLACK_LIST_UNIT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_OPEN_PERSONAL_SHOP_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iED )
		&& PUT( m_wstrNickName )
		&& PUT( m_cPersonalShopType )		
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_PERSONAL_SHOP_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iED )
		&& GET( m_wstrNickName )
		&& GET( m_cPersonalShopType )		
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_OPEN_PERSONAL_SHOP_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPersonalShopUID )
		&& PUT( m_cPersonalShopType )
		//{{ 2011. 04. 12	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_iGSUID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrAgencyOpenDate )
#endif SERV_PSHOP_AGENCY
		//}}
		;
};

SERIALIZE_DEFINE_GET( KERM_OPEN_PERSONAL_SHOP_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPersonalShopUID )
		&& GET( m_cPersonalShopType )
		//{{ 2011. 04. 12	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_iGSUID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrAgencyOpenDate )
#endif SERV_PSHOP_AGENCY
		//}}
		;
};

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_REG_PERSONAL_SHOP_ITEM_REQ, obj, ks )
{
	return PUT( m_wstrPersonalShopName )
		&& PUT( m_vecSellItemInfo )
		//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_vecInventorySlotInfo )
#endif SERV_PSHOP_AGENCY
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_REG_PERSONAL_SHOP_ITEM_REQ, obj, ks )
{
	return GET( m_wstrPersonalShopName )
		&& GET( m_vecSellItemInfo )
		//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_vecInventorySlotInfo )
#endif SERV_PSHOP_AGENCY
		//}}
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_REG_PERSONAL_SHOP_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		//{{ 2011. 04. 14	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_vecInventorySlotInfo )
#endif SERV_PSHOP_AGENCY
		//}}
		&& PUT( m_iOpeningED )
		&& PUT( m_wstrPersonalShopName )
		&& PUT( m_vecSellItemInfo )
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& PUT( m_vecAddCompleteItemInfo )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_REG_PERSONAL_SHOP_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		//{{ 2011. 04. 14	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_vecInventorySlotInfo )
#endif SERV_PSHOP_AGENCY
		//}}
		&& GET( m_iOpeningED )
		&& GET( m_wstrPersonalShopName )
		&& GET( m_vecSellItemInfo )
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& GET( m_vecAddCompleteItemInfo )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		;
}
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_CHECK_JOIN_PERSONAL_SHOP_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_iPersonalShopUID );
}

SERIALIZE_DEFINE_GET( KERM_CHECK_JOIN_PERSONAL_SHOP_ACK, obj, ks )
{
	return  GET( m_iOK )
		&&	GET( m_iPersonalShopUID );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_JOIN_PERSONAL_SHOP_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		&& PUT( m_iUnitUID )
#endif SERV_INTEGRATION
		//}}
		&& PUT( m_wstrNickName )
		&& PUT( m_bIsSquare )
		//{{ 2011. 05. 06	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_cJoinType )
#endif SERV_PSHOP_AGENCY

#ifdef SERV_UPGRADE_TRADE_SYSTEM
        && PUT( m_iUsedItemID ) // 상점 업그레이드 아이템(보증서)
#endif SERV_UPGRADE_TRADE_SYSTEM

		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_JOIN_PERSONAL_SHOP_REQ, obj, ks )
{
	return GET( m_iUserUID )
		//{{ 2010. 05. 18  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
		&& GET( m_iUnitUID )
#endif SERV_INTEGRATION
		//}}
		&& GET( m_wstrNickName )
		&& GET( m_bIsSquare )
		//{{ 2011. 05. 06	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_cJoinType )
#endif SERV_PSHOP_AGENCY
		//}}
#ifdef SERV_UPGRADE_TRADE_SYSTEM
        && GET( m_iUsedItemID ) // 상점 업그레이드 아이템(보증서)
#endif SERV_UPGRADE_TRADE_SYSTEM

		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_JOIN_PERSONAL_SHOP_ACK, obj, ks )
{
	return PUT( m_iOK )
		//{{ 2011. 07. 19	최육사	대리상점 구매 수수료 없앤 이벤트
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
		&& PUT( m_bIsPShopAgency )
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
		//}}
		&& PUT( m_iPersonalShopUID )
		&& PUT( m_wstrPersonalShopName )
		&& PUT( m_wstrNickName )
		&& PUT( m_vecSellItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_JOIN_PERSONAL_SHOP_ACK, obj, ks )
{
	return GET( m_iOK )
		//{{ 2011. 07. 19	최육사	대리상점 구매 수수료 없앤 이벤트
#ifdef SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
		&& GET( m_bIsPShopAgency )
#endif SERV_PSHOP_AGENCY_NO_COMMISSION_EVENT
		//}}
		&& GET( m_iPersonalShopUID )
		&& GET( m_wstrPersonalShopName )
		&& GET( m_wstrNickName )
		&& GET( m_vecSellItemInfo )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_LEAVE_PERSONAL_SHOP_REQ, obj, ks )
{
	return  PUT( m_iReason );
}

SERIALIZE_DEFINE_GET( KERM_LEAVE_PERSONAL_SHOP_REQ, obj, ks )
{
	return  GET( m_iReason );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_BUY_PERSONAL_SHOP_ITEM_REQ, obj, ks )
{
	return PUT( m_iBuyItemUID )
		&& PUT( m_iBuyItemID )
		&& PUT( m_iBuyQuantity )
		&& PUT( m_iBuyUnitNowED )
		&& PUT( m_kItemQuantityUpdateTemp )
		&& PUT( m_vecItemInfoTemp )
		&& PUT( m_iEDTemp )
		;
}

SERIALIZE_DEFINE_GET( KERM_BUY_PERSONAL_SHOP_ITEM_REQ, obj, ks )
{
	return GET( m_iBuyItemUID )
		&& GET( m_iBuyItemID )
		&& GET( m_iBuyQuantity )
		&& GET( m_iBuyUnitNowED )
		&& GET( m_kItemQuantityUpdateTemp )
		&& GET( m_vecItemInfoTemp )
		&& GET( m_iEDTemp )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, obj, ks )
{
	return PUT( m_iSellUnitUID )
		&& PUT( m_iSellUnitEDIN )
		&& PUT( m_iSellCommissionED )
		&& PUT( m_kSellIntemInfoOUT )
		&& PUT( m_iPriceUnitUID )
		&& PUT( m_iPriceUnitEDOUT )
		//{{ 2010. 03. 31  최육사	거래게시판
		&& PUT( m_iPriceCommissionED )
		//}}
		&& PUT( m_kPriceIntemInfoIN )
		//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD
		&& PUT( m_bIsSquare )
#endif DEF_TRADE_BOARD
		//}}		
		&& PUT( m_kItemQuantityUpdateTemp )
		&& PUT( m_vecItemInfoTemp )
		&& PUT( m_iEDTemp )
		;
}

SERIALIZE_DEFINE_GET( KERM_BUY_PERSONAL_SHOP_ITEM_COMPLETE_NOT, obj, ks )
{
	return GET( m_iSellUnitUID )
		&& GET( m_iSellUnitEDIN )
		&& GET( m_iSellCommissionED )
		&& GET( m_kSellIntemInfoOUT )
		&& GET( m_iPriceUnitUID )
		&& GET( m_iPriceUnitEDOUT )
		//{{ 2010. 03. 31  최육사	거래게시판
		&& GET( m_iPriceCommissionED )
		//}}
		&& GET( m_kPriceIntemInfoIN )
		//{{ 2010. 03. 31  최육사	거래게시판
#ifdef DEF_TRADE_BOARD
		&& GET( m_bIsSquare )
#endif DEF_TRADE_BOARD
		//}}		
		&& GET( m_kItemQuantityUpdateTemp )
		&& GET( m_vecItemInfoTemp )
		&& GET( m_iEDTemp )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ, obj, ks )
{
	return PUT( m_kSellItemInfo )
		&& PUT( m_iBuyQuantity )
		&& PUT( m_iBuyUnitUID )
		&& PUT( m_iTotalPrice )
		;
}

SERIALIZE_DEFINE_GET( KERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_REQ, obj, ks )
{
	return GET( m_kSellItemInfo )
		&& GET( m_iBuyQuantity )
		&& GET( m_iBuyUnitUID )
		&& GET( m_iTotalPrice )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iBuyUnitUID )
		&& PUT( m_iBuyItemUID )
		&& PUT( m_iBuyQuantity )
		&& PUT( m_kItemQuantityUpdateTemp )
		&& PUT( m_vecItemInfoTemp )
		&& PUT( m_iEDTemp )
		;
}

SERIALIZE_DEFINE_GET( KERM_CONFIRM_BUY_PERSONAL_SHOP_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iBuyUnitUID )
		&& GET( m_iBuyItemUID )
		&& GET( m_iBuyQuantity )
		&& GET( m_kItemQuantityUpdateTemp )
		&& GET( m_vecItemInfoTemp )
		&& GET( m_iEDTemp )
		;
}

SERIALIZE_DEFINE_PUT( KERM_BREAK_PERSONAL_SHOP_NOT, obj, ks )
{
	return PUT( m_iReason )
		&& PUT( m_iHostUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_BREAK_PERSONAL_SHOP_NOT, obj, ks )
{
	return GET( m_iReason )
		&& GET( m_iHostUID )
		;
}

//{{ 2009. 9. 22  최육사	전직캐쉬
SERIALIZE_DEFINE_PUT( KDBE_CHANGE_UNIT_CLASS_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_usEventID )
		&& PUT( m_cUnitClass )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iTheNumberOfSkillPagesAvailable )
#endif	// SERV_SKILL_PAGE_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_UNIT_CLASS_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_usEventID )
		&& GET( m_cUnitClass )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iTheNumberOfSkillPagesAvailable )
#endif	// SERV_SKILL_PAGE_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_UNIT_CLASS_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_usEventID )
		&& PUT( m_cUnitClass )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_UNIT_CLASS_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_usEventID )
		&& GET( m_cUnitClass )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}
//}}

SERIALIZE_DEFINE_PUT( KDBE_ENCHANT_ITEM_REQ, obj, ks )
{
    return PUT( m_iUnitUID ) 
		&& PUT( m_iEnchantResult ) 
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID ) 
		&& PUT( m_iLevelAfterEnchant ) 
		&& PUT( m_kItemQuantityUpdate ) 
		&& PUT( m_vecUpdatedInventorySlot ) 
		;
}

SERIALIZE_DEFINE_GET( KDBE_ENCHANT_ITEM_REQ, obj, ks )
{
    return GET( m_iUnitUID ) 
		&& GET( m_iEnchantResult ) 
		&& GET( m_iItemUID ) 
		&& GET( m_iItemID ) 
		&& GET( m_iLevelAfterEnchant ) 
		&& GET( m_kItemQuantityUpdate ) 
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ENCHANT_ITEM_ACK, obj, ks )
{
    return PUT( m_iOK )
		&& PUT( m_iEnchantResult ) 
		&& PUT( m_iItemUID ) 
		&& PUT( m_iItemID ) 
		&& PUT( m_iLevelAfterEnchant ) 
		&& PUT( m_kItemQuantityUpdate ) 
		&& PUT( m_vecUpdatedInventorySlot ) 
		;
}

SERIALIZE_DEFINE_GET( KDBE_ENCHANT_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iEnchantResult ) 
		&& GET( m_iItemUID ) 
		&& GET( m_iItemID ) 
		&& GET( m_iLevelAfterEnchant ) 
		&& GET( m_kItemQuantityUpdate ) 
		&& GET( m_vecUpdatedInventorySlot )
		;
}

//{{ 2008. 12. 18  최육사	보조제 로그
SERIALIZE_DEFINE_PUT( KELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_cBeforeLevel )
		&& PUT( m_cAfterLevel )
		//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가 - 로그 남기기
		// 파괴 방지까지 합쳐서 한번에 적용. 
#ifdef SERV_ENCHANT_PLUS_ITEM
		&& PUT( m_bUseSupport )
		&& PUT( m_bUsePlus )
		&& PUT( m_bUseDestroyGuard )
#endif
		//}}
		;
}

SERIALIZE_DEFINE_GET( KELOG_ENCHANT_SUPPORT_MATERIAL_LOG_NOT, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_cBeforeLevel )
		&& GET( m_cAfterLevel )
		//{{ 2011.5.23 지헌 : 강화시 성공 확률 증가 아이템 추가 - 로그 남기기
		// 파괴 방지까지 합쳐서 한번에 적용. 
#ifdef SERV_ENCHANT_PLUS_ITEM
		&& GET( m_bUseSupport )
		&& GET( m_bUsePlus )
		&& GET( m_bUseDestroyGuard )
#endif
		//}}
		;
}
//}}

//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KDBE_RESOLVE_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_bJackpot )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESOLVE_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_bJackpot )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RESOLVE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bJackpot )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESOLVE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bJackpot )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEventInfo, obj, ks )
{
	return PUT( m_iEventUID )
		&& PUT( m_iEventType )
		&& PUT( m_wstrEventName )
		&& PUT( m_wstrBeginDate )
		&& PUT( m_wstrEndDate )
		&& PUT( m_iScriptID )
		&& PUT( m_bEnable )
#ifdef SERV_ALLOW_EVENT_ERASE
		&& PUT( m_bOn )
#endif SERV_ALLOW_EVENT_ERASE
		;
}

SERIALIZE_DEFINE_GET( KEventInfo, obj, ks )
{
	return GET( m_iEventUID )
		&& GET( m_iEventType )
		&& GET( m_wstrEventName )
		&& GET( m_wstrBeginDate )
		&& GET( m_wstrEndDate )
		&& GET( m_iScriptID )
		&& GET( m_bEnable )
#ifdef SERV_ALLOW_EVENT_ERASE
		&& GET( m_bOn )
#endif SERV_ALLOW_EVENT_ERASE
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EVENT_UPDATE_ACK, obj, ks )
{
	return PUT( m_vecEventList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_UPDATE_ACK, obj, ks )
{
	return GET( m_vecEventList )
		;
}

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
SERIALIZE_DEFINE_PUT( KDBE_RELEASE_TICK_UPDATE_ACK, obj, ks )
{
	return PUT( m_mapReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RELEASE_TICK_UPDATE_ACK, obj, ks )
{
	return GET( m_mapReleaseTick )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_DATA_UPDATE_ACK, obj, ks )
{
	return PUT( m_mapEventData )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_DATA_UPDATE_ACK, obj, ks )
{
	return GET( m_mapEventData )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_MONSTER_UPDATE_ACK, obj, ks )
{
	return PUT( m_mapEventMonster )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_MONSTER_UPDATE_ACK, obj, ks )
{
	return GET( m_mapEventMonster )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK, obj, ks )
{
	return PUT( m_vecEventNotifyMsgServerInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_NOTIFY_MSG_SERVER_INFO_UPDATE_ACK, obj, ks )
{
	return GET( m_vecEventNotifyMsgServerInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_REWARD_DATA_UPDATE_ACK, obj, ks )
{
	return PUT( m_mapRewardData )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_REWARD_DATA_UPDATE_ACK, obj, ks )
{
	return GET( m_mapRewardData )
		;
}

//{{ 2011. 08. 29	최육사	캐릭터 레벨업 이벤트
#ifdef SERV_CHAR_LEVEL_UP_EVENT
SERIALIZE_DEFINE_PUT( KDBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK, obj, ks )
{
	return PUT( m_mapLevelUpRewardItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_REWARD_LEVEL_UP_UPDATE_ACK, obj, ks )
{
	return GET( m_mapLevelUpRewardItem )
		;
}
#endif SERV_CHAR_LEVEL_UP_EVENT
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
SERIALIZE_DEFINE_PUT( KDBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK, obj, ks )
{
	return PUT( m_mapRewardItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_REWARD_RETAIN_USER_ITEM_UPDATE_ACK, obj, ks )
{
	return GET( m_mapRewardItem )
		;
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2008. 2. 26  최육사  개인상점 로그
SERIALIZE_DEFINE_PUT( KELOG_INSERT_PERSONAL_SHOP_LOG_NOT, obj, ks )
{
	return PUT( m_bIsSquare )
		//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_cPShopType )
#endif SERV_PSHOP_AGENCY
		//}}
		&& PUT( m_iFromUnitUID )
		&& PUT( m_iToUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_iQuantity )
		&& PUT( m_iED )
		;
}

SERIALIZE_DEFINE_GET( KELOG_INSERT_PERSONAL_SHOP_LOG_NOT, obj, ks )
{
	return GET( m_bIsSquare )
		//{{ 2011. 05. 11	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_cPShopType )
#endif SERV_PSHOP_AGENCY
		//}}
		&& GET( m_iFromUnitUID )
		&& GET( m_iToUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_iQuantity )
		&& GET( m_iED )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_UNIT_CLASS_NOT, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_cUnitClass );
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_UNIT_CLASS_NOT, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_cUnitClass );
}

//{{ 2008. 3. 4  최육사  어뷰저 로그
SERIALIZE_DEFINE_PUT( KELOG_INSERT_ABUSER_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrIP )
		&& PUT( m_iGameType )
		&& PUT( m_fPlayTimeLimit )
		&& PUT( m_bPublic )
		&& PUT( m_iPlayWithMember )
		&& PUT( m_wstrEndGameTime )
		&& PUT( m_iPlayTime )
		&& PUT( m_iRewardVP )	
		&& PUT( m_iRewardEXP )
		&& PUT( m_iKillCount )
		&& PUT( m_iDeathCount )
		;
}

SERIALIZE_DEFINE_GET( KELOG_INSERT_ABUSER_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrIP )
		&& GET( m_iGameType )
		&& GET( m_fPlayTimeLimit )
		&& GET( m_bPublic )
		&& GET( m_iPlayWithMember )
		&& GET( m_wstrEndGameTime )
		&& GET( m_iPlayTime )
		&& GET( m_iRewardVP )	
		&& GET( m_iRewardEXP )
		&& GET( m_iKillCount )
		&& GET( m_iDeathCount )
		;
}
//}}

//{{ 2008. 3. 5  최육사  소켓
//{{ 2010. 04. 15  최육사	소켓 개편
//////////////////////////////////////////////////////////////////////////
#ifdef SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KDBE_SOCKET_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecSocketInfo )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& PUT( m_iSocketUseCount )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세
		&& PUT( m_byteExpandedSocketNum )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KDBE_SOCKET_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecSocketInfo )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& GET( m_iSocketUseCount )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		&& GET( m_byteExpandedSocketNum )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SOCKET_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecSocketInfo )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& PUT( m_iSocketUseCount )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_SOCKET_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecSocketInfo )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_kItemQuantityUpdate )	
		&& GET( m_vecUpdatedInventorySlot )
		//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& GET( m_iSocketUseCount )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		;
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////


SERIALIZE_DEFINE_PUT( KDBE_SOCKET_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecSocketInfo )
		&& PUT( m_iSocketResult )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_kItemQuantityUpdate )		
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SOCKET_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecSocketInfo )
		&& GET( m_iSocketResult )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_kItemQuantityUpdate )		
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SOCKET_ITEM_ACK, obj, ks )
{
	return PUT( m_vecSocketInfo )
		&& PUT( m_iSocketResult )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_kItemQuantityUpdate )		
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SOCKET_ITEM_ACK, obj, ks )
{
	return GET( m_vecSocketInfo )
		&& GET( m_iSocketResult )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_kItemQuantityUpdate )	
		&& GET( m_vecUpdatedInventorySlot )
		;
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_SOCKET_NEW
//////////////////////////////////////////////////////////////////////////
//}}
//}}

SERIALIZE_DEFINE_PUT( KERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT, obj, ks )
{
    return PUT( m_UnitUID ) 
		&& PUT( m_vecInventorySlotInfo )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iEquippedTitleID )
#else
		&& PUT( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_CHANGE_EQUIPPED_ITEM_IN_SQUARE_NOT, obj, ks )
{
    return GET( m_UnitUID )
		&& GET( m_vecInventorySlotInfo )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iEquippedTitleID )
#else
		&& GET( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, obj, ks )
{
	//{{ 2010. 12. 23	최육사	장비 변경 패킷 타입
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
	return PUT( m_cChangeEquippedPacketType )
		&& PUT( m_UnitUID )
#else
	return PUT( m_UnitUID )
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
	//}}
		&& PUT( m_vecInventorySlotInfo )
		&& PUT( m_kRoomUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHANGE_EQUIPPED_ITEM_IN_ROOM_NOT, obj, ks )
{
	//{{ 2010. 12. 23	최육사	장비 변경 패킷 타입
#ifdef SERV_CHANGE_EQUIPPED_PACKET_REFAC
	return GET( m_cChangeEquippedPacketType )
		&& GET( m_UnitUID )
#else
	return GET( m_UnitUID )
#endif SERV_CHANGE_EQUIPPED_PACKET_REFAC
		//}}
		&& GET( m_vecInventorySlotInfo )
		&& GET( m_kRoomUserInfo )
		;
}

//{{ 2008. 11. 30  최육사	
SERIALIZE_DEFINE_PUT( KERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecInventorySlotInfo )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iEquippedTitleID )
#else
		&& PUT( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecInventorySlotInfo )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iEquippedTitleID )
#else
		&& GET( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_vecInventorySlotInfo )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iEquippedTitleID )
#else
		&& PUT( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KETR_CHANGE_EQUIPPED_ITEM_IN_FIELD_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_vecInventorySlotInfo )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iEquippedTitleID )
#else
		&& GET( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		;
}
//}}

SERIALIZE_DEFINE_PUT( KELOG_RESURRECTION_STONE_LOG_NOT, obj, ks )
{
	return PUT( m_UnitUID )
		&& PUT( m_iDungeonID )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KELOG_RESURRECTION_STONE_LOG_NOT, obj, ks )
{
	return GET( m_UnitUID )
		&& GET( m_iDungeonID )
		&& GET( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_RESOLVE_JACKPOT_LOG_NOT, obj, ks )
{
	return PUT( m_UnitUID )
		&& PUT( m_iResolveItemID )
		&& PUT( m_iElapsedTime )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KELOG_RESOLVE_JACKPOT_LOG_NOT, obj, ks )
{
	return GET( m_UnitUID )
		&& GET( m_iResolveItemID )
		&& GET( m_iElapsedTime )
		&& GET( m_wstrRegDate )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INIT_SKILL_TREE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iItemUID )
		&& PUT( m_iSPoint )
		&& PUT( m_iCSPoint )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& PUT( m_iDefaultSkillID1 )
		&& PUT( m_iDefaultSkillID2 )
		&& PUT( m_iDefaultSkillID3 )
		&& PUT( m_iDefaultSkillID4 )
		&& PUT( m_iDefaultSkillID5 )
		&& PUT( m_iDefaultSkillID6 )
		&& PUT( m_iBeforeSPoint )
		&& PUT( m_iBeforeCSPoint )
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*	
		&& PUT( m_iDefaultSkillID )
		&& PUT( m_iDefaultSkillID2 )
*/		
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}

SERIALIZE_DEFINE_GET( KDBE_INIT_SKILL_TREE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iItemUID )
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& GET( m_iDefaultSkillID1 )
		&& GET( m_iDefaultSkillID2 )
		&& GET( m_iDefaultSkillID3 )
		&& GET( m_iDefaultSkillID4 )
		&& GET( m_iDefaultSkillID5 )
		&& GET( m_iDefaultSkillID6 )
		&& GET( m_iBeforeSPoint )
		&& GET( m_iBeforeCSPoint )
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*		
		&& GET( m_iDefaultSkillID )
		&& GET( m_iDefaultSkillID2 )
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INIT_SKILL_TREE_ACK, obj, ks )
{
	return PUT( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iItemUID )
		&& PUT( m_iSPoint )
		&& PUT( m_iCSPoint )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& PUT( m_iDefaultSkillID1 )
		&& PUT( m_iDefaultSkillID2 )
		&& PUT( m_iDefaultSkillID3 )
		&& PUT( m_iDefaultSkillID4 )
		&& PUT( m_iDefaultSkillID5 )
		&& PUT( m_iDefaultSkillID6 )
		&& PUT( m_iBeforeSPoint )
		&& PUT( m_iBeforeCSPoint )
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		&& PUT( m_iDefaultSkillID )
		&& PUT( m_iDefaultSkillID2 )
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}

SERIALIZE_DEFINE_GET( KDBE_INIT_SKILL_TREE_ACK, obj, ks )
{
	return GET( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iItemUID )
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint )
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& GET( m_iDefaultSkillID1 )
		&& GET( m_iDefaultSkillID2 )
		&& GET( m_iDefaultSkillID3 )
		&& GET( m_iDefaultSkillID4 )
		&& GET( m_iDefaultSkillID5 )
		&& GET( m_iDefaultSkillID6 )
		&& GET( m_iBeforeSPoint )
		&& GET( m_iBeforeCSPoint )
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*	
		&& GET( m_iDefaultSkillID )
		&& GET( m_iDefaultSkillID2 )
*/		
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}

//{{ 2008. 3. 28  최육사  보상
SERIALIZE_DEFINE_PUT( KDBE_RECOMMEND_USER_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RECOMMEND_USER_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RECOMMEND_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRecommendedUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RECOMMEND_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRecommendedUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_REWARD_TO_POST_REQ, obj, ks )
{
	return PUT( m_iFromUnitUID )
		&& PUT( m_iToUnitUID )
		&& PUT( m_iRewardType )
		&& PUT( m_iRewardID )
		&& PUT( m_sQuantity )		
		//{{ 2008. 9. 26  최육사	우체국 확장
		&& PUT( m_wstrMessage )
		//}}
		&& PUT( m_bGameServerEvent )
		//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		&& PUT( m_iDBIndex )
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_REWARD_TO_POST_REQ, obj, ks )
{
	return GET( m_iFromUnitUID )
		&& GET( m_iToUnitUID )
		&& GET( m_iRewardType )
		&& GET( m_iRewardID )
		&& GET( m_sQuantity )		
		//{{ 2008. 9. 26  최육사	우체국 확장
		&& GET( m_wstrMessage )
		//}}
		&& GET( m_bGameServerEvent )
		//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		&& GET( m_iDBIndex )
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_REWARD_TO_POST_ACK, obj, ks )
{
	return PUT( m_iOK )
		//{{ 2008. 9. 30  최육사	우체국 확장
		&& PUT( m_iRewardLetter )
		//}}
		//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		&& PUT( m_iDBIndex )
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}}		
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_REWARD_TO_POST_ACK, obj, ks )
{
	return GET( m_iOK )
		//{{ 2008. 9. 30  최육사	우체국 확장
		&& GET( m_iRewardLetter )
		//}}
		//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		&& GET( m_iDBIndex )
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}}
		;
}
//}}

//{{ 2008. 3. 30  최육사  우체국
SERIALIZE_DEFINE_PUT( KDBE_GET_POST_LETTER_LIST_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_POST_LETTER_LIST_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_POST_LETTER_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecPostItem )
		&& PUT( m_vecBlackList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_POST_LETTER_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecPostItem )
		&& GET( m_vecBlackList )
		;
}

//{{ 2008. 9. 18  최육사	편지 보내기
SERIALIZE_DEFINE_PUT( KDBE_PREPARE_INSERT_LETTER_TO_POST_REQ, obj, ks )
{
	return PUT( m_iFromUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_wstrToNickName )
		&& PUT( m_kSendLetter )
		&& PUT( m_iSendLetterCost )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& PUT( m_iIncrementED )
		&& PUT( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_PREPARE_INSERT_LETTER_TO_POST_REQ, obj, ks )
{
	return GET( m_iFromUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_wstrToNickName )
		&& GET( m_kSendLetter )
		&& GET( m_iSendLetterCost )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& GET( m_iIncrementED )
		&& GET( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_PREPARE_INSERT_LETTER_TO_POST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_wstrToNickName )
		&& PUT( m_kSendLetter )
		&& PUT( m_iSendLetterCost )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& PUT( m_iIncrementED )
		&& PUT( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_PREPARE_INSERT_LETTER_TO_POST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_wstrToNickName )
		&& GET( m_kSendLetter )
		&& GET( m_iSendLetterCost )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& GET( m_iIncrementED )
		&& GET( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_LETTER_TO_POST_REQ, obj, ks )
{
	return PUT( m_iFromUnitUID )
		&& PUT( m_wstrToNickName )
		&& PUT( m_kSendLetter )		
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_iSendLetterCost )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_LETTER_TO_POST_REQ, obj, ks )
{
	return GET( m_iFromUnitUID )
		&& GET( m_wstrToNickName )
		&& GET( m_kSendLetter )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_iSendLetterCost )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_LETTER_TO_POST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrToNickName )		
		&& PUT( m_kSendLetter )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_iSendLetterCost )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_LETTER_TO_POST_ACK, obj, ks )
{
	return GET( m_iOK )		
		&& GET( m_wstrToNickName )		
		&& GET( m_kSendLetter )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_iSendLetterCost )
		;
}

SERIALIZE_DEFINE_PUT( KELG_FIND_USER_SEND_LETTER_NOT, obj, ks )
{
	return PUT( m_kLetter )
		;
}

SERIALIZE_DEFINE_GET( KELG_FIND_USER_SEND_LETTER_NOT, obj, ks )
{
	return GET( m_kLetter )
		;
}
//}}

//{{ 2008. 9. 18  최육사	우편함
SERIALIZE_DEFINE_PUT( KDBE_READ_LETTER_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iPostNo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_READ_LETTER_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iPostNo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_FROM_LETTER_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iPostNo )
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& PUT( m_cLetterType )
#else
		&& PUT( m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		&& PUT( m_mapInsertedItem )		
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_iED )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& PUT( m_iIncrementED )
		&& PUT( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_FROM_LETTER_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iPostNo )
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& GET( m_cLetterType )
#else
		&& GET( m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		&& GET( m_mapInsertedItem )		
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_iED )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& GET( m_iIncrementED )
		&& GET( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_FROM_LETTER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPostNo )
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& PUT( m_cLetterType )
		&& PUT( m_iWeddingUID )
		&& PUT( m_iWeddingItemUID )
#else
		&& PUT( m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		&& PUT( m_mapInsertedItem )		
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_iED )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& PUT( m_iIncrementED )
		&& PUT( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_FROM_LETTER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPostNo )
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& GET( m_cLetterType )
		&& GET( m_iWeddingUID )
		&& GET( m_iWeddingItemUID )
#else
		&& GET( m_bSystemLetter )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		&& GET( m_mapInsertedItem )		
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_iED )
		//{{ 2012. 08. 23	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& GET( m_iIncrementED )
		&& GET( m_kItemQuantityUpdate )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DELETE_LETTER_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecPostNo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_LETTER_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecPostNo )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_POST_ITEM_TO_INVENTORY_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_vecPostItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_POST_ITEM_TO_INVENTORY_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_vecPostItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_POST_ITEM_TO_INVENTORY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_vecPostItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_POST_ITEM_TO_INVENTORY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_vecPostItemInfo )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KERM_SET_QUEST_ITEM_INFO_NOT, obj, ks )
{
	return PUT( m_mapDropQuestItembyIngQuest )
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& PUT( m_setQuestInfo )
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
		&& PUT( m_setGoingQuestInfo )
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
		;
}

SERIALIZE_DEFINE_GET( KERM_SET_QUEST_ITEM_INFO_NOT, obj, ks )
{
	return GET( m_mapDropQuestItembyIngQuest )
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& GET( m_setQuestInfo )
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
	//}}
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
		&& GET( m_setGoingQuestInfo )
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DEL_TUTORIAL_REQ, obj, ks )
{
	return PUT( m_iTeacherUID )
		&& PUT( m_iStudentUID )
		&& PUT( m_cReason )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DEL_TUTORIAL_REQ, obj, ks )
{
	return GET( m_iTeacherUID )
		&& GET( m_iStudentUID )
		&& GET( m_cReason )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DEL_TUTORIAL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iTeacherUID )
		&& PUT( m_iStudentUID )
		&& PUT( m_cReason )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DEL_TUTORIAL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iTeacherUID )
		&& GET( m_iStudentUID )
		&& GET( m_cReason )
		;
}

SERIALIZE_DEFINE_PUT( KELG_DEL_TUTORIAL_NOT, obj, ks )
{
	return PUT( m_iReceiverUnitUID )
		&& PUT( m_iDeleteUnitUID )
		//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		&& PUT( m_iReason )
#else SERV_NETERROR_STR_GET_FROM_CLIENT
		&& PUT( m_wstrReason )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KELG_DEL_TUTORIAL_NOT, obj, ks )
{
	return GET( m_iReceiverUnitUID )
		&& GET( m_iDeleteUnitUID )
		//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		&& GET( m_iReason )
#else SERV_NETERROR_STR_GET_FROM_CLIENT
		&& GET( m_wstrReason )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KELG_REQUEST_TUTORIAL_NOT, obj, ks )
{
	return PUT( m_iReceiverUnitUID )
		&& PUT( m_kTutorialUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_REQUEST_TUTORIAL_NOT, obj, ks )
{
	return GET( m_iReceiverUnitUID )
		&& GET( m_kTutorialUnitInfo )
		;
}

//SERIALIZE_DEFINE_PUT( KELG_REQUEST_TUTORIAL_REPLY_NOT, obj, ks )
//{
//	return PUT( m_iReceiverUnitUID )
//		&& PUT( m_kTutorialUnitInfo )
//		&& PUT( m_bAccept )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KELG_REQUEST_TUTORIAL_REPLY_NOT, obj, ks )
//{
//	return GET( m_iReceiverUnitUID )
//		&& GET( m_kTutorialUnitInfo )
//		&& GET( m_bAccept )
//		;
//}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_TUTORIAL_REQ, obj, ks )
{
	return PUT( m_iTeacherUID )
		&& PUT( m_kStudentUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TUTORIAL_REQ, obj, ks )
{
	return GET( m_iTeacherUID )
		&& GET( m_kStudentUnitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_TUTORIAL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iTeacherUID )
		&& PUT( m_kStudentUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TUTORIAL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iTeacherUID )
		&& GET( m_kStudentUnitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_GET_PURCHASED_CASH_ITEM_NOT, obj, ks )
{
    return PUT( m_iUnitUID )
		&& PUT( m_iItemID )
		&& PUT( m_sPeriod )
		&& PUT( m_iQuantity )
		&& PUT( m_ucLevel )
		&& PUT( m_cUnitClass )
		;
}

SERIALIZE_DEFINE_GET( KELOG_GET_PURCHASED_CASH_ITEM_NOT, obj, ks )
{
    return GET( m_iUnitUID )
		&& GET( m_iItemID )
		&& GET( m_sPeriod )
		&& GET( m_iQuantity )
		&& GET( m_ucLevel )
		&& GET( m_cUnitClass )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXPAND_INVENTORY_SLOT_REQ, obj, ks )
{
    return PUT( m_iUnitUID )
		&& PUT( m_usEventID )
		&& PUT( m_mapExpandedSlot )
        && PUT( m_iED )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_INVENTORY_SLOT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_usEventID )
		&& GET( m_mapExpandedSlot )
        && GET( m_iED )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXPAND_INVENTORY_SLOT_ACK, obj, ks )
{
    return PUT( m_mapExpandedSlot )
		&& PUT( m_usEventID )
        && PUT( m_iED )
        && PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_INVENTORY_SLOT_ACK, obj, ks )
{
    return GET( m_mapExpandedSlot )
		&& GET( m_usEventID )
        && GET( m_iED )
        && GET( m_iOK )
		;
}

//{{ 2008. 4. 21  최육사  스킬슬롯B
SERIALIZE_DEFINE_PUT( KDBE_EXPAND_SKILL_SLOT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iPeriodExpire )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_usTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM

		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
		&& PUT( m_usEventID )
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_SKILL_SLOT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iPeriodExpire )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_usTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM

		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
		&& GET( m_usEventID )
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXPAND_SKILL_SLOT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPeriodExpire)
		&& PUT( m_wstrSkillSlotBEndDate )
		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
		&& PUT( m_usEventID )
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_SKILL_SLOT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPeriodExpire )
		&& GET( m_wstrSkillSlotBEndDate )
		//{{ 2011. 11. 30	최육사	패키지 상품 추가
#ifdef SERV_ADD_PACKAGE_PRODUCT
		&& GET( m_usEventID )
#endif SERV_ADD_PACKAGE_PRODUCT
		//}}
		;
}
//}}



//{{ 2008. 12. 14  최육사	캐릭터 슬롯 확장
SERIALIZE_DEFINE_PUT( KDBE_EXPAND_CHAR_SLOT_REQ, obj, ks )
{
	return PUT( m_iExpandSlotSize )
		&& PUT( m_iCharSlotMax )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_CHAR_SLOT_REQ, obj, ks )
{
	return GET( m_iExpandSlotSize )
		&& GET( m_iCharSlotMax )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXPAND_CHAR_SLOT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCharSlotSize )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_CHAR_SLOT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCharSlotSize )
		;
}
//}}

//{{ 2008. 5. 5  최육사  장바구니 얻어오기
SERIALIZE_DEFINE_PUT( KDBE_GET_WISH_LIST_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_WISH_LIST_REQ, obj, ks )
{
	return GET( m_iUserUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_WISH_LIST_ACK, obj, ks )
{
	return PUT( m_mapWishList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_WISH_LIST_ACK, obj, ks )
{
	return GET( m_mapWishList )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_IS_LOGIN_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_bIsLogin )
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& PUT( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_IS_LOGIN_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_bIsLogin )
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& GET( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
        ;
}

//{{ 2008. 5. 19  최육사  
SERIALIZE_DEFINE_PUT( KENX_UPDATE_PRODUCT_LIST_NOT, obj, ks )
{
	return PUT( m_vecProductItemID )
		&& PUT( m_vecCashItemRessolveInfo )
		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
		&& PUT( m_vecCashProductList )
#endif SERV_CASH_ITEM_LIST
		//}}
		&& PUT( m_ulProductNoResStone )		
		;
}

SERIALIZE_DEFINE_GET( KENX_UPDATE_PRODUCT_LIST_NOT, obj, ks )
{
	return GET( m_vecProductItemID )
		&& GET( m_vecCashItemRessolveInfo )
		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
		&& GET( m_vecCashProductList )
#endif SERV_CASH_ITEM_LIST
		//}}
		&& GET( m_ulProductNoResStone )		
		;
}
//}}

//{{ 2008. 5. 19  최육사  체험ID 이벤트
SERIALIZE_DEFINE_PUT( KDBE_UPDATE_EVENT_TIME_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& PUT( m_vecCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT	
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_EVENT_TIME_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& GET( m_vecCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT	
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_EVENT_TIME_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_setConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& PUT( m_setCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_EVENT_TIME_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_setConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& GET( m_setCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT
		//}}
		;
}
//}}

//{{ 2008. 5. 21  최육사  아이템 어뷰저 로그
SERIALIZE_DEFINE_PUT( KELOG_ITEM_ABUSER_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecItemAbuserLog )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ITEM_ABUSER_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecItemAbuserLog )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_ENCHANT_ABUSER_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_cEnchantLevel )
		&& PUT( m_iEnchantResult )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ENCHANT_ABUSER_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_cEnchantLevel )
		&& GET( m_iEnchantResult )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_ED_ABUSER_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iState )
		&& PUT( m_iBeforeED )
		&& PUT( m_iAfterED )
		&& PUT( m_iED )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ED_ABUSER_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iState )
		&& GET( m_iBeforeED )
		&& GET( m_iAfterED )
		&& GET( m_iED )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT, obj, ks )
{
	return PUT( m_vecStudentUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_TUTORIAL_DUNGEON_ROOM_UPDATE_NOT, obj, ks )
{
	return GET( m_vecStudentUnitUID )
		;
}

//{{ 2008. 5. 28  최육사  인벤 아이템 슬롯 정보 업데이트
SERIALIZE_DEFINE_PUT( KDBE_UPDATE_INVENTORY_ITEM_POS_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kItemPositionUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_INVENTORY_ITEM_POS_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kItemPositionUpdate )
		;
}
//}}

//{{ 2008. 6. 20  최육사  선물하기
SERIALIZE_DEFINE_PUT( KELG_PRESENT_CASH_ITEM_REQ, obj, ks )
{
	return PUT( m_uiPwdHash )
		&& PUT( m_wstrPassword )
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_wstrMessage )
		&& PUT( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& PUT( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KELG_PRESENT_CASH_ITEM_REQ, obj, ks )
{
	return GET( m_uiPwdHash )
		&& GET( m_wstrPassword )
		&& GET( m_wstrReceiverNickName )
		&& GET( m_wstrMessage )
		&& GET( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& GET( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KELG_PRESENT_CASH_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_wstrMessage )
		&& PUT( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& PUT( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KELG_PRESENT_CASH_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrReceiverNickName )
		&& GET( m_wstrMessage )
		&& GET( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& GET( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, obj, ks )
{
	return PUT( m_wstrReceiverNickName )
		&& PUT( m_wstrMessage )
		&& PUT( m_vecPurchaseReqInfo )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_iUseCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& PUT( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_REQ, obj, ks )
{
	return GET( m_wstrReceiverNickName )
		&& GET( m_wstrMessage )
		&& GET( m_vecPurchaseReqInfo )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_iUseCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& GET( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrMessage )
		&& PUT( m_vecPurchaseReqInfo )
#ifdef SERV_GLOBAL_BILLING
		&& PUT( m_iReceiverUserUID )
		&& PUT( m_iReceiverUnitUID )
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_cReceiverLevel )
		&& PUT( m_iReceiverUnitClass )
#endif // SERV_GLOBAL_BILLING
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_iUseCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_GET( KDBE_PRESENT_CASH_ITEM_CHECK_NICKNAME_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrUserID )
		&& GET( m_wstrMessage )
		&& GET( m_vecPurchaseReqInfo )
#ifdef SERV_GLOBAL_BILLING
		&& GET( m_iReceiverUserUID )
		&& GET( m_iReceiverUnitUID )
		&& GET( m_wstrReceiverNickName )
		&& GET( m_cReceiverLevel )
		&& GET( m_iReceiverUnitClass )
#endif // SERV_GLOBAL_BILLING
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_iUseCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}
//}}

//{{ 2009. 7. 6  최육사		랭킹 개편
SERIALIZE_DEFINE_PUT( KDBE_DELETE_RANKING_INFO_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_RANKING_INFO_NOT, obj, ks )
{
	return GET( m_iRankingType )
		;
}

SERIALIZE_DEFINE_PUT( KELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ, obj, ks )
{
	return PUT( m_iServerUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ, obj, ks )
{
	return GET( m_iServerUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK, obj, ks )
{
	return PUT( m_mapHenirRanking )
		&& PUT( m_mapLastRank )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK, obj, ks )
{
	return GET( m_mapHenirRanking )
		&& GET( m_mapLastRank )
		;
}

SERIALIZE_DEFINE_PUT( KELG_HENIR_RANKING_REFRESH_NOT, obj, ks )
{
	return PUT( m_mapHenirRanking )
		;
}

SERIALIZE_DEFINE_GET( KELG_HENIR_RANKING_REFRESH_NOT, obj, ks )
{
	return GET( m_mapHenirRanking )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_WEB_RANKING_INFO_ACK, obj, ks )
{
	return PUT( m_vecDungeonRanking )
		&& PUT( m_vecPvpRanking )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_WEB_RANKING_INFO_ACK, obj, ks )
{
	return GET( m_vecDungeonRanking )
		&& GET( m_vecPvpRanking )
		;
}

//{{ 2009. 11. 9  최육사	로그인서버GameDB
SERIALIZE_DEFINE_PUT( KELG_GET_WEB_RANKING_REFRESH_NOT, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_cRankingRefreshType )
		&& PUT( m_uiLastIndex )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_WEB_RANKING_REFRESH_NOT, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_cRankingRefreshType )
		&& GET( m_uiLastIndex )
		;
}

SERIALIZE_DEFINE_PUT( KELG_WEB_RANKING_REFRESH_NOT, obj, ks )
{
	return PUT( m_bInit )
		&& PUT( m_cRankingRefreshType )
		&& PUT( m_uiTotalSize )
		&& PUT( m_vecDungeonRanking )
		&& PUT( m_mapDungeonRanking )
		&& PUT( m_vecPvpRanking )
		&& PUT( m_mapPvpRanking )
		;
}

SERIALIZE_DEFINE_GET( KELG_WEB_RANKING_REFRESH_NOT, obj, ks )
{
	return GET( m_bInit )
		&& GET( m_cRankingRefreshType )
		&& GET( m_uiTotalSize )
		&& GET( m_vecDungeonRanking )
		&& GET( m_mapDungeonRanking )
		&& GET( m_vecPvpRanking )
		&& GET( m_mapPvpRanking )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KELG_NEW_RECORD_HENIR_RANKING_NOT, obj, ks )
{
	return PUT( m_kNewRecordRanking )
		;
}

SERIALIZE_DEFINE_GET( KELG_NEW_RECORD_HENIR_RANKING_NOT, obj, ks )
{
	return GET( m_kNewRecordRanking )
		;
}
//}}

//{{ 2009. 7. 31  최육사	랭킹 추가 기능
SERIALIZE_DEFINE_PUT( KELG_ADMIN_NEW_RECORD_TEST_NOT, obj, ks )
{
	return PUT( m_vecNewRecordTest )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_NEW_RECORD_TEST_NOT, obj, ks )
{
	return GET( m_vecNewRecordTest )
		;
}

SERIALIZE_DEFINE_PUT( KELG_ADMIN_NEW_RECORD_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_kNewRecordRanking )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_NEW_RECORD_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_kNewRecordRanking )
		;
}

SERIALIZE_DEFINE_PUT( KELG_ADMIN_DELETE_RECORD_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_DELETE_RECORD_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_wstrNickName )
		;
}
//}}

//{{ 2008. 9. 3  최육사		속성강화
SERIALIZE_DEFINE_PUT( KDBE_ATTRIB_ENCHANT_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_cAttribEnchantSlotNo )
		&& PUT( m_cAttribEnchantID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )		
		;
}

SERIALIZE_DEFINE_GET( KDBE_ATTRIB_ENCHANT_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_cAttribEnchantSlotNo )
		&& GET( m_cAttribEnchantID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )		
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ATTRIB_ENCHANT_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_cAttribEnchantSlotNo )
		&& PUT( m_cAttribEnchantID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )		
		;
}

SERIALIZE_DEFINE_GET( KDBE_ATTRIB_ENCHANT_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_cAttribEnchantSlotNo )
		&& GET( m_cAttribEnchantID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
//}}

//{{ 2008. 9. 26  최육사	우체국 블랙리스트
SERIALIZE_DEFINE_PUT( KDBE_NEW_POST_BLACK_LIST_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KDBE_NEW_POST_BLACK_LIST_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_NEW_POST_BLACK_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KDBE_NEW_POST_BLACK_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrNickName )
		;
}
//}}

//{{ 2008. 10. 7  최육사	타이틀
SERIALIZE_DEFINE_PUT( KDBE_INSERT_TITLE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iTitleID )
		&& PUT( m_sPeriod )
		&& PUT( m_bGameServerEvent )
		//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
		&& PUT( m_bExpandPeriod )
#endif SERV_TITLE_ITEM_NEW
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TITLE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iTitleID )
		&& GET( m_sPeriod )
		&& GET( m_bGameServerEvent )
		//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
		&& GET( m_bExpandPeriod )
#endif SERV_TITLE_ITEM_NEW
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_TITLE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID ) 
		&& PUT( m_iTitleID )
		&& PUT( m_wstrEndDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TITLE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID ) 
		&& GET( m_iTitleID )
		&& GET( m_wstrEndDate )
		;
}

SERIALIZE_DEFINE_PUT( KELG_RANKING_TITLE_REWARD_NOT, obj, ks )
{
	return PUT( m_mapTitleReward )
		;
}

SERIALIZE_DEFINE_GET( KELG_RANKING_TITLE_REWARD_NOT, obj, ks )
{
	return GET( m_mapTitleReward )
		;
}
//}}

//{{ 2008. 10. 22  최육사	캐쉬 분해 로그
SERIALIZE_DEFINE_PUT( KELOG_CASH_ITEM_RESOLVE_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iResultQuantity )
		//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
		&& PUT( m_iAfterItemID )
		&& PUT( m_iAfterItemQuantity )
		&& PUT( m_iLevel )
#endif SERV_MULTI_RESOLVE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KELOG_CASH_ITEM_RESOLVE_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iResultQuantity )
		//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
		&& GET( m_iAfterItemID )
		&& GET( m_iAfterItemQuantity )
		&& GET( m_iLevel )
#endif SERV_MULTI_RESOLVE
		//}}
		;
}
//}}

//{{ 2008. 11. 10  최육사	아이템 강화시 파괴로그
SERIALIZE_DEFINE_PUT( KELOG_ENCHANT_ITEM_BREAK_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_cEnchantLevel )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ENCHANT_ITEM_BREAK_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_cEnchantLevel )
		;
}
//}}

//{{ 2008. 10. 30  최육사	어뷰저 리스트
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
SERIALIZE_DEFINE_PUT( KDBE_ABUSER_LIST_REQ, obj, ks )
{
	return PUT( m_iRtt )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ABUSER_LIST_REQ, obj, ks )
{
	return GET( m_iRtt )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ABUSER_LIST_ACK, obj, ks )
{
	return PUT( m_iRtt )
		&& PUT( m_vecAbuserList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ABUSER_LIST_ACK, obj, ks )
{
	return GET( m_iRtt )
		&& GET( m_vecAbuserList )
		;
}
#else
SERIALIZE_DEFINE_PUT( KDBE_ABUSER_LIST_REQ, obj, ks )
{
	return PUT( m_mapReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ABUSER_LIST_REQ, obj, ks )
{
	return GET( m_mapReleaseTick )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ABUSER_LIST_ACK, obj, ks )
{
	return PUT( m_mapAbuserList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ABUSER_LIST_ACK, obj, ks )
{
	return GET( m_mapAbuserList )
		;
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//}}

//{{ 2008. 11. 10  최육사	sms
SERIALIZE_DEFINE_PUT( KDBE_SEND_PHONE_MSG_NOT, obj, ks )
{
	return PUT( m_vecPhoneNum )
		&& PUT( m_wstrSMSMessage )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SEND_PHONE_MSG_NOT, obj, ks )
{
	return GET( m_vecPhoneNum )
		&& GET( m_wstrSMSMessage )
		;
}
//}}

//{{ 2008. 12. 21  최육사	아이템 복구
SERIALIZE_DEFINE_PUT( KDBE_RESTORE_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iLevelAfterEnchant )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESTORE_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iLevelAfterEnchant )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RESTORE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_iLevelAfterEnchant )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESTORE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_iLevelAfterEnchant )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_RESTORE_ITEM_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iBeforeEnchantLv )
		&& PUT( m_iAfterEnchantLv )
		;
}

SERIALIZE_DEFINE_GET( KELOG_RESTORE_ITEM_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iBeforeEnchantLv )
		&& GET( m_iAfterEnchantLv )
		;
}
//}}

//{{ 2008. 12. 25  최육사	부여
SERIALIZE_DEFINE_PUT( KDBE_ENCHANT_ATTACH_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iLevelAfterEnchant )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ENCHANT_ATTACH_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iLevelAfterEnchant )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ENCHANT_ATTACH_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_iLevelAfterEnchant )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ENCHANT_ATTACH_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_iLevelAfterEnchant )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_ATTACH_ITEM_ENCHANT_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iAttachItemID )
		&& PUT( m_iDestItemUID )
		&& PUT( m_iBeforeEnchantLv )
		&& PUT( m_iAfterEnchantLv )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ATTACH_ITEM_ENCHANT_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iAttachItemID )
		&& GET( m_iDestItemUID )
		&& GET( m_iBeforeEnchantLv )
		&& GET( m_iAfterEnchantLv )
		;
}
//}}

//{{ 2008. 11. 18  최육사	아이템 교환
SERIALIZE_DEFINE_PUT( KDBE_ITEM_EXCHANGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )		
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )	
		//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
		&& PUT( m_mapResultItem )	
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
		//}}
		//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		&& PUT( m_vecDestItem )	
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		//}}
		//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
		&& PUT( m_iSourceItemID )
		&& PUT( m_iSourceItemQuantity )
		&& PUT( m_cExchangeType )
#endif SERV_EXCHANGE_LOG
		//}
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EXCHANGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )		
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )	
		//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
		&& GET( m_mapResultItem )
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
		//}}
		//{{ 2012. 08. 14	박세훈	대천사의 주화 교환 로그
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		&& GET( m_vecDestItem )
#endif SERV_ARCHUANGEL_S_COIN_EVENT_LOG
		//}}
		//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
		&& GET( m_iSourceItemID )
		&& GET( m_iSourceItemQuantity )
		&& GET( m_cExchangeType )
#endif SERV_EXCHANGE_LOG
		//}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ITEM_EXCHANGE_ACK, obj, ks )
{
	return PUT( m_iOK )		
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )		
		//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
		&& PUT( m_mapResultItem )
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
		//}
		//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
		&& PUT( m_iSourceItemID )
		&& PUT( m_iSourceItemQuantity )	
		&& PUT( m_cExchangeType )
#endif SERV_EXCHANGE_LOG
		//}
#ifdef SERV_GROW_UP_SOCKET
		&& PUT( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_2013_JUNGCHU_TITLE
		&& PUT( m_b12TimesRewarded )
#endif SERV_2013_JUNGCHU_TITLE
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EXCHANGE_ACK, obj, ks )
{
	return GET( m_iOK )		
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
		&& GET( m_mapResultItem )
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
		//}
		//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
		&& GET( m_iSourceItemID )
		&& GET( m_iSourceItemQuantity )
		&& GET( m_cExchangeType )
#endif SERV_EXCHANGE_LOG
		//}
#ifdef SERV_GROW_UP_SOCKET
		&& GET( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_2013_JUNGCHU_TITLE
		&& GET( m_b12TimesRewarded )
#endif SERV_2013_JUNGCHU_TITLE
		;
}

SERIALIZE_DEFINE_PUT( KELOG_ITEM_EXCHANGE_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iDeletedItemUID )
		&& PUT( m_iDeletedItemID )
		&& PUT( m_iChangedItemUID )
		&& PUT( m_iChangedItemID )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ITEM_EXCHANGE_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iDeletedItemUID )
		&& GET( m_iDeletedItemID )
		&& GET( m_iChangedItemUID )
		&& GET( m_iChangedItemID )
		;
}
//}}

//{{ 2008. 11. 19  최육사	어뷰저 자동 등록
SERIALIZE_DEFINE_PUT( KDBE_REG_ABUSER_AUTOMATIC_NOT, obj, ks )
{
	return PUT( m_mapRegAbuserInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_REG_ABUSER_AUTOMATIC_NOT, obj, ks )
{
	return GET( m_mapRegAbuserInfo )
		;
}
//}}

//{{ 2008. 12. 30  최육사	매력아이템 로그
SERIALIZE_DEFINE_PUT( KELOG_CHARM_ITEM_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iCharmItemID )
		&& PUT( m_iResultItemID )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KELOG_CHARM_ITEM_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iCharmItemID )
		&& GET( m_iResultItemID )
		&& GET( m_iQuantity )
		;
}
//}}

//{{ 2008. 11. 4  최육사	광장 개편
SERIALIZE_DEFINE_PUT( KERM_UPDATE_FIELD_INFO_NOT, obj, ks )
{
	return PUT( m_iMapID )
		&& PUT( m_iFieldUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_FIELD_INFO_NOT, obj, ks )
{
	return GET( m_iMapID )
		&& GET( m_iFieldUID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_JOIN_FIELD_REQ, obj, ks )
{
	return PUT( m_iFieldUID )
		&& PUT( m_kFieldUserInfo )
		//{{ 2008. 12. 9  최육사	친구, 파티 리스트
		&& PUT( m_setPartyUIDList )
		&& PUT( m_setFriendUIDList )
		&& PUT( m_kFieldOption )
        //}}
		;
}

SERIALIZE_DEFINE_GET( KERM_JOIN_FIELD_REQ, obj, ks )
{
	return GET( m_iFieldUID )
		&& GET( m_kFieldUserInfo )
		//{{ 2008. 12. 9  최육사	친구, 파티 리스트
		&& GET( m_setPartyUIDList )
		&& GET( m_setFriendUIDList )
		&& GET( m_kFieldOption )
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KERM_JOIN_FIELD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iFieldUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_JOIN_FIELD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iFieldUID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_LEAVE_FIELD_REQ, obj, ks )
{
	return PUT( m_iReason )
		&& PUT( m_iMapID )
		;
}

SERIALIZE_DEFINE_GET( KERM_LEAVE_FIELD_REQ, obj, ks )
{
	return GET( m_iReason )
		&& GET( m_iMapID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_LEAVE_FIELD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iReason )
		&& PUT( m_iMapID )
		;
}

SERIALIZE_DEFINE_GET( KERM_LEAVE_FIELD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iReason )
		&& GET( m_iMapID )
		;
}

SERIALIZE_DEFINE_PUT( KETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_UnitUID )
		&& PUT( m_vPos )
		&& PUT( m_vecFieldFrame )
		&& PUT( m_bIsRight )		
		;
}

SERIALIZE_DEFINE_GET( KETR_FIELD_UNIT_SYNC_DATA_BROAD_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_UnitUID )
		&& GET( m_vPos )
		&& GET( m_vecFieldFrame )
		&& GET( m_bIsRight )		
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KETR_CHANGE_USER_LIST_IN_SECTOR_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_vecNewUserList )
        && PUT( m_vecDelUserList )
		;
}

SERIALIZE_DEFINE_GET( KETR_CHANGE_USER_LIST_IN_SECTOR_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_vecNewUserList )
        && GET( m_vecDelUserList )
		;
}
//////////////////////////////////////////////////////////////////////////

//{{ 2008. 12. 9  최육사	친구 리스트
SERIALIZE_DEFINE_PUT( KERM_OPTION_UPDATE_NOT, obj, ks )
{
	return PUT( m_kFieldOption )
		;
}

SERIALIZE_DEFINE_GET( KERM_OPTION_UPDATE_NOT, obj, ks )
{
	return GET( m_kFieldOption )
		;
}

SERIALIZE_DEFINE_PUT( KERM_UPDATE_PARTY_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_setPartyUIDList )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_PARTY_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_setPartyUIDList )
		;
}

SERIALIZE_DEFINE_PUT( KETR_UPDATE_PARTY_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iPartyUID )
		;	
}

SERIALIZE_DEFINE_GET( KETR_UPDATE_PARTY_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_iPartyUID )
		;	
}
//}}

//{{ 2009. 1. 16  최육사	채널리스트
SERIALIZE_DEFINE_PUT( KDBE_CHANNEL_LIST_REQ, obj, ks )
{
	return PUT( m_iServerGroupID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANNEL_LIST_REQ, obj, ks )
{
	return GET( m_iServerGroupID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANNEL_LIST_ACK, obj, ks )
{
	return PUT( m_mapChannelList )
		
		//{{ 2012. 06. 29	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
		 && PUT( m_mapChannelBonusList )
#endif SERV_CHANNEL_LIST_RENEWAL
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANNEL_LIST_ACK, obj, ks )
{
	return GET( m_mapChannelList )

		//{{ 2012. 06. 29	박세훈	채널 UI 유동적으로 변경 가능한 시스템 ( Merge )
#ifdef SERV_CHANNEL_LIST_RENEWAL
		&& GET( m_mapChannelBonusList )
#endif SERV_CHANNEL_LIST_RENEWAL
		//}}
		;
}
//}}

//{{ 2009. 2. 13  최육사	채널 이동
//SERIALIZE_DEFINE_PUT( KELG_UPDATE_USER_MIGRATION_INFO_REQ, obj, ks )
//{
//	return PUT( m_iServerUID )
//		&& PUT( m_iAccountAuthKey )
//		&& PUT( m_kUserInfo )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KELG_UPDATE_USER_MIGRATION_INFO_REQ, obj, ks )
//{
//	return GET( m_iServerUID )
//		&& GET( m_iAccountAuthKey )
//		&& GET( m_kUserInfo )
//		;
//}
//
//SERIALIZE_DEFINE_PUT( KELG_UPDATE_USER_MIGRATION_INFO_ACK, obj, ks )
//{
//	return PUT( m_iOK )
//		&& PUT( m_iServerUID )
//		&& PUT( m_iAccountAuthKey )
//		&& PUT( m_iUnitUID )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KELG_UPDATE_USER_MIGRATION_INFO_ACK, obj, ks )
//{
//	return GET( m_iOK )
//		&& GET( m_iServerUID )
//		&& GET( m_iAccountAuthKey )
//		&& GET( m_iUnitUID )
//		;
//}
//
//SERIALIZE_DEFINE_PUT( KELG_GET_USER_MIGRATION_INFO_REQ, obj, ks )
//{
//	return PUT( m_iServerUID )
//		&& PUT( m_iAccountAuthKey )
//		&& PUT( m_iUnitUID )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KELG_GET_USER_MIGRATION_INFO_REQ, obj, ks )
//{
//	return GET( m_iServerUID )
//		&& GET( m_iAccountAuthKey )
//		&& GET( m_iUnitUID )
//		;
//}
//
//SERIALIZE_DEFINE_PUT( KELG_GET_USER_MIGRATION_INFO_ACK, obj, ks )
//{
//	return PUT( m_iOK )
//		&& PUT( m_kUserInfo )
//		;
//}
//
//SERIALIZE_DEFINE_GET( KELG_GET_USER_MIGRATION_INFO_ACK, obj, ks )
//{
//	return GET( m_iOK )
//		&& GET( m_kUserInfo )
//		;
//}
//}}

//{{ 2009. 4. 1  최육사		친구차단옵션
SERIALIZE_DEFINE_PUT( KELG_UPDATE_DENY_FRIEND_SHIP_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_bDenyFriendShip )
		;
}

SERIALIZE_DEFINE_GET( KELG_UPDATE_DENY_FRIEND_SHIP_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_bDenyFriendShip )
		;
}
//}}

//{{ 2009. 4. 7  최육사		닉네임 변경
SERIALIZE_DEFINE_PUT( KDBE_DELETE_NICK_NAME_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_NICK_NAME_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		;
}
//}}

//{{ 2009. 4. 14  최육사	친구리스트
SERIALIZE_DEFINE_PUT( KERM_UPDATE_FRIEND_NOT, obj, ks )
{
	return PUT( m_setFriendUIDList )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_FRIEND_NOT, obj, ks )
{
	return GET( m_setFriendUIDList )
		;
}
//}}

//{{ 2009. 4. 17  최육사	유저찾기
SERIALIZE_DEFINE_PUT( KELG_SEARCH_UNIT_REQ, obj, ks )
{
	return PUT( m_usEventID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_SEARCH_UNIT_REQ, obj, ks )
{
	return GET( m_usEventID )
		&& GET( m_wstrNickName )
		&& GET( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_SEARCH_UNIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_usEventID )
		&& PUT( m_kCUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_SEARCH_UNIT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_usEventID )
		&& GET( m_kCUnitInfo )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KERM_OPEN_TC_ROOM_REQ, obj, ks )
{
	return  PUT( m_kTCInfo ) &&
		PUT( m_kRoomUserInfo );
}

SERIALIZE_DEFINE_GET( KERM_OPEN_TC_ROOM_REQ, obj, ks )
{
	return  GET( m_kTCInfo ) &&
		GET( m_kRoomUserInfo );
}


SERIALIZE_DEFINE_PUT( KERM_JOIN_ROOM_REQ, obj, ks )
{
	return PUT( m_bQuickJoin )
		&& PUT( m_wstrPassword ) 
		&& PUT( m_kRoomUserInfo )
		&& PUT( m_vecStudentUnitUID )		
#ifdef SERV_NEW_PVPROOM_PROCESS
		&& PUT( m_bInvitation )
#endif SERV_NEW_PVPROOM_PROCESS
		;
}

SERIALIZE_DEFINE_GET( KERM_JOIN_ROOM_REQ, obj, ks )
{
	return GET( m_bQuickJoin )
		&& GET( m_wstrPassword )
		&& GET( m_kRoomUserInfo )
		&& GET( m_vecStudentUnitUID )		
#ifdef SERV_NEW_PVPROOM_PROCESS
		&& GET( m_bInvitation )
#endif SERV_NEW_PVPROOM_PROCESS
		;
}
//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
SERIALIZE_DEFINE_PUT( KERM_LEAVE_ROOM_REQ, obj, ks )
{
	return PUT( m_iReason )
		&& PUT( m_iItemID )
		//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_bSendAck )
		&& PUT( m_kBattleFieldJoinInfo )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& PUT( m_kResultInfo )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		&& PUT( m_kGamePlayNetworkInfo )
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_LEAVE_ROOM_REQ, obj, ks )
{
	return GET( m_iReason )
		&& GET( m_iItemID )
		//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_bSendAck )
		&& GET( m_kBattleFieldJoinInfo )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& GET( m_kResultInfo )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		&& GET( m_kGamePlayNetworkInfo )
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
		//}}
		;
}
#else
SERIALIZE_DEFINE_PUT( KERM_LEAVE_ROOM_REQ, obj, ks )
{
	return PUT( m_iReason )
		;
}

SERIALIZE_DEFINE_GET( KERM_LEAVE_ROOM_REQ, obj, ks )
{
	return GET( m_iReason )
		;
}
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
//}}

SERIALIZE_DEFINE_PUT( KERM_ROOM_LIST_INFO_NOT, obj, ks )
{
	return PUT( m_iCode )
		&& PUT( m_kRoomInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_ROOM_LIST_INFO_NOT, obj, ks )
{
	return GET( m_iCode )
		&& GET( m_kRoomInfo )
		;
}

SERIALIZE_DEFINE_PUT( KELG_INVITE_PVP_ROOM_REQ, obj, ks )
{
	return PUT( m_wstrFromNickName )
		&& PUT( m_iToUnitUID )
		&& PUT( m_kRoomInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_PVP_ROOM_REQ, obj, ks )
{
	return GET( m_wstrFromNickName )
		&& GET( m_iToUnitUID )
		&& GET( m_kRoomInfo )
		;
}

SERIALIZE_DEFINE_PUT( KELG_INVITE_PVP_ROOM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_PVP_ROOM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrNickName )
		;
}

//{{ 2009. 4. 22  최육사	대전유저리스트
SERIALIZE_DEFINE_PUT( KECN_NEW_PVP_USER_INFO_NOT, obj, ks )
{
	return PUT( m_nGSUID )
		&& PUT( m_kUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KECN_NEW_PVP_USER_INFO_NOT, obj, ks )
{
	return GET( m_nGSUID )
		&& GET( m_kUserInfo )
		;
}

SERIALIZE_DEFINE_PUT( KECN_UPDATE_PVP_USER_INFO_NOT, obj, ks )
{
	return PUT( m_vecUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KECN_UPDATE_PVP_USER_INFO_NOT, obj, ks )
{
	return GET( m_vecUserInfo )
		;
}
//}}

//{{ 2009. 5. 8  최육사		실시간 레벨업
SERIALIZE_DEFINE_PUT( KERM_CHAR_LEVEL_UP_NOT, obj, ks )
{
	return PUT( m_kBaseStat )
		&& PUT( m_kRoomUserInfo )
#ifdef SERV_ELESIS_UPDATE_EVENT
		&& PUT( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
		;
}

SERIALIZE_DEFINE_GET( KERM_CHAR_LEVEL_UP_NOT, obj, ks )
{
	return GET( m_kBaseStat )
		&& GET( m_kRoomUserInfo )
#ifdef SERV_ELESIS_UPDATE_EVENT
		&& GET( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INSERT_CASH_SKILL_POINT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iCSPoint )
		&& PUT( m_iPeriod )
		&& PUT( m_bUpdateEndDateOnly )
		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
		&& PUT( m_iSkillPointItemID )
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_CASH_SKILL_POINT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iCSPoint )
		&& GET( m_iPeriod )
		&& GET( m_bUpdateEndDateOnly )
		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
		&& GET( m_iSkillPointItemID )
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INSERT_CASH_SKILL_POINT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCSPoint )
		&& PUT( m_iPeriod )
		&& PUT( m_wstrEndDate )
		&& PUT( m_bUpdateEndDateOnly )
		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
		&& PUT( m_iSkillPointItemID )
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_CASH_SKILL_POINT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCSPoint )
		&& GET( m_iPeriod )
		&& GET( m_wstrEndDate )
		&& GET( m_bUpdateEndDateOnly )
		//{{ 2010. 12. 8	최육사	이벤트용 그노시스 축복
#ifdef SERV_EVENT_CASH_SKILL_POINT_ITEM
		&& GET( m_iSkillPointItemID )
#endif SERV_EVENT_CASH_SKILL_POINT_ITEM
		//}}
		;
}





#ifdef SERV_SKILL_PAGE_SYSTEM

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPIRE_CASH_SKILL_POINT_REQ, obj, ks )
{
	return  PUT( m_iUnitUID )
		&&	PUT( m_vecRetrievedSkillPageData )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPIRE_CASH_SKILL_POINT_REQ, obj, ks )
{
	return  GET( m_iUnitUID )
		&&	GET( m_vecRetrievedSkillPageData )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPIRE_CASH_SKILL_POINT_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_vecRetrievedSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPIRE_CASH_SKILL_POINT_ACK, obj, ks )
{
	return  GET( m_iOK )
		&&	GET( m_vecRetrievedSPoint )
		;
}

#else // SERV_SKILL_PAGE_SYSTEM

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPIRE_CASH_SKILL_POINT_REQ, obj, ks )
{
	return  PUT( m_iUnitUID )
		&&	PUT( m_iRetrievedSPoint )
		&&	PUT( m_vecUserSkillData )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPIRE_CASH_SKILL_POINT_REQ, obj, ks )
{
	return  GET( m_iUnitUID )
		&&	GET( m_iRetrievedSPoint )
		&&	GET( m_vecUserSkillData )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPIRE_CASH_SKILL_POINT_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_iRetrievedSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPIRE_CASH_SKILL_POINT_ACK, obj, ks )
{
	return  GET( m_iOK )
		&&	GET( m_iRetrievedSPoint )
		;
}

#endif // SERV_SKILL_PAGE_SYSTEM



//////////////////////////////////////////////////////////////////////////
//{{ 2009. 8. 4  최육사		봉인 스킬
SERIALIZE_DEFINE_PUT( KDBE_UNSEAL_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iSkillID )
		;	
}

SERIALIZE_DEFINE_GET( KDBE_UNSEAL_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iSkillID )
		;	
}

SERIALIZE_DEFINE_PUT( KDBE_UNSEAL_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iSkillID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UNSEAL_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iSkillID )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELOG_INIT_SKILL_TREE_NOT, obj, ks )
{
	return  PUT( m_iUnitUID )
		&&	PUT( m_iSPoint )
		&&	PUT( m_iCSPoint )
		&&	PUT( m_iItemUID )
		;
}

SERIALIZE_DEFINE_GET( KELOG_INIT_SKILL_TREE_NOT, obj, ks )
{
	return  GET( m_iUnitUID )
		&&	GET( m_iSPoint )
		&&	GET( m_iCSPoint )
		&&	GET( m_iItemUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELOG_INSERT_CASH_SKILL_POINT_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iUnitLevel )
		&& PUT( m_iMaxCSPoint )
		&& PUT( m_iPeriod )
		&& PUT( m_bUpdateEndDateOnly )
		;
}

SERIALIZE_DEFINE_GET( KELOG_INSERT_CASH_SKILL_POINT_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iUnitLevel )
		&& GET( m_iMaxCSPoint )
		&& GET( m_iPeriod )
		&& GET( m_bUpdateEndDateOnly )
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 14  최육사	내구도소모개편
SERIALIZE_DEFINE_PUT( KERM_DECREASE_ENDURANCE_DUNGEON_NOT, obj, ks )
{
	return PUT( m_cPartyNumMember )
		&& PUT( m_iEnduranceDamage )
		;
}

SERIALIZE_DEFINE_GET( KERM_DECREASE_ENDURANCE_DUNGEON_NOT, obj, ks )
{
	return GET( m_cPartyNumMember )
		&& GET( m_iEnduranceDamage )
		;
}

SERIALIZE_DEFINE_PUT( KERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, obj, ks )
{
	return PUT( m_UnitUID )
		&& PUT( m_vecInventorySlotInfo )
		&& PUT( m_kRoomUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_ZERO_ENDURANCE_ITEM_IN_ROOM_NOT, obj, ks )
{
	return GET( m_UnitUID )
		&& GET( m_vecInventorySlotInfo )
		&& GET( m_kRoomUserInfo )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 23  최육사	시간드롭
SERIALIZE_DEFINE_PUT( KDBE_GET_TIME_DROP_INFO_ACK, obj, ks )
{
	return PUT( m_vecTimeDropInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_TIME_DROP_INFO_ACK, obj, ks )
{
	return GET( m_vecTimeDropInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHECK_TIME_DROP_RESULT_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iTimeDropID )
		&& PUT( m_iRewardItemID )
		&& PUT( m_wstrRewardItemName )
		&& PUT( m_iDropItemUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_TIME_DROP_RESULT_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_iTimeDropID )
		&& GET( m_iRewardItemID )
		&& GET( m_wstrRewardItemName )
		&& GET( m_iDropItemUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHECK_TIME_DROP_RESULT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGSUID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iTimeDropID )
		&& PUT( m_iRewardItemID )
		&& PUT( m_wstrRewardItemName )
		&& PUT( m_iDropItemUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_TIME_DROP_RESULT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGSUID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_iTimeDropID )
		&& GET( m_iRewardItemID )
		&& GET( m_wstrRewardItemName )
		&& GET( m_iDropItemUID )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 7. 22  최육사	메가폰 로그
SERIALIZE_DEFINE_PUT( KELOG_MEGAPHONE_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemID )
		;
}

SERIALIZE_DEFINE_GET( KELOG_MEGAPHONE_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemID )
		;
}
//}}

//{{ 2009. 7. 10  최육사	사기 시스템
SERIALIZE_DEFINE_PUT( KERM_INCREASE_PARTY_FEVER_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_sSubStageNpcCount )
		&& PUT( m_cRoomType )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_INCREASE_PARTY_FEVER_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_sSubStageNpcCount )
		&& GET( m_cRoomType )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}
//}}

//{{ 2012. 05. 10	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
SERIALIZE_DEFINE_PUT( KERM_MAX_PARTY_FEVER_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_MAX_PARTY_FEVER_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2009. 7. 27  최육사	헤니르 클리어 로그
SERIALIZE_DEFINE_PUT( KELOG_HENIR_DUNGEON_CLEAR_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iPlayTime )
		;
}

SERIALIZE_DEFINE_GET( KELOG_HENIR_DUNGEON_CLEAR_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iPlayTime )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KDBE_REQUEST_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_wstrNickName ) &&
            PUT( m_wstrMessage );
}

SERIALIZE_DEFINE_GET( KDBE_REQUEST_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_wstrNickName ) &&
            GET( m_wstrMessage );
}

SERIALIZE_DEFINE_PUT( KDBE_REQUEST_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iUnitUID ) &&
            PUT( m_wstrNickName ) &&
            PUT( m_wstrMessage );
}

SERIALIZE_DEFINE_GET( KDBE_REQUEST_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iUnitUID ) &&
            GET( m_wstrNickName ) &&
            GET( m_wstrMessage );
}

SERIALIZE_DEFINE_PUT( KELG_REQUEST_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iDestUnitUID ) &&
            PUT( m_wstrDestNickName ) &&
            PUT( m_iUnitUID ) &&
            PUT( m_wstrNickName ) &&
            PUT( m_wstrMessage );
}

SERIALIZE_DEFINE_GET( KELG_REQUEST_FRIEND_NOT, obj, ks )
{
    return  GET( m_iDestUnitUID ) &&
            GET( m_wstrDestNickName ) &&
            GET( m_iUnitUID ) &&
            GET( m_wstrNickName ) &&
            GET( m_wstrMessage );
}

SERIALIZE_DEFINE_PUT( KDBE_ACCEPT_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_ACCEPT_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_PUT( KELG_ACCEPT_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iDestUnitUID ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KELG_ACCEPT_FRIEND_NOT, obj, ks )
{
    return  GET( m_iDestUnitUID ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KDBE_DENY_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_DENY_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_PUT( KELG_DENY_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iDestUnitUID ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KELG_DENY_FRIEND_NOT, obj, ks )
{
    return  GET( m_iDestUnitUID ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KDBE_BLOCK_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_BLOCK_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_PUT( KELG_BLOCK_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iDestUnitUID ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KELG_BLOCK_FRIEND_NOT, obj, ks )
{
    return  GET( m_iDestUnitUID ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KDBE_UNBLOCK_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_UNBLOCK_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_PUT( KELG_UNBLOCK_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iDestUnitUID ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KELG_UNBLOCK_FRIEND_NOT, obj, ks )
{
    return  GET( m_iDestUnitUID ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KDBE_DELETE_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_iFriendUnitUID );
}

SERIALIZE_DEFINE_PUT( KELG_DELETE_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iDestUnitUID ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KELG_DELETE_FRIEND_NOT, obj, ks )
{
    return  GET( m_iDestUnitUID ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KDBE_MOVE_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_iFriendUnitUID ) &&
            PUT( m_cTargetGroupID );
}

SERIALIZE_DEFINE_GET( KDBE_MOVE_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_iFriendUnitUID ) &&
            GET( m_cTargetGroupID );
}

SERIALIZE_DEFINE_PUT( KDBE_MAKE_FRIEND_GROUP_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_cGroupID ) &&
            PUT( m_wstrGroupName );
}

SERIALIZE_DEFINE_GET( KDBE_MAKE_FRIEND_GROUP_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_cGroupID ) &&
            GET( m_wstrGroupName );
}

SERIALIZE_DEFINE_PUT( KDBE_RENAME_FRIEND_GROUP_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_cGroupID ) &&
            PUT( m_wstrGroupName );
}

SERIALIZE_DEFINE_GET( KDBE_RENAME_FRIEND_GROUP_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_cGroupID ) &&
            GET( m_wstrGroupName );
}

SERIALIZE_DEFINE_PUT( KDBE_DELETE_FRIEND_GROUP_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_cGroupID );
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_FRIEND_GROUP_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_cGroupID );
}

SERIALIZE_DEFINE_PUT( KDBE_FRIEND_MESSAGE_NOT, obj, ks )
{
    return  PUT( m_iSenderUnitUID ) &&
            PUT( m_wstrReceiverNickName ) &&
            PUT( m_cMessageType ) &&
            PUT( m_wstrMessage );
}

SERIALIZE_DEFINE_GET( KDBE_FRIEND_MESSAGE_NOT, obj, ks )
{
    return  GET( m_iSenderUnitUID ) &&
            GET( m_wstrReceiverNickName ) &&
            GET( m_cMessageType ) &&
            GET( m_wstrMessage );
}

SERIALIZE_DEFINE_PUT( KELG_UNIT_DISCONNECT_NOT, obj, ks )
{
    return PUT( m_iUnitUID )
		&& PUT( m_setFriendUID )
		//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_iGuildUID )
#endif GUILD_TEST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KELG_UNIT_DISCONNECT_NOT, obj, ks )
{
    return GET( m_iUnitUID )
		&& GET( m_setFriendUID )
		//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_iGuildUID )
#endif GUILD_TEST
		//}}
		;
}

SERIALIZE_DEFINE_PUT(  KELG_FRIEND_POSITION_UPDATE_NOT, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_setFriendUID ) &&
            PUT( m_cPosition );
}

SERIALIZE_DEFINE_GET( KELG_FRIEND_POSITION_UPDATE_NOT, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_setFriendUID ) &&
            GET( m_cPosition );
}

//{{ 2009. 7. 29  최육사	item set cheat
SERIALIZE_DEFINE_PUT( KDBE_ADMIN_GET_ITEM_SET_NOT, obj, ks )
{
	return PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_GET_ITEM_SET_NOT, obj, ks )
{
	return GET( m_wstrNickName )
		;
}
//}}

//{{ 2009. 8. 27  최육사	밀봉
SERIALIZE_DEFINE_PUT( KDBE_SEAL_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_ucSealResult )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SEAL_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_ucSealResult )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SEAL_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_ucSealResult )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SEAL_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_ucSealResult )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
/* 해외용 패킷쪽으로 위치 이동 시킴
//09. 08. 06 김정협 Gash인증을 위하여 SERIALIZE 작성
SERIALIZE_DEFINE_PUT(  KEGASH_AUTHENTICATION_REQ, obj, ks )
{
	return  PUT( m_iActorUID ) &&
            PUT( m_strServiceCode ) &&
		    PUT( m_strServiceRegion ) &&
		    PUT( m_strServiceAccountID ) &&
		    PUT( m_strServicePassword ) &&
		    PUT( m_strLockFlag ) &&
		    PUT( m_strIPAddress ) &&
		    PUT( m_strPacketID );
}

SERIALIZE_DEFINE_GET(  KEGASH_AUTHENTICATION_REQ, obj, ks )
{
	return  GET( m_iActorUID ) &&
            GET( m_strServiceCode ) &&
		    GET( m_strServiceRegion ) &&
		    GET( m_strServiceAccountID ) &&
		    GET( m_strServicePassword ) &&
		    GET( m_strLockFlag ) &&
		    GET( m_strIPAddress ) &&
		    GET( m_strPacketID );
}


//09. 08. 06 김정협 Gash인증을 위하여 SERIALIZE 작성
SERIALIZE_DEFINE_PUT(  KEGASH_AUTHORIZATION_REQ, obj, ks )
{
	return  PUT( m_iActorUID ) &&
            PUT( m_strServiceCode ) &&
		    PUT( m_strServiceRegion ) &&
		    PUT( m_strServiceAccountID ) &&
		    PUT( m_strServicePassword ) &&
		    PUT( m_strAuthenticationFlag ) &&
		    PUT( m_strType ) &&
		    PUT( m_strDuplicateCheck ) &&
		    PUT( m_strIPAddress ) &&
		    PUT( m_strPacketID );
}

SERIALIZE_DEFINE_GET(  KEGASH_AUTHORIZATION_REQ, obj, ks )
{
	return  GET( m_iActorUID ) &&
            GET( m_strServiceCode ) &&
		    GET( m_strServiceRegion ) &&
		    GET( m_strServiceAccountID ) &&
		    GET( m_strServicePassword ) &&
		    GET( m_strAuthenticationFlag ) &&
		    GET( m_strType ) &&
		    GET( m_strDuplicateCheck ) &&
		    GET( m_strIPAddress ) &&
		    GET( m_strPacketID );
}



//09. 08. 06 김정협 Gash인증을 위하여 SERIALIZE 작성
SERIALIZE_DEFINE_PUT(  KEGASH_ACCOUNTING_REQ, obj, ks )
{
	return  PUT( m_iActorUID ) &&
            PUT( m_strServiceCode ) &&
		    PUT( m_strServiceRegion ) &&
		    PUT( m_strServiceAccountID ) &&
		    PUT( m_strType ) &&
		    PUT( m_strLoginTime ) &&
		    PUT( m_strLogoutTime ) &&
		    PUT( m_strIPAddress ) &&
		    PUT( m_strPacketID );
}

SERIALIZE_DEFINE_GET(  KEGASH_ACCOUNTING_REQ, obj, ks )
{
	return  GET( m_iActorUID ) &&
            GET( m_strServiceCode ) &&
		    GET( m_strServiceRegion ) &&
		    GET( m_strServiceAccountID ) &&
		    GET( m_strType ) &&
		    GET( m_strLoginTime ) &&
		    GET( m_strLogoutTime ) &&
		    GET( m_strIPAddress ) &&
		    GET( m_strPacketID );
}

SERIALIZE_DEFINE_PUT( KGashResponseData, obj, ks )
{
	return  PUT( m_strResultCode ) &&
		    PUT( m_strOutString ) &&
		    PUT( m_strPacketID );
}

SERIALIZE_DEFINE_GET( KGashResponseData, obj, ks )
{
	return  GET( m_strResultCode ) &&
		    GET( m_strOutString ) &&
		    GET( m_strPacketID );
}


SERIALIZE_DEFINE_PUT( KDBE_GASH_USER_LOGIN_REQ, obj, ks )
{
	return  PUT( m_wstrServiceAccountID ) &&
			PUT( m_wstrOTP );
};

SERIALIZE_DEFINE_GET( KDBE_GASH_USER_LOGIN_REQ, obj, ks )
{
	return  GET( m_wstrServiceAccountID ) &&
		    GET( m_wstrOTP );
};

SERIALIZE_DEFINE_PUT( KDBE_GASH_USER_LOGIN_ACK, obj, ks )
{
	return  PUT( m_iOK ) &&
            PUT( m_iUserUID ) &&
			PUT( m_wstrOTP );
};

SERIALIZE_DEFINE_GET( KDBE_GASH_USER_LOGIN_ACK, obj, ks )
{
	return  GET( m_iOK ) &&
            GET( m_iUserUID ) &&
			GET( m_wstrOTP );
};
*/

//{{ 2009. 9. 1  최육사		비정상종료통계
SERIALIZE_DEFINE_PUT( KELOG_ABNORMAL_DISCONNECT, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_mapAbnormalDisconnect )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ABNORMAL_DISCONNECT, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_mapAbnormalDisconnect )
		;
}
//}}

//{{ 2009. 9. 9  최육사		밀봉
SERIALIZE_DEFINE_PUT( KELOG_USE_SEAL_ITEM_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iDestItemUID )
		&& PUT( m_iSealItemUID )
		&& PUT( m_ucSealCount )
		;
}

SERIALIZE_DEFINE_GET( KELOG_USE_SEAL_ITEM_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iDestItemUID )
		&& GET( m_iSealItemUID )
		&& GET( m_ucSealCount )
		;
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 9. 17  최육사	길드
#ifdef GUILD_TEST

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
SERIALIZE_DEFINE_PUT( KCreateGuildREQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_wstrGuildName )
		&& PUT( m_wstrGuildMessage )
		;
}

SERIALIZE_DEFINE_GET( KCreateGuildREQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_wstrGuildName )
		&& GET( m_wstrGuildMessage )
		;
}

SERIALIZE_DEFINE_PUT( KCreateGuildACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_kCreatedGuildInfo )
		//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		&& PUT( m_bReqGiant_DeleteName )
		&& PUT( m_wstrGuildName )
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		;
}

SERIALIZE_DEFINE_GET( KCreateGuildACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_kCreatedGuildInfo )
		//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		&& GET( m_bReqGiant_DeleteName )
		&& GET( m_wstrGuildName )
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		;
}

SERIALIZE_DEFINE_PUT( KChangeGuildNameCheckREQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNewGuildName )
		;
}

SERIALIZE_DEFINE_GET( KChangeGuildNameCheckREQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNewGuildName )
		;
}

SERIALIZE_DEFINE_PUT( KChangeGuildNameREQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_wstrOldGuildName )
		&& PUT( m_wstrNewGuildName )
		;
}

SERIALIZE_DEFINE_GET( KChangeGuildNameREQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_wstrOldGuildName )
		&& GET( m_wstrNewGuildName )
		;
}

SERIALIZE_DEFINE_PUT( KChangeGuildNameACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_kGuildInfo )
		&& PUT( m_wstrOldGuildName )
		;
}

SERIALIZE_DEFINE_GET( KChangeGuildNameACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_kGuildInfo )
		&& GET( m_wstrOldGuildName )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_GUILD_NAME_CHECK_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_GUILD_NAME_CHECK_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		;
}
#else
SERIALIZE_DEFINE_PUT( KDBE_CREATE_GUILD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_wstrGuildName )
		&& PUT( m_wstrGuildMessage )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_GUILD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_wstrGuildName )
		&& GET( m_wstrGuildMessage )		
		;
}
SERIALIZE_DEFINE_PUT( KDBE_CREATE_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_kCreatedGuildInfo )
		//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		&& PUT( m_bReqGiant_DeleteName )
		&& PUT( m_wstrGuildName )
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_kCreatedGuildInfo )
		//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		&& GET( m_bReqGiant_DeleteName )
		&& GET( m_wstrGuildName )
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		;
}


#endif SERV_GUILD_CHANGE_NAME
//}}
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CREATE_GUILD_RESULT_REQ, obj, ks )
{
	return PUT( m_kGuildInfo )
		&& PUT( m_vecMemberList )		
		&& PUT( m_kLoginGuildMember )
		//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KELG_CREATE_GUILD_RESULT_REQ, obj, ks )
{
	return GET( m_kGuildInfo )
		&& GET( m_vecMemberList )		
		&& GET( m_kLoginGuildMember )
		//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_INFO_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_kLoginGuildMember )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_GUILD_INFO_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_kLoginGuildMember )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_GET_GUILD_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kGuildInfo )
		&& PUT( m_vecMemberList )
		&& PUT( m_kLoginGuildMember )
		//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
#ifdef SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
		&& PUT( m_mapRankerUnitInfo )
		&& PUT( m_mapRankerUIDInfo )
		&& PUT( m_mapRankerUserInfo )
#endif // SERV_LOCAL_RANKING_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_GUILD_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kGuildInfo )
		&& GET( m_vecMemberList )
		&& GET( m_kLoginGuildMember )
		//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
#ifdef SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
		&& GET( m_mapRankerUnitInfo )
		&& GET( m_mapRankerUIDInfo )
		&& GET( m_mapRankerUserInfo )
#endif // SERV_LOCAL_RANKING_SYSTEM
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_USER_LIST_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_uiViewPage )
		&& PUT( m_cGuildUserListType )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_GUILD_USER_LIST_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_uiViewPage )
		&& GET( m_cGuildUserListType )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_UPDATE_GUILD_MEMBER_INFO_NOT, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_kMemberInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_UPDATE_GUILD_MEMBER_INFO_NOT, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_kMemberInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_INVITE_GUILD_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iSenderUnitUID )
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_iReceiverUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_GUILD_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iSenderUnitUID )
		&& GET( m_wstrReceiverNickName )
		&& GET( m_iReceiverUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_INVITE_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrReceiverNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrReceiverNickName )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_INVITE_GUILD_FIND_USER_REQ, obj, ks )
{
	return PUT( m_usEventID )
		&& PUT( m_kGuildInfo )
		&& PUT( m_iSenderUnitUID )
		&& PUT( m_wstrSenderNickName )
		&& PUT( m_iReceiverUnitUID )
		&& PUT( m_bAutoJoinGuild )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_GUILD_FIND_USER_REQ, obj, ks )
{
	return GET( m_usEventID )
		&& GET( m_kGuildInfo )
		&& GET( m_iSenderUnitUID )
		&& GET( m_wstrSenderNickName )
		&& GET( m_iReceiverUnitUID )
		&& GET( m_bAutoJoinGuild )
		;
}


SERIALIZE_DEFINE_PUT( KELG_INVITE_GUILD_FIND_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_usEventID )
		&& PUT( m_iGuildUID )
		&& PUT( m_iSenderUnitUID )
		&& PUT( m_iReceiverUnitUID )
		&& PUT( m_wstrReceiverNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_GUILD_FIND_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_usEventID )
		&& GET( m_iGuildUID )
		&& GET( m_iSenderUnitUID )
		&& GET( m_iReceiverUnitUID )
		&& GET( m_wstrReceiverNickName )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_INVITE_GUILD_REPLY_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_wstrSenderNickName )
		&& PUT( m_iReason )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_GUILD_REPLY_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_wstrSenderNickName )
		&& GET( m_iReason )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		;
}


SERIALIZE_DEFINE_PUT( KELG_INVITE_GUILD_REPLY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iGuildMasterUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_GUILD_REPLY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iGuildMasterUnitUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_JOIN_GUILD_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_kJoinGuildMember )		
		&& PUT( m_iGuildMasterUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_JOIN_GUILD_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_kJoinGuildMember )		
		&& GET( m_iGuildMasterUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_JOIN_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_kJoinGuildMember )
		;
}

SERIALIZE_DEFINE_GET( KDBE_JOIN_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_kJoinGuildMember )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_JOIN_GUILD_REQ, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_kLoginGuildMember )
		;
}

SERIALIZE_DEFINE_GET( KELG_JOIN_GUILD_REQ, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_kLoginGuildMember )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_JOIN_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kGuildInfoNot )
		;
}

SERIALIZE_DEFINE_GET( KELG_JOIN_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kGuildInfoNot )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHANGE_GUILD_MEMBER_GRADE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_iTargetUnitUID )
		&& PUT( m_wstrTargetNickName )
		&& PUT( m_ucMemberShipGrade )
		&& PUT( m_bChangeGuildMaster )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_GUILD_MEMBER_GRADE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_iTargetUnitUID )
		&& GET( m_wstrTargetNickName )
		&& GET( m_ucMemberShipGrade )
		&& GET( m_bChangeGuildMaster )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iTargetUnitUID )
		&& PUT( m_wstrTargetNickName )
		&& PUT( m_ucMemberShipGrade )
		&& PUT( m_bChangeGuildMaster )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iTargetUnitUID )
		&& GET( m_wstrTargetNickName )
		&& GET( m_ucMemberShipGrade )
		&& GET( m_bChangeGuildMaster )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHANGE_GUILD_MESSAGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_GUILD_MESSAGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_GUILD_MESSAGE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_GUILD_MESSAGE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_wstrMessage )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_KICK_GUILD_MEMBER_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_iTargetUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_KICK_GUILD_MEMBER_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_iTargetUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_KICK_GUILD_MEMBER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_iTargetUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_KICK_GUILD_MEMBER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_iTargetUnitUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_KICK_RESULT_LETTER_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrGuildName )
		;
}

SERIALIZE_DEFINE_GET( KELG_KICK_RESULT_LETTER_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrGuildName )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iTargetUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_GUILD_MEMBER_MESSAGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iTargetUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iTargetUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iTargetUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_wstrMessage )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_DISBAND_GUILD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DISBAND_GUILD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DISBAND_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DISBAND_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_iItemUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_iItemUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPAND_GUILD_MAX_MEMBER_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_GUILD_MAX_MEMBER_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXPAND_GUILD_MAX_MEMBER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_usMaxNumMember )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_GUILD_MAX_MEMBER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_usMaxNumMember )
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 10. 27  최육사	길드레벨
SERIALIZE_DEFINE_PUT( KDBE_UPDATE_GUILD_EXP_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_iHonorPoint )
		&& PUT( m_iGuildEXP )
		&& PUT( m_iGuildLimitEXP )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_GUILD_EXP_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_iHonorPoint )
		&& GET( m_iGuildEXP )
		&& GET( m_iGuildLimitEXP )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_GUILD_EXP_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )		
		&& PUT( m_iGuildEXP )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_GUILD_EXP_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_iGuildEXP )
		;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
SERIALIZE_DEFINE_PUT( KELG_CHANGE_GUILD_NAME_RESULT_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_wstrGuildName )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANGE_GUILD_NAME_RESULT_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_wstrGuildName )
		;
}
//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_ulOrderNo )
		&& PUT( m_ulProductNo )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_ulOrderNo )
		&& GET( m_ulProductNo )
		&& GET( m_iQuantity )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CHANGE_GUILD_NAME_TIME_CHECK_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_ulOrderNo )
		&& PUT( m_ulProductNo )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANGE_GUILD_NAME_TIME_CHECK_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_ulOrderNo )
		&& GET( m_ulProductNo )
		&& GET( m_iQuantity )
		;
}
#endif SERV_GUILD_CHANGE_NAME
//}}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_GUILD_LEVEL_UP_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_wstrGuildName )
		&& PUT( m_ucGuildLevel )
		//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_iGuildSPoint )
#endif GUILD_SKILL_TEST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_GUILD_LEVEL_UP_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_wstrGuildName )
		&& GET( m_ucGuildLevel )
		//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_iGuildSPoint )
#endif GUILD_SKILL_TEST
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GUILD_LEVEL_UP_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_wstrGuildName )
		&& PUT( m_ucGuildLevel )
		//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_iGuildSPoint )
#endif GUILD_SKILL_TEST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_GUILD_LEVEL_UP_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_wstrGuildName )
		&& GET( m_ucGuildLevel )
		//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_iGuildSPoint )
#endif GUILD_SKILL_TEST
		//}}
		;
}
//}}


#endif GUILD_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 9. 23  최육사	1차전직캐쉬템
SERIALIZE_DEFINE_PUT( KERM_CHANGE_FIELD_USER_INFO_NOT, obj, ks )
{
	return PUT( m_kInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHANGE_FIELD_USER_INFO_NOT, obj, ks )
{
	return GET( m_kInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KETR_CHANGE_FIELD_USER_INFO_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_wstrGuildName )
#endif GUILD_TEST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KETR_CHANGE_FIELD_USER_INFO_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		//{{ 2009. 10. 9  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_wstrGuildName )
#endif GUILD_TEST
		//}}
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 10. 26  최육사	트래픽어택
SERIALIZE_DEFINE_PUT( KELOG_TRAFFIC_ATTACKER_LOG_NOT, obj, ks )
{
	return PUT( m_wstrUserID )
		&& PUT( m_iChannelID )
		;
}

SERIALIZE_DEFINE_GET( KELOG_TRAFFIC_ATTACKER_LOG_NOT, obj, ks )
{
	return GET( m_wstrUserID )
		&& GET( m_iChannelID )
		;
}
//}}

//{{ 2009. 10. 27  최육사	랭킹
SERIALIZE_DEFINE_PUT( KELG_FIND_DELETE_CHAR_IN_RANKING_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecRankingType )
		;
}

SERIALIZE_DEFINE_GET( KELG_FIND_DELETE_CHAR_IN_RANKING_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecRankingType )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_SKILL_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildSkillID )		
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_GUILD_SKILL_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildSkillID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_GET_GUILD_SKILL_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildSkillID )
		&& PUT( m_iGuildSkillLevel )
		&& PUT( m_iGuildSkillCSPoint )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_iMaxGuildCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_GUILD_SKILL_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildSkillID )
		&& GET( m_iGuildSkillLevel )
		&& GET( m_iGuildSkillCSPoint )
		&& GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		&& GET( m_iMaxGuildCSPoint )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_GET_GUILD_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildSkillID )
		&& PUT( m_iGuildSkillLevel )
		&& PUT( m_iGuildSkillCSPoint )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_GUILD_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildSkillID )
		&& GET( m_iGuildSkillLevel )
		&& GET( m_iGuildSkillCSPoint )
		&& GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_RESET_GUILD_SKILL_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildSkillID )
		&& PUT( m_iGuildSkillLevel )
		&& PUT( m_iGuildSkillCSPoint )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_iMaxGuildCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_GUILD_SKILL_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildSkillID )
		&& GET( m_iGuildSkillLevel )
		&& GET( m_iGuildSkillCSPoint )
		&& GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		&& GET( m_iMaxGuildCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_RESET_GUILD_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )		
		&& PUT( m_iGuildSkillID )
		&& PUT( m_iGuildSkillLevel )
		&& PUT( m_iGuildSkillCSPoint )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_GUILD_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )		
		&& GET( m_iGuildSkillID )
		&& GET( m_iGuildSkillLevel )
		&& GET( m_iGuildSkillCSPoint )
		&& GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_INIT_GUILD_SKILL_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_INIT_GUILD_SKILL_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iItemUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_INIT_GUILD_SKILL_NOT, obj, ks )
{
	return PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KELG_INIT_GUILD_SKILL_NOT, obj, ks )
{
	return GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INIT_GUILD_SKILL_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INIT_GUILD_SKILL_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INIT_GUILD_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INIT_GUILD_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_iPeriod )
		;
}

SERIALIZE_DEFINE_GET( KELG_INSERT_GUILD_CASH_SKILL_POINT_NOT, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildCSPoint )
		&& GET( m_iPeriod )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INSERT_GUILD_CASH_SKILL_POINT_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_iPeriod )
		&& PUT( m_bUpdateEndDateOnly )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_GUILD_CASH_SKILL_POINT_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iGuildCSPoint )
		&& GET( m_iPeriod )
		&& GET( m_bUpdateEndDateOnly )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_iPeriod )
		&& PUT( m_wstrEndDate )
		&& PUT( m_bUpdateEndDateOnly )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iGuildCSPoint )
		&& GET( m_iPeriod )
		&& GET( m_wstrEndDate )
		&& GET( m_bUpdateEndDateOnly )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iRetrievedSPoint )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_vecGuildSkillData )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iRetrievedSPoint )
		&& GET( m_iGuildSPoint )
		&& GET( m_vecGuildSkillData )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iRetrievedSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iRetrievedSPoint )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildSPoint )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildSPoint )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildSPoint )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildSPoint )
		;
}

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 12. 8  최육사	크리스마스이벤트
SERIALIZE_DEFINE_PUT( KDBE_CHECK_TIME_EVENT_COMPLETE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecCompletedEvent )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_TIME_EVENT_COMPLETE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecCompletedEvent )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHECK_TIME_EVENT_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRewardCount )
		&& PUT( m_vecRestartEvent )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_TIME_EVENT_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRewardCount )
		&& GET( m_vecRestartEvent )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 16  최육사	동접툴
SERIALIZE_DEFINE_PUT( KDBE_GET_CONCURRENT_USER_INFO_REQ, obj, ks )
{
	return PUT( m_cCCUType )
		&& PUT( m_iServerUID )
		&& PUT( m_wstrDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_CONCURRENT_USER_INFO_REQ, obj, ks )
{
	return GET( m_cCCUType )
		&& GET( m_iServerUID )
		&& GET( m_wstrDate )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_GET_CONCURRENT_USER_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_cCCUType )
		&& PUT( m_iServerUID )
		&& PUT( m_wstrDate )
		&& PUT( m_vecCCUInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_CONCURRENT_USER_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_cCCUType )
		&& GET( m_iServerUID )
		&& GET( m_wstrDate )
		&& GET( m_vecCCUInfo )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 01. 04  최육사	길드 광고
#ifdef SERV_GUILD_AD

SERIALIZE_DEFINE_PUT( KDBE_GET_GUILD_BOARD_INFO_ACK, obj, ks )
{
	return PUT( m_vecGuildAdList )
		&& PUT( m_vecApplyJoinGuildList )
#ifdef SERV_GUILD_FIND_AD
		&& PUT( m_vecGuildFindAdList )
		&& PUT( m_vecGuildInviteMsgList )
#endif SERV_GUILD_FIND_AD
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_GUILD_BOARD_INFO_ACK, obj, ks )
{
	return GET( m_vecGuildAdList )
		&& GET( m_vecApplyJoinGuildList )
#ifdef SERV_GUILD_FIND_AD
		&& GET( m_vecGuildFindAdList )
		&& GET( m_vecGuildInviteMsgList )
#endif SERV_GUILD_FIND_AD
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_AD_LIST_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cSortType )
		&& PUT( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_GUILD_AD_LIST_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_cSortType )
		&& GET( m_uiViewPage )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_SKILL_IN_BOARD_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_GUILD_SKILL_IN_BOARD_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_GET_GUILD_SKILL_IN_BOARD_ACK, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kGuildSkillInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_GUILD_SKILL_IN_BOARD_ACK, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_kGuildSkillInfo )
		;
}


//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KELG_REGISTRATION_GUILD_AD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_sPeriod )
		&& PUT( m_wstrAdMessage )
		&& PUT( m_iCost )
		;
}

SERIALIZE_DEFINE_GET( KELG_REGISTRATION_GUILD_AD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_sPeriod )
		&& GET( m_wstrAdMessage )
		&& GET( m_iCost )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_REGISTRATION_GUILD_AD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_sPeriod )
		&& PUT( m_wstrAdMessage )
		&& PUT( m_iCost )
		;
}

SERIALIZE_DEFINE_GET( KELG_REGISTRATION_GUILD_AD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_sPeriod )
		&& GET( m_wstrAdMessage )
		&& GET( m_iCost )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_REGISTRATION_GUILD_AD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_sPeriod )
		&& PUT( m_wstrAdMessage )
		&& PUT( m_iCost )
		&& PUT( m_bExistExpiredAd )
		;
}

SERIALIZE_DEFINE_GET( KDBE_REGISTRATION_GUILD_AD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_sPeriod )
		&& GET( m_wstrAdMessage )
		&& GET( m_iCost )
		&& GET( m_bExistExpiredAd )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_REGISTRATION_GUILD_AD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iGuildUID )
		&& PUT( m_sPeriod )
		&& PUT( m_wstrAdRegDate )
		&& PUT( m_wstrAdEndDate )
		&& PUT( m_wstrAdMessage )
		&& PUT( m_iCost )
		&& PUT( m_bExistExpiredAd )
		;
}

SERIALIZE_DEFINE_GET( KDBE_REGISTRATION_GUILD_AD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iGuildUID )
		&& GET( m_sPeriod )
		&& GET( m_wstrAdRegDate )
		&& GET( m_wstrAdEndDate )
		&& GET( m_wstrAdMessage )
		&& GET( m_iCost )
		&& GET( m_bExistExpiredAd )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_APPLY_JOIN_GUILD_REQ, obj, ks )
{
	return PUT( m_kApplyJoinGuildInfo )
		&& PUT( m_bOldApplyDelete )
		;
}

SERIALIZE_DEFINE_GET( KELG_APPLY_JOIN_GUILD_REQ, obj, ks )
{
	return GET( m_kApplyJoinGuildInfo )
		&& GET( m_bOldApplyDelete )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_APPLY_JOIN_GUILD_REQ, obj, ks )
{
	return PUT( m_kApplyJoinGuildInfo )
		&& PUT( m_bOldApplyDelete )
		&& PUT( m_vecDeleteApply )
		;
}

SERIALIZE_DEFINE_GET( KDBE_APPLY_JOIN_GUILD_REQ, obj, ks )
{
	return GET( m_kApplyJoinGuildInfo )
		&& GET( m_bOldApplyDelete )
		&& GET( m_vecDeleteApply )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_APPLY_JOIN_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kApplyJoinGuildInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_APPLY_JOIN_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kApplyJoinGuildInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_GET_APPLY_JOIN_GUILD_LIST_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_APPLY_JOIN_GUILD_LIST_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_uiViewPage )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_ACCEPT_JOIN_GUILD_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iApplyJoinUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_ACCEPT_JOIN_GUILD_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iApplyJoinUnitUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iApplyJoinUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iApplyJoinUnitUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_ACCEPT_JOIN_GUILD_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_kJoinGuildMemberInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCEPT_JOIN_GUILD_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_kJoinGuildMemberInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_ACCEPT_JOIN_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_kJoinGuildMemberInfo )
		&& PUT( m_wstrGuildName )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCEPT_JOIN_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_kJoinGuildMemberInfo )
		&& GET( m_wstrGuildName )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_DELETE_APPLY_JOIN_GUILD_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iDeletedUnitUID )
		&& PUT( m_bDeleteChar )
		;
}

SERIALIZE_DEFINE_GET( KELG_DELETE_APPLY_JOIN_GUILD_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iDeletedUnitUID )
		&& GET( m_bDeleteChar )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_DELETE_APPLY_JOIN_GUILD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iDeletedUnitUID )
		&& PUT( m_bDeleteChar )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_APPLY_JOIN_GUILD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iDeletedUnitUID )
		&& GET( m_bDeleteChar )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_DELETE_GUILD_AD_LIST_NOT, obj, ks )
{
	return PUT( m_vecDeleteGuildAd )
		&& PUT( m_vecDeleteApply )
#ifdef SERV_GUILD_FIND_AD
		&& PUT( m_vecDeleteGuildFindAd )
		&& PUT( m_vecDeleteInvite )
#endif SERV_GUILD_FIND_AD
		;
}

SERIALIZE_DEFINE_GET( KDBE_DELETE_GUILD_AD_LIST_NOT, obj, ks )
{
	return GET( m_vecDeleteGuildAd )
		&& GET( m_vecDeleteApply )
#ifdef SERV_GUILD_FIND_AD
		&& GET( m_vecDeleteGuildFindAd )
		&& GET( m_vecDeleteInvite )
#endif SERV_GUILD_FIND_AD
		;
}


#endif SERV_GUILD_AD
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 01. 11  최육사	추천인리스트
SERIALIZE_DEFINE_PUT( KDBE_GET_RECOMMEND_USER_LIST_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_RECOMMEND_USER_LIST_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_GET_RECOMMEND_USER_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecRecommendUserList )
		//{{ 2012. 01. 17  김민성	추천인 리스트 최대 전송 패킷 크기 조절
#ifdef SERV_RECOMMEND_LIST_OVER_SIZE
		&& PUT( m_uiViewPage )
		&& PUT( m_uiTotalPage )
#endif SERV_RECOMMEND_LIST_OVER_SIZE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_RECOMMEND_USER_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecRecommendUserList )
		//{{ 2012. 01. 17  김민성	추천인 리스트 최대 전송 패킷 크기 조절
#ifdef SERV_RECOMMEND_LIST_OVER_SIZE
		&& GET( m_uiViewPage )
		&& GET( m_uiTotalPage )
#endif SERV_RECOMMEND_LIST_OVER_SIZE
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_RECOMMEND_USER_INFO_NOT, obj, ks )
{
	return PUT( m_iRecommendedUnitUID )
		&& PUT( m_iRecommendUnitUID )
		&& PUT( m_cType )
		&& PUT( m_iData )
		;
}

SERIALIZE_DEFINE_GET( KELG_RECOMMEND_USER_INFO_NOT, obj, ks )
{
	return GET( m_iRecommendedUnitUID )
		&& GET( m_iRecommendUnitUID )
		&& GET( m_cType )
		&& GET( m_iData )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 01. 27  최육사	방 정보
#ifdef SERV_ROOM_COUNT

SERIALIZE_DEFINE_PUT( KERM_ROOM_COUNT_INFO_NOT, obj, ks )
{
	return PUT( m_cRoomType )
		&& PUT( m_bIsOpen )
		;
}

SERIALIZE_DEFINE_GET( KERM_ROOM_COUNT_INFO_NOT, obj, ks )
{
	return GET( m_cRoomType )
		&& GET( m_bIsOpen )
		;
}

#endif SERV_ROOM_COUNT
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST

SERIALIZE_DEFINE_PUT( KDBE_CHECK_HACKING_TOOL_LIST_REQ, obj, ks )
{
	return PUT( m_iReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_HACKING_TOOL_LIST_REQ, obj, ks )
{
	return GET( m_iReleaseTick )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHECK_HACKING_TOOL_LIST_ACK, obj, ks )
{
	return PUT( m_iReleaseTick )
		&& PUT( m_vecHackingToolList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_HACKING_TOOL_LIST_ACK, obj, ks )
{
	return GET( m_iReleaseTick )
		&& GET( m_vecHackingToolList )
		;
}

#endif SERV_HACKING_TOOL_LIST
//}}



//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM

SERIALIZE_DEFINE_PUT( KELG_INVITE_PARTY_FIND_RECEIVER_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_iHostUnitUID )
		&& PUT( m_wstrHostNickName )
		&& PUT( m_iReceiverUnitUID )
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
		&& PUT( m_iServerGroupID )
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_kCreatePartyInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_PARTY_FIND_RECEIVER_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_iHostUnitUID )
		&& GET( m_wstrHostNickName )
		&& GET( m_iReceiverUnitUID )
#ifdef	SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE // 적용날짜: 2013-04-04
		&& GET( m_iServerGroupID )
#endif	// SERV_GLOBAL_USER_LIST_NICKNAME_STORAGE
		&& GET( m_wstrReceiverNickName )
		&& GET( m_kCreatePartyInfo )
		;
}


SERIALIZE_DEFINE_PUT( KELG_INVITE_PARTY_FIND_RECEIVER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iHostUnitUID )
		&& PUT( m_vecReceiverPartyUserInfo )
		&& PUT( m_kCreatePartyInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_PARTY_FIND_RECEIVER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iHostUnitUID )
		&& GET( m_vecReceiverPartyUserInfo )
		&& GET( m_kCreatePartyInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_LEAVE_PARTY_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_iReason )
		//{{ 2011. 11. 28	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_iNewJoinPartyUID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPM_LEAVE_PARTY_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_iReason )
		//{{ 2011. 11. 28	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_iNewJoinPartyUID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KEPM_LEAVE_PARTY_ACK, obj, ks )
{
	return PUT( m_iOK )
		//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
		&& PUT( m_iReason )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
		//}}
		&& PUT( m_iPartyUID )
		//{{ 2011. 11. 28	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_iNewJoinPartyUID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPM_LEAVE_PARTY_ACK, obj, ks )
{
	return GET( m_iOK )
		//{{ 2011. 02. 21  김민성	특정 던전 입장 아이템 버그 수정
#ifdef SERV_DUNGEON_REQUIRED_ITEM_BUG
		&& GET( m_iReason )
#endif SERV_DUNGEON_REQUIRED_ITEM_BUG
		//}}
		&& GET( m_iPartyUID )
		//{{ 2011. 11. 28	최육사	배틀필드
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_iNewJoinPartyUID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_INVITE_PARTY_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_kPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_INVITE_PARTY_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_kPartyUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_INVITE_PARTY_REPLY_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_cReplyReason )
		&& PUT( m_kPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_INVITE_PARTY_REPLY_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_cReplyReason )
		&& GET( m_kPartyUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_INVITE_PARTY_REPLY_TO_HOST_NOT, obj, ks )
{
	return PUT( m_iHostUnitUID )
		&& PUT( m_cReplyReason )
		&& PUT( m_kPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_INVITE_PARTY_REPLY_TO_HOST_NOT, obj, ks )
{
	return GET( m_iHostUnitUID )
		&& GET( m_cReplyReason )
		&& GET( m_kPartyUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CREATE_PARTY_BY_INVITE_REQ, obj, ks )
{
	return PUT( m_cAuthLevel )
		&& PUT( m_kCreatePartyInfo )
		&& PUT( m_kHostPartyUserInfo )
		&& PUT( m_kReceiverPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CREATE_PARTY_BY_INVITE_REQ, obj, ks )
{
	return GET( m_cAuthLevel )
		&& GET( m_kCreatePartyInfo )
		&& GET( m_kHostPartyUserInfo )
		&& GET( m_kReceiverPartyUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CREATE_PARTY_BY_INVITE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kPartyInfo )
		&& PUT( m_kHostPartyUserInfo )
		&& PUT( m_kReceiverPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CREATE_PARTY_BY_INVITE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kPartyInfo )
		&& GET( m_kHostPartyUserInfo )
		&& GET( m_kReceiverPartyUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_UPDATE_PARTY_USER_INFO_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_bChangeField )
		&& PUT( m_iUnitUID )
		&& PUT( m_cState )
		&& PUT( m_iStateCode )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		;
}

SERIALIZE_DEFINE_GET( KEPM_UPDATE_PARTY_USER_INFO_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_bChangeField )
		&& GET( m_iUnitUID )
		&& GET( m_cState )
		&& GET( m_iStateCode )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_CHANGE_READY_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_bReady )
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_CHANGE_READY_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_bReady )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_CHANGE_DUNGEON_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_iDungeonID )
		&& PUT( m_DifficultyLevel )
		&& PUT( m_cDungeonMode )
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_CHANGE_DUNGEON_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_iDungeonID )
		&& GET( m_DifficultyLevel )
		&& GET( m_cDungeonMode )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_GAME_START_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_sWorldID )
		&& PUT( m_usEventID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_GAME_START_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_sWorldID )
		&& GET( m_usEventID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_GAME_START_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_usEventID )
		//{{ 2012. 05. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_iDungeonMode )
		&& PUT( m_iDungeonID )
		&& PUT( m_iFailUnitUID )
		&& PUT( m_wstrFailUserNickName )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_GAME_START_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_usEventID )
		//{{ 2012. 05. 18	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_iDungeonMode )
		&& GET( m_iDungeonID )
		&& GET( m_iFailUnitUID )
		&& GET( m_wstrFailUserNickName )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CHECK_FOR_PARTY_GAME_START_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_usEventID )
		&& PUT( m_sWorldID )
		&& PUT( m_kDungeonGameInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHECK_FOR_PARTY_GAME_START_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_usEventID )
		&& GET( m_sWorldID )
		&& GET( m_kDungeonGameInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CHECK_FOR_PARTY_GAME_START_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPartyUID )
		&& PUT( m_usEventID )
		&& PUT( m_sWorldID )
		&& PUT( m_vecUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHECK_FOR_PARTY_GAME_START_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPartyUID )
		&& GET( m_usEventID )
		&& GET( m_sWorldID )
		&& GET( m_vecUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_END_GAME_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_END_GAME_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_CHANGE_HOST_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_iNewHostUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_CHANGE_HOST_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_iNewHostUnitUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CHECK_PARTY_CHANGE_HOST_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_cPartyType )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_iOldHostUnitUID )
		&& PUT( m_iNewHostUnitUID )
		&& PUT( m_iDungeonID )
		&& PUT( m_cDifficulty )
		&& PUT( m_cDungeonMode )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& PUT( m_bForRegroupParty )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHECK_PARTY_CHANGE_HOST_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		//{{ 2011. 07. 19	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_cPartyType )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_iOldHostUnitUID )
		&& GET( m_iNewHostUnitUID )
		&& GET( m_iDungeonID )
		&& GET( m_cDifficulty )
		&& GET( m_cDungeonMode )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& GET( m_bForRegroupParty )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KEPM_CHECK_PARTY_CHANGE_HOST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPartyUID )
		&& PUT( m_iOldHostUnitUID )
		&& PUT( m_iNewHostUnitUID )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& PUT( m_bForRegroupParty )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHECK_PARTY_CHANGE_HOST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPartyUID )
		&& GET( m_iOldHostUnitUID )
		&& GET( m_iNewHostUnitUID )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& GET( m_bForRegroupParty )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_CHANGE_NUM_OF_PER_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_cNumOfPer )
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_CHANGE_NUM_OF_PER_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_cNumOfPer )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_cGetItemType )
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_CHANGE_GET_ITEM_TYPE_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_cGetItemType )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CHAT_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_cChatPacketType )
		&& PUT( m_cRoomChatType )
		&& PUT( m_ToUnitUID )
		&& PUT( m_wstrRecieverUnitNickName )
		&& PUT( m_wstrMsg )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHAT_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_cChatPacketType )
		&& GET( m_cRoomChatType )
		&& GET( m_ToUnitUID )
		&& GET( m_wstrRecieverUnitNickName )
		&& GET( m_wstrMsg )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_PARTY_BAN_USER_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_BAN_USER_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_iUnitUID )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_PARTY_BAN_USER_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		;    
}

SERIALIZE_DEFINE_GET( KEPM_PARTY_BAN_USER_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		;    
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_DUNGEON_SECRET_STAGE_ENTER_CHECK_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_REQUEST_UPDATE_PARTY_MEMBER_LIST_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_setPartyUIDList )
		;
}

SERIALIZE_DEFINE_GET( KEPM_SEND_UPDATE_PARTY_MEMBER_LIST_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_setPartyUIDList )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_UPDATE_PARTY_IN_PARTY_LIST_NOT, obj, ks )
{
	return PUT( m_kPartyInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_UPDATE_PARTY_IN_PARTY_LIST_NOT, obj, ks )
{
	return GET( m_kPartyInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
SERIALIZE_DEFINE_PUT( KEPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_sPartyFever )
		;
}

SERIALIZE_DEFINE_GET( KEPM_UPDATE_PARTY_FEVER_IN_PARTY_LIST_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_sPartyFever )
		;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_OPEN_PARTY_LIST_NOT, obj, ks )
{
	return PUT( m_kPartyInfo )
		&& PUT( m_vecUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_OPEN_PARTY_LIST_NOT, obj, ks )
{
	return GET( m_kPartyInfo )
		&& GET( m_vecUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CLOSE_PARTY_LIST_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CLOSE_PARTY_LIST_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_vecUpdatePartyUser )
		&& PUT( m_vecDeletePartyUser )
		;
}

SERIALIZE_DEFINE_GET( KEPM_UPDATE_PARTY_MEMBER_IN_PARTY_LIST_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_vecUpdatePartyUser )
		&& GET( m_vecDeletePartyUser )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_kPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_ADD_PARTY_MEMBER_IN_PARTY_LIST_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_kPartyUserInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_DEL_PARTY_MEMBER_IN_PARTY_LIST_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_iUnitUID )
		;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

SERIALIZE_DEFINE_PUT( KDBE_ATTENDANCE_CHECK_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ATTENDANCE_CHECK_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_INCREASE_WEB_POINT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrUserID )
		&& PUT( m_cPointType )
		&& PUT( m_iIncreasePoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INCREASE_WEB_POINT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrUserID )
		&& GET( m_cPointType )
		&& GET( m_iIncreasePoint )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_INCREASE_WEB_POINT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrUserID )
		&& PUT( m_cPointType )
		&& PUT( m_iIncreasePoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INCREASE_WEB_POINT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_wstrUserID )
		&& GET( m_cPointType )
		&& GET( m_iIncreasePoint )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_GET_WEB_POINT_REWARD_ACK, obj, ks )
{
	return PUT( m_vecWebPointReward )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_WEB_POINT_REWARD_ACK, obj, ks )
{
	return GET( m_vecWebPointReward )
		;
}

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef DEF_TRADE_BOARD
SERIALIZE_DEFINE_PUT( KERM_PERSONAL_SHOP_INFO_NOT, obj, ks )
{
	return PUT( m_iCode )
		&& PUT( m_iPSUID )
		&& PUT( m_wstrSellerNickName )
		//{{ 2011. 05. 03	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_iHostUnitUID )
#endif SERV_PSHOP_AGENCY
		//}}
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_PERSONAL_SHOP_INFO_NOT, obj, ks )
{
	return GET( m_iCode )
		&& GET( m_iPSUID )
		&& GET( m_wstrSellerNickName )
		//{{ 2011. 05. 03	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_iHostUnitUID )
#endif SERV_PSHOP_AGENCY
		//}}
		&& GET( m_vecItemInfo )
		;
}
#endif DEF_TRADE_BOARD



//{{ 2010. 03. 22  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_EXPAND_SKILL_NOTE_PAGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cExpandedMaxPageNum )
		&& PUT( m_cRollBackMaxPageNum )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_SKILL_NOTE_PAGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cExpandedMaxPageNum )
		&& GET( m_cRollBackMaxPageNum )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_EXPAND_SKILL_NOTE_PAGE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_cExpandedMaxPageNum )
		&& PUT( m_cRollBackMaxPageNum )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_SKILL_NOTE_PAGE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_cExpandedMaxPageNum )
		&& GET( m_cRollBackMaxPageNum )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_REG_SKILL_NOTE_MEMO_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cPageNum )
		&& PUT( m_iMemoID )
		&& PUT( m_vecInventorySlotInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_REG_SKILL_NOTE_MEMO_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cPageNum )
		&& GET( m_iMemoID )
		&& GET( m_vecInventorySlotInfo )
		;
}

#endif SERV_SKILL_NOTE
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 05. 12  최육사	서버 코드 정리
SERIALIZE_DEFINE_PUT( KERM_MY_USER_UNIT_INFO_TO_SERVER_REQ, obj, ks )
{
	return PUT( m_kRoomUserInfo )
		&& PUT( m_kDungeonUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_MY_USER_UNIT_INFO_TO_SERVER_REQ, obj, ks )
{
	return GET( m_kRoomUserInfo )
		&& GET( m_kDungeonUnitInfo )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW

SERIALIZE_DEFINE_PUT( KELG_UPDATE_CCU_INFO_NOT, obj, ks )
{
	return PUT( m_kCCUInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_UPDATE_CCU_INFO_NOT, obj, ks )
{
	return GET( m_kCCUInfo )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_UPDATE_CCU_INFO_NOT, obj, ks )
{
	return PUT( m_wstrRegDate )
		&& PUT( m_vecCCUInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_CCU_INFO_NOT, obj, ks )
{
	return GET( m_wstrRegDate )
		&& GET( m_vecCCUInfo )
		;
}

#endif SERV_CCU_NEW
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 06. 16  최육사	실시간 스크립트 갱신
#ifdef SERV_REALTIME_SCRIPT

SERIALIZE_DEFINE_PUT( KESR_SCRIPT_REFRESH_ORDER_NOT, obj, ks )
{
	return PUT( m_iOrderType )
		;
}

SERIALIZE_DEFINE_GET( KESR_SCRIPT_REFRESH_ORDER_NOT, obj, ks )
{
	return GET( m_iOrderType )
		;
}

#endif SERV_REALTIME_SCRIPT
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 06. 20  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD

SERIALIZE_DEFINE_PUT( KE_STATISTICS_INFO_NOT, obj, ks )
{
	return PUT( m_vecStatisticsInfo )
		;
}

SERIALIZE_DEFINE_GET( KE_STATISTICS_INFO_NOT, obj, ks )
{
	return GET( m_vecStatisticsInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_HENIR_RANKING_LOG_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		;
}

SERIALIZE_DEFINE_GET( KE_HENIR_RANKING_LOG_NOT, obj, ks )
{
	return GET( m_iRankingType )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_PVP_NOT, obj, ks )
{
	return PUT( m_iPVPChannelClass )
		&& PUT( m_iOwnerUserUID )
		&& PUT( m_wstrIP )
		&& PUT( m_nUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iUnitClass )
		&& PUT( m_iRoomUID )
		&& PUT( m_iPlayMode )
		&& PUT( m_bIsItemMode )
		&& PUT( m_bPublic )
		&& PUT( m_iNumMember )
		&& PUT( m_fPlayTimeLimit )
		&& PUT( m_wstrEndGameTime )
		&& PUT( m_iPlayTime )
		&& PUT( m_iVSPoint )
		&& PUT( m_iEXP )
		&& PUT( m_iNumMDKill )
		&& PUT( m_iNumDie )
		&& PUT( m_iResult )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_PVP_NOT, obj, ks )
{
	return GET( m_iPVPChannelClass )
		&& GET( m_iOwnerUserUID )
		&& GET( m_wstrIP )
		&& GET( m_nUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_iUnitClass )
		&& GET( m_iRoomUID )
		&& GET( m_iPlayMode )
		&& GET( m_bIsItemMode )
		&& GET( m_bPublic )
		&& GET( m_iNumMember )
		&& GET( m_fPlayTimeLimit )
		&& GET( m_wstrEndGameTime )
		&& GET( m_iPlayTime )
		&& GET( m_iVSPoint )
		&& GET( m_iEXP )
		&& GET( m_iNumMDKill )
		&& GET( m_iNumDie )
		&& GET( m_iResult )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_PVP_ROOM_NOT, obj, ks )
{
	return PUT( m_iPlayMode )
		&& PUT( m_bIsItemMode )
		&& PUT( m_sWorldID )
		&& PUT( m_iNumMember )
		&& PUT( m_iPlayTime )
		&& PUT( m_wstrEndGameTime )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_PVP_ROOM_NOT, obj, ks )
{
	return GET( m_iPlayMode )
		&& GET( m_bIsItemMode )
		&& GET( m_sWorldID )
		&& GET( m_iNumMember )
		&& GET( m_iPlayTime )
		&& GET( m_wstrEndGameTime )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_DUNGEON_ROOM_NOT, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_iDifficultyLevel )
		&& PUT( m_iIsChallenge )
		&& PUT( m_iWin )
		&& PUT( m_iLose )
		&& PUT( m_iStartNumMember )
		&& PUT( m_iEndNumMember )
		&& PUT( m_iTotalPlayTime )
		&& PUT( m_wstrEndGameTime )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_DUNGEON_ROOM_NOT, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_iDifficultyLevel )
		&& GET( m_iIsChallenge )
		&& GET( m_iWin )
		&& GET( m_iLose )
		&& GET( m_iStartNumMember )
		&& GET( m_iEndNumMember )
		&& GET( m_iTotalPlayTime )
		&& GET( m_wstrEndGameTime )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_TITLE_NOT, obj, ks )
{
	return PUT( m_iTitleID )
		&& PUT( m_wstrCharName )
		&& PUT( m_ucLevel )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_TITLE_NOT, obj, ks )
{
	return GET( m_iTitleID )
		&& GET( m_wstrCharName )
		&& GET( m_ucLevel )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_DUNGEON_NOT, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_nUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iDifficultyLevel )
		&& PUT( m_iIsChallenge )
		&& PUT( m_iUnitClass )
		&& PUT( m_iClear )
		&& PUT( m_iStartNumMember )
		&& PUT( m_iEndNumMember )
		&& PUT( m_ucLevel )
		&& PUT( m_iPlayTime )
		&& PUT( m_iEXP )
		&& PUT( m_iED )
		&& PUT( m_cComboRank )
		&& PUT( m_iComboScore )
		&& PUT( m_cTechnicalRank )
		&& PUT( m_iTechnicalScore )
		&& PUT( m_cTimeRank )
		&& PUT( m_cDamagedRank )
		&& PUT( m_iDamageNum )
		&& PUT( m_cTotalRank )
		&& PUT( m_iRessurectionStoneCount )
		&& PUT( m_iPassedStageCount )
		&& PUT( m_iPassedSubStageCount )
		//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_bIsWithPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 11. 8	최육사	던전 로그 DB
#ifdef SERV_DUNGEON_LOG_DB
		&& PUT( m_wstrRegDate )
#endif SERV_DUNGEON_LOG_DB
		//}}
		//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
		&& PUT( m_sNpcDieCount )
		&& PUT( m_iGivenDamage )
		&& PUT( m_iAttackDamage )
		&& PUT( m_sUsingSkillKind )
		&& PUT( m_iTotalEXP )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		&& PUT( m_iPetID )
#else
		&& PUT( m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		&& PUT( m_cPetEvoStep )
#endif SERV_ADD_DUNGEON_LOG_COLUMN
		//}}
		//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
		&& PUT( m_iDashCount )
#endif SERV_DUNGEON_DASH_LOG
		//}}
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& PUT( m_iBaseHP )
		&& PUT( m_iEndHP )
		&& PUT( m_iRecovHP )
		&& PUT( m_iContinue )
		&& PUT( m_iStartContinue )
		&& PUT( m_iContinueUseStage )
		&& PUT( m_iContinueUseSub )
		&& PUT( m_iFrame )
		&& PUT( m_wstrChannelIP )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 07. 11 김민성
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
		&& PUT( m_iUserUID )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
		//}}
		//{{ 2012. 11. 6	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_cAutoPartyPlay )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		&& PUT( m_iAutoPartyWaitTime )
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
		//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		&& PUT( m_iLeaveReason )
		&& PUT( m_cRoomState )
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		//}}
		//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
		&& PUT( m_bSkillSlotType )
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
		//}}
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_DUNGEON_NOT, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_nUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_iDifficultyLevel )
		&& GET( m_iIsChallenge )
		&& GET( m_iUnitClass )
		&& GET( m_iClear )
		&& GET( m_iStartNumMember )
		&& GET( m_iEndNumMember )
		&& GET( m_ucLevel )
		&& GET( m_iPlayTime )
		&& GET( m_iEXP )
		&& GET( m_iED )
		&& GET( m_cComboRank )
		&& GET( m_iComboScore )
		&& GET( m_cTechnicalRank )
		&& GET( m_iTechnicalScore )
		&& GET( m_cTimeRank )
		&& GET( m_cDamagedRank )
		&& GET( m_iDamageNum )
		&& GET( m_cTotalRank )
		&& GET( m_iRessurectionStoneCount )
		&& GET( m_iPassedStageCount )
		&& GET( m_iPassedSubStageCount )
		//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_bIsWithPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 11. 8	최육사	던전 로그 DB
#ifdef SERV_DUNGEON_LOG_DB
		&& GET( m_wstrRegDate )
#endif SERV_DUNGEON_LOG_DB
		//}}
		//{{ 2011. 03. 16	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN
		&& GET( m_sNpcDieCount )
		&& GET( m_iGivenDamage )
		&& GET( m_iAttackDamage )
		&& GET( m_sUsingSkillKind )
		&& GET( m_iTotalEXP )
#ifdef SERV_PETID_DATA_TYPE_CHANGE
		&& GET( m_iPetID )
#else
		&& GET( m_cPetID )
#endif SERV_PETID_DATA_TYPE_CHANGE
		&& GET( m_cPetEvoStep )
#endif SERV_ADD_DUNGEON_LOG_COLUMN
		//}}
		//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
		&& GET( m_iDashCount )
#endif SERV_DUNGEON_DASH_LOG
		//}}
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& GET( m_iBaseHP )
		&& GET( m_iEndHP )
		&& GET( m_iRecovHP )
		&& GET( m_iContinue )
		&& GET( m_iStartContinue )
		&& GET( m_iContinueUseStage )
		&& GET( m_iContinueUseSub )
		&& GET( m_iFrame )
		&& GET( m_wstrChannelIP )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 07. 11 김민성
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
		&& GET( m_iUserUID )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_3
		//}}
		//{{ 2012. 11. 6	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_cAutoPartyPlay )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		&& GET( m_iAutoPartyWaitTime )
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
		//{{ 2012. 12. 12	최육사	배틀필드 시스템
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		&& GET( m_iLeaveReason )
		&& GET( m_cRoomState )
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
		//}}
		//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
		&& GET( m_bSkillSlotType )
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_HENIR_RANKING_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_vecRankingInfo )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_HENIR_RANKING_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_vecRankingInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_PET_SUMMON_NOT, obj, ks )
{
	return PUT( m_wstrNickName )		
		&& PUT( m_ucLevel )
		&& PUT( m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		&& PUT( m_iPetID )
#else
		&& PUT( m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		&& PUT( m_cEvolutionStep )
		&& PUT( m_iIntimacy )
		&& PUT( m_sExtroversion )
		&& PUT( m_sEmotion )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_PET_SUMMON_NOT, obj, ks )
{
	return GET( m_wstrNickName )		
		&& GET( m_ucLevel )
		&& GET( m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		&& GET( m_iPetID )
#else
		&& GET( m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		&& GET( m_cEvolutionStep )
		&& GET( m_iIntimacy )
		&& GET( m_sExtroversion )
		&& GET( m_sEmotion )
		&& GET( m_wstrRegDate )
		;
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG

SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_SERVER_DISCONNECT_NOT, obj, ks )
{
	return PUT( m_cLogType )
		&& PUT( m_wstrSourceName )
		&& PUT( m_wstrDestinationName )
		&& PUT( m_wstrDestinationIP )
		&& PUT( m_wstrReason )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_SERVER_DISCONNECT_NOT, obj, ks )
{
	return GET( m_cLogType )
		&& GET( m_wstrSourceName )
		&& GET( m_wstrDestinationName )
		&& GET( m_wstrDestinationIP )
		&& GET( m_wstrReason )
		&& GET( m_wstrRegDate )
		;
}

#endif SERV_SERVER_DISCONNECT_LOG
//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT, obj, ks )
{
	return PUT( m_iQuestID )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT, obj, ks )
{
	return GET( m_iQuestID )
		&& GET( m_wstrRegDate )
		;
}
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG

SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_BILLING_PACKET_NOT, obj, ks )
{
	return PUT( m_cLogType )
		&& PUT( m_iUserUID )
		&& PUT( m_ulOrderNo )
		&& PUT( m_wstrRegDate )
		&& PUT( m_usEventID )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_BILLING_PACKET_NOT, obj, ks )
{
	return GET( m_cLogType )
		&& GET( m_iUserUID )
		&& GET( m_ulOrderNo )
		&& GET( m_wstrRegDate )
		&& GET( m_usEventID )
		;
}

#endif SERV_BILLING_PACKET_LOG
//}}

#endif SERV_STATISTICS_THREAD
//}}

//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHECK_ACCOUNT_BLOCK_LIST_REQ, obj, ks )
{
	return PUT( m_mapReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_ACCOUNT_BLOCK_LIST_REQ, obj, ks )
{
	return GET( m_mapReleaseTick )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK, obj, ks )
{
	return PUT( m_mapReleaseTick )
		&& PUT( m_mapAccountBlockList )
		//{{ 2011. 08. 08	최육사	머신ID 블럭 기능
#ifdef SERV_MACHINE_ID_BLOCK
		&& PUT( m_vecMachineIDBlockList )
#endif SERV_MACHINE_ID_BLOCK
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK, obj, ks )
{
	return GET( m_mapReleaseTick )
		&& GET( m_mapAccountBlockList )
		//{{ 2011. 08. 08	최육사	머신ID 블럭 기능
#ifdef SERV_MACHINE_ID_BLOCK
		&& GET( m_vecMachineIDBlockList )
#endif SERV_MACHINE_ID_BLOCK
		//}}
		;
}

#endif SERV_ACCOUNT_BLOCK
//}}


//////////////////////////////////////////////////////////////////////////
//{{2010. 06. 30  최육사	근성도 통계
#ifdef SERV_SPIRIT_STATISTICS

SERIALIZE_DEFINE_PUT( KDBE_SPIRIT_STATISTICS_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
		&& PUT( m_iSpiritPlayTime )
#else
		&& PUT( m_iDungeonPlayTime )
#endif SERV_FIELD_PLAY_TIME_LOG
		//}}
		&& PUT( m_iDurationTime )
		&& PUT( m_iUsedSpirit )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SPIRIT_STATISTICS_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
		&& GET( m_iSpiritPlayTime )
#else
		&& GET( m_iDungeonPlayTime )
#endif SERV_FIELD_PLAY_TIME_LOG
		//}}
		&& GET( m_iDurationTime )
		&& GET( m_iUsedSpirit )
		;
}

#endif SERV_SPIRIT_STATISTICS
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

SERIALIZE_DEFINE_PUT( KDBE_CREATE_PET_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kPetInfo )
#ifdef SERV_PERIOD_PET
		&& PUT( m_sPeriod )
#endif SERV_PERIOD_PET
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_PET_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kPetInfo )
#ifdef SERV_PERIOD_PET
		&& GET( m_sPeriod )
#endif SERV_PERIOD_PET
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CREATE_PET_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kPetInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_PET_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kPetInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_SUMMON_PET_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iBeforeSummonPetUID )
		&& PUT( m_iSummonPetUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_kItemPositionUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SUMMON_PET_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iBeforeSummonPetUID )
		&& GET( m_iSummonPetUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_kItemPositionUpdate )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_SUMMON_PET_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iBeforeSummonPetUID )
		&& PUT( m_iSummonPetUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_kItemPositionUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SUMMON_PET_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iBeforeSummonPetUID )
		&& GET( m_iSummonPetUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_kItemPositionUpdate )
		;
}


SERIALIZE_DEFINE_PUT( KERM_SUMMON_PET_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecPetInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_SUMMON_PET_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecPetInfo )
		;
}


SERIALIZE_DEFINE_PUT( KETR_SUMMON_PET_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_vecPetInfo )
		;
}

SERIALIZE_DEFINE_GET( KETR_SUMMON_PET_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_vecPetInfo )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_PET_ACTION_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cActionType )
		;
}

SERIALIZE_DEFINE_GET( KERM_PET_ACTION_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cActionType )
		;
}

SERIALIZE_DEFINE_PUT( KETR_PET_ACTION_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cActionType )
		;
}

SERIALIZE_DEFINE_GET( KETR_PET_ACTION_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_cActionType )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_DECREASE_PET_SATIETY_NOT, obj, ks )
{
	return PUT( m_cTeamNumMember )
		;
}

SERIALIZE_DEFINE_GET( KERM_DECREASE_PET_SATIETY_NOT, obj, ks )
{
	return GET( m_cTeamNumMember )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_PET_EVOLUTION_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kEvolutionPetInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_PET_EVOLUTION_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kEvolutionPetInfo )
		;
}


SERIALIZE_DEFINE_PUT( KETR_PET_EVOLUTION_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kEvolutionPetInfo )
		;
}

SERIALIZE_DEFINE_GET( KETR_PET_EVOLUTION_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_kEvolutionPetInfo )
		;
}

//{{ 2012. 02. 22	박세훈	펫 이름 변경권
#ifdef SERV_PET_CHANGE_NAME
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_CHANGE_PET_NAME_REQ, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kPetInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_PET_NAME_REQ, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iUnitUID )
		&& GET( m_kPetInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHANGE_PET_NAME_ACK, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iOK )
		&& PUT( m_kPetInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_PET_NAME_ACK, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iOK )
		&& GET( m_kPetInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
#endif SERV_PET_CHANGE_NAME
//}}


#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

SERIALIZE_DEFINE_PUT( KDBE_RESET_PERIOD_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapExpandPeriodItemList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_PERIOD_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )		
		&& GET( m_mapExpandPeriodItemList )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_RESET_PERIOD_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecItemPeriodInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_PERIOD_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecItemPeriodInfo )
		;
}

#endif SERV_RESET_PERIOD_EVENT

#ifdef SERV_KOG_STATISTICS
SERIALIZE_DEFINE_PUT( KDBE_CCU_AGENT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iDailyUniqueUser )
		&& PUT( m_iDailyResisterredUser )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CCU_AGENT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iDailyUniqueUser )
		&& GET( m_iDailyResisterredUser )
		;
}
#endif SERV_KOG_STATISTICS
//}}


//{{ 2010. 9. 8	최육사	네트워크 트래픽 유발 유저 모니터링
#ifdef SERV_TRAFFIC_USER_MONITORING

SERIALIZE_DEFINE_PUT( KELOG_INSERT_TRAFFIC_ABUSER_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapTrafficEventCount )
		;
}

SERIALIZE_DEFINE_GET( KELOG_INSERT_TRAFFIC_ABUSER_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapTrafficEventCount )
		;
}

#endif SERV_TRAFFIC_USER_MONITORING
//}}


//{{ 2010. 9. 29	최육사	ED 모니터링 치트
#ifdef SERV_ADMIN_SHOW_ED

SERIALIZE_DEFINE_PUT( KELG_ADMIN_SHOW_ED_CHEAT_REQ, obj, ks )
{
	return PUT( m_iSenderUserUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_SHOW_ED_CHEAT_REQ, obj, ks )
{
	return GET( m_iSenderUserUID )
		;
}


SERIALIZE_DEFINE_PUT( KELG_ADMIN_SHOW_ED_CHEAT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iSenderUserUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iED )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_SHOW_ED_CHEAT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iSenderUserUID )
		&& GET( m_wstrNickName )
		&& GET( m_iED )
		;
}

#endif SERV_ADMIN_SHOW_ED
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
SERIALIZE_DEFINE_PUT( KELG_REG_REJECTED_USER_NOT, obj ,ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_cRejectedReason )
#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
		&& PUT( m_cPeriodUAL )
		&& PUT( m_wstrBlockReason )
#endif // SERV_PERIOD_ACCOUNT_BLOCK
		;
}

SERIALIZE_DEFINE_GET( KELG_REG_REJECTED_USER_NOT, obj ,ks )
{
	return GET( m_iUserUID )
		&& GET( m_cRejectedReason )
#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
		&& GET( m_cPeriodUAL )
		&& GET( m_wstrBlockReason )
#endif // SERV_PERIOD_ACCOUNT_BLOCK
		;
}
#endif SERV_CN_SERVER_HACKING_CHECK
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
SERIALIZE_DEFINE_PUT( KE_DISCONNECT_SERVER_REPORT_NOT, obj, ks )
{
	return PUT( m_cType )
		;
}

SERIALIZE_DEFINE_GET( KE_DISCONNECT_SERVER_REPORT_NOT, obj, ks )
{
	return GET( m_cType )
		;
}
#endif SERV_MORNITORING
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER

SERIALIZE_DEFINE_PUT( KDBE_GET_MODULE_INFO_LIST_ACK, obj, ks )
{
	return PUT( m_vecModuleList )
		//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
		&& PUT( m_mapReleaseTick )
#endif SERV_HACKING_DLL_CHECK
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_MODULE_INFO_LIST_ACK, obj, ks )
{
	return GET( m_vecModuleList )
		//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
		&& GET( m_mapReleaseTick )
#endif SERV_HACKING_DLL_CHECK
		//}}
		;
}

#endif SERV_DLL_MANAGER
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG

SERIALIZE_DEFINE_PUT( KDBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrLoginIP )
		;
}

SERIALIZE_DEFINE_GET( KDBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_wstrUserID )
		&& GET( m_wstrLoginIP )
		;
}

#endif SERV_IP_ACCOUNT_LOG
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM

SERIALIZE_DEFINE_PUT( KE_LOCAL_LOG_ABUSER_MORNITORING_NOT, obj, ks )
{
	return PUT( m_vecAbuserEventLog )
		;
}

SERIALIZE_DEFINE_GET( KE_LOCAL_LOG_ABUSER_MORNITORING_NOT, obj, ks )
{
	return GET( m_vecAbuserEventLog )
		;
}

#endif SERV_AUTO_HACK_CHECK_GET_ITEM
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 11. 18	최육사	아이템 판매 차단
#ifdef SERV_AUTO_HACK_CHECK_SELL_ITEM

SERIALIZE_DEFINE_PUT( KERM_CHECK_SELL_ED_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHECK_SELL_ED_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_iQuantity )
		;
}

#endif SERV_AUTO_HACK_CHECK_SELL_ITEM
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT

SERIALIZE_DEFINE_PUT( KDBE_GET_DUNGEON_EVENT_INFO_ACK, obj, ks )
{
	return PUT( m_vecEventInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_DUNGEON_EVENT_INFO_ACK, obj, ks )
{
	return GET( m_vecEventInfo )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT, obj, ks )
{
	return PUT( m_iEventUID )
		&& PUT( m_iKillUnitUID )
		&& PUT( m_tNpcDieDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_EVENT_MONSTER_KILL_INFO_NOT, obj, ks )
{
	return GET( m_iEventUID )
		&& GET( m_iKillUnitUID )
		&& GET( m_tNpcDieDate )
		;
}

#endif SERV_TIME_DROP_MONSTER_EVENT
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 7	최육사	메일 전송 시스템
#ifdef SERV_MAIL_SYSTEM

SERIALIZE_DEFINE_PUT( KEMAIL_SEND_EMAIL_NOT, obj, ks )
{
	return PUT( m_vecRecvMailAddrList )
		&& PUT( m_wstrTitle )
		&& PUT( m_wstrDesc )
		&& PUT( m_vecAttachFileNameList )
		&& PUT( m_vecAttachFileFullPathList )
		;
}

SERIALIZE_DEFINE_GET( KEMAIL_SEND_EMAIL_NOT, obj, ks )
{
	return GET( m_vecRecvMailAddrList )
		&& GET( m_wstrTitle )
		&& GET( m_wstrDesc )
		&& GET( m_vecAttachFileNameList )
		&& GET( m_vecAttachFileFullPathList )
		;
}

#endif SERV_MAIL_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 8	최육사	적정 레벨 장비 통계
#ifdef SERV_APPROPRIATE_LEVEL_STAT

SERIALIZE_DEFINE_PUT( KELOG_APPROPRIATE_LEVEL_STATISTICS_NOT, obj, ks )
{
	return PUT( m_tDungeonStartTime )
		&& PUT( m_iDungeonID )
		&& PUT( m_cDifficultyLevel )
		&& PUT( m_vecUserAppropriateLevel )
		;    
}

SERIALIZE_DEFINE_GET( KELOG_APPROPRIATE_LEVEL_STATISTICS_NOT, obj, ks )
{
	return GET( m_tDungeonStartTime )
		&& GET( m_iDungeonID )
		&& GET( m_cDifficultyLevel )
		&& GET( m_vecUserAppropriateLevel )
		;    
}

#endif SERV_APPROPRIATE_LEVEL_STAT
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT

SERIALIZE_DEFINE_PUT( KDBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK, obj, ks )
{
	return PUT( m_vecTimeOpenRandomItemInfo )
		&& PUT( m_setGetRewardUserUIDList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_TIME_RANDOM_ITEM_EVENT_INFO_ACK, obj, ks )
{
	return GET( m_vecTimeOpenRandomItemInfo )
		&& GET( m_setGetRewardUserUIDList )
		;
}


SERIALIZE_DEFINE_PUT( KELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		&& PUT( m_iRandomItemID )
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}
		;
}

SERIALIZE_DEFINE_GET( KELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		&& GET( m_iRandomItemID )
#endif SERV_RANDOM_CUBE_GOLD_REWARD
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KELG_CHECK_TIME_RANDOM_ITEM_EVENT_ACK, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iEventItemID )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHECK_TIME_RANDOM_ITEM_EVENT_ACK, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iEventItemID )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT, obj, ks )
{
	return PUT( m_kDBUpdateInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_TIME_RANDOM_ITEM_EVENT_RESULT_NOT, obj, ks )
{
	return GET( m_kDBUpdateInfo )
		;
}

#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}


//{{ 2011. 01. 04	최육사	속성 부적
#ifdef SERV_ATTRIBUTE_CHARM

SERIALIZE_DEFINE_PUT( KDBE_ATTRIB_ATTACH_ITEM_REQ, obj ,ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_kAttribEnchantInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ATTRIB_ATTACH_ITEM_REQ, obj ,ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_kAttribEnchantInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_ATTRIB_ATTACH_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_kAttribEnchantInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ATTRIB_ATTACH_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_kAttribEnchantInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


SERIALIZE_DEFINE_PUT( KELOG_ATTRIB_ATTACH_ITEM_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iAttachItemID )
		&& PUT( m_iDestItemUID )
		&& PUT( m_kBeforeAttribEnchantInfo )
		&& PUT( m_kAfterAttribEnchantInfo )
		;
}

SERIALIZE_DEFINE_GET( KELOG_ATTRIB_ATTACH_ITEM_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iAttachItemID )
		&& GET( m_iDestItemUID )
		&& GET( m_kBeforeAttribEnchantInfo )
		&& GET( m_kAfterAttribEnchantInfo )
		;
}

#endif SERV_ATTRIBUTE_CHARM
//}}

//{{ 2011. 01. 13 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
SERIALIZE_DEFINE_PUT( KDBE_GET_CONCURRENT_USER_LOGIN_REQ, obj, ks  )
{
	return PUT( m_wstrID )
		&& PUT( m_wstrPW )
		;
};
SERIALIZE_DEFINE_GET( KDBE_GET_CONCURRENT_USER_LOGIN_REQ, obj, ks  )
{
	return GET( m_wstrID )
		&& GET( m_wstrPW )
		;
};

SERIALIZE_DEFINE_PUT( KDBE_GET_CONCURRENT_USER_LOGIN_ACK, obj, ks  )
{
	return PUT( m_iOK )
		&& PUT( m_iAuthLevel )
	;
};
SERIALIZE_DEFINE_GET( KDBE_GET_CONCURRENT_USER_LOGIN_ACK, obj, ks  )
{
	return GET( m_iOK )
		&& GET( m_iAuthLevel )
		;
};

SERIALIZE_DEFINE_PUT( KDBE_GET_GAME_SERVER_LIST_ACK, obj, ks  )
{
	return PUT( m_iServerCount )
		&& PUT( m_vecServerList )
		;
};
SERIALIZE_DEFINE_GET( KDBE_GET_GAME_SERVER_LIST_ACK, obj, ks  )
{
	return GET( m_iServerCount )
		&& GET( m_vecServerList )
		;
};

#endif SERV_CCU_MONITORING_TOOL
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 01. 13	최육사	캐릭터 로그
#ifdef SERV_CHAR_LOG

SERIALIZE_DEFINE_PUT( KELOG_USER_CHARACTER_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_bOnlyOneUnit )
		&& PUT( m_iServerUID )
		&& PUT( m_sLogType )
		&& PUT( m_ucLevel )
		&& PUT( m_cUnitClass )
		&& PUT( m_iDungeonID )
		&& PUT( m_iProductNo )
		&& PUT( m_iTotalDungeonPlayTime )
		&& PUT( m_iTotalPvpPlayTime )
		&& PUT( m_iDurationTime )
		&& PUT( m_wstrAccountCreateDate )
		&& PUT( m_wstrUnitCreateDate )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KELOG_USER_CHARACTER_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_bOnlyOneUnit )
		&& GET( m_iServerUID )
		&& GET( m_sLogType )
		&& GET( m_ucLevel )
		&& GET( m_cUnitClass )
		&& GET( m_iDungeonID )
		&& GET( m_iProductNo )
		&& GET( m_iTotalDungeonPlayTime )
		&& GET( m_iTotalPvpPlayTime )
		&& GET( m_iDurationTime )
		&& GET( m_wstrAccountCreateDate )
		&& GET( m_wstrUnitCreateDate )
		&& GET( m_wstrRegDate )
		;
}

#endif SERV_CHAR_LOG
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 01. 26	최육사	던전 클리어 타임 핵유저 체크
#ifdef SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK

SERIALIZE_DEFINE_PUT( KDBE_REG_DUNGEON_CLEAR_TIME_HACK_USER_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_REG_DUNGEON_CLEAR_TIME_HACK_USER_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_wstrRegDate )
		;
}

#endif SERV_DUNGEON_CLEAR_TIME_HACK_USER_CHECK
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 02. 08	최육사	캐쉬템 구매 로그
#ifdef SERV_BUY_CASH_ITEM_LOG
SERIALIZE_DEFINE_PUT( KELOG_BUY_CASH_ITEM_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iServerGroupID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_bIsFirstBuy )
		&& PUT( m_cBuyType )
		&& PUT( m_iProductNo )
		&& PUT( m_iBuyQuantity )
		&& PUT( m_iTotalSalePrice )
		&& PUT( m_wstrRegDate )
		//{{ 2011. 10. 11    김민성    캐쉬 아이템 선물 할 때 받는 유저의 정보 DB에 기록하기
#ifdef SERV_PRESENT_CASH_ITEM_ADD_LOG
		&& PUT( m_wstrReceiverUserID )
#endif SERV_PRESENT_CASH_ITEM_ADD_LOG
		//}} 
		;
}

SERIALIZE_DEFINE_GET( KELOG_BUY_CASH_ITEM_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iServerGroupID )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_bIsFirstBuy )
		&& GET( m_cBuyType )
		&& GET( m_iProductNo )
		&& GET( m_iBuyQuantity )
		&& GET( m_iTotalSalePrice )
		&& GET( m_wstrRegDate )
		//{{ 2011. 10. 11    김민성    캐쉬 아이템 선물 할 때 받는 유저의 정보 DB에 기록하기
#ifdef SERV_PRESENT_CASH_ITEM_ADD_LOG
		&& GET( m_wstrReceiverUserID )
#endif SERV_PRESENT_CASH_ITEM_ADD_LOG
		//}} 
		;
}
#endif SERV_BUY_CASH_ITEM_LOG
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
SERIALIZE_DEFINE_PUT( KELOG_CASH_PRODUCT_LIST_UPDATE_NOT, obj, ks )
{
	return PUT( m_vecCashProductList )
		;
}

SERIALIZE_DEFINE_GET( KELOG_CASH_PRODUCT_LIST_UPDATE_NOT, obj, ks )
{
	return GET( m_vecCashProductList )
		;
}
#endif SERV_CASH_ITEM_LIST
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK

SERIALIZE_DEFINE_PUT( KELG_TRADE_BLOCK_NOT, obj, ks )
{
	return PUT( m_bIsTradeBlock )
		;
}

SERIALIZE_DEFINE_GET( KELG_TRADE_BLOCK_NOT, obj, ks )
{
	return GET( m_bIsTradeBlock )
		;
}

#endif SERV_HACK_USER_TRADE_BLOCK
//}}


//{{ 2011. 03. 17	김민성	던전 플레이 로그 요청사항 
#ifdef SERV_DUNGEON_TEMP_LOG_DB
SERIALIZE_DEFINE_PUT( KDBE_DB_TEMP_LOG_DUNGEON_NOT, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iUserUID )
		&& PUT( m_iDifficulty )
		&& PUT( m_bIsChallenge )
		&& PUT( m_iUnitClass )
		&& PUT( m_bIsClear )
		&& PUT( m_iStartMember )
		&& PUT( m_iEndMember )
		&& PUT( m_ucLv )
		&& PUT( m_iPlayTime )
		&& PUT( m_cComboRank )
		&& PUT( m_iComboScore )
		&& PUT( m_cTechnicalRank )
		&& PUT( m_iTechnScore )
		&& PUT( m_cTimeRank )
		&& PUT( m_cDamageRank )
		&& PUT( m_cTotalRank )
		&& PUT( m_iPassStage )
		&& PUT( m_iPassSub )
		&& PUT( m_iNormKill )
		&& PUT( m_iLoEliteKill )
		&& PUT( m_iHiEliteKill )
		&& PUT( m_iMidBossKill )
		&& PUT( m_iBossKill )
		&& PUT( m_iAttkedDamage )
		&& PUT( m_iRecovHP )
		&& PUT( m_iBaseHP )
		&& PUT( m_iGivenDamage )
		&& PUT( m_iPartyDamage )
		&& PUT( m_iSkillUsedCount )
		&& PUT( m_wstrRegDate )
		&& PUT( m_bComeBackParty )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DB_TEMP_LOG_DUNGEON_NOT, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_iUserUID )
		&& GET( m_iDifficulty )
		&& GET( m_bIsChallenge )
		&& GET( m_iUnitClass )
		&& GET( m_bIsClear )
		&& GET( m_iStartMember )
		&& GET( m_iEndMember )
		&& GET( m_ucLv )
		&& GET( m_iPlayTime )
		&& GET( m_cComboRank )
		&& GET( m_iComboScore )
		&& GET( m_cTechnicalRank )
		&& GET( m_iTechnScore )
		&& GET( m_cTimeRank )
		&& GET( m_cDamageRank )
		&& GET( m_cTotalRank )
		&& GET( m_iPassStage )
		&& GET( m_iPassSub )
		&& GET( m_iNormKill )
		&& GET( m_iLoEliteKill )
		&& GET( m_iHiEliteKill )
		&& GET( m_iMidBossKill )
		&& GET( m_iBossKill )
		&& GET( m_iAttkedDamage )
		&& GET( m_iRecovHP )
		&& GET( m_iBaseHP )
		&& GET( m_iGivenDamage )
		&& GET( m_iPartyDamage )
		&& GET( m_iSkillUsedCount )
		&& GET( m_wstrRegDate )
		&& GET( m_bComeBackParty )
		;
}
#endif SERV_DUNGEON_TEMP_LOG_DB
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 04. 12	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY


SERIALIZE_DEFINE_PUT( KERM_LOAD_PSHOP_BOARD_INFO_NOT, obj, ks )
{
	return PUT( m_iGSUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_LOAD_PSHOP_BOARD_INFO_NOT, obj, ks )
{
	return GET( m_iGSUID )
		;
}


SERIALIZE_DEFINE_PUT( KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cPersonalShopType )
		&& PUT( m_wstrAgencyExpirationDate )
		&& PUT( m_wstrAgencyOpenDate )
		&& PUT( m_wstrPersonalShopName )
		&& PUT( m_vecSellItemInfo )
		&& PUT( m_bOnSale )
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cPersonalShopType )
		&& GET( m_wstrAgencyExpirationDate )
		&& GET( m_wstrAgencyOpenDate )
		&& GET( m_wstrPersonalShopName )
		&& GET( m_vecSellItemInfo )
		&& GET( m_bOnSale )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_LOAD_PSHOP_AGENCY_ACK, obj, ks )
{
	return PUT( m_vecOpenPShopAgency )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOAD_PSHOP_AGENCY_ACK, obj, ks )
{
	return GET( m_vecOpenPShopAgency )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_sAgencyPeriod )
		&& PUT( m_usEventID )
        && PUT( m_cShopType )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_PERIOD_PSHOP_AGENCY_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_sAgencyPeriod )
		&& GET( m_usEventID )
        && GET( m_cShopType )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_INSERT_PERIOD_PSHOP_AGENCY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrAgencyExpirationDate )
		&& PUT( m_usEventID )
        && PUT( m_cShopType )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_PERIOD_PSHOP_AGENCY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrAgencyExpirationDate )
		&& GET( m_usEventID )
        && GET( m_cShopType )
		;
}


SERIALIZE_DEFINE_PUT( KERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT, obj, ks )
{
	return PUT( m_wstrAgencyExpirationDate )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_PERIOD_PSHOP_AGENCY_NOT, obj, ks )
{
	return GET( m_wstrAgencyExpirationDate )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ, obj, ks )
{
	return PUT( m_iUserUID )
        && PUT( m_iUnitUID )
        && PUT( m_wstrNickName )
        && PUT( m_cPersonalShopType )
        && PUT( m_wstrAgencyExpirationDate )
        && PUT( m_wstrAgencyOpenDate )
        && PUT( m_wstrPersonalShopName )
        && PUT( m_vecSellItemInfo )
        && PUT( m_bOnSale )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHECK_MY_PSHOP_AGENCY_INFO_REQ, obj, ks )
{
    return GET( m_iUserUID )
        && GET( m_iUnitUID )
        && GET( m_wstrNickName )
        && GET( m_cPersonalShopType )
        && GET( m_wstrAgencyExpirationDate )
        && GET( m_wstrAgencyOpenDate )
        && GET( m_wstrPersonalShopName )
        && GET( m_vecSellItemInfo )
        && GET( m_bOnSale )
		;
}


SERIALIZE_DEFINE_PUT( KERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK, obj, ks )
{
	return PUT( m_iPShopAgencyUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHECK_MY_PSHOP_AGENCY_INFO_ACK, obj, ks )
{
	return GET( m_iPShopAgencyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_OPEN_PSHOP_AGENCY_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iED )
		&& PUT( m_wstrNickName )
		&& PUT( m_cPersonalShopType )
		&& PUT( m_wstrAgencyExpirationDate )
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_PSHOP_AGENCY_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iED )
		&& GET( m_wstrNickName )
		&& GET( m_cPersonalShopType )
		&& GET( m_wstrAgencyExpirationDate )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_wstrPersonalShopName )
		&& PUT( m_vecSellItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_wstrPersonalShopName )
		&& GET( m_vecSellItemInfo )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_wstrPersonalShopName )
		&& PUT( m_vecSellItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PREPARE_REG_PSHOP_AGENCY_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_wstrPersonalShopName )
		&& GET( m_vecSellItemInfo )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_STOP_SALE_PSHOP_AGENCY_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iReason )
		;
}

SERIALIZE_DEFINE_GET( KDBE_STOP_SALE_PSHOP_AGENCY_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iReason )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_STOP_SALE_PSHOP_AGENCY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iReason )
		;
}

SERIALIZE_DEFINE_GET( KDBE_STOP_SALE_PSHOP_AGENCY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iReason )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_BREAK_PSHOP_AGENCY_NOT, obj, ks )
{
	return PUT( m_iReason )
		&& PUT( m_iHostGSUID )
		&& PUT( m_iHostUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_BREAK_PSHOP_AGENCY_NOT, obj, ks )
{
	return GET( m_iReason )
		&& GET( m_iHostGSUID )
		&& GET( m_iHostUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_BUY_PSHOP_AGENCY_ITEM_REQ, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iHostUnitUID )
		&& PUT( m_iSellUnitEDIn )
		&& PUT( m_kSellItemDBUpdate )
		&& PUT( m_iPriceGSUID )
		&& PUT( m_iPriceUnitUID )
		&& PUT( m_iPriceUnitEDOUT )
		&& PUT( m_iPriceCommissionED )
		&& PUT( m_kPriceIntemInfoIN )
		&& PUT( m_kItemQuantityUpdateTemp )
		&& PUT( m_vecItemInfoTemp )
		&& PUT( m_iEDTemp )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BUY_PSHOP_AGENCY_ITEM_REQ, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iHostUnitUID )
		&& GET( m_iSellUnitEDIn )
		&& GET( m_kSellItemDBUpdate )
		&& GET( m_iPriceGSUID )
		&& GET( m_iPriceUnitUID )
		&& GET( m_iPriceUnitEDOUT )
		&& GET( m_iPriceCommissionED )
		&& GET( m_kPriceIntemInfoIN )
		&& GET( m_kItemQuantityUpdateTemp )
		&& GET( m_vecItemInfoTemp )
		&& GET( m_iEDTemp )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ, obj, ks )
{
	return PUT( m_vecPickUpItemList )
		&& PUT( m_mapPickUpItemInfo )
		&& PUT( m_iTotalAddED )
		;
}

SERIALIZE_DEFINE_GET( KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_REQ, obj, ks )
{
	return GET( m_vecPickUpItemList )
		&& GET( m_mapPickUpItemInfo )
		&& GET( m_iTotalAddED )
		;
}


SERIALIZE_DEFINE_PUT( KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return PUT( m_cShopType )
        && PUT( m_vecPickUpItemList )
		;
}

SERIALIZE_DEFINE_GET( KERM_PREPARE_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return GET( m_cShopType )
        && GET( m_vecPickUpItemList )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_vecPickUpItemList )
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& PUT( m_iSellItemTotalED )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_PICK_UP_FROM_PSHOP_AGENCY_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_vecPickUpItemList )
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& GET( m_iSellItemTotalED )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KDBE_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_vecPickUpItemList )
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& PUT( m_iSellItemTotalED )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		&& PUT( m_vecPickUpFailList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_vecPickUpItemList )
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& GET( m_iSellItemTotalED )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		&& GET( m_vecPickUpFailList )
		;
}


SERIALIZE_DEFINE_PUT( KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iTotalSellEDIn )
		&& PUT( m_vecPickUpSuccessItemList )
		&& PUT( m_vecSellItemInfo )
        && PUT( m_bRemainSellItem )
		;
}

SERIALIZE_DEFINE_GET( KERM_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iTotalSellEDIn )
		&& GET( m_vecPickUpSuccessItemList )
		&& GET( m_vecSellItemInfo )
        && GET( m_bRemainSellItem )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_vecSellItemInfo )
        && PUT( m_bRemainSellItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_vecSellItemInfo )
        && GET( m_bRemainSellItem )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapInsertItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_vecSellItemInfo )
        && PUT( m_bRemainSellItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_TO_INVENTORY_PICK_UP_FROM_PSHOP_AGENCY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapInsertItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_vecSellItemInfo )
        && GET( m_bRemainSellItem )
		;
}


#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
SERIALIZE_DEFINE_PUT( KEGB_GET_MISSION_INFO_REQ, obj, ks )
{
	return PUT( m_iServerUID )
		;
}
SERIALIZE_DEFINE_GET( KEGB_GET_MISSION_INFO_REQ, obj, ks )
{
	return GET( m_iServerUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_GET_MISSION_INFO_ACK, obj, ks )
{
	return PUT( m_bActive )
		&& PUT( m_iFlag )
		&& PUT( m_iWorldCrystalCount )
		//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& PUT( m_iWorldMaxCrystalCount )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
		&& PUT( m_iMissionTime )
		&& PUT( m_iDungeonPlayTime )
		//{{ 2012. 04. 24	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		&& PUT( m_mapDefenseOpenTime )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& PUT( m_usWorldBuffDurationTime )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
		;
}
SERIALIZE_DEFINE_GET( KEGB_GET_MISSION_INFO_ACK, obj, ks )
{
	return GET( m_bActive )
		&& GET( m_iFlag )
		&& GET( m_iWorldCrystalCount )
		//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& GET( m_iWorldMaxCrystalCount )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
		&& GET( m_iMissionTime )
		&& GET( m_iDungeonPlayTime )
		//{{ 2012. 04. 24	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		&& GET( m_mapDefenseOpenTime )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		//{{ 2013. 04. 15	박세훈	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& GET( m_usWorldBuffDurationTime )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEGB_UPDATE_WORLD_BUFF_NOT, obj, ks )
{
	return PUT( m_bActive )
		&& PUT( m_usSuccessWorldBuff )
		&& PUT( m_usWorldBuffDurationTime )
		;
}
SERIALIZE_DEFINE_GET( KEGB_UPDATE_WORLD_BUFF_NOT, obj, ks )
{
	return GET( m_bActive )
		&& GET( m_usSuccessWorldBuff )
		&& GET( m_usWorldBuffDurationTime )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INCREASE_DEFENCE_CRYSTAL_REQ, obj, ks )
{
	return PUT( m_wsrtStartTime )
		&& PUT( m_iProtectedCrystalCount )
		&& PUT( m_iTargetCrystal )
		;
}
SERIALIZE_DEFINE_GET( KDBE_INCREASE_DEFENCE_CRYSTAL_REQ, obj, ks )
{
	return GET( m_wsrtStartTime )
		&& GET( m_iProtectedCrystalCount )
		&& GET( m_iTargetCrystal )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_UPDATE_MISSION_START_TIME_NOT, obj, ks )
{
	return PUT( m_wsrtStartTime )
		;
}
SERIALIZE_DEFINE_GET( KEGB_UPDATE_MISSION_START_TIME_NOT, obj, ks )
{
	return GET( m_wsrtStartTime )
		;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
SERIALIZE_DEFINE_PUT( KDBE_GET_SECOND_SECURITY_INFO_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_mapComeBackRewardCondition )
		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		&& PUT( m_wstrID )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_SECOND_SECURITY_INFO_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_mapComeBackRewardCondition )
		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		&& GET( m_wstrID )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_SECOND_SECURITY_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bUseSecondPW )
		&& PUT( m_iFailedCount )
		&& PUT( m_wstrSecondPW )
		&& PUT( m_wstrLastAuthDate )
//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& PUT( m_bIsFirst )
		&& PUT( m_bIsComeBackUser )
		&& PUT( m_iRewardStep )
		&& PUT( m_wstrLastConnectDate )
		&& PUT( m_wstrComeBackBuffEndDate )
#endif SERV_COME_BACK_USER_REWARD
//}} 
//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		&& PUT( m_iBF_Team )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
//}}

//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		&& PUT( m_mmapDailyGiftBoxList )
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}
	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		&& PUT( m_wstrWinterVacationEventRegDate )
		&& PUT( m_iWinterVacationEventCount )
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}
	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	&& PUT( m_vecFirstSelectUnitReward )
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	//}
		;
}
SERIALIZE_DEFINE_GET( KDBE_GET_SECOND_SECURITY_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bUseSecondPW )
		&& GET( m_iFailedCount )
		&& GET( m_wstrSecondPW )
		&& GET( m_wstrLastAuthDate )
//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& GET( m_bIsFirst )
		&& GET( m_bIsComeBackUser )
		&& GET( m_iRewardStep )
		&& GET( m_wstrLastConnectDate )
		&& GET( m_wstrComeBackBuffEndDate )
#endif SERV_COME_BACK_USER_REWARD
//}} 
//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		&& GET( m_iBF_Team )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
//}}

		//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		&& GET( m_mmapDailyGiftBoxList )
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		&& GET( m_wstrWinterVacationEventRegDate )
		&& GET( m_iWinterVacationEventCount )
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		&& GET( m_vecFirstSelectUnitReward )
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CREATE_SECOND_SECURITY_REQ, obj, ks )
{
	return PUT( m_iUserID )
		&& PUT( m_wstrSecondPW )
		;
}
SERIALIZE_DEFINE_GET( KDBE_CREATE_SECOND_SECURITY_REQ, obj, ks )
{
	return GET( m_iUserID )
		&& GET( m_wstrSecondPW )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CREATE_SECOND_SECURITY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrSecondPW )
		;
}
SERIALIZE_DEFINE_GET( KDBE_CREATE_SECOND_SECURITY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrSecondPW )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SEUCCESS_SECOND_SECURITY_REQ, obj, ks )
{
	return PUT( m_iUserID )
		&& PUT( m_wstrCurrTime )
		;
}
SERIALIZE_DEFINE_GET( KDBE_SEUCCESS_SECOND_SECURITY_REQ, obj, ks )
{
	return GET( m_iUserID )
		&& GET( m_wstrCurrTime )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SEUCCESS_SECOND_SECURITY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrCurrTime )
		;
}
SERIALIZE_DEFINE_GET( KDBE_SEUCCESS_SECOND_SECURITY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrCurrTime )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_SECOND_SECURITY_PW_REQ, obj, ks )
{
	return PUT( m_iUserID )
		&& PUT( m_wstrOldSecondPW )
		&& PUT( m_wstrNewSecondPW )
		;
}
SERIALIZE_DEFINE_GET( KDBE_CHANGE_SECOND_SECURITY_PW_REQ, obj, ks )
{
	return GET( m_iUserID )
		&& GET( m_wstrOldSecondPW )
		&& GET( m_wstrNewSecondPW )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_SECURITY_TYPE_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iSecurityType )
		;
}
SERIALIZE_DEFINE_GET( KDBE_UPDATE_SECURITY_TYPE_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iSecurityType )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_FIRST_WRITE_SECURITY_TYPE_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iAge )
		&& PUT( m_iSecurityType )
		&& PUT( m_wstrCurrTime )
		;
}
SERIALIZE_DEFINE_GET( KDBE_FIRST_WRITE_SECURITY_TYPE_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iAge )
		&& GET( m_iSecurityType )
		&& GET( m_wstrCurrTime )
		;
}
#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 05. 13	최육사	대리 상인 좀비 유저 버그 해결
#ifdef SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
SERIALIZE_DEFINE_PUT( KERM_CHECK_ZOMBIE_USER_REQ, obj, ks )
{
	return PUT( m_iRoomUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHECK_ZOMBIE_USER_REQ, obj, ks )
{
	return GET( m_iRoomUID )
		;
}


SERIALIZE_DEFINE_PUT( KERM_CHECK_ZOMBIE_USER_ACK, obj, ks )
{
	return PUT( m_cZombieCheckType )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHECK_ZOMBIE_USER_ACK, obj, ks )
{
	return GET( m_cZombieCheckType )
		;
}
#endif SERV_PSHOP_AGENCY_ZOMBIE_USER_BUG_FIX
//}}

//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
SERIALIZE_DEFINE_PUT( KEPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_bCheck )
		;
}
SERIALIZE_DEFINE_GET( KEPM_PARTY_CHANGE_CHECK_MIN_LEVEL_REQ, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_bCheck )
		;
}
#endif SERV_DUNGEON_DIFFICULTY_REVISION
//}}

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
SERIALIZE_DEFINE_PUT( KDBE_WRITE_COME_BACK_REWARD_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iRewardStep )
		&& PUT( m_wstrComeBackBuffEndDate )
		;
}
SERIALIZE_DEFINE_GET( KDBE_WRITE_COME_BACK_REWARD_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iRewardStep )
		&& GET( m_wstrComeBackBuffEndDate )
		;
}

SERIALIZE_DEFINE_PUT( KEPM_CHANGE_COME_BACK_BUFF_NOT, obj, ks )
{
	return PUT( m_bIsBuffFinish )
		&& PUT( m_wstrComeBackBuffEnd )
		;
}
SERIALIZE_DEFINE_GET( KEPM_CHANGE_COME_BACK_BUFF_NOT, obj, ks )
{
	return GET( m_bIsBuffFinish )
		&& GET( m_wstrComeBackBuffEnd )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_FIRST_WRITE_COME_BACK_INFO_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iAge )
		&& PUT( m_iUnitUID )
		&& PUT( m_iUnitClass )
		&& PUT( m_iUnitLevel )
		&& PUT( m_bTodayFirst )
		&& PUT( m_iRewardLevel )
		&& PUT( m_wsrtRegDate )
		;
}
SERIALIZE_DEFINE_GET( KDBE_FIRST_WRITE_COME_BACK_INFO_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iAge )
		&& GET( m_iUnitUID )
		&& GET( m_iUnitClass )
		&& GET( m_iUnitLevel )
		&& GET( m_bTodayFirst )
		&& GET( m_iRewardLevel )
		&& GET( m_wsrtRegDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WRITE_COME_BACK_LOGOUT_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_wsrtLogOutDate )
		;
}
SERIALIZE_DEFINE_GET( KDBE_WRITE_COME_BACK_LOGOUT_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_wsrtLogOutDate )
		;
}
#endif SERV_COME_BACK_USER_REWARD
//}} 


//{{ 2011. 06. 13	최육사	스테이지 로그
#ifdef SERV_DUNGEON_STAGE_LOG
SERIALIZE_DEFINE_PUT( KELOG_DUNGEON_STAGE_LOG_NOT, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_iStageID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cLevel )
		&& PUT( m_cDifficulty )
		&& PUT( m_bIsChallenge )
		&& PUT( m_cUnitClass )
		&& PUT( m_iStagePlayTime )
		&& PUT( m_iStageDamage )
		&& PUT( m_iResurrectionCount )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KELOG_DUNGEON_STAGE_LOG_NOT, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_iStageID )
		&& GET( m_iUnitUID )
		&& GET( m_cLevel )
		&& GET( m_cDifficulty )
		&& GET( m_bIsChallenge )
		&& GET( m_cUnitClass )
		&& GET( m_iStagePlayTime )
		&& GET( m_iStageDamage )
		&& GET( m_iResurrectionCount )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_DUNGEON_STAGE_LOG
//}}


//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM

SERIALIZE_DEFINE_PUT( KEGB_REQUEST_MATCH_MAKING_REQ, obj, ks )
{
	return PUT( m_bRegFrontOfList )
		&& PUT( m_cPvpPlayerCount )
		&& PUT( m_cPvpGameType )
		&& PUT( m_vecMatchUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REQUEST_MATCH_MAKING_REQ, obj, ks )
{
	return GET( m_bRegFrontOfList )
		&& GET( m_cPvpPlayerCount )
		&& GET( m_cPvpGameType )
		&& GET( m_vecMatchUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_CANCEL_MATCH_MAKING_REQ, obj, ks )
{
	return PUT( m_iMatchUID )
		&& PUT( m_iPartyUID )
		&& PUT( m_iReason )
		&& PUT( m_bSendBySystem )
		;
}

SERIALIZE_DEFINE_GET( KEGB_CANCEL_MATCH_MAKING_REQ, obj, ks )
{
	return GET( m_iMatchUID )
		&& GET( m_iPartyUID )
		&& GET( m_iReason )
		&& GET( m_bSendBySystem )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_REMATCH_MAKING_USER_NOT, obj, ks )
{
	return PUT( m_cPvpPlayerCount )
		&& PUT( m_cPvpGameType )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REMATCH_MAKING_USER_NOT, obj, ks )
{
	return GET( m_cPvpPlayerCount )
		&& GET( m_cPvpGameType )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ, obj, ks )
{
	return PUT( m_bRegFrontOfList )
		&& PUT( m_cPvpPlayerCount )
		&& PUT( m_cPvpGameType )
		;
}

SERIALIZE_DEFINE_GET( KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_REQ, obj, ks )
{
	return GET( m_bRegFrontOfList )
		&& GET( m_cPvpPlayerCount )
		&& GET( m_cPvpGameType )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bRegFrontOfList )
		&& PUT( m_cPvpPlayerCount )
		&& PUT( m_cPvpGameType )
		&& PUT( m_vecMatchUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_GET_PARTY_INFO_FOR_MATCH_MAKING_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bRegFrontOfList )
		&& GET( m_cPvpPlayerCount )
		&& GET( m_cPvpGameType )
		&& GET( m_vecMatchUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK, obj, ks )
{
	return PUT( m_kRoomUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_GET_ROOM_USER_INFO_FOR_MATCH_ACK, obj, ks )
{
	return GET( m_kRoomUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, obj, ks )
{
	return PUT( m_iMatchUID )
		&& PUT( m_cMatchType )
		&& PUT( m_cPvpNpcType )
		&& PUT( m_kRoomInfo )
		&& PUT( m_vecRedTeam )
		&& PUT( m_vecBlueTeam )
		&& PUT( m_mapMatchWaitTime )
		&& PUT( m_mapPvpNpcInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_OPEN_PVP_ROOM_FOR_MATCH_REQ, obj, ks )
{
	return GET( m_iMatchUID )
		&& GET( m_cMatchType )
		&& GET( m_cPvpNpcType )
		&& GET( m_kRoomInfo )		
		&& GET( m_vecRedTeam )
		&& GET( m_vecBlueTeam )
		&& GET( m_mapMatchWaitTime )
		&& GET( m_mapPvpNpcInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT, obj, ks )
{
	return PUT( m_kPvpUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_PVP_PLAY_UNIT_INFO_NOT, obj, ks )
{
	return GET( m_kPvpUnitInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_PVP_MATCH_RESULT_LOG_NOT, obj, ks )
{
	return PUT( m_cMatchType )
		&& PUT( m_cNpcPvpType )
		&& PUT( m_wstrRegDate )
		&& PUT( m_iPlayTime )
		&& PUT( m_vecPvpMatchUserLog )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PVP_MATCH_RESULT_LOG_NOT, obj, ks )
{
	return GET( m_cMatchType )
		&& GET( m_cNpcPvpType )
		&& GET( m_wstrRegDate )
		&& GET( m_iPlayTime )
		&& GET( m_vecPvpMatchUserLog )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT, obj, ks )
{
	return PUT( m_cType )
		;
}

SERIALIZE_DEFINE_GET( KEGB_MATCH_MAKING_ADMIN_COMMAND_NOT, obj, ks )
{
	return GET( m_cType )
		;
}

#endif SERV_PVP_NEW_SYSTEM
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 08. 03	최육사	대전 강제 종료에 대한 예외처리
#ifdef SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
SERIALIZE_DEFINE_PUT( KDBE_QUIT_USER_PVP_RESULT_UPDATE_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iOfficialMatchCnt )
		&& PUT( m_iRating )
		&& PUT( m_iMaxRating )
		&& PUT( m_iRPoint )
		&& PUT( m_iAPoint )
		&& PUT( m_bIsWinBeforeMatch )
		&& PUT( m_cEmblemEnum )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_fKFactor )
#endif SERV_2012_PVP_SEASON2
		//}}

		&& PUT( m_iWin )
		&& PUT( m_iLose )
		;
}

SERIALIZE_DEFINE_GET( KDBE_QUIT_USER_PVP_RESULT_UPDATE_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iOfficialMatchCnt )
		&& GET( m_iRating )
		&& GET( m_iMaxRating )
		&& GET( m_iRPoint )
		&& GET( m_iAPoint )
		&& GET( m_bIsWinBeforeMatch )
		&& GET( m_cEmblemEnum )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_fKFactor )
#endif SERV_2012_PVP_SEASON2
		//}}

		&& GET( m_iWin )
		&& GET( m_iLose )
		;
}
#endif SERV_CLIENT_QUIT_PVP_BUG_PLAY_FIX
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
SERIALIZE_DEFINE_PUT( KDBE_RETAINING_SELECT_REWARD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_kItemQuantityUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RETAINING_SELECT_REWARD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_kItemQuantityUpdate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RETAINING_SELECT_REWARD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RETAINING_SELECT_REWARD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_RETAINING_USER_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iItemID )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_RETAINING_USER_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iItemID )
		&& GET( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHECK_RETAINING_USER_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_RETAINING_USER_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHECK_RETAINING_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_RETAINING_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_wstrDate )
		;
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2011. 08. 09  김민성 (2011.08.11) 특정일 이후 생성한 계정에 대하여 신규케릭터 생성 시 아이템 지급 이벤트
#ifdef SERV_NEW_CREATE_CHAR_EVENT
SERIALIZE_DEFINE_PUT( KDBE_GAME_CREATE_UNIT_REQ, obj, ks )
{
	return PUT( m_wstrNickName )
		&& PUT( m_iClass )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GAME_CREATE_UNIT_REQ, obj, ks )
{
	return GET( m_wstrNickName )
		&& GET( m_iClass )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_NEW_CREATE_CHAR_EVENT
//}}

//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
SERIALIZE_DEFINE_PUT( KSOAP_GET_TOONILAND_ID_REQ, obj, ks )
{
	return PUT( m_wstrNexonID )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_GET_TOONILAND_ID_REQ, obj, ks )
{
	return GET( m_wstrNexonID )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_GET_TOONILAND_ID_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrChannelingUID )
		&& PUT( m_wstrToonilandID )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_GET_TOONILAND_ID_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrChannelingUID )
		&& GET( m_wstrToonilandID )
		;
}
#endif SERV_CHANNELING_SOAP
//}}

//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
SERIALIZE_DEFINE_PUT( KEGB_TODAY_RANDOM_QUEST_ACK, obj, ks )
{
	return PUT( m_mapTodayRandomQuest )
		;
};
SERIALIZE_DEFINE_GET( KEGB_TODAY_RANDOM_QUEST_ACK, obj, ks )
{
	return GET( m_mapTodayRandomQuest )
		;
};
#endif SERV_RANDOM_DAY_QUEST
//}}

//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
SERIALIZE_DEFINE_PUT( KDBE_UPDATE_CHANNEL_RANDOMKEY_REQ, obj, ks )
{
	return PUT( m_wstrUserID )
		;
};
SERIALIZE_DEFINE_GET( KDBE_UPDATE_CHANNEL_RANDOMKEY_REQ, obj, ks )
{
	return GET( m_wstrUserID )
		;
};

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrUserID )
		&& PUT( m_iRandomKey )
		;
};
SERIALIZE_DEFINE_GET( KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrUserID )
		&& GET( m_iRandomKey )
		;
};
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{ 2011. 09. 19  김민성	칭호 획득 조건 추가
#ifdef SERV_ADD_TITLE_CONDITION
SERIALIZE_DEFINE_PUT( KERM_USER_UNIT_DIE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_fReBirthTime )
		&& PUT( m_VP )
		&& PUT( m_EXP )
		&& PUT( m_ucDieReason )
		;
};
SERIALIZE_DEFINE_GET( KERM_USER_UNIT_DIE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_fReBirthTime )
		&& GET( m_VP )
		&& GET( m_EXP )
		&& GET( m_ucDieReason )
		;
};
#endif SERV_ADD_TITLE_CONDITION
//}}

//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
SERIALIZE_DEFINE_PUT( KERM_FIELD_USER_LIST_MONITORING_MODE_REQ, obj, ks )
{
	return PUT( m_uiViewPage )
		&& PUT( m_mapUnit_ED )
		;
};
SERIALIZE_DEFINE_GET( KERM_FIELD_USER_LIST_MONITORING_MODE_REQ, obj, ks )
{
	return GET( m_uiViewPage )
		&& GET( m_mapUnit_ED )
		;
};

SERIALIZE_DEFINE_PUT( KERM_SQUARE_UNIT_REFRESH_ED_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iED )
		;
};
SERIALIZE_DEFINE_GET( KERM_SQUARE_UNIT_REFRESH_ED_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iED )
		;
};
#endif SERV_ED_MONITORING_IN_GAME
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 10. 13	최육사	아이템 사용 DB 업데이트 수정
#ifdef SERV_USE_ITEM_DB_UPDATE_FIX
SERIALIZE_DEFINE_PUT( KDBE_USE_ITEM_IN_INVENTORY_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iUsedItemID )
#ifdef SERV_GOLD_TICKET
		&& PUT( m_iItemUID )
#endif //SERV_GOLD_TICKET
		&& PUT( m_iWarpPointMapID )
		//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_iED )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
		&& PUT( m_iTempCode )
//#endif SERV_PET_AUTO_LOOTING
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_USE_ITEM_IN_INVENTORY_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iUsedItemID )
#ifdef SERV_GOLD_TICKET
		&& GET( m_iItemUID )
#endif //SERV_GOLD_TICKET
		&& GET( m_iWarpPointMapID )
		//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_iED )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
		&& GET( m_iTempCode )
//#endif SERV_PET_AUTO_LOOTING
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KDBE_USE_ITEM_IN_INVENTORY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUsedItemID )
#ifdef SERV_GOLD_TICKET
		&& PUT( m_iItemUID )
#endif //SERV_GOLD_TICKET
		&& PUT( m_iWarpPointMapID )
		//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_iED )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
		&& PUT( m_iTempCode )
//#endif SERV_PET_AUTO_LOOTING
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_USE_ITEM_IN_INVENTORY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUsedItemID )
#ifdef SERV_GOLD_TICKET
		&& GET( m_iItemUID )
#endif //SERV_GOLD_TICKET
		&& GET( m_iWarpPointMapID )
		//{{ 2012. 10. 25	박세훈	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_iED )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
//#ifdef SERV_PET_AUTO_LOOTING
		&& GET( m_iTempCode )
//#endif SERV_PET_AUTO_LOOTING
		//}}
		;
}
#endif SERV_USE_ITEM_DB_UPDATE_FIX
//}}

//{{ 2011.11.02     김민성    서버에서 SN 발급 및 유효성 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
SERIALIZE_DEFINE_PUT( KDBE_CHECK_SERVER_SN_REQ, obj, ks )
{
	return PUT( m_iServerSN )
		&& PUT( m_strMachineID )
		&& PUT( m_wstrID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_SERVER_SN_REQ, obj, ks )
{
	return GET( m_iServerSN )
		&& GET( m_strMachineID )
		&& GET( m_wstrID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CREATE_SERVER_SN_REQ, obj, ks )
{
	return PUT( m_strMachineID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_SERVER_SN_REQ, obj, ks )
{
	return GET( m_strMachineID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CREATE_SERVER_SN_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iServerSN )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_SERVER_SN_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iServerSN )
		;
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK
//}}

//{{  2011.11.08     김민성    강화 로그
#ifdef SERV_ENCHANT_ITEM_LOG
SERIALIZE_DEFINE_PUT( KELOG_HIGH_ENCHANT_ITEM_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_iBeforeLevel )
		&& PUT( m_iAfterLevel )
		&& PUT( m_bSupportMaterial )
		&& PUT( m_iEnchantResult )
		&& PUT( m_wstrTime )
		;
}

SERIALIZE_DEFINE_GET( KELOG_HIGH_ENCHANT_ITEM_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_iBeforeLevel )
		&& GET( m_iAfterLevel )
		&& GET( m_bSupportMaterial )
		&& GET( m_iEnchantResult )
		&& GET( m_wstrTime )
		;
}
#endif SERV_ENCHANT_ITEM_LOG
//}}

//{{  2011.11.24     김민성    캐릭터 접속 로그
#ifdef SERV_CHAR_CONNECT_LOG
SERIALIZE_DEFINE_PUT( KDBE_UNIT_CONNECT_LOG_NOT, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iUnitClass )
		&& PUT( m_iExp )
		&& PUT( m_iLevel )
		&& PUT( m_iPlayTime )
		&& PUT( m_bPcBang )
		&& PUT( m_cAuthLevel )
		&& PUT( m_iLoginLevel )
		&& PUT( m_wstrName )
		&& PUT( m_wstrLogin )
		&& PUT( m_wstrLogout )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UNIT_CONNECT_LOG_NOT, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_iUnitUID )
		&& GET( m_iUnitClass )
		&& GET( m_iExp )
		&& GET( m_iLevel )
		&& GET( m_iPlayTime )
		&& GET( m_bPcBang )
		&& GET( m_cAuthLevel )
		&& GET( m_iLoginLevel )
		&& GET( m_wstrName )
		&& GET( m_wstrLogin )
		&& GET( m_wstrLogout )
		;
}
#endif SERV_CHAR_CONNECT_LOG
//}}

//{{ 2011. 11. 21  김민성	전직 변경 아이템
#ifdef SERV_UNIT_CLASS_CHANGE_ITEM
SERIALIZE_DEFINE_PUT( KDBE_BUY_UNIT_CLASS_CHANGE_REQ, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iNewUnitClass )
		&& PUT( m_iSPoint )
		&& PUT( m_iCSPoint )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& PUT( m_iDefaultSkillID1 )
		&& PUT( m_iDefaultSkillID2 )
		&& PUT( m_iDefaultSkillID3 )
		&& PUT( m_iDefaultSkillID4 )
		&& PUT( m_iDefaultSkillID5 )
		&& PUT( m_iDefaultSkillID6 )
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*	
		&& PUT( m_iDefaultSkillID )
		&& PUT( m_iDefaultSkillID2 )
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		&& PUT( m_mapChangeSkill )
		
#ifdef SERV_REFORM_SKILL_NOTE
#else // SERV_REFORM_SKILL_NOTE
		&& PUT( m_mapChangeMemo )
#endif // SERV_REFORM_SKILL_NOTE

		
		&& PUT( m_mapChangeItem )
		&& PUT( m_mapChangeCompleteQuest )
		&& PUT( m_mapChangeInProgressQuest )
		&& PUT( m_vecDeleteCompleteQuest )
		&& PUT( m_vecDeleteInProgressQuest )
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		&& PUT( m_bUnlimitedSecondJobItem )
		&& PUT( m_iOldUnitClass )
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
		;
}

SERIALIZE_DEFINE_GET( KDBE_BUY_UNIT_CLASS_CHANGE_REQ, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iNewUnitClass )
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& GET( m_iDefaultSkillID1 )
		&& GET( m_iDefaultSkillID2 )
		&& GET( m_iDefaultSkillID3 )
		&& GET( m_iDefaultSkillID4 )
		&& GET( m_iDefaultSkillID5 )
		&& GET( m_iDefaultSkillID6 )
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
		&& GET( m_iDefaultSkillID )
		&& GET( m_iDefaultSkillID2 )
*/	
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		&& GET( m_mapChangeSkill )
		
#ifdef SERV_REFORM_SKILL_NOTE
#else // SERV_REFORM_SKILL_NOTE
		&& GET( m_mapChangeMemo )
#endif // SERV_REFORM_SKILL_NOTE

		&& GET( m_mapChangeItem )
		&& GET( m_mapChangeCompleteQuest )
		&& GET( m_mapChangeInProgressQuest )
		&& GET( m_vecDeleteCompleteQuest )
		&& GET( m_vecDeleteInProgressQuest )
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
		&& GET( m_bUnlimitedSecondJobItem )
		&& GET( m_iOldUnitClass )
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB
		;
}
#endif SERV_UNIT_CLASS_CHANGE_ITEM
//}}

//{{ 2011. 12. 02  김민성	AP 구매 로그 수치
#ifdef SERV_BUY_AP_ITEM_LOG
SERIALIZE_DEFINE_PUT( KDBE_BUY_AP_ITEM_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemID )
		&& PUT( m_iQuantity )
		&& PUT( m_iBeforAP )
		&& PUT( m_iAfterAP )
		&& PUT( m_wstrBuyDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BUY_AP_ITEM_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemID )
		&& GET( m_iQuantity )
		&& GET( m_iBeforAP )
		&& GET( m_iAfterAP )
		&& GET( m_wstrBuyDate )
		;
}
#endif SERV_BUY_AP_ITEM_LOG
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
SERIALIZE_DEFINE_PUT( KDBE_UPDATE_PAKCET_MORNITORING_LOG_NOT, obj, ks )
{
	return PUT( m_vecGSUserPacketLatencyInfo )
		&& PUT( m_vecLoginProxyPacketCountInfo )
		&& PUT( m_vecCnProxyPacketCountInfo )
		&& PUT( m_vecGlobalProxyPacketCountInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_PAKCET_MORNITORING_LOG_NOT, obj, ks )
{
	return GET( m_vecGSUserPacketLatencyInfo )
		&& GET( m_vecLoginProxyPacketCountInfo )
		&& GET( m_vecCnProxyPacketCountInfo )
		&& GET( m_vecGlobalProxyPacketCountInfo )
		;
}
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}

//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
SERIALIZE_DEFINE_PUT( KDBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kAuthenticateReq )
		&& PUT( m_kNexonAccountInfo )
		&& PUT( m_iServerSN )
		&& PUT( m_strMachineID )
		//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
		&& PUT( m_iMonitoringFlag )
#endif SERV_SERIAL_NUMBER_MORNITORING
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kAuthenticateReq )
		&& GET( m_kNexonAccountInfo )
		&& GET( m_iServerSN )
		&& GET( m_strMachineID )
		//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
		&& GET( m_iMonitoringFlag )
#endif SERV_SERIAL_NUMBER_MORNITORING
		//}}
		;
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
//}}

//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
SERIALIZE_DEFINE_PUT( KELG_CHECK_MODULE_INFO_UPDATE_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapModuleName )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHECK_MODULE_INFO_UPDATE_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapModuleName )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHECK_MODULE_INFO_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iServerSN )
		&& PUT( m_vecMonitoringDLL )
		&& PUT( m_vecBlackListDLL )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_MODULE_INFO_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iServerSN )
		&& GET( m_vecMonitoringDLL )
		&& GET( m_vecBlackListDLL )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_HACKING_MODULE_LIST_REQ, obj, ks )
{
	return PUT( m_iReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_HACKING_MODULE_LIST_REQ, obj, ks )
{
	return GET( m_iReleaseTick )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_HACKING_MODULE_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iReleaseTick )
		&& PUT( m_vecModuleList )
		&& PUT( m_bRequest )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_HACKING_MODULE_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iReleaseTick )
		&& GET( m_vecModuleList )
		&& GET( m_bRequest )
		;
}
#endif SERV_HACKING_DLL_CHECK

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM

SERIALIZE_DEFINE_PUT( KEGB_GET_AUTO_PARTY_BONUS_INFO_ACK, obj, ks )
{
	return PUT( m_vecAutoPartyBonusInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_GET_AUTO_PARTY_BONUS_INFO_ACK, obj, ks )
{
	return GET( m_vecAutoPartyBonusInfo )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, obj, ks )
{
	return PUT( m_cDungeonMode )
		&& PUT( m_iDungeonIDWithDif )
		&& PUT( m_bActivatedBonus )
		;
}

SERIALIZE_DEFINE_GET( KEGB_UPDATE_AUTO_PARTY_BONUS_INFO_NOT, obj, ks )
{
	return GET( m_cDungeonMode )
		&& GET( m_iDungeonIDWithDif )
		&& GET( m_bActivatedBonus )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ, obj, ks )
{
	return PUT( m_kBattleFieldJoinInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_REQ, obj, ks )
{
	return GET( m_kBattleFieldJoinInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kBattleFieldJoinInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_GET_PARTY_INFO_FOR_JOIN_BATTLE_FIELD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kBattleFieldJoinInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_OPEN_BATTLE_FIELD_REQ, obj, ks )
{
	return PUT( m_kRoomInfo )
		&& PUT( m_kRoomUserInfo )
		&& PUT( m_vecStudentUnitUID )
		&& PUT( m_kBattleFieldJoinInfo )
		&& PUT( m_kReturnToFieldInfo )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		&& PUT( m_KBossFieldCreateInfo )
		&& PUT( m_kBossFieldJoinInfo )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_BATTLE_FIELD_REQ, obj, ks )
{
	return GET( m_kRoomInfo )
		&& GET( m_kRoomUserInfo )
		&& GET( m_vecStudentUnitUID )
		&& GET( m_kBattleFieldJoinInfo )
		&& GET( m_kReturnToFieldInfo )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		&& GET( m_KBossFieldCreateInfo )
		&& GET( m_kBossFieldJoinInfo )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}


SERIALIZE_DEFINE_PUT( KERM_OPEN_BATTLE_FIELD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_RoomInfo )
		&& PUT( m_vecSlot )
		&& PUT( m_wstrCNIP )
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        && PUT( m_kBattleFieldJoinInfo )
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		&& PUT( m_StartPosIndex )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		&& PUT( m_wstrUDPRelayIP )
		&& PUT( m_usUDPRelayPort )
#endif SERV_CHOOSE_FASTEST_HOST
		//}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		&& PUT( m_tRemainFieldHoldingTime )
		&& PUT( m_kBossFieldJoinInfo )
		&& PUT( m_bBossField )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_BATTLE_FIELD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_RoomInfo )
		&& GET( m_vecSlot )
		&& GET( m_wstrCNIP )
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        && GET( m_kBattleFieldJoinInfo )
#else   SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		&& GET( m_StartPosIndex )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		&& GET( m_wstrUDPRelayIP )
		&& GET( m_usUDPRelayPort )
#endif SERV_CHOOSE_FASTEST_HOST
		//}
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-05	// 박세훈
		&& GET( m_tRemainFieldHoldingTime )
		&& GET( m_kBossFieldJoinInfo )
		&& GET( m_bBossField )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}


SERIALIZE_DEFINE_PUT( KERM_JOIN_BATTLE_FIELD_REQ, obj, ks )
{
	return PUT( m_kRoomUserInfo )
		&& PUT( m_vecStudentUnitUID )
		&& PUT( m_kBattleFieldJoinInfo )
		&& PUT( m_kReturnToFieldInfo )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		&& PUT( m_kBossFieldJoinInfo )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KERM_JOIN_BATTLE_FIELD_REQ, obj, ks )
{
	return GET( m_kRoomUserInfo )
		&& GET( m_vecStudentUnitUID )
		&& GET( m_kBattleFieldJoinInfo )
		&& GET( m_kReturnToFieldInfo )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		&& GET( m_kBossFieldJoinInfo )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
SERIALIZE_DEFINE_PUT( KERM_JOIN_BATTLE_FIELD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kBattleFieldJoinInfo )
		&& PUT( m_RoomInfo )
		&& PUT( m_vecSlot )
		&& PUT( m_wstrCNIP )
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		&& PUT( m_iLastTouchIndex )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		&& PUT( m_iRequireLevel )
		&& PUT( m_iRequireDungeonID )
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		&& PUT( m_wstrUDPRelayIP )
		&& PUT( m_usUDPRelayPort )
#endif SERV_CHOOSE_FASTEST_HOST
		//}
		//{{ 2012. 11. 21	최육사		난입자에게 캐쉬샵 유저 정보 보내기
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
		&& PUT( m_vecEnterCashShopUser )
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
		//}}
		&& PUT( m_tRemainFieldHoldingTime )
		&& PUT( m_kBossFieldJoinInfo )
		&& PUT( m_bBossField )
		;
}

SERIALIZE_DEFINE_GET( KERM_JOIN_BATTLE_FIELD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kBattleFieldJoinInfo )
		&& GET( m_RoomInfo )
		&& GET( m_vecSlot )
		&& GET( m_wstrCNIP )
#ifndef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		&& GET( m_iLastTouchIndex )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		&& GET( m_iRequireLevel )
		&& GET( m_iRequireDungeonID )
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		&& GET( m_wstrUDPRelayIP )
		&& GET( m_usUDPRelayPort )
#endif SERV_CHOOSE_FASTEST_HOST
		//}
		//{{ 2012. 11. 21	최육사		난입자에게 캐쉬샵 유저 정보 보내기
#ifdef SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
		&& GET( m_vecEnterCashShopUser )
#endif SERV_VIEW_CASH_SHOP_USER_LIST_IN_BATTLE_FIELD
		//}}
		&& GET( m_tRemainFieldHoldingTime )
		&& GET( m_kBossFieldJoinInfo )
		&& GET( m_bBossField )
		;
}
#endif // SERV_BATTLE_FIELD_BOSS


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_BATTLE_FIELD_ROOM_INFO_NOT, obj, ks )
{
	return PUT( m_cType )
		&& PUT( m_kBFRoomInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_BATTLE_FIELD_ROOM_INFO_NOT, obj, ks )
{
	return GET( m_cType )
		&& GET( m_kBFRoomInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ, obj, ks )
{
	return PUT( m_kDungeonGameInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_REQ, obj, ks )
{
	return GET( m_kDungeonGameInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrFailUserNickName )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHECK_DUNGEON_GAME_PLAY_CONDITION_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrFailUserNickName )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ, obj, ks )
{
	return PUT( m_bRemakingAutoParty )
		&& PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_REQ, obj, ks )
{
	return GET( m_bRemakingAutoParty )
		&& GET( m_iPartyUID )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrFailUserNickName )
		&& PUT( m_bRemakingAutoParty )
		&& PUT( m_kAutoPartyCondition )
		&& PUT( m_vecAutoPartyUser )
		;
}

SERIALIZE_DEFINE_GET( KEPM_GET_PARTY_INFO_FOR_AUTO_PARTY_MAKING_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrFailUserNickName )
		&& GET( m_bRemakingAutoParty )
		&& GET( m_kAutoPartyCondition )
		&& GET( m_vecAutoPartyUser )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrFailUserNickName )
		;
}

SERIALIZE_DEFINE_GET( KEPM_CHECK_FOR_AUTO_PARTY_MAKING_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrFailUserNickName )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_REQUEST_AUTO_PARTY_MAKING_REQ, obj, ks )
{
	return PUT( m_bRemakingAutoParty )
		&& PUT( m_kAutoPartyCondition )
		&& PUT( m_vecAutoPartyUser )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REQUEST_AUTO_PARTY_MAKING_REQ, obj, ks )
{
	return GET( m_bRemakingAutoParty )
		&& GET( m_kAutoPartyCondition )
		&& GET( m_vecAutoPartyUser )
		
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_CANCEL_AUTO_PARTY_MAKING_REQ, obj, ks )
{
	return PUT( m_iAutoPartyUID )
		&& PUT( m_iAutoPartyWaitNumber )
		&& PUT( m_iReason )
		&& PUT( m_bSendBySystem )
		;
}

SERIALIZE_DEFINE_GET( KEGB_CANCEL_AUTO_PARTY_MAKING_REQ, obj, ks )
{
	return GET( m_iAutoPartyUID )
		&& GET( m_iAutoPartyWaitNumber )
		&& GET( m_iReason )
		&& GET( m_bSendBySystem )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_REMAKING_AUTO_PARTY_NOT, obj, ks )
{
	return PUT( m_kAutoPartyInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REMAKING_AUTO_PARTY_NOT, obj, ks )
{
	return GET( m_kAutoPartyInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_GET_REGROUP_PARTY_INFO_REQ, obj, ks )
{
	return PUT( m_iAutoPartyUID )
		&& PUT( m_kDungeonGameInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_GET_REGROUP_PARTY_INFO_REQ, obj, ks )
{
	return GET( m_iAutoPartyUID )
		&& GET( m_kDungeonGameInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEGB_GET_REGROUP_PARTY_INFO_ACK, obj, ks )
{
	return PUT( m_kPartyUserInfo )
		&& PUT( m_kPartyRoomUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_GET_REGROUP_PARTY_INFO_ACK, obj, ks )
{
	return GET( m_kPartyUserInfo )
		&& GET( m_kPartyRoomUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, obj, ks )
{
	return PUT( m_iAutoPartyUID )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& PUT( m_iAutoPartyHostUnitUID )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		&& PUT( m_kCreatePartyInfo )
		&& PUT( m_vecRegroupPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_REQ, obj, ks )
{
	return GET( m_iAutoPartyUID )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& GET( m_iAutoPartyHostUnitUID )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		&& GET( m_kCreatePartyInfo )
		&& GET( m_vecRegroupPartyUserInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iAutoPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_REGROUP_PARTY_FOR_AUTO_PARTY_DUNGEON_GAME_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iAutoPartyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_REQUEST_REGROUP_PARTY_NOT, obj, ks )
{
	return PUT( m_kCreatePartyInfo )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& PUT( m_iOldHostUnitUID )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		&& PUT( m_sPartyFever )
		&& PUT( m_vecRegroupPartyUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPM_REQUEST_REGROUP_PARTY_NOT, obj, ks )
{
	return GET( m_kCreatePartyInfo )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& GET( m_iOldHostUnitUID )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		&& GET( m_sPartyFever )
		&& GET( m_vecRegroupPartyUserInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT, obj, ks )
{
	return PUT( m_cType )
		;
}

SERIALIZE_DEFINE_GET( KEGB_AUTO_PARTY_MAKING_ADMIN_COMMAND_NOT, obj, ks )
{
	return GET( m_cType )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_BATTLE_FIELD_ZU_ECHO_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		//{{ 2012. 11. 20	최육사		배틀필드 좀비 유저 체크 강화
#ifdef SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		&& PUT( m_iRoomUID )
#endif SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_BATTLE_FIELD_ZU_ECHO_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		//{{ 2012. 11. 20	최육사		배틀필드 좀비 유저 체크 강화
#ifdef SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		&& GET( m_iRoomUID )
#endif SERV_ADVANCED_BATTLE_FIELD_ZOMBIE_USER_CHECK
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_DUMP_ROOM_MONSTER_NOT, obj, ks )
{
	return PUT( m_wstrNickName )
		&& PUT( m_iNpcUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_DUMP_ROOM_MONSTER_NOT, obj, ks )
{
	return GET( m_wstrNickName )
		&& GET( m_iNpcUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_DUMP_BATTLE_FIELD_NOT, obj, ks )
{
	return PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KERM_DUMP_BATTLE_FIELD_NOT, obj, ks )
{
	return GET( m_wstrNickName )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		&& PUT( m_vecPartyUnitUIDList )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_PARTY_INFO_TO_BATTLE_FIELD_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		&& GET( m_vecPartyUnitUIDList )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT, obj, ks )
{
	return PUT( m_kJoinRoomInfo )
		;  
}

SERIALIZE_DEFINE_GET( KEPM_UPDATE_PARTY_USER_JOIN_ROOM_INFO_NOT, obj, ks )
{
	return GET( m_kJoinRoomInfo )
		;  
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_END_GAME_REGROUP_PARTY_NOT, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_END_GAME_REGROUP_PARTY_NOT, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iPartyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT, obj, ks )
{
	return PUT( m_iPartyUID )
		;
}

SERIALIZE_DEFINE_GET( KEPM_LEAVE_PARTY_BY_AUTO_PARTY_NOT, obj, ks )
{
	return GET( m_iPartyUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_COMPLETE_RETURN_TO_FIELD_NOT, obj, ks )
{
	return PUT( m_kReturnToFieldInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_COMPLETE_RETURN_TO_FIELD_NOT, obj, ks )
{
	return GET( m_kReturnToFieldInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELOG_BATTLE_FIELD_LEAVE_LOG_NOT, obj, ks )
{
	return PUT( m_wstrServerIP )
		&& PUT( m_iBattleFieldID )
		&& PUT( m_iRoomUID )
		&& PUT( m_cLeaveType )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_iPlayTime )
		&& PUT( m_iGetEXP )
		&& PUT( m_iGetED )
		&& PUT( m_wstrRegDate )
		//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
		&& PUT( m_iFieldFrame )
#endif SERV_FIELD_FRAME_LOG
		//}}
#ifdef SERV_ADD_FIELD_LOG_COLUMN
		&& PUT( m_sNpcDieCount )
#endif SERV_ADD_FIELD_LOG_COLUMN
		;
}

SERIALIZE_DEFINE_GET( KELOG_BATTLE_FIELD_LEAVE_LOG_NOT, obj, ks )
{
	return GET( m_wstrServerIP )
		&& GET( m_iBattleFieldID )
		&& GET( m_iRoomUID )
		&& GET( m_cLeaveType )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_iPlayTime )
		&& GET( m_iGetEXP )
		&& GET( m_iGetED )
		&& GET( m_wstrRegDate )
		//{{ 2012. 11. 20	박세훈	Field Frame 로그 추가
#ifdef SERV_FIELD_FRAME_LOG
		&& GET( m_iFieldFrame )
#endif SERV_FIELD_FRAME_LOG
		//}}
#ifdef SERV_ADD_FIELD_LOG_COLUMN
		&& GET( m_sNpcDieCount )
#endif SERV_ADD_FIELD_LOG_COLUMN
		;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//////////////////////////////////////////////////////////////////////////
//}}

//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
SERIALIZE_DEFINE_PUT( KDBE_GET_CENTER_SERVER_LIST_ACK, obj, ks )
{
	return PUT( m_vecCenterServerList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_CENTER_SERVER_LIST_ACK, obj, ks )
{
	return GET( m_vecCenterServerList )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT, obj, ks )
{
	return PUT( m_vecUdpRelayCheckLog )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_UDP_RELAY_CHECK_STATISTICS_NOT, obj, ks )
{
	return GET( m_vecUdpRelayCheckLog )
		;
}
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 03. 25	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
SERIALIZE_DEFINE_PUT( KEGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT, obj, ks )
{
	return PUT( m_iTheGateOfDarknessIndividualBuffLevel );
}

SERIALIZE_DEFINE_GET( KEGB_THE_GATE_OF_DARKNESS_INDIVIDUAL_BUFF_NOT, obj, ks )
{
	return GET( m_iTheGateOfDarknessIndividualBuffLevel );
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}

//{{ 2012. 03. 29	김민성		중복 sn 접속 감시
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
SERIALIZE_DEFINE_PUT( KEGB_REGISTER_SERVERSN_REQ, obj, ks )
{
	return PUT( m_iUserUid )
		&& PUT( m_iServerSN )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REGISTER_SERVERSN_REQ, obj, ks )
{
	return GET( m_iUserUid )
		&& GET( m_iServerSN )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_REGISTER_SERVERSN_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUserUid )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REGISTER_SERVERSN_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUserUid )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_UNREGISTER_SERVERSN_NOT, obj, ks )
{
	return PUT( m_iServerSN )
		&& PUT( m_iUserUid )
		;
}

SERIALIZE_DEFINE_GET( KEGB_UNREGISTER_SERVERSN_NOT, obj, ks )
{
	return GET( m_iServerSN )
		&& GET( m_iUserUid )
		;
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_FILTER
//}}

//{{ 2012. 03. 29	김민성		x2.exe 모듈 유무 검사
#ifdef SERV_CHECK_X2_EXE_MODULE
SERIALIZE_DEFINE_PUT( KDBE_MODULE_EXIST_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iServerSN )
		&& PUT( m_mapModule )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_MODULE_EXIST_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iServerSN )
		&& GET( m_mapModule )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_CHECK_X2_EXE_MODULE
//}}

//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP
SERIALIZE_DEFINE_PUT( KSOAP_CHECK_NEXON_ID_REQ, obj, ks )
{
	return PUT( m_iServiceCode )
		&& PUT( m_iGameCode )
		&& PUT( m_iNexonSN )
		&& PUT( m_wstrLoginID )
		&& PUT( m_strPassword )
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_wstrMessage )
		&& PUT( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& PUT( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KSOAP_CHECK_NEXON_ID_REQ, obj, ks )
{
	return GET( m_iServiceCode )
		&& GET( m_iGameCode )
		&& GET( m_iNexonSN )
		&& GET( m_wstrLoginID )
		&& GET( m_strPassword )
		&& GET( m_wstrReceiverNickName )
		&& GET( m_wstrMessage )
		&& GET( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& GET( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_CHECK_NEXON_ID_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrLoginID )
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_wstrMessage )
		&& PUT( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& PUT( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KSOAP_CHECK_NEXON_ID_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrLoginID )
		&& GET( m_wstrReceiverNickName )
		&& GET( m_wstrMessage )
		&& GET( m_vecPurchaseReqInfo )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
		&& GET( m_bUseCoupon )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}
#endif SERV_NEXON_AUTH_SOAP
//}}

//{{ 2012. 05. 08	김민성       개인 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_TRADE
SERIALIZE_DEFINE_PUT( KDBE_TRADE_COMPLETE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iED )
		&& PUT( m_kQuantityUpdate )
		&& PUT( m_vecInventorySlotInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_TRADE_COMPLETE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iED )
		&& GET( m_kQuantityUpdate )
		&& GET( m_vecInventorySlotInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_TRADE_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iED )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecInventorySlotInfo )
		&& PUT( m_mapUpdateFailed )
		;
}

SERIALIZE_DEFINE_GET( KDBE_TRADE_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iED )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecInventorySlotInfo )
		&& GET( m_mapUpdateFailed )
		;
}
#endif SERV_TRADE_LOGIC_CHANGE_TRADE
//}}

//{{ 2012. 05. 30	최육사		공식 대전 버그 수정
#ifdef SERV_OFFCIAL_PVP_BUG_FIX
SERIALIZE_DEFINE_PUT( KEPM_UPDATE_PVP_INFO_NOT, obj, ks )
{
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	return PUT( m_cRank )
		&& PUT( m_iOfficialMatchCnt )
		&& PUT( m_iRating )
#else
	return PUT( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& PUT( m_iRPoint )
		;
}

SERIALIZE_DEFINE_GET( KEPM_UPDATE_PVP_INFO_NOT, obj, ks )
{
	//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
	return GET( m_cRank )
		&& GET( m_iOfficialMatchCnt )
		&& GET( m_iRating )
#else
	return GET( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& GET( m_iRPoint )
		;
}
#endif SERV_OFFCIAL_PVP_BUG_FIX
//}}

//{{ 2012. 05. 29	김민성       개인 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
SERIALIZE_DEFINE_PUT( KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ, obj, ks )
{
	return PUT( m_iSellUnitUID )
		&& PUT( m_iSellUnitGSUID )
		&& PUT( m_iSellUnitEDIN )
		&& PUT( m_iSellCommissionED )
		&& PUT( m_iBuyUnitUID )
		&& PUT( m_iBuyUnitGSUID )
		&& PUT( m_iBuyUnitEDOUT )
		&& PUT( m_iBuyCommissionED )
		&& PUT( m_bBuyUnitIsSquare )
		&& PUT( m_kSellItemInfo )
		&& PUT( m_kBuyItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_REQ, obj, ks )
{
	return GET( m_iSellUnitUID )
		&& GET( m_iSellUnitGSUID )
		&& GET( m_iSellUnitEDIN )
		&& GET( m_iSellCommissionED )
		&& GET( m_iBuyUnitUID )
		&& GET( m_iBuyUnitGSUID )
		&& GET( m_iBuyUnitEDOUT )
		&& GET( m_iBuyCommissionED )
		&& GET( m_bBuyUnitIsSquare )
		&& GET( m_kSellItemInfo )
		&& GET( m_kBuyItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kResult )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BUY_PERSONAL_SHOP_ITEM_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kResult )
		;
}
#endif SERV_TRADE_LOGIC_CHANGE_PERSONAL_SHOP
//}}

//{{ 2012. 05. 17	박세훈	키보드 맵핑 정보를 DB에서 읽고 쓰기
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
SERIALIZE_DEFINE_PUT( KDBE_KEYBOARD_MAPPING_INFO_WRITE_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_kKeyboardMappingInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_KEYBOARD_MAPPING_INFO_WRITE_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_kKeyboardMappingInfo )
		;
}
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}

//{{ 2012. 05. 29	박세훈	채팅 옵션 정보를 DB에서 읽고 쓰기
#ifdef SERV_CHAT_OPTION_INFO_RW
SERIALIZE_DEFINE_PUT( KDBE_CHAT_OPTION_INFO_WRITE_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_cIndex )
		&& PUT( m_bValue )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHAT_OPTION_INFO_WRITE_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_cIndex )
		&& GET( m_bValue )
		;
}
#endif SERV_CHAT_OPTION_INFO_RW
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2012. 05. 30	최육사	그노시스 기간 치트
#ifdef SERV_CASH_SKILL_POINT_DATE_CHANGE
SERIALIZE_DEFINE_PUT( KDBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrModifyDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_CASH_SKILL_POINT_DATE_CHANGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrModifyDate )
		;
}
#endif SERV_CASH_SKILL_POINT_DATE_CHANGE
//}}

//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iItemID )
		&& PUT( m_iState )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iItemID )
		&& GET( m_iState )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 06. 28	김민성       이벤트 재화(천사의 깃털)
#ifdef SERV_EVENT_MONEY
SERIALIZE_DEFINE_PUT( KDBE_UPDATE_EVENT_MONEY_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iOldQuantity )
		&& PUT( m_iNewQuantity )
		&& PUT( m_iRewardItemID )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_EVENT_MONEY_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iOldQuantity )
		&& GET( m_iNewQuantity )
		&& GET( m_iRewardItemID )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_EVENT_MONEY
//}}

//{{ 2012. 07. 02	최육사		대기열 시스템
#ifdef SERV_WAIT_QUEUE_SYSTEM
SERIALIZE_DEFINE_PUT( KDBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK, obj, ks )
{
	return PUT( m_iTotalCCU )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_TOTAL_CCU_FOR_WAIT_QUEUE_ACK, obj, ks )
{
	return GET( m_iTotalCCU )
		;
}
#endif SERV_WAIT_QUEUE_SYSTEM
//}}

//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
SERIALIZE_DEFINE_PUT( KDBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_vecUnSealedSkill )
		&& PUT( m_mapSkillInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_CHEAT_GET_ALL_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_vecUnSealedSkill )
		&& GET( m_mapSkillInfo )
		;
}
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}

//{{ 2012. 07. 27	최육사		사제시스템 EXP 보너스 버그 수정
#ifdef SERV_TUTO_EXP_BONUS_BUG_FIX
SERIALIZE_DEFINE_PUT( KERM_UPDATE_TUTORIAL_INFO_NOT, obj, ks )
{
	return PUT( m_vecStudentUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_TUTORIAL_INFO_NOT, obj, ks )
{
	return GET( m_vecStudentUnitUID )
		;
}
#endif SERV_TUTO_EXP_BONUS_BUG_FIX
//}}

//{{ 우편 ED 거래시 ED 동기화 - 김민성
#ifdef SERV_SEND_LETTER_BEFOR_ED_SYNC
SERIALIZE_DEFINE_PUT( KDBE_SYNC_ED_REQ, obj, ks )
{
	return PUT( m_iFromUnitUID )
		&& PUT( m_bIsGmMessage )
		&& PUT( m_wstrToNickName )
		&& PUT( m_wstrTitle )
		&& PUT( m_wstrMessage )
		&& PUT( m_iED )
		&& PUT( m_iItemUID )
		&& PUT( m_iQuantity )
		&& PUT( m_iChangeED )
		&& PUT( m_iSendLetterCost )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SYNC_ED_REQ, obj, ks )
{
	return GET( m_iFromUnitUID )
		&& GET( m_bIsGmMessage )
		&& GET( m_wstrToNickName )
		&& GET( m_wstrTitle )
		&& GET( m_wstrMessage )
		&& GET( m_iED )
		&& GET( m_iItemUID )
		&& GET( m_iQuantity )
		&& GET( m_iChangeED )
		&& GET( m_iSendLetterCost )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SYNC_ED_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SYNC_ED_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kInfo )
		;
}
#endif SERV_SEND_LETTER_BEFOR_ED_SYNC
//}}

//{{ 2012 대전 시즌2 전야 런칭 이벤트	- 김민성
#ifdef SERV_2012_PVP_SEASON2_EVENT
SERIALIZE_DEFINE_PUT( KDBE_PVP_WIN_EVENT_CHECK_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iIndex )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PVP_WIN_EVENT_CHECK_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iIndex )
		&& GET( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_PVP_WIN_EVENT_CHECK_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iIndex )
		&& PUT( m_wstrRegDate )
		&& PUT( m_kRewardAck )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PVP_WIN_EVENT_CHECK_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iIndex )
		&& GET( m_wstrRegDate )
		&& GET( m_kRewardAck )
		;
}
#endif SERV_2012_PVP_SEASON2_EVENT
//}}

//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
SERIALIZE_DEFINE_PUT( KBingoEventLog, obj, ks )
{
	return PUT( iUnitUID )
		&& PUT( iActionType )
		&& PUT( iOpenNum )
		&& PUT( iPos )
		&& PUT( iItemID )
		&& PUT( wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KBingoEventLog, obj, ks )
{
	return GET( iUnitUID )
		&& GET( iActionType )
		&& GET( iOpenNum )
		&& GET( iPos )
		&& GET( iItemID )
		&& GET( wstrRegDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_BINGO_EVENT_INFO_WRITE_REQ, obj, ks )
{
	return PUT( m_usAckEventID )
		&& PUT( m_bChanceChanged )
		&& PUT( m_iBalance )
		&& PUT( m_iLotteryChance )
		&& PUT( m_iLotteryRecord )
		&& PUT( m_iMixChance )
		&& PUT( m_iResetChance )
		&& PUT( m_mapBingoBoard )
		&& PUT( m_mapPresentInfo )
		&& PUT( m_vecLog )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BINGO_EVENT_INFO_WRITE_REQ, obj, ks )
{
	return GET( m_usAckEventID )
		&& GET( m_bChanceChanged )
		&& GET( m_iBalance )
		&& GET( m_iLotteryChance )
		&& GET( m_iLotteryRecord )
		&& GET( m_iMixChance )
		&& GET( m_iResetChance )
		&& GET( m_mapBingoBoard )
		&& GET( m_mapPresentInfo )
		&& GET( m_vecLog )
		;
}
#endif SERV_EVENT_BINGO
//}}

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
SERIALIZE_DEFINE_PUT( KERM_UPDATE_QUEST_INFO_NOT, obj, ks )
{
	return PUT( m_vecNPC )
		&& PUT( m_isDungeon )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_QUEST_INFO_NOT, obj, ks )
{
	return GET( m_vecNPC )
		&& GET( m_isDungeon )
		;
}
#endif SERV_REFORM_QUEST
//}}

//{{ 2012. 10. 16	최육사		ED&아이템 모니터링 시스템
#ifdef SERV_ED_AND_ITEM_MORNITORING_SYSTEM
SERIALIZE_DEFINE_PUT( KEDMornitoringInfo, obj, ks )
{
	return PUT( m_iMaxED )
		&& PUT( m_iAverageED )
		;
}

SERIALIZE_DEFINE_GET( KEDMornitoringInfo, obj, ks )
{
	return GET( m_iMaxED )
		&& GET( m_iAverageED )
		;
}


SERIALIZE_DEFINE_PUT( KItemMornitoringInfo, obj, ks )
{
	return PUT( m_iMaxItemCount )
		&& PUT( m_iAverageItemCount )
		;
}

SERIALIZE_DEFINE_GET( KItemMornitoringInfo, obj, ks )
{
	return GET( m_iMaxItemCount )
		&& GET( m_iAverageItemCount )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK, obj, ks )
{
	return PUT( m_mapEDMornitoringInfo )
		&& PUT( m_mapItemMornitoringInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ED_AND_ITEM_MORNITOING_INFO_ACK, obj, ks )
{
	return GET( m_mapEDMornitoringInfo )
		&& GET( m_mapItemMornitoringInfo )
		;
}
#endif SERV_ED_AND_ITEM_MORNITORING_SYSTEM
//}}

//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
SERIALIZE_DEFINE_PUT( KDBUpdateGamePlayNetworkInfo, obj, ks )
{
	return PUT( m_iGameType )
		&& PUT( m_iHolePunchingTryCount )
		&& PUT( m_iP2PPlayTime )
		&& PUT( m_iRelayPlayTime )
		&& PUT( m_wstrRelayServerIP )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBUpdateGamePlayNetworkInfo, obj, ks )
{
	return GET( m_iGameType )
		&& GET( m_iHolePunchingTryCount )
		&& GET( m_iP2PPlayTime )
		&& GET( m_iRelayPlayTime )
		&& GET( m_wstrRelayServerIP )
		&& GET( m_wstrRegDate )
		;
}


SERIALIZE_DEFINE_PUT( KELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT, obj, ks )
{
	return PUT( m_vecDBUpdateInfo )
		;
}

SERIALIZE_DEFINE_GET( KELOG_COLLECTION_OF_RELAY_AND_P2P_INFO_NOT, obj, ks )
{
	return GET( m_vecDBUpdateInfo )
		;
}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
SERIALIZE_DEFINE_PUT( KERM_UPDATE_VISIT_CASH_SHOP_NOT, obj, ks )
{
	return PUT( m_bEnterCashShop )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_VISIT_CASH_SHOP_NOT, obj, ks )
{
	return GET( m_bEnterCashShop )
		&& GET( m_iUnitUID )
		;
}
#endif SERV_VISIT_CASH_SHOP
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2012. 03. 20	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
SERIALIZE_DEFINE_PUT( KEGB_REGISTER_UNIT_NOT, obj, ks )
{
	return PUT( m_kUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REGISTER_UNIT_NOT, obj, ks )
{
	return GET( m_kUnitInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEGB_UNIT_DISCONNECT_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
		&& PUT( m_iUserUID )
#endif SERV_EVENT_JACKPOT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEGB_UNIT_DISCONNECT_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
		&& GET( m_iUserUID )
#endif SERV_EVENT_JACKPOT
		//}}
		;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//{{ 2012. 10. 7	박세훈	랜선렉 로그 추가
#ifdef UDP_CAN_NOT_SEND_USER_KICK_LOG
SERIALIZE_DEFINE_PUT( KLanBugRecord, obj, ks )
{
	return PUT( m_iMyUnitUID )
		&& PUT( m_iAccusationUnitUID )
		&& PUT( m_byteNum )
		&& PUT( m_bNowKickOut )
		&& PUT( m_iMyUnitWin )
		;
}

SERIALIZE_DEFINE_GET( KLanBugRecord, obj, ks )
{
	return GET( m_iMyUnitUID )
		&& GET( m_iAccusationUnitUID )
		&& GET( m_byteNum )
		&& GET( m_bNowKickOut )
		&& GET( m_iMyUnitWin )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UDP_CHECK_KICK_USER_NOT, obj, ks )
{
	return PUT( m_iRoomUID )
		&& PUT( m_iPVPChannelClass )
		&& PUT( m_iMatchType )
		&& PUT( m_vecLanBugRecord )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UDP_CHECK_KICK_USER_NOT, obj, ks )
{
	return GET( m_iRoomUID )
		&& GET( m_iPVPChannelClass )
		&& GET( m_iMatchType )
		&& GET( m_vecLanBugRecord )
		;
}
#endif UDP_CAN_NOT_SEND_USER_KICK_LOG
//}}

//{{ 2012. 11. 13	최육사		자동파티 대기열 카운트 표시
#ifdef SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
SERIALIZE_DEFINE_PUT( KEGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT, obj, ks )
{
	return PUT( m_iAutoPartyWaitNumber )
		;
}

SERIALIZE_DEFINE_GET( KEGB_REQUEST_AUTO_PARTY_WAIT_USER_COUNT_NOT, obj, ks )
{
	return GET( m_iAutoPartyWaitNumber )
		;
}
#endif SERV_AUTO_PARTY_WAIT_USER_COUNT_VIEW
//}}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
SERIALIZE_DEFINE_PUT( KECN_PARTY_BUFF_UPDATE_NOT, obj, ks )
{
	return PUT( m_mapActivateBuffList )
		&& PUT( m_setDeactivateBuffList )
		;
}

SERIALIZE_DEFINE_GET( KECN_PARTY_BUFF_UPDATE_NOT, obj, ks )
{
	return GET( m_mapActivateBuffList )
		&& GET( m_setDeactivateBuffList )
		;
}

SERIALIZE_DEFINE_PUT( KERM_ACTIVATE_BUFF_NOT, obj, ks )
{
	return PUT( m_vecActivateBuffList )
		;
}

SERIALIZE_DEFINE_GET( KERM_ACTIVATE_BUFF_NOT, obj, ks )
{
	return GET( m_vecActivateBuffList )
		;
}

SERIALIZE_DEFINE_PUT( KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecActivateBuff )
		&& PUT( m_vecDeActivateBuff )
		&& PUT( m_bSendAck )
		;
}

SERIALIZE_DEFINE_GET( KELG_UPDATE_USER_UNIT_BUFF_INFO_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecActivateBuff )
		&& GET( m_vecDeActivateBuff )
		&& GET( m_bSendAck )
		;
}

SERIALIZE_DEFINE_PUT( KELG_UPDATE_USER_UNIT_BUFF_INFO_ACK, obj, ks )
{
	return PUT( m_kUnitBuffInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_UPDATE_USER_UNIT_BUFF_INFO_ACK, obj, ks )
{
	return GET( m_kUnitBuffInfo )
		;
}

SERIALIZE_DEFINE_PUT( KERM_PARTY_OPEN_DUNGEON_ROOM_NOT, obj, ks )
{
	return PUT( m_sWorldID )
		&& PUT( m_RoomInfo )
		&& PUT( m_vecSlot )
		&& PUT( m_vecObserverSlot )
		&& PUT( m_wstrCNIP )
		&& PUT( m_vecInventorySlotInfo )
		&& PUT( m_mapActivateBuff )
		;
}

SERIALIZE_DEFINE_GET( KERM_PARTY_OPEN_DUNGEON_ROOM_NOT, obj, ks )
{
	return GET( m_sWorldID )
		&& GET( m_RoomInfo )
		&& GET( m_vecSlot )
		&& GET( m_vecObserverSlot )
		&& GET( m_wstrCNIP )
		&& GET( m_vecInventorySlotInfo )
		&& GET( m_mapActivateBuff )
		;
}
#endif SERV_SERVER_BUFF_SYSTEM
//}}

//{{ 2012. 12. 18	최육사	아라 파티 플레이 보너스 경험치
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
SERIALIZE_DEFINE_PUT( KERM_DEACTIVATE_BUFF_NOT, obj, ks )
{
	return PUT( m_vecDeactivateBuffID )
		;
}

SERIALIZE_DEFINE_GET( KERM_DEACTIVATE_BUFF_NOT, obj, ks )
{
	return GET( m_vecDeactivateBuffID )
		;
}
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
//}}

//{{ 2012. 11. 20	최육사		좀비 배틀필드 체크 및 삭제
#ifdef SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE
SERIALIZE_DEFINE_PUT( KERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT, obj, ks )
{
	return PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KERM_KILL_ZOMBIE_USER_IN_BATTLE_FIELD_NOT, obj, ks )
{
	return GET( m_wstrNickName )
		;
}
#endif SERV_ZOMBIE_BATTLE_FIELD_ROOM_CHECK_AND_DELETE

//{{ 2012. 12. 12	최육사	결과창 이탈 유저 로그
#ifdef SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
SERIALIZE_DEFINE_PUT( KDBE_DUNGEON_RESULT_LEAVE_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrLeaveReason )
		&& PUT( m_wstrRoomState )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DUNGEON_RESULT_LEAVE_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrLeaveReason )
		&& GET( m_wstrRoomState )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_CHECK_USER_NEVER_RETURN_TO_FIELD
//}}
//}}

//{{ 2012. 11. 15	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG
SERIALIZE_DEFINE_PUT( KERM_FIELD_USER_STATISTICS_NOT, obj, ks )
{
	return PUT( m_iGamePlayTime )
		;
}

SERIALIZE_DEFINE_GET( KERM_FIELD_USER_STATISTICS_NOT, obj, ks )
{
	return GET( m_iGamePlayTime )
		;
}
#endif SERV_FIELD_PLAY_TIME_LOG
//}}

//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
SERIALIZE_DEFINE_PUT( KJackPotEventInitInfo, obj, ks )
{
	return PUT( m_wstrCuponNumber )
		&& PUT( m_wstrBeginDate )
		&& PUT( m_wstrEndDate )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iState )
		;
}

SERIALIZE_DEFINE_GET( KJackPotEventInitInfo, obj, ks )
{
	return GET( m_wstrCuponNumber )
		&& GET( m_wstrBeginDate )
		&& GET( m_wstrEndDate )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iState )
		;
}

SERIALIZE_DEFINE_PUT( KJackPotEventWinInfo, obj, ks )
{
	return PUT( m_wstrCuponNumber )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iState )
		;
}

SERIALIZE_DEFINE_GET( KJackPotEventWinInfo, obj, ks )
{
	return GET( m_wstrCuponNumber )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iState )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_JACKPOT_EVENT_INIT_ACK, obj, ks )
{
	return PUT( m_vecJackpotEventInitInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_JACKPOT_EVENT_INIT_ACK, obj, ks )
{
	return GET( m_vecJackpotEventInitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_JACKPOT_EVENT_UPDATE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrCuponNumber )
		;
}

SERIALIZE_DEFINE_GET( KDBE_JACKPOT_EVENT_UPDATE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_wstrCuponNumber )
		;
}
#endif SERV_EVENT_JACKPOT
//}}

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
SERIALIZE_DEFINE_PUT( KDBE_NEW_ACCOUNT_QUEST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kQuestReq )
		;
}

SERIALIZE_DEFINE_GET( KDBE_NEW_ACCOUNT_QUEST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kQuestReq )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ACCOUNT_QUEST_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kQuestReq )
		&& PUT( m_tCompleteTime )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCOUNT_QUEST_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kQuestReq )
		&& GET( m_tCompleteTime )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ACCOUNT_QUEST_UPDATE_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_vecAccountQuesting )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCOUNT_QUEST_UPDATE_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_vecAccountQuesting )
		;
}
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}

//{{ 2012. 12. 19  교환(한정 수량) - 김민성
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
SERIALIZE_DEFINE_PUT( KDBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ, obj, ks )
{
	return PUT( m_kReq )
		&& PUT( m_kToDB )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ, obj, ks )
{
	return GET( m_kReq )
		&& GET( m_kToDB )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kDB )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kDB )
		;
}
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
//}}

//{{ 2012. 12. 24	박세훈	펫 오토 루팅 기능 추가
#ifdef SERV_PET_AUTO_LOOTING
SERIALIZE_DEFINE_PUT( KDBE_PET_AUTO_LOOTING_NOT, obj, ks )
{
	return PUT( m_iPetUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PET_AUTO_LOOTING_NOT, obj, ks )
{
	return GET( m_iPetUID )
		;
}
#endif SERV_PET_AUTO_LOOTING
//}}

//{{ 2013. 01. 09 던전 강퇴 시스템 - 김민성
#ifdef SERV_DUNGEON_FORCED_EXIT_SYSTEM
SERIALIZE_DEFINE_PUT( KERM_BAD_ATTITUDE_USER_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iDungeonIDAndDif )
		&& PUT( m_sStartMember )
		&& PUT( m_sNowMember )
		&& PUT( m_sVoteCount )
		&& PUT( m_sExitStage )
		&& PUT( m_sExitSubStage )
		&& PUT( m_sBadAttitudePoint )
		&& PUT( m_wstrRegDate )
		&& PUT( m_bExitResult )
		;
}

SERIALIZE_DEFINE_GET( KERM_BAD_ATTITUDE_USER_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iDungeonIDAndDif )
		&& GET( m_sStartMember )
		&& GET( m_sNowMember )
		&& GET( m_sVoteCount )
		&& GET( m_sExitStage )
		&& GET( m_sExitSubStage )
		&& GET( m_sBadAttitudePoint )
		&& GET( m_wstrRegDate )
		&& GET( m_bExitResult )
		;
}
#endif SERV_DUNGEON_FORCED_EXIT_SYSTEM
//}

//{{ 2013. 02. 	최육사	Tick퍼포먼스 DB로그 기록
#ifdef SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
SERIALIZE_DEFINE_PUT( KDBE_TICK_PERFORMANCE_LOG_NOT, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_iServerType )
		&& PUT( m_dwMaxTickCount )
		&& PUT( m_wstrMaxTickRegDate )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_TICK_PERFORMANCE_LOG_NOT, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_iServerType )
		&& GET( m_dwMaxTickCount )
		&& GET( m_wstrMaxTickRegDate )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_SIMLAYER_TICK_PERFORMANCE_DB_LOG
//}}

//{{ 2013. 02. 19   교환 로그 추가 - 김민성
#ifdef SERV_EXCHANGE_LOG
SERIALIZE_DEFINE_PUT( KDBE_ITEM_EXCHANGE_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cExchangeType )
		&& PUT( m_iSourceItemID )
		&& PUT( m_iSourceItemQuantity )
		&& PUT( m_mapResultItem )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EXCHANGE_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_cExchangeType )
		&& GET( m_iSourceItemID )
		&& GET( m_iSourceItemQuantity )
		&& GET( m_mapResultItem )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_EXCHANGE_LOG
//}


//{{ 2013. 02. 21	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
SERIALIZE_DEFINE_PUT( KDBE_LOG_STATISTICS_INFO_NOT, obj, ks )
{
	return PUT( m_vecLogStat )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOG_STATISTICS_INFO_NOT, obj, ks )
{
	return GET( m_vecLogStat )
		;
}
#endif SERV_LOG_SYSTEM_NEW
//}}

//{{ 2013. 2. 26	박세훈	랜선렉 방지 코드2
#ifdef SERV_FIX_SYNC_PACKET_USING_RELAY
SERIALIZE_DEFINE_PUT( KDBE_UDP_CHECK_KICK_USER_NOT, obj, ks )
{
	return PUT( m_iRoomUID )
		&& PUT( m_iPVPChannelClass )
		&& PUT( m_iMatchType )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UDP_CHECK_KICK_USER_NOT, obj, ks )
{
	return GET( m_iRoomUID )
		&& GET( m_iPVPChannelClass )
		&& GET( m_iMatchType )
		&& GET( m_iUnitUID )
		;
}
#endif SERV_FIX_SYNC_PACKET_USING_RELAY
//}}

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
SERIALIZE_DEFINE_PUT( KDBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUID )
		&& PUT( m_wstrResetTime )
		&& PUT( m_mapRankerUnitInfo )
		&& PUT( m_mapRankerUIDInfo )
		&& PUT( m_mapRankerUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUID )
		&& GET( m_wstrResetTime )
		&& GET( m_mapRankerUnitInfo )
		&& GET( m_mapRankerUIDInfo )
		&& GET( m_mapRankerUserInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_bFiltered )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GAME_LOCAL_RANKING_USER_INFO_READ_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_bFiltered )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUID )
		&& PUT( m_kUserInfo )
		&& PUT( m_wstrUpdated )
		&& PUT( m_kUnitInfo )
		&& PUT( m_bFiltered )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUID )
		&& GET( m_kUserInfo )
		&& GET( m_wstrUpdated )
		&& GET( m_kUnitInfo )
		&& GET( m_bFiltered )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_nViewPage )
		&& PUT( m_cMainTabIndex )
		&& PUT( m_cSubTabIndex )
		&& PUT( m_byteFilter )
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		&& PUT( m_bRankingButtonClick )
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		;
}

SERIALIZE_DEFINE_GET( KDBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_nViewPage )
		&& GET( m_cMainTabIndex )
		&& GET( m_cSubTabIndex )
		&& GET( m_byteFilter )
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		&& GET( m_bRankingButtonClick )
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUID )
		&& PUT( m_kUserInfo )
		&& PUT( m_wstrUpdated )
		&& PUT( m_kUnitInfo )
		&& PUT( m_nViewPage )
		&& PUT( m_cMainTabIndex )
		&& PUT( m_cSubTabIndex )
		&& PUT( m_byteFilter )
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		&& PUT( m_bRankingButtonClick )
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUID )
		&& GET( m_kUserInfo )
		&& GET( m_wstrUpdated )
		&& GET( m_kUnitInfo )
		&& GET( m_nViewPage )
		&& GET( m_cMainTabIndex )
		&& GET( m_cSubTabIndex )
		&& GET( m_byteFilter )
#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
		&& GET( m_bRankingButtonClick )
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iIncreasedSpirit )
		&& PUT( m_iIncreasedAP )
		&& PUT( m_bForce )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GAME_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iIncreasedSpirit )
		&& GET( m_iIncreasedAP )
		&& GET( m_bForce )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUID )
		&& PUT( m_kUserInfo )
		&& PUT( m_wstrUpdated )
		&& PUT( m_kUnitInfo )
		&& PUT( m_iIncreasedSpirit )
		&& PUT( m_iIncreasedAP )
		&& PUT( m_bForce )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUID )
		&& GET( m_kUserInfo )
		&& GET( m_wstrUpdated )
		&& GET( m_kUnitInfo )
		&& GET( m_iIncreasedSpirit )
		&& GET( m_iIncreasedAP )
		&& GET( m_bForce )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_LOCAL_RANKING_USER_INFO_WRITE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kInfo )
		&& PUT( m_wstrUpdated )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOCAL_RANKING_USER_INFO_WRITE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kInfo )
		&& GET( m_wstrUpdated )
		;
}

SERIALIZE_DEFINE_PUT( KELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, obj, ks )
{
	return PUT( m_iUID )
		&& PUT( m_kUserInfo )
		&& PUT( m_kUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_LOCAL_RANKING_CHANGED_RANKER_INFO_NOT, obj, ks )
{
	return GET( m_iUID )
		&& GET( m_kUserInfo )
		&& GET( m_kUnitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_LOCAL_RANKING_RESET_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUID )
		&& PUT( m_wstrUpdated )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOCAL_RANKING_RESET_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUID )
		&& GET( m_wstrUpdated )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT, obj, ks )
{
	return PUT( m_mapChangeInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT, obj, ks )
{
	return GET( m_mapChangeInfo )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ, obj, ks )
{
	return PUT( m_iNexonSN )
		&& PUT( m_strPassword )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ, obj, ks )
{
	return GET( m_iNexonSN )
		&& GET( m_strPassword )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ, obj, ks )
{
	return PUT( m_iNexonSN )
		&& PUT( m_strPassword )
		&& PUT( m_kInfo )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ, obj, ks )
{
	return GET( m_iNexonSN )
		&& GET( m_strPassword )
		&& GET( m_kInfo )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kInfo )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kInfo )
		;
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
SERIALIZE_DEFINE_PUT( KDBE_RECOMMEND_USER_GET_NEXON_SN_REQ, obj, ks )
{
	return PUT( m_iRecommendUserNexonSN )
		&& PUT( m_iRecommendUnitUID )
		&& PUT( m_wstrRecommendedUserNickName )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RECOMMEND_USER_GET_NEXON_SN_REQ, obj, ks )
{
	return GET( m_iRecommendUserNexonSN )
		&& GET( m_iRecommendUnitUID )
		&& GET( m_wstrRecommendedUserNickName )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RECOMMEND_USER_GET_NEXON_SN_ACK, obj, ks )
{
	return PUT( m_iRecommendUserNexonSN )
		&& PUT( m_iRecommendUnitUID )
		&& PUT( m_iRecommendedUserNexonSN )
		&& PUT( m_wstrRecommendedUserNickName )
#ifdef SERV_RECOMMEND_LIST_EVENT_2013_07	// 적용날짜: 2013-07-04
		&& PUT( m_iRecommendedUserChannelCode )
#endif	// SERV_RECOMMEND_LIST_EVENT_2013_07
		;
}

SERIALIZE_DEFINE_GET( KDBE_RECOMMEND_USER_GET_NEXON_SN_ACK, obj, ks )
{
	return GET( m_iRecommendUserNexonSN )
		&& GET( m_iRecommendUnitUID )
		&& GET( m_iRecommendedUserNexonSN )
		&& GET( m_wstrRecommendedUserNickName )
#ifdef SERV_RECOMMEND_LIST_EVENT_2013_07	// 적용날짜: 2013-07-04
		&& GET( m_iRecommendedUserChannelCode )
#endif	// SERV_RECOMMEND_LIST_EVENT_2013_07
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_CHECK_SAME_USER_REQ, obj, ks )
{
	return PUT( m_iServiceCode )
		&& PUT( m_iNexonSN_A )
		&& PUT( m_iNexonSN_B )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_CHECK_SAME_USER_REQ, obj, ks )
{
	return GET( m_iServiceCode )
		&& GET( m_iNexonSN_A )
		&& GET( m_iNexonSN_B )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_CHECK_SAME_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_CHECK_SAME_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		;
}
#endif SERV_RECOMMEND_LIST_EVENT
//}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
SERIALIZE_DEFINE_PUT( KEGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_wstrNickName )
		&& PUT( m_bConnect )
		;
}

SERIALIZE_DEFINE_GET( KEGB_ADMIN_HERO_PVP_USER_TOURNAMENT_CONNECTION_NOT, obj, ks )
{
	return GET( m_iServerGroupID )
		&& GET( m_wstrNickName )
		&& GET( m_bConnect )
		;
}
#endif	// SERV_HERO_PVP_MANAGE_LIST

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-05
SERIALIZE_DEFINE_PUT( KELG_LOCAL_RANKING_WATCH_UNIT_REQ, obj, ks )
{
	return PUT( m_iRequestGSUID )
		&& PUT( m_iRequestUnitUID )
		&& PUT( m_iTargetUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_LOCAL_RANKING_WATCH_UNIT_REQ, obj, ks )
{
	return GET( m_iRequestGSUID )
		&& GET( m_iRequestUnitUID )
		&& GET( m_iTargetUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_LOCAL_RANKING_WATCH_UNIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kUserInfo )
		&& PUT( m_kUnitInfo )
		&& PUT( m_iRequestGSUID )
		&& PUT( m_iRequestUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_LOCAL_RANKING_WATCH_UNIT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kUserInfo )
		&& GET( m_kUnitInfo )
		&& GET( m_iRequestGSUID )
		&& GET( m_iRequestUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_LOCAL_RANKING_SYSTEM_CHECK_NOT, obj, ks )
{
	return PUT( m_iMainTabIndex )
		&& PUT( m_iSubTabIndex )
		;
}

SERIALIZE_DEFINE_GET( KELG_LOCAL_RANKING_SYSTEM_CHECK_NOT, obj, ks )
{
	return GET( m_iMainTabIndex )
		&& GET( m_iSubTabIndex )
		;
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
SERIALIZE_DEFINE_PUT( KERM_PREPARE_FOR_DEFENCE_DUNGEON_REQ, obj, ks )
{
	return PUT( m_kDefenceDungeonInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_PREPARE_FOR_DEFENCE_DUNGEON_REQ, obj, ks )
{
	return GET( m_kDefenceDungeonInfo )
		;
}


SERIALIZE_DEFINE_PUT( KERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK, obj, ks )
{
	return PUT( m_kRoomUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_PREPARE_FOR_DEFENCE_DUNGEON_ACK, obj, ks )
{
	return GET( m_kRoomUserInfo )
		;
}


SERIALIZE_DEFINE_PUT( KERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT, obj, ks )
{
	return PUT( m_iDefenceDungeonPhase )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHANGE_DEFENCE_DUNGEON_PHASE_NOT, obj, ks )
{
	return GET( m_iDefenceDungeonPhase )
		;
}
#endif SERV_NEW_DEFENCE_DUNGEON
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
SERIALIZE_DEFINE_PUT( KDBE_LOAD_WEDDING_HALL_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapWeddingInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOAD_WEDDING_HALL_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapWeddingInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WEDDING_HALL_INFO_UPDATE_NOT, obj, ks )
{
	return PUT( m_mapWeddingInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WEDDING_HALL_INFO_UPDATE_NOT, obj, ks )
{
	return GET( m_mapWeddingInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_COUPLE_PROPOSE_USER_FIND_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrOtherNickName )
		&& PUT( m_iOtherUserUID )
		&& PUT( m_iOtherUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_cRelationshipType )
		;
}

SERIALIZE_DEFINE_GET( KDBE_COUPLE_PROPOSE_USER_FIND_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrOtherNickName )
		&& GET( m_iOtherUserUID )
		&& GET( m_iOtherUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_cRelationshipType )
		;
}

SERIALIZE_DEFINE_PUT( KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ, obj, ks )
{
	return PUT( m_iOtherUserUID )
		&& PUT( m_iOtherUnitUID )
		&& PUT( m_wstrRequestUnitName )
		&& PUT( m_iRequestUserUID )
		&& PUT( m_iRequestUnitUID )
		&& PUT( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_GET( KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ, obj, ks )
{
	return GET( m_iOtherUserUID )
		&& GET( m_iOtherUnitUID )
		&& GET( m_wstrRequestUnitName )
		&& GET( m_iRequestUserUID )
		&& GET( m_iRequestUnitUID )
		&& GET( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_PUT( KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KELG_COUPLE_PROPOSE_NOT, obj, ks )
{
	return PUT( m_wstrRequestUnitName )
		&& PUT( m_iRequestUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_COUPLE_PROPOSE_NOT, obj, ks )
{
	return GET( m_wstrRequestUnitName )
		&& GET( m_iRequestUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRequestUnitUID )
		&& PUT( m_wstrRequestUnitName )
		&& PUT( m_iAcceptUnitUID )
		&& PUT( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_GET( KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRequestUnitUID )
		&& GET( m_wstrRequestUnitName )
		&& GET( m_iAcceptUnitUID )
		&& GET( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_PUT( KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRequestUnitUID )
		&& PUT( m_wstrRequestUnitName )
		&& PUT( m_iAcceptUnitUID )
		&& PUT( m_wstrAcceptUnitName )
		&& PUT( m_tDate )
		;
}

SERIALIZE_DEFINE_GET( KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRequestUnitUID )
		&& GET( m_wstrRequestUnitName )
		&& GET( m_iAcceptUnitUID )
		&& GET( m_wstrAcceptUnitName )
		&& GET( m_tDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_COUPLE_MAKING_SUCCESS_REQ, obj, ks )
{
	return PUT( m_iRequestUnitUID )
		&& PUT( m_wstrRequestUnitName )
		&& PUT( m_iAcceptUserUID )
		&& PUT( m_ucAcceptUnitLevel )
		&& PUT( m_cAcceptUnitClass )
		&& PUT( m_iAcceptUnitUID )
		&& PUT( m_wstrAcceptUnitName )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_GET( KDBE_COUPLE_MAKING_SUCCESS_REQ, obj, ks )
{
	return GET( m_iRequestUnitUID )
		&& GET( m_wstrRequestUnitName )
		&& GET( m_iAcceptUserUID )
		&& GET( m_ucAcceptUnitLevel )
		&& GET( m_cAcceptUnitClass )
		&& GET( m_iAcceptUnitUID )
		&& GET( m_wstrAcceptUnitName )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_COUPLE_MAKING_SUCCESS_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRequestUnitUID )
		&& PUT( m_wstrRequestUnitName )
		&& PUT( m_iAcceptUserUID )
		&& PUT( m_ucAcceptUnitLevel )
		&& PUT( m_cAcceptUnitClass )
		&& PUT( m_iAcceptUnitUID )
		&& PUT( m_wstrAcceptUnitName )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_tDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_COUPLE_MAKING_SUCCESS_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRequestUnitUID )
		&& GET( m_wstrRequestUnitName )
		&& GET( m_iAcceptUserUID )
		&& GET( m_ucAcceptUnitLevel )
		&& GET( m_cAcceptUnitClass )
		&& GET( m_iAcceptUnitUID )
		&& GET( m_wstrAcceptUnitName )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_tDate )
		;
}

SERIALIZE_DEFINE_PUT( KERM_COUPLE_MAKING_SUCCESS_NOT, obj, ks )
{
	return PUT( m_iRequestUnitUID )
		&& PUT( m_wstrRequestUnitName )
		&& PUT( m_iAcceptUnitUID )
		&& PUT( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_GET( KERM_COUPLE_MAKING_SUCCESS_NOT, obj, ks )
{
	return GET( m_iRequestUnitUID )
		&& GET( m_wstrRequestUnitName )
		&& GET( m_iAcceptUnitUID )
		&& GET( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_PUT( KETR_COUPLE_MAKING_SUCCESS_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_wstrRequestUnitName )
		&& PUT( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_GET( KETR_COUPLE_MAKING_SUCCESS_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_wstrRequestUnitName )
		&& GET( m_wstrAcceptUnitName )
		;
}

SERIALIZE_DEFINE_PUT( KELG_RELATIONSHIP_INFO_REQ, obj, ks )
{
	return PUT( m_iRequestGSUID )
		&& PUT( m_iRequestUnitUID )
		&& PUT( m_iAcceptUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_RELATIONSHIP_INFO_REQ, obj, ks )
{
	return GET( m_iRequestGSUID )
		&& GET( m_iRequestUnitUID )
		&& GET( m_iAcceptUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KETR_RELATIONSHIP_EFFECT_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iManUID )
		&& PUT( m_iGirlUID )
		&& PUT( m_wstrLoveWord )
		&& PUT( m_iEffectIndex )
		;
}

SERIALIZE_DEFINE_GET( KETR_RELATIONSHIP_EFFECT_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iManUID )
		&& GET( m_iGirlUID )
		&& GET( m_wstrLoveWord )
		&& GET( m_iEffectIndex )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WEDDING_PROPOSE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iManUID )
		&& PUT( m_iGirlUID )
		&& PUT( m_cWeddingHallType )
		&& PUT( m_cOfficiantNPC )
		&& PUT( m_wstrWeddingDate )
		&& PUT( m_wstrWeddingMsg )
		&& PUT( m_wstrWeddingNickName )
		&& PUT( m_vecInviteUnitList )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WEDDING_PROPOSE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iManUID )
		&& GET( m_iGirlUID )
		&& GET( m_cWeddingHallType )
		&& GET( m_cOfficiantNPC )
		&& GET( m_wstrWeddingDate )
		&& GET( m_wstrWeddingMsg )
		&& GET( m_wstrWeddingNickName )
		&& GET( m_vecInviteUnitList )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WEDDING_PROPOSE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iWeddingUID )
		&& PUT( m_iManUID )
		&& PUT( m_iGirlUID )
		&& PUT( m_cWeddingHallType )
		&& PUT( m_cOfficiantNPC )
		&& PUT( m_wstrWeddingDate )
		&& PUT( m_wstrWeddingMsg )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WEDDING_PROPOSE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iWeddingUID )
		&& GET( m_iManUID )
		&& GET( m_iGirlUID )
		&& GET( m_cWeddingHallType )
		&& GET( m_cOfficiantNPC )
		&& GET( m_wstrWeddingDate )
		&& GET( m_wstrWeddingMsg )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KELG_ADD_WEDDING_HALL_INFO_NOT, obj, ks )
{
	return PUT( m_kWeddingHallInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADD_WEDDING_HALL_INFO_NOT, obj, ks )
{
	return GET( m_kWeddingHallInfo )
		;
}

SERIALIZE_DEFINE_PUT( KELG_WEDDING_ITEM_FIND_INFO_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iOK )
		&& PUT( m_mapWeddingItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_WEDDING_ITEM_FIND_INFO_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iOK )
		&& GET( m_mapWeddingItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WEDDING_ITEM_DELETE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapWeddingItemInfo )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WEDDING_ITEM_DELETE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapWeddingItemInfo )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WEDDING_ITEM_DELETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapWeddingItemInfo )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_kItemQuantityUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WEDDING_ITEM_DELETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapWeddingItemInfo )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_kItemQuantityUpdate )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ, obj, ks )
{
	return PUT( m_iMapID )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		&& PUT( m_iCalledUserUID )
		&& PUT( m_iCalledGSUID )
		&& PUT( m_iChannelID )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_cCallType )
		;
}

SERIALIZE_DEFINE_GET( KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ, obj, ks )
{
	return GET( m_iMapID )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		&& GET( m_iCalledUserUID )
		&& GET( m_iCalledGSUID )
		&& GET( m_iChannelID )
		&& GET( m_iLoverUnitUID )
		&& GET( m_cCallType )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT, obj, ks )
{
	return PUT( m_bAgree )
		&& PUT( m_iMapID )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		&& PUT( m_iCalledUserUID )
		&& PUT( m_iCalledGSUID )
		&& PUT( m_iMyUserUID )
		&& PUT( m_iMyGSUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT, obj, ks )
{
	return GET( m_bAgree )
		&& GET( m_iMapID )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		&& GET( m_iCalledUserUID )
		&& GET( m_iCalledGSUID )
		&& GET( m_iMyUserUID )
		&& GET( m_iMyGSUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, obj, ks )
{
	return PUT( m_bUseWarpItem )
		&& PUT( m_iMapID )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		;
}

SERIALIZE_DEFINE_GET( KELG_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, obj, ks )
{
	return GET( m_bUseWarpItem )
		&& GET( m_iMapID )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		;
}

SERIALIZE_DEFINE_PUT( KERM_CALL_MY_LOVER_LEAVE_FIELD_REQ, obj, ks )
{
	return PUT( m_iReason )
		&& PUT( m_iMapID )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		&& PUT( m_iLimitLevel )
		&& PUT( m_iRequireDungeonID )
		;
}

SERIALIZE_DEFINE_GET( KERM_CALL_MY_LOVER_LEAVE_FIELD_REQ, obj, ks )
{
	return GET( m_iReason )
		&& GET( m_iMapID )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		&& GET( m_iLimitLevel )
		&& GET( m_iRequireDungeonID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_CALL_MY_LOVER_LEAVE_FIELD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iReason )
		&& PUT( m_iMapID )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		&& PUT( m_iLimitLevel )
		&& PUT( m_iRequireDungeonID )
		;
}

SERIALIZE_DEFINE_GET( KERM_CALL_MY_LOVER_LEAVE_FIELD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iReason )
		&& GET( m_iMapID )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		&& GET( m_iLimitLevel )
		&& GET( m_iRequireDungeonID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_CALL_MY_LOVER_CHECK_ROOM_REQ, obj, ks )
{
	return PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_cCallType )
		&& PUT( m_iChannelID )
		&& PUT( m_iCalledUserUID )
		&& PUT( m_iCalledGSUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_CALL_MY_LOVER_CHECK_ROOM_REQ, obj, ks )
{
	return GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		&& GET( m_iLoverUnitUID )
		&& GET( m_cCallType )
		&& GET( m_iChannelID )
		&& GET( m_iCalledUserUID )
		&& GET( m_iCalledGSUID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_CALL_MY_LOVER_CHECK_ROOM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kBTInfo )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_cCallType )
		;
}

SERIALIZE_DEFINE_GET( KERM_CALL_MY_LOVER_CHECK_ROOM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kBTInfo )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		&& GET( m_iLoverUnitUID )
		&& GET( m_cCallType )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kBTInfo )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_cCallType )
		&& PUT( m_iChannelID )
		&& PUT( m_iCalledUserUID )
		&& PUT( m_iCalledGSUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kBTInfo )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		&& GET( m_iLoverUnitUID )
		&& GET( m_cCallType )
		&& GET( m_iChannelID )
		&& GET( m_iCalledUserUID )
		&& GET( m_iCalledGSUID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT, obj, ks )
{
	return PUT( m_bAgree )
		&& PUT( m_iCalledUserUID )
		&& PUT( m_iCalledGSUID )
		&& PUT( m_iMyUserUID )
		&& PUT( m_iMyGSUID )
		&& PUT( m_iBattleFieldID )
		&& PUT( m_RoomUID )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		;
}

SERIALIZE_DEFINE_GET( KELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT, obj, ks )
{
	return GET( m_bAgree )
		&& GET( m_iCalledUserUID )
		&& GET( m_iCalledGSUID )
		&& GET( m_iMyUserUID )
		&& GET( m_iMyGSUID )
		&& GET( m_iBattleFieldID )
		&& GET( m_RoomUID )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT, obj, ks )
{
	return PUT( m_iCalledUserUID )
		&& PUT( m_iCalledUnitUID )
		&& PUT( m_iCalledGSUID )
		&& PUT( m_iBattleFieldID )
		&& PUT( m_RoomUID )
		&& PUT( m_vPos )
		&& PUT( m_LastTouchLineIndex )
		;
}

SERIALIZE_DEFINE_GET( KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT, obj, ks )
{
	return GET( m_iCalledUserUID )
		&& GET( m_iCalledUnitUID )
		&& GET( m_iCalledGSUID )
		&& GET( m_iBattleFieldID )
		&& GET( m_RoomUID )
		&& GET( m_vPos )
		&& GET( m_LastTouchLineIndex )
		;
}

SERIALIZE_DEFINE_PUT( KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ, obj, ks )
{
	return PUT( m_iCalledUserUID )
		&& PUT( m_iCalledUnitUID )
		&& PUT( m_iCalledGSUID )
		&& PUT( m_RoomUID )
		&& PUT( m_vPos )
		&& PUT( m_kRoomUserInfo )
		&& PUT( m_vecStudentUnitUID )
		&& PUT( m_kBattleFieldJoinInfo )
		&& PUT( m_kReturnToFieldInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_CALL_MY_LOVER_JOIN_BATTLE_FIELD_REQ, obj, ks )
{
	return GET( m_iCalledUserUID )
		&& GET( m_iCalledUnitUID )
		&& GET( m_iCalledGSUID )
		&& GET( m_RoomUID )
		&& GET( m_vPos )
		&& GET( m_kRoomUserInfo )
		&& GET( m_vecStudentUnitUID )
		&& GET( m_kBattleFieldJoinInfo )
		&& GET( m_kReturnToFieldInfo )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCalledUserUID )
		&& PUT( m_iCalledGSUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCalledUserUID )
		&& GET( m_iCalledGSUID )
		;
}

SERIALIZE_DEFINE_PUT( KERM_WEDDING_HALL_INFO_NOT, obj, ks )
{
	return PUT( m_cType )
		&& PUT( m_kHallInfo )
		&& PUT( m_vecJoinUnitList )
		;
}

SERIALIZE_DEFINE_GET( KERM_WEDDING_HALL_INFO_NOT, obj, ks )
{
	return GET( m_cType )
		&& GET( m_kHallInfo )
		&& GET( m_vecJoinUnitList )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CHECK_WEDDING_HALL_INFO_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iGSUserUID )
		&& PUT( m_iWeddingUID )
		&& PUT( m_cUnitWeddingStatus )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHECK_WEDDING_HALL_INFO_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iGSUserUID )
		&& GET( m_iWeddingUID )
		&& GET( m_cUnitWeddingStatus )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CHECK_WEDDING_HALL_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kWeddingInfo )
		&& PUT( m_cUnitWeddingStatus )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHECK_WEDDING_HALL_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kWeddingInfo )
		&& GET( m_cUnitWeddingStatus )
		;
}

SERIALIZE_DEFINE_PUT( KERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ, obj, ks )
{
	return PUT( m_kWeddingInfo )
		&& PUT( m_cUnitWeddingStatus )
		;
}

SERIALIZE_DEFINE_GET( KERM_LEAVE_BATTLE_FIELD_FOR_JOIN_WEDDING_HALL_REQ, obj, ks )
{
	return GET( m_kWeddingInfo )
		&& GET( m_cUnitWeddingStatus )
		;
}

SERIALIZE_DEFINE_PUT( KERM_OPEN_WEDDING_HALL_REQ, obj, ks )
{
	return PUT( m_kWeddingInfo )
		&& PUT( m_kRoomUserInfo )
		&& PUT( m_kReturnToFieldInfo )
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_WEDDING_HALL_REQ, obj, ks )
{
	return GET( m_kWeddingInfo )
		&& GET( m_kRoomUserInfo )
		&& GET( m_kReturnToFieldInfo )
		;
}

SERIALIZE_DEFINE_PUT( KERM_OPEN_WEDDING_HALL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kWeddingInfo )
		&& PUT( m_RoomInfo )
		&& PUT( m_vecSlot )
		&& PUT( m_wstrCNIP )
		&& PUT( m_wstrUDPRelayIP )
		&& PUT( m_usUDPRelayPort )
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_WEDDING_HALL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kWeddingInfo )
		&& GET( m_RoomInfo )
		&& GET( m_vecSlot )
		&& GET( m_wstrCNIP )
		&& GET( m_wstrUDPRelayIP )
		&& GET( m_usUDPRelayPort )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_BREAK_UP_REQ, obj, ks )
{
	return PUT( m_iMyUnitUID )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_iCost )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BREAK_UP_REQ, obj, ks )
{
	return GET( m_iMyUnitUID )
		&& GET( m_iLoverUnitUID )
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_BREAK_UP_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iMyUnitUID )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_iCost )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BREAK_UP_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iMyUnitUID )
		&& GET( m_iLoverUnitUID )
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KELG_BREAK_UP_NOT, obj, ks )
{
	return PUT( m_iLoverUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_BREAK_UP_NOT, obj, ks )
{
	return GET( m_iLoverUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WEDDING_COMPLETE_REQ, obj, ks )
{
	return PUT( m_iWeddingUID )
		&& PUT( m_iGroom )
		&& PUT( m_iBride )
		&& PUT( m_wstrWeddingDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WEDDING_COMPLETE_REQ, obj, ks )
{
	return GET( m_iWeddingUID )
		&& GET( m_iGroom )
		&& GET( m_iBride )
		&& GET( m_wstrWeddingDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WEDDING_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrWeddingDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WEDDING_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrWeddingDate )
		;
}

SERIALIZE_DEFINE_PUT( KERM_START_WEDDING_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGroom )
		&& PUT( m_iBride )
		&& PUT( m_mapRewardItem )
		;
}

SERIALIZE_DEFINE_GET( KERM_START_WEDDING_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGroom )
		&& GET( m_iBride )
		&& GET( m_mapRewardItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_LOVE_WORD_REQ, obj, ks )
{
	return PUT( m_iUseUnitUID )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_wstrNewLoveWord )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_kItemQuantityUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_LOVE_WORD_REQ, obj, ks )
{
	return GET( m_iUseUnitUID )
		&& GET( m_iLoverUnitUID )
		&& GET( m_wstrNewLoveWord )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_kItemQuantityUpdate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_LOVE_WORD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUseUnitUID )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_wstrNewLoveWord )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_LOVE_WORD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUseUnitUID )
		&& GET( m_iLoverUnitUID )
		&& GET( m_wstrNewLoveWord )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CHANGE_LOVE_WORD_NOT, obj, ks )
{
	return PUT( m_iUseUnitUID )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_wstrNewLoveWord )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHANGE_LOVE_WORD_NOT, obj, ks )
{
	return GET( m_iUseUnitUID )
		&& GET( m_iLoverUnitUID )
		&& GET( m_wstrNewLoveWord )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_WEDDING_REWARD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iLoverUnitUID )
		&& PUT( m_bTitleReward )
		&& PUT( m_iTitleRewardStep )
		&& PUT( kPostReq )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_WEDDING_REWARD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iLoverUnitUID )
		&& GET( m_bTitleReward )
		&& GET( m_iTitleRewardStep )
		&& GET( kPostReq )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_WEDDING_REWARD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bTitleReward )
		&& PUT( m_iTitleRewardStep )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_WEDDING_REWARD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bTitleReward )
		&& GET( m_iTitleRewardStep )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ADMIN_CHANGE_COUPLE_DATE_REQ, obj, ks )
{
	return PUT( m_fAddRate )
		&& PUT( m_iUnitUID )
		&& PUT( m_iLoverUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_CHANGE_COUPLE_DATE_REQ, obj, ks )
{
	return GET( m_fAddRate )
		&& GET( m_iUnitUID )
		&& GET( m_iLoverUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_ucLevel )
		&& PUT( m_cUnitClass )
		&& PUT( m_cCoupleType )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WRITE_RELATIONSHIP_INFO_LOG_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_ucLevel )
		&& GET( m_cUnitClass )
		&& GET( m_cCoupleType )
		&& GET( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_WRITE_RELATIONSHIP_INFO_LOG_ACK, obj, ks )
{
	return PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KDBE_WRITE_RELATIONSHIP_INFO_LOG_ACK, obj, ks )
{
	return GET( m_iOK )
		;
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2
SERIALIZE_DEFINE_PUT( KECN_SET_ACTIVE_LAGCHECK_NOT, obj, ks )
{
	return PUT( m_bActiveLagCheck )
		;
}

SERIALIZE_DEFINE_GET( KECN_SET_ACTIVE_LAGCHECK_NOT, obj, ks )
{
	return GET( m_bActiveLagCheck )
		;
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK_VER2

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
SERIALIZE_DEFINE_PUT( KDBE_GET_RIDING_PET_LIST_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_RIDING_PET_LIST_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_RIDING_PET_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iAfterWorkStorageKey )
		&& PUT( m_vecRidingPetList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_RIDING_PET_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iAfterWorkStorageKey )
		&& GET( m_vecRidingPetList )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CREATE_RIDING_PET_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_usCreatePetID )
		&& PUT( m_sPeriod )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_RIDING_PET_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_usCreatePetID )
		&& GET( m_sPeriod )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CREATE_RIDING_PET_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kCreatedPetInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CREATE_RIDING_PET_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kCreatedPetInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RELEASE_RIDING_PET_REQ, obj, ks )
{
	return PUT( m_iRidingPetUID )
		&& PUT( m_iUnitUID )
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
		&& PUT( m_bEventQuest)
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST
		;
}

SERIALIZE_DEFINE_GET( KDBE_RELEASE_RIDING_PET_REQ, obj, ks )
{
	return GET( m_iRidingPetUID )
		&& GET( m_iUnitUID )
#ifdef SERV_EVENT_RIDING_WITH_SUB_QUEST
		&& GET( m_bEventQuest)
#endif //SERV_EVENT_RIDING_WITH_SUB_QUEST
		;
}

SERIALIZE_DEFINE_PUT( KETR_SUMMON_RIDING_PET_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iRidingPetUID )
		&& PUT( m_usRidingPetID )
		;
}

SERIALIZE_DEFINE_GET( KETR_SUMMON_RIDING_PET_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_iRidingPetUID )
		&& GET( m_usRidingPetID )
		;
}

SERIALIZE_DEFINE_PUT( KETR_UNSUMMON_RIDING_PET_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KETR_UNSUMMON_RIDING_PET_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		;
}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT// 작업날짜: 2013-05-12	// 박세훈
SERIALIZE_DEFINE_PUT( KDBE_SEND_LOVE_LETTER_EVENT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrMessage )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SEND_LOVE_LETTER_EVENT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrMessage )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SEND_LOVE_LETTER_EVENT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SEND_LOVE_LETTER_EVENT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
#endif // SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG// 작업날짜: 2013-05-15	// 박세훈
SERIALIZE_DEFINE_PUT( KDBE_LOCAL_RANKING_SYSTEM_STATISTICS_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_ucLevel )
		&& PUT( m_bLocalRankingUser )
		&& PUT( m_usLocalRankingClickCount )
		;
}

SERIALIZE_DEFINE_GET( KDBE_LOCAL_RANKING_SYSTEM_STATISTICS_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_ucLevel )
		&& GET( m_bLocalRankingUser )
		&& GET( m_usLocalRankingClickCount )
		;
}
#endif // SERV_LOCAL_RANKING_SYSTEM_STATISTICS_LOG

#ifdef SERV_DUNGEON_STAGE_LOAD_LOG// 작업날짜: 2013-05-15	// 박세훈
SERIALIZE_DEFINE_PUT( DungonStageLoadLog, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cNormalStage )
		&& PUT( m_cSecretStage )
		&& PUT( m_cEliteStage )
		&& PUT( m_cClearConditionCheckCount )
		&& PUT( m_iDungeonID )
		&& PUT( m_iNowStageID )
		&& PUT( m_iReqStageID )
		&& PUT( m_bHost )
		&& PUT( m_cMemberNum )
		;
}

SERIALIZE_DEFINE_GET( DungonStageLoadLog, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cNormalStage )
		&& GET( m_cSecretStage )
		&& GET( m_cEliteStage )
		&& GET( m_cClearConditionCheckCount )
		&& GET( m_iDungeonID )
		&& GET( m_iNowStageID )
		&& GET( m_iReqStageID )
		&& GET( m_bHost )
		&& GET( m_cMemberNum )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DUNGEON_STAGE_LOAD_LOG_NOT, obj, ks )
{
	return PUT( m_iRoomUID )
		&& PUT( m_vecDungonStageLoadLog )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DUNGEON_STAGE_LOAD_LOG_NOT, obj, ks )
{
	return GET( m_iRoomUID )
		&& GET( m_vecDungonStageLoadLog )
		;
}
#endif // SERV_DUNGEON_STAGE_LOAD_LOG


//{{ 2013. 05. 10	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
SERIALIZE_DEFINE_PUT( KDBE_ITEM_EVALUATE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iCost )
		&& PUT( m_vecRandomSocket )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EVALUATE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_iCost )
		&& GET( m_vecRandomSocket )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_ITEM_EVALUATE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_iCost )
		&& PUT( m_vecRandomSocket )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EVALUATE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_iCost )
		&& GET( m_vecRandomSocket )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_RESTORE_ITEM_EVALUATE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iTargetItemUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESTORE_ITEM_EVALUATE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iTargetItemUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RESTORE_ITEM_EVALUATE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iTargetItemUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESTORE_ITEM_EVALUATE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iTargetItemUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_ITEM_CONVERT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iCommissionED )
		&& PUT( m_vecItemInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_CONVERT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iCommissionED )
		&& GET( m_vecItemInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ITEM_CONVERT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCommissionED )
		&& PUT( m_mapItemInfo )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_CONVERT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCommissionED )
		&& GET( m_mapItemInfo )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}



#ifdef SERV_FIELD_WORKINGS_BLOCK_LOG// 작업날짜: 2013-05-15	// 박세훈
SERIALIZE_DEFINE_PUT( KDBE_FIELD_WORKINGS_BLOCK_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cX2State )
		&& PUT( m_iPhysicalMemory )
		&& PUT( m_iVirtualMemory )
		&& PUT( m_cProcessTerm )
		&& PUT( m_cFieldUserNum )
		&& PUT( m_cFieldNpcNum )
		;
}

SERIALIZE_DEFINE_GET( KDBE_FIELD_WORKINGS_BLOCK_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cX2State )
		&& GET( m_iPhysicalMemory )
		&& GET( m_iVirtualMemory )
		&& GET( m_cProcessTerm )
		&& GET( m_cFieldUserNum )
		&& GET( m_cFieldNpcNum )
		;
}
#endif // SERV_FIELD_WORKINGS_BLOCK_LOG

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-22	// 박세훈
SERIALIZE_DEFINE_PUT( KDBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT, obj, ks )
{
	return PUT( m_byteBlockCheckCount )
		&& PUT( m_wstrBlockCheckResetDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT, obj, ks )
{
	return GET( m_byteBlockCheckCount )
		&& GET( m_wstrBlockCheckResetDate )
		;
}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_TIME_ENCHANT_EVENT// 작업날짜: 2013-05-28	// 박세훈
SERIALIZE_DEFINE_PUT( KTimeEnchantEventInfo, obj, ks )
{
	return PUT( m_iEventUID )
		&& PUT( m_tBeginDate )
		&& PUT( m_tEndDate )
		&& PUT( m_cEnchantLevel )
		;
}

SERIALIZE_DEFINE_GET( KTimeEnchantEventInfo, obj, ks )
{
	return GET( m_iEventUID )
		&& GET( m_tBeginDate )
		&& GET( m_tEndDate )
		&& GET( m_cEnchantLevel )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_TIME_ENCHANT_EVENT_INFO_ACK, obj, ks )
{
	return PUT( m_vecTimeEnchantEventInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_TIME_ENCHANT_EVENT_INFO_ACK, obj, ks )
{
	return GET( m_vecTimeEnchantEventInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_TIME_ENCHANT_EVENT_INFO_NOT, obj, ks )
{
	return PUT( m_iTimeEnchantEventUID )
		&& PUT( m_kTimeEnchantEventInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_TIME_ENCHANT_EVENT_INFO_NOT, obj, ks )
{
	return GET( m_iTimeEnchantEventUID )
		&& GET( m_kTimeEnchantEventInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_TIME_ENCHANT_EVENT_INFO_NOT, obj, ks )
{
	return PUT( m_iTimeEnchantEventUID )
		&& PUT( m_iNumOfTimeEnchantEvent )
		;
}

SERIALIZE_DEFINE_GET( KEGB_TIME_ENCHANT_EVENT_INFO_NOT, obj, ks )
{
	return GET( m_iTimeEnchantEventUID )
		&& GET( m_iNumOfTimeEnchantEvent )
		;
}

SERIALIZE_DEFINE_PUT( KOpenRandomItemStorage, obj, ks )
{
	return PUT( m_iRessurectionCount )
		&& PUT( m_iRestoreSpirit )
		&& PUT( m_iRandomItemID )
		&& PUT( m_bIsCharmItem )
		&& PUT( m_iSpendED )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KOpenRandomItemStorage, obj, ks )
{
	return GET( m_iRessurectionCount )
		&& GET( m_iRestoreSpirit )
		&& GET( m_iRandomItemID )
		&& GET( m_bIsCharmItem )
		&& GET( m_iSpendED )
		&& GET( m_mapInsertedItem )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_TIME_ENCHANT_EVENT_NOT, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iAfterWorkStorageKey )
		&& PUT( m_usEventID )
		;
}

SERIALIZE_DEFINE_GET( KEGB_TIME_ENCHANT_EVENT_NOT, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iAfterWorkStorageKey )
		&& GET( m_usEventID )
		;
}

SERIALIZE_DEFINE_PUT( KTimeEnchantEventResult, obj, ks )
{
	return PUT( m_iAfterWorkStorageKey )
		&& PUT( m_cTimeEnchantEventLevel )
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
		&& PUT( m_bNotifyGetItem )
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
		;
}

SERIALIZE_DEFINE_GET( KTimeEnchantEventResult, obj, ks )
{
	return GET( m_iAfterWorkStorageKey )
		&& GET( m_cTimeEnchantEventLevel )
#ifdef SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
		&& GET( m_bNotifyGetItem )
#endif // SERV_CUBE_AUTO_OPEN_NOTIFY_OFF
		;
}

SERIALIZE_DEFINE_PUT( KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iTimeEnchantEventUID )
		&& PUT( m_kTimeEnchantEventInfo )
		&& PUT( m_iAfterWorkStorageKey )
		&& PUT( m_usEventID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iTimeEnchantEventUID )
		&& GET( m_kTimeEnchantEventInfo )
		&& GET( m_iAfterWorkStorageKey )
		&& GET( m_usEventID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iAfterWorkStorageKey )
		&& PUT( m_usEventID )
		&& PUT( m_cEnchantLevel )
		;
}

SERIALIZE_DEFINE_GET( KDBE_TIME_ENCHANT_EVENT_INFO_UPDATE_ACK, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iAfterWorkStorageKey )
		&& GET( m_usEventID )
		&& GET( m_cEnchantLevel )
		;
}
#endif // SERV_TIME_ENCHANT_EVENT

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
SERIALIZE_DEFINE_PUT( KDBE_HACKING_USER_CHECK_COUNT_INFO_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_byteType )
		&& PUT( m_wstrReason )
		;
}

SERIALIZE_DEFINE_GET( KDBE_HACKING_USER_CHECK_COUNT_INFO_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_byteType )
		&& GET( m_wstrReason )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_HACKING_USER_CHECK_COUNT_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_byteType )
		&& PUT( m_byteBlockCheckCount )
		&& PUT( m_wstrReason )
		&& PUT( m_wstrLastResetDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_HACKING_USER_CHECK_COUNT_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_byteType )
		&& GET( m_byteBlockCheckCount )
		&& GET( m_wstrReason )
		&& GET( m_wstrLastResetDate )
		;
}

SERIALIZE_DEFINE_PUT( KHackingUserCheckCountDB, obj, ks )
{
	return PUT( m_byteBlockCheckCount )
		&& PUT( m_wstrLastResetDate )
		;
}

SERIALIZE_DEFINE_GET( KHackingUserCheckCountDB, obj, ks )
{
	return GET( m_byteBlockCheckCount )
		&& GET( m_wstrLastResetDate )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_mapHackingUserCheckCountDB )
		;
}

SERIALIZE_DEFINE_GET( KDBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_mapHackingUserCheckCountDB )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ACCOUNT_BLOCK_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_byteType )
		&& PUT( m_wstrReason )
		&& PUT( m_wstrEndDate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ACCOUNT_BLOCK_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_byteType )
		&& GET( m_wstrReason )
		&& GET( m_wstrEndDate )
		;
}
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
SERIALIZE_DEFINE_PUT( KDBE_INSERT_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM

		&& PUT( m_mapSkillList )
		&& PUT( m_iCSPoint )
		&& PUT( m_iTotalSpendSkillPoint )
		&& PUT( m_iBeforeCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM

		&& GET( m_mapSkillList )
		&& GET( m_iCSPoint )
		&& GET( m_iTotalSpendSkillPoint )
		&& GET( m_iBeforeCSPoint )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK ) 

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM

		&& PUT( m_mapSkillList )
		&& PUT( m_iCSPoint )
		&& PUT( m_iTotalSpendSkillPoint )
		&& PUT( m_iBeforeCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM

		&& GET( m_mapSkillList )
		&& GET( m_iCSPoint )
		&& GET( m_iTotalSpendSkillPoint )
		&& GET( m_iBeforeCSPoint )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RESET_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iDelSkillID )
		&& PUT( m_iDelSkillLevel )
		&& PUT( m_iCSPoint )
		&& PUT( m_iBeforeSPoint )
		&& PUT( m_iBeforeCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iDelSkillID )
		&& GET( m_iDelSkillLevel )
		&& GET( m_iCSPoint )
		&& GET( m_iBeforeSPoint )
		&& GET( m_iBeforeCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RESET_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iDelSkillID )
		&& PUT( m_iDelSkillLevel )
		&& PUT( m_iCSPoint )
		&& PUT( m_iBeforeSPoint )
		&& PUT( m_iBeforeCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iDelSkillID )
		&& GET( m_iDelSkillLevel )
		&& GET( m_iCSPoint )
		&& GET( m_iBeforeSPoint )
		&& GET( m_iBeforeCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ADMIN_CHANGE_UNIT_CLASS_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
		&& PUT( m_iNewDefaultSkill3 )
		&& PUT( m_iNewDefaultSkill4 )
		&& PUT( m_iNewDefaultSkill5 )
		&& PUT( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_CHANGE_UNIT_CLASS_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
		&& GET( m_iNewDefaultSkill3 )
		&& GET( m_iNewDefaultSkill4 )
		&& GET( m_iNewDefaultSkill5 )
		&& GET( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ADMIN_CHANGE_UNIT_CLASS_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
		&& PUT( m_iNewDefaultSkill3 )
		&& PUT( m_iNewDefaultSkill4 )
		&& PUT( m_iNewDefaultSkill5 )
		&& PUT( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_CHANGE_UNIT_CLASS_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
		&& GET( m_iNewDefaultSkill3 )
		&& GET( m_iNewDefaultSkill4 )
		&& GET( m_iNewDefaultSkill5 )
		&& GET( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ADMIN_AUTO_GET_ALL_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_cUnitClass )
		&& PUT( m_iSPoint )
		&& PUT( m_iCSPoint )
		&& PUT( m_mapGetSkillList )
		&& PUT( m_vecUnsealedSkillID )
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
		&& PUT( m_iNewDefaultSkill3 )
		&& PUT( m_iNewDefaultSkill4 )
		&& PUT( m_iNewDefaultSkill5 )
		&& PUT( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_AUTO_GET_ALL_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_cUnitClass )
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint )
		&& GET( m_mapGetSkillList )
		&& GET( m_vecUnsealedSkillID )
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
		&& GET( m_iNewDefaultSkill3 )
		&& GET( m_iNewDefaultSkill4 )
		&& GET( m_iNewDefaultSkill5 )
		&& GET( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ADMIN_AUTO_GET_ALL_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_cUnitClass )
		&& PUT( m_iSPoint )
		&& PUT( m_iCSPoint )
		&& PUT( m_mapGetSkillList )
		&& PUT( m_vecUnsealedSkillID )
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
		&& PUT( m_iNewDefaultSkill3 )
		&& PUT( m_iNewDefaultSkill4 )
		&& PUT( m_iNewDefaultSkill5 )
		&& PUT( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_AUTO_GET_ALL_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_cUnitClass )
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint )
		&& GET( m_mapGetSkillList )
		&& GET( m_vecUnsealedSkillID )
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
		&& GET( m_iNewDefaultSkill3 )
		&& GET( m_iNewDefaultSkill4 )
		&& GET( m_iNewDefaultSkill5 )
		&& GET( m_iNewDefaultSkill6 )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ADMIN_GET_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iSkillID )
		&& PUT( m_iSkillLevel )
		&& PUT( m_iCSPoint )
		&& PUT( m_bUnsealed )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_GET_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iSkillID )
		&& GET( m_iSkillLevel )
		&& GET( m_iCSPoint )
		&& GET( m_bUnsealed )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ADMIN_GET_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iSkillID )
		&& PUT( m_iSkillLevel )
		&& PUT( m_iCSPoint )
		&& PUT( m_bUnsealed )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ADMIN_GET_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iSkillID )
		&& GET( m_iSkillLevel )
		&& GET( m_iCSPoint )
		&& GET( m_bUnsealed )
		;
}

#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
SERIALIZE_DEFINE_PUT( KDBE_RESET_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iSkillID )
		&& PUT( m_iSkillLevel )
		&& PUT( m_iSkillCSPoint )
		&& PUT( m_iCSPoint )
		//&& PUT( m_iRetrievedSPoint )
		//&& PUT( m_iRetrievedCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iSkillID )
		&& GET( m_iSkillLevel )
		&& GET( m_iSkillCSPoint )
		&& GET( m_iCSPoint )
		//&& GET( m_iRetrievedSPoint )
		//&& GET( m_iRetrievedCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RESET_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iSkillID )
		&& PUT( m_iSkillLevel )
		&& PUT( m_iSkillCSPoint )
		&& PUT( m_iCSPoint )
		//&& PUT( m_iRetrievedSPoint )
		//&& PUT( m_iRetrievedCSPoint )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESET_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iSkillID )
		&& GET( m_iSkillLevel )
		&& GET( m_iSkillCSPoint )
		&& GET( m_iCSPoint )
		//&& GET( m_iRetrievedSPoint )
		//&& GET( m_iRetrievedCSPoint )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-17	// 박세훈
SERIALIZE_DEFINE_PUT( KSOAP_COUPON_LIST_REQ, obj, ks )
{
	return PUT( m_bUsed )
		&& PUT( m_strNexonID )
		&& PUT( m_sCouponBoxType )
		&& PUT( m_iStartRowIndex )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_COUPON_LIST_REQ, obj, ks )
{
	return GET( m_bUsed )
		&& GET( m_strNexonID )
		&& GET( m_sCouponBoxType )
		&& GET( m_iStartRowIndex )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_COUPON_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bUsed )
		&& PUT( m_sCouponBoxType )
		&& PUT( m_iStartRowIndex )
		&& PUT( m_iCouponCount )
		&& PUT( m_vecCouponList )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_COUPON_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bUsed )
		&& GET( m_sCouponBoxType )
		&& GET( m_iStartRowIndex )
		&& GET( m_iCouponCount )
		&& GET( m_vecCouponList )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_REGIST_COUPON_REQ, obj, ks )
{
	return PUT( m_strCouponPin )
		&& PUT( m_strNexonID )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_REGIST_COUPON_REQ, obj, ks )
{
	return GET( m_strCouponPin )
		&& GET( m_strNexonID )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_ITEM_COUPON_USING_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulRemoteIP )
		&& PUT( m_byteReason )
		&& PUT( m_wstrGameID )
		&& PUT( m_byteIsCafe )
		&& PUT( m_wstrUserID )
		&& PUT( m_ulUserOID )
		&& PUT( m_wstrUserName )
		&& PUT( m_iCouponCardNo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_ITEM_COUPON_USING_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulRemoteIP )
		&& GET( m_byteReason )
		&& GET( m_wstrGameID )
		&& GET( m_byteIsCafe )
		&& GET( m_wstrUserID )
		&& GET( m_ulUserOID )
		&& GET( m_wstrUserName )
		&& GET( m_iCouponCardNo )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_ITEM_COUPON_USING_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_wstrCouponTXID )
		&& PUT( m_ulResult )
		&& PUT( m_ulOrderNo )
		&& PUT( m_vecCouponProductInfo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_ITEM_COUPON_USING_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_wstrCouponTXID )
		&& GET( m_ulResult )
		&& GET( m_ulOrderNo )
		&& GET( m_vecCouponProductInfo )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_COUPON_TARGET_ITEM_REQ, obj, ks )
{
	return PUT( m_iCouponCardNo )
		&& PUT( m_iStartRowIndex )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_COUPON_TARGET_ITEM_REQ, obj, ks )
{
	return GET( m_iCouponCardNo )
		&& GET( m_iStartRowIndex )
		;
}

SERIALIZE_DEFINE_PUT( KSOAP_COUPON_TARGET_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCouponCardNo )
		&& PUT( m_iStartRowIndex )
		&& PUT( m_iTargetItemCount )
		&& PUT( m_vecTargetItem )
		;
}

SERIALIZE_DEFINE_GET( KSOAP_COUPON_TARGET_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCouponCardNo )
		&& GET( m_iStartRowIndex )
		&& GET( m_iTargetItemCount )
		&& GET( m_vecTargetItem )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulRemoteIP )
		&& PUT( m_wstrUserID )
		&& PUT( m_ulUserOID )
		&& PUT( m_ulProductNo )
		&& PUT( m_usOrderQuantity )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulRemoteIP )
		&& GET( m_wstrUserID )
		&& GET( m_ulUserOID )
		&& GET( m_ulProductNo )
		&& GET( m_usOrderQuantity )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		&& PUT( m_vecDiscountCoupon )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		&& GET( m_vecDiscountCoupon )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulRemoteIP )
		&& PUT( m_wstrUserID )
		&& PUT( m_ulUserOID )
		&& PUT( m_vecDiscountCouponInquiryInfo )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_REQ, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulRemoteIP )
		&& GET( m_wstrUserID )
		&& GET( m_ulUserOID )
		&& GET( m_vecDiscountCouponInquiryInfo )
		;
}

SERIALIZE_DEFINE_PUT( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK, obj, ks )
{
	return PUT( m_ulPacketNo )
		&& PUT( m_bytePacketType )
		&& PUT( m_ulResult )
		&& PUT( m_vecDiscountCoupon )
		;
}

SERIALIZE_DEFINE_GET( KENX_BT_NISMS_DISCOUNT_COUPON_INQUIRY_LIST_ACK, obj, ks )
{
	return GET( m_ulPacketNo )
		&& GET( m_bytePacketType )
		&& GET( m_ulResult )
		&& GET( m_vecDiscountCoupon )
		;
}
#endif // SERV_NEXON_COUPON_SYSTEM

#ifdef SERV_ITEM_EXCHANGE_LIMIT// 작업날짜: 2013-06-29	// 박세훈
SERIALIZE_DEFINE_PUT( KExchangeLimitInfo, obj, ks )
{
	return PUT( m_iLimitUID )
		&& PUT( m_tBeginDate )
		&& PUT( m_tEndDate )
		&& PUT( m_setDestItemID )
		&& PUT( m_iLimitQuantity )
		;
}

SERIALIZE_DEFINE_GET( KExchangeLimitInfo, obj, ks )
{
	return GET( m_iLimitUID )
		&& GET( m_tBeginDate )
		&& GET( m_tEndDate )
		&& GET( m_setDestItemID )
		&& GET( m_iLimitQuantity )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXCHANGE_LIMIT_INFO_ACK, obj, ks )
{
	return PUT( m_listExchangeLimitInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXCHANGE_LIMIT_INFO_ACK, obj, ks )
{
	return GET( m_listExchangeLimitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXCHANGE_LIMIT_INFO_NOT, obj, ks )
{
	return PUT( m_iExchangeLimitUID )
		&& PUT( m_iLimitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXCHANGE_LIMIT_INFO_NOT, obj, ks )
{
	return GET( m_iExchangeLimitUID )
		&& GET( m_iLimitUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_EXCHANGE_LIMIT_INFO_NOT, obj, ks )
{
	return PUT( m_iExchangeLimitUID )
		&& PUT( m_mapExchangeLimitInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGB_EXCHANGE_LIMIT_INFO_NOT, obj, ks )
{
	return GET( m_iExchangeLimitUID )
		&& GET( m_mapExchangeLimitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_EXCHANGE_LIMIT_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iAfterWorkStorageKey )
		&& PUT( m_iDestItemID )
		;
}

SERIALIZE_DEFINE_GET( KEGB_EXCHANGE_LIMIT_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iAfterWorkStorageKey )
		&& GET( m_iDestItemID )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_EXCHANGE_LIMIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iExchangeLimitUID )
		&& PUT( m_iLimitUID )
		&& PUT( m_iRollBackUID )
		&& PUT( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_GET( KEGB_EXCHANGE_LIMIT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iExchangeLimitUID )
		&& GET( m_iLimitUID )
		&& GET( m_iRollBackUID )
		&& GET( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ, obj, ks )
{
	return PUT( m_iExchangeLimitUID )
		&& PUT( m_iLimitUID )
		&& PUT( m_iDestItemID )
		&& PUT( m_wstrRegDate )
		&& PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXCHANGE_LIMIT_INFO_UPDATE_REQ, obj, ks )
{
	return GET( m_iExchangeLimitUID )
		&& GET( m_iLimitUID )
		&& GET( m_iDestItemID )
		&& GET( m_wstrRegDate )
		&& GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iExchangeLimitUID )
		&& PUT( m_iLimitUID )
		&& PUT( m_iRollBackUID )
		&& PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXCHANGE_LIMIT_INFO_UPDATE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iExchangeLimitUID )
		&& GET( m_iLimitUID )
		&& GET( m_iRollBackUID )
		&& GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iAfterWorkStorageKey )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iExchangeLimitUID )
		&& PUT( m_iLimitUID )
		&& PUT( m_iRollBackUID )
		;
}

SERIALIZE_DEFINE_GET( KEGB_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iExchangeLimitUID )
		&& GET( m_iLimitUID )
		&& GET( m_iRollBackUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT, obj, ks )
{
	return PUT( m_iRollBackUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXCHANGE_LIMIT_INFO_ROLLBACK_NOT, obj, ks )
{
	return GET( m_iRollBackUID )
		;
}
#endif // SERV_ITEM_EXCHANGE_LIMIT

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈
SERIALIZE_DEFINE_PUT( KDBE_JUMPING_CHARACTER_INFO_NOT, obj, ks )
{
	return PUT( m_mapJumpingInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_JUMPING_CHARACTER_INFO_NOT, obj, ks )
{
	return GET( m_mapJumpingInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_JUMPING_CHARACTER_UPDATE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iLevel )
		&& PUT( m_cExpandedMaxPageNum )
		&& PUT( m_iClass )
		&& PUT( m_vecNewDefaultSkill )
		&& PUT( m_vecUnSealedSkill )
		&& PUT( m_wstrRegDate )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM

		;
}

SERIALIZE_DEFINE_GET( KDBE_JUMPING_CHARACTER_UPDATE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iLevel )
		&& GET( m_cExpandedMaxPageNum )
		&& GET( m_iClass )
		&& GET( m_vecNewDefaultSkill )
		&& GET( m_vecUnSealedSkill )
		&& GET( m_wstrRegDate )

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM

		;
}

SERIALIZE_DEFINE_PUT( KDBE_JUMPING_CHARACTER_UPDATE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iLevel )
		&& PUT( m_cExpandedMaxPageNum )
		&& PUT( m_iClass )
		&& PUT( m_vecNewDefaultSkill )
		&& PUT( m_vecUnSealedSkill )
		;
}

SERIALIZE_DEFINE_GET( KDBE_JUMPING_CHARACTER_UPDATE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iLevel )
		&& GET( m_cExpandedMaxPageNum )
		&& GET( m_iClass )
		&& GET( m_vecNewDefaultSkill )
		&& GET( m_vecUnSealedSkill )
		;
}
#endif // SERV_JUMPING_CHARACTER

#ifdef SERV_FINALITY_SKILL_SYSTEM	// 적용날짜: 2013-08-01
SERIALIZE_DEFINE_PUT( KDBE_ITEM_EXTRACT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_mapResultItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EXTRACT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_mapResultItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ITEM_EXTRACT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_mapResultItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ITEM_EXTRACT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_mapResultItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_USE_FINALITY_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_USE_FINALITY_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_USE_FINALITY_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_USE_FINALITY_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
#endif // SERV_FINALITY_SKILL_SYSTEM

#ifdef SERV_HENIR_CLEAR_TIME_HACK_USER_CHECK
SERIALIZE_DEFINE_PUT( KHenirHackUserData, obj, ks )
{
    return PUT ( m_iUserUID ) 
        && PUT( m_iUnitUID )
        && PUT( m_iPlayTime )
        && PUT (m_iMemberNum )
        ;
}

SERIALIZE_DEFINE_GET( KHenirHackUserData, obj, ks )
{
    return GET ( m_iUserUID ) 
        && GET( m_iUnitUID )
        && GET( m_iPlayTime )
        && GET (m_iMemberNum )
        ;
}

SERIALIZE_DEFINE_PUT( KELOG_HENIR_HACKING_LOG_NOT, obj, ks )
{
    return PUT ( m_vecHackUser );
}

SERIALIZE_DEFINE_GET( KELOG_HENIR_HACKING_LOG_NOT, obj, ks )
{
    return GET ( m_vecHackUser );
}
#endif

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-10-28	// 박세훈
SERIALIZE_DEFINE_PUT( KEGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT, obj, ks )
{
	return PUT( m_iBattleFieldID ) 
		&& PUT( m_byteIncreasedValue )
		;
}

SERIALIZE_DEFINE_GET( KEGB_UPDATE_TOTAL_DANGEROUS_VALUE_NOT, obj, ks )
{
	return GET( m_iBattleFieldID ) 
		&& GET( m_byteIncreasedValue )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_BATTLE_FIELD_BOSS_INFO_NOT, obj, ks )
{
	return PUT( m_tRemainPortalTime )
		&& PUT( m_tRemainCoolTime )
		&& PUT( m_iPortalAppearanceMap )
		&& PUT( m_iPortalDestination )
		&& PUT( m_bPortalOpen )
		&& PUT( m_bCoolTime )
		;
}

SERIALIZE_DEFINE_GET( KEGB_BATTLE_FIELD_BOSS_INFO_NOT, obj, ks )
{
	return GET( m_tRemainPortalTime )
		&& GET( m_tRemainCoolTime )
		&& GET( m_iPortalAppearanceMap )
		&& GET( m_iPortalDestination )
		&& GET( m_bPortalOpen )
		&& GET( m_bCoolTime )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SOCKET_EXPAND_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iItemUID )
		&& PUT( m_vecItemSocket )
		&& PUT( m_byteExpandedSocketNum )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SOCKET_EXPAND_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iItemUID )
		&& GET( m_vecItemSocket )
		&& GET( m_byteExpandedSocketNum )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SOCKET_EXPAND_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iItemUID )
		&& PUT( m_byteExpandedSocketNum )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SOCKET_EXPAND_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iItemUID )
		&& GET( m_byteExpandedSocketNum )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_BOSS_FIELD_LOG_NOT, obj, ks )
{
	return PUT( m_kFieldLog )
		&& PUT( m_mapUserLog )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BOSS_FIELD_LOG_NOT, obj, ks )
{
	return GET( m_kFieldLog )
		&& GET( m_mapUserLog )
		;
}
#endif // SERV_BATTLE_FIELD_BOSS

#ifdef SERV_GOOD_ELSWORD
SERIALIZE_DEFINE_PUT( KELOG_EXPAND_BANK_INVENTORY_NOT, obj, ks )
{
    return PUT( m_iUserUID )
        && PUT( m_iUnitUID )
        && PUT( m_iUnitLevel )
        && PUT( m_iBeforeED )
        && PUT( m_iUseED )
        && PUT( m_iCategory )
        && PUT( m_iNumSlot )
        ;
}

SERIALIZE_DEFINE_GET( KELOG_EXPAND_BANK_INVENTORY_NOT, obj, ks )
{
    return GET( m_iUserUID )
        && GET( m_iUnitUID )
        && GET( m_iUnitLevel )
        && GET( m_iBeforeED )
        && GET( m_iUseED )
        && GET( m_iCategory )
        && GET( m_iNumSlot )
        ;
}
#endif // SERV_GOOD_ELSWORD

SERIALIZE_DEFINE_PUT( KFieldBossGenKillStat, obj, ks )
{
    return PUT( m_nFieldID )
        //&& PUT( m_nNumGenBoss )
        //&& PUT( m_nNumDieBoss )
        //&& PUT( m_nNumDropItem )
        && PUT( m_nNumUser )
        //&& PUT( m_nDieNPCUID )
        && PUT( m_nSpawnID )
        ;
}

SERIALIZE_DEFINE_GET( KFieldBossGenKillStat, obj, ks )
{
    return GET( m_nFieldID )
        //&& GET( m_nNumGenBoss )
        //&& GET( m_nNumDieBoss )
        //&& GET( m_nNumDropItem )
        && GET( m_nNumUser )
        //&& GET( m_nDieNPCUID )
        && GET( m_nSpawnID )
        ;
}

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
SERIALIZE_DEFINE_PUT( KELOG_KOM_FILE_CHECK_LOG_NOT, obj, ks )
{
	return PUT( m_wstrInvaildKomName )
		&& PUT( m_iUserUID )
		;
}

SERIALIZE_DEFINE_GET( KELOG_KOM_FILE_CHECK_LOG_NOT, obj, ks )
{
	return GET( m_wstrInvaildKomName )
		&& GET( m_iUserUID )
		;
}
#endif // SERV_KOM_FILE_CHECK_ADVANCED

#ifdef SERV_SKILL_PAGE_SYSTEM
SERIALIZE_DEFINE_PUT( KDBE_EXPAND_SKILL_PAGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecSkillPageNumberToBeAdded )
		&& PUT( m_iSPoint )
		&& PUT( m_iCSPoint )
		&& PUT( m_iDefaultSkill[0] )
		&& PUT( m_iDefaultSkill[1] )
		&& PUT( m_iDefaultSkill[2] )
		&& PUT( m_iDefaultSkill[3] )
		&& PUT( m_iDefaultSkill[4] )
		&& PUT( m_iDefaultSkill[5] )
		&& PUT( m_cUnitClass )
		&& PUT( m_usEventID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_SKILL_PAGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecSkillPageNumberToBeAdded )
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint )
		&& GET( m_iDefaultSkill[0] )
		&& GET( m_iDefaultSkill[1] )
		&& GET( m_iDefaultSkill[2] )
		&& GET( m_iDefaultSkill[3] )
		&& GET( m_iDefaultSkill[4] )
		&& GET( m_iDefaultSkill[5] )
		&& GET( m_cUnitClass )
		&& GET( m_usEventID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EXPAND_SKILL_PAGE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iSkillPageNumberToBeAdded )
		&& PUT( m_iSPoint )
		&& PUT( m_iCSPoint )
		&& PUT( m_iDefaultSkill[0] )
		&& PUT( m_iDefaultSkill[1] )
		&& PUT( m_iDefaultSkill[2] )
		&& PUT( m_iDefaultSkill[3] )
		&& PUT( m_iDefaultSkill[4] )
		&& PUT( m_iDefaultSkill[5] )
		&& PUT( m_cUnitClass )
		&& PUT( m_usEventID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EXPAND_SKILL_PAGE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iSkillPageNumberToBeAdded )
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint )
		&& GET( m_iDefaultSkill[0] )
		&& GET( m_iDefaultSkill[1] )
		&& GET( m_iDefaultSkill[2] )
		&& GET( m_iDefaultSkill[3] )
		&& GET( m_iDefaultSkill[4] )
		&& GET( m_iDefaultSkill[5] )
		&& GET( m_cUnitClass )
		&& GET( m_usEventID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iSkillPagesNumberToBeActive )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iSkillPagesNumberToBeActive )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iSkillPagesNumberToBeActive )
		;
}

SERIALIZE_DEFINE_GET( KDBE_DECIDE_TO_USE_THIS_SKILL_PAGE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iSkillPagesNumberToBeActive )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_EXPAND_SKILL_PAGE_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_ucLevel )
		&& PUT( m_ucUnitClass )
		&& PUT( m_ucBeforeTheNumberOfSkillPagesAvailable )
		&& PUT( m_ucPaymentType )
		&& PUT( m_wstrSenderNickName )
		;
}

SERIALIZE_DEFINE_GET( KELOG_EXPAND_SKILL_PAGE_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_ucLevel )
		&& GET( m_ucUnitClass )
		&& GET( m_ucBeforeTheNumberOfSkillPagesAvailable )
		&& GET( m_ucPaymentType )
		&& GET( m_wstrSenderNickName )
		;
}
#endif // SERV_SKILL_PAGE_SYSTEM

#ifdef SERV_ENTRY_POINT
SERIALIZE_DEFINE_PUT( KDBE_GET_CREATE_UNIT_TODAY_COUNT_REQ, obj, ks )
{
    return PUT( m_iUserUID )
        ;
}

SERIALIZE_DEFINE_GET( KDBE_GET_CREATE_UNIT_TODAY_COUNT_REQ, obj, ks )
{
    return GET( m_iUserUID )
        ;
}
#endif SERV_ENTRY_POINT

#ifdef SERV_NAVER_CHANNELING
SERIALIZE_DEFINE_PUT( KNAVER_SOAP_GET_TOKEN_REQ, obj, ks )
{
    return PUT( m_iNaverSN )
        && PUT( m_iGameCode )
        ;
}

SERIALIZE_DEFINE_GET( KNAVER_SOAP_GET_TOKEN_REQ, obj, ks )
{
    return GET( m_iNaverSN )
        && GET( m_iGameCode )
        ;
}

SERIALIZE_DEFINE_PUT( KNAVER_SOAP_GET_TOKEN_ACK, obj, ks )
{
    return PUT( m_iOK )
        && PUT( m_strAccessToken )
        ;
}

SERIALIZE_DEFINE_GET( KNAVER_SOAP_GET_TOKEN_ACK, obj, ks )
{
    return GET( m_iOK )
        && GET( m_strAccessToken )
        ;
}


#endif SERV_NAVER_CHANNELING

#ifdef SERV_MODFIY_FLAG_REALTIME_PATCH
SERIALIZE_DEFINE_PUT( KEGS_DEL_COMMON_FLAG_NOT, obj, ks )
{
	return PUT( dwFlag )
		;
}

SERIALIZE_DEFINE_GET( KEGS_DEL_COMMON_FLAG_NOT, obj, ks )
{
	return GET( dwFlag )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_ADD_COMMON_FLAG_NOT, obj, ks )
{
	return PUT( dwFlag )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADD_COMMON_FLAG_NOT, obj, ks )
{
	return GET( dwFlag )
		;
}

SERIALIZE_DEFINE_PUT( KECN_DEL_COMMON_FLAG_NOT, obj, ks )
{
	return PUT( dwFlag )
		;
}

SERIALIZE_DEFINE_GET( KECN_DEL_COMMON_FLAG_NOT, obj, ks )
{
	return GET( dwFlag )
		;
}
SERIALIZE_DEFINE_PUT( KECN_ADD_COMMON_FLAG_NOT, obj, ks )
{
	return PUT( dwFlag )
		;
}

SERIALIZE_DEFINE_GET( KECN_ADD_COMMON_FLAG_NOT, obj, ks )
{
	return GET( dwFlag )
		;
}
#endif // SERV_MODFIY_FLAG_REALTIME_PATCH