#include "StdAfx.h"
#include ".\ktdguiunibuffer.h"
#include "KSuperAssert.h"
#include "..\X2Lib\X2Define.h"

#define DXUT_MAX_EDITBOXLENGTH 0xFFFF

// Static member initialization
HINSTANCE CKTDGUIUniBuffer::s_hDll = NULL;
HRESULT (WINAPI *CKTDGUIUniBuffer::_ScriptApplyDigitSubstitution)( const SCRIPT_DIGITSUBSTITUTE*, SCRIPT_CONTROL*, SCRIPT_STATE* ) = Dummy_ScriptApplyDigitSubstitution;
HRESULT (WINAPI *CKTDGUIUniBuffer::_ScriptStringAnalyse)( HDC, const void *, int, int, int, DWORD, int, SCRIPT_CONTROL*, SCRIPT_STATE*, const int*, SCRIPT_TABDEF*, const BYTE*, SCRIPT_STRING_ANALYSIS* ) = Dummy_ScriptStringAnalyse;
HRESULT (WINAPI *CKTDGUIUniBuffer::_ScriptStringCPtoX)( SCRIPT_STRING_ANALYSIS, int, BOOL, int* ) = Dummy_ScriptStringCPtoX;
HRESULT (WINAPI *CKTDGUIUniBuffer::_ScriptStringXtoCP)( SCRIPT_STRING_ANALYSIS, int, int*, int* ) = Dummy_ScriptStringXtoCP;
HRESULT (WINAPI *CKTDGUIUniBuffer::_ScriptStringFree)( SCRIPT_STRING_ANALYSIS* ) = Dummy_ScriptStringFree;
const SCRIPT_LOGATTR* (WINAPI *CKTDGUIUniBuffer::_ScriptString_pLogAttr)( SCRIPT_STRING_ANALYSIS ) = Dummy_ScriptString_pLogAttr;
const int* (WINAPI *CKTDGUIUniBuffer::_ScriptString_pcOutChars)( SCRIPT_STRING_ANALYSIS ) = Dummy_ScriptString_pcOutChars;

void CKTDGUIUniBuffer::Initialize()
{
	if( s_hDll ) // Only need to do once
		return;

	WCHAR wszPath[MAX_PATH+1];
	if( !::GetSystemDirectory( wszPath, MAX_PATH+1 ) )
		return;

	// Verify whether it is safe to concatenate these strings
	int len1 = lstrlen(wszPath);
	int len2 = lstrlen(UNISCRIBE_DLLNAME);
	if (len1 + len2 > MAX_PATH)
	{
		return;
	}

	// We have verified that the concatenated string will fit into wszPath,
	// so it is safe to concatenate them.
	StringCchCat( wszPath, MAX_PATH, UNISCRIBE_DLLNAME );

	s_hDll = LoadLibrary( wszPath );
	if( s_hDll )
	{
		FARPROC Temp;
		GETPROCADDRESS( s_hDll, ScriptApplyDigitSubstitution, Temp );
		GETPROCADDRESS( s_hDll, ScriptStringAnalyse, Temp );
		GETPROCADDRESS( s_hDll, ScriptStringCPtoX, Temp );
		GETPROCADDRESS( s_hDll, ScriptStringXtoCP, Temp );
		GETPROCADDRESS( s_hDll, ScriptStringFree, Temp );
		GETPROCADDRESS( s_hDll, ScriptString_pLogAttr, Temp );
		GETPROCADDRESS( s_hDll, ScriptString_pcOutChars, Temp );
	}
}

void CKTDGUIUniBuffer::Uninitialize()
{
	if( s_hDll )
	{
		PLACEHOLDERPROC( ScriptApplyDigitSubstitution );
		PLACEHOLDERPROC( ScriptStringAnalyse );
		PLACEHOLDERPROC( ScriptStringCPtoX );
		PLACEHOLDERPROC( ScriptStringXtoCP );
		PLACEHOLDERPROC( ScriptStringFree );
		PLACEHOLDERPROC( ScriptString_pLogAttr );
		PLACEHOLDERPROC( ScriptString_pcOutChars );

		FreeLibrary( s_hDll );
		s_hDll = NULL;
	}
}

