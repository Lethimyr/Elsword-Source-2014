#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "KTDGFontVertex.h"
#include "KTDGDynamicTexture.h"

#ifdef SUPPORT_THAI_FONT
	#include "KTDGFontUtil.h"
	#include <boost/unordered_map.hpp>
#endif SUPPORT_THAI_FONT

//#define MAX_FONT_SIZE				(256)
//#define FONT_CACHE_SIZE				(512)
#ifdef FONT_CASH_DATA_STRUCTURE_REFORM
#define HASH_BUCKET_SIZE			65536
#endif FONT_CASH_DATA_STRUCTURE_REFORM

#define MAX_FONT_SIZE				(128)
#define FONT_CACHE_SIZE				(256)
#define FONT_CACHE_PAGE				(4)
#define VIRTUAL_FONT_LINE_MAX		(3000)

//#ifndef DYNAMIC_VERTEX_BUFFER_OPT
//	#define	MAX_CHAR_IN_PAGE			(((int)FONT_CACHE_SIZE/12)*((int)FONT_CACHE_SIZE/12))		//12 X 12를 최소로 생각함
//	#define MAX_VERTEX_IN_PAGE			(MAX_CHAR_IN_PAGE*6)
//#endif

#define DT_NO_COLOR					(0xF0000000)

//#define _16BIT_FONT
#ifdef _16BIT_FONT
//D3DCOLOR_A1R5G5B5
#define MYD3DCOLOR_ARGB(a,r,g,b)	(WORD)( ((a&0x1) << 15) | ((r&0x1F)<<10) | ((g&0x1F) << 5) | (b&0x1F) )
#define MYFONTCOLORFMT				(D3DFMT_A1R5G5B5)
#define MYFONTCOLOR_PTR				WORD*
#define MYFONTCOLOR_ALPHA(col)		( ( col & (0x1 << 15) ) >> 15 )
#else
#define MYD3DCOLOR_ARGB(a,r,g,b)	D3DCOLOR_ARGB(a,r,g,b)
#define MYFONTCOLORFMT				(D3DFMT_A8R8G8B8)
#define MYFONTCOLOR_PTR				DWORD*
#define MYFONTCOLOR_ALPHA(col)		( ( col & 0xff000000 ) >> 24 )
#endif

#ifdef _HV_FONT_SORT_
// DT_HVCENTER 은 DT_CENTER(이진수 0001)와 DT_VCENTER(이진수 0100) 비트 연산자 OR 로 구성된 값으로 5(0101)
// DT_HVCENTER 는 DT_CENTER와 DT_VCENTER를 개별로 비트 연산자 | 하면 true 나와야 하는 값으로 구성해야 함
#define DT_HVCENTER		( DT_CENTER + DT_VCENTER )
#endif _HV_FONT_SORT_

struct SGCFontCache
{
	static float PixelToTex;

	int iLeft, iTop;	
#ifdef SUPPORT_THAI_FONT
	std::wstring _Char;
#else SUPPORT_THAI_FONT
	WCHAR _Char;
#endif SUPPORT_THAI_FONT
	DWORD dwLastUse;
	int iWidth,iHeight;
	int	iPage;

	float _texLeft;
	float _texRight;
	float _texTop;
	float _texBottom;

    SGCFontCache()
        : iLeft( 0 )
        , iTop( 0 )
#ifdef SUPPORT_THAI_FONT
		, _Char( L"" )
#else SUPPORT_THAI_FONT
        , _Char( 0 )
#endif SUPPORT_THAI_FONT
        , dwLastUse( 0 )
        , iWidth( 0 )
        , iHeight( 0 )
        , iPage( 0 )
		, _texLeft( 0.f )
    	, _texRight( 0.f )
    	, _texTop( 0.f )
		, _texBottom( 0.f )
	{
	}

	void SetPos(int x, int y, int iFontSize)
	{
		iLeft = x * (iFontSize);
		iTop  = y * iFontSize;

		_texLeft = PixelToTex * ((float)iLeft + 0.5f);
		_texTop = PixelToTex * ((float)iTop + 0.5f);		
	}

	void _SetRect(int width, int height)
	{
		iWidth = width;
		iHeight = height;

		_texRight = PixelToTex * ((float)(iLeft + iWidth) + 0.5f);
		_texBottom = PixelToTex * ((float)(iTop + iHeight) + 0.5f);
	}
};

