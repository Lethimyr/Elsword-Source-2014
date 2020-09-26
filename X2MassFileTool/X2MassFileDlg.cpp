// X2MassFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\X2MassFiledlg.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CX2MassFileDlg dialog

CX2MassFileDlg::CX2MassFileDlg(CWnd* pParent /*=NULL*/)
: CDialog(CX2MassFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CX2MassFileDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_pMassFile	= new CMassFile();
	m_bZip		= true;

	m_KomFilePath = _T("");
}

CX2MassFileDlg::~CX2MassFileDlg()
{
	if( m_pMassFile != NULL )
	{
		delete m_pMassFile;
		m_pMassFile	= NULL;
	}
}

void CX2MassFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CX2MassFileDlg)
	DDX_Control(pDX, IDC_FILELIST, c_FileList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CX2MassFileDlg, CDialog)
	//{{AFX_MSG_MAP(CX2MassFileDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_FILELIST, OnBegindragFilelist)
	ON_NOTIFY(NM_RCLICK, IDC_FILELIST, OnRclickFilelist)
	ON_COMMAND(IDC_SELECT_ALL, OnSelectAll)
	ON_COMMAND(IDC_INVERT_SELECTION, OnInvertSelection)
	ON_COMMAND(IDC_REMOVE_FROM_LIST, OnRemoveFromList)
	ON_COMMAND(IDC_CLEAR_LIST, OnClearList)
	ON_WM_DROPFILES()
	ON_COMMAND(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_CHECK_ZIP, OnBnClickedCheckZip)
	ON_BN_CLICKED(IDC_BUT_SAVE, OnBnClickedButSave)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUT_RELEASE, OnBnClickedButRelease)
	ON_COMMAND(IDC_RELEASE, OnRelease)
	ON_COMMAND(IDC_REMOVE_KEY_DEL, OnRemoveKeyDel)
	ON_BN_CLICKED(IDC_BUT_FILEOPEN, OnBnClickedButFileopen)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CX2MassFileDlg message handlers

// We need to call TranslateAccelerator() ourselves - dialogs don't do this
// automagically.
BOOL CX2MassFileDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( NULL != m_hAccel && 
		pMsg->message >= WM_KEYFIRST  &&  pMsg->message <= WM_KEYLAST )
	{
		return TranslateAccelerator ( m_hWnd, m_hAccel, pMsg );
	}
	else
		return CDialog::PreTranslateMessage(pMsg);
}


bool CX2MassFileDlg::LoadConfig()
{
	int iArgc = __argc;
	WCHAR** pcArgv = __wargv;

	if ( iArgc > 1 )
	{
		char pcBuf[MAX_PATH] = {0};
		WideCharToMultiByte( CP_ACP, 0, pcArgv[1], -1, pcBuf, MAX_PATH, NULL, NULL );
		m_strOutFilename = pcBuf;
	}

	std::fstream fin;
	fin.open( "X2MConfig.ini", std::ios::in );

	if ( fin.good() == false )
	{		
		return false;
	}

	while ( true )
	{
		if ( fin.eof() == true )
			break;

		std::string strTag;
		std::string strData;
		fin >> strTag;
		
		char acBuf[MAX_PATH] = {0};
		fin.getline(acBuf, MAX_PATH);
		strData = &acBuf[1];

		if ( strTag == "Path" )
		{
			WCHAR wpcBuf[MAX_PATH] = { 0 };
			MultiByteToWideChar( CP_ACP, 0, strData.c_str(), -1, wpcBuf, MAX_PATH );

			CheckList( wpcBuf );
		}
		else if ( strTag == "OutFile" )
		{
			m_strOutFilename = strData;
		}
	}

	fin.close();

	OnSelectAll();
	OnSave();

	return true;
}


