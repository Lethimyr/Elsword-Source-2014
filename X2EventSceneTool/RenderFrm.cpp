#include "dxstdafx.h"
#include ".\renderfrm.h"
#include ".\resource.h"

CRenderFrm::CRenderFrm(void)
{
	//{{Begin} 옵션 다이얼로그 생성
	//이벤트 리스트 DLG
	SetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST,
				CreateDialog(g_pKTDXApp->GetHInstance(),
				MAKEINTRESOURCE(IDD_EVENTSCENE_LIST),
				g_pKTDXApp->GetHWND(),
				&_DLG_PROC_::EventSceneListDlgProc));
	::ShowWindow(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), SW_SHOW);
	//이벤트 에디터 DLG
	SetDlgHwnd(_EST_OBJECT_::DH_EVENTSCRIPT_EDIT,
				CreateDialog(g_pKTDXApp->GetHInstance(),
				MAKEINTRESOURCE(IDD_EVENTSCENE_EDIT),
				g_pKTDXApp->GetHWND(),
				&_DLG_PROC_::EventSceneEditerDlgProc));
	::ShowWindow(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCRIPT_EDIT), SW_SHOW);

	//{{End} 옵션 다이얼로그 생성

//  	//{{begin} CX2EventScene 에서 필요힌 함수 바인딩 및 루아 데이터 처리
//  	if( g_pKTDXApp->GetDeviceManager()->LoadLuaTinker(L"Enum.lua") == false )
//  	{
//  		ERRORMSG(L"Enum.lua parsing fail.!");
//  	}
//  	//{{end} CX2EventScene 에서 필요힌 함수 바인딩 및 루아 데이터 처리
}

CRenderFrm::~CRenderFrm(void)
{
	for( int i = 0; i < _EST_OBJECT_::DH_END; ++i )
		::DestroyWindow(m_hWnd[i]);

	g_pRenderFrm = NULL;
}


HRESULT CRenderFrm::OnFrameMove( double fTime, float fElapsedTime )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->OnFrameMove( fTime, fElapsedTime );
	g_pKTDXApp->GetDGManager()->OnFrameMove( fTime, fElapsedTime );

	m_EventScene.OnFrameMove(fTime, fElapsedTime);

	return S_OK;
}

HRESULT CRenderFrm::OnFrameRender()
{
	g_pKTDXApp->GetDGManager()->ObjectChainSort();

	g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
	g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();
	g_pKTDXApp->GetDGManager()->FrontUIRender();

	return S_OK;
}

HRESULT CRenderFrm::OnResetDevice()
{
	return S_OK;
}

HRESULT CRenderFrm::OnLostDevice()
{

	return S_OK;
}

bool CRenderFrm::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	m_EventScene.MsgProc(hWnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_CREATE:
		::InitCommonControls();
		return true;

	case WM_LBUTTONDBLCLK:
		GetRenderFrm()->SetFocusPlaySceneID();
		return true;
	}

	return false;
}

void CRenderFrm::SetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType, HWND hWnd )
{
	if(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END)
		m_hWnd[dlgType] = hWnd;
	else
		assert(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END);
}

HWND CRenderFrm::GetDlgHwnd( _EST_OBJECT_::DLG_HWND dlgType )
{
	if(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END)
		return m_hWnd[dlgType];
	else
		assert(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END);

	return NULL;
}

HWND CRenderFrm::GetDlgItem( _EST_OBJECT_::DLG_HWND dlgType, int iDlgItem )
{
	if(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END)
		return ::GetDlgItem(m_hWnd[dlgType], iDlgItem);
	else
		assert(dlgType >= 0 && dlgType < _EST_OBJECT_::DH_END);

	return NULL;
}

void CRenderFrm::SetFocusPlaySceneID()
{
	if( false == GetLastPlaySceneID().empty() )
	{
		HWND hWndList = ::GetDlgItem(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST), IDC_EVENTID_LIST);
		if(NULL == hWndList)
			return;

		WCHAR wszID[256] = L"";
		wcscpy(wszID, GetLastPlaySceneID().c_str());

		// 포커스를 EventList 창으로 옴김
		::SetFocus(GetDlgHwnd(_EST_OBJECT_::DH_EVENTSCENE_LIST));

		LVFINDINFO plvfi;
		plvfi.flags	= LVFI_STRING;
		plvfi.psz	= wszID;

		int iSelIndex = ListView_FindItem(hWndList, -1, &plvfi);

		if( -1 != iSelIndex )
		{
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
		}
	}
}