//--------------------------------------------------------------------------------------
// File: X2.cpp
//
// Basic starting point for new Direct3D samples
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
 


// wiselog 남기는 위치
//게임 맨 처음 시작했을 때							chceck1
//방화벽 예외 처리하고, 버전파일 체크하고 난 후 	chceck2
//100% 로딩 시작하면서 								chceck3
//100% 로딩 끝난 직후 								chceck4
//게임서버에 계정인증 직후							chceck5	




#include "dxstdafx.h"
#include "resource.h"

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 

#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment( lib, "KTDXLIBD.lib" )
	#pragma comment( lib, "X2LibD.lib" )
#else
	#pragma comment( lib, "KTDXLIB.lib" )
	#pragma comment( lib, "X2Lib.lib" )
#endif

#ifdef CLIENT_PURPLE_MODULE	// 임규수 일본 추가
#include "../X2Lib/OnlyGlobal/JP/Auth/PurpleForClient.h"
#endif // CLIENT_PURPLE_MODULE

#ifdef GAMEGUARD_AUTO_UPDATE
	#pragma comment( lib, "libboost_system-vc100-mt-1_44.lib" )
	#pragma comment( lib, "libboost_filesystem-vc100-mt-1_44.lib" )
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/assign.hpp>
#endif // GAMEGUARD_AUTO_UPDATE

#ifdef SERV_COUNTRY_PH
#include "../KNCSDK/Include/cryptopp/rsa.h"
#endif //SERV_COUNTRY_PH

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
bool                    g_bShowHelp = true;     // If true, it renders the UI control text
#ifndef  REMOVE_DXUT_SAMPLE_CODE
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
CDXUTDialog             g_HUD;                  // dialog for standard controls
CDXUTDialog             g_SampleUI;             // dialog for sample specific controls
#endif  REMOVE_DXUT_SAMPLE_CODE

LPDIRECT3DDEVICE9		g_pd3dDevice;
LPDIRECT3D9				g_pD3D;
CX2SMTPMail*			g_pX2SMTPMail = NULL;
HANDLE					g_hMutex = NULL;		// Mutex of game client program.
//{{ robobeg : 2008-10-10
CKTDGEffectStateManager* g_pEffectStateManager = NULL;
//}} robobeg : 2008-10-10


static bool g_bRenderEtc = false;	// toggle fullscreen, 따위의 버튼을 렌더링 할지 말지

static float	g_fLastUpdateTime_MemInfo = 10000.f;
DWORD			g_iAvailableTextureMem = 0;
DWORD			g_iTotalTextureMem = 0;
DWORD			g_iAvailableTextureMem2 = 0;
MEMORYSTATUS	g_MemStatus;

double			g_fTotalFrame = 0.0f;
int				g_RenderCount = 0;

#ifndef _SERVICE_
#ifdef CHECK_PROCESS_MEMORY_USAGE
typedef struct _PROCESS_MEMORY_COUNTERS_EX {
	DWORD cb;
	DWORD PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivateUsage;
} PROCESS_MEMORY_COUNTERS_EX;
typedef PROCESS_MEMORY_COUNTERS_EX *PPROCESS_MEMORY_COUNTERS_EX;
#endif CHECK_PROCESS_MEMORY_USAGE
#endif _SERVICE_
//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3



//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
#ifndef REMOVE_DXUT_SAMPLE_CODE
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
#endif  REMOVE_DXUT_SAMPLE_CODE
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
void    RenderText(float fElapsedTime);

#ifdef OBTAIN_SEDEBUG_PRIVILEGE
bool	ObtainSeDebugPrivilege();
#endif OBTAIN_SEDEBUG_PRIVILEGE


//{{ 허상형 : [2009/8/4] //	핵실드
#ifdef HACK_SHIELD
bool InitializeHackShield()
{
	// 	핵실드 초기화 및 구동

#ifdef THEMIDA_BY_TOOL_TEAM
	int hackingCheckVariable; 
#endif THEMIDA_BY_TOOL_TEAM



	THEMIDA_VM_START 

	int iRet = HS_Init();


	if(iRet != HS_ERR_OK)
	{
		//	에러 출력 후 종료
		//ShowErrorMsg( iRet );
		return false;
	}

	//	핵쉴드가 시작되었을 경우 비정상 종료시에도 핵쉴드를 종료 시킬 수 있게 설정
	::SetUnhandledExceptionFilter( GAME_UnhandledExceptionHandler );

	THEMIDA_CHECK_PROTECTION( hackingCheckVariable, 0xabc231d8 ) 

	iRet = HS_StartService();


	THEMIDA_REPORT_IF_TAMPERED( hackingCheckVariable, 0xabc231d8, ANTI_HACK_WSTRING_BYPASS_HACKSHIELD )

	if(iRet != HS_ERR_OK)
	{
		//	에러 출력 후 종료
		//ShowErrorMsg( iRet );
		HS_UnInit();
		return false;
	}

#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
	if( _AhnHS_CheckHackShieldRunningStatus()  != HS_ERR_OK )
	{
		MessageBoxW( g_pKTDXApp->GetHWND(), L"핵실드가 정상 동작하지 않습니다.", L"HackShield", MB_OK );
		HS_UnInit();
		return false;
	}
#endif


	THEMIDA_VM_END

	return true;

}


void FinalizeHackShield()
{

	// 핵실드 정지 및 끝내기


	THEMIDA_VM_START 

	int iRet = HS_StopService();
	if(iRet != HS_ERR_OK)
	{
		//	에러 출력
		//ShowErrorMsg( iRet );
		HS_UnInit();
	}

	iRet = HS_UnInit();
	if(iRet != HS_ERR_OK)
	{
		//	에러 출력
		//ShowErrorMsg( iRet );
	}

	THEMIDA_VM_END
}

#ifdef HACKSHIELD_AUTO_UPDATE 
bool UpdateHackShield()
{
	// 	핵실드 업데이트
#ifdef THEMIDA_BY_TOOL_TEAM
	int hackingCheckVariable; 
#endif THEMIDA_BY_TOOL_TEAM

	// 핵실드 업데이트 전에 업데이트를 위한 파일들이 제대로 준비되어 있는지 체크 //
	//HS_PrepareUpdate();

	THEMIDA_VM_START 

	int iRet;

	THEMIDA_CHECK_PROTECTION( hackingCheckVariable, 0x5fdde064 ) 

	iRet = HS_Update();

	THEMIDA_REPORT_IF_TAMPERED( hackingCheckVariable, 0x5fdde064, ANTI_HACK_WSTRING_BYPASS_HACKSHIELD )

	if(iRet != ERROR_SUCCESS)
	{
		//	에러 출력 후 종료
		//ShowErrorMsg( iRet );
		return false;
	}

	THEMIDA_VM_END

	return true;
}
#endif HACKSHIELD_AUTO_UPDATE

//{{ 2011.3.21	조효진	핵쉴드 모니터링 서버 연동
#ifdef HACKSHIELD_MORNITORING
bool StartMonitorHackShield()
{
	// 	핵실드 업데이트
#ifdef THEMIDA_BY_TOOL_TEAM
	int hackingCheckVariable; 
#endif THEMIDA_BY_TOOL_TEAM

	THEMIDA_VM_START 

	int iRet;

	THEMIDA_CHECK_PROTECTION( hackingCheckVariable, 0x7c273dfc ) 

	iRet = HS_StartMonitor();

	THEMIDA_REPORT_IF_TAMPERED( hackingCheckVariable, 0x7c273dfc, ANTI_HACK_WSTRING_BYPASS_HACKSHIELD )

	if(iRet != ERROR_SUCCESS)
	{
		//	에러 출력 후 종료
		//ShowErrorMsg( iRet );
		return false;
	}

	THEMIDA_VM_END

	return true;
}
#endif HACKSHIELD_MORNITORING
//}}
#endif HACK_SHIELD
//}} 허상형 : [2009/8/4] //	핵실드

#ifdef CLIENT_USE_XTRAP	// XTRAP 클라이언트 - 함수들
// XTRAP 클라 실행
void StartXTrap()
{
	// WISELOGIC 지원 팀에게 발급받은 암호

#ifdef _SERVICE_
#ifdef _OPEN_TEST_
	char szXTrapArg[] = "660970B4786BD2AEC5246D9844CFE8628BDB1432DEB010E156CE1D08A21CB097AF4079B7CD358A9DD7AFBA25BC24EF28F651E37E2F245EBFBD8718147A8A52A20F7D04245246E455F727430F8907395F442D6D1B1D42427132E20322641AE92C";
#else _OPEN_TEST_
	char szXTrapArg[] = "660970B4786BD2AEC5206D9844CFE862F7C086099FC02945DD3FAEDC194D12B64A288B4A54E6F4CA955146191420E3674CF7405C13282585DF02B1886E4C31850F7D04245246E40CAC79010DC9537800A68F313AB50983AB992078D963692A8CD7DC797060F5244ACBF9D10940F8B94FA67CD5767231B9C2EE84061319EC";
#endif _OPEN_TEST_
#endif _SERVICE_

#ifdef _IN_HOUSE_// 사내 테스트
	char szXTrapArg[] = "660970B4786BD2AEC5206D9844CFE862F7C086099FC02945DD3FAEDC194D12B64A288B4A54E6F4CA955146191420E3674CF7405C13282585DF02B1886E4C31850F7D04245246E40CAC79010DC9537800A68F313AB50983AB992078D963692A8CD7DC797060F5244ACBF9D10940F8B94FA67CD5767231B9C2EE84061319EC";
#endif

	// XTRAP 클라 실행
	XTrap_C_Start((LPCSTR)szXTrapArg, NULL);
}
// XTRAP 클라 실행 확인
void KeepAliveXTrap()
{
	// XTRAP 정상 초기화 되었는지 확인하는 부분
	XTrap_C_KeepAlive();
}
#endif CLIENT_USE_XTRAP

#ifdef GAMEGUARD_AUTO_UPDATE
void CopyGameGuardFiles()
{
#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif
	using namespace boost::assign;
	std::vector<wstring> vecGameGuardFiles;

	// 해외팀 각국가 전용 디파인
#if defined( CLIENT_COUNTRY_CN )
	vecGameGuardFiles += L"ElswordCN.ini", L"ElswordCNTest.ini", L"GameGuard.des";
#elif defined( CLIENT_COUNTRY_TH )
	vecGameGuardFiles += L"ElswordTH.ini", L"ElswordTHTest.ini", L"GameGuard.des";
#elif defined( CLIENT_COUNTRY_JP )
	vecGameGuardFiles += L"ElswordJP.ini", L"ElswordJPTest.ini", L"GameGuard.des";
#elif defined( CLIENT_COUNTRY_ID )
	vecGameGuardFiles += L"ElswordID.ini", L"ElswordIDTest.ini", L"GameGuard.des";
#elif defined( CLIENT_COUNTRY_BR )
	vecGameGuardFiles += L"ElswordBR.ini", L"ElswordBRTest.ini", L"GameGuard.des";
#endif

	BOOST_FOREACH(wstring file, vecGameGuardFiles)
	{
		const std::wstring wstrSourcePath = L"GameGuard\\";
		const std::wstring wstrDestinationPath = L".\\";

		try
		{
			if(boost::filesystem::exists(wstrDestinationPath+file) == false)
				boost::filesystem::copy_file(wstrSourcePath+file, wstrDestinationPath+file);
		}
		catch (exception& e)
		{
			// 게임가드 측에서 자동 차단되므로 카피가 안되도 실행되도록 별다른 처리는 하지 않음
			assert(!"GameGuard 파일 복사 실패");
			TRACEA(e.what());
		}
	}

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif
}
#endif //GAMEGUARD_AUTO_UPDATE

