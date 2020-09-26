#include "StdAfx.h"
#include ".\x2stringfilter.h"

CX2StringFilter::CX2StringFilter()
{
	m_BanNickNameList.reserve( 512 );
	m_BanWordList.reserve( 2048 );
#ifdef SERV_POST_BAN_WORD_FILTER
	m_BanPostWordList.reserve( 512 );
#endif //SERV_POST_BAN_WORD_FILTER

	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "StringFilter", this );

	//{{ 임홍락 [2012.05.16] 클라이언트 단일화 // CLIENT_USE_NATION_FLAG 필수 포함임
#ifdef UNITED_CLIENT_EU
	std::wstring wstrStringFilterFile = GetWstrStringFilterFile();
    if ( g_pKTDXApp->LoadLuaTinker( wstrStringFilterFile ) == false )
#else UNITED_CLIENT_EU
    if ( g_pKTDXApp->LoadLuaTinker( L"StringFilter.lua" ) == false )
#endif UNITED_CLIENT_EU
	//}}
    {
		ErrorLogMsg( XEM_ERROR65, "StringFilter.lua 파일 불러오기 오류" );
		//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"StringFilter파일 불러오기 오류", this );
		MessageBox( g_pKTDXApp->GetHWND(), L"StringFilter.lua", GET_STRING( STR_ID_745 ), MB_OK );
		return;
    }

	//if( m_KLuaManager.BeginTable( "BanNickNameList" ) == true )
	//{
	//	wstring banNickName;
	//	int tableIndex = 1;

	//	while( m_KLuaManager.GetValue( tableIndex, banNickName ) == true )
	//	{
	//		MakeUpperCase( banNickName );
	//		m_BanNickNameList.push_back( banNickName );
	//		tableIndex++;
	//	}

	//	m_KLuaManager.EndTable();
	//}

	//if( m_KLuaManager.BeginTable( "BanWordList" ) == true )
	//{
	//	wstring banWord;
	//	int tableIndex = 1;

	//	while( m_KLuaManager.GetValue( tableIndex, banWord ) == true )
	//	{
	//		MakeUpperCase( banWord );
	//		m_BanWordList.push_back( banWord );
	//		tableIndex++;
	//	}

	//	m_KLuaManager.EndTable();
	//}

}

CX2StringFilter::~CX2StringFilter(void)
{
	
}

//{{ 임홍락 [2012.05.16] 클라이언트 단일화 // CLIENT_USE_NATION_FLAG 필수 포함임
#ifdef UNITED_CLIENT_EU
std::wstring CX2StringFilter::GetWstrStringFilterFile()
{
	std::wstring wstrFile = L"";
	switch(g_pMain->GetNationFlag())
	{
	case CX2Main::NF_DE:	wstrFile = L"StringFilter_DE.lua";	break;
	case CX2Main::NF_FR:	wstrFile = L"StringFilter_FR.lua";	break;
	case CX2Main::NF_IT:	wstrFile = L"StringFilter_IT.lua";	break;
	case CX2Main::NF_PL:	wstrFile = L"StringFilter_PL.lua";	break;
	case CX2Main::NF_ES:	wstrFile = L"StringFilter_ES.lua";	break;
	case CX2Main::NF_EN:	wstrFile = L"StringFilter_EN.lua";	break;
	default:
		wstrFile = L"StringFilter.lua";
		break;
	}

	return wstrFile;
}
#endif UNITED_CLIENT_EU
//}}

#ifdef SERV_POST_BAN_WORD_FILTER
void CX2StringFilter::AddPostWordFilter_LUA( char* szFilter )
{
	std::wstring banPostWord;
	ConvertUtf8ToWCHAR( banPostWord, szFilter );
	MakeUpperCase( banPostWord );

	m_BanPostWordList.push_back( banPostWord );
}
#endif //SERV_POST_BAN_WORD_FILTER

