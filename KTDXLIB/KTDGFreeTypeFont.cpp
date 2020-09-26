#include "StdAfx.h"
#include "KTDGFreeTypeFont.h"
#include <algorithm>

#ifdef USE_FREE_TYPE

float SGCFontCache::PixelToTex = 1.0f / FONT_CACHE_SIZE;
#define PAGE_VERTEX_MULTIPLE    2

//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),

	KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
	KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),
	KTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ),

	KTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ),

	KSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ),
	KSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ),
	KSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR ),
};//akStates[]



CKTDGFreeTypeFont::CKTDGFreeTypeFont( std::string strFontName, int iFontSize, int iOutLineSize, 
	bool bRHW, int fontWeight /*= FW_NORMAL*/, int enlargeNum /*= 1*/, bool bNoRes )
{
	m_bLoad = false;
	m_EnlargeNum = enlargeNum;

	if ( m_EnlargeNum < 1 )
		m_EnlargeNum = 1;

	m_strFontName = strFontName;
	m_iOrgFontSize = iFontSize * enlargeNum;
	m_iOutLineSize = iOutLineSize;
	m_iFontWeight = fontWeight;

	m_iFontSize = m_iOrgFontSize;
	m_iFontCacheSize = m_iFontSize + 2 + m_iOutLineSize * 2;

	m_bRHW = bRHW;
	m_bNoRes = bNoRes;

	for( int i = 0; i < FONT_CACHE_PAGE; i++ )
	{
		m_pTexture[i]			= NULL;
		m_pTextureOutLine[i]	= NULL;
		m_iCandidate[i]			= 0;
	}

	//{{ robobeg : 2008-10-13
	m_RenderStateID = s_akStates;
	//}} robobeg : 2008-10-13

	m_iTotalCache = 0;
	m_iNumCandidatesInAPage = 0;
	m_uDCFontCount = 0;
	m_dwCurrentStamp = 1;

	m_iSizeArray = 0;
	m_FontCacheArray = NULL;
	ZeroMemory( m_FontCacheHashTable, sizeof( m_FontCacheHashTable ) );

	m_wstrBuffer.clear();
	ZeroMemory( m_apPageVertex, sizeof(m_apPageVertex) );

	m_FreeTypeLibrary = g_pKTDXApp->GetDGManager()->GetFontManager()->GetFreeTypeLibrary();
	m_FreeTypeFace = NULL;
#ifdef USE_FREE_TYPE_SUB_FONT
	m_FreeTypeSubFace = NULL;
#endif USE_FREE_TYPE_SUB_FONT
	m_KoreanFreeTypeFace = NULL;
}


CKTDGFreeTypeFont::~CKTDGFreeTypeFont(void)
{
	for( int i = 0; i < ARRAY_SIZE( m_apPageVertex ); i++ )
	{
		if ( m_apPageVertex[ i ] != NULL )
		{
			free( m_apPageVertex[ i ] );
			m_apPageVertex[ i ] = NULL;
		}//if
	}//for
}

void CKTDGFreeTypeFont::OnResetDevice()
{
	if( m_bLoad == false )
	{
		Load();
	}
	else
	{
		int iOldFontSize = m_iFontSize;
		float fFontSize = (float)m_iOrgFontSize;
		if ( m_bNoRes == false )
			fFontSize *= g_pKTDXApp->GetResolutionScaleY();
		int iNewFontSize = (int)fFontSize;

		if ( iOldFontSize != iNewFontSize )
		{
			UnLoad();
			Load();
		}//if
		// Only : D3DPOOL_DEFAULT
		for(int i = 0; i < FONT_CACHE_PAGE; i++)
		{				
			if ( m_pTexture[i] != NULL )
				m_pTexture[i]->ResetDevice();
			if( m_pTextureOutLine[i] != NULL )
			{
				m_pTextureOutLine[i]->ResetDevice();
			}
		}
	}
}

void CKTDGFreeTypeFont::OnLostDevice()
{
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		if ( m_pTexture[i] != NULL )
			m_pTexture[i]->LostDevice();
		if( m_pTextureOutLine[i] != NULL )
		{
			m_pTextureOutLine[i]->LostDevice();
		}
	}
}
void CKTDGFreeTypeFont::_EstimateFontSize( WCHAR wChar, SIZE* pSize )
{
	if( pSize == NULL )
		return;

	if( wChar >= HASH_BUCKET_SIZE ||
		wChar == '\r')
	{
		pSize->cx = pSize->cy = 0;
		return;
	}//if

	FontHash& fontHash = m_FontCacheHashTable[ wChar ];
	if( fontHash.m_bFontSize == false )
	{
		fontHash.m_bFontSize = true;
		SGCFontCache* pFontCache = GetFontCache( wChar );

		fontHash.m_size.cx = pFontCache->iWidth - m_iOutLineSize * 2 - 1;
		fontHash.m_size.cy = pFontCache->iHeight - m_iOutLineSize * 2 - 1;
	}//if

	*pSize = fontHash.m_size;
}//CKTDGFreeTypeFont::_EstimateFontSize()

