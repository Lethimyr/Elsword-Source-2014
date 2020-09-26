// KomListViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "KomListViewDlg.h"
#include ".\komlistviewdlg.h"
#include "resource.h"
#include <strsafe.h>
#include <stdlib.h>

// CKomListViewDlg dialog

IMPLEMENT_DYNAMIC(CKomListViewDlg, CDialog)
CKomListViewDlg::CKomListViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKomListViewDlg::IDD, pParent), m_pmapNewMemberFile( NULL )
{
}

CKomListViewDlg::CKomListViewDlg( std::map< std::wstring, std::vector< MemberFileInfo > >& mapNewMemberFile,
								 CWnd* pParent /*= NULL*/) : CDialog(CKomListViewDlg::IDD, pParent), m_pmapNewMemberFile( NULL )
{
	m_pmapNewMemberFile = &mapNewMemberFile;
}

CKomListViewDlg::~CKomListViewDlg()
{
}

void CKomListViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_KOM, m_lcKomList);
	DDX_Control(pDX, IDC_EDIT_FILE_COUNT, m_editFileCount);
	InitList();
	SetKomList();
}


BEGIN_MESSAGE_MAP(CKomListViewDlg, CDialog)
//	ON_WM_CREATE()
END_MESSAGE_MAP()


// CKomListViewDlg message handlers
int CKomListViewDlg::InitList()
{
	const int COLUMN_WIDTH = 600;
	
	m_COL.mask			= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	m_COL.fmt			= LVCFMT_LEFT;
	m_COL.cx			= COLUMN_WIDTH;
	m_COL.pszText		= L"Kom 파일명";
	m_COL.iSubItem		= 0;
	m_lcKomList.InsertColumn( 0, &m_COL );

	m_LI.mask			= LVIF_TEXT;
	m_LI.state			= LVIS_FOCUSED;
	m_LI.stateMask		= 0;

	ListView_SetExtendedListViewStyle(m_lcKomList, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	return 0;
}


int CKomListViewDlg::SetKomList()
{
	static const int TEXT_SIZE = 10;
	std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mItr = (*m_pmapNewMemberFile).begin();
	UINT		ulNumOfLists = 0;
	WCHAR	wstrMemberFileName[MAX_PATH] = {0, };
	WCHAR	strNumberOfFile[TEXT_SIZE] = {0, };

	while ( mItr != (*m_pmapNewMemberFile).end() )
	{
		m_LI.iItem		= ulNumOfLists;
		m_LI.iSubItem	= 0;
		StringCchCopyW( wstrMemberFileName, MAX_PATH, (mItr->first).c_str() );
		m_LI.pszText	= wstrMemberFileName;
		m_LI.cchTextMax	= MAX_PATH;
		m_lcKomList.InsertItem( &m_LI );

		mItr++;
		ulNumOfLists++;
	}

	_ultow( ulNumOfLists, strNumberOfFile, 10 );		// 10은 10진수
	m_editFileCount.SetWindowTextW( strNumberOfFile );

	return 0;
}