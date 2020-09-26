/** @file
@author cruise@kogsoft.com
*/

#ifndef _KLUAMANAGER_H_
#define _KLUAMANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <KncLua.h>
//#include "LUA/lua.h"
//#include "LUA/lauxlib.h"
//#include "LUA/lualib.h"
#include <windows.h>
#include <assert.h>

/** LUA manager class.
LUA언어로 작성된 configuration file을 읽어 오고, 그 내용을 C++
프로그램으로 가져오기 위한 class. LUA의 기본형은 number (float 형태로
저장), string 2가지 뿐이고, 이들을 table (structure 개념과 유사)로
저장할 수 있다. table 내의 table도 지원된다. 자세한 내용은 LUA manual을
참고하기 바란다.

@verbinclude testKLuaManager.cpp
*/

#define LUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == E_FAIL ) \
	(dest) = (initValue); \
}

#define LUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
}

//{{ 2008. 3. 18  최육사  에러 스트링
#define LUA_GET_VALUE_RETURN_ERR( luaManager, key, dest, initValue, errStr, returnCode ) \
{ \
	if( (luaManager).GetValue( (key), (dest) ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	(errStr) = (key); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
}
//}}

#define LUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == E_FAIL ) \
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
	if( (luaManager).GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
	else \
	{ \
	dest = (destType)(intBuffer); \
	} \
}

//{{ 2008. 3. 18  최육사  에러 스트링
#define LUA_GET_VALUE_RETURN_ENUM_ERR( luaManager, key, dest, destType, initValue, errStr, returnCode ) \
{ \
	int intBuffer = 0; \
	if( (luaManager).GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	(errStr) = (key); \
	START_LOG( cerr, L"Lua Get Value Error [" << key << "]" ) \
	<< END_LOG; \
	returnCode; \
	} \
	else \
	{ \
	dest = (destType)(intBuffer); \
	} \
}
//}}


//pointer
#define PLUA_GET_VALUE( luaManager, key, dest, initValue ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == E_FAIL ) \
	(dest) = (initValue); \
}

#define PLUA_GET_VALUE_RETURN( luaManager, key, dest, initValue, returnCode ) \
{ \
	if( (luaManager)->GetValue( (key), (dest) ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	returnCode; \
	} \
}

#define PLUA_GET_VALUE_ENUM( luaManager, key, dest, destType, initValue ) \
{ \
	int intBuffer = 0; \
	if( (luaManager)->GetValue( (key), intBuffer ) == E_FAIL ) \
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
	if( (luaManager)->GetValue( (key), intBuffer ) == E_FAIL ) \
	{ \
	(dest) = (initValue); \
	returnCode; \
	} \
	else \
	{ \
	dest = (destType)(intBuffer); \
	} \
}

//typedef long HRESULT;

class KLuaManager
{
public:
    /// Default constructor.
    KLuaManager(void);
    /// Constructor with initial internal stack size.
    KLuaManager(int iStackSize);
    /// Constructor for LuaTinker Helper
    KLuaManager(lua_State* pLuaState);
    /// Destructor.
    ~KLuaManager(void);
private:
    /// Copy constructor: DISABLED.
    KLuaManager(const KLuaManager&);
    /// Assignment operator: DISABLED.
    void operator=(const KLuaManager&);
public:
    /// String representation.
    std::string ToString(void) const;
    /// Get the low-level LUA state.
    inline lua_State* GetLuaState(void) { return m_pkLuaState; }

    // CHECK TYPE
    bool IsNumber( const char* pszName );
    bool IsNumber( int iIndex );
    bool IsString( const char* pszName );
    bool IsString( int iIndex );

    // COMMAND EXECUTION ***********************************************

    /// string으로 주어진 LUA 명령을 실행한다.
    HRESULT DoString(const char* pszStr, bool bLogOutput = true);
    /// file에 주어진 LUA 명령을 실행한다.
    HRESULT DoFile(const char* pszFileName, bool bLogOutput = true);

    // TABLE HANDLING **************************************************

    /// pszName의 이름을 가지는 table을 LUA stack으로 가져온다.
    HRESULT BeginTable(const char* pszName, bool bLogOutput = true);
	HRESULT BeginTable(const WCHAR* pszName, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return BeginTable( charBuf, bLogOutput );
	}
    /// (table 내에서) iIndex번째 table을 LUA stack으로 가져온다.
    HRESULT BeginTable(int iIndex, bool bLogOutput = true);
    /// LUA stack에서 가장 최근에 사용한 table을 제거한다.
    HRESULT EndTable(bool bLogOutput = true);

    // GET BOOL-TYPE VALUE *********************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, bool& bValue, bool bLogOutput = true);
	HRESULT GetValue(const WCHAR* pszName, bool& bValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, bValue, bLogOutput );
	}
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, bool& bValue, bool bLogOutput = true);

    // GET INTEGER-TYPE VALUE ******************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* pszName, short& sValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, sValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, short& sValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, short& sValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, int& iValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* pszName, int& iValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, iValue, bLogOutput );
	}
    HRESULT GetValue(int iIndex, int& iValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* szName, long& lValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* szName, long& lValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, lValue, bLogOutput );
	}
    HRESULT GetValue(int iIndex, long& lValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* pszName, unsigned short& usValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, usValue, bLogOutput );
	}
    HRESULT GetValue(int iIndex, unsigned short& usValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* pszName, unsigned int& uiValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, uiValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* szName, unsigned long& ulValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, szName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, ulValue, bLogOutput );
	}
    HRESULT GetValue(const char* szName, unsigned long& ulValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput = true);

    HRESULT GetValue(const char* pszName, char& cValue, bool bLogOutput = true);
    HRESULT GetValue(int iIndex, char& cValue, bool bLogOutput = true);

    // GET FLOAT-TYPE VALUE ********************************************

    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* pszName, float& fValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, float& fValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, float& fValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* pszName, double& fValue, bool bLogOutput = true)
	{
		char charBuf[255] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 255, NULL, NULL );
		return GetValue( charBuf, fValue, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, double& fValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, double& fValue, bool bLogOutput = true);

    // GET STRING-TYPE VALUE *******************************************

	HRESULT GetValue(const WCHAR* pszName, std::wstring& wstrValue, bool bLogOutput = true)
	{
		char charBuf[511] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 511, NULL, NULL );
		std::string strBuf;
		HRESULT retval = GetValue( charBuf, strBuf, bLogOutput );
		if( retval == E_FAIL )
			return retval;

		WCHAR wcharBuf[511] = {0,};
		::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wcharBuf, 511 );
		wstrValue = wcharBuf;

		return retval;
	}
    /// pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
    HRESULT GetValue(const char* pszName, std::string& strValue, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
	HRESULT GetValue(int iIndex, std::wstring& wstrValue, bool bLogOutput = true)
	{
		std::string strBuf;
		HRESULT retval = GetValue( iIndex, strBuf, bLogOutput );
		if( retval == E_FAIL )
			return retval;

		WCHAR wcharBuf[511] = {0,};
		::MultiByteToWideChar( CP_UTF8, 0, strBuf.c_str(), -1, wcharBuf, 511 );
		wstrValue = wcharBuf;

		return retval;
	}
    HRESULT GetValue(int iIndex, std::string& strValue, bool bLogOutput = true);
    /// pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
	HRESULT GetValue(const WCHAR* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true)
	{
		char charBuf[511] = {0,};
		::WideCharToMultiByte( CP_ACP, 0, pszName, -1, charBuf, 511, NULL, NULL );
		return GetValue( charBuf, pszValue, iMaxLen, bLogOutput );
	}
    HRESULT GetValue(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput = true);
    /// (table 내에서) iIndex번째 변수의 값을 가져온다.
    HRESULT GetValue(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput = true);

    //private:
    /// Internal function: pszName의 이름을 가지는 변수/table을 LUA stack으로 가져온다.
    HRESULT Get(const char* pszName);
    /// Internal function: (table 내에서) iIndex번째 변수/table을 LUA stack으로 가져온다.
    HRESULT Get(int iIndex);

private:
    lua_State* m_pkLuaState; ///< LUA state: 실제 동작을 담당.
    int m_iTableDepth; ///< LUA stack에 올라간 table의 갯수.
    bool m_bLuaTinkerHelper;	// Lua Tinker를 사용하기 위한거라면...m_pkLuaState를 만들지도 지우지도 않고, 사용만 함

private:    
    int m_iLogLevel;
};

#endif // _KLUAMANAGER_H_
