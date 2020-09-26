// LineBaseEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "MainFrm.h"
#include "LineBaseEditor.h"
#include "afxdialogex.h"


// CLineBaseEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLineBaseEditor, CDialogEx)

CLineBaseEditor::CLineBaseEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLineBaseEditor::IDD, pParent)
{
	m_bCheckList = false;
	EnableAutomation();

}

CLineBaseEditor::~CLineBaseEditor()
{
}

void CLineBaseEditor::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialogEx::OnFinalRelease();
}

void CLineBaseEditor::UpdateLineInfo()
{
	CKTDGLineMap *pLineMap = g_pX2Game->GetWorld()->GetLineMap();
	
	int iScrollPos = m_LineList.GetScrollPos(SB_VERT);

	m_LineList.DeleteColumn(3);
	m_LineList.DeleteColumn(2);
	m_LineList.DeleteColumn(1);
	m_LineList.DeleteColumn(0);	
	m_LineList.DeleteAllItems();


	m_LineList.InsertColumn(0, _T("Line"), LVCFMT_CENTER, 40 );
	m_LineList.InsertColumn(1, _T("Set"), LVCFMT_CENTER, 30 );
	m_LineList.InsertColumn(2, _T("C"), LVCFMT_CENTER, 30 );
	m_LineList.InsertColumn(3, _T("T"), LVCFMT_CENTER, 30 );

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData != NULL )
		{			
			WCHAR wLineBuf[10];
			wsprintfW(wLineBuf, L"%d", i);
			m_LineList.InsertItem(i, wLineBuf);

			WCHAR wLineSetBuf[10];
			wsprintfW(wLineSetBuf, L"%d", pLineData->lineSet);
			m_LineList.SetItemText(i, 1, wLineSetBuf);

			WCHAR wLineBufCamera[10];
			wsprintfW(wLineBufCamera, L"%d", pLineData->m_iOriCameraIndex);
			m_LineList.SetItemText(i, 2, wLineBufCamera);

			WCHAR wLineBufTrigger[10];
			wsprintfW(wLineBufTrigger, L"%d", pLineData->m_iTriggerId);
			m_LineList.SetItemText(i, 3, wLineBufTrigger);

		}
		else
		{
			m_LineList.InsertItem(i, L"Empty");
		}
	}
	m_LineList.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES) );
	
	UpdateData(TRUE);

	int iNum(-1);
	POSITION pos = m_LineList.GetFirstSelectedItemPosition();
	iNum = m_LineList.GetNextSelectedItem(pos);

	CRect ref;
	m_LineList.GetItemRect(0, ref, LVIR_BOUNDS);
	CSize szHeight(0, ref.Height() * iScrollPos);
	m_LineList.Scroll(szHeight);
	m_LineList.SetScrollPos(SB_VERT, iScrollPos, FALSE);
}

void CLineBaseEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_LINE, m_LineList);
	DDX_Control(pDX, IDC_LIST_LINE, m_LineList);
	DDX_Control(pDX, IDC_EL_SNAP, m_bCheckSnap);
	DDX_Control(pDX, IDC_SPIN_EL_FILLET, m_spinCtrl);
	//DDX_Control(pDX, IDC_SPIN_EL_FILLET, m_spinCtrl);
}


BEGIN_MESSAGE_MAP(CLineBaseEditor, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_RADIO_LINE_VERTEX, &CLineBaseEditor::OnBnClickedRadioLineVertex)
	ON_BN_CLICKED(IDC_RADIO_LINE_SEGMENT, &CLineBaseEditor::OnBnClickedRadioLineSegment)
	ON_BN_CLICKED(ID_EL_MAKEFIRST, &CLineBaseEditor::OnBnClickedElMakefirst)
	ON_BN_CLICKED(ID_EL_WELD, &CLineBaseEditor::OnBnClickedElWeld)
	ON_BN_CLICKED(IDC_EL_SNAP, &CLineBaseEditor::OnBnClickedElSnap)
	ON_BN_CLICKED(ID_EL_DIVIDE, &CLineBaseEditor::OnBnClickedElDivide)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EL_FILLET, &CLineBaseEditor::OnDeltaposSpinElFillet)
	ON_BN_CLICKED(IDC_EL_JOIN, &CLineBaseEditor::OnBnClickedElJoin)
	ON_BN_CLICKED(ID_EL_COPY, &CLineBaseEditor::OnBnClickedElCopy)
