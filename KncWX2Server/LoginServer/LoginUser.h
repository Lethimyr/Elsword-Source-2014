#pragma once
#include "Actor.h"
#include "KncSend.h"

#include "Event.h"
#include "CommonPacket.h"
#include "ServerPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ActorManager.h"

#include <windows.h>    // ::GetTickCount()


SmartPointer(KLoginUser);

class KLoginUser : public KActor
{
    NiDeclareRTTI;
    DeclToStringW;
    DeclPfID;

	//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING
	struct SLoginDelayDump
	{
		DWORD		m_dwLoginTick;
		DWORD		m_dwMaxTick;
		DWORD		m_dwLoginAuthCount;
		CTime		m_tRegTime;

		SLoginDelayDump()
		{
			Reset();
		}

		void Reset()
		{
			m_dwLoginTick = ::GetTickCount();
			m_dwMaxTick = 0;
			m_dwLoginAuthCount = 0;
			m_tRegTime = CTime();
		}
	};
#endif SERV_TOONILAND_CHANNELING
	//}}

public:
    KLoginUser(void);
    virtual ~KLoginUser(void);

    // override
    virtual void Tick();

protected:  // util function, Packet Handling	
	//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
	bool AuthenticateUser( const wchar_t* szPassport, const wchar_t* szHWID, KNexonAccountInfo& kInfo );
#else
	bool AuthenticateUser( const wchar_t* szPassport, KNexonAccountInfo& kInfo );
#endif SERV_OTP_AUTH
	//}}    
	bool AuthCheckPassword( const wchar_t* szPassword, UINT32 uiPwdHash );

	//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING
	void LoginBegin();
	void LoginEnd();
	DWORD GetLoginMaxTick() const					{ return m_kLoginTickDump.m_dwMaxTick; }
	const std::wstring GetLoginMaxTickRegDate() const;
	void TickDumpReset()							{ m_kLoginTickDump.Reset(); }
#endif SERV_TOONILAND_CHANNELING
	//}}

    // packet send function
    template < typename T > bool SendPacket( unsigned short usEventID, T& data, bool bLogging = true, bool bCompress = false );
    template < class T > void _SendToAccountDB( unsigned short usEventID_, UidType anTrace[], const T& data_ );
    template < class T > void SendToAccountDB( unsigned short usEventID, const T& data );
	//{{ 2009. 11. 19  최육사	길드구조개선
	template < class T > void SendToGameDB( unsigned short usEventID, const T& data );
	//}}
    template < class T > void SendToLogDB( unsigned short usEventID, const T& data );
    template < class T > void SendToGSUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToGSUser( UidType nToGSUID, UidType nToGSUSer, unsigned short usEventID, const T& data );
	template < class T > void SendToGSCharacter( UidType nToUnitUID, unsigned short usEventID, const T& data );
    template < class T > void SendToGameServer( unsigned short usEventID, const T& data );
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	template < class T > void SendToChannelUser( UidType nTo, unsigned short usEventID, const T& data );
	template < class T > void SendToChannelServer( unsigned short usEventID, const T& data );

	// ChannelUser와는 입력 파라미터가 다름 (EPUBLISHER_ 기준으로 구성됨)
	void RequestPublisherLogin(const KEPUBLISHER_AUTHENTICATION_REQ& kPacket_);
	void HandlePublisherLoginAck(const KEPUBLISHER_AUTHENTICATION_ACK& kPacket_, OUT bool& bDoNotDisconnect);

	void RequestKOGOTPLogin(const KECH_VERIFY_ACCOUNT_REQ& kPacket_);
	void HandleKOGOTPLoginAck();

	void RequestPublisherSecurityAuth(const KEPUBLISHER_SECURITY_AUTH_REQ& kPacket_);
	void HandlePublisherSecurityAuthAck(const KEPUBLISHER_SECURITY_AUTH_ACK& kPacket_);
	template < class T > void SendToKOGBillingDB( unsigned short usEventID, const T& data );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_COUNTRY_PH
	template < class T > void SendToKOGBillingDB( unsigned short usEventID, const T& data );
#endif //SERV_COUNTRY_PH

