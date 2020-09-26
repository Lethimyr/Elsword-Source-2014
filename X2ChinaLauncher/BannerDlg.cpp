// BannerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "BannerDlg.h"
#include ".\bannerdlg.h"


// CBannerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBannerDlg, CDialog)
CBannerDlg::CBannerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBannerDlg::IDD, pParent)
{
}

CBannerDlg::~CBannerDlg()
{
}

BOOL CBannerDlg::OnInitDialog()
{
	//LONG temp = GetWindowLong( m_hWnd, GWL_EXSTYLE );
	//temp = temp & ~WS_EX_LAYERED;

	//SetWindowLong( m_hWnd, GWL_EXSTYLE, GetWindowLong( m_hWnd, GWL_EXSTYLE ) & ~WS_EX_LAYERED );

	return TRUE;
}

BOOL CBannerDlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
		//alt가 눌렸을때
	case WM_SYSKEYDOWN:
		{
			if(pMsg->wParam == VK_F4)
				return TRUE;
		}
		break;
		// 키가 눌렸을때
	case WM_KEYDOWN:
		switch(pMsg->wParam)
		{
			// 리턴키\tab 
		case VK_RETURN:
			return TRUE;

			// ESC키
		case VK_ESCAPE:
			return TRUE;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CBannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBannerDlg, CDialog)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CBannerDlg 메시지 처리기입니다.

void CBannerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CBannerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return FALSE;
	//return CDialog::OnEraseBkgnd(pDC);
}
