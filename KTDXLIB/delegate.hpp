/*
    (c) Sergey Ryazanov (http://home.onego.ru/~ryazanov)

    Fast delegate compatible with C++ Standard.
    Required files are 'delegate.hpp', 'delegate_list.hpp', and 'delegate_template.hpp'
*/
#ifndef SRUTIL_DELEGATE_INCLUDED
#define SRUTIL_DELEGATE_INCLUDED

#define SRUTIL_DELEGATE_CALLTYPE __fastcall
#include "delegate_list.hpp"
#undef SRUTIL_DELEGATE_CALLTYPE

#endif//SRUTIL_DELEGATE_INCLUDED

/** @example    delegate

    #include <iostream>
    #include <vector>
    #include "delegate.hpp"

    typedef srutil::delegate2<void, int, int> TestDelegate;

    void f(int, int) {std::cout << "f invoked.\n";}
    class TestClass
    {
    public:
        void m1(int, int) {std::cout << "m1 invoked for object " << this << ".\n";}
        void m2(int, int) const {std::cout << "m2 invoked for object " << this << ".\n";}
        static void m3(int, int) {std::cout << "m3 invoked.\n";}
    };

    void Test(TestDelegate d)
    {
        if (d)
        {
            std::cout << "Direct invoking... ";
            d(5, 10);

            std::cout << "Invoking through invoker... ";
            TestDelegate::invoker_type inv(5, 10);
            inv(d);
        }

        if (!d)
            std::cout << "The delegate is empty.\n";
    }//Test()

    void main()
    {
        TestClass obj;

        Test( TestDelegate() );
        Test( TestDelegate::from_function<&f>() );
        Test( TestDelegate::from_method<TestClass, &TestClass::m1>(&obj) );
        Test( TestDelegate::from_const_method<TestClass, &TestClass::m2>(&obj) );
        Test( TestDelegate::from_function<&TestClass::m3>() );

        std::vector<TestDelegate> vecDelegate;
        std::vector<TestDelegate>::iterator vitor;
        vecDelegate.push_back( TestDelegate::from_function<&f>() );
        vecDelegate.push_back( TestDelegate::from_method<TestClass, &TestClass::m1>(&obj) );

        vitor = vecDelegate.begin();
        while ( vitor != vecDelegate.end() )
        {
            TestDelegate d = *vitor;
            d( 5, 10 );
            ++vitor;
        }//while
    }//main()
    // console output
    //
    //The delegate is empty.
    //Direct invoking... f invoked.
    //Invoking through invoker... f invoked.
    //Direct invoking... m1 invoked for object 0012FECB.
    //Invoking through invoker... m1 invoked for object 0012FECB.
    //Direct invoking... m2 invoked for object 0012FECB.
    //Invoking through invoker... m2 invoked for object 0012FECB.
    //Direct invoking... m3 invoked.
    //Invoking through invoker... m3 invoked.
    //f invoked.
    //m1 invoked for object 0012FECB.
    //Press any key to continue

*/
