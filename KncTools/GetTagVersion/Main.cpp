#include "KncFirewall.h"

#include <cstdio>       // printf
#include <process.h>    // system
#define _WIN32_DCOM     // windows.h 가 인클루드 되기 전에 _WIN32_COM이 정의되어있지 않으면
                        // CoInitializeEx를 사용할 수 없다.
#include <windows.h>    // GetModuleFileName, MAX_PATH

void main()
{
    // 050222. init COM. 내부에서 COM 초기화 하지 않는다. 직접 해주어야 한다.
    HRESULT hrComInit = CoInitializeEx( 0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE );
    if( hrComInit != RPC_E_CHANGED_MODE && FAILED(hrComInit) )
    {
        printf( "Failed to init COM: 0x%08lx\n", hrComInit );
        return; 
    }

    char pathbuff[MAX_PATH]     = {0};
    ::GetModuleFileName( NULL, pathbuff, MAX_PATH );            // get file name (full path)

    // install...
    bool bReturn = KncFirewall::OnInstallApplication( pathbuff, "KNC방화벽테스트" );
    ::printf( "Install - bReturn : %d \n\n", bReturn );
    ::system( "pause" );

    // on execute...
    int nReturn = KncFirewall::GuarantedFireWallException( pathbuff, "KNC방화벽테스트" );
    ::printf( "\nGuaranted Firewall Exception - nReturn : %d \n\n", nReturn );
    ::system( "pause" );

    // on uninstall...
    bReturn = KncFirewall::OnUninstallApplication( pathbuff );
    ::printf( "\nUninstall - bReturn : %d \n\n", bReturn );
    ::system( "pause" );
}