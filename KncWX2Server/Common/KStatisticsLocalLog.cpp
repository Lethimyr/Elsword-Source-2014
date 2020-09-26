#include "KStatisticsLocalLog.h"

#include "BaseServer.h"
#include "../common/KFolderTraverse.h"
#include "../GameServer/Inventory.h"
#include "../CenterServer/PvpRoom.h"

 #include <sys/stat.h>

KStatisticsLocalLog::KStatisticsLocalLog()
{
	InitLocalLogInfo();
}

KStatisticsLocalLog::~KStatisticsLocalLog()
{
	FlushLocalLogStm();
}

//{{ 2008. 10. 27  최육사	코드 통합
void KStatisticsLocalLog::InitLocalLogInfo()
{
	m_kLocalLog[LOG_PVP].Init(			KStatistics::SI_LOC_PVP_VICTORY_PER,	"SI_LOG_PVP_VICTORY_PER" );
	m_kLocalLog[LOG_PVP_ROOM].Init(		KStatistics::SI_LOC_PVP_ROOM,			"SI_LOG_PVP_ROOM" );
	m_kLocalLog[LOG_DUNGEON_ROOM].Init( KStatistics::SI_LOC_DUNGEON_ROOM,		"SI_LOG_DUNGEON_ROOM" );
	m_kLocalLog[LOG_TITLE].Init(		KStatistics::SI_LOC_TITLE,				"SI_LOG_TITLE" );
	//{{ 2009. 5. 3  최육사		던전로그 통합
	m_kLocalLog[LOG_DUNGEON].Init(		KStatistics::SI_LOC_DUNGEON,			"SI_LOG_DUNGEON" );
	//}}
	//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	m_kLocalLog[LOG_PET_SUMMON].Init(	KStatistics::SI_LOC_PET_SUMMON,			"SI_LOG_PET_SUMMON" );
#endif SERV_PET_SYSTEM
	//}}
	//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
	m_kLocalLog[LOG_SERV_DISCONNECT].Init( KStatistics::SI_LOC_SERV_DISCONNECT,	"SI_LOG_SERV_DISCONNECT" );
#endif SERV_SERVER_DISCONNECT_LOG
	//}}
	//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	m_kLocalLog[LOG_BILLING_PACKET].Init( KStatistics::SI_LOC_BILLING_PACKET,	"SI_LOG_BILLING_PACKET" );
#endif SERV_BILLING_PACKET_LOG
	//}}
	//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	m_kLocalLog[LOG_ABUSER_MORNITORING].Init( KStatistics::SI_LOC_ABUSER_MORNITORING,	"SI_LOG_ABUSER_MORNITORING" );
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
	//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	m_kLocalLog[LOG_UNDEFINED_QUEST_TEMPLET].Init( KStatistics::SI_LOC_UNDEFINED_QUEST_TEMPLET,	"SI_LOG_UNDEFINED_QUEST_TEMPLET" );
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

}

void KStatisticsLocalLog::InitLocalLog( KLocalLog eEnum )
{	
	if( m_kLocalLog[eEnum].m_wfsLocalLog.is_open() )
		return;

	std::string strFilename;
	strFilename  = "Statistics\\";
	strFilename += m_kLocalLog[eEnum].m_strLogFileName;
	strFilename += ".txt";

	struct _stat info;
	int result = _stat( strFilename.c_str(), &info );

	m_kLocalLog[eEnum].m_wfsLocalLog.imbue(std::locale("korean", LC_CTYPE));
	m_kLocalLog[eEnum].m_wfsLocalLog.open( strFilename.c_str(), std::ios::out | std::ios::app );	

	// 파일이 존재하지 않았을 경우 생성
	if ( result != 0 )
		WriteLocalLogHead( m_kLocalLog[eEnum].m_wfsLocalLog, m_kLocalLog[eEnum].m_iStatisticsID );
	m_kLocalLog[eEnum].m_wfsLocalLog.flush();
}

std::wfstream& KStatisticsLocalLog::LocalLogStm( KLocalLog eEnum )
{
	InitLocalLog( eEnum );

	return m_kLocalLog[eEnum].m_wfsLocalLog;
}
//}}

