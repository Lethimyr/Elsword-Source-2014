#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "GSAccountDBThread.h"
#include "GameServer.h"
//{{ 2012. 05. 10	최육사	서버 버전
//#ifdef SERV_VERSION_FLAG
#include "SimLayer.h"
//#endif SERV_VERSION_FLAG
//}}
#include "NetError.h"

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
#include "GameSysVal/GameSysVal.h"
#endif SERV_COME_BACK_USER_REWARD
//}} 

#ifdef SERV_KEY_MAPPING_INT
#define GAMEPAD_ID_MODIFIER 20000
#endif //SERV_KEY_MAPPING_INT

//ImplementDBThread( KGSAccountDBThread );
ImplPfID( KGSAccountDBThread, PI_GS_ACCOUNT_DB );

#define CLASS_TYPE KGSAccountDBThread

IMPL_PROFILER_DUMP( KGSAccountDBThread )
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

KGSAccountDBThread::~KGSAccountDBThread(void)
{
}

void KGSAccountDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {
		//{{ 2009. 12. 22  최육사	서버군확장
		CASE( ELOG_STAT_PLAY_TIME );
		//}}

        CASE_NOPARAM( DBE_UPDATE_LOGIN_TIME_NOT );
        CASE( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ );

	   _CASE( DBE_ADMIN_CHANGE_AUTH_LEVEL_REQ, KEGS_ADMIN_CHANGE_AUTH_LEVEL_REQ );
	   _CASE( DBE_ADMIN_GET_AUTH_LEVEL_LIST_REQ, KEGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ );

	   _CASE( DBE_ACCOUNT_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_ACK );
	   //{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
	   _CASE( DBE_ACCOUNT_SELECT_UNIT_REQ, KDBE_SELECT_UNIT_ACK );
#else
	   _CASE( DBE_ACCOUNT_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_ACK );
#endif SERV_SELECT_UNIT_PACKET_DIVISION
	   //}}
		//{{ 2009. 6. 4  최육사	캐릭터 선택
	   _CASE( DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_REQ, KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK );
		//}}
#ifdef SERV_UNIT_WAIT_DELETE
	   _CASE( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_ACK );
#else //SERV_UNIT_WAIT_DELETE
	   _CASE( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_ACK );
#endif //SERV_UNIT_WAIT_DELETE

		//{{ 2008. 3. 28  최육사  추천인
		CASE_NOPARAM( DBE_RECOMMEND_USER_NOT );
		//}}

	   _CASE( DBE_AGREE_HACK_USER_REQ, UidType );
	   _CASE( DBE_REPORT_HACK_USER_NOT, UidType );

	   _CASE( DBE_GET_MY_MESSENGER_SN_REQ, KEGS_GET_MY_MESSENGER_SN_REQ );

		//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
		CASE( DBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT );
#endif SERV_IP_ACCOUNT_LOG
		//}}

		//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY
		CASE( DBE_GET_SECOND_SECURITY_INFO_REQ );
		CASE( DBE_SEUCCESS_SECOND_SECURITY_REQ );
		_CASE( DBE_FAILED_SECOND_SECURITY_REQ, UidType );
		CASE( DBE_CREATE_SECOND_SECURITY_REQ );
		CASE( DBE_DELETE_SECOND_SECURITY_REQ );
		CASE( DBE_CHANGE_SECOND_SECURITY_PW_REQ );
		CASE( DBE_UPDATE_SECURITY_TYPE_NOT );
#endif SERV_SECOND_SECURITY
		//}}

		//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
		CASE( DBE_WRITE_COME_BACK_REWARD_NOT );
		CASE( DBE_WRITE_COME_BACK_END_NOT );
		CASE( DBE_WRITE_COME_BACK_LOGOUT_NOT );
#endif SERV_COME_BACK_USER_REWARD
		//}}
		//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
		CASE( DBE_CHECK_RETAINING_USER_REQ );
		CASE( DBE_INSERT_RETAINING_USER_REQ );
#endif SERV_NEXON_SESSION_PROTOCOL
		//}} 
		//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
		_CASE( DBE_UPDATE_CHANNEL_RANDOMKEY_NOT, KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK );
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
		//}}
		//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		CASE( DBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ );
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
		//}}

		//{{ 2012. 05. 17	박세훈	키보드 맵핑 정보를 DB에서 읽고 쓰기
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
		CASE( DBE_KEYBOARD_MAPPING_INFO_WRITE_REQ );
		_CASE( DBE_KEYBOARD_MAPPING_INFO_READ_REQ, UidType );
#endif SERV_KEYBOARD_MAPPING_INFO_RW
		//}}

		//{{ 2012. 05. 29	박세훈	채팅 옵션 정보를 DB에서 읽고 쓰기
#ifdef SERV_CHAT_OPTION_INFO_RW
		CASE( DBE_CHAT_OPTION_INFO_WRITE_REQ );
		_CASE( DBE_CHAT_OPTION_INFO_READ_REQ, UidType );
#endif SERV_CHAT_OPTION_INFO_RW
		//}}
		//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
		CASE( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT );
#endif SERV_EVENT_DAILY_GIFT_BOX
		//}}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
		CASE( DBE_UPDATE_EVENT_MONEY_REQ );
#endif // SERV_EVENT_MONEY	

		//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
		//CASE_NOPARAM( DBE_BINGO_EVENT_INFO_READ_REQ );
		CASE( DBE_BINGO_EVENT_INFO_WRITE_REQ );
#endif SERV_EVENT_BINGO
		//}}
		//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
		CASE_NOPARAM( DBE_2012_WINTER_VACATION_EVENT_NOT );
#endif SERV_2012_WINTER_VACATION_EVENT
		//}}
		//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
		_CASE( DBE_NEW_ACCOUNT_QUEST_REQ, KDBE_NEW_QUEST_REQ );
		_CASE( DBE_ACCOUNT_QUEST_COMPLETE_REQ, KDBE_QUEST_COMPLETE_REQ );
		CASE( DBE_ACCOUNT_QUEST_UPDATE_NOT );
#endif SERV_ACCOUNT_MISSION_SYSTEM
		//}}
		//{{ 2012. 12. 19  교환(한정 수량) - 김민성
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
		CASE( DBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ );
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
		//}}
		//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		CASE( DBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT );
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}

#ifdef SERV_NEW_PUNISHMENT
		_CASE( DBE_GET_PUNISHMENT_REQ, UidType );
#endif SERV_NEW_PUNISHMENT

		//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
		_CASE( DBE_GET_ANTI_ADDICTION_INFO_REQ, UidType );
		CASE( DBE_UPDATE_ANTI_ADDICTION_INFO );
#endif SERV_ANTI_ADDICTION_SYSTEM
		//}}
		//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
		CASE( DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_REQ );
		CASE( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ );
		CASE( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ );
		CASE( DBE_LOCAL_RANKING_USER_INFO_WRITE_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
		//}}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-22	// 박세훈
		CASE( DBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT );
		CASE_NOPARAM( DBE_BLOCK_COUNT_CHECK_INFO_READ_REQ );
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
		CASE( DBE_HACKING_USER_CHECK_COUNT_INFO_REQ );
		CASE( DBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT );
		CASE( DBE_ACCOUNT_BLOCK_NOT );
#endif // SERV_HACKING_USER_CHECK_COUNT

#ifdef SERV_ENTRY_POINT
        CASE( DBE_GET_PURE_SECOND_SECURITY_INFO_REQ );
#endif SERV_ENTRY_POINT
    default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KGSAccountDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KGSAccountDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
    SendToUser( nTo, usEventID, char() );
}

int KGSAccountDBThread::GetMessengerSN( IN UidType iUnitUID, OUT u_int& uiKNMSerialNum )
{
	int iOK = NetError::ERR_ODBC_01;

	const int iServerGroupID = KBaseServer::GetKObj()->GetServerGroupID();

	// 넥슨 메신저용 CharacterSN.
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
	DO_QUERY( L"exec dbo.P_MessengerSN_SEL", L"%d, %d", % iServerGroupID % iUnitUID );
#else //SERV_GLOBAL_COMMON
	DO_QUERY( L"exec dbo.P_MessengerSN_SEL_KR", L"%d, %d", % iServerGroupID % iUnitUID );
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_MessengerSN", L"%d, %d", % iServerGroupID % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		int iTemp = 0;
		FETCH_DATA( iTemp );

		uiKNMSerialNum = static_cast<u_int>(iTemp);

		m_kODBC.EndFetch();
	}
	else
	{
		iOK = NetError::ERR_SELECT_UNIT_04;
		goto end_proc;
	}

	iOK = NetError::NET_OK;

end_proc:
	return iOK;
}

//{{ 2009. 12. 22  최육사	서버군확장
IMPL_ON_FUNC( ELOG_STAT_PLAY_TIME )
{
	int iOK = NetError::ERR_ODBC_01;

	//{{ 2011. 08. 09	최육사	머신ID 블럭기능
#ifdef SERV_MACHINE_ID_BLOCK
	//{{ 2012. 11. 14	박세훈	Field PT 로그 추가
#ifdef SERV_FIELD_PLAY_TIME_LOG

#ifdef SERV_RENEWAL_SP
	bool bPCBangFlag = kPacket_.m_bIsPcbang;	//m_bIsPcbang 해외도 필요해서 오과금 define 밖으로 뺌
	int iChannelCode = 0;
	
#ifdef SERV_CONNECT_LOG_CHANNELING
	iChannelCode = static_cast< int >(kPacket_.m_ucChannelCode);
#endif //SERV_CONNECT_LOG_CHANNELING

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_AConnectLog_INS", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		% bPCBangFlag	
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		% bPCBangFlag	
		);
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_RENEWAL_SP
#ifdef SERV_CONNECT_LOG_CHANNELING
	int iChannelCode = static_cast< int >(kPacket_.m_ucChannelCode);

	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		);
#else SERV_CONNECT_LOG_CHANNELING
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDungeonPlayTime
		% kPacket_.m_iFieldPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		);

#endif SERV_CONNECT_LOG_CHANNELING
#endif //SERV_RENEWAL_SP
#else

#ifdef SERV_CONNECT_LOG_CHANNELING
	int iChannelCode = static_cast< int >(kPacket_.m_ucChannelCode);

	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\', %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		% iChannelCode
		);
#else SERV_CONNECT_LOG_CHANNELING
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		% kPacket_.m_wstrMachineID
		);

#endif SERV_CONNECT_LOG_CHANNELING

#endif SERV_FIELD_PLAY_TIME_LOG
	//}}
#else
	DO_QUERY( L"exec dbo.mup_insert_user_play_time_all", L"%d, N\'%s\', N\'%s\', N\'%s\', %d, %d, %d, %d, %d, %d, %d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_wstrIP
		% kPacket_.m_wstrLoginTime
		% kPacket_.m_wstrLogoutTime
		% kPacket_.m_bLoginFail
		% kPacket_.m_bLogoutFail
		% kPacket_.m_iTotalPlayTime
		% kPacket_.m_iPvpPlayTime
		% kPacket_.m_iDisconnectReason
		% kPacket_.m_iDisconnectUserFSM
		% 0 //kPacket_.m_iArcadePlayTime
		% kPacket_.m_iServerGroupID
		);
#endif SERV_MACHINE_ID_BLOCK
	//}}	

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	LOG_SUCCESS( iOK == NetError::NET_OK )
		<< BUILD_LOG( iOK )
		<< END_LOG;
}
//}}

IMPL_ON_FUNC_NOPARAM( DBE_UPDATE_LOGIN_TIME_NOT )
{
    int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_UPD_LoginTime", L"%d", % FIRST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.mup_update_login_time", L"%d", % FIRST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );

        m_kODBC.EndFetch();
    }

end_proc:
    if( iOK != NetError::NET_OK )
    {
        START_LOG( cerr, L"최근 로그인 시각 업데이트 실패." )
            << BUILD_LOG( iOK )
            << BUILD_LOG( FIRST_SENDER_UID )
            << END_LOG;
    }

    return;
}

IMPL_ON_FUNC( EGS_CHANGE_OPTION_PLAY_GUIDE_REQ )
{
    KPacketOK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_UPD_PlayGuide", L"%d, %d", % LAST_SENDER_UID % kPacket_.m_bOn );
#else //SERV_ALL_RENEWAL_SP
    DO_QUERY( L"exec dbo.mup_set_play_guide", L"%d, %d", % LAST_SENDER_UID % kPacket_.m_bOn );
#endif //SERV_ALL_RENEWAL_SP

    int iOK;
    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( iOK );
        m_kODBC.EndFetch();

		switch( iOK )
		{
		case 0:
			kPacket.m_iOK = NetError::NET_OK;
			break;

		case -1:
			kPacket.m_iOK = NetError::ERR_CLIENT_STATE;
			break;

		case -11:
			kPacket.m_iOK = NetError::ERR_NEW_ACCOUNT_02;
			break;			
		}
    }

