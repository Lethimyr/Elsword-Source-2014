#include "stdafx.h"

#include <KncUtil.h>
#include "KTDXStringTable.h"

ImplementSingleton( CKTDXStringTable );

CKTDXStringTable::CKTDXStringTable()
{
    m_vecGeneralStringTable.clear();
    m_wstrEmpty.clear();
#ifdef NETERROR_KOREAN_HELP
	m_bEnableDebugInfo = false;
#endif NETERROR_KOREAN_HELP
}

CKTDXStringTable::~CKTDXStringTable()
{
}

bool CKTDXStringTable::InitializeStringTable( std::wstring& wstrGeneralStringFileName, std::wstring& wstrScriptStringFileName )
{
    return LoadStringTable( wstrGeneralStringFileName, m_vecGeneralStringTable );
}

std::wstring CKTDXStringTable::GetReplacedString( int iID, char* szParamTypeList, ... )
{
    int iTableSize = ( int )m_vecGeneralStringTable.size();

    ASSERT( iID >= 0 && iID < iTableSize );

    if( iID < 0 || iID >= iTableSize )
    {
        return m_wstrEmpty;
    }

    std::wstring wstrReplacedString;
#ifdef NETERROR_KOREAN_HELP
	if(m_bEnableDebugInfo)
		wstrReplacedString = boost::str( boost::wformat( L"[S%1%]%2%" ) % iID % m_vecGeneralStringTable[iID] );
	else
#endif NETERROR_KOREAN_HELP
		wstrReplacedString = m_vecGeneralStringTable[iID];

    int iParamTypeListSize = strlen( szParamTypeList );

    va_list marker;
    va_start( marker, szParamTypeList );

    for( int i = 0; i < iParamTypeListSize; i++ )
    {
        std::wstringstream wstmReplaceSymbol;
        wstmReplaceSymbol << L"@" << ( i + 1 );

        std::wstringstream wstmReplaceValue;

        // 대입될 값을 스트링으로 만든다.
        switch( szParamTypeList[i] )
        {
        case 'h':       // 부호를 표현하는 정수로 변환
            {
                int iParam = va_arg( marker, int );
                if( iParam > 0 )
                {
                    wstmReplaceValue << L"+";
                }
                wstmReplaceValue << iParam;
            }
            break;
        case 'i':       // 정수로 변환
            {			
                int iParam = va_arg( marker, int );
                wstmReplaceValue << iParam;
            }
            break;
        case 'e':       // 부호를 표현하는 실수로 변환
            {
                // 소수점 뒤 둘째자리까지만 표현하고, 끝이 0이면 0을 자른다. 1.50 -> 1.5
                double dParam = va_arg( marker, double );
                wchar_t szNumber[64];
                StringCchPrintfW( szNumber, ARRAY_SIZE( szNumber ), L"%.2f", dParam );
                dParam = _wtof( szNumber );
                if( dParam > 0.0 )
                {
                    wstmReplaceValue << L"+";
                }
                wstmReplaceValue << dParam;
            }
            break;
        case 'f':       // 실수로 변환
            {
                // 소수점 뒤 둘째자리까지만 표현하고, 끝이 0이면 0을 자른다. 1.50 -> 1.5
                double dParam = va_arg( marker, double );
                wchar_t szNumber[64];
                StringCchPrintfW( szNumber, ARRAY_SIZE( szNumber ), L"%.2f", dParam );
                dParam = _wtof( szNumber );
                wstmReplaceValue << dParam;
            }
            break;
        case 'I':       // ID의 문자열로 변환
            {
                int iParam = va_arg( marker, int );
                wstmReplaceValue << GetGeneralString( iParam ); 
            }
            break;			
        case 's':       // 문자열로 변환
            {
                std::string strValue = va_arg( marker, char* );
                wstmReplaceValue << KncUtil::toWideString( strValue );
            }
            break;
        case 'S':
            {
                wstmReplaceValue << va_arg( marker, wchar_t* );
            }
            break;
        case 'l':       // std::string 으로 변환
            {
                std::string strValue = va_arg( marker, std::string );
                wstmReplaceValue << KncUtil::toWideString( strValue );
            }
            break;
        case 'L':       // std::wstring 으로 변환
            {
                wstmReplaceValue << va_arg( marker, std::wstring );
            }
            break;
        case 'M':       // 영어권 국가의 달력 표현
        case 'm':
            {
            }
            break;
        case 'o':       // 영어권 국가의 서수 표현
        case 'O':
            {
            }
            break;
        case 'n':       // 영어권 국가의 서수 표현
        case 'N':
            {
            }
            break;
        }

        std::wstring wstrReplaceSymbol = wstmReplaceSymbol.str().c_str();
        std::wstring wstrReplaceValue = wstmReplaceValue.str().c_str();

        size_t iFindPos = 0;
		size_t iCurrentPos = 0;
        while( ( iFindPos = wstrReplacedString.find( wstrReplaceSymbol, iCurrentPos ) ) != std::wstring::npos )
        {
            wstrReplacedString.replace( iFindPos, wstrReplaceSymbol.size(), wstrReplaceValue );
			iCurrentPos = iFindPos + wstrReplaceValue.length();
        }
    }
    va_end( marker );

    return wstrReplacedString;
}

