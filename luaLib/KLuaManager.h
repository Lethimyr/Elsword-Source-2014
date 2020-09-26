/** @file
@author cruise@kogsoft.com
*/

#ifndef _KOG_KLUAMANAGER_H_
#define _KOG_KLUAMANAGER_H_ "$Id: KLuaManager.h,v 1.6 2003/02/17 03:50:55 cvs Exp $"
#pragma once // for _MSC_VER > 1000

#include <string>
#include <sstream> 
#include <malloc.h>


#include "lua_tinker.h"


#include <windows.h>
#include <assert.h>
#include <WTypes.h>

#ifndef STRSAFE_NO_DEPRECATE
#	define STRSAFE_NO_DEPRECATE
#endif
#include <strsafe.h>

/** LUA manager class.
LUA언어로 작성된 configuration file을 읽어 오고, 그 내용을 C++
프로그램으로 가져오기 위한 class. LUA의 기본형은 number (float 형태로
저장), string 2가지 뿐이고, 이들을 table (structure 개념과 유사)로
저장할 수 있다. table 내의 table도 지원된다. 자세한 내용은 LUA manual을
참고하기 바란다.
 
@verbinclude testKLuaManager.cpp
*/




//#if defined(DEBUG) || defined(_DEBUG)
//#	define //CHECK_LUA_THREAD_ID ASSERT( KLuaManager::m_ThreadIDofObjectLoaderInSimulationServer != ::GetCurrentThreadId() );
//#else
//#	define //CHECK_LUA_THREAD_ID
//#endif 





#ifndef LUA_RESULT
#	define LUA_RESULT bool
#	define LUA_OK true
#	define LUA_FAIL false
#endif // LUA_RESULT





//{{ robobeg : 2008-10-29

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

#else // not defined _DEBUG ////////////////////////////////////////////////////

// ASSERT
//
#if !defined(ASSERT)
#define ASSERT(exp)
#endif // #if !defined(ASSERT)

#endif // defined(_DEBUG)





#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))
#endif ARRAY_SIZE


// Lua stack 작업할 때 stack push/pop 알치를 체크하기 위해
/** _L을 사용한 이유는 재귀적으로 블록을 사용했을 때 이름 충돌로 인한 컴파일러 에러를 막기 위한 것이다.
    사용자가 _L이름을 사용하지 않는다고 가정한다.
    - jintaeks on 2008-05-29, 10:28 */
#define KLUABEGIN( stack )   {  ASSERT( (stack) != NULL ); lua_State* _L = stack; lua_State* L = _L; unsigned _iCurTop, _iLuaTop; _iCurTop = _iLuaTop = (unsigned) lua_gettop( L );
#define KLUAEND()               ASSERT( L != NULL ); _iCurTop = (unsigned) lua_gettop( L ); ASSERT( _iLuaTop == _iCurTop ); }
#define KLUAENDWITHPUSH( arg )  ASSERT( L != NULL ); _iCurTop = (unsigned) lua_gettop( L ); ASSERT( _iLuaTop + (arg) == _iCurTop ); }
#define KLUAENDWITHPOP( arg )  ASSERT( L != NULL ); _iCurTop = (unsigned) lua_gettop( L ); ASSERT( _iLuaTop - (arg) == _iCurTop ); }
#define KLUAENDUNWIND()         ASSERT( L != NULL ); lua_settop( L, _iLuaTop ); }


//}} robobeg : 2008-10-29



//#ifndef PROFILE_BUILD
//	#define KTDXPROFILE()
//	#define KTDXPROFILE2(name)
//	#define KTDXPROFILE_BEGIN(name)
//	#define KTDXPROFILE_END()
//#endif


#define LUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == false ) \
		(dest) = (initValue); \
}

#define LUA_GET_VALUE_UTF8( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager).GetValueUtf8( (key), (dest) ) == false ) \
		(dest) = (initValue); \
}

#define LUA_GET_CHARARRAY( luaManager, key, dest, size, initValue ) \
{ \
	if( dest != NULL && (luaManager).GetValue( (key), (dest), (size) ) == false ) \
		StringCchCopyA( (dest), (size), (initValue) ); \
}

