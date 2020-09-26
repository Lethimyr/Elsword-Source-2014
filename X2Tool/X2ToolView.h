
// X2ToolView.h : CX2ToolView 클래스의 인터페이스
//

#pragma once

#include "LineSector.h"

class CX2ToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CX2ToolView();
	DECLARE_DYNCREATE(CX2ToolView)

// 특성입니다.
public:
	CX2ToolDoc* GetDocument() const;

public:
	CLineSector m_DlgLineSector;

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate( );

	virtual int PreTranslateMessage(MSG* pMsg);
// 구현입니다.
public:
	virtual ~CX2ToolView();

	void ResetDungeon();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnRender();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNewLine();	
	afx_msg void OnLoadLineMap();
	afx_msg void OnLineSave();
	afx_msg void OnLineSaveas();
	afx_msg void OnOpenWorld();
	afx_msg void OnLineExport();
	afx_msg void OnWorldSave();
	afx_msg void OnWorldSaveas();
	afx_msg void OnNewWorld();
	afx_msg void OnLoadHeightmap();
	afx_msg void OnShowObject();
	afx_msg void OnFreezing();
	afx_msg void OnViewSelected();
	afx_msg void OnViewTopfront();
	afx_msg void OnSector();
	afx_msg void OnNewDungeon();
	afx_msg void OnLoadDungeon();
	afx_msg void OnSaveasDungeon();
};

#ifndef _DEBUG  // X2ToolView.cpp의 디버그 버전
inline CX2ToolDoc* CX2ToolView::GetDocument() const
   { return reinterpret_cast<CX2ToolDoc*>(m_pDocument); }
#endif