end_proc:
    SendToUser( LAST_SENDER_UID, EGS_CHANGE_OPTION_PLAY_GUIDE_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ADMIN_CHANGE_AUTH_LEVEL_REQ, KEGS_ADMIN_CHANGE_AUTH_LEVEL_REQ )
{
	KEGS_ADMIN_CHANGE_AUTH_LEVEL_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserAuthority_MER", L"N\'%s\', %d", % kPacket_.m_wstrUserID % (int)kPacket_.m_cAuthLevel );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_set_user_authority", L"N\'%s\', %d", % kPacket_.m_wstrUserID % (int)kPacket_.m_cAuthLevel );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_03;
			break;

		case -2:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_04;
			break;

		case -11:
		case -12:
			kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_05;
			break;
		}
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_CHANGE_AUTH_LEVEL_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ADMIN_GET_AUTH_LEVEL_LIST_REQ, KEGS_ADMIN_GET_AUTH_LEVEL_LIST_REQ )
{
	KEGS_ADMIN_GET_AUTH_LEVEL_LIST_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Muser_SEL_Authority", L"%d", % (int)kPacket_.m_cAuthLevel );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_authority_list", L"%d", % (int)kPacket_.m_cAuthLevel );
#endif //SERV_ALL_RENEWAL_SP

	while( m_kODBC.Fetch() )
	{
		std::wstring wstrUserID;
		std::wstring wstrUserName;
		FETCH_DATA( wstrUserID >> wstrUserName );

		wstrUserID += L" : ";
		wstrUserID += wstrUserName;

		kPacket.m_vecUserIDList.push_back( wstrUserID );
	}

	if( kPacket.m_vecUserIDList.empty() == true )
		kPacket.m_iOK = NetError::ERR_ADMIN_COMMAND_06;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ADMIN_GET_AUTH_LEVEL_LIST_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ACCOUNT_CREATE_UNIT_REQ, KEGS_CREATE_UNIT_ACK )
{
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"사내 테스트 버전 게임서버에서 새로운 캐릭터가 생성되었습니다!" )
			<< BUILD_LOG( kPacket_.m_kUnitInfo.m_wstrNickName );

		kPacket_.m_iOK = NetError::NET_OK;
	}
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
		DO_QUERY( L"exec dbo.P_MessengerSN_INS", L"%d, %d, %d",
			% LAST_SENDER_UID
			% GetKGameServer()->GetServerGroupID()
			% kPacket_.m_kUnitInfo.m_nUnitUID
			);
#else //SERV_GLOBAL_COMMON
		DO_QUERY( L"exec dbo.P_MessengerSN_INS_KR", L"%d, %d, %d",
			% LAST_SENDER_UID
			% GetKGameServer()->GetServerGroupID()
			% kPacket_.m_kUnitInfo.m_nUnitUID
			);
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_insert_MessengerSN", L"%d, %d, %d",
			% LAST_SENDER_UID
			% GetKGameServer()->GetServerGroupID()
			% kPacket_.m_kUnitInfo.m_nUnitUID
			);
#endif //SERV_ALL_RENEWAL_SP

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			kPacket_.m_iOK = NetError::ERR_CREATE_UNIT_07;
			goto end_proc;
		}
	}
	//#endif SERV_VERSION_FLAG
	//}}

end_proc:
	LOG_SUCCESS( kPacket_.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iOK) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_CREATE_UNIT_ACK, kPacket_ );
}

//{{ 2012. 12. 10  캐릭터 선택 패킷 분할 - 김민성
#ifdef SERV_SELECT_UNIT_PACKET_DIVISION
_IMPL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_REQ, KDBE_SELECT_UNIT_ACK )
#else
_IMPL_ON_FUNC( DBE_ACCOUNT_SELECT_UNIT_REQ, KEGS_SELECT_UNIT_ACK )
#endif SERV_SELECT_UNIT_PACKET_DIVISION
//}}
{
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"사내 테스트 버전 게임서버에서 캐릭터가 선택되었습니다!" )
			<< BUILD_LOG( kPacket_.m_kUnitInfo.m_wstrNickName );

		kPacket_.m_iOK = NetError::NET_OK;
		kPacket_.m_kUnitInfo.m_uiKNMSerialNum = static_cast<u_int>(rand()); // dummy값을 넣자
	}
	else
	{
		kPacket_.m_iOK = GetMessengerSN( kPacket_.m_kUnitInfo.m_nUnitUID, kPacket_.m_kUnitInfo.m_uiKNMSerialNum );

		if( kPacket_.m_kUnitInfo.m_uiKNMSerialNum == 0 )
		{
			START_LOG( cerr, L"넥슨 메신저SN값이 이상합니다. 새로운 메신저SN값을 발급합니다." )
				<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
				<< BUILD_LOG( kPacket_.m_kUnitInfo.m_uiKNMSerialNum )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// 넥슨 메신저SN 새로 발급
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kUnitInfo.m_nUnitUID
				);
#else //SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS_KR", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_insert_MessengerSN", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_ALL_RENEWAL_SP		

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"넥슨메신저SN 재발급 실패!?" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( GetKGameServer()->GetServerGroupID() )
					<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
					<< END_LOG;

				// 게임은 진행할수 있도록 ok값 보내자
				kPacket_.m_iOK = NetError::NET_OK;
			}
			else
			{
				// 발급되었으면 SN을 받자!
				kPacket_.m_iOK = GetMessengerSN( kPacket_.m_kUnitInfo.m_nUnitUID, kPacket_.m_kUnitInfo.m_uiKNMSerialNum );
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		kPacket_.m_iEventMoney = 0;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_SEL ", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_GET ", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iEventMoney );

			m_kODBC.EndFetch();
		}
	}
#endif // SERV_EVENT_MONEY

	//{{ 2012. 10. 10	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		ReadBingoEvent( anTrace_, kPacket_.m_kBingoEvent );
		kPacket_.m_iOK = kPacket_.m_kBingoEvent.m_iOK;

		if( kPacket_.m_kBingoEvent.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"DB에서 빙고 이벤트 정보 읽기 실패!!" )
				<< BUILD_LOG( kPacket_.m_kBingoEvent.m_iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< END_LOG;
		}
	}
#endif SERV_EVENT_BINGO
	//}}

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-31
	if( ( kPacket_.m_iOK == NetError::NET_OK ) && ( kPacket_.m_kLocalRankingUserInfo.m_iUserUID == 0 ) )
	{
		// 랭커 유저 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			kPacket_.m_iOK = NetError::ERR_ODBC_01;

			FETCH_DATA( kPacket_.m_iOK
				>> kPacket_.m_kLocalRankingUserInfo.m_byteFilter
				>> kPacket_.m_kLocalRankingUserInfo.m_iCategory
				>> kPacket_.m_kLocalRankingUserInfo.m_byteGender
				>> kPacket_.m_kLocalRankingUserInfo.m_iBirth
				>> kPacket_.m_kLocalRankingUserInfo.m_wstrProfile
				>> kPacket_.m_wstrLocalRankingUserInfoUpdated
				);
			m_kODBC.EndFetch();
		}
	}
#endif	// SERV_LOCAL_RANKING_SYSTEM

	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	// 계정 퀘스트 정보를 읽어 와야 한다.
	{
		//////////////////////////////////////////////////////////////////////////
		// 완료한 계정 퀘스트를 받아 온다.
		CTime tCompleteDate;
		std::wstring wstrCompleteDate;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// 완료 날짜 변환
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_vecAccountCompleteQuest.push_back( kInfo );
		}
		//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// 완료 날짜 변환
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_vecAccountCompleteQuest.push_back( kInfo );
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

		//////////////////////////////////////////////////////////////////////////
		// 진행 중인(수락한) 계정 퀘스트를 받아 온다.
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KQuestInstance		kQuest;
			KSubQuestInstance	kSub[5];
			kQuest.m_OwnorUnitUID = kPacket_.m_kUnitInfo.m_nUnitUID;

			FETCH_DATA( kQuest.m_iID
				>> kSub[0].m_ucClearData
				>> kSub[1].m_ucClearData
				>> kSub[2].m_ucClearData
				>> kSub[3].m_ucClearData
				>> kSub[4].m_ucClearData
				);
			for( int i = 0; i < 5; i++ )
				kQuest.m_vecSubQuestInstance.push_back( kSub[i] );

			kPacket_.m_vecAccountQuesting.push_back( kQuest );
		}
	}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	LOG_SUCCESS( kPacket_.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_iOK ) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_kUnitInfo.m_nUnitUID )
		<< END_LOG;
	//#endif SERV_VERSION_FLAG
	//}}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_SELECT_UNIT_ACK, kPacket_ );
}

//{{ 2009. 6. 4  최육사	캐릭터 선택
_IMPL_ON_FUNC( DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_REQ, KDBE_CHANNEL_CHANGE_GAME_SELECT_UNIT_ACK )
{
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"사내 테스트 버전 게임서버에서 채널 이동으로 인해 캐릭터가 자동으로 재선택되었습니다!" )
			<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_wstrNickName );

		kPacket_.m_kSelectUnitAck.m_iOK = NetError::NET_OK;

		kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum = static_cast<u_int>(rand()); // dummy값을 넣자

	}
	else
	{
		kPacket_.m_kSelectUnitAck.m_iOK = GetMessengerSN( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID, kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum );

		if( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum == 0 )
		{
			START_LOG( cerr, L"넥슨 메신저SN값이 이상합니다. 새로운 메신저SN값을 발급합니다." )
				<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID )
				<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum )
				<< END_LOG;

			//////////////////////////////////////////////////////////////////////////
			// 넥슨 메신저SN 새로 발급
			int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
#ifdef SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID
				);
#else //SERV_GLOBAL_COMMON
			DO_QUERY( L"exec dbo.P_MessengerSN_INS_KR", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_GLOBAL_COMMON
#else //SERV_ALL_RENEWAL_SP

			DO_QUERY( L"exec dbo.mup_insert_MessengerSN", L"%d, %d, %d",
				% LAST_SENDER_UID
				% GetKGameServer()->GetServerGroupID()
				% kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID
				);
#endif //SERV_ALL_RENEWAL_SP

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"넥슨메신저SN 재발급 실패!?" )
					<< BUILD_LOG( LAST_SENDER_UID )
					<< BUILD_LOG( GetKGameServer()->GetServerGroupID() )
					<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID )
					<< END_LOG;

				// 게임은 진행할수 있도록 ok값 보내자
				kPacket_.m_kSelectUnitAck.m_iOK = NetError::NET_OK;
			}
			else
			{
				// 발급되었으면 SN을 받자!
				kPacket_.m_kSelectUnitAck.m_iOK = GetMessengerSN( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID, kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_uiKNMSerialNum );
			}
			//////////////////////////////////////////////////////////////////////////
		}
	}
//#endif SERV_VERSION_FLAG
//}}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
	if( kPacket_.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
	{
		kPacket_.m_kSelectUnitAck.m_iEventMoney = 0;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_SEL ", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEventPoint_GET ", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_kSelectUnitAck.m_iEventMoney );

			m_kODBC.EndFetch();
		}
	}
#endif // SERV_EVENT_MONEY

	//{{ 2012. 10. 10	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
	if( kPacket_.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
	{
		ReadBingoEvent( anTrace_, kPacket_.m_kSelectUnitAck.m_kBingoEvent );
		kPacket_.m_kSelectUnitAck.m_iOK = kPacket_.m_kSelectUnitAck.m_kBingoEvent.m_iOK;
	}
#endif SERV_EVENT_BINGO
	//}}

	//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
	// 계정 퀘스트 정보를 읽어 와야 한다.
	{
		//////////////////////////////////////////////////////////////////////////
		// 완료한 계정 퀘스트를 받아 온다.
		CTime tCompleteDate;
		std::wstring wstrCompleteDate;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// 완료 날짜 변환
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_kSelectUnitAck.m_vecAccountCompleteQuest.push_back( kInfo );
		}

		//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KCompleteQuestInfo kInfo;
			kInfo.m_iCompleteCount = 1;

			FETCH_DATA( kInfo.m_iQuestID
				>> wstrCompleteDate );

			// 완료 날짜 변환
			LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );
			kInfo.m_tCompleteDate = tCompleteDate.GetTime();

			kPacket_.m_kSelectUnitAck.m_vecAccountCompleteQuest.push_back( kInfo );
		}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
		//}

		//////////////////////////////////////////////////////////////////////////
		// 진행 중인(수락한) 계정 퀘스트를 받아 온다.
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_SEL", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_GET", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
		while( m_kODBC.Fetch() )
		{
			KQuestInstance		kQuest;
			KSubQuestInstance	kSub[5];
			kQuest.m_OwnorUnitUID = kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID;

			FETCH_DATA( kQuest.m_iID
				>> kSub[0].m_ucClearData
				>> kSub[1].m_ucClearData
				>> kSub[2].m_ucClearData
				>> kSub[3].m_ucClearData
				>> kSub[4].m_ucClearData
				);
			for( int i = 0; i < 5; i++ )
				kQuest.m_vecSubQuestInstance.push_back( kSub[i] );

			kPacket_.m_kSelectUnitAck.m_vecAccountQuesting.push_back( kQuest );
		}
	}
