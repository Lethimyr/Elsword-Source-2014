#include <StdAfx.h>

static const char*	s_pcNullString = "";
static const wchar_t*	s_pwcNullString = L"";

void    KFileSerializer::Reserve( DWORD dwInSize_ )
{
    m_vecbBuffer.reserve( dwInSize_ );
}//KFileSerializer::Reserve()


void    KFileSerializer::Swap( std::vector<BYTE>& vecbInOut_ )
{
    vecbInOut_.swap( m_vecbBuffer );
    m_mapStringOffset.clear();
	m_mapWideStringOffset.clear();
}//TiXmlFileSerializer::Swap()

HRESULT KFileSerializer::LoadFile( const char* pszInFilename_, DWORD dwInSize_ )
{
    ASSERT( m_iLockCounter <= 0 );
    if ( m_iLockCounter > 0 )
        return E_FAIL;

    Clear();

    ASSERT( pszInFilename_ != NULL && pszInFilename_[0] != NULL );
    if ( !( pszInFilename_ != NULL && pszInFilename_[0] != NULL ) )
        return E_FAIL;

	HANDLE	hFile = ::CreateFileA( pszInFilename_, GENERIC_READ, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
        return E_FAIL;
    DWORD dwSize = ::GetFileSize( hFile, NULL );
    if ( dwSize == 0 || dwSize == INVALID_FILE_SIZE )
    {
        ::CloseHandle( hFile );
        return E_FAIL;
    }//if
    if ( dwInSize_ > 0 && dwSize > dwInSize_ )
        dwSize = dwInSize_;

    AppendSpace( dwSize );
    ::SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
    DWORD   dwReadBytes;
    BOOL bRead = ::ReadFile( hFile, &m_vecbBuffer[0], dwSize, &dwReadBytes, NULL );
    ASSERT( bRead && dwReadBytes == dwSize );
    if ( !bRead || dwReadBytes != dwSize )
    {
        ::CloseHandle( hFile );
        return E_FAIL;
    }//if
    ::CloseHandle( hFile );
    return S_OK;
}//KFileSerializer::LoadFile()

HRESULT         KFileSerializer::LoadFile( const wchar_t* pszInFilename_, DWORD dwInSize_ )
{
    ASSERT( m_iLockCounter <= 0 );
    if ( m_iLockCounter > 0 )
        return E_FAIL;

    Clear();

    ASSERT( pszInFilename_ != NULL && pszInFilename_[0] != NULL );
    if ( !( pszInFilename_ != NULL && pszInFilename_[0] != NULL ) )
        return E_FAIL;

	HANDLE	hFile = ::CreateFileW( pszInFilename_, GENERIC_READ, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
        return E_FAIL;
    DWORD dwSize = ::GetFileSize( hFile, NULL );
    if ( dwSize == 0 || dwSize == INVALID_FILE_SIZE )
    {
        ::CloseHandle( hFile );
        return E_FAIL;
    }//if
    if ( dwInSize_ > 0 && dwSize > dwInSize_ )
        dwSize = dwInSize_;

    AppendSpace( dwSize );
    ::SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
    DWORD   dwReadBytes;
    BOOL bRead = ::ReadFile( hFile, &m_vecbBuffer[0], dwSize, &dwReadBytes, NULL );
    ASSERT( bRead && dwReadBytes == dwSize );
    if ( !bRead || dwReadBytes != dwSize )
    {
        ::CloseHandle( hFile );
        return E_FAIL;
    }//if
    ::CloseHandle( hFile );
    return S_OK;
}


HRESULT         KFileSerializer::SaveFile( const wchar_t* pszInFilename_ ) const
{
    ASSERT( pszInFilename_ != NULL && pszInFilename_[0] != NULL );
    if ( !( pszInFilename_ != NULL && pszInFilename_[0] != NULL ) )
        return E_FAIL;

    DWORD   dwSize = GetSize();
    if ( dwSize == 0 )
        return E_FAIL;

    HANDLE hFile = ::CreateFileW( pszInFilename_, GENERIC_WRITE, 0
        , NULL, CREATE_ALWAYS, 0, NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
        return E_FAIL;

    DWORD   dwNumWrites = 0;
    BOOL bWrite = ::WriteFile( hFile, &m_vecbBuffer[0], dwSize, &dwNumWrites, NULL );
    ASSERT( bWrite && dwNumWrites == dwSize );
    if ( !( bWrite && dwNumWrites == dwSize ) )
    {
        ::CloseHandle( hFile );
        return E_FAIL;
    }//if
    ::CloseHandle( hFile );
    return S_OK;
}



HRESULT  KFileSerializer::SaveFile( const char* pszInFilename_ ) const
{
    ASSERT( pszInFilename_ != NULL && pszInFilename_[0] != NULL );
    if ( !( pszInFilename_ != NULL && pszInFilename_[0] != NULL ) )
        return E_FAIL;

    DWORD   dwSize = GetSize();
    if ( dwSize == 0 )
        return E_FAIL;

    HANDLE hFile = ::CreateFileA( pszInFilename_, GENERIC_WRITE, 0
        , NULL, CREATE_ALWAYS, 0, NULL );
    if ( hFile == INVALID_HANDLE_VALUE )
        return E_FAIL;

    DWORD   dwNumWrites = 0;
    BOOL bWrite = ::WriteFile( hFile, &m_vecbBuffer[0], dwSize, &dwNumWrites, NULL );
    ASSERT( bWrite && dwNumWrites == dwSize );
    if ( !( bWrite && dwNumWrites == dwSize ) )
    {
        ::CloseHandle( hFile );
        return E_FAIL;
    }//if
    ::CloseHandle( hFile );
    return S_OK;
}//KFileSerializer::SaveFile()


DWORD   KFileSerializer::AppendSpace( DWORD dwInSize_ )
{
    ASSERT( m_iLockCounter <= 0 );
    DWORD   dwSize = GetSize();
    if ( dwInSize_ == 0 )
        return NULL;
    m_vecbBuffer.resize( dwSize + dwInSize_ );
    return  dwSize;
}//KFileSerializer::AppendSpace()


void    KFileSerializer::AlignSpace( DWORD dwInAlign_ )
{
    if( dwInAlign_ <= 1 )
        return;
    DWORD	dwOffset = GetSize();
    if( ( dwOffset % dwInAlign_ ) == 0 )
        return;
    DWORD	dwNewSize = ( dwOffset + dwInAlign_ ) / dwInAlign_ * dwInAlign_;
    m_vecbBuffer.resize( dwNewSize );
    ZeroMemory( &m_vecbBuffer[ dwOffset ], dwNewSize - dwOffset );
}//KFileSerializer::AlignSpace()



DWORD   KFileSerializer::AppendData( DWORD dwInSize_, const void* pInData_ )
{
    ASSERT( m_iLockCounter <= 0 );
    DWORD   dwSize = GetSize();
    if ( dwInSize_ == 0 )
        return NULL;
    ASSERT( pInData_ != NULL );
    m_vecbBuffer.resize( dwSize + dwInSize_ );
    CopyMemory( &m_vecbBuffer[dwSize], pInData_, dwInSize_ );


    return  dwSize;
}//KFileSerializer::AppendData()


DWORD   KFileSerializer::AppendString( bool bShort, bool bNoMap, const char* pszInChar_ )
{
    ASSERT( m_iLockCounter <= 0 );

    if ( pszInChar_ == NULL )
        return 0;
    DWORD   dwLength = (DWORD) strlen( pszInChar_ );
    if( dwLength == 0 )
        return 0;

    DWORD   dwOffset = GetSize();

	if ( bNoMap == false )
	{
		std::pair<KStringOffsetMap::iterator, bool> insertResult
			= m_mapStringOffset.insert( KStringOffsetMap::value_type( pszInChar_, dwOffset ) );
		if( !insertResult.second )
		{
			return insertResult.first->second;
		}//if
	}
	
	if ( bShort == false )
	{
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength + 1 );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pszInChar_, dwLength );
			m_vecbBuffer[dwOffset+sizeof(DWORD) + dwLength] = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pszInChar_, dwLength );
		}//if.. else..
	}
	else
	{
		WORD wLength = (WORD) dwLength;
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength + 1 );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pszInChar_, wLength );
			m_vecbBuffer[dwOffset+sizeof(WORD) + wLength] = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pszInChar_, wLength );
		}//if.. else..
	}

    return  dwOffset;
}//KFileSerializer::AppendString()


