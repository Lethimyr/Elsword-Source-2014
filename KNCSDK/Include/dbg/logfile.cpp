//////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Triple Buffer Software 2003 All rights reserved                                        //
//                                                                                                  //
// This is free sour code. You may check the terms and conditions for free source code usage at:    //
//                                                                                                  //
// http://triplebuffer.devmaster.net/license.php                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////


#include <ctime>
#include <string>
#include "logfile.hpp"

//{{ florist
#include "atltime.h"    // 대놓고 Shared Class 사용. VC 6.0은 사용할 수 없다.

// The log file logs everything to this file name
#define TB_LOGFILE_NAME "log.htm"   // 헤더에 있던 매크로 선언을 cpp로 옮겨왔다.

#include <direct.h>     // _mkdir
#include <windows.h>    // GetCurrentDirectory
#include <sstream>      // std::ostringstream
//}} florist
//{{ 2013. 02. 19	최육사	로그 시스템 개선
//#ifdef SERV_LOG_SYSTEM_NEW
#include <hash_map>
//#endif SERV_LOG_SYSTEM_NEW
//}}

#define BOOST_UTF8_BEGIN_NAMESPACE namespace utf {
#define BOOST_UTF8_END_NAMESPACE }
#define BOOST_UTF8_DECL 
#include "utf8_codecvt_facet.cpp"


#include <boost/archive/codecvt_null.hpp>


// 현재 경로에 sub folder를 받는다. 함수는 sub-folder의 이름만 받는다.
bool MakeFolder( const wchar_t* szPath )
{
    wchar_t pathbuff[MAX_PATH]     = {0};
    ::GetCurrentDirectoryW( MAX_PATH, pathbuff );

    std::wstring strFolderPath( pathbuff );
    strFolderPath += L"\\";
    strFolderPath += szPath;

    if( 0 == ::_wmkdir( strFolderPath.c_str() ) ) return true;  // 생성된 경우.

    switch( errno )
    {
    case EEXIST:    // 이미 존재하는 경우
        return true;
        break;
    default:        // unknown error.
        break;
    }

    return false;
}

namespace dbg
{

    //{{ florist
    logfile cout(   lm_comment,     0 );
    logfile cerr(   lm_error,       0 );
    logfile cwarn(  lm_warning,     1 );
    logfile clog(   lm_info,        2 );

    logfile cout2(  lm_comment2,    0 );
    logfile cerr2(  lm_error2,      0 );
    logfile cwarn2( lm_warning2,    1 );
    logfile clog2(  lm_info2,       2 );
    // original below
    //logfile clog( lm_info );
    //logfile cerr( lm_error );
    //logfile cwarn( lm_warning );
    //logfile cout( lm_comment );
    //}} florist

    // html constants
    namespace
    {
        const wchar_t* const   szFontColor[lm_number_of_modes] = {
            L"#00FF00",
            L"#FF0000",
            L"#FF9900",
            L"#C1F0FF",

            L"#00FFD2",
            L"#DD3434",
            L"#FFDE00",
            L"#C1C8FF",
        };
        const unsigned int nFontSize[lm_number_of_modes] = {
            2, 2, 2, 2, 2, 2, 2, 2
        };

        const wchar_t* const szFontFace[lm_number_of_modes] = {
            L"courier new",
            L"courier new",
            L"courier new",
            L"courier new",
            L"courier",
            L"courier",
            L"courier",
            L"courier",
        };
        //const char* const   errorCol    =   "FF0000";
        //const unsigned int  errorSize   =   2;
        //const char* const   errorFont   =   "courier new";

        //const char* const   infoCol     =   "#C1F0FF";
        //const unsigned int  infoSize    =   2;
        //const char* const   infoFont    =   "courier new";

        //const char* const   commCol     =   "#00FF00";
        //const unsigned int  commSize    =   2;
        //const char* const   commFont    =   "courier new";

