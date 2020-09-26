#pragma once
#include "afxwin.h"


// CPathDialog 대화 상자입니다.

class CPathDialog : public CDialog
{
	DECLARE_DYNAMIC(CPathDialog)

public:
	CPathDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOldPath();
	afx_msg void OnBnClickedNewPath();
	afx_msg void OnBnClickedOk();
	

public:
	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );
	std::wstring& GetOldPath() { return m_wstrOldPath; }
	std::wstring& GetNewPath() { return m_wstrNewPath; }
	bool GetLuaScriptCompileFlag() { return m_bDoCompile; }
	bool GetLuaScriptCryptFlag() { return m_bDoCrypt; }
	bool GetXFileConvertFlag() { return m_bDoXConvert; }

private:
	std::wstring m_wstrOldPath;		// 구버전 폴더 경로
	std::wstring m_wstrNewPath;		// 신버전 폴더 경로
	bool m_bDoCompile;
	bool m_bDoCrypt;
	bool m_bDoXConvert;
public:
	CEdit m_oldPathEdit;
	CEdit m_newPathEdit;
	CButton m_checkCompile;
	CButton m_checkCrypt;
	CButton m_checkXConvert;
	afx_msg void OnBnClickedCompileCheck();
	afx_msg void OnBnClickedCryptCheck();
	afx_msg void OnBnClickedXFileConvertCheck();	
};