DWORD   KFileSerializer::AppendString( bool bShort, bool bNoMap, const char* pszInChar_, DWORD dwLength )
{
    ASSERT( m_iLockCounter <= 0 );

    if ( pszInChar_ == NULL || dwLength == 0 )
        return 0;

    DWORD   dwOffset = GetSize();

	if ( bNoMap == false )
	{
		std::pair<KStringOffsetMap::iterator, bool> insertResult
			= m_mapStringOffset.insert( KStringOffsetMap::value_type( std::string( pszInChar_, dwLength ), dwOffset ) );
		if( !insertResult.second )
		{
			return insertResult.first->second;
		}//if
	}
	
	if ( bShort == false )
	{
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength + 1 );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pszInChar_, dwLength );
			m_vecbBuffer[dwOffset+sizeof(DWORD) + dwLength] = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pszInChar_, dwLength );
		}//if.. else..
	}
	else
	{
		WORD wLength = (WORD) dwLength;
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength + 1 );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pszInChar_, wLength );
			m_vecbBuffer[dwOffset+sizeof(WORD) + wLength] = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pszInChar_, wLength );
		}//if.. else..
	}

    return  dwOffset;
}//KFileSerializer::AppendString()


DWORD   KFileSerializer::AppendString( bool bShort, bool bNoMap, const std::string& strIn_ )
{
    ASSERT( m_iLockCounter <= 0 );

    DWORD   dwLength = strIn_.size();
    if( dwLength == 0 )
        return 0;

    DWORD   dwOffset = GetSize();

	if ( bNoMap == false )
	{
		std::pair<KStringOffsetMap::iterator, bool> insertResult
			= m_mapStringOffset.insert( KStringOffsetMap::value_type( strIn_, dwOffset ) );
		if( !insertResult.second )
		{
			return insertResult.first->second;
		}//if
	}
	
	if ( bShort == false )
	{
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength + 1 );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], strIn_.c_str(), dwLength );
			m_vecbBuffer[dwOffset+sizeof(DWORD) + dwLength] = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], strIn_.c_str(), dwLength );
		}//if.. else..
	}
	else
	{
		WORD wLength = (WORD) dwLength;
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength + 1 );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], strIn_.c_str(), wLength );
			m_vecbBuffer[dwOffset+sizeof(WORD) + wLength] = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], strIn_.c_str(), wLength );
		}//if.. else..
	}

    return  dwOffset;
}//KFileSerializer::AppendString()

