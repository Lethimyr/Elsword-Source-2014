/** @file   KJohnStrUtil.h
	@brief  interface of KJohnStrUtil
	@author seojt@kogstudios.com
	@since  2005-5-2, 17:47:36

    @date   seojt on 2005-5-4, 13:47:33
            - StringCopyA(), StringCopyW()
    @date   seojt on 2005-6-22, 9:38:06
            - StringPrintf()
    @date   seojt on 2005-10-1, 16:23:05
            int _iMaxLength added for converson functions
    @date   seojt on 2006-3-3, 10:29:10
            - tokenize()
*/
#pragma once

#ifndef _KJOHNSTRUTIL_H
#define _KJOHNSTRUTIL_H

//#include "KJohnGen.h"
//#include <strsafe.h>
//#include <vector>
//#include <string>
//#include <malloc.h>
//#pragma warning(push)
//#pragma warning(disable: 4276)


//------------------------------------------------------------------------------
// UNICODE compatible string utility functions
//
inline int StringLengthA(const CHAR* _pszString, int _iMaxLength = MAX_PATH)
{
    size_t  iLength;
    StringCchLengthA( _pszString, _iMaxLength, &iLength );
    return (int)iLength;
}//StringLength()


inline int StringLengthW(const WCHAR* _pszString, int _iMaxLength = MAX_PATH)
{
    size_t  iLength;
    StringCchLengthW( _pszString, _iMaxLength, &iLength );
    return (int)iLength;
}//StringLength()


inline int StringLength(const TCHAR* _pszString, int _iMaxLength = MAX_PATH)
{
    size_t  iLength;
    StringCchLength( _pszString, _iMaxLength, &iLength );
    return (int)iLength;
}//StringLength()


inline CHAR* StringCopyA(CHAR* _pszDest, const CHAR* _pszSrc, int _iMaxLen = MAX_PATH)
{
    StringCchCopyA( _pszDest, _iMaxLen, _pszSrc );
    return _pszDest;
}//StringCopyA()


inline WCHAR* StringCopyW(WCHAR* _pszDest, const WCHAR* _pszSrc, int _iMaxLen = MAX_PATH)
{
    StringCchCopyW( _pszDest, _iMaxLen, _pszSrc );
    return _pszDest;
}//StringCopyW()


inline TCHAR* StringCopy(TCHAR* _pszDest, const TCHAR* _pszSrc, int _iMaxLen = MAX_PATH)
{
    StringCchCopy( _pszDest, _iMaxLen, _pszSrc );
    return _pszDest;
}//StringCopy()


inline CHAR* StringCatA(CHAR* _pszDest, const CHAR* _pszSrc, int _iMaxLen = MAX_PATH)
{
    StringCchCatA( _pszDest, _iMaxLen, _pszSrc );
    return _pszDest;
}//StringCatA()


inline WCHAR* StringCatW(WCHAR* _pszDest, const WCHAR* _pszSrc, int _iMaxLen = MAX_PATH)
{
    StringCchCatW( _pszDest, _iMaxLen, _pszSrc );
    return _pszDest;
}//StringCatA()


inline TCHAR* StringCat(TCHAR* _pszDest, const TCHAR* _pszSrc, int _iMaxLen = MAX_PATH)
{
    StringCchCat( _pszDest, _iMaxLen, _pszSrc );
    return _pszDest;
}//StringCat()


inline int StringCompareA(const CHAR* _pszFirst, const CHAR* _pszSecond)
{
    return _stricmp( _pszFirst, _pszSecond );
}//StringCompareA()


inline int StringCompareW(const WCHAR* _pszFirst, const WCHAR* _pszSecond)
{
    return _wcsicmp( _pszFirst, _pszSecond );
}//StringCompareA()


inline int StringCompare(const TCHAR* _pszFirst, const TCHAR* _pszSecond)
{
#ifdef _UNICODE
    return _wcsicmp( _pszFirst, _pszSecond );
#else
    return _stricmp( _pszFirst, _pszSecond );
#endif // _UNICODE
}//StringCompare()


inline void StringPrintfA(CHAR* _pszOut, const CHAR* _pszFormat, ... )
{
    va_list argList;

    va_start( argList, _pszFormat );
    StringCchVPrintfA( _pszOut, MAX_PATH, _pszFormat, argList );
    va_end( argList );
}//StringPrintfA()


