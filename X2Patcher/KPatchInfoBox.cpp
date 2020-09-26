/// @file   KPatchInfoBox.cpp
/// @since  2003-09-15 오후 2:06:50
/// @author pch413@kogstudios.com

#include "KPatchInfoBox.h"
#include <string>

#define PATCH_INFO_FILE_NAME    "PatchInfo.txt"
#define NO_PATCH_INFO_MESSAGE   "패치 정보 파일 없음."

KBrowserWrapper KPatchInfoBox::m_browser;   ///< HTML file browser

/// Open patch info box.
/// @param hParentWnd handle to parent window
/// @return the nResult parameter in the call to the EndDialog function used to terminate the dialog box
INT_PTR KPatchInfoBox::Open(HWND hParentWnd /* = NULL */)
{
    return ::DialogBox(::GetModuleHandle(NULL),
                       MAKEINTRESOURCE(IDD_PATCHINFOBOX),
                       ((NULL == hParentWnd) ? ::GetDesktopWindow() : hParentWnd),
                       KPatchInfoBox::PatchInfoBoxProc);
}

/// Application-defined dialog box callback function.
/// @param hDlg handle to dialog box
/// @param uMsg message
/// @param wParam first message parameter
/// @param lParam second message parameter
/// @return TRUE if it processed the message, and FALSE if it did not
BOOL CALLBACK KPatchInfoBox::PatchInfoBoxProc(HWND hDlg,
                                              UINT uMsg,
                                              WPARAM wParam,
                                              LPARAM lParam)
{
    static HBITMAP bkgndBmp     = NULL;
    static HBITMAP okBtnBmp     = NULL;
    //static HBITMAP cancelBtnBmp = NULL;

    switch (uMsg)
    {
        case WM_INITDIALOG:
        {
            bkgndBmp = ::LoadBitmap(::GetModuleHandle(NULL),
                                    MAKEINTRESOURCE(IDB_PATCHINFOBOX_BKGND));
            ASSERT(NULL != bkgndBmp);
            okBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),
                                    MAKEINTRESOURCE(IDB_PATCHINFOBOX_OK));
            ASSERT(NULL != okBtnBmp);

            // OK 버튼만 띄움
            /*
            cancelBtnBmp = ::LoadBitmap(::GetModuleHandle(NULL),
                                        MAKEINTRESOURCE(IDB_PATCHINFOBOX_CANCEL));
            ASSERT(NULL != cancelBtnBmp);
            */

            ::SendDlgItemMessage(hDlg, IDOK, BM_SETIMAGE, IMAGE_BITMAP,
                                 reinterpret_cast<LPARAM>(okBtnBmp));

            /*
            ::SendDlgItemMessage(hDlg, IDCANCEL, BM_SETIMAGE, IMAGE_BITMAP,
                                 reinterpret_cast<LPARAM>(cancelBtnBmp));
            */

            //::SetWindowText( hDlg, _T("엘소드") );
			::SetWindowText( hDlg, _T("Elsword") );
            ::SetWindowPos( hDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW );

            RECT rcWindow = { 0, };
            RECT rcClient = { 0, };
            HWND hEdit = ::GetDlgItem( hDlg, IDC_EDIT_PATCH_INFO );
            ::GetWindowRect( hEdit, &rcWindow );
            ::GetClientRect( hEdit, &rcClient );
            POINT pt = { rcWindow.left, rcWindow.top };
            ::ScreenToClient( hDlg, &pt );
            char szBuffer[MAX_PATH] = { 0, };

			WCHAR wcharBuf[255] = {0,};

            ::GetCurrentDirectory( MAX_PATH, wcharBuf );
				WideCharToMultiByte( CP_ACP, 0, wcharBuf, -1, szBuffer, 255, NULL, NULL );
            std::string strPath = szBuffer;
            if ( '\\' != strPath[strPath.size() - 1] ) strPath.append( "\\" );
            strPath.append( "PatchInfo.htm" );
 
			WCHAR wcharBuf2[255] = {0,};
			MultiByteToWideChar( CP_ACP, 0, strPath.c_str() , -1, wcharBuf2, 255 );	

            if ( true == m_browser.Create( /*const_cast<WCHAR*>( wcharBuf2 )*/ _T("http://59.25.186.60/PatchInfo.htm"), pt.x, pt.y,
                                           rcClient.right + ::GetSystemMetrics( SM_CXVSCROLL ),
                                           rcClient.bottom, hDlg,
                                           KBrowserWrapper::BROWSER_TYPE_CHILD ) )
            {
                ::DestroyWindow( hEdit );
                return TRUE;
            }

            HANDLE hFile  = NULL;
            DWORD  dwSize = 0;
            DWORD  dwRead = 0;

			MultiByteToWideChar( CP_ACP, 0, PATCH_INFO_FILE_NAME , -1, wcharBuf2, 255 );	

            if (INVALID_HANDLE_VALUE == (hFile = ::CreateFile(wcharBuf2,
                                                              GENERIC_READ,
                                                              0,
                                                              NULL,
                                                              OPEN_EXISTING,
                                                              FILE_ATTRIBUTE_NORMAL,
                                                              NULL)))
            {
                ::SendDlgItemMessage(hDlg, IDC_EDIT_PATCH_INFO, WM_SETTEXT, 0,
                                     reinterpret_cast<LPARAM>(NO_PATCH_INFO_MESSAGE));
                return TRUE;
            }

            ASSERT(NULL != hFile);

            if (INVALID_FILE_SIZE == (dwSize = ::GetFileSize(hFile, NULL)))
            {
                ::SendDlgItemMessage(hDlg, IDC_EDIT_PATCH_INFO, WM_SETTEXT, 0,
                                     reinterpret_cast<LPARAM>(NO_PATCH_INFO_MESSAGE));
                return TRUE;
            }

            char* szPatchInfo = new char[dwSize + 1];
            ASSERT(NULL != szPatchInfo);
            ::ZeroMemory(szPatchInfo, dwSize + 1);

            if (FALSE == ::ReadFile(hFile, szPatchInfo, dwSize, &dwRead, NULL))
            {
                ::SendDlgItemMessage(hDlg, IDC_EDIT_PATCH_INFO, WM_SETTEXT, 0,
                                     reinterpret_cast<LPARAM>(NO_PATCH_INFO_MESSAGE));
                return TRUE;
            }

            if (dwSize != dwRead)
            {
                ::SendDlgItemMessage(hDlg, IDC_EDIT_PATCH_INFO, WM_SETTEXT, 0,
                                     reinterpret_cast<LPARAM>(NO_PATCH_INFO_MESSAGE));
                return TRUE;
            }

            szPatchInfo[dwSize] = '\0';
            ::SendDlgItemMessage(hDlg, IDC_EDIT_PATCH_INFO, WM_SETTEXT, 0,
                                 reinterpret_cast<LPARAM>(szPatchInfo));

            delete[] szPatchInfo;
            ::CloseHandle(hFile);
            return TRUE;
        }
        case WM_DESTROY:
        {
            if (NULL != bkgndBmp)     { ::DeleteObject(bkgndBmp);     bkgndBmp     = NULL; }
            if (NULL != okBtnBmp)     { ::DeleteObject(okBtnBmp);     okBtnBmp     = NULL; }
            //if (NULL != cancelBtnBmp) { ::DeleteObject(cancelBtnBmp); cancelBtnBmp = NULL; }
            return TRUE;
        }
        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
                case IDOK:        ::EndDialog(hDlg, IDOK);              return TRUE;
                //case IDCANCEL:    ::EndDialog(hDlg, IDCANCEL);          return TRUE;
            }
            switch (HIWORD(wParam))
            {
                case EN_SETFOCUS: ::SetFocus(::GetDlgItem(hDlg, IDOK)); return TRUE;
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
            HBITMAP oldBmp = reinterpret_cast<HBITMAP>(::SelectObject(memDC, bkgndBmp));
            RECT rc;
            ::GetClientRect(hDlg, &rc);
            ::BitBlt(ps.hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);
            ::SelectObject(memDC, oldBmp);
            ::DeleteDC(memDC);
            ::EndPaint(hDlg, &ps);
            return TRUE;
        }
    }

    return FALSE;
}

/** @example KPatchInfoBox.h

#include "KPatchInfoBox.h"

/// Application's entry point.
/// @param hInstance handle to current instance
/// @param hPrevInstance handle to previous instance
/// @param lpCmdLine command line
/// @param nCmdShow show state
/// @return WM_QUIT message's wParam parameter or zero
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    INT_PTR ret = KPatchInfoBox::Open();

    if (IDOK == ret)
    {
        // TODO : If return value is IDOK...
    }
    else if (IDCANCEL == ret)
    {
        // TODO : If return value is IDCANCEL...
    }

    return 0;
}

*/