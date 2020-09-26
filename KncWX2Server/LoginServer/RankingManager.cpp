#include ".\rankingmanager.h"
#include "Enum\Enum.h"
#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "BaseServer.h"
//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	#include "LoginServer.h"
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
//}}

#include <iomanip>


ImplementSingleton( KRankingManager );

KRankingManager::KRankingManager(void) :
m_bInit( false ),
m_bReservedRankingReward( false )
{
	// lua
	lua_tinker::class_add<KRankingManager>( g_pLua, "KRankingManager" );
	lua_tinker::class_def<KRankingManager>( g_pLua, "dump",							&KRankingManager::Dump );
	lua_tinker::class_def<KRankingManager>( g_pLua, "clear",						&KRankingManager::ClearRanking_LUA );
	lua_tinker::class_def<KRankingManager>( g_pLua, "AddHenirRankingInfo",			&KRankingManager::AddHenirRankingInfo_LUA );
	lua_tinker::class_def<KRankingManager>( g_pLua, "AddRankingRewardInfo",			&KRankingManager::AddRankingRewardInfo_LUA );
	//{{ 2009. 8. 3  최육사		랭킹 보상 시작
	lua_tinker::class_def<KRankingManager>( g_pLua, "SetRankingRewardBeginDate",	&KRankingManager::SetRankingRewardBeginDate_LUA );
	//}}
	lua_tinker::class_def<KRankingManager>( g_pLua, "NewRecordForTest",				&KRankingManager::NewRecordForTest_LUA ); // 테스트용
	//{{ 2009. 7. 31  최육사	랭킹 삭제
	lua_tinker::class_def<KRankingManager>( g_pLua, "NewRecord",					&KRankingManager::NewRecord_LUA ); // 개발자전용
	lua_tinker::class_def<KRankingManager>( g_pLua, "DeleteRecord",					&KRankingManager::DeleteRecord_LUA ); // 개발자전용
	//}}

	lua_tinker::decl( g_pLua, "RankingManager", this );
}

KRankingManager::~KRankingManager(void)
{
}

ImplToStringW( KRankingManager )
{
	// 덤프이벤트 보내기
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, DMP_DUMP_RANKING_MANAGER_NOT );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
	return stm_;
}

bool KRankingManager::OpenScriptFile( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}

bool KRankingManager::AddHenirRankingInfo_LUA( int iRankingType, int iLastRank, int iHour, int iDayOfWeek /*= 0*/, int iWeekOfMonth /*= 0*/ )
{
	if( iRankingType < 0  ||  iHour < 0  ||  iDayOfWeek < 0  ||  iWeekOfMonth < 0 )
	{
		START_LOG( cerr, L"헤니르의 시공 랭킹 스크립트 내용이 이상합니다!" )
			<< BUILD_LOG( iRankingType )
			<< BUILD_LOG( iLastRank )
			<< BUILD_LOG( iHour )
			<< BUILD_LOG( iDayOfWeek )
			<< BUILD_LOG( iWeekOfMonth )
			<< END_LOG;

		return false;
	}
#ifndef SERV_HENIR_NO_REWARD
	// 랭킹 객체 생성
    KHenirRanking kInfo;
	kInfo.SetLastRank( static_cast<u_int>(iLastRank) );
	kInfo.SetRefreshTime( iRankingType, iWeekOfMonth, iDayOfWeek, iHour );
	m_mapHenirRanking.insert( std::make_pair( iRankingType, kInfo ) );
#endif //SERV_HENIR_NO_REWARD
	return true;
}

bool KRankingManager::AddRankingRewardInfo_LUA( int iRankingType, int iRank, int iTitleID, short sPeriod )
{
	if( iRankingType < 0  ||  iRank <= 0  ||  iTitleID <= 0  ||  sPeriod < 0 )
	{
		START_LOG( cerr, L"랭킹 타이틀 보상 스크립트 내용이 이상합니다!" )
			<< BUILD_LOG( iRankingType )
			<< BUILD_LOG( iRank )
			<< BUILD_LOG( iTitleID )
			<< BUILD_LOG( sPeriod )
			<< END_LOG;
		return false;
	}

	KTitleRewardKey kKey( iRankingType, iRank );
	KTitleReward kReward;
	kReward.m_iTitleID = iTitleID;
	kReward.m_sPeriod = sPeriod;

	std::map< KTitleRewardKey, std::vector< KTitleReward > >::iterator mit;
	mit = m_mapRankingTitleReward.find( kKey );
	if( mit == m_mapRankingTitleReward.end() )
	{
		std::vector< KTitleReward > vecReward;
		vecReward.push_back( kReward );
		m_mapRankingTitleReward.insert( std::make_pair( kKey, vecReward ) );
	}
	else
	{
		mit->second.push_back( kReward );
	}

	return true;
}