inline void StringPrintfW(WCHAR* _pszOut, const WCHAR* _pszFormat, ... )
{
    va_list argList;

    va_start( argList, _pszFormat );
    StringCchVPrintfW( _pszOut, MAX_PATH, _pszFormat, argList );
    va_end( argList );
}//StringPrintfW()


inline void StringPrintf(TCHAR* _pszOut, const TCHAR* _pszFormat, ... )
{
    va_list argList;

    va_start( argList, _pszFormat );
    StringCchVPrintf( _pszOut, MAX_PATH, _pszFormat, argList );
    va_end( argList );
}//StringPrintf()


inline WCHAR* ConvertAnsiStringToWide(WCHAR* _wstrDestination, const CHAR* _strSource, int _iMaxLength = MAX_PATH)
{
    if ( _wstrDestination == NULL || _strSource == NULL )
        return NULL;

    int cchDestChar = StringLengthA( _strSource, _iMaxLength );

    ::MultiByteToWideChar( CP_ACP, 0, _strSource, -1, _wstrDestination, cchDestChar );
        //                                        ^ means destination is large enough
    _wstrDestination[cchDestChar] = 0;
    return _wstrDestination;
}//ConvertAnsiStringToWide()


inline CHAR* ConvertWideStringToAnsi(CHAR* strDestination, const WCHAR* _wstrSource, int _iMaxLength = MAX_PATH)
{
    if ( strDestination == NULL || _wstrSource == NULL )
        return NULL;

    //int cchDestChar = StringLengthW( _wstrSource, _iMaxLength );

    WideCharToMultiByte( CP_ACP, 0, _wstrSource, -1, strDestination, _iMaxLength, NULL, NULL );
    //strDestination[cchDestChar] = 0;
    return strDestination;
}//ConvertWideStringToAnsi()


inline CHAR* ConvertGenericStringToAnsi(CHAR* strDestination, const TCHAR* _tstrSource, int _iMaxLength = MAX_PATH)
{
    if ( strDestination == NULL || _tstrSource == NULL )
        return NULL;

#ifdef _UNICODE
    ConvertWideStringToAnsi( strDestination, _tstrSource, _iMaxLength );
#else
    int cchDestChar = StringLength( _tstrSource, _iMaxLength );
    StringCchCopyA( strDestination, cchDestChar+1, _tstrSource );
    strDestination[cchDestChar] = 0;
#endif

    return strDestination;
}//ConvertGenericStringToAnsi()


inline WCHAR* ConvertGenericStringToWide(WCHAR* _wstrDestination, const TCHAR* _tstrSource, int _iMaxLength = MAX_PATH)
{
    if ( _wstrDestination == NULL || _tstrSource == NULL )
        return NULL;

#ifdef _UNICODE
    int cchDestChar = StringLength( _tstrSource, _iMaxLength );
	StringCchCopyW( _wstrDestination, cchDestChar+1, _tstrSource );
    _wstrDestination[cchDestChar] = L'\0';
#else
    ConvertAnsiStringToWide( _wstrDestination, _tstrSource, _iMaxLength );
#endif

    return _wstrDestination;
}//ConvertGenericStringToWide()


inline TCHAR* ConvertAnsiStringToGeneric(TCHAR* tstrDestination, const CHAR* _strSource, int _iMaxLength = MAX_PATH)
{
    if ( tstrDestination == NULL || _strSource == NULL )
        return NULL;
        
#ifdef _UNICODE
    ConvertAnsiStringToWide( tstrDestination, _strSource, _iMaxLength );
#else
    int cchDestChar = StringLengthA( _strSource, _iMaxLength );

    StringCchCopyA( tstrDestination, cchDestChar+1, _strSource );
    tstrDestination[cchDestChar] = '\0';
#endif

    return tstrDestination;
}//ConvertAnsiStringToGeneric()


inline TCHAR* ConvertWideStringToGeneric(TCHAR* tstrDestination, const WCHAR* _wstrSource, int _iMaxLength = MAX_PATH)
{
    if ( tstrDestination == NULL || _wstrSource == NULL )
        return NULL;

#ifdef _UNICODE
    int cchDestChar = StringLengthW( _wstrSource, _iMaxLength );
    StringCchCopyW( tstrDestination, cchDestChar+1, _wstrSource );
    tstrDestination[cchDestChar] = L'\0';
#else
    ConvertWideStringToAnsi( tstrDestination, _wstrSource, _iMaxLength );
#endif

    return tstrDestination;
}//ConvertWideStringToGeneric()


