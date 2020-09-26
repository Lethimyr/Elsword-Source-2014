// X2MassFile.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "X2MassFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CX2MassFileApp

BEGIN_MESSAGE_MAP(CX2MassFileApp, CWinApp)
	//{{AFX_MSG_MAP(CX2MassFileApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CX2MassFileApp construction

CX2MassFileApp::CX2MassFileApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CX2MassFileApp object

CX2MassFileApp theApp;

UINT g_uCustomClipbrdFormat = RegisterClipboardFormat ( _T("X2MassFile_3BCFE9D1_6D61_4cb6_9D0B_3BB3F643CA82") );
bool g_bNT = (0 == (GetVersion() & 0x80000000) );

/////////////////////////////////////////////////////////////////////////////
// CX2MassFileApp initialization

BOOL CX2MassFileApp::InitInstance()
{
	AfxOleInit();

	CX2MassFileDlg dlg;
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