int CKTDGFreeTypeFont::_EstimateStringWidth( const WCHAR* pwStr, int iSize )
{
	if ( pwStr == NULL || iSize <= 0 )
		return 0;

	SIZE    size;
	int iWidth = 0;
	for( int i = 0; i < iSize; i++ )
	{
		_EstimateFontSize( pwStr[i], &size );
		iWidth += size.cx;
	}//for

	return iWidth;
}//CKTDGFreeTypeFont::_EstimateStringWidth()

int CKTDGFreeTypeFont::_EstimateStringHeight( const WCHAR* wstrBuf, int iSize )
{
	if ( wstrBuf == NULL || iSize <= 0 )
		return 0;

	int iHeight = 0;
	bool bNewLine = false;

	for(int i = 0; i < iSize; i++)
	{
		if( wstrBuf[i] == L'\r' || wstrBuf[i] == L'\n' || i == iSize - 1 )
		{	
			bNewLine = false;
			if( wstrBuf[i] == L'\n' || i == iSize - 1 )
				bNewLine = true;

			if( bNewLine )
			{
				// 안티-앨리어싱 때문에 +1
				iHeight += m_iFontSize + 1;
			}			
		}
	}

	return  iHeight;
}//CKTDGFreeTypeFont::_EstimateStringHeight()

bool CKTDGFreeTypeFont::Load()
{
	if( m_bLoad == true )
		return true;

	float fFontSize = (float)m_iOrgFontSize;
	if ( m_bNoRes == false )
		fFontSize *= g_pKTDXApp->GetResolutionScaleY();

	m_iFontSize = (int)fFontSize;
	m_iFontCacheSize = m_iFontSize + 2 + m_iOutLineSize * 2;
	m_iNumCandidatesInAPage = 3 * ( FONT_CACHE_SIZE / m_iFontCacheSize ) * ( FONT_CACHE_SIZE / m_iFontCacheSize );

	// 실제 폰트
	{
#ifdef  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
        const std::vector<BYTE>& vec = g_pKTDXApp->GetDGManager()->GetFontManager()->GetFontMemory( m_strFontName );
        if( vec.empty() == true || FT_New_Memory_Face( m_FreeTypeLibrary, ( const FT_Byte* )&vec.front(), vec.size(), 0, &m_FreeTypeFace ) )
#else   X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER pMemory = g_pKTDXApp->GetDGManager()->GetFontManager()->GetFontMemory( m_strFontName );
		if( FT_New_Memory_Face( m_FreeTypeLibrary, ( const FT_Byte* )pMemory->pRealData, pMemory->size, 0, &m_FreeTypeFace ) )
#endif  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		{
			ErrorLogMsg( KEM_ERROR426, "FT_New_Memory_Face" );
			MessageBoxA( NULL, m_strFontName.c_str(), "Font Load Error!", NULL );
			return false;
		}
		// freetype은 소수점 사이즈를 지원하므로 적용해도 좋을듯
		if( FT_Set_Char_Size( m_FreeTypeFace, m_iFontSize << 6, 0, 72, 72 ) )
		{
			MessageBoxA( NULL, m_strFontName.c_str(), "Font Size Error!", NULL );
			ErrorLogMsg( KEM_ERROR426, "FT_Set_Char_Size" );
			return false;
		}

	}

#ifndef CLIENT_INT_INTERNAL
#ifdef USE_FREE_TYPE_SUB_FONT
	// 실제 폰트가 지원못하는 글자를 위한 서브 폰트
	{
#ifdef  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		const std::vector<BYTE>& vec = g_pKTDXApp->GetDGManager()->GetFontManager()->GetFontMemory( "JhengHeiBold" );
		if( vec.empty() == true || FT_New_Memory_Face( m_FreeTypeLibrary, ( const FT_Byte* )&vec.front(), vec.size(), 0, &m_FreeTypeSubFace ) )
#else   X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER pMemory = g_pKTDXApp->GetDGManager()->GetFontManager()->GetFontMemory( "JhengHeiBold" );
		if( FT_New_Memory_Face( m_FreeTypeLibrary, ( const FT_Byte* )pMemory->pRealData, pMemory->size, 0, &m_FreeTypeSubFace ) )
#endif  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		{
			ErrorLogMsg( KEM_ERROR426, "FT_New_Memory_Face" );
			MessageBoxA( NULL, m_strFontName.c_str(), "Font Load Error!", NULL );
			return false;
		}
		// freetype은 소수점 사이즈를 지원하므로 적용해도 좋을듯
		if( FT_Set_Char_Size( m_FreeTypeSubFace, m_iFontSize << 6, 0, 72, 72 ) )
		{
			MessageBoxA( NULL, m_strFontName.c_str(), "Font Size Error!", NULL );
			ErrorLogMsg( KEM_ERROR426, "FT_Set_Char_Size" );
			return false;
		}
	}
