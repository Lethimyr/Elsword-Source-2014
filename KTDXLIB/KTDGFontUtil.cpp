#include "stdafx.h"
#include "KTDGFontUtil.h"
//

#include "KncUtil.h"

#include <boost/algorithm/string/case_conv.hpp>



namespace KTDGUTIL_MATH
{

}

namespace KTDGUTIL_STR
{
	void RTrim( std::string& str_ )
	{
		str_.erase( str_.find_last_not_of( " \t\n") + 1 );
	}

	void RTrim( std::wstring& str_ )
	{
		str_.erase( str_.find_last_not_of( L" \t\n") + 1 );
	}

	void LTrim( std::string& str_ )
	{
		str_.erase( 0, str_.find_first_not_of( " \t\n" ) );
	}

	void LTrim( std::wstring& str_ )
	{
		str_.erase( 0, str_.find_first_not_of( L" \t\n" ) );
	}

	void Trim( std::string& str_ )
	{
		LTrim( str_ );
		RTrim( str_ );
	}

	void Trim( std::wstring& str_ )
	{
		LTrim( str_ );
		RTrim( str_ );
	}

	//태국어에서 오동작하여 Boost라이브러리로 수정함..

	void ToUpper( std::string& str_ )
	{
		//std::transform( str_.begin(), str_.end(), str_.begin(), std::toupper );
		boost::to_upper(str_);
	}

	void ToUpper( std::wstring& str_ )
	{
		//std::transform( str_.begin(), str_.end(), str_.begin(), std::toupper );
		boost::to_upper(str_);
	}

	void ToLower( std::string & str_ )
	{
		//std::transform( str_.begin(), str_.end(), str_.begin(), std::tolower );
		boost::to_lower(str_);
	}

	void ToLower( std::wstring & str_ )
	{
		//std::transform( str_.begin(), str_.end(), str_.begin(), std::tolower );
		boost::to_lower(str_);
	}

	const char*	GCStrWideToChar(const WCHAR* strSrc)
	{
		static char strTemp[4096];
		memset( strTemp, 0, sizeof(char)*4096 );
		std::wstring strWide = strSrc;
		std::string strNarrow = KncUtil::toNarrowString( strWide );
		lstrcpyA( strTemp, strNarrow.c_str() );

		return strTemp;
	}

	const WCHAR* GCStrCharToWide(const char* strSrc)
	{
		static WCHAR strTemp[4096];
		memset( strTemp, 0, sizeof(WCHAR)*4096 );
		std::string strNarrow = strSrc;
		std::wstring strWide = KncUtil::toWideString( strNarrow );
		lstrcpyW( strTemp, strWide.c_str() );

		return strTemp;
	}

	size_t GetStringBytes( const std::wstring& str_ )
	{
		size_t sizeofbyte = 0;
		for( int i = 0; i < (int)str_.size(); ++i )
		{
			WCHAR wChar = str_[i];
			if( wChar < L' ' )
			{
				continue;
			}
			else if( wChar >= L' ' && wChar <= L'~' )
			{
				sizeofbyte += 1;
			}
			else
			{
				sizeofbyte += 2;
			}
		}
		return sizeofbyte;
	}

	bool CheckInvalidCharacters( const std::wstring& str_ )
	{
		for( int i = 0; i < (int)str_.size(); ++i )
		{
			WCHAR wChar = str_[i];
			if( wChar < L'!' )
			{
				return false;
			}
		}
		return true;
	}

	LPCTSTR GCStrToDefined( const wchar_t* strSrc )
	{
#if defined( UNICODE )
		return strSrc;
#else
		return GCStrWideToChar( strSrc );
#endif
	}

	LPCTSTR GCStrToDefined( const char* strSrc )
	{
#if defined( UNICODE )
		return GCStrCharToWide( strSrc );
#else
		return strSrc;
#endif
	}
}

namespace KTDGUTIL_GRAPHICS
{
}

namespace KTDGUTIL_TIME
{
	int GetYear( time_t tTime_ )
	{
		CTime kTime(tTime_);
		return kTime.GetYear();
	}
	int GetMonth( time_t tTime_ )
	{
		CTime kTime(tTime_);
		return kTime.GetMonth();
	}
	int GetDay( time_t tTime_ )
	{
		CTime kTime(tTime_);
		return kTime.GetDay();
	}
	int GetHour( time_t tTime_ )
	{
		CTime kTime(tTime_);
		return kTime.GetHour();
	}
	int GetMinute( time_t tTime_ )
	{
		CTime kTime(tTime_);
		return kTime.GetMinute();
	}
	int GetSecond( time_t tTime_ )
	{
		CTime kTime(tTime_);
		return kTime.GetSecond();
	}
	time_t GetCurTime()
	{
		return static_cast<time_t>(CTime::GetCurrentTime().GetTime());
	}
	bool IsFuture( time_t tTime_ )
	{
		CTime kCurtime = CTime::GetCurrentTime();
		CTime kCheckTime = tTime_;
		if ( kCurtime <= kCheckTime )
			return true;
		return false;
	}
	bool IsPast( time_t tTime_ )
	{
		return !IsFuture( tTime_);
	}
}


namespace KTDGUTIL_STR_THAI
{
	enum CLASS {	
		CTRL= 0,
		NON= 1,
		CONS= 2,
		LV= 3,
		FV1= 4,
		FV2= 5,
		FV3= 6,
		BV1= 7,
		BV2= 8,
		BD= 9,
		TONE=10,
		AD1=11,
		AD2=12,
		AD3=13,
		AV1=14,
		AV2=15,
		AV3=16,
	};

