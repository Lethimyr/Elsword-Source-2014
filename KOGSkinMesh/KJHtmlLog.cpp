/** @file   KJHtmlLog.cpp
	@brief  implementation of KJHtmlLog
*/

#define _TOSTR1(x)    #x
#define _TOSTR(x)     _TOSTR1(x)
#include _TOSTR(PROJECT_PCH)
#include "KJHtmlLog.h"
#include <strsafe.h>
#include <time.h>
#include <stdlib.h>
// 다른 프로젝트에서 쉽게 log 기능을 추가할 수 있도록, 파일 의존도를 줄임
// - jintaeks on 2008-10-09, 10:25
//#include "KJohnStrUtil.h"
//#include "KJohnWin32Util.h"
#include <strsafe.h>
#ifdef _ODS306_PROJECT // 2008-7-29
#include "LuaApp/KConsole.h"
#endif // _ODS306_PROJECT // 2008-7-29



///-----------------------------------------------------------------------------
// static members
//
/// pszPrefix로 시작하고, 날짜와 시간을 postfix로 하는 파일이름(.htm)을 생성한다.
/*static*/char* KJHtmlLog::MakeLogFilename( char* pszPrefix )
{
    static char s_buffer[MAX_PATH] = "";

    time_t  t       = time( NULL );
    tm*     time    = localtime( &t );
    StringCchPrintfA( s_buffer, ARRAY_SIZE( s_buffer ), "%s_%04d%02d%02d_%02d시%02d분%02d초.htm",
        pszPrefix, time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
        time->tm_hour, time->tm_min, time->tm_sec );

    return s_buffer;
}//KJHtmlLog::MakeLogFilename()


static bool _DeleteFile(const CHAR* pszFilePath_)
{
    DWORD dwAtt;

    // readonly 속성이 지정되어 있으면 먼저 속성을 해제한다.
    if ( 0xffffffff != ( dwAtt = ::GetFileAttributesA( pszFilePath_ ) ) )
    {
        ::SetFileAttributesA( pszFilePath_, dwAtt & (~FILE_ATTRIBUTE_READONLY) );
    }//if

    return ::DeleteFileA( pszFilePath_ ) ? true : false;
}//_DeleteFile()


static int _WideToCharString( CHAR* strDestination, int _iMaxLength, const WCHAR* _wstrSource, UINT uInCodePage_ = CP_ACP )
{
    bool    bTest = strDestination == NULL || _iMaxLength <= 0;

    if ( _wstrSource == NULL )
    {
        if ( !bTest )
            strDestination[ 0 ] = NULL;
        return 1;
    }//if

    if ( bTest )
        return  ::WideCharToMultiByte( uInCodePage_, 0, _wstrSource, -1, NULL, 0, NULL, NULL );

    return  ::WideCharToMultiByte( uInCodePage_, 0, _wstrSource, -1, strDestination, _iMaxLength, NULL, NULL );
}//_WideToCharString()


//------------------------------------------------------------------------------
//
// constructor.
KJHtmlLog::KJHtmlLog( const char* pszLogFile, bool bForceDelete/* = false*/ )
{
    _Init();
    if ( bForceDelete )
    {
        _DeleteFile( pszLogFile );
    }//if
    BeginLog( pszLogFile );
}//KJHtmlLog::KJHtmlLog()


// destructor.
KJHtmlLog::~KJHtmlLog()
{
    EndLog();
}//KJHtmlLog::~KJHtmlLog()


void KJHtmlLog::_Init()
{
    m_szLogFile[0]          = NULL;
    m_bEnableLog            = true;
    m_bLogToDebugConsole    = true;
}//KJHtmlLog::_Init()


void KJHtmlLog::BeginLog( const char* pszLogFile )
{
    if ( !m_bEnableLog ) return;
    ASSERT( pszLogFile );

    StringCchCopyA( m_szLogFile, ARRAY_SIZE( m_szLogFile ), pszLogFile );
    Write( "<html>\r\n"
        "<head>\r\n"
        //"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
        "<title> Log </title>\r\n"
        "<H2><font color=\"white\">%s</font></H2>\r\n"
        "</head>\r\n"
        "<body bgcolor=\"#000000\">\r\n<br><hr><br>"
        "<pre>\r\n"
        "<font face=\"Fixedsys\" size=\"2\" color=\"white\">",
        pszLogFile );
}//KJHtmlLog::BeginLog()


