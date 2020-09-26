#include <winsock2.h>
#include "CommonPacket.h"



//////////////////////////////////////////////////////////////////////////
#ifdef SERV_GLOBAL_BILLING

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
SERIALIZE_DEFINE_PUT( KGlobalCashInfo, obj, ks )
{
	return PUT( m_ulCash[0] )
		&& PUT( m_ulCash[1] )
		&& PUT( m_ulCash[2] )
		&& PUT( m_ulCash[3] )
		//&& PUT( m_ulCash[4] ) //GCT_PUBLISHER_BONUS_CASH 사용 시 주석 해제
		;
}

SERIALIZE_DEFINE_GET( KGlobalCashInfo, obj, ks )
{
	return GET( m_ulCash[0] )
		&& GET( m_ulCash[1] )
		&& GET( m_ulCash[2] )
		&& GET( m_ulCash[3] )
		//&& GET( m_ulCash[4] ) //GCT_PUBLISHER_BONUS_CASH 사용 시 주석 해제
		;
}
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES

SERIALIZE_DEFINE_PUT( KBillProductCategory, obj, ks )
{
	return PUT( m_cCategoryNo )
		&& PUT( m_wstrCategoryName )
		&& PUT( m_cParentCategoryNo )
		&& PUT( m_cDisplayNo );
}

SERIALIZE_DEFINE_GET( KBillProductCategory, obj, ks )
{
	return GET( m_cCategoryNo )
		&& GET( m_wstrCategoryName )
		&& GET( m_cParentCategoryNo )
		&& GET( m_cDisplayNo );
}

SERIALIZE_DEFINE_PUT( KBillProductInfo, obj, ks )
{
	return PUT( m_iProductNo )
		&& PUT( m_iProductID )
		&& PUT( m_bSale )
		&& PUT( m_bEvent )
		&& PUT( m_bShow )
		&& PUT( m_bIsCashInvenSkip )
		&& PUT( m_wstrProductName )
		&& PUT( m_cCategoryNo )
		&& PUT( m_iSalePrice )
		&& PUT( m_iRealPrice )
		&& PUT( m_cPeriod )
		&& PUT( m_cQuantity )
		&& PUT( m_bEnableGift )
		&& PUT( m_cLimitLevel )
		&& PUT( m_wstrEndDate )
#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
		&& PUT( m_setSalePeriod )
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
		;
}

SERIALIZE_DEFINE_GET( KBillProductInfo, obj, ks )
{
	return GET( m_iProductNo )
		&& GET( m_iProductID )
		&& GET( m_bSale )
		&& GET( m_bEvent )
		&& GET( m_bShow )
		&& GET( m_bIsCashInvenSkip )
		&& GET( m_wstrProductName )
		&& GET( m_cCategoryNo )
		&& GET( m_iSalePrice )
		&& GET( m_iRealPrice )
		&& GET( m_cPeriod )
		&& GET( m_cQuantity )
		&& GET( m_bEnableGift )
		&& GET( m_cLimitLevel )
		&& GET( m_wstrEndDate )
#ifdef SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
		&& GET( m_setSalePeriod )
#endif SERV_REAL_TIME_SALE_PERIOD_DESCRIPTION
		;
}

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

