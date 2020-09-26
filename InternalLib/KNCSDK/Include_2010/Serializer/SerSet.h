#ifndef SERSET_H
#define SERSET_H "$Id: SerSet.h,v 1.3 2003/01/14 05:44:39 cvs Exp $"

#include "SerDefs.h"
#include "SerSTL.h"

#include <set>

#include <iterator>
/**
 * SerializeHelper functions for set<> and multiset<>
 **/
namespace SerializeHelper
{
    template <typename T, typename COMPARE>
    inline SerializeTag ComputeTag(const std::set<T,COMPARE>&)
    {
        return eTAG_SET;
    }

    template <typename T, typename COMPARE>
    inline bool PutInto(KSerializer& ser, const std::set<T,COMPARE>& obj)
    {
        return PutRange(ser, obj.begin(), obj.end(), obj.size());
    }

    template <typename T, typename COMPARE>
    inline bool GetFrom(KSerializer& ser, std::set<T,COMPARE>& obj)
    {
    obj.clear();
        return GetRange(ser, std::inserter(obj, obj.end()), (T*)0);
    }

    template <typename T, typename COMPARE>
    inline SerializeTag ComputeTag(const std::multiset<T,COMPARE>&)
    {
        return eTAG_MULTISET;
    }

    template <typename T, typename COMPARE>
    inline bool PutInto(KSerializer& ser, const std::multiset<T,COMPARE>& obj)
    {
        return PutRange(ser, obj.begin(), obj.end(), obj.size());
    }

    template <typename T, typename COMPARE>
    inline bool GetFrom(KSerializer& ser, std::multiset<T,COMPARE>& obj)
    {
        obj.clear();
        return GetRange(ser, std::inserter(obj, obj.end()), (T*)0);
    }
}

#endif
