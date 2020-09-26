// PathDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "PathDialog.h"
#include ".\pathdialog.h"
#include "resource.h"


// CPathDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathDialog, CDialog)
CPathDialog::CPathDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPathDialog::IDD, pParent)
{
}

CPathDialog::~CPathDialog()
{
}

void CPathDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OLD_PATH_EDIT, m_oldPathEdit);
	DDX_Control(pDX, IDC_NEW_PATH_EDIT, m_newPathEdit);
	DDX_Control(pDX, IDC_COMPILE_CHECK, m_checkCompile);
	DDX_Control(pDX, IDC_COMPILE_CHECK2, m_checkCrypt);
	DDX_Control(pDX, IDC_COMPILE_CHECK3, m_checkXConvert);
}

BOOL CPathDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_checkCompile.SetCheck( TRUE );
	m_bDoCompile = true;

	m_checkCrypt.SetCheck( TRUE );
	m_bDoCrypt = true;

	//m_checkXConvert.SetCheck( TRUE );
	m_bDoXConvert = false;

	return TRUE;
}


BEGIN_MESSAGE_MAP(CPathDialog, CDialog)
	ON_BN_CLICKED(ID_OLD_PATH, OnBnClickedOldPath)
	ON_BN_CLICKED(ID_NEW_PATH, OnBnClickedNewPath)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_COMPILE_CHECK, OnBnClickedCompileCheck)
	ON_BN_CLICKED(IDC_COMPILE_CHECK2, OnBnClickedCryptCheck)
	ON_BN_CLICKED(IDC_COMPILE_CHECK3, OnBnClickedXFileConvertCheck)
END_MESSAGE_MAP()


// CPathDialog 메시지 처리기입니다.

void CPathDialog::OnBnClickedOldPath()
{
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( BrowseFolder( this->m_hWnd, TEXT("구버전 폴더를 선택하세요"), StartPath, folder ) == false )
		return;
	
	m_oldPathEdit.SetWindowText( folder );
	m_wstrOldPath = folder;
}

void CPathDialog::OnBnClickedNewPath()
{
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( BrowseFolder( this->m_hWnd, TEXT("신버전 폴더를 선택하세요"), StartPath, folder ) == false )
	return;

	m_newPathEdit.SetWindowText( folder );
	m_wstrNewPath = folder;
}
void CPathDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
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

bool CPathDialog::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
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
void CPathDialog::OnBnClickedCompileCheck()
{
	m_bDoCompile = ( m_checkCompile.GetCheck() == 0 ? false : true );
}

void CPathDialog::OnBnClickedCryptCheck()
{
	m_bDoCrypt = ( m_checkCrypt.GetCheck() == 0 ? false : true );
}

void CPathDialog::OnBnClickedXFileConvertCheck()
{
	MessageBox( L"아직 사용하실 수 없습니다!" );
	//m_bDoXConvert = ( m_checkXConvert.GetCheck() == 0 ? false : true );

	m_checkXConvert.SetCheck( FALSE );
}
