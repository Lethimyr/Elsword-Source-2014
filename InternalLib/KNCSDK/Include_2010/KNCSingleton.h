#pragma once

// 생성자는 명시적으로 protected로 이동하기 위해
// 매크로에 포함하지 않는다. 싱글턴으로 사용하고자 하는
// 클래스를 정의할 때에는 명시적으로 생성자를 protected로 옮겨주어야 한다.
#define DeclareSingleton( classname ) \
    public: \
        static classname* GetInstance(); \
        static void ReleaseInstance(); \
    protected: \
        static classname*   ms_selfInstance

#define DefSingletonInline(ClassName)   \
    inline ClassName* Si##ClassName##() { return ClassName::GetInstance(); }

#define ImplementSingleton( classname ) \
    classname* classname::ms_selfInstance = NULL; \
    classname* classname::GetInstance() \
    { \
        if( ms_selfInstance == NULL ) \
            ms_selfInstance = new classname; \
        return ms_selfInstance; \
    } \
    void classname::ReleaseInstance() \
    { \
        if( ms_selfInstance != NULL ) \
        { \
            delete ms_selfInstance; \
            ms_selfInstance = NULL; \
        } \
    }

// instance keeping : 싱글턴 패턴의 모양을 하고 있으나, 객체 생성 시점과 소멸시점이 명확한 디자인.
// 단지 생성해서 보관해둔 인스턴스로의 접근이 용이하다는 점만을 싱글턴과 유사하게 처리함.
// KObj : Keeped Object - 생성되어 보관중인 객체(인스턴스).

#define DeclInstanceKeeping( classname ) \
    public: \
        static classname* GetKObj(); \
        static void ReleaseKObj(); \
        template<class T> static classname* KeepObject() { ms_object = new T; return ms_object; } \
    protected: \
        static classname*   ms_object

#define DefKObjectInline( child, parent )   \
    inline child* Get##child##() { return (child*)parent::GetKObj(); }

#define ImplInstanceKeeping( classname ) \
    classname* classname::ms_object = NULL; \
    classname* classname::GetKObj() \
    { \
        return ms_object; \
    } \
    void classname::ReleaseKObj() \
    { \
        if( ms_object != NULL ) \
        { \
            delete ms_object; \
            ms_object = NULL; \
        } \
    }