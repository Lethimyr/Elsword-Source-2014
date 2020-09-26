#include <boost/test/unit_test.hpp>

#include <iostream>
#include <dbg/dbg.hpp>

using namespace dbg;

// struct to check logging capabilities.
struct vector
{
    float x, y, z;

    friend std::wostream& operator << ( std::wostream& o, vector& v )
    {
        o<< L"("<< v.x<< L","<< v.y<< L","<< v.z<< L")";
        return o;
    }
};

// Function used in rpofiling section
void some_wacky_useless_function()
{
    for( int i = 0; i < 99999; ++i )
    {
        double f = 54.547/i*24524+24542-94856-756/0.0000004563416*.00000000004596;
        int g = (int)f;
        f = g << i;
    }
}

void Debug_demo()
{
    profiler::init_profiler();

    int* p;

    p = new int[10]; // memory leak

    p = new int;

    delete p;


    float* pp = new float[5];

    // delete pp; // should be delete []. memory manager will break on this

    // float[5] is leaked...

    // Random vars to log.
    int i1 = 33;
    int i2 = 44;
    float f1 = 8.0945f;
    double d1 = 4.6366;
    char c1 = 'a';
    vector v1 = { 3.5, 4.5, 5.5 };

    cout<< "Test some random text"<< endl;
    cerr<< "Test an error followed by a line break"<< lbreak;
    cwarn<< tab<< "test a tab and a warning followed by and end or paragraph"<< endp;
    clog<< "Just logging normally"<< endl;

    clog<< endl<< "testing assert without a break point"<< endl;

    cassert( i1 == i2 );

    cassert(0)(i1)(i2)(v1)(f1)(d1).msg("some message");
    cassert(0)(i1)(i2)(v1)(f1)(d1).msg(L"wide char some message");

    clog<< endp<< "Profiling some for loops"<< endp;

    profiler::start( L"main" );
    {
        profiler::start( L"block 1" );
        for( int i = 0; i < 3; ++i )
        {
            profiler::start( L"in for loop 1" );
            some_wacky_useless_function();
            profiler::end();
        }
        profiler::end();
    }

    {
        profiler::start( L"block 2" );
        for( int i = 0; i < 5; ++i )
        {
            profiler::start( L"in for loop 1" );
            some_wacky_useless_function();
            profiler::end();
        }
        for( int i = 0; i < 2; ++i )
        {
            profiler::start( L"in for loop 2" );
            some_wacky_useless_function();
            profiler::end();
        }
        profiler::end();
    }
    profiler::end();

    profiler::kill_profiler( L"profile_info.html" );

    clog<< L"testing assert with a breakpoint (ie: .fatal() )"<< endl;


    cassert(0)(v1)(c1)(d1).msg(L"program end");
    cassert(0)(v1)(c1)(d1).msg(L"program end, WideChar ");
    //cassert(0)(v1)(c1)(d1).msg("program end").fatal();
}

void Debug_FileNameTest()
{
    // 이미 Debug_demo의 호출을 통해 현재 작업경로에 log.htm을 생성했을 것이다.
    //dbg::logfile::CloseFile();
    dbg::logfile::SetFileNameAtTime( true );    // 함수가 호출되면 이전의 설정들을 지우고 새로 초기화한다.
    dbg::logfile::SetLogFileTitle( L"직접 지정한 로그 이름" );  // 새로 생성할 파일의 타이틀을 지정한다.

    START_LOG( cout, "폴더가 없을 경우 직접 폴더를 생성합니다." )
        << END_LOG;

    START_LOG( cout, L"폴더가 없을 경우 직접 폴더를 생성합니다." )
        << END_LOG;
    dbg::cout << "하하하";

    std::wstring m_strName = L"이름출력 테스트";

    dbg::cout << dbg::endl;

    START_LOG_WITH_NAME( cout );
    START_LOG_WITH_NAME( cerr );
    START_LOG_WITH_NAME( cwarn );
    START_LOG_WITH_NAME( clog );

    dbg::cout << dbg::endl;

    START_LOG_WITH_NAME( cout2 );
    START_LOG_WITH_NAME( cerr2 );
    START_LOG_WITH_NAME( cwarn2 );
    START_LOG_WITH_NAME( clog2 );

    dbg::cout << "log file name : " << dbg::logfile::GetLogFileName() << dbg::endl;
}

void Debug_TestLogLevel()
{
    dbg::cout << "default log level is 2" << dbg::lbreak;

    dbg::cout << "dbg::cout : log level 0 " << dbg::endl;
    dbg::cerr << "dbg::cerr : log level 0 " << dbg::endl;
    dbg::cwarn << "dbg::cwran : log level 1 " << dbg::endl;
    dbg::clog << "dbg::cout : log level 2 " << dbg::endl;

    dbg::logfile::SetLogLevel( 1 );

    dbg::cout << "dbg::cout : log level 0 " << dbg::endl;
    dbg::cerr << "dbg::cerr : log level 0 " << dbg::endl;
    dbg::cwarn << "dbg::cwran : log level 1 " << dbg::endl;
    dbg::clog << "dbg::cout : log level 2 " << dbg::endl;

    dbg::logfile::SetLogLevel( 0 );

    dbg::cout << "dbg::cout : log level 0 " << dbg::endl;
    dbg::cerr << "dbg::cerr : log level 0 " << dbg::endl;
    dbg::cwarn << "dbg::cwran : log level 1 " << dbg::endl;
    dbg::clog << "dbg::cout : log level 2 " << dbg::endl;

    dbg::logfile::SetLogLevel( -1 );

    dbg::cout << "dbg::cout : log level 0 " << dbg::endl;
    dbg::cerr << "dbg::cerr : log level 0 " << dbg::endl;
    dbg::cwarn << "dbg::cwran : log level 1 " << dbg::endl;
    dbg::clog << "dbg::cout : log level 2 " << dbg::endl;

    dbg::logfile::SetLogLevel( 2 );

    dbg::cout << "dbg::cout : log level 0 " << dbg::endl;
    dbg::cerr << "dbg::cerr : log level 0 " << dbg::endl;
    dbg::cwarn << "dbg::cwran : log level 1 " << dbg::endl;
    dbg::clog << "dbg::cout : log level 2 " << dbg::endl;

    int     a = 1234;
    char*   b = "dummy string";

    START_LOG( clog, "make log using macro" )
        << BUILD_LOG( a )
        << BUILD_LOG( b )
        << END_LOG;

    std::string m_strName = "테스트용 m_strName 변수";

    START_LOG_WITH_NAME( cout );
    START_LOG_WITH_NAME( cout2 );
    START_LOG_WITH_NAME( cerr );
    START_LOG_WITH_NAME( cerr2 );
    START_LOG_WITH_NAME( cwarn );
    START_LOG_WITH_NAME( cwarn2 );
    START_LOG_WITH_NAME( clog );
    START_LOG_WITH_NAME( clog2 );

    dbg::cout << "일반 스트링과 " << L"유니코드 스트링을 함께 출력해도" << "내부에서 자동 변환됨." << dbg::endl;
}

void TestDebug()
{
    Debug_demo();
    Debug_FileNameTest();
    Debug_TestLogLevel();
}