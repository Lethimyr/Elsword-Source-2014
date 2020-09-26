#include "StdAfx.h"
#include ".\KTDGDeviceFontBase.h"
#include <algorithm>
//#include "../GCUtil/KJohnGen.h"


float SGCFontCacheBase::PixelToTex = 1.0f / FONT_CACHE_SIZE;
#define PAGE_VERTEX_MULTIPLE    2

//{{ robobeg : 2008-10-13
static const CKTDGStateManager::KState s_akStates[] = 
{
	KRenderState( D3DRS_ALPHABLENDENABLE,	TRUE ),
	KRenderState( D3DRS_ZWRITEENABLE,		FALSE ),

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
    KRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA ),
    KRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA ),
	KTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ),

	KTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ),
	KTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ),
#endif

	KSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR ),
	KSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ),
	KSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR ),
};//akStates[]



class CKTDGFontCacheScheduler
{
public: 
	bool operator()( const SGCFontCacheBase* p, const SGCFontCacheBase* q ) const
	{
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		return (LONG) ( p->dwLastUse - q->dwLastUse ) < 0;
#else
		return p->dwLastUse < q->dwLastUse;
#endif
	}
};

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
CKTDGDeviceFontBase::CKTDGDeviceFontBase(std::string strFontName, int iFontSize, int iOutLineSize, 
	bool bRHW, int fontWeight /*= FW_NORMAL*/, int enlargeNum /*= 1*/,  bool bNoRes )
#else
CKTDGDeviceFontBase::CKTDGDeviceFontBase(std::string strFontName, int iFontSize, int iOutLineSize, 
								 bool bRHW, int fontWeight /*= FW_NORMAL*/, int enlargeNum /*= 1*/ )
#endif
: m_wstrBuffer( L"" )
{
	m_bLoad = false;
#ifdef  KTDGDEVICEFONT_BACKGROUND_PRELOADING
	m_bPreload = false;
#endif

	m_EnlargeNum = enlargeNum;

	if ( m_EnlargeNum < 1 )
		m_EnlargeNum = 1;

	m_strFontName = strFontName;
	m_iOrgFontSize = (iFontSize*enlargeNum);
	m_iOutLineSize = iOutLineSize;
	m_iFontWeight = fontWeight;

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_iFontSize = m_iOrgFontSize;
	m_iFontCacheSize = (m_iFontSize) + m_iOutLineSize*2;
#endif

	m_bRHW = bRHW;
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_bNoRes = bNoRes;
#endif

	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		m_pTexture[i]			= NULL;
		m_pTextureOutLine[i]	= NULL;
		m_iCandidate[i]			= 0;
	}

//{{ robobeg : 2008-10-13
    m_RenderStateID = s_akStates;
//}} robobeg : 2008-10-13

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
    m_iTotalCache = 0;
    m_iNumCandidatesInAPage = 0;
	m_uDCFontCount = 0;
#endif
	m_dwCurrentStamp = 1;

#ifdef  FONT_CASH_DATA_STRUCTURE_REFORM
	m_iSizeArray = 0;
	m_FontCacheArray = NULL;
	ZeroMemory( m_FontCacheHashTable, sizeof( m_FontCacheHashTable ) );
#else
	m_mapChar.clear();
#endif FONT_CASH_DATA_STRUCTURE_REFORM

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_hPen = NULL;
    m_hOldPen = NULL;
	m_hBitmap = NULL;
    m_hOldBitmap = NULL;
	m_hDC = NULL;
	m_hFont = NULL;
	m_hOldFont = NULL;

    m_wstrBuffer.clear();
	ZeroMemory( m_apPageVertex, sizeof(m_apPageVertex) );
#endif
}

CKTDGDeviceFontBase::~CKTDGDeviceFontBase(void)
{
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	for( int i = 0; i < ARRAY_SIZE( m_apPageVertex ); i++ )
	{
		if ( m_apPageVertex[ i ] != NULL )
		{
			free( m_apPageVertex[ i ] );
			m_apPageVertex[ i ] = NULL;
		}//if
	}//for
#endif
}

