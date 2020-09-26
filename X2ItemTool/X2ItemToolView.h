// X2ItemToolView.h : iCX2ItemToolView 클래스의 인터페이스
//


#pragma once

#include "MainDlg.h"


class CX2ItemToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CX2ItemToolView();
	DECLARE_DYNCREATE(CX2ItemToolView)

// 특성
public:
	CX2ItemToolDoc* GetDocument() const;

// 작업
public:

// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현
public:
	virtual ~CX2ItemToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);




protected:
	CMainDlg		m_MainDlg;

public:
	afx_msg void OnFileOpen();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // X2ItemToolView.cpp의 디버그 버전
inline CX2ItemToolDoc* CX2ItemToolView::GetDocument() const
   { return reinterpret_cast<CX2ItemToolDoc*>(m_pDocument); }
#endif

