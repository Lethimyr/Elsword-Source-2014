#include <winsock2.h>
#include <iomanip>
#include <sstream>

#include <dbg/dbg.hpp>

#include "LoginGameDBThread.h"
#include "LoginServer.h"
#include "ActorManager.h"
#include "NetError.h"
#include "Enum/Enum.h"
#include "SimLayer.h"
#include "X2Data/XSLUnit.h"

#define CLASS_TYPE KLoginGameDBThread
ImplPfID( KLoginGameDBThread, PI_LOGIN_GAME_DB );

IMPL_PROFILER_DUMP( KLoginGameDBThread )
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
		DO_QUERY_NO_PROFILE( L"exec dbo.gup_insert_querystats", L"N\'%s\', %d, %d, %d, %d, %d, %d",
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

KLoginGameDBThread::~KLoginGameDBThread(void)
{
}

void KLoginGameDBThread::ProcessEvent( const KEventPtr& spEvent_ )
{
    switch( spEvent_->m_usEventID )
    {    
        CASE_NOPARAM( DBE_GET_WEB_RANKING_INFO_REQ );

		//{{ 2009. 11. 19  최육사	길드구조개선
		CASE( DBE_GET_GUILD_INFO_REQ );
		CASE( DBE_GUILD_LEVEL_UP_REQ );
		//}}

		//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

		CASE( DBE_GET_GUILD_SKILL_REQ );
		CASE( DBE_INSERT_GUILD_CASH_SKILL_POINT_REQ );
		CASE( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ );
	   _CASE( DBE_ADMIN_GET_GUILD_SKILL_POINT_REQ, KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ );

#endif GUILD_SKILL_TEST
		//}}

		//////////////////////////////////////////////////////////////////////////   
		//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD

		CASE_NOPARAM( DBE_GET_GUILD_BOARD_INFO_REQ );
	   _CASE( DBE_GET_GUILD_SKILL_IN_BOARD_REQ, KELG_GET_GUILD_SKILL_IN_BOARD_REQ );
		CASE( DBE_REGISTRATION_GUILD_AD_REQ );
	   _CASE( DBE_MODIFY_REG_GUILD_AD_REQ, KELG_MODIFY_REG_GUILD_AD_REQ );
		CASE( DBE_APPLY_JOIN_GUILD_REQ );
	   _CASE( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_REQ, KELG_ACCEPT_JOIN_GUILD_REQ );
		CASE( DBE_ACCEPT_JOIN_GUILD_REQ );
	   _CASE( DBE_DELETE_APPLY_JOIN_GUILD_REQ, KELG_DELETE_APPLY_JOIN_GUILD_REQ );
		CASE( DBE_DELETE_GUILD_AD_LIST_NOT );

#endif SERV_GUILD_AD   
		//}}
		//////////////////////////////////////////////////////////////////////////

		//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT	
	   _CASE( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_REQ, KDBE_GET_WEB_POINT_REWARD_ACK );
#endif SERV_WEB_POINT_EVENT
		//}}

		//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY
		CASE( DBE_INSERT_REWARD_TO_POST_REQ );

		CASE_NOPARAM( DBE_LOAD_PSHOP_AGENCY_REQ );
	   _CASE( DBE_OPEN_PSHOP_AGENCY_REQ, KERM_OPEN_PERSONAL_SHOP_ACK );
	   _CASE( DBE_BREAK_PSHOP_AGENCY_REQ, KERM_BREAK_PSHOP_AGENCY_NOT );
	   _CASE( DBE_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_ACK );
		CASE( DBE_BUY_PSHOP_AGENCY_ITEM_REQ );
		CASE( DBE_STOP_SALE_PSHOP_AGENCY_REQ );
		CASE( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ );
#endif SERV_PSHOP_AGENCY
		//}}
		//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
		CASE( DBE_INSERT_TITLE_REQ );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
		//}}

		//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
		CASE( DBE_CREATE_GUILD_REQ );
		CASE( DBE_CHANGE_GUILD_NAME_CHECK_REQ );
		CASE( DBE_CHANGE_GUILD_NAME_REQ );
#endif SERV_GUILD_CHANGE_NAME
		//}}
		//{{ 2012. 06. 05	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
	   _CASE( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ );
#endif SERV_BATTLE_FIELD_SYSTEM
		//}}
	   //{{ 2013. 3. 18	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
	   CASE_NOPARAM( DBE_GAME_LOCAL_RANKING_INIT_INFO_REQ );
	   _CASE( DBE_LOCAL_RANKING_RESET_REQ, int );
	   CASE( DBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT );
	   CASE_NOPARAM( DBE_LOCAL_RANKING_RESET_CHECK_REQ );
	   _CASE( DBE_GAME_LOCAL_RANKING_WATCH_UNIT_REQ, KELG_LOCAL_RANKING_WATCH_UNIT_REQ );
#endif SERV_LOCAL_RANKING_SYSTEM
	   //}}

	   //{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
	   CASE_NOPARAM( DBE_LOAD_WEDDING_HALL_INFO_REQ );
	   CASE( DBE_WEDDING_HALL_INFO_UPDATE_NOT );
	   _CASE( DBE_RELATIONSHIP_INFO_GAME_DB_REQ, KELG_RELATIONSHIP_INFO_REQ );
	   _CASE( DBE_WEDDING_ITEM_FIND_INFO_REQ, KELG_WEDDING_ITEM_FIND_INFO_REQ );
#endif SERV_RELATIONSHIP_SYSTEM
	   //}

	default:
        START_LOG( cerr, L"이벤트 핸들러가 정의되지 않았음. " << spEvent_->GetIDStr() );
    }

	//////////////////////////////////////////////////////////////////////////
#ifdef SP_PROFILER
	CALL_PROFILER_DUMP( SP_PROFILER_DUMP_TIME );
#endif SP_PROFILER
	//////////////////////////////////////////////////////////////////////////
}

void KLoginGameDBThread::SendToServer( unsigned short usEventID )
{
    SendToServer( usEventID, char() );
}

void KLoginGameDBThread::SendToUser( UidType nTo, unsigned short usEventID )
{
	SendToUser( nTo, usEventID, char() );
}

//{{ 2010. 02. 02  최육사	길드 게시판
#ifdef SERV_GUILD_AD

bool KLoginGameDBThread::DeleteApplyJoinGuildList( IN int iGuildUID, IN UidType iUnitUID )
{
	int iOK = NetError::ERR_ODBC_01;

	// 길드 가입 신청 삭제
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_BBS_Join_DEL", L"%d, %d", % iGuildUID % iUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_guild_bbs_join", L"%d, %d", % iGuildUID % iUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

end_proc:
	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 가입 신청 삭제 실패." )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( iGuildUID )
			<< BUILD_LOG( iUnitUID )
			<< END_LOG;
		return false;
	}

	return true;
}

bool KLoginGameDBThread::DeleteApplyJoinGuildList( IN const std::vector< KApplyDeleteInfo >& vecInfo )
{
	bool bRet = true;

	// 길드 가입 신청 삭제
	std::vector< KApplyDeleteInfo >::const_iterator vit;
	for( vit = vecInfo.begin(); vit != vecInfo.end(); ++vit )
	{
		if( DeleteApplyJoinGuildList( vit->m_iGuildUID, vit->m_iUnitUID ) == false )
		{
			START_LOG( cerr, L"길드 가입 신청 삭제 실패." )
				<< BUILD_LOG( vit->m_iGuildUID )
				<< BUILD_LOG( vit->m_iUnitUID )
				<< END_LOG;

			bRet = false;
		}
	}

	return bRet;
}

#endif SERV_GUILD_AD
//}}

//{{ 2009. 7. 6  최육사		랭킹 개편
IMPL_ON_FUNC_NOPARAM( DBE_GET_WEB_RANKING_INFO_REQ )
{
	KDBE_GET_WEB_RANKING_INFO_ACK kPacket;

	//////////////////////////////////////////////////////////////////////////
	// 던전 랭킹 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GRank_SEL_Exp" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.gup_get_rank_exp" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KDungeonRankingInfo kInfo;

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_iRank
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kInfo.m_iEXP );

		kPacket.m_vecDungeonRanking.push_back( kInfo );
	}

	//////////////////////////////////////////////////////////////////////////
	// 대전 랭킹 얻기
	//{{ 2011. 07. 26	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GRank_Vs" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.gup_get_rank_vs" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KPvpRankingInfo kInfo;
		int iPvpRank = 0;

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_iRank
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> iPvpRank
			>> kInfo.m_iWin
			>> kInfo.m_iRPoint );
		
		//{{ 2012. 09. 11	박세훈	2012 대전 시즌2
#ifdef SERV_2012_PVP_SEASON2
		kInfo.m_cRank	= static_cast<CXSLUnit::PVP_RANK>( iPvpRank );
#else
		kInfo.m_iRating = CXSLUnit::PvpRankToPvpEmblem( static_cast<CXSLUnit::PVP_RANK>(iPvpRank) );
#endif SERV_2012_PVP_SEASON2
		//}}

		kPacket.m_vecPvpRanking.push_back( kInfo );
	}
#else
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GRank_Vs" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.gup_get_rank_vs" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KPvpRankingInfo kInfo;
		int iiVsPointMax = 0;

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_iRank
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> iiVsPointMax
			>> kInfo.m_iWin
			>> kInfo.m_iLose );

		kInfo.m_cPvpEmblem = CXSLUnit::PvpEmblemToPvpEmblemEnum( CXSLUnit::GetPVPEmblem( iiVsPointMax ) );

		kPacket.m_vecPvpRanking.push_back( kInfo );
	}
#endif SERV_PVP_NEW_SYSTEM
	//}}	

end_proc:
	SendToServer( DBE_GET_WEB_RANKING_INFO_ACK, kPacket );
}
//}}

//{{ 2009. 11. 19  최육사	길드구조개선
IMPL_ON_FUNC( DBE_GET_GUILD_INFO_REQ )
{
	KDBE_GET_GUILD_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kLoginGuildMember = kPacket_.m_kLoginGuildMember;

	//////////////////////////////////////////////////////////////////////////
	// 길드 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_info", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kGuildInfo.m_wstrGuildName
			>> kPacket.m_kGuildInfo.m_usMaxNumMember
			>> kPacket.m_kGuildInfo.m_ucGuildLevel
			>> kPacket.m_kGuildInfo.m_iGuildEXP
			>> kPacket.m_kGuildInfo.m_wstrGuildMessage
			>> kPacket.m_kGuildInfo.m_wstrFoundingDay );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 정보 얻기 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_06;
	}
	else
	{
		// 길드 정보 얻기 성공
		kPacket.m_kGuildInfo.m_iGuildUID = kPacket_.m_iGuildUID;
	}

	//////////////////////////////////////////////////////////////////////////
	// 길드원 리스트 얻기
#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-26
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_SEL_ByGuildNo", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_GET", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KGuildMemberInfo kInfo;
		std::wstring wstrLogOutTime;
		KLocalRankingUnitInfo kUnitInfo;
		UidType	iUserUID = 0;

		FETCH_DATA( kInfo.m_iUnitUID
			>> iUserUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT]
			>> kUnitInfo.m_iPoint[SEnum::LRMTC_AP]
			>> kInfo.m_ucMemberShipGrade
			>> wstrLogOutTime
			>> kInfo.m_wstrMessage
			);

		CTime tLogOutTime;
		KncUtil::ConvertStringToCTime( wstrLogOutTime, tLogOutTime );
		kInfo.m_tLogOutTime = tLogOutTime.GetTime();

		kPacket.m_vecMemberList.push_back( kInfo );

		kUnitInfo.m_iUnitUID		= kInfo.m_iUnitUID;
		kUnitInfo.m_wstrNickName	= kInfo.m_wstrNickName;
		kUnitInfo.m_ucLevel			= kInfo.m_ucLevel;
		kUnitInfo.m_cUnitClass		= kInfo.m_cUnitClass;
		kPacket.m_mapRankerUnitInfo.insert( std::map<UidType, KLocalRankingUnitInfo>::value_type( kUnitInfo.m_iUnitUID, kUnitInfo ) );
		kPacket.m_mapRankerUIDInfo.insert( std::map<UidType, UidType>::value_type( kUnitInfo.m_iUnitUID, iUserUID ) );
	}
