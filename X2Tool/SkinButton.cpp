#include "stdafx.h"
#include "SkinButton.h"
#include "MemDC.h"

extern CGlobal Global;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinButton

// Mask for control's type
#define SKINBTN_TYPEMASK1 SS_TYPEMASK

CSkinButton::CSkinButton()
{
	m_bIsPressed		= FALSE;
	m_bIsFocused		= FALSE;
	m_bIsDisabled		= FALSE;
	m_bMouseOnButton	= FALSE;

	FreeResourcesBitmap();
	FreeResourcesIcon();

	// By default icon is aligned horizontally
	m_byAlign = SKINBTN_ALIGN_HORIZ;

	// By default use usual pressed style
	SetPressedStyle(SKINBTN_PRESSED_LEFTRIGHT, FALSE);
  
	// By default the button is not the default button
	m_bIsDefault = FALSE;

	// Set default colors
	SetDefaultColors(FALSE);

	// Invalid value, since type still unknown
	m_nTypeStyle = SKINBTN_TYPEMASK1;

	m_ptBitmapOrg.x = 0;
	m_ptBitmapOrg.y = 0;
	m_ptIconOrg.x = 0;
	m_ptIconOrg.y = 0;
}

CSkinButton::~CSkinButton()
{
	FreeResourcesBitmap();
	FreeResourcesIcon();
}

BEGIN_MESSAGE_MAP(CSkinButton, CButton)
    //{{AFX_MSG_MAP(CSkinButton)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BM_SETSTYLE, OnSetStyle)
END_MESSAGE_MAP()

void CSkinButton::FreeResourcesBitmap()
{
	::ZeroMemory(&m_csBitmaps, sizeof(m_csBitmaps));
}

void CSkinButton::FreeResourcesIcon()
{
	::ZeroMemory(&m_csIcons, sizeof(m_csIcons));
}

void CSkinButton::PreSubclassWindow() 
{
	UINT nBS = GetButtonStyle();

	// Set initial control type
	m_nTypeStyle = nBS & SKINBTN_TYPEMASK1;

	// Set initial default state flag
	if( m_nTypeStyle == BS_DEFPUSHBUTTON )
	{
		// Set default state for a default button
		m_bIsDefault = TRUE;

		// Adjust style for default button
		m_nTypeStyle = BS_PUSHBUTTON;
	}

	// You should not set the Owner Draw before this call
	// (don't use the resource editor "Owner Draw" or
	// ModifyStyle(0, BS_OWNERDRAW) before calling PreSubclassWindow() )
	ASSERT(m_nTypeStyle != BS_OWNERDRAW);

	// Switch to owner-draw
	ModifyStyle(SKINBTN_TYPEMASK1, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

UINT CSkinButton::OnGetDlgCode() 
{
	UINT nCode = CButton::OnGetDlgCode();

	// Tell the system if we want default state handling
	// (losing default state always allowed)
	nCode |= (m_bIsDefault ? DLGC_DEFPUSHBUTTON : DLGC_UNDEFPUSHBUTTON);

	return nCode;
}

BOOL CSkinButton::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_LBUTTONDBLCLK )
		pMsg->message = WM_LBUTTONDOWN;

	return CButton::PreTranslateMessage(pMsg);
}

HBRUSH CSkinButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
}

void CSkinButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	SetDefaultColors();
}

LRESULT CSkinButton::OnSetStyle(WPARAM wParam, LPARAM lParam)
{
	UINT nNewType = (wParam & SKINBTN_TYPEMASK1);

	// Update default state flag
	if( nNewType == BS_DEFPUSHBUTTON )
	{
		m_bIsDefault = TRUE;
	}
	else if( nNewType == BS_PUSHBUTTON )
	{
		// Losing default state always allowed
		m_bIsDefault = FALSE;
	}

	// Can't change control type after owner-draw is set.
	// Let the system process changes to other style bits
	// and redrawing, while keeping owner-draw style
	return DefWindowProc(BM_SETSTYLE,
		(wParam & ~SKINBTN_TYPEMASK1) | BS_OWNERDRAW, lParam);
}

