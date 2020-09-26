// X2KomFileViewerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "X2KomFileViewerDlg.h"
#include ".\x2komfileviewerdlg.h"
#include "UncompressDlg.h"
#include "X2ModifyKomMemberDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2KomFileViewerDlg 대화 상자



CX2KomFileViewerDlg::CX2KomFileViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX2KomFileViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIsKomParsingOnly = false;
}

void CX2KomFileViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_memberList);
	DDX_Control(pDX, IDC_INSERTED_LIST, m_insertedListCtrl);
	DDX_Control(pDX, IDC_DELETED_LIST, m_deletedListCtrl);	
	DDX_Control(pDX, IDC_MEMBER_COUNT, m_memberCount);
	DDX_Control(pDX, IDC_MEMBER_COUNT2, m_komSizeEdit);
	DDX_Control(pDX, IDC_KOM_LIST, m_komList);
	DDX_Control(pDX, IDC_FILE_FIND_EDIT, m_editFileFind);
	DDX_Control(pDX, IDC_MT_CHECK, m_checkMT);
}

BEGIN_MESSAGE_MAP(CX2KomFileViewerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, OnNMCustomdrawList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST, OnLvnItemActivateList)
	ON_BN_CLICKED(IDC_CREATE_KOM, OnBnClickedCreateKom)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_KOM_LIST, OnLvnItemActivateKomList)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_FIND_FILE, OnBnClickedFindFile)
	ON_BN_CLICKED(IDC_UNCOMPRESS, OnBnClickedUncompress)
	ON_BN_CLICKED(IDC_MODIFY_KOM, OnBnClickedModifyKom)
END_MESSAGE_MAP()


// CX2KomFileViewerDlg 메시지 처리기

BOOL CX2KomFileViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	// 프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// 리스트 박스 초기화
	InitListControl();

	// 리스트 박스 아이템 추가
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( theApp.m_komFileManager.GetMapNewMemberFile().begin()->first, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else	// 구버전 폴더 패스만 지정했을때
	{
		InsertKomList( theApp.m_komFileManager.GetMapOldMemberFile().begin()->first, theApp.m_komFileManager.GetMapOldMemberFile() );
	}	

	m_komList.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );

	m_memberList.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_memberList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );

	m_insertedListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_insertedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	m_deletedListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_deletedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	
	//ShowWindow(SW_SHOWMAXIMIZED);
	return TRUE;  // 컨트롤에 대한 포커스를 설정하지 않을 경우 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면 
// 아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CX2KomFileViewerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다. 
HCURSOR CX2KomFileViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CX2KomFileViewerDlg::InitListControl()
{
	SHFILEINFO rInfo;
	HIMAGELIST hImglist;

	hImglist = (HIMAGELIST) SHGetFileInfo ( _T(""), 0, &rInfo, sizeof(SHFILEINFO), 
		SHGFI_ICON | SHGFI_SMALLICON | 
		SHGFI_SYSICONINDEX );

	m_imglist.Attach ( hImglist );

	// kom member file list
	m_komList.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_komList.InsertColumn ( 0, _T("Kom Name"), LVCFMT_LEFT, 0, 0 );
	m_komList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_komList.SetExtendedStyle ( LVS_EX_INFOTIP );

	// kom member file list
	m_memberList.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_memberList.InsertColumn ( 0, _T("KomNum"), LVCFMT_LEFT, 0, 0 );
	m_memberList.InsertColumn ( 1, _T("Filename"), LVCFMT_LEFT, 0, 1 );
	m_memberList.InsertColumn ( 2, _T("Type"), LVCFMT_LEFT, 0, 2 );
	m_memberList.InsertColumn ( 3, _T("Size"), LVCFMT_LEFT, 0, 3 );

	m_memberList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );
	
	m_memberList.SetExtendedStyle ( LVS_EX_INFOTIP );

	// kom member file list
	m_insertedListCtrl.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_insertedListCtrl.InsertColumn ( 0, _T("Filename"), LVCFMT_LEFT, 0, 0 );
	m_insertedListCtrl.InsertColumn ( 1, _T("Type"), LVCFMT_LEFT, 0, 1 );
	m_insertedListCtrl.InsertColumn ( 2, _T("Size"), LVCFMT_LEFT, 0, 2 );

	m_insertedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );	

	m_insertedListCtrl.SetExtendedStyle ( LVS_EX_INFOTIP );

	// kom member file list
	m_deletedListCtrl.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );
	
	m_deletedListCtrl.InsertColumn ( 0, _T("Filename"), LVCFMT_LEFT, 0, 0 );
	m_deletedListCtrl.InsertColumn ( 1, _T("Type"), LVCFMT_LEFT, 0, 1 );
	m_deletedListCtrl.InsertColumn ( 2, _T("Size"), LVCFMT_LEFT, 0, 2 );

	m_deletedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );	

	m_deletedListCtrl.SetExtendedStyle ( LVS_EX_INFOTIP );
}

