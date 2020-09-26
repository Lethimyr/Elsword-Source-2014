// Global.h: interface for the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__BCF123B6_793D_4A34_93B2_51169F691D0B__INCLUDED_)
#define AFX_GLOBAL_H__BCF123B6_793D_4A34_93B2_51169F691D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------
#define IDX_BMP_CAPTION_LEFT				0		//
#define IDX_BMP_CAPTION_CENTER				1		//
#define IDX_BMP_CAPTION_RIGHT				2		//
#define IDX_BMP_FRAME_LEFT					3		//
#define IDX_BMP_FRAME_LEFT_BOTTOM			4		//
#define IDX_BMP_FRAME_BOTTOM				5		//
#define IDX_BMP_FRAME_RIGHT_BOTTOM			6		//
#define IDX_BMP_FRAME_RIGHT					7		//
#define IDX_BMP_BOX_MINIMIZE				8		//
#define IDX_BMP_BOX_MAXMIZE					9		//
#define IDX_BMP_BOX_RESTORE					10		//
#define IDX_BMP_BOX_EXIT					11		//
#define IDX_BMP_BOX_MINIMIZE_ON				12		//
#define IDX_BMP_BOX_MAXMIZE_ON				13		//
#define IDX_BMP_BOX_RESTORE_ON				14		//
#define IDX_BMP_BOX_EXIT_ON					15		//
#define IDX_BMP_BOX_MINIMIZE_OVER			16		//
#define IDX_BMP_BOX_MAXMIZE_OVER			17		//
#define IDX_BMP_BOX_RESTORE_OVER			18		//
#define IDX_BMP_BOX_EXIT_OVER				19		//
#define IDX_BMP_BTN_BASE					20		//	
#define IDX_BMP_LIST_HEADER_SHADOW			21		//
#define IDX_BMP_LIST_HEADER_DIVISION		22		//
#define IDX_BMP_MAX							23		//

//--------------------------------------------------------
#define IDX_ICON_SKIN						0		//
#define IDX_ICON_SKIN_INACTIVE				1		//
#define IDX_ICON_OK							2		//
#define IDX_ICON_CANCEL						3		//
#define IDX_ICON_HOME						4		//
#define IDX_ICON_FAVORITES					5		//
#define IDX_ICON_INTERNET					6		//
#define IDX_ICON_SETTING					7		//
#define IDX_ICON_MAX						8		//

//--------------------------------------------------------
#define ICON16								0		//
#define ICON24								1		//
#define ICON32								2		//

//--------------------------------------------------------
#define IDX_FONT_SMALL						0		//
#define IDX_FONT_MEDIUM						1		//
#define IDX_FONT_LARGE						2		//
#define IDX_FONT_MAX						3		//

//--------------------------------------------------------
#define IDX_RGB_MASK						0		//
#define IDX_RGB_BACKGROUND					1		//
#define IDX_RGB_FONT						2		//
#define IDX_RGB_FONT_CAPTION				3		//
#define IDX_RGB_FONT_CAPTION_INACTIVE		4		//
#define IDX_RGB_CONTROL_BORDER				5		//
#define IDX_RGB_MAX							6		//

//
#define KEYDOWN(vk_code)	((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)		((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//===========================================================================
// function pointer definitions for SetLayeredWindowAttributes in User32.dll
//===========================================================================
typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

typedef struct tagBITMAP_INFO
{
	HBITMAP hBitmap;
	BITMAP	bitmap;
} BITMAP_INFO;

class CGlobal
{
public:
	CGlobal();
	virtual ~CGlobal();

// Attributes
public:
	CFont			*m_pFont[IDX_FONT_MAX];
	BITMAP_INFO		m_infoBitmap[IDX_BMP_MAX];
	HICON			m_hIcon[IDX_ICON_MAX][3];
	COLORREF		m_cf[IDX_RGB_MAX];

// Operations
public:
	lpfn			SetLayeredWindowAttributes;

	void			DeleteBitmaps();	
	void			LoadBitmaps();	
	HBITMAP			GetBitmap(UINT nBitmapID);
	SIZE			GetBitmapSize(UINT nBitmapID);
	CString			GetBitmapFileName(UINT nBitmapID);

	CString			GetModulePath();

	void			DeleteIcons();
	void			LoadIcons();
	CString			GetIconFileName(UINT nIconID);
	HICON			GetIcon(UINT nIconID, UINT nType);

	void			SetRGB(UINT nRGBID, COLORREF cf);
	COLORREF		GetRGB(UINT nRGBID);
	
	void			DeleteFonts();
	CFont	*		SetFont(UINT nFontID, CString sFaceName, UINT nFontSize, BOOL bBold=FALSE);
	CFont	*		GetFont(UINT nFontID);

	CSize			FormatText(CString& sText, CDC* pDC, int nWidth);

	COLORREF		Darker(COLORREF crBase, float fFactor);
	COLORREF		Lighter(COLORREF crBase, float fFactor);
};

#endif // !defined(AFX_GLOBAL_H__BCF123B6_793D_4A34_93B2_51169F691D0B__INCLUDED_)