    void SendToAccountDB( unsigned short usEventID );
	//{{ 2009. 11. 19  최육사	길드구조개선
	void SendToGameDB( unsigned short usEventID );
	//}}
    void SendToGSUser( UidType nTo, unsigned short usEventID );

//#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-08	// 해외팀 주석 처리
	template < class T > void SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data );
	void SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID );
//#endif	// SERV_LOCAL_RANKING_SYSTEM

	//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
	//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
   _DECL_ON_FUNC( ELG_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
#else
   _DECL_ON_FUNC( ELG_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
	//}}
   _DECL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}
    DECL_ON_FUNC( ELG_NEXON_USER_AUTHENTICATE_REQ );
	//{{ 2009. 5. 26  최육사	채널이동
	DECL_ON_FUNC( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ );	
	//}}
	DECL_ON_FUNC( ELG_REGISTER_USER_REQ );
	//{{ 2009. 5. 28  최육사	채널이동
	DECL_ON_FUNC( ELG_CHANNEL_CHANGE_REGISTER_USER_REQ );
	//}}

	DECL_ON_FUNC( ELG_USER_DISCONNECT_REQ );

#ifdef SERV_CHATTING_OBSERVATION_CN
	DECL_ON_FUNC( ELG_CHAT_OBSERVATION_NOT );
#endif //SERV_CHATTING_OBSERVATION_CN

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	_DECL_ON_FUNC( ELG_VERIFY_ACCOUNT_REQ, KEPUBLISHER_AUTHENTICATION_REQ );
	_DECL_ON_FUNC( ELG_SECURITY_AUTH_REQ, KEPUBLISHER_SECURITY_AUTH_REQ );
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY

#ifdef SERV_COUNTRY_CN
	DECL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ );

	DECL_ON_FUNC( ELG_VERIFY_GIANT_MATRIX_CARD_REQ );

	DECL_ON_FUNC( EBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK );
	DECL_ON_FUNC( ELG_EXCHANGE_CASH_REQ );
	DECL_ON_FUNC( EBILL_EXCHANGE_CASH_RESERVE_ACK );

	_DECL_ON_FUNC( ELG_USE_COUPON_REQ, KEBILL_USE_COUPON_REQ );
	DECL_ON_FUNC( EBILL_USE_COUPON_RESERVE_ACK );

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	DECL_ON_FUNC( ELG_CASH_DIRECT_CHARGE_CN_REQ );
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH

#ifdef SERV_INFOSERVER_ADD_WORK
	DECL_ON_FUNC( EGIANT_INFO_USERONLINE_REQ );
#endif //SERV_INFOSERVER_ADD_WORK
#endif // SERV_COUNTRY_CN

	//{{ 2010. 06. 29  최육사	게임 PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	DECL_ON_FUNC( ELG_NEXON_USER_LOGIN_TIME_OUT_NOT );
