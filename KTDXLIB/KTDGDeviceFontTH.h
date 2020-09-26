#pragma once
#pragma warning( disable : 4996 )

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "KTDGFontVertex.h"
#include "KTDGDynamicTexture.h"
#include "KTDGFontUtil.h"

#include <boost/unordered_map.hpp>

//#define MAX_FONT_SIZE				(256)
//#define FONT_CACHE_SIZE				(512)
#ifdef FONT_CASH_DATA_STRUCTURE_REFORM
#define HASH_BUCKET_SIZE			65536
#endif FONT_CASH_DATA_STRUCTURE_REFORM

#define MAX_FONT_SIZE				(128)
#define FONT_CACHE_SIZE				(256)
#define FONT_CACHE_PAGE				(4)
#define VIRTUAL_FONT_LINE_MAX		(3000)

#ifndef DYNAMIC_VERTEX_BUFFER_OPT
	#define	MAX_CHAR_IN_PAGE			(((int)FONT_CACHE_SIZE/12)*((int)FONT_CACHE_SIZE/12))		//12 X 12를 최소로 생각함
	#define MAX_VERTEX_IN_PAGE			(MAX_CHAR_IN_PAGE*6)
#endif

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

struct SGCFontCacheTH
{
	static float PixelToTex;

	int iLeft, iTop;	
	std::wstring _Char;
	DWORD dwLastUse;
	int iWidth,iHeight;
	int	iPage;

	float _texLeft;
	float _texRight;
	float _texTop;
	float _texBottom;

    SGCFontCacheTH()
        : iLeft( 0 )
        , iTop( 0 )
        , _Char( L"" )
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

class CKTDGDeviceFontTH
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
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	CKTDGDeviceFontTH(std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1, bool bNoRes = false );
#else
	CKTDGDeviceFontTH(std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1 );
#endif
	virtual ~CKTDGDeviceFontTH(void);

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

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
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
#endif

	__forceinline int GetFontSize()
	{
		return (m_iFontSize/m_EnlargeNum) + m_iOutLineSize*2;
	}
	int GetWidth( WCHAR wchar );
	int GetWidthLetter( std::wstring wLetter );
	int GetHeight() const;
	int GetHeight( WCHAR wLetter ) const;
	int GetWidth( const WCHAR* wszText, int nCount = -1 );
	int GetHeight( const WCHAR* wszText, int nCount = -1 ) const;
	
	__forceinline int GetPureWidth( const WCHAR* wszText )
	{
		SIZE size;
		int iLen = (int)wcslen(wszText);

		GetTextExtentPoint32W( m_hDC, wszText, iLen, &size);

		return size.cx;
	}

	/// draw text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
#else
	__forceinline void OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP )
	{
		KTDXPROFILE();

		if( wszText == NULL || wcslen(wszText) == 0 )
			return;

		int iFinalLeft = GetLeftPos( iLeft, wszText, pRt, dwFlag );
		m_dwCurrentStamp = timeGetTime();

		for(int i = 0; i < FONT_CACHE_PAGE; i++)
		{
			if ( m_pTexture[i] != NULL )
				m_pTexture[i]->SetChange(false);

			if( m_pTextureOutLine[i] != NULL )
				m_pTextureOutLine[i]->SetChange(false);
		}

		D3DCOLOR curcol;

		if( m_iOutLineSize )
		{
			curcol = colorOutLine;
			Buffering(iFinalLeft, iTop, (WCHAR*)wszText, curcol, colorOutLine, true );
			Flush(true);
		}

		curcol = color;
		Buffering(iFinalLeft, iTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0 );
		Flush(false);	
	}
#endif
	

	/// draw multi-lined text.
	/// @param  iLeft: left-position of first char of text string(screen coordinate)
	/// @param  iTop: top-position of first char of text string
	/// @param  pszText: [in] output text
	/// @param  fLineSpace: line space ratio
	///         - ex) fLineSpace == 2.f means double space
	/// @note   we use '\n' and "\\n" as line breaking special characters
	///         so "Hello\nWorld" and "Hello\\nWorld" will be rendered same format.
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
#else
	__forceinline void OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP)
	{
		if( wszText == NULL || wcslen(wszText) == 0 )
			return;

		m_dwCurrentStamp = timeGetTime();
		for(int i = 0; i < FONT_CACHE_PAGE; i++)
		{
			m_pTexture[i]->SetChange(false);
			if( m_pTextureOutLine[i] )
			{
				m_pTextureOutLine[i]->SetChange(false);
			}
		}

		static WCHAR			wstrBuf[VIRTUAL_FONT_LINE_MAX];

#ifdef	CONVERSION_VS
		StringCchCopy( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
		//wcscpy_s( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
#else	CONVERSION_VS
		wcscpy( wstrBuf, wszText );
#endif	CONVERSION_VS

		WCHAR*			pToken = wstrBuf;
		int iSize       = (int)wcslen(wstrBuf);
		bool bNewLine = false;
		int iTopLocal = iTop;

		D3DCOLOR curcol;

		if( m_iOutLineSize )
		{
			curcol = colorOutLine;

			for(int i = 0; i < iSize; i++)
			{
				if( wstrBuf[i] == L'\n' )
					bNewLine = true;

				if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' || i == iSize - 1 )
				{	
					bNewLine = false;
					if( wstrBuf[i] == L'\n' )
						bNewLine = true;
					if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' )
						wstrBuf[i] = 0;

					int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag );

					Buffering(iLineLeft, iTopLocal, pToken, curcol, color, true );

					if( bNewLine )
					{
						if( m_bRHW == true )
							iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace);
						else
							iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace);
					}

					pToken = &wstrBuf[i+1];
				}
			}

			Flush(true);
		}

