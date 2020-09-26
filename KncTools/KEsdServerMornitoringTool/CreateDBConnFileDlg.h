#pragma once
#include "afxwin.h"


// CCreateDBConnFileDlg 대화 상자입니다.

class CCreateDBConnFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateDBConnFileDlg)

public:
	CCreateDBConnFileDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCreateDBConnFileDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATE_DBCONNFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_editBoxFilePath;
};
