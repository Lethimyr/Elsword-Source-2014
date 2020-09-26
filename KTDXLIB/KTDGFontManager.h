#pragma once

class CKTDGFontManager
{
	public:
		enum FONT_STYLE
		{
			FS_NONE = 0,
			FS_SHADOW,
			FS_SHELL,
		};

		struct FontArticle
		{
			int			ID;
			wstring		strMsg;
			D3DXCOLOR	color;
			D3DXCOLOR	outLineColor;
			bool		bInstance;
			DWORD		sortFlag;
			FONT_STYLE	fontStyle;

			D3DXVECTOR3	pos;
			RECT		rect;

			bool		bSpread;
			int			iSpreadCount;
			float		fSpreadTimeGap;
			float		fNowSpreadTime;			

			FontArticle()
                : ID( 0 )
                , strMsg()
                , color( 0, 0, 0, 0 )
                , outLineColor( 0, 0, 0, 0 )
                , bInstance( false )
                , sortFlag( 0 )
                , fontStyle( FS_NONE )
                , pos(0,0,0)
                , bSpread( false )
                , iSpreadCount( 0 )
                , fSpreadTimeGap( 0.f )
                , fNowSpreadTime( 0.f )
			{
                rect.left = rect.top = rect.right = rect.bottom = 0;
			}
		};

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		class CKTDGFont
		{
			public:
				CKTDGFont( LPDIRECT3DDEVICE9 pDevice, const WCHAR* pFontName = L"굴림체", int Height = 15, int Width = 0, int Weight = FW_NORMAL );
				~CKTDGFont(void);

				HRESULT			ResetFont( int Height = -1, int Width = -1, int Weight = -1, WCHAR* pFontName = NULL );

				// 2D
				int				Push2DText( const WCHAR* strMsg, int xPos, int yPos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_LEFT, FONT_STYLE fontStyle = FS_NONE, bool bInstance = false, bool bSpread = false, float fSpreadTimeGap = 0.f );
				void			Draw2DText( const WCHAR* strMsg, int xPos, int yPos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_LEFT, FONT_STYLE fontStyle = FS_NONE, int nCount = -1 );
				FontArticle*	Get2DArticle( const int ID );
				bool			Delete2DFont( const int ID );
				void			Clear2DFont();

				// 3D
				int				Push3DText( WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_LEFT, FONT_STYLE fontStyle = FS_NONE, bool bInstance = true );
				void			Draw3DText( WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_LEFT, FONT_STYLE fontStyle = FS_NONE );
				FontArticle*	Get3DArticle( const int ID );
				bool			Delete3DFont( const int ID );
				void			Clear3DFont();
				
				// 3D => 2D
				int				PushProjectsText( const WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_CENTER, FONT_STYLE fontStyle = FS_NONE, bool bInstance = true );
				void			DrawProjectsText( const WCHAR* strMsg, D3DXVECTOR3 pos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_CENTER, FONT_STYLE fontStyle = FS_NONE );
				FontArticle*	GetProjectsArticle( const int ID );
				bool			DeleteProjectsFont( const int ID );
				void			ClearProjectsFont();

				// Text
				void			DrawTextFont( const WCHAR* strMsg, RECT* pos, int nCount, D3DXCOLOR color, bool bOutline, D3DXCOLOR outlineColor, DWORD sortFlag = (DT_LEFT | DT_VCENTER) );
				void			CalcTextRect( const WCHAR* strMsg, RECT* pos, int nCount, DWORD format = (DT_VCENTER | DT_CALCRECT) );

				void			Flush();

				void			OnFrameMove( float fTime, float fElapsedTime );
				void			OnFrameRender();
				HRESULT			OnResetDevice();
				HRESULT			OnLostDevice();

				ID3DXFont*		GetD3DFont()	{ return m_pFont; }
				ID3DXSprite*	GetD3DSprite()	{ return m_pTextSprite; }
				int				GetHeight()		{ return m_Height; }

				const WCHAR*	GetFontName() { return m_FontName.c_str(); }
				int				GetWeight() { return m_Weight; }

				HDC				GetDC() { return m_pFont->GetDC(); }

			private:
				void			DrawFont( const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR color, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount = -1 );
				void			DrawFontStyle( const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount = -1 );

				LPDIRECT3DDEVICE9	m_pd3dDevice;
				ID3DXFont*			m_pFont;
				ID3DXSprite*		m_pTextSprite;
				CKTDGMatrix*		m_pMatrix;
				int					m_TextID;

				int					m_Height;
				int					m_Width;
				int					m_Weight;
				wstring				m_FontName;

				vector<FontArticle*>	m_vecFont2D;
				vector<FontArticle*>	m_vecFont3D;
				vector<FontArticle*>	m_vecFontProjects;

				D3DXMATRIX			m_matProjection, m_matView, m_matIdentityWorld;
		};
#endif

