/** @file   KJohnGen.h
	@brief  interface of KJohnGen
	@author seojt@kogstudios.com
    @date   seojt on 2005-5-2, 11:42:04
            - UNICODE compatible
    @date   seojt on 2005-07-06
            - ARRAY_SIZE
    @date   seojt on Thu, 2005-10-27
            - ASSERT_FLOAT()
    @date   seojt on 2006-1-12, 14:33:14
            - D3D_DEBUG_INFO
    @date   seojt on 2006-2-1, 10:09:29
            - NONCOPYABLE()
    @date   seojt on 2006-7-13, 14:30:50
            - ASSERT_HEAP()
*/
#pragma once

#ifndef _KJOHNGEN_H
#define _KJOHNGEN_H

//#pragma warning(push)
#pragma warning(disable:4503) // decorated name length exceeded, name was truncated
#pragma warning(disable:4786) // identifier was truncated to '255' characters in the debug information
#pragma warning(disable:4788) // identifier was truncated to 'number' characters
#pragma warning(disable:4995) // 'function': name was marked as #pragma deprecated
#pragma warning(disable:4702) // unreachable code
//{{ robobeg : 20060-09-13 // 11:34
#pragma warning(disable:4127) // conditional expression is constant
//}} robobeg : 20060-09-13 // 11:34

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // #ifndef WIN32_LEAN_AND_MEAN

//#include <windows.h>
//#include <crtdbg.h>
//#include <strsafe.h>
#ifdef _DEBUG // 2006-1-12, 14:32:55
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif // #ifndef D3D_DEBUG_INFO
#endif // _DEBUG
//#include <d3dx9.h> // KJohnGen.h uses DirectX9 library if required


//------------------------------------------------------------------------------
// _DEBUG
//
#if defined(_DEBUG)

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp_)         _ASSERTE(exp_)
#endif // #if !defined(ASSERT)

// TRACE
//
#if !defined(TRACE)
    inline void _tracew(const WCHAR* fmt_, ...)
    {
        WCHAR   out[4096];
        va_list argList;

        va_start( argList, fmt_ );
        StringCchVPrintfW( out, 4095, fmt_, argList );
        va_end( argList );
        OutputDebugStringW( out );
    }//_tracew()

    inline void _tracea(const char* pszFormat_, ...)
    {
        char    out[4096];
        va_list argList;

        va_start( argList, pszFormat_ );
        StringCchVPrintfA( out, 4095, pszFormat_, argList );
        va_end( argList );
        OutputDebugStringA( out );
    }//_tracea()
    #define TRACEA              _tracea
    #define TRACEW              _tracew

#ifdef UNICODE // 2005-5-2, 11:40:58
#define TRACE               _tracew
#else
#define TRACE               _tracea
#endif // #ifdef UNICODE // 2005-5-2, 11:40:58
#endif // #if !defined(TRACE)

/** @example    BREAK()
    BREAK( i == 3 );
*/
#ifndef BREAK
#define BREAK(cond_)         if (cond_) __asm int 3
#endif // #ifndef BREAK

/** @example    CASSERT()
    CASSERT( sizeof(int) == 4 );
*/
#ifndef CASSERT
#define CASSERT(expn_)       typedef char __C_ASSERT__[(expn_)?1:-1]
#endif // #ifndef CASSERT

#else // not defined _DEBUG ////////////////////////////////////////////////////

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp)
#endif // #if !defined(ASSERT)

// TRACE
//
#if !defined(TRACE)
    #define TRACE               1 ? (void)0 :
    #define TRACEA              1 ? (void)0 :
    #define TRACEW              1 ? (void)0 :
#endif // #if !defined(TRACE)

#ifndef BREAK
#define BREAK(cond_)
#endif // #ifndef BREAK

#ifndef CASSERT
#define CASSERT(expn_)
#endif // #ifndef CASSERT

#endif // defined(_DEBUG)
//
//------------------------------------------------------------------------------


inline void _traceAlways(const char* pszFormat_, ...)
{
    char    out[4096];
    va_list argList;

    va_start( argList, pszFormat_ );
    StringCchVPrintfA( out, 4095, pszFormat_, argList );
    va_end( argList );
    OutputDebugStringA( out );
}//_traceAlways()
#define TRACE_ALWAYS    _traceAlways


// VERIFY
//
#if !defined(VERIFY)
#ifdef _DEBUG
#define VERIFY(exp_)         _ASSERTE(exp_)
#else
#define VERIFY(exp_)         (exp_)
#endif
#endif // #if !defined(VERIFY)


//{{ robobeg : 2006-08-02 // 10:53
//------------------------------------------------------------------------------
// KASSERT, KVERIFY, KVERIFYRET
//

#ifdef _DEBUG
#ifndef KASSERT
#define KASSERT( expr, msg ) \
    if ( !(expr) ) \
		_CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, __FUNCDNAME__, (msg) );
#endif // #ifndef KASSERT

#ifndef KVERIFY
#define KVERIFY( expr, msg )	KASSERT( (expr), (msg) )
#endif // #ifndef KVERIFY

#ifndef KVERIFYRET
#define KVERIFYRET( expr, ret, msg ) \
    if ( !(expr) && \
        ( 1 == _CrtDbgReport( _CRT_ASSERT, \
        __FILE__, __LINE__, __FUNCDNAME__, (msg) ) ) ) \
    { \
        \
        return (ret); \
    }
