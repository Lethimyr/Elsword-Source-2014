#include "LoginUser.h"
#include <dbg/dbg.hpp>
#include "defaultFSM.h"

#include "DBLayer.h"
#include "NetError.h"
#include "LoginSimLayer.h"
#include "LoginServer.h"
//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	#include "LoginRoomManager.h"
#endif SERV_PSHOP_AGENCY
//}}
#include "Nexon/AuthCheck.h"
#include <boost/format.hpp>
//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	#include "NexonAuthManager.h"
#else
	#include "NexonBillingAuthManager.h"
#endif SERV_PCBANG_AUTH_NEW
//}}
#ifdef SERV_COUNTRY_TH
	#include "../Common/OnlyGlobal/AuthAndBilling/TH/AsiaSoftAuthManager.h"
#endif // SERV_COUNTRY_TH

#ifdef SERV_KOG_OTP_VERIFY
	#include "LoginSimLayer.h"
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_COUNTRY_CN
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantAuthManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantRoleRegManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantInfoManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantBillingManager.h"
#include "../Common/OnlyGlobal/AuthAndBilling/CN/GiantCouponManager.h"
#endif SERV_COUNTRY_CN

#ifdef SERV_COUNTRY_PH
#include "../Common/OnlyGlobal/AuthAndBilling/PH/GarenaBillingServer.h"
#endif //SERV_COUNTRY_PH

//{{ 2008. 7. 14  최육사  랭킹
#include "RankingManager.h"
//}}
//{{ 2009. 9. 23  최육사	길드
#include "GuildManager.h"
//}}
//{{ 2010. 01. 13  최육사	길드게시판
#include "GuildBoardManager.h"
//}}
//{{ 2010. 06. 01  최육사	동접 처리 개편
#ifdef SERV_CCU_NEW
	#include "ServerCCUManager.h"
#endif SERV_CCU_NEW
//}}
//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
	#include "AccountBlockManager.h"
#endif SERV_ACCOUNT_BLOCK
//}}
//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	#include "Mornitoring/MornitoringManager.h"
#endif SERV_MORNITORING
//}}
//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
	#include "DLLManager.h"
#endif SERV_DLL_MANAGER
//}}
//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	#include "RandomItemEventManager.h"
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}
//{{ 2011. 08. 17	최육사	머신ID 중복 접속 차단
#ifdef SERV_MACHINE_ID_DUPLICATE_CHECK
	#include "GameSysVal/GameSysVal.h"
#endif SERV_MACHINE_ID_DUPLICATE_CHECK
//}}
//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
	#include "BuffManager.h"
#endif SERV_SERVER_BUFF_SYSTEM
//}
//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	#include "WeddingHallManager.h"
#endif SERV_RELATIONSHIP_SYSTEM
//}

#pragma comment( lib, "../Common/Nexon/AuthCheck.lib" )

NiImplementRTTI( KLoginUser, KActor );
ImplPfID( KLoginUser, PI_LOGIN_USER );

#define CLASS_TYPE  KLoginUser

KLoginUser::KLoginUser(void) 
{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	m_dwPerformerType = PI_NULL;
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY
}

KLoginUser::~KLoginUser(void)
{
}

ImplToStringW( KLoginUser )
{
    return START_TOSTRING_PARENTW( KActor );
}

void KLoginUser::ProcessEvent( const KEventPtr& spEvent_ )
{
    if( RoutePacket( spEvent_.get() ) )
        return;

    DWORD dwElapTime = ::GetTickCount();  // 040428. 패킷 처리 소요시간을 구한다.

    switch( spEvent_->m_usEventID )
    {
	//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
	//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING	
	//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION
   _CASE( ELG_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ );
#else
   _CASE( ELG_VERIFY_SERVER_CONNECT_REQ, KNetAddress );
#endif SERV_INTEGRATION
	//}}
   _CASE( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK );
	//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
	//}}
    CASE( ELG_NEXON_USER_AUTHENTICATE_REQ );
	//{{ 2009. 5. 26  최육사	채널이동
	CASE( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ );	
	//}}
	CASE( ELG_REGISTER_USER_REQ );
	//{{ 2009. 5. 28  최육사	채널이동
	CASE( ELG_CHANNEL_CHANGE_REGISTER_USER_REQ );
	//}}
	CASE( ELG_USER_DISCONNECT_REQ );

#ifdef SERV_CHATTING_OBSERVATION_CN
	CASE( ELG_CHAT_OBSERVATION_NOT );
#endif //SERV_CHATTING_OBSERVATION_CN

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	_CASE( ELG_VERIFY_ACCOUNT_REQ, KEPUBLISHER_AUTHENTICATION_REQ );
	_CASE( ELG_SECURITY_AUTH_REQ, KEPUBLISHER_SECURITY_AUTH_REQ );
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY

	//{{ 2010. 06. 29  최육사	게임 PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
	CASE( ELG_NEXON_USER_LOGIN_TIME_OUT_NOT );
#endif SERV_PCBANG_AUTH_NEW
	//}}

	CASE( ELG_REGISTER_UNIT_NOT );
	CASE( ELG_UNIT_DISCONNECT_NOT );
	CASE( ELG_SEARCH_UNIT_REQ );
	CASE( ELG_GET_UNIT_INFO_ACK );

   _CASE( ELG_KNM_REQUEST_NEW_FRIEND_INFO_REQ, KEGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ );
    CASE( ELG_KNM_INVITE_REQ );
   _CASE( ELG_KNM_TOGETHER_REQ, KEGS_KNM_TOGETHER_REQ );
    CASE( ELG_KNM_TOGETHER_BY_UNIT_INFO_ACK );

	CASE( ELG_CHAT_REQ );
   _CASE( ELG_ADMIN_KICK_USER_REQ, KEGS_ADMIN_KICK_USER_REQ );
	//{{ 2008. 2. 4  최육사  
	CASE( ELG_SEARCH_BLACK_LIST_UNIT_REQ );
	//}}

	//{{ 2008. 3. 28  최육사  추천인	
   _CASE( ELG_RECOMMEND_USER_RESULT_NOT, KEGS_RECOMMEND_USER_RESULT_NOT );
	//}}

   _CASE( ELG_TUTORIAL_UPDATE_UNIT_INFO_NOT, KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT );
    CASE( ELG_DEL_TUTORIAL_NOT );
	CASE( ELG_REQUEST_TUTORIAL_NOT );
   _CASE( ELG_REQUEST_TUTORIAL_REPLY_NOT, KEGS_REQUEST_TUTORIAL_REPLY_NOT );

   	//{{ 2008. 6. 20  최육사  선물하기
   	CASE( ELG_PRESENT_CASH_ITEM_REQ );
   _CASE( ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT );
   	//}}
	
	//{{ 2008. 7. 14  최육사  랭킹
    CASE( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ );
	CASE( ELG_NEW_RECORD_HENIR_RANKING_NOT );
	CASE( ELG_GET_WEB_RANKING_REFRESH_NOT );
	//}}

	//{{ 2008. 9. 24  최육사	우체국 확장
	CASE( ELG_FIND_USER_SEND_LETTER_NOT );
	//}}

	//{{ 2008. 10. 15  최육사	타이틀
   _CASE( ELG_INSERT_TITLE_NOT, KDBE_INSERT_TITLE_ACK );
	//}}

	//{{ 2009. 4. 1  최육사		친구차단옵션
	CASE( ELG_UPDATE_DENY_FRIEND_SHIP_NOT );
	//}}

	//{{ 2009. 4. 7  최육사		친구정보
   _CASE( ELG_FIND_FRIEND_UNIT_INFO_REQ, KEGS_UPDATE_FRIEND_INFO_REQ );
	//}}

	//{{ 2009. 4. 20  최육사	초대하기
	CASE( ELG_INVITE_PVP_ROOM_REQ );
	//}}

    CASE( ELG_REQUEST_FRIEND_NOT );

    CASE( ELG_ACCEPT_FRIEND_NOT );

    CASE( ELG_DENY_FRIEND_NOT );

    CASE( ELG_BLOCK_FRIEND_NOT );

    CASE( ELG_UNBLOCK_FRIEND_NOT );

    CASE( ELG_DELETE_FRIEND_NOT );

    CASE( ELG_FRIEND_POSITION_UPDATE_NOT );

	//////////////////////////////////////////////////////////////////////////	
	//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST

	//{{ 2009. 11. 20  최육사	길드구조개선	
	CASE( ELG_CREATE_GUILD_RESULT_REQ );
	//}}

	CASE( ELG_GET_GUILD_INFO_REQ );
	//{{ 2009. 11. 19  최육사	길드구조개선
	CASE( DBE_GET_GUILD_INFO_ACK );
	//}}

	CASE( ELG_GET_GUILD_USER_LIST_REQ );

	CASE( ELG_INVITE_GUILD_REQ );
	CASE( ELG_INVITE_GUILD_FIND_USER_ACK );

	CASE( ELG_INVITE_GUILD_REPLY_REQ );
	CASE( ELG_JOIN_GUILD_REQ );

	CASE( ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ );

   _CASE( ELG_UPDATE_CHANGE_GUILD_MEMBER_GRADE_NOT, KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK );
   _CASE( ELG_UPDATE_CHANGE_GUILD_MESSAGE_NOT, KDBE_CHANGE_GUILD_MESSAGE_ACK );
   _CASE( ELG_UPDATE_KICK_GUILD_MEMBER_NOT, KDBE_KICK_GUILD_MEMBER_ACK );
   _CASE( ELG_UPDATE_CHANGE_GUILD_MEMBER_MESSAGE_NOT, KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK );
   _CASE( ELG_UPDATE_DISBAND_GUILD_NOT, KDBE_DISBAND_GUILD_ACK );

	CASE( ELG_UPDATE_GUILD_MEMBER_INFO_NOT );

	CASE( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ );
   _CASE( DLG_UPDATE_EXPAND_GUILD_MAX_MEMBER_NOT, KDBE_EXPAND_GUILD_MAX_MEMBER_ACK );

	//{{ 2009. 10. 27  최육사	길드레벨
   _CASE( ELG_UPDATE_GUILD_EXP_NOT, KDBE_UPDATE_GUILD_EXP_ACK );	
	//}}

   //{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
   CASE( ELG_CREATE_GUILD_REQ );
   CASE( DBE_CREATE_GUILD_ACK );
   CASE( ELG_CHANGE_GUILD_NAME_CHECK_REQ );
   CASE( DBE_CHANGE_GUILD_NAME_CHECK_ACK );
   CASE( ELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ );
   CASE( ELG_CHANGE_GUILD_NAME_REQ );
   CASE( DBE_CHANGE_GUILD_NAME_ACK );
   CASE( ELG_CHANGE_GUILD_NAME_RESULT_REQ );
#endif SERV_GUILD_CHANGE_NAME
   //}}

#endif GUILD_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////	

	//{{ 2009. 10. 27  최육사	길드레벨
	CASE( ELG_FIND_DELETE_CHAR_IN_RANKING_NOT );
	//}}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	CASE( ELG_GET_GUILD_SKILL_REQ );
	CASE( DBE_GET_GUILD_SKILL_ACK );

	CASE( ELG_RESET_GUILD_SKILL_REQ );
   _CASE( ELG_RESET_GUILD_SKILL_DEL_ITEM_ACK, KDBE_RESET_GUILD_SKILL_ACK );

	CASE( ELG_INIT_GUILD_SKILL_REQ );
   _CASE( ELG_INIT_GUILD_SKILL_DEL_ITEM_ACK, KDBE_INIT_GUILD_SKILL_ACK );

	CASE( ELG_INSERT_GUILD_CASH_SKILL_POINT_NOT );

	CASE( ELG_ADMIN_GET_GUILD_SKILL_POINT_REQ );

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////

	//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD

	CASE( ELG_GET_GUILD_AD_LIST_REQ );
	CASE( ELG_GET_GUILD_SKILL_IN_BOARD_REQ );
	CASE( DBE_GET_GUILD_SKILL_IN_BOARD_ACK );

	CASE( ELG_REGISTRATION_GUILD_AD_REQ );
	CASE( DBE_REGISTRATION_GUILD_AD_ACK );

	CASE( ELG_MODIFY_REG_GUILD_AD_REQ );
	CASE( DBE_MODIFY_REG_GUILD_AD_ACK );

	CASE( ELG_APPLY_JOIN_GUILD_REQ );
	CASE( DBE_APPLY_JOIN_GUILD_ACK );

	CASE( ELG_GET_APPLY_JOIN_GUILD_LIST_REQ );

	CASE( ELG_ACCEPT_JOIN_GUILD_REQ );
	CASE( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK );

	CASE( ELG_DELETE_APPLY_JOIN_GUILD_REQ );
	CASE( DBE_DELETE_APPLY_JOIN_GUILD_ACK );

#endif SERV_GUILD_AD
	//}}

	//{{ 2010. 01. 11  최육사	추천인리스트
   _CASE( ELG_GET_RECOMMEND_USER_LIST_REQ, KDBE_GET_RECOMMEND_USER_LIST_ACK );
	CASE( ELG_RECOMMEND_USER_INFO_NOT );
	//}}

	//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW
	CASE( ELG_UPDATE_CCU_INFO_NOT );
#endif SERV_CCU_NEW
	//}}
	//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT
   _CASE( ELG_ADMIN_SHOW_ED_CHEAT_REQ, KEGS_ADMIN_SHOW_ED_CHEAT_REQ );
	CASE( ELG_ADMIN_SHOW_ED_CHEAT_ACK );
#endif SERV_RESET_PERIOD_EVENT
	//}}
	//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	CASE( ELG_REG_REJECTED_USER_NOT );
#endif SERV_CN_SERVER_HACKING_CHECK
	//}}
	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
   _CASE( ELG_REG_TRADE_BLOCK_USER_NOT, KELG_REG_REJECTED_USER_NOT );
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}
	//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
	_CASE( ELG_DISCONNECT_SERVER_REPORT_NOT, KE_DISCONNECT_SERVER_REPORT_NOT );
#endif SERV_MORNITORING
	//}}
	//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
	_CASE( ELG_MODULE_INFO_UPDATE_NOT, KEGS_MODULE_INFO_UPDATE_NOT );
#endif SERV_DLL_MANAGER
	//}}
	//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	CASE( ELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ );
#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
	//}}
	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
   _CASE( ELG_CHECK_MACHINE_ID_REQ, KEGS_CHECK_MACHINE_ID_REQ );
#endif SERV_MACHINE_ID_BLOCK
   //}}
	//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING
	CASE_NOPARAM( ELG_DUMP_LOGIN_USER_NOT );
#endif SERV_TOONILAND_CHANNELING
	//}}

	//{{ 2011. 12. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	CASE( ELG_INVITE_PARTY_FIND_RECEIVER_REQ );
   _CASE( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
   _CASE( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK );
#endif SERV_BATTLE_FIELD_SYSTEM
	//}}

   //{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
   CASE( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ );
#endif SERV_SERVER_BUFF_SYSTEM
   //}

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
   CASE( ELG_ADMIN_DELETE_GUILD_ADD_NOT );
#endif

   //{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
   CASE( ELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT );
   CASE( ELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT );
   CASE( ELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT );
#endif SERV_DELETE_HENIR_RANKING
   //}}

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
	CASE( ELG_PCBANG_LOGOUT_NOT );
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
   _CASE( EGS_LOCAL_RANKING_INQUIRY_REQ, KLocalRankingInquiryData );
   CASE( ELG_LOCAL_RANKING_GUILD_CHANGE_INFO_NOT );
   CASE( ELG_LOCAL_RANKING_WATCH_UNIT_REQ );
   CASE( ELG_LOCAL_RANKING_WATCH_UNIT_ACK );
   CASE( DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_ACK );
#endif	// SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_COUNTRY_CN
   CASE( EPUBLISHER_BILLING_BALANCE_REQ );
   CASE( EBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK );
   CASE( ELG_EXCHANGE_CASH_REQ );
   CASE( EBILL_EXCHANGE_CASH_RESERVE_ACK );
   _CASE( ELG_USE_COUPON_REQ, KEBILL_USE_COUPON_REQ );
   CASE( EBILL_USE_COUPON_RESERVE_ACK );
#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	CASE( ELG_CASH_DIRECT_CHARGE_CN_REQ );
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH
#endif SERV_COUNTRY_CN

   //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
   CASE( ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ );
   _CASE( ELG_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT );
   _CASE( ELG_COUPLE_PROPOSE_AGREE_CHECK_NOT, KEGS_COUPLE_PROPOSE_AGREE_NOT );
   _CASE( ELG_COUPLE_PROPOSE_RESULT_FAIL_NOT, KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT );
   CASE( ELG_RELATIONSHIP_INFO_REQ );
   _CASE( DBE_RELATIONSHIP_INFO_GAME_DB_ACK, KEGS_RELATIONSHIP_INFO_ACK );
   _CASE( ELG_RELATIONSHIP_INFO_GAME_SERVER_ACK, KEGS_RELATIONSHIP_INFO_ACK );
   CASE( ELG_WEDDING_ITEM_FIND_INFO_REQ );
   _CASE( DBE_WEDDING_ITEM_FIND_INFO_ACK, KELG_WEDDING_ITEM_FIND_INFO_REQ );
   CASE( ELG_ADD_WEDDING_HALL_INFO_NOT );
   CASE( ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ );
   CASE( ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT );
   CASE( ELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ );
   CASE( ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT );
   CASE( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT );
   CASE( ELG_CHECK_WEDDING_HALL_INFO_REQ );
   CASE( ELG_BREAK_UP_NOT );
   CASE( ELG_CHANGE_LOVE_WORD_NOT );
#endif SERV_RELATIONSHIP_SYSTEM
   //}

#ifdef SERV_RELATIONSHIP_EVENT_INT
	CASE( ELG_EVENT_PROPOSE_CHECK_CONNECT_USER_REQ );
	_CASE( ELG_EVENT_PROPOSE_AGREE_CHECK_NOT, KEGS_EVENT_PROPOSE_AGREE_NOT );
	_CASE( ELG_EVENT_PROPOSE_RESULT_SUCCESS_NOT, KERM_EVENT_PROPOSE_RESULT_SUCCESS_NOT );
	CASE( ELG_DIVORCE_NOT );
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_COUNTRY_PH
   _CASE( ELG_GN_CHANGE_GAME_CURRENCY_NOT, KEBILL_GN_CHANGE_GAME_CURRENCY_REQ );
#endif //SERV_COUNTRY_PH
   
#ifdef SERV_RECRUIT_EVENT_BASE
   _CASE( ELG_REGISTER_RECRUITER_NOT, KEGS_REGISTER_RECRUITER_NOT );
   _CASE( ELG_GET_RECRUIT_RECRUITER_LIST_REQ, KEGS_GET_RECRUIT_RECRUITER_LIST_ACK );
   CASE( ELG_RECRUIT_RECRUITER_INFO_NOT );
#endif SERV_RECRUIT_EVENT_BASE

    CASE_NOPARAM( E_RESERVE_DESTROY );

    default:
        dbg::cerr << L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr();
    }

    dwElapTime = ::GetTickCount() - dwElapTime;
    if( dwElapTime > 3000 )
    {
        START_LOG_WITH_NAME( cwarn )
            << BUILD_LOG( spEvent_->GetIDStr() )
            << BUILD_LOG( spEvent_->m_usEventID )
            << BUILD_LOG( dwElapTime );
    }
}

void KLoginUser::Tick()
{
    KActor::Tick();

    switch( GetStateID() ) 
	{
    case KDefaultFSM::S_EXIT:
        KActorManager::GetKObj()->ReserveDelete( m_nUID );    // 자기 자신을 스스로 삭제할 수 없음.
        break;
    }
}

//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
//////////////////////////////////////////////////////////////////////////