DWORD   KFileSerializer::AppendWideString( bool bShort, bool bNoMap, const wchar_t* pwszInChar_ )
{
    ASSERT( m_iLockCounter <= 0 );

    if ( pwszInChar_ == NULL )
        return 0;
    DWORD   dwLength = (DWORD) wcslen( pwszInChar_ );
    if( dwLength == 0 )
        return 0;

	AlignSpace( sizeof(wchar_t) );

    DWORD   dwOffset = GetSize();

	if ( bNoMap == false )
	{
		std::pair<KWideStringOffsetMap::iterator, bool> insertResult
			= m_mapWideStringOffset.insert( KWideStringOffsetMap::value_type( pwszInChar_, dwOffset ) );
		if( !insertResult.second )
		{
			return insertResult.first->second;
		}//if
	}
	
	if ( bShort == false )
	{
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + (dwLength + 1)*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pwszInChar_, dwLength*sizeof(wchar_t) );
			*( (wchar_t*) &m_vecbBuffer[dwOffset+sizeof(DWORD)+dwLength*sizeof(wchar_t) ] ) = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength * sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pwszInChar_, dwLength*sizeof(wchar_t) );
		}//if.. else..
	}
	else
	{
		WORD wLength = (WORD) dwLength;
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + (wLength + 1)*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pwszInChar_, wLength*sizeof(wchar_t) );
			*( (wchar_t*) &m_vecbBuffer[dwOffset+sizeof(WORD) + wLength*sizeof(wchar_t)] ) = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pwszInChar_, wLength*sizeof(wchar_t) );
		}//if.. else..
	}

    return  dwOffset;
}//KFileSerializer::AppendWideString()