//{{ 2009. 8. 3  최육사		랭킹보상시작
bool KRankingManager::SetRankingRewardBeginDate_LUA( int iRankingType, const char* pBeginDate )
{
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	if( SEnum::IsHenirRankingType( iRankingType ) == false )
#else // SERV_HENIR_RENEWAL_2013
	if( iRankingType < 0  ||  iRankingType > SEnum::RT_MONTH_RANKING )
#endif // SERV_HENIR_RENEWAL_2013
	{
		START_LOG( cerr, L"Ranking정보가 이상합니다!" )
			<< BUILD_LOG( iRankingType )			
			<< END_LOG;

		return false;
	}

	std::wstring wstrBeginDate = KncUtil::toWideString( std::string( pBeginDate ) );

	std::map< int, KHenirRanking >::iterator mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 랭킹정보의 보상시작시각을 등록하려하였음!" )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	// 랭킹 시작시각 등록
	mit->second.SetRewardBeginTime( wstrBeginDate );
	return true;
}
//}}

void KRankingManager::NewRecordForTest_LUA( int iCount )
{
	_JIF( iCount <= 150, return; );

	KELG_ADMIN_NEW_RECORD_TEST_NOT kNot;

	for( int i = 0; i < iCount; ++i )
	{
		CStringW strRefreshTime;
		strRefreshTime.Format( L"%d번녀석", i );

		KHenirRankingInfo kInfo;
		kInfo.m_iUnitUID = 34729 + i;
		kInfo.m_wstrNickName = strRefreshTime.GetBuffer();
		kInfo.m_ucLevel = rand() % 50;
		kInfo.m_cUnitClass = ( rand() % 5 ) + 1;
		kInfo.m_iStageCount = ( rand() % 30 ) + 1;
		kInfo.m_iRank = 0;
		kInfo.m_ulPlayTime = ( rand() % 200 ) + 10;

		kNot.m_vecNewRecordTest.push_back( kInfo );
	}

	// 이벤트 보내기
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, ELG_ADMIN_NEW_RECORD_TEST_NOT, kNot );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

//{{ 2009. 7. 31  최육사	랭킹 삭제
void KRankingManager::NewRecord_LUA( int iRankingType, int iStageCount, UINT ulPlayTime, __int64 tRegDate )
{
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	if( ( SEnum::IsHenirRankingType( iRankingType ) == false ) ||  ( iStageCount < 0 ) )
#else // SERV_HENIR_RENEWAL_2013
	if( iRankingType < 0  ||  iRankingType > SEnum::RT_MONTH_RANKING  ||  iStageCount < 0 )
#endif // SERV_HENIR_RENEWAL_2013
	{
		START_LOG( cerr, L"Ranking정보가 이상합니다!" )
			<< BUILD_LOG( iRankingType )
			<< BUILD_LOG( iStageCount )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_NEW_RECORD_NOT kNot;
	kNot.m_iRankingType = iRankingType;
	kNot.m_kNewRecordRanking.m_iStageCount = iStageCount;
	kNot.m_kNewRecordRanking.m_ulPlayTime = ulPlayTime;
	kNot.m_kNewRecordRanking.m_tRegDate = tRegDate;

	int iUnitUID = 0;
	int iUnitClass = 0;
	int iLevel = 0;

	KLuaManager luaMgr( g_pLua );
	LUA_GET_VALUE( luaMgr, L"UnitUID",		iUnitUID,		0 );
	LUA_GET_VALUE( luaMgr, L"NickName",		kNot.m_kNewRecordRanking.m_wstrNickName,	L"" );
	LUA_GET_VALUE( luaMgr, L"UnitClass",	iUnitClass,		0 );
	LUA_GET_VALUE( luaMgr, L"Level",		iLevel,			0 );

	if( iUnitUID < 0  ||  iUnitClass < 0  ||  iLevel <= 0  ||  kNot.m_kNewRecordRanking.m_wstrNickName.empty() )
	{
		START_LOG( cerr, L"Ranking Unit정보가 이상합니다!" )
			<< BUILD_LOG( iUnitUID )
			<< BUILD_LOG( kNot.m_kNewRecordRanking.m_wstrNickName )
			<< BUILD_LOG( iUnitClass )
			<< BUILD_LOG( iLevel )
			<< END_LOG;
		return;
	}

	kNot.m_kNewRecordRanking.m_iUnitUID = static_cast<UidType>(iUnitUID);
	kNot.m_kNewRecordRanking.m_cUnitClass = static_cast<char>(iUnitClass);
	kNot.m_kNewRecordRanking.m_ucLevel = static_cast<UCHAR>(iLevel);

	// 이벤트 보내기
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, ELG_ADMIN_NEW_RECORD_NOT, kNot );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

void KRankingManager::DeleteRecord_LUA( int iRankingType, const char* pNickName )
{
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-24	// 박세훈
	if( SEnum::IsHenirRankingType( iRankingType ) == false )
#else // SERV_HENIR_RENEWAL_2013
	if( iRankingType < 0  ||  iRankingType > SEnum::RT_MONTH_RANKING )
#endif // SERV_HENIR_RENEWAL_2013
	{
		START_LOG( cerr, L"RankingType이 이상합니다!" )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;
		return;
	}

	if( pNickName == NULL )
	{
		START_LOG( cerr, L"Ranking NickName이 이상합니다!" )
			<< END_LOG;
		return;
	}

	KELG_ADMIN_DELETE_RECORD_NOT kNot;

	kNot.m_iRankingType = iRankingType;
	kNot.m_wstrNickName = KncUtil::toWideString( std::string( pNickName ) );

	// 이벤트 보내기
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_GS_FIELD, NULL, ELG_ADMIN_DELETE_RECORD_NOT, kNot );
	spEvent->m_kDestPerformer.AddUID( 0 );

	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}
