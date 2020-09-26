#include "StdAfx.h"
#include ".\ktdguiimeeditbox.h"

// IME constants
#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor )      ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )

#define IMEID_CHT_VER42 ( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )   // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43 ( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )   // New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44 ( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )   // New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50 ( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )   // New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51 ( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )   // New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52 ( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )   // New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60 ( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )   // New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41 ( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )   // MSPY1.5  // SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42 ( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )   // MSPY2    // Win2k/WinME
#define IMEID_CHS_VER53 ( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )   // MSPY3    // WinXP

//09. 05. 13 김정협 Reading출력 범위 조정 
#define MAX_READING_COUNT 5

// Function pointers
INPUTCONTEXT* (WINAPI * CKTDGUIIMEEditBox::_ImmLockIMC)( HIMC ) = CKTDGUIIMEEditBox::Dummy_ImmLockIMC;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmUnlockIMC)( HIMC ) = CKTDGUIIMEEditBox::Dummy_ImmUnlockIMC;
LPVOID (WINAPI * CKTDGUIIMEEditBox::_ImmLockIMCC)( HIMCC ) = CKTDGUIIMEEditBox::Dummy_ImmLockIMCC;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmUnlockIMCC)( HIMCC ) = CKTDGUIIMEEditBox::Dummy_ImmUnlockIMCC;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmDisableTextFrameService)( DWORD ) = CKTDGUIIMEEditBox::Dummy_ImmDisableTextFrameService;
LONG (WINAPI * CKTDGUIIMEEditBox::_ImmGetCompositionStringW)( HIMC, DWORD, LPVOID, DWORD ) = CKTDGUIIMEEditBox::Dummy_ImmGetCompositionStringW;
DWORD (WINAPI * CKTDGUIIMEEditBox::_ImmGetCandidateListW)( HIMC, DWORD, LPCANDIDATELIST, DWORD ) = CKTDGUIIMEEditBox::Dummy_ImmGetCandidateListW;
HIMC (WINAPI * CKTDGUIIMEEditBox::_ImmGetContext)( HWND ) = CKTDGUIIMEEditBox::Dummy_ImmGetContext;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmReleaseContext)( HWND, HIMC ) = CKTDGUIIMEEditBox::Dummy_ImmReleaseContext;
HIMC (WINAPI * CKTDGUIIMEEditBox::_ImmAssociateContext)( HWND, HIMC ) = CKTDGUIIMEEditBox::Dummy_ImmAssociateContext;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmGetOpenStatus)( HIMC ) = CKTDGUIIMEEditBox::Dummy_ImmGetOpenStatus;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmSetOpenStatus)( HIMC, BOOL ) = CKTDGUIIMEEditBox::Dummy_ImmSetOpenStatus;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmGetConversionStatus)( HIMC, LPDWORD, LPDWORD ) = CKTDGUIIMEEditBox::Dummy_ImmGetConversionStatus;
HWND (WINAPI * CKTDGUIIMEEditBox::_ImmGetDefaultIMEWnd)( HWND ) = CKTDGUIIMEEditBox::Dummy_ImmGetDefaultIMEWnd;
UINT (WINAPI * CKTDGUIIMEEditBox::_ImmGetIMEFileNameA)( HKL, LPSTR, UINT ) = CKTDGUIIMEEditBox::Dummy_ImmGetIMEFileNameA;
UINT (WINAPI * CKTDGUIIMEEditBox::_ImmGetVirtualKey)( HWND ) = CKTDGUIIMEEditBox::Dummy_ImmGetVirtualKey;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmNotifyIME)( HIMC, DWORD, DWORD, DWORD ) = CKTDGUIIMEEditBox::Dummy_ImmNotifyIME;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmSetConversionStatus)( HIMC, DWORD, DWORD ) = CKTDGUIIMEEditBox::Dummy_ImmSetConversionStatus;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmSimulateHotKey)( HWND, DWORD ) = CKTDGUIIMEEditBox::Dummy_ImmSimulateHotKey;
BOOL (WINAPI * CKTDGUIIMEEditBox::_ImmIsIME)( HKL ) = CKTDGUIIMEEditBox::Dummy_ImmIsIME;
UINT (WINAPI * CKTDGUIIMEEditBox::_GetReadingString)( HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT ) = CKTDGUIIMEEditBox::Dummy_GetReadingString; // Traditional Chinese IME
BOOL (WINAPI * CKTDGUIIMEEditBox::_ShowReadingWindow)( HIMC, BOOL ) = CKTDGUIIMEEditBox::Dummy_ShowReadingWindow; // Traditional Chinese IME
BOOL (APIENTRY * CKTDGUIIMEEditBox::_VerQueryValueA)( const LPVOID, LPSTR, LPVOID *, PUINT ) = CKTDGUIIMEEditBox::Dummy_VerQueryValueA;
BOOL (APIENTRY * CKTDGUIIMEEditBox::_GetFileVersionInfoA)( LPSTR, DWORD, DWORD, LPVOID )= CKTDGUIIMEEditBox::Dummy_GetFileVersionInfoA;
DWORD (APIENTRY * CKTDGUIIMEEditBox::_GetFileVersionInfoSizeA)( LPSTR, LPDWORD ) = CKTDGUIIMEEditBox::Dummy_GetFileVersionInfoSizeA;

HINSTANCE	CKTDGUIIMEEditBox::s_hDllImm32;				// IMM32 DLL handle
HINSTANCE	CKTDGUIIMEEditBox::s_hDllVer;				// Version DLL handle
HKL			CKTDGUIIMEEditBox::s_hklCurrent;			// Current keyboard layout of the process
bool		CKTDGUIIMEEditBox::s_bVerticalCand;			// Indicates that the candidates are listed vertically
bool		CKTDGUIIMEEditBox::s_bHideCaret;			// If true, we don't render the caret.
WCHAR		CKTDGUIIMEEditBox::s_aszIndicator[5][3] =	// String to draw to indicate current input locale
{
	L"En",
		L"\x7B80",
		L"\x7E41",
		L"\xAC00",
		L"\x3042",
};
LPWSTR    CKTDGUIIMEEditBox::s_wszCurrIndicator = CKTDGUIIMEEditBox::s_aszIndicator[0];  // Points to an indicator string that corresponds to current input locale
bool      CKTDGUIIMEEditBox::s_bInsertOnType;     // Insert the character as soon as a key is pressed (Korean behavior)
HINSTANCE CKTDGUIIMEEditBox::s_hDllIme;           // Instance handle of the current IME module
HIMC      CKTDGUIIMEEditBox::s_hImcDef;           // Default input context
CKTDGUIIMEEditBox::IMESTATE  CKTDGUIIMEEditBox::s_ImeState = IMEUI_STATE_OFF;
bool      CKTDGUIIMEEditBox::s_bEnableImeSystem;  // Whether the IME system is active
POINT     CKTDGUIIMEEditBox::s_ptCompString;      // Composition string position. Updated every frame.
int       CKTDGUIIMEEditBox::s_nCompCaret;
int       CKTDGUIIMEEditBox::s_nFirstTargetConv;  // Index of the first target converted char in comp string.  If none, -1.
CKTDGUIUniBuffer CKTDGUIIMEEditBox::s_CompString = CKTDGUIUniBuffer( 0 );
BYTE      CKTDGUIIMEEditBox::s_abCompStringAttr[MAX_COMPSTRING_SIZE];
DWORD     CKTDGUIIMEEditBox::s_adwCompStringClause[MAX_COMPSTRING_SIZE];
WCHAR     CKTDGUIIMEEditBox::s_wszReadingString[32];
CKTDGUIIMEEditBox::CCandList CKTDGUIIMEEditBox::s_CandList;       // Data relevant to the candidate list
bool      CKTDGUIIMEEditBox::s_bShowReadingWindow; // Indicates whether reading window is visible
bool      CKTDGUIIMEEditBox::s_bHorizontalReading; // Indicates whether the reading window is vertical or horizontal
bool      CKTDGUIIMEEditBox::s_bChineseIME;
bool      CKTDGUIIMEEditBox::s_bCreateDeviceCheck = false;
CGrowableArray< CKTDGUIIMEEditBox::CInputLocale > CKTDGUIIMEEditBox::s_Locale; // Array of loaded keyboard layout on system
#if defined(DEBUG) || defined(_DEBUG)
bool      CKTDGUIIMEEditBox::m_bIMEStaticMsgProcCalled = false;
#endif


//--------------------------------------------------------------------------------------
CKTDGUIIMEEditBox::CKTDGUIIMEEditBox()
: m_pFont( NULL )
, m_rcText( RECT() )
, m_rcIndicator( RECT() )
#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
, m_bIsForceSendCustomMsgChange( false )
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION
{
	m_bBGCheck		= false;

	m_CustomMsgEditBoxChange	= -1;
	m_CustomMsgEditBoxEnter		= -1;
	m_CustomMsgEditBoxFocusOut	= -1;
#ifdef NUMBER_TO_LANGUAGE
	m_CustomMsgEditBoxFocusIn	= -1;
#endif NUMBER_TO_LANGUAGE

	m_pNormalTex	= NULL;
	m_pDisableTex	= NULL;

	m_pNormalPoint	= NULL;
	m_pDisablePoint	= NULL;
	m_pTextPoint	= NULL;

	CKTDGUIIMEEditBox::Initialize();			// ensure static vars are properly init'ed first
	_ImmDisableTextFrameService( (DWORD)-1 );	// Disable TSF for the current process

	CKTDGUIIMEEditBox::StaticOnCreateDevice();

	m_ControlType	= CKTDGUIControl::UCT_IME_EDITBOX;

	m_bCaretOn		= true;
	m_dfBlink		= GetCaretBlinkTime() * 0.0005f;
	m_dfLastBlink	= DXUTGetGlobalTimer()->GetAbsoluteTime();
	s_bHideCaret	= false;
	m_nFirstVisible	= 0;
	m_nCaret		= m_nSelStart = 0;
	m_bInsertMode	= true;

	m_bMouseDrag	= false;

	s_bEnableImeSystem		= true;
	m_nIndicatorWidth		= 0;
	m_DefTextAlign			= -1;
	m_ptTextAlignOffset.x	= 0;
	m_ptTextAlignOffset.y	= 0;

	m_TextColor				= D3DCOLOR_ARGB( 255,   0,   0,   0 );
	m_SelTextColor			= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_SelBkColor			= D3DCOLOR_ARGB( 255,  40,  50,  92 );
	m_CaretColor			= D3DCOLOR_ARGB( 255,   0,   0,   0 );
	m_ReadingColor			= D3DCOLOR_ARGB( 188, 255, 255, 255 );
	m_ReadingWinColor		= D3DCOLOR_ARGB( 128, 0, 0, 0 );
	m_ReadingSelColor		= D3DCOLOR_ARGB( 255, 255, 0, 0 );
	m_ReadingSelBkColor		= D3DCOLOR_ARGB( 128, 80, 80, 80 );
	m_CandidateColor		= D3DCOLOR_ARGB( 255, 200, 200, 200 );
	m_CandidateWinColor		= D3DCOLOR_ARGB( 128, 0, 0, 0 );
	m_CandidateSelColor		= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_CandidateSelBkColor	= D3DCOLOR_ARGB( 128, 158, 158, 158 );
	m_CompColor				= D3DCOLOR_ARGB( 255, 200, 200, 255 );
	m_CompWinColor			= D3DCOLOR_ARGB( 198, 0, 0, 0 );
	m_CompCaretColor		= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_CompTargetColor		= D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_CompTargetBkColor		= D3DCOLOR_ARGB( 255, 150, 150, 150 );
	m_CompTargetNonColor	= D3DCOLOR_ARGB( 255, 255, 255, 0 );
	m_CompTargetNonBkColor	= D3DCOLOR_ARGB( 255, 150, 150, 150 );

	m_NowFontColor			= 0xff000000;
	m_OutlineColor			= 0xffffffff;
	m_bOutlineCheck			= false;

	m_pSndKeyDown	= NULL;

	m_GroupID		= -1;
	m_GroupSequence	= -1;


	//{{ robobeg : 2008-10-28
	//KLuaManager kLuaManager;
	KLuaManager kLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	//}} robobeg : 2008-10-28

	if(  g_pKTDXApp->LoadAndDoMemory( &kLuaManager, L"UI_Control_Sound.lua" ) == false )
	{
		return;
	}

	wstring keyDownSndFileName;

	LUA_GET_VALUE( kLuaManager, "EditBox_KeyDown", keyDownSndFileName, L"" );
	m_pSndKeyDown = g_pKTDXApp->GetDeviceManager()->OpenSound( keyDownSndFileName );

	m_bEnablePaste = true;

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	ClearText();
#endif

#ifdef FIX_IME_EDITBOX_LOSE_FIRST_CHAR
	m_rcText.left = 0;
	m_rcText.top = 0;
	m_rcText.right = 0;
	m_rcText.bottom = 0;
#endif FIX_IME_EDITBOX_LOSE_FIRST_CHAR

#ifdef DLL_BUILD
	m_pCheckedEdgeTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"UIEdge.tga" );
	m_bEditEdge = false;
	m_bUpdate = false;		// GUITool에서 실행될때는 모든 메세지를 차단.
#endif
}


//--------------------------------------------------------------------------------------
CKTDGUIIMEEditBox::~CKTDGUIIMEEditBox()
{
	SAFE_DELETE( m_pNormalTex );
	SAFE_DELETE( m_pDisableTex );

	SAFE_DELETE( m_pNormalPoint );
	SAFE_DELETE( m_pDisablePoint );
	SAFE_DELETE( m_pTextPoint );

	SAFE_CLOSE( m_pSndKeyDown );

#ifdef DLL_BUILD
	SAFE_CLOSE( m_pCheckedEdgeTexture );
#endif
}

