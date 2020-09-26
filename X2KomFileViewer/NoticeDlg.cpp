#include "stdafx.h"
#include "X2KomFileViewer.h"
#include "NoticeDlg.h"
#include ".\noticedlg.h"


// CNoticeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNoticeDlg, CDialog)
CNoticeDlg::CNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeDlg::IDD, pParent)
{
}

CNoticeDlg::~CNoticeDlg()
{
}
 
void CNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NOTICE, m_noticeStatic);
}


BEGIN_MESSAGE_MAP(CNoticeDlg, CDialog)
	ON_WM_KEYDOWN()
	
END_MESSAGE_MAP()


BOOL CNoticeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( m_wstrText.empty() == false )
		m_noticeStatic.SetWindowText( m_wstrText.c_str() );

	return TRUE;
}