//}}

void KRankingManager::Tick()
{
	// 헤니르 시공 랭킹
	TickHenirRanking();
	
	// 대전&던전 랭킹
	TickDungeonAndPvpRanking();
}

void KRankingManager::Init()
{
	// 대전/던전 랭킹 갱신 시각 초기화
	if( m_tResetRankingTime.GetTime() == 0 )
	{
		// 디폴트
		CTime tCurTime	 = CTime::GetCurrentTime();
#ifdef SERV_COUNTRY_US
		CTime tResetTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 3, 30, 0 );
#else
		CTime tResetTime = CTime( tCurTime.GetYear(), tCurTime.GetMonth(), tCurTime.GetDay(), 6, 30, 0 );
#endif //SERV_COUNTRY_US
		tResetTime += CTimeSpan( 1, 0, 0, 0 );

		m_tResetRankingTime = tResetTime;

		CStringW strRefreshTime = (CStringW)m_tResetRankingTime.Format( _T("%Y-%m-%d %H:%M:%S") );

		START_LOG( cout, L"대전/던전레벨 랭킹 갱신 시각 설정!" << strRefreshTime.GetBuffer() );
	}

	//////////////////////////////////////////////////////////////////////////	
	// 던전, 대전 랭킹 얻기
	SendToGameDB( DBE_GET_WEB_RANKING_INFO_REQ );

	//////////////////////////////////////////////////////////////////////////	
	// DB로부터 저장된 헤니르의 시공 랭킹 받아오기
	SendToLogDB( DBE_GET_HENIR_RANKING_INFO_REQ );
}

void KRankingManager::ShutDown()
{
	KDBE_BACKUP_RANKING_INFO_NOT kPacketToDB;
	GetHenirRankingInfo( kPacketToDB.m_mapHenirRanking, false );

	// DB Update
	SendToLogDB( DBE_BACKUP_RANKING_INFO_NOT, kPacketToDB );

	// Refresh Time File Backup
	RefreshTimeFileBackup();
}

void KRankingManager::DumpRankingInfo()
{
	START_LOG( cout, L"-------------------- Dump RankingManager --------------------" )
		<< BUILD_LOG( m_bInit )
		<< BUILD_LOG( m_bReservedRankingReward );

	std::map< int, KHenirRanking >::const_iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		CTime tRefreshTime = mit->second.GetRefreshTime();

		CStringW strRefreshTime = (CStringW)tRefreshTime.Format( _T("%Y-%m-%d %H:%M:%S") );

		START_LOG( cout, L"Ranking Type : " << mit->first )
			<< BUILD_LOG( mit->second.IsRankingChanged() )
			<< BUILD_LOG( mit->second.GetRankingInfo().size() )
			<< BUILD_LOG( strRefreshTime.GetBuffer() );
	}
}

void KRankingManager::TickHenirRanking()
{
	// 1. 5초마다 체크
	if( m_tBroadCastTimer.elapsed() < 5.0 )
		return;

	// 2. 타이머 초기화	
	m_tBroadCastTimer.restart();

	// 3. Init()함수 호출 후 DB로부터 저장된 랭킹 받아오기가 실패했다면..	
	if( !m_bInit )
	{
		START_LOG( cerr, L"5초안에 헤니르 시공 랭킹정보를 DB로부터 받지 못하였습니다. 다시 랭킹 정보 요청합니다." );

		// 5초안에 DB로부터 랭킹 정보를 못받으면 다시 받아오도록 하자!
		SendToLogDB( DBE_GET_HENIR_RANKING_INFO_REQ );
		return;
	}

	// 4. 랭킹 보상 예약
	if( m_bReservedRankingReward )
	{
		if( CheckRefreshTimeFileBackup() == false )
		{
			m_bReservedRankingReward = true;
		}
	}

	// 4. 랭킹 초기화 및 보상 체크
	if( m_tClearTimer.elapsed() > 60.0 )
	{
		CheckRankingClearAndReward();

		m_tClearTimer.restart();
	}

	// 5. 갱신된 랭킹 얻기
	KELG_HENIR_RANKING_REFRESH_NOT kPacket;
	GetHenirRankingInfo( kPacket.m_mapHenirRanking, true );

	// 6. 모든 게임서버에 브로드 캐스팅
	if( kPacket.m_mapHenirRanking.empty() == false )
	{
		UidType anTrace[2] = { 0, -1 };
		KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, 0 , anTrace, ELG_HENIR_RANKING_REFRESH_NOT, kPacket );
	}    
}

void KRankingManager::ClearRanking_LUA( bool bSendToFTP /*= true*/ )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( 0, NULL, ELG_CLEAR_RANKING_NOT, bSendToFTP );

	// 이벤트 큐잉
	KBaseServer::GetKObj()->QueueingEvent( spEvent );
}

