#include "StdAfx.h"
#include ".\ktdgfontmanager.h"

CKTDGFontManager::CKTDGFontManager( LPDIRECT3DDEVICE9 pd3dDevice )
{
#ifdef USE_FREE_TYPE
	int error = FT_Init_FreeType( &m_FreeTypeLibrary );
	if( error )
	{
		wstringstream strstm;
		strstm << "Font Init Failed : returns " << error;
		MessageBoxW( NULL, strstm.str().c_str(), L"Error!", NULL );
		ErrorLogMsg( KEM_ERROR426, strstm.str().c_str() );
	}
#endif //USE_FREE_TYPE
}

CKTDGFontManager::~CKTDGFontManager(void)
{
#ifdef USE_FREE_TYPE
	FT_Done_FreeType( m_FreeTypeLibrary );
#endif //USE_FREE_TYPE
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	for( int i = 0; i < (int)m_FontList.size(); i++ )
	{
		SAFE_DELETE( m_FontList[i] );
	}
	m_FontList.clear();
#endif

	for( int i = 0; i < (int)m_UKFontList.size(); i++ )
	{
		SAFE_DELETE( m_UKFontList[i] );
	}
	m_UKFontList.clear();

#ifdef DLL_BUILD
	for ( int i = 0; i < (int)m_UseFontNameList.size(); i++ )
	{
		wstring fontName = m_UseFontNameList[i];
		//RemoveFontResourceEx( fontName.c_str(), FR_PRIVATE, NULL );
		if( RemoveFontResource( fontName.c_str() ) == false )
		{
			int i = 0;
		}
	}
#endif DLL_BUILD
}



void CKTDGFontManager::OnFrameMove( float fTime, float fElapsedTime )
{
#ifndef X2VIEWER //JHKang
	KLagCheck( eKnown_LagCheckType_KTDGFontManager_FrameMove );
#endif //X2VIEWER

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	for( int i = 0; i < (int)m_FontList.size(); i++ )
	{
		m_FontList[i]->OnFrameMove( fTime, fElapsedTime );
	}
#endif

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	for( int i = 0; i < (int) m_UKFontList.size(); i++ )
	{
		CUKFont* pFont = m_UKFontList[i];
		if ( pFont == NULL )
			continue;
		pFont->OnFrameMove( fElapsedTime );
	}
#endif
}

void CKTDGFontManager::OnFrameRender()
{
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	for( int i = 0; i < (int)m_FontList.size(); i++ )
	{
		m_FontList[i]->OnFrameRender();
	}
#endif

#ifdef  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	for( int i = 0; i < (int) m_UKFontList.size(); i++ )
	{
		CUKFont* pFont = m_UKFontList[i];
		if ( pFont == NULL )
			continue;
		pFont->OnFrameRender();
	}
#endif
}



HRESULT	CKTDGFontManager::OnResetDevice()
{
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	for( int i = 0; i < (int)m_FontList.size(); i++ )
	{
		if ( m_FontList[i] != NULL )
			m_FontList[i]->OnResetDevice();
	}
#endif

	for( int i = 0; i < (int)m_UKFontList.size(); i++ )
	{
		if ( m_UKFontList[i] != NULL )
			m_UKFontList[i]->OnResetDevice();
	}

	return S_OK;
}

HRESULT	CKTDGFontManager::OnLostDevice()
{
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	for( int i = 0; i < (int)m_FontList.size(); i++ )
	{
		if ( m_FontList[i] != NULL )
			m_FontList[i]->OnLostDevice();
	}
#endif

	for( int i = 0; i < (int)m_UKFontList.size(); i++ )
	{
		if ( m_UKFontList[i] != NULL )
			m_UKFontList[i]->OnLostDevice();
	}

	return S_OK;
}


#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
CKTDGFontManager::CKTDGFont* CKTDGFontManager::CreateNewFont(  const WCHAR* pFontName, int Height, int Width, int Weight )
{
	/*
	if ( m_mapFontList.find( fontIndex ) != m_mapUKFontList.end() )
	{

		return NULL;
	}
*/
	CKTDGFont* pFont = new CKTDGFont( g_pKTDXApp->GetDevice(), pFontName, Height, Width, Weight );
	m_FontList.push_back( pFont );




	//m_mapFontList.insert(map<string, string>::value_type( fontIndex, pFont );

	return pFont;
}
#endif

void CKTDGFontManager::CreateNewUIFont_LUA( int fontID, const char* pFontName, int Height, int Weight )
{
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	wstring fontName;
	ConvertUtf8ToWCHAR( fontName, pFontName );
	CKTDGFont* pFont = CreateNewFont( fontName.c_str(), Height, 0, Weight );

	g_pKTDXApp->GetDGManager()->GetDialogManager()->SetFont( fontID, pFont );
#endif
}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
void CKTDGFontManager::CreateNewUIUKFont_LUA( int fontID, const char* pFontName, int Height, int Weight, int enlargeNum, int iOutlineSize, bool bNoRes )
//#else
//void CKTDGFontManager::CreateNewUIUKFont_LUA( int fontID, const char* pFontName, int Height, int Weight, int enlargeNum )
//#endif
{
	wstring fontName;
	ConvertUtf8ToWCHAR( fontName, pFontName );
	CUKFont* pFont;

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	pFont = CreateNewUKFont( fontName.c_str(), Height, iOutlineSize, true, Weight, enlargeNum, bNoRes );
//#else
//	pFont = CreateNewUKFont( fontName.c_str(), Height, 1, true, Weight, enlargeNum );
//#endif
	
	g_pKTDXApp->GetDGManager()->GetDialogManager()->SetUKFont( fontID, pFont );
}

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
bool CKTDGFontManager::DestroyFont( CKTDGFont* pFont )
{
	for( int i = 0; i < (int)m_FontList.size(); i++ )
	{
		if( pFont == m_FontList[i] )
		{
			SAFE_DELETE( pFont );
			m_FontList.erase( m_FontList.begin() + i );
			return true;
		}
	}
	return false;
}
#endif

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
void CKTDGFontManager::MapUIFontToUKFont( int fontID, int ukFontID )
{
	g_pKTDXApp->GetDGManager()->GetDialogManager()->MapFontToUKFont( fontID, ukFontID );
}//CKTDGFontManager::MapUIFontToUKFont()
#endif


//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
CKTDGFontManager::CUKFont*	CKTDGFontManager::CreateNewUKFont( const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight /*= FW_NORMAL*/, int enlargeNum /*= 1*/, bool bNoRes )
//#else
//CKTDGFontManager::CUKFont*	CKTDGFontManager::CreateNewUKFont( const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight /*= FW_NORMAL*/, int enlargeNum /*= 1*/ )
//#endif
{
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	CUKFont* pFont = new CUKFont( wstrFontName, iFontSize, iOutLineSize, bRHW, fontWeight, enlargeNum, bNoRes );
//#else
//	CUKFont* pFont = new CUKFont( wstrFontName, iFontSize, iOutLineSize, bRHW, fontWeight, enlargeNum );
//#endif
	m_UKFontList.push_back( pFont );

	//m_mapUKFontList.insert(map<string, string>::value_type( fontIndex, pFont );

	return pFont;
}

