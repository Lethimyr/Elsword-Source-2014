#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <fstream>
#include <map>
#include <sstream>

#include "define.h"
#include "Registry.h"
#include "KMsgBox.h"
#include "ThreadDefs.h"
#include "KAssert.h"
#include "KPatchInfoBox.h"
#include "PatcherStringSet.h"
#include "patch.h"
#include "time.h"

#ifdef PATCHER_CHANNELING_AERIA
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#endif //PATCHER_CHANNELING_AERIA

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "libxml_smtd.lib" )
#else
#pragma comment( lib, "libxml_smt.lib" )
#endif
// Define macro constants.
#define WND_WIDTH   740                 ///< Width of download window.
#define WND_HEIGHT  556		           ///< Height of download height.

#define UPDATE_POP_UP_BUTTON_X			33
#define UPDATE_POP_UP_BUTTON_Y			197
#define UPDATE_POP_UP_BUTTON_WIDTH		134
#define UPDATE_POP_UP_BUTTON_HEIGHT		47

#define PATCH_CANCEL_BUTTON_X			754
#define PATCH_CANCEL_BUTTON_Y			14
#define PATCH_CANCEL_BUTTON_WIDTH		25
#define PATCH_CANCEL_BUTTON_HEIGHT		24

#define PATCH_NEW_CANCEL_BUTTON_X			711
#define PATCH_NEW_CANCEL_BUTTON_Y			15
#define PATCH_NEW_CANCEL_BUTTON_WIDTH		25
#define PATCH_NEW_CANCEL_BUTTON_HEIGHT		22

#define PATCH_MINIMIZE_BUTTON_X			725
#define PATCH_MINIMIZE_BUTTON_Y			14
#define PATCH_MINIMIZE_BUTTON_WIDTH		25
#define PATCH_MINIMIZE_BUTTON_HEIGHT	24

#define PATCH_NEW_MINIMIZE_BUTTON_X			681
#define PATCH_NEW_MINIMIZE_BUTTON_Y			14
#define PATCH_NEW_MINIMIZE_BUTTON_WIDTH		25
#define PATCH_NEW_MINIMIZE_BUTTON_HEIGHT	22

extern "C"
{
	long DisplayHTMLPage( HWND hWnd, LPTSTR webPageName );
	void ResizeBrowser( HWND hWnd, DWORD width, DWORD height );
	long EmbedBrowserObject( HWND hWnd );
	void UnEmbedBrowserObject( HWND hWnd );
	void DoPageAction(HWND hWnd, DWORD action);
}


// Variable declarations.
KPatcher g_patcher;// PatcherLib
HANDLE      g_hMutex = NULL;    ///< Mutex of game client program.
//std::ofstream       g_logStream;        ///< Log file stream.
bool				g_bTestMode = true;

HMODULE				g_hUnicowsModule;
HINSTANCE			g_hInstance = NULL;
int					g_nCmdShow = 0;
HWND				g_hWnd;
LPSTR				g_lpCmdLine = NULL;

bool				g_bUpdatePopUpButtonDown = false;
bool				g_bPatchCancelButtonDown = false;
bool				g_bPatchCancelButtonOver = false;

bool				g_bUpdating = false;

float g_fTotalDownloadRatio = 0;
float g_fFileDownloadRatio = 0;
float g_fCheckFileRatio = 0;

bool g_bCheckFileRatioFull = false;
std::string g_CurFilename = "";

//KHttpWrapper		g_HttpWrapper;

#ifdef LOAD_WEB_MAIN
KBrowserWrapper		g_browser; ///< HTML file browser
#endif LOAD_WEB_MAIN


// Function declarations.
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
DWORD   WINAPI   ThreadProc( LPVOID lpParameter );
bool             IsDXVerCheck( void );
bool			 IsDotNetVerCheck( void );
void             Log( const char* szLogMsg );
void             LogError( const char* szLogMsg );

