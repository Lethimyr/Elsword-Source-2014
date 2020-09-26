// Global.cpp: implementation of the CGlobal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
CGlobal::CGlobal()
{
	for(int i=0; i<IDX_BMP_MAX; i++)
	{
		m_infoBitmap[i].hBitmap = NULL;
		::ZeroMemory(&m_infoBitmap[i].bitmap, sizeof(BITMAP));
	}

	for(int i=0; i<IDX_FONT_MAX; i++)
		m_pFont[i] = NULL;

	for(int i=0; i<IDX_ICON_MAX; i++)
	{
		m_hIcon[i][0] = NULL;
		m_hIcon[i][1] = NULL;
		m_hIcon[i][2] = NULL;
	}

	for(int i=0; i<IDX_RGB_MAX; i++)
		m_cf[i] = RGB(0,0,0);

	//  get the function pointer for SetLayeredWindowAttributes in User32.dll
    HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
    SetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
    //if(SetLayeredWindowAttributes == NULL)
	//	AfxMessageBox ("Layering is not supported in this version of Windows", MB_ICONEXCLAMATION);
}

CGlobal::~CGlobal()
{
	DeleteBitmaps();
	DeleteFonts();
	DeleteIcons();
}

void CGlobal::DeleteBitmaps()
{
	for(UINT i=0; i<IDX_BMP_MAX; i++)
	{
		if( m_infoBitmap[i].hBitmap )
		{
			::DeleteObject(m_infoBitmap[i].hBitmap);
			m_infoBitmap[i].hBitmap = NULL;
		}
		::ZeroMemory(&m_infoBitmap[i].bitmap, sizeof(BITMAP));
	}
}

void CGlobal::DeleteIcons()
{
	for(UINT i=0; i<IDX_ICON_MAX; i++)
	{
		for(UINT k=0; k<3; k++)
		{
			if( m_hIcon[i][k] )
			{
				::DestroyIcon(m_hIcon[i][k]);
				m_hIcon[i][k] = NULL;
			}
		}
	}
}

CString CGlobal::GetModulePath()
{
	CString sPath;
	GetModuleFileName( AfxGetApp()->m_hInstance, sPath.GetBuffer(MAX_PATH), MAX_PATH );
	sPath.ReleaseBuffer();

	int nPos = sPath.ReverseFind(_T('\\'));
	if( nPos != -1 )
		sPath = sPath.Left( nPos );

	return sPath;
}

void CGlobal::LoadBitmaps()
{
	DeleteBitmaps();

	CString strFileName(_T(""));
	CString sModulePath = GetModulePath() + _T("\\");

	for(UINT i=0; i<IDX_BMP_MAX; i++)
	{
		strFileName = GetBitmapFileName(i);
		if( strFileName.IsEmpty() )
			continue;
		
		strFileName = sModulePath + strFileName;

		m_infoBitmap[i].hBitmap = (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), 
											(LPCTSTR)strFileName, 
											IMAGE_BITMAP, 
											0, 
											0, 
											LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

		::GetObject(m_infoBitmap[i].hBitmap, sizeof(BITMAP), &m_infoBitmap[i].bitmap);
	}
}

