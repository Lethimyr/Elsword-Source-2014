#pragma once

#include "BtnSt.h"
//#include "ServerFeelDlg.h"

class CServerList;

class CBitmapScrollBar: public CButtonST
{
public:
	CBitmapScrollBar();
	~CBitmapScrollBar();

	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void SetManager(CServerList* pManager) { m_pManager = pManager; }
	void SetOwnerBackground(CDC* pDC) { m_pOwnerBackgroundDC = pDC; }
	//void SetRegion(const RECT& rc) { m_Region = rc; }

	int GetBarMoveY() { return m_iBarMoveY; }

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnPaint();

	//LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//void OnWindowPosChanged(WINDOWPOS* lpwndpos);

private:
	void RestoreBackground(CDC* pParentDC);

	CServerList* m_pManager;
	BOOL m_bIsBarMove;
	int m_iBarStartY;
	int m_iBarMoveY;
	//RECT m_Region;
	///BOOL bNeedUpdate;

	CRect m_PreRect;

	CDC* m_pOwnerBackgroundDC;
	//CServerFeelDlg m_ServerFeelDlg;

};