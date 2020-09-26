#ifndef SERIALIZER_H
#define SERIALIZER_H "$Id: Serializer.h,v 1.5 2003/04/19 03:28:43 cvs Exp $"

#pragma warning(disable: 4786)

#include "BaseTypes.h"
#include "SerDefs.h"

#include <string>

class KSerBuffer;                  // fwd

class KSerializer
{
public:
    KSerializer();
    virtual ~KSerializer();

    bool BeginWriting(KSerBuffer* pkBuf, bool tagging = false);
    bool EndWriting();

    bool BeginReading(KSerBuffer* pkBuf, bool tagging = false);
    bool EndReading();

	DWORD GetReadLength() const;

    // user-defined class
    template <typename T>
    bool Put(const T& obj)
    {
        SerializeTag tag = SerializeHelper::ComputeTag(obj);
        return WriteTag(tag) && 
               SerializeHelper::PutInto(*this, obj);
    }

    template <typename T>
    bool Get(T& obj)
    {
        SerializeTag tag = SerializeHelper::ComputeTag(obj);
        return ReadAndCheckTag(tag) && 
               SerializeHelper::GetFrom(*this, obj);
    }

    // builtin types
    bool Put(char x);
    bool Get(char& x);
    bool Put(wchar_t x);
    bool Get(wchar_t& x);
    bool Put(UCHAR x);
    bool Get(UCHAR& x);
    bool Put(short x);
    bool Get(short& x);
    bool Put(USHORT x);
    bool Get(USHORT& x);
    bool Put(int x);
    bool Get(int& x);
    bool Put(DWORD x);
    bool Get(DWORD& x);
    bool Put(UINT x);
    bool Get(UINT& x);
    bool Put(INT64 x);
    bool Get(INT64& x);
    bool Put(UINT64 x);
    bool Get(UINT64& x);
    bool Put(float x);
    bool Get(float& x);
    bool Put(double x);
    bool Get(double& x);
    bool Put(bool x);
    bool Get(bool& x);

    // string
    bool Put(const std::string& x);
    bool Put(const char* x);
    bool Get(std::string& x);

    // wstring
    bool Put(const std::wstring& x);
    bool Put(const wchar_t* x);
    bool Get(std::wstring& x);

    // raw byte array of known size
    bool PutRaw(const void* buf, DWORD len);
    bool GetRaw(void* buf, DWORD len);

    // array
    template <typename T>
    bool PutArray(const T* objs, DWORD numItems)
    {
        // write tag
        if( !WriteTag(eTAG_ARRAY) )
            return false;

        // store size
        if( !Put(numItems) )
            return false;
        // store each element
        for( DWORD i = 0; i < numItems; ++i )
            if( !Put(objs[i]) )
                return false;
        return true;
    }

    template <typename T>
    bool GetArray(T* objs, DWORD& numInOut)
    {
        // check tag
        if( !ReadAndCheckTag(eTAG_ARRAY) )
            return false;

        DWORD numItems;
        if( !Get(numItems) )
            return false;
        // too many to read?
        if( numItems > numInOut )
            return false;
        // read each element
        for( DWORD i = 0; i < numItems; ++i )
            if( !Get(objs[i]) )
                return false;
        numInOut = numItems;
        return true;
    }

protected:
    template <typename T>
    inline bool WriteNumber(T x, SerializeTag tag)
    {
        return WriteTag(tag) && WriteBytes(&x, sizeof(T));
    }

    template <typename T>
    inline bool ReadNumber(T& x, SerializeTag tag)
    {
        return ReadAndCheckTag(tag) && ReadBytes(&x, sizeof(T));
    }

    // tags
    bool WriteTag(SerializeTag tag);
    bool ReadAndCheckTag(SerializeTag expectedTag);

    // low-level Buffer access
    bool WriteBytes(const void* buf, DWORD len);
    bool ReadBytes(void* buf, DWORD len);

private:
    // disabled - no copying
    KSerializer(const KSerializer&);
    void operator=(const KSerializer&);

protected:
    KSerBuffer*     m_buffer;
    bool            m_tagsEnabled;

};

#endif