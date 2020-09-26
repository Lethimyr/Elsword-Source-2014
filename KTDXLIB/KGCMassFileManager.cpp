// KGCMassFileManager.cpp: implementation of the KGCMassFileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "zlib.h"
#include "KGCMassFileBufferManager.h"
#include "KGCMassFileManager.h"
#include <libxml/parser.h>
#include <libxml/tree.h>


// 2008.09.22 dmlee
// LoadMassFile() 함수에서 .kom 파일열기 실패하면 3초 정도 기다렸다가 다시 파일 열기를 2번 정도 더 해본다
// 파일 열기가 실패하는 원인은 "다른 프로세스가 파일을 사용 중이기 때문에 프로세스가 액세스 할 수 없습니다."라는데... 잘 모르겠음.
// 이런 방법으로 해서 파일 열기 실패에 의한 크래시의 대부분을 해결했음. ^^

// 2008.09.16 dmlee
// 보다 상세한 에러로그를 남기기 위해서 fopen, fread, fclose를 CreateFile, ReadFile, CloseHandle로 교체

// 2008.05.27 dmlee
// buffer 여러개 사용하도록 고치면서 memory_file 코드 부분 comment out


//#ifndef MASS_FILE_FIRST
//#define MASS_FILE_FIRST
//#endif// MASS_FILE_FIRST

//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//__declspec(thread)  /*static*/  
//	KGCMassFileManager::KGCMASSFILEMANAGER_OVERLAPPED   KGCMassFileManager::m_overlapped;           
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
__declspec(thread)
	KGCMassFileManager::KTLSData*  KGCMassFileManager::ms_pkTLSData = NULL;
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
/*static*/ KGCMassFileBufferManager         KGCMassFileManager::m_massFileBufMan;
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16


//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
/*static*/
const KGCMassFileManager::KTLSData&	KGCMassFileManager::GetUpdatedTLSData()
{
	if ( ms_pkTLSData == NULL )
		ms_pkTLSData = new KTLSData;
	KTLSData& kTLSData = *ms_pkTLSData;
	if ( m_dwDataDirectoryStamp != kTLSData.m_dwDataDirectoryStamp )
	{
		CSLock locker( m_csStrDataDirectory );
		kTLSData.m_dwDataDirectoryStamp = m_dwDataDirectoryStamp;
		kTLSData.m_vecstrDataDirectory = m_vecstrDataDirectory;
	}
	return	kTLSData;
}
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK



//#ifndef BACKGROUND_LOADING_TEST // 2008-10-17
////{{AFX
//KGCMassFileManager::KGCBufferManager KGCMassFileManager::m_BufferManager;
////}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

KGCMassFileManager::KGCMassFileManager()
{
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	InitServerCurrentTime();
#endif SERV_MASSFILE_MAPPING_FUNCTION

//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
	m_bLockMassFileMap = false;
	m_dwDataDirectoryStamp = 0;
//#else	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	::InitializeCriticalSection( &m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	ZeroMemory( (void*) &m_adwThreadId[0], sizeof(m_adwThreadId) );
	RegisterCurrentThread();
//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//	m_dwForegroundThreadId = 0;
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

}//KGCMassFileManager::KGCMassFileManager()


KGCMassFileManager::~KGCMassFileManager()
{
//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	::EnterCriticalSection( &m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK	

	for(int i = 0; i < (int)m_vecMassFile.size(); i++)
	{
		delete m_vecMassFile[i];
	}
	m_vecMassFile.clear();
	m_mapMassFile.clear();

#ifdef X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE
	std::map< std::string, CMassFile::MASSFILE_MEMBERFILEINFO_POINTER >::iterator it;
	std::map< std::string, CMassFile::MASSFILE_MEMBERFILEINFO_POINTER >::iterator iend = m_mapNewFolderFile.end();
	for( it = m_mapNewFolderFile.begin(); it != iend; ++it )
	{
		delete it->second;
	}
	m_mapNewFolderFile.clear();
#endif//X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE

//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
	{
		CSLock	locker( m_csStrDataDirectory );
		m_dwDataDirectoryStamp = 0;
		m_vecstrDataDirectory.clear();
	}
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	::LeaveCriticalSection( &m_csFileManager );
//
//	::DeleteCriticalSection( &m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
}//KGCMassFileManager::~KGCMassFileManager()


//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
bool	KGCMassFileManager::RegisterCurrentThread()
{
	DWORD	dwCurrentThreadId = ::GetCurrentThreadId();
	for( int i = 0; i < MASS_FILE_MANAGER_MAX_NUM_THREADS; i++ )
	{
		DWORD	dwOldValue = ::InterlockedCompareExchange( &m_adwThreadId[i], dwCurrentThreadId, 0 );
		if ( dwOldValue == 0 )
			return true;
	}
	return false;
}//KGCMassFileManager::RegisterCurrentThread()

inline int		KGCMassFileManager::GetCurrentThreadIndex()
{
	DWORD	dwCurrentThreadId = ::GetCurrentThreadId();
	for( int i = 0; i < MASS_FILE_MANAGER_MAX_NUM_THREADS; i++ )
	{
		if ( m_adwThreadId[i] == dwCurrentThreadId )
			return i;
	}
	return -1;
}
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO



int  KGCMassFileManager::GetTotalFileCount()
{
//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

	return (int)m_mapMassFile.size();
}//KGCMassFileManager::GetTotalFileCount()

#ifdef WCHAR_DIR
void KGCMassFileManager::AddDataDirectory(const std::string& strDirectory, bool bInsertFront /* = false */ )
{
	std::wstring wstrDirectory;
	ConvertCharToWCHAR( wstrDirectory, strDirectory );
	AddDataDirectory( wstrDirectory );
}
#else WCHAR_DIR
void KGCMassFileManager::AddDataDirectory(const std::wstring& wstrDirectory, bool bInsertFront /* = false */ )
{
	char charBuf[512] = "";
	::WideCharToMultiByte( CP_ACP, 0, wstrDirectory.c_str(), -1, charBuf, 512, NULL, NULL );

	std::string strDirectory = charBuf;

	AddDataDirectory(strDirectory);
}
#endif WCHAR_DIR

#ifdef MASSFILE_MAPPING_FUNCTION //SERV_MASSFILE_MAPPING_FUNCTION
void KGCMassFileManager::InitServerCurrentTime()
{
	// 서버와 연결이 되기 전에도 쓰기 위해 일단 클라이언트 시간으로 초기화
	CTime tCurrent = CTime::GetCurrentTime();
	std::wstring wstrCurrentTime = ( CStringW )tCurrent.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	KncUtil::ConvertStringToCTime( wstrCurrentTime, m_ServerCurrentTime );
	m_TickCountAtGetTime = ::GetTickCount();
}

__time64_t KGCMassFileManager::GetServerCurrentTime()
{
	DWORD diffTickCount = ::GetTickCount() - m_TickCountAtGetTime;
	return m_ServerCurrentTime.GetTime() + (diffTickCount/1000);
}

void KGCMassFileManager::SetServerCurrentTime( wstring wstrTime )
{
	KncUtil::ConvertStringToCTime( wstrTime, m_ServerCurrentTime );
	m_TickCountAtGetTime = ::GetTickCount();
}
#endif MASSFILE_MAPPING_FUNCTION //SERV_MASSFILE_MAPPING_FUNCTION

#ifdef MASSFILE_MAPPING_FUNCTION
bool KGCMassFileManager::AddMappingList_LUA(int nIndex, const char* szDestMappingFileName, const char* szSrcMappingFileName)
{
	std::map<int, std::map<string, string>>::iterator mit = m_mapMassFileMappingList.find(nIndex);

	if(mit != m_mapMassFileMappingList.end())
	{
		mit->second.insert(std::make_pair(szDestMappingFileName, szSrcMappingFileName));
	}
	else
	{
		std::map<string, string> mapMappingList;
		mapMappingList.insert(std::make_pair(szDestMappingFileName, szSrcMappingFileName));
		m_mapMassFileMappingList.insert(std::make_pair(nIndex, mapMappingList));
	}

	return true;
}

bool KGCMassFileManager::AddMappingTime_LUA(int nIndex, const char* szStartDate, const char* szEndDate)
{
	MASSFILE_MAPPING_TIME stMappingTime;
	std::wstring wstrStartDate, wstrEndDate;

	ConvertCharToWCHAR(wstrStartDate, szStartDate);
	ConvertCharToWCHAR(wstrEndDate, szEndDate);

	CTime tStartTime, tEndTime;
	KncUtil::ConvertStringToCTime( const_cast< std::wstring & >( wstrStartDate ), tStartTime );
	KncUtil::ConvertStringToCTime( const_cast< std::wstring & >( wstrEndDate ), tEndTime );

	stMappingTime.tStartTime = tStartTime;
	stMappingTime.tEndTime = tEndTime;

	m_mapMassFileMappingTime.insert(std::make_pair(nIndex, stMappingTime));

	return true;
}

bool KGCMassFileManager::MassFileMapping()
{
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	// 예전 매핑이 있다면 되돌림
#ifdef WCHAR_DIR
	std::map< std::wstring, SMassFileIndex >::iterator mitMassFileBackup;
#else WCHAR_DIR
	std::map< std::string, SMassFileIndex >::iterator mitMassFileBackup;
#endif WCHAR_DIR

	for( mitMassFileBackup = m_mapMassFileBackup.begin(); mitMassFileBackup != m_mapMassFileBackup.end(); ++mitMassFileBackup )
	{
		m_mapMassFile[ mitMassFileBackup->first ] = mitMassFileBackup->second;
	}
	m_mapMassFileBackup.clear();

	CTime tCurrentTime( GetServerCurrentTime() );
#else SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrentTime = CTime::GetCurrentTime();
#endif SERV_MASSFILE_MAPPING_FUNCTION

	for(std::map<int, MASSFILE_MAPPING_TIME>::iterator mit = m_mapMassFileMappingTime.begin(); mit != m_mapMassFileMappingTime.end(); ++mit)
	{
		if(mit->second.tStartTime <= tCurrentTime && tCurrentTime <= mit->second.tEndTime)
		{
			int nMappingIndex = mit->first;

			std::map<int, std::map<string, string>>::iterator mit = m_mapMassFileMappingList.find(nMappingIndex);

			if(mit != m_mapMassFileMappingList.end())
			{
				std::map<string, string>::iterator mitFileNameMapping = mit->second.begin();

				for(mitFileNameMapping; mitFileNameMapping != mit->second.end(); ++mitFileNameMapping)
				{
#ifdef WCHAR_DIR
					std::wstring strVirtualFileName;
					std::wstring strRealFileName;
					ConvertCharToWCHAR( strVirtualFileName, mitFileNameMapping->first );
					ConvertCharToWCHAR( strRealFileName, mitFileNameMapping->second );
#else WCHAR_DIR
					std::string strVirtualFileName = mitFileNameMapping->first;
					std::string strRealFileName = mitFileNameMapping->second;
#endif WCHAR_DIR
					MakeUpperCase(strVirtualFileName);
					MakeUpperCase(strRealFileName);

					if(m_mapMassFile.find(strVirtualFileName) != m_mapMassFile.end() && m_mapMassFile.find(strRealFileName) != m_mapMassFile.end())
					{
						//CSLock locker( m_csFileManager );
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
						m_mapMassFileBackup[strVirtualFileName] = m_mapMassFile[strVirtualFileName];
#endif SERV_MASSFILE_MAPPING_FUNCTION
						m_mapMassFile[strVirtualFileName] = m_mapMassFile[strRealFileName];
					}
				}
			}
		}
	}

	return true;
}

#ifdef ALWAYS_MAPPING_MUSIC
std::string  KGCMassFileManager::MappingFileMusic(std::string inName)
{
	CTime tCurrentTime( GetServerCurrentTime() );
	for(std::map<int, MASSFILE_MAPPING_TIME>::iterator mit = m_mapMassFileMappingTime.begin(); mit != m_mapMassFileMappingTime.end(); ++mit)
	{
		if(mit->second.tStartTime <= tCurrentTime && tCurrentTime <= mit->second.tEndTime)
		{
			int nMappingIndex = mit->first;

			std::map<int, std::map<string, string>>::iterator TempMit = m_mapMassFileMappingList.find(nMappingIndex);

			if(TempMit != m_mapMassFileMappingList.end())
			{
				///여기서 대문자로 컨버팅해서 들어간다.
				std::string TempCompareString = inName;
				MakeUpperCase(TempCompareString);
				std::map<string, string>::iterator mitFileNameMapping;
				for( mitFileNameMapping = TempMit->second.begin(); mitFileNameMapping != TempMit->second.end(); ++mitFileNameMapping)
				{
					std::string Temp = mitFileNameMapping->first;
					MakeUpperCase(Temp);
					if(TempCompareString.compare(Temp) == 0)
					{
						std::string returnString = mitFileNameMapping->second;
						return returnString;
					}
				}
			}
		}
	}
	return inName;
}
#endif ALWAYS_MAPPING_MUSIC

#ifdef WCHAR_DIR
bool KGCMassFileManager::ExternFileMapping( wstring& strExternFileName )
#else WCHAR_DIR
bool KGCMassFileManager::ExternFileMapping( string& strExternFileName )
#endif WCHAR_DIR
{
#ifdef SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrentTime( GetServerCurrentTime() );
#else SERV_MASSFILE_MAPPING_FUNCTION
	CTime tCurrentTime = CTime::GetCurrentTime();
#endif SERV_MASSFILE_MAPPING_FUNCTION

	for(std::map<int, MASSFILE_MAPPING_TIME>::iterator mit = m_mapMassFileMappingTime.begin(); mit != m_mapMassFileMappingTime.end(); ++mit)
	{
		if(mit->second.tStartTime <= tCurrentTime && tCurrentTime <= mit->second.tEndTime)
		{
			int nMappingIndex = mit->first;

			std::map<int, std::map<string, string>>::iterator mit = m_mapMassFileMappingList.find(nMappingIndex);

			if(mit != m_mapMassFileMappingList.end())
			{
				std::map<string, string>::iterator mitFileNameMapping = mit->second.begin();

				for(mitFileNameMapping; mitFileNameMapping != mit->second.end(); ++mitFileNameMapping)
				{
#ifdef WCHAR_DIR
					std::wstring strVirtualFileName;
					std::wstring strRealFileName;
					ConvertCharToWCHAR( strVirtualFileName, mitFileNameMapping->first );
					ConvertCharToWCHAR( strRealFileName, mitFileNameMapping->second );
#else WCHAR_DIR
					std::string strVirtualFileName = mitFileNameMapping->first;
					std::string strRealFileName = mitFileNameMapping->second;
#endif WCHAR_DIR
					MakeUpperCase(strVirtualFileName);
					MakeUpperCase(strRealFileName);

					if(0 == strExternFileName.compare(strVirtualFileName))
					{
						strExternFileName = strRealFileName;

						return true;
					}
				}
			}
		}
	}

	return false;
}
#endif MASSFILE_MAPPING_FUNCTION

#ifdef WCHAR_DIR
void KGCMassFileManager::AddDataDirectory( const std::wstring& strDirectory, bool bInsertFront /*= false*/ )
#else WCHAR_DIR
void KGCMassFileManager::AddDataDirectory( const std::string& strDirectory, bool bInsertFront /*= false*/ )
#endif WCHAR_DIR
{
//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
	CSLock locker( m_csStrDataDirectory );
//#else	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

#ifdef WCHAR_DIR
	std::vector< std::wstring >::iterator vit;
#else WCHAR_DIR
	std::vector< std::string >::iterator vit;
#endif WCHAR_DIR
	vit = std::find(m_vecstrDataDirectory.begin(), m_vecstrDataDirectory.end(), strDirectory);
    
	if( vit == m_vecstrDataDirectory.end() )
	{
		if( true == bInsertFront )
		{
			m_vecstrDataDirectory.insert( m_vecstrDataDirectory.begin(), strDirectory );
		}
		else
		{
			m_vecstrDataDirectory.push_back( strDirectory );
		}
		m_dwDataDirectoryStamp++;
		if ( m_dwDataDirectoryStamp == 0 )
			m_dwDataDirectoryStamp = 1;
	}
}//KGCMassFileManager::AddDataDirectory()

//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
void		KGCMassFileManager::LockMassFileMap()
{
	m_bLockMassFileMap = true;
}
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