bool KRankingManager::BackupRankingLogFile( IN int iRankingType, IN bool bSendToFTP /*= true*/ )
{	
	// 해당 랭킹 타입에 대한 랭킹 정보를 파일로 남기고 FTP로 전송
	std::map< int, KHenirRanking >::const_iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 랭킹타입의 랭킹정보를 LogFile로 남기려 했다" )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	const std::vector< KHenirRankingInfo >& vecRankingList = mit->second.GetRankingInfo();

	//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
	//////////////////////////////////////////////////////////////////////////
	KE_LOCAL_LOG_HENIR_RANKING_NOT kNot;
	kNot.m_iRankingType = iRankingType;
	kNot.m_vecRankingInfo = vecRankingList;
	KSIManager.QueueingEvent( E_LOCAL_LOG_HENIR_RANKING_NOT, kNot );
	//////////////////////////////////////////////////////////////////////////
#else
	//////////////////////////////////////////////////////////////////////////
	
	for( u_int ui = 0; ui < vecRankingList.size(); ++ui )
	{
		const KHenirRankingInfo& kRankingInfo = vecRankingList[ui];

		// 기록 시각 얻기
		CTime kEndGameTime = CTime( kRankingInfo.m_tRegDate );

#define KLocAlign std::setw(8) << std::setiosflags( std::ios::left )

		std::wfstream& fout = KSIManager.GetLocalLog().LocalLogStm_Henir_Ranking( iRankingType );

		fout << L"\n";
		fout << KLocAlign << mit->first << L"\t";
		fout << KLocAlign << ui << L"\t";
		fout << KLocAlign << kRankingInfo.m_iRank << L"\t";
		fout << KLocAlign << kRankingInfo.m_iStageCount << L"\t";
		fout << KLocAlign << kRankingInfo.m_ulPlayTime << L"\t";
		fout << KLocAlign << kRankingInfo.m_wstrNickName << L"\t";
		fout << KLocAlign << static_cast<int>(kRankingInfo.m_cUnitClass) << L"\t";
		fout << KLocAlign << static_cast<int>(kRankingInfo.m_ucLevel) << L"\t";
		fout << KLocAlign << (const wchar_t*)kEndGameTime.Format(_T("%Y-%m-%d %H:%M:%S")) << L"\t";

		fout.flush();
	}
	//////////////////////////////////////////////////////////////////////////
#endif SERV_STATISTICS_THREAD
	//}}

	// FTP 전송
	if( bSendToFTP )
	{
		//{{ 2010. 06. 21  최육사	통계 스레드
#ifdef SERV_STATISTICS_THREAD
		KE_HENIR_RANKING_LOG_NOT kNot;
		kNot.m_iRankingType = iRankingType;
		KSIManager.QueueingEvent( E_HENIR_RANKING_LOG_NOT, kNot );
#else
		KSIManager.SendHenirRankingLog( iRankingType );
#endif SERV_STATISTICS_THREAD
		//}}
	}

	// 확인 로그
	START_LOG( cout, L"랭킹 초기화에 따른 랭킹 정보 TEXT파일 백업 처리 완료!" )
		<< BUILD_LOG( iRankingType )
		<< BUILD_LOG( vecRankingList.size() )
		<< BUILD_LOG( bSendToFTP );

	return true;
}

void KRankingManager::RefreshTimeFileBackup()
{
	std::wstring wstrFilePath;

	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	wstrFilePath = buff;
	wstrFilePath += L"\\RefreshTimeBackup.ini";

	std::map< int, KHenirRanking >::const_iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		CStringW cstrRankingType;
		cstrRankingType.Format( L"RANKING_TYPE_%d", mit->first );
		
		CStringW cstrRefreshTime;
		cstrRefreshTime.Format( L"%d", mit->second.GetRefreshTime().GetTime() );

		::WritePrivateProfileStringW( L"Ranking Refresh Time", cstrRankingType.GetBuffer(), cstrRefreshTime.GetBuffer(), wstrFilePath.c_str() );
	}

	START_LOG( cout, L"서버 종료 전 랭킹 갱신 시각 백업 완료!" )
		<< BUILD_LOG( wstrFilePath );
}

