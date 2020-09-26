/** @file   AssertTrace.h
    @desc   interface of AssertTrace
    @author seojt@kogstudios.com
    @since  2008-1-5, 17:13
*/

#ifndef _ASSERTTRACE_H
#define _ASSERTTRACE_H

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable: 4995) // 'vsprintf': name was marked as #pragma deprectaed

#ifdef _DEBUG // 2008-1-5
    #ifndef ASSERT
        #define ASSERT(pred_)    assert(pred_)
    #endif // ASSERT
    #if !defined(TRACEA)
        inline void _tracea2(const char* pszFormat_, ...)
        {
            char    out[4096];
            va_list argList;

            va_start( argList, pszFormat_ );

#ifdef	CONVERSION_VS
			vsprintf_s( out, 4096, pszFormat_, argList );
#else	CONVERSION_VS
			vsprintf( out, pszFormat_, argList );
#endif	CONVERSION_VS

            va_end( argList );
            OutputDebugStringA( out );
        }//_tracea2()
        #define TRACEA              _tracea2
    #endif
#else
    #ifndef ASSERT
        #define ASSERT(pred_)
    #endif // ASSERT
    #if !defined(TRACEA)
        #define TRACEA              1 ? (void)0 :
    #endif //#if !defined(TRACE)
#endif // _DEBUG // 2008-1-5

// VERIFY
//
#if !defined(VERIFY)
    #ifdef _DEBUG
        #define VERIFY(exp_)         assert(exp_)
    #else
        #define VERIFY(exp_)         (exp_)
    #endif
#endif // #if !defined(VERIFY)


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

#pragma warning(pop)

#endif // _ASSERTTRACE_H
