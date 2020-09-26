/** @file   KSmartNull.h
    @desc   interface of KSmartNull
    @author robobeg@kogstudios.com
*/
#pragma once

namespace   kog
{
    struct KSmartNull
    {
        bool            operator==( KSmartNull ) const { return true; }
        bool            operator != ( KSmartNull ) const { return false; }
    };//struct KSmartNull

    struct KReserveTag
    {
    };//struct KReserveTag

    extern KSmartNull  null;
    extern KReserveTag reserve;

    template<typename A>
    void                nullify( A& );

    template<typename A>
    void                nullify( A&, KReserveTag );
}//namespace   kog

#define _KSMARTNULL_COMPARISON_NO_PARAM( CONTAINER, EMPTYFN) \
    class   CONTAINER; \
    inline bool operator==( const CONTAINER& In_, kog::KSmartNull ) \
    { \
        return ( EMPTYFN ); \
    } \
    inline bool operator==( kog::KSmartNull, const CONTAINER& In_ ) \
    { \
        return ( EMPTYFN ); \
    } \
    inline bool operator != ( const CONTAINER& In_, kog::KSmartNull ) \
    { \
        return !( EMPTYFN ); \
    } \
    inline bool operator != ( kog::KSmartNull, const CONTAINER& In_ ) \
    { \
        return !( EMPTYFN ); \
    }

#define _KSMARTNULL_NULLIFY_NO_PARAM( CONTAINER, NULLIFY ) \
    void    nullify( CONTAINER& Out_ ) \
    { \
        NULLIFY; \
    }

#define _KSMARTNULL_NULLIFY_RESERVE_NO_PARAM( CONTAINER, NULLIFY, NULLIFY_RESERVE ) \
    void    nullify( CONTAINER& Out_ ) \
    { \
        NULLIFY; \
    } \
    void    nullify( CONTAINER& Out_, kog::KReserveTag ) \
    { \
        NULLIFY_RESERVE; \
    }



