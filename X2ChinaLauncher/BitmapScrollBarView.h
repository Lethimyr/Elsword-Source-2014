#pragma once

#include <vector>
#include "BtnSt.h"
//#include "ServerFeelDlg.h"

class CServerList;
class CBitmapScrollBar;

class CBitmapScrollBarView: public CBitmapButton
{
public:
	CBitmapScrollBarView();
	~CBitmapScrollBarView();

	void Create(int x, int y, CWnd* pParentWnd, UINT uiCtrlID, UINT uiResourceID);
	void SetManager(CServerList* pManager) { m_pManager = pManager; }
	void Attatch(CBitmapScrollBar* pElement);


	CDC* GetDCBackground() { return &m_dcBG; }

	//CServerFeelDlg* GetServerFeelDlg() { return &m_ServerFeelDlg; }

	//virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()

protected:

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnPaint();

private:
	CServerList* m_pManager;

	CBitmapScrollBar*  m_pScrollbar;


	CDC m_dcBG;
	CBitmap m_bmpBG;
	CBitmap* m_pbmpOldBG;
};