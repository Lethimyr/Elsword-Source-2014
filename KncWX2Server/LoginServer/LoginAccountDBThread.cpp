#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "LoginAccountDBThread.h"
#include "LoginServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "SimLayer.h"
#ifdef SERV_ACCOUNT_BLOCK
	#include "AccountBlockManager.h"
#endif SERV_ACCOUNT_BLOCK

#ifdef SERV_KOG_OTP_VERIFY
	#include <boost/random.hpp>
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_COUNTRY_PH
#include "../Common/OnlyGlobal/AuthAndBilling/PH/GarenaBillingServer.h"
#endif //SERV_COUNTRY_PH

#define CLASS_TYPE KLoginAccountDBThread
ImplPfID( KLoginAccountDBThread, PI_LOGIN_ACCOUNT_DB );

IMPL_PROFILER_DUMP( KLoginAccountDBThread )
{
	for( unsigned int ui = 0; ui < vecDump.size(); ++ui )
	{
		unsigned int iAvg = 0;
		if( vecDump[ui].m_iQueryCount > 0 )	iAvg = vecDump[ui].m_iTotalTime / vecDump[ui].m_iQueryCount;		
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.P_QueryStats_INS", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_PROFILE( L"exec dbo.mup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
			% vecDump[ui].m_wstrQuery
			% vecDump[ui].m_iMinTime
			% iAvg
			% vecDump[ui].m_iMaxTime
			% vecDump[ui].m_iOver1Sec
			% vecDump[ui].m_iQueryCount
			% vecDump[ui].m_iQueryFail
			);
#endif //SERV_ALL_RENEWAL_SP
		continue;

end_proc:
		START_LOG( cerr, vecDump[ui].m_wstrQuery )
			<< BUILD_LOG( vecDump[ui].m_iMinTime )
			<< BUILD_LOG( vecDump[ui].m_iMaxTime )
			<< BUILD_LOG( vecDump[ui].m_iTotalTime )
			<< BUILD_LOG( vecDump[ui].m_iQueryCount )
			<< BUILD_LOG( iAvg )
			<< BUILD_LOG( vecDump[ui].m_iOver1Sec )
			<< BUILD_LOG( vecDump[ui].m_iQueryFail )
			<< END_LOG;
	}
}

KLoginAccountDBThread::~KLoginAccountDBThread(void)
{
}

void KLoginAccountDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
	   _CASE( DBE_NEW_USER_JOIN_REQ, KEGS_NEW_USER_JOIN_REQ );
        CASE( ELG_NEXON_USER_AUTHENTICATE_REQ );
		//{{ 2009. 5. 26  최육사	채널이동
		CASE( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ );
		//}}
		CASE( DBE_UPDATE_IS_LOGIN_NOT );
		//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK
		CASE( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ );
#endif SERV_ACCOUNT_BLOCK
		//}}
		//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK
	   _CASE( DBE_REG_REJECTED_USER_NOT, KELG_REG_REJECTED_USER_NOT );
#endif SERV_CN_SERVER_HACKING_CHECK
		//}}
	   //{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
	   _CASE( DBE_REG_TRADE_BLOCK_USER_NOT, KELG_REG_REJECTED_USER_NOT );
#endif SERV_HACK_USER_TRADE_BLOCK
	   //}}
	   //{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
	   CASE( DBE_GET_HACKING_MODULE_LIST_REQ );
#endif SERV_HACKING_DLL_CHECK
	   //}}
	   //{{ 2013. 3. 18	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	   CASE( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_REQ );
	   CASE( DBE_ACCOUNT_GET_GUILD_INFO_REQ );
	   CASE( DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
	   //}}
#ifdef SERV_COUNTRY_PH
	   CASE( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ );
	   CASE( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ );
#endif //SERV_COUNTRY_PH
	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KLoginAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

void KLoginAccountDBThread::SendToGSUser( UidType nGS, UidType nTo, unsigned short usEventID )
{
    SendToGSUser( nGS, nTo, usEventID, char() );
}

_IMPL_ON_FUNC( DBE_NEW_USER_JOIN_REQ, KEGS_NEW_USER_JOIN_REQ )
{
    KEGS_NEW_USER_JOIN_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
	DO_QUERY( L"exec dbo.P_MUser_INS_TW", L"N\'%s\', N\'%s\', N\'%s\', %d", % kPacket_.m_wstrID % kPacket_.m_wstrPassword % kPacket_.m_wstrName % kPacket_.m_byteGuestUser );
#else //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER
    DO_QUERY( L"exec dbo.mup_create_user", L"N\'%s\', N\'%s\', N\'%s\', %d", % kPacket_.m_wstrID % kPacket_.m_wstrPassword % kPacket_.m_wstrName % kPacket_.m_byteGuestUser );
#endif //SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_iOK
            >> kPacket.m_iUserUID );

        m_kODBC.EndFetch();
    }

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        // 사내 계정 생성 실패.
        START_LOG( clog, L"신규 유저 생성 실패." )
            << BUILD_LOG( kPacket.m_iOK )
            << BUILD_LOG( kPacket.m_iUserUID )
            << END_LOG;

        goto end_proc;
    }

end_proc:
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
        << BUILD_LOG( kPacket_.m_wstrID )
        << BUILD_LOG( kPacket_.m_wstrPassword )
        << BUILD_LOG( kPacket_.m_wstrName )
        << BUILD_LOG( kPacket.m_iOK )
        << BUILD_LOG( kPacket.m_iUserUID )
        << END_LOG;

    SendToGSUser( LAST_SENDER_UID, FIRST_SENDER_UID, DBE_NEW_USER_JOIN_ACK, kPacket );
}