#else	// SERV_LOCAL_RANKING_SYSTEM
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Member_SEL_Info", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_member_info", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KGuildMemberInfo kInfo;
		std::wstring wstrLogOutTime;

		FETCH_DATA( kInfo.m_iUnitUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kInfo.m_ucMemberShipGrade
			>> wstrLogOutTime
			>> kInfo.m_wstrMessage
			);

		CTime tLogOutTime;
		KncUtil::ConvertStringToCTime( wstrLogOutTime, tLogOutTime );
		kInfo.m_tLogOutTime = tLogOutTime.GetTime();

		kPacket.m_vecMemberList.push_back( kInfo );
	}
#endif	// SERV_LOCAL_RANKING_SYSTEM

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	//////////////////////////////////////////////////////////////////////////
	// 캐시스킬 포인트 관련 정보 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_skill_point_info", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_kGuildSkillInfo.m_iGuildSPoint
			>> kPacket.m_kGuildSkillInfo.m_iGuildCSPoint
			>> kPacket.m_kGuildSkillInfo.m_iMaxGuildCSPoint
			>> kPacket.m_kGuildSkillInfo.m_wstrGuildCSPointEndDate );

		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"길드스킬포인트 정보 얻어오기 실패로 기본값을 넣어줌." )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_kGuildSkillInfo.m_iGuildSPoint = 0;
		kPacket.m_kGuildSkillInfo.m_iGuildCSPoint = 0;
		kPacket.m_kGuildSkillInfo.m_iMaxGuildCSPoint = 0;
		kPacket.m_kGuildSkillInfo.m_wstrGuildCSPointEndDate = L"2000-01-01 00:00:00";
	}
	
	// GUILD SKILL LIST 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Skill_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_skill_list_new", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KGuildSkillData kInfo;

		FETCH_DATA( kInfo.m_iSkillID
			>> kInfo.m_cSkillLevel
			>> kInfo.m_cSkillCSPoint
			);

		kPacket.m_kGuildSkillInfo.m_vecGuildSkill.push_back( kInfo );
	}

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////
#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-24
end_proc:
	KncSend( GetPfID(), 0, PI_ACCOUNT_DB, 0, anTrace_, DBE_ACCOUNT_GET_GUILD_INFO_REQ, kPacket );
#else	// SERV_LOCAL_RANKING_SYSTEM
end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_GUILD_INFO_ACK, kPacket );
#endif	// SERV_LOCAL_RANKING_SYSTEM
}

IMPL_ON_FUNC( DBE_GUILD_LEVEL_UP_REQ )
{
	KDBE_GUILD_LEVEL_UP_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
	kPacket.m_ucGuildLevel = kPacket_.m_ucGuildLevel;
	//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
	kPacket.m_iGuildSPoint = kPacket_.m_iGuildSPoint;
#endif GUILD_SKILL_TEST
	//}}

	//////////////////////////////////////////////////////////////////////////
	// 길드 LEVEL 업데이트
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_UPD_Level", L"%d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_ucGuildLevel
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_level", L"%d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_ucGuildLevel
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 레벨 업데이트 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOGc( kPacket_.m_ucGuildLevel )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_29; break; // 트랜젝션 실패
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

	//{{ 2009. 11. 26  최육사	길드스킬
#ifdef GUILD_SKILL_TEST
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER", L"%d, %d", % kPacket_.m_iGuildUID % kPacket_.m_iGuildSPoint );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_skill_point", L"%d, %d", % kPacket_.m_iGuildUID % kPacket_.m_iGuildSPoint );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 스킬 포인트 업데이트 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOGc( kPacket_.m_ucGuildLevel )
			<< BUILD_LOG( kPacket_.m_iGuildSPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드		
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

#endif GUILD_SKILL_TEST
	//}}

end_proc:
	SendToServer( DBE_GUILD_LEVEL_UP_ACK, kPacket );
}
//}}

//{{ 2009. 11. 24  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

IMPL_ON_FUNC( DBE_GET_GUILD_SKILL_REQ )
{
	KDBE_GET_GUILD_SKILL_ACK kPacket;
	kPacket.m_iOK				 = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID			 = kPacket_.m_iGuildUID;
	kPacket.m_iUnitUID			 = kPacket_.m_iUnitUID;
	kPacket.m_iGuildSkillID		 = kPacket_.m_iGuildSkillID;
	kPacket.m_iGuildSkillLevel	 = kPacket_.m_iGuildSkillLevel;
	kPacket.m_iGuildSkillCSPoint = kPacket_.m_iGuildSkillCSPoint;
	kPacket.m_iGuildSPoint		 = kPacket_.m_iGuildSPoint;
	kPacket.m_iGuildCSPoint		 = kPacket_.m_iGuildCSPoint;

	// 캐시 스킬 포인트 정보를 갱신
	if( kPacket_.m_iGuildCSPoint >= 0 )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_UPD", L"%d, %d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildCSPoint
			% kPacket_.m_iMaxGuildCSPoint
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_cash_skill_point_info", L"%d, %d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildCSPoint
			% kPacket_.m_iMaxGuildCSPoint
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}	

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"기간제 길드 스킬 포인트 DB업데이트 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )				
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iGuildCSPoint )
				<< BUILD_LOG( kPacket_.m_iMaxGuildCSPoint )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
			case -2: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_06; break; // 길드스킬 배우기 실패
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
			}

			goto end_proc;
		}
	}
	// 스킬 포인트 업데이트
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER", L"%d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildSPoint );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_skill_point", L"%d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildSPoint );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드 스킬 포인트 DB업데이트 실패!" )
				<< BUILD_LOG( kPacket.m_iOK )			
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iUnitUID )
				<< BUILD_LOG( kPacket_.m_iGuildSPoint )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
			case -2: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_06; break; // 길드스킬 배우기 실패
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
			}

			goto end_proc;
		}
	}
	
	// 스킬 획득 정보를 갱신
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Skill_MER", L"%d, %d, %d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildSkillID
		% kPacket_.m_iGuildSkillLevel
		% kPacket_.m_iGuildSkillCSPoint );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_skill_info_new", L"%d, %d, %d, %d",
		% kPacket_.m_iGuildUID
		% kPacket_.m_iGuildSkillID
		% kPacket_.m_iGuildSkillLevel
		% kPacket_.m_iGuildSkillCSPoint );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 스킬 배우기 DB업데이트 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildSkillID )
			<< BUILD_LOG( kPacket_.m_iGuildSkillLevel )
			<< BUILD_LOG( kPacket_.m_iGuildSkillCSPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_06; break;
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_06; break;
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}
		
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_GUILD_SKILL_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_INSERT_GUILD_CASH_SKILL_POINT_REQ )
{
	KDBE_INSERT_GUILD_CASH_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK					= NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID				= kPacket_.m_iGuildUID;	
	kPacket.m_iGuildCSPoint			= kPacket_.m_iGuildCSPoint;
	kPacket.m_iPeriod				= kPacket_.m_iPeriod;
	kPacket.m_bUpdateEndDateOnly	= kPacket_.m_bUpdateEndDateOnly;

	// 기간 연장인지? 새로 추가인지?
	if( kPacket_.m_bUpdateEndDateOnly )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_UPD_Period", L"%d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_cash_skill_period", L"%d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_wstrEndDate );
			m_kODBC.EndFetch();
		}	

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to insert guild cash skill point!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iGuildCSPoint )
				<< BUILD_LOG( kPacket_.m_iPeriod )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드			
			case -3: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_16; break;
			case -4: kPacket.m_iOK = NetError::ERR_GUILD_SKILL_16; break;
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
			}

			goto end_proc;	
		}
	}
	else
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER_Cash", L"%d, %d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildCSPoint
			% kPacket_.m_iPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_guild_cash_skill_point_info", L"%d, %d, %d",
			% kPacket_.m_iGuildUID
			% kPacket_.m_iGuildCSPoint
			% kPacket_.m_iPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK
				>> kPacket.m_wstrEndDate );
			m_kODBC.EndFetch();
		}	

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to insert guild cash skill point!" )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iGuildCSPoint )
				<< BUILD_LOG( kPacket_.m_iPeriod )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
			}

			goto end_proc;	
		}
	}

end_proc:
	SendToServer( DBE_INSERT_GUILD_CASH_SKILL_POINT_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_REQ )
{
	KDBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK				= NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID			= kPacket_.m_iGuildUID;
	kPacket.m_iRetrievedSPoint	= kPacket_.m_iRetrievedSPoint;

	// guild cash skill point 정보를 초기화 한다
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_UPD", L"%d, %d, %d", 
		% kPacket_.m_iGuildUID 
		% 0 
		% 0
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_cash_skill_point_info", L"%d, %d, %d", 
		% kPacket_.m_iGuildUID 
		% 0 
		% 0
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"failed to reset guild cash skill point!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

	// SP정보도 DB에 업데이트!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER", L"%d, %d", % kPacket_.m_iGuildUID % kPacket_.m_iGuildSPoint );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_skill_point", L"%d, %d", % kPacket_.m_iGuildUID % kPacket_.m_iGuildSPoint );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 스킬 포인트 업데이트 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )			
			<< BUILD_LOG( kPacket_.m_iGuildSPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드		
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

	// note!! 300개 이상 skill정보가 바뀌는 경우가 있을까? 로그만 남기기.
	if( kPacket_.m_vecGuildSkillData.size() > 300 )
	{
		START_LOG( cerr, L"too many skill update on cash skill point expiration!" )
			<< BUILD_LOG( (int) kPacket_.m_vecGuildSkillData.size() )
			<< END_LOG;
	}

	for( u_int ui = 0; ui < kPacket_.m_vecGuildSkillData.size(); ++ui )
	{
		const KGuildSkillData& kGuildSkillData = kPacket_.m_vecGuildSkillData[ui];
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_Skill_MER", L"%d, %d, %d, %d",
			% kPacket_.m_iGuildUID
			% kGuildSkillData.m_iSkillID
			% kGuildSkillData.m_cSkillLevel
			% kGuildSkillData.m_cSkillCSPoint );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_guild_skill_info_new", L"%d, %d, %d, %d",
			% kPacket_.m_iGuildUID
			% kGuildSkillData.m_iSkillID
			% kGuildSkillData.m_cSkillLevel
			% kGuildSkillData.m_cSkillCSPoint );
#endif //SERV_ALL_RENEWAL_SP
		int iResult = NetError::ERR_UNKNOWN;
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iResult );
			m_kODBC.EndFetch();
		}

		if( iResult != NetError::NET_OK )
		{
			START_LOG( cerr, L"failed to update skill on cash skill point expire!" )
				<< BUILD_LOG( kPacket.m_iOK )				
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_iRetrievedSPoint )
				<< BUILD_LOG( kGuildSkillData.m_iSkillID )
				<< BUILD_LOG( kGuildSkillData.m_cSkillLevel )
				<< BUILD_LOG( kGuildSkillData.m_cSkillCSPoint )
				<< END_LOG;

			switch( kPacket.m_iOK )
			{
			case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
			default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
			}
		}
	}

end_proc:
	SendToServer( DBE_EXPIRE_GUILD_CASH_SKILL_POINT_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_ADMIN_GET_GUILD_SKILL_POINT_REQ, KELG_ADMIN_GET_GUILD_SKILL_POINT_REQ )
{
	KELG_ADMIN_GET_GUILD_SKILL_POINT_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iGuildSPoint = kPacket_.m_iGuildSPoint;

	// 길드 스킬 포인트 올리자!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER", L"%d, %d", % kPacket_.m_iGuildUID % kPacket_.m_iGuildSPoint );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_skill_point", L"%d, %d", % kPacket_.m_iGuildUID % kPacket_.m_iGuildSPoint );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 스킬 포인트 업데이트 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )			
			<< BUILD_LOG( kPacket_.m_iGuildSPoint )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드		
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

end_proc:
	SendToServer( DBE_ADMIN_GET_GUILD_SKILL_POINT_ACK, kPacket );
}

#endif GUILD_SKILL_TEST
//}}

//////////////////////////////////////////////////////////////////////////   
//{{ 2010. 01. 13  최육사	길드게시판
#ifdef SERV_GUILD_AD