//--------------------------------------------------------------------------------------
// PlaceCaret: Set the caret to a character position, and adjust the scrolling if
//             necessary.
//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::PlaceCaret( int nCP )
{
	assert( nCP >= 0 && nCP <= m_Buffer.GetTextSize() );
	m_nCaret = nCP;

	// Obtain the X offset of the character.
	int nX1st, nX, nX2;
	nX1st = nX = nX2 = 0;
	m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nX1st );  // 1st visible char
	m_Buffer.CPtoX( nCP, FALSE, &nX );  // LEAD
	// If nCP is the NULL terminator, get the leading edge instead of trailing.
	if( nCP == m_Buffer.GetTextSize() )
		nX2 = nX;
	else
		m_Buffer.CPtoX( nCP, TRUE, &nX2 );  // TRAIL

	// If the left edge of the char is smaller than the left edge of the 1st visible char,
	// we need to scroll left until this char is visible.
	if( nX < nX1st )
	{
		// Simply make the first visible character the char at the new caret position.
		m_nFirstVisible = nCP;
	}
	else
	{
		// If the right of the character is bigger than the offset of the control's
		// right edge, we need to scroll right to this character.
		if( nX2 > nX1st + RtWidth( m_rcText ) )
		{
			// Compute the X of the new left-most pixel
			int nXNewLeft = nX2 - RtWidth( m_rcText );

			// Compute the char position of this character
			int nCPNew1st, nNewTrail;
			m_Buffer.XtoCP( nXNewLeft, &nCPNew1st, &nNewTrail );

			// If this coordinate is not on a character border,
			// start from the next character so that the caret
			// position does not fall outside the text rectangle.
			int nXNew1st;
			m_Buffer.CPtoX( nCPNew1st, FALSE, &nXNew1st );
			if( nXNew1st < nXNewLeft )
				++nCPNew1st;

			m_nFirstVisible = nCPNew1st;
		}
	}
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::ClearText()
{
	m_Buffer.Clear();
	m_nFirstVisible = 0;
	m_nCaret = 0;
	//	PlaceCaret( 0 );
	m_nSelStart = 0;
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::SetText( LPCWSTR wszText, bool bSelected )
{
	assert( wszText != NULL );

#ifdef FIX_IME_EDITBOX_UNLIMITED_CHARS
	if( wcslen( wszText ) > static_cast<UINT>( m_Buffer.GetByteLimit() ) )
	{
		std::wstring wstrLimitedText = wszText;
		wstrLimitedText = wstrLimitedText.substr( 0, m_Buffer.GetByteLimit() );
		m_Buffer.SetText( wstrLimitedText.c_str() );
	}
	else
#endif //FIX_IME_EDITBOX_UNLIMITED_CHARS
	m_Buffer.SetText( wszText );
	m_nFirstVisible = 0;
	// Move the caret to the end of the text
	PlaceCaret( m_Buffer.GetTextSize() );
	m_nSelStart = bSelected ? 0 : m_nCaret;
}

//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::SetText_LUA( const char* pcText )
{
	std::wstring strText;
	ConvertUtf8ToWCHAR( strText, pcText );
	SetText( strText.c_str() );
}

//--------------------------------------------------------------------------------------
HRESULT CKTDGUIIMEEditBox::GetTextCopy( LPWSTR strDest, UINT bufferCount )
{
	assert( strDest );

	StringCchCopy( strDest, bufferCount, m_Buffer.GetBuffer() );

	return S_OK;
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::DeleteSelectionText()
{
	int nFirst = __min( m_nCaret, m_nSelStart );
	int nLast = __max( m_nCaret, m_nSelStart );
	// Update caret and selection
	PlaceCaret( nFirst );
	m_nSelStart = m_nCaret;
	// Remove the characters
	for( int i = nFirst; i < nLast; ++i )
		m_Buffer.RemoveChar( nFirst );
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::SendKey( BYTE nVirtKey )
{
	keybd_event( nVirtKey, 0, 0,               0 );
	keybd_event( nVirtKey, 0, KEYEVENTF_KEYUP, 0 );
}


//--------------------------------------------------------------------------------------
// Called by CDXUTDialogResourceManager::OnCreateDevice.  This gives the class a
// chance to initialize its default input context associated with the app window.
HRESULT CKTDGUIIMEEditBox::StaticOnCreateDevice()
{
	if( s_bCreateDeviceCheck == false )
	{
		// Save the default input context
		s_hImcDef = _ImmGetContext( DXUTGetHWND() );
		_ImmReleaseContext( DXUTGetHWND(), s_hImcDef );

		s_bCreateDeviceCheck = true;
	}

	return S_OK;
}

void CKTDGUIIMEEditBox::CopyToClipboard()
{
	// Copy the selection text to the clipboard
	if( m_nCaret != m_nSelStart && OpenClipboard( NULL ) )
	{
		EmptyClipboard();

		HGLOBAL hBlock = GlobalAlloc( GMEM_MOVEABLE, sizeof(WCHAR) * ( m_Buffer.GetTextSize() + 1 ) );
		if( hBlock )
		{
			WCHAR *pwszText = (WCHAR*)GlobalLock( hBlock );
			if( pwszText )
			{
				int nFirst = __min( m_nCaret, m_nSelStart );
				int nLast = __max( m_nCaret, m_nSelStart );
				if( nLast - nFirst > 0 )
					CopyMemory( pwszText, m_Buffer.GetBuffer() + nFirst, (nLast - nFirst) * sizeof(WCHAR) );
				pwszText[nLast - nFirst] = L'\0';  // Terminate it
				GlobalUnlock( hBlock );
			}
			SetClipboardData( CF_UNICODETEXT, hBlock );
		}
		CloseClipboard();
		// We must not free the object until CloseClipboard is called.
		if( hBlock )
			GlobalFree( hBlock );
	}
}


void CKTDGUIIMEEditBox::PasteFromClipboard()
{
	DeleteSelectionText();

	if( m_bEnablePaste == false )
		return;

	if( OpenClipboard( NULL ) )
	{
		HANDLE handle = GetClipboardData( CF_UNICODETEXT );
		if( handle )
		{
			// Convert the ANSI string to Unicode, then
			// insert to our buffer.
			WCHAR *pwszText = (WCHAR*)GlobalLock( handle );
			if( pwszText )
			{
				// Copy all characters up to null.
				if( m_Buffer.InsertString( m_nCaret, pwszText ) )
					PlaceCaret( m_nCaret + lstrlenW( pwszText ) );
				m_nSelStart = m_nCaret;
				GlobalUnlock( handle );
			}
		}
		CloseClipboard();
	}
}

#define IN_FLOAT_CHARSET( c ) \
	( (c) == L'-' || (c) == L'.' || ( (c) >= L'0' && (c) <= L'9' ) )

void CKTDGUIIMEEditBox::ParseFloatArray( float *pNumbers, int nCount )
{
	int nWritten = 0;  // Number of floats written
	const WCHAR *pToken, *pEnd;
	WCHAR wszToken[60];

	pToken = m_Buffer.GetBuffer();
	while( nWritten < nCount && *pToken != L'\0' )
	{
		// Skip leading spaces
		while( *pToken == L' ' )
			++pToken;

		if( *pToken == L'\0' )
			break;

		// Locate the end of number
		pEnd = pToken;
		while( IN_FLOAT_CHARSET( *pEnd ) )
			++pEnd;

		// Copy the token to our buffer
		int nTokenLen = __min( sizeof(wszToken) / sizeof(wszToken[0]) - 1, int(pEnd - pToken) );
		StringCchCopy( wszToken, nTokenLen, pToken );
		*pNumbers = (float)wcstod( wszToken, NULL );
		++nWritten;
		++pNumbers;
		pToken = pEnd;
	}
}


void CKTDGUIIMEEditBox::SetTextFloatArray( const float *pNumbers, int nCount )
{
	WCHAR wszBuffer[512] = {0};
	WCHAR wszTmp[64];

	if( pNumbers == NULL )
		return;

	for( int i = 0; i < nCount; ++i )
	{
		StringCchPrintf( wszTmp, 64, L"%.4f ", pNumbers[i] );
		StringCchCat( wszBuffer, 512, wszTmp );
	}

	// Don't want the last space
	if( nCount > 0 && wcslen(wszBuffer) > 0 )
		wszBuffer[wcslen(wszBuffer)-1] = 0;

	SetText( wszBuffer );
}

void CKTDGUIIMEEditBox::SetMultiLineOption_LUA( int groupID, int groupSequence )
{
	m_GroupID = groupID;
	m_GroupSequence = groupSequence;
}

#ifdef DLL_BUILD	// GUITool
void CKTDGUIIMEEditBox::MoveControl( float fx, float fy )
{
	if( NULL != m_pTextPoint )
		return m_pTextPoint->Move(fx, fy);
}

D3DXVECTOR2 CKTDGUIIMEEditBox::GetPos()
{
	if( NULL != m_pTextPoint )
		return m_pTextPoint->leftTopPoint;

	return D3DXVECTOR2(0, 0);
}

vector<D3DXVECTOR2> CKTDGUIIMEEditBox::GetPosList()
{
	vector<D3DXVECTOR2> ret;	

	if( NULL != m_pTextPoint )
	{
		ret.push_back(m_pTextPoint->leftTopPoint);
		ret.push_back(m_pTextPoint->rightBottomPoint);
	}

	return ret;
}

void CKTDGUIIMEEditBox::SetEditGUI( bool bEdit )
{
	SetColor(D3DXCOLOR(0xffffffff));
	m_bEditEdge = bEdit;
}

bool CKTDGUIIMEEditBox::IsSelectByEditGui( POINT pt )
{
	return Pick2DRect( pt, m_NowPoint.leftTopPoint, m_NowPoint.rightTopPoint, m_NowPoint.leftBottomPoint ,m_NowPoint.rightBottomPoint );	
}

void CKTDGUIIMEEditBox::DrawEditEdge()
{
	if( false == m_bEditEdge )
		return;	

	if ( m_pCheckedEdgeTexture == NULL )
		return;	

	//const CKTDGUIControl::UIPointData & point = *m_pEditEdgePoint;
	D3DXCOLOR tempColor;

	int edgeWidth = 2;
	D3DXCOLOR edgeColor = D3DXCOLOR(0xffff0000);

	tempColor.a = edgeColor.a * m_pDialog->GetColor().a * m_Color.a;
	tempColor.r = edgeColor.r * m_pDialog->GetColor().r * m_Color.r;
	tempColor.g = edgeColor.g * m_pDialog->GetColor().g * m_Color.g;
	tempColor.b = edgeColor.b * m_pDialog->GetColor().b * m_Color.b;

	RECT edgeRect;
	edgeRect.left = (int)m_NowPoint.leftTopPoint.x;
	edgeRect.top = (int)m_NowPoint.leftTopPoint.y;
	edgeRect.right = (int)m_NowPoint.rightBottomPoint.x;
	edgeRect.bottom = (int)m_NowPoint.rightBottomPoint.y;

	int _width = (int)(edgeRect.right - edgeRect.left);
	int _height = (int)(edgeRect.bottom - edgeRect.top);

	//if ( m_bDrawEdgeOut == true )
	{
		// 좌 left/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth), 
			edgeWidth , 
			_height + edgeWidth, 
			tempColor );

		// 하left/bottom
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left - edgeWidth), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.bottom ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );

		// 우right/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.right ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top ), 
			edgeWidth, 
			_height + edgeWidth, 
			tempColor );

		// 상left/top
		m_pCheckedEdgeTexture->Draw( (int)(m_pDialog->GetPos().x + m_OffsetPos.x + edgeRect.left ), 
			(int)(m_pDialog->GetPos().y + m_OffsetPos.y + edgeRect.top - edgeWidth ), 
			_width + edgeWidth, 
			edgeWidth, 
			tempColor );
	}
}
#endif


//--------------------------------------------------------------------------------------
//  GetImeId( UINT uIndex )
//      returns 
//  returned value:
//  0: In the following cases
//      - Non Chinese IME input locale
//      - Older Chinese IME
//      - Other error cases
//
//  Othewise:
//      When uIndex is 0 (default)
//          bit 31-24:  Major version
//          bit 23-16:  Minor version
//          bit 15-0:   Language ID
//      When uIndex is 1
//          pVerFixedInfo->dwFileVersionLS
//
//  Use IMEID_VER and IMEID_LANG macro to extract version and language information.
//  

// We define the locale-invariant ID ourselves since it doesn't exist prior to WinXP
// For more information, see the CompareString() reference.
#define LCID_INVARIANT MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT)

