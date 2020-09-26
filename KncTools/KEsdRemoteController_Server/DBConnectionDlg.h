#pragma once
#include "afxwin.h"


// DBConnectionDlg 대화 상자입니다.

class DBConnectionDlg : public CDialog
{
	DECLARE_DYNAMIC(DBConnectionDlg)

public:
	DBConnectionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DBConnectionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

	void SetEditList( std::vector< KClearString >* pVecDBConnIDPW ) { m_pVecDBConnIDPW = pVecDBConnIDPW; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	std::vector< KClearString >* m_pVecDBConnIDPW;
public:
	afx_msg void OnBnClickedOk();
protected:
	CEdit m_IDEditBox;
	CEdit m_PWEditBox;
};
