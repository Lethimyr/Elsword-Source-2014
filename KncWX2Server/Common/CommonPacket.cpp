#include <winsock2.h>
#include "CommonPacket.h"


//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
SERIALIZE_DEFINE_PUT( KWeddingHallInfo, obj, ks )
{
	return PUT( m_iWeddingUID )
		&& PUT( m_cWeddingHallType )
		&& PUT( m_cOfficiantNPC )
		&& PUT( m_iGroom )
		&& PUT( m_iBride )
		&& PUT( m_wstrWeddingDate )
		&& PUT( m_tWeddingDate )
		&& PUT( m_wstrWeddingMsg )
		&& PUT( m_iRoomUID )
		&& PUT( m_bSuccess )
		&& PUT( m_bDelete )
		;
}

SERIALIZE_DEFINE_GET( KWeddingHallInfo, obj, ks )
{
	return GET( m_iWeddingUID )
		&& GET( m_cWeddingHallType )
		&& GET( m_cOfficiantNPC )
		&& GET( m_iGroom )
		&& GET( m_iBride )
		&& GET( m_wstrWeddingDate )
		&& GET( m_tWeddingDate )
		&& GET( m_wstrWeddingMsg )
		&& GET( m_iRoomUID )
		&& GET( m_bSuccess )
		&& GET( m_bDelete )
		;
}

SERIALIZE_DEFINE_PUT( KWeddingItemInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iWeddingUID )
		&& PUT( m_cWeddingHallType )
		&& PUT( m_cOfficiantNPC )
		&& PUT( m_iGroom )
		&& PUT( m_iBride )
		&& PUT( m_wstrGroom )
		&& PUT( m_wstrBride )
		&& PUT( m_wstrWeddingDate )
		&& PUT( m_wstrWeddingMsg )
		;
}

SERIALIZE_DEFINE_GET( KWeddingItemInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iWeddingUID )
		&& GET( m_cWeddingHallType )
		&& GET( m_cOfficiantNPC )
		&& GET( m_iGroom )
		&& GET( m_iBride )
		&& GET( m_wstrGroom )
		&& GET( m_wstrBride )
		&& GET( m_wstrWeddingDate )
		&& GET( m_wstrWeddingMsg )
		;
}

SERIALIZE_DEFINE_PUT( KRelationshipInfo, obj, ks )
{
	return PUT( m_cRelationshipType )
		&& PUT( m_iOtherUnitUID )
		&& PUT( m_wstrOtherNickName )
		&& PUT( m_wstrLoveWord )
		&& PUT( m_tDate )
		&& PUT( m_tLastReward )
		&& PUT( m_cRewardTitleType )
		;
}

SERIALIZE_DEFINE_GET( KRelationshipInfo, obj, ks )
{
	return GET( m_cRelationshipType )
		&& GET( m_iOtherUnitUID )
		&& GET( m_wstrOtherNickName )
		&& GET( m_wstrLoveWord )
		&& GET( m_tDate )
		&& GET( m_tLastReward )
		&& GET( m_cRewardTitleType )
		;
}
#endif SERV_RELATIONSHIP_SYSTEM
//}}


//{{ 2012. 1. 9	최육사		DB접속 암호 이중화
#ifdef SERV_DB_CONNECTION_SECURITY
SERIALIZE_DEFINE_PUT( KDBConnectionInfo, obj, ks )
{
	return PUT( m_iDBType )
		&& PUT( m_wstrConnectionString )
		&& PUT( m_iThreadCount )
		;
}

SERIALIZE_DEFINE_GET( KDBConnectionInfo, obj, ks )
{
	return GET( m_iDBType )
		&& GET( m_wstrConnectionString )
		&& GET( m_iThreadCount )
		;
}
#endif SERV_DB_CONNECTION_SECURITY
//}}


SERIALIZE_DEFINE_PUT( KNetAddress, obj, ks )
{
    return PUT( m_wstrIP )
		&& PUT( m_usPort )
		;
}

SERIALIZE_DEFINE_GET( KNetAddress, obj, ks )
{
    return GET( m_wstrIP )
		&& GET( m_usPort )
		;
}

SERIALIZE_DEFINE_PUT( KNexonAccountInfo, obj, ks )
{
    return PUT( m_wstrID )
		&& PUT( m_uiNexonSN )
		&& PUT( m_wstrLoginIP )
		&& PUT( m_bSex )
		&& PUT( m_uiAge )
		&& PUT( m_uiPwdHash )
		&& PUT( m_uiSsnHash )
		&& PUT( m_uiFlag0 )
		&& PUT( m_uiFlag1 )
		//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		&& PUT( m_bNewMembership )
		&& PUT( m_uMainAuthLevel )
		&& PUT( m_uSubAuthLevel )
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		//}}
		//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
		&& PUT( m_wstrNationCode )
		&& PUT( m_wstrMeta )
		&& PUT( m_uSecureCode )
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		&& PUT( m_uChannelCode )
		&& PUT( m_wstrChannelUID )
#else
		&& PUT( m_uRegisteredPC )
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
#else
		&& PUT( m_uiServerHash )
		&& PUT( m_wstrServerName )
#endif SERV_OTP_AUTH
		//}}
		&& PUT( m_byteGuestUser )
		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		&& PUT( m_iBF_Team )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KNexonAccountInfo, obj, ks )
{
    return GET( m_wstrID )
		&& GET( m_uiNexonSN )
		&& GET( m_wstrLoginIP )
		&& GET( m_bSex )
		&& GET( m_uiAge )
		&& GET( m_uiPwdHash )
		&& GET( m_uiSsnHash )
		&& GET( m_uiFlag0 )
		&& GET( m_uiFlag1 )
		//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		&& GET( m_bNewMembership )
		&& GET( m_uMainAuthLevel )
		&& GET( m_uSubAuthLevel )
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		//}}
		//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
		&& GET( m_wstrNationCode )
		&& GET( m_wstrMeta )
		&& GET( m_uSecureCode )
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		&& GET( m_uChannelCode )
		&& GET( m_wstrChannelUID )
#else
		&& GET( m_uRegisteredPC )
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
#else
		&& GET( m_uiServerHash )
		&& GET( m_wstrServerName )
#endif SERV_OTP_AUTH
		//}}
		&& GET( m_byteGuestUser )
		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		&& GET( m_iBF_Team )
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}
        ;
}

SERIALIZE_DEFINE_PUT( KUserAuthenticateReq, obj, ks )
{
    return PUT( m_bDebugAuth )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrPasswd )
		//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
		&& PUT( m_wstrHWID )
#endif SERV_OTP_AUTH
		//}}
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		&& PUT( m_strMachineID )
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		&& PUT( m_kChannelRandomKey )
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
		//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		&& PUT( m_charServerSN[0] )
		&& PUT( m_charServerSN[1] )
		&& PUT( m_charServerSN[2] )
		&& PUT( m_charServerSN[3] )
		&& PUT( m_charServerSN[4] )
		&& PUT( m_charServerSN[5] )
		&& PUT( m_charServerSN[6] )
		&& PUT( m_charServerSN[7] )
		&& PUT( m_charServerSN[8] )
		&& PUT( m_charServerSN[9] )
		&& PUT( m_charServerSN[10] )
		&& PUT( m_charServerSN[11] )
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 03. 27	김민성		머신 시간을 검사하자(50분 이상 차이나면 모니터링)
#ifdef SERV_CHECK_MACHINE_LOCALTIME
		&& PUT( m_wstrClientTime )
#endif SERV_CHECK_MACHINE_LOCALTIME
		//}}
//#ifdef SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING
		&& PUT( m_iChannelingCode )
//#endif SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING
		//{{ 2013.03.05 조효진		KOG_OTP 정리 및 재구성
#ifdef SERV_KOG_OTP_VERIFY
		&& PUT( m_bManualLogin )
#endif SERV_KOG_OTP_VERIFY
		//}}
#ifdef SERV_COUNTRY_TH	
		&& PUT( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
#ifdef SERV_STEAM
		&& PUT( m_bSteamClient )
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		&& PUT( m_bAeriaClient )
#endif //SERV_CHANNELING_AERIA
		;
}

SERIALIZE_DEFINE_GET( KUserAuthenticateReq, obj, ks )
{
    return GET( m_bDebugAuth )
		&& GET( m_wstrUserID )
		&& GET( m_wstrPasswd )
		//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
		&& GET( m_wstrHWID )
#endif SERV_OTP_AUTH
		//}}
		//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
		&& GET( m_strMachineID )
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
		//}}
		//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		&& GET( m_kChannelRandomKey )
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
		//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		&& GET( m_charServerSN[0] )
		&& GET( m_charServerSN[1] )
		&& GET( m_charServerSN[2] )
		&& GET( m_charServerSN[3] )
		&& GET( m_charServerSN[4] )
		&& GET( m_charServerSN[5] )
		&& GET( m_charServerSN[6] )
		&& GET( m_charServerSN[7] )
		&& GET( m_charServerSN[8] )
		&& GET( m_charServerSN[9] )
		&& GET( m_charServerSN[10] )
		&& GET( m_charServerSN[11] )
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}
		//{{ 2012. 03. 27	김민성		머신 시간을 검사하자(50분 이상 차이나면 모니터링)
#ifdef SERV_CHECK_MACHINE_LOCALTIME
		&& GET( m_wstrClientTime )
#endif SERV_CHECK_MACHINE_LOCALTIME
		//}}
//#ifdef SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING
		&& GET( m_iChannelingCode )
//#endif SERV_CHANNELING_USER_MANAGER // SERV_JAPAN_CHANNELING
		//{{ 2013.03.05 조효진		KOG_OTP 정리 및 재구성
#ifdef SERV_KOG_OTP_VERIFY
		&& GET( m_bManualLogin )
#endif SERV_KOG_OTP_VERIFY
		//}}
#ifdef SERV_COUNTRY_TH	
		&& GET( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
#ifdef SERV_STEAM
		&& GET( m_bSteamClient )
#endif //SERV_STEAM
#ifdef SERV_CHANNELING_AERIA
		&& GET( m_bAeriaClient )
#endif //SERV_CHANNELING_AERIA
		;
}

SERIALIZE_DEFINE_PUT( KAccountOption, obj, ks )
{
    return PUT( m_bPlayGuide )
		;
}

SERIALIZE_DEFINE_GET( KAccountOption, obj, ks )
{
    return GET( m_bPlayGuide )
		;
}

SERIALIZE_DEFINE_PUT( KAccountBlockInfo, obj, ks )
{
    return PUT( m_wstrEndTime )
		&& PUT( m_wstrBlockReason )
#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
		&& PUT( m_byteBlockType )
		&& PUT( m_wstrBlockReason2 )
		&& PUT( m_wstrBlockEndDate )
#endif // SERV_HACKING_USER_CHECK_COUNT
		;
}

SERIALIZE_DEFINE_GET( KAccountBlockInfo, obj, ks )
{
    return GET( m_wstrEndTime )
		&& GET( m_wstrBlockReason )
#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
		&& GET( m_byteBlockType )
		&& GET( m_wstrBlockReason2 )
		&& GET( m_wstrBlockEndDate )
#endif // SERV_HACKING_USER_CHECK_COUNT
		;
}

SERIALIZE_DEFINE_PUT( KAccountInfo, obj, ks )
{
    return PUT( m_nUserUID )
		&& PUT( m_wstrID )
		&& PUT( m_wstrName )
		&& PUT( m_iAuthLevel )
		&& PUT( m_bInternalUser )
		&& PUT( m_kAccountOption )
		&& PUT( m_kAccountBlockInfo )
		&& PUT( m_bIsRecommend )
		&& PUT( m_bIsGuestUser )
        && PUT( m_wstrOTP )
		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
		&& PUT( m_wstrRegDate )
#endif SERV_CASH_ITEM_LIST
		//}}
		//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
		&& PUT( m_wstrLastLogin )
#endif SERV_SECOND_SECURITY
		//}}
		//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		&& PUT( m_iChannelRandomKey )
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KAccountInfo, obj, ks )
{
    return GET( m_nUserUID )
		&& GET( m_wstrID )
		&& GET( m_wstrName )
		&& GET( m_iAuthLevel )
		&& GET( m_bInternalUser )
		&& GET( m_kAccountOption )
		&& GET( m_kAccountBlockInfo )
		&& GET( m_bIsRecommend )
		&& GET( m_bIsGuestUser )		
        && GET( m_wstrOTP )
		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
		&& GET( m_wstrRegDate )
#endif SERV_CASH_ITEM_LIST
		//}}
		//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
		&& GET( m_wstrLastLogin )
#endif SERV_SECOND_SECURITY
		//}}
		//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		&& GET( m_iChannelRandomKey )
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
		;
}







SERIALIZE_DEFINE_PUT( KItemAttributeEnchantInfo, obj, ks )
{
	return PUT( m_cAttribEnchant0 )
		&& PUT( m_cAttribEnchant1 )
		&& PUT( m_cAttribEnchant2 )
		;
}

SERIALIZE_DEFINE_GET( KItemAttributeEnchantInfo, obj, ks )
{
	return GET( m_cAttribEnchant0 )
		&& GET( m_cAttribEnchant1 )
		&& GET( m_cAttribEnchant2 )
		;
}










SERIALIZE_DEFINE_PUT( KItemInfo, obj, ks )
{
	return PUT( m_iItemID )
		&& PUT( m_cUsageType )
		&& PUT( m_iQuantity )
		&& PUT( m_sEndurance )		
		&& PUT( m_ucSealData )
		&& PUT( m_cEnchantLevel )
		&& PUT( m_kAttribEnchantInfo )
        && PUT( m_vecItemSocket )
		//{{ 2013. 05. 09	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		&& PUT( m_vecRandomSocket )
		&& PUT( m_cItemState )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		&& PUT( m_sPeriod )
		&& PUT( m_wstrExpirationDate )
#ifdef SERV_GOLD_TICKET
		&& PUT(	m_iGoldTicketKeyUID )
#endif //SERV_GOLD_TICKET
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		&& PUT( m_byteExpandedSocketNum )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KItemInfo, obj, ks )
{
	return GET( m_iItemID )
		&& GET( m_cUsageType )
		&& GET( m_iQuantity )
		&& GET( m_sEndurance )		
		&& GET( m_ucSealData )
		&& GET( m_cEnchantLevel )		
		&& GET( m_kAttribEnchantInfo )
		&& GET( m_vecItemSocket )
		//{{ 2013. 05. 09	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		&& GET( m_vecRandomSocket )
		&& GET( m_cItemState )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		&& GET( m_sPeriod )
		&& GET( m_wstrExpirationDate )
#ifdef SERV_GOLD_TICKET
		&& GET(	m_iGoldTicketKeyUID )
#endif //SERV_GOLD_TICKET
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-18	// 박세훈
		&& GET( m_byteExpandedSocketNum )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_PUT( KBuyGPItemInfo, obj, ks )
{
    return PUT( m_iItemID )
		&& PUT( m_iPrice )
		&& PUT( m_iPvPPoint )
		&& PUT( m_cPeriodType )
		&& PUT( m_iQuantity )
		&& PUT( m_sEndurance )
		&& PUT( m_sPeriod )
		;
}

SERIALIZE_DEFINE_GET( KBuyGPItemInfo, obj, ks )
{
    return GET( m_iItemID )
		&& GET( m_iPrice )
		&& GET( m_iPvPPoint )
		&& GET( m_cPeriodType )
		&& GET( m_iQuantity )
		&& GET( m_sEndurance )
		&& GET( m_sPeriod )
		;
}


//{{ 2010. 8. 3	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

SERIALIZE_DEFINE_PUT( KItemPosition, obj, ks )
{
	return PUT( m_iSlotCategory )
		&& PUT( m_iSlotID )
		&& PUT( m_iPetUID )
		;
}

SERIALIZE_DEFINE_GET( KItemPosition, obj, ks )
{
	return GET( m_iSlotCategory )
		&& GET( m_iSlotID )
		&& GET( m_iPetUID )
		;
}

#endif SERV_PET_SYSTEM
//}}


SERIALIZE_DEFINE_PUT( KInventoryItemSimpleInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_cSlotCategory )
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		&& PUT( m_sSlotID )
#else
		&& PUT( m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		&& PUT( m_EnchantLevel )
		&& PUT( m_kAttribEnchantInfo )
		;
}

SERIALIZE_DEFINE_GET( KInventoryItemSimpleInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iItemID )
		&& GET( m_cSlotCategory )
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		&& GET( m_sSlotID )
#else
		&& GET( m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		&& GET( m_EnchantLevel )
		&& GET( m_kAttribEnchantInfo )
		;
}

SERIALIZE_DEFINE_PUT( KInventoryItemInfo, obj, ks )
{
    return PUT( m_iItemUID )
		&& PUT( m_cSlotCategory )
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		&& PUT( m_sSlotID )
#else
		&& PUT( m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}		
		&& PUT( m_kItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KInventoryItemInfo, obj, ks )
{
    return GET( m_iItemUID )
		&& GET( m_cSlotCategory )
		//{{ 2011. 12. 15	최육사	인벤토리 슬롯ID크기 늘리기
#ifdef SERV_EXPAND_SLOT_ID_DATA_SIZE
		&& GET( m_sSlotID )
#else
		&& GET( m_cSlotID )
#endif SERV_EXPAND_SLOT_ID_DATA_SIZE
		//}}
		&& GET( m_kItemInfo )
		;
}

//{{ 2009. 11. 15  최육사	아이템삭제사유
SERIALIZE_DEFINE_PUT( KDeletedItemInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_ucDeleteReason )
		;
}

SERIALIZE_DEFINE_GET( KDeletedItemInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_ucDeleteReason )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KItemQuantityUpdate, obj, ks )
{
    return PUT( m_mapQuantityChange )
		&& PUT( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_GET( KItemQuantityUpdate, obj, ks )
{
    return GET( m_mapQuantityChange )
		&& GET( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_PUT( KDungeonClearInfo, obj, ks )
{
    return PUT( m_iDungeonID )
		&& PUT( m_iMaxScore )
		&& PUT( m_cMaxTotalRank )
		&& PUT( m_wstrClearTime )
		&& PUT( m_bNew )
		;
}

SERIALIZE_DEFINE_GET( KDungeonClearInfo, obj, ks )
{
    return GET( m_iDungeonID )
		&& GET( m_iMaxScore )
		&& GET( m_cMaxTotalRank )
		&& GET( m_wstrClearTime )
		&& GET( m_bNew )
		;
}

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
SERIALIZE_DEFINE_PUT( KDungeonPlayInfo, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_iPlayTimes )
		&& PUT( m_iClearTimes )
		&& PUT( m_bNew )
		;
}
SERIALIZE_DEFINE_GET( KDungeonPlayInfo, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_iPlayTimes )
		&& GET( m_iClearTimes )
		&& GET( m_bNew )
		;
}
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

SERIALIZE_DEFINE_PUT( KTCClearInfo, obj, ks )
{
	return PUT( m_iTCID )
		&& PUT( m_wstrClearTime )
		&& PUT( m_bNew )
		;
}

SERIALIZE_DEFINE_GET( KTCClearInfo, obj, ks )
{
	return GET( m_iTCID )
		&& GET( m_wstrClearTime )
		&& GET( m_bNew )
		;
}

SERIALIZE_DEFINE_PUT( KStat, obj, ks )
{
	return PUT( m_iBaseHP )
		&& PUT( m_iAtkPhysic )
		&& PUT( m_iAtkMagic )
		&& PUT( m_iDefPhysic )
		&& PUT( m_iDefMagic )
		;
}

SERIALIZE_DEFINE_GET( KStat, obj, ks )
{
	return GET( m_iBaseHP )
		&& GET( m_iAtkPhysic )
		&& GET( m_iAtkMagic )
		&& GET( m_iDefPhysic )
		&& GET( m_iDefMagic )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KSkillData, obj, ks )
{
	return PUT( m_iSkillID )
		&& PUT( m_cSkillLevel )
		;

}

SERIALIZE_DEFINE_GET( KSkillData, obj, ks )
{
	return GET( m_iSkillID )
		&& GET( m_cSkillLevel )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KUnitSkillData, obj, ks )
{
	return PUT( m_aEquippedSkill[0] )
		&& PUT( m_aEquippedSkill[1] )
		&& PUT( m_aEquippedSkill[2] )
		&& PUT( m_aEquippedSkill[3] )
		&& PUT( m_aEquippedSkillSlotB[0] )
		&& PUT( m_aEquippedSkillSlotB[1] )
		&& PUT( m_aEquippedSkillSlotB[2] )
		&& PUT( m_aEquippedSkillSlotB[3] )
		&& PUT( m_wstrSkillSlotBEndDate )
		&& PUT( m_cSkillSlotBExpirationState )
		&& PUT( m_vecPassiveSkill )
		//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_vecGuildPassiveSkill )
#endif GUILD_SKILL_TEST
		//}}
		//{{ 2010. 03. 13  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
		&& PUT( m_vecSkillNote )
#endif SERV_SKILL_NOTE
		//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_nActiveSkillPagesNumber)
		&& PUT( m_nTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KUnitSkillData, obj, ks )
{
	return GET( m_aEquippedSkill[0] )
		&& GET( m_aEquippedSkill[1] )
		&& GET( m_aEquippedSkill[2] )
		&& GET( m_aEquippedSkill[3] )
		&& GET( m_aEquippedSkillSlotB[0] )
		&& GET( m_aEquippedSkillSlotB[1] )
		&& GET( m_aEquippedSkillSlotB[2] )
		&& GET( m_aEquippedSkillSlotB[3] )
		&& GET( m_wstrSkillSlotBEndDate )
		&& GET( m_cSkillSlotBExpirationState )
		&& GET( m_vecPassiveSkill )
		//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_vecGuildPassiveSkill )
#endif GUILD_SKILL_TEST
		//}}
		//{{ 2010. 03. 13  최육사	기술의 노트
#ifdef SERV_SKILL_NOTE
		&& GET( m_vecSkillNote )
#endif SERV_SKILL_NOTE
		//}}
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_nActiveSkillPagesNumber)
		&& GET( m_nTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM
		;   
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KFriendInfo, obj, ks )
{
    return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cGroupID )
		&& PUT( m_cStatus )
		&& PUT( m_cPosition )
		;
}

SERIALIZE_DEFINE_GET( KFriendInfo, obj, ks )
{
    return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cGroupID )
		&& GET( m_cStatus )
		&& GET( m_cPosition )
		;
}

SERIALIZE_DEFINE_PUT( KFriendMessageInfo, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_wstrNickName ) &&
            PUT( m_cMessageType ) &&
            PUT( m_wstrMessage ) &&
            PUT( m_wstrRegDate );
}

SERIALIZE_DEFINE_GET( KFriendMessageInfo, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_wstrNickName ) &&
            GET( m_cMessageType ) &&
            GET( m_wstrMessage ) &&
            GET( m_wstrRegDate );
}

SERIALIZE_DEFINE_PUT( KMessengerInfo, obj, ks )
{
    return PUT( m_mapFriendInfo )
		&& PUT( m_mapGroup )
        && PUT( m_vecFriendMessageInfo )
		;
}

SERIALIZE_DEFINE_GET( KMessengerInfo, obj, ks )
{
    return GET( m_mapFriendInfo )
		&& GET( m_mapGroup )
        && GET( m_vecFriendMessageInfo )
		;
}

//{{ 2009. 10. 27  최육사	길드레벨
SERIALIZE_DEFINE_PUT( KUserGuildInfo, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_wstrGuildName )
		&& PUT( m_ucMemberShipGrade )
		&& PUT( m_iHonorPoint )
		;
}

SERIALIZE_DEFINE_GET( KUserGuildInfo, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_wstrGuildName )
		&& GET( m_ucMemberShipGrade )
		&& GET( m_iHonorPoint )
		;
}
//}}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KBuffInfo, obj, ks )
{
	return PUT( m_kFactorInfo )
		&& PUT( m_iBuffStartTime )
		&& PUT( m_iBuffEndTime )
		;
}

SERIALIZE_DEFINE_GET( KBuffInfo, obj, ks )
{
	return GET( m_kFactorInfo )
		&& GET( m_iBuffStartTime )
		&& GET( m_iBuffEndTime )
		;
}

SERIALIZE_DEFINE_PUT( KUnitBuffInfo, obj, ks )
{
	return PUT( m_iInsertTime )
		&& PUT( m_mapBuffInfo )
		;
}

SERIALIZE_DEFINE_GET( KUnitBuffInfo, obj, ks )
{
	return GET( m_iInsertTime )
		&& GET( m_mapBuffInfo )
		;
}

SERIALIZE_DEFINE_PUT( KNpcUnitBuffInfo, obj, ks )
{
	return PUT( m_iNpcUID )
		&& PUT( m_vecBuffFactor )
		;
}

SERIALIZE_DEFINE_GET( KNpcUnitBuffInfo, obj, ks )
{
	return GET( m_iNpcUID )
		&& GET( m_vecBuffFactor )
		;
}

#endif SERV_SERVER_BUFF_SYSTEM
//}}


SERIALIZE_DEFINE_PUT( KUnitInfo, obj, ks )
{
	return PUT( m_iOwnerUserUID ) 
		&& PUT( m_cAuthLevel ) 
		&& PUT( m_nUnitUID ) 
		&& PUT( m_uiKNMSerialNum ) 
		&& PUT( m_cUnitClass ) 		
		&& PUT( m_wstrNickName ) 
		&& PUT( m_wstrIP ) 
		&& PUT( m_usPort ) 
		&& PUT( m_iED ) 
		&& PUT( m_ucLevel ) 
		&& PUT( m_iEXP )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_iOfficialMatchCnt )
		&& PUT( m_iRating )
		&& PUT( m_iMaxRating )
		&& PUT( m_iRPoint )
		&& PUT( m_iAPoint )
		&& PUT( m_bIsWinBeforeMatch )

		//{{ 2012. 06. 20	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank )
		//{{ 2012. 06. 25	박세훈	2012 대전 시즌2
		&& PUT( m_fKFactor )
		&& PUT( m_bIsRedistributionUser )
		&& PUT( m_iPastSeasonWin )
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		&& PUT( m_iPVPEmblem )
		&& PUT( m_iVSPoint )
		&& PUT( m_iVSPointMax )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_iSPoint ) 
		&& PUT( m_iCSPoint ) 
		&& PUT( m_iMaxCSPoint ) 
		&& PUT( m_wstrCSPointEndDate ) 
		&& PUT( m_nNowBaseLevelEXP ) 
		&& PUT( m_nNextBaseLevelEXP ) 
		&& PUT( m_nStraightVictories ) 
		&& PUT( m_kStat ) 
		&& PUT( m_kGameStat ) 
		//{{ 2012. 02. 02	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_kLastPos ) 
#else
		&& PUT( m_nMapID ) 
		&& PUT( m_ucLastTouchLineIndex )
		&& PUT( m_usLastPosValue )
#endif SERV_BATTLE_FIELD_SYSTEM
		//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		&& PUT( m_vecBuffInfo )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		&& PUT( m_iWin ) 
		&& PUT( m_iLose )
		&& PUT( m_mapDungeonClear )
		&& PUT( m_mapTCClear )
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& PUT( m_mapDungeonPlay )
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& PUT( m_mapEquippedItem )
		&& PUT( m_UnitSkillData )
		&& PUT( m_bIsParty )
		&& PUT( m_iSpiritMax )			
		&& PUT( m_iSpirit )
		&& PUT( m_bIsGameBang )
#ifdef SERV_PC_BANG_TYPE
		&& PUT( m_iPcBangType )
#endif SERV_PC_BANG_TYPE
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iTitleID )
#else
		&& PUT( m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_kUserGuildInfo )
#endif GUILD_TEST
		//}}
        && PUT( m_wstrLastLoginTime )
		//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
		&& PUT( m_wstrLastDate )
		&& PUT( m_bDeleted )			
		&& PUT( m_trDelAbleDate )
		&& PUT( m_trRestoreAbleDate )
#endif SERV_UNIT_WAIT_DELETE
		//}}
#ifdef SERV_ADD_WARP_BUTTON
		&& PUT( m_trWarpVipEndData )
#endif // SERV_ADD_WARP_BUTTON
#ifdef SERV_GROW_UP_SOCKET
		&& PUT( m_iEventQuestClearCount )
		&& PUT( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
		&& PUT( m_iAccumulationSpirit )
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
#ifdef SERV_CHINA_SPIRIT_EVENT
		&& PUT( m_arrChinaSpirit[0] )
		&& PUT( m_arrChinaSpirit[1] )
		&& PUT( m_arrChinaSpirit[2] )
		&& PUT( m_arrChinaSpirit[3] )
		&& PUT( m_arrChinaSpirit[4] )
		&& PUT( m_arrChinaSpirit[5] )
#endif SERV_CHINA_SPIRIT_EVENT
#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		&& PUT( m_bRecruit )
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
#ifdef SERV_NEW_YEAR_EVENT_2014
		&& PUT( m_ucOldYearMissionRewardedLevel )
		&& PUT( m_iNewYearMissionStepID )
#endif SERV_NEW_YEAR_EVENT_2014
#ifdef SERV_EVENT_CHECK_POWER
		&& PUT( m_ucCheckPowerCount )
		&& PUT( m_iCheckPowerTime )
		&& PUT( m_bCheckPowerShowPopUp )
		&& PUT( m_ucCheckPowerScore )
#endif SERV_EVENT_CHECK_POWER
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& PUT( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

SERIALIZE_DEFINE_GET( KUnitInfo, obj, ks )
{
	return GET( m_iOwnerUserUID )
		&& GET( m_cAuthLevel )
		&& GET( m_nUnitUID )
		&& GET( m_uiKNMSerialNum )
		&& GET( m_cUnitClass )		
		&& GET( m_wstrNickName )
		&& GET( m_wstrIP )
		&& GET( m_usPort )
		&& GET( m_iED )
		&& GET( m_ucLevel )
		&& GET( m_iEXP )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_iOfficialMatchCnt )
		&& GET( m_iRating )
		&& GET( m_iMaxRating )
		&& GET( m_iRPoint )
		&& GET( m_iAPoint )
		&& GET( m_bIsWinBeforeMatch )

		//{{ 2012. 06. 20	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
		//{{ 2012. 06. 25	박세훈	2012 대전 시즌2
		&& GET( m_fKFactor )
		&& GET( m_bIsRedistributionUser )
		&& GET( m_iPastSeasonWin )
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		&& GET( m_iPVPEmblem )
		&& GET( m_iVSPoint )
		&& GET( m_iVSPointMax )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_iSPoint )
		&& GET( m_iCSPoint ) 
		&& GET( m_iMaxCSPoint ) 
		&& GET( m_wstrCSPointEndDate ) 
		&& GET( m_nNowBaseLevelEXP )
		&& GET( m_nNextBaseLevelEXP )
		&& GET( m_nStraightVictories )
		&& GET( m_kStat )
		&& GET( m_kGameStat )
		//{{ 2012. 02. 02	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_kLastPos ) 
#else
		&& GET( m_nMapID ) 
		&& GET( m_ucLastTouchLineIndex )
		&& GET( m_usLastPosValue )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS

		&& GET( m_vecBuffInfo )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		&& GET( m_iWin )
		&& GET( m_iLose )
		&& GET( m_mapDungeonClear )
		&& GET( m_mapTCClear )
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& GET( m_mapDungeonPlay )
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& GET( m_mapEquippedItem )
		&& GET( m_UnitSkillData )
		&& GET( m_bIsParty )
		&& GET( m_iSpiritMax )
		&& GET( m_iSpirit )
		&& GET( m_bIsGameBang )
#ifdef SERV_PC_BANG_TYPE
		&& GET( m_iPcBangType )
#endif SERV_PC_BANG_TYPE
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iTitleID )
#else
		&& GET( m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_kUserGuildInfo )
