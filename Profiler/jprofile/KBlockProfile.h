/** @file   KBlockProfile.h
    @brief  interface of KBlockProfile
    @author seojt@kogstudios.com
    @since  2006-1-12, 18:37:06
*/

#ifndef _KBLOCKPROFILE_H
#define _KBLOCKPROFILE_H

//#include "../KJohnGen.h"
//#include "./jprofile/KProfile.h"
//#include "./KProfile.h"


///-----------------------------------------------------------------------------
/// @class  KBlockProfile
///
class KBlockProfile : public KProfile
{
private:
    static char     s_szNull[];

private:
    char*   m_pszName;
    int     m_iOutputTarget;

public:
    /// constructor.
    KBlockProfile(const char* pszName_=NULL, int iTarget=0)
        : m_iOutputTarget(iTarget)
    {
        m_pszName = const_cast<char*>(pszName_);
        if ( pszName_ == NULL )
            m_pszName = s_szNull;
        Begin();
    }//KBlockProfile()

    /// destructor.
    ~KBlockProfile()
    {
        End();
        TraceTime();
    }//~KBlockProfile()

    /// debug TRACE profile result.
    void TraceTime()
    {
        if ( m_iOutputTarget == 0 )
        {
            if ( GetTime() < 1.0e-004 )
            {
                //TRACEA( "  KProfile <%s> = %g\n", m_pszName, GetTime() );
            }
            else
            {
                TRACEA( "¡ßKProfile <%s> = %g\n", m_pszName, GetTime() );
            }//if.. else..
        }
        else if ( m_iOutputTarget == 1 )
        {
            printf( "¡ßKProfile <%s> = %g\n", m_pszName, GetTime() );
        }//if.. else if..
    }//TraceTime()
};//class KBlockProfile

#endif // _KBLOCKPROFILE_H


/** @example    KBlockProfile
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

        KTimer  timer;
        int     iCounter = 0;
        timer.Start();
        while ( 1 )
        {
            timer.UpdateTime();
            ++iCounter;
            {
                KBlockProfile profile( "Sleep" );
                Sleep( 1000 );
            }
            
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

    @endcode
*/
