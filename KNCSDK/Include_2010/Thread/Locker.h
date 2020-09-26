#ifndef LOCKER_H
#define LOCKER_H

#include <windows.h>    // CRITICAL_SECTION

struct KncCriticalSection : public CRITICAL_SECTION
{
    KncCriticalSection()    { InitializeCriticalSection( this ); }
    ~KncCriticalSection()   { DeleteCriticalSection( this ); }
};

// helper class for lock
class KLocker
{
public:
    KLocker(CRITICAL_SECTION& cs ) : m_pcs( &cs )
    {
        Lock();
    }
    
    ~KLocker() 
    { 
        if( m_pcs )
            Unlock(); 
    }
    void Lock()
    {
        EnterCriticalSection( m_pcs );
    }
    void Unlock()
    {
        LeaveCriticalSection( m_pcs );
        m_pcs = NULL;
    }
protected:
    CRITICAL_SECTION*   m_pcs;
};

#endif
