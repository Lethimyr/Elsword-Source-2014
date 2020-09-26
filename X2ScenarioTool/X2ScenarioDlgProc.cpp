#include "dxstdafx.h"
#include ".\x2scenariodlgproc.h"
#include "../resource.h"

namespace _DLG_PROC_
{
	//## 액션 컨트롤 조작 함수 모음..
	//# 1. 텍스쳐 출력 관련 조작 함수들..
	void AddAction01ComboboxItem( WCHAR* wszItem );
	//# 2. 텍스쳐 효과 

	//{{ 리소스 관리 프로시져.
	BOOL CALLBACK ResourceDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_INITDIALOG:
		case WM_CREATE:
			{
				//////////////////////////////////////////////////////////////////////////
				//# 리스트 뷰 초기화
				LVCOLUMN col;

				// 헤더추가.
				col.mask		= LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
				col.fmt			= LVCFMT_LEFT;
				col.cx			= 250;
				col.pszText		= L"파일명";
				col.iSubItem	= 0;
				::SendMessage(::GetDlgItem(hDlg,IDC_LIST1), LVM_INSERTCOLUMN,0,(LPARAM)&col);

				//col.mask		= LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
				//col.fmt			= LVCFMT_LEFT;
				////col.cx			= 50;
				//col.pszText		= L"-";
				//col.iSubItem	= 1;
				//::SendMessage(::GetDlgItem(hDlg,IDC_LIST1), LVM_INSERTCOLUMN,1,(LPARAM)&col);

				// 리스트뷰의 배경 격자를 표시 | 한 행 전체 선택
				ListView_SetExtendedListViewStyle(::GetDlgItem(hDlg,IDC_LIST1), LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
				//# 리스트 뷰 초기화
				//////////////////////////////////////////////////////////////////////////
				return TRUE;
			}
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
				return TRUE;
			}

