// X2ItemTool.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "X2ItemTool.h"
#include "MainFrm.h"

#include "X2ItemToolDoc.h"
#include "X2ItemToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2ItemToolApp

BEGIN_MESSAGE_MAP(CX2ItemToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CX2ItemToolApp 생성

CX2ItemToolApp::CX2ItemToolApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CX2ItemToolApp 개체입니다.

CX2ItemToolApp theApp;
// 이 식별자는 응용 프로그램에서 통계적으로 고유한 값을 가지도록 생성되었습니다.
// 특정 식별자를 선호할 경우 변경할 수 있습니다.

// {F55175D4-DEA7-48C5-8285-FA9A8CC99042}
static const CLSID clsid =
{ 0xF55175D4, 0xDEA7, 0x48C5, { 0x82, 0x85, 0xFA, 0x9A, 0x8C, 0xC9, 0x90, 0x42 } };
const GUID CDECL BASED_CODE _tlid =
		{ 0xDAFC907, 0xC29A, 0x41FC, { 0x99, 0x83, 0x1, 0x9, 0x97, 0xA3, 0x73, 0x8C } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CX2ItemToolApp 초기화

BOOL CX2ItemToolApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
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
		RUNTIME_CLASS(CX2ItemToolDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CX2ItemToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// COleTemplateServer를 문서 템플릿에 연결합니다.
	// COleTemplateServer는 OLE 컨테이너를 요청하는 대신 문서 템플릿에
	// 지정된 정보를 사용하여 새 문서를
	// 만듭니다.
	m_server.ConnectTemplate(clsid, pDocTemplate, TRUE);
		// 참고: SDI 응용 프로그램은 명령줄에 /Embedding 또는 /Automation이
		// 있을 경우에만 서버 개체를 등록합니다.
	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 응용 프로그램이 /Embedding 또는 /Automation 스위치로 시작되었습니다.
	// 응용 프로그램을 자동화 서버로 실행합니다.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// 모든 OLE 서버 팩터리를 실행 중으로 등록합니다. 이렇게 하면
		// OLE 라이브러리가 다른 응용 프로그램에서 개체를 만들 수 있습니다.
		COleTemplateServer::RegisterAll();

		// 주 창을 표시하지 않습니다.
		return TRUE;
	}
	// 응용 프로그램이 /Unregserver 또는 /Unregister 스위치로 시작되었습니다. 
	// typelibrary를 등록 취소합니다. 다른 등록 취소는 ProcessShellCommand()에서 발생합니다.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, NULL, NULL, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// 응용 프로그램이 독립 실행형으로 시작되었거나 다른 스위치로 시작되었습니다(예: /Register
	// 또는 /Regserver). typelibrary를 포함하여 레지스트리 항목을 업데이트합니다.
	else
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}
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
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CX2ItemToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CX2ItemToolApp 메시지 처리기