IMPL_ON_FUNC_NOPARAM( DBE_GET_GUILD_BOARD_INFO_REQ )
{
	KDBE_GET_GUILD_BOARD_INFO_ACK kPacket;	

	// 길드 광고 리스트 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GGuild_BBS_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.gup_get_guild_bbs_list" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KGuildAdInfo kInfo;

		FETCH_DATA( kInfo.m_iGuildUID
			>> kInfo.m_wstrGuildName
			>> kInfo.m_ucGuildLevel
			>> kInfo.m_wstrMasterNickName
			>> kInfo.m_usCurNumMember
			>> kInfo.m_wstrAdMessage
			//{{ 2010. 02. 04  최육사	길드 광고 오류 수정
			>> kInfo.m_wstrFoundingDay
			//}}
			>> kInfo.m_wstrAdRegDate
			>> kInfo.m_wstrAdEndDate );

		kPacket.m_vecGuildAdList.push_back( kInfo );
	}

	// 길드 광고 등록 유저 얻기	
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GGuild_BBS_Join_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.gup_get_guild_bbs_join_list" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KApplyJoinGuildInfo kInfo;

		FETCH_DATA( kInfo.m_iApplyJoinGuildUID
			>> kInfo.m_iUnitUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cUnitClass
			>> kInfo.m_ucLevel
			>> kInfo.m_wstrMessage
			>> kInfo.m_wstrRegDate );

		kPacket.m_vecApplyJoinGuildList.push_back( kInfo );
	}	
    
end_proc:
	SendToServer( DBE_GET_GUILD_BOARD_INFO_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_GET_GUILD_SKILL_IN_BOARD_REQ, KELG_GET_GUILD_SKILL_IN_BOARD_REQ )
{
	KDBE_GET_GUILD_SKILL_IN_BOARD_ACK kPacket;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;

	//////////////////////////////////////////////////////////////////////////
	//{{ 2009. 11. 25  최육사	길드스킬
#ifdef GUILD_SKILL_TEST

	//////////////////////////////////////////////////////////////////////////
	// 캐시스킬 포인트 관련 정보 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_skill_point_info", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_kGuildSkillInfo.m_iGuildSPoint
			>> kPacket.m_kGuildSkillInfo.m_iGuildCSPoint
			>> kPacket.m_kGuildSkillInfo.m_iMaxGuildCSPoint
			>> kPacket.m_kGuildSkillInfo.m_wstrGuildCSPointEndDate );

		m_kODBC.EndFetch();
	}
	else
	{
		START_LOG( cerr, L"길드스킬포인트 정보 얻어오기 실패로 기본값을 넣어줌." )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_kGuildSkillInfo.m_iGuildSPoint = 0;
		kPacket.m_kGuildSkillInfo.m_iGuildCSPoint = 0;
		kPacket.m_kGuildSkillInfo.m_iMaxGuildCSPoint = 0;
		kPacket.m_kGuildSkillInfo.m_wstrGuildCSPointEndDate = L"2000-01-01 00:00:00";
	}

	// GUILD SKILL LIST 받아오기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Skill_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_skill_list_new", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KGuildSkillData kInfo;

		FETCH_DATA( kInfo.m_iSkillID
			>> kInfo.m_cSkillLevel
			>> kInfo.m_cSkillCSPoint
			);

		kPacket.m_kGuildSkillInfo.m_vecGuildSkill.push_back( kInfo );
	}

#endif GUILD_SKILL_TEST
	//}}
	//////////////////////////////////////////////////////////////////////////	

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_GET_GUILD_SKILL_IN_BOARD_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_REGISTRATION_GUILD_AD_REQ )
{
	KDBE_REGISTRATION_GUILD_AD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	kPacket.m_iCost = kPacket_.m_iCost;
	kPacket.m_bExistExpiredAd = kPacket_.m_bExistExpiredAd;

	// 기존에 기간만료된 광고가 있다면 먼저 삭제하자!
	if( kPacket_.m_bExistExpiredAd )
	{
		// 길드 광고 삭제
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GGuild_BBS_DEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_guild_bbs", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"길드 광고 삭제 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< END_LOG;

			kPacket.m_iOK = NetError::ERR_GUILD_BOARD_06;
			goto end_proc;
		}
	}

	// 길드 광고를 등록하자!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_BBS_INS", L"%d, N\'%s\', %d", % kPacket_.m_iGuildUID % kPacket_.m_wstrAdMessage % kPacket_.m_sPeriod );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_guild_bbs", L"%d, N\'%s\', %d", % kPacket_.m_iGuildUID % kPacket_.m_wstrAdMessage % kPacket_.m_sPeriod );
#endif //SERV_ALL_RENEWAL_SP

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrAdRegDate
			>> kPacket.m_wstrAdEndDate );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 광고 등록 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< BUILD_LOG( kPacket_.m_wstrAdMessage )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_BOARD_06; break; // 존재하지않는 길드
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_REGISTRATION_GUILD_AD_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_MODIFY_REG_GUILD_AD_REQ, KELG_MODIFY_REG_GUILD_AD_REQ )
{
	KDBE_MODIFY_REG_GUILD_AD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_sPeriod = kPacket_.m_sPeriod;
	kPacket.m_wstrAdMessage = kPacket_.m_wstrAdMessage;
	kPacket.m_iCost = kPacket_.m_iCost;

	// 길드 광고 삭제
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_BBS_DEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_delete_guild_bbs", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 광고 삭제 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		goto end_proc;
	}

	// 길드 광고를 등록하자!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_BBS_INS", L"%d, N\'%s\', %d", % kPacket_.m_iGuildUID % kPacket_.m_wstrAdMessage % kPacket_.m_sPeriod );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_guild_bbs", L"%d, N\'%s\', %d", % kPacket_.m_iGuildUID % kPacket_.m_wstrAdMessage % kPacket_.m_sPeriod );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrAdRegDate
			>> kPacket.m_wstrAdEndDate );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 광고 등록 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< BUILD_LOG( kPacket_.m_wstrAdMessage )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_BOARD_06; break; // 존재하지않는 길드
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_MODIFY_REG_GUILD_AD_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_APPLY_JOIN_GUILD_REQ )
{
	KDBE_APPLY_JOIN_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_kApplyJoinGuildInfo = kPacket_.m_kApplyJoinGuildInfo;

	// 길드 가입 신청 삭제
	if( DeleteApplyJoinGuildList( kPacket_.m_vecDeleteApply ) == false )
	{
		START_LOG( cerr, L"길드 가입 신청 삭제 실패." )
			<< BUILD_LOG( kPacket_.m_vecDeleteApply.size() )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_12;
		goto end_proc;
	}

	// 길드 가입 신청을 등록하자!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_BBS_Join_INS", L"%d, %d, N\'%s\'",
		% kPacket_.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID
		% kPacket_.m_kApplyJoinGuildInfo.m_iUnitUID
		% kPacket_.m_kApplyJoinGuildInfo.m_wstrMessage );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_guild_bbs_join", L"%d, %d, N\'%s\'",
		% kPacket_.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID
		% kPacket_.m_kApplyJoinGuildInfo.m_iUnitUID
		% kPacket_.m_kApplyJoinGuildInfo.m_wstrMessage );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kApplyJoinGuildInfo.m_wstrRegDate );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 광고 등록 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_kApplyJoinGuildInfo.m_iApplyJoinGuildUID )
			<< BUILD_LOG( kPacket_.m_kApplyJoinGuildInfo.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_kApplyJoinGuildInfo.m_wstrMessage )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_BOARD_08; break; // 존재하지않는 길드
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_APPLY_JOIN_GUILD_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_REQ, KELG_ACCEPT_JOIN_GUILD_REQ )
{
	KDBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iApplyJoinUnitUID = kPacket_.m_iApplyJoinUnitUID;
	
	// 길드 가입 신청 삭제
	if( DeleteApplyJoinGuildList( kPacket_.m_iGuildUID, kPacket_.m_iApplyJoinUnitUID ) == false )
	{
		START_LOG( cerr, L"길드 가입 신청 삭제 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iApplyJoinUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_11;
		goto end_proc;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_DELETE_APLLY_FOR_ACCEPT_JOIN_GUILD_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_ACCEPT_JOIN_GUILD_REQ )
{
    KDBE_ACCEPT_JOIN_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_kJoinGuildMemberInfo = kPacket_.m_kJoinGuildMemberInfo;

	//////////////////////////////////////////////////////////////////////////
	// 길드 가입
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuildMember_INS", L"%d, %d, N\'%s\'", 
		% kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID
		% kPacket_.m_iGuildUID
		% L""
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild_member", L"%d, %d, N\'%s\'", 
		% kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID
		% kPacket_.m_iGuildUID
		% L""
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )	
	{
		if( kPacket.m_iOK == -3 )
		{
			START_LOG( cwarn, L"길드 최대 가입 인원 부족." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID )
				<< END_LOG;
		}
		else
		{
			START_LOG( cerr, L"길드 가입 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( kPacket_.m_iGuildUID )
				<< BUILD_LOG( kPacket_.m_kJoinGuildMemberInfo.m_iUnitUID )
				<< END_LOG;
		}

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_14; break; // 이미 길드에 가입되어있음
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_18; break; // 길드 가입 실패
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_18; break; // 길드 가입 최대 인원 제한
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}
	}

end_proc:
	SendToServer( DBE_ACCEPT_JOIN_GUILD_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_DELETE_APPLY_JOIN_GUILD_REQ, KELG_DELETE_APPLY_JOIN_GUILD_REQ )
{
	KDBE_DELETE_APPLY_JOIN_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::NET_OK;
	kPacket.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iDeletedUnitUID = kPacket_.m_iDeletedUnitUID;
	kPacket.m_bDeleteChar = kPacket_.m_bDeleteChar;
	
	// 길드 가입 신청 삭제
	if( DeleteApplyJoinGuildList( kPacket_.m_iGuildUID, kPacket_.m_iDeletedUnitUID ) == false )
	{
		START_LOG( cerr, L"길드 가입 신청 삭제 실패." )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iDeletedUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_GUILD_BOARD_12;
	}

	SendToUser( LAST_SENDER_UID, DBE_DELETE_APPLY_JOIN_GUILD_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_DELETE_GUILD_AD_LIST_NOT )
{
	{ // 길드 광고 삭제
		std::vector< int >::const_iterator vit;
		for( vit = kPacket_.m_vecDeleteGuildAd.begin(); vit != kPacket_.m_vecDeleteGuildAd.end(); ++vit )
		{
			int iOK = NetError::ERR_ODBC_01;

			// 길드 광고 삭제
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GGuild_BBS_DEL", L"%d", % *vit );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_delete_guild_bbs", L"%d", % *vit );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

end_proc:
			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"길드 광고 삭제 실패." )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( *vit )
					<< END_LOG;
			}
		}
	}

	// 길드 가입 신청 삭제
	if( DeleteApplyJoinGuildList( kPacket_.m_vecDeleteApply ) == false )
	{
        START_LOG( cerr, L"길드 가입 신청 삭제 실패!" )
			<< BUILD_LOG( kPacket_.m_vecDeleteApply.size() )
			<< END_LOG;
	}
}

#endif SERV_GUILD_AD   
//}}
//////////////////////////////////////////////////////////////////////////

//{{ 2010. 02. 23  최육사	웹 포인트 이벤트
#ifdef SERV_WEB_POINT_EVENT	

_IMPL_ON_FUNC( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_REQ, KDBE_GET_WEB_POINT_REWARD_ACK )
{
	std::vector< KWebPointRewardInfo >::iterator vit;
	for( vit = kPacket_.m_vecWebPointReward.begin(); vit != kPacket_.m_vecWebPointReward.end(); ++vit )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % vit->m_wstrNickName );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % vit->m_wstrNickName );
#endif //SERV_ALL_RENEWAL_SP
		UidType iUnitUID = 0;
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iUnitUID );
			m_kODBC.EndFetch();
		}

		// UnitUID를 저장한다.
		if( iUnitUID > 0 )
		{
			vit->m_iUnitUID = iUnitUID;
		}
	}

end_proc:
	SendToServer( DBE_GET_WEB_POINT_REWARD_CHECK_NICKNAME_ACK, kPacket_ );
}

#endif SERV_WEB_POINT_EVENT
//}}

//{{ 2011. 04. 29	최육사	대리상인
#ifdef SERV_PSHOP_AGENCY

