// DungeonInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "LineStartPositionInfo.h"


// CDungeonInfo

IMPLEMENT_DYNAMIC(CLineStartPositionInfo, CDockablePane)

CLineStartPositionInfo::CLineStartPositionInfo()
{

}

CLineStartPositionInfo::~CLineStartPositionInfo()
{
}


BEGIN_MESSAGE_MAP(CLineStartPositionInfo, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CDungeonInfo 메시지 처리기입니다.

int CLineStartPositionInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	//m_wndChild.Create(_T("STATIC"), _T("Dockable pane window"), WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 1234);
	m_LineStartPositionDlg.Create(IDD_DIALOG_LINE_STARTPOSITION, this);
	m_LineStartPositionDlg.ShowWindow(SW_SHOW);
	return 0;
}

void CLineStartPositionInfo::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	m_LineStartPositionDlg.MoveWindow(0,0,cx,cy);
// 	if(m_wndChild.GetSafeHwnd())
// 	{
// 		m_wndChild.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
// 	}
}