void CX2KomFileViewerDlg::InsertKomList( const std::wstring& wstrKomName, const std::map< std::wstring, std::vector< MemberFileInfo > >& mapMemberFile )
{
	m_komList.DeleteAllItems();
	m_memberList.DeleteAllItems();

	// 현재 멤버 파일 리스트 검사	
	if( mapMemberFile.empty() == true )
		return;

	if( theApp.m_komFileManager.IsCompareKom() == true )			// 구폴더, 신폴더 둘다 열렸을 때
	{
		// 구버전, 신버전 둘다 열었을 때 구버전에 없는 추가된 파일 리스트 출력
        InsertInsertedFileList( theApp.m_komFileManager.GetInsertedFile() );

		// 구버전 폴더에는 있는데 신버전 폴더에는 없는 삭제된 파일 리스트 출력
		std::vector< MemberFileInfo >& vecDeletedFile = theApp.m_komFileManager.GetDeletedFile();
		InsertDeletedFileList( vecDeletedFile );
		
		std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mit = mapMemberFile.begin();
		for( ; mit != mapMemberFile.end(); mit++ )
		{
			m_komList.InsertItem( m_komList.GetItemCount(), mit->first.c_str() );

			// member list
			if( mit->first == wstrKomName )		// data0 중
			{
				std::vector< MemberFileInfo >::const_iterator vit = mit->second.begin();
				for( ; vit != mit->second.end(); vit++ )		// 신버전 폴더
				{
					const MemberFileInfo& memberFile = *vit;
					CString Temp;
					int iCount = m_memberList.GetItemCount();
					m_memberList.InsertItem( iCount, mit->first.c_str() );
					m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
					m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
					Temp.Format( L"%d KB", memberFile.m_iFileSize );
					m_memberList.SetItemText( iCount, 2, L"kom(압축)" );
					m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
				}

				for( vit = vecDeletedFile.begin(); vit != vecDeletedFile.end(); vit++ )	// 구버전 폴더엔 있고 신버전 폴더엔 없는 지워야 하는 것들(data0?)
				{
					const MemberFileInfo& memberFile = *vit;
					if( memberFile.m_wstrKomString == mit->first )
					{
						CString Temp;
						int iCount = m_memberList.GetItemCount();
						m_memberList.InsertItem( iCount, mit->first.c_str() );
						m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
						m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
						Temp.Format( L"%d KB", memberFile.m_iFileSize );
						m_memberList.SetItemText( iCount, 2, L"kom(압축)" );
						m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
					}
				}
			}
		}

		// 삭제되었는데 kom파일이 없는것일때
		std::vector< MemberFileInfo >::const_iterator vitDel;
		for( vitDel = vecDeletedFile.begin(); vitDel != vecDeletedFile.end(); vitDel++ )	// data0 외의 것들?
		{
			const MemberFileInfo& memberFile = *vitDel;
			std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mit = mapMemberFile.find( memberFile.m_wstrKomString );
			if( mit == mapMemberFile.end() )
			{
				CString Temp;
				int iCount = m_memberList.GetItemCount();
				m_memberList.InsertItem( iCount, memberFile.m_wstrKomString.c_str() );
				m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
				m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
				Temp.Format( L"%d KB", memberFile.m_iFileSize );
				m_memberList.SetItemText( iCount, 2, L"kom(압축)" );
				m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
			}
		}

		CString temp;
		temp.Format( L"%d", m_memberList.GetItemCount() );
		m_memberCount.SetWindowText( temp );
	}
	else				// 구폴더 or 신폴더 중 하나만 열렸을 때
	{
		// kom list
		std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mit = mapMemberFile.begin();
		for( ; mit != mapMemberFile.end(); mit++ )
		{			
			m_komList.InsertItem( m_komList.GetItemCount(), mit->first.c_str() );	// Kom File List

			// member list
			if( mit->first == wstrKomName )		// data0 만??
			{
				InsertMemberList( mit->first, mit->second );
			}
		}

		CString temp;
		temp.Format( L"%d", m_memberList.GetItemCount() );
		m_memberCount.SetWindowText( temp );
	}
}