bool KLoginUser::AuthenticateUser( const wchar_t* szPassport, const wchar_t* szHWID, KNexonAccountInfo& kInfo )
{
	//Nexon ID & Guest ID ..?
	PassportType passportType = AuthCheck_GetPassportType( szPassport );

	AuthCheckError errorCode = AUTHCHECK_ERROR_OK;

	switch( passportType )
	{
		//process nexon id..
	case kPassportType_Nexon:
		{
			//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
			wchar_t		szID[MAX_PATH] = { 0, };
			UINT64		nNexonSN = 0;
			wchar_t		szLoginIP[MAX_PATH] = { 0, };
			UINT32		uSex = 0;
			UINT32		uAge = 0;
			wchar_t		szNationCode[MAX_PATH] = { 0, };
			wchar_t		szMeta[MAX_PATH] = { 0, };
			UINT8		uSecureCode = 0;										// 0 이면 미사용 , 1 이면 OTP 사용, 2 이면 지정PC, 3 이면 넥슨 스틱
			UINT8		uChannelCode = -1;
			wchar_t		szChannelUID[MAX_PATH] = { 0, };
			BOOL		bNewMembership = 0;
			INT8		uMainAuthLevel = 0;			// ATL 메인 레벨
			INT8		uSubAuthLevel = 0;			// ATL 서브 레벨
			
			//////////////////////////////////////////////////////////////////////////
			LoginBegin();
			//////////////////////////////////////////////////////////////////////////

			errorCode = AuthCheck_CheckSession3( FALSE,							// IN	BOOL			bRemoteCheck
												szPassport,						// IN	const TCHAR*	szPassport,					// 1024
												kInfo.m_wstrLoginIP.c_str(),	// IN	const TCHAR*	szUserIp,					// 32
												szHWID,							// IN	const TCHAR*	szHWID,						// 64
												0,								// IN	UINT32			uGameCode,
												szID,							// OUT	TCHAR*			pszId			= NULL,		// 32		-- 의미 변경(숫자@nx) 넥슨 유저만
												&nNexonSN,						// OUT	UINT64*			pnNexonSN64
												szLoginIP,						// OUT	TCHAR*			pszLoginIp		= NULL,		// 32
												&uSex,							// OUT	UINT32*			puSex,
												&uAge,							// OUT	UINT32*			puAge,
												szNationCode,					// OUT	TCHAR*			pszNationCode	= NULL,		// 3
												szMeta,							// OUT	TCHAR*			pszMeta			= NULL,		// 1024
												&uSecureCode,					// OUT	UINT8*			puSecureCode,
												&uChannelCode,					// OUT	UINT8*			puChannelCode,
												szChannelUID,					// OOUT	TCHAR*			pszChannelUID	= NULL,		// 32
												&bNewMembership,				// OUT	BOOL*			pbNewMembership
												&uMainAuthLevel,				// OUT	INT8*			pnMainAuthLevel,
												&uSubAuthLevel					// OUT	INT8*			pnSubAuthLevel
												);

			//////////////////////////////////////////////////////////////////////////
			LoginEnd();
			//////////////////////////////////////////////////////////////////////////
			// 4바이트 값이 넘는다면 확인차 로그를 찍자.
			if( nNexonSN > UINT_MAX )
			{
				START_LOG( cout2, L"값의 범위가 넘는 값이 들어왔다." )
					<< BUILD_LOG( szID )
					<< BUILD_LOG( nNexonSN )
					<< END_LOG;
			}

			std::wstring wstrID = szID;
			if( static_cast<int>( wstrID.find( L"@tooni", 0 ) ) != -1 )	// '@tooni' 가 포함 되어 있다.
			{
				if( uChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT )
				{
					START_LOG( cout2, L"@tooni 가 포함 되어 있다. 그런데 채널코드가 3이 아니다!" )
						<< BUILD_LOG( szPassport )
						<< BUILD_LOG( (int)errorCode )
						<< BUILD_LOG( szID )
						<< BUILD_LOG( nNexonSN )
						<< BUILD_LOG( uChannelCode )
						<< END_LOG;

					// 보정하자
					uChannelCode = KNexonAccountInfo::CE_TOONILAND_ACCOUNT;
				}
			}
            else if( static_cast<int>( wstrID.find( L"@naver", 0 ) ) != -1 )
            {
                if( uChannelCode != KNexonAccountInfo::CE_NAVER_ACCOUNT )
                {
                    START_LOG( cout2, L"@naver 가 포함 되어 있다. 그런데 채널코드가 6 아니다!" )
                        << BUILD_LOG( szPassport )
                        << BUILD_LOG( (int)errorCode )
                        << BUILD_LOG( szID )
                        << BUILD_LOG( nNexonSN )
                        << BUILD_LOG( uChannelCode )
                        << END_LOG;

                    // 보정하자
                    uChannelCode = KNexonAccountInfo::CE_NAVER_ACCOUNT;
                }
            }
			else if( static_cast<int>( wstrID.find( L"@nx", 0 ) ) != -1 )  //'@nx' 가 포함 되어 있다.
			{
				if( uChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
				{
					START_LOG( cout2, L"@nx 가 없다. 그런데 채널코드가 0이 아니다!" )
						<< BUILD_LOG( szPassport )
						<< BUILD_LOG( (int)errorCode )
						<< BUILD_LOG( szID )
						<< BUILD_LOG( nNexonSN )
						<< BUILD_LOG( uChannelCode )
						<< END_LOG;

					// 보정하자
					uChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT;
				}
			}
			else
			{
				if( uChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT )
				{
					START_LOG( cout2, L"'@' 가 없다. 그런데 채널코드가 0이 아니다!" )
						<< BUILD_LOG( szPassport )
						<< BUILD_LOG( (int)errorCode )
						<< BUILD_LOG( szID )
						<< BUILD_LOG( nNexonSN )
						<< BUILD_LOG( uChannelCode )
						<< END_LOG;

					// 보정하자
					uChannelCode = KNexonAccountInfo::CE_NEXON_ACCOUNT;
				}
			}
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
			//}}

			START_LOG( clog, L"OTP 추가버전 passport인증 결과!!" )
				<< BUILD_LOG( szPassport )
				<< BUILD_LOG( (int)errorCode )
				<< BUILD_LOG( szID )
				<< BUILD_LOG( nNexonSN )
				<< BUILD_LOG( szLoginIP )
				<< BUILD_LOG( uSex )
				<< BUILD_LOG( uAge )
				//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
				<< BUILD_LOG( bNewMembership )
				<< BUILD_LOG( uMainAuthLevel )
				<< BUILD_LOG( uSubAuthLevel )
#else
				<< BUILD_LOG( uPwdHash )
				<< BUILD_LOG( uSsnHash )
				<< BUILD_LOG( uFlag0 )
				<< BUILD_LOG( uFlag1 )
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
				//}}
				<< BUILD_LOG( szNationCode )
				<< BUILD_LOG( szMeta )
				<< BUILD_LOG( uSecureCode )
				<< BUILD_LOG( uChannelCode )
				<< BUILD_LOG( szChannelUID )
				;

			if( errorCode == AUTHCHECK_ERROR_OK )
			{
				if( kInfo.m_wstrLoginIP.compare( szLoginIP ) != 0 )
				{
					START_LOG( cerr, L"[NesonID]KSession의 유저 IP와 넥슨 패스워드에서 추출한 IP가 다름." )
						<< BUILD_LOG( kInfo.m_wstrLoginIP )
						<< BUILD_LOG( szLoginIP )
						<< END_LOG;

					// 인증 실패 처리 할까 말까..
				}

				kInfo.m_wstrID = szID;
				//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
				kInfo.m_uiNexonSN = static_cast<UINT32>(nNexonSN);
#else
				kInfo.m_uiNexonSN = nNexonSN;
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
				//}}
				kInfo.m_wstrLoginIP = szLoginIP;
				if( uSex == 1 || uSex == 3 )
				{
					// 남성
					kInfo.m_bSex = true;
				}
				else if( uSex == 2 || uSex == 4 )
				{
					// 여성
					kInfo.m_bSex = false;
				}
				else
				{
					START_LOG( cwarn, L"넥슨 인증 성별 값이 이상함." )
						<< BUILD_LOG( szID )
						<< BUILD_LOG( szPassport )
						<< BUILD_LOG( uSex )
						<< END_LOG;
				}
				kInfo.m_uiAge = uAge;
				//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
				kInfo.m_bNewMembership = (bNewMembership > 0) ? true : false;
				kInfo.m_uMainAuthLevel = static_cast<UINT8>(uMainAuthLevel);
				kInfo.m_uSubAuthLevel = static_cast<UINT8>(uSubAuthLevel);
#else
				kInfo.m_uiPwdHash = uPwdHash;
				kInfo.m_uiSsnHash = uSsnHash;
				kInfo.m_uiFlag0 = uFlag0;
				kInfo.m_uiFlag1 = uFlag1;
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
				//}}
				kInfo.m_wstrNationCode = szNationCode;
				kInfo.m_wstrMeta = szMeta;
				kInfo.m_uSecureCode = uSecureCode;
				kInfo.m_uChannelCode = uChannelCode;

				if( kInfo.m_uChannelCode != KNexonAccountInfo::CE_NEXON_ACCOUNT && 
                    kInfo.m_uChannelCode != KNexonAccountInfo::CE_TOONILAND_ACCOUNT && 
                    kInfo.m_uChannelCode != KNexonAccountInfo::CE_NAVER_ACCOUNT )
				{
					START_LOG( cerr, L"사용하지 않는 채널링 값입니다." )
						<< BUILD_LOG( errorCode )
						<< BUILD_LOG( kInfo.m_uChannelCode )
						<< END_LOG;

					return false;
				}

				kInfo.m_wstrChannelUID = szChannelUID;
			}
		}
		break;
		//process guest id..
	case kPassportType_Guest:
		{
			INT32		nGameCode = 0;
			INT64		nGuestSN = 0;
			wchar_t		szGuestId[MAX_PATH] = { 0, };
			wchar_t		szLoginIP[MAX_PATH] = { 0, };
			SYSTEMTIME	timeCreated;
			SYSTEMTIME	timeToExpire;

			errorCode = AuthCheck_CheckGuestPassport( szPassport, &nGameCode, &nGuestSN, szGuestId, szLoginIP, &timeCreated, &timeToExpire );

			if( errorCode == AUTHCHECK_ERROR_OK )
			{
				if( kInfo.m_wstrLoginIP.compare( szLoginIP ) != 0 )
				{
					START_LOG( cerr, L"[GuestID]KSession의 유저 IP와 넥슨 패스워드에서 추출한 IP가 다름." )
						<< BUILD_LOG( kInfo.m_wstrLoginIP )
						<< BUILD_LOG( szLoginIP )
						<< END_LOG;

					// 인증 실패 처리 할까 말까..
				}

				kInfo.m_wstrID = szGuestId;
				//080514.hoons. 게스트아이디를 따로 식별하기 위해 "_"를 붙인다.
				//넥슨과의 협의로 게스트 아이디는 넥슨아이디보다 한글자 적게 받는다.
				kInfo.m_wstrID += L"/";
				kInfo.m_uiNexonSN = static_cast<UINT32>(nGuestSN);
				kInfo.m_wstrLoginIP = szLoginIP;
				kInfo.m_byteGuestUser = 1;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"Error: Passport type is undefined!!!" )
				<< END_LOG;

			return false;
		}
		break;
	}

	if( errorCode != AUTHCHECK_ERROR_OK )
	{
		switch( errorCode )
		{
		case AUTHCHECK_ERROR_TIMEOUT:
			{
				START_LOG( cerr, L"passport 인증 가능 기간 만료!" )
					<< BUILD_LOG( errorCode )
					<< BUILD_LOG( kInfo.m_wstrLoginIP )
					<< BUILD_LOG( kInfo.m_byteGuestUser )
					<< END_LOG;
			}
			break;

		default:
			{
				START_LOG( cerr, L"아이디 인증 실패!" )
					<< BUILD_LOG( errorCode )
					<< BUILD_LOG( kInfo.m_wstrLoginIP )
					<< BUILD_LOG( kInfo.m_byteGuestUser )
					<< END_LOG;
			}
			break;
		}
	}

	return ( errorCode == AUTHCHECK_ERROR_OK );//(bRet == TRUE);  // BOOL -> bool
}
//////////////////////////////////////////////////////////////////////////
#else
//////////////////////////////////////////////////////////////////////////

bool KLoginUser::AuthenticateUser( const wchar_t* szPassport, KNexonAccountInfo& kInfo )
{
	//Nexon ID & Guest ID ..?
	PassportType passportType = AuthCheck_GetPassportType( szPassport );

	AuthCheckError errorCode = AUTHCHECK_ERROR_OK;

	switch( passportType )
	{
		//process nexon id..
	case kPassportType_Nexon:
		{
			wchar_t szID[MAX_PATH];
			UINT32 nNexonSN;
			wchar_t szLoginIP[MAX_PATH];
			UINT32 uSex;
			UINT32 uAge;
			UINT32 uPwdHash;
			UINT32 uSsnHash;
			UINT32 uFlag0;
			UINT32 uFlag1;
			UINT32 uServerHash;
			wchar_t szServerName[MAX_PATH];

			errorCode = AuthCheck_LocalCheckPassport( szPassport, szID, &nNexonSN, szLoginIP, &uSex, &uAge, &uPwdHash, &uSsnHash, &uFlag0, &uFlag1, &uServerHash, szServerName );

			if( errorCode == AUTHCHECK_ERROR_OK )
			{
				if( kInfo.m_wstrLoginIP.compare( szLoginIP ) != 0 )
				{
					START_LOG( cerr, L"[NesonID]KSession의 유저 IP와 넥슨 패스워드에서 추출한 IP가 다름." )
						<< BUILD_LOG( kInfo.m_wstrLoginIP )
						<< BUILD_LOG( szLoginIP )
						<< END_LOG;

					// 인증 실패 처리 할까 말까..
				}

				kInfo.m_wstrID = szID;
				kInfo.m_uiNexonSN = nNexonSN;
				kInfo.m_wstrLoginIP = szLoginIP;
				if( uSex == 1 )
				{
					// 남성
					kInfo.m_bSex = true;
				}
				else if( uSex == 2 )
				{
					// 여성
					kInfo.m_bSex = false;
				}
				else
				{
					START_LOG( cwarn, L"넥슨 인증 성별 값이 이상함." )
						<< BUILD_LOG( szID )
						<< BUILD_LOG( szPassport )
						<< BUILD_LOG( uSex )
						<< END_LOG;
				}
				kInfo.m_uiAge = uAge;
				kInfo.m_uiPwdHash = uPwdHash;
				kInfo.m_uiSsnHash = uSsnHash;
				kInfo.m_uiFlag0 = uFlag0;
				kInfo.m_uiFlag1 = uFlag1;
				kInfo.m_uiServerHash = uServerHash;
				kInfo.m_wstrServerName = szServerName;
			}
		}
		break;
		//process guest id..
	case kPassportType_Guest:
		{
			INT32		nGameCode = 0;
			INT64		nGuestSN = 0;
			wchar_t		szGuestId[MAX_PATH] = { 0, };
			wchar_t		szLoginIP[MAX_PATH] = { 0, };
			SYSTEMTIME	timeCreated;
			SYSTEMTIME	timeToExpire;

			errorCode = AuthCheck_CheckGuestPassport( szPassport, &nGameCode, &nGuestSN, szGuestId, szLoginIP, &timeCreated, &timeToExpire );

			if( errorCode == AUTHCHECK_ERROR_OK )
			{
				if( kInfo.m_wstrLoginIP.compare( szLoginIP ) != 0 )
				{
					START_LOG( cerr, L"[GuestID]KSession의 유저 IP와 넥슨 패스워드에서 추출한 IP가 다름." )
						<< BUILD_LOG( kInfo.m_wstrLoginIP )
						<< BUILD_LOG( szLoginIP )
						<< END_LOG;

					// 인증 실패 처리 할까 말까..
				}

				kInfo.m_wstrID = szGuestId;
				//080514.hoons. 게스트아이디를 따로 식별하기 위해 "_"를 붙인다.
				//넥슨과의 협의로 게스트 아이디는 넥슨아이디보다 한글자 적게 받는다.
				kInfo.m_wstrID += L"/";
				kInfo.m_uiNexonSN = static_cast<UINT32>(nGuestSN);
				kInfo.m_wstrLoginIP = szLoginIP;
				kInfo.m_byteGuestUser = 1;
			}
		}
		break;

	default:
		{
			START_LOG( cerr, L"Error: Passport type is undefined!!!" )
				<< END_LOG;

			return false;
		}
		break;
	}

	if( errorCode != AUTHCHECK_ERROR_OK )
	{
		switch( errorCode )
		{
		case AUTHCHECK_ERROR_TIMEOUT:
			{
				START_LOG( cerr, L"passport 인증 가능 기간 만료!" )
					<< BUILD_LOG( errorCode )
					<< BUILD_LOG( kInfo.m_wstrLoginIP )
					<< BUILD_LOG( kInfo.m_byteGuestUser )
					<< END_LOG;
			}
			break;

		default:
			{
				START_LOG( cerr, L"아이디 인증 실패!" )
					<< BUILD_LOG( errorCode )
					<< BUILD_LOG( kInfo.m_wstrLoginIP )
					<< BUILD_LOG( kInfo.m_byteGuestUser )
					<< END_LOG;
			}
			break;
		}
	}

	return ( errorCode == AUTHCHECK_ERROR_OK );//(bRet == TRUE);  // BOOL -> bool
}

//////////////////////////////////////////////////////////////////////////
#endif SERV_OTP_AUTH
//}}

//{{ 2008. 6. 11  최육사  선물하기
bool KLoginUser::AuthCheckPassword( const wchar_t* szPassword, UINT32 uiPwdHash )
{
	UINT32 uPwdHash = AuthCheck_GetHashValue( szPassword );

	return ( uiPwdHash == uPwdHash );
}
//}}

//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING
void KLoginUser::LoginBegin()
{
	const DWORD dwCurTickCount = ::GetTickCount();
	m_kLoginTickDump.m_dwLoginTick = dwCurTickCount;
}

void KLoginUser::LoginEnd()
{
	// 카운트 하나 올리자!
	++m_kLoginTickDump.m_dwLoginAuthCount;

	const DWORD dwCurTickCount = ::GetTickCount();
	const DWORD dwDelayTick = dwCurTickCount - m_kLoginTickDump.m_dwLoginTick;

	if( dwDelayTick > m_kLoginTickDump.m_dwMaxTick )
	{
		m_kLoginTickDump.m_dwMaxTick = dwDelayTick;
		m_kLoginTickDump.m_tRegTime = CTime::GetCurrentTime();

		if( dwDelayTick > 30000 )
		{
			START_LOG( cout, L"[경고!] Login Tick 지연시간이 30초를 넘었다!" )
				<< BUILD_LOG( dwDelayTick );
		}
	}
}

const std::wstring KLoginUser::GetLoginMaxTickRegDate() const
{
	std::wstring wstrRegDate = ( std::wstring )( m_kLoginTickDump.m_tRegTime.Format( _T( "%Y-%m-%d_%H%M%S" ) ) );
	return wstrRegDate;
}
#endif SERV_TOONILAND_CHANNELING
//}}

void KLoginUser::OnDestroy()
{
    KSession::OnDestroy();

    switch( GetStateID() ) {
    case KDefaultFSM::S_INIT:
    case KDefaultFSM::S_LOGINED:
        StateTransition( KDefaultFSM::I_EXIT );    // OnDestroy()가 불린 현재 KSession::Tick 처리 중이며,
                                                   // KLoginUser::Tick 처리 단계에서 종료 대기명단에 등록된다.
        break;
    }

	//{{ 2011.2.11  조효진  모니터링툴 서버 포트 체크 시 에러로그 남는거 수정
#ifdef SERV_PERMIT_PORT_CHECK
	// 모니터링 툴 서버의 IP에서는 게임 서버를 동작 할 수 없다고 가정함 //
	// 모니터링 서버에 의한 포트 체크일 경우 아래의 다른 동작들 할 필요없음
	if( KBaseServer::GetKObj()->IsMonitoringServerForPortCheck( GetIPStr() ) )
	{
		//KSession::OnDestroy();					// 이미 위에서 수행함
		//StateTransition( KDefaultFSM::I_EXIT );	// 이미 위에서 수행함
		START_LOG( clog, L"Mornitoring Server check Center Server's Port" );
		return;
	}
#endif SERV_PERMIT_PORT_CHECK
	//}}

	// 유저 리스트에서 해당 게임서버의 유저 정보 삭제
    GetKLoginSimLayer()->UnRegAllUserOfThisGS( GetUID() );

	//{{ 2011. 05. 11	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
	// 게임서버가 죽은 경우, 해당 게임서버에 접속중이던 유저들을 방에서 모두 빼낸다.
	{
		KEventPtr spEvent( new KEvent );
		spEvent->SetData( PI_GS_SERVER, NULL, ERM_CHECK_INVALID_USER_NOT, m_nUID );
		GetKLoginRoomManager()->QueueingToAll( spEvent );
	}
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2009. 1. 30  최육사	객체 소멸 로그
	if( GetDisconnectReason() == KStatistics::eSIColDR_Server_Shutdown )
	{
		START_LOG( cout, L"서버 셧다운으로 인한 LoginUser객체 소멸" );
	}
	else
	{
		START_LOG( cout2, L"비정상적인 원인으로 인한 LoginUser객체 소멸" )
			<< BUILD_LOG( GetIPStr() )
			<< BUILD_LOG( GetDisconnectReason() )			
			<< dbg::tab << L"종료 사유 : " << KStatistics::GetDissconnectReasonString( GetDisconnectReason() ) << dbg::endl
			<< BUILD_LOG( GetLastIocpFailedError() );

		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
		CTime kRegDate = CTime::GetCurrentTime();
		KE_LOCAL_LOG_SERVER_DISCONNECT_NOT kNot;
		kNot.m_cLogType				= KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT;
		kNot.m_wstrSourceName		= boost::str( boost::wformat( L"%d" ) % KBaseServer::GetKObj()->GetUID() );
		kNot.m_wstrDestinationName	= boost::str( boost::wformat( L"%d" ) % GetUID() );
		kNot.m_wstrDestinationIP	= KncUtil::toWideString( GetIPStr() );
		kNot.m_wstrReason			= KStatistics::GetDissconnectReasonString( GetDisconnectReason() );
		kNot.m_wstrRegDate			= (const wchar_t*)kRegDate.Format(_T("%Y-%m-%d %H:%M:%S"));
		KSIManager.QueueingEvent( E_LOCAL_LOG_SERVER_DISCONNECT_NOT, kNot );
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
		//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
		SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_LOGIN_PROXY );
#endif SERV_MORNITORING
		//}}
	}
	//}}
}

bool KLoginUser::RoutePacket( const KEvent* pkEvent_ )
{
    if( pkEvent_->m_kDestPerformer.m_dwPerformerID != PI_LOGIN_USER )
    {
		//{{ 2011.2.11  조효진  모니터링툴 서버 포트 체크 시 에러로그 남는거 수정
#ifdef SERV_PERMIT_PORT_CHECK
		// LoginUser 모니터링 서버인 경우 정상적인 객체가 아니므로 RoutPacket 과정을 거치면 안됨
		if( KBaseServer::GetKObj()->IsMonitoringServerForPortCheck( GetIPStr() ) )
		{
			return true;
		}
#endif SERV_PERMIT_PORT_CHECK
		//}}

        int iEval = CompareServerLevel( pkEvent_->m_kDestPerformer.m_dwPerformerID, PI_LOGIN_USER );
        if( iEval < 0 )
        {
            if( pkEvent_->IsEmptyTrace() )
            {
                START_LOG_WITH_NAME( cerr ) 
                    << BUILD_LOG( pkEvent_->GetIDStr() );

                return true;
            }

            KEvent kEvent( *pkEvent_ );

            kEvent.PopTrace();

            LIF( KSession::SendPacket( kEvent ) );
            return true;
        }
        else if( iEval > 0 )
        {
            START_LOG( cerr, L"현재는 센터 서버가 최상위 서버이다." )
                << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                << BUILD_LOG( pkEvent_->m_usEventID )
                << BUILD_LOG( pkEvent_->GetIDStr() )
                << END_LOG;

            return true;
        }
        else
        {
            KEventPtr spEvent( pkEvent_->Clone() );
            spEvent->PushTrace( GetUID() );

            switch( GetPerformerClass( pkEvent_->m_kDestPerformer.m_dwPerformerID ) )
            {
            case PC_SERVER:
                KBaseServer::GetKObj()->QueueingEvent( spEvent );
                return true;
            case PC_ACCOUNT_DB:
            case PC_LOG_DB:
                SiKDBLayer()->QueueingEvent( spEvent );
                return true;
            case PC_ROOM:
				//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
				GetKLoginRoomManager()->QueueingEventToRoom( spEvent );
#endif SERV_PSHOP_AGENCY
				//}}
                return true;
            case PC_NX_AUTH:
                if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
                {
					//{{ 2010. 06. 08  최육사	넥슨PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW
					SiKNexonAuthManager()->QueueingEvent( spEvent );
#else
					SiKNexonBillingAuthManager()->QueueingEvent( spEvent );
#endif SERV_PCBANG_AUTH_NEW
					//}}                    
                }
                else
                {
                    START_LOG( cerr, L"넥슨 국내인증이 아님." )
                        << BUILD_LOG( KSimLayer::GetKObj()->GetAuthFlag() )
                        //<< BUILD_LOG( KSimLayer::GetKObj()->GetAuthFlagStr( KSimLayer::GetKObj()->GetAuthFlag() ) )
                        << END_LOG;
                }
                return true;
#ifdef SERV_COUNTRY_CN
			case PC_GIANT_AUTH:
				SiKGiantAuthManager()->QueueingEvent( spEvent );
				return true;
			case PC_GIANT_ROLEREG:
				SiKGiantRoleRegManager()->QueueingEvent( spEvent );
				return true;
			case PC_GIANT_INFO:
				SiKGiantInfoManager()->QueueingEvent( spEvent );
				return true;
			case PC_GIANT_BILLING:
				SiKGiantBillingManager()->QueueingEvent( spEvent );
				return true;
			case PC_GIANT_COUPON:
				SiKGiantCouponManager()->QueueingEvent( spEvent );
				return true;
#endif SERV_COUNTRY_CN
            default:
                START_LOG( cerr, L"도착지가 이상한 패킷." )
                    << BUILD_LOG( pkEvent_->m_kDestPerformer.m_dwPerformerID )
                    << END_LOG;
                return true;
            }
        }
    }

    return false;
}

void KLoginUser::SendToAccountDB( unsigned short usEventID )
{
    SendToAccountDB( usEventID, char() );
}

//{{ 2009. 11. 19  최육사	길드구조개선
void KLoginUser::SendToGameDB( unsigned short usEventID )
{
    SendToGameDB( usEventID, char() );
}
//}}

void KLoginUser::SendToGSUser( UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nTo, usEventID, char() );
}

//#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-08	// 해외팀 주석 처리
void KLoginUser::SendToGSCharacter( UidType nToGSUID, UidType nToUnitUID, unsigned short usEventID )
{
	SendToGSCharacter( nToGSUID, nToUnitUID, usEventID, char() );
}
//#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2011. 07. 19	최육사	서버간 접속 코드 리팩토링
//#ifdef SERV_SERVER_CONNECT_CODE_REFACTORING
//{{ 2010. 05. 13  최육사	대전 던전 서버군 통합
#ifdef SERV_INTEGRATION

_IMPL_ON_FUNC( ELG_VERIFY_SERVER_CONNECT_REQ, KECN_VERIFY_SERVER_CONNECT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_kNetAddress.m_usPort );

	if( kPacket_.m_kNetAddress.m_wstrIP != KncUtil::toWideString( GetIPStr() ) )
	{
		START_LOG( cerr, L"유효하지 않은 IP" )
			<< BUILD_LOG( kPacket_.m_kNetAddress.m_wstrIP )
			<< BUILD_LOG( GetIPStr() );

		KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
		kPacket.m_nOK = NetError::ERR_VERIFY_02;
		SendPacket( ELG_VERIFY_SERVER_CONNECT_ACK, kPacket );
		return;
	}

	SendToLogDB( DBE_VERIFY_SERVER_CONNECT_REQ, kPacket_ );
}

#else

_IMPL_ON_FUNC( ELG_VERIFY_SERVER_CONNECT_REQ, KNetAddress )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_INIT ) );

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< BUILD_LOG( kPacket_.m_usPort );

	if( kPacket_.m_wstrIP != KncUtil::toWideString( GetIPStr() ) )
	{
		START_LOG( cerr, L"유효하지 않은 IP" )
			<< BUILD_LOG( kPacket_.m_wstrIP )
			<< BUILD_LOG( GetIPStr() );

		KECN_VERIFY_SERVER_CONNECT_ACK kPacket;
		kPacket.m_nOK = NetError::ERR_VERIFY_02;
		SendPacket( ELG_VERIFY_SERVER_CONNECT_ACK, kPacket );
		return;
	}

	SendToLogDB( DBE_VERIFY_SERVER_CONNECT_REQ, kPacket_ );
}

#endif SERV_INTEGRATION
//}}

_IMPL_ON_FUNC( DBE_VERIFY_SERVER_CONNECT_ACK, KECN_VERIFY_SERVER_CONNECT_ACK )
{
    VERIFY_STATE(( 1, KDefaultFSM::S_INIT ));

	//{{ 2010. 04. 22  최육사	서버군 전체 통합
#ifdef SERV_INTEGRATION
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nUID )
		<< BUILD_LOG( kPacket_.m_nOK )
		<< BUILD_LOG( kPacket_.m_strName )
		<< BUILD_LOG( kPacket_.m_iDBRegServerGroupID )
		<< BUILD_LOG( kPacket_.m_sMaxNum );
#else
	LOG_SUCCESS( kPacket_.m_nOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket_.m_nUID )
		<< BUILD_LOG( kPacket_.m_nOK )
		<< BUILD_LOG( kPacket_.m_strName )
		<< BUILD_LOG( kPacket_.m_sGroupID )
		<< BUILD_LOG( kPacket_.m_sMaxNum );
#endif SERV_INTEGRATION
	//}}

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
	switch(kPacket_.m_iServerType)
	{
	case 3:
		m_dwPerformerType = PI_LOGIN_PROXY;
		break;
	case 4:
		m_dwPerformerType = PI_CHANNEL_LOGIN_PROXY;
		break;
	default:
		m_dwPerformerType = PI_NULL;
		break;
	}
#endif SERV_FROM_CHANNEL_TO_LOGIN_PROXY

    SendPacket( ELG_VERIFY_SERVER_CONNECT_ACK, kPacket_ );

    if( kPacket_.m_nOK != NetError::NET_OK )
    {
        ReserveDestroy();
        return;
    }
	else
	{
#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		switch(m_dwPerformerType)
		{
		case PI_LOGIN_PROXY:
			START_LOG( cout2, L"게임서버와 정상적으로 접속하였습니다!" )
				<< BUILD_LOG( kPacket_.m_nUID )
				<< BUILD_LOG( kPacket_.m_iServerType )
				<< BUILD_LOG( kPacket_.m_strName );
			break;
		case PI_CHANNEL_LOGIN_PROXY:
			START_LOG( cout2, L"채널서버와 정상적으로 접속하였습니다!" )
				<< BUILD_LOG( kPacket_.m_nUID )
				<< BUILD_LOG( kPacket_.m_iServerType )
				<< BUILD_LOG( kPacket_.m_strName );
			break;
		}
#else // SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		START_LOG( cout2, L"게임서버와 정상적으로 접속하였습니다!" )
			<< BUILD_LOG( kPacket_.m_nUID )
			<< BUILD_LOG( kPacket_.m_strName );
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY
		
	}

    KActorPtr spActor( KActorManager::GetKObj()->Get( kPacket_.m_nUID ) );
    if( spActor != NULL )   // 서버 이중 접속
    {
        SET_ERROR( ERR_VERIFY_03 ); // 서버 UID 충돌
        ReserveDestroy();
        return;
    }

    SetName( kPacket_.m_strName );
    LIF( KActorManager::GetKObj()->UpdateUID( kPacket_.m_nUID, *this ) );
    StateTransition( KDefaultFSM::I_AUTHENTICATED );

	//{{ 2011. 05. 04	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
	KERM_LOAD_PSHOP_BOARD_INFO_NOT kPacketNot;
	kPacketNot.m_iGSUID = GetUID();
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_ROOM, NULL, ERM_LOAD_PSHOP_BOARD_INFO_NOT, kPacketNot );
	GetKLoginRoomManager()->QueueingToAll( CXSLRoom::RT_PSHOP_AGENCY, spEvent );
#endif SERV_PSHOP_AGENCY
	//}}

	//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	{
		KDBE_LOAD_WEDDING_HALL_INFO_ACK kPacket;
		kPacket.m_iOK = NetError::NET_OK;
		SiKWeddingHallManager()->GetAllWeddingInfo( kPacket.m_mapWeddingInfo );

		UidType anTrace[2] = { kPacket_.m_nUID, -1 };
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEDDING_HALL_INFO_UPDATE_NOT, kPacket );
	}
