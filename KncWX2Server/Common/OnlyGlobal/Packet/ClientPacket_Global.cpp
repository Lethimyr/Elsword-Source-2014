#include <winsock2.h>
#include "ClientPacket.h"

#ifdef SERV_GLOBAL_AUTH
SERIALIZE_DEFINE_PUT( KECH_SECURITY_AUTH_REQ, obj, ks )
{
	return PUT( m_iSecurityAuthType )
		&& PUT( m_iUserUID )
		&& PUT( m_wstrUserID )
#ifdef SERV_PUBLISHER_MATRIXKEY
		&& PUT( m_wstrMatrix )
#endif // SERV_PUBLISHER_MATRIXKEY
		;
}

SERIALIZE_DEFINE_GET( KECH_SECURITY_AUTH_REQ, obj, ks )
{
	return GET( m_iSecurityAuthType )
		&& GET( m_iUserUID )
		&& GET( m_wstrUserID )
#ifdef SERV_PUBLISHER_MATRIXKEY
		&& GET( m_wstrMatrix )
#endif // SERV_PUBLISHER_MATRIXKEY
		;
}

SERIALIZE_DEFINE_PUT( KECH_SECURITY_AUTH_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_strSecondSecurityURL )
		;
}

SERIALIZE_DEFINE_GET( KECH_SECURITY_AUTH_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_strSecondSecurityURL )
		;
}

SERIALIZE_DEFINE_PUT( KECH_SECURITY_INFO_NOT, obj, ks )
{
	return PUT( m_iSecurityInfoType )
		&& PUT( m_uiMatrixKey )
		&& PUT( m_wstrCoordinate )
		;
}

SERIALIZE_DEFINE_GET( KECH_SECURITY_INFO_NOT, obj, ks )
{
	return GET( m_iSecurityInfoType )
		&& GET( m_uiMatrixKey )
		&& GET( m_wstrCoordinate )
		;
}

#endif // SERV_GLOBAL_AUTH


#ifdef SERV_GLOBAL_BILLING

SERIALIZE_DEFINE_PUT( KEGS_EXCHANGE_CASH_REQ, obj, ks )
{
	return PUT( m_uiExchangeValue )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EXCHANGE_CASH_REQ, obj, ks )
{
	return GET( m_uiExchangeValue )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_EXCHANGE_CASH_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPoint )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_AddBonusCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_GET( KEGS_EXCHANGE_CASH_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPoint )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_AddBonusCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
SERIALIZE_DEFINE_PUT( KEGS_BONUS_CASH_EVENT_NOT, obj, ks )
{
	return PUT( m_bEventStart )
		;
}

SERIALIZE_DEFINE_GET( KEGS_BONUS_CASH_EVENT_NOT, obj, ks )
{
	return GET( m_bEventStart )
		;
}
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES

SERIALIZE_DEFINE_PUT( KEGS_BILL_PRODUCT_INFO_REQ, obj, ks )
{
	return  PUT( m_iPage );
}

SERIALIZE_DEFINE_GET( KEGS_BILL_PRODUCT_INFO_REQ, obj, ks )
{
	return  GET( m_iPage );
}

SERIALIZE_DEFINE_PUT( KEGS_BILL_PRODUCT_INFO_ACK, obj, ks )
{
	return PUT( m_bFinal )
		&& PUT( m_vecProductInfo )
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		&& PUT( m_mapKeepShowItem )
		&& PUT( m_DisCountInfoMap )
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#ifdef SERV_WISH_LIST_NO_ITEM
		&& PUT( m_setWishListNoItemList )
#endif SERV_WISH_LIST_NO_ITEM
		;
}

SERIALIZE_DEFINE_GET( KEGS_BILL_PRODUCT_INFO_ACK, obj, ks )
{
	return GET( m_bFinal )
		&& GET( m_vecProductInfo )
#ifdef SERV_KEEP_ITEM_SHOW_CASHSHOP
		&& GET( m_mapKeepShowItem )
		&& GET( m_DisCountInfoMap )
#endif SERV_KEEP_ITEM_SHOW_CASHSHOP
#ifdef SERV_WISH_LIST_NO_ITEM
		&& GET( m_setWishListNoItemList )
#endif SERV_WISH_LIST_NO_ITEM

		;
}