bool CKTDGUIUniBuffer::SetBufferSize( int nNewSize )
{
	// If the current size is already the maximum allowed,
	// we can't possibly allocate more.
	if( m_nBufferSize == DXUT_MAX_EDITBOXLENGTH )
	{
		ErrorLog( KEM_ERROR39 );
		return false;
	}

	int nAllocateSize = ( nNewSize == -1 || nNewSize < m_nBufferSize * 2 ) ? ( m_nBufferSize ? m_nBufferSize * 2 : 256 ) : nNewSize * 2;

	// Cap the buffer size at the maximum allowed.
	if( nAllocateSize > DXUT_MAX_EDITBOXLENGTH )
		nAllocateSize = DXUT_MAX_EDITBOXLENGTH;

	WCHAR *pTempBuffer = new WCHAR[nAllocateSize];
	WCHAR *pTempBufferHC = new WCHAR[nAllocateSize];
	
	if( pTempBuffer == NULL || pTempBufferHC == NULL )
	{
		SAFE_DELETE( pTempBuffer );
		SAFE_DELETE( pTempBufferHC );

		ErrorLog( KEM_ERROR40 );
		return false;
	}

	if( m_pwszBuffer )
	{
		CopyMemory( pTempBuffer, m_pwszBuffer, m_nBufferSize * sizeof(WCHAR) );
		delete[] m_pwszBuffer;

		CopyMemory( pTempBufferHC, m_pwszBufferHC, m_nBufferSize * sizeof(WCHAR) );
		delete[] m_pwszBufferHC;
	}
	else
	{
		ZeroMemory( pTempBuffer, sizeof(WCHAR) * nAllocateSize );
		ZeroMemory( pTempBufferHC, sizeof(WCHAR) * nAllocateSize );
	}

	m_pwszBuffer = pTempBuffer;
	m_pwszBufferHC = pTempBufferHC;
	m_nBufferSize = nAllocateSize;
	return true;
}