#endif SERV_ACCOUNT_MISSION_SYSTEM
	//}}

	LOG_SUCCESS( kPacket_.m_kSelectUnitAck.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr( kPacket_.m_kSelectUnitAck.m_iOK ) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_kSelectUnitAck.m_kUnitInfo.m_nUnitUID )
		<< END_LOG;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHANNEL_CHANGE_ACCOUNT_SELECT_UNIT_ACK, kPacket_ );
}
//}}

#ifdef SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_FINAL_DELETE_UNIT_ACK )
#else //SERV_UNIT_WAIT_DELETE
_IMPL_ON_FUNC( DBE_ACCOUNT_DELETE_UNIT_REQ, KEGS_DELETE_UNIT_ACK )
#endif //SERV_UNIT_WAIT_DELETE
{
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"사내 테스트 버전 게임서버에서 캐릭터가 삭제 되었습니다!" )
			<< BUILD_LOG( kPacket_.m_wstrNickName );

		kPacket_.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket_.m_iOK = GetMessengerSN( kPacket_.m_iUnitUID, kPacket_.m_uiKNMSerialNum );
	}
	//#endif SERV_VERSION_FLAG
	//}}

	LOG_SUCCESS( kPacket_.m_iOK == NetError::NET_OK )
		<< BUILD_LOG( NetError::GetErrStr(kPacket_.m_iOK) )
		<< BUILD_LOG( LAST_SENDER_UID )
		<< BUILD_LOG( kPacket_.m_iUnitUID )
		<< END_LOG;

	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_DELETE_UNIT_ACK, kPacket_ );
}

//{{ 2008. 3. 28  최육사  추천인
IMPL_ON_FUNC_NOPARAM( DBE_RECOMMEND_USER_NOT )
{
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUser_UPD_IsRecommend", L"%d", % LAST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_isrecommend", L"%d", % LAST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
	int iOK = NetError::ERR_ODBC_01;

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"MUser 테이블의 추천인 flag 갱신 실패!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< END_LOG;
	}
}
//}}

_IMPL_ON_FUNC( DBE_AGREE_HACK_USER_REQ, UidType )
{
	KEGS_AGREE_HACK_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserHackerList_UPD", L"%d, %d", % kPacket_ % 1 );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_hackerlist", L"%d, %d", % kPacket_ % 1 );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = NetError::ERR_HACK_USER_01;

		START_LOG( cerr, L"해킹 동의 데이터 업데이트 실패.!" )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_AGREE_HACK_USER_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_REPORT_HACK_USER_NOT, UidType )
{
	int iOK = NetError::ERR_ODBC_01;

	// 이메일 감시 대상자 등록
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserHackerList_INS", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_hackerlist", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"해킹유저 신고등록 실패.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_ )
			<< END_LOG;
	}
}

_IMPL_ON_FUNC( DBE_GET_MY_MESSENGER_SN_REQ, KEGS_GET_MY_MESSENGER_SN_REQ )
{
	KEGS_GET_MY_MESSENGER_SN_ACK kPacket;
	
	//{{ 2012. 05. 10	최육사	서버 버전
	//#ifdef SERV_VERSION_FLAG
	if( KSimLayer::GetKObj()->GetVersionFlag() == KSimLayer::VF_INTERNAL )
	{
		START_LOG( cout, L"사내 테스트 버전 게임서버에서 메신저SN을 요청하였습니다!" )
			<< BUILD_LOG( kPacket_ );

		kPacket.m_iOK = NetError::NET_OK;
	}
	else
	{
		kPacket.m_iOK = GetMessengerSN( kPacket_, kPacket.m_uiKNMSerialNum );
	}
	//#endif SERV_VERSION_FLAG
	//}}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"넥슨 메신져 SN 요청 실패.!" )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( NetError::GetErrStr(kPacket.m_iOK) )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_GET_MY_MESSENGER_SN_ACK, kPacket );
}

//{{ 2010. 11. 2	최육사	IP주소 계정 가입 로그
#ifdef SERV_IP_ACCOUNT_LOG
IMPL_ON_FUNC( DBE_JOIN_ACCOUNT_CHECK_IP_LOG_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_HackingUser_INS", L"%d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_wstrLoginIP );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_hacking_user", L"%d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_wstrLoginIP );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}
	
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"특정 IP로 계정 생성되는 정보 로그 남기기 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wstrLoginIP )
			<< END_LOG;
	}
}
#endif SERV_IP_ACCOUNT_LOG
//}}

//{{ 2011. 05. 02  김민성	2차 보안 시스템
#ifdef SERV_SECOND_SECURITY

IMPL_ON_FUNC( DBE_GET_PURE_SECOND_SECURITY_INFO_REQ )
{
    KDBE_GET_SECOND_SECURITY_INFO_ACK kPacket;
    kPacket.m_iOK = NetError::ERR_ODBC_01;
    kPacket.m_iFailedCount = 0;
    kPacket.m_bUseSecondPW = false;

    //{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
    CTime tCurr = CTime::GetCurrentTime();
    CTime tEndDate;
#endif SERV_COME_BACK_USER_REWARD
    //}} 

    // 2차 비번 얻기 : kPacket_ --> UserUID
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_second_pw_Info", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP

    if( m_kODBC.BeginFetch() )
    {
        FETCH_DATA( kPacket.m_bUseSecondPW
            >> kPacket.m_wstrSecondPW
            >> kPacket.m_iFailedCount
            >> kPacket.m_wstrLastAuthDate );

        m_kODBC.EndFetch();
    }

    if( kPacket.m_bUseSecondPW == true && kPacket.m_iFailedCount >= 10 )	// 2차 비밀번호 입력 실패 10회 이상
    {
        kPacket.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT;

        START_LOG( clog, L"2차 비밀번호 입력 실패 10회 이상" )
            << BUILD_LOG( kPacket.m_bUseSecondPW )
            << BUILD_LOG( kPacket.m_iFailedCount );
        goto end_proc;
    }

    kPacket.m_iOK = NetError::NET_OK;

end_proc:
    SendToUser( LAST_SENDER_UID, DBE_GET_PURE_SECOND_SECURITY_INFO_ACK, kPacket );

}

IMPL_ON_FUNC( DBE_GET_SECOND_SECURITY_INFO_REQ )
{
	KDBE_GET_SECOND_SECURITY_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iFailedCount = 0;
	kPacket.m_bUseSecondPW = false;

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	CTime tCurr = CTime::GetCurrentTime();
	CTime tEndDate;
#endif SERV_COME_BACK_USER_REWARD
	//}} 

	// 2차 비번 얻기 : kPacket_ --> UserUID
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_second_pw_Info", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_bUseSecondPW
			     >> kPacket.m_wstrSecondPW
				 >> kPacket.m_iFailedCount
				 >> kPacket.m_wstrLastAuthDate );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_bUseSecondPW == true && kPacket.m_iFailedCount >= 10 )	// 2차 비밀번호 입력 실패 10회 이상
	{
		kPacket.m_iOK = NetError::ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT;

		START_LOG( clog, L"2차 비밀번호 입력 실패 10회 이상" )
			<< BUILD_LOG( kPacket.m_bUseSecondPW )
			<< BUILD_LOG( kPacket.m_iFailedCount );
		goto end_proc;
	}

	//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
	kPacket.m_iRewardStep = 0;
	kPacket.m_bIsFirst = false;
	kPacket.m_bIsComeBackUser = false;

	// DB에 복귀 보상 정보가 있는지 확인한다.
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_Comebackuser_info", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_bIsComeBackUser
			>> kPacket.m_wstrComeBackBuffEndDate );

		m_kODBC.EndFetch();
	}

	START_LOG( clog, L"유저의 휴면 복귀 정보를 DB에서 얻어오기 성공!!" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
		<< BUILD_LOG( kPacket.m_bIsComeBackUser )
		<< BUILD_LOG( kPacket.m_iRewardStep  )
		<< END_LOG;

	// DB에 휴면 유저 정보가 있다.
	if( kPacket.m_bIsComeBackUser == true )
	{
		if( KncUtil::ConvertStringToCTime( kPacket.m_wstrComeBackBuffEndDate , tEndDate ) == false )
		{
			START_LOG( cerr, L"날짜 변환 실패!!!" )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
				<< END_LOG;
			
			// 이전 시간으로 최화 시킴
			tEndDate = CTime( 2000, 1, 1, 0, 0, 0 );
		}

		// 복귀 버프 끝나는 시간이 현재보다 이전(버프 없음)
		if( tEndDate < tCurr )
		{
			kPacket.m_bIsComeBackUser = false;	// 버프 없도록 한다.

			// LastConnection 정보를 이용여 복귀 유저인지 판단
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_MUserComeback_Logout_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_get_comeback_logout", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_wstrLastConnectDate );

				m_kODBC.EndFetch();
			}

			// 시간 정보가 비어 있지 않다면
			if( kPacket.m_wstrLastConnectDate.empty() == false )
			{
				// 휴면 유저인지 판단하여 맞다면 보상 등급 확인
				if( CheckIsComeBackUser( kPacket.m_wstrLastConnectDate, kPacket.m_iRewardStep, kPacket.m_wstrComeBackBuffEndDate, kPacket_.m_mapComeBackRewardCondition ) == true )
				{
					kPacket.m_bIsFirst = true;
					kPacket.m_bIsComeBackUser = true;	// 복귀 유저가 맞다!

					START_LOG( clog, L"마지막 접속 시간으로 휴면 유저 판단 및 보상 등급 판단" )
						<< BUILD_LOG( kPacket_.m_iUserUID )
						<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
						<< BUILD_LOG( kPacket.m_bIsComeBackUser )
						<< BUILD_LOG( kPacket.m_bIsFirst )
						<< BUILD_LOG( kPacket.m_iRewardStep )
						<< END_LOG;
				}
				else
				{
					// 복귀 유저가 아니다 데이타 초기화
					kPacket.m_iRewardStep = 0;
					kPacket.m_bIsFirst = false;
					kPacket.m_bIsComeBackUser = false;
					kPacket.m_wstrComeBackBuffEndDate = L"";

					// DB에 기록하자 - 버프가 꺼졌다고
					int _iOK = NetError::ERR_ODBC_00;
#ifdef SERV_ALL_RENEWAL_SP
					DO_QUERY( L"exec dbo.P_MUserComeback_UPD", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
					DO_QUERY( L"exec dbo.mup_update_Comebackuser_finish", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
					if( m_kODBC.BeginFetch() )
					{
						FETCH_DATA( _iOK );

						m_kODBC.EndFetch();
					}

					if( _iOK != NetError::NET_OK )
					{
						START_LOG( cerr, L"잘못된 복귀 유저 정보 입력실패!!" )
							<< BUILD_LOG( kPacket_.m_iUserUID )
							<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
							<< BUILD_LOG( kPacket.m_bIsComeBackUser )
							<< BUILD_LOG( _iOK )
							<< END_LOG;
					}

					START_LOG( clog, L"마지막 접속 시간을 보고 휴면 복귀 유저거 아니라고 판단" )
						<< BUILD_LOG( kPacket_.m_iUserUID )
						<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
						<< BUILD_LOG( kPacket.m_bIsComeBackUser )
						<< BUILD_LOG( kPacket.m_bIsFirst )
						<< BUILD_LOG( kPacket.m_iRewardStep )
						<< END_LOG;
				}
			}
		}
		// 복귀 버프 끝나는 시간이 현재보다 이후(버프 있음)
		else
		{
			// 복귀 유저가 아니다
			kPacket.m_bIsFirst = false;
			kPacket.m_bIsComeBackUser = true;

			START_LOG( clog, L"이미 휴면 복귀 유저!!" )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
				<< BUILD_LOG( kPacket.m_bIsComeBackUser )
				<< BUILD_LOG( kPacket.m_bIsFirst )
				<< BUILD_LOG( kPacket.m_iRewardStep )
				<< END_LOG;
			//{{ 2012. 05. 16	박세훈	첫 접속 시 가이드 라인 띄워주기
#ifdef SERV_EVENT_GUIDELINE_POPUP
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_MUserComeback_Logout_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.mup_get_comeback_logout", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket.m_wstrLastConnectDate );
				m_kODBC.EndFetch();
			}
#endif SERV_EVENT_GUIDELINE_POPUP
			//}}
		}
	}
	// DB에 휴면 유저 정보가 없다.
	else
	{
		// LastConnection 정보를 이용하여 복귀 유저인지 판단
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserComeback_Logout_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.mup_get_comeback_logout", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_wstrLastConnectDate );

			m_kODBC.EndFetch();
		}

		// 시간 정보가 비어 있지 않다면
		if( kPacket.m_wstrLastConnectDate.empty() == false )
		{
			// 휴면 유저인지 판단하여 맞다면 보상 등급 확인
			if( CheckIsComeBackUser( kPacket.m_wstrLastConnectDate, kPacket.m_iRewardStep, kPacket.m_wstrComeBackBuffEndDate, kPacket_.m_mapComeBackRewardCondition ) == true )
			{
				// 복귀 유저이다.
				kPacket.m_bIsFirst = true;
				kPacket.m_bIsComeBackUser = true;

				START_LOG( clog, L"마지막 접속 시간으로 휴면 유저 판단 및 보상 등급 판단" )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
					<< BUILD_LOG( kPacket.m_bIsComeBackUser )
					<< BUILD_LOG( kPacket.m_bIsFirst )
					<< BUILD_LOG( kPacket.m_iRewardStep )
					<< END_LOG;
			}
			else
			{
				// 복귀 유저가 아니다 데이타 초기화
				kPacket.m_iRewardStep = 0;
				kPacket.m_bIsFirst = false;
				kPacket.m_bIsComeBackUser = false;
				kPacket.m_wstrComeBackBuffEndDate = L"";

				START_LOG( clog, L"마지막 접속 시간이 없다. 새 유저가 접속하였다." )
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< BUILD_LOG( kPacket.m_wstrComeBackBuffEndDate )
					<< BUILD_LOG( kPacket.m_bIsComeBackUser )
					<< BUILD_LOG( kPacket.m_bIsFirst )
					<< BUILD_LOG( kPacket.m_iRewardStep )
					<< END_LOG;
			}
		}
	}