bool KRankingManager::CheckRefreshTimeFileBackup()
{
	bool bCheckSuccess = true;
	std::wstring wstrFilePath;

	CTime tCurTime = CTime::GetCurrentTime();

	wchar_t buff[MAX_PATH] = {0};
	::GetCurrentDirectoryW( MAX_PATH, buff );
	wstrFilePath = buff;
	wstrFilePath += L"\\RefreshTimeBackup.ini";

	char strFilePath[255] = {0,};
	::WideCharToMultiByte( CP_ACP, 0, wstrFilePath.c_str(), -1, strFilePath, 255, NULL, NULL );

	std::ifstream ifs;
	ifs.open( strFilePath );
	if( !ifs.is_open() )
	{
		START_LOG( cout, L"RefreshTimeBackup파일이 없습니다! [참고] 서버를 처음 동작시켰을 경우 파일이 없을 수 있습니다." )
			<< BUILD_LOG( wstrFilePath );

		return true;
	}

	std::map< int, KHenirRanking >::iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		// 랭킹 보상 시작시각
		if( tCurTime < mit->second.GetRewardBeginTime() )
			continue;

		CStringW cstrRankingType;
		cstrRankingType.Format( L"RANKING_TYPE_%d", mit->first );

		memset( buff, 0, MAX_PATH );

		::GetPrivateProfileStringW( L"Ranking Refresh Time", cstrRankingType.GetBuffer(), 0, buff, MAX_PATH, wstrFilePath.c_str() );

		CTime tBackupRefreshTime( static_cast<__time64_t>(_wtoi( buff )) );

		// 백업되었던 refreshtime과 설정된 refreshtime을 비교한다!
		if( tBackupRefreshTime == mit->second.GetRefreshTime() )
			continue;

		// 랭킹 보상 처리
		if( RankingRewardProcess( mit->first ) == false )
		{
			START_LOG( clog, L"보상처리 실패! 예약해놓고 다음 Tick에 다시한번 시도하자!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;

			bCheckSuccess = false;
			continue;
		}

		// FTP로 전송
		if( BackupRankingLogFile( mit->first, true ) == false )
		{
			START_LOG( cerr, L"파일백업 및 FTP전송 실패!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			// 로그만 찍자~
		}

		// 랭킹 정보 초기화
		mit->second.ClearRanking();

#ifndef SERV_NOT_DELETE_HENIR_RANKING
		// 랭킹 백업데이터 초기화
		KDBE_DELETE_RANKING_INFO_NOT kNot;
		kNot.m_iRankingType = mit->first;
		SendToLogDB( DBE_DELETE_RANKING_INFO_NOT, kNot );
#endif // SERV_NOT_DELETE_HENIR_RANKING

		// 모든 처리가 성공하였으니 RefreshTimeBackup 정보를 갱신함
		CStringW cstrRefreshTime;
		cstrRefreshTime.Format( L"%d", mit->second.GetRefreshTime().GetTime() );

		::WritePrivateProfileStringW( L"Ranking Refresh Time", cstrRankingType.GetBuffer(), cstrRefreshTime.GetBuffer(), wstrFilePath.c_str() );

		START_LOG( cout, L"서버 종료 시간동안 RefreshTime이 포함되어있어서 서버켜자마자 보상처리함!" )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( tBackupRefreshTime.GetTime() );
	}

	return bCheckSuccess;
}

void KRankingManager::CheckRankingClearAndReward( bool bForce /*= false*/ )
{
	CTime tCurTime = CTime::GetCurrentTime();

	std::map< int, KHenirRanking >::iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
		//{{ 2009. 8. 3  최육사		랭킹 초기화 시작시각
		if( tCurTime < mit->second.GetRewardBeginTime()  &&  !bForce )
			continue;
		//}}

		// 초기화 할때가 되었남?
		if( tCurTime < mit->second.GetRefreshTime()  &&  !bForce )
			continue;
		
		// 랭킹 보상 처리
		if( RankingRewardProcess( mit->first ) == false )
		{
			START_LOG( cerr, L"랭킹 보상 처리 실패!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			continue;
		}

		// FTP로 전송
		if( BackupRankingLogFile( mit->first, true ) == false )
		{
			START_LOG( cerr, L"파일백업 및 FTP전송 실패!" )
				<< BUILD_LOG( mit->first )
				<< END_LOG;
			// 로그만 찍자~
		}

		// 랭킹 정보 초기화
		mit->second.ClearRanking();

#ifndef SERV_NOT_DELETE_HENIR_RANKING
		// 랭킹 백업데이터 초기화
		KDBE_DELETE_RANKING_INFO_NOT kNot;
		kNot.m_iRankingType = mit->first;
		SendToLogDB( DBE_DELETE_RANKING_INFO_NOT, kNot );
#endif // SERV_NOT_DELETE_HENIR_RANKING

		// 다음 RefreshTime 결정
		mit->second.NextRefreshTimeSet();
	}
}

void KRankingManager::GetHenirRankingInfo( std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking, bool bChangedOnly )
{
	mapHenirRanking.clear();

#ifndef SERV_HENIR_NO_REWARD
	if( bChangedOnly )
	{
		// 랭킹 100위안에 드는지 검사하여 든다면 랭킹갱신
		std::map< int, KHenirRanking >::iterator mitHR;
		for( mitHR = m_mapHenirRanking.begin(); mitHR != m_mapHenirRanking.end(); ++mitHR )
		{
			// 랭킹이 갱신되었는지 체크!
			if( mitHR->second.IsRankingChanged() )
			{
				mapHenirRanking.insert( std::make_pair( mitHR->first, mitHR->second.GetRankingInfo() ) );

				// 다음번에 체크 안되도록 false로 세팅
				mitHR->second.SetRankingChanged( false );
			}
		}
	}
	else
	{
		// 모든 랭킹 얻기
		std::map< int, KHenirRanking >::const_iterator mitHR;
		for( mitHR = m_mapHenirRanking.begin(); mitHR != m_mapHenirRanking.end(); ++mitHR )
		{
			mapHenirRanking.insert( std::make_pair( mitHR->first, mitHR->second.GetRankingInfo() ) );
		}
	}
#endif //SERV_HENIR_NO_REWARD
}

void KRankingManager::GetHenirRankingInfo( IN int iRankingType, OUT std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking )
{
	mapHenirRanking.clear();

	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 랭킹타입입니다." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return;
	}

	mapHenirRanking.insert( std::make_pair( mit->first, mit->second.GetRankingInfo() ) );	
}

