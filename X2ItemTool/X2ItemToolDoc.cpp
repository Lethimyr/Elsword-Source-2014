// X2ItemToolDoc.cpp : CX2ItemToolDoc 클래스의 구현
//

#include "stdafx.h"
#include "X2ItemTool.h"

#include "X2ItemToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2ItemToolDoc

IMPLEMENT_DYNCREATE(CX2ItemToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CX2ItemToolDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CX2ItemToolDoc, CDocument)
END_DISPATCH_MAP()

// 참고: VBA에서의 형식 안전 바인딩을 지원하기 위해 IID_IX2ItemTool에 대한 지원을 추가했습니다.
// 이 IID는 .IDL 파일의 dispinterface에 첨부된 
// GUID와 일치해야 합니다.

// {551A743D-ABB1-4F0C-88D1-0E24EC0CAD00}
static const IID IID_IX2ItemTool =
{ 0x551A743D, 0xABB1, 0x4F0C, { 0x88, 0xD1, 0xE, 0x24, 0xEC, 0xC, 0xAD, 0x0 } };

BEGIN_INTERFACE_MAP(CX2ItemToolDoc, CDocument)
	INTERFACE_PART(CX2ItemToolDoc, IID_IX2ItemTool, Dispatch)
END_INTERFACE_MAP()


// CX2ItemToolDoc 생성/소멸

CX2ItemToolDoc::CX2ItemToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

	EnableAutomation();

	AfxOleLockApp();
}

CX2ItemToolDoc::~CX2ItemToolDoc()
{
	AfxOleUnlockApp();
}

BOOL CX2ItemToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CX2ItemToolDoc serialization

void CX2ItemToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CX2ItemToolDoc 진단

#ifdef _DEBUG
void CX2ItemToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CX2ItemToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CX2ItemToolDoc 명령
