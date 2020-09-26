#pragma once
#include "afxwin.h"


// CNoticeDlg 대화 상자입니다.

class CNoticeDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoticeDlg)

public:
	CNoticeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNoticeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NOTICE_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	void ChangeText( std::wstring& wstrText ) { m_wstrText = wstrText; }
	CStatic m_noticeStatic;	
	std::wstring m_wstrText;
};
