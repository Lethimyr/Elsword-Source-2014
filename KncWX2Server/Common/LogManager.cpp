#include "LogManager.h"
#include <dbg/dbg.hpp>



//{{ 2013. 02. 15	최육사	로그 시스템 개선
//#ifdef SERV_LOG_SYSTEM_NEW

ImplementRefreshSingleton( KLogManager )

KLogManager::KLogManager() :
m_fCheckTimeGap( 300.0 ),				// 디폴트 값은 5분
m_fErrorLogStatTimeGap( 1800.0 ),		// 디폴트 값은 30분
m_iLogFileMaxSize( 10 * 1024 * 1024 ),	// 디폴트 값은 10 mb
m_bNewLogByFileSize( false ),			// 디폴트 값은 false
m_bNewLogDaily( false ),				// 디폴트 값은 false
m_bErrorLogStat( false )				// 디폴트 값은 false
{
	m_tLastCheckTime = CTime::GetCurrentTime();
}

KLogManager::~KLogManager()
{
}

ImplToStringW( KLogManager )
{
	return stm_;
}

ImplementLuaScriptParser( KLogManager )
{
	lua_tinker::class_add<KLogManager>( GetLuaState(), "KLogManager" );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetNewLogByFileSize",		&KLogManager::SetNewLogByFileSize_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetNewLogDaily",			&KLogManager::SetNewLogDaily_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetErrorLogStat",			&KLogManager::SetErrorLogStat_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetCheckTimeGap",			&KLogManager::SetCheckTimeGap_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetErrorLogStatTimeGap",	&KLogManager::SetErrorLogStatTimeGap_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "SetLogFileMaxSize",			&KLogManager::SetLogFileMaxSize_LUA );
	lua_tinker::class_def<KLogManager>( GetLuaState(), "dump",						&KLogManager::Dump );

	lua_tinker::decl( GetLuaState(), "LogManager", this );
}

void KLogManager::SetNewLogByFileSize_LUA( bool bValue )
{
	m_bNewLogByFileSize = bValue;

	START_LOG( cout, L"파일 크기 초과시 로그 파일 생성 여부 : " << m_bNewLogByFileSize );
}

void KLogManager::SetNewLogDaily_LUA( bool bValue )
{
	m_bNewLogDaily = bValue;

	START_LOG( cout, L"매일 새로운 로그 파일 생성 여부 : " << m_bNewLogDaily );
}

void KLogManager::SetErrorLogStat_LUA( bool bValue )
{
	m_bErrorLogStat = bValue;

	START_LOG( cout, L"에러 로그 통계 남기기 여부 : " << m_bErrorLogStat );
}

void KLogManager::SetCheckTimeGap_LUA( double fTimeGap )
{
	m_fCheckTimeGap = fTimeGap;

	START_LOG( cout, L"로그 파일 체크 타임 주기 : " << m_fCheckTimeGap );
}

void KLogManager::SetErrorLogStatTimeGap_LUA( double fTimeGap )
{
	m_fErrorLogStatTimeGap = fTimeGap;

	START_LOG( cout, L"에러 로그 통계 DB업데이트 타임 주기 : " << m_fErrorLogStatTimeGap );
}

void KLogManager::SetLogFileMaxSize_LUA( int iMByte, int iKByte )
{
	m_iLogFileMaxSize = 0;
	m_iLogFileMaxSize += 1024 * iKByte;
	m_iLogFileMaxSize += 1024 * 1024 * iMByte;

	START_LOG( cout, L"로그 파일 최대 크기" )
		<< BUILD_LOG( m_iLogFileMaxSize )
		<< BUILD_LOG( m_iLogFileMaxSize / 1024 )
		<< BUILD_LOG( m_iLogFileMaxSize / 1024 / 1024 );
}

