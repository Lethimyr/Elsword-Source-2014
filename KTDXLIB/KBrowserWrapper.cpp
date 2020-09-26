

//{{ dmlee 2008.03.26
// 넥슨 캐시결제 페이지 창 띄우기 작업함. 
// 전체화면에서 win32 window 뜨게 하기 위해서 g_pKTDXApp->GetDevice()->SetDialogBoxMode( TRUE ), 창이 닫히면 (FALSE) 해줘야 (performance 문제 때문에)
// f_pWrapper 라는 변수가 static global 이라서 KBrowserWrapper는 하나만 만들어야 됨. 즉, singleton
// KTDXLIB에 KBrowserWrapper가 있으나 KBrowserWrapper가 쓰는 모든 dialog, bitmap등 resource는 x2/resource.h에 있음.
// KBrowserWrapper의 객체는 CX2Main의 멤버로
//}} dmlee 2008.03.26


#include "stdafx.h"
#include "KBrowserWrapper.h"


static KBrowserWrapper* f_pWrapper = NULL;


BOOL CALLBACK DlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
	{
	case WM_INITDIALOG:
		{
			// 브라우저 열기
			f_pWrapper->OpenBrowser( hDlg );

			//// 마우스 클립
			//RECT rc;
			//::GetWindowRect( hDlg, &rc );
			//::ClipCursor( &rc );

			return TRUE;
		} break;

	case WM_COMMAND:
		{
			// 브라우저 닫기 및 마우스 클립 해제
			//switch ( LOWORD( wParam ) )
			//{
			//case IDOK:
			//	{
			//		if( false == f_pWrapper->IsClosed() )
			//		{
			//			f_pWrapper->CloseBrowser();
			//		}
			//		::EndDialog( hDlg, IDOK );
			//		::ClipCursor( NULL );

			//		//{{ dmlee 2008.03.26
			//		g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			//		//}} dmlee 2008.03.26

			//		return TRUE;
			//	}
			//case IDCANCEL:
			//	{
			//		if( false == f_pWrapper->IsClosed() )
			//		{
			//			f_pWrapper->CloseBrowser();
			//		}
			//		::EndDialog( hDlg, IDCANCEL );
			//		::ClipCursor( NULL );

			//		//{{ dmlee 2008.03.26
			//		g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			//		//}} dmlee 2008.03.26

			//		return TRUE;
			//	}
			//}
			return FALSE;
		} break;

	case WM_CLOSE:
		{
			if( false == f_pWrapper->IsClosed() )
			{
				f_pWrapper->CloseBrowser();
			}
			::EndDialog( hDlg, IDCANCEL );
			::ClipCursor( NULL );

			//{{ dmlee 2008.03.26
			g_pKTDXApp->GetDevice()->SetDialogBoxMode( FALSE );
			//}} dmlee 2008.03.26

			return TRUE;
		} break;
	}

    return FALSE;
}

KBrowserWrapper::KBrowserWrapper( void )
{
    f_pWrapper  = this;

    ::ZeroMemory( &m_rect, sizeof( RECT ) );
    m_bClosed   = true;
	m_bCheckClosed = true;
    m_hDlg      = NULL;
	m_pPCHBrowser = NULL;
}

KBrowserWrapper::~KBrowserWrapper( void )
{
    DeleteBrowserObj();
    f_pWrapper = NULL;
}

#ifdef SEND_NEXON_WISE_LOG_BY_URL // 김태환
BOOL KBrowserWrapper::Create( RECT& rect, HWND hWndParent, DWORD dwStyle /*= SW_SHOW*/ )
#else // SEND_NEXON_WISE_LOG_BY_URL
BOOL KBrowserWrapper::Create( RECT& rect, HWND hWndParent )
#endif //  SEND_NEXON_WISE_LOG_BY_URL
{
    m_rect      = rect;
    m_bClosed   = false;
	m_bCheckClosed = true;

	//{{ dmlee 2008.03.24
	const int MAGIC_IDD_BROWSER_DIALOG = 115;					// note!! x2/resource.h에 있는 IDD_BROWSER_DIALOG
	HINSTANCE hInst = ::GetModuleHandle( NULL );
	LPTSTR szRes = MAKEINTRESOURCE( MAGIC_IDD_BROWSER_DIALOG );
	m_hDlg = ::CreateDialog( hInst, szRes, hWndParent, DlgProc );
	if( NULL == m_hDlg )
	{
		return FALSE;
	}

#ifdef SEND_NEXON_WISE_LOG_BY_URL // 김태환
	::ShowWindow( m_hDlg, dwStyle );	/// 표시 설정
#endif //  SEND_NEXON_WISE_LOG_BY_URL

	//}} dmlee 2008.03.24

    return TRUE;
}

bool KBrowserWrapper::OpenBrowser( HWND hDlg )
{
    if ( false == f_pWrapper->AllocateBrowserObj() )
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

	//리소스 수정 필요
	/*
    if ( FALSE == m_pPCHBrowser->Create( WS_VISIBLE|WS_CHILD, rect, hDlg, 10000 ) )
    {
        ::MessageBox( hDlg,
                        g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TEXT ).c_str(),
                        g_pkStrLoader->GetString( STR_ID_APPMSG_ERR_CASH_FAIL_TITLE ).c_str(),
                        MB_OK );
    }

    ::MoveWindow( hDlg, m_rect.left, m_rect.top, w, h, TRUE );
    ::ShowWindow( hDlg, SW_SHOW );

    // 닫기 버튼 위치 교정 및 이름 설정
    int btnx = ( w - 100 ) / 2;
    int btny = h - 25;
    ::MoveWindow( ::GetDlgItem( hDlg, IDOK ), btnx, btny, 80, 20, TRUE );
    ::SetWindowText( ::GetDlgItem( hDlg, IDOK ),
                     g_pkStrLoader->GetString( STR_ID_CASH_BUY_CLOSE_BUTTON ).c_str() );
					 */



	//{{ dmlee 2008.03.24
#ifdef CONVERSION_VS
	const int MAGIC_CHILD_WINDOW_ID = 5678; // note!! m_pPCHBrowser->m_hWrapperWnd의 ID
#else CONVERSION_VS
	const MAGIC_CHILD_WINDOW_ID = 5678; // note!! m_pPCHBrowser->m_hWrapperWnd의 ID
#endif CONVERSION_VS
	DWORD dwStyle = WS_VISIBLE | WS_CHILD;
	if ( FALSE == m_pPCHBrowser->Create( dwStyle, rect, hDlg, MAGIC_CHILD_WINDOW_ID ) )
	{
		::MessageBox( hDlg,
			L"open browser failed",
			L"error",
			MB_OK );
	}

	::MoveWindow( hDlg, m_rect.left, m_rect.top, w, h, TRUE );
	::ShowWindow( hDlg, SW_SHOW );

	// 닫기 버튼 위치 
	//int btnx = ( w - 100 ) / 2;
	//int btny = h - 40;
	//::MoveWindow( ::GetDlgItem( hDlg, IDOK ), btnx, btny, 80, 20, TRUE );
	//}} dmlee 2008.03.24


    return true;
}

void KBrowserWrapper::CloseBrowser( void )
{
    m_bClosed = true;
	m_bCheckClosed = false;
	if( NULL != m_pPCHBrowser )
		m_pPCHBrowser->Destroy();

    DeleteBrowserObj();
}