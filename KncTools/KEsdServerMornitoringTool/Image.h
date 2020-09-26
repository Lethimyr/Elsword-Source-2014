// Image.h: interface for the CImage class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_IMAGE_H__EF814475_1A4F_4B64_8299_620D3C3DBF0A__INCLUDED_)
//#define AFX_IMAGE_H__EF814475_1A4F_4B64_8299_620D3C3DBF0A__INCLUDED_
//
//#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000


#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "ijl.h"

#define MAX_TABLE 500

class CImage  
{
public:
	void Init(HWND);	
	void SaveBitmap(const wchar_t*);
	void CreateImage_ForDay(HDC, const wchar_t*, const int*, int);
	void CreateImage_ForMonth(const wchar_t*, wchar_t*[30], const int*, int, int);
	void CreateImage_ModeCount(const wchar_t*, wchar_t*[128][2], int, int, int);
	void createImage_FrameRate(const wchar_t*, wchar_t*[30], double[6][30], int, int, int);


	CImage();
	virtual ~CImage();

private:
	void DDBToDIB();
	void Delete();

private:
	HWND m_hWnd;
	HBITMAP m_hBitmap;				//그래프 생성용 비트맵핸들
	int m_pMaxTable[MAX_TABLE];		//그래프 Y축 상한값 테이블

	BITMAPFILEHEADER m_stFileHeader;
	BITMAPINFOHEADER m_stInfoHeader;
	BITMAPINFO *m_pstInfo;
	DWORD m_dwSize;
	int m_nPalSize;
};

//#endif // !defined(AFX_IMAGE_H__EF814475_1A4F_4B64_8299_620D3C3DBF0A__INCLUDED_)