void CKTDGDeviceFontBase::OnResetDevice()
{
	if( m_bLoad == false )
	{
		Load();
	}
	else
	{
		int iOldFontSize = m_iFontSize;
		float fFontSize = (float)m_iOrgFontSize;
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		if ( m_bNoRes == false )
			fFontSize *= g_pKTDXApp->GetResolutionScaleY();
		int iNewFontSize = (int)fFontSize;

		if ( iOldFontSize != iNewFontSize )
		{
			UnLoad();
			Load();
		}//if
#else
		UnLoad();
		Load();
#endif
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

void CKTDGDeviceFontBase::OnLostDevice()
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



#ifdef  KTDGDEVICEFONT_SIZE_CACHE

void    CKTDGDeviceFontBase::_EstimateFontSize( WCHAR wChar, SIZE* pSize ) const
{
    if ( pSize == NULL )
        return;
    if ( wChar >= HASH_BUCKET_SIZE || m_hDC == NULL )
    {
        pSize->cx = pSize->cy = 0;
        return;
    }//if
    FontHash& fontHash = m_FontCacheHashTable[ wChar ];
    if ( fontHash.m_bFontSize == false )
    {
        fontHash.m_bFontSize = true;
		GetTextExtentPoint32W( m_hDC, &wChar, 1, &fontHash.m_size);	
    }//if

    *pSize = fontHash.m_size;
}//CKTDGDeviceFontBase::_EstimateFontSize()

int     CKTDGDeviceFontBase::_EstimateStringWidth( const WCHAR* pwStr, int iSize ) const
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
}//CKTDGDeviceFontBase::_EstimateStringWidth()

int     CKTDGDeviceFontBase::_EstimateStringHeight( const WCHAR* wstrBuf, int iSize ) const
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
                iHeight += m_iFontSize;
			}			
		}
	}

    return  iHeight;
}//CKTDGDeviceFontBase::_EstimateStringHeight()
#endif  KTDGDEVICEFONT_SIZE_CACHE


bool CKTDGDeviceFontBase::Load()
{
	if( m_bLoad == true )
		return true;

	float fFontSize = (float)m_iOrgFontSize;
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	if ( m_bNoRes == false )
		fFontSize *= g_pKTDXApp->GetResolutionScaleY();
#else
	fFontSize *= g_pKTDXApp->GetResolutionScaleY();
#endif
	
	m_iFontSize = (int)fFontSize;


	m_iFontCacheSize = (m_iFontSize) + m_iOutLineSize*2;	

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
    m_iNumCandidatesInAPage = 3 * (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize);
#endif

#ifndef NUMBER_TO_LANGUAGE
	WCHAR wChar[255];
	MultiByteToWideChar( CP_ACP, 0, m_strFontName.c_str(), -1, wChar, sizeof(wChar) );
	m_hFont = CreateFontW( m_iFontSize , 0, 0, 0, m_iFontWeight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, wChar);
#else
#ifdef CLEARTYPE_FONT
	m_hFont = CreateFontA( m_iFontSize , 0, 0, 0, m_iFontWeight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH|FF_DONTCARE, m_strFontName.c_str());
#else
	m_hFont = CreateFontA( m_iFontSize , 0, 0, 0, m_iFontWeight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, m_strFontName.c_str());
#endif
#endif NUMBER_TO_LANGUAGE

	m_hDC = CreateCompatibleDC( NULL );
	m_hBitmap = CreateCompatibleBitmap( GetDC(GetDesktopWindow()), MAX_FONT_SIZE+1, MAX_FONT_SIZE+1 );	
	m_hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap );	
	m_hOldFont = (HFONT)SelectObject( m_hDC, m_hFont );
	SetTextColor( m_hDC, RGB(255,0,0));
	SetBkColor( m_hDC, RGB(0,0,0) );
	SetTextCharacterExtra( m_hDC, 0 );


	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		m_pTexture[i] = new CKTDGDynamicTexture();

		if( !m_pTexture[i]->CreateTexture(FONT_CACHE_SIZE, FONT_CACHE_SIZE, MYFONTCOLORFMT) )
		{
			ErrorLogMsg( KEM_ERROR6, m_strFontName.c_str() );
			return false;
		}

		if( m_iOutLineSize > 0 )
		{
			m_pTextureOutLine[i] = new CKTDGDynamicTexture();

			if( !m_pTextureOutLine[i]->CreateTexture(FONT_CACHE_SIZE, FONT_CACHE_SIZE, MYFONTCOLORFMT) )
			{
				ErrorLogMsg( KEM_ERROR7, m_strFontName.c_str() );
				return false;
			}
		}
	}

	InitCache();

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
    int iNumVertices = 6 * m_iNumCandidatesInAPage;

    for( int i = 0; i < ARRAY_SIZE( m_apPageVertex ); i++ )
    {
        if ( m_apPageVertex[ i ] != NULL )
            free( m_apPageVertex[ i ] );
        m_apPageVertex[ i ] = (void*) malloc( iNumVertices * ( ( m_bRHW == true ) ? sizeof(S2DUIVertex) : sizeof(SLVERTEX) ) );
        ASSERT( m_apPageVertex[ i ] != NULL );
    }//for
#endif
	m_bLoad = true;
	return true;
}

