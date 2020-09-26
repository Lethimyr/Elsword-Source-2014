#pragma once
#include <list>
#include <vector>
#include <utility>


template < class T, unsigned int uiMaxNum = 5 >
class KCompareLimitList
{
public:
	KCompareLimitList()
	{
		m_uiMaxNum = uiMaxNum;
	}

	KCompareLimitList( const unsigned int uiMaxNum )
	{
		m_uiMaxNum = uiMaxNum;
	}

	~KCompareLimitList()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// 작은 값 순으로 정렬
	void IfUpperValuePushBack( int iCompareValue, const T& data )
	{
		// 비교 카운트가 작은것이면 값을 insert한다.
		std::list< std::pair< int, T > >::iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
            if( iCompareValue < lit->first )
			{
				m_listData.insert( lit, std::pair< int, T >(iCompareValue, data) );
				break;
			}
		}

		if( lit == m_listData.end() )
		{
			m_listData.push_back( std::pair< int, T >(iCompareValue, data) );
		}

		// 최대 보유하는 갯수가 될때까지 리스트의 뒷부분을 뺀다.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 큰 값 순으로 정렬
	void IfLowerValuePushBack( int iCompareValue, const T& data )
	{
		// 비교 카운트가 큰것이면 값을 insert한다.
		std::list< std::pair< int, T > >::iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
			if( iCompareValue > lit->first )
			{
				m_listData.insert( lit, std::pair< int, T >(iCompareValue, data) );
				break;
			}
		}

		if( lit == m_listData.end() )
		{
			m_listData.push_back( std::pair< int, T >(iCompareValue, data) );
		}

		// 최대 보유하는 갯수가 될때까지 리스트의 뒷부분을 뺀다.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 강제로 맨 앞에 넣기!
	void InsertFront( const T& data )
	{
		// 비교 카운트가 작은것이면 값을 insert한다.
		m_listData.push_front( std::pair< int, T >(0, data) );

		// 최대 보유하는 갯수가 될때까지 리스트의 뒷부분을 뺀다.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 강제로 맨 뒤에 넣기!
	void PushBack( const T& data )
	{
		// 비교 카운트가 작은것이면 값을 insert한다.
		m_listData.push_back( std::pair< int, T >(0, data) );

		// 최대 보유하는 갯수가 될때까지 리스트의 뒷부분을 뺀다.
		while( m_listData.size() > m_uiMaxNum )
		{
			m_listData.pop_back();
		}
	}

	const std::list< std::pair< int, T > >& GetList()	{ return m_listData; }
	unsigned int Size() const							{ return m_listData.size(); }
	unsigned int MaxSize() const						{ return m_uiMaxNum; }

	void GetValueList( std::list< T >& listData )
	{
		listData.clear();

		std::list< std::pair< int, T > >::const_iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
			listData.push_back( lit->second );
		}
	}

	void GetValueList( std::vector< T >& vecData )
	{
		vecData.clear();

		std::list< std::pair< int, T > >::const_iterator lit;
		for( lit = m_listData.begin(); lit != m_listData.end(); ++lit )
		{
			vecData.push_back( lit->second );
		}
	}

private:
	unsigned int						m_uiMaxNum;
	std::list< std::pair< int, T > >	m_listData;
};



