// XComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "XComboBox.h"
#include ".\xcombobox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT NEAR WM_XCOMBOBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XCOMBOBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CXComboBox

CXComboBox* CXComboBox::m_pXComboBox = NULL; 

CXComboBox::CXComboBox()
{
}

CXComboBox::~CXComboBox()
{
}


BEGIN_MESSAGE_MAP(CXComboBox, CComboBox)
	//{{AFX_MSG_MAP(CXComboBox)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
//	ON_WM_CREATE()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXComboBox message handlers

CXComboBox* CXComboBox::GetInstance()
{
	if(m_pXComboBox == NULL)
	{
		m_pXComboBox = new CXComboBox;
	}
	return m_pXComboBox;
}

void CXComboBox::DeleteInstance()
{
	if(m_pXComboBox != NULL)
	{
		if(m_pXComboBox->m_hWnd != NULL)
			m_pXComboBox->DestroyWindow();

		delete m_pXComboBox;
		m_pXComboBox = NULL;
	}
}

void CXComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(GetParent()->m_hWnd, WM_XCOMBOBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);		
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CXComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN ) 
	{
		// Enter Key
		if( pMsg->wParam == VK_RETURN || 
			pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_TAB ) 
		{
			m_nVK = pMsg->wParam;
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}
void CXComboBox::OnCbnCloseup()
{
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(::GetParent(m_hWnd), WM_XCOMBOBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);	
}

/*
int CXComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here	

	return 0;
}
*/