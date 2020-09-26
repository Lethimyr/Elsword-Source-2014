#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <d3dx9math.h>
#include <vector>
#include <set>
#include <map>
#include <hash_map>
#include <algorithm>
#include <boost/static_assert.hpp>
#include <boost/foreach.hpp>

using   namespace std;

#define ASSERT(exp)

#include    <CRC_32.h>

#include "../LuaCommon/KLuaManager.h"
#include "../LuaCommon/KLuabinder.h"

#define X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL
#define X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
#define X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
//#define ADD_ITEM_TEMPLET_ITEM

#include    <KFileSerializer.h>
#include    <X2Item_PreHeader.h>
#include    <X2Item.h>
#include    <X2ItemManager.h>
#include    <X2Stub.h>

#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))

extern  void        _ErrorLogMsg( int errnum, const char* pszMsg, const char* pszFile, const char* pszFunction, int line );
extern  void        _ErrorLogMsg( int errnum, const wchar_t* pwszMsg, const char* pszFile, const char* pszFunction, int line );

#define ErrorLogMsg( errorEnum, errorMsg )		\
{ \
    _ErrorLogMsg( errorEnum, errorMsg, __FILE__, __FUNCTION__, __LINE__ ); \
}

#define XEM_ERROR144    144       // Item.lua 파싱 오류
#define XEM_ERROR145    145       // SetItem.lua 파싱 오류
#define XEM_ERROR146    146       // 툴 오류



