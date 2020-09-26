// X2EncryptFileViewerDoc.cpp : CX2EncryptFileViewerDoc 클래스의 구현
//

#include "stdafx.h"
#include "X2EncryptFileViewer.h"

#include "X2EncryptFileViewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CX2EncryptFileViewerDoc

IMPLEMENT_DYNCREATE(CX2EncryptFileViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CX2EncryptFileViewerDoc, CDocument)
END_MESSAGE_MAP()


// CX2EncryptFileViewerDoc 생성/소멸

CX2EncryptFileViewerDoc::CX2EncryptFileViewerDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CX2EncryptFileViewerDoc::~CX2EncryptFileViewerDoc()
{
}

BOOL CX2EncryptFileViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CX2EncryptFileViewerDoc serialization

void CX2EncryptFileViewerDoc::Serialize(CArchive& ar)
{
	// CEditView에는 모든 serialization을 처리하는 edit 컨트롤이 들어 있습니다.
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SerializeRaw(ar);
}


// CX2EncryptFileViewerDoc 진단

#ifdef _DEBUG
void CX2EncryptFileViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CX2EncryptFileViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CX2EncryptFileViewerDoc 명령
