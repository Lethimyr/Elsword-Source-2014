/** @file   KJohnGen.h
	@brief  interface of KJohnGen
	@author seojt@kogstudios.com
    @date   seojt on 2005-5-2, 11:42:04
            - UNICODE compatible
    @date   seojt on 2005-07-06
            - ARRAY_SIZE
    @date   seojt on Thu, 2005-10-27
            - ASSERT_FLOAT()
    @date   seojt on 2006-1-12, 14:33:14
            - D3D_DEBUG_INFO
    @date   seojt on 2006-2-1, 10:09:29
            - NONCOPYABLE()
    @date   seojt on 2006-7-13, 14:30:50
            - ASSERT_HEAP()
    @date   jintaeks on 2008-03-13, 10:32
            - VAR_NAME() 추가
                --  변수의 이름을 문자열로 얻는 매크로
*/

#ifndef _KJOHNGEN_H
#define _KJOHNGEN_H

//#pragma warning(push)
#pragma warning(disable: 4503) // decorated name length exceeded, name was truncated
#pragma warning(disable: 4786) // identifier was truncated to '255' characters in the debug information
#pragma warning(disable: 4788) // identifier was truncated to 'number' characters
#pragma warning(disable: 4995) // 'function': name was marked as #pragma deprecated
#pragma warning(disable: 4702) // unreachable code
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 4505) // unreferenced local function has been removed
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
//{{ robobeg : 2008-03-20
#pragma warning(disable:4530) // C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc
#pragma warning(disable:4244) // 'argument' : conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4714) // function 'function' marked as __forceinline not inlined
//}} robobeg : 2008-03-20
#pragma warning(disable:4512) // boost::algorithm::detail::is_classifiedF : 대입연산자를 생성하지 못했습니다. - jintaeks on 2008-09-25, 10:12


//{{ robobeg : 2008-03-20
#ifndef STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_DEPRECATE
#endif // #ifndef STRSAFE_NO_DEPRECATE

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // #ifndef _USE_MATH_DEFINES
//}} robobeg : 2008-03-20


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // #ifndef WIN32_LEAN_AND_MEAN

//{{ seojt // 2008-3-20, 8:57
// This define must occur before any headers are included - 존로빈스, Debugging Application, p.788
/** _CRTDBG_MAP_ALLOC 매크로를 활성화하면 malloc.h에서 에러가 발생해서 보류해 두다.
    - jintaeks on 2008-03-20, 9:02 */
//#ifndef _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC
//#endif // _CRTDBG_MAP_ALLOC
//}} seojt // 2008-3-20, 8:57
#include <assert.h>
#include <wchar.h>
#include <windows.h>
//#include <assert.h>
//#include <strsafe.h> // 포함시키지 말것 - jintaeks on 2007-12-31, 16:02
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <float.h>
#ifdef _DEBUG // 2006-1-12, 14:32:55
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif // #ifndef D3D_DEBUG_INFO
#endif // _DEBUG
#include <d3dx9.h> // KJohnGen.h uses DirectX9 library if required
#include <string>
// include CRTDBG.H after all other headers - 존로빈스, Debugging Application, p.788
#include <crtdbg.h>

//{{ robobeg : 2008-03-20

#define _KPREAL_FLOAT // 이 매크로를 define하면 KpReal을 float로 사용한다.

#ifdef _KPREAL_FLOAT
typedef float   KpReal;
#else
typedef double  KpReal;
#endif // #ifdef _KPREAL_FLOAT

#ifndef KHRMERGE
#define KHRMERGE( hr, hr2 )  if ( SUCCEEDED( hr ) )  (hr) = (hr2);
#endif // #ifndef KHRMERGE

//}} robobeg : 2008-03-20


//------------------------------------------------------------------------------
// _DEBUG
//
#if defined(_DEBUG)

#ifndef _INC_CRTDBG
#define _ASSERTE    assert
#endif // _INC_CRTDBG

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp_)         _ASSERTE(exp_)
#endif // #if !defined(ASSERT)

