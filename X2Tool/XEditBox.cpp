// XEditBox.cpp : implementation file
//

#include "stdafx.h"
#include "XEditBox.h"

#include "XSpinBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT NEAR WM_XEDITBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XEDITBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CXEditBox

CXEditBox* CXEditBox::m_pXEditBox = NULL; 

CXEditBox::CXEditBox()
{
	m_nMinNum = -1;
	m_nMaxNum = -1;

	m_nVK = 0;
}

CXEditBox::~CXEditBox()
{
	CXSpinBox::DeleteInstance();
}


BEGIN_MESSAGE_MAP(CXEditBox, CEdit)
	//{{AFX_MSG_MAP(CXEditBox)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_XSPINBOX_DELTA,OnSpinDelta)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXEditBox message handlers

CXEditBox* CXEditBox::GetInstance()
{
	if(m_pXEditBox == NULL)
	{
		m_pXEditBox = new CXEditBox;
	}
	return m_pXEditBox;
}

void CXEditBox::DeleteInstance()
{
	if(m_pXEditBox != NULL)
	{
		if(m_pXEditBox->m_hWnd != NULL)
			m_pXEditBox->DestroyWindow();

		delete m_pXEditBox;
		m_pXEditBox = NULL;
	}
}

void CXEditBox::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);	
	::SendMessage(GetParent()->m_hWnd, WM_XEDITBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)NULL);	
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CXEditBox::PreTranslateMessage(MSG* pMsg) 
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
	
	return CEdit::PreTranslateMessage(pMsg);
}

LRESULT CXEditBox::OnSpinDelta(WPARAM wParam, LPARAM lParam)
{
	BOOL bUp = (BOOL)wParam;

	CString sText(_T(""));
	GetWindowText(sText);
	int nValue = _tstoi(sText);

	if( bUp )
	{
		nValue++;
		if( nValue <= m_nMaxNum )
		{
			sText.Format(_T("%d"), nValue);
			SetWindowText(sText);
		}
	}
	else
	{
		nValue--;
		if( nValue >= m_nMinNum )
		{
			sText.Format(_T("%d"), nValue);
			SetWindowText(sText);
		}
	}

	return 0L;
}

int CXEditBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	// Create Spin Box
	if( m_nMinNum >=0 && m_nMaxNum >=0 )
	{
		ModifyStyle(0, WS_CLIPCHILDREN);	
		CXSpinBox::DeleteInstance();
		CXSpinBox *pSpinBox = CXSpinBox::GetInstance();
		if( pSpinBox )
		{
			CRect rc;
			GetClientRect(&rc);
			rc.left = rc.right - 16;
			pSpinBox->Create(UDS_WRAP | UDS_SETBUDDYINT | UDS_AUTOBUDDY | UDS_NOTHOUSANDS | 
				WS_CHILD | WS_VISIBLE, rc, this, NULL);
			pSpinBox->SetRange(0, 1);
		}
	}

	return 0;
}

void CXEditBox::OnPaint()
{
	Default();

	if( m_nMinNum >=0 && m_nMaxNum >=0 &&
		CXSpinBox::m_pXSpinBox )
	{
		CXSpinBox::m_pXSpinBox->RedrawWindow();
	}
}