#ifndef USE_FREE_TYPE
HRESULT CKTDGUIUniBuffer::Analyse()
{
	if( m_Analysis )
		_ScriptStringFree( &m_Analysis );

	SCRIPT_CONTROL ScriptControl; // For uniscribe
	SCRIPT_STATE   ScriptState;   // For uniscribe
	ZeroMemory( &ScriptControl, sizeof(ScriptControl) );
	ZeroMemory( &ScriptState, sizeof(ScriptState) );
	_ScriptApplyDigitSubstitution ( NULL, &ScriptControl, &ScriptState );

	if( m_pFontNode == NULL || m_pwszBuffer == NULL
#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		|| m_pFontNode->GetKTDGDeviceFont() == NULL)
#else
		|| m_pFontNode->GetD3DFont() == NULL)
#endif
	{
		if ( m_pwszBuffer == NULL )
		{
			//ErrorLog( KEM_ERROR41 );
		}
		else
		{
			//ErrorLogMsg( KEM_ERROR41, m_pwszBuffer );
		}
		return E_FAIL;
	}

	WCHAR* pcAnalyseString = NULL;
	if (m_bUseHideChar)
	{
		int iLen = lstrlenW(m_pwszBuffer);
		ZeroMemory( m_pwszBufferHC, sizeof(WCHAR) * m_nBufferSize );

		for (int iC = 0; iC < iLen; ++iC)
			m_pwszBufferHC[iC] = m_cHideChar;

		pcAnalyseString = m_pwszBufferHC;
	}
	else
	{
		pcAnalyseString = m_pwszBuffer;
	}

#ifdef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
	HRESULT hr = _ScriptStringAnalyse( m_pFontNode->GetDC(),
#else
	HRESULT hr = _ScriptStringAnalyse( m_pFontNode->GetD3DFont()->GetDC(),
#endif
		pcAnalyseString,
		lstrlenW( pcAnalyseString ) + 1,  // NULL is also analyzed.
		lstrlenW( pcAnalyseString ) * 3 / 2 + 16,
		-1,
		SSA_BREAK | SSA_GLYPHS | SSA_FALLBACK | SSA_LINK,
		0,
		&ScriptControl,
		&ScriptState,
		NULL,
		NULL,
		NULL,
		&m_Analysis );

	if( SUCCEEDED( hr ) )
		m_bAnalyseRequired = false;  // Analysis is up-to-date
	return hr;
}
#endif //USE_FREE_TYPE

bool CKTDGUIUniBuffer::isEnglish( WCHAR cWChar )
{
	if ( m_bCheckEnglishBigAndSmall == true )
	{
		if ( m_bBigEnglish == true )
		{
			if 	( cWChar >= 'A' && cWChar <= 'Z' ) 
			{
				return true;
			}
		}
		else
		{
			if (  cWChar >= 'a' && cWChar <= 'z' ) 
			{
				return true;
			}
		}
	}
	else
	{
		if ( ( cWChar >= 'A' && cWChar <= 'Z' ) ||
			(  cWChar >= 'a' && cWChar <= 'z' ) )
		{
			return true;
		}
	}

	

	

	return false;
}

CKTDGUIUniBuffer::CKTDGUIUniBuffer( int nInitialSize )
{
	CKTDGUIUniBuffer::Initialize();  // ensure static vars are properly init'ed first

	m_nBufferSize = 0;
	m_pwszBuffer = NULL;
	m_pwszBufferHC = NULL;
	m_bAnalyseRequired = true;
	m_Analysis = NULL;
	m_pFontNode = NULL;
	m_bUseHideChar = false;
	m_bOneCharMode = false;
	m_bOnlyNumMode = false;
	m_bOnlyNumAndEnglishMode = false;
	m_bBigEnglish = true;
	m_nLengthLimit = DXUT_MAX_EDITBOXLENGTH;
	m_WidthLimit = 99999;
    m_LineLimit = 99999;
	m_bByteLimit = false;
	m_ByteLimit = 0;
	m_bAutoChangeBigEnglish = false;

	wmemset( Defence1, 0, 100 );
	wmemset( Defence2, 0, 100 );
	m_bCheckEnglishBigAndSmall = false;
	m_cHideChar = 0;


	if( nInitialSize > 0 )
		SetBufferSize( nInitialSize );
}

CKTDGUIUniBuffer::~CKTDGUIUniBuffer()
{
	delete[] m_pwszBuffer;
	delete[] m_pwszBufferHC;
	if( m_Analysis )
		_ScriptStringFree( &m_Analysis );
}

WCHAR& CKTDGUIUniBuffer::operator[]( int n )  // No param checking
{
	// This version of operator[] is called only
	// if we are asking for write access, so
	// re-analysis is required.
	m_bAnalyseRequired = true;
	return m_pwszBuffer[n];
}

void CKTDGUIUniBuffer::Clear()
{
	*m_pwszBuffer = L'\0';
	m_bAnalyseRequired = true;
}

bool CKTDGUIUniBuffer::InsertChar( int nIndex, WCHAR wChar )
{
	assert( nIndex >= 0 );

	if( nIndex < 0 || nIndex > lstrlenW( m_pwszBuffer ) )
		return false;  // invalid index

	// Check for maximum length allowed
	if( GetTextSize() + 1 >= DXUT_MAX_EDITBOXLENGTH )
		return false;

	if ( m_bAutoChangeBigEnglish == true )
	{
		wChar = towupper( wChar );
	}

	// 숫자 입력 모드 처리
	if ( m_bOnlyNumMode == true && iswdigit( wChar ) == 0 )
		return false;

	if ( m_bOnlyNumAndEnglishMode == true )
	{
		if ( ( iswdigit( wChar ) != 0 || isEnglish( wChar ) == true ) )
		{

		}
		else
		{
			return false;
		}
		
	}

	// 한문자 모드일 경우 처리
	if ( m_bOneCharMode == true )
	{
		if( 2 >= m_nBufferSize && !SetBufferSize( -1 ) )
		{
			ErrorLogMsg( KEM_ERROR42, "out of memory" );
			return false;  // out of memory
		}

		assert( m_nBufferSize >= 2 );

		m_pwszBuffer[ 0 ] = wChar;
		m_bAnalyseRequired = true;

		return false;
	}

	if ( m_bByteLimit == true )
	{
		string tempString;
		ConvertWCHARToChar( tempString, m_pwszBuffer );

		if ( m_ByteLimit <= (int)tempString.size() )
			return false;
	}

	if( GetTextSize() >= m_nLengthLimit )
		return false;

	int width;
	//CPtoX( GetTextSize(), FALSE, &width );
    width = GetLineWidth(nIndex);
	if ( wChar != L'\n' && width >= m_WidthLimit )
    {
        InsertChar(nIndex, L'\n');
        InsertChar(nIndex+1, wChar);
        return true;
    }
	

    // Limit Height
    int nLine = GetLineCount(GetTextSize());
    if( nLine > m_LineLimit || (wChar == L'\n' && nLine >= m_LineLimit) )
        return false;

	if( lstrlenW( m_pwszBuffer ) + 1 >= m_nBufferSize )
	{
		if( !SetBufferSize( -1 ) )
		{
			ErrorLogMsg( KEM_ERROR42, "out of memory" );
			return false;  // out of memory
		}
	}

	assert( m_nBufferSize >= 2 );

	// Shift the characters after the index, start by copying the null terminator
	WCHAR* dest = m_pwszBuffer + lstrlenW(m_pwszBuffer)+1;
	WCHAR* stop = m_pwszBuffer + nIndex;
	WCHAR* src = dest - 1;

	while( dest > stop )
	{
		*dest-- = *src--;
	}

	// Set new character
	m_pwszBuffer[ nIndex ] = wChar;
	m_bAnalyseRequired = true;

	return true;
}

bool CKTDGUIUniBuffer::RemoveChar( int nIndex )
{
	if( !lstrlenW( m_pwszBuffer ) || nIndex < 0 || nIndex >= lstrlenW( m_pwszBuffer ) )
		return false;  // Invalid index

	MoveMemory( m_pwszBuffer + nIndex, m_pwszBuffer + nIndex + 1, sizeof(WCHAR) * ( lstrlenW( m_pwszBuffer ) - nIndex ) );
	m_bAnalyseRequired = true;
	return true;
}

bool CKTDGUIUniBuffer::InsertString( int nIndex, const WCHAR *pStr, int nCount )
{
	assert( nIndex >= 0 );

	if( nIndex > lstrlenW( m_pwszBuffer ) )
		return false;  // invalid index

	if( -1 == nCount )
		nCount = lstrlenW( pStr );

	wstring InsertString = pStr;
	if ( m_bAutoChangeBigEnglish == true )
	{
		MakeUpperCase( InsertString );
	}

	// Check for maximum length allowed
	if( GetTextSize() + nCount >= DXUT_MAX_EDITBOXLENGTH )
		return false;

	if( GetTextSize() + nCount > m_nLengthLimit + 1 )
		return false;



	int width = 0;
	//CPtoX( GetTextSize(), FALSE, &width );
    width = GetLineWidth(nIndex);    
	if ( width >= m_WidthLimit )
		return false;

    // Limit Height
    int nLine = GetLineCount(GetTextSize());
    if( nLine > m_LineLimit )
        return false;

	if ( m_bByteLimit == true )
	{
		string tempString;
		string tempString2;
		ConvertWCHARToChar( tempString, m_pwszBuffer );
		ConvertWCHARToChar( tempString2, InsertString.c_str() );

#ifdef FIX_PASTE_ON_EDITBOX
		if ( m_ByteLimit < (int)(tempString.size() + tempString2.size()) )
#else //FIX_PASTE_ON_EDITBOX
		if ( m_ByteLimit <= (int)(tempString.size() + tempString2.size()) )
#endif //FIX_PASTE_ON_EDITBOX
			return false;
	}

	// 한문자 모드일 경우, 글자수 제한
	if ( m_bOneCharMode == true && GetTextSize() + nCount >= 3 )
		return false;

	if( lstrlenW( m_pwszBuffer ) + nCount >= m_nBufferSize )
	{
		if( !SetBufferSize( lstrlenW( m_pwszBuffer ) + nCount + 1 ) )
			return false;  // out of memory
	}

	MoveMemory( m_pwszBuffer + nIndex + nCount, m_pwszBuffer + nIndex, sizeof(WCHAR) * ( lstrlenW( m_pwszBuffer ) - nIndex + 1 ) );
	CopyMemory( m_pwszBuffer + nIndex, InsertString.c_str(), nCount * sizeof(WCHAR) );
	m_bAnalyseRequired = true;

	return true;
}

bool CKTDGUIUniBuffer::SetText( LPCWSTR wszText )
{
	assert( wszText != NULL );

	int nRequired = int(wcslen( wszText ) + 1);

	// Check for maximum length allowed
	if( nRequired >= DXUT_MAX_EDITBOXLENGTH )
		return false;

	if( nRequired > m_nLengthLimit + 1 )
		return false;

	// 한문자 모드일 경우, 글자수 제한
	if ( m_bOneCharMode == true && nRequired >= 3 )
		return false;

	while( GetBufferSize() < nRequired )
		if( !SetBufferSize( -1 ) )
			break;
	// Check again in case out of memory occurred inside while loop.
	if( GetBufferSize() >= nRequired )
	{
		StringCchCopy( m_pwszBuffer, GetBufferSize(), wszText );
		m_bAnalyseRequired = true;
		return true;
	}
	else
		return false;
}

int CKTDGUIUniBuffer::GetCaretLineMove( int nCP, int stepLine )
{
    if ( m_pFontNode != NULL )
    {
#ifndef USE_FREE_TYPE
		HDC hFontDC = m_pFontNode->GetDC();
#endif //USE_FREE_TYPE

        wstring textMsg = m_pwszBuffer;
        if ( m_bUseHideChar == true )
        {
            fill(textMsg.begin(), textMsg.end(), m_cHideChar );
        }        

        wstring lineMsg;
        int nBegin[2000], nEnd[2000];
        int temp, oldIndex = 1, nIndex = 0, nCurrentLine = 0;
        int nLen = GetTextSize();

        nBegin[nIndex] = 0;
        
        temp = textMsg.find(L"\n");
        while(temp >= 0)
        {
            nEnd[nIndex] = temp + oldIndex - 1;
            ++nIndex;
            nBegin[nIndex] = temp + oldIndex;

            lineMsg = textMsg.substr(temp+oldIndex, nLen);
            
            oldIndex = temp + oldIndex + 1;
            temp = lineMsg.find(L"\n");
        }
        nEnd[nIndex] = GetTextSize();

        for(int i=0; i<=nIndex; ++i)
        {
            if(nBegin[i] <= nCP && nEnd[i] >= nCP)
            {
                nCurrentLine = i;
                break;
            }
        }

        int nCaret = nCP - nBegin[nCurrentLine];
        int nResult = 0;

        if(stepLine+nCurrentLine < 0 || stepLine+nCurrentLine > nIndex)
            return -1;

        nResult = nBegin[nCurrentLine+stepLine] + nCaret;
        if(nEnd[nCurrentLine+stepLine] < nResult)
            nResult = nEnd[nCurrentLine+stepLine];

        return nResult;        
    }

    return -1;
}

int CKTDGUIUniBuffer::GetLineCount(int nCP)
{
    if ( m_pFontNode != NULL )
	{
#ifndef USE_FREE_TYPE
        HDC hFontDC = m_pFontNode->GetDC();
#endif //USE_FREE_TYPE

        wstring textMsg = m_pwszBuffer;
        if ( m_bUseHideChar == true )
        {
            fill(textMsg.begin(), textMsg.end(), m_cHideChar );
        }

        int nLine = 0;

        wstring lineMsg = textMsg.substr(0, nCP);

        int beginInx = lineMsg.find(L"\n");
        while(lineMsg.length() > 0 && beginInx >= 0)
        {
            ++nLine;
            lineMsg = lineMsg.substr(beginInx + 1, nCP);
            beginInx = lineMsg.find(L"\n");
        }

        return nLine+1;
    }

    return 0;
}

int CKTDGUIUniBuffer::GetLineWidth( int nCP )
{
    if ( m_pFontNode != NULL )
    {
#ifndef USE_FREE_TYPE
		HDC hFontDC = m_pFontNode->GetDC();
#endif //USE_FREE_TYPE

        wstring textMsg = m_pwszBuffer;
        if ( m_bUseHideChar == true )
        {
            fill(textMsg.begin(), textMsg.end(), m_cHideChar );
        }

        wstring textMsgBegin, textMsgEnd;

        textMsgBegin = textMsg.substr( 0, nCP );
        textMsgEnd = textMsg.substr(nCP, GetTextSize());

        int nNewLineBegin   = textMsgBegin.rfind(L"\n");
        int nNewLineEnd     = textMsgEnd.find(L"\n");

        if( nNewLineBegin >= 0)
        {
            if(nNewLineEnd >= 0)                
                textMsg = textMsg.substr(nNewLineBegin+1, nCP + nNewLineEnd - nNewLineBegin);
            else
                textMsg = textMsg.substr(nNewLineBegin+1, GetTextSize());
        }        
        else
        {
            if(nNewLineEnd >= 0)                
                textMsg = textMsg.substr(0, nCP + nNewLineEnd);
            else
                textMsg = textMsg.substr(0, GetTextSize());
        }

        SIZE size;
		int iLen = (int)wcslen(textMsg.c_str());
#ifdef USE_FREE_TYPE
		size.cx = m_pFontNode->GetWidth( textMsg.c_str() );
#else //USE_FREE_TYPE
		GetTextExtentPoint32W( m_pFontNode->GetDC(), textMsg.c_str(), iLen, &size);
#endif //USE_FREE_TYPE

#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
		return static_cast<int>( static_cast<float>( size.cx ) / g_pKTDXApp->GetResolutionScaleX() );
#else //FIX_CARET_POSITION_AND_FONT_SIZE
		return size.cx;       
#endif //FIX_CARET_POSITION_AND_FONT_SIZE
    }

    return 0;
}

int CKTDGUIUniBuffer::GetLineCP(int nLine)
{
    if ( m_pFontNode != NULL )
    {
#ifndef USE_FREE_TYPE
		HDC hFontDC = m_pFontNode->GetDC();
#endif //USE_FREE_TYPE

        wstring textMsg = m_pwszBuffer;
        if ( m_bUseHideChar == true )
        {
            fill(textMsg.begin(), textMsg.end(), m_cHideChar );
        }        

        if(nLine == 0)
            return 0;

        wstring lineMsg = textMsg.substr(0, GetTextSize());
        int nNewLine = 0;
        for(int i=1; i<=nLine; ++i)
        {
            nNewLine = lineMsg.find(L"\n");
            if(nNewLine < 0)
                return 0;

            lineMsg = lineMsg.substr(nNewLine+1, GetTextSize());
        }

        return nNewLine+1;       
    }
    return 0;
}

HRESULT CKTDGUIUniBuffer::CPtoY( int nCP, BOOL bTrail, int *pY )
{    
    assert( pY ); 
    *pY = 0;  // Default

    int nNewLine = 0;
    int indexNewLine;

    HRESULT hr = S_OK;

    if ( m_pFontNode != NULL )
    {
#ifndef USE_FREE_TYPE
		HDC hFontDC = m_pFontNode->GetDC();
#endif //USE_FREE_TYPE

        wstring textMsg = m_pwszBuffer;
        if ( m_bUseHideChar == true )
        {
            fill(textMsg.begin(), textMsg.end(), m_cHideChar );
        }

        textMsg = textMsg.substr( 0, nCP );

        indexNewLine = textMsg.find(L"\n");

        while( indexNewLine >= 0 )
        {
            ++nNewLine;
            if( (int)wcslen(textMsg.c_str())-1 > indexNewLine )
                textMsg = textMsg.substr(indexNewLine+1, nCP);
            else
                break;

            indexNewLine = textMsg.find(L"\n");
        }   

        SIZE size;
		int iLen = (int)wcslen(textMsg.c_str());
#ifdef USE_FREE_TYPE
		// GetTextExtentPoint32W 함수의 경우 마지막에 있는 LF를 빼고 세로 사이즈를 재므로 똑같이 처리해 주기 위해 잘라줍니다. by 박진웅
		if( textMsg.back() == L'\n' )
			textMsg = textMsg.substr( 0, textMsg.size() - 1 );
		size.cy = m_pFontNode->GetHeight( textMsg.c_str() );
#else //USE_FREE_TYPE
		GetTextExtentPoint32W( m_pFontNode->GetDC(), textMsg.c_str(), iLen, &size);
#endif //USE_FREE_TYPE  


#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
		*pY = static_cast<int>( static_cast<float>( size.cy ) / g_pKTDXApp->GetResolutionScaleY() * nNewLine );
#else //FIX_CARET_POSITION_AND_FONT_SIZE
		*pY = size.cy * nNewLine; 
#endif //FIX_CARET_POSITION_AND_FONT_SIZE
    }

    return hr;
}

HRESULT CKTDGUIUniBuffer::CPtoX( int nCP, BOOL bTrail, int *pX )
{
	assert( pX );
	*pX = 0;  // Default

	HRESULT hr = S_OK;

	if ( m_pFontNode != NULL )
	{
#ifndef USE_FREE_TYPE
		HDC hFontDC = m_pFontNode->GetDC();
#endif //USE_FREE_TYPE

		wstring textMsg = m_pwszBuffer;
		if ( m_bUseHideChar == true )
		{
			fill(textMsg.begin(), textMsg.end(), m_cHideChar );
		}

		textMsg = textMsg.substr( 0, nCP );

        int nNewLine = textMsg.rfind(L"\n");
        if( nNewLine >= 0)
        {
            textMsg = textMsg.substr(nNewLine+1, nCP);
        }        

		SIZE size;
		ZeroMemory( &size, sizeof( size ) );
		int iLen = (int)wcslen(textMsg.c_str());
#ifdef USE_FREE_TYPE
		size.cx = m_pFontNode->GetWidth( textMsg.c_str() );
#else //USE_FREE_TYPE
		GetTextExtentPoint32W( m_pFontNode->GetDC(), textMsg.c_str(), iLen, &size);
#endif //USE_FREE_TYPE

#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
		*pX = static_cast<int>( static_cast<float>( size.cx ) / g_pKTDXApp->GetResolutionScaleX() );
#else //FIX_CARET_POSITION_AND_FONT_SIZE
		*pX = size.cx;
#endif //FIX_CARET_POSITION_AND_FONT_SIZE
	}

	
/*
	CKTDGFontManager::CUKFont* pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( 2 );

	if ( pFont != NULL )
	{
		wstring textMsg = m_pwszBuffer;
		if ( m_bUseHideChar == true )
		{
			fill(textMsg.begin(), textMsg.end(), m_cHideChar );
		}

		textMsg = textMsg.substr( 0, nCP );
		*pX = pFont->GetPureWidth( textMsg.c_str() );
	}
*/
/*
	if( m_bAnalyseRequired )
		hr = Analyse();

	if( SUCCEEDED( hr ) && m_Analysis != NULL )
	{


		hr = _ScriptStringCPtoX( m_Analysis, nCP, bTrail, pX );		
		if ( hr != S_OK )
		{
			if ( m_pwszBuffer != NULL )
			{
				std::wostringstream os;
				os
					<< L"_ScriptStringCPtoX Crash, "
					<< L"nCP : " << nCP << L", "
					<< L"bTrail : " << bTrail << L", "
					<< L"pX : " << pX << L", "
					<< L"m_Analysis : " << m_Analysis
					<< L"내용 : " << m_pwszBuffer;

				ErrorLogMsg( XEM_ERROR99, os.str().c_str() );
			}
			*pX = 0;
		}
	}
	*/
	return hr;
}

HRESULT CKTDGUIUniBuffer::XYtoCP( int nX, int nY, int *pCP, int *pnTrail )
{
    assert( pCP && pnTrail );
    *pCP = 0; *pnTrail = FALSE;

    HRESULT hr = S_OK;

    if ( m_pFontNode != NULL )
    {
#ifndef USE_FREE_TYPE
		HDC hFontDC = m_pFontNode->GetDC();
#endif //USE_FREE_TYPE

        wstring textMsg = m_pwszBuffer;
        int tx, ty, rx, ry;
        bool bY;

        ry = 0;
        bY = false;

        for(int i=0; i<(int)textMsg.length(); ++i)
        {
            CPtoX(i, FALSE, &tx);
            CPtoY(i, FALSE, &ty);
            ry = ty + m_pFontNode->GetHeight();
            
            if(nY <= ry && nY >= ty)
            {
                bY = true;
                if(nX < tx)
                {
                    *pCP = i - 1;
                    return S_OK;
                }
                else
                {
                    rx = i;
                }
            }
            else
            {
                if(bY == true)
                {
                    *pCP = rx;
                    return S_OK;
                }
            }            
        }

        if(textMsg.length() > 0)
            *pCP = textMsg.length() -1;
        else
            *pCP = 0;
        return S_OK;
    }

    *pCP = 0;
    return S_OK;
}

HRESULT CKTDGUIUniBuffer::XtoCP( int nX, int *pCP, int *pnTrail )
{
	assert( pCP && pnTrail );
	*pCP = 0; *pnTrail = FALSE;  // Default

#ifdef FIX_CARET_POSITION_AND_FONT_SIZE
	nX = static_cast<int> ( static_cast<float>( nX ) * g_pKTDXApp->GetResolutionScaleX() );
#endif //FIX_CARET_POSITION_AND_FONT_SIZE

#ifdef USE_FREE_TYPE
	if( m_pFontNode == NULL )
		return S_FALSE;

	wstring wstrBuffer = m_pwszBuffer;
	for( int iCurrentCharIndex = 0; iCurrentCharIndex < static_cast<int>(wstrBuffer.length()); ++iCurrentCharIndex )
	{
		if( m_pFontNode->GetWidth( wstrBuffer.substr( 0, iCurrentCharIndex + 1 ).c_str() ) >= nX )
		{
			*pCP = iCurrentCharIndex;
			return S_OK;
		}
	}
	*pCP = wstrBuffer.length();

	return S_OK;
#else //USE_FREE_TYPE

	HRESULT hr = S_OK;
	if( m_bAnalyseRequired )
		hr = Analyse();

	if( SUCCEEDED( hr ) )
		hr = _ScriptStringXtoCP( m_Analysis, nX, pCP, pnTrail );
    

	// If the coordinate falls outside the text region, we
	// can get character positions that don't exist.  We must
	// filter them here and convert them to those that do exist.
	if( *pCP == -1 && *pnTrail == TRUE )
	{
		*pCP = 0; *pnTrail = FALSE;
	} else
		if( *pCP > lstrlenW( m_pwszBuffer ) && *pnTrail == FALSE )
		{
			*pCP = lstrlenW( m_pwszBuffer ); *pnTrail = TRUE;
		}

		return hr;

#endif //USE_FREE_TYPE
}

void CKTDGUIUniBuffer::GetPriorItemPos( int nCP, int *pPrior )
{
	*pPrior = nCP;  // Default is the char itself


#ifdef USE_FREE_TYPE
	wstring wstrBuffer = m_pwszBuffer;

	int nInitial = nInitial = nCP - 1;
	for( int iCurrentCharIndex = nInitial; iCurrentCharIndex > 0; --iCurrentCharIndex )
	{
		if( wstrBuffer[iCurrentCharIndex] != L' ' &&
			wstrBuffer[iCurrentCharIndex - 1] == L' ' )
		{
			*pPrior = iCurrentCharIndex;
			return;
		}
	}
	*pPrior = 0;
#else //USE_FREE_TYPE
	if( m_bAnalyseRequired )
		if( FAILED( Analyse() ) )
			return;

	const SCRIPT_LOGATTR *pLogAttr = _ScriptString_pLogAttr( m_Analysis );
	if( !pLogAttr )
		return;

	if( !_ScriptString_pcOutChars( m_Analysis ) )
		return;
	int nInitial = *_ScriptString_pcOutChars( m_Analysis );
	if( nCP - 1 < nInitial )
		nInitial = nCP - 1;
	for( int i = nInitial; i > 0; --i )
		if( pLogAttr[i].fWordStop ||       // Either the fWordStop flag is set
			( !pLogAttr[i].fWhiteSpace &&  // Or the previous char is whitespace but this isn't.
			pLogAttr[i-1].fWhiteSpace ) )
		{
			*pPrior = i;
			return;
		}
		// We have reached index 0.  0 is always a break point, so simply return it.
		*pPrior = 0;
#endif //USE_FREE_TYPE
}

void CKTDGUIUniBuffer::GetNextItemPos( int nCP, int *pPrior )
{
	*pPrior = nCP;  // Default is the char itself

#ifdef USE_FREE_TYPE
	wstring wstrBuffer = m_pwszBuffer;

	int nInitial = nInitial = nCP + 1;
	for( int iCurrentCharIndex = nInitial; iCurrentCharIndex < static_cast<int>(wstrBuffer.length()) - 1; ++iCurrentCharIndex )
	{
		if( wstrBuffer[iCurrentCharIndex] == L' ' &&
			wstrBuffer[iCurrentCharIndex + 1] != L' ' )
		{
			*pPrior = iCurrentCharIndex + 1;
			return;
		}
	}
	*pPrior = wstrBuffer.length();
#else //USE_FREE_TYPE
	HRESULT hr = S_OK;
	if( m_bAnalyseRequired )
		hr = Analyse();
	if( FAILED( hr ) )
		return;

	const SCRIPT_LOGATTR *pLogAttr = _ScriptString_pLogAttr( m_Analysis );
	if( !pLogAttr )
		return;

	if( !_ScriptString_pcOutChars( m_Analysis ) )
		return;
	int nInitial = *_ScriptString_pcOutChars( m_Analysis );
	if( nCP + 1 < nInitial )
		nInitial = nCP + 1;
	for( int i = nInitial; i < *_ScriptString_pcOutChars( m_Analysis ) - 1; ++i )
	{
		if( pLogAttr[i].fWordStop )      // Either the fWordStop flag is set
		{
			*pPrior = i;
			return;
		}
		else
			if( pLogAttr[i].fWhiteSpace &&  // Or this whitespace but the next char isn't.
				!pLogAttr[i+1].fWhiteSpace )
			{
				*pPrior = i+1;  // The next char is a word stop
				return;
			}
	}
	// We have reached the end. It's always a word stop, so simply return it.
	*pPrior = *_ScriptString_pcOutChars( m_Analysis ) - 1;
#endif //USE_FREE_TYPE
}

void CKTDGUIUniBuffer::SetHideChar(bool bUseHideChar, WCHAR cHideChar)
{
	m_bUseHideChar = bUseHideChar;
	m_cHideChar = cHideChar;
}