SERIALIZE_DEFINE_PUT( KEGS_BILL_INVENTORY_INQUIRY_REQ, obj, ks )
{
	return PUT( m_nItemPerPage )
		&& PUT( m_iCurrentPage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_BILL_INVENTORY_INQUIRY_REQ, obj, ks )
{
	return GET( m_nItemPerPage )
		&& GET( m_iCurrentPage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_BILL_INVENTORY_INQUIRY_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_vecOrderInfo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	PUT( m_vecPackageInfo )
		&&	PUT( m_vecSubPackageTrans )
		&&	PUT( m_vecSubPackageInfo )
#endif // SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KEGS_BILL_INVENTORY_INQUIRY_ACK, obj, ks )
{
	return  GET( m_iOK ) 
		&&	GET( m_vecOrderInfo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	GET( m_vecPackageInfo )
		&&	GET( m_vecSubPackageTrans )
		&&	GET( m_vecSubPackageInfo )
#endif // SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_PUT( KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ, obj, ks )
{
	return PUT( m_iTransNo )
		&& PUT( m_iProductNo ) 
		&& PUT( m_mapSocketForCash )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& PUT( m_mapSocketGroupIDForCash )
#endif // SERV_GLOBAL_CASH_PACKAGE
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& PUT( m_byteProductKind )
#endif // SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KEGS_BILL_GET_PURCHASED_CASH_ITEM_REQ, obj, ks )
{
	return GET( m_iTransNo )
		&& GET( m_iProductNo )
		&& GET( m_mapSocketForCash )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& GET( m_mapSocketGroupIDForCash )
#endif // SERV_GLOBAL_CASH_PACKAGE
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&& GET( m_byteProductKind )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

#ifdef SERV_GOLD_TICKET
SERIALIZE_DEFINE_PUT( KEGS_CHARGE_POINT_NOT, obj, ks)
{
	return PUT(m_iOK)
		&& PUT(m_iChargedCash);
};

SERIALIZE_DEFINE_GET( KEGS_CHARGE_POINT_NOT, obj, ks)
{
	return GET(m_iOK)
		&& GET(m_iChargedCash);
};
#endif //SERV_GOLD_TICKET

#endif // SERV_GLOBAL_BILLING

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
SERIALIZE_DEFINE_PUT( KEGS_ADMIN_DELETE_GUILD_ADD_NOT, obj, ks )
{
	return PUT( m_iGuildUID	   )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_DELETE_GUILD_ADD_NOT, obj, ks )
{
	return GET( m_iGuildUID	   )
		;
}
#endif

//{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
SERIALIZE_DEFINE_PUT( KEGS_ADMIN_DELETE_HENIR_RANKING_RANK_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_iRank		   )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_DELETE_HENIR_RANKING_RANK_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_iRank		   )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_iUnitUID	   )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_iUnitUID	   )
		;
}
#endif SERV_DELETE_HENIR_RANKING


#ifdef SERV_NEW_UNIT_TRADE_LIMIT
SERIALIZE_DEFINE_PUT( KEGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT, obj, ks )
{
	return PUT( m_wstrNewCharTBMsg )
		;
}

SERIALIZE_DEFINE_GET( KEGS_NEW_CHARACTER_TRADE_BLOCK_MSG_NOT, obj, ks )
{
	return GET( m_wstrNewCharTBMsg )
		;
}
#endif SERV_NEW_UNIT_TRADE_LIMIT

// 2011.05.05 lygan_조성욱 // 중국용 커뮤니티탭 유저 정보 보기
#ifdef SERV_USER_WATCH_NEW

SERIALIZE_DEFINE_PUT( KEGS_USER_COMMUNITY_SURVEY_NEW_REQ , obj, ks )
{
	return PUT(m_wUnitNickName)
		&& PUT(m_cCommunity_Tab_Type)
		&& PUT(m_iOK);

}
SERIALIZE_DEFINE_GET( KEGS_USER_COMMUNITY_SURVEY_NEW_REQ , obj, ks )
{
	return GET(m_wUnitNickName)
		&& GET(m_cCommunity_Tab_Type)
		&& GET(m_iOK);

}

SERIALIZE_DEFINE_PUT( KEGS_USER_COMMUNITY_SURVEY_NEW_ACK , obj, ks )
{
	return PUT(unitUID)
		&& PUT(m_cCommunity_Tab_Type)
		&& PUT(m_iOK);

}
SERIALIZE_DEFINE_GET( KEGS_USER_COMMUNITY_SURVEY_NEW_ACK , obj, ks )
{
	return GET(unitUID)
		&& GET(m_cCommunity_Tab_Type)
		&& GET(m_iOK);

}
#endif //SERV_USER_WATCH_NEW

#ifdef SERV_NPROTECT_CS_AUTH_30
SERIALIZE_DEFINE_PUT( KEGS_NPROTECT_CSAUTH3_CHECK_NOT, obj, ks )
{
    return PUT( m_deqCSAuth3 )
		;
}

SERIALIZE_DEFINE_GET( KEGS_NPROTECT_CSAUTH3_CHECK_NOT, obj, ks )
{
    return GET( m_deqCSAuth3 )
		;
}
#endif SERV_NPROTECT_CS_AUTH_30

#ifdef SERV_PVP_REMATCH
SERIALIZE_DEFINE_PUT( KEGS_PVP_REMATCH_REQ, obj, ks )
{
	return PUT( m_bAcceptRematch )
		;
}
SERIALIZE_DEFINE_GET( KEGS_PVP_REMATCH_REQ, obj, ks )
{
	return GET( m_bAcceptRematch )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_PVP_REMATCH_ACK, obj, ks )
{
	return PUT( m_iOK )
		;
}
SERIALIZE_DEFINE_GET( KEGS_PVP_REMATCH_ACK, obj, ks )
{
	return GET( m_iOK )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_PVP_REMATCH_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_bAcceptRematch )
		;
}
SERIALIZE_DEFINE_GET( KEGS_PVP_REMATCH_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_bAcceptRematch )
		;
}
#endif SERV_PVP_REMATCH

//{{ 2012.02.20 조효진	캐릭터 삭제 프로세스 변경 (삭제 대기 기간 도입)
#ifdef SERV_UNIT_WAIT_DELETE
SERIALIZE_DEFINE_PUT( KEGS_RESTORE_UNIT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iUserUID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_RESTORE_UNIT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iUserUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_RESTORE_UNIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_tRestoreAbleDate )
		;
}

SERIALIZE_DEFINE_GET( KEGS_RESTORE_UNIT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_tRestoreAbleDate )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_FINAL_DELETE_UNIT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iUserUID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_FINAL_DELETE_UNIT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iUserUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_FINAL_DELETE_UNIT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_uiKNMSerialNum )
		//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
		&& PUT( m_iGuildUID )
#endif GUILD_TEST
		//}}
		//{{ 2011. 02. 23	최육사	캐릭터 로그
#ifdef SERV_CHAR_LOG
		&& PUT( m_wstrNickName )
		&& PUT( m_ucLevel )
		&& PUT( m_cUnitClass )
#endif SERV_CHAR_LOG
		//}}
		&& PUT( m_tReDelAbleDate )
		;
}

SERIALIZE_DEFINE_GET( KEGS_FINAL_DELETE_UNIT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_uiKNMSerialNum )
		//{{ 2009. 10. 8  최육사	길드
#ifdef GUILD_TEST
		&& GET( m_iGuildUID )
#endif GUILD_TEST
		//}}
		//{{ 2011. 02. 23	최육사	캐릭터 로그
#ifdef SERV_CHAR_LOG
		&& GET( m_wstrNickName )
		&& GET( m_ucLevel )
		&& GET( m_cUnitClass )
#endif SERV_CHAR_LOG
		//}}
		&& GET( m_tReDelAbleDate )
		;
}
#endif SERV_UNIT_WAIT_DELETE
//}}

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
SERIALIZE_DEFINE_PUT( KEGS_RESOLVE_ITEM_AVATAR_REQ, obj, ks )
{
	return PUT( m_mapResolveDataAvatar )
		&& PUT( m_iAttachFee )
		;
}