void CX2KomFileViewerDlg::InsertMemberList( const std::wstring& wstrKomName, const std::vector< MemberFileInfo >& vecMemberFileList )
{
	m_memberList.DeleteAllItems();

	std::vector< MemberFileInfo >::const_iterator vit = vecMemberFileList.begin();
	for( ; vit != vecMemberFileList.end(); vit++ )
	{
		const MemberFileInfo& memberFile = *vit;
		CString Temp;
		int iCount = m_memberList.GetItemCount();
		m_memberList.InsertItem( iCount, wstrKomName.c_str() );
		m_memberList.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
		m_memberList.SetItemText( iCount, 1, memberFile.m_wstrFileName.c_str() );
		Temp.Format( L"%d KB", memberFile.m_iFileSize );
		m_memberList.SetItemText( iCount, 2, L"kom(압축)" );
		m_memberList.SetItem( iCount, 3, LVIF_TEXT, Temp, 0, 0, 0, 0 );
	}
}

void CX2KomFileViewerDlg::InsertInsertedFileList( const std::vector< MemberFileInfo >& vecMemberFileList )
{
	m_insertedListCtrl.DeleteAllItems();

	std::vector< MemberFileInfo >::const_iterator vit = vecMemberFileList.begin();
	for( ; vit != vecMemberFileList.end(); vit++ )
	{
		const MemberFileInfo& memberFile = *vit;
		CString Temp;
		int iCount = m_insertedListCtrl.GetItemCount();
		m_insertedListCtrl.InsertItem( iCount, memberFile.m_wstrFileName.c_str() );
		m_insertedListCtrl.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
		Temp.Format( L"%d KB", memberFile.m_iFileSize );
		m_insertedListCtrl.SetItemText( iCount, 1, L"kom(압축)" );
		m_insertedListCtrl.SetItem( iCount, 2, LVIF_TEXT, Temp, 0, 0, 0, 0 );
	}
}

void CX2KomFileViewerDlg::InsertDeletedFileList( std::vector< MemberFileInfo >& vecMemberFileList )
{
	m_deletedListCtrl.DeleteAllItems();


	WCHAR temp[100] = L"Motion_Arme_VioletMage.x";
	std::wstring a = temp;
	std::vector< MemberFileInfo >::iterator vit2 = std::find_if( vecMemberFileList.begin(), vecMemberFileList.end(), MemberFileInfo::FIND_IF( a ) );
	if ( vit2 != vecMemberFileList.end() )
	{
		MemberFileInfo i = (*vit2);
		int k = 100;
	}

	std::vector< MemberFileInfo >::const_iterator vit = vecMemberFileList.begin();
	for( vit = vecMemberFileList.begin(); vit != vecMemberFileList.end(); vit++ )
	{
		const MemberFileInfo& memberFile = *vit;
		CString Temp;
		int iCount = m_deletedListCtrl.GetItemCount();
		m_deletedListCtrl.InsertItem( iCount, memberFile.m_wstrFileName.c_str() );
		m_deletedListCtrl.SetItemData( iCount, (DWORD)memberFile.m_iFileState );
		Temp.Format( L"%d KB", memberFile.m_iFileSize );
		m_deletedListCtrl.SetItemText( iCount, 1, L"kom(압축)" );
		m_deletedListCtrl.SetItem( iCount, 2, LVIF_TEXT, Temp, 0, 0, 0, 0 );
	}
}

void CX2KomFileViewerDlg::OnNMCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{	
	NMLVCUSTOMDRAW* pNMCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT /*| CDDS_SUBITEM*/) == pNMCD->nmcd.dwDrawStage )
	{
		DWORD dwFileState = m_memberList.GetItemData( pNMCD->nmcd.dwItemSpec );
		switch( dwFileState )
		{
		case FS_INSERTED:
			pNMCD->clrText = RGB( 0, 0, 255 );
			break;

		case FS_CHANGED:
			pNMCD->clrText = RGB( 0, 255, 0 );
			break;

		case FS_DELETED:
			pNMCD->clrText = RGB( 255, 0, 0 );
			break;

		default:
			pNMCD->clrText = RGB( 0, 0, 0 );
			break;
		}

		*pResult = CDRF_NOTIFYITEMDRAW;
	}
}

void CX2KomFileViewerDlg::OnLvnItemActivateList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;
}

