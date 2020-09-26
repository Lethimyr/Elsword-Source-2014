#include <boost/test/unit_test.hpp>

#if (_MSC_VER < 1300)   // for VC++ 6.0
#define __FUNCTION__ __FILE__
#endif

#ifndef _MT // multi threaded 컴파일이 아닌 경우
#pragma message( __FILE__ "(8):Thread 관련 library는 싱글 스레드 환경에서 사용할 수 없습니다." )
#else

#include <Thread/Thread.h>
#include <iostream>

class KTest : public KThread
{
public:
    KTest()             { Begin(); }
    virtual ~KTest()    { End(); }
    virtual void Run()
    {
        DWORD   ret = NULL;
        bool    bEnd = false;
        while( !bEnd )
        {
            ret = ::WaitForSingleObject( m_hKillEvent, 300 );
            switch( ret )
            {
            case WAIT_OBJECT_0:
                bEnd = true;
                break;
            case WAIT_TIMEOUT:
                std::cout << __FUNCTION__ << std::endl;
                break;
            } // switch
        } // while
    }
};


class ThreadTest1
{
public:
    void Working()
    {
        std::cout << __FUNCTION__ << std::endl;
    }
    void Working1()
    {
        std::cout << __FUNCTION__ << std::endl;
    }
};


class KTimerTest
{
public:
    KTimerTest()
    {
        m_pkTimer   = new KThreadTimer<KTimerTest>( *this );
    }
    ~KTimerTest()
    {
        delete m_pkTimer;
    }

    void AddTimer()
    {
        m_pkTimer->AddTimer( "Timer0", 1000, KTimerTest::Working0);
        m_pkTimer->AddTimer( "Timer1", 4000, KTimerTest::Working1);
    }
    void DeleteTimer()
    {
        m_pkTimer->DeleteTimer( "Timer0" );
    }
    void SetTimerPeriod()
    {
        m_pkTimer->SetTimerPeriod( "Timer1", 1000);
    }


    void Working0()
    {
        std::cout << "working0: " <<  timeGetTime()-m_dwLastRunTime0 << std::endl;
        m_dwLastRunTime0    = timeGetTime();
    }
    void Working1()
    {
        std::cout << "working1: " << timeGetTime()-m_dwLastRunTime1 << std::endl;
        m_dwLastRunTime1    = timeGetTime();
    }
    void Begin() 
    { 
        m_dwLastRunTime0    = timeGetTime();
        m_dwLastRunTime1    = timeGetTime();
        m_pkTimer->Begin(); 
    }

    DWORD       m_dwLastRunTime0;
    DWORD       m_dwLastRunTime1;

    KThreadTimer<KTimerTest>*     m_pkTimer;
};


void TestThread()
{
    KTest t;

    ThreadTest1 test1;
    
    KTThread<ThreadTest1> aTest(test1,
                               &ThreadTest1::Working, 
                                200); // 200 주기로 함수 실행, 0면 한번만 실행 종료
    aTest.Begin();

    KTimerTest      aTimer;
    aTimer.AddTimer();
    aTimer.Begin();

    getchar();
    aTimer.DeleteTimer();
    aTimer.SetTimerPeriod();

    getchar();

}

#endif