BOOL CX2MassFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog.
	HICON hLgIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	HICON hSmIcon = (HICON) LoadImage ( AfxGetResourceHandle(), 
		MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR );

	SetIcon(hLgIcon, TRUE);         // Set big icon
	SetIcon(hSmIcon, FALSE);        // Set small icon

	// Remove Size & Maximize from the system menu.

	pSysMenu->DeleteMenu ( SC_MAXIMIZE, MF_BYCOMMAND );
	pSysMenu->DeleteMenu ( SC_SIZE, MF_BYCOMMAND );

	// Get a handle to the system small icon image list, and set that as the
	// image list for our list control

	SHFILEINFO rInfo;
	HIMAGELIST hImglist;

	hImglist = (HIMAGELIST) SHGetFileInfo ( _T(""), 0, &rInfo, sizeof(SHFILEINFO), 
		SHGFI_ICON | SHGFI_SMALLICON | 
		SHGFI_SYSICONINDEX );

	m_imglist.Attach ( hImglist );

	// Make sure the list control has the share image list style, so it won't
	// destroy the system image list when the dialog closes.

	c_FileList.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	c_FileList.SetImageList ( &m_imglist, LVSIL_SMALL );

	// Set up the list columns.

	c_FileList.InsertColumn ( 0, _T("Filename"), LVCFMT_LEFT, 0, 0 );
	c_FileList.InsertColumn ( 1, _T("Type"), LVCFMT_LEFT, 0, 1 );
	c_FileList.InsertColumn ( 2, _T("Size"), LVCFMT_LEFT, 0, 2 );

	c_FileList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	c_FileList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	c_FileList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	// Enable tooltips for items that aren't completely visible.

	c_FileList.SetExtendedStyle ( LVS_EX_INFOTIP );

	// Position the dlg at the bottom-right of the desktop.
	CRect  rcDesktop;
	CRect  rcDlg;
	CPoint pt;

	SystemParametersInfo ( SPI_GETWORKAREA, 0, &rcDesktop, 0 );
	GetWindowRect ( &rcDlg );

	pt.x = rcDesktop.right/2 - rcDlg.Width()/2;
	pt.y = rcDesktop.bottom/2 - rcDlg.Height()/2;

	SetWindowPos ( NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

	// Load the accelerator table.
	m_hAccel = LoadAccelerators ( AfxGetResourceHandle(),
		MAKEINTRESOURCE(IDR_ACCEL) );

	// control init
	this->SendDlgItemMessage( IDC_CHECK_ZIP, BM_SETCHECK, BST_CHECKED, 0 );

	// 설정 파일이 있을 경우, 종료
	if ( LoadConfig() == true )
	{
		PostQuitMessage( 0 );
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CX2MassFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CX2MassFileDlg::OnDestroy() 
{
	// Detach our CImageList from the system image list.

	if ( NULL != m_imglist.GetSafeHandle() )
		m_imglist.Detach();

	CDialog::OnDestroy();
}

void CX2MassFileDlg::OnBegindragFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW*    pNMLV = (NMLISTVIEW*) pNMHDR;
	COleDataSource datasrc;
	HGLOBAL        hgDrop;
	DROPFILES*     pDrop;
	CStringList    lsDraggedFiles;
	POSITION       pos;
	int            nSelItem;
	CString        sFile;
	UINT           uBuffSize = 0;
	TCHAR*         pszBuff;
	FORMATETC      etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	bool			bDeleteCheck = false;

	*pResult = 0;   // return value ignored

	// For every selected item in the list, put the filename into lsDraggedFiles.

	pos = c_FileList.GetFirstSelectedItemPosition();

	while( NULL != pos )
	{
		nSelItem = c_FileList.GetNextSelectedItem ( pos );
		sFile = c_FileList.GetItemText ( nSelItem, 0 );

		if( sFile.Find( L"C:\\" ) < 0 )
		{
			if( sFile.Find( L"D:\\" ) < 0 )
			{
				if( m_KomFilePath != "" )
				{
					CString csTemp = _T("");
					csTemp = Decompress( sFile );
					if( csTemp != "" )
					{
						sFile = _T("");
						sFile = csTemp;
						bDeleteCheck = true;
					}
				}
			}
		}

		lsDraggedFiles.AddTail ( sFile );

		uBuffSize += lstrlen ( sFile ) + 1;
	}

	// Add 1 extra for the final null char, and the size of the DROPFILES struct.

	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

	// Allocate memory from the heap for the DROPFILES struct.
	hgDrop = GlobalAlloc ( GHND | GMEM_SHARE, uBuffSize );

	if ( NULL == hgDrop )
		return;

	pDrop = (DROPFILES*) GlobalLock ( hgDrop );

	if ( NULL == pDrop )
	{
		GlobalFree ( hgDrop );
		return;
	}

	// Fill in the DROPFILES struct.

	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.

	pDrop->fWide = TRUE;
#endif;

	// Copy all the filenames into memory after the end of the DROPFILES struct.
	pos = lsDraggedFiles.GetHeadPosition();
	pszBuff = (TCHAR*) (LPBYTE(pDrop) + sizeof(DROPFILES));

	while ( NULL != pos )
	{
		lstrcpy ( pszBuff, (LPCTSTR) lsDraggedFiles.GetNext ( pos ) );
		pszBuff = 1 + _tcschr ( pszBuff, '\0' );
	}

	GlobalUnlock ( hgDrop );

	// Put the data in the data source.

	datasrc.CacheGlobalData ( CF_HDROP, hgDrop, &etc );

	// Add in our own custom data, so we know that the drag originated from our 
	// window.  CMyDropTarget::DragEnter() checks for this custom format, and
	// doesn't allow the drop if it's present.  This is how we prevent the user
	// from dragging and then dropping in our own window.
	// The data will just be a dummy bool.
	HGLOBAL hgBool;

	hgBool = GlobalAlloc ( GHND | GMEM_SHARE, sizeof(bool) );

	if ( NULL == hgBool )
	{
		GlobalFree ( hgDrop );
		return;
	}

	// Put the data in the data source.

	etc.cfFormat = g_uCustomClipbrdFormat;

	datasrc.CacheGlobalData ( g_uCustomClipbrdFormat, hgBool, &etc );


	// Start the drag 'n' drop!

	DROPEFFECT dwEffect = DROPEFFECT_MOVE;
	if( bDeleteCheck )
	{
		dwEffect = datasrc.DoDragDrop ( DROPEFFECT_MOVE );
	}
	else
	{
		dwEffect = datasrc.DoDragDrop ( DROPEFFECT_COPY );// | DROPEFFECT_MOVE );
	}

	// If the DnD completed OK, we remove all of the dragged items from our
	// list.

	switch ( dwEffect )
	{
	case DROPEFFECT_COPY:
		break;
	case DROPEFFECT_MOVE:
		{
			// Note: Don't call GlobalFree() because the data will be freed by the drop target.

			if( bDeleteCheck == false )
			{
				for ( nSelItem = c_FileList.GetNextItem ( -1, LVNI_SELECTED );
					nSelItem != -1;
					nSelItem = c_FileList.GetNextItem ( nSelItem, LVNI_SELECTED ) )
				{
					c_FileList.DeleteItem ( nSelItem );
					nSelItem--;
				}

				// Resize the list columns.

				c_FileList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
				c_FileList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
				c_FileList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
			}
		}
		break;

	case DROPEFFECT_NONE:
		{
			// This needs special handling, because on NT, DROPEFFECT_NONE
			// is returned for move operations, instead of DROPEFFECT_MOVE.
			// See Q182219 for the details.
			// So if we're on NT, we check each selected item, and if the
			// file no longer exists, it was moved successfully and we can
			// remove it from the list.

			if ( g_bNT )
			{
				bool bDeletedAnything = true; /* false; */
				for( nSelItem = c_FileList.GetNextItem ( -1, LVNI_SELECTED );
					nSelItem != -1;
					nSelItem = c_FileList.GetNextItem ( nSelItem, LVNI_SELECTED ) )
				{
					CString sFilename = c_FileList.GetItemText ( nSelItem, 0 );

					if ( 0xFFFFFFFF == GetFileAttributes ( sFile ) &&
						GetLastError() == ERROR_FILE_NOT_FOUND )
					{
						// We couldn't read the file's attributes, and GetLastError()
						// says the file doesn't exist, so remove the corresponding 
						// item from the list.

						c_FileList.DeleteItem ( nSelItem );

						nSelItem--;
						bDeletedAnything = true;
					}
				}

				// Resize the list columns if we deleted any items.

				if ( bDeletedAnything )
				{
					c_FileList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
					c_FileList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
					c_FileList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

					// Note: Don't call GlobalFree() because the data belongs to 
					// the caller.
				}
				else
				{
					// The DnD operation wasn't accepted, or was canceled, so we 
					// should call GlobalFree() to clean up.

					GlobalFree ( hgDrop );
					GlobalFree ( hgBool );
				}
			}   // end if (NT)
			else
			{
				// We're on 9x, and a return of DROPEFFECT_NONE always means
				// that the DnD operation was aborted.  We need to free the
				// allocated memory.

				GlobalFree ( hgDrop );
				GlobalFree ( hgBool );
			}
		}
		break;  // end case DROPEFFECT_NONE
	}   // end switch
}

void CX2MassFileDlg::OnRclickFilelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint ptCursorPos;
	CMenu  menu;

	// We'll show the menu at the current cursor position.

	GetCursorPos ( &ptCursorPos );

	VERIFY ( menu.LoadMenu ( IDR_LIST_CONTEXTMENU ));
	ASSERT ( NULL != menu.GetSubMenu(0) );  // check that there's a popup menu at index 0.

	// Show the menu!

	menu.GetSubMenu(0)->TrackPopupMenu ( TPM_RIGHTBUTTON, ptCursorPos.x,
		ptCursorPos.y, this );

	*pResult = 0;   // return value ignored
}