/// Application's entry point.
/// @param hInstance handle to current instance
/// @param hPrevInstance handle to previous instance
/// @param lpCmdLine command line
/// @param nCmdShow show state
/// @return WM_QUIT message's wParam parameter or zero
int APIENTRY WinMain( HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine,
                      int nCmdShow )
{

	g_lpCmdLine = lpCmdLine;


#ifdef PATCHER_CHANNELING_AERIA	
	char path[2048];
	LPSTR fileptr;	
	GetFullPathNameA(".",2048,path,&fileptr);
	std::string aeriaPath = path;
	aeriaPath = aeriaPath + "\\ESAERIA.exe";
	if( true == PathFileExistsA(aeriaPath.c_str()) && __argc == 1 )
	{
		/* 실행시키기
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);

		ZeroMemory( &pi, sizeof(pi) );

		std::string workingpath = aeriaPath.substr(0,aeriaPath.find_last_of("\\"));

		CreateProcessA(NULL,(LPSTR)aeriaPath.c_str(),NULL,NULL,FALSE,0,NULL,workingpath.c_str(),&si,&pi);
		*/
		MessageBoxA(NULL, "To play Elsword, please run ESAERIA.EXE.", "", MB_OK);
		return 0;
	}	
#endif //PATCHER_CHANNELING_AERIA

#if defined(PATCHER_COUNTRY_DE)
	#include "PatcherStringSet\PatcherStringSet_DE.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_FR)
	#include "PatcherStringSet\PatcherStringSet_FR.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_IT)
	#include "PatcherStringSet\PatcherStringSet_IT.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_PL)
	#include "PatcherStringSet\PatcherStringSet_PL.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_ES)
	#include "PatcherStringSet\PatcherStringSet_ES.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_EN)
	#include "PatcherStringSet\PatcherStringSet_EN.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_US)

// 미국은 클라이언트 로그인 방식

///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_JP)
	#include "PatcherStringSet\PatcherStringSet_US.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_ID)
	#include "PatcherStringSet\PatcherStringSet_ID.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_TH)
	#include "PatcherStringSet\PatcherStringSet_TH.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_BR)
	#include "PatcherStringSet\PatcherStringSet_BR.h"
///////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(PATCHER_COUNTRY_PH)
	#include "PatcherStringSet\PatcherStringSet_PH.h"

#ifdef PATCHER_RUN_GARENA_MESSENGER

	std::string str = (std::string) __argv[1];
	std::string strCheck = "";
	std::string strCheckLength = "";
	if (str.length() != 0 && str.length() > 7)
	{
		strCheck = str.substr(0,7);
		strCheckLength = str.substr(7,str.length());
		
		if ( strcmp(strCheck.c_str(), "-token:") != 0 || strCheckLength.length() < 300)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}

#endif //PATCHER_RUN_GARENA_MESSENGER

#elif defined(PATCHER_COUNTRY_TW) || defined(PATCHER_COUNTRY_HK)
	#include "PatcherStringSet\PatcherStringSet_TW.h"
#else
	#include "PatcherStringSet\PatcherStringSet_US.h"
#endif //PATCHER_COUNTRY_JP_XX


	g_nCmdShow = nCmdShow;
	

    // 사내 테스트 모드에서는 로그파일을 생성한다.
//#if defined(PATCHER_TEST) || defined(KR_QA_TEST_PATCHER)
//        g_logStream.open( "__LOG_Elsword_Patcher__.txt" );
//#endif

#ifdef BLOCK_MANUAL_LOGIN
	if(__argv[1] == NULL)	// NULL 이외에 체크 할 수 있는 데이터가 있으면 그걸로 체크해보자.
	{
#ifdef BLOCK_MANUAL_LOGIN_BYPASS_FOR_TEST_SERVER
		g_lpCmdLine = "1";
#else BLOCK_MANUAL_LOGIN_BYPASS_FOR_TEST_SERVER
		PostQuitMessage(0);
#endif BLOCK_MANUAL_LOGIN_BYPASS_FOR_TEST_SERVER
	}
#endif BLOCK_MANUAL_LOGIN
    // 패치 작업 전에 메세지 박스를 띄워놓는다.
    //KMsgBox::Open( STRINGMAIN02,
    //               STRINGMAIN03 );

    // 뮤텍스로 게임 클라이언트가 이미 실행 중인지 확인한다.
	g_hMutex = ::CreateMutex( NULL, TRUE, _T(MUTEX_NAME) );

	//{{ 허상형 : [2009/9/18] //	사내 서버에서는 중복 실행 가능하게
#if !( defined(PATCHER_TEST) || defined(KR_QA_TEST_PATCHER) )
    if ( ( NULL == g_hMutex ) || ( ERROR_ALREADY_EXISTS == ::GetLastError() ) )
    {
        //KMsgBox::Close();
        //KMsgBox::Open( STRINGMAIN04,
        //               STRINGMAIN05,
        //               KMsgBox::MBOX_OK );

		MessageBoxW( g_hWnd, STRINGMAIN04, STRINGMAIN05, MB_OK );
        return 0; // exit
    }
#endif	//	PATCHER_TEST
	//}} 허상형 : [2009/9/18] //	사내 서버에서는 중복 실행 가능하게


    // 윈도우 클래스를 등록한다.
    WNDCLASSEX wndClsEx    = { 0, };
    wndClsEx.cbSize        = sizeof( WNDCLASSEX );
//    wndClsEx.hbrBackground = reinterpret_cast<HBRUSH>( ::GetStockObject( WHITE_BRUSH ) );
	wndClsEx.hbrBackground = NULL;
    wndClsEx.hCursor       = ::LoadCursor( NULL, IDC_ARROW );
    wndClsEx.hIcon         = ::LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );
    wndClsEx.hInstance     = hInstance;
    wndClsEx.lpfnWndProc   = WndProc;
    wndClsEx.lpszClassName = _T("ElswordPatcher");
    ::RegisterClassEx( &wndClsEx );

    // 윈도우 크기와 위치를 조절한다. 화면 중앙에 위치시킨다.
    RECT rcSize;
    RECT rcPos;
    ::SetRect( &rcSize, 0, 0, WND_WIDTH, WND_HEIGHT );
    ::AdjustWindowRect( &rcSize, WS_POPUP, FALSE );
    ::GetClientRect( ::GetDesktopWindow(), &rcPos );
    rcPos.left = static_cast<LONG>( ( rcPos.right  - rcSize.right  ) / 2 );
    rcPos.top  = static_cast<LONG>( ( rcPos.bottom - rcSize.bottom ) / 2 );

	// 윈도우를 만든다. (윈도우 생성과 동시에 다운로드 쓰레드가 실행된다.)
	g_hWnd = ::CreateWindowEx( 0, _T("ElswordPatcher"), _T("Elsword"), WS_POPUP | WS_CLIPCHILDREN,
		rcPos.left, rcPos.top, rcSize.right, rcSize.bottom,
		::GetDesktopWindow(), NULL, hInstance, NULL ) ;
    
    if ( NULL == g_hWnd )
    {
        //KMsgBox::Close();
        //KMsgBox::Open( STRINGMAIN12,
        //               STRINGMAIN13,
        //               KMsgBox::MBOX_OK );

		MessageBoxW( g_hWnd, STRINGMAIN12, STRINGMAIN13, MB_OK );
        return 0; // exit
    }

#ifdef LOAD_WEB_MAIN
	g_browser.Create(const_cast<LPTSTR>( g_pcBannerLink ), 
		BANNER_X, BANNER_Y, BANNER_WIDTH, BANNER_HEIGHT,
		g_hWnd, KBrowserWrapper::BROWSER_TYPE_CHILD );
#endif LOAD_WEB_MAIN

    // 메인 메세지 루프를 돈다.
    MSG msg;
    while ( TRUE == ::GetMessage( &msg, NULL, 0, 0 ) )
    {
        //::TranslateMessage( &msg ); // 필요없다.
        ::DispatchMessage( &msg );
    }

    // 윈도우 클래스를 해제.
    ::UnregisterClass( _T("ElswordPatcher"), hInstance );

    // 테스트 모드에서 생성된 로그파일을 닫는다.
//#if defined(PATCHER_TEST) || defined(KR_QA_TEST_PATCHER)
//        g_logStream.close();
//#endif

	if ( NULL != g_hMutex )
	{
		::ReleaseMutex( g_hMutex );
		::CloseHandle( g_hMutex );
		g_hMutex = NULL;
	}


    return (int)msg.wParam; // exit
}