void CX2KomFileViewerDlg::OnBnClickedCreateKom()	// 압축하기
{
	// 저장 경로 얻기
	WCHAR StartPath[128];
	WCHAR folder[128];

	if( m_bIsKomParsingOnly )		
	{
		MessageBox( L"압축할수 있는 모드가 아닙니다!" );	// 구버전 폴더만 열었을 때
		return;
	}

	if( BrowseFolder( this->m_hWnd, TEXT("구버전 폴더를 선택하세요"), StartPath, folder ) == false )
		return;

	// Is Multi Thread ?
	bool bIsMultiThreadPacking = ( m_checkMT.GetCheck() == TRUE ? true : false );

	if( theApp.m_komFileManager.CreateKomFile( std::wstring( folder ), bIsMultiThreadPacking ) == false )
	{
		MessageBox( L"kom파일 생성 실패!" );
	}
	else
	{
		MessageBox( L"kom파일 생성 성공!" );
	}
}

int CALLBACK BrowseCallbackProc2( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
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

bool CX2KomFileViewerDlg::BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder )
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner	  = hParent;
	bi.pidlRoot		  = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle	  = szTitle;
	bi.ulFlags		  = 0;
	bi.lpfn			  = BrowseCallbackProc2;
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
void CX2KomFileViewerDlg::OnLvnItemActivateKomList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString temp = m_komList.GetItemText( m_komList.GetSelectionMark(), 0 );
	std::wstring wstrKomName = temp.GetBuffer();
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( wstrKomName, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else
	{
		InsertKomList( wstrKomName, theApp.m_komFileManager.GetMapOldMemberFile() );        
	}	

	int iKomSize = theApp.m_komFileManager.GetKomSize( std::wstring( temp.GetBuffer() ) );
	WCHAR buf[256] = L"";
	wsprintf( buf, L"%d mb", iKomSize/1024 );
	m_komSizeEdit.SetWindowText( buf );
	
	*pResult = 0;
}

#include "PathDialog.h"
#include "NoticeDlg.h"

static HWND g_noticeHwnd2;

// notice thread
UINT WINAPI NoticeThread2( LPVOID arg )
{
	CNoticeDlg noticeDlg;	
	g_noticeHwnd2 = noticeDlg.m_hWnd;	
	noticeDlg.DoModal();
	return 0;
}

void CX2KomFileViewerDlg::OnBnClickedButton1()
{
	// 경로 새로 얻기
	CPathDialog pathDlg;
	if( pathDlg.DoModal() != IDOK )
		return;

	// 얻은 경로로 새로 kom파일 파싱하기
	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == true )
	{
		::MessageBox( NULL, L"경로를 입력하지 않았습니다!", L"오류!", MB_OK );
		return;
	}

	// 상황알림 쓰레드 실행
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, NoticeThread2, (LPVOID)NULL, NULL, NULL );

	// Kom 파일 파싱 & 멤버 파일 로드
	if( pathDlg.GetOldPath().empty() == true  &&  pathDlg.GetNewPath().empty() == false )
	{
		// new member file obtain
		if( theApp.m_komFileManager.ParsingMemberFile( pathDlg.GetNewPath(), std::map< std::wstring, MemberFileInfo >() ) == false )
		{
			::MessageBox( NULL, L"member파일 분석 실패!", L"오류!", MB_OK );
			return;
		}
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == false )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFileAndMemberFile( pathDlg.GetOldPath(), pathDlg.GetNewPath(), std::map< std::wstring, MemberFileInfo >() ) == false )
		{
			::MessageBox( NULL, L"member파일 분석 실패!", L"오류!", MB_OK );
			return;
		}
	}
	else if( pathDlg.GetOldPath().empty() == false  &&  pathDlg.GetNewPath().empty() == true )
	{
		// kom file parsing & new member file obtain
		if( theApp.m_komFileManager.ParsingKomFile( pathDlg.GetOldPath() ) == false )
		{
			::MessageBox( NULL, L"kom파일 분석 실패!", L"오류!", MB_OK );
			return;
		}
	}

	// 상황알림 대화상자 제거
	::SendMessage( g_noticeHwnd2, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );

	// 리스트 박스 아이템 추가
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( theApp.m_komFileManager.GetMapNewMemberFile().begin()->first, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else
	{
		InsertKomList( theApp.m_komFileManager.GetMapNewMemberFile().begin()->first, theApp.m_komFileManager.GetMapOldMemberFile() );
	}	

	m_komList.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );

	m_memberList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
	m_memberList.SetColumnWidth ( 3, LVSCW_AUTOSIZE_USEHEADER );

	m_insertedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_insertedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	m_deletedListCtrl.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	m_deletedListCtrl.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
}