        //const char* const   warnCol     =   "#FF9900";
        //const unsigned int  warnSize    =   2;
        //const char* const   warnFont    =   "courier new";

        const wchar_t* const pageCol       =   L"#000000";          // Default page color
        const wchar_t* const linkCol       =   L"#FFFFFF";          // Default colors for links
        const wchar_t* const visitedCol    =   L"#FFFF97";          // Default color for visited links
        const wchar_t* const titleCol      =   L"#FFFF97";          // Color of title
        //{{ florist. 
        std::wstring logTitle = L"Log";
        // original below
        //const char* const logTitle      =   "Base Server Log";  // Title of log file
        //}}

        std::locale old_locale;
        std::locale utf8_locale( old_locale,new utf::utf8_codecvt_facet );
        //std::locale utf16_locale( old_locale,new boost::archive::codecvt_null<wchar_t> );
        std::locale utf16_locale( "" );// 051226. kkurrung. 콘솔에 한글 출력 문제(코드페이지 지원문제).

    } 


    std::wofstream logfile::m_file;
    bool logfile::m_initialized = false;

    bool logfile::m_abCurMode[lm_number_of_modes] = { false };
    //bool logfile::m_was_error = false;
    //bool logfile::m_was_info = false;
    //bool logfile::m_was_comm = false;
    //bool logfile::m_was_warn = false;

    //{{ florist
    bool logfile::ms_bFileNameAtTime = false;   // default : false. 클라이언트의 기본값에 맞추자.
    short logfile::ms_sLevel = 2;
    std::string logfile::ms_strFileName( "Logfile Not Initialize Yet." );

    //logfile::logfile( e_log_mode lm )
    logfile::logfile( e_log_mode lm, unsigned short sLevel )
    {
        m_mode = lm;
        m_sLevel = sLevel;
    }
    //}} florist

    void logfile::init()
    {
        if( !m_initialized )
        {
            //{{ florist
            OpenLogFile();
            // original
            //m_file.open( TB_LOGFILE_NAME );
            //}} florist

            if( !m_file )
                m_initialized = false;
            else
            {
                m_initialized = true;

                std::time_t t = std::time( NULL );
                std::tm *tme = std::localtime( &t );

                m_file <<  L"<html> <HEAD>" 
                       << L"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                       << L"<CENTER> <TITLE>"
                    << logTitle<<  L"</TITLE> <H3><font color=\""
                    << titleCol<<  L"\">" << logTitle<<  L" for "
                    << tme->tm_mday<<  L"/"<< tme->tm_mon + 1<<  L"/"<< (tme->tm_year + 1900)
                    <<  L" "<< tme->tm_hour<<  L":"<< tme->tm_min<<  L":"<< tme->tm_sec<< L"</font><br><br>";


                m_file <<  L"</CENTER> </HEAD>\
                           <BODY link=\""<< linkCol<<  L"\" vlink=\""<< visitedCol
                           <<  L"\" alink=\""<< linkCol<<  L"\" bgcolor=\""<< pageCol<<  L"\">\
                                                                                       <br><hr><br>";
                //{{ florist
                m_file << L"<pre>";
                //}} florist
            }
        }
    }

    logfile::~logfile()
    {
        if( m_initialized )
        {
            m_file.close();
            m_initialized = false;
        }
    }

    void logfile::set_mode()
    {
        this->init();
        this->end();
        this->start();
    }


    logfile &logfile::line_file_func( const unsigned int line, const wchar_t* file, const wchar_t* func )
    {   
        
        this->set_mode();
        std::wstring str = file;
        str = str.substr( str.rfind(  L"\\" ) + 1 );
        m_file<< func<<  L"("<< line <<  L")::" << str.c_str();

        return *this;
    }