IMPL_ON_FUNC( DBE_INSERT_REWARD_TO_POST_REQ )
{
	KDBE_INSERT_REWARD_TO_POST_ACK kPacket;
	kPacket.m_iOK					  	   = NetError::ERR_ODBC_01;
	kPacket.m_iRewardLetter.m_iFromUnitUID = kPacket_.m_iFromUnitUID;
	kPacket.m_iRewardLetter.m_iToUnitUID   = kPacket_.m_iToUnitUID;
	kPacket.m_iRewardLetter.m_cScriptType  = kPacket_.m_iRewardType;
	kPacket.m_iRewardLetter.m_iScriptIndex = kPacket_.m_iRewardID;
	kPacket.m_iRewardLetter.m_iQuantity	   = kPacket_.m_sQuantity;
	kPacket.m_iRewardLetter.m_wstrMessage  = kPacket_.m_wstrMessage;

	// 보상
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPost_INS", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_sQuantity
		% kPacket_.m_iRewardType
		% kPacket_.m_iRewardID
		% kPacket_.m_wstrMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_insert_post_item", L"%d, %d, %d, %d, %d, N\'%s\'",
		% kPacket_.m_iFromUnitUID
		% kPacket_.m_iToUnitUID
		% kPacket_.m_sQuantity
		% kPacket_.m_iRewardType
		% kPacket_.m_iRewardID
		% kPacket_.m_wstrMessage
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_iRewardLetter.m_iPostNo
			>> kPacket.m_iRewardLetter.m_wstrRegDate );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		switch( kPacket.m_iOK )
		{
		case -1:    kPacket.m_iOK = NetError::ERR_REWARD_TO_POST_00;   break;
		}

		goto end_proc;
	}

end_proc:	
	SendToServer( DBE_INSERT_REWARD_TO_POST_ACK, kPacket );
}

IMPL_ON_FUNC_NOPARAM( DBE_LOAD_PSHOP_AGENCY_REQ )
{
	KDBE_LOAD_PSHOP_AGENCY_ACK kPacket;

	// 대리 상점 정보 얻기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GPShopInfo_SEL_All" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.gup_get_PShop_info" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT kInfo;

		FETCH_DATA( kInfo.m_iUserUID
			>> kInfo.m_iUnitUID
			>> kInfo.m_wstrNickName
			>> kInfo.m_cPersonalShopType
			>> kInfo.m_wstrPersonalShopName
			>> kInfo.m_wstrAgencyOpenDate
			>> kInfo.m_wstrAgencyExpirationDate
			>> kInfo.m_bOnSale
			);

		kPacket.m_vecOpenPShopAgency.push_back( kInfo );
	}

	// 대리 상점 인벤 정보 얻기
	{
		BOOST_TEST_FOREACH( KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT&, kInfo, kPacket.m_vecOpenPShopAgency )
		{
			// 상점 판매 물품 얻기
#ifdef SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX // 김민성 // 적용날짜: 2013-07-11
			DO_QUERY( L"exec dbo.P_GPShopItem_SEL", L"%d", % kInfo.m_iUnitUID );
#else	// SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX
			DO_QUERY( L"exec dbo.gup_get_PShopItem_Inventory_by_UnitUID", L"%d", % kInfo.m_iUnitUID );
#endif // SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX
			while( m_kODBC.Fetch() )
			{
				KSellPShopItemBackupData kItemInfo;
				kItemInfo.m_kSellPShopItemInfo.m_cPShopItemType = KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY;
				
				//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
				const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
				const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS
				int arrSocketOption[byteArraySize];
				memset( arrSocketOption, 0, sizeof(int) * byteArraySize );
#else
				short arrSocketOption[4] = {0};
#endif SERV_ITEM_OPTION_DATA_SIZE
				//}} 

#ifdef SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX // 김민성 // 적용날짜: 2013-07-11
				int arrRandomSocketOption[5] = {0};

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
				FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
					>> arrSocketOption[0]
					>> arrSocketOption[1]
					>> arrSocketOption[2]
					>> arrSocketOption[3]
					>> arrSocketOption[4]
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
					>> arrRandomSocketOption[0]
					>> arrRandomSocketOption[1]
					>> arrRandomSocketOption[2]
					>> arrRandomSocketOption[3]
					>> arrRandomSocketOption[4]
					>> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
					>> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
					>> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
					>> kItemInfo.m_iTotalSoldItemQuantity
					>> kItemInfo.m_iTotalSellCommissionED
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory					
					);
#else // SERV_BATTLE_FIELD_BOSS
				FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
					>> arrSocketOption[0]
					>> arrSocketOption[1]
					>> arrSocketOption[2]
					>> arrSocketOption[3]
					>> arrRandomSocketOption[0]
					>> arrRandomSocketOption[1]
					>> arrRandomSocketOption[2]
					>> arrRandomSocketOption[3]
					>> arrRandomSocketOption[4]
					>> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
					>> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
					>> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
					>> kItemInfo.m_iTotalSoldItemQuantity
					>> kItemInfo.m_iTotalSellCommissionED
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory					
					);
#endif // SERV_BATTLE_FIELD_BOSS

					int iCheckRandomIdx;
					for( iCheckRandomIdx = 4; iCheckRandomIdx >= 0; --iCheckRandomIdx )
					{
						if( arrRandomSocketOption[iCheckRandomIdx] != 0 )
							break;
					}

					for( int iIdx = 0; iIdx <= iCheckRandomIdx; ++iIdx )
					{
						kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecRandomSocket.push_back( arrRandomSocketOption[iIdx] );
					}

#else	// SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX

				FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
					>> arrSocketOption[0]
					>> arrSocketOption[1]
					>> arrSocketOption[2]
					>> arrSocketOption[3]
					>> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
					>> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
					>> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
					>> kItemInfo.m_iTotalSoldItemQuantity
					>> kItemInfo.m_iTotalSellCommissionED
					>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory
					);
#endif	// SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX

				int iCheckIdx = byteArraySize;
				while( 0 <= --iCheckIdx )
				{
					if( arrSocketOption[iCheckIdx] != 0 )
						break;
				}

				for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
				{
					kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );
				}

				kInfo.m_vecSellItemInfo.push_back( kItemInfo );
			}
		}
	}

	// 대리상점이 정상적으로 개설된건지 체크하기!
	{
		BOOST_TEST_FOREACH( const KERM_OPEN_PSHOP_AGENCY_BY_SERVER_NOT&, kInfo, kPacket.m_vecOpenPShopAgency )
		{
			if( kInfo.m_vecSellItemInfo.empty() == false )
				continue;

			START_LOG( cerr, L"등록된 아이템이 없는데 개설된 상태입니다! 강제로 상점을 닫습니다!" )
				<< BUILD_LOG( kInfo.m_iUserUID )
				<< BUILD_LOG( kInfo.m_iUnitUID )
				<< BUILD_LOG( kInfo.m_wstrNickName )
				<< BUILD_LOG( kInfo.m_wstrAgencyExpirationDate )
				<< BUILD_LOG( kInfo.m_wstrAgencyOpenDate )
				<< BUILD_LOG( kInfo.m_vecSellItemInfo.size() )
				<< END_LOG;				

			int iOK = NetError::ERR_ODBC_01;

			// 대리 상인 닫기
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GPShopInfo_UPD_IsPShopOpen", L"%d, %d", 
				% kInfo.m_iUnitUID
				% false
				);
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.gup_update_PShop_info_IsPShopOpen", L"%d, %d", 
				% kInfo.m_iUnitUID
				% false
				);
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK );
				m_kODBC.EndFetch();
			}

			if( iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"대리 상인 닫기 실패!" )
					<< BUILD_LOG( iOK )
					<< BUILD_LOG( kInfo.m_iUnitUID )
					<< END_LOG;
			}
		}
	}

end_proc:
	SendToServer( DBE_LOAD_PSHOP_AGENCY_ACK, kPacket );
}

_IMPL_ON_FUNC( DBE_OPEN_PSHOP_AGENCY_REQ, KERM_OPEN_PERSONAL_SHOP_ACK )
{
	kPacket_.m_iOK = NetError::ERR_ODBC_01;

	//////////////////////////////////////////////////////////////////////////
	// 대리 상인 UID 등록
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPShopInfo_UPD_PShopInfo", L"%d, %d, N\'%s\', %d, %d",
		% kPacket_.m_iUnitUID
		% true
		% kPacket_.m_wstrAgencyOpenDate
		% true
		% (int)kPacket_.m_cPersonalShopType
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_PShopinfo", L"%d, %d, N\'%s\', %d, %d",
		% kPacket_.m_iUnitUID
		% true
		% kPacket_.m_wstrAgencyOpenDate
		% true
		% (int)kPacket_.m_cPersonalShopType
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상인 UID 등록 실패!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iPersonalShopUID )
			<< BUILD_LOG( kPacket_.m_wstrAgencyOpenDate )
			<< BUILD_LOGc( kPacket_.m_cPersonalShopType )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_PERSONAL_SHOP_44;
		goto end_proc;
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_OPEN_PSHOP_AGENCY_ACK, kPacket_ );
}

_IMPL_ON_FUNC( DBE_BREAK_PSHOP_AGENCY_REQ, KERM_BREAK_PSHOP_AGENCY_NOT )
{
	int iOK = NetError::ERR_ODBC_01;

	// 대리 상인 닫기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPShopInfo_UPD_IsPShopOpen", L"%d, %d", 
		% kPacket_.m_iHostUID
		% false
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_PShop_info_IsPShopOpen", L"%d, %d", 
		% kPacket_.m_iHostUID
		% false
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iOK );
		m_kODBC.EndFetch();
	}

	if( iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상인 닫기 실패!" )
			<< BUILD_LOG( iOK )
			<< BUILD_LOG( kPacket_.m_iHostUID )
			<< END_LOG;

		iOK = NetError::ERR_UNKNOWN;
	}

end_proc:
	SendToServer( DBE_BREAK_PSHOP_AGENCY_ACK, kPacket_ );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
_IMPL_ON_FUNC( DBE_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_ACK )
{
	// SP 쿼리를 만들어 보자
	//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef SERV_ALL_RENEWAL_SP
	std::wstring wstrQuerySP = L"exec dbo.P_GPShopInfo_INS_TradeNew ";
#else //SERV_ALL_RENEWAL_SP
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Pshop_New ";
#endif //SERV_ALL_RENEWAL_SP
#else
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Pshop ";
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	int iTemp_Type = 2;		// 대리 상점 아이템 등록 sp 타입
	int iTemp_Zero = 0;

	// 대리 상점 이름 등록
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPShopInfo_UPD_PShopName", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrPersonalShopName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_PShop_info_PShopName", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrPersonalShopName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상점 이름 등록 실패!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrPersonalShopName )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_PERSONAL_SHOP_45;
		goto end_proc;
	}

	wstrQuerySP += boost::str( boost::wformat( L"%d" ) % iTemp_Type );

	// 판매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iUnitUID );

	// 구매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );

	// 판매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// 수수료가 다르기 때문에 각자 처리 해야한다.

	// 구매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// 수수료가 다르기 때문에 각자 처리 해야한다.

	// 등록 아이템
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kSellItemInfo,  kPacket_.m_vecSellItemInfo)
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kSellItemInfo.m_kInventoryItemInfo.m_iItemUID );								// ItemUID
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity );				// Quantity
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kSellItemInfo.m_iPricePerOne );												// price
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % static_cast<int>(kSellItemInfo.m_kInventoryItemInfo.m_cSlotCategory) );		// category
	}

	for(int i = static_cast<int>(kPacket_.m_vecSellItemInfo.size()) ; i < SEnum::PAE_SELL_ITEM_LIMIT_NUM ; ++i )// 아이템이 빈 곳을 채워주는 곳
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// ItemUID	
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// Quantity
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// price
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// category
	}
	
	// 상점 이름
	wstrQuerySP += boost::str( boost::wformat( L", N\'%s\'" ) % kPacket_.m_wstrPersonalShopName );

	DO_QUERY_NO_ARG( wstrQuerySP.c_str() );
	
	// 아이템은 9개 이므로 9개로 고정한다.
	UidType iItemUID[SEnum::PAE_SELL_ITEM_LIMIT_NUM] = {0,};

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK
			>> iItemUID[0]			// 판매되는 아이템
			>> iItemUID[1]
			>> iItemUID[2]
			>> iItemUID[3]
			>> iItemUID[4]
			>> iItemUID[5]
			>> iItemUID[6]
			>> iItemUID[7]
			>> iItemUID[8]	
            >> iItemUID[9]
            >> iItemUID[10]
            >> iItemUID[11]
            >> iItemUID[12]
            >> iItemUID[13]
            >> iItemUID[14]
            );

		m_kODBC.EndFetch();

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
            // slect pshopitem..
#ifdef SERV_UPGRADE_TRADE_SYSTEM // 해외팀 추가
            GetPShopItem( kPacket_.m_iUnitUID, kPacket_.m_vecAddCompleteItemInfo );
