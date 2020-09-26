#include "dxstdafx.h"
#include ".\x2scenariotoolstage.h"
#include "../resource.h"

CX2ScenarioToolStage::CX2ScenarioToolStage(void)
{}

CX2ScenarioToolStage::~CX2ScenarioToolStage(void)
{
	for(int i = 0; i < static_cast<int>(DH_END); ++i)
	{
		::CloseWindow(m_hWnd[i]);
		m_hWnd[i] = NULL;
	}
}

HRESULT CX2ScenarioToolStage::OnFrameMove( double fTime, float fElapsedTime )
{
	m_cScenarioManager.OnFrameMove(fTime, fElapsedTime);

	return S_OK;
}

HRESULT CX2ScenarioToolStage::OnFrameRender()
{
	m_cScenarioManager.OnFrameRender();

	return S_OK;
}

HRESULT CX2ScenarioToolStage::OnResetDevice()
{
	m_cScenarioManager.OnResetDevice();

	return S_OK;
}

HRESULT CX2ScenarioToolStage::OnLostDevice()
{
	m_cScenarioManager.OnLostDevice();

	return S_OK;
}

bool CX2ScenarioToolStage::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_CREATE:
		::InitCommonControls();
		return true;
	}

	if( uMsg == WM_DROPFILES )
	{
		SendMessage(GetDlgHwnd(DH_RESOURCE), uMsg, wParam, lParam);
	}

	return m_cScenarioManager.MsgProc(hWnd, uMsg, wParam, lParam);
}

void CX2ScenarioToolStage::CreateDlg()
{
	//府家胶 包府.
	SetDlgHwnd(DH_RESOURCE, CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_DIALOG_RESOURCE), g_pKTDXApp->GetHWND(), _DLG_PROC_::ResourceDlgProc));
	::ShowWindow(GetDlgHwnd(DH_RESOURCE), SW_SHOW);
	//咀记 包府.
	SetDlgHwnd(DH_ACTION, CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_ACTION), g_pKTDXApp->GetHWND(), _DLG_PROC_::ActionDlgProc));
	::ShowWindow(GetDlgHwnd(DH_ACTION), SW_SHOW);
	//咀记 府胶飘.
	SetDlgHwnd(DH_ACTION_LIST, CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_ACTION_LIST), g_pKTDXApp->GetHWND(), _DLG_PROC_::ActionListDlgProc));
	::ShowWindow(GetDlgHwnd(DH_ACTION_LIST), SW_SHOW);
}

void CX2ScenarioToolStage::SetDlgHwnd(DLG_HWND dlgType, HWND hWnd)
{
	if(dlgType >= 0 && dlgType < DH_END)
		m_hWnd[dlgType] = hWnd;
	else
		assert(dlgType >= 0 && dlgType < DH_END);
}

HWND CX2ScenarioToolStage::GetDlgHwnd(DLG_HWND dlgType)
{
	if(dlgType >= 0 && dlgType < DH_END)
		return m_hWnd[dlgType];
	else
		assert(dlgType >= 0 && dlgType < DH_END);

	return NULL;
}

HWND CX2ScenarioToolStage::GetDlgItem(DLG_HWND dlgType, int iDlgItem)
{
	if(dlgType >= 0 && dlgType < DH_END)
		return ::GetDlgItem(m_hWnd[dlgType], iDlgItem);
	else
		assert(dlgType >= 0 && dlgType < DH_END);

	return NULL;
}

void CX2ScenarioToolStage::HideActionSubDlg()
{
	::ShowWindow(GetDlgHwnd(DH_TEX_IO), SW_HIDE);
	::ShowWindow(GetDlgHwnd(DH_TEX_02), SW_HIDE);
}

void CX2ScenarioToolStage::ShowActionSubDlg(int iSelSubDlg)
{
	::ShowWindow(GetDlgHwnd(static_cast<DLG_HWND>(DH_TEX_IO+iSelSubDlg)), SW_SHOW);
}