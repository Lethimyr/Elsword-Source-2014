#ifndef KEYEDSERIALIZER_H
#define KEYEDSERIALIZER_H "$Id: KeyedSerializer.h,v 1.4 2003/01/14 01:55:38 cvs Exp $"

#include "SerBuffer.h"
#include "SerDefs.h"
#include "Serializer.h"
#include "SerMap.h"

#include <map>
#include <vector>

template <typename KEY>
class KKeyedSerializer
{
public:
    KKeyedSerializer();
    ~KKeyedSerializer();

    // default copy/assignment functions are fine

    /**
    * Put the given object into the map, using the given key.  Any
    * existing object with this key is overwritten.
    **/
    template <typename T>
    bool Put(const KEY& key, const T& obj)
    {
        // serialize object into a buffer
        KSerBuffer buf;
        KSerializer ser;
        if( !ser.BeginWriting(&buf, m_useTagging) ||
            !ser.Put(obj) ||
            !ser.EndWriting())
            return false;
        
        // add to the map (avoid copying by inserting an empty Buffer
        // first, and using Swap())
        m_objects[key] = KSerBuffer();
        m_objects[key].Swap(buf);
        return true;
    }

    /**
    * Get the given object from the map, using the key.  The same
    * object may be read multiple times - its data is not "consumed" by
    * this function.
    **/
    template <typename T>
    bool Get(const KEY& key, T& obj) const
    {
        // get buffer
        BufferMap::const_iterator it = m_objects.find(key);
        if (it == m_objects.end())
            return false;
        // have to make a copy, because it will be const
        KSerBuffer buf(it->second);
        // unserialize
        KSerializer ser;
        return ser.BeginReading(&buf, m_useTagging) && 
               ser.Get(obj) && 
               ser.EndReading();
    }

    bool HasKey(const KEY& key) const;
    void GetKeys(std::vector<KEY>& outKeys) const;
    void Erase(const KEY& key);
    void Clear();

    // should internal serialization use tags?
    void SetTagging(bool useTagging);

    // serialization helpers - since friendship and template classes
    // don't really work well together, we can't make the
    // SerializeHelper functions friends directly.
    bool PutInto(KSerializer& ser) const;
    bool GetFrom(KSerializer& ser);

    bool Equals(const KKeyedSerializer& other) const;

private:

    typedef std::map<KEY, KSerBuffer>  BufferMap;

    bool m_useTagging;
    BufferMap                       m_objects;
}; // end KeyedSerializer class

// template methods

template <typename KEY>
inline KKeyedSerializer<KEY>::KKeyedSerializer()
    : m_useTagging(false)
{
    // empty
}

/**
 * Destructor
 **/
template <typename KEY>
inline KKeyedSerializer<KEY>::~KKeyedSerializer()
{
    // empty
}

/**
 * Return true if the given key is in the map
 **/
template <typename KEY>
bool KKeyedSerializer<KEY>::HasKey(const KEY& key) const
{
    return m_objects.find(key) != m_objects.end();
}

/**
 * Retrieve a vector of all keys
 **/
template <typename KEY>
void KKeyedSerializer<KEY>::GetKeys(std::vector<KEY>& outKeys) const
{
    BufferMap::const_iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); ++it)
        outKeys.push_back(it->first);
}

/**
 * Remove a key from the map
 **/
template <typename KEY>
void KKeyedSerializer<KEY>::Erase(const KEY& key)
{
    m_objects.erase(key);
}

/**
 * Erase object contents
 **/
template <typename KEY>
void KKeyedSerializer<KEY>::Clear()
{
    m_objects.clear();
}


/**
 * Turn internal tagging on/off
 **/
template <typename KEY>
void KKeyedSerializer<KEY>::SetTagging(bool useTagging)
{
    m_useTagging = useTagging;
}


/**
 * Return true if objects are equal
 **/
template <typename KEY>
bool KKeyedSerializer<KEY>::Equals(const KKeyedSerializer<KEY>& other) const
{
    return (m_useTagging == other.m_useTagging) && (m_objects == other.m_objects);
}

/**
 * Store object in serializer
 **/
template <typename KEY>
bool KKeyedSerializer<KEY>::PutInto(KSerializer& ser) const
{
    return ser.Put(m_useTagging) && ser.Put(m_objects);
}

/**
 * Load object from serializer
 **/
template <typename KEY>
bool KKeyedSerializer<KEY>::GetFrom(KSerializer& ser)
{
    m_objects.clear();
    return ser.Get(m_useTagging) && ser.Get(m_objects);
}

/**
 * Equality operators
 **/
template <typename KEY>
bool operator==(const KKeyedSerializer<KEY>& lhs,
                const KKeyedSerializer<KEY>& rhs)
{
    return lhs.Equals(rhs);
}

template <typename KEY>
bool operator!=(const KKeyedSerializer<KEY>& lhs,
                const KKeyedSerializer<KEY>& rhs)
{
    return !lhs.Equals(rhs);
}

/**
 * SerializeHelper functions for KKeyedSerializer<>
 **/
namespace SerializeHelper
{
    template <typename KEY>
    inline SerializeTag ComputeTag(const KKeyedSerializer<KEY>&)
    {
        return eTAG_KEYEDSERIALIZER;
    }

    template <typename KEY>
    inline bool PutInto(KSerializer& ser, const KKeyedSerializer<KEY>& obj)
    {
        return obj.PutInto(ser);
    }

    template <typename KEY>
    inline bool GetFrom(KSerializer& ser, KKeyedSerializer<KEY>& obj)
    {
        return obj.GetFrom(ser);
    }
}

#endif