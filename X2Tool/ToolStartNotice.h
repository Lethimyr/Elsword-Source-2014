#pragma once


// CToolStartNotice 대화 상자입니다.

class CToolStartNotice : public CDialogEx
{
	DECLARE_DYNAMIC(CToolStartNotice)

public:
	CToolStartNotice(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CToolStartNotice();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_START_NOTICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOknotice();
};
