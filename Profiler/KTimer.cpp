/** @file   KTimer.cpp
    @desc   implementation of KTimer
*/

#define _TOSTR1(x)    #x
#define _TOSTR(x)     _TOSTR1(x)
#include _TOSTR(PROJECT_PCH)
#include "KTimer.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")


///-----------------------------------------------------------------------------
//
KTimer::KTimer()
{
    m_bIsRunning    = FALSE;
    m_llTime        = 0;
    m_llBaseTime    = 0;
    m_llStopTime    = 0;
    m_llLastTime    = 0;
    m_llCurrentTime = 0;

    LARGE_INTEGER qwTicksPerSec;

    m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
    if ( m_bUsingQPF )
        m_llTicksPerSec = qwTicksPerSec.QuadPart;
    else
        m_llTicksPerSec = 1000;

    m_dSecsPerTick = 1./double( m_llTicksPerSec );
}//KTimer::KTimer()


INT64 KTimer::_GetLLRealTime()
{
    LARGE_INTEGER   qwTime;

    if ( m_bUsingQPF )
    {
        QueryPerformanceCounter( &qwTime );
        m_llTime = qwTime.QuadPart;
    }
    else
    {
        DWORD   dwNew;

        dwNew = timeGetTime();
        qwTime.QuadPart = m_llTime;
        if ( dwNew < qwTime.LowPart )
            qwTime.HighPart++;
        qwTime.LowPart = dwNew;
        m_llTime = qwTime.QuadPart;
    }//if.. else..

    return m_llTime;
}//KTimer::AdjustTime()


void KTimer::Reset()
{
    INT64   llTime = _GetLLPseudoTime();

    m_llLastTime = m_llCurrentTime = m_llBaseTime = llTime;
}//KTimer::Reset()


void KTimer::Start()
{
    if ( !m_bIsRunning )
    {
        INT64    timediff = _GetLLRealTime() - m_llStopTime;

        m_llBaseTime    += timediff;
        m_llLastTime    += timediff;
        m_llCurrentTime += timediff;
        m_bIsRunning    = TRUE;
    }//if
}//KTimer::Start()


void KTimer::Stop()
{
    if ( m_bIsRunning )
    {
        INT64   llRealTime = _GetLLRealTime();

        m_llStopTime = llRealTime;
        m_bIsRunning = FALSE;
    }//if
}//KTimer::Stop()
