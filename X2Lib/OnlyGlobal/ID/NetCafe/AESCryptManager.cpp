// AESCryptManager.cpp: implementation of the CAESCryptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AESCryptManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString GetEncKeyValue()
{
	CString strReturn = _T("");
	const TCHAR lpszENC[][16] = { _T("4"), _T("B"), _T("0"), _T("1"), _T("1"), _T("F"), _T("4"), _T("0"), _T("8"), _T("F"), _T("C"), _T("C"), _T("0"), _T("1"), _T("D"), _T("2") };
	strReturn.Format(_T("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s"), lpszENC[0],lpszENC[1],lpszENC[2],lpszENC[3],lpszENC[4],lpszENC[5],lpszENC[6],lpszENC[7],lpszENC[8],lpszENC[9],lpszENC[10],lpszENC[11],lpszENC[12],lpszENC[13],lpszENC[14],lpszENC[15]);
	return strReturn;
}

CString GetchKeyEncKeyValue()
{	
	CString strReturn = _T("");
	const TCHAR lpszENC[][16] = {_T("!"),_T("@"),_T("#"),_T("$"),_T("%"),_T("^"),_T("&"), _T("*"),_T("("),_T(")"),_T("!"),_T("@"),_T("#"),_T("$"), _T("%"),_T("^")};
	strReturn.Format(_T("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s"), lpszENC[0],lpszENC[1],lpszENC[2],lpszENC[3],lpszENC[4],lpszENC[5],lpszENC[6],lpszENC[7],lpszENC[8],lpszENC[9],lpszENC[10],lpszENC[11],lpszENC[12],lpszENC[13],lpszENC[14],lpszENC[15]);
	return strReturn;
}

CAESCryptManager::CAESCryptManager()
{

}

CAESCryptManager::~CAESCryptManager()
{

}

LONG CAESCryptManager::GetCryptInfo(LPCTSTR pszSource, LPTSTR pRetBuffer, DWORD dwSize, BOOL bUsePadding)
{
	LONG lReturn = 0;

	try
	{		
		if(pszSource == NULL)
		{
			if(pRetBuffer != NULL && dwSize > 0)
			{
				pRetBuffer[0] = '\0';
			}
			return 0;
		}


		//1
		///*
		USES_CONVERSION;
		char* chData = W2A(pszSource);
		
		CAESCryptAPI CryptAPI( (TCHAR*)(LPCTSTR)GetEncKeyValue(),  (TCHAR*)(LPCTSTR)GetchKeyEncKeyValue(), bUsePadding);
		CryptAPI.DecryptAPI(chData);
		
		CString strDecData;
		strDecData.Format(L"%s",  A2W(CryptAPI.GetCryptString()));
		//*/

		//2
		/*
		int nLen = WideCharToMultiByte(CP_ACP, 0, pszSource, -1, NULL, 0, NULL, NULL); 
		char* chData = new char[nLen];
		WideCharToMultiByte(CP_ACP, 0, pszSource, -1, chData, nLen, 0,0);   

		CAESCryptAPI CryptAPI( (TCHAR*)(LPCTSTR)GetEncKeyValue(),  (TCHAR*)(LPCTSTR)GetchKeyEncKeyValue(), bUsePadding);
		CryptAPI.DecryptAPI(chData);		

		LPWSTR wData = new WCHAR[255];
		nLen = MultiByteToWideChar(CP_ACP, 0,CryptAPI.GetCryptString(), -1, NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, CryptAPI.GetCryptString(), -1, wData, nLen);

		CString strDecData;
		strDecData.Format(L"%s",wData);
		delete[] wData;
		*/
		
		
		lReturn = strDecData.GetLength();
		if(pRetBuffer == NULL)
		{
			return 0;
		}
		else
		{
			if(dwSize <= lReturn)
			{
				SetLastError(ERROR_INSUFFICIENT_BUFFER);
				return 0;
			}
			_tcscpy(pRetBuffer, strDecData);
		}
	}
	catch(exception& e)
	{	
		if(strcmp(e.what(), "not enough memory") == 0)
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		else
			SetLastError(ERROR_INVALID_PARAMETER);

		return 0;
	}

	return lReturn;	
}
