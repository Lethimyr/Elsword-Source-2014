#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLineBaseEditor 대화 상자입니다.

class CLineBaseEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CLineBaseEditor)

public:
	CLineBaseEditor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLineBaseEditor();

	virtual void OnFinalRelease();

	void UpdateLineInfo();
	void SetFilletValue(int iVal);
	void SetVertexPos(D3DXVECTOR3 vPos);
	void SetCheck(int iItem, bool bCheck);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EDIT_LINE };

	bool m_bCheckList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:	
	afx_msg BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedRadioLineVertex();
	afx_msg void OnBnClickedRadioLineSegment();
	afx_msg void OnBnClickedElMakefirst();

	CListCtrl m_LineList;
	afx_msg void OnBnClickedElWeld();
	afx_msg void OnBnClickedElSnap();
	CButton m_bCheckSnap;
	afx_msg void OnBnClickedElDivide();
	afx_msg void OnDeltaposSpinElFillet(NMHDR *pNMHDR, LRESULT *pResult);
	
	CSpinButtonCtrl m_spinCtrl;
	afx_msg void OnBnClickedElJoin();
	afx_msg void OnBnClickedElCopy();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonPos();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonLineindex();
	afx_msg void OnBnClickedRadioLineStartposition();
	afx_msg void OnLvnEndlabeleditListLine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListLine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRearrange();	
	
};