#endif GUILD_TEST
		//}}
        && GET( m_wstrLastLoginTime )
		//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
		&& GET( m_wstrLastDate )
		&& GET( m_bDeleted )			
		&& GET( m_trDelAbleDate )
		&& GET( m_trRestoreAbleDate )
#endif SERV_UNIT_WAIT_DELETE
		//}}
#ifdef SERV_ADD_WARP_BUTTON
		&& GET( m_trWarpVipEndData )
#endif // SERV_ADD_WARP_BUTTON
#ifdef SERV_GROW_UP_SOCKET
		&& GET( m_iEventQuestClearCount )
		&& GET( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
		&& GET( m_iAccumulationSpirit )
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM
#ifdef SERV_CHINA_SPIRIT_EVENT
		&& GET( m_arrChinaSpirit[0] )
		&& GET( m_arrChinaSpirit[1] )
		&& GET( m_arrChinaSpirit[2] )
		&& GET( m_arrChinaSpirit[3] )
		&& GET( m_arrChinaSpirit[4] )
		&& GET( m_arrChinaSpirit[5] )
#endif SERV_CHINA_SPIRIT_EVENT
#ifdef SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
		&& GET( m_bRecruit )
#endif SERV_RECRUIT_EVENT_QUEST_FOR_NEW_USER
#ifdef SERV_NEW_YEAR_EVENT_2014
		&& GET( m_ucOldYearMissionRewardedLevel )
		&& GET( m_iNewYearMissionStepID )
#endif SERV_NEW_YEAR_EVENT_2014
#ifdef SERV_EVENT_CHECK_POWER
		&& GET( m_ucCheckPowerCount )
		&& GET( m_iCheckPowerTime )
		&& GET( m_bCheckPowerShowPopUp )
		&& GET( m_ucCheckPowerScore )
#endif SERV_EVENT_CHECK_POWER
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& GET( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

/////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( VECTOR3, obj, ks )
{
	return  PUT( x )
		&&	PUT( y )
		&&	PUT( z )
		;
}

SERIALIZE_DEFINE_GET( VECTOR3, obj, ks )
{
	return  GET( x )
		&&	GET( y )
		&&	GET( z )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KRoomInfo, obj, ks )
{
    return PUT( m_RoomType )
        && PUT( m_RoomUID )
        && PUT( m_uiRoomListID )
        && PUT( m_RoomName )
        && PUT( m_RoomState )
        && PUT( m_bPublic )
		&& PUT( m_bTeamBalance )
        && PUT( m_wstrPassword )
        && PUT( m_MaxSlot )
        && PUT( m_JoinSlot )
        && PUT( m_wstrUDPRelayIP )
        && PUT( m_usUDPRelayPort )
		//{{ 2011. 12. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_bStartedByAutoParty )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
        && PUT( m_PVPGameType )
#ifdef DUNGEON_ITEM
		&& PUT( m_bIsItemMode )
#endif DUNGEON_ITEM
		&& PUT( m_iPVPChannelClass )
        && PUT( m_WinKillNum )
        && PUT( m_bCanIntrude )
        && PUT( m_fPlayTime )
        && PUT( m_WorldID )		
		&& PUT( m_ShowPvpMapWorldID )
        && PUT( m_DifficultyLevel )
        && PUT( m_iDungeonID )
		&& PUT( m_cGetItemType )
		&& PUT( m_cDungeonMode )
		&& PUT( m_iPartyUID )
		//{{ 2011. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM		
		&& PUT( m_iBattleFieldID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
#ifdef SERV_PVP_REMATCH
		&& PUT( m_mapAllPlayersSelectedMap )
#endif SERV_PVP_REMATCH
		//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& PUT( m_bDefenceDungeonOpen )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KRoomInfo, obj, ks )
{
    return GET( m_RoomType )
        && GET( m_RoomUID )
        && GET( m_uiRoomListID )
        && GET( m_RoomName )
        && GET( m_RoomState )
        && GET( m_bPublic )
		&& GET( m_bTeamBalance )
        && GET( m_wstrPassword )
        && GET( m_MaxSlot )
        && GET( m_JoinSlot )
        && GET( m_wstrUDPRelayIP )
        && GET( m_usUDPRelayPort )
		//{{ 2011. 12. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_bStartedByAutoParty )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
        && GET( m_PVPGameType )
#ifdef DUNGEON_ITEM
		&& GET( m_bIsItemMode )
#endif DUNGEON_ITEM
		&& GET( m_iPVPChannelClass )
        && GET( m_WinKillNum )
        && GET( m_bCanIntrude )
        && GET( m_fPlayTime )
        && GET( m_WorldID )		
		&& GET( m_ShowPvpMapWorldID )
        && GET( m_DifficultyLevel )
        && GET( m_iDungeonID )
		&& GET( m_cGetItemType )
		&& GET( m_cDungeonMode )
		&& GET( m_iPartyUID )
		//{{ 2011. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM		
		&& GET( m_iBattleFieldID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
#ifdef SERV_PVP_REMATCH
		&& GET( m_mapAllPlayersSelectedMap )
#endif SERV_PVP_REMATCH
		//{{ 2013. 04. 10	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& GET( m_bDefenceDungeonOpen )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
        ;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KTCInfo, obj, ks )
{
	return  PUT( m_iTCID )
		&&  PUT( m_iDungeonID )
		&&	PUT( m_RoomUID )
		&&	PUT( m_cRoomType )
		&&	PUT( m_wstrUDPRelayIP )
		&&  PUT( m_usUDPRelayPort )
		&&  PUT( m_fPlayTime )
		;
}

SERIALIZE_DEFINE_GET( KTCInfo, obj, ks )
{
	return  GET( m_iTCID )
		&&  GET( m_iDungeonID )
		&&	GET( m_RoomUID )
		&&	GET( m_cRoomType )
		&&	GET( m_wstrUDPRelayIP )
		&&  GET( m_usUDPRelayPort )
		&&  GET( m_fPlayTime )
		;
}

//////////////////////////////////////////////////////////////////////////

//{{ QUEST 개편 - 김민성
#ifdef SERV_REFORM_QUEST
SERIALIZE_DEFINE_PUT( KSubQuestData, obj, ks )
{
	return  PUT( m_iInInventoryItemCount )
		&&  PUT( m_bSussece )
		;
}

SERIALIZE_DEFINE_GET( KSubQuestData, obj, ks )
{
	return  GET( m_iInInventoryItemCount )
		&&	GET( m_bSussece )
		;
}
#endif SERV_REFORM_QUEST
//}}

SERIALIZE_DEFINE_PUT( KSubQuestInfo, obj, ks )
{
	return  PUT( m_mapSubQuestInfo )
		;
}

SERIALIZE_DEFINE_GET( KSubQuestInfo, obj, ks )
{
	return  GET( m_mapSubQuestInfo )
		;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2010. 07. 20  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

SERIALIZE_DEFINE_PUT( KPetInfo, obj, ks )
{
	return PUT( m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		&& PUT( m_iPetID )
#else
		&& PUT( m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		&& PUT( m_wstrPetName )
		&& PUT( m_cEvolutionStep )
		&& PUT( m_sSatiety )
		&& PUT( m_iIntimacy )
		&& PUT( m_sExtroversion )
		&& PUT( m_sEmotion )
		&& PUT( m_bAutoFeed )
		&& PUT( m_wstrLastFeedDate )
		&& PUT( m_wstrLastSummonDate )
		&& PUT( m_wstrRegDate )
		&& PUT( m_bAutoLooting )
#ifdef SERV_EVENT_PET_INVENTORY
		//이벤트 펫 먹이 추가 
		&& PUT( m_bEventFoodEat )
		&& PUT( m_bIsEventPetID )
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_FREE_AUTO_LOOTING
		&& PUT(	m_bFreeAutoLooting )
#endif SERV_FREE_AUTO_LOOTING
#ifdef SERV_PERIOD_PET
		&& PUT(	m_wstrDestroyDate )
#endif SERV_PERIOD_PET
		;
}

SERIALIZE_DEFINE_GET( KPetInfo, obj, ks )
{
	return GET( m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		&& GET( m_iPetID )
#else
		&& GET( m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		&& GET( m_wstrPetName )
		&& GET( m_cEvolutionStep )
		&& GET( m_sSatiety )
		&& GET( m_iIntimacy )
		&& GET( m_sExtroversion )
		&& GET( m_sEmotion )
		&& GET( m_bAutoFeed )
		&& GET( m_wstrLastFeedDate )
		&& GET( m_wstrLastSummonDate )
		&& GET( m_wstrRegDate )
		&& GET( m_bAutoLooting )
#ifdef SERV_EVENT_PET_INVENTORY
		//이벤트 펫 먹이 추가 
		&& GET( m_bEventFoodEat )
		&& GET( m_bIsEventPetID )
#endif SERV_EVENT_PET_INVENTORY
#ifdef SERV_FREE_AUTO_LOOTING
		&& GET(	m_bFreeAutoLooting )
#endif SERV_FREE_AUTO_LOOTING
#ifdef SERV_PERIOD_PET
		&& GET(	m_wstrDestroyDate )
#endif SERV_PERIOD_PET
		;
}


SERIALIZE_DEFINE_PUT( KFieldPetInfo, obj, ks )
{
	return PUT( m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		&& PUT( m_iPetID )
#else
		&& PUT( m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		&& PUT( m_wstrPetName )
		&& PUT( m_cEvolutionStep )
		&& PUT( m_iIntimacy )
		;
}

SERIALIZE_DEFINE_GET( KFieldPetInfo, obj, ks )
{
	return GET( m_iPetUID )
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
		&& GET( m_iPetID )
#else
		&& GET( m_cPetID )
#endif //SERV_PETID_DATA_TYPE_CHANGE
		&& GET( m_wstrPetName )
		&& GET( m_cEvolutionStep )
		&& GET( m_iIntimacy )
		;
}

#endif SERV_PET_SYSTEM
//}}
//////////////////////////////////////////////////////////////////////////


//{{ 2011. 12. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM

SERIALIZE_DEFINE_PUT( KGamePlayStatus, obj, ks )
{
	return PUT( m_iMaxHP )
		&& PUT( m_iCurHP )
		&& PUT( m_iMaxMP )
		&& PUT( m_iCurMP )
		&& PUT( m_iCurHyperGage )
		&& PUT( m_cCurHyperCount )
		&& PUT( m_cCharAbilType )
		&& PUT( m_iCharAbilCount )
		&& PUT( m_mapSkillCoolTime )
		&& PUT( m_mapQuickSlotCoolTime )
		&& PUT( m_mapPetMP )
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& PUT( m_mapRidingPetCoolTime )
#endif	// SERV_RIDING_PET_SYSTM
		;
}

SERIALIZE_DEFINE_GET( KGamePlayStatus, obj, ks )
{
	return GET( m_iMaxHP )
		&& GET( m_iCurHP )
		&& GET( m_iMaxMP )
		&& GET( m_iCurMP )
		&& GET( m_iCurHyperGage )
		&& GET( m_cCurHyperCount )
		&& GET( m_cCharAbilType )
		&& GET( m_iCharAbilCount )
		&& GET( m_mapSkillCoolTime )
		&& GET( m_mapQuickSlotCoolTime )
		&& GET( m_mapPetMP )
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& GET( m_mapRidingPetCoolTime )
#endif	// SERV_RIDING_PET_SYSTM
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KPartyMemberStatus, obj, ks )
{
	return PUT( m_fHpPercent )
		&& PUT( m_fMpPercent )
		;
}

SERIALIZE_DEFINE_GET( KPartyMemberStatus, obj, ks )
{
	return GET( m_fHpPercent )
		&& GET( m_fMpPercent )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KGamePlayStatusContainer, obj, ks )
{
	return PUT( m_vecGamePlayStatus )
		;
}

SERIALIZE_DEFINE_GET( KGamePlayStatusContainer, obj, ks )
{
	return GET( m_vecGamePlayStatus )
		;
}


SERIALIZE_DEFINE_PUT( KLastPositionInfo, obj, ks )
{
	return PUT( m_iMapID )
		&& PUT( m_ucLastTouchLineIndex )
		&& PUT( m_usLastPosValue )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		&& PUT( m_bIgnoreLastTouch )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KLastPositionInfo, obj, ks )
{
	return GET( m_iMapID )
		&& GET( m_ucLastTouchLineIndex )
		&& GET( m_usLastPosValue )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
		&& GET( m_bIgnoreLastTouch )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}



SERIALIZE_DEFINE_PUT( KRoomUserInfo, obj, ks )
{
    return PUT( m_iGSUID )
		&& PUT( m_iOwnerUserUID )
		//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		&& PUT( m_sServerGroupID )
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}
		&& PUT( m_cAuthLevel )
		&& PUT( m_bMale )
		&& PUT( m_ucAge )
		&& PUT( m_nUnitUID )
		&& PUT( m_uiKNMSerialNum )
		&& PUT( m_cUnitClass )		
		&& PUT( m_wstrNickName )
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& PUT( m_iNumResurrectionStone )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		&& PUT( m_wstrIP )
		&& PUT( m_usPort )
		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		&& PUT( m_wstrInternalIP )
		&& PUT( m_usInternalPort )
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
		//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_iPartyUID )
		&& PUT( m_vecGamePlayStatus )		
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		&& PUT( m_vecBuffInfo )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		&& PUT( m_ucLevel )
		&& PUT( m_kGameStat )
		&& PUT( m_mapEquippedItem )
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		&& PUT( m_mapSpecialItem )
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		&& PUT( m_UnitSkillData )
		//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_bIsPvpNpc )
		&& PUT( m_iOfficialMatchCnt )
		&& PUT( m_iRating )
		&& PUT( m_iMaxRating )
		&& PUT( m_bIsWinBeforeMatch )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank)
		&& PUT( m_cRankForServer )
		&& PUT( m_fKFactor )
		&& PUT( m_bIsRedistributionUser )
#endif SERV_2012_PVP_SEASON2
		//}}
//#else
//		&& PUT( m_iPVPEmblem )
//#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2013. 03. 14	 사용하지 않는 RoomUserInfo 의 데이터 삭제 - 김민성
#ifdef SERV_DELETE_ROOM_USER_INFO_DATA
#else
		&& PUT( m_iSpiritMax )
		&& PUT( m_iSpirit )
#endif SERV_DELETE_ROOM_USER_INFO_DATA
		//}
		&& PUT( m_bIsGameBang )
#ifdef SERV_PC_BANG_TYPE
		&& PUT( m_iPcBangType )
#endif SERV_PC_BANG_TYPE
		&& PUT( m_bIsObserver )
		//{{ 2010. 05. 11  최육사	이벤트 보상 코드 정리
//#ifdef SERV_ROOMUSERINFO_REFAC
		&& PUT( m_mapBonusRate )
//#endif SERV_ROOMUSERINFO_REFAC
		//}}
		&& PUT( m_bIsRingofpvprebirth )
		&& PUT( m_bIsGuestUser )
		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
//#ifdef SERV_QUEST_CLEAR_EXPAND
		&& PUT( m_mapOngoingQuest )
//#else
//		&& PUT( m_mapDropQuestItembyIngQuest )
//#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
//#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iTitleID )
//#else
//		&& PUT( m_sTitleID )
//#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_wstrGuildName )
		&& PUT( m_iGuildUID )
		&& PUT( m_ucMemberShipGrade )
#endif GUILD_TEST
		//}}
		//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& PUT( m_setQuestInfo )
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		&& PUT(	m_bUseItem )
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
		&& PUT( m_setGoingQuestInfo )
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& PUT( m_bComeBackUser )
#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		&& PUT( m_bHenirReward )
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_setUseSkillBuffInPlay )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
		&& PUT( m_bEnterCashShop )
#endif SERV_VISIT_CASH_SHOP
		//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& PUT( m_iRidingPetUID )
		&& PUT( m_usRidingPetID )
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& PUT( m_cWeddingStatus )
		&& PUT( m_iLoverUnitUID )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_GROW_UP_SOCKET
		&& PUT( m_iEventQuestClearCount )
		&& PUT( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET

#ifdef SERV_BLESS_OF_GODDESS_EVENT
		&& PUT( m_bMaxLevelUnitInAccount )
#endif SERV_BLESS_OF-GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& PUT( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		
#ifdef SERV_RELATIONSHIP_EVENT_INT
		&& PUT( m_bCouple )
		&& PUT(	m_iRelationTargetUserUid )
		&& PUT( m_wstrRelationTargetUserNickname )
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
		&& PUT( m_iRecruiterUnitUID )
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		&& PUT( m_iValentineItemCount )
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& PUT( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

SERIALIZE_DEFINE_GET( KRoomUserInfo, obj, ks )
{
    return GET( m_iGSUID )
		&& GET( m_iOwnerUserUID )
		//{{ 2011. 12. 06	최육사	특정 서버군 이벤트 시스템
#ifdef SERV_SERVER_GROUP_EVENT_SYSTEM
		&& GET( m_sServerGroupID )
#endif SERV_SERVER_GROUP_EVENT_SYSTEM
		//}}
		&& GET( m_cAuthLevel )
		&& GET( m_bMale )
		&& GET( m_ucAge )
		&& GET( m_nUnitUID )
		&& GET( m_uiKNMSerialNum )
		&& GET( m_cUnitClass )
		&& GET( m_wstrNickName )
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& GET( m_iNumResurrectionStone )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		&& GET( m_wstrIP )
		&& GET( m_usPort )
		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		&& GET( m_wstrInternalIP )
		&& GET( m_usInternalPort )
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
		//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_iPartyUID )
		&& GET( m_vecGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 04. 21	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		&& GET( m_vecBuffInfo )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		&& GET( m_ucLevel )
		&& GET( m_kGameStat )
		&& GET( m_mapEquippedItem )
#ifdef SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		&& GET( m_mapSpecialItem )
#endif SERV_PAYMENT_ITEM_WITH_CONSUMING_OTHER_ITEM
		&& GET( m_UnitSkillData )
		//{{ 2011. 07. 11	최육사	대전 개편
//#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_bIsPvpNpc )
		&& GET( m_iOfficialMatchCnt )
		&& GET( m_iRating )
		&& GET( m_iMaxRating )
		&& GET( m_bIsWinBeforeMatch )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
		&& GET( m_cRankForServer )
		&& GET( m_fKFactor )
		&& GET( m_bIsRedistributionUser )
#endif SERV_2012_PVP_SEASON2
		//}}
//#else
//		&& GET( m_iPVPEmblem )
//#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2013. 03. 14	 사용하지 않는 RoomUserInfo 의 데이터 삭제 - 김민성
#ifdef SERV_DELETE_ROOM_USER_INFO_DATA
#else
		&& GET( m_iSpiritMax )
		&& GET( m_iSpirit )
#endif SERV_DELETE_ROOM_USER_INFO_DATA
		//}
		&& GET( m_bIsGameBang )
#ifdef SERV_PC_BANG_TYPE
		&& GET( m_iPcBangType )
#endif SERV_PC_BANG_TYPE
		&& GET( m_bIsObserver )
		//{{ 2010. 05. 11  최육사	이벤트 보상 코드 정리
//#ifdef SERV_ROOMUSERINFO_REFAC
		&& GET( m_mapBonusRate )
//#endif SERV_ROOMUSERINFO_REFAC
		//}}
		&& GET( m_bIsRingofpvprebirth )
		&& GET( m_bIsGuestUser )
		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
//#ifdef SERV_QUEST_CLEAR_EXPAND
		&& GET( m_mapOngoingQuest )
//#else
//		&& GET( m_mapDropQuestItembyIngQuest )
//#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
//#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iTitleID )
//#else
//		&& GET( m_sTitleID )
//#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_wstrGuildName )
		&& GET( m_iGuildUID )
		&& GET( m_ucMemberShipGrade )
#endif GUILD_TEST
		//}}
		//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 05. 04  김민성	던전 클리어시 아이템 지급 조건 추가
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		&& GET( m_setQuestInfo )
#ifdef SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
		&& GET(	m_bUseItem )
#endif //SERV_DUNGEON_CLEAR_PAYMENT_ITEM_FIX
#endif SERV_DUNGEON_CLEAR_PAYMENT_ITEM
		//}}
#ifdef SERV_PAYMENT_ITEM_ON_GOING_QUEST
		&& GET( m_setGoingQuestInfo )
#endif SERV_PAYMENT_ITEM_ON_GOING_QUEST
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& GET( m_bComeBackUser )
#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		&& GET( m_bHenirReward )
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2012. 05. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_setUseSkillBuffInPlay )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 09. 19   김민성   캐시샵 방문 상태
#ifdef SERV_VISIT_CASH_SHOP
		&& GET( m_bEnterCashShop )
#endif SERV_VISIT_CASH_SHOP
		//}}

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& GET( m_iRidingPetUID )
		&& GET( m_usRidingPetID )
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& GET( m_cWeddingStatus )
		&& GET( m_iLoverUnitUID )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_GROW_UP_SOCKET
		&& GET( m_iEventQuestClearCount )
		&& GET( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
		
#ifdef SERV_BLESS_OF_GODDESS_EVENT
		&& GET( m_bMaxLevelUnitInAccount )
#endif SERV_BLESS_OF-GODDESS_EVENT

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& GET( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		&& GET( m_bCouple )
		&& GET(	m_iRelationTargetUserUid )
		&& GET( m_wstrRelationTargetUserNickname )
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
		&& GET( m_iRecruiterUnitUID )
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
		&& GET( m_iValentineItemCount )
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& GET( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KRoomSlotInfo, obj, ks )
{
    return  PUT( m_Index )
        &&	PUT( m_SlotState )
        &&	PUT( m_bHost )
        &&	PUT( m_bReady )
		&&	PUT( m_bPitIn )
        &&  PUT( m_bTrade )
        &&	PUT( m_TeamNum )
        &&	PUT( m_kRoomUserInfo )
//#ifdef PVP_BOSS_COMBAT_TEST
//		&& PUT( m_bIsBoss )
//#endif PVP_BOSS_COMBAT_TEST
        ;
}

SERIALIZE_DEFINE_GET( KRoomSlotInfo, obj, ks )
{
    return  GET( m_Index )
        &&	GET( m_SlotState )
        &&	GET( m_bHost )
        &&	GET( m_bReady )
		&&	GET( m_bPitIn )
        &&  GET( m_bTrade )
        &&	GET( m_TeamNum )
        &&	GET( m_kRoomUserInfo )
//#ifdef PVP_BOSS_COMBAT_TEST
//		&& GET( m_bIsBoss )
//#endif PVP_BOSS_COMBAT_TEST
        ;
}

#ifdef SERV_GLOBAL_EVENT_TABLE
SERIALIZE_DEFINE_PUT( KGlobalEventTableData, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iEventID )
		&& PUT( m_usCategory )
		&& PUT( m_veciParamData )
		&& PUT( m_iEventType )
		&& PUT( m_iEventScriptID )
		&& PUT( m_cEventAccountType )
		;
}
SERIALIZE_DEFINE_GET( KGlobalEventTableData, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iEventID )
		&& GET( m_usCategory )
		&& GET( m_veciParamData )
		&& GET( m_iEventType )
		&& GET( m_iEventScriptID )
		&& GET( m_cEventAccountType )
		;
}
#endif //SERV_GLOBAL_EVENT_TABLE

//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
SERIALIZE_DEFINE_PUT( KECN_VERIFY_SERVER_CONNECT_ACK, obj, ks )
{
    return PUT( m_nOK )
		&& PUT( m_nUID )
		//{{ 2010. 04. 22  최육사	서버군 전체 통합
#ifdef SERV_INTEGRATION
		&& PUT( m_iDBRegServerGroupID )
		&& PUT( m_iLocalServerGroupID )
#else
		&& PUT( m_sGroupID )
#endif SERV_INTEGRATION
		//}}
		&& PUT( m_iServerUID )
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		&& PUT( m_iServerType )
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		&& PUT( m_strName )
		&& PUT( m_sMaxNum )
		&& PUT( m_kAddr )
		&& PUT( m_iVersion )
		;
}

SERIALIZE_DEFINE_GET( KECN_VERIFY_SERVER_CONNECT_ACK, obj, ks )
{
    return GET( m_nOK )
		&& GET( m_nUID )
		//{{ 2010. 04. 22  최육사	서버군 전체 통합
#ifdef SERV_INTEGRATION
		&& GET( m_iDBRegServerGroupID )
		&& GET( m_iLocalServerGroupID )
#else
		&& GET( m_sGroupID )
#endif SERV_INTEGRATION
		//}}
		&& GET( m_iServerUID )
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		&& GET( m_iServerType )
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		&& GET( m_strName )
		&& GET( m_sMaxNum )
		&& GET( m_kAddr )
		&& GET( m_iVersion )
		;
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}


SERIALIZE_DEFINE_PUT( KPacketOK, obj, ks )
{
    return  PUT( m_iOK );
}

SERIALIZE_DEFINE_GET( KPacketOK, obj, ks )
{
    return  GET( m_iOK );
}


//#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
SERIALIZE_DEFINE_PUT( KPacketReason, obj, ks )
{
    return  PUT( m_iReason );
}

SERIALIZE_DEFINE_GET( KPacketReason, obj, ks )
{
    return  GET( m_iReason );
}

//#endif	SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_OPEN_ROOM_REQ, obj, ks )
{
    return PUT( m_bQuickJoin )
		&& PUT( m_kRoomInfo )
		&& PUT( m_kRoomUserInfo )
		&& PUT( m_vecStudentUnitUID )
		//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& PUT( m_wstrChannelIP )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2011. 12. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_iCurEXP )
		&& PUT( m_iCurED )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KERM_OPEN_ROOM_REQ, obj, ks )
{
    return GET( m_bQuickJoin )
		&& GET( m_kRoomInfo )
		&& GET( m_kRoomUserInfo )
		&& GET( m_vecStudentUnitUID )
		//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& GET( m_wstrChannelIP )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2011. 12. 13	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_iCurEXP )
		&& GET( m_iCurED )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KQuestUpdate, obj, ks )
{
	return  PUT( m_iQuestID ) &&
			PUT( m_vecClearData );
}

SERIALIZE_DEFINE_GET( KQuestUpdate, obj, ks )
{
	return  GET( m_iQuestID ) &&
			GET( m_vecClearData );
}

//{{ 2007. 8. 16  최육사  차단옵션 정보 추가
SERIALIZE_DEFINE_PUT( KDenyOptions, obj, ks )
{	
	return  PUT( m_cDenyFriendShip )
		&&	PUT( m_cDenyInviteGuild )
		&&	PUT( m_cDenyParty )
		&&  PUT( m_cDenyPersonalTrade )
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&&  PUT( m_cDenyRequestCouple )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
        &&  PUT( m_cDenyInvitePracticePVP )
		;
}

SERIALIZE_DEFINE_GET( KDenyOptions, obj, ks )
{
	return  GET( m_cDenyFriendShip )
		&&	GET( m_cDenyInviteGuild )
		&&	GET( m_cDenyParty )
		&&  GET( m_cDenyPersonalTrade )
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&&  GET( m_cDenyRequestCouple )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		&&  GET( m_cDenyInvitePracticePVP );
}

//{{ 2008. 1. 31  최육사  
SERIALIZE_DEFINE_PUT( KChatBlackListUnit, obj, ks )
{
	return  PUT( m_iUnitUID )
		&&	PUT( m_wstrNickName )		
		;
}

SERIALIZE_DEFINE_GET( KChatBlackListUnit, obj, ks )
{
	return  GET( m_iUnitUID )
		&&	GET( m_wstrNickName )		
		;
}
//}}

//{{ 2008. 7. 14  최육사  랭킹
SERIALIZE_DEFINE_PUT( KRankerInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KRankerInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		;
}


//{{ 2009. 7. 1  최육사		헤니르의 시공
SERIALIZE_DEFINE_PUT( KHenirRankingInfo, obj, ks )
{
	return PUT( m_iRank )
		&& PUT( m_iStageCount )
		&& PUT( m_ulPlayTime )
		&& PUT( m_tRegDate )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-16	// 박세훈
		&& PUT( m_bytePlayStartedUserNum )
#endif // SERV_HENIR_RENEWAL_2013
		;
}

SERIALIZE_DEFINE_GET( KHenirRankingInfo, obj, ks )
{
	return GET( m_iRank )
		&& GET( m_iStageCount )
		&& GET( m_ulPlayTime )
		&& GET( m_tRegDate )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-16	// 박세훈
		&& GET( m_bytePlayStartedUserNum )
#endif // SERV_HENIR_RENEWAL_2013
		;
}
//}}

//{{ 2009. 7. 3  최육사		대전랭킹, 레벨랭킹
SERIALIZE_DEFINE_PUT( KDungeonRankingInfo, obj, ks )
{
	return PUT( m_iRank )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_iEXP )
		;
}

SERIALIZE_DEFINE_GET( KDungeonRankingInfo, obj, ks )
{
	return GET( m_iRank )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_iEXP )
		;
}

SERIALIZE_DEFINE_PUT( KPvpRankingInfo, obj, ks )
{
	return PUT( m_iRank )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )		
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank )
#else
		&& PUT( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& PUT( m_iRPoint )
#else
		&& PUT( m_cPvpEmblem )
		&& PUT( m_iLose )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_iWin )		
		;
}

SERIALIZE_DEFINE_GET( KPvpRankingInfo, obj, ks )
{
	return GET( m_iRank )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )		
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
#else
		&& GET( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& GET( m_iRPoint )
#else
		&& GET( m_cPvpEmblem )
		&& GET( m_iLose )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_iWin )		
		;
}
//}}

//{{ 2008. 6. 10  최육사  아이템 이벤트
SERIALIZE_DEFINE_PUT( KConnectTimeEventInfo, obj, ks )
{
	return PUT( m_iEventUID )
		&& PUT( m_wstrEventTime )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& PUT( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}

		//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		&& PUT( m_iScriptID )
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}
		;
}

SERIALIZE_DEFINE_GET( KConnectTimeEventInfo, obj, ks )
{
	return GET( m_iEventUID )
		&& GET( m_wstrEventTime )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& GET( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}

		//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		&& GET( m_iScriptID )
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}
		;
}

//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT

SERIALIZE_DEFINE_PUT( KCumulativeTimeEventInfo, obj, ks )
{
	return PUT( m_iEventUID )
		&& PUT( m_iCumulativeTime )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& PUT( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KCumulativeTimeEventInfo, obj, ks )
{
	return GET( m_iEventUID )
		&& GET( m_iCumulativeTime )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& GET( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}
		;
}

#endif CUMULATIVE_TIME_EVENT
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
SERIALIZE_DEFINE_PUT( KEGS_HENIR_REWARD_COUNT_NOT, obj, ks )
{
	return PUT( m_bUnLimited )
		&& PUT( m_iNormal )
		&& PUT( m_iPremium )
		&& PUT( m_iEvent )
		&& PUT( m_iPremiumMAX )
		&& PUT( m_iEventMAX )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
		&& PUT( m_iChallengeNormal )
		&& PUT( m_iChallengePremium )
		&& PUT( m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
		;
}

SERIALIZE_DEFINE_GET( KEGS_HENIR_REWARD_COUNT_NOT, obj, ks )
{
	return GET( m_bUnLimited )
		&& GET( m_iNormal )
		&& GET( m_iPremium )
		&& GET( m_iEvent )
		&& GET( m_iPremiumMAX )
		&& GET( m_iEventMAX )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
		&& GET( m_iChallengeNormal )
		&& GET( m_iChallengePremium )
		&& GET( m_iChallengeEvent )
#endif // SERV_HENIR_RENEWAL_2013
		;
}
#endif SERV_NEW_HENIR_TEST
//}}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_UNIT_INFO_REQ, obj, ks )
{
    return PUT( m_bFinal )		
		&& PUT( m_iEventID )		
		&& PUT( m_iUnitUID )
		&& PUT( m_iEXP )
		&& PUT( m_iLevel )
		&& PUT( m_iED )
		//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
		&& PUT( m_iRealDataED )
#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
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

#else
		&& PUT( m_iVSPoint )
		&& PUT( m_iVSPointMax )
#endif SERV_PVP_NEW_SYSTEM
		//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
#else // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iSPoint )
#endif // SERV_SKILL_PAGE_SYSTEM

		&& PUT( m_iWin )
		&& PUT( m_iLose )
		//{{ 2012. 02. 02	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_kLastPos )
#else
		&& PUT( m_iMapID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& PUT( m_mapDungeonClear )
		&& PUT( m_mapTCClear )
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& PUT( m_mapDungeonPlay )
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_kItemPositionUpdate )
		&& PUT( m_vecQuestData )
		&& PUT( m_vecMissionData )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iInitTitleID )
		&& PUT( m_iEquippedTitleID )
#else
		&& PUT( m_sInitTitleID )
		&& PUT( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_mapSkillSlotVector )
		&& PUT( m_wstrSkillSlotBEndDate )
#else // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_vecSkillSlot )
#endif // SERV_SKILL_PAGE_SYSTEM
		
		&& PUT( m_kDenyOptions )
		&& PUT( m_iSpirit )
		&& PUT( m_bIsSpiritUpdated )
		&& PUT( m_iNumResurrectionStone )
		//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&& PUT( m_iNumAutoPaymentResStone )
#endif AP_RESTONE
		//}}
		&& PUT( m_mapWishList )
		//{{ 2009. 7. 7  최육사		랭킹개편
		&& PUT( m_vecHenirRanking )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
		&& PUT( m_vecHenirHeroRanking )
#endif // SERV_HENIR_RENEWAL_2013
		//}}
		//{{ 2009. 10. 7  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_iGuildUID )
		&& PUT( m_iHonorPoint )
#endif GUILD_TEST
		//}}
		//{{ 2009. 12. 8  최육사	이벤트개편
#ifdef CUMULATIVE_TIME_EVENT
		&& PUT( m_vecUpdateEventTime )
#endif CUMULATIVE_TIME_EVENT
		//}}		
		//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST
		&& PUT( m_vecGiveUpQuestList )
#endif SERV_DAY_QUEST
		//}}
		//{{ 2010. 8. 4	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&& PUT( m_mapCharGameCount )
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		&& PUT( m_kHenirRewardCnt )
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		&& PUT( m_vecRecordBuffInfo )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
		&& PUT( m_iChangedLocalRankingSpirit )
		&& PUT( m_iChangedLocalRankingAP )
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& PUT( m_vecRidingPetList )
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& PUT( m_cUnitRelationshipType )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
		&& PUT( m_iAccumultionSpirit )
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& PUT( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
#ifdef SERV_CHINA_SPIRIT_EVENT
		&& PUT( m_arrChinaSpirit[0] )
		&& PUT( m_arrChinaSpirit[1] )
		&& PUT( m_arrChinaSpirit[2] )
		&& PUT( m_arrChinaSpirit[3] )
		&& PUT( m_arrChinaSpirit[4] )
		&& PUT( m_arrChinaSpirit[5] )
#endif SERV_CHINA_SPIRIT_EVENT
#ifdef SERV_ELESIS_UPDATE_EVENT
		&& PUT( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
        ;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_UNIT_INFO_REQ, obj, ks )
{
    return GET( m_bFinal )
		&& GET( m_iEventID )
		&& GET( m_iUnitUID )
		&& GET( m_iEXP )
		&& GET( m_iLevel )
		&& GET( m_iED )
		//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
		&& GET( m_iRealDataED )
#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
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

#else
		&& GET( m_iVSPoint )
		&& GET( m_iVSPointMax )
#endif SERV_PVP_NEW_SYSTEM
		//}}

#ifdef SERV_SKILL_PAGE_SYSTEM
#else // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iSPoint )
#endif // SERV_SKILL_PAGE_SYSTEM

		&& GET( m_iWin )
		&& GET( m_iLose )
		//{{ 2012. 02. 02	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_kLastPos )
#else
		&& GET( m_iMapID )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& GET( m_mapDungeonClear )
		&& GET( m_mapTCClear )
#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& GET( m_mapDungeonPlay )
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_kItemPositionUpdate )
		&& GET( m_vecQuestData )
		&& GET( m_vecMissionData )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iInitTitleID )
		&& GET( m_iEquippedTitleID )
#else
		&& GET( m_sInitTitleID )
		&& GET( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_mapSkillSlotVector )
		&& GET( m_wstrSkillSlotBEndDate )
#else // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_vecSkillSlot )
#endif // SERV_SKILL_PAGE_SYSTEM
		
		&& GET( m_kDenyOptions )
		&& GET( m_iSpirit )
		&& GET( m_bIsSpiritUpdated )
		&& GET( m_iNumResurrectionStone )
		//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&& GET( m_iNumAutoPaymentResStone )
#endif AP_RESTONE
		//}}
		&& GET( m_mapWishList )
		//{{ 2009. 7. 7  최육사		랭킹개편
		&& GET( m_vecHenirRanking )
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
		&& GET( m_vecHenirHeroRanking )
#endif // SERV_HENIR_RENEWAL_2013
		//}}
		//{{ 2009. 10. 7  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_iGuildUID )
		&& GET( m_iHonorPoint )
#endif GUILD_TEST
		//}}
		//{{ 2009. 12. 8  최육사	이벤트개편
#ifdef CUMULATIVE_TIME_EVENT
		&& GET( m_vecUpdateEventTime )
#endif CUMULATIVE_TIME_EVENT
		//}}		
		//{{ 2010. 02. 09  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST
		&& GET( m_vecGiveUpQuestList )
#endif SERV_DAY_QUEST
		//}}
		//{{ 2010. 8. 4	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2011. 01. 17	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&& GET( m_mapCharGameCount )
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 08. 12   김민성      헤니르 개편 
#ifdef SERV_NEW_HENIR_TEST
		&& GET( m_kHenirRewardCnt )
#endif SERV_NEW_HENIR_TEST
		//}}
		//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM		
		//}}
		//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
		&& GET( m_vecRecordBuffInfo )
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
		//}}
		//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
		&& GET( m_iChangedLocalRankingSpirit )
		&& GET( m_iChangedLocalRankingAP )
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& GET( m_vecRidingPetList )
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& GET( m_cUnitRelationshipType )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_ACCUMULATION_SPIRIT_SYSTEM
		&& GET( m_iAccumultionSpirit )
#endif SERV_ACCUMULATION_SPIRIT_SYSTEM

#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& GET( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_CHINA_SPIRIT_EVENT
		&& GET( m_arrChinaSpirit[0] )
		&& GET( m_arrChinaSpirit[1] )
		&& GET( m_arrChinaSpirit[2] )
		&& GET( m_arrChinaSpirit[3] )
		&& GET( m_arrChinaSpirit[4] )
		&& GET( m_arrChinaSpirit[5] )
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
		&& GET( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
        ;
}
//}}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_UNIT_INFO_ACK, obj, ks )
{
    return PUT( m_iOK )		
		&& PUT( m_iEventID )		
		&& PUT( m_iEXP )
		&& PUT( m_iED )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_iOfficialMatchCnt )
		&& PUT( m_iRating )
		&& PUT( m_iMaxRating )
		&& PUT( m_iRPoint )
		&& PUT( m_iAPoint )
		&& PUT( m_iWin )
		&& PUT( m_iLose )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_fKFactor )
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		&& PUT( m_iVSPoint )
		&& PUT( m_iVSPointMax )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_vecDungeonClear )
		&& PUT( m_vecTCClear )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_kItemPositionUpdate )
		&& PUT( m_iNumResurrectionStone )
		//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&& PUT( m_iNumAutoPaymentResStone )
#endif AP_RESTONE
		//}}
		//{{ 2010. 8. 4	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
		&& PUT( m_iChangedLocalRankingSpirit )
		&& PUT( m_iChangedLocalRankingAP )
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& PUT( m_vecRidingPetUID )
#endif	// SERV_RIDING_PET_SYSTM
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_UNIT_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iEventID )
		&& GET( m_iEXP )
		&& GET( m_iED )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_iOfficialMatchCnt )
		&& GET( m_iRating )
		&& GET( m_iMaxRating )
		&& GET( m_iRPoint )
		&& GET( m_iAPoint )
		&& GET( m_iWin )
		&& GET( m_iLose )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_fKFactor )
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		&& GET( m_iVSPoint )
		&& GET( m_iVSPointMax )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_vecDungeonClear )
		&& GET( m_vecTCClear )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_kItemPositionUpdate )
		&& GET( m_iNumResurrectionStone )
		//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&& GET( m_iNumAutoPaymentResStone )
