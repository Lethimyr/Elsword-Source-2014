#pragma once

#include <string>
#include "SecureTypedef.h"
#include "ToString.h"
#include <Serializer/Serializer.h>

namespace KncSecurity
{
    class KByteStream;
}
SERIALIZE_DEFINE_TAG( KncSecurity::KByteStream, eTAG_USERCLASS );
SERIALIZE_DECLARE_PUTGET( KncSecurity::KByteStream );

NAMESPACE_BEGIN( KncSecurity )

class KByteStream : public std::basic_string<byte>
{
    DeclToStringA;

public:
    KByteStream() {}
    KByteStream( const unsigned char* pData, size_t nSize ) : std::basic_string<byte>( pData, nSize ) {}
    KByteStream( std::basic_string<byte>& kData ) : std::basic_string<byte>( kData ) {}
	virtual ~KByteStream() {}

    template <class T> void Assign( const T& objAssign, size_t nSize )
    {
        assign( reinterpret_cast< const unsigned char* >( &objAssign ), nSize );
    }

    template <class T> void Assign( const T& objAssign )
    {
        Assign( objAssign, sizeof(objAssign) );
    }

    template <class T> void Append( const T& objAppend, size_t nSize )
    {
        append( reinterpret_cast< const unsigned char* >( &objAppend ), nSize );
    }

    template <class T> void Append( const T& objAppend )
    {
        Append( objAppend, sizeof(objAppend) );
    }

    SERIALIZE_DECLARE_FRIEND( KByteStream );
};

DeclOstmOperatorA( KByteStream );

NAMESPACE_END