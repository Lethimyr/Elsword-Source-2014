#pragma once

inline void MakeUpperCase(char* str) 
{
	for( char* i = str; (*i) != 0; i++ ) 
		*i = (char)toupper(*i);
}

inline void MakeUpperCase(std::string &str) 
{
	for( std::string::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (char)toupper(*i);
}

inline void MakeUpperCase(WCHAR* str) 
{
	for( WCHAR* i = str; (*i) != 0; i++ ) 
		*i = (WCHAR)towupper(*i);
}

inline void MakeUpperCase(std::wstring &str) 
{
	for( std::wstring::iterator i = str.begin(); i != str.end(); i++ ) 
		*i = (WCHAR)towupper(*i);
}

inline void MakeUpperCase(WCHAR* dest, size_t size, const WCHAR* str) 
{
	if ( dest == NULL || size == 0 )
		return;
	//KTDXPROFILE();
	WCHAR lower;
	size_t  count = 0;
	while( count + 1 < size && ( lower = (*str) ) != 0 )
	{
		*dest = (WCHAR)towupper(lower);
		dest++;
		str++;
	}//while
	*dest = 0;
}

inline std::wstring& ConvertCharToWCHAR( std::wstring& dest, const std::string& src )
{
	dest.resize( 0 );
	if( src.empty() )
		return dest;
	
	int iSrcLength = src.length();
	if ( iSrcLength <= 0 )
		return dest;

	int iDestSize = ::MultiByteToWideChar( CP_ACP, 0, src.c_str(), iSrcLength, NULL, 0 );
	if ( iDestSize <= 0 )
		return dest;

	wchar_t* wcharBuf = (wchar_t*) _malloca( iDestSize * sizeof( wchar_t ) );
	ASSERT( wcharBuf != NULL );
	int iNewSize = ::MultiByteToWideChar( CP_ACP, 0, src.c_str(), iSrcLength, wcharBuf, iDestSize );
	ASSERT( iNewSize == iDestSize );
	dest.assign( wcharBuf, iNewSize );
    _freea( wcharBuf );
    return  dest;
}



inline std::string& ConvertWCHARToChar( std::string& dest, const std::wstring& src )
{
	dest.resize( 0 );
	if( src.empty() )
		return dest; 

	int iSrcLength = src.length();
	if ( iSrcLength <= 0 )
		return dest; 

	int iDestSize = ::WideCharToMultiByte( CP_ACP, 0, src.c_str(), iSrcLength, NULL, 0, NULL, NULL );
	if ( iDestSize <= 0 )
		return dest; 

	char* pszConv = (char*) _malloca( iDestSize * sizeof(char) );
	ASSERT( pszConv != NULL );
	int iNewSize = ::WideCharToMultiByte( CP_ACP, 0, src.c_str(), iSrcLength, pszConv, iDestSize, NULL, NULL );
	ASSERT( iNewSize == iDestSize );
	dest.assign( pszConv, iNewSize );
    _freea( pszConv );
    return  dest;
}




inline std::wstring& ConvertCharToWCHAR( std::wstring& dest, const char* pSrc, const int iLength = -1  )
{
//{{ robobeg : 2008-01-08
	//if ( pSrc == NULL )
	//	return;

	//int iSize = strlen( pSrc );
	//if ( iSize <= 0 )
	//	return;

	//WCHAR* wcharBuf = (WCHAR*) alloca( (iSize + 1) * sizeof( WCHAR ) );
	//ZeroMemory( wcharBuf, (iSize + 1) * sizeof( WCHAR ) );

	//MultiByteToWideChar( CP_ACP, 0, pSrc, -1, wcharBuf, iSize );
	//dest = wcharBuf;

    dest.resize( 0 );
    if ( pSrc == NULL )
        return dest;

	int iSrcLength = iLength;
	if( iSrcLength < 0 )
	{
		iSrcLength = strlen( pSrc );
		if ( iSrcLength <= 0 )
			return dest;
	}

    int iDestSize = ::MultiByteToWideChar( CP_ACP, 0, pSrc, iSrcLength, NULL, 0 );
    if ( iDestSize <= 0 )
	    return dest;

	wchar_t* wcharBuf = (wchar_t*) _malloca( iDestSize * sizeof( wchar_t ) );
    ASSERT( wcharBuf != NULL );
    int iNewSize = ::MultiByteToWideChar( CP_ACP, 0, pSrc, iSrcLength, wcharBuf, iDestSize );
    ASSERT( iNewSize == iDestSize );
    dest.assign( wcharBuf, iNewSize );
    _freea( wcharBuf );
    return  dest;
//}} robobeg : 2008-01-08
}


inline std::string& ConvertWCHARToChar( std::string& dest, const wchar_t* pSrc, const int iLength = -1 )
{
//{{ robobeg : 2008-01-08
	//if ( pSrc == NULL )
	//	return;

	//int iSize = wcslen( pSrc ) * sizeof( WCHAR );
	//if ( iSize <= 0 )
	//	return;

	//char* charBuf = (char*) alloca( iSize + 1 );
	//ZeroMemory( charBuf, iSize + 1 );

	//WideCharToMultiByte( CP_ACP, 0, pSrc, -1, charBuf, iSize, NULL, NULL );
	//dest = charBuf;

    dest.resize( 0 );
    if ( pSrc == NULL )
        return dest;

	int iSrcLength = iLength;
	if( iSrcLength < 0 )
	{
		iSrcLength = wcslen( pSrc );
		if ( iSrcLength <= 0 )
			return dest;
	}

    int iDestSize = ::WideCharToMultiByte( CP_ACP, 0, pSrc, iSrcLength, NULL, 0, NULL, NULL );
    if ( iDestSize <= 0 )
        return dest;

    char* pszConv = (char*) _malloca( iDestSize * sizeof(char) );
    ASSERT( pszConv != NULL );
    int iNewSize = ::WideCharToMultiByte( CP_ACP, 0, pSrc, iSrcLength, pszConv, iDestSize, NULL, NULL );
    ASSERT( iNewSize == iDestSize );
    dest.assign( pszConv, iNewSize );
    _freea( pszConv );
    return dest;
//}} robobeg : 2008-01-08
}

inline std::wstring& ConvertUtf8ToWCHAR( std::wstring& dest, const char* pSrc, const int iLength = -1 )
{
    dest.resize( 0 );
    if( !pSrc )
        return dest;

    int iSrcLength = iLength;
    if( iSrcLength < 0 )
    {
        iSrcLength = strlen( pSrc );
        if ( iSrcLength <= 0 )
            return dest;
    }

    int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, NULL, 0 );
    if ( iDestSize <= 0 )
        return dest;

    wchar_t* wcharBuf = (wchar_t*) _malloca( iDestSize * sizeof( wchar_t ) );
    ASSERT( wcharBuf != NULL );
    int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, pSrc, iSrcLength, wcharBuf, iDestSize );
    ASSERT( iNewSize == iDestSize );
    dest.assign( wcharBuf, iNewSize );
    _freea( wcharBuf );
    return  dest;
}


inline std::wstring& ConvertUtf8ToWCHAR( std::wstring& dest, const std::string& src )
{
	dest.resize( 0 );
	if( src.empty() )
		return dest;
	
	int iSrcLength = src.length();
	if ( iSrcLength <= 0 )
		return dest;

	int iDestSize = ::MultiByteToWideChar( CP_UTF8, 0, src.c_str(), iSrcLength, NULL, 0 );
	if ( iDestSize <= 0 )
		return dest;

	wchar_t* wcharBuf = (wchar_t*) _malloca( iDestSize * sizeof( wchar_t ) );
	ASSERT( wcharBuf != NULL );
	int iNewSize = ::MultiByteToWideChar( CP_UTF8, 0, src.c_str(), iSrcLength, wcharBuf, iDestSize );
	ASSERT( iNewSize == iDestSize );
	dest.assign( wcharBuf, iNewSize );
    _freea( wcharBuf );
    return  dest;
}

