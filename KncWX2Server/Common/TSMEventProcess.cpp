#include "TSMEventProcess.h"
#include "ThreadStatisticsManager.h"
#include "BaseServer.h"
#include <iomanip>
#include "X2Data/XSLDungeon.h"

#define KLocAlign std::setw(8) << std::setiosflags( std::ios::left )


#ifdef SERV_STATISTICS_THREAD


ImplPfID( KTSMEventProcess, PI_NULL );

#define CLASS_TYPE  KTSMEventProcess

KTSMEventProcess::KTSMEventProcess()
{
	//{{ 2011. 11. 21  김민성	통계 로그 수치
#ifdef SERV_STATISTICS_LOG_COUNT
	m_iStatisticsLogCount = 0;
#endif SERV_STATISTICS_LOG_COUNT
	//}}
}

KTSMEventProcess::~KTSMEventProcess()
{
}

void KTSMEventProcess::ProcessEvent( const KEventPtr& spEvent_ )
{
	DWORD dwElapTime = ::GetTickCount();

	switch( spEvent_->m_usEventID )
	{
	CASE( E_STATISTICS_INFO_NOT );
	CASE_NOPARAM( EGS_SEND_STATISTICS_LOCLOG_REQ );
	CASE_NOPARAM( EGS_SEND_STATISTICS_DBLOG_REQ );
	CASE_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ );
	CASE_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ );
	CASE( E_HENIR_RANKING_LOG_NOT );
	CASE( E_LOCAL_LOG_PVP_NOT );
	CASE( E_LOCAL_LOG_PVP_ROOM_NOT );
	CASE( E_LOCAL_LOG_DUNGEON_ROOM_NOT );
	CASE( E_LOCAL_LOG_TITLE_NOT );
	CASE( E_LOCAL_LOG_DUNGEON_NOT );
	CASE( E_LOCAL_LOG_HENIR_RANKING_NOT );
	//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	CASE( E_LOCAL_LOG_PET_SUMMON_NOT );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
	CASE( E_LOCAL_LOG_SERVER_DISCONNECT_NOT );
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	CASE( E_LOCAL_LOG_BILLING_PACKET_NOT );
#endif SERV_BILLING_PACKET_LOG
	//}}
	//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	CASE( E_LOCAL_LOG_ABUSER_MORNITORING_NOT );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	CASE( E_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT );
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

	default:
		START_LOG( cerr, L"핸들러가 지정되지 않은 이벤트." )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< END_LOG;
	}

	dwElapTime = ::GetTickCount() - dwElapTime;
	if( dwElapTime > 3000 )
	{
		START_LOG( cwarn, L"이벤트 처리 소요 시간." )
			<< BUILD_LOG( spEvent_->GetIDStr() )
			<< BUILD_LOG( spEvent_->m_usEventID )
			<< BUILD_LOG( dwElapTime );
	}

	//////////////////////////////////////////////////////////////////////////
	//{{ 2011. 11. 21  김민성	통계 로그 수치
#ifdef SERV_STATISTICS_LOG_COUNT
	m_iStatisticsLogCount += 1;

	if( m_tStatisticsLogCount.elapsed() > 60.0 )
	{
		// 남기자
		KSIManager.UpdateStatisticsLogCount( m_iStatisticsLogCount );

		// 남기면 초기화
		m_iStatisticsLogCount = 0;
		m_tStatisticsLogCount.restart();
	}
#endif SERV_STATISTICS_LOG_COUNT
	//}}
	//////////////////////////////////////////////////////////////////////////
	
}

IMPL_ON_FUNC( E_STATISTICS_INFO_NOT )
{
	std::vector< KStatisticsInfo >::const_iterator vit;
	for( vit = kPacket_.m_vecStatisticsInfo.begin(); vit != kPacket_.m_vecStatisticsInfo.end(); ++vit )
	{
		KSIManager.UpdateStatistics( vit->m_iStatisticsID, vit->m_kKey, vit->m_iIndex, vit->m_iCount );
	}
}

IMPL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_LOCLOG_REQ )
{
	KSIManager.OnFlushLocalLog( true );

	START_LOG( cout, L"게임서버 텍스트 통계 백업!" );
}

IMPL_ON_FUNC_NOPARAM( EGS_SEND_STATISTICS_DBLOG_REQ )
{
    KSIManager.FlushData();

	START_LOG( cout, L"게임서버 DB 통계 백업!" );
}

IMPL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_LOCLOG_REQ )
{
	KSIManager.OnFlushLocalLog( true );

	START_LOG( cout, L"센터서버 텍스트 통계 백업!" );
}

