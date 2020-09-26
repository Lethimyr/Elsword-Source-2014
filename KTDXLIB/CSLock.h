/** @file   CSLock.h
	@brief  interface of CSLock
	@author seojt@kogstudios.com
	@since  2002-11-08 pm 05:21:22

    @date   2003-10-11 ¿ÀÀü 10:45:31
            - struct MemberCriticalSection
*/

#if !defined(_CSLOCK_H)
#define _CSLOCK_H

//#include "KJohnGen.h"

/** @example synchronized(cs)
    @code
    CRITICAL_SECTION    cs;
    synchronized(cs)
    {
        // sync code here
    }
    @endcode
*/
#define synchronized(cs_)        if( CSLock lock = cs_ )


//------------------------------------------------------------------------------
/// @struct     MemberCriticalSection
///
template<bool t_isMultiThreaded>
struct MemberCriticalSectionTemplate;

template<>
struct MemberCriticalSectionTemplate<true> : public CRITICAL_SECTION
{
    /// constructor.
    MemberCriticalSectionTemplate()
    {
        ::InitializeCriticalSection( this );
    }//CriticalSection()

    /// destructor.
    ~MemberCriticalSectionTemplate()
    {
        ::DeleteCriticalSection( this );
    }//~CriticalSection()

    void Enter()
    {
        ::EnterCriticalSection( this );
    }//Enter()

    void Leave()
    {
        ::LeaveCriticalSection( this );
    }//Leave()
};//struct MemberCriticalSection


template<>
struct MemberCriticalSectionTemplate<false>
{
    void    Enter() {}
    void    Leave() {}
};//struct MemberCriticalSection

typedef MemberCriticalSectionTemplate<true>     MemberCriticalSection;


//------------------------------------------------------------------------------
/// @struct CSLock
///
struct CSLock
{
    CRITICAL_SECTION&   m_pcs;

    CSLock(CRITICAL_SECTION& _cs) : m_pcs(_cs)
    {
        ::EnterCriticalSection( &m_pcs );
    }//CSLock()

    CSLock(MemberCriticalSection& mcs) : m_pcs(mcs)
    {
        ::EnterCriticalSection( &m_pcs );
    }//CSLock()

    ~CSLock()
    {
        ::LeaveCriticalSection( &m_pcs );
    }//~CSLock()

    operator bool()
    {
        return true;
    }

private:
    // prevent assignment and copy construction.
    CSLock& operator=(const CSLock& right);
    CSLock(const CSLock& copy);
};//struct CSLock


///-----------------------------------------------------------------------------
//
template<bool t_isMultiThreaded>
struct CSLockTemplate;

template<>
struct CSLockTemplate<true>
{
public:
    CRITICAL_SECTION&   m_pcs;

private:

    void Lock()     { ::EnterCriticalSection( &m_pcs ); }
    void Unlock()   { ::LeaveCriticalSection( &m_pcs ); }

public:

    CSLockTemplate(CRITICAL_SECTION& _cs) : m_pcs(_cs)
    {
        Lock();
    }//CSLockTemplate()

    CSLockTemplate(MemberCriticalSectionTemplate<true>& mcs) : m_pcs(mcs)
    {
        Lock();
    }//CSLockTemplate()

    ~CSLockTemplate()
    {
        Unlock();
    }//~CSLockTemplate()

private:
    // prevent assignment and copy construction.
    CSLockTemplate& operator=(const CSLockTemplate& right);
    CSLockTemplate(const CSLockTemplate& copy);
};//struct CSLockTemplate


template<>
struct CSLockTemplate<false>
{
public:

    CSLockTemplate(CRITICAL_SECTION&) {}

    CSLockTemplate(MemberCriticalSectionTemplate<false>&) {}

private:
    // prevent assignment and copy construction.
    CSLockTemplate& operator=(const CSLockTemplate& right);
    CSLockTemplate(const CSLockTemplate& copy);
};//struct CSLockTemplate


#define MCSLock(mcs)    CSLockTemplate<true>    __lock(mcs)

#endif // !defined(_CSLOCK_H)


/** @example    CSLock
    @code

    #include <iostream>
    #include "CSLock.h"

    using namespace std;

    CRITICAL_SECTION    g_cs;

    void main()
    {
        ::InitializeCriticalSection( &g_cs );

        synchronized( g_cs )
        {
            cout << "hello" << endl;
        }

        ::DeleteCriticalSection( &g_cs );
    }//main()

    @endcode
*/
