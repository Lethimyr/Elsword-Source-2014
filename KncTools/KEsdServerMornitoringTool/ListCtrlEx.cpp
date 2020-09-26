#include "stdafx.h"
#include <assert.h>
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx construction/destruction

CListCtrlEx::CListCtrlEx()
{
	m_clrText = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBkgnd = ::GetSysColor(COLOR_WINDOW);
	m_clrHText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_clrHBkgnd = ::GetSysColor(COLOR_HIGHLIGHT);	
}

CListCtrlEx::~CListCtrlEx()
{
}

void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

    switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	   *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
 	    *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		{
			int iCol = lplvcd->iSubItem;
			int iRow = static_cast<int>(lplvcd->nmcd.dwItemSpec);
			CString sItem = GetItemText(iRow, iCol);
			CRect rcItem;
			GetCellRect(iRow, iCol, LVIR_BOUNDS, rcItem);
			
			CDC *pDC= CDC::FromHandle(lplvcd->nmcd.hdc);
			int State = ListView_GetItemState(m_hWnd, iRow, LVIS_CUT|LVIS_SELECTED|LVIS_FOCUSED);

			pDC->FillSolidRect(&rcItem, (State &LVIS_SELECTED) ? m_clrHBkgnd : m_clrBkgnd);
			rcItem.DeflateRect(2, 1);
			if(sItem.Right(1) == '%')
			{
				pDC->Rectangle(rcItem);
				rcItem.DeflateRect(1, 1);
				CRect rcLeft, rcRight;
				rcLeft = rcRight = rcItem;
				rcLeft.right = rcLeft.left + rcItem.Width();
				rcRight.left = rcLeft.right;
				
				pDC->FillSolidRect(rcLeft, static_cast<COLORREF>( _wtoi( sItem ) ) );
				CRgn rgn;
				rgn.CreateRectRgnIndirect(rcLeft);
				pDC->SelectClipRgn(&rgn);
				pDC->SetTextColor(m_clrBkgnd);				

				rgn.SetRectRgn(rcRight);
				pDC->SelectClipRgn(&rgn);
				pDC->SetTextColor(m_clrText);
				pDC->SelectClipRgn(NULL);

				*pResult= CDRF_SKIPDEFAULT;
			}
			else
			{
				pDC->SetTextColor((State & LVIS_SELECTED) ? m_clrHText:m_clrText);
				pDC->DrawText(sItem, -1, &rcItem, DT_NOCLIP | DT_LEFT | DT_VCENTER);

				*pResult= CDRF_NOTIFYSUBITEMDRAW;
			}
		}

		break;
	}
}

BOOL CListCtrlEx::GetCellRect(int iRow, int iCol, int nArea, CRect &rect)
{
	if(iCol)
		return GetSubItemRect(iRow, iCol, nArea, rect);
	
	if(GetHeaderCtrl()->GetItemCount() == 1)
		return GetItemRect(iRow, rect, nArea);
	
	CRect rCol1;
	if(!GetSubItemRect(iRow, 1, nArea, rCol1))
		return FALSE;
	
	if(!GetItemRect(iRow, rect, nArea))
		return FALSE;
	
	rect.right = rCol1.left;
	
	return TRUE;
}
/*
void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 16;
}

void CListCtrlEx::Init()
{
	// WM_WINDOWPOSCHANGED를 일부러 발생시켜서 부모 윈도우가
	// WM_MEASUREITEM 메세지를 보내도록 한다.
	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
}
*/