#endif SERV_RELATIONSHIP_SYSTEM
	//}
}
//#endif SERV_SERVER_CONNECT_CODE_REFACTORING
//}}

#ifdef SERV_GLOBAL_AUTH
// 해외용이지만 NEXON 포함된 패킷명은 다른 코드와의 혼란을 피하기 위해 변경하지 않음
IMPL_ON_FUNC( ELG_NEXON_USER_AUTHENTICATE_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	bool bSuccess = true;
	
//#if defined(SERV_COUNTRY_JP)	
	//AuthenticateGloablUser();
//#endif // SERV_COUNTRY_JP

	LOG_SUCCESS( bSuccess )
		<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_bDebugAuth )
		<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrPasswd )
		<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrLoginIP );

#ifdef SERV_KOG_OTP_VERIFY
	kPacket_.m_bServerUseKogOTP = GetKLoginSimLayer()->GetUseKogOTP();
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_EPAY_SYSTEM
	kPacket_.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kPacket_.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
#endif //SERV_EPAY_SYSTEM

	_SendToAccountDB( ELG_NEXON_USER_AUTHENTICATE_REQ, anTrace_, kPacket_ );
}

// 해외용이지만 NEXON 포함된 패킷명은 다른 코드와의 혼란을 피하기 위해 변경하지 않음
//{{ 2009. 5. 26  최육사	채널이동
IMPL_ON_FUNC( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	bool bSuccess = true;

//#if defined(SERV_COUNTRY_JP)	
	//AuthenticateGloablUser();
//#endif // SERV_COUNTRY_JP

	LOG_SUCCESS( bSuccess )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrPasswd )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_wstrLoginIP );

#ifdef SERV_KOG_OTP_VERIFY
	kPacket_.m_bServerUseKogOTP = GetKLoginSimLayer()->GetUseKogOTP();
#endif // SERV_KOG_OTP_VERIFY
#ifdef SERV_EPAY_SYSTEM
	kPacket_.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kPacket_.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
#endif //SERV_EPAY_SYSTEM

	_SendToAccountDB( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ, anTrace_, kPacket_ );
}
//}}


#else // SERV_GLOBAL_AUTH
/* 코드 헷갈려서 주석 처리함

IMPL_ON_FUNC( ELG_NEXON_USER_AUTHENTICATE_REQ )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    bool bSuccess = true;

    DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();
    if( !kPacket_.m_kAuthenticateReq.m_bDebugAuth && ( dwAuthFlag == KSimLayer::AF_NEXON_KOREA ) ) // 디버그를 위한 인증이 아니라면 넥슨 인증.
    {
		//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
		bSuccess = AuthenticateUser( kPacket_.m_kAuthenticateReq.m_wstrPasswd.c_str(), 
									 kPacket_.m_kAuthenticateReq.m_wstrHWID.c_str(), 
									 kPacket_.m_kNexonAccountInfo );
#else
		bSuccess = AuthenticateUser( kPacket_.m_kAuthenticateReq.m_wstrPasswd.c_str(), kPacket_.m_kNexonAccountInfo );
#endif SERV_OTP_AUTH
		//}}
    }

    LOG_SUCCESS( bSuccess )
        << BUILD_LOG( kPacket_.m_kAuthenticateReq.m_bDebugAuth )
        << BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrUserID )
        << BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrPasswd )
        << BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrLoginIP );

    if( !bSuccess )
    {
        KELG_NEXON_USER_AUTHENTICATE_ACK kPacketAck;
        kPacketAck.m_iOK = NetError::ERR_VERIFY_10;
        SendToGSUser( FIRST_SENDER_UID, ELG_NEXON_USER_AUTHENTICATE_ACK, kPacketAck );
        return;
    }
	else
	{
		if( !kPacket_.m_kAuthenticateReq.m_bDebugAuth )
		{
			//080515.hoons.체험아이디 일경우 _를 붙여서 진행을 시키기 위해.
			//if( kPacket_.m_kNexonAccountInfo.m_byteGuestUser == 1 )
			if( kPacket_.m_kAuthenticateReq.m_wstrUserID.compare( kPacket_.m_kNexonAccountInfo.m_wstrID ) != 0 )
			{
				START_LOG( clog, L"입력 아이디와 인증아이디가 다른경우.!" )
					<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrUserID )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					;

				kPacket_.m_kAuthenticateReq.m_wstrUserID = kPacket_.m_kNexonAccountInfo.m_wstrID;
			}
		}

		//{{ 2012. 06. 28	김민성       통합 멤버십 ATL 1.0 -> 2.0 변경
#ifdef SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		if( kPacket_.m_kNexonAccountInfo.m_bNewMembership == true 
		 && kPacket_.m_kNexonAccountInfo.m_uMainAuthLevel < 2 )
		{
			// 인증은 성공 했지만 ATL 2.0 이 아니라면
			START_LOG( cerr, L"통합 맴버십 ATL 등급 확인 결과" )
				<< BUILD_LOG( bSuccess )
				<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrUserID )
				<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_uMainAuthLevel )
				<< END_LOG;

			KELG_NEXON_USER_AUTHENTICATE_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_VERIFY_24;
			SendToGSUser( FIRST_SENDER_UID, ELG_NEXON_USER_AUTHENTICATE_ACK, kPacketAck );
			return;
		}
#endif SERV_CHANGE_NEXON_AUTH_ATL_LEVEL
		//}}
	}

    _SendToAccountDB( ELG_NEXON_USER_AUTHENTICATE_REQ, anTrace_, kPacket_ );
}

//{{ 2009. 5. 26  최육사	채널이동
IMPL_ON_FUNC( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	bool bSuccess = true;

	if( !kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth ) // 디버그를 위한 인증이 아니라면 넥슨 인증.
	{
		//{{ 2010. 06. 22  최육사	OTP적용
#ifdef SERV_OTP_AUTH
		bSuccess = AuthenticateUser( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrPasswd.c_str(), 
									 kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrHWID.c_str(), 
									 kPacket_.m_kUserAuthReq.m_kNexonAccountInfo );
#else
		bSuccess = AuthenticateUser( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrPasswd.c_str(), kPacket_.m_kUserAuthReq.m_kNexonAccountInfo );
#endif SERV_OTP_AUTH
		//}}		
	}

	LOG_SUCCESS( bSuccess )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrPasswd )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_wstrLoginIP );

	if( !bSuccess )
	{
		KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK kPacketAck;
		kPacketAck.m_kUserAuthAck.m_iOK = NetError::ERR_VERIFY_10;
		SendToGSUser( FIRST_SENDER_UID, ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK, kPacketAck );
		return;
	}
	else
	{
		if( !kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth )
		{
			//080515.hoons.체험아이디 일경우 _를 붙여서 진행을 시키기 위해.
			//if( kPacket_.m_kNexonAccountInfo.m_byteGuestUser == 1 )
			if( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID.compare( kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_wstrID ) != 0 )
			{
				START_LOG( clog, L"입력 아이디와 인증아이디가 다른경우.!" )
					<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID )
					<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_wstrID )
					;

				kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID = kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_wstrID;
			}
		}
	}

	_SendToAccountDB( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ, anTrace_, kPacket_ );
}
//}}
*/ // 코드 헷갈려서 주석 처리함
#endif // SERV_GLOBAL_AUTH

IMPL_ON_FUNC( ELG_REGISTER_USER_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
	if( SiKAccountBlockManager()->IsBlockedAccount( FIRST_SENDER_UID, KAccountBlockManager::BT_CONNECT_BLOCK ) == true )
	{
		// 해킹 의심 유저 차단 기능으로 인한 접속 제한
		KELG_REGISTER_USER_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_ACCOUNT_BLOCK_00;
		SendToGSUser( FIRST_SENDER_UID, ELG_REGISTER_USER_ACK, kPacket );
		return;
	}
#endif SERV_ACCOUNT_BLOCK
	//}}

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( SiKAccountBlockManager()->IsBlockedAccount( FIRST_SENDER_UID, KAccountBlockManager::BT_TRADE_BLOCK ) == true )
	{
		KELG_TRADE_BLOCK_NOT kNot;
		kNot.m_bIsTradeBlock = true;
		SendToGSUser( FIRST_SENDER_UID, ELG_TRADE_BLOCK_NOT, kNot );
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

	//{{ 2011. 08. 17	최육사	머신ID 블럭
#ifdef SERV_MACHINE_ID_BLOCK
	if( SiKGameSysVal()->IsMachineIDBlock() == true )
	{
		if( SiKAccountBlockManager()->IsBlockedMachineID( kPacket_.m_strMachineID ) == true )
		{
			KELG_REGISTER_USER_ACK kPacket;
			kPacket.m_iOK = NetError::ERR_CHECK_MACHINE_ID_00;
			SendToGSUser( FIRST_SENDER_UID, ELG_REGISTER_USER_ACK, kPacket );
			return;
		}
	}
#endif SERV_MACHINE_ID_BLOCK
	//}}


#ifdef SERV_COUNTRY_TH
	START_LOG( clog, L"RegUser 유저 등록 전 정보확인." )
		<< BUILD_LOG( FIRST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_wstrUserID )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( kPacket_.m_wstrSocketID )
		<< END_LOG;
#endif //SERV_COUNTRY_TH

	// 이중 접속 처리.
	KELG_REGISTER_USER_ACK kPacket;
    kPacket.m_iOK = GetKLoginSimLayer()->RegUser( FIRST_SENDER_UID, GetUID(), kPacket_, false );
    if( kPacket.m_iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"유저 등록 중 에러." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_wstrUserID )
            << BUILD_LOG( GetUID() )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
            << END_LOG;
    }

#ifdef SERV_GLOBAL_AUTH
    if( KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_NONE && 
		KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_INTERNAL )
#else
    if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
#endif SERV_GLOBAL_AUTH
    {
        // OK 인 경우 PC 방 인증 처리 루틴으로 넘어갔다. 실패했으면 끊는다.
        if( kPacket.m_iOK != NetError::NET_OK )
        {
            SendToGSUser( FIRST_SENDER_UID, E_KICK_USER_NOT, kPacket );
            return;
        }
		//인증에 성공하면account db에 접속정보를 등록시킨다.
		else
		{
			KDBE_UPDATE_IS_LOGIN_NOT kNot;
			kNot.m_iUserUID = FIRST_SENDER_UID;
			kNot.m_bIsLogin = true;
			SendToAccountDB( DBE_UPDATE_IS_LOGIN_NOT, kNot );
		}

#ifdef SERV_GLOBAL_AUTH
		if (false == GetKLoginSimLayer()->IsPublisherCheckGameServerLogin())
		{
			// 따로 퍼블리셔에서 Ack 받는게 없으면 여기서 지워줌
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
			GetKLoginSimLayer()->UnRegAuthWaitUser( FIRST_SENDER_UID );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX

			// 퍼블 인증이 없는 경우 여기서 ELG_REGISTER_USER_ACK 가야함
			SendToGSUser( FIRST_SENDER_UID, ELG_REGISTER_USER_ACK, kPacket );
		}
#endif //SERV_GLOBAL_AUTH
    }
    else
    {
		//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
		// 사내서버의 경우 PC방 인증서버로 인증패킷을 보내지 않기 때문에 중복체크를 바로 해제 시킨다.
		GetKLoginSimLayer()->UnRegAuthWaitUser( FIRST_SENDER_UID );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
		//}}

        // 기본 인증
        SendToGSUser( FIRST_SENDER_UID, ELG_REGISTER_USER_ACK, kPacket );
        return;
    }
}

//{{ 2009. 5. 28  최육사	채널이동
IMPL_ON_FUNC( ELG_CHANNEL_CHANGE_REGISTER_USER_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	if( SiKAccountBlockManager()->IsBlockedAccount( FIRST_SENDER_UID, KAccountBlockManager::BT_TRADE_BLOCK ) == true )
	{
		KELG_TRADE_BLOCK_NOT kNot;
		kNot.m_bIsTradeBlock = true;
		SendToGSUser( FIRST_SENDER_UID, ELG_TRADE_BLOCK_NOT, kNot );
	}
#endif SERV_HACK_USER_TRADE_BLOCK
	//}}

	// 이중 접속 처리.
	KELG_CHANNEL_CHANGE_REGISTER_USER_ACK kPacket;	

	//{{ 2010. 07. 01  최육사	PC방 인증 서버 개편	
	kPacket.m_iOK = GetKLoginSimLayer()->RegUser( FIRST_SENDER_UID, GetUID(), kPacket_.m_kRegUserReq, true );
	//}}
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"유저 등록 중 에러." )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kRegUserReq.m_wstrUserID )
			<< BUILD_LOG( GetUID() )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( NetError::GetErrStr( kPacket.m_iOK ) )
			<< END_LOG;
	}
	else
	{
		kPacket.m_kConnectAck = kPacket_.m_kConnectAck;
		kPacket.m_kVerifyAccountAck = kPacket_.m_kVerifyAccountAck;
		kPacket.m_kSelectUnitReq = kPacket_.m_kSelectUnitReq;
		kPacket.m_kChangeUserInfo = kPacket_.m_kChangeUserInfo;
	}

#ifdef SERV_GLOBAL_AUTH
    if( KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_NONE && 
		KSimLayer::GetKObj()->GetAuthFlag() != KSimLayer::AF_INTERNAL )
#else
	if( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_NEXON_KOREA )
#endif SERV_GLOBAL_AUTH
	{
		// OK 인 경우 PC 방 인증 처리 루틴으로 넘어갔다. 실패했으면 끊는다.
		if( kPacket.m_iOK != NetError::NET_OK )
		{
			//SendToGSUser( FIRST_SENDER_UID, E_KICK_USER_NOT, kPacket );
			return;
		}
		//인증에 성공하면account db에 접속정보를 등록시킨다.
		else
		{
			KDBE_UPDATE_IS_LOGIN_NOT kNot;
			kNot.m_iUserUID = FIRST_SENDER_UID;
			kNot.m_bIsLogin = true;
			SendToAccountDB( DBE_UPDATE_IS_LOGIN_NOT, kNot );
		}
	}

	//{{ 2012. 09. 03	최육사		중복 접속 버그 수정
#ifdef SERV_DUPLICATE_CONNECT_BUG_FIX
	// 채널이동의 경우 중복 체크를 하지 않는다. 따라서 여기서 그냥 바로 해제 시킨다.
	GetKLoginSimLayer()->UnRegAuthWaitUser( FIRST_SENDER_UID );
#endif SERV_DUPLICATE_CONNECT_BUG_FIX
	//}}
	
	// 응답
	SendToGSUser( FIRST_SENDER_UID, ELG_CHANNEL_CHANGE_REGISTER_USER_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( ELG_USER_DISCONNECT_REQ )
{
	VERIFY_STATE(( 1, KDefaultFSM::S_LOGINED ));

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( FIRST_SENDER_UID );

	GetKLoginSimLayer()->UnRegUser( FIRST_SENDER_UID );

	KDBE_UPDATE_IS_LOGIN_NOT kNot;
	kNot.m_iUserUID = FIRST_SENDER_UID;
	kNot.m_bIsLogin = false;
	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	kNot.m_mapAccCountInfo = kPacket_.m_mapAccCountInfo;
#endif SERV_ACCOUNT_COUNT
	//}}
	SendToAccountDB( DBE_UPDATE_IS_LOGIN_NOT, kNot );
	
	switch( kPacket_.m_usEventID )
	{
	case EGS_DISCONNECT_FOR_SERVER_SELECT_REQ:
	case EGS_DISCONNECT_FOR_CHANNEL_CHANGE_REQ:
		break;

	default:
		// 채널이동으로 인한 접속종료가 아니라면 여기서 끝
		return;
	}
	
	SendToGSUser( FIRST_SENDER_UID, ELG_USER_DISCONNECT_ACK, kPacket_ );
}

#ifdef SERV_CHATTING_OBSERVATION_CN
IMPL_ON_FUNC( ELG_CHAT_OBSERVATION_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGIANT_INFO_CHAT_NOT kObservationPacket;
	kObservationPacket.m_usZone = SiKGiantInfoManager()->GetServerInfo().m_usZone;
	kObservationPacket.m_usGame = SiKGiantInfoManager()->GetServerInfo().m_usGame;
	kObservationPacket.m_wstrUserName = kPacket_.m_wstrSenderUnitNickName;
	kObservationPacket.m_uiCountryID = static_cast<unsigned int>(kPacket_.m_iServerUID);
	kObservationPacket.m_uiSceneID = 0;			// 의미없음
	kObservationPacket.m_uiType = kPacket_.m_cChatPacketType;
	kObservationPacket.m_wstrContent = kPacket_.m_wstrMsg;

	KEventPtr spEvent( new KEvent );
	spEvent->SetData(PI_NULL, NULL, EGIANT_INFO_CHAT_NOT, kObservationPacket );
	SiKGiantInfoManager()->QueueingEvent( spEvent );
}
#endif //SERV_CHATTING_OBSERVATION_CN

#ifdef SERV_FROM_CHANNEL_TO_LOGIN_PROXY
_IMPL_ON_FUNC( ELG_VERIFY_ACCOUNT_REQ, KEPUBLISHER_AUTHENTICATION_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	RequestPublisherLogin(kPacket_);
}

_IMPL_ON_FUNC( ELG_SECURITY_AUTH_REQ, KEPUBLISHER_SECURITY_AUTH_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	RequestPublisherSecurityAuth(kPacket_);
}
#endif // SERV_FROM_CHANNEL_TO_LOGIN_PROXY

//////////////////////////////////////////////////////////////////////////

#ifdef SERV_COUNTRY_CN
IMPL_ON_FUNC( EPUBLISHER_BILLING_BALANCE_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ kPacket;
	kPacket.m_kCheckGiantBalanceReq = kPacket_;

	SendToKOGBillingDB(EBILL_GET_TRANS_FOR_CHECK_BALANCE_REQ, kPacket);
}

//IMPL_ON_FUNC( ELG_VERIFY_GIANT_MATRIX_CARD_REQ )
//{
//	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
//
//	KEGIANT_AUTH_LOGIN_MTCARD kPacket;
//	kPacket.m_iUserUID = LAST_SENDER_UID;
//	kPacket.m_uiRequestID = static_cast<unsigned int>(GetUID());
//	kPacket.m_usZone = SiKGiantAuthManager()->GetServerInfo().m_usZone;
//	kPacket.m_usGame = SiKGiantAuthManager()->GetServerInfo().m_usGame;
//	kPacket.m_wstrName = kPacket_.m_wstrID;
//	kPacket.m_wstrMatrix = kPacket_.m_wstrMatrix;
//	kPacket.m_uiKey = kPacket_.m_uiGiantMatrixKey;
//
//	KEventPtr spEvent( new KEvent );
//	UidType anTrace[2] = { GetUID(), -1 };
//	spEvent->SetData(PI_NULL, anTrace, EGIANT_AUTH_LOGIN_MTCARD, kPacket );
//	SiKGiantAuthManager()->QueueingEvent( spEvent );
//}

IMPL_ON_FUNC( EBILL_GET_TRANS_FOR_CHECK_BALANCE_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEPUBLISHER_BILLING_BALANCE_REQ kPacket;
	kPacket = kPacket_.m_kCheckGiantBalanceReq;
	kPacket.m_wstrTID = kPacket_.m_wstrTransaction;
	kPacket.m_iAT = KGiantBillingPacket::GB_RT_BALLANCE;

	START_LOG( clog, L"EGIANT_BILLING_BALANCE_REQ" )
		<< BUILD_LOG( kPacket.m_uiPublisherUID )
		<< BUILD_LOG( kPacket.m_wstrAccount )
		<< BUILD_LOG( kPacket.m_wstrTID )
		<< BUILD_LOG( kPacket.m_iAT )
		<< END_LOG;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_LOGIN_GIANT_BILLING, anTrace, EPUBLISHER_BILLING_BALANCE_REQ, kPacket );
	SiKGiantBillingManager()->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( ELG_EXCHANGE_CASH_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEBILL_EXCHANGE_CASH_RESERVE_REQ kPacket;
	kPacket.m_PurchaserInfo = kPacket_.m_PurchaserInfo;
	kPacket.m_iPoint = kPacket_.m_iPoint;

	SendToKOGBillingDB(EBILL_EXCHANGE_CASH_RESERVE_REQ, kPacket);
}

IMPL_ON_FUNC( EBILL_EXCHANGE_CASH_RESERVE_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	//{{ megagame / 2011.08.04 / 주문 예약 실패시에 절대 전환 요청하면 안됨. 주의!
	if(kPacket_.m_iOK != NetError::NET_OK)
	{
		KEBILL_EXCHANGE_CASH_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		kPacket.m_iUnitUID = kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUnitUID;

		SendToGSUser(GetUID(), kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUserUID, EBILL_EXCHANGE_CASH_ACK, kPacket);
		return;
	}
	//}}

	KEGIANT_BILLING_CONSUME_REQ kPacket;
	kPacket.m_uiGiantUID = kPacket_.m_kPacketReq.m_PurchaserInfo.m_uiPublisherUID;
	kPacket.m_iPoint = kPacket_.m_kPacketReq.m_iPoint;
	kPacket.m_iSource = SiKGiantBillingManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrTID = kPacket_.m_wstrTransaction;
	kPacket.m_wstrIP = kPacket_.m_kPacketReq.m_PurchaserInfo.m_wstrIP;
	kPacket.m_iAT = KGiantBillingPacket::GB_RT_EXCHANGE_POINT;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData(PI_LOGIN_GIANT_BILLING, anTrace, EGIANT_BILLING_CONSUME_REQ, kPacket );
	SiKGiantBillingManager()->QueueingEvent( spEvent );
}

_IMPL_ON_FUNC( ELG_USE_COUPON_REQ, KEBILL_USE_COUPON_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	SendToKOGBillingDB(EBILL_USE_COUPON_RESERVE_REQ, kPacket_);
}

IMPL_ON_FUNC( EBILL_USE_COUPON_RESERVE_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	//{{ megagame / 2011.08.04 / 주문 예약 실패시에 절대 사용 요청하면 안됨. 주의!
	if(kPacket_.m_iOK != NetError::NET_OK)
	{
		KEBILL_USE_COUPON_RESULT_ACK kPacket;
		kPacket.m_iOK = kPacket_.m_iOK;
		SendToGSUser(GetUID(), kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUserUID, EBILL_USE_COUPON_RESULT_ACK, kPacket);
		return;
	}
	//}}

#ifdef SERV_COUNTRY_CN
	KEGIANT_BILLING_PCARD_CMD kPacket;
	kPacket.m_uiGiantUID = kPacket_.m_kPacketReq.m_PurchaserInfo.m_uiPublisherUID;
	kPacket.m_iSource = SiKGiantCouponManager()->GetServerInfo().m_usGame;
	kPacket.m_wstrTID = kPacket_.m_kPacketReq.m_wstrTransaction;
	kPacket.m_wstrPCardID = kPacket_.m_kPacketReq.m_wstrSerialCode;
	kPacket.m_wstrIP = kPacket_.m_kPacketReq.m_PurchaserInfo.m_wstrIP;
	kPacket.m_iAT = KGiantBillingPacket::GB_RT_USE_ITEM_CARD;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_LOGIN_GIANT_COUPON, anTrace, EGIANT_BILLING_PCARD_CMD, kPacket );
	SiKGiantCouponManager()->QueueingEvent( spEvent );
#else //SERV_COUNTRY_CN
	START_LOG( cerr, L"캐릭터 등록 실패" )
		<< BUILD_LOG( iOK )
		<< BUILD_LOG( NetError::GetErrStr( iOK ) )
		<< END_LOG;

	KEBILL_USE_COUPON_RESULT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_UNKNOWN;
	SendToGSUser(GetUID(), kPacket_.m_kPacketReq.m_PurchaserInfo.m_iUserUID, EBILL_USE_COUPON_RESULT_ACK, kPacket);
	return;

#endif //SERV_COUNTRY_CN
}

#ifdef SERV_INFOSERVER_ADD_WORK
IMPL_ON_FUNC( EGIANT_INFO_USERONLINE_REQ )
{
	KEGIANT_INFO_USERONLINE_ACK kPacket;
	kPacket.m_wstrUserName = kPacket_.m_wstrUserName;
	kPacket.m_wstrGMName = kPacket_.m_wstrGMName;

	KUserList::KGSUserInfo kUserInfo;

	if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrUserName , kUserInfo ) == false )
	{
		kPacket_.m_bUserOnline = false;
	}
	else
	{
		kPacket_.m_bUserOnline = true;
	}

	kPacket.m_bUserOnline = kPacket_.m_bUserOnline;

	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData(PI_LOGIN_GIANT_INFO, anTrace, EGIANT_INFO_USERONLINE_ACK, kPacket );
	SiKGiantInfoManager()->QueueingEvent( spEvent );
}
#endif //SERV_INFOSERVER_ADD_WORK

#endif // SERV_COUNTRY_CN
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 06. 29  최육사	게임 PC방 인증 서버 개편
#ifdef SERV_PCBANG_AUTH_NEW

IMPL_ON_FUNC( ELG_NEXON_USER_LOGIN_TIME_OUT_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	//////////////////////////////////////////////////////////////////////////		
	START_LOG( cout, L"넥슨 PC방 인증 서버 개편 : 로그인 타임 아웃 처리!" )
		<< BUILD_LOG( kPacket_.m_wstrUserID );
	//////////////////////////////////////////////////////////////////////////
    
	GetKLoginSimLayer()->LoginTimeOut( kPacket_ );
}

#endif SERV_PCBANG_AUTH_NEW
//}}

