#ifndef THREAD_H
#define THREAD_H "$Id: Thread.h,v 1.1 2003/05/23 01:18:40 cvs Exp $"

#include <windows.h>
#include <string>
#include <map>
#include "Locker.h"

#include <process.h>
#pragma comment( lib, "Winmm" )     // timeGetTime()

#ifndef _BEGINTHREADEX 
typedef unsigned (__stdcall* PTHREAD_START)(void*);
#   define _BEGINTHREADEX( psa, \
                           cbStack, \
                           pfnStartAddr, \
                           pvParam, \
                           fdwCreate, \
                           pdwThreadId) \
          ((HANDLE)_beginthreadex( (void*) (psa), \
                                   (unsigned) (cbStack), \
                                   (PTHREAD_START) (pfnStartAddr), \
                                   (void*) (pvParam), \
                                   (unsigned) (fdwCreate), \
                                   (unsigned*) (pdwThreadId)))
#endif

class KThreadManager;
class KThread
{
public:
    KThread() : m_hKillEvent( NULL ), m_hThread( NULL ), m_pkThreadMgr( NULL ) {}
    virtual ~KThread()
    {
        End();

        if( NULL != m_hKillEvent )
        {
            ::CloseHandle( m_hKillEvent );
            m_hKillEvent = NULL;
        } // if
    }
    
    virtual bool Begin()                        // _BEGINTHREADEX 호출. Run()이 호출되는 시점.
    {
        if( m_hThread != NULL )
            return false;

        m_hKillEvent = CreateEvent(NULL, false, false, NULL);
        if( NULL == m_hKillEvent )
            return false;

        DWORD dwThreadId;
        m_hThread = _BEGINTHREADEX( NULL,
            0,
            ThreadProc,
            this,
            0,
            &dwThreadId );
        if( NULL == m_hThread )
        {
            ::CloseHandle( m_hKillEvent);
            m_hKillEvent = NULL;
            return false;
        } // if

        return true;    
    }
    virtual void End(DWORD dwTimeout = 3000)    // 스레드의 종료를 요청하고 이를 기다렸다가 릴리즈
    {
        if( NULL != m_hThread )
        {
            SetEvent( m_hKillEvent );
            ::WaitForSingleObject( m_hThread, dwTimeout );
            ::CloseHandle( m_hThread );
            m_hThread = NULL;
        } // if
    }
    
    virtual void Run() = 0;
    
    void SetThreadManager( KThreadManager* pkMgr ) { m_pkThreadMgr = pkMgr; }

protected:
    static unsigned int WINAPI ThreadProc(LPVOID pvParam)
    {
        KThread* thread = reinterpret_cast<KThread*>(pvParam);
        thread->Run();
        return true;
    }
    
protected:
    HANDLE          m_hKillEvent;
    HANDLE          m_hThread;
    KThreadManager* m_pkThreadMgr;
};


template<typename T>
class KTThread
{
public:
    KTThread(T& kobject, void (T::*pfnRun)(), DWORD dwTimeout );
    virtual ~KTThread();
    
    virtual bool Begin();
    virtual void End(DWORD dwTimeout = 3000);
    
    inline void Run();
    
    DWORD GetTimeout() { return m_dwTimeout; }
    void SetTimeout(DWORD dwTimeout) { m_dwTimeout = dwTimeout; }
    DWORD GetThreadID() { return m_dwThreadID; }

protected:
    static unsigned int WINAPI ThreadProc(LPVOID pvParam)
    {
        KTThread* thread = reinterpret_cast<KTThread*>(pvParam);
        thread->Run();
        return true;
    }
    
protected:
    HANDLE      m_hKillEvent;
    HANDLE      m_hThread;
    DWORD       m_dwThreadID;
    
    T&          m_kobject;
    void       (T::*m_pfnRun)();
    DWORD       m_dwTimeout;
};


