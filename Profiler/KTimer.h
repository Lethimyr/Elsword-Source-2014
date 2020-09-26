/** @file   KTimer.h
    @brief  interface of KTimer
    @author seojt@kogstudios.com
    @since  started on December 13, 2001

    @date   robobeg@kogstudios.com on June 9, 2003
            - provide uniform sematics for both pc and xbox.
*/
#pragma once

#if !defined(_TIMER_H)
#define _TIMER_H

//#include "KJohnGen.h"


///-----------------------------------------------------------------------------
/// @class  KTimer
/// @brief  general purpose timer
///
class KTimer
{
private:
    BOOL    m_bIsRunning;
    BOOL    m_bUsingQPF;
    INT64   m_llTime;
    INT64   m_llStopTime;
    INT64   m_llBaseTime;
    INT64   m_llCurrentTime;
    INT64   m_llLastTime;
    INT64   m_llTicksPerSec;
    double  m_dSecsPerTick;

public:
    /// constructor.
    /// initially the app time is 0 and the timer is stopped.
    KTimer();

private:
    INT64 _GetLLRealTime();

    inline INT64 _GetLLPseudoTime()
    {
        return m_bIsRunning ? _GetLLRealTime() : m_llStopTime;
    }//_GetLLPseudoTime()

public:
    /// check whether timer is running.
    inline BOOL IsStarted() const { return m_bIsRunning; }

    /// resets the timer to 0 and lets the timer keep stopped or running.
    void Reset();

    /// starts the stopped timer. does not affect the timer if it is running.
    void Start();

    /// stops the running timer. does not affect the timer if it is stopped.
    void Stop();

    /// gets the absolute time of the system.
    inline float GetAbsoluteTime()
    {
        return float( _GetLLRealTime() * m_dSecsPerTick );
    }//GetAbsoluteTime()

    inline void UpdateTime()
    {
        if ( m_bIsRunning )
        {
            m_llLastTime    = m_llCurrentTime;
            m_llCurrentTime = _GetLLRealTime();
        }//if
    }//UpdateTime()

    /// get real current time of this function call.
    inline float GetActualCurrentTime()
    {
        return float( ( _GetLLRealTime() - m_llBaseTime ) * m_dSecsPerTick );
    }//GetActualCurrentTime()

    /// get (current) time of UpdateTime() called.
    inline float GetCurrentTime()
    {
        return float( ( m_llCurrentTime - m_llBaseTime ) * m_dSecsPerTick );
    }//GetCurrentTime()

    /// get elapsed time after last UpdateTime() call.
    inline float GetElapsedTime()
    {
        return float( ( m_llCurrentTime - m_llLastTime ) * m_dSecsPerTick );
    }//GetElapsedTime();

    inline void AdvanceTime(float fAdvanceTime)
    {
        m_llBaseTime -= ( INT64 ) ( fAdvanceTime / m_dSecsPerTick );
    }//AdvanceTime()

    inline void SetTime(float fSetTime)
    {
        m_llBaseTime = m_llCurrentTime - (INT64)( fSetTime / m_dSecsPerTick );
    }//SetTime()
};//class KTimer

#endif // !defined(_TIMER_H)


/** @example    KTimer
    @code

    #pragma warning(disable: 4995)
    #include "KJohnGen.h"
    #include <iostream>
    #include <mmsystem.h>
    #include <conio.h>
    #include "KTimer.h"

    #pragma comment(lib, "winmm.lib")

    using namespace std;

    void main()
    //void main(int argc, char* argv[])
    {
        INIT_CRTDEBUG();

        KTimer  timer;
        int     iCounter = 0;
        timer.Start();
        while ( 1 )
        {
            timer.UpdateTime();
            ++iCounter;
            Sleep( 1000 );
            printf( "%05d: abs=%g, actual cur=%g, cur=%g, elapsed=%g\n",
                iCounter,
                timer.GetAbsoluteTime(),        // absolute time
                timer.GetActualCurrentTime(),   // real current time
                timer.GetCurrentTime(),         // current time of UpdateTime() called
                timer.GetElapsedTime() );       // elapsed time after last UpdateTime() call
            if ( kbhit() )
                break;
        }//while
        timer.Stop();
        getch();
    }//main()
    // console oupput:
    //
    //00001: abs=37696, actual cur=0.999627, cur=7.30304e-007, elapsed=7.30304e-007
    //00002: abs=37697, actual cur=1.99963, cur=1.0002, elapsed=1.0002
    //00003: abs=37698, actual cur=2.99963, cur=1.99979, elapsed=0.999594
    //00004: abs=37699, actual cur=3.99964, cur=2.99979, elapsed=0.999997
    //00005: abs=37700, actual cur=4.99964, cur=3.99979, elapsed=1
    //00006: abs=37701, actual cur=5.99965, cur=4.99982, elapsed=1.00003
    //00007: abs=37702, actual cur=6.99965, cur=5.99981, elapsed=0.999996
    //00008: abs=37703, actual cur=7.99965, cur=6.99982, elapsed=1
    //00009: abs=37704, actual cur=8.99966, cur=7.99982, elapsed=1
    //00010: abs=37705, actual cur=9.99966, cur=8.99982, elapsed=1
    //Press any key to continue

    @endcode
*/