//	ON_BN_CLICKED(IDC_BUTTON2, &CLineBaseEditor::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON1, &CLineBaseEditor::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON_POS, &CLineBaseEditor::OnBnClickedButtonPos)
//	ON_BN_CLICKED(IDC_BUTTON3, &CLineBaseEditor::OnBnClickedButton3)
//	ON_BN_CLICKED(IDC_BUTTON_LINEINDEX, &CLineBaseEditor::OnBnClickedButtonLineindex)
	ON_BN_CLICKED(IDC_RADIO_LINE_STARTPOSITION, &CLineBaseEditor::OnBnClickedRadioLineStartposition)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_LINE, &CLineBaseEditor::OnLvnEndlabeleditListLine)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LINE, &CLineBaseEditor::OnLvnItemchangedListLine)
	ON_BN_CLICKED(IDC_BUTTON_REARRANGE, &CLineBaseEditor::OnBnClickedButtonRearrange)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLineBaseEditor, CDialogEx)
END_DISPATCH_MAP()

// 참고: IID_ILineBaseEditor에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {6A86666B-21C7-49EC-AA8E-E695BFE66C26}
static const IID IID_ILineBaseEditor =
{ 0x6A86666B, 0x21C7, 0x49EC, { 0xAA, 0x8E, 0xE6, 0x95, 0xBF, 0xE6, 0x6C, 0x26 } };

BEGIN_INTERFACE_MAP(CLineBaseEditor, CDialogEx)
	INTERFACE_PART(CLineBaseEditor, IID_ILineBaseEditor, Dispatch)
END_INTERFACE_MAP()


void CLineBaseEditor::SetFilletValue(int iVal)
{
	WCHAR wFillet[10];
	wsprintfW(wFillet, L"%d", iVal);
	GetDlgItem(IDC_EDIT_EL_FILLET)->SetWindowText(wFillet);
}

void CLineBaseEditor::SetVertexPos(D3DXVECTOR3 vPos)
{
// 	WCHAR wVertX[20], wVertY[20], wVertZ[20];
// 	StringCchPrintfW( wVertX, ARRAY_SIZE( wVertX ), L" %5.2f", vPos.x );
// 	StringCchPrintfW( wVertY, ARRAY_SIZE( wVertY ), L" %5.2f", vPos.y );
// 	StringCchPrintfW( wVertZ, ARRAY_SIZE( wVertZ ), L" %5.2f", vPos.z );
// 
// 	GetDlgItem(IDC_EDIT_VERT_X)->SetWindowText(wVertX);
// 	GetDlgItem(IDC_EDIT_VERT_Y)->SetWindowText(wVertY);
// 	GetDlgItem(IDC_EDIT_VERT_Z)->SetWindowText(wVertZ);
}

BOOL CLineBaseEditor::OnInitDialog()
{
	UpdateData();

	m_spinCtrl.SetRange(0, 9999);
	UDACCEL accels[] = { {0,1} };
	m_spinCtrl.SetAccel( 0, accels );
		
	GetDlgItem(IDC_EDIT_EL_COPY)->SetWindowText(L"240");

	return TRUE;
}

BOOL CLineBaseEditor::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN )
			return TRUE;
		if( pMsg->wParam == VK_DELETE )
		{
			CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
			if( pMain != NULL )
			{
				pMain->DoDeleteEditLine();
				return TRUE;
			}			
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLineBaseEditor::OnShowWindow(BOOL bShow, UINT nStatus)
{
	static bool bFirstShow = true;
	if( g_pMain == NULL && g_pX2Game == NULL )
		return;

	if( bFirstShow == false )
		return;

	if( bShow == false )
	{
		return;
	}

	bFirstShow = false;
	

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLineVertex == true )
	{
		CheckRadioButton(IDC_RADIO_LINE_VERTEX, IDC_RADIO_LINE_STARTPOSITION, IDC_RADIO_LINE_VERTEX);				
		OnBnClickedRadioLineVertex();
	}
	else
	{
		if( pMain->m_bEditStartPosition == false )
		{
			CheckRadioButton(IDC_RADIO_LINE_VERTEX, IDC_RADIO_LINE_STARTPOSITION, IDC_RADIO_LINE_SEGMENT);
			OnBnClickedRadioLineSegment();
		}
		else
		{
			CheckRadioButton(IDC_RADIO_LINE_VERTEX, IDC_RADIO_LINE_STARTPOSITION, IDC_RADIO_LINE_STARTPOSITION);
			OnBnClickedRadioLineStartposition();
		}
		
	}
	
	
	UpdateLineInfo();
	
	GetDlgItem(IDC_EDIT_EL_DIVIDE)->SetWindowText(L"2");
}


void CLineBaseEditor::OnBnClickedRadioLineVertex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	GetDlgItem(ID_EL_MAKEFIRST)->EnableWindow(TRUE);
	GetDlgItem(ID_EL_DIVIDE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_EL_DIVIDE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EL_FILLET)->EnableWindow(TRUE);
	GetDlgItem(ID_EL_WELD)->EnableWindow(TRUE);
	GetDlgItem(IDC_EL_SNAP)->EnableWindow(TRUE);	
	GetDlgItem(IDC_EDIT_EL_FILLET)->EnableWindow(TRUE);
	GetDlgItem(IDC_SPIN_EL_FILLET)->EnableWindow(TRUE);
	GetDlgItem(IDC_EL_JOIN)->EnableWindow(TRUE);
	GetDlgItem(ID_EL_COPY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_EL_COPY)->EnableWindow(FALSE);