// TRACE
//
    #if !defined(TRACE)
        inline void _tracew(const WCHAR* fmt_, ...)
        {
            WCHAR   out[4096];
            va_list argList;

            va_start( argList, fmt_ );
            //StringCchVPrintfW( out, ARRAY_SIZE( out ), fmt_, argList );
            ::vswprintf( out, fmt_, argList );
            va_end( argList );
            ::OutputDebugStringW( out );
        }//_tracew()

        inline void _tracea(const char* pszFormat_, ...)
        {
            char    out[4096];
            va_list argList;

            va_start( argList, pszFormat_ );
            //StringCchVPrintfA( out, ARRAY_SIZE( out ), pszFormat_, argList );
            ::vsprintf( out, pszFormat_, argList );
            va_end( argList );
            ::OutputDebugStringA( out );
        }//_tracea()
        #if !defined(TRACEA)
        #define TRACEA              _tracea
        #endif // #if !defined(TRACEA)
        #define TRACEW              _tracew

    #ifdef UNICODE // 2005-5-2, 11:40:58
    #define TRACE               _tracew
    #else
    #define TRACE               _tracea
    #endif // #ifdef UNICODE // 2005-5-2, 11:40:58
    #endif // #if !defined(TRACE)

/** @example    BREAK()
    BREAK( i == 3 );
*/
#ifndef BREAK
#define BREAK(cond_)         if (cond_) __asm int 3
#endif // #ifndef BREAK

/** @example    CASSERT()
    CASSERT( sizeof(int) == 4 );
*/
/// compile time assertion
#ifndef CASSERT
#define CASSERT(expn_)       typedef char __C_ASSERT__[(expn_)?1:-1]
#endif // #ifndef CASSERT

#else // not defined _DEBUG ////////////////////////////////////////////////////

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp)
#endif // #if !defined(ASSERT)

// TRACE
//
    #if !defined(TRACE)
        //#define TRACE               1 ? (void)0 :
        /** The __noop intrinsic specifies that a function should be ignored and the argument list
            be parsed but no code be generated for the arguments. It is intended for use in global
            debug functions that take a variable number of arguments.
            - jintaeks on 2008-03-04, 10:00 */
        #define TRACE               __noop
        #if !defined(TRACEA)
        //#define TRACEA              1 ? (void)0 :
        #define TRACEA              __noop
        #endif // #if !defined(TRACEA)
        //#define TRACEW              1 ? (void)0 :
        #define TRACEW              __noop
    #endif // #if !defined(TRACE)

#ifndef BREAK
#define BREAK(cond_)
#endif // #ifndef BREAK

#ifndef CASSERT
#define CASSERT(expn_)
#endif // #ifndef CASSERT

#endif // defined(_DEBUG)
//
//------------------------------------------------------------------------------


inline void _traceAlways(const char* pszFormat_, ...)
{
    char    out[4096];
    va_list argList;

    va_start( argList, pszFormat_ );
    //StringCchVPrintfA( out, ARRAY_SIZE( out ), pszFormat_, argList );
    ::vsprintf( out, pszFormat_, argList );
    va_end( argList );
    ::OutputDebugStringA( out );
}//_traceAlways()
#define TRACE_ALWAYS    _traceAlways


// VERIFY
//
#if !defined(VERIFY)
#ifdef _DEBUG
#define VERIFY(exp_)         _ASSERTE(exp_)
#else
#define VERIFY(exp_)         (exp_)
#endif
#endif // #if !defined(VERIFY)


// VERIFYHR
//
#if !defined(VERIFYHR)
#ifdef _DEBUG
#define VERIFYHR(exp_)      VERIFY(SUCCEEDED( (exp_) ))
#else
#define VERIFYHR(exp_)      (exp_)
#endif
#endif // #if !defined(VERIFYHR)


// ASSERTHR
//
#if !defined(ASSERTHR)
#ifdef _DEBUG
#define ASSERTHR(exp_)      ASSERT(SUCCEEDED( (exp_) ))
#else
#define ASSERTHR(exp_)
#endif
#endif // #if !defined(ASSERTHR)


//{{ seojt // 2008-3-17, 7:26
#ifdef _DEBUG
    #ifndef MSG_IF
    #define MSG_IF( expr, msg ) \
        if ( (expr) ) \
        { \
            if ( 1 == ::_CrtDbgReport( _CRT_ASSERT, \
            __FILE__, __LINE__, __FUNCDNAME__, (msg) ) ) \
                ::DebugBreak(); \
        }
    #endif // #ifndef MSG_IF

    #ifndef MSG_RET_IF
    #define MSG_RET_IF( expr, msg, ret ) \
        if ( (expr) ) \
        { \
            if ( 1 == ::_CrtDbgReport( _CRT_ASSERT, \
            __FILE__, __LINE__, __FUNCDNAME__, (msg) ) ) \
                ::DebugBreak(); \
            return (ret); \
        }
    #endif // #ifndef MSG_RET_IF