class CKTDGDeviceFont
{
public:
	enum
	{
		DEFAULT_FONT_SIZE   = 12,
		LARGE_FONT_SIZE     = 16,
		TITLE_FONT_SIZE     = 32,
	};
public:
	/* 3배 이상 확대해서 쓸 경우 문제 생길 수 있음.. 자 해결해주세요 ㅎ */
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	CKTDGDeviceFont(std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1, bool bNoRes = false );
//#else
//	CKTDGDeviceFont(std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1 );
//#endif
	virtual ~CKTDGDeviceFont(void);

	void OnResetDevice();
	void OnLostDevice();

	bool Load();
	void UnLoad(bool bComplete = true);

#ifdef KTDGDEVICEFONT_BACKGROUND_PRELOADING
	void PreloadFont( const WCHAR* pwstr );
	bool IsFontLoaded() const { return m_bLoad; }
	bool IsFontPreloaded() const { return m_bPreload; }
#endif

	static std::string MakeFontID( std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL );

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	HDC GetFontDC()
	{
		return m_hDC;
	}

	int	GetAdjustedFontSize()
	{
		return m_iFontSize;
	}
	int GetOrgFontSize()
	{
		return m_iOrgFontSize;
	}
//#endif

#ifdef  KTDGDEVICEFONT_SIZE_CACHE
    void            _EstimateFontSize( WCHAR wChar, SIZE* pSize ) const;
    int             _EstimateStringWidth( const WCHAR* pwStr, int iSize ) const;
    int             _EstimateStringHeight( const WCHAR* pwStr, int iSize ) const;
#endif  