// 	GetDlgItem(IDC_EDIT_VERT_X)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT_VERT_Y)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_EDIT_VERT_Z)->EnableWindow(TRUE);
// 	GetDlgItem(IDC_BUTTON_POS)->EnableWindow(TRUE);

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_bEditLineVertex = true;
	pMain->m_bEditStartPosition = false;

	CKTDGLineMap *pLinemap = g_pX2Game->GetWorld()->GetLineMap();
	for(int i=0; i<pLinemap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLinemap->AccessLineData(i);
		pLineData->m_bOver = false;
		pLineData->m_bSelected = false;
	}

	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();		
	pMainFrame->m_wndProperties.UpdateProp(1);
}


void CLineBaseEditor::OnBnClickedRadioLineSegment()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	GetDlgItem(ID_EL_MAKEFIRST)->EnableWindow(FALSE);
	GetDlgItem(ID_EL_DIVIDE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_EL_DIVIDE)->EnableWindow(TRUE);
	GetDlgItem(IDC_EL_FILLET)->EnableWindow(FALSE);
	GetDlgItem(ID_EL_WELD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EL_SNAP)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_EL_FILLET)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_EL_FILLET)->EnableWindow(FALSE);
	GetDlgItem(IDC_EL_JOIN)->EnableWindow(FALSE);
	GetDlgItem(ID_EL_COPY)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_EL_COPY)->EnableWindow(TRUE);		
	
// 	GetDlgItem(IDC_EDIT_VERT_X)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT_VERT_Y)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT_VERT_Z)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_BUTTON_POS)->EnableWindow(FALSE);	

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_bEditLineVertex = false;
	pMain->m_bEditStartPosition = false;

	CKTDGLineMap *pLinemap = g_pX2Game->GetWorld()->GetLineMap();
	for(int i=0; i<pLinemap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLinemap->AccessLineData(i);
		pLineData->m_bOver = false;
		pLineData->m_bSelected = false;
	}

	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();		
	pMainFrame->m_wndProperties.UpdateProp(2);
}


void CLineBaseEditor::OnBnClickedElMakefirst()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true && pMain->m_bEditLineVertex == true && pMain->m_bEditStartPosition == false )
	{
		pMain->SetMakeFirst();
	}
}


void CLineBaseEditor::OnBnClickedElWeld()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true && pMain->m_bEditLineVertex == true && pMain->m_bEditStartPosition == false )
	{
		pMain->SetWeld();
	}
}


void CLineBaseEditor::OnBnClickedElSnap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	UINT bChecked = IsDlgButtonChecked(IDC_EL_SNAP);
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true )
	{
		if( bChecked == BST_CHECKED )
			pMain->m_bSnap = true;
		else
			pMain->m_bSnap = false;
	}
}


void CLineBaseEditor::OnBnClickedElDivide()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString csDivide;
	GetDlgItem(IDC_EDIT_EL_DIVIDE)->GetWindowText(csDivide);

	int iCount = _ttoi(csDivide);
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true )
		pMain->DivideLine(iCount);
}


void CLineBaseEditor::OnDeltaposSpinElFillet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	CString csDivide;
	GetDlgItem(IDC_EDIT_EL_FILLET)->GetWindowText(csDivide);

	int iCount = _ttoi(csDivide);
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true )
		pMain->SetFillet(iCount);
}


void CLineBaseEditor::OnBnClickedElJoin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true )
		pMain->JoinVertex();
}


void CLineBaseEditor::OnBnClickedElCopy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString csDivide;
	GetDlgItem(IDC_EDIT_EL_COPY)->GetWindowText(csDivide);

	int iCount = _ttoi(csDivide);
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true )
		pMain->CopyLine(iCount);
}

void CLineBaseEditor::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	CString csPos;
// 	GetDlgItem(IDC_EDIT_VERT_X)->GetWindowText(csPos);
// 	float fX = _ttof(csPos);
// 
// 	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
// 	if( pMain->m_bEditLine == true )
// 		pMain->SetVertexPos( D3DXVECTOR3(fX, 0.f, 0.f), D3DXVECTOR3(1.f, 0.f, 0.f), true );
}

void CLineBaseEditor::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	CString csPos;
// 	GetDlgItem(IDC_EDIT_VERT_Y)->GetWindowText(csPos);
// 	float fY = _ttof(csPos);
// 
// 	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
// 	if( pMain->m_bEditLine == true )
// 		pMain->SetVertexPos( D3DXVECTOR3(0.f, fY, 0.f), D3DXVECTOR3(0.f, 1.f, 0.f), true );
}