DWORD   KFileSerializer::AppendWideString( bool bShort, bool bNoMap, const wchar_t* pwszInChar_, DWORD dwLength )
{
    ASSERT( m_iLockCounter <= 0 );

    if ( pwszInChar_ == NULL || dwLength == 0 )
        return 0;

	AlignSpace( sizeof(wchar_t) );

    DWORD   dwOffset = GetSize();

	if ( bNoMap == false )
	{
		std::pair<KWideStringOffsetMap::iterator, bool> insertResult
			= m_mapWideStringOffset.insert( 
			KWideStringOffsetMap::value_type( std::wstring( pwszInChar_, dwLength ), dwOffset ) );
		if( !insertResult.second )
		{
			return insertResult.first->second;
		}//if
	}
	
	if ( bShort == false )
	{
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + (dwLength + 1)*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pwszInChar_, dwLength*sizeof(wchar_t) );
			*( (wchar_t*) &m_vecbBuffer[dwOffset+sizeof(DWORD)+dwLength*sizeof(wchar_t) ] ) = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength * sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], pwszInChar_, dwLength*sizeof(wchar_t) );
		}//if.. else..
	}
	else
	{
		WORD wLength = (WORD) dwLength;
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + (wLength + 1)*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pwszInChar_, wLength*sizeof(wchar_t) );
			*( (wchar_t*) &m_vecbBuffer[dwOffset+sizeof(WORD) + wLength*sizeof(wchar_t)] ) = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], pwszInChar_, wLength*sizeof(wchar_t) );
		}//if.. else..
	}

    return  dwOffset;
}//KFileSerializer::AppendWideString()


DWORD   KFileSerializer::AppendWideString( bool bShort, bool bNoMap, const std::wstring& wstrIn_ )
{
    ASSERT( m_iLockCounter <= 0 );


    DWORD   dwLength = wstrIn_.size();
    if( dwLength == 0 )
        return 0;
	AlignSpace( sizeof(wchar_t) );

    DWORD   dwOffset = GetSize();

	if ( bNoMap == false )
	{
		std::pair<KWideStringOffsetMap::iterator, bool> insertResult
			= m_mapWideStringOffset.insert( 
			KWideStringOffsetMap::value_type( wstrIn_, dwOffset ) );
		if( !insertResult.second )
		{
			return insertResult.first->second;
		}//if
	}

	
	if ( bShort == false )
	{
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + (dwLength + 1)*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], wstrIn_.c_str(), dwLength*sizeof(wchar_t) );
			*( (wchar_t*) &m_vecbBuffer[dwOffset+sizeof(DWORD)+dwLength*sizeof(wchar_t) ] ) = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(DWORD) + dwLength * sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &dwLength, sizeof(DWORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(DWORD)], wstrIn_.c_str(), dwLength*sizeof(wchar_t) );
		}//if.. else..
	}
	else
	{
		WORD wLength = (WORD) dwLength;
		if ( m_bSaveStringEndNull )
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + (wLength + 1)*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], wstrIn_.c_str(), wLength*sizeof(wchar_t) );
			*( (wchar_t*) &m_vecbBuffer[dwOffset+sizeof(WORD) + wLength*sizeof(wchar_t)] ) = 0;
		}
		else
		{
			m_vecbBuffer.resize( dwOffset + sizeof(WORD) + wLength*sizeof(wchar_t) );
			CopyMemory( &m_vecbBuffer[dwOffset], &wLength, sizeof(WORD) );
			CopyMemory( &m_vecbBuffer[dwOffset+sizeof(WORD)], wstrIn_.c_str(), wLength*sizeof(wchar_t) );
		}//if.. else..
	}

    return  dwOffset;
}//KFileSerializer::AppendString()



