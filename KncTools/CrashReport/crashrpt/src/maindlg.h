///////////////////////////////////////////////////////////////////////////////
//
//  Module: maindlg.h
//
//    Desc: Main crash report dialog, responsible for gathering additional
//          user information and allowing user to examine crash report.
//
// Copyright (c) 2003 Michael Carruth
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _MAINDLG_H_
#define _MAINDLG_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Utility.h"
#include "DeadLink.h"
#include "detaildlg.h"
#include "aboutdlg.h"
#include "MailSMTPHeader.h"
#include "MailMIMEHeader.h"


//
// RTF load callback
//
DWORD CALLBACK LoadRTFString(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CString *sText = (CString*)dwCookie;
   LONG lLen = sText->GetLength();

   for (*pcb = 0; *pcb < cb && *pcb < lLen; (*pcb)++)
   {  
      pbBuff[*pcb] = sText->GetAt(*pcb);
   }

   return 0;
}


////////////////////////////// Class Definitions /////////////////////////////

// ===========================================================================
// CMainDlg
// 
// See the module comment at top of file.
//
_Mail m_Mail;
class CSendMailDlg;
CSendMailDlg *g_pSendMailDlg;
class CSendMailDlg : public CDialogImpl<CSendMailDlg>
{
public:
	enum { IDD = IDD_SENDMAILDLG};
	BEGIN_MSG_MAP(CSendMailDlg)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInitDialog)
	END_MSG_MAP()
	unsigned long m_ulSMTPAddr;
	CSendMailDlg()
	{
		g_pSendMailDlg = this;
	}
	static void _fnEnd(BOOL bSuccess)
	{
		MIME_Free(m_Mail.dataHead1);
		MIME_Free(m_Mail.dataHead1);
		MIME_Free(m_Mail.dataBody);
		MIME_Free(m_Mail.dataAttach);
/*		if (bSuccess)
			AfxGetMainWnd()->MessageBox("메일을 보냈습니다.");
		else
			AfxGetMainWnd()->MessageBox(SMTP_GetLastMsg());*/
		// 여기다 다이얼로그 닫는걸 놓는게 제일 좋을듯 하다.
		g_pSendMailDlg->EndDialog(g_pSendMailDlg->GetDlgCtrlID());
	}
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();
		// 데이타 다 만들고 이제 보낸다.
		if (!SMTP_SendMail(this->m_hWnd, m_ulSMTPAddr, &m_Mail, _fnEnd, NULL)){
			//실패
			MIME_Free(m_Mail.dataHead1);
			MIME_Free(m_Mail.dataHead1);
			MIME_Free(m_Mail.dataBody);
			MIME_Free(m_Mail.dataAttach);
			MessageBox( SMTP_GetLastMsg());	//실패의 원인보여주기
		}
		// 여기서 Dialog닫으면 메일 안온다.
		return TRUE;
	}
	void SetMailData( _Mail mail, unsigned long ulSMTPAddr)
	{
		m_Mail = mail;
		m_ulSMTPAddr = ulSMTPAddr;
	}
};

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

   CString	   m_sSuject;		 // 메일 제목
   CString     m_sEmail;         // Email: From
   CString     m_sDescription;   // Email: Body
   CDeadLink   m_link;           // Dead link
   TStrStrMap  *m_pUDFiles;      // Files <name,desc>

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(IDC_LINK, OnLinkClick)
      MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		COMMAND_ID_HANDLER(IDOK, OnSend)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

   
   //-----------------------------------------------------------------------------
   // CMainDlg
   //
   // Loads RichEditCtrl library
   //
   CMainDlg() 
   {
      LoadLibrary(CRichEditCtrl::GetLibraryName());
   };

	
   //-----------------------------------------------------------------------------
   // OnInitDialog
   //
   // 
   //
   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

	   // Add "About..." menu item to system menu.

	   // IDM_ABOUTBOX must be in the system command range.
      ATLASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX); 
      ATLASSERT(IDM_ABOUTBOX < 0xF000); 

      CMenu sysMenu;
      sysMenu.Attach(GetSystemMenu(FALSE));
      if (sysMenu.IsMenu())
      {
		   CString strAboutMenu;
		   strAboutMenu.LoadString(IDS_ABOUTBOX);
		   if (!strAboutMenu.IsEmpty())
		   {
            sysMenu.AppendMenu(MF_SEPARATOR);
			   sysMenu.AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		   }
	   }
      //
      // Set title using app name
      //
      SetWindowText(CUtility::getAppName());

      //
      // Use app icon
      //
      CStatic icon;
      icon.Attach(GetDlgItem(IDI_APPICON));
      icon.SetIcon(::LoadIcon((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));
      icon.Detach();

      //
      // Set failure heading
      //
      EDITSTREAM es;
      es.pfnCallback = LoadRTFString;

      CString sText;
      sText.Format(IDS_HEADER, CUtility::getAppName());
      es.dwCookie = (DWORD)&sText;

      CRichEditCtrl re;
      re.Attach(GetDlgItem(IDC_HEADING_TEXT));
      re.StreamIn(SF_RTF, es);
      re.Detach();

      //
      // Hook dead link
      //
      m_link.SubclassWindow(GetDlgItem(IDC_LINK));
	  
/*      HWND     hWndEmail = GetDlgItem(IDC_EMAIL);
      HWND     hWndDesc = GetDlgItem(IDC_DESCRIPTION);

	  TCHAR default_email[256] = {0,};
	  TCHAR default_desc[256] = {0,};
	  GetPrivateProfileString("DESC", "DEFAULT_EMAIL","ERROR", default_email, 256, "./CrashReport.ini");	
	  GetPrivateProfileString("DESC", "DEFAULT_DESC", "ERROR",default_desc, 256, "./CrashReport.ini");

      ::SetWindowText(hWndEmail, default_email );
      ::SetWindowText(hWndDesc,default_desc);
*/

      return TRUE;
	}

	
   //-----------------------------------------------------------------------------
   // OnLinkClick
   //
   // Display details dialog instead of opening URL
   //
   LRESULT OnLinkClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      CDetailDlg dlg;
      dlg.m_pUDFiles = m_pUDFiles;
      dlg.DoModal();
      return 0;
   }

   //-----------------------------------------------------------------------------
   // OnSysCommand
   //
   // 
   //
   LRESULT OnSysCommand(UINT, WPARAM wParam, LPARAM , BOOL& bHandled)
   {
      bHandled = FALSE;

      if ((wParam & 0xFFF0) == IDM_ABOUTBOX)
      {
         CAboutDlg dlg;
         dlg.DoModal();
         bHandled = TRUE;
      }

      return 0;
   }

	
   //-----------------------------------------------------------------------------
   // OnSend
   //
   // Send handler, validates email address entered, if one, and returns.
   //
   LRESULT OnSend(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
	  //HWND     hWndSubject = GetDlgItem(IDC_SUBJECT);
   //   HWND     hWndEmail = GetDlgItem(IDC_EMAIL);
   //   HWND     hWndDesc = GetDlgItem(IDC_DESCRIPTION);
	  //int      nSubjectLen = ::GetWindowTextLength(hWndSubject) +1;
	  //int      nEmailLen = ::GetWindowTextLength(hWndEmail) + 1;
   //   int      nDescLen = ::GetWindowTextLength(hWndDesc) + 1;

   //   LPTSTR lpStr = m_sEmail.GetBufferSetLength(nEmailLen);
   //   ::GetWindowText(hWndEmail, lpStr, nEmailLen);
   //   m_sEmail.ReleaseBuffer();

   //   lpStr = m_sDescription.GetBufferSetLength(nDescLen);
   //   ::GetWindowText(hWndDesc, lpStr, nDescLen);
   //   m_sDescription.ReleaseBuffer();

	  //lpStr = m_sSuject.GetBufferSetLength(nSubjectLen);
	  //::GetWindowText(hWndSubject, lpStr, nSubjectLen);
	  //m_sSuject.ReleaseBuffer();
	  
	  m_sEmail			= "none";
	  m_sDescription	= "none";
	  m_sSuject			= "none";
	  

      //
      // If an email address was entered, verify that
      // it [1] contains a @ and [2] the last . comes
      // after the @.
      //


	  //사용자의 email주소는 입력 받지 않는 것으로 하자.
	  //if (m_sEmail.GetLength() &&
   //       (m_sEmail.Find(_T('@')) < 0 ||
   //        m_sEmail.ReverseFind(_T('.')) < m_sEmail.Find(_T('@'))))
   //   {
   //      // alert user
   //      TCHAR szBuf[256];
		 //  ::LoadString(_Module.GetResourceInstance(), IDS_INVALID_EMAIL, szBuf, 255);
   //      MessageBox(szBuf, CUtility::getAppName(), MB_OK);
   //      // select email
   //      ::SetFocus(hWndEmail);
   //   }
   //   else
	  m_sEmail = "user_email@yahoo.com";
      {
	      EndDialog(wID);
      }

      return 0;
   }

   //-----------------------------------------------------------------------------
   // OnCancel
   //
   // 
   //
   LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
      EndDialog(wID);
		return 0;
   }

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif	// #ifndef _MAINDLG_H_
