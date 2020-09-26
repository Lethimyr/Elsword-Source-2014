#undef max
#include <boost/random.hpp>
#include <Thread/Locker.h>

#include "KncUidType.h"

#if defined( SERV_COUNTRY_CN ) || defined( SERV_COUNTRY_US )
	#define EXTEND_SERVER_GROUP_MASK
#endif //defined( SERV_COUNTRY_CN ) || defined( SERV_COUNTRY_US )

namespace KncUid {

    typedef unsigned int DWORD;

    boost::mt19937 rng;
    boost::uniform_int<DWORD> uint32( 1, UINT_MAX );
#ifdef EXTEND_SERVER_GROUP_MASK
	boost::uniform_int<UidType> uint40( 1, 0x0000000fffffffff );
#else // EXTEND_SERVER_GROUP_MASK
    boost::uniform_int<UidType> uint40( 1, 0x000000ffffffffff );
#endif // EXTEND_SERVER_GROUP_MASK
    boost::variate_generator<boost::mt19937&, boost::uniform_int<DWORD> > die32(rng, uint32);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<UidType> > die40(rng, uint40);

    KncCriticalSection m_cs;

    DWORD Die32()           { KLocker lock( m_cs ); return die32(); }
    UidType GetTempUID() 
    { 
        KLocker lock( m_cs ); 
        return die40() | (UidType)0x1 << 62;
    }

    UidType ExtractPureUID( UidType iUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
        return iUID_ & ( UidType )0x0000000fffffffff;
#else // EXTEND_SERVER_GROUP_MASK
        return iUID_ & ( UidType )0x000000ffffffffff;
#endif // EXTEND_SERVER_GROUP_MASK
    }

    UidType ExtractServerGroupID( UidType iUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
		return ( iUID_ >> 52 ) & ( UidType )0x00000000000003ff;
#else // EXTEND_SERVER_GROUP_MASK
        return ( iUID_ >> 56 ) & ( UidType )0x000000000000003f;
#endif // EXTEND_SERVER_GROUP_MASK
    }

    UidType ExtractServerID( UidType iUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
		return ( iUID_ >> 44 ) & ( UidType )0x00000000000000ff;
#else // EXTEND_SERVER_GROUP_MASK
        return ( iUID_ >> 48 ) & ( UidType )0x00000000000000ff;
#endif // EXTEND_SERVER_GROUP_MASK
    }

    UidType ExtractReservedID( UidType iUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
		return ( iUID_ >> 36 ) & ( UidType )0x00000000000000ff;
#else // EXTEND_SERVER_GROUP_MASK
        return ( iUID_ >> 40 ) & ( UidType )0x00000000000000ff;
#endif // EXTEND_SERVER_GROUP_MASK
    }

	//{{ 2010. 01. 05  최육사	DestUID값을 임시발급UID로 만듭니다.
	void SetTempUID( UidType& iDestUID_ )
	{
		iDestUID_ = ( iDestUID_ & 0xBfffffffffffffff ) | ( ( ( UidType )0x1 << 62 ) & 0x4000000000000000 );
	}
	//}}

    void SetPureUID( UidType& iDestUID_, UidType iSrcUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
		iDestUID_ = ( iDestUID_ & 0xfffffff000000000 ) | ( iSrcUID_ & ( UidType )0x0000000fffffffff );
#else // EXTEND_SERVER_GROUP_MASK
        iDestUID_ = ( iDestUID_ & 0xffffff0000000000 ) | ( iSrcUID_ & ( UidType )0x000000ffffffffff );
#endif // EXTEND_SERVER_GROUP_MASK
    }

    void SetServerGroupID( UidType& iDestUID_, UidType iSrcUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
		iDestUID_ = ( iDestUID_ & 0xC00fffffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x00000000000003ff ) << 52 ) & 0x3ff0000000000000 );
#else // EXTEND_SERVER_GROUP_MASK
        iDestUID_ = ( iDestUID_ & 0xC0ffffffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x000000000000003f ) << 56 ) & 0x3f00000000000000 );
#endif // EXTEND_SERVER_GROUP_MASK
    }

    void SetServerID( UidType& iDestUID_, UidType iSrcUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
		iDestUID_ = ( iDestUID_ & 0xfff00fffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x00000000000000ff ) << 44 ) & 0x000ff00000000000 );
#else // EXTEND_SERVER_GROUP_MASK
        iDestUID_ = ( iDestUID_ & 0xff00ffffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x00000000000000ff ) << 48 ) & 0x00ff000000000000 );
#endif // EXTEND_SERVER_GROUP_MASK
    }

    void SetReservedID( UidType& iDestUID_, UidType iSrcUID_ )
    {
#ifdef EXTEND_SERVER_GROUP_MASK
		iDestUID_ = ( iDestUID_ & 0xfffff00fffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x00000000000000ff ) << 36 ) & 0x00000ff000000000 );
#else // EXTEND_SERVER_GROUP_MASK
		iDestUID_ = ( iDestUID_ & 0xffff00ffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x00000000000000ff ) << 40 ) & 0x0000ff0000000000 );
#endif // EXTEND_SERVER_GROUP_MASK
    }

	//serverid & reservedid 통합.
	UidType ExtractCodeID(UidType iUID_)
	{
#ifdef EXTEND_SERVER_GROUP_MASK
		return ( iUID_ >> 36 ) & (UidType)0x000000000000ffff;
#else // EXTEND_SERVER_GROUP_MASK
		return ( iUID_ >> 40 ) & (UidType)0x000000000000ffff;
#endif // EXTEND_SERVER_GROUP_MASK
	}
	void SetCodeID( UidType& iDestUID_, UidType iSrcUID_ )
	{
#ifdef EXTEND_SERVER_GROUP_MASK
		iDestUID_ = ( iDestUID_ & 0xfff0000fffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x000000000000ffff ) << 36 ) & 0x000ffff000000000 );
#else // EXTEND_SERVER_GROUP_MASK
		iDestUID_ = ( iDestUID_ & 0xff0000ffffffffff ) | ( ( ( iSrcUID_ & ( UidType )0x000000000000ffff ) << 40 ) & 0x00ffff0000000000 );
#endif // EXTEND_SERVER_GROUP_MASK
	}
}




