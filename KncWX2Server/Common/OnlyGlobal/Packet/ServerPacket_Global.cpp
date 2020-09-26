#include <winsock2.h>
#include "ServerPacket.h"


#ifdef SERV_LOGIN_RESULT_INFO
SERIALIZE_DEFINE_PUT( KLoginResultInfo, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_uiServerUID )
		&& PUT( m_vecResultCount )
		;
}
SERIALIZE_DEFINE_GET( KLoginResultInfo, obj, ks )
{
	return GET( m_iServerGroupID )
		&& GET( m_uiServerUID )
		&& GET( m_vecResultCount )
		;
}
#endif SERV_LOGIN_RESULT_INFO

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
SERIALIZE_DEFINE_PUT( KELG_PCBANG_LOGOUT_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrIP )
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSockID )
		&& PUT( m_iGSUID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_GET( KELG_PCBANG_LOGOUT_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_wstrUserID )
		&& GET( m_wstrIP )
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSockID )
		&& GET( m_iGSUID )
#endif //SERV_COUNTRY_TH
		;
}


#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
SERIALIZE_DEFINE_PUT( KELG_ADMIN_DELETE_GUILD_ADD_NOT, obj, ks )
{
	return PUT( m_iGuildUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_DELETE_GUILD_ADD_NOT, obj, ks )
{
	return GET( m_iGuildUID )
		;
}
#endif

//{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
SERIALIZE_DEFINE_PUT( KELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_iRank )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_iRank )
		;
}

SERIALIZE_DEFINE_PUT( KELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_wstrNickName )
		;
}

SERIALIZE_DEFINE_PUT( KELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT, obj, ks )
{
	return PUT( m_iRankingType )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT, obj, ks )
{
	return GET( m_iRankingType )
		&& GET( m_iUnitUID )
		;
}
#endif SERV_DELETE_HENIR_RANKING
//}}

#ifdef SERV_NEW_PUNISHMENT

SERIALIZE_DEFINE_PUT( KPunishmentInfo, obj, ks )
{
	return PUT( m_iType )
		&& PUT( m_iStartTime )
		&& PUT( m_iDuration )
		;
}

SERIALIZE_DEFINE_GET( KPunishmentInfo, obj, ks )
{
	return GET( m_iType )
		&& GET( m_iStartTime )
		&& GET( m_iDuration )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_PUNISHMENT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecPunishment )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_PUNISHMENT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecPunishment )
		;
}

#endif SERV_NEW_PUNISHMENT

//====================================================================//
//====================================================================//
//====================================================================//
//{{ 2013.03.19 조효진 해외 공용 인증 시스템 구성
#ifdef SERV_GLOBAL_AUTH

//{{ 2010. 09. 10 조효진 Gameforge 인증을 위하여 SERIALIZE 작성
SERIALIZE_DEFINE_PUT( KEPUBLISHER_AUTHENTICATION_REQ, obj, ks )
{
	return PUT( m_iActorUID )
		&& PUT( m_wstrIP )
		&& PUT( m_wstrServiceAccountID )
		&& PUT( m_wstrServicePassword )
		&& PUT( m_iChannelingCode )
#ifdef SERV_KOG_OTP_VERIFY
		&& PUT( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_LOGIN_MAC_ADDRESS
		&& PUT( m_wstrMACAddress )
#endif // SERV_LOGIN_MAC_ADDRESS
#ifdef SERV_PUBLISHER_OTP
		&& PUT( m_wstrPublisherOTP )
#endif // SERV_PUBLISHER_OTP
#ifdef SERV_USE_DOMAIN_ACCOUNT
		&& PUT( m_wstrDomain )
#endif // SERV_USE_DOMAIN_ACCOUNT
#ifdef SERV_COUNTRY_CN
		&& PUT( m_uiRequestID )
		&& PUT( m_usZone )
		&& PUT( m_usGame )
#endif // SERV_COUNTRY_CN
		;
}


SERIALIZE_DEFINE_GET(  KEPUBLISHER_AUTHENTICATION_REQ, obj, ks )
{
	return GET( m_iActorUID )
		&& GET( m_wstrIP )
		&& GET( m_wstrServiceAccountID ) 
		&& GET( m_wstrServicePassword )
		&& GET( m_iChannelingCode )
#ifdef SERV_KOG_OTP_VERIFY
		&& GET( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_LOGIN_MAC_ADDRESS
		&& GET( m_wstrMACAddress )
#endif // SERV_LOGIN_MAC_ADDRESS
#ifdef SERV_PUBLISHER_OTP
		&& GET( m_wstrPublisherOTP )
#endif // SERV_PUBLISHER_OTP
#ifdef SERV_USE_DOMAIN_ACCOUNT
		&& GET( m_wstrDomain )
#endif // SERV_USE_DOMAIN_ACCOUNT
#ifdef SERV_COUNTRY_CN
		&& GET( m_uiRequestID ) 
		&& GET( m_usZone ) 
		&& GET( m_usGame )
#endif // SERV_COUNTRY_CN
		;
}


SERIALIZE_DEFINE_PUT( KEPUBLISHER_AUTHENTICATION_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_iResult ) 
		&& PUT( m_wstrID ) 
		&& PUT( m_uiPublisherUID )
		&& PUT( m_wstrIP )
		&& PUT( m_iChannelingCode )
#ifdef SERV_PURCHASE_TOKEN
		&& PUT( wstrPurchTok )
#endif // SERV_PURCHASE_TOKEN
		&& PUT( m_uiAccountType ) 
		&& PUT( m_uiAccountState )
#ifdef SERV_LOGIN_TOU
		&& PUT( m_strAgreementURL )
#endif //SERV_LOGIN_TOU
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrMasterID )
		&& PUT( m_wstrSocketID ) 
		&& PUT( m_bIsAKeyUser )
		&& PUT( m_wstrAkeyUserID )
#endif // SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_AUTHENTICATION_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iResult )
		&& GET( m_wstrID ) 
		&& GET( m_uiPublisherUID )
		&& GET( m_wstrIP )
		&& GET( m_iChannelingCode )
#ifdef SERV_PURCHASE_TOKEN
		&& GET( wstrPurchTok )
#endif // SERV_PURCHASE_TOKEN
		&& GET( m_uiAccountType ) 
		&& GET( m_uiAccountState )
#ifdef SERV_LOGIN_TOU
		&& GET( m_strAgreementURL )
#endif //SERV_LOGIN_TOU
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrMasterID )
		&& GET( m_wstrSocketID ) 
		&& GET( m_bIsAKeyUser )
		&& GET( m_wstrAkeyUserID )
#endif // SERV_COUNTRY_TH
		;
}
//}}