#ifdef SERV_COME_BACK_USER_NOT_USE
	kPacket.m_iRewardStep = 0;
	kPacket.m_bIsFirst = false;
	kPacket.m_bIsComeBackUser = false;
	kPacket.m_wstrComeBackBuffEndDate = L"";
#endif // SERV_COME_BACK_USER_NOT_USE

#endif SERV_COME_BACK_USER_REWARD
	//}} 

	//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_SEL", L"%d", %kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_GET", L"%d", %kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			// 해당 유저가 없을 시 -1 반환
			FETCH_DATA( kPacket.m_wstrWinterVacationEventRegDate
				>>	kPacket.m_iWinterVacationEventCount
				);
			m_kODBC.EndFetch();
		}
		else
		{
			START_LOG( cerr, L"쿼리 호출 실패")
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< END_LOG;
			kPacket.m_iOK = NetError::ERR_ODBC_01;
			goto end_proc;
		}

		CTime tCurrentTime = CTime::GetCurrentTime();

		// kPacket.m_iWinterVacationEventCount < 0 이라는 것은 현재 DB에 기록되어 있지 않은 유저라는 뜻이다.
		// '2012-12-13 00:00:00' ~ '2013-02-14 07:00:00' 기간 동안 복귀 유저로 들어왔다면 이벤트 수행 자격을 충족함
		if(	( kPacket.m_iWinterVacationEventCount < 0 )			&&
			( kPacket.m_bIsComeBackUser == true )				&&
			( CTime( 2012, 12, 13, 0, 0, 0 ) <= tCurrentTime )	&&
			( tCurrentTime < CTime( 2013, 2, 14, 7, 0, 0 ) )
			)
		{
			kPacket.m_wstrWinterVacationEventRegDate = tCurrentTime.Format( _T("%Y-%m-%d %H:%M:%S") );

			DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_INT", L"%d, N\'%s\'", % kPacket_.m_iUserUID % kPacket.m_wstrWinterVacationEventRegDate );
			if( m_kODBC.BeginFetch() )
			{
				// 입력 성공 시 0, 실패 시 -1 반환
				FETCH_DATA( kPacket.m_iWinterVacationEventCount );
				m_kODBC.EndFetch();

				if( kPacket.m_iWinterVacationEventCount < 0 )
				{
					START_LOG( cerr, L"2012 겨울 방학 전야 이벤트 insert 쿼리 실패" )
						<< BUILD_LOG( kPacket_.m_iUserUID )
						<< END_LOG;
					kPacket.m_iOK = NetError::ERR_ODBC_01;
					goto end_proc;
				}
			}
			else
			{
				START_LOG( cerr, L"쿼리 호출 실패")
					<< BUILD_LOG( kPacket_.m_iUserUID )
					<< END_LOG;
				kPacket.m_iOK = NetError::ERR_ODBC_01;
				goto end_proc;
			}
		}
	}
#endif SERV_2012_WINTER_VACATION_EVENT
	//}}

		//{{  2011.11.08     김민성    버블파이터 공동 프로모션 이벤트
#ifdef SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
	//////////////////////////////////////////////////////////////////////////
	//버파 이벤트 용 특공대 정보 얻어오기
	//////////////////////////////////////////////////////////////////////////
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Elsword_Bubble_Event_SEL", L"N\'%s\'", % kPacket_.m_wstrID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_elsword_bubble_team", L"N\'%s\'", % kPacket_.m_wstrID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iBF_Team );

		m_kODBC.EndFetch();
	}
#endif SERV_BUBBLE_FIGHTER_TOGETHER_EVENT
		//}}

	//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_SEL", L"%d", % kPacket_.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_GET", L"%d", % kPacket_.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int					iResult = NetError::ERR_ODBC_01;
		int					iItemID;
		KDailyGiftBoxInfo	kDailyGiftBoxInfo;

		FETCH_DATA( iResult
			>> kDailyGiftBoxInfo.m_iUnitUID
			>> iItemID
			>> kDailyGiftBoxInfo.m_iState
			>> kDailyGiftBoxInfo.m_wstrRegDate
			);

		if( iResult == NetError::NET_OK )
		{
			CTime tRegDate;
			if( KncUtil::ConvertStringToCTime( kDailyGiftBoxInfo.m_wstrRegDate, tRegDate ) == true )
			{
				kPacket.m_mmapDailyGiftBoxList.insert( std::multimap<int, KDailyGiftBoxInfo>::value_type( iItemID, kDailyGiftBoxInfo ) );
			}
		}
	}
#endif SERV_EVENT_DAILY_GIFT_BOX
	//}}
	
	//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
	{
		short sServerGroupID = static_cast<short>(KBaseServer::GetKObj()->GetServerGroupID());

		DO_QUERY( L"exec dbo.P_MEventReward_GET", L"%d, %d", %LAST_SENDER_UID %sServerGroupID );

		while( m_kODBC.Fetch() )
		{
			KFirstSelectUnitReward kRewardInfo;

			FETCH_DATA( kRewardInfo.m_iDBIndexID
					 >> kRewardInfo.m_iItemID
					 >> kRewardInfo.m_iQuantity
					 >> kRewardInfo.m_cGiveType
					 >> kRewardInfo.m_cRewardType)

			kPacket.m_vecFirstSelectUnitReward.push_back( kRewardInfo );
		}
	}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
		//}

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_SECOND_SECURITY_INFO_ACK, kPacket );

}

IMPL_ON_FUNC( DBE_SEUCCESS_SECOND_SECURITY_REQ )
{

	KDBE_SEUCCESS_SECOND_SECURITY_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_AuthSuccess", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrCurrTime );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_second_pw_authsuccess", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrCurrTime );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kAck.m_iOK 
				 >> kAck.m_wstrCurrTime );
		m_kODBC.EndFetch();
	}

end_proc:
	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2차 보안 인증 성공 기록 실패!!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< BUILD_LOG( kAck.m_wstrCurrTime )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_SEUCCESS_SECOND_SECURITY_ACK, kAck );
}

_IMPL_ON_FUNC( DBE_FAILED_SECOND_SECURITY_REQ, UidType )
{
	int iOK = NetError::ERR_ODBC_01;
	int iFailedCount = 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_AuthFailed", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_second_pw_authfailed", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> iFailedCount );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2차 보안 인증 실패 기록 실패!!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( iFailedCount )
			<< END_LOG;
	}
	else
	{
		if( iFailedCount >= 10 )
		{
			START_LOG( clog, L"실패 카운트가 10이상인 경우 10으로 보정!" )
				<< BUILD_LOG( kPacket_ )
				<< BUILD_LOG( iFailedCount )
				<< END_LOG;

			iFailedCount = 10;
		}
	}

end_proc:	
	SendToUser( LAST_SENDER_UID, DBE_FAILED_SECOND_SECURITY_ACK, iFailedCount );
}

IMPL_ON_FUNC( DBE_CREATE_SECOND_SECURITY_REQ )
{
	KDBE_CREATE_SECOND_SECURITY_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_MER_Create", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_create_second_pw", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}
	kPacket.m_wstrSecondPW = kPacket_.m_wstrSecondPW;

end_proc:
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2차 보안 생성 실패!!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserID )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

	}
	SendToUser( LAST_SENDER_UID, DBE_CREATE_SECOND_SECURITY_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DELETE_SECOND_SECURITY_REQ )
{
	KDBE_DELETE_SECOND_SECURITY_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_Delete", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_delete_second_pw", L"%d, N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrSecondPW );
#endif //SERV_ALL_RENEWAL_SP
	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2차 보안 삭제 실패!!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserID )
			<< BUILD_LOG( kPacket_.m_wstrSecondPW )
			<< END_LOG;

	}
	SendToUser( LAST_SENDER_UID, DBE_DELETE_SECOND_SECURITY_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_CHANGE_SECOND_SECURITY_PW_REQ )
{
	KDBE_CHANGE_SECOND_SECURITY_PW_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD", L"%d, N\'%s\', N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrOldSecondPW % kPacket_.m_wstrNewSecondPW );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_second_pw", L"%d, N\'%s\', N\'%s\'", % kPacket_.m_iUserID % kPacket_.m_wstrOldSecondPW % kPacket_.m_wstrNewSecondPW );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	kPacket.m_wstrSecondPW = kPacket_.m_wstrNewSecondPW;

end_proc:
	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"2차 보안 비밀번호 변경 실패!!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUserID )
			<< BUILD_LOG( kPacket_.m_wstrOldSecondPW  )
			<< BUILD_LOG( kPacket_.m_wstrNewSecondPW  )
			<< END_LOG;

	}
	SendToUser( LAST_SENDER_UID, DBE_CHANGE_SECOND_SECURITY_PW_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UPDATE_SECURITY_TYPE_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserSecondPW_UPD_Refresh_WEB", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iSecurityType );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_securitytype", L"%d, %d", % kPacket_.m_iUserUID % kPacket_.m_iSecurityType );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK ); // DB쿼리 규칙상 쿼리가 성공하면 무조건 0값이 반환
		m_kODBC.EndFetch();
	}
end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"유저 보안 사용 여부 DB 기록 실패!!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iSecurityType )
			<< END_LOG;
	}
}
#endif SERV_SECOND_SECURITY
//}}

//{{ 2011. 05. 27    김민성    휴면 복귀 유저 보상
#ifdef SERV_COME_BACK_USER_REWARD
bool KGSAccountDBThread::CheckIsComeBackUser( IN std::wstring& wstrLastDate, OUT int& iRewardStep, OUT std::wstring& wstrEndBuffDate, IN std::map< int, int >& mapComeBackRewardCondition )
{
	CTime tCurr = CTime::GetCurrentTime();
	CTime tLastConnect;

	if( KncUtil::ConvertStringToCTime( wstrLastDate, tLastConnect ) == false )
	{
		START_LOG( cerr, L"시간 변경 실패" )
			<< BUILD_LOG( wstrLastDate )
			<< END_LOG;
		return false;
	}

	CTimeSpan tSpanLastConnect = tCurr - tLastConnect;

	if( tSpanLastConnect.GetDays() < 0 )
	{
		CStringW cStrCurr, cStrLast;
		cStrCurr.Format(L"%d/%d/%d %d:%d:%d", tCurr.GetYear(), tCurr.GetMonth(), tCurr.GetDay(), tCurr.GetHour(), tCurr.GetMinute(), tCurr.GetSecond());
		cStrLast.Format(L"%d/%d/%d %d:%d:%d", tLastConnect.GetYear(), tLastConnect.GetMonth(), tLastConnect.GetDay(), tLastConnect.GetHour(),
			tLastConnect.GetMinute(), tLastConnect.GetSecond());

		START_LOG( clog, L"계산된 날짜 값이 이상합니다" )
			<< BUILD_LOG( tSpanLastConnect.GetDays() )
			<< BUILD_LOG( cStrCurr.GetBuffer())
			<< BUILD_LOG( cStrLast.GetBuffer())
			<< END_LOG;
		return false;
	}

	{
		// 반복문을 돌면서 접속 날 수 대비 보상 단계를 구한다.
		std::map< int, int >::iterator	mitMain = mapComeBackRewardCondition.begin();
		for( ; mitMain != mapComeBackRewardCondition.end() ; ++mitMain )
		{
			if( mitMain->second <= tSpanLastConnect.GetDays() )
			{
				if( iRewardStep < mitMain->first )
				{
					iRewardStep = mitMain->first;
				}
			}
		}
	}

	CTimeSpan tSpan;

	switch( iRewardStep )
	{
	case KGameSysVal::CBS_ONE:
		{
			tSpan = CTimeSpan( KGameSysVal::CBSD_ONE , 0, 0, 0);
			tCurr += tSpan;
			wstrEndBuffDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
		}break;
	case KGameSysVal::CBS_TWO:
		{
			tSpan = CTimeSpan( KGameSysVal::CBSD_TWO , 0, 0, 0);
			tCurr += tSpan;
			wstrEndBuffDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
		}break;
	case KGameSysVal::CBS_THREE:
		{
			tSpan = CTimeSpan( KGameSysVal::CBSD_THREE , 0, 0, 0);
			tCurr += tSpan;
			wstrEndBuffDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );
		}break;
	default:
		{
			wstrEndBuffDate = L"2000-01-01 00:00:00";
			START_LOG( clog, L"복귀유저 보상 단계가 잘못 되었다" )
				<< BUILD_LOG( iRewardStep )
				<< BUILD_LOG( tSpanLastConnect.GetDays() )
				<< END_LOG;
			return false;
		}break;
	}

	return true;
}

