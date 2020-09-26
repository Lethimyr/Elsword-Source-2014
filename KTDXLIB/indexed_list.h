#pragma once


#include	<vector>

namespace	kog
{

template <typename T>
class	indexed_list
{
private:

	struct	storage_type
	{
		T			m_value;
		size_t		m_szNext;
		size_t		m_szPrev;

		storage_type() {}
		storage_type( const T& value, size_t szPrev, size_t szNext )
			: m_value(value)
			, m_szPrev(szPrev)
			, m_szNext(szNext)
		{
		}
	};//struct	value_type

public:

	class	iterator
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef T	value_type;
		typedef ptrdiff_t	difference_type;
		typedef T*	pointer;
		typedef T&	reference;

		iterator()
			: m_pContainer(NULL)
			, m_szIndex(0)
		{
		}

		iterator(indexed_list& cont,size_t szIndex)
			: m_pContainer(&cont)
			, m_szIndex(szIndex)
		{
		}

		value_type& operator*();
		value_type* operator->();
		iterator& operator++();
		const iterator operator++(int) { iterator itTemp(*this); ++(*this); return itTemp; }
		iterator& operator--();
		const iterator operator--(int) { iterator itTemp(*this); --(*this); return itTemp; }
		bool operator == ( const iterator& rhs_ ) const
		{
			return m_pContainer == rhs_.m_pContainer && m_szIndex == rhs_.m_szIndex;
		}
		bool operator != ( const iterator& rhs_ ) const
		{
			return ! this->operator == ( rhs_ );
		}

		indexed_list*	GetContainer() const { return m_pContainer; }
		size_t				GetIndex() const { return m_szIndex; }

	private:
		indexed_list*	m_pContainer;
		size_t		m_szIndex;
	};

	typedef T		value_type;
	typedef size_t	size_type;

public:

	indexed_list( size_type szNumLists = 1, size_type szReserved = 0 )
	{
		m_vecStorage.reserve( szNumLists + szReserved );
		m_vecStorage.resize( szNumLists );
		for( size_type sz = 0; sz != szNumLists; sz++ )
		{
			storage_type& storage = m_vecStorage[sz];
			storage.m_szNext = storage.m_szPrev = sz;
		}//for
	}
	~indexed_list() {}

	void		reserve( size_type szReserved )
	{
		m_vecStorage.reserve( szReserved );
	}

    size_type   storage_size() const 
    {
        return  m_vecStorage.size();
    }

	bool		empty( size_type szList = 0 )
	{
		ASSERT( szList < m_vecStorage.size() );
		storage_type& stoSentinel = m_vecStorage[szList];
		return	stoSentinel.m_szNext == szList;
	}
    size_type   size( size_type szList = 0 )
    {
		ASSERT( szList < m_vecStorage.size() );
        size_type   szSize = 0;
        for( size_type   szIndex = szList;
            szList != ( szIndex = m_vecStorage[ szIndex ].m_szNext ); 
            ++szSize);
        return szSize;
    }

	iterator	begin( size_type szList = 0)
	{
		ASSERT( szList < m_vecStorage.size() );
		return	iterator(*this, m_vecStorage[szList].m_szNext);
	}

	iterator	end( size_type szList = 0 )
	{
		ASSERT( szList < m_vecStorage.size() );
		return	iterator(*this, szList);
	}
	value_type&	back( size_type szList = 0 )
	{
		ASSERT( szList < m_vecStorage.size() );
		ASSERT( empty( szList ) == false );
		return m_vecStorage[  m_vecStorage[szList].m_szPrev ].m_value;
	}
	value_type& front( size_type szList = 0 )
	{
		ASSERT( szList < m_vecStorage.size() );
		ASSERT( empty( szList ) == false );
		return m_vecStorage[  m_vecStorage[szList].m_szNext ].m_value;
	}

	size_type	prev( size_type szPtr )
	{
		ASSERT( szPtr < m_vecStorage.size() );
		return	m_vecStorage[ szPtr ].m_szPrev;
	}

	size_type	next( size_type szPtr )
	{
		ASSERT( szPtr < m_vecStorage.size() );
		return	m_vecStorage[ szPtr ].m_szNext;
	}

	value_type&	data( size_type szPtr )
	{
		ASSERT( szPtr < m_vecStorage.size() );
		return	m_vecStorage[ szPtr ].m_value;
	}

	iterator    push_back( const value_type& value, size_type szList = 0 )
	{
		ASSERT( szList < m_vecStorage.size() );
		size_type	sz = m_vecStorage.size();
		size_type	szPrev = m_vecStorage[szList].m_szPrev;
		m_vecStorage.push_back( storage_type( value, szPrev, szList ) );
		m_vecStorage[szPrev].m_szNext = sz;
		m_vecStorage[szList].m_szPrev = sz;
		return	iterator(*this, sz);
	}


	iterator    push_back_default( size_type szList = 0 )
	{
		ASSERT( szList < m_vecStorage.size() );
		size_type	sz = m_vecStorage.size();
		size_type	szPrev = m_vecStorage[szList].m_szPrev;
		m_vecStorage.push_back( storage_type() );
		storage_type& storage = m_vecStorage.back();
		storage.m_szPrev = szPrev;
		storage.m_szNext = szList;
		m_vecStorage[szPrev].m_szNext = sz;
		m_vecStorage[szList].m_szPrev = sz;
		return	iterator(*this, sz);
	}

