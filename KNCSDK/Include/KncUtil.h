#pragma once

#define BEGIN_NAMESPACE( name ) namespace name {
#define END_NAMESPACE }
#define NUM_ELEMENTS(x) ( sizeof((x)) / sizeof(0[x]) ) // vector등에 사용시 컴파일에러를 내도록 의도된 매크로입니다.

#include <KncMacro.h>
#include <string>
#include <tchar.h>
#include <windows.h>
#include <atltime.h>
#include <cmath>    // pow()
#include <cryptopp/md5.h>
#include <boost/format.hpp>
#include <sstream>
#include <set>
#include <vector>

//{{ GetTimeStampForLoadedLibrary
#include <DbgHelp.h>
#pragma comment( lib, "DbgHelp" )
//}}


BEGIN_NAMESPACE( KncUtil )

inline std::wstring GetLastErrMsg()
{
	LPWSTR lpMsgBuf = NULL;
	if( 0 == ::FormatMessageW( 
							FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							::GetLastError(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
							(LPWSTR) &lpMsgBuf,
							0,
							NULL ) )
	{
		if( lpMsgBuf )
			LocalFree( lpMsgBuf );
		return std::wstring(L"에러값 얻기 실패.!");
	}

	std::wstring strRet( lpMsgBuf );
	::LocalFree( lpMsgBuf );

	return strRet;
}

// MD5 문자열 얻기
inline std::string GetMD5( std::string& strIN_ )
{
    byte temp[16];
    CryptoPP::MD5 md5Obj;
    md5Obj.Update( (byte*)strIN_.data(), strIN_.length() );
    md5Obj.Final( temp );

    std::stringstream stm;
    for( int i = 0 ; i < 16 ; i++)
        stm << boost::format( "%02x" ) % (int)temp[i];

    return stm.str();
}

// 실행파일 명을 알려준다.
inline void GetAppName( OUT std::wstring& strAppName ) // 051228. kkurrung. Modify .
{
    wchar_t szFileName[_MAX_PATH];
    ::GetModuleFileNameW(NULL, szFileName, _MAX_FNAME);

    CStringW sAppName; // Extract from last '\' to '.'
    sAppName = szFileName;
    strAppName = sAppName.Mid(sAppName.ReverseFind(L'\\') + 1)
        .SpanExcluding(L".");
}

inline bool GetTimeStampByNameW( OUT std::wstring& strTimeStamp ) // 051228. kkurrung. Modify .
{
    // 실행 파일 생성 시각 얻기.
    HANDLE hFile;
    HANDLE hFMap;
    wchar_t szFileName[MAX_PATH];
    ::GetModuleFileNameW(NULL, szFileName, MAX_PATH);
    hFile = CreateFileW( szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile == INVALID_HANDLE_VALUE )
    {
        return false;
    }

    // 파일 맵핑 오브젝트를 만든다.
    hFMap = CreateFileMappingW( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
    // 주소 공간에 맵한다.
    HMODULE hModule;
    hModule = ( HMODULE )MapViewOfFile( hFMap, FILE_MAP_READ, 0, 0, 0 );
    CTime time( GetTimestampForLoadedLibrary( hModule ) );

    strTimeStamp = (CStringW)time.Format( _T("%Y-%m-%d %H:%M:%S") );
    
    UnmapViewOfFile( hModule );
    CloseHandle( hFMap );
    CloseHandle( hFile );

    return true;
}

//{{ 060105. kkurrung. ANSI Version
inline bool GetTimeStampByNameA( OUT std::string& strTimeStamp )
{
    // 실행 파일 생성 시각 얻기.
    HANDLE hFile;
    HANDLE hFMap;
    char szFileName[MAX_PATH];
    ::GetModuleFileNameA(NULL, szFileName, MAX_PATH);
    hFile = CreateFileA( szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile == INVALID_HANDLE_VALUE )
    {
        return false;
    }

    // 파일 맵핑 오브젝트를 만든다.
    hFMap = CreateFileMappingA( hFile, NULL, PAGE_READONLY, 0, 0, NULL );
    // 주소 공간에 맵한다.
    HMODULE hModule;
    hModule = ( HMODULE )MapViewOfFile( hFMap, FILE_MAP_READ, 0, 0, 0 );
    CTime time( GetTimestampForLoadedLibrary( hModule ) );

    strTimeStamp = (CStringA)time.Format( _T("%Y-%m-%d %H:%M:%S") );

    UnmapViewOfFile( hModule );
    CloseHandle( hFMap );
    CloseHandle( hFile );

    return true;
}
//}} kkurrung.

// pow를 사용하므로, #include <cmath> 해주어야 한다.
inline long hstol(char* str) // 넷마블 옛 인증방식에 쓰이던 함수. hex 문자열을 숫자로 바꿔준다.
{
    if ( str == NULL ) return 0L;

    int     len = (int)::strlen(str);
    long    ret = 0L;
    long    p;
    long    index;
    int     i, j;

    // remove leading "0x" if exists
    if ( len >= 3 )
    {
        if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X') )
        {
            len -= 2;
            memmove(str, &str[2], len);
        }//if
    }//if

    for (i=len-1, j=0; i>=0; --i, ++j)
    {
        p      = (long)pow(16, j);

        {   // 이전에는 내부 클래스의 static function으로 구현되었으나, inline 함수로 만들면서 수정.
            if ( '0' <= str[i] && str[i] <= '9')
                index = str[i] - '0';

            else if ( 'a' <= str[i] && str[i] <= 'f' )
                index = str[i] - 'a' + 10;

            else if ( 'A' <= str[i] && str[i] <= 'F' )
                index = str[i] - 'A' + 10;

            else
                index = 0;
        }

        ret   += p*index;
    }//for

    return ret;
}

inline time_t TimeToInt( CTime& cTime_ )
{
    // CTime::GetTime()은 __time64_t를 리턴한다. 안전하게 struct tm을 얻어서 timt_t를 구하자.
    struct tm tmBuff;
    cTime_.GetLocalTm( &tmBuff );
    return ::mktime( &tmBuff );
}

inline std::string toNarrowString( const wchar_t* pStr , int len = -1 )
{
    std::string buf ;
    if( pStr == NULL )
        return buf;
    if( len != -1 && len <= 0 ) // florist. 0이어도 통과시키지 말자.
        return buf;

    // figure out how many narrow characters we are going to get 
    int nChars = WideCharToMultiByte( CP_ACP , 0 , 
        pStr , len , NULL , 0 , NULL , NULL ) ; 
    if ( len == -1 )
        -- nChars ; 
    if ( nChars == 0 )
        return "" ;

    // convert the wide string to a narrow string
    // nb: slightly naughty to write directly into the string like this
    buf.resize( nChars ) ;
    WideCharToMultiByte( CP_ACP , 0 , pStr , len , 
        const_cast<char*>(buf.c_str()) , nChars , NULL , NULL ) ; 

    return buf ; 
}

inline std::string toNarrowString( const std::wstring& str )
{
    return toNarrowString( str.c_str() );
}

inline std::wstring toWideString( const char* pStr , int len = -1 )
{
    std::wstring buf ;
    if( pStr == NULL )
        return buf;
    if( len != -1 && len <= 0 ) // florist. 0이어도 통과시키지 말자.
        return buf;

    // figure out how many wide characters we are going to get 
    int nChars = MultiByteToWideChar( CP_ACP , 0 , pStr , len , NULL , 0 ) ; 
    if ( len == -1 )
        -- nChars ; 
    if ( nChars == 0 )
        return L"" ;

    // convert the narrow string to a wide string 
    // nb: slightly naughty to write directly into the string like this
    buf.resize( nChars ) ; 
    MultiByteToWideChar( CP_ACP , 0 , pStr , len , 
        const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

    return buf ;
}

inline std::wstring toWideString( const std::string& str )
{
    return toWideString( str.c_str() );
}

inline std::wstring toWideString_UTF8( const char* pStr , int len = -1 )
{
	std::wstring buf ;
	if( pStr == NULL )
		return buf;
	if( len != -1 && len <= 0 ) // florist. 0이어도 통과시키지 말자.
		return buf;

	// figure out how many wide characters we are going to get 
	int nChars = MultiByteToWideChar( CP_UTF8 , 0 , pStr , len , NULL , 0 ) ; 
	if ( len == -1 )
		-- nChars ; 
	if ( nChars == 0 )
		return L"" ;

	// convert the narrow string to a wide string 
	// nb: slightly naughty to write directly into the string like this
	buf.resize( nChars ) ; 
	MultiByteToWideChar( CP_UTF8 , 0 , pStr , len , 
		const_cast<wchar_t*>(buf.c_str()) , nChars ) ; 

	return buf ;
}

inline std::wstring toWideString_UTF8( const std::string& str )
{
	return toWideString_UTF8( str.c_str() );
}

inline bool ConvertStringToCTime( const std::wstring& wstrDate, CTime& ctDate )
{
    //## 날짜 문자열은 항상 : 2008-01-22 11:16:00 이런식의 패턴으로 넘어와야한다.

    if( wstrDate.size() < 19 )
    {
        return false;
    }

    //#0. year
    //#1. month
    //#2. day
    //#3. hour
    //#4. min
    //#5. sec
    std::wstring wstrToken[6];
    int iNowToken = 0;

    const WCHAR* pT = wstrDate.c_str();

    while( (*pT) != 0 )
    {
        switch( (*pT) )
        {
        case L'-':
        case L' ':
        case L':':
            {
                ++iNowToken;
            }
            break;

        default:
            {
                if( (*pT) >= L'0' && (*pT) <= L'9' || (*pT) == L'.' )
                {
                    if( wstrToken[iNowToken].size() == 0 )
                        wstrToken[iNowToken] = (*pT);
                    else
                        wstrToken[iNowToken] += (*pT);
                }
            }
        }

        if( iNowToken >= 6 )
            break;

        ++pT;
    }

    int	iToken[6];
    for( int i = 0; i < 6; ++i )
    {
        iToken[i] = 0;
        iToken[i] = _wtoi( wstrToken[i].c_str() );
    }

	//{{ 2010. 10. 13	최육사	CTime에서 exception발생 안시키도록 밖에서 따로 검사
	if( iToken[2] < 1  ||  iToken[2] > 31 )
	{
		return false;
	}

	if( iToken[1] < 1  ||  iToken[1] > 12 )
	{
		return false;
	}

	if( iToken[0] < 1900 )
	{
		return false;
	}
	//}}

	//{{ 2009. 10. 21  최육사	초기값 체크 - exception내지 않고 false를 return하도록 함.
	if( iToken[0] == 1900  &&
		iToken[1] == 1 &&
		iToken[2] == 1 &&
		iToken[3] == 0 &&
		iToken[4] == 0 &&
		iToken[5] == 0 )
	{
		// 이런경우에도 false리턴 해야함.
		return false;
	}
	//}}

    ctDate = CTime( iToken[0], iToken[1], iToken[2], iToken[3], iToken[4], iToken[5] );

    return true;
}

//{{ 2011. 04. 25	최육사	랜덤 획득 로직 개선
inline std::vector< int > GetRandomNumberList( const int iMaxNum )
{
	std::set< int > setDataSource;
	std::vector< int > vecResult;

	for( int iNum = 0; iNum < iMaxNum; ++iNum )
	{
		setDataSource.insert( iNum );
	}

	for( int iNum = 0; iNum < iMaxNum; ++iNum )
	{
		if( setDataSource.size() == 0 )
			break;

		const int iRandomRet = static_cast<int>(rand() % static_cast<int>(setDataSource.size()));
		std::set< int >::iterator sit;
		int iIndex = 0;
		for( sit = setDataSource.begin(); sit != setDataSource.end(); ++sit )
		{
			if( iIndex != iRandomRet )
			{
				++iIndex;
				continue;
			}

			vecResult.push_back( *sit );
			break;
		}

		if( sit != setDataSource.end() )
		{
			setDataSource.erase( sit );
		}
	}

	return vecResult;
}

template< class T >
inline std::vector< T > NumberShuffle( const std::vector< T >& vecSource )
{
	std::vector< T > vecDataSource = vecSource;
	std::vector< T > vecResult;
	const int iMaxNum = static_cast<int>(vecDataSource.size());

	for( int iNum = 0; iNum < iMaxNum; ++iNum )
	{
		if( vecDataSource.size() == 0 )
			break;

		const int iRandomRet = static_cast<int>(rand() % static_cast<int>(vecDataSource.size()));

		std::vector< T >::iterator vitSelected;
		int iIndex = 0;
		for( vitSelected = vecDataSource.begin(); vitSelected != vecDataSource.end(); ++vitSelected )
		{
			if( iIndex != iRandomRet )
			{
				++iIndex;
				continue;
			}

			vecResult.push_back( *vitSelected );
			break;
		}

		if( vitSelected != vecDataSource.end() )
		{
			vecDataSource.erase( vitSelected );
		}
	}

	return vecResult;
}
//}}

//{{ 2011. 06. 13	최육사	스테이지 로그
//#ifdef SERV_DUNGEON_STAGE_LOG
inline std::wstring GetCurrentTimeString()
{
	CTime tCurTime = CTime::GetCurrentTime();
	std::wstring wstrRegDate = (const wchar_t*)tCurTime.Format( _T( "%Y-%m-%d %H:%M:%S" ) );
	return wstrRegDate;
}
//#endif SERV_DUNGEON_STAGE_LOG
//}}


//{{ 2011. 07. 27    김민성    투니랜드 채널링
//#ifdef SERV_TOONILAND_CHANNELING
inline std::wstring EraseFrontWord( const std::wstring& wstrWord, const wchar_t flag )
{
	std::wstring wstrResult = wstrWord;

	unsigned int uiIndex = static_cast<unsigned int>(wstrResult.find( L';' ));
	if( uiIndex != 0 )
	{
		wstrResult.erase( 0, uiIndex + 1 );
	}

	return wstrResult;
}
//#endif SERV_TOONILAND_CHANNELING
//}}


END_NAMESPACE