void*   KFileSerializer::AccessData(DWORD dwInOffset_, DWORD dwInSize_ )
{
    ASSERT( m_iLockCounter <= 0 );
    ASSERT( dwInOffset_ + dwInSize_ <= GetSize() );
    if ( dwInOffset_ + dwInSize_ > GetSize() )
        return NULL;
    return  &m_vecbBuffer[dwInOffset_];
}//KFileSerializer::AccessSpace()
const void* KFileSerializer::AccessData( DWORD dwInOffset_, DWORD dwInSize_ ) const
{
    ASSERT( m_iLockCounter <= 0 );
    ASSERT( dwInOffset_ + dwInSize_ <= GetSize() );
    if ( dwInOffset_ + dwInSize_ > GetSize() )
        return NULL;
    return  &m_vecbBuffer[dwInOffset_];

}//KFileSerializer::AccessData()


HRESULT	KFileSerializer::RetrieveData( void* pOutData_, DWORD dwInOffset_, DWORD dwInSize_ ) const
{
    const void* pData = AccessData( dwInOffset_, dwInSize_ );
    if( pData == 0 )
        return E_FAIL;
    CopyMemory( pOutData_, pData, dwInSize_ );
    return S_OK;
}//KFileSerializer::RetrieveData()

HRESULT KFileSerializer::CopyData( DWORD dwInOffset_, DWORD dwInSize_, const void* pInData_ )
{
    ASSERT( dwInOffset_ + dwInSize_ <= GetSize() );
    if ( dwInOffset_ + dwInSize_ > GetSize() )
        return E_FAIL;
    if ( dwInSize_ == 0 )
        return S_OK;
    ASSERT( pInData_ != NULL );
    CopyMemory( &m_vecbBuffer[dwInOffset_], pInData_, dwInSize_ );

    return S_OK;
}//KFileSerializer::CopyData()



const void* KMemoryDeserializer::AccessData( DWORD dwInOffset_, DWORD dwInSize_ ) const
{
    if ( !CheckRange( dwInOffset_, dwInSize_ ) )
        return NULL;
    return  &static_cast<const BYTE*>(m_pData)[ dwInOffset_ ];
}//KMemoryDeserializer::AccessData()


HRESULT     KMemoryDeserializer::RetrieveData( void* pOutData_, DWORD dwInOffset_, DWORD dwInSize_ ) const
{
    const void* pData = AccessData( dwInOffset_, dwInSize_ );
    if ( pData == NULL )
        return E_FAIL;
    CopyMemory( pOutData_, pData, dwInSize_ );
    return S_OK;
}//KMemoryDeserializer::RetrieveData()


char*       KMemoryDeserializer::RetrieveDupString( DWORD dwInOffset_ ) const
{
    const void* pData = AccessData( dwInOffset_, sizeof(DWORD) );
    if ( pData == NULL )
        return NULL;
    DWORD   dwSize = *static_cast<const DWORD*>( pData );
    const void* pString = AccessData( dwInOffset_ + sizeof(DWORD), dwSize );
    if ( pString == NULL )
        return NULL;
    char* pszOut = new char[ dwSize + 1 ];
    ASSERT( pszOut != NULL );
    CopyMemory( pszOut, pString, dwSize );
    pszOut[ dwSize ] = 0;
    return  pszOut;
}//KMemoryDeserializer::RetrieveDupString

const char*	KMemoryDeserializer::RetrieveString( DWORD dwInOffset_, DWORD& size, bool& bNullEnded ) const
{
	if ( dwInOffset_ == 0 )
	{
		size = 0;
		bNullEnded = true;
		return	s_pcNullString;
	}
    DWORD	dwLength;
    HRESULT	hr = RetrieveData( &dwLength, dwInOffset_, sizeof(DWORD) );
    if( FAILED( hr ) )
	{
		size = 0;
		bNullEnded = true;
        return s_pcNullString;
	}
    if( dwLength == 0 )
	{
		size = 0;
		bNullEnded = true;
        return s_pcNullString;
	}
	const char* pcString = (const char*) AccessData( dwInOffset_ + sizeof(DWORD), dwLength + 1 );
	if ( pcString == NULL )
	{
		pcString = (const char*) AccessData( dwInOffset_ + sizeof(DWORD), dwLength );
		if ( pcString == NULL )
		{
			size = 0;
			bNullEnded = true;
			return s_pcNullString;
		}//if
		size = (unsigned) dwLength;
		bNullEnded = false;
		return	pcString;
	}//if
	size = (DWORD) dwLength;
	bNullEnded = ( pcString[dwLength] == 0 ) ? true : false;
	return	pcString;
}