void CX2MassFileDlg::OnDropFiles(HDROP hdrop) 
{
	UINT				uNumFiles;
	TCHAR				szNextFile [MAX_PATH];
	LVFINDINFO			rlvFind = { LVFI_STRING, szNextFile };

	char				strTemp[MAX_PATH] = "";
	TCHAR				tstrTemp[MAX_PATH] = _T("");
	bool				bKomCheck = false;

	uNumFiles = DragQueryFile( hdrop, -1, NULL, 0 );

	for( UINT uFile = 0; uFile < uNumFiles; ++uFile )
	{
		if( DragQueryFile( hdrop, uFile, szNextFile, MAX_PATH ) > 0 )
		{
			if( CheckList( szNextFile ) == OS_KOM )
				break;
		}
	}   // end for

	// Resize columns so all text is visible.
	c_FileList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	c_FileList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
	c_FileList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

	int nIndex = c_FileList.GetItemCount();
	c_FileList.EnsureVisible ( nIndex-1, FALSE );

	DragFinish( hdrop );

	// 외부 윈도우에서 파일을 가져오면서 잊어버렸던 포커스를 찾아온다.
	HWND hWnd = ::GetForegroundWindow();
	HWND hWnd2 = this->m_hWnd;
	::PostMessage( hWnd, WM_KILLFOCUS, (WPARAM)hWnd, 0);
	::PostMessage( hWnd2, WM_SETFOCUS, (WPARAM)hWnd2, 0);
	::SetForegroundWindow(hWnd2);
}

