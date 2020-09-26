#include "stdafx.h"
#include "KTDXStringLoader.h"
#include <atlstr.h>
#include <locale>

CKTDXStringLoader::CKTDXStringLoader( void )
{
}

CKTDXStringLoader::~CKTDXStringLoader( void )
{
    m_vecString.clear();
}

const std::wstring& CKTDXStringLoader::GetString( int ID )
{
    //if( (int)m_vecString.size() <= ID || ID < 0 )
	//	return m_vecString[STR_ID_BLANK];

    return m_vecString[ID];
}

//===============================================================================
// std::string KGCStringLoader::GetReplacedString(int ID, char* szParamList, ...)
// 의 szParamList가 나타내는것
//
//i			숫자로 바꿔줌
//I			ID의 문자열로 교체해줌
//s or S	char* 문자열로 교체해줌
//l or L	std::string 문자열로 교체해줌

//  1672 = "캐릭터 @1 획득에 실패하였습니다"
//  str1700 = "라스"

//  GetReplacedString( 1672 , "s" , "라스" ); -->   캐릭터 라스 획득에 실패하였습니다
//  GetReplacedString( 1672 , "I" , 1700 );   -->   캐릭터 라스 획득에 실패하였습니다

//  1641 = "@1차직업 @2단필살기"
//  GetReplacedString( 1641 , "ii" , 1, 1 );   -->  1차직업 1단필살기

//===============================================================================
std::wstring CKTDXStringLoader::GetReplacedString(int ID, char* szParamList, ...)
{
	//if( (int)m_vecString.size() <= ID || ID < 0 )
	//	return m_vecString[STR_ID_BLANK];

	std::wstring original = m_vecString[ID];

	CString strTemp = original.c_str();
	CString strReplaceTarget;
	CString strReplace;

	int iParamSize = strlen(szParamList);

	va_list marker;
	va_start( marker, szParamList );

	for(int i = 0; i < iParamSize; i++)
	{
		strReplaceTarget.Format( L"@%d", i+1 );
		// @i를 교체합니다
		switch(szParamList[i])
		{
		case 'i':			//숫자로 바꿔줌
			{			
				int iParam = va_arg( marker, int);
				strReplace.Format( L"%d", iParam );
			}
			break;
		case 'I':			//ID의 문자열로 교체해줌
			{
				int iParam = va_arg( marker, int);
				std::wstring str = GetString( iParam );
				strReplace = str.c_str();
			}
			break;			
		case 's':			//char* 문자열로 교체해줌
		case 'S':
			{
				WCHAR* str = va_arg( marker, WCHAR*);
				strReplace = str;
			}
			break;
		case 'l':			//std::string 문자열로 교체해줌
		case 'L':
			{
				std::wstring str = va_arg( marker, std::wstring);
				strReplace = str.c_str();
			}
			break;
		}

		strTemp.Replace( strReplaceTarget, strReplace );
	}
	va_end( marker );

	std::wstring strret = strTemp;

	return strret;
}

bool CKTDXStringLoader::Load( const WCHAR* pFileName )
{
	//m_vecString.reserve(MAX_NUM_STRING);

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pFileName );
	if( Info == NULL )
	{
		//스트링 테이블 로드 실패
		ErrorLogMsg( KEM_ERROR127, pFileName );
		return false;
	}

	/*
	CString cStr;
	for( int i = 0 ; i < MAX_NUM_STRING ; i++ )
	{
		memset( strTemp, 0, sizeof(strTemp) );
		cStr = _getws( Info->pRealData );
		cStr = &strTemp[cStr.Find(L'\t') + 1];

		WCHAR* tok;
		tok = wcstok( (LPTSTR)(LPCTSTR)cStr, L"\"\r" );

		cStr = tok;
		cStr.Replace( L"\\n", L"\r\n" );

		m_vecString.push_back((LPTSTR)(LPCTSTR)cStr);
	}
	*/

    return true;
}
