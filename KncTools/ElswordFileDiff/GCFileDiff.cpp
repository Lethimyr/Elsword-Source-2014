// GCFileDiff.cpp : Defines the class behaviors for the application.
//


// dmlee 2008.05.13
// command line param으로 두 폴더 path를 입력받고 newver.xml 파일 생성하는 기능 추가, visualbuild에서 사용하기 위해
// 첫번째 인자로 옛날 버전 폴더, 두번째 인자로 새 버전 폴더 입력



#include "stdafx.h"
#include "GCFileDiff.h"
#include "GCFileDiffDlg.h"
#include "KMsgBox.h"

#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffApp

BEGIN_MESSAGE_MAP(CGCFileDiffApp, CWinApp)
	//{{AFX_MSG_MAP(CGCFileDiffApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffApp construction

CGCFileDiffApp::CGCFileDiffApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGCFileDiffApp object

CGCFileDiffApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGCFileDiffApp initialization

BOOL CGCFileDiffApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/



	//{{ dmlee 2008.05.13 command line param으로 두 폴더 path를 입력받고 newver.xml 파일 생성
	// 1. command line params이 있으면 파싱해서 처리하고 return
	LPSTR *szArglist = NULL;
	int nArgs = 0;

	szArglist = CommandLineToArgvA( GetCommandLine(), &nArgs );
	if( NULL != szArglist && nArgs == 3 )
	{
		for( int i=0; i<nArgs; i++) 
		{
			printf( "%s", szArglist[i] );
		}

		std::string strOldPath = szArglist[1];
		std::string strNewPath = szArglist[2];

		CGCFileDiffDlg dlg;
		dlg.CreateNewVersionFile( strOldPath, strNewPath );


		// Free memory allocated for CommandLineToArgvW arguments.
		LocalFree(szArglist);

		// exit the application
		return FALSE;
	}
	//}} dmlee 2008.05.13 command line param으로 두 폴더 path를 입력받고 newver.xml 파일 생성



	// 2. 다이얼로그에서 파일 폴더 입력받아서 
	CGCFileDiffDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



PCHAR* CGCFileDiffApp::CommandLineToArgvA( PCHAR CmdLine, int* _argc )
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

	len = strlen(CmdLine);
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