void CX2MassFileDlg::AddListItem( UINT nIndex, const WCHAR* wstrFileName )
{
	SHFILEINFO			rFileInfo;
	LVITEM				rItem;
	HANDLE				hFind;
	WIN32_FIND_DATA		rFind;
	WCHAR				szFileLen[64];

	SHGetFileInfo ( wstrFileName, 0, &rFileInfo, sizeof(rFileInfo),
		SHGFI_SYSICONINDEX | SHGFI_ATTRIBUTES |
		SHGFI_TYPENAME );

	ZeroMemory ( &rItem, sizeof(LVITEM) );

	rItem.mask    = LVIF_IMAGE | LVIF_TEXT;
	rItem.iItem   = nIndex;

	WCHAR wstrTemp[MAX_PATH] = L"";
	wcscpy( wstrTemp, wstrFileName );
	rItem.pszText = wstrTemp;

	rItem.iImage  = rFileInfo.iIcon;

	if ( rFileInfo.dwAttributes & SFGAO_GHOSTED )
	{
		rItem.mask |= LVIF_STATE;
		rItem.state = rItem.stateMask = LVIS_CUT;
	}

	c_FileList.InsertItem ( &rItem );

	c_FileList.SetItemText ( nIndex, 1, rFileInfo.szTypeName );

	hFind = FindFirstFile ( wstrFileName, &rFind );

	if ( INVALID_HANDLE_VALUE != hFind )
	{
		StrFormatByteSize ( rFind.nFileSizeLow, szFileLen, 64 );
		FindClose ( hFind );
	}

	c_FileList.SetItemText ( nIndex, 2, szFileLen );
}