bool CKTDGFontManager::DestroyUKFont( CUKFont* pFont )
{
	for( int i = 0; i < (int)m_UKFontList.size(); i++ )
	{
		if( pFont == m_UKFontList[i] )
		{
			SAFE_DELETE( pFont );
			m_UKFontList.erase( m_UKFontList.begin() + i );
			return true;
		}
	}
	return false;
}

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
CKTDGFontManager::CKTDGFont::CKTDGFont( LPDIRECT3DDEVICE9 pDevice, const WCHAR* pFontName, int Height, int Width, int Weight )
{
	m_pFont			= NULL;
	m_pTextSprite	= NULL;
	m_pMatrix		= NULL;
	m_TextID		= 0;

	m_Height		= Height;
	m_Width			= Width;
	m_Weight		= Weight;
	m_FontName		= pFontName;

	HRESULT hr;

	hr = D3DXCreateFont( g_pKTDXApp->GetDevice(), Height, Width, Weight, 1, FALSE, DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
					pFontName, &m_pFont );

	if ( hr != S_OK )
	{
		m_pFont = NULL;
		wstringstream strstm;
		strstm << pFontName << L"_";
		switch( hr )
		{
			case D3DERR_INVALIDCALL:
				strstm << L"D3DERR_INVALIDCALL";
				break;

			case D3DXERR_INVALIDDATA:
				strstm << L"D3DXERR_INVALIDDATA";
				break;

			case E_OUTOFMEMORY:
				strstm << L"E_OUTOFMEMORY";
				break;
		}
		ErrorLogMsg( KEM_ERROR14, strstm.str().c_str() );
		
		hr = D3DXCreateFont( g_pKTDXApp->GetDevice(), Height, Width, Weight, 1, FALSE, DEFAULT_CHARSET, 
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
			L"굴림", &m_pFont );

		m_FontName = L"굴림";

	}

	//SetTextCharacterExtra( m_pFont->GetDC(), 2 );

	hr = D3DXCreateSprite( g_pKTDXApp->GetDevice(), &m_pTextSprite );

	if ( hr != S_OK )
	{
		string strName;
		ConvertWCHARToChar( strName, pFontName );
		ErrorLogMsg( KEM_ERROR15, strName.c_str() );
	}

	m_pMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );

	D3DXMatrixIdentity( &m_matIdentityWorld );
}

CKTDGFontManager::CKTDGFont::~CKTDGFont(void)
{
	Clear2DFont();
	Clear3DFont();
	ClearProjectsFont();

	SAFE_DELETE( m_pMatrix );
	SAFE_RELEASE( m_pTextSprite );
	SAFE_RELEASE( m_pFont );
}

HRESULT	CKTDGFontManager::CKTDGFont::ResetFont( int Height, int Width, int Weight, WCHAR* pFontName )
{
	SAFE_RELEASE( m_pFont );

	if( Height != -1 )
		m_Height		= Height;
	if( Width != -1 )
		m_Width			= Width;
	if( Weight != -1 )
		m_Weight		= Weight;
	if( pFontName != NULL )
		m_FontName		= pFontName;

	return D3DXCreateFont( g_pKTDXApp->GetDevice(), 
					(int)(m_Height * g_pKTDXApp->GetResolutionScaleY()), 
					(int)(m_Width * g_pKTDXApp->GetResolutionScaleX()), 
					Weight, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
					DEFAULT_PITCH | FF_DONTCARE, pFontName, &m_pFont );
}

int CKTDGFontManager::CKTDGFont::Push2DText( const WCHAR* strMsg, int xPos, int yPos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag, FONT_STYLE fontStyle, bool bInstance, bool bSpread, float fSpreadTimeGap )
{

	if( bSpread == true )
	{
		FontArticle* pFontArticle = new FontArticle;

		m_TextID++;

		pFontArticle->bSpread		 = true;
		pFontArticle->fSpreadTimeGap = fSpreadTimeGap;
		pFontArticle->fNowSpreadTime = 0.f;
		pFontArticle->iSpreadCount	 = 0;

		pFontArticle->color			= color;
		pFontArticle->outLineColor	= outlineColor;
		pFontArticle->pos.x			= (float)xPos;
		pFontArticle->pos.y			= (float)yPos;
		pFontArticle->strMsg		= strMsg;
		pFontArticle->bInstance		= bInstance;
		pFontArticle->sortFlag		= sortFlag;
		pFontArticle->ID			= m_TextID;
		pFontArticle->fontStyle		= fontStyle;

		m_vecFont2D.push_back( pFontArticle );

		return m_TextID;
	}
	else
	{
		FontArticle* pFontArticle = new FontArticle;

		m_TextID++;

		pFontArticle->color			= color;
		pFontArticle->outLineColor	= outlineColor;
		pFontArticle->pos.x			= (float)xPos;
		pFontArticle->pos.y			= (float)yPos;
		pFontArticle->strMsg		= strMsg;
		pFontArticle->bInstance		= bInstance;
		pFontArticle->sortFlag		= sortFlag;
		pFontArticle->ID			= m_TextID;
		pFontArticle->fontStyle		= fontStyle;

		m_vecFont2D.push_back( pFontArticle );

		return m_TextID;
	}
}

void CKTDGFontManager::CKTDGFont::Draw2DText( const WCHAR* strMsg, int xPos, int yPos, 
											 D3DXCOLOR color, D3DXCOLOR outlineColor,
											 DWORD sortFlag /* = DT_LEFT */,
											 FONT_STYLE fontStyle /* = FS_NONE */,
											 int nCount /* = -1  */)
{
	KTDXPROFILE();

	if( NULL == strMsg )
		return;

	if ( (int)wcslen(strMsg) == 0 )
		return;

	m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	g_pKTDXApp->SetWorldTransform( &m_matIdentityWorld );

	RECT rect;
	rect.left	= xPos;
	rect.top	= yPos;

	D3DXMATRIX matTransform;
	D3DXMatrixScaling( &matTransform, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY(), g_pKTDXApp->GetResolutionScaleX() );
	m_pTextSprite->SetTransform( &matTransform );

	DrawFont( strMsg, rect, sortFlag, color, outlineColor, fontStyle, nCount );

	m_pTextSprite->End();
}

CKTDGFontManager::FontArticle* CKTDGFontManager::CKTDGFont::Get2DArticle( const int ID )
{
	for( int i = 0; i < (int)m_vecFont2D.size(); ++i )
	{
		if( m_vecFont2D[i]->ID == ID )
		{
			return m_vecFont2D[i];
		}
	}

	return NULL;
}

bool CKTDGFontManager::CKTDGFont::Delete2DFont( const int ID )
{
	for( int i = 0; i < (int)m_vecFont2D.size(); ++i )
	{
		if( m_vecFont2D[i]->ID == ID )
		{
			SAFE_DELETE( m_vecFont2D[i] );
			m_vecFont2D.erase( m_vecFont2D.begin() + i );
			return true;
		}
	}

	return false;
}

void CKTDGFontManager::CKTDGFont::Clear2DFont()
{
	int i = 0;
	int nFontNum = 0;

	nFontNum = (int)m_vecFont2D.size();
	for( int i = 0; i < nFontNum; ++i )
	{
		SAFE_DELETE( m_vecFont2D[i] );
	}
	m_vecFont2D.clear();
}

int CKTDGFontManager::CKTDGFont::Push3DText( WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag, FONT_STYLE fontStyle, bool bInstance /* = true  */)
{
	FontArticle* pFontArticle = new FontArticle;

	m_TextID++;

	pFontArticle->color			= color;
	pFontArticle->outLineColor	= outlineColor;
	pFontArticle->pos			= pos;
	pFontArticle->strMsg		= strMsg;
	pFontArticle->sortFlag		= sortFlag;
	pFontArticle->bInstance		= bInstance;
	pFontArticle->ID			= m_TextID;
	pFontArticle->fontStyle		= fontStyle;

	m_vecFont3D.push_back( pFontArticle );

	return m_TextID;
}

void CKTDGFontManager::CKTDGFont::Draw3DText( WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag, FONT_STYLE fontStyle )
{
	if( NULL == strMsg )
		return;

	if ( (int)wcslen(strMsg) == 0 )
		return;

	m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE );

	m_pMatrix->Move( pos );
	m_pMatrix->Rotate( D3DXVECTOR3( D3DX_PI, 0.0f, 0.0f ) );
	m_pMatrix->UpdateWorldMatrix( CKTDGMatrix::BT_ALL );

	RECT rect;
	rect.left	= 0;
	rect.top	= 0;
	rect.right	= 0;
	rect.bottom = 0;

	DrawFont( strMsg, rect, sortFlag, color, outlineColor, fontStyle );

	m_pTextSprite->End();
}

CKTDGFontManager::FontArticle* CKTDGFontManager::CKTDGFont::Get3DArticle( const int ID )
{
	for( int i = 0; i < (int)m_vecFont3D.size(); ++i )
	{
		if( m_vecFont3D[i]->ID == ID )
		{
			return m_vecFont3D[i];
		}
	}

	return NULL;
}