/// Application-defined function that processes messages sent to a window.
/// @param hWnd handle to window
/// @param uMsg message identifier
/// @param wParam first message parameter
/// @param lParam second message parameter
/// @return the result of the message processing
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    #define SAFE_DELETE_OBJECT(h) { if (h) { ::DeleteObject(h); (h) = NULL; } }
    #define SAFE_CLOSE_HANDLE(h)  { if (h) { ::CloseHandle(h);  (h) = NULL; } }

    static HANDLE  hThread  = NULL;
    static HBITMAP bkgndBmp = NULL;
	static HBITMAP gaugeCheckBmp = NULL;
	static HBITMAP gaugeCheckEffectBmp = NULL;
	static HBITMAP gaugeUpdateBmp = NULL;
	static HBITMAP gaugeUpdateEffectBmp = NULL;
	static HBITMAP messageCheckingBmp = NULL;
	static HBITMAP messageUpdatingBmp = NULL;
	static HBITMAP gaugeTotalUpdateBmp = NULL;

	static HBITMAP hBackBit = NULL;

	static int g_MousePosX = 0;
	static int g_MousePosY = 0;

    switch ( uMsg )
    {
        case WM_CREATE:
        {
            DWORD dwThreadID;
			g_hUnicowsModule = LoadLibraryA("unicows.dll");
			HRGN hWndRgn;

			// 윈도우 크기와 위치를 조절한다. 화면 중앙에 위치시킨다.
			RECT rcSize;
			RECT rcPos;
			::SetRect( &rcSize, 0, 0, WND_WIDTH, WND_HEIGHT );
			::GetClientRect( hWnd, &rcPos );
			rcPos.left = static_cast<LONG>( ( rcPos.right  - rcSize.right  ) / 2 );
			rcPos.top  = static_cast<LONG>( ( rcPos.bottom - rcSize.bottom ) / 2 );

			hWndRgn = CreateRoundRectRgn( rcPos.left, rcPos.top , rcPos.left + WND_WIDTH , rcPos.top + WND_HEIGHT, 9, 9 );
			SetWindowRgn( hWnd, hWndRgn, FALSE );
			
            if ( NULL == ( bkgndBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_BACKGROUND ) ) ) )
                return -1;

            if ( NULL == ( gaugeCheckBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_GAUGE_CHECK ) ) ) )
                return -1;

			if ( NULL == ( gaugeCheckEffectBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_GAUGE_CHECK_EFFECT ) ) ) )
				return -1;

			if ( NULL == ( gaugeUpdateBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_GAUGE_UPDATE ) ) ) )
				return -1;

			if ( NULL == ( gaugeUpdateEffectBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_GAUGE_UPDATE_EFFECT ) ) ) )
				return -1;

			if ( NULL == ( messageCheckingBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_MESSAGE_CHECKING ) ) ) )
				return -1;

			if ( NULL == ( messageUpdatingBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_MESSAGE_UPDATING ) ) ) )
				return -1;

			if ( NULL == ( gaugeTotalUpdateBmp = ::LoadBitmap( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDB_GAUGE_TOTAL_UPDATE ) ) ) )
				return -1;

            if ( NULL == ( hThread = _BEGINTHREADEX( NULL, 0,
                                                     ThreadProc,
                                                     NULL, 0, &dwThreadID ) ) )
            {
                return -1;
            }

            return 0; // success
        }

        case WM_DESTROY:
        {
            SAFE_DELETE_OBJECT( bkgndBmp );
			SAFE_DELETE_OBJECT( gaugeCheckBmp );
			SAFE_DELETE_OBJECT( gaugeCheckEffectBmp );
			SAFE_DELETE_OBJECT( gaugeCheckBmp );
			SAFE_DELETE_OBJECT( gaugeCheckEffectBmp );
			SAFE_DELETE_OBJECT( gaugeUpdateBmp );
			SAFE_DELETE_OBJECT( gaugeUpdateEffectBmp );
			SAFE_DELETE_OBJECT( messageCheckingBmp );
			SAFE_DELETE_OBJECT( messageUpdatingBmp );
			SAFE_DELETE_OBJECT( gaugeTotalUpdateBmp );

			SAFE_CLOSE_HANDLE( hThread );
			FreeLibrary( g_hUnicowsModule );
		
            ::PostQuitMessage( 0 );
            return 0;
        } break;

        case WM_MOUSEMOVE:
        {
			g_MousePosX = LOWORD(lParam);
			g_MousePosY = HIWORD(lParam);

            if ( wParam & MK_LBUTTON )
            {
				if ( g_bPatchCancelButtonDown == false && g_bUpdatePopUpButtonDown == false )
				{
					::SendMessage( hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam );
				}
            }

            return 0;
        } break;
