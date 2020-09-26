/********************************************************************
	created:	2005/02/22
	created:	22:2:2005   16:40
	author:		Florist.
	
	description:	
        050129. 초기 release.
        050222. 내부 COM 초기화 처리 삭제. ( 초기화 flag가 문제됨. 클라이언트에서 직접 초기화)

*********************************************************************/
#pragma once

#undef _DLLAPI

#if defined(_USRDLL)    // DLL library compile 중
#   define _DLLAPI __declspec(dllexport)
#else
#   define _DLLAPI __declspec(dllimport)
#endif

//#ifndef _USRDLL
//#   ifdef _DEBUG
//#       pragma comment( lib, "KncFirewallD" )
//#   else
//#       pragma comment( lib, "KncFirewall" )
//#   endif
//#endif

#pragma comment( lib, "KncFirewall" )

#define _WIN32_DCOM 
#include <tchar.h>
#include <wchar.h>

#include <windows.h>

extern "C"
{
    namespace KncFirewall 
    {
        enum ENUM_RETURN 
        {
            KNC_OK = 0,                 // ㅇㅋㅂㄹ.
            __INIT_COM_FAILED,          // COM 초기화 실패. 과연 이런 경우도 있을 것인가? // 050222. 내부 COM 초기화 삭제.
            NOT_WINXP_SP2,              // Windows XP SP2 가 아님.
            NO_EXCEPTION_ALLOWED,       // '모든 예외 허용 안함' 모드 동작중 - 게임 실행 불가능
            NOT_SYSADMIN,               // 예외 항목을 변경하려 했으나 시스템 ADMIN 권한이 없음
        };

        // 050222. 아래 세 함수 모두 사용하기 전에 반드시 CoInitialize 가 된 상태여야 한다.
        // 그렇지 않으면 WindowXP sp2 에서도 WindowXP sp2 가 아니라고 인식하게 된다.
		_DLLAPI bool OnInstallApplication( WCHAR* szwFullFilePath, WCHAR* szwName );
        _DLLAPI bool OnUninstallApplication( WCHAR* szwFullFilePath );
        _DLLAPI int GuarantedFireWallException( WCHAR* szwFullFilePath, WCHAR* szwName );
    }
}
