#pragma once

//////////////////////////////////////////////////////////////////////////
//## UI msg proc
//## Desc : UI 를 컨트롤 할 메세지 프로시져.
//##		- 컨터롤의 생성, 초기화, 입,출력
//////////////////////////////////////////////////////////////////////////

namespace _DLG_PROC_
{
	BOOL CALLBACK EventSceneListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	BOOL CALLBACK EventSceneEditerDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	LRESULT	CALLBACK EventSceneListSearchEditSubProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