#ifdef NEW_MAIL_LOG
#ifdef WCHAR_DIR
KGCMassFileManager::CMassFile* KGCMassFileManager::AddMassFile(const std::wstring& strKomFileName, OUT bool& bIsDuplicationError_)
#else WCHAR_DIR
KGCMassFileManager::CMassFile* KGCMassFileManager::AddMassFile(const std::string& strKomFileName, OUT bool& bIsDuplicationError_)
#endif WCHAR_DIR
#else
#ifdef WCHAR_DIR
KGCMassFileManager::CMassFile* KGCMassFileManager::AddMassFile(const std::wstring& strKomFileName)
#else WCHAR_DIR
KGCMassFileManager::CMassFile* KGCMassFileManager::AddMassFile(const std::string& strKomFileName)
#endif WCHAR_DIR
#endif // NEW_MAIL_LOG
{
//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
	if ( m_bLockMassFileMap == true )
	{
		ErrorLogMsg( KEM_ERROR426, strKomFileName.c_str() );
		return NULL;
	}
//#else	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

	CMassFile* pMassFile = new CMassFile();
	if( !pMassFile->LoadMassFile(strKomFileName.c_str()) )
	{
		delete pMassFile;
		ErrorLogMsg( KEM_ERROR132, strKomFileName.c_str() );
		return NULL;
	}

	m_vecMassFile.push_back(pMassFile);

	CMassFile::MASSFILE_MEMBERFILEINFO Info;
	Info.dwFlag = 0;
	SMassFileIndex sFileIndex;
	//Real Data는 받아오지 않음
	//ZeroMemory(&Info,sizeof(Info));

	/*
	sFileIndex.pMassFile = pMassFile;
	UINT uiNumOfFile = pMassFile->GetFileCount();		// MassFile이 가지고 있는 파일의 갯수
	const char* pSzMemberFileName = NULL;
	for ( UINT i = 0; i < uiNumOfFile; ++i )
	{
		sFileIndex.Index = i;
		 
		pSzMemberFileName = pMassFile->GetMemberFileName( i );
		m_mapMassFile[pSzMemberFileName] = sFileIndex;
	}
	//*/

	//*
#if defined(_IN_HOUSE_) || defined(_IN_HOUSE_SERVICE_READY_QA_)
	FILE *fp = fopen("existfileinfo.txt", "a");
#endif
	for(int i = 0; i < (int)pMassFile->GetFileCount(); i++)
	{
		pMassFile->GetMemberFile( *this, i, &Info );
		sFileIndex.Index = i;
		sFileIndex.pMassFile = pMassFile;

		MassFileMap::_Pairib ib = m_mapMassFile.insert( MassFileMap::value_type( Info.strFileName, sFileIndex ) );

		if( ib.second == false )
		{
#ifdef INT_KOM_DUPLICATE		
			MassFileMap::const_iterator iter = m_mapMassFile.find( Info.strFileName );
			if( 0 == strcmp( iter->second.pMassFile->m_strMassFileName, L"./dataINT.kom" ))
			{
				break;
			}
#endif //INT_KOM_DUPLICATE			

#ifdef NEW_MAIL_LOG
			bIsDuplicationError_ = true;
#endif // NEW_MAIL_LOG

 #if defined(_IN_HOUSE_) || defined(_IN_HOUSE_SERVICE_READY_QA_)
 			// 중북파일 리스트
 			static int nDuplicateFile = 0;
#ifdef WCHAR_DIR
			if( fp != NULL )
				fwprintf_s( fp, L"%s\n", Info.strFileName );

			wstring strInfo = L"중복파일입니다.";
			++nDuplicateFile;
			if( nDuplicateFile >= 5 )
				strInfo = L"포맷중....";
			else if( nDuplicateFile >= 2 )
				strInfo = L"바이러스 검출!.";
			MessageBoxW(NULL, Info.strFileName, strInfo.c_str(), MB_OK);
#else WCHAR_DIR
			if( fp != NULL )
 			fprintf( fp, "%s\n", Info.strFileName );
 
 			string strInfo = "중복파일입니다.";
 			++nDuplicateFile;
 			if( nDuplicateFile >= 5 )
 				strInfo = "포맷중....";
 			else if( nDuplicateFile >= 2 )
 				strInfo = "바이러스 검출!.";
 			MessageBoxA(NULL, Info.strFileName, strInfo.c_str(), MB_OK);
#endif WCHAR_DIR
 #endif
 
 #ifdef NOT_ALLOW_DUPLICATE_FILES
			delete pMassFile;

#ifdef WCHAR_DIR
			wstring strErrLog = strKomFileName;
			strErrLog += L"(";
			strErrLog += Info.strFileName;
			strErrLog += L") is duplicated";
			ErrorLogMsg( KEM_ERROR132, strErrLog.c_str() );

			MessageBoxW( NULL, strErrLog.c_str(), strErrLog.c_str(), MB_OK );
#else WCHAR_DIR
			string strErrLog = strKomFileName;
			strErrLog += "(";
			strErrLog += Info.strFileName;
			strErrLog += ") is duplicated";
			ErrorLogMsg( KEM_ERROR132, strErrLog.c_str() );

			MessageBoxA(NULL, strErrLog.c_str(), strErrLog.c_str(), MB_OK);
#endif WCHAR_DIR
#if defined(_IN_HOUSE_) || defined(_IN_HOUSE_SERVICE_READY_QA_)
			if( fp != NULL )
				fclose(fp);
#endif
			return NULL;
#endif
 		}

	}
#if defined(_IN_HOUSE_) || defined(_IN_HOUSE_SERVICE_READY_QA_)
	fclose(fp);
#endif

	//*/
	return pMassFile;
}//KGCMassFileManager::AddMassFile()

#ifdef X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE
void KGCMassFileManager::AddNewFolderFile( const char* szSearchDir )
{
	HANDLE				hSearch;
	WIN32_FIND_DATAA	fd;
	char				szSearchPath[256] = {0,};

#ifdef	CONVERSION_VS
	strncpy_s(szSearchPath, _countof(szSearchPath), szSearchDir , 255);
#else	CONVERSION_VS
	strncpy(szSearchPath, szSearchDir , 255);
#endif	CONVERSION_VS

	//strcpy(szSearchPath, szSearchDir);

	StringCchCatA(szSearchPath, 256, "\\*.*");
	//strcat(szSearchPath, "\\*.*");

	hSearch = FindFirstFileA(szSearchPath, &fd);

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if( 0 != strcmp(fd.cFileName, ".") && 0 != strcmp(fd.cFileName, "..") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{			
				char szSearchPath2[256] = {0,};
				strncpy_s(szSearchPath2, _countof(szSearchPath2), szSearchDir , 255);
				StringCchCatA(szSearchPath2, 256, "\\");
				StringCchCatA(szSearchPath2, 256, fd.cFileName);

				AddNewFolderFile( szSearchPath2 );
			}
			else
			{
				std::string strFileName = fd.cFileName;
				MakeUpperCase( strFileName );

				CMassFile::MASSFILE_MEMBERFILEINFO_POINTER kRealFileInfo;
				LoadRealFile( strFileName, &kRealFileInfo );

				m_mapNewFolderFile.insert( std::make_pair( strFileName, kRealFileInfo ) );
			}
		}
	}while(FindNextFileA(hSearch, &fd));

	FindClose(hSearch);
}
#endif//X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE

bool KGCMassFileManager::IsValidFile( std::wstring wstrFileName )
{
	if ( wstrFileName.empty() == true )
		return false;

#ifdef WCHAR_DIR
	WCHAR strFileName[255] = { 0,};
#else WCHAR_DIR
	char strFileName[255] = { 0,};
#endif WCHAR_DIR

    CMassFile::MASSFILE_MEMBERFILEINFO  mFileInfo;
	mFileInfo.dwFlag = 0;
	MakeUpperCase( wstrFileName );
#ifdef WCHAR_DIR
	StrCpyW( strFileName, wstrFileName.c_str() );
#else WCHAR_DIR
	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, 255, NULL, NULL );
#endif WCHAR_DIR

#ifdef MASS_FILE_FIRST

#ifdef X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE
	if( m_mapNewFolderFile.find( strFileName ) != m_mapNewFolderFile.end() )
		return true;
#endif//X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE

	if( m_mapMassFile.find(strFileName) == m_mapMassFile.end() )
	{	
#if !defined( _SERVICE_ ) //&& !defined( _VTUNE_ )
		if( LoadRealFile( strFileName, &mFileInfo ) == true )
			return true;
		else
#endif//_SERVICE_
		{
			//ErrorLogMsg( KEM_ERROR133, wstrFileName.c_str() );
			return false;
		}
	}
	else
	{	
		return true;
	}
#else
	if( LoadRealFile( strFileName, &mFileInfo ) == true )
		return true;

	else if( m_mapMassFile.find(strFileName) != m_mapMassFile.end() )
	{
		return true;
	}
	else
	{
		//ErrorLogMsg( KEM_ERROR134, wstrFileName.c_str() );
		return false;
	}
#endif MASS_FILE_FIRST
}//KGCMassFileManager::IsValidFile()


//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
bool KGCMassFileManager::IsValidFile_LocalFile( std::wstring wstrFileName )
//#else   X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//bool KGCMassFileManager::IsValidFile( std::wstring wstrFileName )
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
{
	if ( wstrFileName.empty() == true )
		return false;
//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

	char strFileName[255] = {0,};

    CMassFile::MASSFILE_MEMBERFILEINFO  mFileInfo;
	mFileInfo.dwFlag = 0;
	MakeUpperCase( wstrFileName );
	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, 255, NULL, NULL );

#ifdef WCHAR_DIR
	if( LoadRealFile( wstrFileName, &mFileInfo ) == true )
		return true;
#else
	if( LoadRealFile( strFileName, &mFileInfo ) == true )
		return true;
#endif WCHAR_DIR

    return false;

}//KGCMassFileManager::IsValidFile()


#ifdef  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
        // 두 개의 map 상 혹은 물리 위치상 adjacent 한 파일 데이타 로딩을 위한 인터페이스
        // 텍스쳐파일과 .TET 파일 로딩을 위해 사용한다.
KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR
KGCMassFileManager::LoadTwoDataFiles( std::string strFileName, std::string strFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO_PTR& info2, 
            bool bRealData )
{
    CMassFile::MASSFILE_MEMBERFILEINFO_POINTER retInfo;
    info2 = CMassFile::MASSFILE_MEMBERFILEINFO_POINTER();

	ASSERT( false == strFileName.empty() );
	if ( strFileName.empty() == true )
		return	retInfo;

	retInfo.dwFlag = ( bRealData ) ? MFI_REALDATA : 0;
	MakeUpperCase( strFileName );
    if ( strFileName2.empty() == false )
    {
	    info2.dwFlag = ( bRealData ) ? MFI_REALDATA : 0;
	    MakeUpperCase( strFileName2 );
    }

#ifdef MASS_FILE_FIRST
#ifdef WCHAR_DIR
	std::wstring wstrFileName = KncUtil::toWideString( strFileName );
	std::wstring wstrFileName2 = KncUtil::toWideString( strFileName2 );

	MassFileMap::const_iterator iter = m_mapMassFile.find( wstrFileName );
#else
	MassFileMap::const_iterator iter = m_mapMassFile.find( strFileName );
#endif WCHAR_DIR

	if( iter == m_mapMassFile.end() )
	{	
#if !defined( _SERVICE_ ) //&& !defined( _VTUNE_ )
#ifdef WCHAR_DIR
		if( LoadTwoRealFiles( wstrFileName, retInfo, wstrFileName2, info2 ) == true )
#else
		if( LoadTwoRealFiles( strFileName, retInfo, strFileName2, info2 ) == true )
#endif WCHAR_DIR
			return retInfo;
		else
#endif//_SERVICE_
		{
			ErrorLogMsg( KEM_ERROR135, strFileName.c_str() );
			return retInfo;
		}
	}
	else
	{	
		iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &retInfo );
#ifdef WCHAR_DIR
        if ( wstrFileName2.empty() == false )
        {
            iter = m_mapMassFile.find( wstrFileName2 );
            if ( iter != m_mapMassFile.end() )
                iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &info2 );
        }
#else
		if ( strFileName2.empty() == false )
		{
			iter = m_mapMassFile.find( strFileName2 );
			if ( iter != m_mapMassFile.end() )
				iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &info2 );
		}
#endif WCHAR_DIR
		return retInfo;
	}
#else
#ifdef WCHAR_DIR
	std::wstring wstrFileName = KncUtil::toWideString( strFileName );
	std::wstring wstrFileName2 = KncUtil::toWideString( strFileName2 );

	if ( LoadTwoRealFiles( wstrFileName, retInfo, wstrFileName2, info2 ) == true )
		return retInfo;

	MassFileMap::const_iterator iter = m_mapMassFile.find( wstrFileName );
#else
	if ( LoadTwoRealFiles( strFileName, retInfo, strFileName2, info2 ) == true )
		return retInfo;

	MassFileMap::const_iterator iter = m_mapMassFile.find( strFileName );
#endif WCHAR_DIR

	if( iter != m_mapMassFile.end() )
	{
		iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &retInfo );
#ifdef WCHAR_DIR
		if ( wstrFileName2.empty() == false )
		{
			iter = m_mapMassFile.find( wstrFileName2 );
			
			if ( iter != m_mapMassFile.end() )
				iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &info2 );
		}
#else
		if ( strFileName2.empty() == false )
		{
			iter = m_mapMassFile.find( strFileName2 );

			if ( iter != m_mapMassFile.end() )
				iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &info2 );
		}
#endif WCHAR_DIR
		return retInfo;
	}
	else
	{
		ErrorLogMsg( KEM_ERROR136, strFileName.c_str() );
		return retInfo;
	}
#endif MASS_FILE_FIRST

}
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES


//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
#ifdef WCHAR_DIR
KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::wstring strFileName, bool bRealData 
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
      , bool bKeepCompressedData
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS  
      )
#else WCHAR_DIR
KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::string strFileName, bool bRealData )
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
      , bool bKeepCompressedData
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS  
      )
#endif WCHAR_DIR
{
    CMassFile::MASSFILE_MEMBERFILEINFO_POINTER retInfo;

	ASSERT( false == strFileName.empty() );
	if ( strFileName.empty() == true )
		return	retInfo;

//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
	retInfo.dwFlag = ( bRealData ) ? MFI_REALDATA : 0;
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
    if ( bRealData && bKeepCompressedData )
        retInfo.dwFlag |= MFI_COMPRESSEDDATA;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
	MakeUpperCase( strFileName );

#ifdef MASS_FILE_FIRST

#ifdef X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE
	std::map< std::string, CMassFile::MASSFILE_MEMBERFILEINFO_POINTER >::iterator itNewFolderFile = m_mapNewFolderFile.find( strFileName );
	if(  itNewFolderFile != m_mapNewFolderFile.end() )
		return itNewFolderFile->second;
#endif//X2OPTIMIZE_REFERENCE_RESOURCE_NEW_FOLDER_FOR_VTUNE

	MassFileMap::const_iterator iter = m_mapMassFile.find( strFileName );

	if( iter == m_mapMassFile.end() )
	{	
#if !defined( _SERVICE_ ) //&& !defined( _VTUNE_ )
		if( LoadRealFile( strFileName, &retInfo ) == true )
			return retInfo;
		else
#endif//_SERVICE_
		{
#ifndef X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
			// .TET 파일의 경우 없는 경우가 많으므로 에러로그를 남기지 않고, 다른 방법으로 에러체크할 방법이 필요. 일단 임시로 요렇게.
#ifdef WCHAR_DIR
			if( std::string::npos == strFileName.find( L".TET" ) )
#else WCHAR_DIR
			if( std::string::npos == strFileName.find( ".TET" ) )
#endif WCHAR_DIR
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
			{
				ErrorLogMsg( KEM_ERROR135, strFileName.c_str() );
			}
			return retInfo;
		}
	}
	else
	{	
		iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &retInfo );
		return retInfo;
	}
#else
	if( LoadRealFile( strFileName, &retInfo ) == true )
		return retInfo;

	MassFileMap::const_iterator iter = m_mapMassFile.find( strFileName );
	if( iter != m_mapMassFile.end() )
	{
		iter->second.pMassFile->GetMemberFile( *this, iter->second.Index, &retInfo );
		return retInfo;
	}
	else
	{
#ifndef X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
		// .TET 파일의 경우 없는 경우가 많으므로 에러로그를 남기지 않고, 다른 방법으로 에러체크할 방법이 필요. 일단 임시로 요렇게.
#ifdef WCHAR_DIR
		if( std::string::npos == strFileName.find( L".TET" ) )
#else WCHAR_DIR
		if( std::string::npos == strFileName.find( ".TET" ) )
#endif WCHAR_DIR
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
		{
			ErrorLogMsg( KEM_ERROR136, strFileName.c_str() );
		}

		return retInfo;
	}
#endif MASS_FILE_FIRST
}//KGCMassFileManager::LoadDataFile()

#ifdef WCHAR_DIR
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::string strFileName, bool bRealData
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
      , bool bKeepCompressedData
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS  
      )
//#else   X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::string strFileName, bool bRealData )
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
{
	WCHAR wstrFileName[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, strFileName.c_str(), -1, wstrFileName, ARRAY_SIZE(wstrFileName) );
	return LoadDataFile( wstrFileName, bRealData
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        , bKeepCompressedData
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        );
}//KGCMassFileManager::LoadDataFile()
#else WCHAR_DIR
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::wstring wstrFileName, bool bRealData
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
      , bool bKeepCompressedData
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS  
      )
//#else   X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::wstring wstrFileName, bool bRealData )
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
{
	char strFileName[255] = {0,};
	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, ARRAY_SIZE(strFileName), NULL, NULL );
    return LoadDataFile( strFileName, bRealData
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        , bKeepCompressedData
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        );
}//KGCMassFileManager::LoadDataFile()
#endif WCHAR_DIR

KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile_LocalFile( std::string strFileName, bool bRealData )
{
    CMassFile::MASSFILE_MEMBERFILEINFO_POINTER retInfo;

	ASSERT( false == strFileName.empty() );
	if ( strFileName.empty() == true )
		return	retInfo;

	retInfo.dwFlag = ( bRealData ) ? MFI_REALDATA : 0;
	MakeUpperCase( strFileName );

#ifdef WCHAR_DIR
	WCHAR wstrFileName[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, strFileName.c_str(), -1, wstrFileName, ARRAY_SIZE(wstrFileName) );

	LoadRealFile( wstrFileName, &retInfo );
#else
	LoadRealFile( strFileName, &retInfo );
#endif WCHAR_DIR
    return retInfo;
}//KGCMassFileManager::LoadDataFile()

KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile_LocalFile( std::wstring wstrFileName, bool bRealData )
{
	char strFileName[255] = {0,};
	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, ARRAY_SIZE(strFileName), NULL, NULL );
    return LoadDataFile_LocalFile( strFileName, bRealData );
}//KGCMassFileManager::LoadDataFile()


#ifdef  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES

KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadTwoDataFiles( 
    const std::wstring& wstrFileName, const std::wstring& wstrFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO_PTR& info2,
    bool bRealData )
{
	char strFileName[255] = {0,};
	char strFileName2[255] = {0,};
	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, ARRAY_SIZE(strFileName), NULL, NULL );
	WideCharToMultiByte( CP_ACP, 0, wstrFileName2.c_str(), -1, strFileName2, ARRAY_SIZE(strFileName2), NULL, NULL );
    return LoadTwoDataFiles( strFileName, strFileName2, info2, bRealData );
}//KGCMassFileManager::LoadDataFile()

#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES


//#else // BACKGROUND_LOADING_TEST // 2008-10-16
//
////{{AFX: jintaeks on 2008-10-16, 14:57
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::string strFileName, bool bRealData, bool bLocalFileOnly )
//#else   X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_PTR  KGCMassFileManager::LoadDataFile( std::string strFileName, bool bRealData )
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//{
//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK	
//
//	m_Info.dwFlag = bRealData;
//	MakeUpperCase( strFileName );
//
//#ifdef MASS_FILE_FIRST
//
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//    if ( bLocalFileOnly == true )
//    {
//		if ( LoadRealFile( strFileName, &m_Info ) == true )
//            return &m_Info;
//        return NULL;
//    }
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//
//	MassFileMap::const_iterator iter = m_mapMassFile.find(strFileName);
//
//	if( iter == m_mapMassFile.end() )
//	{	
//		if( LoadRealFile( strFileName, &m_Info ) == true )
//			return &m_Info;
//		else
//		{
//			// .TET 파일의 경우 없는 경우가 많으므로 에러로그를 남기지 않고, 다른 방법으로 에러체크할 방법이 필요. 일단 임시로 요렇게.
//			if( std::string::npos == strFileName.find( ".TET" ) )
//			{
//				ErrorLogMsg( KEM_ERROR135, strFileName.c_str() );
//			}
//            return NULL;
//		}
//	}
//	else
//	{	
//		const SMassFileIndex *sFileIndex = &iter->second;
//		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
//		return &m_Info;
//	}
//#else
//	if( LoadRealFile( strFileName, &m_Info ) == true )
//		return &m_Info;
//
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//    if ( bLocalFileOnly == true )
//    {
//        return NULL;
//    }
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//
//	MassFileMap::const_iterator iter = m_mapMassFile.find(strFileName);
//	if( iter != m_mapMassFile.end() )
//	{
//		const SMassFileIndex *sFileIndex = &iter->second;
//		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
//		return &m_Info;
//	}
//	else
//	{
//		// .TET 파일의 경우 없는 경우가 많으므로 에러로그를 남기지 않고, 다른 방법으로 에러체크할 방법이 필요. 일단 임시로 요렇게.
//		if( std::string::npos == strFileName.find( ".TET" ) )
//		{
//			ErrorLogMsg( KEM_ERROR136, strFileName.c_str() );
//		}
//		return NULL;
//	}
//#endif MASS_FILE_FIRST
//}//KGCMassFileManager::LoadDataFile()
//
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO* KGCMassFileManager::LoadDataFile( std::wstring wstrFileName, bool bRealData, bool bLocalFileOnly )
//#else   X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO* KGCMassFileManager::LoadDataFile( std::wstring wstrFileName, bool bRealData )
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//{
//#ifndef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK	
//
//	char strFileName[255] = {0,};
//
//	m_Info.dwFlag = bRealData;
//	MakeUpperCase( wstrFileName );
//	WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, strFileName, 255, NULL, NULL );
//
//#ifdef MASS_FILE_FIRST
//
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//    if ( bLocalFileOnly == true )
//    {
//		if ( LoadRealFile( strFileName, &m_Info ) == true )
//            return &m_Info;
//        return NULL;
//    }
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//
//	MassFileMap::const_iterator iter = m_mapMassFile.find(strFileName);
//	if( iter == m_mapMassFile.end() )
//	{	
//		if( LoadRealFile( strFileName, &m_Info ) == true )
//			return &m_Info;
//		else
//		{
//			//ErrorLogMsg( KEM_ERROR137, wstrFileName.c_str() );
//			return NULL;
//		}
//	}
//	else
//	{	
//		const SMassFileIndex *sFileIndex = &iter->second;
//		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
//		return &m_Info;
//	}
//#else 
//	if( LoadRealFile( strFileName, &m_Info ) == true )
//		return &m_Info;
//
//#ifdef  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//    if ( bLocalFileOnly == true )
//    {
//		return NULL;
//    }
//#endif  X2OPTIMIZE_MASS_FILE_MANAGER_REAL_FILE_ONLY
//
//	MassFileMap::const_iterator iter = m_mapMassFile.find(strFileName);
//	if( iter != m_mapMassFile.end() )
//	{
//		const SMassFileIndex *sFileIndex = &iter->second;
//		sFileIndex->pMassFile->GetMemberFile( sFileIndex->Index, &m_Info );
//		return &m_Info;
//	}
//	else
//	{
//		//ErrorLogMsg( KEM_ERROR138, wstrFileName.c_str() );
//		return NULL;
//	}
//#endif MASS_FILE_FIRST
//}//KGCMassFileManager::LoadDataFile()
////}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16


//KGCMassFileManager::KGCMemoryFile* KGCMassFileManager::LoadMemoryFile( std::string strFileName )
//{
//	if( LoadDataFile( strFileName ) != NULL )
//	{
//		m_MemoryFile.SetFile( m_Info.pRealData, m_Info.size );
//		return &m_MemoryFile;
//	}
//	else
//	{
//		ErrorLogMsg( KEM_ERROR139, strFileName.c_str() );
//		return NULL;
//	}
//}
//
//KGCMassFileManager::KGCMemoryFile* KGCMassFileManager::LoadMemoryFile( std::wstring wstrFileName )
//{
//	if( LoadDataFile( wstrFileName ) != NULL )
//	{
//		m_MemoryFile.SetFile( m_Info.pRealData, m_Info.size );
//		return &m_MemoryFile;
//	}
//	else
//	{
//		ErrorLogMsg( KEM_ERROR140, wstrFileName.c_str() );
//		return NULL;
//	}
//}

