// WGameServerDlg.h : header file
//

#pragma once

#include "afxwin.h"

#include <vector>
#include "SecurityString.h"

// DlgTest 대화 상자입니다.

class CDBConnectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBConnectionDlg)

public:
	CDBConnectionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDBConnectionDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DBCONNDLG };

	void SetEditList( std::vector< KClearString< std::wstring > >* pEditList ) { m_pEditList = pEditList; }
	std::vector< KClearString< std::wstring > >* m_pEditList;

	bool IsOK() { return m_bCheckOK; }
	bool m_bCheckOK;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