#ifdef	CONVERSION_VS
		StringCchCopy( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
		//wcscpy_s( wstrBuf, _countof(wstrBuf), wszText );
#else	CONVERSION_VS
		wcscpy( wstrBuf, wszText );
#endif	CONVERSION_VS

		pToken = wstrBuf;
		curcol = color;
		iTopLocal = iTop;

		for(int i = 0; i < iSize; i++)
		{
			if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' || i == iSize - 1 )
			{	
				bNewLine = false;
				if( wstrBuf[i] == L'\n' )
					bNewLine = true;
				if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' )
					wstrBuf[i] = 0;

				int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag );

				Buffering(iLineLeft, iTopLocal, pToken, curcol, color, (dwFlag & DT_NO_COLOR) != 0 );
				//Display Here
				if( bNewLine )
				{
					if( m_bRHW == true )
						iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace);
					else
						iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum ) * fLineSpace);
				}			

				pToken = &wstrBuf[i+1];
			}
		}

		Flush(false);
	}
#endif

private:
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	__forceinline int GetLeftPos( const int& iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag, int nCount = -1, float fScaleX = 1.f )
#else
	__forceinline int GetLeftPos( const int& iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag )
#endif
	{
		KTDXPROFILE();

		if ( pRt == NULL )
		{
			if( dwFlag & DT_CENTER )
			{
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText ) * fScaleX;
				return iLeft - (int) ( fFontWidth * 0.5f );
#else
				int iFontWidth = GetWidth(wszText);
				return iLeft - ( iFontWidth ) / 2;
#endif
			}
			else if ( dwFlag & DT_RIGHT )
			{
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText ) * fScaleX;
				return iLeft + (int) ( - fFontWidth + (m_iFontSize / m_EnlargeNum) );
#else
				int iFontWidth = GetWidth(wszText);
				return iLeft - iFontWidth + (m_iFontSize / m_EnlargeNum);
#endif
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

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText ) * fScaleX;

				return pRt->left + (int) ( ( iWidth - fFontWidth ) * 0.5f );
#else
				int iFontWidth = GetWidth(wszText);

				return pRt->left + ( iWidth - iFontWidth ) / 2;
#endif
			}
			else if( dwFlag & DT_RIGHT )
			{
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				float fFontWidth = GetWidth( wszText ) * fScaleX;
				return pRt->right - (int) ( fFontWidth );
#else
				int iFontWidth = GetWidth(wszText);
				return pRt->right - iFontWidth;
#endif
			}
			else
			{
				return iLeft;
			}
		}

		return 0;
	}

	void InitCache();
public:
	SGCFontCacheTH* GetFontCache(std::wstring _Char);