#endif SERV_PCBANG_AUTH_NEW
	//}}

	DECL_ON_FUNC( ELG_REGISTER_UNIT_NOT );
	DECL_ON_FUNC( ELG_UNIT_DISCONNECT_NOT );
	DECL_ON_FUNC( ELG_SEARCH_UNIT_REQ );
	DECL_ON_FUNC( ELG_GET_UNIT_INFO_ACK );

   _DECL_ON_FUNC( ELG_KNM_REQUEST_NEW_FRIEND_INFO_REQ, KEGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ );
    DECL_ON_FUNC( ELG_KNM_INVITE_REQ );
   _DECL_ON_FUNC( ELG_KNM_TOGETHER_REQ, KEGS_KNM_TOGETHER_REQ );
    DECL_ON_FUNC( ELG_KNM_TOGETHER_BY_UNIT_INFO_ACK );

	DECL_ON_FUNC( ELG_CHAT_REQ );
   _DECL_ON_FUNC( ELG_ADMIN_KICK_USER_REQ, KEGS_ADMIN_KICK_USER_REQ );
	//{{ 2008. 2. 4  최육사  
	DECL_ON_FUNC( ELG_SEARCH_BLACK_LIST_UNIT_REQ );
	//}}

	//{{ 2008. 3. 28  최육사  추천인	
   _DECL_ON_FUNC( ELG_RECOMMEND_USER_RESULT_NOT, KEGS_RECOMMEND_USER_RESULT_NOT );
	//}}

   //080407.hoons. 사제시스템
   _DECL_ON_FUNC( ELG_TUTORIAL_UPDATE_UNIT_INFO_NOT, KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT );
    DECL_ON_FUNC( ELG_DEL_TUTORIAL_NOT );
	DECL_ON_FUNC( ELG_REQUEST_TUTORIAL_NOT );
   _DECL_ON_FUNC( ELG_REQUEST_TUTORIAL_REPLY_NOT, KEGS_REQUEST_TUTORIAL_REPLY_NOT );

	//{{ 2008. 6. 20  최육사  선물하기
    DECL_ON_FUNC( ELG_PRESENT_CASH_ITEM_REQ );
   _DECL_ON_FUNC( ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT );
	//}}

	//{{ 2008. 7. 14  최육사  랭킹
    DECL_ON_FUNC( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ );
	DECL_ON_FUNC( ELG_NEW_RECORD_HENIR_RANKING_NOT );
	DECL_ON_FUNC( ELG_GET_WEB_RANKING_REFRESH_NOT );
	//}}

	//{{ 2008. 9. 24  최육사	우체국 확장
	DECL_ON_FUNC( ELG_FIND_USER_SEND_LETTER_NOT );
	//}}

	//{{ 2008. 10. 15  최육사	타이틀
   _DECL_ON_FUNC( ELG_INSERT_TITLE_NOT, KDBE_INSERT_TITLE_ACK );
	//}}

	//{{ 2009. 4. 1  최육사		친구차단옵션
	DECL_ON_FUNC( ELG_UPDATE_DENY_FRIEND_SHIP_NOT );
	//}}

	//{{ 2009. 4. 7  최육사		친구정보
   _DECL_ON_FUNC( ELG_FIND_FRIEND_UNIT_INFO_REQ, KEGS_UPDATE_FRIEND_INFO_REQ );
	//}}

	//{{ 2009. 4. 20  최육사	초대하기
	DECL_ON_FUNC( ELG_INVITE_PVP_ROOM_REQ );
	//}}

    DECL_ON_FUNC( ELG_REQUEST_FRIEND_NOT );

    DECL_ON_FUNC( ELG_ACCEPT_FRIEND_NOT );

    DECL_ON_FUNC( ELG_DENY_FRIEND_NOT );

    DECL_ON_FUNC( ELG_BLOCK_FRIEND_NOT );

    DECL_ON_FUNC( ELG_UNBLOCK_FRIEND_NOT );

    DECL_ON_FUNC( ELG_DELETE_FRIEND_NOT );

    DECL_ON_FUNC( ELG_FRIEND_POSITION_UPDATE_NOT );

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST

	//{{ 2009. 11. 20  최육사	길드구조개선
	DECL_ON_FUNC( ELG_CREATE_GUILD_RESULT_REQ );
	//}}

	DECL_ON_FUNC( ELG_GET_GUILD_INFO_REQ );
	//{{ 2009. 11. 19  최육사	길드구조개선
	DECL_ON_FUNC( DBE_GET_GUILD_INFO_ACK );
	//}}

	DECL_ON_FUNC( ELG_GET_GUILD_USER_LIST_REQ );

	DECL_ON_FUNC( ELG_INVITE_GUILD_REQ );
	DECL_ON_FUNC( ELG_INVITE_GUILD_FIND_USER_ACK );

	DECL_ON_FUNC( ELG_INVITE_GUILD_REPLY_REQ );
	DECL_ON_FUNC( ELG_JOIN_GUILD_REQ );

	DECL_ON_FUNC( ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ );

   _DECL_ON_FUNC( ELG_UPDATE_CHANGE_GUILD_MEMBER_GRADE_NOT, KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK );
   _DECL_ON_FUNC( ELG_UPDATE_CHANGE_GUILD_MESSAGE_NOT, KDBE_CHANGE_GUILD_MESSAGE_ACK );
   _DECL_ON_FUNC( ELG_UPDATE_KICK_GUILD_MEMBER_NOT, KDBE_KICK_GUILD_MEMBER_ACK );
   _DECL_ON_FUNC( ELG_UPDATE_CHANGE_GUILD_MEMBER_MESSAGE_NOT, KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK );
   _DECL_ON_FUNC( ELG_UPDATE_DISBAND_GUILD_NOT, KDBE_DISBAND_GUILD_ACK );

	DECL_ON_FUNC( ELG_UPDATE_GUILD_MEMBER_INFO_NOT );

	DECL_ON_FUNC( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ );
   _DECL_ON_FUNC( DLG_UPDATE_EXPAND_GUILD_MAX_MEMBER_NOT, KDBE_EXPAND_GUILD_MAX_MEMBER_ACK );

	//{{ 2009. 10. 27  최육사	길드레벨
   _DECL_ON_FUNC( ELG_UPDATE_GUILD_EXP_NOT, KDBE_UPDATE_GUILD_EXP_ACK );	
	//}}

   //{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
   DECL_ON_FUNC( ELG_CREATE_GUILD_REQ );
   DECL_ON_FUNC( DBE_CREATE_GUILD_ACK );
   DECL_ON_FUNC( ELG_CHANGE_GUILD_NAME_CHECK_REQ );
   DECL_ON_FUNC( DBE_CHANGE_GUILD_NAME_CHECK_ACK );
   DECL_ON_FUNC( ELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ );
   DECL_ON_FUNC( ELG_CHANGE_GUILD_NAME_REQ );
   DECL_ON_FUNC( DBE_CHANGE_GUILD_NAME_ACK );
   DECL_ON_FUNC( ELG_CHANGE_GUILD_NAME_RESULT_REQ );	
