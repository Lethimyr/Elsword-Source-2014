#pragma warning(disable: 4786)

#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <set>
#include <windows.h>
using namespace std;

#include "Thread.h"


class ThreadTest1
{
public:
    void Working()
    {
        cout << "working" << endl;
    }
    void Working1()
    {
        cout << "working1" << endl;
    }
};


class KTest : public KThread
{
public:
    KTest();
    virtual ~KTest();
    
    virtual void Run();
};

KTest::KTest()
{
    Begin();
}

KTest::~KTest()
{
    End();
}

void KTest::Run()
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
            cout << "run" << endl;
            break;
        } // switch
    } // while
} // Working



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
        m_pkTimer->AddTimer(std::string("Timer0"), 1000, KTimerTest::Working0);
        m_pkTimer->AddTimer(std::string("Timer1"), 4000, KTimerTest::Working1);
    }
    void DeleteTimer()
    {
        m_pkTimer->DeleteTimer(std::string("Timer0"));
    }
    void SetTimerPeriod()
    {
        m_pkTimer->SetTimerPeriod(std::string("Timer1"), 1000);
    }


    void Working0()
    {
        cout << "working0: " <<  timeGetTime()-m_dwLastRunTime0 << endl;
        m_dwLastRunTime0    = timeGetTime();
    }
    void Working1()
    {
        cout << "working1: " << timeGetTime()-m_dwLastRunTime1 << endl;
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






void main()
{
//    KTest t;
//
//    ThreadTest1 test1;
//    
//    KTThread<ThreadTest1> aTest(test1,
//                               &ThreadTest1::Working, 
//                                200); // 200 주기로 함수 실행, 0면 한번만 실행 종료
//    aTest.Begin();

    KTimerTest      aTimer;
    aTimer.AddTimer();
    aTimer.Begin();

    getchar();
    aTimer.DeleteTimer();
    aTimer.SetTimerPeriod();

    getchar();

} // main()
