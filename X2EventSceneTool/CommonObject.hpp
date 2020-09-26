#pragma once

//////////////////////////////////////////////////////////////////////////
//## common object by event scene tool
//## Des : 이벤트씬 관련 클래스 & 구조체 & 키 모음.
//////////////////////////////////////////////////////////////////////////

namespace _EST_OBJECT_
{
	enum DLG_HWND
	{
		DH_EVENTSCENE_LIST,
		DH_EVENTSCRIPT_EDIT,

		DH_END,
	};

	// dlg 를 이용한 file open & file save 
	inline bool GetDlgFileName( IN HWND hWndOwner, OUT std::wstring & wstrFileName, bool bIsOpen )
	{
		OPENFILENAME ofn;
		WCHAR wszFileName[512];  // path까지 포함한 파일 이름

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize		= sizeof(ofn);
		ofn.hwndOwner		= hWndOwner; 
		ofn.lpstrFile		= (LPWSTR)wszFileName;
		ofn.lpstrFile[0]	= '\0';
		ofn.nMaxFile		= sizeof(wszFileName);
		ofn.lpstrFilter		= L"lua script\0*.lua\0";
		ofn.nFilterIndex	= 1;
		ofn.lpstrFileTitle	= NULL;
		ofn.nMaxFileTitle	= 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if( bIsOpen == true )
		{
			if( TRUE == GetOpenFileName( &ofn ) )
			{
				wstrFileName = ofn.lpstrFile;
				return true;
			}
		}
		else
		{
			if( TRUE == GetSaveFileName( &ofn ) )
			{
				wstrFileName = ofn.lpstrFile;
				return true;
			}
		}

		return false;
	}

	// fullpath 를 받아 path, filename 을 분리 
	inline bool DecomposePathAndFileName( OUT WCHAR* wszPath, OUT WCHAR* wszFileName, IN const WCHAR* wszFullPath )
	{
		if( NULL == wszFullPath )
			return false;

		WCHAR drive[_MAX_DRIVE] = L"";
		WCHAR dir[_MAX_DIR] = L"";
		WCHAR fname[_MAX_FNAME] = L"";
		WCHAR ext[_MAX_EXT] = L"";

		_wsplitpath( wszFullPath, drive, dir, fname, ext);
		if( NULL != wszFileName )
		{
			wcscat( wszFileName, fname);
			wcscat( wszFileName, ext);	
		}
		if( NULL != wszPath )
		{
			wcscat( wszPath, drive );
			wcscat( wszPath, dir );
		}

		return true;
	}

	inline void LV_SetfocusAndFindItem(IN HWND hWnd, IN const WCHAR *wszFindItem)
	{
		//if( hWnd == NULL || wszFindItem == NULL )
		//	return;

		//// 포커스를 EventList 창으로 옴김
		//::SetFocus(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST));

		//LVFINDINFO plvfi;
		//plvfi.flags	= LVFI_STRING;
		//plvfi.psz	= wszFindItem;

		//int iSelIndex = ListView_FindItem(hWndList, -1, &plvfi);

		//if( -1 != iSelIndex )
		//{
		//	// 비선택상태로 초기화
		//	ListView_SetItemState(hWndList, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);

		//	// 선택된 항목으로 포커스 이동
		//	ListView_SetItemState(hWndList,         // handle to listview
		//		iSelIndex,
		//		LVIS_FOCUSED | LVIS_SELECTED,         // index to listview item
		//		LVIS_FOCUSED | LVIS_SELECTED // item state
		//		);

		//	//선택시킨 항목이 화면에 보여지고 있지 않다면 보이도록 스크롤.
		//	ListView_EnsureVisible(hWndList, iSelIndex, FALSE);
		//}
	}

	// key 1 : EVENT_SCENE_ID
	// key 2 : script of g_pEventScene:AddEventScene_LUA
	typedef std::map< std::string, std::string > EventSceneData;

	// key : EVENT_SCENE_ID
	typedef std::vector< std::string > EventIDList;
}