IMPL_ON_FUNC( ELG_REGISTER_UNIT_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    START_LOG_WITH_NAME( clog )
        << BUILD_LOG( FIRST_SENDER_UID );

    int iOK = GetKLoginSimLayer()->RegUnit( FIRST_SENDER_UID, kPacket_.m_nUnitUID, kPacket_.m_wstrUnitNickName, kPacket_.m_uiKNMSerialNum, kPacket_.m_bDenyFriendShip );
    if( iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"캐릭터 등록 실패" )
            << BUILD_LOG( iOK )
            << BUILD_LOG( NetError::GetErrStr( iOK ) )
            << END_LOG;

        return;
    }


    std::set< UidType >::iterator sit;
    {
        KEGS_FRIEND_POSITION_UPDATE_NOT kNotToSender;
        for( sit = kPacket_.m_setFriendUID.begin(); sit != kPacket_.m_setFriendUID.end(); sit++ )
        {
            bool bExist = false;
            KUserList::KGSUserInfo kGSUserInfo;
            if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( *sit, kGSUserInfo ) )
            {
                bExist = true;
                KEGS_FRIEND_POSITION_UPDATE_NOT kNotToFriend;
                kNotToFriend.m_mapFriendPosition.insert( std::make_pair( kPacket_.m_nUnitUID, KFriendInfo::FP_ON_LINE ) );
                SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, EGS_FRIEND_POSITION_UPDATE_NOT, kNotToFriend );
            }
            kNotToSender.m_mapFriendPosition.insert( std::make_pair( *sit, bExist ? KFriendInfo::FP_ON_LINE : KFriendInfo::FP_OFF_LINE ) );
        }
        SendToGSUser( FIRST_SENDER_UID, EGS_FRIEND_POSITION_UPDATE_NOT, kNotToSender );
    }

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-04-07
	GetKLoginSimLayer()->RemoveLocalRankingUserUnitInfo( kPacket_.m_nUnitUID );
#endif	// SERV_LOCAL_RANKING_SYSTEM
}

IMPL_ON_FUNC( ELG_UNIT_DISCONNECT_NOT )
{
	VERIFY_STATE(( 1, KDefaultFSM::S_LOGINED ));

	START_LOG_WITH_NAME( clog )
		<< BUILD_LOG( FIRST_SENDER_UID );

	GetKLoginSimLayer()->UnRegUnit( FIRST_SENDER_UID );

    std::set< UidType >::iterator sit;
    {
        for( sit = kPacket_.m_setFriendUID.begin(); sit != kPacket_.m_setFriendUID.end(); sit++ )
        {
            KUserList::KGSUserInfo kGSUserInfo;
            if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( *sit, kGSUserInfo ) )
            {
                KEGS_FRIEND_POSITION_UPDATE_NOT kNotToFriend;
                kNotToFriend.m_mapFriendPosition.insert( std::make_pair( kPacket_.m_iUnitUID, KFriendInfo::FP_OFF_LINE ) );
                SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, EGS_FRIEND_POSITION_UPDATE_NOT, kNotToFriend );
            }
        }
    }

	//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
	if( kPacket_.m_iGuildUID > 0 )
	{
        if( SiKGuildManager()->LogoutGuildMember( kPacket_.m_iGuildUID, kPacket_.m_iUnitUID ) == false )
		{
			START_LOG( cerr, L"길드 유저 로그아웃 실패!" )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;
		}
	}
#endif GUILD_TEST
	//}}
}

IMPL_ON_FUNC( ELG_SEARCH_UNIT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;

	switch( kPacket_.m_usEventID )
	{
	case EGS_SEARCH_UNIT_REQ:
	case EGS_GET_CONNECTION_UNIT_INFO_REQ:
		{
			if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrNickName, kUserInfo ) == false )
			{
				KELG_SEARCH_UNIT_ACK kPacket;
				kPacket.m_iOK		= NetError::ERR_SEARCH_UNIT_03; // 현재 접속 중이 아닙니다.
				kPacket.m_usEventID = kPacket_.m_usEventID;
				kPacket.m_kCUnitInfo.m_wstrCharName = kPacket_.m_wstrNickName;
				SendToGSUser( FIRST_SENDER_UID, ELG_SEARCH_UNIT_ACK, kPacket );
				return;
			}
		}
		break;

	case EGS_WATCH_UNIT_REQ:
		{
			if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kUserInfo ) == false )
			{
				KELG_SEARCH_UNIT_ACK kPacket;
				kPacket.m_iOK		= NetError::ERR_SEARCH_UNIT_03; // 현재 접속 중이 아닙니다.
				kPacket.m_usEventID = kPacket_.m_usEventID;
				kPacket.m_kCUnitInfo.m_iUnitUID = kPacket_.m_iUnitUID;
				SendToGSUser( FIRST_SENDER_UID, ELG_SEARCH_UNIT_ACK, kPacket );
				return;
			}
		}
		break;
		// 2011.05.05 lygan_조성욱 // 중국용 커뮤니티탭 유저 정보 보기
#ifdef SERV_USER_WATCH_NEW
	case EGS_USER_COMMUNITY_SURVEY_NEW_REQ:
		{
			if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrNickName, kUserInfo ) == false )
			{
				KELG_SEARCH_UNIT_ACK kPacket;
				kPacket.m_iOK		= NetError::ERR_SEARCH_UNIT_03; // 현재 접속 중이 아닙니다.
				kPacket.m_usEventID = kPacket_.m_usEventID;
				kPacket.m_kCUnitInfo.m_wstrCharName = kPacket_.m_wstrNickName;
				SendToGSUser( FIRST_SENDER_UID, ELG_SEARCH_UNIT_ACK, kPacket );
				return;
			}
		}
		break;
#endif //SERV_USER_WATCH_NEW

	default:
		{
			START_LOG( cerr, L"이상한 eventid가 담겨져 왔습니다." )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;
		}
		return;
	}

	//찾는 캐릭이 있으면 해당 GS로 정보요청
	KELG_GET_UNIT_INFO_REQ kReq;
	kReq.m_usEventID		= kPacket_.m_usEventID;
	kReq.m_iDemandGSUID		= GetUID();
	kReq.m_iDemandUserUID	= FIRST_SENDER_UID;
	kReq.m_wstrNickName		= kPacket_.m_wstrNickName;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_GET_UNIT_INFO_REQ, kReq );
}

IMPL_ON_FUNC( ELG_GET_UNIT_INFO_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_SEARCH_UNIT_ACK kPacket;	
	kPacket.m_iOK			= kPacket_.m_iOK;
	kPacket.m_usEventID		= kPacket_.m_usEventID;
	kPacket.m_kCUnitInfo	= kPacket_.m_kCUnitInfo;

	SendToGSUser( kPacket_.m_iDemandGSUID, kPacket_.m_iDemandUserUID, ELG_SEARCH_UNIT_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_KNM_REQUEST_NEW_FRIEND_INFO_REQ, KEGS_KNM_REQUEST_NEW_FRIEND_INFO_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_KNM_REQUEST_NEW_FRIEND_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	KUserList::KGSUserInfo kUserInfo;

	kPacket.m_wstrUnitNickName		= kPacket_.m_wstrUnitNickName;

	//찾는 캐릭이 없으면.
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrUnitNickName, kUserInfo ) == false )
	{
		kPacket.m_iOK	= NetError::ERR_KNM_02;

		goto end_proc;
	}

	if( kUserInfo.m_nUnitUID <= 0 )
	{
		kPacket.m_iOK	= NetError::ERR_KNM_02;

		START_LOG( cerr, L"친구등록할 유닛 UID 가 없네.ㅡ=ㅡ^" )
			<< BUILD_LOG( kUserInfo.m_nUnitUID )
			<< BUILD_LOG( kUserInfo.m_wstrUnitNickName )
			<< END_LOG;

		goto end_proc;
	}

	//{{ 2009. 3. 24  최육사	GM제한
	if( kUserInfo.m_cAuthLevel == SEnum::UAL_GM )
	{
		kPacket.m_iOK	= NetError::ERR_GM_CHAR_DENY_00;
		goto end_proc;
	}
	//}}

	//{{ 2009. 4. 1  최육사		친구초대 차단
	if( kUserInfo.m_bDenyFriendShip == true )
	{
		kPacket.m_iOK	= NetError::ERR_KNM_06;
		goto end_proc;
	}
	//}}

	//{{ 2008. 5. 16  최육사  체험ID 제한
	if( kUserInfo.m_bIsGuestUser )
	{
		kPacket.m_iOK	= NetError::ERR_GUEST_USER_03;
		goto end_proc;
	}
	//}}

	//찾는 유닛의 정보를 담아 보내준다.
	//..MessengerSN
	//..NickName
	kPacket.m_uiKNMSerialNum	= kUserInfo.m_uiKNMSerialNum;
	kPacket.m_wstrUnitNickName	= kUserInfo.m_wstrUnitNickName;

end_proc:
	SendToGSUser( FIRST_SENDER_UID, ELG_KNM_REQUEST_NEW_FRIEND_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_KNM_INVITE_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	SET_ERROR( NET_OK );

	KUserList::KGSUserInfo	kUserInfo;

	if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrInviteUnitNickName, kUserInfo ) == false )
	{
		SET_ERROR( ERR_KNM_04 );

		goto end_proc;
	}
	else
	{
		if( kUserInfo.m_nUnitUID > 0 )
		{
			KEGS_KNM_INVITE_NOT	kNot;
			kNot.m_wstrNickName		= kPacket_.m_wstrMyNickName;
			kNot.m_kUnitInfo		= kPacket_.m_kMyUnitInfo;

			SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_KNM_INVITE_NOT, kNot );

			goto end_proc;
		}
		else
		{
			SET_ERROR( ERR_KNM_04 );

			goto end_proc;
		}
	}

end_proc:
	KEGS_KNM_INVITE_ACK kPacket;
	kPacket.m_iOK = NetError::GetLastError();
	SendToGSUser( FIRST_SENDER_UID, ELG_KNM_INVITE_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_KNM_TOGETHER_REQ, KEGS_KNM_TOGETHER_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	SET_ERROR( NET_OK );

	KUserList::KGSUserInfo	kUserInfo;

	if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_, kUserInfo ) == false )
	{
		SET_ERROR( ERR_KNM_05 );

		goto end_proc;
	}
	else
	{
		if( kUserInfo.m_nUnitUID > 0 )
		{
			KELG_KNM_TOGETHER_BY_UNIT_INFO_REQ kPacket;
			kPacket.m_iDemandGSUID		= GetUID();
			kPacket.m_iDemandUserUID	= FIRST_SENDER_UID;
			SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_KNM_TOGETHER_BY_UNIT_INFO_REQ, kPacket );

			return;
		}
		else
		{
			SET_ERROR( ERR_KNM_05 );

			goto end_proc;
		}
	}

end_proc:
	KEGS_KNM_TOGETHER_ACK kPacket;
	kPacket.m_iOK = NetError::GetLastError();
	SendToGSUser( FIRST_SENDER_UID, ELG_KNM_TOGETHER_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_KNM_TOGETHER_BY_UNIT_INFO_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_KNM_TOGETHER_ACK kPacket;
	kPacket.m_iOK		= kPacket_.m_iOK;
	kPacket.m_kUnitInfo	= kPacket_.m_kUnitInfo;

	SendToGSUser( kPacket_.m_iDemandGSUID, kPacket_.m_iDemandUserUID, ELG_KNM_TOGETHER_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_CHAT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	SET_ERROR( NET_OK );

	KUserList::KGSUserInfo	kUserInfo;

    switch( kPacket_.m_cChatPacketType )
    {
    case KEGS_CHAT_REQ::CPT_WHISPER:
        {
            if( !GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrRecieverUnitNickName, kUserInfo ) )
            {
                SET_ERROR( ERR_CHAT_04 );

                goto end_proc;
            }
        }
        break;
    case KEGS_CHAT_REQ::CPT_MESSENGER:
        {
            if( !GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrRecieverUnitNickName, kUserInfo ) )
            {
                SET_ERROR( ERR_CHAT_04 );

                goto end_proc;
            }
        }
        break;
		//{{ 2009. 9. 29  최육사	길드
#ifdef GUILD_TEST
	case KEGS_CHAT_REQ::CPT_GUILD:
		{
            if( !SiKGuildManager()->GuildChat( kPacket_ ) )
			{
				SET_ERROR( ERR_GUILD_06 ); // 길드정보를 찾을수가 없다.
			}

			goto end_proc;
		}
		break;
#endif GUILD_TEST
		//}}
		//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	case KEGS_CHAT_REQ::CPT_RELATIONSHIP:
		{
			if( !GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrRecieverUnitNickName, kUserInfo ) )
			{
				//  접속 중인 유저가 없어도 에러 팝업을 띄우지 않도록 한다
				KELG_CHAT_ACK kAck;
				KELG_CHAT_NOT kNot;

				kAck.m_iOK = NetError::GetLastError();
				SendToGSUser( FIRST_SENDER_UID, ELG_CHAT_ACK, kAck );

				kNot.m_cChatPacketType			= kPacket_.m_cChatPacketType;
				//{{ 2008. 2. 1  최육사  
				kNot.m_iSenderUnitUID			= kPacket_.m_iSenderUnitUID;
				//}}
				kNot.m_wstrSenderUnitNickName	= kPacket_.m_wstrSenderUnitNickName;
				kNot.m_wstrRecieverUnitNickName	= kPacket_.m_wstrRecieverUnitNickName;
				kNot.m_iReceiverUnitUID         = kUserInfo.m_nUnitUID;
				kNot.m_wstrMsg					= kPacket_.m_wstrMsg;

				SendToGSUser( FIRST_SENDER_UID, ELG_CHAT_NOT, kNot );
				
				return;
			}
		}
		break;
#endif SERV_RELATIONSHIP_SYSTEM
		//}
    default:
        break;
    }

	if( kUserInfo.m_nUnitUID > 0 )
	{
		KELG_CHAT_ACK kAck;
		KELG_CHAT_NOT kNot;

		kAck.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_CHAT_ACK, kAck );

		kNot.m_cChatPacketType			= kPacket_.m_cChatPacketType;
		//{{ 2008. 2. 1  최육사  
		kNot.m_iSenderUnitUID			= kPacket_.m_iSenderUnitUID;
		//}}
		kNot.m_wstrSenderUnitNickName	= kPacket_.m_wstrSenderUnitNickName;
		kNot.m_wstrRecieverUnitNickName	= kPacket_.m_wstrRecieverUnitNickName;
        kNot.m_iReceiverUnitUID         = kUserInfo.m_nUnitUID;
		kNot.m_wstrMsg					= kPacket_.m_wstrMsg;

		SendToGSUser( FIRST_SENDER_UID, ELG_CHAT_NOT, kNot );
		SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_CHAT_NOT, kNot );

		return;
	}
	else
	{
		SET_ERROR( ERR_CHAT_04 );

		goto end_proc;
	}

end_proc:
	KELG_CHAT_ACK kAck;
	kAck.m_iOK = NetError::GetLastError();
	SendToGSUser( FIRST_SENDER_UID, ELG_CHAT_ACK, kAck );
}

_IMPL_ON_FUNC( ELG_ADMIN_KICK_USER_REQ, KEGS_ADMIN_KICK_USER_REQ )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    KEGS_ADMIN_KICK_USER_ACK kPacket;
    KUserList::KGSUserInfo kInfo;

	if( kPacket_.m_bIsUserID )
	{
		if( !GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrUnitNickName, kInfo ) )
		{
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;
			SendToGSUser( FIRST_SENDER_UID, ELG_ADMIN_KICK_USER_ACK, kPacket );
			return;
		}
	}
	else
	{
		if( !GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrUnitNickName, kInfo ) )
		{
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;
			SendToGSUser( FIRST_SENDER_UID, ELG_ADMIN_KICK_USER_ACK, kPacket );
			return;
		}
	}    

    if( kInfo.m_nGSUID <= 0 || kInfo.m_nUserUID <= 0 )
    {
        START_LOG( cerr, L"유저 정보 이상." )
            << BUILD_LOG( FIRST_SENDER_UID )
            << BUILD_LOG( kPacket_.m_wstrUnitNickName )
            << BUILD_LOG( kInfo.m_nGSUID )
            << BUILD_LOG( kInfo.m_nUserUID )
            << BUILD_LOG( kInfo.m_wstrUserID )
            << BUILD_LOG( kInfo.m_nUnitUID )
            << BUILD_LOG( kInfo.m_wstrUnitNickName )
            << END_LOG;

        kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;
        SendToGSUser( FIRST_SENDER_UID, ELG_ADMIN_KICK_USER_ACK, kPacket );
        return;
    }

    kPacket.m_iOK = NetError::NET_OK;
    SendToGSUser( FIRST_SENDER_UID, ELG_ADMIN_KICK_USER_ACK, kPacket );
    int iOK = NetError::ERR_ADMIN_COMMAND_07;
    SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, E_KICK_USER_NOT, iOK );
}

//{{ 2008. 2. 4  최육사  
IMPL_ON_FUNC( ELG_SEARCH_BLACK_LIST_UNIT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;

	KELG_SEARCH_BLACK_LIST_UNIT_ACK kPacket;

	if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrNickName, kUserInfo ) == false )
	{
		// 현재 접속하지 않은 유저임
		kPacket.m_iOK	= NetError::ERR_BLACKLIST_07;
		SendToGSUser( FIRST_SENDER_UID, ELG_SEARCH_BLACK_LIST_UNIT_ACK, kPacket );
		return;
	}

	//찾는 캐릭이 있으면 해당 캐릭터의 UID 전송
	kPacket.m_iOK	   = NetError::NET_OK;
	kPacket.m_iUnitUID = kUserInfo.m_nUnitUID;

	SendToGSUser( FIRST_SENDER_UID, ELG_SEARCH_BLACK_LIST_UNIT_ACK, kPacket );
}
//}}

//{{ 2008. 3. 28  최육사  추천인
_IMPL_ON_FUNC( ELG_RECOMMEND_USER_RESULT_NOT, KEGS_RECOMMEND_USER_RESULT_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iRecommendedUnitUID, kUserInfo ) == false )
	{
		// 현재 접속하지 않은 유저라면 여기서 끝~!
		return;
	}

	//찾는 캐릭이 있으면 해당 캐릭터의 UID 전송	
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_RECOMMEND_USER_RESULT_NOT, kPacket_ );
}
//}}

_IMPL_ON_FUNC( ELG_TUTORIAL_UPDATE_UNIT_INFO_NOT, KEGS_TUTORIAL_UPDATE_UNIT_INFO_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUID, kUserInfo ) == false )
	{
		START_LOG( clog, L"찾는 유저가 없음.!" )
			<< BUILD_LOG( kPacket_.m_iReceiverUID )
			<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_iUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( (int)kPacket_.m_bIsReply )
			;

		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_TUTORIAL_UPDATE_UNIT_INFO_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_DEL_TUTORIAL_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUnitUID, kUserInfo ) == false )
	{
		START_LOG( clog, L"찾는 유저가 없음.!" )
			<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
			<< BUILD_LOG( kPacket_.m_iDeleteUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
			<< BUILD_LOG( kPacket_.m_iReason )
#else SERV_NETERROR_STR_GET_FROM_CLIENT
			<< BUILD_LOG( kPacket_.m_wstrReason )
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
			//}}
			;

		return;
	}

	KEGS_DEL_TUTORIAL_NOT kPacket;
	kPacket.m_iUnitUID = kPacket_.m_iDeleteUnitUID;
	//{{ 2011.09.16 조효진  넷 에러 스트링을 클라이언트쪽에서 가져와서 출력
#ifdef SERV_NETERROR_STR_GET_FROM_CLIENT
	kPacket.m_iReason = kPacket_.m_iReason;
#else SERV_NETERROR_STR_GET_FROM_CLIENT
	kPacket.m_wstrReason = kPacket_.m_wstrReason;
#endif SERV_NETERROR_STR_GET_FROM_CLIENT
	//}}	

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_DEL_TUTORIAL_NOT, kPacket );
}

IMPL_ON_FUNC( ELG_REQUEST_TUTORIAL_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUnitUID, kUserInfo ) == false )
	{
		START_LOG( clog, L"찾는 유저가 없음.!" )
			<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
			<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_iUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			;

		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_REQUEST_TUTORIAL_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_REQUEST_TUTORIAL_REPLY_NOT, KEGS_REQUEST_TUTORIAL_REPLY_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUnitUID, kUserInfo ) == false )
	{
		START_LOG( clog, L"찾는 유저가 없음.!" )
			<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
			<< BUILD_LOG( kPacket_.m_kTutorialUnitInfo.m_iUnitUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			;

		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_REQUEST_TUTORIAL_REPLY_NOT, kPacket_ );
}

//{{2008. 4. 15  최육사  파티
//_IMPL_ON_FUNC( ELG_INVITE_PARTY_NOT, KERM_INVITE_PARTY_NOT )
//{
//	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
//
//	KUserList::KGSUserInfo kUserInfo;
//	if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrReceiverNickName, kUserInfo ) == false )
//	{
//		START_LOG( clog, L"찾는 유저가 없음.!" )
//			<< BUILD_LOG( kPacket_.m_wstrReceiverNickName )
//			<< BUILD_LOG( kPacket_.m_iSenderUID )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			;
//
//		return;
//	}
//
//	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INVITE_PARTY_NOT, kPacket_ );
//}
//
//_IMPL_ON_FUNC( ELG_INVITE_PARTY_REPLY_NOT, KERM_INVITE_PARTY_REPLY_NOT )
//{
//	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
//
//	KUserList::KGSUserInfo kUserInfo;
//	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUID, kUserInfo ) == false )
//	{
//		START_LOG( clog, L"찾는 유저가 없음.!" )
//			<< BUILD_LOG( kPacket_.m_iReceiverUID )
//			<< BUILD_LOG( kPacket_.m_iMemberUID )
//			<< BUILD_LOG( FIRST_SENDER_UID )
//			;
//
//		return;
//	}
//
//	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INVITE_PARTY_REPLY_NOT, kPacket_ );
//}
//}}

//{{ 2008. 6. 20  최육사  선물하기
IMPL_ON_FUNC( ELG_PRESENT_CASH_ITEM_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_PRESENT_CASH_ITEM_ACK kPacket;	

	// 비밀번호 인증
	if( !AuthCheckPassword( kPacket_.m_wstrPassword.c_str(), kPacket_.m_uiPwdHash ) )
	{
		// 비밀번호 인증 실패
		kPacket.m_iOK			   = NetError::ERR_BUY_CASH_ITEM_21;
		goto end_proc;
	}
	
	kPacket.m_iOK				   = NetError::NET_OK;
	kPacket.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
	kPacket.m_wstrMessage		   = kPacket_.m_wstrMessage;
	kPacket.m_vecPurchaseReqInfo   = kPacket_.m_vecPurchaseReqInfo;
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-07-29	// 박세훈
	kPacket.m_bUseCoupon			= kPacket_.m_bUseCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM
	
end_proc:
	SendToGSUser( FIRST_SENDER_UID, ELG_PRESENT_CASH_ITEM_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUserID( kPacket_.m_wstrReceiverUserID, kUserInfo ) == false )
	{
		// 접속하지 않은 유저라면 여기서 끝..
		return;
	}

	//찾는 캐릭이 있으면 해당 캐릭터에게 메시지 전송
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_PRESENT_MESSAGE_TO_RECEIVER_NOT, kPacket_ );
}
//}}

//{{ 2008. 7. 14  최육사  랭킹
IMPL_ON_FUNC( ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_REQ )
{
	// 모든 랭킹정보를 게임서버에게 주자!
	KELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK kPacket;
	SiKRankingManager()->GetHenirRankingInfo( kPacket.m_mapHenirRanking, false );
	SiKRankingManager()->GetHenirLastRank( kPacket.m_mapLastRank );
	
	UidType anTrace[2] = { kPacket_.m_iServerUID, -1 };
	KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_GET_RANKING_FOR_GS_RANKING_MANAGER_ACK, kPacket );

	// 대전&던전 랭킹을 게임서버로 보내자
	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true; // 초기화
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_VECTOR;
		SiKRankingManager()->GetDungeonRankingVector( 0, kPacket );
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true; // 초기화
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_MAP;
		SiKRankingManager()->GetDungeonRankingMap( 0, kPacket );
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true; // 초기화
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_VECTOR;
		SiKRankingManager()->GetPvpRankingVector( 0, kPacket );
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}

	{
		KELG_WEB_RANKING_REFRESH_NOT kPacket;
		kPacket.m_bInit = true; // 초기화
		kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_MAP;
		SiKRankingManager()->GetPvpRankingMap( 0, kPacket );
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
	}
}

IMPL_ON_FUNC( ELG_NEW_RECORD_HENIR_RANKING_NOT )
{
	// 유저가 헤니르 시공을 클리어하여 랭킹 등록을 요청하였음!
	if( SiKRankingManager()->NewRecordHenirRanking( kPacket_.m_kNewRecordRanking ) == false )
	{
		START_LOG( clog, L"랭킹 등록 시도하였으나 순위안에 들지 못하였음" )
			<< BUILD_LOG( kPacket_.m_kNewRecordRanking.m_wstrNickName )
			<< BUILD_LOG( kPacket_.m_kNewRecordRanking.m_iStageCount )
			<< BUILD_LOG( kPacket_.m_kNewRecordRanking.m_ulPlayTime );
		return;
	}	

	// 100위안에 랭크 되었다면 유저에게 알려주자! - 어떤 랭킹타입에 몇위 했는지도 알려줘야할까?
	SendToGSUser( FIRST_SENDER_UID, ELG_NEW_RECORD_RANKING_SUCCESS_NOT );
}
//}}

//{{ 2009. 11. 9  최육사	로그인서버GameDB
IMPL_ON_FUNC( ELG_GET_WEB_RANKING_REFRESH_NOT )
{
	UidType anTrace[2] = { kPacket_.m_iServerUID, -1 };

	// 대전&던전 랭킹을 게임서버로 보내자
	switch( kPacket_.m_cRankingRefreshType )
	{
	case KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_VECTOR:
		{
			KELG_WEB_RANKING_REFRESH_NOT kPacket;
			kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_VECTOR;
			SiKRankingManager()->GetDungeonRankingVector( kPacket_.m_uiLastIndex, kPacket );
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
		}
		break;

	case KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_MAP:
		{
			KELG_WEB_RANKING_REFRESH_NOT kPacket;
			kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::DUNGEON_MAP;
			SiKRankingManager()->GetDungeonRankingMap( kPacket_.m_uiLastIndex, kPacket );
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
		}
		break;

	case KELG_WEB_RANKING_REFRESH_NOT::PVP_VECTOR:
		{
			KELG_WEB_RANKING_REFRESH_NOT kPacket;
			kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_VECTOR;
			SiKRankingManager()->GetPvpRankingVector( kPacket_.m_uiLastIndex, kPacket );
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
		}
		break;

	case KELG_WEB_RANKING_REFRESH_NOT::PVP_MAP:
		{
			KELG_WEB_RANKING_REFRESH_NOT kPacket;
			kPacket.m_cRankingRefreshType = KELG_WEB_RANKING_REFRESH_NOT::PVP_MAP;
			SiKRankingManager()->GetPvpRankingMap( kPacket_.m_uiLastIndex, kPacket );
			KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0, anTrace, ELG_WEB_RANKING_REFRESH_NOT, kPacket );
		}
		break;

	default:
		{
            START_LOG( cerr, L"대전&던전 랭킹 타입이 이상합니다." )
				<< BUILD_LOGc( kPacket_.m_cRankingRefreshType )
				<< END_LOG;
		}
		break;
	}
}
//}}

