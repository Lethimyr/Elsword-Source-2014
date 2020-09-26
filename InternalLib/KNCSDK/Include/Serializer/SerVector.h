#ifndef SERVECTOR_H
#define SERVECTOR_H "$Id: SerVector.h,v 1.3 2003/01/14 05:44:39 cvs Exp $"

#include "SerDefs.h"
#include "SerSTL.h"

#include <vector>

/**
 * SerializeHelper functions for vector<>
 **/
namespace SerializeHelper
{
    template <typename T>
    inline SerializeTag ComputeTag(const std::vector<T>&)
    {
        return eTAG_VECTOR;
    }

    template <typename T>
    inline bool PutInto(KSerializer& ser, const std::vector<T>& obj)
    {
        return PutRange(ser, obj.begin(), obj.end(), (DWORD)obj.size());
    }

    template <typename T>
    inline bool GetFrom(KSerializer& ser, std::vector<T>& obj)
    {
        obj.clear();
        return GetRange(ser, std::back_inserter(obj), (T*)0);
    }
}

#endif