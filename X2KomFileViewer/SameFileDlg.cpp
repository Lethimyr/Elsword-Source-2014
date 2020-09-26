// SameFileDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "SameFileDlg.h"
#include ".\samefiledlg.h"


// CSameFileDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSameFileDlg, CDialog)
CSameFileDlg::CSameFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSameFileDlg::IDD, pParent)
{
}

CSameFileDlg::~CSameFileDlg()
{
}

void CSameFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAME_LIST, m_sameList);
}

BOOL CSameFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( m_mapSameFileList.empty() == true )
		return TRUE;

	m_sameList.ModifyStyle ( 0, LVS_SHAREIMAGELISTS );

	m_sameList.InsertColumn ( 0, _T("File Path"), LVCFMT_LEFT, 0, 0 );
	m_sameList.SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
	m_sameList.SetExtendedStyle ( LVS_EX_INFOTIP );

	std::map< std::wstring, std::vector< std::wstring > >::iterator mit = m_mapSameFileList.begin();
	for( ; mit != m_mapSameFileList.end(); mit++ )
	{
		if( mit->second.size() > 1 )
		{
			std::vector< std::wstring >::iterator vit = mit->second.begin();
			for( ; vit != mit->second.end(); vit++ )
			{
				int iCount = m_sameList.GetItemCount();
				m_sameList.InsertItem( iCount, vit->c_str() );
			}
		}
	}

	m_sameList.SetColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );

	return TRUE;
}


BEGIN_MESSAGE_MAP(CSameFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSameFileDlg 메시지 처리기입니다.

void CSameFileDlg::OnBnClickedOk()
{
	//std::string strDelCommand;
	//std::vector< std::wstring >::iterator vit = m_vecSameFileList.begin();
	//for( ; vit != m_vecSameFileList.end(); vit++ )
	//{
	//	std::string strDelFileName;
	//	ConvertWCHARToChar( strDelFileName, vit->c_str() );
	//	strDelCommand = "del ";
	//	strDelCommand += strDelFileName;
	//	system( strDelCommand.c_str() );
	//}
	
	OnOK();
}