bool CKTDGFontManager::CKTDGFont::Delete3DFont( const int ID )
{
	for( int i = 0; i < (int)m_vecFont3D.size(); ++i )
	{
		if( m_vecFont3D[i]->ID == ID )
		{
			SAFE_DELETE( m_vecFont3D[i] );
			m_vecFont3D.erase( m_vecFont3D.begin() + i );
			return true;
		}
	}
	return false;
}

void CKTDGFontManager::CKTDGFont::Clear3DFont()
{
	int i = 0;
	int nFontNum = 0;

	nFontNum = (int)m_vecFont3D.size();
	for( int i = 0; i < nFontNum; ++i )
	{
		SAFE_DELETE( m_vecFont3D[i] );
	}
	m_vecFont3D.clear();
}

int CKTDGFontManager::CKTDGFont::PushProjectsText( const WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag, FONT_STYLE fontStyle, bool bInstance /* = true  */)
{
	FontArticle*	pFontArticle = new FontArticle;

	m_TextID++;

	pFontArticle->color			= color;
	pFontArticle->outLineColor	= outlineColor;
	pFontArticle->pos			= pos;
	pFontArticle->strMsg		= strMsg;
	pFontArticle->bInstance		= bInstance;
	pFontArticle->sortFlag		= sortFlag;
	pFontArticle->ID			= m_TextID;
	pFontArticle->fontStyle		= fontStyle;

	m_vecFontProjects.push_back( pFontArticle );

	return m_TextID;
}

void CKTDGFontManager::CKTDGFont::DrawProjectsText( const WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag, FONT_STYLE fontStyle )
{
	if( NULL == strMsg )
		return;

	if ( (int)wcslen(strMsg) == 0 )
		return;

	m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	D3DXVECTOR3 vOut;

	m_pTextSprite->SetTransform( &m_matIdentityWorld );
	g_pKTDXApp->SetWorldTransform( &m_matIdentityWorld );

	//D3DXMATRIX matTransform;
	//D3DXMatrixScaling( &matTransform, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY(), g_pKTDXApp->GetResolutionScaleX() );
	//m_pTextSprite->SetTransform( &matTransform );
	D3DXVec3Project( &vOut, &pos, &g_pKTDXApp->GetViewport(), &m_matProjection, &m_matView, &m_matIdentityWorld );

	// Cull
	if ( 0.f > vOut.z || vOut.z > 1.f )
	{
		//KDbgStm << "Text Out Cull" << trace;
		return;
	}

	RECT rect;
	rect.left	= (LONG)vOut.x;
	rect.top	= (LONG)vOut.y;

	DrawFont( strMsg, rect, sortFlag, color, outlineColor, fontStyle );

	m_pTextSprite->End();
}

CKTDGFontManager::FontArticle* CKTDGFontManager::CKTDGFont::GetProjectsArticle( const int ID )
{
	for( int i = 0; i < (int)m_vecFontProjects.size(); ++i )
	{
		if( m_vecFontProjects[i]->ID == ID )
		{
			return m_vecFontProjects[i];
		}
	}
	return NULL;
}

bool CKTDGFontManager::CKTDGFont::DeleteProjectsFont( const int ID )
{
	for( int i = 0; i < (int)m_vecFontProjects.size(); ++i )
	{
		if( m_vecFontProjects[i]->ID == ID )
		{
			SAFE_DELETE( m_vecFontProjects[i] );
			m_vecFontProjects.erase( m_vecFontProjects.begin() + i );
			return true;
		}
	}
	return false;
}

void CKTDGFontManager::CKTDGFont::ClearProjectsFont()
{
	int i = 0;
	int nFontNum = 0;

	nFontNum = (int)m_vecFontProjects.size();

	for( int i = 0; i < nFontNum; ++i )
	{
		SAFE_DELETE( m_vecFontProjects[i] );
	}
	m_vecFontProjects.clear();
}

void CKTDGFontManager::CKTDGFont::DrawTextFont(const WCHAR* strMsg, RECT* pos, int nCount, D3DXCOLOR color, bool bOutline, D3DXCOLOR outlineColor, DWORD sortFlag )
{
	if( NULL == strMsg )
		return; 

	if ( (int)wcslen(strMsg) == 0 )
		return;

	D3DXMATRIX matTransform;
	D3DXMatrixScaling( &matTransform, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY(), g_pKTDXApp->GetResolutionScaleX() );
	m_pTextSprite->SetTransform( &matTransform );

	m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

	g_pKTDXApp->SetWorldTransform( &m_matIdentityWorld );

	if( bOutline == true )
		DrawFontStyle( strMsg, *pos, sortFlag, outlineColor, FS_SHELL );

	m_pFont->DrawText( m_pTextSprite, strMsg, nCount, pos, sortFlag, color );

	m_pTextSprite->End();
/*

	wstring stringMsg = strMsg;
	wstring realStringMsg = L"";
	realStringMsg = stringMsg.substr( 0, nCount );

	CKTDGFontManager::CUKFont* pUkFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 0 );
	if ( bOutline == true )
	{
		pUkFont->OutTextMultiline( pos->left, (int)( ( pos->bottom + pos->top ) / 2.0f )  - (int)(pUkFont->GetFontSize()/2.0f) , realStringMsg.c_str(), color, CKTDGFontManager::FS_SHELL, outlineColor, 1.0f, NULL, sortFlag );
	}
	else
	{
		pUkFont->OutTextMultiline( pos->left, (int)( ( pos->bottom + pos->top ) / 2.0f) - (int)(pUkFont->GetFontSize()/2.0f), realStringMsg.c_str(), color, CKTDGFontManager::FS_NONE, outlineColor, 1.0f, NULL, sortFlag );
	}
*/
}