#ifdef SERV_EPAY_SYSTEM
DWORD EpayCheckRunProces(LPCWSTR clientname)
{

	CStringW wstrTemp(clientname);
	wstrTemp.MakeUpper();

	CStringW wstrFileName;

	PROCESSENTRY32 Process32;
	HANDLE hnd;
	Process32.dwSize = sizeof(PROCESSENTRY32);

	hnd = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if(Process32First(hnd, &Process32) == false)
		return FALSE;

	do 
	{
		wstrFileName = Process32.szExeFile;
		wstrFileName.MakeUpper();
		if(wstrTemp == wstrFileName)
		{

			wstrFileName.Empty();
			return Process32.th32ProcessID;
		}
	} while (Process32Next(hnd, &Process32));

	return FALSE;

}

DWORD EpayCheckRunProcesCount(LPCWSTR clientname, OUT int& iCheckProcesCount)
{

	CStringW wstrTemp(clientname);
	wstrTemp.MakeUpper();

	CStringW wstrFileName;

	PROCESSENTRY32 Process32;
	HANDLE hnd;
	Process32.dwSize = sizeof(PROCESSENTRY32);

	hnd = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	iCheckProcesCount = 0;

	if(Process32First(hnd, &Process32) == false)
		return FALSE;

	do 
	{
		wstrFileName = Process32.szExeFile;
		wstrFileName.MakeUpper();
		if(wstrTemp == wstrFileName)
		{

			wstrFileName.Empty();
			iCheckProcesCount ++;
			//return Process32.th32ProcessID;
		}
	} while (Process32Next(hnd, &Process32));

	return TRUE;

}

bool EPayPrepareUpdate() // epay 관련 업데이트 가능하도록 하는 함수
{

	WCHAR	wszFullFilePath[MAX_PATH] = L"";
	WCHAR	wszFilePath[MAX_PATH] = L"";
	WCHAR	wszDriveName[10] = L"";
	WCHAR	wszMsg[MAX_PATH] = L"";

	string tempNameFrome = "";
	string tempNameTo = "";

	//	실행파일 위치 얻어오기
	GetModuleFileNameW( NULL, wszFullFilePath, MAX_PATH );
	_wsplitpath(wszFullFilePath, wszDriveName, wszFilePath, NULL, NULL);

	wcscpy( wszFullFilePath, L"" );

	// Data 폴더 패스 
	std::wstring wstrDataFolderFullPath(wszFullFilePath);

	std::wstring wstrSupportFolderFullPath(wszFullFilePath);
	wstrSupportFolderFullPath += L".\\SupportFiles";

	std::wstring wstrEpayFolderFullPath(wszFullFilePath);
	wstrEpayFolderFullPath += L".\\epay";

	//ConvertWCHARToChar(tempNameFrome,wstrSupportFolderFullPath);
	//ConvertWCHARToChar(tempNameTo,wstrEpayFolderFullPath);

	boost::filesystem::wpath full_pathBefore( boost::filesystem::initial_path<boost::filesystem::wpath>() ); 
	full_pathBefore = wstrSupportFolderFullPath;

	boost::filesystem::wpath full_pathAfter( boost::filesystem::initial_path<boost::filesystem::wpath>() ); 
	full_pathAfter = wstrEpayFolderFullPath;

	if( false == boost::filesystem::is_directory(wstrSupportFolderFullPath) ) // SupportFiles 폴더 자체가 없는 경우
	{
		return false;
	}

	int ix2RunCount = 0;

	EpayCheckRunProcesCount( L"x2.exe", ix2RunCount );

	if ( ix2RunCount >= 2 )
		return false;

	if( EpayCheckRunProces(L"autoupdate.exe") != FALSE || EpayCheckRunProces(L"epay.exe") != FALSE || EpayCheckRunProces(L"epayupdate.exe") != FALSE )
	{
		return false;
	}

	if( false == boost::filesystem::is_directory(wstrEpayFolderFullPath) ) // 폴더 자체가 없는 경우
	{
		// epay 폴더 생성
		boost::filesystem::create_directory(wstrEpayFolderFullPath);
	}

	wstring wstrAutoUpdateProCheck = wstrEpayFolderFullPath + L"/autoupdate.exe";
	wstring wstrAutoUpdateXmlCheck = wstrEpayFolderFullPath + L"/autoupdate.xml";

	wstring wstrAutoUpdateProCheckSupport = wstrSupportFolderFullPath + L"/autoupdate.exe";
	wstring wstrAutoUpdateXmlCheckSupport = wstrSupportFolderFullPath + L"/autoupdate.xml";

	boost::filesystem::remove( wstrAutoUpdateProCheck );
	boost::filesystem::remove( wstrAutoUpdateXmlCheck );

	boost::filesystem::copy_file( wstrAutoUpdateProCheckSupport, wstrAutoUpdateProCheck );
	boost::filesystem::copy_file( wstrAutoUpdateXmlCheckSupport, wstrAutoUpdateXmlCheck );

	STARTUPINFO stif;
	PROCESS_INFORMATION pi;
	memset(&stif,0,sizeof(STARTUPINFO));
	stif.cb=sizeof(STARTUPINFO);
	stif.dwFlags= STARTF_USESHOWWINDOW;
	stif.wShowWindow=SW_HIDE;

	WCHAR	wszExeFilePath[MAX_PATH] = L"";
	WCHAR	wszCommandLine[MAX_PATH] = L"";

	wcscat( wszFullFilePath, L".\\epay" );

	wcscat( wszExeFilePath, wszFullFilePath );
	wcscat( wszExeFilePath, L"\\autoupdate.exe" );

	//wcscat(wszCommandLine, L"\\ autoupdate.xml 0 0");

	CreateProcessW(wszExeFilePath,wszCommandLine,NULL,NULL,0,0,NULL,wszFullFilePath,&stif,&pi);

	return true;

}
#endif //SERV_EPAY_SYSTEM

#ifdef GET_ELSWORD_FOCUS
//--------------------------------------------------------------------------------
// 2011.4.8 조효진 핵실드 사용 시 포커스 넘어 가는 문제 때문에 해당 함수 사용
//--------------------------------------------------------------------------------
void ReallySetForegroundWindow( HWND hWnd )
{
	DWORD foregroundThreadID; // foreground window thread
	DWORD ourThreadID; // our active thread
	// If the window is in a minimized state, maximize now
	if (GetWindowLong(hWnd, GWL_STYLE) & WS_MINIMIZE)
	{
		ShowWindow(hWnd, SW_MAXIMIZE);
		UpdateWindow(hWnd);
	}
	// Check to see if we are the foreground thread
	foregroundThreadID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
	ourThreadID = GetCurrentThreadId();
	// If not, attach our thread's 'input' to the foreground thread's
	if (foregroundThreadID != ourThreadID)
		AttachThreadInput(foregroundThreadID, ourThreadID, TRUE);
	// Bring our window to the foreground
	SetForegroundWindow(hWnd);
	// If we attached our thread, detach it now
	if (foregroundThreadID != ourThreadID)
		AttachThreadInput(foregroundThreadID, ourThreadID, FALSE);
	// Force our window to redraw
	InvalidateRect(hWnd, NULL, TRUE);
}
//--------------------------------------------------------------------------------
#endif GET_ELSWORD_FOCUS

#ifdef ALLOW_MULTI_CLIENT
__forceinline bool MultiClientValidation()
{
	try 
	{
		static const int MaxMultiClient = 3;
		HANDLE mutex = NULL;
		boost::wformat fmtMutexName(L"ElswordX2_%1%");

		int i=0; 
		for(; i<MaxMultiClient; i++) 
		{
			std::wstring strMutextName = boost::str( fmtMutexName % i);
			mutex = OpenMutex(MUTEX_ALL_ACCESS, false, strMutextName.c_str() ); 
			DWORD errcod = ::GetLastError();
			if(mutex == NULL) 
			{ 
				mutex = CreateMutex(NULL, true, strMutextName.c_str()); 
				if(mutex != NULL)
				{
					g_hMutex = mutex;
					break;
				}
			}
			else
			{
				CloseHandle(mutex);
			}
		} 
		if(i>=3) 
		{ 
			return false;
		} 

		return true;
	} 
	catch (std::exception& ex) 
	{ 
		return false;
	}
}
#endif ALLOW_MULTI_CLIENT


#ifdef CHECK_WINDOWS_LANG

bool CheckWindowsLanguage()
{

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_START
#endif


	// NOTE: 이 함수가 여러번 호출된다면 CLEAR_START를 사용할 수 없다. 
	THEMIDA_CLEAR_START
	// TODO: 여기에 VM_START 같이 쓸 수 있는지 알아보자


	unsigned short LangType = PRIMARYLANGID(GetSystemDefaultLangID());  //프라이머리 언어만 뽑는다. 
	switch( LangType )
	{
		// 지원하지 않는 윈도우에 대해서만 정리한다.
	case LANG_CHINESE:
		{
			// 실행할 수 없는 윈도우!!!
			//MessageBox(NULL, L"지원하지 않는 윈도우입니다.", L"Error!", MB_OK);
			return false;
		} break;
	} 

	UINT uContryCode = GetACP();		
	switch(uContryCode)
	{
	case 936:
	case 950:
	case 20000:
	case 20002:
	case 20936:
	case 50227:
	case 50229:
	case 50935:
	case 50936:
	case 54936:
		{
			// 실행할 수 없는 윈도우!!!
			//MessageBox(NULL, L"지원하지 않는 윈도우입니다.", L"Error!", MB_OK);
			return false;
		} break;
	}



#ifdef SYS_INFO

#if 0
	//{{ 박교현 : [2010/06/07]	//	입력 로케일에 따른 중국 접속 금지 처리
#ifdef ADD_INFO_INPUT_LOCALE
	std::vector<LANGID> vecInputLocaleID;
	CX2SystemInfo::QueryInputLocale(vecInputLocaleID);
	BOOST_TEST_FOREACH( LANGID, lid, vecInputLocaleID)
	{
		if(PRIMARYLANGID(lid) == LANG_CHINESE)
			return false;
	}
#endif // ADD_INFO_INPUT_LOCALE
	//}}
#endif

	//{{ 박교현 : [2010/06/07]	//	인스톨 로케일에 따른 중국 접속 금지 처리
#ifdef ADD_INFO_INSTALL_LOCALE
	LANGID InstallLocale = PRIMARYLANGID( CX2SystemInfo::QueryInstallLocaleID() );
	switch( InstallLocale )
	{
		// 지원하지 않는 윈도우에 대해서만 정리한다.
	case LANG_CHINESE:
		{
			// 실행할 수 없는 윈도우!!!
			//MessageBox(NULL, L"지원하지 않는 윈도우입니다.", L"Error!", MB_OK);
			return false;
		} break;
	} 
#endif // ADD_INFO_INSTALL_LOCALE

#endif // SYS_INFO



	return true; 


	THEMIDA_CLEAR_END

#if defined( _SERVICE_ )
	ELSWORD_VIRTUALIZER_END
#endif

}
#endif // CHECK_WINDOWS_LANG



