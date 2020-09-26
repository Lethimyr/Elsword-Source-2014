// DBConnectionIDPWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "DBConnectionIDPWDlg.h"
#include ".\dbconnectionidpwdlg.h"
#include "kncutil.h"


// CDBConnectionIDPWDlg dialog

IMPLEMENT_DYNAMIC(CDBConnectionIDPWDlg, CDialog)
CDBConnectionIDPWDlg::CDBConnectionIDPWDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBConnectionIDPWDlg::IDD, pParent)
{
}

CDBConnectionIDPWDlg::~CDBConnectionIDPWDlg()
{
}

void CDBConnectionIDPWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_DBConnID);
	DDX_Control(pDX, IDC_EDIT2, m_DBConnPW);
}


BEGIN_MESSAGE_MAP(CDBConnectionIDPWDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDBConnectionIDPWDlg message handlers

void CDBConnectionIDPWDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_DBConnID.GetWindowText( m_strID );
	m_DBConnPW.GetWindowText( m_strPW );

	OnOK();
}

void CDBConnectionIDPWDlg::GetStringID( std::string& strID )
{
	strID = KncUtil::toNarrowString( std::wstring( m_strID.GetBuffer() ) );
}

void CDBConnectionIDPWDlg::GetStringPW( std::string& strPW )
{
	strPW = KncUtil::toNarrowString( std::wstring( m_strPW.GetBuffer() ) );
}
