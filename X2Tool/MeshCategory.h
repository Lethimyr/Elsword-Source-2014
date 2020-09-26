#pragma once
#include "afxcmn.h"


// CMeshCategory 대화 상자입니다.

class CMeshCategory : public CDialogEx
{
	DECLARE_DYNAMIC(CMeshCategory)

public:
	CMeshCategory(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeshCategory();

	void UpdateTree(wstring wstrPath, HTREEITEM hParent);
	void UpdateLib();
	void UpdateList(wstring wstrLib);
	void ClearSelected();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listObject;
	vector<wstring> m_vecFileName;
	bool m_bCanUseLib;
	
	CTreeCtrl m_treeLib;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditLibraryPath();
	afx_msg void OnLvnItemchangedListObject(NMHDR *pNMHDR, LRESULT *pResult);
};