CX2MassFileDlg::OPEN_STAGE CX2MassFileDlg::CheckList( WCHAR* wstrFileName )
{
	int				nIndex = c_FileList.GetItemCount();
	LVFINDINFO		rlvFind = { LVFI_STRING, wstrFileName };

	char			strTemp[MAX_PATH]	= "";
	TCHAR			tstrTemp[MAX_PATH]	= _T("");

	OPEN_STAGE		openStage = OS_NONE;

	WCHAR drive[_MAX_DRIVE]	= L"";
	WCHAR dir[_MAX_DIR]		= L"";
	WCHAR fname[_MAX_FNAME]	= L"";
	WCHAR ext[_MAX_EXT]		= L"";

	_wsplitpath( wstrFileName, drive, dir, fname, ext);

	if( wcscmp( ext, L".kom" ) == 0 )
		openStage = OS_KOM;
	else if( wcscmp( ext, L"" ) == 0 )
		openStage = OS_FOLDER;
	else
		openStage = OS_FILE;

	//kom file
	switch( openStage )
	{
	case OS_KOM:
		{
			m_KomFilePath	= L"";
			m_KomFilePath	= wstrFileName;

			WideCharToMultiByte( CP_ACP, 0, wstrFileName, -1, strTemp, MAX_PATH, NULL, NULL );
			m_pMassFile->LoadMassFile( strTemp );

			CMassFile::MASSFILE_MEMBERFILEINFO Info;
			ZeroMemory( &Info, sizeof( Info ) );

			for( int e = 0; e < c_FileList.GetItemCount(); ++e )
			{
				delete (void*)c_FileList.GetItemData( e );
			}
			c_FileList.DeleteAllItems();

			CString	Temp;
			for( UINT i = 0; i < m_pMassFile->GetFileCount(); ++i )
			{
				m_pMassFile->GetMemberFile(i,&Info);

				MultiByteToWideChar( CP_ACP, 0, Info.strFileName, -1, tstrTemp, MAX_PATH );
				c_FileList.InsertItem( i, tstrTemp );
				c_FileList.SetItemData( i, (DWORD)new CString( tstrTemp ) );

				Temp.Format( L"%d KB", Info.size/1024 );

				c_FileList.SetItemText( i, 1, L"kom(압축)" );
				c_FileList.SetItem( i, 2, LVIF_TEXT, Temp, 0, 0, 0, 0 );
			}

			UpdateData( FALSE );

			return OS_KOM;
		}
		break;

	case OS_FOLDER:
		{
			if( -1 != c_FileList.FindItem( &rlvFind, -1 ) )
				return OS_NONE;

			std::vector<std::wstring> vecFile;

			FindFloderFile( wstrFileName, vecFile );

			for( int i = 0; i < (int)vecFile.size(); ++i )
			{
				AddListItem( nIndex, vecFile[i].c_str() );

				++nIndex;
			}

			return OS_FOLDER;
		}
		break;

	case OS_FILE:
		{
			if( -1 != c_FileList.FindItem( &rlvFind, -1 ) )
				return OS_NONE;

			AddListItem( nIndex, wstrFileName );

			return OS_FILE;
		}
		break;
	}

	return OS_NONE;
}


/////////////////////////////////////////////////////////////////////////////
// CX2MassFileDlg command handlers