SERIALIZE_DEFINE_PUT( KDBE_CH_USER_GENERAL_LOGIN_REQ, obj, ks )
{
	return PUT( m_wstrServiceAccountID ) 
		&& PUT( m_wstrIP )
		&& PUT( m_uiPublisherUID )
		&& PUT( m_iChannelingCode )
#ifdef SERV_PURCHASE_TOKEN
		&& PUT( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
#ifdef SERV_KOG_OTP_VERIFY
		&& PUT( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
		&& PUT( m_wstrPassword )
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrMasterID )
		&& PUT( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		&& PUT( m_uiAccountType )
#endif // SERV_ANTI_ADDICTION_SYSTEM
#ifdef SERV_COUNTRY_PH
		&& PUT( m_usGarenaCyberCafe )
#endif //SERV_COUNTRY_PH
		;
};

SERIALIZE_DEFINE_GET( KDBE_CH_USER_GENERAL_LOGIN_REQ, obj, ks )
{
	return GET( m_wstrServiceAccountID ) 
		&& GET( m_wstrIP )
		&& GET( m_uiPublisherUID )
		&& GET( m_iChannelingCode )
#ifdef SERV_PURCHASE_TOKEN
		&& GET( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
#ifdef SERV_KOG_OTP_VERIFY
		&& GET( m_bServerUseKogOTP )
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_USE_KOG_ACCOUNTDB_PASSWORD
		&& GET( m_wstrPassword )
#endif //SERV_USE_KOG_ACCOUNTDB_PASSWORD
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrMasterID )
		&& GET( m_wstrSocketID )
#endif // SERV_COUNTRY_TH
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		&& GET( m_uiAccountType )
#endif // SERV_ANTI_ADDICTION_SYSTEM
#ifdef SERV_COUNTRY_PH
		&& GET( m_usGarenaCyberCafe )
#endif //SERV_COUNTRY_PH
		;
};

SERIALIZE_DEFINE_PUT( KDBE_CH_USER_GENERAL_LOGIN_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_wstrUserID ) 
		&& PUT( m_iUserUID ) 
		&& PUT( m_wstrOTP ) 
		&& PUT( m_uiPublisherUID )
		&& PUT( m_iChannelingCode )
#ifdef SERV_PURCHASE_TOKEN
		&& PUT( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
		;
};

SERIALIZE_DEFINE_GET( KDBE_CH_USER_GENERAL_LOGIN_ACK, obj, ks )
{
	return GET( m_iOK ) 
		&& GET( m_wstrUserID ) 
		&& GET( m_iUserUID ) 
		&& GET( m_wstrOTP )
		&& GET( m_uiPublisherUID )
		&& GET( m_iChannelingCode )
#ifdef SERV_PURCHASE_TOKEN
		&& GET( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
		;
};

SERIALIZE_DEFINE_PUT( KDBE_CH_USER_KOGOTP_LOGIN_REQ, obj, ks )
{
	return PUT( m_wstrServiceAccountID ) 
		&& PUT( m_uiPublisherUID )
		&& PUT( m_wstrOTP )
		&& PUT( m_wstrIP )
#ifdef SERV_PURCHASE_TOKEN
		&& PUT( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
#if defined( SERV_STEAM ) || defined( SERV_ALL_RENEWAL_SP )
		&& PUT( m_iChannelingCode )
#endif //( SERV_STEAM ) || ( SERV_ALL_RENEWAL_SP )
		;
};

SERIALIZE_DEFINE_GET( KDBE_CH_USER_KOGOTP_LOGIN_REQ, obj, ks )
{
	return GET( m_wstrServiceAccountID ) 
		&& GET( m_uiPublisherUID )
		&& GET( m_wstrOTP )
		&& GET( m_wstrIP )
#ifdef SERV_PURCHASE_TOKEN
		&& GET( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
#if defined( SERV_STEAM ) || defined( SERV_ALL_RENEWAL_SP )
		&& GET( m_iChannelingCode )
#endif //( SERV_STEAM ) || ( SERV_ALL_RENEWAL_SP )
		;
};

SERIALIZE_DEFINE_PUT( KDBE_CH_USER_KOGOTP_LOGIN_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_wstrUserID ) 
		&& PUT( m_iUserUID ) 
		&& PUT( m_wstrOTP ) 
		&& PUT( m_uiPublisherUID )
#ifdef SERV_PURCHASE_TOKEN
		&& PUT( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
		;
};

SERIALIZE_DEFINE_GET( KDBE_CH_USER_KOGOTP_LOGIN_ACK, obj, ks )
{
	return GET( m_iOK ) 
		&& GET( m_wstrUserID ) 
		&& GET( m_iUserUID ) 
		&& GET( m_wstrOTP )
		&& GET( m_uiPublisherUID )
#ifdef SERV_PURCHASE_TOKEN
		&& GET( m_wstrPurchaseTok )
#endif SERV_PURCHASE_TOKEN
		;
};


SERIALIZE_DEFINE_PUT( KELG_GAME_USER_AUTHENTICATE_REQ, obj, ks )
{
	return  PUT( m_kAuthenticateReq ) &&
		PUT( m_kNexonAccountInfo )
#ifdef SERV_EPAY_SYSTEM
		&& PUT( m_usZone )
		&& PUT( m_usGame )
#endif //SERV_EPAY_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KELG_GAME_USER_AUTHENTICATE_REQ, obj, ks )
{
	return  GET( m_kAuthenticateReq ) &&
		GET( m_kNexonAccountInfo )
#ifdef SERV_EPAY_SYSTEM
		&& GET( m_usZone )
		&& GET( m_usGame )
#endif //SERV_EPAY_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KELG_GAME_USER_AUTHENTICATE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kAccountInfo )
		&& PUT( m_kNexonAccountInfo )
		&& PUT( m_cHackingUserType )
		&& PUT( m_uiGiantAccountType )
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& PUT( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
#ifdef SERV_EPAY_SYSTEM
		&& PUT( m_usZone )
		&& PUT( m_usGame )
#endif //SERV_EPAY_SYSTEM
		;
}

SERIALIZE_DEFINE_GET( KELG_GAME_USER_AUTHENTICATE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kAccountInfo )
		&& GET( m_kNexonAccountInfo )
		&& GET( m_cHackingUserType )
		&& GET( m_uiGiantAccountType )
		//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
		&& GET( m_mapAccCountInfo )
#endif SERV_ACCOUNT_COUNT
		//}}
#ifdef SERV_EPAY_SYSTEM
		&& GET( m_usZone )
		&& GET( m_usGame )
#endif //SERV_EPAY_SYSTEM
		;
}

SERIALIZE_DEFINE_PUT( KEPUBLISHER_REPORT_HACK_USER_NOT, obj, ks )
{
	return PUT( m_iUserUID ) 
		&& PUT( m_wstrUserID ) 
		&& PUT( m_iUnitUID ) 
		&& PUT( m_wstrNickName ) 
		&& PUT( m_iServerUID ) 
		&& PUT( m_strIP )
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_REPORT_HACK_USER_NOT, obj, ks )
{
	return GET( m_iUserUID ) 
		&& GET( m_wstrUserID ) 
		&& GET( m_iUnitUID ) 
		&& GET( m_wstrNickName ) 
		&& GET( m_iServerUID ) 
		&& GET( m_strIP )
		;
}

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_COUNTRY_TH

//{{ 허상형 : [2012/6/19] //	AsiaSoft 인증
SERIALIZE_DEFINE_GET( KEAS_AUTH_LOGIN_REQ, obj, ks )
{
	return  GET( m_iUserUID )
		&&	GET( m_wstrUserID ) 
		&&	GET( m_wstrPassword ) 
		&&	GET( m_wstrDomain )
		&&	GET( m_wstrIP )
		&&	GET( m_wstrSockID )
		&&	GET( m_wstrAkey )
		;
};

SERIALIZE_DEFINE_PUT( KEAS_AUTH_LOGIN_REQ, obj, ks )
{
	return  PUT( m_iUserUID ) 
		&&	PUT( m_wstrUserID ) 
		&&	PUT( m_wstrPassword ) 
		&&	PUT( m_wstrDomain ) 
		&&	PUT( m_wstrIP ) 
		&&	PUT( m_wstrSockID )
		&&	PUT( m_wstrAkey )
		;
};

SERIALIZE_DEFINE_GET( KEAS_AUTH_LOGIN_ACK, obj, ks )
{
	return  GET( m_iOK )
		&&	GET( m_iRecvCode )
		&&	GET( m_wstrRecvMsg )
		&&	GET( m_wstrGameID )
		&&	GET( m_iAkeyFlag )
		&&	GET( m_wstrAkeyUserID )
		&&	GET( m_wstrSockID )
		&&	GET( m_wstrAkey )
		;
}

SERIALIZE_DEFINE_PUT( KEAS_AUTH_LOGIN_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_iRecvCode )
		&&	PUT( m_wstrRecvMsg )
		&&	PUT( m_wstrGameID )
		&&	PUT( m_iAkeyFlag )
		&&	PUT( m_wstrAkeyUserID )
		&&	PUT( m_wstrSockID )
		&&	PUT( m_wstrAkey )
		;
};

SERIALIZE_DEFINE_GET( KEAS_GET_IPBONUS_REQ, obj, ks )
{
	return  GET( m_iUserUID )
		&&	GET( m_wstrMasterID )
		&&	GET( m_wstrIP )
		&&	GET( m_bIsLogin )
		&&	GET( m_wstrSockID )
		&&	GET( m_iGSUID )
		;
}

SERIALIZE_DEFINE_PUT( KEAS_GET_IPBONUS_REQ, obj, ks )
{
	return  PUT( m_iUserUID )
		&&	PUT( m_wstrMasterID )
		&&	PUT( m_wstrIP )
		&&	PUT( m_bIsLogin )
		&&	PUT( m_wstrSockID )
		&&	PUT( m_iGSUID )
		;
};

SERIALIZE_DEFINE_GET( KEAS_GET_IPBONUS_ACK, obj, ks )
{
	return  GET( m_iOK )
		&&	GET( m_iRecvCode )
		&&	GET( m_wstrRecvMsg )
		&&	GET( m_iPCBangLevel )
		&&	GET( m_wstrSockID )
		;
}

SERIALIZE_DEFINE_PUT( KEAS_GET_IPBONUS_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_iRecvCode )
		&&	PUT( m_wstrRecvMsg )
		&&	PUT( m_iPCBangLevel )
		&&	PUT( m_wstrSockID )
		;
};

SERIALIZE_DEFINE_GET( KEAS_AUTH_AKEY_REQ, obj, ks )
{
	return  GET( m_wstrDomain )
		&&	GET( m_wstrAkeyUserID )
		&&	GET( m_wstrAkey )
		&&	GET( m_wstrIP )
		&&	GET( m_iServerUID )
		&&	GET( m_kEPUBLISHER_AUTHENTICATION_ACK )
		;
}

SERIALIZE_DEFINE_PUT( KEAS_AUTH_AKEY_REQ, obj, ks )
{
	return  PUT( m_wstrDomain )
		&&	PUT( m_wstrAkeyUserID )
		&&	PUT( m_wstrAkey )
		&&	PUT( m_wstrIP )
		&&	PUT( m_iServerUID )
		&&	PUT( m_kEPUBLISHER_AUTHENTICATION_ACK )
		;
};

SERIALIZE_DEFINE_GET( KEAS_AUTH_AKEY_ACK, obj, ks )
{
	return  GET( m_iOk )
		&&	GET( m_wstrRecvMessage )
		&&	GET( m_iServerUID )
		&&	GET( m_kEPUBLISHER_AUTHENTICATION_ACK )
		;
}

SERIALIZE_DEFINE_PUT( KEAS_AUTH_AKEY_ACK, obj, ks )
{
	return  PUT( m_iOk )
		&&	PUT( m_wstrRecvMessage )
		&&	PUT( m_iServerUID )
		&&	PUT( m_kEPUBLISHER_AUTHENTICATION_ACK )
		;
};

SERIALIZE_DEFINE_GET( KDBE_AS_USER_LOGIN_REQ, obj, ks )
{
	return  GET( m_kAuthenticateReq )
		&&	GET( m_kNexonAccountInfo )
		&&	GET( m_wstrMasterID )
		&&	GET( m_wstrSocketID )
		&&	GET( m_wstrOTP )
		&&	GET( m_bLogin )
		;
};

SERIALIZE_DEFINE_PUT( KDBE_AS_USER_LOGIN_REQ, obj, ks )
{
	return  PUT( m_kAuthenticateReq )
		&&	PUT( m_kNexonAccountInfo )
		&&	PUT( m_wstrMasterID )
		&&	PUT( m_wstrSocketID )
		&&	PUT( m_wstrOTP )
		&&	PUT( m_bLogin )
		;
};

SERIALIZE_DEFINE_PUT( KDBE_AS_USER_LOGIN_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrSocketID )
		&& PUT( m_iUserUID )
		&& PUT( m_wstrOTP )
		;
}

SERIALIZE_DEFINE_GET( KDBE_AS_USER_LOGIN_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrUserID )
		&& GET( m_wstrSocketID )
		&& GET( m_iUserUID )
		&& GET( m_wstrOTP )
		;
}
//}} 허상형 : [2012/6/19] //	AsiaSoft 인증



#endif // SERV_COUNTRY_TH


//#ifdef SERV_COUNTRY_CN

// 2010.07.07 박교현 / 자이언트 HLServer 인증
SERIALIZE_DEFINE_PUT( KEGIANT_COMMON_INITIALIZE_REQ, obj, ks )
{
	return PUT( m_wstrIP )
		&& PUT( m_usPort )
		;
};

SERIALIZE_DEFINE_GET( KEGIANT_COMMON_INITIALIZE_REQ, obj, ks )
{
	return GET( m_wstrIP )
		&& GET( m_usPort )
		;
};

SERIALIZE_DEFINE_PUT( KEGIANT_COMMON_INITIALIZE_ACK, obj, ks )
{
	return PUT( m_usZone )
		&& PUT( m_usGame )
		&& PUT( m_wstrZoneName )
		&& PUT( m_byteNetType )
		;
};

SERIALIZE_DEFINE_GET( KEGIANT_COMMON_INITIALIZE_ACK, obj, ks )
{
	return GET( m_usZone )
		&& GET( m_usGame )
		&& GET( m_wstrZoneName )
		&& GET( m_byteNetType )
		;
};

SERIALIZE_DEFINE_PUT( KEGIANT_COMMON_NULL_SERVER, obj, ks )
{
	return true;
};

SERIALIZE_DEFINE_GET( KEGIANT_COMMON_NULL_SERVER, obj, ks )
{
	return true;
};

SERIALIZE_DEFINE_PUT( KEGIANT_COMMON_NULL_CLIENT, obj, ks )
{
	return true;
};

SERIALIZE_DEFINE_GET( KEGIANT_COMMON_NULL_CLIENT, obj, ks )
{
	return true;
};


SERIALIZE_DEFINE_PUT( KEGIANT_AUTH_LOGIN_SUCCESS, obj, ks )
{
	return PUT( m_uiRequestID)
		&& PUT( m_uiUID )
		&& PUT( m_wstrName )
		&& PUT( m_uiType )
		&& PUT( m_wstrEMail )
		&& PUT( m_uiState )
		&& PUT( m_uiBindType )
		&& PUT( m_wstrSuperpasswd )
		&& PUT( m_wstrMobileAccount )
		;
};

SERIALIZE_DEFINE_GET( KEGIANT_AUTH_LOGIN_SUCCESS, obj, ks )
{
	return GET( m_uiRequestID)
		&& GET( m_uiUID )
		&& GET( m_wstrName )
		&& GET( m_uiType )
		&& GET( m_wstrEMail )
		&& GET( m_uiState )
		&& GET( m_uiBindType )
		&& GET( m_wstrSuperpasswd )
		&& GET( m_wstrMobileAccount )
		;
};

SERIALIZE_DEFINE_PUT( KEGIANT_AUTH_LOGIN_FAIL, obj, ks )
{
	return PUT( m_uiRequestID )
		&& PUT( m_iReturnCode )
		&& PUT( m_wstrName )
		&& PUT( m_wstrName )
		&& PUT( m_uiKey )
		&& PUT( m_wstrCoordinate )
		;
};

SERIALIZE_DEFINE_GET( KEGIANT_AUTH_LOGIN_FAIL, obj, ks )
{
	return GET( m_uiRequestID )
		&& GET( m_iReturnCode )
		&& GET( m_wstrName )
		&& GET( m_wstrName )
		&& GET( m_uiKey )
		&& GET( m_wstrCoordinate )
		;
};


SERIALIZE_DEFINE_PUT( KDBE_GIANT_USER_LOGIN_REQ, obj, ks )
{
	return  PUT( m_bLogin )
		&&	PUT( m_wstrServiceAccountID ) 
		&&	PUT( m_uiGiantUID )
		&&	PUT( m_wstrOTP )
		&&	PUT( m_uiAccountType )
		&&	PUT( m_uiAccountState )
		;
};

SERIALIZE_DEFINE_GET( KDBE_GIANT_USER_LOGIN_REQ, obj, ks )
{
	return  GET( m_bLogin )
		&&	GET( m_wstrServiceAccountID ) 
		&&	GET( m_uiGiantUID )
		&&	GET( m_wstrOTP )
		&&	GET( m_uiAccountType )
		&&	GET( m_uiAccountState )
		;
};


SERIALIZE_DEFINE_PUT( KDBE_GIANT_USER_LOGIN_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		&&	PUT( m_wstrUserID ) 
		&&	PUT( m_iUserUID ) 
		&&	PUT( m_wstrOTP ) 
		&&	PUT( m_uiGiantUID )
		&&	PUT( m_uiAccountType )
		;
};

SERIALIZE_DEFINE_GET( KDBE_GIANT_USER_LOGIN_ACK, obj, ks )
{
	return  GET( m_iOK ) 
		&&	GET( m_wstrUserID ) 
		&&	GET( m_iUserUID ) 
		&&	GET( m_wstrOTP )
		&&	GET( m_uiGiantUID )
		&&	GET( m_uiAccountType )
		;
};



//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
SERIALIZE_DEFINE_PUT( KDBE_GET_ANTI_ADDICTION_INFO_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		&&	PUT( m_iPlayTime ) 
		&&	PUT( m_iRestTime ) 
		&&	PUT( m_wstrLastLogoutTime )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ANTI_ADDICTION_INFO_ACK, obj, ks )
{
	return	GET( m_iOK ) 
		&&	GET( m_iPlayTime ) 
		&&	GET( m_iRestTime ) 
		&&	GET( m_wstrLastLogoutTime )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_ANTI_ADDICTION_INFO, obj, ks )
{
	return	PUT( m_iUserUID )
		&&	PUT( m_iPlayTime )
		&&	PUT( m_iRestTime )
		&&	PUT( m_wstrLastLogoutTime )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_ANTI_ADDICTION_INFO, obj, ks )
{
	return  GET( m_iUserUID )
		&&	GET( m_iPlayTime )
		&&	GET( m_iRestTime )
		&&	GET( m_wstrLastLogoutTime )
		;
}
#endif SERV_ANTI_ADDICTION_SYSTEM
//}}


SERIALIZE_DEFINE_PUT( KEPUBLISHER_BILLING_BALANCE_REQ, obj, ks )
{
	return PUT( m_uiPublisherUID )
		&& PUT( m_uiActorUID )
		&& PUT( m_wstrAccount )
		&& PUT( m_wstrIP )
#ifdef SERV_COUNTRY_CN
		&& PUT( m_wstrTID )
		&& PUT( m_iAT )
#endif // SERV_COUNTRY_CN
#ifdef SERV_COUNTRY_IN
		&& PUT( m_wstrNickName )
#endif SERV_COUNTRY_IN
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_BILLING_BALANCE_REQ, obj, ks )
{
	return GET( m_uiPublisherUID )
		&& GET( m_uiActorUID )
		&& GET( m_wstrAccount )
		&& GET( m_wstrIP )
#ifdef SERV_COUNTRY_CN
		&& GET( m_wstrTID )
		&& GET( m_iAT )
#endif // SERV_COUNTRY_CN
#ifdef SERV_COUNTRY_IN
		&& GET( m_wstrNickName )
#endif SERV_COUNTRY_IN
		;
}

SERIALIZE_DEFINE_PUT( KEPUBLISHER_BILLING_BALANCE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_ulBalance )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_iCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_BILLING_BALANCE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_ulBalance )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_iCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_PUT( KELG_VERIFY_GIANT_MATRIX_CARD_REQ, obj, ks )
{
	return	PUT( m_wstrID )
		&&	PUT( m_wstrMatrix )
		&&	PUT( m_uiGiantMatrixKey )
		;
}

SERIALIZE_DEFINE_GET( KELG_VERIFY_GIANT_MATRIX_CARD_REQ, obj, ks )
{
	return	GET( m_wstrID )
		&&	GET( m_wstrMatrix )
		&&	GET( m_uiGiantMatrixKey )
		;
}


SERIALIZE_DEFINE_PUT( KEGIANT_BILLING_RETURN, obj, ks )
{
	return PUT( m_wstrTID )
		&& PUT( m_iBalance )
		&& PUT( m_iBonus )
		&& PUT( m_iHadFilled )
		&& PUT( m_iRet )
		&& PUT( m_iSubat )
		&& PUT( m_iAT )
		&& PUT( m_iPoint )
		&& PUT( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_BILLING_RETURN, obj, ks )
{
	return GET( m_wstrTID )
		&& GET( m_iBalance )
		&& GET( m_iBonus )
		&& GET( m_iHadFilled )
		&& GET( m_iRet )
		&& GET( m_iSubat )
		&& GET( m_iAT )
		&& GET( m_iPoint )
		&& GET( m_uiGiantUID )
		;
}



SERIALIZE_DEFINE_PUT( KEGIANT_BILLING_USE_CARD_CMD, obj, ks )
{
	return PUT( m_sSource )
		&& PUT( m_wstrAccount )
		&& PUT( m_wstrTID )
		&& PUT( m_wstrCardID )
		&& PUT( m_wstrPasswd )
		&& PUT( m_wstrIP )
		&& PUT( m_uiUID )
		&& PUT( m_iAT )
		&& PUT( m_uiConTempID )
		&& PUT( m_uiOperateUID )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_BILLING_USE_CARD_CMD, obj, ks )
{
	return GET( m_sSource )
		&& GET( m_wstrAccount )
		&& GET( m_wstrTID )
		&& GET( m_wstrCardID )
		&& GET( m_wstrPasswd )
		&& GET( m_wstrIP )
		&& GET( m_uiUID )
		&& GET( m_iAT )
		&& GET( m_uiConTempID )
		&& GET( m_uiOperateUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_BILLING_USE_CARD_RET_CMD, obj, ks )
{
	return PUT( m_sSource )
		&& PUT( m_wstrAccount )
		&& PUT( m_wstrTID )
		&& PUT( m_wstrCardID )
		&& PUT( m_wstrPasswd )
		&& PUT( m_wstrIP )
		&& PUT( m_iPoint )
		&& PUT( m_iRet )
		&& PUT( m_uiUID )
		&& PUT( m_iAT )
		&& PUT( m_uiConTempID )
		&& PUT( m_uiOperateUID )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_BILLING_USE_CARD_RET_CMD, obj, ks )
{
	return GET( m_sSource )
		&& GET( m_wstrAccount )
		&& GET( m_wstrTID )
		&& GET( m_wstrCardID )
		&& GET( m_wstrPasswd )
		&& GET( m_wstrIP )
		&& GET( m_iPoint )
		&& GET( m_iRet )
		&& GET( m_uiUID )
		&& GET( m_iAT )
		&& GET( m_uiConTempID )
		&& GET( m_uiOperateUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_BILLING_CONSUME_REQ, obj, ks )
{
	return PUT( m_uiGiantUID )
		&& PUT( m_iPoint )
		&& PUT( m_iSource )
		&& PUT( m_wstrTID )
		&& PUT( m_wstrRemark )
		&& PUT( m_wstrIP )
		&& PUT( m_iAT )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_BILLING_CONSUME_REQ, obj, ks )
{
	return GET( m_uiGiantUID )
		&& GET( m_iPoint )
		&& GET( m_iSource )
		&& GET( m_wstrTID )
		&& GET( m_wstrRemark )
		&& GET( m_wstrIP )
		&& GET( m_iAT )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_BILLING_CONSUME_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iBalance )
		&& PUT( m_iBonus )
		&& PUT( m_bHasFilled )
		&& PUT( m_iPoint )
		&& PUT( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_BILLING_CONSUME_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iBalance )
		&& PUT( m_iBonus )
		&& PUT( m_bHasFilled )
		&& PUT( m_iPoint )
		&& PUT( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_BILLING_PCARD_CMD, obj, ks )
{
	return PUT( m_uiGiantUID )
		&& PUT( m_iSource )
		&& PUT( m_wstrTID )
		&& PUT( m_wstrPCardID )
		&& PUT( m_wstrIP )
		&& PUT( m_iAT )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_BILLING_PCARD_CMD, obj, ks )
{
	return GET( m_uiGiantUID )
		&& GET( m_iSource )
		&& GET( m_wstrTID )
		&& GET( m_wstrPCardID )
		&& GET( m_wstrIP )
		&& GET( m_iAT )
		;
}


SERIALIZE_DEFINE_PUT( KEGIANT_BILLING_SCARD_CMD, obj, ks )
{
	return PUT( m_uiGiantUID )
		&& PUT( m_iSource )
		&& PUT( m_wstrTID )
		&& PUT( m_wstrSCardID )
		&& PUT( m_wstrIP )
		&& PUT( m_iAT )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_BILLING_SCARD_CMD, obj, ks )
{
	return GET( m_uiGiantUID )
		&& GET( m_iSource )
		&& GET( m_wstrTID )
		&& GET( m_wstrSCardID )
		&& GET( m_wstrIP )
		&& GET( m_iAT )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG, obj, ks )
{
	return  PUT( m_usRegType ) 
		&&	PUT( m_usServerID ) 
		&&	PUT( m_uiGiantUID )
		&&	PUT( m_usZone )
		&&	PUT( m_usGame )
		&&	PUT( m_wstrName )
		&&	PUT( m_uiRoleUID )
		&&	PUT( m_usState )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG, obj, ks )
{
	return	GET( m_usRegType ) 
		&&	GET( m_usServerID ) 
		&&	GET( m_uiGiantUID )
		&&	GET( m_usZone )
		&&	GET( m_usGame )
		&&	GET( m_wstrName )
		&&	GET( m_uiRoleUID )
		&&	GET( m_usState )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_CREATE_UNIT_REQ, obj, ks )
{
	return  PUT( m_iActorUID ) 
		&&	PUT( m_uiGiantUID )
		&&	PUT( m_CreateUnitReqPacket )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_CREATE_UNIT_REQ, obj, ks )
{
	return	GET( m_iActorUID )
		&&	GET( m_uiGiantUID )
		&&	GET( m_CreateUnitReqPacket )
		;
}


SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_CREATE_UNIT_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		&&	PUT( m_CreateUnitReqPacket )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_CREATE_UNIT_ACK, obj, ks )
{
	return	GET( m_iOK )
		&&	GET( m_CreateUnitReqPacket )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_DELETE_UNIT_REQ, obj, ks )
{
	return	PUT( m_wstrNickName )
		&&	PUT( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_DELETE_UNIT_REQ, obj, ks )
{
	return	GET( m_wstrNickName )
		&&	GET( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_DELETE_UNIT_ACK, obj, ks )
{
	return	PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_DELETE_UNIT_ACK, obj, ks )
{
	return	GET( m_iOK )
		;
}

SERIALIZE_DEFINE_PUT( KGiantDeletedUnitInfo, obj, ks )
{
	return	PUT( m_wstrNickName )
		&&	PUT( m_iUnitUID )
		&&	PUT( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_GET( KGiantDeletedUnitInfo, obj, ks )
{
	return	GET( m_wstrNickName )
		&&	GET( m_iUnitUID )
		&&	GET( m_uiGiantUID )
		;
}
// 현재 사용하는 곳 없음 주석 처리 
// SERIALIZE_DEFINE_PUT( KDBE_INSERT_GIANT_DELETE_UNIT_REQ, obj, ks )
// {
// 	return	PUT( m_DeletedUnitInfo )
// 		;
// };
// 
// SERIALIZE_DEFINE_GET( KDBE_INSERT_GIANT_DELETE_UNIT_REQ, obj, ks )
// {
// 	return	GET( m_DeletedUnitInfo )
// 		;
// };

// 현재 사용하는 곳 없음 주석 처리 
// SERIALIZE_DEFINE_PUT( KDBE_UPDATE_GIANT_DELETE_UNIT_NOT, obj, ks )
// {
// 	return	PUT( m_wstrNickName )
// 		&&	PUT( m_uiGiantUID )
// 		;
// };
// 
// SERIALIZE_DEFINE_GET( KDBE_UPDATE_GIANT_DELETE_UNIT_NOT, obj, ks )
// {
// 	return	GET( m_wstrNickName )
// 		&&	GET( m_uiGiantUID )
// 		;
// };


SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_CREATE_GUILD_REQ, obj, ks )
{
	return	PUT( m_iActorUID )
		&&	PUT( m_uiGiantUID )
		&&	PUT( m_CreateGuildReqPacket )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_CREATE_GUILD_REQ, obj, ks )
{
	return	GET( m_iActorUID )
		&&	GET( m_uiGiantUID )
		&&	GET( m_CreateGuildReqPacket )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_CREATE_GUILD_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		&&	PUT( m_CreateGuildReqPacket )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_CREATE_GUILD_ACK, obj, ks )
{
	return	GET( m_iOK )
		&&	GET( m_CreateGuildReqPacket )
		;
}


SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_DELETE_GUILD_REQ, obj, ks )
{
	return  PUT( m_wstrGuildName ) 
		&&	PUT( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_DELETE_GUILD_REQ, obj, ks )
{
	return	GET( m_wstrGuildName )
		&&	GET( m_uiGiantUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_ROLEREG_DELETE_GUILD_ACK, obj, ks )
{
	return  PUT( m_iOK ) 
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_ROLEREG_DELETE_GUILD_ACK, obj, ks )
{
	return	GET( m_iOK )
		;
}

// 현재 사용하는 곳 없음. 주석처리
// SERIALIZE_DEFINE_PUT( KDBE_GET_GIANT_DELETED_UNIT_LIST_ACK, obj, ks )
// {
// 	return	PUT( m_vecDeletedUnitInfo )
// 		;
// }
// 
// SERIALIZE_DEFINE_GET( KDBE_GET_GIANT_DELETED_UNIT_LIST_ACK, obj, ks )
// {
// 	return	GET( m_vecDeletedUnitInfo )
// 		;
//

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_GET_CCU_REQ, obj, ks )
{
	return  PUT( m_TimeStamp ) 
		;
};

SERIALIZE_DEFINE_GET( KEGIANT_INFO_GET_CCU_REQ, obj, ks )
{
	return  GET( m_TimeStamp ) 
		;
};

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_GET_TOTAL_CCU_ACK, obj, ks )
{
	return  PUT( m_TimeStamp ) 
		&&	PUT( m_usServerID ) 
		&&	PUT( m_usServerType ) 
		&&	PUT( m_usZone ) 
		&&	PUT( m_usGame ) 
		&&	PUT( m_strZoneName ) 
		&&	PUT( m_uiCCU ) 
		;
};

SERIALIZE_DEFINE_GET( KEGIANT_INFO_GET_TOTAL_CCU_ACK, obj, ks )
{
	return  GET( m_TimeStamp ) 
		&&	GET( m_usServerID ) 
		&&	GET( m_usServerType ) 
		&&	GET( m_usZone ) 
		&&	GET( m_usGame ) 
		&&	GET( m_strZoneName ) 
		&&	GET( m_uiCCU ) 
		;
};

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_GET_CHANNEL_CCU_ACK, obj, ks )
{
	return  PUT( m_TimeStamp ) 
		&&	PUT( m_usZone ) 
		&&	PUT( m_usGame ) 
		//&&	PUT( m_uiCountryNum ) 
		//&&	PUT( m_uiCountryID ) 
		//&&	PUT( m_uiCountryCCU ) 
		&&	PUT( m_mapChannelCCU )
		;
};

SERIALIZE_DEFINE_GET( KEGIANT_INFO_GET_CHANNEL_CCU_ACK, obj, ks )
{
	return  GET( m_TimeStamp ) 
		&&	GET( m_usZone ) 
		&&	GET( m_usGame ) 
		//&&	GET( m_uiCountryNum ) 
		//&&	GET( m_uiCountryID ) 
		//&&	GET( m_uiCountryCCU ) 
		&&	GET( m_mapChannelCCU )
		;
};

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_CHAT_NOT, obj, ks )
{
	return  PUT( m_usZone ) 
		&&	PUT( m_usGame ) 
		&&	PUT( m_wstrUserName ) 
		&&	PUT( m_wstrGMName ) 
		&&	PUT( m_uiCountryID ) 
		&&	PUT( m_uiSceneID ) 
		&&	PUT( m_uiType ) 
		&&	PUT( m_wstrContent ) 
		&&	PUT( m_wstrServer ) 
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_INFO_CHAT_NOT, obj, ks )
{
	return  GET( m_usZone ) 
		&&	GET( m_usGame ) 
		&&	GET( m_wstrUserName ) 
		&&	GET( m_wstrGMName ) 
		&&	GET( m_uiCountryID ) 
		&&	GET( m_uiSceneID ) 
		&&	GET( m_uiType ) 
		&&	GET( m_wstrContent ) 
		&&	GET( m_wstrServer ) 
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_PUNISH_REQ, obj, ks )
{
	return  PUT( m_usZone ) 
		&&	PUT( m_usGame ) 
		&&	PUT( m_wstrUserName ) 
		&&	PUT( m_wstrCountryName ) 
		&&	PUT( m_wstrServerName ) 
		&&	PUT( m_wstrGMName ) 
		&&	PUT( m_wstrReason ) 
		&&	PUT( m_usOperation ) 
		&&	PUT( m_iDelay ) 
		&&	PUT( m_sLevel ) 
		&&	PUT( m_uiGiantUID ) 
		&&	PUT( m_uiStartTime ) 
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_INFO_PUNISH_REQ, obj, ks )
{
	return  GET( m_usZone ) 
		&&	GET( m_usGame ) 
		&&	GET( m_wstrUserName ) 
		&&	GET( m_wstrCountryName ) 
		&&	GET( m_wstrServerName ) 
		&&	GET( m_wstrGMName ) 
		&&	GET( m_wstrReason ) 
		&&	GET( m_usOperation ) 
		&&	GET( m_iDelay ) 
		&&	GET( m_sLevel ) 
		&&	GET( m_uiGiantUID ) 
		&&	GET( m_uiStartTime ) 
		;
}


#ifdef SERV_INFOSERVER_ADD_WORK
SERIALIZE_DEFINE_PUT( KEGIANT_INFO_USERONLINE_REQ, obj, ks )
{
	return  PUT( m_usZone ) 
		&&	PUT( m_usGame ) 
		&&	PUT( m_wstrUserName ) 
		&&	PUT( m_wstrGMName ) 
		&&	PUT( m_bUserOnline ) 
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_INFO_USERONLINE_REQ, obj, ks )
{
	return  GET( m_usZone ) 
		&&	GET( m_usGame ) 
		&&	GET( m_wstrUserName ) 
		&&	GET( m_wstrGMName ) 
		&&	GET( m_bUserOnline )  
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_USERONLINE_ACK, obj, ks )
{
	return  PUT( m_wstrUserName ) 
		&&	PUT( m_wstrGMName ) 
		&&	PUT( m_bUserOnline ) 
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_INFO_USERONLINE_ACK, obj, ks )
{
	return  GET( m_wstrUserName ) 
		&&	GET( m_wstrGMName ) 
		&&	GET( m_bUserOnline ) 
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_REQUESTID_REQ, obj, ks )
{
	return  PUT( m_usZone ) 
		&&	PUT( m_usGame ) 
		&&	PUT( m_wstrUserName ) 
		&&	PUT( m_wstrGMName ) 
		&&	PUT( m_uiGiantUID ) 
		&&	PUT( m_wstrAccount ) 
		&&	PUT( m_bWrite ) 
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_INFO_REQUESTID_REQ, obj, ks )
{
	return  GET( m_usZone ) 
		&&	GET( m_usGame ) 
		&&	GET( m_wstrUserName ) 
		&&	GET( m_wstrGMName ) 
		&&	GET( m_uiGiantUID ) 
		&&	GET( m_wstrAccount ) 
		&&	GET( m_bWrite ) 
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_INFO_REQUESTID_ACK, obj, ks )
{
	return  PUT( m_wstrUserName ) 
		&&	PUT( m_wstrGMName ) 
		&&	PUT( m_uiGiantUID ) 
		&&	PUT( m_wstrAccount ) 
		&&	PUT( m_bWrite ) 
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_INFO_REQUESTID_ACK, obj, ks )
{
	return  GET( m_wstrUserName ) 
		&&	GET( m_wstrGMName ) 
		&&	GET( m_uiGiantUID ) 
		&&	GET( m_wstrAccount ) 
		&&	GET( m_bWrite ) 
		;
}


SERIALIZE_DEFINE_PUT( KELG_CHAT_OBSERVATION_NOT, obj, ks )
{
	return	PUT( m_iServerUID )
		&&	PUT( m_cChatPacketType )
		&&	PUT( m_wstrSenderUnitNickName )
		&&	PUT( m_wstrMsg )
		;
}

SERIALIZE_DEFINE_GET( KELG_CHAT_OBSERVATION_NOT, obj, ks )
{
	return	GET( m_iServerUID )
		&&	GET( m_cChatPacketType )
		&&	GET( m_wstrSenderUnitNickName )
		&&	GET( m_wstrMsg )
		;
}
#endif //SERV_INFOSERVER_ADD_WORK

//#endif // SERV_COUNTRY_CN
//////////////////////////////////////////////////////////////////////////

SERIALIZE_DEFINE_PUT( KEPUBLISHER_SECURITY_AUTH_REQ, obj, ks )
{
	return PUT( m_iSecurityAuthType )
		&& PUT( m_iUserUID )
		&& PUT( m_wstrUserID )
		&& PUT( m_uiPublisherUID )
		&& PUT( m_wstrIP )
#ifdef SERV_PUBLISHER_MATRIXKEY
		&& PUT( m_uiMatrixKey )
		&& PUT( m_wstrMatrix )
#endif // SERV_PUBLISHER_MATRIXKEY
#ifdef SERV_COUNTRY_CN
		&& PUT( m_uiRequestID )
		&& PUT( m_usZone )
		&& PUT( m_usGame )
#endif // SERV_COUNTRY_CN
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_SECURITY_AUTH_REQ, obj, ks )
{
	return GET( m_iSecurityAuthType )
		&& GET( m_iUserUID )
		&& GET( m_wstrUserID )
		&& GET( m_uiPublisherUID )
		&& GET( m_wstrIP )
#ifdef SERV_PUBLISHER_MATRIXKEY
		&& GET( m_uiMatrixKey )
		&& GET( m_wstrMatrix )
#endif // SERV_PUBLISHER_MATRIXKEY
#ifdef SERV_COUNTRY_CN
		&& GET( m_uiRequestID )
		&& GET( m_usZone )
		&& GET( m_usGame )
#endif // SERV_COUNTRY_CN
		;
}

SERIALIZE_DEFINE_PUT( KEPUBLISHER_SECURITY_AUTH_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_strSecondSecurityURL )
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_SECURITY_AUTH_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_strSecondSecurityURL )
		;
}


SERIALIZE_DEFINE_PUT( KEPUBLISHER_SECURITY_INFO_NOT, obj, ks )
{
	return PUT( m_iSecurityInfoType )
		&& PUT( m_uiMatrixKey )
		&& PUT( m_wstrCoordinate )
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_SECURITY_INFO_NOT, obj, ks )
{
	return GET( m_iSecurityInfoType )
		&& GET( m_uiMatrixKey )
		&& GET( m_wstrCoordinate )
		;
}




SERIALIZE_DEFINE_PUT( KEPUBLISHER_REG_USER_REQ, obj, ks )
{
	return PUT( m_iActorUID )
		&& PUT( m_wstrServiceAccountID )
		&& PUT( m_wstrServicePassword )
		&& PUT( m_strIP )
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSockID )
		&& PUT( m_iGSUID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_REG_USER_REQ, obj, ks )
{
	return GET( m_iActorUID )
		&& GET( m_wstrServiceAccountID )
		&& GET( m_wstrServicePassword )
		&& GET( m_strIP )
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSockID )
		&& GET( m_iGSUID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_PUT( KEPUBLISHER_REG_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrServiceAccountID )
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		&& PUT( m_bIsPcBang )
		&& PUT( m_iPCBangType )
		&& PUT( m_iCheckState )
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSockID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_REG_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrServiceAccountID )
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		&& GET( m_bIsPcBang )
		&& GET( m_iPCBangType )
		&& GET( m_iCheckState )
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSockID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_PUT( KEPUBLISHER_UNREG_USER_REQ, obj, ks )
{
	return PUT( m_iActorUID )
		&& PUT( m_wstrServiceAccountID )
		&& PUT( m_wstrServicePassword )
		&& PUT( m_strIP )
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSockID )
		&& PUT( m_iGSUID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_UNREG_USER_REQ, obj, ks )
{
	return GET( m_iActorUID )
		&& GET( m_wstrServiceAccountID )
		&& GET( m_wstrServicePassword )
		&& GET( m_strIP )
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSockID )
		&& GET( m_iGSUID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_PUT( KEPUBLISHER_UNREG_USER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrServiceAccountID )
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_UNREG_USER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrServiceAccountID )
		;
}

