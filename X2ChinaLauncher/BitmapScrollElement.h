#pragma once

#include "BtnSt.h"
//#include "ServerFeelDlg.h"

#define IDC_SERVERELEMENTBUTTON_BEGIN 555

class CBitmapScrollElement: public CButtonST
{
public:
	CBitmapScrollElement();
	~CBitmapScrollElement();

	//virtual BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, 	const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//void SetID(UINT uiID) { m_uiID = uiID; }
	//void UpdateFeelType();
	void PrepareServerFeel(const BOOL bHot, const CPoint& ptHotOffset, const CSize& ptHotSize, 
								const BOOL bNew, const CPoint& ptNewOffset, const CSize& ptNewSize,
									const BOOL bOpen, const CPoint& ptOpenOffset, const CSize& ptOpenSize );

	void SetOwnerBackground(CDC* pDC) { m_pOwnerBackgroundDC = pDC; }

	DECLARE_MESSAGE_MAP()

protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnPaint();
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	void RestoreBackground(CDC* pParentDC);

	//UINT	m_uiID;
	int m_PreOffsetY;
	//int m_CurOffsetY;

	CDC* m_pOwnerBackgroundDC;
	//CServerFeelDlg m_ServerFeelDlg;

	//CButtonST m_ServerFeel;

	//hot
	CDC m_dcHot;
	CBitmap m_bmpHot;
	CPoint m_ptHotbmpPos;
	CSize m_szHotBmpSize;
	BOOL m_bHotBmp;

	//new
	CDC m_dcNew;
	CBitmap m_bmpNew;
	CPoint m_ptNewbmpPos;
	CSize m_szNewBmpSize;
	BOOL m_bNewBmp;

	//open
	CDC m_dcOpen;
	CBitmap m_bmpOpen;
	CPoint m_ptOpenbmpPos;
	CSize m_szOpenBmpSize;
	BOOL m_bOpenBmp;
};