#ifndef KSERIALIZER_H
#define KSERIALIZER_H

#include <vector>
#include <string>
#include <cassert>

#include "KMemPool.h"

class KSerializer
{

	KDeclareMemPool(KSerializer);

public:
    KSerializer(int iReserveSize = ms_uiReserveSize)
	: m_uiCurReadPos( 0 ), m_uiCurWritePos( 0 )
    {
        m_vecDatagram.reserve(iReserveSize);
        m_vecDatagram.resize(iReserveSize);
    }

    void InitPool() 
	{ 
		m_uiCurReadPos = 0;
		m_uiCurWritePos = 0;
	}
    void ReleasePool() 
	{
		m_uiCurReadPos = 0;
		m_uiCurWritePos = 0;
	}

	virtual void Clear()
	{
		m_uiCurReadPos = 0;
		m_uiCurWritePos = 0;
	}

	unsigned int GetCurrWritePos()
	{
		return m_uiCurWritePos;
	}

	unsigned int GetCurrReadPos()
	{
		return m_uiCurReadPos;
	}

	void SetCurrReadPos(unsigned int uiDataSize)
	{
		ExtendBuffer(uiDataSize);
		m_uiCurReadPos = uiDataSize;
	}

	void SetCurrWritePos(unsigned int uiDataSize)
	{
		ExtendBuffer(uiDataSize);
        m_uiCurWritePos = uiDataSize;
	}

	const char* GetBuffer()
	{
		return GetPosition(0);
	}

	void SetBuffer( char* pcBuffer, unsigned int uiSize )
	{
		ExtendBuffer( uiSize );

		Clear();
		memcpy(&m_vecDatagram[0], pcBuffer, uiSize);

		m_uiCurWritePos = uiSize;
	}

	void SetBuffer(char* pcBuffer, unsigned int iPosition, unsigned int uiSize)
	{
		ExtendBuffer(uiSize + iPosition - m_uiCurWritePos);

		int iCurWritePos = m_uiCurWritePos;

		if (m_uiCurWritePos < iPosition + uiSize)
			iCurWritePos = iPosition + uiSize;
        
		m_uiCurWritePos = iPosition;
		WriteBytes(pcBuffer, uiSize);

		m_uiCurWritePos = iCurWritePos;
	}

	void InsertBuffer(char* pcBuffer, unsigned int uiSize)
	{
		ExtendBuffer(uiSize);
		WriteBytes(pcBuffer, uiSize);
	}

	int GetDataCheckSize()
	{
		return 0;
	}

	char* GetPosition(unsigned int iPos)
	{
		if (m_vecDatagram.capacity() <= iPos)
		{
			ExtendBuffer( iPos - m_uiCurWritePos );
		}

		return &m_vecDatagram[iPos];
	}

    virtual void ExtendBuffer(unsigned int uiSize)
    {
        if (m_vecDatagram.capacity() > m_uiCurWritePos + uiSize)
        {
            return;
        }

        unsigned int uiExtendSize = (unsigned int)m_vecDatagram.capacity() + ms_uiReserveSize;
        m_vecDatagram.reserve(uiExtendSize);
        m_vecDatagram.resize(uiExtendSize);
        memset(&m_vecDatagram[m_uiCurWritePos], 0, ms_uiReserveSize);

        ExtendBuffer(uiSize);
    }

    // Out Stream
    template<class T>
    KSerializer& operator << (T& kValue)
    {
        unsigned int uiSize = sizeof(kValue);

		WriteBytes(&kValue, uiSize);

        return *this;
    }

	KSerializer& operator << (int kValue)
	{
		unsigned int uiSize = sizeof(kValue);

		WriteBytes(&kValue, uiSize);

		return *this;
	}

    KSerializer& operator << (const char* kValue)
    {
        unsigned int uiSize = (unsigned int)strlen(kValue) + 1;

		//WriteTag(KSerializer::eString);
		WriteBytes(&uiSize, sizeof(unsigned int));
		WriteBytes(kValue, uiSize);

        return *this;
    }

    KSerializer& operator << (std::string& kValue)
    {
		unsigned int uiSize = (unsigned int)kValue.size();

		//WriteTag(KSerializer::eString);
		WriteBytes(&uiSize, sizeof(unsigned int));
		WriteBytes(kValue.c_str(), uiSize);

        return *this;
    }

	KSerializer& operator << (KSerializer& kValue)
	{
		unsigned int uiSize = kValue.GetCurrWritePos();

		//WriteTag(KSerializer::eSerializer);
		WriteBytes(&uiSize, sizeof(unsigned int));
		WriteBytes(kValue.GetPosition(0), uiSize);

		return *this;
	}

    // In Stream
    template<class T>
    KSerializer& operator >> (T& kValue)
    {
        unsigned int uiSize = sizeof(kValue);

        //VerifyTag(KSerializer::eDefault);
        //ReadBytes(&uiSize, sizeof(unsigned int));
        ReadBytes(&kValue, uiSize);

        return *this;
    }

    KSerializer& operator >> (std::string& kValue)
    {
        //VerifyTag(KSerializer::eString);

        unsigned int uiSize;
        ReadBytes(&uiSize, sizeof(unsigned int));

        char* pcBuf = new char[uiSize];
        ReadBytes(pcBuf, uiSize);

        kValue.assign(pcBuf, uiSize);
        delete [] pcBuf;

        return *this;
    }

    KSerializer& operator >> (char* kValue)
    {
        //VerifyTag(KSerializer::eString);

        unsigned int uiSize;
        ReadBytes(&uiSize, sizeof(unsigned int));
        ReadBytes(kValue, uiSize);

        return *this;
    }

	KSerializer& operator >> (KSerializer& kValue)
	{
		//VerifyTag(KSerializer::eSerializer);

		unsigned int uiSize;
		ReadBytes(&uiSize, sizeof(unsigned int));

		kValue.Clear();
		kValue.SetCurrWritePos(uiSize);

		ReadBytes(kValue.GetPosition(0), uiSize);
		return *this;
	}

    // I/O Function
    virtual void WriteBytes(const void* pvBuf, unsigned int uiSize)
    {
        ExtendBuffer(uiSize);

        memcpy(&m_vecDatagram[m_uiCurWritePos], pvBuf, uiSize);
        m_uiCurWritePos += uiSize;
    }

    void ReadBytes(void* pvBuf, unsigned int uiSize)
    {
		memcpy(pvBuf, &m_vecDatagram[m_uiCurReadPos], uiSize);
        m_uiCurReadPos += uiSize;
    }

    void WriteTag(const char uiTag)
    {
        ExtendBuffer(1);

        WriteBytes(&uiTag, 1);
    }

    bool VerifyTag(const char uiTag)
    {
        char uiReadTag;
        ReadBytes(&uiReadTag, 1);

        assert(uiReadTag == uiTag && "invalid Serializer tag");
        return (uiReadTag == uiTag);
    }

protected:
    enum ETag
    {
        eDefault,
        eString,
		eSerializer
    };

    std::vector<char> m_vecDatagram;

	unsigned int m_uiCurWritePos;
	unsigned int m_uiCurReadPos;

    static const unsigned int ms_uiReserveSize;

};

#endif // KSERIALIZER_H