	__forceinline int GetFontSize()
	{
		return (m_iFontSize/m_EnlargeNum) + m_iOutLineSize*2;
	}
#ifdef SUPPORT_THAI_FONT
	__forceinline int GetWidth( WCHAR wLetter )
	{
		//	성조 문자면 길이 0
		if( KTDGUTIL_STR_THAI::IsIntonation( wLetter ) == true )
			return 0;

		std::wstring wstr;
		wstr.clear();
		wstr += wLetter;
		return GetWidthLetter( wstr );
	}
#else SUPPORT_THAI_FONT
	__forceinline int GetWidth( WCHAR wLetter ) const
	{
#ifndef NUMBER_TO_LANGUAGE
		ABCFLOAT abcFloat;    
		UINT uiChar = wLetter;  
		GetCharABCWidthsFloatW(m_hDC, uiChar, uiChar, &abcFloat);  

		return static_cast<int>( (abcFloat.abcfA + abcFloat.abcfB + abcFloat.abcfC) / m_EnlargeNum + (m_iOutLineSize * 2) - 1 );
#else
		SIZE size;
#ifdef  KTDGDEVICEFONT_SIZE_CACHE
        _EstimateFontSize( wLetter, &size );
#else   KTDGDEVICEFONT_SIZE_CACHE
		GetTextExtentPoint32W( m_hDC, &wLetter, 1, &size);	
#endif  KTDGDEVICEFONT_SIZE_CACHE
		return (int)(size.cx/m_EnlargeNum) + (m_iOutLineSize * 2) - 1;
#endif NUMBER_TO_LANGUAGE
	}
#endif SUPPORT_THAI_FONT
	__forceinline int GetHeight( WCHAR wLetter ) const
	{
		SIZE size;
#ifdef  KTDGDEVICEFONT_SIZE_CACHE
        _EstimateFontSize( wLetter, &size );
#else   KTDGDEVICEFONT_SIZE_CACHE
		GetTextExtentPoint32W( m_hDC, &wLetter, 1, &size);	
#endif  KTDGDEVICEFONT_SIZE_CACHE
		return (int)(size.cy/m_EnlargeNum);
	}
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	__forceinline int GetWidth( const WCHAR* wszText, int nCount = -1 );
//#else
//	__forceinline int GetWidth( const WCHAR* wszText )
//		{
//		KTDXPROFILE();
//
//		if( NULL == wszText )
//			return 0;
//
//		int iLen = (int)wcslen(wszText);
//		int iCur = 0;
//		int	maxWidth = 0;
//
//		m_wstrBuffer.resize(0);
//		for(int i = 0; i < iLen; i++ )
//		{
//			if ( wszText[i] == '\n' )
//			{
//				SIZE size;
//				int iLength = m_wstrBuffer.size();
//				GetTextExtentPoint32W( m_hDC, m_wstrBuffer.c_str(), iLength, &size);	
//				int tempWidth = (int)(size.cx/m_EnlargeNum)  + (iLength * m_iOutLineSize * 2) - iLength;
//				if ( tempWidth > maxWidth )
//					maxWidth = tempWidth;
//
//				m_wstrBuffer.resize(0);
//				continue;
//			}
//
//
//			if( wszText[i] == L'#' && i < iLen -1 && ( wszText[i+1] == L'c' || wszText[i+1] == L'C' ) )
//			{
//				if( wszText[i+2] == 'x' || wszText[i+2] == 'X' )
//				{
//					i += 2;
//				}
//				else
//				{
//					i += 7;
//				}
//				continue;
//			}
//			
//			if ( wszText[i] != '\0' )
//				m_wstrBuffer += wszText[i];
//		}
//		/*
//		SIZE size;
//		int iLength = wcslen(strBuf.c_str());
//		GetTextExtentPoint32W( m_hDC, strBuf.c_str(), iLength, &size);	
//		//SAFE_DELETE_ARRAY( strBuf );
//		return (int)size.cx  + iLength * m_iOutLineSize * 2;
//		*/
//		SIZE size;
//		int iLength = m_wstrBuffer.size();
//		GetTextExtentPoint32W( m_hDC, m_wstrBuffer.c_str(), iLength, &size);	
//		int tempWidth = (int)(size.cx/m_EnlargeNum)  + (iLength * m_iOutLineSize * 2)  - iLength;
//		if ( tempWidth > maxWidth )
//			maxWidth = tempWidth;
//
//		m_wstrBuffer.resize(0);
//
//		return maxWidth;
//	}
//#endif

#ifdef SUPPORT_THAI_FONT
	__forceinline int GetWidthLetter( std::wstring wLetter )
	{
		SGCFontCache *cache = GetFontCache( wLetter );
		if( cache == NULL )
			return 0;

		return cache->iWidth;// + m_iOutLineSize * 2;;
	}
#endif SUPPORT_THAI_FONT
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	__forceinline int GetHeight( const WCHAR* wszText, int nCount = -1 ) const
	{
		KTDXPROFILE();

		SIZE size;

		int iSize = ( nCount < 0 ) ? (int) wcslen(wszText) : nCount;
#ifdef  KTDGDEVICEFONT_SIZE_CACHE
        size.cy = _EstimateStringHeight( wszText, iSize );
#else   KTDGDEVICEFONT_SIZE_CACHE
		GetTextExtentPoint32W( m_hDC, wszText, iSize, &size);
#endif  KTDGDEVICEFONT_SIZE_CACHE
		return (int)(size.cy/m_EnlargeNum);
	}
//#else
//	__forceinline int GetHeight( const WCHAR* wszText ) const
//	{
//		KTDXPROFILE();
//
//		SIZE size;
//		GetTextExtentPoint32W( m_hDC, wszText, (int)wcslen(wszText), &size);	
//		return (int)(size.cy/m_EnlargeNum);
//	}
//#endif

	int GetHeight()
	{
		return (m_iFontSize/m_EnlargeNum);
	}

	__forceinline int GetPureWidth( const WCHAR* wszText ) const
	{
		SIZE size;
		int iLen = (int)wcslen(wszText);
#ifdef  KTDGDEVICEFONT_SIZE_CACHE
        size.cx = _EstimateStringWidth( wszText, iLen );
#else   KTDGDEVICEFONT_SIZE_CACHE
		GetTextExtentPoint32W( m_hDC, wszText, iLen, &size);
#endif  KTDGDEVICEFONT_SIZE_CACHE
		return size.cx;
	}

