/** @file   KMessageConsole.cpp
	@brief  implementation of KMessageConsole
*/

#define _TOSTR1(x)    #x
#define _TOSTR(x)     _TOSTR1(x)
#include _TOSTR(PROJECT_PCH)
#include <assert.h>
#include "KMessageConsole.h"
//#include "DXUTmisc.h"
#include "../ktdxlib/dxut/DXUTmisc.h"
#include "KJohnStrUtil.h"


extern ID3DXFont*              g_pFont_Profiler;		// Font for drawing text
extern ID3DXSprite*            g_pTextSprite_Profiler;  // Sprite for batching draw text calls


//------------------------------------------------------------------------------
// static function(s)
//
/*static*/ void KMessageConsole::OutText(float _x, float _y, const char* _pszText, D3DXCOLOR _dxColor)
{
    CDXUTTextHelper txtHelper( g_pFont_Profiler, g_pTextSprite_Profiler, 15 );
//{{ robobeg : 2006-09-05 // 10;25
    //TCHAR           tstrMsg[MAX_PATH];
    WCHAR   wszMsg[MAX_PATH];
//}} robobeg : 2006-09-05 // 10;25

    txtHelper.Begin();
    txtHelper.SetInsertionPos( (int)_x, (int)_y );
    txtHelper.SetForegroundColor( _dxColor );
//{{ robobeg : 2006-09-05 // 10:25
    //ConvertAnsiStringToGeneric( tstrMsg, _pszText );
    //txtHelper.DrawTextLine( tstrMsg );
    ConvertAnsiStringToWide( wszMsg, _pszText, MAX_PATH );
    txtHelper.DrawTextLine( wszMsg );
//}}robobeg : 2006-09-05 // 10:25
    txtHelper.End();
}//KMessageConsole::OutText()


/*static*/ void KMessageConsole::OutText(float _x, float _y, D3DXCOLOR _dxColor, const char* _pszFormat, ...)
{
    char    out[1024];
	va_list argList;

    va_start( argList, _pszFormat );
    StringCchVPrintfA( out, 1024, _pszFormat, argList );
    va_end( argList );

    KMessageConsole::OutText( _x, _y, out, _dxColor );
}//OutText()


/*static*/ void KMessageConsole::OutText(float _x, float _y, const wchar_t* _pwszText, D3DXCOLOR _dxColor)
{
    CDXUTTextHelper txtHelper( g_pFont_Profiler, g_pTextSprite_Profiler, 15 );

    txtHelper.Begin();
    txtHelper.SetInsertionPos( (int)_x, (int)_y );
    txtHelper.SetForegroundColor( _dxColor );
    txtHelper.DrawTextLine( _pwszText );
    txtHelper.End();
}//KMessageConsole::OutText()


/*static*/ void KMessageConsole::OutText(float _x, float _y, D3DXCOLOR _dxColor, const wchar_t* _pwszFormat, ...)
{
    wchar_t    out[1024];
	va_list argList;

    va_start( argList, _pwszFormat );
    StringCchVPrintfW( out, 1024, _pwszFormat, argList );
    va_end( argList );

    KMessageConsole::OutText( _x, _y, out, _dxColor );
}//OutText()


//------------------------------------------------------------------------------
// member functions
//
// constructor.
KMessageConsole::KMessageConsole()
{
    _Init();
}//KMessageConsole::KMessageConsole()


// destructor.
/*virtual*/KMessageConsole::~KMessageConsole()
{
    SafeDelete();
}//KMessageConsole::~KMessageConsole()


void KMessageConsole::_Init()
{
    m_fStateTimer       = 0.f;
    m_iMaxNumOfMessage  = 10;
    m_eState            = KMC_STATE_NORMAL;
    m_fFadeOutTime      = 0.5f;
    m_fLineShowTime     = 2.f;
    m_fTextHeight       = 16.f;
    m_fLineSpaceRatio   = 1.f;
}//KMessageConsole::_Init()


void KMessageConsole::SafeDelete()
{
    m_listStr.clear();
}//KMessageConsole::SafeDelete()


void KMessageConsole::AddMessage(const char* _pszMsg, DWORD _dwColor)
{
    if ( m_listStr.size() >= (size_t)m_iMaxNumOfMessage )
        RemoveAMessageAtHead();

    KMsgConsoleNode mcNode;
    mcNode.m_dxColor    = D3DXCOLOR( _dwColor );
//{{ robobeg : 2006-09-05 // 10:25
    //mcNode.m_strMsg     = _pszMsg;
    ConvertStrStringToStdWString( mcNode.m_wstrMsg, _pszMsg );
//}} robobeg : 2006-09-05 // 10:25

    m_listStr.push_back( mcNode );
}//KMessageConsole::AddMessage()


