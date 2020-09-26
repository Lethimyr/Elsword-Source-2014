#pragma once
#include "2DPushGraph.h"
#include "afxcmn.h"
#include "KEsdRemoteController_ConsoleImp.h"


// KDateInfoDlg 대화 상자입니다.

class KDateInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(KDateInfoDlg)

	enum DATE_INFO_TAB
	{
		DIT_DAY,
		DIT_WEEK,
	};

public:
	KDateInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	KDateInfoDlg( int iGraphMaxValue, CWnd* pParent = NULL);
	virtual ~KDateInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DATE_INFO_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	void InitListCtrl();
	void InsertEsdListCtrl( KESDDateStatistics& kInfo );
	void AutoSizeListCtrl();
	void DrawLineCCUGraph( std::wstring& wstrDate );
	void BackToYesterday( IN OUT SYSTEMTIME& sysTime );

public:
	void SetGraphMaxValue( int iMaxValue ) { m_iGraphMaxValue = iMaxValue; }
	void SetMainWindowPos( RECT Pos ) { m_MainWindowPos = Pos; }

protected:
	int  m_iGraphMaxValue;
	RECT m_dayCalPos;
	RECT m_dayBtnPos;
	RECT m_weekCalPos;
	RECT m_weekBtnPos;

	RECT m_MainWindowPos;

	C2DPushGraph m_PushGraph;
	std::wstring m_wstrSelectedDate;

	// Day Statistics
	std::vector< CCUInfo > m_vecDateCCU;
	KESDDateStatistics m_dateStatistics;

	// Week Statistics
	std::wstring m_wstrLastGetDate;
	SYSTEMTIME  m_today;
	std::vector< KESDDateStatistics > m_vecWeekStatistics;

public:
	CListCtrl m_dateInfoList;
	CMonthCalCtrl m_monthCalCtrl;
	CButton m_dateInfoBtn;
	afx_msg void OnMcnSelchangeMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_dateInfoTab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
//	afx_msg void OnNMCustomdrawAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAlphaSlider(NMHDR *pNMHDR, LRESULT *pResult);
};