#ifdef WCHAR_DIR
bool KGCMassFileManager::LoadRealFile(const std::wstring& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO* pOut)
#else WCHAR_DIR
bool KGCMassFileManager::LoadRealFile(const std::string& strFileName, CMassFile::MASSFILE_MEMBERFILEINFO* pOut)
#endif WCHAR_DIR
{
//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
	const KTLSData& kTLSData = GetUpdatedTLSData();
#ifdef WCHAR_DIR
	const std::vector<std::wstring>& vecstrDataDirectory = kTLSData.m_vecstrDataDirectory;
#else WCHAR_DIR
	const std::vector<std::string>& vecstrDataDirectory = kTLSData.m_vecstrDataDirectory;
#endif WCHAR_DIR
//#else	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//	CSLock locker( m_csFileManager );
//	const std::vector<std::string>& vecstrDataDirectory = m_vecstrDataDirectory;
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

	HANDLE hFile = INVALID_HANDLE_VALUE; 

#ifdef WCHAR_DIR
	std::wstring strName;	// MASSFILE_MAPPING_FUNCTION
#else WCHAR_DIR
	std::string strName;	// MASSFILE_MAPPING_FUNCTION
#endif WCHAR_DIR
	for( int i = 0; i < (int)vecstrDataDirectory.size(); i++ )
	{
		strName.resize(0);
		strName = vecstrDataDirectory[i];
#ifdef MASSFILE_MAPPING_FUNCTION
#ifdef WCHAR_DIR
		std::wstring strRealFileName = strFileName;
#else WCHAR_DIR
		std::string strRealFileName = strFileName;
#endif WCHAR_DIR
		if(ExternFileMapping(strRealFileName))
			strName += strRealFileName;
		else
			strName += strFileName;
#else
		strName += strFileName;		
#endif MASSFILE_MAPPING_FUNCTION

#ifdef WCHAR_DIR
		hFile = CreateFileW( strName.c_str(),	// file to open
#else WCHAR_DIR
		hFile = CreateFileA( strName.c_str(),	// file to open
#endif WCHAR_DIR
			GENERIC_READ,          				// open for reading
			FILE_SHARE_READ,       				// share for reading
			NULL,                  				// default security
			OPEN_EXISTING,         				// existing file only
			FILE_ATTRIBUTE_NORMAL, 				// normal file
			NULL);                 				// no attr. template

		if( INVALID_HANDLE_VALUE != hFile ) 
		{ 
			break;
		}
	}
	


	if( INVALID_HANDLE_VALUE == hFile ) 
	{ 
		// .tet, .xet 파일은 없는 경우가 많아서 log 안 남김. 그리고 다른 파일의 경우에도 필요한 경우에만 로그 남기도록 수정
#ifdef LOADREALFILE_ERROR_LOG
		if( strFileName.empty() )
		{
			LeaveLastErrorLog( KEM_ERROR141, "_FILE_NAME_IS_NULL_", "open error" );
		}
		else
		{
			string tempFileName = strFileName;
			MakeUpperCase( tempFileName );
			string::size_type iCharIndex = tempFileName.find( ".TET" );
			if( string::npos == iCharIndex ) // .tet 파일이 아니면
			{
				iCharIndex = tempFileName.find( ".XET" );
				if( string::npos == iCharIndex ) // .xet 파일이 아니면
				{
					LeaveLastErrorLog( KEM_ERROR141, strFileName.c_str(), "open error" );
				}
			}
		}
#endif LOADREALFILE_ERROR_LOG
		
		return false;
	}


	DWORD dwFileSize = ::GetFileSize( hFile, NULL );
	if( INVALID_FILE_SIZE == dwFileSize )
	{
		LeaveLastErrorLog( KEM_ERROR141, strFileName.c_str(), "file size error" );
		CloseHandle( hFile );
		return false;
	}	
	
	pOut->size = (long) dwFileSize; // warning!! error-prone

	if(pOut->dwFlag & MFI_REALDATA)
	{
//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
		KGCMassFileBufferPtr bufferPtr = KGCMassFileManager::m_massFileBufMan.GetBuffer( pOut->size );
		pOut->pRealData = bufferPtr->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//		KGCMassFileBufferPtr bufferPtr = KGCMassFileManager::m_massFileBufMan.GetBuffer();
//		pOut->pRealData = bufferPtr->GetBuffer( pOut->size );
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
        
//#else // BACKGROUND_LOADING_TEST // 2008-10-16
//		pOut->pRealData = (char*)m_BufferManager.GetBuffer(pOut->size);
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16

		DWORD dwBytesRead = 0;
		if( FALSE == ::ReadFile(hFile, pOut->pRealData, dwFileSize, &dwBytesRead, NULL) )
		{
            pOut->pRealData = NULL;
			LeaveLastErrorLog( KEM_ERROR141, strFileName.c_str(), "read error" );

			CloseHandle(hFile);
			return false;
		}

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
        pOut->pBuffer = bufferPtr;
//#else // BACKGROUND_LOADING_TEST // 2008-10-16
//        //{{AFX
//		pOut->pBuffer = m_BufferManager.GetCurrBuffer();
//        //}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16
#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        pOut->dwFlag |= MFI_LOCALFILE;
#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE

#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        pOut->dwFlag &= ~MFI_COMPRESSEDDATA;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS


	}

#ifdef WCHAR_DIR
	StrCpyW( pOut->strFileName, strFileName.c_str() );
#else WCHAR_DIR
#ifdef	CONVERSION_VS
	strncpy_s( pOut->strFileName, _countof(pOut->strFileName), strFileName.c_str() , MASSFILE_NAMESIZE-1 );
#else	CONVERSION_VS
	strncpy( pOut->strFileName, strFileName.c_str() , MASSFILE_NAMESIZE-1 );
#endif	CONVERSION_VS
#endif WCHAR_DIR
	//strcpy( pOut->strFileName, strFileName.c_str() );
	CloseHandle(hFile);

	return true;
}//KGCMassFileManager::LoadRealFile()


#ifdef X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES
#ifdef WCHAR_DIR
bool    KGCMassFileManager::LoadTwoRealFiles(const std::wstring& strFileName, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info,
	const std::wstring& strFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info2 )
#else
bool    KGCMassFileManager::LoadTwoRealFiles(const std::string& strFileName, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info,
            const std::string& strFileName2, OUT CMassFile::MASSFILE_MEMBERFILEINFO& info2 )
#endif WCHAR_DIR
{
	const KTLSData& kTLSData = GetUpdatedTLSData();
#ifdef WCHAR_DIR
	const std::vector<std::wstring>& vecstrDataDirectory = kTLSData.m_vecstrDataDirectory;
#else
	const std::vector<std::string>& vecstrDataDirectory = kTLSData.m_vecstrDataDirectory;
#endif WCHAR_DIR

	HANDLE hFile = INVALID_HANDLE_VALUE;
    HANDLE hFile2 = INVALID_HANDLE_VALUE;

#ifdef WCHAR_DIR
	std::wstring strName;
	std::wstring strName2;
#else
	std::string	strName;
	std::string	strName2;
#endif WCHAR_DIR
    int i = 0;
	for( ; i < (int)vecstrDataDirectory.size(); i++ )
	{
		strName.resize(0);
		strName = vecstrDataDirectory[i];
		strName += strFileName;

#ifdef WCHAR_DIR
		hFile = CreateFileW(strName.c_str(),	// file to open
#else
		hFile = CreateFileA(strName.c_str(),	// file to open
#endif WCHAR_DIR
			GENERIC_READ,          				// open for reading
			FILE_SHARE_READ,       				// share for reading
			NULL,                  				// default security
			OPEN_EXISTING,         				// existing file only
			FILE_ATTRIBUTE_NORMAL, 				// normal file
			NULL);                 				// no attr. template

		if( INVALID_HANDLE_VALUE != hFile ) 
		{ 
			break;
		}
	}
    if ( INVALID_HANDLE_VALUE != hFile  && strFileName2.empty() == false )
    {
        strName2 = vecstrDataDirectory[i];
        strName2 += strFileName2;

#ifdef WCHAR_DIR
		hFile = CreateFileW(strName2.c_str(),	// file to open
#else
		hFile = CreateFileA(strName2.c_str(),	// file to open
#endif WCHAR_DIR
			GENERIC_READ,          				// open for reading
			FILE_SHARE_READ,       				// share for reading
			NULL,                  				// default security
			OPEN_EXISTING,         				// existing file only
			FILE_ATTRIBUTE_NORMAL, 				// normal file
			NULL);  
    }

	if( INVALID_HANDLE_VALUE == hFile ) 
	{ 
		// .tet, .xet 파일은 없는 경우가 많아서 log 안 남김. 그리고 다른 파일의 경우에도 필요한 경우에만 로그 남기도록 수정
#ifdef LOADREALFILE_ERROR_LOG
		if( strFileName.empty() )
		{
			LeaveLastErrorLog( KEM_ERROR141, "_FILE_NAME_IS_NULL_", "open error" );
		}
		else
		{
			string tempFileName = strFileName;
			MakeUpperCase( tempFileName );
			string::size_type iCharIndex = tempFileName.find( ".TET" );
			if( string::npos == iCharIndex ) // .tet 파일이 아니면
			{
				iCharIndex = tempFileName.find( ".XET" );
				if( string::npos == iCharIndex ) // .xet 파일이 아니면
				{
					LeaveLastErrorLog( KEM_ERROR141, strFileName.c_str(), "open error" );
				}
			}
		}
#endif LOADREALFILE_ERROR_LOG
		
		return false;
	}

	DWORD dwFileSize = ::GetFileSize( hFile, NULL );
	if( INVALID_FILE_SIZE == dwFileSize )
	{
		LeaveLastErrorLog( KEM_ERROR141, strFileName.c_str(), "file size error" );
		CloseHandle( hFile );
        if ( INVALID_HANDLE_VALUE != hFile2 )
            CloseHandle( hFile2 );
		return false;
    }
	info.size = (long) dwFileSize; // warning!! error-prone

    DWORD   dwFileSize2 = 0;
    if ( hFile2 != INVALID_HANDLE_VALUE )
    {
        dwFileSize2 = ::GetFileSize( hFile2, NULL );
        if ( INVALID_FILE_SIZE == dwFileSize2 )
        {
		    LeaveLastErrorLog( KEM_ERROR141, strFileName2.c_str(), "file size error" );
		    CloseHandle( hFile2 );
            hFile2 = INVALID_HANDLE_VALUE;
        }
        else
        {
            info2.size = (long) dwFileSize2;
        }
    }

	if( info.dwFlag & MFI_REALDATA)
	{
		KGCMassFileBufferPtr bufferPtr = KGCMassFileManager::m_massFileBufMan.GetBuffer( info.size );
		info.pRealData = bufferPtr->GetBuffer();

		DWORD dwBytesRead = 0;
		if( FALSE == ::ReadFile(hFile, info.pRealData, dwFileSize, &dwBytesRead, NULL) )
		{
            info.pRealData = NULL;
			LeaveLastErrorLog( KEM_ERROR141, strFileName.c_str(), "read error" );
			CloseHandle(hFile);
            if ( INVALID_HANDLE_VALUE != hFile2 )
                CloseHandle( hFile2 );
			return false;
		}

        info.pBuffer = bufferPtr;

#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        info.dwFlag |= MFI_LOCALFILE;
#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE

#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        info.dwFlag &= ~MFI_COMPRESSEDDATA;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS

	}
    if ( hFile2 != INVALID_HANDLE_VALUE && ( info2.dwFlag & MFI_REALDATA ) != 0 )
    {
		KGCMassFileBufferPtr bufferPtr = KGCMassFileManager::m_massFileBufMan.GetBuffer( info2.size );
		info2.pRealData = bufferPtr->GetBuffer();
		DWORD dwBytesRead = 0;
		if( FALSE == ::ReadFile(hFile2, info2.pRealData, dwFileSize2, &dwBytesRead, NULL) )
		{
            info2.pRealData = NULL;
			LeaveLastErrorLog( KEM_ERROR141, strFileName2.c_str(), "read error" );
		}
        else
        {
            info2.pBuffer = bufferPtr;
#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
            info2.dwFlag |= MFI_LOCALFILE;
#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        }

#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
        info2.dwFlag &= ~MFI_COMPRESSEDDATA;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
    }

#ifdef	CONVERSION_VS
#ifdef WCHAR_DIR
	StrCpyW( info.strFileName, strFileName.c_str() );
	if ( hFile2 != INVALID_HANDLE_VALUE )
		StrCpyW( info2.strFileName, strFileName2.c_str() );
#else
	strncpy_s( info.strFileName, _countof(info.strFileName), strFileName.c_str() , MASSFILE_NAMESIZE-1 );
	if ( hFile2 != INVALID_HANDLE_VALUE )
		strncpy_s( info2.strFileName, _countof(info2.strFileName), strFileName2.c_str() , MASSFILE_NAMESIZE-1 );
#endif WCHAR_DIR
#else	CONVERSION_VS
	strncpy( info.strFileName, strFileName.c_str() , MASSFILE_NAMESIZE-1 );
    if ( hFile2 != INVALID_HANDLE_VALUE )
        strncpy( info2.strFileName, strFileName2.c_str() , MASSFILE_NAMESIZE-1 );
#endif	CONVERSION_VS

    if ( hFile != INVALID_HANDLE_VALUE )
	    CloseHandle(hFile);
    if ( hFile2 != INVALID_HANDLE_VALUE )
        CloseHandle(hFile2 );

	return true;

}
#endif  X2OPTIMIZE_SUPPORT_LOADING_TWO_ADJACENT_MASS_FILES



////////////////////////////////////////////////////////////////////////////////////
// MassFile
////////////////////////////////////////////////////////////////////////////////////

//#ifndef BACKGROUND_LOADING_TEST // 2008-10-17
////{{AFX
//KGCMassFileManager::KGCBufferManager KGCMassFileManager::CMassFile::m_BufferManager1;
//KGCMassFileManager::KGCBufferManager KGCMassFileManager::CMassFile::m_BufferManager2;
////}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

KGCMassFileManager::CMassFile::CMassFile()
{
	//::InitializeCriticalSection( &m_csMassFile );
    //m_vecMemberFileHeader;
    m_strMassFileName[0] = NULL;

	//{{ robobeg : 2011-08-16
	//m_hFile = INVALID_HANDLE_VALUE;
//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	std::fill( &m_ahFilePerThread[0], &m_ahFilePerThread[MASS_FILE_MANAGER_MAX_NUM_THREADS], INVALID_HANDLE_VALUE );
//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//	m_hFileForegroundSync = INVALID_HANDLE_VALUE;
//	m_hFileBackgroundAsync = INVALID_HANDLE_VALUE;
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	//}} robobeg : 2011-08-16
    m_iFileVersion = 0;
	memset( &m_MassFileHeader, 0, sizeof(MASSFILE_HEADER) );	
	m_MassFileHeader.bCompressed = true;
}//KGCMassFileManager::CMassFile::CMassFile()


KGCMassFileManager::CMassFile::~CMassFile()
{
	//::EnterCriticalSection( &m_csMassFile );
//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	for( int i = 0; i < MASS_FILE_MANAGER_MAX_NUM_THREADS; i++ )
	{
		HANDLE	hFile = m_ahFilePerThread[i];
		if ( hFile != INVALID_HANDLE_VALUE )
		{
			m_ahFilePerThread[i] = INVALID_HANDLE_VALUE;
			::CloseHandle( hFile );
		}
		else
		{
			ErrorLog( KEM_ERROR142 );
		}
	}
//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//	if ( INVALID_HANDLE_VALUE != m_hFileForegroundSync )
//	{
//		CloseHandle( m_hFileForegroundSync );
//	}
//	else
//	{
//		ErrorLog( KEM_ERROR142 );
//	}
//
//	if ( INVALID_HANDLE_VALUE != m_hFileBackgroundAsync )
//	{
//		CloseHandle( m_hFileBackgroundAsync );
//	}
//	else
//	{
//		ErrorLog( KEM_ERROR142 );
//	}
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	//::LeaveCriticalSection( &m_csMassFile );

	//::DeleteCriticalSection( &m_csMassFile );
}//KGCMassFileManager::CMassFile::~CMassFile()


UINT KGCMassFileManager::CMassFile::GetFileCount()
{
	//CSLock locker( m_csMassFile );

	return m_MassFileHeader.iTotalFileNo;
}//KGCMassFileManager::CMassFile::GetFileCount()


#ifdef WCHAR_DIR
void KGCMassFileManager::CMassFile::GetFileName( WCHAR* Buffer, const WCHAR* strFilePath )
#else WCHAR_DIR
void KGCMassFileManager::CMassFile::GetFileName( char* Buffer, const char* strFilePath )
#endif WCHAR_DIR
{
#ifdef WCHAR_DIR
	for( int i = (int)wcslen( strFilePath ) - 1; i > 0; i-- )
#else WCHAR_DIR
	for( int i = (int)strlen( strFilePath ) - 1; i > 0; i-- )
#endif WCHAR_DIR
	{
		if(strFilePath[i] == '\\' || strFilePath[i] == '/')
		{
#ifdef WCHAR_DIR
			StrCpyW( Buffer,&strFilePath[i + 1] );
#else WCHAR_DIR
	#ifdef	CONVERSION_VS
			strncpy_s(Buffer, MASSFILE_NAMESIZE, &strFilePath[i+1], MASSFILE_NAMESIZE-1);
	#else	CONVERSION_VS
			strncpy(Buffer,&strFilePath[i+1], MASSFILE_NAMESIZE-1);
	#endif	CONVERSION_VS
#endif WCHAR_DIR
			
			//strcpy(Buffer,&strFilePath[i+1]);
			return;
		}
	}

#ifdef WCHAR_DIR
	StrCpyW( Buffer, strFilePath );
#else WCHAR_DIR
#ifdef	CONVERSION_VS
	strncpy_s(Buffer, MASSFILE_NAMESIZE, strFilePath , MASSFILE_NAMESIZE-1);
#else	CONVERSION_VS
	strncpy(Buffer,strFilePath , MASSFILE_NAMESIZE-1);
#endif	CONVERSION_VS
#endif WCHAR_DIR
	//strcpy(Buffer,strFilePath);
	return;
}//KGCMassFileManager::CMassFile::GetFileName()


#ifdef WCHAR_DIR
bool KGCMassFileManager::CMassFile::LoadMassFile( const WCHAR* strMassFile )
#else WCHAR_DIR
bool KGCMassFileManager::CMassFile::LoadMassFile( const char* strMassFile )
#endif WCHAR_DIR
{
	//CSLock locker( m_csMassFile );
//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	for( int i = 0; i < MASS_FILE_MANAGER_MAX_NUM_THREADS; i++ )
	{
		HANDLE	hFileTemp = m_ahFilePerThread[i];
		if ( hFileTemp != INVALID_HANDLE_VALUE )
		{
			m_ahFilePerThread[i] = INVALID_HANDLE_VALUE;
			::CloseHandle( hFileTemp );
		}
	}
//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//	if( INVALID_HANDLE_VALUE != m_hFileForegroundSync )
//	{
//		::CloseHandle( m_hFileForegroundSync );
//		m_hFileForegroundSync = INVALID_HANDLE_VALUE;
//	}
//	if( INVALID_HANDLE_VALUE != m_hFileBackgroundAsync )
//	{
//		::CloseHandle( m_hFileBackgroundAsync );
//		m_hFileBackgroundAsync = INVALID_HANDLE_VALUE;
//	}
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

	HANDLE	hFile = INVALID_HANDLE_VALUE;

//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

	for( int i = 0; i < MASS_FILE_MANAGER_MAX_NUM_THREADS; i++ )
	{
#ifdef	_SERVICE_
		unsigned	uRetryCount = ( i == 0 ) ? 2 : 0 ;
#else	_SERVICE_
		unsigned	uRetryCount = 0;
#endif	_SERVICE_

		HANDLE	hFileTemp = KGCMassFileManager::OpenReadFileHandle( strMassFile, uRetryCount, false );
		if ( hFileTemp == INVALID_HANDLE_VALUE )
		{
			return false;
		}
		m_ahFilePerThread[i] = hFileTemp;
	}
	hFile = m_ahFilePerThread[0];

//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//
//#ifdef	_SERVICE_
//	unsigned	uRetryCount = 2;
//#else	_SERVICE_
//	unsigned	uRetryCount = 0;
//#endif	_SERVICE_
//
//	m_hFileForegroundSync = KGCMassFileManager::OpenReadFileHandle( strMassFile, uRetryCount, false );
//	if ( m_hFileForegroundSync == INVALID_HANDLE_VALUE )
//		return false;
//	m_hFileBackgroundAsync = KGCMassFileManager::OpenReadFileHandle( strMassFile, 0, true );
//	if ( m_hFileBackgroundAsync == INVALID_HANDLE_VALUE )
//		return false;
//
//	hFile = m_hFileForegroundSync;
//
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

#ifdef WCHAR_DIR
	wcsncpy_s( m_strMassFileName, _countof(m_strMassFileName), strMassFile , MAX_PATH-1 );
#else WCHAR_DIR
#ifdef	CONVERSION_VS
	strncpy_s( m_strMassFileName, _countof(m_strMassFileName), strMassFile , MAX_PATH-1 );
#else	CONVERSION_VS
	strncpy( m_strMassFileName, strMassFile , MAX_PATH-1 );
#endif	CONVERSION_VS
#endif WCHAR_DIR
	
	//strcpy( m_strMassFileName, strMassFile );
	m_vecMemberFileHeader.clear();
	memset(&m_MassFileHeader,0,sizeof(MASSFILE_HEADER));

	// 신나는 하드코딩의 세계 
	// 신버전 콤을 지원하고, 구 버전 콤이랑 어떻게 싸바해볼려는 처절한 발악
	// 

	 DWORD   dwOffset = 0;
	DWORD dwBytesRead = 0;
	if( FALSE == KGCMassFileManager::ReadFile( hFile, m_strMassFileName, &m_MassFileHeader, sizeof(MASSFILE_HEADER), &dwBytesRead, dwOffset ) )
	{
		LeaveLastErrorLog( KEM_ERROR143, strMassFile, "read error 1" );
		return false;
	}

	dwOffset += sizeof(MASSFILE_HEADER);

	m_iFileVersion = (m_MassFileHeader.strVersionInfo[23] -'0') * 10 + m_MassFileHeader.strVersionInfo[25] - '0';

	switch(m_iFileVersion)
	{	
	case 1:
	case 2:
		{
			for(int i = 0; i < (int)m_MassFileHeader.iTotalFileNo; i++)
			{
				MASSFILE_MEMBERFILEHEADER MemberHeader;
				if( FALSE == KGCMassFileManager::ReadFile( hFile, m_strMassFileName, &MemberHeader, sizeof(MASSFILE_MEMBERFILEHEADER), &dwBytesRead, dwOffset ) )
				{
					LeaveLastErrorLog( KEM_ERROR143, strMassFile, "read error 2" );
					return false;
				}
				dwOffset += sizeof(MASSFILE_MEMBERFILEHEADER);

#ifdef WCHAR_DIR
				wstring temp = MemberHeader.strFileName;
				MakeUpperCase( temp );
				wsprintf( MemberHeader.strFileName, temp.c_str() );
#else WCHAR_DIR
				string temp = MemberHeader.strFileName;
				MakeUpperCase( temp );

				StringCchPrintfA( MemberHeader.strFileName,  MASSFILE_NAMESIZE , temp.c_str() );
				//sprintf( MemberHeader.strFileName, temp.c_str() );
#endif WCHAR_DIR

				m_vecMemberFileHeader.push_back(MemberHeader);
			}
		}
        break;
	case 3:
		{
			DWORD filetime;
			DWORD adler32;
			DWORD headersize;

			
			if(FALSE==KGCMassFileManager::ReadFile( hFile, m_strMassFileName, &filetime, sizeof(DWORD), &dwBytesRead, dwOffset ))
			{
				LeaveLastErrorLog( KEM_ERROR143, strMassFile, "read error 2" );
				return false;
			}
			dwOffset += sizeof(DWORD);
			if(FALSE==KGCMassFileManager::ReadFile( hFile, m_strMassFileName, &adler32, sizeof(DWORD), &dwBytesRead, dwOffset ))
			{
				LeaveLastErrorLog( KEM_ERROR143, strMassFile, "read error 2" );
				return false;
			}
			dwOffset += sizeof(DWORD);
			if(FALSE==KGCMassFileManager::ReadFile( hFile, m_strMassFileName, &headersize, sizeof(DWORD), &dwBytesRead, dwOffset ))
			{
				LeaveLastErrorLog( KEM_ERROR143, strMassFile, "read error 2" );
				return false;
			}
			dwOffset += sizeof(DWORD);
			char * header = new char[headersize+1];
			if(FALSE==KGCMassFileManager::ReadFile( hFile, m_strMassFileName, header, headersize , &dwBytesRead, dwOffset ))
			{
				delete [] header;
				LeaveLastErrorLog( KEM_ERROR143, strMassFile, "read error 2" );
				return false;
			}
			dwOffset += headersize;
			header[headersize]='\0';

			xmlDoc * doc;			
			doc = xmlReadMemory(header,headersize,"Komfiles.xml",NULL,0);			
			delete [] header;

			xmlNode * files = xmlDocGetRootElement(doc);
			long offset = 72 + headersize - (sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_MassFileHeader.iTotalFileNo);


			if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"Files")==0)
			{			
				xmlNode * file = files->children;
				while(file)
				{
					if(file->type == XML_ELEMENT_NODE && strcmp((char*)file->name,"File")==0)
					{	
						MASSFILE_MEMBERFILEHEADER MemberHeader;
						xmlAttr * attribute =  file->properties;
						while(attribute)
						{
							if(strcmp((char*)attribute->name,"Name")==0 )
							{
#ifdef WCHAR_DIR
								WCHAR filename[MASSFILE_NAMESIZE] = {0,};
								MultiByteToWideChar( CP_ACP, 0, (char*)attribute->children->content, -1, filename, ARRAY_SIZE(filename) );
								wcsncpy_s( MemberHeader.strFileName, _countof(MemberHeader.strFileName), filename, MASSFILE_NAMESIZE - 1 );
#else WCHAR_DIR
					#ifdef	CONVERSION_VS
								strncpy_s(MemberHeader.strFileName, _countof(MemberHeader.strFileName), (char*)attribute->children->content , MASSFILE_NAMESIZE-1);
					#else	CONVERSION_VS
								strncpy(MemberHeader.strFileName,(char*)attribute->children->content , MASSFILE_NAMESIZE-1);
					#endif	CONVERSION_VS
#endif WCHAR_DIR
								//strcpy(MemberHeader.strFileName,(char*)attribute->children->content);
								MakeUpperCase(MemberHeader.strFileName);
							}
							else if(strcmp((char*)attribute->name,"CompressedSize")==0)
							{
					#ifdef	CONVERSION_VS
								sscanf_s((char*)attribute->children->content,"%d",&MemberHeader.CompressSize);
					#else	CONVERSION_VS
								sscanf((char*)attribute->children->content,"%d",&MemberHeader.CompressSize);
					#endif	CONVERSION_VS
							}
							else if(strcmp((char*)attribute->name,"Size")==0)
							{
					#ifdef	CONVERSION_VS
								sscanf_s((char*)attribute->children->content,"%d",&MemberHeader.RealSize);
					#else	CONVERSION_VS
								sscanf((char*)attribute->children->content,"%d",&MemberHeader.RealSize);
					#endif	CONVERSION_VS								
							}
							attribute = attribute->next;
						}
						MemberHeader.offSet = offset;
						offset += MemberHeader.CompressSize;
						m_vecMemberFileHeader.push_back(MemberHeader);
					}
					file = file->next;
				}
			}			
			xmlFreeDoc(doc);
		}
		break;
	default:
		LeaveLastErrorLog( KEM_ERROR143, strMassFile, "read error 3" );
		return false;
	}

	return true;
}//KGCMassFileManager::CMassFile::LoadMassFile()


//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16

bool KGCMassFileManager::CMassFile::_GetMemberFile( KGCMassFileManager& kManager, const MASSFILE_MEMBERFILEHEADER& MEMBERFILEHEADER, MASSFILE_MEMBERFILEINFO* pOut)
{
    ASSERT( pOut );

	HANDLE	hFile = INVALID_HANDLE_VALUE;

//#ifdef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

	int		iThreadIndex = kManager.GetCurrentThreadIndex();
	if ( iThreadIndex < 0 || iThreadIndex >= MASS_FILE_MANAGER_MAX_NUM_THREADS )
		return false;
	hFile = m_ahFilePerThread[ iThreadIndex ];

//#else	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//
//	bool    bAsync = kManager.GetForegroundThreadId() != ::GetCurrentThreadId();
//	hFile = ( bAsync == false ) ? m_hFileForegroundSync : m_hFileBackgroundAsync;
//
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

	if ( hFile == INVALID_HANDLE_VALUE )
		return false;

    // initialize [out] parameter
	pOut->size          = MEMBERFILEHEADER.RealSize;
#ifdef WCHAR_DIR
	wcsncpy_s( pOut->strFileName, _countof(MEMBERFILEHEADER.strFileName ) , MEMBERFILEHEADER.strFileName ,  MASSFILE_NAMESIZE-1);
#else WCHAR_DIR
#ifdef	CONVERSION_VS
	strncpy_s( pOut->strFileName, _countof(pOut->strFileName), MEMBERFILEHEADER.strFileName , MASSFILE_NAMESIZE-1 );
#else	CONVERSION_VS
	strncpy( pOut->strFileName, MEMBERFILEHEADER.strFileName , MASSFILE_NAMESIZE-1 );
#endif	CONVERSION_VS
	//strcpy( pOut->strFileName, MEMBERFILEHEADER.strFileName );
#endif WCHAR_DIR
    pOut->pRealData     = NULL;


	//GetRealData
	if( pOut->dwFlag & MFI_REALDATA )
	{			
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_MassFileHeader.iTotalFileNo;


		if(m_MassFileHeader.bCompressed)
		{	

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
			KGCMassFileBufferPtr bufferPtr2 = KGCMassFileManager::m_massFileBufMan.GetBuffer( MEMBERFILEHEADER.CompressSize );
			char* pCompressedData = bufferPtr2->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//			KGCMassFileBufferPtr bufferPtr2 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
//			char* pCompressedData = bufferPtr2->GetBuffer( MEMBERFILEHEADER.CompressSize );
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

//#else // BACKGROUND_LOADING_TEST // 2008-10-16
//
//            //{{AFX
//			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize, true);
//            //}}AFX
//			
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16


			//DWORD dwSetFilePointer;
			//dwSetFilePointer = SetFilePointer( m_hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );

			//if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
			//{ 
			//	LeaveLastErrorLog( KEM_ERROR390, MEMBERFILEHEADER.strFileName, "file pointer error" );
			//	return false;
			//}

			DWORD dwBytesRead;
			if( FALSE == KGCMassFileManager::ReadFile( hFile, MEMBERFILEHEADER.strFileName, pCompressedData, MEMBERFILEHEADER.CompressSize, &dwBytesRead, posRealDataStart + MEMBERFILEHEADER.offSet
//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO				
//				, bAsync
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO									
				) )
			{
				LeaveLastErrorLog( KEM_ERROR390, MEMBERFILEHEADER.strFileName, "read error" );
				return false;
			}

#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
            if ( pOut->dwFlag & MFI_COMPRESSEDDATA )
            {
                pOut->pRealData = pCompressedData;
                pOut->pBuffer = bufferPtr2;
                pOut->compressedSize = MEMBERFILEHEADER.CompressSize;
            }
            else
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
            {


    //#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
    //#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
			    KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer( MEMBERFILEHEADER.RealSize );
			    pOut->pRealData = bufferPtr1->GetBuffer();
    //#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
    //			KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
    //			pOut->pRealData = bufferPtr1->GetBuffer( MEMBERFILEHEADER.RealSize );
    //#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
                pOut->pBuffer   = bufferPtr1;

    //#else // BACKGROUND_LOADING_TEST // 2008-10-16
    //
    //            //{{AFX
    //			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
    //			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
    //            //}}AFX
    //#endif // BACKGROUND_LOADING_TEST // 2008-10-16

			    unsigned long size = MEMBERFILEHEADER.RealSize;
#ifdef  X2OPTIMIZE_ZLIB_UNCOMPRESS_ERROR_CHECK
			    if ( Z_OK != uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize) )
                {
                    pOut->pRealData = NULL;
                    pOut->pBuffer.reset();
				    LeaveLastErrorLog( KEM_ERROR390, MEMBERFILEHEADER.strFileName, " uncompress error" );
				    return false;
                }
#else   X2OPTIMIZE_ZLIB_UNCOMPRESS_ERROR_CHECK
			    uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
#endif  X2OPTIMIZE_ZLIB_UNCOMPRESS_ERROR_CHECK
                ASSERT( size == MEMBERFILEHEADER.RealSize );
            }
		}
		else
		{

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
			KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer( MEMBERFILEHEADER.RealSize );
			pOut->pRealData = bufferPtr1->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//			KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
//			pOut->pRealData = bufferPtr1->GetBuffer( MEMBERFILEHEADER.RealSize );
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
            pOut->pBuffer   = bufferPtr1;
//#else // BACKGROUND_LOADING_TEST // 2008-10-16
//
//            //{{AFX
//			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
//			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
//            //}}AFX
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16

// 			DWORD dwSetFilePointer;
// 			dwSetFilePointer = SetFilePointer( m_hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );
// 
// 			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
// 			{ 
// 				LeaveLastErrorLog( KEM_ERROR390, MEMBERFILEHEADER.strFileName, "file pointer error 2" );
// 				return false;
// 			}
#ifdef  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS
            pOut->dwFlag &= ~MFI_COMPRESSEDDATA;
#endif  X2OPTIMIZE_ENCRYPT_AFTER_COMPRESS

			DWORD dwBytesRead;
			if( FALSE == KGCMassFileManager::ReadFile( hFile, MEMBERFILEHEADER.strFileName, pOut->pRealData, MEMBERFILEHEADER.RealSize, &dwBytesRead, posRealDataStart + MEMBERFILEHEADER.offSet
//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO				
//				, bAsync
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO	
				) )
			{
				LeaveLastErrorLog( KEM_ERROR390, MEMBERFILEHEADER.strFileName, "read error 2" );
				return false;
			}
		}	
	}

	return true;
}//KGCMassFileManager::CMassFile::_GetMemberFile()


#ifdef WCHAR_DIR
bool KGCMassFileManager::CMassFile::GetMemberFile( KGCMassFileManager& kManager, const WCHAR* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
#else WCHAR_DIR
bool KGCMassFileManager::CMassFile::GetMemberFile( KGCMassFileManager& kManager, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
#endif WCHAR_DIR
{
	//CSLock locker( m_csMassFile );

	MASSFILE_MEMBERFILEHEADER   MEMBERFILEHEADER;
	bool                        find = false;

	for ( int i = 0; i < (int)m_vecMemberFileHeader.size(); ++i )
	{
#ifdef WCHAR_DIR
		if ( StrCmpW(m_vecMemberFileHeader[i].strFileName,strRealFile) == 0 )
#else WCHAR_DIR
		if ( strcmp(m_vecMemberFileHeader[i].strFileName,strRealFile) == 0 )
#endif WCHAR_DIR
		{
			MEMBERFILEHEADER = m_vecMemberFileHeader[i];
			find = true;
			break;
		}//if
	}//for

	if ( find == false )
		return false;

    return _GetMemberFile( kManager, MEMBERFILEHEADER, pOut );
}//KGCMassFileManager::CMassFile::GetMemberFile()

bool KGCMassFileManager::CMassFile::GetMemberFile( KGCMassFileManager& kManager, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
{
	//CSLock locker( m_csMassFile );

	if ( iFileIndex < 0 || iFileIndex >= (int)m_vecMemberFileHeader.size() )
		return false;

	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER = m_vecMemberFileHeader[iFileIndex];	

    return _GetMemberFile( kManager, MEMBERFILEHEADER, pOut );
}//KGCMassFileManager::CMassFile::GetMemberFile()

//#else // BACKGROUND_LOADING_TEST // 2008-10-16
//
////{{AFX: jintaeks on 2008-10-16, 15:11
//bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
//{
//	//CSLock locker( m_csMassFile );
//
//	if( INVALID_HANDLE_VALUE == m_hFile )
//	{
//		return false;
//	}
//
//	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;
//	bool find = false;
//	for(int i = 0; i < (int)m_vecMemberFileHeader.size(); i++)
//	{
//		if(strcmp(m_vecMemberFileHeader[i].strFileName,strRealFile) == 0)
//		{
//			MEMBERFILEHEADER = m_vecMemberFileHeader[i];
//			find = true;
//			break;
//		}
//	}
//	if(find == false)
//		return false;
//
//	pOut->size				= MEMBERFILEHEADER.RealSize;
//	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
//
//	//GetRealData
//	if( pOut->dwFlag & MFI_REALDATA )
//	{			
//		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_MassFileHeader.iTotalFileNo;
//
//
//		if(m_MassFileHeader.bCompressed)
//		{	
//			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize, true);
//
//// 			DWORD dwSetFilePointer;
//// 			dwSetFilePointer = SetFilePointer( m_hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );
//// 
//// 			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
//// 			{ 
//// 				LeaveLastErrorLog( KEM_ERROR390, strRealFile, "file pointer error" );
//// 				return false;
//// 			}
//
//			DWORD dwBytesRead;
//			if( FALSE == KGCMassFileManager::NewReadFile( m_hFile, MEMBERFILEHEADER.strFileName, pCompressedData, MEMBERFILEHEADER.CompressSize, &dwBytesRead, posRealDataStart + MEMBERFILEHEADER.offSet ) )
//			{
//				LeaveLastErrorLog( KEM_ERROR390, strRealFile, "read error" );
//				return false;
//			}
//
//			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
//			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
//
//			unsigned long size = MEMBERFILEHEADER.RealSize;
//			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
//		}
//		else
//		{
//			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
//			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
//
//// 			DWORD dwSetFilePointer;
//// 			dwSetFilePointer = SetFilePointer( m_hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );
//// 
//// 			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
//// 			{ 
//// 				LeaveLastErrorLog( KEM_ERROR390, strRealFile, "file pointer error 2" );
//// 				return false;
//// 			}
//
//
//			DWORD dwBytesRead;
//			if( FALSE == ReadFile( m_hFile, pOut->pRealData, MEMBERFILEHEADER.RealSize, &dwBytesRead, NULL ) )
//			{
//				LeaveLastErrorLog( KEM_ERROR390, strRealFile, "read error 2" );
//				return false;
//			}
//		}	
//	}
//
//	return true;
//}//KGCMassFileManager::CMassFile::GetMemberFile()
//
//
//bool KGCMassFileManager::CMassFile::GetMemberFile(int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
//{
//	//CSLock locker( m_csMassFile );
//
//
//	if( INVALID_HANDLE_VALUE == m_hFile )
//	{
//		return false;
//	}
//
//	if(iFileIndex < 0 || iFileIndex >= (int)m_vecMemberFileHeader.size())
//		return false;
//
//	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER = m_vecMemberFileHeader[iFileIndex];	
//
//	pOut->size = MEMBERFILEHEADER.RealSize;
//	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
//	pOut->pRealData = 0;
//
//	if( pOut->dwFlag & MFI_REALDATA )
//	{		
//		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * m_MassFileHeader.iTotalFileNo;
//
//		if(m_MassFileHeader.bCompressed)
//		{		
//			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize, true);
//			
//// 			DWORD dwSetFilePointer;
//// 			dwSetFilePointer = SetFilePointer( m_hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );
//// 
//// 			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
//// 			{ 
//// 				LeaveLastErrorLog( KEM_ERROR394, MEMBERFILEHEADER.strFileName, "file pointer error" );
//// 				return false;
//// 			}
//
//			DWORD dwBytesRead;
//			if( FALSE == KGCMassFileManager::NewReadFile( m_hFile, MEMBERFILEHEADER.strFileName, pCompressedData, MEMBERFILEHEADER.CompressSize, &dwBytesRead, posRealDataStart + MEMBERFILEHEADER.offSet ) )
//			{
//				LeaveLastErrorLog( KEM_ERROR394, MEMBERFILEHEADER.strFileName, "read error" );
//				return false;
//			}
//
//
//			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
//			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
//
//			unsigned long size = MEMBERFILEHEADER.RealSize;
//			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
//		}
//		else
//		{
//			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
//			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
//
//// 			DWORD dwSetFilePointer;
//// 			dwSetFilePointer = SetFilePointer( m_hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );
//// 
//// 			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
//// 			{ 
//// 				LeaveLastErrorLog( KEM_ERROR394, MEMBERFILEHEADER.strFileName, "file pointer error 2" );
//// 				return false;
//// 			}
//
//
//			DWORD dwBytesRead;
//			if( FALSE == KGCMassFileManager::NewReadFile( m_hFile, MEMBERFILEHEADER.strFileName, pOut->pRealData, MEMBERFILEHEADER.RealSize, &dwBytesRead, posRealDataStart + MEMBERFILEHEADER.offSet ) )
//			{
//				LeaveLastErrorLog( KEM_ERROR394, MEMBERFILEHEADER.strFileName, "read error 2" );
//				return false;
//			}
//		}
//	}
//
//
//	return true;
//}//KGCMassFileManager::CMassFile::GetMemberFile()
////}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16

/*
#ifdef BACKGROUND_LOADING_TEST // 2008-10-16

bool KGCMassFileManager::CMassFile::_GetMemberFileFromKom(HANDLE hFile, MASSFILE_HEADER& Header, MASSFILE_MEMBERFILEHEADER& MEMBERFILEHEADER, MASSFILE_MEMBERFILEINFO* pOut)
{
    ASSERT( pOut );

	pOut->size = MEMBERFILEHEADER.RealSize;
#ifdef	CONVERSION_VS
	strncpy_s(pOut->strFileName, _countof(pOut->strFileName), MEMBERFILEHEADER.strFileName, MASSFILE_NAMESIZE-1 );
#else	CONVERSION_VS
	strncpy(pOut->strFileName, MEMBERFILEHEADER.strFileName, MASSFILE_NAMESIZE-1 );
#endif	CONVERSION_VS
	//strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
	pOut->pRealData = 0;

#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
    KGCMassFileBufferPtr    bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
    KGCMassFileBufferPtr    bufferPtr2 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
#endif // BACKGROUND_LOADING_TEST // 2008-10-16

	if( pOut->dwFlag & MFI_REALDATA )
	{		
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

		if(Header.bCompressed)
		{		

#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
			char* pCompressedData = bufferPtr2->GetBuffer( MEMBERFILEHEADER.CompressSize );
#else // BACKGROUND_LOADING_TEST // 2008-10-16

            //{{AFX
			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize, true);
            //}}AFX

#endif // BACKGROUND_LOADING_TEST // 2008-10-16

			DWORD dwSetFilePointer;
			dwSetFilePointer = SetFilePointer( hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );

			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
			{ 
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "file pointer error" );
				return false;
			}

			DWORD dwBytesRead;
			if( FALSE == ReadFile( hFile, pCompressedData, MEMBERFILEHEADER.CompressSize, &dwBytesRead, NULL ) )
			{
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "read error" );
				return false;
			}


#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
			pOut->pRealData = bufferPtr1->GetBuffer( MEMBERFILEHEADER.RealSize );
			pOut->pBuffer   = bufferPtr1;
#else // BACKGROUND_LOADING_TEST // 2008-10-16

            //{{AFX
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
            //}}AFX

#endif // BACKGROUND_LOADING_TEST // 2008-10-16

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
			pOut->pRealData = bufferPtr1->GetBuffer( MEMBERFILEHEADER.RealSize );
            pOut->pBuffer   = bufferPtr1;
#else // BACKGROUND_LOADING_TEST // 2008-10-16

            //{{AFX
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();
            //}}AFX

#endif // BACKGROUND_LOADING_TEST // 2008-10-16

			DWORD dwSetFilePointer;
			dwSetFilePointer = SetFilePointer( hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );

			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
			{ 
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "file pointer error 2" );
				return false;
			}

			DWORD dwBytesRead;
			if( FALSE == ReadFile( hFile, pOut->pRealData, MEMBERFILEHEADER.RealSize, &dwBytesRead, NULL ) )
			{
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "read error 2" );
				return false;
			}
		}
	}

	CloseHandle( hFile );
	return true;
}//KGCMassFileManager::CMassFile::_GetMemberFileFromKom()


bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
{
	HANDLE hFile;

	hFile = CreateFileA( strMassFile,		// file to open
		GENERIC_READ,          				// open for reading
		FILE_SHARE_READ,       				// share for reading
		NULL,                  				// default security
		OPEN_EXISTING,         				// existing file only
		FILE_ATTRIBUTE_NORMAL, 				// normal file
		NULL);                 				// no attr. template

	if ( INVALID_HANDLE_VALUE == hFile ) 
	{ 
		LeaveLastErrorLog( KEM_ERROR392, strMassFile, "open error" );
		return false;
	}//if

	CSLock locker( m_csMassFile );

	MASSFILE_HEADER Header;
	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;
	int iFileIndex = -1;
	DWORD dwBytesRead = 0;

	if ( FALSE == ReadFile(hFile, &Header, sizeof(MASSFILE_HEADER), &dwBytesRead, NULL) )
	{
		LeaveLastErrorLog( KEM_ERROR392, strMassFile, "read error 1" );
		CloseHandle(hFile);
		return false;
	}//if

	for(int i = 0; i < (int)Header.iTotalFileNo; i++)
	{
		if ( FALSE == ReadFile(hFile, &MEMBERFILEHEADER, sizeof(MEMBERFILEHEADER), &dwBytesRead, NULL) )
		{
			LeaveLastErrorLog( KEM_ERROR392, strMassFile, "read error 2" );
			CloseHandle(hFile);
			return false;
		}//if

		if ( strcmp(MEMBERFILEHEADER.strFileName,strRealFile) == 0 )
		{
			iFileIndex = i;
			break;
		}//if
	}//for
	
	if ( iFileIndex == -1 ) //파일을 찾을수 없음
	{
		CloseHandle( hFile );
		return false;
	}//if

    return _GetMemberFileFromKom( hFile, Header, MEMBERFILEHEADER, pOut );
}//KGCMassFileManager::CMassFile::GetMemberFile()


bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
{
	HANDLE hFile;

	hFile = CreateFileA( strMassFile,		// file to open
		GENERIC_READ,          				// open for reading
		FILE_SHARE_READ,       				// share for reading
		NULL,                  				// default security
		OPEN_EXISTING,         				// existing file only
		FILE_ATTRIBUTE_NORMAL, 				// normal file
		NULL);                 				// no attr. template

	if ( INVALID_HANDLE_VALUE == hFile ) 
	{ 
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "open error" );
		return false;
	}//if

	CSLock locker( m_csMassFile );
	MASSFILE_HEADER Header;

	DWORD dwBytesRead = 0;
	if ( FALSE == ReadFile(hFile, &Header, sizeof(MASSFILE_HEADER), &dwBytesRead, NULL) )
	{
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "read error 1" );
		CloseHandle(hFile);
		return false;
	}//if

	if ( (int)Header.iTotalFileNo <= iFileIndex )
	{
		CloseHandle( hFile );
		return false;
	}//if

	long MemberFileHeaderStart = sizeof(MASSFILE_HEADER);	
	long posRealDataStart = MemberFileHeaderStart + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

	//GetMemberFileHeader
	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;

	DWORD dwSetFilePointer;
	dwSetFilePointer = SetFilePointer( hFile, MemberFileHeaderStart + iFileIndex * sizeof(MASSFILE_MEMBERFILEHEADER), NULL, FILE_BEGIN );

	if ( INVALID_SET_FILE_POINTER == dwSetFilePointer )
	{ 
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "file pointer error" );
		return false;
	}//if

	if ( FALSE == ReadFile(hFile, &MEMBERFILEHEADER, sizeof(MASSFILE_MEMBERFILEHEADER), &dwBytesRead, NULL) )
	{
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "read error 2" );
		CloseHandle(hFile);
		return false;
	}//if

    return _GetMemberFileFromKom( hFile, Header, MEMBERFILEHEADER, pOut );
}//KGCMassFileManager::CMassFile::GetMemberFile()

#else // BACKGROUND_LOADING_TEST // 2008-10-16

//{{AFX: jintaeks on 2008-10-16, 15:21
bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strMassFile, const char* strRealFile, MASSFILE_MEMBERFILEINFO* pOut)
{
	HANDLE hFile;

	hFile = CreateFileA( strMassFile,		// file to open
		GENERIC_READ,          				// open for reading
		FILE_SHARE_READ,       				// share for reading
		NULL,                  				// default security
		OPEN_EXISTING,         				// existing file only
		FILE_ATTRIBUTE_NORMAL, 				// normal file
		NULL);                 				// no attr. template

	if( INVALID_HANDLE_VALUE == hFile ) 
	{ 
		LeaveLastErrorLog( KEM_ERROR392, strMassFile, "open error" );
		return false;
	}

	CSLock locker( m_csMassFile );

	MASSFILE_HEADER Header;
	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;
	int iFileIndex = -1;
	
	DWORD dwBytesRead = 0;
	if( FALSE == ReadFile(hFile, &Header, sizeof(MASSFILE_HEADER), &dwBytesRead, NULL) )
	{
		LeaveLastErrorLog( KEM_ERROR392, strMassFile, "read error 1" );
		CloseHandle(hFile);
		return false;
	}

	for(int i = 0; i < (int)Header.iTotalFileNo; i++)
	{
		if( FALSE == ReadFile(hFile, &MEMBERFILEHEADER, sizeof(MEMBERFILEHEADER), &dwBytesRead, NULL) )
		{
			LeaveLastErrorLog( KEM_ERROR392, strMassFile, "read error 2" );
			CloseHandle(hFile);
			return false;
		}
	
		if(strcmp(MEMBERFILEHEADER.strFileName,strRealFile) == 0)
		{
			iFileIndex = i;
			break;
		}
	}
	
	if(iFileIndex == -1) //파일을 찾을수 없음
	{
		CloseHandle( hFile );
		return false;
	}

	pOut->size = MEMBERFILEHEADER.RealSize;
	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
	pOut->pRealData = 0;

	if( pOut->dwFlag & MFI_REALDATA )
	{		
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

		if(Header.bCompressed)
		{		
			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize, true);

			DWORD dwSetFilePointer;
			dwSetFilePointer = SetFilePointer( hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );

			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
			{ 
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "file pointer error" );
				return false;
			}

			DWORD dwBytesRead;
			if( FALSE == ReadFile( hFile, pCompressedData, MEMBERFILEHEADER.CompressSize, &dwBytesRead, NULL ) )
			{
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "read error" );
				return false;
			}

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();

			DWORD dwSetFilePointer;
			dwSetFilePointer = SetFilePointer( hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );

			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
			{ 
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "file pointer error 2" );
				return false;
			}

			DWORD dwBytesRead;
			if( FALSE == ReadFile( hFile, pOut->pRealData, MEMBERFILEHEADER.RealSize, &dwBytesRead, NULL ) )
			{
				LeaveLastErrorLog( KEM_ERROR392, MEMBERFILEHEADER.strFileName, "read error 2" );
				return false;
			}
		}
	}

	CloseHandle( hFile );
	return true;
}//KGCMassFileManager::CMassFile::GetMemberFile()


bool KGCMassFileManager::CMassFile::GetMemberFile(const char* strMassFile, int iFileIndex, MASSFILE_MEMBERFILEINFO* pOut)
{
	HANDLE hFile;

	hFile = CreateFileA( strMassFile,		// file to open
		GENERIC_READ,          				// open for reading
		FILE_SHARE_READ,       				// share for reading
		NULL,                  				// default security
		OPEN_EXISTING,         				// existing file only
		FILE_ATTRIBUTE_NORMAL, 				// normal file
		NULL);                 				// no attr. template

	if( INVALID_HANDLE_VALUE == hFile ) 
	{ 
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "open error" );
		return false;
	}

	CSLock locker( m_csMassFile );
	MASSFILE_HEADER Header;

	DWORD dwBytesRead = 0;
	if( FALSE == ReadFile(hFile, &Header, sizeof(MASSFILE_HEADER), &dwBytesRead, NULL) )
	{
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "read error 1" );
		CloseHandle(hFile);
		return false;
	}

	if((int)Header.iTotalFileNo <= iFileIndex)
	{
		CloseHandle( hFile );
		return false;
	}
	long MemberFileHeaderStart = sizeof(MASSFILE_HEADER);	
	long posRealDataStart = MemberFileHeaderStart + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

	//GetMemberFileHeader
	MASSFILE_MEMBERFILEHEADER MEMBERFILEHEADER;

	DWORD dwSetFilePointer;
	dwSetFilePointer = SetFilePointer( hFile, MemberFileHeaderStart + iFileIndex * sizeof(MASSFILE_MEMBERFILEHEADER), NULL, FILE_BEGIN );

	if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
	{ 
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "file pointer error" );
		return false;
	}



	if( FALSE == ReadFile(hFile, &MEMBERFILEHEADER, sizeof(MASSFILE_MEMBERFILEHEADER), &dwBytesRead, NULL) )
	{
		LeaveLastErrorLog( KEM_ERROR393, strMassFile, "read error 2" );
		CloseHandle(hFile);
		return false;
	}

	pOut->size = MEMBERFILEHEADER.RealSize;
	strcpy(pOut->strFileName, MEMBERFILEHEADER.strFileName);
	pOut->pRealData = 0;

	if( pOut->dwFlag & MFI_REALDATA )
	{		
		long posRealDataStart = sizeof(MASSFILE_HEADER) + sizeof(MASSFILE_MEMBERFILEHEADER) * Header.iTotalFileNo;

		if(Header.bCompressed)
		{		
			char* pCompressedData = m_BufferManager2.GetBuffer(MEMBERFILEHEADER.CompressSize, true);

			DWORD dwSetFilePointer;
			dwSetFilePointer = SetFilePointer( hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );

			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
			{ 
				LeaveLastErrorLog( KEM_ERROR393, MEMBERFILEHEADER.strFileName, "file pointer error 2" );
				return false;
			}

			DWORD dwBytesRead;
			if( FALSE == ReadFile( hFile, pCompressedData, MEMBERFILEHEADER.CompressSize, &dwBytesRead, NULL ) )
			{
				LeaveLastErrorLog( KEM_ERROR393, MEMBERFILEHEADER.strFileName, "read error 2" );
				return false;
			}

			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();

			unsigned long size = MEMBERFILEHEADER.RealSize;
			uncompress((BYTE*)pOut->pRealData,&size,(BYTE*)pCompressedData,MEMBERFILEHEADER.CompressSize);
		}
		else
		{
			pOut->pRealData = (char*)m_BufferManager1.GetBuffer(MEMBERFILEHEADER.RealSize);
			pOut->pBuffer = m_BufferManager1.GetCurrBuffer();

			DWORD dwSetFilePointer;
			dwSetFilePointer = SetFilePointer( hFile, posRealDataStart + MEMBERFILEHEADER.offSet, NULL, FILE_BEGIN );

			if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
			{ 
				LeaveLastErrorLog( KEM_ERROR393, MEMBERFILEHEADER.strFileName, "file pointer error 3" );
				return false;
			}

			DWORD dwBytesRead;
			if( FALSE == ReadFile( hFile, pOut->pRealData, MEMBERFILEHEADER.RealSize, &dwBytesRead, NULL ) )
			{
				LeaveLastErrorLog( KEM_ERROR393, MEMBERFILEHEADER.strFileName, "read error 3" );
				return false;
			}
		}
	}

	CloseHandle( hFile );
	return true;
}//KGCMassFileManager::CMassFile::GetMemberFile()
//}}AFX
#endif // BACKGROUND_LOADING_TEST // 2008-10-16
*/

#ifdef WCHAR_DIR
long KGCMassFileManager::CMassFile::GetFileSize( WCHAR* strFileName )
#else WCHAR_DIR
long KGCMassFileManager::CMassFile::GetFileSize( char* strFileName )
#endif WCHAR_DIR
{
	HANDLE hFile;

#ifdef WCHAR_DIR
	hFile = CreateFileW( strFileName,		// file to open
#else WCHAR_DIR
	hFile = CreateFileA( strFileName,		// file to open
#endif WCHAR_DIR
		GENERIC_READ,          				// open for reading
		FILE_SHARE_READ,       				// share for reading
		NULL,                  				// default security
		OPEN_EXISTING,         				// existing file only
		FILE_ATTRIBUTE_NORMAL, 				// normal file
		NULL);                 				// no attr. template

	if( INVALID_HANDLE_VALUE == hFile ) 
	{ 
		LeaveLastErrorLog( KEM_ERROR391, strFileName, "open error" );
		return -1;
	}
	
	DWORD dwFileSize = ::GetFileSize(hFile, NULL);
	if( INVALID_FILE_SIZE == dwFileSize )
	{
		LeaveLastErrorLog( KEM_ERROR391, strFileName, "file size error" );
	}	
	CloseHandle( hFile );
	return (long) dwFileSize;
}//KGCMassFileManager::CMassFile::GetFileSize()


#ifdef WCHAR_DIR
bool KGCMassFileManager::CMassFile::SaveMassFile( WCHAR* strFileName, std::vector<WCHAR*> *pVecFile, bool bZip )
#else WCHAR_DIR
bool KGCMassFileManager::CMassFile::SaveMassFile( char* strFileName, std::vector<char*> *pVecFile, bool bZip )
#endif WCHAR_DIR
{
	FILE* file = NULL;
#ifdef WCHAR_DIR
	file = _wfopen(strFileName, L"w+b");
#else WCHAR_DIR
#ifdef	CONVERSION_VS
	fopen_s( &file, strFileName, "w+b");
#else	CONVERSION_VS
	file = fopen(strFileName, "w+b");
#endif	CONVERSION_VS
#endif WCHAR_DIR
	
	if(file == NULL)
		return false;

	//CSLock locker( m_csMassFile );

	//MASSFILE_HEADER
	MASSFILE_HEADER Header;
	Header.iTotalFileNo = (int)pVecFile->size();
	Header.bCompressed = bZip;

	StringCchPrintfA( Header.strVersionInfo, 50, "KOG GC TEAM MASSFILE V.%d.%d.",MASSFILE_VERSION/10,MASSFILE_VERSION%10 );
	//sprintf( Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.",MASSFILE_VERSION/10,MASSFILE_VERSION%10 );
	fwrite(&Header,sizeof(MASSFILE_HEADER),1,file);

	//MASSFILE_MEMBERFILEHEADER
//#ifndef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
////#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
//    KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
//    KGCMassFileBufferPtr bufferPtr2 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
////#endif // BACKGROUND_LOADING_TEST // 2008-10-16
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

	MASSFILE_MEMBERFILEHEADER MemberFileHeader;
	long TotalSize = 0;
	for(int i = 0; i < (int)pVecFile->size(); i++)
	{
		fwrite(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
	}
	TotalSize = 0;

	//RealFile
	FILE *pMemberFile = 0;
	char *pFileData = 0;
	long filesize;	

#ifdef CONVERSION_VS
	for(int i = 0; i < (int)pVecFile->size(); i++)
#else CONVERSION_VS
	for(i = 0; i < (int)pVecFile->size(); i++)
#endif CONVERSION_VS
	{
		filesize = GetFileSize((*pVecFile)[i]);

#ifdef WCHAR_DIR
		pMemberFile = _wfopen( (*pVecFile)[i], L"rb" );
#else WCHAR_DIR
#ifdef	CONVERSION_VS
		fopen_s( &pMemberFile, (*pVecFile)[i],"rb");
#else	CONVERSION_VS
		pMemberFile = fopen((*pVecFile)[i],"rb");
#endif	CONVERSION_VS
#endif WCHAR_DIR
		if(pMemberFile == NULL)
		{
			fclose(file);
			MessageBox( NULL, L"MassFile Error", L"Error", 0 );
			ErrorLogMsg( KEM_ERROR107, strFileName );
			return false;
		}

		unsigned long WriteByte = 0;

		if(bZip)
		{

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-17
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
			WriteByte = compressBound( filesize+8 );
			KGCMassFileBufferPtr bufferPtr2 = KGCMassFileManager::m_massFileBufMan.GetBuffer( filesize );
			KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer( WriteByte );
			pFileData = bufferPtr2->GetBuffer();
			char* pDestData = bufferPtr1->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//			pFileData = bufferPtr2->GetBuffer( filesize );
//			char* pDestData = bufferPtr1->GetBuffer( compressBound( filesize+8 ) );
//			WriteByte = bufferPtr1->GetBufferSize();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

//#else // BACKGROUND_LOADING_TEST // 2008-10-17
//
//            //{{AFX
//			pFileData = (char*)m_BufferManager2.GetBuffer(filesize, true);
//			char* pDestData = m_BufferManager1.GetBuffer( compressBound( filesize+8 ), true );
//            //}}AFX
//
//			WriteByte = m_BufferManager1.GetBufferSize();
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

			fread(pFileData,sizeof(char),filesize,pMemberFile);

			if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)pFileData,filesize))
			{
				fclose(file);
				return false;
			}
			fwrite(pDestData,sizeof(char),WriteByte,file);
		}
		else
		{

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-17
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
			KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer( filesize );
			pFileData = bufferPtr1->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//			pFileData = bufferPtr1->GetBuffer( filesize );
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//#else // BACKGROUND_LOADING_TEST // 2008-10-17
//
//            //{{AFX
//			pFileData = (char*)m_BufferManager1.GetBuffer(filesize, true);
//            //}}AFX
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

			fread(pFileData,sizeof(char),filesize,pMemberFile);
			fwrite(pFileData,sizeof(char),filesize,file);
			WriteByte = filesize;
		}//if.. else..

		fclose(pMemberFile);


		//==============================
		//현재 파일 커서를 받아온뒤..
		int cursor = ftell(file);

		//Header를 수정하고 다시 커서 제자리로...		
		fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);

		fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
		fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);


		memset(MemberFileHeader.strFileName,0,sizeof(char)*MASSFILE_NAMESIZE);
		GetFileName(MemberFileHeader.strFileName,(*pVecFile)[i]);
#ifdef WCHAR_DIR
		if(wcslen( MemberFileHeader.strFileName ) >= MASSFILE_NAMESIZE)
#else WCHAR_DIR
		if(strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
#endif WCHAR_DIR
		{
			fclose(file);
#ifdef WCHAR_DIR
			MessageBox( NULL, MemberFileHeader.strFileName, L"File Name Too Long!!!", 0 );
#else WCHAR_DIR
			WCHAR tempStr[255] = {0,};
			MultiByteToWideChar( CP_ACP, 0, MemberFileHeader.strFileName, -1, tempStr, 255);
			MessageBox( NULL, tempStr, L"File Name Too Long!!!", 0 );
			ErrorLogMsg( KEM_ERROR108, strFileName );
#endif WCHAR_DIR
			return false;
		}

		MemberFileHeader.CompressSize = WriteByte;
		MemberFileHeader.offSet = TotalSize;
		TotalSize += WriteByte;
		MemberFileHeader.RealSize = filesize;

		fwrite(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

		fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

		fseek(file,cursor,SEEK_SET);
		//==============================		
	}

	fclose(file);
	return true;
}//KGCMassFileManager::CMassFile::SaveMassFile()


#ifdef WCHAR_DIR
bool KGCMassFileManager::CMassFile::ModifyMassFile( KGCMassFileManager& kManager, CMassFile* pMassFile,WCHAR* strFileName, std::vector<WCHAR*> *pVecFile, bool bZip )
#else WCHAR_DIR
bool KGCMassFileManager::CMassFile::ModifyMassFile( KGCMassFileManager& kManager, CMassFile* pMassFile,char* strFileName, std::vector<char*> *pVecFile, bool bZip )
#endif WCHAR_DIR
{
	FILE* file = NULL;
#ifdef WCHAR_DIR
	file = _wfopen( strFileName, L"w+b" );
#else WCHAR_DIR
#ifdef	CONVERSION_VS
	fopen_s( &file, strFileName, "w+b");
#else	CONVERSION_VS
	file = fopen(strFileName, "w+b");
#endif	CONVERSION_VS
#endif WCHAR_DIR
	
	if(file == NULL)
		return false;

	//CSLock locker( m_csMassFile );

//#ifndef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
////#ifdef BACKGROUND_LOADING_TEST // 2008-10-16
//    KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
//    KGCMassFileBufferPtr bufferPtr2 = KGCMassFileManager::m_massFileBufMan.GetBuffer();
////#endif // BACKGROUND_LOADING_TEST // 2008-10-16
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

	MASSFILE_HEADER Header;
	Header.iTotalFileNo = (UINT)pVecFile->size();
	Header.bCompressed = bZip;

	StringCchPrintfA( Header.strVersionInfo, 50, "KOG GC TEAM MASSFILE V.%d.%d.",MASSFILE_VERSION/10,MASSFILE_VERSION%10 ); 
	//sprintf( Header.strVersionInfo, "KOG GC TEAM MASSFILE V.%d.%d.",MASSFILE_VERSION/10,MASSFILE_VERSION%10 );

	fwrite(&Header,sizeof(MASSFILE_HEADER),1,file);

	MASSFILE_MEMBERFILEINFO OutFile;
	MASSFILE_MEMBERFILEHEADER MemberFileHeader;

	for ( int i = 0; i < (int)pVecFile->size(); i++ )
	{
		fwrite(&MemberFileHeader, sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
	}

	FILE			*pMemberFile = 0;
	char			*pFileData = 0;
	long			FileSize = 0;
	unsigned long	WriteByte = 0;
	long			TotalSize = 0;

	for(int i = 0; i < (int)pVecFile->size(); i++)
	{
#ifdef WCHAR_DIR
		WCHAR Filename[MASSFILE_NAMESIZE];
#else WCHAR_DIR
		char Filename[MASSFILE_NAMESIZE];
#endif WCHAR_DIR
		GetFileName(Filename,(*pVecFile)[i]);
		if( !pMassFile->GetMemberFile( kManager, Filename, &OutFile ) )
		{
			//실제 파일을 로딩해서 저장
			//파일이 안에 없음
			FileSize = GetFileSize((*pVecFile)[i]);

#ifdef WCHAR_DIR
			pMemberFile = _wfopen((*pVecFile)[i],L"rb");
#else WCHAR_DIR
#ifdef	CONVERSION_VS
			fopen_s( &pMemberFile, (*pVecFile)[i],"rb");
#else	CONVERSION_VS
			pMemberFile = fopen((*pVecFile)[i],"rb");
#endif	CONVERSION_VS
#endif WCHAR_DIR
			
			if(pMemberFile == NULL)
			{
				fclose(file);
				MessageBox( NULL, L"MassFile Error", L"Error", 0 );
				ErrorLogMsg( KEM_ERROR109, strFileName );
				return false;
			}

			if(bZip)
			{

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-17
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
				WriteByte = compressBound( FileSize+8 );
				KGCMassFileBufferPtr bufferPtr2 = KGCMassFileManager::m_massFileBufMan.GetBuffer( FileSize );
				KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer( WriteByte );
				pFileData = bufferPtr2->GetBuffer();
				char* pDestData = bufferPtr1->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//				pFileData = bufferPtr2->GetBuffer( FileSize );
//				char* pDestData = bufferPtr1->GetBuffer( compressBound( FileSize+8 ) );
//				WriteByte = bufferPtr1->GetBufferSize();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER


//#else // BACKGROUND_LOADING_TEST // 2008-10-17
//
//                //{{AFX
//				pFileData = (char*)m_BufferManager2.GetBuffer(FileSize, true);
//				char* pDestData = m_BufferManager1.GetBuffer( compressBound( FileSize+8 ), true );
//
//				WriteByte = m_BufferManager1.GetBufferSize();
//                //}}AFX
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

				fread(pFileData,sizeof(char),FileSize,pMemberFile);

				if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)pFileData,FileSize))
				{
					fclose(file);
					return false;
				}
				fwrite(pDestData,sizeof(char),WriteByte,file);
			}
			else
			{

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-17
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
				KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer( FileSize );
				pFileData = bufferPtr1->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//				pFileData = bufferPtr1->GetBuffer( FileSize );
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//#else // BACKGROUND_LOADING_TEST // 2008-10-17
//
//                //{{AFX
//				pFileData = (char*)m_BufferManager1.GetBuffer(FileSize, true);
//                //}}AFX
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

				fread(pFileData,sizeof(char),FileSize,pMemberFile);
				fwrite(pFileData,sizeof(char),FileSize,file);
				WriteByte = FileSize;
			}

			fclose(pMemberFile);
		}
		else
		{
			if( bZip )
			{
				if( pMassFile->IsCompressed() )
				{
					pMassFile->SetCompressed( false );

					pMassFile->GetMemberFile( kManager, Filename, &OutFile );
					FileSize	= OutFile.size;
					WriteByte	= OutFile.size;

					fwrite(OutFile.pRealData,sizeof(char),WriteByte,file);

					pMassFile->SetCompressed( true );
				}
				else
				{
					FileSize = OutFile.size;

//#ifdef BACKGROUND_LOADING_TEST // 2008-10-17
//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
					WriteByte = compressBound( FileSize+8 );
					KGCMassFileBufferPtr bufferPtr1 = KGCMassFileManager::m_massFileBufMan.GetBuffer( WriteByte );
					char* pDestData = bufferPtr1->GetBuffer();
//#else	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
//					char* pDestData = bufferPtr1->GetBuffer( compressBound( FileSize+8 ) );
//					WriteByte = bufferPtr1->GetBufferSize();
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER

//#else // BACKGROUND_LOADING_TEST // 2008-10-17
//
//                    //{{AFX
//					char* pDestData = m_BufferManager1.GetBuffer( compressBound( FileSize+8 ), true );
//
//					WriteByte = m_BufferManager1.GetBufferSize();
//                    //}}AFX
//
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

					if(Z_OK	!= compress((BYTE*)pDestData,&WriteByte,(BYTE*)OutFile.pRealData,FileSize))
					{
						fclose(file);
						return false;
					}

					fwrite(pDestData,sizeof(char),WriteByte,file);
				}
			}
			else
			{
				FileSize	= OutFile.size;
				WriteByte	= FileSize;
				fwrite( OutFile.pRealData, sizeof(char), FileSize, file );
			}
		}

		int cursor = ftell(file);

		if ( 0 < cursor )
		{
			//Header를 수정하고 다시 커서 제자리로...		
			fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);

			fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);
			fseek(file,sizeof(MASSFILE_HEADER)+ sizeof(MASSFILE_MEMBERFILEHEADER) * i,SEEK_SET);


			memset(MemberFileHeader.strFileName,0,sizeof(char)*MASSFILE_NAMESIZE);
			GetFileName(MemberFileHeader.strFileName,(*pVecFile)[i]);
#ifdef WCHAR_DIR
			if(wcslen( MemberFileHeader.strFileName ) >= MASSFILE_NAMESIZE)
#else WCHAR_DIR
			if(strlen(MemberFileHeader.strFileName) >= MASSFILE_NAMESIZE)
#endif WCHAR_DIR
			{
				fclose(file);
#ifdef WCHAR_DIR
				MessageBoxW(NULL, MemberFileHeader.strFileName, L"File Name Too Long!!!", 0);
#else WCHAR_DIR
				MessageBoxA(NULL,MemberFileHeader.strFileName,"File Name Too Long!!!",0);
#endif WCHAR_DIR
				ErrorLogMsg( KEM_ERROR110, strFileName );
				return false;
			}

			MemberFileHeader.CompressSize = WriteByte;
			MemberFileHeader.offSet = TotalSize;
			TotalSize += WriteByte;
			MemberFileHeader.RealSize = FileSize;

			fwrite(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

			fread(&MemberFileHeader,sizeof(MASSFILE_MEMBERFILEHEADER),1,file);

			fseek(file,cursor,SEEK_SET);
		}
		//==============================		
	}

	fclose(file);
	return true;
}//KGCMassFileManager::CMassFile::ModifyMassFile()