#endif //SERV_UPGRADE_TRADE_SYSTEM

            START_LOG( cerr, L"대리상점 등록 SP 호출이 실패 했습니다." )
                << BUILD_LOG( kPacket_.m_iOK )
                << BUILD_LOG( kPacket_.m_iUnitUID )
                << BUILD_LOG( GET_LAST_QUERY() )
                << BUILD_LOG( kPacket_.m_vecAddCompleteItemInfo.size() )
                << END_LOG;
            
			goto end_proc;
		}

		int index = 0;
		BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kSellItemInfo,  kPacket_.m_vecSellItemInfo)
		{
			KSellPersonalShopItemInfo kItemTemp;
			kItemTemp = kSellItemInfo;
			kItemTemp.m_cPShopItemType = KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY;
			kItemTemp.m_kInventoryItemInfo.m_iItemUID = iItemUID[index++];
			kPacket_.m_vecAddCompleteItemInfo.push_back( kItemTemp );
		}
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_REG_PSHOP_AGENCY_ITEM_ACK, kPacket_ );
}
#else
_IMPL_ON_FUNC( DBE_REG_PSHOP_AGENCY_ITEM_REQ, KERM_REG_PERSONAL_SHOP_ITEM_ACK )
{
	// 대리 상점 이름 등록
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPShopInfo_UPD_PShopName", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrPersonalShopName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_PShop_info_PShopName", L"%d, N\'%s\'",
		% kPacket_.m_iUnitUID
		% kPacket_.m_wstrPersonalShopName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상점 이름 등록 실패!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrPersonalShopName )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_PERSONAL_SHOP_45;
		goto end_proc;
	}

	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kInsertItemInfo, kPacket_.m_vecSellItemInfo )
	{
		//////////////////////////////////////////////////////////////////////////
		// 아이템 생성
		UidType iAgencyItemUID = 0;
		kPacket_.m_iOK = NetError::ERR_ODBC_01;

		int arrSocketInfo[4] = {0,0,0,0}; // DB 테이블의 소켓테이블 참고
		//{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
		std::vector< int >::const_iterator vit;
#else
		std::vector< short >::const_iterator vit;
#endif SERV_ITEM_OPTION_DATA_SIZE
		//}} 
		vit = kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.begin();
		for( int iIdx = 0; iIdx < 4; ++iIdx )
		{
			if( vit == kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.end() )
				continue;

			arrSocketInfo[iIdx] = *vit;
			++vit;
		}
		DO_QUERY( L"exec dbo.gup_insert_PShopItem", 
			L"%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
			L"%d, %d, %d, %d, %d, %d, %d, %d",
			% kInsertItemInfo.m_kInventoryItemInfo.m_iItemUID
			% kPacket_.m_iUnitUID
			% kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
			% (int)kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
			% kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
			% kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
			% (int)kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
			% (int)kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
			% (int)kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
			% (int)kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
			% (int)kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
			% arrSocketInfo[0]
			% arrSocketInfo[1]
			% arrSocketInfo[2]
			% arrSocketInfo[3]
			% (int)kInsertItemInfo.m_iPricePerOne
				% (int)kInsertItemInfo.m_iFeePerOne
				% (int)kInsertItemInfo.m_kInventoryItemInfo.m_cSlotCategory
				);

			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( kPacket_.m_iOK
					>> iAgencyItemUID );

				m_kODBC.EndFetch();
			}

			if( kPacket_.m_iOK != NetError::NET_OK )
			{
				START_LOG( cerr, L"아이템 삽입 실패." )
					<< BUILD_LOG( kPacket_.m_iOK )
					<< BUILD_LOG( kInsertItemInfo.m_kInventoryItemInfo.m_iItemUID )
					<< BUILD_LOG( kPacket_.m_iUnitUID )
					<< BUILD_LOG( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID )
					<< BUILD_LOGc( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType )
					<< BUILD_LOG( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity )
					<< BUILD_LOG( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance )
					<< BUILD_LOGc( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData )
					<< BUILD_LOGc( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel )
					<< BUILD_LOGc( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0 )
					<< BUILD_LOGc( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1 )
					<< BUILD_LOGc( kInsertItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2 )
					<< BUILD_LOG( arrSocketInfo[0] )
					<< BUILD_LOG( arrSocketInfo[1] )
					<< BUILD_LOG( arrSocketInfo[2] )
					<< BUILD_LOG( arrSocketInfo[3] )
					<< BUILD_LOG( kInsertItemInfo.m_iPricePerOne )
					<< BUILD_LOG( kInsertItemInfo.m_iFeePerOne )
					<< BUILD_LOGc( kInsertItemInfo.m_kInventoryItemInfo.m_cSlotCategory )
					<< END_LOG;
			}
			else
			{
				kInsertItemInfo.m_kInventoryItemInfo.m_iItemUID = iAgencyItemUID;
				kInsertItemInfo.m_cPShopItemType = KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY;
			}
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_REG_PSHOP_AGENCY_ITEM_ACK, kPacket_ );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
IMPL_ON_FUNC( DBE_BUY_PSHOP_AGENCY_ITEM_REQ )
{
	kPacket_.m_iOK = NetError::ERR_ODBC_01;

	// SP 쿼리를 만들어 보자
	int iTemp_Type = 3;		// 대리 상점 sp 타입 - 물품 구매
	int iTemp_Zero = 0;
	//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef SERV_ALL_RENEWAL_SP
	std::wstring wstrQuerySP = L"exec dbo.P_GPShopInfo_INS_TradeNew ";
#else //SERV_ALL_RENEWAL_SP
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Pshop_New ";
#endif //SERV_ALL_RENEWAL_SP
#else
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Pshop ";
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	wstrQuerySP += boost::str( boost::wformat( L"%d" ) % iTemp_Type );

	// 판매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iHostUnitUID );

	// 구매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iPriceUnitUID );

	// 판매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );				// 판매자는 회수때 처리

	// 구매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iPriceUnitEDOUT );		// 수수료가 다르기 때문에 각자 처리 해야한다.

	// 판매자 아이템
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_kSellItemDBUpdate.m_iItemUID );	// itemuid - 대리상점 itemuid
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_kSellItemDBUpdate.m_kItemInfo.m_iQuantity );	// quantity
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % 0 );												// price
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % 0 );												// category

	// 구매자 아이템 - 구매자는 아이템이 들어오기만 하기 때문에 없다.
	int index = 1;			// 물건 하나를 등록 했으므로 1로 초기화한다.
	for( ; index < SEnum::PAE_SELL_ITEM_LIMIT_NUM ; ++index )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );
	}

	std::wstring wstrTemp;
	wstrQuerySP += boost::str( boost::wformat( L", N\'%s\'" ) % wstrTemp );

	DO_QUERY_NO_ARG( wstrQuerySP.c_str() );

	// 아이템은 9개 이므로 9개로 고정한다.
	UidType iItemUID[SEnum::PAE_SELL_ITEM_LIMIT_NUM] = {0,};

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK
            >> iItemUID[0]
            >> iItemUID[1]
            >> iItemUID[2]
            >> iItemUID[3]
            >> iItemUID[4]
            >> iItemUID[5]
            >> iItemUID[6]
            >> iItemUID[7]
            >> iItemUID[8]	
            >> iItemUID[9]
            >> iItemUID[10]
            >> iItemUID[11]
            >> iItemUID[12]
            >> iItemUID[13]
            >> iItemUID[14]
            
            );

		m_kODBC.EndFetch();

		if( kPacket_.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"대리 상점 구매 SP 호출이 실패 했습니다." )
				<< BUILD_LOG( kPacket_.m_iOK )
				<< END_LOG;

			goto end_proc;
		}

		// 품목이 하나 뿐이므로
		kPacket_.m_kPriceIntemInfoIN.m_iItemUID = iItemUID[0];
		
		// 거래시 봉인 상태는 SP 에서 해제한다.
		if( kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.IsSealedItem() == true )
		{
			kPacket_.m_kPriceIntemInfoIN.m_kItemInfo.UnsealItem();
		}
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_BUY_PSHOP_AGENCY_ITEM_ACK, kPacket_ );
}
#else
IMPL_ON_FUNC( DBE_BUY_PSHOP_AGENCY_ITEM_REQ )
{
	kPacket_.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPShopItem_UPD", L"%d, %d, %d, %d, %d, %d",
		% kPacket_.m_iHostUnitUID
		% kPacket_.m_kSellItemDBUpdate.m_iItemUID
		% kPacket_.m_kSellItemDBUpdate.m_iQuantity
		% kPacket_.m_kSellItemDBUpdate.m_iTotalSellEDIn
		% kPacket_.m_kSellItemDBUpdate.m_iTotalSoldItemQuantity
		% kPacket_.m_kSellItemDBUpdate.m_iTotalSellCommissionED
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_PShopItem", L"%d, %d, %d, %d, %d, %d",
		% kPacket_.m_iHostUnitUID
		% kPacket_.m_kSellItemDBUpdate.m_iItemUID
		% kPacket_.m_kSellItemDBUpdate.m_iQuantity
		% kPacket_.m_kSellItemDBUpdate.m_iTotalSellEDIn
		% kPacket_.m_kSellItemDBUpdate.m_iTotalSoldItemQuantity
		% kPacket_.m_kSellItemDBUpdate.m_iTotalSellCommissionED
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket_.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상인 물품 구매 요청 실패." )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< BUILD_LOG( kPacket_.m_iHostUnitUID )
			<< BUILD_LOG( kPacket_.m_kSellItemDBUpdate.m_iItemUID )
			<< BUILD_LOG( kPacket_.m_kSellItemDBUpdate.m_iQuantity )
			<< BUILD_LOG( kPacket_.m_kSellItemDBUpdate.m_iTotalSellEDIn )
			<< BUILD_LOG( kPacket_.m_kSellItemDBUpdate.m_iTotalSoldItemQuantity )
			<< BUILD_LOG( kPacket_.m_kSellItemDBUpdate.m_iTotalSellCommissionED )
			<< END_LOG;

		kPacket_.m_iOK = NetError::ERR_PERSONAL_SHOP_15;
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_BUY_PSHOP_AGENCY_ITEM_ACK, kPacket_ );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

IMPL_ON_FUNC( DBE_STOP_SALE_PSHOP_AGENCY_REQ )
{
	KDBE_STOP_SALE_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_iGSUID = kPacket_.m_iGSUID;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iReason = kPacket_.m_iReason;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	// 대리상점 거래 중지 요청
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GPShopInfo_UPD_OnOff", L"%d, %d",
		% kPacket_.m_iUnitUID
		% false
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_PShop_info_On_Off", L"%d, %d",
		% kPacket_.m_iUnitUID
		% false
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"대리 상인 판매 중지 요청 실패." )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_PERSONAL_SHOP_35;
	}

end_proc:
	SendToRoom( FIRST_SENDER_UID, DBE_STOP_SALE_PSHOP_AGENCY_ACK, kPacket );
}

//{{ 2012. 05. 31	김민성       대리 상점 거래 로직 변경
#ifdef SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
IMPL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	KDBE_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_iGSUID = kPacket_.m_iGSUID;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	std::vector<KSellPersonalShopItemInfo> vecAddItem;

	// SP 쿼리를 만들어 보자
	//{{ 2013. 05. 28	최육사	아이템 개편
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
#ifdef SERV_ALL_RENEWAL_SP
	std::wstring wstrQuerySP = L"exec dbo.P_GPShopInfo_INS_TradeNew ";
#else //SERV_ALL_RENEWAL_SP
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Pshop_New ";
#endif //SERV_ALL_RENEWAL_SP
#else
	std::wstring wstrQuerySP = L"exec dbo.P_GItem_Total_Pshop ";