DWORD CKTDGUIIMEEditBox::GetImeId( UINT uIndex )
{
	static HKL hklPrev = 0;
	static DWORD dwID[2] = { 0, 0 };  // Cache the result

	DWORD   dwVerSize;
	DWORD   dwVerHandle;
	LPVOID  lpVerBuffer;
	LPVOID  lpVerData;
	UINT    cbVerData;
	char    szTmp[1024];

	if( uIndex >= sizeof( dwID ) / sizeof( dwID[0] ) )
		return 0;

	if( hklPrev == s_hklCurrent )
		return dwID[uIndex];

	hklPrev = s_hklCurrent;  // Save for the next invocation

	// Check if we are using an older Chinese IME
	if( !( ( s_hklCurrent == _CHT_HKL ) || ( s_hklCurrent == _CHT_HKL2 ) || ( s_hklCurrent == _CHS_HKL ) ) )
	{
		dwID[0] = dwID[1] = 0;
		return dwID[uIndex];
	}

	// Obtain the IME file name
	if ( !_ImmGetIMEFileNameA( s_hklCurrent, szTmp, ( sizeof(szTmp) / sizeof(szTmp[0]) ) - 1 ) )
	{
		dwID[0] = dwID[1] = 0;
		return dwID[uIndex];
	}

	// Check for IME that doesn't implement reading string API
	if ( !_GetReadingString )
	{
		if( ( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME2, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHT_IMEFILENAME3, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME1, -1 ) != CSTR_EQUAL ) &&
			( CompareStringA( LCID_INVARIANT, NORM_IGNORECASE, szTmp, -1, CHS_IMEFILENAME2, -1 ) != CSTR_EQUAL ) )
		{
			dwID[0] = dwID[1] = 0;
			return dwID[uIndex];
		}
	}

	dwVerSize = _GetFileVersionInfoSizeA( szTmp, &dwVerHandle );
	if( dwVerSize )
	{
		lpVerBuffer = HeapAlloc( GetProcessHeap(), 0, dwVerSize );
		if( lpVerBuffer )
		{
			if( _GetFileVersionInfoA( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) )
			{
				if( _VerQueryValueA( lpVerBuffer, "\\", &lpVerData, &cbVerData ) )
				{
					DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
					dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
					if( _GetReadingString
						||
						( GetLanguage() == LANG_CHT &&
						( dwVer == MAKEIMEVERSION(4, 2) || 
						dwVer == MAKEIMEVERSION(4, 3) || 
						dwVer == MAKEIMEVERSION(4, 4) || 
						dwVer == MAKEIMEVERSION(5, 0) ||
						dwVer == MAKEIMEVERSION(5, 1) ||
						dwVer == MAKEIMEVERSION(5, 2) ||
						dwVer == MAKEIMEVERSION(6, 0) ) )
						||
						( GetLanguage() == LANG_CHS &&
						( dwVer == MAKEIMEVERSION(4, 1) ||
						dwVer == MAKEIMEVERSION(4, 2) ||
						dwVer == MAKEIMEVERSION(5, 3) ) )
						)
					{
						dwID[0] = dwVer | GetLanguage();
						dwID[1] = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
					}
				}
			}
			HeapFree( GetProcessHeap(), 0, lpVerBuffer );
		}
	}

	return dwID[uIndex];
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::CheckInputLocale()
{
	static HKL hklPrev = 0;
	s_hklCurrent = GetKeyboardLayout( 0 );
	if ( hklPrev == s_hklCurrent )
		return;

	hklPrev = s_hklCurrent;
	switch ( GetPrimaryLanguage() )
	{
		// Simplified Chinese
	case LANG_CHINESE:
		s_bVerticalCand = true;
		switch ( GetSubLanguage() )
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_CHS];
			s_bVerticalCand = GetImeId() == 0;
			break;
		case SUBLANG_CHINESE_TRADITIONAL:
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_CHT];
			break;
		default:    // unsupported sub-language
			s_wszCurrIndicator = s_aszIndicator[INDICATOR_NON_IME];
			break;
		}
		break;
		// Korean
	case LANG_KOREAN:
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_KOREAN];
		s_bVerticalCand = false;
		break;
		// Japanese
	case LANG_JAPANESE:
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_JAPANESE];
		s_bVerticalCand = true;
		break;
	default:
		// A non-IME language.  Obtain the language abbreviation
		// and store it for rendering the indicator later.
		s_wszCurrIndicator = s_aszIndicator[INDICATOR_NON_IME];
	}

	// If non-IME, use the language abbreviation.
	if( s_wszCurrIndicator == s_aszIndicator[INDICATOR_NON_IME] )
	{
		WCHAR wszLang[5];
		GetLocaleInfoW( MAKELCID( LOWORD( s_hklCurrent ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszLang, 5 );
		s_wszCurrIndicator[0] = wszLang[0];
		s_wszCurrIndicator[1] = towlower( wszLang[1] );
	}
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::CheckToggleState()
{
	CheckInputLocale();
	bool bIme = _ImmIsIME( s_hklCurrent ) != 0;
	s_bChineseIME = ( GetPrimaryLanguage() == LANG_CHINESE ) && bIme;

	HIMC hImc;
	if( NULL != ( hImc = _ImmGetContext( DXUTGetHWND() ) ) )
	{
		if( s_bChineseIME )
		{
			DWORD dwConvMode, dwSentMode;
			_ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
			s_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IMEUI_STATE_ON : IMEUI_STATE_ENGLISH;
		}
		else
		{
			s_ImeState = ( bIme && _ImmGetOpenStatus( hImc ) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
		}
		_ImmReleaseContext( DXUTGetHWND(), hImc );
	}
	else
		s_ImeState = IMEUI_STATE_OFF;
}


//--------------------------------------------------------------------------------------
// Enable/disable the entire IME system.  When disabled, the default IME handling
// kicks in.
void CKTDGUIIMEEditBox::EnableImeSystem( bool bEnable )
{
	s_bEnableImeSystem = bEnable;
}


//--------------------------------------------------------------------------------------
// Sets up IME-specific APIs for the IME edit controls.  This is called every time
// the input locale changes.
void CKTDGUIIMEEditBox::SetupImeApi()
{
	char szImeFile[MAX_PATH + 1];

	_GetReadingString = NULL;
	_ShowReadingWindow = NULL;
	if( _ImmGetIMEFileNameA( s_hklCurrent, szImeFile, sizeof(szImeFile)/sizeof(szImeFile[0]) - 1 ) == 0 )
		return;

	if( s_hDllIme ) FreeLibrary( s_hDllIme );
	s_hDllIme = LoadLibraryA( szImeFile );
	if ( !s_hDllIme )
		return;
	_GetReadingString = (UINT (WINAPI*)(HIMC, UINT, LPWSTR, PINT, BOOL*, PUINT))
		( GetProcAddress( s_hDllIme, "GetReadingString" ) );
	_ShowReadingWindow =(BOOL (WINAPI*)(HIMC, BOOL))
		( GetProcAddress( s_hDllIme, "ShowReadingWindow" ) );
}


//--------------------------------------------------------------------------------------
// Resets the composition string.
void CKTDGUIIMEEditBox::ResetCompositionString()
{
	s_nCompCaret = 0;
	s_CompString.SetText( L"" );
	ZeroMemory( s_abCompStringAttr, sizeof(s_abCompStringAttr) );
}


//--------------------------------------------------------------------------------------
// Truncate composition string by sending keystrokes to the window.
void CKTDGUIIMEEditBox::TruncateCompString( bool bUseBackSpace, int iNewStrLen )
{
	if( !s_bInsertOnType )
		return;

	int cc = (int) wcslen( s_CompString.GetBuffer() );
	assert( iNewStrLen == 0 || iNewStrLen >= cc );

	// Send right arrow keystrokes to move the caret
	//   to the end of the composition string.
	for (int i = 0; i < cc - s_nCompCaret; ++i )
		SendMessage( DXUTGetHWND(), WM_KEYDOWN, VK_RIGHT, 0 );
	SendMessage( DXUTGetHWND(), WM_KEYUP, VK_RIGHT, 0 );

	if( bUseBackSpace || m_bInsertMode )
		iNewStrLen = 0;

	// The caller sets bUseBackSpace to false if there's possibility of sending
	// new composition string to the app right after this function call.
	// 
	// If the app is in overwriting mode and new comp string is 
	// shorter than current one, delete previous comp string 
	// till it's same long as the new one. Then move caret to the beginning of comp string.
	// New comp string will overwrite old one.
	if( iNewStrLen < cc )
	{
		for( int i = 0; i < cc - iNewStrLen; ++i )
		{
			SendMessage( DXUTGetHWND(), WM_KEYDOWN, VK_BACK, 0 );  // Backspace character
			SendMessageW( DXUTGetHWND(), WM_CHAR, VK_BACK, 0 );
		}
		SendMessage( DXUTGetHWND(), WM_KEYUP, VK_BACK, 0 );
	}
	else
		iNewStrLen = cc;

	// Move the caret to the beginning by sending left keystrokes
	for (int i = 0; i < iNewStrLen; ++i )
		SendMessage( DXUTGetHWND(), WM_KEYDOWN, VK_LEFT, 0 );
	SendMessage( DXUTGetHWND(), WM_KEYUP, VK_LEFT, 0 );
}


//--------------------------------------------------------------------------------------
// Sends the current composition string to the application by sending keystroke
// messages.
void CKTDGUIIMEEditBox::SendCompString()
{
	for( int i = 0; i < lstrlen( s_CompString.GetBuffer() ); ++i )
		MsgProc( g_pKTDXApp->GetHWND(), WM_CHAR, (WPARAM)s_CompString[i], 0 );
}


//--------------------------------------------------------------------------------------
// Outputs current composition string then cleans up the composition task.
void CKTDGUIIMEEditBox::FinalizeString( bool bSend )
{
	HIMC hImc;
	if( NULL == ( hImc = _ImmGetContext( DXUTGetHWND() ) ) )
		return;

	static bool bProcessing = false;
	if( bProcessing )    // avoid infinite recursion
	{
		DXUTTRACE( L"CKTDGUIIMEEditBox::FinalizeString: Reentrant detected!\n" );
		_ImmReleaseContext( DXUTGetHWND(), hImc );
		return;
	}
	bProcessing = true;

	if( !s_bInsertOnType && bSend )
	{
		// Send composition string to app.
		LONG lLength = lstrlen( s_CompString.GetBuffer() );
		// In case of CHT IME, don't send the trailing double byte space, if it exists.
		if( GetLanguage() == LANG_CHT
			&& s_CompString[lLength - 1] == 0x3000 )
		{
			s_CompString[lLength - 1] = 0;
		}
		SendCompString();
	}

	ResetCompositionString();
	// Clear composition string in IME
	_ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
	// the following line is necessary as Korean IME doesn't close cand list
	// when comp string is cancelled.
	_ImmNotifyIME( hImc, NI_CLOSECANDIDATE, 0, 0 ); 
	_ImmReleaseContext( DXUTGetHWND(), hImc );
	bProcessing = false;
}


//--------------------------------------------------------------------------------------
// Determine whether the reading window should be vertical or horizontal.
void CKTDGUIIMEEditBox::GetReadingWindowOrientation( DWORD dwId )
{
	s_bHorizontalReading = ( s_hklCurrent == _CHS_HKL ) || ( s_hklCurrent == _CHT_HKL2 ) || ( dwId == 0 );
	if( !s_bHorizontalReading && ( dwId & 0x0000FFFF ) == LANG_CHT )
	{
		WCHAR wszRegPath[MAX_PATH];
		HKEY hKey;
		DWORD dwVer = dwId & 0xFFFF0000;
		StringCchCopy( wszRegPath, MAX_PATH, L"software\\microsoft\\windows\\currentversion\\" );
		StringCchCat( wszRegPath, MAX_PATH, ( dwVer >= MAKEIMEVERSION( 5, 1 ) ) ? L"MSTCIPH" : L"TINTLGNT" );
		LONG lRc = RegOpenKeyExW( HKEY_CURRENT_USER, wszRegPath, 0, KEY_READ, &hKey );
		if (lRc == ERROR_SUCCESS)
		{
			DWORD dwSize = sizeof(DWORD), dwMapping, dwType;
			lRc = RegQueryValueExW( hKey, L"Keyboard Mapping", NULL, &dwType, (PBYTE)&dwMapping, &dwSize );
			if (lRc == ERROR_SUCCESS)
			{
				if ( ( dwVer <= MAKEIMEVERSION( 5, 0 ) && 
					( (BYTE)dwMapping == 0x22 || (BYTE)dwMapping == 0x23 ) )
					||
					( ( dwVer == MAKEIMEVERSION( 5, 1 ) || dwVer == MAKEIMEVERSION( 5, 2 ) ) &&
					(BYTE)dwMapping >= 0x22 && (BYTE)dwMapping <= 0x24 )
					)
				{
					s_bHorizontalReading = true;
				}
			}
			RegCloseKey( hKey );
		}
	}
}


//--------------------------------------------------------------------------------------
// Obtain the reading string upon WM_IME_NOTIFY/INM_PRIVATE notification.
void CKTDGUIIMEEditBox::GetPrivateReadingString()
{
	DWORD dwId = GetImeId();
	if( !dwId )
	{
		s_bShowReadingWindow = false;
		return;
	}

	HIMC hImc;
	hImc = _ImmGetContext( DXUTGetHWND() );
	if( !hImc )
	{
		s_bShowReadingWindow = false;
		return;
	}

	DWORD dwReadingStrLen = 0;
	DWORD dwErr = 0;
	WCHAR *pwszReadingStringBuffer = NULL;  // Buffer for when the IME supports GetReadingString()
	WCHAR *wstr = 0;
	bool bUnicodeIme = false;  // Whether the IME context component is Unicode.
	INPUTCONTEXT *lpIC = NULL;

	//대만일경우 Reading출력을 위해 안들어가게 함 09. 05. 12 김정협 
	if( _GetReadingString   && GetLanguage() != LANG_CHT)
	{
		UINT uMaxUiLen;
		BOOL bVertical;
		// Obtain the reading string size
		dwReadingStrLen = _GetReadingString( hImc, 0, NULL, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		if( dwReadingStrLen )
		{
			wstr = pwszReadingStringBuffer = (LPWSTR)HeapAlloc( GetProcessHeap(), 0, sizeof(WCHAR) * dwReadingStrLen );
			if( !pwszReadingStringBuffer )
			{
				// Out of memory. Exit.
				_ImmReleaseContext( DXUTGetHWND(), hImc );
				return;
			}

			// Obtain the reading string
			dwReadingStrLen = _GetReadingString( hImc, dwReadingStrLen, wstr, (PINT)&dwErr, &bVertical, &uMaxUiLen );
		}

		s_bHorizontalReading = !bVertical;
		bUnicodeIme = true;
	}
	else
	{
		// IMEs that doesn't implement Reading String API

		lpIC = _ImmLockIMC( hImc );

		LPBYTE p = 0;
		switch( dwId )
		{
		case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
		case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
		case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
			p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 24 );
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
			dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
			wstr = (WCHAR *)( p + 56 );
			bUnicodeIme = true;
			break;

		case IMEID_CHT_VER50: // 5.0.x.x // WinME
			p = *(LPBYTE *)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 3 * 4 );
			if( !p ) break;
			p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
			dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
			wstr = (WCHAR *)(p + 1*4 + (16*2+2*4) + 5*4);
			bUnicodeIme = false;
			break;

		case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
		case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
		case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
			p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 4);
			if( !p ) break;
			p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
			dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
			wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
			bUnicodeIme = true;
			break;

			// the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
		case IMEID_CHS_VER41:
			{
				int nOffset;
				nOffset = ( GetImeId( 1 ) >= 0x00000002 ) ? 8 : 7;

				p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + nOffset * 4);
				if( !p ) break;
				dwReadingStrLen = *(DWORD *)(p + 7*4 + 16*2*4);
				dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
				dwErr = __min( dwErr, dwReadingStrLen );
				wstr = (WCHAR *)(p + 6*4 + 16*2*1);
				bUnicodeIme = true;
				break;
			}

		case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
			{
				OSVERSIONINFOW osi;
				osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
				GetVersionExW( &osi );

				int nTcharSize = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(WCHAR) : sizeof(char);
				p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
				if( !p ) break;
				dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
				dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
				wstr  = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
				bUnicodeIme = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
			}
		}   // switch
	}

	// Copy the reading string to the candidate list first
	s_CandList.awszCandidate[0][0] = 0;
	s_CandList.awszCandidate[1][0] = 0;
	s_CandList.awszCandidate[2][0] = 0;
	s_CandList.awszCandidate[3][0] = 0;
	s_CandList.dwCount = dwReadingStrLen;
	s_CandList.dwSelection = (DWORD)-1; // do not select any char
	if( bUnicodeIme )
	{
		UINT i;
		for( i = 0; i < dwReadingStrLen; ++i ) // dwlen > 0, if known IME
		{
			if( dwErr <= i && s_CandList.dwSelection == (DWORD)-1 )
			{
				// select error char
				s_CandList.dwSelection = i;
			}

			s_CandList.awszCandidate[i][0] = wstr[i];
			s_CandList.awszCandidate[i][1] = 0;
		}
		s_CandList.awszCandidate[i][0] = 0;
	}
	else
	{
		char *p = (char *)wstr;
		DWORD i, j;
		for( i = 0, j = 0; i < dwReadingStrLen; ++i, ++j ) // dwlen > 0, if known IME
		{
			if( dwErr <= i && s_CandList.dwSelection == (DWORD)-1 )
			{
				s_CandList.dwSelection = j;
			}
			// Obtain the current code page
			WCHAR wszCodePage[8];
			UINT uCodePage = CP_ACP;  // Default code page
			if( GetLocaleInfoW( MAKELCID( GetLanguage(), SORT_DEFAULT ),
				LOCALE_IDEFAULTANSICODEPAGE,
				wszCodePage,
				sizeof(wszCodePage)/sizeof(wszCodePage[0]) ) )
			{
				uCodePage = wcstoul( wszCodePage, NULL, 0 );
			}
			MultiByteToWideChar( uCodePage, 0, p + i, IsDBCSLeadByteEx( uCodePage, p[i] ) ? 2 : 1,
				s_CandList.awszCandidate[j], 1 );
			if( IsDBCSLeadByteEx( uCodePage, p[i] ) )
				++i;
		}
		s_CandList.awszCandidate[j][0] = 0;
		s_CandList.dwCount = j;
	}
	if( !_GetReadingString )
	{
		_ImmUnlockIMCC( lpIC->hPrivate );
		_ImmUnlockIMC( hImc );
		GetReadingWindowOrientation( dwId );
	}
	_ImmReleaseContext( DXUTGetHWND(), hImc );

	if( pwszReadingStringBuffer )
		HeapFree( GetProcessHeap(), 0, pwszReadingStringBuffer );

	// Copy the string to the reading string buffer
	if( s_CandList.dwCount > 0 )
		s_bShowReadingWindow = true;
	else
		s_bShowReadingWindow = false;
	if( s_bHorizontalReading )
	{
		s_CandList.nReadingError = -1;
		s_wszReadingString[0] = 0;
		for( UINT i = 0; i < s_CandList.dwCount; ++i )
		{
			if( s_CandList.dwSelection == i )
				s_CandList.nReadingError = lstrlen( s_wszReadingString );
			StringCchCat( s_wszReadingString, 32, s_CandList.awszCandidate[i] );
		}
	}

	s_CandList.dwPageSize = MAX_CANDLIST;
}


//--------------------------------------------------------------------------------------
// This function is used only briefly in CHT IME handling,
// so accelerator isn't processed.
void CKTDGUIIMEEditBox::PumpMessage()
{
	MSG msg;

	while( PeekMessageW( &msg, NULL, 0, 0, PM_NOREMOVE ) )
	{
		if( !GetMessageW( &msg, NULL, 0, 0 ) )
		{
			g_pKTDXApp->NoticeQuitType( CKTDXApp::KQT_IMEEditBOX_ERROR );
			PostQuitMessage( (int)msg.wParam );
			return;
		}
		TranslateMessage( &msg );
		DispatchMessageA( &msg );
	}
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::OnFocusIn()
{
	CKTDGUIControl::OnFocusIn();

	_ImmAssociateContext( DXUTGetHWND(), NULL );
	ResetCaretBlink();

	//IME 켜고 끄는 부분
	//if( s_bEnableImeSystem )
	//{
	//	_ImmAssociateContext( DXUTGetHWND(), s_hImcDef );
	//	CheckToggleState();
	//}
	//else
	//	_ImmAssociateContext( DXUTGetHWND(), NULL );

	_ImmAssociateContext( DXUTGetHWND(), s_hImcDef );
	CheckToggleState();

	//
	// Set up the IME global state according to the current instance state
	//
	HIMC hImc;
	if( NULL != ( hImc = _ImmGetContext( DXUTGetHWND() ) ) ) 
	{
		if( !s_bEnableImeSystem )
			s_ImeState = IMEUI_STATE_OFF;

		_ImmReleaseContext( DXUTGetHWND(), hImc );
		CheckToggleState();
	}

#ifdef NUMBER_TO_LANGUAGE
	if( true == m_bHasFocus &&
		-1 != m_CustomMsgEditBoxFocusIn )
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxFocusIn, (LPARAM)this );
	}
#endif NUMBER_TO_LANGUAGE
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::OnFocusOut()
{
	bool bHasFocusOld = m_bHasFocus;

	CKTDGUIControl::OnFocusOut();

	ReleaseCapture();

	FinalizeString( false );  // Don't send the comp string as to match RichEdit behavior

	_ImmAssociateContext( DXUTGetHWND(), NULL );


	if( true == bHasFocusOld &&
		-1 != m_CustomMsgEditBoxFocusOut )
	{
		SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxFocusOut, (LPARAM)this );
	}

}

bool CKTDGUIIMEEditBox::IsKorean()
{
	if(GetPrimaryLanguage() == LANG_KOREAN)
		return true;
	else
		return false;
}


//--------------------------------------------------------------------------------------
bool CKTDGUIIMEEditBox::StaticMsgProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	HIMC hImc;

	if( !s_bEnableImeSystem )
		return false;

#if defined(DEBUG) || defined(_DEBUG)
	m_bIMEStaticMsgProcCalled = true;