template<typename T> 
KTThread<T>::KTThread(T& kobject_, void (T::*pfnRun_)(), DWORD dwTimeout_)
    : m_kobject(kobject_), m_dwThreadID( 0 )
{
    m_dwTimeout     = dwTimeout_;
    m_pfnRun        = pfnRun_;
    m_hKillEvent    = NULL;
    m_hThread       = NULL;
}

template<typename T>
KTThread<T>::~KTThread()
{
    End();
    
    if( NULL != m_hKillEvent )
    {
        ::CloseHandle( m_hKillEvent );
        m_hKillEvent = NULL;
    } // if
}

template<typename T> 
bool KTThread<T>::Begin()
{
    if( m_hThread != NULL )
        return false;
    
    m_hKillEvent = CreateEvent(NULL, false, false, NULL);
    if( NULL == m_hKillEvent )
        return false;
    
    m_hThread = _BEGINTHREADEX( NULL,
                                0,
                                ThreadProc,
                                this,
                                0,
                                &m_dwThreadID );
    if( NULL == m_hThread )
    {
        ::CloseHandle( m_hKillEvent);
        m_hKillEvent = NULL;
        return false;
    } // if
    
    return true;    
}

template<typename T>
void KTThread<T>::End(DWORD dwTimeout_)
{
    if( NULL != m_hThread )
    {
        SetEvent( m_hKillEvent );
        ::WaitForSingleObject( m_hThread, dwTimeout_ );
        ::CloseHandle( m_hThread );
        m_hThread = NULL;
    } // if
}

template<typename T> 
void KTThread<T>::Run()
{
    DWORD   ret = NULL;
    bool    bEnd = false;
    while( !bEnd )
    {
        ret = ::WaitForSingleObject( m_hKillEvent, m_dwTimeout );
        switch( ret )
        {
        case WAIT_OBJECT_0:
            bEnd = true;
            break;
        case WAIT_TIMEOUT:
            (m_kobject.*m_pfnRun)();
            break;
        } // switch
    } // while
}


template<typename T>
class KThreadTimer
{
public:
    struct _KTimer
    {
        std::string     m_strName;
        DWORD           m_dwPeriod;
        DWORD           m_dwLastRunTime;
        void           (T::*m_pfnRun)();
    };

    KThreadTimer(T& kobject, DWORD dwPeriod = 1000);
    virtual ~KThreadTimer();
    
    virtual bool Begin();
    virtual void End(DWORD dwTimeout = INFINITE);
    
    bool AddTimer( const char* szName, DWORD dwPeriod, void (T::*pfnRun)());
    void DeleteTimer( const char* szName);
    void SetTimerPeriod( const char* szName, DWORD dwPeriod);

    inline void Run();
    
//    DWORD GetPeriod() { return m_dwTimeout; }
//    void SetPeriod(DWORD dwPeriod) { m_dwPeriod = dwPeriod; }

protected:
    static unsigned int WINAPI ThreadProc(LPVOID pvParam)
    {
        KThreadTimer* thread = reinterpret_cast<KThreadTimer*>(pvParam);
        thread->Run();
        return true;
    }
    
protected:
    typedef std::map<std::string, _KTimer>  MapTimer;
    MapTimer                                m_mapTimer;
    CRITICAL_SECTION                        m_csMapTimer;
    HANDLE                                  m_hKillEvent;
    HANDLE                                  m_hThread;
    DWORD                                   m_dwPeriod;    
    T&                                      m_kobject;
};


template<typename T> 
KThreadTimer<T>::KThreadTimer(T& kobject_, DWORD dwPeriod_)
    : m_kobject(kobject_)
{
    m_dwPeriod      = dwPeriod_;
    m_hKillEvent    = NULL;
    m_hThread       = NULL;
    InitializeCriticalSection(&m_csMapTimer);
}

template<typename T>
KThreadTimer<T>::~KThreadTimer()
{
    End();
    
    if( NULL != m_hKillEvent )
    {
        ::CloseHandle( m_hKillEvent );
        m_hKillEvent = NULL;
    } // if

    m_mapTimer.clear();
    DeleteCriticalSection(&m_csMapTimer);
}