//string KGCMassFileManager::FormatErrorMessage( DWORD dwErrorCode )
//{
//	LPVOID lpMsgBuf;
//	DWORD dwLastErr = GetLastError(); 
//
//	FormatMessageA(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
//		FORMAT_MESSAGE_FROM_SYSTEM |
//		FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL,
//		dwLastErr,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPSTR) &lpMsgBuf,
//		0, NULL );
//
//	string strErrorMsg = (char*)lpMsgBuf;
//	LocalFree(lpMsgBuf);
//
//	return strErrorMsg;
//}

#ifdef WCHAR_DIR
void KGCMassFileManager::LeaveLastErrorLog( KTDX_ERROR_MSG eErrorID, const WCHAR* szDesc1, const char* szDesc2 )
{
	WCHAR wstr2[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szDesc2, -1, wstr2, ARRAY_SIZE(wstr2) );

	LeaveLastErrorLog( eErrorID, szDesc1, wstr2 );
}
void KGCMassFileManager::LeaveLastErrorLog( KTDX_ERROR_MSG eErrorID, const char* szDesc1, const char* szDesc2 )
{
	WCHAR wstr1[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szDesc1, -1, wstr1, ARRAY_SIZE(wstr1) );
	WCHAR wstr2[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szDesc2, -1, wstr2, ARRAY_SIZE(wstr2) );

	LeaveLastErrorLog( eErrorID, wstr1, wstr2 );
}
#endif WCHAR_DIR

