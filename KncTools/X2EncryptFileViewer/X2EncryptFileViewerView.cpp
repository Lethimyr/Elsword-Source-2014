// X2EncryptFileViewerView.cpp : CX2EncryptFileViewerView 클래스의 구현
//

#include "stdafx.h"
#include "X2EncryptFileViewer.h"

#include "X2EncryptFileViewerDoc.h"
#include "X2EncryptFileViewerView.h"
#include "FileCrypt.h"
#include <fstream>
#include <string>
#include ".\x2encryptfileviewerview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2EncryptFileViewerView

IMPLEMENT_DYNCREATE(CX2EncryptFileViewerView, CEditView)

BEGIN_MESSAGE_MAP(CX2EncryptFileViewerView, CEditView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_DECRYPT_SAVE, OnDecryptSave)
END_MESSAGE_MAP()

// CX2EncryptFileViewerView 생성/소멸

CX2EncryptFileViewerView::CX2EncryptFileViewerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_cBufPointer = NULL;
}

CX2EncryptFileViewerView::~CX2EncryptFileViewerView()
{
	if( m_cBufPointer != NULL )
		delete []m_cBufPointer;
}

void CX2EncryptFileViewerView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	DragAcceptFiles();
}

BOOL CX2EncryptFileViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// 자동 줄바꿈을 사용합니다.

	return bPreCreated;
}


// CX2EncryptFileViewerView 인쇄

BOOL CX2EncryptFileViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 CEditView 준비
	return CEditView::OnPreparePrinting(pInfo);
}

void CX2EncryptFileViewerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 기본 CEditView 시작 인쇄
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CX2EncryptFileViewerView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 기본 CEditView 종료 인쇄
	CEditView::OnEndPrinting(pDC, pInfo);
}

void CX2EncryptFileViewerView::OnDropFiles( HDROP hDropInfo )
{
	int nFiles;
	char szPathName[MAX_PATH];
	CString strFileName;

	// 드롭된 파일의 갯수
	nFiles = ::DragQueryFileA( hDropInfo, 0xFFFFFFFF, szPathName, MAX_PATH );

	for(int i = nFiles-1 ; i >= 0; i--)
	{
		// 파일의 경로 얻어옴
		::DragQueryFileA( hDropInfo, i, szPathName, MAX_PATH );		
	}

	::DragFinish( hDropInfo );

	//////////////////////////////////////////////////////////////////////////

	std::ifstream ifs;
	ifs.open( szPathName, std::ios::binary );
	if( ifs.is_open() == false )
	{
		AfxMessageBox( "파일 열기 실패!" );
		return;
	}

	std::string strBuf;
	char cReadCur;
	while( !ifs.eof() )
	{
		ifs.get( cReadCur );
		strBuf.append( 1, cReadCur );
	}	

	if( m_cBufPointer != NULL )
		delete []m_cBufPointer;

	m_cBufPointer = CFileCrypt::FileDecrypt( theApp.GetEncryptionKeyList(), strBuf.c_str(), strBuf.size() );

	//std::wstring wstrBuf;
	//ConvertCharToWCHAR( wstrBuf, m_cBufPointer );

	GetEditCtrl().SetWindowText( m_cBufPointer );

	//////////////////////////////////////////////////////////////////////////	

	CEditView::OnDropFiles( hDropInfo );
}

// CX2EncryptFileViewerView 메시지 처리기
void CX2EncryptFileViewerView::OnDecryptSave()
{
	// 저장 경로 얻기
	CHAR StartPath[128];
	CHAR folder[128];

	// 폴더 선택
	if( BrowseFolder( this->m_hWnd, TEXT("저장할 폴더를 선택하세요"), StartPath, folder ) == false )
		return;

	std::ofstream ofs;	
	std::string strEncryptedFileName = StartPath;
	strEncryptedFileName += "\\test.lua";

	//std::string strTemp;
	//ConvertWCHARToChar( strTemp, strEncryptedFileName.c_str() );

	ofs.open( strEncryptedFileName.c_str(), std::ios::binary );

	if( ofs.is_open() == false || m_cBufPointer == NULL )
		return;

	ofs.write( m_cBufPointer, strlen( m_cBufPointer ) );

	ofs.close();

	MessageBox( "저장성공" );
}

int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	switch( uMsg )
	{
	case BFFM_INITIALIZED:
		if( lpData != NULL )
			::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData );
		break;
	}
	return 0;
}

bool CX2EncryptFileViewerView::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner	  = hParent;
	bi.pidlRoot		  = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle	  = szTitle;
	bi.ulFlags		  = 0;
	bi.lpfn			  = BrowseCallbackProc;
	bi.lParam		  = (LPARAM)StartPath;

	pidl = SHBrowseForFolder( &bi );

	if( pidl == NULL )
		return false;

	SHGetPathFromIDList( pidl, szFolder );

	if( SHGetMalloc( &pMalloc ) != NOERROR )
		return false;

	pMalloc->Free( pidl );
	pMalloc->Release();
	return true;
}


// CX2EncryptFileViewerView 진단

#ifdef _DEBUG
void CX2EncryptFileViewerView::AssertValid() const
{
	CEditView::AssertValid();
}

void CX2EncryptFileViewerView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CX2EncryptFileViewerDoc* CX2EncryptFileViewerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CX2EncryptFileViewerDoc)));
	return (CX2EncryptFileViewerDoc*)m_pDocument;
}
#endif //_DEBUG



