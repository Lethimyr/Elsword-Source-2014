#if !defined(AFX_XEDITBOX_H__2E11CFFF_9B31_4C70_BCF0_CD681692808D__INCLUDED_)
#define AFX_XEDITBOX_H__2E11CFFF_9B31_4C70_BCF0_CD681692808D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XEditBox.h : header file
//

extern UINT NEAR WM_XEDITBOX_CHANGE;

/////////////////////////////////////////////////////////////////////////////
// CXEditBox window

class CXEditBox : public CEdit
{
// Construction
public:
	CXEditBox();

// Attributes
private:
	// Singleton instance
	static CXEditBox* m_pXEditBox;
	int m_nMinNum;
	int m_nMaxNum;

	int m_nVK;

// Operations
public:
	// Returns the instance of the class
	static CXEditBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();
	
	void SetMinNum(int nMinNum){ m_nMinNum = nMinNum; }
	void SetMaxNum(int nMaxNum){ m_nMaxNum = nMaxNum; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXEditBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXEditBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXEditBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnSpinDelta( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XEDITBOX_H__2E11CFFF_9B31_4C70_BCF0_CD681692808D__INCLUDED_)
