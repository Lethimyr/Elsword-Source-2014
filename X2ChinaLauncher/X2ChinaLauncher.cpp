// X2ChinaLauncher.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "X2ChinaLauncher.h"
#include "X2ChinaLauncherDlg.h"
#include "tinyxml/tinyxml.h"
#include <wininet.h>
#include "Config.h"
#include "define.h"
//#include "adler32.h"
#include "patchthread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#define MULTI_CLIENT_OFF

// CX2ChinaLauncherApp

BEGIN_MESSAGE_MAP(CX2ChinaLauncherApp, CWinApp)
	//ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

// 패쳐패치 이후 실행된 것인지 판별
bool g_bPatcherPatched = false;


// CX2ChinaLauncherApp 생성

CX2ChinaLauncherApp::CX2ChinaLauncherApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CX2ChinaLauncherApp 개체입니다.

CX2ChinaLauncherApp theApp;


// CX2ChinaLauncherApp 초기화

BOOL CX2ChinaLauncherApp::InitInstance()
{
#if 0
#include <crtdbg.h>
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
	_CrtDumpMemoryLeaks();
	_CrtSetBreakAlloc(369);
#endif

	//런처 초기화
	InitLauncher();

	// 패쳐 패치 후 재실행 된 것 표시
	if( *m_lpCmdLine == '1' )
	{
		g_bPatcherPatched = true;
	}

	//이미 런처가 실행중인지 확인한다.
	{
		DWORD dwStart = GetTickCount();
		while( true )
		{
			// 예전 프로세스가 끝났는지 확인
			g_hMutex = ::CreateMutex( NULL, TRUE, _T("ChinaLauncher") );
			if( GetLastError() != ERROR_ALREADY_EXISTS )
				break;

			::ReleaseMutex( g_hMutex );
			::CloseHandle( g_hMutex );

			// 1초 지났는지 확인
			DWORD dwNow = GetTickCount();
			if( dwNow - dwStart > 1000 )
			{
				//"이미 런처가 실행중입니다." 문구 출력
				CString strMessage;
				strMessage.LoadString(IDS_ALREADYRUNNINGLAUNCHER_MESSAGE);

				AfxMessageBox(strMessage, MB_OK);

				return FALSE; 
			}
		}
	}

#ifdef MULTI_CLIENT_OFF
#ifndef _IN_HOUSE_
	HANDLE hClientMutex = ::CreateMutex( NULL, TRUE, L"ElswordX2" );

	if( GetLastError() == ERROR_ALREADY_EXISTS ) 
	{ 
		//"클라이언트를 종료하시고 런처를 재실행주십시오." 문구 출력
		CString strMessage;
		strMessage.LoadString(IDS_ALREADYRUNNINGCLIENT_MESSAGE);

		AfxMessageBox(strMessage, MB_OK);

		//::ReleaseMutex( g_hLauncherMutex );
		//::CloseHandle( g_hLauncherMutex );
		//g_hLauncherMutex = NULL;

		return FALSE; 
	} 
	else
	{
		::ReleaseMutex( hClientMutex );
	}
#endif _IN_HOUSE_
#endif //MULTI_CLIENT_OFF

#if 0
	if(!LoadLauncherPatchServer())
	{
		//"서버에 접속할 수 없습니다" 라는 문구 출력
		CString strMessage;
		strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

		AfxMessageBox(strMessage, MB_OK);

		//::ReleaseMutex( g_hLauncherMutex );
		//::CloseHandle( g_hLauncherMutex );
		//g_hLauncherMutex = NULL;

		return FALSE;
	}

	unsigned int uiChecksum;
	int iSelect = rand()%m_vecAddr.size();
	std::string strAddr = m_vecAddr[iSelect].strValue;
	std::string strPath = m_vecAddr[iSelect].strPath;

	{
		int retry = 0;

		//런처 버전을 살펴본다.
		while(!GetVersionInServer(strAddr, strPath+"/patchinfo.xml", uiChecksum))
		{
			iSelect = rand()%m_vecAddr.size();
			strAddr = m_vecAddr[iSelect].strValue;
			strPath = m_vecAddr[iSelect].strPath;

			if(MAX_RETRY == ++retry)
			{
				//"서버에 접속할 수 없습니다" 라는 문구 출력
				CString strMessage;
				strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

				AfxMessageBox(strMessage, MB_OK);

				//::ReleaseMutex( g_hLauncherMutex );
				//::CloseHandle( g_hLauncherMutex );
				//g_hLauncherMutex = NULL;

				return FALSE;
			}
		}
	}

	if(IsNewVersion(uiChecksum))
	{
		int retry = 0;

		while(!PatchLauncher(strAddr, strPath+"/"+"ElswordLauncher.exe", "ElswordLauncher.exe"))
		{
			iSelect = rand()%m_vecAddr.size();
			strAddr = m_vecAddr[iSelect].strValue;
			strPath = m_vecAddr[iSelect].strPath;

			if(MAX_RETRY == ++retry)
			{
				//"서버에 접속할 수 없습니다" 라는 문구 출력
				CString strMessage;
				strMessage.LoadString(IDS_SERVERERROR_MESSAGE);

				AfxMessageBox(strMessage, MB_OK);

				//::ReleaseMutex( g_hLauncherMutex );
				//::CloseHandle( g_hLauncherMutex );
				//g_hLauncherMutex = NULL;

				return FALSE;
			}
			::Sleep( MAX_DELAY );
		}

		//::ReleaseMutex( g_hLauncherMutex );
		//::CloseHandle( g_hLauncherMutex );
		//g_hLauncherMutex = NULL;

		ReRunLauncher();
		return FALSE;
	}
#endif

	// 뮤텍스로 게임 클라이언트가 이미 실행 중인지 확인한다.
	//g_hMutex = ::CreateMutex( NULL, TRUE, _T("ElswordX2") );

	//if( GetLastError() == ERROR_ALREADY_EXISTS ) 
	//{ 
	//	AfxMessageBox( _T("게임이 이미 실행중입니다.")); 
	//	DestroyWindow();
	//	return FALSE; 
	//}

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성한 응용 프로그램"));

	CX2ChinaLauncherDlg dlg;
	m_pMainWnd = &dlg;






	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 대화 상자가 확인을 눌러 없어지는 경우 처리할
		// 코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 대화 상자가 취소를 눌러 없어지는 경우 처리할
		// 코드를 배치합니다.
	}

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고
	// 응용 프로그램을 끝낼 수 있도록 FALSE를 반환합니다.
	return FALSE;
}