/*static*/
#ifdef WCHAR_DIR
void KGCMassFileManager::LeaveLastErrorLog( KTDX_ERROR_MSG eErrorID, const WCHAR* szDesc1, const WCHAR* szDesc2 )
#else WCHAR_DIR
void KGCMassFileManager::LeaveLastErrorLog( KTDX_ERROR_MSG eErrorID, const char* szDesc1, const char* szDesc2 )
#endif WCHAR_DIR
{
	LPVOID lpMsgBuf;
	DWORD dwLastErr = GetLastError(); 

#ifdef WCHAR_DIR
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwLastErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR) &lpMsgBuf,
		0, NULL );
#else WCHAR_DIR
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwLastErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR) &lpMsgBuf,
		0, NULL );
#endif WCHAR_DIR

#ifdef WCHAR_DIR
	wstring strErrorMsg = L"";
#else WCHAR_DIR
	string strErrorMsg = "";
#endif WCHAR_DIR
	if( NULL != lpMsgBuf )
	{
#ifdef WCHAR_DIR
		strErrorMsg = (WCHAR*)lpMsgBuf;
#else WCHAR_DIR
		strErrorMsg = (char*)lpMsgBuf;
#endif WCHAR_DIR
	}
	LocalFree(lpMsgBuf);

	if( NULL != szDesc1 )
	{
		strErrorMsg += szDesc1;
	}

	if( NULL != szDesc2 )
	{
		strErrorMsg += szDesc2;
	}

	ErrorLogMsg( eErrorID, strErrorMsg.c_str() );
}//KGCMassFileManager::LeaveLastErrorLog()