SERIALIZE_DEFINE_PUT( KELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrServiceAccountID )
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		&& PUT( m_bIsPcBang )
		&& PUT( m_iPCBangType )
		&& PUT( m_iCheckState )
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER
#ifdef SERV_COUNTRY_TH
		&& PUT( m_wstrSockID )
#endif //SERV_COUNTRY_TH
		;
}

SERIALIZE_DEFINE_GET( KELG_REGISTER_USER_GLOBAL_PUBLISHER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrServiceAccountID )
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
		&& GET( m_bIsPcBang )
		&& GET( m_iPCBangType )
		&& GET( m_iCheckState )
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER
#ifdef SERV_COUNTRY_TH
		&& GET( m_wstrSockID )
#endif //SERV_COUNTRY_TH
		;
}
//===============================================================================//
//{{ 2011. 03. 03	최육사	중국 서버 빌링

SERIALIZE_DEFINE_PUT( KEBILL_CHECK_PRODUCT_INFO_REQ, obj, ks )
{
	return  

#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		PUT( m_mapReleaseTick )
#else
		PUT( m_iReleaseTick )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_GET( KEBILL_CHECK_PRODUCT_INFO_REQ, obj, ks )
{
	return 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		GET( m_mapReleaseTick )
#else
		GET( m_iReleaseTick )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_CHECK_PRODUCT_INFO_ACK, obj, ks )
{
	return 
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		PUT( m_mapReleaseTick )
#else
		PUT( m_iReleaseTick )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_mapBillProductInfo )
		&& PUT( m_mapBillProductAttribute )
		&& PUT( m_mapBillPackageInfo )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_vecStartEvent )
		&& PUT( m_vecEndEvent )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_GET( KEBILL_CHECK_PRODUCT_INFO_ACK, obj, ks )
{
	return
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		GET( m_mapReleaseTick )
#else
		GET( m_iReleaseTick )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_mapBillProductInfo )
		&& GET( m_mapBillProductAttribute )
		&& GET( m_mapBillPackageInfo )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_vecStartEvent )
		&& GET( m_vecEndEvent )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

//{{ 허상형 : [2010/7/28/] //	캐시 로그
SERIALIZE_DEFINE_PUT( KBuyCharInfo, obj, ks )
{
	return PUT( m_cLevel )
		&& PUT( m_iUnitClass );
}

SERIALIZE_DEFINE_GET( KBuyCharInfo, obj, ks )
{
	return GET( m_cLevel )
		&& GET( m_iUnitClass );
}
//}} 허상형 : [2010/7/28/] //	캐시 로그

SERIALIZE_DEFINE_PUT( KEBILL_BUY_PRODUCT_REQ, obj, ks )
{
	return	PUT( m_iServerGroupID ) 
		&&	PUT( m_iChannelingCode ) 
		&&	PUT( m_wstrUserID ) 
		&&	PUT( m_uiPublisherUID )
		&&	PUT( m_iUserUID )
		&&	PUT( m_iUnitUID )
		&&	PUT( m_wstrNickName ) 
		&&	PUT( m_strUserIP ) 
		&&	PUT( m_vecBillBuyInfo )
		//{{ 허상형 : [2010/7/28/] //	캐시 로그
		&&	PUT( m_BuyCharInfo )
		//}} 허상형 : [2010/7/28/] //	캐시 로그
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&&	PUT( m_iUseCashType )
		&&	PUT( m_iUserIP )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		&&	PUT( m_wstrTotalOrderID )
#ifdef SERV_JAPAN_CHANNELING
		&& PUT( m_iChannelingCode )
#endif SERV_JAPAN_CHANNELING
		;
}

SERIALIZE_DEFINE_GET( KEBILL_BUY_PRODUCT_REQ, obj, ks )
{
	return	GET( m_iServerGroupID ) 
		&&	GET( m_iChannelingCode ) 
		&&	GET( m_wstrUserID ) 
		&&	GET( m_uiPublisherUID ) 
		&&	GET( m_iUserUID ) 
		&&	GET( m_iUnitUID ) 
		&&	GET( m_wstrNickName ) 
		&&	GET( m_strUserIP ) 
		&&	GET( m_vecBillBuyInfo )
		//{{ 허상형 : [2010/7/28/] //	캐시 로그
		&&	GET( m_BuyCharInfo )
		//}} 허상형 : [2010/7/28/] //	캐시 로그
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&&	GET( m_iUseCashType )
		&&	GET( m_iUserIP )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		&&	GET( m_wstrTotalOrderID )
#ifdef SERV_JAPAN_CHANNELING
		&&  GET( m_iChannelingCode )
#endif SERV_JAPAN_CHANNELING
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_BUY_PRODUCT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecBillOrderInfo )
		//{{ 100713 김정협 패키지아이템일 경우 찜하기 목록에서 빼주기 위한 목록
		&& PUT( m_vecPackageID )
		//}} 100713 김정협 패키지아이템일 경우 찜하기 목록에서 빼주기 위한 목록
		&& PUT( m_iTotalCash )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_iCashType )
		&& PUT( m_RemainCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_GET( KEBILL_BUY_PRODUCT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecBillOrderInfo )
		//{{ 100713 김정협 패키지아이템일 경우 찜하기 목록에서 빼주기 위한 목록
		&& GET( m_vecPackageID )
		//}} 100713 김정협 패키지아이템일 경우 찜하기 목록에서 빼주기 위한 목록
		&& GET( m_iTotalCash )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_iCashType )
		&& GET( m_RemainCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}


SERIALIZE_DEFINE_PUT( KEBILL_PREPARE_BUY_PRODUCT_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_kEBILL_BUY_PRODUCT_REQ )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_PREPARE_BUY_PRODUCT_ACK, obj, ks )
{
	return GET( m_iOK ) 
		&& GET( m_kEBILL_BUY_PRODUCT_REQ )
		;
}


SERIALIZE_DEFINE_GET( KEBILL_GIFT_ITEM_REQ, obj, ks )
{
	return GET( m_iServerGroupID )
		&& GET( m_iChannelingCode )
		&& GET( m_wstrSenderUserID )
		&& GET( m_uiSenderPublisherUID )
		&& GET( m_wstrSenderNickName )
		&& GET( m_wstrReceiverUserID )
		&& GET( m_wstrReceiverNickName )
		&& GET( m_vecBillBuyInfo )
		&& GET( m_iSenderUserUID )
		&& GET( m_iSenderUnitUID )
		&& GET( m_iReceiverUserUID )
		&& GET( m_iReceiverUnitUID )
		&& GET( m_wstrPresentMessage )
		//{{ 허상형 : [2010/7/28/] //	캐시 로그
		&& GET( m_BuyCharInfo )
		&& GET( m_RecvCharInfo )
		//}} 허상형 : [2010/7/28/] //	캐시 로그
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_iUseCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_wstrUserIP )
		&& GET( m_wstrTotalOrderID )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_GIFT_ITEM_REQ, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_iChannelingCode )
		&& PUT( m_wstrSenderUserID )
		&& PUT( m_uiSenderPublisherUID )
		&& PUT( m_wstrSenderNickName )
		&& PUT( m_wstrReceiverUserID )
		&& PUT( m_wstrReceiverNickName )
		&& PUT( m_vecBillBuyInfo )
		&& PUT( m_iSenderUserUID )
		&& PUT( m_iSenderUnitUID )
		&& PUT( m_iReceiverUserUID )
		&& PUT( m_iReceiverUnitUID )
		&& PUT( m_wstrPresentMessage )
		//{{ 허상형 : [2010/7/28/] //	캐시 로그
		&& PUT( m_BuyCharInfo )
		&& PUT( m_RecvCharInfo )
		//}} 허상형 : [2010/7/28/] //	캐시 로그
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_iUseCashType )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_wstrUserIP )
		&& PUT( m_wstrTotalOrderID )
		;
}


