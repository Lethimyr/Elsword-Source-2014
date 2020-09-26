//#pragma once  // 여러번 include 가능. 대신 #include "ImportKncSDK.h"를 명시한 헤더를 한번만 include되도록 설정하자.

#ifndef KNCLIBNAME
#error ImportKOGSDK.h를 include 하기 전에 KNCLIBNAME 매크로를 정의해 주어야 합니다.
#endif

#if (_MSC_VER == 1200 )     // Visual Studio 6.0
#   error KOGSDK는 Microsoft Visual Studio 6.0 컴파일러를 지원하지 않습니다. 
#elif (_MSC_VER == 1300 )   // Visual Studio .NET 2002
#   error KOGSDK는 Microsoft Visual Studio .NET 2002 컴파일러를 지원하지 않습니다. 
#elif (_MSC_VER == 1310 )   // Visual Studio .NET 2003
#   define CLVERSION   "71"
#elif (_MSC_VER == 1400 )   // Visual Studio 2005 Express Edition
#   error KOGSDK는 아직 Visual Studio 2005 Express Edition를 지원하지 않습니다. 준비중입니다.
#endif

#ifdef _MT
#   ifdef _DLL
#       ifdef _DEBUG
#           define CRTTYPE  "MDd"   // _MDd
#       else
#           define CRTTYPE  "MD"    // _MD
#       endif
#   else
#       ifdef _DEBUG
#           define CRTTYPE  "MTd"   // _MTd
#       else
#           define CRTTYPE  "MT"    // _MT
#       endif
#   endif
#else
#   ifdef _DEBUG
#       define CRTTYPE  "MLd"       // _MLd
#   else
#       define CRTTYPE  "ML"        // _ML
#   endif
#endif

#include <iostream>     // a Standard C++ Library Header

#pragma comment( lib, KNCLIBNAME CLVERSION CRTTYPE )
#pragma message( KNCLIBNAME CLVERSION CRTTYPE ".lib is imported" )

#undef KNCLIBNAME
#undef CLVERSION
#undef CRTTYPE