void CX2MassFileDlg::OnSelectAll() 
{
	int  nIndex, nMaxIndex = c_FileList.GetItemCount() - 1;

	for ( nIndex = 0; nIndex <= nMaxIndex; nIndex++ )
	{
		c_FileList.SetItemState ( nIndex, LVIS_SELECTED, LVIS_SELECTED );
	}
}

void CX2MassFileDlg::OnInvertSelection() 
{
	int  nIndex, nMaxIndex = c_FileList.GetItemCount() - 1;
	UINT uItemState;

	for ( nIndex = 0; nIndex <= nMaxIndex; nIndex++ )
	{
		uItemState = c_FileList.GetItemState ( nIndex, LVIS_SELECTED );

		c_FileList.SetItemState ( nIndex, uItemState ^ LVIS_SELECTED, 
			LVIS_SELECTED );
	}
}

void CX2MassFileDlg::OnRemoveFromList() 
{
	int nSelItem;

	for ( nSelItem = c_FileList.GetNextItem ( -1, LVNI_SELECTED );
		nSelItem != -1;
		nSelItem = c_FileList.GetNextItem ( nSelItem, LVNI_SELECTED ) )
	{
		c_FileList.DeleteItem ( nSelItem );
		nSelItem--;
	}
}

void CX2MassFileDlg::OnClearList() 
{
	m_KomFilePath = _T("");

	if( m_pMassFile != NULL )
	{
		delete m_pMassFile;
		m_pMassFile	= NULL;
	}
	m_pMassFile	= new CMassFile();

	c_FileList.DeleteAllItems();
}

void CX2MassFileDlg::OnSave()
{
	UpdateData();

	int selectNum = c_FileList.GetSelectedCount();
	if( selectNum == 0 )
	{
		ERRORMSG( L"먼저 파일을 선택해야함.!" );
		return;
	}

	// 파일 리스트 추가
	char	*strFileName;
	WCHAR	wstrTemp[MAX_PATH]	= L"";
	std::vector<char*>	vecFileName;	

	POSITION pos = c_FileList.GetFirstSelectedItemPosition();
	while( pos )
	{
		strFileName = new char[MAX_PATH];

		int nItem; 

		nItem = c_FileList.GetNextSelectedItem( pos );
		c_FileList.GetItemText( nItem, 0, wstrTemp, MAX_PATH );
		WideCharToMultiByte( CP_ACP, 0, wstrTemp, -1, strFileName, MAX_PATH, NULL, NULL );
		vecFileName.push_back( strFileName );
	}

	// 아웃 파일 이름이 지정되어있지 않으면, 다이얼로그 처리
	char strTemp[MAX_PATH]	=  "";

	if ( m_strOutFilename == "" )
	{
		CString strFilter = _T("KOG Mass Files (*.kom)|*.kom|");
		CFileDialog dlg(FALSE,L"kom",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

		if( dlg.DoModal() != IDOK )
			return;

		WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)dlg.GetPathName(), -1, strTemp, MAX_PATH, NULL, NULL );
	}
	else
	{
		strcpy( strTemp, m_strOutFilename.c_str() );
	}
	
	m_pMassFile->ModifyMassFile( m_pMassFile, strTemp, &vecFileName );

	for( int i = 0; i < (int)vecFileName.size(); ++i )
	{
		delete [] vecFileName[i];
	}
	vecFileName.clear();

	// 아웃 파일 이름이 지정되어있지 않으면, 메세지 창 띄움
	// ( 콘솔 모드일 경우 예외 처리 )
	if ( m_strOutFilename == "" )
	{
		INFORMSG( L"압축완료" );
	}
}

void CX2MassFileDlg::OnBnClickedCheckZip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bZip = !m_bZip;
}

void CX2MassFileDlg::OnBnClickedButSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnSave();
}