#define LUA_GET_CHARARRAY_UTF8( luaManager, key, dest, size, initValue ) \
{ \
	if( dest != NULL && (luaManager).GetValueUtf8( (key), (dest), (size) ) == false ) \
		StringCchCopyA( (dest), (size), (initValue) ); \
}


#define LUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
}

#define LUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == false ) \
	{ \
		(dest) = (initValue); \
	} \
	else \
	{ \
		(dest) = (destType)intBuffer; \
	} \
}

#define LUA_GET_VALUE_RETURN_ENUM( luaManager, key, dest, destType, initValue, returnCode ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
	else \
	{ \
		dest = (destType)(intBuffer); \
	} \
}


//pointer
#define PLUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == false ) \
		(dest) = (initValue); \
}

#define PLUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
}

#define PLUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager)->GetValue( (key), intBuffer ) == false ) \
	{ \
		(dest) = (initValue); \
	} \
	else \
	{ \
		(dest) = (destType)intBuffer; \
	} \
}

#define PLUA_GET_VALUE_RETURN_ENUM( luaManager, key, dest, destType, initValue, returnCode ) \
{ \
	int intBuffer = 0; \
	if( (luaManager)->GetValue( (key), intBuffer ) == false ) \
	{ \
		(dest) = (initValue); \
		returnCode; \
	} \
	else \
	{ \
		dest = (destType)(intBuffer); \
	} \
}


  
//{{ 2008. 3. 18  최육사  에러 스트링
#define LUA_GET_VALUE_RETURN_ERR( luaManager, key, dest, initValue, errStr, returnCode ) \
{ \
 if( (luaManager).GetValue( (key), (dest) ) == LUA_FAIL ) \
 { \
 (dest) = (initValue); \
 (errStr) = (key); \
 returnCode; \
 } \
}
//}}




//{{ 2008. 3. 18  최육사  에러 스트링
#define LUA_GET_VALUE_RETURN_ENUM_ERR( luaManager, key, dest, destType, initValue, errStr, returnCode ) \
{ \
 int intBuffer = 0; \
 if( (luaManager).GetValue( (key), intBuffer ) == LUA_FAIL ) \
 { \
 (dest) = (initValue); \
 (errStr) = (key); \
 returnCode; \
 } \
 else \
 { \
 dest = (destType)(intBuffer); \
 } \
}
//}} 