SERIALIZE_DEFINE_GET( KEGS_RESOLVE_ITEM_AVATAR_REQ, obj, ks )
{
	return GET( m_mapResolveDataAvatar )
		&& GET( m_iAttachFee )
		;
}

//////////////////////////////////////////////////////////////////////////
SERIALIZE_DEFINE_PUT( KEGS_RESOLVE_ITEM_AVATAR_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bJackpot )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_vecKInventorySlotInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_RESOLVE_ITEM_AVATAR_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bJackpot )
		&& GET( m_mapInsertedItem )
		&& GET( m_vecKInventorySlotInfo )
		;
}

#endif SERV_MULTI_RESOLVE
//}}

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
SERIALIZE_DEFINE_PUT( KEGS_SYNTHESIS_SOCKET_GROUPID_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapAvatarSocketOptionGroupID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_SYNTHESIS_SOCKET_GROUPID_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapAvatarSocketOptionGroupID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_SYNTHESIS_ITEM_REQ, obj, ks )
{
	return PUT( m_mapSynthesisData )
		&& PUT( m_iKeyData )
		&& PUT( m_iSocketNo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_SYNTHESIS_ITEM_REQ, obj, ks )
{
	return GET( m_mapSynthesisData )
		&& GET( m_iKeyData )
		&& GET( m_iSocketNo )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_SYNTHESIS_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRessurectionCount )
		&& PUT( m_iRestoreSpirit )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_vecKInventorySlotInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_SYNTHESIS_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )		
		&& GET( m_iRessurectionCount )
		&& GET( m_iRestoreSpirit )
		&& GET( m_mapInsertedItem )
		&& GET( m_vecKInventorySlotInfo )
		;
}
//}}
#endif SERV_SYNTHESIS_AVATAR
//}}

#ifdef SERVER_GROUP_UI_ADVANCED
SERIALIZE_DEFINE_PUT( KECH_GET_SERVERGROUP_LIST_NOT, obj, ks )
{
	return PUT( m_mapServerGroupList )
		;
}

SERIALIZE_DEFINE_GET( KECH_GET_SERVERGROUP_LIST_NOT, obj, ks )
{
	return GET( m_mapServerGroupList )
		;
}
#endif SERVER_GROUP_UI_ADVANCED

//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
SERIALIZE_DEFINE_PUT( KEGS_ANTI_ADDICTION_NOT, obj, ks )
{
	return  PUT( m_iType )
		&&	PUT( m_iPlayTime )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ANTI_ADDICTION_NOT, obj, ks )
{
	return  GET( m_iType )
		&&	GET( m_iPlayTime )
		;
}
#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

#ifdef SERV_EPAY_SYSTEM
SERIALIZE_DEFINE_PUT( KEGS_EPAY_INFO_NOT, obj, ks )
{
	return PUT( m_wstrLoginID )
		&& PUT( m_usZone )
		&& PUT( m_usGame )
		&& PUT( m_wstrCharacterName )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EPAY_INFO_NOT, obj, ks )
{
	return GET( m_wstrLoginID )
		&& GET( m_usZone )
		&& GET( m_usGame )
		&& GET( m_wstrCharacterName )
		&& GET( m_iUnitUID )
		;
}
#endif //SERV_EPAY_SYSTEM

#ifdef SERV_IDENTITY_CONFIRM_POPUP_MESSAGE
SERIALIZE_DEFINE_PUT( KEGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT, obj, ks )
{
	return PUT( m_bCheckAdult )
		;
}

SERIALIZE_DEFINE_GET( KEGS_IDENTITY_CONFIRM_POPUP_MESSAGE_NOT, obj, ks )
{
	return GET( m_bCheckAdult )
		;
}
#endif //SERV_IDENTITY_CONFIRM_POPUP_MESSAGE

#ifdef SERV_USE_XTRAP
SERIALIZE_DEFINE_PUT( KEGS_XTRAP_REQ, obj, ks )
{
	return PUT(m_vecData);
}

SERIALIZE_DEFINE_GET( KEGS_XTRAP_REQ, obj, ks )
{
	return GET(m_vecData);
}

SERIALIZE_DEFINE_PUT( KEGS_XTRAP_ACK, obj, ks )
{
	return PUT(m_vecData);
}

SERIALIZE_DEFINE_GET( KEGS_XTRAP_ACK, obj, ks )
{
	return GET(m_vecData);
}
#endif SERV_USE_XTRAP

#ifdef SERV_INVISIBLE_GM
SERIALIZE_DEFINE_PUT( KEGS_TOGGLE_INVISIBLE_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kFieldUserInfo )
		;
}
SERIALIZE_DEFINE_GET( KEGS_TOGGLE_INVISIBLE_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kFieldUserInfo )
		;
}
#endif SERV_INVISIBLE_GM

#ifdef	SERV_SHARING_BANK_TEST

#ifdef SERV_NEW_UNIT_TRADE_LIMIT
SERIALIZE_DEFINE_PUT( KEGS_GET_SHARE_BANK_REQ, obj, ks )
{
	return PUT( m_wstrNickName )
		&& PUT( m_iNewUnitTradeBlockDay )
		&& PUT( m_iNewUnitTradeBlockUnitClass )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_SHARE_BANK_REQ, obj, ks )
{
	return GET( m_wstrNickName )
		&& GET( m_iNewUnitTradeBlockDay )
		&& GET( m_iNewUnitTradeBlockUnitClass )
		;
}
#endif // SERV_NEW_UNIT_TRADE_LIMIT

SERIALIZE_DEFINE_PUT( KEGS_GET_SHARE_BANK_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_iBankSize )
		&& PUT( m_mapItem )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_SHARE_BANK_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_iBankSize )
		&& GET( m_mapItem )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_UPDATE_ITEM_POSITION_NOT, obj, ks )
{
	return PUT( m_vecShareItemResult )
		;
}

SERIALIZE_DEFINE_GET( KEGS_UPDATE_ITEM_POSITION_NOT, obj, ks )
{
	return GET( m_vecShareItemResult )
		;
}
#endif	SERV_SHARING_BANK_TEST

#ifdef SERV_SHARING_BANK_QUEST_CASH
SERIALIZE_DEFINE_PUT( KEGS_SHARING_BACK_OPEN_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iOpenType )
		;
};