		case WM_DROPFILES:
			{
				WCHAR FullFileName[MAX_PATH] = L"";
				WCHAR FileName[256] = L"";
				WCHAR FullDir[MAX_PATH] = L"";
				WCHAR drive[_MAX_DRIVE] = L"";
				WCHAR dir[_MAX_DIR] = L"";
				WCHAR fname[_MAX_FNAME] = L"";
				WCHAR ext[_MAX_EXT] = L"";

				DragQueryFile((HDROP) wParam, 0, FullFileName, MAX_PATH);
				_wsplitpath( FullFileName, drive, dir, fname, ext);

				wcscat( FileName, fname);
				wcscat( FileName, ext);

				wcscat( FullDir, drive );
				wcscat( FullDir, dir );

				//리스트뷰에 파일이름이 없으면 추가.
				LVFINDINFO plvfi;
				plvfi.flags	= LVFI_STRING;
				plvfi.psz	= FileName;
				if(ListView_FindItem(::GetDlgItem(hDlg,IDC_LIST1), -1, &plvfi) == -1)
				{
					// add file name by listbox
					LVITEM li;
					li.mask = LVIF_TEXT|LVIF_IMAGE;

					li.iSubItem	= 0;
					li.iItem	= 0;
					li.pszText	= FileName;
					::SendMessage(::GetDlgItem(hDlg,IDC_LIST1),LVM_INSERTITEM,0,(LPARAM)&li);

					// add file name by action01 of combo box
					AddAction01ComboboxItem(FileName);

					// add dir by engine
					std::string strDataDir;
					ConvertWCHARToChar( strDataDir, FullDir );
					g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strDataDir );
				}
				return TRUE;
			}

		//리스트뷰는 WM_NOTIFY 로 메세지가 날아온다.
		case WM_NOTIFY:
			{				
				if(((LPNMHDR)lParam)->hwndFrom == ::GetDlgItem(hDlg,IDC_LIST1))
				{
					switch(((LPNMHDR)lParam)->code)
					{
					case NM_DBLCLK:
						{
							//아이템 삭제처리.
							int iSelectItem = ListView_GetNextItem(::GetDlgItem(hDlg,IDC_LIST1), -1, LVNI_ALL|LVNI_SELECTED);
							if(iSelectItem != -1)
							{
								ListView_DeleteItem(::GetDlgItem(hDlg,IDC_LIST1), iSelectItem);
							}
						}
						return TRUE;
					}
				}
				return TRUE;
			}
		}

		return FALSE;
	}
	//}} 리소스 관리 프로시져.

	//{{ Action list
	BOOL CALLBACK ActionListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_CREATE:
		case WM_INITDIALOG:
			{
				//////////////////////////////////////////////////////////////////////////
				//# 리스트 뷰 초기화
				LVCOLUMN col;

				// 헤더추가.
				col.mask		= LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
				col.fmt			= LVCFMT_LEFT;
				col.cx			= 350;
				col.pszText		= L"액션 리스트.";
				col.iSubItem	= 0;
				::SendMessage(::GetDlgItem(hDlg,IDC_LIST_ACTION), LVM_INSERTCOLUMN,0,(LPARAM)&col);

				// 리스트뷰의 배경 격자를 표시 | 한 행 전체 선택 | 체크박스 달기
				ListView_SetExtendedListViewStyle(::GetDlgItem(hDlg,IDC_LIST_ACTION), LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_HEADERDRAGDROP);
				//# 리스트 뷰 초기화
				//////////////////////////////////////////////////////////////////////////

				LVITEM li;
				li.mask = LVIF_TEXT|LVIF_IMAGE;

				li.iSubItem	= 0;
				li.iItem	= 0;
				li.pszText	= L"Test";
				for(int i = 0; i < 10; ++i)
				{
					::SendMessage(::GetDlgItem(hDlg,IDC_LIST_ACTION),LVM_INSERTITEM,0,(LPARAM)&li);
				}
			}
			return TRUE;
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
				return TRUE;
			}
		}
		return FALSE;
	}
	//}} Action list
	
	//{{ 액션관리 프로시져.
	//# 액션에 사용되는 UI 프로시져 선언..

	//## 액션 컨트롤 조작 함수 모음..
	//# 1. 텍스쳐 출력 관련 조작 함수들..
	void AddAction01ComboboxItem( WCHAR* wszItem )
	{
		SendMessage(GetScenarioTool()->GetDlgItem(DH_TEX_IO, IDC_COMBO_SEL_IMAGE), CB_ADDSTRING, 0, (LPARAM)wszItem);
	}

	BOOL CALLBACK ActionTexIOProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK ActionTexture02Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	// 액션의 기능들을 탭으로 관리하는 부모 다이얼로그..
	// : 메뉴 탭 및 액션 다이얼로그 추가는 이곳에서..
	BOOL CALLBACK ActionDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_INITDIALOG:
		case WM_CREATE:
			{
				//## 탭 컨트롤 추가.
				TCITEM ti;
				ti.mask = TCIF_TEXT;
				//#이미지 출력.
				ti.pszText = L"이미지 입/출력";
				TabCtrl_InsertItem(::GetDlgItem(hDlg, IDC_TAB1), DH_TEX_IO, &ti);
				//#이미지 효과.
				ti.pszText = L"이미지 효과";
				TabCtrl_InsertItem(::GetDlgItem(hDlg, IDC_TAB1), DH_TEX_02, &ti);

				//## 액션 윈도우 생성.
				GetScenarioTool()->SetDlgHwnd(DH_TEX_IO, CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_ACTION_TEX_IO), hDlg, ActionTexIOProc));
				::ShowWindow(GetScenarioTool()->GetDlgHwnd(DH_TEX_IO), SW_SHOW);

				GetScenarioTool()->SetDlgHwnd(DH_TEX_02, CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_ACTION_TEXTURE_02), hDlg, ActionTexture02Proc));
			}
			return TRUE;
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
			}
			return TRUE;

		case WM_NOTIFY:
			{
				switch(((LPNMHDR)lParam)->code)
				{
				case TCN_SELCHANGE:
					{
						GetScenarioTool()->HideActionSubDlg();

						int iSelTabCtrl = TabCtrl_GetCurSel(::GetDlgItem(hDlg, IDC_TAB1));
						if(iSelTabCtrl >= 0 && iSelTabCtrl < DH_END)
							GetScenarioTool()->ShowActionSubDlg(iSelTabCtrl);
					}
				}
				return TRUE;
			}
		}

		return FALSE;
	}
	// 텍스쳐 입/출력..
	BOOL CALLBACK ActionTexIOProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_CREATE:
		case WM_INITDIALOG:
			{
				////# test
				//WCHAR wszTest[MAX_PATH] = L"TEST";
				//for(int i = 0; i < 10; ++i)
				//	SendMessage(::GetDlgItem(hDlg, IDC_COMBO_SEL_IMAGE), CB_ADDSTRING, 0, (LPARAM)wszTest);
			}
			return TRUE;
		}
		return FALSE;
	}
	// 텍스쳐 애니메이션..
	BOOL CALLBACK ActionTexture02Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		return FALSE;
	}
	//}} 액션관리 프로시져.
};