//{{ dmlee 2008.06.01
//
//#define LUA_GET_NODEFAULT_( from, key, ret ) \
//	KNewLuaManager::Get_( (from), (key), (ret) )
//
//#define LUA_GET_( from, key, ret, def ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (ret) ) ) \
//	{ (ret) = (def); } \
//} 
//
//#define LUA_GET_RETURN_( from, key, ret, def, returnCode ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (ret) ) )	\
//	{ (ret) = (def); returnCode; } \
//}
//
//
//
//
//#define LUA_GET_SUFFIX_NODEFAULT_( from, key, suffix, ret ) \
//	KNewLuaManager::Get_( (from), (key), (suffix), (ret) )
//
//#define LUA_GET_SUFFIX_( from, key, suffix, ret, def ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (suffix), (ret) ) ) \
//	{ (ret) = (def); } \
//}
//
//#define LUA_GET_SUFFIX_RETURN_( from, key, suffix, ret, def, returnCode ) \
//{ \
//	if( false == KNewLuaManager::Get_( (from), (key), (suffix), (ret) ) ) \
//	{ (ret) = (def); returnCode; } \
//}
//
//
//class KNewLuaManager
//{
//public:
//	// global에서 index로 값 읽기
//	template< typename T >
//		static inline bool Get_( lua_State* pLuaState, const int iIndex, T& ret )
//	{		
//		return lua_tinker::get_( pLuaState, iIndex, ret );
//	}
//
//	template<>
//		static inline bool Get_( lua_State* pLuaState, const int iIndex, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = lua_tinker::get_<std::string>( pLuaState, iIndex, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//
//	// global에서 key name으로 값 읽기
//	template< typename T >
//		static inline bool Get_( lua_State* pLuaState, const char* szName, T& ret )
//	{		
//		return lua_tinker::get_( pLuaState, szName, ret );
//	}
//
//	template<>
//		static inline bool Get_( lua_State* pLuaState, const char* szName, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = lua_tinker::get_<std::string>( pLuaState, szName, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//
//
//	// global에서 key name으로 값 읽기 (변형)
//	template< typename T >
//		static inline bool Get_( lua_State* pLuaState, const char* szName, const int iSuffix, T& ret )
//	{	
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		return lua_tinker::get_( pLuaState, strm.str().c_str(), ret );
//	}
//
//	template<>
//		static inline bool Get_( lua_State* pLuaState, const char* szName, const int iSuffix, std::wstring& ret )
//	{		
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		std::string strBuf = "";
//		bool bOK = lua_tinker::get_<std::string>( pLuaState, strm.str().c_str(), strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//
//
//
//	// lua table에서 index로 값 읽기
//	template< typename T >
//		static inline bool Get_(lua_tinker::table& luaTable, const int iIndex, T& ret )
//	{		
//		return luaTable.get_( iIndex, ret );
//	}
//
//	template<>
//		static inline bool Get_(lua_tinker::table& luaTable, const int iIndex, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = luaTable.get_( iIndex, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//	// lua table에서 key name으로 값 읽기
//	template< typename T >
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, T& ret )
//	{		
//		return luaTable.get_( szName, ret );
//	}
//
//	template<>
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, std::wstring& ret )
//	{		
//		std::string strBuf = "";
//		bool bOK = luaTable.get_<std::string>( szName, strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//	// lua table에서 key name으로 값 읽기 ( 변형 )
//	template< typename T >
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, const int iSuffix, T& ret )
//	{		
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		return luaTable.get_( strm.str().c_str(), ret );
//	}
//
//	template<>
//		static inline bool Get_(lua_tinker::table& luaTable, const char* szName, const int iSuffix, std::wstring& ret )
//	{		
//		std::stringstream strm;
//		strm << szName << iSuffix;
//
//		std::string strBuf = "";
//		bool bOK = luaTable.get_( strm.str().c_str(), strBuf );
//		if( true == bOK )
//		{
//			_ConvertCharToWCHAR( ret, strBuf.c_str() );
//		}
//		return bOK;
//	}
//
//private:
//	static inline void _ConvertCharToWCHAR( std::wstring& dest, const char* pSrc )
//	{
//		dest.clear();
//		if ( pSrc == NULL )
//			return;
//
//		int iSrcLength = (int)strlen( pSrc );
//		if ( iSrcLength <= 0 )
//		{
//			dest = L"";
//			return;
//		}
//
//		int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, NULL, 0 );
//		if ( iDestSize <= 0 )
//		{
//			dest = L"";
//			return;
//		}
//
//		WCHAR* wcharBuf = (WCHAR*) _alloca( iDestSize * sizeof( WCHAR ) );
//		int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, wcharBuf, iDestSize );
//		dest.assign( wcharBuf, iNewSize );
//	}
//};

//}} dmlee 2008.06.01


class KLuaManager
{
public:

//{{ robobeg : 2008-10-28

// KLuaManager 를 생성할 때 항상 외부에서 lua_State 를 넘겨받도록 수정
// 단 bInNewEnv_ = true 인 경우에는 lua_State의 global environment 대신
// 독립적인 새 global environment 가 사용된다.
//
// 한 KLuaManager 에 각종 라이브러리와 global variable들을 설정한 후
// SetAsTemplateEnvironment 명령을 수행하고 나면
// 이후 생성되는 KLuaManager 개체들은 모두 이 environment 를 기본적으로 상속받는다.
//
// 이전의 KLuaManager(void) 형태의 constructor 에 대응되는 constructor argument는 
//   KLuaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true )
// 이다.

//{{ robobeg : 2012-11-17
	// 아무 lua state 도 할당되지 않은 상태. 이 녀석을 사용하면 크래쉬 발생한다!
	KLuaManager();
//}} robobeg : 2012-11-17


    /// Default constructor.
    //KLuaManager(void);
    KLuaManager( lua_State* pLuaState, int iTableDepth = 1, bool bInNewEnv_ = false );

    /// Constructor with initial internal stack size.
    //KLuaManager(int iStackSize);