#endif AP_RESTONE
		//}}
		//{{ 2010. 8. 4	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2013. 3. 17	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
		&& GET( m_iChangedLocalRankingSpirit )
		&& GET( m_iChangedLocalRankingAP )
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& GET( m_vecRidingPetUID )
#endif	// SERV_RIDING_PET_SYSTM
		;
}


SERIALIZE_DEFINE_PUT( KERM_UPDATE_PVP_UNIT_INFO_NOT, obj, ks )
{
	return PUT( m_iEXP )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_iCurrentRating )
		&& PUT( m_iCurrentMaxRating )
		&& PUT( m_iCurrentRPoint )
		&& PUT( m_iCurrentAPoint )
		&& PUT( m_bIsOfficialMatch )
		&& PUT( m_iRating )
		&& PUT( m_iRPoint )
		&& PUT( m_iAPoint )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank )
		&& PUT( m_cRankForServer )
		&& PUT( m_fKFactorIncrement )
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		&& PUT( m_iVSPoint )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_iWin	)
		&& PUT( m_iLose )		
		&& PUT( m_bOutRoom )		
		&& PUT( m_iMemberCount )
		&& PUT( m_iPlayTime )
		&& PUT( m_mapItem )
		;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_PVP_UNIT_INFO_NOT, obj, ks )
{
	return GET( m_iEXP )	
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_iCurrentRating )
		&& GET( m_iCurrentMaxRating )
		&& GET( m_iCurrentRPoint )
		&& GET( m_iCurrentAPoint )
		&& GET( m_bIsOfficialMatch )
		&& GET( m_iRating )
		&& GET( m_iRPoint )
		&& GET( m_iAPoint )

		//{{ 2012. 06. 22	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
		&& GET( m_cRankForServer )
		&& GET( m_fKFactorIncrement )
#endif SERV_2012_PVP_SEASON2
		//}}

#else
		&& GET( m_iVSPoint )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_iWin	)
		&& GET( m_iLose )		
		&& GET( m_bOutRoom )		
		&& GET( m_iMemberCount )
		&& GET( m_iPlayTime )		
		&& GET( m_mapItem )
        ;
}

SERIALIZE_DEFINE_PUT( KERM_UPDATE_DUNGEON_UNIT_INFO_NOT, obj, ks )
{
	return PUT( m_iEXP )
		&& PUT( m_iED )
		&& PUT( m_iPvPGameType )
		&& PUT( m_iDungeonID )
		&& PUT( m_iUserCount )
		&& PUT( m_bDungeonClear )
		&& PUT( m_mapGetItem )
		//{{ 2009. 7. 28  최육사	던전결과창보상
		&& PUT( m_mapResultItem )
		&& PUT( m_iDropItemUID )
		//}}
		&& PUT( m_bOutRoom )
		&& PUT( m_iPlayTime )
		&& PUT( m_iMaxScore )
		&& PUT( m_cMaxTotalRank )
		&& PUT( m_bIsTutorialSuccess )
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		&& PUT( m_bGetWorldMissionReward )
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		//{{ 2009. 7. 1  최육사		헤니르 시공
		&& PUT( m_kHenirRankingInfo )
		//}}
		//{{ 2012. 04. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_bStartedByAutoParty )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
        ;
}

SERIALIZE_DEFINE_GET( KERM_UPDATE_DUNGEON_UNIT_INFO_NOT, obj, ks )
{
	return GET( m_iEXP )
		&& GET( m_iED )
		&& GET( m_iPvPGameType )
		&& GET( m_iDungeonID )
		&& GET( m_iUserCount )
		&& GET( m_bDungeonClear )
		&& GET( m_mapGetItem )
		//{{ 2009. 7. 28  최육사	던전결과창보상
		&& GET( m_mapResultItem )
		&& GET( m_iDropItemUID )
		//}}
		&& GET( m_bOutRoom )
		&& GET( m_iPlayTime )
		&& GET( m_iMaxScore )
		&& GET( m_cMaxTotalRank )
		&& GET( m_bIsTutorialSuccess )
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		&& GET( m_bGetWorldMissionReward )
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		//{{ 2009. 7. 1  최육사		헤니르 시공
		&& GET( m_kHenirRankingInfo )
		//}}
		//{{ 2012. 04. 16	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_bStartedByAutoParty )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
        ;
}


SERIALIZE_DEFINE_PUT( KStatisticsDetailData, obj, ks )
{
	return  PUT( m_iNum ) &&
		PUT( m_wszString ) 
		;
}

SERIALIZE_DEFINE_GET( KStatisticsDetailData, obj, ks )
{
	return  GET( m_iNum ) &&
		GET( m_wszString )
		;
}



SERIALIZE_DEFINE_PUT( KErrorMsgInfo, obj, ks )
{
	return  PUT( m_iErrorMsgID ) &&
			PUT( m_wstrErrorMsg ) 
			;
}

SERIALIZE_DEFINE_GET( KErrorMsgInfo, obj, ks )
{
	return  GET( m_iErrorMsgID ) &&
			GET( m_wstrErrorMsg )
			;
}

SERIALIZE_DEFINE_PUT( KStatisticsKey, obj, ks )
{
    return  PUT( m_vecIntKey ) &&
            PUT( m_vecStringKey );
}

SERIALIZE_DEFINE_GET( KStatisticsKey, obj, ks )
{
    return  GET( m_vecIntKey ) &&
            GET( m_vecStringKey );
}

//{{ 2010. 04. 02  최육사	일일퀘스트
SERIALIZE_DEFINE_PUT( KCompleteQuestInfo, obj, ks )
{
	return PUT( m_iQuestID )
		&& PUT( m_iCompleteCount )
		&& PUT( m_tCompleteDate )
		;
}

SERIALIZE_DEFINE_GET( KCompleteQuestInfo, obj, ks )
{
	return GET( m_iQuestID )
		&& GET( m_iCompleteCount )
		&& GET( m_tCompleteDate )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KSubQuestInstance, obj, ks )
{
	return  PUT( m_iID )
		&&	PUT( m_ucClearData )
		&&	PUT( m_bIsSuccess )
		;
}

SERIALIZE_DEFINE_GET( KSubQuestInstance, obj, ks )
{
	return  GET( m_iID )
		&&	GET( m_ucClearData )
		&&	GET( m_bIsSuccess )
		;
}


SERIALIZE_DEFINE_PUT( KQuestInstance, obj, ks )
{
	return  PUT( m_iID ) 
		&&	PUT( m_OwnorUnitUID )
		&&	PUT( m_vecSubQuestInstance )
		;
}

SERIALIZE_DEFINE_GET( KQuestInstance, obj, ks )
{
	return  GET( m_iID ) 
		&&	GET( m_OwnorUnitUID )
		&&	GET( m_vecSubQuestInstance )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KNPCUnitReq, obj, ks )
{
	return PUT( m_UID )
		&& PUT( m_NPCID )
		&& PUT( m_Level )
		&& PUT( m_vPos )
		&& PUT( m_nStartPos )
		//{{ 2011. 11. 15	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_vecPetrolLineIndex )
		&& PUT( m_vecPlayLineIndex )
		&& PUT( m_iGroupID )
		&& PUT( m_bAggressive )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& PUT( m_bActive )
		&& PUT( m_fDelayTime )
		&& PUT( m_KeyCode )
		&& PUT( m_bFocusCamera )
		&& PUT( m_bShowGage )
		&& PUT( m_AddPos )
		&& PUT( m_bHasBossGage )
		&& PUT( m_bShowBossName )
		&& PUT( m_bShowSubBossName )
		&& PUT( m_bSiegeMode )		
		&& PUT( m_bIsRight )
		&& PUT( m_bNoDrop )
		&& PUT( m_fUnitScale )
		&& PUT( m_cTeamNum )
		&& PUT( m_cAIType )
		&& PUT( m_iAllyUID )
#ifdef SERV_TRAPPING_RANGER_TEST
		&& PUT( m_cAllyTeam )
#endif SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 12. 18  최육사	던전경험치개편
		&& PUT( m_cMonsterGrade )
		//}}
		//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_BOSS_GAUGE_HP_LINES
		&& PUT( m_usBossGaugeHPLines )
#endif SERV_BOSS_GAUGE_HP_LINES
		//}}
#ifdef SERV_ALLY_NPC
		&& PUT( m_bAllyNpc )
#endif
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		&& PUT( m_bAttributeNpc )
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		//{{ 2012. 06. 20	박세훈	몬스터 소환 카드
#ifdef SERV_MONSTER_SUMMON_CARD
		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& PUT( m_cCreateMonsterType )
#else
		&& PUT( m_bIsMonsterCard )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
#endif SERV_MONSTER_SUMMON_CARD
		//}}
#ifdef SERV_MODIFY_DUNGEON_STAGING
		&& PUT( m_fShowBossNameDelayTime )
		&& PUT( m_bStopAtStartState )
#endif //SERV_MODIFY_DUNGEON_STAGING
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		&& PUT( m_iBossGroupID )
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
		;
}

SERIALIZE_DEFINE_GET( KNPCUnitReq, obj, ks )
{
	return GET( m_UID )
		&& GET( m_NPCID )
		&& GET( m_Level )
		&& GET( m_vPos )
		&& GET( m_nStartPos )
		//{{ 2011. 11. 15	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_vecPetrolLineIndex )
		&& GET( m_vecPlayLineIndex )
		&& GET( m_iGroupID )
		&& GET( m_bAggressive )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		&& GET( m_bActive )
		&& GET( m_fDelayTime )
		&& GET( m_KeyCode )
		&& GET( m_bFocusCamera )
		&& GET( m_bShowGage )
		&& GET( m_AddPos )
		&& GET( m_bHasBossGage )
		&& GET( m_bShowBossName )
		&& GET( m_bShowSubBossName )
		&& GET( m_bSiegeMode )
		&& GET( m_bIsRight )		
		&& GET( m_bNoDrop )
		&& GET( m_fUnitScale )
		&& GET( m_cTeamNum )
		&& GET( m_cAIType )
		&& GET( m_iAllyUID )
#ifdef SERV_TRAPPING_RANGER_TEST
		&& GET( m_cAllyTeam )
#endif SERV_TRAPPING_RANGER_TEST
		//{{ 2009. 12. 18  최육사	던전경험치개편
		&& GET( m_cMonsterGrade )
		//}}
		//{{ 2011. 01. 26	최육사	퀘스트 클리어 조건 추가
#ifdef SERV_BOSS_GAUGE_HP_LINES
		&& GET( m_usBossGaugeHPLines )
#endif SERV_BOSS_GAUGE_HP_LINES
		//}}
#ifdef SERV_ALLY_NPC
		&& GET( m_bAllyNpc )
#endif
		//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
		&& GET( m_bAttributeNpc )
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 2011. 04. 13  김민성  글로벌 서버 추가
		//{{ 2012. 06. 20	박세훈	몬스터 소환 카드
#ifdef SERV_MONSTER_SUMMON_CARD
		//{{ 2013. 04. 15	최육사	어둠의 문 개편
#ifdef SERV_NEW_DEFENCE_DUNGEON
		&& GET( m_cCreateMonsterType )
#else
		&& GET( m_bIsMonsterCard )
#endif SERV_NEW_DEFENCE_DUNGEON
		//}}
#endif SERV_MONSTER_SUMMON_CARD
		//}}
#ifdef SERV_MODIFY_DUNGEON_STAGING
		&& GET( m_fShowBossNameDelayTime )
		&& GET( m_bStopAtStartState ) 
#endif //SERV_MODIFY_DUNGEON_STAGING
		//{{ 2013. 02. 15   필드 중간 보스 - 김민성
#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
		&& GET( m_iBossGroupID )
#endif SERV_BATTLEFIELD_MIDDLE_BOSS
		//}
		;
}



//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KNPCList, obj, ks )
{
	return PUT( m_NPCList )
		;
}

SERIALIZE_DEFINE_GET( KNPCList, obj, ks )
{
	return GET( m_NPCList )
		;
}


SERIALIZE_DEFINE_PUT( KERM_NPC_UNIT_DIE_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		&&	PUT( m_iNPCID )
		;
}

SERIALIZE_DEFINE_GET( KERM_NPC_UNIT_DIE_ACK, obj, ks )
{
	return  GET( m_iOK ) 
		&&	GET( m_iNPCID )
		;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KQuestReq, obj, ks )
{
	return	PUT( m_UnitUID )
		&&	PUT( m_iQuestID )
		;
}

SERIALIZE_DEFINE_GET( KQuestReq, obj, ks )
{
	return  GET( m_UnitUID )
		&&	GET( m_iQuestID )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KQuestAck, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_iQuestID )
		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		;
}

SERIALIZE_DEFINE_GET( KQuestAck, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iQuestID )
		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KDBE_NEW_QUEST_REQ, obj, ks )
{
	return PUT( m_UnitUID )
		&& PUT( m_iQuestID )
		&& PUT( m_iTalkNPCID )
		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		&& PUT( m_UserUID )
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}
		;
}

SERIALIZE_DEFINE_GET( KDBE_NEW_QUEST_REQ, obj, ks )
{
	return GET( m_UnitUID )
		&& GET( m_iQuestID )
		&& GET( m_iTalkNPCID )
		//{{ 2010. 10. 26	최육사	퀘스트 조건 추가
#ifdef SERV_QUEST_CLEAR_EXPAND
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
#endif SERV_QUEST_CLEAR_EXPAND
		//}}
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		&& GET( m_UserUID )
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}
		;
}
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_QUEST_COMPLETE_REQ, obj, ks )
{
	return PUT( m_UnitUID )
		&& PUT( m_iQuestID )
		&& PUT( m_bIsRepeat )
		&& PUT( m_bIsChangeJob )
		&& PUT( m_cChangeUnitClass )
		&& PUT( m_bIsNew )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )			
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		&& PUT( m_bAutoComplete )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KDBE_QUEST_COMPLETE_REQ, obj, ks )
{
	return GET( m_UnitUID )
		&& GET( m_iQuestID )
		&& GET( m_bIsRepeat )
		&& GET( m_bIsChangeJob )
		&& GET( m_cChangeUnitClass )
		&& GET( m_bIsNew )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )			
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		&& GET( m_bAutoComplete )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iTheNumberOfSkillPagesAvailable )
#endif // SERV_SKILL_PAGE_SYSTEM
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDBE_QUEST_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
		&& PUT( m_kCompleteQuestInfo )
#else
		&& PUT( m_iQuestID )
#endif SERV_DAILY_QUEST
		//}}
		&& PUT( m_bIsChangeJob )
		&& PUT( m_cChangeUnitClass )
		&& PUT( m_bIsNew )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		&& PUT( m_bAutoComplete )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& PUT( m_iNewDefaultSkill1 )
		&& PUT( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}

SERIALIZE_DEFINE_GET( KDBE_QUEST_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		//{{ 2010. 04. 02  최육사	일일퀘스트
#ifdef SERV_DAILY_QUEST
		&& GET( m_kCompleteQuestInfo )
#else
		&& GET( m_iQuestID )
#endif SERV_DAILY_QUEST
		//}}
		&& GET( m_bIsChangeJob )
		&& GET( m_cChangeUnitClass )
		&& GET( m_bIsNew )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		//{{ 2012. 04. 17	최육사	퀘스트 조건 마을 입장
#ifdef SERV_ENTER_FIELD_QUEST_CLEAR
		&& GET( m_bAutoComplete )
#endif SERV_ENTER_FIELD_QUEST_CLEAR
		//}}
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
		&& GET( m_iNewDefaultSkill1 )
		&& GET( m_iNewDefaultSkill2 )
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_REGISTER_UNIT_NOT, obj, ks )
{
	return PUT( m_nUnitUID )
		&& PUT( m_uiKNMSerialNum )
		&& PUT( m_wstrUnitNickName )
		//{{ 2009. 4. 1  최육사		친구차단
		&& PUT( m_bDenyFriendShip )
        //}}
        && PUT( m_setFriendUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_REGISTER_UNIT_NOT, obj, ks )
{
	return GET( m_nUnitUID )
		&& GET( m_uiKNMSerialNum )
		&& GET( m_wstrUnitNickName )
		//{{ 2009. 4. 1  최육사		친구차단
		&& GET( m_bDenyFriendShip )
		//}}
        && GET( m_setFriendUID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KConnectionUnitInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_nKNMSN )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_wstrCharName )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank )
#else
		&& PUT( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& PUT( m_iRPoint )
#else
		&& PUT( m_iPVPEmblem )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_uiPVPRanking )
		&& PUT( m_iWin )
		&& PUT( m_iLose )
		&& PUT( m_uiKillNum )
		//{{ 2009. 4. 17  최육사	확장
		&& PUT( m_iChannelID )
		//}}
		&& PUT( m_cState )
		&& PUT( m_iStateCode )
		//{{ 2009. 4. 17  최육사	확장
		&& PUT( m_iPartyUID )
		//}}
		&& PUT( m_iRoomUID )
		&& PUT( m_bPublic )
		&& PUT( m_bCanIntrude )
		&& PUT( m_iDungeonID )
		//{{ 2009. 4. 21  최육사	확장
		&& PUT( m_iRoomListID )
		//}}
		//{{ 2009. 10. 23  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_wstrGuildName )
#endif GUILD_TEST
		//}}
#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
		&& PUT( m_kLocalRankingUserInfo )
#endif	// SERV_LOCAL_RANKING_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KConnectionUnitInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_nKNMSN )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_wstrCharName )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
#else
		&& GET( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& GET( m_iRPoint )
#else
		&& GET( m_iPVPEmblem )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_uiPVPRanking )
		&& GET( m_iWin )
		&& GET( m_iLose )
		&& GET( m_uiKillNum )
		//{{ 2009. 4. 17  최육사	확장
		&& GET( m_iChannelID )
		//}}
		&& GET( m_cState )
		&& GET( m_iStateCode )
		//{{ 2009. 4. 17  최육사	확장
		&& GET( m_iPartyUID )
		//}}
		&& GET( m_iRoomUID )
		&& GET( m_bPublic )
		&& GET( m_bCanIntrude )
		&& GET( m_iDungeonID )
		//{{ 2009. 4. 21  최육사	확장
		&& GET( m_iRoomListID )
		//}}
		//{{ 2009. 10. 23  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_wstrGuildName )
#endif GUILD_TEST
		//}}
#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
		&& GET( m_kLocalRankingUserInfo )
#endif	// SERV_LOCAL_RANKING_SYSTEM
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_GET_UNIT_INFO_REQ, obj, ks )
{
	return PUT( m_usEventID )
		&& PUT( m_iDemandUserUID )
		&& PUT( m_iDemandGSUID )
		&& PUT( m_nKNMSN )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_UNIT_INFO_REQ, obj, ks )
{
	return GET( m_usEventID )
		&& GET( m_iDemandUserUID )
		&& GET( m_iDemandGSUID )
		&& GET( m_nKNMSN )
		&& GET( m_wstrNickName )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_GET_UNIT_INFO_ACK, obj, ks )
{
	return PUT( m_usEventID )
		&& PUT( m_iDemandUserUID )
		&& PUT( m_iDemandGSUID )
		&& PUT( m_iOK )
		&& PUT( m_kCUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_GET_UNIT_INFO_ACK, obj, ks )
{
	return GET( m_usEventID )
		&& GET( m_iDemandUserUID )
		&& GET( m_iDemandGSUID )
		&& GET( m_iOK )
		&& GET( m_kCUnitInfo )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KSquareInfo, obj, ks )
{
	return PUT( m_cSquareType )
		//{{ 2010. 11. 1	최육사	광장 리스트 개선
#ifdef SERV_SQUARE_LIST_SERV_UID
		&& PUT( m_cSortIndex )
#endif SERV_SQUARE_LIST_SERV_UID
		//}}
		&& PUT( m_iSquareUID )
		&& PUT( m_wstrRoomName	)
		&& PUT( m_MaxSlot )
		&& PUT( m_JoinSlot )
		&& PUT( m_RelayIP )
		&& PUT( m_Port )
		;
}

SERIALIZE_DEFINE_GET( KSquareInfo, obj, ks )
{
	return GET( m_cSquareType )
		//{{ 2010. 11. 1	최육사	광장 리스트 개선
#ifdef SERV_SQUARE_LIST_SERV_UID
		&& GET( m_cSortIndex )
#endif SERV_SQUARE_LIST_SERV_UID
		//}}
		&& GET( m_iSquareUID )
		&& GET( m_wstrRoomName )
		&& GET( m_MaxSlot )
		&& GET( m_JoinSlot )
		&& GET( m_RelayIP )
		&& GET( m_Port )
		;
}

SERIALIZE_DEFINE_PUT( KSquareUserInfo, obj, ks )
{
    return PUT( m_iGSUID )		
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_wstrIP )
		&& PUT( m_usPort )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank )
#else
		&& PUT( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		&& PUT( m_cPVPEmblem )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_bIsParty )
		&& PUT( m_cPersonalShopState )
		&& PUT( m_cPersonalShopType )
		&& PUT( m_wstrPersonalShopName )
		&& PUT( m_vecEquippedItem )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iTitleID )
#else
		&& PUT( m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_wstrGuildName	)
#endif GUILD_TEST
		//}}
		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		&& PUT( m_iED )
#endif SERV_ED_MONITORING_IN_GAME
		//}}
