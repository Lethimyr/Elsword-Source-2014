#pragma once
#include "afxwin.h"


// CKEsdRemoteController_ConsoleLoginDlg 대화 상자입니다.

class CKEsdRemoteController_ConsoleLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CKEsdRemoteController_ConsoleLoginDlg)

public:
	CKEsdRemoteController_ConsoleLoginDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CKEsdRemoteController_ConsoleLoginDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_IDEdit;
	CEdit m_passwordEdit;
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedLogin();	
	CButton m_SaveLoginInfo;
	afx_msg void OnBnClickedCreatedbconn();
};