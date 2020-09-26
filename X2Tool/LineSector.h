#pragma once


// CLineSector 대화 상자입니다.

class CLineSector : public CDialogEx
{
	DECLARE_DYNAMIC(CLineSector)

public:
	CLineSector(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLineSector();

	afx_msg BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditSectorWidth();
	afx_msg void OnEnChangeEditSectorHeight();
	afx_msg void OnEnChangeEditSectorDepth();
	afx_msg void OnBnClickedButton1();
};