	/// Constructor for LuaTinker Helper
	//KLuaManager(lua_State* pLuaState, int iTableDepth = 1);

//}} robobeg : 2008-10-28

    /// Destructor.
    ~KLuaManager(void);

//{{ robobeg : 2012-11-17
	// 설정된 lua state 를 제거하고 초기 상태로 만든다. 
	void	DetachLuaState();
	void	AssignNewLuaState( lua_State* pLuaState, int iTableDepth, bool bInNewEnv_ );
//}} robobeg : 2012-11-17

private:
    /// Copy constructor: DISABLED.
    KLuaManager(const KLuaManager&);
    /// Assignment operator: DISABLED.
    void operator=(const KLuaManager&);
public:

//{{ robobeg : 2008-10-28
    void    Reset();

    /// 현재 KLuaManager 의 environment를 이후 모든 KLuaManager 개체들이 상속받도록 한다.
    //HRESULT SetAsTemplateEnvironment();

    /// 현재 environment 에 로드된 global variables 들을 original lua state 의 global environment 에 복사한다.
    //HRESULT ExportEnvironment();

	bool	ExportFunctionsToGlobalEnv();

//}} robobeg : 2008-10-28


    /// String representation.
    std::string ToString(void) const;
    /// Get the low-level LUA state.
    inline lua_State* GetLuaState(void) { return m_pkLuaState; }

    // COMMAND EXECUTION ***********************************************

    /// string으로 주어진 LUA 명령을 실행한다.
    bool DoString(const char* pszStr, bool bLogOutput = true);
	/// 메모리 로딩
	bool DoMemory( const char* pRealFile, const long nSize, bool bLogOutput = true )
	{
		//CHECK_LUA_THREAD_ID;


#ifdef _ENCRIPT_SCRIPT_
		pRealFile = XORDecrypt( pRealFile, nSize );
#endif

		bool bRetVal = DoMemoryNotEncript( pRealFile, nSize );

#ifdef _ENCRIPT_SCRIPT_
		SAFE_DELETE_ARRAY( pRealFile );
#endif
		return bRetVal;
	}
	bool DoMemoryNotEncript( const char* pRealFile, const long nSize, bool bLogOutput = true )
    {
		//CHECK_LUA_THREAD_ID;

        //{{ 2009. 07. 17. jseop
        ASSERT( pRealFile );
        if( !pRealFile )
        {
            return false;
        }

        bool bOK;

        if( lua_getencoding( GetLuaState() ) == LUA_ENCODING_UTF8 )
        {
            ASSERT( nSize > 0 );
            if( nSize <= 0 )
            {
                return false;
            }

            if( pRealFile[0] == ( char )033 )
            {
                bOK = lua_tinker::dobuffer( m_pkLuaState, pRealFile, nSize );
            }
            else
            {
                ASSERT( nSize >= 3 );
                if( nSize < 3 )
                {
                    return false;
                }

                ASSERT( ( pRealFile[0] == ( char )0xef ) && ( pRealFile[1] == ( char )0xbb ) && ( pRealFile[2] == ( char )0xbf ) );
                if( ( pRealFile[0] != ( char )0xef ) || ( pRealFile[1] != ( char )0xbb ) || ( pRealFile[2] != ( char )0xbf ) )
                {
                    return false;
                }

                bOK = lua_tinker::dobuffer( m_pkLuaState, pRealFile + 3, nSize - 3 );
            }
        }
        else
        {
            bOK = lua_tinker::dobuffer( m_pkLuaState, pRealFile, nSize );
        }
        //{{

        return bOK;
    }

    /// file에 주어진 LUA 명령을 실행한다.
    bool DoFile(const char* pszFileName, bool bLogOutput = true);

    // TABLE HANDLING **************************************************

    /// pszName의 이름을 가지는 table을 LUA stack으로 가져온다.
	bool BeginTable(const char* pszName, int index, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
		char charBuf[255] = {0,};
		StringCchPrintfA( charBuf, ARRAY_SIZE(charBuf), "%s%d", pszName, index );
		return BeginTable( charBuf, bLogOutput );
	}
	