#endif // #ifndef KVERIFYRET
#else
#ifndef KASSERT
#define KASSERT( expr, msg )
#endif // #ifndef KASSERT
#ifndef KVERIFY
#define KVERIFY( expr, msg ) (expr)
#endif // #ifndef KVERIFY
#ifndef KVERIFYRET
#define KVERIFYRET( expr, ret, msg ) \
    if ( !(expr) ) \
        return (ret);
#endif // #ifndef KVERIFYRET
#endif // #ifdef _DEBUG
//}} robobeg : 2006-08-02 // 10:53


// SAFE_DELETE
//
#ifndef SAFE_DELETE
#define SAFE_DELETE(p_)       { if(p_) { delete (p_); (p_)=NULL; } }
#endif


// SAFE_DELETE_ARRAY
//
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p_) { if(p_) { delete[] (p_); (p_)=NULL; } }
#endif


// SAFE_RELEASE
//
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p_)      { if(p_) { (p_)->Release(); (p_)=NULL; } }
#endif


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a_)       (sizeof(a_)/sizeof((a_)[0]))
#endif // #ifndef ARRAY_SIZE


#ifndef LINE1
#define LINE1(x_) #x_
#endif // #ifndef LINE1
#ifndef LINE
#define LINE(x_) LINE1(x_)
#endif // #ifndef LINE
/** @example    TODO()

    #pragma TODO( "fix here" )
    #pragma NOTE( "check here" )

*/
#ifndef TODO
#define TODO(msg_) message ( __FILE__ "(" LINE(__LINE__)  "): [TODO] " msg_ )
#endif // #ifndef TODO
#ifndef NOTE
#define NOTE(msg_) message ( __FILE__ "(" LINE(__LINE__)  "): [NOTE] " msg_ )
#endif // #ifndef NOTE


/** @example ASSERT_FLOAT()
    D3DXVECTOR3 v;
    // ...
    D3DXVec3Normalize( &v );
    ASSERT_FLOAT( v.x );
*/
#ifndef ASSERT_FLOAT
#ifdef _DEBUG
    #define ASSERT_FLOAT(f_) if ( f_ >= 0 ){} else if ( f_ < 0 ){} else { assert( f_ && !"float error" ); }
#else // #ifdef _DEBUG
    #define ASSERT_FLOAT(f_)
#endif // _DEBUG
#endif // #ifndef ASSERT_FLOAT


//------------------------------------------------------------------------------
// CRT debug macros
//
//#define INIT_CRTDEBUG()     _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF)
#ifndef INIT_CRTDEBUG
#define INIT_CRTDEBUG()     _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_DELAY_FREE_MEM_DF)
#endif // #ifndef INIT_CRTDEBUG


/** @example    ASSERT_MEMORY()
	@code

    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        INIT_CRTDEBUG();

        int*    p = new int;
        delete p;

        *p = 3;             // using after deletion
        ASSERT_MEMORY();    // see and check call stack window
        delete p;
    }//main()

	@endcode
*/
#ifndef ASSERT_MEMORY
#define ASSERT_MEMORY()     ASSERT( _CrtCheckMemory() )
#endif // #ifndef ASSERT_MEMORY


/** @example    BREAK_ALLOC()
	@code

  //------------------------------------------------------------------------------
  //
  1) memory leak example:

    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        INIT_CRTDEBUG();

        int*    p = new int;

        *p = 3;
    }//main()

    // debug message:
    Detected memory leaks!
    Dumping objects ->
    {44} normal block at 0x00372DA8, 4 bytes long.
    Data: <    > 03 00 00 00
    Object dump complete.

    // @note    allocation number is {44}, so you can set break point at 44th alloc
    //          see below example


  //------------------------------------------------------------------------------
  //
  2) break on mal-alloc {44}:

    #ifdef PROJECT_PCH
    #define _TOSTR1(x)    #x
    #define _TOSTR(x)     _TOSTR1(x)
    #include _TOSTR(PROJECT_PCH)
    #endif // PROJECT_PCH
    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        BREAK_ALLOC( 44 ); // install break CRT point condition
        INIT_CRTDEBUG();

        int*    p = new int;

        *p = 3;
    }//main()

	@endcode
*/
#ifndef BREAK_ALLOC
#define BREAK_ALLOC(line_)   _CrtSetBreakAlloc( line_ )
#endif // #ifndef BREAK_ALLOC


/** @example ASSERT_HEAP()
    @code

    #include "KJohnGen.h"

    void main()
    {
        int* p = new int;
        int i;
        ASSERT_HEAP( p );
        delete p;
        ASSERT_HEAP( p ); // assertion fails
        ASSERT_HEAP( &i ); // assertion fails
    }//main()

    @endcode
*/
#ifndef ASSERT_HEAP
#define ASSERT_HEAP(p)          ASSERT( _CrtIsValidHeapPointer(p) );
#endif // #ifndef ASSERT_HEAP


#ifndef NONCOPYABLE
#define NONCOPYABLE(name_)      name_(const name_ &); const name_ & operator=(const name_ &);
#endif // NONCOPYABLE

//#pragma warning(pop)

#endif // _KJOHNGEN_H