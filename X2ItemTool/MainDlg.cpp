// MainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2ItemTool.h"
#include "MainDlg.h"
#include ".\maindlg.h"


// CMainDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainDlg 메시지 처리기입니다.

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	g_pItemToolCommon = new CX2ItemToolCommon;


	//GridCtrl Setting
	GetDlgItem(IDC_GRID_CTRL)->GetWindowRect(&m_rcGrid);
	ScreenToClient(&m_rcGrid);
	m_GridCtrl.Create(m_rcGrid, this, IDC_GRID_CTRL, WS_CHILD | WS_BORDER| WS_TABSTOP | WS_VISIBLE);
	m_GridCtrl.SetEditable(TRUE);
	m_GridCtrl.EnableDragAndDrop(TRUE);
	m_GridCtrl.SetTextBkColor(RGB(255, 255, 225));
	m_GridCtrl.SetRowResize(TRUE);
	m_GridCtrl.SetColumnResize(TRUE);
	m_GridCtrl.SetListMode(FALSE);
	m_GridCtrl.SetSingleRowSelection(FALSE);
	m_GridCtrl.SetHeaderSort(FALSE);




	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMainDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SAFE_DELETE( g_pItemToolCommon );
}




void CMainDlg::OpenLuaFile( const WCHAR* wszFileName )
{
	g_pItemToolCommon->GetItemManager()->OpenScriptFile( wszFileName );


	CX2ItemManager* pItemManager = g_pItemToolCommon->GetItemManager();
	map<int, CX2Item::ItemTemplet*>& mapItemTemplet = pItemManager->GetItemTempletIDMap();
	map<int, CX2Item::ItemTemplet*>::iterator it;


	int nRow = (int)mapItemTemplet.size() + 1;
	m_GridCtrl.SetRowCount( nRow );
	m_GridCtrl.SetColumnCount( 50 );


	int iRow = 1;

	for( it=mapItemTemplet.begin(); it!=mapItemTemplet.end(); it++ )
	{
		CX2Item::ItemTemplet* pItemTemplet = (CX2Item::ItemTemplet*) it->second;
        
		for( int iCol=1; iCol<4; iCol++ )
		{
			//m_GridCtrl.SetCellType( iRow, iCol, RUNTIME_CLASS(CGridCell) );
			m_GridCtrl.SetItemText( iRow, iCol, pItemTemplet->m_Name.c_str() );
			//m_GridCtrl.SetItemState( iRow, iCol, m_GridCtrl.GetItemState(iRow,iCol) & ~GVIS_READONLY );
			m_GridCtrl.Invalidate();
		}

		iRow++;
	}
}


void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_GridCtrl.MoveWindow( 0, 0, cx, cy, true );
}

void CMainDlg::ResizeGridCtrl(int cx, int cy)
{
	m_GridCtrl.MoveWindow( 0, 0, cx, cy, true );
}