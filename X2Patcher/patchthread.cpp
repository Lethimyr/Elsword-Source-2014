//#if defined(BUILD_BY_HUDSON)
#include "../hudsondef.h"
//#endif

#include <windows.h>

#include "define.h"
#include "Registry.h"
#include "KMsgBox.h"
#include "ThreadDefs.h"
#include "KAssert.h"
#include "KPatchInfoBox.h"
#include "PatcherStringSet.h"
#include "patch.h"

//-- 지헌 : XTRAP 런쳐 - 해더 포함 부분
// 런쳐
#ifdef LAUNCHER_USE_XTRAP

#include "urlmon.h"
#pragma comment(lib, "urlmon")
#pragma comment(lib, "wininet")
#include "..\\XTRAP\\Launcher\\Xtrap_L_Interface.h"
#if(defined _MT)
#if(defined _DLL)
#pragma comment(lib, "..\\XTRAP\\Launcher\\XTrap4Launcher_mtDll.lib")
#else
#pragma comment(lib, "..\\XTRAP\\Launcher\\XTrap4Launcher_mt.lib")
#endif
#else
#pragma comment(lib, "..\\XTRAP\\Launcher\\XTrap4Launcher_st.lib")
#endif

// XTRAP 런쳐 실행 함수
void StartXTrapLauncher();

#endif


//--------------------------------------------------------------------

extern void Log( const char* szLogMsg );
extern void LogError( const char* szLogMsg );

extern KPatcher g_patcher;
extern HWND g_hWnd;
extern HANDLE g_hMutex;
extern LPSTR g_lpCmdLine;
extern bool g_bUpdating;
extern float g_fTotalDownloadRatio;
extern float g_fFileDownloadRatio;
extern float g_fCheckFileRatio;
extern std::string g_CurFilename;

bool CheckProgress( float p )
{
	g_fCheckFileRatio = p;

	// 전체 퍼센트 업데이트 
	RECT rcAllFileLoadBar;
	rcAllFileLoadBar.bottom	= 451 + 40;
	rcAllFileLoadBar.left	= 160;
	rcAllFileLoadBar.right	= 160 + 40;
	rcAllFileLoadBar.top	= 451;

	::InvalidateRect( g_hWnd, &rcAllFileLoadBar, FALSE );

	if( g_patcher.IsStoped() == true )
	{
		Log( "Exiting Patch Program (패치 프로그램을 종료합니다.)" );
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		return false;
	}

	return true;
}

bool TotalProgress(float p)
{

	/////////////////////////// 2013.09.04 lygan_조성욱 // 해외팀 최신 런처 ///////////////////////////
	g_fTotalDownloadRatio = p;
	RECT rcAllFileLoadBar;
	rcAllFileLoadBar.bottom	= LOAD_NEW_ALL_FILE_BAR_Y + LOAD_NEW_ALL_FILE_BAR_HEIGHT;
	rcAllFileLoadBar.left	= LOAD_NEW_ALL_FILE_BAR_X;
	rcAllFileLoadBar.right	= LOAD_NEW_ALL_FILE_BAR_X + LOAD_NEW_ALL_FILE_BAR_WIDTH;
	rcAllFileLoadBar.top	= LOAD_NEW_ALL_FILE_BAR_Y;

	::InvalidateRect( g_hWnd, &rcAllFileLoadBar, FALSE );
	
	if( g_patcher.IsStoped() == true )
	{
		Log( "Exiting Patch Program (패치 프로그램을 종료합니다.)" );
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		return false;
	}

	return true;
}

bool FileProgress(float p)
{

	/////////////////////////// 2013.09.04 lygan_조성욱 // 해외팀 최신 런처 ///////////////////////////
	g_fFileDownloadRatio = p;
	RECT rcCurFileLoadBar;
	rcCurFileLoadBar.bottom = LOAD_NEW_CUR_FILE_BAR_Y + LOAD_NEW_CUR_FILE_BAR_HEIGHT;
	rcCurFileLoadBar.left	= LOAD_NEW_CUR_FILE_BAR_X;
	rcCurFileLoadBar.right	= LOAD_NEW_CUR_FILE_BAR_X + LOAD_NEW_CUR_FILE_BAR_WIDTH;
	rcCurFileLoadBar.top	= LOAD_NEW_CUR_FILE_BAR_Y;

	::InvalidateRect( g_hWnd, &rcCurFileLoadBar, FALSE );
	::InvalidateRect( g_hWnd, NULL, FALSE );
	return !g_patcher.IsStoped();
}

