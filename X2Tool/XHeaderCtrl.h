
// XHeaderCtrl.h
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XHEADERCTRL_H
#define XHEADERCTRL_H

#include <tchar.h>
#include "MemDC.h"

#define FLATHEADER_TEXT_MAX	80

#define XHEADERCTRL_NO_IMAGE		0
#define XHEADERCTRL_UNCHECKED_IMAGE	1
#define XHEADERCTRL_CHECKED_IMAGE	2

///////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl window

#define FH_PROPERTY_SPACING			1
#define FH_PROPERTY_ARROW			2
#define FH_PROPERTY_STATICBORDER	3
#define FH_PROPERTY_DONTDROPCURSOR	4
#define FH_PROPERTY_DROPTARGET		5


class CXHeaderCtrl : public CHeaderCtrl
{
    DECLARE_DYNCREATE(CXHeaderCtrl)

// Construction
public:
	CXHeaderCtrl();
	virtual ~CXHeaderCtrl();

// Attributes
public:
	BOOL ModifyProperty(WPARAM wParam, LPARAM lParam);
	int GetSpacing() { return m_iSpacing; }
	void SetSpacing(int nSpacing) { m_iSpacing = nSpacing; }
	
	// JCW_+ 2008.04.12 START
	void SetFlat(BOOL bFlat=TRUE) { m_bFlat = bFlat; }
	void SetColorHeader(COLORREF color) { m_colorHeader = color; }
	void SetBgColor(COLORREF color) { m_cr3DFace = color; }
	void SetHeight(int nHeight) { m_nHeight = nHeight; RedrawWindow(); };
	void SetTextFont(CFont *pTextFont) { m_pTextFont = pTextFont; RedrawWindow(); }
	// JCW_+ 2008.04.12 END

// Overrides
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bDoubleBuffer;
	int m_iSpacing;
	SIZE m_sizeImage;
	SIZE m_sizeArrow;
	BOOL m_bStaticBorder;
	UINT m_nDontDropCursor;
	BOOL m_bResizing;
	UINT m_nClickFlags;
	CPoint m_ptClickPoint;

	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crBtnText;	

	// JCW_+ 2008.04.12 START
	BOOL m_bTracking;
	BOOL m_bLMouseDown;

	BOOL m_bFlat;
	COLORREF m_colorHeader;

	int m_nHeight;
	CFont *m_pTextFont;
	// JCW_+ 2008.04.12 END

	void DrawCtrl(CDC* pDC);
	int DrawImage(CDC* pDC, CRect rect, LPHDITEM hdi, BOOL bRight);
	int DrawBitmap(CDC* pDC, CRect rect, LPHDITEM hdi, CBitmap* pBitmap, 
		BITMAP* pBitmapInfo, BOOL bRight);
	int DrawText (CDC* pDC, CRect rect, LPHDITEM lphdi);

	// JCW_+ 2008.04.12 START
	void DrawMouseOver(CDC* pDC, CRect rect);
	void DrawMouseOut(CDC* pDC, CRect rect);
	void DrawMouseDown(CDC* pDC, CRect rect);
	// JCW_+ 2008.04.12 END

// Generated message map functions
protected:
	//{{AFX_MSG(CXHeaderCtrl)
	afx_msg LRESULT OnDeleteItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnInsertItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnLayout(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetImageList(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	// JCW_+ 2008.04.12 START
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	// JCW_+ 2008.04.12 END
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XHEADERCTRL_H