template<typename T> 
bool KThreadTimer<T>::Begin()
{
    if( m_hThread != NULL )
        return false;
    
    m_hKillEvent = CreateEvent(NULL, false, false, NULL);
    if( NULL == m_hKillEvent )
        return false;
    
    DWORD dwThreadId;
    m_hThread = _BEGINTHREADEX( NULL,
                                0,
                                ThreadProc,
                                this,
                                0,
                               &dwThreadId );
    if( NULL == m_hThread )
    {
        ::CloseHandle( m_hKillEvent);
        m_hKillEvent = NULL;
        return false;
    } // if
    
    return true;    
}

template<typename T>
void KThreadTimer<T>::End(DWORD dwTimeout_)
{
    if( NULL != m_hThread )
    {
        SetEvent( m_hKillEvent );
        ::WaitForSingleObject( m_hThread, dwTimeout_ );
        ::CloseHandle( m_hThread );
        m_hThread = NULL;
    } // if
}

template<typename T> 
void KThreadTimer<T>::Run()
{
    DWORD   ret = NULL;
    bool    bEnd = false;
    while( !bEnd )
    {
        if( 0 == m_dwPeriod )
        {
            MapTimer::iterator    mit;
            KLocker lock(m_csMapTimer);
            for( mit = m_mapTimer.begin(); mit != m_mapTimer.end(); mit++ )
            {
                if( timeGetTime() - mit->second.m_dwLastRunTime+(m_dwPeriod*0.1f) > mit->second.m_dwPeriod )
                {
                    (m_kobject.*mit->second.m_pfnRun)();
                    mit->second.m_dwLastRunTime = timeGetTime();
                } // if
            } //for
            bEnd = true;
        }
        else
        {
            ret = ::WaitForSingleObject( m_hKillEvent, m_dwPeriod );
            switch( ret )
            {
            case WAIT_OBJECT_0:
                bEnd = true;
                break;
            case WAIT_TIMEOUT:
                MapTimer::iterator    mit;
                KLocker lock(m_csMapTimer);
                for( mit = m_mapTimer.begin(); mit != m_mapTimer.end(); mit++ )
                {
                    if( (timeGetTime() - mit->second.m_dwLastRunTime)+(m_dwPeriod*0.1f) > mit->second.m_dwPeriod )
                    {
                        (m_kobject.*mit->second.m_pfnRun)();
                        mit->second.m_dwLastRunTime = timeGetTime();
                    } // if
                } //for
                break;
            } // switch
        } //else
    } // while
}

template<typename T> 
bool KThreadTimer<T>::AddTimer( const char* szName_, DWORD dwPeriod_, void (T::*pfnRun_)())
{
    KLocker lock(m_csMapTimer);
    MapTimer::const_iterator    mit;
    mit = m_mapTimer.find( szName_ );
    if( mit != m_mapTimer.end() )
        return false;
            
    _KTimer ktimer;
    ktimer.m_strName        = szName_;
    ktimer.m_dwPeriod       = dwPeriod_;
    ktimer.m_dwLastRunTime  = timeGetTime();
    ktimer.m_pfnRun         = pfnRun_;

    m_mapTimer[szName_]    = ktimer;
    return true;
}

template<typename T> 
void KThreadTimer<T>::DeleteTimer( const char* szName_ )
{
    KLocker lock(m_csMapTimer);
    MapTimer::iterator    mit;
    mit = m_mapTimer.find( szName_ );
    if( mit == m_mapTimer.end() )
        return;
    m_mapTimer.erase( mit );           
}

template<typename T> 
void KThreadTimer<T>::SetTimerPeriod( const char* szName_, DWORD dwPeriod_)
{
    KLocker lock(m_csMapTimer);
    MapTimer::iterator    mit;
    mit = m_mapTimer.find( szName_ );
    if( mit == m_mapTimer.end() )
        return;
    mit->second.m_dwPeriod = dwPeriod_;
}

#endif