//--------------------------------------------------------------------------------------
//클라이언트 메모리 해킹방지를 위한 메인처리 함수
//--------------------------------------------------------------------------------------
INT WINAPI VirtualWinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
#ifdef OBTAIN_SEDEBUG_PRIVILEGE
	ObtainSeDebugPrivilege();
#endif OBTAIN_SEDEBUG_PRIVILEGE

#ifdef GAMEGUARD_AUTO_UPDATE
#ifndef	NO_GAMEGUARD
	CopyGameGuardFiles();
#endif	NO_GAMEGUARD
#endif GAMEGUARD_AUTO_UPDATE

    //{{ seojt // 2008-10-15, 16:35
#ifdef _DEBUG // 2008-10-15
    /** CRT 함수의 메모리 관리자가 보고하는 memory leak을 검사하기 위해
        debug break해야할 메모리 블록 번호를 명시한다.
        - jintaeks on 2008-10-15, 16:35 */
    // qff
    //BREAK_ALLOC( 1272439 );
    //INIT_CRTDEBUG( 0 );
#endif // _DEBUG // 2008-10-15
    //}} seojt // 2008-10-15, 16:35



#ifdef CHECK_WINDOWS_LANG
	if( false == CheckWindowsLanguage() )
	{
		return 0; 
	}
#endif CHECK_WINDOWS_LANG



	g_hInternet = NULL;
	// HTTP Open
	if ( NULL == ( g_hInternet = ::InternetOpen( L"HTTP_WRAPPER_KOG",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0 ) ) )
	{
		ErrorLog( XEM_ERROR106 );
	}

#ifdef LAUNCHER_COMMAND_ARGUMENT
#ifdef	CLOSE_ON_START_FOR_GAMEGUARD
	int a = __argc;
	if(__argc == 2 && StrCmpA(__argv[1], "CloseOnStart") == 0)
	{
		// OK
	}
	else if(__argc != 3)
#else	CLOSE_ON_START_FOR_GAMEGUARD
	if(__argc != 3)
