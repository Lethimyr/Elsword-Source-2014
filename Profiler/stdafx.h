// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.


//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif 

#include <windows.h>
#include <crtdbg.h>
#include <strsafe.h>

//#ifdef _DEBUG // 2006-1-12, 14:32:55
//#ifndef D3D_DEBUG_INFO
//#define D3D_DEBUG_INFO

//#endif // #ifndef D3D_DEBUG_INFO
//#endif // _DEBUG
#include <d3dx9.h> // KJohnGen.h uses DirectX9 library if required


//#include <d3dx9.h>
//#pragma warning(push)
#pragma warning(disable: 4995)
#include <list>
//#include <string>


#include <vector>
#include <malloc.h>
#pragma warning(push)
#pragma warning(disable: 4276)



#include "KJohnGen.h"
#include "KJohnStrUtil.h"

#include "KTimer.h"
#include "./jprofile/KProfile.h"
#include "./jprofile/KBlockProfile.h"
#include "./jprofile/RHProfile.h"


#include "KMessageConsole.h"


#include "KStaticSingleton.h"

#include "KProfileMan.h"