	/// draw text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
//#else
//	__forceinline void OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP )
//	{
//		KTDXPROFILE();
//
//		if( wszText == NULL || wcslen(wszText) == 0 )
//			return;
//
//		int iFinalLeft = GetLeftPos( iLeft, wszText, pRt, dwFlag );
//#ifdef USE_DT_VCENTER
////{{ 2010. 11. 16 성현찬, 문자열 수직정렬
//		int iFinalTop = GetTopPos( iTop, wszText, pRt, dwFlag);
////}}
//#endif // USE_DT_VCENTER
//		m_dwCurrentStamp = timeGetTime();
//
//		for(int i = 0; i < FONT_CACHE_PAGE; i++)
//		{
//			if ( m_pTexture[i] != NULL )
//				m_pTexture[i]->SetChange(false);
//
//			if( m_pTextureOutLine[i] != NULL )
//				m_pTextureOutLine[i]->SetChange(false);
//		}
//
//		D3DCOLOR curcol;
//
//		if( m_iOutLineSize )
//		{
//			curcol = colorOutLine;
//#ifdef USE_DT_VCENTER
////{{ 2010. 11. 16 성현찬, 문자열 수직정렬
//			Buffering(iFinalLeft, iFinalTop, (WCHAR*)wszText, curcol, colorOutLine, true );
////}}
//#else // USE_DT_VCENTER
//			Buffering(iFinalLeft, iTop, (WCHAR*)wszText, curcol, colorOutLine, true );
//#endif // USE_DT_VCENTER
//			Flush(true);
//		}
//
//		curcol = color;
//#ifdef USE_DT_VCENTER
////{{ 2010. 11. 16 성현찬, 문자열 수직정렬
//		Buffering(iFinalLeft, iFinalTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0 );
////}}
//
//#else // USE_DT_VCENTER
//		Buffering(iFinalLeft, iTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0 );
//#endif // USE_DT_VCENTER
//		Flush(false);	
//	}
//#endif
	