#else
    #ifndef MSG_IF
    #define MSG_IF( expr, msg ) \
        if ( (expr) ) \
        { \
            TRACE_ALWAYS( msg ); \
        }
    #endif // #ifndef MSG_IF

    #ifndef MSG_RET_IF
    #define MSG_RET_IF( expr, msg, ret ) \
        if ( (expr) ) \
        { \
            TRACE_ALWAYS( msg ); \
            return (ret); \
        }
    #endif // #ifndef MSG_RET_IF
#endif // #ifdef _DEBUG
//}} seojt // 2008-3-17, 7:26


//{{ robobeg : 2006-08-02 // 10:53
//------------------------------------------------------------------------------
// NEWASSERT, NEWVERIFY, NEWVERIFYRET
//

#ifdef _DEBUG
    #ifndef NEWASSERT
    #define NEWASSERT( expr, msg ) \
        if ( !(expr) ) \
        { \
		    if ( 1 == ::_CrtDbgReport( _CRT_ASSERT, \
            __FILE__, __LINE__, __FUNCDNAME__, (msg) ) ) \
                ::DebugBreak(); \
        }
    #endif // #ifndef NEWASSERT

    #ifndef NEWVERIFY
    #define NEWVERIFY( expr, msg )	NEWASSERT( (expr), (msg) )
    #endif // #ifndef NEWVERIFY

    #ifndef NEWVERIFYRET
    #define NEWVERIFYRET( expr, ret, msg ) \
        if ( !(expr) ) \
        { \
		    if ( 1 == ::_CrtDbgReport( _CRT_ASSERT, \
            __FILE__, __LINE__, __FUNCDNAME__, (msg) ) ) \
                ::DebugBreak(); \
            return (ret); \
        }
    #endif // #ifndef NEWVERIFYRET
#else
    #ifndef NEWASSERT
    #define NEWASSERT( expr, msg )
    #endif // #ifndef NEWASSERT
    #ifndef NEWVERIFY
    #define NEWVERIFY( expr, msg ) (expr)
    #endif // #ifndef NEWVERIFY
    #ifndef NEWVERIFYRET
    #define NEWVERIFYRET( expr, ret, msg ) \
        if ( !(expr) ) \
            return (ret);
    #endif // #ifndef NEWVERIFYRET
#endif // #ifdef _DEBUG
//}} robobeg : 2006-08-02 // 10:53


// SAFE_DELETE
//
#ifndef SAFE_DELETE
#define SAFE_DELETE(p_)       { if(p_) { delete (p_); (p_)=NULL; } }
#endif


// SAFE_DELETE_ARRAY
//
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p_) { if(p_) { delete[] (p_); (p_)=NULL; } }
#endif


// SAFE_RELEASE
//
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p_)      { if(p_) { (p_)->Release(); (p_)=NULL; } }
#endif


//{{ robobeg : 2007-02-20, 14:34

// 
// SAFE_FREE
// SAFE_REALLOC
// SAFE_TYPEDREALLOC
// 

#ifndef SAFE_FREE
#define SAFE_FREE(p)        { if(p) { ::free(p); (p)=NULL; } }
#endif // #ifndef SAFE_FREE

#ifndef SAFE_REALLOC
/// p가 가리키는 곳에 바이트 s만큼을 realloc한다.
/// @param  p : [in,out] 메모리 포인터
/// @param  s : [in] size in bytes(malloc()의 첫번째, realloc()의 두번재 파라미터)
#define SAFE_REALLOC(p,s)   \
{   if((s)==0){ SAFE_FREE(p); }   \
else if (!(p))    { (p) = ::malloc(s); ASSERT(p); } \
else {  (p) = ::realloc((p),(s)); ASSERT(p); } }
#endif // #ifndef SAFE_REALLOC

#ifndef SAFE_TYPEDREALLOC
/// p가 가리키는 곳에 type을 s개 할당한다.
#define SAFE_TYPEDREALLOC(type,p,s)   \
{   if((s)==0){ SAFE_FREE(p); }   \
else if (!(p))    { (p) = (type*) ::malloc((s) * sizeof(type)); ASSERT(p); } \
else {  (p) = (type*) ::realloc((p), (s) * sizeof(type)); ASSERT(p); } }
#endif // #ifndef SAFE_TYPEDREALLOC

