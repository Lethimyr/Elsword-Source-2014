// X2EncryptFileViewerView.h : iCX2EncryptFileViewerView 클래스의 인터페이스
//


#pragma once


class CX2EncryptFileViewerView : public CEditView
{
protected: // serialization에서만 만들어집니다.
	CX2EncryptFileViewerView();
	DECLARE_DYNCREATE(CX2EncryptFileViewerView)

// 특성
public:
	CX2EncryptFileViewerDoc* GetDocument() const;

// 작업
public:

// 재정의
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDropFiles( HDROP hDropInfo );

	bool BrowseFolder( HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR* szFolder );

// 구현
public:
	virtual ~CX2EncryptFileViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	char* m_cBufPointer;

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDecryptSave();
};

#ifndef _DEBUG  // X2EncryptFileViewerView.cpp의 디버그 버전
inline CX2EncryptFileViewerDoc* CX2EncryptFileViewerView::GetDocument() const
   { return reinterpret_cast<CX2EncryptFileViewerDoc*>(m_pDocument); }
#endif