	/// draw multi-lined text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
	/// @param  fLineSpace: line space ratio
	///         - ex) fLineSpace == 2.f means double space
	/// @note   we use '\n' and "\\n" as line breaking special characters
	///         so "Hello\nWorld" and "Hello\\nWorld" will be rendered same format.
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
//#else
//	__forceinline void OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP)
//	{
//		if( wszText == NULL || wcslen(wszText) == 0 )
//			return;
//
//		m_dwCurrentStamp = timeGetTime();
//		for(int i = 0; i < FONT_CACHE_PAGE; i++)
//		{
//			m_pTexture[i]->SetChange(false);
//			if( m_pTextureOutLine[i] )
//			{
//				m_pTextureOutLine[i]->SetChange(false);
//			}
//		}
//
//		static WCHAR			wstrBuf[VIRTUAL_FONT_LINE_MAX];
//
//#ifdef	CONVERSION_VS
//		StringCchCopy( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
//		//wcscpy_s( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
//#else	CONVERSION_VS
//		wcscpy( wstrBuf, wszText );
//#endif	CONVERSION_VS
//
//		WCHAR*			pToken = wstrBuf;
//		int iSize       = (int)wcslen(wstrBuf);
//		bool bNewLine = false;
//#ifdef USE_DT_VCENTER
////{{ 2010. 11. 16 성현찬, 문자열 수직정렬
//		int iTopLocal = GetTopPos( iTop, wszText, pRt, dwFlag, fLineSpace );
////}}
//#else // USE_DT_VCENTER
//		int iTopLocal = iTop;
//#endif // USE_DT_VCENTER
//
//		D3DCOLOR curcol;
//
//		if( m_iOutLineSize )
//		{
//			curcol = colorOutLine;
//
//			for(int i = 0; i < iSize; i++)
//			{
//				if( wstrBuf[i] == L'\n' )
//					bNewLine = true;
//
//				if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' || i == iSize - 1 )
//				{	
//					bNewLine = false;
//					if( wstrBuf[i] == L'\n' )
//						bNewLine = true;
//					if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' )
//						wstrBuf[i] = 0;
//
//					int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag );
//
//					Buffering(iLineLeft, iTopLocal, pToken, curcol, color, true );
//
//					if( bNewLine )
//					{
//						if( m_bRHW == true )
//							iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace);
//						else
//							iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace);
//					}
//
//					pToken = &wstrBuf[i+1];
//				}
//			}
//
//			Flush(true);
//		}
//
//#ifdef	CONVERSION_VS
//		StringCchCopy( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
//		//wcscpy_s( wstrBuf, _countof(wstrBuf), wszText );
//#else	CONVERSION_VS
//		wcscpy( wstrBuf, wszText );
//#endif	CONVERSION_VS
//
//		pToken = wstrBuf;
//		curcol = color;
//#ifdef USE_DT_VCENTER
////{{ 2010. 11. 16 성현찬, 문자열 수직정렬
//		iTopLocal = GetTopPos( iTop, wszText, pRt, dwFlag, fLineSpace );
////}}
//#else // USE_DT_VCENTER
//		iTopLocal = iTop;
//#endif // USE_DT_VCENTER		
//
//		for(int i = 0; i < iSize; i++)
//		{
//			if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' || i == iSize - 1 )
//			{	
//				bNewLine = false;
//				if( wstrBuf[i] == L'\n' )
//					bNewLine = true;
//				if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' )
//					wstrBuf[i] = 0;
//
//				int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag );
//
//				Buffering(iLineLeft, iTopLocal, pToken, curcol, color, (dwFlag & DT_NO_COLOR) != 0 );
//				//Display Here
//				if( bNewLine )
//				{
//					if( m_bRHW == true )
//						iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace);
//					else
//						iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum ) * fLineSpace);
//				}			
//
//				pToken = &wstrBuf[i+1];
//			}
//		}
//
//		Flush(false);
//	}
//#endif

private:
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	__forceinline int GetLeftPos( const int& iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag, int nCount = -1, float fScaleX = 1.f )
//#else
//	__forceinline int GetLeftPos( const int& iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag )
//#endif
	{
		KTDXPROFILE();

		if ( pRt == NULL )
		{
			if( dwFlag & DT_CENTER )
			{
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;
				return iLeft - (int) ( fFontWidth * 0.5f );
//#else
//				int iFontWidth = GetWidth(wszText);
//				return iLeft - ( iFontWidth ) / 2;
//#endif
			}
			else if ( dwFlag & DT_RIGHT )
			{
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;
				return iLeft + (int) ( - fFontWidth + (m_iFontSize / m_EnlargeNum) );
//#else
//				int iFontWidth = GetWidth(wszText);
//				return iLeft - iFontWidth + (m_iFontSize / m_EnlargeNum);
//#endif
			}
			else
			{
				return iLeft;
			}
		}
		else if ( dwFlag & DT_LEFT )
		{
			return iLeft;
		}
		else
		{
			if( dwFlag & DT_CENTER )
			{
				int iWidth = pRt->right - pRt->left;

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;

				return pRt->left + (int) ( ( iWidth - fFontWidth ) * 0.5f );
//#else
//				int iFontWidth = GetWidth(wszText);
//
//				return pRt->left + ( iWidth - iFontWidth ) / 2;
//#endif
			}
			else if( dwFlag & DT_RIGHT )
			{
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;
				return pRt->right - (int) ( fFontWidth );
//#else
//				int iFontWidth = GetWidth(wszText);
//				return pRt->right - iFontWidth;
//#endif
			}
			else
			{
				return iLeft;
			}
		}

		return 0;
	}

#ifdef USE_DT_VCENTER	
//{{ 2010. 11. 16 성현찬, 문자열 수직정렬
	__forceinline int GetTopPos( const int& iTop, const WCHAR* wszText, RECT* pRt, DWORD dwFlag, float fLineSpace = 1.0f )
	{
		KTDXPROFILE();

		if ( pRt == NULL )
		{
			if( dwFlag & DT_VCENTER )
			{
				int enterNum = 1;

				if( NULL != wszText )
				{
					int stringIndex = 0;
					wstring stringData = wszText;


					while( 1 )
					{
						stringIndex = stringData.find( L"\n", stringIndex );
						if ( stringIndex == -1 )
							break;

						stringIndex++;
						enterNum++;
					}
				}

				int iFontHeight = enterNum * ((m_iFontCacheSize / m_EnlargeNum) * static_cast<int>(fLineSpace));

				return iTop - ( iFontHeight ) / 2;
			}
			else
			{
				return iTop;
			}
		}
		else
		{
			if( dwFlag & DT_VCENTER )
			{
				int iHeight = pRt->bottom - pRt->top;
				int enterNum = 1;

				if( NULL != wszText )
				{
					int stringIndex = 0;
					wstring stringData = wszText;


					while( 1 )
					{
						stringIndex = stringData.find( L"\n", stringIndex );
						if ( stringIndex == -1 )
							break;

						stringIndex++;
						enterNum++;
					}
				}

				int iFontHeight = enterNum * ((m_iFontCacheSize / m_EnlargeNum) * static_cast<int>(fLineSpace));

				return pRt->top + ( iHeight - iFontHeight ) / 2;
			}
			else
			{
				return iTop;
			}
		}
//}}
		return 0;
	}
#endif // USE_DT_VCENTER	

