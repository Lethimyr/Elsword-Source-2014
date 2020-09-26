#pragma once


#include <vector>


class KVariableSizeBuffer
{
private:
	unsigned long			m_ulTotalSize;
	unsigned long			m_ulRemainSize;
	std::vector< char >		m_vecBuffer;

public:
	KVariableSizeBuffer() : m_ulTotalSize( 0 ), m_ulRemainSize( 0 )
	{
	}

	virtual ~KVariableSizeBuffer()
	{
	}

	unsigned long GetBufferSize() const		{ return m_vecBuffer.size(); }
	unsigned long GetRemainSize() const		{ return m_ulRemainSize; }
	bool IsAlreadyRecv() const				{ return !m_vecBuffer.empty(); }
	bool IsComplete() const					{ return ( m_ulRemainSize == 0 ); }
	const char* GetBuffer() const			{ return &m_vecBuffer[0]; }
	void Resize( const int iSize )			{ m_vecBuffer.resize( iSize ); }
	void Reserve( const int iSize )			{ m_vecBuffer.reserve( iSize ); }

	// vector버퍼의 메모리를 해제하고 새로 할당 받아서 깨끗하게 초기화 하는 함수
	void Clear()
	{
		m_ulTotalSize = 0;
		m_ulRemainSize = 0;
		m_vecBuffer.clear();
	}

	// vector버퍼 메모리 주소를 재활용 하기 위해 호출하는 초기화 함수
	void Reset()
	{
		m_ulTotalSize = 0;
		m_ulRemainSize = 0;
		m_vecBuffer.erase( m_vecBuffer.begin(), m_vecBuffer.end() );
	}

	void Init( const unsigned long ulTotalSize )
	{
		m_ulTotalSize = ulTotalSize;
		m_ulRemainSize = ulTotalSize;
	}

	void CopyBuffer( const char* pSourceBuffer, const unsigned long ulSourceSize )
	{
		for( unsigned long ul = 0; ul < ulSourceSize; ++ul )
		{
			m_vecBuffer.push_back( pSourceBuffer[ul] );
		}

		m_ulRemainSize -= ulSourceSize;
	}
};


