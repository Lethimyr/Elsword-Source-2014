/** @file   KJohnSingleton.h
	@brief  interface of KJohnSingleton
	@author seojt@kogstudios.com
	@since  2003-12-08 오후 6:06:27

    @date   jintaeks on 2007-04-12, 9:56
            - IMPLEMENT_KJOHNSINGLETON_FUN(T) 매크로 추가
*/

#ifndef _KJOHNSINGLETON_H
#define _KJOHNSINGLETON_H

#include "KJohnGen.h"

//#define IMPLEMENT_KJOHNSINGLETON(T)     public: inline static T* GetSingleton() { ASSERT(ms_singleton); return ms_singleton; }
#define IMPLEMENT_KJOHNSINGLETON(T)     
#define IMPLEMENT_KJOHNSINGLETON_FUN(T) inline T* _##T() { return T::GetSingleton(); }


//------------------------------------------------------------------------------
/// @class  KJohnSingleton<>
/// @desc   KJohnSingleton<>의 주된 목적은 singleton 패턴을 사용하면서, 객체의 생성 소멸 시점을
///         사용자가 manual하게 관리하려는 것이다.
/// @note   you can control singleton creation time
/**
            [] 객체의 생성과 singleton interface의 사용:

            전역 객체 선언으로 KJohnSingleton<>을 상속받은 객체를 만들 수도 있고,
            CreateSingleton() 정적 함수를 사용하여 동적으로 객체를 생성할 수 도 있다.
            어떠한 방식으로 객체를 만들었든지 간에,
            싱글턴 객체를 사용하는 곳에서는 객체이름이 아니라,
            KJohnSingleton<>이 제공하는 interface를 사용하여 singleton 객체를 접근함으로써
            객체의 이름에 independent한 interface를 사용할 수 있게 된다.
            - 코멘트 작성 jintaeks on 2008-04-23, 16:00
*/
template<typename T>
class KJohnSingleton
{
protected:
    static T*   ms_singleton;
    static bool ms_bInternalCreation;   ///< CreateSingleton()를 호출하여 객체를 생성한 경우 true이다.

public:
    /// constructor.
	KJohnSingleton()
    {
        ASSERT( NULL == ms_singleton );
        ms_singleton = (T*)this;
    }//KJohnSingleton()

    /// destructor.
    /// 'KJohnSingleton*'를 바로 사용하여 객체를 파괴할 일이 없다고 가정하고
    /// 파괴자를 가상으로 만들지 않는다. - jintaeks on 2008-02-12, 11:20
	/*virtual*/ ~KJohnSingleton()
    {
        ASSERT( ms_singleton );
        if ( ms_bInternalCreation )
            DestroySingleton();
        ms_singleton = NULL;
    }//KJohnSingleton()

    static T* CreateSingleton()
    {
        if ( ms_singleton != NULL )
            return ms_singleton;

        ms_bInternalCreation = true;
        return ::new T();
    }//CreateSingleton()

    static void DestroySingleton()
    {
        ::delete ms_singleton;
        ms_bInternalCreation = false;
    }//DestroySingleton()

    static T* GetSingleton() { ASSERT(ms_singleton); return ms_singleton; }
};//class KJohnSingleton

template<typename T> T* KJohnSingleton<T>::ms_singleton = NULL;
template<typename T> bool KJohnSingleton<T>::ms_bInternalCreation = false;

#endif // _KJOHNSINGLETON_H


/** @example    KJohnSingleton<>
    @code

    #include <iostream>
    #include "KJohnSingleton.h"

    class KManager : public KJohnSingleton<KManager>
    {
        IMPLEMENT_KJOHNSINGLETON(KManager)

    public:
        void Print() { std::cout << "hello singleton" << std::endl; }
    };//class KManager

    KManager    instance;

    void main()
    {
        KManager::GetSingleton()->Print();
    }//main()

    @endcode
*/
