#pragma once
/**	@file	: 2010/08/31
	@desc	: 숫자가 아닌 다른 형태로 숫자를 표현
	@since	: 31:8:2010   19:41
	@author	: blackhydra
*/

#include <sstream>

using std::string;
using std::stringstream;

/** @class	: CNumbersToLanguage
	@brief	: 
	@desc	: 숫자를 입력하면 한글 또는 다른 언어의 형태로 출력하는 클래스이다.
	@date	: 2010/08/31
*/
class CX2NumbersToLanguage
{
public:
	/**
		@brief TranslateLanguage

		변환될 언어 정의
	*/
	enum TypeLanguage
	{
		TL_CHN,
		TL_ENG,
		TL_JPN,
		TL_KOR,
//#ifdef _LANGUAGE_FIX_CHINESE_
		TL_CHN_SIMPLIFIED,
//#endif _LANGUAGE_FIX_CHINESE_
	};

	/**
		@brief LanguageData

		변환 데이터
	*/
	struct LanguageData
	{
		string LD_KOR[3][10];
		string LD_CHN[3][10];
#ifdef NUMBER_TO_LANGUAGE_JPN
		string LD_JPN[3][10];
#endif // NUMBER_TO_LANGUAGE_JPN
#ifdef _LANGUAGE_FIX_CHINESE_
		wstring LD_CHN_SIMPLIFIED[3][10];
#endif _LANGUAGE_FIX_CHINESE_
		
		LanguageData()
		{
			/// 한국
			string kor1[10]	= {"",	"일", "이", "삼", "사", "오", "육", "칠", "팔", "구"};
			memcpy(LD_KOR[0], kor1, sizeof(string) * 10);
			string kor2[10]	= {"",	"십", "백", "천", "", "", "", "", "", ""};
			memcpy(LD_KOR[1], kor2, sizeof(string) * 10);
			string kor3[10]	= {"", "만","억","조", "경", "해", "자", "양", "구", "간"};
			memcpy(LD_KOR[2], kor3, sizeof(string) * 10);

			/// 한자
			string chn1[10]	= {"",	"一", "二", "三", "四", "五", "六", "七", "八", "九"};
			memcpy(LD_CHN[0], chn1, sizeof(string) * 10);
			string chn2[10]	= {"",	"十", "百", "千", "", "", "", "", "", ""};
			memcpy(LD_CHN[1], chn2, sizeof(string) * 10);
			string chn3[10]	= {"", "万","億","兆", "京", "", "", "", "", ""};
			memcpy(LD_CHN[2], chn3, sizeof(string) * 10);

#ifdef NUMBER_TO_LANGUAGE_JPN
			string jpn1[10]	= {"",	"一", "二", "三", "四", "五", "六", "七", "八", "九"};
			memcpy(LD_JPN[0], jpn1, sizeof(string) * 10);
			string jpn2[10]	= {"",	"十", "百", "千", "", "", "", "", "", ""};
			memcpy(LD_JPN[1], jpn2, sizeof(string) * 10);
			string jpn3[10]	= {"", "万","億","兆", "京", "", "", "", "", ""};
			memcpy(LD_JPN[2], jpn3, sizeof(string) * 10);
#endif // NUMBER_TO_LANGUAGE_JPN

#ifdef _LANGUAGE_FIX_CHINESE_
			/// Chinese (Simplified)
			wstring chn_simple1[10]	= {L"",	L"一", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九"};
			memcpy(LD_CHN_SIMPLIFIED[0], chn_simple1, sizeof(wstring) * 10);
			wstring chn_simple2[10]	= {L"",	L"十", L"百", L"千", L"", L"", L"", L"", L"", L""};
			memcpy(LD_CHN_SIMPLIFIED[1], chn_simple2, sizeof(wstring) * 10);
			wstring chn_simple3[10]	= {L"", L"万", L"亿", L"兆", L"京", L"", L"", L"", L"", L""};
			memcpy(LD_CHN_SIMPLIFIED[2], chn_simple3, sizeof(wstring) * 10);
#endif _LANGUAGE_FIX_CHINESE_
		}

	};

public:
	/// 생성자
	CX2NumbersToLanguage(void);
	/// 소멸자
	~CX2NumbersToLanguage(void);

public:
	/// 숫자를 문자열로
	template < class T > 
	string ToString(const T &arg)
	{
		stringstream out;

		out.imbue(std::locale("C"));
		
		out << arg;
	
		return(out.str());
	};
	
	/// 문자를 숫자로
	int ToNumbers(const char v) {
		stringstream ss;
		ss << v;
		int ret;
		ss >> ret;
		return ret;
	}

	/// string형을 wstring형으로
	std::wstring 
		mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = std::mbstate_t();
		std::vector<wchar_t> buf(str.size() + 1);
		char const* in_next = str.c_str();
		wchar_t* out_next = &buf[0];
		std::codecvt_base::result r = codecvt.in(state, 
			str.c_str(), str.c_str() + str.size(), in_next, 
			&buf[0], &buf[0] + buf.size(), out_next);
		if (r == std::codecvt_base::error)
			throw std::runtime_error("can't convert string to wstring");   
		return std::wstring(&buf[0]);
	}

	/// wstring형을 string형으로
	std::string 
		wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale())
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
		codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
		std::mbstate_t state = std::mbstate_t();
		std::vector<char> buf((str.size() + 1) * codecvt.max_length());
		wchar_t const* in_next = str.c_str();
		char* out_next = &buf[0];
		std::codecvt_base::result r = codecvt.out(state, 
			str.c_str(), str.c_str() + str.size(), in_next, 
			&buf[0], &buf[0] + buf.size(), out_next);
		if (r == std::codecvt_base::error)
			throw std::runtime_error("can't convert wstring to string");   
		return std::string(&buf[0]);
	}

	/// 변환 함수
#ifdef _LANGUAGE_FIX_CHINESE_
	void TranslateDigits(TypeLanguage, __int64, wstring&);
#else _LANGUAGE_FIX_CHINESE_
	void TranslateDigits(TypeLanguage, __int64, string&);
#endif _LANGUAGE_FIX_CHINESE_

	//void TranslateDigits(TypeLanguage, string, string&);

public:
	/// 언어 형태
	TypeLanguage getType(void);

private:
	TypeLanguage	m_eType;			///< 언어 형태
	LanguageData	m_LanguageData;		///< 언어 데이터
};