void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CXMemDC MemDC(CDC::FromHandle(lpDIS->hDC));
	CFont *poldFont = MemDC.SelectObject(GetFont());

	m_bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	m_bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect itemRect = lpDIS->rcItem;

	MemDC.SetBkMode(TRANSPARENT);

	// Prepare draw... paint button background
	DrawBackground(&MemDC, &itemRect);

	// Read the button's title
	CString sTitle;
	GetWindowText(sTitle);	

	CRect captionRect = lpDIS->rcItem;

	// Draw the bitmap
	if( m_csBitmaps[0].hBitmap )
	{
		MemDC.SetBkColor(RGB(255,255,255));
		DrawTheBitmap(&MemDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	}

	// Draw the icon
	if( m_csIcons[0].hIcon )
	{
		captionRect = lpDIS->rcItem;
		DrawTheIcon(&MemDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, m_bIsPressed, m_bIsDisabled);
	}	

	// Write the button title
	if( sTitle.IsEmpty() == FALSE )
	{
		// Draw the button's title
		// If button is pressed then "press" title also
		if( m_bIsPressed)
			captionRect.OffsetRect(1, 1);
    
		// Center text
		CRect centerRect = captionRect;
		MemDC.DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
		MemDC.SetBkMode(TRANSPARENT);

		if( m_bIsDisabled )
		{
			captionRect.OffsetRect(1, 1);
			MemDC.SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
			MemDC.DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
			captionRect.OffsetRect(-1, -1);
			MemDC.SetTextColor(::GetSysColor(COLOR_3DSHADOW));
			MemDC.DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		}
		else
		{
			if( m_bMouseOnButton || m_bIsPressed) 
			{
				MemDC.SetTextColor(m_crColors[SKINBTN_COLOR_FG_IN]);
			}
			else 
			{
				MemDC.SetTextColor(m_crColors[SKINBTN_COLOR_FG_OUT]);
			}
			MemDC.DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		}
	}

	MemDC.SelectObject(poldFont);
}

DWORD CSkinButton::DrawBackground(CDC* pDC, CRect* pRect)
{
	COLORREF crColor;
	if( m_bMouseOnButton || m_bIsPressed) 
	{
		crColor = m_crColors[SKINBTN_COLOR_BK_IN];
	}
	else
	{
		if( m_bIsFocused )
			crColor = m_crColors[SKINBTN_COLOR_BK_FOCUS];
		else
			crColor = m_crColors[SKINBTN_COLOR_BK_OUT];
	}

	CBrush brBackground(crColor);
	pDC->FillRect(pRect, &brBackground);
	brBackground.DeleteObject();

	return SKINBTN_OK;
}
HBITMAP CSkinButton::CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor)
{
	HBITMAP		hMask		= NULL;
	HDC			hdcSrc		= NULL;
	HDC			hdcDest		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL);
	if( hMask == NULL )	return NULL;

	hdcSrc	= ::CreateCompatibleDC(NULL);
	hdcDest	= ::CreateCompatibleDC(NULL);

	hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
	hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

	crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

	::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc,RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);

	SetTextColor(hdcDest, crSaveDestText);

	::SetBkColor(hdcSrc, crSaveBk);
	::SelectObject(hdcSrc, hbmSrcT);
	::SelectObject(hdcDest, hbmDestT);

	::DeleteDC(hdcSrc);
	::DeleteDC(hdcDest);

	return hMask;
}

void CSkinButton::PrepareBitmapRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage)
{
	CRect rBtn;

	rpImage->CopyRect(rpItem);

	switch (m_byAlign)
	{
		case SKINBTN_ALIGN_HORIZ:
			if( bHasTitle == FALSE )
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpImage->left += m_ptBitmapOrg.x;  
				rpTitle->left += dwWidth + m_ptBitmapOrg.x;		
			}
			// Center image vertically
			rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			break;

		case SKINBTN_ALIGN_HORIZ_RIGHT:
			GetClientRect(&rBtn);
			if( bHasTitle == FALSE )
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpTitle->right = rpTitle->Width() - dwWidth - m_ptBitmapOrg.x;
				rpTitle->left = m_ptBitmapOrg.x;
				rpImage->left = rBtn.right - dwWidth - m_ptBitmapOrg.x;
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			break;
		
		case SKINBTN_ALIGN_VERT:
			// Center image horizontally
			rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			if( bHasTitle == FALSE )
			{
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			else
			{
				rpImage->top = m_ptBitmapOrg.y;
				rpTitle->top += dwHeight;
			}
			break;

		case SKINBTN_ALIGN_OVERLAP:
			break;
		case SKINBTN_ALIGN_CENTER :
			if( bHasTitle == FALSE )
			{
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			else
			{
				rpImage->top = 3;
				rpImage->left = ((rpImage->Width() - (long)dwHeight))/2;
			}
			break;
	} // switch
    
	if( bIsPressed )
		rpImage->OffsetRect(m_ptPressedOffset.x, m_ptPressedOffset.y);
}

void CSkinButton::PrepareIconRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage)
{
	CRect rBtn;

	rpImage->CopyRect(rpItem);

	switch (m_byAlign)
	{
		case SKINBTN_ALIGN_HORIZ:
			if( bHasTitle == FALSE )
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpImage->left += m_ptIconOrg.x;  
				rpTitle->left += dwWidth + m_ptIconOrg.x;		
			}
			// Center image vertically
			rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			break;

		case SKINBTN_ALIGN_HORIZ_RIGHT:
			GetClientRect(&rBtn);
			if( bHasTitle == FALSE )
			{
				// Center image horizontally
				rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			}
			else
			{
				// Image must be placed just inside the focus rect
				rpTitle->right = rpTitle->Width() - dwWidth - m_ptIconOrg.x;
				rpTitle->left = m_ptIconOrg.x;
				rpImage->left = rBtn.right - dwWidth - m_ptIconOrg.x;
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			break;
		
		case SKINBTN_ALIGN_VERT:
			// Center image horizontally
			rpImage->left += ((rpImage->Width() - (long)dwWidth)/2);
			if( bHasTitle == FALSE )
			{
				// Center image vertically
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			else
			{
				rpImage->top = m_ptIconOrg.y;
				rpTitle->top += dwHeight;
			}
			break;

		case SKINBTN_ALIGN_OVERLAP:
			break;
		case SKINBTN_ALIGN_CENTER :
			if( bHasTitle == FALSE )
			{
				rpImage->top += ((rpImage->Height() - (long)dwHeight)/2);
			}
			else
			{
				rpImage->top = 3;
				rpImage->left = ((rpImage->Width() - (long)dwHeight))/2;
			}
			break;
	} // switch
    
	if( bIsPressed )
		rpImage->OffsetRect(m_ptPressedOffset.x, m_ptPressedOffset.y);
}

void CSkinButton::DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	BYTE byIndex = 0;

	// Select the icon to use
	if( bIsPressed || m_bMouseOnButton )
		byIndex = 0;
	else
		byIndex = (m_csIcons[1].hIcon == NULL ? 0 : 1);

	CRect	rImage;
	PrepareIconRect(bHasTitle, rpItem, rpCaption, bIsPressed, m_csIcons[byIndex].dwWidth, m_csIcons[byIndex].dwHeight, &rImage);

	pDC->DrawState(	rImage.TopLeft(),
					rImage.Size(), 
					(HICON)m_csIcons[byIndex].hIcon,
					DSS_NORMAL, 
					(HBRUSH)NULL);
}

