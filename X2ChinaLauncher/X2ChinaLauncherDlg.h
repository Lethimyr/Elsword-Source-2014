// X2ChinaLauncherDlg.h : 헤더 파일
//

#pragma once

#include "MainDlg.h"

// CX2ChinaLauncherDlg 대화 상자
class CX2ChinaLauncherDlg : public CDialog
{
// 생성
public:
	CX2ChinaLauncherDlg(CWnd* pParent = NULL);	// 표준 생성자

	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 대화 상자 데이터
	enum { IDD = IDD_X2CHINALAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	HBITMAP Create32BitBitmap( HDC hDCSource, int cx, int cy );

	// 오버랩시킬 다이어로그
	CMainDlg m_MainDlg;

public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);


	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
//	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	afx_msg void OnDestroy();
};