bool KGSAccountDBThread::CheckComeBackFirst( IN std::wstring& wstrEndDate )
{
	CTime tCurr = CTime::GetCurrentTime();
	CTime tEndDate;

	if( KncUtil::ConvertStringToCTime( wstrEndDate , tEndDate ) == false )
	{
		START_LOG( cerr, L"날짜 변환 실패!!!" )
			<< BUILD_LOG( wstrEndDate )
			<< END_LOG;
		return false;
	}

	if( tEndDate < tCurr )	// 복귀 버프 기간이 지났다.
	{
		// 그런데 복귀 유저다.
		// 보상 지급해야 한다.
		return true;
	}

	return false;
}

IMPL_ON_FUNC( DBE_WRITE_COME_BACK_REWARD_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	CTime tCurr = CTime::GetCurrentTime();
	std::wstring wstrComeBackRegDate = tCurr.Format( _T("%Y-%m-%d %H:%M:%S"));
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_MER", L"%d, %d, N\'%s\', N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_iRewardStep %kPacket_.m_wstrComeBackBuffEndDate %wstrComeBackRegDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_create_Comebackuser_info", L"%d, %d, N\'%s\', N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_iRewardStep %kPacket_.m_wstrComeBackBuffEndDate %wstrComeBackRegDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

	// iOK = -1 : MUser 테이블에 해당 UserUID가 없을때
	// iOK = -2 : 휴먼복귀 유저 테이블에 최초로 데이터 insert 시, 에러 
	// iOK = -3 : 휴먼복귀 유저 테이블에 이미 데이터 있는 상태에서 update 시, 에러

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"휴면 복귀 정보가 입력되지 않았습니다." )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iRewardStep )
			<< BUILD_LOG( kPacket_.m_wstrComeBackBuffEndDate )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_WRITE_COME_BACK_END_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_UPD", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_update_Comebackuser_finish", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

	// iOK = -1 : MUserComeback 테이블에 해당 UserUID가 없을때
	// iOK = -2 : 휴먼복귀 유저 테이블에 IsComeBackUser = 0 업데이트 시, 에러

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"휴면 복귀 정보가 입력되지 않았습니다." )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( iOK )
			<< END_LOG;
	}
}

IMPL_ON_FUNC( DBE_WRITE_COME_BACK_LOGOUT_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserComeback_Logout_MER", L"%d, N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_wsrtLogOutDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_comeback_logout", L"%d, N\'%s\'", % kPacket_.m_iUserUID %kPacket_.m_wsrtLogOutDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );

		m_kODBC.EndFetch();
	}

	// dbo.muser 에 해당 useruid 없을때 @iOK = -1
	// dbo.MUserComeback_Logout 인설트 시 에러 @iOK = -2
	// dbo.MUserComeback_Logout 업데이트 시 에러 @iOK = -3

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"휴면 복귀 정보용 로그아웃 기록 실패!!" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_wsrtLogOutDate )
			<< BUILD_LOG( iOK )
			<< END_LOG;
	}
}
#endif SERV_COME_BACK_USER_REWARD
//}}

//{{ 2011. 06. 22    김민성    이탈 방지 모델 - NEXON 세션 시스템 수정
#ifdef SERV_NEXON_SESSION_PROTOCOL
IMPL_ON_FUNC( DBE_CHECK_RETAINING_USER_REQ )
{
	KDBE_CHECK_RETAINING_USER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_RetainingUser_SEL", L"%d", 
		% kPacket_.m_iUserUID
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_Retaining_User", L"%d", 
		% kPacket_.m_iUserUID
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_wstrDate  );
		m_kODBC.EndFetch();
	}

end_proc:

	if( kPacket.m_wstrDate.empty() == true )  // 비었다 -> 받은적이 없다 
	{
		kPacket.m_wstrDate = L"2000-01-01 00:00:00";   // 초기화 기능
	}
	
	SendToUser( LAST_SENDER_UID, DBE_CHECK_RETAINING_USER_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_RETAINING_USER_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_RetainingUser_INS", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iItemID
		% kPacket_.m_wstrRegDate
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_Retaining_User", L"%d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iItemID
		% kPacket_.m_wstrRegDate
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"이탈 방지 유저에 대한 정보 기록 실패" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iItemID )
			<< END_LOG;

		kPacket_.m_iUserUID = 0;   // 로그를 남기지 않기 위한 장치
	}
    
	SendToUser( LAST_SENDER_UID, DBE_INSERT_RETAINING_USER_ACK, kPacket_ );
}
#endif SERV_NEXON_SESSION_PROTOCOL
//}} 

//{{ 2011. 09. 14  김민성	해킹 프로세스 목록 전달 - 게임 로딩 이전 단계
#ifdef SERV_DLL_LIST_CHECK_BEFOR_LOADING
_IMPL_ON_FUNC( DBE_UPDATE_CHANNEL_RANDOMKEY_NOT, KDBE_UPDATE_CHANNEL_RANDOMKEY_ACK )
{
	kPacket_.m_iOK = NetError::ERR_ODBC_00;

	// ID 특수 문자가 포함 되어 있다면 
	if( KODBC::IsInvalidMarkIn( kPacket_.m_wstrUserID ) )
	{
		START_LOG( cout, L"특수 문자 사용한 ID, PW 입력으로 접속 종료!")
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOG( kPacket_.m_iRandomKey )
			<< END_LOG;

		goto end_proc;
	}
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserAuthentication_MER", L"N\'%s\', %d", % kPacket_.m_wstrUserID % kPacket_.m_iRandomKey );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_insert_user_authkey", L"N\'%s\', %d", % kPacket_.m_wstrUserID % kPacket_.m_iRandomKey );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"User 인증 RandomKey 등록 실패.!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_wstrUserID )
			<< BUILD_LOGc( kPacket_.m_iRandomKey )
			<< END_LOG;
	}
}
#endif SERV_DLL_LIST_CHECK_BEFOR_LOADING
//}}

//{{  2012. 1. 16	김민성		게임서버에서 Server SN 검사
#ifdef SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
IMPL_ON_FUNC( DBE_CHECK_SERVER_SN_IN_GAMESERVER_REQ )
{
	kPacket_.m_iOK = NetError::NET_OK;
	std::string	strMachineID;
	__int64	iServerSN = 0;
	int	iResult = -1;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Temp_MID_SN_SEL_IsAbnormalSN", L"%d", % kPacket_.m_iServerSN );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_serversn_check", L"%d", % kPacket_.m_iServerSN );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iResult );
		m_kODBC.EndFetch();
	}

	if( iResult == 1 ) // 접속 제한 SN 존재한다
	{
		kPacket_.m_iOK = NetError::ERR_SERVERSN_07;
		goto end_proc;
	}
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Temp_MID_SN_SEL", L"%d, N\'%s\'", % kPacket_.m_iServerSN % kPacket_.m_strMachineID.c_str() );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.mup_get_mid_sn", L"%d, N\'%s\'", % kPacket_.m_iServerSN % kPacket_.m_strMachineID.c_str() );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( strMachineID
				 >> iServerSN
				 >> iResult     );

		m_kODBC.EndFetch();
	}

	bool bCompareSN = false;
	bool bCompareMid = false;
	bool bExistSN	= false;
	bool bExistMid	= false;

	if( iServerSN > 0 ) // DB에 머신ID가 존재한다.
	{
		bExistMid = true;
		// MachineID랑 매칭이 되는 SN과 클라이언트에서 받은 SN을 비교하자
		if( iServerSN == kPacket_.m_iServerSN )
		{
			bCompareSN = true;
		}
	}

	if( strMachineID.empty() == false ) // DB에 sn이 존재한다.
	{
		bExistSN = true;
		if( strMachineID.compare( kPacket_.m_strMachineID ) == 0 )
		{
			bCompareMid = true;
		}
	}