void CSkinButton::DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	HDC			hdcBmpMem	= NULL;
	HBITMAP		hbmOldBmp	= NULL;
	HDC			hdcMem		= NULL;
	HBITMAP		hbmT		= NULL;
	BYTE		byIndex		= 0;

	// Select the bitmap to use
	if( bIsPressed || m_bMouseOnButton )
		byIndex = 0;
	else
		byIndex = (m_csBitmaps[1].hBitmap == NULL ? 0 : 1);
	
	CRect rImage;
	PrepareBitmapRect(bHasTitle, rpItem, rpCaption, bIsPressed, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, &rImage);

	hdcBmpMem = ::CreateCompatibleDC(pDC->m_hDC);

	hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, m_csBitmaps[byIndex].hBitmap);

	hdcMem = ::CreateCompatibleDC(NULL);

	hbmT = (HBITMAP)::SelectObject(hdcMem, m_csBitmaps[byIndex].hMask);

	::BitBlt(pDC->m_hDC, rImage.left, rImage.top, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcBmpMem, 0, 0, SRCCOPY);

	::SelectObject(hdcMem, hbmT);
	::DeleteDC(hdcMem);

	::SelectObject(hdcBmpMem, hbmOldBmp);
	::DeleteDC(hdcBmpMem);
}

DWORD CSkinButton::SetIcon(HICON hIconIn, HICON hIconOut/*=NULL*/, int x/*=0*/, int y/*=0*/)
{
	BOOL		bRetValue;
	ICONINFO	ii;

	if( m_csIcons[0].hIcon == hIconIn && m_csIcons[1].hIcon == hIconOut )
		return SKINBTN_OK;

	m_ptIconOrg.x = x;
	m_ptIconOrg.y = y;

	// Free any loaded resource
	FreeResourcesIcon();

	// Icon when mouse over button?
	if( hIconIn )
	{		
		m_csIcons[0].hIcon = hIconIn;
		// Get icon dimension
		::ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(hIconIn, &ii);
		if( bRetValue == FALSE )
		{
			FreeResourcesIcon();
			return SKINBTN_INVALIDRESOURCE;
		}

		m_csIcons[0].dwWidth	= (DWORD)(ii.xHotspot * 2);
		m_csIcons[0].dwHeight	= (DWORD)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);		
	}

	// Icon when mouse outside button?
	if( hIconOut )
	{
		m_csIcons[1].hIcon = hIconOut;
		// Get icon dimension
		::ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(hIconOut, &ii);
		if( bRetValue == FALSE )
		{
			FreeResourcesIcon();
			return SKINBTN_INVALIDRESOURCE;
		}

		m_csIcons[1].dwWidth	= (DWORD)(ii.xHotspot * 2);
		m_csIcons[1].dwHeight	= (DWORD)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);
	}

	Invalidate();

	return SKINBTN_OK;
}