#endif

	switch( uMsg )
	{
	case WM_ACTIVATEAPP:
		if( wParam )
		{
			// Populate s_Locale with the list of keyboard layouts.
			UINT cKL = GetKeyboardLayoutList( 0, NULL );
			s_Locale.RemoveAll();
			HKL *phKL = new HKL[cKL];
			if( phKL )
			{
				GetKeyboardLayoutList( cKL, phKL );
				for( UINT i = 0; i < cKL; ++i )
				{
					CInputLocale Locale;

					// Filter out East Asian languages that are not IME.
					if( ( PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_CHINESE ||
						PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_JAPANESE ||
						PRIMARYLANGID( LOWORD( phKL[i] ) ) == LANG_KOREAN ) &&
						!_ImmIsIME( phKL[i] ) )
						continue;

					// If this language is already in the list, don't add it again.
					bool bBreak = false;
					for( int e = 0; e < s_Locale.GetSize(); ++e )
						if( LOWORD( s_Locale.GetAt( e ).m_hKL ) ==
							LOWORD( phKL[i] ) )
						{
							bBreak = true;
							break;
						}
						if( bBreak )
							break;

						Locale.m_hKL = phKL[i];
						WCHAR wszDesc[128] = L"";
						switch( PRIMARYLANGID( LOWORD( phKL[i] ) ) )
						{
							// Simplified Chinese
						case LANG_CHINESE:
							switch( SUBLANGID( LOWORD( phKL[i] ) ) )
							{
							case SUBLANG_CHINESE_SIMPLIFIED:
								StringCchCopy( Locale.m_wszLangAbb, 3, s_aszIndicator[INDICATOR_CHS] );
								break;
							case SUBLANG_CHINESE_TRADITIONAL:
								StringCchCopy( Locale.m_wszLangAbb, 3, s_aszIndicator[INDICATOR_CHT] );
								break;
							default:    // unsupported sub-language
								GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszDesc, 128 );
								Locale.m_wszLangAbb[0] = wszDesc[0];
								Locale.m_wszLangAbb[1] = towlower( wszDesc[1] );
								Locale.m_wszLangAbb[2] = L'\0';
								break;
							}
							break;
							// Korean
						case LANG_KOREAN:
							StringCchCopy( Locale.m_wszLangAbb, 3, s_aszIndicator[INDICATOR_KOREAN] );
							break;
							// Japanese
						case LANG_JAPANESE:
							StringCchCopy( Locale.m_wszLangAbb, 3, s_aszIndicator[INDICATOR_JAPANESE] );
							break;         
						default:
							// A non-IME language.  Obtain the language abbreviation
							// and store it for rendering the indicator later.
							GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SABBREVLANGNAME, wszDesc, 128 );
							Locale.m_wszLangAbb[0] = wszDesc[0];
							Locale.m_wszLangAbb[1] = towlower( wszDesc[1] );
							Locale.m_wszLangAbb[2] = L'\0';
							break;
						}

						GetLocaleInfoW( MAKELCID( LOWORD( phKL[i] ), SORT_DEFAULT ), LOCALE_SLANGUAGE, wszDesc, 128 );
						StringCchCopy( Locale.m_wszLang, 64, wszDesc );

						s_Locale.Add( Locale );
				}
				delete[] phKL;
			}
		}
		break;

	case WM_INPUTLANGCHANGE:
		DXUTTRACE( L"WM_INPUTLANGCHANGE\n" );
		{
			UINT uLang = GetPrimaryLanguage();
			CheckToggleState();
			if ( uLang != GetPrimaryLanguage() )
			{
				// Korean IME always inserts on keystroke.  Other IMEs do not.
				s_bInsertOnType = ( GetPrimaryLanguage() == LANG_KOREAN );
			}

			// IME changed.  Setup the new IME.
			SetupImeApi();
			if( _ShowReadingWindow )
			{
				if ( NULL != ( hImc = _ImmGetContext( DXUTGetHWND() ) ) )
				{
					_ShowReadingWindow( hImc, false );
					_ImmReleaseContext( DXUTGetHWND(), hImc );
				}
			}
		}
		return true;

	case WM_IME_SETCONTEXT:
		DXUTTRACE( L"WM_IME_SETCONTEXT\n" );
		//
		// We don't want anything to display, so we have to clear this
		//
		lParam = 0;
		return false;

		// Handle WM_IME_STARTCOMPOSITION here since
		// we do not want the default IME handler to see
		// this when our fullscreen app is running.
	case WM_IME_STARTCOMPOSITION:
		DXUTTRACE( L"WM_IME_STARTCOMPOSITION\n" );
		ResetCompositionString();

		// Since the composition string has its own caret, we don't render
		// the edit control's own caret to avoid double carets on screen.
		s_bHideCaret = true;
		return true;

	case WM_IME_COMPOSITION:
		DXUTTRACE( L"WM_IME_COMPOSITION\n" );
		return false;
	}

	return false;
}

//--------------------------------------------------------------------------------------
bool CKTDGUIIMEEditBox::HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

	bool bHandled = false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_TAB:
				// We don't process Tab in case keyboard input is enabled and the user
				// wishes to Tab to other controls.
				break;

			case VK_HOME:
				PlaceCaret( 0 );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_END:
				PlaceCaret( m_Buffer.GetTextSize() );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_INSERT:
				if( GetKeyState( VK_CONTROL ) < 0 )
				{
					// Control Insert. Copy to clipboard
					CopyToClipboard();
				} else
					if( GetKeyState( VK_SHIFT ) < 0 )
					{
						// Shift Insert. Paste from clipboard
						PasteFromClipboard();
					} else
					{
						// Toggle caret insert mode
						m_bInsertMode = !m_bInsertMode;
					}
					break;

			case VK_DELETE:
				// Check if there is a text selection.
				if( m_nCaret != m_nSelStart )
				{
					DeleteSelectionText();
					if( m_CustomMsgEditBoxChange != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
					else
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
				}
				else
				{
					// Deleting one character
					if( m_Buffer.RemoveChar( m_nCaret ) )
					{
						if( m_CustomMsgEditBoxChange != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
						else
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
					}
				}
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_LEFT:
				if( GetKeyState( VK_CONTROL ) < 0 )
				{
					// Control is down. Move the caret to a new item
					// instead of a character.
					m_Buffer.GetPriorItemPos( m_nCaret, &m_nCaret );
					PlaceCaret( m_nCaret );
				}
				else
					if( m_nCaret > 0 )
						PlaceCaret( m_nCaret - 1 );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_RIGHT:
				if( GetKeyState( VK_CONTROL ) < 0 )
				{
					// Control is down. Move the caret to a new item
					// instead of a character.
					m_Buffer.GetNextItemPos( m_nCaret, &m_nCaret );
					PlaceCaret( m_nCaret );
				}
				else
					if( m_nCaret < m_Buffer.GetTextSize() )
						PlaceCaret( m_nCaret + 1 );
				if( GetKeyState( VK_SHIFT ) >= 0 )
					// Shift is not down. Update selection
					// start along with the caret.
					m_nSelStart = m_nCaret;
				ResetCaretBlink();
				bHandled = true;
				break;

			case VK_UP:
				if(GetGroupID() >= 0)
				{
					int tempCaret = m_Buffer.GetCaretLineMove(m_nCaret, -1);
					if(tempCaret >= 0)
					{
						m_nCaret = tempCaret;
						PlaceCaret(m_nCaret);

						//if( GetKeyState( VK_SHIFT ) >= 0 )                            
						m_nSelStart = m_nCaret;
						ResetCaretBlink();
					}

				}   
				bHandled = true;
				break;
			case VK_DOWN:
				if(GetGroupID() >= 0)
				{
					int tempCaret = m_Buffer.GetCaretLineMove(m_nCaret, 1);
					if(tempCaret >= 0)
					{
						m_nCaret = tempCaret;
						PlaceCaret(m_nCaret);

						//if( GetKeyState( VK_SHIFT ) >= 0 )                            
						m_nSelStart = m_nCaret;
						ResetCaretBlink();
					}                    
				}               
				bHandled = true;
				break;

			default:
				bHandled = wParam != VK_ESCAPE;  // Let the application handle Esc.
			}
		}
	}
	return bHandled;
}

//--------------------------------------------------------------------------------------
bool CKTDGUIIMEEditBox::HandleMouse( UINT uMsg, POINT pt, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return false;
#endif

	pt.x = GET_X_LPARAM(lParam); 
	pt.y = GET_Y_LPARAM(lParam); 

	D3DXVECTOR2 pos = g_pKTDXApp->MouseConvertByResolution( (float)pt.x, (float)pt.y );
	pt.x = (LONG)pos.x;
	pt.y = (LONG)pos.y;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			// Check if this click is on top of the composition string
			int nCompStrWidth;
			s_CompString.CPtoX( s_CompString.GetTextSize(), FALSE, &nCompStrWidth );

			// pFont->nHeight 대신 폰트 크기 설정하기
			if( s_ptCompString.x <= pt.x &&
				s_ptCompString.y <= pt.y &&
				s_ptCompString.x + nCompStrWidth > pt.x &&
				s_ptCompString.y + m_pFont->GetHeight()  > pt.y )
			{
				int nCharBodyHit, nCharHit;
				int nTrail;

				// Determine the character clicked on.
				s_CompString.XtoCP( pt.x - s_ptCompString.x , &nCharBodyHit, &nTrail );
				if( nTrail && nCharBodyHit < s_CompString.GetTextSize() )
					nCharHit = nCharBodyHit + 1;
				else
					nCharHit = nCharBodyHit;

				// Now generate keypress events to move the comp string cursor
				// to the click point.  First, if the candidate window is displayed,
				// send Esc to close it.
				HIMC hImc = _ImmGetContext( DXUTGetHWND() );
				if( !hImc )
				{
					return true;
				}

				_ImmNotifyIME( hImc, NI_CLOSECANDIDATE, 0, 0 );
				_ImmReleaseContext( DXUTGetHWND(), hImc );

				switch( GetPrimaryLanguage() )
				{
				case LANG_JAPANESE:
					// For Japanese, there are two cases.  If s_nFirstTargetConv is
					// -1, the comp string hasn't been converted yet, and we use
					// s_nCompCaret.  For any other value of s_nFirstTargetConv,
					// the string has been converted, so we use clause information.

					if( s_nFirstTargetConv != -1 )
					{
						int nClauseClicked = 0;
						while( (int)s_adwCompStringClause[nClauseClicked + 1] <= nCharBodyHit )
							++nClauseClicked;

						int nClauseSelected = 0;
						while( (int)s_adwCompStringClause[nClauseSelected + 1] <= s_nFirstTargetConv )
							++nClauseSelected;

						BYTE nVirtKey = nClauseClicked > nClauseSelected ? VK_RIGHT : VK_LEFT;
						int nSendCount = abs( nClauseClicked - nClauseSelected );
						while( nSendCount-- > 0 )
							SendKey( nVirtKey );

						return true;
					}

					// Not converted case. Fall thru to Chinese case.

				case LANG_CHINESE:
					{
						// For Chinese, use s_nCompCaret.
						BYTE nVirtKey = nCharHit > s_nCompCaret ? VK_RIGHT : VK_LEFT;
						int nSendCount = abs( nCharHit - s_nCompCaret );
						while( nSendCount-- > 0 )
							SendKey( nVirtKey );
						break;
					}
				}
				return true;
			}

			// Check if the click is on top of the candidate window
			if( s_CandList.bShowWindow && PtInRect( &s_CandList.rcCandidate, pt ) )
			{
				if( s_bVerticalCand )
				{
					// Vertical candidate window

					// Compute the row the click is on
					int nRow = ( pt.y - s_CandList.rcCandidate.top ) / m_pFont->GetHeight();

					if( nRow < (int)s_CandList.dwCount )
					{
						// nRow is a valid entry.
						// Now emulate keystrokes to select the candidate at this row.
						switch( GetPrimaryLanguage() )
						{
						case LANG_CHINESE:
						case LANG_KOREAN:
							// For Chinese and Korean, simply send the number keystroke.
							SendKey( (BYTE) ('0' + nRow + 1) );
							break;

						case LANG_JAPANESE:
							// For Japanese, move the selection to the target row,
							// then send Right, then send Left.

							BYTE nVirtKey;
							if( nRow > (int)s_CandList.dwSelection )
								nVirtKey = VK_DOWN;
							else
								nVirtKey = VK_UP;
							int nNumToHit = abs( int( nRow - s_CandList.dwSelection ) );
							for( int nStrike = 0; nStrike < nNumToHit; ++nStrike )
								SendKey( nVirtKey );

							// Do this to close the candidate window without ending composition.
							SendKey( VK_RIGHT );
							SendKey( VK_LEFT );

							break;
						}
					}
				} else
				{
					// Horizontal candidate window

					// Determine which the character the click has hit.
					int nCharHit;
					int nTrail;
					s_CandList.HoriCand.XtoCP( pt.x - s_CandList.rcCandidate.left, &nCharHit, &nTrail );

					// Determine which candidate string the character belongs to.
					int nCandidate = s_CandList.dwCount - 1;

					int nEntryStart = 0;
					for( UINT i = 0; i < s_CandList.dwCount; ++i )
					{
						if( nCharHit >= nEntryStart )
						{
							// Haven't found it.
							nEntryStart += lstrlenW( s_CandList.awszCandidate[i] ) + 1;  // plus space separator
						} else
						{
							// Found it.  This entry starts at the right side of the click point,
							// so the char belongs to the previous entry.
							nCandidate = i - 1;
							break;
						}
					}

					// Now emulate keystrokes to select the candidate entry.
					switch( GetPrimaryLanguage() )
					{
					case LANG_CHINESE:
					case LANG_KOREAN:
						// For Chinese and Korean, simply send the number keystroke.
						SendKey( (BYTE) ('0' + nCandidate + 1) );
						break;
					}
				}
				return true;
			}
		}
	}

	// If we didn't care for the msg, let the parent process it.
	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if( !m_bHasFocus )
			{
				m_pDialog->RequestFocus( this );
			}

			if( !ContainPTForIME( pt ) )
			{
				return false;
			}

			m_bMouseDrag = true;
#ifndef DLL_BUILD
			SetCapture( DXUTGetHWND() );
#endif
			// Determine the character corresponding to the coordinates.
			int nCP, nTrail, nX1st;
			m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nX1st );  // X offset of the 1st visible char

			if(GetGroupID() < 0)
			{                
				if( FAILED( m_Buffer.XtoCP( pt.x - m_ptTextAlignOffset.x - m_rcText.left + nX1st, &nCP, &nTrail ) ) )
					return true;
			}
			else
			{
				if( FAILED( m_Buffer.XYtoCP( pt.x - m_ptTextAlignOffset.x - m_rcText.left + nX1st, pt.y+m_ptTextAlignOffset.y - m_rcText.top, &nCP, &nTrail ) ) )
					return true;
			}

			// Cap at the NULL character.
			if( nTrail && nCP < m_Buffer.GetTextSize() )
				PlaceCaret( nCP + 1 );
			else
				PlaceCaret( nCP );
			m_nSelStart = m_nCaret;
			ResetCaretBlink();

			return true;
		}

	case WM_LBUTTONUP:
		ReleaseCapture();
		m_bMouseDrag = false;
		break;

	case WM_MOUSEMOVE:
		if( GetGroupID() < 0 && m_bMouseDrag )
		{
			// Determine the character corresponding to the coordinates.
			int nCP, nTrail, nX1st;
			m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nX1st );  // X offset of the 1st visible char

			if( SUCCEEDED( m_Buffer.XtoCP( pt.x - m_ptTextAlignOffset.x - m_rcText.left + nX1st, &nCP, &nTrail ) ) )
			{
				// Cap at the NULL character.
				if( nTrail && nCP < m_Buffer.GetTextSize() )
					PlaceCaret( nCP + 1 );
				else
					PlaceCaret( nCP );
			}


			// 드래그중에 editbox 영역을 벗어나면 드래그를 멈춤
			if( !ContainPTForIME( pt ) )
			{
				ReleaseCapture();
				m_bMouseDrag = false;
			}

		}
		break;
	}
	return false;
}

//--------------------------------------------------------------------------------------
bool CKTDGUIIMEEditBox::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( m_bEnable == false || m_bShow == false )
		return false;

#ifdef DLL_BUILD
	if( m_bUpdate == false )
		return false;
#endif

#if defined(DEBUG) || defined(_DEBUG)
	// KTDGUIIMEEditBox.cpp used to call CKTDGUIIMEEditBox::MsgProc() so that, but now
	// this is the application's responsiblity.  To do this, call 
	// CKTDGUIDialogManager::MsgProc() before calling this function.
	assert( m_bIMEStaticMsgProcCalled && L"To fix, call CKTDGUIDialogManager::MsgProc() first" );