private:
	void CharToSystemTexture( SGCFontCacheTH* cache);

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
#else
	__forceinline void Buffering(const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor )
	{
		int isize = (int)wcslen(str);
		float bufferingILeft = (float)iLeft;
		D3DCOLOR bufferingColor = color;


		for(int i = 0; i < isize; ++i )
		{
			if( str[i] == L'\n' )
				continue;
			if( str[i] == L'\r' )
				continue;

			// Color Change 		
			if( str[i] == L'#' && i != isize-1 && (str[i+1] == L'C' || str[i+1] == L'c' ) )
			{
				if( str[i+2] == L'X' || str[i+2] == L'x' )
				{
					i += 2;
					if( !bNoColor )
						bufferingColor = DefColor;
					continue;
				}
				else
				{
					if( !bNoColor )
					{
						// #C R G B형식으로 들어있음
						// #CFF0000 -> Red
						int iCol;
						WCHAR strColor[7] = { 0, };

#ifdef	CONVERSION_VS
						wcsncpy_s(strColor, _countof(strColor), &str[i+2],6);
						swscanf_s(strColor,L"%x",&iCol);
#else	CONVERSION_VS
						wcsncpy(strColor,&str[i+2],6);
						swscanf(strColor,L"%x",&iCol);
#endif	CONVERSION_VS

						bufferingColor = ( bufferingColor & 0xFF000000 ) | iCol;
					}
				}			
				i += 7;
				continue;
			}



			SGCFontCache *cache = GetFontCache(str[i]);

			int tempValueX = 0;
			int tempValueY = 0;

			if ( m_EnlargeNum > 1 )
			{
				tempValueX = ( (  cache->iWidth ) / (4*(m_EnlargeNum-1)) );
				tempValueY = ( (  cache->iHeight  ) / (2*(m_EnlargeNum-1)) );
			}


			if( m_bRHW == true )
			{
				S2DUIVertex* pVertex = NULL;
				pVertex = &m_pUIVertex[cache->iPage][m_iCandidate[cache->iPage] * 6];			
				pVertex[0].SetVertexColor( bufferingILeft + tempValueX, (float)iTop, bufferingColor, cache->_texLeft, cache->_texTop );
				pVertex[1].SetVertexColor( bufferingILeft + cache->iWidth - tempValueX, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
				pVertex[2].SetVertexColor( bufferingILeft + tempValueX, (float)iTop + cache->iHeight - tempValueY, bufferingColor, cache->_texLeft, cache->_texBottom );

				pVertex[3].SetVertexColor( bufferingILeft + cache->iWidth - tempValueX, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
				pVertex[4].SetVertexColor( bufferingILeft + cache->iWidth - tempValueX, (float)iTop + cache->iHeight - tempValueY,	bufferingColor, cache->_texRight, cache->_texBottom);
				pVertex[5].SetVertexColor( bufferingILeft + tempValueX, (float)iTop + cache->iHeight - tempValueY, bufferingColor, cache->_texLeft, cache->_texBottom );
			}
			else
			{
				SLVERTEX* pVertex = NULL;
				pVertex = &m_pVertex[cache->iPage][m_iCandidate[cache->iPage] * 6];			
				pVertex[0].SetVertexColor( bufferingILeft, (float)iTop, bufferingColor, cache->_texLeft, cache->_texTop );
				pVertex[1].SetVertexColor( bufferingILeft + cache->iWidth, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
				pVertex[2].SetVertexColor( bufferingILeft, (float)iTop - cache->iHeight, bufferingColor, cache->_texLeft, cache->_texBottom );

				pVertex[3].SetVertexColor( bufferingILeft + cache->iWidth, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
				pVertex[4].SetVertexColor( bufferingILeft + cache->iWidth, (float)iTop - cache->iHeight, bufferingColor, cache->_texRight, cache->_texBottom);
				pVertex[5].SetVertexColor( bufferingILeft, (float)iTop - cache->iHeight, bufferingColor, cache->_texLeft, cache->_texBottom );
			}

			bufferingILeft += ( ( cache->iWidth /m_EnlargeNum ) - 1);
			m_iCandidate[cache->iPage]++;
		}	
	}
#endif
	
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void Flush( bool bOutLine, int iPage = -1 );
#else
	void Flush(bool bOutLine);
#endif

private:
	std::string		m_strFontName;
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	bool            m_bNoRes;
#endif
	int				m_iFontSize;
	int				m_iOrgFontSize;
	int				m_iFontWeight;
	int				m_iOutLineSize;
	int				m_iFontCacheSize;		// m_iFontSize + m_iOutLineSize * 2

	bool			m_bRHW;

	CKTDGDynamicTexture*	m_pTexture[FONT_CACHE_PAGE];
	CKTDGDynamicTexture*	m_pTextureOutLine[FONT_CACHE_PAGE];

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void*		        m_apPageVertex[FONT_CACHE_PAGE];
#else
	S2DUIVertex			m_pUIVertex[FONT_CACHE_PAGE][MAX_VERTEX_IN_PAGE];
	SLVERTEX			m_pVertex[FONT_CACHE_PAGE][MAX_VERTEX_IN_PAGE];
#endif

	int					m_iTotalCache;	
    int                 m_iNumCandidatesInAPage;
	int					m_iCandidate[FONT_CACHE_PAGE];
	bool				m_bLoad;
#ifdef KTDGDEVICEFONT_BACKGROUND_PRELOADING
	bool                m_bPreload;
#endif
	
	DWORD				m_dwCurrentStamp;
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	unsigned            m_uDCFontCount;
#endif

#ifdef  KTDGDEVICEFONT_SIZE_CACHE
    struct  FontHash
    {
        SGCFontCacheTH*   m_pFontCache;
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

	boost::unordered_map< std::wstring , SGCFontCacheTH* > m_mapTHChar;

#else

	SGCFontCache*	m_FontCacheHashTable[HASH_BUCKET_SIZE];
#endif

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
