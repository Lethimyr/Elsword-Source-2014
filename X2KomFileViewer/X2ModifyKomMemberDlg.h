#pragma once
#include "afxwin.h"
#include "afxcmn.h"


struct ModifyData
{
	std::wstring m_wstrKomString;
	std::wstring m_wstrFileFullPath;
};
// CX2ModifyKomMemberDlg 대화 상자입니다.

class CX2ModifyKomMemberDlg : public CDialog
{
	DECLARE_DYNAMIC(CX2ModifyKomMemberDlg)

public:
	CX2ModifyKomMemberDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CX2ModifyKomMemberDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MODIFY_KOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );
	afx_msg void OnBnClickedFindsavepath();
	afx_msg void OnBnClickedFindmodifyfile();
	afx_msg void OnBnClickedModifybegin();
	void InitListControl();

private:
	std::wstring m_wstrKomSavePath;
	std::map< std::wstring, ModifyData > m_mapModifyData;	// 수정하기로 선택한 리소스

public:
	CEdit m_cSaveDirEdit;
	//CEdit m_cModifyFileEdit;
	CListCtrl m_cModifyListBox;
	afx_msg void OnBnClickedDelmodifyfile();
	CButton m_cLuaCompileCheck;
	CButton m_cScriptEncryptCheck;
};