#ifdef	CHECK_FILE_DUPLICATION
bool KGCMassFileManager::ExtractFileDuplicatedToTextFile()
{
	if ( m_vecFileNameDuplicated.empty() )
		return true;
	else
	{
		ASSERT( !L"FileDuplicated Exists" );

		std::ofstream ofs( "FileNameDuplicatedList.txt" );

		if ( true == ofs.is_open() )
		{
			BOOST_TEST_FOREACH( const string&, strFileName, m_vecFileNameDuplicated )
			{
				ofs << strFileName << std::endl;
			}

			ofs.close();
			return true;
		}
		else
			return false;
	}
}
#endif	CHECK_FILE_DUPLICATION

////////////////////////////////////////////////////////////////////////////////////
// BufferManager
////////////////////////////////////////////////////////////////////////////////////

//#ifndef BACKGROUND_LOADING_TEST // 2008-10-17
//    //{{AFX
//	KGCMassFileManager::KGCBufferManager::KGCBufferManager()
//	{
//		::InitializeCriticalSection( &m_csBufferManager );
//
//		m_iCurrBufferIndex = 0;
//		m_iBufferCount = sizeof( m_aBuffer ) / sizeof( Buffer );
//	}//KGCMassFileManager::KGCBufferManager::KGCBufferManager()
//
//	KGCMassFileManager::KGCBufferManager::~KGCBufferManager()
//	{
//		Flush();
//
//		::DeleteCriticalSection( &m_csBufferManager );
//	}//KGCMassFileManager::KGCBufferManager::~KGCBufferManager()
//
//	void KGCMassFileManager::KGCBufferManager::Flush()
//	{
//		CSLock locker( m_csBufferManager );
//
//		for( int i=0; i<m_iBufferCount; i++ )
//		{
//			Buffer& buffer = m_aBuffer[i];
//			buffer.flush();
//		}
//
//		m_iCurrBufferIndex = 0;
//	}//KGCMassFileManager::KGCBufferManager::Flush()
//
//	char* KGCMassFileManager::KGCBufferManager::GetBuffer( int iSize, bool bForceCurrent /*= false*/ )
//	{
//		CSLock locker( m_csBufferManager );
//	#ifndef _SERVICE_
//
//		int iInUseCount = 0;
//		for( int i=0; i<m_iBufferCount; i++ )
//		{
//			if( m_aBuffer[i].m_bInUse == true )
//			{
//				iInUseCount++;
//			}
//		}
//		if( iInUseCount > 4 )
//		{
//			debugPrintf( "mass file buf in-use count: %d", iInUseCount );
//		}
//
//	#endif
//		
//		int iBufferIndex = m_iCurrBufferIndex;
//		if( true == bForceCurrent )
//		{
//			m_aBuffer[iBufferIndex].m_bInUse = false;
//		}
//		
//		while( true )
//		{
//			Buffer& buffer = m_aBuffer[iBufferIndex];
//			if( false == buffer.m_bInUse )
//			{
//				m_iCurrBufferIndex = iBufferIndex;
//				buffer.m_bInUse = true;
//				char* pBuf = buffer.GetMemory( iSize, false );
//				return pBuf;
//			}
//
//			iBufferIndex++;
//			if( iBufferIndex > m_iBufferCount-1 )
//				iBufferIndex = 0;
//
//			if( iBufferIndex == m_iCurrBufferIndex )
//				break;
//		}
//
//		// note!! 여기까지 오면 뭔가 잘못 됐다
//		// m_aBuffer를 vector로 바꾸고 실시간으로 확장할 수 있도록 바꾸자
//		StateLog( L"KGCBufferManager::GetBuffer returns NULL" );
//		assert( false );
//		return NULL;
//	}//KGCMassFileManager::KGCBufferManager::GetBuffer()
//    //}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-17

////////////////////////////////////////////////////////////////////////////////////
// MemoryFile
////////////////////////////////////////////////////////////////////////////////////

//KGCMassFileManager::KGCMemoryFile::KGCMemoryFile()
//{
//	m_iCursorPos	= 0;
//	m_pData			= NULL;
//	m_pCursor		= NULL;
//	m_iSize			= 0;
//}
//
//KGCMassFileManager::KGCMemoryFile::~KGCMemoryFile()
//{
//}
//
//void KGCMassFileManager::KGCMemoryFile::SetFile( char* pData, int size )
//{
//	m_iCursorPos	= 0;
//	m_pData			= pData;
//	m_pCursor		= pData;
//	m_iSize			= size;
//}
//
//int KGCMassFileManager::KGCMemoryFile::ReadFile(void* pDest, int ReadBytes)
//{
//	if( m_iCursorPos + ReadBytes > m_iSize )
//		return 0;
//
//	memcpy(pDest,m_pCursor,ReadBytes);
//	m_pCursor		+= ReadBytes;
//	m_iCursorPos	+= ReadBytes;
//
//	return ReadBytes;
//}
//
//int KGCMassFileManager::KGCMemoryFile::FileSeek(int iPos, int Pivot)
//{
//	switch(Pivot)
//	{	
//		case FILESEEK_CUR:
//			{
//				m_iCursorPos += iPos;
//			}
//			break;
//
//		case FILESEEK_SET:
//			{
//				m_iCursorPos = iPos;
//			}
//			break;
//
//		case FILESEEK_END:
//			{
//				m_iCursorPos = (m_iSize-1) + iPos;
//			}
//			break;	
//	}
//	m_pCursor = m_pData + m_iCursorPos;
//	return m_iCursorPos;
//}

