#pragma once

#include <vector>
//#include "ServerElementButton.h"

class CWnd;
class CBitmapButton;
class CButtonST;
class CBitmapScrollBarView;
class CBitmapScrollBar;
class CBitmapScrollElementView;

class CServerList
{
public:
	enum ECommunicationCompanyType
	{
		eServer_CommunicationCompany_Type_Telecom = 0,
		eServer_CommunicationCompanyType_CNC
	};

	CServerList();
	~CServerList();

	void Init(const CPoint& ScrollBarViewPos, const CPoint& ScrollElementViewPos, 
				const UINT uiScrollBarViewID, const UINT uiScrollBarID, const UINT uiScrollElemViewID, 
				const UINT uiScrollViewResourceID) 
	{ 
		m_ScrollBarViewPos = ScrollBarViewPos; 
		m_ScrollElementViewPos = ScrollElementViewPos;
		m_uiScrollBarViewID = uiScrollBarViewID; 
		m_uiScrollBarID  = uiScrollBarID;
		m_uiScrollElemViewID = uiScrollElemViewID; 
		m_uiScrollViewResourceID = uiScrollViewResourceID;
	}
	void AddElement(CWnd* pParentWnd, const std::wstring& wstrName, const BOOL bHot, const BOOL bNew, const BOOL bOpen, const BOOL bEnable);
	void NotifyElementSelected(const std::wstring& wstrCurElementName, const std::wstring& wstrPreElementName);
	CBitmapScrollBar* GetScrollbar() { return m_pSlideBarButton; }
	CBitmapScrollElementView* GetScrollView() { return m_pSlideView; }
	int CalcElementButtonMove(int iSlideBarMoveY);
	
	BOOL IsEnableScrollBar();
private:
	

	//CButtonST* m_pSlideBarButton;
	//CBitmapButton* m_pSlideBG;

	CPoint m_ScrollBarViewPos;
	CPoint m_ScrollElementViewPos;
	UINT m_uiScrollBarViewID;
	UINT m_uiScrollBarID;
	UINT m_uiScrollElemViewID;

	UINT m_uiScrollViewResourceID;

	CBitmapScrollBarView* m_pSlideBarView;
	CBitmapScrollBar* m_pSlideBarButton;
	CBitmapScrollElementView* m_pSlideView;
	//std::vector<CButtonST*>  m_vecElement;
	//std::vector<CServerElementButton*>  m_vecElement;

	int iElementButton;
	//RECT m_ElementRegion;
	//RECT m_ScrollRegion;

	//RECT m_CurElementRegion;
};