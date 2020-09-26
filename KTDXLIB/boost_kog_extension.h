/** @file   boost_kog_extension.h
    @desc   interface of boost_kog_extension
    @author seojt@kogstudios.com
    @since  2009-1-9, 14:58
    @date   first created to add a function which downcast weak_ptr<T>
*/

#ifndef _BOOST_KOG_EXTENSION_H
#define _BOOST_KOG_EXTENSION_H

#include <boost/assert.hpp>   
#include <boost/shared_ptr.hpp>   
#include <boost/weak_ptr.hpp>   
//#include <boost/pointer_cast.hpp>   

namespace boost
{   
    template<typename TRG_T, typename SRC_T>   
    inline weak_ptr<TRG_T>
    static_pointer_cast( weak_ptr<SRC_T> const & wp_src)
    //   
    // This works even if the source weak_ptr is null or   
    // expired.   
    //   
    // No throw - this should never throw an exception.   
    {   
        // Get a shared_ptr from the weak_ptr.   
        // Use weak_ptr.lock( ) so this will not throw   
        // bad_weak_ptr if the weak_ptr is expired.   
        shared_ptr< SRC_T >   sp_src( wp_src.lock() );   
        BOOST_ASSERT( wp_src.expired() == !sp_src );
          
        // Cast the shared_ptr to the target type.   
        shared_ptr< TRG_T >   sp_trg( static_pointer_cast<TRG_T>( sp_src) );
          
        // Auto-cast the shared_ptr< TRG_T > to the   
        // weak_ptr< TRG_T > return type.   
        return sp_trg;   
    }   

}//namespace boost

#endif // _BOOST_KOG_EXTENSION_H