//{{ robobeg : 2006-09-04 // 23:23

inline HRESULT  ConvertStrStringToStdWString( std::wstring& _wstrDestination, const std::string& _strSource )
{
    int size = ::MultiByteToWideChar( CP_ACP, 0, _strSource.c_str(), (int) _strSource.size(), NULL, 0 );
    if ( size <= 0 )
        return  E_FAIL;
    
    WCHAR* wszBuffer = (WCHAR*) _alloca( ( size + 1 ) * sizeof(WCHAR) );
    ASSERT( wszBuffer != NULL );
    ::MultiByteToWideChar( CP_ACP, 0, _strSource.c_str(), (int) _strSource.size(), wszBuffer, size );
    wszBuffer[ size ] = L'\0';
    _wstrDestination = wszBuffer;
    return S_OK;
}//ConvertStdStringToStdWString()

inline  HRESULT ConvertWStringToStdString( std::string& _strDestination, const std::wstring& _wstrSource )
{
    int size = ::WideCharToMultiByte( CP_ACP, 0, _wstrSource.c_str(), (int) _wstrSource.size(), NULL, 0, NULL, NULL );
    if ( size <= 0 )
        return E_FAIL;

    CHAR*   szBuffer = (CHAR*) _alloca( ( size + 1 ) * sizeof( CHAR ) );
    ASSERT( szBuffer != NULL );
    ::WideCharToMultiByte( CP_ACP, 0, _wstrSource.c_str(), (int) _wstrSource.size(), szBuffer, size, NULL, NULL );
    szBuffer[ size ] = '\0';
    _strDestination = szBuffer;
    return S_OK;
}//ConvertStdWStringToStdString()

//}} robobeg : 2006-09-04 // 23:23


/// @param[in]      str = the input string that will be tokenized
/// @param[in/out]  result = the tokens for str
/// @param[in]      delimiters = the delimiter characters
/// @param[in]      delimiters preserve = same as above, but the delimiter characters
///                 will be put into the result as a token
/// @param[in]      quote = characters to protect the enclosed characters
/// @param[in]      esc = characters to protect a single character
/** @example    tokenize()

    // A string, which contents will be tokenized.
    std::string input;
    // define the characters that will break the string
    std::string delimiter = ",\t";  // use comma and  tab
    // define the characters that will break the string and generate
    // tokens themselves
    std::string keep_delim = ",";  // use semicolon and  colon
    // define the characters that will protect the enclosed text
    std::string quote = "\'\"";  // use single quote and double  quote
    // define the characters that will protect the following character
    std::string esc = "\\#";  // use backslash and the hash sign
    // vector that contains the tokens for input
    std::vector<std::string> tokens;

    input = "This;string,is for\tdemonstration.";
    tokenize( input, tokens, delimiter, keep_delim, quote, esc );

    // to use the tokens, define a token-iterator
    std::vector<std::string>::iterator token;
    // and simply iterate through the tokens
    for ( token = tokens.begin(); tokens.end() != token; ++token )
    {
        printf( "[%s]\n", token->c_str() );
    }//for
    // console output:
    //
    //[This;string]
    //[,]
    //[is for]
    //[demonstration.]
    //Press any key to continue

    delimiter = ";";
    input = "\"This;string;contains;quoted;text\";and;escaped\\;characters.";
    tokenize( input, tokens, delimiter, keep_delim, quote, esc );
    for ( token = tokens.begin(); tokens.end() != token; ++token )
    {
        printf( "[%s]\n", token->c_str() );
    }//for
    // console output:
    //
    //[This;string;contains;quoted;text]
    //[and]
    //[escaped;characters.]
    //Press any key to continue
*/
void tokenize(
    const std::string&          str,
    std::vector<std::string>&   result,
    const std::string&          delimiters,
    const std::string&          delimiters_preserve = "",
    const std::string&          quote               = "\"",
    const std::string&          esc                 = "\\");

#pragma warning(pop)

#endif // _KJOHNSTRUTIL_H


/** @example    KJohnStrUtil
    @code
    @endcode
*/