		class CUKFont
		{
			public:

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				CUKFont(const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1, bool bNoRes = false );
//#else
//				CUKFont(const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1 );
//#endif
				virtual ~CUKFont(void);

				void OnResetDevice();
				void OnLostDevice();

				bool Load();
				void UnLoad(bool bComplete = true);

				static std::string MakeFontID( std::string strFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL );

				int GetFontSize()	{ return m_pUKFont->GetFontSize(); }
				int GetWidth( WCHAR wLetter ) const;
				int GetHeight( WCHAR wLetter ) const;
				int GetWidth( const WCHAR* wszText ) const;
				int GetHeight( const WCHAR* wszText ) const;
				// oasis907 : 김상윤 [2010.11.17] // 라인 break 걸린 텍스트 높이 오차 수정할 수 있게
				int GetPaddedHeight( const WCHAR* wszText, int iAddHeight = 1) const;

				int	GetHeight()
				{
					return m_pUKFont->GetHeight();
				}

				int GetPureWidth( const WCHAR* wszText ) const 
				{
					return m_pUKFont->GetPureWidth( wszText );
				}

				/// draw text.
				/// @param  iLeft: left-position of first char of text string(screen coordinate)
				/// @param  iTop: top-position of first char of text string
				/// @param  pszText: [in] output text
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				void OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff,
					CKTDGFontManager::FONT_STYLE fontStyle = FS_NONE, D3DCOLOR colorOutLine = 0x00000000, 
					RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float fScaleX = 1.f, float fScaleY = 1.f );
//#else
//				void OutTextXY( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff,
//								CKTDGFontManager::FONT_STYLE fontStyle = FS_NONE, D3DCOLOR colorOutLine = 0x00000000, 
//								RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP );
//#endif
				void OutProjectionText( const D3DXVECTOR3& pos, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, 
									CKTDGFontManager::FONT_STYLE fontStyle = FS_NONE, D3DCOLOR colorOutLine = 0x00000000, 
									RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP );

				/// draw multi-lined text.
				/// @param  iLeft: left-position of first char of text string(screen coordinate)
				/// @param  iTop: top-position of first char of text string
				/// @param  pszText: [in] output text
				/// @param  fLineSpace: line space ratio
				///         - ex) fLineSpace == 2.f means double space
				/// @note   we use '\n' and "\\n" as line breaking special characters
				///         so "Hello\nWorld" and "Hello\\nWorld" will be rendered same format.
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
				void OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, CKTDGFontManager::FONT_STYLE fontStyle = FS_NONE, D3DCOLOR colorOutLine = 0x00000000, float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, int nCount = -1, float  fScaleX = 1.f, float fScaleY = 1.f );
//#else
//				void OutTextMultiline( const int& iLeft, const int& iTop, const WCHAR* wszText, D3DCOLOR color = 0xffffffff, 
//									CKTDGFontManager::FONT_STYLE fontStyle = FS_NONE, D3DCOLOR colorOutLine = 0x00000000,
//									float fLineSpace = 1.0f, RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP);
//#endif

#ifdef  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT

#ifndef USE_FREE_TYPE
				HDC GetDC()
				{
					return m_pUKFont->GetFontDC();
				}

				CKTDGDeviceFont* GetKTDGDeviceFont()
				{
					return  m_pUKFont;
				}
#endif //USE_FREE_TYPE

				void			Flush();
				void			OnFrameMove( float fElapsedTime );
				void			OnFrameRender();

				int				Push2DText( const WCHAR* strMsg, int xPos, int yPos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_LEFT, FONT_STYLE fontStyle = FS_NONE, bool bInstance = false, bool bSpread = false, float fSpreadTimeGap = 0.f );
				void			Draw2DText( const WCHAR* strMsg, int xPos, int yPos, D3DXCOLOR color, D3DXCOLOR outlineColor, DWORD sortFlag = DT_LEFT, FONT_STYLE fontStyle = FS_NONE, int nCount = -1 );
				FontArticle*	Get2DArticle( const int ID );
				bool			Delete2DFont( const int ID );
				void			Clear2DFont();

				// Text
				void			DrawTextFont( const WCHAR* strMsg, RECT* pos, int nCount, D3DXCOLOR color, bool bOutline, D3DXCOLOR outlineColor, DWORD sortFlag = (DT_LEFT | DT_VCENTER) );
				void			CalcTextRect( const WCHAR* strMsg, RECT* pos, int nCount, DWORD format = (DT_VCENTER | DT_CALCRECT) );

				void			DrawFont( const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR color, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount = -1 );
				void			DrawFontStyle( const WCHAR* strMsg, RECT rt, DWORD sortFlag, D3DXCOLOR outlineColor, FONT_STYLE fontStyle, int nCount = -1 );
