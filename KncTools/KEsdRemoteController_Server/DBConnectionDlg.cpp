// DBConnectionDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "KEsdRemoteController_Server.h"
#include "DBConnectionDlg.h"
#include ".\dbconnectiondlg.h"


// DBConnectionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(DBConnectionDlg, CDialog)
DBConnectionDlg::DBConnectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DBConnectionDlg::IDD, pParent)
{
}

DBConnectionDlg::~DBConnectionDlg()
{
}

void DBConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_IDEditBox);
	DDX_Control(pDX, IDC_EDIT2, m_PWEditBox);
}


BEGIN_MESSAGE_MAP(DBConnectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// DBConnectionDlg 메시지 처리기입니다.

void DBConnectionDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_pVecDBConnIDPW == NULL )
	{
		::MessageBoxW( NULL, L"DB Connection String Option Error!", L"", 0 );
		OnOK();
		return;
	}

	CString cstrBuffer;
	m_IDEditBox.GetWindowText( cstrBuffer );
	m_pVecDBConnIDPW->push_back( KClearString( cstrBuffer.GetBuffer() ) );

	m_PWEditBox.GetWindowText( cstrBuffer );
	m_pVecDBConnIDPW->push_back( KClearString( cstrBuffer.GetBuffer() ) );

	// comment : this is fake!
	m_IDEditBox.GetWindowText( cstrBuffer );

	OnOK();
}