void KJHtmlLog::EndLog()
{
    if ( !m_bEnableLog ) return;
    Write( "</font>\r\n"
        "</pre>\r\n"
        "</body>\r\n"
        "</html>\r\n" );
}//KJHtmlLog::EndLog()


void KJHtmlLog::_WriteBuffer( const char* pszBuffer_ )
{
    if ( m_bLogToDebugConsole )
    {
        OutputDebugStringA( pszBuffer_ );
    }//if

    FILE*   fp = fopen( m_szLogFile, "ab" );
    if ( fp )
    {
        fprintf( fp, pszBuffer_ );
        fclose( fp );
    }//if
}//KJHtmlLog::_WriteBuffer()


void KJHtmlLog::_WriteBuffer()
{
    _WriteBuffer( m_buffer );
}//KJHtmlLog::_WriteBuffer()


void KJHtmlLog::_WriteBuffer2(const char* pszBuffer_)
{
#ifdef _ODS306_PROJECT // 2008-7-29
    KCon << pszBuffer_;
#endif // _ODS306_PROJECT // 2008-7-29
    _WriteBuffer( pszBuffer_ );
}//KJHtmlLog::_WriteBuffer2()


KJHtmlLog& KJHtmlLog::WriteTimeFileLine(
    const char*     pszSrcFile,
    int             iLine,
    const char*     pszFuncName_ )
{
    ASSERT( pszSrcFile );
    ASSERT( pszFuncName_ );

    if ( !m_bEnableLog ) return *this;

    FILE*   fp = fopen( m_szLogFile, "ab" );
    if ( fp )
    {
        // source filename
        //
        char    szNameExt[MAX_PATH];
        //Win32_GetPathAndFile( NULL, szNameExt, pszSrcFile );
        //char    aszDrive[_MAX_DRIVE];
        //char    aszDir[_MAX_DIR];
        char    aszFName[_MAX_FNAME];
        char    aszExt[_MAX_EXT];

        _splitpath( pszSrcFile, NULL, NULL, aszFName, aszExt );
        StringCchCopyA( szNameExt, MAX_PATH, aszFName );
        StringCchCatA( szNameExt, MAX_PATH, aszExt );

        time_t  t       = time( NULL );
        tm*     time    = localtime( &t );

        {
            //sprintf( m_buffer, "[%04d-%02d-%02d, %02d:%02d:%02d] %s(%s(),%i): ",
            //    time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
            //    time->tm_hour, time->tm_min, time->tm_sec,
            //    szNameExt, pszFuncName_, iLine );
            sprintf( m_buffer, "[%04d-%02d-%02d, %02d:%02d:%02d] %s(%i): ",
                time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
                time->tm_hour, time->tm_min, time->tm_sec,
                szNameExt, iLine );

            if ( m_bLogToDebugConsole )
            {
                OutputDebugStringA( m_buffer );
#ifdef _ODS306_PROJECT // 2008-7-29
                //KCon << m_buffer << kendl;
#endif // _ODS306_PROJECT // 2008-7-29
            }//if

            fprintf( fp, m_buffer );
        }//block

        fclose( fp );
    }//if

    return *this;
}//KJHtmlLog::WriteTimeFileLine()


KJHtmlLog& KJHtmlLog::Write( const char* pszFormat, ... )
{
    ASSERT( pszFormat );

    if ( !m_bEnableLog ) return *this;

    va_list     argList;
    va_start( argList, pszFormat );
    StringCchVPrintfA( m_buffer, ARRAY_SIZE( m_buffer ), pszFormat, argList );
    va_end( argList );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    KCon << m_buffer;
#endif // _ODS306_PROJECT // 2008-7-29

    return *this;
}//KJHtmlLog::Write()