#ifdef SERV_GROW_UP_SOCKET
		&& PUT( m_iEventQuestClearCount )
		&& PUT( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& PUT( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

SERIALIZE_DEFINE_GET( KSquareUserInfo, obj, ks )
{
    return GET( m_iGSUID )		
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_wstrIP )
		&& GET( m_usPort )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
#else
		&& GET( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		&& GET( m_cPVPEmblem )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_bIsParty )
		&& GET( m_cPersonalShopState )
		&& GET( m_cPersonalShopType )
		&& GET( m_wstrPersonalShopName )
		&& GET( m_vecEquippedItem )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iTitleID )
#else
		&& GET( m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_wstrGuildName	)
#endif GUILD_TEST
		//}}
		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		&& GET( m_iED )
#endif SERV_ED_MONITORING_IN_GAME
		//}}
#ifdef SERV_GROW_UP_SOCKET
		&& GET( m_iEventQuestClearCount )
		&& GET( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& GET( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KPartyInfo, obj, ks )
{
    return PUT( m_iPartyUID )
		//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_cPartyType )
#endif SERV_PVP_NEW_SYSTEM
		//}}
        && PUT( m_wstrPartyName )
		&& PUT( m_JoinSlot )
		&& PUT( m_MaxSlot )
		&& PUT( m_bPublic )
		&& PUT( m_iDungeonID )
		&& PUT( m_DifficultyLevel )		
		&& PUT( m_cGetItemType )
		&& PUT( m_cDungeonMode )
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_sPartyFever )
#else
		&& PUT( m_ucPartyFever )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		&& PUT( m_bCheckLowLevel )
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		//}}		
		//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& PUT( m_cPVPGameType )		
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& PUT( m_bComeBackBuff )
		&& PUT( m_wsrtComeBackBuffEnd )
#endif SERV_COME_BACK_USER_REWARD
		//}}
		;
}

SERIALIZE_DEFINE_GET( KPartyInfo, obj, ks )
{
	return GET( m_iPartyUID )
		//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_cPartyType )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_wstrPartyName )
		&& GET( m_JoinSlot )
		&& GET( m_MaxSlot )
		&& GET( m_bPublic )
		&& GET( m_iDungeonID )
		&& GET( m_DifficultyLevel )
		&& GET( m_cGetItemType )
		&& GET( m_cDungeonMode )
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_sPartyFever )
#else
		&& GET( m_ucPartyFever )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2011. 03. 21  김민성	던전 난이도 보정
#ifdef SERV_DUNGEON_DIFFICULTY_REVISION
		&& GET( m_bCheckLowLevel )
#endif SERV_DUNGEON_DIFFICULTY_REVISION
		//}}
		//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		&& GET( m_cPVPGameType )		
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& GET( m_bComeBackBuff )
		&& GET( m_wsrtComeBackBuffEnd )
#endif SERV_COME_BACK_USER_REWARD
		//}}		
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2012. 01. 26	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
SERIALIZE_DEFINE_PUT( KCreatePartyInfo, obj, ks )
{
	return PUT( m_cPartyType )
		&& PUT( m_iDungeonID )
		&& PUT( m_DifficultyLevel )
		&& PUT( m_cGetItemType )
		&& PUT( m_cDungeonMode )
		&& PUT( m_bCheckLowLevel )
		&& PUT( m_cPVPGameType )
		;
}

SERIALIZE_DEFINE_GET( KCreatePartyInfo, obj, ks )
{
	return GET( m_cPartyType )
		&& GET( m_iDungeonID )
		&& GET( m_DifficultyLevel )
		&& GET( m_cGetItemType )
		&& GET( m_cDungeonMode )
		&& GET( m_bCheckLowLevel )
		&& GET( m_cPVPGameType )
		;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 01. 26  최육사	파티통합
#ifdef SERV_CHANNEL_PARTY

SERIALIZE_DEFINE_PUT( KPartyUserInfo, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iChannelID )
		&& PUT( m_iFieldUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_wstrNickName )
		&& PUT( m_ucLevel )
		&& PUT( m_bIsGuestUser )
		&& PUT( m_cState )
		&& PUT( m_iStateCode )
		&& PUT( m_bReady )
		&& PUT( m_bHost )
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& PUT( m_bComeBack )
		&& PUT( m_wsrtComeBackEnd )
#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank )
		&& PUT( m_iOfficialMatchCnt )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& PUT( m_iRating )
		&& PUT( m_iRPoint )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2012. 05. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		&& PUT( m_wstrIP )
		&& PUT( m_usPort )
		&& PUT( m_dwPingScore )
		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		&& PUT( m_wstrInternalIP )
		&& PUT( m_usInternalPort )
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
#endif SERV_CHOOSE_FASTEST_HOST
		//}
		;
}

SERIALIZE_DEFINE_GET( KPartyUserInfo, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iChannelID )
		&& GET( m_iFieldUID )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_wstrNickName )
		&& GET( m_ucLevel )
		&& GET( m_bIsGuestUser )
		&& GET( m_cState )
		&& GET( m_iStateCode )
		&& GET( m_bReady )
		&& GET( m_bHost )
		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		&& GET( m_bComeBack )
		&& GET( m_wsrtComeBackEnd )
#endif SERV_COME_BACK_USER_REWARD
		//}} 
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
		&& GET( m_iOfficialMatchCnt )
#endif SERV_2012_PVP_SEASON2
		//}}
		&& GET( m_iRating )
		&& GET( m_iRPoint )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		//{{ 2012. 05. 29	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 핑 상태체크 호스트 변경 - 김민성
#ifdef SERV_CHOOSE_FASTEST_HOST
		&& GET( m_wstrIP )
		&& GET( m_usPort )
		&& GET( m_dwPingScore )
#endif SERV_CHOOSE_FASTEST_HOST
		//}
		//{{ 2013. 1. 8	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
//#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
		&& GET( m_wstrInternalIP )
		&& GET( m_usInternalPort )
//#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
		//}}
		;
}

//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KPartyUserInfo, obj, ks )
{
    return PUT( m_iUnitUID )
        && PUT( m_wstrNickName )
        && PUT( m_cUnitClass )
        && PUT( m_ucLevel )
        && PUT( m_cState )
		&& PUT( m_iStateCode )
		&& PUT( m_bReady )
		&& PUT( m_bHost )
		;
}

SERIALIZE_DEFINE_GET( KPartyUserInfo, obj, ks )
{
    return GET( m_iUnitUID )
        && GET( m_wstrNickName )
        && GET( m_cUnitClass )
        && GET( m_ucLevel )
        && GET( m_cState )
		&& GET( m_iStateCode )
		&& GET( m_bReady )
		&& GET( m_bHost )
		;
}

#endif SERV_CHANNEL_PARTY
//}}
//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KPartyListUserInfo, obj, ks )
{
	return PUT( m_iChannelID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_cState )
		&& PUT( m_iStateCode )
		&& PUT( m_bHost )
		;
}

SERIALIZE_DEFINE_GET( KPartyListUserInfo, obj, ks )
{
	return GET( m_iChannelID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_cState )
		&& GET( m_iStateCode )
		&& GET( m_bHost )
		;
}


SERIALIZE_DEFINE_PUT( KPartyListInfo, obj, ks )
{
	return	PUT( m_kPartyInfo )
		&&  PUT( m_vecPartyListUserInfo )
		;
}

SERIALIZE_DEFINE_GET( KPartyListInfo, obj, ks )
{
	return  GET( m_kPartyInfo )
		&&  GET( m_vecPartyListUserInfo )
		;
}


SERIALIZE_DEFINE_PUT( KPartyRoomUserInfo, obj, ks )
{
	return PUT( m_kRoomUserInfo )
		&& PUT( m_vecStudentUnitUID )
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_kOldPartyInfo )
		&& PUT( m_bAutoPartyBonus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& PUT( m_wstrChannelIP )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		&& PUT( m_iCurEXP )
		&& PUT( m_iCurED )
		//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		&& PUT( m_iAutoPartyWaitTime )
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
		;
}

SERIALIZE_DEFINE_GET( KPartyRoomUserInfo, obj, ks )
{
	return GET( m_kRoomUserInfo )
		&& GET( m_vecStudentUnitUID )
		//{{ 2012. 02. 28	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_kOldPartyInfo )
		&& GET( m_bAutoPartyBonus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		//{{ 2012. 07. 03	최육사		던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& GET( m_wstrChannelIP )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		&& GET( m_iCurEXP )
		&& GET( m_iCurED )
		//{{ 자동 파티로 던전 시작 시 매칭 대기 시간 로그 추가 - 김민성
#ifdef SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		&& GET( m_iAutoPartyWaitTime )
#endif SERV_AUTO_PARTY_MATCHING_REQUIRED_TIME_LOG
		//}
		;
}


SERIALIZE_DEFINE_PUT( KTradeUserInfo, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		//{{ 2009. 4. 28  최육사	최대 ED제한
		&& PUT( m_iED )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KTradeUserInfo, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		//{{ 2009. 4. 28  최육사	최대 ED제한
		&& GET( m_iED )
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KPersonalShopUserInfo, obj, ks )
{
	return	PUT( m_iGSUID ) &&
			PUT( m_iUserUID ) &&
			PUT( m_iUnitUID ) &&
			PUT( m_wstrNickName ) &&
			PUT( m_bIsSquare );
}

SERIALIZE_DEFINE_GET( KPersonalShopUserInfo, obj, ks )
{
	return	GET( m_iGSUID ) &&
			GET( m_iUserUID ) &&
			GET( m_iUnitUID ) &&
			GET( m_wstrNickName ) &&
			GET( m_bIsSquare );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KERM_SQUARE_LIST_INFO_NOT, obj, ks )
{
	return  PUT( m_iCode ) &&
			PUT( m_kSquareInfo )
			;
}

SERIALIZE_DEFINE_GET( KERM_SQUARE_LIST_INFO_NOT, obj, ks )
{
	return  GET( m_iCode ) &&
			GET( m_kSquareInfo )
			;
}


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
#else	// SERV_UPGRADE_SKILL_SYSTEM_2013
/*
SERIALIZE_DEFINE_PUT( KDBE_INSERT_SKILL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iSkillID )
		&& PUT( m_iSkillLevel )
		&& PUT( m_iSkillCSPoint )
		&& PUT( m_iCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_SKILL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iSkillID )
		&& GET( m_iSkillLevel )
		&& GET( m_iSkillCSPoint )
		&& GET( m_iCSPoint )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_iSkillID )
		&& PUT( m_iSkillLevel )
		&& PUT( m_iSkillCSPoint )
		&& PUT( m_iCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iSkillID )
		&& GET( m_iSkillLevel )
		&& GET( m_iSkillCSPoint )
		&& GET( m_iCSPoint )
		;
}
*/
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KNMUnitInfo, obj, ks )
{
	return PUT( m_iUnitUID ) 
		&& PUT( m_iState )
		&& PUT( m_iDungeonID )
		&& PUT( m_iRoomUID )
		;
}