IMPL_ON_FUNC( ELG_NEXON_USER_AUTHENTICATE_REQ )
{
    KELG_NEXON_USER_AUTHENTICATE_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_kAccountInfo.m_wstrID = kPacket_.m_kAuthenticateReq.m_wstrUserID;
    kPacket.m_kNexonAccountInfo = kPacket_.m_kNexonAccountInfo;
	kPacket.m_cHackingUserType = SEnum::HUT_NORMAL;
	//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
	kPacket.m_bJoinAccount = false;
#endif SERV_IP_ACCOUNT_LOG
	//}}
#ifdef SERV_EPAY_SYSTEM
	kPacket.m_usZone = kPacket_.m_usZone;
	kPacket.m_usGame = kPacket_.m_usGame;
#endif //SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
	kPacket.m_wstrSocketID = kPacket_.m_wstrSocketID;
	START_LOG( clog, L"LoginAccountDB 소켓 요청" )
		<< BUILD_LOG( kPacket_.m_wstrSocketID )
		<< END_LOG;
#endif // SERV_COUNTRY_TH

    // 이 쓰레드에서 sim layer 변수를 참조하는 것이 그닥 좋은 것은 아니다.
    DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();

    std::wstring wstrPassword;
    if( !kPacket_.m_kAuthenticateReq.m_bDebugAuth && dwAuthFlag == KSimLayer::AF_NEXON_KOREA )
    {
        wstrPassword = kPacket_.m_kNexonAccountInfo.m_wstrLoginIP;
    }
    else
    {
        wstrPassword = kPacket_.m_kAuthenticateReq.m_wstrPasswd;
    }

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-03	// 박세훈
	DO_QUERY( L"exec dbo.P_MUser_SEL_Auth", L"%d, N\'%s\', N\'%s\', %d, %d", 
		% kPacket_.m_kAuthenticateReq.m_bDebugAuth 
		% kPacket_.m_kAuthenticateReq.m_wstrUserID 
		% wstrPassword.c_str() 
		% kPacket_.m_kNexonAccountInfo.m_uiAge 
		% kPacket_.m_kNexonAccountInfo.m_uiNexonSN
		);

	if( m_kODBC.BeginFetch() )
	{
		byte byteBlockLevel = 0;

		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kAccountInfo.m_wstrName
			>> kPacket.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason			
			>> kPacket.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kAccountInfo.m_wstrRegDate
			>> kPacket.m_kAccountInfo.m_wstrLastLogin
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_byteBlockType
			>> byteBlockLevel
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason2
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockEndDate
			);

		m_kODBC.EndFetch();

		if( byteBlockLevel == 1 )
		{
			kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockEndDate.clear();
		}
	}
#else // SERV_HACKING_USER_CHECK_COUNT
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_SEL_Auth", L"%d, N\'%s\', N\'%s\', %d, %d", 
		% kPacket_.m_kAuthenticateReq.m_bDebugAuth 
		% kPacket_.m_kAuthenticateReq.m_wstrUserID 
		% wstrPassword.c_str() 
		% kPacket_.m_kNexonAccountInfo.m_uiAge 
		% kPacket_.m_kNexonAccountInfo.m_uiNexonSN );

	if( m_kODBC.BeginFetch() )
	{
		byte byteBlockType = 0;
		byte byteBlockLevel = 0;
		std::wstring wstrBlockReason2 = L"";
		std::wstring wstrBlockEndDate = L"";

		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kAccountInfo.m_wstrName
			>> kPacket.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason			
			>> kPacket.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kAccountInfo.m_wstrRegDate
			>> kPacket.m_kAccountInfo.m_wstrLastLogin
			>> byteBlockType//kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_byteBlockType
			>> byteBlockLevel
			>> wstrBlockReason2//kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason2
			>> wstrBlockEndDate//kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockEndDate
			>> kPacket.m_kNexonAccountInfo.m_uiNexonSN		//SERV_GLOBAL_AUTH //SERV_INT_ONLY
			);

		m_kODBC.EndFetch();
	}
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.mup_auth_user", L"%d, N\'%s\', N\'%s\', %d, %d", 
		% kPacket_.m_kAuthenticateReq.m_bDebugAuth 
		% kPacket_.m_kAuthenticateReq.m_wstrUserID 
		% wstrPassword.c_str() 
		% kPacket_.m_kNexonAccountInfo.m_uiAge 
		% kPacket_.m_kNexonAccountInfo.m_uiNexonSN );

    if( m_kODBC.BeginFetch() )
    {
		//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kAccountInfo.m_wstrName
			>> kPacket.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason			
			>> kPacket.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kAccountInfo.m_wstrRegDate
			>> kPacket.m_kAccountInfo.m_wstrLastLogin
			>> kPacket.m_kNexonAccountInfo.m_uiNexonSN		//SERV_GLOBAL_AUTH //SERV_INT_ONLY
			);
#else
		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
//#ifdef SERV_CASH_ITEM_LIST
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kAccountInfo.m_wstrName
			>> kPacket.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason			
			>> kPacket.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kAccountInfo.m_wstrRegDate
			>> kPacket.m_kNexonAccountInfo.m_uiNexonSN		//SERV_GLOBAL_AUTH //SERV_INT_ONLY
			);
//#endif SERV_CASH_ITEM_LIST
		//}}        
#endif SERV_SECOND_SECURITY
		//}}

        m_kODBC.EndFetch();
    }