#endif SERV_NEW_ITEM_SYSTEM_2013_05
	//}}
	int iTemp_Type = 4;		// 대리 상점 아이템 회수 sp 타입
	int iTemp_Zero = 0;

	wstrQuerySP += boost::str( boost::wformat( L"%d" ) % iTemp_Type );

	// 판매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iUnitUID );

	// 구매자
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );

	// 판매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kPacket_.m_iSellItemTotalED );		// 전체 판매된 ED 총합

	// 구매자 ED
	wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );

	// 등록 아이템
	BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kSellItemInfo,  kPacket_.m_vecPickUpItemList)
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kSellItemInfo.m_kInventoryItemInfo.m_iItemUID );								// ItemUID
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kSellItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity );				// Quantity
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % kSellItemInfo.m_iPricePerOne );												// price
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % static_cast<int>(kSellItemInfo.m_kInventoryItemInfo.m_cSlotCategory) );		// category

		vecAddItem.push_back( kSellItemInfo );
	}

	for(int i = static_cast<int>(kPacket_.m_vecPickUpItemList.size()) ; i < SEnum::PAE_SELL_ITEM_LIMIT_NUM ; ++i )
	{
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// ItemUID	
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// Quantity
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// price
		wstrQuerySP += boost::str( boost::wformat( L", %d" ) % iTemp_Zero );		// category
	}

	// 상점 이름
	std::wstring wstrTemp;
	wstrQuerySP += boost::str( boost::wformat( L", N\'%s\'" ) % wstrTemp );
	
	DO_QUERY_NO_ARG( wstrQuerySP.c_str() );

	// 아이템은 9개 이므로 9개로 고정한다.
	UidType iItemUID[SEnum::PAE_SELL_ITEM_LIMIT_NUM] = {0,};

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
            >> iItemUID[0]
            >> iItemUID[1]
            >> iItemUID[2]
            >> iItemUID[3]
            >> iItemUID[4]
            >> iItemUID[5]
            >> iItemUID[6]
            >> iItemUID[7]
            >> iItemUID[8]	
            >> iItemUID[9]
            >> iItemUID[10]
            >> iItemUID[11]
            >> iItemUID[12]
            >> iItemUID[13]
            >> iItemUID[14]
            );

		m_kODBC.EndFetch();

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"대리상점 회수 SP 호출이 실패 했습니다." )
				<< BUILD_LOG( kPacket.m_iOK )
                << BUILD_LOG( GET_LAST_QUERY() )
				<< END_LOG;

			// 실패 처리
			BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kSellItemInfo,  kPacket_.m_vecPickUpItemList)
			{
				kPacket.m_vecPickUpFailList.push_back( kSellItemInfo.m_kInventoryItemInfo.m_iItemUID );
			}

			goto end_proc;
		}

		int index = 0;
		
		BOOST_TEST_FOREACH( KSellPersonalShopItemInfo&, kSellItemInfo,  kPacket_.m_vecPickUpItemList )
		{
			BOOST_TEST_FOREACH( const KSellPersonalShopItemInfo&, kItemTemp,  vecAddItem )
			{
				if( kItemTemp.m_kInventoryItemInfo.m_iItemUID == kSellItemInfo.m_kInventoryItemInfo.m_iItemUID )
				{
					kSellItemInfo.m_iInventoryItemUID = iItemUID[index++];
				}
			}

			// 회수 - 팔리지 않은 아이템의 경우 봉인 정보를 유지 시켜 준다.
		}

		kPacket.m_iSellItemTotalED = kPacket_.m_iSellItemTotalED;
		kPacket.m_vecPickUpItemList = kPacket_.m_vecPickUpItemList;
	}

end_proc:

	SendToRoom( FIRST_SENDER_UID, DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
}
#else
IMPL_ON_FUNC( DBE_PICK_UP_FROM_PSHOP_AGENCY_REQ )
{
	KDBE_PICK_UP_FROM_PSHOP_AGENCY_ACK kPacket;
	kPacket.m_iGSUID = kPacket_.m_iGSUID;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	BOOST_TEST_FOREACH( const UidType, iItemUID, kPacket_.m_vecPickUpItemList )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GPShopItem_UPD_Delete", L"%d, %d",
			% iItemUID
			% SEnum::PIDR_PICK_UP
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_delete_GPShop_Item", L"%d, %d",
			% iItemUID
			% SEnum::PIDR_PICK_UP
			);
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( kPacket.m_iOK );
			m_kODBC.EndFetch();
		}

end_proc:
		if( kPacket.m_iOK != NetError::NET_OK )
		{
			START_LOG( cerr, L"대리 상인 아이템 pick up 요청 실패." )
				<< BUILD_LOG( kPacket.m_iOK )
				<< BUILD_LOG( iItemUID )
				<< END_LOG;

			kPacket.m_vecPickUpFailList.push_back( iItemUID );
		}
		else
		{
			kPacket.m_vecPickUpItemList.push_back( iItemUID );
		}
	}	

	SendToRoom( FIRST_SENDER_UID, DBE_PICK_UP_FROM_PSHOP_AGENCY_ACK, kPacket );
}
#endif SERV_TRADE_LOGIC_CHANGE_AGENCY_SHOP
//}}

#endif SERV_PSHOP_AGENCY
//}}

//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
IMPL_ON_FUNC( DBE_INSERT_TITLE_REQ )
{
	KDBE_INSERT_TITLE_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iTitleID = kPacket_.m_iTitleID;

	//{{ 2011. 04. 27	최육사	칭호 획득 아이템 개편
#ifdef SERV_TITLE_ITEM_NEW
	if( kPacket_.m_bExpandPeriod )
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_UPD_EndDate", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_update_title", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
	}
	else
#endif SERV_TITLE_ITEM_NEW
		//}}
	{
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GTitle_Complete_MER", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.gup_insert_Title", L"%d, %d, %d",
			% kPacket_.m_iUnitUID
			% kPacket_.m_iTitleID
			% kPacket_.m_sPeriod
			);
#endif //SERV_ALL_RENEWAL_SP
	}

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrEndDate );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"칭호 업데이트가 실패했다? 일어날수 없는 에러." )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iTitleID )
			<< BUILD_LOG( kPacket_.m_sPeriod )
			<< END_LOG;

		kPacket.m_iOK = NetError::ERR_TITLE_04;
	}

end_proc:
	SendToServer( DBE_INSERT_TITLE_ACK, kPacket );
}
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

//{{ 2012. 02. 22	박세훈	길드 이름 변경권
#ifdef SERV_GUILD_CHANGE_NAME
IMPL_ON_FUNC( DBE_CREATE_GUILD_REQ )
{
	KDBE_CREATE_GUILD_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iItemUID = kPacket_.m_iItemUID;

	// 1. 길드 창단 가능한지 체크
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuildMember_SEL_Check", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrGuildName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild_check", L"%d, N\'%s\'", % kPacket_.m_iUnitUID % kPacket_.m_wstrGuildName );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( clog, L"길드 생성할 수 없는것으로 체크됨!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrGuildName );

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_00; break; // 이미 나는 길드에 가입중
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_01; break; // 길드 이름 중복
			//{{ 2009. 10. 26  최육사	길드
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_42; break; // 이 길드 이름은 7일간 사용할 수 없습니다.
			//}}
#ifdef SERV_CREATE_GUILD_ONCE
		case -4: kPacket.m_iOK = NetError::ERR_GUILD_CN01; break; // 길드 생성은 하루에 한번 가능합니다.
#endif SERV_CREATE_GUILD_ONCE
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		// 이름 유일성 인증 끝난 후에 에러 날시 다시 자이언트에 삭제 요청해야 함 
		kPacket.m_bReqGiant_DeleteName = true; 
		kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		goto end_proc;
	}

	// 2. 길드 생성
#ifdef SERV_CREATE_GUILD_EVENT
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_INS", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 30
		% kPacket_.m_wstrGuildMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 30
		% kPacket_.m_wstrGuildMessage
		);
#endif //SERV_ALL_RENEWAL_SP
#else //SERV_CREATE_GUILD_EVENT
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_INS", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 20
		% kPacket_.m_wstrGuildMessage
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_create_guild", L"%d, N\'%s\', %d, N\'%s\'", 
		% kPacket_.m_iUnitUID 
		% kPacket_.m_wstrGuildName
		% 20
		% kPacket_.m_wstrGuildMessage
		);
#endif //SERV_ALL_RENEWAL_SP
#endif //SERV_CREATE_GUILD_EVENT

	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kCreatedGuildInfo.m_iGuildUID
			>> kPacket.m_kCreatedGuildInfo.m_wstrFoundingDay );

		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 생성 실패!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_wstrGuildName )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_04; break; // 길드 창단 에러
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_05; break; // 생성자가 길드원 가입 중 에러			
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

#ifdef SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN
		kPacket.m_bReqGiant_DeleteName = true; 
		kPacket.m_wstrGuildName = kPacket_.m_wstrGuildName;
#endif //SERV_GUARANTEE_UNIQUENESS_OF_NAME_CN

		goto end_proc;
	}
	else
	{
		// 창단 성공하면 초기 길드 정보 세팅
		kPacket.m_kCreatedGuildInfo.m_wstrGuildName		= kPacket_.m_wstrGuildName;
#ifdef SERV_CREATE_GUILD_EVENT
		kPacket.m_kCreatedGuildInfo.m_usMaxNumMember	= 30;
#else //SERV_CREATE_GUILD_EVENT
		kPacket.m_kCreatedGuildInfo.m_usMaxNumMember	= 20;
#endif //SERV_CREATE_GUILD_EVENT
		kPacket.m_kCreatedGuildInfo.m_ucGuildLevel		= 1;
		kPacket.m_kCreatedGuildInfo.m_iGuildEXP			= 0;
		kPacket.m_kCreatedGuildInfo.m_wstrGuildMessage	= kPacket_.m_wstrGuildMessage;
	}

	//{{ 2009. 12. 3  최육사	길드스킬
	// 3. 길드 스킬 포인트
#ifdef GUILD_SKILL_TEST
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SkillPoint_MER", L"%d, %d",
		% kPacket.m_kCreatedGuildInfo.m_iGuildUID
		% 1
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_update_guild_skill_point", L"%d, %d",
		% kPacket.m_kCreatedGuildInfo.m_iGuildUID
		% 1
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 생성 직후에 스킬 포인트 1포인트 insert가 실패하였다. 있을수 없는에러!" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket.m_kCreatedGuildInfo.m_iGuildUID )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06; break; // 존재하지않는 길드
		default: kPacket.m_iOK = NetError::ERR_UNKNOWN; break;
		}

		goto end_proc;
	}
	else
	{
		// 길드 스킬 포인트 초기값은 1포인트다.
		kPacket.m_kGuildSkillInfo.m_iGuildSPoint = 1;
	}

#endif GUILD_SKILL_TEST
	//}}

end_proc:
	KncSend( GetPfID(), 0, PI_LOGIN_USER, LAST_SENDER_UID, anTrace_, DBE_CREATE_GUILD_ACK, kPacket );
}
IMPL_ON_FUNC( DBE_CHANGE_GUILD_NAME_CHECK_REQ )
{
	KDBE_CHANGE_GUILD_NAME_CHECK_ACK kPacket;

	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	// 길드 이름 유효성 검사
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SEL_CheckName", L"N\'%s\'", % kPacket_.m_wstrNewGuildName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Name_CHK", L"N\'%s\'", % kPacket_.m_wstrNewGuildName );
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
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_01; break; // 길드 이름 중복
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_42; break; // 이 길드 이름은 7일간 사용할 수 없습니다.
		default: kPacket.m_iOK = NetError::ERR_GUILD_48; break;
		}

		goto end_proc;
	}

end_proc:
	KncSend( GetPfID(), 0, PI_LOGIN_USER, LAST_SENDER_UID, anTrace_, DBE_CHANGE_GUILD_NAME_CHECK_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_CHANGE_GUILD_NAME_REQ )
{
	KDBE_CHANGE_GUILD_NAME_ACK kPacket;

	kPacket.m_iUnitUID = kPacket_.m_iUnitUID;

	// 길드 이름 변경
	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_UPD_Name", L"%d, N\'%s\'", 
		% kPacket_.m_iGuildUID
		% kPacket_.m_wstrNewGuildName
		);
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_Name_UPT", L"%d, N\'%s\'", 
		% kPacket_.m_iGuildUID
		% kPacket_.m_wstrNewGuildName
		);
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK );
		m_kODBC.EndFetch();
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 이름 변경 실패! ( 아이템 복구 이슈 발생 )" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		switch( kPacket.m_iOK )
		{
		case -1: kPacket.m_iOK = NetError::ERR_GUILD_06;	break;
		case -2: kPacket.m_iOK = NetError::ERR_GUILD_01;	break;
		case -3: kPacket.m_iOK = NetError::ERR_GUILD_42;	break; // 이 길드 이름은 7일간 사용할 수 없습니다.
		default: kPacket.m_iOK = NetError::ERR_GUILD_48;	break;
		}
		goto end_proc;
	}
	
	kPacket.m_kGuildInfo.m_iGuildUID = kPacket_.m_iGuildUID;
	kPacket.m_wstrOldGuildName = kPacket_.m_wstrOldGuildName;

	kPacket.m_iOK = NetError::ERR_ODBC_01;
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GGuild_SEL", L"%d", % kPacket_.m_iGuildUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_guild_info", L"%d", % kPacket_.m_iGuildUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kGuildInfo.m_wstrGuildName
			>> kPacket.m_kGuildInfo.m_usMaxNumMember
			>> kPacket.m_kGuildInfo.m_ucGuildLevel
			>> kPacket.m_kGuildInfo.m_iGuildEXP
			>> kPacket.m_kGuildInfo.m_wstrGuildMessage
			>> kPacket.m_kGuildInfo.m_wstrFoundingDay );

		m_kODBC.EndFetch();

		kPacket.m_wstrOldGuildName = kPacket_.m_wstrOldGuildName;
	}

	if( kPacket.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"길드 정보 얻기 실패" )
			<< BUILD_LOG( kPacket.m_iOK )
			<< BUILD_LOG( kPacket_.m_iUnitUID )
			<< BUILD_LOG( kPacket_.m_iGuildUID )
			<< END_LOG;

		kPacket.m_iOK = NetError::NET_OK;
	}

