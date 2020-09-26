/** @file   KJHtmlLog.h
	@brief  interface of KJHtmlLog
	@author seojt@kogstudios.com
	@since  2005-10-6, 10:15:31

    @date   seojt on 2005-12-2, 15:12:24
            - refactoring
    @date   seojt on 2006-8-2, 17:51:50
            - JLOG_COLOR()
*/

#ifndef _KJHTMLLOG_H
#define _KJHTMLLOG_H

#include "KJohnGen.h"
#include "KGlobalSingleton.h"
#include <fstream>

/** @code
    JLOG( "hello %i", i );
    @endcode
*/
#define JLOG            _KJHtmlLog.WriteTimeFileLine(__FILE__,__LINE__,__FUNCTION__); \
                        _KJHtmlLog.Write
/** @code
    JLOG_COLOR( "red", "hello %i", i );
    @endcode
*/
//#define JLOG_COLOR      _KJHtmlLog.WriteTimeFileLine(__FILE__,__LINE__,__FUNCTION__); \
//                        _KJHtmlLog.WriteColor

#define JLOG_ERROR      _KJHtmlLog.WriteTimeFileLine(__FILE__,__LINE__,__FUNCTION__); \
                        _KJHtmlLog.WriteColorRed

#define JLOG_WARN       _KJHtmlLog.WriteTimeFileLine(__FILE__,__LINE__,__FUNCTION__); \
                        _KJHtmlLog.WriteColorBlue

#define JLOG_INFO       _KJHtmlLog.WriteTimeFileLine(__FILE__,__LINE__,__FUNCTION__); \
                        _KJHtmlLog.WriteColorGreen

#define JLOG_BARE       _KJHtmlLog.Write

#define _JLOG           _KJHtmlLog._WriteBuffer2( "    " ); \
                        _KJHtmlLog.Write


//------------------------------------------------------------------------------
/// @class  KJHtmlLog
/// @brief  .htm 파일 포맷의 로그 파일을 생성한다.
/// @note   Write...()로 시작하는 함수는 단위적이다. 즉 각 함수 호출은
///         파일을 열고 닫는다. 이것은 시스템 crash와 상관없이 로그 호출이 항상
///         기록을 남기도록 하기 위함이다.
///
class KJHtmlLog : public KGlobalSingleton<KJHtmlLog>
{
    IMPLEMENT_KGLOBALSINGLETON( KJHtmlLog )

public:
    /// make time dependent log file name.
    /// log filename will be 'pszPrefix_yyyymmdd_hh시mm분ss초.htm'
    static char* MakeLogFilename( char* pszPrefix );

private:
    char                m_szLogFile[MAX_PATH];  ///< log filename
    bool                m_bEnableLog;           ///< log 시스템을 활성화 시킨다.
    bool                m_bLogToDebugConsole;   ///< log 파일과 debug console에 동시에 출력되도록 한다.
    char                m_buffer[4096];
    wchar_t             m_wbuffer[4096];

private:
    /// constructor.
    /// @param  pszLogFile: log filename
    /// @param  bForceDelete: if true, then delete existing log file.
    /// @see    MakeLogFilename()
    KJHtmlLog( const char* pszLogFile = "JLog.html", bool bForceDelete = true );

    /// destructor.
    ~KJHtmlLog();

private:
    void _Init();
    void BeginLog(const char* pszLogFile);
    void EndLog();
    /// m_buffer[]의 내용을 log파일에 write한다.
    void _WriteBuffer();

public:
    void _WriteBuffer( const char* pszBuffer_ );
    void _WriteBuffer2( const char* pszBuffer_ );
    KJHtmlLog& WriteTimeFileLine( const char* pszSrcFile, int iLine, const char* pszFuncName_ );
    KJHtmlLog& Write( const char* pszFormat, ... );
    KJHtmlLog& Write( const wchar_t* pwszFormat, ... );
    KJHtmlLog& WriteColor( const char* pszColor, const char* pszFormat, ... );
    KJHtmlLog& WriteColor( const wchar_t* pszColor, const wchar_t* pszFormat, ... );

    KJHtmlLog& WriteColorRed( const char* pszFormat, ... );
    KJHtmlLog& WriteColorRed( const wchar_t* pszFormat, ... );
    KJHtmlLog& WriteColorGreen( const char* pszFormat, ... );
    KJHtmlLog& WriteColorGreen( const wchar_t* pszFormat, ... );
    KJHtmlLog& WriteColorBlue( const char* pszFormat, ... );
    KJHtmlLog& WriteColorBlue( const wchar_t* pszFormat, ... );

    // readers/writers
    //
    bool        IsEnableLog() const { return m_bEnableLog; }
    bool        IsLogToDebugConsole() const { return m_bLogToDebugConsole; }
    void        SetEnableLog( bool bEnable_ ) { m_bEnableLog = bEnable_; }
    void        SetLogToDebugConsole( bool bToConsole_ ) { m_bLogToDebugConsole = bToConsole_; }

    /// 새로운 로그 파일 이름을 설정한다.
    bool        SetLogFile( const char* pszFilename_, bool bDeletePrevFileIfExist = false );
};//class KJHtmlLog

#define _KJHtmlLog  KJHtmlLog::Singleton()

#endif // #ifndef _KJHTMLLOG_H


/** @example    KJHtmlLog
	@code

    #include "jlog/KJHtmlLog.h"

    void main()
    {
        static int ii = 99;
        JLOG_BARE( "start server... %i", ii+100 );
        JLOG( "%d", ii );
        JLOG_COLOR( "red", "formatted colored message %i", ii+200 );
        JLOG( "%d", ii+1000 );
    }//main()

    // 'JLog.html' file
    //
    <html>
    <head>
    <title>
    </title>
    </head>
    <body bgcolor="#ffffff">
    <br>
    start server... 199<br>
    [2005-12-02, 15:16:03] iocpserver_seojt.cpp(main(),7): 99<br>
    [2005-12-02, 15:16:03] iocpserver_seojt.cpp(main(),8): <font color=red>
    unformatted error message</font><br>
    [2005-12-02, 15:16:03] iocpserver_seojt.cpp(main(),9): 1099<br>

	@endcode
*/