const std::wstring& CKTDXStringTable::GetGeneralString( int iID )
{
    int iTableSize = ( int )m_vecGeneralStringTable.size();

    // STR_ID_EMPTY 때문에 -1이 넘어올 수도 있다.
    ASSERT( iID >= -1 && iID < iTableSize );

    if( iID < 0 || iID >= iTableSize )
    {
        return m_wstrEmpty;
    }

    return m_vecGeneralStringTable[iID];
}


const std::wstring& CKTDXStringTable::GetScriptString( int iID )
{
	int iTableSize = ( int )m_vecScriptStringTable.size();

    ASSERT( iID >= 0 && iID < iTableSize );

    if( iID < 0 || iID >= iTableSize )
    {
        return m_wstrEmpty;
    }

    return m_vecScriptStringTable[iID];

}

bool CKTDXStringTable::LoadStringTable( std::wstring& wstrFileName, std::vector< std::wstring >& vecStringTable )
{
    vecStringTable.clear();

    // 유니코드 파일을 읽으므로 바이너리 모드로 오픈한다.
	FILE* fp = NULL;
#ifdef	CONVERSION_VS
	_wfopen_s( &fp, wstrFileName.c_str(), L"rb" );
#else	CONVERSION_VS
	fp = _wfopen( wstrFileName.c_str(), L"rb" );
#endif	CONVERSION_VS

    ASSERT( fp );
    if( !fp )
    {
        return false;
    }

    // UTF-16LE의 BOM( Byte Order Mark )을 뛰어 넘는다.
    int iRetFSeek = fseek( fp, 2, SEEK_CUR );
    ASSERT( iRetFSeek == 0 );
    if( iRetFSeek != 0 )
    {
        return false;
    }

    // string table 에서 한 줄을 읽으면 다음과 같은 모양이다.
    // [index]\t[string]\r\n
    const int ciMaxStringLength = 2048;
    wchar_t wszStringRead[ciMaxStringLength];
    int iIndex = 0;
    while( fgetws( wszStringRead, ciMaxStringLength, fp ) )
    {
        std::wstring wstrStringRead( wszStringRead );
        // 최대 스트링 길이만큼 읽었다면 문제가 있는지 의심해봐야 한다.
        ASSERT( ( int )wstrStringRead.size() < ( ciMaxStringLength - 1 ) );

		std::wstring wstrFullString;
		
		//09. 07. 03 김정협  XOR암호화 한 파일 복호화
		size_t iEnd = wstrStringRead.find( L"\r\n" );
		if( iEnd != std::wstring::npos )
		{
			wstrStringRead.erase( iEnd );
		}		
		
		short iKey = 16;
		int nSize = wstrStringRead.length();		
		for ( int i = 0; i < nSize; i++ ) 
		{               
			wstrStringRead[i] = wstrStringRead[i] ^ iKey;
		} 

        // tab 을 읽어서 index를 구분해낸다.
        size_t iTabPos = wstrStringRead.find( L"\t" );
        std::wstring wstrIndex = wstrStringRead.substr( 0, iTabPos );
        int iIndexRead = _wtoi( wstrIndex.c_str() );

        std::wstring wstrContent;
        // 파일의 index가 제대로 설정되어 있는지 검사한다.
        ASSERT( iIndexRead == iIndex );
        if( iIndexRead == iIndex )
        {
            // tab 을 찾은 경우에만 그 뒤의 내용을 읽어야 한다.
            if( iTabPos != std::wstring::npos )
            {
				//09. 07. 03 김정협  미리 없애기 때문에 막음
                /*size_t iEnd = wstrStringRead.find( L"\r\n" );
                if( iEnd != std::wstring::npos )
                {
                    wstrStringRead.erase( iEnd );
                }*/

                ASSERT( iTabPos < wstrStringRead.size() );
                // tab 뒤에 있는 것들만 읽는다.
                if( ( iTabPos + 1 ) < wstrStringRead.size() )
                {
                    wstrContent = wstrStringRead.substr( iTabPos + 1 );
                }
            }
        }

        // \n 으로 표시한 부분에 실제로 줄바꿈 해준다.
        REPLACE_PART_OF_STRING( wstrContent, L"\\n", L"\r\n" );

        // 작은 따옴표가 있는 경우
        //REPLACE_PART_OF_STRING( wstrContent, L"\\\'", L"\'" );

        // 큰 따옴표가 있는 경우
        //REPLACE_PART_OF_STRING( wstrContent, L"\\\"", L"\"" );

        vecStringTable.push_back( wstrContent );
        iIndex++;
    }

    int iRetFClose = fclose( fp );
    ASSERT( iRetFClose == 0 );

    return true;
}