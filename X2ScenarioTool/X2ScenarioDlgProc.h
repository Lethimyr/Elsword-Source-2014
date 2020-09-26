#pragma once

//////////////////////////////////////////////////////////////////////////
//## UI msg proc
//## Desc : UI 를 컨트롤 할 메세지 프로시져.
//##		- 컨터롤의 생성, 초기화, 입,출력
//////////////////////////////////////////////////////////////////////////

namespace _DLG_PROC_
{
	BOOL CALLBACK ResourceDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	BOOL CALLBACK ActionListDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

	BOOL CALLBACK ActionDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
};