void CKTDGFontManager::CKTDGFont::CalcTextRect( const WCHAR* strMsg, RECT* pos, int nCount, DWORD format /* = (DT_VCENTER | DT_CALCRECT */)
{
	//strMsg 글자수에 따른 영역 알아위해
	if( strMsg == NULL || wcscmp( strMsg, L"" ) == 0 )
		return;

	m_pFont->DrawText( NULL, strMsg, nCount, pos, format, 0x00000000 );

	/*
	pos->left	= (LONG)((float)pos->left * g_pKTDXApp->GetResolutionScaleX());
	pos->top	= (LONG)((float)pos->top * g_pKTDXApp->GetResolutionScaleY());
	pos->right	= (LONG)((float)pos->right * g_pKTDXApp->GetResolutionScaleX());
	pos->bottom	= (LONG)((float)pos->bottom * g_pKTDXApp->GetResolutionScaleY());
	*/
}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
void CKTDGFontManager::CKTDGFont::Flush()
{
	int nFontNum = 0;
	int i = 0;

	nFontNum = (int)m_vecFont2D.size();

	//float   fFontScaleY = (float) m_pUKFont->GetAdjustedFontSize() / (float) m_pUKFont->GetOrgFontSize();
	//float   fAspectRatio = g_pKTDXApp->GetResolutionScaleX() / fFontScaleY;
	float   fScaleX = g_pKTDXApp->GetResolutionScaleX();
	float   fScaleY = g_pKTDXApp->GetResolutionScaleY();

	if( nFontNum != 0 )
	{
		for( i = 0; i < nFontNum; ++i )
		{
			FontArticle* pArticle	= m_vecFont2D[i];

			if ( pArticle == NULL )
				continue;

			pArticle->rect.left		= (int)pArticle->pos.x;
			pArticle->rect.top		= (int)pArticle->pos.y;


			if( true == pArticle->bSpread )
			{
				wstring wstrTemp = pArticle->strMsg.substr(0, pArticle->iSpreadCount );

				/*if ( pArticle->sortFlag & DT_SINGLELINE )
					OutTextXY( pArticle->rect.left, pArticle->rect.top, wstrTemp.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, &pArticle->rect, pArticle->sortFlag, wstrTemp.size(), fScaleX, fScaleY );
				else
					OutTextMultiline( pArticle->rect.left, pArticle->rect.top, wstrTemp.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, 1.f, &pArticle->rect, pArticle->sortFlag, wstrTemp.size(), fScaleX, fScaleY );*/

			}
			else
			{
				/*if ( pArticle->sortFlag & DT_SINGLELINE )
					OutTextXY( pArticle->rect.left, pArticle->rect.top, pArticle->strMsg.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, &pArticle->rect, pArticle->sortFlag, pArticle->strMsg.size(), fScaleX, fScaleY );
				else
					OutTextMultiline( pArticle->rect.left, pArticle->rect.top, pArticle->strMsg.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, 1.f, &pArticle->rect, pArticle->sortFlag, pArticle->strMsg.size(), fScaleX, fScaleY );*/
			}		

			if( pArticle->bInstance == true )
			{
				SAFE_DELETE( pArticle );
				m_vecFont2D.erase( m_vecFont2D.begin() + i );
				--nFontNum;
				--i;
			}
		}

		nFontNum = 0;
	}
}
//#else
//void CKTDGFontManager::CKTDGFont::Flush()
//{
//	int nFontNum = 0;
//	int i = 0;
//
//	m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
//
//	D3DXMATRIX matTransform;
//	D3DXMatrixScaling( &matTransform, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY(), g_pKTDXApp->GetResolutionScaleX() );
//	m_pTextSprite->SetTransform( &matTransform );
//
//	nFontNum = (int)m_vecFont2D.size();
//
//	if( nFontNum != 0 )
//	{
//		for( i = 0; i < nFontNum; ++i )
//		{
//			FontArticle* pArticle	= m_vecFont2D[i];
//
//			if ( pArticle == NULL )
//				continue;
//
//			pArticle->rect.left		= (int)pArticle->pos.x;
//			pArticle->rect.top		= (int)pArticle->pos.y;
//
//
//			if( true == pArticle->bSpread )
//			{
//				wstring wstrTemp = pArticle->strMsg.substr(0, pArticle->iSpreadCount );
//
//				DrawFont( wstrTemp.c_str(), pArticle->rect, pArticle->sortFlag, pArticle->color, pArticle->outLineColor, pArticle->fontStyle );
//			}
//			else
//			{
//				DrawFont( pArticle->strMsg.c_str(), pArticle->rect, pArticle->sortFlag, pArticle->color, pArticle->outLineColor, pArticle->fontStyle );
//			}		
//
//			if( pArticle->bInstance == true )
//			{
//				SAFE_DELETE( pArticle );
//				m_vecFont2D.erase( m_vecFont2D.begin() + i );
//				--nFontNum;
//				--i;
//			}
//		}
//
//		nFontNum = 0;
//	}
//
//	nFontNum = (int)m_vecFontProjects.size();
//
//	if( nFontNum != 0 )
//	{
//		D3DXVECTOR3	vOut = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
//
//		m_pTextSprite->SetTransform( &m_matIdentityWorld );
//		g_pKTDXApp->SetWorldTransform( &m_matIdentityWorld );
//		g_pKTDXApp->GetProjectionTransform( &m_matProjection );
//		g_pKTDXApp->GetViewTransform( &m_matView );
//		
//		for( i = 0; i < nFontNum; ++i )
//		{
//			FontArticle* pArticle	= m_vecFontProjects[i];
//			if ( pArticle == NULL )
//				continue;
//
//			D3DXVec3Project( &vOut, &pArticle->pos, &g_pKTDXApp->GetViewport(), &m_matProjection, &m_matView, &m_matIdentityWorld );
//
//			// Cull
//			if ( 0.f > vOut.z || vOut.z > 1.f )
//			{
//				//KDbgStm << "Text Out Cull" << trace;
//				continue;
//			}
//
//			pArticle->rect.left	= (LONG)vOut.x;
//			pArticle->rect.top	= (LONG)vOut.y;
//
//			DrawFont( pArticle->strMsg.c_str(), pArticle->rect, pArticle->sortFlag, pArticle->color, pArticle->outLineColor, pArticle->fontStyle );
//			
//			if( pArticle->bInstance == true )
//			{
//				SAFE_DELETE( pArticle );
//				m_vecFontProjects.erase( m_vecFontProjects.begin() + i );
//				--nFontNum;
//				--i;
//			}
//		}
//
//		nFontNum = 0;
//	}
//	m_pTextSprite->End();
//
//	nFontNum = (int)m_vecFont3D.size();
//
//	m_pTextSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE );
//
//	if( nFontNum != 0 )
//	{
//		for( i = 0; i < nFontNum; ++i )
//		{
//			FontArticle* pArticle	= m_vecFont3D[i];
//			pArticle->rect.left		= 0;
//			pArticle->rect.top		= 0;
//			pArticle->rect.right	= 0;
//			pArticle->rect.bottom	= 0;
//
//			m_pMatrix->Move( pArticle->pos );
//			m_pMatrix->Rotate( D3DXVECTOR3( D3DX_PI, 0.0f, 0.0f ) );
//			m_pMatrix->UpdateWorldMatrix( CKTDGMatrix::BT_ALL );
//
//			DrawFont( pArticle->strMsg.c_str(), pArticle->rect, pArticle->sortFlag, pArticle->color, pArticle->outLineColor, pArticle->fontStyle );
//			
//			if( pArticle->bInstance == true )
//			{
//				SAFE_DELETE( pArticle );
//				m_vecFont3D.erase( m_vecFont3D.begin() + i );
//				--nFontNum;
//				--i;
//			}
//		}
//	}
//
//	m_pTextSprite->End();
//}
//#endif

void CKTDGFontManager::CKTDGFont::DrawFont( const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR color, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount /* = -1  */)
{
	KTDXPROFILE();

	if( NULL == strMsg )
		return;
    
	if ( (int)wcslen( strMsg ) == 0 )
	{
		return;
	}

	switch( sortFlag )
	{
		case DT_CENTER:
			{
				m_pFont->DrawText( m_pTextSprite, strMsg, nCount, &rt, DT_CALCRECT , 0xffffffff );
				int width	= rt.right - rt.left;
				rt.left		-= width/2;
				rt.right	-= width/2;
			}
			break;

		case DT_RIGHT:
			{
				m_pFont->DrawText( m_pTextSprite, strMsg, nCount, &rt, DT_CALCRECT , 0xffffffff );
				int width	= rt.right - rt.left;
				rt.left		-= width;
				rt.right	-= width;
			}
			break;
#ifdef USE_DT_VCENTER			
//{{ 2010. 11. 16 성현찬, 문자열 수직정렬
		case DT_VCENTER:
			{
				m_pFont->DrawText( m_pTextSprite, strMsg, nCount, &rt, DT_CALCRECT , 0xffffffff );
				int height	= rt.bottom - rt.top;
				rt.bottom	-= height/2;
				rt.top			-= height/2;
			}
			break;
//}}
#endif // USE_DT_VCENTER
#ifdef _HV_FONT_SORT_
		case DT_HVCENTER:
			{
				m_pFont->DrawText( m_pTextSprite, strMsg, nCount, &rt, DT_CALCRECT , 0xffffffff );
				int width	= rt.right - rt.left;
				int height	= rt.bottom - rt.top;
				rt.left		-= width/2;
				rt.right	-= width/2;				
				rt.bottom	-= height/2;
				rt.top			-= height/2;
			}
			break;
#endif _HV_FONT_SORT_
	}

	if( fontStyle != FS_NONE )
	{
		//DrawFontStyle( strMsg, rt, DT_LEFT | DT_NOCLIP, outlineColor, fontStyle );
		//{{ 허상형 : [2009/10/28] //	FS_NONE이 아닐때도 nCount 입력 하도록 수정
		DrawFontStyle( strMsg, rt, DT_LEFT | DT_NOCLIP, outlineColor, fontStyle, nCount );
		//}} 허상형 : [2009/10/28] //	FS_NONE이 아닐때도 nCount 입력 하도록 수정
	}

	m_pFont->DrawText( m_pTextSprite, strMsg, nCount, &rt, DT_LEFT | DT_NOCLIP, color );
	/*

	CKTDGFontManager::CUKFont* pUkFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 0 );
	pUkFont->OutTextMultiline( rt.left, (int)( ( rt.bottom + rt.top ) / 2.0f) - (int)(pUkFont->GetFontSize()/2.0f) , strMsg, color, fontStyle, outlineColor, 1.0f, &rt, sortFlag );
*/
}

