#include "KMsgBox.h"

HWND KMsgBox::ms_hDlg  = NULL;                       ///< Handle to message box.
UINT KMsgBox::ms_uType = KMsgBox::MBOX_NOTHING;      ///< Type of message box.
char KMsgBox::ms_szTxtLn1[LINE_LENGTH + 1] = { 0, }; ///< First line message.
char KMsgBox::ms_szTxtLn2[LINE_LENGTH + 1] = { 0, }; ///< Second line message.

/// Open message box.
/// @param szTxtLn1 first line message
/// @param szTxtLn2 second line message
/// @param hParentWnd handle to parent window
/// @param uType type of message box
/// @return the nResult parameter in the call to the EndDialog function used to terminate the dialog box, zero, 1, or -1
INT_PTR KMsgBox::Open(const char* szTxtLn1,
                      const char* szTxtLn2,
                      UINT uType /* = MBOX_DEFAULT */,
                      HWND hParentWnd /* = NULL */)
{
    if (NULL == hParentWnd) hParentWnd = ::GetDesktopWindow();
    ::ZeroMemory(ms_szTxtLn1, sizeof(ms_szTxtLn1));
    ::ZeroMemory(ms_szTxtLn2, sizeof(ms_szTxtLn2));
    size_t len1 = ::strlen(szTxtLn1);
    size_t len2 = ::strlen(szTxtLn2);
    ::memcpy(ms_szTxtLn1, szTxtLn1, (len1 < LINE_LENGTH ? len1 : LINE_LENGTH));
    ::memcpy(ms_szTxtLn2, szTxtLn2, (len2 < LINE_LENGTH ? len2 : LINE_LENGTH));
    LPTSTR szRes;
    switch (ms_uType = uType)
    {
        case MBOX_DEFAULT:  szRes = MAKEINTRESOURCE(IDD_MSGBOX_DEFAULT);  break;
        case MBOX_OK:       szRes = MAKEINTRESOURCE(IDD_MSGBOX_OK);       break;
        case MBOX_OKCANCEL: szRes = MAKEINTRESOURCE(IDD_MSGBOX_OKCANCEL); break;
        default: ASSERT(false && _T("Invalid message box type!"));        break;
    }
    if (MBOX_DEFAULT == ms_uType)
    {
        ::CreateDialog(::GetModuleHandle(NULL), szRes, hParentWnd, MsgBoxProcUTF8);
        return TRUE;
    }
    else
    {
        return ::DialogBox(::GetModuleHandle(NULL), szRes, hParentWnd, MsgBoxProcUTF8);
    }
}

/// Close message box.
/// @param iResult return value of message box.
/// @return return value of message box.
BOOL KMsgBox::Close(int iResult /* = IDOK */)
{
    if (NULL != ms_hDlg)
    {
        if (MBOX_DEFAULT == ms_uType)
        {
            ::SendMessage(ms_hDlg, WM_CLOSE, 0, 0);
            return TRUE;
        }
        else
        {
            return ::EndDialog(ms_hDlg, iResult);
        }
    }
    return FALSE;
}