int CX2ChinaLauncherApp::ExitInstance()
{
	if(g_hMutex)
	{
		::ReleaseMutex( g_hMutex );
		::CloseHandle( g_hMutex );
		g_hMutex = NULL;
	}

	return CWinApp::ExitInstance(); 
}

void CX2ChinaLauncherApp::InitLauncher()
{
	char path[2048];
	GetModuleFileNameA(NULL,path,2048);
	std::string pathstr = path;
	std::transform(pathstr.begin(),pathstr.end(),pathstr.begin(),tolower);

	int last = pathstr.find_last_of('\\');
	m_strLauncherName = pathstr.substr(last+1);
	
	LPSTR fileptr;	
	GetFullPathNameA(".",2048,path,&fileptr);
	m_strLocalPath = path;
	std::transform(m_strLocalPath.begin(), m_strLocalPath.end(), m_strLocalPath.begin(),tolower);
	if(m_strLocalPath.find_last_of('\\') +1 != m_strLocalPath.length())
		m_strLocalPath.append("\\");	

	std::string tempfilename = m_strLocalPath+m_strLauncherName+".tmp";
	DeleteFileForce(tempfilename);
}

BOOL CX2ChinaLauncherApp::DeleteFileForce(const std::string filename)
{
	if(DeleteFileA(filename.c_str()) == 0)
	{
		if(GetLastError() == ERROR_ACCESS_DENIED)
		{
			if(GetFileAttributesA(filename.c_str()) & FILE_ATTRIBUTE_READONLY)
			{
				SetFileAttributesA(filename.c_str(),FILE_ATTRIBUTE_NORMAL);
				if(DeleteFileA(filename.c_str()) != 0)				
					return true;
			}
		}
		else if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			return true;
		}
	}
	else
	{
		return true;
	}
	return false;

}