SERIALIZE_DEFINE_GET( KEGS_SHARING_BACK_OPEN_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iOpenType )
		;
};
#endif SERV_SHARING_BANK_QUEST_CASH

#ifdef SERV_ADVERTISEMENT_EVENT
SERIALIZE_DEFINE_PUT( KEGS_GET_ADVERTISEMENT_EVENT_ACK, obj, ks )
{
	return PUT( m_vecAdvertisementURL )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_ADVERTISEMENT_EVENT_ACK, obj, ks )
{
	return GET( m_vecAdvertisementURL )
		;
}
#endif SERV_ADVERTISEMENT_EVENT


#ifdef SERV_ADD_WARP_BUTTON
SERIALIZE_DEFINE_PUT( KEGS_WARP_BY_BUTTON_REQ, obj, ks )
{
	return PUT( m_iCurrentVillageWarpIndex )
		&& PUT( m_bFreeWarp )
		;
};

SERIALIZE_DEFINE_GET( KEGS_WARP_BY_BUTTON_REQ, obj, ks )
{
	return GET( m_iCurrentVillageWarpIndex )
		&& GET( m_bFreeWarp )
		;
};

SERIALIZE_DEFINE_PUT( KEGS_WARP_BY_BUTTON_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iED )
		&& PUT( m_iWarpPointMapID )
		;
};

SERIALIZE_DEFINE_GET( KEGS_WARP_BY_BUTTON_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iED )
		&& GET( m_iWarpPointMapID )
		;
};
#endif // SERV_ADD_WARP_BUTTON

#ifdef SERV_NEW_EVENT_TYPES

SERIALIZE_DEFINE_PUT( KEGS_UPDATE_MAX_LEVEL_NOT, obj, ks )
{
	return PUT( m_iMaxLevel )
		;
}

SERIALIZE_DEFINE_GET( KEGS_UPDATE_MAX_LEVEL_NOT, obj, ks )
{
	return GET( m_iMaxLevel )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_UPDATE_DUNGEON_STATUS_NOT, obj, ks )
{
	return PUT( m_mapEnableDungeon )
		;
}

SERIALIZE_DEFINE_GET( KEGS_UPDATE_DUNGEON_STATUS_NOT, obj, ks )
{
	return GET( m_mapEnableDungeon )
		;
}

#endif SERV_NEW_EVENT_TYPES

#ifdef SERV_CODE_EVENT
SERIALIZE_DEFINE_PUT( KEGS_UPDATE_CODE_EVENT_NOT, obj, ks )
{
	return PUT( m_mapEnableCodeEnum )
		;
}
SERIALIZE_DEFINE_GET( KEGS_UPDATE_CODE_EVENT_NOT, obj, ks )
{
	return GET( m_mapEnableCodeEnum )
		;
}
#endif SERV_CODE_EVENT

#ifdef SERV_ADD_WARP_BUTTON
SERIALIZE_DEFINE_PUT( KEGS_UPDATE_WARP_VIP_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_trWarpVipEndDate )
		;
}


SERIALIZE_DEFINE_GET( KEGS_UPDATE_WARP_VIP_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_trWarpVipEndDate )
		;
}
#endif // SERV_ADD_WARP_BUTTON


#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
SERIALIZE_DEFINE_PUT( KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_REQ, obj, ks )
{
	return PUT( m_bCheckClinetDirectPcBang )
		&& PUT( m_iIsGameBangType )
#ifdef SERV_ID_NETMARBLE_PCBANG
		&& PUT( m_wstrPublicIP )
		&& PUT( m_wstrMacAdress )
#endif //SERV_ID_NETMARBLE_PCBANG
		;
}


SERIALIZE_DEFINE_GET( KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_REQ, obj, ks )
{
	return GET( m_bCheckClinetDirectPcBang )
		&& GET( m_iIsGameBangType )
#ifdef SERV_ID_NETMARBLE_PCBANG
		&& GET( m_wstrPublicIP )
		&& GET( m_wstrMacAdress )
#endif //SERV_ID_NETMARBLE_PCBANG
		;
}
SERIALIZE_DEFINE_PUT( KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK, obj, ks )
{
	return PUT( m_bCheckClinetDirectPcBang )
		&& PUT( m_iIsGameBangType )
#ifdef SERV_PC_BANG_TYPE
		&& PUT( m_kPcBangReward )
#endif SERV_PC_BANG_TYPE
		;
}


SERIALIZE_DEFINE_GET( KEGS_CLIENT_DIRECT_CONNECT_AUTH_PCBAG_ACK, obj, ks )
{
	return GET( m_bCheckClinetDirectPcBang )
		&& GET( m_iIsGameBangType )
#ifdef SERV_PC_BANG_TYPE
		&& GET( m_kPcBangReward )
#endif SERV_PC_BANG_TYPE
		;
}
#endif //SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG
SERIALIZE_DEFINE_PUT( KEGS_OPEN_RANDOM_ITEM_DEVELOPER_REQ, obj, ks )
{
	return PUT( m_iItemID )
		&& PUT( m_iItemKeyID )
		&& PUT( m_iOpenCount )
		;
}

SERIALIZE_DEFINE_GET( KEGS_OPEN_RANDOM_ITEM_DEVELOPER_REQ, obj, ks )
{
	return GET( m_iItemID )
		&& GET( m_iItemKeyID )
		&& GET( m_iOpenCount )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_map_RandomItemResult )
		;
}

SERIALIZE_DEFINE_GET( KEGS_OPEN_RANDOM_ITEM_DEVELOPER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_map_RandomItemResult )
		;
}
#endif//SERV_DEVELOPER_RANDOM_OPEN_ITEM_LOG

#ifdef SERV_CHECK_TIME_QUEST
SERIALIZE_DEFINE_PUT( KEGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT, obj, ks )
{
	return PUT( m_vecWarningQuestID );
}

SERIALIZE_DEFINE_GET( KEGS_EVENT_QUEST_CHECK_FOR_ADMIN_NOT, obj, ks )
{
	return GET( m_vecWarningQuestID );
}
#endif // SERV_CHECK_TIME_QUEST