SERIALIZE_DEFINE_GET( KNMUnitInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iState )
		&& GET( m_iDungeonID )
		&& GET( m_iRoomUID )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_KNM_TOGETHER_BY_UNIT_INFO_REQ, obj, ks )
{
	return  PUT( m_iDemandGSUID ) &&
		PUT( m_iDemandUserUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_KNM_TOGETHER_BY_UNIT_INFO_REQ, obj, ks )
{
	return  GET( m_iDemandGSUID ) &&
		GET( m_iDemandUserUID )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KELG_KNM_TOGETHER_BY_UNIT_INFO_ACK, obj, ks )
{
	return  PUT( m_iOK ) &&
		PUT( m_iDemandGSUID ) &&
		PUT( m_iDemandUserUID ) &&
		PUT( m_kUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KELG_KNM_TOGETHER_BY_UNIT_INFO_ACK, obj, ks )
{
	return  GET( m_iOK ) &&
		GET( m_iDemandGSUID ) &&
		GET( m_iDemandUserUID ) &&
		GET( m_kUnitInfo )
		;
}

//SERIALIZE_DEFINE_PUT( KNXShopOrder, obj, ks )
//{
//    return  PUT( order_no ) &&
//            PUT( package_product_no ) &&
//            PUT( product_no ) &&
//            PUT( product_name ) &&
//            PUT( product_dbid ) &&
//            PUT( product_expireDay ) &&
//            PUT( product_pieces ) &&
//            PUT( order_quantity ) &&
//            PUT( receive_order_quantity ) &&
//            PUT( remain_order_quantity ) &&
//            PUT( is_order_package ) &&
//            PUT( sender_game_no ) &&
//            PUT( sender_game_server ) &&
//            PUT( sender_game_id ) &&
//            PUT( present_message ) &&
//            PUT( product_attribute0 ) &&
//            PUT( product_attribute1 ) &&
//            PUT( product_attribute2 ) &&
//            PUT( product_attribute3 ) &&
//            PUT( product_attribute4 ) &&
//            PUT( register_date ) &&
//            PUT( is_order_read ) &&
//            PUT( product_type );
//}
//
//SERIALIZE_DEFINE_GET( KNXShopOrder, obj, ks )
//{
//    return  GET( order_no ) &&
//            GET( package_product_no ) &&
//            GET( product_no ) &&
//            GET( product_name ) &&
//            GET( product_dbid ) &&
//            GET( product_expireDay ) &&
//            GET( product_pieces ) &&
//            GET( order_quantity ) &&
//            GET( receive_order_quantity ) &&
//            GET( remain_order_quantity ) &&
//            GET( is_order_package ) &&
//            GET( sender_game_no ) &&
//            GET( sender_game_server ) &&
//            GET( sender_game_id ) &&
//            GET( present_message ) &&
//            GET( product_attribute0 ) &&
//            GET( product_attribute1 ) &&
//            GET( product_attribute2 ) &&
//            GET( product_attribute3 ) &&
//            GET( product_attribute4 ) &&
//            GET( register_date ) &&
//            GET( is_order_read ) &&
//            GET( product_type );
//}
//
//SERIALIZE_DEFINE_PUT( KNXShopPackage, obj, ks )
//{
//    return  PUT( order_no ) &&
//            PUT( package_product_no ) &&
//            PUT( product_no ) &&
//            PUT( product_name ) &&
//            PUT( product_dbid ) &&
//            PUT( product_expireDay ) &&
//            PUT( product_attribute0 ) &&
//            PUT( product_attribute1 ) &&
//            PUT( product_attribute2 ) &&
//            PUT( product_attribute3 ) &&
//            PUT( product_attribute4 );
//}
//
//SERIALIZE_DEFINE_GET( KNXShopPackage, obj, ks )
//{
//    return  GET( order_no ) &&
//            GET( package_product_no ) &&
//            GET( product_no ) &&
//            GET( product_name ) &&
//            GET( product_dbid ) &&
//            GET( product_expireDay ) &&
//            GET( product_attribute0 ) &&
//            GET( product_attribute1 ) &&
//            GET( product_attribute2 ) &&
//            GET( product_attribute3 ) &&
//            GET( product_attribute4 );
//}
//
//SERIALIZE_DEFINE_PUT( KNXShopLottery, obj, ks )
//{
//    return  PUT( order_no ) &&
//            PUT( lottery_product_no ) &&
//            PUT( product_no ) &&
//            PUT( product_name ) &&
//            PUT( product_dbid ) &&
//            PUT( product_expireDay ) &&
//            PUT( product_attribute0 ) &&
//            PUT( product_attribute1 ) &&
//            PUT( product_attribute2 ) &&
//            PUT( product_attribute3 ) &&
//            PUT( product_attribute4 );
//}
//
//SERIALIZE_DEFINE_GET( KNXShopLottery, obj, ks )
//{
//    return  GET( order_no ) &&
//            GET( lottery_product_no ) &&
//            GET( product_no ) &&
//            GET( product_name ) &&
//            GET( product_dbid ) &&
//            GET( product_expireDay ) &&
//            GET( product_attribute0 ) &&
//            GET( product_attribute1 ) &&
//            GET( product_attribute2 ) &&
//            GET( product_attribute3 ) &&
//            GET( product_attribute4 );
//}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEXPData, obj, ks )
{
	return PUT( m_iEXP )
		&& PUT( m_iPartyEXP )
		&& PUT( m_iSocketOptEXP )
		//{{ 2012. 12. 18	최육사	필드에서도 경험치 이벤트 적용
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		&& PUT( m_iEventBonusEXP )
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEXPData, obj, ks )
{
	return GET( m_iEXP )
		&& GET( m_iPartyEXP )
		&& GET( m_iSocketOptEXP )
		//{{ 2012. 12. 18	최육사	필드에서도 경험치 이벤트 적용
#ifdef SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		&& GET( m_iEventBonusEXP )
#endif SERV_EVENT_EXP_BONUS_IN_BATTLE_FIELD
		//}}
        ;
}

SERIALIZE_DEFINE_PUT( KNXBTCategoryInfo, obj, ks )
{
    return  PUT( m_ulCategoryNo ) &&
            PUT( m_wstrCategoryName ) &&
            PUT( m_ulParentCategoryNo ) &&
            PUT( m_ulDisplayNo );
}

SERIALIZE_DEFINE_GET( KNXBTCategoryInfo, obj, ks )
{
    return  GET( m_ulCategoryNo ) &&
            GET( m_wstrCategoryName ) &&
            GET( m_ulParentCategoryNo ) &&
            GET( m_ulDisplayNo );
}

SERIALIZE_DEFINE_PUT( KNXBTBonusProductInfo, obj, ks )
{
    return  PUT( m_ulProductNo ) &&
            PUT( m_wstrExtend );
}

SERIALIZE_DEFINE_GET( KNXBTBonusProductInfo, obj, ks )
{
    return  GET( m_ulProductNo ) &&
            GET( m_wstrExtend );
}

//{{ 2008. 7. 10  최육사  빌링패킷 수정
SERIALIZE_DEFINE_PUT( KNXBTBonusProductInfoPickUp, obj, ks )
{
	return PUT( m_ulBonusProduct )
		&& PUT( m_ulBonusAmount )
		;
}

SERIALIZE_DEFINE_GET( KNXBTBonusProductInfoPickUp, obj, ks )
{
	return GET( m_ulBonusProduct )
		&& GET( m_ulBonusAmount )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KNXBTProductInfo, obj, ks )
{
    return PUT( m_ulProductNo )
		&& PUT( m_ulRelationProductNo )
		//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경		
		&& PUT( m_sProductExpire )
		//}}
		&& PUT( m_usProductPieces )
		&& PUT( m_wstrProductID )
		&& PUT( m_wstrProductGUID )
		&& PUT( m_ulPaymentType )
		&& PUT( m_wstrProductType )
		&& PUT( m_ulSalePrice )
		&& PUT( m_ulCategoryNo )
        && PUT( m_vecBonusProductInfo )
		;
}

SERIALIZE_DEFINE_GET( KNXBTProductInfo, obj, ks )
{
    return GET( m_ulProductNo )
		&& GET( m_ulRelationProductNo )
		//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경		
		&& GET( m_sProductExpire )
		//}}
		&& GET( m_usProductPieces )
		&& GET( m_wstrProductID )
		&& GET( m_wstrProductGUID )
		&& GET( m_ulPaymentType )
		&& GET( m_wstrProductType )
		&& GET( m_ulSalePrice )
		&& GET( m_ulCategoryNo )
        && GET( m_vecBonusProductInfo )
		;
}

SERIALIZE_DEFINE_PUT( KNXBTPurchaseReqInfo, obj, ks )
{
	return PUT( m_ulProductNo )
		&& PUT( m_usOrderQuantity )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
		&& PUT( m_iCouponCardNo )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KNXBTPurchaseReqInfo, obj, ks )
{
	return GET( m_ulProductNo )
		&& GET( m_usOrderQuantity )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
		&& GET( m_iCouponCardNo )
#endif // SERV_NEXON_COUPON_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KNXBTPurchaseAckInfo, obj, ks )
{
    return PUT( m_ulProductNo )
		&& PUT( m_usOrderQuantity )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
		&& PUT( m_ulCouponUseApproval )
#endif // SERV_NEXON_COUPON_SYSTEM
		&& PUT( m_wstrExtendValue )
		;
}

SERIALIZE_DEFINE_GET( KNXBTPurchaseAckInfo, obj, ks )
{
	return GET( m_ulProductNo )
		&& GET( m_usOrderQuantity )
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-23	// 박세훈
		&& GET( m_ulCouponUseApproval )
#endif // SERV_NEXON_COUPON_SYSTEM
		&& GET( m_wstrExtendValue )
		;
}

SERIALIZE_DEFINE_PUT( KNXBTOrderInfo, obj, ks )
{
    return  PUT( m_ulOrderNo ) &&
            PUT( m_ulProductNo ) &&
            PUT( m_byteProductKind ) &&
            PUT( m_wstrProductName ) &&
            PUT( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            PUT( m_sProductExpire ) &&
			//}}
            PUT( m_usProductPieces ) &&
            PUT( m_usOrderQuantity ) &&
            PUT( m_usRemainOrderQuantity ) &&
            PUT( m_byteIsPresent ) &&
            PUT( m_byteIsRead ) &&
            PUT( m_byteSenderServerNo ) &&
            PUT( m_wstrSenderGameID ) &&
            PUT( m_wstrSenderPresentMessage ) &&
            PUT( m_wstrProductAttribute0 ) &&
            PUT( m_wstrProductAttribute1 ) &&
            PUT( m_wstrProductAttribute2 ) &&
            PUT( m_wstrProductAttribute3 ) &&
            PUT( m_wstrProductAttribute4 ) &&
            PUT( m_wstrExtendValue );
}

SERIALIZE_DEFINE_GET( KNXBTOrderInfo, obj, ks )
{
    return  GET( m_ulOrderNo ) &&
            GET( m_ulProductNo ) &&
            GET( m_byteProductKind ) &&
            GET( m_wstrProductName ) &&
            GET( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            GET( m_sProductExpire ) &&
			//}}
            GET( m_usProductPieces ) &&
            GET( m_usOrderQuantity ) &&
            GET( m_usRemainOrderQuantity ) &&
            GET( m_byteIsPresent ) &&
            GET( m_byteIsRead ) &&
            GET( m_byteSenderServerNo ) &&
            GET( m_wstrSenderGameID ) &&
            GET( m_wstrSenderPresentMessage ) &&
            GET( m_wstrProductAttribute0 ) &&
            GET( m_wstrProductAttribute1 ) &&
            GET( m_wstrProductAttribute2 ) &&
            GET( m_wstrProductAttribute3 ) &&
            GET( m_wstrProductAttribute4 ) &&
            GET( m_wstrExtendValue );
}

SERIALIZE_DEFINE_PUT( KNXBTPackageInfo, obj, ks )
{
    return  PUT( m_ulOrderNo ) &&
            PUT( m_ulPackageProductNo ) &&
            PUT( m_ulProductNo ) &&
            PUT( m_wstrProductName ) &&
            PUT( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            PUT( m_sProductExpire ) &&
			//}}
            PUT( m_usProductPieces ) &&
            PUT( m_wstrProductAttribute0 ) &&
            PUT( m_wstrProductAttribute1 ) &&
            PUT( m_wstrProductAttribute2 ) &&
            PUT( m_wstrProductAttribute3 ) &&
            PUT( m_wstrProductAttribute4 );
}

SERIALIZE_DEFINE_GET( KNXBTPackageInfo, obj, ks )
{
    return  GET( m_ulOrderNo ) &&
            GET( m_ulPackageProductNo ) &&
            GET( m_ulProductNo ) &&
            GET( m_wstrProductName ) &&
            GET( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            GET( m_sProductExpire ) &&
			//}}
            GET( m_usProductPieces ) &&
            GET( m_wstrProductAttribute0 ) &&
            GET( m_wstrProductAttribute1 ) &&
            GET( m_wstrProductAttribute2 ) &&
            GET( m_wstrProductAttribute3 ) &&
            GET( m_wstrProductAttribute4 );
}

//{{ 2008. 7. 28  최육사	패키지 
SERIALIZE_DEFINE_PUT( KNXBTPickUpPackageInfo, obj, ks )
{
	return PUT( m_ulProductNo )
		&& PUT( m_wstrProductID )
		//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경		
		&& PUT( m_sProductExpire )
		//}}
		&& PUT( m_usProductPieces )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& PUT( m_iSubProductNo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KNXBTPickUpPackageInfo, obj, ks )
{
	return GET( m_ulProductNo )
		&& GET( m_wstrProductID )
		//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경		
		&& GET( m_sProductExpire )
		//}}
		&& GET( m_usProductPieces )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& GET( m_iSubProductNo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}
//}}

SERIALIZE_DEFINE_PUT( KNXBTLotteryInfo, obj, ks )
{
    return  PUT( m_ulOrderNo ) &&
            PUT( m_ulLotteryProductNo ) &&
            PUT( m_ulProductNo ) &&
            PUT( m_wstrProductName ) &&
            PUT( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            PUT( m_sProductExpire ) &&
			//}}
            PUT( m_usProductPieces ) &&
            PUT( m_wstrProductAttribute0 ) &&
            PUT( m_wstrProductAttribute1 ) &&
            PUT( m_wstrProductAttribute2 ) &&
            PUT( m_wstrProductAttribute3 ) &&
            PUT( m_wstrProductAttribute4 );
}

SERIALIZE_DEFINE_GET( KNXBTLotteryInfo, obj, ks )
{
    return  GET( m_ulOrderNo ) &&
            GET( m_ulLotteryProductNo ) &&
            GET( m_ulProductNo ) &&
            GET( m_wstrProductName ) &&
            GET( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            GET( m_sProductExpire ) &&
			//}}
            GET( m_usProductPieces ) &&
            GET( m_wstrProductAttribute0 ) &&
            GET( m_wstrProductAttribute1 ) &&
            GET( m_wstrProductAttribute2 ) &&
            GET( m_wstrProductAttribute3 ) &&
            GET( m_wstrProductAttribute4 );
}

SERIALIZE_DEFINE_PUT( KNXBTCouponProductInfo, obj, ks )
{
    return  PUT( m_ulProductNo ) &&
            PUT( m_wstrExtendValue );
}

SERIALIZE_DEFINE_GET( KNXBTCouponProductInfo, obj, ks )
{
    return  GET( m_ulProductNo ) &&
            GET( m_wstrExtendValue );
}

SERIALIZE_DEFINE_PUT( KNXBTSubProductInfo, obj, ks )
{
    return  PUT( m_ulProductNo ) &&
            PUT( m_wstrProductName ) &&
            PUT( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            PUT( m_sProductExpire ) &&
			//}}
            PUT( m_usProductPieces ) &&
            PUT( m_wstrProductAttribute0 ) &&
            PUT( m_wstrProductAttribute1 ) &&
            PUT( m_wstrProductAttribute2 ) &&
            PUT( m_wstrProductAttribute3 ) &&
            PUT( m_wstrProductAttribute4 );
}

SERIALIZE_DEFINE_GET( KNXBTSubProductInfo, obj, ks )
{
    return  GET( m_ulProductNo ) &&
            GET( m_wstrProductName ) &&
            GET( m_wstrProductID ) &&
			//{{ 2009. 12. 2  최육사	캐쉬템기간타입변경			
            GET( m_sProductExpire ) &&
			//}}
            GET( m_usProductPieces ) &&
            GET( m_wstrProductAttribute0 ) &&
            GET( m_wstrProductAttribute1 ) &&
            GET( m_wstrProductAttribute2 ) &&
            GET( m_wstrProductAttribute3 ) &&
            GET( m_wstrProductAttribute4 );
}

#ifndef SERV_GLOBAL_BILLING
/*
SERIALIZE_DEFINE_PUT( KBillProductCategory, obj, ks )
{
    return  PUT( m_cCategoryNo ) &&
            PUT( m_wstrCategoryName ) &&
            PUT( m_cParentCategoryNo ) &&
            PUT( m_cDisplayNo );
}

SERIALIZE_DEFINE_GET( KBillProductCategory, obj, ks )
{
    return  GET( m_cCategoryNo ) &&
            GET( m_wstrCategoryName ) &&
            GET( m_cParentCategoryNo ) &&
            GET( m_cDisplayNo );
}

SERIALIZE_DEFINE_PUT( KBillProductInfo, obj, ks )
{
    return  PUT( m_iProductNo ) &&
            PUT( m_iProductID ) &&
            PUT( m_bSale ) &&
            PUT( m_bEvent ) &&
#ifdef SERV_GLOBAL_BILLING
			PUT( m_bShow ) &&
			PUT( m_bNotForSale ) &&
			PUT( m_bIsCashInvenSkip ) &&
#endif // SERV_GLOBAL_BILLING
            PUT( m_wstrProductName ) &&
            PUT( m_cCategoryNo ) &&
            PUT( m_iSalePrice ) &&
            PUT( m_iRealPrice ) &&
            PUT( m_cPeriod ) &&
            PUT( m_cQuantity ) &&
            PUT( m_bEnableGift ) &&
            PUT( m_cLimitLevel ) &&
            PUT( m_wstrEndDate );
}

SERIALIZE_DEFINE_GET( KBillProductInfo, obj, ks )
{
    return  GET( m_iProductNo ) &&
            GET( m_iProductID ) &&
            GET( m_bSale ) &&
            GET( m_bEvent ) &&
#ifdef SERV_GLOBAL_BILLING
			GET( m_bShow ) &&
			GET( m_bNotForSale ) &&
			GET( m_bIsCashInvenSkip ) &&
#endif // SERV_GLOBAL_BILLING
            GET( m_wstrProductName ) &&
            GET( m_cCategoryNo ) &&
            GET( m_iSalePrice ) &&
            GET( m_iRealPrice ) &&
            GET( m_cPeriod ) &&
            GET( m_cQuantity ) &&
            GET( m_bEnableGift ) &&
            GET( m_cLimitLevel ) &&
            GET( m_wstrEndDate );
}

// #ifdef SERV_GLOBAL_BILLING
SERIALIZE_DEFINE_PUT( KBillProductAttribute, obj, ks )
{
	return  PUT( m_iProductNo ) &&
		PUT( m_vecAttribute );
}

SERIALIZE_DEFINE_GET( KBillProductAttribute, obj, ks )
{
	return  GET( m_iProductNo ) &&
		GET( m_vecAttribute );
}
// #endif SERV_GLOBAL_BILLING

SERIALIZE_DEFINE_PUT( KBillPackageInfo, obj, ks )
{
    return  PUT( m_iPackageNo ) &&
            PUT( m_iProductNo );
}

SERIALIZE_DEFINE_GET( KBillPackageInfo, obj, ks )
{
    return  GET( m_iPackageNo ) &&
            GET( m_iProductNo );
}

// #ifdef SERV_GLOBAL_BILLING
SERIALIZE_DEFINE_PUT( KBillBuyInfo, obj, ks )
{
	return PUT( m_wstrOrderID )
		&& PUT( m_iProductNo )
		&& PUT( m_iPoint )
		&& PUT( m_iProductID )
		&& PUT( m_usOrderQuantity )
		&& PUT( m_iCategory )
		&& PUT( m_wstrMemo )
		;
}

SERIALIZE_DEFINE_GET( KBillBuyInfo, obj, ks )
{
	return GET( m_wstrOrderID )
		&& GET( m_iProductNo )
		&& GET( m_iPoint )
		&& GET( m_iProductID )
		&& GET( m_usOrderQuantity )
		&& GET( m_iCategory )
		&& GET( m_wstrMemo )
		;
}
// #endif SERV_GLOBAL_BILLING

SERIALIZE_DEFINE_PUT( KBillOrderInfo, obj, ks )
{
    return  PUT( m_iOrderNo ) &&
            PUT( m_iUserUID ) &&
            PUT( m_iProductNo );
}

SERIALIZE_DEFINE_GET( KBillOrderInfo, obj, ks )
{
    return  GET( m_iOrderNo ) &&
            GET( m_iUserUID ) &&
            GET( m_iProductNo );
}
//*/
#endif // SERV_GLOBAL_BILLING

SERIALIZE_DEFINE_PUT( KSquarePersonalShopInfo, obj, ks )
{
	return  PUT( m_cUpdateType ) &&
			PUT( m_cPersonalShopState ) &&
			PUT( m_cPersonalShopType ) &&
			PUT( m_iPersonalShopUID ) &&
			PUT( m_wstrPersonalShopName );
}

SERIALIZE_DEFINE_GET( KSquarePersonalShopInfo, obj, ks )
{
	return  GET( m_cUpdateType ) &&
			GET( m_cPersonalShopState ) &&
			GET( m_cPersonalShopType ) &&
			GET( m_iPersonalShopUID ) &&
			GET( m_wstrPersonalShopName );
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KRegPersonalShopItemInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iQuantity )
		&& PUT( m_iPricePerOne )
		;
}

SERIALIZE_DEFINE_GET( KRegPersonalShopItemInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iQuantity )
		&& GET( m_iPricePerOne )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KPersonalShopItemInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KPersonalShopItemInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iQuantity )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KSellPersonalShopItemInfo, obj, ks )
{
	return PUT( m_kInventoryItemInfo )
		&& PUT( m_iPricePerOne )
		&& PUT( m_iFeePerOne )
		//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_cPShopItemType )
		&& PUT( m_iTotalSellEDIn )
		&& PUT( m_iTotalSoldItemQuantity )
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& PUT( m_iInventoryItemUID )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		;
}

SERIALIZE_DEFINE_GET( KSellPersonalShopItemInfo, obj, ks )
{
	return GET( m_kInventoryItemInfo )
		&& GET( m_iPricePerOne )
		&& GET( m_iFeePerOne )
		//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_cPShopItemType )
		&& GET( m_iTotalSellEDIn )
		&& GET( m_iTotalSoldItemQuantity )
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		&& GET( m_iInventoryItemUID )
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
		//}}
		;
}


//{{ 2011. 04. 22	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
SERIALIZE_DEFINE_PUT( KSellPShopItemDBUpdateInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iQuantity )
		&& PUT( m_iTotalSellEDIn )
		&& PUT( m_iTotalSoldItemQuantity )
		&& PUT( m_iTotalSellCommissionED )
		;
}

SERIALIZE_DEFINE_GET( KSellPShopItemDBUpdateInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iQuantity )
		&& GET( m_iTotalSellEDIn )
		&& GET( m_iTotalSoldItemQuantity )
		&& GET( m_iTotalSellCommissionED )
		;
}


SERIALIZE_DEFINE_PUT( KSellPShopItemBackupData, obj, ks )
{
	return PUT( m_kSellPShopItemInfo )
		&& PUT( m_iTotalSoldItemQuantity )
		&& PUT( m_iTotalSellCommissionED )
		;
}

SERIALIZE_DEFINE_GET( KSellPShopItemBackupData, obj, ks )
{
	return GET( m_kSellPShopItemInfo )
		&& GET( m_iTotalSoldItemQuantity )
		&& GET( m_iTotalSellCommissionED )
		;
}
#endif SERV_PSHOP_AGENCY
//}}


SERIALIZE_DEFINE_PUT( KLogData, obj, ks )
{
    return  PUT( m_byteLogLevel ) &&
            PUT( m_wstrRegDate ) &&
            PUT( m_wstrFunctionName ) &&
            PUT( m_vecContent ) &&
            PUT( m_wstrFileName ) &&
            PUT( m_iFileLine );
}

SERIALIZE_DEFINE_GET( KLogData, obj, ks )
{
    return  GET( m_byteLogLevel ) &&
            GET( m_wstrRegDate ) &&
            GET( m_wstrFunctionName ) &&
            GET( m_vecContent ) &&
            GET( m_wstrFileName ) &&
            GET( m_iFileLine );
}

//////////////////////////////////////////////////////////////////////////
//{{ 2008. 3. 30  최육사  우체국
SERIALIZE_DEFINE_PUT( KPostItemInfo, obj, ks )
{
	return PUT( m_iPostNo )
		&& PUT( m_iFromUnitUID )
		&& PUT( m_iToUnitUID )
		&& PUT( m_cScriptType )
		&& PUT( m_iScriptIndex )
		&& PUT( m_iQuantity )
		&& PUT( m_cEnchantLevel )
		&& PUT( m_kAttribEnchantInfo )		
		&& PUT( m_vecItemSocket )
		//{{ 2013. 05. 09	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		&& PUT( m_vecRandomSocket )
		&& PUT( m_cItemState )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		&& PUT( m_ucSealData )
		&& PUT( m_wstrRegDate )
		&& PUT( m_wstrFromNickName )
		&& PUT( m_wstrTitle )
		&& PUT( m_wstrMessage )
		&& PUT( m_bRead )

		//{{ 2012. 08. 21	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& PUT( m_iItemUID )
		&& PUT( m_iUsageType )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		&& PUT( m_bSenderDeleted )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		&& PUT( m_byteExpandedSocketNum )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KPostItemInfo, obj, ks )
{
	return GET( m_iPostNo )
		&& GET( m_iFromUnitUID )
		&& GET( m_iToUnitUID )
		&& GET( m_cScriptType )
		&& GET( m_iScriptIndex )
		&& GET( m_iQuantity )
		&& GET( m_cEnchantLevel )
		&& GET( m_kAttribEnchantInfo )
		&& GET( m_vecItemSocket )
		//{{ 2013. 05. 09	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		&& GET( m_vecRandomSocket )
		&& GET( m_cItemState )
#endif SERV_NEW_ITEM_SYSTEM_2013_05
		//}}
		&& GET( m_ucSealData )
		&& GET( m_wstrRegDate )
		&& GET( m_wstrFromNickName )
		&& GET( m_wstrTitle )
		&& GET( m_wstrMessage )
		&& GET( m_bRead )

		//{{ 2012. 08. 21	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
		&& GET( m_iItemUID )
		&& GET( m_iUsageType )
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
		//}}
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		&& GET( m_bSenderDeleted )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		&& GET( m_byteExpandedSocketNum )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_PUT( KPostItemTitleInfo, obj, ks )
{
	return PUT( m_iPostNo )
		&& PUT( m_cScriptType )
		&& PUT( m_iScriptIndex )
		&& PUT( m_cEnchantLevel )
		&& PUT( m_wstrRegDate )
		&& PUT( m_wstrFromNickName )
		&& PUT( m_wstrTitle )
		&& PUT( m_bRead )
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		&& PUT( m_bSenderDeleted )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		;
}

SERIALIZE_DEFINE_GET( KPostItemTitleInfo, obj, ks )
{
	return GET( m_iPostNo )
		&& GET( m_cScriptType )
		&& GET( m_iScriptIndex )
		&& GET( m_cEnchantLevel )
		&& GET( m_wstrRegDate )
		&& GET( m_wstrFromNickName )
		&& GET( m_wstrTitle )
		&& GET( m_bRead )
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
		&& GET( m_bSenderDeleted )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
		;
}
//}}	

SERIALIZE_DEFINE_PUT( KPostSmallTitleInfo, obj, ks )
{
	return PUT( m_cScriptType )
		&& PUT( m_wstrLetterTitle )
		;
}

SERIALIZE_DEFINE_GET( KPostSmallTitleInfo, obj, ks )
{
	return GET( m_cScriptType )
		&& GET( m_wstrLetterTitle )
		;
}

SERIALIZE_DEFINE_PUT( KTutorialUnitInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_ucLevel )
		&& PUT( m_wstrNickName )
		&& PUT( m_bIsOnline )
		;
}

SERIALIZE_DEFINE_GET( KTutorialUnitInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_ucLevel )
		&& GET( m_wstrNickName )
		&& GET( m_bIsOnline )
		;
}

SERIALIZE_DEFINE_PUT( KTutorialDBUnitInfo, obj, ks )
{
	return PUT( m_kTutorialUnitInfo )
		&& PUT( m_wstrLastDate )
		;
}

SERIALIZE_DEFINE_GET( KTutorialDBUnitInfo, obj, ks )
{
	return GET( m_kTutorialUnitInfo )
		&& GET( m_wstrLastDate )
		;
}

//{{ 2008. 5. 21  최육사  아이템 어뷰저 로그
SERIALIZE_DEFINE_PUT( KItemAbuserLogInfo, obj, ks )
{
	return PUT( m_iRewardState )
		&& PUT( m_iItemID )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KItemAbuserLogInfo, obj, ks )
{
	return GET( m_iRewardState )
		&& GET( m_iItemID )
		&& GET( m_iQuantity )
		;
}
//}}

//{{ 2008. 9. 23  최육사	캐쉬템 분해
SERIALIZE_DEFINE_PUT( KCashItemResolveInfo, obj, ks )
{
	return PUT( m_iItemID )
		&& PUT( m_iSalePrice )
		;
}

SERIALIZE_DEFINE_GET( KCashItemResolveInfo, obj, ks )
{
	return GET( m_iItemID )
		&& GET( m_iSalePrice )
		;
}
//}}

//{{ 2008. 10. 1  최육사	속성몬스터
SERIALIZE_DEFINE_PUT( KAttribEnchantNpcInfo, obj, ks )
{
	return PUT( m_cAttribEnchant1 )
		&& PUT( m_cAttribEnchant2 )
		&& PUT( m_cExtraSpecialAbility1 )
		&& PUT( m_cExtraSpecialAbility2 )
		;
}

SERIALIZE_DEFINE_GET( KAttribEnchantNpcInfo, obj, ks )
{
	return GET( m_cAttribEnchant1 )
		&& GET( m_cAttribEnchant2 )
		&& GET( m_cExtraSpecialAbility1 )
		&& GET( m_cExtraSpecialAbility2 )
		;
}
//}}

//{{ 2008. 10. 6  최육사	타이틀
SERIALIZE_DEFINE_PUT( KSubMissionInstance, obj, ks )
{
	return PUT( m_iID )
		&& PUT( m_sClearData )
		&& PUT( m_bIsSuccess )
		;
}

SERIALIZE_DEFINE_GET( KSubMissionInstance, obj, ks )
{
	return GET( m_iID )
		&& GET( m_sClearData )
		&& GET( m_bIsSuccess )
		;
}

SERIALIZE_DEFINE_PUT( KMissionInstance, obj, ks )
{
	return PUT( m_iID )
		&& PUT( m_vecSubMissionInstance )
		;
}

SERIALIZE_DEFINE_GET( KMissionInstance, obj, ks )
{
	return GET( m_iID )
		&& GET( m_vecSubMissionInstance )
		;
}

SERIALIZE_DEFINE_PUT( KMissionUpdate, obj, ks )
{
	return PUT( m_iTitleID )
		&& PUT( m_vecClearData )
		;
}

SERIALIZE_DEFINE_GET( KMissionUpdate, obj, ks )
{
	return GET( m_iTitleID )
		&& GET( m_vecClearData )
		;
}

SERIALIZE_DEFINE_PUT( KTitleInfo, obj, ks )
{
	return PUT( m_iTitleID )
		&& PUT( m_wstrEndDate )
		&& PUT( m_kCTime )
		&& PUT( m_bInfinity )
		;
}

SERIALIZE_DEFINE_GET( KTitleInfo, obj, ks )
{
	return GET( m_iTitleID )
		&& GET( m_wstrEndDate )
		&& GET( m_kCTime )
		&& GET( m_bInfinity )
		;
}

SERIALIZE_DEFINE_PUT( KTitleReward, obj, ks )
{
	return PUT( m_iTitleID )
		&& PUT( m_sPeriod )
		;
}

SERIALIZE_DEFINE_GET( KTitleReward, obj, ks )
{
	return GET( m_iTitleID )
		&& GET( m_sPeriod )
		;
}

SERIALIZE_DEFINE_PUT( KRankingTitleReward, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_vecReward )
		;
}

SERIALIZE_DEFINE_GET( KRankingTitleReward, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_vecReward )
		;
}
//}}

//{{ 2008. 10. 30  최육사	어뷰저 리스트
SERIALIZE_DEFINE_PUT( KAbuserList, obj, ks )
{
	return PUT( m_iReleaseTick )
		&& PUT( m_vecAbuserList )
		;
}

SERIALIZE_DEFINE_GET( KAbuserList, obj, ks )
{
	return GET( m_iReleaseTick )
		&& GET( m_vecAbuserList )
		;
}
//}}

SERIALIZE_DEFINE_PUT( KFieldUserInfo, obj, ks )
{
	return PUT( m_iUnitUID	)
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cRank )
#else
		&& PUT( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		&& PUT( m_cPVPEmblem )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& PUT( m_vecEquippedItem )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iTitleID )
#else
		&& PUT( m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		&& PUT( m_vPos )
		&& PUT( m_iPartyUID )
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_wstrGuildName	)
#endif GUILD_TEST
		//}}
		//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}		
		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		&& PUT( m_iED )
#endif SERV_ED_MONITORING_IN_GAME
		//}}
#ifdef SERV_INVISIBLE_GM
		&& PUT( m_bInvisible )
#endif SERV_INVISIBLE_GM
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& PUT( m_iRidingPetUID )
		&& PUT( m_usRidingPetID )
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& PUT( m_iLoverUnitUID )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_GROW_UP_SOCKET
		&& PUT( m_iEventQuestClearCount )
		&& PUT( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& PUT( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

SERIALIZE_DEFINE_GET( KFieldUserInfo, obj, ks )
{
	return GET( m_iUnitUID	)
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		//{{ 2011. 07. 11	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cRank )
#else
		&& GET( m_iRating )
#endif SERV_2012_PVP_SEASON2
		//}}
#else
		&& GET( m_cPVPEmblem )
#endif SERV_PVP_NEW_SYSTEM
		//}}
		&& GET( m_vecEquippedItem )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iTitleID )
#else
		&& GET( m_sTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		&& GET( m_vPos )
		&& GET( m_iPartyUID )
		//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_wstrGuildName	)
#endif GUILD_TEST
		//}}
		//{{ 2010. 07. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_vecPet )
#endif SERV_PET_SYSTEM
		//}}		
		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		&& GET( m_iED )
#endif SERV_ED_MONITORING_IN_GAME
		//}}
#ifdef SERV_INVISIBLE_GM
		&& GET( m_bInvisible )
#endif SERV_INVISIBLE_GM

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-21
		&& GET( m_iRidingPetUID )
		&& GET( m_usRidingPetID )
#endif	// SERV_RIDING_PET_SYSTM
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& GET( m_iLoverUnitUID )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
#ifdef SERV_GROW_UP_SOCKET
		&& GET( m_iEventQuestClearCount )
		&& GET( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& GET( m_iAccountPVPLoseCount )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

//{{ 2009. 1. 13  최육사	필드 유저 리스트
SERIALIZE_DEFINE_PUT( KCommunityUserInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_cState )
		&& PUT( m_iStateCode )
		&& PUT( m_iRoomUID )
		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		&& PUT( m_iED )
#endif SERV_ED_MONITORING_IN_GAME
		//}}
		;
}

SERIALIZE_DEFINE_GET( KCommunityUserInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_cState )
		&& GET( m_iStateCode )
		&& GET( m_iRoomUID )
		//{{ 2011. 09. 20  김민성	게임내 ED 감시 모니터링
#ifdef SERV_ED_MONITORING_IN_GAME
		&& GET( m_iED )
#endif SERV_ED_MONITORING_IN_GAME
		//}}
		;
}
//}}

SERIALIZE_DEFINE_PUT( KTileInfo, obj, ks )
{
	return PUT( m_NumX )
		&& PUT( m_NumY )
		&& PUT( m_NumZ )
		&& PUT( m_SizeX )
		&& PUT( m_SizeY )
		&& PUT( m_SizeZ )
		;
}

SERIALIZE_DEFINE_GET( KTileInfo, obj, ks )
{
	return GET( m_NumX )
		&& GET( m_NumY )
		&& GET( m_NumZ )
		&& GET( m_SizeX )
		&& GET( m_SizeY )
		&& GET( m_SizeZ )
		;
}

SERIALIZE_DEFINE_PUT( KMapInfo, obj, ks )
{
	return PUT( m_MinX )
		&& PUT( m_MinY )
		&& PUT( m_MinZ )
		&& PUT( m_MaxX )
		&& PUT( m_MaxY )
		&& PUT( m_MaxZ )
		;
}

SERIALIZE_DEFINE_GET( KMapInfo, obj, ks )
{
	return GET( m_MinX )
		&& GET( m_MinY )
		&& GET( m_MinZ )
		&& GET( m_MaxX )
		&& GET( m_MaxY )
		&& GET( m_MaxZ )
		;
}

SERIALIZE_DEFINE_PUT( KFieldCoordinates, obj, ks )
{
	return PUT( m_PosX )
		&& PUT( m_PosY )
		&& PUT( m_PosZ )
		;
}

SERIALIZE_DEFINE_GET( KFieldCoordinates, obj, ks )
{
	return GET( m_PosX )
		&& GET( m_PosY )
		&& GET( m_PosZ )
		;
}

SERIALIZE_DEFINE_PUT( KFieldFrameInfo, obj, ks )
{
	return PUT( m_StateID )
		&& PUT( m_FrameCount )
		;
}

SERIALIZE_DEFINE_GET( KFieldFrameInfo, obj, ks )
{
	return GET( m_StateID )
		&& GET( m_FrameCount )
		;
}

SERIALIZE_DEFINE_PUT( KFieldOption, obj, ks )
{
	return PUT( m_sLimitOption )		
		;
}

SERIALIZE_DEFINE_GET( KFieldOption, obj, ks )
{
	return GET( m_sLimitOption )
		;
}

//{{ 2009. 1. 16  최육사	채널리스트
SERIALIZE_DEFINE_PUT( KChannelBonusInfo, obj, ks )
{
	return PUT( m_bEnable )
		&& PUT( m_iBeginLv )
		&& PUT( m_iEndLv )
		&& PUT( m_iPerExp )
		&& PUT( m_iPerED )
		;
}

SERIALIZE_DEFINE_GET( KChannelBonusInfo, obj, ks )
{
	return GET( m_bEnable )
		&& GET( m_iBeginLv )
		&& GET( m_iEndLv )
		&& GET( m_iPerExp )
		&& GET( m_iPerED )
		;
}

SERIALIZE_DEFINE_PUT( KChannelInfo, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_iChannelID )
		&& PUT( m_wstrChannelName )
		&& PUT( m_iServerGroupID )
		&& PUT( m_wstrIP )
		&& PUT( m_usMasterPort )
		&& PUT( m_usNCUDPPort )
		&& PUT( m_iMaxUser )
		&& PUT( m_iCurrentUser )
		//{{ 2009. 4. 1  최육사		파티정보
		&& PUT( m_iCurPartyCount )
		&& PUT( m_iPlayGamePartyCount )
		//}}
		
		//{{ 2012. 05. 16	박세훈	채널 리스트 정보 통합
#ifdef SERV_INTEGRATE_THE_CHANNEL_INFO
		&& PUT( m_kBonusInfo )
#endif SERV_INTEGRATE_THE_CHANNEL_INFO
		//}}
		;
}

SERIALIZE_DEFINE_GET( KChannelInfo, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_iChannelID )
		&& GET( m_wstrChannelName )
		&& GET( m_iServerGroupID )
		&& GET( m_wstrIP )
		&& GET( m_usMasterPort )
		&& GET( m_usNCUDPPort )
		&& GET( m_iMaxUser )
		&& GET( m_iCurrentUser )
		//{{ 2009. 4. 1  최육사		파티정보
		&& GET( m_iCurPartyCount )
		&& GET( m_iPlayGamePartyCount )
		//}}

		//{{ 2012. 05. 16	박세훈	채널 리스트 정보 통합
#ifdef SERV_INTEGRATE_THE_CHANNEL_INFO
		&& GET( m_kBonusInfo )
#endif SERV_INTEGRATE_THE_CHANNEL_INFO
		//}}
		;
}

//{{ 2009. 6. 15  최육사	채널리스트
SERIALIZE_DEFINE_PUT( KChannelSimpleInfo, obj, ks )
{
	return PUT( m_iChannelID )
		&& PUT( m_iMaxUser )
		&& PUT( m_iCurrentUser )
		;
}

SERIALIZE_DEFINE_GET( KChannelSimpleInfo, obj, ks )
{
	return GET( m_iChannelID )
		&& GET( m_iMaxUser )
		&& GET( m_iCurrentUser )
		;
}
//}}
//}}

//{{ 2009. 6. 10  최육사	채널이동정보
SERIALIZE_DEFINE_PUT( KChannelChangeInfo, obj, ks )
{
	return PUT( m_mapEventDurationTime )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& PUT( m_iEquippedTitleID )
#else
		&& PUT( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		&& PUT( m_bIsPcBang )
#ifdef SERV_PC_BANG_TYPE
		&& PUT( m_iPcBangType )
#endif SERV_PC_BANG_TYPE
		&& PUT( m_vecPcBangItemInfo )
		//{{ 2012. 06. 14	박세훈	매일매일 선물 상자
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
		//{{ 2012. 1. 2		Merge 박세훈	( 2013.1.2 지헌 : 미니맵 채널 이동 시 누락됬었던 정보 추가 )
#ifdef SERV_CHANNEL_CHANGE_BUG_FIX
		&& PUT( m_nUnitSlot )
#endif SERV_CHANNEL_CHANGE_BUG_FIX
		//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-30
		&& PUT( m_iSummonedRidingPetUID )
#endif	// SERV_RIDING_PET_SYSTM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		&& PUT( m_ButtonStartUI )
		&& PUT( m_DungeonClearUI )
		&& PUT( m_FieldCountUI )
		&& PUT( m_DungeonCount )
		&& PUT( m_FieldMonsterKillCount )
		&& PUT( m_wstrButtonClickTime )
		&& PUT( m_RemaindTime )
		&& PUT( m_bCoboItemGive )
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& PUT( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}

SERIALIZE_DEFINE_GET( KChannelChangeInfo, obj, ks )
{
	return GET( m_mapEventDurationTime )
		//{{ 2010. 11. 17	최육사	칭호 데이터 크기 늘이기
#ifdef SERV_TITLE_DATA_SIZE
		&& GET( m_iEquippedTitleID )
#else
		&& GET( m_sEquippedTitleID )
#endif SERV_TITLE_DATA_SIZE
		//}}
		&& GET( m_bIsPcBang )
#ifdef SERV_PC_BANG_TYPE
		&& GET( m_iPcBangType )
#endif SERV_PC_BANG_TYPE
		&& GET( m_vecPcBangItemInfo )
		//{{ 2012. 06. 14	박세훈	매일매일 선물 상자
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
		//{{ 2012. 1. 2		Merge 박세훈	( 2013.1.2 지헌 : 미니맵 채널 이동 시 누락됬었던 정보 추가 )
#ifdef SERV_CHANNEL_CHANGE_BUG_FIX
		&& GET( m_nUnitSlot )
#endif SERV_CHANNEL_CHANGE_BUG_FIX
		//}}
#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-30
		&& GET( m_iSummonedRidingPetUID )
#endif	// SERV_RIDING_PET_SYSTM
#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
		&& GET( m_ButtonStartUI )
		&& GET( m_DungeonClearUI )
		&& GET( m_FieldCountUI )
		&& GET( m_DungeonCount )
		&& GET( m_FieldMonsterKillCount )
		&& GET( m_wstrButtonClickTime )
		&& GET( m_RemaindTime )
		&& GET( m_bCoboItemGive )
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& GET( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}
//}}

//{{ 2009. 4. 23  최육사	대전유저리스트
SERIALIZE_DEFINE_PUT( KPVPUserSimpleInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cState )
		&& PUT( m_iStateCode )
		&& PUT( m_iRoomUID )
		;
}

SERIALIZE_DEFINE_GET( KPVPUserSimpleInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cState )
		&& GET( m_iStateCode )
		&& GET( m_iRoomUID )
		;
}



//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KUserSkillData, obj, ks )
{
	return  PUT( m_iSkillID )
		&&	PUT( m_cSkillLevel )
		&&	PUT( m_cSkillCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KUserSkillData, obj, ks )
{
	return  GET( m_iSkillID ) 
		&&	GET( m_cSkillLevel ) 
		&&	GET( m_cSkillCSPoint ) 
		;
}

#ifdef SERV_SKILL_PAGE_SYSTEM

SERIALIZE_DEFINE_PUT( KRetrievedSkillPageData, obj, ks )
{
	return  PUT( m_iRetrievedSPoint )
		&&	PUT( m_vecUserSkillData )
		;
}

SERIALIZE_DEFINE_GET( KRetrievedSkillPageData, obj, ks )
{
	return  GET( m_iRetrievedSPoint )
		&&	GET( m_vecUserSkillData )
		;
}

SERIALIZE_DEFINE_PUT( KUserSkillPageData, obj, ks )
{
	return PUT( m_aEquippedSkill[0] )
		&& PUT( m_aEquippedSkill[1] )
		&& PUT( m_aEquippedSkill[2] )
		&& PUT( m_aEquippedSkill[3] )
		&& PUT( m_aEquippedSkillSlotB[0] )
		&& PUT( m_aEquippedSkillSlotB[1] )
		&& PUT( m_aEquippedSkillSlotB[2] )
		&& PUT( m_aEquippedSkillSlotB[3] )
		&& PUT( m_usSkillPoint )
		&& PUT( m_usCashSkillPoint )
		&& PUT( m_vecUserSkillData )
		;
}

SERIALIZE_DEFINE_GET( KUserSkillPageData, obj, ks )
{
	return  GET( m_aEquippedSkill[0] )
		&& GET( m_aEquippedSkill[1] )
		&& GET( m_aEquippedSkill[2] )
		&& GET( m_aEquippedSkill[3] )
		&& GET( m_aEquippedSkillSlotB[0] )
		&& GET( m_aEquippedSkillSlotB[1] )
		&& GET( m_aEquippedSkillSlotB[2] )
		&& GET( m_aEquippedSkillSlotB[3] )
		&& GET( m_usSkillPoint )
		&& GET( m_usCashSkillPoint )
		&& GET( m_vecUserSkillData ) 
		;
}

#endif // SERV_SKILL_PAGE_SYSTEM

/////////////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDungeonRewardED, obj, ks )
{
	return PUT( m_iED )
		&& PUT( m_iBonusED )
		;
}

SERIALIZE_DEFINE_GET( KDungeonRewardED, obj, ks )
{
	return GET( m_iED )
		&& GET( m_iBonusED )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_ITEM_NOT, obj, ks )
{
	return PUT( m_GetUnitUID )
		&& PUT( m_iItemID )
		&& PUT( m_iDropItemUID )
		&& PUT( m_cGetItemType )
#ifdef DUNGEON_ITEM
		&& PUT( m_bIsItemSuccess )
#endif DUNGEON_ITEM
		&& PUT( m_iTotalED )
		&& PUT( m_mapGetED )
		//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		&& PUT( m_cEnchantLevel )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_ITEM_NOT, obj, ks )
{
	return GET( m_GetUnitUID )
		&& GET( m_iItemID )
		&& GET( m_iDropItemUID )
		&& GET( m_cGetItemType )
#ifdef DUNGEON_ITEM
		&& GET( m_bIsItemSuccess )
#endif DUNGEON_ITEM
		&& GET( m_iTotalED )
		&& GET( m_mapGetED )
		//{{ 2013. 1. 14	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		&& GET( m_cEnchantLevel )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 14  최육사	내구도 소모 개편
SERIALIZE_DEFINE_PUT( KDecreaseEnduranceInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_iEndurance )
		;
}

SERIALIZE_DEFINE_GET( KDecreaseEnduranceInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_iEndurance )
		;
}
//}}

//{{ 2009. 5. 23  최육사	시간드롭
SERIALIZE_DEFINE_PUT( KTimeDropInfo, obj, ks )
{
	return PUT( m_iTimeDropID )
		&& PUT( m_iDropItemID )
		&& PUT( m_wstrDropTime )
		&& PUT( m_wstrServerIP )
		&& PUT( m_bNoticeMsg )
		;
}

SERIALIZE_DEFINE_GET( KTimeDropInfo, obj, ks )
{
	return GET( m_iTimeDropID )
		&& GET( m_iDropItemID )
		&& GET( m_wstrDropTime )
		&& GET( m_wstrServerIP )
		&& GET( m_bNoticeMsg )
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2011. 04. 18	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
SERIALIZE_DEFINE_PUT( KUserPShopAgencyInfo, obj, ks )
{
	return PUT( m_bIsPShopOpen )
		&& PUT( m_wstrAgencyExpirationDate )
        && PUT( m_cShopType )
        && PUT( m_vecSellItemInfo )
        && PUT( m_iUserUID )
        && PUT( m_iUnitUID )
        && PUT( m_wstrNickName )
        && PUT( m_wstrAgencyOpenDate )
        && PUT( m_wstrPersonalShopName )
        && PUT( m_bOnSale )
		;
}

SERIALIZE_DEFINE_GET( KUserPShopAgencyInfo, obj, ks )
{
	return GET( m_bIsPShopOpen )
		&& GET( m_wstrAgencyExpirationDate )
        && GET( m_cShopType )
        && GET( m_vecSellItemInfo )
        && GET( m_iUserUID )
        && GET( m_iUnitUID )
        && GET( m_wstrNickName )
        && GET( m_wstrAgencyOpenDate )
        && GET( m_wstrPersonalShopName )
        && GET( m_bOnSale )
		;
}


SERIALIZE_DEFINE_PUT( KPersonalShopAgencyInfo, obj, ks )
{
	return PUT( m_iHostUnitUID )
		&& PUT( m_iPShopAgencyUID )
		&& PUT( m_wstrAgencyOpenDate )
		&& PUT( m_wstrAgencyExpirationDate )
		&& PUT( m_bOnSale )
		&& PUT( m_vecSellPShopAgencyItem )
		;
}

SERIALIZE_DEFINE_GET( KPersonalShopAgencyInfo, obj, ks )
{
	return GET( m_iHostUnitUID )
		&& GET( m_iPShopAgencyUID )
		&& GET( m_wstrAgencyOpenDate )
		&& GET( m_wstrAgencyExpirationDate )
		&& GET( m_bOnSale )
		&& GET( m_vecSellPShopAgencyItem )
		;
}

#endif SERV_PSHOP_AGENCY
//}}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGS_CONNECT_REQ, obj, ks )
{
	return PUT( m_wstrVersion )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CONNECT_REQ, obj, ks )
{
	return GET( m_wstrVersion )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CONNECT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_UDPPort )		
		&& PUT( m_iChannelID )
		//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
		&& PUT( m_bUDPKickOff )
#endif UDP_CAN_NOT_SEND_USER_KICK
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEGS_CONNECT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_UDPPort )		
		&& GET( m_iChannelID )
		//{{ 2012. 09. 02	박세훈	Merge ( 대전에서 클라이언트 조작등으로 UDP 패킷이 날아오지 않는 유저가 발견되면 서버에서 킥한다. // 2012.06.11 lygan_조성욱 )
#ifdef UDP_CAN_NOT_SEND_USER_KICK
		&& GET( m_bUDPKickOff )
#endif UDP_CAN_NOT_SEND_USER_KICK
		//}}
		;
}


////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGS_VERIFY_ACCOUNT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kAccountInfo )
		&& PUT( m_cHackingUserType )		
		&& PUT( m_wstrAdURL )
		&& PUT( m_bSex )
		&& PUT( m_uiAge )
		//{{ 2010. 12. 1	최육사	NexonSN추가
		&& PUT( m_uiNexonSN )
		//}}
		//{{ 2010. 01. 11	김민성	ip추가
#ifdef SERV_CLIENT_PUBLIC_IP
		&& PUT( m_wstrClientIP )
#endif SERV_CLIENT_PUBLIC_IP
		//}}
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		&& PUT( m_uChannelCode )
		&& PUT( m_wstrChannelUserID )
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		&& PUT( m_iEventMoney )
#endif // SERV_EVENT_MONEY
		;
}

SERIALIZE_DEFINE_GET( KEGS_VERIFY_ACCOUNT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kAccountInfo )
		&& GET( m_cHackingUserType )
		&& GET( m_wstrAdURL )
		&& GET( m_bSex )
		&& GET( m_uiAge )
		//{{ 2010. 12. 1	최육사	NexonSN추가
		&& GET( m_uiNexonSN )
		//}}
		//{{ 2010. 01. 11	김민성	ip추가
#ifdef SERV_CLIENT_PUBLIC_IP
		&& GET( m_wstrClientIP )
#endif SERV_CLIENT_PUBLIC_IP
		//}}
		//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		&& GET( m_uChannelCode )
		&& GET( m_wstrChannelUserID )
#endif SERV_TOONILAND_CHANNELING // SERV_JAPAN_CHANNELING
		//}}
#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		&& GET( m_iEventMoney )
#endif // SERV_EVENT_MONEY
		;
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 5. 30  최육사	임시 인벤
SERIALIZE_DEFINE_PUT( KTempInventoryItemInfo, obj, ks )
{
	return PUT( m_iTempItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_iQuantity )
		&& PUT( m_ucSealData )
		//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		&& PUT( m_cEnchantLevel )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KTempInventoryItemInfo, obj, ks )
{
	return GET( m_iTempItemUID )
		&& GET( m_iItemID )
		&& GET( m_iQuantity )
		&& GET( m_ucSealData )
		//{{ 2012. 1. 16	박세훈	던전 및 필드 드랍 아이템에 강화 부여
#ifdef SERV_ENCHANT_ITEM_DROP_EVENT
		&& GET( m_cEnchantLevel )
#endif SERV_ENCHANT_ITEM_DROP_EVENT
		//}}
		;
}
//}}

SERIALIZE_DEFINE_PUT( KEGS_REQUEST_FRIEND_REQ, obj, ks )
{
    return  PUT( m_wstrNickName ) &&
            PUT( m_wstrMessage );
}

SERIALIZE_DEFINE_GET( KEGS_REQUEST_FRIEND_REQ, obj, ks )
{
    return  GET( m_wstrNickName ) &&
            GET( m_wstrMessage );
}

SERIALIZE_DEFINE_PUT( KEGS_REQUEST_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iUnitUID ) &&
            PUT( m_wstrNickName );
}

SERIALIZE_DEFINE_GET( KEGS_REQUEST_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iUnitUID ) &&
            GET( m_wstrNickName );
}

SERIALIZE_DEFINE_PUT( KEGS_REQUEST_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_wstrNickName ) &&
            PUT( m_wstrMessage );
}

SERIALIZE_DEFINE_GET( KEGS_REQUEST_FRIEND_NOT, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_wstrNickName ) &&
            GET( m_wstrMessage );
}

SERIALIZE_DEFINE_PUT( KEGS_ACCEPT_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_ACCEPT_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_ACCEPT_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_ACCEPT_FRIEND_NOT, obj, ks )
{
    return  GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_DENY_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_DENY_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_DENY_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
            PUT( m_wstrNickName );
}

SERIALIZE_DEFINE_GET( KEGS_DENY_FRIEND_NOT, obj, ks )
{
    return  GET( m_iUnitUID ) &&
            GET( m_wstrNickName );
}

SERIALIZE_DEFINE_PUT( KEGS_BLOCK_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_BLOCK_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_BLOCK_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_BLOCK_FRIEND_NOT, obj, ks )
{
    return  GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_UNBLOCK_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_UNBLOCK_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_UNBLOCK_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_UNBLOCK_FRIEND_NOT, obj, ks )
{
    return  GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_DELETE_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
            PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_DELETE_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
            GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_DELETE_FRIEND_NOT, obj, ks )
{
    return  PUT( m_iUnitUID );
}

SERIALIZE_DEFINE_GET( KEGS_DELETE_FRIEND_NOT, obj, ks )
{
    return  GET( m_iUnitUID );
}

SERIALIZE_DEFINE_PUT( KEGS_MOVE_FRIEND_REQ, obj, ks )
{
    return  PUT( m_iUnitUID ) &&
        PUT( m_cTargetGroupID );
}

SERIALIZE_DEFINE_GET( KEGS_MOVE_FRIEND_REQ, obj, ks )
{
    return  GET( m_iUnitUID ) &&
        GET( m_cTargetGroupID );
}

SERIALIZE_DEFINE_PUT( KEGS_MOVE_FRIEND_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
        PUT( m_iUnitUID ) &&
        PUT( m_cTargetGroupID );
}

SERIALIZE_DEFINE_GET( KEGS_MOVE_FRIEND_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
        GET( m_iUnitUID ) &&
        GET( m_cTargetGroupID );
}

SERIALIZE_DEFINE_PUT( KEGS_MAKE_FRIEND_GROUP_REQ, obj, ks )
{
    return  PUT( m_wstrGroupName );
}

SERIALIZE_DEFINE_GET( KEGS_MAKE_FRIEND_GROUP_REQ, obj, ks )
{
    return  GET( m_wstrGroupName );
}

SERIALIZE_DEFINE_PUT( KEGS_MAKE_FRIEND_GROUP_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
        PUT( m_cGroupID ) &&
        PUT( m_wstrGroupName );
}

SERIALIZE_DEFINE_GET( KEGS_MAKE_FRIEND_GROUP_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
        GET( m_cGroupID ) &&
        GET( m_wstrGroupName );
}

SERIALIZE_DEFINE_PUT( KEGS_RENAME_FRIEND_GROUP_REQ, obj, ks )
{
    return  PUT( m_cGroupID ) &&
        PUT( m_wstrGroupName );
}

SERIALIZE_DEFINE_GET( KEGS_RENAME_FRIEND_GROUP_REQ, obj, ks )
{
    return  GET( m_cGroupID ) &&
        GET( m_wstrGroupName );
}

SERIALIZE_DEFINE_PUT( KEGS_RENAME_FRIEND_GROUP_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
        PUT( m_cGroupID ) &&
        PUT( m_wstrGroupName );
}

SERIALIZE_DEFINE_GET( KEGS_RENAME_FRIEND_GROUP_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
        GET( m_cGroupID ) &&
        GET( m_wstrGroupName );
}

SERIALIZE_DEFINE_PUT( KEGS_DELETE_FRIEND_GROUP_REQ, obj, ks )
{
    return  PUT( m_cGroupID );
}

SERIALIZE_DEFINE_GET( KEGS_DELETE_FRIEND_GROUP_REQ, obj, ks )
{
    return  GET( m_cGroupID );
}

SERIALIZE_DEFINE_PUT( KEGS_DELETE_FRIEND_GROUP_ACK, obj, ks )
{
    return  PUT( m_iOK ) &&
        PUT( m_cGroupID );
}

SERIALIZE_DEFINE_GET( KEGS_DELETE_FRIEND_GROUP_ACK, obj, ks )
{
    return  GET( m_iOK ) &&
        GET( m_cGroupID );
}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 9. 16  최육사	길드
#ifdef GUILD_TEST

SERIALIZE_DEFINE_PUT( KGuildInfo, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_wstrGuildName )
		&& PUT( m_usMaxNumMember )
		&& PUT( m_ucGuildLevel )
		&& PUT( m_iGuildEXP )
		&& PUT( m_wstrGuildMessage )
		&& PUT( m_wstrFoundingDay )
		;
}

