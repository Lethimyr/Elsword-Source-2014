// file   : KBrowserWrapper.cpp
// since  : 2003-11-10 오후 12:24:29
// author : pch413@kogstudios.com
// This requires IE 5.0 ( or better ).
#include "stdafx.h"
#include "KBrowserWrapper.h"
#include "resource.h"

// in Browser.c file ( from http://www.codeproject.com/ )
extern "C"
{
    long DisplayHTMLPage( HWND hWnd, LPCTSTR webPageName, LPCTSTR strPostString_ );
    void ResizeBrowser( HWND hWnd, DWORD width, DWORD height );
    long EmbedBrowserObject( HWND hWnd, HWND hRootWnd );
    void UnEmbedBrowserObject( HWND hWnd );
    void DoPageAction(HWND hWnd, DWORD action);
}

#define BROWSER_ID 1000

static const TCHAR szWrapperClassName[] = _T("WrapperWindow");
static const TCHAR szBrowserClassName[] = _T("BrowserObject");

HWND g_hRootWnd;			// browser window가 생성될 때 임시로 사용되는 변수이다. 

LRESULT CALLBACK KBrowserWrapper::WndProc( HWND hWnd,
                                           UINT uMsg,
                                           WPARAM wParam,
                                           LPARAM lParam )
{
    switch ( uMsg )
    {
		case WM_CREATE:
		{
			g_hRootWnd = ::GetParent( hWnd );
			break;
		}
        case WM_SIZE:
        {
            // Resize the browser child window, leaving room for our buttons.
            HWND hBrowserWnd = ::GetDlgItem( hWnd, BROWSER_ID );
            if ( NULL != hBrowserWnd )
            {
                ::MoveWindow( hBrowserWnd, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE );
            }

            return 0;
        }

        case WM_ERASEBKGND:
        {
            return 0;
        }

        case WM_CLOSE:
        {
            // Close this window. This will also cause the child window hosting the browser
            // control to receive its WM_DESTROY
            ::DestroyWindow( hWnd );

            return 0;
        }

		case WM_KEYDOWN:
			{
				int i=0;
				break;
			}
    }

    return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK KBrowserWrapper::BrowserWndProc( HWND hWnd,
                                                  UINT uMsg,
                                                  WPARAM wParam,
                                                  LPARAM lParam )
{
    switch ( uMsg )
    {
        case WM_SIZE:
        {
            // Resize the browser object to fit the window
            ResizeBrowser( hWnd, LOWORD(lParam), HIWORD(lParam) );

            return 0;
        }

        case WM_CREATE:
        {
            // Embed the browser object into our host window. We need do this only
            // once. Note that the browser object will start calling some of our
            // IOleInPlaceFrame and IOleClientSite functions as soon as we start
            // calling browser object functions in EmbedBrowserObject().
            if ( EmbedBrowserObject( hWnd, g_hRootWnd ) ) return -1;

            return 0; // Success
        }

        case WM_DESTROY:
        {
            // Detach the browser object from this window, and free resources.
            UnEmbedBrowserObject( hWnd );

            return 0;
        }
		case WM_KEYDOWN:
			{
				int i=0;
				break;
			}
    }

    return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
}

KBrowserWrapper::KBrowserWrapper( HINSTANCE hInstance /* = NULL */ )
{
    m_hInstance = ( NULL == hInstance ? ::GetModuleHandle( NULL ) : hInstance );
	m_bOleInit = false;

	m_hDlg        = NULL;
	m_hWrapperWnd = NULL;
	m_hBrowserWnd = NULL;


    _Init();



}

KBrowserWrapper::~KBrowserWrapper( void )
{
    _Uninit();
}

void KBrowserWrapper::_Init( void )
{
	if( S_OK == ::OleInitialize( NULL ) )
	{
		m_bOleInit = true;
	}
    //m_bOleInit = ( S_OK == ::OleInitialize( NULL ) );

    WNDCLASSEX wc;

    ::ZeroMemory( &wc, sizeof(WNDCLASSEX) );

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.hInstance     = m_hInstance;
    wc.lpfnWndProc   = KBrowserWrapper::WndProc;
    wc.lpszClassName = szWrapperClassName;
    wc.style         = CS_CLASSDC | CS_HREDRAW | CS_VREDRAW
                     | CS_PARENTDC | CS_BYTEALIGNCLIENT | CS_DBLCLKS;
    ::RegisterClassEx( &wc );

    wc.lpfnWndProc   = KBrowserWrapper::BrowserWndProc;
    wc.lpszClassName = szBrowserClassName;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    ::RegisterClassEx( &wc );
}

void KBrowserWrapper::_Uninit( void )
{
    ::UnregisterClass( szWrapperClassName, m_hInstance );
    ::UnregisterClass( szBrowserClassName, m_hInstance );

    if ( true == m_bOleInit ) ::OleUninitialize();
}

bool KBrowserWrapper::Create( LPTSTR szWebPage /* = _T("http://www.kogstudios.com/") */,
                              int x            /* = CW_USEDEFAULT */,
                              int y            /* = CW_USEDEFAULT */,
                              int w            /* = CW_USEDEFAULT */,
                              int h            /* = CW_USEDEFAULT */,
                              HWND hParentWnd  /* = HWND_DESKTOP */,
                              DWORD type       /* = BROWSER_TYPE_POPUP */ )
{
    if ( false == m_bOleInit ) return false;

    DWORD dwStyle = ( ( BROWSER_TYPE_CHILD == type ) ? WS_CHILD : WS_OVERLAPPEDWINDOW );

    if ( NULL == ( m_hWrapperWnd = ::CreateWindowEx( 0, szWrapperClassName,
                                                     _T(""),
                                                     dwStyle, 0, 0, 0, 0,
                                                     hParentWnd, NULL,
                                                     m_hInstance, NULL ) ) )
    {
        return false;
    }

    if ( NULL == ( m_hBrowserWnd = ::CreateWindowEx( 0, szBrowserClassName,
                                                     _T(""),
                                                     WS_CHILD | WS_VISIBLE,
                                                     0, 0, 0, 0,
                                                     m_hWrapperWnd,
                                                     reinterpret_cast<HMENU>(BROWSER_ID),
                                                     m_hInstance, NULL ) ) )
    {
        return false;
    }

    DisplayHTMLPage( m_hBrowserWnd, szWebPage, NULL );
    ::MoveWindow( m_hWrapperWnd, x, y, w, h, TRUE );
    ::ShowWindow( m_hWrapperWnd, SW_SHOW );

	

    return true;
}

void KBrowserWrapper::Destroy( void )
{
    ::DestroyWindow( m_hWrapperWnd );

    m_hWrapperWnd = NULL;
    m_hBrowserWnd = NULL;
}

void KBrowserWrapper::Repaint( void )
{
    ::UpdateWindow( m_hWrapperWnd );
    ::UpdateWindow( m_hBrowserWnd );

    DoAction( ACTION_REFRESH );
}

void KBrowserWrapper::Navigate( LPTSTR szWebPage /* = _T("http://www.kogstudios.com/") */ )
{
    if ( NULL != m_hBrowserWnd )
    {
        DisplayHTMLPage( m_hBrowserWnd, szWebPage, NULL );
    }
}

void KBrowserWrapper::DoAction( DWORD dwAction )
{
    DoPageAction( m_hBrowserWnd, dwAction );
}