#endif	CLOSE_ON_START_FOR_GAMEGUARD
	{
		MessageBox(NULL, L"런처를 통해 실행하여 주십시오.", L"알림", MB_OK);
		//MessageBox(NULL, L"请使用登陆器，谢谢！", L"提示", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
#endif // LAUNCHER_COMMAND_ARGUMENT

#ifdef CLIENT_PURPLE_MODULE
	bool bUsePurpleModule = true;	// 사내 기본 인증 사용 시와 CloseOnStart일때는 사용 안함

	if(__argc == 2 && StrCmpA(__argv[1], "CloseOnStart") == 0) // CloseOnStart 모드
		bUsePurpleModule = false;
#ifdef CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH
	else if(__argc != 2)
		bUsePurpleModule = false;
#endif // CLIENT_PURPLE_MODULE_IN_HOUSE_AUTH

	HMODULE hgpshelp = NULL;
	if (bUsePurpleModule)
	{
		int ret = PurpleModuleInit();
		if(0 != ret)
		{
			return 0;
		}
		
		hgpshelp = LoadLibrary(_T("HGPSHELP.DLL"));
	}
#endif // CLIENT_PURPLE_MODULE

#ifdef _SERVICE_
#ifdef _NEXON_KR_
	//OpenGlobalURL( L"http://Elsword.nexon.com/Elsword/etc/gameinstall.aspx?Section=chceck1" );		// wiselog
	OpenGlobalURL( L"http://elsword.nexon.com/wiselog/gameinstall.aspx?section=check1" );		// wiselog
#endif _NEXON_KR_

#ifndef _SERVICE_MANUAL_LOGIN_

#ifdef SERV_CHANNELING_AERIA
	// 패쳐에서는 arg 젤 마지막에 PATCHER_RUN_ONLY 를 붙이는데 여기선 젤 앞에걸로 비교를 하네...
	char* tempArgv = __argv[__argc - 1];
#else //SERV_CHANNELING_AERIA
	char* tempArgv = __argv[1];
#endif //SERV_CHANNELING_AERIA
	if( tempArgv == NULL )
	{
		return 0;
	}
#ifdef CLOSE_ON_START_FOR_GAMEGUARD
	else if( strcmp( tempArgv, "CloseOnStart" ) == 0 ) {}
#endif CLOSE_ON_START_FOR_GAMEGUARD
#ifdef PATCHER_RUN_ONLY
	else if( strcmp(tempArgv, PATCHER_RUN_ONLY ) != 0 )
	{
		return 0;
	}
#endif PATCHER_RUN_ONLY

#endif // _SERVICE_MANUAL_LOGIN_

//#ifndef	_OPEN_TEST_		// 해외팀 제거
#ifdef ALLOW_MULTI_CLIENT
	if(MultiClientValidation() == false)
		return 0;
#else ALLOW_MULTI_CLIENT
	g_hMutex = ::CreateMutex( NULL, TRUE, L"ElswordX2" );
	if ( ( NULL == g_hMutex ) || ( ERROR_ALREADY_EXISTS == ::GetLastError() ) )
	{
		return 0;
	}
#endif ALLOW_MULTI_CLIENT
//#endif	_OPEN_TEST_

#endif _SERVICE_

#ifdef SERV_COUNTRY_PH
#ifdef _SERVICE_
	std::string strGarenaAuthKey = (std::string) __argv[1];
	char* tempArgvGame = __argv[1];
	if (strcmp( tempArgvGame, "CloseOnStart" ) != 0 && strGarenaAuthKey.length() < 300)
	{
		return 0;
	}
#endif _SERVICE_
#endif //SERV_COUNTRY_PH

	g_pX2SMTPMail = new CX2SMTPMail;



#ifndef NO_GAMEGUARD
	
#ifdef GAMEGUARD_INI_COUNTRY
#if defined( CLIENT_COUNTRY_JP )	// 해외팀 각국가 전용 디파인
	std::wstring wstrTest;
#ifdef _OPEN_TEST_
	wstrTest = L"ElswordJPTest";
#else _OPEN_TEST_
	wstrTest = L"ElswordJP";
#endif _OPEN_TEST_
	DWORD dwCode = PreInitNPGameMon( wstrTest.c_str() );
	if( dwCode != NPGAMEMON_SUCCESS )
	{
		std::wstringstream wstrstm;
		wstrstm << L"Game Guardの更新に失敗しました。クライアントを終了します。(" << wstrTest << L", " << dwCode << L")";
		MessageBox( NULL, wstrstm.str().c_str(), L"お知らせ", MB_ICONINFORMATION | MB_OK );
		return 0;
	}
#elif defined( CLIENT_COUNTRY_CN )
#ifdef _OPEN_TEST_
	DWORD dwCode = PreInitNPGameMon( L"ElswordCNTest" );
#else _OPEN_TEST_
	DWORD dwCode = PreInitNPGameMon( L"ElswordCN" );
#endif _OPEN_TEST_
	if ( dwCode != NPGAMEMON_SUCCESS )
	{
		std::wstringstream wstrstm;
		wstrstm << L"反外挂程序GAMEGUARD组件更新失败，要终止客户端。(" << dwCode << L")";
		MessageBox( NULL, wstrstm.str().c_str(), L"提示", MB_ICONINFORMATION | MB_OK );
		return 0;
	}
#elif defined( CLIENT_COUNTRY_TH )
#ifdef _OPEN_TEST_
	DWORD dwCode;
	if( strcmp( tempArgv, "CloseOnStart" ) == 0 )
	{
		dwCode = PreInitNPGameMon( L"ElswordTHINT" );
	}
	else
	{
		dwCode = PreInitNPGameMon( L"ElswordTHTest" );
	}
	if ( dwCode != NPGAMEMON_SUCCESS )
	{
		std::wstringstream wstrstm;
		MessageBox(NULL, L"การเริ่มเกมส์การ์ดerror กรุณาดำเนินการหลังจากยกเลิกโปรแกรมอื่นๆที่ขัดแย้งหรือดำเนินการใหม่อีกครั้ง", L"TESTยืนยัน", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
#else _OPEN_TEST_
	DWORD dwCode;
	if( strcmp( tempArgv, "CloseOnStart" ) == 0 )
	{
		dwCode = PreInitNPGameMon( L"ElswordTHINT" );
	}
	else
	{
		dwCode = PreInitNPGameMon( L"ElswordTH" );
	}
	if ( dwCode != NPGAMEMON_SUCCESS )
	{
		MessageBox(NULL, L"การเริ่มเกมส์การ์ดerror กรุณาดำเนินการหลังจากยกเลิกโปรแกรมอื่นๆที่ขัดแย้งหรือดำเนินการใหม่อีกครั้ง", L"ยืนยัน", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
#endif _OPEN_TEST_
#elif defined( CLIENT_COUNTRY_ID )	
#ifdef _OPEN_TEST_
	DWORD dwCode;
	if( strcmp( tempArgv, "CloseOnStart" ) == 0 )
	{
		dwCode = PreInitNPGameMon( L"ElswordIDINT" );
	}
	else
	{
		dwCode = PreInitNPGameMon( L"ElswordIDTest" );
	}
#else _OPEN_TEST_
	DWORD dwCode;
	if( strcmp( tempArgv, "CloseOnStart" ) == 0 )
	{
		dwCode = PreInitNPGameMon( L"ElswordIDINT" );
	}
	else
	{
		dwCode = PreInitNPGameMon( L"ElswordID" );
	}
#endif _OPEN_TEST_

	if ( dwCode != NPGAMEMON_SUCCESS )
	{
		std::wstringstream wstrstm;
		wstrstm << L"Gagal update GameGuard (Error Code : " << dwCode << L")";
		MessageBox( NULL, wstrstm.str().c_str(), L"Error", MB_ICONINFORMATION | MB_OK );
		return 0;
	}
#elif defined( CLIENT_COUNTRY_BR )	
#ifdef _OPEN_TEST_
	DWORD dwCode = PreInitNPGameMon( L"ElswordBRTest" );
#else _OPEN_TEST_
	DWORD dwCode = PreInitNPGameMon( L"ElswordBR" );
#endif _OPEN_TEST_

	if ( dwCode != NPGAMEMON_SUCCESS )
	{
		std::wstringstream wstrstm;
		wstrstm << L"Fail update GameGuard (Error Code : " << dwCode << L")";
		MessageBox( NULL, wstrstm.str().c_str(), L"Error", MB_ICONINFORMATION | MB_OK );
		return 0;
	}
#elif defined( CLIENT_COUNTRY_PH )
#ifdef _OPEN_TEST_
	DWORD dwCode = PreInitNPGameMon( L"ElswordPHTest" );
	//DWORD dwCode = PreInitNPGameMon( L"ElswordID" );
#else _OPEN_TEST_
	DWORD dwCode = PreInitNPGameMon( L"ElswordPH" );
#endif _OPEN_TEST_

	if ( dwCode != NPGAMEMON_SUCCESS )
	{
		std::wstringstream wstrstm;
		wstrstm << L"Error update GameGuard (Error Code : " << dwCode << L")";
		MessageBox( NULL, wstrstm.str().c_str(), L"Error", MB_ICONINFORMATION | MB_OK );
		return 0;
	}
#endif
#else // GAMEGUARD_INI_COUNTRY
	PreInitNPGameMon( L"ElswordKR" );
#endif // GAMEGUARD_INI_COUNTRY
#endif // NO_GAMEGUARD

#ifdef SERV_STEAM
	CX2Steam::InitSteam();
#endif //SERV_STEAM

	CKTDXApp::StartUp();

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Set the callback functions. These functions allow DXUT to notify
	// the application about device changes, user input, and windows messages.  The 
	// callbacks are optional so you need only set callbacks for events you're interested 
	// in. However, if you don't handle the device reset/lost callbacks then the sample 
	// framework won't be able to reset your device since the application must first 
	// release all device resources before resetting.  Likewise, if you don't handle the 
	// device created/destroyed callbacks then DXUT won't be able to 
	// recreate your device resources.
	DXUTSetCallbackDeviceCreated( OnCreateDevice );
	DXUTSetCallbackDeviceReset( OnResetDevice );
	DXUTSetCallbackDeviceLost( OnLostDevice );
	DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackKeyboard( KeyboardProc );
	DXUTSetCallbackFrameRender( OnFrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );

	// Show the cursor and clip it when in full screen
	DXUTSetCursorSettings( true, true );

#ifdef HACK_SHIELD
#ifdef HACKSHIELD_AUTO_UPDATE
	if( false == UpdateHackShield() )
		return 0;
#endif HACKSHIELD_AUTO_UPDATE

	//{{ 2011.3.21	조효진	핵쉴드 모니터링 서버 연동
#ifdef HACKSHIELD_MORNITORING
	StartMonitorHackShield();	// 실패해도 클라 실행에 지장 없도록 하기 위해서 
#endif HACKSHIELD_MORNITORING
	//}}

	if( false == InitializeHackShield() )
		return 0; 
#endif HACK_SHIELD

	// XTRAP 클라이언트 - X-TRAP 클라 실행
#ifdef CLIENT_USE_XTRAP
	StartXTrap();
#endif // CLIENT_USE_XTRAP

#ifdef SERV_EPAY_SYSTEM
	EPayPrepareUpdate();
#endif //SERV_EPAY_SYSTEM

	InitApp();

	// Initialize DXUT and create the desired Win32 window and Direct3D 
	// device for the application. Calling each of these functions is optional, but they
	// allow you to set several options which control the behavior of the framework.
	DXUTInit( false, false, true ); // Parse the command line, handle the default hotkeys, and show msgboxes

	//{{ JHKang // 2010-8-27 // 다중 모니터 창 이동 가능
#ifdef ENABLE_MULTI_MONITOR
	DXUTSetMultimonSettings(false);
#endif ENABLE_MULTI_MONITOR
	//}} JHKang // 2010-8-27 // 다중 모니터 창 이동 가능

	HICON hIcon = ::LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON1 ) );

	DXUTCreateWindow( L"X2", NULL, hIcon );
#ifdef CLOSE_ON_START_FOR_GAMEGUARD
	if(__argc == 2 && StrCmpA(__argv[1], "CloseOnStart") == 0)
	{
		return 0;
	}
#endif CLOSE_ON_START_FOR_GAMEGUARD
	DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 1024, 768, IsDeviceAcceptable, ModifyDeviceSettings );

#ifdef CLIENT_USE_XTRAP	// XTRAP 클라이언트 - 정상 실행 됬는지 확인
	KeepAliveXTrap();
#endif // CLIENT_USE_XTRAP

	//{{ 2010.12.1 조효진 핵쉴드 사용 시 포커스 넘어가는 문제 해결 
#ifdef GET_ELSWORD_FOCUS
	ReallySetForegroundWindow(DXUTGetHWND());
#endif GET_ELSWORD_FOCUS
	//}}

	// Pass control to DXUT for handling the message pump and 
	// dispatching render calls. DXUT will call your FrameMove 
	// and FrameRender callback when there is idle time between handling window messages.
	DXUTMainLoop();

	// Perform any application-level cleanup here. Direct3D device resources are released within the
	// appropriate callback functions and therefore don't require any cleanup code here.
	CKTDXApp::Exit();

	
	if ( g_hMutex != NULL )
	{
		::ReleaseMutex( g_hMutex );
		::CloseHandle( g_hMutex );
		g_hMutex = NULL;
	}

#ifdef HACK_SHIELD
	FinalizeHackShield();
#endif HACK_SHIELD

#ifdef SERV_STEAM
	CX2Steam::EndSteam();
#endif //SERV_STEAM

#ifdef CLIENT_PURPLE_MODULE
	if (bUsePurpleModule)
	{
		PurpleModuleTerminate();
		if(hgpshelp != NULL)
		{
			FreeLibrary(hgpshelp);
		}
	}
#endif // CLIENT_PURPLE_MODULE

	return DXUTGetExitCode();
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShowCmd )
{
	return VirtualWinMain( hInst, hPrev, lpCmdLine, nShowCmd );
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
	// Initialize dialogs
#ifndef  REMOVE_DXUT_SAMPLE_CODE
	g_SettingsDlg.Init( &g_DialogResourceManager );
	g_HUD.Init( &g_DialogResourceManager );
	g_SampleUI.Init( &g_DialogResourceManager );

	g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
	g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
	g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
	g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22 );

	g_SampleUI.SetCallback( OnGUIEvent ); iY = 10; 
#endif  REMOVE_DXUT_SAMPLE_CODE



	/*
	TODO: add UI controls as needed
	g_SampleUI.AddComboBox( 19, 35, iY += 24, 125, 22 );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text1", NULL );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text2", NULL );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text3", NULL );
	g_SampleUI.GetComboBox( 19 )->AddItem( L"Text4", NULL );
	g_SampleUI.AddCheckBox( 21, L"Checkbox1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddCheckBox( 11, L"Checkbox2", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 12, 1, L"Radio1G1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 13, 1, L"Radio2G1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 14, 1, L"Radio3G1", 35, iY += 24, 125, 22 );
	g_SampleUI.GetRadioButton( 14 )->SetChecked( true ); 
	g_SampleUI.AddButton( 17, L"Button1", 35, iY += 24, 125, 22 );
	g_SampleUI.AddButton( 18, L"Button2", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 15, 2, L"Radio1G2", 35, iY += 24, 125, 22 );
	g_SampleUI.AddRadioButton( 16, 2, L"Radio2G3", 35, iY += 24, 125, 22 );
	g_SampleUI.GetRadioButton( 16 )->SetChecked( true );
	g_SampleUI.AddSlider( 20, 50, iY += 24, 100, 22 );
	g_SampleUI.GetSlider( 20 )->SetRange( 0, 100 );
	g_SampleUI.GetSlider( 20 )->SetValue( 50 );
	g_SampleUI.AddEditBox( 20, L"Test", 35, iY += 24, 125, 32 );
	*/
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
								 D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	// Skip backbuffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3DObject(); 

	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
	// then switch to SWVP.
	if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// Debugging vertex shaders requires either REF or software vertex processing 
	// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
	if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif



	// depth/stencil buffer format을 지정해주자
	CD3DEnumeration *pEnum = DXUTGetEnumeration();
	CD3DEnumDeviceSettingsCombo *pCombo;

	pCombo = pEnum->GetDeviceSettingsCombo( pDeviceSettings );

	if( pCombo->depthStencilFormatList.Contains( D3DFMT_D24X8 ) == true )
		pDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D24X8;
	else if( pCombo->depthStencilFormatList.Contains( D3DFMT_D32 ) == true )
		pDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D32;


	//{{ dmlee 2008.03.25 fullscreen 상태에서 child window를 띄우기 위해
	pDeviceSettings->pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pDeviceSettings->pp.Flags |= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; 
	//}} dmlee 2008.03.25 fullscreen 상태에서 child window를 띄우기 위해






#ifdef DISABLE_DRIVER_MANAGEMENT_TEST
//{{ seojt // 2008-10-14, 15:13
    // 그래픽 카드 드라이버가 아닌 DirectX가 리소스를 매니지 하도록 한다.
    pDeviceSettings->BehaviorFlags |= D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX;
    // DirectX가 생성된 쓰레드가 아닌 다른 쓰레드에서도 D3D Device를 접근할 수 있도록
    // 한다. background resource loading을 위해 처음으로 추가하다.
    // - jintaeks on 2008-10-14, 15:20
    pDeviceSettings->BehaviorFlags |= D3DCREATE_MULTITHREADED;
//}} seojt // 2008-10-14, 15:13
#endif DISABLE_DRIVER_MANAGEMENT_TEST

#ifdef  _USE_LUAJIT_
    pDeviceSettings->BehaviorFlags |= D3DCREATE_FPU_PRESERVE;
#endif  _USE_LUAJIT_

#ifndef CLIENT_USE_XTRAP	// xtrap 맵 파일을 만드는데 방해가 되므로 제거
	// For the first device created if its a REF device, optionally display a warning dialog box
	static bool s_bFirstTime = true;
	if( s_bFirstTime )
	{
		s_bFirstTime = false;
		if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
			DXUTDisplaySwitchingToREFWarning();
	}
#endif // CLIENT_USE_XTRAP	

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	HRESULT hr;
#endif
	// Initialize the font
	 
	g_pd3dDevice = pd3dDevice;
	g_pD3D = DXUTGetD3DObject();

//{{ robobeg : 2008-10-10
    g_pEffectStateManager = new CKTDGEffectStateManager;
    ASSERT( g_pEffectStateManager != NULL );
//}} robobeg : 2008-10-10


	g_pKTDXApp	= new CKTDXApp( DXUTGetHWND(), DXUTGetHINSTANCE(), pd3dDevice );
	g_pMain		= new CX2Main();

	g_pKTDXApp->InsertStage( g_pMain );	




#ifndef REMOVE_DXUT_SAMPLE_CODE
	V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
	V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
#endif

#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnCreateDevice( pd3dDevice );

	KProfileMan::GetSingleton().SetProfileIterator( RHProfileManager::Get_Iterator() );
#endif
	


	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
							   const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	HRESULT hr;
#endif

#ifndef REMOVE_DXUT_SAMPLE_CODE
	V_RETURN( g_DialogResourceManager.OnResetDevice() );
	V_RETURN( g_SettingsDlg.OnResetDevice() );
#endif  REMOVE_DXUT_SAMPLE_CODE

#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnResetDevice( pd3dDevice );
#endif

	// Create a sprite to help batch calls when drawing many lines of text
#ifndef  REMOVE_DXUT_SAMPLE_CODE
	g_HUD.SetLocation( pBackBufferSurfaceDesc->Width-170, 0 );
	g_HUD.SetSize( 170, 170 );
	g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width-170, pBackBufferSurfaceDesc->Height-350 );
	g_SampleUI.SetSize( 170, 300 );