/// Application-defined dialog box callback function.
/// @param hDlg handle to dialog box
/// @param uMsg message
/// @param wParam first message parameter
/// @param lParam second message parameter
/// @return TRUE if it processed the message, and FALSE if it did not
BOOL CALLBACK KMsgBox::MsgBoxProc(HWND hDlg,
                                  UINT uMsg,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    static HBITMAP bkgndBmp     = NULL;
    static HBITMAP okBtnBmp     = NULL;
    static HBITMAP cancelBtnBmp = NULL;

    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            ms_hDlg = hDlg;
            bkgndBmp = ::LoadBitmap(::GetModuleHandle(NULL),
                                    MAKEINTRESOURCE(IDB_MSGBOX));
            ASSERT(NULL != bkgndBmp);
            okBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),
                                    MAKEINTRESOURCE(IDB_BTN_OK));
            ASSERT(NULL != okBtnBmp);
            cancelBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),
                                        MAKEINTRESOURCE(IDB_BTN_CANCEL));
            ASSERT(NULL != cancelBtnBmp);
            switch (ms_uType)
            {
                case MBOX_OKCANCEL:
                    ::SendDlgItemMessage(hDlg, IDCANCEL, BM_SETIMAGE, IMAGE_BITMAP,
                                         reinterpret_cast<LPARAM>(cancelBtnBmp));
                case MBOX_OK:
                    ::SendDlgItemMessage(hDlg, IDOK, BM_SETIMAGE, IMAGE_BITMAP,
                                         reinterpret_cast<LPARAM>(okBtnBmp));
            }
            //::SetWindowText(hDlg, _T("¿¤¼Òµå"));
			::SetWindowText(hDlg, _T("Elsword"));
            ::SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 303, 145, SWP_NOMOVE);
            ::UpdateWindow(hDlg);
            return TRUE;
        }
        case WM_CLOSE:
        {
            ::DestroyWindow(hDlg);
            return TRUE;
        }
        case WM_DESTROY:
        {
            if (NULL != bkgndBmp)     { ::DeleteObject(bkgndBmp);     bkgndBmp     = NULL; }
            if (NULL != okBtnBmp)     { ::DeleteObject(okBtnBmp);     okBtnBmp     = NULL; }
            if (NULL != cancelBtnBmp) { ::DeleteObject(cancelBtnBmp); cancelBtnBmp = NULL; }
            ms_uType = MBOX_NOTHING;
            ms_hDlg  = NULL;
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK:     ::EndDialog(hDlg, IDOK);     return TRUE;
                case IDCANCEL: ::EndDialog(hDlg, IDCANCEL); return TRUE;
            }
            return FALSE;
        }
        case WM_MOUSEMOVE:
        {
            if (wParam & MK_LBUTTON)
            {
                ::SendMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
            }
            return TRUE;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            ::BeginPaint(hDlg, &ps);
            HDC memDC = ::CreateCompatibleDC(ps.hdc);

            // Draw background.
            HBITMAP oldBmp = reinterpret_cast<HBITMAP>(::SelectObject(memDC, bkgndBmp));
            ::BitBlt(ps.hdc, 0, 0, MBOX_WIDTH, MBOX_HEIGHT, memDC, 0, 0, SRCCOPY);
            ::SelectObject(memDC, oldBmp);

            // Print message.
            ::SetBkMode(ps.hdc, TRANSPARENT);
            ::SetTextAlign(ps.hdc, TA_CENTER);
            ::SetTextColor(ps.hdc, 0x00996644);
            int iRevise = (MBOX_DEFAULT == ms_uType ? 12 : 0);


			WCHAR wcharBuf[255] = {0,};
			MultiByteToWideChar( CP_ACP, 0, ms_szTxtLn1 , -1, wcharBuf, 255 );
			WCHAR wcharBuf2[255] = {0,};
			MultiByteToWideChar( CP_ACP, 0, ms_szTxtLn2 , -1, wcharBuf2, 255 );

            for (int x = 0; x < 5; x++)
            {
                for (int y = 0; y < 5; y++)
                {
                    ::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
                              y + FIRST_LINE - 2 + iRevise,
                              wcharBuf, (int)::wcslen(wcharBuf));
                    ::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
                              y + SECOND_LINE - 2 + iRevise,
                              wcharBuf2, (int)::wcslen(wcharBuf2));
                }
            }
            ::SetTextColor(ps.hdc, 0x00ffffff);
            ::TextOut(ps.hdc, MBOX_WIDTH / 2, FIRST_LINE + iRevise,
                      wcharBuf, (int)::wcslen(wcharBuf));
            ::TextOut(ps.hdc, MBOX_WIDTH / 2, SECOND_LINE + iRevise,
                      wcharBuf2, (int)::wcslen(wcharBuf2));

            ::DeleteDC(memDC);
            ::EndPaint(hDlg, &ps);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CALLBACK KMsgBox::MsgBoxProcUTF8(HWND hDlg,
								  UINT uMsg,
								  WPARAM wParam,
								  LPARAM lParam)
{
	static HBITMAP bkgndBmp     = NULL;
	static HBITMAP okBtnBmp     = NULL;
	static HBITMAP cancelBtnBmp = NULL;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			ms_hDlg = hDlg;
			bkgndBmp = ::LoadBitmap(::GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDB_MSGBOX));
			ASSERT(NULL != bkgndBmp);
			okBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDB_BTN_OK));
			ASSERT(NULL != okBtnBmp);
			cancelBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDB_BTN_CANCEL));
			ASSERT(NULL != cancelBtnBmp);
			switch (ms_uType)
			{
			case MBOX_OKCANCEL:
				::SendDlgItemMessage(hDlg, IDCANCEL, BM_SETIMAGE, IMAGE_BITMAP,
					reinterpret_cast<LPARAM>(cancelBtnBmp));
			case MBOX_OK:
				::SendDlgItemMessage(hDlg, IDOK, BM_SETIMAGE, IMAGE_BITMAP,
					reinterpret_cast<LPARAM>(okBtnBmp));
			}
			//::SetWindowText(hDlg, _T("¿¤¼Òµå"));
			::SetWindowText(hDlg, _T("Elsword"));
			::SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 303, 145, SWP_NOMOVE);
			::UpdateWindow(hDlg);
			return TRUE;
		}
	case WM_CLOSE:
		{
			::DestroyWindow(hDlg);
			return TRUE;
		}
	case WM_DESTROY:
		{
			if (NULL != bkgndBmp)     { ::DeleteObject(bkgndBmp);     bkgndBmp     = NULL; }
			if (NULL != okBtnBmp)     { ::DeleteObject(okBtnBmp);     okBtnBmp     = NULL; }
			if (NULL != cancelBtnBmp) { ::DeleteObject(cancelBtnBmp); cancelBtnBmp = NULL; }
			ms_uType = MBOX_NOTHING;
			ms_hDlg  = NULL;
			return TRUE;
		}
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDOK:     ::EndDialog(hDlg, IDOK);     return TRUE;
			case IDCANCEL: ::EndDialog(hDlg, IDCANCEL); return TRUE;
			}
			return FALSE;
		}
	case WM_MOUSEMOVE:
		{
			if (wParam & MK_LBUTTON)
			{
				::SendMessage(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
			}
			return TRUE;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			::BeginPaint(hDlg, &ps);
			HDC memDC = ::CreateCompatibleDC(ps.hdc);

			// Draw background.
			HBITMAP oldBmp = reinterpret_cast<HBITMAP>(::SelectObject(memDC, bkgndBmp));
			::BitBlt(ps.hdc, 0, 0, MBOX_WIDTH, MBOX_HEIGHT, memDC, 0, 0, SRCCOPY);
			::SelectObject(memDC, oldBmp);

			// Print message.
			::SetBkMode(ps.hdc, TRANSPARENT);
			::SetTextAlign(ps.hdc, TA_CENTER);
			::SetTextColor(ps.hdc, 0x00996644);
			int iRevise = (MBOX_DEFAULT == ms_uType ? 12 : 0);


			WCHAR wcharBuf[255] = {0,};
			//ConvertUtf8ToWideString(wcharBuf, LINE_LENGTH, ms_szTxtLn1 );
			ConvertUtf8ToWCHAR( wcharBuf, ms_szTxtLn1, LINE_LENGTH + 1 );
			//MultiByteToWideChar( CP_ACP, 0, ms_szTxtLn1 , -1, wcharBuf, 255 );
			WCHAR wcharBuf2[255] = {0,};
			//ConvertUtf8ToWideString(wcharBuf2, LINE_LENGTH, ms_szTxtLn2 );
			ConvertUtf8ToWCHAR( wcharBuf2, ms_szTxtLn2, LINE_LENGTH + 1 );
			//MultiByteToWideChar( CP_ACP, 0, ms_szTxtLn2 , -1, wcharBuf2, 255 );

			for (int x = 0; x < 5; x++)
			{
				for (int y = 0; y < 5; y++)
				{
					::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
						y + FIRST_LINE - 2 + iRevise,
						wcharBuf, (int)::wcslen(wcharBuf));
					::TextOut(ps.hdc, x + MBOX_WIDTH / 2 - 2,
						y + SECOND_LINE - 2 + iRevise,
						wcharBuf2, (int)::wcslen(wcharBuf2));
				}
			}
			::SetTextColor(ps.hdc, 0x00ffffff);
			::TextOut(ps.hdc, MBOX_WIDTH / 2, FIRST_LINE + iRevise,
				wcharBuf, (int)::wcslen(wcharBuf));
			::TextOut(ps.hdc, MBOX_WIDTH / 2, SECOND_LINE + iRevise,
				wcharBuf2, (int)::wcslen(wcharBuf2));

			::DeleteDC(memDC);
			::EndPaint(hDlg, &ps);
			return TRUE;
		}
	}

	return FALSE;
}