#endif USE_FREE_TYPE_SUB_FONT
#endif CLIENT_INT_INTERNAL

	// 굴림
	if( m_strFontName != FT_DEFAULT_FONTNAME )
	{
#ifdef  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		const std::vector<BYTE>& vec = g_pKTDXApp->GetDGManager()->GetFontManager()->GetFontMemory( FT_DEFAULT_FONTNAME );
		if( vec.empty() == true || FT_New_Memory_Face( m_FreeTypeLibrary, ( const FT_Byte* )&vec.front(), vec.size(), 0, &m_KoreanFreeTypeFace ) )
#else   X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER pMemory = g_pKTDXApp->GetDGManager()->GetFontManager()->GetFontMemory( FT_DEFAULT_FONTNAME );
		if( FT_New_Memory_Face( m_FreeTypeLibrary, ( const FT_Byte* )pMemory->pRealData, pMemory->size, 0, &m_KoreanFreeTypeFace ) )
#endif  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		{
			ErrorLogMsg( KEM_ERROR426, "FT_New_Memory_Face" );
			MessageBoxA( NULL, FT_DEFAULT_FONTNAME, "Font Load Error!", NULL );
			return false;
		}

		// freetype은 소수점 사이즈를 지원하므로 적용해도 좋을듯
		if( FT_Set_Char_Size( m_KoreanFreeTypeFace, m_iFontSize << 6, 0, 72, 72 ) )
		{
			ErrorLogMsg( KEM_ERROR426, "FT_Set_Char_Size" );
			MessageBoxA( NULL, FT_DEFAULT_FONTNAME, "Font Size Error!", NULL );
			return false;
		}
	}


	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		m_pTexture[i] = new CKTDGDynamicTexture();

		if( !m_pTexture[i]->CreateTexture( FONT_CACHE_SIZE, FONT_CACHE_SIZE, MYFONTCOLORFMT ) )
		{
			ErrorLogMsg( KEM_ERROR6, m_strFontName.c_str() );
			return false;
		}

		if( m_iOutLineSize > 0 )
		{
			m_pTextureOutLine[i] = new CKTDGDynamicTexture();

			if( !m_pTextureOutLine[i]->CreateTexture( FONT_CACHE_SIZE, FONT_CACHE_SIZE, MYFONTCOLORFMT ) )
			{
				ErrorLogMsg( KEM_ERROR7, m_strFontName.c_str() );
				return false;
			}
		}
	}

	InitCache();

	int iNumVertices = 6 * m_iNumCandidatesInAPage;

	for( int i = 0; i < ARRAY_SIZE( m_apPageVertex ); i++ )
	{
		if ( m_apPageVertex[ i ] != NULL )
			free( m_apPageVertex[ i ] );
		m_apPageVertex[ i ] = (void*) malloc( iNumVertices * ( ( m_bRHW == true ) ? sizeof(S2DUIVertex) : sizeof(SLVERTEX) ) );
		ASSERT( m_apPageVertex[ i ] != NULL );
	}//for

	m_bLoad = true;
	return true;
}

void CKTDGFreeTypeFont::InitCache()
{
	m_iTotalCache = ( FONT_CACHE_SIZE / m_iFontCacheSize ) * ( FONT_CACHE_SIZE / m_iFontCacheSize ) * FONT_CACHE_PAGE;
	m_iSizeArray = 0;
	m_FontCacheArray = new SGCFontCache* [m_iTotalCache];

	for( int i=0; i< m_iTotalCache; i++ )
	{
		m_FontCacheArray[i] = new SGCFontCache();
	}

	for( int i = 0; i < HASH_BUCKET_SIZE; i++ )
	{
		m_FontCacheHashTable[ i ].Init();
	}//for
}

void CKTDGFreeTypeFont::UnLoad( bool bComplete )
{
	if( m_bLoad == false)
		return;

	m_bLoad = false;

	for( int i = 0; i < FONT_CACHE_PAGE; i++ )
	{	
		SAFE_DELETE( m_pTexture[i] );
		SAFE_DELETE( m_pTextureOutLine[i] );
	}

	for( int i = 0; i < m_iTotalCache; i++ )
	{
		SAFE_DELETE( m_FontCacheArray[i] );
	}
	SAFE_DELETE_ARRAY( m_FontCacheArray );

	for( int i = 0; i < HASH_BUCKET_SIZE; i++ )
	{
		m_FontCacheHashTable[ i ].Init();
	}//for

	for( int i = 0; i < ARRAY_SIZE( m_apPageVertex ); i++ )
	{
		if( m_apPageVertex[ i ] != NULL )
		{
			free( m_apPageVertex[ i ] );
			m_apPageVertex[ i ] = NULL;
		}//if
	}//for
}