#ifdef SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
SERIALIZE_DEFINE_PUT( KEGS_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT, obj, ks )
{
	return PUT( m_iUDPPortSuccessType )
		&& PUT( m_iUseUdpPort )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT, obj, ks )
{
	return GET( m_iUDPPortSuccessType )
		&& GET( m_iUseUdpPort )
		;
}
#endif //SERV_CLIENT_PORT_CHANGE_REQUEST_LOG

#ifdef SERV_SKILL_USE_SUBQUEST
SERIALIZE_DEFINE_PUT( KEGS_SKILL_USE_REQ, obj, ks )
{
	return PUT( m_iSkillID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_SKILL_USE_REQ, obj, ks )
{
	return GET( m_iSkillID )
		;
}

#endif SERV_SKILL_USE_SUBQUEST

//{{김준환 서버시간 받아오기
#ifdef	SERV_SERVER_TIME_GET
SERIALIZE_DEFINE_PUT(KECH_GET_SERVER_TIME_ACK, obj,ks)
{
	return PUT(m_wstrCurrentTime)
		;
}
SERIALIZE_DEFINE_GET(KECH_GET_SERVER_TIME_ACK, obj,ks)
{
	return GET(m_wstrCurrentTime)
		;
}
#endif  SERV_SERVER_TIME_GET

#ifdef SERV_COUNTRY_PH
SERIALIZE_DEFINE_PUT( KEGS_EXCHANGE_CASH_CLIENT_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPoint )
		&& PUT( m_AddBonusCashInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EXCHANGE_CASH_CLIENT_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPoint )
		&& GET( m_AddBonusCashInfo )
		;
}
#endif //SERV_COUNTRY_PH

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
SERIALIZE_DEFINE_PUT(KEGS_CHANGE_PET_ID_NOT, obj,ks)
{
	return PUT( m_iOK )
		&& PUT( m_iPetUID )
		;
}
SERIALIZE_DEFINE_GET(KEGS_CHANGE_PET_ID_NOT, obj,ks)
{
	return GET( m_iOK )
		&& GET( m_iPetUID )
		;
}
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_COUPON_EVENT
SERIALIZE_DEFINE_PUT( KEGS_COUPON_ITEM_CHECK_REQ, obj, ks )
{
	return PUT( m_iButtonType )
		;
}
SERIALIZE_DEFINE_GET( KEGS_COUPON_ITEM_CHECK_REQ, obj, ks )
{
	return GET( m_iButtonType )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_COUPON_ITEM_CHECK_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCouponType )
		&& PUT( m_iItemID )
		;
}
SERIALIZE_DEFINE_GET( KEGS_COUPON_ITEM_CHECK_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCouponType )
		&& GET( m_iItemID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_COUPON_ENTRY_REQ, obj, ks )
{
	return PUT( m_iCouponType )
		;
}
SERIALIZE_DEFINE_GET( KEGS_COUPON_ENTRY_REQ, obj, ks )
{
	return GET( m_iCouponType )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_COUPON_ENTRY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}
SERIALIZE_DEFINE_GET( KEGS_COUPON_ENTRY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
#endif //SERV_COUPON_EVENT

#ifdef SERV_GUILD_FIND_AD
SERIALIZE_DEFINE_PUT( KEGS_GET_GUILD_FIND_AD_LIST_REQ, obj, ks )
{
	return PUT( m_cSortType )
		&& PUT( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_GUILD_FIND_AD_LIST_REQ, obj, ks )
{
	return GET( m_cSortType )
		&& GET( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_GUILD_FIND_AD_LIST_ACK, obj, ks )
{
	return PUT( m_cSortType )
		&& PUT( m_uiTotalPage )
		&& PUT( m_uiViewPage )
		&& PUT( m_vecGuildFindAdList )
		&& PUT( m_bCanShowInviteUserList )
		&& PUT( m_bCanShowInviteGuildList )
		&& PUT( m_bRegMyGuildFindAd )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_GUILD_FIND_AD_LIST_ACK, obj, ks )
{
	return GET( m_cSortType )
		&& GET( m_uiTotalPage )
		&& GET( m_uiViewPage )
		&& GET( m_vecGuildFindAdList )
		&& GET( m_bCanShowInviteUserList )
		&& GET( m_bCanShowInviteGuildList )
		&& GET( m_bRegMyGuildFindAd )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_REGISTRATION_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return PUT( m_bOldInviteMsgDelete )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_iUnitLevel )
		&& PUT( m_wstrNickName )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_REGISTRATION_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return GET( m_bOldInviteMsgDelete )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_iUnitLevel )
		&& GET( m_wstrNickName )
		&& GET( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_REGISTRATION_GUILD_INVITE_MSG_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iED )
		&& PUT( m_sPeriod )
		&& PUT( m_wstrNickName )
		&& PUT( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_REGISTRATION_GUILD_INVITE_MSG_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iED )
		&& GET( m_sPeriod )
		&& GET( m_wstrNickName )
		&& GET( m_wstrMessage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_REGISTRATION_GUILD_FIND_AD_REQ, obj, ks )
{
	return PUT( m_sPeriod )
		&& PUT( m_wstrFindAdMessage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_REGISTRATION_GUILD_FIND_AD_REQ, obj, ks )
{
	return GET( m_sPeriod )
		&& GET( m_wstrFindAdMessage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_REGISTRATION_GUILD_FIND_AD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iED )
		&& PUT( m_sPeriod )
		&& PUT( m_wstrFindAdMessage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_REGISTRATION_GUILD_FIND_AD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iED )
		&& GET( m_sPeriod )
		&& GET( m_wstrFindAdMessage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_GUILD_INVITE_USER_LIST_REQ, obj, ks )
{
	return PUT( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_GUILD_INVITE_USER_LIST_REQ, obj, ks )
{
	return GET( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_GUILD_INVITE_USER_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrGuildName )
		&& PUT( m_uiTotalPage )
		&& PUT( m_uiViewPage )
		&& PUT( m_vecGuildInviteUserList )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_GUILD_INVITE_USER_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrGuildName )
		&& GET( m_uiTotalPage )
		&& GET( m_uiViewPage )
		&& GET( m_vecGuildInviteUserList )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_GUILD_INVITE_GUILD_LIST_REQ, obj, ks )
{
	return PUT( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_GUILD_INVITE_GUILD_LIST_REQ, obj, ks )
{
	return GET( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_GUILD_INVITE_GUILD_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_uiTotalPage )
		&& PUT( m_uiViewPage )
		&& PUT( m_vecGuildInviteGuildList )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_GUILD_INVITE_GUILD_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_uiTotalPage )
		&& GET( m_uiViewPage )
		&& GET( m_vecGuildInviteGuildList )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_ACCEPT_INVITE_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_ACCEPT_INVITE_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CANCEL_INVITE_MSG_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iDeletedUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CANCEL_INVITE_MSG_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iDeletedUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CANCEL_INVITE_MSG_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bCancel )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CANCEL_INVITE_MSG_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bCancel )
		;
}
#endif SERV_GUILD_FIND_AD

#ifdef SERV_READY_TO_SOSUN_EVENT
SERIALIZE_DEFINE_PUT( KEGS_READY_TO_SOSUN_EVENT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecKInventorySlotInfo )
		&& PUT( m_iFirstUnitClass )
		;
}

SERIALIZE_DEFINE_GET( KEGS_READY_TO_SOSUN_EVENT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecKInventorySlotInfo )
		&& GET( m_iFirstUnitClass )
		;
}
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_MOMOTI_EVENT
SERIALIZE_DEFINE_PUT( KEGS_MOMOTI_QUIZ_EVENT_REQ, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_istrReply )
		;
}