void KRankingManager::InitHenirRanking( const std::map< int, std::vector< KHenirRankingInfo > >& mapHenirRanking )
{
	std::map< int, std::vector< KHenirRankingInfo > >::const_iterator mit;
	for( mit = mapHenirRanking.begin(); mit != mapHenirRanking.end(); ++mit )
	{
		std::map< int, KHenirRanking >::iterator mitHR;
		mitHR = m_mapHenirRanking.find( mit->first );
		if( mitHR == m_mapHenirRanking.end() )
		{
			START_LOG( cerr, L"DB로 부터 받은 랭킹리스트의 RankingType이 서버에 등록되지 않았다!" )
				<< BUILD_LOG( mit->first )
				<< BUILD_LOG( mit->second.size() )
				<< END_LOG;
			continue;
		}

		// 랭킹 리스트 정보 업데이트
		mitHR->second.SetRankingInfo( mit->second );

		START_LOG( cout, L"DB로부터 헤니르의 시공 랭킹 정보 받기 성공!" )
			<< BUILD_LOG( mit->first )
			<< BUILD_LOG( mit->second.size() );
	}

	// 헤니르의 시공 랭킹 백업 데이터 로드 성공!
	m_bInit = true;

	// 서버가 shutdown되었던 시간동안 refresh시점이 포함되었는지 검사하여 보상처리
	if( CheckRefreshTimeFileBackup() == false )
	{
		m_bReservedRankingReward = true; // 처리 실패시 다시처리예약!
	}
}

void KRankingManager::GetHenirLastRank( std::map< int, u_int >& mapLastRank )
{
	mapLastRank.clear();

	std::map< int, KHenirRanking >::iterator mitHR;
	for( mitHR = m_mapHenirRanking.begin(); mitHR != m_mapHenirRanking.end(); ++mitHR )
	{
		mapLastRank.insert( std::make_pair( mitHR->first, mitHR->second.GetLastRank() ) );
	}
}

bool KRankingManager::NewRecordHenirRanking( const KHenirRankingInfo& kNewRecord )
{
	bool bNewRecord = false;

	// 랭킹에 등록 시도! 새로운 랭크에 등록성공하면 true반환!
	std::map< int, KHenirRanking >::iterator mit;
	for( mit = m_mapHenirRanking.begin(); mit != m_mapHenirRanking.end(); ++mit )
	{
#ifdef SERV_HENIR_RENEWAL_2013// 작업날짜: 2013-09-17	// 박세훈
		if( ( mit->first == SEnum::RT_HERO_RANKING ) && ( kNewRecord.QualificationForHeroRank() == false ) )
			continue;
#endif // SERV_HENIR_RENEWAL_2013

		if( mit->second.CheckNewRecord( kNewRecord ) == true )
		{
			bNewRecord = true;
		}
	}

	return bNewRecord;
}

bool KRankingManager::NewRecordHenirRanking( IN const KHenirRankingInfo& kNewRecord, IN int iRankingType )
{
	// 랭킹에 등록 시도! 새로운 랭크에 등록성공하면 true반환!
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 랭킹타입입니다." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	START_LOG( cout2, L"----- 랭킹 추가 정보 -----" )
		<< BUILD_LOG( kNewRecord.m_iStageCount )
		<< BUILD_LOG( kNewRecord.m_ulPlayTime )
		<< BUILD_LOG( kNewRecord.m_tRegDate )
		<< BUILD_LOG( kNewRecord.m_iUnitUID )
		<< BUILD_LOG( kNewRecord.m_wstrNickName )
		<< BUILD_LOGc( kNewRecord.m_cUnitClass )
		<< BUILD_LOGc( kNewRecord.m_ucLevel );

	return mit->second.CheckNewRecord( kNewRecord );
}

//{{ 2009. 7. 31  최육사	랭킹 관리
void KRankingManager::NewRecordForTest( const std::vector< KHenirRankingInfo >& vecNewRecordList )
{
	std::vector< KHenirRankingInfo >::const_iterator vit;
	for( vit = vecNewRecordList.begin(); vit != vecNewRecordList.end(); ++vit )
	{
		SiKRankingManager()->NewRecordHenirRanking( *vit );
	}
}

bool KRankingManager::DeleteRecord( int iRankingType, const std::wstring& wstrNickName )
{
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 RankingType입니다." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}
    
	return mit->second.DeleteRecord( wstrNickName );
}

bool KRankingManager::DeleteRecord( int iRankingType, UidType iUnitUID )
{
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 RankingType입니다." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	return mit->second.DeleteRecord( iUnitUID );
}
//}}