IMPL_ON_FUNC_NOPARAM( ECN_SEND_STATISTICS_DBLOG_REQ )
{
    KSIManager.FlushData();

	START_LOG( cout, L"센터서버 DB 통계 백업!" );
}

IMPL_ON_FUNC( E_HENIR_RANKING_LOG_NOT )
{
    KSIManager.SendHenirRankingLog( kPacket_.m_iRankingType );
}

IMPL_ON_FUNC( E_LOCAL_LOG_PVP_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_PVP );

	fout << L"\n";
	fout << KLocAlign << kPacket_.m_iPVPChannelClass << L"\t";
	fout << KLocAlign << kPacket_.m_iOwnerUserUID << L"\t";
	fout << KLocAlign << kPacket_.m_wstrIP << L"\t";
	fout << KLocAlign << kPacket_.m_nUnitUID << L"\t";
	fout << KLocAlign << kPacket_.m_wstrNickName << L"\t";
	fout << KLocAlign << kPacket_.m_iUnitClass << L"\t";
	fout << KLocAlign << kPacket_.m_iRoomUID << L"\t";
	fout << KLocAlign << kPacket_.m_iPlayMode << L"\t";	
	fout << KLocAlign << kPacket_.m_bIsItemMode << L"\t";
	fout << KLocAlign << kPacket_.m_bPublic << L"\t";
	fout << KLocAlign << kPacket_.m_iNumMember << L"\t";
	fout << KLocAlign << kPacket_.m_fPlayTimeLimit << L"\t";
	fout << KLocAlign << kPacket_.m_wstrEndGameTime << L"\t";
	fout << KLocAlign << kPacket_.m_iPlayTime << L"\t";
	fout << KLocAlign << kPacket_.m_iVSPoint << L"\t";
	fout << KLocAlign << kPacket_.m_iEXP << L"\t";
	fout << KLocAlign << kPacket_.m_iNumMDKill << L"\t";
	fout << KLocAlign << kPacket_.m_iNumDie << L"\t";
	fout << KLocAlign << kPacket_.m_iResult << L"\t";

	fout.flush();
}

IMPL_ON_FUNC( E_LOCAL_LOG_PVP_ROOM_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_PVP_ROOM );

	fout << L"\n";
	fout << KLocAlign << kPacket_.m_bIsItemMode << L"\t";	
	fout << KLocAlign << kPacket_.m_bIsItemMode << L"\t";
	fout << KLocAlign << kPacket_.m_sWorldID << L"\t";
	fout << KLocAlign << kPacket_.m_iNumMember << L"\t";
	fout << KLocAlign << kPacket_.m_iPlayTime << L"\t";
	fout << KLocAlign << kPacket_.m_wstrEndGameTime << L"\t";

	fout.flush();
}

IMPL_ON_FUNC( E_LOCAL_LOG_DUNGEON_ROOM_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_DUNGEON_ROOM );

	fout << L"\n";
	fout << KLocAlign << kPacket_.m_iDungeonID << L"\t";
	fout << KLocAlign << kPacket_.m_iDifficultyLevel << L"\t";
	fout << KLocAlign << kPacket_.m_iIsChallenge << L"\t";
	fout << KLocAlign << kPacket_.m_iWin << L"\t";
	fout << KLocAlign << kPacket_.m_iLose << L"\t";
	fout << KLocAlign << kPacket_.m_iStartNumMember << L"\t";
	fout << KLocAlign << kPacket_.m_iEndNumMember << L"\t";
	fout << KLocAlign << kPacket_.m_iTotalPlayTime << L"\t";
	fout << KLocAlign << kPacket_.m_wstrEndGameTime << L"\t";

	fout.flush();
}

IMPL_ON_FUNC( E_LOCAL_LOG_TITLE_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_TITLE );

	fout << L"\n";
	fout << KLocAlign << kPacket_.m_iTitleID << L"\t";
	fout << KLocAlign << kPacket_.m_wstrCharName << L"\t";
	fout << KLocAlign << kPacket_.m_ucLevel << L"\t";

	fout.flush();
}