//{{ 2008. 9. 24  최육사	우체국 확장
IMPL_ON_FUNC( ELG_FIND_USER_SEND_LETTER_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_kLetter.m_iToUnitUID, kUserInfo ) )
		return;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_FIND_USER_SEND_LETTER_NOT, kPacket_ );
}
//}}

//{{ 2008. 10. 15  최육사	타이틀
_IMPL_ON_FUNC( ELG_INSERT_TITLE_NOT, KDBE_INSERT_TITLE_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kUserInfo ) )
		return;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INSERT_TITLE_NOT, kPacket_ );
}
//}}

//{{ 2009. 4. 1  최육사		친구차단옵션
IMPL_ON_FUNC( ELG_UPDATE_DENY_FRIEND_SHIP_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
	
	if( !GetKLoginSimLayer()->SetDenyFriendShip( kPacket_.m_iUnitUID, kPacket_.m_bDenyFriendShip ) )
	{
		START_LOG( cwarn, L"친구 차단 옵션을 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOGc( kPacket_.m_bDenyFriendShip )
			<< END_LOG;
	}
}
//}}

//{{ 2009. 4. 7  최육사		친구정보
_IMPL_ON_FUNC( ELG_FIND_FRIEND_UNIT_INFO_REQ, KEGS_UPDATE_FRIEND_INFO_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    KEGS_UPDATE_FRIEND_INFO_ACK kPacket;

	KUserList::KGSUserInfo kUserInfo;
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrFriendNickName, kUserInfo ) )
	{
		// 친구가 현재 접속해 있지 않다!

		START_LOG( clog, L"친구가 접속중이지 않다!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_wstrFriendNickName );

		kPacket.m_iOK = NetError::ERR_UPDATE_FRIEND_01;
	}
	else
	{
		kPacket.m_iOK				 = NetError::NET_OK;
		kPacket.m_iFriendUnitUID	 = kUserInfo.m_nUnitUID;
		kPacket.m_wstrFriendNickName = kPacket_.m_wstrFriendNickName;
		//{{ 2009. 4. 17  최육사	채널id
		kPacket.m_iChannelID		 = kUserInfo.m_iChannelID;
		//}}
	}
	
	SendToGSUser( LAST_SENDER_UID, ELG_FIND_FRIEND_UNIT_INFO_ACK, kPacket );
}
//}}

//{{ 2009. 4. 20  최육사	초대하기
IMPL_ON_FUNC( ELG_INVITE_PVP_ROOM_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	SET_ERROR( NET_OK );

	KUserList::KGSUserInfo	kUserInfo;

	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iToUnitUID, kUserInfo ) == false )
	{
		SET_ERROR( ERR_KNM_02 );
		goto end_proc;
	}
	else
	{
		//{{ 2009. 3. 24  최육사	GM제한
		if( kUserInfo.m_cAuthLevel == SEnum::UAL_GM )
		{
			SET_ERROR( ERR_GM_CHAR_DENY_00 );
			goto end_proc;
		}
		//}}

		if( kUserInfo.m_nUnitUID > 0 )
		{
			KEGS_INVITE_PVP_ROOM_NOT kNot;
			kNot.m_wstrNickName		= kPacket_.m_wstrFromNickName;
			kNot.m_kRoomInfo		= kPacket_.m_kRoomInfo;

			SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INVITE_PVP_ROOM_NOT, kNot );
			goto end_proc;
		}
		else
		{
			SET_ERROR( ERR_KNM_04 );
			goto end_proc;
		}
	}

end_proc:
	KELG_INVITE_PVP_ROOM_ACK kPacketAck;
	kPacketAck.m_wstrNickName = kUserInfo.m_wstrUnitNickName;
	kPacketAck.m_iOK		  = NetError::GetLastError();
	SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_PVP_ROOM_ACK, kPacketAck );
}
//}}

IMPL_ON_FUNC( ELG_REQUEST_FRIEND_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    KUserList::KGSUserInfo	kUserInfo;

    if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iDestUnitUID, kUserInfo ) )
    {
        // 캐릭터가 접속 중이 아님
        KDBE_FRIEND_MESSAGE_NOT kPacketMsg;
        kPacketMsg.m_iSenderUnitUID = kPacket_.m_iUnitUID;
        kPacketMsg.m_wstrReceiverNickName = kPacket_.m_wstrDestNickName;
        kPacketMsg.m_cMessageType = KFriendMessageInfo::MT_REQUEST_FRIEND;
        kPacketMsg.m_wstrMessage = kPacket_.m_wstrMessage;

        UidType anTrace[2] = { GetUID(), -1 };
        KEvent kEvent;
        kEvent.SetData( PI_GS_GAME_DB, anTrace, DBE_FRIEND_MESSAGE_NOT, kPacketMsg );
        KSession::SendPacket( kEvent );
        return;
    }

    KEGS_REQUEST_FRIEND_NOT kPacketNot;
    kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
    kPacketNot.m_wstrNickName = kPacket_.m_wstrNickName;
    kPacketNot.m_wstrMessage = kPacket_.m_wstrMessage;
    SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGS_REQUEST_FRIEND_NOT, kPacketNot );
}

IMPL_ON_FUNC( ELG_ACCEPT_FRIEND_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    KUserList::KGSUserInfo	kUserInfo;

    if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iDestUnitUID, kUserInfo ) )
    {
        // 캐릭터가 접속 중이 아님
        return;
    }

    KEGS_ACCEPT_FRIEND_NOT kPacketNot;
    kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
    SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGS_ACCEPT_FRIEND_NOT, kPacketNot );
}

IMPL_ON_FUNC( ELG_DENY_FRIEND_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

#ifdef SERV_DENY_FRIEND_FIX
	KUserList::KGSUserInfo	kDestUserInfo, kUserInfo;
	// 나쁜놈
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kUserInfo ) )
	{	
		return;	// 캐릭터가 접속 중이 아님
	}
	// 불쌍한놈
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iDestUnitUID, kDestUserInfo ) )
	{
		return;	// 캐릭터가 접속 중이 아님
	}
#else // SERV_DENY_FRIEND_FIX
	KUserList::KGSUserInfo	kUserInfo;

	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iDestUnitUID, kUserInfo ) )
	{
		// 캐릭터가 접속 중이 아님
		return;
	}
#endif // SERV_DENY_FRIEND_FIX

    KEGS_DENY_FRIEND_NOT kPacketNot;
    kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
    kPacketNot.m_wstrNickName = kUserInfo.m_wstrUnitNickName;
#ifdef SERV_DENY_FRIEND_FIX
	SendToGSUser( kDestUserInfo.m_nGSUID, kDestUserInfo.m_nUserUID, EGS_DENY_FRIEND_NOT, kPacketNot );
#else // SERV_DENY_FRIEND_FIX
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGS_DENY_FRIEND_NOT, kPacketNot );
#endif // SERV_DENY_FRIEND_FIX
}

IMPL_ON_FUNC( ELG_BLOCK_FRIEND_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    KUserList::KGSUserInfo	kUserInfo;

    if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iDestUnitUID, kUserInfo ) )
    {
        // 캐릭터가 접속 중이 아님
        return;
    }

    KEGS_BLOCK_FRIEND_NOT kPacketNot;
    kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
    SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGS_BLOCK_FRIEND_NOT, kPacketNot );
}

IMPL_ON_FUNC( ELG_UNBLOCK_FRIEND_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    KUserList::KGSUserInfo	kUserInfo;

    if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iDestUnitUID, kUserInfo ) )
    {
        // 캐릭터가 접속 중이 아님
        return;
    }

    KEGS_UNBLOCK_FRIEND_NOT kPacketNot;
    kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
    SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGS_UNBLOCK_FRIEND_NOT, kPacketNot );
}

IMPL_ON_FUNC( ELG_DELETE_FRIEND_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    KUserList::KGSUserInfo	kUserInfo;

    if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iDestUnitUID, kUserInfo ) )
    {
        // 캐릭터가 접속 중이 아님
        return;
    }

    KEGS_DELETE_FRIEND_NOT kPacketNot;
    kPacketNot.m_iUnitUID = kPacket_.m_iUnitUID;
    SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, EGS_DELETE_FRIEND_NOT, kPacketNot );
}

IMPL_ON_FUNC( ELG_FRIEND_POSITION_UPDATE_NOT )
{
    VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    std::set< UidType >::iterator sit;
    {
        for( sit = kPacket_.m_setFriendUID.begin(); sit != kPacket_.m_setFriendUID.end(); sit++ )
        {
            bool bExist = false;
            KUserList::KGSUserInfo kGSUserInfo;
            if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( *sit, kGSUserInfo ) )
            {
                KEGS_FRIEND_POSITION_UPDATE_NOT kNotToFriend;
                kNotToFriend.m_mapFriendPosition.insert( std::make_pair( kPacket_.m_iUnitUID, kPacket_.m_cPosition ) );
                SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, EGS_FRIEND_POSITION_UPDATE_NOT, kNotToFriend );
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 9. 22  최육사	길드
#ifdef GUILD_TEST

//{{ 2009. 11. 20  최육사	길드구조개선
IMPL_ON_FUNC( ELG_CREATE_GUILD_RESULT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// DB로부터 받아온 길드 정보 업데이트
	if( SiKGuildManager()->UpdateGuildInfo( kPacket_.m_kGuildInfo ) == false )
	{
		START_LOG( cerr, L"길드 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< END_LOG;

		// 일단 통과 시키자! 멤버 정보를 업데이트하기 위해서
	}

	// 길드 멤버 정보 업데이트
	if( SiKGuildManager()->UpdateGuildMemberList( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket_.m_vecMemberList ) == false )
	{
		START_LOG( cerr, L"길드 멤버 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< END_LOG;

		// 일단 통과 시키자! 로그인 처리를 하기 위해서
	}

	//{{ 2009. 12. 3  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	if( SiKGuildManager()->UpdateGuildSkill( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket_.m_kGuildSkillInfo ) == false )
	{
		START_LOG( cerr, L"길드 스킬 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< END_LOG;

		// 일단 통과 시키자! 로그인 처리를 하기 위해서
	}
#endif GUILD_SKILL_TEST
	//}}

	// 있다면 자신의 길드멤버정보를 업데이트한다!
	if( SiKGuildManager()->LoginGuildMember( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket_.m_kLoginGuildMember ) == false )
	{
		START_LOG( cerr, L"길드 멤버 로그인 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 일단 그대로 통과.
	}

	// 업데이트한 정보를 다시 받자!
	KEGS_GUILD_INFO_NOT kPacket;
	LIF( SiKGuildManager()->GetGuildInfo( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket ) );

	SendToGSUser( FIRST_SENDER_UID, ELG_CREATE_GUILD_RESULT_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( ELG_GET_GUILD_INFO_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
	
	KEGS_GUILD_INFO_NOT kPacket;

    // 길드 정보을 찾자!
	if( SiKGuildManager()->GetGuildInfo( kPacket_.m_iGuildUID, kPacket ) == false )
	{
		// 없다면 DB로 가서 길드정보를 가져오자!
		//kPacket.m_iOK = NetError::ERR_GUILD_06;

		//////////////////////////////////////////////////////////////////////////
		// 로그인서버에 길드정보가 없다면 DB로 가서 받아오자!
		KDBE_GET_GUILD_INFO_REQ kPacketToDB;
		kPacketToDB.m_iUnitUID = kPacket_.m_iUnitUID;
		kPacketToDB.m_iGuildUID = kPacket_.m_iGuildUID;
		kPacketToDB.m_kLoginGuildMember = kPacket_.m_kLoginGuildMember;
		SendToGameDB( DBE_GET_GUILD_INFO_REQ, kPacketToDB );
		return;
		//////////////////////////////////////////////////////////////////////////
	}
	
	// 있다면 자신의 길드멤버정보를 업데이트한다!
	if( SiKGuildManager()->LoginGuildMember( kPacket_.m_iGuildUID, kPacket_.m_kLoginGuildMember ) == false )
	{
		START_LOG( cerr, L"길드 멤버 로그인 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 일단 그대로 통과.
	}
	
	SendToGSUser( FIRST_SENDER_UID, ELG_GET_GUILD_INFO_ACK, kPacket );
}

//{{ 2009. 11. 19  최육사	길드구조개선
IMPL_ON_FUNC( DBE_GET_GUILD_INFO_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
	    START_LOG( cerr, L"DB로부터 길드 정보 얻기 실패! guilduid는 있는데 길드정보가 없다?" )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kLoginGuildMember.m_iUnitUID )
			<< END_LOG;
	
		return;
	}

	// DB로부터 받아온 길드 정보 업데이트
	if( SiKGuildManager()->UpdateGuildInfo( kPacket_.m_kGuildInfo ) == false )
	{
		START_LOG( cerr, L"길드 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< END_LOG;

		// 일단 통과 시키자! 멤버 정보를 업데이트하기 위해서
	}

	//{{ 2010. 01. 21  최육사	길드 게시판
#ifdef SERV_GUILD_AD

	// 해당 길드의 길드 광고가 존재하는지 확인
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_kGuildInfo.m_iGuildUID ) == true )
	{
		// 존재한다면 길드 스킬 정보를 삭제 처리 하자!
        SiKGuildBoardManager()->DeleteGuildSkillInfo( kPacket_.m_kGuildInfo.m_iGuildUID );
	}

#endif SERV_GUILD_AD
	//}}

	// 길드 멤버 정보 업데이트
	if( SiKGuildManager()->UpdateGuildMemberList( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket_.m_vecMemberList ) == false )
	{
		START_LOG( cerr, L"길드 멤버 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< END_LOG;

		// 일단 통과 시키자! 로그인 처리를 하기 위해서
	}

	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	if( SiKGuildManager()->UpdateGuildSkill( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket_.m_kGuildSkillInfo ) == false )
	{
		START_LOG( cerr, L"길드 스킬 정보 업데이트 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< END_LOG;

		// 일단 통과 시키자! 로그인 처리를 하기 위해서
	}

#endif GUILD_SKILL_TEST
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
	KGuildPtr spGuild = SiKGuildManager()->GetGuild( kPacket_.m_kGuildInfo.m_iGuildUID );
	if( spGuild != NULL )
	{
		spGuild->UpdateGuildMemberRanking( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket_.m_mapRankerUnitInfo, kPacket_.m_mapRankerUIDInfo, kPacket_.m_mapRankerUserInfo );
	}
#endif	// SERV_LOCAL_RANKING_SYSTEM

	// 현재 해당 길드원이 로그인해 있는지 확인한다.
	KUserList::KGSUserInfo kGSUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_kLoginGuildMember.m_iUnitUID, kGSUserInfo ) == false )
	{
		START_LOG( clog, L"DB로부터 길드정보를 가져오는 사이에 유저가 로그아웃을 한것임." )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kLoginGuildMember.m_iUnitUID );

		// 더이상 처리할게 없으므로 return
		return;
	}

	// 있다면 자신의 길드멤버정보를 업데이트한다!
	if( SiKGuildManager()->LoginGuildMember( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket_.m_kLoginGuildMember ) == false )
	{
		START_LOG( cerr, L"길드 멤버 로그인 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_kGuildInfo.m_iGuildUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 일단 그대로 통과.
	}

	// 업데이트한 정보를 다시 받자!
	KEGS_GUILD_INFO_NOT kPacket;
	LIF( SiKGuildManager()->GetGuildInfo( kPacket_.m_kGuildInfo.m_iGuildUID, kPacket ) );

	SendToGSCharacter( kPacket_.m_kLoginGuildMember.m_iUnitUID, ELG_GET_GUILD_INFO_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( ELG_GET_GUILD_USER_LIST_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_GET_GUILD_USER_LIST_ACK kPacket;

	// GuidUID에 해당되는 길드멤버 리스트를 얻자!
	if( SiKGuildManager()->GetGuildUserList( kPacket_, kPacket ) == false )
	{
		START_LOG( cerr, L"길드 유저리스트 얻기 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}
	
	SendToGSUser( FIRST_SENDER_UID, ELG_GET_GUILD_USER_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_INVITE_GUILD_ACK kPacketAck;
	KELG_INVITE_GUILD_FIND_USER_REQ kPacket;	

	// 길드에 초대할 유저가 현재 접속해있는지 찾아보자!
	KUserList::KGSUserInfo kGSUserInfo;
	kPacketAck.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;

	if( kPacket_.m_iReceiverUnitUID > 0 )
	{
		// UnitUId로 찾기
		if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUnitUID, kGSUserInfo ) == false )
		{
			kPacketAck.m_iOK = NetError::ERR_GUILD_08;
			SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_GUILD_ACK, kPacketAck );
			return;
		}
	}	
	else
	{
		// 닉네임으로 찾기
		if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrReceiverNickName, kGSUserInfo ) == false )
		{			
			kPacketAck.m_iOK = NetError::ERR_GUILD_08;
			SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_GUILD_ACK, kPacketAck );
			return;
		}
	}

	// 초대 대상자 닉네임 얻기
	kPacketAck.m_wstrReceiverNickName = kGSUserInfo.m_wstrUnitNickName;

	// 초대대상자가 체험ID인지 알아보자!
	if( kGSUserInfo.m_bIsGuestUser )
	{
		kPacketAck.m_iOK = NetError::ERR_GUILD_09;
		SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_GUILD_ACK, kPacketAck );
		return;
	}

	// 길드에게 초대장을 날려도 되는지 허락받자!
	if( SiKGuildManager()->CheckInviteCondition( kPacket_.m_iGuildUID, kGSUserInfo.m_nUnitUID, kPacket_.m_iSenderUnitUID, kPacket ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_GUILD_ACK, kPacketAck );
		return;
	}

	// 초대대장자에게 직접 찾아가자!
	kPacket.m_usEventID = ELG_INVITE_GUILD_REQ;
	kPacket.m_bAutoJoinGuild = false; // 자동 초대 아니다.
	kPacket.m_iSenderUnitUID = kPacket_.m_iSenderUnitUID;
	kPacket.m_iReceiverUnitUID = kGSUserInfo.m_nUnitUID;
	SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, ELG_INVITE_GUILD_FIND_USER_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_FIND_USER_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 응답을 다시 보내자!
	KUserList::KGSUserInfo kGSUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iSenderUnitUID, kGSUserInfo ) == false )
	{
		// 초대장을 발송한 유저가 접속종료했다!
		return;
	}

	switch( kPacket_.m_usEventID )
	{
	case ELG_INVITE_GUILD_REQ:
		{
			KELG_INVITE_GUILD_ACK kPacketAck;
			kPacketAck.m_iOK = kPacket_.m_iOK;
			kPacketAck.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
			SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, ELG_INVITE_GUILD_ACK, kPacketAck );
		}
		break;

	case DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK:
		{
			KEGS_ACCEPT_JOIN_GUILD_ACK kPacketAck;
			kPacketAck.m_iOK = kPacket_.m_iOK;
			kPacketAck.m_wstrReceiverNickName = kPacket_.m_wstrReceiverNickName;
			SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, ELG_ACCEPT_JOIN_GUILD_ACK, kPacketAck );
		}
		break;

	default:
		{
			START_LOG( cerr, L"정의 되지 않은 eventid입니다." )
				<< BUILD_LOG( KEvent::GetIDStr( kPacket_.m_usEventID ) )
				<< END_LOG;
		}
	}
}

IMPL_ON_FUNC( ELG_INVITE_GUILD_REPLY_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_INVITE_GUILD_REPLY_ACK kPacketAck;
	kPacketAck.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacketAck.m_iOK = kPacket_.m_iReason;	

    // 수락
	if( kPacket_.m_iReason == NetError::NET_OK )
	{
		// 길드 가입 수락시 예약하기
		if( SiKGuildManager()->CheckJoinGuild( kPacket_.m_iGuildUID, kPacket_.m_iUnitUID, kPacketAck.m_iGuildMasterUnitUID ) == false )
		{
			// 길드 가입 실패 처리!
			kPacketAck.m_iOK = NetError::GetLastError();
		}
	}
	// 거부
	else if( kPacket_.m_iReason == NetError::ERR_GUILD_17 )
	{
		// 초대장을 발송한 유저가 아직 접속해있다면 거부응답을 보낸다!
		KUserList::KGSUserInfo kGSUserInfo;
		if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrSenderNickName, kGSUserInfo ) == true )
		{
			KEGS_INVITE_GUILD_REPLY_DENY_NOT KNot;
			KNot.m_wstrNickName = kPacket_.m_wstrNickName;
			SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, ELG_INVITE_GUILD_REPLY_DENY_NOT, KNot );
		}
	}

	// 다시 돌아가자!
	SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_GUILD_REPLY_ACK, kPacketAck );
}

