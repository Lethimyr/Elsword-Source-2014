// Picture.cpp: implementation of the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Picture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define HIMETRIC_INCH	2540

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPicture::CPicture()
{
	m_pIPicture = NULL;
}

CPicture::~CPicture()
{
	if (m_pIPicture)
		m_pIPicture->Release();
}

BOOL CPicture::Load(LPCTSTR pszFileName)
{
	TRY
	{
		CFile File(pszFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
		DWORD dwFileSize = File.GetLength();
		LPVOID pData = NULL;
		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
		pData = ::GlobalLock(hGlobal);
		DWORD dwBytesRead;
		dwBytesRead = File.Read(pData, dwFileSize);
		::GlobalUnlock(hGlobal);
		File.Close();
		
		LPSTREAM pstm = NULL;
		HRESULT hr = ::CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
		if (FAILED(hr))
			return FALSE;

		if (m_pIPicture)
		{
			m_pIPicture->Release();
			m_pIPicture = NULL;
		}
		hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID*)&m_pIPicture);
		pstm->Release();
		if (FAILED(hr))
			return FALSE;
	}
	CATCH_ALL (e)
	{
		e->ReportError();
	}
	END_CATCH_ALL

	return TRUE;
}

void CPicture::Draw(CDC *pDC, int x, int y)
{
	if (!m_pIPicture)
		return;

	// get width and height of picture
	long hmWidth;
	long hmHeight;
	m_pIPicture->get_Width(&hmWidth);
	m_pIPicture->get_Height(&hmHeight);
	// convert himetric to pixels
	int nWidth	= ::MulDiv(hmWidth, ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX), HIMETRIC_INCH);
	int nHeight	= ::MulDiv(hmHeight, ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), HIMETRIC_INCH);
	// display picture using IPicture::Render
	CRect rc;
	pDC->GetWindow()->GetClientRect(&rc);
	m_pIPicture->Render(pDC->m_hDC, x, y, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, &rc);
}

CSize CPicture::GetSize()
{
	CSize Size;
	Size.cx	= ::MulDiv(hmWidth, ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSX), HIMETRIC_INCH);
	Size.cy	= ::MulDiv(hmHeight, ::GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), HIMETRIC_INCH);

	return Size;
}
