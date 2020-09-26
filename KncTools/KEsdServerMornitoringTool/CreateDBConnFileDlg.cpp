// CreateDBConnFileDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "CreateDBConnFileDlg.h"
#include ".\createdbconnfiledlg.h"


// CCreateDBConnFileDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCreateDBConnFileDlg, CDialog)
CCreateDBConnFileDlg::CCreateDBConnFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateDBConnFileDlg::IDD, pParent)
{
}

CCreateDBConnFileDlg::~CCreateDBConnFileDlg()
{
}

void CCreateDBConnFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editBoxFilePath);
}


BEGIN_MESSAGE_MAP(CCreateDBConnFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateDBConnFileDlg 메시지 처리기입니다.

void CCreateDBConnFileDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString cstrFilePath;
	m_editBoxFilePath.GetWindowText( cstrFilePath );
	if( theApp.m_kImp.CreateDBConnectionFile( std::wstring( cstrFilePath.GetBuffer() ) ) )
	{
		::MessageBox( NULL, L"DB Connection File 생성 성공!", L"확인!", NULL );
	}
	else
	{
		::MessageBox( NULL, L"DB Connection File 생성 실패!", L"에러!", NULL );
	}

	OnOK();
}