void CX2MassFileDlg::OnBnClickedButRelease()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int selectNum = c_FileList.GetSelectedCount();
	if( (m_KomFilePath == "") && (selectNum == 0) )
	{
		ERRORMSG( L"먼저 파일을 선택해 주세요" );
		return;
	}

	CString* strSelected;
	if( selectNum != 0 )
	{
		strSelected = new CString [selectNum];
		POSITION pos = c_FileList.GetFirstSelectedItemPosition();
		int i = 0;
		while (pos)
		{
			int nItem; 
			nItem = c_FileList.GetNextSelectedItem(pos);
			strSelected[i] = *(CString*)( c_FileList.GetItemData( nItem ) );
			i++;
		}
	}

	ITEMIDLIST*		pidlBrowse;
	TCHAR           pszPathname[MAX_PATH];
	BROWSEINFO		BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset( &BrInfo, 0, sizeof(BrInfo) );
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("압축을 풀 디렉토리를 선택하세요");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 다이얼로그를 띄우기
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);    

	if( pidlBrowse == NULL)
		return;

	// 패스를 얻어옴
	::SHGetPathFromIDList( pidlBrowse, pszPathname );

	//CMassFile을 이용해 파일을 읽어냄
	char  strTemp[MAX_PATH] = "";
	TCHAR tstrTemp[MAX_PATH] = _T("");

	WideCharToMultiByte( CP_ACP, 0, m_KomFilePath, -1, strTemp, MAX_PATH, NULL, NULL );
	m_pMassFile->LoadMassFile(strTemp);
	UINT FileNo = m_pMassFile->GetFileCount();
	CMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
	memset(&FileInfo,0,sizeof(FileInfo));
	FileInfo.dwFlag = MFI_REALDATA;

	CString OutFileName;

	if( c_FileList.GetSelectedCount() != 0 )
	{
		for( int i = 0; i < selectNum; i++ )
		{
			WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)strSelected[i], -1, strTemp, MAX_PATH, NULL, NULL );
			m_pMassFile->GetMemberFile( strTemp,&FileInfo);

			MultiByteToWideChar( CP_ACP, 0, FileInfo.strFileName, -1, tstrTemp, MAX_PATH );
			OutFileName.Format( _T("%s\\%s"), pszPathname, tstrTemp );

			WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)OutFileName, -1, strTemp, MAX_PATH, NULL, NULL );
			FILE* file = fopen( strTemp, "wb" );
			if( file == NULL )
			{
				ERRORMSG( L"압축해제 실패" );
				return;
			}		
			fwrite(FileInfo.pRealData,1,FileInfo.size,file);
			fclose(file);

			FileInfo.pRealData = NULL;
		}

		INFORMSG( L"압축해제 완료" );

		return;
	}
	for(int i = 0; i < (int)FileNo; i++)
	{
		m_pMassFile->GetMemberFile( i, &FileInfo);

		MultiByteToWideChar( CP_ACP, 0, FileInfo.strFileName, -1, tstrTemp, MAX_PATH );
		OutFileName.Format( _T("%s\\%s"), pszPathname, tstrTemp );

		WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)OutFileName, -1, strTemp, MAX_PATH, NULL, NULL );
		FILE* file = fopen( strTemp, "wb" );
		if(file == NULL)
		{
			ERRORMSG( L"압축해제 실패" );
			return;
		}		
		fwrite(FileInfo.pRealData,1,FileInfo.size,file);
		fclose(file);

		FileInfo.pRealData = NULL;
	}

	INFORMSG( L"압축해제 완료" );
}

void CX2MassFileDlg::OnRelease()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnBnClickedButRelease();
}

CString CX2MassFileDlg::Decompress( CString& csComFile )
{
	CMassFile::MASSFILE_MEMBERFILEINFO FileInfo;
	memset(&FileInfo,0,sizeof(FileInfo));
	FileInfo.dwFlag = MFI_REALDATA;

	CString OutFileName;
	char strTemp[MAX_PATH] = "";
	TCHAR tstrTemp[MAX_PATH] = _T("");

	//m_pMassFile->GetMemberFile( (LPCTSTR)csMassFile, (LPCTSTR)csComFile, &FileInfo );
	WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)csComFile, -1, strTemp, MAX_PATH, NULL, NULL );
	m_pMassFile->GetMemberFile( strTemp, &FileInfo);

	TCHAR dir[MAX_PATH] = _T("");
	GetCurrentDirectory( MAX_PATH, dir );
	MultiByteToWideChar( CP_ACP, 0, FileInfo.strFileName, -1, tstrTemp, MAX_PATH );
	OutFileName.Format( _T("%s\\%s"), dir, tstrTemp );

	WideCharToMultiByte( CP_ACP, 0, (LPCTSTR)OutFileName, -1, strTemp, MAX_PATH, NULL, NULL );
	FILE* file = fopen( strTemp, "wb" );

	if( file == NULL )
	{
		ERRORMSG( L"압축해제 실패(CX2MassFileDlg::Decompress())" );
		return L"";
	}		

	fwrite( FileInfo.pRealData, 1, FileInfo.size, file );
	fclose(file);

	FileInfo.pRealData = NULL;

	return OutFileName;
}

