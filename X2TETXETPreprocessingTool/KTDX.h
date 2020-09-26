#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <d3dx9.h>
#include <vector>
#include <set>
#include <map>
#include <hash_map>
#include <algorithm>
#include <boost/static_assert.hpp>
#include <boost/foreach.hpp>

using   namespace std;

#define ASSERT(exp)

#include <CRC_32.h>


#include "../LuaCommon/KLuaManager.h"
#include "../LuaCommon/KLuabinder.h"

#define X2OPTIMIZE_TET_XET_PREPROCESSING
#define X2OPTIMIZE_SCRIPT_PREPROCESSING_TOOL

#include    <X2Stub.h>
#include    <KFileSerializer.h>
#include    <KTDXDeviceTET_Preprocessing.h>
#include    <KTDXDeviceXET_Preprocessing.h>
#include    <KTDXCommonFunc.h>


#define ARRAY_SIZE(a)       (sizeof(a)/sizeof((a)[0]))

extern  void        _ErrorLogMsg( int errnum, const char* pszMsg, const char* pszFile, const char* pszFunction, int line );
extern  void        _ErrorLogMsg( int errnum, const wchar_t* pwszMsg, const char* pszFile, const char* pszFunction, int line );

#define ErrorLogMsg( errorEnum, errorMsg )		\
{ \
    _ErrorLogMsg( errorEnum, errorMsg, __FILE__, __FUNCTION__, __LINE__ ); \
}

#define KEM_ERROR95     95
#define KEM_ERROR96     96
#define XEM_ERROR146    146