#endif //SERV_ALL_RENEWAL_SP
#endif // SERV_HACKING_USER_CHECK_COUNT

    if( kPacket.m_iOK != NetError::NET_OK )
    {
        // 계정 DB 인증 실패
        START_LOG( cwarn, L"존재하지 않거나 삭제/비번 다른 계정." )
            << BUILD_LOG( kPacket.m_iOK )
            << END_LOG;

        switch( kPacket.m_iOK )
        {
        case -1:
            kPacket.m_iOK = NetError::ERR_VERIFY_04;
            break;
        case -2:            // 추후에 계정 삭제가 들어갈 경우 삭제된 유저가 접속했을 때의 처리가 필요하다.
            kPacket.m_iOK = NetError::ERR_VERIFY_05;
            break;
        case -3:
            kPacket.m_iOK = NetError::ERR_VERIFY_06;
            break;
        default:
			START_LOG( cerr, L"쿼리에 정의 되지 않은 오류 입니다!" )
				<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_bDebugAuth )
				<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrUserID )
				<< BUILD_LOG( wstrPassword )
				<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_uiAge )
				<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_uiNexonSN )
				<< END_LOG;

            kPacket.m_iOK = NetError::ERR_UNKNOWN;
            break;
        }

        // 신규 유저 생성. 사내 인증이 아닌 경우에 한다.
        if( kPacket.m_iOK == NetError::ERR_VERIFY_04 && !kPacket_.m_kAuthenticateReq.m_bDebugAuth )
        {
            std::wstring wstrSP;
            switch( dwAuthFlag )
            {
            case KSimLayer::AF_NEXON_KOREA:

#ifdef SERV_ALL_RENEWAL_SP
				{
					int iChannelCode = static_cast< int >(kPacket_.m_kNexonAccountInfo.m_uChannelCode);

					DO_QUERY( L"exec dbo.P_MUser_INS_Nexon", L"N\'%s\', N\'%s\', %d, %d, %d, N\'%s\', %d, %d",
						% kPacket_.m_kAuthenticateReq.m_wstrUserID
						% wstrPassword
						% kPacket_.m_kNexonAccountInfo.m_uiNexonSN
						% kPacket_.m_kNexonAccountInfo.m_bSex
						% kPacket_.m_kNexonAccountInfo.m_uiAge
						% kPacket_.m_kAuthenticateReq.m_wstrUserID
						% kPacket_.m_kNexonAccountInfo.m_byteGuestUser
						% iChannelCode														// 채널링 코드 입력
						);
				}
#else //SERV_ALL_RENEWAL_SP
				//{{ 2011. 07. 27    김민성    투니랜드 채널링
#ifdef SERV_TOONILAND_CHANNELING
				{
					int iChannelCode = static_cast< int >(kPacket_.m_kNexonAccountInfo.m_uChannelCode);

					DO_QUERY( L"exec dbo.mup_create_nx_user", L"N\'%s\', N\'%s\', %d, %d, %d, N\'%s\', %d, %d",
						% kPacket_.m_kAuthenticateReq.m_wstrUserID
						% wstrPassword
						% kPacket_.m_kNexonAccountInfo.m_uiNexonSN
						% kPacket_.m_kNexonAccountInfo.m_bSex
						% kPacket_.m_kNexonAccountInfo.m_uiAge
						% kPacket_.m_kAuthenticateReq.m_wstrUserID
						% kPacket_.m_kNexonAccountInfo.m_byteGuestUser
						% iChannelCode														// 채널링 코드 입력
						);
				}
#else
				DO_QUERY( L"exec dbo.mup_create_nx_user", L"N\'%s\', N\'%s\', %d, %d, %d, N\'%s\', %d",
					% kPacket_.m_kAuthenticateReq.m_wstrUserID
					% wstrPassword
					% kPacket_.m_kNexonAccountInfo.m_uiNexonSN
					% kPacket_.m_kNexonAccountInfo.m_bSex
					% kPacket_.m_kNexonAccountInfo.m_uiAge
					% kPacket_.m_kAuthenticateReq.m_wstrUserID
					% kPacket_.m_kNexonAccountInfo.m_byteGuestUser
					);
#endif SERV_TOONILAND_CHANNELING
				//}}
#endif //SERV_ALL_RENEWAL_SP
                
                break;
            // 해외 인증 다 해당됨 (해외의 경우 Game Server 접속 시 인증은 계정 생성 하지 않음.)
            default:
				START_LOG( cerr, L"이 인증모드에서는 이미 계정 생성이 되어 있어야 한다." )
					<< BUILD_LOG( dwAuthFlag )
					<< END_LOG;
				goto end_proc;
                break;
            }

            if( m_kODBC.BeginFetch() )
            {
				//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
				FETCH_DATA( kPacket.m_iOK
					>> kPacket.m_kAccountInfo.m_nUserUID
					>> kPacket.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
					>> kPacket.m_kAccountInfo.m_bInternalUser
					>> kPacket.m_kAccountInfo.m_bIsRecommend
					>> kPacket.m_kAccountInfo.m_wstrRegDate
					);
#else
				FETCH_DATA( kPacket.m_iOK
					>> kPacket.m_kAccountInfo.m_nUserUID
					>> kPacket.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
					>> kPacket.m_kAccountInfo.m_bInternalUser
					>> kPacket.m_kAccountInfo.m_bIsRecommend
					);
#endif SERV_CASH_ITEM_LIST
				//}}                

				m_kODBC.EndFetch();
            }

            if( kPacket.m_iOK == NetError::NET_OK )
            {
                kPacket.m_kAccountInfo.m_wstrName = kPacket_.m_kAuthenticateReq.m_wstrUserID;
                kPacket.m_kAccountInfo.m_iAuthLevel = 0;
				//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
				kPacket.m_bJoinAccount = true;
#endif SERV_IP_ACCOUNT_LOG
				//}}
            }
            else
            {
                // 신규 유저 생성 실패.
                START_LOG( clog, L"신규 유저 생성 실패." )
                    << BUILD_LOG( kPacket.m_iOK )
                    << END_LOG;
            }
        }
    }
#ifdef SERV_KOG_OTP_VERIFY
	else if( kPacket_.m_bServerUseKogOTP )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserOTP_SEL_Check", L"%d, N\'%s\'",
			% kPacket.m_kAccountInfo.m_nUserUID
			% wstrPassword );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_verify_user_otp", L"%d, N\'%s\'",
			% kPacket.m_kAccountInfo.m_nUserUID
			% wstrPassword );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				);
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			// OTP 검증 실패
			START_LOG( clog, L"OTP 검증 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket.m_kAccountInfo.m_nUserUID )
				<< BUILD_LOG( wstrPassword )
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			goto end_proc;
		}

		//{{ 2010/10/23	조효진	랜돔 시드 문제 때문에 부스터 Random으로 교체함 
		static boost::mt19937 generator(timeGetTime());	// timeGetTime() 빼면 고정 시드
		static boost::uniform_int<> uni_dist(0,INT_MAX) ;
		static boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen(generator, uni_dist);

		int iOTP  = gen();
		//}}

		wchar_t wszOTP[128];
#ifdef _CONVERT_VS_2010
				_itow_s( iOTP, wszOTP, 10 );
#else
				::_itow( iOTP, wszOTP, 10 );
#endif _CONVERT_VS_2010
		kPacket.m_kAccountInfo.m_wstrOTP = wszOTP;

#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserOTP_UPD", L"%d, N\'%s\'",
			% kPacket.m_kAccountInfo.m_nUserUID
			% kPacket.m_kAccountInfo.m_wstrOTP );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_update_user_otp", L"%d, N\'%s\'",
			% kPacket.m_kAccountInfo.m_nUserUID
			% kPacket.m_kAccountInfo.m_wstrOTP );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				);
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			// OTP 업데이트 실패
			START_LOG( clog, L"OTP 업데이트 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_UNKNOWN;
			goto end_proc;
		}
	}
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_COUNTRY_CN
	if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		DO_QUERY( L"exec mup_get_giant_account_type", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );

		int iOK = 0;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK 
				>> kPacket.m_uiGiantAccountType	);

			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			// 자이언트 계정 타입 가져오기 실패
			START_LOG( clog, L"자이언트 계정 타입 가져오기 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket.m_uiGiantAccountType )
				<< END_LOG;
		}
	}
	else
	{
		// 사내
		kPacket.m_uiGiantAccountType = 24;
	}