#endif SERV_GUILD_CHANGE_NAME
   //}}

#endif GUILD_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2009. 10. 27  최육사	길드레벨
	DECL_ON_FUNC( ELG_FIND_DELETE_CHAR_IN_RANKING_NOT );
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	DECL_ON_FUNC( ELG_GET_GUILD_SKILL_REQ );
	DECL_ON_FUNC( DBE_GET_GUILD_SKILL_ACK );

	DECL_ON_FUNC( ELG_RESET_GUILD_SKILL_REQ );
   _DECL_ON_FUNC( ELG_RESET_GUILD_SKILL_DEL_ITEM_ACK, KDBE_RESET_GUILD_SKILL_ACK );

	DECL_ON_FUNC( ELG_INIT_GUILD_SKILL_REQ );
   _DECL_ON_FUNC( ELG_INIT_GUILD_SKILL_DEL_ITEM_ACK, KDBE_INIT_GUILD_SKILL_ACK );

	DECL_ON_FUNC( ELG_INSERT_GUILD_CASH_SKILL_POINT_NOT );

	DECL_ON_FUNC( ELG_ADMIN_GET_GUILD_SKILL_POINT_REQ );

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD

	DECL_ON_FUNC( ELG_GET_GUILD_AD_LIST_REQ );
	DECL_ON_FUNC( ELG_GET_GUILD_SKILL_IN_BOARD_REQ );
	DECL_ON_FUNC( DBE_GET_GUILD_SKILL_IN_BOARD_ACK );

	DECL_ON_FUNC( ELG_REGISTRATION_GUILD_AD_REQ );
	DECL_ON_FUNC( DBE_REGISTRATION_GUILD_AD_ACK );

	DECL_ON_FUNC( ELG_MODIFY_REG_GUILD_AD_REQ );
	DECL_ON_FUNC( DBE_MODIFY_REG_GUILD_AD_ACK );

	DECL_ON_FUNC( ELG_APPLY_JOIN_GUILD_REQ );
	DECL_ON_FUNC( DBE_APPLY_JOIN_GUILD_ACK );

	DECL_ON_FUNC( ELG_GET_APPLY_JOIN_GUILD_LIST_REQ );

	DECL_ON_FUNC( ELG_ACCEPT_JOIN_GUILD_REQ );
	DECL_ON_FUNC( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK );

	DECL_ON_FUNC( ELG_DELETE_APPLY_JOIN_GUILD_REQ );
	DECL_ON_FUNC( DBE_DELETE_APPLY_JOIN_GUILD_ACK );

