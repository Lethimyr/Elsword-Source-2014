#pragma once
#ifndef _KFILESERIALIZER_H
#define _KFILESERIALIZER_H

class   KFileSerializer
{
public:

    KFileSerializer() : m_iLockCounter(0), m_bSaveStringEndNull( false ) {}
    ~KFileSerializer() { ASSERT( m_iLockCounter == 0 ); }
    void            Clear()         { m_vecbBuffer.clear(); m_mapStringOffset.clear(); m_iLockCounter = 0;  }
    void            Reserve( DWORD dwInSize_ );
    DWORD           GetSize() const { return (DWORD) m_vecbBuffer.size(); }
    void            Swap( std::vector<BYTE>& vecbInOut_ );

    HRESULT         LoadFile( const char* pszInFilename_, DWORD dwInSize_ = 0 );
    HRESULT         SaveFile( const char* pszInFilename_ ) const;
    HRESULT         LoadFile( const wchar_t* pszInFilename_, DWORD dwInSize_ = 0 );
    HRESULT         SaveFile( const wchar_t* pszInFilename_ ) const;

    DWORD           AppendSpace( DWORD dwInSize_ );
    void            AlignSpace( DWORD dwInAlign_ = sizeof(DWORD) );
    DWORD           AppendData( DWORD dwInSize_, const void* pInData_ );
    DWORD           AppendString( bool bShort, bool bNoMap, const char* pszInChar_ );
	DWORD			AppendString( bool bShort, bool bNoMap, const char* pszInChar_, DWORD dwSize );
	DWORD			AppendString( bool bShort, bool bNoMap, const std::string& strIn_ );
	DWORD			AppendWideString( bool bShort, bool bNoMap, const wchar_t* pwszInChar_ );
	DWORD			AppendWideString( bool bShort, bool bNoMap, const wchar_t* pwszInChar_, DWORD dwSize );
	DWORD			AppendWideString( bool bShort, bool bNoMap, const std::wstring& wstrIn_ );
	
    void*           Lock()       { m_iLockCounter++; return &m_vecbBuffer[0]; }
    void            Unlock()    { ASSERT( m_iLockCounter > 0 ); m_iLockCounter--; }
    void*           AccessData( DWORD dwInOffset_, DWORD dwInSize_ );
    const void*     AccessData( DWORD dwInOffset_, DWORD dwuiInSize_ ) const;
	HRESULT			RetrieveData( void* pOutData_, DWORD dwInOffset_, DWORD dwInSize_ ) const;
    HRESULT         CopyData( DWORD dwInOffset_, DWORD dwInSize_, const void* pInData_ );

	bool			IsSaveStringEndNull() const			{ return m_bSaveStringEndNull; }
	void			SetSaveStringEndNull( bool bSave )	{ m_bSaveStringEndNull = bSave; }
private:
    std::vector<BYTE>   m_vecbBuffer;
	typedef stdext::hash_map<std::string,unsigned>	
						KStringOffsetMap;
	typedef stdext::hash_map<std::wstring,unsigned>	
						KWideStringOffsetMap;
	KStringOffsetMap	m_mapStringOffset;	
	KWideStringOffsetMap
						m_mapWideStringOffset;
	bool				m_bSaveStringEndNull;
    int                 m_iLockCounter;
};//class   KFileSerializer


class   KMemoryDeserializer
{
public:

    KMemoryDeserializer( DWORD dwInSize_, const void* pInData_ ) : m_dwSize( dwInSize_ ), m_pData( pInData_ ) { ASSERT( dwInSize_ == 0 || pInData_ != NULL ); }
    const void*     GetData() const { return m_pData; }
    DWORD           GetSize() const { return m_dwSize; }
    bool            CheckRange( DWORD dwInOffset_, DWORD dwInSize_ ) const { return dwInOffset_ + dwInSize_ <= m_dwSize; }
    HRESULT         RetrieveData( void* pOutData_, DWORD dwInOffset_, DWORD dwInSize_ ) const;
    char*           RetrieveDupString( DWORD dwInOffset_ ) const;
	const char*		RetrieveString( DWORD dwInOffset_, DWORD& size, bool& bNullEnded ) const;	
	const char*		RetrieveString_Short( DWORD dwInOffset_, WORD& size, bool& bNullEnded ) const;
	const wchar_t*	RetrieveWideString( DWORD dwInOffset_, DWORD& dwSize, bool& bNullEnded ) const;
	const wchar_t*	RetrieveWideString_Short( DWORD dwInOffset_, WORD& size, bool& bNullEnded ) const;
    const void*     AccessData( DWORD dwInOffset_, DWORD dwInSize_ ) const;

private:
    const void* m_pData;
    DWORD       m_dwSize;
};//class   KMemoryDeserializer


#endif // #ifndef _KFILESERIALIZER_H