SERIALIZE_DEFINE_GET( KEBILL_GIFT_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecBillOrderInfo )
		&& GET( m_iTotalCash )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_iCashType )
		&& GET( m_RemainCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_GIFT_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecBillOrderInfo )
		&& PUT( m_iTotalCash )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_iCashType )
		&& PUT( m_RemainCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_PREPARE_GIFT_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_kEBILL_GIFT_ITEM_REQ )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_PREPARE_GIFT_ITEM_ACK, obj, ks )
{
	return GET( m_iOK ) 
		&& GET( m_kEBILL_GIFT_ITEM_REQ )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT, obj, ks )
{
	return PUT( m_iUserUID ) 
		&& PUT( m_iResultCode )
		&& PUT( m_strOutString )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_CASH_DECREASE_ERROR_LOG_NOT, obj, ks )
{
	return GET( m_iUserUID ) 
		&& GET( m_iResultCode )
		&& GET( m_strOutString )
		;
}

//

SERIALIZE_DEFINE_PUT( KPurchaserInfo, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_iUserUID )
		&& PUT( m_wstrUserName )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrUnitNickName )
		&& PUT( m_ucLevel )
		&& PUT( m_wstrIP )
		&& PUT( m_uiPublisherUID )
		&& PUT( m_iChannelingCode )
		;
}

SERIALIZE_DEFINE_GET( KPurchaserInfo, obj, ks )
{
	return GET( m_iServerGroupID )
		&& GET( m_iUserUID )
		&& GET( m_wstrUserName )
		&& GET( m_iUnitUID )
		&& GET( m_wstrUnitNickName )
		&& GET( m_ucLevel )
		&& GET( m_wstrIP )
		&& GET( m_uiPublisherUID )
		&& GET( m_iChannelingCode )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_USE_COUPON_REQ, obj, ks )
{		
	return PUT( m_PurchaserInfo )
		&& PUT( m_iCouponType )
		&& PUT( m_wstrSerialCode )
		&& PUT( m_wstrTransaction )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_USE_COUPON_REQ, obj, ks )
{
	return GET( m_PurchaserInfo )
		&& GET( m_iCouponType )
		&& GET( m_wstrSerialCode )
		&& GET( m_wstrTransaction )
		;
}




SERIALIZE_DEFINE_PUT( KBillCouponItemInfo, obj, ks )
{
	return PUT( m_iProductNO )
		&& PUT( m_iCount )
		&& PUT( m_iTranNo )
		;
}


SERIALIZE_DEFINE_GET( KBillCouponItemInfo, obj, ks )
{
	return GET( m_iProductNO )
		&& GET( m_iCount )
		&& GET( m_iTranNo )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_USE_COUPON_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_vecBillCouponItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_USE_COUPON_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_vecBillCouponItemInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEBILL_USE_COUPON_RESULT_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUserUID )
		&& PUT( m_wstrTransaction )
		&& PUT( m_iRet )
		&& PUT( m_iChargeItem )
		&& PUT( m_iChargeItemCnt )
		&& PUT( m_iChargeCash )
		&& PUT( m_iChargeBonus )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_USE_COUPON_RESULT_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUserUID )
		&& GET( m_wstrTransaction )
		&& GET( m_iRet )
		&& GET( m_iChargeItem )
		&& GET( m_iChargeItemCnt )
		&& GET( m_iChargeCash )
		&& GET( m_iChargeBonus )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_USE_COUPON_RESULT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iTotalCash )
		&& PUT( m_vecBillCouponItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_USE_COUPON_RESULT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iTotalCash )
		&& GET( m_vecBillCouponItemInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_USE_COUPON_RESERVE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kPacketReq )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_USE_COUPON_RESERVE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kPacketReq )
		;
}



SERIALIZE_DEFINE_PUT( KEBILL_PICK_UP_REQ, obj, ks )
{
	return  PUT( m_iTransNo )
		&&	PUT( m_iProductNo )
		&&	PUT( m_iUnitUID )
		&&	PUT( m_mapSocketForCash )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	PUT( m_byteProductKind )
		&&	PUT( m_vecPickUpPackageInfo )
		&&	PUT( m_iSubProductNo )
		&&	PUT( m_iSubProductID )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KEBILL_PICK_UP_REQ, obj, ks )
{
	return  GET( m_iTransNo )
		&&	GET( m_iProductNo )
		&&	GET( m_iUnitUID )
		&&	GET( m_mapSocketForCash )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	GET( m_byteProductKind )
		&&	GET( m_vecPickUpPackageInfo )
		&&	GET( m_iSubProductNo )
		&&	GET( m_iSubProductID )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_PICK_UP_ACK, obj, ks )
{
	return  PUT( m_iOK ) &&
		PUT( m_iProductNo ) &&
		PUT( m_mapSocketForCash );
}

SERIALIZE_DEFINE_GET( KEBILL_PICK_UP_ACK, obj, ks )
{
	return  GET( m_iOK ) &&
		GET( m_iProductNo ) &&
		GET( m_mapSocketForCash );
}

SERIALIZE_DEFINE_PUT( KELG_EXCHANGE_CASH_REQ, obj, ks )
{
	return PUT( m_PurchaserInfo )
		&& PUT( m_iPoint )
		;
}

SERIALIZE_DEFINE_GET( KELG_EXCHANGE_CASH_REQ, obj, ks )
{
	return GET( m_PurchaserInfo )
		&& GET( m_iPoint )
		;
}

SERIALIZE_DEFINE_PUT( KELG_EXCHANGE_CASH_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPoint )
		;
}

SERIALIZE_DEFINE_GET( KELG_EXCHANGE_CASH_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPoint )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_CHECK_BALANCE_REQ, obj, ks )
{
	return PUT( m_iPublisherCashBalance )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_CHECK_BALANCE_REQ, obj, ks )
{
	return GET( m_iPublisherCashBalance )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_EXCHANGE_CASH_RESERVE_REQ, obj, ks )
{
	return PUT( m_PurchaserInfo )
		&& PUT( m_iPoint )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_EXCHANGE_CASH_RESERVE_REQ, obj, ks )
{
	return GET( m_PurchaserInfo )
		&& GET( m_iPoint )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_EXCHANGE_CASH_RESERVE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrTransaction )
		&& PUT( m_kPacketReq )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_EXCHANGE_CASH_RESERVE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrTransaction )
		&& GET( m_kPacketReq )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_EXCHANGE_CASH_REQ, obj, ks )
{
	return PUT( m_iGSUID )
		&& PUT( m_iUserUID )
		&& PUT( m_wstrTransaction )
		&& PUT( m_iRet )
		&& PUT( m_iPoint )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_EXCHANGE_CASH_REQ, obj, ks )
{
	return GET( m_iGSUID )
		&& GET( m_iUserUID )
		&& GET( m_wstrTransaction )
		&& GET( m_iRet )
		&& GET( m_iPoint )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_EXCHANGE_CASH_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_iTotalCash )
		&& PUT( m_iChargeCash )
		&& PUT( m_iChargeBonus )
		&& PUT( m_iBonusItem )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_RemainCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_GET( KEBILL_EXCHANGE_CASH_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_iTotalCash )
		&& GET( m_iChargeCash )
		&& GET( m_iChargeBonus )
		&& GET( m_iBonusItem )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_RemainCashInfo )
#endif //SERV_SUPPORT_SEVERAL_CASH_TYPES
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ, obj, ks )
{
	return PUT( m_kCheckGiantBalanceReq )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ, obj, ks )
{
	return GET( m_kCheckGiantBalanceReq )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrTransaction )
		&& PUT( m_kCheckGiantBalanceReq )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrTransaction )
		&& GET( m_kCheckGiantBalanceReq )
		;
}

//{{ 허상형 : [2010/8/19/] //	선물 기능 개편 SERV_GLOBAL_BILLING. 유럽에 추가 조효진
SERIALIZE_DEFINE_PUT( KDBE_GET_NICKNAME_BY_UNITUID_REQ, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_vecBillOrderInfo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	PUT( m_vecPackageInfo )
		&&	PUT( m_vecSubPackageTrans )
		&&	PUT( m_vecSubPackageInfo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_NICKNAME_BY_UNITUID_REQ, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_vecBillOrderInfo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	GET( m_vecPackageInfo )
		&&	GET( m_vecSubPackageTrans )
		&&	GET( m_vecSubPackageInfo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}


SERIALIZE_DEFINE_PUT( KDBE_GET_NICKNAME_BY_UNITUID_ACK, obj, ks )
{
	return PUT( m_mapNickName )
		&& PUT( m_vecBillOrderInfo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	PUT( m_vecPackageInfo )
		&&	PUT( m_vecSubPackageTrans )
		&&	PUT( m_vecSubPackageInfo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_NICKNAME_BY_UNITUID_ACK, obj, ks )
{
	return GET( m_mapNickName )
		&& GET( m_vecBillOrderInfo )
#ifdef SERV_GLOBAL_CASH_PACKAGE
		&&	GET( m_vecPackageInfo )
		&&	GET( m_vecSubPackageTrans )
		&&	GET( m_vecSubPackageInfo )
#endif //SERV_GLOBAL_CASH_PACKAGE
		;
}
//}} 허상형 : [2010/8/19/] //	선물 기능 개편
//}}


#ifdef GIANT_RESURRECTION_CASHSTONE

SERIALIZE_DEFINE_PUT( KDBE_REALTIME_RESURRECTION_CASHSTONE_NOT, obj, ks )
{
	return  PUT( m_iUnitUID )
		&&	PUT( m_iNumResurrection_CashStone )
		;
}

SERIALIZE_DEFINE_GET( KDBE_REALTIME_RESURRECTION_CASHSTONE_NOT, obj, ks )
{
	return  GET( m_iUnitUID )
		&&	GET( m_iNumResurrection_CashStone )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_RESURRECTION_CASHSTONE_NOT, obj, ks )
{
	return  PUT( m_iUnitUID )
		&&	PUT( m_iNumResurrection_CashStone )
		;
}

SERIALIZE_DEFINE_GET( KDBE_RESURRECTION_CASHSTONE_NOT, obj, ks )
{
	return  GET( m_iUnitUID )
		&&	GET( m_iNumResurrection_CashStone )
		;
}

#endif // GIANT_RESURRECTION_CASHSTONE

//}}

//////////////////////////////////////////////////////////////////////////
#ifdef SERV_COUNTRY_US

SERIALIZE_DEFINE_PUT( KEPL_BT_HEALTH_CHECK_REQ, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode );
}

SERIALIZE_DEFINE_GET( KEPL_BT_HEALTH_CHECK_REQ, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode );
}

SERIALIZE_DEFINE_PUT( KEPL_BT_HEALTH_CHECK_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode );
}

SERIALIZE_DEFINE_GET( KEPL_BT_HEALTH_CHECK_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode );
}

SERIALIZE_DEFINE_PUT( KEPL_BT_BALANCE_CHECK_REQ, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_ulUserNo ) &&
		PUT( m_wstrUserID ) &&

		PUT( m_ulRealCash ) &&
		PUT( m_ulBonusCash )
		;
}

SERIALIZE_DEFINE_GET( KEPL_BT_BALANCE_CHECK_REQ, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_ulUserNo ) &&
		GET( m_wstrUserID ) &&

		GET( m_ulRealCash ) &&
		GET( m_ulBonusCash )
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BT_BALANCE_CHECK_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_ulUserNo ) &&
		PUT( m_wstrUserID ) &&

		PUT( m_ulRealCash ) &&
		PUT( m_ulBonusCash ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPL_BT_BALANCE_CHECK_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_ulUserNo ) &&
		GET( m_wstrUserID ) &&

		GET( m_ulRealCash ) &&
		GET( m_ulBonusCash ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		GET( m_wstrRetMsg )
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KEPL_BT_PURCHASEITEM_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_wstrOrderID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrUserID ) &&
		PUT( m_iUserUID ) &&
		PUT( m_wstrNickName ) &&
		PUT( m_strUserIP ) &&
		PUT( m_vecBillBuyInfo ) &&
		PUT(m_bResurrectionStone) &&
		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPL_BT_PURCHASEITEM_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_wstrOrderID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrUserID ) &&
		GET( m_iUserUID ) &&
		GET( m_wstrNickName ) &&
		GET( m_strUserIP ) &&
		GET( m_vecBillBuyInfo ) &&
		GET(m_bResurrectionStone) &&
		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		GET( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BT_GIFTITEM_REQ, obj, ks )
{
	return   PUT( m_ulReqKey ) &&

		PUT( m_wstrOrderID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrSenderUserID ) &&
		PUT( m_wstrSenderNickName ) &&
		PUT( m_wstrReceiverUserID ) &&
		PUT( m_iSenderUnitUID ) &&
		PUT( m_iReceiverUID ) &&
		PUT( m_wstrPresentMessage ) &&
		PUT( m_strUserIP ) &&
		PUT( m_vecBillBuyInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPL_BT_GIFTITEM_REQ, obj, ks )
{
	return  GET( m_ulReqKey ) &&

		GET( m_wstrOrderID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrSenderUserID ) &&
		GET( m_wstrSenderNickName ) &&
		GET( m_wstrReceiverUserID ) &&
		GET( m_iSenderUnitUID ) &&
		GET( m_iReceiverUID ) &&
		GET( m_wstrPresentMessage ) &&
		GET( m_strUserIP ) &&
		GET( m_vecBillBuyInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BT_GIFTITEM_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_wstrOrderID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrUserID ) &&
		PUT( m_iUserUID ) &&
		PUT( m_wstrNickName ) &&
		PUT( m_strUserIP ) &&
		PUT( m_vecBillBuyInfo ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPL_BT_GIFTITEM_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_wstrOrderID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrUserID ) &&
		GET( m_iUserUID ) &&
		GET( m_wstrNickName ) &&
		GET( m_strUserIP ) &&
		GET( m_vecBillBuyInfo ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		GET( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BT_REGCOUPON_ACK, obj, ks )
{
	return PUT( m_usReqLen )
		&& PUT( m_usReqType )
		&& PUT( m_ulReqKey )
		&& PUT( m_usRetCode )
		&& PUT( m_iUserUID )
		&& PUT( m_vecBillBuyInfo )
		//&& PUT( m_iItemID )
		//&& PUT( m_iItemCount )

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		&& PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET(  KEPL_BT_REGCOUPON_ACK, obj, ks )
{
	return GET( m_usReqLen )
		&& GET( m_usReqType ) 
		&& GET( m_ulReqKey )
		&& GET( m_usRetCode )
		&& GET( m_iUserUID )
		&& GET( m_vecBillBuyInfo )
		//&& GET( m_iItemID )
		//&& GET( m_iItemCount )
		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		&& GET( m_wstrRetMsg )
		//}}
		;
}

#endif // SERV_COUNTRY_US
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_COUNTRY_BR

SERIALIZE_DEFINE_PUT( KEPL_BR_HEALTH_CHECK_REQ, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode );
}

SERIALIZE_DEFINE_GET( KEPL_BR_HEALTH_CHECK_REQ, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode );
}

SERIALIZE_DEFINE_PUT( KEPL_BR_HEALTH_CHECK_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode );
}

SERIALIZE_DEFINE_GET( KEPL_BR_HEALTH_CHECK_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode );
}

SERIALIZE_DEFINE_PUT( KEPL_BR_BALANCE_CHECK_REQ, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_ulUserNo ) &&
		PUT( m_wstrUserID ) &&

		PUT( m_ulRealCash ) &&
		PUT( m_ulBonusCash )
		;
}

SERIALIZE_DEFINE_GET( KEPL_BR_BALANCE_CHECK_REQ, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_ulUserNo ) &&
		GET( m_wstrUserID ) &&

		GET( m_ulRealCash ) &&
		GET( m_ulBonusCash )
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BR_BALANCE_CHECK_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_ulUserNo ) &&
		PUT( m_wstrUserID ) &&

		PUT( m_ulRealCash ) &&
		PUT( m_ulBonusCash ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPL_BR_BALANCE_CHECK_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_ulUserNo ) &&
		GET( m_wstrUserID ) &&

		GET( m_ulRealCash ) &&
		GET( m_ulBonusCash ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		GET( m_wstrRetMsg )
		//}}
		;
}


SERIALIZE_DEFINE_PUT( KEPL_BR_PURCHASEITEM_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_wstrOrderID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrUserID ) &&
		PUT( m_iUserUID ) &&
		PUT( m_wstrNickName ) &&
		PUT( m_strUserIP ) &&
		PUT( m_vecBillBuyInfo ) &&
		PUT(m_bResurrectionStone) &&
		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPL_BR_PURCHASEITEM_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_wstrOrderID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrUserID ) &&
		GET( m_iUserUID ) &&
		GET( m_wstrNickName ) &&
		GET( m_strUserIP ) &&
		GET( m_vecBillBuyInfo ) &&
		GET(m_bResurrectionStone) &&
		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		GET( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BR_GIFTITEM_REQ, obj, ks )
{
	return   PUT( m_ulReqKey ) &&

		PUT( m_wstrOrderID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrSenderUserID ) &&
		PUT( m_wstrSenderNickName ) &&
		PUT( m_wstrReceiverUserID ) &&
		PUT( m_iSenderUnitUID ) &&
		PUT( m_iReceiverUID ) &&
		PUT( m_wstrPresentMessage ) &&
		PUT( m_strUserIP ) &&
		PUT( m_vecBillBuyInfo )
		;
}

