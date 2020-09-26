///////////////////////////////////////////////////////////////////////////////
//
//  Module: CrashHandler.cpp
//
//    Desc: See CrashHandler.h
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrashHandler.h"
#include "zlibcpp.h"
#include "excprpt.h"
#include "maindlg.h"
#include "process.h"

//#include "mailmsg.h"

// global app module
CAppModule _Module;

// maps crash objects to processes
CSimpleMap<int, CCrashHandler*> _crashStateMap;

// unhandled exception callback set with SetUnhandledExceptionFilter()
LONG WINAPI CustomUnhandledExceptionFilter(PEXCEPTION_POINTERS pExInfo)
{
   _crashStateMap.Lookup(_getpid())->GenerateErrorReport(pExInfo);

   return EXCEPTION_EXECUTE_HANDLER;
}

CCrashHandler::CCrashHandler(LPGETLOGFILE lpfn /*=NULL*/, 
                             LPCTSTR lpcszTo /*=NULL*/, 
                             LPCTSTR lpcszSubject /*=NULL*/)
{
   // wtl initialization stuff...
	//HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hRes = S_OK;//::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

   hRes = _Module.Init(NULL, GetModuleHandle("KncCrashRpt.dll"));
   ATLASSERT(SUCCEEDED(hRes));

	::DefWindowProc(NULL, 0, 0, 0L);

   // initialize member data
   m_lpfnCallback = NULL;
   m_oldFilter    = NULL;

   // save user supplied callback
   if (lpfn)
      m_lpfnCallback = lpfn;

   // add this filter in the exception callback chain
   m_oldFilter = SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);

   // attach this handler with this process
   m_pid = _getpid();
   _crashStateMap.Add(m_pid, this);

   // save optional email info
   m_sTo = lpcszTo;
   m_sSubjectPrefix = lpcszSubject;
}

CCrashHandler::~CCrashHandler()
{
   // reset exception callback
   if (m_oldFilter)
      SetUnhandledExceptionFilter(m_oldFilter);

   _crashStateMap.Remove(m_pid);

   // uninitialize
   _Module.Term();
//	::CoUninitialize();

}

void CCrashHandler::AddFile(LPCTSTR lpFile, LPCTSTR lpDesc)
{
   // make sure the file exist
   HANDLE hFile = ::CreateFile(
                     lpFile,
                     GENERIC_READ,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     0);
   if (hFile)
   {
      // add file to report
      m_files[lpFile] = lpDesc;
      ::CloseHandle(hFile);
   }
}

void CCrashHandler::GenerateErrorReport(PEXCEPTION_POINTERS pExInfo)
{
   CExceptionReport  rpt(pExInfo);
   CMainDlg          mainDlg;
   CZLib             zlib;
   CString           sTempFileName = CUtility::getTempFileName();
   unsigned int      i;

   // let client add application specific files to report
   if (m_lpfnCallback && !m_lpfnCallback(this))
      return;

   m_SystemAnalysis.StoreMySystemAnalysis();

   // add crash files to report
   m_files[rpt.getCrashFile()] = CString((LPCTSTR)IDS_CRASH_DUMP);
   m_files[rpt.getCrashLog()] = CString((LPCTSTR)IDS_CRASH_LOG);
   // add symbol files to report
   for (i = 0; i < (UINT)rpt.getNumSymbolFiles(); i++)
      m_files[(LPCTSTR)rpt.getSymbolFile(i)] = 
      CString((LPCTSTR)IDS_SYMBOL_FILE);
	m_files["HardwareInfo.txt"] = "HardwareInfo.txt";
   // zip the report
   if (!zlib.Open(sTempFileName))
      return;
   
   // add report files to zip
   TStrStrMap::iterator cur = m_files.begin();
   for (i = 0; i < m_files.size(); i++, cur++)
      zlib.AddFile((*cur).first);
   zlib.Close();
//	if ( m_SystemAnalysis.StoreMySystemAnalysis() == false )
//		MessageBox(0, "실패", 0, 0);
   // display main dialog

   mainDlg.m_pUDFiles = &m_files;

   CopyFile(sTempFileName, "C:\\CrashReport.zip", TRUE);
   SetMailSubject( mainDlg.m_sSuject );
   MailReport(rpt, sTempFileName, mainDlg.m_sEmail, mainDlg.m_sDescription);

   if (IDOK == mainDlg.DoModal())
   {
   }
   DeleteFile("./HardwareInfo.txt");
   DeleteFile("C:\\CrashReport.zip");
   DeleteFile(sTempFileName);
}

BOOL CCrashHandler::SaveReport(CExceptionReport&, LPCTSTR lpcszFile)
{
   // let user more zipped report
   return (CopyFile(lpcszFile, CUtility::getSaveFileName(), TRUE));
}

BOOL CCrashHandler::MailReport(CExceptionReport&, LPCTSTR lpcszFile,
                               LPCTSTR lpcszEmail, LPCTSTR lpcszDesc)
{
	_Mail mail;
	CSendMailDlg sendMailDlg;
//	TCHAR smtpserver[256] = {0,};
//	GetPrivateProfileString("SETUP", "SMTP","ERROR", smtpserver, 256, "./CrashReport.ini");	
	unsigned long ulSMTPAddr = SMTP_TransAddr("www.kogstudios.com");
	mail.from		=(char*)lpcszEmail;
	mail.to			=(LPSTR)(LPCSTR)m_sTo;
	mail.cc			=NULL;
	mail.bcc		=NULL;
	mail.dataHead1	=MIME_MakeHead1(mail.to, mail.cc);
	mail.dataHead2	=MIME_MakeHead2("크래쉬리포트보내는사람", mail.from, m_sSubject.GetBuffer(m_sSubject.GetLength()), 1);
	mail.dataBody	=MIME_MakeBody((char*)lpcszDesc, 0);
	char **pFiles=NULL;
	int cFiles=0;

	pFiles=(char**)malloc(sizeof(char*));
	pFiles[0]="C:\\CrashReport.zip";
	cFiles++;

	mail.dataAttach	=MIME_MakeAttach(pFiles, cFiles);
	mail.dataEnd	=MIME_GetEnd();

	sendMailDlg.SetMailData(mail, ulSMTPAddr);
	sendMailDlg.DoModal();

	return TRUE;
}

void CCrashHandler::SetMailSubject( CString sSubject )
{
	m_sSubject = m_sSubjectPrefix + sSubject;
}


void CCrashHandler::SetMailSubjectPrefix( LPCTSTR szPrefix )
{
	m_sSubjectPrefix = szPrefix;
}