#ifdef SERV_POST_BAN_WORD_FILTER
bool CX2StringFilter::CheckIsValidPostWordString( FILTER_TYPE filterType, const WCHAR* pString, OUT wstring &pOutString )
{
	wstring pStringForCheck = pString;
	MakeUpperCase(pStringForCheck);

	if ( filterType == FT_POST )
	{
		for ( int i = 0; i < (int)m_BanPostWordList.size(); i++ )
		{
			wstring banPostWord = m_BanPostWordList[i];
			if ( (int)pStringForCheck.find( banPostWord.c_str() ) != -1 ) //찾았다
			{
				pOutString = banPostWord;
				return false;
			}
		} 
	}
	
	return true;
}
#endif //SERV_POST_BAN_WORD_FILTER

bool CX2StringFilter::CheckIsValidString( FILTER_TYPE filterType, const WCHAR* pString )
{
	wstring pStringForCheck = pString;
	MakeUpperCase(pStringForCheck);

	if ( filterType == FT_NICKNAME )
	{
		
		for ( int i = 0; i < (int)pStringForCheck.size(); i++ )
		{
			WCHAR tempChar = pStringForCheck[i];
// 09.08.24 태완 : 한글 필터링 하는 부분을 중국어 코드 필터링으로 바꿈
#ifdef _LANGUAGE_FIX_CHINESE_
			if ( tempChar >= 0x4E00 && tempChar <= 0x9FA5 )		// Unicode Range for Chinese
			{
				continue;
			}
#else
			if ( tempChar >= 0xAC00 && tempChar <= 0xD7AF ) 
			{
#ifdef SERV_STRING_FILTER_KOREAN
				return false;
#else
				continue;
#endif SERV_STRING_FILTER_KOREAN
			}
#endif

#ifdef _LANGUAGE_FIX_JAPANESE_
			if( (tempChar >= 0x3040 && tempChar <= 0x309F) ||  //히라가나
				(tempChar >= 0x30A0 && tempChar <= 0x30FF) ||  //카타카나
				(tempChar >= 0x31F0 && tempChar <= 0x31FF) ||  //카타카나 음성확장
				(tempChar >= 0x2E80 && tempChar <= 0x2EFF) ||  //한중일 부수 보충
				(tempChar >= 0x3400 && tempChar <= 0x4DBF) ||  //한중일 통합한자 확장 -A
				(tempChar >= 0x4E00 && tempChar <= 0x9FBF) ||  //한중일 통합한자
				(tempChar >= 0xF900 && tempChar <= 0xFAFF) ||  //한중일 호환용 한자
				(tempChar >= 0x20000 && tempChar <= 0x2A6DF) ||  //한중일 통합 한자 확장
				(tempChar >= 0x2F800 && tempChar <= 0x2FA1F)     //한중일 호환용한자보충
				)
			{
				continue;
			}
#endif _LANGUAGE_FIX_JAPANESE_

#ifdef CLIENT_COUNTRY_TH 
			if( tempChar >= 0x0E01 && tempChar <= 0x0E5B ) // Thiland Language Area
			{
				continue;
			}
#endif CLIENT_COUNTRY_TH

			if ( tempChar >= 'A' && tempChar <= 'Z' )
			{
				continue;
			}

			if ( tempChar >= '0' && tempChar <= '9' )
			{
				continue;
			}

			return false;
		}
		


		


		for ( int i = 0; i < (int)m_BanNickNameList.size(); i++ )
		{
			wstring banNickName = m_BanNickNameList[i];
			if ( pStringForCheck == banNickName )
			{
				return false;
			}
		}

		//{{ 2009. 2. 17  최육사	닉네임 포함 문자
		for( int i = 0; i < (int)m_BanNickNameWordList.size(); ++i )
		{
			wstring banNickNameWord = m_BanNickNameWordList[i];
			if( (int)pStringForCheck.find( banNickNameWord.c_str() ) != -1 ) // 찾았다
			{
				return false;
			}
		}
		//}}

		for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
		{
			wstring banWord = m_BanWordList[i];
			if ( (int)pStringForCheck.find( banWord.c_str() ) != -1 ) //찾았다
			{				
				return false;
			}
		}

		if ( (int)pStringForCheck.find( L" " ) != -1 )
		{
			return false;
		}

		if ( (int)pStringForCheck.find( L"\"" ) != -1 )
		{
			return false;
		}

		if ( (int)pStringForCheck.find( L"/" ) != -1 )
		{
			return false;
		}

		if ( (int)pStringForCheck.find( L"\\" ) != -1 )
		{
			return false;
		}

		if ( (int)pStringForCheck.find( L"(" ) != -1 )
		{
			return false;
		}
		if ( (int)pStringForCheck.find( L")" ) != -1 )
		{
			return false;
		}

		if ( (int)pStringForCheck.find( L"]" ) != -1 )
		{
			return false;
		}

		if ( (int)pStringForCheck.find( L"[" ) != -1 )
		{
			return false;
		}

		if ( (int)pStringForCheck.find( L"%" ) != -1 )
		{
			return false;
		}
		if ( (int)pStringForCheck.find( L"!" ) != -1 )
		{
			return false;
		}
		if ( (int)pStringForCheck.find( L"GM" ) != -1 )
		{
			return false;
		}
		if ( (int)pStringForCheck.find( GET_STRING( STR_ID_746 ) ) != -1 )
		{
			return false;
		}
/*
		WCHAR noWanSungWordList[] = L"ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣㄳㄵㄶㄺㄻㄼㄽㄾㄿㅀㅄ";

		for ( int i = 0; i < (int)sizeof(noWanSungWordList)/sizeof(WCHAR); i++ )
		{
			WCHAR noWanSungWord = noWanSungWordList[i];
			if ( (int)pStringForCheck.find( noWanSungWord ) != -1 )
			{
				return false;
			}
		}
*/
	}
	else if ( filterType == FT_CHAT )
	{
		for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
		{
			wstring banWord = m_BanWordList[i];
			if ( (int)pStringForCheck.find( banWord.c_str() ) != -1 ) //찾았다
			{
                return false;
			}
		} 
#ifdef BAN_CARRIAGE_RETURN
		if ( (int)pStringForCheck.find( '\r' ) != -1 )
		{
			return false;
		}
#endif 

	}
#ifdef SERV_POST_BAN_WORD_FILTER
	else if ( filterType == FT_POST )
	{
		for ( int i = 0; i < (int)m_BanPostWordList.size(); i++ )
		{
			wstring banPostWord = m_BanPostWordList[i];
			if ( (int)pStringForCheck.find( banPostWord.c_str() ) != -1 ) //찾았다
			{
				return false;
			}
		} 
	}
#endif //SERV_POST_BAN_WORD_FILTER
	else
	{
		MessageBox( g_pKTDXApp->GetHWND(), GET_STRING( STR_ID_747 ), GET_STRING( STR_ID_748 ), MB_OK );
		return false;
	}

	return true;
}