void CKTDGFontManager::CKTDGFont::DrawFontStyle(const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount /* = -1  */)
{
	KTDXPROFILE();

	if( NULL == strMsg )
		return;

	if ( (int)wcslen( strMsg ) == 0 )
	{
		return;
	}

	LONG	gap = 1;
	RECT	rect = rt;

	switch( fontStyle )
	{
	case FS_NONE:
		break;
	case FS_SHADOW:
		rect.left	+= gap; 
		rect.top	+= gap;
		rect.right	+= gap;
		rect.bottom	+= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount, &rect, sortFlag, outlineColor );
		break;
	case FS_SHELL:
		rect		= rt;
		rect.left	-= gap; 
		rect.right	-= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );
		rect		= rt;
		rect.left	+= gap; 
		rect.right	+= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );
		rect		= rt;
		rect.top	-= gap; 
		rect.bottom	-= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );
		rect		= rt;
		rect.top	+= gap; 
		rect.bottom	+= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );

		rect		= rt;
		rect.left	-= gap; 
		rect.top	-= gap;
		rect.right	-= gap; 
		rect.bottom	-= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );
		rect		= rt;
		rect.left	-= gap; 
		rect.top	+= gap;
		rect.right	-= gap; 
		rect.bottom	+= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );
		rect		= rt;
		rect.left	+= gap; 
		rect.top	-= gap;
		rect.right	+= gap; 
		rect.bottom	-= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );
		rect		= rt;
		rect.left	+= gap; 
		rect.top	+= gap;
		rect.right	+= gap; 
		rect.bottom	+= gap;
		m_pFont->DrawText( m_pTextSprite, strMsg, nCount,&rect, sortFlag, outlineColor );
		break;
	}
}

void CKTDGFontManager::CKTDGFont::OnFrameMove( float fTime, float fElapsedTime )
{
	int nFontNum = 0;
	int i = 0;

	nFontNum = (int)m_vecFont2D.size();

	for( i = 0; i < nFontNum; i++ )
	{
		FontArticle* pArticle = m_vecFont2D[i];
		if( false == pArticle->bSpread )
			continue;

		pArticle->fNowSpreadTime += fElapsedTime;
		if( pArticle->fNowSpreadTime < pArticle->fSpreadTimeGap )
			continue;

		pArticle->fNowSpreadTime = 0.f;

		if( pArticle->iSpreadCount < (int) pArticle->strMsg.length() )
			pArticle->iSpreadCount++;
	}
}

void CKTDGFontManager::CKTDGFont::OnFrameRender()
{
	Flush();
}

HRESULT CKTDGFontManager::CKTDGFont::OnResetDevice()
{
	HRESULT hr;

	if( m_pFont )
		V_RETURN( m_pFont->OnResetDevice() );

	if( m_pTextSprite )
		V_RETURN( m_pTextSprite->OnResetDevice() );

	//ResetFont( m_Height, m_Width );

	return S_OK;
}

HRESULT CKTDGFontManager::CKTDGFont::OnLostDevice()
{
	HRESULT hr;

	if( m_pFont )
		V_RETURN( m_pFont->OnLostDevice() );

	if( m_pTextSprite )
		V_RETURN( m_pTextSprite->OnLostDevice() );

	return S_OK;
}
#endif





//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
CKTDGFontManager::CUKFont::CUKFont( const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, 
	bool bRHW, int fontWeight/* = FW_NORMAL */, int enlargeNum /* = 1 */, bool bNoRes )
//#else
//CKTDGFontManager::CUKFont::CUKFont( const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, 
//								   bool bRHW, int fontWeight/* = FW_NORMAL */, int enlargeNum /* = 1 */ )
//#endif
{
	string strFontName;
	ConvertWCHARToChar( strFontName, wstrFontName );
	
#ifdef USE_FREE_TYPE
	m_pUKFont = new CKTDGFreeTypeFont( strFontName, iFontSize, iOutLineSize, bRHW, fontWeight, enlargeNum, bNoRes );
#else //USE_FREE_TYPE

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	m_pUKFont = new CKTDGDeviceFont( strFontName, iFontSize, iOutLineSize, bRHW, fontWeight, enlargeNum, bNoRes );
//#else
//	m_pUKFont = new CKTDGDeviceFont( strFontName, iFontSize, iOutLineSize, bRHW, fontWeight, enlargeNum );
//#endif //DYNAMIC_VERTEX_BUFFER_OPT

#endif //USE_FREE_TYPE
	m_pUKFont->Load();

	D3DXMatrixIdentity( &m_matIdentity );

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_TextID = 0;
#endif
}

CKTDGFontManager::CUKFont::~CUKFont(void)
{
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	Clear2DFont();
#endif

	m_pUKFont->UnLoad();
	SAFE_DELETE( m_pUKFont );
}

void CKTDGFontManager::CUKFont::OnResetDevice()
{
	if ( m_pUKFont != NULL )
		m_pUKFont->OnResetDevice();
}
void CKTDGFontManager::CUKFont::OnLostDevice()
{
	if ( m_pUKFont != NULL )
		m_pUKFont->OnLostDevice();
}

bool CKTDGFontManager::CUKFont::Load()
{
	return m_pUKFont->Load();
}
void CKTDGFontManager::CUKFont::UnLoad(bool bComplete /*= true */)
{
	m_pUKFont->UnLoad( bComplete );
}

std::string CKTDGFontManager::CUKFont::MakeFontID( std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight/* = FW_NORMAL*/ )
{
#ifdef USE_FREE_TYPE
	return "";
#else //USE_FREE_TYPE
	return CKTDGDeviceFont::MakeFontID( strFontName, iFontSize, iOutLineSize, bRHW, fontWeight );
#endif //USE_FREE_TYPE
}

int CKTDGFontManager::CUKFont::GetWidth( WCHAR wLetter ) const
{
	return m_pUKFont->GetWidth( wLetter );
}
int CKTDGFontManager::CUKFont::GetHeight( WCHAR wLetter ) const
{
	return m_pUKFont->GetHeight( wLetter );
}
int CKTDGFontManager::CUKFont::GetWidth( const WCHAR* wszText ) const
{
	return m_pUKFont->GetWidth( wszText );
}
int CKTDGFontManager::CUKFont::GetHeight( const WCHAR* wszText ) const
{
	KTDXPROFILE();

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

	if( NULL == m_pUKFont )
	{
		// ERROR; m_pUKFONT shouldn't be NULL
		return 1;
	}

#ifdef KTDGDEVICEFONT_SIZE_CACHE
	int stringHeight = enterNum * m_pUKFont->GetHeight();
#else
	int stringHeight = enterNum * m_pUKFont->GetHeight( wszText );
#endif
	return stringHeight;
}

int CKTDGFontManager::CUKFont::GetPaddedHeight( const WCHAR* wszText, int iAddHeight ) const
{
	KTDXPROFILE();

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

	if( NULL == m_pUKFont )
	{
		// ERROR; m_pUKFONT shouldn't be NULL
		return 1;
	}

#ifdef KTDGDEVICEFONT_SIZE_CACHE
	int stringHeight = enterNum * ( m_pUKFont->GetHeight() + iAddHeight );
#else
	int stringHeight = enterNum * (m_pUKFont->GetHeight( wszText ) + iAddHeight);
#endif
	return stringHeight;
}