IMPL_ON_FUNC( ELG_JOIN_GUILD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_JOIN_GUILD_ACK kPacket;

	// 예약된 정보를 지운다
	if( SiKGuildManager()->RemoveReserve( kPacket_.m_iGuildUID, kPacket_.m_kLoginGuildMember.m_iUnitUID ) == false )
	{
		START_LOG( cerr, L"길드 가입 예약 정보 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_kLoginGuildMember.m_iUnitUID )
			<< END_LOG;
	}

	// 길드 가입 성공이 아니면 종료처리
	if( kPacket_.m_iOK != NetError::NET_OK )
		return;
	
	// 있다면 길드 가입 정보를 업데이트 한다!
	if( SiKGuildManager()->JoinGuildMember( kPacket_.m_iGuildUID, kPacket_.m_kLoginGuildMember, true ) == false )
	{
		START_LOG( cerr, L"길드 멤버 로그인 정보 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< END_LOG;

		// 일단 그대로 통과.
		kPacket.m_iOK = NetError::ERR_GUILD_06;
	}
	else
	{
		// 성공!
		kPacket.m_iOK = NetError::NET_OK;

		// 길드 정보을 찾자!
		LIF( SiKGuildManager()->GetGuildInfo( kPacket_.m_iGuildUID, kPacket.m_kGuildInfoNot ) );

		//{{ 2010. 01. 25  최육사	길드 게시판
#ifdef SERV_GUILD_AD

		// 현재 길드 인원 업데이트
		SiKGuildBoardManager()->UpdateGuildCurMemberCount( kPacket_.m_iGuildUID, static_cast<u_short>(kPacket.m_kGuildInfoNot.m_vecMemberUnitUID.size()) );

		// 길드 가입 신청 해놓은게 있다면 삭제한다.
		int iApplyJoinGuildUID = 0;
		if( SiKGuildBoardManager()->GetMyApplyJoinGuildUID( kPacket_.m_kLoginGuildMember.m_iUnitUID, iApplyJoinGuildUID ) == true )
		{
			// 가입 신청 삭제
			if( SiKGuildBoardManager()->DeleteApplyJoinGuild( kPacket_.m_kLoginGuildMember.m_iUnitUID ) == true )
			{
				KDBE_DELETE_GUILD_AD_LIST_NOT kPacketToDB;
				kPacketToDB.m_vecDeleteApply.push_back( KApplyDeleteInfo( iApplyJoinGuildUID, kPacket_.m_kLoginGuildMember.m_iUnitUID ) );
				SendToGameDB( DBE_DELETE_GUILD_AD_LIST_NOT, kPacketToDB );
			}
		}

#endif SERV_GUILD_AD
		//}}
	}

	SendToGSCharacter( kPacket_.m_iUnitUID, ELG_JOIN_GUILD_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK kPacket;

	// 길드매니저에서 길드관리자 임명 가능한지 체크!
	if( SiKGuildManager()->CheckChangeGradeGuild( kPacket_, kPacket ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}

	SendToGSUser( FIRST_SENDER_UID, ELG_CHECK_CHANGE_GUILD_MEMBER_GRADE_ACK, kPacket );
}

_IMPL_ON_FUNC( ELG_UPDATE_CHANGE_GUILD_MEMBER_GRADE_NOT, KDBE_CHANGE_GUILD_MEMBER_GRADE_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    // 변경된 정보를 업데이트하고 길드원들에게 알린다!
	if( SiKGuildManager()->UpdateChangeGuildMemberGrade( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 멤버 등급 변경 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
			<< BUILD_LOGc( kPacket_.m_ucMemberShipGrade )
			<< END_LOG;
	}

	//{{ 2010. 01. 25  최육사	길드 게시판
#ifdef SERV_GUILD_AD

	// 길드 광고가 등록되어있는지 확인하고 있다면 길드 마스터 정보를 업데이트 한다.
	if( kPacket_.m_bChangeGuildMaster )
	{
		if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
		{
			KEGS_GUILD_INFO_NOT kNot;
			SiKGuildManager()->GetGuildInfo( kPacket_.m_iGuildUID, kNot );

			// 길드 마스터 닉네임 업데이트
			SiKGuildBoardManager()->UpdateGuildMasterNickName( kPacket_.m_iGuildUID, kNot.m_wstrGuildMasterName );
		}
	}

#endif SERV_GUILD_AD
	//}}
}

_IMPL_ON_FUNC( ELG_UPDATE_CHANGE_GUILD_MESSAGE_NOT, KDBE_CHANGE_GUILD_MESSAGE_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 변경된 정보를 업데이트
	if( SiKGuildManager()->UpdateChangeGuildMessage( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 메시지 변경 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )			
			<< BUILD_LOG( kPacket_.m_wstrMessage )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ELG_UPDATE_KICK_GUILD_MEMBER_NOT, KDBE_KICK_GUILD_MEMBER_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 변경된 정보를 업데이트하고 길드원들에게 알린다!
	std::wstring wstrGuildName;
	if( SiKGuildManager()->UpdateKickGuildMember( kPacket_, wstrGuildName ) == false )
	{
		START_LOG( cerr, L"길드원 탈퇴 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
			<< END_LOG;
		return;
	}

	// 길드원 탈퇴 처리가 성공했다면..
	KEGS_GUILD_INFO_NOT kNot;
	SiKGuildManager()->GetGuildInfo( kPacket_.m_iGuildUID, kNot );

	// 현재 등록한 길드 광고가 있는지 확인
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		// 현재 인원 정보 업데이트
		SiKGuildBoardManager()->UpdateGuildCurMemberCount( kPacket_.m_iGuildUID, static_cast<u_short>(kNot.m_vecMemberUnitUID.size()) );
	}
	
	// 강퇴한 것인지 체크
	if( kPacket_.m_iUnitUID == kPacket_.m_iTargetUnitUID )
		return;

	// 강퇴당한 길드원은 현재 접속중인가?
	KUserList::KGSUserInfo kGSUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iTargetUnitUID, kGSUserInfo ) == true )
		return;

    // 오프라인이라면 편지로 강퇴당한걸 알려주러가자!
	KELG_KICK_RESULT_LETTER_NOT kPacketNot;
	kPacketNot.m_iUnitUID = kPacket_.m_iTargetUnitUID;
	kPacketNot.m_wstrGuildName = wstrGuildName;
	SendToGameServer( ELG_KICK_RESULT_LETTER_NOT, kPacketNot );
}

_IMPL_ON_FUNC( ELG_UPDATE_CHANGE_GUILD_MEMBER_MESSAGE_NOT, KDBE_CHANGE_GUILD_MEMBER_MESSAGE_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 변경된 정보를 업데이트하고 길드원들에게 알린다!
	if( SiKGuildManager()->UpdateChangeGuildMemberMessage( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드원 인사말 변경 정보 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iTargetUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrMessage )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( ELG_UPDATE_DISBAND_GUILD_NOT, KDBE_DISBAND_GUILD_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드가 해산되었으므로 길드정보를 지우자!
	if( SiKGuildManager()->DisbandGuild( kPacket_.m_iGuildUID ) == false )
	{
		START_LOG( cerr, L"길드 해산 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}

	//{{ 2010. 01. 25  최육사	길드 게시판
#ifdef SERV_GUILD_AD

	// 길드가 해산되었으므로 길드광고가 있는지 체크해서 삭제하자!
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		KDBE_DELETE_GUILD_AD_LIST_NOT kPacketNot;

		// 길드 가입 신청 리스트 삭제
		SiKGuildBoardManager()->DeleteApplyJoinGuildList( kPacket_.m_iGuildUID, kPacketNot.m_vecDeleteApply );

		// 게시판에서 해당 길드 광고 삭제
		SiKGuildBoardManager()->DeleteGuildAd( kPacket_.m_iGuildUID );

		kPacketNot.m_vecDeleteGuildAd.push_back( kPacket_.m_iGuildUID );
		SendToGameDB( DBE_DELETE_GUILD_AD_LIST_NOT, kPacketNot );
	}

#endif SERV_GUILD_AD
	//}}
}

IMPL_ON_FUNC( ELG_UPDATE_GUILD_MEMBER_INFO_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드 멤버 정보 갱신!
	if( SiKGuildManager()->UpdateGuildMemberInfo( kPacket_.m_iGuildUID, kPacket_.m_kMemberInfo ) == false )
	{
		// 시간차로 발생 가능하다.
		START_LOG( cwarn, L"길드 멤버 정보 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_CHECK_EXPAND_GUILD_MAX_MEMBER_ACK kPacket;

	// 길드 인원 확장 가능한지 체크하자!
	if( SiKGuildManager()->CheckGuildMaxMember( kPacket_ ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
		kPacket.m_iItemUID = kPacket_.m_iItemUID;
	}

	SendToGSUser( FIRST_SENDER_UID, ELG_CHECK_EXPAND_GUILD_MAX_MEMBER_ACK, kPacket );
}

_IMPL_ON_FUNC( DLG_UPDATE_EXPAND_GUILD_MAX_MEMBER_NOT, KDBE_EXPAND_GUILD_MAX_MEMBER_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드 멤버 최대 인원 확장!
	if( SiKGuildManager()->UpdateGuildMaxMember( kPacket_.m_iGuildUID, kPacket_.m_usMaxNumMember ) == false )
	{
		START_LOG( cerr, L"길드 최대 인원 정보 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_usMaxNumMember )
			<< END_LOG;
	}
}

//{{ 2009. 10. 27  최육사	길드레벨
_IMPL_ON_FUNC( ELG_UPDATE_GUILD_EXP_NOT, KDBE_UPDATE_GUILD_EXP_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드 경험치 업데이트
	if( SiKGuildManager()->UpdateGuildEXP( kPacket_.m_iGuildUID, kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 경험치 정보 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iGuildEXP )
			<< END_LOG;
	}

	//{{ 2010. 01. 25  최육사	길드 게시판
#ifdef SERV_GUILD_AD

	// 등록된 길드 광고가 있다면 경험치 정보를 업데이트 하자!
	SiKGuildBoardManager()->UpdateGuildEXP( kPacket_.m_iGuildUID, kPacket_.m_iGuildEXP );

#endif SERV_GUILD_AD
	//}}
}
//}}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
IMPL_ON_FUNC( ELG_CREATE_GUILD_REQ )
{
	if( SiKGuildManager()->ReserveGuildName( kPacket_.m_iUnitUID, kPacket_.m_wstrGuildName ) == false )
	{
		KELG_CREATE_GUILD_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_01;
		SendToGSUser( FIRST_SENDER_UID, ELG_CREATE_GUILD_ACK, kPacket );
		return;
	}
	anTrace_[1] = GetUID();
	KncSend( GetPfID(), GetUID(), PI_LOGIN_GAME_DB, 0, anTrace_, DBE_CREATE_GUILD_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_CREATE_GUILD_ACK )
{
	SiKGuildManager()->CancelGuildName( kPacket_.m_iUnitUID );
	SendToGSUser( FIRST_SENDER_UID, ELG_CREATE_GUILD_ACK, kPacket_ );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_CHECK_REQ )
{
	if( SiKGuildManager()->ReserveGuildName( kPacket_.m_iUnitUID, kPacket_.m_wstrNewGuildName ) == false )
	{
		KELG_CHANGE_GUILD_NAME_CHECK_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_GUILD_01;
		SendToGSUser( FIRST_SENDER_UID, ELG_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
		return;
	}
	anTrace_[1] = GetUID();
	KncSend( GetPfID(), GetUID(), PI_LOGIN_GAME_DB, 0, anTrace_, DBE_CHANGE_GUILD_NAME_CHECK_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_NAME_CHECK_ACK )
{
	KELG_CHANGE_GUILD_NAME_CHECK_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		SiKGuildManager()->CancelGuildName( kPacket_.m_iUnitUID );
		kPacket.m_iOK = kPacket_.m_iOK;
	}
	else if( SiKGuildManager()->CheckReservedGuildName( kPacket_.m_iUnitUID ) == false )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_51;
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
	}
	SendToGSUser( FIRST_SENDER_UID, ELG_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_TIME_CHECK_REQ )
{
	KELG_CHANGE_GUILD_NAME_TIME_CHECK_ACK kPacket;
	kPacket.m_ulOrderNo = kPacket_.m_ulOrderNo;
	kPacket.m_ulProductNo = kPacket_.m_ulProductNo;
	kPacket.m_iQuantity = kPacket_.m_iQuantity;

	if( SiKGuildManager()->CheckReservedGuildName( kPacket_.m_iUnitUID ) == true )
	{
		SiKGuildManager()->TimeRefresh( kPacket_.m_iUnitUID );
		kPacket.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_GUILD_51;
	}
	SendToGSUser( FIRST_SENDER_UID, ELG_CHANGE_GUILD_NAME_TIME_CHECK_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_REQ )
{
	SiKGuildManager()->GetReservedGuildName( kPacket_.m_iUnitUID, kPacket_.m_wstrNewGuildName );
	anTrace_[1] = GetUID();
	KncSend( GetPfID(), GetUID(), PI_LOGIN_GAME_DB, 0, anTrace_, DBE_CHANGE_GUILD_NAME_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_NAME_ACK )
{
	SiKGuildManager()->CancelGuildName( kPacket_.m_iUnitUID );
	SendToGSUser( FIRST_SENDER_UID, ELG_CHANGE_GUILD_NAME_ACK, kPacket_ );
}

IMPL_ON_FUNC( ELG_CHANGE_GUILD_NAME_RESULT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KGuildPtr kGuildPtr = SiKGuildManager()->GetGuild( kPacket_.m_iGuildUID );
	if( kGuildPtr == NULL )
	{
		START_LOG( cerr, L"GuildUID에 해당하는 Guild가 존재하지 않습니다. ( GameServer와 LoginServer 데이터 불일치 발생 가능성")
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_wstrGuildName )
			<< END_LOG;
		return;
	}

	kGuildPtr->SetGuildName( kPacket_.m_wstrGuildName );

	// 길드 광고가 등록되어있는지 확인하고 있다면 길드 이름 정보를 업데이트 한다.
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		// 길드 이름 업데이트
		SiKGuildBoardManager()->UpdateGuildName( kPacket_.m_iGuildUID, kPacket_.m_wstrGuildName );
	}

	// 모든 길드원에게 길드정보 변경을 알리자!
	{
		KEGS_GUILD_INFO_NOT kPacketNot;
		//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
		kGuildPtr->GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID, kPacketNot.m_kGuildSkillInfo );
#else
		kGuildPtr->GetGuildInfo( kPacketNot.m_kGuildInfo, kPacketNot.m_wstrGuildMasterName, kPacketNot.m_vecMemberUnitUID );
#endif GUILD_SKILL_TEST
		//}}
		kGuildPtr->BroadCast( ELG_GUILD_INFO_NOT, kPacketNot );
	}

	//// 변경된 길드 메시지도 날리자!
	//{
	//	KEGS_GUILD_MESSAGE_NOT kPacket;
	//	kPacket.m_cMessageType		= KEGS_GUILD_MESSAGE_NOT::MT_CHANGE_GUILD_NAME;
	//	kPacket.m_wstrMessage		= kPacket_.m_wstrGuildName;
	//	kGuildPtr->BroadCast( ELG_GUILD_MESSAGE_NOT, kPacket );
	//}
}

#endif SERV_GUILD_CHANGE_NAME
//}}

#endif GUILD_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2009. 10. 27  최육사	랭킹
IMPL_ON_FUNC( ELG_FIND_DELETE_CHAR_IN_RANKING_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	std::vector< int >::const_iterator vit;
	for( vit = kPacket_.m_vecRankingType.begin(); vit != kPacket_.m_vecRankingType.begin(); ++vit )
	{
		if( SiKRankingManager()->DeleteRecord( *vit, kPacket_.m_iUnitUID ) == false )
		{
            START_LOG( cerr, L"캐릭터 삭제로 인한 랭킹 정보 삭제 처리 실패! " )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( *vit );
		}
	}
}
//}}

//////////////////////////////////////////////////////////////////////////
//{{ 2009. 11. 23  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KDBE_GET_GUILD_SKILL_REQ kPacketToDB;

	// 길드 스킬 배우기
	if( SiKGuildManager()->CheckLearnGuildSkill( kPacket_, kPacketToDB ) == false )
	{
		KEGS_GET_GUILD_SKILL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_GET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// GameDB로 가서 길드 스킬을 배우자!
	SendToGameDB( DBE_GET_GUILD_SKILL_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_GET_GUILD_SKILL_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_GET_GUILD_SKILL_ACK kPacketAck;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_GET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// DB에 저장하고온 길드 스킬을 등록하자!
	if( SiKGuildManager()->InsertGuildSkill( kPacket_ ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_GET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	kPacketAck.m_iOK = NetError::NET_OK;
	SendToGSCharacter( kPacket_.m_iUnitUID, ELG_GET_GUILD_SKILL_ACK, kPacketAck );
}

IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
    
	KDBE_RESET_GUILD_SKILL_REQ kPacketToDB;

	// 길드 스킬 되돌리기 체크
	if( SiKGuildManager()->CheckResetGuildSkill( kPacket_, kPacketToDB ) == false )
	{
		KEGS_RESET_GUILD_SKILL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_RESET_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// GSUser로 가서 길드 스킬 되돌리기 아이템을 삭제처리하자!
	SendToGSUser( FIRST_SENDER_UID, ELG_RESET_GUILD_SKILL_DEL_ITEM_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( ELG_RESET_GUILD_SKILL_DEL_ITEM_ACK, KDBE_RESET_GUILD_SKILL_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드 스킬 되돌리기
	if( SiKGuildManager()->ResetGuildSkill( kPacket_ ) == false )
	{
        START_LOG( cerr, L"길드 스킬 되돌리기 실패! 절대 일어날수 없는 에러!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iGuildSkillID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KDBE_INIT_GUILD_SKILL_REQ kPacketToDB;

	// 길드 스킬 초기화 체크
	if( SiKGuildManager()->CheckInitGuildSkill( kPacket_, kPacketToDB ) == false )
	{
		KEGS_INIT_GUILD_SKILL_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_INIT_GUILD_SKILL_ACK, kPacketAck );
		return;
	}

	// GSUser로 가서 길드 스킬 초기화 아이템을 삭제처리하자!
	SendToGSUser( FIRST_SENDER_UID, ELG_INIT_GUILD_SKILL_DEL_ITEM_REQ, kPacketToDB );
}

_IMPL_ON_FUNC( ELG_INIT_GUILD_SKILL_DEL_ITEM_ACK, KDBE_INIT_GUILD_SKILL_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드 스킬 초기화
	if( SiKGuildManager()->InitGuildSkill( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 스킬 초기화 실패! 절대 일어날수 없는 에러!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iGuildSPoint )
			<< BUILD_LOG( kPacket_.m_iGuildCSPoint )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ELG_INSERT_GUILD_CASH_SKILL_POINT_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드 캐쉬 스킬 포인트
	if( SiKGuildManager()->InsertGuildCashSkillPoint( kPacket_ ) == false )
	{
		START_LOG( cerr, L"길드 캐쉬 스킬 포인트 업데이트 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iGuildCSPoint )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ELG_ADMIN_GET_GUILD_SKILL_POINT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 길드 스킬 포인트 치트
	if( SiKGuildManager()->AdminInsertGuildSkillPoint( kPacket_ ) == false )
	{
		START_LOG( cwarn, L"길드 스킬 포인트 치트 사용 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildSPoint )
			<< END_LOG;

		KEGS_ADMIN_GET_GUILD_SKILL_POINT_ACK kPacket;
		kPacket.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
	}
}

#endif GUILD_SKILL_TEST
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD

IMPL_ON_FUNC( ELG_GET_GUILD_AD_LIST_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );
    
	KEGS_GET_GUILD_AD_LIST_ACK kPacket;
	kPacket.m_bCanShowApplyList = false;
	kPacket.m_bRegMyGuildAd = false;	
	
	// 길드 광고 리스트 얻기
	SiKGuildBoardManager()->GetGuildAdList( kPacket_, kPacket );

	// 길드 가입 신청 리스트 볼수 있는지 검사
	if( kPacket_.m_iGuildUID > 0 )
	{
		if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
		{
			kPacket.m_bCanShowApplyList = !SiKGuildBoardManager()->IsExpiredGuildAd( kPacket_.m_iGuildUID );
			kPacket.m_bRegMyGuildAd = kPacket.m_bCanShowApplyList;
		}
	}
	else
	{
		int iGuildUID = 0;
        kPacket.m_bCanShowApplyList = SiKGuildBoardManager()->GetMyApplyJoinGuildUID( kPacket_.m_iUnitUID, iGuildUID );

		if( SiKGuildBoardManager()->IsExistGuildAd( iGuildUID ) == true )
		{
			kPacket.m_bCanShowApplyList = !SiKGuildBoardManager()->IsExpiredGuildAd( iGuildUID );
		}

		kPacket.m_bRegMyGuildAd = false;
	}

	SendToGSUser( FIRST_SENDER_UID, ELG_GET_GUILD_AD_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_GET_GUILD_SKILL_IN_BOARD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_GET_GUILD_SKILL_IN_BOARD_ACK kPacket;
    
	// 광고가 등록된 길드인지 확인한다.
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == false )
	{
		START_LOG( cerr, L"광고가 등록되지 않은 길드의 스킬 보기를 요청하였습니다." )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_01;
		SendToGSUser( FIRST_SENDER_UID, ELG_GET_GUILD_SKILL_IN_BOARD_ACK, kPacket );
		return;
	}

	// 길드 스킬 정보를 얻자!
	if( SiKGuildManager()->GetGuildSkillInfo( kPacket_.m_iGuildUID, kPacket.m_kGuildSkillInfo ) == false )
	{
		// 길드 매니저에서 못찾았으면 길드 광고에서 찾아보자!
		if( SiKGuildBoardManager()->GetGuildSkillInfo( kPacket_.m_iGuildUID, kPacket.m_kGuildSkillInfo ) == false )
		{
			// 그래도 못찾았으면 DB로 가서 얻어오자!
			SendToGameDB( DBE_GET_GUILD_SKILL_IN_BOARD_REQ, kPacket_ );
			return;
		}
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSUser( FIRST_SENDER_UID, ELG_GET_GUILD_SKILL_IN_BOARD_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_GET_GUILD_SKILL_IN_BOARD_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

    // GuildBoardManager에 길드스킬정보를 저장해두자!
	if( SiKGuildBoardManager()->SetGuildSkillInfo( kPacket_.m_iGuildUID, kPacket_.m_kGuildSkillInfo ) == false )
	{
		START_LOG( cerr, L"DB로부터 받아온 길드 스킬 정보 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;
	}

    KEGS_GET_GUILD_SKILL_IN_BOARD_ACK kPacket;
	kPacket.m_kGuildSkillInfo = kPacket_.m_kGuildSkillInfo;
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( kPacket_.m_iUnitUID, ELG_GET_GUILD_SKILL_IN_BOARD_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_REGISTRATION_GUILD_AD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_REGISTRATION_GUILD_AD_ACK kPacket;
	kPacket.m_iCost = kPacket_.m_iCost;
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;

	// 길드 광고 등록 가능한 길드원등급인지 체크
	if( SiKGuildManager()->CheckRegistrationGuildAd( kPacket_.m_iGuildUID, kPacket_.m_iUnitUID ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

	// 이미 해당 길드에서 등록한 길드광고가 있는지 확인
	bool bExistExpiredAd = false;

	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		// 그렇다면 기간이 다된 광고인지 확인
		bExistExpiredAd = SiKGuildBoardManager()->IsExpiredGuildAd( kPacket_.m_iGuildUID );
		if( bExistExpiredAd == false )
		{
			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_05;
			SendToGSUser( FIRST_SENDER_UID, ELG_REGISTRATION_GUILD_AD_ACK, kPacket );
			return;
		}
	}

	// DB로 광고 등록하러 가자!
	KDBE_REGISTRATION_GUILD_AD_REQ kPacketToDB;
	kPacketToDB.m_iUnitUID			= kPacket_.m_iUnitUID;
	kPacketToDB.m_iGuildUID 		= kPacket_.m_iGuildUID;
	kPacketToDB.m_sPeriod			= kPacket_.m_sPeriod;
	kPacketToDB.m_wstrAdMessage		= kPacket_.m_wstrAdMessage;
	kPacketToDB.m_iCost				= kPacket_.m_iCost;
	kPacketToDB.m_bExistExpiredAd	= bExistExpiredAd;
	SendToGameDB( DBE_REGISTRATION_GUILD_AD_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_REGISTRATION_GUILD_AD_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_REGISTRATION_GUILD_AD_ACK kPacket;
	kPacket.m_iCost = kPacket_.m_iCost;
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cwarn, L"DB에 길드 광고 등록 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = kPacket_.m_iOK;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}

    // 길드 광고 정보 만들기
	KGuildAdInfo kGuildAdInfo;

	if( SiKGuildManager()->MakeGuildAdInfo( kPacket_, kGuildAdInfo ) == false )
	{
		START_LOG( cerr, L"길드 광고 정보 만들기 실패! 절대 일어나서는 안되는에러!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_06;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_REGISTRATION_GUILD_AD_ACK, kPacket );
		return;
	}
	    
	if( kPacket_.m_bExistExpiredAd == false )
	{
		// 길드 게시판에 새로 광고 등록
		SiKGuildBoardManager()->NewGuildAd( kGuildAdInfo );
	}
	else
	{
		// 기간 만료 해제
		SiKGuildBoardManager()->UnexpiredGuildAd( kGuildAdInfo );
	}

	// 광고 등록 성공!
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( kPacket_.m_iUnitUID, ELG_REGISTRATION_GUILD_AD_ACK, kPacket );

	// 길드 멤버들에게 길드 광고가 등록되었다고 알리자!
	SiKGuildManager()->ResultRegGuildAd( kPacket_.m_iGuildUID, kPacket_.m_wstrAdEndDate );
}

IMPL_ON_FUNC( ELG_MODIFY_REG_GUILD_AD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_MODIFY_REG_GUILD_AD_ACK kPacket;
	kPacket.m_iCost = kPacket_.m_iCost;
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;

	// 길드 광고 등록 가능한 길드원등급인지 체크
	if( SiKGuildManager()->CheckRegistrationGuildAd( kPacket_.m_iGuildUID, kPacket_.m_iUnitUID ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// 이미 해당 길드에서 등록한 길드광고가 있는지 확인
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == false )
	{
		START_LOG( cerr, L"이미 등록한 길드 광고가 있는 경우에만 패킷이 와야함." )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_UNKNOWN;
		SendToGSUser( FIRST_SENDER_UID, ELG_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// 그렇다면 기간이 다된 광고인지 확인
	if( SiKGuildBoardManager()->IsExpiredGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		START_LOG( cerr, L"기간이 아직 다 되지 않은 광고가 있는 경우에만 패킷이 와야함." )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_05;
		SendToGSUser( FIRST_SENDER_UID, ELG_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// DB로 광고 등록하러 가자!
	SendToGameDB( DBE_MODIFY_REG_GUILD_AD_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_MODIFY_REG_GUILD_AD_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_MODIFY_REG_GUILD_AD_ACK kPacket;
	kPacket.m_iCost = kPacket_.m_iCost;
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cwarn, L"DB에 길드 광고 정보 수정 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = kPacket_.m_iOK;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// 길드 광고 정보 수정하기!
	KGuildAdInfo kGuildAdInfo;

	if( SiKGuildManager()->MakeGuildAdInfo( kPacket_, kGuildAdInfo ) == false )
	{
		START_LOG( cerr, L"길드 광고 정보 만들기 실패! 절대 일어나서는 안되는에러!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_06;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_MODIFY_REG_GUILD_AD_ACK, kPacket );
		return;
	}

	// 광고 수정 정보를 길드 게시판에 업데이트
	SiKGuildBoardManager()->UpdateGuildAdInfo( kGuildAdInfo );

	// 광고 등록 성공!
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( kPacket_.m_iUnitUID, ELG_MODIFY_REG_GUILD_AD_ACK, kPacket );

	// 길드 멤버들에게 길드 광고가 등록되었다고 알리자!
	SiKGuildManager()->ResultRegGuildAd( kPacket_.m_iGuildUID, kPacket_.m_wstrAdEndDate );
}

IMPL_ON_FUNC( ELG_APPLY_JOIN_GUILD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_APPLY_JOIN_GUILD_ACK kPacket;
	KDBE_APPLY_JOIN_GUILD_REQ kPacketToDB;

	// 가입 신청 처리
	if( SiKGuildBoardManager()->ApplyJoinGuild( kPacket_, kPacketToDB.m_vecDeleteApply ) == false )
	{
		kPacket.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// 가입 신청을 성공하였다면 DB에 업데이트 하러 가자!
	kPacketToDB.m_kApplyJoinGuildInfo = kPacket_.m_kApplyJoinGuildInfo;
	kPacketToDB.m_bOldApplyDelete = kPacket_.m_bOldApplyDelete;
	SendToGameDB( DBE_APPLY_JOIN_GUILD_REQ, kPacketToDB );
}

IMPL_ON_FUNC( DBE_APPLY_JOIN_GUILD_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		// DB업데이트 실패! 등록했던 가입신청을 삭제한다!
		SiKGuildBoardManager()->RollbackApplyJoinGuild( kPacket_ );
	}
	else
	{
		// DB업데이트 성공! 가입신청 정보 업데이트(RegDate)
        SiKGuildBoardManager()->UpdateApplyJoinGuildInfo( kPacket_ );
	}

	KEGS_APPLY_JOIN_GUILD_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	SendToGSCharacter( kPacket_.m_kApplyJoinGuildInfo.m_iUnitUID, ELG_APPLY_JOIN_GUILD_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_GET_APPLY_JOIN_GUILD_LIST_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_GET_APPLY_JOIN_GUILD_LIST_ACK kPacket;

	// 길드 가입자라면?
	if( kPacket_.m_iGuildUID == 0 )
	{
		// 가입 신청한 길드UID얻기
		if( SiKGuildBoardManager()->GetMyApplyJoinGuildUID( kPacket_.m_iUnitUID, kPacket_.m_iGuildUID ) == false )
		{
			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_14;
			SendToGSUser( FIRST_SENDER_UID, ELG_GET_APPLY_JOIN_GUILD_LIST_ACK, kPacket );
			return;
		}

		if( SiKGuildBoardManager()->IsExistApplyJoinGuild( kPacket_.m_iGuildUID, kPacket_.m_iUnitUID ) == false )
		{
			START_LOG( clog, L"길드 가입 신청 리스트를 볼수 없는 유저입니다." )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_14;
			SendToGSUser( FIRST_SENDER_UID, ELG_GET_APPLY_JOIN_GUILD_LIST_ACK, kPacket );
			return;
		}
	}
	// 이미 길드에 소속된 유저라면?
	else
	{
		// 길드 가입자 리스트를 볼수 있는 길드원인지 체크
		if( SiKGuildManager()->CheckGetApplyJoinGuildList( kPacket_ ) == false )
		{
			START_LOG( clog, L"길드 가입 신청 리스트를 볼수 없는 유저입니다." )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::GetLastError();
			SendToGSUser( FIRST_SENDER_UID, ELG_GET_APPLY_JOIN_GUILD_LIST_ACK, kPacket );
			return;
		}
	}

	// 길드 가입자 리스트 얻기
	if( SiKGuildBoardManager()->GetApplyJoinGuildList( kPacket_, kPacket ) == false )
	{
		START_LOG( cerr, L"길드 가입 신청 리스트 받아오기 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_GET_APPLY_JOIN_GUILD_LIST_ACK, kPacket );
		return;
	}

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSUser( FIRST_SENDER_UID, ELG_GET_APPLY_JOIN_GUILD_LIST_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_ACCEPT_JOIN_GUILD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_INVITE_GUILD_FIND_USER_REQ kPacket;
	KEGS_ACCEPT_JOIN_GUILD_ACK kPacketAck;

	// 존재하는 가입 신청인지 확인
	if( SiKGuildBoardManager()->IsExistApplyJoinGuild( kPacket_.m_iGuildUID, kPacket_.m_iApplyJoinUnitUID ) == false )
	{
		kPacketAck.m_iOK = NetError::ERR_GUILD_BOARD_10;
		SendToGSUser( FIRST_SENDER_UID, ELG_ACCEPT_JOIN_GUILD_ACK, kPacketAck );
		return;
	}

	// 길드 가입 수락이 가능한지 체크
	if( SiKGuildManager()->CheckInviteCondition( kPacket_.m_iGuildUID, kPacket_.m_iApplyJoinUnitUID, kPacket_.m_iUnitUID, kPacket ) == false )
	{
		kPacketAck.m_iOK = NetError::GetLastError();
		SendToGSUser( FIRST_SENDER_UID, ELG_ACCEPT_JOIN_GUILD_ACK, kPacketAck );
		return;
	}

	// DB로 가서 가입 신청을 먼저 지우자!
	SendToGameDB( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_INVITE_GUILD_FIND_USER_REQ kPacket;
	KEGS_ACCEPT_JOIN_GUILD_ACK kPacketAck;

	// DB에서 길드 가입 신청 삭제 실패
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacketAck.m_iOK = kPacket_.m_iOK;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_ACCEPT_JOIN_GUILD_ACK, kPacketAck );
		return;
	}

	// 길드 가입 수락이 가능한지 체크
	if( SiKGuildManager()->CheckInviteCondition( kPacket_.m_iGuildUID, kPacket_.m_iApplyJoinUnitUID, kPacket_.m_iUnitUID, kPacket ) == false )
	{
		START_LOG( cerr, L"길드 가입 수락 권한이 없습니다. 먼저 체크를 했었을텐데.. 절대 일어나선 안되는 에러" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iApplyJoinUnitUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		// 기존에 검사를 한번 했었기 때문에 그냥 통과시킨다.
	}

	// 가입 수락 대상자가 현재 접속중인지 확인
	KUserList::KGSUserInfo kGSUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iApplyJoinUnitUID, kGSUserInfo ) == true )
	{
		// 접속중이라면 길드 초대 기능을 그대로 사용 ( 대신 초대장을 받은 초대 대상자는 강제로 가입수락 처리 되는 방식으로 함 )
		kPacket.m_usEventID = DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK;
		kPacket.m_bAutoJoinGuild = true; // 강제 가입 수락
		kPacket.m_iSenderUnitUID = kPacket_.m_iUnitUID;
		kPacket.m_iReceiverUnitUID = kGSUserInfo.m_nUnitUID;
		SendToGSUser( kGSUserInfo.m_nGSUID, kGSUserInfo.m_nUserUID, ELG_INVITE_GUILD_FIND_USER_REQ, kPacket );
	}
	else
	{
		// 접속중이 아니라면 강제로 가입 처리
		KDBE_ACCEPT_JOIN_GUILD_REQ kPacketToDB;
		kPacketToDB.m_iGuildUID = kPacket_.m_iGuildUID;
		if( SiKGuildBoardManager()->MakeGuildMemberInfoForAcceptJoinGuild( kPacket_.m_iGuildUID, kPacket_.m_iApplyJoinUnitUID, kPacketToDB.m_kJoinGuildMemberInfo ) == false )
		{
			START_LOG( cerr, L"길드 가입 수락 실패!" )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iApplyJoinUnitUID )
				<< END_LOG;

			kPacketAck.m_iOK = NetError::ERR_GUILD_BOARD_11;
			SendToGSCharacter( kPacket_.m_iUnitUID, ELG_ACCEPT_JOIN_GUILD_ACK, kPacketAck );
			return;
		}

		// 정상적으로 가입 수락 절차를 진행했으므로 OK값 보내자!
		kPacketAck.m_iOK = NetError::NET_OK;
		kPacketAck.m_wstrReceiverNickName = kPacketToDB.m_kJoinGuildMemberInfo.m_wstrNickName;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_ACCEPT_JOIN_GUILD_ACK, kPacketAck );

		// DB로가서 가입 진행!
		SendToGameDB( DBE_ACCEPT_JOIN_GUILD_REQ, kPacketToDB );
	}

	// 정상적으로 삭제처리가 되었으면 가입 신청 리스트에서도 삭제
	if( SiKGuildBoardManager()->DeleteApplyJoinGuild( kPacket_.m_iApplyJoinUnitUID ) == false )
	{
		START_LOG( cerr, L"존재하는 가입신청인지 체크 했는데 삭제 실패했다?" )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iApplyJoinUnitUID )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( ELG_DELETE_APPLY_JOIN_GUILD_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_DELETE_APPLY_JOIN_GUILD_ACK kPacket;

	// 가입 신청자라면..
	if( kPacket_.m_iGuildUID == 0  &&  kPacket_.m_iUnitUID == kPacket_.m_iDeletedUnitUID )
	{
		int iGuildUID = 0;
        if( SiKGuildBoardManager()->GetMyApplyJoinGuildUID( kPacket_.m_iUnitUID, iGuildUID ) == false )
		{
			// 캐릭터 삭제시 요청이라면 여기서 끝낸다.
			if( kPacket_.m_bDeleteChar == true )
			{
				return;
			}

			START_LOG( cerr, L"존재하지 않는 가입 신청입니다." )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iDeletedUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_10;
			SendToGSUser( FIRST_SENDER_UID, ELG_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
			return;
		}

		kPacket_.m_iGuildUID = iGuildUID;

		// 존재하는 가입 신청인지 확인
		if( SiKGuildBoardManager()->IsExistApplyJoinGuild( kPacket_.m_iGuildUID, kPacket_.m_iDeletedUnitUID ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 가입 신청입니다." )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iDeletedUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_10;
			SendToGSUser( FIRST_SENDER_UID, ELG_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
			return;
		}
	}
	// 길드 마스터, 길드 관리자라면..
	else
	{
		// 존재하는 가입 신청인지 확인
		if( SiKGuildBoardManager()->IsExistApplyJoinGuild( kPacket_.m_iGuildUID, kPacket_.m_iDeletedUnitUID ) == false )
		{
			START_LOG( cerr, L"존재하지 않는 가입 신청입니다." )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iDeletedUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_10;
			SendToGSUser( FIRST_SENDER_UID, ELG_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
			return;
		}

		// 삭제 가능한 길드 등급인지 검사
		if( SiKGuildManager()->CheckDeleteApplyJoin( kPacket_ ) == false )
		{
			START_LOG( cerr, L"길드 가입 신청을 삭제할 권한이 없습니다." )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iDeletedUnitUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_00;
			SendToGSUser( FIRST_SENDER_UID, ELG_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
			return;
		}
	}

	// DB로 먼저가서 삭제하자!
	SendToGameDB( DBE_DELETE_APPLY_JOIN_GUILD_REQ, kPacket_ );
}

IMPL_ON_FUNC( DBE_DELETE_APPLY_JOIN_GUILD_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_DELETE_APPLY_JOIN_GUILD_ACK kPacket;

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = kPacket_.m_iOK;
		SendToGSCharacter( kPacket_.m_iUnitUID, ELG_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
		return;
	}

	// 길드 가입 신청 삭제
	if( SiKGuildBoardManager()->DeleteApplyJoinGuild( kPacket_.m_iDeletedUnitUID ) == false )
	{
		START_LOG( cerr, L"길드 가입 신청을 삭제할 권한이 없습니다." )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iDeletedUnitUID )
			<< END_LOG;
	}

	// 캐릭터 삭제라면 여기서 끝
	if( kPacket_.m_bDeleteChar == true )
		return;
	
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSCharacter( kPacket_.m_iUnitUID, ELG_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
}

#endif SERV_GUILD_AD
//}}

//{{ 2010. 01. 11  최육사	추천인리스트
_IMPL_ON_FUNC( ELG_GET_RECOMMEND_USER_LIST_REQ, KDBE_GET_RECOMMEND_USER_LIST_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 현재 접속중인 추천인인지 체크!
	std::vector< KRecommendUserInfo >::iterator vit = kPacket_.m_vecRecommendUserList.begin();
	for( ; vit != kPacket_.m_vecRecommendUserList.end(); ++vit )
	{
		if( GetKLoginSimLayer()->IsLoginedUnitUID( vit->m_iUnitUID ) == true )
		{
			// 온라인 상태
			vit->m_bIsOnline = true;
		}
	}

	SendToGSUser( FIRST_SENDER_UID, ELG_GET_RECOMMEND_USER_LIST_ACK, kPacket_ );
}

IMPL_ON_FUNC( ELG_RECOMMEND_USER_INFO_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iRecommendedUnitUID, kUserInfo ) == false )
	{
		// 현재 접속하지 않은 유저라면 여기서 끝~!
		return;
	}

	//찾는 캐릭이 있으면 해당 캐릭터의 UID 전송
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_RECOMMEND_USER_INFO_NOT, kPacket_ );
}
//}}

//{{ 2010. 05. 31  최육사	동접 정보 개편
#ifdef SERV_CCU_NEW

IMPL_ON_FUNC( ELG_UPDATE_CCU_INFO_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 동접 정보 집계
	SiKServerCCUManager()->UpdateCCUInfo( kPacket_.m_kCCUInfo );
}

#endif SERV_CCU_NEW
//}}

//{{ 2010. 8. 16	최육사	기간 리셋 아이템 이벤트
#ifdef SERV_RESET_PERIOD_EVENT

_IMPL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_REQ, KEGS_ADMIN_SHOW_ED_CHEAT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrNickName, kUserInfo ) == false )
	{
		KEGS_ADMIN_SHOW_ED_CHEAT_ACK kPacket;
		kPacket.m_iOK		= NetError::ERR_UNKNOWN;
		SendToGSUser( FIRST_SENDER_UID, ELG_ADMIN_SHOW_ED_CHEAT_ACK, kPacket );
		return;
	}

	//찾는 캐릭이 있으면 해당 GS로 정보요청
	KELG_ADMIN_SHOW_ED_CHEAT_REQ kPacket;
	kPacket.m_iSenderUserUID = FIRST_SENDER_UID;
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_ADMIN_SHOW_ED_CHEAT_REQ, kPacket );
}

IMPL_ON_FUNC( ELG_ADMIN_SHOW_ED_CHEAT_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( kPacket_.m_iSenderUserUID, kUserInfo ) == false )
	{
		START_LOG( cerr, L"운영자가 현재 접속중이 아닙니다." )
			<< BUILD_LOG( kPacket_.m_iSenderUserUID )
			<< END_LOG;
		return;
	}

	//결과를 전송하자!
	KEGS_ADMIN_SHOW_ED_CHEAT_ACK kPacket;
	kPacket.m_iOK = kPacket_.m_iOK;
	kPacket.m_wstrNickName = kPacket_.m_wstrNickName;
	kPacket.m_iED = kPacket_.m_iED;
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_ADMIN_SHOW_ED_CHEAT_ACK, kPacket );
}

#endif SERV_RESET_PERIOD_EVENT
//}}

//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK

IMPL_ON_FUNC( ELG_REG_REJECTED_USER_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	START_LOG( cout, L"해킹 의심 유저 접속 제한 조치!" )
		<< BUILD_LOG( kPacket_.m_iUserUID );

#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
	if( kPacket_.m_cPeriodUAL == SEnum::UAL_NORMAL )
	{
		SiKAccountBlockManager()->AddBlockedAccount( kPacket_.m_iUserUID, KAccountBlockManager::BT_CONNECT_BLOCK );
	}
#else // SERV_PERIOD_ACCOUNT_BLOCK
	SiKAccountBlockManager()->AddBlockedAccount( kPacket_.m_iUserUID, KAccountBlockManager::BT_CONNECT_BLOCK );
#endif // SERV_PERIOD_ACCOUNT_BLOCK

	SendToAccountDB( DBE_REG_REJECTED_USER_NOT, kPacket_ );
}

#endif SERV_CN_SERVER_HACKING_CHECK
//}}

//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
_IMPL_ON_FUNC( ELG_REG_TRADE_BLOCK_USER_NOT, KELG_REG_REJECTED_USER_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	START_LOG( cout, L"해킹 의심 유저 거래 제한 조치!" )
		<< BUILD_LOG( kPacket_.m_iUserUID );

	SiKAccountBlockManager()->AddBlockedAccount( kPacket_.m_iUserUID, KAccountBlockManager::BT_TRADE_BLOCK );

	SendToAccountDB( DBE_REG_TRADE_BLOCK_USER_NOT, kPacket_ );
}
#endif SERV_HACK_USER_TRADE_BLOCK
//}}

//{{ 2010. 10. 12	최육사	서버 모니터링
#ifdef SERV_MORNITORING
_IMPL_ON_FUNC( ELG_DISCONNECT_SERVER_REPORT_NOT, KE_DISCONNECT_SERVER_REPORT_NOT )
{
	switch( kPacket_.m_cType )
	{
	case KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_CN_PROXY );
		}
		break;

	case KMornitoringManager::ET_DISCONNECT_GAME_BILLING:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_GAME_BILLING );
		}
		break;

		//{{ 2012. 01. 27  김민성	로그인 인증서버 연결 끊김 자동 sms 전송
#ifdef SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
	case KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH:
		{
			SiKMornitoringManager()->IncreaseDisconnectCount( KMornitoringManager::ET_DISCONNECT_LOGIN_AUTH );
		}
		break;
#endif SERV_NEXON_AUTH_SERVER_DISCONNECT_SMS
		//}}

	default:
		START_LOG( cerr, L"잘못된 타입입니다!" )
			<< BUILD_LOGc( kPacket_.m_cType )
			<< END_LOG;
		return;
	}
}
#endif SERV_MORNITORING
//}}

//{{ 2010. 10. 13	최육사	DLL Manager
#ifdef SERV_DLL_MANAGER
_IMPL_ON_FUNC( ELG_MODULE_INFO_UPDATE_NOT, KEGS_MODULE_INFO_UPDATE_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	//{{ 2012. 01. 27  김민성	해킹 프로세스 검사 기능
#ifdef SERV_HACKING_DLL_CHECK
	KELG_CHECK_MODULE_INFO_UPDATE_NOT kNot;
	kNot.m_iOK = NetError::NET_OK;

	SiKDLLManager()->UpdateModuleList( kPacket_.m_vecModuleName );

	if( SiKDLLManager()->CheckHackingModuleList( kPacket_.m_vecModuleName, kNot ) == false )
	{
		// 데이터가 비어 있으면 보내지 않는다.
		if( kNot.m_mapModuleName.empty() == true )
		{
			return;
		}

		// 블랙 리스트에 등록된 DLL 를 사용하는 유저이므로 강퇴 시키자!
		kNot.m_iOK = NetError::ERR_CHECK_DLL_01;
		SendToGSUser( FIRST_SENDER_UID, ELG_CHECK_MODULE_INFO_UPDATE_NOT, kNot );
	}
#else
	SiKDLLManager()->UpdateModuleList( kPacket_.m_vecModuleName );
#endif SERV_HACKING_DLL_CHECK
	//}}
}
#endif SERV_DLL_MANAGER
//}}

//{{ 2010. 12. 20	최육사	특정 시각 큐브 보상
#ifdef SERV_TIME_OPEN_RANDOM_ITEM_EVENT

IMPL_ON_FUNC( ELG_CHECK_TIME_RANDOM_ITEM_EVENT_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KELG_CHECK_TIME_RANDOM_ITEM_EVENT_ACK kPacket;
	kPacket.m_iItemUID = kPacket_.m_iItemUID;	
	//{{ 2011. 12.13    김민성	접속 이벤트 기능 추가 (반복 지급)
#ifdef SERV_RANDOM_CUBE_GOLD_REWARD
	SiKRandomItemEventManager()->CheckTimeOpenRandomItemEvent( kPacket_.m_iUserUID, kPacket_.m_iUnitUID, kPacket_.m_iRandomItemID, kPacket.m_iEventItemID );
#else
	SiKRandomItemEventManager()->CheckTimeOpenRandomItemEvent( kPacket_.m_iUserUID, kPacket_.m_iUnitUID, kPacket.m_iEventItemID );
#endif SERV_RANDOM_CUBE_GOLD_REWARD
	//}}
	SendToGSUser( FIRST_SENDER_UID, ELG_CHECK_TIME_RANDOM_ITEM_EVENT_ACK, kPacket );
}

#endif SERV_TIME_OPEN_RANDOM_ITEM_EVENT
//}}

//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
_IMPL_ON_FUNC( ELG_CHECK_MACHINE_ID_REQ, KEGS_CHECK_MACHINE_ID_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KEGS_CHECK_MACHINE_ID_ACK kPacketAck;
	kPacketAck.m_strMachineID = kPacket_.m_strMachineID;

	if( SiKAccountBlockManager()->IsBlockedMachineID( kPacket_.m_strMachineID ) == true )
	{
		kPacketAck.m_iOK = NetError::ERR_CHECK_MACHINE_ID_00;
		SendToGSUser( FIRST_SENDER_UID, ELG_CHECK_MACHINE_ID_ACK, kPacketAck );
		return;
	}
    
	kPacketAck.m_iOK = NetError::NET_OK;
	SendToGSUser( FIRST_SENDER_UID, ELG_CHECK_MACHINE_ID_ACK, kPacketAck );
}
#endif SERV_MACHINE_ID_BLOCK
//}}

//{{ 2011. 08. 11	최육사	투니 랜드 채널링 
#ifdef SERV_TOONILAND_CHANNELING
IMPL_ON_FUNC_NOPARAM( ELG_DUMP_LOGIN_USER_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	START_LOG( cout, L"-------- Login User Dump ---------" )
		<< BUILD_LOG( GetUID() )
		<< BUILD_LOG( m_kLoginTickDump.m_dwLoginAuthCount )
		<< BUILD_LOG( m_kLoginTickDump.m_dwMaxTick )
		<< BUILD_LOG( GetLoginMaxTickRegDate() );
}
#endif SERV_TOONILAND_CHANNELING
//}}

//{{ 2011. 12. 23	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
IMPL_ON_FUNC( ELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	bool bFindSuccess = false;
	KUserList::KGSUserInfo kUserInfo;

	// UnitUID값으로 먼저 현재 접속중인지 찾아보자
	if( kPacket_.m_iReceiverUnitUID != 0 )
	{
		bFindSuccess = GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUnitUID, kUserInfo );
	}

	// UnitUID로도 찾지 못했다면 닉네임으로 찾아보자!
	if( kPacket_.m_wstrReceiverNickName.empty() == false  &&  bFindSuccess == false )
	{
		bFindSuccess = GetKLoginSimLayer()->GetGSUserInfoByUnitNickName( kPacket_.m_wstrReceiverNickName, kUserInfo );
	}

	if( bFindSuccess == false )
	{
		if( kPacket_.m_wstrReceiverNickName.empty() == false )
		{
			// 현재 로그인 서버에 접속중인 상태가 아니다! DB로 가서 실제 존재하는 닉네임인지 확인하자!
			SendToGameDB( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, kPacket_ );
		}
		else
		{
			START_LOG( cerr, L"클라이언트에서 반드시 닉네임 정보를 보냈을텐데 닉네임이 비어서 왔네.." )
				<< BUILD_LOG( kPacket_.m_iHostUnitUID )
				<< BUILD_LOG( kPacket_.m_wstrHostNickName )
				<< BUILD_LOG( kPacket_.m_iReceiverUnitUID )
				<< BUILD_LOG( kPacket_.m_wstrReceiverNickName )
				<< END_LOG;

			KELG_INVITE_PARTY_FIND_RECEIVER_ACK kPacketAck;
			kPacketAck.m_iOK = NetError::ERR_PARTY_07;
			kPacketAck.m_iHostUnitUID = kPacket_.m_iHostUnitUID;
			kPacketAck.m_kCreatePartyInfo = kPacket_.m_kCreatePartyInfo;
			SendToGSUser( FIRST_SENDER_UID, ELG_INVITE_PARTY_FIND_RECEIVER_ACK, kPacketAck );
		}

		return;
	}

	// 찾는 캐릭이 있으면 해당 GS로 정보요청
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUnitUID, kUserInfo ) == false )
	{
		START_LOG( cerr, L"파티 초대자의 정보를 찾지 못하였습니다!" )
			<< BUILD_LOG( kPacket_.m_iHostUnitUID )
			<< END_LOG;
		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INVITE_PARTY_FIND_RECEIVER_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_INVITE_PARTY_GET_RECEIVER_USER_INFO_ACK, KELG_INVITE_PARTY_FIND_RECEIVER_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iHostUnitUID, kUserInfo ) == false )
	{
		START_LOG( cerr, L"파티 초대자의 정보를 찾지 못하였습니다!" )
			<< BUILD_LOG( kPacket_.m_iHostUnitUID )
			<< END_LOG;
		return;
	}

	// 찾는 캐릭이 있으면 해당 GS로 정보요청
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_INVITE_PARTY_FIND_RECEIVER_ACK, kPacket_ );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 버프 관리 시스템 - 김민성
#ifdef SERV_SERVER_BUFF_SYSTEM
IMPL_ON_FUNC( ELG_UPDATE_USER_UNIT_BUFF_INFO_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	if( kPacket_.m_vecDeActivateBuff.empty() == false )
	{
		if( SiKBuffManager()->DeleteUnitBuffInfo( kPacket_.m_iUnitUID, kPacket_.m_vecDeActivateBuff ) == false )
		{
			START_LOG( clog, L"버프 매니저에 유닛의 버프 정보를 삭제 하지 못했습니다." )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;
		}
	}

	if( kPacket_.m_vecActivateBuff.empty() == false )
	{
		if( SiKBuffManager()->AddUnitBuffInfo( kPacket_.m_iUnitUID, kPacket_.m_vecActivateBuff ) == false )
		{
			START_LOG( clog, L"버프 매니저에 유닛의 버프 정보를 추가 하지 못했습니다." )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< END_LOG;
		}
	}
	
	KELG_UPDATE_USER_UNIT_BUFF_INFO_ACK kPacket;

	if( SiKBuffManager()->GetUnitAllBuffInfo( kPacket_.m_iUnitUID, kPacket.m_kUnitBuffInfo ) == false )
	{
		START_LOG( clog, L"버프 매니저에 유닛의 버프 정보를 가져오지 못했습니다." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;
	}

	// 확인 로그
	if( kPacket.m_kUnitBuffInfo.m_mapBuffInfo.size() > 100 )
	{
		START_LOG( cerr, L"헐! 하나의 캐릭터에 걸린 버프 갯수가 100개를 넘었다!!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket.m_kUnitBuffInfo.m_mapBuffInfo.size() )
			<< END_LOG;

		// 로그만 찍자
	}

	if( kPacket_.m_bSendAck )
	{
		SendToGSUser( FIRST_SENDER_UID, ELG_UPDATE_USER_UNIT_BUFF_INFO_ACK, kPacket );
	}
}
#endif SERV_SERVER_BUFF_SYSTEM
//}

#ifdef SERV_DELETE_GUILD_ADD_CHEAT
IMPL_ON_FUNC( ELG_ADMIN_DELETE_GUILD_ADD_NOT )
{
	if( SiKGuildBoardManager()->IsExistGuildAd( kPacket_.m_iGuildUID ) == true )
	{
		KDBE_DELETE_GUILD_AD_LIST_NOT kPacketNot;

		// 길드 가입 신청 리스트 삭제
		SiKGuildBoardManager()->DeleteApplyJoinGuildList( kPacket_.m_iGuildUID, kPacketNot.m_vecDeleteApply );

		// 게시판에서 해당 길드 광고 삭제
		SiKGuildBoardManager()->DeleteGuildAd( kPacket_.m_iGuildUID );

		kPacketNot.m_vecDeleteGuildAd.push_back( kPacket_.m_iGuildUID );
		SendToGameDB( DBE_DELETE_GUILD_AD_LIST_NOT, kPacketNot );

		START_LOG( cout, L"길드 광고 삭제 성공!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID );
	}
	else
	{
		START_LOG( cout, L"길드 광고 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_iGuildUID );
	}
}
#endif SERV_DELETE_GUILD_ADD_CHEAT

//{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
IMPL_ON_FUNC( ELG_ADMIN_DELETE_HENIR_RANKING_RANK_NOT )
{
	if( kPacket_.m_iRankingType < 0  ||  kPacket_.m_iRankingType > SEnum::RT_MONTH_RANKING )
	{
		START_LOG( cerr, L"RankingType이 이상합니다!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< END_LOG;
		return;
	}

	if( SiKRankingManager()->DeleteSearchRank( kPacket_.m_iRankingType, kPacket_.m_iRank ) )
	{
		START_LOG( cout, L"랭킹 기록 삭제 성공!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_iRank );
	}
	else
	{
		START_LOG( cout, L"랭킹 기록 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_iRank );
	}
}

IMPL_ON_FUNC( ELG_ADMIN_DELETE_HENIR_RANKING_NICKNAME_NOT )
{
	if( kPacket_.m_iRankingType < 0  ||  kPacket_.m_iRankingType > SEnum::RT_MONTH_RANKING )
	{
		START_LOG( cerr, L"RankingType이 이상합니다!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< END_LOG;
		return;
	}

	if( SiKRankingManager()->DeleteRecord( kPacket_.m_iRankingType, kPacket_.m_wstrNickName ) )
	{
		START_LOG( cout, L"랭킹 기록 삭제 성공!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_wstrNickName );
	}
	else
	{
		START_LOG( cout, L"랭킹 기록 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_wstrNickName );
	}
}

IMPL_ON_FUNC( ELG_ADMIN_DELETE_HENIR_RANKING_UNITUID_NOT )
{
	if( kPacket_.m_iRankingType < 0  ||  kPacket_.m_iRankingType > SEnum::RT_MONTH_RANKING )
	{
		START_LOG( cerr, L"RankingType이 이상합니다!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< END_LOG;
		return;
	}

	if( SiKRankingManager()->DeleteRecord( kPacket_.m_iRankingType, kPacket_.m_iUnitUID ) )
	{
		START_LOG( cout, L"랭킹 기록 삭제 성공!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_iUnitUID );
	}
	else
	{
		START_LOG( cout, L"랭킹 기록 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_iRankingType )
			<< BUILD_LOG( kPacket_.m_iUnitUID );
	}
}
#endif SERV_DELETE_HENIR_RANKING
//}}

IMPL_ON_FUNC_NOPARAM( E_RESERVE_DESTROY )
{
    m_bDestroyReserved = true;
}

#ifdef SERV_CHECK_PCBANG_BY_PUBLISHER
IMPL_ON_FUNC( ELG_PCBANG_LOGOUT_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	GetKLoginSimLayer()->LogoutPcBang( kPacket_ );
}
#endif //SERV_CHECK_PCBANG_BY_PUBLISHER

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
_IMPL_ON_FUNC( EGS_LOCAL_RANKING_INQUIRY_REQ, KLocalRankingInquiryData )
{
	KEGS_LOCAL_RANKING_INQUIRY_ACK kPacket;
	kPacket.m_nViewPage		= kPacket_.m_nViewPage;
	kPacket.m_cMainTabIndex	= kPacket_.m_cMainTabIndex;
	kPacket.m_cSubTabIndex	= kPacket_.m_cSubTabIndex;
	kPacket.m_byteFilter	= kPacket_.m_byteFilter;
	kPacket.m_kMyRankInfo	= kPacket_.m_kMyRankInfo;

	KGuildPtr spGuild = SiKGuildManager()->GetGuild( kPacket_.m_iGuildUID );
	if( spGuild == NULL )
	{
		kPacket.m_iOK = NetError::ERR_GUILD_06;
		SendToGSUser( FIRST_SENDER_UID, EGS_LOCAL_RANKING_INQUIRY_ACK, kPacket );
		return;
	}

	spGuild->GetRankerList( kPacket.m_nViewPage
		, kPacket.m_cMainTabIndex
		, kPacket.m_byteFilter
		, kPacket_.m_setFriendList
		, kPacket.m_nTotalPage
		, kPacket.m_vecRankerList
		, kPacket.m_kPrevRankerInfo
		, kPacket.m_kMyRankInfo
		);

	kPacket.m_iOK = NetError::NET_OK;
	SendToGSUser( FIRST_SENDER_UID, EGS_LOCAL_RANKING_INQUIRY_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_GUILD_CHANGE_INFO_NOT )
{
	KGuildPtr spGuild = SiKGuildManager()->GetGuild( kPacket_.m_iUID );
	if( spGuild == NULL )
	{
		return;
	}

	std::vector<KELG_LOCAL_RANKING_RANK_UP_NOT>	vecRankUpNot;

	if( spGuild->SomethingChanged( kPacket_.m_kUserInfo, kPacket_.m_kUnitInfo, vecRankUpNot ) == false )
	{
		return;
	}

	if( vecRankUpNot.empty() == false )
	{
		BOOST_TEST_FOREACH( const KELG_LOCAL_RANKING_RANK_UP_NOT&, kPacket, vecRankUpNot )
		{
			SendToGSUser( FIRST_SENDER_UID, ELG_LOCAL_RANKING_RANK_UP_NOT, kPacket );
		}
	}
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	KUserList::KGSUserInfo kUserInfo;

	// 접속 중인 유저라면 직접 얻으로 가자
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iTargetUnitUID, kUserInfo ) == true )
	{
		SendToGSCharacter( kUserInfo.m_nGSUID, kUserInfo.m_nUnitUID, ELG_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket_ );
		return;
	}

	KELG_LOCAL_RANKING_WATCH_UNIT_ACK kPacket;

	// 보관 중인 정보가 있다면, 꺼내서 주자
	if( GetKLoginSimLayer()->GetLocalRankingUserUnitInfo( kPacket_.m_iTargetUnitUID, kPacket.m_kUserInfo, kPacket.m_kUnitInfo ) == true )
	{
		kPacket.m_iOK = NetError::NET_OK;
		SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ELG_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
		return;
	}

	// DB에서 꺼내오자
	SendToGameDB( DBE_GAME_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket_ );
}

IMPL_ON_FUNC( ELG_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ELG_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_ACK )
{
	// 비공개 정보 처리
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		if( SEnum::CheckFlag( kPacket_.m_kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_CATEGORY ) == true )
		{
			kPacket_.m_kUserInfo.m_iCategory = SEnum::LRLC_NONE;
		}

		if( SEnum::CheckFlag( kPacket_.m_kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_BIRTH ) == true )
		{
			kPacket_.m_kUserInfo.m_iBirth = 0;
		}

		if( SEnum::CheckFlag( kPacket_.m_kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_PROFILE ) == true )
		{
			kPacket_.m_kUserInfo.m_wstrProfile.clear();
		}

		if( SEnum::CheckFlag( kPacket_.m_kUserInfo.m_byteFilter, SEnum::LRF_USER_INFO_GENDER ) == true )
		{
			kPacket_.m_kUserInfo.m_byteGender = 0;
		}

		// 보관
		GetKLoginSimLayer()->ReserveLocalRankingUserUnitInfo( kPacket_.m_kUserInfo, kPacket_.m_kUnitInfo );
	}

	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ELG_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket_ );
}
#endif	// SERV_LOCAL_RANKING_SYSTEM

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_REQ )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iOtherUnitUID, kUserInfo ) == false )
	{
		KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_08;
		kAck.m_wstrNickName = kPacket_.m_wstrAcceptUnitName;
		SendToGSUser( FIRST_SENDER_UID, ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK, kAck );
		return;
	}

	// 신청자에게 결과 회신
	KELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;
	kAck.m_wstrNickName = kPacket_.m_wstrAcceptUnitName;
	SendToGSUser( FIRST_SENDER_UID, ELG_COUPLE_PROPOSE_CHECK_CONNECT_USER_ACK, kAck );

	// 커플 신청 알림
	KELG_COUPLE_PROPOSE_NOT kPacket;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_COUPLE_PROPOSE_NOT, kPacket );
}

_IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, KERM_COUPLE_PROPOSE_RESULT_SUCCESS_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iAcceptUnitUID, kUserInfo ) == false )
	{
		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_COUPLE_PROPOSE_RESULT_SUCCESS_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_AGREE_CHECK_NOT, KEGS_COUPLE_PROPOSE_AGREE_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iRequestUnitUID, kUserInfo ) == false )
	{
		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_COUPLE_PROPOSE_AGREE_CHECK_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_COUPLE_PROPOSE_RESULT_FAIL_NOT, KERM_COUPLE_PROPOSE_RESULT_FAIL_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iAcceptUnitUID, kUserInfo ) == false )
	{
		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ERM_COUPLE_PROPOSE_RESULT_FAIL_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_RELATIONSHIP_INFO_REQ )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iAcceptUnitUID, kUserInfo ) == false )
	{
		// 접속 중이 아니므로 GameDB 에서 정보를 가져 오자
		SendToGameDB( DBE_RELATIONSHIP_INFO_GAME_DB_REQ, kPacket_ );
	}
	else
	{
		// 접속 중이면 해당 유저의 게임 서버로 가서 정보를 가져 오자
		SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_RELATIONSHIP_INFO_GAME_SERVER_REQ, kPacket_ );
	}
}

_IMPL_ON_FUNC( DBE_RELATIONSHIP_INFO_GAME_DB_ACK, KEGS_RELATIONSHIP_INFO_ACK )
{
	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ELG_RELATIONSHIP_INFO_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_RELATIONSHIP_INFO_GAME_SERVER_ACK, KEGS_RELATIONSHIP_INFO_ACK )
{
	SendToGSCharacter( kPacket_.m_iRequestGSUID, kPacket_.m_iRequestUnitUID, ELG_RELATIONSHIP_INFO_ACK, kPacket_ );
}

IMPL_ON_FUNC( ELG_WEDDING_ITEM_FIND_INFO_REQ )
{
	if( SiKWeddingHallManager()->GetWeddingItemInfo( kPacket_.m_mapWeddingItemInfo ) == false )
	{
		KUserList::KGSUserInfo kInfo;
		if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kInfo ) == false )	
			return;

		kPacket_.m_iOK = NetError::ERR_RELATIONSHIP_25;
		SendToGSUser( kInfo.m_nGSUID, kInfo.m_nUserUID, ELG_WEDDING_ITEM_FIND_INFO_ACK, kPacket_ );
		return;
	}

	std::map< UidType, KWeddingItemInfo >::iterator mit = kPacket_.m_mapWeddingItemInfo.begin();
	for( ; mit != kPacket_.m_mapWeddingItemInfo.end() ; ++mit )
	{
		if( mit->second.m_iGroom > 0 )
		{
			KUserList::KGSUserInfo kGroomInfo;
			if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( mit->second.m_iGroom, kGroomInfo ) == true )
			{
				mit->second.m_wstrGroom = kGroomInfo.m_wstrUnitNickName;
			}
		}

		if( mit->second.m_iBride > 0 )
		{
			KUserList::KGSUserInfo kBrideInfo;
			if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( mit->second.m_iBride, kBrideInfo ) == true )
			{
				mit->second.m_wstrBride = kBrideInfo.m_wstrUnitNickName;
			}
		}
	}

	SendToGameDB( DBE_WEDDING_ITEM_FIND_INFO_REQ, kPacket_ );
}

_IMPL_ON_FUNC( DBE_WEDDING_ITEM_FIND_INFO_ACK, KELG_WEDDING_ITEM_FIND_INFO_REQ )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iUnitUID, kUserInfo ) == false )
		return;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_WEDDING_ITEM_FIND_INFO_ACK, kPacket_ );
}

IMPL_ON_FUNC( ELG_ADD_WEDDING_HALL_INFO_NOT )
{
	if( SiKWeddingHallManager()->AddWeddingInfo( kPacket_.m_kWeddingHallInfo ) == false )
	{
		START_LOG( cerr, L"결혼식 정보 입력 실패!" )
			<< BUILD_LOG( kPacket_.m_kWeddingHallInfo.m_iWeddingUID )
			<< BUILD_LOGc( kPacket_.m_kWeddingHallInfo.m_cWeddingHallType )
			<< BUILD_LOGc( kPacket_.m_kWeddingHallInfo.m_cOfficiantNPC )
			<< BUILD_LOG( kPacket_.m_kWeddingHallInfo.m_iGroom )
			<< BUILD_LOG( kPacket_.m_kWeddingHallInfo.m_iBride )
			<< BUILD_LOG( kPacket_.m_kWeddingHallInfo.m_wstrWeddingDate )
			<< END_LOG;
	}

	KDBE_LOAD_WEDDING_HALL_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_mapWeddingInfo.insert( std::make_pair( kPacket_.m_kWeddingHallInfo.m_iWeddingUID, kPacket_.m_kWeddingHallInfo ) );
	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData( PI_GS_USER, anTrace, ELG_ADD_WEDDING_HALL_INFO_NOT, kPacket );
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_REQ )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iLoverUnitUID, kUserInfo ) == false )
	{
		// 소환 하는 사람에게 실패 처리 알림
		KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_08;
		SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK, kPacket );
		return;
	}

	// 동일 채널이 아니라면 소환 막자
	if( kUserInfo.m_iChannelID != kPacket_.m_iChannelID )
	{
		KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_30;
		SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK, kPacket );
		return;
	}

	// 소환 될 사람에게 의견 물으러 가자
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_CALL_MY_LOVER_VILLAGE_NOT, kPacket_ );

	// 소환한 사람에게 알려주기
	KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT )
{
	// 소환한 사람에게 결과 전달
	SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_AGREE_RESULT_VILLAGE_NOT, kPacket_ );

	if( kPacket_.m_bAgree == true )
	{
		// 소환 될 사람에게 마을 이동 패킷 전달
		KELG_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT kReq;
		kReq.m_bUseWarpItem = false;
		kReq.m_iMapID = kPacket_.m_iMapID;
		kReq.m_vPos = kPacket_.m_vPos;
		kReq.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
		SendToGSUser( kPacket_.m_iMyGSUID, kPacket_.m_iMyUserUID, ELG_CALL_MY_LOVER_STATE_CHANGE_FIELD_NOT, kReq );
	}
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_FIND_USER_ROOM_REQ )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iLoverUnitUID, kUserInfo ) == false )
	{
		// 소환 하는 사람에게 실패 처리 알림
		KELG_CALL_MY_LOVER_FIND_USER_ROOM_ACK kPacket;
		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_08;
		SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_FIND_USER_ROOM_ACK, kPacket );
		return;
	}

	// 동일 채널이라면 소환 하도록 하자 - 추후 생각하자
	// ?????

	// 소환 될 사람에게 의견 물으러 가자
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_CALL_MY_LOVER_ROOM_NOT, kPacket_ );

	// 소환한 사람에게 알려주기
	KELG_CALL_MY_LOVER_FIND_USER_VILLAGE_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_FIND_USER_ROOM_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUserUID( kPacket_.m_iCalledUserUID, kUserInfo ) == false )
	{
		SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT, kPacket_ );
		return;
	}

	// 소환자 에게
	SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_AGREE_RESULT_ROOM_NOT, kPacket_ );

	// 소환 될 사람에게 마을 이동 패킷 전달
	if( kPacket_.m_bAgree == true )
	{
		KELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT kPacket;
		kPacket.m_iCalledUserUID = kPacket_.m_iCalledUserUID;
		kPacket.m_iCalledUnitUID = kUserInfo.m_nUnitUID;
		kPacket.m_iCalledGSUID = kPacket_.m_iCalledGSUID;
		kPacket.m_iBattleFieldID = kPacket_.m_iBattleFieldID;
		kPacket.m_RoomUID = kPacket_.m_RoomUID;
		kPacket.m_vPos = kPacket_.m_vPos;
		kPacket.m_LastTouchLineIndex = kPacket_.m_LastTouchLineIndex;
		SendToGSUser( kPacket_.m_iMyGSUID, kPacket_.m_iMyUserUID, ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_NOT, kPacket );
	}
}