KJHtmlLog& KJHtmlLog::Write( const wchar_t* pwszFormat, ... )
{
    ASSERT( pwszFormat );

    if ( !m_bEnableLog ) return *this;

    va_list     argList;
    va_start( argList, pwszFormat );
    StringCchVPrintfW( m_wbuffer, ARRAY_SIZE( m_wbuffer ), pwszFormat, argList );
    va_end( argList );

    _WideToCharString( m_buffer, ARRAY_SIZE(m_buffer), m_wbuffer );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    KCon << m_buffer;
#endif // _ODS306_PROJECT // 2008-7-29

    return *this;
}//KJHtmlLog::Write()


KJHtmlLog& KJHtmlLog::WriteColor( const char* pszColor, const char* pszFormat, ... )
{
    ASSERT( pszColor );
    ASSERT( pszFormat );

    if ( !m_bEnableLog ) return *this;

    char    szFormatBuffer[4096];
    StringCchPrintfA( szFormatBuffer, ARRAY_SIZE( szFormatBuffer ),
        "<font color=%s>\r\n%s</font>\r\n",
        pszColor, pszFormat );

    va_list     argList;
    va_start( argList, pszFormat );
    StringCchVPrintfA( m_buffer, ARRAY_SIZE( m_buffer ), szFormatBuffer, argList );
    va_end( argList );

    _WriteBuffer();

    return *this;
}//KJHtmlLog::WriteColor()
 

KJHtmlLog& KJHtmlLog::WriteColor( const wchar_t* pszColor, const wchar_t* pwszFormat, ... )
{
    if ( !m_bEnableLog ) return *this;

    wchar_t    wszFormatBuffer[4096];
    StringCchPrintfW( wszFormatBuffer, ARRAY_SIZE( wszFormatBuffer ),
        L"<font color=%S>\r\n%s</font>\r\n",
        pszColor, pwszFormat );

    va_list     argList;
    va_start( argList, pwszFormat );
    StringCchVPrintfW( m_wbuffer, ARRAY_SIZE( m_wbuffer ), wszFormatBuffer, argList );
    va_end( argList );

    _WideToCharString( m_buffer, ARRAY_SIZE(m_buffer), m_wbuffer );

    _WriteBuffer();

    return *this;
}//KJHtmlLog::WriteColor()


KJHtmlLog& KJHtmlLog::WriteColorRed( const char* pszFormat, ... )
{
    _WriteBuffer( "<font color=red>" );

    va_list     argList;
    va_start( argList, pszFormat );
    StringCchVPrintfA( m_buffer, ARRAY_SIZE( m_buffer ), pszFormat, argList );
    va_end( argList );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    unsigned int iOldColor = KConsole::Singleton().GetCurColor();
    KConsole::Singleton().SetColor( KConsole::eRedOnBlack );
    KCon << m_buffer;
	KConsole::Singleton().SetColor( iOldColor );
#endif // _ODS306_PROJECT // 2008-7-29

    _WriteBuffer( "</font>\r\n" );

    return *this;
}//KJHtmlLog::WriteColorRed()


KJHtmlLog& KJHtmlLog::WriteColorRed( const wchar_t* pwszFormat, ... )
{
    _WriteBuffer( "<font color=red>" );

    va_list     argList;
    va_start( argList, pwszFormat );
    StringCchVPrintfW( m_wbuffer, ARRAY_SIZE( m_wbuffer ), pwszFormat, argList );
    _WideToCharString( m_buffer, ARRAY_SIZE(m_buffer), m_wbuffer );
    va_end( argList );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    unsigned int iOldColor = KConsole::Singleton().GetCurColor();
    KConsole::Singleton().SetColor( KConsole::eRedOnBlack );
    KCon << m_buffer;
	KConsole::Singleton().SetColor( iOldColor );
#endif // _ODS306_PROJECT // 2008-7-29

    _WriteBuffer( "</font>\r\n" );

    return *this;
}//KJHtmlLog::WriteColorRed()