#endif //SERV_COUNTRY_CN
	
	//해킹유저 정보 받아오기.
	if( kPacket.m_iOK == NetError::NET_OK )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec P_MUserHackerList_SEL", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec mup_get_hackerlist", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
		int iOK = 0;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		switch( iOK )
		{
		//일반유저
		case 0:
			kPacket.m_cHackingUserType = SEnum::HUT_NORMAL;
			break;
		//등록되고 확인한 유저
		case 1:
			kPacket.m_cHackingUserType = SEnum::HUT_AGREE_HACK_USER;
			break;
		//등록되고 확인안한 유저.
		case 2:
			kPacket.m_cHackingUserType = SEnum::HUT_DISAGREE_HACK_USER;
			break;

		default:
			{
				START_LOG( cerr, L"해킹유저 타입이상.!" )
					<< BUILD_LOG( kPacket.m_kAccountInfo.m_nUserUID )
					<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrUserID )
					<< BUILD_LOG( iOK )
					<< END_LOG;
			}
			break;
		}
	}

	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	// 캐릭터 카운트 정보 얻기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserUID_Count_SEL", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_UserUID_Count", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int iLogType = 0;
		int iCountValue = 0;

		FETCH_DATA( iLogType
			>> iCountValue
			);

		kPacket.m_mapAccCountInfo.insert( std::make_pair( iLogType, iCountValue ) );
	}
#endif SERV_ACCOUNT_COUNT
	//}}

	//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
	if( kPacket_.m_kNexonAccountInfo.m_byteGuestUser != 1 )  // 체험 아이디가 아닐때만 얻어온다.
	{
		int iOK_ = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserAuthentication_SEL", L"N\'%s\'", % kPacket.m_kAccountInfo.m_wstrID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_get_user_authkey", L"N\'%s\'", % kPacket.m_kAccountInfo.m_wstrID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_kAccountInfo.m_iChannelRandomKey 
				>> iOK_ );

			m_kODBC.EndFetch();
		}

		if( iOK_ != 0)
		{
			START_LOG( cerr, L"해킹유저 타입이상.!" )
				<< BUILD_LOG( kPacket.m_kAccountInfo.m_wstrID )
				<< BUILD_LOG( kPacket.m_kAccountInfo.m_iChannelRandomKey  )
				<< BUILD_LOG( iOK_ )
				<< END_LOG;
		}
	}
	else
	{
		kPacket.m_iOK = NetError::NET_OK;
		START_LOG( clog, L"체험아이디는 인증 랜덤키를 사용하지 않는다" )
			<< BUILD_LOG( kPacket.m_kAccountInfo.m_wstrID )
			<< BUILD_LOG( kPacket.m_kAccountInfo.m_iChannelRandomKey  )
			<< END_LOG;
	}
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
	//}}
	
#ifdef SERV_COUNTRY_PH
	unsigned short usGarenaCyberCafe = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserOption_SEL", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserOption_GET", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( usGarenaCyberCafe);

		m_kODBC.EndFetch();
	}
	kPacket.m_usGarenaCyberCafe = usGarenaCyberCafe;
#endif //SERV_COUNTRY_PH

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	if( kPacket.m_iOK == NetError::NET_OK )
	{
		kPacket.m_iEventMoney = 0;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_SEL ", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_GET ", L"%d", % kPacket.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iEventMoney );

			m_kODBC.EndFetch();
		}
	}
#endif // SERV_EVENT_MONEY
	
end_proc:
    LOG_SUCCESS( kPacket.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket.m_iOK )
		<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_bDebugAuth )
		<< BUILD_LOG( kPacket_.m_kAuthenticateReq.m_wstrUserID )
		<< BUILD_LOG( wstrPassword )
		<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_uiAge )
		<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_uiNexonSN );

    SendToGSUser( LAST_SENDER_UID, FIRST_SENDER_UID, ELG_NEXON_USER_AUTHENTICATE_ACK, kPacket );
}

