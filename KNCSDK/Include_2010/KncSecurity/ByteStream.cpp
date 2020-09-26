#include "ByteStream.h"

#include <iomanip>

SERIALIZE_DEFINE_PUT( KncSecurity::KByteStream, obj, ks )
{
    return  ks.Put( (DWORD)obj.length() ) &&
            ks.PutRaw( obj.data(), obj.length() );
}

SERIALIZE_DEFINE_GET( KncSecurity::KByteStream, obj, ks )
{
    DWORD dwSize;
    ks.Get( dwSize );
    obj.resize( dwSize );
    ks.GetRaw( &obj[0], dwSize );

    return true;
}

NAMESPACE_BEGIN( KncSecurity )

ImplToStringA( KByteStream )
{
    stm_ << "KByteStream: size=" << length();
    if( empty() )
        return stm_ << " [ No Data. ]" << std::endl;

    stm_ << " [ ";
    stm_ << std::setprecision( 2 );
    std::basic_string<byte>::const_iterator sit;
    for( sit = begin(); sit != end(); sit++ )
    {
        stm_ << std::hex << std::setw( 2 ) << std::setfill( '0' ) << (const int)*sit << " ";
    }

    return stm_ << " ]" << std::dec;
}

NAMESPACE_END