CString CGlobal::GetBitmapFileName(UINT nBitmapID)
{
	switch( nBitmapID )
	{
	case IDX_BMP_CAPTION_LEFT				:	return _T("skin\\caption_left.bmp");			break;
	case IDX_BMP_CAPTION_CENTER				:	return _T("skin\\caption_center.bmp");			break;
	case IDX_BMP_CAPTION_RIGHT				:	return _T("skin\\caption_right.bmp");			break;
	case IDX_BMP_FRAME_LEFT					:	return _T("skin\\frame_left.bmp");				break;
	case IDX_BMP_FRAME_LEFT_BOTTOM			:	return _T("skin\\frame_leftbottom.bmp");		break;
	case IDX_BMP_FRAME_BOTTOM				:	return _T("skin\\frame_bottom.bmp");			break;
	case IDX_BMP_FRAME_RIGHT_BOTTOM			:	return _T("skin\\frame_rightbottom.bmp");		break;
	case IDX_BMP_FRAME_RIGHT				:	return _T("skin\\frame_right.bmp");				break;
	case IDX_BMP_BOX_MINIMIZE				:	return _T("skin\\box_minimize.bmp");			break;
	case IDX_BMP_BOX_MAXMIZE				:	return _T("skin\\box_maximize.bmp");			break;
	case IDX_BMP_BOX_RESTORE				:	return _T("skin\\box_restore.bmp");				break;
	case IDX_BMP_BOX_EXIT					:	return _T("skin\\box_exit.bmp");				break;
	case IDX_BMP_BOX_MINIMIZE_ON			:	return _T("skin\\box_minimize_on.bmp");			break;
	case IDX_BMP_BOX_MAXMIZE_ON				:	return _T("skin\\box_maximize_on.bmp");			break;
	case IDX_BMP_BOX_RESTORE_ON				:	return _T("skin\\box_restore_on.bmp");			break;
	case IDX_BMP_BOX_EXIT_ON				:	return _T("skin\\box_exit_on.bmp");				break;
	case IDX_BMP_BOX_MINIMIZE_OVER			:	return _T("skin\\box_minimize_over.bmp");		break;
	case IDX_BMP_BOX_MAXMIZE_OVER			:	return _T("skin\\box_maximize_over.bmp");		break;
	case IDX_BMP_BOX_RESTORE_OVER			:	return _T("skin\\box_restore_over.bmp");		break;
	case IDX_BMP_BOX_EXIT_OVER				:	return _T("skin\\box_exit_over.bmp");			break;
	case IDX_BMP_BTN_BASE					:	return _T("skin\\btn_base.bmp");				break;
	case IDX_BMP_LIST_HEADER_SHADOW			:	return _T("skin\\list_header_shadow.bmp");		break;
	case IDX_BMP_LIST_HEADER_DIVISION		:	return _T("skin\\list_header_division.bmp");		break;
	}
	return _T("");
}

