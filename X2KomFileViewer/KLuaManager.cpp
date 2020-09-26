/** @file
@author cruise@kogsoft.com
*/

#include "stdafx.h"
#include "KLuaManager.h"

#include <iostream>
#include <sstream>

#include "KSuperAssert.h"
#include "KTDXErrorDefine.h"

const static char* cvsid = "$Id: KLuaManager.cpp,v 1.5 2003/02/17 03:50:55 cvs Exp $";
static const char s_szKLuaManagerTemplate[] = "_KLuaManager_";



//{{ robobeg : 2008-10-28

KLuaManager::KLuaManager( lua_State* pLuaState, int iTableDepth, bool bInNewEnv_ )
{
    m_iTableDepth = iTableDepth;
    m_pkLuaStateOriginal = pLuaState;
    m_iLuaStateRef = LUA_REFNIL;

    if ( !bInNewEnv_ )
    {
        m_pkLuaState = pLuaState;
    }
    else
    {
        KLUABEGIN( m_pkLuaStateOriginal )

            m_pkLuaState = lua_newthread( L );
        lua_newtable( L );
        lua_pushstring( L, s_szKLuaManagerTemplate );
        lua_rawget( L, LUA_REGISTRYINDEX );
        if ( lua_istable( L, -1 ) )
            lua_setmetatable( L, -2 );
        else
            lua_pop( L, 1 );
        lua_setfenv( L, -2 );
        m_iLuaStateRef = lua_ref( L, true );
        ASSERT( m_iLuaStateRef != LUA_REFNIL );
        KLUAEND()

    }//if.. else..
}//KLuaManager::KLuaManager()



/** Destructor. */
KLuaManager::~KLuaManager(void)
{
    if ( m_iLuaStateRef != LUA_REFNIL )
    {
        KLUABEGIN( m_pkLuaStateOriginal )

            lua_unref( L, m_iLuaStateRef );

        KLUAEND()
            m_iLuaStateRef = LUA_REFNIL;
    }//if

    m_iTableDepth = 0;
    m_pkLuaState = NULL;
    m_pkLuaStateOriginal = NULL;
}

void    KLuaManager::Reset()
{
    KLUABEGIN( m_pkLuaStateOriginal )

        if ( m_iLuaStateRef != LUA_REFNIL )
        {
            lua_unref( L, m_iLuaStateRef );
            m_iLuaStateRef = LUA_REFNIL;
        }//if

        m_pkLuaState = lua_newthread( L );
        lua_newtable( L );
        lua_pushstring( L, s_szKLuaManagerTemplate );
        lua_rawget( L, LUA_REGISTRYINDEX );
        if ( lua_istable( L, -1 ) )
            lua_setmetatable( L, -2 );
        else
            lua_pop( L, 1 );
        lua_setfenv( L, -2 );
        m_iLuaStateRef = lua_ref( L, true );
        ASSERT( m_iLuaStateRef != LUA_REFNIL );

        KLUAEND()

}//KLuaManager::Reset()


HRESULT KLuaManager::SetAsTemplateEnvironment()
{
    if ( m_iLuaStateRef == LUA_REFNIL )
        return E_FAIL;

    KLUABEGIN( m_pkLuaStateOriginal )

        lua_getref( L, m_iLuaStateRef );
    lua_pushstring( L, s_szKLuaManagerTemplate );
    lua_newtable( L );
    lua_pushstring( L, "__index" );
    lua_getfenv( L, -4 );
    lua_rawset( L, -3 );
    lua_rawset( L, LUA_REGISTRYINDEX );
    lua_pop( L, 1 );

    KLUAEND()

        return S_OK;
}//KLuaManager::SetAsTemplateEnvironment()


HRESULT KLuaManager::ExportEnvironment()
{
    if ( m_iLuaStateRef == LUA_REFNIL )
        return E_FAIL;

    KLUABEGIN( m_pkLuaStateOriginal )

        lua_getref( L, m_iLuaStateRef );
    lua_getfenv( L, -1 );
    lua_pushnil(L);  /* first key */
    while (lua_next(L, -2) != 0) 
    {
        lua_pushvalue( L, -2 );
        lua_insert( L, -2 );
        lua_rawset( L, LUA_GLOBALSINDEX );
    }//while
    lua_pop( L, 2 );

    KLUAEND()

        return S_OK;
}//KLuaManager::ExportEnvironment()


