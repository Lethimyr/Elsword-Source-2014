#pragma once

/** @file   KGlobalSingleton.h
    @brief  interface of KGlobalSingleton
    @author robobeg@kogstudios.com
    @since  2008-05-28 오후 7:29

    @note   항상 WinMain 이전에 global 개체가 생성되고 WinMain을 빠져나간 후 소멸되는 것을 보장
            WinMain 이전에 다른 global variable 초기화 과정에서 Singleton()로 접근한 경우에도 개체가 생성, 초기화됨을 보장

    @note   KGlobalSingleton 변수들 사이의 destruction sequence에 의한 문제가 발생할 수 있다.
            KGlobalSingleton 의 destructor에서는 가급적 아무 일도 하지 말고 프로그램 종료 전에 모든 뒷처리를 마치자.

            [] 파괴 순서에 문제가 발생하는 경우의 예:
                A,B가 각각의 singleton 객체일 때, A객체의 파괴자에서 B객체를 접근한다.
                하지만, B객체가 이미 파괴되고 없는 상태라면 문제가 발생한다.
                이러한 상황은 발생하지 않도록 코딩하는 것이 바람직하며, 이러한 순서를 고려해야
                한다면, A객체의 생성자에서 B.Singleton()을 호출해 준다.
                그러면, B가 A보다 먼저 생성되므로, 파괴될 때는 반대로 A가 먼저 파괴된다.
                - jintaeks on 2008-05-29, 19:50
*/

// 임의의 클래스 타입A에 대해 KGlobalSingleton<A>::Singleton() 는 싱글턴 개체 한개를 제공한다.

// KGlobalSingleton로부터 유도한 클래스를 자체로는 생성 불가능한 singleton으로 만들려면 다음 두가지를 추가해야 한다.
// 1. 클래스 내부에 IMPLEMENT_KGLOBALSINGLETON( 클래스명 ) 추가
// 2. 클래스의 모든 constructor를 private 혹은 protected 로 선언

#ifdef CONVERSION_VS
#define IMPLEMENT_KGLOBALSINGLETON(T) \
    private: \
        friend class KGlobalSingleton<T>::KSingletonCreator;// \
        //T(const T&); \
        //T& operator = ( const T& );
#else CONVERSION_VS
#define IMPLEMENT_KGLOBALSINGLETON(T) \
	private: \
	friend class KGlobalSingleton<T>::KSingletonCreator; \
	T(const T&); \
	T& operator = ( const T& );
#endif CONVERSION_VS

template<typename T>
class   KGlobalSingleton
{
public:

    typedef T   Type;

    inline static Type&    Singleton() { return KSingletonCreator::Access(); }

    inline KGlobalSingleton() {}

private:

    KGlobalSingleton( const KGlobalSingleton& );
    KGlobalSingleton& operator = ( const KGlobalSingleton& );

#ifdef CONVERSION_VS
	protected:
#else CONVERSION_VS
	private:
#endif CONVERSION_VS

    class   KSingletonCreator
    {
    public:

        inline KSingletonCreator()
        {
            Access();
        }//KSingletonCreator()

        inline static Type&    Access()
        {
            /** inline 확장되는 함수의 내부에 선언된 static 변수의 instance가 유일하지 않은
                버그를 해결하기 위해, singleton의 instance를 얻는 함수에서 inline 확장되지
                않는 _NoInlineInstance()함수의 reference를 받아서 사용하도록 하다.
                - jintaeks on 2008-05-28, 9:45 */
            static  Type&  ref = _NoInlineAccess();
            return  ref;
        }//Access()

    private:

        inline void _DoNothing() {}

        __declspec(noinline) static Type& _NoInlineAccess()
        {
            static Type obj;
            /** WinMain()실행 전에 Singleton()을 호출하지 않더라도 객체를
                생성되는 것을 보장하기 위해,
                정적 객체의 생성자가 _NoInlineInstance()를 호출하도록 하다.
                ms_kSingletonCreator객체가 선언되었지만, 참조하는 곳이 없으므로
                객체가 생성되지 않는다. 하지만, KSingletonCreator의 생성자가
                Access()를 호출할 때, 처음 호출되는 _NoInlineAccess();함수의
                내부에 ms_kSingletonCreator의 멤버 함수, _DoNothing()를 호출하는
                코드가 있으므로, ms_kSingletonCreator를 생성해야 하며,
                이것은 _NoInlineAccess()내부에 선언된,
                static Type obj;가 실제로 생성됨을 의미한다.
                - jintaeks on 2008-05-28, 9:48 */
            ms_kSingletonCreator._DoNothing();
            return  obj;
        }//_NoInlineAccess()

    private:
        static KSingletonCreator    ms_kSingletonCreator;

    };//struct  KSingletonCreator

};//template<typename T>


template <typename T>
/*static*/ typename KGlobalSingleton<T>::KSingletonCreator  KGlobalSingleton<T>::KSingletonCreator::ms_kSingletonCreator;