SERIALIZE_DEFINE_GET( KEPL_BR_GIFTITEM_REQ, obj, ks )
{
	return  GET( m_ulReqKey ) &&

		GET( m_wstrOrderID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrSenderUserID ) &&
		GET( m_wstrSenderNickName ) &&
		GET( m_wstrReceiverUserID ) &&
		GET( m_iSenderUnitUID ) &&
		GET( m_iReceiverUID ) &&
		GET( m_wstrPresentMessage ) &&
		GET( m_strUserIP ) &&
		GET( m_vecBillBuyInfo )
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BR_GIFTITEM_ACK, obj, ks )
{
	return  PUT( m_usReqLen ) &&
		PUT( m_usReqType ) &&
		PUT( m_ulReqKey ) &&
		PUT( m_usRetCode ) &&

		PUT( m_wstrOrderID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrUserID ) &&
		PUT( m_iUserUID ) &&
		PUT( m_wstrNickName ) &&
		PUT( m_strUserIP ) &&
		PUT( m_vecBillBuyInfo ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET( KEPL_BR_GIFTITEM_ACK, obj, ks )
{
	return  GET( m_usReqLen ) &&
		GET( m_usReqType ) &&
		GET( m_ulReqKey ) &&
		GET( m_usRetCode ) &&

		GET( m_wstrOrderID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrUserID ) &&
		GET( m_iUserUID ) &&
		GET( m_wstrNickName ) &&
		GET( m_strUserIP ) &&
		GET( m_vecBillBuyInfo ) &&

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		GET( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_PUT( KEPL_BR_REGCOUPON_ACK, obj, ks )
{
	return PUT( m_usReqLen )
		&& PUT( m_usReqType )
		&& PUT( m_ulReqKey )
		&& PUT( m_usRetCode )
		&& PUT( m_iUserUID )
		&& PUT( m_vecBillBuyInfo )
		//&& PUT( m_iItemID )
		//&& PUT( m_iItemCount )

		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		&& PUT( m_wstrRetMsg )
		//}}
		;
}

SERIALIZE_DEFINE_GET(  KEPL_BR_REGCOUPON_ACK, obj, ks )
{
	return GET( m_usReqLen )
		&& GET( m_usReqType ) 
		&& GET( m_ulReqKey )
		&& GET( m_usRetCode )
		&& GET( m_iUserUID )
		&& GET( m_vecBillBuyInfo )
		//&& GET( m_iItemID )
		//&& GET( m_iItemCount )
		//{{ 리턴메세지를 확인하기 위한 부분 추가합니다. 2012.03.07 박진웅
		&& GET( m_wstrRetMsg )
		//}}
		;
}

#endif // SERV_COUNTRY_BR


//////////////////////////////////////////////////////////////////////////
#ifdef SERV_COUNTRY_TWHK

//{{ 2011.7.12 지헌 : Gash 포인트 통합
#ifdef SERV_BILLING_ADD_GASH_POINT
SERIALIZE_DEFINE_PUT(  KEGASH_GET_GASHPOINT_REQ, obj, ks )
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

SERIALIZE_DEFINE_GET(  KEGASH_GET_GASHPOINT_REQ, obj, ks )
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

SERIALIZE_DEFINE_PUT(  KEGASH_DECREASE_GASHPOINT_REQ, obj, ks )
{
	return  PUT( m_iActorUID ) &&
		PUT( m_strServiceCode ) &&
		PUT( m_strServiceRegion ) &&
		PUT( m_strServiceAccountID ) &&
		PUT( m_strType ) &&
		PUT( m_strShopTime ) &&
		PUT( m_strPoints ) &&
		PUT( m_strIPAddress ) &&
		PUT( m_strMemo ) &&
		PUT( m_strPacketID );
}

SERIALIZE_DEFINE_GET(  KEGASH_DECREASE_GASHPOINT_REQ, obj, ks )
{
	return  GET( m_iActorUID ) &&
		GET( m_strServiceCode ) &&
		GET( m_strServiceRegion ) &&
		GET( m_strServiceAccountID ) &&
		GET( m_strType ) &&
		GET( m_strShopTime ) &&
		GET( m_strPoints ) &&
		GET( m_strIPAddress ) &&
		GET( m_strMemo ) &&
		GET( m_strPacketID );
}

SERIALIZE_DEFINE_PUT( KEGASH_USE_GASH_LOG_NOT, obj, ks )
{
	return  PUT( m_wstrTotalOrderID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrUserID ) &&
		PUT( m_iUserUID ) &&
		PUT( m_wstrNickName ) &&
		PUT( m_iBeforeCash ) &&
		PUT( m_iAfterGash ) &&
		PUT( m_iGashPoint ) &&
		PUT( m_iItemID ) &&
		PUT( m_iProductID ) &&
		PUT( m_iFlag ) &&
		PUT( m_iAfterGash ) &&
		PUT( m_wstrMemo ) &&
		PUT( m_wstrUserIP );
}

SERIALIZE_DEFINE_GET( KEGASH_USE_GASH_LOG_NOT, obj, ks )
{
	return  GET( m_wstrTotalOrderID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrUserID ) &&
		GET( m_iUserUID ) &&
		GET( m_wstrNickName ) &&
		GET( m_iBeforeCash ) &&
		GET( m_iAfterGash ) &&
		GET( m_iGashPoint ) &&
		GET( m_iItemID ) &&
		GET( m_iProductID ) &&
		GET( m_iFlag ) &&
		GET( m_iAfterGash ) &&
		GET( m_wstrMemo ) &&
		GET( m_wstrUserIP );
}
#endif//SERV_BILLING_ADD_GASH_POINT
//}}

#ifdef SERV_GOLD_TICKET
SERIALIZE_DEFINE_PUT( KEPUBLISHER_CHARGE_POINT_REQ, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_wstrUserID )
		&& PUT( m_iCharUID )
		&& PUT( m_iItemUID )
		&& PUT( m_iItemID )
		&& PUT( m_wstrUserIP )
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_CHARGE_POINT_REQ, obj, ks )
{
	return GET( m_iServerGroupID ) 
		&& GET( m_wstrUserID ) 
		&& GET( m_iCharUID ) 
		&& GET( m_iItemUID ) 
		&& GET( m_iItemID ) 
		&& GET( m_wstrUserIP )
		;
}

SERIALIZE_DEFINE_PUT( KEPUBLISHER_CHARGE_POINT_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_iChargedCash )
		;
}

SERIALIZE_DEFINE_GET( KEPUBLISHER_CHARGE_POINT_ACK, obj, ks )
{
	return GET( m_iOK ) 
		&& GET( m_iChargedCash )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_CHARGE_POINT_NOT, obj, ks )
{
	return PUT( m_iOK ) 
		&& PUT( m_iChargedCash ) 
		&& PUT( m_iNowCash ) 
		&& PUT( m_iCharUID ) 
		&& PUT( m_iItemID ) 
		&& PUT( m_iItemUID )
		;
}

SERIALIZE_DEFINE_GET( KELOG_CHARGE_POINT_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iChargedCash )
		&& GET( m_iNowCash )
		&& GET( m_iCharUID )
		&& GET( m_iItemID )
		&& GET( m_iItemUID )
		;
}
#endif SERV_GOLD_TICKET


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
			PUT( m_strAccountID ) &&
		    PUT( m_strPacketID );
}

SERIALIZE_DEFINE_GET( KGashResponseData, obj, ks )
{
	return  GET( m_strResultCode ) &&
		    GET( m_strOutString ) &&
			GET( m_strAccountID ) &&
		    GET( m_strPacketID );
}

#endif // SERV_COUNTRY_TWHK
//////////////////////////////////////////////////////////////////////////

#ifdef SERV_COUNTRY_IN
SERIALIZE_DEFINE_PUT( KDBE_PUBLISHER_AUTHENTICATION_REQ, obj, ks )
{
	return PUT( m_wstrServiceAccountID )
		&& PUT( m_wstrServicePassword )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PUBLISHER_AUTHENTICATION_REQ, obj, ks )
{
	return GET( m_wstrServiceAccountID )
		&& GET( m_wstrServicePassword )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_PUBLISHER_AUTHENTICATION_ACK, obj, ks )
{
	return PUT( m_uiPublisherUID )
		&& PUT( m_iOK )
		;
}

SERIALIZE_DEFINE_GET( KDBE_PUBLISHER_AUTHENTICATION_ACK, obj, ks )
{
	return GET( m_uiPublisherUID )
		&& GET( m_iOK )
		;
}
#endif SERV_COUNTRY_IN

#endif SERV_GLOBAL_AUTH
//}}

//{{ 허상형 : [2010/8/31/] //	부활석 통계 분리 작업
#ifdef SERV_SELECT_UNIT_NEW
SERIALIZE_DEFINE_PUT( KELOG_UPDATE_STONE_NOT, obj, ks )
{
	return PUT( m_iSupplyCnt )
		&& PUT( m_iQuantity )
		;
}

SERIALIZE_DEFINE_GET( KELOG_UPDATE_STONE_NOT, obj, ks )
{
	return GET( m_iSupplyCnt )
		&& GET( m_iQuantity )
		;
}
#endif	//	SERV_SELECT_UNIT_NEW
//}} 허상형 : [2010/8/31/] //	부활석 통계 분리 작업

//{{ 2010. 11. 12  조효진	GameForge 채팅 로그 관련 처리
#ifdef SERV_RECORD_CHAT
SERIALIZE_DEFINE_PUT( KDCHATLOG_RECORD_CHAT_NOT, obj, ks )
{
	return	PUT( m_iAccountUID ) &&
		PUT( m_iUnitUID ) &&
		PUT( m_iServerGroupID ) &&
		PUT( m_wstrChatText ) &&
		PUT( m_iChatType );
}

SERIALIZE_DEFINE_GET( KDCHATLOG_RECORD_CHAT_NOT, obj, ks )
{
	return	GET( m_iAccountUID ) &&
		GET( m_iUnitUID ) &&
		GET( m_iServerGroupID ) &&
		GET( m_wstrChatText ) &&
		GET( m_iChatType );
}
#endif SERV_RECORD_CHAT
//}}

//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
SERIALIZE_DEFINE_PUT( KDBE_OPEN_SYNTHESIS_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_setSealCashItem )
		&& PUT( m_mapSynthesisData )
		&& PUT( m_iKeyData )
		&& PUT( m_iLevel )
		;
}

SERIALIZE_DEFINE_GET( KDBE_OPEN_SYNTHESIS_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_setSealCashItem )
		&& GET( m_mapSynthesisData )
		&& GET( m_iKeyData )
		&& GET( m_iLevel )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_OPEN_SYNTHESIS_ITEM_ACK, obj, ks )
{
	return  PUT( m_iOK )
		&&	PUT( m_mapInsertedItem )
		&&	PUT( m_kItemQuantityUpdate )		
		&&	PUT( m_vecUpdatedInventorySlot )
		&&	PUT( m_mapItemInfo )
		&&	PUT( m_mapSynthesisData )
		&&	PUT( m_iKeyData )
		;
}

SERIALIZE_DEFINE_GET( KDBE_OPEN_SYNTHESIS_ITEM_ACK, obj, ks )
{
	return  GET( m_iOK )
		&&	GET( m_mapInsertedItem )
		&&	GET( m_kItemQuantityUpdate )		
		&&	GET( m_vecUpdatedInventorySlot )
		&&	GET( m_mapItemInfo )
		&&	GET( m_mapSynthesisData )
		&&	GET( m_iKeyData )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_SYNTHESIS_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iLevel )
		&& PUT( m_iKeyUID )
		&& PUT( m_mapSynthesisData )
		&& PUT( m_vecItemInfo )
		&& PUT( m_mapInsertedItemInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SYNTHESIS_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iLevel )
		&& GET( m_iKeyUID )
		&& GET( m_mapSynthesisData )
		&& GET( m_vecItemInfo )
		&& GET( m_mapInsertedItemInfo )
		;
}

#endif SERV_SYNTHESIS_AVATAR
//}}

//{{ 2011. 12. 09	박교현		서버군 리스트
#ifdef SERVER_GROUP_UI_ADVANCED
SERIALIZE_DEFINE_PUT( KDBE_SERVERGROUP_LIST_ACK, obj, ks )
{
	return PUT( m_mapServerGroupList )
		;
};

SERIALIZE_DEFINE_GET( KDBE_SERVERGROUP_LIST_ACK, obj, ks )
{
	return GET( m_mapServerGroupList )
		;
};
#endif SERVER_GROUP_UI_ADVANCED
//}}

#ifdef SERV_INVISIBLE_GM
SERIALIZE_DEFINE_PUT( KERM_TOGGLE_INVISIBLE_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_kFieldUserInfo )
		;
};
SERIALIZE_DEFINE_GET( KERM_TOGGLE_INVISIBLE_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_kFieldUserInfo )
		;
};
SERIALIZE_DEFINE_PUT( KETR_TOGGLE_INVISIBLE_NOT, obj, ks )
{
	return PUT( m_vecUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kFieldUserInfo )
		;
};
SERIALIZE_DEFINE_GET( KETR_TOGGLE_INVISIBLE_NOT, obj, ks )
{
	return GET( m_vecUID )
		&& GET( m_iUnitUID )
		&& GET( m_kFieldUserInfo )
		;
};
#endif SERV_INVISIBLE_GM

#ifdef	SERV_SHARING_BANK_TEST
SERIALIZE_DEFINE_PUT( KTradeShareItemInfo, obj, ks )
{
	return  PUT( m_iItemUID ) 
		&&	PUT( m_iFromUnitUID )
		&&	PUT( m_iToUnitUID )
		&&	PUT( m_cInventoryCategory )
		&&	PUT( m_sSlotID )
		&&	PUT( m_ucSealCnt )
		&&	PUT( m_ucDeleteReason )
		&&	PUT( m_iItemID )
		&&	PUT( m_iNewItemUID )
		;
}

SERIALIZE_DEFINE_GET( KTradeShareItemInfo, obj, ks )
{
	return  GET( m_iItemUID ) 
		&&	GET( m_iFromUnitUID )
		&&	GET( m_iToUnitUID )
		&&	GET( m_cInventoryCategory )
		&&	GET( m_sSlotID )
		&&	GET( m_ucSealCnt )
		&&	GET( m_ucDeleteReason )
		&&	GET( m_iItemID )
		&&	GET( m_iNewItemUID )
		;
}

SERIALIZE_DEFINE_PUT( KUpdateShareItemInfo, obj, ks )
{
	return  PUT( m_iItemUID ) 
		&&	PUT( m_iUnitUID )
		&&	PUT( m_iUsageType )
		&&	PUT( m_iValue )
		;
}

SERIALIZE_DEFINE_GET( KUpdateShareItemInfo, obj, ks )
{
	return  GET( m_iItemUID ) 
		&&	GET( m_iUnitUID )
		&&	GET( m_iUsageType )
		&&	GET( m_iValue )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_SHARE_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapUpdatePosition )
		&& PUT( m_mapInItem )
		&& PUT( m_mapOutItem )
		&& PUT( m_mapUpdateEndurance )
		&& PUT( m_mapUpdateQuantity )
		&& PUT( m_vecDeletedItem )
		&& PUT( m_bReload )
		&& PUT( m_wstrReloadNickname )
		&& PUT( m_vecInventorySlotItem )
		;
}
SERIALIZE_DEFINE_GET( KDBE_UPDATE_SHARE_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapUpdatePosition )
		&& GET( m_mapInItem )
		&& GET( m_mapOutItem )
		&& GET( m_mapUpdateEndurance )
		&& GET( m_mapUpdateQuantity )
		&& GET( m_vecDeletedItem )
		&& GET( m_bReload )
		&& GET( m_wstrReloadNickname )
		&& GET( m_vecInventorySlotItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_SHARE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bReload )
		&& PUT( m_wstrReloadNickname )
		&& PUT( m_vecTradeShareItemResult )
		&& PUT( m_vecInventorySlotItem )
		;
}
SERIALIZE_DEFINE_GET( KDBE_UPDATE_SHARE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bReload )
		&& GET( m_wstrReloadNickname )
		&& GET( m_vecTradeShareItemResult )
		&& GET( m_vecInventorySlotItem )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_ITEM_TRADE_ERROR_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iType )
		&& PUT( m_wsrtRegDate )
		;
}
SERIALIZE_DEFINE_GET( KELOG_ITEM_TRADE_ERROR_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iType )
		&& PUT( m_wsrtRegDate )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHANGE_INVENTORY_SLOT_ITEM_REQ, obj, ks )
{
	return PUT( m_cFromSlotType )
		&& PUT( m_iFromSlotID )
		&& PUT( m_cToSlotType )
		&& PUT( m_iToSlotID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_kItemPositionUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_INVENTORY_SLOT_ITEM_REQ, obj, ks )
{
	return GET( m_cFromSlotType )
		&& GET( m_iFromSlotID )
		&& GET( m_cToSlotType )
		&& GET( m_iToSlotID )
		&& GET( m_iUnitUID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_kItemPositionUpdate )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHANGE_INVENTORY_SLOT_ITEM_ACK, obj, ks )
{
	return PUT( m_cFromSlotType )
		&& PUT( m_iFromSlotID )
		&& PUT( m_cToSlotType )
		&& PUT( m_iToSlotID )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_kItemEnduranceUpdate )
		&& PUT( m_kItemPositionUpdate )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_INVENTORY_SLOT_ITEM_ACK, obj, ks )
{
	return GET( m_cFromSlotType )
		&& GET( m_iFromSlotID )
		&& GET( m_cToSlotType )
		&& GET( m_iToSlotID )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_kItemEnduranceUpdate )
		&& GET( m_kItemPositionUpdate )
		;
}
#endif	SERV_SHARING_BANK_TEST

//{{ 지헌 - 은행 개편 퀘스트, 캐쉬 작업 
#ifdef SERV_SHARING_BANK_QUEST_CASH
SERIALIZE_DEFINE_PUT( KDBE_SHARING_BACK_OPEN_REQ, obj, ks )
{
	return PUT(m_iUserUID)
		&& PUT(m_iUnitUID)
		&& PUT(m_iOpenType)
		;
};

SERIALIZE_DEFINE_GET( KDBE_SHARING_BACK_OPEN_REQ, obj, ks )
{
	return GET(m_iUserUID)
		&& GET(m_iUnitUID)
		&& GET(m_iOpenType)
		;
};

SERIALIZE_DEFINE_PUT( KDBE_SHARING_BACK_OPEN_ACK, obj, ks )
{
	return PUT(m_iOK)
		&& PUT(m_iOpenType)
		;
};

SERIALIZE_DEFINE_GET( KDBE_SHARING_BACK_OPEN_ACK, obj, ks )
{
	return GET(m_iOK)
		&& GET(m_iOpenType)
		;
};
#endif SERV_SHARING_BANK_QUEST_CASH
//}}

#ifdef SERV_ADVERTISEMENT_EVENT
SERIALIZE_DEFINE_PUT( KDBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_vecCompletedEvent )
		;
}

SERIALIZE_DEFINE_GET( KDBE_INSERT_ADVERTISEMENT_EVENT_INFO_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_vecCompletedEvent )
		;
}
#endif SERV_ADVERTISEMENT_EVENT