#endif

	bool trappedData;
	bool *trapped = &trappedData;

	HIMC hImc;
	static LPARAM lAlt = 0x80000000, lCtrl = 0x80000000, lShift = 0x80000000;

	*trapped = false;
	if( !s_bEnableImeSystem )
		return EBProc( uMsg, wParam, lParam );

	switch( uMsg )
	{
		//
		//  IME Handling
		//
	case WM_IME_COMPOSITION:
		DXUTTRACE( L"WM_IME_COMPOSITION\n" );
		{
			LONG lRet;  // Returned count in CHARACTERS
			WCHAR wszCompStr[MAX_COMPSTRING_SIZE];

			*trapped = true;
			if( NULL == ( hImc = _ImmGetContext( DXUTGetHWND() ) ) )
			{
				break;
			}

			// Get the caret position in composition string
			if ( lParam & GCS_CURSORPOS )
			{
				s_nCompCaret = _ImmGetCompositionStringW( hImc, GCS_CURSORPOS, NULL, 0 );
				if( s_nCompCaret < 0 )
					s_nCompCaret = 0; // On error, set caret to pos 0.
			}

			// ResultStr must be processed before composition string.
			//
			// This is because for some IMEs, such as CHT, pressing Enter
			// to complete the composition sends WM_IME_COMPOSITION with both
			// GCS_RESULTSTR and GCS_COMPSTR.  Retrieving the result string
			// gives the correct string, while retrieving the comp string
			// (GCS_COMPSTR) gives empty string.  GCS_RESULTSTR should be
			// handled first so that the application receives the string.  Then
			// GCS_COMPSTR can be handled to clear the comp string buffer.

			if ( lParam & GCS_RESULTSTR )
			{
				DXUTTRACE( L"  GCS_RESULTSTR\n" );
				lRet = _ImmGetCompositionStringW( hImc, GCS_RESULTSTR, wszCompStr, sizeof( wszCompStr ) );
				if( lRet > 0 )
				{
					lRet /= sizeof(WCHAR);
					wszCompStr[lRet] = 0;  // Force terminate
					TruncateCompString( false, (int)wcslen( wszCompStr ) );
					s_CompString.SetText( wszCompStr );
					SendCompString();
					ResetCompositionString();
				}
			}

			//
			// Reads in the composition string.
			//
			if ( lParam & GCS_COMPSTR )
			{
				DXUTTRACE( L"  GCS_COMPSTR\n" );
				//////////////////////////////////////////////////////
				// Retrieve the latest user-selected IME candidates
				lRet = _ImmGetCompositionStringW( hImc, GCS_COMPSTR, wszCompStr, sizeof( wszCompStr ) );
				if( lRet > 0 )
				{
					lRet /= sizeof(WCHAR);  // Convert size in byte to size in char
					wszCompStr[lRet] = 0;  // Force terminate
					//
					// Remove the whole of the string
					//
					TruncateCompString( false, (int)wcslen( wszCompStr ) );

					s_CompString.SetText( wszCompStr );

					// Older CHT IME uses composition string for reading string
					if ( GetLanguage() == LANG_CHT && !GetImeId() )
					{
						if( lstrlen( s_CompString.GetBuffer() ) )
						{
							////09. 05. 13 김정협 Reading출력 범위 조정 
							s_CandList.dwCount = MAX_READING_COUNT;             // Maximum possible length for reading string is 4
							s_CandList.dwSelection = (DWORD)-1; // don't select any candidate

							// Copy the reading string to the candidate list
							//09. 05. 13 김정협 Reading출력 범위 조정 
							for( int i = MAX_READING_COUNT-1; i >= 0; --i )
							{
								if( i > lstrlen( s_CompString.GetBuffer() ) - 1 )
									s_CandList.awszCandidate[i][0] = 0;  // Doesn't exist
								else
								{
									s_CandList.awszCandidate[i][0] = s_CompString[i];
									s_CandList.awszCandidate[i][1] = 0;
								}
							}
							s_CandList.dwPageSize = MAX_CANDLIST;
							// Clear comp string after we are done copying
							ZeroMemory( (LPVOID)s_CompString.GetBuffer(), 4 * sizeof(WCHAR) );
							s_bShowReadingWindow = true;
							GetReadingWindowOrientation( 0 );
							if( s_bHorizontalReading )
							{
								s_CandList.nReadingError = -1;  // Clear error

								// Create a string that consists of the current
								// reading string.  Since horizontal reading window
								// is used, we take advantage of this by rendering
								// one string instead of several.
								//
								// Copy the reading string from the candidate list
								// to the reading string buffer.
								s_wszReadingString[0] = 0;
								for( UINT i = 0; i < s_CandList.dwCount; ++i )
								{
									if( s_CandList.dwSelection == i )
										s_CandList.nReadingError = lstrlen( s_wszReadingString );
									StringCchCat( s_wszReadingString, 32, s_CandList.awszCandidate[i] );
								}
							}
						}
						else
						{
							s_CandList.dwCount = 0;
							s_bShowReadingWindow = false;
						}
					}

					if( s_bInsertOnType )
					{
						// Send composition string to the edit control
						SendCompString();
						// Restore the caret to the correct location.
						// It's at the end right now, so compute the number
						// of times left arrow should be pressed to
						// send it to the original position.
						int nCount = lstrlen( s_CompString.GetBuffer() + s_nCompCaret );
						// Send left keystrokes
						for( int i = 0; i < nCount; ++i )
							SendMessage( DXUTGetHWND(), WM_KEYDOWN, VK_LEFT, 0 );
						SendMessage( DXUTGetHWND(), WM_KEYUP, VK_LEFT, 0 );
					}
				}

#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
				/// 한글 글자 하나만 변경 되어도 메시지 전송
				if( true == m_bIsForceSendCustomMsgChange )
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION

				ResetCaretBlink();
			}

			// Retrieve comp string attributes
			if( lParam & GCS_COMPATTR )
			{
				lRet = _ImmGetCompositionStringW( hImc, GCS_COMPATTR, s_abCompStringAttr, sizeof( s_abCompStringAttr ) );
				if( lRet > 0 )
					s_abCompStringAttr[lRet] = 0;  // ??? Is this needed for attributes?
			}

			// Retrieve clause information
			if( lParam & GCS_COMPCLAUSE )
			{
#ifdef IME_CRASH_FIX
				switch( GetPrimaryLanguage() )
				{
				case LANG_JAPANESE:
#endif IME_CRASH_FIX
				lRet = _ImmGetCompositionStringW(hImc, GCS_COMPCLAUSE, s_adwCompStringClause, sizeof( s_adwCompStringClause ) );
				s_adwCompStringClause[lRet / sizeof(DWORD)] = 0;  // Terminate
#ifdef IME_CRASH_FIX
				break;
				}
#endif IME_CRASH_FIX
			}

			_ImmReleaseContext( DXUTGetHWND(), hImc );
		}
		break;

	case WM_IME_ENDCOMPOSITION:
		DXUTTRACE( L"WM_IME_ENDCOMPOSITION\n" );
		TruncateCompString();
		ResetCompositionString();
		// We can show the edit control's caret again.
		s_bHideCaret = false;
		// Hide reading window
		s_bShowReadingWindow = false;
		break;

	case WM_IME_NOTIFY:
		DXUTTRACE( L"WM_IME_NOTIFY %u\n", wParam );
		switch( wParam )
		{
		case IMN_SETCONVERSIONMODE:
			DXUTTRACE( L"  IMN_SETCONVERSIONMODE\n" );
		case IMN_SETOPENSTATUS:
			DXUTTRACE( L"  IMN_SETOPENSTATUS\n" );
			CheckToggleState();
			break;

		case IMN_OPENCANDIDATE:
		case IMN_CHANGECANDIDATE:
			{
				DXUTTRACE( wParam == IMN_CHANGECANDIDATE ? L"  IMN_CHANGECANDIDATE\n" : L"  IMN_OPENCANDIDATE\n" );

				s_CandList.bShowWindow = true;
				*trapped = true;
				if( NULL == ( hImc = _ImmGetContext( DXUTGetHWND() ) ) )
					break;

				LPCANDIDATELIST lpCandList = NULL;
				DWORD dwLenRequired;

				s_bShowReadingWindow = false;
				// Retrieve the candidate list
				dwLenRequired = _ImmGetCandidateListW( hImc, 0, NULL, 0 );
				if( dwLenRequired )
				{
					lpCandList = (LPCANDIDATELIST)HeapAlloc( GetProcessHeap(), 0, dwLenRequired );
					dwLenRequired = _ImmGetCandidateListW( hImc, 0, lpCandList, dwLenRequired );
				}

				if( lpCandList )
				{
					// Update candidate list data
					s_CandList.dwSelection = lpCandList->dwSelection;
					s_CandList.dwCount = lpCandList->dwCount;

					int nPageTopIndex = 0;
					s_CandList.dwPageSize = __min( lpCandList->dwPageSize, MAX_CANDLIST );
					if( GetPrimaryLanguage() == LANG_JAPANESE )
					{
						// Japanese IME organizes its candidate list a little
						// differently from the other IMEs.
						nPageTopIndex = ( s_CandList.dwSelection / s_CandList.dwPageSize ) * s_CandList.dwPageSize;
					}
					else
						nPageTopIndex = lpCandList->dwPageStart;

					// Make selection index relative to first entry of page
					s_CandList.dwSelection = ( GetLanguage() == LANG_CHS && !GetImeId() ) ? (DWORD)-1
						: s_CandList.dwSelection - nPageTopIndex;

					ZeroMemory( s_CandList.awszCandidate, sizeof(s_CandList.awszCandidate) );
					for( UINT i = nPageTopIndex, j = 0;
						(DWORD)i < lpCandList->dwCount && j < s_CandList.dwPageSize;
						i++, j++ )
					{
						// Initialize the candidate list strings
						LPWSTR pwsz = s_CandList.awszCandidate[j];
						// For every candidate string entry,
						// write [index] + Space + [String] if vertical,
						// write [index] + [String] + Space if horizontal.
						*pwsz++ = (WCHAR)( L'0' + ( (j + 1) % 10 ) );  // Index displayed is 1 based
						if( s_bVerticalCand )
							*pwsz++ = L' ';
						WCHAR *pwszNewCand = (LPWSTR)( (LPBYTE)lpCandList + lpCandList->dwOffset[i] );
						while ( *pwszNewCand )
							*pwsz++ = *pwszNewCand++;
						if( !s_bVerticalCand )
							*pwsz++ = L' ';
						*pwsz = 0;  // Terminate
					}

					// Make dwCount in s_CandList be number of valid entries in the page.
					s_CandList.dwCount = lpCandList->dwCount - lpCandList->dwPageStart;
					if( s_CandList.dwCount > lpCandList->dwPageSize )
						s_CandList.dwCount = lpCandList->dwPageSize;

					HeapFree( GetProcessHeap(), 0, lpCandList );
					_ImmReleaseContext( DXUTGetHWND(), hImc );

					// Korean and old Chinese IME can't have selection.
					// User must use the number hotkey or Enter to select
					// a candidate.
					if( GetPrimaryLanguage() == LANG_KOREAN ||
						GetLanguage() == LANG_CHT && !GetImeId() )
					{
						s_CandList.dwSelection = (DWORD)-1;
					}

					// Initialize s_CandList.HoriCand if we have a
					// horizontal candidate window.
					if( !s_bVerticalCand )
					{
						WCHAR wszCand[256] = L"";

						s_CandList.nFirstSelected = 0;
						s_CandList.nHoriSelectedLen = 0;
						for( UINT i = 0; i < MAX_CANDLIST; ++i )
						{
							if( s_CandList.awszCandidate[i][0] == L'\0' )
								break;

							WCHAR wszEntry[32];
							StringCchPrintf( wszEntry, 32, L"%s ", s_CandList.awszCandidate[i] );
							// If this is the selected entry, mark its char position.
							if( s_CandList.dwSelection == i )
							{
								s_CandList.nFirstSelected = lstrlen( wszCand );
								s_CandList.nHoriSelectedLen = lstrlen( wszEntry ) - 1;  // Minus space
							}
							StringCchCat( wszCand, 256, wszEntry );
						}
						wszCand[lstrlen(wszCand) - 1] = L'\0';  // Remove the last space
						s_CandList.HoriCand.SetText( wszCand );
					}
				}
				break;
			}

		case IMN_CLOSECANDIDATE:
			{
				DXUTTRACE( L"  IMN_CLOSECANDIDATE\n" );
				s_CandList.bShowWindow = false;
				if( !s_bShowReadingWindow )
				{
					s_CandList.dwCount = 0;
					ZeroMemory( s_CandList.awszCandidate, sizeof(s_CandList.awszCandidate) );
				}
				*trapped = true;
				break;
			}

		case IMN_PRIVATE:
			DXUTTRACE( L"  IMN_PRIVATE\n" );
			{
				if( !s_CandList.bShowWindow )
					GetPrivateReadingString();

				// Trap some messages to hide reading window
				DWORD dwId = GetImeId();
				switch( dwId )
				{
				case IMEID_CHT_VER42:
				case IMEID_CHT_VER43:
				case IMEID_CHT_VER44:
				case IMEID_CHS_VER41:
				case IMEID_CHS_VER42:
					if( ( lParam == 1 ) || ( lParam == 2 ) )
					{
						*trapped = true;
					}
					break;

				case IMEID_CHT_VER50:
				case IMEID_CHT_VER51:
				case IMEID_CHT_VER52:
				case IMEID_CHT_VER60:
				case IMEID_CHS_VER53:
					if( (lParam == 16) || (lParam == 17) || (lParam == 26) || (lParam == 27) || (lParam == 28) )
					{
						*trapped = true;
					}
					break;
				}
			}
			break;

		default:
			*trapped = true;
			break;
		}
		break;

		// When Text Service Framework is installed in Win2K, Alt+Shift and Ctrl+Shift combination (to switch input
		// locale / keyboard layout) doesn't send WM_KEYUP message for the key that is released first. We need to check
		// if these keys are actually up whenever we receive key up message for other keys.
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if ( !( lAlt & 0x80000000 ) && wParam != VK_MENU && ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) == 0 )
		{
			PostMessageW( GetFocus(), WM_KEYUP, (WPARAM)VK_MENU, ( lAlt & 0x01ff0000 ) | 0xC0000001 );
		}   
		else if ( !( lCtrl & 0x80000000 ) && wParam != VK_CONTROL && ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) == 0 )
		{
			PostMessageW( GetFocus(), WM_KEYUP, (WPARAM)VK_CONTROL, ( lCtrl & 0x01ff0000 ) | 0xC0000001 );
		}
		else if ( !( lShift & 0x80000000 ) && wParam != VK_SHIFT && ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) == 0 )
		{
			PostMessageW( GetFocus(), WM_KEYUP, (WPARAM)VK_SHIFT, ( lShift & 0x01ff0000 ) | 0xC0000001 );
		}
		// fall through WM_KEYDOWN / WM_SYSKEYDOWN
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		switch ( wParam )
		{
		case VK_MENU:
			lAlt = lParam;
			break;
		case VK_SHIFT:
			lShift = lParam;
			break;
		case VK_CONTROL:
			lCtrl = lParam;
			break;
		}
		//break;
		// Fall through to default case
		// so we invoke the parent.

	default:
		// Let the parent handle the message that we
		// don't handle.
		return EBProc( uMsg, wParam, lParam );

	}  // switch

	return *trapped;
}

