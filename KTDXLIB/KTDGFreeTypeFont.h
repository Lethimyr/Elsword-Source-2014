#pragma once

// 이 클래스는 다음 디파인이 켜진 것을 기준으로 작성되었습니다.
// DYNAMIC_VERTEX_BUFFER_OPT
// KTDGDEVICEFONT_SIZE_CACHE
// CONVERSION_VS
// FONT_CASH_DATA_STRUCTURE_REFORM

#ifdef USE_FREE_TYPE

#include "KTDGDeviceFont.h"



class CKTDGFreeTypeFont
{
public:
	CKTDGFreeTypeFont( std::string strFontPath, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1, bool bNoRes = false );
	virtual ~CKTDGFreeTypeFont(void);

	void OnResetDevice();
	void OnLostDevice();

	bool Load();
	void UnLoad(bool bComplete = true);

	static std::string MakeFontID( std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL );

	int	GetAdjustedFontSize()
	{
		return m_iFontSize;
	}
	int GetOrgFontSize()
	{
		return m_iOrgFontSize;
	}

    void            _EstimateFontSize( WCHAR wChar, SIZE* pSize );
    int             _EstimateStringWidth( const WCHAR* pwStr, int iSize );
    int             _EstimateStringHeight( const WCHAR* pwStr, int iSize );

	__forceinline int GetFontSize()
	{
		return ( m_iFontSize / m_EnlargeNum ) + m_iOutLineSize * 2;
	}
	__forceinline int GetWidth( WCHAR wLetter )
	{
#ifndef NUMBER_TO_LANGUAGE
		ABCFLOAT abcFloat;
		UINT uiChar = wLetter;
		GetCharABCWidthsFloatW(m_hDC, uiChar, uiChar, &abcFloat);

		return static_cast<int>( (abcFloat.abcfA + abcFloat.abcfB + abcFloat.abcfC) / m_EnlargeNum + (m_iOutLineSize * 2) - 1 );
#else
		SIZE size;
        _EstimateFontSize( wLetter, &size );
		return (int)( size.cx / m_EnlargeNum ) + ( m_iOutLineSize * 2 ) - 1;
#endif NUMBER_TO_LANGUAGE
	}
	__forceinline int GetHeight( WCHAR wLetter )
	{
		SIZE size;
        _EstimateFontSize( wLetter, &size );
		return (int)( size.cy / m_EnlargeNum );
	}
	__forceinline int GetWidth( const WCHAR* wszText, int nCount = -1 );
	__forceinline int GetHeight( const WCHAR* wszText, int nCount = -1 )
	{
		KTDXPROFILE();

		SIZE size;

		int iSize = ( nCount < 0 ) ? (int) wcslen(wszText) : nCount;
        size.cy = _EstimateStringHeight( wszText, iSize );
		return (int)( size.cy / m_EnlargeNum );
	}

	int GetHeight()
	{
		// 안티-앨리어싱 때문에 +1
		return ( m_iFontSize / m_EnlargeNum ) + 1;
	}

	__forceinline int GetPureWidth( const WCHAR* wszText )
	{
		SIZE size;
		int iLen = (int)wcslen( wszText );
        size.cx = _EstimateStringWidth( wszText, iLen );
		return size.cx;
	}

	void OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
	void OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, D3DCOLOR colorOutLine = 0xff000000, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
	SGCFontCache* GetFontCache(WCHAR _Char);

private:
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
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE

	__forceinline int GetLeftPos( const int& iLeft, const WCHAR* wszText, RECT* pRt, DWORD dwFlag, int nCount = -1, float fScaleX = 1.f )
	{
		KTDXPROFILE();

		if ( pRt == NULL )
		{
			if( dwFlag & DT_CENTER )
			{
				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;
				return iLeft - (int) ( fFontWidth * 0.5f );
			}
			else if ( dwFlag & DT_RIGHT )
			{
				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;
				return iLeft + (int) ( - fFontWidth + (m_iFontSize / m_EnlargeNum) );
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

				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;
				return pRt->left + (int) ( ( iWidth - fFontWidth ) * 0.5f );
			}
			else if( dwFlag & DT_RIGHT )
			{
				float fFontWidth = GetWidth( wszText, nCount ) * fScaleX;
				return pRt->right - (int) ( fFontWidth );
			}
			else
			{
				return iLeft;
			}
		}

		return 0;
	}
	void InitCache();
	void CharToSystemTexture( SGCFontCache* cache );

#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
	void Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f, const int& iRight = -1 );
#else HIDE_FONT_OUT_OF_EDITBOX_SIZE
	void Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE
	void Flush( bool bOutLine, int iPage = -1 );
private:
	FT_Library		m_FreeTypeLibrary;
	FT_Face			m_FreeTypeFace;
#ifdef USE_FREE_TYPE_SUB_FONT
	FT_Face			m_FreeTypeSubFace;
#endif USE_FREE_TYPE_SUB_FONT
	FT_Face			m_KoreanFreeTypeFace;

	std::string		m_strFontName;
	bool            m_bNoRes;

	int				m_iFontSize;
	int				m_iOrgFontSize;
	int				m_iFontWeight;
	int				m_iOutLineSize;
	int				m_iFontCacheSize;		// m_iFontSize + m_iOutLineSize * 2

	bool			m_bRHW;

	CKTDGDynamicTexture*	m_pTexture[FONT_CACHE_PAGE];
	CKTDGDynamicTexture*	m_pTextureOutLine[FONT_CACHE_PAGE];

	void*		        m_apPageVertex[FONT_CACHE_PAGE];

	int					m_iTotalCache;	
	int                 m_iNumCandidatesInAPage;
	int					m_iCandidate[FONT_CACHE_PAGE];
	bool				m_bLoad;

	DWORD				m_dwCurrentStamp;
	unsigned            m_uDCFontCount;

	int					m_iSizeArray;
	SGCFontCache**		m_FontCacheArray;

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

	//{{ robobeg : 2008-10-13
	CKTDGStateManager::KStateID m_RenderStateID;
	//}} robobeg : 2008-10-13
	int					m_EnlargeNum;

	std::wstring		m_wstrBuffer;			// 내부적으로 계산을 위해서 사용하는 string buffer
};

#endif USE_FREE_TYPE