//{{ 2009. 5. 26  최육사	채널이동
IMPL_ON_FUNC( ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_REQ )
{
	KELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK kPacket;
	kPacket.m_kUserAuthAck.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrID = kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID;
	kPacket.m_kUserAuthAck.m_kNexonAccountInfo = kPacket_.m_kUserAuthReq.m_kNexonAccountInfo;
	kPacket.m_kUserAuthAck.m_cHackingUserType = SEnum::HUT_NORMAL;
	kPacket.m_kConnectAck = kPacket_.m_kConnectAck;			// 채널이동을 위한 정보
	kPacket.m_kSelectUnitReq = kPacket_.m_kSelectUnitReq;	// 채널이동을 위한 정보
	kPacket.m_kChangeUserInfo = kPacket_.m_kChangeUserInfo; // 채널이동을 위한 정보
#ifdef SERV_EPAY_SYSTEM
	kPacket.m_usZone = kPacket_.m_usZone;
	kPacket.m_usGame = kPacket_.m_usGame;
#endif //SERV_EPAY_SYSTEM
#ifdef SERV_COUNTRY_TH
	kPacket.m_wstrSocketID = kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrSocketID;
#endif // SERV_COUNTRY_TH

    // 이 쓰레드에서 sim layer 변수를 참조하는 것이 그닥 좋은 것은 아니다.
    DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();

    std::wstring wstrPassword;
    if( !kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth && dwAuthFlag == KSimLayer::AF_NEXON_KOREA )
    {
        wstrPassword = kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_wstrLoginIP;
    }
    else
    {
        wstrPassword = kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrPasswd;
    }

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-03	// 박세훈
	DO_QUERY( L"exec dbo.P_MUser_SEL_Auth", L"%d, N\'%s\', N\'%s\', %d, %d",
		% kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth
		% kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID
		% wstrPassword.c_str()
		% kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_uiAge
		% kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_uiNexonSN
		);

	if( m_kODBC.BeginFetch() )
	{
		byte byteBlockLevel = 0;

		FETCH_DATA( kPacket.m_kUserAuthAck.m_iOK
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrName
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrRegDate
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrLastLogin
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_byteBlockType
			>> byteBlockLevel
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason2
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockEndDate
			);

		m_kODBC.EndFetch();

		if( byteBlockLevel == 1 )
		{
			kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockEndDate.clear();
		}
	}
#else // SERV_HACKING_USER_CHECK_COUNT
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_SEL_Auth", L"%d, N\'%s\', N\'%s\', %d, %d", % kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth % kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID % wstrPassword.c_str() % kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_uiAge % kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_uiNexonSN );
	
	if( m_kODBC.BeginFetch() )
	{
		byte byteBlockType = 0;
		byte byteBlockLevel = 0;
		std::wstring wstrBlockReason2 = L"";
		std::wstring wstrBlockEndDate = L"";

		FETCH_DATA( kPacket.m_kUserAuthAck.m_iOK
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrName
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrRegDate
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrLastLogin
			>> byteBlockType//kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_byteBlockType
			>> byteBlockLevel
			>> wstrBlockReason2//kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason2
			>> wstrBlockEndDate//kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockEndDate
			>> kPacket.m_kUserAuthAck.m_kNexonAccountInfo.m_uiNexonSN		// SERV_GLOBAL_AUTH
			);
	}
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_auth_user", L"%d, N\'%s\', N\'%s\', %d, %d", % kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_bDebugAuth % kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID % wstrPassword.c_str() % kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_uiAge % kPacket_.m_kUserAuthReq.m_kNexonAccountInfo.m_uiNexonSN );

	if( m_kODBC.BeginFetch() )
	{
		//{{ 2011. 02. 23	최육사	캐쉬 상품 리스트
#ifdef SERV_CASH_ITEM_LIST
		FETCH_DATA( kPacket.m_kUserAuthAck.m_iOK
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrName
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrRegDate
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrLastLogin
			>> kPacket.m_kUserAuthAck.m_kNexonAccountInfo.m_uiNexonSN		// SERV_GLOBAL_AUTH
			);
#else
		FETCH_DATA( kPacket.m_kUserAuthAck.m_iOK
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrName
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_iAuthLevel
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountOption.m_bPlayGuide
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bInternalUser
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrEndTime
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_kAccountBlockInfo.m_wstrBlockReason			
			>> kPacket.m_kUserAuthAck.m_kAccountInfo.m_bIsRecommend
			>> kPacket.m_kAccountInfo.m_wstrRegDate			//SERV_GLOBAL_AUTH //SERV_INT_ONLY	
			>> kPacket.m_kAccountInfo.m_wstrLastLogin		//SERV_GLOBAL_AUTH //SERV_INT_ONLY
			>> kPacket.m_kNexonAccountInfo.m_uiNexonSN		//SERV_GLOBAL_AUTH //SERV_INT_ONLY
			);
#endif SERV_CASH_ITEM_LIST
		//}}		

		m_kODBC.EndFetch();
	}
#endif //SERV_ALL_RENEWAL_SP
#endif // SERV_HACKING_USER_CHECK_COUNT

	if( kPacket.m_kUserAuthAck.m_iOK != NetError::NET_OK )
	{
		// 계정 DB 인증 실패
		START_LOG( clog, L"존재하지 않거나 삭제/비번 다른 계정." )
			<< BUILD_LOG( kPacket.m_kUserAuthAck.m_iOK )
			<< END_LOG;

		switch( kPacket.m_kUserAuthAck.m_iOK )
		{
		case -1:
			kPacket.m_kUserAuthAck.m_iOK = NetError::ERR_VERIFY_04;
			break;
		case -2:            // 추후에 계정 삭제가 들어갈 경우 삭제된 유저가 접속했을 때의 처리가 필요하다.
			kPacket.m_kUserAuthAck.m_iOK = NetError::ERR_VERIFY_05;
			break;
		case -3:
			kPacket.m_kUserAuthAck.m_iOK = NetError::ERR_VERIFY_06;
			break;
		default:
			kPacket.m_kUserAuthAck.m_iOK = NetError::ERR_UNKNOWN;
			break;
		}

		// 채널이동 실패처리 합니다.
	}
#ifdef SERV_KOG_OTP_VERIFY
	else if( kPacket_.m_bServerUseKogOTP )
    {
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserOTP_SEL_Check", L"%d, N\'%s\'",
			% kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
			% wstrPassword );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_verify_user_otp", L"%d, N\'%s\'",
            % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
            % wstrPassword );
#endif //SERV_ALL_RENEWAL_SP
        if( m_kODBC.BeginFetch() )
        {
            FETCH_DATA( kPacket.m_kUserAuthAck.m_iOK
                );
            m_kODBC.EndFetch();
        }

		if( kPacket.m_kUserAuthAck.m_iOK != NetError::NET_OK )
		{
			// OTP 검증 실패
			START_LOG( clog, L"OTP 검증 실패." )
				<< BUILD_LOG( kPacket.m_kUserAuthAck.m_iOK )
				<< BUILD_LOG( kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID )
				<< BUILD_LOG( wstrPassword )
				<< END_LOG;

			kPacket.m_kUserAuthAck.m_iOK = NetError::ERR_UNKNOWN;
			goto end_proc;
		}

        if( kPacket.m_kUserAuthAck.m_iOK == NetError::NET_OK )
        {
			//{{ 2011/1/6	조효진	랜돔 시드 문제 때문에 부스터 Random으로 교체함 
			static boost::mt19937 generator(timeGetTime());	// timeGetTime() 빼면 고정 시드
			static boost::uniform_int<> uni_dist(0,INT_MAX) ;
			static boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen(generator, uni_dist);

			int iOTP  = gen();
            wchar_t wszOTP[128];
#ifdef _CONVERT_VS_2010
			_itow_s( iOTP, wszOTP, 10 );
#else
			::_itow( iOTP, wszOTP, 10 );
#endif _CONVERT_VS_2010
            kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrOTP = wszOTP;

#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_MUserOTP_UPD", L"%d, N\'%s\'",
				% kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
				% kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrOTP );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_update_user_otp", L"%d, N\'%s\'",
                % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID
                % kPacket.m_kUserAuthAck.m_kAccountInfo.m_wstrOTP );
#endif //SERV_ALL_RENEWAL_SP
            if( m_kODBC.BeginFetch() )
            {
                FETCH_DATA( kPacket.m_kUserAuthAck.m_iOK
                    );
                m_kODBC.EndFetch();
            }

            if( kPacket.m_kUserAuthAck.m_iOK != NetError::NET_OK )
            {
                // OTP 업데이트 실패
                START_LOG( clog, L"OTP 업데이트 실패." )
                    << BUILD_LOG( kPacket.m_kUserAuthAck.m_iOK )
                    << END_LOG;

				kPacket.m_kUserAuthAck.m_iOK = NetError::ERR_UNKNOWN;
				goto end_proc;
            }
        }
    }
#endif // SERV_KOG_OTP_VERIFY

#ifdef SERV_COUNTRY_CN
	if ( KSimLayer::GetKObj()->GetAuthFlag() == KSimLayer::AF_GLOBAL_SERVICE )
	{
		DO_QUERY( L"exec mup_get_giant_account_type", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );

		int iOK = 0;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK 
				>> kPacket.m_uiGiantAccountType	);

			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			// 자이언트 계정 타입 가져오기 실패
			START_LOG( clog, L"자이언트 계정 타입 가져오기 실패." )
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( kPacket.m_uiGiantAccountType )
				<< END_LOG;
		}
	}
	else
	{
		// 사내
		kPacket.m_uiGiantAccountType = 24;
	}
