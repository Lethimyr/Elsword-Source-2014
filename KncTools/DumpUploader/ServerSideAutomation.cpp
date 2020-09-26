// ServerSideAutomation.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ServerSideAutomation.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.
#include "AutoManager.h"
#include "MailSender.h"
#include "KncFirewall.h"
CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
        HRESULT hrComInit = CoInitializeEx( 0, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE );
        if( hrComInit != RPC_E_CHANGED_MODE && FAILED(hrComInit) )
        {
            printf( "Failed to init COM: 0x%08lx\n", hrComInit );
            return 1; 
        }//end if

        char pathbuff[MAX_PATH]     = {0};
        ::GetModuleFileName( NULL, pathbuff, MAX_PATH );            // get file name (full path)

        // install...
        bool bReturn = KncFirewall::OnInstallApplication( pathbuff, pathbuff );
        ::printf( "Install - bReturn : %d \n\n", bReturn );
        //::system( "pause" );

        // on execute...
        int nReturn = KncFirewall::GuarantedFireWallException( pathbuff, pathbuff );
        ::printf( "\nGuaranted Firewall Exception - nReturn : %d \n\n", nReturn );
        //::system( "pause" );

        CAutoManager autoManager;        
        autoManager.Process( argc, argv );

        // on uninstall...
        bReturn = KncFirewall::OnUninstallApplication( pathbuff );
        ::printf( "\nUninstall - bReturn : %d \n\n", bReturn );
        //::system( "pause" );

     
	}

	return nRetCode;
}