//////////////////////////////////////////////////////////////////////////
	if( bExistSN == true )
	{
		if( bExistMid == true )
		{
			if( bCompareSN == true )
			{
				if( bCompareMid == true )
				{
					// 정상
				}
				else
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// 다 존재 하는데 mid 만 같지 않다?
					START_LOG( cerr, L"Server SN - 정상처리되긴 하는데....이런 경우가 있나?_1" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
			else
			{
				if( bCompareMid == true )
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// 다 존재 하는데 sn 만 같지 않다?
					START_LOG( cerr, L"Server SN - 정상처리되긴 하는데....이런 경우가 있나?_2" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// a/2, b/1 
					// 존재하기는 하지만 모두 맞지 않는 데이터이다.  대부분이 이것이다.
					// 재발급, 메일, 종료
					kPacket_.m_iOK = NetError::ERR_SERVERSN_01;

					START_LOG( clog, L"Server SN - 재발급, 메일, 종료" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
		else
		{
			if( bCompareSN == true )
			{
				kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

				// 클라가 준 mid 로 찾은 sn이 없는데 bCompareSN 가 true 일 수 있나?
				START_LOG( cerr, L"Server SN - 정상처리되긴 하는데....이런 경우가 있나?_3" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else
			{
				if( bCompareMid == true )
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// 클라가 준 mid 로 찾은 sn이 없는데 bCompareMid 만 true 일 수 있나?
					START_LOG( cerr, L"Server SN - 정상처리되긴 하는데....이런 경우가 있나?_4" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// c/1 
					// 클라가 준 mid 로 찾은 sn이 클라가 준 sn으로 찾은 mid는 있다.
					// 업데이트, 메일, 정상					
					kPacket_.m_iOK = NetError::ERR_SERVERSN_03;

					START_LOG( clog, L"MachineID 정보 update" )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
	}
	else
	{
		if( bExistMid == true )
		{
			if( bCompareSN == true )
			{
				kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

				// 클라가 준 sn 이 존재하지 않는데 bCompareMid 와 bCompareSN 이 True 가 될 수 없다.
				START_LOG( cerr, L"Server SN - 정상처리되긴 하는데....이런 경우가 있나?_4" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else
			{
				if( bCompareMid == true )
				{
					kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

					// 클라가 준 sn 이 존재하지 않는데 bCompareMid 와 bCompareSN 이 True 가 될 수 없다.
					START_LOG( cerr, L"Server SN - 정상처리되긴 하는데....이런 경우가 있나?_5" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
				else
				{
					// a/3
					// 클라가 준 sn 은 존재하지만 mid가 없다. 
					// 재발급, 메일, 종료
					kPacket_.m_iOK = NetError::ERR_SERVERSN_05;

					START_LOG( clog, L"Server SN - 재발급, 메일, 종료" )
						<< BUILD_LOG( kPacket_.m_iOK )
						<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
						<< BUILD_LOG( kPacket_.m_iServerSN )
						<< BUILD_LOG( kPacket_.m_strMachineID )
						<< BUILD_LOG( iServerSN )
						<< BUILD_LOG( strMachineID )
						<< BUILD_LOG( bExistSN )
						<< BUILD_LOG( bExistMid )
						<< BUILD_LOG( bCompareMid )
						<< BUILD_LOG( bCompareSN )
						<< END_LOG;
				}
			}
		}
		else
		{
			if( bCompareSN == true || bCompareMid == true )
			{
				kPacket_.m_iOK = NetError::ERR_SERVERSN_04;

				// 클라가 준 sn 과 mid 가 존재하지 않는데 bCompareMid 와 bCompareSN 이 True 가 될 수 없다.
				START_LOG( cerr, L"Server SN - 정상처리되긴 하는데....이런 경우가 있나?_6" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
			else //( bCompareSN == false && bCompareMid == false )
			{
				// c/3
				// 클라가 준 sn, mid 가 없다. 새로 만들어 주자
				// 발급, 메일, 종료
				kPacket_.m_iOK = NetError::ERR_SERVERSN_02;

				START_LOG( clog, L"Server SN - 발급, 메일, 종료" )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kPacket_.m_kNexonAccountInfo.m_wstrID )
					<< BUILD_LOG( kPacket_.m_iServerSN )
					<< BUILD_LOG( kPacket_.m_strMachineID )
					<< BUILD_LOG( iServerSN )
					<< BUILD_LOG( strMachineID )
					<< BUILD_LOG( bExistSN )
					<< BUILD_LOG( bExistMid )
					<< BUILD_LOG( bCompareMid )
					<< BUILD_LOG( bCompareSN )
					<< END_LOG;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////

	/*
	// DB에 기록 되지 않은 MID, SN 이다. - 있을수 음씀 - channel server 에서 발급 되었음
	if( bExistSN == false || bExistMid == false )
	{
		START_LOG( cerr, L"GameServe Server SN 확인 - 존재하지 않는다." )
			<< BUILD_LOG( kPacket_.m_iServerSN )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< BUILD_LOG( iServerSN )
			<< BUILD_LOG( strMachineID )
			<< BUILD_LOG( bExistSN )
			<< BUILD_LOG( bExistMid )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_SERVERSN_01;
		goto end_proc;
	}
	// 매칭 되지 않는 MID 와 SN 이 맞지 않다.
	if( bCompareSN == false || bCompareMid == false )
	{
		START_LOG( cerr, L"GameServe Server SN 확인 - 존재하지만 같지 않다." )
			<< BUILD_LOG( kPacket_.m_iServerSN )
			<< BUILD_LOG( kPacket_.m_strMachineID )
			<< BUILD_LOG( iServerSN )
			<< BUILD_LOG( strMachineID )
			<< BUILD_LOG( bCompareSN )
			<< BUILD_LOG( bCompareMid )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_SERVERSN_02;
		goto end_proc;
	}
	*/

	//{{ 2012. 04. 02	김민성		sn 모니터링 기능 분리
#ifdef SERV_SERIAL_NUMBER_MORNITORING
	if( kPacket_.m_iOK == NetError::NET_OK )
	{
		DO_QUERY( L"exec dbo.P_SNMonitoring_SEL", L"%d", % kPacket_.m_iServerSN );
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket_.m_iMonitoringFlag );
			m_kODBC.EndFetch();
		}

		START_LOG( clog, L"SN모니터링 값을 잘 받아 왔는가?" )
			<< BUILD_LOG( kPacket_.m_iServerSN )
			<< BUILD_LOG( kPacket_.m_iMonitoringFlag )
			<< END_LOG;
	}
#endif SERV_SERIAL_NUMBER_MORNITORING
	//}}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHECK_SERVER_SN_IN_GAMESERVER_ACK, kPacket_ );
}
#endif SERV_SERIAL_NUMBER_AVAILABILITY_CHECK_IN_GAMESERVER
//}}

//{{ 2012. 05. 17	박세훈	키보드 맵핑 정보를 DB에서 읽고 쓰기
#ifdef SERV_KEYBOARD_MAPPING_INFO_RW
IMPL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_WRITE_REQ )
{
	KEGS_KEYBOARD_MAPPING_INFO_WRITE_ACK kPacket;
	std::map<short, short>::const_iterator cit;

	kPacket.m_iOK = NetError::NET_OK;

#ifdef SERV_KEY_MAPPING_INT
	for( cit = kPacket_.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo.begin(); cit != kPacket_.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo.end(); ++cit )
	{
		short sAction = cit->first + GAMEPAD_ID_MODIFIER;
		short sKey = cit->second;
		kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.insert( std::make_pair( sAction, sKey ) );
	}
#endif //SERV_KEY_MAPPING_INT

	for( cit = kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.begin(); cit != kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.end(); ++cit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserKeyBoard_MER", L"%d, %d, %d", % kPacket_.m_iUserUID % cit->first % cit->second );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MUserKeyBoard_INT", L"%d, %d, %d", % kPacket_.m_iUserUID % cit->first % cit->second );
#endif //SERV_ALL_RENEWAL_SP
		int iResult = -1;
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iResult );
			m_kODBC.EndFetch();
		}

		if( iResult != NetError::NET_OK )
		{
			kPacket.m_iOK = -1;
		}
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( cit == kPacket_.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.begin() )
		{
			// 한개도 기록 못했다.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_01;
		}
		else
		{
			// 일부만 기록했다.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_02;
		}
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_KEYBOARD_MAPPING_INFO_WRITE_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_KEYBOARD_MAPPING_INFO_READ_REQ, UidType )
{
	KEGS_KEYBOARD_MAPPING_INFO_NOT kPacket;
	
	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserKeyBoard_SEL", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MUserKeyBoard_GET", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		int iResult;
		short iKey;
		short iValue;
		FETCH_DATA( iResult
			>> iKey
			>> iValue
			);

		if( iResult != NetError::NET_OK )
		{
			if( iResult != -1 )
			{
				kPacket.m_iOK = iResult;
			}
		}
		else
		{
#ifdef SERV_KEY_MAPPING_INT
			if( iKey >= GAMEPAD_ID_MODIFIER )
				kPacket.m_kKeyboardMappingInfo.m_mapGamePadMappingInfo.insert( std::map<int, int>::value_type( iKey - GAMEPAD_ID_MODIFIER, iValue ) );
			else
#endif //SERV_KEY_MAPPING_INT
			kPacket.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.insert( std::map<int, int>::value_type( iKey, iValue ) );
		}
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( kPacket.m_kKeyboardMappingInfo.m_mapKeyboardMappingInfo.empty() == true )
		{
			// 한개도 가져오지 못했다.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_03;
		}
		else
		{
			// 일부만 가져왔다.
			kPacket.m_iOK = NetError::ERR_KEYBOARD_MAPPING_04;
		}
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_KEYBOARD_MAPPING_INFO_READ_ACK, kPacket );
}
#endif SERV_KEYBOARD_MAPPING_INFO_RW
//}}

//{{ 2012. 05. 29	박세훈	채팅 옵션 정보를 DB에서 읽고 쓰기
#ifdef SERV_CHAT_OPTION_INFO_RW
IMPL_ON_FUNC( DBE_CHAT_OPTION_INFO_WRITE_REQ )
{
	KEGS_CHAT_OPTION_INFO_WRITE_ACK kPacket;

	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_INT", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_cIndex % kPacket_.m_bValue );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_INT", L"%d, %d, %d", % kPacket_.m_iUserUID % kPacket_.m_cIndex % kPacket_.m_bValue );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		kPacket.m_iOK = NetError::ERR_CHAT_OPTION_01;
		START_LOG( cerr, L"채팅 옵션 정보 저장 실패!!" )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_cIndex )
			<< BUILD_LOG( kPacket_.m_bValue )
			<< END_LOG;
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHAT_OPTION_INFO_WRITE_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_CHAT_OPTION_INFO_READ_REQ, UidType )
{
	KEGS_CHAT_OPTION_INFO_NOT kPacket;

	kPacket.m_iOK = NetError::NET_OK;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_SEL", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MChatOption_GET", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		byte cIndex;
		bool bValue;
		FETCH_DATA( kPacket.m_iOK
			>> cIndex
			>> bValue
			);

		if( kPacket.m_iOK == NetError::NET_OK )
		{
			kPacket.m_mapChatOptionSetInfo.insert( std::map<byte, bool>::value_type( cIndex, bValue ) );
		}
		else if( kPacket.m_iOK == -1 )
		{
			kPacket.m_iOK = NetError::NET_OK;
		}
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		if( kPacket.m_mapChatOptionSetInfo.empty() == true )
		{
			// 한개도 가져오지 못했다.
			kPacket.m_iOK = NetError::ERR_CHAT_OPTION_02;
		}
		else
		{
			// 일부만 가져왔다.
			kPacket.m_iOK = NetError::ERR_CHAT_OPTION_03;
		}
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_CHAT_OPTION_INFO_READ_ACK, kPacket );
}
#endif SERV_CHAT_OPTION_INFO_RW
//}}


//{{ 2012. 06. 07	박세훈	매일매일 선물 상자
#ifdef SERV_EVENT_DAILY_GIFT_BOX
IMPL_ON_FUNC( DBE_PURCHASED_DAILY_GIFT_BOX_ITEM_WRITE_NOT )
{
	int iResult = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_MER", L"%d, %d, %d, %d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iItemID % kPacket_.m_iState % kPacket_.m_wstrRegDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_EveryDayGift_INT", L"%d, %d, %d, %d, N\'%s\'", % kPacket_.m_iUserUID % kPacket_.m_iUnitUID % kPacket_.m_iItemID % kPacket_.m_iState % kPacket_.m_wstrRegDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iResult );
		m_kODBC.EndFetch();
	}

	if( iResult != NetError::NET_OK )
	{
		if( iResult == -2 )
		{
			START_LOG( cerr, L"이미 존재하는 정보입니다.")
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"구입한 DailyGiftBox 아이템 기록에 실패하였습니다.")
				<< BUILD_LOG( iResult )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iItemID )
				<< BUILD_LOG( kPacket_.m_iState )
				<< BUILD_LOG( kPacket_.m_wstrRegDate )
				<< END_LOG;
		}
	}
end_proc:
	return;
}
#endif SERV_EVENT_DAILY_GIFT_BOX
//}}

#ifdef SERV_EVENT_MONEY	// 김민성 // 적용날짜: 2013-07-04
IMPL_ON_FUNC( DBE_UPDATE_EVENT_MONEY_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MEventPoint_MER", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iOldQuantity 
		% kPacket_.m_iNewQuantity 
		% kPacket_.m_iRewardItemID 
		% kPacket_.m_wstrRegDate );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MEventPoint_UPD", L"%d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_iOldQuantity 
		% kPacket_.m_iNewQuantity 
		% kPacket_.m_iRewardItemID 
		% kPacket_.m_wstrRegDate );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"천사의 깃털 업데이트 실패" )
			<< BUILD_LOG( kPacket_.m_iUserUID  )
			<< BUILD_LOG( kPacket_.m_iUnitUID  )
			<< BUILD_LOG( kPacket_.m_iOldQuantity  )
			<< BUILD_LOG( kPacket_.m_iNewQuantity  )
			<< BUILD_LOG( kPacket_.m_iRewardItemID )
			<< BUILD_LOG( kPacket_.m_wstrRegDate )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_UPDATE_EVENT_MONEY_ACK, kPacket_.m_iOldQuantity + kPacket_.m_iNewQuantity );
}
#endif // SERV_EVENT_MONEY

//{{ 2012. 09. 22	박세훈	빙고 이벤트
#ifdef SERV_EVENT_BINGO
void KGSAccountDBThread::ReadBingoEvent( IN const UidType anTrace_[], OUT KDBE_BINGO_EVENT_INFO_READ_ACK& kPacket )
//IMPL_ON_FUNC_NOPARAM( DBE_BINGO_EVENT_INFO_READ_REQ )
{
	//KDBE_BINGO_EVENT_INFO_READ_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	//----------------------------------------------------//
	// 기회 비용 읽기
	DO_QUERY( L"exec dbo.P_MEvent_Bingo_UserInfo_GET", L"%d", 
		% LAST_SENDER_UID
		);

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iBalance
			>> kPacket.m_iLotteryChance
			>> kPacket.m_iLotteryRecord
			>> kPacket.m_iMixChance
			>> kPacket.m_iResetChance
			);
		m_kODBC.EndFetch();
	}

	//----------------------------------------------------//

	byte	i;
	byte	iPos;
	int		iValue;
	bool	bOpen;

	// 빙고판 정보 읽기
	DO_QUERY( L"exec dbo.P_MEvent_Bingo_GET", L"%d", 
		% LAST_SENDER_UID
		);

	// 있을 수 없는 양의 정보 오류 체크를 위해 일부러 한칸 더 받기를 시도한다.
	kPacket.m_vecBingoBoard.resize( BSV_BINGOBOARD_SLOT_NUM_CHECK );

	for( i=0; i < BSV_BINGOBOARD_SLOT_NUM_CHECK; ++i )
	{
		if( m_kODBC.Fetch() )
		{
			FETCH_DATA( iPos
				>> iValue
				>> bOpen
				);
			kPacket.m_vecBingoBoard[iPos] = ( bOpen ) ? iValue + 100 : iValue;
		}
		else
		{
			kPacket.m_vecBingoBoard.resize( i );
			break;
		}
	}

	if( i != BSV_BINGOBOARD_SLOT_NUM )
	{
		kPacket.m_vecBingoBoard.clear();
	}

	//----------------------------------------------------//
	// 보상 정보 읽기
	DO_QUERY( L"exec dbo.P_MEvent_Bingo_Present_GET", L"%d", 
		% LAST_SENDER_UID
		);

	// 있을 수 없는 양의 정보 오류 체크를 위해 일부러 한칸 더 받기를 시도한다.
	kPacket.m_vecPresentInfo.resize( BSV_PRESENT_NUM_CHECK );

	for( i=0; i < BSV_PRESENT_NUM_CHECK; ++i )
	{
		if( m_kODBC.Fetch() )
		{
			FETCH_DATA( iPos
				>> iValue
				>> bOpen
				);
			kPacket.m_vecPresentInfo[iPos] = std::make_pair( iValue, bOpen );
		}
		else
		{
			kPacket.m_vecPresentInfo.resize( i );
			break;
		}
	}

	if( i != BSV_PRESENT_NUM )
	{
		kPacket.m_vecBingoBoard.clear();
	}

	//----------------------------------------------------//

	kPacket.m_iOK = NetError::NET_OK;

end_proc:
	return;
//	SendToUser( LAST_SENDER_UID, DBE_BINGO_EVENT_INFO_READ_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_BINGO_EVENT_INFO_WRITE_REQ )
{
	KDBE_BINGO_EVENT_INFO_WRITE_ACK kPacket;
	int iOK = NetError::ERR_ODBC_01;

	kPacket.m_usAckEventID = kPacket_.m_usAckEventID;
	//----------------------------------------------------//

	if( kPacket_.m_bChanceChanged == true )
	{
		iOK = NetError::ERR_ODBC_01;
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_UserInfo_INT", L"%d, %d, %d, %d, %d, %d", 
			% LAST_SENDER_UID
			% kPacket_.m_iBalance
			% kPacket_.m_iLotteryChance
			% kPacket_.m_iLotteryRecord
			% kPacket_.m_iMixChance
			% kPacket_.m_iResetChance
			);

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"빙고 이벤트 기회 정보 업데이트 실패")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_iBalance )
				<< BUILD_LOG( kPacket_.m_iLotteryChance )
				<< BUILD_LOG( kPacket_.m_iLotteryRecord )
				<< BUILD_LOG( kPacket_.m_iMixChance )
				<< BUILD_LOG( kPacket_.m_iResetChance )
				<< END_LOG;

			kPacket.m_iBalance			= kPacket_.m_iBalance;
			kPacket.m_iLotteryChance	= kPacket_.m_iLotteryChance;
			kPacket.m_iLotteryRecord	= kPacket_.m_iLotteryRecord;
			kPacket.m_iMixChance		= kPacket_.m_iMixChance;
			kPacket.m_iResetChance		= kPacket_.m_iResetChance;
			kPacket.m_bChanceChanged	= false;
		}
	}

	//----------------------------------------------------//
	// 빙고판 정보 쓰기
	for( std::map<byte, byte>::const_iterator it=kPacket_.m_mapBingoBoard.begin(); it != kPacket_.m_mapBingoBoard.end(); ++it )	// 빙고 숫자판
	{
		iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_MER", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% ( ( it->second < BSV_NUM_MAX ) ? it->second : it->second - BSV_NUM_MAX )
			% ( BSV_NUM_MAX <= it->second )
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_INT", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% ( ( it->second < BSV_NUM_MAX ) ? it->second : it->second - BSV_NUM_MAX )
			% ( BSV_NUM_MAX <= it->second )
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"빙고판 정보 업데이트 실패")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( it->first )
				<< BUILD_LOG( it->second )
				<< END_LOG;
			kPacket.m_mapBingoBoard.insert( std::map<byte, byte>::value_type( it->first, it->second ) );
		}
	}

	//----------------------------------------------------//
	// 선물 정보 쓰기
	for( std::map<byte, std::pair<int, bool> >::const_iterator it=kPacket_.m_mapPresentInfo.begin(); it != kPacket_.m_mapPresentInfo.end(); ++it )	// 선물 정보
	{
		iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Present_MER", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% it->second.first
			% it->second.second
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Present_INT", L"%d, %d, %d, %d", 
			% LAST_SENDER_UID
			% it->first
			% it->second.first
			% it->second.second
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"빙고 이벤트 보상 정보 업데이트 실패")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( it->first )
				<< BUILD_LOG( it->second.first )
				<< BUILD_LOG( it->second.second )
				<< END_LOG;
			kPacket.m_mapPresentInfo.insert( std::map<byte, std::pair<int, bool> >::value_type( it->first, std::make_pair( it->second.first, it->second.second ) ) );
		}
	}

	//----------------------------------------------------//
	// 로그 정보 쓰기
	BOOST_TEST_FOREACH( KBingoEventLog, kBingoEventLog, kPacket_.m_vecLog )
	{
		iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Log_INS", L"%d, %d, %d, %d, %d, %d, N\'%s\'", 
			% LAST_SENDER_UID
			% kBingoEventLog.iUnitUID
			% kBingoEventLog.iActionType
			% kBingoEventLog.iPos
			% kBingoEventLog.iOpenNum
			% kBingoEventLog.iItemID
			% kBingoEventLog.wstrRegDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_Bingo_Log_INT", L"%d, %d, %d, %d, %d, %d, N\'%s\'", 
			% LAST_SENDER_UID
			% kBingoEventLog.iUnitUID
			% kBingoEventLog.iActionType
			% kBingoEventLog.iPos
			% kBingoEventLog.iOpenNum
			% kBingoEventLog.iItemID
			% kBingoEventLog.wstrRegDate
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"빙고 이벤트 로그 정보 업데이트 실패")
				<< BUILD_LOG( iOK )
				<< BUILD_LOG( LAST_SENDER_UID )
				<< BUILD_LOG( kBingoEventLog.iUnitUID )
				<< BUILD_LOG( kBingoEventLog.iActionType )
				<< BUILD_LOG( kBingoEventLog.iPos )
				<< BUILD_LOG( kBingoEventLog.iOpenNum )
				<< BUILD_LOG( kBingoEventLog.iItemID )
				<< BUILD_LOG( kBingoEventLog.wstrRegDate )
				<< END_LOG;
			kPacket.m_vecLog.push_back( kBingoEventLog );
		}
	}

	//----------------------------------------------------//