DWORD WINAPI ThreadProc( LPVOID lpParameter )
{
	std::string patchpath;

	g_fFileDownloadRatio  = 0.f;
	g_fTotalDownloadRatio = 0.f;

	g_fCheckFileRatio = 0.f;

	// 세션 열기 
	if(g_patcher.OpenSession("ELSWORD_PATCHER") == false)
	{
		LogError( "Open Internet Session Fail" );
		MessageBoxW( g_hWnd, STRINGPATCHER04, STRINGPATCHER05, MB_OK );
		PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		return -1;
	}	

	Log( "Opened HTTP Successfully" );

//#if defined(BUILD_BY_HUDSON)
	patchpath = PATCH_PATH;
//#endif

	//{{ 10.01.14 김정협 외부파일로 경로 읽는 부분 추가
#ifdef FOREIGN_CONFIG_FILE
	std::ifstream ifs;
	ifs.open( ".\\FilePath", std::ios::in );
	if( ifs.is_open() )
	{
		CHAR szData[1024];
		std::string strData;

		while( !ifs.eof() )
		{
			ifs >> szData;
			//strData = szData;				
		}
		patchpath = szData;
	}
#endif //FOREIGN_CONFIG_FILE
	//}} 10.01.14 김정협 외부파일로 경로 읽는 부분 추가

	// 다운로드 윈도우 띄우자 
	::ShowWindow( g_hWnd, SW_SHOW );
	::SetWindowPos( g_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
	::SetFocus( g_hWnd );

	std::string strConnectPath = "Connecting to ";
	strConnectPath.append( patchpath );
	strConnectPath.append( " (" );
	strConnectPath.append( patchpath );
	strConnectPath.append(" 에 접속합니다.)");

	Log( strConnectPath.c_str() );
	// 접속 
	int retry = 0;
	while(false ==g_patcher.Connect(patchpath))
	{
		if(MAX_RETRY == ++retry)
		{
			int nPosition = patchpath.find_last_of("/");
			std::string strFileName = patchpath.substr(nPosition + 1, patchpath.length() - nPosition);
			strFileName = "Failed to download PatchPath filename file " + strFileName + "(" + strFileName + " 파일 다운로드에 실패했습니다.)";
			Log( &strFileName[0] );

			MessageBoxW( g_hWnd, STRINGPATCHER06, STRINGPATCHER07, MB_OK );
			PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
			return -1;
		}
		::Sleep( MAX_DELAY );
	}

	//패치인포
	Log( "Version File Download (버전 파일 다운로드)" );
	retry=0;
	while(false ==g_patcher.GetServerPatchInfo())
	{
		g_patcher.Reconnect();
		if(MAX_RETRY == ++retry)
		{
			Log( "Version File Download Unsuccessful (버전 파일 다운로드 실패)" );
			//KMsgBox::Close();
			//KMsgBox::Open( STRINGPATCHER12,
			//	STRINGPATCHER12,
			//	KMsgBox::MBOX_OK );;

			MessageBoxW( g_hWnd, STRINGPATCHER12, STRINGPATCHER12, MB_OK );
			PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
			return -1;
		}
	}	
	Log( "Version File Download Successful (버전 파일 다운로드 성공)" );

#ifdef PATCHER_SELF_PATCH
	Log( "Comparing Patcher (패쳐 비교)" );
	if( g_patcher.CheckPatcherChange() )
	{
		retry=0;
		while(false ==g_patcher.UpdatePatcher())
		{
			if(MAX_RETRY == ++retry)
			{
				Log( "Patcher Download Unsuccessful (패쳐 다운로드 실패)" );

				MessageBoxW( g_hWnd, STRINGPATCHER20, STRINGPATCHER21, MB_OK );
				PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
				return -1;
			}
			::Sleep( MAX_DELAY );
		}
		Log( "Patcher Download Successful (패쳐 다운로드 성공)" );
		
		if ( NULL != g_hMutex )
		{
			::ReleaseMutex( g_hMutex );
			::CloseHandle( g_hMutex );
			g_hMutex = NULL;
		}

		if( false == g_patcher.RunPatcher(g_lpCmdLine))
		{
			Log( "Patcher Execution Unsuccessful (패쳐 실행 실패)" );

			MessageBoxW( g_hWnd, STRINGPATCHER20, STRINGPATCHER21, MB_OK );
			PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
			return -1;
		}
		PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		return -1;
	}
#endif PATCHER_SELF_PATCH

	Log( "Local Data Creation (로컬 정보 생성)");
	if( g_patcher.GetLocalPatchInfo(CheckProgress) == false )
	{
		Log( "Local Data Creation Unsuccessful (로컬 정보 생성 실패)" );

		MessageBoxW( g_hWnd, STRINGPATCHER18, STRINGPATCHER19, MB_OK );
		PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		return -1;
	}
	g_patcher.Diff();

	if( g_patcher.IsPatchComplete() == false )
	{
		g_bUpdating = true;

		while(g_patcher.IsPatchComplete() == false && g_patcher.IsStoped()==false)
		{
			g_CurFilename = g_patcher.GetFirstFileName();
			g_fFileDownloadRatio = 0;
			::InvalidateRect( g_hWnd, NULL, FALSE );
			retry=0;
			while( false == g_patcher.ProcessOneFile( TotalProgress,FileProgress ) )
			{
				g_patcher.Reconnect();
				if(MAX_RETRY == ++retry)
				{
					Log( g_CurFilename.c_str() );
					Log( "File Download Unsuccessful (파일 다운로드에 실패했습니다.)" );

					MessageBoxW( g_hWnd, STRINGPATCHER20, STRINGPATCHER21, MB_OK );
					PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
					return -1;
				}
			}	
		}
		if( true == g_patcher.IsPatchComplete() )
		{
				//g_CurFilename			= STRINGPATCHER35;	/// 패치가 완료 되었습니다.
				g_fFileDownloadRatio	= 1.f;				/// 보기 좋게 다운률 100%로 나올 수 있도록 설정
				g_fTotalDownloadRatio	= 1.f;

				g_fCheckFileRatio		= 1.f;

				::InvalidateRect( g_hWnd, NULL, FALSE );

				::SetFocus( g_hWnd );						/// 완료 되었으니, 포커스 주자
		}

	}

	if(g_patcher.IsStoped() == true)
	{
		Log( "Exiting Patch Program (패치 프로그램을 종료합니다.)" );
		::PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		return 0;
	}
	
	// 클라이언트 실행 
	
	if ( NULL != g_hMutex )
	{
		::ReleaseMutex( g_hMutex );
		::CloseHandle( g_hMutex );
		g_hMutex = NULL;
	}

	// ---- 지헌 : XTRAP 런쳐 - 클라 실행 직전에 XTRAP 런쳐를 구동시킨다-----------------------
#ifdef LAUNCHER_USE_XTRAP
	Log( "About to Call Thread (스래드에서 호출 직전)" );
	StartXTrapLauncher();
#endif // LAUNCHER_USE_XTRAP
	//---------------------------------------------

#ifdef PATCHER_RUN_ONLY
	char charBuf[1024] = {0,};
	memcpy( charBuf, g_lpCmdLine, strlen( g_lpCmdLine ) );
	strcat(charBuf, " ");
	strcat(charBuf, PATCHER_RUN_ONLY );

	if(g_patcher.RunClient("data\\x2.exe",charBuf) == false)
#else //PATCHER_RUN_ONLY

#ifdef PATCHER_RUN_GARENA_MESSENGER
	std::string str = (std::string) __argv[1];
	std::string strCheck = "";
	if (str.length() != 0 && str.length() > 7)
	{
		strCheck = str.substr(7,str.length());
	}
	if(g_patcher.RunClient("data\\x2.exe",strCheck) == false)
#else //PATCHER_RUN_GARENA_MESSENGER
	if(g_patcher.RunClient("data\\x2.exe",g_lpCmdLine) == false)
#endif //PATCHER_RUN_GARENA_MESSENGER
	
#endif //PATCHER_RUN_ONLY
	{
		Log( "Failed To Launch Elsword (엘소드 실행에 실패했습니다.)" );

		MessageBoxW( g_hWnd, STRINGMAIN20, STRINGMAIN21, MB_OK );
		PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
		return -1;
	}

	Log( "Elsword Launch Successful (엘소드 실행에 성공했습니다.)" );
	PostMessage( g_hWnd, WM_CLOSE, 0, 0 );
	return 0;	
}

// 지헌 : XTRAP 테스트 - XTRAP 런쳐 실행 함수
#ifdef LAUNCHER_USE_XTRAP
void StartXTrapLauncher()
{	
	// 클라 실행 직전에 XTRAP 런쳐를 구동시킨다
	// WISELOGIC 지원 팀에게 발급받은 암호
#ifdef _OPEN_TEST_
	// 테섭
	char szXTrapArg[] = "660970B4786BD2AEC5246D9844CFE8628BDB1432DEB010E156CE1D08A21CB097AF4079B7CD358A9DD7AFBA25BC24EF28F651E37E2F245EBFBD8718147A8A52A20F7D04245246E455F727430F8907395F442D6D1B1D42427132E20322641AE92C";
#else _OPEN_TEST_
	// 본섭
	char szXTrapArg[] = "660970B4786BD2AEC5206D9844CFE862F7C086099FC02945DD3FAEDC194D12B64A288B4A54E6F4CA955146191420E3674CF7405C13282585DF02B1886E4C31850F7D04245246E40CAC79010DC9537800A68F313AB50983AB992078D963692A8CD7DC797060F5244ACBF9D10940F8B94FA67CD5767231B9C2EE84061319EC";
#endif _OPEN_TEST_

	XTrap_L_PatchC(szXTrapArg, ".\\data\\", 60, XPATCH_SHOW_OPTION_IMAGE_ALL);
}
#endif //LAUNCHER_USE_XTRAP
//----------------------------------------------------------------