end_proc:
	KncSend( GetPfID(), 0, PI_LOGIN_USER, LAST_SENDER_UID, anTrace_, DBE_CHANGE_GUILD_NAME_ACK, kPacket );
}
#endif SERV_GUILD_CHANGE_NAME
//}}

//{{ 2012. 06. 05	최육사	배틀필드 시스템
#ifdef SERV_BATTLE_FIELD_SYSTEM
_IMPL_ON_FUNC( DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_REQ, KELG_INVITE_PARTY_FIND_RECEIVER_REQ )
{
	KELG_INVITE_PARTY_FIND_RECEIVER_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_iHostUnitUID = kPacket_.m_iHostUnitUID;

	// 닉네임 찾기!
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUIDByNickname", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.gup_get_unit_uid", L"N\'%s\'", % kPacket_.m_wstrReceiverNickName );
#endif //SERV_ALL_RENEWAL_SP
	UidType iUnitUID = 0;
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iUnitUID );
		m_kODBC.EndFetch();
	}

	if( iUnitUID != 0 )
	{
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_03;
	}
	else
	{
		kPacket.m_iOK = NetError::ERR_SEARCH_UNIT_04;
	}

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_CHECK_NICKNAME_FOR_INVITE_PARTY_ACK, kPacket );
}
#endif SERV_BATTLE_FIELD_SYSTEM
//}}

//{{ 2013. 3. 18	박세훈	 로컬 랭킹 시스템
#ifdef SERV_LOCAL_RANKING_SYSTEM
IMPL_ON_FUNC_NOPARAM( DBE_GAME_LOCAL_RANKING_INIT_INFO_REQ )
{
	KDBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_ACK kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;

	// 시스템 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GFriendSystem_LastReset_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GFriendSystem_LastReset_GET" );
#endif //SERV_ALL_RENEWAL_SP
	// 최초라서 데이터가 없다면, DB가 알아서 당일 정보를 기록한 뒤 되돌려 준다.
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iUID
			>> kPacket.m_wstrResetTime
			);

		m_kODBC.EndFetch();
	}

	// 랭커 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GFriendSystem_Ranker_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GFriendSystem_RankerInfo_GET" );
#endif //SERV_ALL_RENEWAL_SP
	while( m_kODBC.Fetch() )
	{
		UidType iUnitUID;
		UidType	iUserUID;

		FETCH_DATA( iUnitUID
			>> iUserUID
			);
		kPacket.m_mapRankerUIDInfo.insert( std::map<UidType, UidType>::value_type( iUnitUID, iUserUID ) );
	}

	for( std::map<UidType, UidType>::const_iterator it = kPacket.m_mapRankerUIDInfo.begin(); it != kPacket.m_mapRankerUIDInfo.end(); ++it )
	{
		// 랭커 유닛 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_SEL", L"%d", % it->first );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GFriendSystem_UnitInfo_GET", L"%d", % it->first );
#endif //SERV_ALL_RENEWAL_SP
		if( m_kODBC.BeginFetch() )
		{
			KLocalRankingUnitInfo kUnitInfo;
			int iOK = NetError::ERR_ODBC_01;
			int	iUID;

			FETCH_DATA( iOK
				>> iUID
				>> kUnitInfo.m_wstrNickName
				>> kUnitInfo.m_ucLevel
				>> kUnitInfo.m_cUnitClass
				>> kUnitInfo.m_iPoint[SEnum::LRMTC_SPIRIT]
				>> kUnitInfo.m_iPoint[SEnum::LRMTC_AP]
				);
			m_kODBC.EndFetch();

			if( iOK == NetError::NET_OK )
			{
				kUnitInfo.m_iUnitUID = it->first;
				kPacket.m_mapRankerUnitInfo.insert( std::map<UidType, KLocalRankingUnitInfo>::value_type( kUnitInfo.m_iUnitUID, kUnitInfo ) );
			}
		}
	}

end_proc:
	KncSend( GetPfID(), 0, PI_ACCOUNT_DB, 0, NULL, DBE_ACCOUNT_LOCAL_RANKING_INIT_INFO_REQ, kPacket );
}

_IMPL_ON_FUNC( DBE_LOCAL_RANKING_RESET_REQ, int )
{
	KDBE_LOCAL_RANKING_RESET_ACK kPacket;
	kPacket.m_iOK	= NetError::ERR_ODBC_01;
	kPacket.m_iUID	= kPacket_;

	// 초기화 작업 및 UID 갱신
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_Ranker_DEL_Weekly", L"%d", % kPacket_ );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_Reset", L"%d", % kPacket_ );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_wstrUpdated
			);
		m_kODBC.EndFetch();
	}

end_proc:
	SendToServer( DBE_LOCAL_RANKING_RESET_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_LOCAL_RANKING_RANKER_CHANGE_INFO_NOT )
{
	std::map<UidType, bool>::const_iterator it;
	for( it = kPacket_.m_mapChangeInfo.begin(); it != kPacket_.m_mapChangeInfo.end(); ++it )
	{
		int iOK	= NetError::ERR_ODBC_01;

		if( it->second == true )
		{
			DO_QUERY( L"exec dbo.P_GFriendSystem_Ranker_INS", L"%d", % it->first );
		}
		else
		{
			DO_QUERY( L"exec dbo.P_GFriendSystem_Ranker_DEL", L"%d", % it->first );
		}
		
		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK );
			m_kODBC.EndFetch();
		}
	}

end_proc:
	return;
}

IMPL_ON_FUNC_NOPARAM( DBE_LOCAL_RANKING_RESET_CHECK_REQ )
{
	int				iUID = 0;
	std::wstring	wstrResetTime;

	// 시스템 정보 읽기
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GFriendSystem_LastReset_SEL" );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY_NO_ARG( L"exec dbo.P_GFriendSystem_LastReset_GET" );
#endif //SERV_ALL_RENEWAL_SP
	// 최초라서 데이터가 없다면, DB가 알아서 당일 정보를 기록한 뒤 되돌려 준다.
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( iUID
			>> wstrResetTime
			);

		m_kODBC.EndFetch();
	}

end_proc:
	SendToServer( DBE_LOCAL_RANKING_RESET_CHECK_ACK, iUID );
}

_IMPL_ON_FUNC( DBE_GAME_LOCAL_RANKING_WATCH_UNIT_REQ, KELG_LOCAL_RANKING_WATCH_UNIT_REQ )
{
	KDBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_REQ kPacket;
	kPacket.m_iOK = NetError::ERR_ODBC_01;
	kPacket.m_kUnitInfo.m_iUnitUID	= kPacket_.m_iTargetUnitUID;
	kPacket.m_iRequestGSUID			= kPacket_.m_iRequestGSUID;
	kPacket.m_iRequestUnitUID		= kPacket_.m_iRequestUnitUID;

	// 유저 정보
#ifdef SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_LogoffUserInfo_SEL", L"%d", % kPacket_.m_iTargetUnitUID );
#else //SERV_ALL_RENEWAL_SP
	DO_QUERY( L"exec dbo.P_GFriendSystem_LogoffUserInfo_GET", L"%d", % kPacket_.m_iTargetUnitUID );
#endif //SERV_ALL_RENEWAL_SP
	if( m_kODBC.BeginFetch() )
	{
		FETCH_DATA( kPacket.m_iOK
			>> kPacket.m_kUserInfo.m_iUserUID
			>> kPacket.m_kUnitInfo.m_wstrNickName
			>> kPacket.m_kUnitInfo.m_ucLevel
			>> kPacket.m_kUnitInfo.m_cUnitClass
			);
		m_kODBC.EndFetch();

		if( kPacket.m_iOK != NetError::NET_OK )
		{
			kPacket.m_iOK = NetError::ERR_LOCALRANKING_10;
			goto end_proc;
		}
	}

	KncSend( PI_GS_GAME_DB, 0, PI_ACCOUNT_DB, 0, anTrace_, DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_REQ, kPacket );
	return;

end_proc:
	SendToUser( LAST_SENDER_UID, DBE_ACCOUNT_LOCAL_RANKING_WATCH_UNIT_ACK, kPacket );
}
#endif SERV_LOCAL_RANKING_SYSTEM
//}}

//{{ 2013. 04. 01	 인연 시스템 - 김민성
#ifdef SERV_RELATIONSHIP_SYSTEM
IMPL_ON_FUNC_NOPARAM( DBE_LOAD_WEDDING_HALL_INFO_REQ )
{
	KDBE_LOAD_WEDDING_HALL_INFO_ACK kPacket;
	kPacket.m_iOK	= NetError::ERR_ODBC_01;
	
	DO_QUERY_NO_ARG( L"exec dbo.P_GCouple_WeddingInfo_SEL" );

	while( m_kODBC.Fetch() )
	{
		KWeddingHallInfo kWedding;
		// 결혼 완료 안된 것만 가져 오므로
		kWedding.m_bSuccess = false;
		kWedding.m_bDelete = false;
		kWedding.m_iRoomUID = 0;		

		FETCH_DATA( kWedding.m_iWeddingUID
				 >> kWedding.m_iGroom 
				 >> kWedding.m_iBride
				 >> kWedding.m_cWeddingHallType
				 >> kWedding.m_cOfficiantNPC
				 >> kWedding.m_wstrWeddingDate
				 >> kWedding.m_wstrWeddingMsg );

		kPacket.m_mapWeddingInfo.insert( std::make_pair(kWedding.m_iWeddingUID, kWedding ) );

		// 100 개씩 끊어서 전달 하다록 하자
		if( kPacket.m_mapWeddingInfo.size() >= 100 )
		{
			kPacket.m_iOK	= NetError::NET_OK;
			SendToServer( DBE_LOAD_WEDDING_HALL_INFO_ACK, kPacket );

			// 보냈으니 초기화
			kPacket.m_iOK	= NetError::ERR_ODBC_01;
			kPacket.m_mapWeddingInfo.clear();
		}
	}

	kPacket.m_iOK	= NetError::NET_OK;

end_proc:
	SendToServer( DBE_LOAD_WEDDING_HALL_INFO_ACK, kPacket );
}

IMPL_ON_FUNC( DBE_WEDDING_HALL_INFO_UPDATE_NOT )
{
	bool bResult = true;

	std::map< int, KWeddingHallInfo >::iterator mit = kPacket_.m_mapWeddingInfo.begin();
	for( ; mit != kPacket_.m_mapWeddingInfo.end() ; ++mit )
	{
		int iOK	= NetError::ERR_ODBC_01;

		DO_QUERY( L"exec dbo.P_GCouple_WeddingInfo_UPD", L"%d", 
				% mit->second.m_iWeddingUID );

		if( m_kODBC.BeginFetch() )
		{
			FETCH_DATA( iOK	);
			m_kODBC.EndFetch();
		}

		if( iOK != NetError::NET_OK )
		{
			bResult = false;

			START_LOG( cerr, L"결혼 예약 데이터 update 실패!!" )
				<< BUILD_LOG( mit->second.m_iWeddingUID )
				<< BUILD_LOG( mit->second.m_iGroom )
				<< BUILD_LOG( mit->second.m_iBride )
				<< BUILD_LOGc( mit->second.m_cWeddingHallType )
				<< BUILD_LOGc( mit->second.m_cOfficiantNPC )
				<< BUILD_LOG( mit->second.m_wstrWeddingDate )
				<< END_LOG;
		}
	}
	
end_proc:
	START_LOG( cout, L"결혼 예약 데이터 update 결과 : " )
		<< BUILD_LOG( bResult )
		<< END_LOG;
}