void KMessageConsole::AddMessage(DWORD _dwColor, const char* _pszFormat, ...)
{
    char    out[1024];
	va_list argList;

    va_start( argList, _pszFormat );
    StringCchVPrintfA( out, 1024, _pszFormat, argList );
    va_end( argList );

    AddMessage( out, _dwColor );
}//KMessageConsole::AddMessage()


//{{ robobeg : 2006-09-05 // 10:16

void KMessageConsole::AddMessage(const wchar_t* _pwszMsg, DWORD _dwColor)
{
    if ( m_listStr.size() >= (size_t)m_iMaxNumOfMessage )
        RemoveAMessageAtHead();

    KMsgConsoleNode mcNode;
    mcNode.m_dxColor    = D3DXCOLOR( _dwColor );
    mcNode.m_wstrMsg     = _pwszMsg;

    m_listStr.push_back( mcNode );
}//KMessageConsole::AddMessage()


void KMessageConsole::AddMessage(DWORD _dwColor, const wchar_t* _pwszFormat, ...)
{
    wchar_t    out[1024];
	va_list argList;

    va_start( argList, _pwszFormat );
    StringCchVPrintfW( out, 1024, _pwszFormat, argList );
    va_end( argList );

    AddMessage( out, _dwColor );
}//KMessageConsole::AddMessage()

//}} robobeg : 2006-09-05 // 10:16


BOOL KMessageConsole::RemoveAMessageAtHead()
{
    if ( m_listStr.size() <= 0 )
        return FALSE;

    STRLIST_ITOR    litor = m_listStr.begin();
    if ( litor != m_listStr.end() )
    {
        litor = m_listStr.erase( litor );
        return TRUE;
    }//if

    return FALSE;
}//KMessageConsole::RemoveAMessageAtHead()


void KMessageConsole::SetMaxNumOfMessages(int _iNumMaxMessage)
{
    m_iMaxNumOfMessage = _iNumMaxMessage;
}//KMessageConsole::SetMaxNumOfMessages()


void KMessageConsole::OnRender(float _x, float _y, float _fElapsedTime)
{
    STRLIST_ITOR    litor = m_listStr.begin();

    if ( litor == m_listStr.end() )
        return;

    // increase state timer
    //
    m_fStateTimer += _fElapsedTime;

    // render first line
    //
    float           fLeft       = _x;
    float           fTop        = _y;
    float           fTextHeight = m_fTextHeight * m_fLineSpaceRatio;
    float           fRatio;

    if ( KMC_STATE_FADEOUT == m_eState )
    {
        if ( m_fStateTimer >= m_fFadeOutTime )
        {
            m_eState        = KMC_STATE_NORMAL;
            m_fStateTimer   = 0.f;
            RemoveAMessageAtHead();
            litor = m_listStr.begin();
        }//if

        fRatio = m_fStateTimer / m_fFadeOutTime;
        fRatio = __min( 1.f, __max(0.f,fRatio) );
        fTop  -= fTextHeight * fRatio;
        litor->m_dxColor.a = 1.f - fRatio;
        if ( litor != m_listStr.end() )
        {
//{{ robobeg : 2006-09-05 // 11:18
            //KMessageConsole::OutText( fLeft, fTop, litor->m_strMsg.c_str(), litor->m_dxColor );
            KMessageConsole::OutText( fLeft, fTop, litor->m_wstrMsg.c_str(), litor->m_dxColor );
//}} robobeg : 2006-09-05 // 11:18
            fTop += fTextHeight;
            ++litor;
        }//if
    }
    else if ( KMC_STATE_NORMAL == m_eState )
    {
        if ( m_fStateTimer >= m_fLineShowTime )
        {
            m_eState        = KMC_STATE_FADEOUT;
            m_fStateTimer   = 0.f;
        }//if
    }//if

    // render remaining lines
    //
    while ( litor != m_listStr.end() )
    {
        litor->m_dxColor.a = 1.f;
//{{ robobeg : 2006-09-05 // 11:18
        //KMessageConsole::OutText( fLeft, fTop, litor->m_strMsg.c_str(), litor->m_dxColor );
        KMessageConsole::OutText( fLeft, fTop, litor->m_wstrMsg.c_str(), litor->m_dxColor );
//}} robobeg : 2006-09-05 // 11:18
        fTop += fTextHeight;

        ++litor;
    }//while
}//KMessageConsole::OnRender()