DWORD CSkinButton::SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut, COLORREF crTransColorOut)
{
	int		nRetValue = 0;
	BITMAP	csBitmapSize;

	if( m_csBitmaps[0].hBitmap == hBitmapIn && m_csBitmaps[0].crTransparent == crTransColorIn &&
		m_csBitmaps[1].hBitmap == hBitmapOut && m_csBitmaps[1].crTransparent == crTransColorOut )
		return SKINBTN_OK;

	m_ptBitmapOrg.x = 0;
	m_ptBitmapOrg.y = 0;

	// Free any loaded resource
	FreeResourcesBitmap();

	if( hBitmapIn )
	{
		m_csBitmaps[0].hBitmap = hBitmapIn;
		m_csBitmaps[0].crTransparent = crTransColorIn;
		// Get bitmap size
		nRetValue = ::GetObject(hBitmapIn, sizeof(csBitmapSize), &csBitmapSize);
		if( nRetValue == 0 )
		{
			FreeResourcesBitmap();
			return SKINBTN_INVALIDRESOURCE;
		}
		m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create mask for bitmap In
		m_csBitmaps[0].hMask = CreateBitmapMask(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
		if( m_csBitmaps[0].hMask == NULL )
		{
			FreeResourcesBitmap();
			return SKINBTN_FAILEDMASK;
		}		
	}

	if( hBitmapOut )
	{
		m_csBitmaps[1].hBitmap = hBitmapOut;
		m_csBitmaps[1].crTransparent = crTransColorOut;
		// Get bitmap size
		nRetValue = ::GetObject(hBitmapOut, sizeof(csBitmapSize), &csBitmapSize);
		if( nRetValue == 0 )
		{
			FreeResourcesBitmap();
			return SKINBTN_INVALIDRESOURCE;
		}
		m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create mask for bitmap Out
		m_csBitmaps[1].hMask = CreateBitmapMask(hBitmapOut, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransColorOut);
		if( m_csBitmaps[1].hMask == NULL )
		{
			FreeResourcesBitmap();
			return SKINBTN_FAILEDMASK;
		}
	}

	Invalidate();

	return SKINBTN_OK;
}

DWORD CSkinButton::SetAlign(BYTE byAlign, BOOL bRepaint)
{
	switch (byAlign)
	{    
		case SKINBTN_ALIGN_HORIZ:
		case SKINBTN_ALIGN_HORIZ_RIGHT:
		case SKINBTN_ALIGN_VERT:
		case SKINBTN_ALIGN_OVERLAP:
			m_byAlign = byAlign;
			if( bRepaint )	Invalidate();
			return SKINBTN_OK;
			break;
	}

	return SKINBTN_INVALIDALIGN;
}

DWORD CSkinButton::SetPressedStyle(BYTE byStyle, BOOL bRepaint)
{
	switch (byStyle)
	{
		case SKINBTN_PRESSED_LEFTRIGHT:
			m_ptPressedOffset.x = 1;
			m_ptPressedOffset.y = 1;
			break;
		case SKINBTN_PRESSED_TOPBOTTOM:
			m_ptPressedOffset.x = 0;
			m_ptPressedOffset.y = 2;
			break;
		default:
			return SKINBTN_INVALIDPRESSEDSTYLE;
	}

	if( bRepaint )	Invalidate();

	return SKINBTN_OK;
}

DWORD CSkinButton::SetDefaultColors(BOOL bRepaint)
{
	m_crColors[SKINBTN_COLOR_BK_IN]		= Global.GetRGB(IDX_RGB_BACKGROUND);
	m_crColors[SKINBTN_COLOR_FG_IN]		= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[SKINBTN_COLOR_BK_OUT]	= Global.GetRGB(IDX_RGB_BACKGROUND);
	m_crColors[SKINBTN_COLOR_FG_OUT]	= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[SKINBTN_COLOR_BK_FOCUS]	= Global.GetRGB(IDX_RGB_BACKGROUND);
	m_crColors[SKINBTN_COLOR_FG_FOCUS]	= ::GetSysColor(COLOR_BTNTEXT);

	if( bRepaint )	Invalidate();

	return SKINBTN_OK;
} 

DWORD CSkinButton::SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint)
{
	if( byColorIndex >= SKINBTN_MAX_COLORS )	return SKINBTN_INVALIDINDEX;

	// Set new color
	m_crColors[byColorIndex] = crColor;

	if( bRepaint )	Invalidate();

	return SKINBTN_OK;
}

DWORD CSkinButton::GetColor(BYTE byColorIndex, COLORREF* crpColor)
{
	if( byColorIndex >= SKINBTN_MAX_COLORS )	return SKINBTN_INVALIDINDEX;

	// Get color
	*crpColor = m_crColors[byColorIndex];

	return SKINBTN_OK;
}