#endif //SERV_COUNTRY_CN
	
	//해킹유저 정보 받아오기.
	if( kPacket.m_kUserAuthAck.m_iOK == NetError::NET_OK )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec P_MUserHackerList_SEL", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec mup_get_hackerlist", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
		int iOK = 0;

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );

			m_kODBC.EndFetch();
		}

		switch( iOK )
		{
			//일반유저
		case 0:
			kPacket.m_kUserAuthAck.m_cHackingUserType = SEnum::HUT_NORMAL;
			break;
			//등록되고 확인한 유저
		case 1:
			kPacket.m_kUserAuthAck.m_cHackingUserType = SEnum::HUT_AGREE_HACK_USER;
			break;
			//등록되고 확인안한 유저.
		case 2:
			kPacket.m_kUserAuthAck.m_cHackingUserType = SEnum::HUT_DISAGREE_HACK_USER;
			break;

		default:
			{
				START_LOG( cerr, L"해킹유저 타입이상.!" )
					<< BUILD_LOG( kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID )
					<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID )
					<< BUILD_LOG( iOK )
					<< END_LOG;
			}
			break;
		}
	}

	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	// 캐릭터 카운트 정보 얻기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserUID_Count_SEL", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_UserUID_Count", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int iLogType = 0;
		int iCountValue = 0;

		FETCH_DATA( iLogType
			>> iCountValue
			);

		kPacket.m_kUserAuthAck.m_mapAccCountInfo.insert( std::make_pair( iLogType, iCountValue ) );
	}
#endif SERV_ACCOUNT_COUNT
	//}}

	//{{ 2012. 02. 21	김민성	2차 보안 채널 이동시(미니맵) 오류 수정
#ifdef SERV_SECOND_SECURITY_PW_ERROR_MODIFY
	// 2차 비번 얻기 : kPacket_ --> UserUID
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_SEL", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_second_pw_Info", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_bUseSecondPW
				 >> kPacket.m_wstrSecondPW
				 >> kPacket.m_iFailedCount
				 >> kPacket.m_wstrLastAuthDate );

		m_kODBC.EndFetch();
	}
#endif SERV_SECOND_SECURITY_PW_ERROR_MODIFY
	//}}

#ifdef SERV_COUNTRY_PH
	unsigned short usGarenaCyberCafe = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserOption_SEL", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserOption_GET", L"%d", % kPacket.m_kUserAuthAck.m_kAccountInfo.m_nUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( usGarenaCyberCafe);

		m_kODBC.EndFetch();
	}
	
	kPacket.m_usGarenaCyberCafe = usGarenaCyberCafe;
#endif //SERV_COUNTRY_PH

end_proc:
	LOG_SUCCESS( kPacket.m_kUserAuthAck.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( kPacket.m_kUserAuthAck.m_iOK )
		<< BUILD_LOG( kPacket_.m_kUserAuthReq.m_kAuthenticateReq.m_wstrUserID );

	SendToGSUser( LAST_SENDER_UID, FIRST_SENDER_UID, ELG_CHANNEL_CHANGE_NEXON_USER_AUTHENTICATE_ACK, kPacket );
}
//}}

IMPL_ON_FUNC( DBE_UPDATE_IS_LOGIN_NOT )
{
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_UPD_IsLogin", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_bIsLogin );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_islogin", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_bIsLogin );
#endif //SERV_ALL_RENEWAL_SP
	int iOK = NetError::NET_OK;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	//{{ 2011. 02. 23	최육사	계정 단위 카운트
#ifdef SERV_ACCOUNT_COUNT
	{
		std::map< int, int >::const_iterator mitCGC;
		for( mitCGC = kPacket_.m_mapAccCountInfo.begin(); mitCGC != kPacket_.m_mapAccCountInfo.end(); ++mitCGC )
		{
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_MUserUID_Count_MER", L"%d, %d, %d", 
				% kPacket_.m_iUserUID
				% mitCGC->first
				% mitCGC->second
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_update_UserUID_Count", L"%d, %d, %d", 
				% kPacket_.m_iUserUID
				% mitCGC->first
				% mitCGC->second
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}
		}
	}
#endif SERV_ACCOUNT_COUNT
	//}}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( L"Guest User 로그인 정보 갱신.!" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_bIsLogin )
		;
}

//{{ 2010. 06. 22  최육사	계정블럭기능
#ifdef SERV_ACCOUNT_BLOCK

IMPL_ON_FUNC( DBE_CHECK_ACCOUNT_BLOCK_LIST_REQ )
{
	KDBE_CHECK_ACCOUNT_BLOCK_LIST_ACK kPacket;

	// Release Tick 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_ReleaseTick_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.mup_get_release_tick" );
#endif //SERV_ALL_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		int iType = 0;
		int iReleaseTick = 0;

		FETCH_DATA( iType
			>> iReleaseTick );

		// 해당 타입 있는가?
		std::map< int, int >::const_iterator mit = kPacket_.m_mapReleaseTick.find( iType );
		if( mit == kPacket_.m_mapReleaseTick.end() )
			continue;

		// 해당 타입의 Tick값보다 높은가?
		if( iReleaseTick <= mit->second )
			continue;
		
		kPacket.m_mapReleaseTick.insert( std::make_pair( iType, iReleaseTick ) );
	}

	if( kPacket.m_mapReleaseTick.empty() )
		return;

	{
		std::map< int, int >::const_iterator mit;
		for( mit = kPacket.m_mapReleaseTick.begin(); mit != kPacket.m_mapReleaseTick.end(); ++mit )
		{
			switch( mit->first )
			{
			case KAccountBlockManager::BT_CONNECT_BLOCK: // 접속 차단 리스트
				{
					std::set< UidType > setAccountBlockList;

					// 계정 블럭 리스트 얻기
#ifdef SERV_ALL_RENEWAL_SP
					DO_QUERY_NO_ARG( L"exec dbo.P_RejectedUser_SEL" );
#else //SERV_ALL_RENEWAL_SP
					DO_QUERY_NO_ARG( L"exec dbo.mup_get_rejected_user" );
#endif //SERV_ALL_RENEWAL_SP
					while( m_kODBC.Fetch() )
					{
						UidType iUserUID = 0;

						FETCH_DATA( iUserUID );

						setAccountBlockList.insert( iUserUID );
					}

					kPacket.m_mapAccountBlockList.insert( std::make_pair( mit->first, setAccountBlockList ) );
				}
				break;

			case KAccountBlockManager::BT_TRADE_BLOCK: // 개인 거래 차단 리스트
				{
					std::set< UidType > setTradeBlockList;
#ifdef SERV_ALL_RENEWAL_SP
					DO_QUERY_NO_ARG( L"exec dbo.P_TradeBlock_SEL" );
#else //SERV_ALL_RENEWAL_SP
					// 거래 블럭 리스트 얻기
					DO_QUERY_NO_ARG( L"exec dbo.mup_get_TradeBlock_user" );
#endif //SERV_ALL_RENEWAL_SP

					while( m_kODBC.Fetch() )
					{
						UidType iUserUID = 0;

						FETCH_DATA( iUserUID );

						setTradeBlockList.insert( iUserUID );
					}

					kPacket.m_mapAccountBlockList.insert( std::make_pair( mit->first, setTradeBlockList ) );
				}
				break;

				//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
			case KAccountBlockManager::BT_MACHINE_ID_BLOCK:
				{
					// 거래 블럭 리스트 얻기
#ifdef SERV_ALL_RENEWAL_SP
					DO_QUERY_NO_ARG( L"exec dbo.P_Temp_Add_SEL" );
#else //SERV_ALL_RENEWAL_SP
					DO_QUERY_NO_ARG( L"exec dbo.mup_get_add" );
#endif //SERV_ALL_RENEWAL_SP

					while( m_kODBC.Fetch() )
					{
						std::string strMachineID;

						FETCH_DATA( strMachineID );

						kPacket.m_vecMachineIDBlockList.push_back( strMachineID );
					}
				}
				break;
#endif SERV_MACHINE_ID_BLOCK
				//}}

			default:
				break;
			}
		}
	}

