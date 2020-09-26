// LineSector.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "LineSector.h"
#include "afxdialogex.h"


// CLineSector 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLineSector, CDialogEx)

CLineSector::CLineSector(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLineSector::IDD, pParent)
{

}

CLineSector::~CLineSector()
{
}

BOOL CLineSector::OnInitDialog()
{
	GetDlgItem(IDC_EDIT_SECTOR_WIDTH)->SetWindowTextW(L"768");
	GetDlgItem(IDC_EDIT_SECTOR_HEIGHT)->SetWindowTextW(L"768");
	GetDlgItem(IDC_EDIT_SECTOR_DEPTH)->SetWindowTextW(L"768");

	return TRUE;
}

void CLineSector::OnShowWindow( BOOL bShow, UINT nStatus )
{	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_bTileRender = bShow == TRUE ? true : false;
}

void CLineSector::OnClose()
{
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_bTileRender = false;

	CDialogEx::OnClose();
}

void CLineSector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLineSector, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_SECTOR_WIDTH, &CLineSector::OnEnChangeEditSectorWidth)
	ON_EN_CHANGE(IDC_EDIT_SECTOR_HEIGHT, &CLineSector::OnEnChangeEditSectorHeight)
	ON_EN_CHANGE(IDC_EDIT_SECTOR_DEPTH, &CLineSector::OnEnChangeEditSectorDepth)
	ON_BN_CLICKED(IDC_BUTTON1, &CLineSector::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLineSector 메시지 처리기입니다.


void CLineSector::OnEnChangeEditSectorWidth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csTemp;
	GetDlgItem(IDC_EDIT_SECTOR_WIDTH)->GetWindowText(csTemp);	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iSectorWidth = _ttoi(csTemp.GetString());
}


void CLineSector::OnEnChangeEditSectorHeight()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csTemp;
	GetDlgItem(IDC_EDIT_SECTOR_HEIGHT)->GetWindowText(csTemp);	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iSectorHeight = _ttoi(csTemp.GetString());
}


void CLineSector::OnEnChangeEditSectorDepth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csTemp;
	GetDlgItem(IDC_EDIT_SECTOR_DEPTH)->GetWindowText(csTemp);	
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_iSectorDepth = _ttoi(csTemp.GetString());
}


void CLineSector::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->CreateSector();

	wstring wstrSectorInfo = pMain->GetSectorInfo();
	GetDlgItem(IDC_EDIT_SECTOR_INFO)->SetWindowTextW(wstrSectorInfo.c_str());
}