void CX2MassFileDlg::FindFloderFile( WCHAR* wstrFloder, std::vector<std::wstring>& vecFloder )
{
	HANDLE				hSearch;
	WIN32_FIND_DATA		fd;
	WCHAR				wstrSearchPath[256];


	wcscpy( wstrSearchPath, wstrFloder);
	wcscat( wstrSearchPath, L"\\*.*");

	hSearch = FindFirstFile( wstrSearchPath, &fd );

	if(hSearch == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if( wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..") )
		{
			if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				WCHAR	wstrNewSearchDir[256];

				wcscpy( wstrNewSearchDir, wstrFloder );
				wcscat( wstrNewSearchDir, L"\\");
				wcscat( wstrNewSearchDir, fd.cFileName );

				FindFloderFile( wstrNewSearchDir, vecFloder );
			}
			else
			{
				WCHAR			wstrNewSearchFile[256];
				std::wstring	wstrFile;

				wcscpy( wstrNewSearchFile, wstrFloder );
				wcscat( wstrNewSearchFile, L"\\");
				wcscat( wstrNewSearchFile, fd.cFileName );

				wstrFile = wstrNewSearchFile;

				vecFloder.push_back( wstrFile );
			}
		}

	}while( FindNextFile( hSearch, &fd ) );

	FindClose(hSearch);
}

void CX2MassFileDlg::OnRemoveKeyDel()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnRemoveFromList();
}

void CX2MassFileDlg::OnBnClickedButFileopen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OPENFILENAME	OFN;
	WCHAR			lpstrFile[50000] = L"";
	WCHAR			wcDirectory[MAX_PATH] = L"";
	WCHAR			wcFileName[MAX_PATH] = L"";
	WCHAR			wcAddItem[MAX_PATH] = L"";
	WCHAR			*p;
	int				i = 1;

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize		= sizeof(OPENFILENAME);
	OFN.hwndOwner		= this->m_hWnd;
	OFN.lpstrFilter		= L"모든 파일(*.*)\0*.*\0";
	OFN.lpstrFile		= lpstrFile;
	OFN.nMaxFile		= 10000;
	OFN.Flags			= OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	if( GetOpenFileName(&OFN) != 0 ) 
	{
		p = lpstrFile;
		wcscpy( wcDirectory, p );
		wcscpy( wcFileName, wcDirectory );

		if( *(p + wcslen( wcFileName ) + 1) == 0 )
		{
			//AddListItem( nIndex, wcFileName );
			CheckList( wcFileName );
		}
		else
		{
			for (;;) 
			{
				p = p + wcslen( wcFileName ) + 1;
				if( *p == 0 )
				{
					break;
				}

				wcscpy( wcFileName, p );
				wcscpy( wcAddItem, wcDirectory );
				wcscat( wcAddItem, L"\\" );
				wcscat( wcAddItem, wcFileName );

				//AddListItem( nIndex, wcAddItem );
				if( CheckList( wcAddItem ) == OS_KOM )
					break;
			}
		}

		c_FileList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
		c_FileList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
		c_FileList.SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );

		int	nIndex = c_FileList.GetItemCount();
		c_FileList.EnsureVisible ( nIndex-1, FALSE );
	}
	else 
	{
		if( CommDlgExtendedError() == FNERR_BUFFERTOOSMALL )
		{
			ERRORMSG( L"버퍼 크기가 너무 작습니다" );
		}
	}
}