bool CKTDGUIIMEEditBox::EBProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_CHAR:
		{
			switch( (WCHAR)wParam )
			{
				// Backspace
			case VK_BACK:
				{
					PlaySound( m_pSndKeyDown );

					// If there's a selection, treat this
					// like a delete key.
					if( m_nCaret != m_nSelStart )
					{
						DeleteSelectionText();

						if( m_CustomMsgEditBoxChange != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
						else
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
					}
					else
						if( m_nCaret > 0 )
						{
							// Move the caret, then delete the char.
							PlaceCaret( m_nCaret - 1 );
							m_nSelStart = m_nCaret;
							m_Buffer.RemoveChar( m_nCaret );


							if( m_CustomMsgEditBoxChange != -1 )
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
							else
								SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
						}
						else
						{
							if ( GetPrevIMEEditBox() != NULL )
							{
								GetPrevIMEEditBox()->EBProc( uMsg, wParam, lParam );
								GetPrevIMEEditBox()->RequestFocus();
								return false;
							}
						}
						ResetCaretBlink();
						break;
				}

			case 24:        // Ctrl-X Cut
			case VK_CANCEL: // Ctrl-C Copy
				{					
					CopyToClipboard();

					// If the key is Ctrl-X, delete the selection too.
					if( (WCHAR)wParam == 24 )
					{
						DeleteSelectionText();

						if( m_CustomMsgEditBoxChange != -1 )
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
						else
							SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
					}

					break;
				}

				// Ctrl-V Paste
			case 22:
				{
					PasteFromClipboard();

					if( m_CustomMsgEditBoxChange != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
					else
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );

					break;
				}

				// Ctrl-A Select All
			case 1:
				if( m_nSelStart == m_nCaret )
				{
					m_nSelStart = 0;
					PlaceCaret( m_Buffer.GetTextSize() );
				}
				break;		

				// Junk characters we don't want in the string
			case 26:  // Ctrl Z
			case 2:   // Ctrl B
			case 14:  // Ctrl N
			case 19:  // Ctrl S
			case 4:   // Ctrl D
			case 6:   // Ctrl F
			case 7:   // Ctrl G
			case 10:  // Ctrl J
			case 11:  // Ctrl K
			case 12:  // Ctrl L
			case 17:  // Ctrl Q
			case 23:  // Ctrl W
			case 5:   // Ctrl E
			case 18:  // Ctrl R
			case 20:  // Ctrl T
			case 25:  // Ctrl Y
			case 21:  // Ctrl U
			case 9:   // Ctrl I
			case 15:  // Ctrl O
			case 16:  // Ctrl P
			case 27:  // Ctrl [
			case 29:  // Ctrl ]
			case 28:  // Ctrl \ 
				break;

			case VK_RETURN:
				// Invoke the callback when the user presses Enter.
				if(GetGroupID() < 0)
				{
					g_pKTDXApp->GetDIManager()->Getkeyboard()->SetExtraEnter(TRUE);

					if( m_CustomMsgEditBoxEnter != -1 )
					{
						//{{09.03.27 태완 : IME 엔터와 다이얼로그 엔터가 한 번에 중복처리되지 않게 하기 위해서 락을 건다
						//g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock( DIK_RETURN, TRUE );
						g_pKTDXApp->GetDIManager()->Getkeyboard()->SetLock( DIK_RETURN, TRUE );
						//}}
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxEnter, (LPARAM)this );
					}
					else
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_ENTER, (LPARAM)this );
					break;
				}
			default:
				{
					if( GetKeyState( VK_SHIFT ) < 0 && GetKeyState( VK_CONTROL ) < 0)
						break;

					//if ( m_pSndKeyDown != NULL )
					//	m_pSndKeyDown->Play();

					PlaySound( m_pSndKeyDown );

					// If there's a selection and the user
					// starts to type, the selection should
					// be deleted.
					if( m_nCaret != m_nSelStart )
						DeleteSelectionText();

					// If we are in overwrite mode and there is already
					// a char at the caret's position, simply replace it.
					// Otherwise, we insert the char as normal.
					if( !m_bInsertMode && m_nCaret < m_Buffer.GetTextSize() )
					{
						m_Buffer[m_nCaret] = (WCHAR)wParam;
						PlaceCaret( m_nCaret + 1 );
						m_nSelStart = m_nCaret;
					} else
					{
						// Insert the char
						if(GetGroupID() >= 0 && wParam == 13) // 'Enter'
						{
							if( m_Buffer.InsertChar( m_nCaret, (WCHAR)'\n' ) )
							{                            
								PlaceCaret( m_nCaret + 1 );
								m_nSelStart = m_nCaret;							
							}                            
						}
						else if( m_Buffer.InsertChar( m_nCaret, (WCHAR)wParam ) )
						{                   
							const WCHAR *buffer = m_Buffer.GetBuffer();
							if(GetGroupID() >= 0 && buffer[m_nCaret] == L'\n')
								PlaceCaret( m_nCaret + 2 );
							else
								PlaceCaret( m_nCaret + 1 );
							m_nSelStart = m_nCaret;							
						}
						else
						{
							if ( GetNextIMEEditBox() != NULL )
							{
								GetNextIMEEditBox()->EBProc( uMsg, wParam, lParam );

								LONG lRet;  // Returned count in CHARACTERS
								WCHAR wszCompStr[MAX_COMPSTRING_SIZE] = {0};
								HIMC hImc;
								if( ( hImc = _ImmGetContext( DXUTGetHWND() ) ) != NULL )
								{
									lRet = _ImmGetCompositionStringW( hImc, GCS_COMPSTR, wszCompStr, sizeof( wszCompStr ) );
									GetNextIMEEditBox()->RequestFocus();
									if( lRet > 0 )
									{
										lRet /= sizeof(WCHAR);
										wszCompStr[lRet] = 0;  // Force terminate
										WCHAR compWCHAR = wszCompStr[lRet - 1];
										s_CompString.SetText( wszCompStr );
									}
								}

								return false;
							}
						}
					}
					ResetCaretBlink();

					if( m_CustomMsgEditBoxChange != -1 )
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
					else
						SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
				}
			}
			return true;
		}
	}
	return false;
}



//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::RenderCandidateReadingWindow( IDirect3DDevice9* pd3dDevice, float fElapsedTime, bool bReading )
{
	RECT rc;
	//09. 05. 13 김정협 Reading출력 범위 조정 
	UINT nNumEntries = bReading ? MAX_READING_COUNT : MAX_CANDLIST;
	D3DCOLOR TextColor, TextBkColor, SelTextColor, SelBkColor;
	int nX, nXFirst, nXComp;
	m_Buffer.CPtoX( m_nCaret, FALSE, &nX );
	m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nXFirst );

	if( bReading )
	{
		TextColor = m_ReadingColor;
		TextBkColor = m_ReadingWinColor;
		SelTextColor = m_ReadingSelColor;
		SelBkColor = m_ReadingSelBkColor;
	} else
	{
		TextColor = m_CandidateColor;
		TextBkColor = m_CandidateWinColor;
		SelTextColor = m_CandidateSelColor;
		SelBkColor = m_CandidateSelBkColor;
	}

	// For Japanese IME, align the window with the first target converted character.
	// For all other IMEs, align with the caret.  This is because the caret
	// does not move for Japanese IME.
	if ( GetLanguage() == LANG_CHT && !GetImeId() )
		nXComp = 0;
	else
		if( GetPrimaryLanguage() == LANG_JAPANESE )
			s_CompString.CPtoX( s_nFirstTargetConv, FALSE, &nXComp );
		else
			s_CompString.CPtoX( s_nCompCaret, FALSE, &nXComp );

	// Compute the size of the candidate window
	int nWidthRequired = 0;
	int nHeightRequired = 0;
	int nSingleLineHeight = 0;

	if( ( s_bVerticalCand && !bReading ) ||
		( !s_bHorizontalReading && bReading ) )
	{
		// Vertical window
		for( UINT i = 0; i < nNumEntries; ++i )
		{
			if( s_CandList.awszCandidate[i][0] == L'\0' )
				break;
			SetRect( &rc, 0, 0, 0, 0 );
			CalcTextRect( s_CandList.awszCandidate[i], &rc );
			nWidthRequired = __max( nWidthRequired, rc.right - rc.left );
			nSingleLineHeight = __max( nSingleLineHeight, rc.bottom - rc.top );
		}
		nHeightRequired = nSingleLineHeight * nNumEntries;
	} else
	{
		// Horizontal window
		SetRect( &rc, 0, 0, 0, 0 );
		if( bReading )
			CalcTextRect( s_wszReadingString, &rc );
		else
			CalcTextRect( s_CandList.HoriCand.GetBuffer(), &rc );
		nWidthRequired = rc.right - rc.left;
		nSingleLineHeight = nHeightRequired = rc.bottom - rc.top;
	}

	// Now that we have the dimension, calculate the location for the candidate window.
	// We attempt to fit the window in this order:
	// bottom, top, right, left.

	bool bHasPosition = false;

	// Bottom
	//SetRect( &rc, s_ptCompString.x + nXComp, s_ptCompString.y + m_rcText.bottom - m_rcText.top,
	//	s_ptCompString.x + nXComp + nWidthRequired, s_ptCompString.y + m_rcText.bottom - m_rcText.top + nHeightRequired );
	//Candidate 위치 09. 05. 12 김정협
	if( GetPrimaryLanguage() == LANG_CHINESE )
	{
		//중국계통은 대화창 위쪽으로 생성
		SetRect( &rc, s_ptCompString.x + nXComp, s_ptCompString.y - nHeightRequired,
			s_ptCompString.x + nXComp + nWidthRequired, s_ptCompString.y  );
	}
	else
	{
		//대화창 밑으로
		SetRect( &rc, s_ptCompString.x + nXComp, s_ptCompString.y + m_rcText.bottom - m_rcText.top,
			s_ptCompString.x + nXComp + nWidthRequired, s_ptCompString.y + m_rcText.bottom - m_rcText.top + nHeightRequired );
	}
	// if the right edge is cut off, move it left.
	if( rc.right > (LONG)g_pKTDXApp->GetViewport().Width )
	{
		rc.left -= rc.right - (LONG)g_pKTDXApp->GetViewport().Width;
		rc.right = (LONG)g_pKTDXApp->GetViewport().Width;
	}
	if( rc.bottom <= (LONG)g_pKTDXApp->GetViewport().Height )
		bHasPosition = true;

	// Top
	if( !bHasPosition )
	{
		SetRect( &rc, s_ptCompString.x + nXComp, s_ptCompString.y - nHeightRequired,
			s_ptCompString.x + nXComp + nWidthRequired, s_ptCompString.y );
		// if the right edge is cut off, move it left.
		if( rc.right > (LONG)g_pKTDXApp->GetViewport().Width )
		{
			rc.left -= rc.right - (LONG)g_pKTDXApp->GetViewport().Width;
			rc.right = (LONG)g_pKTDXApp->GetViewport().Width;
		}
		if( rc.top >= 0 )
			bHasPosition = true;
	}

	// Right
	if( !bHasPosition )
	{
		int nXCompTrail;
		s_CompString.CPtoX( s_nCompCaret, TRUE, &nXCompTrail );
		SetRect( &rc, s_ptCompString.x + nXCompTrail, 0,
			s_ptCompString.x + nXCompTrail + nWidthRequired, nHeightRequired );
		if( rc.right <= (LONG)g_pKTDXApp->GetViewport().Width )
			bHasPosition = true;
	}

	// Left
	if( !bHasPosition )
	{
		SetRect( &rc, s_ptCompString.x + nXComp - nWidthRequired, 0,
			s_ptCompString.x + nXComp, nHeightRequired );
		if( rc.right >= 0 )
			bHasPosition = true;
	}

	if( !bHasPosition )
	{
		// The dialog is too small for the candidate window.
		// Fall back to render at 0, 0.  Some part of the window
		// will be cut off.
		rc.left = 0;
		rc.right = nWidthRequired;
	}

	// If we are rendering the candidate window, save the position
	// so that mouse clicks are checked properly.
	if( !bReading )
		s_CandList.rcCandidate = rc;

	// Render the elements
	DrawRect( &rc, TextBkColor );
	if( ( s_bVerticalCand && !bReading ) ||
		( !s_bHorizontalReading && bReading ) )
	{
		// Vertical candidate window
		for( UINT i = 0; i < nNumEntries; ++i )
		{
			// Here we are rendering one line at a time
			rc.bottom = rc.top + nSingleLineHeight;
			// Use a different color for the selected string
			if( s_CandList.dwSelection == i )
			{
				DrawRect( &rc, SelBkColor );
				m_NowFontColor = SelTextColor;
			} else
				m_NowFontColor = TextColor;

			DrawText( s_CandList.awszCandidate[i], &rc );

			rc.top += nSingleLineHeight;
		}
	} else
	{
		// Horizontal candidate window
		m_NowFontColor = TextColor;
		if( bReading )
			DrawText( s_wszReadingString, &rc );
		else
			DrawText( s_CandList.HoriCand.GetBuffer(), &rc );

		// Render the selected entry differently
		if( !bReading )
		{
			int nXLeft, nXRight;
			s_CandList.HoriCand.CPtoX( s_CandList.nFirstSelected, FALSE, &nXLeft );
			s_CandList.HoriCand.CPtoX( s_CandList.nFirstSelected + s_CandList.nHoriSelectedLen, FALSE, &nXRight );

			rc.right = rc.left + nXRight;
			rc.left += nXLeft;
			DrawRect( &rc, SelBkColor );
			m_NowFontColor = SelTextColor;
			DrawText( s_CandList.HoriCand.GetBuffer() + s_CandList.nFirstSelected,
				&rc, s_CandList.nHoriSelectedLen );
		}
	}
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::RenderComposition( IDirect3DDevice9* pd3dDevice, float fElapsedTime )
{
	RECT rcCaret = { 0, 0, 0, 0 };
	int nX = 0, nXFirst = 0;
	int nY = 0, nYFirst = 0;
	bool bComCatEndCheck = false;
	bool bComCatEndCheckInLoop = false;
	RECT rcTemp;

	m_Buffer.CPtoX( m_nCaret, FALSE, &nX );
	m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nXFirst );

	nY = nYFirst = 0;
	if(GetGroupID() >= 0)
	{
		m_Buffer.CPtoY( m_nCaret, FALSE, &nY );
		m_Buffer.CPtoY( m_nFirstVisible, FALSE, &nYFirst );
	}


	// Get the required width
	RECT rc = { m_rcText.left + nX - nXFirst, m_rcText.top,
		m_rcText.left + nX - nXFirst, m_rcText.bottom };

	if(GetGroupID() >= 0)
	{
		rc.left = m_rcText.left + nX - nXFirst;
		rc.top = m_rcText.top + nY - nYFirst;
		rc.right = m_rcText.left + nX - nXFirst;
		rc.bottom = m_rcText.top + nY - nYFirst + m_pFont->GetHeight();
	}

	CalcTextRect( s_CompString.GetBuffer(), &rc );

	// If the composition string is too long to fit within
	// the text area, move it to below the current line.
	// This matches the behavior of the default IME.

	if( rc.right > m_rcText.right )
	{
		OffsetRect( &rc, m_rcText.left - rc.left, rc.bottom - rc.top );
		bComCatEndCheck = true;
	}


	// Save the rectangle position for processing highlighted text.
	RECT rcFirst = rc;

	// Update s_ptCompString for RenderCandidateReadingWindow().
	s_ptCompString.x = rc.left; s_ptCompString.y = rc.top;

	D3DCOLOR TextColor = m_CompColor;
	// Render the window and string.
	// If the string is too long, we must wrap the line.
	m_NowFontColor = TextColor;
	const WCHAR *pwszComp = s_CompString.GetBuffer();
	int nCharLeft = s_CompString.GetTextSize();

	for( ; ; )
	{
		// Find the last character that can be drawn on the same line.
		int nLastInLine;
		int bTrail;
		s_CompString.XtoCP( m_rcText.right - rc.left, &nLastInLine, &bTrail );
		int nNumCharToDraw = __min( nCharLeft, nLastInLine );

		if(GetGroupID() < 0)
			CalcTextRect( pwszComp, &rc, nNumCharToDraw, DT_VCENTER | DT_CALCRECT );
		else
			CalcTextRect( pwszComp, &rc, nNumCharToDraw, DT_LEFT | DT_CALCRECT );

		rcTemp = rc;
		if( bComCatEndCheck == true )
			SetRect( &rcTemp, m_rcText.right - (rc.right-rc.left), rc.top - (rc.bottom-rc.top), m_rcText.right, rc.top );

		// Draw the background
		// For Korean IME, blink the composition window background as if it
		// is a cursor.
		// 조합중인 한글 배경캐럿 그리기
		if( GetPrimaryLanguage() == LANG_KOREAN )
		{
			if( m_bCaretOn )
			{
				DrawRect( &rcTemp, m_CompWinColor );
			}
			else
			{
				// Not drawing composition string background. We
				// use the editbox's text color for composition
				// string text.
				TextColor = m_NowFontColor;
			}
		} else
		{
			// Non-Korean IME. Always draw composition background.
			DrawRect( &rcTemp, m_CompWinColor );
		}

		// Draw the text
		//pElement->FontColor.Current = TextColor;

		RECT	rcTempText = rcTemp;
		rcTempText.left -= (LONG)m_OffsetPos.x;
		rcTempText.right -= (LONG)m_OffsetPos.x;
		rcTempText.top -= (LONG)m_OffsetPos.y;
		rcTempText.bottom -= (LONG)m_OffsetPos.y;

		if(GetGroupID() < 0)
			DrawText( pwszComp, &rcTempText, nNumCharToDraw );
		else
			DrawText( pwszComp, &rcTempText, nNumCharToDraw, DT_LEFT );

		// Advance pointer and counter
		nCharLeft	-= nNumCharToDraw;
		pwszComp	+= nNumCharToDraw;
		if( nCharLeft <= 0 )
			break;

		// Advance rectangle coordinates to beginning of next line
		OffsetRect( &rc, m_rcText.left - rc.left, rc.bottom - rc.top );
		bComCatEndCheck = true;
		bComCatEndCheckInLoop = true;
	}

	// Load the rect for the first line again.
	rc = rcFirst;
	rcTemp = rc;

	if( bComCatEndCheck == true && bComCatEndCheckInLoop == false )
	{
		SetRect( &rcTemp, m_rcText.right - (rc.right-rc.left), rc.top - (rc.bottom-rc.top), m_rcText.right, rc.top );

	}

	// Inspect each character in the comp string.
	// For target-converted and target-non-converted characters,
	// we display a different background color so they appear highlighted.
	int nCharFirst = 0;
	nXFirst = 0;
	s_nFirstTargetConv = -1;
	BYTE *pAttr;
	const WCHAR *pcComp;
	for( pcComp = s_CompString.GetBuffer(), pAttr = s_abCompStringAttr;
		*pcComp != L'\0'; ++pcComp, ++pAttr )
	{
		D3DCOLOR bkColor;

		// Render a different background for this character
		int nXLeft, nXRight;
		s_CompString.CPtoX( int(pcComp - s_CompString.GetBuffer()), FALSE, &nXLeft );
		s_CompString.CPtoX( int(pcComp - s_CompString.GetBuffer()), TRUE, &nXRight );

		// Check if this character is off the right edge and should
		// be wrapped to the next line.
		if( nXRight - nXFirst > m_rcText.right - rc.left )
		{
			// Advance rectangle coordinates to beginning of next line
			OffsetRect( &rc, m_rcText.left - rc.left, rc.bottom - rc.top );

			// Update the line's first character information
			nCharFirst = int(pcComp - s_CompString.GetBuffer());
			s_CompString.CPtoX( nCharFirst, FALSE, &nXFirst );
		}

		// If the caret is on this character, save the coordinates
		// for drawing the caret later.
		if( s_nCompCaret == int(pcComp - s_CompString.GetBuffer()) )
		{
			rcCaret = rcTemp;
			rcCaret.left += nXLeft - nXFirst - 1;
			rcCaret.right = rcCaret.left + 2;
		}

		// Set up color based on the character attribute
		if( *pAttr == ATTR_TARGET_CONVERTED )
		{
			m_NowFontColor	= m_CompTargetColor;
			bkColor			= m_CompTargetBkColor;
		}
		else
			if( *pAttr == ATTR_TARGET_NOTCONVERTED )
			{
				m_NowFontColor	= m_CompTargetNonColor;
				bkColor			= m_CompTargetNonBkColor;
			}
			else
			{
				continue;
			}

			RECT rcTarget = { rc.left + nXLeft - nXFirst, rc.top, rc.left + nXRight - nXFirst, rc.bottom };
			DrawRect( &rcTarget, bkColor );

			RECT rcTempText	= rcTarget;

			rcTempText.left		-= (LONG)m_OffsetPos.x;
			rcTempText.right	-= (LONG)m_OffsetPos.x;
			rcTempText.top		-= (LONG)m_OffsetPos.y;
			rcTempText.bottom	-= (LONG)m_OffsetPos.y;

			DrawText( pcComp, &rcTempText, 1 );

			// Record the first target converted character's index
			if( -1 == s_nFirstTargetConv )
				s_nFirstTargetConv = int(pAttr - s_abCompStringAttr);
	}

	// Render the composition caret
	if( m_bCaretOn )
	{
		// If the caret is at the very end, its position would not have
		// been computed in the above loop.  We compute it here.
		if( s_nCompCaret == s_CompString.GetTextSize() )
		{
			s_CompString.CPtoX( s_nCompCaret, FALSE, &nX );

			rcCaret			 = rcTemp;
			rcCaret.left	+= nX - nXFirst - 1;
			rcCaret.right	 = rcCaret.left + 2;
		}

		DrawRect( &rcCaret, m_CompCaretColor );
	}
}