void CKTDGDeviceFontBase::InitCache()
{
	m_iTotalCache = (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize) * FONT_CACHE_PAGE;
#ifdef FONT_CASH_DATA_STRUCTURE_REFORM

	m_iSizeArray = 0;

	m_FontCacheArray = new SGCFontCacheBase* [m_iTotalCache];

	for(int i=0; i<m_iTotalCache; i++)
	{
		m_FontCacheArray[i] = new SGCFontCacheBase();
	}
	
#ifdef KTDGDEVICEFONT_SIZE_CACHE
    for( int i = 0; i < HASH_BUCKET_SIZE; i++ )
    {
        m_FontCacheHashTable[ i ].Init();
    }//for
#else
	memset(m_FontCacheHashTable, NULL, sizeof(SGCFontCacheBase*)*HASH_BUCKET_SIZE);
#endif
#endif FONT_CASH_DATA_STRUCTURE_REFORM
}

void CKTDGDeviceFontBase::UnLoad(bool bComplete)
{
	if( m_bLoad == false)
		return;

	m_bLoad = false;

	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		SAFE_DELETE(m_pTexture[i]);		
		SAFE_DELETE(m_pTextureOutLine[i]);
	}

	SelectObject(m_hDC, m_hOldBitmap );
	SelectObject( m_hDC, m_hOldPen );
	SelectObject( m_hDC, m_hOldFont );

	DeleteObject( m_hBitmap );
	DeleteObject( m_hOldPen );
	DeleteObject( m_hOldFont );

	DeleteDC( m_hDC );

#ifdef FONT_CASH_DATA_STRUCTURE_REFORM

	for(int i=0; i<m_iTotalCache; i++)
	{
		SAFE_DELETE(m_FontCacheArray[i]);
	}
	SAFE_DELETE_ARRAY(m_FontCacheArray);

#ifdef KTDGDEVICEFONT_SIZE_CACHE
    for( int i = 0; i < HASH_BUCKET_SIZE; i++ )
    {
        m_FontCacheHashTable[ i ].Init();
    }//for
#else
	memset(m_FontCacheHashTable, NULL, sizeof(SGCFontCacheBase*)*HASH_BUCKET_SIZE);
#endif

#else
	std::map< WCHAR, SGCFontCacheBase* >::iterator itor = m_mapChar.begin();
	while( itor != m_mapChar.end() )
	{
		SAFE_DELETE( itor->second );
		++itor;
	}
	m_mapChar.clear();
#endif FONT_CASH_DATA_STRUCTURE_REFORM
	
#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	for( int i = 0; i < ARRAY_SIZE( m_apPageVertex ); i++ )
	{
		if ( m_apPageVertex[ i ] != NULL )
		{
			free( m_apPageVertex[ i ] );
			m_apPageVertex[ i ] = NULL;
		}//if
	}//for
#endif
}

std::string CKTDGDeviceFontBase::MakeFontID( std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight/* = FW_NORMAL*/ )
{
	std::stringstream stream;
	stream << strFontName << iFontSize << "_" << iOutLineSize << "_" << bRHW << "_" << fontWeight;
	//static char strTemp[MAX_PATH];	
	//sprintf(strTemp,"%s%d_%d_%d_%d",strFontName.c_str(), iFontSize, iOutLineSize, bRHW, fontWeight );
	strFontName = stream.str();

#ifdef	CONVERSION_VS
	_strupr_s( (char*)strFontName.c_str(), strFontName.length() ); 
#else	CONVERSION_VS
	strupr( (char*)strFontName.c_str() ); 
#endif	CONVERSION_VS	

	debugPrintf( "%s", strFontName.c_str() );
	return strFontName;
}

#ifdef KTDGDEVICEFONT_BACKGROUND_PRELOADING
void CKTDGDeviceFontBase::PreloadFont( const WCHAR* pwstr )
{

	if ( m_bLoad == false || m_bPreload == true )
		return;

	if ( pwstr != NULL && pwstr[0] != NULL )
	{
		while( *pwstr != NULL )
		{
			TextOutW( m_hDC, 0, MAX_FONT_SIZE+1, pwstr, 1 );
			pwstr++;
		}//while
	}

	m_bPreload = true;

}//CKTDGDeviceFontBase::PreloadFont()
#endif

