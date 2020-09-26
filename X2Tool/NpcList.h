#pragma once
#include "afxcmn.h"


// CNpcList 대화 상자입니다.

class CNpcList : public CDialogEx
{
	DECLARE_DYNAMIC(CNpcList)

public:
	CNpcList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNpcList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_NPC_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void InitNpcData();

	afx_msg void OnBnClickedOk();
	CListCtrl m_listNpcData;
	vector<CX2UnitManager::NPC_UNIT_ID> m_vecNpc;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
