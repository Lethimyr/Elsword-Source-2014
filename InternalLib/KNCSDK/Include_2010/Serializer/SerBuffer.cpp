static const char* cvsid = "$Id: SerBuffer.cpp,v 1.3 2003/01/14 01:55:38 cvs Exp $";

#include "SerBuffer.h"
#include "Serializer.h"

#include <cassert>
#include <cstring>

/**
 * Equality operator
 **/
bool operator==(const KSerBuffer& lhs, const KSerBuffer& rhs)
{
    return (lhs.GetLength() == rhs.GetLength()) &&
           (memcmp(lhs.GetData(), rhs.GetData(), lhs.GetLength()) == 0);
}

/**
 * Append data to the end of the buffer
 **/
bool KSerBuffer::Write(const void* data, DWORD len)
{
    assert(data != 0);
    assert(len > 0);

    // resize to new length
    DWORD oldSize = (DWORD)m_buf.size();
    m_buf.resize(oldSize + len);
    // copy directly into the vector's memory
    memcpy(&m_buf[oldSize], data, len);
    return true;
}

/**
 * Read bytes from the current read position
 **/
bool KSerBuffer::Read(void* dataOut, DWORD len)
{
    assert(dataOut != 0);
    assert(len > 0);

    // not enough to read?
    if( len > (m_buf.size() - m_readOffset) )
        return false;
    // copy, advance read head
    memcpy(dataOut, &m_buf[m_readOffset], len);
    m_readOffset += len;
    return true;
}

/**
 * Serialization
 **/
SERIALIZE_DEFINE_PUT(KSerBuffer, obj, serializer)
{
  // store size and bytes, but not read offset
  DWORD len = obj.GetLength();
  return serializer.Put(len) &&
        ((len == 0) || ( serializer.Put( obj.m_bCompress ) && serializer.PutRaw(obj.GetData(), len) ));
}

SERIALIZE_DEFINE_GET(KSerBuffer, obj, serializer)
{
  // clear existing contents
  obj.Clear();
  // read size
  DWORD len;
  if( !serializer.Get(len) )
  {
    return false;
  }

  if( len == 0 )
  {
    return true;
  }

  if( len > serializer.GetReadLength() )
  {
	  return false;
  }

  // resize and read data
  obj.m_buf.resize(len);
  return serializer.Get( obj.m_bCompress ) && serializer.GetRaw(&obj.m_buf[0], len);
}

#include "zlib/zlib.h"

bool KSerBuffer::Compress()
{
    if( m_bCompress ) return true;

    // 압축된 데이터를 담을 버퍼를 준비한다.
    std::vector<UCHAR> vecBuff;
    DWORD dwSize = compressBound( GetLength() );
    vecBuff.resize( dwSize );

    // compress data.
    DWORD dwOriginSize = GetLength();
    int nRet = compress2( &vecBuff[0], &dwSize, (UCHAR*)GetData(), GetLength(), 1 );
    if( nRet != Z_OK )
        return false;

    vecBuff.resize( dwSize );   // trim extra buffer space

    // 압축된 데이터를 kbuff에 옮겨 담는다.
    Clear();
    Write( &dwOriginSize, sizeof( dwOriginSize) );
    Write( (void*)&vecBuff[0], vecBuff.size() );

    m_bCompress = true;
    return true;
}

bool KSerBuffer::UnCompress()
{    
    if( !m_bCompress ) return true;

    DWORD dwOriginSize;
    DWORD dwRetSize;

    m_readOffset = 0;

    Read( &dwOriginSize, sizeof( dwOriginSize ) );

    dwRetSize = dwOriginSize;

    std::vector<UCHAR> vecDecompBuff;
    vecDecompBuff.resize( dwOriginSize );
    
    int nRet = uncompress( &vecDecompBuff[0], &dwRetSize, (UCHAR*)&m_buf[m_readOffset], m_buf.size() - sizeof( dwOriginSize ) );
    if( nRet != Z_OK || dwRetSize != dwOriginSize )
        return false;
   
    Clear();
    Write( (void*)(&vecDecompBuff[0]), dwOriginSize );

    m_bCompress = false;

    return true;
}