SERIALIZE_DEFINE_GET( KGuildInfo, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_wstrGuildName )
		&& GET( m_usMaxNumMember )
		&& GET( m_ucGuildLevel )
		&& GET( m_iGuildEXP )
		&& GET( m_wstrGuildMessage )
		&& GET( m_wstrFoundingDay )
		;
}

SERIALIZE_DEFINE_PUT( KGuildMemberInfo, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_ucMemberShipGrade )
		&& PUT( m_iChannelID )
		&& PUT( m_iMapID )
		&& PUT( m_cPosition )
		&& PUT( m_tLogInTime )
		&& PUT( m_tLogOutTime )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KGuildMemberInfo, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_ucMemberShipGrade )
		&& GET( m_iChannelID )
		&& GET( m_iMapID )
		&& GET( m_cPosition )
		&& GET( m_tLogInTime )
		&& GET( m_tLogOutTime )
		&& GET( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_PUT( KGuildMemberMessageInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_ucMemberShipGrade )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KGuildMemberMessageInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_ucMemberShipGrade )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_wstrMessage )
		;
}

#endif GUILD_TEST
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

SERIALIZE_DEFINE_PUT( KGuildSkillData, obj, ks )
{
	return PUT( m_iSkillID )
		&& PUT( m_cSkillLevel )
		&& PUT( m_cSkillCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KGuildSkillData, obj, ks )
{
	return GET( m_iSkillID )
		&& GET( m_cSkillLevel )
		&& GET( m_cSkillCSPoint )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KGuildSkillInfo, obj, ks )
{
	return PUT( m_vecGuildSkill )
		&& PUT( m_iGuildSPoint )
		&& PUT( m_iGuildCSPoint )
		&& PUT( m_iMaxGuildCSPoint )
		&& PUT( m_wstrGuildCSPointEndDate )
		;
}

SERIALIZE_DEFINE_GET( KGuildSkillInfo, obj, ks )
{
	return GET( m_vecGuildSkill )
		&& GET( m_iGuildSPoint )
		&& GET( m_iGuildCSPoint )
		&& GET( m_iMaxGuildCSPoint )
		&& GET( m_wstrGuildCSPointEndDate )
		;
}

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGS_GUILD_INFO_NOT, obj, ks )
{
	return PUT( m_kGuildInfo )
		&& PUT( m_wstrGuildMasterName )
		&& PUT( m_vecMemberUnitUID )
		//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& PUT( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEGS_GUILD_INFO_NOT, obj, ks )
{
	return GET( m_kGuildInfo )
		&& GET( m_wstrGuildMasterName )
		&& GET( m_vecMemberUnitUID )
		//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		&& GET( m_kGuildSkillInfo )
#endif GUILD_SKILL_TEST
		//}}
		;
}


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 4  최육사	접속시간이벤트시스템개편
SERIALIZE_DEFINE_PUT( KUserConnectTimeEventInfo, obj, ks )
{
	return PUT( m_iEventID )
		&& PUT( m_fEventTime )
		&& PUT( m_kCTime )
		&& PUT( m_wstrEventName )
		&& PUT( m_bInit )
		&& PUT( m_bGiveOnce )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& PUT( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}
		//{{ 2010. 7. 27  최육사	접속 시간 이벤트 버그 수정
#ifdef SERV_TIME_EVENT_BUG_FIX
		&& PUT( m_fBackUpTime )
#endif SERV_TIME_EVENT_BUG_FIX
		//}}

		//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		&& PUT( m_iScriptID )
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
		&& PUT( m_bRepeatRewardEvent )
		&& PUT( m_iBeforeEventID )
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		&& PUT( m_iEventItemID )
#endif
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& PUT( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& PUT( m_bNewUnit )
		&& PUT( m_bNewUnit2 )
		&& PUT( m_bCurrentUnit )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		&& PUT( m_bOnlySteamUser )
		&& PUT( m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
		;
}

SERIALIZE_DEFINE_GET( KUserConnectTimeEventInfo, obj, ks )
{
	return GET( m_iEventID )
		&& GET( m_fEventTime )
		&& GET( m_kCTime )
		&& GET( m_wstrEventName )
		&& GET( m_bInit )
		&& GET( m_bGiveOnce )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& GET( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}
		//{{ 2010. 7. 27  최육사	접속 시간 이벤트 버그 수정
#ifdef SERV_TIME_EVENT_BUG_FIX
		&& GET( m_fBackUpTime )
#endif SERV_TIME_EVENT_BUG_FIX
		//}}

		//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		&& GET( m_iScriptID )
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
		//}}
#ifdef SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
		&& GET( m_bRepeatRewardEvent )
		&& GET( m_iBeforeEventID )
#endif //SERV_CONNECT_EVENT_CONSECUTIVELY_REWARD
#ifdef SERV_ITEM_IN_INVENTORY_CONNECT_EVENT
		&& GET( m_iEventItemID )
#endif
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& GET( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& GET( m_bNewUnit )
		&& GET( m_bNewUnit2 )
		&& GET( m_bCurrentUnit )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		&& GET( m_bOnlySteamUser )
		&& GET( m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
		;
}


SERIALIZE_DEFINE_PUT( KUserCumulativeTimeEventInfo, obj, ks )
{
	return PUT( m_iEventUID )
		&& PUT( m_iEventID )
		&& PUT( m_bInit )
		&& PUT( m_wstrEventName )
		&& PUT( m_fEventTime )
		&& PUT( m_bDirectReward )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& PUT( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& PUT( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& PUT( m_bNewUnitEvent )
		&& PUT( m_bNewUnitEvent2 )
		&& PUT( m_bCurrentUnitEvent )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		;
}

SERIALIZE_DEFINE_GET( KUserCumulativeTimeEventInfo, obj, ks )
{
	return GET( m_iEventUID )
		&& GET( m_iEventID )
		&& GET( m_bInit )
		&& GET( m_wstrEventName )
		&& GET( m_fEventTime )
		&& GET( m_bDirectReward )
		//{{ 2010. 06. 11  최육사	계정단위 접속시간 이벤트
#ifdef SERV_ACC_TIME_EVENT
		&& GET( m_bAccountEvent )
#endif SERV_ACC_TIME_EVENT
		//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& GET( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT

#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& GET( m_bNewUnitEvent )
		&& GET( m_bNewUnitEvent2 )
		&& GET( m_bCurrentUnitEvent )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR

		;
}


SERIALIZE_DEFINE_PUT( KTimeEventSimpleInfo, obj, ks )
{
	return PUT( m_iEventUID )
		&& PUT( m_iEventID )
		&& PUT( m_wstrEventName )
		&& PUT( m_fEventTime )
#ifdef SERV_ADVERTISEMENT_EVENT
		&& PUT( m_wstrAdvertisementURL )
#endif SERV_ADVERTISEMENT_EVENT
		;
}

SERIALIZE_DEFINE_GET( KTimeEventSimpleInfo, obj, ks )
{
	return GET( m_iEventUID )
		&& GET( m_iEventID )
		&& GET( m_wstrEventName )
		&& GET( m_fEventTime )
#ifdef SERV_ADVERTISEMENT_EVENT
		&& GET( m_wstrAdvertisementURL )
#endif SERV_ADVERTISEMENT_EVENT
		;
}
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 12. 16  최육사	동접툴
SERIALIZE_DEFINE_PUT( KCCUInfo, obj, ks )
{
	return PUT( m_iCCUTime )
		&& PUT( m_iCCUCount )
		;
}

SERIALIZE_DEFINE_GET( KCCUInfo, obj, ks )
{
	return GET( m_iCCUTime )
		&& GET( m_iCCUCount )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_TOOL_GET_CCU_INFO_REQ, obj, ks )
{
	return PUT( m_cCCUType )
		&& PUT( m_iServerUID )
		&& PUT( m_wstrDate )
		;
}

SERIALIZE_DEFINE_GET( KE_TOOL_GET_CCU_INFO_REQ, obj, ks )
{
	return GET( m_cCCUType )
		&& GET( m_iServerUID )
		&& GET( m_wstrDate )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KE_TOOL_GET_CCU_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_cCCUType )
		&& PUT( m_iServerUID )
		&& PUT( m_wstrDate )
		&& PUT( m_vecCCUInfo )
		;
}

SERIALIZE_DEFINE_GET( KE_TOOL_GET_CCU_INFO_ACK, obj, ks )
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


//////////////////////////////////////////////////////////////////////////
//{{ 2009. 12. 31  최육사	길드 광고
SERIALIZE_DEFINE_PUT( KGuildAdInfo, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_wstrGuildName )
		&& PUT( m_ucGuildLevel )
		&& PUT( m_iGuildEXP )
		&& PUT( m_wstrMasterNickName )
		&& PUT( m_usCurNumMember )
		&& PUT( m_wstrAdMessage )
		//{{ 2010. 02. 04  최육사	길드 광고 오류 수정
		&& PUT( m_wstrFoundingDay )
		//}}
		&& PUT( m_wstrAdRegDate )
		&& PUT( m_wstrAdEndDate )
		;
}

SERIALIZE_DEFINE_GET( KGuildAdInfo, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_wstrGuildName )
		&& GET( m_ucGuildLevel )
		&& GET( m_iGuildEXP )
		&& GET( m_wstrMasterNickName )
		&& GET( m_usCurNumMember )		
		&& GET( m_wstrAdMessage )
		//{{ 2010. 02. 04  최육사	길드 광고 오류 수정
		&& GET( m_wstrFoundingDay )
		//}}
		&& GET( m_wstrAdRegDate )
		&& GET( m_wstrAdEndDate )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KApplyJoinGuildInfo, obj, ks )
{
	return PUT( m_iApplyJoinGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_wstrMessage )
		&& PUT( m_wstrRegDate )
		&& PUT( m_tRegDate )
		;
}

SERIALIZE_DEFINE_GET( KApplyJoinGuildInfo, obj, ks )
{
	return GET( m_iApplyJoinGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_wstrMessage )
		&& GET( m_wstrRegDate )
		&& GET( m_tRegDate )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KApplyDeleteInfo, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KApplyDeleteInfo, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		;
}

//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 01. 07  최육사	추천인리스트
SERIALIZE_DEFINE_PUT( KRecommendUserInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_ucLevel )
		&& PUT( m_wstrRecommendDate )
		&& PUT( m_bIsOnline )
#ifdef SERV_RECRUIT_EVENT_BASE
		&& PUT( m_cUnitClass )
#endif SERV_RECRUIT_EVENT_BASE
		;
}

SERIALIZE_DEFINE_GET( KRecommendUserInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_ucLevel )
		&& GET( m_wstrRecommendDate )
		&& GET( m_bIsOnline )
#ifdef SERV_RECRUIT_EVENT_BASE
		&& GET( m_cUnitClass )
#endif SERV_RECRUIT_EVENT_BASE
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 02. 06  최육사	일일 이벤트 퀘스트
#ifdef SERV_DAY_QUEST

SERIALIZE_DEFINE_PUT( KEventQuestInfo, obj, ks )
{
	return PUT( m_bInit )
		&& PUT( m_iEventUID )
		&& PUT( m_iQuestID )
		;
}

SERIALIZE_DEFINE_GET( KEventQuestInfo, obj, ks )
{
	return GET( m_bInit )
		&& GET( m_iEventUID )
		&& GET( m_iQuestID )
		;
}

#endif SERV_DAY_QUEST
//}}


//{{ 2010. 02. 12  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST 

SERIALIZE_DEFINE_PUT( KHackingToolInfo, obj, ks )
{
	return PUT( m_wstrProcessName )
		&& PUT( m_wstrWindowClassName )
		&& PUT( m_cFlag )		
		;
}

SERIALIZE_DEFINE_GET( KHackingToolInfo, obj, ks )
{
	return GET( m_wstrProcessName )
		&& GET( m_wstrWindowClassName )
		&& GET( m_cFlag )		
		;
}

#endif SERV_HACKING_TOOL_LIST
//}}

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KWebPointRewardInfo, obj, ks )
{
	return PUT( m_iApplySN )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iUnitUID )
		&& PUT( m_iRewardItemID )
		;
}

SERIALIZE_DEFINE_GET( KWebPointRewardInfo, obj, ks )
{
	return GET( m_iApplySN )
		&& GET( m_wstrUserID )
		&& GET( m_wstrNickName )
		&& GET( m_iUnitUID )
		&& GET( m_iRewardItemID )
		;
}

#endif SERV_WEB_POINT_EVENT
//}}

#ifdef DEF_TRADE_BOARD
SERIALIZE_DEFINE_PUT( KTradeBoardItemInfo, obj, ks )
{
	return PUT( m_iPersonalShopUID )
		//{{ 2011. 05. 03	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& PUT( m_iHostUnitUID )
#endif SERV_PSHOP_AGENCY
		//}}
		&& PUT( m_wstrSellerNickName )
		&& PUT( m_kSellItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KTradeBoardItemInfo, obj, ks )
{
	return GET( m_iPersonalShopUID )
		//{{ 2011. 05. 03	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		&& GET( m_iHostUnitUID )
#endif SERV_PSHOP_AGENCY
		//}}
		&& GET( m_wstrSellerNickName )
		&& GET( m_kSellItemInfo )
		;
}
#endif DEF_TRADE_BOARD



//{{ 2010. 05. 12  최육사	서버 코드 정리
SERIALIZE_DEFINE_PUT( KDungeonPlayResultInfo, obj, ks )
{
	return PUT( m_fHP )
		&& PUT( m_nDamageCount )
		&& PUT( m_TotalScore )
		&& PUT( m_ComboScore )
		&& PUT( m_TechScore )
		//{{ 2011. 01. 21	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
		&& PUT( m_iTotalGivenDamamge )
		&& PUT( m_iTotalAttackedDamage )
		&& PUT( m_iTotalSumNpcHP )
		&& PUT( m_iTotalRecoveryHP )
		&& PUT( m_iUsingKindOfSkill )
		&& PUT( m_iCommandTechScore )
		&& PUT( m_iSkillTechScore )
#endif SERV_DUNGEON_RANK_NEW
		//}}
		//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
		&& PUT( m_iDashCount )
#endif SERV_DUNGEON_DASH_LOG
		//}}
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& PUT( m_iFrame )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
		&& PUT( m_bSkillSlotType )
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
		//}}
		;
}

SERIALIZE_DEFINE_GET( KDungeonPlayResultInfo, obj, ks )
{
	return GET( m_fHP )
		&& GET( m_nDamageCount )
		&& GET( m_TotalScore )
		&& GET( m_ComboScore )
		&& GET( m_TechScore )
		//{{ 2011. 01. 21	최육사	던전 랭크 개편
#ifdef SERV_DUNGEON_RANK_NEW
		&& GET( m_iTotalGivenDamamge )
		&& GET( m_iTotalAttackedDamage )
		&& GET( m_iTotalSumNpcHP )
		&& GET( m_iTotalRecoveryHP )
		&& GET( m_iUsingKindOfSkill )
		&& GET( m_iCommandTechScore )
		&& GET( m_iSkillTechScore )
#endif SERV_DUNGEON_RANK_NEW
		//}}
		//{{ 2012. 03. 05	김민성	던전 대시 로그
#ifdef SERV_DUNGEON_DASH_LOG
		&& GET( m_iDashCount )
#endif SERV_DUNGEON_DASH_LOG
		//}}
		//{{ 2012. 04. 17	최육사	던전 로그 컬럼 추가
#ifdef SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		&& GET( m_iFrame )
#endif SERV_ADD_DUNGEON_LOG_COLUMN_NUM_2
		//}}
		//{{ 2012. 12. 12  스킬 슬롯 사용 타입 로그 - 김민성
#ifdef SERV_USE_SKILL_SLOT_TYPE_LOG
		&& GET( m_bSkillSlotType )
#endif SERV_USE_SKILL_SLOT_TYPE_LOG
		//}}
		;
}
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW

SERIALIZE_DEFINE_PUT( KUpdateCCUInfo, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_iConcurrentUser )
		&& PUT( m_iCurPartyCount )
		&& PUT( m_iPlayGamePartyCount )
#ifdef SERV_CHANNELING_USER_MANAGER
		&& PUT( m_mapChannelingUserList )
#endif SERV_CHANNELING_USER_MANAGER
		;
}

SERIALIZE_DEFINE_GET( KUpdateCCUInfo, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_iConcurrentUser )
		&& GET( m_iCurPartyCount )
		&& GET( m_iPlayGamePartyCount )
#ifdef SERV_CHANNELING_USER_MANAGER
		&& GET( m_mapChannelingUserList )
#endif SERV_CHANNELING_USER_MANAGER
		;
}

#endif SERV_CCU_NEW
//}}

//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
SERIALIZE_DEFINE_PUT( KRolicyResults, obj, ks )
{
	return PUT( m_byteRolicyCount )
		&& PUT( m_byteRolicyListNo )
		&& PUT( m_wstrRolicyName )
		&& PUT( m_byteRolicyResult )
		&& PUT( m_wstrOption )
		;
}

SERIALIZE_DEFINE_GET( KRolicyResults, obj, ks )
{
	return GET( m_byteRolicyCount )
		&& GET( m_byteRolicyListNo )
		&& GET( m_wstrRolicyName )
		&& GET( m_byteRolicyResult )
		&& GET( m_wstrOption )
		;
}
#endif SERV_SHUTDOWN_SYSTEM
//}}

//{{ 2010. 06. 07  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW

SERIALIZE_DEFINE_PUT( KPropertyValue, obj, ks )
{
	return PUT( m_bytePropertyValue )
		&& PUT( m_uintPropertyValue )
		&& PUT( m_ulongPropertyValue )
		&& PUT( m_wstrPropertyValue )
		&& PUT( m_vecPropertyValue )
		//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
		&& PUT( m_mapRolicyResult )
		&& PUT( m_int64PropertyValue )
#endif SERV_SHUTDOWN_SYSTEM
	//}}
		;
}

SERIALIZE_DEFINE_GET( KPropertyValue, obj, ks )
{
	return GET( m_bytePropertyValue )
		&& GET( m_uintPropertyValue )
		&& GET( m_ulongPropertyValue )
		&& GET( m_wstrPropertyValue )
		&& GET( m_vecPropertyValue )
		//{{ 2011. 09. 29  김민성	셧다운제 도입
#ifdef SERV_SHUTDOWN_SYSTEM
		&& GET( m_mapRolicyResult )
		&& GET( m_int64PropertyValue )
#endif SERV_SHUTDOWN_SYSTEM
	//}}
		;
}

#endif SERV_PCBANG_AUTH_NEW
//}}


//{{ 2010. 06. 20  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD

SERIALIZE_DEFINE_PUT( KStatisticsInfo, obj, ks )
{
	return PUT( m_iStatisticsID )
		&& PUT( m_kKey )
		&& PUT( m_iIndex )
		&& PUT( m_iCount )
		;
}

SERIALIZE_DEFINE_GET( KStatisticsInfo, obj, ks )
{
	return GET( m_iStatisticsID )
		&& GET( m_kKey )
		&& GET( m_iIndex )
		&& GET( m_iCount )
		;
}

#endif SERV_STATISTICS_THREAD
//}}

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

SERIALIZE_DEFINE_PUT( KItemPeriodInfo, obj, ks )
{
	return PUT( m_iItemUID )
		&& PUT( m_sPeriod )
		&& PUT( m_wstrExpirationDate )
		;
}

SERIALIZE_DEFINE_GET( KItemPeriodInfo, obj, ks )
{
	return GET( m_iItemUID )
		&& GET( m_sPeriod )
		&& GET( m_wstrExpirationDate )
		;
}

#endif SERV_RESET_PERIOD_EVENT
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER

SERIALIZE_DEFINE_PUT( KModuleInfo, obj, ks )
{
	return PUT( m_iModuleUID )
		&& PUT( m_wstrModuleName )
		&& PUT( m_iCount )
		&& PUT( m_iFlag )
		//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
		&& PUT( m_iAllow )
#else
		&& PUT( m_bAllow )
#endif SERV_HACKING_DLL_CHECK
		//}}
		&& PUT( m_bReserveDBUpdate )
		;
}

SERIALIZE_DEFINE_GET( KModuleInfo, obj, ks )
{
	return GET( m_iModuleUID )
		&& GET( m_wstrModuleName )
		&& GET( m_iCount )
		&& GET( m_iFlag )
		//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
		&& GET( m_iAllow )
#else
		&& GET( m_bAllow )
#endif SERV_HACKING_DLL_CHECK
		//}}
		&& GET( m_bReserveDBUpdate )
		;
}

#endif SERV_DLL_MANAGER
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 10. 27	최육사	펫 오라 스킬
#ifdef SERV_PET_AURA_SKILL

SERIALIZE_DEFINE_PUT( KStatIncreaseRate, obj, ks )
{
	return PUT( m_fIncreaseAtkPhysicRate )
		&& PUT( m_fIncreaseAtkMagicRate )
		&& PUT( m_fIncreaseDefPhysicRate )
		&& PUT( m_fIncreaseDefMagicRate )
		;
}

SERIALIZE_DEFINE_GET( KStatIncreaseRate, obj, ks )
{
	return GET( m_fIncreaseAtkPhysicRate )
		&& GET( m_fIncreaseAtkMagicRate )
		&& GET( m_fIncreaseDefPhysicRate )
		&& GET( m_fIncreaseDefMagicRate )
		;
}

#endif SERV_PET_AURA_SKILL
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 11. 17	최육사	오토핵 검증 기능
//#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM

SERIALIZE_DEFINE_PUT( KAbuserEventLog, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iED )
		&& PUT( m_iGetItemCount )
		//{{ 2013. 05. 29	최육사	패킷 모니터링 근성도 항목 추가
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
		&& PUT( m_iSpirit )
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
		//}}
		&& PUT( m_iRegDate )
		&& PUT( m_usEventID )
		;
}

SERIALIZE_DEFINE_GET( KAbuserEventLog, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iED )
		&& GET( m_iGetItemCount )
		//{{ 2013. 05. 29	최육사	패킷 모니터링 근성도 항목 추가
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
		&& GET( m_iSpirit )
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
		//}}
		&& GET( m_iRegDate )
		&& GET( m_usEventID )
		;
}

//#endif SERV_AUTO_HACK_CHECK_GET_ITEM
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 11. 20	최육사	특정 시각 몬스터 출현
#ifdef SERV_TIME_DROP_MONSTER_EVENT

SERIALIZE_DEFINE_PUT( KDungeonDropEventNpcInfo, obj, ks )
{
	return PUT( m_iEventUID )		
		&& PUT( m_iNpcID )
		&& PUT( m_tDropDate )
		;
}

SERIALIZE_DEFINE_GET( KDungeonDropEventNpcInfo, obj, ks )
{
	return GET( m_iEventUID )		
		&& GET( m_iNpcID )
		&& GET( m_tDropDate )
		;
}

#endif SERV_TIME_DROP_MONSTER_EVENT
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 8	최육사	적정 레벨 장비 통계
#ifdef SERV_APPROPRIATE_LEVEL_STAT

SERIALIZE_DEFINE_PUT( KUserAppropriateLevelInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_ucLevel )
		&& PUT( m_iWeaponLevel )
		&& PUT( m_iWeaponGrade )
		&& PUT( m_iStartedAtkPhysic )
		&& PUT( m_iStartedAtkMagic )
		;
}

SERIALIZE_DEFINE_GET( KUserAppropriateLevelInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_ucLevel )
		&& GET( m_iWeaponLevel )
		&& GET( m_iWeaponGrade )
		&& GET( m_iStartedAtkPhysic )
		&& GET( m_iStartedAtkMagic )
		;
}

#endif SERV_APPROPRIATE_LEVEL_STAT
//}}


//////////////////////////////////////////////////////////////////////////
//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT

SERIALIZE_DEFINE_PUT( KTimeOpenRandomItemEventInfo, obj, ks )
{
	return PUT( m_iID )
		&& PUT( m_iItemID )
		&& PUT( m_wstrDropTime )
		//{{ 2011. 07. 04    김민성    네이버 프로모션
#ifdef SERV_NAVER_PROMOTIONS
		&& PUT( m_wstrDropEndTime )
#endif SERV_NAVER_PROMOTIONS
	//}} 
	//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		&& PUT( m_iOpenItemID )
#endif SERV_RANDOM_CUBE_GOLD_REWARD
	//}}
		;
}

SERIALIZE_DEFINE_GET( KTimeOpenRandomItemEventInfo, obj, ks )
{
	return GET( m_iID )
		&& GET( m_iItemID )
		&& GET( m_wstrDropTime )
		//{{ 2011. 07. 04    김민성    네이버 프로모션
#ifdef SERV_NAVER_PROMOTIONS
		&& GET( m_wstrDropEndTime )
#endif SERV_NAVER_PROMOTIONS
	//}} 
	//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
		&& GET( m_iOpenItemID )
#endif SERV_RANDOM_CUBE_GOLD_REWARD
	//}}
		;
}


SERIALIZE_DEFINE_PUT( KTimeOpenRandomItemEventDBUPdate, obj, ks )
{
	return PUT( m_iID )
		&& PUT( m_tUpdateTime )
		&& PUT( m_iGetUserUID )
		&& PUT( m_iGetUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KTimeOpenRandomItemEventDBUPdate, obj, ks )
{
	return GET( m_iID )
		&& GET( m_tUpdateTime )
		&& GET( m_iGetUserUID )
		&& GET( m_iGetUnitUID )
		;
}

#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT

//{{ 2011. 01. 13 김민성	동접 모니터링 툴
#ifdef SERV_CCU_MONITORING_TOOL
//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KToolServerList, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_iServerSetID )
		&& PUT( m_iServerType )
		&& PUT( m_iChannelID )
		&& PUT( m_wstrChannelName )
		&& PUT( m_wstrIP )
		&& PUT( m_iPort )
		;
}

SERIALIZE_DEFINE_GET( KToolServerList, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_iServerSetID )
		&& GET( m_iServerType )
		&& GET( m_iChannelID )
		&& GET( m_wstrChannelName )
		&& GET( m_wstrIP )
		&& GET( m_iPort )
		;
}

SERIALIZE_DEFINE_PUT( KE_TOOL_CHECK_LOGIN_REQ, obj, ks )
{
	return PUT( m_wstrID )
		&& PUT( m_wstrPW )
		;
}
SERIALIZE_DEFINE_GET( KE_TOOL_CHECK_LOGIN_REQ, obj, ks )
{
	return GET( m_wstrID )
		&& GET( m_wstrPW )
		;
}
SERIALIZE_DEFINE_PUT( KE_TOOL_CHECK_LOGIN_ACK, obj, ks )
{
	return PUT( m_iAuthLevel)
		;
}
SERIALIZE_DEFINE_GET( KE_TOOL_CHECK_LOGIN_ACK, obj, ks )
{
	return GET( m_iAuthLevel )
		;
}

SERIALIZE_DEFINE_PUT( KE_TOOL_SERVER_LIST_ACK, obj, ks )
{ 
	return PUT( m_iServerCount)
		&& PUT( m_vecServerList)
		;
}
SERIALIZE_DEFINE_GET( KE_TOOL_SERVER_LIST_ACK, obj, ks )
{
	return GET( m_iServerCount )
		&& GET( m_vecServerList)
		;
}
#endif SERV_CCU_MONITORING_TOOL
//}}

