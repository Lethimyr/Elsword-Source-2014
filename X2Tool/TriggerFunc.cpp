// TriggerFunc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "X2Tool.h"
#include "TriggerFunc.h"
#include "afxdialogex.h"


// CTriggerFunc 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTriggerFunc, CDialogEx)

CTriggerFunc::CTriggerFunc(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTriggerFunc::IDD, pParent)
{

}

CTriggerFunc::~CTriggerFunc()
{
}

void CTriggerFunc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CTriggerFunc::SetFunctionData(wstring wstrFunctionData)
{
	//m_richFunction.SetWindowText(wstrFunctionData.c_str());
	GetDlgItem(IDC_EDIT_TRIGGER_FUNC)->SetWindowText(wstrFunctionData.c_str());
}

BEGIN_MESSAGE_MAP(CTriggerFunc, CDialogEx)
END_MESSAGE_MAP()


// CTriggerFunc 메시지 처리기입니다.
