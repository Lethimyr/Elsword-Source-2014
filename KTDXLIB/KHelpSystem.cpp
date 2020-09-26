/** @file   KHelpSystem.cpp
	@brief  implementation of KHelpSystem
*/

//#define _TOSTR1(x)    #x
//#define _TOSTR(x)     _TOSTR1(x)
//#include _TOSTR(PROJECT_PCH)
#include "StdAfx.h"
#pragma warning(push)
#pragma warning(disable: 4995)
#include "KHelpSystem.h"


//------------------------------------------------------------------------------
//
// constructor.
KHelpSystem::KHelpSystem()
{
    OutputDebugStringA( "[KHelpSystem] KHelpSystem()\n" );
    _Init();
}//KHelpSystem::KHelpSystem()


// destructor.
KHelpSystem::~KHelpSystem()
{
    OutputDebugStringA( "[KHelpSystem] ~KHelpSystem()\n" );
}//KHelpSystem::~KHelpSystem()


void KHelpSystem::_Init()
{
    m_iCursor = 0;
    m_bShowHelp = false;
}//KHelpSystem::_Init()


void KHelpSystem::Clear()
{
    m_vecHelps.clear();
    m_iCursor = 0;
}//KHelpSystem::Clear()


bool KHelpSystem::AddHelpObject(const char* szHelpTypeName, HELP_SYSTEM_CALLBACK fpCallback)
{
    KHelpNode   node;

    StringCchCopyA( node.m_szHelpTypeName, ARRAY_SIZE( node.m_szHelpTypeName ), szHelpTypeName );
    node.m_fpCallback = fpCallback;

    m_vecHelps.push_back( node );

#ifdef _DEBUG // 2005-9-5, 14:45:38
    char buffer[256];
    StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "[KHelpSystem] AddHelpObject(): %s: callback=%p, size=%i\n",
        szHelpTypeName, fpCallback, m_vecHelps.size() );
    OutputDebugStringA( buffer );
#endif // _DEBUG

    return true;
}//KHelpSystem::AddHelpObject()


bool KHelpSystem::DisplayHelp(void* pParam/*=NULL*/)
{
    if ( m_bShowHelp == false )
        return false;

    if ( m_iCursor >= 0 && m_iCursor < (int) m_vecHelps.size() )
    {
        HELP_SYSTEM_CALLBACK    fpCallback;
        fpCallback = m_vecHelps[m_iCursor].m_fpCallback;
        fpCallback( pParam );
        return true;
    }//if

    return false;
}//KHelpSystem::DisplayHelp()


void KHelpSystem::NextHelp()
{
    m_iCursor += 1;
#ifdef _DEBUG // 2005-9-5, 14:45:26
    char    buffer[256];
    StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "[KHelpSystem] m_vecHelps.size()=%i\n", m_vecHelps.size() );
    OutputDebugStringA( buffer );
#endif // _DEBUG
    if ( m_iCursor >= (int)m_vecHelps.size() )
    {
        m_iCursor = 0;
    }//if
}//KHelpSystem::NextHelp()


void KHelpSystem::PrevHelp()
{
    m_iCursor -= 1;
    if ( m_iCursor < 0 )
    {
        m_iCursor = m_vecHelps.size() - 1;
    }//if
}//KHelpSystem::PrevHelp()


const char* KHelpSystem::CurrentHelpName()
{
    if ( m_iCursor < 0 || m_iCursor >= (int) m_vecHelps.size() )
        return NULL;

    return m_vecHelps[m_iCursor].m_szHelpTypeName;
}//KHelpSystem::CurrentHelpName()


bool KHelpSystem::SetCurrentHelp(const char* pszHelpTypeName)
{
    if ( NULL == pszHelpTypeName )
        return false;

    for (size_t i=0; i < m_vecHelps.size(); ++i)
    {
        if ( 0 == _stricmp(m_vecHelps[i].m_szHelpTypeName,pszHelpTypeName) )
        {
            m_iCursor = i;
            return true;
        }//if
    }//for

    return false;
}//KHelpSystem::SetCurrentHelp()


//------------------------------------------------------------------------------
// debug function(s)
//
void KHelpSystem::Debug_Dump()
{
    char buffer[256];
    HELP_VECTOR::iterator itor;

    StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "[KHelpSystem::Debug_Dump] size=%i\n", m_vecHelps.size() );
    OutputDebugStringA( buffer );
    itor = m_vecHelps.begin();
    while ( itor != m_vecHelps.end() )
    {
        StringCchPrintfA( buffer, ARRAY_SIZE( buffer ), "\t HelpSystem=%s, callback=%p\n",
            itor->m_szHelpTypeName, itor->m_fpCallback );
        OutputDebugStringA( buffer );

        ++itor;
    }//while
}//KHelpSystem::Debug_Dump()

#pragma warning(pop)
