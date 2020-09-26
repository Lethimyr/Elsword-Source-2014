
#ifndef _SKINBUTTON_H
#define _SKINBUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// Return values
#define	SKINBTN_OK							0
#define	SKINBTN_INVALIDRESOURCE				1
#define	SKINBTN_FAILEDMASK					2
#define	SKINBTN_INVALIDINDEX				3
#define	SKINBTN_INVALIDALIGN				4
#define	SKINBTN_BADPARAM					5
#define	SKINBTN_INVALIDPRESSEDSTYLE			6

class CSkinButton : public CButton
{
public:
    CSkinButton();
	~CSkinButton();

	enum	{	SKINBTN_ALIGN_HORIZ=0,				// Icon/bitmap on the left, text on the right
				SKINBTN_ALIGN_VERT,					// Icon/bitmap on the top, text on the bottom
				SKINBTN_ALIGN_HORIZ_RIGHT,			// Icon/bitmap on the right, text on the left
				SKINBTN_ALIGN_OVERLAP,				// Icon/bitmap on the same space as text
				SKINBTN_ALIGN_CENTER
			};

	enum	{	SKINBTN_COLOR_BK_IN=0,				// Background color when mouse is INside
				SKINBTN_COLOR_FG_IN,				// Text color when mouse is INside
				SKINBTN_COLOR_BK_OUT,				// Background color when mouse is OUTside
				SKINBTN_COLOR_FG_OUT,				// Text color when mouse is OUTside
				SKINBTN_COLOR_BK_FOCUS,				// Background color when the button is focused
				SKINBTN_COLOR_FG_FOCUS,				// Text color when the button is focused
				SKINBTN_MAX_COLORS
			};

    enum	{	SKINBTN_PRESSED_LEFTRIGHT=0,		// Pressed style from left to right (as usual)
				SKINBTN_PRESSED_TOPBOTTOM			// Pressed style from top to bottom
			};

	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSkinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	DWORD SetDefaultColors(BOOL bRepaint = TRUE);
	DWORD SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint = TRUE);
	DWORD GetColor(BYTE byColorIndex, COLORREF* crpColor);

	DWORD SetAlign(BYTE byAlign, BOOL bRepaint = TRUE);
	DWORD SetPressedStyle(BYTE byStyle, BOOL bRepaint = TRUE);

	DWORD SetIcon(HICON hIconIn, HICON hIconOut=NULL, int x=0, int y=0);
	DWORD SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut=NULL, COLORREF crTransColorOut=0);

	POINT m_ptBitmapOrg;
	POINT m_ptIconOrg;

	POINT m_ptPressedOffset;	

protected:
    //{{AFX_MSG(CSkinButton)
	afx_msg void OnSysColorChange();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	BOOL		m_bMouseOnButton;				// Is mouse over the button?
	BOOL		m_bIsPressed;					// Is button pressed?
	BOOL		m_bIsFocused;					// Is button focused?
	BOOL		m_bIsDisabled;					// Is button disabled?
	BOOL		m_bIsDefault;					// Is default button?
	BYTE		m_byAlign;						// Align mode
	COLORREF	m_crColors[SKINBTN_MAX_COLORS];	// Colors to be used

	UINT		m_nTypeStyle;					// Button style

private:
	LRESULT OnSetStyle(WPARAM wParam, LPARAM lParam);

	void FreeResourcesBitmap();
	void FreeResourcesIcon();
	void PrepareBitmapRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage);
	void PrepareIconRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage);

	HBITMAP CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor);
	virtual DWORD DrawBackground(CDC* pDC, CRect* pRect);
	virtual void DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);
	virtual void DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled);

#pragma pack(1)
	typedef struct _STRUCT_ICONS
	{
		HICON		hIcon;			// Handle to icon
		DWORD		dwWidth;		// Width of icon
		DWORD		dwHeight;		// Height of icon
	} STRUCT_ICONS;
#pragma pack()

#pragma pack(1)
	typedef struct _STRUCT_BITMAPS
	{
		HBITMAP		hBitmap;		// Handle to bitmap
		DWORD		dwWidth;		// Width of bitmap
		DWORD		dwHeight;		// Height of bitmap
		HBITMAP		hMask;			// Handle to mask bitmap
		COLORREF	crTransparent;	// Transparent color
	} STRUCT_BITMAPS;
#pragma pack()

	STRUCT_ICONS	m_csIcons[2];
	STRUCT_BITMAPS	m_csBitmaps[2];

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif//_SKINBUTTON_H