IMPL_ON_FUNC( E_LOCAL_LOG_DUNGEON_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_DUNGEON );

	fout << L"\n";
	fout << KLocAlign << kPacket_.m_iDungeonID << L"\t";	
	fout << KLocAlign << kPacket_.m_nUnitUID << L"\t";
	fout << KLocAlign << kPacket_.m_wstrNickName << L"\t";
	fout << KLocAlign << kPacket_.m_iDifficultyLevel << L"\t";
	fout << KLocAlign << kPacket_.m_iIsChallenge << L"\t";
	fout << KLocAlign << kPacket_.m_iUnitClass << L"\t";
	fout << KLocAlign << kPacket_.m_iClear << L"\t";
	fout << KLocAlign << kPacket_.m_iStartNumMember << L"\t";
	fout << KLocAlign << kPacket_.m_iEndNumMember << L"\t";
	fout << KLocAlign << kPacket_.m_ucLevel << L"\t";
	fout << KLocAlign << kPacket_.m_iPlayTime << L"\t";
	fout << KLocAlign << kPacket_.m_iEXP << L"\t";
	fout << KLocAlign << kPacket_.m_iED << L"\t";
	fout << KLocAlign << CXSLDungeon::GetRankString( (CXSLDungeon::RANK_TYPE)kPacket_.m_cComboRank ) << L"\t";
	fout << KLocAlign << kPacket_.m_iComboScore << L"\t";
	fout << KLocAlign << CXSLDungeon::GetRankString( (CXSLDungeon::RANK_TYPE)kPacket_.m_cTechnicalRank ) << L"\t";
	fout << KLocAlign << kPacket_.m_iTechnicalScore << L"\t";
	fout << KLocAlign << CXSLDungeon::GetRankString( (CXSLDungeon::RANK_TYPE)kPacket_.m_cTimeRank ) << L"\t";
	fout << KLocAlign << CXSLDungeon::GetRankString( (CXSLDungeon::RANK_TYPE)kPacket_.m_cDamagedRank ) << L"\t";
	fout << KLocAlign << kPacket_.m_iDamageNum << L"\t";
	fout << KLocAlign << CXSLDungeon::GetRankString( (CXSLDungeon::RANK_TYPE)kPacket_.m_cTotalRank ) << L"\t";
	fout << KLocAlign << kPacket_.m_iRessurectionStoneCount << L"\t";
	fout << KLocAlign << kPacket_.m_iPassedStageCount << L"\t";
	fout << KLocAlign << kPacket_.m_iPassedSubStageCount << L"\t";
	//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	fout << KLocAlign << kPacket_.m_bIsWithPet << L"\t";
#endif SERV_PET_SYSTEM
	//}}

	fout.flush();
}

IMPL_ON_FUNC( E_LOCAL_LOG_HENIR_RANKING_NOT )
{
	for( u_int ui = 0; ui < kPacket_.m_vecRankingInfo.size(); ++ui )
	{
		const KHenirRankingInfo& kRankingInfo = kPacket_.m_vecRankingInfo[ui];

		// 기록 시각 얻기
		CTime kEndGameTime = CTime( kRankingInfo.m_tRegDate );

		std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm_Henir_Ranking( kPacket_.m_iRankingType );

		fout << L"\n";
		fout << KLocAlign << kPacket_.m_iRankingType << L"\t";
		fout << KLocAlign << ui << L"\t";
		fout << KLocAlign << kRankingInfo.m_iRank << L"\t";
		fout << KLocAlign << kRankingInfo.m_iStageCount << L"\t";
		fout << KLocAlign << kRankingInfo.m_ulPlayTime << L"\t"; // 초단위 시간
		fout << KLocAlign << (kRankingInfo.m_ulPlayTime / 60) << L":" << (kRankingInfo.m_ulPlayTime % 60) << L"\t"; // 분단위 시간
		fout << KLocAlign << kRankingInfo.m_wstrNickName << L"\t";
		fout << KLocAlign << static_cast<int>(kRankingInfo.m_cUnitClass) << L"\t";
		fout << KLocAlign << static_cast<int>(kRankingInfo.m_ucLevel) << L"\t";
		fout << KLocAlign << (const wchar_t*)kEndGameTime.Format(_T("%Y-%m-%d %H:%M:%S")) << L"\t";

		fout.flush();
	}
}

//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM

IMPL_ON_FUNC( E_LOCAL_LOG_PET_SUMMON_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_PET_SUMMON );

	fout << L"\n";
	fout << KLocAlign << kPacket_.m_wstrNickName << L"\t";
	fout << KLocAlign << (int)kPacket_.m_ucLevel << L"\t";
	fout << KLocAlign << kPacket_.m_iPetUID << L"\t";
#ifdef SERV_PETID_DATA_TYPE_CHANGE // 2013.07.02
	fout << KLocAlign << kPacket_.m_iPetID << L"\t";
#else
	fout << KLocAlign << (int)kPacket_.m_cPetID << L"\t";
#endif //SERV_PETID_DATA_TYPE_CHANGE
	fout << KLocAlign << (int)kPacket_.m_cEvolutionStep << L"\t";
	fout << KLocAlign << kPacket_.m_iIntimacy << L"\t";
	fout << KLocAlign << kPacket_.m_sExtroversion << L"\t";
	fout << KLocAlign << kPacket_.m_sEmotion << L"\t";
	fout << KLocAlign << kPacket_.m_wstrRegDate << L"\t";

	fout.flush();
}

