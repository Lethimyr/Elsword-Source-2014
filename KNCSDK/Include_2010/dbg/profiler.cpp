//////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright Triple Buffer Software 2003 All rights reserved                                        //
//                                                                                                  //
// This is free sour code. You may check the terms and conditions for free source code usage at:    //
//                                                                                                  //
// http://triplebuffer.devmaster.net/license.php                                                    //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma  warning ( disable : 4996 )
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include "logfile.hpp"
#include "profiler.hpp"

namespace dbg
{
    namespace 
    {                
		const wchar_t* const errorCol		=	L"#FF0000";

		const wchar_t* const infoCol		=	L"#C1F0FF";
		const wchar_t* const infoFont		=	L"courier new";

		const wchar_t* const pageCol		=	L"#000000";			// Default page color
		const wchar_t* const linkCol		=	L"#FFFFFF";			// Default colors for links
		const wchar_t* const visitedCol	=	L"#FFFF97";			// Default color for visited links
		const wchar_t* const titleCol		=	L"#FFFF97";			// Color of title
		const wchar_t* const logTitle		=	L"GameLib Profiler";	// Title
    } 

    //Clock* profiler::mpClock = NULL;
    profiler::profile_info* profiler::m_root = NULL;
    profiler::profile_info* profiler::m_current = NULL;
    i64 profiler::m_frequency = 0;

    void profiler::start( const wchar_t* name )
    {
#ifdef TB_USE_PROFILER
        // Check if it's already there
        profile_info::hash_t::iterator it = m_current->children.find( name );
        
        LARGE_INTEGER li;
        QueryPerformanceCounter( &li );

        if( it != m_current->children.end() )
        {
            it->second->start_time = static_cast<float>(li.QuadPart) / static_cast<float>(m_frequency);
            it->second->parent = m_current;
            m_current = it->second;
        }
        else
        {
            // Create a new one
            m_current->children[name] = new profiler::profile_info;
            profiler::profile_info* pinfo = m_current->children[name];

            pinfo->total_time = 0.0f; 
            pinfo->start_time = static_cast<float>(li.QuadPart) / static_cast<float>(m_frequency);
            pinfo->num_profiles = 0;
            pinfo->parent = m_current;

            m_current = pinfo;
        }
#endif // TB_USE_PROFILER
    }

    void profiler::end()
    {
#ifdef TB_USE_PROFILER
        LARGE_INTEGER li;
        QueryPerformanceCounter( &li );

        float elapsed_time = static_cast<float>(li.QuadPart) / static_cast<float>(m_frequency) - m_current->start_time;

        m_current->total_time += elapsed_time;

        if( m_current->num_profiles == 0 )
        {
            m_current->max_time = m_current->min_time = elapsed_time;
        }
        else
        {
            if( m_current->max_time < elapsed_time )
                m_current->max_time = elapsed_time;

            if( m_current->min_time > elapsed_time )
                m_current->min_time = elapsed_time;
        }
        
        ++m_current->num_profiles;

        m_current = m_current->parent;
#endif // TB_USE_PROFILER
    }

    profiler::profiler( const wchar_t* name )
    {
        profiler::start( name );
    }

    profiler::~profiler()
    {
        profiler::end();
    }

    void profiler::init_profiler()
    {
#ifdef TB_USE_PROFILER
        LARGE_INTEGER li;

        if( !QueryPerformanceFrequency( &li ) )
        {
            throw std::runtime_error( "Performace timer not supported (QPC/QPF)" );
        }

        m_frequency = li.QuadPart;

        if( !m_root )
        {
            m_root = new profiler::profile_info;
            m_current = m_root;
            m_root->parent = 0;
        }
#endif // TB_USE_PROFILER
    }


    void profiler::kill_hash_map( profile_info::hash_t* pmap )
    {
        for( profile_info::hash_t::iterator it = pmap->begin();
            it != pmap->end();
            it++ )
        {
            if( it->second->children.size() )
                profiler::kill_hash_map( &it->second->children );

            delete it->second;
        }

        pmap->clear();
    }