HRESULT CKTDGUIIMEEditBox::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

	CKTDGUIControl::OnFrameMove( fTime, fElapsedTime );

	m_fElapsedTime = fElapsedTime;

	if( m_bBGCheck == true )
	{
		if( m_bEnable == false )
		{
			m_EndPoint = *m_pDisablePoint;
			m_NowPoint = *m_pDisablePoint;
		}
		else
		{
			m_EndPoint = *m_pNormalPoint;
			m_NowPoint = *m_pNormalPoint;
		}
	}
	else
	{
		m_NowPoint = *m_pTextPoint;
	}

	m_rcText.left	= (LONG)(m_pTextPoint->leftTopPoint.x) + (LONG)m_OffsetPos.x;
	m_rcText.top	= (LONG)(m_pTextPoint->leftTopPoint.y) + (LONG)m_OffsetPos.y ;
	m_rcText.right	= (LONG)(m_pTextPoint->rightBottomPoint.x) + (LONG)m_OffsetPos.x;
	m_rcText.bottom	= (LONG)(m_pTextPoint->rightBottomPoint.y) + (LONG)m_OffsetPos.y;

#ifdef SERV_KEY_MAPPING_INT
	//{{10.05.25 정협 : 패드로도 채팅창 열고 닫게
	if(GET_KEY_STATE(GA_RETURN) == TRUE)
	{
		//int i = GAMEACTION_RETURN;
		unsigned char ucUserInput;
		if(g_pKTDXApp->GetDIManager()->ListenDeviceInput(ucUserInput, JOYSTICK) == true)
		{		
			if(GetGroupID() < 0)
			{
				g_pKTDXApp->GetDIManager()->GetJoystic()->SetExtraEnter(TRUE);					

				if( m_CustomMsgEditBoxEnter != -1 )
				{
					//{{09.03.27 태완 : IME 엔터와 다이얼로그 엔터가 한 번에 중복처리되지 않게 하기 위해서 락을 건다
					//g_pKTDXApp->GetDIManager()->GetJoystic()->SetLock( DIK_RETURN, TRUE );
					//					SET_KEYLOCK(GA_RETURN, TRUE);
					//}}
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxEnter, (LPARAM)this );
				}
				else
					SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_ENTER, (LPARAM)this );

			}
		}
	}
#endif // SERV_KEY_MAPPING_INT
	//}}10.05.25 정협 : 패드로도 채팅창 열고 닫게

	UpdateTextAlignOffset();

	return S_OK;
}

void CKTDGUIIMEEditBox::SetEnable( bool bEnable )
{
	m_bEnable = bEnable;
}


//--------------------------------------------------------------------------------------
HRESULT CKTDGUIIMEEditBox::OnFrameRender()
{
	KTDXPROFILE();
	if( m_bShow == false )
		return S_OK;

#ifdef DLL_BUILD
	DrawEditEdge();
#endif
	// Let the parent render first (edit control)
	// EditBox Render
	{
		HRESULT hr;
		int nSelStartX = 0, nCaretX = 0;  // Left and right X cordinates of the selection region
		int nCaretY = 0;        

		m_Buffer.SetFontNode( m_pFont );
		PlaceCaret( m_nCaret );  // Call PlaceCaret now that we have the font info (node),



		// Render the control graphics(배경 출력)
		if( m_bBGCheck == true )
		{
			UpdateVertex( m_Vertex[CKTDGUIControl::VP_LEFT_TOP], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_TOP], 
				m_Vertex[CKTDGUIControl::VP_LEFT_BOTTOM], 
				m_Vertex[CKTDGUIControl::VP_RIGHT_BOTTOM],
				m_NowPoint );

			RenderVertex( m_Vertex, m_NowPoint );
		}

		//
		// Compute the X coordinates of the first visible character.
		//
		int nXFirst = 0, nYFirst = 0;
		m_Buffer.CPtoX( m_nFirstVisible, FALSE, &nXFirst );

		//
		// Compute the X coordinates of the selection rectangle
		//
		hr = m_Buffer.CPtoX( m_nCaret, FALSE, &nCaretX );
		if(GetGroupID() >= 0)
		{
			hr = m_Buffer.CPtoY( m_nCaret, FALSE, &nCaretY );            
			m_Buffer.CPtoY( m_nFirstVisible, FALSE, &nYFirst );            
		}

		if( m_nCaret != m_nSelStart )
			hr = m_Buffer.CPtoX( m_nSelStart, FALSE, &nSelStartX );
		else
			nSelStartX = nCaretX;



		//
		// Render the selection rectangle
		//
		RECT rcSelection;  // Make this available for rendering selected text
		if( m_nCaret != m_nSelStart )
		{
			int nSelLeftX = nCaretX, nSelRightX = nSelStartX;

			// Swap if left is bigger than right
			if( nSelLeftX > nSelRightX )
			{
				int nTemp	= nSelLeftX;
				nSelLeftX	= nSelRightX;
				nSelRightX	= nTemp; 
			}

			if(GetGroupID() >= 0)
				SetRect( &rcSelection, nSelLeftX, m_rcText.top + nCaretY, nSelRightX, m_rcText.top + nCaretY + m_pFont->GetHeight() );                
			else
				SetRect( &rcSelection, nSelLeftX, m_rcText.top, nSelRightX, m_rcText.bottom );

			OffsetRect( &rcSelection, m_rcText.left - nXFirst, 0 );
			IntersectRect( &rcSelection, &m_rcText, &rcSelection );
			DrawRect( &rcSelection, m_SelBkColor );
		}

		//
		// Render the text
		//
		m_NowFontColor	= m_TextColor;

		RECT rcTempText;
		rcTempText			 = m_rcText;
		rcTempText.left		-= (LONG)m_OffsetPos.x;
		rcTempText.right	-= (LONG)m_OffsetPos.x;
		rcTempText.top		-= (LONG)m_OffsetPos.y;
		rcTempText.bottom	-= (LONG)m_OffsetPos.y;

		if(GetGroupID() < 0)
			DrawText( m_Buffer.GetBuffer() + m_nFirstVisible, &rcTempText );
		else
			DrawText( m_Buffer.GetBuffer() + m_nFirstVisible, &rcTempText, -1, DT_LEFT ); // | DT_NOPREFIX  | DT_WORDBREAK | DT_EDITCONTROL);



		// Render the selected text
		if( m_nCaret != m_nSelStart )
		{
			RECT rcTempSelection = rcSelection;

			rcTempSelection.left	+= (LONG) (-m_OffsetPos.x + m_ptTextAlignOffset.x);
			rcTempSelection.right	+= (LONG) (-m_OffsetPos.x + m_ptTextAlignOffset.x);
			rcTempSelection.top		+= (LONG) (-m_OffsetPos.y + m_ptTextAlignOffset.y);
			rcTempSelection.bottom	+= (LONG) (-m_OffsetPos.y + m_ptTextAlignOffset.y);

			int nFirstToRender		= __max( m_nFirstVisible, __min( m_nSelStart, m_nCaret ) );
			int nNumChatToRender	= __max( m_nSelStart, m_nCaret ) - nFirstToRender;

			m_NowFontColor = m_SelTextColor;

			if(GetGroupID() < 0)
				DrawText( m_Buffer.GetBuffer() + nFirstToRender, &rcTempSelection, nNumChatToRender );            
			else
				DrawText( m_Buffer.GetBuffer() + nFirstToRender, &rcTempSelection, nNumChatToRender, DT_LEFT );

		}

		//
		// Blink the caret
		//
		if( DXUTGetGlobalTimer()->GetAbsoluteTime() - m_dfLastBlink >= m_dfBlink )
		{
			m_bCaretOn = !m_bCaretOn;
			m_dfLastBlink = DXUTGetGlobalTimer()->GetAbsoluteTime();
		}	

		//
		// Render the caret if this control has the focus
		//


		if( m_bHasFocus && m_bCaretOn && !s_bHideCaret )
		{
			// Start the rectangle with insert mode caret
			RECT rcCaret = { m_rcText.left - nXFirst + nCaretX - 1, m_rcText.top,
				m_rcText.left - nXFirst + nCaretX + 1, m_rcText.bottom };

			if(GetGroupID() >= 0)
			{                
				rcCaret.left = m_rcText.left - nXFirst + nCaretX - 1;
				rcCaret.top = m_rcText.top + nCaretY - nYFirst;
				rcCaret.right = m_rcText.left - nXFirst + nCaretX + 1;
				rcCaret.bottom = m_rcText.top + nCaretY - nYFirst + m_pFont->GetHeight();
			}


			// If we are in overwrite mode, adjust the caret rectangle
			// to fill the entire character.
			if( !m_bInsertMode )
			{
				// Obtain the right edge X coord of the current character
				int nRightEdgeX;
				m_Buffer.CPtoX( m_nCaret, TRUE, &nRightEdgeX );
				rcCaret.right = m_rcText.left - nXFirst + nRightEdgeX;
			}

			DrawRect( &rcCaret, m_CaretColor );
		}

	}

	s_CompString.SetFontNode( m_pFont );
	s_CandList.HoriCand.SetFontNode( m_pFont );

	//
	// Now render the IME elements
	//

	if( m_bHasFocus )
	{
		// Display the composition string.
		// This method should also update s_ptCompString
		// for RenderCandidateReadingWindow.
		RenderComposition( g_pKTDXApp->GetDevice(), m_fElapsedTime );

		// Display the reading/candidate window. RenderCandidateReadingWindow()
		// uses s_ptCompString to position itself.  s_ptCompString must have
		// been filled in by RenderComposition().
		if( s_bShowReadingWindow )
		{
			// Reading window
			RenderCandidateReadingWindow( g_pKTDXApp->GetDevice(), m_fElapsedTime, true );
		}
		else
		{
			// Candidate list window
			if( s_CandList.bShowWindow )
				RenderCandidateReadingWindow( g_pKTDXApp->GetDevice(), m_fElapsedTime, false );
		}
	}

	return S_OK;
}

void CKTDGUIIMEEditBox::SetNormalTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pNormalTex );

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pNormalTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pNormalPoint != NULL )
		m_pNormalPoint->pUITextureData = m_pNormalTex;

	m_bBGCheck = true;
}

void CKTDGUIIMEEditBox::SetDisableTex_LUA( const char* pFileName, const char* key )
{
	SAFE_DELETE( m_pDisableTex );

	wstring wstrFileName;
	ConvertUtf8ToWCHAR( wstrFileName, pFileName );

	wstring wstrKeyName;
	ConvertUtf8ToWCHAR( wstrKeyName, key );

	m_pDisableTex = SetTexureData_( wstrFileName.c_str(), wstrKeyName.c_str() );
	if( m_pDisablePoint != NULL )
		m_pDisablePoint->pUITextureData = m_pDisableTex;

	m_bBGCheck = true;
}

#ifdef LOGIN_KEYBOARD_SECURITY
void CKTDGUIIMEEditBox::ProcessBackspace()
{
	PlaySound( m_pSndKeyDown );

	// If there's a selection, treat this
	// like a delete key.
	if( m_nCaret != m_nSelStart )
	{
		DeleteSelectionText();

		if( m_CustomMsgEditBoxChange != -1 )
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
		else
			SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
	}
	else
		if( m_nCaret > 0 )
		{
			// Move the caret, then delete the char.
			PlaceCaret( m_nCaret - 1 );
			m_nSelStart = m_nCaret;
			m_Buffer.RemoveChar( m_nCaret );

			if( m_CustomMsgEditBoxChange != -1 )
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_CUSTOM_EVENT, m_CustomMsgEditBoxChange, (LPARAM)this );
			else
				SendInternelEvent( g_pKTDXApp->GetHWND(), CKTDXApp::KM_UI_CONTROL_EVENT, IEEM_CHANGE, (LPARAM)this );
		}
		ResetCaretBlink();
}
#endif LOGIN_KEYBOARD_SECURITY