SERIALIZE_DEFINE_GET( KEGS_MOMOTI_QUIZ_EVENT_REQ, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_istrReply )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_MOMOTI_QUIZ_EVENT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCheckReward )
		;
}

SERIALIZE_DEFINE_GET( KEGS_MOMOTI_QUIZ_EVENT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCheckReward )
		;
}
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
SERIALIZE_DEFINE_PUT( KEGS_USE_PROPOSE_ITEM_REQ, obj, ks )
{
	return PUT( m_iUsedItemUID )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KEGS_USE_PROPOSE_ITEM_REQ, obj, ks )
{
	return GET( m_iUsedItemUID )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_USE_PROPOSE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrNickName )
		&& PUT( m_vecKInventorySlotInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_USE_PROPOSE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrNickName )
		&& GET( m_vecKInventorySlotInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT, obj, ks )
{
	return PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EVENT_PROPOSE_RESULT_ACCEPTOR_NOT, obj, ks )
{
	return GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_USE_DIVORCE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KEGS_USE_DIVORCE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecUpdatedInventorySlot )
		;
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_CHINA_SPIRIT_EVENT
SERIALIZE_DEFINE_PUT( KEGS_USE_SPIRIT_REWARD_REQ, obj, ks )
{
	return PUT( m_iLocationIndex )
		;
}
SERIALIZE_DEFINE_GET( KEGS_USE_SPIRIT_REWARD_REQ, obj, ks )
{
	return GET( m_iLocationIndex )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_USE_SPIRIT_REWARD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_arrChinaSpirit[0] )
		&& PUT( m_arrChinaSpirit[1] )
		&& PUT( m_arrChinaSpirit[2] )
		&& PUT( m_arrChinaSpirit[3] )
		&& PUT( m_arrChinaSpirit[4] )
		&& PUT( m_arrChinaSpirit[5] )
		&& PUT( m_iLocationIndex )
		;
}
SERIALIZE_DEFINE_GET( KEGS_USE_SPIRIT_REWARD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_arrChinaSpirit[0] )
		&& GET( m_arrChinaSpirit[1] )
		&& GET( m_arrChinaSpirit[2] )
		&& GET( m_arrChinaSpirit[3] )
		&& GET( m_arrChinaSpirit[4] )
		&& GET( m_arrChinaSpirit[5] )
		&& GET( m_iLocationIndex )
		;
}
#endif SERV_CHINA_SPIRIT_EVENT

#ifdef SERV_PERIOD_PET
SERIALIZE_DEFINE_PUT( KEGS_RELEASE_PET_REQ, obj, ks )
{
	return PUT( m_iPetUID )
		;
}
SERIALIZE_DEFINE_GET( KEGS_RELEASE_PET_REQ, obj, ks )
{
	return GET( m_iPetUID )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_RELEASE_PET_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPetUID )
		;
}
SERIALIZE_DEFINE_GET( KEGS_RELEASE_PET_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPetUID )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_RELEASE_PET_NOT, obj, ks )
{
	return PUT( m_iPetUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iPetID )
		;
}
SERIALIZE_DEFINE_GET( KEGS_RELEASE_PET_NOT, obj, ks )
{
	return GET( m_iPetUID )
		&& GET( m_iUnitUID )
		&& GET( m_iPetID )
		;
}
#endif SERV_PERIOD_PET

#ifdef SERV_CUBE_IN_ITEM_MAPPING
SERIALIZE_DEFINE_PUT( KEGS_CUBE_IN_ITEM_MAPPING_NOT , obj, ks )
{
	return PUT( m_mapCubeInItemMapping )
		;
}
SERIALIZE_DEFINE_GET( KEGS_CUBE_IN_ITEM_MAPPING_NOT , obj, ks )
{
	return GET( m_mapCubeInItemMapping )
		;
}
#endif SERV_CUBE_IN_ITEM_MAPPING