end_proc:
	SendToServer( DBE_CHECK_ACCOUNT_BLOCK_LIST_ACK, kPacket );
}

#endif SERV_ACCOUNT_BLOCK
//}}

//{{ 2010. 10. 05	최육사	센터서버 해킹체크
#ifdef SERV_CN_SERVER_HACKING_CHECK

_IMPL_ON_FUNC( DBE_REG_REJECTED_USER_NOT, KELG_REG_REJECTED_USER_NOT )
{
	// DB에 Rejected User 등록!
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
	if( kPacket_.m_cPeriodUAL == SEnum::UAL_NORMAL )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_RejectedUser_MER", L"%d, %d", % kPacket_.m_iUserUID % (int)kPacket_.m_cRejectedReason );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_insert_rejected_user", L"%d, %d", % kPacket_.m_iUserUID % (int)kPacket_.m_cRejectedReason );
#endif //SERV_ALL_RENEWAL_SP
	}
	else if( kPacket_.m_cPeriodUAL < SEnum::UAL_NORMAL )
	{
		DO_QUERY( L"exec dbo.P_UserAuthLevel_INS", L"%d, %d, %d, N\'%s\'",
			% kPacket_.m_iUserUID
			% static_cast<int>( kPacket_.m_cPeriodUAL )
			% static_cast<int>( kPacket_.m_cPeriodUAL )
			% kPacket_.m_wstrBlockReason
			);
	}
#else // SERV_PERIOD_ACCOUNT_BLOCK
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_RejectedUser_MER", L"%d, %d", % kPacket_.m_iUserUID % (int)kPacket_.m_cRejectedReason );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_rejected_user", L"%d, %d", % kPacket_.m_iUserUID % (int)kPacket_.m_cRejectedReason );
#endif //SERV_ALL_RENEWAL_SP
#endif // SERV_PERIOD_ACCOUNT_BLOCK

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Rejected User 등록 실패.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOGc( kPacket_.m_cRejectedReason )
#ifdef SERV_PERIOD_ACCOUNT_BLOCK// 작업날짜: 2013-05-27	// 박세훈
			<< BUILD_LOGc( kPacket_.m_cPeriodUAL )
			<< BUILD_LOG( kPacket_.m_wstrBlockReason )
#endif // SERV_PERIOD_ACCOUNT_BLOCK
			<< END_LOG;
	}
}

#endif SERV_CN_SERVER_HACKING_CHECK
//}}

//{{ 2011. 03. 02	최육사	개인 거래 차단
#ifdef SERV_HACK_USER_TRADE_BLOCK
_IMPL_ON_FUNC( DBE_REG_TRADE_BLOCK_USER_NOT, KELG_REG_REJECTED_USER_NOT )
{
	// DB에 Trade Block User 등록!
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_TradeBlock_INS", L"%d, %d", % kPacket_.m_iUserUID % (int)kPacket_.m_cRejectedReason );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_TradeBlock_user", L"%d, %d", % kPacket_.m_iUserUID % (int)kPacket_.m_cRejectedReason );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Trade Block User 등록 실패.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOGc( kPacket_.m_cRejectedReason )
			<< END_LOG;
	}
}
#endif SERV_HACK_USER_TRADE_BLOCK
//}}

//{{  2012. 1. 30	김민성		한방핵 방어(해킹 프로세스 검사)
#ifdef SERV_HACKING_DLL_CHECK
IMPL_ON_FUNC( DBE_GET_HACKING_MODULE_LIST_REQ )
{
	KDBE_GET_HACKING_MODULE_LIST_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_00;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_ReleaseTick_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.mup_get_release_tick" );
#endif //SERV_ALL_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		int iType = 0;
		int iReleaseTick = 0;

		FETCH_DATA( iType
				 >> iReleaseTick );

		START_LOG( clog, L"릴리즈틱 확인하자" )
			<< BUILD_LOG( iType )
			<< BUILD_LOG( iReleaseTick )
			<< END_LOG;

		if( KAccountBlockManager::BT_MODULE_LIST == iType && iReleaseTick > kPacket_.m_iReleaseTick )
		{
			kAck.m_bRequest = true;
			kAck.m_iReleaseTick = iReleaseTick;

			START_LOG( clog, L"Hacking Module List 갱신이 가능하다!" )
				<< BUILD_LOG( kAck.m_bRequest )
				<< BUILD_LOG( kAck.m_iReleaseTick )
				<< END_LOG;
		}
	}

	if( kAck.m_bRequest == true )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_ARG( L"exec dbo.P_HackingModulelist_SEL" );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY_NO_ARG( L"exec dbo.P_HackingModulelist_GET" );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KModuleInfo kInfo;

			FETCH_DATA( kInfo.m_wstrModuleName
					 >> kInfo.m_iAllow );

			kAck.m_vecModuleList.push_back( kInfo );
		}
	}

	kAck.m_iOK = NetError::NET_OK;

	START_LOG( clog, L"Hacking Module List 갱신이 가능하다!" )
		<< BUILD_LOG( kAck.m_vecModuleList.size() )
		<< END_LOG;

