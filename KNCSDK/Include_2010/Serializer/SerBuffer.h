#ifndef SERBUFFER_H
#define SERBUFFER_H "$Id: SerBuffer.h,v 1.2 2003/01/14 01:55:38 cvs Exp $"

#include "BaseTypes.h"
#include "SerDefs.h"

#include <vector>

class KSerBuffer;
SERIALIZE_DEFINE_TAG(KSerBuffer, eTAG_BUFFER);
SERIALIZE_DECLARE_PUTGET(KSerBuffer);

// 속도 개선의 여지가 많음
class KSerBuffer
{
public:
    KSerBuffer();
    KSerBuffer(const KSerBuffer& other);

    ~KSerBuffer();

    const KSerBuffer& operator=(const KSerBuffer& other);

    bool Write(const void* data, DWORD len);
    bool Read(void* dataOut, DWORD len);
    const void* GetData() const;
    DWORD GetLength() const;
    void Clear();
    void Reset();
    DWORD GetReadLength() const;

    //  [12/20/2003]
    const std::vector<char>& GetBuffer() const { return m_buf; }

    void Swap(KSerBuffer& other);

    bool Compress();
    bool UnCompress();

private:
    bool                  m_bCompress;
    std::vector<char>     m_buf;          // buffer
    DWORD                 m_readOffset;   // "read head" offset from m_buf[0]

    // serialization needs friend access
    SERIALIZE_DECLARE_FRIEND(KSerBuffer);
};

// equality operator
bool operator==(const KSerBuffer& lhs, const KSerBuffer& rhs);

inline KSerBuffer::KSerBuffer()
    : m_readOffset(0), m_bCompress(false)
{
}

inline KSerBuffer::KSerBuffer(const KSerBuffer& other)
    : m_buf(other.m_buf),
      m_readOffset(other.m_readOffset),
      m_bCompress( other.m_bCompress )
{
}

inline KSerBuffer::~KSerBuffer()
{
}

inline const KSerBuffer& KSerBuffer::operator=(const KSerBuffer& other)
{
    // check self-assignment
    if( this != &other )
    {
        m_buf = other.m_buf;
        m_readOffset = other.m_readOffset;
        m_bCompress = other.m_bCompress;
    }
    return *this;
}

inline void KSerBuffer::Swap(KSerBuffer& other)
{
    m_buf.swap(other.m_buf);
    DWORD tmp = m_readOffset;
    m_readOffset = other.m_readOffset;
    other.m_readOffset = tmp;
    std::swap( m_bCompress, other.m_bCompress );

}

inline const void* KSerBuffer::GetData() const
{
    return m_buf.empty() ? 0 : &m_buf[0];
}

inline DWORD KSerBuffer::GetLength() const
{
    return (DWORD)m_buf.size();
}

inline void KSerBuffer::Clear()
{
    m_bCompress = false;
    m_buf.clear();
    m_readOffset = 0;
}

inline void KSerBuffer::Reset()
{
    m_readOffset = 0;
}

inline DWORD KSerBuffer::GetReadLength() const
{
    return (DWORD)(m_buf.size() - m_readOffset);
}

#endif
