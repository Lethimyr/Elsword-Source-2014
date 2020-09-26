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
	HRESULT		DoString(const char* strBuf);
//#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
    HRESULT		DoMemory(const char* pBuffer, const long nSize, bool bLocal = false )
//#else   X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
//	HRESULT		DoMemory(const char* pBuffer, const long nSize)
//#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
	{
        if ( m_pkLuaState == NULL || pBuffer == NULL || nSize == 0 )
            return E_FAIL;;

//#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        if ( bLocal == false )
//#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        {
#ifdef _ENCRIPT_SCRIPT_
		pBuffer = XORDecrypt( pBuffer, nSize );
#endif
        }


		HRESULT retVal = DoMemoryNotEncript( pBuffer, nSize );

//#ifdef  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        if ( bLocal == false )
//#endif  X2OPTIMIZE_INDICATE_LOCAL_MASSFILE
        {
#ifdef _ENCRIPT_SCRIPT_
		SAFE_DELETE_ARRAY( pBuffer );
#endif
        }

		return retVal;
	}

	HRESULT		DoMemoryNotEncript(const char* pBuffer, const long nSize)
	{
        if ( m_pkLuaState == NULL || pBuffer == NULL || nSize == 0 )
            return E_FAIL;

        bool bOK;

        if( lua_getencoding( GetLuaState() ) == LUA_ENCODING_UTF8 )
        {
            assert( nSize > 0 );
            if( nSize <= 0 )
            {
                return false;
            }

            if( pBuffer[0] == ( char )033 )
            {
                bOK = lua_tinker::dobuffer( m_pkLuaState, pBuffer, nSize );
            }
            else
            {
                assert( nSize >= 3 );
                if( nSize < 3 )
                {
                    return E_FAIL;
                }

                assert( ( pBuffer[0] == ( char )0xef ) && ( pBuffer[1] == ( char )0xbb ) && ( pBuffer[2] == ( char )0xbf ) );
                if( ( pBuffer[0] != ( char )0xef ) || ( pBuffer[1] != ( char )0xbb ) || ( pBuffer[2] != ( char )0xbf ) )
                {
                    return E_FAIL;
                }

                bOK = lua_tinker::dobuffer( m_pkLuaState, pBuffer + 3, nSize - 3 );
            }
        }
        else
        {
            bOK = lua_tinker::dobuffer( m_pkLuaState, pBuffer, nSize );
        }
        //{{

		if( true == bOK )
			return S_OK;
		else
			return E_FAIL;
	}

    void		Interpret();
	lua_State*	GetLuaState(){ return m_pkLuaState; }

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

