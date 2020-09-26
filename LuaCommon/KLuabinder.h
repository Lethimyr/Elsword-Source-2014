#pragma once

#include <windows.h>
#include "lua_tinker.h"
#include <assert.h>


class KLuabinder
{

public:
	KLuabinder();
	KLuabinder( lua_State* pLuaState ){ m_pkLuaState = pLuaState; }
	virtual ~KLuabinder();
	
	//void		SetLuaTinker(){ lua_tinker::init(m_pkLuaState); } 

	HRESULT		DoFile(const char* strFilename);
#ifndef X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
	HRESULT		DoString(const char* strBuf);
#endif  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE

	//HRESULT
    bool    DoMemoryNotEncrypt(const char* pBuffer, long nSize)
	{
        if ( m_pkLuaState == NULL || pBuffer == NULL || nSize <= 0 )
            return false;

        bool bOK = false;

        if( lua_getencoding( GetLuaState() ) != LUA_ENCODING_LEGACY )
        {
            if( pBuffer[0] == ( char )033 )
            {
                bOK = lua_tinker::dobuffer( m_pkLuaState, pBuffer, nSize );
            }
            else
            {
                assert( nSize >= 3 );
                if( nSize < 3 )
                {
                    return false;
                }

                assert( ( pBuffer[0] == ( char )0xef ) && ( pBuffer[1] == ( char )0xbb ) && ( pBuffer[2] == ( char )0xbf ) );
                if( ( pBuffer[0] != ( char )0xef ) || ( pBuffer[1] != ( char )0xbb ) || ( pBuffer[2] != ( char )0xbf ) )
                {
                    return false;
                }

                bOK = lua_tinker::dobuffer( m_pkLuaState, pBuffer + 3, nSize - 3 );
            }
        }
        else
        {
            bOK = lua_tinker::dobuffer( m_pkLuaState, pBuffer, nSize );
        }
        //{{

        return  bOK;
	}

#ifndef X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
    void		Interpret();
#endif  X2OPTIMIZE_REMOVE_LUA_INTERPRETER_MODULE
	lua_State*	GetLuaState(){ return m_pkLuaState; }

#ifdef  _USE_LUAJIT_
    void    FlushJitCompileMode();
#endif  _USE_LUAJIT_

protected:

	void Initialize();
	void Finalize();
	
	
private:
    lua_State* m_pkLuaState;
};

#define KDeclareLuaInvoke \
public: \
    void to_lua(lua_State* L) { lua_tinker::push_::invoke(L, this); }

#define KRegLua(x) x