void CKTDGDeviceFontBase::CharToSystemTexture( SGCFontCacheBase* cache)
{	
	COLORREF black = RGB(0,0,0);
	TextOutW( m_hDC, 0, 0, &cache->_Char, 1 );
	COLORREF _col;

	MYFONTCOLOR_PTR pFirst = (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits;

	for(int y = 0; y < cache->iHeight; y++)
	{
		pFirst = (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits + (y + cache->iTop ) * FONT_CACHE_SIZE;

		for(int x = 0; x < cache->iWidth; x++)
		{
			if( y < m_iOutLineSize || y >= cache->iHeight - m_iOutLineSize || 
				x < m_iOutLineSize || x >= cache->iWidth -  m_iOutLineSize )
			{
				*(pFirst + x + cache->iLeft ) = 0;
			}
			else
			{			
				//투명한 부분임			
				_col = GetPixel( m_hDC, x-m_iOutLineSize, y-m_iOutLineSize );			
				if( _col == black )
				{
					*(pFirst + x + cache->iLeft ) = 0;//MYD3DCOLOR_ARGB(0,0,0,0);
				}
				else
				{
					//alpha 를 0~f로 표현하자
					/* ARGB : 1 5 5 5 mode에서 사용
					int iCol = (int)((float)GetRValue(_col) * 0x1f / 255.0f);						
					*(pFirst + x + cache->iLeft ) = MYD3DCOLOR_ARGB(1,iCol,iCol,iCol);
					*/
					int iCol = (int)((float)GetRValue(_col));// * 255.0f / 255.0f);						
					*(pFirst + x + cache->iLeft ) = MYD3DCOLOR_ARGB(iCol,iCol,iCol,iCol);
				}			
			}
		}
	}

	//----------------------------------------------------------------------------------------------------------------------------------
	// 아웃라인 중심이 벗어나면 안되니깐.. 직접 아래와 같이 코딩합니다
	if( m_iOutLineSize != 0 )
	{
		MYFONTCOLOR_PTR pFirstFont		= (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits;
		MYFONTCOLOR_PTR pFirstOutLine	= (MYFONTCOLOR_PTR)m_pTextureOutLine[cache->iPage]->m_LockedRect.pBits;
		MYFONTCOLOR_PTR pColor;
		bool			bFind;

		for(int y = 0; y < cache->iHeight; y++)
		{			
			pFirstOutLine	= (MYFONTCOLOR_PTR)m_pTextureOutLine[cache->iPage]->m_LockedRect.pBits + (y + cache->iTop ) * FONT_CACHE_SIZE;			
			
			for(int x = 0; x < cache->iWidth; x++)
			{	
				bFind = false;
								
				// Outline만큼... 체크해보자.
				for(int yy = max(0, y-m_iOutLineSize); yy <= min( cache->iHeight - 1, y + m_iOutLineSize ) && !bFind; yy++ )
				{					
					pFirstFont		= (MYFONTCOLOR_PTR)m_pTexture[cache->iPage]->m_LockedRect.pBits + (yy + cache->iTop ) * FONT_CACHE_SIZE;
				//	*(pFirstOutLine + x + cache->iLeft ) = *(pFirstFont + max(0, x-m_iOutLineSize) + cache->iLeft );
				//	bFind = true;
				//	break;

					for(int xx = max(0, x-m_iOutLineSize); xx <= min( cache->iWidth - 1, x + m_iOutLineSize ) && !bFind; xx++ )
					{
						pColor = (pFirstFont + xx + cache->iLeft );
						if( MYFONTCOLOR_ALPHA(*pColor) > 0 )
						{
							*(pFirstOutLine + x + cache->iLeft ) = *pColor;
							bFind = true;
						}
					}	
				}

				if( bFind == false )
				{
					*(pFirstOutLine + x + cache->iLeft ) = 0;
				}
			}
		}
	}
	//----------------------------------------------------------------------------------------------------------------------------------
}

SGCFontCacheBase* CKTDGDeviceFontBase::GetFontCache(WCHAR _Char)
{
#ifdef FONT_CASH_DATA_STRUCTURE_REFORM

    {
#ifdef KTDGDEVICEFONT_SIZE_CACHE
        SGCFontCacheBase *pExistingCache = m_FontCacheHashTable[_Char].m_pFontCache;
#else
        SGCFontCacheBase *pExistingCache = m_FontCacheHashTable[_Char];
#endif

	    if( pExistingCache != NULL)
	    {
		    pExistingCache->dwLastUse = m_dwCurrentStamp++;
		    return pExistingCache;
	    }
    }

	if(m_iSizeArray >= m_iTotalCache)
	{
		DWORD dwLastUseMin = m_FontCacheArray[0]->dwLastUse;
		int IdxMin = 0;
		for(int i=1; i<m_iTotalCache; i++)
		{
			if( (LONG) ( dwLastUseMin - m_FontCacheArray[i]->dwLastUse ) > 0 )
			{
				IdxMin = i;
				dwLastUseMin = m_FontCacheArray[i]->dwLastUse;
			}
		}

#ifdef KTDGDEVICEFONT_SIZE_CACHE
		m_FontCacheHashTable[m_FontCacheArray[IdxMin]->_Char].m_pFontCache = NULL;
        m_FontCacheHashTable[_Char].m_pFontCache = m_FontCacheArray[IdxMin];
#else
		m_FontCacheHashTable[m_FontCacheArray[IdxMin]->_Char] = NULL;
		m_FontCacheHashTable[_Char] = m_FontCacheArray[IdxMin];
#endif
		
		SGCFontCacheBase *cache = m_FontCacheArray[IdxMin];

		cache->_Char = _Char;
		cache->dwLastUse = m_dwCurrentStamp++;

		SIZE size;
#ifdef KTDGDEVICEFONT_SIZE_CACHE
        _EstimateFontSize( _Char, &size );
#else
		GetTextExtentPoint32W( m_hDC, &cache->_Char, 1, &size);	
#endif

		if( size.cx >= m_iFontSize )
			size.cx = m_iFontSize;
		if( size.cy >= m_iFontSize )
			size.cy = m_iFontSize;
		cache->_SetRect(size.cx + m_iOutLineSize*2, size.cy + m_iOutLineSize*2 );
		CharToSystemTexture( cache );

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		RECT    rect;
		rect.left = cache->iLeft;
		rect.top  = cache->iTop;
		rect.right = cache->iLeft + cache->iWidth;
		rect.bottom = cache->iTop + cache->iHeight;

		m_pTexture[cache->iPage]->SetChange( rect );

		if( m_pTextureOutLine[cache->iPage] )
			m_pTextureOutLine[cache->iPage]->SetChange( rect );
#else
		m_pTexture[cache->iPage]->SetChange(true);

		if( m_pTextureOutLine[cache->iPage] )
			m_pTextureOutLine[cache->iPage]->SetChange(true);
#endif

		return cache;
	}

#ifdef KTDGDEVICEFONT_SIZE_CACHE	
	m_FontCacheHashTable[_Char].m_pFontCache = m_FontCacheArray[m_iSizeArray];
#else
	m_FontCacheHashTable[_Char] = m_FontCacheArray[m_iSizeArray];
#endif

	SGCFontCacheBase *cache = m_FontCacheArray[m_iSizeArray];

	const int iCharPerPage = (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize);
	//const int iCharPerPage = 10; // TEST CODE
	int iPage = m_iSizeArray / iCharPerPage;
	int iIndexInPage = m_iSizeArray % iCharPerPage;
	int y = iIndexInPage / (FONT_CACHE_SIZE / m_iFontCacheSize);
	int x = iIndexInPage % (FONT_CACHE_SIZE / m_iFontCacheSize);

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	if( iPage >= FONT_CACHE_PAGE )
	{
		WCHAR wszText[256] = L"";
		swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"Font Cache Page : %d", iPage );
		ErrorLogMsg( KEM_ERROR8, wszText );
		iPage = 0;
	}
#endif

	cache->iPage = iPage;
	cache->SetPos(x,y,m_iFontCacheSize );
	cache->_Char = _Char;
	cache->dwLastUse = m_dwCurrentStamp++;
	SIZE size;
#ifdef KTDGDEVICEFONT_SIZE_CACHE
    _EstimateFontSize( _Char, &size );
#else
	GetTextExtentPoint32W( m_hDC, &cache->_Char, 1, &size);	
#endif
	if( size.cx >= m_iFontSize )
		size.cx = m_iFontSize;
	if( size.cy >= m_iFontSize )
		size.cy = m_iFontSize;
	cache->_SetRect( size.cx + m_iOutLineSize*2, size.cy + m_iOutLineSize*2 );

	CharToSystemTexture( cache );

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	RECT    rect;
	rect.left = cache->iLeft;
	rect.top  = cache->iTop;
	rect.right = cache->iLeft + cache->iWidth;
	rect.bottom = cache->iTop + cache->iHeight;

	m_pTexture[cache->iPage]->SetChange( rect );
	if( m_pTextureOutLine[cache->iPage] )
		m_pTextureOutLine[cache->iPage]->SetChange( rect );
#else
	m_pTexture[cache->iPage]->SetChange(true);

	if( m_pTextureOutLine[cache->iPage] )
		m_pTextureOutLine[cache->iPage]->SetChange(true);
#endif

	m_iSizeArray++;

	return cache;


#else
	std::map< WCHAR, SGCFontCacheBase* >::iterator itor = m_mapChar.find(_Char);

	if( itor != m_mapChar.end() )
	{
		itor->second->dwLastUse = m_dwCurrentStamp++;
		return itor->second;
	}

	//자리가 없으므로 자리를 마련함
	if( (int)m_mapChar.size() >= m_iTotalCache )
	{		
		std::vector<SGCFontCacheBase*> vecCache;
#ifdef DYNAMIC_VERTEX_BUFFER_OPT		
        vecCache.reserve( m_mapChar.size() );
#endif
		std::map< WCHAR, SGCFontCacheBase* >::iterator i = m_mapChar.begin();

		while( i != m_mapChar.end() )
		{
			vecCache.push_back( i->second );
			i++;
		}

		std::sort( vecCache.begin(), vecCache.end(), CKTDGFontCacheScheduler() );
		SGCFontCacheBase *cache = vecCache[0];

		m_mapChar.erase( m_mapChar.find(cache->_Char) );
		cache->_Char = _Char;
		cache->dwLastUse = m_dwCurrentStamp++;
		SIZE size;
#ifdef  KTDGDEVICEFONT_SIZE_CACHE
        _EstimateFontSize( _Char, &size );
#else   KTDGDEVICEFONT_SIZE_CACHE
		GetTextExtentPoint32W( m_hDC, &cache->_Char, 1, &size);	
#endif  KTDGDEVICEFONT_SIZE_CACHE
		if( size.cx >= m_iFontSize )
			size.cx = m_iFontSize;
		if( size.cy >= m_iFontSize )
			size.cy = m_iFontSize;
		cache->_SetRect(size.cx + m_iOutLineSize*2, size.cy + m_iOutLineSize*2 );
		CharToSystemTexture( cache );
#ifdef FIX_MEMORY_VIOLATE
		m_mapChar.insert( std::map< WCHAR, SGCFontCacheBase* >::value_type(_Char, cache) );
#else
		m_mapChar[_Char] = cache;
#endif FIX_MEMORY_VIOLATE

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		RECT    rect;
		rect.left = cache->iLeft;
		rect.top  = cache->iTop;
		rect.right = cache->iLeft + cache->iWidth;
		rect.bottom = cache->iTop + cache->iHeight;

		m_pTexture[cache->iPage]->SetChange( rect );
		if( m_pTextureOutLine[cache->iPage] )
			m_pTextureOutLine[cache->iPage]->SetChange( rect );
			
		m_mapChar[_Char] = cache;
#else
		m_pTexture[cache->iPage]->SetChange(true);

		if( m_pTextureOutLine[cache->iPage] )
			m_pTextureOutLine[cache->iPage]->SetChange(true);
#endif

		return cache;
	}


	SGCFontCacheBase *cache = new SGCFontCacheBase();
	const int iCharPerPage = (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize);
	//const int iCharPerPage = 10; // TEST CODE
	int iPage = (int)m_mapChar.size() / iCharPerPage;
	int iIndexInPage = (int)m_mapChar.size() % iCharPerPage;
	int y = iIndexInPage / (FONT_CACHE_SIZE / m_iFontCacheSize);
	int x = iIndexInPage % (FONT_CACHE_SIZE / m_iFontCacheSize);

	cache->iPage = iPage;

	cache->SetPos(x,y,m_iFontCacheSize );
	cache->_Char = _Char;
	cache->dwLastUse = m_dwCurrentStamp++;
	SIZE size;
#ifdef KTDGDEVICEFONT_SIZE_CACHE
    _EstimateFontSize( _Char, &size );
#else
	GetTextExtentPoint32W( m_hDC, &cache->_Char, 1, &size);	
#endif
	if( size.cx >= m_iFontSize )
		size.cx = m_iFontSize;
	if( size.cy >= m_iFontSize )
		size.cy = m_iFontSize;
	cache->_SetRect( size.cx + m_iOutLineSize*2, size.cy + m_iOutLineSize*2 );

	CharToSystemTexture( cache );

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	RECT    rect;
	rect.left = cache->iLeft;
	rect.top  = cache->iTop;
	rect.right = cache->iLeft + cache->iWidth;
	rect.bottom = cache->iTop + cache->iHeight;

	m_pTexture[cache->iPage]->SetChange( rect );
	if( m_pTextureOutLine[cache->iPage] )
		m_pTextureOutLine[cache->iPage]->SetChange( rect );
	
	m_mapChar[_Char] = cache;
#else
	m_pTexture[cache->iPage]->SetChange(true);

	if( m_pTextureOutLine[cache->iPage] )
		m_pTextureOutLine[cache->iPage]->SetChange(true);
#endif

	return cache;
#endif FONT_CASH_DATA_STRUCTURE_REFORM
}


#ifdef DYNAMIC_VERTEX_BUFFER_OPT
void CKTDGDeviceFontBase::Flush(bool bOutLine, int iPage )
{
	KTDXPROFILE();

	if ( iPage >= FONT_CACHE_PAGE )
		return;

	int iStartPage = ( iPage < 0 ) ? 0 : iPage;
	int iEndPage = ( iPage < 0 ) ? ( FONT_CACHE_PAGE - 1 ) : iPage;

	KD3DPUSH( m_RenderStateID )

#ifndef DYNAMIC_VERTEX_BUFFER_OPT
	HRESULT hr;
#endif

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
#ifdef DYNAMIC_VERTEX_BUFFER_OPT

                BOOST_STATIC_ASSERT( S2DUIVertex::FVF == D3DFVF_XYZRHW_DIFFUSE_TEX1 );
                g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZRHW_DIFFUSE_TEX1, D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_apPageVertex[i] );
#else

			    g_pKTDXApp->GetDevice()->SetFVF( S2DUIVertex::FVF );
			    hr = g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_apPageVertex[i], sizeof( S2DUIVertex ) );

#endif
		    }
		    else
		    {

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
                BOOST_STATIC_ASSERT( SLVERTEX::FVF == D3DFVF_XYZ_DIFFUSE_TEX1 );
                g_pKTDXApp->GetDVBManager()->DrawPrimitive( CKTDGDynamicVBManager::DVB_TYPE_XYZ_DIFFUSE_TEX1, D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_apPageVertex[i] );
#else
			    g_pKTDXApp->GetDevice()->SetFVF( SLVERTEX::FVF );
			    hr = g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_apPageVertex[i], sizeof( SLVERTEX ) );