    void profiler::output_profile_info( profile_info::hash_t* pmap, std::ofstream& outfile, profile_info* parent )
    {

        for( profile_info::hash_t::iterator it = pmap->begin();
            it != pmap->end();
            it++ )
        {
            wchar_t buffer[32];        
            std::wstring fonts = L"<font face=\"";
            fonts += infoFont;
            fonts += L"\" size=\"";
            fonts += _itow( 2, buffer, 10 );
            fonts += L"\" color=\"";
            fonts += errorCol;
            fonts += L"\">";

            wchar_t buffer2[32];
            std::wstring fonts2 = L"<font face=\"";
            fonts2 += infoFont;
            fonts2 += L"\" size=\"";
            fonts2 += _itow( 2, buffer2, 10 );
            fonts2 += L"\" color=\"";
            fonts2 += infoCol;
            fonts2 += L"\">";

            outfile<< L"<tr><td>";
            outfile<< fonts.c_str()<< it->first;
            outfile<< L"</td><td>";
            outfile<< fonts2.c_str()<< L"<b>AVG:</b></font> "<< fonts2.c_str()<< it->second->total_time / it->second->num_profiles;
            outfile<< L"</td><td>";
            outfile<< fonts2.c_str()<< L"<b>MAX:</b></font> "<< fonts2.c_str()<< it->second->max_time;
            outfile<< L"</td><td>";
            outfile<< fonts2.c_str()<< L"<b>MIN:</b></font> "<< fonts2.c_str()<< it->second->min_time;
            outfile<< L"</td><td>";

            if( parent )
            {
                float avg_of_parent = 0;
                avg_of_parent = 100.0f / parent->total_time;
                avg_of_parent *= it->second->total_time;

                outfile<< fonts2.c_str()<< L"<b>AVG% of Parent:</b></font> "<< fonts2.c_str()
                    << static_cast<int>(avg_of_parent)<< L"%";
            }
            else
            {
                outfile<< fonts2.c_str()<< L"<b>AVG% of Parent:</b></font> "<< fonts2.c_str()<< L"No Parent";
            }

            outfile<< L"</td></tr>";

            if( it->second->children.size() )
            {
                outfile<< L"<tr><td></td><td colspan=4><table border=1 cellpadding=3 bordercolorlight=#707070 bordercolordark=#444444>";
                profiler::output_profile_info( &it->second->children, outfile, it->second );
                outfile<< L"</table></td></tr>";
            }
        }
    }

    void profiler::kill_profiler( const wchar_t* output_file )
    {
#ifdef TB_USE_PROFILER
        if( m_root )
        {
            if( m_root->children.size() )
            {
                if( output_file )
                {
                    std::string strOutFile = toNarrowString( output_file, wcslen( output_file ) );
                    std::ofstream outfile;
                    outfile.open( output_file );

                    if( !outfile )
                    {
                        cerr<< L"[profiler::kill_profiler] Failed to output profile information to file"<< endl;
                    }
                    else
                    {
                        outfile<< L"<html> <HEAD> <CENTER>\
                            <TITLE>"<< logTitle<< L"</TITLE>\
				            <H3><font color=\""<< titleCol<< L"\">"\
					            << logTitle<< L"</font></H3>\
				            </CENTER> </HEAD>\
				            <BODY link=\""<< linkCol<< L"\" vlink=\""<< visitedCol\
				            << L"\" alink=\""<< linkCol<< L"\" bgcolor=\""<< pageCol<< L"\"><BR><BR>";

                        outfile<< L"<table border=1 cellpadding=3 bordercolorlight=#707070 bordercolordark=#444444>";
                        profiler::output_profile_info( &m_root->children, outfile, 0 );
                        outfile<< L"</table>";
                    }
                }
                else
                {
                    cwarn<< L"[profiler::kill_profiler] Profile output filename not specified"<< endl;
                }

                profiler::kill_hash_map( &m_root->children );
            }

            delete m_root;
            m_root = 0;
            m_current = 0;
        }
#endif // TB_USE_PROFILER
    }

} // dbg
