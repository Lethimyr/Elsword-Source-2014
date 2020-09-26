#include "ThreadStatisticsManager.h"

#include "KFolderTraverse.h"

#include <dbg/dbg.hpp>
#include <algorithm>
#include "Socket/NetCommon.h"
#include "BaseServer.h"


#ifdef SERV_STATISTICS_THREAD


ImplToStringW( KThreadStatisticsManager )
{
    stm_ << L"-> " << L"KThreadStatisticsManager" << std::endl
         << TOSTRINGW( ( int )m_mapStatistics.size() );

    std::map< int, KStatisticsPtr >::const_iterator mit;
    for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
    {
        stm_ << L"    " << L"( " << KStatistics::GetIDStr( mit->first )
             << L", " << mit->second->GetFlushGap()
             << L", " << ( mit->second->IsWriteToDB() ? L"true" : L"false" ) << L" )" << std::endl;
    }

    return stm_;
}

KThreadStatisticsManager::KThreadStatisticsManager()
{
	m_bIsInit					= 0;
	m_iLocalLogLastCheckTime	= 0;
	m_iLocalLogLastUpdateTime	= 0;
	m_bSendToFTPStatistics		= false;
	m_bUserLog					= false;
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	m_bBillingPacketLog			= false;
#endif SERV_BILLING_PACKET_LOG
	//}}

	CreateDirectory( L"Statistics", NULL );

	m_wstrFTPServerIP			= L"14.45.79.17";
	m_iFTPPort					= 21;
	m_wstrFTPID					= L"elswordftp";
	m_wstrFTPPW					= L"spdlqj.com6424";

	//{{ 2011. 11. 21  김민성	통계 로그 수치
#ifdef SERV_STATISTICS_LOG_COUNT
	m_iStatisticsFileNo = 0;
	m_tStatisticsLogCount.restart();
#endif SERV_STATISTICS_LOG_COUNT
	//}}
}

KThreadStatisticsManager::~KThreadStatisticsManager()
{
}

void KThreadStatisticsManager::Run()
{
	START_LOG( clog, L"Enter Run()" );

	DWORD ret;

	while( true )
	{
		ret = ::WaitForSingleObject( m_hKillEvent, 1 );

		if( ret == WAIT_OBJECT_0 ) break;

		if( ret == WAIT_TIMEOUT ) Tick();

		else    START_LOG( cout, L"*** WaitForSingleObject() - return : " << ret );
	}
}

void KThreadStatisticsManager::Init()
{
	// locallogkey초기화
	InitLocalLogKeys();

	START_LOG( cout, L"Thread Statistics Manager 초기화 완료! thread시작!" );

	// thread시작
	Begin();

	// 초기화 여부 저장
	m_bIsInit = true;
}

void KThreadStatisticsManager::Shutdown()
{
	// 초기화 되지 않았다면 아무런 처리를 하지 않는다!
	if( m_bIsInit == false )
		return;

	// thread정지
	End();

	// 통계 정보 백업
	FlushData();
	OnFlushLocalLog();
}

#include <KncLua.h>
#include <lua_tinker.h>

void KThreadStatisticsManager::RegToLua()
{
	lua_tinker::class_add<KThreadStatisticsManager>( g_pLua, "KStatisticsManager" );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "dump",					&KThreadStatisticsManager::Dump );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "dumpstat",				&KThreadStatisticsManager::DumpStatistics );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "AddStatistics",			&KThreadStatisticsManager::AddStatistics_LUA );
	//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SetStatisticsFlushTime",	&KThreadStatisticsManager::SetStatisticsFlushTime_LUA );
#endif SERV_ITEM_STATISTICS_TO_DB
	//}}
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "OnFlushLocalLog",			&KThreadStatisticsManager::OnFlushLocalLog );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "FlushData",				&KThreadStatisticsManager::FlushData );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "ClearLocalLogData",		&KThreadStatisticsManager::ClearLocalLogData_ );
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SendToFTPStatistics",		&KThreadStatisticsManager::SetSendToFTPStatistics_LUA );	
	//{{ 2009. 9. 13  최육사	유저로그
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SetUserLog",				&KThreadStatisticsManager::SetUserLog_LUA );
	//}}
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SetBillingPacketLog",		&KThreadStatisticsManager::SetBillingPacketLog_LUA );
#endif SERV_BILLING_PACKET_LOG
	//}}
	//{{ 2009. 5. 7  최육사		테스트코드
	//lua_tinker::class_def<KThreadStatisticsManager>( g_pLua, "SendServerLog",			&KThreadStatisticsManager::SendServerLog );
	//}}	

