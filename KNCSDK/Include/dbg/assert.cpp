//////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Triple Buffer Software 2003 All rights reserved                                        //
//                                                                                                  //
// This is free sour code. You may check the terms and conditions for free source code usage at:    //
//                                                                                                  //
// http://triplebuffer.devmaster.net/license.php                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "assert.hpp"

#pragma warning( push )
#pragma warning( disable : 4355 )

#undef m_custom_assert_a
#undef m_custom_assert_b

#include "KncUtil.h"
namespace dbg
{
    
    custom_assert cassert;

    custom_assert::custom_assert()
        : m_custom_assert_a(*this), m_custom_assert_b(*this)
    {
    }

    custom_assert& custom_assert::operator () ( const wchar_t* expr, unsigned int line, const wchar_t* func, const wchar_t* file )
    {
        m_expression = expr;
        m_filename = file;
        m_line = line;

        cerr<<  L"<u>Assert failure</u> in " ;
        cerr<<  L"<a href=\"" << file<< L"\">";
        cerr.line_file_func( line, file, func )<< L"</a>. Expr is \""<< expr << L"\""<< endl;
        return *this;
    }

    custom_assert& custom_assert::operator () ( const char* expr, unsigned int line, const wchar_t* func, const wchar_t* file )
    {
        m_expression = KncUtil::toWideString( expr, strlen( expr) );
        m_filename = file;
        m_line = line;

        cerr<<  L"<u>Assert failure</u> in " ;
        cerr<<  L"<a href=\"" << file<< L"\">";
        cerr.line_file_func( line, file, func )<< L"</a>. Expr is \""<< expr << L"\""<< endl;
        return *this;
    }

    custom_assert& custom_assert::msg( const wchar_t* msg )
    {
        cerr<< tab<< L"[Msg] "<< msg<< endl;
        return *this;
    }

    custom_assert& custom_assert::msg( const char* msg )
    {
        std::wstring strTmp = KncUtil::toWideString( msg, strlen(msg) );
        cerr<< tab << L"[Msg] "<< strTmp.c_str() << endl;
        return *this;
    }

    void custom_assert::fatal()
    {
        cerr << L"<center>F A T A L</center>"<< endl;

#ifdef _DEBUG
        _assert( KncUtil::toNarrowString( m_expression.c_str(),m_expression.length()).c_str(), KncUtil::toNarrowString(m_filename.c_str(),m_filename.length()).c_str(), m_line );
//        _assert( m_expression.c_str(), m_filename, m_line );
#endif

    }


} // game

#pragma warning( pop )