#ifdef SERV_REFRESH_EVENT_USING_RELEASE_TICK
SERIALIZE_DEFINE_PUT( KDBE_CHECK_EVENT_UPDATE_ACK, obj, ks )
{
	return PUT( m_mapReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_EVENT_UPDATE_ACK, obj, ks )
{
	return GET( m_mapReleaseTick )
		;
}
#endif SERV_REFRESH_EVENT_USING_RELEASE_TICK


//{{ 2013. 05. 14	최육사	아이템 개편
#ifdef SERV_BLOCK_LIST
SERIALIZE_DEFINE_PUT( KDBE_CHECK_BLOCK_LIST_REQ, obj, ks )
{
	return PUT( m_iReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_BLOCK_LIST_REQ, obj, ks )
{
	return GET( m_iReleaseTick )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHECK_BLOCK_LIST_ACK, obj, ks )
{
	return PUT( m_iReleaseTick )
		&& PUT( m_mapBlockInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_BLOCK_LIST_ACK, obj, ks )
{
	return GET( m_iReleaseTick )
		&& GET( m_mapBlockInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEGB_CHECK_BLOCK_USER_NOT, obj, ks )
{
	return PUT( m_vecBlockType )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrIP )
		;
}

SERIALIZE_DEFINE_GET( KEGB_CHECK_BLOCK_USER_NOT, obj, ks )
{
	return GET( m_vecBlockType )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrIP )
		;
}


SERIALIZE_DEFINE_PUT( KEGB_UPDATE_BLOCK_RESULT_NOT, obj, ks )
{
	return PUT( m_mapBlockResult )
		;
}

SERIALIZE_DEFINE_GET( KEGB_UPDATE_BLOCK_RESULT_NOT, obj, ks )
{
	return GET( m_mapBlockResult )
		;
}
#endif SERV_BLOCK_LIST
//}}
#ifdef SERV_ADD_WARP_BUTTON
SERIALIZE_DEFINE_PUT( KDBE_INSERT_WARP_VIP_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iPeriod )
		;
}


SERIALIZE_DEFINE_GET( KDBE_INSERT_WARP_VIP_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iPeriod )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_WARP_VIP_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrEndDate )
		;
}


SERIALIZE_DEFINE_GET( KDBE_INSERT_WARP_VIP_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrEndDate )
		;
}
#endif // SERV_ADD_WARP_BUTTON


#ifdef SERV_EVENT_BUY_FAKE_ITEM
SERIALIZE_DEFINE_PUT( KEBILL_CHECK_BUY_FAKE_ITEM_REQ, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_iUserUID )
		&& PUT( m_ulProductNo )
		;
}
SERIALIZE_DEFINE_GET( KEBILL_CHECK_BUY_FAKE_ITEM_REQ, obj, ks )
{
	return GET( m_iServerGroupID )
		&& GET( m_iUserUID )
		&& GET( m_ulProductNo )
		;
}
SERIALIZE_DEFINE_PUT( KEBILL_CHECK_BUY_FAKE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK ) 
		;
}
SERIALIZE_DEFINE_GET( KEBILL_CHECK_BUY_FAKE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK ) 
		;
}
#endif //SERV_EVENT_BUY_FAKE_ITEM

#ifdef SERV_MOMOTI_EVENT
SERIALIZE_DEFINE_PUT( KDBE_MOMOTI_QUIZ_EVENT_REQ, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_istrReply )
		;
}
SERIALIZE_DEFINE_GET( KDBE_MOMOTI_QUIZ_EVENT_REQ, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_istrReply )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_MOMOTI_QUIZ_EVENT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iCheckReward )
		;
}
SERIALIZE_DEFINE_GET( KDBE_MOMOTI_QUIZ_EVENT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iCheckReward )
		;
}
#endif //SERV_MOMOTI_EVENT

#ifdef SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL
SERIALIZE_DEFINE_PUT( KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitLevel )
		&& PUT( m_iGetConnectExperience )
		&& PUT( m_iRewardBonusItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitLevel )
		&& GET( m_iGetConnectExperience )
		&& GET( m_iRewardBonusItem )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRewardBonusItem )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRewardBonusItem )
		;
}
#endif //SERV_EVENT_BOUNS_ITEM_AFTER_7DAYS_BY_LEVEL

#ifdef SERV_CLIENT_PORT_CHANGE_REQUEST_LOG
SERIALIZE_DEFINE_PUT( KDBE_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT, obj, ks )
{
	return PUT( m_iUDPPortSuccessType )
		&& PUT( m_iUseUdpPort )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CLIENT_POPRT_CHANGE_REQUEST_INFO_NOT, obj, ks )
{
	return GET( m_iUDPPortSuccessType )
		&& GET( m_iUseUdpPort )
		&& GET( m_iUnitUID )
		;
}
#endif //SERV_CLIENT_PORT_CHANGE_REQUEST_LOG

#ifdef SERV_SKILL_POINT_CORRECTION
SERIALIZE_DEFINE_PUT( KDBE_SKILL_POINT_CORRECTION_INFO_NOT, obj, ks )
{
	return PUT( m_iServerGroupID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iUnitClass )
		&& PUT( m_sLevel )
		&& PUT( m_iSPoint )
		&& PUT( m_iUsedSPoint )
		&& PUT( m_iCSPoint )
		&& PUT( m_iUsedCSPoint )
		&& PUT( m_iMaxCSPoint )
		&& PUT( m_iCorrectionSP )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SKILL_POINT_CORRECTION_INFO_NOT, obj, ks )
{
	return GET( m_iServerGroupID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iUnitClass )
		&& GET( m_sLevel )
		&& GET( m_iSPoint )
		&& GET( m_iUsedSPoint )
		&& GET( m_iCSPoint )
		&& GET( m_iUsedCSPoint )
		&& GET( m_iMaxCSPoint )
		&& GET( m_iCorrectionSP )
		;
}
#endif //SERV_SKILL_POINT_CORRECTION

#ifdef SERV_ID_NETMARBLE_PCBANG
SERIALIZE_DEFINE_PUT( KDBE_ID_PCBANG_CEHCK_AUTH_REQ, obj, ks )
{
	return PUT( m_bCheckClinetDirectPcBang )
		&& PUT( m_iIsGameBangType )
		&& PUT( m_wstrPublicIP )
		&& PUT( m_wstrMacAdress )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ID_PCBANG_CEHCK_AUTH_REQ, obj, ks )
{
	return GET( m_bCheckClinetDirectPcBang )
		&& GET( m_iIsGameBangType )
		&& GET( m_wstrPublicIP )
		&& GET( m_wstrMacAdress )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_ID_PCBANG_CEHCK_AUTH_ACK, obj, ks )
{
	return PUT( m_bCheckClinetDirectPcBang )
		&& PUT( m_iIsGameBangType )
		&& PUT( m_iIsNetmarbleGameBangType )
		;
}

SERIALIZE_DEFINE_GET( KDBE_ID_PCBANG_CEHCK_AUTH_ACK, obj, ks )
{
	return GET( m_bCheckClinetDirectPcBang )
		&& GET( m_iIsGameBangType )
		&& GET( m_iIsNetmarbleGameBangType )
		;
}
#endif //SERV_ID_NETMARBLE_PCBANG

#ifdef SERV_COUNTRY_PH

SERIALIZE_DEFINE_PUT( KEJSON_GN_CHECK_ACCOUNT_REQ, obj, ks )
{
	return PUT( m_uiGarenaUID )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEJSON_GN_CHECK_ACCOUNT_REQ, obj, ks )
{
	return GET( m_uiGarenaUID )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEJSON_GN_CHECK_ACCOUNT_ACK, obj, ks )
{
	return PUT( m_iResult )
		&& PUT( m_uiGarenaUID )
		&& PUT( m_uiCurrency_Balance )
		&& PUT( m_wstrCharName )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEJSON_GN_CHECK_ACCOUNT_ACK, obj, ks )
{
	return GET( m_iResult )
		&& GET( m_uiGarenaUID )
		&& GET( m_uiCurrency_Balance )
		&& GET( m_wstrCharName )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEJSON_GN_CHANGE_GAME_CURRENCY_REQ, obj, ks )
{
	return PUT( m_uiGameCurrency_Amount )
		&& PUT( m_uiShell_Amount )
		&& PUT( m_uiGarenaUID )
		&& PUT( m_strGarenaTransactionID )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEJSON_GN_CHANGE_GAME_CURRENCY_REQ, obj, ks )
{
	return GET( m_uiGameCurrency_Amount )
		&& GET( m_uiShell_Amount )
		&& GET( m_uiGarenaUID )
		&& GET( m_strGarenaTransactionID )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEJSON_GN_CHANGE_GAME_CURRENCY_ACK, obj, ks )
{
	return PUT( m_iResult )
		&& PUT( m_uiGarenaUID )
		&& PUT( m_strGarenaTransactionID )
		&& PUT( m_int64GameTransactionID )
		&& PUT( m_uiGameCurrency_Amount )
		&& PUT( m_uiGameCurrency_Balance )
		&& PUT( m_strDateTime )
		&& PUT( m_uiShell_Amount )
		&& PUT( m_uiGameCurrency_BeforeAmount )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEJSON_GN_CHANGE_GAME_CURRENCY_ACK, obj, ks )
{
	return GET( m_iResult )
		&& GET( m_uiGarenaUID )
		&& GET( m_strGarenaTransactionID )
		&& GET( m_int64GameTransactionID )
		&& GET( m_uiGameCurrency_Amount )
		&& GET( m_uiGameCurrency_Balance )
		&& GET( m_strDateTime )
		&& GET( m_uiShell_Amount )
		&& GET( m_uiGameCurrency_BeforeAmount )
		&& GET( m_iSessionUID )
		;
}
SERIALIZE_DEFINE_PUT( KEJSON_GN_TRANSACTION_REQ, obj, ks )
{
	return PUT( m_uiGarenaUID )
		&& PUT( m_strGarenaTransactionID )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEJSON_GN_TRANSACTION_REQ, obj, ks )
{
	return GET( m_uiGarenaUID )
		&& GET( m_strGarenaTransactionID )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEJSON_GN_TRANSACTION_ACK, obj, ks )
{
	return PUT( m_iResult )
		&& PUT( m_uiGarenaUID )
		&& PUT( m_strGarenaTransactionID )
		&& PUT( m_int64GameTransactionID )
		&& PUT( m_uiGameCurrency_Amount )
		&& PUT( m_uiShell_Amount )
		&& PUT( m_strDateTime )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEJSON_GN_TRANSACTION_ACK, obj, ks )
{
	return GET( m_iResult )
		&& GET( m_uiGarenaUID )
		&& GET( m_strGarenaTransactionID )
		&& GET( m_int64GameTransactionID )
		&& GET( m_uiGameCurrency_Amount )
		&& GET( m_uiShell_Amount )
		&& GET( m_strDateTime )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_GN_CHANGE_GAME_CURRENCY_REQ, obj, ks )
{
	return PUT( m_kGarenaREQ )
		&& PUT( m_PurchaserInfo )
		&& PUT( m_iResult )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_GN_CHANGE_GAME_CURRENCY_REQ, obj, ks )
{
	return GET( m_kGarenaREQ )
		&& GET( m_PurchaserInfo )
		&& GET( m_iResult )
		;
}


SERIALIZE_DEFINE_PUT( KEBILL_GN_CHANGE_GAME_CURRENCY_ACK, obj, ks )
{
	return PUT( m_kGarenaACK )
		&& PUT( m_AddCashInfo )
		&& PUT( m_bIsOffLine )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_GN_CHANGE_GAME_CURRENCY_ACK, obj, ks )
{
	return GET( m_kGarenaACK )
		&& GET( m_AddCashInfo )
		&& GET( m_bIsOffLine )
		;
}


SERIALIZE_DEFINE_PUT( KEBILL_LOGIN_CHECK_BALANCE_REQ, obj, ks )
{
	return PUT( m_uiPublisherUID )
		&& PUT( m_wstrCharName )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_LOGIN_CHECK_BALANCE_REQ, obj, ks )
{
	return GET( m_uiPublisherUID )
		&& GET( m_wstrCharName )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_LOGIN_CHECK_BALANCE_ACK, obj, ks )
{
	return PUT( m_uiPublisherUID )
		&& PUT( m_iOK )
		&& PUT( m_ulBalance )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES		
		&& PUT( m_bOnlyType )
		&& PUT( m_iCashType )
		&& PUT( m_GlobalCashInfo )
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& PUT( m_wstrCharName )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_LOGIN_CHECK_BALANCE_ACK, obj, ks )
{
	return GET( m_uiPublisherUID )
		&& GET( m_iOK )
		&& GET( m_ulBalance )
#ifdef SERV_SUPPORT_SEVERAL_CASH_TYPES		
		&& GET( m_bOnlyType )
		&& GET( m_iCashType )
		&& GET( m_GlobalCashInfo )
#endif // SERV_SUPPORT_SEVERAL_CASH_TYPES
		&& GET( m_wstrCharName )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ, obj, ks )
{
	return PUT( m_uiGarenaUID )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ, obj, ks )
{
	return GET( m_uiGarenaUID )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK, obj, ks )
{
	return PUT( m_iResult )
		&& PUT( m_iUserUID )
		&& PUT( m_wstrUserID )	
		&& PUT( m_wstrUserName )
		&& PUT( m_uiPublisherUID )
		&& PUT( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK, obj, ks )
{
	return GET( m_iResult )
		&& GET( m_iUserUID )
		&& GET( m_wstrUserID )	
		&& GET( m_wstrUserName )
		&& GET( m_uiPublisherUID )
		&& GET( m_iSessionUID )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ, obj, ks )
{
	return PUT( m_kGNGameCurrencyREQ )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ, obj, ks )
{
	return GET( m_kGNGameCurrencyREQ )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK, obj, ks )
{
	return PUT( m_kGNGameCurrencyREQ )
		&& PUT( m_iResult )
		&& PUT( m_iUserUID )	
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrUserName )
		&& PUT( m_uiPublisherUID )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK, obj, ks )
{
	return GET( m_kGNGameCurrencyREQ )
		&& GET( m_iResult )
		&& GET( m_iUserUID )	
		&& GET( m_wstrUserID )
		&& GET( m_wstrUserName )
		&& GET( m_uiPublisherUID )
		;
}


SERIALIZE_DEFINE_PUT( KEGS_EXCHANGE_CASH_NOT, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPoint )
		&& PUT( m_AddBonusCashInfo )
		;
}

SERIALIZE_DEFINE_GET( KEGS_EXCHANGE_CASH_NOT, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPoint )
		&& GET( m_AddBonusCashInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEBILL_GARENA_PREPARE_PRESENT_CHECK_REQ, obj, ks )
{
	return PUT( m_iUserUID )	
		&& PUT( m_kEGSPresentCashItemREQ )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_GARENA_PREPARE_PRESENT_CHECK_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_kEGSPresentCashItemREQ )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_GARENA_PREPARE_PRESENT_CHECK_ACK, obj, ks )
{
	return PUT( m_iOK )	
		&& PUT( m_kEGSPresentCashItemREQ )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_GARENA_PREPARE_PRESENT_CHECK_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kEGSPresentCashItemREQ )
		;
}

#endif //SERV_COUNTRY_PH


//{{ 2013. 09. 23	최육사	일본 이벤트 중계DB작업
//#ifdef SERV_RELAY_DB_CONNECTION
SERIALIZE_DEFINE_PUT( KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT, obj, ks )
{
	return PUT( m_wstrUserID )
		&& PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrNickName )
		&& PUT( m_cLevel )
		&& PUT( m_cUnitClass )
		&& PUT( m_bDeletedUnit )
		&& PUT( m_wstrUserRegDate )
		&& PUT( m_wstrUnitRegDate )
		&& PUT( m_iUpdateType )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_USER_INFO_TO_RELAY_DB_JP_EVENT_NOT, obj, ks )
{
	return GET( m_wstrUserID )
		&& GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrNickName )
		&& GET( m_cLevel )
		&& GET( m_cUnitClass )
		&& GET( m_bDeletedUnit )
		&& GET( m_wstrUserRegDate )
		&& GET( m_wstrUnitRegDate )
		&& GET( m_iUpdateType )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_ACK, obj, ks )
{
	return PUT( m_vecRewardInfo )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHECK_REWARD_FROM_RELAY_DB_JP_EVENT_ACK, obj, ks )
{
	return GET( m_vecRewardInfo )
		;
}


SERIALIZE_DEFINE_PUT( KEBILL_REWARD_COUPON_JP_EVENT_REQ, obj, ks )
{
	return PUT( m_kRewardInfo )
		&& PUT( m_kUseCouponPacket )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_REWARD_COUPON_JP_EVENT_REQ, obj, ks )
{
	return GET( m_kRewardInfo )
		&& GET( m_kUseCouponPacket )
		;
}

SERIALIZE_DEFINE_PUT( KEBILL_REWARD_COUPON_JP_EVENT_ACK, obj, ks )
{
	return PUT( m_kRewardInfo )
		&& PUT( m_kUseCouponPacket )
		;
}

SERIALIZE_DEFINE_GET( KEBILL_REWARD_COUPON_JP_EVENT_ACK, obj, ks )
{
	return GET( m_kRewardInfo )
		&& GET( m_kUseCouponPacket )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT, obj, ks )
{
	return PUT( m_iRID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UPDATE_REWARD_COMPLETE_JP_EVENT_NOT, obj, ks )
{
	return GET( m_iRID )
		;
}
//#endif SERV_RELAY_DB_CONNECTION
//}}

#ifdef SERV_HALLOWEEN_PUMPKIN_FAIRY_PET
SERIALIZE_DEFINE_PUT( KDBE_CHANGE_PET_ID_REQ, obj, ks )
{
	return PUT( m_iPetUID )
		&& PUT( m_iBeforePetID )
		&& PUT( m_iAfterPetID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_PET_ID_REQ, obj, ks )
{
	return GET( m_iPetUID )
		&& GET( m_iBeforePetID )
		&& GET( m_iAfterPetID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_PET_ID_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iPetUID )
		&& PUT( m_iBeforePetID )
		&& PUT( m_iAfterPetID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_PET_ID_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iPetUID )
		&& GET( m_iBeforePetID )
		&& GET( m_iAfterPetID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANGE_PET_ID_LOG_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iPetUID )
		&& PUT( m_iBeforePetID )
		&& PUT( m_iAfterPetID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANGE_PET_ID_LOG_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iPetUID )
		&& GET( m_iBeforePetID )
		&& GET( m_iAfterPetID )
		;
}
#endif //SERV_HALLOWEEN_PUMPKIN_FAIRY_PET

#ifdef SERV_PERIOD_PET
SERIALIZE_DEFINE_PUT( KDBE_RELEASE_PET_REQ, obj, ks )
{
	return PUT( m_iPetUID )
		&& PUT( m_iUnitUID )
		;
}
SERIALIZE_DEFINE_GET( KDBE_RELEASE_PET_REQ, obj, ks )
{
	return GET( m_iPetUID )
		&& GET( m_iUnitUID )
		;
}
#endif SERV_PERIOD_PET

#ifdef SERV_COUPON_EVENT
SERIALIZE_DEFINE_PUT( KDBE_COUPON_ENTRY_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iCouponType )
		&& PUT( m_kItemQuantityUpdate )
        && PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_COUPON_ENTRY_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iCouponType )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_COUPON_ENTRY_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdate )
        && PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_COUPON_ENTRY_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KELOG_COUPON_EVENT_NOT, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iCouponType )
		;
}

