#include "stdafx.h"
#include "KNMBrowserWrapper.h"

static KNMBrowserWrapper* f_pNMWrapper = NULL;

void SWEBCallBack_OnQuit()
{
	if( false == f_pNMWrapper->IsClosed() )
	{
		f_pNMWrapper->CloseBrowser();
	}
	::EndDialog( f_pNMWrapper->GetWndHandle(), IDCANCEL );
	::ClipCursor( NULL );

	g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
}

BOOL CALLBACK NMDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
	{
	case WM_INITDIALOG:
		{
			g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE );

			// 브라우저 열기
			f_pNMWrapper->OpenBrowser( hDlg );

			return TRUE;
		} break;
	}

    return FALSE;
}

KNMBrowserWrapper::KNMBrowserWrapper( void )
{
    f_pNMWrapper  = this;

    ::ZeroMemory( &m_rect, sizeof( RECT ) );
    m_bClosed   = true;
	m_bCheckClosed = true;
    m_hDlg      = NULL;
	m_pSSOBrowser = NULL;
}

KNMBrowserWrapper::~KNMBrowserWrapper( void )
{
    m_pSSOBrowser = NULL;
    f_pNMWrapper = NULL;
}

BOOL KNMBrowserWrapper::Create( RECT& rect, HWND hWndParent )
{
    m_rect      = rect;
    m_bClosed   = false;
	m_bCheckClosed = true;

	const int MAGIC_IDD_BROWSER_DIALOG = 115;
	HINSTANCE hInst = ::GetModuleHandle( NULL );
	LPTSTR szRes = MAKEINTRESOURCE( MAGIC_IDD_BROWSER_DIALOG );
	m_hDlg = ::CreateDialog( hInst, szRes, hWndParent, NMDlgProc );
	if( NULL == m_hDlg )
	{
		return FALSE;
	}

    return TRUE;
}

bool KNMBrowserWrapper::OpenBrowser( HWND hDlg )
{
	if ( false == f_pNMWrapper->AllocateBrowserObj() )
	{
		return false;
	}

	int w = m_rect.right - m_rect.left;
	int h = m_rect.bottom - m_rect.top;

	RECT rect;
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = w - 5;
	rect.bottom = h;

	const int MAGIC_CHILD_WINDOW_ID = 5678;
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;

	if( FALSE == m_pSSOBrowser->Create( dwStyle, rect, hDlg, MAGIC_CHILD_WINDOW_ID ) )
	{
		::MessageBox( hDlg, L"Open WebBrowser Failed", L"Error", MB_OK );
		return false;
	}

	m_pSSOBrowser->SetCallback_OnQuit( SWEBCallBack_OnQuit );

	::MoveWindow( hDlg, m_rect.left, m_rect.top, w, h, TRUE );
	::ShowWindow( hDlg, SW_SHOW );

	return true;
}

void KNMBrowserWrapper::CloseBrowser( void )
{
    m_bClosed = true;
	m_bCheckClosed = false;

    m_pSSOBrowser = NULL;
}

