#pragma once
#include "afxcmn.h"

#include "SkinDialog.h"
#include "SkinButton.h"
#include "XListCtrl.h"

// CToolDungeonMap 대화 상자입니다.

class CToolDungeonMap : public CDialogEx
{
	DECLARE_DYNAMIC(CToolDungeonMap)

public:
	CToolDungeonMap(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CToolDungeonMap();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	void UpdateDungeonMap();
	void OnPaintMap();
public:
	CXListCtrl m_listDungeonMap;

	afx_msg LRESULT OnEditChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnComboChange(WPARAM wParam, LPARAM lParam);
};
