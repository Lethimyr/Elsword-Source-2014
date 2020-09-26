#include "dxstdafx.h"
#include ".\dlgproc.h"
#include ".\resource.h"

namespace _DLG_PROC_
{
	// EventScene List 의 Search 기능 지원시 Search Edit 의 엔터키 기능 지원을 위해
	WNDPROC OldSearchEditProc;

	//{{Begin} message handler for event lst
	BOOL CALLBACK EventSceneListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
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
				col.pszText		= L"EVENT ID";
				col.iSubItem	= 0;
				::SendMessage(::GetDlgItem(hDlg,IDC_LIST1), LVM_INSERTCOLUMN,0,(LPARAM)&col);

				// 리스트뷰의 배경 격자를 표시 | 한 행 전체 선택
				ListView_SetExtendedListViewStyle(::GetDlgItem(hDlg,IDC_LIST1), LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
				//# 리스트 뷰 초기화
				//////////////////////////////////////////////////////////////////////////

				//# Search edit binding for subproc
				OldSearchEditProc = (WNDPROC)(::SetWindowLongPtr(::GetDlgItem(hDlg,IDC_EDIT_SEARCH_ID), GWLP_WNDPROC, (LONG)EventSceneListSearchEditSubProc));
				return TRUE;
			}
		case WM_CLOSE:
		case WM_DESTROY:
			{
				::SetWindowLongPtr(::GetDlgItem(hDlg,IDC_EDIT_SEARCH_ID), GWLP_WNDPROC, (LONG)(OldSearchEditProc));

				DestroyWindow(hDlg);
				hDlg = NULL;
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
							int iSelectItem = ListView_GetNextItem(::GetDlgItem(hDlg,IDC_LIST1), -1, LVNI_ALL|LVNI_SELECTED);
							if(iSelectItem != -1)
							{
								// 리스트에서 선택된 항목 텍스트를 가져옴
								WCHAR wszSelectItemText[256] = L"";
								ListView_GetItemText(::GetDlgItem(hDlg,IDC_EVENTID_LIST), iSelectItem, 0, wszSelectItemText, 255);

								// UI 에디트 창에 스크립트를 뿌려준다.
								std::wstring wstrUTF8Script;
								if( GetRenderFrm()->GetEventScriptParsing()->GetUTF8byEventScriptData(wszSelectItemText, wstrUTF8Script) == true )
								{
									SetWindowText(::GetDlgItem(GetRenderFrm()->GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCRIPT_EDIT), IDC_EDIT1), wstrUTF8Script.c_str());
								}
							}
						}return TRUE;

					case NM_RDBLCLK:
						{
							int iSelectItem = ListView_GetNextItem(::GetDlgItem(hDlg,IDC_LIST1), -1, LVNI_ALL|LVNI_SELECTED);
							if(iSelectItem != -1)
							{
								// 리스트에서 선택된 항목 텍스트를 가져옴
								WCHAR wszSelectItemText[256] = L"";
								ListView_GetItemText(::GetDlgItem(hDlg,IDC_EVENTID_LIST), iSelectItem, 0, wszSelectItemText, 255);

								// 플레이~~
								if( true == GetRenderFrm()->GetX2EventScene()->PlayEventScene(wszSelectItemText) )
									GetRenderFrm()->SetLastPlaySceneID(wszSelectItemText);
							}
						}return TRUE;
					}
				}
				return TRUE;
			}

		case WM_COMMAND:
			{
				int wmID, wmEvent;
				wmID = LOWORD(wParam);
				wmEvent = HIWORD(wParam);

				// parse the menu selections
				switch(wmID)
				{
				case IDC_BUT_OPEN:
					{
						std::wstring wstrFileName;
						if( true == _EST_OBJECT_::GetDlgFileName(g_pKTDXApp->GetHWND(), wstrFileName, true) )
						{
							GetRenderFrm()->GetEventScriptParsing()->Clear();
							GetRenderFrm()->GetX2EventScene()->Clear();

							GetRenderFrm()->GetEventScriptParsing()->OpenScriptFile(wstrFileName.c_str());

							if( GetRenderFrm()->GetX2EventScene()->OpenScriptFile(L"EventSceneObject.lua") == false )
							{
								std::wstring wstrMSG;
								wstrMSG = L"루아 파싱 실패.!";
								wstrMSG += L"EventSceneObject.lua";
								ERRORMSG(wstrMSG.c_str());
							}
							WCHAR wszPath[MAX_PATH] = L"";
							WCHAR wszFileName[MAX_PATH] = L"";
							// 풀패스를 경로와 파일이름으로 분리
							if( true == _EST_OBJECT_::DecomposePathAndFileName(wszPath, wszFileName, wstrFileName.c_str()) )
							{
								if( GetRenderFrm()->GetX2EventScene()->OpenScriptFile(wszFileName) == false )
								{
									std::wstring wstrMSG;
									wstrMSG = L"루아 파싱 실패.!";
									wstrMSG += wszFileName;
									ERRORMSG(wstrMSG.c_str());
								}
							}

							_EST_OBJECT_::EventIDList eventIDList;
							GetRenderFrm()->GetEventScriptParsing()->GetEventIDList(eventIDList);

							// 리스트뷰에 이벤트 아이디 추가
							for( int i = 0; i < static_cast<int>(eventIDList.size()); ++i )
							{
								WCHAR wstrTemp[256] = L"";
								::MultiByteToWideChar( CP_ACP, 0, eventIDList[i].c_str(), -1, wstrTemp, 255 );

								LVFINDINFO plvfi;
								plvfi.flags	= LVFI_STRING;
								plvfi.psz	= wstrTemp;

								// 아이디 추가하면서 같은 이름의 아이디가 있는지 체크
								if(ListView_FindItem(::GetDlgItem(hDlg,IDC_EVENTID_LIST), -1, &plvfi) == -1)
								{
									// add file name by listbox
									LVITEM li;
									li.mask = LVIF_TEXT|LVIF_IMAGE;

									li.iSubItem	= 0;
									li.iItem	= 0;
									li.pszText	= wstrTemp;
									::SendMessage(::GetDlgItem(hDlg,IDC_EVENTID_LIST),LVM_INSERTITEM,0,(LPARAM)&li);
								}
							}
						}
					}break;

				case IDC_BUT_SAVE:
					{
						GetRenderFrm()->GetEventScriptParsing()->SaveScript();
					}break;

				case IDC_BUT_ADD:
					{
					}break;
				}

				return TRUE;
			}
		}

		return FALSE;
	}
	//{{End} message handler for event list

	//{{Begin} message handler for event editer
	BOOL CALLBACK EventSceneEditerDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		switch(iMessage)
		{
		case WM_CREATE:
		case WM_INITDIALOG:
			{
			}
			return TRUE;
		case WM_CLOSE:
		case WM_DESTROY:
			{
				DestroyWindow(hDlg);
				hDlg = NULL;
				return TRUE;
			}

		case WM_COMMAND:
			{
				int wmID, wmEvent;
				wmID = LOWORD(wParam);
				wmEvent = HIWORD(wParam);

				// parse the menu selections
				switch(wmID)
				{
				case IDC_BUT_REFRESH:
					{
						int iScriptSize = 0;
						WCHAR *pwszBuffer = new WCHAR[MAX_SCRIPT_BUFFER];

						// edit 창에 있는 데이터를 읽어와..
						iScriptSize = ::GetWindowText(::GetDlgItem(hDlg, IDC_EDIT1),pwszBuffer,MAX_SCRIPT_BUFFER);

						if( iScriptSize > 0 )
						{
							// 새로 파싱을 하도록 한다.
							bool bResult = false;
							bResult = GetRenderFrm()->GetEventScriptParsing()->ScriptRefresh(pwszBuffer, iScriptSize + 1); // +1 을 해주는 이유는 파일 엔드 포인트까지 잃도록 하기위해
						}

						SAFE_DELETE_ARRAY(pwszBuffer); 
					}break;
				}

				return TRUE;
			}
		}
		return FALSE;
	}
	//{{End} message handler for event editer

	//{{Begin} message handler for search edit by event list
	LRESULT	CALLBACK EventSceneListSearchEditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		if( iMessage == WM_KEYDOWN && wParam == VK_RETURN )
		{
			HWND hWndList = ::GetDlgItem(GetRenderFrm()->GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), IDC_EVENTID_LIST);
			HWND hWndStatic = ::GetDlgItem(GetRenderFrm()->GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), IDC_STATIC_SEARCH_MSG);

			if(NULL != hWndList)
			{
				WCHAR wszID[256] = L"";
				::GetWindowText(hWnd, wszID, 256);

				LVFINDINFO plvfi;
				plvfi.flags	= LVFI_STRING;
				plvfi.psz	= wszID;

				//int iSelIndex = ListView_FindItem(hWndList, -1, &plvfi);

				//# 부분선택 검색을 위해 아이템 항목을 뽑아 직접 검색
				int iSelIndex = -1;
				int iMaxListCount = ListView_GetItemCount(hWndList);
				WCHAR wszTemp[256] = L"";
				for( int i = 0; i < iMaxListCount; ++i )
				{
					ListView_GetItemText(hWndList, i, 0, wszTemp, 256);
					if( 0 == wcsncmp(wszID, wszTemp, wcslen(wszID)) )
					{
						iSelIndex = i;
						break;
					}
				}

				if( -1 != iSelIndex )
				{
					::SetFocus(hWndList);

					// 비선택상태로 초기화
					ListView_SetItemState(hWndList, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);

					// 선택된 항목으로 포커스 이동
					ListView_SetItemState(hWndList,         // handle to listview
						iSelIndex,
						LVIS_FOCUSED | LVIS_SELECTED,         // index to listview item
						LVIS_FOCUSED | LVIS_SELECTED // item state
						);

					//선택시킨 항목이 화면에 보여지고 있지 않다면 보이도록 스크롤.
					ListView_EnsureVisible(hWndList, iSelIndex, FALSE);

					std::wstring wstrMsg = L"찾기성공";
					wstrMsg += L"(";
					wstrMsg += wszID;
					wstrMsg += L")";
					::SetWindowText(hWndStatic, wstrMsg.c_str());
				}
				else
				{
					std::wstring wstrMsg = L"찾기실패";
					wstrMsg += L"(";
					wstrMsg += wszID;
					wstrMsg += L")";
					::SetWindowText(hWndStatic, wstrMsg.c_str());
				}
			}
		}

		return CallWindowProc( OldSearchEditProc, hWnd, iMessage, wParam, lParam );
	}
	//{{End} message handler for search edit by event list
};