//}} robobeg : 2007-02-20, 14:34


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a_)       (sizeof(a_)/sizeof((a_)[0]))
#endif // #ifndef ARRAY_SIZE


inline const char*  SAFE_STRING( const std::string& strIn_ )
{
    return  ( strIn_.empty() ) ? "" : strIn_.c_str(); 
}//SAFE_STRING()

inline const char*  SAFE_STRING( const char* pszIn_ )
{
    return  ( pszIn_ == NULL ) ? "" : pszIn_;
}//SAFE_STRING()

inline const wchar_t*  SAFE_STRING( const std::wstring& wstrIn_ )
{
    return  ( wstrIn_.empty() ) ? L"" : wstrIn_.c_str(); 
}//SAFE_STRING()

inline const wchar_t*  SAFE_STRING( const wchar_t* pwszIn_ )
{
    return  ( pwszIn_ == NULL ) ? L"" : pwszIn_;
}//SAFE_STRING()


#if !defined( _MSC_VER ) || ( _MSC_VER < 1300 )

template<typename T>
inline void STL_CLEAR( T& InOut_ )
{
    T  temp;
    InOut_.swap( temp );
}//STL_CLEAR()

#else

template<typename T>
inline void STL_CLEAR( T& InOut_ )
{
    InOut_.clear();
}//STL_CLEAR()

#endif // #if _MSC_VER < 1300


/// 전역변수 초기화 순서와 무관한 안전한 스태틱 멤버 변수를 정의하는 매크로
/// 스태틱 멤버 변수 접근은 Access##Name(), Get##Name() 으로 한다.
/** 전역변수 초기화 순서와 무관하다는 의미는, SAFE_STATIC_MEMBER()로 선언된 변수들은
    프로그램의 시작 전에 무조건 메모리할당과 초기화가 진행된다는 의미이다.
    이렇게 구현되지 않은 스태틱 변수는 변수를 처음 접근할 때 초기화된다.

    @param  Type: 변수의 type을 지정한다.
    @param  Name: Access##Name(), Get##Name()에 해당하는 변수이름의 일부를 지정한다.
            클래스 내부에서 실제 이름은 s_member이지만, 매크로가 변수이름에 따라
            유일하게 클래스들을 생성하도록 설계하였다.
    - jintaeks on 2008-07-22, 15:15 */
#define     SAFE_STATIC_MEMBER( Type, Name ) \
    class _KSafeStaticMember##Name \
    { \
    private: \
        static  _KSafeStaticMember##Name    ms_staticMember; \
        __declspec(noinline) static Type& _NonlinearAccess()   { static Type s_member; ms_staticMember._DoNothing();  return s_member; } \
        inline void           _DoNothing()       {} \
    public: \
        inline _KSafeStaticMember##Name()   { Access(); } \
        inline static Type&   Access()        { static Type& s_ref = _NonlinearAccess(); return s_ref; } \
    };\
    _KSafeStaticMember##Name    _m_safeStaticMemberInitializer##Name; \
    inline  static Type&   Access##Name()   { return _KSafeStaticMember##Name::Access(); } \
    inline  static const Type& Get##Name()  { return _KSafeStaticMember##Name::Access(); }

#define     SAFE_STATIC_MEMBER_INIT( Type, Name, InitValue ) \
    class _KSafeStaticMember##Name \
    { \
    private: \
        static  _KSafeStaticMember##Name    ms_staticMember; \
        __declspec(noinline) static Type& _NonlinearAccess()   { static Type s_member = (InitValue); ms_staticMember._DoNothing(); return s_member; } \
        inline void           _DoNothing()       {} \
    public: \
        inline _KSafeStaticMember##Name()   { Access(); } \
        inline static Type&   Access()        { static Type& s_ref = _NonlinearAccess(); return s_ref; } \
    }; \
    _KSafeStaticMember##Name    _m_safeStaticMemberInitializer##Name; \
    inline  static Type&   Access##Name()   { return _KSafeStaticMember##Name::Access(); } \
    inline  static const Type& Get##Name()  { return _KSafeStaticMember##Name::Access(); }


/// 각 SAFE_STATIC_MEMBER로 정의된 스태틱 멤버 변수에 대응하여 .cpp 파일에 IMPLEMENT_SAFE_STATIC_MEMBER 를 선언해야 함
/** @example

    1) KTrafficSystem.h 파일의 구현:

        class KTrafficSystem : public KD3DObjectFramework
        {
        private:
            SAFE_STATIC_MEMBER( KFovEx, Fov )
            ...
        };

    2) KTrafficSystem.cpp 파일의 구현:

        IMPLEMENT_SAFE_STATIC_MEMBER( KTrafficSystem, Fov );

*/