const char*	KMemoryDeserializer::RetrieveString_Short( DWORD dwInOffset_, WORD& size, bool& bNullEnded ) const
{
	if ( dwInOffset_ == 0 )
	{
		size = 0;
		bNullEnded = true;
		return	s_pcNullString;
	}
    WORD	wLength;
    HRESULT	hr = RetrieveData( &wLength, dwInOffset_, sizeof(WORD) );
    if( FAILED( hr ) )
	{
		size = 0;
		bNullEnded = true;
        return s_pcNullString;
	}
    if( wLength == 0 )
	{
		size = 0;
		bNullEnded = true;
        return s_pcNullString;
	}
	const char* pcString = (const char*) AccessData( dwInOffset_ + sizeof(WORD), wLength + 1 );
	if ( pcString == 0 )
	{
		pcString = (const char*) AccessData( dwInOffset_ + sizeof(WORD), wLength );
		if ( pcString == 0 )
		{
			size = 0;
			bNullEnded = true;
			return s_pcNullString;
		}//if
		size = (WORD) wLength;
		bNullEnded = false;
		return	pcString;
	}//if
	size = (WORD) wLength;
	bNullEnded = ( pcString[wLength] == 0 ) ? true : false;
	return	pcString;
}

const wchar_t*	KMemoryDeserializer::RetrieveWideString( DWORD dwInOffset_, DWORD& size, bool& bNullEnded ) const
{
	if ( dwInOffset_ == 0 )
	{
		size = 0;
		bNullEnded = true;
		return	s_pwcNullString;
	}
    DWORD	dwLength;
    HRESULT	hr = RetrieveData( &dwLength, dwInOffset_, sizeof(DWORD) );
    if( FAILED( hr ) )
	{
		size = 0;
		bNullEnded = true;
        return s_pwcNullString;
	}
    if( dwLength == 0 )
	{
		size = 0;
		bNullEnded = true;
        return s_pwcNullString;
	}
	const wchar_t* pwcString = (const wchar_t*) AccessData( dwInOffset_ + sizeof(DWORD), (dwLength + 1)*sizeof(wchar_t) );
	if ( pwcString == NULL )
	{
		pwcString = (const wchar_t*) AccessData( dwInOffset_ + sizeof(DWORD), dwLength*sizeof(wchar_t) );
		if ( pwcString == NULL )
		{
			size = 0;
			bNullEnded = true;
			return s_pwcNullString;
		}//if
		size = (DWORD) dwLength;
		bNullEnded = false;
		return	pwcString;
	}//if
	size = (DWORD) dwLength;
	bNullEnded = ( pwcString[dwLength] == 0 ) ? true : false;
	return	pwcString;
}

const wchar_t*	KMemoryDeserializer::RetrieveWideString_Short( DWORD dwInOffset_, WORD& size, bool& bNullEnded ) const
{
	if ( dwInOffset_ == 0 )
	{
		size = 0;
		bNullEnded = true;
		return	s_pwcNullString;
	}
    WORD	wLength;
    HRESULT	hr = RetrieveData( &wLength, dwInOffset_, sizeof(WORD) );
    if( FAILED( hr ) )
	{
		size = 0;
		bNullEnded = true;
        return s_pwcNullString;
	}
    if( wLength == 0 )
	{
		size = 0;
		bNullEnded = true;
        return s_pwcNullString;
	}
	const wchar_t* pwcString = (const wchar_t*) AccessData( dwInOffset_ + sizeof(WORD),(wLength + 1)*sizeof(wchar_t) );
	if ( pwcString == 0 )
	{
		pwcString = (const wchar_t*) AccessData( dwInOffset_ + sizeof(WORD), wLength*sizeof(wchar_t) );
		if ( pwcString == 0 )
		{
			size = 0;
			bNullEnded = true;
			return s_pwcNullString;
		}//if
		size = (WORD) wLength;
		bNullEnded = false;
		return	pwcString;
	}//if
	size = (WORD) wLength;
	bNullEnded = ( pwcString[wLength] == 0 ) ? true : false;
	return	pwcString;
}