void CKTDGUIIMEEditBox::SetNormalPoint_LUA()
{
	SAFE_DELETE( m_pNormalPoint );

	m_pNormalPoint = SetPointData_();

	if( m_pNormalTex != NULL )
	{
		m_pNormalPoint->pUITextureData = m_pNormalTex;
		m_pNormalPoint->SetAutoPointByTextureSize();
	}

	m_bBGCheck = true;
}

void CKTDGUIIMEEditBox::SetDisablePoint_LUA()
{
	SAFE_DELETE( m_pDisablePoint );

	m_pDisablePoint = SetPointData_();

	if( m_pDisableTex != NULL )
	{
		m_pDisablePoint->pUITextureData = m_pDisableTex;
		m_pDisablePoint->SetAutoPointByTextureSize();
	}

	m_bBGCheck = true;
}

void CKTDGUIIMEEditBox::SetTextPoint_LUA()
{
	SAFE_DELETE( m_pTextPoint );
	m_pTextPoint = SetPointData_();

#ifdef FIX_IME_EDITBOX_LOSE_FIRST_CHAR
	m_rcText.left	= (LONG)(m_pTextPoint->leftTopPoint.x);
	m_rcText.top	= (LONG)(m_pTextPoint->leftTopPoint.y);
	m_rcText.right	= (LONG)(m_pTextPoint->rightBottomPoint.x);
	m_rcText.bottom	= (LONG)(m_pTextPoint->rightBottomPoint.y);
#endif //FIX_IME_EDITBOX_LOSE_FIRST_CHAR
}

void CKTDGUIIMEEditBox::SetFont_LUA( int nFontIndex )
{
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetMappedUKFont( nFontIndex );
#else
	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont( nFontIndex );
#endif
}

//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::ResetCaretBlink()
{
	m_bCaretOn = true;
	m_dfLastBlink = DXUTGetGlobalTimer()->GetAbsoluteTime();
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::Initialize()
{
	if( s_hDllImm32 ) // Only need to do once
		return;

	FARPROC Temp;

	s_CompString.SetBufferSize( MAX_COMPSTRING_SIZE );

	WCHAR wszPath[MAX_PATH+1];
	if( !::GetSystemDirectory( wszPath, MAX_PATH+1 ) )
		return;

	StringCchCat( wszPath, MAX_PATH, IMM32_DLLNAME );
	s_hDllImm32 = LoadLibrary( wszPath );

	if( s_hDllImm32 )
	{
		GETPROCADDRESS( s_hDllImm32, ImmLockIMC, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmUnlockIMC, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmLockIMCC, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmUnlockIMCC, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmDisableTextFrameService, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmGetCompositionStringW, Temp );		//조합중인 유니코드 얻어오기함수 Ex."ㅎ 하 한 ㄱ 그 글" => "한글"
		GETPROCADDRESS( s_hDllImm32, ImmGetCandidateListW, Temp );			//특수문자표 얻어오기(arg 2: N_OPENCANDIDATE <= 리스트 열기 플레그)
		GETPROCADDRESS( s_hDllImm32, ImmGetContext, Temp );					//현재 모드(한/영..) 상태 검사
		GETPROCADDRESS( s_hDllImm32, ImmReleaseContext, Temp );				//입력 해제
		GETPROCADDRESS( s_hDllImm32, ImmAssociateContext, Temp );			//IME On/Off Function
		GETPROCADDRESS( s_hDllImm32, ImmGetOpenStatus, Temp );				//determines whether the IME is open or closed.
		GETPROCADDRESS( s_hDllImm32, ImmSetOpenStatus, Temp );				//opens or closes the IME.
		GETPROCADDRESS( s_hDllImm32, ImmGetConversionStatus, Temp );		//현제 설정도니 모드 얻어오기 Ex. IME_CMODE_HANGEUL등..
		GETPROCADDRESS( s_hDllImm32, ImmGetDefaultIMEWnd, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmGetIMEFileNameA, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmGetVirtualKey, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmNotifyIME, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmSetConversionStatus, Temp );		//언어모드 셋팅하기
		GETPROCADDRESS( s_hDllImm32, ImmSimulateHotKey, Temp );
		GETPROCADDRESS( s_hDllImm32, ImmIsIME, Temp );						//IME 입력모드가 열렸는지 체크
	}

	if( !::GetSystemDirectory( wszPath, MAX_PATH+1 ) )
		return;

	StringCchCat( wszPath, MAX_PATH, VER_DLLNAME );
	s_hDllVer = LoadLibrary( wszPath );

	if( s_hDllVer )
	{
		GETPROCADDRESS( s_hDllVer, VerQueryValueA, Temp );
		GETPROCADDRESS( s_hDllVer, GetFileVersionInfoA, Temp );
		GETPROCADDRESS( s_hDllVer, GetFileVersionInfoSizeA, Temp );
	}
}


//--------------------------------------------------------------------------------------
void CKTDGUIIMEEditBox::Uninitialize()
{
	if( s_hDllImm32 )
	{
		PLACEHOLDERPROC( ImmLockIMC );
		PLACEHOLDERPROC( ImmUnlockIMC );
		PLACEHOLDERPROC( ImmLockIMCC );
		PLACEHOLDERPROC( ImmUnlockIMCC );
		PLACEHOLDERPROC( ImmDisableTextFrameService );
		PLACEHOLDERPROC( ImmGetCompositionStringW );
		PLACEHOLDERPROC( ImmGetCandidateListW );
		PLACEHOLDERPROC( ImmGetContext );
		PLACEHOLDERPROC( ImmReleaseContext );
		PLACEHOLDERPROC( ImmAssociateContext );
		PLACEHOLDERPROC( ImmGetOpenStatus );
		PLACEHOLDERPROC( ImmSetOpenStatus );
		PLACEHOLDERPROC( ImmGetConversionStatus );
		PLACEHOLDERPROC( ImmGetDefaultIMEWnd );
		PLACEHOLDERPROC( ImmGetIMEFileNameA );
		PLACEHOLDERPROC( ImmGetVirtualKey );
		PLACEHOLDERPROC( ImmNotifyIME );
		PLACEHOLDERPROC( ImmSetConversionStatus );
		PLACEHOLDERPROC( ImmSimulateHotKey );
		PLACEHOLDERPROC( ImmIsIME );

		FreeLibrary( s_hDllImm32 );
		s_hDllImm32 = NULL;
	}
	if( s_hDllIme )
	{
		PLACEHOLDERPROC( GetReadingString );
		PLACEHOLDERPROC( ShowReadingWindow );

		FreeLibrary( s_hDllIme );
		s_hDllIme = NULL;
	}
	if( s_hDllVer )
	{
		PLACEHOLDERPROC( VerQueryValueA );
		PLACEHOLDERPROC( GetFileVersionInfoA );
		PLACEHOLDERPROC( GetFileVersionInfoSizeA );

		FreeLibrary( s_hDllVer );
		s_hDllVer = NULL;
	}
}

//--------------------------------------------------------------------------------------
HRESULT CKTDGUIIMEEditBox::DrawText( LPCWSTR strText, RECT* prcDest, int nCount /* = -1 */, DWORD format /* = DT_LEFT | DT_VCENTER  */)
{
	if ( m_DefTextAlign != -1 && CheckTextOversize() == false )
		format = m_DefTextAlign;

	RECT rcScreen = *prcDest;

	OffsetRect( &rcScreen,
		(LONG)(m_pDialog->GetPos().x + m_OffsetPos.x),
		(LONG)(m_pDialog->GetPos().y + m_OffsetPos.y) );

	if( m_bOutlineCheck == true )
	{
		m_pFont->DrawTextFont( strText, &rcScreen, nCount, m_NowFontColor, true, m_OutlineColor, format );
	}
	else
	{
		m_pFont->DrawTextFont( strText, &rcScreen, nCount, m_NowFontColor, false, m_OutlineColor, format );
	}

	return S_OK;
}

HRESULT CKTDGUIIMEEditBox::CalcTextRect( LPCWSTR strText, RECT* prcDest, int nCount /* = -1 */, DWORD format /* = (DT_VCENTER | DT_CALCRECT */)
{
	if ( m_DefTextAlign != -1 && CheckTextOversize() == false )
		format = m_DefTextAlign;

	m_pFont->CalcTextRect( strText, prcDest, nCount, format );

	return S_OK;
}

bool CKTDGUIIMEEditBox::CheckTextOversize()
{
	int iLen;
	m_Buffer.CPtoX( m_Buffer.GetTextSize(), FALSE, &iLen );
	if ( iLen > m_rcText.right - m_rcText.left )
	{			
		return true;
	}

	return false;
}

HRESULT CKTDGUIIMEEditBox::DrawRect( RECT* prcDest, D3DCOLOR color )
{
	RECT rcScreen = *prcDest;

	// 텍스트 정렬
	rcScreen.left		+= m_ptTextAlignOffset.x;
	rcScreen.right		+= m_ptTextAlignOffset.x;
	rcScreen.top		+= m_ptTextAlignOffset.y;
	rcScreen.bottom		+= m_ptTextAlignOffset.y;

// 오현빈 // 2013-12-06 // 좌/우 해상도 늘어났을 때, 커서 텍스쳐때문에 반각 처럼 보이는 문제 수정
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
	long lScreenRight = static_cast<long>((rcScreen.right + m_pDialog->GetPos().x) - (rcScreen.left + m_pDialog->GetPos().x));
	long lScreenBottom = static_cast<long>((rcScreen.bottom + m_pDialog->GetPos().y) - (rcScreen.top + m_pDialog->GetPos().y));
#endif // FIX_CARET_POSITION_AND_FONT_SIZE
	
	rcScreen.left		= (LONG)(rcScreen.left * g_pKTDXApp->GetResolutionScaleX() + m_pDialog->GetPos().x * g_pKTDXApp->GetResolutionScaleX());
	rcScreen.top		= (LONG)(rcScreen.top * g_pKTDXApp->GetResolutionScaleY() + m_pDialog->GetPos().y * g_pKTDXApp->GetResolutionScaleY());
#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
	rcScreen.right		= rcScreen.left + lScreenRight;
	rcScreen.bottom		= rcScreen.top + lScreenBottom;
#else
	rcScreen.right		= (LONG)(rcScreen.right * g_pKTDXApp->GetResolutionScaleX() + m_pDialog->GetPos().x * g_pKTDXApp->GetResolutionScaleX());
	rcScreen.bottom		= (LONG)(rcScreen.bottom * g_pKTDXApp->GetResolutionScaleY() + m_pDialog->GetPos().y * g_pKTDXApp->GetResolutionScaleY());
#endif // FIX_CARET_POSITION_AND_FONT_SIZE
	
	
	CKTDGUIControl::VERTEX_UI vertices[4] =
	{
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.left	 - 0.5f, (float)rcScreen.top	-0.5f, 0.5f, 1.0f, color, 0, 0 ),
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.right - 0.5f, (float)rcScreen.top	-0.5f, 0.5f, 1.0f, color, 0, 0 ),
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.left	 - 0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 ),
		CKTDGUIControl::VERTEX_UI( (float)rcScreen.right - 0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 )
		/*
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		{ (float)rcScreen.left	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0 },
		{ (float)rcScreen.right	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0 },
		{ (float)rcScreen.left	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 },
		{ (float)rcScreen.right	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0 },
#else
		(float)rcScreen.left	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0,
		(float)rcScreen.right	-0.5f, (float)rcScreen.top		-0.5f, 0.5f, 1.0f, color, 0, 0, 
		(float)rcScreen.right	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0, 
		(float)rcScreen.left	-0.5f, (float)rcScreen.bottom	-0.5f, 0.5f, 1.0f, color, 0, 0,
#endif*/


	};
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	m_pFont->GetD3DSprite()->Begin( D3DXSPRITE_DONOTSAVESTATE | D3DXSPRITE_DONOTMODIFY_RENDERSTATE );
	m_pFont->GetD3DSprite()->Flush();

	IDirect3DVertexDeclaration9 *pDecl = NULL;
	g_pKTDXApp->GetDevice()->GetVertexDeclaration( &pDecl );
	g_pKTDXApp->GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
#endif

	KD3DBEGIN()
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2 );

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		RenderVertex( vertices, true );
#else
		g_pKTDXApp->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, vertices, sizeof(CKTDGUIControl::VERTEX_UI) );
#endif
	KD3DEND()

#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	g_pKTDXApp->GetDevice()->SetVertexDeclaration( pDecl );
	pDecl->Release();

	m_pFont->GetD3DSprite()->End();
#endif

	return S_OK;
}


bool CKTDGUIIMEEditBox::ContainPTForIME(POINT pt)
{
	D3DXVECTOR2 arPoint[4];



	arPoint[0] = m_pDialog->GetPos() + m_NowPoint.leftTopPoint + m_OffsetPos;
	arPoint[1] = m_pDialog->GetPos() + m_NowPoint.rightTopPoint + m_OffsetPos;
	arPoint[2] = m_pDialog->GetPos() + m_NowPoint.leftBottomPoint + m_OffsetPos;
	arPoint[3] = m_pDialog->GetPos() + m_NowPoint.rightBottomPoint + m_OffsetPos;

	return Pick2DRect( pt, arPoint[0], arPoint[1], arPoint[2], arPoint[3] ); 

	//return Pick2DRect( pt, m_NowPoint.leftTopPoint, m_NowPoint.rightTopPoint, m_NowPoint.leftBottomPoint ,m_NowPoint.rightBottomPoint ); 
}

CKTDGUIIMEEditBox* CKTDGUIIMEEditBox::GetNextIMEEditBox()
{
	for ( int i = 0; i < m_pDialog->GetControlSize(); i++ )
	{
		if ( m_pDialog->GetControl(i) != NULL &&
			m_pDialog->GetControl(i)->GetType() == CKTDGUIControl::UCT_IME_EDITBOX )
		{
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDialog->GetControl(i);
			if ( pIMEEditBox->GetGroupID() == GetGroupID() )
			{
				if ( pIMEEditBox->GetGroupSequence() == GetGroupSequence() + 1 )
				{
					return pIMEEditBox;
				}
			}
		}
	}

	return NULL;
}

CKTDGUIIMEEditBox* CKTDGUIIMEEditBox::GetPrevIMEEditBox()
{
	for ( int i = 0; i < m_pDialog->GetControlSize(); i++ )
	{
		if ( m_pDialog->GetControl(i) != NULL &&
			m_pDialog->GetControl(i)->GetType() == CKTDGUIControl::UCT_IME_EDITBOX )
		{
			CKTDGUIIMEEditBox* pIMEEditBox = (CKTDGUIIMEEditBox*)m_pDialog->GetControl(i);
			if ( pIMEEditBox->GetGroupID() == GetGroupID() )
			{
				if ( pIMEEditBox->GetGroupSequence() == GetGroupSequence() - 1 )
				{
					return pIMEEditBox;
				}
			}
		}
	}

	return NULL;
}

void CKTDGUIIMEEditBox::UpdateTextAlignOffset()
{
	// 중앙 정렬일 경우 처리
	if ( m_DefTextAlign != -1 && m_DefTextAlign & DT_CENTER && CheckTextOversize() == false )
	{
		int iBegin, iEnd;
		m_Buffer.CPtoX(m_Buffer.GetTextSize(), FALSE, &iEnd);
		m_Buffer.CPtoX(m_nFirstVisible, FALSE, &iBegin);

		float fCenterOffset = (m_NowPoint.rightTopPoint.x - m_NowPoint.leftTopPoint.x) * 0.5f;
		fCenterOffset -= ( iEnd - iBegin ) * 0.5f;

		m_ptTextAlignOffset.x = (int) fCenterOffset;
		m_ptTextAlignOffset.y = 0;
	}
	else
	{
		m_ptTextAlignOffset.x = 0;
		m_ptTextAlignOffset.y = 0;
	}
}

#ifdef UPGRADE_TRADE_SYSTEM_ADD_FUNCTION // 김태환
/** @function	: GetNowText
	@brief		: 현재 작성중인 모든 문자 반환
*/
const wstring CKTDGUIIMEEditBox::GetNowText()		
{ 
	/// 완성된 문자들 저장
	wstring wstrResultText = m_Buffer.GetBuffer();

	/// 만약 미완성된 문자가 있다면, 해당 문자도 저장
	if ( true == s_bHideCaret )
		wstrResultText += s_CompString.GetBuffer();

	return   wstrResultText;
}
#endif //UPGRADE_TRADE_SYSTEM_ADD_FUNCTION