// X2ItemToolDoc.h : CX2ItemToolDoc 클래스의 인터페이스
//


#pragma once

class CX2ItemToolDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CX2ItemToolDoc();
	DECLARE_DYNCREATE(CX2ItemToolDoc)

// 특성
public:

// 작업
public:

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CX2ItemToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()

	// OLE 디스패치 맵 함수를 생성했습니다.

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