_IMPL_ON_FUNC( DBE_RELATIONSHIP_INFO_GAME_DB_REQ, KELG_RELATIONSHIP_INFO_REQ )
{
	KEGS_RELATIONSHIP_INFO_ACK kAck;
	kAck.m_iOK = NetError::ERR_ODBC_01;
	kAck.m_iRequestGSUID = kPacket_.m_iRequestGSUID;
	kAck.m_iRequestUnitUID = kPacket_.m_iRequestUnitUID;
	kAck.m_iAcceptUnitUID = kPacket_.m_iAcceptUnitUID;

	DO_QUERY( L"exec dbo.P_GCouple_Info_SEL_PartnerInfo", L"%d", % kPacket_.m_iAcceptUnitUID );
	if( m_kODBC.BeginFetch() )
	{
		bool bDummy = false;
		int iDummyINT = 0;
		UidType iDummyUID = 0;
		std::wstring wstrLastLogOutTime;

		FETCH_DATA( kAck.m_iOK
			>> kAck.m_ucLevel
			>> kAck.m_cUnitClass
			>> wstrLastLogOutTime
			);

		kAck.m_bIsConnect = false;
		kAck.m_iMapID = 0;
		kAck.m_wstrChannelName = L"미접속";

		CTime tLogOutTime;
		KncUtil::ConvertStringToCTime( wstrLastLogOutTime, tLogOutTime );
		kAck.m_tLastLogOutTime = tLogOutTime.GetTime();

		m_kODBC.EndFetch();
	}

	if( kAck.m_iOK == NetError::NET_OK )
	{
		kAck.m_iOK = NetError::ERR_ODBC_01;

		// 보유 아이템을 얻는다.
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
#ifdef SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_SEL_EquippedItemList", L"%d", % kPacket_.m_iAcceptUnitUID );
#else //SERV_ALL_RENEWAL_SP
		DO_QUERY( L"exec dbo.P_GItem_Equipped_Item_SEL", L"%d", % kPacket_.m_iAcceptUnitUID );
#endif //SERV_ALL_RENEWAL_SP
#else // SERV_BATTLE_FIELD_BOSS
		DO_QUERY( L"exec dbo.gup_get_equipped_item_list", L"%d", % kPacket_.m_iAcceptUnitUID );
#endif // SERV_BATTLE_FIELD_BOSS
		while( m_kODBC.Fetch() )
		{
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
			const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS
			int arrSocketOption[byteArraySize];
			memset( arrSocketOption, 0, sizeof(int) * byteArraySize );

			int iEnchantLevel = 0;
			KInventoryItemInfo kInventoryItemInfo;

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
			FETCH_DATA( kInventoryItemInfo.m_iItemUID
				>> kInventoryItemInfo.m_kItemInfo.m_iItemID
				>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
				>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
				>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
				>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
				>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
				>> iEnchantLevel
				>> arrSocketOption[0]
				>> arrSocketOption[1]
				>> arrSocketOption[2]
				>> arrSocketOption[3]
				>> arrSocketOption[4]
				>> kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
				>> kInventoryItemInfo.m_cSlotCategory
				>> kInventoryItemInfo.m_sSlotID
				);
#else // SERV_BATTLE_FIELD_BOSS
			FETCH_DATA( kInventoryItemInfo.m_iItemUID
				>> kInventoryItemInfo.m_kItemInfo.m_iItemID
				>> kInventoryItemInfo.m_kItemInfo.m_cUsageType
				>> kInventoryItemInfo.m_kItemInfo.m_iQuantity
				>> kInventoryItemInfo.m_kItemInfo.m_sEndurance
				>> kInventoryItemInfo.m_kItemInfo.m_sPeriod
				>> kInventoryItemInfo.m_kItemInfo.m_wstrExpirationDate
				>> iEnchantLevel
				>> arrSocketOption[0]
				>> arrSocketOption[1]
				>> arrSocketOption[2]
				>> arrSocketOption[3]
				>> kInventoryItemInfo.m_cSlotCategory
					>> kInventoryItemInfo.m_sSlotID );
#endif // SERV_BATTLE_FIELD_BOSS


				//{{ 2008. 2. 20  최육사  강화
				kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel = static_cast<char>(iEnchantLevel);
				//}}

				//{{ 2008. 3. 7  최육사  소켓
				int iCheckIdx = byteArraySize;
				while( 0 <= --iCheckIdx )
				{
					if( arrSocketOption[iCheckIdx] != 0 )
						break;
				}

				for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
				{
					kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );				
				}
				//}}

				std::map< int, KInventoryItemInfo >::iterator mit;
				mit = kAck.m_mapOtherUnitEquippedItem.find( kInventoryItemInfo.m_sSlotID );

				if( mit != kAck.m_mapOtherUnitEquippedItem.end() )
				{
					START_LOG( cerr, L"장착 아이템 정보가 중복됨." )
						<< BUILD_LOG( kInventoryItemInfo.m_iItemUID )
						<< BUILD_LOG( static_cast<int>(kInventoryItemInfo.m_sSlotID) )
						<< BUILD_LOG( kInventoryItemInfo.m_kItemInfo.m_iItemID )
						<< END_LOG;
				}
				kAck.m_mapOtherUnitEquippedItem.insert( std::make_pair( kInventoryItemInfo.m_sSlotID, kInventoryItemInfo ) );
		}
	}

	kAck.m_iOK = NetError::NET_OK;

	START_LOG( cout, L"연인의 장착장비 정보 얻기 확인!" )
		<< END_LOG;

end_proc:

	if( kAck.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"연인의 장착장비 정보 얻기 실패!" )
			<< BUILD_LOG( kAck.m_iOK )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_RELATIONSHIP_INFO_GAME_DB_ACK, kAck );
};

_IMPL_ON_FUNC( DBE_WEDDING_ITEM_FIND_INFO_REQ, KELG_WEDDING_ITEM_FIND_INFO_REQ )
{
	kPacket_.m_iOK = NetError::NET_OK;

	std::map< UidType, KWeddingItemInfo >::iterator mit = kPacket_.m_mapWeddingItemInfo.begin();
	for( ; mit != kPacket_.m_mapWeddingItemInfo.end() ; ++mit )
	{
		// 신랑 이름이 없을 경우
		if( mit->second.m_wstrGroom.empty() == true && mit->second.m_iGroom > 0 )
		{
			int iOK = 0;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_ByUnitUID", L"%d", % mit->second.m_iGroom );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUID", L"%d", % mit->second.m_iGroom );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK
						 >> mit->second.m_wstrGroom );
				
				m_kODBC.EndFetch();
			}

			if( iOK != 0 )
			{
				kPacket_.m_iOK = NetError::ERR_RELATIONSHIP_26;
				break;
			}
		}

		// 신부 이름이 없을 경우
		if( mit->second.m_wstrBride.empty() == true && mit->second.m_iBride > 0 )
		{
			int iOK = 0;
#ifdef SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_ByUnitUID", L"%d", % mit->second.m_iBride );
#else //SERV_ALL_RENEWAL_SP
			DO_QUERY( L"exec dbo.P_GUnitNickName_SEL_UnitUID", L"%d", % mit->second.m_iBride );
#endif //SERV_ALL_RENEWAL_SP
			if( m_kODBC.BeginFetch() )
			{
				FETCH_DATA( iOK
					>> mit->second.m_wstrBride );

				m_kODBC.EndFetch();
			}
			if( iOK != 0 )
			{
				kPacket_.m_iOK = NetError::ERR_RELATIONSHIP_26;
				break;
			}
		}
	}

end_proc:

	if( kPacket_.m_iOK != NetError::NET_OK )
	{
		START_LOG( cerr, L"결혼식 예약자 닉네임 얻기 실패!" )
			<< BUILD_LOG( kPacket_.m_iOK )
			<< END_LOG;
	}

	SendToUser( LAST_SENDER_UID, DBE_WEDDING_ITEM_FIND_INFO_ACK, kPacket_ );
}
#endif SERV_RELATIONSHIP_SYSTEM
//}

#ifdef SERV_UPGRADE_TRADE_SYSTEM
void KLoginGameDBThread::GetPShopItem( IN const UidType& iUnitUID_, OUT std::vector< KSellPersonalShopItemInfo >& vecSellItemInfo_ )
{
#ifdef SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX // 김민성 // 적용날짜: 2013-07-11
    DO_QUERY( L"exec dbo.P_GPShopItem_SEL", L"%d", % iUnitUID_ );
#else	// SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX
    DO_QUERY( L"exec dbo.gup_get_PShopItem_Inventory_by_UnitUID", L"%d", % kInfo.m_iUnitUID );
#endif // SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX
    while( m_kODBC.Fetch() )
    {
        KSellPShopItemBackupData kItemInfo;
        kItemInfo.m_kSellPShopItemInfo.m_cPShopItemType = KSellPersonalShopItemInfo::SPIT_PSHOP_AGENCY;

        //{{ 2011. 07. 25    김민성    아이템 옵션ID 데이터 사이즈 증가
#ifdef SERV_ITEM_OPTION_DATA_SIZE
#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		const byte byteArraySize = 5;
#else // SERV_BATTLE_FIELD_BOSS
		const byte byteArraySize = 4;
#endif // SERV_BATTLE_FIELD_BOSS
		int arrSocketOption[byteArraySize];
		memset( arrSocketOption, 0, sizeof(int) * byteArraySize );
#else
        short arrSocketOption[4] = {0};
#endif SERV_ITEM_OPTION_DATA_SIZE
        //}} 

#ifdef SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX // 김민성 // 적용날짜: 2013-07-11
        int arrRandomSocketOption[5] = {0};

#ifdef SERV_BATTLE_FIELD_BOSS// 작업날짜: 2013-11-20	// 박세훈
		FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
			>> arrSocketOption[0]
			>> arrSocketOption[1]
			>> arrSocketOption[2]
			>> arrSocketOption[3]
			>> arrSocketOption[4]
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_byteExpandedSocketNum
			>> arrRandomSocketOption[0]
			>> arrRandomSocketOption[1]
			>> arrRandomSocketOption[2]
			>> arrRandomSocketOption[3]
			>> arrRandomSocketOption[4]
			>> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
			>> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
			>> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
			>> kItemInfo.m_iTotalSoldItemQuantity
			>> kItemInfo.m_iTotalSellCommissionED
			>> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory					
			);
#else // SERV_BATTLE_FIELD_BOSS
        FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
            >> arrSocketOption[0]
        >> arrSocketOption[1]
        >> arrSocketOption[2]
        >> arrSocketOption[3]
        >> arrRandomSocketOption[0]
        >> arrRandomSocketOption[1]
        >> arrRandomSocketOption[2]
        >> arrRandomSocketOption[3]
        >> arrRandomSocketOption[4]
        >> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
            >> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
            >> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
            >> kItemInfo.m_iTotalSoldItemQuantity
            >> kItemInfo.m_iTotalSellCommissionED
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory					
            );
#endif // SERV_BATTLE_FIELD_BOSS

        int iCheckRandomIdx;
        for( iCheckRandomIdx = 4; iCheckRandomIdx >= 0; --iCheckRandomIdx )
        {
            if( arrRandomSocketOption[iCheckRandomIdx] != 0 )
                break;
        }

        for( int iIdx = 0; iIdx <= iCheckRandomIdx; ++iIdx )
        {
            kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecRandomSocket.push_back( arrRandomSocketOption[iIdx] );
        }

#else	// SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX

        FETCH_DATA( kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_iItemUID
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iItemID
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cUsageType
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_iQuantity
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_sEndurance
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_ucSealData
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_cEnchantLevel
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant0
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant1
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_kAttribEnchantInfo.m_cAttribEnchant2
            >> arrSocketOption[0]
        >> arrSocketOption[1]
        >> arrSocketOption[2]
        >> arrSocketOption[3]
        >> kItemInfo.m_kSellPShopItemInfo.m_iPricePerOne
            >> kItemInfo.m_kSellPShopItemInfo.m_iFeePerOne
            >> kItemInfo.m_kSellPShopItemInfo.m_iTotalSellEDIn
            >> kItemInfo.m_iTotalSoldItemQuantity
            >> kItemInfo.m_iTotalSellCommissionED
            >> kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_cSlotCategory
            );
#endif	// SERV_AGENCY_SHOP_ITEM_EVALUATE_FIX

		int iCheckIdx = byteArraySize;
		while( 0 <= --iCheckIdx )
		{
			if( arrSocketOption[iCheckIdx] != 0 )
				break;
		}

        for( int iIdx = 0; iIdx <= iCheckIdx; ++iIdx )
        {
            kItemInfo.m_kSellPShopItemInfo.m_kInventoryItemInfo.m_kItemInfo.m_vecItemSocket.push_back( arrSocketOption[iIdx] );
        }

        vecSellItemInfo_.push_back( kItemInfo.m_kSellPShopItemInfo );
    }

end_proc:
    ;

}
#endif