//{{ 2011. 01. 25  김민성  월드 트리거 전달(던전만 적용중 - 이후 PVP 확장)
#ifdef SERV_WORLD_TRIGGER_RELOCATION
SERIALIZE_DEFINE_PUT( KEGS_WORLD_TRIGGER_RELOCATION_REQ, obj, ks )
{ 
	return PUT( m_iWorldTrigger)
		;
}
SERIALIZE_DEFINE_GET( KEGS_WORLD_TRIGGER_RELOCATION_REQ, obj, ks )
{
	return GET( m_iWorldTrigger )
		;
}
#endif SERV_WORLD_TRIGGER_RELOCATION
//}}


//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
SERIALIZE_DEFINE_PUT( KCashPruductInfo, obj, ks )
{
	return PUT( m_ulProductNo )
		&& PUT( m_sProductExpire )
		&& PUT( m_usProductPieces )
		&& PUT( m_iItemID )
		&& PUT( m_ulSalePrice )
		;
}

SERIALIZE_DEFINE_GET( KCashPruductInfo, obj, ks )
{
	return GET( m_ulProductNo )
		&& GET( m_sProductExpire )
		&& GET( m_usProductPieces )
		&& GET( m_iItemID )
		&& GET( m_ulSalePrice )
		;
}
#endif SERV_CASH_ITEM_LIST
//}}

//{{ 2011. 03. 26  김민성	청 1차 추가 스킬 추가 - 버프
#ifdef SERV_ADD_BUFF_SKILL_INFO
SERIALIZE_DEFINE_PUT( KBuffSkillInfo, obj, ks )
{
	return PUT( m_iCaughtUnitUID )
		&& PUT( m_iBuffEnum )
		&& PUT( m_iCastingUnitUID )
		&& PUT( m_bIsBuffCasting )
		&& PUT( m_cBuffLevel )
		;
}
SERIALIZE_DEFINE_GET( KBuffSkillInfo, obj, ks )
{
	return GET( m_iCaughtUnitUID )
		&& GET( m_iBuffEnum )
		&& GET( m_iCastingUnitUID )
		&& GET( m_bIsBuffCasting )
		&& GET( m_cBuffLevel )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_BUFF_SKILL_INFO_CONVEY_REQ, obj, ks )
{
	return PUT( m_iBuffEnum )
		&& PUT( m_iCastingUnitUID )
		&& PUT( m_fBuffTime )
		&& PUT( m_bIsBuffCasting )
		&& PUT( m_cBuffLevel )
		&& PUT( m_vecCaughtUIDList )
		;
}
SERIALIZE_DEFINE_GET( KEGS_BUFF_SKILL_INFO_CONVEY_REQ, obj, ks )
{
	return GET( m_iBuffEnum )
		&& GET( m_iCastingUnitUID )
		&& GET( m_fBuffTime )
		&& GET( m_bIsBuffCasting )
		&& GET( m_cBuffLevel )
		&& GET( m_vecCaughtUIDList )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_BUFF_SKILL_INFO_CONVEY_NOT, obj, ks )
{
	return PUT( m_mapCaughtUIDList )
		;
}
SERIALIZE_DEFINE_GET( KEGS_BUFF_SKILL_INFO_CONVEY_NOT, obj, ks )
{
	return GET( m_mapCaughtUIDList )
		;
}
#endif SERV_ADD_BUFF_SKILL_INFO
//}}

//{{ 2011. 04. 05  김민성	우편 및 거래 감시
#ifdef SERV_MONITORING_LETTER_AND_TRADE
SERIALIZE_DEFINE_PUT( KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ, obj, ks )
{
	return PUT( m_cType )
		&& PUT( m_iRecvUnitUID )
		&& PUT( m_wstrRegDate )
		;
}
SERIALIZE_DEFINE_GET( KDBE_WRITE_LETTER_AND_TRADE_MOINTORING_REQ, obj, ks )
{
	return GET( m_cType )
		&& GET( m_iRecvUnitUID )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_MONITORING_LETTER_AND_TRADE
//}}

//{{ 2011. 04. 13  김민성  글로벌 서버 추가
#ifdef SERV_INSERT_GLOBAL_SERVER
SERIALIZE_DEFINE_PUT( KDefenseOpenTime, obj, ks )
{
	return PUT( m_iStartHour )
		&& PUT( m_iStartMinute )
		&& PUT( m_iDurationTime )
		;
}
SERIALIZE_DEFINE_GET( KDefenseOpenTime, obj, ks )
{
	return GET( m_iStartHour )
		&& GET( m_iStartMinute )
		&& GET( m_iDurationTime )
		;
}

SERIALIZE_DEFINE_PUT( KAttribNpcUnitInfo, obj, ks )
{
	return PUT( m_sNPCUnitReq )
		&& PUT( m_sAttribEnchantNpcInfo )
		;
}
SERIALIZE_DEFINE_GET( KAttribNpcUnitInfo, obj, ks )
{
	return GET( m_sNPCUnitReq )
		&& GET( m_sAttribEnchantNpcInfo )
		;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 2011. 04. 13  김민성  글로벌 서버 추가

//{{ 2011. 06. 23	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM

SERIALIZE_DEFINE_PUT( KMatchUserInfo, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iPartyUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		
		//{{ 2012. 06. 20	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& PUT( m_cCurrentRank )
		&& PUT( m_iOfficialMatchCnt )
#endif SERV_2012_PVP_SEASON2
		//}}

		&& PUT( m_iRating )
		&& PUT( m_iRPoint )
		&& PUT( m_iMatchWaitTime )
		;
}

SERIALIZE_DEFINE_GET( KMatchUserInfo, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_iPartyUID )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )

		//{{ 2012. 06. 20	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		&& GET( m_cCurrentRank )
		&& GET( m_iOfficialMatchCnt )
#endif SERV_2012_PVP_SEASON2
		//}}

		&& GET( m_iRating )
		&& GET( m_iRPoint )
		&& GET( m_iMatchWaitTime )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KPvpNpcInfo, obj, ks )
{
	return PUT( m_iNpcID )
		;
}

SERIALIZE_DEFINE_GET( KPvpNpcInfo, obj, ks )
{
	return GET( m_iNpcID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KPvpMatchUserResultLog, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_ucLevel )
		&& PUT( m_cUnitClass )
		&& PUT( m_iRating )
		&& PUT( m_cPvpRank )
		&& PUT( m_iRPoint )
		&& PUT( m_iAPoint )
		&& PUT( m_iOfficialMatchCnt )
		&& PUT( m_cTeam )
		&& PUT( m_bPartyApp )
		&& PUT( m_iWaitTime )
		&& PUT( m_iWin )		
		&& PUT( m_iKill )
		&& PUT( m_iAssist )
		&& PUT( m_iDeath )
		&& PUT( m_iNetRatingInc )
		&& PUT( m_iNetRPInc )
		&& PUT( m_iNetAPInc )
		&& PUT( m_iEXPearned )
		&& PUT( m_cEndType )
		;
}

SERIALIZE_DEFINE_GET( KPvpMatchUserResultLog, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_ucLevel )
		&& GET( m_cUnitClass )
		&& GET( m_iRating )
		&& GET( m_cPvpRank )
		&& GET( m_iRPoint )
		&& GET( m_iAPoint )
		&& GET( m_iOfficialMatchCnt )
		&& GET( m_cTeam )
		&& GET( m_bPartyApp )
		&& GET( m_iWaitTime )
		&& GET( m_iWin )		
		&& GET( m_iKill )
		&& GET( m_iAssist )
		&& GET( m_iDeath )
		&& GET( m_iNetRatingInc )
		&& GET( m_iNetRPInc )
		&& GET( m_iNetAPInc )
		&& GET( m_iEXPearned )
		&& GET( m_cEndType )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KPvpPlayResultInfo, obj, ks )
{
	return PUT( m_iRPoint )
		&& PUT( m_iAPoint )
		&& PUT( m_iWinCount )
		&& PUT( m_bIsPlayWithParty )
		;
}

SERIALIZE_DEFINE_GET( KPvpPlayResultInfo, obj, ks )
{
	return GET( m_iRPoint )
		&& GET( m_iAPoint )
		&& GET( m_iWinCount )
		&& GET( m_bIsPlayWithParty )
		;
}


#endif SERV_PVP_NEW_SYSTEM
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
SERIALIZE_DEFINE_PUT( KRewardItem, obj, ks )
{
	return PUT( m_iItemID )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KRewardItem, obj, ks )
{
	return GET( m_iItemID )
		&& GET( m_iQuantity )
		;
}
#else
//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
SERIALIZE_DEFINE_PUT( KRewardItem, obj, ks )
{
	return PUT( m_iItemID )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KRewardItem, obj, ks )
{
	return GET( m_iItemID )
		&& GET( m_iQuantity )
		;
}
#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2012. 10. 10	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
SERIALIZE_DEFINE_PUT( KDBE_BINGO_EVENT_INFO_READ_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iBalance )
		&& PUT( m_iLotteryChance )
		&& PUT( m_iLotteryRecord )
		&& PUT( m_iMixChance )
		&& PUT( m_iResetChance )
		&& PUT( m_vecBingoBoard )
		&& PUT( m_vecPresentInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_BINGO_EVENT_INFO_READ_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iBalance )
		&& GET( m_iLotteryChance )
		&& GET( m_iLotteryRecord )
		&& GET( m_iMixChance )
		&& GET( m_iResetChance )
		&& GET( m_vecBingoBoard )
		&& GET( m_vecPresentInfo )
		;
}
#endif SERV_EVENT_BINGO
//}}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
#ifdef SERV_CODE_EVENT
		&& PUT( m_setCodeEventScriptID )
#endif //SERV_CODE_EVENT
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& PUT( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
#ifdef SERV_CODE_EVENT
		&& GET( m_setCodeEventScriptID )
#endif //SERV_CODE_EVENT
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& GET( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}

/////////////////////////////////////////////////////////////////////////
//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_1_NOT, obj, ks )
{
	return	PUT( m_iOK )
		&&	PUT( m_kUnitInfo )
	//{{ 2011. 01. 18	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&&	PUT( m_wstrUnitCreateDate )
#endif SERV_CHAR_LOG
	//}}
	//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
		&&	PUT( m_iRealDataED )
#endif SERV_LOGOUT_ED_CHECK
	//}}
	//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
		&&	PUT( m_wstrUnitLastLoginDate )
#endif SERV_DAILY_CHAR_FIRST_SELECT
	//}}
		&&	PUT( m_mapInventorySlotSize )
		&&	PUT( m_mapItem )
		&&	PUT( m_iNumResurrectionStone )
	//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&&	PUT( m_iNumAutoPaymentResStone )
		&&	PUT( m_wstrAutoPayResStoneLastDate )
#endif AP_RESTONE
	//}}
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
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		&& PUT( m_bIsNewUnitTradeBlock )
		&& PUT( m_iNewUnitTradeBlockDay )
		&& PUT( m_iNewUnitTradeBlockUnitClass )
#endif // SERV_NEW_UNIT_TRADE_LIMIT
	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& PUT( m_kRelationshipInfo )
#endif SERV_RELATIONSHIP_SYSTEM
	//}
		;

}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_1_NOT, obj, ks )
{
	return	GET( m_iOK )
		&&  GET( m_kUnitInfo )
		//{{ 2011. 01. 18	최육사	캐릭터 카운트 정보
#ifdef SERV_CHAR_LOG
		&&	GET( m_wstrUnitCreateDate )
#endif SERV_CHAR_LOG
		//}}
		//{{ 2011. 09. 23	최육사	로그오프 상태 ED체크
#ifdef SERV_LOGOUT_ED_CHECK
		&&	GET( m_iRealDataED )
#endif SERV_LOGOUT_ED_CHECK
		//}}
		//{{ 2011. 03. 22	최육사	캐릭터 첫 접속 로그
#ifdef SERV_DAILY_CHAR_FIRST_SELECT
		&&	GET( m_wstrUnitLastLoginDate )
#endif SERV_DAILY_CHAR_FIRST_SELECT
		//}}
		&&	GET( m_mapInventorySlotSize )
		&&	GET( m_mapItem )
		&&	GET( m_iNumResurrectionStone )
		//{{ 2009. 10. 14  최육사	리필부활석
#ifdef AP_RESTONE
		&&	GET( m_iNumAutoPaymentResStone )
		&&	GET( m_wstrAutoPayResStoneLastDate )
#endif AP_RESTONE
		//}}
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
#ifdef SERV_NEW_UNIT_TRADE_LIMIT
		&& GET( m_bIsNewUnitTradeBlock )
		&& GET( m_iNewUnitTradeBlockDay )
		&& GET( m_iNewUnitTradeBlockUnitClass )
#endif // SERV_NEW_UNIT_TRADE_LIMIT
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
		&& GET( m_kRelationshipInfo )
#endif SERV_RELATIONSHIP_SYSTEM
		//}
		;
}

SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_2_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecQuest )
		&& PUT( m_vecCompletQuest )
	//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
		&& PUT( m_mapRandomQuestList )
#endif SERV_RANDOM_DAY_QUEST
	//}}
		;
}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_2_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecQuest )
		&& GET( m_vecCompletQuest )
		//{{ 2011. 08. 29	김민성       일일 랜덤 퀘스트
#ifdef SERV_RANDOM_DAY_QUEST
		&& GET( m_mapRandomQuestList )
#endif SERV_RANDOM_DAY_QUEST
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_3_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecMission )
		&& PUT( m_vecTitle )
		//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_iSummonedPetUID )
		&& PUT( m_vecPetList )
		&& PUT( m_vecCanEvolutionPetList )
		&& PUT( m_vecNeverSummonPetList )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& PUT( m_mapPetItem )
#endif SERV_PET_SYSTEM
		//}}	
		;
}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_3_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecMission )
		&& GET( m_vecTitle )
		//{{ 2010. 7. 21  최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_iSummonedPetUID )
		&& GET( m_vecPetList )
		&& GET( m_vecCanEvolutionPetList )
		&& GET( m_vecNeverSummonPetList )
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 8. 2	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		&& GET( m_mapPetItem )
#endif SERV_PET_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_4_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecChatBlackList )
		&& PUT( m_kDenyOptions )
		&& PUT( m_bIsRecommend )
		&& PUT( m_iRecommendUnitUID )
#ifdef SERV_RECRUIT_EVENT_BASE
		&& PUT( m_vecRecruiterUnitInfo )
		&& PUT( m_vecRecruitUnitInfo )
#endif SERV_RECRUIT_EVENT_BASE
		&& PUT( m_vecTutorialDBUnitInfo )
		&& PUT( m_vecConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& PUT( m_vecCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT
		//}}
		&& PUT( m_vecHenirRanking )
		&& PUT( m_kMessengerInfo )
		//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& PUT( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_4_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecChatBlackList )
		&& GET( m_kDenyOptions )
		&& GET( m_bIsRecommend )
		&& GET( m_iRecommendUnitUID )
#ifdef SERV_RECRUIT_EVENT_BASE
		&& GET( m_vecRecruiterUnitInfo )
		&& GET( m_vecRecruitUnitInfo )
#endif SERV_RECRUIT_EVENT_BASE
		&& GET( m_vecTutorialDBUnitInfo )
		&& GET( m_vecConnectTimeEvent )
		//{{ 2009. 12. 7  최육사	누적시간이벤트
#ifdef CUMULATIVE_TIME_EVENT
		&& GET( m_vecCumulativeTimeEvent )
#endif CUMULATIVE_TIME_EVENT
		//}}
		&& GET( m_vecHenirRanking )
		&& GET( m_kMessengerInfo )
		//{{ 2012. 02. 21	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
		&& GET( m_kGamePlayStatus )
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_5_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bIsSpiritUpdated )
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
		//{{ 2011. 10. 13	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
		&& PUT( m_bCashShopOpen )
#endif SERV_CONTENT_MANAGER
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
		//{{ 2012. 12. 20	최육사	아라 첫 선택 튜토리얼
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
		&& PUT( m_bFirstSelect )
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
		//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& PUT( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
		&& PUT( m_bLocalRankingUser )
#endif	// SERV_LOCAL_RANKING_SYSTEM
		
#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		&& PUT( m_iConnectExperienceAck )
		&& PUT( m_iReward7DaysItem )
#endif // SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& PUT( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		&& PUT( m_bCouple )
		&& PUT( m_iRelationTargetUserUid )
		&& PUT( m_wstrRelationTargetUserNickname )
#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		&& PUT( m_bOnlySteamUser )
		&& PUT(	m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
		&& PUT( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		&& PUT( m_wstrGiveMeTheItemTime_One )
		&& PUT( m_wstrGiveMeTheItemTime_Two )
		&& PUT( m_wstrGiveMeTheItemTime_Tree )
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& PUT( m_vec4ThAnnivEventRewardInfo )
		&& PUT( m_tLastRewardTime )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_5_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bIsSpiritUpdated )
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
		//}}
		//{{ 2011. 10. 13	최육사	컨텐츠 관리자
#ifdef SERV_CONTENT_MANAGER
		&& GET( m_bCashShopOpen )
#endif SERV_CONTENT_MANAGER
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
		//{{ 2012. 12. 20	최육사	아라 첫 선택 튜토리얼
#ifdef SERV_ARA_FIRST_SELECT_TUTORIAL
		&& GET( m_bFirstSelect )
#endif SERV_ARA_FIRST_SELECT_TUTORIAL
		//}}
#ifdef SERV_CUSTOM_CONNECT_EVENT
		&& GET( m_iCustomEventID )
#endif //SERV_CUSTOM_CONNECT_EVENT
#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
		&& GET( m_bLocalRankingUser )
#endif	// SERV_LOCAL_RANKING_SYSTEM
		
#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
		&& GET( m_iConnectExperienceAck )
		&& GET( m_iReward7DaysItem )
#endif // SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
#ifdef SERV_GATE_OF_DARKNESS_SUPPORT_EVENT
		&& GET( m_iGateOfDarknessSupportEventTime )
#endif SERV_GATE_OF_DARKNESS_SUPPORT_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
		&& GET( m_bCouple )
		&& GET( m_iRelationTargetUserUid )
		&& GET( m_wstrRelationTargetUserNickname )
#endif SERV_RELATIONSHIP_EVENT_INT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		&& GET( m_bOnlySteamUser )
		&& GET(	m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT

#ifdef SERV_ELESIS_UPDATE_EVENT
		&& GET( m_iNoteViewCount )
#endif SERV_ELESIS_UPDATE_EVENT
#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
		&& GET( m_wstrGiveMeTheItemTime_One )
		&& GET( m_wstrGiveMeTheItemTime_Two )
		&& GET( m_wstrGiveMeTheItemTime_Tree )
#endif SERV_EVENT_CHUNG_GIVE_ITEM
#ifdef SERV_4TH_ANNIVERSARY_EVENT
		&& GET( m_vec4ThAnnivEventRewardInfo )
		&& GET( m_tLastRewardTime )
#endif SERV_4TH_ANNIVERSARY_EVENT
		;
}

SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_ACK, obj, ks )
{
	return GET( m_iOK )
		;
}
#else
SERIALIZE_DEFINE_PUT( KEGS_SELECT_UNIT_ACK, obj, ks )
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
		&& PUT( m_vecSkillAcquired )
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
		;
}

SERIALIZE_DEFINE_GET( KEGS_SELECT_UNIT_ACK, obj, ks )
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
		&& GET( m_vecSkillAcquired )
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
		;
}
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}
//////////////////////////////////////////////////////////////////////////
//{{ 2011. 12. 14	최육사	패킷 처리 지연 시간 체크
#ifdef SERV_PACKET_PROCESSING_LATENCY_CHECK
SERIALIZE_DEFINE_PUT( KPacketProcessingLatencyInfo, obj, ks )
{
	return PUT( m_usEventID )
		&& PUT( m_iUnitUID )
		&& PUT( m_dwMaxLatency )
		&& PUT( m_wstrMaxLatencyTime )
		;
}

SERIALIZE_DEFINE_GET( KPacketProcessingLatencyInfo, obj, ks )
{
	return GET( m_usEventID )
		&& GET( m_iUnitUID )
		&& GET( m_dwMaxLatency )
		&& GET( m_wstrMaxLatencyTime )
		;
}


SERIALIZE_DEFINE_PUT( KPacketProcessingCountInfo, obj, ks )
{
	return PUT( m_usEventID )
		&& PUT( m_dwCount )
		;
}

SERIALIZE_DEFINE_GET( KPacketProcessingCountInfo, obj, ks )
{
	return GET( m_usEventID )
		&& GET( m_dwCount )
		;
}
#endif SERV_PACKET_PROCESSING_LATENCY_CHECK
//}}

//{{ 2012. 02. 03	박세훈	이벤트 관련정보 처리방법 변경 ( Script -> DB )
#ifdef SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
SERIALIZE_DEFINE_PUT( EVENT_DATA, obj, ks )
{
	return PUT( m_fEXPRate )
		&& PUT( m_fVPRate )
		&& PUT( m_fEDRate )

		&& PUT( m_iDungeonID )
		&& PUT( m_iDungeonMode )
		&& PUT( m_iUnitLevel )
		&& PUT( m_cUnitType )
		&& PUT( m_cUnitClass )
		&& PUT( m_iPartyNum )

		// 아이템 이벤트
		&& PUT( m_fEventTime )
		&& PUT( m_iEventReward )
		&& PUT( m_mapMinMaxReward )
		&& PUT( m_bGiveOnce )

		&& PUT( m_bDelEndEvent )
		&& PUT( m_bPcBangEvent )
		&& PUT( m_bDirectReward )
		&& PUT( m_bAccountEvent )

		&& PUT( m_iDropCount )
		&& PUT( m_bWithPlayPcBang )
		&& PUT( m_iChannelCode )
		&& PUT( m_iBF_Team )

		&& PUT( m_iHenirRewardEventCount )
		&& PUT( m_bHenirRewardUnLimited )

		&& PUT( m_iServerGroup )
		;
}

SERIALIZE_DEFINE_GET( EVENT_DATA, obj, ks )
{
	return GET( m_fEXPRate )
		&& GET( m_fVPRate )
		&& GET( m_fEDRate )

		&& GET( m_iDungeonID )
		&& GET( m_iDungeonMode )
		&& GET( m_iUnitLevel )
		&& GET( m_cUnitType )
		&& GET( m_cUnitClass )
		&& GET( m_iPartyNum )

		// 아이템 이벤트
		&& GET( m_fEventTime )
		&& GET( m_iEventReward )
		&& GET( m_mapMinMaxReward )
		&& GET( m_bGiveOnce )

		&& GET( m_bDelEndEvent )
		&& GET( m_bPcBangEvent )
		&& GET( m_bDirectReward )
		&& GET( m_bAccountEvent )

		&& GET( m_iDropCount )
		&& GET( m_bWithPlayPcBang )
		&& GET( m_iChannelCode )
		&& GET( m_iBF_Team )

		&& GET( m_iHenirRewardEventCount )
		&& GET( m_bHenirRewardUnLimited )

		&& GET( m_iServerGroup )
		;
}

SERIALIZE_DEFINE_PUT( EVENT_MONSTER_DATA, obj, ks )
{
	return PUT( m_iEventID )
		&& PUT( m_iMonsterDropCount )
		&& PUT( m_bIsAdventInBossStage )
		&& PUT( m_mapNpcIDAndRate )
		&& PUT( m_setAdventDungeon )
		&& PUT( m_setNoAdventDungeon )
		&& PUT( m_setNoAdventStage )
		//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
		&& PUT( m_bAllyNPC )
#endif SERV_ALLY_EVENT_MONSTER
		//}}
		;
}

SERIALIZE_DEFINE_GET( EVENT_MONSTER_DATA, obj, ks )
{
	return GET( m_iEventID )
		&& GET( m_iMonsterDropCount )
		&& GET( m_bIsAdventInBossStage )
		&& GET( m_mapNpcIDAndRate )
		&& GET( m_setAdventDungeon )
		&& GET( m_setNoAdventDungeon )
		&& GET( m_setNoAdventStage )
		//{{ 이벤트 몬스터 출현 ( 아군 몬스터	) - 김민성
#ifdef SERV_ALLY_EVENT_MONSTER
		&& GET( m_bAllyNPC )
#endif SERV_ALLY_EVENT_MONSTER
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KRewardData, obj, ks )
{
	return PUT( m_iRewardItemID )
		&& PUT( m_iQuantity )
		&& PUT( m_sPeriod )
		;
}

SERIALIZE_DEFINE_GET( KRewardData, obj, ks )
{
	return GET( m_iRewardItemID )
		&& GET( m_iQuantity )
		&& GET( m_sPeriod )
		;
}

#endif SERV_CHANGE_EVENT_INFO_SCRIPT_TO_DB
//}}
//////////////////////////////////////////////////////////////////////////
//{{ 2011. 11. 1	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
SERIALIZE_DEFINE_PUT( KBattleFieldJoinInfo, obj, ks )
{
	return PUT( m_iBattleFieldID )
		&& PUT( m_iStartPosIndex )
		&& PUT( m_vecPartyMemberList )
		&& PUT( m_bMoveForMyParty )
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        && PUT( m_bNowBattleFieldPositionInfoStartPosition )
        && PUT( m_usBattleFieldPositionValue )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
		;
}

SERIALIZE_DEFINE_GET( KBattleFieldJoinInfo, obj, ks )
{
	return GET( m_iBattleFieldID )
		&& GET( m_iStartPosIndex )
		&& GET( m_vecPartyMemberList )
		&& GET( m_bMoveForMyParty )
#ifdef  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
        && GET( m_bNowBattleFieldPositionInfoStartPosition )
        && GET( m_usBattleFieldPositionValue )
#endif  SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

		;
}


SERIALIZE_DEFINE_PUT( KBattleFieldRoomInfo, obj, ks )
{
	return PUT( m_iBattleFieldID )
		&& PUT( m_RoomUID )
		&& PUT( m_MaxSlot )
		&& PUT( m_mapUnitUIDPartyUID )
		&& PUT( m_mapPartyList )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-06	// 박세훈
		&& PUT( m_byteBossFieldState )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}

SERIALIZE_DEFINE_GET( KBattleFieldRoomInfo, obj, ks )
{
	return GET( m_iBattleFieldID )
		&& GET( m_RoomUID )
		&& GET( m_MaxSlot )
		&& GET( m_mapUnitUIDPartyUID )
		&& GET( m_mapPartyList )
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-06	// 박세훈
		&& GET( m_byteBossFieldState )
#endif // SERV_BATTLE_FIELD_BOSS
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDangerousEventInfo, obj, ks )
{
	return PUT( m_setEventInfo )
		;
}

SERIALIZE_DEFINE_GET( KDangerousEventInfo, obj, ks )
{
	return GET( m_setEventInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KOldPartyInfo, obj, ks )
{
	return PUT( m_iOldPartyUID )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& PUT( m_iOldHostUnitUID )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		&& PUT( m_setOldPartyMember )
		&& PUT( m_sOldPartyFever )
		;
}

SERIALIZE_DEFINE_GET( KOldPartyInfo, obj, ks )
{
	return GET( m_iOldPartyUID )
		//{{ 2012. 11. 13	최육사		파티 재집결 파티장 오류 수정
#ifdef SERV_REGROUP_OLD_PARTY_HOST
		&& GET( m_iOldHostUnitUID )
#endif SERV_REGROUP_OLD_PARTY_HOST
		//}}
		&& GET( m_setOldPartyMember )
		&& GET( m_sOldPartyFever )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KAutoPartyMakingProcessCondition, obj, ks )
{
	return PUT( m_iDungeonMode )
		&& PUT( m_iDungeonIDWithDif )
		&& PUT( m_iBeginLevel )
		&& PUT( m_iEndLevel )
		;
}

SERIALIZE_DEFINE_GET( KAutoPartyMakingProcessCondition, obj, ks )
{
	return GET( m_iDungeonMode )
		&& GET( m_iDungeonIDWithDif )
		&& GET( m_iBeginLevel )
		&& GET( m_iEndLevel )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KAutoPartyConditionInfo, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_cDifficultyLevel )
		&& PUT( m_cGetItemType )
		&& PUT( m_cDungeonMode )
		&& PUT( m_cLimitPlayerCount )
		;
}

SERIALIZE_DEFINE_GET( KAutoPartyConditionInfo, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_cDifficultyLevel )
		&& GET( m_cGetItemType )
		&& GET( m_cDungeonMode )
		&& GET( m_cLimitPlayerCount )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KAutoPartyUserInfo, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kOldPartyInfo )
		&& PUT( m_wstrNickName )
		&& PUT( m_cUnitClass )
		&& PUT( m_ucLevel )
		&& PUT( m_iAutoPartyWaitTime )
		&& PUT( m_bAutoPartyBonus )
		;
}

SERIALIZE_DEFINE_GET( KAutoPartyUserInfo, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUnitUID )
		&& GET( m_kOldPartyInfo )
		&& GET( m_wstrNickName )
		&& GET( m_cUnitClass )
		&& GET( m_ucLevel )
		&& GET( m_iAutoPartyWaitTime )
		&& GET( m_bAutoPartyBonus )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KRegroupPartyUserInfo, obj, ks )
{
	return PUT( m_kPartyUserInfo )
		&& PUT( m_kPartyRoomUserInfo )
		&& PUT( m_kOldPartyInfo )
		;
}

SERIALIZE_DEFINE_GET( KRegroupPartyUserInfo, obj, ks )
{
	return GET( m_kPartyUserInfo )
		&& GET( m_kPartyRoomUserInfo )
		&& GET( m_kOldPartyInfo )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KAutoPartyBonusInfo, obj, ks )
{
	return PUT( m_cDungeonMode )
		&& PUT( m_iDungeonIDWithDif )
		&& PUT( m_bActivatedBonus )
		;
}

SERIALIZE_DEFINE_GET( KAutoPartyBonusInfo, obj, ks )
{
	return GET( m_cDungeonMode )
		&& GET( m_iDungeonIDWithDif )
		&& GET( m_bActivatedBonus )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KPartyUserJoinRoomInfo, obj, ks )
{
	return PUT( m_iRoomUID )
		&& PUT( m_iBattleFieldID )
		;
}

SERIALIZE_DEFINE_GET( KPartyUserJoinRoomInfo, obj, ks )
{
	return GET( m_iRoomUID )
		&& GET( m_iBattleFieldID )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDungeonGameInfo, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_cDungeonMode )
		&& PUT( m_cDifficulty )
		;
}

SERIALIZE_DEFINE_GET( KDungeonGameInfo, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_cDungeonMode )
		&& GET( m_cDifficulty )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KDungeonGameSimpleInfo, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_cDungeonMode )
		;
}

SERIALIZE_DEFINE_GET( KDungeonGameSimpleInfo, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_cDungeonMode )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KAutoPartyDungeonBonusInfo, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_cDungeonMode )
		&& PUT( m_bActivatedBonus )
		;
}