end_proc:
	SendToUser( LAST_SENDER_UID, kPacket_.m_usAckEventID, kPacket );
}
#endif SERV_EVENT_BINGO
//}}

//{{ 2012. 12. 12	박세훈	겨울 방학 전야 이벤트( 임시, 하드 코딩 )
#ifdef SERV_2012_WINTER_VACATION_EVENT
IMPL_ON_FUNC_NOPARAM( DBE_2012_WINTER_VACATION_EVENT_NOT )
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	CTimeSpan tGab = tCurrentTime - CTime( 2012, 12, 13, 0, 0, 0 );

	DO_QUERY( L"exec dbo.P_MEvent_WinterVacation_UPT", L"%d, %d", % FIRST_SENDER_UID % tGab.GetTotalSeconds() );
	if( m_kODBC.BeginFetch() )
	{
		int iTemp = 0;
		FETCH_DATA( iTemp );
		m_kODBC.EndFetch();

		if( iTemp != 0 )
		{
			START_LOG( cerr, L"마지막 이벤트 완료 시간 기록 실패!" )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( tGab.GetTotalSeconds() )
				<< END_LOG;
		}
	}
	else
	{
		START_LOG( cerr, L"마지막 이벤트 완료 시간 기록 실패!" )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( tGab.GetTotalSeconds() )
			<< END_LOG;
	}

end_proc:
	SendToUser( FIRST_SENDER_UID, DBE_2012_WINTER_VACATION_EVENT_NOT, static_cast<int>( tGab.GetTotalSeconds() ) );
	return;
}
#endif SERV_2012_WINTER_VACATION_EVENT
//}}

//{{ 2012. 12. 14  계정 미션 시스템 ( 계정단위 퀘스트 ) - 김민성
#ifdef SERV_ACCOUNT_MISSION_SYSTEM
_IMPL_ON_FUNC( DBE_NEW_ACCOUNT_QUEST_REQ, KDBE_NEW_QUEST_REQ )
{
	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	KDBE_NEW_ACCOUNT_QUEST_ACK kPacket;
	kPacket.m_iOK		= NetError::ERR_ODBC_01;
	kPacket.m_kQuestReq	= kPacket_;

	//////////////////////////////////////////////////////////////////////////
	// 계정 퀘스트 수락
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_Mquests_MER", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_UserUID 
		% kPacket_.m_iQuestID 
		% 0 
		% 0 
		% 0 
		% 0
		% 0
		% wstrNow  );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MQuests_INS_UPT", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
		% kPacket_.m_UserUID 
		% kPacket_.m_iQuestID 
		% 0 
		% 0 
		% 0 
		% 0
		% 0
		% wstrNow  );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"계정 퀘스트 DB 생성 실패." )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}

end_proc:

	SendToUser( LAST_SENDER_UID, DBE_NEW_ACCOUNT_QUEST_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ACCOUNT_QUEST_COMPLETE_REQ, KDBE_QUEST_COMPLETE_REQ )
{
	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	KDBE_ACCOUNT_QUEST_COMPLETE_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_kQuestReq		= kPacket_;

	std::wstring wstrCompleteDate;

	//{{ 2013. 03. 21	 계정 퀘스트 - 요일별 활성화 기능 - 김민성
#ifdef SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	if( kPacket_.m_bIsRepeat == true )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_INS", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_INT", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#endif //SERV_ALL_RENEWAL_SP
	}
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_INS", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Complete_INT", L"%d, %d, %d, N\'%s\'", 
			% FIRST_SENDER_UID 
			% kPacket_.m_UnitUID 
			% kPacket_.m_iQuestID
			% wstrNow   );
#endif //SERV_ALL_RENEWAL_SP
	}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"계정 퀘스트 완료 DB 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}
	else
	{
		CTime tCompleteDate;
		LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );

		// 퀘스트 완료 업데이트 성공!
		kPacket.m_tCompleteTime = tNow.GetTime();
	}
#else
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MQuests_Complete_INS", L"%d, %d, %d, N\'%s\'", 
		% FIRST_SENDER_UID 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iQuestID
		% wstrNow   );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MQuests_Complete_INT", L"%d, %d, %d, N\'%s\'", 
		% FIRST_SENDER_UID 
		% kPacket_.m_UnitUID 
		% kPacket_.m_iQuestID
		% wstrNow   );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"계정 퀘스트 완료 DB 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( FIRST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_UnitUID )
			<< BUILD_LOG( kPacket_.m_iQuestID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_QUEST_10;
		goto end_proc;
	}
	else
	{
		CTime tCompleteDate;
		LIF( KncUtil::ConvertStringToCTime( wstrCompleteDate, tCompleteDate ) );

		// 퀘스트 완료 업데이트 성공!
		kPacket.m_tCompleteTime = tNow.GetTime();
	}
#endif SERV_ACCOUNT_QUEST_DAY_OF_WEEK
	//}

end_proc:

	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_QUEST_COMPLETE_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ACCOUNT_QUEST_UPDATE_NOT )
{
	CTime tNow = CTime::GetCurrentTime();
	std::wstring wstrNow = tNow.Format( _T( "%Y-%m-%d %H:%M:%S" ) );

	BOOST_TEST_FOREACH( KQuestUpdate&, kInfo, kPacket_.m_vecAccountQuesting )
	{
		if( kInfo.m_vecClearData.size() != 5 )
		{
			START_LOG( cerr, L"퀘스트 클리어 정보가 5개가 아니다! 절대 일어나서는 안되는 에러!" )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kInfo.m_iQuestID )
				<< BUILD_LOG( kInfo.m_vecClearData.size() )
				<< END_LOG;
			continue;
		}
		
		int iOK = 0;
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_Repeat_SEL", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% kPacket_.m_iUserUID 
			% kInfo.m_iQuestID 
			% (int)kInfo.m_vecClearData[0]
			% (int)kInfo.m_vecClearData[1]
			% (int)kInfo.m_vecClearData[2]
			% (int)kInfo.m_vecClearData[3]
			% (int)kInfo.m_vecClearData[4]
			% wstrNow  );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MQuests_INS_UPT", L"%d, %d, %d, %d, %d, %d, %d, N\'%s\'", 
			% kPacket_.m_iUserUID 
			% kInfo.m_iQuestID 
			% (int)kInfo.m_vecClearData[0]
			% (int)kInfo.m_vecClearData[1]
			% (int)kInfo.m_vecClearData[2]
			% (int)kInfo.m_vecClearData[3]
			% (int)kInfo.m_vecClearData[4]
			% wstrNow  );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"계정 퀘스트 DB 생성 실패." )
				<< BUILD_LOG( kPacket_.m_iUserUID )
				<< BUILD_LOG( kInfo.m_iQuestID )
				<< BUILD_LOG( iOK )
				<< END_LOG;
		}
	}

	return;
