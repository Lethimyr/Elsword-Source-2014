#if !defined(AFX_XDATEBOX_H__3F6322A0_C720_405C_89F5_743E746954EA__INCLUDED_)
#define AFX_XDATEBOX_H__3F6322A0_C720_405C_89F5_743E746954EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XDateBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXDateBox window
extern UINT NEAR WM_XDATEBOX_CHANGE;

class CXDateBox : public CDateTimeCtrl
{
// Construction
public:
	CXDateBox();

// Attributes
public:
	

private:
	int m_nVK;

	// Singleton instance
	static CXDateBox* m_pXDateBox;

// Operations
public:
	// Returns the instance of the class
	static CXDateBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

	static CString GetDate(CTime &time, CString &sDateFormat);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXDateBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXDateBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXDateBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XDATEBOX_H__3F6322A0_C720_405C_89F5_743E746954EA__INCLUDED_)