/*
		case WM_RBUTTONUP:
			{
				// 패치 정보를 알려준다.
				KMsgBox::Close();
				if ( IDCANCEL == KPatchInfoBox::Open() ) return -1;
				
				return 0;
			}

			*/
		case WM_LBUTTONDOWN:
			{	
				g_MousePosX = LOWORD(lParam);
				g_MousePosY = HIWORD(lParam);

				return 0;
			} break;

		case WM_LBUTTONUP:
			{	
				g_MousePosX = LOWORD(lParam);
				g_MousePosY = HIWORD(lParam);

				if ( g_MousePosX > UPDATE_POP_UP_BUTTON_X && g_MousePosY > UPDATE_POP_UP_BUTTON_Y && 
					g_MousePosX < UPDATE_POP_UP_BUTTON_X + UPDATE_POP_UP_BUTTON_WIDTH &&
					g_MousePosY < UPDATE_POP_UP_BUTTON_Y + UPDATE_POP_UP_BUTTON_HEIGHT && g_bUpdatePopUpButtonDown == true )
				{
					/*
					// 패치 정보를 알려준다.
					KMsgBox::Close();
					if ( IDCANCEL == KPatchInfoBox::Open() ) return -1;
					*/
				}

				if ( g_MousePosX > PATCH_NEW_CANCEL_BUTTON_X && g_MousePosY > PATCH_NEW_CANCEL_BUTTON_Y && 
					g_MousePosX < PATCH_NEW_CANCEL_BUTTON_X + PATCH_NEW_CANCEL_BUTTON_WIDTH &&
					g_MousePosY < PATCH_NEW_CANCEL_BUTTON_Y + PATCH_NEW_CANCEL_BUTTON_HEIGHT )
				{
					g_patcher.Stop();
				}

				if ( g_MousePosX > PATCH_NEW_MINIMIZE_BUTTON_X && g_MousePosY > PATCH_NEW_MINIMIZE_BUTTON_Y && 
					g_MousePosX < PATCH_NEW_MINIMIZE_BUTTON_X + PATCH_NEW_MINIMIZE_BUTTON_WIDTH &&
					g_MousePosY < PATCH_NEW_MINIMIZE_BUTTON_Y + PATCH_NEW_MINIMIZE_BUTTON_HEIGHT )
				{
					ShowWindow( hWnd, SW_MINIMIZE );
				}

				::InvalidateRect(  hWnd, NULL, FALSE );

				g_bUpdatePopUpButtonDown = false;
				g_bPatchCancelButtonDown = false;

				return 0;
			} break;

		case WM_KEYDOWN:
        {
            if ( VK_ESCAPE == wParam && NULL != hThread )
            {
                if ( NULL != g_hMutex )
                {
                    ::ReleaseMutex( g_hMutex );
                    ::CloseHandle( g_hMutex );
					g_hMutex = NULL;
                }
            }
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            ::BeginPaint( hWnd, &ps );
            HDC memDC = ::CreateCompatibleDC( ps.hdc );	
			

			HBITMAP hOldBitmap;			
			RECT rcSize;
			GetClientRect(hWnd,&rcSize);
			if (hBackBit == NULL) 
			{
				hBackBit=CreateCompatibleBitmap(ps.hdc,WND_WIDTH,WND_HEIGHT);
			}
			hOldBitmap=(HBITMAP)SelectObject(memDC,hBackBit);
			FillRect(memDC,&rcSize,(HBRUSH)GetStockObject( WHITE_BRUSH ) );

			// 이미지 그리기 
			HDC memDC2 = ::CreateCompatibleDC( ps.hdc );
			HBITMAP oldBmp = reinterpret_cast<HBITMAP>( ::SelectObject( memDC2, bkgndBmp ) );
			BitBlt( memDC, 0, 0, WND_WIDTH, WND_HEIGHT, memDC2, 0, 0, SRCCOPY );

			//int iEffectPosX = LOAD_ALL_FILE_BAR_X + timeGetTime() % ( iWidth > 5 ? ( iWidth - 5 ) : 1 );
			if( g_bUpdating )
			{
				SelectObject( memDC2, messageUpdatingBmp );
				BitBlt( memDC, 20, 450, WND_WIDTH, WND_HEIGHT, memDC2, 0, 0, SRCCOPY );



				SelectObject( memDC2, gaugeTotalUpdateBmp );
				StretchBlt( memDC, LOAD_NEW_CUR_FILE_BAR_X, LOAD_NEW_CUR_FILE_BAR_Y, 
					static_cast<int>( g_fFileDownloadRatio * ( GAUGE_NEW_BAR_WIDTH ) ), LOAD_NEW_CUR_FILE_BAR_HEIGHT, 
					memDC2, 0, 0, 1, LOAD_NEW_CUR_FILE_BAR_HEIGHT, SRCCOPY );

				SelectObject( memDC2, gaugeUpdateBmp );
				StretchBlt( memDC, LOAD_NEW_ALL_FILE_BAR_X, LOAD_NEW_ALL_FILE_BAR_Y, 
					static_cast<int>( g_fTotalDownloadRatio * ( GAUGE_NEW_BAR_WIDTH ) ), LOAD_NEW_ALL_FILE_BAR_HEIGHT, 
					memDC2, 0, 0, 1, LOAD_NEW_ALL_FILE_BAR_HEIGHT, SRCCOPY );

				
				/////////////////////////// 2013.09.04 lygan_조성욱 // 예전에 해외팀 공통으로 사용하던 이미지용 ///////////////////////////
				/*StretchBlt( memDC, LOAD_ALL_FILE_BAR_X, LOAD_ALL_FILE_BAR_Y, iWidth, LOAD_ALL_FILE_BAR_HEIGHT, 
					memDC2, 0, 0, 5, LOAD_ALL_FILE_BAR_HEIGHT, SRCCOPY );*/

				//SelectObject( memDC2, gaugeUpdateEffectBmp );
				//BitBlt( memDC, iEffectPosX, LOAD_ALL_FILE_BAR_Y, 5, LOAD_ALL_FILE_BAR_HEIGHT, memDC2, 0, 0, SRCCOPY );
			}
			else
			{
				SelectObject( memDC2, messageCheckingBmp );
				BitBlt( memDC, 20, 450, WND_WIDTH, WND_HEIGHT, memDC2, 0, 0, SRCCOPY );

				//SelectObject( memDC2, gaugeUpdateBmp );
				////SelectObject( memDC2, gaugeCheckBmp );
				//int test = static_cast<int>( g_fTotalDownloadRatio * ( GAUGE_NEW_BAR_WIDTH ) );
				//StretchBlt( memDC, LOAD_NEW_ALL_FILE_BAR_X, LOAD_NEW_ALL_FILE_BAR_Y, 
				//	test, LOAD_NEW_ALL_FILE_BAR_HEIGHT, 
				//	memDC2, 0, 0, 1, LOAD_NEW_ALL_FILE_BAR_HEIGHT, SRCCOPY );

				/////////////////////////// 2013.09.04 lygan_조성욱 // 예전에 해외팀 공통으로 사용하던 이미지용 ///////////////////////////
				/*SelectObject( memDC2, gaugeCheckBmp );
				StretchBlt( memDC, LOAD_ALL_FILE_BAR_X, LOAD_ALL_FILE_BAR_Y, iWidth, LOAD_ALL_FILE_BAR_HEIGHT, 
				memDC2, 0, 0, 5, LOAD_ALL_FILE_BAR_HEIGHT, SRCCOPY );*/

				//SelectObject( memDC2, gaugeCheckEffectBmp );
				//BitBlt( memDC, iEffectPosX, LOAD_ALL_FILE_BAR_Y, 5, LOAD_ALL_FILE_BAR_HEIGHT, memDC2, 0, 0, SRCCOPY );
			}


			DeleteDC(memDC2);
			SetBkMode( memDC, TRANSPARENT );

			if( g_bUpdating )
			{
				HFONT font, oldfont;
				font= ::CreateFont(PATCHER_FONT_SIZE,0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,0,0,0,0,PATCHER_FONT);
				oldfont=(HFONT)SelectObject(memDC,font);
				::SetTextAlign( memDC, TA_TOP | TA_LEFT );
				::SetTextColor( memDC, 0xF0F0F0F0 );
				
				::TextOutA( memDC, 482, 450, g_CurFilename.c_str(), g_CurFilename.length() );
				char tmp[256];
				sprintf(tmp,"%d%%", (int)( ( g_fFileDownloadRatio + 0.005f ) * 100 ) );
				::TextOutA( memDC, 690 ,492 , tmp, (int)::strlen( tmp ) );
				
				char tmp2[256];
				//g_fTotalDownloadRatio
				sprintf(tmp2,"%d%%", (int)( ( g_fTotalDownloadRatio + 0.005f ) * 100 ) );
				::TextOutA( memDC, 690 ,513 , tmp2, (int)::strlen( tmp2 ) );


				/*if( g_fFileDownloadRatio > 0.05f )
				{
				char tmp[256];
				::SetTextAlign( memDC, TA_TOP | TA_RIGHT );
				sprintf(tmp,"%d%%", (int)( ( g_fFileDownloadRatio + 0.005f ) * 100 ) );
				::TextOutA( memDC, g_fFileDownloadRatio * ( LOAD_NEW_CUR_FILE_BAR_WIDTH ) + LOAD_NEW_CUR_FILE_BAR_X , LOAD_NEW_CUR_FILE_BAR_Y , tmp, (int)::strlen( tmp ) );

				}

				if( g_fTotalDownloadRatio > 0.05f )
				{
				char tmp[256];
				::SetTextAlign( memDC, TA_TOP | TA_RIGHT );
				sprintf(tmp,"%d%%", (int)( ( g_fTotalDownloadRatio + 0.005f ) * 100 ) );
				::TextOutA( memDC, g_fTotalDownloadRatio * ( LOAD_NEW_ALL_FILE_BAR_WIDTH ) + LOAD_NEW_ALL_FILE_BAR_X , LOAD_NEW_ALL_FILE_BAR_Y , tmp, (int)::strlen( tmp ) );
				}*/

				::SelectObject(memDC,oldfont);
				::DeleteObject(font);
			}
			else
			{
				HFONT font, oldfont;
				font= ::CreateFont(PATCHER_FONT_SIZE,0,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,0,0,0,0,PATCHER_FONT);
				oldfont=(HFONT)SelectObject(memDC,font);
				::SetTextAlign( memDC, TA_TOP | TA_LEFT );
				::SetTextColor( memDC, 0xF0F0F0F0 );

				char tmp[256];
				if ( g_fCheckFileRatio == 0.f && g_bCheckFileRatioFull == true)
				{
					g_fCheckFileRatio = 1.f;
				}

				g_bCheckFileRatioFull = true;

				sprintf(tmp,"%d%%", (int)( ( g_fCheckFileRatio + 0.005f ) * 100 ) );
				::TextOutA( memDC, 20 +120 +20 ,451 , tmp, (int)::strlen( tmp ) );


				::SelectObject(memDC,oldfont);
				::DeleteObject(font);
			}
			

			

			BitBlt(ps.hdc,0,0,rcSize.right,rcSize.bottom,memDC,0,0,SRCCOPY);
			SelectObject(memDC,hOldBitmap);
			DeleteDC(memDC);
			EndPaint( hWnd, &ps );


/*#ifdef DMLEE_PATCHER_TEST

			if( g_bPatchOptionButtonDown == false )
			{
				if ( g_bPatchOptionButtonOver == true )
				{
					::StretchBlt( ps.hdc, PATCH_OPTION_BUTTON_X, PATCH_OPTION_BUTTON_Y, PATCH_OPTION_BUTTON_WIDTH, PATCH_OPTION_BUTTON_HEIGHT, 
						memDC,
						432, 465, 63, 38, 
						//146, 418, 284 - 146, 456 - 418,
						SRCCOPY );
				}
				else
				{
					::StretchBlt( ps.hdc, PATCH_OPTION_BUTTON_X, PATCH_OPTION_BUTTON_Y, PATCH_OPTION_BUTTON_WIDTH, PATCH_OPTION_BUTTON_HEIGHT,
						memDC,
						432, 465, 63, 38, 
						//5, 418, 143 - 5, 456 - 418,
						SRCCOPY );
				}
			}
			else
			{
				::StretchBlt( ps.hdc, PATCH_OPTION_BUTTON_X, PATCH_OPTION_BUTTON_Y, PATCH_OPTION_BUTTON_WIDTH, PATCH_OPTION_BUTTON_HEIGHT,
					memDC,
					432, 465, 63, 38, 
					//287, 418, 425 - 287, 456 - 418,
					SRCCOPY );
			}



			if( g_bPatchLaunchButtonDown == false )
			{
				if( true == g_bPatchLaunchButtonEnabled && 
					g_bPatchLaunchButtonOver == true )
				{
					::StretchBlt( ps.hdc, PATCH_LAUNCH_BUTTON_X, PATCH_LAUNCH_BUTTON_Y, PATCH_LAUNCH_BUTTON_WIDTH, PATCH_LAUNCH_BUTTON_HEIGHT, 
						memDC,
						432, 418, 63, 38, 
						//146, 418, 284 - 146, 456 - 418,
						SRCCOPY );
				}
				else
				{
					::StretchBlt( ps.hdc, PATCH_LAUNCH_BUTTON_X, PATCH_LAUNCH_BUTTON_Y, PATCH_LAUNCH_BUTTON_WIDTH, PATCH_LAUNCH_BUTTON_HEIGHT,
						memDC,
						432, 418, 63, 38, 
						//5, 418, 143 - 5, 456 - 418,
						SRCCOPY );
				}
			}
			else
			{
				::StretchBlt( ps.hdc, PATCH_LAUNCH_BUTTON_X, PATCH_LAUNCH_BUTTON_Y, PATCH_LAUNCH_BUTTON_WIDTH, PATCH_LAUNCH_BUTTON_HEIGHT,
					memDC,
					432, 418, 63, 38, 
					//287, 418, 425 - 287, 456 - 418,
					SRCCOPY );
			}

#endif */
			
            return 0;
        } break;
		
    }

    return ::DefWindowProc( hWnd, uMsg, wParam, lParam );

    #undef SAFE_DELETE_OBJECT
    #undef SAFE_CLOSE_HANDLE
}