//{{ 2008. 10. 27  최육사	코드 통합
void KStatisticsLocalLog::CloseLocalLogData()
{	
	for( int iIdx = 0; iIdx < LOG_MAX; ++iIdx )
	{
		if( m_kLocalLog[iIdx].m_wfsLocalLog.is_open() )
			m_kLocalLog[iIdx].m_wfsLocalLog.close();
	}
}
//}}

//-----------------------------------------------------------------------------
void _DeleteFile(WIN32_FIND_DATA& wfd)
{
	if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		std::wstring strFilename = wfd.cFileName;
		
		// 던전
		if ( strFilename.find( L"SI_LOG_DUNGEON" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
		
		// 대전
		if ( strFilename.find( L"SI_LOG_PVP_VICTORY_PER" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}

		// 대전 방
		if ( strFilename.find( L"SI_LOG_PVP_ROOM" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
		
		// 던전 방
		if ( strFilename.find( L"SI_LOG_DUNGEON_ROOM" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}

		//{{ 2008. 10. 16  최육사	타이틀
		if ( strFilename.find( L"SI_LOG_TITLE" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
		//}}

		//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
		if ( strFilename.find( L"SI_LOG_PET_SUMMON" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
#endif SERV_PET_SYSTEM
		//}}

		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
		if ( strFilename.find( L"SI_LOG_SERV_DISCONNECT" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
#endif SERV_SERVER_DISCONNECT_LOG
		//}}

		//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
		if ( strFilename.find( L"SI_LOG_BILLING_PACKET" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
#endif SERV_BILLING_PACKET_LOG
		//}}

		//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
		if ( strFilename.find( L"SI_LOG_ABUSER_MORNITORING" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
		//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
		if ( strFilename.find( L"SI_LOG_UNDEFINED_QUEST_TEMPLET" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
#endif // SERV_LOG_UNDEFINED_QUEST_TEMPLET

	}
}

void _DeleteHenirRankingFile(WIN32_FIND_DATA& wfd)
{
	if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		std::wstring strFilename = wfd.cFileName;

		// 헤니르 시공 랭킹
		if ( strFilename.find( L"SI_LOG_HENIR_RANKING" ) != std::wstring::npos )
		{
			DeleteFile(KMakeAbsolutePath(wfd.cFileName).c_str());
		}
	}
}

void KStatisticsLocalLog::ClearLocalLog()
{
	CloseLocalLogData();

	// 일일 통계 파일 검색해서 모두 삭제
	KApplyAllFiles( _DeleteFile );

	KSIManager.ClearLocalLogData( KStatistics::SI_LOC_PVP );
	KSIManager.ClearLocalLogData( KStatistics::SI_LOC_ITEM );
	KSIManager.ClearLocalLogData( KStatistics::SI_LOC_SPIRIT );
	KSIManager.ClearLocalLogData( KStatistics::SI_LOC_ENCHANT );
	
	KSIManager.InitLocalLogKeys();
	
	//FlushLocalLogStm(); [참고] 의도가 파악되지 않은 코드
	//}}
}

void KStatisticsLocalLog::FlushLocalLogStm( int iStatisticsID, const char* strFileName )
{
	if ( KSIManager.CheckStatistics( iStatisticsID ) == true )
	{
		std::string strPath = "Statistics\\";
		strPath += strFileName;

		std::wfstream fout;
		fout.imbue(std::locale("korean", LC_CTYPE));
		fout.open( strPath.c_str(), std::ios::out );

		WriteLocalLogHead( fout, iStatisticsID );
		KSIManager.WriteLocalLogData( fout, iStatisticsID );

		fout.close();
	}
}

void KStatisticsLocalLog::FlushLocalLogStm()
{
	// PVP Log
	FlushLocalLogStm( KStatistics::SI_LOC_PVP, "SI_LOG_PVP.TXT" );

	// Item Log
	FlushLocalLogStm( KStatistics::SI_LOC_ITEM, "SI_LOG_ITEM.TXT" );
	
	// Spirit Log
	FlushLocalLogStm( KStatistics::SI_LOC_SPIRIT, "SI_LOG_SPIRIT.TXT" );

	//{{ 2008. 2. 24  최육사  강화 통계
	FlushLocalLogStm( KStatistics::SI_LOC_ENCHANT, "SI_LOG_ENCHANT.TXT" );
	//}}
}

void KStatisticsLocalLog::WriteLocalLogHead( std::wfstream& fout, int iStatisticsID )
{
	switch ( iStatisticsID )
	{
	case KStatistics::SI_LOC_DUNGEON:
		{
			fout
				<< L" DungeonID " << L"\t"				
				<< L" UnitUID   " << L"\t"
				<< L" NickName  " << L"\t"
				<< L" Diffculty " << L"\t"
				<< L"IsChallenge" << L"\t"
				<< L" UnitClass " << L"\t"
				<< L"   Clear   " << L"\t"
				<< L"StartMember" << L"\t"
				<< L" End Member" << L"\t"
				<< L"   LV      " << L"\t"
				<< L" PlayTime  " << L"\t"
				<< L"   EXP     " << L"\t"
				<< L"   ED      " << L"\t"
				<< L"   Combo   " << L"\t"
				<< L"Combo Score" << L"\t"
				<< L" Technical " << L"\t"
				<< L"Techn Score" << L"\t"
				<< L"   Time    " << L"\t"
				<< L"  Damaged  " << L"\t"
				<< L"Damaged Num" << L"\t"
				<< L"   Rank    " << L"\t"
				<< L" Continue  " << L"\t"
				<< L" PassStage " << L"\t"
				<< L" PassSubSt " << L"\t"
				//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
				<< L"IsWithPet  " << L"\t"
#endif SERV_PET_SYSTEM
				//}}
				;
		}
		break;
	case KStatistics::SI_LOC_PVP:
		{
			fout
				<< L"  Channel  " << L"\t"
				<< L"   Create  " << L"\t"
				<< L"  Elsword  " << L"\t"
				<< L"   AiSha   " << L"\t"
				<< L"   Rena    " << L"\t"
				<< L"   Raven   " << L"\t"				
				<< L"   Eve     " << L"\t"
				<< L"  SKnight  " << L"\t"
				<< L"  MKnight  " << L"\t"
				<< L"  CRanger  " << L"\t"
				<< L"  SRanger  " << L"\t"
				<< L" HMagician " << L"\t"
				<< L" DMagician " << L"\t"
				<< L" SoulTaker " << L"\t"
				<< L" OverTaker " << L"\t"				
				<< L"Exotic Gear" << L"\t"
				<< L"Architectur" << L"\t"
				<< L"  Intrude  " << L"\t"
				<< L"  GiveUp   " << L"\t"
				<< L"   VP      " << L"\t"
				<< L"   EXP     " << L"\t"
				<< L"  Team  MC " << L"\t"
				<< L"  Team  PT " << L"\t"
				<< L" TDeath MC " << L"\t"
				<< L" TDeath PT " << L"\t"
				<< L" SurviveMC " << L"\t"
				<< L" SurvivePT " << L"\t"				
				;
		}
		break;
	case KStatistics::SI_LOC_ITEM:
		{
			fout
				<< L"   ItemID  " << L"\t"
				<< L"   Trade   " << L"\t"
				//{{ 2010. 03. 31  최육사	거래게시판
				<< L"   PShop   " << L"\t"
				<< L"   TBoard  " << L"\t"
				//}}
				<< L"   Buy     " << L"\t"
				<< L"   Sell    " << L"\t"
				<< L"   Use     " << L"\t"
				<< L"   Make    " << L"\t"
				<< L"   Failed  " << L"\t"
				<< L"   Drop    " << L"\t"
				<< L"   Quest   " << L"\t"
				<< L"   Equip   " << L"\t"
				<< L"   Random  " << L"\t"
				<< L"   Resolve " << L"\t"
				<< L"  R_Result " << L"\t"
				<< L"   Mall    " << L"\t"
				<< L"   Post    " << L"\t"
				<< L"   Attrib  " << L"\t"
				<< L" Exchange S" << L"\t"
				<< L" Exchange R" << L"\t"
				//{{ 2010. 05. 20  최육사	아이템 버리기 통계
				<< L"   Discard " << L"\t"
				//}}
				//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
				<< L"  Pet Feed " << L"\t"
#endif SERV_PET_SYSTEM
				//}}
				//{{ 2011. 04. 26	최육사	대리 상인
#ifdef SERV_PSHOP_AGENCY
				<< L" Reg PShop " << L"\t"
				<< L" Buy PShop " << L"\t"
				<< L"PickUpPShop" << L"\t"
#endif SERV_PSHOP_AGENCY
				//}}
				;
		}
		break;
	case KStatistics::SI_LOC_SPIRIT:
		{
			fout
				<< L"   Level   " << L"\t"
				<< L"   Play    " << L"\t"
				<< L" UseSpirit " << L"\t"
				<< L" SpiriZero " << L"\t"
				;
		}
		break;
	case KStatistics::SI_LOC_ENCHANT:
		{
			fout
				<< L"   ItemID  " << L"\t"
				<< L"  Lv1 Succ " << L"\t"
				<< L"  Lv1 Fail " << L"\t"
				<< L"  Lv2 Succ " << L"\t"
				<< L"  Lv2 Fail " << L"\t"
				<< L"  Lv3 Succ " << L"\t"
				<< L"  Lv3 Fail " << L"\t"
				<< L"  Lv4 Succ " << L"\t"
				<< L"  Lv4 Fail " << L"\t"
				<< L"  Lv5 Succ " << L"\t"
				<< L"  Lv5 Fail " << L"\t"
				<< L"  Lv6 Succ " << L"\t"
				<< L"  Lv6 Fail " << L"\t"
				<< L"  Lv7 Succ " << L"\t"
				<< L"  Lv7 Fail " << L"\t"
				<< L"  Lv8 Succ " << L"\t"
				<< L"  Lv8 Fail " << L"\t"
				<< L"  Lv9 Succ " << L"\t"
				<< L"  Lv9 Fail " << L"\t"
				;
		}
		break;		
	case KStatistics::SI_LOC_PVP_VICTORY_PER:
		{
			fout
				<< L" PvpChannel" << L"\t"
				<< L"  UserUID  " << L"\t"
				<< L"  IP Addr  " << L"\t"
				<< L"  UnitUID  " << L"\t"
				<< L"  NickName " << L"\t"
				<< L" UnitClass " << L"\t"
				<< L"  RoomUID  " << L"\t"
				<< L"  PvP Type " << L"\t"
				//{{ 2010. 01. 06  최육사	아이템전
				<< L"  ItemGame " << L"\t"
				//}}
				<< L"  IsSecret " << L"\t"
				<< L" PlayerCnt " << L"\t"
				<< L"  Set Time " << L"\t"
				<< L"  End Time " << L"\t"
				<< L"  PlayTime " << L"\t"
				<< L"  GetVP    " << L"\t"
				<< L"  GetExp   " << L"\t"
				<< L"  Kill Cnt " << L"\t"
				<< L"  DeathCnt " << L"\t"
				<< L"  Result   " << L"\t"
				;
		}
		break;		
	case KStatistics::SI_LOC_PVP_ROOM:
		{
			fout
				<< L"  PvpType  " << L"\t"				
				<< L"  ItemGame " << L"\t"
				<< L"  MapID    " << L"\t"
				<< L"  Member   " << L"\t"				
				<< L"  PlayTime " << L"\t"
				<< L"  EndTime  " << L"\t"
				;
		}
		break;
	case KStatistics::SI_LOC_DUNGEON_ROOM:
		{
			fout
				<< L" DungeonID " << L"\t"
				<< L" Diffculty " << L"\t"
				<< L"IsChallenge" << L"\t"
				<< L"  Clear    " << L"\t"
				<< L"  Fail     " << L"\t"
				<< L"StartMember" << L"\t"
				<< L" End Member" << L"\t"
				<< L"  PlayTime " << L"\t"
				<< L"  EndTime  " << L"\t"
				;
		}
		break;
	case KStatistics::SI_LOC_HENIR_RANKING:
		{
			fout
				<< L"RankingType" << L"\t"
				<< L"  Index    " << L"\t"
				<< L"  Rank     " << L"\t"
				<< L" StageCount" << L"\t"
				<< L"PlayTime(s)" << L"\t"
				<< L"PlayTime(m)" << L"\t"
				<< L"  UserName " << L"\t"
				<< L"  UnitClass" << L"\t"
				<< L"  Level    " << L"\t"
				<< L"  RegDate  " << L"\t"
				;
		}
		break;		
	case KStatistics::SI_LOC_TITLE:
		{
			fout
				<< L"  Title ID " << L"\t"
				<< L"  NickName " << L"\t"
				<< L"  Char Lv  " << L"\t"
				;
		}
		break;
		//{{ 2010. 9. 14	최육사	펫 시스템
#ifdef SERV_PET_SYSTEM
	case KStatistics::SI_LOC_PET_SUMMON:
		{
			fout
				<< L"  NickName " << L"\t"
				<< L"    Level  " << L"\t"
				<< L"  Pet UID  " << L"\t"
				<< L"  Pet ID   " << L"\t"
				<< L" Evol Step " << L"\t"
				<< L"  Intimacy " << L"\t"
				<< L"Extroversio" << L"\t"
				<< L"  Emotion  " << L"\t"
				<< L"  RegDate  " << L"\t"
				;
		}
		break;
#endif SERV_PET_SYSTEM
		//}}
		//{{ 2010. 10. 11	최육사	서버간 접속 끊김 로그
#ifdef SERV_SERVER_DISCONNECT_LOG
	case KStatistics::SI_LOC_SERV_DISCONNECT:
		{
			fout
				<< L"  LogType  " << L"\t"
				<< L"  Source   " << L"\t"
				<< L"Destination" << L"\t"
				<< L"    ip     " << L"\t"
				<< L"  RegDate  " << L"\t"
				<< L"  Reason   " << L"\t"
				;
		}
		break;
#endif SERV_SERVER_DISCONNECT_LOG
		//}}
		//{{ 2010. 10. 11	최육사	넥슨 빌링 패킷 로그
#ifdef SERV_BILLING_PACKET_LOG
	case KStatistics::SI_LOC_BILLING_PACKET:
		{
			fout
				<< L"  LogType  " << L"\t"
				<< L"  UserUID  " << L"\t"
				<< L"  OrderNo  " << L"\t"
				<< L"  RegDate  " << L"\t"
				<< L"Packet Name" << L"\t"
				;
		}
		break;
#endif SERV_BILLING_PACKET_LOG
		//}}
		//{{ 2010. 11. 17	최육사	오토핵 검증 기능
#ifdef SERV_AUTO_HACK_CHECK_GET_ITEM
	case KStatistics::SI_LOC_ABUSER_MORNITORING:
		{
			fout
				<< L"  UserUID  " << L"\t"
				<< L"  UnitUID  " << L"\t"
				<< L"   ED      " << L"\t"
				<< L"Item Count " << L"\t"
				//{{ 2013. 05. 29	최육사	패킷 모니터링 근성도 항목 추가
#ifdef SERV_PACKET_MORNITORING_SPIRIT_COL
				<< L"  Spirit   " << L"\t"
#endif SERV_PACKET_MORNITORING_SPIRIT_COL
				//}}
				<< L"  RegDate  " << L"\t"
				<< L"Packet Name" << L"\t"
				;
		}
		break;
#endif SERV_AUTO_HACK_CHECK_GET_ITEM
		//}}
#ifdef SERV_LOG_UNDEFINED_QUEST_TEMPLET
	case KStatistics::SI_LOC_UNDEFINED_QUEST_TEMPLET:
		{
			fout
				<< L"  QuesitID  " << L"\t"
				<< L"  RegDate  " << L"\t"
				;
		}
		break;
#endif SERV_LOG_UNDEFINED_QUEST_TEMPLET
	}
}

//{{ 2008. 5. 20  최육사  일일 통계
void KStatisticsLocalLog::LoadLocalLogFromFile( int iStatisticsID, const char* strFileName, unsigned int uiLastIndex )
{
	if ( KSIManager.CheckStatistics( iStatisticsID ) == true )
	{
		std::string strPath = "Statistics\\";
		strPath += strFileName;

		std::wfstream fin;
		fin.imbue(std::locale("korean"));
		fin.open( strPath.c_str(), std::ios::in );

		WCHAR pcBuf[MAX_PATH] = { 0 };

		// Load Head
		fin.getline( pcBuf, MAX_PATH );

		// Load Data
		while ( true )
		{
			if ( fin.eof() == true )
				break;

			fin.getline( pcBuf, MAX_PATH );
			std::wistringstream is( pcBuf );

			int iKeyID = 0;
			is >> iKeyID;

			if ( is.good() == false )
				break;

			KStatisticsKey kKey;
			kKey.m_vecIntKey.push_back( iKeyID );

			for ( unsigned int iD = 0; iD < uiLastIndex; ++iD )
			{
				int iData = 0;
				is >> iData;

				if ( is.good() == false )
					break;

				KSIManager.IncreaseCount( iStatisticsID, kKey, iD, iData );
			}
		}

		fin.close();
	}
}
//}}

void KStatisticsLocalLog::LoadLocalLogFromFile()
{
	// Once Call
	static bool sb_FuncPassed = false;
	if ( sb_FuncPassed == true ) return;
	sb_FuncPassed = true;

	// PVP Log
	LoadLocalLogFromFile( KStatistics::SI_LOC_PVP, "SI_LOG_PVP.TXT", KPvPRoom::SP_NUM );

	// Item Log
	LoadLocalLogFromFile( KStatistics::SI_LOC_ITEM, "SI_LOG_ITEM.TXT", KStatistics::SI_ITEM_MAX );
	
	// Spirit Log
	LoadLocalLogFromFile( KStatistics::SI_LOC_SPIRIT, "SI_LOG_SPIRIT.TXT", KStatistics::eSIColSpirit_Max );
	
	// Enchant Log
	LoadLocalLogFromFile( KStatistics::SI_LOC_ENCHANT, "SI_LOG_ENCHANT.TXT", KStatistics::eSIColEnchant_Max );
}

void KStatisticsLocalLog::InitLocalLog_Henir_Ranking( int iRankingType )
{
	_JIF( ( SEnum::IsHenirRankingType( iRankingType ) == true ), return; );

	// 헤니르 시공 랭킹
	if( m_wfsLocalLog_Henir_Ranking[iRankingType].is_open() )
		return;

	CStringA cstrFileName;
	cstrFileName.Format( "Statistics\\SI_LOG_HENIR_RANKING_RANKING_TYPE_%d.txt", iRankingType );

	std::string strFilename;
	strFilename = cstrFileName.GetBuffer();

	struct _stat info;
	int result = _stat( strFilename.c_str(), &info );

	m_wfsLocalLog_Henir_Ranking[iRankingType].imbue(std::locale("korean", LC_CTYPE));
	m_wfsLocalLog_Henir_Ranking[iRankingType].open( strFilename.c_str(), std::ios::out | std::ios::app );

	// 파일이 존재하지 않았을 경우 생성
	if ( result != 0 )
		WriteLocalLogHead( m_wfsLocalLog_Henir_Ranking[iRankingType], KStatistics::SI_LOC_HENIR_RANKING );

	m_wfsLocalLog_Henir_Ranking[iRankingType].flush();
}

std::wfstream& KStatisticsLocalLog::LocalLogStm_Henir_Ranking( int iRankingType )
{
	_JIF( ( SEnum::IsHenirRankingType( iRankingType ) == true ), return m_wfsLocalLog_Henir_Ranking[SEnum::RT_DAY_RANKING]; );

	InitLocalLog_Henir_Ranking( iRankingType );

	return m_wfsLocalLog_Henir_Ranking[iRankingType];
}

void KStatisticsLocalLog::CloseHenirRankingLogData( int iRankingType )
{
	_JIF( ( SEnum::IsHenirRankingType( iRankingType ) == true ), return; );

	// 아케이드 랭킹
	if( m_wfsLocalLog_Henir_Ranking[iRankingType].is_open() )
		m_wfsLocalLog_Henir_Ranking[iRankingType].close();
}

void KStatisticsLocalLog::DeleteHenirRankingLog()
{
	KApplyAllFiles( _DeleteHenirRankingFile );
}