void CKTDGFreeTypeFont::CharToSystemTexture( SGCFontCache* cache )
{
	FT_Face ftCurrentFace = NULL;

	unsigned int index = FT_Get_Char_Index( m_FreeTypeFace, cache->_Char );

	// 폰트가 정상적으로 존재할 때
	if( index > 0 )
	{
		if( FT_Load_Glyph( m_FreeTypeFace, index, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP ) == 0 )
		{
			if( FT_Render_Glyph( m_FreeTypeFace->glyph, FT_RENDER_MODE_NORMAL ) == 0 )
			{
				ftCurrentFace = m_FreeTypeFace;
			}
		}
	}
#ifdef USE_FREE_TYPE_SUB_FONT
	// 서브폰트로 재시도
	if( ftCurrentFace == NULL )
	{
		if( NULL != m_FreeTypeSubFace )
		{
			index = FT_Get_Char_Index( m_FreeTypeSubFace, cache->_Char );
			if( index > 0 )
			{
				int i = FT_Load_Glyph( m_FreeTypeSubFace, index, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP );

				if( i == 0 )
				{
					if( FT_Render_Glyph( m_FreeTypeSubFace->glyph, FT_RENDER_MODE_NORMAL ) == 0 )
					{
						ftCurrentFace = m_FreeTypeSubFace;
					}
				}
			}	
		}
	}
#endif USE_FREE_TYPE_SUB_FONT

	// 비상폰트로 재시도
	if( ftCurrentFace == NULL )
	{
		if( NULL != m_KoreanFreeTypeFace )
		{
			index = FT_Get_Char_Index( m_KoreanFreeTypeFace, cache->_Char );
			if( index > 0 )
			{
				if( FT_Load_Glyph( m_KoreanFreeTypeFace, index, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP ) == 0 )
				{
					if( FT_Render_Glyph( m_KoreanFreeTypeFace->glyph, FT_RENDER_MODE_NORMAL ) == 0 )
					{
						ftCurrentFace = m_KoreanFreeTypeFace;
					}
				}
			}	
		}
	}

	// 폰트 그리기에 실패
	if( ftCurrentFace == NULL )
		return;

	// 폰트의 실제 가로, 세로 픽셀수
	int iGlyphWidth = ftCurrentFace->glyph->bitmap.width;
	int iGlyphRows = ftCurrentFace->glyph->bitmap.rows;

	// 아웃라인 비트맵 생성
	FT_Bitmap OutLineBitmap;
	int iOutLineGlyphWidth = iGlyphWidth;
	int iOutLineGlyphRows = iGlyphRows;
	if( m_iOutLineSize > 0 )
	{
		FT_Bitmap_New( &OutLineBitmap );
		FT_Bitmap_Copy( m_FreeTypeLibrary, &ftCurrentFace->glyph->bitmap, &OutLineBitmap );
		FT_Bitmap_Embolden( m_FreeTypeLibrary, &OutLineBitmap, m_iOutLineSize << 7, m_iOutLineSize << 7 );

		iOutLineGlyphWidth = OutLineBitmap.width;
		iOutLineGlyphRows = OutLineBitmap.rows;
	}

	// 위치 정렬을 위한 여백 값을 구함
	FT_Glyph_Metrics* pMetrics = &ftCurrentFace->glyph->metrics;
	FT_Size_Metrics* pSizeMetrics = &ftCurrentFace->size->metrics;

	int iPaddingX = max( pMetrics->horiBearingX >> 6, 0 );
	int iPaddingY = ( pSizeMetrics->ascender - pMetrics->horiBearingY ) >> 6;

	// 폰트 캐쉬에 맞게 사이즈를 추가하여 표시
	// GetTextExtentPoint32W 의 역할을 여기서 함
	int iAdvanceX = max( ( pMetrics->horiAdvance >> 6 ) + m_iOutLineSize * 2, iOutLineGlyphWidth );
	iAdvanceX = min( iAdvanceX, m_iFontCacheSize );
	int iAdvenceY = min( iOutLineGlyphRows + iPaddingY + 1, m_iFontCacheSize );
	cache->_SetRect( iAdvanceX, iAdvenceY );

	// 폰트 캐쉬에 그림
	for( int iYInFontCache = 0; iYInFontCache < cache->iHeight; ++iYInFontCache )
	{
		int iYInGlyph = iYInFontCache - m_iOutLineSize - iPaddingY;
		int iYInOutLineGlyph = iYInFontCache - iPaddingY;

		for( int iXInFontCache = 0; iXInFontCache < cache->iWidth; ++iXInFontCache )
		{
			int iXInGlyph = iXInFontCache - m_iOutLineSize - iPaddingX;

			// 현재 폰트캐쉬 페이지의 포인터 구하고
			MYFONTCOLOR_PTR pCurrentPixel =
				(MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits +
				( cache->iTop + iYInFontCache ) * FONT_CACHE_SIZE +
				cache->iLeft + iXInFontCache;

			// 실제 점을 찍는다.
			if( iYInGlyph >= 0 && iYInGlyph < iGlyphRows && iXInGlyph >= 0 && iXInGlyph < iGlyphWidth )
			{
				unsigned char cColor = ftCurrentFace->glyph->bitmap.buffer[ iYInGlyph * iGlyphWidth + iXInGlyph ];
				*pCurrentPixel = MYD3DCOLOR_ARGB( cColor, 0xff, 0xff, 0xff);
			}
			else
			{
				*pCurrentPixel = 0;
			}

			// 아웃라인
			if( m_iOutLineSize > 0 )
			{
				int iXInOutLineGlyph = iXInFontCache - iPaddingX;

				// 현재 아웃라인 폰트캐쉬 페이지의 포인터 구하고
				MYFONTCOLOR_PTR pOutLineCurrentPixel =
					(MYFONTCOLOR_PTR)m_pTextureOutLine[cache->iPage]->m_LockedRect.pBits +
					( cache->iTop + iYInFontCache ) * FONT_CACHE_SIZE +
					cache->iLeft + iXInFontCache;

				// 실제 점을 찍는다.
				if( iYInOutLineGlyph >= 0 && iYInOutLineGlyph < iOutLineGlyphRows && iXInOutLineGlyph >= 0 && iXInOutLineGlyph < iOutLineGlyphWidth )
				{
					unsigned char cColor = OutLineBitmap.buffer[ iYInOutLineGlyph * iOutLineGlyphWidth + iXInOutLineGlyph ];
					*pOutLineCurrentPixel = MYD3DCOLOR_ARGB( cColor, 0xff, 0xff, 0xff );
				}
				else
				{
					*pOutLineCurrentPixel = 0;
				}
			}
		}
	}

}

SGCFontCache* CKTDGFreeTypeFont::GetFontCache( WCHAR _Char )
{
	SGCFontCache *pExistingCache = m_FontCacheHashTable[_Char].m_pFontCache;

	if( pExistingCache != NULL)
	{
		pExistingCache->dwLastUse = m_dwCurrentStamp++;
		return pExistingCache;
	}

	if( m_iSizeArray >= m_iTotalCache )
	{
		// 가장 사용한지 오래된 캐쉬 찾기
		DWORD dwLastUseMin = m_FontCacheArray[0]->dwLastUse;
		int IdxMin = 0;
		for( int i = 1; i < m_iTotalCache; i++ )
		{
			if( (LONG) ( dwLastUseMin - m_FontCacheArray[i]->dwLastUse ) > 0 )
			{
				IdxMin = i;
				dwLastUseMin = m_FontCacheArray[i]->dwLastUse;
			}
		}

		// 해당 문자의 캐쉬 사용권을 박탈해서
		m_FontCacheHashTable[m_FontCacheArray[IdxMin]->_Char].m_pFontCache = NULL;

		// 새로운 문자에게 할당
		m_FontCacheHashTable[_Char].m_pFontCache = m_FontCacheArray[IdxMin];

		// 새로운 문자에 맞춰 캐쉬 내용 갱신
		SGCFontCache *cache = m_FontCacheArray[IdxMin];

		cache->_Char = _Char;
		cache->dwLastUse = m_dwCurrentStamp++;

		CharToSystemTexture( cache );

		RECT    rect;
		rect.left = cache->iLeft;
		rect.top  = cache->iTop;
		rect.right = cache->iLeft + cache->iWidth;
		rect.bottom = cache->iTop + cache->iHeight;

		m_pTexture[cache->iPage]->SetChange( rect );

		if( m_pTextureOutLine[cache->iPage] )
			m_pTextureOutLine[cache->iPage]->SetChange( rect );

		return cache;
	}

	m_FontCacheHashTable[_Char].m_pFontCache = m_FontCacheArray[m_iSizeArray];

	SGCFontCache *cache = m_FontCacheArray[m_iSizeArray];

	const int iCharPerPage = ( FONT_CACHE_SIZE / m_iFontCacheSize ) * ( FONT_CACHE_SIZE / m_iFontCacheSize );
	int iPage = m_iSizeArray / iCharPerPage;
	int iIndexInPage = m_iSizeArray % iCharPerPage;
	int y = iIndexInPage / ( FONT_CACHE_SIZE / m_iFontCacheSize );
	int x = iIndexInPage % ( FONT_CACHE_SIZE / m_iFontCacheSize );

	cache->iPage = iPage;
	cache->SetPos( x, y, m_iFontCacheSize );
	cache->_Char = _Char;
	cache->dwLastUse = m_dwCurrentStamp++;

	CharToSystemTexture( cache );

	RECT    rect;
	rect.left = cache->iLeft;
	rect.top  = cache->iTop;
	rect.right = cache->iLeft + cache->iWidth;
	rect.bottom = cache->iTop + cache->iHeight;

	m_pTexture[cache->iPage]->SetChange( rect );
	if( m_pTextureOutLine[cache->iPage] )
		m_pTextureOutLine[cache->iPage]->SetChange( rect );

	m_iSizeArray++;

	return cache;
}


void CKTDGFreeTypeFont::Flush(bool bOutLine, int iPage )
{
	KTDXPROFILE();

	if ( iPage >= FONT_CACHE_PAGE )
		return;

	int iStartPage = ( iPage < 0 ) ? 0 : iPage;
	int iEndPage = ( iPage < 0 ) ? ( FONT_CACHE_PAGE - 1 ) : iPage;

	KD3DPUSH( m_RenderStateID )

	for(int i = iStartPage; i <= iEndPage; i++)
	{	
		if( m_iCandidate[i] == 0 )
			continue;

		if ( m_apPageVertex[i] == NULL )
		{
			m_iCandidate[i] = 0;
			continue;
		}//if

		if( bOutLine )
			m_pTextureOutLine[i]->SetTexture(0);
		else
			m_pTexture[i]->SetTexture(0);

		if( m_bRHW == true )
		{
			BOOST_STATIC_ASSERT( S2DUIVertex::FVF == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
			g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_apPageVertex[i] );
	    }
	    else
	    {
            BOOST_STATIC_ASSERT( SLVERTEX::FVF == D3DFVF_XYZ_DIFFUSE_TEX1 );
            g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE_TEX1, D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_apPageVertex[i] );
	    }

		m_iCandidate[i] = 0;
	}

	KD3DEND()
}
#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
void CKTDGFreeTypeFont::Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount, float fScaleX, float fScaleY, const int& iRight )
#else HIDE_FONT_OUT_OF_EDITBOX_SIZE
void CKTDGFreeTypeFont::Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount, float fScaleX, float fScaleY )
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE
{
	if ( str == NULL )
		return;

	int isize = ( nCount < 0 ) ? (int)wcslen(str) : nCount;
	float bufferingILeft = (float)iLeft;
	D3DCOLOR bufferingColor = color;


	for(int i = 0; i < isize; ++i )
	{
#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
		if( iRight > 0 && bufferingILeft > iRight )
			break;
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE

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
					wcsncpy_s(strColor, _countof(strColor), &str[i+2],6);
					swscanf_s(strColor,L"%x",&iCol);
					bufferingColor = ( bufferingColor & 0xFF000000 ) | iCol;
				}
			}			
			i += 7;
			continue;
		}

		SGCFontCache *cache = GetFontCache(str[i]);
        ASSERT( cache != NULL && cache->iPage >= 0 && cache->iPage < FONT_CACHE_PAGE
            && m_apPageVertex[cache->iPage] != NULL );
        if ( false == ( cache != NULL && cache->iPage >= 0 && cache->iPage < FONT_CACHE_PAGE ) )
            return;
        if ( m_apPageVertex[cache->iPage] == NULL )
        {
            m_iCandidate[ cache->iPage ] = 0;
            return;
        }//if

		if ( m_iCandidate[cache->iPage] >= m_iNumCandidatesInAPage )
        {
            Flush( bOutLine, cache->iPage );
            ASSERT( m_iCandidate[cache->iPage] < m_iNumCandidatesInAPage );
            if ( m_iCandidate[cache->iPage] >= m_iNumCandidatesInAPage )
			{
				m_iCandidate[ cache->iPage ] = 0;
				return;
			}//if
		}//if

		float tempValueX = 0;
		float tempValueY = 0;
		float fCacheWidth = cache->iWidth * fScaleX;
		float fCacheHeight = cache->iHeight * fScaleY;
		
		if ( m_EnlargeNum > 1 )
		{
			tempValueX = ( ( cache->iWidth ) / ( 4 * ( m_EnlargeNum - 1 ) ) ) * fScaleX;
			tempValueY = ( ( cache->iHeight  ) / ( 2 * ( m_EnlargeNum - 1 ) ) ) * fScaleY;
		}

		if( m_bRHW == true )
		{
			S2DUIVertex* pVertex = NULL;
			pVertex = &( (S2DUIVertex*) m_apPageVertex[cache->iPage] )[m_iCandidate[cache->iPage] * 6];	
			pVertex[0].SetVertexColor( bufferingILeft + tempValueX, (float)iTop, bufferingColor, cache->_texLeft, cache->_texTop );
			pVertex[1].SetVertexColor( bufferingILeft + fCacheWidth - tempValueX, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
			pVertex[2].SetVertexColor( bufferingILeft + tempValueX, (float)iTop + fCacheHeight - tempValueY, bufferingColor, cache->_texLeft, cache->_texBottom );

			pVertex[3].SetVertexColor( bufferingILeft + fCacheWidth - tempValueX, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
			pVertex[4].SetVertexColor( bufferingILeft + fCacheWidth - tempValueX, (float)iTop + fCacheHeight - tempValueY,	bufferingColor, cache->_texRight, cache->_texBottom);
			pVertex[5].SetVertexColor( bufferingILeft + tempValueX, (float)iTop + fCacheHeight - tempValueY, bufferingColor, cache->_texLeft, cache->_texBottom );
		}
		else
		{
			SLVERTEX* pVertex = NULL;
			pVertex = &( (SLVERTEX*) m_apPageVertex[cache->iPage] )[m_iCandidate[cache->iPage] * 6];		
			pVertex[0].SetVertexColor( bufferingILeft, (float)iTop, bufferingColor, cache->_texLeft, cache->_texTop );
			pVertex[1].SetVertexColor( bufferingILeft + fCacheWidth, (float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
			pVertex[2].SetVertexColor( bufferingILeft, (float)iTop - fCacheHeight,	bufferingColor, cache->_texLeft, cache->_texBottom );

			pVertex[3].SetVertexColor( bufferingILeft + fCacheWidth, (float)(float)iTop, bufferingColor, cache->_texRight, cache->_texTop );
			pVertex[4].SetVertexColor( bufferingILeft + fCacheWidth, iTop - fCacheHeight, bufferingColor, cache->_texRight, cache->_texBottom);
			pVertex[5].SetVertexColor( bufferingILeft, (float)iTop - fCacheHeight, bufferingColor, cache->_texLeft, cache->_texBottom );
		}

		//{{ robobeg : 2011-11-17
		if ( m_iOutLineSize != 0 )
			bufferingILeft += ( ( cache->iWidth /m_EnlargeNum ) - 2) * fScaleX;
		else
			bufferingILeft += ( ( cache->iWidth /m_EnlargeNum ) - 1) * fScaleX;
		//}} robobeg : 2011-11-17
		m_iCandidate[cache->iPage]++;
	}	
}

int CKTDGFreeTypeFont::GetWidth( const WCHAR* wszText, int nCount )
{
	KTDXPROFILE();

	if( NULL == wszText || nCount == 0 )
		return 0;
    if ( wszText[0] == NULL )
        return 0;
    int iLen = ( nCount < 0 ) ? (int)wcslen(wszText) : nCount;

	int iCur = 0;
	int	maxWidth = 0;

	m_wstrBuffer.resize(0);
	for(int i = 0; i < iLen; i++ )
	{
		if ( wszText[i] == '\n' )
		{
			SIZE size;
			int iLength = m_wstrBuffer.size();
            size.cx = _EstimateStringWidth( m_wstrBuffer.c_str(), iLength );
//{{ robobeg : 2011-11-17
            int tempWidth = ( m_iOutLineSize != 0 )
                ? ( (int)(size.cx/m_EnlargeNum)  + (iLength * m_iOutLineSize * 2) - iLength )
                : ( (int)(size.cx/m_EnlargeNum) );
//}} robobeg : 2011-11-17
			if ( tempWidth > maxWidth )
				maxWidth = tempWidth;

			m_wstrBuffer.resize(0);
			continue;
		}


		if( wszText[i] == L'#' && i < iLen -1 && ( wszText[i+1] == L'c' || wszText[i+1] == L'C' ) )
		{
			if( wszText[i+2] == 'x' || wszText[i+2] == 'X' )
			{
				i += 2;
			}
			else
			{
				i += 7;
			}
			continue;
		}
		
		if ( wszText[i] != '\0' )
			m_wstrBuffer += wszText[i];
	}
	/*
	SIZE size;
	int iLength = wcslen(strBuf.c_str());
	GetTextExtentPoint32W( m_hDC, strBuf.c_str(), iLength, &size);	
	//SAFE_DELETE_ARRAY( strBuf );
	return (int)size.cx  + iLength * m_iOutLineSize * 2;
	*/
	SIZE size;
	int iLength = m_wstrBuffer.size();
    size.cx = _EstimateStringWidth( m_wstrBuffer.c_str(), iLength );
//{{ robobeg : 2011-11-17
	int tempWidth = ( m_iOutLineSize != 0 )
        ? ( (int)(size.cx/m_EnlargeNum)  + (iLength * m_iOutLineSize * 2)  - iLength )
        : ( (int)(size.cx/m_EnlargeNum) );
//}} robobeg : 2011-11-17
	if ( tempWidth > maxWidth )
		maxWidth = tempWidth;

	m_wstrBuffer.resize(0);

	return maxWidth;
}

void CKTDGFreeTypeFont::OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color, D3DCOLOR colorOutLine, RECT* pRt, DWORD dwFlag, int nCount, float fScaleX, float fScaleY )
{
	KTDXPROFILE();

	if( wszText == NULL || wszText[0] == 0 )
		return;

	int iFinalLeft = GetLeftPos( iLeft, wszText, pRt, dwFlag, nCount, fScaleX );
	
	m_dwCurrentStamp = timeGetTime();

	D3DCOLOR curcol;

	if( m_iOutLineSize )
	{
		curcol = colorOutLine;
#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
		int iLineRight = GetRightPos( iLeft, wszText, pRt, dwFlag, -1, fScaleX );
		Buffering( true, iFinalLeft, iTop, (WCHAR*)wszText, curcol, colorOutLine, true, nCount, fScaleX, fScaleY, iLineRight );
#else HIDE_FONT_OUT_OF_EDITBOX_SIZE
		Buffering( true, iFinalLeft, iTop, (WCHAR*)wszText, curcol, colorOutLine, true, nCount, fScaleX, fScaleY );
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE
		Flush( true );
	}

	curcol = color;
#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
	int iLineRight = GetRightPos( iLeft, wszText, pRt, dwFlag, -1, fScaleX );
	Buffering( false, iFinalLeft, iTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0, nCount, fScaleX, fScaleY, iLineRight );
#else HIDE_FONT_OUT_OF_EDITBOX_SIZE
	Buffering( false, iFinalLeft, iTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0, nCount, fScaleX, fScaleY );
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE
	Flush(false);	
}


void CKTDGFreeTypeFont::OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color, D3DCOLOR colorOutLine, float fLineSpace, RECT* pRt, DWORD dwFlag, int nCount, float fScaleX, float fScaleY )
{
	if( wszText == NULL || wszText[0] == 0 || nCount == 0 )
		return;

//#ifdef FIX_TEXT_SCALING
//	ASSERT( fScaleX == 1.f );
//	ASSERT( fScaleY == 1.f );
//#endif FIX_TEXT_SCALING

	static WCHAR			wstrBuf[VIRTUAL_FONT_LINE_MAX];

	int iOrgLength = wcslen( wszText );
	if ( nCount < 0 )
		nCount = iOrgLength;
	else
		nCount = __min( nCount, iOrgLength );
	nCount = __min( nCount, VIRTUAL_FONT_LINE_MAX - 1 );

	StringCchCopy( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
	wstrBuf[nCount] = 0;

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

				int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag, -1, fScaleX );

#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
				int iLineRight = GetRightPos( iLeft, pToken, pRt, dwFlag, -1, fScaleX );
				Buffering( true, iLineLeft, iTopLocal, pToken, curcol, color, true, -1, fScaleX, fScaleY, iLineRight );
#else HIDE_FONT_OUT_OF_EDITBOX_SIZE
				Buffering( true, iLineLeft, iTopLocal, pToken, curcol, color, true, -1, fScaleX, fScaleY );
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE

				if( bNewLine )
				{
					if( m_bRHW == true )
						iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum - 1 ) * fLineSpace * fScaleY );
					else
						iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum - 1 ) * fLineSpace * fScaleY );
				}

				pToken = &wstrBuf[i+1];
			}
		}

		Flush(true);
	}

	StringCchCopy( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
	wstrBuf[nCount] = 0;

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

			int iLineLeft = GetLeftPos( iLeft, pToken, pRt, dwFlag, -1, fScaleX );

#ifdef HIDE_FONT_OUT_OF_EDITBOX_SIZE
			int iLineRight = GetRightPos( iLeft, pToken, pRt, dwFlag, -1, fScaleX );
			Buffering( false, iLineLeft, iTopLocal, pToken, curcol, color, (dwFlag & DT_NO_COLOR) != 0, -1, fScaleX, fScaleY, iLineRight );
#else HIDE_FONT_OUT_OF_EDITBOX_SIZE
			Buffering( false, iLineLeft, iTopLocal, pToken, curcol, color, (dwFlag & DT_NO_COLOR) != 0, -1, fScaleX, fScaleY );
#endif HIDE_FONT_OUT_OF_EDITBOX_SIZE
			//Display Here
			if( bNewLine )
			{
				if( m_bRHW == true )
					iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum - 1) * fLineSpace * fScaleY );
				else
					iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum - 1) * fLineSpace * fScaleY );
			}			

			pToken = &wstrBuf[i+1];
		}
	}

	Flush(false);
}


#endif USE_FREE_TYPE