void CGlobal::LoadIcons()
{
	DeleteIcons();

	CString strFileName(_T(""));
	CString sModulePath = GetModulePath() + _T("\\");

	for(UINT i=0; i<IDX_ICON_MAX; i++)
	{
		strFileName = GetIconFileName(i);
		if( strFileName.IsEmpty() )
			continue;
		
		strFileName = sModulePath + strFileName;

		m_hIcon[i][0] = (HICON)::LoadImage(::AfxGetInstanceHandle(), strFileName, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		m_hIcon[i][1] = (HICON)::LoadImage(::AfxGetInstanceHandle(), strFileName, IMAGE_ICON, 24, 24, LR_LOADFROMFILE);
		m_hIcon[i][2] = (HICON)::LoadImage(::AfxGetInstanceHandle(), strFileName, IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	}
}

CString CGlobal::GetIconFileName(UINT nIconID)
{
	switch( nIconID )
	{
	case IDX_ICON_SKIN							:	return _T("skin\\skin.ico");					break;
	case IDX_ICON_SKIN_INACTIVE					:	return _T("skin\\skin_inactive.ico");			break;
	case IDX_ICON_OK							:	return _T("skin\\ok.ico");						break;
	case IDX_ICON_CANCEL						:	return _T("skin\\close.ico");					break;
	case IDX_ICON_HOME							:	return _T("skin\\home.ico");					break;
	case IDX_ICON_FAVORITES						:	return _T("skin\\favorites.ico");				break;
	case IDX_ICON_INTERNET						:	return _T("skin\\internet.ico");				break;
	case IDX_ICON_SETTING						:	return _T("skin\\setting.ico");					break;
	}
	return _T("");
}

SIZE CGlobal::GetBitmapSize(UINT nBitmapID)
{
	SIZE size;
	if( nBitmapID >= IDX_BMP_MAX )
	{
		size.cx=0;
		size.cy=0;
		return size;
	}

	size.cx = m_infoBitmap[nBitmapID].bitmap.bmWidth;
	size.cy = m_infoBitmap[nBitmapID].bitmap.bmHeight;
	return size;
}

HBITMAP CGlobal::GetBitmap(UINT nBitmapID)
{
	if( nBitmapID >= IDX_BMP_MAX )
		return NULL;

	return m_infoBitmap[nBitmapID].hBitmap;
}

HICON CGlobal::GetIcon(UINT nIconID, UINT nType)
{
	if( nIconID >= IDX_ICON_MAX ||
		nType > ICON32 )
		return NULL;

	return m_hIcon[nIconID][nType];
}

void CGlobal::DeleteFonts()
{
	for(UINT i=0; i<IDX_FONT_MAX; i++)
	{
		if( m_pFont[i] ) 
		{
			m_pFont[i]->DeleteObject();
			delete m_pFont[i];
			m_pFont[i] = NULL;
		}
	}
}

void CGlobal::SetRGB(UINT nRGBID, COLORREF cf)
{
	if( nRGBID >= IDX_RGB_MAX )
		return;

	m_cf[nRGBID] = cf;
}

COLORREF CGlobal::GetRGB(UINT nRGBID)
{
	if( nRGBID >= IDX_RGB_MAX )
		return RGB(0,0,0);

	return m_cf[nRGBID];
}

CFont * CGlobal::GetFont(UINT nFontID) 
{ 
	if( nFontID >= IDX_FONT_MAX )
		return NULL;

	return m_pFont[nFontID]; 
}

CFont * CGlobal::SetFont(UINT nFontID, CString sFaceName, UINT nFontSize, BOOL bBold/*=FALSE*/)
{
	if( nFontID >= IDX_FONT_MAX )
		return NULL;

	CWnd *pMain = AfxGetMainWnd();
	if( !pMain )
		return NULL;	

	// Font Name
	LOGFONT logFont;
	ZeroMemory(&logFont,sizeof(logFont));
	_tcscpy( logFont.lfFaceName, sFaceName );

	// Font Style
	logFont.lfItalic = FALSE;
	if( bBold )
		logFont.lfWeight = FW_BOLD;
	else
		logFont.lfWeight = FW_NORMAL;

	// Font Size
	HDC hdc = ::GetDC(pMain->m_hWnd);
	if( !hdc ) 
		logFont.lfHeight = nFontSize;
	else 
		logFont.lfHeight = -MulDiv(nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	::ReleaseDC(pMain->m_hWnd, hdc);

	// New Font
	if( m_pFont[nFontID] ) 
	{
		m_pFont[nFontID]->DeleteObject();
		delete m_pFont[nFontID];
		m_pFont[nFontID] = NULL;
	}

	m_pFont[nFontID] = (CFont *)new CFont;	
	if( !m_pFont[nFontID]->CreateFontIndirect( &logFont ) )
	{
		delete m_pFont[nFontID];
		m_pFont[nFontID] = NULL;
	}

	return m_pFont[nFontID];
}

CSize CGlobal::FormatText(CString& sText, CDC* pDC, int nWidth)
{
	CRect rect(0, 0, nWidth, 20);
	UINT uFlags = DT_CALCRECT | DT_SINGLELINE | DT_MODIFYSTRING | DT_END_ELLIPSIS;

	::DrawText(pDC->GetSafeHdc(), sText.GetBuffer(sText.GetLength() + 4), -1, rect, uFlags);
	sText.ReleaseBuffer();

	return pDC->GetTextExtent(sText);
}

COLORREF CGlobal::Darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF CGlobal::Lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor > 1.0f);

	fFactor = max(fFactor, 1.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}