// WGameServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\DBConnectionDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


IMPLEMENT_DYNAMIC(CDBConnectionDlg, CDialog)
CDBConnectionDlg::CDBConnectionDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDBConnectionDlg::IDD, pParent)
{
	m_pEditList = NULL;
	m_bCheckOK = false;
}

CDBConnectionDlg::~CDBConnectionDlg()
{
}

void CDBConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDBConnectionDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDBConnectionDlg 메시지 처리기입니다.

void CDBConnectionDlg::OnBnClickedOk()
{
	if( m_pEditList != NULL )
	{
		// 초기화
		WCHAR editBuf[256];

		// ID에서 값얻기
		ZeroMemory( editBuf, 256 );
        GetDlgItemText( IDC_EDIT1, editBuf, 256 );
		m_pEditList->push_back( KClearString< std::wstring >( editBuf ) );

		// 흔적지우기
		//memset( editBuf, 5, wcslen( editBuf ) );
		//SetDlgItemText( IDC_EDIT1, editBuf );

		// PW에서 값얻기
		ZeroMemory( editBuf, 256 );
		GetDlgItemText( IDC_EDIT2, editBuf, 256 );
		m_pEditList->push_back( KClearString< std::wstring >( editBuf ) );

		// 흔적지우기
		//memset( editBuf, 5, wcslen( editBuf ) );
		//SetDlgItemText( IDC_EDIT2, editBuf );
	}
	else
	{
		MessageBoxW( L"m_pEditList is NULL Pointer!" );
	}

	m_bCheckOK = true;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CDBConnectionDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}