#define     IMPLEMENT_SAFE_STATIC_MEMBER( Class, Name ) \
    Class::_KSafeStaticMember##Name Class::_KSafeStaticMember##Name::ms_staticMember;



/** @exam   VAR_NAME
    int* p = new int;
    TRACEA( "%s = %8.8x\r\n", VAR_NAME(p), p );
    delete p;
*/
/// 변수의 이름을 문자열로 얻기 위해 사용한다.
#ifndef VAR_NAME
#define VAR_NAME(p)     (#p)
#endif // VAR_NAME

#ifndef VAR_WNAME
#define VAR_WNAME(p)     (L#p)
#endif // VAR_WNAME

#ifndef LINE1
#define LINE1(x_) #x_
#endif // #ifndef LINE1
#ifndef LINE
#define LINE(x_) LINE1(x_)
#endif // #ifndef LINE
/** @example    TODO()

    #pragma TODO( "fix here" )
    #pragma NOTE( "check here" )

*/
#ifndef TODO
#define TODO(msg_) message ( __FILE__ "(" LINE(__LINE__)  "): [TODO] " msg_ )
#endif // #ifndef TODO
#ifndef NOTE
#define NOTE(msg_) message ( __FILE__ "(" LINE(__LINE__)  "): [NOTE] " msg_ )
#endif // #ifndef NOTE


/** @example ASSERT_FLOAT()
    D3DXVECTOR3 v;
    // ...
    D3DXVec3Normalize( &v );
    ASSERT_FLOAT( v.x );
*/
#ifndef ASSERT_FLOAT
#ifdef _DEBUG
    #define ASSERT_FLOAT(f_) if ( f_ >= 0 ){} else if ( f_ < 0 ){} else { assert( f_ && !"float error" ); }
#else // #ifdef _DEBUG
    #define ASSERT_FLOAT(f_)
#endif // _DEBUG
#endif // #ifndef ASSERT_FLOAT


//------------------------------------------------------------------------------
// CRT debug macros
//
#ifndef INIT_CRTDEBUG
/** 메모리 할당과 해제 요청이 있을 때마다 모든 메모리를 검사해서
    overwrite나 underwrite를 발견하고 싶으면 flag_에

        _CRTDBG_CHECK_ALWAYS_DF

    를 명시한다.
    - jintaeks on 2008-03-20, 10:36 */
#define INIT_CRTDEBUG(flag_)    ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|flag_)
#endif // #ifndef INIT_CRTDEBUG


/** @example    ASSERT_MEMORY()
	@code

    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        INIT_CRTDEBUG();

        int*    p = new int;
        delete p;

        *p = 3;             // using after deletion
        ASSERT_MEMORY();    // see and check call stack window
        delete p;
    }//main()

	@endcode
*/
#ifndef ASSERT_MEMORY
#define ASSERT_MEMORY()     ASSERT( ::_CrtCheckMemory() )
#endif // #ifndef ASSERT_MEMORY


/** @example    BREAK_ALLOC()
	@code

  //------------------------------------------------------------------------------
  //
  1) memory leak example:

    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        INIT_CRTDEBUG();

        int*    p = new int;

        *p = 3;
    }//main()

    // debug message:
    Detected memory leaks!
    Dumping objects ->
    {44} normal block at 0x00372DA8, 4 bytes long.
    Data: <    > 03 00 00 00
    Object dump complete.

    // @note    allocation number is {44}, so you can set break point at 44th alloc
    //          see below example


  //------------------------------------------------------------------------------
  //
  2) break on mal-alloc {44}:

    #ifdef PROJECT_PCH
    #define _TOSTR1(x)    #x
    #define _TOSTR(x)     _TOSTR1(x)
    #include _TOSTR(PROJECT_PCH)
    #endif // PROJECT_PCH
    #include "KJohnGen.h"
    #include <iostream>

    void main()
    {
        BREAK_ALLOC( 44 ); // install break CRT point condition
        INIT_CRTDEBUG();

        int*    p = new int;

        *p = 3;
    }//main()

	@endcode
*/
#ifndef BREAK_ALLOC
#define BREAK_ALLOC(line_)   ::_CrtSetBreakAlloc( line_ )
#endif // #ifndef BREAK_ALLOC


