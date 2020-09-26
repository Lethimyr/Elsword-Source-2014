// Picture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__EE67E130_17C9_4CDE_999E_F8858E00BA94__INCLUDED_)
#define AFX_PICTURE_H__EE67E130_17C9_4CDE_999E_F8858E00BA94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPicture  
{
public:
	CPicture();
	virtual ~CPicture();

// Attributes
protected:
	LPPICTURE m_pIPicture;

// Operations
public:
	BOOL Load(LPCTSTR pszFileName);
	void Draw(CDC *pDC, int x, int y);
	CSize GetSize();
};

#endif // !defined(AFX_PICTURE_H__EE67E130_17C9_4CDE_999E_F8858E00BA94__INCLUDED_)
