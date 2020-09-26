// X2EncryptFileViewer.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "X2EncryptFileViewer.h"
#include "MainFrm.h"

#include "X2EncryptFileViewerDoc.h"
#include "X2EncryptFileViewerView.h"
#include <fstream>
#include "FileCrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


lua_State* g_pLua	= NULL;

// CX2EncryptFileViewerApp

BEGIN_MESSAGE_MAP(CX2EncryptFileViewerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CX2EncryptFileViewerApp 생성

CX2EncryptFileViewerApp::CX2EncryptFileViewerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CX2EncryptFileViewerApp 개체입니다.

CX2EncryptFileViewerApp theApp;

// CX2EncryptFileViewerApp 초기화

BOOL CX2EncryptFileViewerApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	//lua init
	{
		g_pLua = lua_open();

		if( g_pLua == NULL )
		{
			::MessageBoxW( NULL, L"루아 생성 실패.!", L"", MB_OK );
			return FALSE;
		}

		luaopen_base( g_pLua );  // 루아함수 tonumber를 사용해야 한다.
		luaopen_table( g_pLua );
		luaopen_io( g_pLua );
		luaopen_string( g_pLua );
		luaopen_math( g_pLua );
		lua_settop( g_pLua, 0 );

		lua_tinker::lua_state::open( g_pLua );
	}

	// Laod Config
	LoadConfig();	
	
	// lua close
	lua_close( g_pLua );




	//{{ dmlee 2008.05.26 command line param이 있으면 암호화된 파일을 일반 텍스트로 변환해서 출력
	LPSTR* szArglist = NULL;
	int nArgs = 0;

	szArglist = CommandLineToArgvA( (PCHAR) GetCommandLine(), &nArgs );
	if( NULL != szArglist && nArgs == 3 )
	{
		for( int i=0; i<nArgs; i++) 
		{
			printf( "%s", szArglist[i] );
		}

		std::string srcPath = szArglist[1];
		std::string destPath = szArglist[2];

		// Free memory allocated for CommandLineToArgvW arguments.
		LocalFree(szArglist);


		char* cBuffer = NULL;

		std::ifstream ifs;
		ifs.open( srcPath.c_str(), std::ios::binary );
		if( ifs.is_open() == false )
		{
			return FALSE;
		}

		std::string strBuf;
		char cReadCur;
		while( !ifs.eof() )
		{
			ifs.get( cReadCur );
			strBuf.append( 1, cReadCur );
		}	

		if( cBuffer != NULL )
			delete[] cBuffer;

		cBuffer = CFileCrypt::FileDecrypt( theApp.GetEncryptionKeyList(), strBuf.c_str(), (int)strBuf.size() );

		std::ofstream ofs;	
		ofs.open( destPath.c_str(), std::ios::binary );
		if( ofs.is_open() == true )
		{
			if( NULL != cBuffer )
			{
				ofs.write( cBuffer, (std::streamsize)strlen( cBuffer ) );
			}
			ofs.close();
		}

		if( cBuffer != NULL )
			delete[] cBuffer;


		// exit the application
		return FALSE;
	}
	//}} dmlee 2008.05.26 command line param이 있으면 암호화된 파일을 일반 텍스트로 변환해서 출력




	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.
	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	// 문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CX2EncryptFileViewerDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CX2EncryptFileViewerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 명령줄에 지정된 명령을 디스패치합니다. 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	// SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.









	return TRUE;
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDecryptSave();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_COMMAND(ID_DECRYPT_SAVE, OnDecryptSave)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CX2EncryptFileViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CX2EncryptFileViewerApp 메시지 처리기

void CX2EncryptFileViewerApp::LoadConfig()
{
	lua_tinker::class_<CX2EncryptFileViewerApp>("CX2EncryptFileViewerApp")		
		.def("AddEncryptionKey",			CX2EncryptFileViewerApp::AddEncryptionKey_Lua)
		;

	lua_tinker::decl( "CX2EncryptFileViewerApp", this );

	char acCurPath[_MAX_PATH] = {0};
	GetCurrentDirectoryA(_MAX_PATH, acCurPath);
	std::string strPath = acCurPath;
	strPath += "\\";
	strPath += "Config.lua";

	if( 0 != lua_dofile( g_pLua, strPath.c_str() ) )
	{
		::MessageBoxW( NULL, L"Config.lua open failed!", L"", MB_OK );
	}	
}

bool CX2EncryptFileViewerApp::AddEncryptionKey_Lua()
{
	KLuaManager luaManager( g_pLua );

	int iEncryptionKey;

	LUA_GET_VALUE_RETURN(	luaManager,	"m_iEncryptionKey",		iEncryptionKey,		0,	goto LoadFail; );

	m_vecEncryptionKey.push_back( iEncryptionKey );

	return true;

LoadFail:
	::MessageBoxW( NULL, L"AddEncryptionKey 파싱 실패", L"", MB_OK );

	return false;
}
void CAboutDlg::OnDecryptSave()
{
	MessageBox( "" );
}







PCHAR* CX2EncryptFileViewerApp::CommandLineToArgvA( PCHAR CmdLine, int* _argc )
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = (ULONG)strlen(CmdLine);
	i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
		i + (len+2)*sizeof(CHAR));

	_argv = (PCHAR)(((PUCHAR)argv)+i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while( a = CmdLine[i] ) {
		if(in_QM) {
			if(a == '\"') {
				in_QM = FALSE;
			} else {
				_argv[j] = a;
				j++;
			}
		} else {
			switch(a) {
case '\"':
	in_QM = TRUE;
	in_TEXT = TRUE;
	if(in_SPACE) {
		argv[argc] = _argv+j;
		argc++;
	}
	in_SPACE = FALSE;
	break;
case ' ':
case '\t':
case '\n':
case '\r':
	if(in_TEXT) {
		_argv[j] = '\0';
		j++;
	}
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	break;
default:
	in_TEXT = TRUE;
	if(in_SPACE) {
		argv[argc] = _argv+j;
		argc++;
	}
	_argv[j] = a;
	j++;
	in_SPACE = FALSE;
	break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
}