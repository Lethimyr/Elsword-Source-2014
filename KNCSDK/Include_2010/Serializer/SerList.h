#ifndef SERLIST_H
#define SERLIST_H "$Id: SerList.h,v 1.3 2003/01/14 05:44:39 cvs Exp $"

#include "SerDefs.h"
#include "SerSTL.h"

#include <list>

/**
 * SerializeHelper functions for list<>
 **/
namespace SerializeHelper
{
    template <typename T>
    inline SerializeTag ComputeTag(const std::list<T>&)
    {
        return eTAG_LIST;
    }

    template <typename T>
    inline bool PutInto(KSerializer& ser, const std::list<T>& obj)
    {
        return PutRange(ser, obj.begin(), obj.end(), obj.size());
    }

    template <typename T>
    inline bool GetFrom(KSerializer& ser, std::list<T>& obj)
    {
    obj.clear();
        return GetRange(ser, std::back_inserter(obj), (T*)0);
    }
}

#endif