#   undef _ENUM
#   define _ENUM( id ) lua_tinker::decl( g_pLua, #id, KStatistics::##id );
#   include "StatisticsID_def.h"

	lua_tinker::decl( g_pLua, "StatisticsManager", this );
}

void KThreadStatisticsManager::AddStatistics_LUA( int iStatisticsID, float fFlushGap, bool bWriteToDB )
{
	KStatisticsPtr spStatistics = KStatisticsPtr( new KStatistics );
	spStatistics->Init( iStatisticsID, fFlushGap, bWriteToDB );
	m_mapStatistics.insert( std::make_pair( iStatisticsID, spStatistics ) );
}

//{{ 2011. 04. 13	최육사	아이템 통계 DB기록
#ifdef SERV_ITEM_STATISTICS_TO_DB
void KThreadStatisticsManager::SetStatisticsFlushTime_LUA( int iStatisticsID, int iHour )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		START_LOG( cerr, L"통계 객체가 존재하지 않습니다!" )
			<< BUILD_LOG( iStatisticsID )
			<< BUILD_LOG( iHour )
			<< END_LOG;
		return;
	}

	KStatisticsPtr spStatistics = mit->second;
	if( spStatistics == NULL )
	{
		START_LOG( cerr, L"통계 객체 포인터 값이 이상합니다!" )
			<< BUILD_LOG( iStatisticsID )
			<< BUILD_LOG( iHour )
			<< END_LOG;
		return;
	}
	
	// 특정 시각 Flush세팅
	spStatistics->SetFlushTime( iHour );
}
#endif SERV_ITEM_STATISTICS_TO_DB
//}}

void KThreadStatisticsManager::SetSendToFTPStatistics_LUA( bool bSendToFTPStatistics )
{
	m_bSendToFTPStatistics = bSendToFTPStatistics;

	START_LOG( cout2, L"SetSendToFTPStatistics : " << m_bSendToFTPStatistics );
}

void KThreadStatisticsManager::SetUserLog_LUA( bool bVal )
{
	m_bUserLog = bVal;

	START_LOG( cout2, L"SetUserLog : " << m_bUserLog );
}

//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
void KThreadStatisticsManager::SetBillingPacketLog_LUA( bool bVal )
{
	m_bBillingPacketLog = bVal;

	START_LOG( cout2, L"SetBillingPacketLog : " << m_bBillingPacketLog );
}
#endif SERV_BILLING_PACKET_LOG
//}}

void KThreadStatisticsManager::IncreaseCount( int iStatisticsID, const KStatisticsKey& kKey, int iIndex, int iCount )
{
	KE_STATISTICS_INFO_NOT kNot;
	kNot.AddStatisticsInfo( iStatisticsID, kKey, iIndex, iCount );

	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, E_STATISTICS_INFO_NOT, kNot );
	m_kTSMEventProcess.QueueingEvent( spEvent );
}

void KThreadStatisticsManager::IncreaseCount( const KE_STATISTICS_INFO_NOT& kInfo )
{
	KEventPtr spEvent( new KEvent );
	spEvent->SetData( PI_NULL, NULL, E_STATISTICS_INFO_NOT, kInfo );
	m_kTSMEventProcess.QueueingEvent( spEvent );
}

void KThreadStatisticsManager::QueueingEventID( unsigned short usEventID )
{
	QueueingEvent( usEventID, char() );
}

int KThreadStatisticsManager::GetCurLocalTime()
{
	time_t t0 = time(0);
#ifdef _CONVERT_VS_2010
	tm t;
	localtime_s(&t, &t0);
	int iCurTime = t.tm_year * 365 * 24 * 60 + t.tm_yday * 24 * 60 + t.tm_hour * 60 + t.tm_min;
#else
	tm* t = localtime(&t0);
	int iCurTime = t->tm_year * 365 * 24 * 60 + t->tm_yday * 24 * 60 + t->tm_hour * 60 + t->tm_min;
#endif _CONVERT_VS_2010

	return iCurTime;
}