/// 시스템에 설치된 DirectX의 버전이 9.0c이상인지 확인한다.
/// @return DirectX의 버전이 9.0c이상이면 true, 아니면 false
// DirectX End-User Runtimes 가 2006 8월 버전이상인가?
bool IsDXVerCheck( void )
{
#   define      NUM_VERSION_FIELD   (4)
    KRegistry   kReg;
    std::string strVer;

	//DirectX의 버전이 9.0c이상인가?
    // 레지스트리를 참조하자.
    if ( false == kReg.OpenKey( HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\DirectX\\" ) ||
         false == kReg.GetValue( "Version", strVer ) )
    {
        return false; // 레지스트리 정보가 없는 것으로 보아 9.0c 이하의 버전이다.
    }

    // 9.0c의 버전 숫자는 4.09.00.0904이다.
    int aiBound[NUM_VERSION_FIELD]  = { 4, 9, 0, 904 };
    int aiVer[NUM_VERSION_FIELD]    = { 0, };

    // '.'로 나눠진 필드를 각각 분리한다.
    for ( int i = 0; i < NUM_VERSION_FIELD; ++i )
    {
        std::string::size_type dotPos = strVer.find( "." );
        if ( std::string::npos == dotPos )
        {
            aiVer[i] = std::atoi( strVer.c_str() );
        }
        else
        {
            aiVer[i] = std::atoi( strVer.substr( 0, dotPos + 1 ).c_str() );
            strVer.erase( 0, dotPos + 1 );
        }
    }

    // 각 필드의 숫자를 비교한다.
    for ( int i = 0; i < NUM_VERSION_FIELD; ++i )
    {
        if ( aiVer[i] < aiBound[i] )
        {
            return false; // 9.0c 이하의 버전이다.
        }
    }

    return true; // ok

#   undef NUM_VERSION_FIELD
}

bool IsDotNetVerCheck( void )
{
	KRegistry   kReg;
	std::string strVer;

	// 레지스트리를 참조하자.
	if ( false == kReg.OpenKey( HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v3.0" ) )
	{
		return false; // 레지스트리 정보가 없는 것으로 보아 이하의 버전이다.
	}

	return true; // ok
}

/// 로그파일에 로그 메세지를 남긴다.
/// @param szLogMsg 로그 메세지
/// @return void
void Log( const char* szLogMsg )
{
//#if defined(PATCHER_TEST)
//        g_logStream << szLogMsg << std::endl;
//#endif
}

/// 로그파일에 로그 에러 메세지를 남긴다.
/// @param szLogMsg 로그 메세지
/// @return void
void LogError( const char* szLogMsg )
{
//#if defined(PATCHER_TEST)
//        DWORD   dwErr = ::GetLastError();
//        HRESULT hr    = HRESULT_FROM_WIN32( dwErr );
//        g_logStream << szLogMsg << std::endl
//                    << "    (WIN32 System Error Code : "
//                    << dwErr << ")" << std::endl
//                    << "    (HRESULT Code : "
//                    << hr << ")" << std::endl;
//#endif
}





#ifdef DMLEE_PATCHER_TEST

//////////////////////////////////////////////////////////////////////////
// option dialog
BOOL CALLBACK OptionDialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_INITDIALOG:
		{
			// 설정 가능한 해상도 옵션을 찾아서 combo box에
			HWND hComboResolution = GetDlgItem( hWnd, IDC_COMBO_RESOLUTION );

			DWORD iModeNum = 0;
			DEVMODE devMode;
			WCHAR wszText[512] = L"";

			std::map< wstring, bool > mapScrRes;
			while( TRUE == EnumDisplaySettings(NULL, iModeNum++, &devMode) )
			{
				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d x %d", (int) devMode.dmPelsWidth, (int) devMode.dmPelsHeight );
				mapScrRes[ wszText ] = true;
			}
			
			std::map<wstring, bool>::iterator it;
			for( it = mapScrRes.begin(); it != mapScrRes.end(); it++ )
			{
				SendMessage( hComboResolution, CB_ADDSTRING, 0, (LPARAM) (WCHAR*) it->first.c_str() );
			}
			SendMessage( hComboResolution, CB_SELECTSTRING, 0, (LPARAM) L"1024 x 768" );


			// gameoptions.lua 파일이 있으면 열어서 현재 옵션 정보를 가져온다
			if( NULL != g_pPatcher )
			{
				std::string strPath = g_pPatcher->GetProgramPath();
				if( 0 != strPath.length() )
				{
					if( '\\' != strPath[strPath.size() - 1] )
					{
						strPath.append( "\\" );
					}
					strPath.append( "GameOptions.lua" );
				}

				LoadGameOptionFile( strPath.c_str(), g_GameOptionList );
				
				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d x %d", g_GameOptionList.m_iResolutionX, g_GameOptionList.m_iResolutionY );
				SendMessage( hComboResolution, CB_SELECTSTRING, 0, (LPARAM) wszText );
			}

			return TRUE;
		} break;

	case WM_COMMAND:
		{
			switch( LOWORD(wParam) )
			{
			case IDOK:
				{				
					// 게임 옵션 파일 저장

					// gameoptions.lua 파일이 있으면 열어서 현재 옵션 정보를 가져온다
					if( NULL != g_pPatcher )
					{
						std::string strPath = g_pPatcher->GetProgramPath();
						if( 0 != strPath.length() )
						{
							if( '\\' != strPath[strPath.size() - 1] )
							{
								strPath.append( "\\" );
							}

							strPath.append( "GameOptions.lua" );
						}

						SaveGameOptionFile( strPath.c_str(), g_GameOptionList );
					}

					EndDialog( hWnd, 0 );
					return TRUE;
				} break;

			case IDCANCEL:
				{
					EndDialog( hWnd, 0 );
					return TRUE;

				} break;

			case IDC_COMBO_RESOLUTION:
				{
					switch( HIWORD(wParam) )
					{
					case CBN_SELCHANGE:
						{
							//int iSelectedResolutionIndex = 0;
							WCHAR wszText[512] = L"";

							HWND hComboResolution = GetDlgItem( hWnd, IDC_COMBO_RESOLUTION );

							int iSelectedResolutionIndex = SendMessage( hComboResolution, CB_GETCURSEL, 0, 0 );
							SendMessage( hComboResolution, CB_GETLBTEXT, iSelectedResolutionIndex, (LPARAM) wszText );


							wstring wstrText = wszText;
							int iTempIndex = wstrText.find( L"x" );

							wstring wstrWidth = wstrText.substr( 0, iTempIndex-1 );
							wstring wstrHeight = wstrText.substr( iTempIndex+1, 100 );

							g_GameOptionList.m_iResolutionX = wcstol( wstrWidth.c_str(), NULL, 10 );
							g_GameOptionList.m_iResolutionY = wcstol( wstrHeight.c_str(), NULL, 10 );

						} break;
					}
					
				} break;
			}

			return FALSE;
		} break;
	}

	return FALSE;
}




bool SaveGameOptionFile( const char* pOptionFilePath, CX2GameOption::OptionList& optionList )
{
	if( NULL == pOptionFilePath )
		return false;

	const string strFileName = pOptionFilePath;

	FILE* file = NULL;
	file = fopen( strFileName.c_str(), "w" );		
	if( NULL == file )
	{
		return false;
	}


	fwprintf( file, L"-- updated by patcher" );
	fwprintf( file, L"  \n" );
	fwprintf( file, L"  \n" );
	fwprintf( file, L"GAME_OPTION_CURRENT = \n" );
	fwprintf( file, L"{ \n" );


	fwprintf( file, L"\t-- GRAPHIC OPTION \n" );
	//fwprintf( file, L"\tAUTOSET_LEVEL		= %d, \n", optionList.m_iAutoSetLevel );

	switch( optionList.m_UnitDetail )
	{
	case CX2GameOption::OL_HIGH:
		fwprintf( file, L"\tGRAPHIC_UNIT		= OPTION_LEVEL[\"OL_HIGH\"], \n" );
		break;
	case CX2GameOption::OL_MEDIUM:
		fwprintf( file, L"\tGRAPHIC_UNIT		= OPTION_LEVEL[\"OL_MEDIUM\"], \n" );
		break;
	case CX2GameOption::OL_LOW:
		fwprintf( file, L"\tGRAPHIC_UNIT		= OPTION_LEVEL[\"OL_LOW\"], \n" );
		break;
	}

	switch( optionList.m_TexDetail )
	{
	case CX2GameOption::OL_HIGH:
		fwprintf( file, L"\tGRAPHIC_TEXTURE		= OPTION_LEVEL[\"OL_HIGH\"], \n" );
		break;
	case CX2GameOption::OL_MEDIUM:
		fwprintf( file, L"\tGRAPHIC_TEXTURE		= OPTION_LEVEL[\"OL_MEDIUM\"], \n" );
		break;
	case CX2GameOption::OL_LOW:
		fwprintf( file, L"\tGRAPHIC_TEXTURE		= OPTION_LEVEL[\"OL_LOW\"], \n" );
		break;
	}

	switch( optionList.m_MapDetail )
	{
	case CX2GameOption::OL_HIGH:
		fwprintf( file, L"\tGRAPHIC_MAP			= OPTION_LEVEL[\"OL_HIGH\"], \n" );
		break;
	case CX2GameOption::OL_MEDIUM:
		fwprintf( file, L"\tGRAPHIC_MAP			= OPTION_LEVEL[\"OL_MEDIUM\"], \n" );
		break;
	case CX2GameOption::OL_LOW:
		fwprintf( file, L"\tGRAPHIC_MAP			= OPTION_LEVEL[\"OL_LOW\"], \n" );
		break;
	}

	if( true == optionList.m_bEffect )
		fwprintf( file, L"\tGRAPHIC_EFFECT		= TRUE, \n" );
	else
		fwprintf( file, L"\tGRAPHIC_EFFECT		= FALSE, \n" );

	fwprintf( file, L"\tRESOLUTION_X		= %d, \n", optionList.m_iResolutionX );
	fwprintf( file, L"\tRESOLUTION_Y		= %d, \n", optionList.m_iResolutionY );


	fwprintf( file, L"\n\n\n" );
	fwprintf( file, L"\t-- SOUND OPTION \n" );


	if( true == optionList.m_bSound )
		fwprintf( file, L"\tSOUND			= TRUE, \n" );
	else
		fwprintf( file, L"\tSOUND			= FALSE, \n" );

	if( true == optionList.m_bMusic )
		fwprintf( file, L"\tMUSIC			= TRUE, \n" );
	else
		fwprintf( file, L"\tMUSIC			= FALSE, \n" );


	fwprintf( file, L"\tSOUND_VOLUME		= %d, \n", (int)optionList.m_fSoundVolume );
	fwprintf( file, L"\tMUSIC_VOLUME		= %d, \n", (int)optionList.m_fMusicVolume );

	fwprintf( file, L"\n\n\n" );
	fwprintf( file, L"\t-- OTHER OPTION \n" );


	if( true == optionList.m_bDynamicCamera )
		fwprintf( file, L"\tDYNAMIC_CAMERA		= TRUE, \n" );
	else
		fwprintf( file, L"\tDYNAMIC_CAMERA		= FALSE, \n" );


	fwprintf( file, L"} \n" );
	fclose(file);

	return true;
}


bool LoadGameOptionFile( const char* pFilePath, CX2GameOption::OptionList& optionList )
{
	if( NULL == pFilePath )
		return false;


	enum TOKEN_STATE
	{
		TS_NONE,
		TS_GRAPHIC_UNIT,
		TS_GRAPHIC_TEXTURE,
		TS_GRAPHIC_MAP,
		TS_GRAPHIC_EFFECT,
		TS_RESOLUTION_X,
		TS_RESOLUTION_Y,
		TS_SOUND,
		TS_MUSIC,
		TS_SOUND_VOLUME,
		TS_MUSIC_VOLUME,
		TS_DYNAMIC_CAMERA,
		TS_DUNGEON_ZOOM_CAMERA,
	};
	
	ifstream ifs;
	ifs.open( pFilePath, ios::in );
	if( false == ifs.is_open() )
		return false;

	int iStartIndex = 0;
	CHAR szData[1024];
	string strData;


	TOKEN_STATE eNowTokenState = TS_NONE;
	CX2GameOption::OptionLevel eOptionLevel;
	while( !ifs.eof() )
	{
		ifs >> szData;
		strData = szData;

		if( strData.compare( "=" ) == 0 )
			continue;

		switch( eNowTokenState )
		{
		default:
		case TS_NONE:
			{
				if( 0 == strData.compare( "GRAPHIC_UNIT" ) )
				{
					eNowTokenState = TS_GRAPHIC_UNIT;
				}
				else if( 0 == strData.compare( "GRAPHIC_TEXTURE" ) )
				{
					eNowTokenState = TS_GRAPHIC_TEXTURE;
				}
				else if( 0 == strData.compare( "GRAPHIC_MAP" ) )
				{
					eNowTokenState = TS_GRAPHIC_MAP;
				}	
				else if( 0 == strData.compare( "GRAPHIC_EFFECT" ) )
				{
					eNowTokenState = TS_GRAPHIC_EFFECT;
				}	
				else if( 0 == strData.compare( "RESOLUTION_X" ) )
				{
					eNowTokenState = TS_RESOLUTION_X;
				}	
				else if( 0 == strData.compare( "RESOLUTION_Y" ) )
				{
					eNowTokenState = TS_RESOLUTION_Y;
				}	
				else if( 0 == strData.compare( "SOUND" ) )
				{
					eNowTokenState = TS_SOUND;
				}	
				else if( 0 == strData.compare( "MUSIC" ) )
				{
					eNowTokenState = TS_MUSIC;
				}	
				else if( 0 == strData.compare( "SOUND_VOLUME" ) )
				{
					eNowTokenState = TS_SOUND_VOLUME;
				}	
				else if( 0 == strData.compare( "MUSIC_VOLUME" ) )
				{
					eNowTokenState = TS_MUSIC_VOLUME;
				}	
				else if( 0 == strData.compare( "DYNAMIC_CAMERA" ) )
				{
					eNowTokenState = TS_DYNAMIC_CAMERA;
				}	
				else if( 0 == strData.compare( "DUNGEON_ZOOM_CAMERA" ) )
				{
					eNowTokenState = TS_DUNGEON_ZOOM_CAMERA;
				}	



			} break;

		case TS_GRAPHIC_UNIT:
		case TS_GRAPHIC_TEXTURE:
		case TS_GRAPHIC_MAP:
			{
				eOptionLevel = CX2GameOption::OL_LOW;

				iStartIndex = strData.find( "OL_HIGH" );
				if( iStartIndex > 0 )
				{
					eOptionLevel = CX2GameOption::OL_HIGH;
				}
				else
				{
					iStartIndex = strData.find( "OL_MEDIUM" );
					if( iStartIndex > 0 )
					{
						eOptionLevel = CX2GameOption::OL_MEDIUM;
					}
				}

				switch( eNowTokenState )
				{
				case TS_GRAPHIC_UNIT:
					{
						optionList.m_UnitDetail = eOptionLevel;
					} break;
				case TS_GRAPHIC_TEXTURE:
					{
						optionList.m_TexDetail = eOptionLevel;
					} break;
				case TS_GRAPHIC_MAP:
					{
						optionList.m_MapDetail = eOptionLevel;
					} break;
				}

				eNowTokenState = TS_NONE;
			} break;

		case TS_GRAPHIC_EFFECT:
		case TS_SOUND:
		case TS_MUSIC:
		case TS_DYNAMIC_CAMERA:
		case TS_DUNGEON_ZOOM_CAMERA:
			{
				bool bOn = false;
				iStartIndex = strData.find( "TRUE" );
				if( iStartIndex > 0 )
				{
					bOn = true;
				}

				switch( eNowTokenState )
				{
				case TS_GRAPHIC_EFFECT:
					{
						optionList.m_bEffect = bOn;
					} break;
				case TS_SOUND:
					{
						optionList.m_bSound = bOn;
					} break;

				case TS_MUSIC:
					{
						optionList.m_bMusic = bOn;
					} break;

				case TS_DYNAMIC_CAMERA:
					{
						optionList.m_bDynamicCamera = bOn;
					} break;
				}

				eNowTokenState = TS_NONE;
			} break;

		case TS_RESOLUTION_X:
			{
				optionList.m_iResolutionX = atoi( strData.c_str() );
				eNowTokenState = TS_NONE;
			} break;

		case TS_RESOLUTION_Y:
			{
				optionList.m_iResolutionY = atoi( strData.c_str() );
				eNowTokenState = TS_NONE;
			} break;

		case TS_SOUND_VOLUME:
			{
				optionList.m_fSoundVolume = (float) atof( strData.c_str() );
				eNowTokenState = TS_NONE;
			} break;

		case TS_MUSIC_VOLUME:
			{
				optionList.m_fMusicVolume = (float) atof( strData.c_str() );
				eNowTokenState = TS_NONE;
			} break;
		}
	}

	return true;
}


#endif // DMLEE_PATCHER_TEST