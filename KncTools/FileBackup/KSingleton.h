#ifndef  _KOG_KSINGLETON_H_
#define  _KOG_KSINGLETON_H_ "$Id: KSingleton.h,v 1.4 2003/01/15 12:54:56 cvs Exp $"
#pragma once // for _MSC_VER > 1000

//{ added by florist
// code from <lua.h> -> <Common/KbDllApi.h>
#ifndef _DLLAPI
#if defined(_USRDLL) // DLL library compile 중
#define	_DLLAPI	__declspec(dllexport)
#elif defined(_DLL) // DLL library 사용
#define	_DLLAPI	__declspec(dllimport)
#else
#define _DLLAPI
#endif
#endif

//}


/** Template to implement singleton classes.
Singleton은 프로그램 전체에서 오직 하나만 존재해야 하는 것을 의미한다.
이 template은 이미 구현된 class를 wrapping해서, Singleton 속성을
가지도록 만들어 준다. 이미 KLogManager란 class를 구현했으면,
<i>KSingleton<KLogManager>::GetInstance()</i> 호출은 새로운 (그리고
유일한) KLogManager를 만들거나, 이미 존재하는 KLogManager를 가져오는
역할을 하고, <i>KSingleton<KLogManager>::GetInstance()->Log(...)</i> 와
같은 형태로 KLogManager내의 기능을 직접 사용할 수 있다. 사용이 완전히
끝난 Singleton은 프로그램 전체의 종료 시에
<i>KSingleton<LogManager>::ReleaseInstance()</i> 함수를 불러서 완전히
지워야 한다.
*/
template <typename TYPE>
class KSingleton
{
private:
    KSingleton(void) { } // disable construction of this class !
public:
    /** Get the singleton instance. */
    static TYPE* GetInstance(void);
    /** Release the instance. */
    static void ReleaseInstance(void);
private:
    static TYPE* ms_selfInstance; ///< The real class for singleton.
};

template <typename TYPE>
TYPE* KSingleton<TYPE>::ms_selfInstance = 0;

template <typename TYPE>
TYPE* KSingleton<TYPE>::GetInstance(void)
{
    if (ms_selfInstance == 0)
    {
        ms_selfInstance = new TYPE();
    }
    return ms_selfInstance;
}

template <typename TYPE>
void KSingleton<TYPE>::ReleaseInstance(void)
{
    if (ms_selfInstance != 0)
    {
        delete ms_selfInstance;
        ms_selfInstance = 0;
    }
}


#endif // _KOG_KSINGLETON_H_