SERIALIZE_DEFINE_PUT( KBillPackageInfo, obj, ks )
{
	return PUT( m_iPackageNo )
		&& PUT( m_vecProductNo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& PUT( m_ulProductNo )
		&& PUT( m_iProductID )
		&& PUT( m_wstrProductID )
		&& PUT( m_sProductExpire )
		&& PUT( m_usProductPieces )
		&& PUT( m_vecSocketOption )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KBillPackageInfo, obj, ks )
{
	return GET( m_iPackageNo )
		&& GET( m_vecProductNo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& GET( m_ulProductNo )
		&& GET( m_iProductID )
		&& GET( m_wstrProductID )
		&& GET( m_sProductExpire )
		&& GET( m_usProductPieces )
		&& GET( m_vecSocketOption )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_PUT( KBillBuyInfo, obj, ks )
{
	return PUT( m_iProductNo )
		&& PUT( m_iPoint )
		&& PUT( m_iProductID )
		&& PUT( m_usOrderQuantity )
		&& PUT( m_iCategory )
		&& PUT( m_wstrMemo )
		&& PUT( m_wstrOrderID )
		;
}

SERIALIZE_DEFINE_GET( KBillBuyInfo, obj, ks )
{
	return GET( m_iProductNo )
		&& GET( m_iPoint )
		&& GET( m_iProductID )
		&& GET( m_usOrderQuantity )
		&& GET( m_iCategory )
		&& GET( m_wstrMemo )
		&& GET( m_wstrOrderID )
		;
}

SERIALIZE_DEFINE_PUT( KBillOrderInfo, obj, ks )
{
	return  PUT( m_iTransNo ) &&
		PUT( m_wstrOrderID ) &&
		PUT( m_kBillProductInfo ) &&
		PUT( m_vecSocketOption ) &&
		PUT( m_byteType ) &&
		PUT( m_wstrSenderNickName ) &&
		PUT( m_wstrSenderPresentMessage )
		//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
		&& PUT( m_iFromUnitUID )
		//}} 허상형 : [2010/8/19/] //	선물 기능 개편
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& PUT( m_byteProductKind )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KBillOrderInfo, obj, ks )
{
	return  GET( m_iTransNo ) &&
		GET( m_wstrOrderID ) &&
		GET( m_kBillProductInfo ) &&
		GET( m_vecSocketOption ) &&
		GET( m_byteType ) &&
		GET( m_wstrSenderNickName ) &&
		GET( m_wstrSenderPresentMessage )
		//{{ 허상형 : [2010/8/19/] //	선물 기능 개편
		&& GET( m_iFromUnitUID )
		//}} 허상형 : [2010/8/19/] //	선물 기능 개편
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& GET( m_byteProductKind )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
SERIALIZE_DEFINE_PUT( kDisCountItemInfo, obj, ks )
{
	return  PUT( m_DisCountProNo ) &&
		PUT( m_BasicProNo ) &&
		PUT( m_DisCountKeepItem )
		;
}
SERIALIZE_DEFINE_GET( kDisCountItemInfo, obj, ks )
{

	return  GET( m_DisCountProNo ) &&
		GET( m_BasicProNo ) &&
		GET( m_DisCountKeepItem )
		;
}
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#endif SERV_GLOBAL_BILLING
//////////////////////////////////////////////////////////////////////////


  
#ifdef SERV_NEW_PVPROOM_PROCESS

SERIALIZE_DEFINE_PUT( KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ, obj, ks )
{
	return PUT( m_bPublic )
		&& PUT( m_wstrPassword )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CHANGE_PVP_ROOM_PUBLIC_REQ, obj, ks )
{
	return GET( m_bPublic )
		&& GET( m_wstrPassword )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK, obj, ks )
{
	return PUT( m_bPublic )
		&& PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CHANGE_PVP_ROOM_PUBLIC_ACK, obj, ks )
{
	return GET( m_bPublic )
		&& GET( m_iOK )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CHANGE_PVP_ROOM_PUBLIC_NOT, obj, ks )
{
	return PUT( m_bPublic )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CHANGE_PVP_ROOM_PUBLIC_NOT, obj, ks )
{
	return GET( m_bPublic )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CHANGE_PVP_ROOM_NAME_REQ, obj, ks )
{
	return PUT( m_wstrRoomName )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CHANGE_PVP_ROOM_NAME_REQ, obj, ks )
{
	return GET( m_wstrRoomName )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CHANGE_PVP_ROOM_NAME_ACK, obj, ks )
{
	return PUT( m_wstrRoomName )
		&& PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CHANGE_PVP_ROOM_NAME_ACK, obj, ks )
{
	return GET( m_wstrRoomName )
		&& GET( m_iOK )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CHANGE_PVP_ROOM_NAME_NOT, obj, ks )
{
	return PUT( m_wstrRoomName )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CHANGE_PVP_ROOM_NAME_NOT, obj, ks )
{
	return GET( m_wstrRoomName )
		;
}

#endif SERV_NEW_PVPROOM_PROCESS

#ifdef SERV_PC_BANG_TYPE
SERIALIZE_DEFINE_PUT( KPcBangReward, obj, ks )
{
	return PUT( m_iPcBangType )
		&& PUT( m_fEXPRate )
		&& PUT( m_fEDRate )
		&& PUT( m_fVSPointRate )
		&& PUT( m_bSpiritFree )
		&& PUT( m_fPetEXPRate )
		&& PUT( m_iAdditionalHenirReward )
		&& PUT( m_bSecretDungeonFree )
		&& PUT( m_bAvatar )
		&& PUT( m_bTitle )
		;
}

SERIALIZE_DEFINE_GET( KPcBangReward, obj, ks )
{
	return GET( m_iPcBangType )
		&& GET( m_fEXPRate )
		&& GET( m_fEDRate )
		&& GET( m_fVSPointRate )
		&& GET( m_bSpiritFree )
		&& GET( m_fPetEXPRate )
		&& GET( m_iAdditionalHenirReward )
		&& GET( m_bSecretDungeonFree )
		&& GET( m_bAvatar )
		&& GET( m_bTitle )
		;
}
#endif SERV_PC_BANG_TYPE

//{{ 2011. 12. 09	박교현		서버군 리스트
#ifdef SERVER_GROUP_UI_ADVANCED
SERIALIZE_DEFINE_PUT( KServerGroupInfo, obj, ks )
{
	return PUT( m_iOrder )
		&& PUT( m_iServerGroupUID )
		&& PUT( m_wstrServerGroupName )
		&& PUT( m_kServerIP )
		&& PUT( m_usMasterPort )
		&& PUT( m_usNCUDPPort )
		&& PUT( m_bEnable )
		;
}

SERIALIZE_DEFINE_GET( KServerGroupInfo, obj, ks )
{
	return GET( m_iOrder )
		&& GET( m_iServerGroupUID )
		&& GET( m_wstrServerGroupName )
		&& GET( m_kServerIP )
		&& GET( m_usMasterPort )
		&& GET( m_usNCUDPPort )
		&& GET( m_bEnable )
		;
}
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_CATCH_HACKUSER_INFO
SERIALIZE_DEFINE_PUT( KEGS_CATCH_HACKUSER_INFO_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iCrashType )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CATCH_HACKUSER_INFO_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iCrashType )
		;
}
#endif SERV_CATCH_HACKUSER_INFO

//{{ 2013. 05. 14	최육사	아이템 개편
#ifdef SERV_BLOCK_LIST
SERIALIZE_DEFINE_PUT( KBlockInfo, obj, ks )
{
	return PUT( m_iBlockUID )
		&& PUT( m_cBlockType )
		&& PUT( m_iBlockTarget )
		&& PUT( m_wstrBlockTarget )
		&& PUT( m_wstrStartDate )
		&& PUT( m_wstrEndDate )
		;
}

SERIALIZE_DEFINE_GET( KBlockInfo, obj, ks )
{
	return GET( m_iBlockUID )
		&& GET( m_cBlockType )
		&& GET( m_iBlockTarget )
		&& GET( m_wstrBlockTarget )
		&& GET( m_wstrStartDate )
		&& GET( m_wstrEndDate )
		;
}
#endif SERV_BLOCK_LIST
//}}

#ifdef	SERV_SHARING_BANK_TEST
SERIALIZE_DEFINE_PUT( KTradeShareItemResult, obj, ks )
{
	return  PUT( m_iBeforeItemUID ) 
		&&	PUT( m_iNewItemUID )
		&&	PUT( m_wstrRegDate )
		;
}

SERIALIZE_DEFINE_GET( KTradeShareItemResult, obj, ks )
{
	return  GET( m_iBeforeItemUID ) 
		&&	GET( m_iNewItemUID )
		&&	GET( m_wstrRegDate )
		;
}
#endif	SERV_SHARING_BANK_TEST

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
SERIALIZE_DEFINE_PUT( KRandomItemResult, obj, ks )
{
	return PUT( m_iItemCount )
		&& PUT( m_bIsSealItem )
		&& PUT( m_dRate )
		&& PUT( m_dItemTotalCount )
		&& PUT( m_bIsNotifyMsgAttration )
		;
}

SERIALIZE_DEFINE_GET( KRandomItemResult, obj, ks )
{
	return GET( m_iItemCount )
		&& GET( m_bIsSealItem )
		&& GET( m_dRate )
		&& GET( m_dItemTotalCount )
		&& GET( m_bIsNotifyMsgAttration )
		;
}
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef SERV_ID_NETMARBLE_PCBANG
SERIALIZE_DEFINE_PUT( KECH_PCBANG_IP_AND_MAC_INFO_NOT, obj, ks )
{
	return PUT( m_wstrPublicIP )
		&& PUT( m_wstrMacInfo )
		;
}

SERIALIZE_DEFINE_GET( KECH_PCBANG_IP_AND_MAC_INFO_NOT, obj, ks )
{
	return GET( m_wstrPublicIP )
		&& GET( m_wstrMacInfo )
		;
}
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_COUNTRY_PH
SERIALIZE_DEFINE_PUT( kGarenaUserTokInfo_t, obj, ks )
{
	return PUT( m_Uid )
		&& PUT( m_Username )
		&& PUT( m_Timestamp )
		&& PUT( m_GcaType )
		&& PUT( m_GoldMemberType )
		&& PUT( m_IpBonus )
		&& PUT( m_Reserved )
		;
}

SERIALIZE_DEFINE_GET( kGarenaUserTokInfo_t, obj, ks )
{
	return GET( m_Uid )
		&& GET( m_Username )
		&& GET( m_Timestamp )
		&& GET( m_GcaType )
		&& GET( m_GoldMemberType )
		&& GET( m_IpBonus )
		&& GET( m_Reserved )
		;
}

#endif //SERV_COUNTRY_PH



//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
//#ifdef SERV_RELAY_DB_CONNECTION
SERIALIZE_DEFINE_PUT( KJPEventRewardInfo, obj, ks )
{
	return PUT( m_iRID )
		&& PUT( m_wstrUserID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iRewardID )
		;
}

SERIALIZE_DEFINE_GET( KJPEventRewardInfo, obj, ks )
{
	return GET( m_iRID )
		&& GET( m_wstrUserID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iRewardID )
		;
}
//#endif SERV_RELAY_DB_CONNECTION
//}}


#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
SERIALIZE_DEFINE_PUT( KEventCharacterRanking, obj, ks )
{
	return PUT( m_iDungeonCharacter1 )
		&& PUT( m_iDungeonCharacter2 )
		&& PUT( m_iDungeonCharacter3 )
		&& PUT( m_iDungeonCharacter4 )
		&& PUT( m_iPVPCharacter1 )
		&& PUT( m_iPVPCharacter2 )
		&& PUT( m_iPVPCharacter3 )
		&& PUT( m_iPVPCharacter4 )
		&& PUT( m_iFieldCharacter1 )
		&& PUT( m_iFieldCharacter2 )
		&& PUT( m_iFieldCharacter3 )
		&& PUT( m_iFieldCharacter4 )
		;
}

SERIALIZE_DEFINE_GET( KEventCharacterRanking, obj, ks )
{
	return GET( m_iDungeonCharacter1 )
		&& GET( m_iDungeonCharacter2 )
		&& GET( m_iDungeonCharacter3 )
		&& GET( m_iDungeonCharacter4 )
		&& GET( m_iPVPCharacter1 )
		&& GET( m_iPVPCharacter2 )
		&& GET( m_iPVPCharacter3 )
		&& GET( m_iPVPCharacter4 )
		&& GET( m_iFieldCharacter1 )
		&& GET( m_iFieldCharacter2 )
		&& GET( m_iFieldCharacter3 )
		&& GET( m_iFieldCharacter4 )
		;
}
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
SERIALIZE_DEFINE_PUT( EVENT_DATA, obj, ks )
{
	return PUT( m_fEXPRate )
		&& PUT( m_fVPRate )
		&& PUT( m_fEDRate )
		&& PUT( m_fRankingPointRate )
		&& PUT( m_iDungeonID )
		&& PUT( m_iDungeonMode )
		&& PUT( m_iUnitLevel )
		&& PUT( m_cUnitType )
		&& PUT( m_cUnitClass )
		&& PUT( m_iPartyNum )
		&& PUT( m_fEventTime )
		&& PUT( m_iEventReward )
		&& PUT( m_mapMinMaxReward )
		&& PUT( m_bGiveOnce )
		&& PUT( m_bDelEndEvent )
		&& PUT( m_bPcBangEvent )
		&& PUT( m_bDirectReward )
		&& PUT( m_bAccountEvent )
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		&& PUT( m_fDropRate )
#else // SERV_DROP_EVENT_RENEWAL
		&& PUT( m_iDropCount )
#endif // SERV_DROP_EVENT_RENEWAL
		&& PUT( m_bWithPlayPcBang )
		&& PUT( m_iChannelCode )
		&& PUT( m_iBF_Team )
		&& PUT( m_iHenirRewardEventCount )
		&& PUT( m_bHenirRewardUnLimited )
		&& PUT( m_iServerGroup )
		&& PUT( m_bAccumulateEXP )
#ifdef DROPEVENT_RENEWAL
		&& PUT( m_iStartLevel )
		&& PUT( m_iEndLevel )
#endif DROPEVENT_RENEWAL
		&& PUT( m_bRepeatEvent )
		&& PUT( m_bRepeatRewardEvent )
		&& PUT( m_iBeforeEventID )		
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		&& PUT( m_bPlayWithCharEXP )
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		&& PUT( m_iEventGroupID )
		&& PUT( m_fQuestItemDropRate )
#ifdef SERV_ADVERTISEMENT_EVENT
		&& PUT( m_wstrAdvertisementURL )
#endif SERV_ADVERTISEMENT_EVENT		
		&& PUT( m_iEventItemID )
		&& PUT( m_iCustomEventID )
#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT		
		&& PUT( m_fDefenseDungeonOpenRate )
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT
		&& PUT( m_byteCriterionUserType )
		&& PUT( m_byteCriterionPresentUserType )
#endif	// SERV_CRITERION_DATE_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		&& PUT( m_bOnlySteamUser )
		&& PUT( m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& PUT( m_bNewUnitEvent )
		&& PUT( m_bNewUnitEvent2 )
		&& PUT( m_bCurrentUnitEvent )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		&& PUT( m_iUnitClassLevel )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
#ifdef SERV_HENIR_REWARD_EVENT // 작업날짜: 2013-09-09	// 박세훈
		&& PUT( m_bAccumulate )
#endif // SERV_HENIR_REWARD_EVENT
		;
}

SERIALIZE_DEFINE_GET( EVENT_DATA, obj, ks )
{
	return GET( m_fEXPRate )
		&& GET( m_fVPRate )
		&& GET( m_fEDRate )
		&& GET( m_fRankingPointRate )
		&& GET( m_iDungeonID )
		&& GET( m_iDungeonMode )
		&& GET( m_iUnitLevel )
		&& GET( m_cUnitType )
		&& GET( m_cUnitClass )
		&& GET( m_iPartyNum )
		&& GET( m_fEventTime )
		&& GET( m_iEventReward )
		&& GET( m_mapMinMaxReward )
		&& GET( m_bGiveOnce )
		&& GET( m_bDelEndEvent )
		&& GET( m_bPcBangEvent )
		&& GET( m_bDirectReward )
		&& GET( m_bAccountEvent )
#ifdef SERV_DROP_EVENT_RENEWAL// 작업날짜: 2013-09-09	// 박세훈
		&& GET( m_fDropRate )
#else // SERV_DROP_EVENT_RENEWAL
		&& GET( m_iDropCount )
#endif // SERV_DROP_EVENT_RENEWAL
		&& GET( m_bWithPlayPcBang )
		&& GET( m_iChannelCode )
		&& GET( m_iBF_Team )
		&& GET( m_iHenirRewardEventCount )
		
		&& GET( m_bHenirRewardUnLimited )
		&& GET( m_iServerGroup )
		&& GET( m_bAccumulateEXP )
#ifdef DROPEVENT_RENEWAL
		&& GET( m_iStartLevel )
		&& GET( m_iEndLevel )
#endif DROPEVENT_RENEWAL
		&& GET( m_bRepeatEvent )
		&& GET( m_bRepeatRewardEvent )
		&& GET( m_iBeforeEventID )
#ifdef SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		&& GET( m_bPlayWithCharEXP )
#endif SERV_PLAY_WITH_CHAR_PARTY_BONUS_EXP
		&& GET( m_iEventGroupID )
		&& GET( m_fQuestItemDropRate )
#ifdef SERV_ADVERTISEMENT_EVENT
		&& GET( m_wstrAdvertisementURL )
#endif SERV_ADVERTISEMENT_EVENT
		&& GET( m_iEventItemID )
		&& GET( m_iCustomEventID )
#ifdef	SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT		
		&& GET( m_fDefenseDungeonOpenRate )
#endif	// SERV_DEFENSE_DUNGEON_OPEN_RATE_EVENT
#ifdef	SERV_CRITERION_DATE_EVENT
		&& GET( m_byteCriterionUserType )
		&& GET( m_byteCriterionPresentUserType )
#endif	// SERV_CRITERION_DATE_EVENT
#ifdef SERV_STEAM_USER_CONNECT_EVENT
		&& GET( m_bOnlySteamUser )
		&& GET( m_bOnlyNotSteamUser )
#endif //SERV_STEAM_USER_CONNECT_EVENT
#ifdef SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
		&& GET( m_bNewUnitEvent )
		&& GET( m_bNewUnitEvent2 )
		&& GET( m_bCurrentUnitEvent )
#endif //SERV_TIME_EVENT_ONLY_CURRENT_USER_CHAR
#ifdef SERV_UNIT_CLASS_LEVEL_EVENT
		&& GET( m_iUnitClassLevel )
#endif SERV_UNIT_CLASS_LEVEL_EVENT
#ifdef SERV_HENIR_REWARD_EVENT // 작업날짜: 2013-09-09	// 박세훈
		&& GET( m_bAccumulate )
#endif // SERV_HENIR_REWARD_EVENT
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

SERIALIZE_DEFINE_PUT( KESG_REWARD_DB_DATA_NOT, obj, ks )
{
	return PUT( m_mapTotalRewardData )
		;
}

SERIALIZE_DEFINE_GET( KESG_REWARD_DB_DATA_NOT, obj, ks )
{
	return GET( m_mapTotalRewardData )
		;
}

#endif //SERV_EVENT_DB_CONTROL_SYSTEM

#ifdef SERV_CUBE_IN_ITEM_MAPPING
SERIALIZE_DEFINE_PUT( kRandomItemMappingToInfoClient, obj, ks )
{
	return PUT( m_iBeforeGroupID )
		&& PUT( m_iAfterGroupID )
		&& PUT( m_bCheckMapping )
		&& PUT( m_tStartTime )
		&& PUT( m_tEndTime )
		;
}

SERIALIZE_DEFINE_GET( kRandomItemMappingToInfoClient, obj, ks )
{
	return GET( m_iBeforeGroupID )
		&& GET( m_iAfterGroupID )
		&& GET( m_bCheckMapping )
		&& GET( m_tStartTime )
		&& GET( m_tEndTime )
		;
}


#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING 
SERIALIZE_DEFINE_PUT( KRandomItemMappingToInfoServer, obj, ks )
{
	return PUT( m_iIndex )
		&& PUT( m_iCubeID )
		&& PUT( m_iKeyItemID )
		&& PUT( m_iBeforeGroupID )
		&& PUT( m_iAfterGroupID )
		&& PUT( m_tStartTime )
		&& PUT( m_tEndTime )
		;
}

SERIALIZE_DEFINE_GET( KRandomItemMappingToInfoServer, obj, ks )
{
	return GET( m_iIndex )
		&& GET( m_iCubeID )
		&& GET( m_iKeyItemID )
		&& GET( m_iBeforeGroupID )
		&& GET( m_iAfterGroupID )
		&& GET( m_tStartTime )
		&& GET( m_tEndTime )
		;
}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
#endif //SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_USE_GM_TOOL_INFO
SERIALIZE_DEFINE_PUT( KItemName, obj, ks )
{
	return PUT( m_wstrKRItemName )
		&& PUT( m_wstrTransItemName )
		;
}

SERIALIZE_DEFINE_GET( KItemName, obj, ks )
{
	return GET( m_wstrKRItemName )
		&& GET( m_wstrTransItemName )
		;
}
#endif //SERV_USE_GM_TOOL_INFO


//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
SERIALIZE_DEFINE_PUT( KGlobalMissionReward, obj, ks )
{
	return PUT( m_iRewardType )
		&& PUT( m_iRewardDurationTime )
		&& PUT( m_iScriptID )
		;
}
SERIALIZE_DEFINE_GET( KGlobalMissionReward, obj, ks )
{
	return GET( m_iRewardType )
		&& GET( m_iRewardDurationTime )
		&& GET( m_iScriptID )
		;
}

SERIALIZE_DEFINE_PUT( KGlobalMissionInfo, obj, ks )
{
	return PUT( m_bActive )
		&& PUT( m_wstrMissionStartTime )
		&& PUT( m_iDurationMinute )
		&& PUT( m_iRemainSecond )
		&& PUT( m_iMissionType )
		&& PUT( m_iClearCondition )
		&& PUT( m_iClearCount )
		&& PUT( m_iNowCount )
		;
}
SERIALIZE_DEFINE_GET( KGlobalMissionInfo, obj, ks )
{
	return GET( m_bActive )
		&& GET( m_wstrMissionStartTime )
		&& GET( m_iDurationMinute )
		&& GET( m_iRemainSecond )
		&& GET( m_iMissionType )
		&& GET( m_iClearCondition )
		&& GET( m_iClearCount )
		&& GET( m_iNowCount )
		;
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	임홍락	글로벌 미션 매니저

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING 
SERIALIZE_DEFINE_PUT( KPacketGetItemOnOff, obj, ks )
{
	return PUT( m_iType )
		&& PUT( m_iItemID )
		&& PUT( m_wstrStartTime )
		&& PUT( m_wstrEndTime )
		;
}

SERIALIZE_DEFINE_GET( KPacketGetItemOnOff, obj, ks )
{
	return GET( m_iType )
		&& GET( m_iItemID )
		&& GET( m_wstrStartTime )
		&& GET( m_wstrEndTime )
		;
}
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_STRING_FILTER_USING_DB 
SERIALIZE_DEFINE_PUT( KStringFilterInfo, obj, ks )
{
	return PUT( m_sStringFilterType )
		&& PUT( m_wstrFilter )
		;
}

SERIALIZE_DEFINE_GET( KStringFilterInfo, obj, ks )
{
	return GET( m_sStringFilterType )
		&& GET( m_wstrFilter )
		;
}
#endif //SERV_STRING_FILTER_USING_DB
