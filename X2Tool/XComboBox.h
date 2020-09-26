#if !defined(AFX_XCOMBOBOX_H__05957708_2213_4285_AF31_211F3A09A31A__INCLUDED_)
#define AFX_XCOMBOBOX_H__05957708_2213_4285_AF31_211F3A09A31A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXComboBox window
extern UINT NEAR WM_XCOMBOBOX_CHANGE;

class CXComboBox : public CComboBox
{
// Construction
public:
	CXComboBox();

// Attributes
private:
	int m_nVK;

	// Singleton instance
	static CXComboBox* m_pXComboBox;

// Operations
public:
	// Returns the instance of the class
	static CXComboBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXComboBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCbnCloseup();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCOMBOBOX_H__05957708_2213_4285_AF31_211F3A09A31A__INCLUDED_)