//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
void CKTDGFontManager::CUKFont::OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color /*= 0xffffffff*/, CKTDGFontManager::FONT_STYLE fontStyle /* = FS_NONE */, D3DCOLOR colorOutLine /*= 0xff000000*/, RECT* pRt /* = NULL */, DWORD dwFlag /* = DT_LEFT|DT_TOP  */, int nCount /*= -1 */, float fScaleX, float fScaleY )
//#else
//void CKTDGFontManager::CUKFont::OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color /*= 0xffffffff*/, 
//										  CKTDGFontManager::FONT_STYLE fontStyle /* = FS_NONE */, D3DCOLOR colorOutLine /*= 0xff000000*/,
//										  RECT* pRt /* = NULL */, DWORD dwFlag /* = DT_LEFT|DT_TOP  */)
//#endif
{
	KTDXPROFILE();

	if ( fontStyle == FS_NONE || fontStyle == FS_SHADOW )
		colorOutLine = D3DXCOLOR( 0, 0, 0, 0 );

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	RECT    rtScale;

	if ( pRt != NULL )
	{
		rtScale.left = (int) ( pRt->left * g_pKTDXApp->GetResolutionScaleX() );
		rtScale.top  = (int) ( pRt->top * g_pKTDXApp->GetResolutionScaleY() );
		rtScale.right = (int) ( pRt->right * g_pKTDXApp->GetResolutionScaleX() );
		rtScale.bottom = (int) ( pRt->bottom * g_pKTDXApp->GetResolutionScaleY() );
		pRt = &rtScale;
	}//if

	m_pUKFont->OutTextXY( (int)( iLeft * g_pKTDXApp->GetResolutionScaleX() )
		, (int)( iTop * g_pKTDXApp->GetResolutionScaleY() ), wszText, color, colorOutLine, pRt, dwFlag, nCount
		, fScaleX, fScaleY );
//#else
//	m_pUKFont->OutTextXY( (int)( iLeft * g_pKTDXApp->GetResolutionScaleX() ), (int)( iTop * g_pKTDXApp->GetResolutionScaleY() ), wszText, color, colorOutLine, pRt, dwFlag );
//#endif
}

void CKTDGFontManager::CUKFont::OutProjectionText( const D3DXVECTOR3& pos, const WCHAR* wszText, D3DCOLOR color /*= 0xffffffff*/, 
												  CKTDGFontManager::FONT_STYLE fontStyle /* = FS_NONE */, D3DCOLOR colorOutLine /*= 0xff000000*/, 
												  RECT* pRt /* = NULL */, DWORD dwFlag /* = DT_LEFT|DT_TOP  */ )
{
	KTDXPROFILE();

	D3DXVECTOR3 screenPos;

	KTDXPROFILE_BEGIN( "projection to screen" );

	D3DXMatrixIdentity( &m_matIdentity );
	g_pKTDXApp->SetWorldTransform( &m_matIdentity );
	g_pKTDXApp->GetProjectionTransform( &m_matProj );
	g_pKTDXApp->GetViewTransform( &m_matView );

	D3DVIEWPORT9 kViewPort = g_pKTDXApp->GetViewport();

	// Process
	D3DXVec3Project( &screenPos, &pos, &kViewPort, &m_matProj, &m_matView, &m_matIdentity );

	// Cull	
	if ( 0.f > screenPos.z || screenPos.z > 1.f )
	{
		//KDbgStm << "Text Out Cull" << trace;
		return;
	}

	if ( fontStyle == FS_NONE || fontStyle == FS_SHADOW )
		colorOutLine = D3DXCOLOR( 0, 0, 0, 0 );
	KTDXPROFILE_END();


	KTDXPROFILE_BEGIN( "m_pUKFont->OutTextXY" );
	m_pUKFont->OutTextXY( (int)screenPos.x, (int)screenPos.y, wszText, color, colorOutLine, pRt, dwFlag );
	KTDXPROFILE_END();
}

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
void CKTDGFontManager::CUKFont::OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color /*= 0xffffffff*/, CKTDGFontManager::FONT_STYLE fontStyle /* = FS_NONE */, D3DCOLOR colorOutLine /*= 0xff000000*/, float fLineSpace /* = 1.0f */, RECT* pRt /* = NULL */, DWORD dwFlag /* = DT_LEFT|DT_TOP */, int nCount /* = -1 */, float fScaleX, float fScaleY )
//#else
//void CKTDGFontManager::CUKFont::OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color /*= 0xffffffff*/, 
//												 CKTDGFontManager::FONT_STYLE fontStyle /* = FS_NONE */, D3DCOLOR colorOutLine /*= 0xff000000*/, 
//												 float fLineSpace /* = 1.0f */, RECT* pRt /* = NULL */, DWORD dwFlag /* = DT_LEFT|DT_TOP */)
//#endif
{
	KTDXPROFILE();

	if ( fontStyle == FS_NONE || fontStyle == FS_SHADOW )
		colorOutLine = D3DXCOLOR( 0, 0, 0, 0 );

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	RECT    rtScale;
	if ( pRt != NULL )
	{
		rtScale.left = (int) ( pRt->left * g_pKTDXApp->GetResolutionScaleX() );
		rtScale.top  = (int) ( pRt->top * g_pKTDXApp->GetResolutionScaleY() );
		rtScale.right = (int) ( pRt->right * g_pKTDXApp->GetResolutionScaleX() );
		rtScale.bottom = (int) ( pRt->bottom * g_pKTDXApp->GetResolutionScaleY() );
		pRt = &rtScale;
	}//if

	m_pUKFont->OutTextMultiline( (int)( iLeft * g_pKTDXApp->GetResolutionScaleX() ), 
								(int)( iTop * g_pKTDXApp->GetResolutionScaleY() ), 
								wszText, color, colorOutLine, fLineSpace, pRt, dwFlag, nCount, 
								fScaleX, fScaleY );
//#else
//	m_pUKFont->OutTextMultiline( (int)( iLeft * g_pKTDXApp->GetResolutionScaleX() ), (int)( iTop * g_pKTDXApp->GetResolutionScaleY() ), wszText, color, colorOutLine, fLineSpace, pRt, dwFlag );
//#endif
}

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
void CKTDGFontManager::CUKFont::DrawTextFont( const WCHAR* strMsg, RECT* pos, int nCount, D3DXCOLOR color, bool bOutline, D3DXCOLOR outlineColor, DWORD sortFlag )
{
	if ( pos == NULL )
		return;

	if ( strMsg == NULL || strMsg[0] == NULL || nCount == 0 )
		return;

	if ( bOutline == true )
		DrawFont( strMsg, *pos, sortFlag, color, outlineColor, FS_SHELL, nCount );
	else
		DrawFont( strMsg, *pos, sortFlag, color, 0xff000000, FS_NONE, nCount );
}

void CKTDGFontManager::CUKFont::CalcTextRect( const WCHAR* strMsg, RECT* pos, int nCount, DWORD format )
{
	if ( pos == NULL )
		return;

	if ( strMsg == NULL || strMsg[0] == NULL || nCount == 0 )
	{
		pos->left = pos->top = pos->right = pos->bottom = 0;
		return;
	}//if

	if ( format & DT_SINGLELINE )
	{
		pos->bottom = pos->top + m_pUKFont->GetHeight();
		pos->right = pos->left + m_pUKFont->GetWidth( strMsg, nCount );
	}
	else
	{
		pos->right = pos->left + m_pUKFont->GetWidth( strMsg, nCount );
		pos->bottom = pos->top + m_pUKFont->GetHeight( strMsg, nCount );
	}//if.. else..
}

