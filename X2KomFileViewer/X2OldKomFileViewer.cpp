// X2OldKomFileViewer.cpp : implementation file
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "X2OldKomFileViewer.h"
#include ".\X2OldKomFileViewer.h"
#include "resource.h"
#include <strsafe.h>
#include <stdlib.h>

// CX2OldKomFileViewer dialog

IMPLEMENT_DYNAMIC(CX2OldKomFileViewer, CDialog)
CX2OldKomFileViewer::CX2OldKomFileViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CX2OldKomFileViewer::IDD, pParent)
{
}

CX2OldKomFileViewer::~CX2OldKomFileViewer()
{
}

void CX2OldKomFileViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OLD_KOM, m_lcOldKomList);
	InitList();
	SetOldKomList();
}


BEGIN_MESSAGE_MAP(CX2OldKomFileViewer, CDialog)
END_MESSAGE_MAP()


// CX2OldKomFileViewer message handlers
void CX2OldKomFileViewer::InitList()
{
	const int COLUMN_WIDTH = 300;

	m_COL.mask			= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	m_COL.fmt			= LVCFMT_LEFT;
	//m_COL.cx			= COLUMN_WIDTH;
	m_COL.pszText		= L"Kom 파일명";
	m_COL.iSubItem		= 0;
	m_lcOldKomList.InsertColumn( 0, &m_COL );

	//m_COL.cx			= COLUMN_WIDTH;
	m_COL.pszText		= L"파일경로";
	m_COL.iSubItem		= 1;
	m_lcOldKomList.InsertColumn( 1, &m_COL );

	m_lcOldKomList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_lcOldKomList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );

	m_LI.mask			= LVIF_TEXT;
	m_LI.state			= LVIS_FOCUSED;
	m_LI.stateMask		= 0;


	/*m_lcOldKomList.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_lcOldKomList.InsertColumn ( 0, _T("File name"), LVCFMT_LEFT, 0, 0 );
	m_lcOldKomList.InsertColumn ( 1, _T("Path"), LVCFMT_LEFT, 0, 1 );	

	m_lcOldKomList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_lcOldKomList.SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );*/

	ListView_SetExtendedListViewStyle(m_lcOldKomList, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
}

void CX2OldKomFileViewer::SetOldKomList()
{
	const std::map< std::wstring, std::vector< MemberFileInfo > >&			mapOldMemberFile = 
		theApp.m_komFileManager.GetMapOldMemberFile();
	std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mit =  
		mapOldMemberFile.begin();

	theApp.m_komFileManager.m_vecFolderList;
	theApp.m_komFileManager.m_vecKomNameList;

	UINT		ulNumOfLists = 0;
	WCHAR		wstrMemberFileName[MAX_PATH] = {0, };

	for ( UINT i = 0; i < theApp.m_komFileManager.m_vecKomNameList.size(); i++ )
	{
		if ( mapOldMemberFile.find( theApp.m_komFileManager.m_vecKomNameList[i] ) != mapOldMemberFile.end() )
		{
			//list에 집어 넣기
			m_LI.iItem		= ulNumOfLists;
			m_LI.iSubItem	= 0;
			StringCchCopyW( wstrMemberFileName, MAX_PATH, (theApp.m_komFileManager.m_vecKomNameList[i]).c_str() );
			m_LI.pszText	= wstrMemberFileName;
			m_LI.cchTextMax	= MAX_PATH;
			m_lcOldKomList.InsertItem( &m_LI );

			ulNumOfLists++;
		}
	}
	//static const int TEXT_SIZE = 10;
	//std::map< std::wstring, std::vector< MemberFileInfo > >::const_iterator mItr = (*m_pmapNewMemberFile).begin();
	//UINT		ulNumOfLists = 0;
	//WCHAR	wstrMemberFileName[MAX_PATH] = {0, };
	//WCHAR	strNumberOfFile[TEXT_SIZE] = {0, };

	//while ( mItr != (*m_pmapNewMemberFile).end() )
	//{
	//	m_LI.iItem		= ulNumOfLists;
	//	m_LI.iSubItem	= 0;
	//	StringCchCopyW( wstrMemberFileName, MAX_PATH, (mItr->first).c_str() );
	//	m_LI.pszText	= wstrMemberFileName;
	//	m_LI.cchTextMax	= MAX_PATH;
	//	m_lcKomList.InsertItem( &m_LI );

	//	mItr++;
	//	ulNumOfLists++;
	//}

	//_ultow( ulNumOfLists, strNumberOfFile, 10 );		// 10은 10진수
	//m_editFileCount.SetWindowTextW( strNumberOfFile );
}