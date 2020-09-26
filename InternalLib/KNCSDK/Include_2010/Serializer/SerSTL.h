#ifndef SERSTL_H
#define SERSTL_H "$Id: SerSTL.h,v 1.3 2003/01/14 05:44:39 cvs Exp $"

#include "Serializer.h"

namespace SerializeHelper
{
    /**
    * Helper for STL PutInto functions
    **/
    template <typename Iterator>
    bool PutRange(KSerializer& ser, 
                  const Iterator& begin, 
                  const Iterator& end, 
                  DWORD numItems)
    {
        // store the size
        if( !ser.Put(numItems) )
            return false;
        // store each element
        DWORD count = 0;
        for( Iterator it = begin; it != end; ++it, ++count )
            if( !ser.Put(*it) )
                return false;
        // just in case distance(begin,end) != numItems
        return (count == numItems);
    }

    /**
    * Helper for STL GetFrom functions
    **/
    template <typename T, typename Iterator>
    bool GetRange(KSerializer& ser, 
                  Iterator it, 
                  const T*)
    {
        // read size
        DWORD size;
        if( !ser.Get(size) )
            return false;
        // read each item
        for( DWORD i = 0; i < size; ++i )
        {
            T obj;
            if (!ser.Get(obj))
                return false;
            *it = obj; // insertion
        } // for

        return true;
    }
}

#endif