#define _KSMARTNULL_COMPARISON_ONE_PARAM( CONTAINER, TYPE1, EMPTYFN) \
    template < TYPE1 PARAM1 > \
    class   CONTAINER; \
    template < TYPE1 PARAM1 > \
    inline bool operator==( const CONTAINER< PARAM1 >& In_, kog::KSmartNull ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 > \
    inline bool operator==( kog::KSmartNull, const CONTAINER< PARAM1 >& In_ ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 > \
    inline bool operator != ( const CONTAINER< PARAM1 >& In_, kog::KSmartNull ) \
    { \
        return !( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 > \
    inline bool operator != ( kog::KSmartNull, const CONTAINER< PARAM1 >& In_ ) \
    { \
        return !( EMPTYFN ); \
    }

#define _KSMARTNULL_NULLIFY_ONE_PARAM( CONTAINER, TYPE1, NULLIFY ) \
    template < TYPE1 PARAM1 > \
    void    nullify( CONTAINER< PARAM1 >& Out_ ) \
    { \
        NULLIFY; \
    }

#define _KSMARTNULL_NULLIFY_RESERVE_ONE_PARAM( CONTAINER, TYPE1, NULLIFY, NULLIFY_RESERVE ) \
    template < TYPE1 PARAM1 > \
    void    nullify( CONTAINER< PARAM1 >& Out_ ) \
    { \
        NULLIFY; \
    } \
    template < TYPE1 PARAM1 > \
    void    nullify( CONTAINER< PARAM1 >& Out_, kog::KReserveTag ) \
    { \
        NULLIFY_RESERVE; \
    }



#define _KSMARTNULL_COMPARISON_TWO_PARAMS( CONTAINER, TYPE1, TYPE2, EMPTYFN) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    class   CONTAINER; \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    inline bool operator==( const CONTAINER< PARAM1 , PARAM2 >& In_, kog::KSmartNull ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    inline bool operator==( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 >& In_ ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    inline bool operator != ( const CONTAINER< PARAM1 , PARAM2 >& In_, kog::KSmartNull ) \
    { \
        return !( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    inline bool operator != ( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 >& In_ ) \
    { \
        return !( EMPTYFN ); \
    }

#define _KSMARTNULL_NULLIFY_TWO_PARAMS( CONTAINER, TYPE1, TYPE2, NULLIFY ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 >& Out_ ) \
    { \
        NULLIFY; \
    }

#define _KSMARTNULL_NULLIFY_RESERVE_TWO_PARAMS( CONTAINER, TYPE1, TYPE2, NULLIFY, NULLIFY_RESERVE ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 >& Out_ ) \
    { \
        NULLIFY; \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 >& Out_, kog::KReserveTag ) \
    { \
        NULLIFY_RESERVE; \
    }


#define _KSMARTNULL_COMPARISON_THREE_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, EMPTYFN) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    class   CONTAINER; \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    inline bool operator==( const CONTAINER< PARAM1 , PARAM2 , PARAM3 >& In_, kog::KSmartNull ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    inline bool operator==( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 >& In_ ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    inline bool operator != ( const CONTAINER< PARAM1 , PARAM2 , PARAM3 >& In_, kog::KSmartNull ) \
    { \
        return !( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    inline bool operator != ( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 >& In_ ) \
    { \
        return !( EMPTYFN ); \
    }

#define _KSMARTNULL_NULLIFY_THREE_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, NULLIFY ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 >& Out_ ) \
    { \
        NULLIFY; \
    }

#define _KSMARTNULL_NULLIFY_RESERVE_THREE_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, NULLIFY, NULLIFY_RESERVE ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 >& Out_ ) \
    { \
        NULLIFY; \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 >& Out_, kog::KReserveTag ) \
    { \
        NULLIFY_RESERVE; \
    }


#define _KSMARTNULL_COMPARISON_FOUR_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, EMPTYFN) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    class   CONTAINER; \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    inline bool operator==( const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 >& In_, kog::KSmartNull ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    inline bool operator==( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 >& In_ ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    inline bool operator != ( const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 >& In_, kog::KSmartNull ) \
    { \
        return !( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    inline bool operator != ( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 >& In_l ) \
    { \
        return !( EMPTYFN ); \
    }

#define _KSMARTNULL_NULLIFY_FOUR_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, NULLIFY ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 >& Out_ ) \
    { \
        NULLIFY; \
    }

#define _KSMARTNULL_NULLIFY_RESERVE_FOUR_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, NULLIFY, NULLIFY_RESERVE ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 >& Out_ ) \
    { \
        NULLIFY; \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 >& Out_, kog::KReserveTag ) \
    { \
        NULLIFY_RESERVE; \
    }


#define _KSMARTNULL_COMPARISON_FIVE_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, EMPTYFN) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    class   CONTAINER; \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    inline bool operator==( const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 >& In_, kog::KSmartNull ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    inline bool operator==( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 >& In_ ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    inline bool operator != ( const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 >& In_, kog::KSmartNull ) \
    { \
        return !( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    inline bool operator != ( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 >& In_ ) \
    { \
        return !( EMPTYFN ); \
    }

#define _KSMARTNULL_NULLIFY_FIVE_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, NULLIFY ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 >& Out_ ) \
    { \
        NULLIFY; \
    }

#define _KSMARTNULL_NULLIFY_RESERVE_FIVE_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, NULLIFY, NULLIFY_RESERVE ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 >& Out_ ) \
    { \
        NULLIFY; \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 >& Out_, kog::KReserveTag ) \
    { \
        NULLIFY_RESERVE; \
    }


#define _KSMARTNULL_COMPARISON_SIX_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, EMPTYFN) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6 > \
    class   CONTAINER; \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6 > \
    inline bool operator==( const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 , PARAM6 >& In_, kog::KSmartNull ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6 > \
    inline bool operator==( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 , PARAM6 >& In_ ) \
    { \
        return ( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6  > \
    inline bool operator != ( const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 , PARAM6 >& In_, kog::KSmartNull ) \
    { \
        return !( EMPTYFN ); \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6   > \
    inline bool operator != ( kog::KSmartNull, const CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 , PARAM6 >& In_ ) \
    { \
        return !( EMPTYFN ); \
    }

#define _KSMARTNULL_NULLIFY_SIX_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, NULLIFY ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 , PARAM6 >& Out_ ) \
    { \
        NULLIFY; \
    }

#define _KSMARTNULL_NULLIFY_RESERVE_SIX_PARAMS( CONTAINER, TYPE1, TYPE2, TYPE3, TYPE4, TYPE5, TYPE6, NULLIFY, NULLIFY_RESERVE ) \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 , PARAM6 >& Out_ ) \
    { \
        NULLIFY; \
    } \
    template < TYPE1 PARAM1 , TYPE2 PARAM2 , TYPE3 PARAM3 , TYPE4 PARAM4 , TYPE5 PARAM5 , TYPE6 PARAM6 > \
    void    nullify( CONTAINER< PARAM1 , PARAM2 , PARAM3 , PARAM4 , PARAM5 , PARAM6 >& Out_, kog::KReserveTag ) \
    { \
        NULLIFY_RESERVE; \
    }



// for standard template library

namespace   std
{
    _KSMARTNULL_COMPARISON_ONE_PARAM( auto_ptr, class, In_.get() == NULL )

}//namespace   std


// for boost library

namespace   boost
{
    _KSMARTNULL_COMPARISON_ONE_PARAM( scoped_ptr, class, !In_ == true )
    _KSMARTNULL_COMPARISON_ONE_PARAM( scoped_array, class, !In_ == true )
    _KSMARTNULL_COMPARISON_ONE_PARAM( shared_ptr, class, !In_ == true )
    _KSMARTNULL_COMPARISON_ONE_PARAM( shared_array, class, !In_ == true )
    _KSMARTNULL_COMPARISON_ONE_PARAM( weak_ptr, class, In_.expired() == true )
    _KSMARTNULL_COMPARISON_ONE_PARAM( intrusive_ptr, class, !In_ == true )

    _KSMARTNULL_COMPARISON_ONE_PARAM( optional, class, !In_ == true );

}//namespace   boost


// for ATL CComPtr CComQIPtr

namespace ATL
{

    _KSMARTNULL_COMPARISON_ONE_PARAM( CComPtr, class, !In_ == true )
    _KSMARTNULL_COMPARISON_TWO_PARAMS( CComQIPtr, class, const IID*, !In_ == true )

}


namespace   kog
{
    _KSMARTNULL_NULLIFY_ONE_PARAM( std::auto_ptr, class, Out_.reset() )

    _KSMARTNULL_NULLIFY_ONE_PARAM( boost::scoped_ptr, class, Out_.reset() )
    _KSMARTNULL_NULLIFY_ONE_PARAM( boost::scoped_array, class, Out_.reset() )
    _KSMARTNULL_NULLIFY_ONE_PARAM( boost::shared_ptr, class, Out_.reset() )
    _KSMARTNULL_NULLIFY_ONE_PARAM( boost::shared_array, class, Out_.reset() )
    _KSMARTNULL_NULLIFY_ONE_PARAM( boost::weak_ptr, class, Out_.reset() )
    _KSMARTNULL_NULLIFY_ONE_PARAM( boost::intrusive_ptr, class, Out_.reset() )

    _KSMARTNULL_NULLIFY_ONE_PARAM( boost::optional, class, Out_ = boost::none );

    _KSMARTNULL_NULLIFY_ONE_PARAM( ATL::CComPtr, class, Out_.Release() )
    _KSMARTNULL_NULLIFY_TWO_PARAMS( ATL::CComQIPtr, class, const IID*, Out_.Release() )
}

using   namespace kog;