wstring CX2StringFilter::FilteringNoteString( const WCHAR* pString, WCHAR wcharForReplace )
{
    if ( pString == NULL )
    {
        wstring filteringErrorMsg = GET_STRING( STR_ID_749 );
        return filteringErrorMsg;
    }

	wstring stringForCheck = pString;
	wstring orgStringForCheck = pString;
	MakeUpperCase( stringForCheck );

	for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
	{
		wstring banWord = m_BanWordList[i];
		int iLocation = 0;

		if(banWord.compare(L"\n") == 0)
			continue;

		while( (int)stringForCheck.find( banWord.c_str(), iLocation ) != -1 ) //찾았다
		{
			int index = stringForCheck.find( banWord.c_str(), iLocation );
			int banWordSize = (int)banWord.size();
			WCHAR buff[256] = {0};
			for ( int i = 0; i < banWordSize; i++ )
			{
				buff[i] = wcharForReplace;
				buff[i + 1] = L'\0';
			}
			wstring stringForReplace = buff;
			orgStringForCheck.replace( index, banWordSize, stringForReplace );
			//stringForCheck = orgStringForCheck;
			iLocation = index + banWordSize;
		}
	}

	return orgStringForCheck;
}

wstring CX2StringFilter::FilteringChatString( const WCHAR* pString, WCHAR wcharForReplace )
{
	if ( pString == NULL )
	{
		wstring filteringErrorMsg = GET_STRING( STR_ID_749 );
		return filteringErrorMsg;
	}

	wstring stringForCheck = pString;
	wstring orgStringForCheck = pString;
	MakeUpperCase( stringForCheck );

	int nStringLength = lstrlen(pString);
	for(int i=0; i<nStringLength; ++i)
	{
		if( pString[i] == 0x1C ||
			pString[i] == 0x1D ||
			pString[i] == 0x1E ||
			pString[i] == 0x1F )
		{			
			return L"♡♡♡!";
		}
	}

	for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
	{
		wstring banWord = m_BanWordList[i];
		int iLocation = 0;

		while( (int)stringForCheck.find( banWord.c_str(), iLocation ) != -1 ) //찾았다
		{
			int index = stringForCheck.find( banWord.c_str(), iLocation );
			int banWordSize = (int)banWord.size();
			WCHAR buff[256] = {0};
			for ( int i = 0; i < banWordSize; i++ )
			{
				buff[i] = wcharForReplace;
				buff[i + 1] = L'\0';
			}
			wstring stringForReplace = buff;
			orgStringForCheck.replace( index, banWordSize, stringForReplace );
			//stringForCheck = orgStringForCheck;
			iLocation = index + banWordSize;
		}
	}

	return orgStringForCheck;


}