	void InitCache();
public:
#ifdef SUPPORT_THAI_FONT
	SGCFontCache* GetFontCache( std::wstring _Char );
#else SUPPORT_THAI_FONT
	SGCFontCache* GetFontCache(WCHAR _Char);
#endif SUPPORT_THAI_FONT
private:
	void CharToSystemTexture( SGCFontCache* cache);

#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
	__forceinline int GetRightPos( const int& iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag, int nCount = -1, float fScaleX = 1.f )
	{
		KTDXPROFILE();

		if( pRt == NULL )
		{
			return 0;
		}
		else if( pRt->right > pRt->left )
		{
			return pRt->right;
		}

		return 0;
	}
#endif //HIDE_FONT_OUT_OF_EDITBOX_SIZE

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
		void Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f, const int& iRight = -1  );
	#else //HIDE_FONT_OUT_OF_EDITBOX_SIZE
		void Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
	#endif //HIDE_FONT_OUT_OF_EDITBOX_SIZE
//#else
//	__forceinline void Buffering(const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor )
//	{
//		int isize = (int)wcslen(str);
//		float bufferingILeft = (float)iLeft;
//		D3DCOLOR bufferingColor = color;
//
//
//		for(int i = 0; i < isize; ++i )
//		{
//			if( str[i] == L'\n' )
//				continue;
//			if( str[i] == L'\r' )
//				continue;
//
//			// Color Change 		
//			if( str[i] == L'#' && i != isize-1 && (str[i+1] == L'C' || str[i+1] == L'c' ) )
//			{
//				if( str[i+2] == L'X' || str[i+2] == L'x' )
//				{
//					i += 2;
//					if( !bNoColor )
//						bufferingColor = DefColor;
//					continue;
//				}
//				else
//				{
//					if( !bNoColor )
//					{
//						// #C R G B형식으로 들어있음
//						// #CFF0000 -> Red
//						int iCol;
//						WCHAR strColor[7] = { 0, };
//
//#ifdef	CONVERSION_VS
//						wcsncpy_s(strColor, _countof(strColor), &str[i+2],6);
//						swscanf_s(strColor,L"%x",&iCol);
//#else	CONVERSION_VS
//						wcsncpy(strColor,&str[i+2],6);
//						swscanf(strColor,L"%x",&iCol);
//#endif	CONVERSION_VS
//
//						bufferingColor = ( bufferingColor & 0xFF000000 ) | iCol;
//					}
//				}			
//				i += 7;
//				continue;
//			}
//
//
//
//			SGCFontCache *cache = GetFontCache(str[i]);
//
//			int tempValueX = 0;
//			int tempValueY = 0;
//
//			if ( m_EnlargeNum > 1 )
//			{
//				tempValueX = ( (  cache->iWidth ) / (4*(m_EnlargeNum-1)) );
//				tempValueY = ( (  cache->iHeight  ) / (2*(m_EnlargeNum-1)) );
//			}
//
//
//			if( m_bRHW == true )
//			{
//				S2DUIVertex* pVertex = NULL;
//				pVertex = &m_pUIVertex[cache->iPage][m_iCandidate[cache->iPage] * 6];			
//				pVertex[0].SetVertexColor( bufferingILeft + tempValueX, (float)iTop, bufferingColor, cache->_texLeft, cache->_texTop );
//				pVertex[1].SetVertexColor( bufferingILeft + cache->iWidth - tempValueX, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
//				pVertex[2].SetVertexColor( bufferingILeft + tempValueX, (float)iTop + cache->iHeight - tempValueY, bufferingColor, cache->_texLeft, cache->_texBottom );
//
//				pVertex[3].SetVertexColor( bufferingILeft + cache->iWidth - tempValueX, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
//				pVertex[4].SetVertexColor( bufferingILeft + cache->iWidth - tempValueX, (float)iTop + cache->iHeight - tempValueY,	bufferingColor, cache->_texRight, cache->_texBottom);
//				pVertex[5].SetVertexColor( bufferingILeft + tempValueX, (float)iTop + cache->iHeight - tempValueY, bufferingColor, cache->_texLeft, cache->_texBottom );
//			}
//			else
//			{
//				SLVERTEX* pVertex = NULL;
//				pVertex = &m_pVertex[cache->iPage][m_iCandidate[cache->iPage] * 6];			
//				pVertex[0].SetVertexColor( bufferingILeft, (float)iTop, bufferingColor, cache->_texLeft, cache->_texTop );
//				pVertex[1].SetVertexColor( bufferingILeft + cache->iWidth, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
//				pVertex[2].SetVertexColor( bufferingILeft, (float)iTop - cache->iHeight, bufferingColor, cache->_texLeft, cache->_texBottom );
//
//				pVertex[3].SetVertexColor( bufferingILeft + cache->iWidth, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
//				pVertex[4].SetVertexColor( bufferingILeft + cache->iWidth, (float)iTop - cache->iHeight, bufferingColor, cache->_texRight, cache->_texBottom);
//				pVertex[5].SetVertexColor( bufferingILeft, (float)iTop - cache->iHeight, bufferingColor, cache->_texLeft, cache->_texBottom );
//			}
//
//			bufferingILeft += ( ( cache->iWidth /m_EnlargeNum ) - 1);
//			m_iCandidate[cache->iPage]++;
//		}	
//	}
//#endif
	
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void Flush( bool bOutLine, int iPage = -1 );
//#else
//	void Flush(bool bOutLine);
//#endif

