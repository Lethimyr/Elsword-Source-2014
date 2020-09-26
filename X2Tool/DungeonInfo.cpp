// DungeonInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "DungeonInfo.h"


// CDungeonInfo

IMPLEMENT_DYNAMIC(CDungeonInfo, CDockablePane)

CDungeonInfo::CDungeonInfo()
{

}

CDungeonInfo::~CDungeonInfo()
{
}


BEGIN_MESSAGE_MAP(CDungeonInfo, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDungeonInfo 메시지 처리기입니다.

int CDungeonInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_wndChild.Create(_T("STATIC"), _T("Dockable pane window"), WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 1234);
	m_DungeonDlg.Create(IDD_DIALOG_TEST, this);
	m_DungeonDlg.ShowWindow(SW_SHOW);
	return 0;
}

void CDungeonInfo::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_DungeonDlg.MoveWindow(0,0,cx,cy);
// 	if(m_wndChild.GetSafeHwnd())
// 	{
// 		m_wndChild.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
// 	}
}