bool CX2StringFilter::CheckIsKoreanNEnglishNNumber( const WCHAR* pString )
{
	wstring pStringForCheck = pString;
	MakeUpperCase(pStringForCheck);

	for ( int i = 0; i < (int)pStringForCheck.size(); i++ )
	{
		WCHAR tempChar = pStringForCheck[i];

		if ( tempChar >= 0xAC00 && tempChar <= 0xD7AF ) 
		{
#ifdef SERV_STRING_FILTER_KOREAN
			return false;
#else
			continue;
#endif SERV_STRING_FILTER_KOREAN
		}

		if ( tempChar >= 'A' && tempChar <= 'Z' )
		{
			continue;
		}

		if ( tempChar >= '0' && tempChar <= '9' )
		{
			continue;
		}

		return false;
	}


	return true;
}

#ifdef SEARCH_UNIT_NICKNAME_TW
bool CX2StringFilter::CheckIsUseableUnicodeCharacterNickname( const WCHAR* pString )
{
	wstring pStringForCheck = pString;
	MakeUpperCase(pStringForCheck);

	for ( int i = 0; i < (int)pStringForCheck.size(); i++ )
	{
		WCHAR tempChar = pStringForCheck[i];

#ifdef _LANGUAGE_FIX_CHINESE_
		// 중국어
		if ( tempChar >= 0x4E00 && tempChar <= 0x9FA5 )		// Unicode Range for Chinese
		{
			continue;
		}
#else
		// 한국어
		if ( tempChar >= 0xAC00 && tempChar <= 0xD7AF ) 
		{
			continue;
		}
#endif

		if ( tempChar >= 'A' && tempChar <= 'Z' )
		{
			continue;
		}

		if ( tempChar >= '0' && tempChar <= '9' )
		{
			continue;
		}

		return false;
	}

	return true;
}
#endif SEARCH_UNIT_NICKNAME_TW

void CX2StringFilter::AddNickNameFilter_LUA( char* szFilter )
{
    std::wstring banNickName;
    ConvertUtf8ToWCHAR( banNickName, szFilter );
	MakeUpperCase( banNickName );

	m_BanNickNameList.push_back( banNickName );
}

//{{ 2009. 2. 17  최육사	닉네임 포함 문자
void CX2StringFilter::AddNickNameWordFilter_LUA( char* szFilter )
{
    std::wstring banNickNameWord;
    ConvertUtf8ToWCHAR( banNickNameWord, szFilter );
    MakeUpperCase( banNickNameWord );

	m_BanNickNameWordList.push_back( banNickNameWord );
}
//}}

void CX2StringFilter::AddWordFilter_LUA( char* szFilter )
{
    std::wstring banWord;
    ConvertUtf8ToWCHAR( banWord, szFilter );
    MakeUpperCase( banWord );

	m_BanWordList.push_back( banWord );
}