SERIALIZE_DEFINE_GET( KELOG_COUPON_EVENT_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iCouponType )
		;
}
#endif SERV_COUPON_EVENT

#ifdef SERV_GUILD_FIND_AD
SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_FIND_AD_LIST_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cSortType )
		&& PUT( m_uiViewPage )
		;
}
SERIALIZE_DEFINE_GET( KELG_GET_GUILD_FIND_AD_LIST_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_cSortType )
		&& GET( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_PUT( KELG_REGISTRATION_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iGuildMasterUnitUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrMessage )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		;
}
SERIALIZE_DEFINE_GET( KELG_REGISTRATION_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iGuildMasterUnitUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrMessage )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KELG_REGISTRATION_GUILD_INVITE_MSG_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrNickName )
		&& PUT( m_wstrMessage )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		;
}
SERIALIZE_DEFINE_GET( KELG_REGISTRATION_GUILD_INVITE_MSG_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrNickName )
		&& GET( m_wstrMessage )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_REGISTRATION_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iGuildMasterUnitUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_wstrMessage )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		&& PUT( m_bExistExpiredGuildInviteMsg )
		;
}
SERIALIZE_DEFINE_GET( KDBE_REGISTRATION_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iGuildMasterUnitUID )
		&& GET( m_iUnitUID )
		&& GET( m_wstrMessage )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		&& GET( m_bExistExpiredGuildInviteMsg )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_REGISTRATION_GUILD_INVITE_MSG_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iGuildMasterUnitUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_iUnitLevel )
		&& PUT( m_wstrNickName )
        && PUT( m_wstrMessage )
		&& PUT( m_wstrMsgRegDate )
		&& PUT( m_wstrMsgEndDate )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost ) 
		&& PUT( m_bExistExpiredGuildInviteMsg )
		;
}
SERIALIZE_DEFINE_GET( KDBE_REGISTRATION_GUILD_INVITE_MSG_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iGuildMasterUnitUID )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_iUnitLevel )
		&& GET( m_wstrNickName )
		&& GET( m_wstrMessage )
		&& GET( m_wstrMsgRegDate )
		&& GET( m_wstrMsgEndDate )
		&& GET( m_sPeriod )
		&& GET( m_iCost ) 
		&& GET( m_bExistExpiredGuildInviteMsg )		
		;
}

SERIALIZE_DEFINE_PUT( KDBE_MODIFY_REG_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_iUnitLevel )
		&& PUT( m_wstrNickName )
		&& PUT( m_wstrMessage )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		;
}
SERIALIZE_DEFINE_GET( KDBE_MODIFY_REG_GUILD_INVITE_MSG_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_iUnitLevel )
		&& GET( m_wstrNickName )
		&& GET( m_wstrMessage )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KELG_REGISTRATION_GUILD_FIND_AD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrFindAdMessage )
		&& PUT( m_sPeriod )		
		&& PUT( m_iCost )
		;
}
SERIALIZE_DEFINE_GET( KELG_REGISTRATION_GUILD_FIND_AD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrFindAdMessage )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KELG_REGISTRATION_GUILD_FIND_AD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrFindAdMessage )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		;
}
SERIALIZE_DEFINE_GET( KELG_REGISTRATION_GUILD_FIND_AD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrFindAdMessage )
		&& GET( m_sPeriod )		
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_REGISTRATION_GUILD_FIND_AD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrFindAdMessage )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		&& PUT( m_bExistExpiredFindAd )
		;
}
SERIALIZE_DEFINE_GET( KDBE_REGISTRATION_GUILD_FIND_AD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrFindAdMessage )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		&& GET( m_bExistExpiredFindAd )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_REGISTRATION_GUILD_FIND_AD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_iUnitLevel )
		&& PUT( m_wstrNickName )
		&& PUT( m_wstrFindAdMessage )
		&& PUT( m_wstrFindAdRegDate )
		&& PUT( m_wstrFindAdEndDate )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		&& PUT( m_bExistExpiredFindAd )
		;
}
SERIALIZE_DEFINE_GET( KDBE_REGISTRATION_GUILD_FIND_AD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_iUnitLevel )
		&& GET( m_wstrNickName )
		&& GET( m_wstrFindAdMessage )
		&& GET( m_wstrFindAdRegDate )
		&& GET( m_wstrFindAdEndDate )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		&& GET( m_bExistExpiredFindAd )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_MODIFY_REG_GUILD_FIND_AD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrFindAdMessage )
		&& PUT( m_sPeriod )
		&& PUT( m_iCost )
		;
}
SERIALIZE_DEFINE_GET( KDBE_MODIFY_REG_GUILD_FIND_AD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrFindAdMessage )
		&& GET( m_sPeriod )
		&& GET( m_iCost )
		;
}

SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_INVITE_USER_LIST_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID)
		&& PUT( m_uiViewPage )
		;
}
SERIALIZE_DEFINE_GET( KELG_GET_GUILD_INVITE_USER_LIST_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_INVITE_USER_LIST_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_uiTotalPage )
		&& PUT( m_uiViewPage )
		&& PUT( m_vecGuildInviteUserList )
		;
}
SERIALIZE_DEFINE_GET( KELG_GET_GUILD_INVITE_USER_LIST_ACK, obj, ks )
{
	return GET( m_iOK)
		&& GET( m_uiTotalPage )
		&& GET( m_uiViewPage )
		&& GET( m_vecGuildInviteUserList )
		;
}

SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_INVITE_GUILD_LIST_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_uiViewPage )
		;
}
SERIALIZE_DEFINE_GET( KELG_GET_GUILD_INVITE_GUILD_LIST_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_uiViewPage )
		;
}

SERIALIZE_DEFINE_PUT( KELG_GET_GUILD_INVITE_GUILD_LIST_ACK, obj, ks )
{
	return PUT( m_uiTotalPage )
		&& PUT( m_uiViewPage )
		&& PUT( m_vecGuildInviteGuildList )
		;
}
SERIALIZE_DEFINE_GET( KELG_GET_GUILD_INVITE_GUILD_LIST_ACK, obj, ks )
{
	return GET( m_uiTotalPage )
		&& GET( m_uiViewPage )
		&& GET( m_vecGuildInviteGuildList )
		;
}

SERIALIZE_DEFINE_PUT( KELG_ACCEPT_INVITE_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kLoginGuildMember )
		&& PUT( m_bDeleteApplyJoin )
		&& PUT( m_bDeleteFindAd )
		&& PUT( m_bDeleteInviteMsg )
		;
}
SERIALIZE_DEFINE_GET( KELG_ACCEPT_INVITE_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_kLoginGuildMember )
		&& GET( m_bDeleteApplyJoin )
		&& GET( m_bDeleteFindAd )
		&& GET( m_bDeleteInviteMsg )
		;
}

SERIALIZE_DEFINE_PUT( KELG_ACCEPT_INVITE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iGuildMasterUnitUID )
		;
}
SERIALIZE_DEFINE_GET( KELG_ACCEPT_INVITE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iGuildMasterUnitUID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_ACCEPT_INVITE_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_kLoginGuildMember )
		;
}
SERIALIZE_DEFINE_GET( KDBE_ACCEPT_INVITE_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_kLoginGuildMember )
		;
}

SERIALIZE_DEFINE_PUT( KELG_CANCEL_INVITE_MSG_REQ, obj, ks )
{
	return PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iDeletedUnitUID )
		&& PUT( m_bDeleteChar )
		;
}
SERIALIZE_DEFINE_GET( KELG_CANCEL_INVITE_MSG_REQ, obj, ks )
{
	return GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iDeletedUnitUID )
		&& GET( m_bDeleteChar )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CANCEL_INVITE_MSG_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iGuildUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iDeletedUnitUID )
		&& PUT( m_bDeleteChar )
		;
}
SERIALIZE_DEFINE_GET( KDBE_CANCEL_INVITE_MSG_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iGuildUID )
		&& GET( m_iUnitUID )
		&& GET( m_iDeletedUnitUID )
		&& GET( m_bDeleteChar )
		;
}

SERIALIZE_DEFINE_PUT( KELG_DELETE_GUILD_FIND_AD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT (m_bDeleteChar )
		;
}
SERIALIZE_DEFINE_GET( KELG_DELETE_GUILD_FIND_AD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET (m_bDeleteChar )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_DELETE_GUILD_FIND_AD_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iUnitUID )
		&& PUT( m_bDeleteChar )
		;
}
SERIALIZE_DEFINE_GET( KDBE_DELETE_GUILD_FIND_AD_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iUnitUID )
		&& GET( m_bDeleteChar )
		;
}
#endif SERV_GUILD_FIND_AD

#ifdef SERV_DUNGEON_ROOM_USER_CHECK
SERIALIZE_DEFINE_PUT( KERM_CHECK_DUNGEON_ROOM_USER_REQ, obj, ks )
{
	return PUT( m_iRoomUID )
		;
}

SERIALIZE_DEFINE_GET( KERM_CHECK_DUNGEON_ROOM_USER_REQ, obj, ks )
{
	return GET( m_iRoomUID )
		;
}
#endif SERV_DUNGEON_ROOM_USER_CHECK

#ifdef SERV_READY_TO_SOSUN_EVENT
SERIALIZE_DEFINE_PUT( KDBE_READY_TO_SOSUN_EVENT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecItemInfo )
		&& PUT( m_iFirstUnitClass )
		;
}
SERIALIZE_DEFINE_GET( KDBE_READY_TO_SOSUN_EVENT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecItemInfo )
		&& GET( m_iFirstUnitClass )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_READY_TO_SOSUN_EVENT_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_mapInsertedItem )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_mapItemInfo )
		&& PUT( m_iFirstUnitClass )
		;
}
SERIALIZE_DEFINE_GET( KDBE_READY_TO_SOSUN_EVENT_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_mapInsertedItem )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_mapItemInfo )
		&& GET( m_iFirstUnitClass )
		;
}
#endif SERV_READY_TO_SOSUN_EVENT

#ifdef SERV_TOUR_TICKET_EVENT
SERIALIZE_DEFINE_PUT( KDBE_REGIST_TOUR_TICKET_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_REGIST_TOUR_TICKET_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		;
}
#endif SERV_TOUR_TICKET_EVENT

#ifdef SERV_RELATIONSHIP_EVENT_INT
SERIALIZE_DEFINE_PUT( KDBE_EVENT_PROPOSE_USER_FIND_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrOtherNickName )
		&& PUT( m_iOtherUserUID )
		&& PUT( m_iOtherUnitUID )
		&& PUT( m_cUnitClass )
		&& PUT( m_bCouple )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_PROPOSE_USER_FIND_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrOtherNickName )
		&& GET( m_iOtherUserUID )
		&& GET( m_iOtherUnitUID )
		&& GET( m_cUnitClass )
		&& GET( m_bCouple )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ, obj, ks )
{
	return PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_REQ, obj, ks )
{
	return GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_MAKING_SUCCESS_ACCEPTOR_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_DIVORCE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iRelationTargetUserUid )
		&& PUT( m_vecUpdatedInventorySlot )
		&& PUT( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_DIVORCE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iRelationTargetUserUid )
		&& GET( m_vecUpdatedInventorySlot )
		&& GET( m_vecDeleted )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_EVENT_DIVORCE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_iRelationTargetUserUid )
		&& PUT(	m_kItemQuantityUpdate )
		&& PUT( m_vecUpdatedInventorySlot )		
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_DIVORCE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_iRelationTargetUserUid )
		&& GET( m_kItemQuantityUpdate )
		&& GET( m_vecUpdatedInventorySlot )		
		;
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_RECRUIT_EVENT_BASE
SERIALIZE_DEFINE_PUT( KDBE_USE_RECRUIT_TICKET_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		;
}
SERIALIZE_DEFINE_GET( KDBE_USE_RECRUIT_TICKET_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_REGISTER_RECRUITER_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrNickname )		
		;
}
SERIALIZE_DEFINE_GET( KDBE_REGISTER_RECRUITER_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrNickname )		
		;
}
SERIALIZE_DEFINE_PUT( KDBE_REGISTER_RECRUITER_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_kRecruiterUnitInfo )		
		;
}
SERIALIZE_DEFINE_GET( KDBE_REGISTER_RECRUITER_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_kRecruiterUnitInfo )		
		;
}

SERIALIZE_DEFINE_PUT( KELG_RECRUIT_RECRUITER_INFO_NOT, obj, ks )
{
	return PUT( m_iSenderUnitUID )
		&& PUT( m_iReceiverUnitUID )
		&& PUT(	m_cType )
		&& PUT( m_iData )		
		;
}
SERIALIZE_DEFINE_GET( KELG_RECRUIT_RECRUITER_INFO_NOT, obj, ks )
{
	return GET( m_iSenderUnitUID )
		&& GET( m_iReceiverUnitUID )
		&& GET( m_cType )
		&& GET( m_iData )		
		;
}
SERIALIZE_DEFINE_PUT( KDBE_GET_RECRUIT_RECRUITER_LIST_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		;
}
SERIALIZE_DEFINE_GET( KDBE_GET_RECRUIT_RECRUITER_LIST_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		;
}
#endif SERV_RECRUIT_EVENT_BASE

#ifdef SERV_EVENT_CHARACTER_QUEST_RANKING
SERIALIZE_DEFINE_PUT( KDBE_SET_EVENT_INFO_NOT, obj, ks )
{
	return PUT( m_iQuestID )
		&& PUT( m_iUnitType )
		;
}
SERIALIZE_DEFINE_GET( KDBE_SET_EVENT_INFO_NOT, obj, ks )
{
	return GET( m_iQuestID )
		&& GET( m_iUnitType )
		;
}
#endif //SERV_EVENT_CHARACTER_QUEST_RANKING

#ifdef SERV_CONTENT_MANAGER_INT
SERIALIZE_DEFINE_PUT( KDBE_GET_CASHSHOP_ON_OFF_INFO_REQ, obj, ks )
{
	return PUT( m_bFirstCashshopOnOffCheck )
		&& PUT( m_iReleaseTick )
		&& PUT( m_bEnableCashshop )
		;
}
SERIALIZE_DEFINE_GET( KDBE_GET_CASHSHOP_ON_OFF_INFO_REQ, obj, ks )
{
	return GET( m_bFirstCashshopOnOffCheck )
		&& GET( m_iReleaseTick )
		&& GET( m_bEnableCashshop )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_CASHSHOP_ON_OFF_INFO_ACK, obj, ks )
{
	return PUT( m_iReleaseTick )
		&& PUT( m_bEnableCashshop )
		&& PUT( m_iOK )
		;
}
SERIALIZE_DEFINE_GET( KDBE_GET_CASHSHOP_ON_OFF_INFO_ACK, obj, ks )
{
	return GET( m_iReleaseTick )
		&& GET( m_bEnableCashshop )
		&& GET( m_iOK )
		;
}
#endif SERV_CONTENT_MANAGER_INT

#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
SERIALIZE_DEFINE_PUT( KDBE_EVENT_DB_SCRIPT_ACK, obj, ks )
{
	return PUT( m_mapEventDBData )	
		&& PUT( m_mapDBRewardData )
		;
}

SERIALIZE_DEFINE_GET( KDBE_EVENT_DB_SCRIPT_ACK, obj, ks )
{
	return GET( m_mapEventDBData )
		&& GET( m_mapDBRewardData )
		;
}
#endif //SERV_EVENT_DB_CONTROL_SYSTEM
#ifdef SERV_ADD_EVENT_DB
SERIALIZE_DEFINE_PUT( KDBE_GAME_SELECT_UNIT_EVENT_DATA_NOT, obj, ks )
{
	return PUT( m_iUserUID )	
		&& PUT( m_kSelectUnitAck )
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& PUT( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}

SERIALIZE_DEFINE_GET( KDBE_GAME_SELECT_UNIT_EVENT_DATA_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_kSelectUnitAck )
#ifdef SERV_GLOBAL_EVENT_TABLE
		&& GET( m_mapGlobalEventData )
#endif //SERV_GLOBAL_EVENT_TABLE
		;
}

SERIALIZE_DEFINE_PUT( KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT, obj, ks )
{
	return PUT( m_iUserUID )	
		&& PUT( m_kSelectUnitAck )
		;
}

SERIALIZE_DEFINE_GET( KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_EVENT_DATA_NOT, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_kSelectUnitAck )
		;
}

#endif //SERV_ADD_EVENT_DB
#ifdef SERV_NEW_YEAR_EVENT_2014
SERIALIZE_DEFINE_PUT( KDBE_2013_EVENT_MISSION_COMPLETE_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_ucLevel )
		;
}
SERIALIZE_DEFINE_GET( KDBE_2013_EVENT_MISSION_COMPLETE_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_ucLevel )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_2013_EVENT_MISSION_COMPLETE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_ucLevel )
		;
}
SERIALIZE_DEFINE_GET( KDBE_2013_EVENT_MISSION_COMPLETE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_ucLevel )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_2014_EVENT_MISSION_COMPLETE_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iNewYearMissionStepID )
		;
}
SERIALIZE_DEFINE_GET( KDBE_2014_EVENT_MISSION_COMPLETE_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iNewYearMissionStepID )
		;
}
#endif SERV_NEW_YEAR_EVENT_2014

#ifdef SERV_USE_GM_TOOL_INFO
SERIALIZE_DEFINE_PUT( KDBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT, obj, ks )
{
	return PUT( m_CurrentItemTempletNameMap )
		;
}

SERIALIZE_DEFINE_GET( KDBE_USE_GM_TOOL_INSERT_ITEM_INFO_NOT, obj, ks )
{
	return GET( m_CurrentItemTempletNameMap )
		;
}
#endif //SERV_USE_GM_TOOL_INFO