void CKTDGFontManager::CUKFont::DrawFont( const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR color, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount /* = -1  */)
{
	KTDXPROFILE();

	if( NULL == strMsg || strMsg[0] == NULL || nCount == 0 )
		return;

	switch( sortFlag )
	{
		case DT_CENTER:
			{
				CalcTextRect( strMsg, &rt, nCount, 0 );
				int width	= rt.right - rt.left;
				rt.left		-= width/2;
				rt.right	-= width/2;
			}
			break;

		case DT_RIGHT:
			{
				CalcTextRect( strMsg, &rt, nCount, 0 );
				int width	= rt.right - rt.left;
				rt.left		-= width;
				rt.right	-= width;
			}
			break;

		case DT_VCENTER:
			{
				int height	= rt.bottom - rt.top;
				int fontHeight = m_pUKFont->GetHeight( strMsg, nCount );
				rt.top		= rt.top + height/2 - fontHeight/2;
				rt.bottom	= rt.bottom - height/2 + fontHeight/2;
			}
			break;
#ifdef _HV_FONT_SORT_
		case DT_HVCENTER:
			{
				CalcTextRect( strMsg, &rt, nCount, 0 );
				int width	= rt.right - rt.left;
				rt.left		-= width/2;
				rt.right	-= width/2;

				int height	= rt.bottom - rt.top;
				int fontHeight = m_pUKFont->GetHeight( strMsg, nCount );
				rt.top		= rt.top + height/2 - fontHeight/2;
				rt.bottom	= rt.bottom - height/2 + fontHeight/2;
			}
			break;
#endif _HV_FONT_SORT_
	}

    RECT    rtScale;
    rtScale.left = (int) ( rt.left * g_pKTDXApp->GetResolutionScaleX() );
    rtScale.top  = (int) ( rt.top * g_pKTDXApp->GetResolutionScaleY() );
    rtScale.right = (int) ( rt.right * g_pKTDXApp->GetResolutionScaleX() );
    rtScale.bottom = (int) ( rt.bottom * g_pKTDXApp->GetResolutionScaleY() );
	    
    if ( fontStyle != FS_NONE )
    {
        DrawFontStyle( strMsg, rt, DT_LEFT | DT_NOCLIP, outlineColor, fontStyle, nCount );
        m_pUKFont->OutTextMultiline( rtScale.left
            , rtScale.top
            , strMsg, color, 0xff000000, 1.0f, &rtScale, sortFlag, nCount
#ifndef FIX_CARET_POSITION_AND_FONT_SIZE
			, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY() 
#endif //FIX_CARET_POSITION_AND_FONT_SIZE
			);
    }
    else
    {
        m_pUKFont->OutTextMultiline( rtScale.left
            , rtScale.top
			, strMsg, color, 0xff000000, 1.0f, &rtScale, sortFlag, nCount
#ifndef FIX_CARET_POSITION_AND_FONT_SIZE
			, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY() 
#endif //FIX_CARET_POSITION_AND_FONT_SIZE
			);
    }//if.. else..
}

void CKTDGFontManager::CUKFont::DrawFontStyle(const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount /* = -1  */)
{
	KTDXPROFILE();

	if( NULL == strMsg || strMsg[0] == NULL || nCount == 0 )
		return;

	LONG	gap = 1;
	RECT	rect = rt;
	RECT rtScale;
	float   fXScale = g_pKTDXApp->GetResolutionScaleX();
	float   fYScale = g_pKTDXApp->GetResolutionScaleY();

	switch( fontStyle )
	{
	case FS_NONE:
		break;
	case FS_SHADOW:
		rect.left	+= gap; 
		rect.top	+= gap;
		rect.right	+= gap;
		rect.bottom	+= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		break;
	case FS_SHELL:
		rect		= rt;
		rect.left	-= gap; 
		rect.right	-= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		rect		= rt;
		rect.left	+= gap; 
		rect.right	+= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		rect		= rt;
		rect.top	-= gap; 
		rect.bottom	-= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		rect		= rt;
		rect.top	+= gap; 
		rect.bottom	+= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);

		rect		= rt;
		rect.left	-= gap; 
		rect.top	-= gap;
		rect.right	-= gap; 
		rect.bottom	-= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		rect		= rt;
		rect.left	-= gap; 
		rect.top	+= gap;
		rect.right	-= gap; 
		rect.bottom	+= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		rect		= rt;
		rect.left	+= gap; 
		rect.top	-= gap;
		rect.right	+= gap; 
		rect.bottom	-= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		rect		= rt;
		rect.left	+= gap; 
		rect.top	+= gap;
		rect.right	+= gap; 
		rect.bottom	+= gap;
		rtScale.left = (int) ( rect.left * fXScale );
		rtScale.top  = (int) ( rect.top * fYScale );
		rtScale.right = (int) ( rect.right * fXScale );
		rtScale.bottom = (int) ( rect.bottom * fYScale );
		m_pUKFont->OutTextMultiline( rtScale.left, rtScale.top, strMsg, outlineColor, 0xff000000, 1.0f, &rtScale, DT_LEFT|DT_TOP, nCount
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
#else FIX_CARET_POSITION_AND_FONT_SIZE
			, fXScale, fYScale
#endif FIX_CARET_POSITION_AND_FONT_SIZE
			);
		break;
	}
}

void CKTDGFontManager::CUKFont::Flush()
{
	int nFontNum = 0;
	int i = 0;

	nFontNum = (int)m_vecFont2D.size();

	//float   fFontScaleY = (float) m_pUKFont->GetAdjustedFontSize() / (float) m_pUKFont->GetOrgFontSize();
	//float   fAspectRatio = g_pKTDXApp->GetResolutionScaleX() / fFontScaleY;
	float   fScaleX = g_pKTDXApp->GetResolutionScaleX();
	float   fScaleY = g_pKTDXApp->GetResolutionScaleY();
	//float   fScaleX = 1.f;
	//float   fScaleY = 1.f;

	if( nFontNum != 0 )
	{
		for( i = 0; i < nFontNum; ++i )
		{
			FontArticle* pArticle	= m_vecFont2D[i];

			if ( pArticle == NULL )
				continue;

			pArticle->rect.left		= (int)pArticle->pos.x;
			pArticle->rect.top		= (int)pArticle->pos.y;


			if( true == pArticle->bSpread )
			{
				wstring wstrTemp = pArticle->strMsg.substr(0, pArticle->iSpreadCount );

				if ( pArticle->sortFlag & DT_SINGLELINE )
					OutTextXY( pArticle->rect.left, pArticle->rect.top, wstrTemp.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, &pArticle->rect, pArticle->sortFlag, wstrTemp.size(), fScaleX, fScaleY );
				else
					OutTextMultiline( pArticle->rect.left, pArticle->rect.top, wstrTemp.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, 1.f, &pArticle->rect, pArticle->sortFlag, wstrTemp.size(), fScaleX, fScaleY );

			}
			else
			{
				if ( pArticle->sortFlag & DT_SINGLELINE )
					OutTextXY( pArticle->rect.left, pArticle->rect.top, pArticle->strMsg.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, &pArticle->rect, pArticle->sortFlag, pArticle->strMsg.size(), fScaleX, fScaleY );
				else
					OutTextMultiline( pArticle->rect.left, pArticle->rect.top, pArticle->strMsg.c_str(), pArticle->color, pArticle->fontStyle
					, pArticle->outLineColor, 1.f, &pArticle->rect, pArticle->sortFlag, pArticle->strMsg.size(), fScaleX, fScaleY );
			}		

			if( pArticle->bInstance == true )
			{
				SAFE_DELETE( pArticle );
				m_vecFont2D.erase( m_vecFont2D.begin() + i );
				--nFontNum;
				--i;
			}
		}

		nFontNum = 0;
	}
}

void CKTDGFontManager::CUKFont::OnFrameMove( float fElapsedTime )
{
	int nFontNum = 0;
	int i = 0;

	nFontNum = (int)m_vecFont2D.size();

	for( i = 0; i < nFontNum; i++ )
	{
		FontArticle* pArticle = m_vecFont2D[i];
		if( false == pArticle->bSpread )
			continue;

		pArticle->fNowSpreadTime += fElapsedTime;
		if( pArticle->fNowSpreadTime < pArticle->fSpreadTimeGap )
			continue;

		pArticle->fNowSpreadTime = 0.f;

		if( pArticle->iSpreadCount < (int) pArticle->strMsg.length() )
			pArticle->iSpreadCount++;
	}
}

void CKTDGFontManager::CUKFont::OnFrameRender()
{
	Flush();
}

