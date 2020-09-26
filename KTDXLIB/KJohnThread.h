/** @file   KJohnThread.h
    @brief  interface of KJohnThread
    @author seojt@kogstudios.com
    @since  2006-1-11, 10:26:49

    @date   jintaeks on 2008-06-13, 10:01
            - 설명문 추가
*/

#ifndef _KJOHNTHREAD_H
#define _KJOHNTHREAD_H

//#include "KJohnGen.h"
#include <process.h>


/** _beginthreadex() vs. CreateThread()

    There is a hot discussion on one Russian developers forum about correct threads creation. 
    _beginthreadex is more correct way because it initializes CRT for the newly created thread.

    But there is one great "BUT"!!!

    For example we have any callback function which uses CRT (like an exported function from dll). 
    This function could be invoked from any thread including threads, that was created via 
    CreateThread (i.e. CRT was not initialized for this thread). So this call can cause an undefined 
    behaviour! But there is not way to determine CRT initialization status and initialize CRT 
    manually. 

    It looks like a potential source of great troubles.

    @see    http://social.msdn.microsoft.com/Forums/en-US/vclanguage/thread/c727ae29-5a7a-42b6-ad0b-f6b21c1180b2/
    - jintaeks on 2008-11-06, 14:44 */
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
#endif // _BEGINTHREADEX


///-----------------------------------------------------------------------------
/// @class  KJohnThread
/// @brief  base class for a thread job
/// @note   override below 3 virtual functions
/** @code
    virtual bool BeginThread();
    virtual void EndThread(DWORD dwTimeoutMilliseconds_ = INFINITE);
    virtual DWORD RunThread() = 0;
    @endcode
*/
///
class KJohnThread
{
public:
    /** virtual RunThread()를 override해서 사용하지 않는 경우, 사용자가
        지정해 주는 thread callback함수의 type 정의 */
    typedef DWORD (WINAPI *THREAD_CALLBACK)(LPVOID);

protected:
    DWORD   m_dwThreadID;   ///< thread ID
    HANDLE  m_hThread;      ///< thread handle

public:
    /// constructor.
    KJohnThread(void)
    {
        _Init();
    }//KJohnThread

    /// destructor.
    virtual ~KJohnThread(void)
    {
        // 안전하게 thread를 종료하므로, BeginThread()가 호출되지 않은 상태에서
        // 호출되어도 된다.
        EndThread();
    }//~KJohnThread

private:
    /** KJohnThread를 상속받은 객체가 복사되는 것을 방지한다.
        - jintaeks on 2008-06-13, 10:00 */
    KJohnThread(const KJohnThread& rhs_);
    KJohnThread& operator=(const KJohnThread& rhs_);

private:
    void _Init()
    {
        m_dwThreadID    = NULL;
        m_hThread       = NULL;
    }//_Init()

public:
    static DWORD WINAPI ThreadFunc(LPVOID lpParam_)
    { 
        KJohnThread*    pThread = reinterpret_cast<KJohnThread*>(lpParam_);
        ASSERT( pThread );
        return pThread->RunThread();
    }//ThreadFunc()

public:
    /// begin thread.
    virtual bool BeginThread(THREAD_CALLBACK pThreadCallback_=NULL, LPVOID pParam_=NULL, bool bStartSuspended = false )
    {
        ASSERT( m_hThread == NULL );

        if ( pThreadCallback_ )
        {
            m_hThread = _BEGINTHREADEX(
                NULL,                   // default security attributes 
                0,                      // use default stack size  
                pThreadCallback_,        // thread function 
                pParam_,                   // argument to thread function 
                ( bStartSuspended == true ) ? CREATE_SUSPENDED : 0,                      // use default creation flags 
                &m_dwThreadID );        // returns the thread identifier 
        }
        else
        {
            m_hThread = _BEGINTHREADEX( 
                NULL,                       // default security attributes 
                0,                          // use default stack size  
                KJohnThread::ThreadFunc,    // thread function 
                this,                       // argument to thread function 
                ( bStartSuspended == true ) ? CREATE_SUSPENDED : 0,                          // use default creation flags 
                &m_dwThreadID );            // returns the thread identifier 
        }//if.. else..

        return m_hThread == NULL ? false : true;
    }//BeginThread()

    /// wait thread ending.
    /// thread를 종료시키고, thread handle을 clear한다.
    /// @param  dwTimeoutMilliseconds_: 종료 대기 시간 millisecond 단위로 지정한다.
    virtual void EndThread(DWORD dwTimeoutMilliseconds_ = INFINITE)
    {
        if ( m_hThread != NULL )
        {
            ::WaitForSingleObject( m_hThread, dwTimeoutMilliseconds_ );
            ::CloseHandle( m_hThread );
            m_hThread       = NULL;
            m_dwThreadID    = NULL;
        }//if
    }//EndThread()

    /// threading pure virtual function.
    virtual DWORD RunThread() = 0;

public:
    /// get thread ID.
    DWORD GetThreadID() const { return m_dwThreadID; }

    /// get thread handle.
    HANDLE GetThreadHandle() const { return m_hThread; }
};//class KJohnThread

#endif // _KJOHNTHREAD_H


/** @example    KJohnThread
    @code

    #include "KJohnGen.h"
    #include <iostream>
    #include <mmsystem.h>
    #include <conio.h>
    #include "KJohnThread.h"

    using namespace std;

    class TThread : public KJohnThread
    {
    public:
        virtual bool BeginThread()
        {
            return KJohnThread::BeginThread();
        }//BeginThread()

        virtual void EndThread(DWORD dwTimeoutMilliseconds_ = INFINITE)
        {
            KJohnThread::EndThread();
        }//EndThread()

        virtual DWORD RunThread()
        {
            char szMsg[80];

            StringCchPrintfA( szMsg, ARRAY_SIZE( szMsg ), "thread ID = %d", m_dwThreadID );
            MessageBox( NULL, szMsg, "ThreadFunc", MB_OK );

            return 0;
        }//RunThread()
    };//class TThread : public KJohnThread

    void main()
    {
        INIT_CRTDEBUG();

        TThread     thread; 

        thread.BeginThread();
        thread.EndThread();
    }//main()

    @endcode
*/
