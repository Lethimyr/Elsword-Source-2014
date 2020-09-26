// X2ItemToolView.cpp : CX2ItemToolView 클래스의 구현
//

#include "stdafx.h"
#include "X2ItemTool.h"

#include "X2ItemToolDoc.h"
#include "X2ItemToolView.h"
#include ".\x2itemtoolview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2ItemToolView

IMPLEMENT_DYNCREATE(CX2ItemToolView, CView)

BEGIN_MESSAGE_MAP(CX2ItemToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CX2ItemToolView 생성/소멸

CX2ItemToolView::CX2ItemToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CX2ItemToolView::~CX2ItemToolView()
{
}

BOOL CX2ItemToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CX2ItemToolView 그리기

void CX2ItemToolView::OnDraw(CDC* /*pDC*/)
{
	CX2ItemToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CX2ItemToolView 인쇄

BOOL CX2ItemToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CX2ItemToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CX2ItemToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CX2ItemToolView 진단

#ifdef _DEBUG
void CX2ItemToolView::AssertValid() const
{
	CView::AssertValid();
}

void CX2ItemToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CX2ItemToolDoc* CX2ItemToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CX2ItemToolDoc)));
	return (CX2ItemToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CX2ItemToolView 메시지 처리기

void CX2ItemToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_MainDlg.DestroyWindow();
}

int CX2ItemToolView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_MainDlg.Create( IDD_MAIN_DIALOG, this );

	return 0;
}

void CX2ItemToolView::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	//this->UpdateData();

	//CFile f;

	WCHAR strFilter[] = { L"lua Files (*.lua)|*.lua|All Files (*.*)|*.*||" };
	CFileDialog fileDlg( true, L".lua", NULL, 0, strFilter);

	if( fileDlg.DoModal() == IDOK )
	{
		CString fileName = fileDlg.GetFileName();
		LPWSTR wszFileName = fileName.GetBuffer( fileName.GetLength() );

		m_MainDlg.OpenLuaFile( wszFileName );
		GetDocument()->SetTitle( wszFileName );

		//g_pItemToolCommon->GetItemManager()->OpenScriptFile( wszFileName );
		

		//f.Open(FileDlg.GetFileName(), CFile::modeCreate | CFile::modeWrite);
		//CArchive ar(&f, CArchive::store);

		//ar << m_Make << m_Model << m_Year << m_Mileage << m_Owner;
		//ar.Close();
	}

	//f.Close();
}

void CX2ItemToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_MainDlg.MoveWindow( 0, 0, cx, cy, true );
	m_MainDlg.ResizeGridCtrl( cx, cy );
	m_MainDlg.Invalidate();
	//m_MainDlg.OnSize( nType, cx, cy );
}
