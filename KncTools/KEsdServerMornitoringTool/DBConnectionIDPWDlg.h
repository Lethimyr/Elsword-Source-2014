#pragma once
#include "afxwin.h"


// CDBConnectionIDPWDlg dialog

class CDBConnectionIDPWDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBConnectionIDPWDlg)

public:
	CDBConnectionIDPWDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDBConnectionIDPWDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	void GetStringID( std::string& strID );
	void GetStringPW( std::string& strPW );

private:
	CString m_strID;
	CString m_strPW;
public:
	CEdit m_DBConnID;
	CEdit m_DBConnPW;
	afx_msg void OnBnClickedOk();
};