private:
	std::string		m_strFontName;
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	bool            m_bNoRes;
//#endif
	int				m_iFontSize;
	int				m_iOrgFontSize;
	int				m_iFontWeight;
	int				m_iOutLineSize;
	int				m_iFontCacheSize;		// m_iFontSize + m_iOutLineSize * 2

	bool			m_bRHW;

	CKTDGDynamicTexture*	m_pTexture[FONT_CACHE_PAGE];
	CKTDGDynamicTexture*	m_pTextureOutLine[FONT_CACHE_PAGE];

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void*		        m_apPageVertex[FONT_CACHE_PAGE];
//#else
//	S2DUIVertex			m_pUIVertex[FONT_CACHE_PAGE][MAX_VERTEX_IN_PAGE];
//	SLVERTEX			m_pVertex[FONT_CACHE_PAGE][MAX_VERTEX_IN_PAGE];
//#endif

	int					m_iTotalCache;	
    int                 m_iNumCandidatesInAPage;
	int					m_iCandidate[FONT_CACHE_PAGE];
	bool				m_bLoad;
#ifdef KTDGDEVICEFONT_BACKGROUND_PRELOADING
	bool                m_bPreload;
#endif
	
	DWORD				m_dwCurrentStamp;
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	unsigned            m_uDCFontCount;
//#endif

#ifdef FONT_CASH_DATA_STRUCTURE_REFORM
	int					m_iSizeArray;
	SGCFontCache**	m_FontCacheArray;
#ifdef  KTDGDEVICEFONT_SIZE_CACHE
    struct  FontHash
    {
        SGCFontCache*   m_pFontCache;
        bool            m_bFontSize;
		SIZE            m_size;
        FontHash()
            : m_pFontCache( NULL )
            , m_bFontSize( false )
        {
            m_size.cx = m_size.cy = 0;
        }
        void            Init()
        {
            m_pFontCache = NULL;
            m_bFontSize = false;
            m_size.cx = m_size.cy = 0;
        }
    };//struct  FontHash

	mutable FontHash    m_FontCacheHashTable[HASH_BUCKET_SIZE];

#else

	SGCFontCache*	m_FontCacheHashTable[HASH_BUCKET_SIZE];
#endif //KTDGDEVICEFONT_SIZE_CACHE

#else //FONT_CASH_DATA_STRUCTURE_REFORM

#ifdef SUPPORT_THAI_FONT
	boost::unordered_map< std::wstring , SGCFontCache* > m_mapChar;
#else //SUPPORT_THAI_FONT
	std::map< WCHAR, SGCFontCache* > m_mapChar;
#endif //SUPPORT_THAI_FONT

#endif //FONT_CASH_DATA_STRUCTURE_REFORM

	// about DC
	HPEN				m_hPen, m_hOldPen;
	HBITMAP				m_hBitmap, m_hOldBitmap;
	HDC					m_hDC;
	HFONT				m_hFont;
	HFONT				m_hOldFont;

//{{ robobeg : 2008-10-13
	CKTDGStateManager::KStateID m_RenderStateID;
//}} robobeg : 2008-10-13
	int					m_EnlargeNum;

	std::wstring		m_wstrBuffer;			// 내부적으로 계산을 위해서 사용하는 string buffer
};
