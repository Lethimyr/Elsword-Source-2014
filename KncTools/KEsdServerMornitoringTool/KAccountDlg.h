#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CKAccountDlg 대화 상자입니다.

class CKAccountDlg : public CDialog
{
	DECLARE_DYNAMIC(CKAccountDlg)

public:
	CKAccountDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CKAccountDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ACCOUNT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNewAccount();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelAccount();
	CEdit m_newIdEdit;
	CEdit m_newPwEdit;
	CEdit m_rePwEdit;
	CListCtrl m_accList;

public:
	void UpdateAccountList();
	afx_msg void OnLvnItemActivateAccountList(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_authLevel;
};