//{{ 2012. 09. 03	임홍락	글로벌 미션 매니저
#ifdef SERV_GLOBAL_MISSION_MANAGER
SERIALIZE_DEFINE_PUT( KEGB_GET_GLOBAL_MISSION_INFO_REQ, obj, ks )
{
	return PUT( m_iServerUID )
		;
}
SERIALIZE_DEFINE_GET( KEGB_GET_GLOBAL_MISSION_INFO_REQ, obj, ks )
{
	return GET( m_iServerUID )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_GET_GLOBAL_MISSION_INFO_ACK, obj, ks )
{
	return PUT( m_iFlag )
		&& PUT( m_wstrGlobalMissionDay )
		&& PUT( m_mapGlobalMissionInfo )
		;
}
SERIALIZE_DEFINE_GET( KEGB_GET_GLOBAL_MISSION_INFO_ACK, obj, ks )
{
	return GET( m_iFlag )
		&& GET( m_wstrGlobalMissionDay )
		&& GET( m_mapGlobalMissionInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_GET_GLOBAL_MISSION_INFO_REQ, obj, ks )
{
	return PUT( m_mapGlobalMissionInfo )
		;
}
SERIALIZE_DEFINE_GET( KDBE_GET_GLOBAL_MISSION_INFO_REQ, obj, ks )
{
	return GET( m_mapGlobalMissionInfo )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_UPDATE_GLOBAL_MISSION_INFO_REQ, obj, ks )
{
	return PUT( m_wstrGlobalMissionDay )
		&& PUT( m_iNowCount )
		;
}
SERIALIZE_DEFINE_GET( KDBE_UPDATE_GLOBAL_MISSION_INFO_REQ, obj, ks )
{
	return GET( m_wstrGlobalMissionDay )
		&& GET( m_iNowCount )
		;
}

SERIALIZE_DEFINE_PUT( KEGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT, obj, ks )
{

	return PUT( m_mapGlobalMissionStartTime )
		;
}
SERIALIZE_DEFINE_GET( KEGB_UPDATE_GLOBAL_MISSION_START_TIME_NOT, obj, ks )
{
	return GET( m_mapGlobalMissionStartTime )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_INSERT_CONNECT_EVENT_REQ, obj, ks )
{
	return PUT( m_wstrEventName )
		&& PUT( m_iEventType )
		&& PUT( m_wstrNextDayStartTime )
		&& PUT( m_wstrNextDayEndTime )
		&& PUT( m_iScriptID )
		;
}
SERIALIZE_DEFINE_GET( KDBE_INSERT_CONNECT_EVENT_REQ, obj, ks )
{ 
	return GET( m_wstrEventName )
		&& GET( m_iEventType )
		&& GET( m_wstrNextDayStartTime )
		&& GET( m_wstrNextDayEndTime )
		&& GET( m_iScriptID )
		;
}

SERIALIZE_DEFINE_PUT( KDBE_REGIST_GLOBAL_MISSION_CLEAR_NOT, obj, ks )
{
	return PUT( m_wstrGlobalMissionDay )
		&& PUT( m_iUnitUID )
		;
}
SERIALIZE_DEFINE_GET( KDBE_REGIST_GLOBAL_MISSION_CLEAR_NOT, obj, ks )
{
	return GET( m_wstrGlobalMissionDay )
		&& GET( m_iUnitUID )
		;
}
#endif SERV_GLOBAL_MISSION_MANAGER
//}} 2012. 09. 03	임홍락	글로벌 미션 매니저

#ifdef SERV_EVENT_CHECK_POWER
SERIALIZE_DEFINE_PUT( KDBE_START_CHECK_POWER_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_ucCheckPowerCount )
		&& PUT( m_iCheckPowerTime )
		&& PUT( m_ucCheckPowerScore )
		;
}
SERIALIZE_DEFINE_GET( KDBE_START_CHECK_POWER_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_ucCheckPowerCount )
		&& GET( m_iCheckPowerTime )
		&& GET( m_ucCheckPowerScore )
		;
}
#endif SERV_EVENT_CHECK_POWER

#ifdef SERV_UNLIMITED_SECOND_CHANGE_JOB
SERIALIZE_DEFINE_PUT( KDBE_UNLIMITED_SECOND_CHANGE_JOB_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iOldUnitClass )
		&& PUT( m_iNewUnitClass )
		;
}

SERIALIZE_DEFINE_GET( KDBE_UNLIMITED_SECOND_CHANGE_JOB_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iOldUnitClass )
		&& GET( m_iNewUnitClass )
		;
}
#endif	//	SERV_UNLIMITED_SECOND_CHANGE_JOB

#ifdef SERV_SHARING_BANK_EVENT
SERIALIZE_DEFINE_PUT( KDBE_SHARING_BANK_EVENT_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_iUnitUID )
		&& PUT( m_iItemID )
		;
}

SERIALIZE_DEFINE_GET( KDBE_SHARING_BANK_EVENT_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_iUnitUID )
		&& GET( m_iItemID )
		;
}
#endif

#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING

SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_ONOFF_NPCSHOP_REQ, obj, ks )
{
	return PUT( m_mapTimeControlItem_StaticDBReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_ONOFF_NPCSHOP_REQ, obj, ks )
{
	return GET( m_mapTimeControlItem_StaticDBReleaseTick )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_ONOFF_NPCSHOP_ACK, obj, ks )
{
	return PUT( m_mapTimeControlItem_StaticDBReleaseTick )
		&& PUT( m_mapGetItemOnOff )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_ONOFF_NPCSHOP_ACK, obj, ks )
{
	return GET( m_mapTimeControlItem_StaticDBReleaseTick )
		&& GET( m_mapGetItemOnOff )
		;
}
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING
//}}
#ifdef SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING

SERIALIZE_DEFINE_PUT( KDBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ, obj, ks )
{
	return PUT( m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_REQ, obj, ks )
{
	return GET( m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK, obj, ks )
{
	return PUT( m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick )
		&& PUT( m_mapGetCubeInItemMappingOnOff )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_CUBE_IN_ITEM_MAPPING_ONOFF_ACK, obj, ks )
{
	return GET( m_mapTimeControlCubeInItemMapping_StaticDBReleaseTick )
		&& GET( m_mapGetCubeInItemMappingOnOff )
		;
}
#endif SERV_CUBE_IN_ITEM_MAPPING_BY_DBTIME_SETTING
//}}


#ifdef SERV_ITEM_ACTION_BY_DBTIME_SETTING
SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_ONOFF_NPCSHOP_REQ, obj, ks )
{
	return PUT( m_mapTimeControlItem_StaticDBReleaseTick )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_ONOFF_NPCSHOP_REQ, obj, ks )
{
	return GET( m_mapTimeControlItem_StaticDBReleaseTick )
		;
}


SERIALIZE_DEFINE_PUT( KDBE_GET_ITEM_ONOFF_NPCSHOP_ACK, obj, ks )
{
	return PUT( m_mapTimeControlItem_StaticDBReleaseTick )
		&& PUT( m_mapGetItemOnOff )
		;
}

SERIALIZE_DEFINE_GET( KDBE_GET_ITEM_ONOFF_NPCSHOP_ACK, obj, ks )
{
	return GET( m_mapTimeControlItem_StaticDBReleaseTick )
		&& GET( m_mapGetItemOnOff )
		;
}
#endif SERV_ITEM_ACTION_BY_DBTIME_SETTING

#ifdef SERV_EVENT_PET_INVENTORY
SERIALIZE_DEFINE_PUT( KDBE_EVENT_PET_EVENT_FOOD_EAT_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrPetName )
		&& PUT( m_iPetUID )
		&& PUT( m_iPetID )
		&& PUT( m_bEventFoodEat )
		&& PUT( m_vecInventorySlotInfo )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_PET_EVENT_FOOD_EAT_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrPetName )
		&& GET( m_iPetUID )
		&& GET( m_iPetID )
		&& GET( m_bEventFoodEat )
		&& GET( m_vecInventorySlotInfo )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_EVENT_PET_EVENT_FOOD_EAT_ACK, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrPetName )
		&& PUT( m_iPetUID )
		&& PUT( m_iPetID )
		&& PUT( m_bEventFoodEat )
		&& PUT( m_vecInventorySlotInfo )
		&& PUT( m_iOK )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_PET_EVENT_FOOD_EAT_ACK, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrPetName )
		&& GET( m_iPetUID )
		&& GET( m_iPetID )
		&& GET( m_bEventFoodEat )
		&& GET( m_vecInventorySlotInfo )
		&& GET( m_iOK )
		;
}
#endif SERV_EVENT_PET_INVENTORY

#ifdef SERV_EVENT_CHUNG_GIVE_ITEM
SERIALIZE_DEFINE_PUT( KDBE_EVENT_CHUNG_GIVE_ITEM_REQ, obj, ks )
{
	return PUT( m_wstrGiveItemTime_One )
		&& PUT( m_wstrGiveItemTime_Two )
		&& PUT( m_wstrGiveItemTime_Tree )
		&& PUT( m_iUnitUID )
		&& PUT( m_bTwoGiveItem )
		&& PUT( m_iChoice )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_CHUNG_GIVE_ITEM_REQ, obj, ks )
{
	return GET( m_wstrGiveItemTime_One )
		&& GET( m_wstrGiveItemTime_Two )
		&& GET( m_wstrGiveItemTime_Tree )
		&& GET( m_iUnitUID )
		&& GET( m_bTwoGiveItem )
		&& GET( m_iChoice )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_EVENT_CHUNG_GIVE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_bTwoGiveItem )
		&& PUT( m_iChoice )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_CHUNG_GIVE_ITEM_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_bTwoGiveItem )
		&& GET( m_iChoice )
		;
}
#endif SERV_EVENT_CHUNG_GIVE_ITEM

#ifdef SERV_EVENT_COBO_DUNGEON_AND_FIELD
SERIALIZE_DEFINE_PUT( KDBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrButtonClickTime_One )
		&& PUT( m_bItemGive )
		&& PUT( m_iDungeonClearCount )
		&& PUT( m_iFieldMonsterKillCount )
		&& PUT( m_WeekEndItem )
		&& PUT( m_Nowday )
		&& PUT( m_bStartButton )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_COBO_DUNGEON_AND_FIELD_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrButtonClickTime_One )
		&& GET( m_bItemGive )
		&& GET( m_iDungeonClearCount )
		&& GET( m_iFieldMonsterKillCount )
		&& GET( m_WeekEndItem )
		&& GET( m_Nowday )
		&& GET( m_bStartButton )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_EVENT_COBO_DUNGEON_AND_FIELD_ACK, obj, ks )
{
	return PUT( m_iOk )
		&& PUT( m_bWeekEndItem )
		&& PUT( m_iDungeonClearCount )
		&& PUT( m_iFieldMonsterKillCount )
		&& PUT( m_NowDay )
		&& PUT( m_wstrButtonClickTime_One )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_COBO_DUNGEON_AND_FIELD_ACK, obj, ks )
{
	return GET( m_iOk )
		&& GET( m_bWeekEndItem )
		&& GET( m_iDungeonClearCount )
		&& GET( m_iFieldMonsterKillCount )
		&& GET( m_NowDay )
		&& GET( m_wstrButtonClickTime_One )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_wstrButtonClickTime_One )
		&& PUT( m_bItemGive )
		&& PUT( m_iDungeonClearCount )
		&& PUT( m_iFieldMonsterKillCount )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_COBO_DUNGEON_AND_FIELD_NOT, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_wstrButtonClickTime_One )
		&& GET( m_bItemGive )
		&& GET( m_iDungeonClearCount )
		&& GET( m_iFieldMonsterKillCount )
		;
}
#endif SERV_EVENT_COBO_DUNGEON_AND_FIELD

#ifdef SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM
SERIALIZE_DEFINE_PUT( KDBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_REQ, obj, ks )
{
	return PUT( m_iUnitUID )
		&& PUT( m_iValenTineItemCount )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_REQ, obj, ks )
{
	return GET( m_iUnitUID )
		&& GET( m_iValenTineItemCount )
		;
}
SERIALIZE_DEFINE_PUT( KDBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_ACK, obj, ks )
{
	return PUT( m_iOk )
		&& PUT( m_iValenTineItemCount )
		;
}
SERIALIZE_DEFINE_GET( KDBE_EVENT_VALENTINE_DUNGEON_GIVE_ITEM_ACK, obj, ks )
{
	return GET( m_iOk )
		&& GET( m_iValenTineItemCount )
		;
}
#endif SERV_EVENT_VALENTINE_DUNGEON_GIVE_ITEM

#ifdef SERV_GLOBAL_EVENT_TABLE
SERIALIZE_DEFINE_PUT( KDBE_GLOBAL_EVENT_TABLE_INSERT_NOT, obj, ks )
{
	return PUT( m_mapGlobalEventData )
		;
}
SERIALIZE_DEFINE_GET( KDBE_GLOBAL_EVENT_TABLE_INSERT_NOT, obj, ks )
{
	return GET( m_mapGlobalEventData )
		;
}


#endif //SERV_GLOBAL_EVENT_TABLE

#ifdef SERV_COUNTRY_IN
SERIALIZE_DEFINE_PUT( KNaunGetCashRequestData, obj, ks )
{
	return PUT( m_wstrUserUID )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrCompanyCode )
		&& PUT( m_wstrNickName )
		&& PUT( m_ulShopBalance )
		&& PUT( m_ulContentsBalance )
		&& PUT( m_ulBonusBalance )
		&& PUT( m_ulEtcBalance )
		&& PUT( m_ulShopMileage )
		&& PUT( m_ulContentsMileage )
		&& PUT( m_wstrReturnCode )
		;
}

SERIALIZE_DEFINE_GET( KNaunGetCashRequestData, obj, ks )
{
	return GET( m_wstrUserUID )
		&& GET( m_wstrUserID )
		&& GET( m_wstrCompanyCode )
		&& GET( m_wstrNickName )
		&& GET( m_ulShopBalance )
		&& GET( m_ulContentsBalance )
		&& GET( m_ulBonusBalance )
		&& GET( m_ulEtcBalance )
		&& GET( m_ulShopMileage )
		&& GET( m_ulContentsMileage )
		&& GET( m_wstrReturnCode )
		;
}

SERIALIZE_DEFINE_PUT( KNaunBuyProductRequestData, obj, ks )
{
	return PUT( m_wstrUserUID )
		&& PUT( m_wstrUserID )
		&& PUT( m_wstrCompanyCode )
		&& PUT( m_wstrNickName )
		&& PUT( m_wstrIP )
		&& PUT( m_ulBuyType )
		&& PUT( m_wstrProductName )
		&& PUT( m_wstrProductNo )
		&& PUT( m_wstrCategory )
		&& PUT( m_ulProductCnt )
		&& PUT( m_wstrProductEtc )
		&& PUT( m_wstrToUserUID )
		&& PUT( m_wstrToUserID )
		&& PUT( m_ulPrice )
		&& PUT( m_wstrAgencyNo )
		&& PUT( m_wstrEtc2 )
		&& PUT( m_wstrEtc3 )
		&& PUT( m_wstrEtc4 )
		&& PUT( m_wstrOrderNo )
		&& PUT( m_wstrReturnCode )
		;
}

SERIALIZE_DEFINE_GET( KNaunBuyProductRequestData, obj, ks )
{
	return GET( m_wstrUserUID )
		&& GET( m_wstrUserID )
		&& GET( m_wstrCompanyCode )
		&& GET( m_wstrNickName )
		&& GET( m_wstrIP )
		&& GET( m_ulBuyType )
		&& GET( m_wstrProductName )
		&& GET( m_wstrProductNo )
		&& GET( m_wstrCategory )
		&& GET( m_ulProductCnt )
		&& GET( m_wstrProductEtc )
		&& GET( m_wstrToUserUID )
		&& GET( m_wstrToUserID )
		&& GET( m_ulPrice )
		&& GET( m_wstrAgencyNo )
		&& GET( m_wstrEtc2 )
		&& GET( m_wstrEtc3 )
		&& GET( m_wstrEtc4 )
		&& GET( m_wstrOrderNo )
		&& GET( m_wstrReturnCode )
		;
}
#endif SERV_COUNTRY_IN



#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
SERIALIZE_DEFINE_PUT( KELG_CASH_DIRECT_CHARGE_CN_REQ, obj, ks )
{
	return PUT( m_iUserUID )
		&& PUT( m_wstrServiceAccountID )
		&& PUT(	m_iUnitUID )
		&& PUT( m_wstrUnitNickName )
		&& PUT( m_wstrIP )
		;
}

SERIALIZE_DEFINE_GET( KELG_CASH_DIRECT_CHARGE_CN_REQ, obj, ks )
{
	return GET( m_iUserUID )
		&& GET( m_wstrServiceAccountID )
		&& GET(	m_iUnitUID )
		&& GET( m_wstrUnitNickName )
		&& GET( m_wstrIP )
		;
}
/*
SERIALIZE_DEFINE_PUT( KELG_CASH_DIRECT_CHARGE_CN_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrToken )
		;
}

SERIALIZE_DEFINE_GET( KELG_CASH_DIRECT_CHARGE_CN_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrToken )
		;
}
*/
SERIALIZE_DEFINE_PUT( KEGIANT_AUTH_DIRECT_CHARGE_REQ, obj, ks )
{
	return PUT( m_wstrServiceAccountID )
		&& PUT( m_uiUserUID )
		&& PUT(	m_ulGameZone )
		&& PUT( m_uiUnitUID )
		&& PUT( m_wstrUnitNickName )
		&& PUT( m_wstrIP )
		//&& PUT( m_usTokenLen )
		//&& PUT( m_cToken )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_AUTH_DIRECT_CHARGE_REQ, obj, ks )
{
	return GET( m_wstrServiceAccountID )
		&& GET( m_uiUserUID )
		&& GET(	m_ulGameZone )
		&& GET( m_uiUnitUID )
		&& GET( m_wstrUnitNickName )
		&& GET( m_wstrIP )
		//&& GET( m_usTokenLen )
		//&& GET( m_cToken )
		;
}

SERIALIZE_DEFINE_PUT( KEGIANT_AUTH_DIRECT_CHARGE_ACK, obj, ks )
{
	return PUT( m_iOK )
		&& PUT( m_wstrServiceAccountID )
		&& PUT( m_uiUserUID )
		&& PUT(	m_ulGameZone )
		&& PUT( m_uiUnitUID )
		&& PUT( m_wstrUnitNickName )
		&& PUT( m_wstrIP )
		&& PUT( m_usTokenLen )
		&& PUT( m_wstrToken )
		;
}

SERIALIZE_DEFINE_GET( KEGIANT_AUTH_DIRECT_CHARGE_ACK, obj, ks )
{
	return GET( m_iOK )
		&& GET( m_wstrServiceAccountID )
		&& GET( m_uiUserUID )
		&& GET(	m_ulGameZone )
		&& GET( m_uiUnitUID )
		&& GET( m_wstrUnitNickName )
		&& GET( m_wstrIP )
		&& GET( m_usTokenLen )
		&& GET( m_wstrToken )
		;
}
#endif //SERV_DIRECT_CHARGE_ELSWORD_CASH

#ifdef SERV_STRING_FILTER_USING_DB
SERIALIZE_DEFINE_PUT( KDBE_STRING_FILTER_UPDATE_ACK, obj, ks )
{
	return PUT( m_vecStringFilterList )
		;
}

SERIALIZE_DEFINE_GET( KDBE_STRING_FILTER_UPDATE_ACK, obj, ks )
{
	return GET( m_vecStringFilterList )
		;
}
#endif //SERV_STRING_FILTER_USING_DB