    bool    make_new_list( size_type szList )
    {
        if ( szList >= m_vecStorage.size() )
            return false;
		storage_type& stoPtr = m_vecStorage[szList];
        if ( stoPtr.m_szPrev == szList
            && stoPtr.m_szNext == szList )
            return true;
		{
			size_type	szPrev = stoPtr.m_szPrev;
			size_type	szNext = stoPtr.m_szNext;
			m_vecStorage[szPrev].m_szNext = szNext;
			m_vecStorage[szNext].m_szPrev = szPrev;
		}
        stoPtr.m_szPrev = stoPtr.m_szNext = szList;
        return true;
    }

	bool	splice( const iterator& iterEnd, const iterator& iterPtr )
	{
		if ( iterEnd.GetContainer() != this
			|| iterPtr.GetContainer() != this )
			return false;
		if ( iterEnd == iterPtr )
			return true;
		unsigned	szEnd = iterEnd.GetIndex();
		unsigned	szPtr = iterPtr.GetIndex();
		storage_type& stoPtr = m_vecStorage[szPtr];
		{
			size_type	szPrev = stoPtr.m_szPrev;
			size_type	szNext = stoPtr.m_szNext;
			m_vecStorage[szPrev].m_szNext = szNext;
			m_vecStorage[szNext].m_szPrev = szPrev;
		}
		storage_type& stoEnd = m_vecStorage[szEnd];
		size_type	szEndPrev = stoEnd.m_szPrev;
		storage_type& stoEndPrev = m_vecStorage[szEndPrev];
		stoEndPrev.m_szNext = szPtr;	stoPtr.m_szPrev = szEndPrev;
		stoEnd.m_szPrev = szPtr;	stoPtr.m_szNext = szEnd;
		return	true;
	}

	bool	splice( const iterator& iterEnd, size_type szPtr )
	{
		if ( iterEnd.GetContainer() != this )
            return false;
        if ( szPtr >= m_vecStorage.size() )
            return false;
		unsigned	szEnd = iterEnd.GetIndex();
		if ( szEnd == szPtr )
			return true;
		storage_type& stoPtr = m_vecStorage[szPtr];
		{
			size_type	szPrev = stoPtr.m_szPrev;
			size_type	szNext = stoPtr.m_szNext;
			m_vecStorage[szPrev].m_szNext = szNext;
			m_vecStorage[szNext].m_szPrev = szPrev;
		}
		storage_type& stoEnd = m_vecStorage[szEnd];
		size_type	szEndPrev = stoEnd.m_szPrev;
		storage_type& stoEndPrev = m_vecStorage[szEndPrev];
		stoEndPrev.m_szNext = szPtr;	stoPtr.m_szPrev = szEndPrev;
		stoEnd.m_szPrev = szPtr;	stoPtr.m_szNext = szEnd;
		return	true;
	}

	bool	splice_list( const iterator& iterEnd, size_type szList )
	{
		if ( iterEnd.GetContainer() != this )
			return false;
		size_type szEnd = iterEnd.GetIndex();
		ASSERT( szEnd < m_vecStorage.size() );
		ASSERT( szList < m_vecStorage.size() );
		storage_type&	stoSentinel = m_vecStorage[ szList ];
		size_type	szNext = stoSentinel.m_szNext;
		if ( szNext == szList )
			return true;
		size_type	szPrev = stoSentinel.m_szPrev;
		stoSentinel.m_szPrev = stoSentinel.m_szNext = szList;
		storage_type& stoEnd = m_vecStorage[szEnd];
		size_type	szEndPrev = stoEnd.m_szPrev;
		storage_type& stoEndPrev = m_vecStorage[szEndPrev];
		stoEndPrev.m_szNext = szNext;	m_vecStorage[ szNext ].m_szPrev = szEndPrev;
		stoEnd.m_szPrev = szPrev;	m_vecStorage[ szPrev ].m_szNext = szEnd;
		return true;
	}

	bool	splice_and_merge_list( const iterator& iterEnd, size_type szList )
	{
		if ( iterEnd.GetContainer() != this )
			return false;
		size_type szEnd = iterEnd.GetIndex();
		ASSERT( szEnd < m_vecStorage.size() );
		ASSERT( szList < m_vecStorage.size() );
		storage_type&	stoSentinel = m_vecStorage[ szList ];
		size_type	szNext = stoSentinel.m_szNext;
		storage_type& stoEnd = m_vecStorage[szEnd];
		size_type	szEndPrev = stoEnd.m_szPrev;
		storage_type& stoEndPrev = m_vecStorage[szEndPrev];
		stoEndPrev.m_szNext = szNext;	m_vecStorage[ szNext ].m_szPrev = szEndPrev;
		stoEnd.m_szPrev = szList;	stoSentinel.m_szNext = szEnd;
		return true;
	}

	void	swap( indexed_list<T>& rhs_ )
	{
		m_vecStorage.swap( rhs_.m_vecStorage );
	}

private:

	std::vector<storage_type>	m_vecStorage;
};

template <typename T>
inline typename indexed_list<T>::iterator::value_type& indexed_list<T>::iterator::operator*() { return m_pContainer->data(m_szIndex); }

template <typename T>
inline typename indexed_list<T>::iterator::value_type* indexed_list<T>::iterator::operator->() { return &m_pContainer->data(m_szIndex); }

template <typename T>
inline typename indexed_list<T>::iterator& indexed_list<T>::iterator::operator++() { m_szIndex = m_pContainer->next(m_szIndex); return *this; }

template <typename T>
inline typename indexed_list<T>::iterator& indexed_list<T>::iterator::operator--() { m_szIndex = m_pContainer->prev(m_szIndex); return *this; }

}