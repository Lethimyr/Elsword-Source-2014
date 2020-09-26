#ifndef SERPAIR_H
#define SERPAIR_H "$Id: SerPair.h,v 1.3 2003/01/14 05:44:39 cvs Exp $"

#include "SerDefs.h"
#include "Serializer.h"

//#include <utility>

/**
 * SerializeHelper functions for pair<T1,T2>
 **/
namespace SerializeHelper
{
    template <typename T1, typename T2>
    inline SerializeTag ComputeTag(const std::pair<T1,T2>&)
    {
    return eTAG_PAIR;
    }

    template <typename T1, typename T2>
    inline bool PutInto(KSerializer& ser, const std::pair<T1,T2>& obj)
    {
        return ser.Put(obj.first) && ser.Put(obj.second);
    }

    template <typename T1, typename T2>
    inline bool GetFrom(KSerializer& ser, std::pair<T1,T2>& obj)
    {
        return ser.Get(obj.first) && ser.Get(obj.second);
    }
};

#endif