#endif  REMOVE_DXUT_SAMPLE_CODE
	g_pKTDXApp->OnResetDevice( pBackBufferSurfaceDesc );



	//{{ dmlee 2009/11/16 alt_tab 기능 막기 코드 인듯.
	//if ( DXUTGetDeviceSettings().pp.Windowed == TRUE )
	//{
	//	UnregisterHotKey(DXUTGetHWND(), 0);
	//}
	//else
	//{
	//	RegisterHotKey(DXUTGetHWND(), 0, MOD_ALT, VK_TAB);
	//}
	//}} dmlee 2009/11/16 alt_tab 기능 막기 코드 인듯.
	
	

	return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnPreFrameMove( fElapsedTime );
#endif

	KTDXPROFILE();

	g_pKTDXApp->OnFrameMove( fTime, fElapsedTime );
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	//KTDXPROFILE();

	HRESULT hr;

	// If the settings dialog is being shown, then
	// render it instead of rendering the app's scene
#ifndef REMOVE_DXUT_SAMPLE_CODE
	if( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.OnRender( fElapsedTime );
		return;
	}
#endif  REMOVE_DXUT_SAMPLE_CODE


	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		//2012.09.17 박진웅. 녹화 표시 파티클을 출력하지 않게 한다.
#ifdef BANDICAM_HIDE_RECORDING_PARTICLE
		CX2State* pCurrentState = NULL;
		if( g_pMain != NULL && g_pMain->m_BandiCaptureLibrary.IsCapturing())
		{
			pCurrentState = (CX2State*)g_pMain->GetNowState();
			if( pCurrentState != NULL )
				pCurrentState->ShowBandicamRecParticle( false );
		}//if
#endif BANDICAM_HIDE_RECORDING_PARTICLE

		KTDXPROFILE_BEGIN("KTDXRender");
		g_pKTDXApp->OnFrameRender();
		KTDXPROFILE_END();


		if( true == g_bRenderEtc )
		{
			KTDXPROFILE_BEGIN("DXUTRender");
			DXUT_BeginPerfEvent( DXUT_PERFEVENTCOLOR, L"HUD / Stats" ); // These events are to help PIX identify what the code is doing
			KTDXPROFILE_BEGIN("RenderText");
			RenderText(fElapsedTime);
			KTDXPROFILE_END();

#ifndef  REMOVE_DXUT_SAMPLE_CODE
			KTDXPROFILE_BEGIN("g_HUD.OnRender");
			V( g_HUD.OnRender( fElapsedTime ) );
			KTDXPROFILE_END();
			KTDXPROFILE_BEGIN("g_SampleUI.OnRender");
			V( g_SampleUI.OnRender( fElapsedTime ) );
			KTDXPROFILE_END();
#endif  REMOVE_DXUT_SAMPLE_CODE
			DXUT_EndPerfEvent();
			KTDXPROFILE_END();
		}

#ifdef KHELP_SYSTEM_TEST // 2008-10-23
        _KHelpSystem.DisplayHelp();
#endif // KHELP_SYSTEM_TEST // 2008-10-23

#ifndef _SERVICE_
		if( g_pMain != NULL && g_pData != NULL && g_pData->GetMyUser() != NULL && 
			g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR &&
			g_pData->GetDungeonResultInfo() != NULL && g_pX2Room != NULL )
		{
			CX2DungeonGame::DungeonResultInfo* pDungeonResultInfo = g_pData->GetDungeonResultInfo();

			int playTimeAllSec = g_pData->GetDungeonResultInfo()->m_Sec; 

			int playTimeHour = playTimeAllSec/3600; 
			int	playTimeMinute = ( playTimeAllSec%3600 ) / 60;
			int playTimeSec = ( playTimeAllSec%3600 ) % 60;

			int peopleNum = 0;
			for ( int i = 0; i < (int)pDungeonResultInfo->m_DungeonResultUnitInfoList.size(); i++ )
			{
				CX2DungeonGame::DungeonResultUnitInfo* pDungeonResultUnitInfo = pDungeonResultInfo->m_DungeonResultUnitInfoList[i];
				if ( pDungeonResultUnitInfo == NULL )
					continue;

				CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotDataByUnitUID( pDungeonResultUnitInfo->m_UnitUID );

				if ( pSlotData == NULL || pSlotData->m_pUnit == NULL )
					continue;

				WCHAR buff[256] = {0};
				wstring tempResultString;

				tempResultString = pSlotData->m_pUnit->GetNickName();
				tempResultString += L"\n";
				
				StringCchPrintf( buff, 256, L"Exp = %d,  m_nClearBonusEXP = %d, m_nRankBonusEXP = %d \n m_nPartyBonusEXP = %d, m_nMvpBonusEXP = %d m_nGameBangBonusEXP = %d, \n m_nPremiumBonusEXP = %d, m_nEventBonusExp = %d, ED = %d, KillNum = %d, PlayTime = %d:%d:%d\n", 
					pDungeonResultUnitInfo->m_nEXP, 
					pDungeonResultUnitInfo->m_nClearBonusEXP, 
					pDungeonResultUnitInfo->m_nRankBonusEXP, 
					pDungeonResultUnitInfo->m_nPartyBonusEXP, 
					pDungeonResultUnitInfo->m_nMvpBonusEXP, 
					pDungeonResultUnitInfo->m_nGameBangBonusEXP, 
					pDungeonResultUnitInfo->m_nPremiumBonusEXP, 
					pDungeonResultUnitInfo->m_nEventBonusExp, 
					pDungeonResultUnitInfo->m_nED, pDungeonResultUnitInfo->m_nNPCKillNum, playTimeHour, playTimeMinute, playTimeSec );
				/*wsprintf( buff, L"Exp = %d,  m_nClearBonusEXP = %d, m_nRankBonusEXP = %d \n m_nPartyBonusEXP = %d, m_nMvpBonusEXP = %d m_nGameBangBonusEXP = %d, \n m_nPremiumBonusEXP = %d, m_nEventBonusExp = %d, ED = %d, KillNum = %d, PlayTime = %d:%d:%d\n", 
					pDungeonResultUnitInfo->m_nEXP, 
					pDungeonResultUnitInfo->m_nClearBonusEXP, 
					pDungeonResultUnitInfo->m_nRankBonusEXP, 
					pDungeonResultUnitInfo->m_nPartyBonusEXP, 
					pDungeonResultUnitInfo->m_nMvpBonusEXP, 
					pDungeonResultUnitInfo->m_nGameBangBonusEXP, 
					pDungeonResultUnitInfo->m_nPremiumBonusEXP, 
					pDungeonResultUnitInfo->m_nEventBonusExp, 
					pDungeonResultUnitInfo->m_nED, pDungeonResultUnitInfo->m_nNPCKillNum, playTimeHour, playTimeMinute, playTimeSec );*/
				tempResultString += buff;
				
				StringCchPrintf( buff, 256,  L"TotalScore = %d, ComboScore = %d, TechnicalScore = %d, Damaged = %d, \nRankBonus = %d PartyBonus = %d\n",
					pDungeonResultUnitInfo->m_nTotalScore, pDungeonResultUnitInfo->m_nComboScore, pDungeonResultUnitInfo->m_nTechnicalScore, pDungeonResultUnitInfo->m_nDamaged, (int)(pDungeonResultUnitInfo->m_fRankBonus * 100 ), (int)(pDungeonResultUnitInfo->m_fPartyBonus * 100) );
				/*wsprintf( buff, L"TotalScore = %d, ComboScore = %d, TechnicalScore = %d, Damaged = %d, \nRankBonus = %d PartyBonus = %d\n",
					pDungeonResultUnitInfo->m_nTotalScore, pDungeonResultUnitInfo->m_nComboScore, pDungeonResultUnitInfo->m_nTechnicalScore, pDungeonResultUnitInfo->m_nDamaged, (int)(pDungeonResultUnitInfo->m_fRankBonus * 100 ), (int)(pDungeonResultUnitInfo->m_fPartyBonus * 100) );*/
				tempResultString += buff;

				StringCchPrintf( buff, 256, L"ComboRank = %s, TechnicalRank = %s, DamageRank = %s, TimeRank = %s, TotalRank = %s",
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cComboRank ).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cTechnicalRank ).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cDamagedRank).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cTimeRank).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cTotalRank).c_str() );
				/*wsprintf( buff, L"ComboRank = %s, TechnicalRank = %s, DamageRank = %s, TimeRank = %s, TotalRank = %s",
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cComboRank ).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cTechnicalRank ).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cDamagedRank).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cTimeRank).c_str(),
					g_pData->GetComboString( pDungeonResultUnitInfo->m_cTotalRank).c_str() );*/
				tempResultString += buff;

				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextMultiline( 40, (peopleNum * 120) + 70, tempResultString.c_str(), D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), 1.0f, NULL, DT_LEFT );

				peopleNum++;
			}
		}
#endif
		
#ifdef BANDICAM_RECORDING
		if( g_pMain != NULL && g_pChatBox != NULL && 
			g_pMain->IsCapturing() == true )
		{
			switch( g_pMain->m_BandiCaptureLibrary.Work((LONG_PTR)pd3dDevice) )
			{
			case BC_OK_IS_NOT_CAPTURING:	//캡쳐중이 아닙니다.
			case BC_OK_COMPLETED_BY_TIME:	//녹화 자동 완료 설정(시간)에 의해 녹화 완료되었습니다.
			case BC_OK_COMPLETED_BY_SIZE:	//녹화 자동 완료 설정(파일크기)에 의해 녹화 완료 되었습니다.
			case BCERR_VIDEO_CAPTURE:		//캡쳐중 에러가 발생하였습니다.
			case BCERR_FILE_CREATE:			//파일 생성을 실패하였습니다.
				{
					g_pMain->Bandi_EndVidioCapture();
				}break;
			case BCERR_FILE_WRITE:			// 파일 쓰기를 실패하였습니다.
			case BCERR_NOT_ENOUGH_DISKSPACE:// 하드디스크 여유 공간 부족이 부족합니다.
				{
					g_pMain->Bandi_EndVidioCapture();
					g_pChatBox->AddChatLog( GET_STRING(STR_ID_17540), KEGS_CHAT_REQ::CPT_SYSTEM, D3DXCOLOR(1,1,0,1), L"#CFFFF00" );
				}break;
			default:
				break;
			}
#ifdef BANDICAM_HIDE_RECORDING_PARTICLE
			if( pCurrentState != NULL )
				pCurrentState->ShowBandicamRecParticle( true );
#endif BANDICAM_HIDE_RECORDING_PARTICLE
		}
#endif BANDICAM_RECORDING

#ifdef PROFILE_BUILD
		KProfileMan::GetSingleton().OnFrameRender( pd3dDevice, fElapsedTime );
#endif

		V( pd3dDevice->EndScene() );
	}
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText(float fElapsedTime)
{
	// The helper object simply helps keep track of text position, and color
	// and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
	// If NULL is passed in as the sprite object, then it will work however the 
	// pFont->DrawText() will not be batched together.  Batching calls will improves performance.
	//   CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

	g_fTotalFrame += DXUTGetFPS();
	g_RenderCount += 1;
	
	// Output statistics
	WCHAR appData[512] = {0,};
	WCHAR appData3[512] = {0,};
	WCHAR wszText[512] = L"";
	swprintf( appData, DXUTGetFrameStats() );
	wcscat( appData, L"\n" );
	wcscat( appData, DXUTGetDeviceStats() );
	wcscat( appData, L", 평균 프레임 : ");
	wcscat( appData, _itow( (int)g_fTotalFrame / g_RenderCount, appData3, 10 ) );
	if ( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", 오브젝트 LOD : %d %%", g_pX2Game->GetWorld()->GetLOD() );
		wcscat( appData, wszText );
	}

	if ( g_pData != NULL )
	{
		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", 캐릭터 LOD : %d %%", g_pData->GetModelDetailPercent() );
		wcscat( appData, wszText );
	}