void KMsgBox::ConvertUtf8ToWCHAR( WCHAR* dest, const char* pSrc, const int iLength )
{
	::ZeroMemory(dest, sizeof(dest));
	if( !pSrc )
	{
		return;
	}

	int iSrcLength = iLength;
	if( -1 == iSrcLength )
	{
		iSrcLength = strlen( pSrc );
		if ( iSrcLength <= 0 )
		{
			dest = L"";
			return;
		}//if
	}

	int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, NULL, 0 );
	//int iDestSize = iSrcLength;
	if ( iDestSize <= 0 )
	{
		dest = L"";
		return;
	}//if


	WCHAR wcharBuf[255] = {0,};
	ASSERT( wcharBuf != NULL );
	int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, wcharBuf, iDestSize );
	ASSERT( iNewSize == iDestSize );
	wcscpy( dest, wcharBuf );
	//dest.assign( wcharBuf, iNewSize );

}
/*
inline void ConvertCharToWideString( std::wstring& _wstrDestination, const char* pszInStr_, UINT uInCodePage_ )
{
	if ( pszInStr_ == NULL )
	{
		_wstrDestination.clear();
		return;
	}//if

	size_t uSourceLen = strlen( pszInStr_ );
	if ( uSourceLen == 0 )
	{
		_wstrDestination = L"";
		return;
	}//if

	int size;
	if ( uInCodePage_ == CP_UTF8 )
		size = EstimateUtf8ToWideStringLength( uSourceLen );
	else
		size = ::MultiByteToWideChar( uInCodePage_, 0, pszInStr_, uSourceLen, NULL, 0 );
	if ( size <= 0 )
	{
		_wstrDestination = L"";
		return;
	}//if

	WCHAR* wszBuffer = (WCHAR*) _alloca( size * sizeof(WCHAR) );
	ASSERT( wszBuffer != NULL );
	int iNewSize = ::MultiByteToWideChar( uInCodePage_, 0, pszInStr_, uSourceLen, wszBuffer, size );
	_wstrDestination.assign( wszBuffer, iNewSize );

}//ConvertCharToWideString()
*/
