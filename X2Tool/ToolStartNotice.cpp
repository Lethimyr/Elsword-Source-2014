// ToolStartNotice.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "ToolStartNotice.h"
#include "afxdialogex.h"


// CToolStartNotice 대화 상자입니다.

IMPLEMENT_DYNAMIC(CToolStartNotice, CDialogEx)

CToolStartNotice::CToolStartNotice(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolStartNotice::IDD, pParent)
{

}

CToolStartNotice::~CToolStartNotice()
{
}

void CToolStartNotice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CToolStartNotice, CDialogEx)
	ON_BN_CLICKED(IDOKNOTICE, &CToolStartNotice::OnBnClickedOknotice)
END_MESSAGE_MAP()


// CToolStartNotice 메시지 처리기입니다.


void CToolStartNotice::OnBnClickedOknotice()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"허허 용케 눌렀네...\n하지만 이건 아니란다...\n\n팁을 알려주자면, ESC를 눌러봐!", L"알림", MB_OK);
}
