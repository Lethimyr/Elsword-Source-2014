#include "SimLayer.h"
#include ".\xslstringfilter.h"

//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
	ImplementRefreshSingleton( CXSLStringFilter );
#else
	ImplementSingleton( CXSLStringFilter );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}

CXSLStringFilter::CXSLStringFilter()
{
	//lua_tinker::class_<CXSLStringFilter>("CXSLStringFilter")
	//	.def("AddNickNameFilter",			CXSLStringFilter::AddNickNameFilter_LUA)
	//	.def("AddWordFilter",				CXSLStringFilter::AddWordFilter_LUA)
	//	;

	//lua_tinker::decl( "StringFilter", this );



	//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
#else
	lua_tinker::class_add<CXSLStringFilter>( g_pLua, "CXSLStringFilter" );
	lua_tinker::class_def<CXSLStringFilter>( g_pLua, "AddNickNameFilter",		&CXSLStringFilter::AddNickNameFilter_LUA );
	lua_tinker::class_def<CXSLStringFilter>( g_pLua, "AddNickNameWordFilter",	&CXSLStringFilter::AddNickNameWordFilter_LUA );
	lua_tinker::class_def<CXSLStringFilter>( g_pLua, "AddWordFilter",			&CXSLStringFilter::AddWordFilter_LUA );
#ifdef SERV_POST_BAN_WORD_FILTER
	lua_tinker::class_def<CXSLStringFilter>( g_pLua, "AddPostWordFilter",		&CXSLStringFilter::AddPostWordFilter_LUA );
#endif //SERV_POST_BAN_WORD_FILTER

	lua_tinker::decl( g_pLua, "StringFilter", this );
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
	//}}
}

CXSLStringFilter::~CXSLStringFilter(void)
{

}

//{{ 2012. 01. 27	박세훈	String Filter 실시간 스크립트 갱신
#ifdef SERV_STRING_FILTER_SCRIPT_REFRESH
ImplToStringW( CXSLStringFilter )
{
	stm_	<< L"----------[ CXSLStringFilter ]----------" << std::endl
		<< TOSTRINGW( m_BanNickNameList.size() )
		<< TOSTRINGW( m_BanNickNameWordList.size() )
		<< TOSTRINGW( m_BanWordList.size() )
#ifdef SERV_POST_BAN_WORD_FILTER
		<< TOSTRINGW( m_BanPostWordList.size() )
#endif //SERV_POST_BAN_WORD_FILTER
		;

	return stm_;
}

ImplementLuaScriptParser( CXSLStringFilter )
{
	lua_tinker::class_add<CXSLStringFilter>( GetLuaState(), "CXSLStringFilter" );
	lua_tinker::class_def<CXSLStringFilter>( GetLuaState(), "AddNickNameFilter",		&CXSLStringFilter::AddNickNameFilter_LUA );
	lua_tinker::class_def<CXSLStringFilter>( GetLuaState(), "AddNickNameWordFilter",	&CXSLStringFilter::AddNickNameWordFilter_LUA );
	lua_tinker::class_def<CXSLStringFilter>( GetLuaState(), "AddWordFilter",			&CXSLStringFilter::AddWordFilter_LUA );
#ifdef SERV_POST_BAN_WORD_FILTER
	lua_tinker::class_def<CXSLStringFilter>( GetLuaState(), "AddPostWordFilter",		&CXSLStringFilter::AddPostWordFilter_LUA );
#endif //SERV_POST_BAN_WORD_FILTER
	lua_tinker::class_def<CXSLStringFilter>( GetLuaState(), "dump",						&CXSLStringFilter::Dump );

	lua_tinker::decl( GetLuaState(), "StringFilter", this );
}
#else
bool CXSLStringFilter::OpenScript( const char* pFileName)
{
	_JIF( 0 == LUA_DOFILE( g_pLua, pFileName ), return false );

	return true;
}
#endif SERV_STRING_FILTER_SCRIPT_REFRESH
//}}