    void logfile::start()
    {
        if( !m_initialized )
            return;

        if( ! m_abCurMode[ m_mode ] )
        //if( m_mode == lm_comment && !m_was_comm )
        {
            m_file<<  L"<font face=\""<< szFontFace[ m_mode ]
                <<  L"\" size=\""<< nFontSize[ m_mode ]
                <<  L"\" color=\""<< szFontColor[ m_mode ]<<  L"\">";

            ZeroMemory( m_abCurMode, sizeof( m_abCurMode ) );
            m_abCurMode[ m_mode ] = true;
        }
        //    m_abCurMode[lm_comment] = true;
        //    m_was_comm = true;
        //    m_was_info = false;
        //    m_was_error = false;
        //    m_was_warn = false; 
        //}
        //else if( m_mode == lm_error && !m_was_error )
        //{
        //    m_file<<  "<font face=\""<< errorFont
        //        <<  "\" size=\""<< errorSize
        //        <<  "\" color=\""<< errorCol<<  "\">";

        //    m_was_error = true;
        //    m_was_info = false;
        //    m_was_comm = false; 
        //    m_was_warn = false; 
        //}
        //else if( m_mode == lm_info && !m_was_info )
        //{
        //    m_file<<  "<font face=\""<< infoFont
        //        <<  "\" size=\""<< infoSize
        //        <<  "\" color=\""<< infoCol<<  "\">";

        //    m_was_info = true;
        //    m_was_error = false;
        //    m_was_comm = false; 
        //    m_was_warn = false; 
        //}
        //else if( m_mode == lm_warning && !m_was_warn )
        //{
        //    m_file<<  "<font face=\""<< warnFont
        //        <<  "\" size=\""<< warnSize
        //        <<  "\" color=\""<< warnCol<<  "\">";

        //    m_was_warn = true;
        //    m_was_info = false;
        //    m_was_error = false;
        //    m_was_comm = false; 
        //}
    }

    void logfile::end()
    {
        if( !m_initialized )
            return;

        if( !m_abCurMode[ m_mode ] )
        //if( m_was_info && m_mode != lm_info )
        {
            m_file <<  L"</font>";
            m_file.flush();
        }
        //else if( m_was_error && m_mode != lm_error )
        //{
        //    m_file<<  "</font>";
        //    m_file.flush();
        //}
        //else if( m_was_comm && m_mode != lm_comment )
        //{
        //    m_file<<  "</font>";

        //}
        //else if( m_was_warn && m_mode != lm_warning )
        //{
        //    m_file<<  "</font>";

        //}
    }

    //{{ florist
    void logfile::CloseFile()
    {
        if( ! m_initialized ) return;

        m_file.close();
        m_initialized   = false;
        ZeroMemory( m_abCurMode, sizeof( m_abCurMode ) );
        //m_was_comm      = false;
        //m_was_info      = false;
        //m_was_error     = false;
        //m_was_warn      = false; 
    }

    void logfile::SetLogLevel( short sLevel )
    {
        cout << lbreak << L"Change LogLevel (from " << ms_sLevel << L" to " << sLevel << L" )." << lbreak;
        wprintf( L"Change LogLevel (from %d to %d ).\n", ms_sLevel, sLevel );

        ms_sLevel = sLevel;
    }

    void logfile::SetFileNameAtTime( bool bFileNameAtTime )     // true로 설정하면 log 폴더 밑에 로그파일을 쌓아둔다.
    {
        CloseFile();
        ms_bFileNameAtTime = bFileNameAtTime;
    }

    void logfile::OpenLogFile()
    {
        char szBuff[MAX_PATH] = {0};
        ::GetCurrentDirectoryA( MAX_PATH, szBuff );
        std::ostringstream stm;
        stm << szBuff << "\\";

        if( ms_bFileNameAtTime )
        {
            // 폴더가 없으면 만든다.
            MakeFolder( L"log" );
            stm << (CStringA)CTime::GetCurrentTime().Format(_T("log\\log_%Y.%m.%d_%H.%M.%S.htm"));
        }
        else
            stm << TB_LOGFILE_NAME;

        ms_strFileName = stm.str();
            
        // Set a New global locale
        //std::locale::global(utf8_locale);

        m_file.imbue( utf8_locale );
        m_file.open( ms_strFileName.c_str(), std::ios::binary | std::ios::out );
        std::wcout.imbue( utf16_locale );

    }

