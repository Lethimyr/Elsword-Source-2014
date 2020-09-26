#pragma once
#include <map>
#include <string>
#include <vector>
#include <time.h>


//////////////////////////////////////////////////////////////////////////
// 기능 : 객체 소멸시 heap memory 공간을 초기화 시키는 string class
template< typename T >
class KClearString
{
public:
	typedef typename T::value_type			value_type;
	typedef typename T::difference_type		difference_type;
	typedef typename T::reference			reference;
	typedef typename T::iterator			iterator;

public:
	KClearString()										{}	
	KClearString( const value_type* _str )				{ m_strBuf = _str; }
	KClearString( const T& _str )						{ m_strBuf = _str; }
	~KClearString()										{ clear(); }

	const T& GetString() const							{ return m_strBuf; }
	iterator begin()									{ return m_strBuf.begin(); }
	iterator end()										{ return m_strBuf.end(); }

	reference operator[](difference_type _Off)			{ return m_strBuf[_Off]; }

	const value_type* c_str() const						{ return m_strBuf.c_str(); }
	void clear()										{ m_strBuf.resize( m_strBuf.size(), 0 ); m_strBuf.clear(); }
	int length() const									{ return static_cast<int>(m_strBuf.length()); }
	bool empty() const									{ return m_strBuf.empty(); }

	void operator=( const T& _str )						{ m_strBuf = _str; }
	void operator=( const KClearString& _str )			{ m_strBuf = _str.c_str(); }

	void operator+=( const value_type _str )			{ m_strBuf += _str; }
	void operator+=( const value_type* _str )			{ m_strBuf += _str; }
	void operator+=( const T& _str )					{ m_strBuf += _str; }
	void operator+=( KClearString& _str )				{ m_strBuf += _str.c_str(); }

private:
	T m_strBuf;
};


//////////////////////////////////////////////////////////////////////////
// 기능 : string정보를 메모리 공간에 분산시켜 외부로 노출되지 않도록함
// 특징 : 일반적인 string class에 비해 성능은 떨어짐
template< typename T >
class KSecurityString
{
public:
	typedef typename T::value_type value_type;

	struct KSentence
	{
		unsigned int	m_uiSentenceIndex;
		value_type		m_vtSentence;

		KSentence()
		{
			m_uiSentenceIndex = 0;
			m_vtSentence = 0;
		}

		KSentence( unsigned int	uiSentenceIndex, value_type vtSentence )
		{
			m_uiSentenceIndex = uiSentenceIndex;
			m_vtSentence = vtSentence;
		}
	};

public:
	KSecurityString( bool bSrand = false )
	{
		if( bSrand ) SRand();
	}

	KSecurityString( const T& strObj, bool bSrand = false )
	{
		if( bSrand ) SRand();
		Init( strObj );
	}

	~KSecurityString(void)
	{
	}

	void SRand()
	{
		srand( (unsigned) time( NULL ) );
	}

	void Init( const T& strObj )
	{
		m_mapProtectBuf.clear();

		for( unsigned int ui = 0; ui < strObj.length(); ++ui )
		{
			unsigned int randomIndex = rand() % UINT_MAX;
			while( m_mapProtectBuf.find( randomIndex ) != m_mapProtectBuf.end() )
			{
				randomIndex = rand() % UINT_MAX;
			}
			m_mapProtectBuf.insert( std::make_pair( randomIndex, KSentence( ui, strObj[ui] ) ) );
		}
	}

	bool IsEmpty() const
	{
		return m_mapProtectBuf.empty();
	}

	T GetString() const
	{
		T strTemp;
		std::map< unsigned int, value_type > mapTemp;

		{
			std::map< unsigned int, KSentence >::const_iterator mit;
			for( mit = m_mapProtectBuf.begin(); mit != m_mapProtectBuf.end(); ++mit )
			{
				mapTemp.insert( std::make_pair( mit->second.m_uiSentenceIndex, mit->second.m_vtSentence ) );
			}
		}
		{
			std::map< unsigned int, value_type >::iterator mit;
			for( mit = mapTemp.begin(); mit != mapTemp.end(); ++mit )
			{
				strTemp += mit->second;
			}
		}

		return strTemp;
	}

	KClearString< T > GetClearString() const
	{
		KClearString< T > strTemp;
		std::map< unsigned int, value_type > mapTemp;

		{
			std::map< unsigned int, KSentence >::const_iterator mit;
			for( mit = m_mapProtectBuf.begin(); mit != m_mapProtectBuf.end(); ++mit )
			{
				mapTemp.insert( std::make_pair( mit->second.m_uiSentenceIndex, mit->second.m_vtSentence ) );
			}
		}
		{
			std::map< unsigned int, value_type >::iterator mit;
			for( mit = mapTemp.begin(); mit != mapTemp.end(); ++mit )
			{
				strTemp += mit->second;
			}
		}	

		return strTemp;
	}

private:
	std::map< unsigned int, KSentence > m_mapProtectBuf;
};