void KThreadStatisticsManager::ClearLocalLogData_()
{
	m_kStatisticsLocalLog.ClearLocalLog();

	// 덤프 시간 저장
	int iCurTime = GetCurLocalTime();
	std::wfstream fout( "Statistics\\LastUpdateLog.txt", std::ios::out );
	fout << iCurTime;
	fout.close();
}

void KThreadStatisticsManager::_QueueingEvent( const KEventPtr& spEvent_ )
{
	m_kTSMEventProcess.QueueingEvent( spEvent_ );
}

void KThreadStatisticsManager::Tick()
{
	std::map< int, KStatisticsPtr >::iterator mit;
	for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
	{
		mit->second->Tick();
	}

	UpdateFlushLocalLog();

	//{{ 2008. 4. 7  최육사  파일로 존재하는 그날의 통계는 다시 읽지 않는다. [서버 통계 기획서 참고]
	//m_kStatisticsLocalLog.LoadLocalLogFromFile();
	//}}

	// 이벤트 처리
	m_kTSMEventProcess.Tick();


	//{{ 2011. 11. 21  김민성	통계 로그 수치
#ifdef SERV_STATISTICS_LOG_COUNT
	WriteStatisticsLogCount();
#endif SERV_STATISTICS_LOG_COUNT
	//}}
}

void KThreadStatisticsManager::FlushData()
{
	std::map< int, KStatisticsPtr >::iterator mit;
	for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
	{
		mit->second->FlushData();
	}
}

void KThreadStatisticsManager::DumpStatistics( int iStatisticsID ) const
{
    std::map< int, KStatisticsPtr >::const_iterator mit = m_mapStatistics.find( iStatisticsID );
    if( mit == m_mapStatistics.end() )
    {
        START_LOG( cerr, L"해당 통계가 없거나 등록되어 있지 않음." )
            << BUILD_LOG( iStatisticsID )
            << BUILD_LOG( KStatistics::GetIDStr( iStatisticsID ) )
            << END_LOG;
    }
    else
	{
        mit->second->Dump();
    }
}

bool KThreadStatisticsManager::CheckStatistics( int iStatisticsID )
{
	std::map< int, KStatisticsPtr >::const_iterator mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		return false;
	}

	return true;
}

bool KThreadStatisticsManager::UpdateStatistics( int iStatisticsID, const KStatisticsKey& kKey, int iIndex, int iCount )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		START_LOG( cerr, L"요런 통계 없음." )
			<< BUILD_LOG( iStatisticsID )
			<< BUILD_LOG( iIndex )
			<< BUILD_LOG( iCount )
			<< END_LOG;

		return false;
	}

	mit->second->IncreaseCount( kKey, iIndex, iCount );
	return true;
}

//-----------------------------------------------------------------------------
// Local Log

void KThreadStatisticsManager::InitLocalLogKeys()
{
	std::map< int, KStatisticsPtr >::iterator mit;
	for( mit = m_mapStatistics.begin(); mit != m_mapStatistics.end(); ++mit )
	{
		mit->second->InitLocalLogKeys();
	}
}

void KThreadStatisticsManager::WriteLocalLogData( std::wfstream& fout, int iStatisticsID )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		START_LOG( cerr, L"요런 통계 없음." )
			<< BUILD_LOG( iStatisticsID )
			<< END_LOG;

		return;
	}

	mit->second->WriteLocalLogData( fout );
}

//{{ 2007. 12. 18  최육사  
void KThreadStatisticsManager::ClearLocalLogData( int iStatisticsID )
{
	std::map< int, KStatisticsPtr >::iterator mit;
	mit = m_mapStatistics.find( iStatisticsID );
	if( mit == m_mapStatistics.end() )
	{
		// 에러로그 찍을필요 없음.
		// [참고] 센터서버와 게임서버에서 사용되는 Local Log 항목이 다르기 때문.
		return;
	}

	mit->second->ClearLocalLogData();
}
//}}

//{{ 2008. 5. 20  최육사  통계 코드 수정
struct KSendLocalLog
{
	KLogReporter& m_kLogReporter;
	const std::wstring m_wstrFindTagName;
	KSendLocalLog( KLogReporter& kLogReporter, const wchar_t* wstrFindTagName )	: m_kLogReporter( kLogReporter ), m_wstrFindTagName( wstrFindTagName ) {}

