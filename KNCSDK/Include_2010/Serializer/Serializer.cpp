static const char* cvsid = "$Id: Serializer.cpp,v 1.3 2003/01/14 01:55:38 cvs Exp $";

#include "Serializer.h"
#include "SerBuffer.h"
#include "Byteorder.h"
#include <cassert>


KSerializer::KSerializer()
    : m_tagsEnabled(false),
      m_buffer(0)
{
  // empty
}

KSerializer::~KSerializer()
{
    m_buffer = 0;
}

bool KSerializer::BeginWriting(KSerBuffer* pkBuf, bool tagging)
{
    assert(pkBuf != 0);
    m_tagsEnabled = tagging;
    m_buffer = pkBuf;
    return true;
}

bool KSerializer::EndWriting()
{
    m_buffer = 0;
    return true;
}

bool KSerializer::BeginReading(KSerBuffer* pkBuf, bool tagging)
{
    assert(pkBuf != 0);
    m_tagsEnabled = tagging;
    m_buffer = pkBuf;
    return true;
}

bool KSerializer::EndReading()
{
    m_buffer = 0;
    return true;
}

DWORD KSerializer::GetReadLength() const
{
	return m_buffer->GetReadLength();
}


bool KSerializer::Put(char x)
{
    return WriteNumber(x, eTAG_CHAR);
}

bool KSerializer::Get(char& x)
{
    return ReadNumber(x, eTAG_CHAR);
}

//{{ Begin 051226. kkurrung.
bool KSerializer::Put(wchar_t x)
{
    return WriteNumber(x, eTAG_WCHAR);
}

bool KSerializer::Get(wchar_t& x)
{
    return ReadNumber(x, eTAG_WCHAR);
}
//}} 051226. End kkurrung.

bool KSerializer::Put(UCHAR x)
{
    return WriteNumber(x, eTAG_UCHAR);
}

bool KSerializer::Get(UCHAR& x)
{
    return ReadNumber(x, eTAG_UCHAR);
}

bool KSerializer::Put(short x)
{
    x = HTON_I16(x);
    return WriteNumber(x, eTAG_SHORT);
}

bool KSerializer::Get(short& x)
{
    bool ret = ReadNumber(x, eTAG_SHORT);
    x = NTOH_I16(x);
    return ret;
}

bool KSerializer::Put(USHORT x)
{
    x = HTON_I16(x);
    return WriteNumber(x, eTAG_USHORT);
}

bool KSerializer::Get(USHORT& x)
{
    bool ret = ReadNumber(x, eTAG_USHORT);
    x = NTOH_I16(x);
    return ret;
}

bool KSerializer::Put(int x)
{
    x = HTON_I32(x);
    return WriteNumber(x, eTAG_INT);
}

bool KSerializer::Get(int& x)
{
    bool ret = ReadNumber(x, eTAG_INT);
    x = NTOH_I32(x);
    return ret;
}

bool KSerializer::Put(DWORD x)
{
    x = HTON_I32(x);
    return WriteNumber(x, eTAG_DWORD);
}

bool KSerializer::Get(DWORD& x)
{
    bool ret = ReadNumber(x, eTAG_DWORD);
    x = NTOH_I32(x);
    return ret;
}

bool KSerializer::Put(UINT x)
{
    return Put((DWORD)x);
}

bool KSerializer::Get(UINT& x)
{
    return Get((DWORD&)x);
}

bool KSerializer::Put(INT64 x)
{
    x = HTON_I64(x);
    return WriteNumber(x, eTAG_INT64);
}

bool KSerializer::Get(INT64& x)
{
    bool ret = ReadNumber(x, eTAG_INT64);
    x = NTOH_I64(x);
    return ret;
}

bool KSerializer::Put(UINT64 x)
{
    x = HTON_I64(x);
    return WriteNumber(x, eTAG_UINT64);
}

bool KSerializer::Get(UINT64& x)
{
    bool ret = ReadNumber(x, eTAG_UINT64);
    x = NTOH_I64(x);
    return ret;
}


bool KSerializer::Put(float x)
{
    x = HTON_F32(x);
    return WriteNumber(x, eTAG_FLOAT);
}

bool KSerializer::Get(float& x)
{
    bool ret = ReadNumber(x, eTAG_FLOAT);
    x = NTOH_F32(x);
    return ret;
}

bool KSerializer::Put(double x)
{
    x = HTON_F64(x);
    return WriteNumber(x, eTAG_DOUBLE);
}

bool KSerializer::Get(double& x)
{
    bool ret = ReadNumber(x, eTAG_DOUBLE);
    x = NTOH_F64(x);
    return ret;
}

bool KSerializer::Put(bool x)
{
    UCHAR val = x ? 1 : 0;
    return WriteNumber(val, eTAG_BOOL);
}