#endif SERV_GUILD_AD
	//}}

	//{{ 2010. 01. 11  최육사	추천인리스트
   _DECL_ON_FUNC( ELG_GET_RECOMMEND_USER_LIST_REQ, KDBE_GET_RECOMMEND_USER_LIST_ACK );
	DECL_ON_FUNC( ELG_RECOMMEND_USER_INFO_NOT );
	//}}

	//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW
	DECL_ON_FUNC( ELG_UPDATE_CCU_INFO_NOT );
#endif SERV_CCU_NEW
	//}}	
	//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT
   _DECL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_REQ, KEGS_ADMIN_SHOW_ED_CHEAT_REQ );
	DECL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_ACK );
#endif SERV_RESET_PERIOD_EVENT
	//}}
	//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	DECL_ON_FUNC( ELG_REG_REJECTED_USER_NOT );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
   _DECL_ON_FUNC( ELG_REG_TRADE_BLOCK_USER_NOT, KELG_REG_REJECTED_USER_NOT );
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	_DECL_ON_FUNC( ELG_DISCONNECT_SERVER_REPORT_NOT, KE_DISCONNECT_SERVER_REPORT_NOT );
#endif SERV_MORNITORING
	//}}
	//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
   _DECL_ON_FUNC( ELG_MODULE_INFO_UPDATE_NOT, KEGS_MODULE_INFO_UPDATE_NOT );
#endif SERV_DLL_MANAGER
	//}}
	//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	DECL_ON_FUNC( ELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}
	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
   _DECL_ON_FUNC( ELG_CHECK_MACHINE_ID_REQ, KEGS_CHECK_MACHINE_ID_REQ );
#endif SERV_MACHINE_ID_BLOCK
   //}}
	//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING
	DECL_ON_FUNC_NOPARAM( ELG_DUMP_LOGIN_USER_NOT );
#endif SERV_TOONILAND_CHANNELING
	//}}

	//{{ 2011. 12. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	DECL_ON_FUNC( ELG_INVITE_PARTY_FIND_RECEIVER_REQ );
   _DECL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
   _DECL_ON_FUNC( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

   //{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
   DECL_ON_FUNC( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ );
#endif SERV_SERVER_BUFF_SYSTEM
   //}

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
   DECL_ON_FUNC( ELG_ADMIN_DELETE_GUILD_ADD_NOT );
#endif SERV_DELETE_GUILD_ADD_CHEAT

   //{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
   DECL_ON_FUNC( ELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT );
   DECL_ON_FUNC( ELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT );
   DECL_ON_FUNC( ELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT );
#endif SERV_DELETE_HENIR_RANKING
   //}}
#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
	DECL_ON_FUNC( ELG_PCBANG_LOGOUT_NOT );
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
   _DECL_ON_FUNC( EGS_LOCAL_RANKING_INQUIRY_REQ, KLocalRankingInquiryData );
   DECL_ON_FUNC( ELG_LOCAL_RANKING_GUILD_CHANGE_INFO_NOT );
   DECL_ON_FUNC( ELG_LOCAL_RANKING_WATCH_UNIT_REQ );
   DECL_ON_FUNC( ELG_LOCAL_RANKING_WATCH_UNIT_ACK );
   DECL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_ACK );
