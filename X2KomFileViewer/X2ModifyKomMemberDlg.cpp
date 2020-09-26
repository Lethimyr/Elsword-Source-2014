// X2ModifyKomMemberDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
//#include "X2OldKomFileViewer.h"
#include "X2ModifyKomMemberDlg.h"
#include ".\x2modifykommemberdlg.h"
#include "NoticeDlg.h"


// CX2ModifyKomMemberDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CX2ModifyKomMemberDlg, CDialog)
CX2ModifyKomMemberDlg::CX2ModifyKomMemberDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX2ModifyKomMemberDlg::IDD, pParent)
{
}

CX2ModifyKomMemberDlg::~CX2ModifyKomMemberDlg()
{
}

void CX2ModifyKomMemberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAVE_DIR_EDIT, m_cSaveDirEdit);
	//DDX_Control(pDX, IDC_MODIFY_FILE_EDIT, m_cModifyFileEdit);
	DDX_Control(pDX, IDC_MODIFY_LIST, m_cModifyListBox);
	DDX_Control(pDX, IDC_CHECK_C, m_cLuaCompileCheck);
	DDX_Control(pDX, IDC_CHECK_E, m_cScriptEncryptCheck);
}

BOOL CX2ModifyKomMemberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 리스트 컨트롤 초기화
	InitListControl();

	// 체크박스 초기화
	m_cLuaCompileCheck.SetCheck( TRUE );
	m_cScriptEncryptCheck.SetCheck( TRUE );

	return TRUE;
}

void CX2ModifyKomMemberDlg::InitListControl()
{
	// kom member file list
	m_cModifyListBox.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );


	m_cModifyListBox.InsertColumn ( 0, _T("KomNum"), LVCFMT_LEFT, 0, 0 );
	m_cModifyListBox.InsertColumn ( 1, _T("Type"), LVCFMT_LEFT, 0, 1 );
	m_cModifyListBox.InsertColumn ( 2, _T("File name"), LVCFMT_LEFT, 0, 2 );
	m_cModifyListBox.InsertColumn ( 3, _T("Full Path"), LVCFMT_LEFT, 0, 3 );	

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	/*m_cModifyListBox.InsertColumn ( 0, _T("KomNum"), LVCFMT_LEFT, 0, 0 );
	m_cModifyListBox.InsertColumn ( 1, _T("File name"), LVCFMT_LEFT, 0, 1 );
	m_cModifyListBox.InsertColumn ( 2, _T("Full Path"), LVCFMT_LEFT, 0, 2 );	

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );*/

	m_cModifyListBox.SetExtendedStyle( LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT );
}


BEGIN_MESSAGE_MAP(CX2ModifyKomMemberDlg, CDialog)
	ON_BN_CLICKED(IDFINDSAVEPATH, OnBnClickedFindsavepath)
	ON_BN_CLICKED(IDFINDMODIFYFILE, OnBnClickedFindmodifyfile)
	ON_BN_CLICKED(IDMODIFYBEGIN, OnBnClickedModifybegin)
	ON_BN_CLICKED(IDDELMODIFYFILE, OnBnClickedDelmodifyfile)
END_MESSAGE_MAP()


// CX2ModifyKomMemberDlg 메시지 처리기입니다.