#endif SERV_PET_SYSTEM
//}}

//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG

IMPL_ON_FUNC( E_LOCAL_LOG_SERVER_DISCONNECT_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_SERV_DISCONNECT );

	fout << L"\n";
	switch( kPacket_.m_cLogType )
	{
	case KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_CONNECT:
		{
			fout << KLocAlign << L"Connect" << L"\t";
		}
		break;

	case KE_LOCAL_LOG_SERVER_DISCONNECT_NOT::SDLT_DISCONNECT:
		{
			fout << KLocAlign << L"Disconnect" << L"\t";
		}
		break;

	default:
		START_LOG( cerr, L"잘못된 로그 타입입니다!" )
			<< BUILD_LOGc( kPacket_.m_cLogType )
			<< END_LOG;
		return;
	}
	
	fout << KLocAlign << kPacket_.m_wstrSourceName << L"\t";
	fout << KLocAlign << kPacket_.m_wstrDestinationName << L"\t";
	fout << KLocAlign << kPacket_.m_wstrDestinationIP << L"\t";	
	fout << KLocAlign << kPacket_.m_wstrRegDate << L"\t";
	fout << KLocAlign << kPacket_.m_wstrReason << L"\t";

	fout.flush();
}

#endif SERV_SERVER_DISCONNECT_LOG
//}}

#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
IMPL_ON_FUNC( E_LOCAL_LOG_UNDEFINED_QUEST_TEMPLET_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_UNDEFINED_QUEST_TEMPLET );
	fout << L"\n";

	fout << KLocAlign << kPacket_.m_iQuestID << L"\t";
	fout << KLocAlign << kPacket_.m_wstrRegDate << L"\t";

	fout.flush();
}
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET


//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG

IMPL_ON_FUNC( E_LOCAL_LOG_BILLING_PACKET_NOT )
{
	if( KSIManager.IsBillingPacketLog() == false )
		return;

	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_BILLING_PACKET );

	fout << L"\n";
	switch( kPacket_.m_cLogType )
	{
	case KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_SEND:
		{
			fout << KLocAlign << L"Send" << L"\t";
		}
		break;

	case KE_LOCAL_LOG_BILLING_PACKET_NOT::BPLT_RECV:
		{
			fout << KLocAlign << L"Recv" << L"\t";
		}
		break;

	default:
		START_LOG( cerr, L"잘못된 로그 타입입니다!" )
			<< BUILD_LOGc( kPacket_.m_cLogType )
			<< END_LOG;
		return;
	}

	fout << KLocAlign << kPacket_.m_iUserUID << L"\t";
	fout << KLocAlign << kPacket_.m_ulOrderNo << L"\t";
	fout << KLocAlign << kPacket_.m_wstrRegDate << L"\t";
	fout << KLocAlign << KEvent::GetIDStr( kPacket_.m_usEventID ) << L"\t";

	fout.flush();
}

#endif SERV_BILLING_PACKET_LOG
//}}

//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM

IMPL_ON_FUNC( E_LOCAL_LOG_ABUSER_MORNITORING_NOT )
{
	std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm( KStatisticsLocalLog::LOG_ABUSER_MORNITORING );

	CTime tRegDate;
	std::wstring wstrRegDate;

	BOOST_TEST_FOREACH( const KAbuserEventLog&, kInfo, kPacket_.m_vecAbuserEventLog )
	{
		tRegDate = kInfo.m_iRegDate;
		wstrRegDate = ( CStringW )( tRegDate.Format( _T( "%d_%H:%M:%S" ) ) );

		fout << L"\n";
		fout << KLocAlign << kInfo.m_iUserUID << L"\t";
		fout << KLocAlign << kInfo.m_iUnitUID << L"\t";
		fout << KLocAlign << kInfo.m_iED << L"\t";
		fout << KLocAlign << kInfo.m_iGetItemCount << L"\t";
		//{{ 2013. 05. 29	최육사	패킷 모니터링 근성도 항목 추가
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
		fout << KLocAlign << kInfo.m_iSpirit << L"\t";
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
		//}}
		fout << KLocAlign << wstrRegDate << L"\t";
		fout << KLocAlign << KEvent::GetIDStr( kInfo.m_usEventID ) << L"\t";

		fout.flush();
	}
}

#endif SERV_AUTO_HACK_CHECK_GET_ITEM
//}}


#endif SERV_STATISTICS_THREAD