#endif	// SERV_LOCAL_RANKING_SYSTEM

   //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
   DECL_ON_FUNC( ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ );
   _DECL_ON_FUNC( ELG_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT );
   _DECL_ON_FUNC( ELG_COUPLE_PROPOSE_AGREE_CHECK_NOT, KEGS_COUPLE_PROPOSE_AGREE_NOT );
   _DECL_ON_FUNC( ELG_COUPLE_PROPOSE_RESULT_FAIL_NOT, KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT );
   DECL_ON_FUNC( ELG_RELATIONSHIP_INFO_REQ );
   _DECL_ON_FUNC( DBE_RELATIONSHIP_INFO_GAME_DB_ACK, KEGS_RELATIONSHIP_INFO_ACK );
   _DECL_ON_FUNC( ELG_RELATIONSHIP_INFO_GAME_SERVER_ACK, KEGS_RELATIONSHIP_INFO_ACK );
   DECL_ON_FUNC( ELG_WEDDING_ITEM_FIND_INFO_REQ );
   _DECL_ON_FUNC( DBE_WEDDING_ITEM_FIND_INFO_ACK, KELG_WEDDING_ITEM_FIND_INFO_REQ );
   DECL_ON_FUNC( ELG_ADD_WEDDING_HALL_INFO_NOT );
   DECL_ON_FUNC( ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ );
   DECL_ON_FUNC( ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT );
   DECL_ON_FUNC( ELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ );
   DECL_ON_FUNC( ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT );
   DECL_ON_FUNC( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT );
   DECL_ON_FUNC( ELG_CHECK_WEDDING_HALL_INFO_REQ );
   DECL_ON_FUNC( ELG_BREAK_UP_NOT );
   DECL_ON_FUNC( ELG_CHANGE_LOVE_WORD_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
   //}

#ifdef SERV_RELATIONSHIP_EVENT_INT
	DECL_ON_FUNC( ELG_EVENT_PROPOSE_CHECK_CONNECT_USER_REQ );
	_DECL_ON_FUNC( ELG_EVENT_PROPOSE_AGREE_CHECK_NOT, KEGS_EVENT_PROPOSE_AGREE_NOT );
	_DECL_ON_FUNC( ELG_EVENT_PROPOSE_RESULT_SUCCESS_NOT, KERM_EVENT_PROPOSE_RESULT_SUCCESS_NOT );
	DECL_ON_FUNC( ELG_DIVORCE_NOT );
#endif SERV_RELATIONSHIP_EVENT_INT

    DECL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY );

#ifdef SERV_COUNTRY_PH
	DECL_ON_FUNC( EBILL_LOGIN_CHECK_BALANCE_ACK );
	_DECL_ON_FUNC( ELG_GN_CHANGE_GAME_CURRENCY_NOT, KEBILL_GN_CHANGE_GAME_CURRENCY_REQ );
	DECL_ON_FUNC( EBILL_GN_CHANGE_GAME_CURRENCY_ACK );
	DECL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK );
	DECL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK );
#endif //SERV_COUNTRY_PH

#ifdef SERV_RECRUIT_EVENT_BASE
	_DECL_ON_FUNC( ELG_REGISTER_RECRUITER_NOT, KEGS_REGISTER_RECRUITER_NOT );
	_DECL_ON_FUNC( ELG_GET_RECRUIT_RECRUITER_LIST_REQ, KEGS_GET_RECRUIT_RECRUITER_LIST_ACK );
	DECL_ON_FUNC( ELG_RECRUIT_RECRUITER_INFO_NOT );
