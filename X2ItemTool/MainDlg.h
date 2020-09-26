#pragma once


// CMainDlg 대화 상자입니다.

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


protected:
	CGridCtrl		m_GridCtrl;
	CRect           m_rcGrid;
public:
	afx_msg void OnDestroy();

	void OpenLuaFile( const WCHAR* wszFileName );
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void ResizeGridCtrl( int cx, int cy );
};