bool KGCMassFileManager::SetSubDirectory()
{
#ifdef WCHAR_DIR
	WCHAR	szSearchDir[MAX_PATH] = L"";
	WCHAR	szCurrentDir[MAX_PATH] = L"";

	GetCurrentDirectoryW( MAX_PATH, szSearchDir );

	wcsncpy_s( szCurrentDir, _countof(szCurrentDir), szSearchDir , MAX_PATH-1 );
	StringCchCatW( szCurrentDir, MAX_PATH , L"\\" );
#else WCHAR_DIR
	char	szSearchDir[MAX_PATH] = "";
	char	szCurrentDir[MAX_PATH] = "";

#ifdef EFFECT_TOOL
	FILE *fp = fopen("ResourcePath.txt", "r");
	if( NULL != fp )
	{
		fscanf ( fp, "%s", szSearchDir );

#ifdef	CONVERSION_VS
		strncpy_s( szCurrentDir, _countof(szCurrentDir), szSearchDir , MAX_PATH-1 );
#else	//CONVERSION_VS
		strncpy( szCurrentDir, szSearchDir , MAX_PATH-1 );
#endif	//CONVERSION_VS

		StringCchCatA( szCurrentDir, MAX_PATH , "\\" );
		AddDataDirectory( szCurrentDir );

		GetSubDirectory( szSearchDir );

		fclose( fp );
	}
#endif // EFFECT_TOOL

	GetCurrentDirectoryA( MAX_PATH, szSearchDir );

#ifdef	CONVERSION_VS
	strncpy_s( szCurrentDir, _countof(szCurrentDir), szSearchDir , MAX_PATH-1 );
#else	CONVERSION_VS
	strncpy( szCurrentDir, szSearchDir , MAX_PATH-1 );
#endif	CONVERSION_VS
	
	StringCchCatA( szCurrentDir, MAX_PATH , "\\" );
	//strcat( szCurrentDir, "\\" );
#endif WCHAR_DIR

	AddDataDirectory( szCurrentDir );

	return GetSubDirectory( szSearchDir );	
}//KGCMassFileManager::SetSubDirectory()


#ifdef WCHAR_DIR
bool KGCMassFileManager::GetSubDirectory( WCHAR* szSearchDir)
#else WCHAR_DIR
bool KGCMassFileManager::GetSubDirectory( char* szSearchDir)
#endif WCHAR_DIR
{

#ifdef WCHAR_DIR
	HANDLE				hSearch;
	WIN32_FIND_DATAW	fd;
	WCHAR				szSearchPath[256] = { 0, };

	wcsncpy_s(szSearchPath, _countof(szSearchPath), szSearchDir , 255);
	StringCchCatW(szSearchPath, 256, L"\\*.*");

	hSearch = FindFirstFileW( szSearchPath, &fd );
#else WCHAR_DIR
	HANDLE				hSearch;
	WIN32_FIND_DATAA	fd;
	char				szSearchPath[256] = {0,};
	//char				szSearchPath[256];

#ifdef	CONVERSION_VS
	strncpy_s(szSearchPath, _countof(szSearchPath), szSearchDir , 255);
#else	CONVERSION_VS
	strncpy(szSearchPath, szSearchDir , 255);
#endif	CONVERSION_VS
	
	//strcpy(szSearchPath, szSearchDir);

	StringCchCatA(szSearchPath, 256, "\\*.*");
	//strcat(szSearchPath, "\\*.*");

	hSearch = FindFirstFileA(szSearchPath, &fd);
#endif WCHAR_DIR

	if(hSearch == INVALID_HANDLE_VALUE)
		return false;

#ifdef WCHAR_DIR
	do
	{
		if(StrCmpW(fd.cFileName, L".") && StrCmpW(fd.cFileName, L".."))
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				WCHAR	szNewSearchDir[256];
				wstring	strDir;

				StrCpyW(szNewSearchDir, szSearchDir);
				StrCatW(szNewSearchDir, L"\\");
				StrCatW(szNewSearchDir, fd.cFileName);

				//#ifndef _SERVICE_
				if( StrCmpW( fd.cFileName, L".svn" ) != 0 &&
					StrCmpW( fd.cFileName, L"Branches" ) != 0
					//#ifdef SERV_EPAY_SYSTEM //2012.03.22 lygan_조성욱 // 사내에서 epay관련 하위 폴더 경로는 검색 하지 않게 한다.
					&& StrCmpW( fd.cFileName, L"epay" ) != 0 
					&& StrCmpW( fd.cFileName, L"SupportFiles" ) != 0
					//#endif //SERV_EPAY_SYSTEM
					)
					//#endif _SERVICE_
				{
					strDir = szNewSearchDir;
					strDir += L"\\";
					AddDataDirectory( strDir);

					GetSubDirectory( szNewSearchDir);
				}
			}
		}

	}while(FindNextFileW(hSearch, &fd));
#else WCHAR_DIR
	do
	{
		if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
		{
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char	szNewSearchDir[256] = {0,};
				//char	szNewSearchDir[256];
				string	strDir;

#ifdef	CONVERSION_VS
				strncpy_s(szNewSearchDir, _countof(szNewSearchDir), szSearchDir , 255);
#else	CONVERSION_VS
				strncpy(szNewSearchDir, szSearchDir , 255);
#endif	CONVERSION_VS
				
				//strcpy(szNewSearchDir, szSearchDir);
				StringCchCatA(szNewSearchDir, 256, "\\");
				//strcat(szNewSearchDir, "\\");
				StringCchCatA(szNewSearchDir, 256,  fd.cFileName);
				//strcat(szNewSearchDir, fd.cFileName);

//#ifndef _SERVICE_
				if( strcmp( fd.cFileName, ".svn" ) != 0 &&
					strcmp( fd.cFileName, "Branches" ) != 0
//#ifdef SERV_EPAY_SYSTEM //2012.03.22 lygan_조성욱 // 사내에서 epay관련 하위 폴더 경로는 검색 하지 않게 한다.
					&& strcmp( fd.cFileName, "epay" ) != 0 
					&& strcmp( fd.cFileName, "SupportFiles" ) != 0
//#endif //SERV_EPAY_SYSTEM
					)
//#endif _SERVICE_
				{
					strDir = szNewSearchDir;
					strDir += "\\";
					AddDataDirectory( strDir);

					GetSubDirectory( szNewSearchDir);
				}
			}
		}

	}while(FindNextFileA(hSearch, &fd));
#endif WCHAR_DIR

	FindClose(hSearch);

	return true;
}//KGCMassFileManager::GetSubDirectory()


#ifdef WCHAR_DIR
WCHAR* KGCMassFileManager::GetPullPath( WCHAR* cFileName )
#else WCHAR_DIR
char* KGCMassFileManager::GetPullPath( char* cFileName )
#endif WCHAR_DIR
{
//#ifdef	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
	const KTLSData& kTLSData = GetUpdatedTLSData();
#ifdef WCHAR_DIR
	const std::vector<std::wstring>& vecstrDataDirectory = kTLSData.m_vecstrDataDirectory;
#else WCHAR_DIR
	const std::vector<std::string>& vecstrDataDirectory = kTLSData.m_vecstrDataDirectory;
#endif WCHAR_DIR
//#else	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK
//    CSLock  lock( m_csFileManager );
//	const std::vector<std::string>& vecstrDataDirectory = m_vecstrDataDirectory;
//#endif	X2OPTIMIZE_REMOVE_MASS_FILE_MANAGER_LOCK

	FILE* file = NULL;
#ifdef WCHAR_DIR
	std::wstring strName;
#else WCHAR_DIR
	std::string strName;
#endif WCHAR_DIR

	for( int i = 0; i < (int)vecstrDataDirectory.size(); ++i )
	{
		strName.resize(0);
		strName = vecstrDataDirectory[i];
		strName += cFileName;

#ifdef WCHAR_DIR
		_wfopen_s( &file, strName.c_str(), L"rb" );
#else WCHAR_DIR
#ifdef	CONVERSION_VS
		fopen_s( &file, strName.c_str(), "rb" );
#else	CONVERSION_VS
		file = fopen( strName.c_str(), "rb" );
#endif	CONVERSION_VS
#endif WCHAR_DIR
		if(file != NULL)
		{
#ifdef WCHAR_DIR
			wcsncpy_s( cFileName, MAX_PATH, strName.c_str() , MAX_PATH-1);
#else WCHAR_DIR
#ifdef	CONVERSION_VS
			strncpy_s( cFileName, MAX_PATH, strName.c_str() , MAX_PATH-1);
#else	CONVERSION_VS
			strncpy( cFileName, strName.c_str() , MAX_PATH-1);
#endif	CONVERSION_VS
#endif WCHAR_DIR
			
			//strcpy( cFileName, strName.c_str());
			break;
		}
	}

	if( file == NULL)
	{
		return NULL;
	}

	fclose( file );
	return cFileName;
}//KGCMassFileManager::GetPullPath()


//#ifdef BACKGROUND_LOADING_TEST // 2008-10-16

#ifdef WCHAR_DIR
char* KGCMassFileManager::GetPullPath( char* cFileName )
{
	WCHAR strFileName[MAX_PATH] = {0,};
	MultiByteToWideChar( CP_ACP, 0, cFileName, -1, strFileName, ARRAY_SIZE(strFileName) );
	GetPullPath( strFileName );
	WideCharToMultiByte( CP_ACP, 0, strFileName, -1, cFileName, ARRAY_SIZE(strFileName), NULL, NULL );
	return cFileName;
}
#else WCHAR_DIR
WCHAR* KGCMassFileManager::GetPullPath( WCHAR* wcFileName )
{
	char strFileName[MAX_PATH] = {0,};
	WideCharToMultiByte( CP_ACP, 0, wcFileName, -1, strFileName, ARRAY_SIZE(strFileName), NULL, NULL );
	GetPullPath( strFileName );
	MultiByteToWideChar( CP_ACP, 0, strFileName, -1, wcFileName, ARRAY_SIZE(strFileName) );
	return wcFileName;
}//KGCMassFileManager::GetPullPath()
#endif WCHAR_DIR

//#else // BACKGROUND_LOADING_TEST
//
////{{AFX: jintaeks on 2008-10-16, 14:58
//WCHAR* KGCMassFileManager::GetPullPath( WCHAR* wcFileName )
//{
//	char strFileName[255] = {0,};
//	std::string strName;
//
//	WideCharToMultiByte( CP_ACP, 0, wcFileName, -1, strFileName, 255, NULL, NULL );
//
//	FILE* file = NULL;
//
//	for( int i = 0; i < (int)m_vecstrDataDirectory.size(); ++i )
//	{
//		strName = m_vecstrDataDirectory[i] + strFileName;
//
//		file = fopen( strName.c_str(), "rb" );
//		if(file != NULL)
//		{
//			MultiByteToWideChar( CP_ACP, 0, strName.c_str(), -1, wcFileName, 255);
//			break;
//		}
//	}
//
//	if( file == NULL)
//	{
//		return NULL;
//	}
//
//	fclose( file );
//	return wcFileName;
//}//KGCMassFileManager::GetPullPath()
////}}AFX
//#endif // BACKGROUND_LOADING_TEST // 2008-10-16


/*static*/
#ifdef WCHAR_DIR
HANDLE KGCMassFileManager::OpenReadFileHandle( const WCHAR* pszFilename, unsigned uRetryCount, bool bAsync )
#else WCHAR_DIR
HANDLE KGCMassFileManager::OpenReadFileHandle( const char* pszFilename, unsigned uRetryCount, bool bAsync )
#endif WCHAR_DIR
{
	if ( pszFilename == NULL || pszFilename[0] == NULL )
		return INVALID_HANDLE_VALUE;

	HANDLE	hFile = INVALID_HANDLE_VALUE;
	
	for( unsigned u = 0; u <= uRetryCount; u++ )
	{
#ifdef WCHAR_DIR
		hFile = ::CreateFileW( pszFilename,		// file to open
#else WCHAR_DIR
		hFile = ::CreateFileA( pszFilename,		// file to open
#endif WCHAR_DIR
			GENERIC_READ,          				// open for reading
			FILE_SHARE_READ,       				// share for reading
			NULL,                  				// default security
			OPEN_EXISTING,         				// existing file only
			( bAsync == true ) ? ( FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED ) : FILE_ATTRIBUTE_NORMAL, 
			NULL);   

		if ( hFile != INVALID_HANDLE_VALUE )
			return hFile;

		if ( u < uRetryCount )
		{
			char buffer[256];
			sprintf_s( buffer, ARRAY_SIZE(buffer), "open error %d - retrying", u + 1 );
			LeaveLastErrorLog( KEM_ERROR143, pszFilename, buffer );
			::Sleep( 300 );
		}
	}

	LeaveLastErrorLog( KEM_ERROR143, pszFilename, "open error 4" );
	return INVALID_HANDLE_VALUE;
}

/*static*/
#ifdef WCHAR_DIR
BOOL KGCMassFileManager::ReadFile( HANDLE hFile, const WCHAR* pLogInfo, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, DWORD dwOffset
//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//								  , bool bAsync
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	)
#else WCHAR_DIR
BOOL KGCMassFileManager::ReadFile( HANDLE hFile, const char* pLogInfo, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, DWORD dwOffset
//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//	, bool bAsync
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	)
#endif WCHAR_DIR
{
	if ( lpNumberOfBytesRead != NULL )
	{
		*lpNumberOfBytesRead = 0L;
	}//if


//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//	if ( bAsync == false )
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
	{

		DWORD dwSetFilePointer = ::SetFilePointer( hFile, dwOffset, NULL, FILE_BEGIN );

		if( INVALID_SET_FILE_POINTER == dwSetFilePointer )
		{ 
			LeaveLastErrorLog( KEM_ERROR390, pLogInfo, "file pointer error" );
			return FALSE;
		}

		DWORD dwBytesRead = 0;
		if( FALSE == ::ReadFile( hFile, lpBuffer, nNumberOfBytesToRead, &dwBytesRead, NULL )
			|| nNumberOfBytesToRead != dwBytesRead )
		{
			LeaveLastErrorLog( KEM_ERROR390, pLogInfo, "read error" );
			return FALSE;
		}

		if ( lpNumberOfBytesRead != NULL )
		{
			*lpNumberOfBytesRead = dwBytesRead;
		}//if

		return TRUE;

	}
//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//	else
//	{
//		ZeroMemory( &m_overlapped, sizeof(m_overlapped ) );
//		m_overlapped.m_overlapped.Offset = dwOffset;
//		m_overlapped.m_dwNumBytesTransferred = 0;
//		m_overlapped.m_eFinishCode = KGCMASSFILEMANAGER_OVERLAPPED::NOT_FINISHED;
//
//		if ( FALSE == ::ReadFileEx( hFile, lpBuffer, nNumberOfBytesToRead, &m_overlapped.m_overlapped, _CompletionRoutine ) )
//		{
//			LeaveLastErrorLog( KEM_ERROR390, pLogInfo, "read error" );
//			return FALSE;
//		}//if
//		while( m_overlapped.m_eFinishCode == KGCMASSFILEMANAGER_OVERLAPPED::NOT_FINISHED )
//		{
//			::SleepEx( INFINITE, TRUE );
//		}//while
//
//		if ( m_overlapped.m_eFinishCode == KGCMASSFILEMANAGER_OVERLAPPED::FINISHED )
//		{
//			if ( lpNumberOfBytesRead != NULL )
//			{
//				*lpNumberOfBytesRead = m_overlapped.m_dwNumBytesTransferred;
//			}//if
//
//			if ( m_overlapped.m_dwNumBytesTransferred != nNumberOfBytesToRead )
//			{
//				LeaveLastErrorLog( KEM_ERROR390, pLogInfo, "read error" );
//				return FALSE;
//			}//if
//
//			return TRUE;
//		}//if
//
//		LeaveLastErrorLog( KEM_ERROR390, pLogInfo, "read error" );
//		return FALSE;
//
//	}//if.. else..
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO

	return FALSE;

}//KGCMassFileManager::NewReadFile()

//#ifndef	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO
//
//
///*static*/
//void WINAPI KGCMassFileManager::_CompletionRoutine( DWORD dwErrorCode, DWORD dwNumTrans, LPOVERLAPPED lpOverlapped )
//{
//	KGCMASSFILEMANAGER_OVERLAPPED* temp =(KGCMASSFILEMANAGER_OVERLAPPED*)(lpOverlapped);
//	if ( temp == NULL )
//		return;
//
//	if ( dwErrorCode != 0 )
//	{
//		temp->m_dwNumBytesTransferred = 0L;
//		temp->m_eFinishCode = KGCMASSFILEMANAGER_OVERLAPPED::FINISHED_WITH_ERROR;
//	}
//	else
//	{
//		temp->m_dwNumBytesTransferred = dwNumTrans;
//		temp->m_eFinishCode = KGCMASSFILEMANAGER_OVERLAPPED::FINISHED;
//	}//if.. else..
//
//}//KGCMassFileManager::_CompletionRoutine()
//
//#endif	X2OPTIMIZE_MASS_FILE_MANAGER_PER_THREAD_SYNC_IO


//#ifdef	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER
void	KGCMassFileManager::ReleaseAllMemoryBuffers()
{
//#ifdef	BACKGROUND_LOADING_TEST
	m_massFileBufMan.ReleaseAllBuffers();
//#endif	BACKGROUND_LOADING_TEST
}
//#endif	X2OPTIMIZE_MASS_FILE_BUFFER_MANAGER