	bool BeginTable(const wchar_t* pszName, int index, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		char charBuf2[255] = {0,};
		StringCchPrintfA( charBuf2, ARRAY_SIZE(charBuf2), "%s%d", charBuf, index );
		return BeginTable( charBuf2, bLogOutput );
	}
	
	bool BeginTable(const wchar_t* pszName, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return BeginTable( charBuf, bLogOutput );
	}

    bool BeginTable(const char* pszName, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 table을 LUA stack으로 가져온다.
    bool BeginTable(int iIndex, bool bLogOutput = true);
    /// LUA stack에서 가장 최근에 사용한 table을 제거한다.
    bool EndTable(bool bLogOutput = true);

//{{ robobeg : 2008-10-28

	bool MakeTableReference( const wchar_t* pszName, int iInRef_, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return MakeTableReference( charBuf, iInRef_, bLogOutput );
	}//

    bool MakeTableReference( const char* pszName, int iInRef_, bool bLogOutput = true);

    bool BeginTableByReference( int iInRef_, bool bLogOutput = true );

//}} robobeg : 2008-10-28

//{{ robobeg : 2013-01-17
	int	GetLuaGeneralRef()	{ return m_iLuaGeneralRef; }
	int	MakeGeneralReference( const char* pszName, int type, bool bRawGlobal = false );

	int MakeFunctionReference( const std::wstring& funcName, bool bRawGlobal = false );
	int MakeFunctionReference( const wchar_t* funcName, bool bRawGlobal = false );
	int MakeFunctionReference( const char* funcName, bool bRawGlobal = false );
//}} robobeg : 2013-01-17

//{{ robobeg : 2013-10-24
    int GetNumIndexedValues();
    int GetNumIndexedValues( const char* pszName );
    int GetNumIndexedTables();
    int GetNumIndexedTables( const char* pszTableName );
//}} robobeg : 2013-10-24

    // GET BOOL-TYPE VALUE *********************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, bool& bValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, bValue, bLogOutput );
	}
    bool GetValue(const char* pszName, bool& bValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, bool& bValue, bool bLogOutput = true);

