#pragma once

#define UNISCRIBE_DLLNAME L"\\usp10.dll"

#define GETPROCADDRESS( Module, APIName, Temp ) \
	Temp = GetProcAddress( Module, #APIName ); \
	if( Temp ) \
	*(FARPROC*)&_##APIName = Temp

#define PLACEHOLDERPROC( APIName ) \
	_##APIName = Dummy_##APIName

#define IMM32_DLLNAME L"\\imm32.dll"
#define VER_DLLNAME L"\\version.dll"

class CKTDGUIUniBuffer
{
	public:
		CKTDGUIUniBuffer( int nInitialSize = 1 );
		~CKTDGUIUniBuffer(void);

		static	void	Initialize();
		static	void	Uninitialize();

				int		GetBufferSize() { return m_nBufferSize; }
				bool	SetBufferSize( int nSize );
				int		GetTextSize()  { return lstrlenW( m_pwszBuffer ); }
		const	WCHAR*	GetBuffer() { return m_pwszBuffer; }

		const	WCHAR& operator[]( int n ) const { return m_pwszBuffer[n]; }
				WCHAR& operator[]( int n );

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
				CKTDGFontManager::CUKFont* GetFontNode() { return m_pFontNode; }
				void SetFontNode( CKTDGFontManager::CUKFont* pFontNode ) { m_pFontNode = pFontNode; }
#else
				CKTDGFontManager::CKTDGFont* GetFontNode() { return m_pFontNode; }
				void SetFontNode( CKTDGFontManager::CKTDGFont* pFontNode ) { m_pFontNode = pFontNode; }
#endif
				void		Clear();

				bool		InsertChar( int nIndex, WCHAR wChar ); // Inserts the char at specified index. If nIndex == -1, insert to the end.
				bool		RemoveChar( int nIndex );  // Removes the char at specified index. If nIndex == -1, remove the last char.
				bool		InsertString( int nIndex, const WCHAR *pStr, int nCount = -1 );  // Inserts the first nCount characters of the string pStr at specified index.  If nCount == -1, the entire string is inserted. If nIndex == -1, insert to the end.
				bool		SetText( LPCWSTR wszText );

				// Uniscribe                
                HRESULT     CPtoY( int nCP, BOOL bTrail, int *pY );
				HRESULT		CPtoX( int nCP, BOOL bTrail, int *pX );
                HRESULT		XtoCP( int nX, int *pCP, int *pnTrail );
                HRESULT     XYtoCP( int nX, int nY, int *pCP, int *pnTrail );
				void		GetPriorItemPos( int nCP, int *pPrior );
				void		GetNextItemPos( int nCP, int *pPrior );                
                int         GetLineWidth(int nCP);
                int         GetCaretLineMove( int nCP, int stepLine );
                int         GetLineCP(int nLine);
                int         GetLineCount(int nCP);

				// Hide Char
		int		m_nBufferSize;			// Size of the buffer allocated, in characters
		int		m_nLengthLimit;			// 글자 제한	
		bool	m_bOneCharMode;			// 한문자 모드
				void		SetHideChar( bool bUseHideChar, WCHAR cHideChar );
				void		SetLengthLimit( int val ) { m_nLengthLimit = val; }
				void		SetOneCharMode( bool val ) { m_bOneCharMode = val; }
				void		SetOnlyNumMode( bool val ) { m_bOnlyNumMode = val; }
				void		SetWidthLimit( int val ) { m_WidthLimit = val; }
                void		SetLineLimit( int val ) { m_LineLimit = val; }
				void		SetByteLimit( int val ) { m_bByteLimit = true, m_ByteLimit = val; }
#ifdef FIX_IME_EDITBOX_UNLIMITED_CHARS
				int			GetByteLimit() { return m_bByteLimit ? m_ByteLimit : m_nLengthLimit; }
#endif //FIX_IME_EDITBOX_UNLIMITED_CHARS
				void		SetOnlyNumAndEnglishMode( bool val, bool bCheckEnglishBigAndSmall, bool bBigEnglish ) 
				{ 
					m_bOnlyNumAndEnglishMode = val; 
					m_bCheckEnglishBigAndSmall = bCheckEnglishBigAndSmall;
					m_bBigEnglish = bBigEnglish; 
				}

				void		SetAutoChangeBigEnglish( bool val ) { m_bAutoChangeBigEnglish = val; }

	private:
#ifndef USE_FREE_TYPE
		HRESULT	Analyse();      // Uniscribe -- Analyse() analyses the string in the buffer
#endif //USE_FREE_TYPE
		bool isEnglish( WCHAR cWChar );

		WCHAR*	m_pwszBuffer;			// Buffer to hold text
		WCHAR*	m_pwszBufferHC;			// Buffer to hold text
		bool	m_bOnlyNumMode;			// 숫자 입력 모드
		int		m_WidthLimit;
        int     m_LineLimit;
		bool	m_bOnlyNumAndEnglishMode;
		bool	m_bCheckEnglishBigAndSmall;
		bool	m_bBigEnglish;	//숫자와 영어 모드에서 영어가 대문자로 할것인지?
		bool	m_bByteLimit;
		int		m_ByteLimit;
		bool	m_bAutoChangeBigEnglish;
		
		// Hide Char
		WCHAR	m_cHideChar;
		bool	m_bUseHideChar;

		// Uniscribe-specific
		WCHAR					Defence1[100];
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		CKTDGFontManager::CUKFont*	    m_pFontNode;        // Font node for the font that this buffer uses
#else
		CKTDGFontManager::CKTDGFont*	m_pFontNode;        // Font node for the font that this buffer uses
#endif

		bool							m_bAnalyseRequired;         // True if the string has changed since last analysis.
		SCRIPT_STRING_ANALYSIS			m_Analysis;					// Analysis for the current string

		WCHAR					Defence2[100];

	private:
		// Empty implementation of the Uniscribe API
		static HRESULT WINAPI Dummy_ScriptApplyDigitSubstitution( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* ) { return E_NOTIMPL; }
		static HRESULT WINAPI Dummy_ScriptStringAnalyse( HDC, const void *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* ) { return E_NOTIMPL; }
		static HRESULT WINAPI Dummy_ScriptStringCPtoX( SCRIPT_STRING_ANALYSIS, int, BOOL, int* ) { return E_NOTIMPL; }
		static HRESULT WINAPI Dummy_ScriptStringXtoCP( SCRIPT_STRING_ANALYSIS, int, int*, int* ) { return E_NOTIMPL; }
		static HRESULT WINAPI Dummy_ScriptStringFree( SCRIPT_STRING_ANALYSIS* ) { return E_NOTIMPL; }
		static const SCRIPT_LOGATTR* WINAPI Dummy_ScriptString_pLogAttr( SCRIPT_STRING_ANALYSIS ) { return NULL; }
		static const int* WINAPI Dummy_ScriptString_pcOutChars( SCRIPT_STRING_ANALYSIS ) { return NULL; }

		// Function pointers
		static HRESULT (WINAPI *_ScriptApplyDigitSubstitution)( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* );
		static HRESULT (WINAPI *_ScriptStringAnalyse)( HDC, const void *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* );
		static HRESULT (WINAPI *_ScriptStringCPtoX)( SCRIPT_STRING_ANALYSIS, int, BOOL, int* );
		static HRESULT (WINAPI *_ScriptStringXtoCP)( SCRIPT_STRING_ANALYSIS, int, int*, int* );
		static HRESULT (WINAPI *_ScriptStringFree)( SCRIPT_STRING_ANALYSIS* );
		static const SCRIPT_LOGATTR* (WINAPI *_ScriptString_pLogAttr)( SCRIPT_STRING_ANALYSIS );
		static const int* (WINAPI *_ScriptString_pcOutChars)( SCRIPT_STRING_ANALYSIS );

		static HINSTANCE s_hDll;  // Uniscribe DLL handle
};
