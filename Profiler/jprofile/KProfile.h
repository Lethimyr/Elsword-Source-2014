/** @file   KProfile.h
    @brief  interface of KProfile
    @author seojt@kogstudios.com
    @since  2006-1-12, 18:37:06

    @date   seojt on 2006-4-24, 14:35:32
            - AccessTimer()
*/
#pragma once

#ifndef _KProfile_H
#define _KProfile_H

//#include "../KJohnGen.h"
//#include "../KTimer.h"


///-----------------------------------------------------------------------------
/// @class  KProfile
///
class KProfile
{
protected:
    static KTimer   s_timer;

public:
    static KTimer& AccessTimer() { return s_timer; }

protected:
    float   m_fStartTime;
    float   m_fStopTime;

public:
    /// constructor.
    KProfile()
    {
        if ( FALSE == s_timer.IsStarted() )
            s_timer.Start();
    }//KProfile()

    /// destructor.
    ~KProfile()
    {
    }//~KProfile()

    /// save begin profile time.
    void Begin()
    {
        m_fStartTime = s_timer.GetActualCurrentTime();
    }//Begin()

    /// save end profile time.
    float End()
    {
        m_fStopTime = s_timer.GetActualCurrentTime();
        return m_fStopTime - m_fStartTime;
    }//End()

    /// get profile time.
    float GetTime() const { return m_fStopTime - m_fStartTime; }
};//class KProfile

#endif // _KProfile_H


/** @example    KProfile
    @code

    #pragma warning(disable: 4995)
    #include "KJohnGen.h"
    #include <iostream>
    #include <mmsystem.h>
    #include <conio.h>
    #include "KTimer.h"
    #include "./jprofile/KBlockProfile.h"

    #pragma comment(lib, "winmm.lib")

    using namespace std;

    void main()
    //void main(int argc, char* argv[])
    {
        INIT_CRTDEBUG();

        KProfile    profile;
        KTimer      timer;
        int         iCounter = 0;

        timer.Start();
        while ( 1 )
        {
            timer.UpdateTime();
            ++iCounter;

            Sleep( 1000 );
            
            profile.Begin();
            printf( "%05d: abs=%g, actual cur=%g, cur=%g, elapsed=%g\n",
                iCounter,
                timer.GetAbsoluteTime(),        // absolute time
                timer.GetActualCurrentTime(),   // real current time
                timer.GetCurrentTime(),         // current time of UpdateTime() called
                timer.GetElapsedTime() );       // elapsed time after last UpdateTime() call
            profile.End();
            TRACEA( "profile time=%g\n", profile.GetTime() );
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
