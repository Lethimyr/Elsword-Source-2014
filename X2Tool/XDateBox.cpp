// XDateBox.cpp : implementation file
//

#include "stdafx.h"
#include "XDateBox.h"
#include ".\xdatebox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT NEAR WM_XDATEBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XDATEBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CXDateBox

CXDateBox* CXDateBox::m_pXDateBox = NULL; 

CXDateBox::CXDateBox()
{
	m_nVK = 0;
}

CXDateBox::~CXDateBox()
{
}


BEGIN_MESSAGE_MAP(CXDateBox, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CXDateBox)
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(DTN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXDateBox message handlers

CXDateBox* CXDateBox::GetInstance()
{
	if(m_pXDateBox == NULL)
	{
		m_pXDateBox = new CXDateBox;
	}
	return m_pXDateBox;
}

void CXDateBox::DeleteInstance()
{
	if(m_pXDateBox != NULL)
	{
		if(m_pXDateBox->m_hWnd != NULL)
			m_pXDateBox->DestroyWindow();

		delete m_pXDateBox;
		m_pXDateBox = NULL;
	}
}

void CXDateBox::OnKillFocus(CWnd* pNewWnd) 
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

	CMonthCalCtrl *pMonthWnd = GetMonthCalCtrl();
	if( !pMonthWnd )
	{
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		::SendMessage(::GetParent(m_hWnd), WM_XDATEBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);		
	}
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CXDateBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN ) 
	{
		// Key
		if( pMsg->wParam == VK_RETURN || 
			pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_TAB ||
			pMsg->wParam == VK_DELETE ) 
		{
			m_nVK = pMsg->wParam;

			CMonthCalCtrl *pMonthWnd = GetMonthCalCtrl();
			if( !pMonthWnd )
				::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
	}
	
	return CDateTimeCtrl::PreTranslateMessage(pMsg);
}

// 날짜 포맷으로 부터 날짜 문자열 획득
CString CXDateBox::GetDate(CTime &time, CString &sDateFormat)
{
	int nStart=0, nFind=0;
	CString sDate(_T("")), sTemp(_T(""));

	// Year(yyyy)
	nFind = sDateFormat.Find(_T("yyyy"), nStart);	
	if( nFind >= 0 )
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%04d"), time.GetYear());
		sDate += sTemp;

		nStart = nFind + 4;
	}

	// Month(MM)
	nFind = sDateFormat.Find(_T("MM"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetMonth());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Day(dd)
	nFind = sDateFormat.Find(_T("dd"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetDay());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Hour(HH)
	nFind = sDateFormat.Find(_T("HH"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetHour());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Minute(mm)
	nFind = sDateFormat.Find(_T("mm"), nStart);		
	if( nFind >= 0 )
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetMinute());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Seconds(ss)
	nFind = sDateFormat.Find(_T("ss"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetSecond());
		sDate += sTemp;		

		nStart = nFind + 2;
	}

	sDate += sDateFormat.Mid(nStart);

	sDate.Remove(_T('\''));
	return sDate;
}

void CXDateBox::OnCloseup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(::GetParent(m_hWnd), WM_XDATEBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);	

	*pResult = 0;
}

int CXDateBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDateTimeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
