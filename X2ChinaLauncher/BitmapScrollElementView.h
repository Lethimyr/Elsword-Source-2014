#pragma once

#include <vector>
#include "BtnSt.h"
//#include "ServerFeelDlg.h"

class CServerList;
class CBitmapScrollElement;

class CBitmapScrollElementView: public CBitmapButton
{
public:
/*	enum EServerFeelType
	{
		eServer_Feel_Type_None = 0,
		eServer_Feel_Type_HOT,
		eServer_Feel_Type_NEW
	};*/

	CBitmapScrollElementView();
	~CBitmapScrollElementView();

	void Create(int x, int y, CWnd* pParentWnd, UINT uiCtrlID, UINT uiResourceID);
	void SetManager(CServerList* pManager) { m_pManager = pManager; }
	void AddElement(CBitmapScrollElement* pElement/*, EServerFeelType eFeelType*/);

	//CServerFeelDlg* GetServerFeelDlg() { return &m_ServerFeelDlg; }

	void NotifyScroll(int iOffsetY, BOOL bUp);
	void NotifyElementSelected(const std::wstring& wstrCurElementName, const std::wstring& wstrPreElementName);

	CRect GetAllElementRect() { return m_AllElementRect; }

	CDC* GetDCBackground() { return &m_dcBG; }

	//virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()

protected:

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnPaint();

private:
	CServerList* m_pManager;

	std::vector<CBitmapScrollElement*>  m_vecElement;
	//std::vector<EServerFeelType> m_vecServerFeelType;

	CRect m_AllElementRect;

	CDC m_dcBG;
	CBitmap m_bmpBG;
	CBitmap* m_pbmpOldBG;

	//CServerFeelDlg m_ServerFeelDlg;
	//CButton m_TestButton;
};