int CALLBACK BrowseCallbackProcMKMDlg( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
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

bool CX2ModifyKomMemberDlg::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner	  = hParent;
	bi.pidlRoot		  = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle	  = szTitle;
	bi.ulFlags		  = 0;
	bi.lpfn			  = BrowseCallbackProcMKMDlg;
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

void CX2ModifyKomMemberDlg::OnBnClickedFindsavepath()	// 결과물을 저장할 폴더 선택
{
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( BrowseFolder( this->m_hWnd, TEXT("재압축된 결과물을 저장할 폴더를 선택하세요."), StartPath, folder ) == false )
		return;

	m_cSaveDirEdit.SetWindowText( folder );
	m_wstrKomSavePath = folder;
}

void CX2ModifyKomMemberDlg::OnBnClickedFindmodifyfile() // 수정될 리소스 찾기
{
	CFileDialog Files( TRUE, L"*.*", NULL, OFN_ENABLESIZING, L"All Files (*.*)|*.*|" );
	if( Files.DoModal() == IDCANCEL ) return;

	std::wstring wstrFileFullPath = Files.GetPathName().GetBuffer();
	std::wstring wstrFileName = Files.GetFileName().GetBuffer();

	//////////////////////////////////////////////////////////////////////////	
	std::wstring wstrKomString;
	std::wstring wstrType;
	int iIndex;
	if( theApp.m_komFileManager.MemberFileFind( wstrFileName, true, wstrKomString, iIndex ) == false )	// 추가 작업 이라면
	{
		if( MessageBox( L"Kom파일의 멤버파일중 이름이 일치하는 파일이 없습니다!\n새로 추가 하시겠습니까?", NULL, MB_YESNO ) == IDNO )
			return;
		else
		{
			// to do: listbox 출력
			//CX2OldKomFileViewer oldKomFileViewrDlg;		// DLG 클래스
		
			//if ( oldKomFileViewrDlg.DoModal() == IDOK )
			//{
			//	wstrKomString	= oldKomFileViewrDlg.GetSelectedItemText();
			//	wstrType		= L"추가";
			//}
			//else
			//	return;
			
		}
	}
	else		// 교체 작업 이라면
	{
		wstrType		= L"교체";
	}

	if( m_mapModifyData.find( wstrFileName ) != m_mapModifyData.end() )
	{
		MessageBox( L"이미 등록된 파일입니다." );
		return;
	}

	ModifyData modifyData;
	modifyData.m_wstrFileFullPath = wstrFileFullPath;
	modifyData.m_wstrKomString	  = wstrKomString;

	m_mapModifyData.insert( std::make_pair( wstrFileName, modifyData ) );

	//////////////////////////////////////////////////////////////////////////

	int iCount = m_cModifyListBox.GetItemCount();
	m_cModifyListBox.InsertItem( iCount, wstrKomString.c_str() );
	m_cModifyListBox.SetItemText( iCount, 1, wstrType.c_str() );
	m_cModifyListBox.SetItemText( iCount, 2, wstrFileName.c_str() );
	m_cModifyListBox.SetItemText( iCount, 3, wstrFileFullPath.c_str() );

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );

	/*m_cModifyListBox.SetItemText( iCount, 1, wstrFileName.c_str() );
	m_cModifyListBox.SetItemText( iCount, 2, wstrFileFullPath.c_str() );

	m_cModifyListBox.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_cModifyListBox.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );*/
	
}

void CX2ModifyKomMemberDlg::OnBnClickedDelmodifyfile()			// 리스트에서 제거
{
	int iIndex = m_cModifyListBox.GetSelectionMark();
	CString cstrFileName = m_cModifyListBox.GetItemText( iIndex, 0 );
	m_cModifyListBox.DeleteItem( iIndex );
	m_mapModifyData.erase( std::wstring( cstrFileName.GetBuffer() ) );
}

static HWND g_repackagingMBHwnd;

UINT WINAPI RepackagingMessageBoxThread( LPVOID arg )
{
	CNoticeDlg noticeDlg;
	std::wstring wstrMessage = L"리소스 교체중...";
	noticeDlg.ChangeText( wstrMessage );
	g_repackagingMBHwnd = noticeDlg.m_hWnd;
	noticeDlg.DoModal();
	return 0;
}

void CX2ModifyKomMemberDlg::OnBnClickedModifybegin()
{
	if( m_wstrKomSavePath.empty() )
	{
		MessageBox( L"결과물을 저장할 경로를 설정해주세요!" );
		return;
	}

	if( m_mapModifyData.empty() )
	{
		MessageBox( L"선택하신 리소스가 없습니다!" );
		return;
	}

	// 상황알림 쓰레드 실행
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, RepackagingMessageBoxThread, (LPVOID)NULL, NULL, NULL );

	bool bLuaScriptCompile = ( m_cLuaCompileCheck.GetCheck() == TRUE );
	bool bScriptEncrypt = ( m_cScriptEncryptCheck.GetCheck() == TRUE );
	if( theApp.m_komFileManager.RepackagingKomFile( m_wstrKomSavePath, m_mapModifyData, bLuaScriptCompile, bScriptEncrypt ) )
	{
		::SendMessage( g_repackagingMBHwnd, WM_CLOSE, 0, 0 );
		TerminateThread( hNoticeThread, 0 );
		MessageBox( L"리소스 갈아끼우기 성공!" );
		return;
	}

	::SendMessage( g_repackagingMBHwnd, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );
}


