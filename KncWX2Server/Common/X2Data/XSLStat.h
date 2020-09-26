#pragma once

#include <Windows.h>

#include <vector>
#include <map>
#include <string>

#include <KncLua.h>
#include <lua_tinker.h>
#include "Lua/KLuaManager.h"
#include "ClientPacket.h"
#include "BaseServer.h"
#include "XSLInventory.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#endif

inline void TableBind( KLuaManager* pLuaManager )
{
	bool	retVal = true;
	int		index = 1;
	std::string	buffer;

	if( pLuaManager->GetValue(index,buffer) == E_FAIL )
		retVal = false;
	else
		retVal = true;

	while( retVal == true )
	{
		_JIF( 0 == luaL_dostring( g_pLua, buffer.c_str() ), return );

		if( pLuaManager->GetValue(index,buffer) == E_FAIL )
			retVal = false;
		else
			retVal = true;

		index++;
	}
}

inline bool IncludeLua_( const char* pFileName )
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}

class CXSLStat
{
	public:
		struct Stat
		{
			float	m_fBaseHP;

			//공격
			float	m_fAtkPhysic;
			float	m_fAtkMagic;			

			//방어
			float	m_fDefPhysic;
			float	m_fDefMagic;

			Stat()
			{
				Init();
			}
			void Init()
			{
				m_fBaseHP			= 0.0f;
				m_fAtkPhysic		= 0.0f;
				m_fAtkMagic			= 0.0f;
				m_fDefPhysic		= 0.0f;
				m_fDefMagic			= 0.0f;
			}
			void AddStat( const Stat& baseStat )
			{
				m_fBaseHP			+= baseStat.m_fBaseHP;
				m_fAtkPhysic		+= baseStat.m_fAtkPhysic;
				m_fAtkMagic			+= baseStat.m_fAtkMagic;
				m_fDefPhysic		+= baseStat.m_fDefPhysic;
				m_fDefMagic			+= baseStat.m_fDefMagic;
			}
		};

	public:
		CXSLStat(void);
		~CXSLStat(void);
};