int CKTDGFontManager::CUKFont::Push2DText( const WCHAR* strMsg, int xPos, int yPos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag, FONT_STYLE fontStyle, bool bInstance, bool bSpread, float fSpreadTimeGap )
{
	if( bSpread == true )
	{
		FontArticle* pFontArticle = new FontArticle;

		m_TextID++;

		pFontArticle->bSpread		 = true;
		pFontArticle->fSpreadTimeGap = fSpreadTimeGap;
		pFontArticle->fNowSpreadTime = 0.f;
		pFontArticle->iSpreadCount	 = 0;

		pFontArticle->color			= color;
		pFontArticle->outLineColor	= outlineColor;
		pFontArticle->pos.x			= (float)xPos;
		pFontArticle->pos.y			= (float)yPos;
		pFontArticle->strMsg		= strMsg;
		pFontArticle->bInstance		= bInstance;
		pFontArticle->sortFlag		= sortFlag;
		pFontArticle->ID			= m_TextID;
		pFontArticle->fontStyle		= fontStyle;

		m_vecFont2D.push_back( pFontArticle );

		return m_TextID;
	}
	else
	{
		FontArticle* pFontArticle = new FontArticle;

		m_TextID++;

		pFontArticle->color			= color;
		pFontArticle->outLineColor	= outlineColor;
		pFontArticle->pos.x			= (float)xPos;
		pFontArticle->pos.y			= (float)yPos;
		pFontArticle->strMsg		= strMsg;
		pFontArticle->bInstance		= bInstance;
		pFontArticle->sortFlag		= sortFlag;
		pFontArticle->ID			= m_TextID;
		pFontArticle->fontStyle		= fontStyle;

		m_vecFont2D.push_back( pFontArticle );

		return m_TextID;
	}
}

void CKTDGFontManager::CUKFont::Draw2DText( const WCHAR* strMsg, int xPos, int yPos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag, FONT_STYLE fontStyle, int nCount )
{
	KTDXPROFILE();

	if( NULL == strMsg )
		return;

	RECT rect;
	rect.left		= xPos;
	rect.top		= yPos;
	rect.right		= xPos;
	rect.bottom		= yPos;

	//float   fFontScaleY = (float) m_pUKFont->GetAdjustedFontSize() / (float) m_pUKFont->GetOrgFontSize();
	//float   fAspectRatio = g_pKTDXApp->GetResolutionScaleX() / fFontScaleY;

	if ( sortFlag & DT_SINGLELINE )
		OutTextXY( rect.left, rect.top, strMsg, color, fontStyle
		, outlineColor, &rect, sortFlag, nCount
#ifndef REMOVE_TEXT_SCALING
		, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY()
#endif //REMOVE_TEXT_SCALING
		);
	else
		OutTextMultiline( rect.left, rect.top, strMsg, color, fontStyle
		, outlineColor, 1.0f, &rect, sortFlag, nCount
#ifndef REMOVE_TEXT_SCALING
		, g_pKTDXApp->GetResolutionScaleX(), g_pKTDXApp->GetResolutionScaleY()
#endif //REMOVE_TEXT_SCALING
		);

}

CKTDGFontManager::FontArticle* CKTDGFontManager::CUKFont::Get2DArticle( const int ID )
{
	for( int i = 0; i < (int)m_vecFont2D.size(); ++i )
	{
		if( m_vecFont2D[i]->ID == ID )
		{
			return m_vecFont2D[i];
		}
	}
	return NULL;
}

bool CKTDGFontManager::CUKFont::Delete2DFont( const int ID )
{
	for( int i = 0; i < (int)m_vecFont2D.size(); ++i )
	{
		if( m_vecFont2D[i]->ID == ID )
		{
			SAFE_DELETE( m_vecFont2D[i] );
			m_vecFont2D.erase( m_vecFont2D.begin() + i );
			return true;
		}
	}
	return false;
}

void CKTDGFontManager::CUKFont::Clear2DFont()
{
	int i = 0;
	int nFontNum = 0;

	nFontNum = (int)m_vecFont2D.size();
	for( int i = 0; i < nFontNum; ++i )
	{
		SAFE_DELETE( m_vecFont2D[i] );
	}
	m_vecFont2D.clear();
}
#endif

#ifdef DLL_BUILD
bool CKTDGFontManager::InstallFont( const char* pValueName, const char* pFontFileName )
{
	WCHAR FontPath[MAX_PATH] = {0};
	WCHAR tempFontFileName[MAX_PATH] = {0};
	WCHAR FontFileName[MAX_PATH] = {0};
	WCHAR FilePath[MAX_PATH] = {0};

	wstring fontFileName;
	wstring realFontFileName;
	ConvertUtf8ToWCHAR( fontFileName, pFontFileName );
	ConvertUtf8ToWCHAR( realFontFileName, pFontFileName );

	StringCchPrintf( FontFileName, MAX_PATH ,  L"%s", fontFileName.c_str() );
	StringCchPrintf( tempFontFileName, MAX_PATH , L"%s", fontFileName.c_str() );
	/*wsprintf( FontFileName, L"%s", fontFileName.c_str() );
	wsprintf( tempFontFileName, L"%s", fontFileName.c_str() );*/

	if ( g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->GetPullPath( tempFontFileName ) == NULL )
		return false;

	StringCchPrintf( FilePath, MAX_PATH , g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->GetPullPath( FontFileName ) );
	//wsprintf( FilePath, g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->GetPullPath( FontFileName ) );
	
	if ( AddFontResource( FilePath ) == FALSE )
	{		
		return false;
	}

	m_UseFontNameList.push_back( FilePath );

	return true;

}
#endif DLL_BUILD

#ifdef USE_FREE_TYPE
void CKTDGFontManager::InstallFont( string strFontName, string strFontPath )
{
#ifdef  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
    if ( strFontName.empty() == true || strFontPath.empty() == true )
        return;
    MakeUpperCase( strFontName );
    MakeUpperCase( strFontPath );

	if( m_mapFontNameFile.find( strFontName ) != m_mapFontNameFile.end() )
		return;
    m_mapFontNameFile[ strFontName ] = strFontPath;
    
    FontFileMemoryMap::const_iterator iter = m_mapFontFileMemory.find( strFontPath );
    if ( iter != m_mapFontFileMemory.end() )
        return;

#ifdef NO_LOCAL_FONT
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER  info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( strFontPath );
#else //NO_LOCAL_FONT
	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER  info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile_LocalFile( strFontPath );
#endif //NO_LOCAL_FONT
	if( info->pRealData == NULL || info->size <= 0 )
	{
		ErrorLogMsg( KEM_ERROR16, strFontPath.c_str() );
		MessageBoxA( NULL, strFontPath.c_str(), "Font Error!", NULL );
		return;
	}
    FontFileMemoryMap::_Pairib ib = m_mapFontFileMemory.insert( FontFileMemoryMap::value_type( strFontPath, std::vector<BYTE>() ) );
    ib.first->second.reserve( info->size );
    ib.first->second.assign( (const BYTE*) info->pRealData, (const BYTE*) ( info->pRealData + info->size ) );

#else   X2OPTIMIZE_FREE_TYPE_FONT_MEMORY

	// 이미 등록된 폰트는 무시합니다.
	if( m_mapUseFontMemory.find( strFontName ) != m_mapUseFontMemory.end() )
		return;
	m_mapUseFontMemory[ strFontName ] = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile_LocalFile( strFontPath );
	if( m_mapUseFontMemory[ strFontName ]->size <= 0 )
	{
		ErrorLogMsg( KEM_ERROR16, strFontPath.c_str() );
		MessageBoxA( NULL, strFontPath.c_str(), "Font Error!", NULL );
		return;
	}

#endif  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
}

#ifdef  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
const std::vector<BYTE>& CKTDGFontManager::GetFontMemory( string strFontName ) const
{
    static std::vector<BYTE>    s_vecDummy;

    if ( strFontName.empty() == true )
        return s_vecDummy;
    MakeUpperCase( strFontName );

	FontNameFileMap::const_iterator iter = m_mapFontNameFile.find( strFontName );
    if ( iter == m_mapFontNameFile.end() )
        return  s_vecDummy;

	FontFileMemoryMap::const_iterator mitUseFontMemory = m_mapFontFileMemory.find( iter->second );
	if( mitUseFontMemory == m_mapFontFileMemory.end() )
	{
		return s_vecDummy;
	}
    return  mitUseFontMemory->second;
}


#endif  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY

#endif //USE_FREE_TYPE