end_proc:
	SendToServer( DBE_GET_HACKING_MODULE_LIST_ACK, kAck );
}
#endif SERV_HACKING_DLL_CHECK
//}}

//{{ 2013. 3. 18	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_REQ )
{
	std::map<UidType, UidType>::const_iterator it;
	for( it = kPacket_.m_mapRankerUIDInfo.begin(); it != kPacket_.m_mapRankerUIDInfo.end(); ++it )
	{
		// 랭커 유저 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % it->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % it->second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			KLocalRankingUserInfo kUserInfo;
			int iOK = NetError::ERR_ODBC_01;
			std::wstring wstrUpdateDate;

			FETCH_DATA( iOK
				>> kUserInfo.m_byteFilter
				>> kUserInfo.m_iCategory
				>> kUserInfo.m_byteGender
				>> kUserInfo.m_iBirth
				>> kUserInfo.m_wstrProfile
				>> wstrUpdateDate
				);
			m_kODBC.EndFetch();

			if( iOK == NetError::NET_OK )
			{
				kUserInfo.m_iUserUID = it->second;
				kPacket_.m_mapRankerUserInfo.insert( std::map<UidType, KLocalRankingUserInfo>::value_type( kUserInfo.m_iUserUID, kUserInfo ) );
			}
		}
	}

	kPacket_.m_iOK = NetError::NET_OK;

end_proc:
	SendToServer( DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_GET_GUILD_INFO_REQ )
{
	std::map<UidType, UidType>::const_iterator it;
	for( it = kPacket_.m_mapRankerUIDInfo.begin(); it != kPacket_.m_mapRankerUIDInfo.end(); ++it )
	{
		// 랭커 유저 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % it->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % it->second );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			KLocalRankingUserInfo kUserInfo;
			int iOK = NetError::ERR_ODBC_01;
			std::wstring wstrUpdateDate;

			FETCH_DATA( iOK
				>> kUserInfo.m_byteFilter
				>> kUserInfo.m_iCategory
				>> kUserInfo.m_byteGender
				>> kUserInfo.m_iBirth
				>> kUserInfo.m_wstrProfile
				>> wstrUpdateDate
				);
			m_kODBC.EndFetch();

			if( iOK == NetError::NET_OK )
			{
				kUserInfo.m_iUserUID = it->second;
				kPacket_.m_mapRankerUserInfo.insert( std::map<UidType, KLocalRankingUserInfo>::value_type( kUserInfo.m_iUserUID, kUserInfo ) );
			}
		}
	}

	kPacket_.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_GUILD_INFO_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	kPacket_.m_iOK = NetError::ERR_ODBC_01;

	// 랭커 유저 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		std::wstring wstrUpdated;
		FETCH_DATA( kPacket_.m_iOK
			>> kPacket_.m_kUserInfo.m_byteFilter
			>> kPacket_.m_kUserInfo.m_iCategory
			>> kPacket_.m_kUserInfo.m_byteGender
			>> kPacket_.m_kUserInfo.m_iBirth
			>> kPacket_.m_kUserInfo.m_wstrProfile
			>> wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket_ );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef SERV_COUNTRY_PH
IMPL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_REQ )
{
	KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK kPacket;

	kPacket.m_iResult = 0;
	kPacket.m_uiPublisherUID = kPacket_.m_uiGarenaUID;
	kPacket.m_iSessionUID = kPacket_.m_iSessionUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_By_PublisherSN_SEL", L"%d", % kPacket_.m_uiGarenaUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_By_PublisherSN_GET", L"%d", % kPacket_.m_uiGarenaUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{

		FETCH_DATA( kPacket.m_iUserUID
			>> kPacket.m_wstrUserID
			>> kPacket.m_wstrUserName
			);
		m_kODBC.EndFetch();

	}
	else
	{
		kPacket.m_iResult = 1;
		kPacket.m_iUserUID = 0;
		kPacket.m_wstrUserID = L"";
		kPacket.m_wstrUserName = L"";

		START_LOG( cwarn, L"우리게임에 캐릭터도 없는데 전환 신청이 들어 왔다." )
			<< BUILD_LOG( kPacket_.m_uiGarenaUID )
			<< END_LOG;
		goto end_proc;
	}

end_proc:	
		
		KEventPtr spEvent( new KEvent );
		spEvent->SetData(PI_LOGIN_GARENA_BILLING, NULL, EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK, kPacket );

		SiKGarenaBillingServer()->QueueingEvent( spEvent );

		//SendToUser( FIRST_SENDER_UID, EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_ACCOUNT_CHECK_ACK, kPacket );

}

IMPL_ON_FUNC( EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_REQ )
{
	KEBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK kPacket;

	kPacket.m_kGNGameCurrencyREQ = kPacket_.m_kGNGameCurrencyREQ;

	kPacket.m_iResult = 0;
	kPacket.m_uiPublisherUID = kPacket_.m_kGNGameCurrencyREQ.m_uiGarenaUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_By_PublisherSN_SEL", L"%d", % kPacket_.m_kGNGameCurrencyREQ.m_uiGarenaUID);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_By_PublisherSN_GET", L"%d", % kPacket_.m_kGNGameCurrencyREQ.m_uiGarenaUID);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{

		FETCH_DATA( kPacket.m_iUserUID
			>> kPacket.m_wstrUserID
			>> kPacket.m_wstrUserName
			);
		m_kODBC.EndFetch();

	}
	else
	{
		kPacket.m_iResult = 1;
		kPacket.m_iUserUID = 0;
		kPacket.m_wstrUserID = L"";
		kPacket.m_wstrUserName = L"";

		START_LOG( cerr, L"우리게임에 캐릭터도 없는데 전환 신청이 들어 왔다." )
			<< BUILD_LOG( kPacket_.m_kGNGameCurrencyREQ.m_uiGarenaUID )
			<< END_LOG;
		goto end_proc;
	}

end_proc:

	KEventPtr spEvent( new KEvent );
	spEvent->SetData(PI_LOGIN_GARENA_BILLING, NULL, EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK, kPacket );

	SiKGarenaBillingServer()->QueueingEvent( spEvent );
	//SendToUser( FIRST_SENDER_UID, EBILL_LOGIN_ACCOUNT_USER_OFFLINE_GN_EXCHANGE_ACK, kPacket );

}
#endif //SERV_COUNTRY_PH