#endif
		    }

		m_iCandidate[i] = 0;
	}

	KD3DEND()
}
#else
void CKTDGDeviceFontBase::Flush(bool bOutLine)
{
	KTDXPROFILE();

	KD3DPUSH( m_RenderStateID )

		HRESULT hr;

	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{	
		if( m_iCandidate[i] == 0 )
			continue;

		if( bOutLine )
			m_pTextureOutLine[i]->SetTexture(0);
		else
			m_pTexture[i]->SetTexture(0);

		if( m_iCandidate[i]*2 > MAX_VERTEX_IN_PAGE )
		{
			WCHAR wszText[256] = L"";
			swprintf( wszText, sizeof(wszText) / sizeof(WCHAR) - 1, L"Font Vertex Over : %d/%d", m_iCandidate[i]*2, MAX_VERTEX_IN_PAGE );
			ErrorLogMsg( KEM_ERROR8, wszText );
		}

		if( m_bRHW == true )
		{
			g_pKTDXApp->GetDevice()->SetFVF( S2DUIVertex::FVF );
			hr = g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iCandidate[i]*2,  m_pUIVertex[i], sizeof( S2DUIVertex ) );
		}
		else
		{
			g_pKTDXApp->GetDevice()->SetFVF( SLVERTEX::FVF );
			hr = g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iCandidate[i]*2, m_pVertex[i], sizeof( SLVERTEX ) );
		}

		if ( hr != D3D_OK )
		{
			ErrorLogMsg( KEM_ERROR8, m_strFontName.c_str() );
		}

		m_iCandidate[i] = 0;
	}

	KD3DEND()
}
#endif

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
void CKTDGDeviceFontBase::Buffering( bool bOutLine, const int& iLeft, const int& iTop, const WCHAR* str, const D3DCOLOR& color, const D3DCOLOR& DefColor,bool bNoColor, int nCount, float fScaleX, float fScaleY )
{
	if ( str == NULL )
		return;

	int isize = ( nCount < 0 ) ? (int)wcslen(str) : nCount;
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

		SGCFontCacheBase *cache = GetFontCache(str[i]);
        ASSERT( cache != NULL && cache->iPage >= 0 && cache->iPage < FONT_CACHE_PAGE
            && m_apPageVertex[cache->iPage] != NULL );
        if ( false == ( cache != NULL && cache->iPage >= 0 && cache->iPage < FONT_CACHE_PAGE ) )
            return;
        if ( m_apPageVertex[cache->iPage] == NULL )
        {
            m_iCandidate[ cache->iPage ] = 0;
            return;
        }//if

#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		if ( m_iCandidate[cache->iPage] >= m_iNumCandidatesInAPage )
        {
            Flush( bOutLine, cache->iPage );
            ASSERT( m_iCandidate[cache->iPage] < m_iNumCandidatesInAPage );
            if ( m_iCandidate[cache->iPage] >= m_iNumCandidatesInAPage )
#else
		if ( m_iCandidate[cache->iPage] >= PAGE_VERTEX_MULTIPLE * (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize ) )
		{
			Flush( bOutLine, cache->iPage );
			ASSERT( m_iCandidate[cache->iPage] < PAGE_VERTEX_MULTIPLE * (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize ) );
			if ( m_iCandidate[cache->iPage] >= PAGE_VERTEX_MULTIPLE * (FONT_CACHE_SIZE / m_iFontCacheSize) * (FONT_CACHE_SIZE / m_iFontCacheSize ) )
#endif
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
			bufferingILeft += ( ( cache->iWidth /m_EnlargeNum ) - 1) * fScaleX;
		else
			bufferingILeft += ( ( cache->iWidth /m_EnlargeNum ) ) * fScaleX;
		//}} robobeg : 2011-11-17
		m_iCandidate[cache->iPage]++;
	}	
}

int CKTDGDeviceFontBase::GetWidth( const WCHAR* wszText, int nCount )
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
#ifdef KTDGDEVICEFONT_SIZE_CACHE
            size.cx = _EstimateStringWidth( m_wstrBuffer.c_str(), iLength );
#else
			GetTextExtentPoint32W( m_hDC, m_wstrBuffer.c_str(), iLength, &size);
#endif
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
#ifdef KTDGDEVICEFONT_SIZE_CACHE
    size.cx = _EstimateStringWidth( m_wstrBuffer.c_str(), iLength );
#else
	GetTextExtentPoint32W( m_hDC, m_wstrBuffer.c_str(), iLength, &size);	
#endif
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

void CKTDGDeviceFontBase::OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color, D3DCOLOR colorOutLine, RECT* pRt, DWORD dwFlag, int nCount, float fScaleX, float fScaleY )
{
	KTDXPROFILE();

	if( wszText == NULL || wszText[0] == 0 )
		return;

	int iFinalLeft = GetLeftPos( iLeft, wszText, pRt, dwFlag, nCount, fScaleX );
	
	m_dwCurrentStamp = timeGetTime();
#ifndef DYNAMIC_VERTEX_BUFFER_OPT
	
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		if ( m_pTexture[i] != NULL )
			m_pTexture[i]->SetChange(false);

		if( m_pTextureOutLine[i] != NULL )
			m_pTextureOutLine[i]->SetChange(false);
	}