#endif SERV_RECRUIT_EVENT_BASE

    bool RoutePacket( const KEvent* pkEvent );   // CnUser, GSUser가 같이 선언하지만 상속은 아님.
    //                          ^ KEvent가 수정되지 않아야 한다. (SmartPtr로는 불가능)

    // override.
    virtual void ProcessEvent( const KEventPtr& spEvent );
    virtual void OnDestroy();   // LOGINED 상태로 빠져나오고, DB data update를 신청한다 

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	DWORD			m_dwPerformerType;
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING
private:
	SLoginDelayDump				m_kLoginTickDump;
#endif SERV_TOONILAND_CHANNELING
	//}}
};

template < typename T >
bool KLoginUser::SendPacket( unsigned short usEventID, T& data, bool bLogging, bool bCompress )
{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	return KSession::SendPacket( m_dwPerformerType, GetUID(), NULL, usEventID, data, bLogging, bCompress );
#else
	return KSession::SendPacket( PI_LOGIN_PROXY, GetUID(), NULL, usEventID, data, bLogging, bCompress );
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY
}

template < class T >
void KLoginUser::_SendToAccountDB( unsigned short usEventID_, UidType anTrace_[], const T& data_ )
{
    UidType anTrace[2] = { -1, -1 };
    if( anTrace_ != NULL )  // 외부에서 넘어온 값이 있다면 복사한다.
    {
        ::memcpy( anTrace, anTrace_, sizeof( anTrace ) );
    }

    if( anTrace[0] == -1 )
        anTrace[0] = GetUID();
    else
        anTrace[1] = GetUID();

    KncSend( GetPfID(), GetUID(), PI_ACCOUNT_DB, 0, anTrace, usEventID_, data_ );
}

template < class T >
void KLoginUser::SendToAccountDB( unsigned short usEventID, const T& data )
{
    _SendToAccountDB( usEventID, NULL, data );
}

//{{ 2009. 11. 19  최육사	길드구조개선
template < class T >
void KLoginUser::SendToGameDB( unsigned short usEventID_, const T& data_ )
{
	UidType anTrace[2] = { -1, -1 };

	if( anTrace[0] == -1 )
		anTrace[0] = GetUID();
	else
		anTrace[1] = GetUID();

	KncSend( GetPfID(), GetUID(), PI_LOGIN_GAME_DB, 0, anTrace, usEventID_, data_ );
}
//}}

template < class T >
void KLoginUser::SendToLogDB( unsigned short usEventID_, const T& data_ )
{
    UidType anTrace[2] = { -1, -1 };

    if( anTrace[0] == -1 )
        anTrace[0] = GetUID();
    else
        anTrace[1] = GetUID();

    KncSend( GetPfID(), GetUID(), PI_LOG_DB, 0, anTrace, usEventID_, data_ );
}