#endif
			
			private:
#ifdef USE_FREE_TYPE
				CKTDGFreeTypeFont*		m_pUKFont;
#else //USE_FREE_TYPE
				CKTDGDeviceFont*		m_pUKFont;
#endif //USE_FREE_TYPE

				D3DXMATRIX m_matIdentity;
				D3DXMATRIX m_matProj;
				D3DXMATRIX m_matView;

#ifdef  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
				int					m_TextID;
				vector<FontArticle*>	m_vecFont2D;
#endif
		};
#ifdef USE_FREE_TYPE
		FT_Library	GetFreeTypeLibrary() const { return m_FreeTypeLibrary; }

#ifdef  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		const std::vector<BYTE>& GetFontMemory( string strFontName ) const;
#else   X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER GetFontMemory( string strFontName )
		{
			UseFontMemoryMap::iterator mitUseFontMemory = m_mapUseFontMemory.find( strFontName );
			if( mitUseFontMemory == m_mapUseFontMemory.end() )
			{
				KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER retInfo;
				return retInfo;
			}

			return mitUseFontMemory->second;
		}
#endif  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
		void InstallFont( string strFontName, string strFontPath );
#endif //USE_FREE_TYPE

	public:
		CKTDGFontManager( LPDIRECT3DDEVICE9 pd3dDevice );
		~CKTDGFontManager(void);
		
		void OnFrameMove( float fTime, float fElapsedTime );
		void OnFrameRender();

		HRESULT	OnResetDevice();
		HRESULT	OnLostDevice();

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		void        ResetDCFontUsed();
		bool        CheckDCFontUsed();
#else
		CKTDGFont*	CreateNewFont( const WCHAR* pFontName = L"굴림체", int Height = 15, int Width = 0, int Weight = FW_NORMAL );
#endif

		void		CreateNewUIFont_LUA( int fontID, const char* pFontName = "굴림체", int Height = 15, int Weight = FW_NORMAL );
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		bool		DestroyFont( CKTDGFont* pFont );
#endif

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		CUKFont*	CreateNewUKFont( const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1, bool bNoRes = false );
//#else
//		CUKFont*	CreateNewUKFont( const WCHAR* wstrFontName, int iFontSize, int iOutLineSize, bool bRHW, int fontWeight = FW_NORMAL, int enlargeNum = 1 );
//#endif

//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
		void		CreateNewUIUKFont_LUA( int fontID, const char* pFontName, int Height, int Weight, int enlargeNum, int iOutlineSize = 1, bool bNoRes = false );
//#else
//		void		CreateNewUIUKFont_LUA( int fontID, const char* pFontName, int Height, int Weight, int enlargeNum );
//#endif
		bool		DestroyUKFont( CUKFont* pFont );

#ifdef  KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		void        MapUIFontToUKFont( int fontID, int ukFontID );
#endif

#ifdef DLL_BUILD
		bool			InstallFont( const char* pValueName, const char* pFontFileName );
		vector<wstring>				m_UseFontNameList;
#endif DLL_BUILD

	private:
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
        vector<CKTDGFont*>	m_FontList;
#endif
		vector<CUKFont*>	m_UKFontList;


#ifdef USE_FREE_TYPE
#ifdef  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
        typedef std::map< std::string, std::string >       FontNameFileMap;
        typedef std::map< std::string, std::vector<BYTE> > FontFileMemoryMap;
        FontNameFileMap     m_mapFontNameFile;
        FontFileMemoryMap   m_mapFontFileMemory;
#else   X2OPTIMIZE_FREE_TYPE_FONT_MEMORY
        typedef std::map< std::string, KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER >  UseFontMemoryMap;
		UseFontMemoryMap    m_mapUseFontMemory;
#endif  X2OPTIMIZE_FREE_TYPE_FONT_MEMORY

		FT_Library			m_FreeTypeLibrary;
#endif //USE_FREE_TYPE
};