	void operator()( const WIN32_FIND_DATA& wfd )
	{
		if ( ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == false )
		{
			std::wstring strFilename = wfd.cFileName;
			//std::transform(strFilename.begin(), strFilename.end(), strFilename.begin(), std::toupper);

			std::wstring strFindTag = m_wstrFindTagName;
			int iPosTag = strFilename.find( strFindTag );
			if ( iPosTag == std::string::npos )
				return;

			std::wstring strFindExt = L".txt";
			int iPosExt = strFilename.rfind( strFindExt );
			if ( iPosExt == std::string::npos )
				return;

			std::wstring strRootName = L"Statistics\\";

			std::wstring strFileFirst = strFilename.substr( 0, iPosExt );
			std::wostringstream ostmDgTag;
			ostmDgTag << strFileFirst;
			KThreadStatisticsManager::AttachTimeTag( ostmDgTag );
			KThreadStatisticsManager::AttachSvnameTag( ostmDgTag );
			ostmDgTag << L".txt";

			m_kLogReporter.InsertFile( (strRootName + strFilename).c_str(), ostmDgTag.str().c_str() );
		}
	}	
};
//}}

void KThreadStatisticsManager::BackUpAndSendFTPLocalLog( bool bSendFileFtp )
{
	START_LOG( cout, L"텍스트 통계 파일 백업 시작!" );

	KLogReporter kLogReporter;
	kLogReporter.SetConnectInfo( m_wstrFTPServerIP, m_iFTPPort );
	kLogReporter.SetAccount( m_wstrFTPID, m_wstrFTPPW );

	// - - - - - - - - - - - - - - - - - - - - - - - - -
	// File
	std::wostringstream ostmPvp;
	ostmPvp << L"SI_LOG_PVP";
	AttachTimeTag( ostmPvp );
	AttachSvnameTag( ostmPvp );
	ostmPvp << L".txt";

	std::wostringstream ostmItem;
	ostmItem << L"SI_LOG_ITEM";
	AttachTimeTag( ostmItem );
	AttachSvnameTag( ostmItem );
	ostmItem << L".txt";
	
	std::wostringstream ostmSpirit;
	ostmSpirit << L"SI_LOG_SPIRIT";
	AttachTimeTag( ostmSpirit );
	AttachSvnameTag( ostmSpirit );
	ostmSpirit << L".txt";
	
	std::wostringstream ostmEnchant;
	ostmEnchant << L"SI_LOG_ENCHANT";
	AttachTimeTag( ostmEnchant );
	AttachSvnameTag( ostmEnchant );
	ostmEnchant << L".txt";

	// - - - - - - - - - - - - - - - - - - - - - - - - -

	m_kStatisticsLocalLog.CloseLocalLogData();

	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_DUNGEON" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_PVP_VICTORY_PER" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_PVP_ROOM" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_DUNGEON_ROOM" ) );
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_TITLE" ) );
	//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_PET_SUMMON" ) );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_SERV_DISCONNECT" ) );
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_BILLING_PACKET" ) );
#endif SERV_BILLING_PACKET_LOG
	//}}
	//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_ABUSER_MORNITORING" ) );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_UNDEFINED_QUEST_TEMPLET" ) );
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

	if ( CheckStatistics( KStatistics::SI_LOC_PVP ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_PVP.txt", ostmPvp.str().c_str() );

	if ( CheckStatistics( KStatistics::SI_LOC_ITEM ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_ITEM.txt", ostmItem.str().c_str() );
	
	if ( CheckStatistics( KStatistics::SI_LOC_SPIRIT ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_SPIRIT.txt", ostmSpirit.str().c_str() );
	
	if ( CheckStatistics( KStatistics::SI_LOC_ENCHANT ) == true )
		kLogReporter.InsertFile( L"Statistics\\SI_LOG_ENCHANT.txt", ostmEnchant.str().c_str() );

	// FTP전송 파일 백업
	BackUpSendFTPFiles( kLogReporter.m_vecFileList );

	START_LOG( cout, L"텍스트 통계 파일 백업 완료!" );

	// FTP전송
	if( bSendFileFtp )
	{
		START_LOG( cout, L"텍스트 통계 파일 FTP 전송 시작!" );

		kLogReporter.Send();

		START_LOG( cout, L"텍스트 통계 파일 FTP 전송 완료!" );
	}
}

void KThreadStatisticsManager::SendHenirRankingLog( int iRankingType )
{
	START_LOG( cout, L"헤니르 랭킹 로그 파일 백업 시작!" )
		<< BUILD_LOG( iRankingType );

	KLogReporter kLogReporter;
	kLogReporter.SetConnectInfo( m_wstrFTPServerIP, m_iFTPPort );
	kLogReporter.SetAccount( m_wstrFTPID, m_wstrFTPPW );

	m_kStatisticsLocalLog.CloseHenirRankingLogData( iRankingType );

	KApplyAllFiles( KSendLocalLog( kLogReporter, L"SI_LOG_HENIR_RANKING" ) );

	// FTP전송 파일 백업
	BackUpSendFTPFiles( kLogReporter.m_vecFileList );

	START_LOG( cout, L"헤니르 랭킹 로그 파일 백업 완료!" )
		<< BUILD_LOG( iRankingType );

	//kLogReporter.Send(); - 문제가 좀 있다, 좀더 테스트 필요, 일단 파일백업만이라도 해두자!

	// 파일 삭제
	m_kStatisticsLocalLog.DeleteHenirRankingLog();
}

//{{ 2008. 6. 27  최육사  FTP전송 파일 백업
void KThreadStatisticsManager::BackUpSendFTPFiles( const std::vector< KLogReporter::KFileInfo >& vecFileList )
{
	CTime tCurTime = CTime::GetCurrentTime();

	// 1. 백업 폴더 생성
#ifdef SERV_FTP_PATH_CHANGE
	std::wstring wstrBackupFolder = L"D:\\StatisticsBackup\\";
#else // SERV_FTP_PATH_CHANGE
	std::wstring wstrBackupFolder = L"C:\\StatisticsBackup\\";
#endif // SERV_FTP_PATH_CHANGE
	CreateDirectoryW( wstrBackupFolder.c_str(), NULL );
		
	// 2. 일자별 폴더 생성
	wstrBackupFolder += ( CStringW )( tCurTime.Format( _T( "%Y-%m-%d\\" ) ) );
	CreateDirectoryW( wstrBackupFolder.c_str(), NULL );

	// 3. 통계 및 로그 파일 백업
	std::vector< KLogReporter::KFileInfo >::const_iterator vit;
	for( vit = vecFileList.begin(); vit != vecFileList.end(); ++vit )
	{	
		const KLogReporter::KFileInfo& kFileInfo = *vit;		

		std::wstring wstrDesFileName = wstrBackupFolder;
		wstrDesFileName += kFileInfo.m_strRemoteFileName;

		CopyFileW( kFileInfo.m_strLocalFileName.c_str(), wstrDesFileName.c_str(), false );
	}
}
//}}

void KThreadStatisticsManager::AttachTimeTag( std::wostringstream& os )
{
	time_t t0 = time(0);
#ifdef _CONVERT_VS_2010
	tm t;
	localtime_s(&t, &t0);
	os
		<< L"_["
		<< 1900 + t.tm_year	<< L"_"
		<< t.tm_mon + 1		<< L"_"
		<< t.tm_mday		<< L"_"
		<< t.tm_hour		<< L"_"
		<< t.tm_min			<< L"]";
#else
	tm* t = localtime(&t0);
	os
		<< L"_["
		<< 1900 + t->tm_year	<< L"_"
		<< t->tm_mon + 1		<< L"_"
		<< t->tm_mday			<< L"_"
		<< t->tm_hour			<< L"_"
		<< t->tm_min			<< L"]";
#endif _CONVERT_VS_2010
}

void KThreadStatisticsManager::AttachSvnameTag( std::wostringstream& os )
{
	char strTemp[256] = {0};
	::gethostname( strTemp, 256 );

	HOSTENT* hostent;
	char* ip;

	hostent = ::gethostbyname( strTemp );
	ip = ::inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );

	os
		<< L"_[" << ip << L"]";
}

void KThreadStatisticsManager::CheckFlushLocalLog()
{
	//int iCurCheckTime = timeGetTime();
	//if ( iCurCheckTime - m_iLocalLogLastCheckTime < 1 )
	//{
	//	return;
	//}

	// 초기화
	if ( m_iLocalLogLastUpdateTime == 0 )
	{
		std::wfstream fin( "Statistics\\LastUpdateLog.txt", std::ios::in );
		if ( fin.good() )
		{
			fin >> m_iLocalLogLastUpdateTime;
		}
		else
		{
			m_iLocalLogLastUpdateTime = GetCurLocalTime();
		}
		fin.close();
	}
}

void KThreadStatisticsManager::UpdateFlushLocalLog()
{
	CheckFlushLocalLog();

	if ( m_iLocalLogLastUpdateTime == 0 )
		return;

	time_t t0 = time(0);
#ifdef _CONVERT_VS_2010
	tm t;
	localtime_s(&t, &t0);
#else
	tm* t = localtime(&t0);
#endif _CONVERT_VS_2010

	int iCurTime = GetCurLocalTime();

	// App 실행시, 한번만 실행
	static bool bAtOnce = false;
	if ( bAtOnce == false )
	{
		// 타입 갭이 1일 이상일 경우
		if ( iCurTime - m_iLocalLogLastUpdateTime >= 24 * 60 )
		{
			OnFlushLocalLog();

			ClearLocalLogData_();

			m_iLocalLogLastUpdateTime = iCurTime;

			bAtOnce = true;
		}
	}

	// 스케쥴러, 4시에 전송, 타입 갭이 2시간 이상일 경우
#ifdef _CONVERT_VS_2010
	if ( t.tm_hour == 4 && 
		iCurTime - m_iLocalLogLastUpdateTime >= 2 * 60 )
#else
	if ( t->tm_hour == 4 && 
		iCurTime - m_iLocalLogLastUpdateTime >= 2 * 60 )
#endif _CONVERT_VS_2010
	{
		OnFlushLocalLog();

		ClearLocalLogData_();

		m_iLocalLogLastUpdateTime = iCurTime;
	}
}

void KThreadStatisticsManager::OnFlushLocalLog( bool bClearLocalLog /*= false*/ )
{
	m_kStatisticsLocalLog.FlushLocalLogStm();

	// 백업은 기본적으로 무조건 한다. FTP전송은 조건부로 처리.
	BackUpAndSendFTPLocalLog( m_bSendToFTPStatistics );

	//{{ 2008. 7. 1  최육사	 통계 데이터 초기화 여부
	if( bClearLocalLog )
	{
		m_kStatisticsLocalLog.ClearLocalLog();
	}
	//}}
}

#endif SERV_STATISTICS_THREAD


//{{ 2011. 11. 21  김민성	통계 로그 수치
#ifdef SERV_STATISTICS_LOG_COUNT
void KThreadStatisticsManager::UpdateStatisticsLogCount( IN UidType iCount )
{
	CTime tCurr = CTime::GetCurrentTime();
	std::wstring strTime = tCurr.Format( _T("%Y-%m-%d %H:%M:%S") );

	m_mapStatisticsLogCount.insert( std::make_pair( strTime, iCount) );
}

void KThreadStatisticsManager::WriteStatisticsLogCount()
{
	if( m_tStatisticsLogCount.elapsed() > 3600 )
	{
		m_tStatisticsLogCount.restart();
		++m_iStatisticsFileNo;
		
		if( m_iStatisticsFileNo > 24 ) // 1일 치만 기록하자
		{
			return;
		}

		std::string strPath = "Statistics\\StatisticsCount";
		std::string strPath_txt = ".txt"; 
		std::string strPath_Mid = boost::str( boost::format( "%d" ) % m_iStatisticsFileNo );
		
		strPath += strPath_Mid;
		strPath += strPath_txt;

		std::wfstream fout;
		fout.imbue(std::locale("korean", LC_CTYPE));
		fout.open( strPath.c_str(), std::ios::out );

		std::map< std::wstring, UidType >::iterator mit = m_mapStatisticsLogCount.begin();
		if( mit == m_mapStatisticsLogCount.end() )
		{
			return;
		}
		for( ; mit != m_mapStatisticsLogCount.end() ; ++mit )
		{
			fout << mit->first << L"\t" << mit->second << L"\n";
		}

		fout.close();

		m_mapStatisticsLogCount.clear();
	}
}
#endif SERV_STATISTICS_LOG_COUNT
//}}