#ifdef SERV_RECRUIT_EVENT_BASE
SERIALIZE_DEFINE_PUT( KEGS_USE_RECRUIT_TICKET_REQ, obj, ks )
{
	return PUT( m_iItemUID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_USE_RECRUIT_TICKET_REQ, obj, ks )
{
	return GET( m_iItemUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_USE_RECRUIT_TICKET_ACK, obj, ks )
{
	return PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KEGS_USE_RECRUIT_TICKET_ACK, obj, ks )
{
	return GET( m_iOK )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_REGISTER_RECRUITER_REQ, obj, ks )
{
	return PUT( m_wstrNickname )
		;
}

SERIALIZE_DEFINE_GET( KEGS_REGISTER_RECRUITER_REQ, obj, ks )
{
	return GET( m_wstrNickname )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_REGISTER_RECRUITER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecKInventorySlotInfo )
		&& PUT( m_iUsedItemID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_REGISTER_RECRUITER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecKInventorySlotInfo )
		&& GET( m_iUsedItemID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_REGISTER_RECRUITER_NOT, obj, ks )
{
	return PUT( m_iRecruiterUnitUID )
		&& PUT( m_kRecruitUnitInfo )
		;
}
SERIALIZE_DEFINE_GET( KEGS_REGISTER_RECRUITER_NOT, obj, ks )
{
	return GET( m_iRecruiterUnitUID )
		&& GET( m_kRecruitUnitInfo )		
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_RECRUIT_RECRUITER_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecRecruitUnitInfo )
		&& PUT( m_vecRecruiterUnitInfo )
		;
}
SERIALIZE_DEFINE_GET( KEGS_GET_RECRUIT_RECRUITER_LIST_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecRecruitUnitInfo )
		&& GET( m_vecRecruiterUnitInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_RECRUIT_RECRUITER_INFO_NOT, obj, ks )
{
	return PUT( m_iSenderUnitUID )
		&& PUT( m_cType )
		&& PUT( m_iData )
		;
}
SERIALIZE_DEFINE_GET( KEGS_RECRUIT_RECRUITER_INFO_NOT, obj, ks )
{
	return GET( m_iSenderUnitUID )
		&& GET( m_cType )
		&& GET( m_iData )
		;
}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING

SERIALIZE_DEFINE_PUT( KEGS_GET_EVENT_INFO_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kEventCharacterRaking )
		;
}
SERIALIZE_DEFINE_GET( KEGS_GET_EVENT_INFO_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kEventCharacterRaking )
		;
}

#endif SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_ELESIS_UPDATE_EVENT
SERIALIZE_DEFINE_PUT( KEGS_EVENT_NOTE_VIEW_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iTitleID )
		&& PUT( m_iNoteViewCount )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_NOTE_VIEW_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iTitleID )
		&& GET( m_iNoteViewCount )
		;
}
#endif SERV_ELESIS_UPDATE_EVENT

#ifdef SERV_NEW_YEAR_EVENT_2014
SERIALIZE_DEFINE_PUT( KEGS_2013_EVENT_MISSION_COMPLETE_REQ, obj, ks )
{
	return PUT( m_bLevelUpEvent )
		;
}
SERIALIZE_DEFINE_GET( KEGS_2013_EVENT_MISSION_COMPLETE_REQ, obj, ks )
{
	return GET( m_bLevelUpEvent )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_2013_EVENT_MISSION_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRewardedLevel )
		&& PUT( m_vecInventorySlotInfo )
		;
}
SERIALIZE_DEFINE_GET( KEGS_2013_EVENT_MISSION_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRewardedLevel )
		&& GET( m_vecInventorySlotInfo )
		;
}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_GLOBAL_MISSION_MANAGER
SERIALIZE_DEFINE_PUT( KEGS_GLOBAL_MISSION_UPDATE_NOT, obj, ks )
{
	return PUT( m_mapGlobalMissionInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GLOBAL_MISSION_UPDATE_NOT, obj, ks )
{
	return GET( m_mapGlobalMissionInfo )
		;
}
#endif SERV_GLOBAL_MISSION_MANAGER
#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
SERIALIZE_DEFINE_PUT( KEGS_UNLIMITED_SECOND_CHANGE_JOB_NOT , obj, ks )
{
	return PUT( m_iUnlimitedSecondChangeJob )
		;
}
SERIALIZE_DEFINE_GET( KEGS_UNLIMITED_SECOND_CHANGE_JOB_NOT , obj, ks )
{
	return GET( m_iUnlimitedSecondChangeJob )
		;
}
#endif SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_EVENT_CHECK_POWER
SERIALIZE_DEFINE_PUT( KEGS_START_CHECK_POWER_REQ, obj, ks )
{
	return PUT( m_bStart )
		;
}
SERIALIZE_DEFINE_GET( KEGS_START_CHECK_POWER_REQ, obj, ks )
{
	return GET( m_bStart )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_UPDATE_CHECK_POWER_NOT, obj, ks )
{
	return PUT( m_ucCheckPowerCount )
		&& PUT( m_iCheckPowerTime )
		&& PUT( m_ucCheckPowerScore )
		;
}
SERIALIZE_DEFINE_GET( KEGS_UPDATE_CHECK_POWER_NOT, obj, ks )
{
	return GET( m_ucCheckPowerCount )
		&& GET( m_iCheckPowerTime )
		&& GET( m_ucCheckPowerScore )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_SET_MULTIPLYER, obj, ks )
{
	return PUT( fM )
		;
}
SERIALIZE_DEFINE_GET( KEGS_SET_MULTIPLYER, obj, ks )
{
	return GET( fM )
		;
}
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
SERIALIZE_DEFINE_PUT( KEGS_BUY_UI_SETTING_REQ, obj, ks )
{
	return PUT( m_iTimeControlItemType )
		&& PUT( m_iHouseID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_BUY_UI_SETTING_REQ, obj, ks )
{
	return GET( m_iTimeControlItemType )
		&& GET( m_iHouseID )
		;
}


SERIALIZE_DEFINE_PUT( KEGS_BUY_UI_SETTING_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_setGetItemOnOff )
		&& PUT( m_iHouseID )
		&& PUT( m_iTimeControlItemType )
		;
}

SERIALIZE_DEFINE_GET( KEGS_BUY_UI_SETTING_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_setGetItemOnOff )
		&& GET( m_iHouseID )
		&& GET( m_iTimeControlItemType )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK, obj, ks )
{
	return PUT( m_mapGetItemOnOff )
		;
}

SERIALIZE_DEFINE_GET( KEGS_GET_TIME_CONTROL_ITME_TALK_LIST_ACK, obj, ks )
{
	return GET( m_mapGetItemOnOff )
		;
}

