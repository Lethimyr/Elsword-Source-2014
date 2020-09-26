// NoticeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "NoticeDlg.h"


// CNoticeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNoticeDlg, CDialog)
CNoticeDlg::CNoticeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoticeDlg::IDD, pParent)
{
}

CNoticeDlg::~CNoticeDlg()
{
}

BOOL CNoticeDlg::PreTranslateMessage(MSG* pMsg)
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

void CNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNoticeDlg, CDialog)
END_MESSAGE_MAP()


// CNoticeDlg 메시지 처리기입니다.