bool CXSLStringFilter::CheckIsValidString( FILTER_TYPE filterType, std::wstring& wstringForCheck )
{
	std::wstring pStringForCheck = wstringForCheck;
	MakeUpperCase(pStringForCheck);

	if ( filterType == FT_NICKNAME )
	{

		for ( int i = 0; i < (int)pStringForCheck.size(); i++ )
		{
			WCHAR tempChar = pStringForCheck[i];

            DWORD dwAuthFlag = KSimLayer::GetKObj()->GetAuthFlag();

#ifdef SERV_STRING_FILTER_KOREAN
			if( dwAuthFlag != KSimLayer::AF_NEXON_KOREA )
			{
				if ( tempChar >= 0xAC00 && tempChar <= 0xD7AF ) 
				{
					return false;
				}
			}
#endif SERV_STRING_FILTER_KOREAN

            if( dwAuthFlag == KSimLayer::AF_INTERNAL )
            {
                continue;
            }
            else if( dwAuthFlag == KSimLayer::AF_NEXON_KOREA )
            {
                if( tempChar >= 0xAC00 && tempChar <= 0xD7AF )     // Korean
                {
                    continue;
                }
            }
#if defined( SERV_COUNTRY_TWHK ) || defined( SERV_COUNTRY_CN )
			else if( dwAuthFlag == KSimLayer::AF_GLOBAL_SERVICE )
			{
				if( tempChar >= 0x4E00 && tempChar <= 0x9FA5 )
				{
					continue;
				}
			}
#endif // SERV_COUNTRY_TWHK

#ifdef SERV_COUNTRY_JP
			else if( dwAuthFlag == KSimLayer::AF_GLOBAL_SERVICE )
			{
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
			}
#endif //SERV_COUNTRY_JP

            else
            {
                continue;
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


		for ( int i = 0; i < (int)m_BanNickNameList.size(); i++ )
		{
			std::wstring banNickName = m_BanNickNameList[i];
			if ( pStringForCheck == banNickName )
			{
				return false;
			}
		}

		//{{ 2009. 2. 17  최육사	닉네임 포함 문자
		for( int i = 0; i < (int)m_BanNickNameWordList.size(); ++i )
		{
			std::wstring banNickNameWord = m_BanNickNameWordList[i];
			if( (int)pStringForCheck.find( banNickNameWord.c_str() ) != -1 ) // 찾았다
			{
				return false;
			}
		}
		//}}

		for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
		{
			std::wstring banWord = m_BanWordList[i];
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
		//if ( (int)pStringForCheck.find( L"운영" ) != -1 ) -- StringFilter.lua 에 정의되어 있으므로 주석처리하였습니다.
		//{
		//	return false;
		//}
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
			std::wstring banWord = m_BanWordList[i];
			if ( (int)pStringForCheck.find( banWord.c_str() ) != -1 ) //찾았다
			{
				return false;
			}
		}
	}
#ifdef SERV_POST_BAN_WORD_FILTER
	else if ( filterType == FT_POST )
	{
		for ( int i = 0; i < (int)m_BanPostWordList.size(); i++ )
		{
			std::wstring banPostWord = m_BanPostWordList[i];
			if ( (int)pStringForCheck.find( banPostWord.c_str() ) != -1 ) //찾았다
			{
				return false;
			}
		}
	}
#endif //SERV_POST_BAN_WORD_FILTER
	else
	{
		return false;
	}

	return true;
}

//std::wstring CXSLStringFilter::FilteringChatString( const WCHAR* pString, WCHAR wcharForReplace )
//{
//	if ( pString == NULL )
//	{		
//		std::wstring filteringErrorMsg = L"wrong string";
//		return filteringErrorMsg;
//	}
//
//	std::wstring stringForCheck = pString;
//	std::wstring orgStringForCheck = pString;
//	MakeUpperCase( stringForCheck );
//
//	for ( int i = 0; i < (int)m_BanWordList.size(); i++ )
//	{
//		std::wstring banWord = m_BanWordList[i];
//		while( (int)stringForCheck.find( banWord.c_str() ) != -1 ) //찾았다
//		{
//			int index = stringForCheck.find( banWord.c_str() );
//			int banWordSize = (int)banWord.size();
//			WCHAR buff[256] = {0};
//			for ( int i = 0; i < banWordSize; i++ )
//			{
//				buff[i] = wcharForReplace;
//				buff[i + 1] = L'\0';
//			}
//			std::wstring stringForReplace = buff;
//			orgStringForCheck.replace( index, banWordSize, stringForReplace );
//			stringForCheck = orgStringForCheck;
//		}
//	}
//
//	return orgStringForCheck;
//
//}

void CXSLStringFilter::AddNickNameFilter_LUA( char* szFilter )
{
	std::wstring banNickName = KncUtil::toWideString_UTF8( std::string( szFilter ) );
	MakeUpperCase(banNickName);

	m_BanNickNameList.push_back( banNickName );
}

//{{ 2009. 2. 17  최육사	닉네임 포함 문자
void CXSLStringFilter::AddNickNameWordFilter_LUA( char* szFilter )
{
	std::wstring banNickNameWord = KncUtil::toWideString_UTF8( std::string( szFilter ) );
	MakeUpperCase(banNickNameWord);

	m_BanNickNameWordList.push_back( banNickNameWord );
}
//}}

void CXSLStringFilter::AddWordFilter_LUA( char* szFilter )
{
	std::wstring banWord = KncUtil::toWideString_UTF8( std::string( szFilter ) );
	MakeUpperCase(banWord);

	m_BanWordList.push_back( banWord );
}

#ifdef SERV_POST_BAN_WORD_FILTER
void CXSLStringFilter::AddPostWordFilter_LUA( char* szFilter )
{
	std::wstring banWord = KncUtil::toWideString_UTF8( std::string( szFilter ) );
	MakeUpperCase(banWord);

	m_BanPostWordList.push_back( banWord );
}
#endif //SERV_POST_BAN_WORD_FILTER

#ifdef SERV_STRING_FILTER_USING_DB
void CXSLStringFilter::GetStringFilter( OUT StringFilterVector& vecStringFilter )
{
	std::vector< std::wstring >::iterator vit;
	KStringFilterInfo kStringFilter;

	kStringFilter.m_sStringFilterType = FWT_NICKNAME;
	for( vit = m_BanNickNameList.begin(); vit != m_BanNickNameList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}

	kStringFilter.m_sStringFilterType = FWT_NICKNAMEWORD;
	for( vit = m_BanNickNameWordList.begin(); vit != m_BanNickNameWordList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}

	kStringFilter.m_sStringFilterType = FWT_WORD;
	for( vit = m_BanWordList.begin(); vit != m_BanWordList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}

	kStringFilter.m_sStringFilterType = FWT_POSTWORD;
	for( vit = m_BanPostWordList.begin(); vit != m_BanPostWordList.end(); ++vit )
	{
		kStringFilter.m_wstrFilter = *vit;
		vecStringFilter.push_back( kStringFilter );
	}
}
#endif //SERV_STRING_FILTER_USING_DB