IMPL_ON_FUNC( ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT )
{
	SendToGSUser( kPacket_.m_iCalledGSUID, kPacket_.m_iCalledUserUID, ELG_CALL_MY_LOVER_JOIN_BATTLE_FIELD_FAILD_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_CHECK_WEDDING_HALL_INFO_REQ )
{
	KELG_CHECK_WEDDING_HALL_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_cUnitWeddingStatus = kPacket_.m_cUnitWeddingStatus;

	if( SiKWeddingHallManager()->GetWeddingInfo( kPacket_.m_iWeddingUID, kPacket.m_kWeddingInfo ) == false )
	{
		START_LOG( cerr, L"존재하지 않는 결혼식장 UID 입니다." )
			<< BUILD_LOG( kPacket_.m_iWeddingUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_RELATIONSHIP_25;
	}

	SendToGSUser( kPacket_.m_iGSUID, kPacket_.m_iGSUserUID, ELG_CHECK_WEDDING_HALL_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_BREAK_UP_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( !GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iLoverUnitUID, kUserInfo ) )
		return;

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_BREAK_UP_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_CHANGE_LOVE_WORD_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iLoverUnitUID, kUserInfo ) == false )
	{
		// 접속하지 않은 상태 이므로 패스
		return;
	}

	// 소환자 에게
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_CHANGE_LOVE_WORD_NOT, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_RELATIONSHIP_EVENT_INT
IMPL_ON_FUNC( ELG_EVENT_PROPOSE_CHECK_CONNECT_USER_REQ )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iOtherUnitUID, kUserInfo ) == false )
	{
		KELG_EVENT_PROPOSE_CHECK_CONNECT_USER_ACK kAck;
		kAck.m_iOK = NetError::ERR_RELATIONSHIP_08;
		kAck.m_wstrNickName = kPacket_.m_wstrAcceptUnitName;
		SendToGSUser( FIRST_SENDER_UID, ELG_EVENT_PROPOSE_CHECK_CONNECT_USER_ACK, kAck );
		return;
	}

	// 신청자에게 결과 회신
	KELG_EVENT_PROPOSE_CHECK_CONNECT_USER_ACK kAck;
	kAck.m_iOK = NetError::NET_OK;
	kAck.m_wstrNickName = kPacket_.m_wstrAcceptUnitName;
	SendToGSUser( FIRST_SENDER_UID, ELG_EVENT_PROPOSE_CHECK_CONNECT_USER_ACK, kAck );

	// 가상 결혼 프로포즈
	KELG_EVENT_PROPOSE_NOT kPacket;
	kPacket.m_wstrRequestUnitName = kPacket_.m_wstrRequestUnitName;
	kPacket.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_EVENT_PROPOSE_NOT, kPacket );
}