void KLogManager::Tick()
{
	// 특정 주기 마다 체크
	if( m_tCheckTimer.elapsed() < m_fCheckTimeGap )
		return;

	m_tCheckTimer.restart();

	// 파일 사이즈 구하기!
	if( m_bNewLogByFileSize )
	{
		const int iFileSize = dbg::logfile::GetLogFileSize();

		// 파일 사이즈가 특정 크기를 넘으면 새로운 로그 파일을 만든다!
		if( iFileSize > m_iLogFileMaxSize )
		{
			// 로그 새로 찍어서 파일 계속 하기!
			START_LOG( cout, L"--------------------------- 로그 파일 용량이 10mb가 초과하여 새로운 파일로 로그 파일을 생성합니다! ---------------------------" )
				<< BUILD_LOG( iFileSize )
				<< BUILD_LOG( m_iLogFileMaxSize )
				<< BUILD_LOG( iFileSize / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 )
				<< BUILD_LOG( iFileSize / 1024 / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 / 1024 );

			// 파일 종료하기!
			dbg::logfile::CloseFile();

			// 로그 새로 찍어서 파일 계속 하기!
			START_LOG( cout, L"--------------------------- 로그 파일 용량이 10mb가 초과하여 새로운 파일로 로그 파일을 생성합니다! ---------------------------" )
				<< BUILD_LOG( iFileSize )
				<< BUILD_LOG( m_iLogFileMaxSize )
				<< BUILD_LOG( iFileSize / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 )
				<< BUILD_LOG( iFileSize / 1024 / 1024 )
				<< BUILD_LOG( m_iLogFileMaxSize / 1024 / 1024 );
			return;
		}
	}
	
	// 0시 기준으로 서버 로그 파일을 갱신합니다.
	if( m_bNewLogDaily )
	{
		CTime tBeforeCheckTime = m_tLastCheckTime;
		m_tLastCheckTime = CTime::GetCurrentTime();

		// 날짜가 달라졌다면 다음날이 된거다!
		if( tBeforeCheckTime.GetDay() != m_tLastCheckTime.GetDay() )
		{
			// 로그 새로 찍어서 파일 계속 하기!
			START_LOG( cout, L"--------------------------- 매일 0시 기준으로 새로운 로그 파일을 생성합니다! ---------------------------" )
				<< BUILD_LOG( tBeforeCheckTime.GetDay() )
				<< BUILD_LOG( m_tLastCheckTime.GetDay() );

			// 파일 종료하기!
			dbg::logfile::CloseFile();

			// 로그 새로 찍어서 파일 계속 하기!
			START_LOG( cout, L"--------------------------- 매일 0시 기준으로 새로운 로그 파일을 생성합니다! ---------------------------" )
				<< BUILD_LOG( tBeforeCheckTime.GetDay() )
				<< BUILD_LOG( m_tLastCheckTime.GetDay() );
			return;
		}
	}

	// 에러 로그 통계 남기기
	if( m_bErrorLogStat )
	{
		if( m_tErrorLogStatTimer.elapsed() > m_fErrorLogStatTimeGap )
		{
			m_tErrorLogStatTimer.restart();

			std::map< unsigned int, dbg::logfile::KLogStat > mapLogStat;
			dbg::cerr.GetLogStat( mapLogStat );
			dbg::cerr.ClearLogStat();

			KDBE_LOG_STATISTICS_INFO_NOT kPacketToLog;

			std::map< unsigned int, dbg::logfile::KLogStat >::const_iterator mit;
			for( mit = mapLogStat.begin(); mit != mapLogStat.end(); ++mit )
			{
				//START_LOG( cout, L"--------------------------- 로그 통계 ---------------------------" )
				//	<< BUILD_LOG( mit->second.m_wstrFunctionName )
				//	<< BUILD_LOG( mit->second.m_wstrFileName )
				//	<< BUILD_LOG( mit->second.m_wstrLineNum )
				//	<< BUILD_LOG( mit->second.m_iCount );

				KLogStatInfo kInfo;
				kInfo.m_wstrFunctionName = mit->second.m_wstrFunctionName;
				kInfo.m_wstrFileName	 = mit->second.m_wstrFileName;
				kInfo.m_wstrLineNum		 = mit->second.m_wstrLineNum;
				kInfo.m_iCount			 = mit->second.m_iCount;
				kPacketToLog.m_vecLogStat.push_back( kInfo );
			}

			if( kPacketToLog.m_vecLogStat.empty() == false )
			{
				SendToLogDB( DBE_LOG_STATISTICS_INFO_NOT, kPacketToLog );

				START_LOG( cout, L"--------------------------- 에러 로그 통계를 남깁니다! ---------------------------" )
					<< BUILD_LOG( kPacketToLog.m_vecLogStat.size() );
			}
		}
	}
}

//#endif SERV_LOG_SYSTEM_NEW
//}}