void CX2KomFileViewerDlg::OnBnClickedFindFile()
{
	CString strText;
	m_editFileFind.GetWindowText( strText );

	int iFindIndex = 0;
	std::wstring wstrMemberFileName = strText.GetBuffer();
	std::wstring wstrKomString;
	if( theApp.m_komFileManager.MemberFileFind( wstrMemberFileName, m_bIsKomParsingOnly, wstrKomString, iFindIndex ) == false )
	{
		MessageBox( L"해당 파일을 찾을 수 없습니다!" );
		return;
	}

	// list 불러오기
	if( m_bIsKomParsingOnly == false )
	{
		InsertKomList( wstrKomString, theApp.m_komFileManager.GetMapNewMemberFile() );
	}
	else
	{
		InsertKomList( wstrKomString, theApp.m_komFileManager.GetMapOldMemberFile() );
	}

	// 찾은 파일 선택하기
	ListView_SetItemState (m_memberList.GetSafeHwnd(), iFindIndex, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
	m_memberList.EnsureVisible( iFindIndex, FALSE );
}

static HWND g_uncompressMBHwnd;

UINT WINAPI UncompressMessageBoxThread( LPVOID arg )
{
	CNoticeDlg noticeDlg;
	std::wstring wstrMessage = L"압축해제중입니다";
	noticeDlg.ChangeText( wstrMessage );
	g_uncompressMBHwnd = noticeDlg.m_hWnd;
	noticeDlg.DoModal();
	return 0;
}

void CX2KomFileViewerDlg::OnBnClickedUncompress() // 압축풀기
{
	// 저장 경로 얻기
	WCHAR StartPath[128];
	WCHAR folder[128];
	
	if( m_bIsKomParsingOnly == false )			// 구버전만 열린게 아니라면
	{
		MessageBox( L"압축을 풀수 있는 모드가 아닙니다!" );
		return;
	}

	// 압축 해제 모드 선택
	CUncompressDlg uncompressDlg;
	uncompressDlg.SetKomList( theApp.m_komFileManager.GetKomRealPathList() );
	if( uncompressDlg.DoModal() == IDCANCEL )
		return;

	// 폴더 선택
	if( BrowseFolder( this->m_hWnd, TEXT("압축해제할 폴더를 선택하세요"), StartPath, folder ) == false )
		return;
	
	std::wstring wstrCurKomName;
	std::vector< std::wstring > vecSeletedFileList;	
	switch( uncompressDlg.GetCompressMode() )
	{
	case UM_SEL_FILE:
		{
			// 선택된 리소스 파일 얻기
			if( m_memberList.GetSelectedCount() != 0 )
			{
				CString strCurKomName = m_memberList.GetItemText( 0, 0 );
				wstrCurKomName = strCurKomName.GetBuffer();

				POSITION pos = m_memberList.GetFirstSelectedItemPosition();
				while( pos )
				{
					int nItem;
					nItem = m_memberList.GetNextSelectedItem( pos );
					CString strTemp = m_memberList.GetItemText( nItem, 1 );
					std::wstring wstrTemp = strTemp.GetBuffer();
					vecSeletedFileList.push_back( wstrTemp );
				}
			}
		}
		break;

	case UM_SEL_KOM:
		{
			// 선택된 KOM파일 얻기
			vecSeletedFileList = uncompressDlg.GetSeletedFileList();
		}
		break;

	default:
		break;
	}
	
	// 상황알림 쓰레드 실행
	HANDLE hNoticeThread = (HANDLE)_beginthreadex( NULL, NULL, UncompressMessageBoxThread, (LPVOID)NULL, NULL, NULL );
	
	std::map< std::wstring, std::vector< std::wstring > > mapDummy;
	if( theApp.m_komFileManager.UncompressKomFile( std::wstring( folder ), vecSeletedFileList, wstrCurKomName, uncompressDlg.GetCompressMode(), false, mapDummy ) == false )
	{
		::SendMessage( g_uncompressMBHwnd, WM_CLOSE, 0, 0 );
		TerminateThread( hNoticeThread, 0 );

		MessageBox( L"압축 풀기 실패!" );
		return;
	}

	::SendMessage( g_uncompressMBHwnd, WM_CLOSE, 0, 0 );
	TerminateThread( hNoticeThread, 0 );

	MessageBox( L"압축 해제 완료!" );
}

void CX2KomFileViewerDlg::OnBnClickedModifyKom()	// 리소스 갈아끼우기
{
	CX2ModifyKomMemberDlg mkmDlg;
	mkmDlg.DoModal();
}
