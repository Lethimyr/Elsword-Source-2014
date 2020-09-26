// TriggerEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "TriggerEdit.h"
#include "afxdialogex.h"


// CTriggerEdit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTriggerEdit, CDialog)

CTriggerEdit::CTriggerEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerEdit::IDD, pParent)
{

}

CTriggerEdit::~CTriggerEdit()
{
}

void CTriggerEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTriggerEdit, CDialog)
END_MESSAGE_MAP()


// CTriggerEdit 메시지 처리기입니다.