    // GET INTEGER-TYPE VALUE ******************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, short& sValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, sValue, bLogOutput );
	}
    bool GetValue(const char* pszName, short& sValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, short& sValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, int& iValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, iValue, bLogOutput );
	}
    bool GetValue(const char* pszName, int& iValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, int& iValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* szName, long& lValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, lValue, bLogOutput );
	}
    bool GetValue(const char* szName, long& lValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, long& lValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, unsigned short& usValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, usValue, bLogOutput );
	}
    bool GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, unsigned short& usValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, unsigned int& uiValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, uiValue, bLogOutput );
	}
    bool GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* szName, unsigned long& ulValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[255] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, ulValue, bLogOutput );
	}
    bool GetValue(const char* szName, unsigned long& ulValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput = true);

    // GET FLOAT-TYPE VALUE ********************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, float& fValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    bool GetValue(const char* pszName, float& fValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, float& fValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, double& fValue, bool bLogOutput = true)
	{
		//KTDXPROFILE();
		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    bool GetValue(const char* pszName, double& fValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, double& fValue, bool bLogOutput = true);





    // GET STRING-TYPE VALUE *******************************************

    /// pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, std::wstring& wstrValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        wstrValue.clear();

		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
		std::string strBuf;
		bool retval = GetValueUtf8( charBuf, strBuf, bLogOutput );
		if( retval == false )
			return retval;

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* ) _alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        wstrValue = wszBuffer;

		return true;
	}
	bool GetValue(const char* pszName, std::wstring& wstrValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        wstrValue.clear();

		std::string strBuf;
		bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
		if( retval == false )
			return retval;

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* ) _alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        wstrValue = wszBuffer;

		return true;
	}
	bool GetValue(const wchar_t* pszName, std::string& strValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        strValue.clear();

		char charBuf[511] = {0,};
		WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
        std::string strBuf;
		bool retval = GetValueUtf8( charBuf, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strValue = szBuffer;

        return true;
	}
    bool GetValue(const char* pszName, std::string& strValue, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();
        strValue.clear();

        std::string strBuf;
        bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strValue = szBuffer;

        return true;
    }
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
	bool GetValue(int iIndex, std::wstring& wstrValue, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

		//KTDXPROFILE();
        wstrValue.clear();

		std::string strBuf;
		bool retval = GetValueUtf8( iIndex, strBuf, bLogOutput );
		if( retval == false )
			return retval;

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* ) _alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        wstrValue = wszBuffer;

		return true;
	}
    bool GetValue(int iIndex, std::string& strValue, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();
        strValue.clear();

        std::string strBuf;
        bool retval = GetValueUtf8( iIndex, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strValue = szBuffer;

        return true;
    }
    /// pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
	bool GetValue(const wchar_t* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true)
	{
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();

        char charBuf[511] = {0,};
        WideCharToMultiByte( CP_UTF8, 0, pszName, -1, charBuf, 511, NULL, NULL );
        std::string strBuf;
        bool retval = GetValueUtf8( charBuf, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strncpy( pszValue, szBuffer, iMaxLen );

        return true;
	}
    bool GetValue(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();

        std::string strBuf;
        bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strncpy( pszValue, szBuffer, iMaxLen );

        return true;
    }

	bool GetValue( const char* pszName, WCHAR* wszValue, int bufferSize, bool bLogOutput = true )
	{
		//CHECK_LUA_THREAD_ID;

		ASSERT( NULL != wszValue );

		std::string strBuf;
		bool retval = GetValueUtf8( pszName, strBuf, bLogOutput );
		if( retval == false )
			return retval;

		::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszValue, ( int )bufferSize-1 );
		wszValue[ bufferSize-1 ] = NULL;
		return true;
	}



    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValue(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput = true)
    {
		//CHECK_LUA_THREAD_ID;

        //KTDXPROFILE();

        std::string strBuf;
        bool retval = GetValueUtf8( iIndex, strBuf, bLogOutput );
        if( !retval )
        {
            return false;
        }

        size_t tSize = strBuf.size() + 1;
        wchar_t* wszBuffer = ( wchar_t* )_alloca( tSize * sizeof( wchar_t ) );
        ASSERT( wszBuffer != NULL );
        size_t tNewSize = ::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wszBuffer, ( int )tSize );
        char* szBuffer = ( char* )_alloca( tNewSize * sizeof( char ) );
        ::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, ( int )tNewSize, NULL, NULL );
        strncpy( pszValue, szBuffer, iMaxLen );

        return true;
    }




private:
    bool GetValueUtf8(const char* pszName, std::string& strValue, bool bLogOutput = true);
    bool GetValueUtf8(int iIndex, std::string& strValue, bool bLogOutput = true);
    bool GetValueUtf8(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    bool GetValueUtf8(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput = true);
    /// Internal function: pszName의 이름을 가지는 변수/table을 LUA stack으로 가져온다.
    bool Get(const char* pszName);
    /// Internal function: (table 내에서) iIndex번째 변수/table을 LUA stack으로 가져온다.
    bool Get(int iIndex);

	void _GenerateLuaTableRef();
	void _GenerateLuaGeneralRef();

private:
    lua_State* m_pkLuaState; ///< LUA state: 실제 동작을 담당.
    int m_iTableDepth; ///< LUA stack에 올라간 table의 갯수.

//{{ robobeg : 2008-10-28
	//bool m_bLuaTinkerHelper;	// Lua Tinker를 사용하기 위한거라면...m_pkLuaState를 만들지도 지우지도 않고, 사용만 함
    lua_State*  m_pkLuaStateOriginal;
    int         m_iLuaStateRef;
    // m_iLuaStateRef == LUA_REFNIL 이면 기존의 m_bLuaTinkerHelper == true 와 동일
//}} robobeg : 2008-10-28
//{{ robobeg : 2012-11-21
	int			m_iLuaTableRef;
//}} robobeg : 2012-11-21
//{{ robobeg : 2013-01-17
	int			m_iLuaGeneralRef;
//}] robobeg : 2013-01-17
	
//private:
//    int m_iLogLevel;



//public: 
//	static DWORD m_ThreadIDofObjectLoaderInSimulationServer;

};

#endif // _KOG_KLUAMANAGER_H_
