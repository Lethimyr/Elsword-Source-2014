// KEsdRemoteController_Console.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "KEsdRemoteController_Console.h"
#include "KEsdRemoteController_ConsoleDlg.h"
#include "KEsdRemoteController_ConsoleLoginDlg.h"
#include <vld.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

lua_State* g_pLua	= NULL;


// CKEsdRemoteController_ConsoleApp

BEGIN_MESSAGE_MAP(CKEsdRemoteController_ConsoleApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKEsdRemoteController_ConsoleApp 생성

CKEsdRemoteController_ConsoleApp::CKEsdRemoteController_ConsoleApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CKEsdRemoteController_ConsoleApp 개체입니다.

CKEsdRemoteController_ConsoleApp theApp;


// CKEsdRemoteController_ConsoleApp 초기화

BOOL CKEsdRemoteController_ConsoleApp::InitInstance()
{
	CWinApp::InitInstance();

	//lua init
	{
		g_pLua = lua_open();

		if( g_pLua == NULL )
		{
			KDbgStm << "루아 생성 실패.!";
			return FALSE;
		}

		//luaopen_base( g_pLua );  // 루아함수 tonumber를 사용해야 한다.
		//luaopen_table( g_pLua );
		//luaopen_io( g_pLua );
		//luaopen_string( g_pLua );
		//luaopen_math( g_pLua );
		//lua_settop( g_pLua, 0 );

		luaL_openlibs( g_pLua );

		lua_settop( g_pLua, 0 );
	}

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성한 응용 프로그램"));

	// config파일 로드
	m_kImp.LoadConfig();
	
	CKEsdRemoteController_ConsoleLoginDlg loginDlg;

#ifdef COMPILE_NO_LOGIN
	INT_PTR nResponse = IDD_LOGIN;
#else
	INT_PTR nResponse = loginDlg.DoModal();
#endif

	if (nResponse == IDD_LOGIN)
	{
		CKEsdRemoteController_ConsoleDlg cDialog;
		m_pMainWnd = &cDialog;

		// AuthLevel에 따른 사용제한
#ifdef COMPILE_NO_LOGIN
		cDialog.SetAuthLevel( 1 );
#else
		cDialog.SetAuthLevel( m_kImp.GetAuthLevel() );
#endif

		INT_PTR nResponse2 = cDialog.DoModal();
		if (nResponse2 == IDOK)
		{
			// TODO: 여기에 대화 상자가 확인을 눌러 없어지는 경우 처리할
			// 코드를 배치합니다.
		}
		else if (nResponse2 == IDCANCEL)
		{
			// TODO: 여기에 대화 상자가 취소를 눌러 없어지는 경우 처리할
			// 코드를 배치합니다.
		}
	}
	else if (nResponse == IDCANCEL)
	{
		lua_close( g_pLua );
		return FALSE;
	}	

	//lua shutdown
	{
		lua_close( g_pLua );
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}
