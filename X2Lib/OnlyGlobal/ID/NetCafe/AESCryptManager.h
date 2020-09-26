// AESCryptManager.h: interface for the CAESCryptManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AESCRYPTMANAGER_H__E6B60ED2_E5F6_4477_B214_0EFEBD0B2D86__INCLUDED_)
#define AFX_AESCRYPTMANAGER_H__E6B60ED2_E5F6_4477_B214_0EFEBD0B2D86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "pmDesEx.h"

class CAESCryptManager  
{
public:
	LONG GetCryptInfo(LPCTSTR pszSource, LPTSTR pRetBuffer, DWORD dwSize, BOOL bUsePadding = FALSE);
	CAESCryptManager();
	virtual ~CAESCryptManager();

};

#endif // !defined(AFX_AESCRYPTMANAGER_H__E6B60ED2_E5F6_4477_B214_0EFEBD0B2D86__INCLUDED_)