	CLASS	CLASS_TABLE[256] = {
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, 
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, CTRL, 
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, CTRL, 
		NON, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, //E01
		CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS, //E02
		CONS, CONS, CONS, CONS,  FV3, CONS,  FV3, CONS, CONS, CONS, CONS, CONS, CONS, CONS, CONS,  NON, //E02
		FV1,  AV2,  FV1,  FV1,  AV1,  AV3,  AV2,  AV3,  BV1,  BV2,   BD,  NON,  NON,  NON,  NON,  NON, //E03
		LV,   LV,   LV,   LV,   LV,   FV2,  NON,  AD2, TONE, TONE, TONE, TONE,  AD1,  AD1,  AD3,  NON,  //E04
		NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON,  NON, CTRL, 
		//   0     1     2     3     4     5     6     7     8     9     10
	};

	WCHAR	INTONATION_TABLE[] = {
		0x0E31,
		0x0E34,
		0x0E35,
		0x0E36,
		0x0E37,
		0x0E47,
		0x0E48,
		0x0E49,
		0x0E4A,
		0x0E4B,
		0x0E4C,
		0x0E38,
		0x0E39,
	};

	namespace INPUT
	{
		enum TYPE { A, C, S, R, X };

		bool COMPOSIBLE[3][5] = {
			{ true, true, true, true, true },
			{ true, true, true,false, true },
			{ true, true,false,false, true },
		};

		TYPE STATE_CHECK[17][17] = {
			{ X,A,A,A,R,R,R,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,R,R,R,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,S,A,C,C,C,C,C,C,C,C,C,C },
			{ X,S,A,S,S,S,S,R,R,R,R,R,R,R,R,R,R },
			{ X,S,A,S,A,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,A,S,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,S,A,R,R,R,C,C,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,A,A,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,R,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,C,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,R,R,R,R,R,R },
			{ X,A,A,A,S,S,A,R,R,R,C,R,C,R,R,R,R },
		};
	}

	namespace OUTPUT
	{
		enum TYPE { N, C, X };

		TYPE STATE_CHECK[17][17] = {
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 0
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 1
			{ X,N,N,N,C,C,C,C,C,C,C,C,C,C,C,C,C }, // 2
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 3
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 4
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 5
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 6
			{ X,N,N,N,N,N,N,N,N,N,C,C,N,N,N,N,N }, // 7
			{ X,N,N,N,N,N,N,N,N,N,C,N,N,N,N,N,N }, // 8
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 9
			{ X,N,N,N,C,N,N,N,N,N,N,N,N,N,N,N,N }, // 10
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 11
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 12
			{ X,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N,N }, // 13
			{ X,N,N,N,N,N,N,N,N,N,C,C,N,N,N,N,N }, // 14
			{ X,N,N,N,N,N,N,N,N,N,C,N,N,N,N,N,N }, // 15
			{ X,N,N,N,N,N,N,N,N,N,C,N,C,N,N,N,N }, // 16
		};
	}


	bool IsComposibleA(BYTE prev, BYTE curr, int mode)
	{
		using namespace KTDGUTIL_STR_THAI::INPUT;

		if(mode > 2) {
			return false;
		} else {
			CLASS prev_class = CLASS_TABLE[prev];
			CLASS curr_class = CLASS_TABLE[curr];
			TYPE state_type = STATE_CHECK[prev_class][curr_class];

			return COMPOSIBLE[mode][state_type];
		}
	}

	// Unicode of thai : 0x0E00~0x0E7F (Direct Mapping)
	// Thai Character -> Unicode    (char+0x0E00)-0xA0 =    char+0x0D60
	// Unicode -> Thai Character (wchar_t-0x0E00)+0xA0 = wchar_t-0x0D60
	// 0x0D60 = 3424

	bool CheckClassTableBound(int sum)
	{
		if (sum < 0 || sum > 255) return false;
		return true;
	}

	const WCHAR* CharNextTh(const WCHAR* lpsz)
	{
		using namespace KTDGUTIL_STR_THAI::OUTPUT;

		const WCHAR* stream = (const WCHAR*)(lpsz);
		while(CheckClassTableBound(stream[0]-3424)==true && 
			CheckClassTableBound(stream[1]-3424)==true &&
			STATE_CHECK[CLASS_TABLE[stream[0]-3424]][CLASS_TABLE[stream[1]-3424]] == C) 
		{
			++stream;
		}

		return stream+1;
	}

	const WCHAR* CharPrevTh(const WCHAR* lpszStart, const WCHAR* lpszCurrent)
	{
		while(lpszStart < lpszCurrent) {

			const WCHAR* lpszNext = CharNextTh(lpszStart);

			if(lpszNext < lpszCurrent) {
				lpszStart = lpszNext;
			} else {
				return lpszStart;
			}
		}

		return lpszStart;
	}


	const char* CharNextThA(const char* lpsz)
	{
		using namespace KTDGUTIL_STR_THAI::OUTPUT;

		const BYTE* stream = (const BYTE*)(lpsz);

		while(CheckClassTableBound(stream[0])==true && 
			CheckClassTableBound(stream[1])==true &&
			STATE_CHECK[CLASS_TABLE[stream[0]]][CLASS_TABLE[stream[1]]] == C) ++stream;

		return (const char*)(stream+1);
	}

	const char* CharPrevThA(const char* lpszStart, const char* lpszCurrent)
	{
		while(lpszStart < lpszCurrent) {

			const char* lpszNext = CharNextThA(lpszStart);

			if(lpszNext < lpszCurrent) {
				lpszStart = lpszNext;
			} else {
				return lpszStart;
			}
		}

		return lpszStart;
	}

	bool IsIntonation(const WCHAR wchar)
	{
		for( int i = 0; i < ARRAY_SIZE(INTONATION_TABLE); i++ )
		{
			if( INTONATION_TABLE[i] == wchar )
				return true;
		}

		return false;
	}
}