#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
SERIALIZE_DEFINE_PUT( KEGS_EVENT_CHUNG_GIVE_ITEM_NOT, obj, ks )
{
	return PUT( m_bGiveItemGet )
		&& PUT( m_wstrToolTipTime )
		&& PUT( m_cGetUnitClass )
		&& PUT( m_bTwoGiveItem )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EVENT_CHUNG_GIVE_ITEM_NOT, obj, ks )
{
	return GET( m_bGiveItemGet )
		&& GET( m_wstrToolTipTime )
		&& GET( m_cGetUnitClass )
		&& GET( m_bTwoGiveItem )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_CHUNG_GIVE_ITEM_REQ, obj, ks )
{
	return PUT( iChoice )
		&& PUT( bTwoGiveItem )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EVENT_CHUNG_GIVE_ITEM_REQ, obj, ks )
{
	return GET( iChoice )
		&& GET( bTwoGiveItem )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_CHUNG_GIVE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrGetItemTime )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EVENT_CHUNG_GIVE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrGetItemTime )
		;
}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
SERIALIZE_DEFINE_PUT( KEGS_EVENT_COBO_DUNGEON_FIELD_NOT , obj, ks )
{
	return PUT( m_StartButtonUI )
		&& PUT( m_DungeonCountUI )
		&& PUT( m_FieldCountUI )
		&& PUT( m_DungeonCount )
		&& PUT( m_FieldMonsterKillCount )
		&& PUT( m_iOk )
		&& PUT( m_iRemaindTime )
		&& PUT( m_tPushTime )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_COBO_DUNGEON_FIELD_NOT , obj, ks )
{
	return GET( m_StartButtonUI )
		&& GET( m_DungeonCountUI )
		&& GET( m_FieldCountUI )
		&& GET( m_DungeonCount )
		&& GET( m_FieldMonsterKillCount )
		&& GET( m_iOk )
		&& GET( m_iRemaindTime )
		&& GET( m_tPushTime )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_COBO_DUNGEON_FIELD_REQ , obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_EventStart )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_COBO_DUNGEON_FIELD_REQ , obj, ks )
{
		return GET( m_iOK )
		&& GET( m_EventStart )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_COBO_DUNGEON_FIELD_ACK , obj, ks )
{
	return PUT( m_bStartUI )
		&& PUT( m_DungeonCountUI )
		&& PUT( m_FieldCountUI )
		&& PUT( m_DungeonCount )
		&& PUT( m_FieldMonsterKillCount )
		&& PUT( m_iOK )
		&& PUT( m_iRemaindTime )
		&& PUT( m_wstrPushTime )
		&& PUT( m_tPushTime )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_COBO_DUNGEON_FIELD_ACK , obj, ks )
{
	return GET( m_bStartUI )
		&& GET( m_DungeonCountUI )
		&& GET( m_FieldCountUI )
		&& GET( m_DungeonCount )
		&& GET( m_FieldMonsterKillCount )
		&& GET( m_iOK )
		&& GET( m_iRemaindTime )
		&& GET( m_wstrPushTime )
		&& GET( m_tPushTime )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT , obj, ks )
{
	return PUT( m_iDungeonClearCount )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_COBO_DUNGEON_CLEAR_COUNT_NOT , obj, ks )
{
	return GET( m_iDungeonClearCount )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT , obj, ks )
{
	return PUT( m_iFieldMonsterKillCount )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_COBO_FIELD_MONSTER_KILL_NOT , obj, ks )
{
	return GET( m_iFieldMonsterKillCount )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_COBO_ITEM_GIVE_CHEAT_NOT , obj, ks )
{
	return PUT( m_CoboEventITemGet )
		&& PUT( m_bNextDay )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_COBO_ITEM_GIVE_CHEAT_NOT , obj, ks )
{
	return GET( m_CoboEventITemGet )
		&& GET( m_bNextDay )
		;
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
SERIALIZE_DEFINE_PUT( KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT , obj, ks )
{
	return PUT( m_iValentineItemCount )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_NOT , obj, ks )
{
	return GET( m_iValentineItemCount )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_CHEAT_NOT , obj, ks )
{
	return PUT( m_iValentineItemCount )
		;
}
SERIALIZE_DEFINE_GET( KEGS_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_CHEAT_NOT , obj, ks )
{
	return GET( m_iValentineItemCount )
		;
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
SERIALIZE_DEFINE_PUT( KEGS_CASH_DIRECT_CHARGE_CN_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CASH_DIRECT_CHARGE_CN_REQ, obj, ks )
{
	return GET( m_iUserUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CASH_DIRECT_CHARGE_CN_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrToken )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CASH_DIRECT_CHARGE_CN_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrToken )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_CASH_DIRECT_CHARGE_CN_NOT, obj, ks )
{
	return PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KEGS_CASH_DIRECT_CHARGE_CN_NOT, obj, ks )
{
	return GET( m_iOK )
		;
}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#ifdef SERV_MANUFACTURE_PERIOD_FIX
SERIALIZE_DEFINE_PUT( KEGS_MANUFACTURE_PERIOD_SETTING_REQ, obj, ks )
{
	return PUT( m_iHouseID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_MANUFACTURE_PERIOD_SETTING_REQ, obj, ks )
{
	return GET( m_iHouseID )
		;
}
SERIALIZE_DEFINE_PUT( KEGS_MANUFACTURE_PERIOD_SETTING_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapPeriodGroup )
		&& PUT( m_iHouseID )
		;
}

SERIALIZE_DEFINE_GET( KEGS_MANUFACTURE_PERIOD_SETTING_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapPeriodGroup )
		&& GET( m_iHouseID )
		;
}
#endif //SERV_MANUFACTURE_PERIOD_FIX

#ifdef SERV_4TH_ANNIVERSARY_EVENT
SERIALIZE_DEFINE_PUT( KEGS_4TH_ANNIV_EVENT_REWARD_REQ, obj, ks )
{
	return PUT( m_iSeletedIndex )
		;
}

SERIALIZE_DEFINE_GET( KEGS_4TH_ANNIV_EVENT_REWARD_REQ, obj, ks )
{
	return GET( m_iSeletedIndex )
		;
}

SERIALIZE_DEFINE_PUT( KEGS_4TH_ANNIV_EVENT_REWARD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iSeletedIndex )
		;
}

SERIALIZE_DEFINE_GET( KEGS_4TH_ANNIV_EVENT_REWARD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iSeletedIndex )
		;
}
#endif // SERV_4TH_ANNIVERSARY_EVENT