#endif

	D3DCOLOR curcol;

	if( m_iOutLineSize )
	{
		curcol = colorOutLine;
		Buffering( true, iFinalLeft, iTop, (WCHAR*)wszText, curcol, colorOutLine, true, nCount, fScaleX, fScaleY );
		Flush( true );
	}

	curcol = color;
	Buffering( false, iFinalLeft, iTop, (WCHAR*)wszText, curcol, color, (dwFlag & DT_NO_COLOR) != 0, nCount, fScaleX, fScaleY );
	Flush(false);	
}


void CKTDGDeviceFontBase::OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color, D3DCOLOR colorOutLine, float fLineSpace, RECT* pRt, DWORD dwFlag, int nCount, float fScaleX, float fScaleY )
{
	if( wszText == NULL || wszText[0] == 0 || nCount == 0 )
		return;

#ifndef DYNAMIC_VERTEX_BUFFER_OPT
	m_dwCurrentStamp = timeGetTime();
	for(int i = 0; i < FONT_CACHE_PAGE; i++)
	{
		m_pTexture[i]->SetChange(false);
		if( m_pTextureOutLine[i] )
		{
			m_pTextureOutLine[i]->SetChange(false);
		}
	}
#endif

	static WCHAR			wstrBuf[VIRTUAL_FONT_LINE_MAX];

	int iOrgLength = wcslen( wszText );
	if ( nCount < 0 )
		nCount = iOrgLength;
	else
		nCount = __min( nCount, iOrgLength );
	nCount = __min( nCount, VIRTUAL_FONT_LINE_MAX - 1 );

#ifdef	CONVERSION_VS
	StringCchCopy( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
	//wcscpy_s( wstrBuf, VIRTUAL_FONT_LINE_MAX, wszText );
#else	CONVERSION_VS
	wcscpy( wstrBuf, wszText );
#endif	CONVERSION_VS
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

				Buffering( true, iLineLeft, iTopLocal, pToken, curcol, color, true, -1, fScaleX, fScaleY );

				if( bNewLine )
				{
					if( m_bRHW == true )
						iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace * fScaleY );
					else
						iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace * fScaleY );
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

			Buffering( false, iLineLeft, iTopLocal, pToken, curcol, color, (dwFlag & DT_NO_COLOR) != 0, -1, fScaleX, fScaleY );
			//Display Here
			if( bNewLine )
			{
				if( m_bRHW == true )
					iTopLocal += (int)((m_iFontCacheSize / m_EnlargeNum) * fLineSpace * fScaleY );
				else
					iTopLocal -= (int)((m_iFontCacheSize / m_EnlargeNum ) * fLineSpace * fScaleY );
			}			

			pToken = &wstrBuf[i+1];
		}
	}

	Flush(false);
}
#endif