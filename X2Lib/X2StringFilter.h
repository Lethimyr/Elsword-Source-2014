#pragma once

class CX2StringFilter
{
	public:

		enum FILTER_TYPE
		{
			//FT_WORD,		//해당 String속에 문제의 단어가 있는가? --"이런 개새끼!!"
			//FT_SENTENCE,	//해당 String을 하나의 단어로 보고 문제가 있는가? -- "운영자"
			FT_NICKNAME = 0,
			FT_CHAT,
//#ifdef SERV_POST_BAN_WORD_FILTER
			FT_POST,
//#endif //SERV_POST_BAN_WORD_FILTER
		};
		
//#ifdef SERV_STRING_FILTER_USING_DB
		enum FILTER_WORD_TYPE
		{
			FWT_NICKNAME = 0,
			FWT_NICKNAMEWORD,
			FWT_WORD,
			FWT_POSTWORD,
		};
//#endif //SERV_STRING_FILTER_USING_DB

		CX2StringFilter();
		virtual ~CX2StringFilter(void);

		bool CheckIsValidString( FILTER_TYPE filterType, const WCHAR* pString );		//유효한 스트링인가?
		wstring FilteringChatString( const WCHAR* pString, WCHAR wcharForReplace );
        wstring FilteringNoteString( const WCHAR* pString, WCHAR wcharForReplace );
		//{{ 임홍락 [2012.05.16] 클라이언트 단일화 // CLIENT_USE_NATION_FLAG 필수 포함임
#ifdef UNITED_CLIENT_EU
		std::wstring GetWstrStringFilterFile();
#endif UNITED_CLIENT_EU
		//}}
#ifdef SERV_POST_BAN_WORD_FILTER
		bool CheckIsValidPostWordString( FILTER_TYPE filterType, const WCHAR* pString, OUT wstring &pOutString );	// 2012.09.10 lygan_조성욱 // 우편 본문 금칙어 걸렸을때 어떤 내용이 걸리는지 표시용
		void AddPostWordFilter_LUA( char* szFilter );
#endif //SERV_POST_BAN_WORD_FILTER

		bool CheckIsKoreanNEnglishNNumber( const WCHAR* pString );
#ifdef SEARCH_UNIT_NICKNAME_TW
		bool CheckIsUseableUnicodeCharacterNickname( const WCHAR* pString );
#endif SEARCH_UNIT_NICKNAME_TW

		void	AddNickNameFilter_LUA( char* szFilter );
		//{{ 2009. 2. 17  최육사	닉네임 포함 문자
		void	AddNickNameWordFilter_LUA( char* szFilter );
		//}}
		void	AddWordFilter_LUA( char* szFilter );
		
	protected:

		//KLuaManager m_KLuaManager;

		vector< wstring > m_BanNickNameList;
		//{{ 2009. 2. 17  최육사	닉네임 포함 문자
		vector< wstring > m_BanNickNameWordList;
		//}}
		vector< wstring > m_BanWordList;
		//{{ //2012.09.10 lygan_조성욱 // 우편에 운영자 사칭 금지 단어 등록
#ifdef SERV_POST_BAN_WORD_FILTER
		vector< wstring > m_BanPostWordList;
#endif //SERV_POST_BAN_WORD_FILTER
		//}}
};