#ifdef FRAME_MOVE_CULL
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L", FrameUpdate : %d %%", (int)CKTDGXSkinAnim::GetTestUpdate() );
	wcscat( appData, wszText );
#endif //FRAME_MOVE_CULL	

//#ifdef  DYNAMIC_VERTEX_BUFFER_OPT

    if ( g_pKTDXApp != NULL && g_pKTDXApp->GetDVBManager() != NULL )
    {
        const WCHAR*  pDynamicVBMode = L"";
        switch( g_pKTDXApp->GetDVBManager()->GetDynamicVBMode() )
        {
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_MANAGED:
            pDynamicVBMode = L"MANAGED";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_PRIMITIVEUP:
            pDynamicVBMode = L"PrimitiveUP";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_DYNAMICVB:
            pDynamicVBMode = L"DynamicVB";
            break;
        case CKTDGDynamicVBManager::DYNAMIC_VB_MODE_DYNAMICVB_WITH_TRY_CATCH:
            pDynamicVBMode = L"DynamicVBWithTC";
            break;
        }//switch

	    swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"\nDynamicVBMode : %s", pDynamicVBMode );
		
	    wcscat( appData, wszText );
#ifdef X2OPTIMIZE_DYNAMICVB_DISCARDCOUNT_LOG
		int iTemp = g_pKTDXApp->GetDVBManager()->GetAvgDiscardCount( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE );
		int iTemp2 = g_pKTDXApp->GetDVBManager()->GetAvgDiscardCount( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE_TEX1 );
		int iTemp3 = g_pKTDXApp->GetDVBManager()->GetAvgDiscardCount( CKTDGDynamicVBManager::DVB_TYPE_XYZ_TEX1 );
		int iTemp4 = g_pKTDXApp->GetDVBManager()->GetAvgDiscardCount( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1 );

		swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"\nDynamicVB DiscardCountPerSec - DVB_TYPE_XYZ_DIFFUSE: %d, DVB_TYPE_XYZ_DIFFUSE_TEX1: %d, DVB_TYPE_XYZ_TEX1: %d, DVB_TYPE_XYZRHW_DIFFUSE_TEX1: %d", 
			iTemp,
			iTemp2,
			iTemp3,
			iTemp4 );
#endif//X2OPTIMIZE_DYNAMICVB_DISCARDCOUNT_LOG			
			
	    wcscat( appData, wszText );
    }//if
//#endif  DYNAMIC_VERTEX_BUFFER_OPT


	wcscat( appData, L"\n" );
	
	WCHAR appData2[256] = {0,};

	int iSimultaneousUserCount = 0;
	int iPVPRoomTeamCount = 0;
	int iPVPRoomTeamDeathCount = 0;
	int iPVPRoomSurvivalCount = 0;
	int iCurPartyCount = 0;
	int iPlayGamePartyCount = 0;
	int iCurrentOnLineIndex = -1;

	if( NULL != g_pMain )
	{	
		iSimultaneousUserCount	= g_pMain->GetServerUserCount();
		std::map<int, int>& mapPVPRoomCount = g_pMain->GetMapServerPVPRoomCount();
		if( mapPVPRoomCount.size() > 0 )
		{
			iPVPRoomTeamCount		= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_TEAM ];
			iPVPRoomTeamDeathCount	= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_TEAM_DEATH ];
			iPVPRoomSurvivalCount	= mapPVPRoomCount[ (int)CX2PVPRoom::PGT_SURVIVAL ];
		}

		iCurPartyCount				= g_pMain->GetCurPartyCount();
		iPlayGamePartyCount			= g_pMain->GetPlayGamePartyCount();

		if ( NULL != g_pTFieldGame && NULL != g_pTFieldGame->GetMyUnit() )
			iCurrentOnLineIndex = g_pTFieldGame->GetMyUnit()->GetLastTouchLineIndex();

		if ( NULL != g_pX2Game && NULL != g_pX2Game->GetMyUnit() )
			iCurrentOnLineIndex = g_pX2Game->GetMyUnit()->GetLastTouchLineIndex();
	}

#ifndef _SERVICE_
	swprintf( appData2, L"프레임무브 FPS: %f 지연시간: %f  프레임무브: %d회  랜더: %d회 \n", 
		g_pKTDXApp->GetFrameMoveFPS(), g_pKTDXApp->GetElapsedTime(), g_pKTDXApp->GetFrameMoveCount(), g_pKTDXApp->GetRenderCount() );
	wcscat( appData, appData2 );
#endif

	swprintf( appData2, L"동접: %d명, 대전방: <팀:%d, 팀데스:%d, 서바:%d>, 파티: %d, 게임중인파티: %d, 현재라인위치: %d", 
		iSimultaneousUserCount, iPVPRoomTeamCount, iPVPRoomTeamDeathCount, iPVPRoomSurvivalCount, iCurPartyCount, iPlayGamePartyCount, iCurrentOnLineIndex );
	wcscat( appData, appData2 );

#ifdef SERV_CHANNELING_USER_MANAGER
	wcscat( appData, L"\n");

	std::map<int,int> mapChannelingUserList = g_pMain->GetServerChannelingUserCount();

	if( !mapChannelingUserList.empty() )
	{
		std::map<int,int>::const_iterator mitCUL;
		for( mitCUL = mapChannelingUserList.begin(); mitCUL != mapChannelingUserList.end(); ++mitCUL )
		{
			const WCHAR*  pSite = L"";
			switch( mitCUL->first )
			{
			case KNexonAccountInfo::JCE_NHN:
				{
					pSite = L"NHN";
				}
				break;
			case KNexonAccountInfo::JCE_MSN:
				{
					pSite = L"MSN";
				}
				break;
			case KNexonAccountInfo::JCE_NICO:
				{
					pSite = L"NICONICO";
				}
				break;
			case KNexonAccountInfo::JCE_NEXON:
				{
					pSite = L"NEXON";
				}
				break;
			}

			swprintf( appData2, L"%s 동접: %d , ", pSite, mitCUL->second );
			wcscat( appData, appData2 );
		}
	}
#endif SERV_CHANNELING_USER_MANAGER


	/*
	txtHelper.Begin();
	txtHelper.SetInsertionPos( 5, 5 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	txtHelper.DrawTextLine( appData );
	*/

	WCHAR timeBuf[200] = {0,};
	g_pMain->GetFontForDebuf()->OutTextMultiline( 5, 5, appData, D3DXCOLOR( 1, 1, 0, 1), 
		CKTDGFontManager::FS_SHELL, D3DXCOLOR( 0, 0, 0, 1 ) );

	
#ifdef SHOW_UDP_NETWORK_INFO_IN_ROOM
	if ( NULL != g_pX2Room )
	{
		int iLineNumber = 0;
		const int iObserverlSlotNum = g_pX2Room->GetObserverSlotNum();
		for ( int i = 0; i < iObserverlSlotNum; i++ )
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetObserverSlotData( i );
			if ( NULL != pSlotData && pSlotData->GetSlotState() != CX2Room::SS_EMPTY && pSlotData->GetSlotState() != CX2Room::SS_CLOSE )
			{
				wstring wstrNetworkInfo = L"Observer";					
				wstrNetworkInfo += g_pX2Room->GetStringNetworkInfo( pSlotData );

				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 40, 200 + iLineNumber * 15, wstrNetworkInfo.c_str(), D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
				++iLineNumber;
			}
		}

		const int iNormalSlotNum = g_pX2Room->GetSlotNum();
		for ( int i = 0; i < iNormalSlotNum; i++ )
		{
			CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( i );
			if ( NULL != pSlotData && pSlotData->GetSlotState() != CX2Room::SS_EMPTY && pSlotData->GetSlotState() != CX2Room::SS_CLOSE )
			{
				wstring wstrNetworkInfo =
					( NULL != pSlotData->m_pUnit ? pSlotData->m_pUnit->GetNickName() : L"" );
				wstrNetworkInfo += g_pX2Room->GetStringNetworkInfo( pSlotData );

				if ( NULL != g_pX2Game )
				{
					const CX2GUUser* pGUUser = g_pX2Game->GetUserUnitByUID( pSlotData->m_UnitUID );
					if ( NULL != pGUUser )
					{
						const int iStateID = pGUUser->GetNowStateID();
						swprintf( timeBuf, L" / StateID: %d", iStateID );
						wstrNetworkInfo += timeBuf;
					}
				}

				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 40, 200 + iLineNumber * 15, wstrNetworkInfo.c_str(), D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
				++iLineNumber;
			}
		}		
	}