//}} robobeg : 2008-10-28


/** String representation. */
std::string KLuaManager::ToString(void) const
{
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
bool KLuaManager::DoString(const char* pszStr, bool bLogOutput)
{
    return lua_tinker::dostring( m_pkLuaState, pszStr );
}

/** file에 주어진 LUA 명령을 실행한다.
@param pszFileName [in] LUA 명령을 담고 있는 file의 이름
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure.
*/
bool KLuaManager::DoFile(const char* pszFileName, bool bLogOutput)
{
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
    if ((Get(pszName) == true) && lua_istable(m_pkLuaState, -1) != 0)
    {
        m_iTableDepth++;		

        //KDbgStm << "BeginTable - " << pszName << " : " << m_iTableDepth << trace;
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
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
    if ((Get(iIndex) == true) && lua_istable(m_pkLuaState, -1) != 0)
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
}

/** LUA stack에서 가장 최근에 사용한 table을 제거한다.
@param bLogOutput [in] false로 설정하면 log message를 남기지 않음; true가
default임.
@return true for success, false for failure (table의 사용이 없던 경우).
*/
bool KLuaManager::EndTable(bool bLogOutput)
{
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
    if ( iInRef_ < 0 )
        return false;

    if ( (Get(pszName) == true) && lua_istable(m_pkLuaState, -1) != 0)
    {
        lua_rawseti( m_pkLuaState, LUA_GLOBALSINDEX, iInRef_ + 1 );
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }//if.. else..
}//KLuaManager::ObtainTableReference()


bool KLuaManager::BeginTableByReference( int iInRef_, bool bLogOutput )
{
    if ( iInRef_ < 0 )
        return false;

    lua_rawgeti( m_pkLuaState, LUA_GLOBALSINDEX, iInRef_ + 1 );
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
    if ((Get(pszName) == true) && lua_isnumber(m_pkLuaState, -1) != 0)
    {
        bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
        lua_pop(m_pkLuaState, 1);
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
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
    if ((Get(iIndex) == true) && lua_isnumber(m_pkLuaState, -1) != 0)
    {
        bValue = (static_cast<int>(lua_tonumber(m_pkLuaState, -1)) != 0);
        lua_pop(m_pkLuaState, 1);
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
}

#define GET_BY_NAME(var,type) \
    if ((Get(pszName) == true) && lua_isnumber(m_pkLuaState, -1) != 0) \
{ \
    var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
    lua_pop(m_pkLuaState, 1); \
    return true; \
} \
    else \
{ \
    lua_pop(m_pkLuaState, 1); \
    return false; \
}

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
    GET_BY_NAME(dValue, double);
}

#define GET_BY_NUM(var,type) \
    if ((Get(iIndex) == true) && lua_isnumber(m_pkLuaState, -1) != 0) \
{ \
    var = static_cast<type>(lua_tonumber(m_pkLuaState, -1)); \
    lua_pop(m_pkLuaState, 1); \
    return true; \
} \
    else \
{ \
    lua_pop(m_pkLuaState, 1); \
    return false; \
}

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
    GET_BY_NUM(dValue, double);
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
    if ((Get(pszName) == true) && lua_isstring(m_pkLuaState, -1) != 0)
    {
        strValue = lua_tostring(m_pkLuaState, -1);
        lua_pop(m_pkLuaState, 1);
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
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
    if ((Get(iIndex) == true) && lua_isstring(m_pkLuaState, -1) != 0)
    {
        strValue = lua_tostring(m_pkLuaState, -1);
        lua_pop(m_pkLuaState, 1);
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
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
    if ((Get(pszName) == true) && lua_isstring(m_pkLuaState, -1) != 0)
    {
        strncpy(pszValue, lua_tostring(m_pkLuaState, -1), iMaxLen);
        lua_pop(m_pkLuaState, 1);
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
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
    if ((Get(iIndex) == true) && lua_isstring(m_pkLuaState, -1) != 0)
    {
        strncpy(pszValue, lua_tostring(m_pkLuaState, -1), iMaxLen);
        lua_pop(m_pkLuaState, 1);
        return true;
    }
    else
    {
        lua_pop(m_pkLuaState, 1);
        return false;
    }
}
