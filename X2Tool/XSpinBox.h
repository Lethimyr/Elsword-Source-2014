#if !defined(AFX_XSPINBOX_H__94AF797E_D6F6_442A_BE80_E964CEB27EFC__INCLUDED_)
#define AFX_XSPINBOX_H__94AF797E_D6F6_442A_BE80_E964CEB27EFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XSpinBox.h : header file
//

extern UINT NEAR WM_XSPINBOX_CHANGING;
extern UINT NEAR WM_XSPINBOX_DELTA;

/////////////////////////////////////////////////////////////////////////////
// CXSpinBox window

class CXSpinBox : public CSpinButtonCtrl
{
// Construction
public:
	CXSpinBox();

// Attributes
	// Singleton instance
	static CXSpinBox* m_pXSpinBox;

// Operations
public:
	// Returns the instance of the class
	static CXSpinBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXSpinBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXSpinBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXSpinBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSPINBOX_H__94AF797E_D6F6_442A_BE80_E964CEB27EFC__INCLUDED_)