/** @example ASSERT_HEAP()
    @code

    #include "KJohnGen.h"

    void main()
    {
        int* p = new int;
        int i;
        ASSERT_HEAP( p );
        delete p;
        ASSERT_HEAP( p ); // assertion fails
        ASSERT_HEAP( &i ); // assertion fails
    }//main()

    @endcode
*/
#ifndef ASSERT_HEAP
#define ASSERT_HEAP(p)          ASSERT( ::_CrtIsValidHeapPointer(p) );
#endif // #ifndef ASSERT_HEAP

#ifndef ASSERT_POINTER
#define ASSERT_POINTER(p,type)  ASSERT( ::_CrtIsValidPointer( p, sizeof(type), TRUE ) );
#endif // ASSERT_POINTER

#ifndef ASSERT_POINTER_READONLY
#define ASSERT_POINTER_READONLY(p,type)  ASSERT( ::_CrtIsValidPointer( p, sizeof(type), FALSE ) );
#endif // ASSERT_POINTER_READONLY


#ifndef NONCOPYABLE
#define NONCOPYABLE(name_)      private: name_(const name_ &); const name_ & operator=(const name_ &);
#endif // NONCOPYABLE


inline D3DXVECTOR2  operator * ( const D3DXVECTOR2& v2In1_, const D3DXVECTOR2& v2In2_ )
{
    return  D3DXVECTOR2( v2In1_.x * v2In2_.x, v2In1_.y * v2In2_.y );
}//


inline D3DXVECTOR3  operator * ( const D3DXVECTOR3& v3In1_, const D3DXVECTOR3& v3In2_ )
{
    return  D3DXVECTOR3( v3In1_.x * v3In2_.x, v3In1_.y * v3In2_.y, v3In1_.z * v3In2_.z );
}//


inline D3DXVECTOR4  operator * ( const D3DXVECTOR4& v4In1_, const D3DXVECTOR4& v4In2_ )
{
    return  D3DXVECTOR4( v4In1_.x * v4In2_.x, v4In1_.y * v4In2_.y, v4In1_.z * v4In2_.z, v4In1_.w * v4In2_.w );
}//

//{{ robobeg : 2008-04-10

//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables
//--------------------------------------------------------------------------------------
//#define KSET_MEMBER( type, mod, name )      inline void Set##name( type mod##In_ )  { m_##mod##name = mod##In_; }
//#define KGET_MEMBER( type, mod, name )      inline const type & Get##name() const { return m_##mod##name; }
//#define KSET_COPYMEMBER( type, mod, name )  inline void Set##name( const type & mod##In_ ) { m_##mod##name = mod##In_; }
//#define KGET_COPYMEMBER( type, mod, name )  inline const type Get##name() const { return m_##mod##name; }
//#define KGET_SET_MEMBER( type, mod, name )  KSET_MEMBER( type, mod, name ) KGET_MEMBER( type, mod, name )
//#define KGET_SET_COPYMEMBER( type, mod, name )  KSET_COPYMEMBER( type, mod, name ) KGET_COPYMEMBER( type, mod, name )
//
//#define KSET_CONDITION( name )              inline void Set##name( bool bIn_ ) { m_b##name = bIn_; }
//#define KIS_CONDITION( name )               inline bool Is##name() const { return m_b##name; }
//#define KIS_SET_CONDITION( name )           KSET_CONDITION( name )  KIS_CONDITION( name )
//
//#define KACCESS_MEMBER( type, mod, name )   inline type& Access##name() { return m_##mod##name; }
//#define KGET_ACCESS_MEMBER( type, mod, name ) KACCESS_MEMBER( type, mod, name )  KGET_MEMBER( type, mod, name )
//
//#define KVALID_PMEMBER( mod, name )         inline bool  IsValid##name() const { return m_##mod##name != NULL; }
//#define KACCESS_PMEMBER( type, mod, name )  inline type& Access##name() { ASSERT( m_##mod##name != NULL ); return *m_##mod##name; }
//#define KGET_PMEMBER( type, mod, name )     inline const type& Get##name() const { ASSERT( m_##mod##name != NULL ); return *m_##mod##name; }
//#define KGET_ACCESS_VALID_PMEMBER( type, mod, name ) KVALID_PMEMBER( mod, name ) KACCESS_PMEMBER( type, mod, name ) KGET_PMEMBER( type, mod, name )

//}} robobeg : 2008-04-10


//#pragma warning(pop)

#endif // _KJOHNGEN_H