bool KSerializer::Get(bool& x)
{
    UCHAR val;
    bool ret = ReadNumber(val, eTAG_BOOL);
    x = (val == 1);
    return ret;
}

bool KSerializer::Put(const std::string& x)
{
    // tag
    if( !WriteTag(eTAG_STRING) )
        return false;

    DWORD size = (DWORD)x.size();
    if( !Put(size) )
        return false;
    // data (only if there is some)
    return (size == 0) || WriteBytes(x.data(), size);
}

bool KSerializer::Put(const char* x)
{
    // could just return Put(string(x)), but that makes a copy..
    assert(x != 0);
    // tag
    if( !WriteTag(eTAG_STRING) )
        return false;

    // size
    DWORD size = (DWORD)strlen(x);
    if( !Put(size) )
        return false;
    // data (only if there is some)
    return (size == 0) || WriteBytes(x, size);
}

bool KSerializer::Get(std::string& x)
{
    // erase contents
    x.erase();
    
    // tag
    if( !ReadAndCheckTag(eTAG_STRING) )
    return false;

    // size
    DWORD size;
    if( !Get(size) )
        return false;

    if( m_buffer != NULL && m_buffer->GetReadLength() < size )
        return false;

    // data (only if there is some)
    if( size == 0 )
        return true;
    // since we can't get a pointer to string's internal data, have to
    // use a temp buffer and copy it...
    char* tempString = new char[size];
    bool ok = ReadBytes(tempString, size);
    if (ok)
    x.assign(tempString, size);
    delete [] tempString;
    return ok;
}


//{{ Begin 051226. kkurrung.
bool KSerializer::Put(const std::wstring& x)
{
    // tag
    if( !WriteTag(eTAG_WSTRING) )
        return false;

    DWORD size = (DWORD)x.size() * sizeof(wchar_t) ;
    if( !Put(size) )
        return false;
    // data (only if there is some)
    return (size == 0) || WriteBytes(x.data(), size );
}

#include <iostream>
bool KSerializer::Put(const wchar_t* x)
{
    // could just return Put(string(x)), but that makes a copy..
    assert(x != 0);
    // tag
    if( !WriteTag(eTAG_WSTRING) )
        return false;

    // size
    DWORD size = (DWORD)wcslen(x) * sizeof(wchar_t);
    if( !Put(size) )
        return false;
    // data (only if there is some)
    return (size == 0) || WriteBytes(x, size);
}

bool KSerializer::Get(std::wstring& x)
{
    // erase contents
    x.erase();

    // tag
    if( !ReadAndCheckTag(eTAG_WSTRING) )
        return false;

    // size
    DWORD size;
    if( !Get(size) )
        return false;

    if( size % 2 != 0 )
        return false;

    if( m_buffer != NULL && m_buffer->GetReadLength() < size )
        return false;

    // data (only if there is some)
    if( size == 0 )
        return true;
    // since we can't get a pointer to string's internal data, have to
    // use a temp buffer and copy it...
    wchar_t* tempString = new wchar_t[size/sizeof(wchar_t)];
    bool ok = ReadBytes((char*)tempString, size);
    if (ok)
        x.assign(tempString, size/sizeof(wchar_t));
    delete [] tempString;
    return ok;
}
//}} 051226. End kkurrung.

/**
 * Raw byte arrays
 **/
bool KSerializer::PutRaw(const void* buf, DWORD len)
{
    assert(buf != 0);
    assert(len > 0);

    // note: no length field
    return WriteTag(eTAG_RAWBYTES) && WriteBytes(buf, len);
}
 
bool KSerializer::GetRaw(void* buf, DWORD len)
{
    assert(buf != 0);
    assert(len > 0);

    // note: no length field
    return ReadAndCheckTag(eTAG_RAWBYTES) && ReadBytes(buf, len);
}


/**
 * Write a tag
 *
 * If not in 'tagging' mode, then automatically succeeds.
 **/
bool KSerializer::WriteTag(const SerializeTag tag)
{
    assert((tag >= 0) && (tag <= 255)); // fit inside uint8

    UCHAR tmp = tag;
    return !m_tagsEnabled || WriteBytes(&tmp, 1);
}

/**
 * Read a tag, and check it against an expected value
 *
 * If not in 'tagging' mode, then automatically succeeds.
 **/
bool KSerializer::ReadAndCheckTag(SerializeTag expectedTag)
{
  UCHAR tmp;
  return !m_tagsEnabled || (ReadBytes(&tmp, 1) && (tmp == expectedTag));
}


/**
 * Write to the output Buffer
 **/
bool KSerializer::WriteBytes(const void* buf, DWORD len)
{
    assert(buf != 0);
    assert(len > 0);

    return m_buffer->Write(buf, len);
}

/**
 * Read from the input Buffer
 **/
bool KSerializer::ReadBytes(void* buf, DWORD len)
{
    assert(buf != 0);
    assert(len > 0);

    return m_buffer->Read(buf, len);
}