void CLineBaseEditor::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	CString csPos;
// 	GetDlgItem(IDC_EDIT_VERT_Z)->GetWindowText(csPos);
// 	float fZ = _ttof(csPos);
// 
// 	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
// 	if( pMain->m_bEditLine == true )
// 		pMain->SetVertexPos( D3DXVECTOR3(0.f, 0.f, fZ), D3DXVECTOR3(0.f, 0.f, 1.f), true );
}

void CLineBaseEditor::OnBnClickedButtonPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	CString csX, csY, csZ;
// 	GetDlgItem(IDC_EDIT_VERT_X)->GetWindowText(csX);
// 	GetDlgItem(IDC_EDIT_VERT_Y)->GetWindowText(csY);
// 	GetDlgItem(IDC_EDIT_VERT_Z)->GetWindowText(csZ);
// 
// 	float fX = _ttof(csX);
// 	float fY = _ttof(csY);
// 	float fZ = _ttof(csZ);
// 
// 	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
// 	if( pMain->m_bEditLine == true )
// 		pMain->SetVertexPos( D3DXVECTOR3(fX, fY, fZ) );
}



void CLineBaseEditor::OnBnClickedButtonLineindex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"추후 업데이트 예정", L"알림!", MB_OK);
}


void CLineBaseEditor::OnBnClickedRadioLineStartposition()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	GetDlgItem(ID_EL_MAKEFIRST)->EnableWindow(FALSE);
	GetDlgItem(ID_EL_DIVIDE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_EL_DIVIDE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EL_FILLET)->EnableWindow(FALSE);
	GetDlgItem(ID_EL_WELD)->EnableWindow(FALSE);
	GetDlgItem(IDC_EL_SNAP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_EL_FILLET)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_EL_FILLET)->EnableWindow(FALSE);
	GetDlgItem(IDC_EL_JOIN)->EnableWindow(FALSE);
	GetDlgItem(ID_EL_COPY)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_EL_COPY)->EnableWindow(FALSE);		

	// 	GetDlgItem(IDC_EDIT_VERT_X)->EnableWindow(FALSE);
	// 	GetDlgItem(IDC_EDIT_VERT_Y)->EnableWindow(FALSE);
	// 	GetDlgItem(IDC_EDIT_VERT_Z)->EnableWindow(FALSE);
	// 	GetDlgItem(IDC_BUTTON_POS)->EnableWindow(FALSE);	

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->m_bEditLineVertex = false;
	pMain->m_bEditStartPosition = true;

	CKTDGLineMap *pLinemap = g_pX2Game->GetWorld()->GetLineMap();
	for(int i=0; i<pLinemap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLinemap->AccessLineData(i);
		pLineData->m_bOver = false;
		pLineData->m_bSelected = false;
	}

	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();		
	pMainFrame->m_wndProperties.UpdateProp(2);
}


void CLineBaseEditor::OnLvnEndlabeleditListLine(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	if( pDispInfo->item.pszText == NULL )
		return;

	int iOldItem = pDispInfo->item.iItem;
	wstring wstrNewItem =  pDispInfo->item.pszText;
	int iNewItem = _ttoi(wstrNewItem.c_str());
	
	if( iOldItem == iNewItem )
		return;

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	if( pMain->m_bEditLine == true )
		pMain->SwapLine(iOldItem, iNewItem);

	UpdateLineInfo();
}


void CLineBaseEditor::OnLvnItemchangedListLine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	if( m_bCheckList == true )
	{
		m_bCheckList = false;
		return;
	}

	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;	
	if( pMain != NULL )
	{
		if( (pNMLV->uNewState == 2 || pNMLV->uNewState == 3) && (pNMLV->uOldState == 0 || pNMLV->uOldState == 1) )
		{
			// 선택
			int iNewSelect = pNMLV->iItem;
			pMain->SelectLine(iNewSelect, true);
			pMain->UpdateLineProperty();
		}
		else if( pNMLV->uNewState == 0 && pNMLV->uOldState == 2 )
		{
			// 선택 해제
			int iNewSelect = pNMLV->iItem;
			pMain->SelectLine(iNewSelect, false);
			pMain->UpdateLineProperty();
		}
	}
}

void CLineBaseEditor::SetCheck(int iItem, bool bCheck)
{
	if( bCheck == true)
	{
		m_LineList.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED );
		m_LineList.EnsureVisible(iItem, TRUE);
		m_LineList.SetFocus();
	}
	else
	{
		m_LineList.SetItemState(iItem, 0, LVIS_SELECTED );
	}

	m_bCheckList = true;
}

void CLineBaseEditor::OnBnClickedButtonRearrange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CX2ToolMain *pMain = (CX2ToolMain*)g_pMain;
	pMain->ReArrangeLine();
}