//{{ 2011.03.04  임규수 헤니르 랭킹 삭제 치트 ( 운영자,개발자 계정 )
#ifdef SERV_DELETE_HENIR_RANKING
bool KRankingManager::DeleteSearchRank( int iRankingType, int iRank)
{
	std::map< int, KHenirRanking >::iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 RankingType입니다." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}

	std::vector< KHenirRankingInfo > vecRankingInfo = mit->second.GetRankingInfo();
	std::vector< KHenirRankingInfo >::iterator vitFind;
	for( vitFind = vecRankingInfo.begin(); vitFind != vecRankingInfo.end(); ++vitFind )
	{
		if( vitFind->m_iRank == iRank )
		{
			return mit->second.DeleteRecord( vitFind->m_wstrNickName );
		}
	}

	return false;
}
#endif SERV_DELETE_HENIR_RANKING
//}}

//{{ 2009. 7. 7  최육사		랭킹 개편
bool KRankingManager::RankingRewardProcess( IN int iRankingType )
{
	KELG_RANKING_TITLE_REWARD_NOT kPacketNot;

	std::map< int, KHenirRanking >::const_iterator mit;
	mit = m_mapHenirRanking.find( iRankingType );
	if( mit == m_mapHenirRanking.end() )
	{
		START_LOG( cerr, L"존재하지 않는 RankingType입니다." )
			<< BUILD_LOG( iRankingType )
			<< END_LOG;

		return false;
	}
	
	// 1. 칭호 보상 처리
	const std::vector< KHenirRankingInfo >& vecRankingList = mit->second.GetRankingInfo();
	std::vector< KHenirRankingInfo >::const_iterator vit;
	for( vit = vecRankingList.begin(); vit != vecRankingList.end(); ++vit )
	{
		KRankingTitleReward kRewardInfo;
        if( GetRankingTitleRewardInfo( iRankingType, vit->m_iRank, kRewardInfo.m_vecReward ) == false )
			continue;
		
		kRewardInfo.m_iUnitUID = vit->m_iUnitUID;
		kPacketNot.m_mapTitleReward.insert( std::make_pair( vit->m_iUnitUID, kRewardInfo ) );
	}

	if( kPacketNot.m_mapTitleReward.empty() )
		return true;

	//{{ 2011. 11. 3	최육사	헤니르 시공 랭킹 보상 안전성 패치
#ifdef SERV_HENIR_RANKING_TITLE_REWARD_FIX
	// 3. 게임서버로 랭킹 보상 보내기
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_LOGIN_NX_AUTH, NULL, ELG_RANKING_TITLE_REWARD_NOT, kPacketNot );
	GetKLoginServer()->QueueingEvent( spEvent );
#else
	//// 2. 연결된 게임서버중 한군데만 전송
	//UidType iGameServerUID = KBaseServer::GetKObj()->GetFirstActorKey();
	//if( iGameServerUID <= 0 )
	//{
	//	START_LOG( clog, L"랭킹 타이틀 보상을 게임서버로 전송하려는데 ActorManager에 연결된 게임서버가 하나도 없군.." )
	//		<< BUILD_LOG( iGameServerUID )
	//		<< END_LOG;

	//	return false;
	//}

	//// 3. 게임서버로 랭킹 보상 보내기
	//UidType anTrace[2] = { iGameServerUID, -1 };
	//KncSend( PI_LOGIN_SERVER, KBaseServer::GetKObj()->GetUID(), PI_GS_SERVER, iGameServerUID, anTrace, ELG_RANKING_TITLE_REWARD_NOT, kPacketNot );
#endif SERV_HENIR_RANKING_TITLE_REWARD_FIX
	//}}	

	// 4. 확인 로그
	START_LOG( cout, L"랭킹 칭호 보상 처리 완료!" )
		<< BUILD_LOG( iRankingType )
		<< BUILD_LOG( kPacketNot.m_mapTitleReward.size() );

	return true;
}

bool KRankingManager::GetRankingTitleRewardInfo( IN int iRankingType, IN int iRank, OUT std::vector< KTitleReward >& vecReward )
{
	vecReward.clear();

	KTitleRewardKey kKey;
	kKey.first  = iRankingType;
	kKey.second = iRank;

	std::map< KTitleRewardKey, std::vector< KTitleReward > >::const_iterator mit;
	mit = m_mapRankingTitleReward.find( kKey );
	if( mit == m_mapRankingTitleReward.end() )
		return false;

	vecReward = mit->second;
	return true;
}
//}}

//{{ 2009. 11. 9  최육사	로그인서버GameDB
void KRankingManager::TickDungeonAndPvpRanking()
{
	// 1. 1분마다 체크
	if( m_kRefreshTimer.elapsed() < 60.0 )
		return;

	// 2. 타이머 초기화	
	m_kRefreshTimer.restart();

	// 3. 설정된 시각이 되면 던전, 대전랭킹 다시 얻기
	CTime tCurTime = CTime::GetCurrentTime();
	if( tCurTime > m_tResetRankingTime )
	{
		// 4. 던전, 대전 랭킹 다시 얻기
		SendToGameDB( DBE_GET_WEB_RANKING_INFO_REQ );

		// 다음날로 세팅
		m_tResetRankingTime += CTimeSpan( 1, 0, 0, 0 );
	}
}