    void logfile::SetLogFileTitle( const wchar_t* szFileTitle )
    {
        logTitle = szFileTitle;
    }
    //}} florist

	//{{ 2013. 02. 15	최육사	로그 시스템 개선
	//#ifdef SERV_LOG_SYSTEM_NEW
	int logfile::GetLogFileSize()
	{
		m_file.seekp( 0, std::ios::end );

		// 파일 사이즈 리턴
		return m_file.tellp();
	}
	
	logfile& logfile::LogStat( const wchar_t* pStrFunctionName, const wchar_t* pStrFileName, const int iLineNum )
	{
		//{{ florist
		if( m_sLevel > ms_sLevel ) return *this;
		//}} florist

		std::wstringstream wstrStream;
		wstrStream << iLineNum;

		// 각 문자열을 조합해서 hash값을 얻자!
		std::wstring wstrHashKey;
		wstrHashKey += pStrFunctionName;
		wstrHashKey += pStrFileName;
		wstrHashKey += wstrStream.str();
		const unsigned int uiHashKey = std::hash_value( wstrHashKey );

		// 해당 해쉬키값으로 컨테이너를 조회한다.
		{
			// lock비용이 최소화가 되어야 한다!
			KLocker lock( m_csLogStat );

			std::map< unsigned int, KLogStat >::iterator mit;
			mit = m_mapLogStat.find( uiHashKey );
			if( mit == m_mapLogStat.end() )
			{
				// 구조체 생성 - 처음 찍혀지는 로그라면 여기서 insert가 될것이다.
				KLogStat kLogStat;
				kLogStat.m_wstrFunctionName = pStrFunctionName;
				kLogStat.m_wstrFileName = pStrFileName;
				kLogStat.m_wstrLineNum = wstrStream.str();
				kLogStat.m_iCount = 1;
				m_mapLogStat.insert( std::make_pair( uiHashKey, kLogStat ) );
			}
			else
			{
				// 카운트 1증가! - 두번째 이후로 찍히는 로그라면 find로 값 변경만 일어나므로 lock비용은 최소화 된다.
				++mit->second.m_iCount;
			}
		}

		return *this;
	}

	void logfile::GetLogStat( std::map< unsigned int, KLogStat >& mapLogStat ) const
	{
		mapLogStat.clear();
        
		{
			KLocker lock( m_csLogStat );

			mapLogStat = m_mapLogStat;
		}
	}

	void logfile::ClearLogStat()
	{
		KLocker lock( m_csLogStat );

		m_mapLogStat.clear();
	}
	//#endif SERV_LOG_SYSTEM_NEW
	//}}

    logfile& endl( logfile& l )
    {
        //l.m_file << "<BR>";
        l.m_file << L"\r\n";
        l.m_file.flush();
        std::wcout << L"\r\n";
        return l;
    }

    logfile& endp( logfile& l )
    {
        //l.m_file << "<p>";
        l.m_file << std::endl << std::endl;
        l.m_file.flush();
        std::wcout << std::endl << std::endl;
        return l;
    }

    logfile& tab( logfile& l )
    {
        //l.m_file << "&nbsp;&nbsp;&nbsp;&nbsp;";
        l.m_file << L"    ";
        l.m_file.flush();
        std::wcout << L"    ";
        return l;
    }

    logfile& lbreak( logfile& l )
    {
        l.m_file << L"<hr>";
        l.m_file.flush();
        std::wcout << L"\r\n-----------------------------------------------\r\n";
        return l;
    }
} // dbg



