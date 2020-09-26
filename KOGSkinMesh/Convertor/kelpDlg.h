// kelpDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <stack>


UINT threadFunc(LPVOID ss); 

// CkelpDlg 대화 상자
class CkelpDlg : public CDialog
{
// 생성
public:
	CkelpDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_KELP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

   

// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
   CListBox m_fileListBox;

private:
   CString m_currentPath;
   
   std::vector<CString> m_fileList;
   std::stack<CString> m_pathStack;

   int m_nDriveCnt, m_oldcnt;
   char *m_pDriveName;

public:
   void GetDrive();
   int MoveDirectroy(CString &strPath);
   void outputList();
   void addFolder(CString strPath);
   afx_msg void OnLbnSelchangeList1();
   afx_msg void OnLbnDblclkList1();
   afx_msg void OnBnClickedOk();
   afx_msg void OnBnClickedCancel();
};