void KRankingManager::UpdateDungeonRanking( const std::vector< KDungeonRankingInfo >& vecDungeonRanking )
{
	m_vecDungeonRanking.clear();
	m_mapDungeonRanking.clear();

	std::vector< KDungeonRankingInfo >::const_iterator vit;
	for( vit = vecDungeonRanking.begin(); vit != vecDungeonRanking.end(); ++vit )
	{
		m_mapDungeonRanking.insert( std::make_pair( vit->m_iUnitUID, *vit ) );

		if( m_vecDungeonRanking.size() < 100 )
		{
			m_vecDungeonRanking.push_back( *vit );
		}
	}

	START_LOG( cout, L"던전 랭킹 갱신!" )
		<< BUILD_LOG( m_mapDungeonRanking.size() )
		<< BUILD_LOG( m_vecDungeonRanking.size() );
}

void KRankingManager::UpdatePvpRanking( const std::vector< KPvpRankingInfo >& vecPvpRanking )
{
	m_vecPvpRanking.clear();
	m_mapPvpRanking.clear();

	std::vector< KPvpRankingInfo >::const_iterator vit;
	for( vit = vecPvpRanking.begin(); vit != vecPvpRanking.end(); ++vit )
	{
		m_mapPvpRanking.insert( std::make_pair( vit->m_iUnitUID, *vit ) );

		if( m_vecPvpRanking.size() < 100 )
		{
			m_vecPvpRanking.push_back( *vit );
		}
	}

	START_LOG( cout, L"대전 랭킹 갱신!" )
		<< BUILD_LOG( m_mapPvpRanking.size() )
		<< BUILD_LOG( m_vecPvpRanking.size() );
}

void KRankingManager::GetDungeonRankingVector( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// 게임서버가 마지막으로 받은 index가 전체 사이즈보다 크다면 더이상 주지 않는다.
	if( uiLastIndex >= m_vecDungeonRanking.size() )
		return;

	for( u_int ui = uiLastIndex; ui < m_vecDungeonRanking.size(); ++ui )
	{
		kNot.m_vecDungeonRanking.push_back( m_vecDungeonRanking[ui] );

		if( kNot.m_vecDungeonRanking.size() >= 150 )		
			break;
	}

	kNot.m_uiTotalSize = m_vecDungeonRanking.size();
}

void KRankingManager::GetDungeonRankingMap( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// 게임서버가 마지막으로 받은 index가 전체 사이즈보다 크다면 더이상 주지 않는다.
	if( uiLastIndex >= m_mapDungeonRanking.size() )
		return;

	u_int uiCount = 0;
	std::map< UidType, KDungeonRankingInfo >::const_iterator mit;
	for( mit = m_mapDungeonRanking.begin(); mit != m_mapDungeonRanking.end(); ++mit, ++uiCount )
	{
		if( uiCount < uiLastIndex )
			continue;

		kNot.m_mapDungeonRanking.insert( std::make_pair( mit->first, mit->second ) );

		if( kNot.m_mapDungeonRanking.size() >= 150 )
			break;
	}

	kNot.m_uiTotalSize = m_mapDungeonRanking.size();
}

void KRankingManager::GetPvpRankingVector( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// 게임서버가 마지막으로 받은 index가 전체 사이즈보다 크다면 더이상 주지 않는다.
	if( uiLastIndex >= m_vecPvpRanking.size() )
		return;

	for( u_int ui = uiLastIndex; ui < m_vecPvpRanking.size(); ++ui )
	{
		kNot.m_vecPvpRanking.push_back( m_vecPvpRanking[ui] );

		if( kNot.m_vecPvpRanking.size() >= 150 )
			break;
	}

	kNot.m_uiTotalSize = m_vecPvpRanking.size();
}

void KRankingManager::GetPvpRankingMap( u_int uiLastIndex, KELG_WEB_RANKING_REFRESH_NOT& kNot )
{
	// 게임서버가 마지막으로 받은 index가 전체 사이즈보다 크다면 더이상 주지 않는다.
	if( uiLastIndex >= m_mapPvpRanking.size() )
		return;

	u_int uiCount = 0;
	std::map< UidType, KPvpRankingInfo >::const_iterator mit;
	for( mit = m_mapPvpRanking.begin(); mit != m_mapPvpRanking.end(); ++mit, ++uiCount )
	{
		if( uiCount < uiLastIndex )
			continue;

		kNot.m_mapPvpRanking.insert( std::make_pair( mit->first, mit->second ) );

		if( kNot.m_mapPvpRanking.size() >= 150 )
			break;
	}

	kNot.m_uiTotalSize = m_mapPvpRanking.size();
}
//}}

void KRankingManager::SendToGameDB( unsigned short usEventID )
{
	SendToGameDB( usEventID, char() );
}

void KRankingManager::SendToLogDB( unsigned short usEventID )
{
	SendToLogDB( usEventID, char() );
}



