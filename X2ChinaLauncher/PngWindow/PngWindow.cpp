#include "stdafx.h"
#include "PngWindow.h"
#include "../X2ChinaLauncher.h"
#include "../LauncherBackgroundData.h"
#include "../LauncherBackgroundDataWeek1.h"
#include "../LauncherBackgroundDataWeek2.h"

time_t MakeTime( int iYear, int iMonth, int iDay, int iHour = 0 )
{
	struct tm imageChangeDay;
	imageChangeDay.tm_year = iYear - 1900;
	imageChangeDay.tm_mon = iMonth - 1;
	imageChangeDay.tm_mday = iDay;
	imageChangeDay.tm_hour = iHour;
	imageChangeDay.tm_min = 0;
	imageChangeDay.tm_sec = 0;
	time_t tImageChangeDay = mktime( &imageChangeDay );

	return tImageChangeDay;
}

void WindowAlphaBlendFromPNG(const char* pFileName, HWND wnd)
{
	CWaitCursor wait;

	HBITMAP hBitmap;
	LPBYTE pBitmapBits;
	LPVOID pImg= NULL;

	// Initalize m_pImgDecoder
	::ImgNewDecoder(&m_pImgDecoder);	

	time_t tCurrent;
	time( &tCurrent );

	//if( tCurrent > MakeTime( 2014, 1, 15, 10 ) )// 3번째 주
	{
		// first try to load with paintlib....
		// 2011.02.16 Lygan_조성욱 // 런처 이미지 수정하기 위해서는 BackgoundGenerator에서 런처이미지를 데이터 파일로 변환해주고 메모리에 복사하는 형식으로 진행 되어야함
		if(::ImgNewDIBFromMem( m_pImgDecoder, g_ucLauncherBackgroundData, g_iLauncherBackgroundDataSize, &pImg ) )
			return;
	}
	/*
	else if( tCurrent > MakeTime( 2013, 9, 11 ) )// 2번째 주
	{
		// first try to load with paintlib....
		// 2011.02.16 Lygan_조성욱 // 런처 이미지 수정하기 위해서는 BackgoundGenerator에서 런처이미지를 데이터 파일로 변환해주고 메모리에 복사하는 형식으로 진행 되어야함
		if( ::ImgNewDIBFromMem( m_pImgDecoder, g_ucLauncherBackgroundDataWeek2, g_iLauncherBackgroundDataSizeWeek2, &pImg ) )
			return;
	}
	*/
	/*
	else// 첫째 주
	{
		// first try to load with paintlib....
		// 2011.02.16 Lygan_조성욱 // 런처 이미지 수정하기 위해서는 BackgoundGenerator에서 런처이미지를 데이터 파일로 변환해주고 메모리에 복사하는 형식으로 진행 되어야함
		if( ::ImgNewDIBFromMem( m_pImgDecoder, g_ucLauncherBackgroundDataWeek1, g_iLauncherBackgroundDataSizeWeek1, &pImg ) )
			return;
	}
	*/



	::ImgGetHandle(pImg, &hBitmap, (LPVOID *)&pBitmapBits);

	// get the bitmap info
	BITMAP bmpInfo;
	::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);

	if (bmpInfo.bmBitsPixel != 32)
	{
		::ImgDeleteDIBSection(pImg);
		return;
	}

	//::ImgDeleteDIBSection(m_pImg); < I dont know why.. but it doesnt work. maybe object was perfectly removed?
	m_pImg= pImg;


	m_bmpDialog.DeleteObject();
	m_bmpDialog.Attach(hBitmap);

	// before using the UpdateLayeredWindow we have to 
	// pre-multiply the RGB channels with the alpha channel
	::PreMultiplyRGBChannels(m_bmpDialog, pBitmapBits);

	// now update dialog....
	UpdateDialog(wnd, m_bmpDialog);

	::ImgDeleteDecoder(m_pImgDecoder);
	::ImgDeleteDIBSection(m_pImg);
}

void PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits)
{
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// pre-multiply rgb channels with alpha channel
	for (int y=0; y<bmpInfo.bmHeight; ++y)
	{
		BYTE *pPixel= pBitmapBits + bmpInfo.bmWidth * 4 * y;

		for (int x=0; x<bmpInfo.bmWidth ; ++x)
		{
			pPixel[0]= pPixel[0]*pPixel[3]/255;
			pPixel[1]= pPixel[1]*pPixel[3]/255;
			pPixel[2]= pPixel[2]*pPixel[3]/255;

			pPixel+= 4;
		}
	}
}

void UpdateDialog(HWND wnd, CBitmap &bmp, BYTE SourceConstantAlpha)
{
	// make sure the window has the WS_EX_LAYERED style
	SetWindowLong(wnd, GWL_EXSTYLE, GetWindowLong(wnd, GWL_EXSTYLE) | WS_EX_LAYERED);


	// ok... into the per-pixel-alpha bendling....

	// Create/setup the DC's

	CDC dcScreen;
	CDC dcMemory;

	//dcScreen.Attach(::GetDC(NULL));
	dcScreen.Attach(::GetDC(wnd));
	dcMemory.CreateCompatibleDC(&dcScreen);

	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmp);


	// get the bitmap dimensions
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg;
	GetWindowRect(wnd, rectDlg);

	// calculate the new window position/size based on the bitmap size
	//CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CPoint ptWindowScreenPosition = CPoint(0, 0);
	CSize szWindow(bmpInfo.bmWidth, bmpInfo.bmHeight);


	// Perform the alpha blend

	// setup the blend function
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0); // start point of the copy from dcMemory to dcScreen

	// perform the alpha blend
	BOOL bRet= ::UpdateLayeredWindow(wnd, dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

	DWORD error =  GetLastError();
	ASSERT(bRet); // ops... something bad just occured....

	// clean up
	dcMemory.SelectObject(pOldBitmap);
}
