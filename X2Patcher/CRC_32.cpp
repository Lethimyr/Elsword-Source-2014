#include "CRC_32.h"
#include <iomanip>
#include <sstream>

CRC_32 CRC;

std::string CRC_32::GetCRCFromBuffer( const void* pvBuf, UINT uiSize )
{
    DWORD dwCRCCode = CRC.CalcCRC( pvBuf, uiSize );

    if ( 0 == dwCRCCode )
    {
        return std::string(); // failed
    }

    std::ostringstream ostrStream;
    ostrStream << std::resetiosflags( std::ios::uppercase )
               << std::hex << std::setw( 8 ) << std::setfill( '0' )
               << dwCRCCode;

    return ostrStream.str(); // succeeded
}

std::string CRC_32::GetCRCFromFile( const char* szFilePath )
{
    HANDLE  hFile       = INVALID_HANDLE_VALUE;
    HANDLE  hFileMap    = NULL;
    void*   pvFile      = NULL;

	WCHAR wcharBuf[255] = {0,};
	MultiByteToWideChar( CP_ACP, 0, szFilePath, -1, wcharBuf, 255 );

    if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFile( wcharBuf,
                                                         GENERIC_READ,
                                                         0,
                                                         NULL,
                                                         OPEN_EXISTING,
                                                         FILE_ATTRIBUTE_NORMAL,
                                                         NULL ) ) )
    {
        return std::string(); // failed
    }

    if ( NULL == ( hFileMap = ::CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, 0, NULL ) ) )
    {
        ::CloseHandle( hFile );

        return std::string(); // failed
    }

    if ( NULL == ( pvFile = ::MapViewOfFile( hFileMap, FILE_MAP_READ, 0, 0, 0 ) ) )
    {
        ::CloseHandle( hFileMap );
        ::CloseHandle( hFile );

        return std::string(); // failed
    }

    DWORD dwFileSize = ::GetFileSize( hFile, NULL );
    DWORD dwCRCCode = CRC.CalcCRC( pvFile, dwFileSize );
    std::ostringstream ostrStream;
    ostrStream << std::resetiosflags( std::ios::uppercase )
               << std::hex << std::setw( 8 ) << std::setfill( '0' )
               << dwCRCCode;

    ::UnmapViewOfFile( pvFile );
    ::CloseHandle( hFileMap );
    ::CloseHandle( hFile );

    return ostrStream.str(); // succeeded
}

std::string CRC_32::GetCRCFromString( const std::string& str )
{
    DWORD dwCRCCode = CRC.CalcCRC( str.c_str(), (int)str.size() );

    if ( 0 == dwCRCCode )
    {
        return std::string(); // failed
    }

    std::ostringstream ostrStream;
    ostrStream << std::resetiosflags( std::ios::uppercase )
        << std::hex << std::setw( 8 ) << std::setfill( '0' )
        << dwCRCCode;

    return ostrStream.str(); // succeeded
}