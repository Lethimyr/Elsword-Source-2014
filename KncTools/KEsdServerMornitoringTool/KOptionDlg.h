#pragma once
#include "afxwin.h"
#include <boost/timer.hpp>


// KOptionDlg 대화 상자입니다.

class KOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(KOptionDlg)

public:
	KOptionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KOptionDlg();

	void SetGraphMaxValue( int iGraphMaxValue );
	int GetGraphMaxValue() { return m_iGraphMaxValue; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OPTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:	
	CEdit m_graphMaxValueEdit;
	afx_msg void OnBnClickedOk();

private:
	int m_iGraphMaxValue;
	boost::timer m_clickTimer;
};