#endif // SHOW_UDP_NETWORK_INFO_IN_ROOM

	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		if( g_pX2Game != NULL && g_pX2Game->GetHostGameUnit() != NULL )
		{		
#ifdef  X2OPTIMIZE_NPC_NONHOST_SIMULATION
            int iAvgPingTime = 0;
            int iAvgPingStd = 0;
            if ( g_pX2Room != NULL )
            {
               const CX2Room::SlotData* pSlotData = g_pX2Room->GetSlotData( g_pX2Game->GetHostGameUnit()->GetUnitIndex() );
               if ( pSlotData != NULL )
               {
                   iAvgPingTime = (int) ( pSlotData->m_fAvgPingTime * 1000 );
#ifdef  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
                   iAvgPingStd = (int) ( pSlotData->m_fAvgPingStd * 1000 );
#endif  SERV_OPTIMIZE_CHOOSE_FASTEST_HOST_ENHANCE
               }
            }
			swprintf( timeBuf, L"HostUser: %s, NumOfNpc: %d, NumOfUser: %d AvgPingToHost: %d(%d)", 
				g_pX2Game->GetHostGameUnit()->GetUnitName().c_str(), g_pX2Game->GetNPCUnitNum(), g_pX2Game->GetUserUnitNum(), iAvgPingTime, iAvgPingStd );
#else   X2OPTIMIZE_NPC_NONHOST_SIMULATION
			swprintf( timeBuf, L"HostUser: %s, NumOfNpc: %d, NumOfUser: %d", 
				g_pX2Game->GetHostGameUnit()->GetUnitName().c_str(), g_pX2Game->GetNPCUnitNum(), g_pX2Game->GetUserUnitNum() );
#endif  X2OPTIMIZE_NPC_NONHOST_SIMULATION

			if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( 40, 460, timeBuf, D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
		}
		
#ifdef ADD_RAID_FIELD_LOG  // 기여도 확인하기
		int iPosY = 300;
		for( int i=0; i<g_pX2Game->GetNPCUnitListSize(); ++i )
		{
			CX2GUNPC* pGUNPC = g_pX2Game->GetNPCUnit(i);

			if( NULL != pGUNPC &&
				(true == pGUNPC->GetIsBosRaidNPC() ||
				true == pGUNPC->GetIsMiddleBosRaidNPC())
				 )
			{
				swprintf( timeBuf, L"%s", pGUNPC->GetUnitName().c_str());
				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 600, iPosY-20, timeBuf, D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

				map<UidType,float> mapDamagedMap = pGUNPC->GetDamagedMap();
				map<UidType,float>::const_iterator iter = mapDamagedMap.begin();
				for ( ; iter != mapDamagedMap.end(); ++iter )
				{
					CX2GUUser* pUser = g_pX2Game->GetUserUnitByUID( iter->first );
					if( NULL != pUser )
					{
						swprintf( timeBuf, L"%s, Damage : %d", pUser->GetUnitName().c_str(), static_cast<int>(iter->second) );
						if( g_pMain->GetFontForDebuf() != NULL )
							g_pMain->GetFontForDebuf()->OutTextXY( 600, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), 
							CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
						iPosY += 20;
					}
				} 
				break;
			}
		}
#endif // ADD_RAID_FIELD_LOG

		// 캐릭터 좌표
		const D3DVECTOR vMyUnitPos = g_pX2Game->GetMyUnit()->GetPos();
		swprintf( timeBuf, L"MyUnitPos X:%d Y:%d, Z:%d", static_cast<int>(vMyUnitPos.x), static_cast<int>(vMyUnitPos.y), static_cast<int>(vMyUnitPos.z) );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 480, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		// 카메라 좌표
		const D3DXVECTOR3* vCameraPos = (true == g_pX2Game->GetFreeCamera() && NULL != g_pX2Game->GetFPSCamera()) ? g_pX2Game->GetFPSCamera()->GetEyePt() : NULL;
		if( NULL != vCameraPos )
		{
			swprintf( timeBuf, L"FreeCameraPos X:%d Y:%d, Z:%d", static_cast<int>(vCameraPos->x), static_cast<int>(vCameraPos->y), static_cast<int>(vCameraPos->z) );
			if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( 40, 500, timeBuf, D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
		}
	}
	else
	{
		// 카메라 좌표
		if( NULL != g_pTFieldGame && true == g_pTFieldGame->GetFreeCamera() )
		{
			const D3DXVECTOR3* vCameraPos = ( NULL != g_pTFieldGame->GetFPSCamera() ) ? g_pTFieldGame->GetFPSCamera()->GetEyePt() : NULL;
			if( NULL != vCameraPos )
			{
				swprintf( timeBuf, L"FreeCameraPos X:%d Y:%d, Z:%d", static_cast<int>(vCameraPos->x), static_cast<int>(vCameraPos->y), static_cast<int>(vCameraPos->z) );
				if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( 40, 500, timeBuf, D3DXCOLOR(1,1,0,1), 
				CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
			}
		}
#ifdef ADD_STATE_WEDDING_GAME
		else if ( NULL != g_pWeddingGame && true == g_pWeddingGame->GetFreeCamera() )
		{
			const D3DXVECTOR3* vCameraPos = ( NULL != g_pWeddingGame->GetFPSCamera() ) ? g_pWeddingGame->GetFPSCamera()->GetEyePt() : NULL;
			if( NULL != vCameraPos )
			{
				swprintf( timeBuf, L"FreeCameraPos X:%d Y:%d, Z:%d", static_cast<int>(vCameraPos->x), static_cast<int>(vCameraPos->y), static_cast<int>(vCameraPos->z) );
				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 40, 500, timeBuf, D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
			}
		}
#endif // ADD_STATE_WEDDING_GAME

	}


#ifdef CHECK_CAMERA_INFO
	if( NULL != g_pX2Game )
	{
		int iPosX = 700;
		int iPosY = 540;

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//카메라 포커스 유닛
		if (NULL != g_pX2Game->GetFocusUnit() &&
			NULL != g_pX2Game->GetFocusUnit()->GetUnit())
		{
			swprintf( timeBuf, L"카메라 포커스 유닛 : %s",  g_pX2Game->GetFocusUnit()->GetUnit()->GetUnitData().m_NickName.c_str());
			if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 		
		}
		iPosY += 20;

		//카메라 포커스 유닛 포인터 주소
		if (NULL != g_pX2Game->GetFocusUnit() )
		{
			swprintf( timeBuf, L"GetFocusUnit 포인터 : %d",  g_pX2Game->GetFocusUnit() );
			if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 		
		}
		iPosY += 20;

		//GetMyUser 포인터 주소
		if (NULL != g_pX2Game->GetMyUnit() )
		{
			swprintf( timeBuf, L"GetMyUser 포인터: %d",  g_pX2Game->GetMyUnit() );
			if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 		
		}
		iPosY += 20;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//카메라 고정 여부
// 		if( true == g_pX2Game->GetFixedCamera() )
// 			swprintf( timeBuf, L"카메라 고정 = true" ) ;
// 		else
// 			swprintf( timeBuf, L"카메라 고정 = false" ) ;
// 
// 		if( g_pMain->GetFontForDebuf() != NULL )
// 			g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 		
// 		iPosY += 20;

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//라인맵 카메라 적용 상태 여부

		if( NULL != g_pX2Game->GetMyUnit() )
		{
			if( true == g_pX2Game->GetMyUnit()->GetIsLineMapCamera() )
				swprintf( timeBuf, L"라인맵 적용 상태 여부 = true" ) ;
			else
				swprintf( timeBuf, L"라인맵 적용 상태 여부 = false" ) ;

			if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 	
		}
		iPosY += 20;

		if( NULL != g_pX2Game->GetX2Camera() )
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////
			//월드 카메라 적용 여부
			if( true == g_pX2Game->GetX2Camera()->GetEnableWorldCamera() )
				swprintf( timeBuf, L"월드 카메라 활성 여부 = true" ) ;
			else
				swprintf( timeBuf, L"월드 카메라 활성 여부 = false" ) ;

			if( g_pMain->GetFontForDebuf() != NULL )
				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 
			iPosY += 20;


// 			if( true == g_pX2Game->GetX2Camera()->GetHoldViewWorldCamera() )
// 				swprintf( timeBuf, L"월드 카메라 고정 여부 = true" ) ;
// 			else
// 				swprintf( timeBuf, L"월드 카메라 고정 여부 = false" ) ;
// 
// 			if( g_pMain->GetFontForDebuf() != NULL )
// 				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 	
// 			iPosY += 20;

// 			if( true == g_pX2Game->GetX2Camera()->GetWorldCameraEdit() )
// 				swprintf( timeBuf, L"월드 카메라 치트 여부 = true" ) ;
// 			else
// 				swprintf( timeBuf, L"월드 카메라 치트 여부 = false" ) ;
// 
// 			if( g_pMain->GetFontForDebuf() != NULL )
// 				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 		
// 			iPosY += 20;
// 
// 			if( true == g_pX2Game->GetX2Camera()->GetNpcTrackingCamera() )
// 				swprintf( timeBuf, L"NPC 카메라 적용 여부 = true" ) ;
// 			else
// 				swprintf( timeBuf, L"NPC 카메라 적용 여부 = false" ) ;
// 
// 			if( g_pMain->GetFontForDebuf() != NULL )
// 				g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 		
// 			iPosY += 20;


			///////////////////////////////////////////////////////////////////////////////////////////////////
			if (NULL != g_pX2Game->GetFocusUnit() )
			{
				D3DXVECTOR3 vPos = g_pX2Game->GetFocusUnit()->GetPos();
				swprintf( timeBuf, L"FocusUnit Pos = %.1f, %.1f, %.1f", vPos.x, vPos.y, vPos.z );

				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 	
			}
			iPosY += 20;

			//카메라 위치
			if( NULL != g_pX2Game->GetX2Camera()->GetCamera() )
			{			
				D3DXVECTOR3 vPos = g_pX2Game->GetX2Camera()->GetCamera().GetEye();
				swprintf( timeBuf, L"EYE = %.1f, %.1f, %.1f", vPos.x, vPos.y, vPos.z );

				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 	
				iPosY += 20;

				vPos = g_pX2Game->GetX2Camera()->GetCamera().GetLookAt();
				swprintf( timeBuf, L"LOOKAT = %.1f, %.1f, %.1f", vPos.x, vPos.y, vPos.z );

				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( iPosX, iPosY, timeBuf, D3DXCOLOR(1,1,0,1), CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 	
				iPosY += 20;
			}
		}
	}
#endif //CHECK_CAMERA_INFO

#ifndef _SERVICE_

	if( g_pKTDXApp != NULL && g_pKTDXApp->GetDevice() != NULL )
	{
		g_fLastUpdateTime_MemInfo += g_pKTDXApp->GetElapsedTime();
		if( g_fLastUpdateTime_MemInfo > 10.f ) 
		{
			g_fLastUpdateTime_MemInfo = 0.f;

			g_MemStatus.dwLength = sizeof( g_MemStatus );
			GlobalMemoryStatus( &g_MemStatus );

			g_pKTDXApp->GetVideoRam( g_iTotalTextureMem, g_iAvailableTextureMem );

			g_iAvailableTextureMem2 = g_pKTDXApp->GetDevice()->GetAvailableTextureMem();
		}
#ifdef CHECK_PROCESS_MEMORY_USAGE   
		PROCESS_MEMORY_COUNTERS_EX pmcex;
		GetProcessMemoryInfo( GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmcex, sizeof(pmcex)); 

		//최고 메모리
		swprintf( timeBuf, L"Peak WorkingSet : %d (MB)", (pmcex.PeakWorkingSetSize)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 540, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT ); 

		//현재 메모리
		swprintf( timeBuf, L"Currnet WorkingSet : %d (MB)", (pmcex.WorkingSetSize)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 560, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );  

		//가상메모리
		swprintf( timeBuf, L"Virtual Memory : %d (MB)", (pmcex.PrivateUsage)/1024/1024);
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 580, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );  
#else
		swprintf( timeBuf, L"PM : %d/%d (MB)", (g_MemStatus.dwTotalPhys-g_MemStatus.dwAvailPhys)/1024/1024, 
			g_MemStatus.dwTotalPhys/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 540, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );

		swprintf( timeBuf, L"VM : %d/%d (MB)", (g_MemStatus.dwTotalVirtual-g_MemStatus.dwAvailVirtual)/1024/1024,
			g_MemStatus.dwTotalVirtual/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 560, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
#endif CHECK_PROCESS_MEMORY_USAGE 
		swprintf( timeBuf, L"Video M : %d/%d (MB) TM : %d(MB)", (g_iTotalTextureMem-g_iAvailableTextureMem)/1024/1024, g_iTotalTextureMem/1024/1024, g_iAvailableTextureMem2/1024/1024 );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 600, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pData->GetDungeonRoom() != NULL && g_pX2Game != NULL && CX2Game::GT_DUNGEON == g_pX2Game->GetGameType() )
	{
		CX2DungeonGame* pDungeonGame = static_cast<CX2DungeonGame*>(g_pX2Game);
		if( pDungeonGame->GetDungeon() != NULL )
		{
			int iStageIndex = pDungeonGame->GetDungeon()->GetNowStageIndex();

			if( pDungeonGame->GetDungeon()->GetNowStage() != NULL )
			{
				int iSubStageIndex = pDungeonGame->GetDungeon()->GetNowStage()->GetNowSubStageIndex();
				swprintf( timeBuf, L"stage : %d-%d", iStageIndex, iSubStageIndex );
				if( g_pMain->GetFontForDebuf() != NULL )
					g_pMain->GetFontForDebuf()->OutTextXY( 40, 620, timeBuf, D3DXCOLOR(1,1,0,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
			}
		}
	}

	if( g_pX2Game != NULL && g_pX2Game->GetMyUnit() != NULL )
	{
		int iLineIndex = (int)g_pX2Game->GetMyUnit()->GetSyncData().lastTouchLineIndex;

		swprintf( timeBuf, L"line : %d", iLineIndex );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 640, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( g_pKTDXApp->GetDGManager() != NULL )
	{
		int nNum = g_pKTDXApp->GetDGManager()->GetRenderObjectChainSize();
		int nAlpha = g_pKTDXApp->GetDGManager()->GetAlphaObjectChainSize();
		int nNonAlpha = g_pKTDXApp->GetDGManager()->GetNonAlphaObjectChainSize();        
//{{ robobeg : 2008-10-18
        int nCull = g_pKTDXApp->GetDGManager()->GetCullCount();
		swprintf( timeBuf, L"obj chain(a/n): %d(%d,%d), Cull: %d", nNum, nAlpha, nNonAlpha, nCull );
//}} robobeg : 2008-10-18

        
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 660, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( NULL != g_pData &&
		NULL != g_pData->GetGameMajorParticle() )
	{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        int nMajorSeqNum =  g_pData->GetGameMajorParticle()->EstimateParticleSequenceNum();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		int nMajorSeqNum =  g_pData->GetGameMajorParticle()->GetParticleSequenceNum();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		swprintf( timeBuf, L"major p: %d", nMajorSeqNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 680, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( NULL != g_pData &&
		NULL != g_pData->GetGameMinorParticle() )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        int nMinorSeqNum =  g_pData->GetGameMinorParticle()->EstimateParticleSequenceNum();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		int nMinorSeqNum =  g_pData->GetGameMinorParticle()->GetParticleSequenceNum();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		swprintf( timeBuf, L"minor p: %d", nMinorSeqNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 700, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( NULL != g_pData &&
		NULL != g_pData->GetGameMajorXMeshPlayer() )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        int nMajorInstanceNum = g_pData->GetGameMajorXMeshPlayer()->EstimateInstanceNum();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		int nMajorInstanceNum = g_pData->GetGameMajorXMeshPlayer()->GetInstanceNum();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		swprintf( timeBuf, L"major m: %d", nMajorInstanceNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 720, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

	if( NULL != g_pData &&
		NULL != g_pData->GetGameMinorXMeshPlayer() )
	{		
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
        int nMinorInstanceNum =  g_pData->GetGameMinorXMeshPlayer()->EstimateInstanceNum();
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
		int nMinorInstanceNum =  g_pData->GetGameMinorXMeshPlayer()->GetInstanceNum();
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE

		swprintf( timeBuf, L"minor m: %d", nMinorInstanceNum );
		if( g_pMain->GetFontForDebuf() != NULL )
			g_pMain->GetFontForDebuf()->OutTextXY( 40, 740, timeBuf, D3DXCOLOR(1,1,0,1), 
			CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
	}

#endif	

	/*
	TODO: add UI text as needed
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	txtHelper.DrawTextLine( L"Put whatever misc status here" );

	// Draw help
	const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetBackBufferSurfaceDesc();
	if( g_bShowHelp )
	{
	txtHelper.SetInsertionPos( 10, pd3dsdBackBuffer->Height-15*6 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.75f, 0.0f, 1.0f ) );
	txtHelper.DrawTextLine( L"Controls (F1 to hide):" );

	txtHelper.SetInsertionPos( 40, pd3dsdBackBuffer->Height-15*5 );
	txtHelper.DrawTextLine( L"Quit: ESC" );
	}
	else
	{
	txtHelper.SetInsertionPos( 10, pd3dsdBackBuffer->Height-15*2 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
	txtHelper.DrawTextLine( L"Press F1 for help" );
	}
	*/
	//   txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	// Always allow dialog resource manager calls to handle global messages
	// so GUI state is updated correctly
	//*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
	//if( *pbNoFurtherProcessing )
	//    return 0;
#ifndef REMOVE_DXUT_SAMPLE_CODE
	if( g_SettingsDlg.IsActive() )
	{
		g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
		return 0;
	}
#endif  REMOVE_DXUT_SAMPLE_CODE

	if( true == g_bRenderEtc )
	{
#ifndef  REMOVE_DXUT_SAMPLE_CODE
		// Give the dialogs a chance to handle the message first
		*pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
		if( *pbNoFurtherProcessing )
		{
			return 0;
		}

		*pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
		if( *pbNoFurtherProcessing )
		{			
			return 0;
		}
#endif  REMOVE_DXUT_SAMPLE_CODE
	}

	if( g_pKTDXApp != NULL )
		*pbNoFurtherProcessing = g_pKTDXApp->MsgProc( hWnd, uMsg, wParam, lParam );
	if( *pbNoFurtherProcessing )
		return 0;

	return 0;
}




//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
#ifdef PROFILE_BUILD
	if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() == CX2User::XUAL_DEV )
	{
		KProfileMan::GetSingleton().OnKeyboardProc( nChar, bKeyDown, bAltDown );
	}
#endif

	if( bKeyDown )
	{
		switch( nChar )
		{
			//case VK_F1: g_bShowHelp = !g_bShowHelp; break;
		case VK_HOME: 
			{
				if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_SPECIAL_USER )
				{
					g_bRenderEtc = !g_bRenderEtc; 
					g_pData->ToggleRenderEtc();
#ifdef NETERROR_KOREAN_HELP
					SiCKTDXStringTable()->GetInstance()->SetEnableDebugInfo(g_bRenderEtc);
#endif NETERROR_KOREAN_HELP
					if( true == g_bRenderEtc )
					{
						g_pChatBox->HideChatLog();
					}
				}
			}
			break;

#ifdef KHELP_SYSTEM_TEST
        case VK_PRIOR:
			if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
                // Shift+End 키를 누른 경우 KHelpSystem을 토글한다.
                if ( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
                {
                    // toggle help system
                    _KHelpSystem.EnableDisplayHelp( !_KHelpSystem.IsDisplayHelp() );
                }
                else
                {
                    // KHelpSystem이 활성화된 상태에서 End키를 누르면 다음 help를 활성화시킨다.
                    if ( _KHelpSystem.IsDisplayHelp() )
                    {
                        _KHelpSystem.PrevHelp();
                    }//if
                }//if.. else..
			}//if
            break;

        case VK_NEXT:
			if ( g_pData->GetMyUser() != NULL && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
			{
                // Shift+End 키를 누른 경우 KHelpSystem을 토글한다.
                if ( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
                {
                    // toggle help system
                    _KHelpSystem.EnableDisplayHelp( !_KHelpSystem.IsDisplayHelp() );
                }
                else
                {
                    // KHelpSystem이 활성화된 상태에서 End키를 누르면 다음 help를 활성화시킨다.
                    if ( _KHelpSystem.IsDisplayHelp() )
                    {
                        _KHelpSystem.NextHelp();
                    }//if
                }//if.. else..
			}//if
            break;
#endif // KHELP_SYSTEM_TEST

//#ifdef NEW_VILLAGE
//				else if( g_pTFieldGame != NULL && g_pTFieldGame->GetCamera() != NULL )
//				{
//					g_pTFieldGame->GetCamera()->SetUseLineCamera( !g_pTFieldGame->GetCamera()->GetUseLineCamera() );
//				}
//#endif
//			} break;
//
//#endif NEW_VILLAGE_RENDERING_TEST
		}
	}
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
#ifndef REMOVE_DXUT_SAMPLE_CODE
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	switch( nControlID )
	{
	case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
	case IDC_TOGGLEREF:        DXUTToggleREF(); break;
	case IDC_CHANGEDEVICE:     
		{
			//ShowCursor( true );
			g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() );
		}
		break;
	}
}
#endif  REMOVE_DXUT_SAMPLE_CODE

//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	g_DialogResourceManager.OnLostDevice();
	g_SettingsDlg.OnLostDevice();
#endif  REMOVE_DXUT_SAMPLE_CODE

	g_pKTDXApp->OnLostDevice();

#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnLostDevice();
#endif

}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
#ifndef REMOVE_DXUT_SAMPLE_CODE
	g_DialogResourceManager.OnDestroyDevice();
	g_SettingsDlg.OnDestroyDevice();
#endif  REMOVE_DXUT_SAMPLE_CODE

	SAFE_DELETE( g_pKTDXApp );

	KNP_Close();

	SAFE_DELETE( g_pX2SMTPMail );

#ifdef PROFILE_BUILD
	KProfileMan::GetSingleton().OnDestroyDevice();

	KProfileMan::GetSingleton().SetProfileIterator( NULL );
#endif


//{{ robobeg : 2008-10-10
    SAFE_RELEASE( g_pEffectStateManager );
	g_pd3dDevice = NULL;
//}} robobeg : 2008-10-10

#ifdef _SERVICE_
	HWND hWeb = ::FindWindow(NULL, L"Elsword - Windows Internet Explorer");
	if( hWeb != NULL )
	{
		::ShowWindow(hWeb, SW_RESTORE);
	}
#endif _SERVICE_
}

//--------------------------------------------------------------------------------------
// HackShield 가 가동되면 HackShield 가 그 권한을 올려 주기 때문에 별도의 작업이 필요없지만
// HackShield 없는 상태에서 테스트 목적으로 리부팅을 해보시려면 SeDebug 권한을 얻는 작업을 진행해야 합니다.
// 아래 ObtainSeDebugPrivilege() 함수를 선언해두시고 게임 클라이언트 초기화 루틴에서 한번만 불러 주면
// 별도의 include 도 필요 없고 함수로 모듈화 되어있으니 그대로 복사하시고 사용하시면 됨.
//--------------------------------------------------------------------------------------
#ifdef OBTAIN_SEDEBUG_PRIVILEGE
bool ObtainSeDebugPrivilege()
{
	BOOL Result;
	TOKEN_PRIVILEGES TokenPrivileges;
	TOKEN_PRIVILEGES PreviousTokenPrivileges;
	LUID luid;
	HANDLE hToken;
	DWORD dwPreviousTokenPrivilegesSize = sizeof(TOKEN_PRIVILEGES);

	Result = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	if (Result == FALSE)
		return false;

	Result = LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	if (Result == FALSE)
	{
		CloseHandle(hToken);
		return false;
	}

	TokenPrivileges.PrivilegeCount            = 1;
	TokenPrivileges.Privileges[0].Luid        = luid;
	TokenPrivileges.Privileges[0].Attributes  = 0;

	AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES),
		&PreviousTokenPrivileges, &dwPreviousTokenPrivilegesSize);

	if (GetLastError() != ERROR_SUCCESS)
		return false;

	PreviousTokenPrivileges.PrivilegeCount             = 1;
	PreviousTokenPrivileges.Privileges[0].Luid         = luid;
	PreviousTokenPrivileges.Privileges[0].Attributes  |= SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &PreviousTokenPrivileges,
		dwPreviousTokenPrivilegesSize, NULL, NULL);

	if (GetLastError() != ERROR_SUCCESS)
		return false;

	CloseHandle(hToken);

	return true;
}
#endif OBTAIN_SEDEBUG_PRIVILEGE


