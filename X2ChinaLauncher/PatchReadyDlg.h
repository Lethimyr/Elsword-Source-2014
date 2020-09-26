#pragma once


// CPatchReadyDlg 대화 상자입니다.

class CPatchReadyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPatchReadyDlg)

public:
	enum 
	{
		WM_CLOSE_EVENT = WM_USER+1,
	};

	CPatchReadyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPatchReadyDlg();

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATCHREADY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg LRESULT OnCloseEvent(WPARAM wParam, LPARAM lParam);

private:
	CStatic m_Static1;
	CStatic m_Static2;
};
