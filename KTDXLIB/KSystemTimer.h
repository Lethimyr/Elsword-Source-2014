/** @file   KSystemTimer.h
    @desc   interface of KSystemTimer
    @author robobeg@kogstudios.com
    @since  2008-02-12

    @date   jintaeks on 2008-03-04, 9:27
            - OnetimeFirstCall() ¼öÁ¤.
*/

#pragma once

#include <windows.h>
#include <mmsystem.h>


#define KSYSTEMTIMER_TARGET_RESOLUTION      1


///-----------------------------------------------------------------------------
/// @class  KSystemTimer
/// @brief  
///
class   KSystemTimer
{
public:
    static bool     ms_bInitialized;
    static UINT     ms_uiTimerRes;

    static void     OnetimeFirstCall()
    {
        TIMECAPS tc;

		if ( ::timeGetDevCaps(&tc, sizeof(TIMECAPS)) == TIMERR_NOERROR )
        {
            ms_uiTimerRes = __min(__max(tc.wPeriodMin, KSYSTEMTIMER_TARGET_RESOLUTION), tc.wPeriodMax);
			::timeBeginPeriod( ms_uiTimerRes );

            //timeBeginPeriod( 1 );
            ms_bInitialized = true;
        }
    }//OnetimeFirstCall()

    static void     OnetimeLastCall()
    {
		::timeEndPeriod( ms_uiTimerRes );
        ms_bInitialized = false;
    }//OnetimeLastCall()

    static DWORD    GetSystemTimeInMilisecond()
    {
		return ::timeGetTime();
    }//GetSystemTimeInMilisecond()
};//class   KSystemTimer