template < class T >
void KLoginUser::SendToGSUser( UidType nTo, unsigned short usEventID, const T& data )
{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	if(m_dwPerformerType != PI_LOGIN_PROXY)
	{
		START_LOG( cerr, L"잘못된 퍼포머에게 메세지 전달 시도" )
			<< BUILD_LOG( m_dwPerformerType )
			<< END_LOG;

		return;
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

    KEvent kEvent;
    kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
    LIF( kEvent.m_kDestPerformer.AddUID( nTo ) );
    KSession::SendPacket( kEvent );
}
//다른 게임서버로 자료를 보내기 위해
template < class T >
void KLoginUser::SendToGSUser( UidType nToGSUID, UidType nToGSUser, unsigned short usEventID, const T& data )
{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	if(m_dwPerformerType != PI_LOGIN_PROXY)
	{
		START_LOG( cerr, L"잘못된 퍼포머에게 메세지 전달 시도" )
			<< BUILD_LOG( m_dwPerformerType )
			<< END_LOG;

		return;
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	KEvent kEvent;
	kEvent.SetData( PI_GS_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToGSUser ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToGSUser )			
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}

template < class T >
void KLoginUser::SendToGSCharacter( UidType nToUnitUID, unsigned short usEventID, const T& data )
{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	if(m_dwPerformerType != PI_LOGIN_PROXY)
	{
		START_LOG( cerr, L"잘못된 퍼포머에게 메세지 전달 시도" )
			<< BUILD_LOG( m_dwPerformerType )
			<< END_LOG;

		return;
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	KEvent kEvent;
	kEvent.SetData( PI_GS_CHARACTER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToUnitUID ) );
	KSession::SendPacket( kEvent );
}

template < class T >
void KLoginUser::SendToGameServer( unsigned short usEventID, const T& data )
{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	if(m_dwPerformerType != PI_LOGIN_PROXY)
	{
		START_LOG( cerr, L"잘못된 퍼포머에게 메세지 전달 시도" )
			<< BUILD_LOG( m_dwPerformerType )
			<< END_LOG;

		return;
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	UidType anTrace[2] = { GetUID(), -1 };
	KEvent kEvent;
	kEvent.SetData( PI_GS_SERVER, anTrace, usEventID, data );
	KSession::SendPacket( kEvent );
}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
template < class T >
void KLoginUser::SendToChannelUser( UidType nTo, unsigned short usEventID, const T& data )
{
	if(m_dwPerformerType != PI_CHANNEL_LOGIN_PROXY)
	{
		START_LOG( cerr, L"잘못된 퍼포머에게 메세지 전달 시도" )
			<< BUILD_LOG( m_dwPerformerType )
			<< END_LOG;

		return;
	}

	KEvent kEvent;
	kEvent.SetData( PI_CHANNEL_USER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nTo ) );
	KSession::SendPacket( kEvent );
}

template < typename T >
void KLoginUser::SendToChannelServer( unsigned short usEventID, const T& data )
{
	if(m_dwPerformerType != PI_CHANNEL_LOGIN_PROXY)
	{
		START_LOG( cerr, L"잘못된 퍼포머에게 메세지 전달 시도" )
			<< BUILD_LOG( m_dwPerformerType )
			<< END_LOG;

		return;
	}

	UidType anTrace[2] = { GetUID(), -1 };
	KEvent kEvent;
	kEvent.SetData( PI_CHANNEL_SERVER, anTrace, usEventID, data );
	KSession::SendPacket( kEvent );
}

template < class T >
void KLoginUser::SendToKOGBillingDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_LOGIN_USER, GetUID(), PI_LOGIN_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

//#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-08	// 해외팀 주석 처리
template < class T >
void KLoginUser::SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID, const T& data )
{
	KEvent kEvent;
	kEvent.SetData( PI_GS_CHARACTER, NULL, usEventID, data );
	LIF( kEvent.m_kDestPerformer.AddUID( nToUnitUID ) );

	KActorPtr spActor = KActorManager::GetKObj()->Get( nToGSUID );

	if( spActor != NULL )
	{
		spActor->SendPacket( kEvent );
	}
	else
	{
		START_LOG( cerr, L"타 게임서버로 패킷 보내기 실패.? 왜왜" )
			<< BUILD_LOG( nToGSUID )
			<< BUILD_LOG( nToUnitUID )			
			<< BUILD_LOG( KEvent::GetIDStr( usEventID ) )
			<< END_LOG;
	}
}
//#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_COUNTRY_PH
template < class T >
void KLoginUser::SendToKOGBillingDB( unsigned short usEventID, const T& data )
{
	UidType anTrace[2] = { GetUID(), -1 };
	KncSend( PI_LOGIN_USER, GetUID(), PI_LOGIN_KOG_BILLING_DB, 0, anTrace, usEventID, data );
}
#endif //SERV_COUNTRY_PH