_IMPL_ON_FUNC( ELG_EVENT_PROPOSE_AGREE_CHECK_NOT, KEGS_EVENT_PROPOSE_AGREE_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iRequestUnitUID, kUserInfo ) == false )
	{
		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_EVENT_PROPOSE_AGREE_CHECK_NOT, kPacket_ );
}

_IMPL_ON_FUNC( ELG_EVENT_PROPOSE_RESULT_SUCCESS_NOT, KERM_EVENT_PROPOSE_RESULT_SUCCESS_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iAcceptUnitUID, kUserInfo ) == false )
	{
		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_EVENT_PROPOSE_RESULT_SUCCESS_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_DIVORCE_NOT, KELG_DIVORCE_NOT )
{
	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iLoverUnitUID, kUserInfo ) == false )
	{
		return;
	}

	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_DIVORCE_NOT, kPacket_ );
}
#endif SERV_RELATIONSHIP_EVENT_INT

#ifdef SERV_COUNTRY_PH
_IMPL_ON_FUNC( ELG_GN_CHANGE_GAME_CURRENCY_NOT, KEBILL_GN_CHANGE_GAME_CURRENCY_REQ )
{
	if (kPacket_.m_iResult == 0)
	{
		SendToKOGBillingDB(EBILL_GN_CHANGE_GAME_CURRENCY_REQ, kPacket_);
	}
	else
	{
		KEJSON_GN_CHANGE_GAME_CURRENCY_ACK kPacketACK;
		kPacketACK.m_iResult = 1;
		kPacketACK.m_uiGarenaUID = kPacket_.m_kGarenaREQ.m_uiGarenaUID;
		kPacketACK.m_int64GameTransactionID = 0LL;
		kPacketACK.m_strGarenaTransactionID = "";
		kPacketACK.m_uiGameCurrency_Amount = 0;
		kPacketACK.m_uiGameCurrency_Balance = 0;

		CTime tCurrent  = CTime::GetCurrentTime();
		kPacketACK.m_strDateTime = (CStringA)tCurrent.Format(_T("%Y-%m-%d %H:%M:%S"));
		kPacketACK.m_uiShell_Amount = kPacket_.m_kGarenaREQ.m_uiShell_Amount;
		kPacketACK.m_uiGameCurrency_BeforeAmount = kPacket_.m_kGarenaREQ.m_uiGameCurrency_Amount;

		kPacketACK.m_iSessionUID = kPacket_.m_kGarenaREQ.m_iSessionUID;

		SendToKOGBillingDB(EBILL_GN_CHANGE_GAME_CURRENCY_FAIL_NOT, kPacketACK);
		

		START_LOG( cerr, L"Garena에서 캐쉬 전환 요청 들어 왔는데 우리한테 없는 유저다!!" )
			<< BUILD_LOG( kPacket_.m_kGarenaREQ.m_uiGarenaUID)
			<< END_LOG;

		KEventPtr spEvent( new KEvent );
		UidType anTrace[2] = { GetUID(), -1 };
		spEvent->SetData(PI_LOGIN_GARENA_BILLING, anTrace, EJSON_GN_CHANGE_GAME_CURRENCY_ACK, kPacketACK );

		SiKGarenaBillingServer()->QueueingEvent( spEvent );
	}
}
#endif //SERV_COUNTRY_PH

#ifdef SERV_RECRUIT_EVENT_BASE
_IMPL_ON_FUNC( ELG_GET_RECRUIT_RECRUITER_LIST_REQ, KEGS_GET_RECRUIT_RECRUITER_LIST_ACK )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	// 현재 접속중인 추천인인지 체크!
	std::vector< KRecommendUserInfo >::iterator vit;
	for( vit = kPacket_.m_vecRecruitUnitInfo.begin(); vit != kPacket_.m_vecRecruitUnitInfo.end(); ++vit )
	{
		if( GetKLoginSimLayer()->IsLoginedUnitUID( vit->m_iUnitUID ) == true )
		{
			// 온라인 상태
			vit->m_bIsOnline = true;

			START_LOG( clog, L"접속해 있는 유저이므로 상태를 갱신합니다." )
				<< BUILD_LOG( vit->m_iUnitUID )
				<< BUILD_LOG( vit->m_wstrNickName )
				<< BUILD_LOG( vit->m_ucLevel )
				<< BUILD_LOG( vit->m_cUnitClass )
				<< BUILD_LOG( vit->m_bIsOnline )
				<< END_LOG;
		}
		else
		{
			// 오프라인 상태
			vit->m_bIsOnline = false;

			START_LOG( clog, L"접속하고 있지 않은 유저이므로 상태를 갱신합니다." )
				<< BUILD_LOG( vit->m_iUnitUID )
				<< BUILD_LOG( vit->m_wstrNickName )
				<< BUILD_LOG( vit->m_ucLevel )
				<< BUILD_LOG( vit->m_cUnitClass )
				<< BUILD_LOG( vit->m_bIsOnline )
				<< END_LOG;
		}
	}
	for( vit = kPacket_.m_vecRecruiterUnitInfo.begin(); vit != kPacket_.m_vecRecruiterUnitInfo.end(); ++vit )
	{
		if( GetKLoginSimLayer()->IsLoginedUnitUID( vit->m_iUnitUID ) == true )
		{
			// 온라인 상태
			vit->m_bIsOnline = true;

			START_LOG( clog, L"접속해 있는 유저이므로 상태를 갱신합니다." )
				<< BUILD_LOG( vit->m_iUnitUID )
				<< BUILD_LOG( vit->m_wstrNickName )
				<< BUILD_LOG( vit->m_ucLevel )
				<< BUILD_LOG( vit->m_cUnitClass )
				<< BUILD_LOG( vit->m_bIsOnline )
				<< END_LOG;
		}
		else
		{
			// 오프라인 상태
			vit->m_bIsOnline = false;

			START_LOG( clog, L"접속하고 있지 않은 유저이므로 상태를 갱신합니다." )
				<< BUILD_LOG( vit->m_iUnitUID )
				<< BUILD_LOG( vit->m_wstrNickName )
				<< BUILD_LOG( vit->m_ucLevel )
				<< BUILD_LOG( vit->m_cUnitClass )
				<< BUILD_LOG( vit->m_bIsOnline )
				<< END_LOG;
		}
	}

	SendToGSUser( FIRST_SENDER_UID, ELG_GET_RECRUIT_RECRUITER_LIST_ACK, kPacket_ );
}

_IMPL_ON_FUNC( ELG_REGISTER_RECRUITER_NOT, KEGS_REGISTER_RECRUITER_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iRecruiterUnitUID, kUserInfo ) == false )
	{
		// 현재 접속하지 않은 유저라면 여기서 끝~!
		return;
	}

	//찾는 캐릭이 있으면 해당 캐릭터의 UID 전송	
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_REGISTER_RECRUITER_NOT, kPacket_ );
}

IMPL_ON_FUNC( ELG_RECRUIT_RECRUITER_INFO_NOT )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	KUserList::KGSUserInfo kUserInfo;
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iReceiverUnitUID, kUserInfo ) == false )
	{
		// 현재 접속하지 않은 유저라면 여기서 끝~!
		return;
	}

	//찾는 캐릭이 있으면 해당 캐릭터의 UID 전송	
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_RECRUIT_RECRUITER_INFO_NOT, kPacket_ );

	// 역전송
	if( GetKLoginSimLayer()->GetGSUserInfoByUnitUID( kPacket_.m_iSenderUnitUID, kUserInfo ) == false )
		return;

	KELG_RECRUIT_RECRUITER_INFO_NOT kReverse = kPacket_;
	kReverse.m_iSenderUnitUID = kPacket_.m_iReceiverUnitUID;
	kReverse.m_iReceiverUnitUID = kPacket_.m_iSenderUnitUID;
	kReverse.m_cType = KRecommendUserInfo::RUIT_LOGIN;
	SendToGSUser( kUserInfo.m_nGSUID, kUserInfo.m_nUserUID, ELG_RECRUIT_RECRUITER_INFO_NOT, kReverse );
}
#endif SERV_RECRUIT_EVENT_BASE


#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
IMPL_ON_FUNC( ELG_CASH_DIRECT_CHARGE_CN_REQ )
{
	VERIFY_STATE( ( 1, KDefaultFSM::S_LOGINED ) );

	START_LOG( cout, L"[TEST] Direct Charge 주소 요청 Login User 에 도착" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
		<< BUILD_LOG( kPacket_.m_wstrIP )
		<< END_LOG;

	if( false == GetKLoginSimLayer()->IsEnableCNDirectCharge() )
	{
		START_LOG( cout, L"[CN] Direct Charge 꺼져 있어서 중국 인증서버에 패킷 보내지 않는다." )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wstrServiceAccountID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrUnitNickName )
			<< BUILD_LOG( kPacket_.m_wstrIP )
			<< END_LOG;

		KEGS_CASH_DIRECT_CHARGE_CN_ACK kPacketAck;
		kPacketAck.m_iOK = NetError::ERR_UNKNOWN;
		SendToGSUser(FIRST_SENDER_UID, ELG_CASH_DIRECT_CHARGE_CN_ACK, kPacketAck);

		return;
	}

	// 필요한 정보 추가로 담고 중국 인증 서버로 패킷 보낸다
	KEGIANT_AUTH_DIRECT_CHARGE_REQ kPacket;
	kPacket.m_wstrServiceAccountID = GetName();
	kPacket.m_uiUserUID = GetUID();
	kPacket.m_ulGameZone = ((SiKGiantAuthManager()->GetServerInfo().m_usGame * 65536) + SiKGiantAuthManager()->GetServerInfo().m_usZone);
	kPacket.m_uiUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_wstrUnitNickName = kPacket_.m_wstrUnitNickName;
	kPacket.m_wstrIP = GetIP();
	
	KEventPtr spEvent( new KEvent );
	UidType anTrace[2] = { GetUID(), -1 };
	spEvent->SetData(PI_NULL, anTrace, EGIANT_AUTH_DIRECT_CHARGE_REQ, kPacket );
	SiKGiantAuthManager()->QueueingEvent( spEvent );
}
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH