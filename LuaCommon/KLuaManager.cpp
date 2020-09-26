/** @file
@author cruise@kogsoft.com
*/

#include "KLuaManager.h"

#include <iostream>
#include <sstream>

//#include "KSuperAssert.h"
//#include "KTDXErrorDefine.h"



//DWORD KLuaManager::m_ThreadIDofObjectLoaderInSimulationServer = 0;



const static char* cvsid = "$Id: KLuaManager.cpp,v 1.5 2003/02/17 03:50:55 cvs Exp $";
//static const char s_szKLuaManagerTemplate[] = "_KLuaManager_";


inline void KLuaManager::_GenerateLuaTableRef()
{
	if ( m_iLuaTableRef == LUA_REFNIL )
	{
		KLUABEGIN( m_pkLuaStateOriginal )
			lua_newtable( L );
			m_iLuaTableRef = lua_ref( L, true );
			ASSERT( m_iLuaTableRef !=  LUA_REFNIL );
		KLUAEND()
	}
}

inline void KLuaManager::_GenerateLuaGeneralRef()
{
	if ( m_iLuaGeneralRef == LUA_REFNIL )
	{
		KLUABEGIN( m_pkLuaStateOriginal )
			lua_newtable( L );
			m_iLuaGeneralRef = lua_ref( L, true );
			ASSERT( m_iLuaGeneralRef !=  LUA_REFNIL );
		KLUAEND()
	}
}


//{{ robobeg : 2012-11-17

KLuaManager::KLuaManager()
	: m_iTableDepth( 0 )
	, m_pkLuaState( NULL )
	, m_pkLuaStateOriginal( NULL )
	, m_iLuaStateRef( LUA_REFNIL )
	, m_iLuaTableRef( LUA_REFNIL )
	, m_iLuaGeneralRef( LUA_REFNIL )
{
}

//}} robobeg : 2012-11-17

//{{ robobeg : 2008-10-28

KLuaManager::KLuaManager( lua_State* pLuaState, int iTableDepth, bool bInNewEnv_ )
	: m_iTableDepth( 0 )
	, m_pkLuaState( NULL )
	, m_pkLuaStateOriginal( NULL )
	, m_iLuaStateRef( LUA_REFNIL )
	, m_iLuaTableRef( LUA_REFNIL )
	, m_iLuaGeneralRef( LUA_REFNIL )
{
	//CHECK_LUA_THREAD_ID;

	AssignNewLuaState( pLuaState, iTableDepth, bInNewEnv_ );
}//KLuaManager::KLuaManager()


/** Destructor. */
KLuaManager::~KLuaManager(void)
{
	//CHECK_LUA_THREAD_ID;

	DetachLuaState();
}

void    KLuaManager::Reset()
{
	//CHECK_LUA_THREAD_ID;

    KLUABEGIN( m_pkLuaStateOriginal )

        if ( m_iLuaStateRef != LUA_REFNIL )
        {
			lua_unref( L, m_iLuaStateRef );
			m_iLuaStateRef = LUA_REFNIL;
        }//if
        if ( m_iLuaTableRef != LUA_REFNIL )
        {
			lua_unref( L, m_iLuaTableRef );
			m_iLuaTableRef = LUA_REFNIL;
        }//if
		if ( m_iLuaGeneralRef != LUA_REFNIL )
		{
			lua_unref( L, m_iLuaGeneralRef );
			m_iLuaGeneralRef = LUA_REFNIL;
		}

        m_pkLuaState = lua_newthread( L );
        lua_newtable( L );
		lua_newtable( L );
		lua_pushstring( L, "__index" );
		lua_getfenv( L, -4 );
		lua_rawset( L, -3 );
		lua_setmetatable( L, -2 );
		lua_setfenv( L, -2 );
        m_iLuaStateRef = lua_ref( L, true );
        ASSERT( m_iLuaStateRef != LUA_REFNIL );
   
    KLUAEND()

}//KLuaManager::Reset()

//{{ robobeg : 2012-11-17
	// 설정된 lua state 를 제거하고 초기 상태로 만든다. 
void	KLuaManager::DetachLuaState()
{

		if ( m_iLuaStateRef != LUA_REFNIL )
		{
			KLUABEGIN( m_pkLuaStateOriginal )
			lua_unref( L, m_iLuaStateRef );
			m_iLuaStateRef = LUA_REFNIL;
				KLUAEND()
		}//if
		if ( m_iLuaTableRef != LUA_REFNIL )
		{
			KLUABEGIN( m_pkLuaStateOriginal )
			lua_unref( L, m_iLuaTableRef );
			m_iLuaTableRef = LUA_REFNIL;
				KLUAEND()
		}
		if ( m_iLuaGeneralRef != LUA_REFNIL )
		{
			KLUABEGIN( m_pkLuaStateOriginal )
			lua_unref( L, m_iLuaGeneralRef );
			m_iLuaGeneralRef = LUA_REFNIL;
			KLUAEND()
		}


    m_iTableDepth = 0;
    m_pkLuaState = NULL;
    m_pkLuaStateOriginal = NULL;
}

void	KLuaManager::AssignNewLuaState( lua_State* pLuaState, int iTableDepth, bool bInNewEnv_ )
{
	DetachLuaState();

	//CHECK_LUA_THREAD_ID;

    m_iTableDepth = iTableDepth;
    m_pkLuaStateOriginal = pLuaState;
    m_iLuaStateRef = LUA_REFNIL;
	m_iLuaTableRef = LUA_REFNIL;
	m_iLuaGeneralRef = LUA_REFNIL;

    if ( !bInNewEnv_ )
    {
        m_pkLuaState = pLuaState;
    }
    else
    {
        KLUABEGIN( m_pkLuaStateOriginal )

			m_pkLuaState = lua_newthread( L );
			lua_newtable( L );
			lua_newtable( L );
			lua_pushstring( L, "__index" );
			lua_getfenv( L, -4 );
			lua_rawset( L, -3 );
			lua_setmetatable( L, -2 );
			lua_setfenv( L, -2 );
			m_iLuaStateRef = lua_ref( L, true );
			ASSERT( m_iLuaStateRef != LUA_REFNIL );

		KLUAEND()

    }//if.. else..
}
//}} robobeg : 2012-11-17



//HRESULT KLuaManager::SetAsTemplateEnvironment()
//{
//    if ( m_iLuaStateRef == LUA_REFNIL )
//        return E_FAIL;
//
//    KLUABEGIN( m_pkLuaStateOriginal )
//
//        lua_getref( L, m_iLuaStateRef );
//        lua_pushstring( L, s_szKLuaManagerTemplate );
//        lua_newtable( L );
//        lua_pushstring( L, "__index" );
//        lua_getfenv( L, -4 );
//        lua_rawset( L, -3 );
//        lua_rawset( L, LUA_REGISTRYINDEX );
//        lua_pop( L, 1 );
//
//    KLUAEND()
//
//    return S_OK;
//}//KLuaManager::SetAsTemplateEnvironment()


//HRESULT KLuaManager::ExportEnvironment()
//{
//    if ( m_iLuaStateRef == LUA_REFNIL )
//        return E_FAIL;
//
//    KLUABEGIN( m_pkLuaStateOriginal )
//
//        lua_getref( L, m_iLuaStateRef );
//        lua_getfenv( L, -1 );
//        lua_pushnil(L);  /* first key */
//        while (lua_next(L, -2) != 0) 
//        {
//            lua_pushvalue( L, -2 );
//            lua_insert( L, -2 );
//            lua_rawset( L, LUA_GLOBALSINDEX );
//        }//while
//        lua_pop( L, 2 );
//
//    KLUAEND()
//
//    return S_OK;
//}//KLuaManager::ExportEnvironment()


//}} robobeg : 2008-10-28



bool	KLuaManager::ExportFunctionsToGlobalEnv()
{
	//CHECK_LUA_THREAD_ID;

	if ( m_iLuaStateRef == LUA_REFNIL )
		return false;

	bool	bRet = false;

	KLUABEGIN( m_pkLuaStateOriginal )

		lua_pushvalue( L, LUA_GLOBALSINDEX );
		lua_getref( L, m_iLuaStateRef );
		lua_getfenv( L, -1 );
		lua_remove( L, -2 );

		bRet = true;
		lua_pushnil( L );
		while( lua_next( L, -2 ) != 0 )
		{
			switch( lua_type( L, -1 ) )
			{
			case LUA_TNIL:
				break;
			case LUA_TNUMBER:
			case LUA_TBOOLEAN:
			case LUA_TSTRING:
			case LUA_TTABLE:
			case LUA_TLIGHTUSERDATA:
				lua_pushvalue( L, -2 );
				lua_pushvalue( L, -2 );
				lua_rawset( L, -6 );
				break;
			
			case LUA_TFUNCTION:
			case LUA_TUSERDATA:
			case LUA_TTHREAD:
				lua_pushvalue( L, -4 );
				lua_setfenv( L, -2 );
				lua_pushvalue( L, -2 );
				lua_pushvalue( L, -2 );
				lua_rawset( L, -6 );
			}//if
			lua_pop( L, 1 );
		}//while

		lua_pop( L, 2 );

	KLUAEND()

	return bRet;
}//KLuaManager::ExportFunctionsToGlobalEnv()






/** String representation. */
std::string KLuaManager::ToString(void) const
{
	//CHECK_LUA_THREAD_ID;

    std::ostringstream s;
    s << "KLuaManager (luaState=" << m_pkLuaState;
    s << ",tableDepth=" << m_iTableDepth << ")";
    return s.str();
}

/** string으로 주어진 LUA 명령을 실행한다.
@param pszStr [in] LUA 명령을 담고 있는 string
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure
*/
#ifndef X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
bool KLuaManager::DoString(const char* pszStr, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

	return lua_tinker::dostring( m_pkLuaState, pszStr );
}
#endif  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE

/** file에 주어진 LUA 명령을 실행한다.
@param pszFileName [in] LUA 명령을 담고 있는 file의 이름
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure.
*/
bool KLuaManager::DoFile(const char* pszFileName, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

	// note!! .kom파일에 포함되지 않은 파일을 직접 참조할 때만 사용해야.
	return lua_tinker::dofile( m_pkLuaState, pszFileName );
}

/** Internal function: pszName의 이름을 가지는 변수/table을 LUA stack으로 가져온다.
@param pszName [in] 변수/table의 이름
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure.
*/
bool KLuaManager::Get(const char* pszName)
{
	//CHECK_LUA_THREAD_ID;

    if (m_iTableDepth > 0)
    {
        lua_pushstring(m_pkLuaState, pszName);
        lua_gettable(m_pkLuaState, -2);
    }
    else
    {
        lua_getglobal(m_pkLuaState, pszName);
    }
    return true;
}

/** Internal function: (table 내에서) iIndex번째 변수/table을 LUA stack으로 가져온다.
@param iIndex [in] 변수/table의 index (1부터 시작함)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (현재 table 안에 들어와 있지 않은
경우).
*/
bool KLuaManager::Get(int iIndex)
{
	//CHECK_LUA_THREAD_ID;

    if (m_iTableDepth <= 0) return false;
    lua_pushnumber(m_pkLuaState, iIndex);
    lua_gettable(m_pkLuaState, -2);
    return true;
}

/** pszName의 이름을 가지는 table을 LUA stack으로 가져온다.
@param pszName [in] table의 이름
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 table이 없는 경우).
*/
bool KLuaManager::BeginTable(const char* pszName, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( Get(pszName) == true) 
    {
        if ( lua_istable(m_pkLuaState, -1) != 0 )
        {
            m_iTableDepth++;		
    		//KDbgStm << "BeginTable - " << pszName << " : " << m_iTableDepth << trace;
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }//if.. else..
    }
    return false;
}

/** (table 내에서) iIndex번째 table을 LUA stack으로 가져온다.
@param iIndex [in] table의 index (1부터 시작함)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (현재 table 안에 들어와 있지 않은
경우, 해당 table이 없는 경우).
*/
bool KLuaManager::BeginTable(int iIndex, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( Get(iIndex) == true )
    {
        if ( lua_istable(m_pkLuaState, -1) != 0)
        {
            m_iTableDepth++;
    		//KDbgStm << "BeginTable - " << iIndex << " : " << m_iTableDepth << trace;
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }
    }
    return false;
}

/** LUA stack에서 가장 최근에 사용한 table을 제거한다.
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (table의 사용이 없던 경우).
*/
bool KLuaManager::EndTable(bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if (m_iTableDepth > 0)
    {
        lua_pop(m_pkLuaState, 1);
        m_iTableDepth--;

		//KDbgStm << "EndTable : " << m_iTableDepth << trace;
        return true;
    }
    else
    {
        return false;
    }
}

//{{ robobeg : 2008-10-28
bool KLuaManager::MakeTableReference( const char* pszName, int iInRef_, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( iInRef_ < 0 )
        return false;

    if ( Get(pszName) == true )
    {
        if ( lua_istable( m_pkLuaState, -1) != 0 )
        {
			_GenerateLuaTableRef();

			lua_getref( m_pkLuaState, m_iLuaTableRef );
			ASSERT( lua_istable( m_pkLuaState, -1 ) != 0 );
			if ( lua_istable( m_pkLuaState, -1 ) != 0 )
			{
				lua_insert( m_pkLuaState, -2 );
				ASSERT( lua_istable( m_pkLuaState, -2 ) != 0 );
				lua_rawseti( m_pkLuaState, -2, iInRef_ + 1 );
			}
			else
			{
				int	iLuaType = lua_type( m_pkLuaState, -1 );
				lua_pop( m_pkLuaState, 2 );
				return false;
			}
			lua_pop( m_pkLuaState, 1 );
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }//if.. else..
    }
    return false;
}//KLuaManager::ObtainTableReference()


bool KLuaManager::BeginTableByReference( int iInRef_, bool bLogOutput )
{
	//CHECK_LUA_THREAD_ID;

    if ( iInRef_ < 0 )
        return false;

	if ( m_iLuaTableRef == LUA_REFNIL )
		return false;

	lua_getref( m_pkLuaState, m_iLuaTableRef );
    lua_rawgeti( m_pkLuaState, -1, iInRef_ + 1 );
	lua_remove( m_pkLuaState, -2 );
    if ( lua_istable(m_pkLuaState, -1) != 0 )
    {
        m_iTableDepth++;

		//KDbgStm << "BeginTable - " << iIndex << " : " << m_iTableDepth << trace;
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
}//KLuaManager::BeginTableByReference()
//}} robobeg : 2008-10-28


////{{ robobeg : 2013-01-17
int	KLuaManager::MakeGeneralReference( const char* pszName, int type, bool bRawGlobal )
{
	int	iRef = LUA_REFNIL;
    if ( bRawGlobal == true )
    {
        if ( m_iLuaStateRef == LUA_REFNIL )
        {
            lua_pushstring( m_pkLuaState, pszName );
            lua_rawget( m_pkLuaState, LUA_GLOBALSINDEX );
        }
        else
        {
            lua_getref( m_pkLuaState, m_iLuaStateRef );
            lua_getfenv( m_pkLuaState, -1 );
            lua_getmetatable( m_pkLuaState, -1 );
            lua_pushstring( m_pkLuaState, "__index" );
            lua_rawget( m_pkLuaState, -2 );
            lua_pushstring( m_pkLuaState, pszName );
            lua_gettable( m_pkLuaState, -2 );
            lua_replace( m_pkLuaState, -5 );
            lua_pop( m_pkLuaState, 3 );
        }
    }
    else
    {
        Get( pszName );
    }
	{
        if ( lua_type( m_pkLuaState, -1) == type )
        {
			_GenerateLuaGeneralRef();

			lua_getref( m_pkLuaState, m_iLuaGeneralRef );
			ASSERT( lua_istable( m_pkLuaState, -1 ) != 0 );

			if ( lua_istable( m_pkLuaState, -1 ) != 0 )
			{
				lua_insert( m_pkLuaState, -2 );
				ASSERT( lua_istable( m_pkLuaState, -2 ) != 0 );
				iRef = luaL_ref( m_pkLuaState, -2 );
			}
			else
			{
				int	iLuaType = lua_type( m_pkLuaState, -1 );
				lua_pop( m_pkLuaState, 2 );
				return LUA_REFNIL;
			}
			lua_pop( m_pkLuaState, 1 );
            return iRef;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }//if.. else..
    }
	return	LUA_REFNIL;
}

//int KLuaManager::MakeFunctionReference( const std::wstring& funcName, bool bRawGlobal )
//{
//	//CHECK_LUA_THREAD_ID;
//
//	char charBuf[511] = {0,};
//	WideCharToMultiByte( CP_UTF8, 0, funcName.c_str(), -1, charBuf, 511, NULL, NULL );
//
//	return MakeFunctionReference( charBuf, bRawGlobal );
//}
//
//int KLuaManager::MakeFunctionReference( const wchar_t* funcName, bool bRawGlobal )
//{
//	//CHECK_LUA_THREAD_ID;
//
//	char charBuf[511] = {0,};
//	WideCharToMultiByte( CP_UTF8, 0, funcName, -1, charBuf, 511, NULL, NULL );
//
//	return MakeFunctionReference( charBuf, bRawGlobal );
//}



int     KLuaManager::MakeFunctionReference( const char* funcName, bool bRawGlobal )
{
    return  MakeGeneralReference( funcName, LUA_TFUNCTION, bRawGlobal );
}

////}} robobeg : 2013-01-17

//{{ robobeg : 2013-10-24
int     KLuaManager::GetNumIndexedValues()
{
    int index = 1;
    while( Get( index ) == true )
    {
        bool    bNil = lua_type(m_pkLuaState, -1 ) == LUA_TNIL;
        lua_pop(m_pkLuaState, 1);
        if ( bNil == true )
            break;
        index++;
    }
    return  index - 1;
}

int     KLuaManager::GetNumIndexedTables()
{
    int index = 1;
    while( BeginTable( index ) == true )
    {
        EndTable();
        index++;
    }
    return  index - 1;
}

int     KLuaManager::GetNumIndexedTables( const char* pszTableName )
{
    if ( pszTableName == NULL || pszTableName[0] == NULL )
        return 0;

    int index = 0;
    while( BeginTable( pszTableName, index ) == true )
    {
        EndTable();
        index++;
    }
    return  index;
}
//}} robobeg : 2013-10-24



/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param bValue [out] 숫자 변수가 0이면 false, 0이 아니면 true (실패 시는 변화
없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, bool& bValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( Get(pszName) == true )
    {
        if ( lua_isnumber(m_pkLuaState, -1) != 0)
        {
            bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
            lua_pop(m_pkLuaState, 1);
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }//if.. else..
    }
    return false;
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param bValue [out] 숫자 변수가 0이면 false, 0이 아니면 true (실패 시는 변화
없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, bool& bValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( Get(iIndex) == true )
    {
        if ( lua_isnumber(m_pkLuaState, -1) != 0)
        {
            bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
            lua_pop(m_pkLuaState, 1);
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }//if.. else..
    }
    return false;
}

#define GET_BY_NAME(var,type) \
    if ( Get(pszName) == true ) \
    { \
        if ( lua_isnumber(m_pkLuaState, -1) != 0 ) \
        { \
            var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
            lua_pop(m_pkLuaState, 1); \
            return true; \
        } \
        else \
        { \
            lua_pop(m_pkLuaState, 1); \
        } \
    } \
    return false;

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param sValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, short& sValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(sValue, short);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param iValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, int& iValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(iValue, int);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param lValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, long& lValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(lValue, long);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param usValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, unsigned short& usValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(usValue, unsigned short);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param uiValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, unsigned int& uiValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(uiValue, unsigned int);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param ulValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, unsigned long& ulValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(ulValue, unsigned long);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param fValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, float& fValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(fValue, float);
}

/** pszName의 이름을 가지는 숫자 변수의 값을 가져온다.
@param pszName [in] 숫자 변수의 이름
@param dValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(const char* pszName, double& dValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NAME(dValue, double);
}

#define GET_BY_NUM(var,type) \
    if ( Get(iIndex) == true ) \
    { \
        if ( lua_isnumber(m_pkLuaState, -1) != 0) \
        { \
            var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
            lua_pop(m_pkLuaState, 1); \
            return true; \
        } \
        else \
        { \
            lua_pop(m_pkLuaState, 1); \
        } \
    } \
    return false;

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param sValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, short& sValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(sValue, short);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param iValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, int& iValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(iValue, int);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param lValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, long& lValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(lValue, long);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param usValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, unsigned short& usValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(usValue, unsigned short);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param uiValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, unsigned int& uiValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(uiValue, unsigned int);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param ulValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, unsigned long& ulValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(ulValue, unsigned long);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param fValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, float& fValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(fValue, float);
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param dValue [out] 숫자 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 숫자 변수가
아닌 경우).
*/
bool KLuaManager::GetValue(int iIndex, double& dValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    GET_BY_NUM(dValue, double);
}

bool KLuaManager::GetValue(const char* pszName, std::wstring& wstrValue, bool bLogOutput )
{
	//CHECK_LUA_THREAD_ID;

	//KTDXPROFILE();
    wstrValue.resize( 0 );

    bool    bReturn = false;
    if ( Get(pszName) == true )
    {
        if ( lua_isstring(m_pkLuaState, -1) != 0 )
        {
            size_t size = 0;
            const char* pszString = lua_tolstring(m_pkLuaState, -1, &size);
	        int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pszString, size, NULL, 0 );
	        if ( iDestSize > 0 )
            {
	            wchar_t* wcharBuf = (wchar_t*) _malloca( iDestSize * sizeof( wchar_t ) );
	            ASSERT( wcharBuf != NULL );
	            int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pszString, size, wcharBuf, iDestSize );
	            ASSERT( iNewSize == iDestSize );
	            wstrValue.assign( wcharBuf, iNewSize );
                _freea( wcharBuf );
            }
            bReturn = true;
        }
        lua_pop(m_pkLuaState, 1);
    }
    return bReturn;
}

bool KLuaManager::GetValue(int iIndex, std::wstring& wstrValue, bool bLogOutput )
{
	//CHECK_LUA_THREAD_ID;

	//KTDXPROFILE();
    wstrValue.resize( 0 );

    bool    bReturn = false;
    if ( Get(iIndex) == true )
    {
        if ( lua_isstring(m_pkLuaState, -1) != 0 )
        {
            size_t size = 0;
            const char* pszString = lua_tolstring(m_pkLuaState, -1, &size);
	        int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pszString, size, NULL, 0 );
	        if ( iDestSize > 0 )
            {
	            wchar_t* wcharBuf = (wchar_t*) _malloca( iDestSize * sizeof( wchar_t ) );
	            ASSERT( wcharBuf != NULL );
	            int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pszString, size, wcharBuf, iDestSize );
	            ASSERT( iNewSize == iDestSize );
	            wstrValue.assign( wcharBuf, iNewSize );
                _freea( wcharBuf );
            }
            bReturn = true;
        }
        lua_pop(m_pkLuaState, 1);
    }
    return bReturn;
}

bool KLuaManager::GetValue( const char* pszName, wchar_t* wszValue, int bufferSize, bool bLogOutput )
{
	//CHECK_LUA_THREAD_ID;

	//KTDXPROFILE();
    if ( wszValue == NULL || bufferSize <= 0 )
        return false;

    bool    bReturn = false;
    if ( Get(pszName) == true )
    {
        if ( lua_isstring(m_pkLuaState, -1) != 0 )
        {
            size_t size = 0;
            const char* pszString = lua_tolstring(m_pkLuaState, -1, &size);
            if ( size == 0 || pszString == NULL || pszString[0] == 0 )
            {
                wszValue[0] = 0;
                bReturn = true;
            }
            else
            {
	            int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pszString, size + 1, wszValue, bufferSize );
                if ( iDestSize > 0 )
                    bReturn = true;
            }
        }
        lua_pop(m_pkLuaState, 1);
    }
    return bReturn;
}


bool KLuaManager::GetValue( int iIndex, wchar_t* wszValue, int bufferSize, bool bLogOutput )
{

	//KTDXPROFILE();
    if ( wszValue == NULL || bufferSize <= 0 )
        return false;

    bool    bReturn = false;
    if ( Get(iIndex) == true )
    {
        if ( lua_isstring(m_pkLuaState, -1) != 0 )
        {
            size_t size = 0;
            const char* pszString = lua_tolstring(m_pkLuaState, -1, &size);
            if ( size == 0 || pszString == NULL || pszString[0] == 0 )
            {
                wszValue[0] = 0;
                bReturn = true;
            }
            else
            {
	            int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pszString, size + 1, wszValue, bufferSize );
                if ( iDestSize > 0 )
                    bReturn = true;
            }
        }
        lua_pop(m_pkLuaState, 1);
    }
    return bReturn;
}

/** pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
@param pszName [in] 문자열 변수의 이름
@param strValue [out] 문자열 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
bool KLuaManager::GetValueUtf8(const char* pszName, std::string& strValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( Get(pszName) == true )
    {
        if ( lua_isstring(m_pkLuaState, -1) != 0 )
        {
            strValue = lua_tostring(m_pkLuaState, -1);
            lua_pop(m_pkLuaState, 1);
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }
    }
    return false;
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param strValue [out] 문자열 변수의 값 (실패시는 변화 없음)
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
bool KLuaManager::GetValueUtf8(int iIndex, std::string& strValue, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( Get(iIndex) == true )
    {
        if ( lua_isstring(m_pkLuaState, -1) != 0 )
        {
            strValue = lua_tostring(m_pkLuaState, -1);
            lua_pop(m_pkLuaState, 1);
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }
    }
    return false;
}

/** pszName의 이름을 가지는 문자열 변수의 값을 가져온다.
@param pszName [in] 문자열 변수의 이름
@param pszValue [out] 문자열 변수의 값이 저장될 주소 (실패시는 변화 없음)
@param iMaxLen [in] pszValue에 최대 저장 가능한 문자의 수
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
bool KLuaManager::GetValueUtf8(const char* pszName, char* pszValue, int iMaxLen, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;

    if ( Get(pszName) == true )
    {
        if ( lua_isstring(m_pkLuaState, -1) != 0 )
        {
            strncpy_s( pszValue, iMaxLen, lua_tostring(m_pkLuaState, -1), _TRUNCATE );
            lua_pop(m_pkLuaState, 1);
            return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }
    }
    return false;
}

/** (table 내에서) iIndex번째 변수의 값을 가져온다.
@param iIndex [in] 변수의 index (1부터 시작함)
@param pszValue [out] 문자열 변수의 값이 저장될 주소 (실패시는 변화 없음)
@param iMaxLen [in] pszValue에 최대 저장 가능한 문자의 수
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (해당 변수가 없거나, 문자열 변수가
아닌 경우).
*/
bool KLuaManager::GetValueUtf8(int iIndex, char* pszValue, int iMaxLen, bool bLogOutput)
{
	//CHECK_LUA_THREAD_ID;


	if ( Get(iIndex) == true )
	{
        if ( lua_isstring(m_pkLuaState, -1) != 0)
        {
		    strncpy_s( pszValue, iMaxLen, lua_tostring(m_pkLuaState, -1), _TRUNCATE );
		    lua_pop(m_pkLuaState, 1);
		    return true;
        }
        else
        {
            lua_pop(m_pkLuaState, 1);
        }
	}
    return false;
}


#ifdef  _USE_LUAJIT_

void    KLuaManager::_SetJitCompileModeRecursively( lua_State* L, EJitCompileMode eMode )
{
	lua_pushnil( L );  /* first key */
	/* table is in the stack at index 't' */
	while ( lua_next( L, -2 ) != 0 ) 
	{
		// value를 읽어온다
		int luaValueType = lua_type( L, -1 );
		switch( luaValueType )
		{
		case LUA_TTABLE:
			{
				_SetJitCompileModeRecursively( L, eMode );
			} 
            break;

        case LUA_TFUNCTION:
            {
                int iMode = 0;
                switch( eMode )
                {
                case JIT_COMPILE_MODE_ON:
                    iMode = LUAJIT_MODE_ON;
                    break;
                case JIT_COMPILE_MODE_FLUSH:
                    iMode = LUAJIT_MODE_FLUSH;
                    break;
                case JIT_COMPILE_MODE_OFF:
                default:
                    iMode = LUAJIT_MODE_OFF;
                    break;
                }
                luaJIT_setmode( L, -1, LUAJIT_MODE_ALLFUNC|iMode );
            }
            break;

		} // switch( luaValueType )

		/* removes 'value'; keeps 'key' for next iteration */
		lua_pop(L, 1);

	} // while (lua_next(L, t) != 0)
}


void    KLuaManager::SetJitCompileMode( EJitCompileMode eMode )
{
    if ( m_pkLuaState == NULL )
        return;

    KLUABEGIN( m_pkLuaState )
		lua_pushvalue( L, LUA_GLOBALSINDEX );
        _SetJitCompileModeRecursively( L, eMode );
	    lua_pop( L, 1 );
	KLUAEND()

}

#endif  _USE_LUAJIT_