end_proc:
	START_LOG( cerr, L"아왜!!!!계정 퀘스트 DB 생성 실패." )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< END_LOG;
}
#endif SERV_ACCOUNT_MISSION_SYSTEM
//}}

//{{ 2012. 12. 19  교환(한정 수량) - 김민성
#ifdef SERV_ITEM_EXCHANGE_LIMIT_COUNT
IMPL_ON_FUNC( DBE_ITEM_EXCHANGE_LIMIT_CHECK_REQ )
{
	KDBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_00;
	kAck.m_kDB = kPacket_.m_kToDB;

	// 결과물이 하나 뿐이라는 검사를 앞에서 했기 때문에...
	std::map<int, int>::iterator mit = kPacket_.m_kToDB.m_mapResultItem.begin();
	if( mit != kPacket_.m_kToDB.m_mapResultItem.end() )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_ExchangeQuantity_UPD", L"%d, %d, %d", 
			% kPacket_.m_kReq.m_iSourceItemID
			% mit->first
			% mit->second	  );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MEvent_ExchangeQuantity_UPT", L"%d, %d, %d", 
			% kPacket_.m_kReq.m_iSourceItemID
			% mit->first
			% mit->second	  );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kAck.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kAck.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"계정 퀘스트 DB 생성 실패." )
				<< BUILD_LOG( FIRST_SENDER_UID )
				<< BUILD_LOG( kPacket_.m_kReq.m_iSourceItemID )
				<< BUILD_LOG( kPacket_.m_kReq.m_iDestItemID )
				<< BUILD_LOG( kAck.m_iOK )
				<< END_LOG;
		}
	}

end_proc:

	SendToUser( LAST_SENDER_UID, DBE_ITEM_EXCHANGE_LIMIT_CHECK_ACK, kAck );
}
#endif SERV_ITEM_EXCHANGE_LIMIT_COUNT
//}}

//{{ 2013. 01. 21  대규모 이벤트 보상 시 최초 선택 캐릭터에게 지급하는 시스템 - 김민성
#ifdef SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
IMPL_ON_FUNC( DBE_UPDATE_FIRST_SELECT_UNIT_REWARD_NOT )
{
	DO_QUERY( L"exec dbo.P_MEventReward_DEL", L"%d, %d, %d", %LAST_SENDER_UID %kPacket_.m_iUnitUID %kPacket_.m_iDBIndexID );

	int iOK = NetError::ERR_ODBC_00;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"캐릭터 선택시 보상 아이템 지급 정보 갱신 실패!" )
			<< BUILD_LOG( LAST_SENDER_UID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iDBIndexID )
			<< END_LOG;
	}
end_proc: ;
}
#endif SERV_FIRST_SELECT_UNIT_REWARD_SYSTEM
//}

#ifdef SERV_NEW_PUNISHMENT
_IMPL_ON_FUNC( DBE_GET_PUNISHMENT_REQ, UidType )
{
	KDBE_GET_PUNISHMENT_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;

	DO_QUERY( L"exec dbo.mup_get_user_punishment", L"%d", % kPacket_ );

	while( m_kODBC.Fetch() )
	{
		KPunishmentInfo info;

		FETCH_DATA( info.m_iType
			>> info.m_iStartTime
			>> info.m_iDuration	);

		START_LOG( clog, L"처벌 기록" )
			<< BUILD_LOG( info.m_iType )
			<< BUILD_LOG( info.m_iStartTime )
			<< BUILD_LOG( info.m_iDuration )
			<< END_LOG;

		kPacket.m_vecPunishment.push_back( info );
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_PUNISHMENT_ACK, kPacket );
}
#endif SERV_NEW_PUNISHMENT

//{{ 박교현 : [2010/03/09] //	중국 중독방지 방침미 시스템
#ifdef SERV_ANTI_ADDICTION_SYSTEM
_IMPL_ON_FUNC( DBE_GET_ANTI_ADDICTION_INFO_REQ, UidType )
{
	KDBE_GET_ANTI_ADDICTION_INFO_ACK kPacket;
	kPacket.m_iPlayTime = 0;
	kPacket.m_iRestTime = 0;

	DO_QUERY( L"exec dbo.mup_get_anti_addiction_info", L"%d", % kPacket_ );

	if( m_kODBC.Fetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iPlayTime
			>> kPacket.m_iRestTime
			>> kPacket.m_wstrLastLogoutTime );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( clog, L"방침미 기록이 없음" )
			<< BUILD_LOG( kPacket_ )
			<< BUILD_LOG( kPacket.m_iOK )
			<< END_LOG;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_ANTI_ADDICTION_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_UPDATE_ANTI_ADDICTION_INFO )
{
	int iOK = NetError::ERR_ODBC_01;

	START_LOG( clog, L"방침미 데이터 업데이트" )
		<< BUILD_LOG( kPacket_.m_iUserUID )
		<< BUILD_LOG( kPacket_.m_iPlayTime )
		<< BUILD_LOG( kPacket_.m_iRestTime )
		<< BUILD_LOG( kPacket_.m_wstrLastLogoutTime )
		<< END_LOG;

	DO_QUERY( L"exec dbo.mup_update_anti_addiction_info", L"%d, %d, %d, N\'%s\'", 
		% kPacket_.m_iUserUID
		% kPacket_.m_iPlayTime
		% kPacket_.m_iRestTime
		% kPacket_.m_wstrLastLogoutTime );

	if( m_kODBC.Fetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		iOK = NetError::ERR_HACK_USER_01;

		START_LOG( cerr, L"방침미 데이터 업데이트 실패.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_iPlayTime )
			<< BUILD_LOG( kPacket_.m_iRestTime )
			<< BUILD_LOG( kPacket_.m_wstrLastLogoutTime )
			<< END_LOG;
	}

end_proc:
	return;
}

#endif SERV_ANTI_ADDICTION_SYSTEM
//}}

//{{ 2013. 3. 4	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_REQ )
{
	// 랭커 유저 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket_.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket_.m_iOK
			>> kPacket_.m_kUserInfo.m_byteFilter
			>> kPacket_.m_kUserInfo.m_iCategory
			>> kPacket_.m_kUserInfo.m_byteGender
			>> kPacket_.m_kUserInfo.m_iBirth
			>> kPacket_.m_kUserInfo.m_wstrProfile
			>> kPacket_.m_wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_USER_INFO_READ_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_REQ )
{
	// 랭커 유저 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket_.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket_.m_iOK
			>> kPacket_.m_kUserInfo.m_byteFilter
			>> kPacket_.m_kUserInfo.m_iCategory
			>> kPacket_.m_kUserInfo.m_byteGender
			>> kPacket_.m_kUserInfo.m_iBirth
			>> kPacket_.m_kUserInfo.m_wstrProfile
			>> kPacket_.m_wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INQUIRY_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_REQ )
{
	// 랭커 유저 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_SEL", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_GET", L"%d", % kPacket_.m_kUserInfo.m_iUserUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		kPacket_.m_iOK = NetError::ERR_ODBC_01;

		FETCH_DATA( kPacket_.m_iOK
			>> kPacket_.m_kUserInfo.m_byteFilter
			>> kPacket_.m_kUserInfo.m_iCategory
			>> kPacket_.m_kUserInfo.m_byteGender
			>> kPacket_.m_kUserInfo.m_iBirth
			>> kPacket_.m_kUserInfo.m_wstrProfile
			>> kPacket_.m_wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_UNIT_INFO_READ_FOR_INCREASE_ACK, kPacket_ );
}

IMPL_ON_FUNC( DBE_LOCAL_RANKING_USER_INFO_WRITE_REQ )
{
	KDBE_LOCAL_RANKING_USER_INFO_WRITE_ACK kPacket;
	kPacket.m_iOK	= NetError::ERR_ODBC_01;
	kPacket.m_kInfo	= kPacket_;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_MER", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteFilter
		% kPacket_.m_iCategory
		% kPacket_.m_byteGender
		% kPacket_.m_iBirth
		% kPacket_.m_wstrProfile
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MFriendSystem_UserInfo_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteFilter
		% kPacket_.m_iCategory
		% kPacket_.m_byteGender
		% kPacket_.m_iBirth
		% kPacket_.m_wstrProfile
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrUpdated	// DB에서 주어준 시각을 기준 삼는다.
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_LOCAL_RANKING_USER_INFO_WRITE_ACK, kPacket );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

#ifdef SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK// 작업날짜: 2013-05-22	// 박세훈
IMPL_ON_FUNC( DBE_BLOCK_COUNT_CHECK_INFO_UPDATE_NOT )
{
	int iOK = NetError::ERR_ODBC_01;
	
	CTime			tCurrentTime				= CTime::GetCurrentTime();
	std::wstring	wstrBlockCheckUpdateDate	= tCurrentTime.Format( _T("%Y-%m-%d %H:%M:%S") );

	DO_QUERY( L"exec dbo.P_FieldBlock_CountCheck_INT_UPD", L"%d, %d, N\'%s\', N\'%s\'",
		% FIRST_SENDER_UID
		% kPacket_.m_byteBlockCheckCount
		% wstrBlockCheckUpdateDate
		% kPacket_.m_wstrBlockCheckResetDate
		);
	
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	return;
}

IMPL_ON_FUNC_NOPARAM( DBE_BLOCK_COUNT_CHECK_INFO_READ_REQ )
{
	int iOK = NetError::ERR_ODBC_01;
	KDBE_BLOCK_COUNT_CHECK_INFO_READ_ACK kPacket;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_FieldBlock_CountCheck_SEL", L"%d", % FIRST_SENDER_UID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_FieldBlock_CountCheck_SEL", L"%d", % FIRST_SENDER_UID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK
			>> kPacket.m_byteBlockCheckCount
			>> kPacket.m_wstrBlockCheckResetDate
			);

		m_kODBC.EndFetch();
	}
	
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_BLOCK_COUNT_CHECK_INFO_READ_ACK, kPacket );
}
#endif // SERV_SYNC_PACKET_USING_RELAY_WORKINGS_CHECK

#ifdef SERV_HACKING_USER_CHECK_COUNT// 작업날짜: 2013-06-02	// 박세훈
IMPL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_INFO_REQ )
{
	KDBE_HACKING_USER_CHECK_COUNT_INFO_ACK kPacket;
	kPacket.m_iOK			= NetError::ERR_ODBC_01;
	kPacket.m_byteType		= kPacket_.m_byteType;
	kPacket.m_wstrReason	= kPacket_.m_wstrReason;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_SEL", L"%d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteType
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_SEL", L"%d, %d",
		% kPacket_.m_iUserUID
		% kPacket_.m_byteType
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_byteBlockCheckCount
			>> kPacket.m_wstrLastResetDate
			);

		m_kODBC.EndFetch();
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_HACKING_USER_CHECK_COUNT_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_HACKING_USER_CHECK_COUNT_UPDATE_NOT )
{
	std::map<byte, KHackingUserCheckCountDB>::const_iterator it;
	for( it = kPacket_.m_mapHackingUserCheckCountDB.begin(); it != kPacket_.m_mapHackingUserCheckCountDB.end(); ++it )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_MER", L"%d, %d, %d, N\'%s\'",
			% kPacket_.m_iUserUID
			% it->first
			% it->second.m_byteBlockCheckCount
			% it->second.m_wstrLastResetDate
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_MHackingUserCheckCount_INT_UPD", L"%d, %d, %d, N\'%s\'",
			% kPacket_.m_iUserUID
			% it->first
			% it->second.m_byteBlockCheckCount
			% it->second.m_wstrLastResetDate
			);
#endif //SERV_ALL_RENEWAL_SP
	}

end_proc:
	return;
}

IMPL_ON_FUNC( DBE_ACCOUNT_BLOCK_NOT )
{
	// DB에 Account Block User 등록!
	int iOK = NetError::ERR_ODBC_01;
	const byte	byteBlockLevel = ( kPacket_.m_wstrEndDate.empty() == true ) ? 1 : 0;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MAccountBlockList_MER", L"%d, %d, %d, N\'%s\', N\'%s\'",
		% kPacket_.m_iUserUID
		% static_cast<int>( kPacket_.m_byteType )
		% static_cast<int>( byteBlockLevel )
		% kPacket_.m_wstrReason
		% kPacket_.m_wstrEndDate
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_MAccountBlockList_INT_UPD", L"%d, %d, %d, N\'%s\', N\'%s\'",
		% kPacket_.m_iUserUID
		% static_cast<int>( kPacket_.m_byteType )
		% static_cast<int>( byteBlockLevel )
		% kPacket_.m_wstrReason
		% kPacket_.m_wstrEndDate
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"Account Block User 등록 실패.!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iUserUID )
			<< BUILD_LOG( kPacket_.m_byteType )
			<< BUILD_LOG( kPacket_.m_wstrReason )
			<< BUILD_LOG( kPacket_.m_wstrEndDate )
			<< END_LOG;
	}
}
#endif // SERV_HACKING_USER_CHECK_COUNT