SERIALIZE_DEFINE_GET( KAutoPartyDungeonBonusInfo, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_cDungeonMode )
		&& GET( m_bActivatedBonus )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KReturnToFieldInfo, obj, ks )
{
	return PUT( m_iReturnToFieldUnitUID )
		&& PUT( m_iRoomUID )
		&& PUT( m_setRemainUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KReturnToFieldInfo, obj, ks )
{
	return GET( m_iReturnToFieldUnitUID )
		&& GET( m_iRoomUID )
		&& GET( m_setRemainUnitUID )
		;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KBuffBehaviorFactor, obj, ks )
{
	return PUT( m_uiType )
		&& PUT( m_vecValues )
		;
}

SERIALIZE_DEFINE_GET( KBuffBehaviorFactor, obj, ks )
{
	return GET( m_uiType )
		&& GET( m_vecValues )
		;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KBuffFinalizerFactor, obj, ks )
{
	return PUT( m_uiType )
		&& PUT( m_vecValues )
		;
}

SERIALIZE_DEFINE_GET( KBuffFinalizerFactor, obj, ks )
{
	return GET( m_uiType )
		&& GET( m_vecValues )
		;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KBuffIdentity, obj, ks )
{
	return PUT( m_eBuffTempletID )
		&& PUT( m_uiUniqueNum )
		;
}

SERIALIZE_DEFINE_GET( KBuffIdentity, obj, ks )
{
	return GET( m_eBuffTempletID )
		&& GET( m_uiUniqueNum )
		;
}


//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KBuffFactor, obj, ks )
{
	return PUT( m_vecBuffBehaviorFactors )
		&& PUT( m_vecBuffFinalizerFactors )
		&& PUT( m_BuffIdentity )
		&& PUT( m_fAccumulationMultiplier )
		&& PUT( m_ucAccumulationCountNow )
		&& PUT( m_iFactorID )
		;
}

SERIALIZE_DEFINE_GET( KBuffFactor, obj, ks )
{
	return GET( m_vecBuffBehaviorFactors )
		&& GET( m_vecBuffFinalizerFactors )
		&& GET( m_BuffIdentity )
		&& GET( m_fAccumulationMultiplier )
		&& GET( m_ucAccumulationCountNow )
		&& GET( m_iFactorID )
		;
}


#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 04. 24	최육사	UDP릴레이 퍼포먼스 체크
#ifdef SERV_UDP_RELAY_CHECKER
SERIALIZE_DEFINE_PUT( KE_CONNECT_RELAY_ACK, obj, ks )
{
	return PUT( m_wstrRelayServerIP )
		;
}

SERIALIZE_DEFINE_GET( KE_CONNECT_RELAY_ACK, obj, ks )
{
	return GET( m_wstrRelayServerIP )
		;
}

SERIALIZE_DEFINE_PUT( KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT, obj, ks )
{
	return PUT( m_wstrRelayServerIP )
		&& PUT( m_dwEndTickCount )
		&& PUT( m_dwEndTimeGetTime )
		;
}

SERIALIZE_DEFINE_GET( KE_UDP_RELAY_SERVER_CHECK_PACKET_NOT, obj, ks )
{
	return GET( m_wstrRelayServerIP )
		&& GET( m_dwEndTickCount )
		&& GET( m_dwEndTimeGetTime )
		;
}

SERIALIZE_DEFINE_PUT( KUdpRelayCheckLog, obj, ks )
{
	return PUT( m_iServerUID )
		&& PUT( m_wstrServerIP )
		&& PUT( m_dwAvgDelayTime )
		&& PUT( m_dwMaxDelayTime )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KUdpRelayCheckLog, obj, ks )
{
	return GET( m_iServerUID )
		&& GET( m_wstrServerIP )
		&& GET( m_dwAvgDelayTime )
		&& GET( m_dwMaxDelayTime )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_UDP_RELAY_CHECKER
//}}

//{{ 2012. 05. 6	박세훈	어둠의 문 개편
#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
SERIALIZE_DEFINE_PUT( KRecordBuffInfo, obj, ks )
{
	return PUT( m_iBuffID )
		&& PUT( m_wstrStartTime )
		;
}

SERIALIZE_DEFINE_GET( KRecordBuffInfo, obj, ks )
{
	return GET( m_iBuffID )
		&& GET( m_wstrStartTime )
		;
}
#endif SERV_REFORM_THE_GATE_OF_DARKNESS
//}}
//{{ 2012. 05. 17	박세훈	키보드 맵핑 정보를 DB에서 읽고 쓰기
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
SERIALIZE_DEFINE_PUT( KKeyboardMappingInfo, obj, ks )
{
	return PUT( m_mapKeyboardMappingInfo )
#ifdef SERV_KEY_MAPPING_INT
		&& PUT( m_mapGamePadMappingInfo )
#endif //SERV_KEY_MAPPING_INT
		;
}

SERIALIZE_DEFINE_GET( KKeyboardMappingInfo, obj, ks )
{
	return GET( m_mapKeyboardMappingInfo )
#ifdef SERV_KEY_MAPPING_INT
		&& GET( m_mapGamePadMappingInfo )
#endif //SERV_KEY_MAPPING_INT
		;
}
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}
//{{ 2012. 06. 13	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
SERIALIZE_DEFINE_PUT( KDailyGiftBoxInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iState )
		&& PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KDailyGiftBoxInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iState )
		&& GET( m_wstrRegDate )
		;
}
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

//{{ 2012. 07. 24	박세훈	해당 캐릭터의 모든 스킬을 다 찍는 치트
#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL
SERIALIZE_DEFINE_PUT( KAdminCheatSkill, obj, ks )
{
	return PUT( m_iSkillLevel )
		&& PUT( m_iSkillCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KAdminCheatSkill, obj, ks )
{
	return GET( m_iSkillLevel )
		&& GET( m_iSkillCSPoint )
		;
}

#ifdef SERV_SKILL_PAGE_SYSTEM
SERIALIZE_DEFINE_PUT( KEGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ, obj, ks )
{
	return PUT( m_iActiveSkillPageNumber )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_CHEAT_GET_ALL_SKILL_REQ, obj, ks )
{
	return GET( m_iActiveSkillPageNumber )
		;
}
#endif // SERV_SKILL_PAGE_SYSTEM

SERIALIZE_DEFINE_PUT( KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, obj, ks )
{
	return PUT( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& PUT( m_vecUnSealedSkill )
		&& PUT( m_mapSkillInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_CHEAT_GET_ALL_SKILL_ACK, obj, ks )
{
	return GET( m_iOK )
#ifdef SERV_SKILL_PAGE_SYSTEM
		&& GET( m_iActiveSkillPageNumber )
#endif // SERV_SKILL_PAGE_SYSTEM
		&& GET( m_vecUnSealedSkill )
		&& GET( m_mapSkillInfo )
		;
}
#endif SERV_ADMIN_CHEAT_GET_ALL_SKILL
//}}

//{{ 2012. 10. 16	최육사		P2P & Relay 정보 수집
#ifdef SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
SERIALIZE_DEFINE_PUT( KGamePlayNetworkInfo, obj, ks )
{
	return PUT( m_iOpponentUnitUID )
		&& PUT( m_iHolePunchingTryCount )
		&& PUT( m_fP2PPlayTime )
		&& PUT( m_fRelayPlayTime )
		;
}

SERIALIZE_DEFINE_GET( KGamePlayNetworkInfo, obj, ks )
{
	return GET( m_iOpponentUnitUID )
		&& GET( m_iHolePunchingTryCount )
		&& GET( m_fP2PPlayTime )
		&& GET( m_fRelayPlayTime )
		;
}


SERIALIZE_DEFINE_PUT( KGamePlayNetworkInfoPackage, obj, ks )
{
	return PUT( m_vecGamePlayNetworkInfo )
		;
}

SERIALIZE_DEFINE_GET( KGamePlayNetworkInfoPackage, obj, ks )
{
	return GET( m_vecGamePlayNetworkInfo )
		;
}
#endif SERV_COLLECTION_OF_RELAY_AND_P2P_INFO
//}}

//{{ 2012. 10. 31	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
SERIALIZE_DEFINE_PUT( KRegUnitInfo, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrUnitNickName )
		&& PUT( m_iGSUID )
		&& PUT( m_iChannelID )
		&& PUT( m_wstrIP )
		&& PUT( m_cAuthLevel )
		//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
		&& PUT( m_iChannelCode )
#endif SERV_EVENT_JACKPOT
		//}}
		;
}

SERIALIZE_DEFINE_GET( KRegUnitInfo, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrUnitNickName )
		&& GET( m_iGSUID )
		&& GET( m_iChannelID )
		&& GET( m_wstrIP )
		&& GET( m_cAuthLevel )
		//{{ 2012. 12. 17	박세훈	잭팟 이벤트
#ifdef SERV_EVENT_JACKPOT
		&& GET( m_iChannelCode )
#endif SERV_EVENT_JACKPOT
		//}}
		;
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2012. 08. 14	박세훈	대천사의 주화 이벤트 가이드 문구 출력
#ifdef SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
SERIALIZE_DEFINE_PUT( KDBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bReceiveTheLetter )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_THE_ARCHUANGEL_S_COIN_EVENT_LETTER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bReceiveTheLetter )
		;
}
#endif SERV_ARCHUANGEL_S_COIN_EVENT_GUIDE
//}}

//{{ 2012. 08. 21	박세훈	우편 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_LETTER
SERIALIZE_DEFINE_PUT( KEGS_GET_ITEM_FROM_LETTER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPostNo )
		&& PUT( m_bSystemLetter )
		&& PUT( m_mapResultItem )
		&& PUT( m_vecKInventorySlotInfo )
		&& PUT( m_iED )
		&& PUT( m_iUnitUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecMoveItemSlotInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_ITEM_FROM_LETTER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPostNo )
		&& GET( m_bSystemLetter )
		&& GET( m_mapResultItem )
		&& GET( m_vecKInventorySlotInfo )
		&& GET( m_iED )
		&& GET( m_iUnitUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecMoveItemSlotInfo )
		;
}
#endif SERV_TRADE_LOGIC_CHANGE_LETTER
//}}

//{{ 2012. 12. 17	박세훈	수동 파티 시 던전 팝업 창 문구 오류 수정
#ifdef SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
SERIALIZE_DEFINE_PUT( KEGS_PARTY_GAME_START_OPINION_CHECK_NOT, obj, ks )
{
	return PUT( m_iDungeonID )
		&& PUT( m_cDifficultyLevel )
		&& PUT( m_cDungeonMode )
		;
}

SERIALIZE_DEFINE_GET( KEGS_PARTY_GAME_START_OPINION_CHECK_NOT, obj, ks )
{
	return GET( m_iDungeonID )
		&& GET( m_cDifficultyLevel )
		&& GET( m_cDungeonMode )
		;
}
#endif SERV_FIX_MANUAL_PARTY_DUNGEON_POPUP_BUG
//}}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGS_ITEM_EXCHANGE_REQ, obj, ks )
{
	return PUT( m_iHouseID )		
		//{{ 2011. 08. 16	최육사	아이템 교환 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
		&& PUT( m_iSourceItemUID )
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
		&& PUT( m_iSourceItemID )
		//{{ 2011. 08. 16	최육사	아이템 교환 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
		&& PUT( m_iSourceQuantity )
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
		&& PUT( m_iDestItemID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ITEM_EXCHANGE_REQ, obj, ks )
{
	return GET( m_iHouseID )		
		//{{ 2011. 08. 16	최육사	아이템 교환 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
		&& GET( m_iSourceItemUID )
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
		&& GET( m_iSourceItemID )
		//{{ 2011. 08. 16	최육사	아이템 교환 개편
#ifdef SERV_ITEM_EXCHANGE_NEW
		&& GET( m_iSourceQuantity )
#endif SERV_ITEM_EXCHANGE_NEW
		//}}
		&& GET( m_iDestItemID )
		;
}


SERIALIZE_DEFINE_PUT( KEGS_ITEM_EXCHANGE_ACK, obj, ks )
{
	return PUT( m_iOK )		
		&& PUT( m_vecKInventorySlotInfo )
		//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
		&& PUT( m_mapResultItem )
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
		//}}
#ifdef SERV_GROW_UP_SOCKET
		&& PUT( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
		;
}

SERIALIZE_DEFINE_GET( KEGS_ITEM_EXCHANGE_ACK, obj, ks )
{
	return GET( m_iOK )		
		&& GET( m_vecKInventorySlotInfo )
		//{{ 2012. 03. 05	김민성	랜덤교환
#ifdef SERV_RANDOM_EXCHANGE_RESULT_VIEW
		&& GET( m_mapResultItem )
#endif SERV_RANDOM_EXCHANGE_RESULT_VIEW
		//}}
#ifdef SERV_GROW_UP_SOCKET
		&& GET( m_iExchangeCount )
#endif SERV_GROW_UP_SOCKET
		;
}

//{{ 2013. 02. 20	최육사	로그 시스템 개선
#ifdef SERV_LOG_SYSTEM_NEW
SERIALIZE_DEFINE_PUT( KLogStatInfo, obj, ks )
{
	return PUT( m_wstrFunctionName )
		&& PUT( m_wstrFileName )
		&& PUT( m_wstrLineNum )
		&& PUT( m_iCount )
		;
}

SERIALIZE_DEFINE_GET( KLogStatInfo, obj, ks )
{
	return GET( m_wstrFunctionName )
		&& GET( m_wstrFileName )
		&& GET( m_wstrLineNum )
		&& GET( m_iCount )
		;
}
#endif SERV_LOG_SYSTEM_NEW
//}}

//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
SERIALIZE_DEFINE_PUT( KFirstSelectUnitReward, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_cRewardType )
		&& PUT( m_cGiveType )
		&& PUT( m_iItemID )
		&& PUT( m_iQuantity )
		&& PUT( m_iDBIndexID )
		;
}

SERIALIZE_DEFINE_GET( KFirstSelectUnitReward, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_cRewardType )
		&& GET( m_cGiveType )
		&& GET( m_iItemID )
		&& GET( m_iQuantity )
		&& GET( m_iDBIndexID )
		;
}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
SERIALIZE_DEFINE_PUT( KLocalRankingUserInfo, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iCategory )
		&& PUT( m_iBirth )
		&& PUT( m_wstrProfile )
		&& PUT( m_byteFilter )
		&& PUT( m_byteGender )
		;
}

SERIALIZE_DEFINE_GET( KLocalRankingUserInfo, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iCategory )
		&& GET( m_iBirth )
		&& GET( m_wstrProfile )
		&& GET( m_byteFilter )
		&& GET( m_byteGender )
		;
}

SERIALIZE_DEFINE_PUT( KLocalRankingUnitInfo, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_ucLevel )
		&& PUT( m_cUnitClass )
		&& PUT( m_iPoint[0] )
		&& PUT( m_iPoint[1] )
		;
}

SERIALIZE_DEFINE_GET( KLocalRankingUnitInfo, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_ucLevel )
		&& GET( m_cUnitClass )
		&& GET( m_iPoint[0] )
		&& GET( m_iPoint[1] )
		;
}

SERIALIZE_DEFINE_PUT( KLocalRankingUserInfoCounter, obj, ks )
{
	return PUT( m_kInfo )
		&& PUT( m_iCounter )
		;
}

SERIALIZE_DEFINE_GET( KLocalRankingUserInfoCounter, obj, ks )
{
	return GET( m_kInfo )
		&& GET( m_iCounter )
		;
}

SERIALIZE_DEFINE_PUT( KLocalRankingUnitInfoCounter, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iCounter )
		&& PUT( m_kInfo )
		;
}

SERIALIZE_DEFINE_GET( KLocalRankingUnitInfoCounter, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iCounter )
		&& GET( m_kInfo )
		;
}

SERIALIZE_DEFINE_PUT( KLocalRankingRankerInfo, obj, ks )
{
	return PUT( m_iRank )
		&& PUT( m_kUserInfo )
		&& PUT( m_kUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KLocalRankingRankerInfo, obj, ks )
{
	return GET( m_iRank )
		&& GET( m_kUserInfo )
		&& GET( m_kUnitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KLocalRankingInquiryData, obj, ks )
{
	return PUT( m_nViewPage )
		&& PUT( m_cMainTabIndex )
		&& PUT( m_cSubTabIndex )
		&& PUT( m_byteFilter )
		&& PUT( m_kMyRankInfo )
		&& PUT( m_setFriendList )
		&& PUT( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_GET( KLocalRankingInquiryData, obj, ks )
{
	return GET( m_nViewPage )
		&& GET( m_cMainTabIndex )
		&& GET( m_cSubTabIndex )
		&& GET( m_byteFilter )
		&& GET( m_kMyRankInfo )
		&& GET( m_setFriendList )
		&& GET( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_LOCAL_RANKING_RANK_UP_NOT, obj, ks )
{
	return PUT( m_iMyRank )
		&& PUT( m_cMainTabIndex )
		&& PUT( m_cSubTabIndex )
		&& PUT( m_kMyUnitInfo )
		&& PUT( m_kNextUnitInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_LOCAL_RANKING_RANK_UP_NOT, obj, ks )
{
	return GET( m_iMyRank )
		&& GET( m_cMainTabIndex )
		&& GET( m_cSubTabIndex )
		&& GET( m_kMyUnitInfo )
		&& GET( m_kNextUnitInfo )
		;
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK
SERIALIZE_DEFINE_PUT( KVillageLagInfo, obj, ks )
{
	return PUT( m_iLagType )
		&& PUT( m_iPartyType )
		&& PUT( m_iPartyUserNum )
		&& PUT( m_iMapID )
		&& PUT( m_iVillageUserNum )
		&& PUT( m_fLagTime )
		&& PUT( m_fPlayTime )
		&& PUT( m_iLagTotalNum )
		&& PUT( m_fLagTotalTime )
		;
}

SERIALIZE_DEFINE_GET( KVillageLagInfo, obj, ks )
{
	return GET( m_iLagType )
		&& GET( m_iPartyType )
		&& GET( m_iPartyUserNum )
		&& GET( m_iMapID )
		&& GET( m_iVillageUserNum )
		&& GET( m_fLagTime )
		&& GET( m_fPlayTime )
		&& GET( m_iLagTotalNum )
		&& GET( m_fLagTotalTime )
		;
}

SERIALIZE_DEFINE_PUT( KFieldLagInfo, obj, ks )
{
	return PUT( m_iLagType )
		&& PUT( m_iPartyType )
		&& PUT( m_iPartyUserNum )
		&& PUT( m_iMapID )
		&& PUT( m_iFieldUserNum )
		&& PUT( m_fLagTime )
		&& PUT( m_fPlayTime )
		&& PUT( m_iLagTotalNum )
		&& PUT( m_fLagTotalTime )
		;
}

SERIALIZE_DEFINE_GET( KFieldLagInfo, obj, ks )
{
	return GET( m_iLagType )
		&& GET( m_iPartyType )
		&& GET( m_iPartyUserNum )
		&& GET( m_iMapID )
		&& GET( m_iFieldUserNum )
		&& GET( m_fLagTime )
		&& GET( m_fPlayTime )
		&& GET( m_iLagTotalNum )
		&& GET( m_fLagTotalTime )
		;
}

SERIALIZE_DEFINE_PUT( KDungeonLagInfo, obj, ks )
{
	return PUT( m_iLagType )
		&& PUT( m_iPartyType )
		&& PUT( m_iPartyUserNum )
		&& PUT( m_iMapID )
		&& PUT( m_iDifficulty )
		&& PUT( m_iStage )
		&& PUT( m_iBlock )
		&& PUT( m_fLagTime )
		&& PUT( m_fPlayTime )
		&& PUT( m_iLagTotalNum )
		&& PUT( m_fLagTotalTime )
		;
}

SERIALIZE_DEFINE_GET( KDungeonLagInfo, obj, ks )
{
	return GET( m_iLagType )
		&& GET( m_iPartyType )
		&& GET( m_iPartyUserNum )
		&& GET( m_iMapID )
		&& GET( m_iDifficulty )
		&& GET( m_iStage )
		&& GET( m_iBlock )
		&& GET( m_fLagTime )
		&& GET( m_fPlayTime )
		&& GET( m_iLagTotalNum )
		&& GET( m_fLagTotalTime )
		;
}

SERIALIZE_DEFINE_PUT( KPvPLagInfo, obj, ks )
{
	return PUT( m_iLagType )
		&& PUT( m_iMapID )
		&& PUT( m_iPvPUserNum )
		&& PUT( m_iRuleType )
		&& PUT( m_iRound )
		&& PUT( m_fLagTime )
		&& PUT( m_fPlayTime )
		&& PUT( m_iLagTotalNum )
		&& PUT( m_fLagTotalTime )
		;
}

SERIALIZE_DEFINE_GET( KPvPLagInfo, obj, ks )
{
	return GET( m_iLagType )
		&& GET( m_iMapID )
		&& GET( m_iPvPUserNum )
		&& GET( m_iRuleType )
		&& GET( m_iRound )
		&& GET( m_fLagTime )
		&& GET( m_fPlayTime )
		&& GET( m_iLagTotalNum )
		&& GET( m_fLagTotalTime )
		;
}
#endif//SERV_ACTIVE_KOG_GAME_PERFORMANCE_CHECK

//{{ 2013. 06. 04	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
SERIALIZE_DEFINE_PUT( KStatRelationLevel, obj, ks )
{
	return PUT( m_fBaseHPRelLV )
		&& PUT( m_fAtkPhysicRelLV )
		&& PUT( m_fAtkMagicRelLV )
		&& PUT( m_fDefPhysicRelLV )
		&& PUT( m_fDefMagicRelLV )
		;
}

SERIALIZE_DEFINE_GET( KStatRelationLevel, obj, ks )
{
	return GET( m_fBaseHPRelLV )
		&& GET( m_fAtkPhysicRelLV )
		&& GET( m_fAtkMagicRelLV )
		&& GET( m_fDefPhysicRelLV )
		&& GET( m_fDefMagicRelLV )
		;
}
#endif SERV_NEW_ITEM_SYSTEM_2013_05
//}}

#ifdef	SERV_HERO_PVP_MANAGE_LIST // 적용날짜: 2013-04-04
SERIALIZE_DEFINE_PUT( KHeroPvpUserInfo, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_wstrNickName )
		&& PUT( m_bOn )
		&& PUT( m_bConnect )
		;
}

SERIALIZE_DEFINE_GET( KHeroPvpUserInfo, obj, ks )
{
	return GET( m_iServerGroupID )
		&& GET( m_wstrNickName )
		&& GET( m_bOn )
		&& GET( m_bConnect )
		;
}
#endif	// SERV_HERO_PVP_MANAGE_LIST

#ifdef	SERV_RIDING_PET_SYSTM// 적용날짜: 2013-04-20
SERIALIZE_DEFINE_PUT( KRidingPetInfo, obj, ks )
{
	return PUT( m_iRidingPetUID )
		&& PUT( m_usRindingPetID )
		&& PUT( m_fStamina )
		&& PUT( m_wstrDestroyDate )
		&& PUT( m_iLastUnSummonDate )
		;
}

SERIALIZE_DEFINE_GET( KRidingPetInfo, obj, ks )
{
	return GET( m_iRidingPetUID )
		&& GET( m_usRindingPetID )
		&& GET( m_fStamina )
		&& GET( m_wstrDestroyDate )
		&& GET( m_iLastUnSummonDate )
		;
}
#endif	// SERV_RIDING_PET_SYSTM

#ifdef SERV_UPGRADE_SKILL_SYSTEM_2013 // 적용날짜: 2013-06-27
SERIALIZE_DEFINE_PUT( KGetSkillInfo, obj, ks )
{
	return PUT( m_iSkillID )
		&& PUT( m_iBeforeSkillLevel )
		&& PUT( m_iSkillLevel )
		&& PUT( m_iSpendSkillCSPoint )
		;
}

SERIALIZE_DEFINE_GET( KGetSkillInfo, obj, ks )
{
	return GET( m_iSkillID )
		&& GET( m_iBeforeSkillLevel )
		&& GET( m_iSkillLevel )
		&& GET( m_iSpendSkillCSPoint )
		;
}
#endif	// SERV_UPGRADE_SKILL_SYSTEM_2013

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-18	// 박세훈
SERIALIZE_DEFINE_PUT( KCouponBoxTargetItem, obj, ks )
{
	return PUT( m_iItemQuantity )
		&& PUT( m_sItemUseDuration )
		&& PUT( m_wstrItemCode )
		&& PUT( m_wstrItemName )
		;
}

SERIALIZE_DEFINE_GET( KCouponBoxTargetItem, obj, ks )
{
	return GET( m_iItemQuantity )
		&& GET( m_sItemUseDuration )
		&& GET( m_wstrItemCode )
		&& GET( m_wstrItemName )
		;
}

SERIALIZE_DEFINE_PUT( KClientCouponBox, obj, ks )
{
	return PUT( m_wstrContractDetailName )
		&& PUT( m_iContractDetailNo )
		&& PUT( m_wstrCouponBoxType )
		&& PUT( m_wstrCouponBoxTypeText )
		&& PUT( m_iCouponCardNo )
		&& PUT( m_iCouponCardStatus )
		&& PUT( m_wstrCouponCardStatusText )
		&& PUT( m_wstrDiscountValue )
		&& PUT( m_wstrExpireDateTime )
		&& PUT( m_wstrObtainDateTime )
		&& PUT( m_wstrValidDateRange )
		&& PUT( m_wstrUsedDateTime )
		&& PUT( m_vecTargetItem )
		;
}

SERIALIZE_DEFINE_GET( KClientCouponBox, obj, ks )
{
	return GET( m_wstrContractDetailName )
		&& GET( m_iContractDetailNo )
		&& GET( m_wstrCouponBoxType )
		&& GET( m_wstrCouponBoxTypeText )
		&& GET( m_iCouponCardNo )
		&& GET( m_iCouponCardStatus )
		&& GET( m_wstrCouponCardStatusText )
		&& GET( m_wstrDiscountValue )
		&& GET( m_wstrExpireDateTime )
		&& GET( m_wstrObtainDateTime )
		&& GET( m_wstrValidDateRange )
		&& GET( m_wstrUsedDateTime )
		&& GET( m_vecTargetItem )
		;
}

SERIALIZE_DEFINE_PUT( KDiscountCoupon, obj, ks )
{
	return PUT( m_iCouponCardNo )
		&& PUT( m_wstrContractDetailName )
		&& PUT( m_wstrDiscountValue )
		&& PUT( m_wstrExpireDateTime )
		;
}

SERIALIZE_DEFINE_GET( KDiscountCoupon, obj, ks )
{
	return GET( m_iCouponCardNo )
		&& GET( m_wstrContractDetailName )
		&& GET( m_wstrDiscountValue )
		&& GET( m_wstrExpireDateTime )
		;
}

SERIALIZE_DEFINE_PUT( KDiscountCouponInquriyInfo, obj, ks )
{
	return PUT( m_ulProductNo )
		&& PUT( m_usOrderQuantity )
		;
}

SERIALIZE_DEFINE_GET( KDiscountCouponInquriyInfo, obj, ks )
{
	return GET( m_ulProductNo )
		&& GET( m_usOrderQuantity )
		;
}
#endif // SERV_NEXON_COUPON_SYSTEM

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-08	// 박세훈
SERIALIZE_DEFINE_PUT( KBossFieldCreateInfo, obj, ks )
{
	return PUT( m_tFieldHoldingTime )
		&& PUT( m_tPortalOpenTime )
		&& PUT( m_iBossFieldID )
		&& PUT( m_iPortalMapID )
		&& PUT( m_bBossField )
		;
}

SERIALIZE_DEFINE_GET( KBossFieldCreateInfo, obj, ks )
{
	return GET( m_tFieldHoldingTime )
		&& GET( m_tPortalOpenTime )
		&& GET( m_iBossFieldID )
		&& GET( m_iPortalMapID )
		&& GET( m_bBossField )
		;
}

SERIALIZE_DEFINE_PUT( KBossFieldJoinInfo, obj, ks )
{
	return PUT( m_iReturnMapID )
		&& PUT( m_ucLastTouchLineIndex )
		&& PUT( m_usLastPosValue )
		&& PUT( m_bIgnoreLastTouch )
		;
}

SERIALIZE_DEFINE_GET( KBossFieldJoinInfo, obj, ks )
{
	return GET( m_iReturnMapID )
		&& GET( m_ucLastTouchLineIndex )
		&& GET( m_usLastPosValue )
		&& GET( m_bIgnoreLastTouch )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ, obj, ks )
{
	return PUT( m_iBattleFieldID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_BOSS_FIELD_GATE_OPEN_REQ, obj, ks )
{
	return GET( m_iBattleFieldID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ, obj, ks )
{
	return PUT( m_iBattleFieldID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_REQ, obj, ks )
{
	return GET( m_iBattleFieldID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iBattleFieldID )
		&& PUT( m_byteTotalDangerousValue )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_GET_TOTAL_DANGEROUS_VALUE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iBattleFieldID )
		&& GET( m_byteTotalDangerousValue )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ, obj, ks )
{
	return PUT( m_iBattleFieldID )
		&& PUT( m_iTotalDangerousValue )
		&& PUT( m_bSet )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_SET_TOTAL_DANGEROUS_VALUE_REQ, obj, ks )
{
	return GET( m_iBattleFieldID )
		&& GET( m_iTotalDangerousValue )
		&& GET( m_bSet )
		;
}

SERIALIZE_DEFINE_PUT( KBossFieldLog, obj, ks )
{
	return PUT( m_iBossFieldID )
		&& PUT( m_iPortalMapID )
		&& PUT( m_iPlayTime )
		&& PUT( m_byteRemainUserCount )
		&& PUT( m_bSucceed )
		&& PUT( m_wstrPortalOpenTime )
		;
}

SERIALIZE_DEFINE_GET( KBossFieldLog, obj, ks )
{
	return GET( m_iBossFieldID )
		&& GET( m_iPortalMapID )
		&& GET( m_iPlayTime )
		&& GET( m_byteRemainUserCount )
		&& GET( m_bSucceed )
		&& GET( m_wstrPortalOpenTime )
		;
}

SERIALIZE_DEFINE_PUT( KBossFieldUserLog, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iGivenDamage )
		&& PUT( m_iAttackDamage )
		&& PUT( m_iEXP )
		&& PUT( m_iED )
		&& PUT( m_sNumResurrectionStone )
		&& PUT( m_byteLevel )
		&& PUT( m_byteClass )
		&& PUT( m_byteCompletionType )
		&& PUT( m_byteContributionRank )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KBossFieldUserLog, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iGivenDamage )
		&& GET( m_iAttackDamage )
		&& GET( m_iEXP )
		&& GET( m_iED )
		&& GET( m_sNumResurrectionStone )
		&& GET( m_byteLevel )
		&& GET( m_byteClass )
		&& GET( m_byteCompletionType )
		&& GET( m_byteContributionRank )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_BOSS_FIELD_LOG_NOT, obj, ks )
{
	return PUT( m_mapGivenDamage )
		&& PUT( m_mapAttackDamage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_BOSS_FIELD_LOG_NOT, obj, ks )
{
	return GET( m_mapGivenDamage )
		&& GET( m_mapAttackDamage )
		;
}
#endif // SERV_BATTLE_FIELD_BOSS


#ifdef SERV_4TH_ANNIVERSARY_EVENT
SERIALIZE_DEFINE_PUT( K4ThAnnivEventInfo, obj, ks )
{
	return PUT( m_tTimeFirstPlay )
		&& PUT( m_tTimeFirstPet )
		&& PUT( m_tTimeFirstHenir )
		&& PUT( m_tTimeFirstDeleteChar )
		&& PUT( m_iItemIDFirstBuy )
		&& PUT( m_iLongestConnectTime )
		&& PUT( m_iCountQuestComplete )
		&& PUT( m_iCountReceivedPost )
		&& PUT( m_iDayTotalConnect )
		&& PUT( m_iCountPvpLose )
		&& PUT( m_iCountResurrect )
		&& PUT( m_iCountDungeonClear )
		;
}

SERIALIZE_DEFINE_GET( K4ThAnnivEventInfo, obj, ks )
{
	return GET( m_tTimeFirstPlay )
		&& GET( m_tTimeFirstPet )
		&& GET( m_tTimeFirstHenir )
		&& GET( m_tTimeFirstDeleteChar )
		&& GET( m_iItemIDFirstBuy )
		&& GET( m_iLongestConnectTime )
		&& GET( m_iCountQuestComplete )
		&& GET( m_iCountReceivedPost )
		&& GET( m_iDayTotalConnect )
		&& GET( m_iCountPvpLose )
		&& GET( m_iCountResurrect )
		&& GET( m_iCountDungeonClear )
		;
}
#endif // SERV_4TH_ANNIVERSARY_EVENT