KJHtmlLog& KJHtmlLog::WriteColorGreen( const char* pszFormat, ... )
{
    _WriteBuffer( "<font color=lightgreen>" );

    va_list     argList;
    va_start( argList, pszFormat );
    StringCchVPrintfA( m_buffer, ARRAY_SIZE( m_buffer ), pszFormat, argList );
    va_end( argList );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    unsigned int iOldColor = KConsole::Singleton().GetCurColor();
    KConsole::Singleton().SetColor( KConsole::eGreenOnBlack );
    KCon << m_buffer;
	KConsole::Singleton().SetColor( iOldColor );
#endif // _ODS306_PROJECT // 2008-7-29

    _WriteBuffer( "</font>\r\n" );

    return *this;
}//KJHtmlLog::WriteColorGreen()


KJHtmlLog& KJHtmlLog::WriteColorGreen( const wchar_t* pwszFormat, ... )
{
    _WriteBuffer( "<font color=lightgreen>" );

    va_list     argList;
    va_start( argList, pwszFormat );
    StringCchVPrintfW( m_wbuffer, ARRAY_SIZE( m_wbuffer ), pwszFormat, argList );
    _WideToCharString( m_buffer, ARRAY_SIZE(m_buffer), m_wbuffer );
    va_end( argList );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    unsigned int iOldColor = KConsole::Singleton().GetCurColor();
    KConsole::Singleton().SetColor( KConsole::eGreenOnBlack );
    KCon << m_buffer;
	KConsole::Singleton().SetColor( iOldColor );
#endif // _ODS306_PROJECT // 2008-7-29

    _WriteBuffer( "</font>\r\n" );

    return *this;
}//KJHtmlLog::WriteColorGreen()


KJHtmlLog& KJHtmlLog::WriteColorBlue( const char* pszFormat, ... )
{
    _WriteBuffer( "<font color=blue>" );

    va_list     argList;
    va_start( argList, pszFormat );
    StringCchVPrintfA( m_buffer, ARRAY_SIZE( m_buffer ), pszFormat, argList );
    va_end( argList );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    unsigned int iOldColor = KConsole::Singleton().GetCurColor();
    KConsole::Singleton().SetColor( KConsole::eBlueOnBlack );
    KCon << m_buffer;
	KConsole::Singleton().SetColor( iOldColor );
#endif // _ODS306_PROJECT // 2008-7-29

    _WriteBuffer( "</font>\r\n" );

    return *this;
}//KJHtmlLog::WriteColorBlue()


KJHtmlLog& KJHtmlLog::WriteColorBlue( const wchar_t* pwszFormat, ... )
{
    _WriteBuffer( "<font color=blue>" );

    va_list     argList;
    va_start( argList, pwszFormat );
    StringCchVPrintfW( m_wbuffer, ARRAY_SIZE( m_wbuffer ), pwszFormat, argList );
    _WideToCharString( m_buffer, ARRAY_SIZE(m_buffer), m_wbuffer );
    va_end( argList );

    StringCchCatA( m_buffer, ARRAY_SIZE( m_buffer ), "\r\n" );
    _WriteBuffer();

#ifdef _ODS306_PROJECT // 2008-7-29
    unsigned int iOldColor = KConsole::Singleton().GetCurColor();
    KConsole::Singleton().SetColor( KConsole::eBlueOnBlack );
    KCon << m_buffer;
	KConsole::Singleton().SetColor( iOldColor );
#endif // _ODS306_PROJECT // 2008-7-29

    _WriteBuffer( "</font>\r\n" );

    return *this;
}//KJHtmlLog::WriteColorBlue()


bool KJHtmlLog::SetLogFile( const char* pszFilename_, bool bDeletePrevFileIfExist /*= false*/ )
{
    // 이전에 작업중인던 log 파일의 종료 작업을 한다.
    if ( m_bEnableLog )
        EndLog();

    // 로그 파일 이름이 NULL이면 false를 리턴한다.
    if ( pszFilename_ == NULL )
        return false;

    // 이미 존재하는 같은 파일 이름의 로그 파일을 삭제한다.
    if ( bDeletePrevFileIfExist )
    {
        _DeleteFile( pszFilename_ );
    }//if

    // 새로운 로그 파일로 기록을 시작한다.
    BeginLog( pszFilename_ );

    return true;
}//KJHtmlLog::SetLogFile()
