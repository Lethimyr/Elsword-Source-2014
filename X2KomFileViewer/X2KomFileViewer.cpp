// X2KomFileViewer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "X2KomFileViewerDlg.h"
#include "PathDialog.h"
#include "NoticeDlg.h"
#include "SameFileDlg.h"
#include "KomListViewDlg.h"
#include <stdlib.h>
#include <strsafe.h>

using std::wifstream;

//#include <vld.h>
//#define _MODIFY_VERSION_01
#define _MODIFY_CONSOLE_VERSION_01

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


lua_State* g_pLua	= NULL;


// CX2KomFileViewerApp

BEGIN_MESSAGE_MAP(CX2KomFileViewerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

static HWND g_noticeHwnd;

// notice thread
UINT WINAPI NoticeThread( LPVOID arg )
{
	CNoticeDlg noticeDlg;	
	g_noticeHwnd = noticeDlg.m_hWnd;
	noticeDlg.DoModal();
	return 0;
}

// CX2KomFileViewerApp 생성

CX2KomFileViewerApp::CX2KomFileViewerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CX2KomFileViewerApp 개체입니다.

CX2KomFileViewerApp theApp;


// CX2KomFileViewerApp 초기화

BOOL CX2KomFileViewerApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	//lua init
	{
		g_pLua = lua_open();

		if( g_pLua == NULL )
		{
			::MessageBox( NULL, L"루아 생성 실패.!", L"", MB_OK );
			return FALSE;
		}

		//luaopen_base( g_pLua );  // 루아함수 tonumber를 사용해야 한다.
		//luaopen_table( g_pLua );
		//luaopen_io( g_pLua );
		//luaopen_string( g_pLua );
		//luaopen_math( g_pLua );
		//lua_settop( g_pLua, 0 );
        luaL_openlibs( g_pLua );
        lua_settop( g_pLua, 0 );

		//lua_tinker::lua_state::open( g_pLua );
	}

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성한 응용 프로그램"));

	// load lua script
	theApp.m_komFileManager.LoadConfig();

	//theApp.m_komFileManager.InitNewFolderPaths_Lua();//( L"D:\X2Project\X2Project\X2KomFileViewer" );
	// Console Mode
	if( CommandLineExecute() == true )
	{
		lua_close( g_pLua );
		return FALSE;
	}
#ifdef _MODIFY_VERSION_01

	BOOL	bIsOpenKomFileList	= FALSE;
	BOOL	bIsOpenFolderList	= FALSE;
	BOOL	bIsKomParsingOnly	= FALSE;
	HANDLE	hNoticeThread		= 0;
	CPathDialog pathDlg;

	std::map< std::wstring, MemberFileInfo > mapLuaFiles; // 컴파일된 스크립트 파일 리스트	
	std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;	// 중복 파일 리스트

	if( pathDlg.DoModal() != IDOK )
	{
		lua_close( g_pLua );
		return FALSE;
	}

	if ( pathDlg.GetOldPath().empty()  == FALSE )				// 이전 버전의 폴더 경로가 입력 됬으면
	{
		bIsOpenKomFileList	= TRUE;
		theApp.m_komFileManager.m_vecKomFileList.resize(0);
		theApp.m_komFileManager.m_vecKomFileList.push_back( pathDlg.GetOldPath() );
	}

	if ( pathDlg.GetNewPath().empty() == FALSE )				// 새로운 버전의 폴더 경로가 입력 됬으면
	{
		bIsOpenFolderList	= TRUE;
		theApp.m_komFileManager.m_vecFolderList.resize(0);
		theApp.m_komFileManager.m_vecFolderList.push_back( pathDlg.GetNewPath() );
	}

	if ( bIsOpenFolderList == FALSE && bIsOpenKomFileList == FALSE )			// 둘다 열리지 않을 경우
	{
		MessageBox( NULL, L"경로를 입력하지 않았습니다!", L"오류!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}
	else if ( bIsOpenFolderList == FALSE && bIsOpenKomFileList == TRUE )		// kom파일만 열렸을경우(구폴더만)
	{
		bIsKomParsingOnly = TRUE;
		theApp.m_komFileManager.GetKomFileListFromLuaScript();
	}
	else if ( bIsOpenFolderList == TRUE && bIsOpenKomFileList == FALSE )		// 폴더만 열렸을경우(신폴더만)
	{
		hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread, (LPVOID)NULL, NULL, NULL );

		// 중복 파일 체크
		if( theApp.m_komFileManager.CheckSameFiles( mapSameFiles ) == false )
		{
			MessageBox( NULL, L"동일 파일 체크 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		// 동일 파일 리스트 띄우기
		if( mapSameFiles.empty() == false )
		{
			CSameFileDlg sameFileDlg;
			sameFileDlg.SetSameFileList( mapSameFiles );
			sameFileDlg.DoModal();
		}
		theApp.m_komFileManager.GetFolderToCompressListFromLuaScript();
	}
	else																		// 둘다 열렸을 경우
	{
		hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread, (LPVOID)NULL, NULL, NULL );

		// 중복 파일 체크
		if( theApp.m_komFileManager.CheckSameFiles(  mapSameFiles ) == false )
		{
			MessageBox( NULL, L"동일 파일 체크 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		// 동일 파일 리스트 띄우기
		if( mapSameFiles.empty() == false )
		{
			CSameFileDlg sameFileDlg;
			sameFileDlg.SetSameFileList( mapSameFiles );
			sameFileDlg.DoModal();
		}

		theApp.m_komFileManager.GetKomFileAndFolderToCompressListFromLuaScript();
	}

	// 상황알림 대화상자 제거
	if ( hNoticeThread )
	{
		::SendMessage( g_noticeHwnd, WM_CLOSE, 0, 0 );
		TerminateThread( hNoticeThread, 0 );
	}

	CX2KomFileViewerDlg dlg;
	dlg.SetKomParsingOnly( bIsKomParsingOnly );
	m_pMainWnd = &dlg;
	dlg.DoModal();

	lua_close( g_pLua );
	return FALSE;

#endif

#ifndef _MODIFY_VERSION_01
	// 경로 얻기 대화상자 띄우기
	CPathDialog pathDlg;
	if( pathDlg.DoModal() != IDOK )
	{
		lua_close( g_pLua );
		return FALSE;
	}

	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == true )	// 구버전, 신버전 폴더 경로 모두 열지 않았을 때
	{
		MessageBox( NULL, L"경로를 입력하지 않았습니다!", L"오류!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == true )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFile( pathDlg.GetOldPath() ) == false )
		{
			MessageBox( NULL, L"KOM파일 분석 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		// 리스트 대화상자
		CX2KomFileViewerDlg dlg;
		dlg.SetKomParsingOnly( true );
		m_pMainWnd = &dlg;
		dlg.DoModal();

		lua_close( g_pLua );
		return FALSE;
	}

	// 상황알림 쓰레드 실행
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread, (LPVOID)NULL, NULL, NULL );
		
	std::map< std::wstring, MemberFileInfo > mapLuaFiles; // 컴파일된 스크립트 파일 리스트	
	std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;	// 중복 파일 리스트

	// 중복 파일 체크
	if( theApp.m_komFileManager.CheckSameFiles( pathDlg.GetNewPath(), mapSameFiles ) == false )
	{
		MessageBox( NULL, L"동일 파일 체크 실패!", L"오류!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}

	// 동일 파일 리스트 띄우기
	if( mapSameFiles.empty() == false )
	{
		CSameFileDlg sameFileDlg;
		sameFileDlg.SetSameFileList( mapSameFiles );
		sameFileDlg.DoModal();
	}	
	
	// Lua Script Compile
	if( pathDlg.GetLuaScriptCompileFlag() == true )
	{			
		if( theApp.m_komFileManager.CompileLuaScript( pathDlg.GetNewPath(), mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Lua Script Compile 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

	// Lua Script Encrypt
	if( pathDlg.GetLuaScriptCryptFlag() == true )
	{
		if( theApp.m_komFileManager.PrepareTextFileEncrypt( pathDlg.GetNewPath(), mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Text File 암호화 준비 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}

		if( theApp.m_komFileManager.EncryptLuaScript( mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Script File 암호화 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

	// X-File Convert
	if( pathDlg.GetXFileConvertFlag() == true )
	{
		if( theApp.m_komFileManager.Convert_X_File( pathDlg.GetNewPath() ) == false )
		{
			MessageBox( NULL, L"X-File Convert 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

	// Kom 파일 파싱 & 멤버 파일 로드
	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == false )
	{
		// new member file obtain
		if( theApp.m_komFileManager.ParsingMemberFile( pathDlg.GetNewPath(), mapLuaFiles, pathDlg.GetLuaScriptCompileFlag(), pathDlg.GetLuaScriptCryptFlag() ) == false )
		{
			MessageBox( NULL, L"member파일 분석 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == false )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( pathDlg.GetOldPath(), pathDlg.GetNewPath(), mapLuaFiles, pathDlg.GetLuaScriptCompileFlag(), pathDlg.GetLuaScriptCryptFlag() ) == false )
		{
			MessageBox( NULL, L"member파일 분석 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}	

	// 멤버 파일 재편성
	if( theApp.m_komFileManager.ReorganizeMemberFile() == false )
	{
		MessageBox( NULL, L"member파일 재편성 실패!", L"오류!", MB_OK );
		lua_close( g_pLua );
		return FALSE;
	}

	// 상황알림 대화상자 제거
	::SendMessage( g_noticeHwnd, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );

	// 리스트 대화상자
	CX2KomFileViewerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// 프로그램 종료시 컴파일된 Lua Script가 있다면 삭제한다
	theApp.m_komFileManager.RemoveCompiledLuaScript( mapLuaFiles );

	// 프로그램 종료시 Convert된 X-File이 있다면 Restore한다
	if( pathDlg.GetXFileConvertFlag() == true )
	{
		if( theApp.m_komFileManager.Convert_X_File( pathDlg.GetNewPath(), true ) == false )
		{
			MessageBox( NULL, L"X-File Convert Restore 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
#endif
	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	lua_close( g_pLua );
	return FALSE;
}

bool CX2KomFileViewerApp::CommandLineExecute()
{
	// 자동 kom 생성
	int iArgc = __argc;
	WCHAR** pcArgv = __wargv;

	if( iArgc <= 4 ) // 명령인자 4개이상 : ExeFileName, OldPath, NewPath, SavePath
		return false;
	
	KLuaManager luaManager( g_pLua );

	std::wstring wstrOldPath    = pcArgv[1];	// L"C:\\oldFolder";
	std::wstring wstrNewPath    = pcArgv[2];	// L"C:\\newFolder\\Script\\Dialog";
	std::wstring wstrSavePath   = pcArgv[3];	// L"C:\\saveFolder";

	// 명령 인자 넣기
	std::set< std::wstring > setArgv;
	for( int i=4; i < iArgc; i++ )
		setArgv.insert( std::wstring( pcArgv[i] ) );
#ifdef _MODIFY_CONSOLE_VERSION_01

	//{{ kimhc	// 각 패치에 맞는 changelist version 파일 읽어온 후 해당 버전 이후로
	////////////// 업데이트 된 파일 list 얻어오기
	WCHAR drive[_MAX_DRIVE]		= L"";
	WCHAR dir[_MAX_DIR]			= L"";

	wifstream	inVersionFile;
	int			iVersion;

	if ( setArgv.find( std::wstring( L"-internal" ) ) != setArgv.end() )
	{
		std::wstring	wstrVersionPath = L"";
		std::string		strVersionPath = "";

		_wsplitpath( pcArgv[0], drive, dir, NULL, NULL );
		wstrVersionPath += drive;
		wstrVersionPath += dir;
		wstrVersionPath += L"internal_changelist_ver.txt";
		ConvertWCHARToChar( strVersionPath, wstrVersionPath.c_str() );

		inVersionFile.open( strVersionPath.c_str(), wifstream::in );
	}

	if ( setArgv.find( std::wstring( L"-opentest" ) ) != setArgv.end() )
	{
		std::wstring	wstrVersionPath = L"";
		std::string		strVersionPath = "";

		_wsplitpath( pcArgv[0], drive, dir, NULL, NULL );
		wstrVersionPath += drive;
		wstrVersionPath += dir;
		wstrVersionPath += L"opentest_changelist_ver.txt";
		ConvertWCHARToChar( strVersionPath, wstrVersionPath.c_str() );

		inVersionFile.open( strVersionPath.c_str(), wifstream::in );
	}

	if ( inVersionFile.is_open() == true )
	{
		WCHAR			skipString[10]				= L"";	// "change " 라는 단어를 스킵
		WCHAR			wstrChangeListVersion[10]	= L"";	// 파싱된 iVersion을 담음
		std::wstring	wstrCommand;
		std::string		strCommand;
		const std::wstring& wstrDepotPath = theApp.m_komFileManager.GetDepotPath();;

		inVersionFile >> skipString >> iVersion;
		inVersionFile.close();

		wstrCommand = L"svn  info -r base ";
		wstrCommand += wstrDepotPath;
		wstrCommand += L" | findstr Revision > latest_version.txt";
		ConvertWCHARToChar( strCommand, wstrCommand.c_str() );
		system( strCommand.c_str() );

		//MessageBox( NULL, wstrCommand.c_str(), NULL, MB_OK );

		std::wstring	wstrVersionPath = L"";
		std::string		strVersionPath = "";

		_wsplitpath( pcArgv[0], drive, dir, NULL, NULL );
		wstrVersionPath += drive;
		wstrVersionPath += dir;
		wstrVersionPath += L"latest_version.txt";
		ConvertWCHARToChar( strVersionPath, wstrVersionPath.c_str() );

		//MessageBox( NULL, wstrVersionPath.c_str(), NULL, MB_OK );

		wifstream inNewVersionFile;
		inNewVersionFile.open( strVersionPath.c_str(), wifstream::in );

		if ( inNewVersionFile.is_open() != true )
		{
			ASSERT( false );
			return false;
		}

		int iNewVer;
		
		inNewVersionFile >> skipString >> iNewVer;
		inNewVersionFile.close();

		
		wstrCommand	= L"svn diff -r ";
		_itow( iVersion, wstrChangeListVersion, 10 ) ;
		wstrCommand += wstrChangeListVersion;
		wstrCommand += L":";
		_itow( iNewVer, wstrChangeListVersion, 10 ) ;
		wstrCommand += wstrChangeListVersion;
		wstrCommand += L" ";
		wstrCommand += wstrDepotPath;
		wstrCommand += L" --summarize > updated_files.txt";

		//MessageBox( NULL, wstrCommand.c_str(), NULL, MB_OK );
		//wstrCommand +=

		//wstrCommand += L"p4 diff2 -q //depot/ElswordNew/X2/dat/...@";
		//_itow( iVersion, wstrChangeListVersion, 10 ) ;
		//wstrCommand += wstrChangeListVersion;
		//wstrCommand += L" ";
		//wstrCommand += theApp.m_komFileManager.GetDepotPath();
		//wstrCommand += L"#have";
		//wstrCommand += L"| more > ";	// "l 는 or 기호임"
		//wstrCommand += L"updated_files.txt";
		ConvertWCHARToChar( strCommand, wstrCommand.c_str() );
		
		system( strCommand.c_str() );

		// set에 변경된 폴더 경로들을 담기
		wifstream	wifUpdatedFiles;

		wifUpdatedFiles.open( "updated_files.txt", wifstream::in );

		if ( wifUpdatedFiles.is_open() == true )
		{
			WCHAR wstrOneLine[MAX_PATH];
			WCHAR wstrFilePath[MAX_PATH];
			WCHAR *pHeadToFilePath			= NULL;
			//WCHAR *pTailToFilePath			= NULL;
			
			std::set< std::wstring > setUpdatedFolders;

			while ( wifUpdatedFiles.good() == true )
			{
				wifUpdatedFiles.getline( wstrOneLine, MAX_PATH );

				if ( StrCmpW( wstrOneLine, L"" ) != 0 )
				{
					pHeadToFilePath = StrStrIW( wstrOneLine, L"dat\\" ) + 4;	// 4는 'dat/'의 길이
					//pTailToFilePath = StrStrIW( pHeadToFilePath, L"#" );

					//StringCchCopyNW( wstrFilePath, MAX_PATH - 1, pHeadToFilePath, pTailToFilePath - pHeadToFilePath );

					_wsplitpath( pHeadToFilePath, NULL, dir, NULL, NULL );

					if ( setUpdatedFolders.find( std::wstring( dir ) ) == setUpdatedFolders.end() )
						setUpdatedFolders.insert( std::wstring( dir ) );
				}
			}

			WCHAR wstrFolderPath[_MAX_DIR];

			std::set< std::wstring >::iterator sItrUpdatedFolders = setUpdatedFolders.begin();

			for ( ; sItrUpdatedFolders != setUpdatedFolders.end(); sItrUpdatedFolders++ )
			{
				WCHAR*			pHeadToToken	= NULL;
				std::wstring	wstrPath		= wstrNewPath;

				wstrPath += L"\\";

				StringCchCopyW( wstrFolderPath, _MAX_DIR, sItrUpdatedFolders->c_str() );
							
				pHeadToToken = wcstok( wstrFolderPath, L"/" );

				if ( pHeadToToken != NULL )
					wstrPath += pHeadToToken;

				while ( pHeadToToken != NULL )
				{
					pHeadToToken = wcstok( NULL, L"/" );

					if ( pHeadToToken != NULL)
					{
						wstrPath += L"\\";
						wstrPath += pHeadToToken;
					}
				}

				theApp.m_komFileManager.m_vecPathListToCompress.push_back( wstrPath );
				
			}
		}
	}

	//{{ hyunchul // 2009-03-31, 14:02
	// theApp.m_komfileManager.m_vecKomFileList에 wstrOldPath를 넣는다.
	// InitNewFolderPaths.lua 로 부터 NewFolder들의 경로를 theApp.m_komFileManager.m_vecFolderList로 읽어들인다.
	theApp.m_komFileManager.m_vecKomFileList.push_back( wstrOldPath );

	std::vector< std::wstring > vecNewFolderList = theApp.m_komFileManager.GetNewFolderList();
	theApp.m_komFileManager.m_vecFolderList.resize(0);
	std::vector< std::wstring >::iterator vItr = vecNewFolderList.begin();
	std::wstring wstrTempPath;
	
	for ( ; vItr != vecNewFolderList.end(); vItr++ )
	{
		wstrTempPath = wstrNewPath;
		wstrTempPath += L"\\";
		wstrTempPath += (*vItr);

		theApp.m_komFileManager.m_vecFolderList.push_back( wstrTempPath );

	}
	//}} hyunchul // 2009-03-31, 14:02


	// 중복 파일 체크(같은 파일명)
	if( setArgv.find( std::wstring( L"-s" ) ) != setArgv.end() )
	{
		std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;
		//if( theApp.m_komFileManager.CheckSameFiles( wstrNewPath, mapSameFiles ) == false )
		//{{ hyunchul // 2009-03-31, 14:50
		if ( theApp.m_komFileManager.CheckSameFiles( mapSameFiles ) == false )
		{
			MessageBox( NULL, L"동일 파일 체크 실패!", L"오류!", MB_OK );
			return true;
		}
		//}} hyunchul // 2009-03-31, 14:50

		if( mapSameFiles.empty() == false )
		{
			MessageBox( NULL, L"이름이 중복된 파일이 있음!", L"오류!", MB_OK );
			return true;
		}
	}

	bool bIsLuaCompile = false;
	bool bIsEncrypt = false;
	std::map< std::wstring, MemberFileInfo > mapLuaFiles;

	//{{ hyunchul // 2009-03-31, 15:11
	// 수정 사항: NewPath가 여러개 입력 받을 수 있게 되었으므로 vecNewFolderList의 갯수 만큼 loop
	// Lua Script Compile
	if( setArgv.find( std::wstring( L"-c" ) ) != setArgv.end() )
	{
		bIsLuaCompile = true;
	
		if( theApp.m_komFileManager.CompileLuaScript( theApp.m_komFileManager.m_vecFolderList, mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Lua Script Compile 실패!", L"오류!", MB_OK );
			return true;
		}
   

	}


	// Lua Script Encrypt
	if( setArgv.find( std::wstring( L"-e" ) ) != setArgv.end() )
	{
		bIsEncrypt = true;
		if( theApp.m_komFileManager.PrepareTextFileEncrypt( theApp.m_komFileManager.m_vecFolderList, mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Text File 암호화 준비 실패!", L"오류!", MB_OK );
			return true;
		}

		if( theApp.m_komFileManager.EncryptLuaScript( mapLuaFiles ) == false )
		{
			MessageBox( NULL, L"Script File 암호화 실패!", L"오류!", MB_OK );
			return true;
		}
	}

	// X-File Convert
	if( setArgv.find( std::wstring( L"-x" ) ) != setArgv.end() )
	{
		if( theApp.m_komFileManager.Convert_X_File( theApp.m_komFileManager.m_vecFolderList ) == false )
		{
			MessageBox( NULL, L"X-File Convert 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
	//}} hyunchul // 2009-03-31, 15:11

	// Kom And Member File Parsing or new member file obtain
	if( setArgv.find( std::wstring( L"-k" ) ) != setArgv.end() )
	{
		//if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( wstrOldPath, wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		if ( theApp.m_komFileManager.GetKomFileAndFolderToCompressListFromLuaScript( vecNewFolderList, theApp.m_komFileManager.m_vecKomNameList, bIsLuaCompile, bIsEncrypt, mapLuaFiles ) == FALSE )
		{
			MessageBox( NULL, L"kom & member파일 분석 실패!", L"오류!", MB_OK );
			return true;
		}		
	}
	else
	{
		//if( theApp.m_komFileManager.ParsingMemberFile( wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		if ( theApp.m_komFileManager.GetFolderToCompressListFromLuaScript( vecNewFolderList, theApp.m_komFileManager.m_vecKomNameList ) == FALSE )
		{
			MessageBox( NULL, L"member파일 분석 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}

#endif

#ifndef _MODIFY_CONSOLE_VERSION_01
	
	// 중복 파일 체크
	if( setArgv.find( std::wstring( L"-s" ) ) != setArgv.end() )
	{
		std::map< std::wstring, std::vector< std::wstring > > mapSameFiles;
		if( theApp.m_komFileManager.CheckSameFiles( wstrNewPath, mapSameFiles ) == false )
		{
			MessageBox( NULL, L"동일 파일 체크 실패!", L"오류!", MB_OK );
			return true;
		}
		
		if( mapSameFiles.empty() == false )
		{
			MessageBox( NULL, L"이름이 중복된 파일이 있음!", L"오류!", MB_OK );
			return true;
		}
	}

	bool bIsLuaCompile = false;
	bool bIsEncrypt = false;
	std::map< std::wstring, MemberFileInfo > mapLuaFiles;

	// Lua Script Compile
	if( setArgv.find( std::wstring( L"-c" ) ) != setArgv.end() )
	{
	bIsLuaCompile = true;

	if( theApp.m_komFileManager.CompileLuaScript( wstrNewPath, mapLuaFiles ) == false )
	{
	MessageBox( NULL, L"Lua Script Compile 실패!", L"오류!", MB_OK );
	return true;
	}
	}

	// Lua Script Encrypt
	if( setArgv.find( std::wstring( L"-e" ) ) != setArgv.end() )
	{
	bIsEncrypt = true;
	if( theApp.m_komFileManager.PrepareTextFileEncrypt( wstrNewPath, mapLuaFiles ) == false )
	{
	MessageBox( NULL, L"Text File 암호화 준비 실패!", L"오류!", MB_OK );
	return true;
	}

	if( theApp.m_komFileManager.EncryptLuaScript( mapLuaFiles ) == false )
	{
	MessageBox( NULL, L"Script File 암호화 실패!", L"오류!", MB_OK );
	return true;
	}
	}

	// X-File Convert
	if( setArgv.find( std::wstring( L"-x" ) ) != setArgv.end() )
	{
	if( theApp.m_komFileManager.Convert_X_File( wstrNewPath ) == false )
	{
	MessageBox( NULL, L"X-File Convert 실패!", L"오류!", MB_OK );
	lua_close( g_pLua );
	return FALSE;
	}
	}

	// Kom And Member File Parsing or new member file obtain
	if( setArgv.find( std::wstring( L"-k" ) ) != setArgv.end() )
	{
		if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( wstrOldPath, wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		{
			MessageBox( NULL, L"kom & member파일 분석 실패!", L"오류!", MB_OK );
			return true;
		}		
	}
	else
	{
		if( theApp.m_komFileManager.ParsingMemberFile( wstrNewPath, mapLuaFiles, bIsLuaCompile, bIsEncrypt ) == false )
		{
			MessageBox( NULL, L"member파일 분석 실패!", L"오류!", MB_OK );
			lua_close( g_pLua );
			return FALSE;
		}
	}
	
	// Reorgnize
	if( theApp.m_komFileManager.ReorganizeMemberFile() == false )
	{
	MessageBox( NULL, L"member파일 재편성 실패!", L"오류!", MB_OK );
	return true;
	}
	
#endif

	// Is MultiThread?
	bool bIsMultiThreadPacking = false;
	if( setArgv.find( std::wstring( L"-m" ) ) != setArgv.end() )
	{
		bIsMultiThreadPacking = true;
	}

	// Kom 파일 갯수 알려주기
	if( setArgv.find( std::wstring( L"-t" ) ) != setArgv.end() )
	{
		//{{ hyunchul // 2009-04-02, 10:53
		std::map< std::wstring, std::vector< MemberFileInfo > >& mapTemp = theApp.m_komFileManager.GetMapNewMemberFile();
		CKomListViewDlg komListViewDlg( mapTemp );
		komListViewDlg.DoModal();
		//}} hyunchul // 2009-04-02, 10:53
	}
	else
	{
		if ( theApp.m_komFileManager.m_vecPathListToCopy.empty() == false )		// 복사할 kom 파일들이 있다면
		{
			//std::vector< std::wstring >::iterator vItrPathListToCopy = theApp.m_komFileManager.m_vecPathListToCopy.begin();
			std::wstring	wstrCommandNotChanged;
			std::wstring	wstrCommand;
			std::string		strCommand;

			wstrCommandNotChanged		= L"copy ";
			wstrCommandNotChanged		+= wstrOldPath;
			wstrCommandNotChanged		+= L"\\";

			for ( UINT i = 0; i < theApp.m_komFileManager.m_vecPathListToCopy.size(); i++ )
			{
				wstrCommand = wstrCommandNotChanged;
				wstrCommand += theApp.m_komFileManager.m_vecPathListToCopy[i];
				wstrCommand += L" ";
				wstrCommand += wstrSavePath;
				wstrCommand += L"\\";
				wstrCommand += theApp.m_komFileManager.m_vecPathListToCopy[i];

				ConvertWCHARToChar( strCommand, wstrCommand.c_str() );

				system( strCommand.c_str() );
				
			}

			// 최신 changelist version 다시 저장 하기(기억해놓기)

			const std::wstring& wstrDepotPath = theApp.m_komFileManager.GetDepotPath();

			if ( setArgv.find( std::wstring( L"-internal" ) ) != setArgv.end() )
			{
				wstrCommand = L"svn  info -r base ";
				wstrCommand += wstrDepotPath;
				wstrCommand += L" | findstr Revision > internal_changelist_ver.txt";

				//MessageBox( NULL, wstrCommand.c_str(), NULL, MB_OK );
				
				//wstrCommand = L"p4 changes -m1 ";
				//wstrCommand += theApp.m_komFileManager.GetDepotPath();
				//wstrCommand += L"#have";
				//wstrCommand += L"| more > ";	// "l 는 or 기호임"
				//wstrCommand += L"internal_changelist_ver.txt";
			}

			if ( setArgv.find( std::wstring( L"-opentest" ) ) != setArgv.end() )
			{
				wstrCommand = L"svn  info -r base ";
				wstrCommand += wstrDepotPath;
				wstrCommand += L" | findstr Revision > internal_changelist_ver.txt";

				//wstrCommand = L"p4 changes -m1 ";
				//wstrCommand += theApp.m_komFileManager.GetDepotPath();
				//wstrCommand += L"#have";
				//wstrCommand += L"| more > ";	// "l 는 or 기호임"
				//wstrCommand += L"opentest_changelist_ver.txt";
			}

			ConvertWCHARToChar( strCommand, wstrCommand.c_str() );

			system( strCommand.c_str() );
		}

		// Create Kom File
		if( theApp.m_komFileManager.CreateKomFile( wstrSavePath, bIsMultiThreadPacking ) == false )
		{
			MessageBox( NULL, L"kom파일 생성 실패!", L"오류!", MB_OK );
			return true;
		}
	}

	// 프로그램 종료시 컴파일된 Lua Script가 있다면 삭제한다
	theApp.m_komFileManager.RemoveCompiledLuaScript( mapLuaFiles );

	// 프로그램 종료시 Convert된 X-File이 있다면 Restore한다
	if( setArgv.find( std::wstring( L"-x" ) ) != setArgv.end() )
	{
		if( theApp.m_komFileManager.Convert_X_File( wstrNewPath, true ) == false )
		{
			MessageBox( NULL, L"X-File Convert Restore 실패!", L"오류!", MB_OK );
			return true;
		}
	}

	// 정상 종료
	return true;
}
